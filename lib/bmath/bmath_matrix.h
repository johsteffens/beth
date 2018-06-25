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

/// Matrix types and operations.

/** Nomenclature
 *  add, zro, neg, sub, mul, mul_vec, mul_scl - according to spect_matrix
 *  inv: inverse
 *  piv: pseudo-inverse
 *  av1: affine vector (vector of size 'n' interpreted vector of size n+1 with last element being '1' ); used for affine transformations
 *  htp: (hermitean) transpose
 *  ltr: lower triangle matrix (evaluation ignores upper triangle)
 *  lt1: lower triangle matrix with main diagonal elements deemed 1 and not evaluated (luc satisfies ltr with respect for evaluation)
 *  utr: upper triangle matrix (evaluation ignores lower triangle)
 *  luc: LU-composite matrix: L - diag(L) + U; diagonal elements of L are all 1 and neeed not be stored
 *  hsm: hermitean (symmetric) matrix.
 *  trc: trace
 *  dag: diagonal
 *  trd: tri-diagonal (symmetric)
 *  ubd: upper-bidiagonal
 *  lbd: lower-bidiagonal
 *  det: determinant
 *  svd: singular value decomposition
 *  evd: eigen value decomposition  (svd for hsm matrix)
 *  iso: isometry (orthonormal, unitary)
 *  opd: outer product of two vectors
 *  udu: similarity transform a of a diagonal matrix: htp_udu: U^T * D * U; udu_htp: U * D * U^T
 */
/**********************************************************************************************************************/

#include "bcore_std.h"
#include "bmath_spect_algebraic.h"
#include "bmath_vector.h"

/**********************************************************************************************************************/
// dynamic size matrix of f3_t

BCORE_DECLARE_OBJECT( bmath_mf3_s )
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

void bmath_mf3_s_fill_random( bmath_mf3_s* o, f3_t min, f3_t max, u2_t* rval );

bmath_mf3_s* bmath_mf3_s_create_set_size( sz_t rows, sz_t cols );
bmath_mf3_s* bmath_mf3_s_create_fill_random( sz_t rows, sz_t cols, f3_t min, f3_t max, u2_t* rval );

static inline bl_t bmath_mf3_s_is_equ_size( const bmath_mf3_s* o, const bmath_mf3_s* op ) { return o->rows == op->rows && o->cols == op->cols; }
static inline bl_t bmath_mf3_s_is_square( const bmath_mf3_s* o ) { return o->rows == o->cols; }

bl_t bmath_mf3_s_is_equ( const bmath_mf3_s* o, const bmath_mf3_s* op );
bl_t bmath_mf3_s_is_zro( const bmath_mf3_s* o );
bl_t bmath_mf3_s_is_one( const bmath_mf3_s* o );
bl_t bmath_mf3_s_is_dag( const bmath_mf3_s* o );
bl_t bmath_mf3_s_is_trd( const bmath_mf3_s* o );
bl_t bmath_mf3_s_is_utr( const bmath_mf3_s* o );
bl_t bmath_mf3_s_is_ltr( const bmath_mf3_s* o );
bl_t bmath_mf3_s_is_hsm( const bmath_mf3_s* o );
bl_t bmath_mf3_s_is_ubd( const bmath_mf3_s* o );
bl_t bmath_mf3_s_is_lbd( const bmath_mf3_s* o );

bl_t bmath_mf3_s_is_near_equ( const bmath_mf3_s* o, const bmath_mf3_s* op, f3_t max_dev );
bl_t bmath_mf3_s_is_near_one( const bmath_mf3_s* o, f3_t max_dev );
bl_t bmath_mf3_s_is_near_zro( const bmath_mf3_s* o, f3_t max_dev );
bl_t bmath_mf3_s_is_near_iso( const bmath_mf3_s* o, f3_t max_dev ); // near isometry (== near orthonormal)
bl_t bmath_mf3_s_is_near_dag( const bmath_mf3_s* o, f3_t max_dev );
bl_t bmath_mf3_s_is_near_trd( const bmath_mf3_s* o, f3_t max_dev );
bl_t bmath_mf3_s_is_near_utr( const bmath_mf3_s* o, f3_t max_dev );
bl_t bmath_mf3_s_is_near_ltr( const bmath_mf3_s* o, f3_t max_dev );
bl_t bmath_mf3_s_is_near_hsm( const bmath_mf3_s* o, f3_t max_dev );
bl_t bmath_mf3_s_is_near_ubd( const bmath_mf3_s* o, f3_t max_dev );
bl_t bmath_mf3_s_is_near_lbd( const bmath_mf3_s* o, f3_t max_dev );

