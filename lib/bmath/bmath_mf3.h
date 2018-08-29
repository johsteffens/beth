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
 *  Basic Algebra: op( A, B, ... ) -> C
 *    add: C = A + B
 *    zro: C =  0
 *    neg: C = -A
 *    sub: C = A - B
 *    mul: C = A * B
 *    mul_vec: y = A * x
 *    mul_scl: C = A * f (f: scalar)
 *    inv: inverse         A^-1 -> C
 *    piv: pseudo-inverse  A^-1 -> C
 *    av1: affine vector (vector of size 'n' interpreted vector of size n+1 with last element being '1' ); used for affine transformations
 *    htp: (hermitean) transpose  C = A^T
 *    pmt: permutation P: C = P * A  (see bmath_pmt.h for more details)
 *
 *  Matrix Status:
 *    hsm: real-symmetric or hermitean matrix.
 *    pdf: positive definite
 *    trc: trace
 *    dag: diagonal
 *    det: determinant
 *    otn: orthonormal mxn marix (for m>n orthonormality applies to columns, otherwise to rows)
 *    uni: unitary ( == orthonormal square marix)
 *
 *  Triangle Matrix, triangle composite matrix
 *    ltr: lower triangle matrix (evaluation ignores upper triangle)
 *    lt1: lower triangle matrix with main diagonal elements deemed 1 and not evaluated (luc satisfies ltr with respect for evaluation)
 *    utr: upper triangle matrix (evaluation ignores lower triangle)
 *    luc: LU-composite matrix: L - diag(L) + U; diagonal elements of L have all value 'one' and are not explicitly stored.
 *
 *  Factorizations:
 *    cld: cholesky-decomposition
 *    lqd: LQ-decomposition
 *    qrd: QR-decomposition
 *    trd: tri diagonal decomposition     or symmetric tri-diagonal matrix
 *    ubd: upper bidiagonal decomposition or upper-bidiagonal matrix
 *    lbd: lower bidiagonal decomposition or lower-bidiagonal matrix
 *    evd: eigen value decomposition
 *    svd: singular value decomposition
 *
 *  Other:
 *    opd: outer product of two vectors
 *    udu: similarity transform a of a diagonal matrix: htp_udu: U^T * D * U; udu_htp: U * D * U^T
 *    grt: givens rotation
 *    esp: enhanced stochastic precision (function with this suffix are (statistically) more precise but likely slower)
 *
 */
/**********************************************************************************************************************/

#include "bcore_std.h"
#include "bmath_flags.h"
#include "bmath_spect_algebraic.h"
#include "bmath_vector.h"
#include "bmath_simd.h"
#include "bmath_grt.h"
#include "bmath_mf3_qrd.h"
#include "bmath_mf3_evd.h"
#include "bmath_mf3_svd.h"

typedef struct bmath_mf3_s bmath_mf3_s;

// mf3 features
typedef void (*bmath_fp_mf3_s_mul     )( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res );
typedef void (*bmath_fp_mf3_s_mul_htp )( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res );

typedef void (*bmath_fp_mf3_s_trd_htp )(                 bmath_mf3_s* a, bmath_mf3_s* v );
typedef void (*bmath_fp_mf3_s_trd     )(                 bmath_mf3_s* a, bmath_mf3_s* v );
typedef bl_t (*bmath_fp_mf3_s_evd_htp )(                 bmath_mf3_s* a, bmath_mf3_s* v );
typedef bl_t (*bmath_fp_mf3_s_svd     )( bmath_mf3_s* u, bmath_mf3_s* a, bmath_mf3_s* v );
typedef void (*bmath_fp_mf3_s_ubd     )( bmath_mf3_s* u, bmath_mf3_s* a, bmath_mf3_s* v );
typedef void (*bmath_fp_mf3_s_lbd     )( bmath_mf3_s* u, bmath_mf3_s* a, bmath_mf3_s* v );
typedef void (*bmath_fp_mf3_s_qrd     )( bmath_mf3_s* u, bmath_mf3_s* a                 );
typedef void (*bmath_fp_mf3_s_qrd_pmt )( bmath_mf3_s* u, bmath_mf3_s* a, bmath_pmt_s* p ); // rank revealing; A = U * A' * P^T
typedef void (*bmath_fp_mf3_s_lqd     )(                 bmath_mf3_s* a, bmath_mf3_s* v );
typedef void (*bmath_fp_mf3_s_pmt_lqd )( bmath_pmt_s* p, bmath_mf3_s* a, bmath_mf3_s* v ); // rank revealing; A = P^T * A' * V

