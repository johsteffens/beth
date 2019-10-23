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

/// This file is included at the beginning of template code files.

/**********************************************************************************************************************/
// floating point template macros

#ifndef BMATH_TEMPLATE_FX_PREC
    #error "Macro 'BMATH_TEMPLATE_FX_PREC' is not defined."
#endif

#if BMATH_TEMPLATE_FX_PREC == 2
    #define fx f2
#elif BMATH_TEMPLATE_FX_PREC == 3
    #define fx f3
#else
    #error "Macro BMATH_TEMPLATE_FX_PREC has wrong value! (Only '2' or '3' are supported)"
#endif // BMATH_TEMPLATE_FX_PREC

// infix
#define vfx  BCAT(v,fx)
#define cfx  BCAT(c,fx)
#define vcfx BCAT(vc,fx)
#define mfx  BCAT(m,fx)

// objects
#define fx_t                     BCATU(fx,t)
#define bmath_fp_u2_argb_from_fx BCATU(bmath_fp_u2_argb_from,fx)
#define bmath_vfx_s              BCATU(bmath,vfx,s)
#define bmath_cfx_s              BCATU(bmath,cfx,s)
#define bmath_vcfx_s             BCATU(bmath,vcfx,s)
#define bmath_arr_vfx_s          BCATU(bmath_arr,vfx,s)
#define bmath_grt_fx_s           BCATU(bmath_grt,fx,s)
#define bmath_arr_grt_fx_s       BCATU(bmath_arr_grt,fx,s)
#define bmath_mfx_s              BCATU(bmath,mfx,s)
#define TYPEOF_bmath_mfx_s       BCATU(TYPEOF,bmath,mfx,s)

/**********************************************************************************************************************/
// AVX related macros

#if BMATH_TEMPLATE_FX_PREC == 2
    #define MM_PX      ps
    #define M5_T       __m256
    #define P5_ZERO    { 0, 0, 0, 0, 0, 0, 0, 0 }
    #define P5_HSUM(v) ( v[0] + v[1] + v[2] + v[3] + v[4] + v[5] + v[6] + v[7] )
#elif BMATH_TEMPLATE_FX_PREC == 3
    #define MM_PX      pd
    #define M5_T       __m256d
    #define P5_ZERO    { 0, 0, 0, 0 }
    #define P5_HSUM(v) ( v[0] + v[1] + v[2] + v[3] )
#endif // BMATH_TEMPLATE_FX_PREC

// P5_SIZE: number of values per M5_T; P5_SIZE = 2 ^ P5_SIZE_EXP
#define P5_SIZE_EXP ( 5 - BMATH_TEMPLATE_FX_PREC )
#define P5_SIZE     ( 1 << P5_SIZE_EXP )

#define M5_LOAD BCATU(_mm256_loadu,  MM_PX) // a = loadu( ptr256 ): a = *ptr256
#define M5_STOR BCATU(_mm256_storeu, MM_PX) // storeu( ptr256, a ): *ptr256 = a
#define M5_SET1 BCATU(_mm256_set1,   MM_PX) // a = set1( f_val ):  duplicates f_val to all elements of a

#define M5_ADD BCATU(_mm256_add, MM_PX) // c = add( a, b ): c = a + b
#define M5_MUL BCATU(_mm256_mul, MM_PX) // c = mul( a, b ): c = a * b

// d = fmadd( a, b, c ): d = a * b + c
#ifdef BMATH_AVX2_FMA
    #define M5_MUL_ADD( a, b, c ) BCATU(_mm256_fmadd, MM_PX)( a, b, c )
#else
    #define M5_MUL_ADD( a, b, c ) M5_ADD( M5_MUL( a, b ), c )
#endif

/**********************************************************************************************************************/

