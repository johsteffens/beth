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
typedef void (*bmath_fp_matrix_mul_vec )( vd_t vec, vc_t mat1, vc_t vec2 ); // vec = mat1 * vec2
typedef void (*bmath_fp_matrix_mul_scl )( vd_t mat, vc_t mat1, vc_t scl2 ); // mat = mat1 * scl2

/**********************************************************************************************************************/

typedef struct bmath_matrix_s
{
    bcore_spect_header_s header;

    const bcore_array_s  * spect_array_matrix;    // array of matrix
    const bmath_vector_s * spect_vector_matrix;   // vector of matrix
    const bcore_matrix_s * spect_matrix_matrix;   // (bcore-)matrix of matrix
    const bcore_inst_s   * spect_inst_matrix;     // inst of matrix
    const bmath_vector_s * spect_vector_default;  // default vector perspective
    const bmath_ring_s   * spect_ring_scalar;     // ring of scalar type (matrix element)

    // ring features
    bmath_fp_add fp_add;
    bmath_fp_zro fp_zro;
    bmath_fp_neg fp_neg;
    bmath_fp_sub fp_sub;
    bmath_fp_mul fp_mul;
    bmath_fp_one fp_one;
    bmath_fp_inv fp_inv;
    bmath_fp_div fp_div;

    // matrix features
    bmath_fp_matrix_mul_vec fp_matrix_mul_vec;
    bmath_fp_matrix_mul_scl fp_matrix_mul_scl;

} bmath_matrix_s;

BCORE_DEFINE_INLINE_SPECT_GET_TYPED( bmath_matrix_s )
BCORE_DEFINE_INLINE_SPECT_GET_AWARE( bmath_matrix_s )

sz_t bmath_matrix_spect_get_rows( const bmath_matrix_s* p, vc_t o );
sz_t bmath_matrix_spect_get_cols( const bmath_matrix_s* p, vc_t o );
void bmath_matrix_spect_zro(      const bmath_matrix_s* p, vd_t o );
void bmath_matrix_spect_neg(      const bmath_matrix_s* p, vd_t o, vc_t mat1 );
void bmath_matrix_spect_cpy(      const bmath_matrix_s* p, vd_t o, vc_t mat1 );
void bmath_matrix_spect_add(      const bmath_matrix_s* p, vd_t o, vc_t mat1, vc_t mat2 );
void bmath_matrix_spect_sub(      const bmath_matrix_s* p, vd_t o, vc_t mat1, vc_t mat2 );
void bmath_matrix_spect_mul(      const bmath_matrix_s* p, vd_t o, vc_t mat1, vc_t mat2 );
void bmath_matrix_spect_one(      const bmath_matrix_s* p, vd_t o );
void bmath_matrix_spect_inv(      const bmath_matrix_s* p, vd_t o, vc_t mat1 );
void bmath_matrix_spect_div(      const bmath_matrix_s* p, vd_t o, vc_t mat1, vc_t mat2 );
void bmath_matrix_spect_mul_scl(  const bmath_matrix_s* p, vd_t o, vc_t mat1, vc_t scl2 );
void bmath_matrix_spect_mul_vec(  const bmath_matrix_s* p, vd_t vec,  vc_t o, vc_t vec2 );

/**********************************************************************************************************************/

vd_t bmath_spect_matrix_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_SPECT_MATRIX_H