typedef bl_t (*bmath_fp_mf3_s_cld     )( const bmath_mf3_s* o, bmath_mf3_s* res );
typedef bl_t (*bmath_fp_mf3_s_lud     )( const bmath_mf3_s* o, bmath_mf3_s* res );
typedef bl_t (*bmath_fp_mf3_s_inv     )( const bmath_mf3_s* o, bmath_mf3_s* res );
typedef bl_t (*bmath_fp_mf3_s_pdf_inv )( const bmath_mf3_s* o, bmath_mf3_s* res );
typedef bl_t (*bmath_fp_mf3_s_piv     )( const bmath_mf3_s* o, f3_t eps, bmath_mf3_s* res );
typedef bl_t (*bmath_fp_mf3_s_hsm_piv )( const bmath_mf3_s* o, f3_t eps, bmath_mf3_s* res );

/// quicktypes for matrix
static inline void bmath_matrix_push_quicktypes( sr_s* list )
{
    bcore_array_r_push_sc( list, "bmath_mf3_s" );

    // features
    bcore_array_r_push_sc( list, "bmath_fp_mf3_s_mul"     );
    bcore_array_r_push_sc( list, "bmath_fp_mf3_s_mul_htp" );

    bcore_array_r_push_sc( list, "bmath_fp_mf3_s_trd_htp" );
    bcore_array_r_push_sc( list, "bmath_fp_mf3_s_trd"     );
    bcore_array_r_push_sc( list, "bmath_fp_mf3_s_evd_htp" );
    bcore_array_r_push_sc( list, "bmath_fp_mf3_s_svd"     );
    bcore_array_r_push_sc( list, "bmath_fp_mf3_s_ubd"     );
    bcore_array_r_push_sc( list, "bmath_fp_mf3_s_lbd"     );
    bcore_array_r_push_sc( list, "bmath_fp_mf3_s_qrd"     );
    bcore_array_r_push_sc( list, "bmath_fp_mf3_s_qrd_pmt" );
    bcore_array_r_push_sc( list, "bmath_fp_mf3_s_lqd"     );
    bcore_array_r_push_sc( list, "bmath_fp_mf3_s_pmt_lqd" );
    bcore_array_r_push_sc( list, "bmath_fp_mf3_s_cld"     );
    bcore_array_r_push_sc( list, "bmath_fp_mf3_s_lud"     );
    bcore_array_r_push_sc( list, "bmath_fp_mf3_s_inv"     );
    bcore_array_r_push_sc( list, "bmath_fp_mf3_s_pdf_inv" );
    bcore_array_r_push_sc( list, "bmath_fp_mf3_s_piv"     );
    bcore_array_r_push_sc( list, "bmath_fp_mf3_s_hsm_piv" );
}

/**********************************************************************************************************************/
/// Matrix object of f3_t

BCORE_DECLARE_OBJECT( bmath_mf3_s )
{
    aware_t _;
    uz_t rows;
    uz_t cols;
    uz_t stride;
    union
    {
        bcore_array_dyn_solid_static_s arr;
        struct
        {
            f3_t* data;
            uz_t  size;
            uz_t  space;
        };
    };
};

void bmath_mf3_s_set_size( bmath_mf3_s* o, uz_t rows, uz_t cols );

