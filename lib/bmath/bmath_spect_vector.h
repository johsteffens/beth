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

#ifndef BMATH_SPECT_VECTOR_H
#define BMATH_SPECT_VECTOR_H

/// A vector is defined by a ring-type and an array.

#include "bmath_spect_algebraic.h"
#include "bcore_spect_array.h"
#include "bmath_quicktypes.h"

/**********************************************************************************************************************/

/** Nomenclature
 *  add, zro, neg, sub - according to spect_group
 *  cpy    : vector = vector;  projecting/injecting copy if dim1 != dim2
 *  mul    : vector = vector * scalar
 *  dot_prd: scalar = vector * vector  (dot-product)
 *  sqr    : scalar = vector^2
 *  sqr_sub: scalar = (vector - vector)^2
 */

/**********************************************************************************************************************/

/// operation - features
typedef void (*bmath_fp_vector_mul     )( vd_t vec, vc_t vec1, vc_t scl2 ); // vec = vec1 * scl2
typedef void (*bmath_fp_vector_dot_prd )( vd_t scl, vc_t vec1, vc_t scl2 ); // scl = vec1 * vec2

/**********************************************************************************************************************/

BCORE_DECLARE_SPECT( bmath_vector_s )
{
    bcore_spect_header_s header;
    const bmath_ring_s*  spect_ring_scalar;  // ring of scalar type (vector element)
    const bcore_array_s* spect_array_vector; // array of vector
    const bcore_inst_s*  spect_inst_vector;  // inst of vector

    bmath_fp_add            fp_add; // feature
    bmath_fp_zro            fp_zro; // feature
    bmath_fp_neg            fp_neg; // feature
    bmath_fp_sub            fp_sub; // feature
    bmath_fp_cpy            fp_cpy; // feature
    bmath_fp_vector_mul     fp_mul; // feature
    bmath_fp_vector_dot_prd fp_dot_prd; // feature
};

sz_t bmath_vector_spect_get_dim( const bmath_vector_s* p, vc_t o );

/** Vector operations:
 *  - Arguments are organized: result, operand1, operand2
 *  - Same object may be used in different argument places.
 *  - Scalar types relate to spect_ring_scalar
 *  - Vectors of different dim are allowed.
 *    Zeros are implicitly appended to match dim where appropriate.
 *  - Result vectors must already have desired target dim. (No vector resizing)
 */
void bmath_vector_spect_zro(     const bmath_vector_s* p, vd_t o );
void bmath_vector_spect_neg(     const bmath_vector_s* p, vd_t o, vc_t vec1 );
void bmath_vector_spect_cpy(     const bmath_vector_s* p, vd_t o, vc_t vec1 );
void bmath_vector_spect_add(     const bmath_vector_s* p, vd_t o, vc_t vec1, vc_t vec2 );
void bmath_vector_spect_sub(     const bmath_vector_s* p, vd_t o, vc_t vec1, vc_t vec2 );
void bmath_vector_spect_mul(     const bmath_vector_s* p, vd_t o, vc_t vec1, vc_t scl2 );
void bmath_vector_spect_dot_prd( const bmath_vector_s* p, vd_t scl, vc_t o,  vc_t vec2 );
void bmath_vector_spect_sqr(     const bmath_vector_s* p, vd_t scl, vc_t o );
void bmath_vector_spect_sqr_sub( const bmath_vector_s* p, vd_t scl, vc_t o,  vc_t vec2 );

sz_t bmath_vector_typed_get_dim( tp_t t, vc_t o );
void bmath_vector_typed_zro(     tp_t t, vd_t o );
void bmath_vector_typed_neg(     tp_t t, vd_t o, vc_t vec1 );
void bmath_vector_typed_cpy(     tp_t t, vd_t o, vc_t vec1 );
void bmath_vector_typed_add(     tp_t t, vd_t o, vc_t vec1, vc_t vec2 );
void bmath_vector_typed_sub(     tp_t t, vd_t o, vc_t vec1, vc_t vec2 );
void bmath_vector_typed_mul(     tp_t t, vd_t o, vc_t vec1, vc_t scl2 );
void bmath_vector_typed_dot_prd( tp_t t, vd_t scl, vc_t o,  vc_t vec2 );
void bmath_vector_typed_sqr(     tp_t t, vd_t scl, vc_t o );
void bmath_vector_typed_sqr_sub( tp_t t, vd_t scl, vc_t o,  vc_t vec2 );

sz_t bmath_vector_aware_get_dim( vc_t o );
void bmath_vector_aware_zro(     vd_t o );
void bmath_vector_aware_neg(     vd_t o, vc_t vec1 );
void bmath_vector_aware_cpy(     vd_t o, vc_t vec1 );
void bmath_vector_aware_add(     vd_t o, vc_t vec1, vc_t vec2 );
void bmath_vector_aware_sub(     vd_t o, vc_t vec1, vc_t vec2 );
void bmath_vector_aware_mul(     vd_t o, vc_t vec1, vc_t scl2 );
void bmath_vector_aware_dot_prd( vd_t s, vc_t o,    vc_t vec2 );
void bmath_vector_aware_sqr(     vd_t s, vc_t o );
void bmath_vector_aware_sqr_sub( vd_t s, vc_t o,    vc_t vec2 );

sz_t bmath_vector_get_dim( sr_s o );
void bmath_vector_zro(     sr_s o );
void bmath_vector_neg(     sr_s o, vc_t vec1 );
void bmath_vector_cpy(     sr_s o, vc_t vec1 );
void bmath_vector_add(     sr_s o, vc_t vec1, vc_t vec2 );
void bmath_vector_sub(     sr_s o, vc_t vec1, vc_t vec2 );
void bmath_vector_mul(     sr_s o, vc_t vec1, vc_t scl2 );
void bmath_vector_dot_prd( vd_t s, sr_s o,    vc_t vec2 );
void bmath_vector_sqr(     vd_t s, sr_s o );
void bmath_vector_sqr_sub( vd_t s, sr_s o,    vc_t vec2 );

sz_t bmath_vector_q_get_dim( const sr_s* o );
void bmath_vector_q_zro(     const sr_s* o );
void bmath_vector_q_neg(     const sr_s* o, vc_t vec1 );
void bmath_vector_q_cpy(     const sr_s* o, vc_t vec1 );
void bmath_vector_q_add(     const sr_s* o, vc_t vec1, vc_t vec2 );
void bmath_vector_q_sub(     const sr_s* o, vc_t vec1, vc_t vec2 );
void bmath_vector_q_mul(     const sr_s* o, vc_t vec1, vc_t scl2 );
void bmath_vector_q_dot_prd( vd_t s, const sr_s* o,    vc_t vec2 );
void bmath_vector_q_sqr(     vd_t s, const sr_s* o );
void bmath_vector_q_sqr_sub( vd_t s, const sr_s* o,    vc_t vec2 );

/**********************************************************************************************************************/

vd_t bmath_spect_vector_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_SPECT_VECTOR_H

