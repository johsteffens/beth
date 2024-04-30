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
#undef fy
#undef vfx
#undef vfy
#undef vcfx
#undef vcfy
#undef mfx
#undef mfy
#undef xsmfx
#undef xsmfy
#undef asmfx
#undef asmfy

// objects
#undef fx_t
#undef fy_t
#undef bmath_vfx_s
#undef bmath_vfy_s
#undef bmath_cfx_s
#undef bmath_cfy_s
#undef bmath_vcfx_s
#undef bmath_vcfy_s
#undef bmath_arr_vfx_s
#undef bmath_arr_vfy_s
#undef bmath_grt_fx_s
#undef bmath_grt_fy_s
#undef bmath_arr_grt_fx_s
#undef bmath_arr_grt_fy_s
#undef bmath_mfx_s
#undef bmath_mfy_s
#undef bmath_xsmfx_s
#undef bmath_xsmfy_s
#undef bmath_asmfx_s
#undef bmath_asmfy_s

#undef TYPEOF_bmath_vfx_s
#undef TYPEOF_bmath_vfy_s
#undef TYPEOF_bmath_cfx_s
#undef TYPEOF_bmath_cfy_s
#undef TYPEOF_bmath_vcfx_s
#undef TYPEOF_bmath_vcfy_s
#undef TYPEOF_bmath_mfx_s
#undef TYPEOF_bmath_mfy_s
#undef TYPEOF_bmath_xsmfx_s
#undef TYPEOF_bmath_xsmfy_s
#undef TYPEOF_bmath_asmfx_s
#undef TYPEOF_bmath_asmfy_s

// AVX related
#undef P5_SIZE
#undef P5_SIZE_B2E
#undef P5_ZERO
#undef P5_HSUM

#undef P6_SIZE
#undef P6_SIZE_B2E
#undef P6_ZERO
#undef P6_HSUM

#undef M5_LOAD
#undef M5_STOR
#undef M5_SET_ALL
#undef M5_SET_ZERO

#undef M6_LOAD
#undef M6_STOR
#undef M6_SET_ALL
#undef M6_SET_ZERO

#undef M5_ADD
#undef M5_SUB
#undef M5_MUL

#undef M6_ADD
#undef M6_SUB
#undef M6_MUL

#undef M5_MUL_ADD
#undef M5_MUL_SUB

#undef M6_MUL_ADD
#undef M6_MUL_SUB

#undef MM_PX
#undef M5_T
#undef M6_T

/**********************************************************************************************************************/

