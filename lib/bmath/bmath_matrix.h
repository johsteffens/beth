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

/** Matrix types and operations.
 *  All routines have been entirely designed from scratch and optimized with modern architectures in mind.
 */

/** Nomenclature
 *  add, zro, neg, sub, mul, mul_vec, mul_scl - according to spect_matrix
 *  inv: inverse
 *  piv: pseudo-inverse
 *  av1: affine vector (vector of size 'n' interpreted vector of size n+1 with last element being '1' ); used for affine transformations
 *  htp: (hermitean) transpose
 *  ltr: lower triangle matrix (evaluation ignores upper triangle)
 *  lt1: lower triangle matrix with main diagonal elements deemed 1 and not evaluated (luc satisfies ltr with respect for evaluation)
 *  utr: upper triangle matrix (evaluation ignores lower triangle)
 *  luc: LU-composite matrix: L - diag(L) + U; diagonal elements of L have all value 'one' and are not explicitly stored.
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
 *  grt: givens rotation
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
void bmath_mf3_s_fill_random( bmath_mf3_s* o, f3_t min, f3_t max, u2_t* rval );

bmath_mf3_s* bmath_mf3_s_create_set_size( sz_t rows, sz_t cols );
bmath_mf3_s* bmath_mf3_s_create_fill_random( sz_t rows, sz_t cols, f3_t min, f3_t max, u2_t* rval );

static inline void bmath_mf3_s_set_size_to( const bmath_mf3_s* o, bmath_mf3_s* res ) { bmath_mf3_s_set_size( res, o->rows, o->cols ); }
static inline bl_t bmath_mf3_s_is_equ_size( const bmath_mf3_s* o, const bmath_mf3_s* op ) { return o->rows == op->rows && o->cols == op->cols; }
static inline bl_t bmath_mf3_s_is_square( const bmath_mf3_s* o ) { return o->rows == o->cols; }

//---------------------------------------------------------------------------------------------------------------------
// checks

/** Near-state means: For each matrix element the absolute difference
 *  to the specified state is less or equal max_dev.
 *  Hence, max_dev == 0 tests for the exact state (s. below).
 */
bl_t bmath_mf3_s_is_near_equ( const bmath_mf3_s* o, const bmath_mf3_s* op, f3_t max_dev ); // equality
bl_t bmath_mf3_s_is_near_one( const bmath_mf3_s* o, f3_t max_dev ); // one (== identity)
bl_t bmath_mf3_s_is_near_zro( const bmath_mf3_s* o, f3_t max_dev ); // zero
bl_t bmath_mf3_s_is_near_iso( const bmath_mf3_s* o, f3_t max_dev ); // near isometry (== near orthonormal)
bl_t bmath_mf3_s_is_near_dag( const bmath_mf3_s* o, f3_t max_dev ); // diagonal
bl_t bmath_mf3_s_is_near_trd( const bmath_mf3_s* o, f3_t max_dev ); // symmetric tri-diagonal
bl_t bmath_mf3_s_is_near_utr( const bmath_mf3_s* o, f3_t max_dev ); // upper triangle
bl_t bmath_mf3_s_is_near_ltr( const bmath_mf3_s* o, f3_t max_dev ); // lower triangle
bl_t bmath_mf3_s_is_near_hsm( const bmath_mf3_s* o, f3_t max_dev ); // symmetric
bl_t bmath_mf3_s_is_near_ubd( const bmath_mf3_s* o, f3_t max_dev ); // upper bi-diagonal
bl_t bmath_mf3_s_is_near_lbd( const bmath_mf3_s* o, f3_t max_dev ); // lower bi-diagonal

static inline bl_t bmath_mf3_s_is_equ( const bmath_mf3_s* o, const bmath_mf3_s* op ) { return bmath_mf3_s_is_near_equ( o, op, 0 ); }
static inline bl_t bmath_mf3_s_is_zro( const bmath_mf3_s* o ) { return bmath_mf3_s_is_near_zro( o, 0 ); }
static inline bl_t bmath_mf3_s_is_one( const bmath_mf3_s* o ) { return bmath_mf3_s_is_near_one( o, 0 ); }
static inline bl_t bmath_mf3_s_is_dag( const bmath_mf3_s* o ) { return bmath_mf3_s_is_near_dag( o, 0 ); }
static inline bl_t bmath_mf3_s_is_trd( const bmath_mf3_s* o ) { return bmath_mf3_s_is_near_trd( o, 0 ); }
static inline bl_t bmath_mf3_s_is_utr( const bmath_mf3_s* o ) { return bmath_mf3_s_is_near_utr( o, 0 ); }
static inline bl_t bmath_mf3_s_is_ltr( const bmath_mf3_s* o ) { return bmath_mf3_s_is_near_ltr( o, 0 ); }
static inline bl_t bmath_mf3_s_is_hsm( const bmath_mf3_s* o ) { return bmath_mf3_s_is_near_hsm( o, 0 ); }
static inline bl_t bmath_mf3_s_is_ubd( const bmath_mf3_s* o ) { return bmath_mf3_s_is_near_ubd( o, 0 ); }
static inline bl_t bmath_mf3_s_is_lbd( const bmath_mf3_s* o ) { return bmath_mf3_s_is_near_lbd( o, 0 ); }

