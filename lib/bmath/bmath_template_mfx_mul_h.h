/** Copyright 2018 Johannes Bernhard Steffens
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

/**********************************************************************************************************************/

/** Collection of algorithms related to matrix multiplication.
 *  All routines have been designed from scratch and optimized with modern architectures in mind.
 *
 *  Blocking and microkernel strategy:
 *    - Recursion rows-cols trying to reach BLOCK_SIZE
 *    - Microkernels are optimized for BLOCK_SIZE and use AVX-SIMD when available
 */

/**********************************************************************************************************************/

#include "bmath_template_fx_begin.h"

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/// o * m -> r
void BCATU(bmath_mfx_s,mul)(     const bmath_mfx_s* o, const bmath_mfx_s* m, bmath_mfx_s* r ); // r can be folded
void BCATU(bmath_mfx_s,mul_esp)( const bmath_mfx_s* o, const bmath_mfx_s* m, bmath_mfx_s* r );

/// o * m^T -> r
void BCATU(bmath_mfx_s,mul_htp)(     const bmath_mfx_s* o, const bmath_mfx_s* m, bmath_mfx_s* r ); // r can be folded
void BCATU(bmath_mfx_s,mul_htp_esp)( const bmath_mfx_s* o, const bmath_mfx_s* m, bmath_mfx_s* r );

/// o^T * m -> r
void BCATU(bmath_mfx_s,htp_mul)(     const bmath_mfx_s* o, const bmath_mfx_s* m, bmath_mfx_s* r ); // r can be folded
void BCATU(bmath_mfx_s,htp_mul_esp)( const bmath_mfx_s* o, const bmath_mfx_s* m, bmath_mfx_s* r );

/// o^T * m^T -> r
void BCATU(bmath_mfx_s,htp_mul_htp)( const bmath_mfx_s* o, const bmath_mfx_s* m, bmath_mfx_s* r ); // r can be folded

///  u * d * u^T -> r
void BCATU(bmath_mfx_s,mul_udu_htp)(     const bmath_mfx_s* u, const bmath_vfx_s* d, bmath_mfx_s* r );
void BCATU(bmath_mfx_s,mul_udu_htp_esp)( const bmath_mfx_s* u, const bmath_vfx_s* d, bmath_mfx_s* r );

///  u * d * v^T -> r
void BCATU(bmath_mfx_s,mul_udv_htp)(     const bmath_mfx_s* u, const bmath_vfx_s* d, const bmath_mfx_s* v, bmath_mfx_s* r );
void BCATU(bmath_mfx_s,mul_udv_htp_esp)( const bmath_mfx_s* u, const bmath_vfx_s* d, const bmath_mfx_s* v, bmath_mfx_s* r );

///  u * t * v^T -> r with t being tridiagonal
void BCATU(bmath_mfx_s,mul_utv_htp)(     const bmath_mfx_s* u, const bmath_mfx_s* t, const bmath_mfx_s* v, bmath_mfx_s* r );
void BCATU(bmath_mfx_s,mul_utv_htp_esp)( const bmath_mfx_s* u, const bmath_mfx_s* t, const bmath_mfx_s* v, bmath_mfx_s* r );

/// ( o * b ) + c -> r
void BCATU(bmath_mfx_s,mul_add)( const bmath_mfx_s* o, const bmath_mfx_s* b, const bmath_mfx_s* c, bmath_mfx_s* r );

/// ( o * b^T ) + c -> r
void BCATU(bmath_mfx_s,mul_htp_add)( const bmath_mfx_s* o, const bmath_mfx_s* b, const bmath_mfx_s* c, bmath_mfx_s* r );

/// ( o^T * b ) + c -> r
void BCATU(bmath_mfx_s,htp_mul_add)( const bmath_mfx_s* o, const bmath_mfx_s* b, const bmath_mfx_s* c, bmath_mfx_s* r );

/// ( o^T * b^T ) + c -> r
void BCATU(bmath_mfx_s,htp_mul_htp_add)( const bmath_mfx_s* o, const bmath_mfx_s* b, const bmath_mfx_s* c, bmath_mfx_s* r );

/** composite multiply-add function. Satisfies functionality of BLAS:DGEMM. Supports folded r.
 *  op(a) * op(b) * c + d * e -> r
 *  op(x) = htpx ? x^T : x;
 *  c, e are scalar.
 *  d can be NULL
 */
void BCATU(bmath_mfx_s,mul_add_cps)( bl_t htpa, const bmath_mfx_s* a, bl_t htpb, const bmath_mfx_s* b, fx_t c, const bmath_mfx_s* d, fx_t e, bmath_mfx_s* r );
void BCATU(bmath_mfx_s,mul_add_cps_selftest)();

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// closing template

#include "bmath_template_fx_end.h"

/**********************************************************************************************************************/