/** Sets all matrix elements to random values.
 *  hsm: true: Creates a symmetric matrix
 *  pdf: true: Creates a positive definite matrix
 *  rd: Rank deficit
 *     If rd < min( cols, rows ) at density == 1.0, the matrix-rank can be expected to be equal to min( cols, rows ) - rd.
 *     If rd >= min( cols, rows ), a zero-matrix is created.
 *     If density is < 1.0, it is matrix-rank <= min( cols, rows ) - rd.
 *
 *  Random generator:
 *    Parameters density, min, max, p_rval apply to the random generator.
 *      rval: Pointer to running variable of random generator.
 *            If NULL, an internal fixed random seed is used.
 *
 *     density (range [0.0, 1.0]) specifies the rate at which the random generator
 *     creates a non-zero value.
 *
 *  If rd == 0 && !pdf, the matrix elements are set directly to random values.
 *  Otherwise o is a product of two random matrices.
 */
void bmath_mf3_s_set_random( bmath_mf3_s* o, bl_t hsm, bl_t pdf, uz_t rd, f3_t density, f3_t min, f3_t max, u2_t* p_rval );

bmath_mf3_s* bmath_mf3_s_create_set_size( uz_t rows, uz_t cols );
bmath_mf3_s* bmath_mf3_s_create_fill_random( uz_t rows, uz_t cols, f3_t min, f3_t max, u2_t* rval );

static inline void bmath_mf3_s_set_size_to( const bmath_mf3_s* o, bmath_mf3_s* res ) { bmath_mf3_s_set_size( res, o->rows, o->cols ); }
static inline bl_t bmath_mf3_s_is_equ_size( const bmath_mf3_s* o, const bmath_mf3_s* op ) { return o->rows == op->rows && o->cols == op->cols; }
static inline bl_t bmath_mf3_s_is_square( const bmath_mf3_s* o ) { return o->rows == o->cols; }

/**********************************************************************************************************************/
/// checks, deviations

/** Near-state means: For each matrix element the absolute difference
 *  to the specified state is less or equal max_dev.
 *  Hence, max_dev == 0 tests for the exact state (s. below).
 */
bl_t bmath_mf3_s_is_near_equ( const bmath_mf3_s* o, const bmath_mf3_s* op, f3_t max_dev ); // equality
bl_t bmath_mf3_s_is_near_one( const bmath_mf3_s* o, f3_t max_dev ); // one (== identity)
bl_t bmath_mf3_s_is_near_zro( const bmath_mf3_s* o, f3_t max_dev ); // zero
bl_t bmath_mf3_s_is_near_otn( const bmath_mf3_s* o, f3_t max_dev ); // near orthonormal
bl_t bmath_mf3_s_is_near_uni( const bmath_mf3_s* o, f3_t max_dev ); // near unitary
bl_t bmath_mf3_s_is_near_dag( const bmath_mf3_s* o, f3_t max_dev ); // diagonal
bl_t bmath_mf3_s_is_near_trd( const bmath_mf3_s* o, f3_t max_dev ); // symmetric tri-diagonal
bl_t bmath_mf3_s_is_near_utr( const bmath_mf3_s* o, f3_t max_dev ); // upper triangle
bl_t bmath_mf3_s_is_near_ltr( const bmath_mf3_s* o, f3_t max_dev ); // lower triangle
bl_t bmath_mf3_s_is_near_hsm( const bmath_mf3_s* o, f3_t max_dev ); // symmetric
bl_t bmath_mf3_s_is_near_ubd( const bmath_mf3_s* o, f3_t max_dev ); // upper bi-diagonal
bl_t bmath_mf3_s_is_near_lbd( const bmath_mf3_s* o, f3_t max_dev ); // lower bi-diagonal