f3_t bmath_mf3_s_f3_trc( const bmath_mf3_s* o );
f3_t bmath_mf3_s_f3_sub_sqr( const bmath_mf3_s* o, const bmath_mf3_s* op );

void bmath_mf3_s_add( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res );
void bmath_mf3_s_zro(       bmath_mf3_s* o );
void bmath_mf3_s_neg( const bmath_mf3_s* o, bmath_mf3_s* res );
void bmath_mf3_s_sub( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res );
void bmath_mf3_s_cpy( const bmath_mf3_s* o, bmath_mf3_s* res );

/// adds outer product of two vectors op1 (X) op2 to matrix
void bmath_mf3_s_add_opd( const bmath_mf3_s* o, const bmath_vf3_s* op1, const bmath_vf3_s* op2, bmath_mf3_s* res );

void bmath_mf3_s_one(           bmath_mf3_s* o );

//---------------------------------------------------------------------------------------------------------------------
// inversion

void bmath_mf3_s_inv(     const bmath_mf3_s* o, bmath_mf3_s* res ); // res = o^-1
void bmath_mf3_s_inv_htp( const bmath_mf3_s* o, bmath_mf3_s* res ); // res = (o^-1)T
void bmath_mf3_s_hsm_inv( const bmath_mf3_s* o, bmath_mf3_s* res ); // res = o^-1 in case o is hermitean/symmetric (3x faster than bmath_mf3_s_inv)

/** Pseudo-inversion for symmetric matrix.
 *  zero-threshold for eigenvalues: eps * max(eigenvalues).
 */
void bmath_mf3_s_hsm_piv( const bmath_mf3_s* o, f3_t eps, bmath_mf3_s* res );

/** Affine inversion.
 *  res = av1-inverse of o.
 *  o and res deemed to be av1-transformations
 */
void bmath_mf3_s_inv_av1(     const bmath_mf3_s* o, bmath_mf3_s* res );
void bmath_mf3_s_hsm_inv_av1( const bmath_mf3_s* o, bmath_mf3_s* res );           // o symmetric
void bmath_mf3_s_hsm_piv_av1( const bmath_mf3_s* o, f3_t eps, bmath_mf3_s* res ); // pseudo inversion; o symmetric

void bmath_mf3_s_div(         const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res );

//---------------------------------------------------------------------------------------------------------------------
// multiplication

void bmath_mf3_s_mul( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res ); // res = o * op
void bmath_mf3_s_mul_dag( const bmath_mf3_s* o, const bmath_vf3_s* dag, bmath_mf3_s* res ); // res = o * dag  (columns get scaled)
void bmath_mf3_s_dag_mul( const bmath_mf3_s* o, const bmath_vf3_s* dag, bmath_mf3_s* res ); // res = dag * o  (rows get scaled)

/// multiplication of o with op(transposed); (faster than mul)
void bmath_mf3_s_mul_htp( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res ); // o * op^T

void bmath_mf3_s_mul_vec( const bmath_mf3_s* o, const bmath_vf3_s* vec, bmath_vf3_s* res );
void bmath_mf3_s_mul_av1( const bmath_mf3_s* o, const bmath_vf3_s* av1, bmath_vf3_s* res ); // affine transformation (see nomenclature 'av1')

void bmath_mf3_s_mul_scl( const bmath_mf3_s* o, const f3_t*        op, bmath_mf3_s* res );
static inline
void bmath_mf3_s_mul_scl_f3( const bmath_mf3_s* o, f3_t op, bmath_mf3_s* res ) { bmath_mf3_s_mul_scl( o, &op, res ); }

/** In place similarity transformation of a diagonal matrix.
 *  R = O * D * O^T
 */
void bmath_mf3_s_udu_htp( const bmath_mf3_s* o, const bmath_vf3_s* dag, bmath_mf3_s* res ); // res = o * dag * o^T

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_htp( const bmath_mf3_s* o, bmath_mf3_s* res ); // hermitean transpose

//---------------------------------------------------------------------------------------------------------------------
// element-, col-, row-access, sub-matrix

void bmath_mf3_s_set_row_by_data( bmath_mf3_s* o, sz_t idx, const f3_t* data );
void bmath_mf3_s_set_col_by_data( bmath_mf3_s* o, sz_t idx, const f3_t* data );
void bmath_mf3_s_set_dag_by_data( bmath_mf3_s* o,           const f3_t* data );
void bmath_mf3_s_set_row_by_vec ( bmath_mf3_s* o, sz_t idx, const bmath_vf3_s* vec );
void bmath_mf3_s_set_col_by_vec ( bmath_mf3_s* o, sz_t idx, const bmath_vf3_s* vec );
void bmath_mf3_s_set_dag_by_vec ( bmath_mf3_s* o,           const bmath_vf3_s* vec );
void bmath_mf3_s_get_row_vec( const bmath_mf3_s* o, sz_t idx, bmath_vf3_s* res );
void bmath_mf3_s_get_col_vec( const bmath_mf3_s* o, sz_t idx, bmath_vf3_s* res );
void bmath_mf3_s_get_dag_vec( const bmath_mf3_s* o,           bmath_vf3_s* res );