//---------------------------------------------------------------------------------------------------------------------
// initialization, copying

f3_t bmath_mf3_s_f3_trc( const bmath_mf3_s* o );
f3_t bmath_mf3_s_f3_sub_sqr( const bmath_mf3_s* o, const bmath_mf3_s* op );

void bmath_mf3_s_zro(       bmath_mf3_s* o );
void bmath_mf3_s_one(       bmath_mf3_s* o );
void bmath_mf3_s_neg( const bmath_mf3_s* o, bmath_mf3_s* res );
void bmath_mf3_s_sub( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res );
void bmath_mf3_s_cpy( const bmath_mf3_s* o, bmath_mf3_s* res );

//---------------------------------------------------------------------------------------------------------------------
// transposition

void bmath_mf3_s_htp( const bmath_mf3_s* o, bmath_mf3_s* res );

//---------------------------------------------------------------------------------------------------------------------
// addition

void bmath_mf3_s_add( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res );

/// adds outer product of two vectors op1 (X) op2 to matrix
void bmath_mf3_s_add_opd( const bmath_mf3_s* o, const bmath_vf3_s* op1, const bmath_vf3_s* op2, bmath_mf3_s* res );

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
// inversion; pseudo-inversion

void bmath_mf3_s_inv(     const bmath_mf3_s* o, bmath_mf3_s* res ); // res = o^-1
void bmath_mf3_s_inv_htp( const bmath_mf3_s* o, bmath_mf3_s* res ); // res = (o^-1)T
void bmath_mf3_s_hsm_inv( const bmath_mf3_s* o, bmath_mf3_s* res ); // res = o^-1 in case o is hermitean/symmetric (3x faster than bmath_mf3_s_inv)

/** Pseudo-Inversion:
 *  Inversion via SVD by setting near-zero singular values to zero (cut-off).
 *  A singular value s is cut off when abs( s ) < eps * max( { abs(s_0), ..., abs(s_n-1) } ).
 */
void bmath_mf3_s_piv(     const bmath_mf3_s* o, f3_t eps, bmath_mf3_s* res ); // general matrix o
void bmath_mf3_s_hsm_piv( const bmath_mf3_s* o, f3_t eps, bmath_mf3_s* res ); // faster inversion in case o is symmetric

/** Affine inversion.
 *  res = av1-inverse of o.
 *  o and res deemed to be av1-transformations
 */
void bmath_mf3_s_inv_av1(     const bmath_mf3_s* o, bmath_mf3_s* res );
void bmath_mf3_s_hsm_inv_av1( const bmath_mf3_s* o, bmath_mf3_s* res );           // o symmetric
void bmath_mf3_s_hsm_piv_av1( const bmath_mf3_s* o, f3_t eps, bmath_mf3_s* res ); // pseudo inversion; o symmetric

void bmath_mf3_s_div(         const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res );

//---------------------------------------------------------------------------------------------------------------------
// element-access, col-access, row-access, sub-matrix

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

void bmath_mf3_s_mul_f3_to_row( bmath_mf3_s* o, f3_t v, sz_t i ); // multiplies v to all values in row i
void bmath_mf3_s_mul_f3_to_col( bmath_mf3_s* o, f3_t v, sz_t i ); // multiplies v to all values in col i

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
 *  Returned object does not own its data and need not be shut down unless it is resized.
 */
bmath_mf3_s bmath_mf3_s_get_weak_sub_mat( const bmath_mf3_s* o, sz_t row, sz_t col, sz_t rows, sz_t cols );

/** Returns a weak row vector.
 *  Returned object does not own its data and need not be shut down unless it is resized.
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
void bmath_mf3_s_decompose_cholesky( const bmath_mf3_s* o, bmath_mf3_s* res );

/** LU decomposition.
 *  o must be square and non-singular.
 *  res: represents a LU-composite matrix (LUC):
 *       U is upper triangle of LUC
 *       L is lower triangle of LUC except diagonal
 *       diagonal of L is all 1 and not stored.
 *  When o is singular, incomputable elements of res are set to zero.
 */
void bmath_mf3_s_decompose_luc( const bmath_mf3_s* o, bmath_mf3_s* res );

/** Inversion and h-transposition of lower triangular matrix.
 *  o is deemed lower triangular (only lower triangular elements are evaluated)
 *  res is upper triangular
 *  It is: 1 = o * resT = resT * o
 *  Algorithm always finishes.
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
// Eigenvalue decomposition (EVD) and supportive operations

/** Stable in-place tri-diagonal decomposition for a symmetric matrix.
 *  Based on Givens rotations.
 *  Input:  a  (symmetric),    v  (rotation, identity or NULL)
 *  Output: a' (tri-diagonal), v' (rotation or NULL)
 *  It is vT * a * v = v'T * a' * v'.
 */
