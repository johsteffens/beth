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
#include "bmath_types.h"

/**********************************************************************************************************************/

/** Nomenclature
 *  add, zro, neg, sub - according to spect_group
 *  cpy    : vector = vector;  projecting/injecting copy if dim1 != dim2
 *  mul    : vector = vector * scalar
 *  dot_prd: scalar = vector * vector  (dot-product)
 *  sqr    : scalar = vector^2
 *  sqr_sub: scalar = (vector - vector)^2
 */

typedef struct bmath_vector bmath_vector;

/**********************************************************************************************************************/

/// operation - features
typedef void (*bmath_fp_vector_mul     )( const bmath_vector* o, const bmath_ring*   op, bmath_vector* res ); // res = o * op
typedef void (*bmath_fp_vector_dot_prd )( const bmath_vector* o, const bmath_vector* op, bmath_ring*   res ); // res = o * op

/**********************************************************************************************************************/

BCORE_DECLARE_SPECT( bmath_vector_s )
{
    bcore_spect_header_s header;
    const bmath_ring_s*  spect_ring_scalar;  // ring of scalar type (vector element)
    const bcore_array_s* spect_array_vector; // array of vector
    const bcore_inst_s*  spect_inst_vector;  // inst of vector

    bmath_fp_add            add; // feature
    bmath_fp_zro            zro; // feature
    bmath_fp_neg            neg; // feature
    bmath_fp_sub            sub; // feature
    bmath_fp_cpy            cpy; // feature
    bmath_fp_vector_mul     mul; // feature
    bmath_fp_vector_dot_prd dot_prd; // feature
};

BCORE_FUNC_SPECT_CONST1_RET1_ARG0_MAP0( bmath_vector, get_dim, sz_t )
BCORE_FUNC_SPECT_CONST0_RET0_ARG0_MAPX( bmath_vector, zro )
BCORE_FUNC_SPECT_CONST1_RET0_ARG1_MAPX( bmath_vector, neg,     bmath_vector*, res )
BCORE_FUNC_SPECT_CONST1_RET0_ARG1_MAPX( bmath_vector, cpy,     bmath_vector*, res )
BCORE_FUNC_SPECT_CONST1_RET0_ARG2_MAPX( bmath_vector, add,     const bmath_vector*, op, bmath_vector*, res )
BCORE_FUNC_SPECT_CONST1_RET0_ARG2_MAPX( bmath_vector, sub,     const bmath_vector*, op, bmath_vector*, res )
BCORE_FUNC_SPECT_CONST1_RET0_ARG2_MAPX( bmath_vector, mul,     const bmath_ring*,   op, bmath_vector*, res  )
BCORE_FUNC_SPECT_CONST1_RET0_ARG2_MAPX( bmath_vector, dot_prd, const bmath_vector*, op, bmath_ring*, res )
BCORE_FUNC_SPECT_CONST1_RET0_ARG1_MAP0( bmath_vector, sqr,                              bmath_ring*, res )
BCORE_FUNC_SPECT_CONST1_RET0_ARG2_MAP0( bmath_vector, sub_sqr, const bmath_vector*, op, bmath_ring*, res )

/**********************************************************************************************************************/

vd_t bmath_spect_vector_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_SPECT_VECTOR_H

