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

#ifndef BMATH_MF3_MUL_H
#define BMATH_MF3_MUL_H

/**********************************************************************************************************************/

/** Collection of algorithms related to matrix multiplication.
 *  All routines have been designed from scratch and optimized with modern architectures in mind.
 *
 *  This header is included in bmath_mf3.h and should not be used in isolation
 *  See bmath_mf3.h for nomenclature.
 */

/**********************************************************************************************************************/

#include "bcore_std.h"
#include "bmath_hwflags.h"
#include "bmath_spect_algebraic.h"
#include "bmath_vector.h"
#include "bmath_grt.h"

/**********************************************************************************************************************/
// dynamic size matrix of f3_t

typedef struct bmath_mf3_s bmath_mf3_s;

//----------------------------------------------------------------------------------------------------------------------

/// o * m -> r
void bmath_mf3_s_mul(     const bmath_mf3_s* o, const bmath_mf3_s* m, bmath_mf3_s* r ); // r can be folded
void bmath_mf3_s_mul_esp( const bmath_mf3_s* o, const bmath_mf3_s* m, bmath_mf3_s* r );

/// o * m^T -> r
void bmath_mf3_s_mul_htp(     const bmath_mf3_s* o, const bmath_mf3_s* m, bmath_mf3_s* r ); // r can be folded
void bmath_mf3_s_mul_htp_esp( const bmath_mf3_s* o, const bmath_mf3_s* m, bmath_mf3_s* r );

/// o^T * m -> r
void bmath_mf3_s_htp_mul(     const bmath_mf3_s* o, const bmath_mf3_s* m, bmath_mf3_s* r ); // r can be folded
void bmath_mf3_s_htp_mul_esp( const bmath_mf3_s* o, const bmath_mf3_s* m, bmath_mf3_s* r );

/// o^T * m^T -> r
void bmath_mf3_s_htp_mul_htp( const bmath_mf3_s* o, const bmath_mf3_s* m, bmath_mf3_s* r ); // r can be folded

///  u * d * u^T -> r
void bmath_mf3_s_mul_udu_htp(     const bmath_mf3_s* u, const bmath_vf3_s* d, bmath_mf3_s* r );
void bmath_mf3_s_mul_udu_htp_esp( const bmath_mf3_s* u, const bmath_vf3_s* d, bmath_mf3_s* r );

///  u * d * v^T -> r
void bmath_mf3_s_mul_udv_htp(     const bmath_mf3_s* u, const bmath_vf3_s* d, const bmath_mf3_s* v, bmath_mf3_s* r );
void bmath_mf3_s_mul_udv_htp_esp( const bmath_mf3_s* u, const bmath_vf3_s* d, const bmath_mf3_s* v, bmath_mf3_s* r );

///  u * t * v^T -> r with t being tridiagonal
void bmath_mf3_s_mul_utv_htp(     const bmath_mf3_s* u, const bmath_mf3_s* t, const bmath_mf3_s* v, bmath_mf3_s* r );
void bmath_mf3_s_mul_utv_htp_esp( const bmath_mf3_s* u, const bmath_mf3_s* t, const bmath_mf3_s* v, bmath_mf3_s* r );

/// ( o * b ) + c -> r
void bmath_mf3_s_mul_add( const bmath_mf3_s* o, const bmath_mf3_s* b, const bmath_mf3_s* c, bmath_mf3_s* r );

/// ( o * b^T ) + c -> r
void bmath_mf3_s_mul_htp_add( const bmath_mf3_s* o, const bmath_mf3_s* b, const bmath_mf3_s* c, bmath_mf3_s* r );

/// ( o^T * b ) + c -> r
void bmath_mf3_s_htp_mul_add( const bmath_mf3_s* o, const bmath_mf3_s* b, const bmath_mf3_s* c, bmath_mf3_s* r );

/// ( o^T * b^T ) + c -> r
void bmath_mf3_s_htp_mul_htp_add( const bmath_mf3_s* o, const bmath_mf3_s* b, const bmath_mf3_s* c, bmath_mf3_s* r );

/** composite multiply-add function. Satisfies functionality of BLAS:DGEMM. Supports folded r.
 *  op(a) * op(b) * c + d * e -> r
 *  op(x) = htpx ? x^T : x;
 *  c, e are scalar.
 *  d can be NULL
 */
void bmath_mf3_s_mul_add_cps( bl_t htpa, const bmath_mf3_s* a, bl_t htpb, const bmath_mf3_s* b, f3_t c, const bmath_mf3_s* d, f3_t e, bmath_mf3_s* r );
void bmath_mf3_s_mul_add_cps_selftest();

//----------------------------------------------------------------------------------------------------------------------

#endif  // BMATH_MF3_MUL_H
