/** Copyright 2019 Johannes Bernhard Steffens
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/// This file is included at the beginning of template code files.

/**********************************************************************************************************************/
// Defining template macros

#ifndef BMATH_TEMPLATE_FX_PREC
    #error "Macro 'BMATH_TEMPLATE_FX_PREC' is not defined."
#endif

#if BMATH_TEMPLATE_FX_PREC == 2

    // leaf
    #define fx_t f2_t
    #define fx_( name ) f2_##name

    // features
    #define bmath_fp_u2_argb_from_fx bmath_fp_u2_argb_from_f2

    // global functions
    #define bmath_simd_fx_( name )  bmath_simd_f2_##name

    // vector
    #define bmath_vfx_s              bmath_vf2_s
    #define bmath_vfx_s_(     name ) bmath_vf2_s_##name
    #define bmath_vfx_(       name ) bmath_vf2_##name
    #define bmath_fx_t_vec_(  name ) bmath_f2_t_vec_##name
    #define bmath_fp_fx_(     name ) bmath_fp_f2_##name

    // arr-vector
    #define bmath_arr_vfx_s          bmath_arr_vf2_s
    #define bmath_arr_vfx_s_( name ) bmath_arr_vf2_s_##name

    // givens rotations
    #define bmath_grt_fx_s              bmath_grt_f2_s
    #define bmath_grt_fx_s_( name )     bmath_grt_f2_s_##name
    #define bmath_arr_grt_fx_( name )   bmath_arr_grt_f2_##name
    #define bmath_arr_grt_fx_s          bmath_arr_grt_f2_s
    #define bmath_arr_grt_fx_s_( name ) bmath_arr_grt_f2_s_##name

    // matrix
    #define bmath_mfx_s             bmath_mf2_s
    #define bmath_mfx_s_(    name ) bmath_mf2_s_##name
    #define bmath_mfx_(      name ) bmath_mf2_##name
    #define bmath_fp_mfx_s_( name ) bmath_fp_mf2_s_##name
    #define TYPEOF_bmath_mfx_s             TYPEOF_bmath_mf2_s
    #define TYPEOF_bmath_fp_mfx_s_( name ) TYPEOF_bmath_fp_mf2_s_##name

    // AVX related
    #define __m256x          __m256
    #define _mm256_add_px    _mm256_add_ps
    #define _mm256_mul_px    _mm256_mul_ps
    #define _mm256_loadu_px  _mm256_loadu_ps
    #define _mm256_storeu_px _mm256_storeu_ps
    #define _mm256_fmadd_px  _mm256_fmadd_ps
    #define _mm256_set1_px   _mm256_set1_ps
    #define M256_SIZE       8
    #define M256_ZERO       { 0, 0, 0, 0, 0, 0, 0, 0 }
    #define M256_HSUM(v)    v[0] + v[1] + v[2] + v[3] + v[4] + v[5] + v[6] + v[7];

#elif BMATH_TEMPLATE_FX_PREC == 3

    // leaf type
    #define fx_t f3_t
    #define fx_( name ) f3_##name

    // features
    #define bmath_fp_u2_argb_from_fx bmath_fp_u2_argb_from_f3

    // global functions
    #define bmath_simd_fx_( name )  bmath_simd_f3_##name

    // vector
    #define bmath_vfx_s             bmath_vf3_s
    #define bmath_vfx_(      name ) bmath_vf3_##name
    #define bmath_vfx_s_(    name ) bmath_vf3_s_##name
    #define bmath_fx_t_vec_( name ) bmath_f3_t_vec_##name
    #define bmath_fp_fx_(    name ) bmath_fp_f3_##name

    // arr-vector
    #define bmath_arr_vfx_s          bmath_arr_vf3_s
    #define bmath_arr_vfx_s_( name ) bmath_arr_vf3_s_##name

    // givens rotations
    #define bmath_grt_fx_s              bmath_grt_f3_s
    #define bmath_grt_fx_s_( name )     bmath_grt_f3_s_##name
    #define bmath_arr_grt_fx_( name )   bmath_arr_grt_f3_##name
    #define bmath_arr_grt_fx_s          bmath_arr_grt_f3_s
    #define bmath_arr_grt_fx_s_( name ) bmath_arr_grt_f3_s_##name

    // matrix
    #define bmath_mfx_s             bmath_mf3_s
    #define bmath_mfx_s_(    name ) bmath_mf3_s_##name
    #define bmath_mfx_(      name ) bmath_mf3_##name
    #define bmath_fp_mfx_s_( name ) bmath_fp_mf3_s_##name
    #define TYPEOF_bmath_mfx_s             TYPEOF_bmath_mf3_s
    #define TYPEOF_bmath_fp_mfx_s_( name ) TYPEOF_bmath_fp_mf3_s_##name

    // AVX related
    #define __m256x          __m256d
    #define _mm256_add_px    _mm256_add_pd
    #define _mm256_mul_px    _mm256_mul_pd
    #define _mm256_loadu_px  _mm256_loadu_pd
    #define _mm256_storeu_px _mm256_storeu_pd
    #define _mm256_fmadd_px  _mm256_fmadd_pd
    #define _mm256_set1_px   _mm256_set1_pd
    #define M256_SIZE       4
    #define M256_ZERO       { 0, 0, 0, 0 }
    #define M256_HSUM(v)    v[0] + v[1] + v[2] + v[3];

#else

    #error "Macro BMATH_TEMPLATE_FX_PREC has wrong value! (Only '2' or '3' are supported)"

#endif // BMATH_TEMPLATE_FX_PREC

/**********************************************************************************************************************/