static inline bl_t bmath_mf3_s_is_equ( const bmath_mf3_s* o, const bmath_mf3_s* op ) { return bmath_mf3_s_is_near_equ( o, op, 0 ); }
static inline bl_t bmath_mf3_s_is_one( const bmath_mf3_s* o ) { return bmath_mf3_s_is_near_one( o, 0 ); }
static inline bl_t bmath_mf3_s_is_zro( const bmath_mf3_s* o ) { return bmath_mf3_s_is_near_zro( o, 0 ); }
static inline bl_t bmath_mf3_s_is_otn( const bmath_mf3_s* o ) { return bmath_mf3_s_is_near_otn( o, 0 ); }
static inline bl_t bmath_mf3_s_is_uni( const bmath_mf3_s* o ) { return bmath_mf3_s_is_near_uni( o, 0 ); }
static inline bl_t bmath_mf3_s_is_dag( const bmath_mf3_s* o ) { return bmath_mf3_s_is_near_dag( o, 0 ); }
static inline bl_t bmath_mf3_s_is_trd( const bmath_mf3_s* o ) { return bmath_mf3_s_is_near_trd( o, 0 ); }
static inline bl_t bmath_mf3_s_is_utr( const bmath_mf3_s* o ) { return bmath_mf3_s_is_near_utr( o, 0 ); }
static inline bl_t bmath_mf3_s_is_ltr( const bmath_mf3_s* o ) { return bmath_mf3_s_is_near_ltr( o, 0 ); }
static inline bl_t bmath_mf3_s_is_hsm( const bmath_mf3_s* o ) { return bmath_mf3_s_is_near_hsm( o, 0 ); }
static inline bl_t bmath_mf3_s_is_ubd( const bmath_mf3_s* o ) { return bmath_mf3_s_is_near_ubd( o, 0 ); }
static inline bl_t bmath_mf3_s_is_lbd( const bmath_mf3_s* o ) { return bmath_mf3_s_is_near_lbd( o, 0 ); }

//---------------------------------------------------------------------------------------------------------------------
// Frobenius norm

/** fdev = ||f(o) - x||
 *  '|| ... ||' = Frobenius norm
 *  f is either idenity (o-->o) or a specifed function
 *  Matrix x is a specified (or implied) state.
 *  Note: By this definition fdev_zro is the Frobenius norm of o.
 */
f3_t bmath_mf3_s_fdev_equ( const bmath_mf3_s* o, const bmath_mf3_s* op );
f3_t bmath_mf3_s_fdev_zro( const bmath_mf3_s* o );
f3_t bmath_mf3_s_fdev_one( const bmath_mf3_s* o );
f3_t bmath_mf3_s_fdev_otn( const bmath_mf3_s* o ); // f = o * oT or oT * o (whichever smaller); x = I

/**********************************************************************************************************************/
/// initializations; copying; basic matrix operations

f3_t bmath_mf3_s_f3_trc( const bmath_mf3_s* o ); // trace
f3_t bmath_mf3_s_f3_sub_sqr( const bmath_mf3_s* o, const bmath_mf3_s* op ); // ( o - op )^2

void bmath_mf3_s_zro(       bmath_mf3_s* o ); // set zero
void bmath_mf3_s_one(       bmath_mf3_s* o ); // set diagonal elements one, rest zero
void bmath_mf3_s_neg( const bmath_mf3_s* o, bmath_mf3_s* res );  // negate
void bmath_mf3_s_sub( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res );
void bmath_mf3_s_cpy( const bmath_mf3_s* o, bmath_mf3_s* res );  // copies content o -> res  (does not change allocation of res)

//---------------------------------------------------------------------------------------------------------------------
// transposition / permutation

void bmath_mf3_s_htp(         const bmath_mf3_s* o, bmath_mf3_s* res );
void bmath_mf3_s_pmt_mul(     const bmath_mf3_s* o, const bmath_pmt_s* p, bmath_mf3_s* res ); // B = P * A   ; row_p[k]( B ) = row_k( A )
void bmath_mf3_s_pmt_htp_mul( const bmath_mf3_s* o, const bmath_pmt_s* p, bmath_mf3_s* res ); // B = P^T * A ; row_k( B ) = row_p[k]( A )
void bmath_mf3_s_mul_pmt(     const bmath_mf3_s* o, const bmath_pmt_s* p, bmath_mf3_s* res ); // B = A * P   ; col_k( B ) = col_p[k]( A )
void bmath_mf3_s_mul_pmt_htp( const bmath_mf3_s* o, const bmath_pmt_s* p, bmath_mf3_s* res ); // B = A * P^T ; col_p[k]( B ) = col_k( A )

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
void bmath_mf3_s_udu_htp( const bmath_mf3_s* u, const bmath_vf3_s* d, bmath_mf3_s* res ); // res = u * d * u^T

