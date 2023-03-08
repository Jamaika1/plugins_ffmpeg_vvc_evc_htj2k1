/*
 * An implementation of the TwoFish algorithm
 * Copyright (c) 2015 Supraja Meedinti
 *
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

#include <string.h>

#include "libavutil/twofish.h"
#include "libavutil/error.h"
#include "libavutil/intreadwrite.h"
#include "libavutil/mem.h"
#include "libavutil/attributes.h"

#define LR(x, n) ((x) << (n) | (x) >> (32 - (n)))
#define RR(x, n) ((x) >> (n) | (x) << (32 - (n)))

typedef struct AVTWOFISH {
    uint32_t K[40];
    uint32_t S[4];
    int ksize;
    uint32_t MDS1[256];
    uint32_t MDS2[256];
    uint32_t MDS3[256];
    uint32_t MDS4[256];
} AVTWOFISH;

static const uint8_t MD1[256] = {
    0x00, 0x5b, 0xb6, 0xed, 0x05, 0x5e, 0xb3, 0xe8, 0x0a, 0x51, 0xbc, 0xe7, 0x0f, 0x54, 0xb9, 0xe2,
    0x14, 0x4f, 0xa2, 0xf9, 0x11, 0x4a, 0xa7, 0xfc, 0x1e, 0x45, 0xa8, 0xf3, 0x1b, 0x40, 0xad, 0xf6,
    0x28, 0x73, 0x9e, 0xc5, 0x2d, 0x76, 0x9b, 0xc0, 0x22, 0x79, 0x94, 0xcf, 0x27, 0x7c, 0x91, 0xca,
    0x3c, 0x67, 0x8a, 0xd1, 0x39, 0x62, 0x8f, 0xd4, 0x36, 0x6d, 0x80, 0xdb, 0x33, 0x68, 0x85, 0xde,
    0x50, 0x0b, 0xe6, 0xbd, 0x55, 0x0e, 0xe3, 0xb8, 0x5a, 0x01, 0xec, 0xb7, 0x5f, 0x04, 0xe9, 0xb2,
    0x44, 0x1f, 0xf2, 0xa9, 0x41, 0x1a, 0xf7, 0xac, 0x4e, 0x15, 0xf8, 0xa3, 0x4b, 0x10, 0xfd, 0xa6,
    0x78, 0x23, 0xce, 0x95, 0x7d, 0x26, 0xcb, 0x90, 0x72, 0x29, 0xc4, 0x9f, 0x77, 0x2c, 0xc1, 0x9a,
    0x6c, 0x37, 0xda, 0x81, 0x69, 0x32, 0xdf, 0x84, 0x66, 0x3d, 0xd0, 0x8b, 0x63, 0x38, 0xd5, 0x8e,
    0xa0, 0xfb, 0x16, 0x4d, 0xa5, 0xfe, 0x13, 0x48, 0xaa, 0xf1, 0x1c, 0x47, 0xaf, 0xf4, 0x19, 0x42,
    0xb4, 0xef, 0x02, 0x59, 0xb1, 0xea, 0x07, 0x5c, 0xbe, 0xe5, 0x08, 0x53, 0xbb, 0xe0, 0x0d, 0x56,
    0x88, 0xd3, 0x3e, 0x65, 0x8d, 0xd6, 0x3b, 0x60, 0x82, 0xd9, 0x34, 0x6f, 0x87, 0xdc, 0x31, 0x6a,
    0x9c, 0xc7, 0x2a, 0x71, 0x99, 0xc2, 0x2f, 0x74, 0x96, 0xcd, 0x20, 0x7b, 0x93, 0xc8, 0x25, 0x7e,
    0xf0, 0xab, 0x46, 0x1d, 0xf5, 0xae, 0x43, 0x18, 0xfa, 0xa1, 0x4c, 0x17, 0xff, 0xa4, 0x49, 0x12,
    0xe4, 0xbf, 0x52, 0x09, 0xe1, 0xba, 0x57, 0x0c, 0xee, 0xb5, 0x58, 0x03, 0xeb, 0xb0, 0x5d, 0x06,
    0xd8, 0x83, 0x6e, 0x35, 0xdd, 0x86, 0x6b, 0x30, 0xd2, 0x89, 0x64, 0x3f, 0xd7, 0x8c, 0x61, 0x3a,
    0xcc, 0x97, 0x7a, 0x21, 0xc9, 0x92, 0x7f, 0x24, 0xc6, 0x9d, 0x70, 0x2b, 0xc3, 0x98, 0x75, 0x2e
};

static const uint8_t MD2[256] = {
    0x00, 0xef, 0xb7, 0x58, 0x07, 0xe8, 0xb0, 0x5f, 0x0e, 0xe1, 0xb9, 0x56, 0x09, 0xe6, 0xbe, 0x51,
    0x1c, 0xf3, 0xab, 0x44, 0x1b, 0xf4, 0xac, 0x43, 0x12, 0xfd, 0xa5, 0x4a, 0x15, 0xfa, 0xa2, 0x4d,
    0x38, 0xd7, 0x8f, 0x60, 0x3f, 0xd0, 0x88, 0x67, 0x36, 0xd9, 0x81, 0x6e, 0x31, 0xde, 0x86, 0x69,
    0x24, 0xcb, 0x93, 0x7c, 0x23, 0xcc, 0x94, 0x7b, 0x2a, 0xc5, 0x9d, 0x72, 0x2d, 0xc2, 0x9a, 0x75,
    0x70, 0x9f, 0xc7, 0x28, 0x77, 0x98, 0xc0, 0x2f, 0x7e, 0x91, 0xc9, 0x26, 0x79, 0x96, 0xce, 0x21,
    0x6c, 0x83, 0xdb, 0x34, 0x6b, 0x84, 0xdc, 0x33, 0x62, 0x8d, 0xd5, 0x3a, 0x65, 0x8a, 0xd2, 0x3d,
    0x48, 0xa7, 0xff, 0x10, 0x4f, 0xa0, 0xf8, 0x17, 0x46, 0xa9, 0xf1, 0x1e, 0x41, 0xae, 0xf6, 0x19,
    0x54, 0xbb, 0xe3, 0x0c, 0x53, 0xbc, 0xe4, 0x0b, 0x5a, 0xb5, 0xed, 0x02, 0x5d, 0xb2, 0xea, 0x05,
    0xe0, 0x0f, 0x57, 0xb8, 0xe7, 0x08, 0x50, 0xbf, 0xee, 0x01, 0x59, 0xb6, 0xe9, 0x06, 0x5e, 0xb1,
    0xfc, 0x13, 0x4b, 0xa4, 0xfb, 0x14, 0x4c, 0xa3, 0xf2, 0x1d, 0x45, 0xaa, 0xf5, 0x1a, 0x42, 0xad,
    0xd8, 0x37, 0x6f, 0x80, 0xdf, 0x30, 0x68, 0x87, 0xd6, 0x39, 0x61, 0x8e, 0xd1, 0x3e, 0x66, 0x89,
    0xc4, 0x2b, 0x73, 0x9c, 0xc3, 0x2c, 0x74, 0x9b, 0xca, 0x25, 0x7d, 0x92, 0xcd, 0x22, 0x7a, 0x95,
    0x90, 0x7f, 0x27, 0xc8, 0x97, 0x78, 0x20, 0xcf, 0x9e, 0x71, 0x29, 0xc6, 0x99, 0x76, 0x2e, 0xc1,
    0x8c, 0x63, 0x3b, 0xd4, 0x8b, 0x64, 0x3c, 0xd3, 0x82, 0x6d, 0x35, 0xda, 0x85, 0x6a, 0x32, 0xdd,
    0xa8, 0x47, 0x1f, 0xf0, 0xaf, 0x40, 0x18, 0xf7, 0xa6, 0x49, 0x11, 0xfe, 0xa1, 0x4e, 0x16, 0xf9,
    0xb4, 0x5b, 0x03, 0xec, 0xb3, 0x5c, 0x04, 0xeb, 0xba, 0x55, 0x0d, 0xe2, 0xbd, 0x52, 0x0a, 0xe5
};

static const uint8_t q0[256] = {
    0xa9, 0x67, 0xb3, 0xe8, 0x04, 0xfd, 0xa3, 0x76, 0x9a, 0x92, 0x80, 0x78, 0xe4, 0xdd, 0xd1, 0x38,
    0x0d, 0xc6, 0x35, 0x98, 0x18, 0xf7, 0xec, 0x6c, 0x43, 0x75, 0x37, 0x26, 0xfa, 0x13, 0x94, 0x48,
    0xf2, 0xd0, 0x8b, 0x30, 0x84, 0x54, 0xdf, 0x23, 0x19, 0x5b, 0x3d, 0x59, 0xf3, 0xae, 0xa2, 0x82,
    0x63, 0x01, 0x83, 0x2e, 0xd9, 0x51, 0x9b, 0x7c, 0xa6, 0xeb, 0xa5, 0xbe, 0x16, 0x0c, 0xe3, 0x61,
    0xc0, 0x8c, 0x3a, 0xf5, 0x73, 0x2c, 0x25, 0x0b, 0xbb, 0x4e, 0x89, 0x6b, 0x53, 0x6a, 0xb4, 0xf1,
    0xe1, 0xe6, 0xbd, 0x45, 0xe2, 0xf4, 0xb6, 0x66, 0xcc, 0x95, 0x03, 0x56, 0xd4, 0x1c, 0x1e, 0xd7,
    0xfb, 0xc3, 0x8e, 0xb5, 0xe9, 0xcf, 0xbf, 0xba, 0xea, 0x77, 0x39, 0xaf, 0x33, 0xc9, 0x62, 0x71,
    0x81, 0x79, 0x09, 0xad, 0x24, 0xcd, 0xf9, 0xd8, 0xe5, 0xc5, 0xb9, 0x4d, 0x44, 0x08, 0x86, 0xe7,
    0xa1, 0x1d, 0xaa, 0xed, 0x06, 0x70, 0xb2, 0xd2, 0x41, 0x7b, 0xa0, 0x11, 0x31, 0xc2, 0x27, 0x90,
    0x20, 0xf6, 0x60, 0xff, 0x96, 0x5c, 0xb1, 0xab, 0x9e, 0x9c, 0x52, 0x1b, 0x5f, 0x93, 0x0a, 0xef,
    0x91, 0x85, 0x49, 0xee, 0x2d, 0x4f, 0x8f, 0x3b, 0x47, 0x87, 0x6d, 0x46, 0xd6, 0x3e, 0x69, 0x64,
    0x2a, 0xce, 0xcb, 0x2f, 0xfc, 0x97, 0x05, 0x7a, 0xac, 0x7f, 0xd5, 0x1a, 0x4b, 0x0e, 0xa7, 0x5a,
    0x28, 0x14, 0x3f, 0x29, 0x88, 0x3c, 0x4c, 0x02, 0xb8, 0xda, 0xb0, 0x17, 0x55, 0x1f, 0x8a, 0x7d,
    0x57, 0xc7, 0x8d, 0x74, 0xb7, 0xc4, 0x9f, 0x72, 0x7e, 0x15, 0x22, 0x12, 0x58, 0x07, 0x99, 0x34,
    0x6e, 0x50, 0xde, 0x68, 0x65, 0xbc, 0xdb, 0xf8, 0xc8, 0xa8, 0x2b, 0x40, 0xdc, 0xfe, 0x32, 0xa4,
    0xca, 0x10, 0x21, 0xf0, 0xd3, 0x5d, 0x0f, 0x00, 0x6f, 0x9d, 0x36, 0x42, 0x4a, 0x5e, 0xc1, 0xe0
};

static const uint8_t q1[256] = {
    0x75, 0xf3, 0xc6, 0xf4, 0xdb, 0x7b, 0xfb, 0xc8, 0x4a, 0xd3, 0xe6, 0x6b, 0x45, 0x7d, 0xe8, 0x4b,
    0xd6, 0x32, 0xd8, 0xfd, 0x37, 0x71, 0xf1, 0xe1, 0x30, 0x0f, 0xf8, 0x1b, 0x87, 0xfa, 0x06, 0x3f,
    0x5e, 0xba, 0xae, 0x5b, 0x8a, 0x00, 0xbc, 0x9d, 0x6d, 0xc1, 0xb1, 0x0e, 0x80, 0x5d, 0xd2, 0xd5,
    0xa0, 0x84, 0x07, 0x14, 0xb5, 0x90, 0x2c, 0xa3, 0xb2, 0x73, 0x4c, 0x54, 0x92, 0x74, 0x36, 0x51,
    0x38, 0xb0, 0xbd, 0x5a, 0xfc, 0x60, 0x62, 0x96, 0x6c, 0x42, 0xf7, 0x10, 0x7c, 0x28, 0x27, 0x8c,
    0x13, 0x95, 0x9c, 0xc7, 0x24, 0x46, 0x3b, 0x70, 0xca, 0xe3, 0x85, 0xcb, 0x11, 0xd0, 0x93, 0xb8,
    0xa6, 0x83, 0x20, 0xff, 0x9f, 0x77, 0xc3, 0xcc, 0x03, 0x6f, 0x08, 0xbf, 0x40, 0xe7, 0x2b, 0xe2,
    0x79, 0x0c, 0xaa, 0x82, 0x41, 0x3a, 0xea, 0xb9, 0xe4, 0x9a, 0xa4, 0x97, 0x7e, 0xda, 0x7a, 0x17,
    0x66, 0x94, 0xa1, 0x1d, 0x3d, 0xf0, 0xde, 0xb3, 0x0b, 0x72, 0xa7, 0x1c, 0xef, 0xd1, 0x53, 0x3e,
    0x8f, 0x33, 0x26, 0x5f, 0xec, 0x76, 0x2a, 0x49, 0x81, 0x88, 0xee, 0x21, 0xc4, 0x1a, 0xeb, 0xd9,
    0xc5, 0x39, 0x99, 0xcd, 0xad, 0x31, 0x8b, 0x01, 0x18, 0x23, 0xdd, 0x1f, 0x4e, 0x2d, 0xf9, 0x48,
    0x4f, 0xf2, 0x65, 0x8e, 0x78, 0x5c, 0x58, 0x19, 0x8d, 0xe5, 0x98, 0x57, 0x67, 0x7f, 0x05, 0x64,
    0xaf, 0x63, 0xb6, 0xfe, 0xf5, 0xb7, 0x3c, 0xa5, 0xce, 0xe9, 0x68, 0x44, 0xe0, 0x4d, 0x43, 0x69,
    0x29, 0x2e, 0xac, 0x15, 0x59, 0xa8, 0x0a, 0x9e, 0x6e, 0x47, 0xdf, 0x34, 0x35, 0x6a, 0xcf, 0xdc,
    0x22, 0xc9, 0xc0, 0x9b, 0x89, 0xd4, 0xed, 0xab, 0x12, 0xa2, 0x0d, 0x52, 0xbb, 0x02, 0x2f, 0xa9,
    0xd7, 0x61, 0x1e, 0xb4, 0x50, 0x04, 0xf6, 0xc2, 0x16, 0x25, 0x86, 0x56, 0x55, 0x09, 0xbe, 0x91
};

struct AVTWOFISH *av_twofish_alloc(void)
{
    return av_mallocz(sizeof(struct AVTWOFISH));
}

const int av_twofish_size = sizeof(AVTWOFISH);

static uint8_t gfmul(uint8_t a, uint8_t b)
{
    uint8_t r = 0, t;
    while (a && b) {
        if (a & 1)
            r = r ^ b;
        t = b & 0x80;
        b = b << 1;
        if (t)
            b = b ^ 0x4d;
        a = a >> 1;
    }
    return r;
}

static uint32_t tf_RS(uint32_t k0, uint32_t k1)
{
    uint8_t s[4], m[8];
    AV_WL32(m, k0);
    AV_WL32(m + 4, k1);
    s[0] = gfmul(0x01, m[0]) ^ gfmul(0xa4, m[1]) ^ gfmul(0x55, m[2]) ^ gfmul(0x87, m[3]) ^ gfmul(0x5a, m[4]) ^ gfmul(0x58, m[5]) ^ gfmul(0xdb, m[6]) ^ gfmul(0x9e, m[7]);
    s[1] = gfmul(0xa4, m[0]) ^ gfmul(0x56, m[1]) ^ gfmul(0x82, m[2]) ^ gfmul(0xf3, m[3]) ^ gfmul(0x1e, m[4]) ^ gfmul(0xc6, m[5]) ^ gfmul(0x68, m[6]) ^ gfmul(0xe5, m[7]);
    s[2] = gfmul(0x02, m[0]) ^ gfmul(0xa1, m[1]) ^ gfmul(0xfc, m[2]) ^ gfmul(0xc1, m[3]) ^ gfmul(0x47, m[4]) ^ gfmul(0xae, m[5]) ^ gfmul(0x3d, m[6]) ^ gfmul(0x19, m[7]);
    s[3] = gfmul(0xa4, m[0]) ^ gfmul(0x55, m[1]) ^ gfmul(0x87, m[2]) ^ gfmul(0x5a, m[3]) ^ gfmul(0x58, m[4]) ^ gfmul(0xdb, m[5]) ^ gfmul(0x9e, m[6]) ^ gfmul(0x03, m[7]);
    return AV_RL32(s);
}

static void tf_h0(uint8_t y[4], uint32_t L[4], int k)
{
    uint8_t l[4];
    if (k == 4) {
        AV_WL32(l, L[3]);
        y[0] = q1[y[0]] ^ l[0];
        y[1] = q0[y[1]] ^ l[1];
        y[2] = q0[y[2]] ^ l[2];
        y[3] = q1[y[3]] ^ l[3];
    }
    if (k >= 3) {
        AV_WL32(l, L[2]);
        y[0] = q1[y[0]] ^ l[0];
        y[1] = q1[y[1]] ^ l[1];
        y[2] = q0[y[2]] ^ l[2];
        y[3] = q0[y[3]] ^ l[3];
    }
    AV_WL32(l, L[1]);
    y[0] = q1[q0[q0[y[0]] ^ l[0]] ^ (L[0] & 0xff)];
    y[1] = q0[q0[q1[y[1]] ^ l[1]] ^ ((L[0] >> 8) & 0xff)];
    y[2] = q1[q1[q0[y[2]] ^ l[2]] ^ ((L[0] >> 16) & 0xff)];
    y[3] = q0[q1[q1[y[3]] ^ l[3]] ^ (L[0] >> 24)];
}

static uint32_t tf_h(uint32_t X, uint32_t L[4], int k)
{
    uint8_t y[4], l[4];
    AV_WL32(y, X);
    tf_h0(y, L, k);

    l[0] = y[0] ^ MD2[y[1]] ^ MD1[y[2]] ^ MD1[y[3]];
    l[1] = MD1[y[0]] ^ MD2[y[1]] ^ MD2[y[2]] ^ y[3];
    l[2] = MD2[y[0]] ^ MD1[y[1]] ^ y[2] ^ MD2[y[3]];
    l[3] = MD2[y[0]] ^ y[1] ^ MD2[y[2]] ^ MD1[y[3]];

    return AV_RL32(l);
}

static uint32_t MDS_mul(AVTWOFISH *cs, uint32_t X)
{
    return cs->MDS1[(X) & 0xff] ^ cs->MDS2[((X) >> 8) & 0xff] ^ cs->MDS3[((X) >> 16) & 0xff] ^ cs->MDS4[(X) >> 24];
}

static void precomputeMDS(AVTWOFISH *cs)
{
    uint8_t y[4];
    int i;
    for (i = 0; i < 256; i++) {
        y[0] = y[1] = y[2] = y[3] = i;
    tf_h0(y, cs->S, cs->ksize);
        cs->MDS1[i] = ((uint32_t)y[0]) ^ ((uint32_t)MD1[y[0]] << 8) ^ ((uint32_t)MD2[y[0]] << 16) ^ ((uint32_t)MD2[y[0]] << 24);
        cs->MDS2[i] = ((uint32_t)MD2[y[1]]) ^ ((uint32_t)MD2[y[1]] << 8) ^ ((uint32_t)MD1[y[1]] << 16) ^ ((uint32_t)y[1] << 24);
        cs->MDS3[i] = ((uint32_t)MD1[y[2]]) ^ ((uint32_t)MD2[y[2]] << 8) ^ ((uint32_t)y[2] << 16) ^ ((uint32_t)MD2[y[2]] << 24);
        cs->MDS4[i] = ((uint32_t)MD1[y[3]]) ^ ((uint32_t)y[3] << 8) ^ ((uint32_t)MD2[y[3]] << 16) ^ ((uint32_t)MD1[y[3]] << 24);
    }
}

static void twofish_encrypt(AVTWOFISH *cs, uint8_t *dst, const uint8_t *src)
{
    uint32_t P[4], t0, t1;
    int i;
    P[0] = AV_RL32(src) ^ cs->K[0];
    P[1] = AV_RL32(src + 4) ^ cs->K[1];
    P[2] = AV_RL32(src + 8) ^ cs->K[2];
    P[3] = AV_RL32(src + 12) ^ cs->K[3];
    for (i = 0; i < 16; i += 2) {
        t0 = MDS_mul(cs, P[0]);
        t1 = MDS_mul(cs, LR(P[1], 8));
        P[2] = RR(P[2] ^ (t0 + t1 + cs->K[2 * i + 8]), 1);
        P[3] = LR(P[3], 1) ^ (t0 + 2 * t1 + cs->K[2 * i + 9]);
        t0 = MDS_mul(cs, P[2]);
        t1 = MDS_mul(cs, LR(P[3], 8));
        P[0] = RR(P[0] ^ (t0 + t1 + cs->K[2 * i + 10]), 1);
        P[1] = LR(P[1], 1) ^ (t0 + 2 * t1 + cs->K[2 * i + 11]);
    }
    P[2] ^= cs->K[4];
    P[3] ^= cs->K[5];
    P[0] ^= cs->K[6];
    P[1] ^= cs->K[7];
    AV_WL32(dst, P[2]);
    AV_WL32(dst + 4, P[3]);
    AV_WL32(dst + 8, P[0]);
    AV_WL32(dst + 12, P[1]);
}

static void twofish_decrypt(AVTWOFISH *cs, uint8_t *dst, const uint8_t *src, uint8_t *iv)
{
    uint32_t P[4], t0, t1;
    int i;
    P[2] = AV_RL32(src) ^ cs->K[4];
    P[3] = AV_RL32(src + 4) ^ cs->K[5];
    P[0] = AV_RL32(src + 8) ^ cs->K[6];
    P[1] = AV_RL32(src + 12) ^ cs->K[7];
    for (i = 15; i >= 0; i -= 2) {
        t0 = MDS_mul(cs, P[2]);
        t1 = MDS_mul(cs, LR(P[3], 8));
        P[0] = LR(P[0], 1) ^ (t0 + t1 + cs->K[2 * i + 8]);
        P[1] = RR(P[1] ^ (t0 + 2 * t1 + cs->K[2 * i + 9]), 1);
        t0 = MDS_mul(cs, P[0]);
        t1 = MDS_mul(cs, LR(P[1], 8));
        P[2] = LR(P[2], 1) ^ (t0 + t1 + cs->K[2 * i + 6]);
        P[3] = RR(P[3] ^ (t0 + 2 * t1 + cs->K[2 * i + 7]), 1);
    }
    P[0] ^= cs->K[0];
    P[1] ^= cs->K[1];
    P[2] ^= cs->K[2];
    P[3] ^= cs->K[3];
    if (iv) {
        P[0] ^= AV_RL32(iv);
        P[1] ^= AV_RL32(iv + 4);
        P[2] ^= AV_RL32(iv + 8);
        P[3] ^= AV_RL32(iv + 12);
        memcpy(iv, src, 16);
    }
    AV_WL32(dst, P[0]);
    AV_WL32(dst + 4, P[1]);
    AV_WL32(dst + 8, P[2]);
    AV_WL32(dst + 12, P[3]);
}

av_cold int av_twofish_init(AVTWOFISH *cs, const uint8_t *key, int key_bits)
{
    int i;
    uint8_t keypad[32];
    uint32_t Key[8], Me[4], Mo[4], A, B;
    const uint32_t rho = 0x01010101;
    if (key_bits < 0)
        return AVERROR(EINVAL);
    if (key_bits <= 128) {
        cs->ksize = 2;
    } else if (key_bits <= 192) {
        cs->ksize = 3;
    } else {
        cs->ksize = 4;
    }
    memset(keypad, 0, sizeof(keypad));
    if (key_bits <= 256) {
        memcpy(keypad, key, key_bits >> 3);
    } else {
        memcpy(keypad, key, 32);
    }
    for (i = 0; i < 2 * cs->ksize ; i++)
        Key[i] = AV_RL32(keypad + 4 * i);
    for (i = 0; i < cs->ksize; i++) {
        Me[i] = Key[2 * i];
        Mo[i] = Key[2 * i + 1];
        cs->S[cs->ksize - i - 1] = tf_RS(Me[i], Mo[i]);
    }
    precomputeMDS(cs);
    for (i = 0; i < 20; i++) {
        A = tf_h((2 * i) * rho, Me, cs->ksize);
        B = tf_h((2 * i + 1) * rho, Mo, cs->ksize);
        B = LR(B, 8);
        cs->K[2 * i] = A + B;
        cs->K[2 * i + 1] = LR((A + (2 * B)), 9);
    }
    if (cs->ksize << 6 != key_bits) {
        return 1;
    } else {
        return 0;
    }
}

void av_twofish_crypt(AVTWOFISH *cs, uint8_t *dst, const uint8_t *src, int count, uint8_t *iv, int decrypt)
{
    int i;
    while (count--) {
        if (decrypt) {
            twofish_decrypt(cs, dst, src, iv);
        } else {
            if (iv) {
                for (i = 0; i < 16; i++)
                    dst[i] = src[i] ^ iv[i];
                twofish_encrypt(cs, dst, dst);
                memcpy(iv, dst, 16);
            } else {
                twofish_encrypt(cs, dst, src);
            }
        }
        src = src + 16;
        dst = dst + 16;
    }
}