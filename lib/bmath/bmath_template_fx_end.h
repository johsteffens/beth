/** Author and Copyright 2019 Johannes Bernhard Steffens
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

// infix
#undef fx
#undef vfx
#undef vcfx
#undef mfx

// objects
#undef fx_t
#undef bmath_fp_u2_argb_from_fx
#undef bmath_vfx_s
#undef bmath_cfx_s
#undef bmath_vcfx_s
#undef bmath_arr_vfx_s
#undef bmath_grt_fx_s
#undef bmath_arr_grt_fx_s
#undef bmath_mfx_s

#undef TYPEOF_bmath_vfx_s
#undef TYPEOF_bmath_cfx_s
#undef TYPEOF_bmath_vcfx_s
#undef TYPEOF_bmath_mfx_s

// AVX related
#undef P5_SIZE
#undef P5_SIZE_EXP
#undef P5_ZERO
#undef P5_HSUM

#undef M5_ADD
#undef M5_MUL
#undef M5_LOAD
#undef M5_STOR
#undef M5_MADD
#undef M5_SET1

#undef MM_PX
#undef M5_T

/**********************************************************************************************************************/