void bmath_mf3_s_swap_row( bmath_mf3_s* o, sz_t i, sz_t j ); // swaps rows i, j
void bmath_mf3_s_swap_col( bmath_mf3_s* o, sz_t i, sz_t j ); // swaps cols i, j

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

/** Returns a weak (rows x cols) sub matrix at offset (row, col) from o.
 *  Returned object need not be shut down.
 */
bmath_mf3_s bmath_mf3_s_get_weak_sub_mat( const bmath_mf3_s* o, sz_t row, sz_t col, sz_t rows, sz_t cols );

/** Returns a weak row vector.
 *  Returned object need not be shut down.
 */
bmath_vf3_s bmath_mf3_s_get_row_weak_vec( const bmath_mf3_s* o, sz_t idx );

//---------------------------------------------------------------------------------------------------------------------
// Special triangular matrices

/** Cholesky decomposition.
 *  o must be positive definite.
 *  Only the lower triangle of o is evaluated.
 *  res: represents the lower-triangular version
 *  It is o = res * resT.
 *  When o is not positive definite, incomputable elements of res are set to zero.
 */
void bmath_mf3_s_decompose_cholesky( const bmath_mf3_s* o, bmath_mf3_s* ltr_res );

/** LU decomposition.
 *  o must be square and invertible.
 *  res: represents a LU-composite matrix (LUC):
 *       U is upper triangle of LUC
 *       L is lower triangle of LUC except diagonal
 *       diagonal of L is all 1 and not stored.
 *  When o is not invertible, incomputable elements of res are set to zero.
 */
void bmath_mf3_s_decompose_luc( const bmath_mf3_s* o, bmath_mf3_s* res );

/** Inversion and h-transposition of lower triangular matrix.
 *  o is deemed lower triangular (only lower triangular elements are evaluated)
 *  res is upper triangular
 *  1 = o * resT = resT * o
 *  Algorithm always succeeds.
 *  If o is not invertible, incomputable elements are set to zero.
 */
void bmath_mf3_s_ltr_inv_htp( const bmath_mf3_s* o, bmath_mf3_s* res );

/** Multiplication of o with o(transposed) of triangular matrix. (res = o * oT)
 *  Only triangular elements of o are evaluated
 *  res is symmetric
 */
void bmath_mf3_s_ltr_mul_htp( const bmath_mf3_s* o, bmath_mf3_s* res ); // lower triangular
void bmath_mf3_s_utr_mul_htp( const bmath_mf3_s* o, bmath_mf3_s* res ); // upper triangular

/** Multiplication of special matrix with a vector res = o * op.
 *  Only relevant triangular elements of o are evaluated.
 *  In-Place for op==res.
 */
void bmath_mf3_s_ltr_mul_vec( const bmath_mf3_s* o, const bmath_vf3_s* op, bmath_vf3_s* res );
void bmath_mf3_s_lt1_mul_vec( const bmath_mf3_s* o, const bmath_vf3_s* op, bmath_vf3_s* res );
void bmath_mf3_s_utr_mul_vec( const bmath_mf3_s* o, const bmath_vf3_s* op, bmath_vf3_s* res );
void bmath_mf3_s_luc_mul_vec( const bmath_mf3_s* o, const bmath_vf3_s* op, bmath_vf3_s* res );

/** Multiplication of special matrix with a transposed matrix, transposed result: resT = o * opT.
 *  Only relevant triangular elements of o are evaluated.
 *  In-Place for op==res.
 */
void bmath_mf3_s_ltr_mul_htp_htp( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res );
void bmath_mf3_s_lt1_mul_htp_htp( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res );
void bmath_mf3_s_utr_mul_htp_htp( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res );
void bmath_mf3_s_luc_mul_htp_htp( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res );

/** Solves multiplication of special matrix with a vector: op = o * res.
 *  Only relevant triangular elements of o are evaluated.
 *  In-Place for op==res.
 */
void bmath_mf3_s_ltr_solve_vec( const bmath_mf3_s* o, const bmath_vf3_s* op, bmath_vf3_s* res );
void bmath_mf3_s_lt1_solve_vec( const bmath_mf3_s* o, const bmath_vf3_s* op, bmath_vf3_s* res );
void bmath_mf3_s_utr_solve_vec( const bmath_mf3_s* o, const bmath_vf3_s* op, bmath_vf3_s* res );
void bmath_mf3_s_luc_solve_vec( const bmath_mf3_s* o, const bmath_vf3_s* op, bmath_vf3_s* res );