void bmath_mf3_s_decompose_trd( bmath_mf3_s* a, bmath_mf3_s* v );

/** Stable in-place QR-decomposition. Based on Givens rotations.
 *  Input:  q  (rotation or identity), r  (any square matrix),
 *  Output: q' (rotation),             r' (upper_triangular) such that with qT * r = q'T * r'.
 *  q == NULL allowed, in which case only r' is computed.
 */
void bmath_mf3_s_decompose_qr( bmath_mf3_s* q, bmath_mf3_s* r );

/** Stable in-place EVD for a symmetric matrix. Jacobi Method.
 *  Input:  a  (symmetric), v  (rotation or identity)
 *  Output: a' (diagonal),  v' (rotation) with vT * a * v = v'T * a' * v'.
 *  Returns 'true' on successful convergence, 'false' otherwise with a' likely not being diagonal.
 *  (Convergence failure is very rare.)
 *  Diagonal elements are sorted in descending value order.
 *  v == NULL allowed, in which case only a' is computed.
 */
bl_t bmath_mf3_s_evd_jacobi( bmath_mf3_s* a, bmath_mf3_s* v );

/** In-place EVD for a symmetric matrix.
 *  Approach: TRD, QR with explicit shifting. (Variant of Francis' QR-Algorithm)
 *  Input:  a  (symmetric), v  (rotation or identity)
 *  Output: a' (diagonal),  v' (rotation) with vT * a * v = v'T * a' * v'.
 *  Returns 'true' on successful convergence, 'false' otherwise with a' likely not being diagonal.
 *  (Convergence failure is very rare.)
 *  Diagonal elements are sorted in descending value order.
 *  v == NULL allowed, in which case only a' is computed.
 */
bl_t bmath_mf3_s_evd( bmath_mf3_s* a, bmath_mf3_s* v );

//---------------------------------------------------------------------------------------------------------------------
// Singular value decomposition (SVD) and supportive operations

/** Stable in-place bi-diagonal decomposition for a general matrix.
 *  Based on givens rotations.
 *  Matrices u, a, v are being modified: mat -> mat'
 *  Input:  u  (nxn unitary or NULL), a  (nxm, any data), v  (nxn unitary or NULL),
 *  Output: u' (nxn unitary or NULL), a' (bi-diagonal),   v' (nxn unitary or NULL)
 *  It is uT * a * v = u'T * a' * v'
 */
void bmath_mf3_s_decompose_ubd( bmath_mf3_s* u, bmath_mf3_s* a, bmath_mf3_s* v ); // upper-bidiagonal
void bmath_mf3_s_decompose_lbd( bmath_mf3_s* u, bmath_mf3_s* a, bmath_mf3_s* v ); // lower-bidiagonal

/** Stable in-place full SVD for a general matrix.
 *  Method: Bi-diagonalization by givens rotations and QR-chasing with implicit shift.
 *          (Variant of Golub-Reinsch-Algorithm)
 *
 *  Matrices u, a, v are being modified: mat -> mat'
 *  Input:  u  (nxn unitary or NULL), a  (nxm, any data), v  (nxn unitary or NULL),
 *  Output: u' (nxn unitary or NULL), a' (diagonal),      v' (nxn unitary or NULL)
 *  It is uT * a * v = u'T * a' * v'
 *
 *  Returns 'true' on successful convergence, 'false' otherwise with a' likely not being diagonal.
 *  (Convergence failure is very rare.)
 *
 *  On success:
 *    - a' is fully diagonal
 *    - diagonal elements are non-negative
 *    - diagonal elements are sorted in descending value order
 *    - Det(u') == 1
 *    - Det(v') == 1 or -1
 */
bl_t bmath_mf3_s_svd( bmath_mf3_s* u, bmath_mf3_s* a, bmath_mf3_s* v ); // for all matrices

//---------------------------------------------------------------------------------------------------------------------
// covariance

/** Sets o to the covariance matrix of a section of arr_vec:
 *  oij = E( E( vi - E( vi ) )E( vj - E( vj ) ) )
 */
void bmath_mf3_s_set_covariance_on_section_fast( bmath_mf3_s* o, bmath_arr_vf3_s* arr_vec, sz_t start, sz_t end ); // fast
void bmath_mf3_s_set_covariance_on_section_sprc( bmath_mf3_s* o, bmath_arr_vf3_s* arr_vec, sz_t start, sz_t end ); // stochastically precise

//---------------------------------------------------------------------------------------------------------------------
// easy inspection

void bmath_mf3_s_to_stdout( const bmath_mf3_s* o );

/**********************************************************************************************************************/

vd_t bmath_matrix_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_MATRIX_H