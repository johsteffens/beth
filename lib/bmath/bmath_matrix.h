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

#ifndef BMATH_MATRIX_H
#define BMATH_MATRIX_H

/**********************************************************************************************************************/

/// Collection of matrices.

/** Nomenclature
 *  add, zro, neg, sub, mul, mul_vec, mul_scl - according to spect_matrix
 *  htp: (hermitean) transpose
 *  ltr: lower triangular matrix
 */
/**********************************************************************************************************************/

#include "bcore_std.h"
#include "bmath_spect_algebraic.h"
#include "bmath_vector.h"

/**********************************************************************************************************************/
// dynamic size matrix of f3_t

BCORE_DECLARE_OBJECT_( bmath_mf3_s )
{
    aware_t _;
    sz_t rows;
    sz_t cols;
    sz_t stride;
    union
    {
        bcore_array_dyn_solid_static_s arr;
        struct
        {
            f3_t* data;
            sz_t  size;
            sz_t  space;
        };
    };
};

void bmath_mf3_s_set_size( bmath_mf3_s* o, sz_t rows, sz_t cols );
static inline
void bmath_mf3_s_set_size_to( const bmath_mf3_s* o, bmath_mf3_s* res ) { bmath_mf3_s_set_size( res, o->rows, o->cols ); }

bl_t bmath_mf3_s_is_equ( const bmath_mf3_s* o, const bmath_mf3_s* op );
bl_t bmath_mf3_s_is_zro( const bmath_mf3_s* o );
bl_t bmath_mf3_s_is_one( const bmath_mf3_s* o );

bl_t bmath_mf3_s_is_near_equ( const bmath_mf3_s* o, const bmath_mf3_s* op, f3_t max_dev );
bl_t bmath_mf3_s_is_near_one( const bmath_mf3_s* o, f3_t max_dev );
bl_t bmath_mf3_s_is_near_zro( const bmath_mf3_s* o, f3_t max_dev );
bl_t bmath_mf3_s_is_near_iso( const bmath_mf3_s* o, f3_t max_dev ); // near isometry (== near orthonormal)

void bmath_mf3_s_add( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res );
void bmath_mf3_s_zro(       bmath_mf3_s* o );
void bmath_mf3_s_neg( const bmath_mf3_s* o, bmath_mf3_s* res );
void bmath_mf3_s_sub( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res );
void bmath_mf3_s_cpy( const bmath_mf3_s* o, bmath_mf3_s* res );
void bmath_mf3_s_mul( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res ); // o * op

/// multiplication of o with op(transposed); (faster than mul)
void bmath_mf3_s_mul_htp( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res );

void bmath_mf3_s_one(       bmath_mf3_s* o );
void bmath_mf3_s_inv( const bmath_mf3_s* o, bmath_mf3_s* res );
void bmath_mf3_s_div( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res );
void bmath_mf3_s_mul_vec( const bmath_mf3_s* o, const bmath_vf3_s* op, bmath_vf3_s* res );
void bmath_mf3_s_mul_scl( const bmath_mf3_s* o, const f3_t*        op, bmath_mf3_s* res );
static inline void bmath_mf3_s_mul_scl_f3( const bmath_mf3_s* o, f3_t op, bmath_mf3_s* res ) { bmath_mf3_s_mul_scl( o, &op, res ); }

void bmath_mf3_s_htp(     const bmath_mf3_s* o, bmath_mf3_s* res ); // hermitean transpose

void bmath_mf3_s_set_row_by_data( bmath_mf3_s* o, sz_t idx, const f3_t* data );
void bmath_mf3_s_set_col_by_data( bmath_mf3_s* o, sz_t idx, const f3_t* data );
void bmath_mf3_s_set_row_by_vec ( bmath_mf3_s* o, sz_t idx, const bmath_vf3_s* vec );
void bmath_mf3_s_set_col_by_vec ( bmath_mf3_s* o, sz_t idx, const bmath_vf3_s* vec );
void bmath_mf3_s_get_row_vec( const bmath_mf3_s* o, sz_t idx, bmath_vf3_s* res );
void bmath_mf3_s_get_col_vec( const bmath_mf3_s* o, sz_t idx, bmath_vf3_s* res );

static inline
void bmath_mf3_s_set_f3( bmath_mf3_s* o, sz_t row, sz_t col, f3_t v )
{
    assert( row < o->rows && col <= o->cols );
    o->data[ row * o->stride + col ] = v;
}

static inline
f3_t bmath_mf3_s_get_f3( const bmath_mf3_s* o, sz_t row, sz_t col )
{
    assert( row < o->rows && col <= o->cols );
    return o->data[ row * o->stride + col ];
}

bmath_vf3_s bmath_mf3_s_get_row_weak_vec( const bmath_mf3_s* o, sz_t idx ); // produces a weak vector without heap usage (no bmath_vf3_s_down needed on result)

/** Cholesky decomposition.
 *  o must be positive definite.
 *  Only the lower triangle of o is evaluated.
 *  res: represents the lower-triangular version
 *  It is o = res * resT.
 *  Algorithm always succeeds.
 *  When o is not positive definite, incomputable elements of res are set to zero.
 */
void bmath_mf3_s_cholesky( const bmath_mf3_s* o, bmath_mf3_s* res );

/** Inversion and h-transposition of lower triangular matrix.
 *  o is deemed lower triangular (only lower triangular elements are evaluated)
 *  res is upper triangular
 *  1 = o * resT = resT * o
 *  Algorithm always succeeds.
 *  If o is not invertible, incomputable elements are set to zero.
 */
void bmath_mf3_s_ltr_inv_htp( const bmath_mf3_s* o, bmath_mf3_s* res );

/** Multiplication of o with o(transposed).
 *  o is deemed triangular (only triangular elements are evaluated)
 *  res is symmetric
 */
void bmath_mf3_s_ltr_mul_htp( const bmath_mf3_s* o, bmath_mf3_s* res ); // lower triangular
void bmath_mf3_s_utr_mul_htp( const bmath_mf3_s* o, bmath_mf3_s* res ); // upper triangular

/// For easy inspection
void bmath_mf3_s_to_stdout( const bmath_mf3_s* o );

/**********************************************************************************************************************/

vd_t bmath_matrix_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_MATRIX_H
