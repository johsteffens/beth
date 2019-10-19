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

/// This file is included at the end of template code files.

/**********************************************************************************************************************/
// Undefining template macros

#undef BMATH_TEMPLATE_FX_PREC

// leaf
#undef fx_t
#undef fx_

// global functions
#undef bmath_simd_fx_

// features
#undef bmath_fp_u2_argb_from_fx

// vector
#undef bmath_vfx_s
#undef bmath_vfx_s_
#undef bmath_vfx_
#undef bmath_fx_t_vec_
#undef bmath_fp_fx_

// arr-vector
#undef bmath_arr_vfx_s
#undef bmath_arr_vfx_s_

// givens rotations
#undef bmath_grt_fx_s
#undef bmath_grt_fx_s_
#undef bmath_arr_grt_fx_
#undef bmath_arr_grt_fx_s
#undef bmath_arr_grt_fx_s_

// matrix
#undef bmath_mfx_s
#undef bmath_mfx_s_
#undef bmath_mfx_
#undef bmath_fp_mfx_s_
#undef TYPEOF_bmath_mfx_s
#undef TYPEOF_bmath_fp_mfx_s_

// AVX related
#undef __m256x
#undef _mm256_add_px
#undef _mm256_mul_px
#undef _mm256_loadu_px
#undef _mm256_storeu_px
#undef _mm256_fmadd_px
#undef _mm256_set1_px
#undef M256_SIZE
#undef M256_ZERO
#undef M256_HSUM

/**********************************************************************************************************************/

