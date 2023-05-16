/*
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "bytestream.h"
#include "h2645_parse.h"
#include "vvc.h"
#include "vvc_parse.h"
#include "avcodec.h"
#include "vvcdec.c"

/*static VVCFrameContext* get_frame_context(const VVCContext *s, const VVCFrameContext *fc, const int delta)
{
    const int size = s->nb_fcs;
    const int idx = (fc - s->fcs + delta  + size) % size;
    return s->fcs + idx;
}*/

static int vvc_decode_nal_units(const uint8_t *buf, int buf_size, VVCParamSets *ps,
                                 VVCContext *s, int is_nalff, int nal_length_size,
                                 int err_recognition, int apply_defdispwin, void *logctx)
{
    int i;
    int ret = 0;
    H2645Packet pkt = { 0 };

    ret = ff_h2645_packet_split(&pkt, buf, buf_size, logctx, is_nalff,
                                nal_length_size, AV_CODEC_ID_VVC, 1, 0);
    if (ret < 0) {
        goto done;
    }

    for (i = 0; i < pkt.nb_nals; i++) {
        H2645NAL *nal = &pkt.nals[i];
        VVCFrameContext *delayed = get_frame_context(s, s->fcs, s->nb_frames - s->nb_delayed);
        GetBitContext gb = nal->gb;
        if (nal->nuh_layer_id > 0)
            continue;

        /* ignore everything except parameter sets and VCL NALUs */
        switch (nal->type) {
        case VVC_VPS_NUT:
            break;
        case VVC_SPS_NUT:
            ret = ff_vvc_decode_sps(ps, &nal->gb, apply_defdispwin, nal->nuh_layer_id, logctx);
            if (ret < 0)
                goto done;
            break;
        case VVC_PPS_NUT:
            ret = ff_vvc_decode_pps(ps, &nal->gb, logctx);
            if (ret < 0)
                goto done;
            break;
        case VVC_PH_NUT:
            ret = ff_vvc_decode_ph(ps, s->pocTid0, IS_CLVSS(s), &nal->gb, logctx);
            if (ret < 0)
                goto done;
            break;
        case VVC_TRAIL_NUT:
        case VVC_STSA_NUT:
        case VVC_RADL_NUT:
        case VVC_RASL_NUT:
        case VVC_IDR_W_RADL:
        case VVC_IDR_N_LP:
        case VVC_CRA_NUT:
        case VVC_GDR_NUT:
            ret = decode_slice(s, delayed, nal, &gb);
            if (ret < 0)
                goto done;
            break;
        case VVC_SUFFIX_APS_NUT:
        case VVC_PREFIX_APS_NUT:
            ret = ff_vvc_decode_aps(ps, &nal->gb, logctx);
            if (ret < 0)
                goto done;
            break;
        default:
            av_log(logctx, AV_LOG_VERBOSE, "Ignoring NAL type %d in extradata\n", nal->type);
            break;
        }
    }

done:
    ff_h2645_packet_uninit(&pkt);
    if (err_recognition & AV_EF_EXPLODE)
        return ret;

    return 0;
}

int ff_vvc_decode_extradata(const uint8_t *data, int size, VVCParamSets *ps,
                             VVCContext *s, int *is_nalff, int *nal_length_size,
                             int err_recognition, int apply_defdispwin, void *logctx)
{
    int ret = 0;
    GetByteContext gb;

    bytestream2_init(&gb, data, size);

    if (size > 3 && (data[0] || data[1] || data[2] > 1)) {
        /* It seems the extradata is encoded as vvcC format.
         * Temporarily, we support configurationVersion==0 until 14496-15 3rd
         * is finalized. When finalized, configurationVersion will be 1 and we
         * can recognize vvcC by checking if avctx->extradata[0]==1 or not. */
        int i, j, num_arrays, nal_len_size;

        *is_nalff = 1;

        bytestream2_skip(&gb, 21);
        nal_len_size = (bytestream2_get_byte(&gb) & 3) + 1;
        num_arrays   = bytestream2_get_byte(&gb);

        /* nal units in the vvcC always have length coded with 2 bytes,
         * so put a fake nal_length_size = 2 while parsing them */
        *nal_length_size = 2;

        /* Decode nal units from vvcC. */
        for (i = 0; i < num_arrays; i++) {
            int type = bytestream2_get_byte(&gb) & 0x3f;
            int cnt  = bytestream2_get_be16(&gb);

            for (j = 0; j < cnt; j++) {
                // +2 for the nal size field
                int nalsize = bytestream2_peek_be16(&gb) + 2;
                if (bytestream2_get_bytes_left(&gb) < nalsize) {
                    av_log(logctx, AV_LOG_ERROR,
                           "Invalid NAL unit size in extradata.\n");
                    return AVERROR_INVALIDDATA;
                }

                ret = vvc_decode_nal_units(gb.buffer, nalsize, ps, s, *is_nalff,
                                            *nal_length_size, err_recognition, apply_defdispwin,
                                            logctx);
                if (ret < 0) {
                    av_log(logctx, AV_LOG_ERROR,
                           "Decoding nal unit %d %d from vvcC failed\n",
                           type, i);
                    return ret;
                }
                bytestream2_skip(&gb, nalsize);
            }
        }

        /* Now store right nal length size, that will be used to parse
         * all other nals */
        *nal_length_size = nal_len_size;
    } else {
        *is_nalff = 0;
        ret = vvc_decode_nal_units(data, size, ps, s, *is_nalff, *nal_length_size,
                                    err_recognition, apply_defdispwin, logctx);
        if (ret < 0)
            return ret;
    }

    return ret;
}
