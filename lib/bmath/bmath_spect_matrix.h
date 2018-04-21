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

#include "bmath_spect_algebraic.h"
#include "bmath_spect_vector.h"
#include "bcore_spect_matrix.h"
#include "bmath_quicktypes.h"

/**********************************************************************************************************************/

/// operation - features
typedef void (*bmath_fp_matrix_mul_vec )( vc_t o, vd_t vec1, vc_t vec2 ); // vec1 = o    * vec2
typedef void (*bmath_fp_matrix_mul_scl )( vd_t o, vc_t mat1, vc_t scl2 ); // o    = mat1 * scl2

/**********************************************************************************************************************/

BCORE_DECLARE_SPECT( bmath_matrix_s )
{
    bcore_spect_header_s header;

    const bcore_array_s  * spect_array_matrix;    // array of matrix
    const bmath_vector_s * spect_vector_matrix;   // vector of matrix
    const bcore_matrix_s * spect_matrix_matrix;   // (bcore-)matrix of matrix
    const bcore_inst_s   * spect_inst_matrix;     // inst of matrix
    const bmath_vector_s * spect_vector_default;  // default vector perspective
    const bmath_ring_s   * spect_ring_scalar;     // ring of scalar type (matrix element)

    // ring features
    bmath_fp_add add;
    bmath_fp_zro zro;
    bmath_fp_neg neg;
    bmath_fp_sub sub;
    bmath_fp_mul mul;
    bmath_fp_one one;
    bmath_fp_inv inv;
    bmath_fp_div div;

    // matrix features
    bmath_fp_matrix_mul_vec matrix_mul_vec;
    bmath_fp_matrix_mul_scl matrix_mul_scl;
};

BCORE_DECLARE_SPECT_FUNC_R0( bmath_matrix, get_rows, sz_t, vc_t )
BCORE_DECLARE_SPECT_FUNC_R0( bmath_matrix, get_cols, sz_t, vc_t )
BCORE_DECLARE_SPECT_MAPD_V0( bmath_matrix, zro, vd_t )
BCORE_DECLARE_SPECT_MAPD_V1( bmath_matrix, neg, vd_t, vc_t, mat1 )
BCORE_DECLARE_SPECT_FUNC_V1( bmath_matrix, cpy, vd_t, vc_t, mat1 )
BCORE_DECLARE_SPECT_FUNC_V2( bmath_matrix, add, vd_t, vc_t, mat1, vc_t, mat2 )
BCORE_DECLARE_SPECT_FUNC_V2( bmath_matrix, sub, vd_t, vc_t, mat1, vc_t, mat2 )
BCORE_DECLARE_SPECT_FUNC_V2( bmath_matrix, mul, vd_t, vc_t, mat1, vc_t, mat2 )
BCORE_DECLARE_SPECT_FUNC_V0( bmath_matrix, one, vd_t )
BCORE_DECLARE_SPECT_FUNC_V1( bmath_matrix, inv, vd_t, vc_t, mat1 )
BCORE_DECLARE_SPECT_FUNC_V2( bmath_matrix, div, vd_t, vc_t, mat1, vc_t, mat2 )
BCORE_DECLARE_SPECT_FUNC_V2( bmath_matrix, mul_scl, vd_t, vc_t, mat1, vc_t, scl2 )
BCORE_DECLARE_SPECT_FUNC_V2( bmath_matrix, mul_vec, vc_t, vd_t, vec1, vc_t, vec2 ) // vec1 = o * vec2

/**********************************************************************************************************************/

vd_t bmath_spect_matrix_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_SPECT_MATRIX_H