//---------------------------------------------------------------------------------------------------------------------
// inversion; pseudo-inversion;

/** Function require a certain form or well-behaveness of the input matrix to succeed.
    In case of failure, false is returned and the resulting matrix, though numerically valid,
    not necessarily a valid result of the intended operation.
 */

bl_t bmath_mf3_s_inv(     const bmath_mf3_s* o, bmath_mf3_s* res ); // res = o^-1
bl_t bmath_mf3_s_inv_htp( const bmath_mf3_s* o, bmath_mf3_s* res ); // res = (o^-1)T
bl_t bmath_mf3_s_pdf_inv( const bmath_mf3_s* o, bmath_mf3_s* res ); // res = o^-1 in case o is positive definite (3x faster than bmath_mf3_s_inv)

/** Pseudo-Inversion:
 *  Inversion via SVD/EVD by setting near-zero singular values to zero (cut-off).
 *  A singular value s is cut off when abs( s ) < eps * max( { abs(s_0), ..., abs(s_n-1) } ).
 *  Returns success of underlying SVD/EVD.
 *  In case of non-success res contains a result but it might not be the true pseudo inverse.
 */
bl_t bmath_mf3_s_piv(     const bmath_mf3_s* o, f3_t eps, bmath_mf3_s* res ); // general matrix o
bl_t bmath_mf3_s_hsm_piv( const bmath_mf3_s* o, f3_t eps, bmath_mf3_s* res ); // faster inversion in case o is symmetric

/** Affine inversion.
 *  res = av1-inverse of o.
 *  o and res deemed to be av1-transformations
 */
bl_t bmath_mf3_s_inv_av1(     const bmath_mf3_s* o, bmath_mf3_s* res );
bl_t bmath_mf3_s_pdf_inv_av1( const bmath_mf3_s* o, bmath_mf3_s* res );           // o positive definite
bl_t bmath_mf3_s_hsm_piv_av1( const bmath_mf3_s* o, f3_t eps, bmath_mf3_s* res ); // pseudo inversion; o symmetric
bl_t bmath_mf3_s_div(         const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res );

/**********************************************************************************************************************/
/// element-access; col-access; row-access; sub-matrix

void bmath_mf3_s_set_row_by_data( bmath_mf3_s* o, uz_t idx, const f3_t* data );
void bmath_mf3_s_set_col_by_data( bmath_mf3_s* o, uz_t idx, const f3_t* data );
void bmath_mf3_s_set_dag_by_data( bmath_mf3_s* o,           const f3_t* data );
void bmath_mf3_s_set_row_by_vec ( bmath_mf3_s* o, uz_t idx, const bmath_vf3_s* vec );
void bmath_mf3_s_set_col_by_vec ( bmath_mf3_s* o, uz_t idx, const bmath_vf3_s* vec );
void bmath_mf3_s_set_dag_by_vec ( bmath_mf3_s* o,           const bmath_vf3_s* vec );
void bmath_mf3_s_get_row_vec( const bmath_mf3_s* o, uz_t idx, bmath_vf3_s* res );
void bmath_mf3_s_get_col_vec( const bmath_mf3_s* o, uz_t idx, bmath_vf3_s* res );
void bmath_mf3_s_get_dag_vec( const bmath_mf3_s* o,           bmath_vf3_s* res );

void bmath_mf3_s_swap_row( bmath_mf3_s* o, uz_t i, uz_t j ); // swaps rows i, j
void bmath_mf3_s_swap_col( bmath_mf3_s* o, uz_t i, uz_t j ); // swaps cols i, j

void bmath_mf3_s_mul_f3_to_row( bmath_mf3_s* o, f3_t v, uz_t i ); // multiplies v to all values in row i
void bmath_mf3_s_mul_f3_to_col( bmath_mf3_s* o, f3_t v, uz_t i ); // multiplies v to all values in col i

