// Copyright (c) the JPEG XL Project Authors. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef LIB_JXL_ENC_XYB_H_
#define LIB_JXL_ENC_XYB_H_

// Converts to XYB color space.

#include "lib/jxl/base/compiler_specific.h"
#include "lib/jxl/base/data_parallel.h"
#include "lib/jxl/base/status.h"
#include "lib/jxl/enc_bit_writer.h"
#include "lib/jxl/image.h"
#include "lib/jxl/image_bundle.h"

namespace jxl {

// Converts any color space to XYB. If `linear` is not null, returns `linear`
// after filling it with a linear sRGB copy of `in`. Otherwise, returns `&in`.
//
// NOTE this return value can avoid an extra color conversion if `in` would
// later be passed to JxlButteraugliComparator.
const ImageBundle* ToXYB(const ImageBundle& in, ThreadPool* pool,
                         Image3F* JXL_RESTRICT xyb, const JxlCmsInterface& cms,
                         ImageBundle* JXL_RESTRICT linear = nullptr);

void Image3FToXYB(const Image3F& in, const ColorEncoding& color_encoding,
                  float intensity_target, ThreadPool* pool,
                  Image3F* JXL_RESTRICT xyb, const JxlCmsInterface& cms);

void LinearRGBRowToXYB(float* JXL_RESTRICT row0, float* JXL_RESTRICT row1,
                       float* JXL_RESTRICT row2,
                       const float* JXL_RESTRICT premul_absorb, size_t xsize);

void ComputePremulAbsorb(float intensity_target, float* premul_absorb);

// Transforms each color component of the given XYB image into the [0.0, 1.0]
// interval with an affine transform.
void ScaleXYB(Image3F* opsin);
void ScaleXYBRow(float* row0, float* row1, float* row2, size_t xsize);

// Bt.601 to match JPEG/JFIF. Outputs _signed_ YCbCr values suitable for DCT,
// see F.1.1.3 of T.81 (because our data type is float, there is no need to add
// a bias to make the values unsigned).
Status RgbToYcbcr(const ImageF& r_plane, const ImageF& g_plane,
                  const ImageF& b_plane, ImageF* y_plane, ImageF* cb_plane,
                  ImageF* cr_plane, ThreadPool* pool);

// DEPRECATED, used by opsin_image_wrapper.
Image3F OpsinDynamicsImage(const Image3B& srgb8, const JxlCmsInterface& cms);

}  // namespace jxl

#endif  // LIB_JXL_ENC_XYB_H_
