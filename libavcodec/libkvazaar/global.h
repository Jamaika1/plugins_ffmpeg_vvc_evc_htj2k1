#ifndef GLOBAL_H_
#define GLOBAL_H_
/*****************************************************************************
 * This file is part of Kvazaar HEVC encoder.
 *
 * Copyright (c) 2021, Tampere University, ITU/ISO/IEC, project contributors
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * * Neither the name of the Tampere University or ITU/ISO/IEC nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * INCLUDING NEGLIGENCE OR OTHERWISE ARISING IN ANY WAY OUT OF THE USE OF THIS
 ****************************************************************************/

/**
 * \file
 * Header that is included in every other header.
 *
 * This file contains global constants that can be referred to from any header
 * or source file. It also contains some helper macros and includes stdint.h
 * so that any file can refer to integer types with exact widths.
 */

#ifdef HAVE_CONFIG_H
// Include config.h generated by automake. This needs to be before any other
// includes in every file, which is why it's in global.
//#include "config.h" // IWYU pragma: export
#endif

// Include some basics in all files, like assert, primitives and NULL.
// If you add anything to this list with export pragma, think long and
// and hard if it's actually a good idea to incude it for every c-file.
#include <assert.h> // IWYU pragma: export
#include <stdbool.h> // IWYU pragma: export
#include <stdint.h> // IWYU pragma: export
#include <stddef.h> // IWYU pragma: export

// The stdlib.h and string.h headers are needed because of MALLOC and FILL
// macros defined here, as IWYU will remove them from files that use only
// those macros.
#include <stdlib.h>
#include <string.h>

/**
 * \defgroup Bitstream
 * HEVC bitstream coding
 *
 * \defgroup CABAC
 * Context Adaptive Binary Arithmetic Encoder implementation
 *
 * \defgroup Compression
 * Prediction parameter decisions and ratedistortion optimization
 *
 * \defgroup Control
 * Initialization and control flow of the encoder
 *
 * \defgroup DataStructures
 * Containers for images, predictions parameters and such
 *
 * \defgroup Extras
 * 3rd party modules not considered part of the encoder.
 *
 * \defgroup Optimization
 * Architecture dependant SIMD optimizations and dynamic dispatch mechanism
 *
 * \defgroup Reconstruction
 * Stuff required for creating the resulting image after lossy compression
 *
 * \defgroup Threading
 * Stuff related to multi-threading using pthreads
 */

 // Pthreads-win32 tries to define timespec even if it has already been defined.
 // In Visual Studio 2015 timespec is defined in time.h so we may need to define
 // HAVE_STRUCT_TIMESPEC.
#if _MSC_VER >= 1900 && !defined(HAVE_STRUCT_TIMESPEC)
#   define HAVE_STRUCT_TIMESPEC
#endif

#if defined(_MSC_VER) && defined(_M_AMD64)
  #define X86_64
#endif

#if defined(__GNUC__) && defined(__x86_64__)
  #define X86_64
#endif

#define PIXEL_MIN 0
#define PIXEL_MAX ((1 << KVZ_BIT_DEPTH) - 1)

typedef int16_t coeff_t;

// #define VERBOSE 1

/* CONFIG VARIABLES */

//spec: references to variables defined in Rec. ITU-T H.265 (04/2013)

//! Limits for prediction block sizes. 0 = 64x64, 4 = 4x4.
#define PU_DEPTH_INTER_MIN 0
#define PU_DEPTH_INTER_MAX 3
#define PU_DEPTH_INTRA_MIN 0
#define PU_DEPTH_INTRA_MAX 4

//! Maximum number of layers in GOP structure (for allocating structures)
#define MAX_GOP_LAYERS 6

//! Maximum CU depth when descending form LCU level.
//! spec: log2_diff_max_min_luma_coding_block_size
#define MAX_DEPTH 3
//! Minimum log2 size of CUs.
//! spec: MinCbLog2SizeY
#define MIN_SIZE 3
//! Minimum log2 size of PUs.
//! Search is started at depth 0 and goes in Z-order to MAX_PU_DEPTH, see search_cu()
#define MAX_PU_DEPTH 4

//! spec: pcm_enabled_flag, Setting to 1 will enable using PCM blocks (current intra-search does not consider PCM)
#define ENABLE_PCM 0

//! skip residual coding when it's under _some_ threshold
#define OPTIMIZATION_SKIP_RESIDUAL_ON_THRESHOLD 0

/* END OF CONFIG VARIABLES */

//! pow(2, MIN_SIZE)
#define CU_MIN_SIZE_PIXELS (1 << MIN_SIZE)
//! spec: CtbSizeY
#define LCU_WIDTH (1 << (MIN_SIZE + MAX_DEPTH))
//! spec: CtbWidthC and CtbHeightC
#define LCU_WIDTH_C (LCU_WIDTH / 2)

