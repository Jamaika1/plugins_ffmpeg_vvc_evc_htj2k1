// Copyright (c) the JPEG XL Project Authors. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "lib/jxl/enc_group.h"

#include <hwy/aligned_allocator.h>
#include <utility>

#undef HWY_TARGET_INCLUDE
#define HWY_TARGET_INCLUDE "lib/jxl/enc_group.cc"
#include <hwy/foreach_target.h>
#include <hwy/highway.h>

#include "lib/jxl/ac_strategy.h"
#include "lib/jxl/base/bits.h"
#include "lib/jxl/base/compiler_specific.h"
#include "lib/jxl/base/profiler.h"
#include "lib/jxl/common.h"
#include "lib/jxl/dct_util.h"
#include "lib/jxl/dec_transforms-inl.h"
#include "lib/jxl/enc_aux_out.h"
#include "lib/jxl/enc_cache.h"
#include "lib/jxl/enc_params.h"
#include "lib/jxl/enc_transforms-inl.h"
#include "lib/jxl/image.h"
#include "lib/jxl/quantizer-inl.h"
#include "lib/jxl/quantizer.h"
HWY_BEFORE_NAMESPACE();
namespace jxl {
namespace HWY_NAMESPACE {

// These templates are not found via ADL.
using hwy::HWY_NAMESPACE::Abs;
using hwy::HWY_NAMESPACE::Ge;
using hwy::HWY_NAMESPACE::IfThenElse;
using hwy::HWY_NAMESPACE::IfThenElseZero;
using hwy::HWY_NAMESPACE::MaskFromVec;
using hwy::HWY_NAMESPACE::Round;

// NOTE: caller takes care of extracting quant from rect of RawQuantField.
void QuantizeBlockAC(const Quantizer& quantizer, const bool error_diffusion,
                     size_t c, float qm_multiplier, size_t quant_kind,
                     size_t xsize, size_t ysize,
                     const float* JXL_RESTRICT block_in, int32_t* quant,
                     int32_t* JXL_RESTRICT block_out) {
  PROFILER_FUNC;
  const float* JXL_RESTRICT qm = quantizer.InvDequantMatrix(quant_kind, c);
  float qac = quantizer.Scale() * (*quant);
  // Not SIMD-fied for now.
  float thres[4] = {0.58f, 0.66f, 0.66f, 0.7f};
  if (c == 0) {
    for (int i = 1; i < 4; ++i) {
      thres[i] = 0.71f;
    }
  }
  if (c == 2) {
    for (int i = 1; i < 4; ++i) {
      thres[i] = 0.75f;
    }
  }
  if (xsize > 1 || ysize > 1) {
    for (int i = 0; i < 4; ++i) {
      thres[i] -= Clamp1(0.003f * xsize * ysize, 0.f, (c > 0 ? 0.08f : 0.12f));
      if (thres[i] < 0.54) {
        thres[i] = 0.54;
      }
    }
  }
  if (!error_diffusion) {
    HWY_CAPPED(float, kBlockDim) df;
    HWY_CAPPED(int32_t, kBlockDim) di;
    HWY_CAPPED(uint32_t, kBlockDim) du;
    const auto quant = Set(df, qac * qm_multiplier);

    for (size_t y = 0; y < ysize * kBlockDim; y++) {
      size_t yfix = static_cast<size_t>(y >= ysize * kBlockDim / 2) * 2;
      const size_t off = y * kBlockDim * xsize;
      for (size_t x = 0; x < xsize * kBlockDim; x += Lanes(df)) {
        auto thr = Zero(df);
        if (xsize == 1) {
          HWY_ALIGN uint32_t kMask[kBlockDim] = {0,   0,   0,   0,
                                                 ~0u, ~0u, ~0u, ~0u};
          const auto mask = MaskFromVec(BitCast(df, Load(du, kMask + x)));
          thr =
              IfThenElse(mask, Set(df, thres[yfix + 1]), Set(df, thres[yfix]));
        } else {
          // Same for all lanes in the vector.
          thr = Set(
              df,
              thres[yfix + static_cast<size_t>(x >= xsize * kBlockDim / 2)]);
        }

        const auto q = Mul(Load(df, qm + off + x), quant);
        const auto in = Load(df, block_in + off + x);
        const auto val = Mul(q, in);
        const auto nzero_mask = Ge(Abs(val), thr);
        const auto v = ConvertTo(di, IfThenElseZero(nzero_mask, Round(val)));
        Store(v, di, block_out + off + x);
      }
    }
    return;
  }

retry:
  int hfNonZeros[4] = {};
  float hfError[4] = {};
  float hfMaxError[4] = {};
  size_t hfMaxErrorIx[4] = {};
  for (size_t y = 0; y < ysize * kBlockDim; y++) {
    for (size_t x = 0; x < xsize * kBlockDim; x++) {
      const size_t pos = y * kBlockDim * xsize + x;
      if (x < xsize && y < ysize) {
        // Ensure block is initialized
        block_out[pos] = 0;
        continue;
      }
      const size_t hfix = (static_cast<size_t>(y >= ysize * kBlockDim / 2) * 2 +
                           static_cast<size_t>(x >= xsize * kBlockDim / 2));
      const float val = block_in[pos] * (qm[pos] * qac * qm_multiplier);
      const float v = (std::abs(val) < thres[hfix]) ? 0 : rintf(val);
      block_out[pos] = static_cast<int32_t>(v);
      const float error = std::abs(val) - std::abs(v);
      hfError[hfix] += error * error;
      if (hfMaxError[hfix] < error) {
        hfMaxError[hfix] = error;
        hfMaxErrorIx[hfix] = pos;
      }
      if (v != 0.0f) {
        hfNonZeros[hfix] += std::abs(v);
      }
    }
  }
  if (c != 1) return;
  constexpr size_t kPartialBlockKinds =
      (1 << AcStrategy::Type::IDENTITY) | (1 << AcStrategy::Type::DCT2X2) |
      (1 << AcStrategy::Type::DCT4X4) | (1 << AcStrategy::Type::DCT4X8) |
      (1 << AcStrategy::Type::DCT8X4) | (1 << AcStrategy::Type::AFV0) |
      (1 << AcStrategy::Type::AFV1) | (1 << AcStrategy::Type::AFV2) |
      (1 << AcStrategy::Type::AFV3);
  if ((1 << quant_kind) & kPartialBlockKinds) return;
  float hfErrorLimit = 0.029f * (xsize * ysize) * kDCTBlockSize * 0.25f;
  bool goretry = false;
  for (int i = 1; i < 4; ++i) {
    if (hfError[i] >= hfErrorLimit &&
        hfNonZeros[i] <= (xsize + ysize) * 0.25f) {
      if (thres[i] >= 0.4f) {
        thres[i] -= 0.01f;
        goretry = true;
      }
    }
  }
  if (goretry) goto retry;

  for (int i = 1; i < 4; ++i) {
    if (hfError[i] >= hfErrorLimit && hfNonZeros[i] == 0) {
      const size_t pos = hfMaxErrorIx[i];
      if (hfMaxError[i] >= 0.4f) {
        block_out[pos] = block_in[pos] > 0.0f ? 1.0f : -1.0f;
      }
    }
  }
}

bool AdjustQuantBlockAC(const Quantizer& quantizer, size_t c,
                        float qm_multiplier, size_t quant_kind, size_t xsize,
                        size_t ysize, const float* JXL_RESTRICT block_in,
                        int32_t* quant) {
  // No quantization adjusting for these small blocks.
  // Quantization adjusting attempts to fix some known issues
  // with larger blocks and on the 8x8 dct's emerging 8x8 blockiness
  // when there are not many non-zeros.
  constexpr size_t kPartialBlockKinds =
      (1 << AcStrategy::Type::IDENTITY) | (1 << AcStrategy::Type::DCT2X2) |
      (1 << AcStrategy::Type::DCT4X4) | (1 << AcStrategy::Type::DCT4X8) |
      (1 << AcStrategy::Type::DCT8X4) | (1 << AcStrategy::Type::AFV0) |
      (1 << AcStrategy::Type::AFV1) | (1 << AcStrategy::Type::AFV2) |
      (1 << AcStrategy::Type::AFV3);
  if ((1 << quant_kind) & kPartialBlockKinds) return true;

  const float* JXL_RESTRICT qm = quantizer.InvDequantMatrix(quant_kind, c);
  float qac = quantizer.Scale() * (*quant);
  float thres[4] = {0.58f, 0.66f, 0.66f, 0.7f};
  if (c == 0) {
    for (int i = 1; i < 4; ++i) {
      thres[i] = 0.71f;
    }
  }
  if (c == 2) {
    for (int i = 1; i < 4; ++i) {
      thres[i] = 0.75f;
    }
  }
  if (xsize > 1 || ysize > 1) {
    for (int i = 0; i < 4; ++i) {
      thres[i] -= Clamp1(0.003f * xsize * ysize, 0.f, (c > 0 ? 0.08f : 0.12f));
      if (thres[i] < 0.54) {
        thres[i] = 0.54;
      }
    }
  }
  float sum_of_highest_freq_row_and_column = 0;
  float hfNonZeros[4] = {};
  for (size_t y = 0; y < ysize * kBlockDim; y++) {
    for (size_t x = 0; x < xsize * kBlockDim; x++) {
      const size_t pos = y * kBlockDim * xsize + x;
      if (x < xsize && y < ysize) {
        continue;
      }
      const size_t hfix = (static_cast<size_t>(y >= ysize * kBlockDim / 2) * 2 +
                           static_cast<size_t>(x >= xsize * kBlockDim / 2));
      const float val = block_in[pos] * (qm[pos] * qac * qm_multiplier);
      const float v = (std::abs(val) < thres[hfix]) ? 0 : rintf(val);
      if (v != 0.0f) {
        hfNonZeros[hfix] += std::abs(val);
        if ((y == ysize * kBlockDim - 1 || x == xsize * kBlockDim - 1) &&
            (x >= xsize * 4 && y >= ysize * 4)) {
          sum_of_highest_freq_row_and_column += std::abs(val);
        }
      }
    }
  }
  // Heuristic for improving accuracy of high-frequency patterns
  // occurring in an environment with no medium-frequency masking
  // patterns. This should be improved later to be done in X and B
  // planes too as 32x32 and larger transforms become rather ugly
  // when this is not compensated for.
  if (15 * sum_of_highest_freq_row_and_column >= hfNonZeros[0] + 1) {
    constexpr int inc = 5;
    *quant += inc;
    if (8 * sum_of_highest_freq_row_and_column >= hfNonZeros[0] + 1) {
      *quant += inc;
    }
    if (5 * sum_of_highest_freq_row_and_column >= hfNonZeros[0] + 1) {
      *quant += inc;
    }
    if (3 * sum_of_highest_freq_row_and_column >= hfNonZeros[0] + 1) {
      *quant += inc;
    }
    if (*quant >= Quantizer::kQuantMax) {
      *quant = Quantizer::kQuantMax - 1;
    }
  }
  if (quant_kind == AcStrategy::Type::DCT) {
    // If this 8x8 block is too flat, increase the adaptive quantization level
    // a bit to reduce visible block boundaries and requantize the block.
    if (hfNonZeros[0] + hfNonZeros[1] + hfNonZeros[2] + hfNonZeros[3] < 11) {
      *quant += 1;
      if (*quant >= Quantizer::kQuantMax) {
        *quant = Quantizer::kQuantMax - 1;
      }
    }
  }
  return hfNonZeros[0] > 2.0f * xsize * ysize;
}

// NOTE: caller takes care of extracting quant from rect of RawQuantField.
void QuantizeRoundtripYBlockAC(PassesEncoderState* enc_state, const size_t size,
                               const Quantizer& quantizer,
                               const bool error_diffusion, size_t quant_kind,
                               size_t xsize, size_t ysize,
                               const float* JXL_RESTRICT biases, int32_t* quant,
                               float* JXL_RESTRICT inout,
                               int32_t* JXL_RESTRICT quantized) {
  {
    int32_t max_quant = 0;
    int quant_orig = *quant;
    float val[3] = {enc_state->x_qm_multiplier, 1.0f,
                    enc_state->b_qm_multiplier};
    int clut[3] = {1, 0, 2};
    for (int ii = 0; ii < 3; ++ii) {
      int c = clut[ii];
      *quant = quant_orig;
      bool stop_adjusting =
          AdjustQuantBlockAC(quantizer, c, val[c], quant_kind, xsize, ysize,
                             inout + c * size, quant);
      max_quant = std::max(*quant, max_quant);
      if (stop_adjusting) {
        break;
      }
    }
    *quant = max_quant;
  }

  QuantizeBlockAC(quantizer, error_diffusion, 1, 1.0f, quant_kind, xsize, ysize,
                  inout + size, quant, quantized + size);

  PROFILER_ZONE("enc quant adjust bias");
  const float* JXL_RESTRICT dequant_matrix =
      quantizer.DequantMatrix(quant_kind, 1);

  HWY_CAPPED(float, kDCTBlockSize) df;
  HWY_CAPPED(int32_t, kDCTBlockSize) di;
  const auto inv_qac = Set(df, quantizer.inv_quant_ac(*quant));
  for (size_t k = 0; k < kDCTBlockSize * xsize * ysize; k += Lanes(df)) {
    const auto quant = Load(di, quantized + size + k);
    const auto adj_quant = AdjustQuantBias(di, 1, quant, biases);
    const auto dequantm = Load(df, dequant_matrix + k);
    Store(Mul(Mul(adj_quant, dequantm), inv_qac), df, inout + size + k);
  }
}

void ComputeCoefficients(size_t group_idx, PassesEncoderState* enc_state,
                         const Image3F& opsin, Image3F* dc) {
  PROFILER_FUNC;
  const Rect block_group_rect = enc_state->shared.BlockGroupRect(group_idx);
  const Rect group_rect = enc_state->shared.GroupRect(group_idx);
  const Rect cmap_rect(
      block_group_rect.x0() / kColorTileDimInBlocks,
      block_group_rect.y0() / kColorTileDimInBlocks,
      DivCeil(block_group_rect.xsize(), kColorTileDimInBlocks),
      DivCeil(block_group_rect.ysize(), kColorTileDimInBlocks));

  const size_t xsize_blocks = block_group_rect.xsize();
  const size_t ysize_blocks = block_group_rect.ysize();

  const size_t dc_stride = static_cast<size_t>(dc->PixelsPerRow());
  const size_t opsin_stride = static_cast<size_t>(opsin.PixelsPerRow());

  ImageI& full_quant_field = enc_state->shared.raw_quant_field;
  const CompressParams& cparams = enc_state->cparams;

  // TODO(veluca): consider strategies to reduce this memory.
  auto mem = hwy::AllocateAligned<int32_t>(3 * AcStrategy::kMaxCoeffArea);
  auto fmem = hwy::AllocateAligned<float>(5 * AcStrategy::kMaxCoeffArea);
  float* JXL_RESTRICT scratch_space =
      fmem.get() + 3 * AcStrategy::kMaxCoeffArea;
  {
    // Only use error diffusion in Squirrel mode or slower.
    const bool error_diffusion = cparams.speed_tier <= SpeedTier::kSquirrel;
    constexpr HWY_CAPPED(float, kDCTBlockSize) d;

    int32_t* JXL_RESTRICT coeffs[kMaxNumPasses][3] = {};
    size_t num_passes = enc_state->progressive_splitter.GetNumPasses();
    JXL_DASSERT(num_passes > 0);
    for (size_t i = 0; i < num_passes; i++) {
      // TODO(veluca): 16-bit quantized coeffs are not implemented yet.
      JXL_ASSERT(enc_state->coeffs[i]->Type() == ACType::k32);
      for (size_t c = 0; c < 3; c++) {
        coeffs[i][c] = enc_state->coeffs[i]->PlaneRow(c, group_idx, 0).ptr32;
      }
    }

    HWY_ALIGN float* coeffs_in = fmem.get();
    HWY_ALIGN int32_t* quantized = mem.get();

    size_t offset = 0;

    for (size_t by = 0; by < ysize_blocks; ++by) {
      int32_t* JXL_RESTRICT row_quant_ac =
          block_group_rect.Row(&full_quant_field, by);
      size_t ty = by / kColorTileDimInBlocks;
      const int8_t* JXL_RESTRICT row_cmap[3] = {
          cmap_rect.ConstRow(enc_state->shared.cmap.ytox_map, ty),
          nullptr,
          cmap_rect.ConstRow(enc_state->shared.cmap.ytob_map, ty),
      };
      const float* JXL_RESTRICT opsin_rows[3] = {
          group_rect.ConstPlaneRow(opsin, 0, by * kBlockDim),
          group_rect.ConstPlaneRow(opsin, 1, by * kBlockDim),
          group_rect.ConstPlaneRow(opsin, 2, by * kBlockDim),
      };
      float* JXL_RESTRICT dc_rows[3] = {
          block_group_rect.PlaneRow(dc, 0, by),
          block_group_rect.PlaneRow(dc, 1, by),
          block_group_rect.PlaneRow(dc, 2, by),
      };
      AcStrategyRow ac_strategy_row =
          enc_state->shared.ac_strategy.ConstRow(block_group_rect, by);
      for (size_t tx = 0; tx < DivCeil(xsize_blocks, kColorTileDimInBlocks);
           tx++) {
        const auto x_factor =
            Set(d, enc_state->shared.cmap.YtoXRatio(row_cmap[0][tx]));
        const auto b_factor =
            Set(d, enc_state->shared.cmap.YtoBRatio(row_cmap[2][tx]));
        for (size_t bx = tx * kColorTileDimInBlocks;
             bx < xsize_blocks && bx < (tx + 1) * kColorTileDimInBlocks; ++bx) {
          const AcStrategy acs = ac_strategy_row[bx];
          if (!acs.IsFirstBlock()) continue;

          size_t xblocks = acs.covered_blocks_x();
          size_t yblocks = acs.covered_blocks_y();

          CoefficientLayout(&yblocks, &xblocks);

          size_t size = kDCTBlockSize * xblocks * yblocks;

          // DCT Y channel, roundtrip-quantize it and set DC.
          int32_t quant_ac = row_quant_ac[bx];
          for (size_t c : {0, 1, 2}) {
            TransformFromPixels(acs.Strategy(), opsin_rows[c] + bx * kBlockDim,
                                opsin_stride, coeffs_in + c * size,
                                scratch_space);
          }
          DCFromLowestFrequencies(acs.Strategy(), coeffs_in + size,
                                  dc_rows[1] + bx, dc_stride);

          QuantizeRoundtripYBlockAC(
              enc_state, size, enc_state->shared.quantizer, error_diffusion,
              acs.RawStrategy(), xblocks, yblocks, kDefaultQuantBias, &quant_ac,
              coeffs_in, quantized);

          // Unapply color correlation
          for (size_t k = 0; k < size; k += Lanes(d)) {
            const auto in_x = Load(d, coeffs_in + k);
            const auto in_y = Load(d, coeffs_in + size + k);
            const auto in_b = Load(d, coeffs_in + 2 * size + k);
            const auto out_x = NegMulAdd(x_factor, in_y, in_x);
            const auto out_b = NegMulAdd(b_factor, in_y, in_b);
            Store(out_x, d, coeffs_in + k);
            Store(out_b, d, coeffs_in + 2 * size + k);
          }

          // Quantize X and B channels and set DC.
          for (size_t c : {0, 2}) {
            QuantizeBlockAC(enc_state->shared.quantizer, error_diffusion, c,
                            c == 0 ? enc_state->x_qm_multiplier
                                   : enc_state->b_qm_multiplier,
                            acs.RawStrategy(), xblocks, yblocks,
                            coeffs_in + c * size, &quant_ac,
                            quantized + c * size);
            DCFromLowestFrequencies(acs.Strategy(), coeffs_in + c * size,
                                    dc_rows[c] + bx, dc_stride);
          }
          row_quant_ac[bx] = quant_ac;
          enc_state->progressive_splitter.SplitACCoefficients(
              quantized, size, acs, bx, by, offset, coeffs);
          offset += size;
        }
      }
    }
  }
}

// NOLINTNEXTLINE(google-readability-namespace-comments)
}  // namespace HWY_NAMESPACE
}  // namespace jxl
HWY_AFTER_NAMESPACE();

