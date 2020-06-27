/** Author and Copyright 2018 Johannes Bernhard Steffens
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

#ifndef BMATH_PMT_H
#define BMATH_PMT_H

/** Permutation (P)
 *  The permutation is a unitary matrix. It is implemented as index array p of size n
 *  providing the map p[k] <-> k.
 *
 *  Mat-Operation     Implementation                ; Name
 *  B = P   * A   <=> row_p[k]( B ) = row_k   ( A ) ; pmt_mul
 *  B = P^T * A   <=> row_k   ( B ) = row_p[k]( A ) ; pmt_htp_mul
 *  B = A   * P   <=> col_k   ( B ) = col_p[k]( A ) ; mul_pmt
 *  B = A   * P^T <=> col_p[k]( B ) = col_k   ( A ) ; mul_pmt_htp
 *
 *  Vec-Operation     Implementation                ; Name
 *  y = P   * x   <=> y_p[k] = x_k                  ; pmt_mul
 *  y = P^T * x   <=> y_k    = x_p[k]               ; pmt_htp_mul
 *
 */

#include "bcore_std.h"
#include "bmath_hwflags.h"
#include "bmath_leaf.h"

typedef struct bmath_mf3_s bmath_mf3_s;

/**********************************************************************************************************************/

BCORE_DECLARE_OBJECT( bmath_pmt_s ) { aware_t _; BCORE_ARRAY_DYN_SOLID_STATIC_S( uz_t, ); };

void bmath_pmt_s_set_size( bmath_pmt_s* o, uz_t size ); // allocates but does not initialize array
void bmath_pmt_s_one_n( bmath_pmt_s* o, uz_t n ); // allocates and initializes as n x n identity

void bmath_pmt_s_random( bmath_pmt_s* o, u3_t* rval ); // sets random permutation
void bmath_pmt_s_one( bmath_pmt_s* o );
void bmath_pmt_s_cpy(    const bmath_pmt_s* o, bmath_pmt_s* res );
void bmath_pmt_s_to_mf3( const bmath_pmt_s* o, bmath_mf3_s* res );

void bmath_pmt_s_htp(     const bmath_pmt_s* o, bmath_pmt_s* res );
void bmath_pmt_s_mul(     const bmath_pmt_s* o, const bmath_pmt_s* op, bmath_pmt_s* res );
void bmath_pmt_s_mul_htp( const bmath_pmt_s* o, const bmath_pmt_s* op, bmath_pmt_s* res );

bl_t bmath_pmt_s_is_equ( const bmath_pmt_s* o, const bmath_pmt_s* op );
bl_t bmath_pmt_s_is_one( const bmath_pmt_s* o );

void bmath_pmt_s_to_string( const bmath_pmt_s* o, st_s* string );
void bmath_pmt_s_to_stdout( const bmath_pmt_s* o );

//----------------------------------------------------------------------------------------------------------------------

vd_t bmath_pmt_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_PMT_H
