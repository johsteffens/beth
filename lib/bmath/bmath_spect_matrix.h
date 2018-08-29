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

#ifndef BMATH_SPECT_MATRIX_H
#define BMATH_SPECT_MATRIX_H

/// A matrix is defined by a ring-type and an object supporting bcore_spect_matrix

#include "bmath_flags.h"
#include "bmath_spect_algebraic.h"
#include "bmath_spect_vector.h"
#include "bcore_spect_matrix.h"
#include "bmath_quicktypes.h"

typedef struct bmath_matrix bmath_matrix;

/**********************************************************************************************************************/

/// operation - features
typedef void (*bmath_fp_matrix_mul_vec )( const bmath_matrix* o1, vc_t v2, vd_t vr ); // vr = o1 * v2
typedef void (*bmath_fp_matrix_mul_scl )( bmath_matrix* or,       vc_t m1, vc_t s2 ); // or = m1 * s2

/**********************************************************************************************************************/

BCORE_DECLARE_SPECT( bmath_matrix_s )
{
    bcore_spect_header_s header;

    const bcore_array_s  * spect_array_matrix;    // array of matrix
    const bmath_vector_s * spect_vector_matrix;   // vector of matrix
    const bcore_matrix_s * spect_matrix_matrix;   // (bcore-)matrix of matrix
    const bcore_inst_s   * spect_inst_matrix;     // inst of matrix
    const bmath_vector_s * spect_vector_assoc;    // associated vector (vector that can represent a row, column or mul_vec operand)
    const bmath_ring_s   * spect_ring_scalar;     // ring of scalar type (matrix element or mul_scl operand)

    // ring features
    bmath_fp_add add;
    bmath_fp_zro zro;
    bmath_fp_neg neg;
    bmath_fp_sub sub;
    bmath_fp_cpy cpy;
    bmath_fp_mul mul;
    bmath_fp_one one;
    bmath_fp_inv inv;
    bmath_fp_div div;

    // matrix features
    bmath_fp_matrix_mul_vec mul_vec;
    bmath_fp_matrix_mul_scl mul_scl;
};

BCORE_FUNC_SPECT_CONST1_RET1_ARG0_MAP0( bmath_matrix, get_rows, uz_t )
BCORE_FUNC_SPECT_CONST1_RET1_ARG0_MAP0( bmath_matrix, get_cols, uz_t )
BCORE_FUNC_SPECT_CONST0_RET0_ARG0_MAPX( bmath_matrix, zro )
BCORE_FUNC_SPECT_CONST1_RET0_ARG1_MAP0( bmath_matrix, cpy, bmath_matrix*, res )
BCORE_FUNC_SPECT_CONST1_RET0_ARG1_MAPX( bmath_matrix, neg, bmath_matrix*, res )
BCORE_FUNC_SPECT_CONST1_RET0_ARG2_MAPX( bmath_matrix, add, const bmath_matrix*, op, bmath_matrix*, res )
BCORE_FUNC_SPECT_CONST1_RET0_ARG2_MAPX( bmath_matrix, sub, const bmath_matrix*, op, bmath_matrix*, res )
BCORE_FUNC_SPECT_CONST1_RET0_ARG2_MAPX( bmath_matrix, mul, const bmath_matrix*, op, bmath_matrix*, res )
BCORE_FUNC_SPECT_CONST0_RET0_ARG0_MAPX( bmath_matrix, one )
BCORE_FUNC_SPECT_CONST1_RET0_ARG1_MAPX( bmath_matrix, inv, bmath_matrix*, res )
BCORE_FUNC_SPECT_CONST1_RET0_ARG2_MAPX( bmath_matrix, div,     const bmath_matrix*, op, bmath_matrix*, res )
BCORE_FUNC_SPECT_CONST1_RET0_ARG2_MAPX( bmath_matrix, mul_scl, const bmath_ring*,   op, bmath_matrix*, res )
BCORE_FUNC_SPECT_CONST1_RET0_ARG2_MAPX( bmath_matrix, mul_vec, const bmath_vector*, op, bmath_vector*, res )

/**********************************************************************************************************************/

vd_t bmath_spect_matrix_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_SPECT_MATRIX_H