static inline
void bmath_mf3_s_set_f3( bmath_mf3_s* o, uz_t row, uz_t col, f3_t v )
{
    assert( row < o->rows && col <= o->cols );
    o->data[ row * o->stride + col ] = v;
}

static inline
f3_t bmath_mf3_s_get_f3( const bmath_mf3_s* o, uz_t row, uz_t col )
{
    assert( row < o->rows && col <= o->cols );
    return o->data[ row * o->stride + col ];
}

/** Returns a weak (rows x cols) sub matrix at offset (row, col) from o.
 *  Returned object does not own its data and need not be shut down unless it is resized.
 */
bmath_mf3_s bmath_mf3_s_get_weak_sub_mat( const bmath_mf3_s* o, uz_t row, uz_t col, uz_t rows, uz_t cols );

/** Returns a weak row vector.
 *  Returned object does not own its data and need not be shut down unless it is resized.
 */
bmath_vf3_s bmath_mf3_s_get_row_weak_vec( const bmath_mf3_s* o, uz_t idx );

/**********************************************************************************************************************/
/// Triangular decompositions, operations and solvers

/** Cholesky decomposition. Returns succcess.
 *  Succeeds when o is positive definite.
 *  If false is returned res is lower triangular but not a valid cholesky decomposition of o.
 *  Only the lower triangle of o is evaluated.
 *  res: represents the lower-triangular version
 *  It is o = res * resT.
 */
bl_t bmath_mf3_s_decompose_cholesky( const bmath_mf3_s* o, bmath_mf3_s* res );

/** LU decomposition. Returns succcess.
 *  TODO: The current implementation is unstable and not guaranteed to succeed
 *  even when o is non-singular --> use partial pivoting to fix this.
 *
 *  If false is returned res is a LU-composite but not a LU decomposition of o.
 *  res: represents a LU-composite matrix (LUC):
 *       U is upper triangle of LUC
 *       L is lower triangle of LUC except diagonal
 *       diagonal of L is all 1 and not stored.
 */
bl_t bmath_mf3_s_decompose_luc( const bmath_mf3_s* o, bmath_mf3_s* res );

/** Inversion and h-transposition of lower triangular matrix.
 *  o is deemed lower triangular (only lower triangular elements are evaluated)
 *  res is upper triangular
 *  It is: 1 = o * resT = resT * o
 *  Algorithm always finishes.
 *  If o singular, incomputable elements are set to zero.
 */
bl_t bmath_mf3_s_ltr_inv_htp( const bmath_mf3_s* o, bmath_mf3_s* res );

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

/**********************************************************************************************************************/
/// Other decompositions

/** Stable in-place tri-diagonal decomposition for a symmetric matrix.
 *  Based on Givens rotations.
 *  Input:  a  (symmetric),    v  (preallocated or NULL)
 *  Output: a' (tri-diagonal), v' (rotation or NULL)
 *  It is vT * a * v = v'T * a' * v'.
 */
void bmath_mf3_s_trd(     bmath_mf3_s* a, bmath_mf3_s* v );
void bmath_mf3_s_trd_htp( bmath_mf3_s* a, bmath_mf3_s* v );

/** Stable bidiagonal decomposition for a general mxn-matrix a -> a'. Based on givens rotations.
 *  It is a = u * a' * vT, with u, v being unitary.
 *      * Supports any n,m configuration
 *      * Supports full and thin decomposition.
 *      * Highly efficient on dense and sparse matrices. (Automatically detects and exploits sparsity)
 *
 *  If matrices u, v are desired, their size has to be preset but they need not be preinitialized.
 *  The routine does not change the actual allocation of either matrix, hence u, a, v may
 *  safely reference external data.
 *  Whether full or thin decomposition is computed depends on how matrices have been preset:
 *
 *     a:  mxn input matrix to be diagonalized
 *
 *     u:
 *         NULL         -  u is not computed
 *         mxm matrix   -  full decomposition
 *         mxn matrix   -  thin decomposition for n < m
 *
 *     v:
 *         NULL         -  v is not computed
 *         nxn matrix   -  full decomposition
 *         nxm matrix   -  thin decomposition for m < n
 *
 *  If thin decomposition is chosen, then a' is returned as (thin) square matrix.
 *  Meaning either a->rows or a->cols is changed to min(m,n) by the routine.
 *
 */