#if HWY_ONCE
namespace jxl {
HWY_EXPORT(ComputeCoefficients);
void ComputeCoefficients(size_t group_idx, PassesEncoderState* enc_state,
                         const Image3F& opsin, Image3F* dc) {
  return HWY_DYNAMIC_DISPATCH(ComputeCoefficients)(group_idx, enc_state, opsin,
                                                   dc);
}

Status EncodeGroupTokenizedCoefficients(size_t group_idx, size_t pass_idx,
                                        size_t histogram_idx,
                                        const PassesEncoderState& enc_state,
                                        BitWriter* writer, AuxOut* aux_out) {
  // Select which histogram to use among those of the current pass.
  const size_t num_histograms = enc_state.shared.num_histograms;
  // num_histograms is 0 only for lossless.
  JXL_ASSERT(num_histograms == 0 || histogram_idx < num_histograms);
  size_t histo_selector_bits = CeilLog2Nonzero(num_histograms);

  if (histo_selector_bits != 0) {
    BitWriter::Allotment allotment(writer, histo_selector_bits);
    writer->Write(histo_selector_bits, histogram_idx);
    allotment.ReclaimAndCharge(writer, kLayerAC, aux_out);
  }
  WriteTokens(enc_state.passes[pass_idx].ac_tokens[group_idx],
              enc_state.passes[pass_idx].codes,
              enc_state.passes[pass_idx].context_map, writer, kLayerACTokens,
              aux_out);

  return true;
}

}  // namespace jxl
#endif  // HWY_ONCE