/** Solves multiplication of special matrix with a transposed matrix, transposed result: opT = o * resT.
 *  Only relevant triangular elements of o are evaluated.
 *  In-Place for op==res.
 */
void bmath_mf3_s_ltr_solve_htp_htp( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res );
void bmath_mf3_s_lt1_solve_htp_htp( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res );
void bmath_mf3_s_utr_solve_htp_htp( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res );
void bmath_mf3_s_luc_solve_htp_htp( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res );

//---------------------------------------------------------------------------------------------------------------------
// Eigenvalue decomposition and supportive operations

/** Stable in-place tri-diagonal decomposition for a symmetric matrix.
 *  Based on Givens rotations.
 *  Input:  a  (symmetric),    v  (rotation, identity or NULL)
 *  Output: a' (tri-diagonal), v' (rotation or NULL)
 *  It is vT * a * v = v'T * a' * v'.
 */
void bmath_mf3_s_hsm_trd_htp_givens( bmath_mf3_s* a, bmath_mf3_s* v );

/** Stable in-place upper-bi-diagonal decomposition for a general matrix.
 *  Based on givens rotations.
 *  Input:  a  (nxm, any data), v  (mxm rotation, identity or NULL), u  (nxn rotation, identity or NULL)
 *  Output: a' (bi-diagonal),   v' (mxm rotation or NULL),           u' (nxn rotation or NULL)
 *  It is uT * a * v = u'T * a' * v
 */
void bmath_mf3_s_ubd_htp_givens( bmath_mf3_s* u, bmath_mf3_s* a, bmath_mf3_s* v );

/** Stable in-place QR-decomposition. Based on Givens rotations.
 *  Input:  q  (rotation or identity), r  (any square matrix),
 *  Output: q' (rotation),             r' (upper_triangular) such that with qT * r = q'T * r'.
 *  q == NULL allowed, in which case only r' is computed.
 */
void bmath_mf3_s_qr_rot_htp_utr_givens( bmath_mf3_s* q, bmath_mf3_s* r );

/** Stable in-place SVD for a symmetric matrix. Jacobi Method.
 *  bmath_mf3_s_evd_htp for more details.
 */
void bmath_mf3_s_evd_htp_jacobi( bmath_mf3_s* a, bmath_mf3_s* q );

/** In-place SVD for a symmetric matrix. Approach: TRD, QR with explicit shifting.
 *  Very efficient for large matrices. >20x faster than Jacobi method but slightly less accurate.
 *  bmath_mf3_s_evd_htp for more details.
 */
void bmath_mf3_s_evd_htp_qr_xshift( bmath_mf3_s* a, bmath_mf3_s* q );

/** In-place SVD for a symmetric matrix. Approach: TRD, QR with isolated shifting.
 *  More stable and slightly more expensive than 'xshift' (Mathematically xshift and ishift are identical).
 *  bmath_mf3_s_evd_htp for more details.
 */
void bmath_mf3_s_evd_htp_qr_ishift( bmath_mf3_s* a, bmath_mf3_s* q );

/** Default in-place SVD for a symmetric matrix.
 *  Input:  a  (symmetric), q  (rotation or identity)
 *  Output: a' (diagonal),  q' (rotation) with qT * a * q = q'T * a' * q'.
 *  Diagonal elements are sorted in descending order.
 *  r == NULL allowed, in which case only a' is computed.
 */
static inline void bmath_mf3_s_evd_htp( bmath_mf3_s* a, bmath_mf3_s* q ) { bmath_mf3_s_evd_htp_qr_ishift( a, q ); }

//---------------------------------------------------------------------------------------------------------------------
// Covariance matrix

/** Sets o to the covariance matrix of a section of arr_vec:
 *  oij = E( E( vi - E( vi ) )E( vj - E( vj ) ) )
 */
void bmath_mf3_s_set_covariance_on_section_fast( bmath_mf3_s* o, bmath_arr_vf3_s* arr_vec, sz_t start, sz_t end ); // fast
void bmath_mf3_s_set_covariance_on_section_sprc( bmath_mf3_s* o, bmath_arr_vf3_s* arr_vec, sz_t start, sz_t end ); // stochastically precise

//---------------------------------------------------------------------------------------------------------------------

/// For easy inspection
void bmath_mf3_s_to_stdout( const bmath_mf3_s* o );

/**********************************************************************************************************************/

vd_t bmath_matrix_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_MATRIX_H