void bmath_mf3_s_ubd( bmath_mf3_s* u, bmath_mf3_s* a, bmath_mf3_s* v ); // upper bidiagonal
void bmath_mf3_s_lbd( bmath_mf3_s* u, bmath_mf3_s* a, bmath_mf3_s* v ); // lower bidiagonal

/** Turns an upper bidiagonal matrix into the lower bidiagonal form a -> a'
 *  by postmultiplying a unitary matrix.
 *  If v is provided, it is modified v -> v' such that a * vT = a' * v'T
 *  (v may be full, thin or NULL)
 */
void bmath_mf3_s_ubd_to_lbd( bmath_mf3_s* a, bmath_mf3_s* v );

/** Turns a lower bidiagonal matrix into the upper bidiagonal form a -> a'
 *  by pre-multiplying a unitary matrix.
 *  If u is provided, it is modified u -> u' such that u * a = u' * a'
 *  (u may be full, thin or NULL)
 */
void bmath_mf3_s_lbd_to_ubd( bmath_mf3_s* u, bmath_mf3_s* a );

/** ubd_to_lbd with v deemed transposed. */
void bmath_mf3_s_ubd_to_lbd_htp( bmath_mf3_s* a, bmath_mf3_s* v );

/** lbd_to_vbd with u deemed transposed. */
void bmath_mf3_s_lbd_to_ubd_htp( bmath_mf3_s* u, bmath_mf3_s* a );

/**********************************************************************************************************************/
/// Covariance

/** Sets o to the covariance matrix of a section of arr_vec:
 *  oij = E( E( vi - E( vi ) )E( vj - E( vj ) ) )
 */
void bmath_mf3_s_set_covariance_on_section_fast( bmath_mf3_s* o, bmath_arr_vf3_s* arr_vec, uz_t start, uz_t end ); // fast
void bmath_mf3_s_set_covariance_on_section_sprc( bmath_mf3_s* o, bmath_arr_vf3_s* arr_vec, uz_t start, uz_t end ); // stochastically precise

/**********************************************************************************************************************/
/// Givens Rotations

/// rotate two adjacent rows
static inline void bmath_mf3_s_arow_rotate( bmath_mf3_s* o, uz_t idx, const bmath_grt_f3_s* grt, uz_t col_start, uz_t col_end )
{
    if( grt->s != 0 && col_end > col_start )
    {
        f3_t* row_a = o->data + o->stride * idx;
        f3_t* row_b = row_a + o->stride;
//        for( uz_t i = col_start; i < col_end; i++ ) bmath_grt_f3_s_rotate( grt, row_a + i, row_b + i );
        bmath_simd_f3_row_rotate( row_a + col_start, row_b + col_start, col_end - col_start, grt );
    }
}

//---------------------------------------------------------------------------------------------------------------------

/// rotate two distant rows
static inline void bmath_mf3_s_drow_rotate( bmath_mf3_s* o, uz_t a_idx, uz_t b_idx, const bmath_grt_f3_s* grt, uz_t col_start, uz_t col_end )
{
    if( grt->s != 0 && col_end > col_start )
    {
        f3_t* row_a = o->data + o->stride * a_idx;
        f3_t* row_b = o->data + o->stride * b_idx;
//        for( uz_t i = col_start; i < col_end; i++ ) bmath_grt_f3_s_rotate( grt, row_a + i, row_b + i );
        bmath_simd_f3_row_rotate( row_a + col_start, row_b + col_start, col_end - col_start, grt );
    }
}

//---------------------------------------------------------------------------------------------------------------------