//! spec: Log2MaxTrafoSize <= Min(CtbLog2SizeY, 5)
#define TR_MAX_LOG2_SIZE 5
#define TR_MAX_WIDTH (1 << TR_MAX_LOG2_SIZE)
//! spec: Log2MinTrafoSize
#define TR_MIN_LOG2_SIZE 2
#define TR_MIN_WIDTH (1 << TR_MIN_LOG2_SIZE)

#if LCU_WIDTH != 64
  #error "Kvazaar only support LCU_WIDTH == 64"
#endif

#define LCU_LUMA_SIZE (LCU_WIDTH * LCU_WIDTH)
#define LCU_CHROMA_SIZE (LCU_WIDTH * LCU_WIDTH >> 2)

/**
 * \brief Number of pixels to delay deblocking.
 *
 * Number of pixels at the bottom and right side of the LCU that are not
 * deblocked until when filtering the neighboring LCU. The last four chroma
 * pixels of the horizontal edges within the LCU are deblocked with the LCU
 * to the right. Therefore, DEBLOCK_DELAY_PX is set to 8 pixels.
 */
#define DEBLOCK_DELAY_PX 8

/**
 * \brief Number of pixels to delay SAO in horizontal and vertical
 * directions.
 *
 * Number of pixels at the bottom and right side of the LCU that are not
 * filtered with SAO until when filtering the neighboring LCU. SAO
 * reconstruction requires that a one pixels border has been deblocked for
 * both luma and chroma.  Therefore, SAO_DELAY_PX is set to
 * DEBLOCK_DELAY_PX + 2.
 */
#define SAO_DELAY_PX (DEBLOCK_DELAY_PX + 2)

#define MAX_REF_PIC_COUNT 16

#define AMVP_MAX_NUM_CANDS 2
#define AMVP_MAX_NUM_CANDS_MEM 3
#define MRG_MAX_NUM_CANDS 5

/* Some tools */
#define ABS(a) ((a) >= 0 ? (a) : (-a))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))
#define CLIP(low,high,value) MAX((low),MIN((high),(value)))
#define CLIP_TO_PIXEL(value) CLIP(0, PIXEL_MAX, (value))
#define CLIP_TO_QP(value) CLIP(0, 51, (value))
#define SWAP(a,b,swaptype) { swaptype tempval; tempval = a; a = b; b = tempval; }
#define CU_WIDTH_FROM_DEPTH(depth) (LCU_WIDTH >> depth)
#define WITHIN(val, min_val, max_val) ((min_val) <= (val) && (val) <= (max_val))
#define CEILDIV(x,y) (((x) + (y) - 1) / (y))

#define LOG2_LCU_WIDTH 6
// CU_TO_PIXEL = y * lcu_width * pic_width + x * lcu_width
#define CU_TO_PIXEL(x, y, depth, stride) (((y) << (LOG2_LCU_WIDTH - (depth))) * (stride) \
                                         + ((x) << (LOG2_LCU_WIDTH - (depth))))
//#define SIGN3(x) ((x) > 0) ? +1 : ((x) == 0 ? 0 : -1)
#define SIGN3(x) (((x) > 0) - ((x) < 0))


#define QUOTE(x) #x
#define QUOTE_EXPAND(x) QUOTE(x)

// NOTE: When making a release, check to see if incrementing libversion in
// configure.ac is necessary.
#ifndef KVZ_VERSION
#define KVZ_VERSION 2.2.0-91f04b4
#endif
#define VERSION_STRING QUOTE_EXPAND(KVZ_VERSION)

//#define VERBOSE 1

#define SAO_ABS_OFFSET_MAX ((1 << (MIN(KVZ_BIT_DEPTH, 10) - 5)) - 1)

#define MAX_TILES_PER_DIM 48
#define MAX_SLICES 16

/* Inlining functions */
#ifdef _MSC_VER /* Visual studio */
  #define INLINE __forceinline
  #pragma inline_recursion(on)
#else /* others */
  #define INLINE inline
#endif

// Return the next aligned address for *p. Result is at most alignment larger than p.
#define ALIGNED_POINTER(p, alignment) (void*)((intptr_t)(p) + (alignment) - ((intptr_t)(p) % (alignment)))
// 32 bytes is enough for AVX2
#define SIMD_ALIGNMENT 32

#ifdef _MSC_VER
  #define ALIGNED(alignment) __declspec(align(alignment))
#else
  #define ALIGNED(alignment) __attribute__((aligned (alignment)))
#endif

#ifdef _MSC_VER
// Buggy VS2010 throws intellisense warnings if void* is not casted.
  #define MALLOC(type, num) (type *)malloc(sizeof(type) * (num))
  #define MALLOC_SIMD_PADDED(type, num, padding) (type *)malloc(sizeof(type) * (num) + (padding))
#else
  #define MALLOC(type, num) malloc(sizeof(type) * (num))
  #define MALLOC_SIMD_PADDED(type, num, padding) malloc(sizeof(type) * (num) + (padding))
#endif

// Use memset through FILL and FILL_ARRAY when appropriate, such as when
// initializing whole structures or arrays. It's still ok to use memset
// directly when doing something more complicated.

// Fill a structure or a static array with val bytes.
#define FILL(var, val) memset(&(var), (val), sizeof(var))
// Fill a number of elements in an array with val bytes.
#define FILL_ARRAY(ar, val, size) \
{\
  void *temp_ptr = (void*)(ar);\
  memset((temp_ptr), (val), (size) * sizeof(*(ar)));\
}

#define FREE_POINTER(pointer) { free((void*)pointer); pointer = NULL; }
#define MOVE_POINTER(dst_pointer,src_pointer) { dst_pointer = src_pointer; src_pointer = NULL; }

#ifndef MAX_INT
#define MAX_INT 0x7FFFFFFF
#endif
#ifndef MAX_INT64
#define MAX_INT64 0x7FFFFFFFFFFFFFFFLL
#endif
#ifndef MAX_DOUBLE
#define MAX_DOUBLE 1.7e+308
#endif

//For transform.h and encoder.h
#define SCALING_LIST_4x4      0
#define SCALING_LIST_8x8      1
#define SCALING_LIST_16x16    2
#define SCALING_LIST_32x32    3
#define SCALING_LIST_SIZE_NUM 4
#define SCALING_LIST_NUM      6
#define MAX_MATRIX_COEF_NUM   64
#define SCALING_LIST_REM_NUM  6

#define MAX_TR_DYNAMIC_RANGE 15

//Constants
typedef enum { COLOR_Y = 0, COLOR_U, COLOR_V } color_t;


// Hardware data (abstraction of defines). Extend for other compilers
#if defined(_M_IX86) || defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__) || defined(_M_X64) || defined(_M_AMD64) || defined(__amd64__) || defined(__x86_64__)
#  define COMPILE_INTEL 1
#else
#  define COMPILE_INTEL 0
#endif

// Visual Studio note:
// Because these macros are only used to guard code that is guarded by CPUID
// at runtime, use /arch parameter to disable them, but enable all intrinsics
// supported by VisualStudio if SSE2 (highest) is enabled.
// AVX and AVX2 are handled by /arch directly and sse intrinsics will use VEX
// versions if they are defined.
#define MSC_X86_SIMD(level) (_M_X64 || (_M_IX86_FP >= (level)))

#if COMPILE_INTEL
#  if defined(__MMX__) || MSC_X86_SIMD(1)
#    define COMPILE_INTEL_MMX 1
#  endif
#  if defined(__SSE__) || MSC_X86_SIMD(1)
#    define COMPILE_INTEL_SSE 1
#  endif
#  if defined(__SSE2__) || MSC_X86_SIMD(2)
#    define COMPILE_INTEL_SSE2 1
#  endif
#  if defined(__SSE3__)
#    define COMPILE_INTEL_SSE3 1
#  endif
#  if defined(__SSSE3__) || MSC_X86_SIMD(2)
#    define COMPILE_INTEL_SSSE3 1
#  endif
#  if defined(__SSE4_1__) || MSC_X86_SIMD(2)
#    define COMPILE_INTEL_SSE41 1
#  endif
#  if defined(__SSE4_2__) || MSC_X86_SIMD(2)
#    define COMPILE_INTEL_SSE42 1
#  endif
#  if defined(__AVX__)
#    define COMPILE_INTEL_AVX 1
#   endif
#  if defined(__AVX2__)
#    define COMPILE_INTEL_AVX2 1
#   endif
#endif

#if defined (_M_PPC) || defined(__powerpc64__) || defined(__powerpc__)
#  define COMPILE_POWERPC 1
#  ifdef __ALTIVEC__
#    define COMPILE_POWERPC_ALTIVEC 1
#  else
#    define COMPILE_POWERPC_ALTIVEC 0
#  endif
#else
#  define COMPILE_POWERPC 0
#endif

#if defined (_M_ARM) || defined(__arm__) || defined(__thumb__)
#  define COMPILE_ARM 1
#else
#  define COMPILE_ARM 0
#endif

// Min & max delta QP limits based on bit depth
#define KVZ_QP_DELTA_MIN -(26 + 3 * (KVZ_BIT_DEPTH - 8))
#define KVZ_QP_DELTA_MAX 25 + 3 * (KVZ_BIT_DEPTH - 8)

#endif