/// rotate two adjacent cols
static inline void bmath_mf3_s_acol_rotate( bmath_mf3_s* o, uz_t idx, const bmath_grt_f3_s* grt, uz_t row_start, uz_t row_end )
{
    if( grt->s != 0 && row_start < row_end )
    {
        f3_t* col_a = o->data + idx;
        f3_t* col_b = col_a + 1;

//        for( uz_t i = row_start; i < row_end; i++ ) bmath_grt_f3_s_rotate( grt, col_a + i * o->stride, col_b + i * o->stride );

        bmath_simd_f3_col_rotate( col_a + row_start * o->stride, col_b + row_start * o->stride, o->stride, row_end - row_start, grt );
    }
}

//---------------------------------------------------------------------------------------------------------------------

/// rotate two distant cols
static inline void bmath_mf3_s_dcol_rotate( bmath_mf3_s* o, uz_t a_idx, uz_t b_idx, const bmath_grt_f3_s* grt, uz_t row_start, uz_t row_end )
{
    if( grt->s != 0 && row_start < row_end )
    {
        f3_t* col_a = o->data + a_idx;
        f3_t* col_b = o->data + b_idx;

//        for( uz_t i = row_start; i < row_end; i++ ) bmath_grt_f3_s_rotate( grt, col_a + i * o->stride, col_b + i * o->stride );

        bmath_simd_f3_col_rotate( col_a + row_start * o->stride, col_b + row_start * o->stride, o->stride, row_end - row_start, grt );
    }
}

//---------------------------------------------------------------------------------------------------------------------

/// apply a forward row-swipe (start --> end; end - start rotations)
static inline void bmath_mf3_s_arow_swipe_fwd( bmath_mf3_s* o, uz_t idx, const bmath_arr_grt_f3_s* grt, uz_t col_start, uz_t col_end )
{
    assert( grt->size >= col_end - 1 );
    f3_t* row = o->data + o->stride * idx;
    for( uz_t i = col_start; i < col_end; i++ ) bmath_grt_f3_s_rotate( grt->data + i, row + i, row + i + 1 );
}

//---------------------------------------------------------------------------------------------------------------------

/// apply a reverse adjacent row-swipe (end --> start; end - start rotations)
static inline void bmath_mf3_s_arow_swipe_rev( bmath_mf3_s* o, uz_t idx, const bmath_arr_grt_f3_s* grt, uz_t col_start, uz_t col_end )
{
    assert( grt->size >= col_end - 1 );
    f3_t* row = o->data + o->stride * idx;
    for( uz_t i = col_end; i > col_start; i-- ) bmath_grt_f3_s_rotate( grt->data + i - 1, row + i - 1, row + i );
}

//---------------------------------------------------------------------------------------------------------------------

/// forward sweep of adjacent row rotations (row_start --> row_end)
void bmath_mf3_s_sweep_arow_rotate_fwd( bmath_mf3_s* o, uz_t row_start, uz_t row_end, const bmath_arr_grt_f3_s* grt, uz_t col_start, uz_t col_end );

/// reverse sweep of adjacent row rotations (row_end --> row_start)
void bmath_mf3_s_sweep_arow_rotate_rev( bmath_mf3_s* o, uz_t row_start, uz_t row_end, const bmath_arr_grt_f3_s* grt, uz_t col_start, uz_t col_end );

/// forward sweep of adjacent col rotations (col_start --> col_end)
void bmath_mf3_s_sweep_acol_rotate_fwd( bmath_mf3_s* o, uz_t col_start, uz_t col_end, const bmath_arr_grt_f3_s* grt, uz_t row_start, uz_t row_end );

/// reverse sweep of distant col rotations (col_end --> col_start)
void bmath_mf3_s_sweep_dcol_rotate_rev( bmath_mf3_s* o, uz_t col_start, uz_t col_end, const bmath_arr_grt_f3_s* grt, uz_t row_start, uz_t row_end );

//---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// Development support

// for easy inspection
void bmath_mf3_s_to_string( const bmath_mf3_s* o, st_s* string );
void bmath_mf3_s_to_stdout( const bmath_mf3_s* o );

/**********************************************************************************************************************/

vd_t bmath_mf3_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_MATRIX_H
