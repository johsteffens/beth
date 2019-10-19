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

/**********************************************************************************************************************/

/** Matrix Template.
 *
 *  Include in the final header file after object definition
 *  with macro BMATH_TEMPLATE_FX_PREC (temporarily) defined.
 *
 *  Example:
 *
 *  #define BMATH_TEMPLATE_FX_PREC 2 // defines all functions for f2_t
 *  #include "bmath_mfx_h.h"
 */

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
 *    mul_hdm: C = A o B  (hadamard product == elementwise product)
 *    mul_vec: y = A * x
 *    mul_scl: C = A * f (f: scalar)
 *    mul_add:     D = A * B + C
 *    mul_scl_add: D = A * b + C (b: scalar)
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
 *    eop: elementary operation: subsequent operation applies to matrix elements rather than the matrix-object
 *    set: 'when used as postfix' explicitly allocates result object (set_size) for given operation (example bmath_mfx_s_htp_set)
 *
 */

/** Memory Layout
 *  Matrix data is stored in row-major order, where 'stride' indicates the memory offset between two adjacent rows.
 *  Normally stride >= cols is expected. See below for exceptions.
 *
 *  Folded Matrix:
 *  A degenerate case is 0 <= stride < cols. This layout is called 'folded', representing overlapping rows.
 *  The folded format is useful in order to perform convolutions via matrix operations.
 *  A folded matrix as const-argument is globally supported.
 *  Modifying functions only support a folded target when explicitly stated.
 *  Note: Not all functions test the folded state.
 */

/**********************************************************************************************************************/

// NOTE: Do not include non-templates (template definitions could be undone/changed)
#include "bmath_template_fx_begin.h"

/**********************************************************************************************************************/

// mfx features
BCORE_FORWARD_OBJECT( bmath_mfx_s );

typedef void (*bmath_fp_mfx_s_(mul         ))( const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res );
typedef void (*bmath_fp_mfx_s_(mul_htp     ))( const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res );
typedef void (*bmath_fp_mfx_s_(htp_mul     ))( const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res );
typedef void (*bmath_fp_mfx_s_(htp_mul_htp ))( const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res );

typedef void (*bmath_fp_mfx_s_(uav     ))( bmath_mfx_s* u, bmath_mfx_s* a, bmath_mfx_s* v ); // uav decomposition
typedef void (*bmath_fp_mfx_s_(ua      ))( bmath_mfx_s* u, bmath_mfx_s* a                 ); //  ua decomposition
typedef void (*bmath_fp_mfx_s_(av      ))(                 bmath_mfx_s* a, bmath_mfx_s* v ); //  av decomposition
typedef void (*bmath_fp_mfx_s_(uau     ))( bmath_mfx_s* u, bmath_mfx_s* a                 ); // uau decomposition
typedef void (*bmath_fp_mfx_s_(vav     ))(                 bmath_mfx_s* a, bmath_mfx_s* v ); // vav decomposition

typedef void (*bmath_fp_mfx_s_(trd_htp ))(                 bmath_mfx_s* a, bmath_mfx_s* v );
typedef void (*bmath_fp_mfx_s_(trd     ))(                 bmath_mfx_s* a, bmath_mfx_s* v );
typedef bl_t (*bmath_fp_mfx_s_(evd_htp ))(                 bmath_mfx_s* a, bmath_mfx_s* v );
typedef bl_t (*bmath_fp_mfx_s_(svd     ))( bmath_mfx_s* u, bmath_mfx_s* a, bmath_mfx_s* v );
typedef void (*bmath_fp_mfx_s_(ubd     ))( bmath_mfx_s* u, bmath_mfx_s* a, bmath_mfx_s* v );
typedef void (*bmath_fp_mfx_s_(lbd     ))( bmath_mfx_s* u, bmath_mfx_s* a, bmath_mfx_s* v );
typedef void (*bmath_fp_mfx_s_(qrd     ))( bmath_mfx_s* u, bmath_mfx_s* a                 );
typedef void (*bmath_fp_mfx_s_(qrd_pmt ))( bmath_mfx_s* u, bmath_mfx_s* a, bmath_pmt_s* p ); // rank revealing; A = U * A' * P^T
typedef void (*bmath_fp_mfx_s_(lqd     ))(                 bmath_mfx_s* a, bmath_mfx_s* v );
typedef void (*bmath_fp_mfx_s_(pmt_lqd ))( bmath_pmt_s* p, bmath_mfx_s* a, bmath_mfx_s* v ); // rank revealing; A = P^T * A' * V

typedef bl_t (*bmath_fp_mfx_s_(cld     ))( const bmath_mfx_s* o, bmath_mfx_s* res );
typedef bl_t (*bmath_fp_mfx_s_(lud     ))( const bmath_mfx_s* o, bmath_mfx_s* res );
typedef bl_t (*bmath_fp_mfx_s_(inv     ))( const bmath_mfx_s* o, bmath_mfx_s* res );
typedef bl_t (*bmath_fp_mfx_s_(pdf_inv ))( const bmath_mfx_s* o, bmath_mfx_s* res );
typedef bl_t (*bmath_fp_mfx_s_(piv     ))( const bmath_mfx_s* o, fx_t eps, bmath_mfx_s* res );
typedef bl_t (*bmath_fp_mfx_s_(hsm_piv ))( const bmath_mfx_s* o, fx_t eps, bmath_mfx_s* res );

/**********************************************************************************************************************/
/// Matrix object of fx_t (bmath_mfx_s)

void bmath_mfx_s_(set_size)( bmath_mfx_s* o, uz_t rows, uz_t cols );

/** Sets all matrix elements to random values.
 *  hsm: true: Creates a symmetric matrix
 *  pdf: true: Creates a positive definite matrix
 *  rd > 0: Embeds a rank deficit
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
void bmath_mfx_s_(set_random)( bmath_mfx_s* o, bl_t hsm, bl_t pdf, uz_t rd, fx_t density, fx_t min, fx_t max, u2_t* p_rval );

bmath_mfx_s* bmath_mfx_s_(create_set_size)( uz_t rows, uz_t cols );
bmath_mfx_s* bmath_mfx_s_(create_fill_random)( uz_t rows, uz_t cols, fx_t min, fx_t max, u2_t* rval );

static inline void bmath_mfx_s_(set_size_to)( const bmath_mfx_s* o, bmath_mfx_s* res ) { bmath_mfx_s_(set_size)( res, o->rows, o->cols ); }
static inline bl_t bmath_mfx_s_(is_equ_size)( const bmath_mfx_s* o, const bmath_mfx_s* op ) { return o->rows == op->rows && o->cols == op->cols; }
static inline bl_t bmath_mfx_s_(is_square)  ( const bmath_mfx_s* o ) { return o->rows == o->cols; }
static inline bl_t bmath_mfx_s_(is_folded)  ( const bmath_mfx_s* o ) { return o->stride < o->cols; }

/**********************************************************************************************************************/
/// weak int & conversion (Returned object does not own its data and need not be shut down unless it is resized)

static inline void bmath_mfx_s_(init_weak)( bmath_mfx_s* o, uz_t rows, uz_t cols, uz_t stride, fx_t* data )
{
    o->rows = rows;
    o->cols = cols;
    o->stride = stride;
    o->data = data;
    o->size = rows * stride;
    o->space = 0;
}

static inline bmath_mfx_s bmath_mfx_(init_weak)( uz_t rows, uz_t cols, uz_t stride, fx_t* data )
{
    bmath_mfx_s o;
    bmath_mfx_s_(init_weak)( &o, rows, cols, stride, data );
    return o;
}

/// Returns a weak (rows x cols) sub matrix at offset (row, col) from o.
bmath_mfx_s bmath_mfx_s_(get_weak_sub_mat)( const bmath_mfx_s* o, uz_t row, uz_t col, uz_t rows, uz_t cols );

/// Returns a weak row vector.
bmath_vfx_s bmath_mfx_s_(get_row_weak_vec)( const bmath_mfx_s* o, uz_t idx );

/**********************************************************************************************************************/
/// checks, deviations

/** Near-state means: For each matrix element the absolute difference
 *  to the specified state is less or equal max_dev.
 *  Hence, max_dev == 0 tests for the exact state (s. below).
 */
bl_t bmath_mfx_s_(is_near_equ)( const bmath_mfx_s* o, const bmath_mfx_s* op, fx_t max_dev ); // equality
bl_t bmath_mfx_s_(is_near_one)( const bmath_mfx_s* o, fx_t max_dev ); // one (== identity)
bl_t bmath_mfx_s_(is_near_zro)( const bmath_mfx_s* o, fx_t max_dev ); // zero
bl_t bmath_mfx_s_(is_near_otn)( const bmath_mfx_s* o, fx_t max_dev ); // near orthonormal
bl_t bmath_mfx_s_(is_near_uni)( const bmath_mfx_s* o, fx_t max_dev ); // near unitary
bl_t bmath_mfx_s_(is_near_dag)( const bmath_mfx_s* o, fx_t max_dev ); // diagonal
bl_t bmath_mfx_s_(is_near_trd)( const bmath_mfx_s* o, fx_t max_dev ); // symmetric tri-diagonal
bl_t bmath_mfx_s_(is_near_utr)( const bmath_mfx_s* o, fx_t max_dev ); // upper triangle
bl_t bmath_mfx_s_(is_near_ltr)( const bmath_mfx_s* o, fx_t max_dev ); // lower triangle
bl_t bmath_mfx_s_(is_near_hsm)( const bmath_mfx_s* o, fx_t max_dev ); // symmetric
bl_t bmath_mfx_s_(is_near_ubd)( const bmath_mfx_s* o, fx_t max_dev ); // upper bi-diagonal
bl_t bmath_mfx_s_(is_near_lbd)( const bmath_mfx_s* o, fx_t max_dev ); // lower bi-diagonal

static inline bl_t bmath_mfx_s_(is_equ)( const bmath_mfx_s* o, const bmath_mfx_s* op ) { return bmath_mfx_s_(is_near_equ)( o, op, 0 ); }
static inline bl_t bmath_mfx_s_(is_one)( const bmath_mfx_s* o ) { return bmath_mfx_s_(is_near_one)( o, 0 ); }
static inline bl_t bmath_mfx_s_(is_zro)( const bmath_mfx_s* o ) { return bmath_mfx_s_(is_near_zro)( o, 0 ); }
static inline bl_t bmath_mfx_s_(is_otn)( const bmath_mfx_s* o ) { return bmath_mfx_s_(is_near_otn)( o, 0 ); }
static inline bl_t bmath_mfx_s_(is_uni)( const bmath_mfx_s* o ) { return bmath_mfx_s_(is_near_uni)( o, 0 ); }
static inline bl_t bmath_mfx_s_(is_dag)( const bmath_mfx_s* o ) { return bmath_mfx_s_(is_near_dag)( o, 0 ); }
static inline bl_t bmath_mfx_s_(is_trd)( const bmath_mfx_s* o ) { return bmath_mfx_s_(is_near_trd)( o, 0 ); }
static inline bl_t bmath_mfx_s_(is_utr)( const bmath_mfx_s* o ) { return bmath_mfx_s_(is_near_utr)( o, 0 ); }
static inline bl_t bmath_mfx_s_(is_ltr)( const bmath_mfx_s* o ) { return bmath_mfx_s_(is_near_ltr)( o, 0 ); }
static inline bl_t bmath_mfx_s_(is_hsm)( const bmath_mfx_s* o ) { return bmath_mfx_s_(is_near_hsm)( o, 0 ); }
static inline bl_t bmath_mfx_s_(is_ubd)( const bmath_mfx_s* o ) { return bmath_mfx_s_(is_near_ubd)( o, 0 ); }
static inline bl_t bmath_mfx_s_(is_lbd)( const bmath_mfx_s* o ) { return bmath_mfx_s_(is_near_lbd)( o, 0 ); }

/// Matrix is nan if at least one element is nan
bl_t bmath_mfx_s_(is_nan)( const bmath_mfx_s* o );

//----------------------------------------------------------------------------------------------------------------------
// Frobenius norm

/** fdev = ||f(o) - x||
 *  '|| ... ||' = Frobenius norm
 *  f is either idenity (o-->o) or a specifed function
 *  Matrix x is a specified (or implied) state.
 *  Note: By this definition fdev_zro is the Frobenius norm of o.
 */
fx_t bmath_mfx_s_(fdev_equ)( const bmath_mfx_s* o, const bmath_mfx_s* op );
fx_t bmath_mfx_s_(fdev_zro)( const bmath_mfx_s* o );
fx_t bmath_mfx_s_(fdev_one)( const bmath_mfx_s* o );
fx_t bmath_mfx_s_(fdev_otn)( const bmath_mfx_s* o ); // f = o * oT or oT * o (whichever smaller); x = I

/**********************************************************************************************************************/
/// initializations; copying; basic matrix operations

fx_t bmath_mfx_s_(fx_max)( const bmath_mfx_s* o ); // maximum value
fx_t bmath_mfx_s_(fx_min)( const bmath_mfx_s* o ); // minimum value
fx_t bmath_mfx_s_(fx_sum)( const bmath_mfx_s* o ); // sum of all elements

fx_t bmath_mfx_s_(fx_trc)( const bmath_mfx_s* o ); // trace
fx_t bmath_mfx_s_(fx_sub_sqr)( const bmath_mfx_s* o, const bmath_mfx_s* op ); // ( o - op )^2

// functions below support folded target
void bmath_mfx_s_(zro)(       bmath_mfx_s* o ); // set zero;                             supports folded matrix
void bmath_mfx_s_(one)(       bmath_mfx_s* o ); // set diagonal elements one, rest zero; supports folded matrix
void bmath_mfx_s_(neg)( const bmath_mfx_s* o, bmath_mfx_s* r );  // negate
void bmath_mfx_s_(cpy)( const bmath_mfx_s* o, bmath_mfx_s* r );  // copies content o -> r  (does not change allocation of res)

//----------------------------------------------------------------------------------------------------------------------
// transposition / permutation

void bmath_mfx_s_(htp)(         const bmath_mfx_s* o, bmath_mfx_s* res );
void bmath_mfx_s_(pmt_mul)(     const bmath_mfx_s* o, const bmath_pmt_s* p, bmath_mfx_s* res ); // B = P * A   ; row_p[k]( B ) = row_k( A )
void bmath_mfx_s_(pmt_htp_mul)( const bmath_mfx_s* o, const bmath_pmt_s* p, bmath_mfx_s* res ); // B = P^T * A ; row_k( B ) = row_p[k]( A )
void bmath_mfx_s_(mul_pmt)(     const bmath_mfx_s* o, const bmath_pmt_s* p, bmath_mfx_s* res ); // B = A * P   ; col_k( B ) = col_p[k]( A )
void bmath_mfx_s_(mul_pmt_htp)( const bmath_mfx_s* o, const bmath_pmt_s* p, bmath_mfx_s* res ); // B = A * P^T ; col_p[k]( B ) = col_k( A )

static inline void bmath_mfx_s_(htp_set)( const bmath_mfx_s* o, bmath_mfx_s* res ) { bmath_mfx_s_(set_size)( res, o->cols, o->rows ); bmath_mfx_s_(htp)( o, res ); }

//----------------------------------------------------------------------------------------------------------------------
// addition, subtraction

void bmath_mfx_s_(sub)( const bmath_mfx_s* o, const bmath_mfx_s* b, bmath_mfx_s* r ); // supports folded target
void bmath_mfx_s_(add)( const bmath_mfx_s* o, const bmath_mfx_s* b, bmath_mfx_s* r ); // supports folded target

//----------------------------------------------------------------------------------------------------------------------
// outer vector product

void bmath_mfx_s_(opd)(     bmath_mfx_s* o, const bmath_vfx_s* a, const bmath_vfx_s* b );                       // opd( a, b )     -> o
void bmath_mfx_s_(opd_add)( bmath_mfx_s* o, const bmath_vfx_s* a, const bmath_vfx_s* b, const bmath_mfx_s* c ); // opd( a, b ) + c -> o

//----------------------------------------------------------------------------------------------------------------------
// matrix * vector [ + vector] --> vector

void bmath_mfx_s_(mul_vec)(     const bmath_mfx_s* o, const bmath_vfx_s* v,   bmath_vfx_s* r ); // r = o * v
void bmath_mfx_s_(mul_av1)(     const bmath_mfx_s* o, const bmath_vfx_s* av1, bmath_vfx_s* r ); // affine transformation (see nomenclature 'av1')
void bmath_mfx_s_(htp_mul_vec)( const bmath_mfx_s* o, const bmath_vfx_s* v,   bmath_vfx_s* r ); // r = o^T * v

void bmath_mfx_s_(mul_vec_add)(     const bmath_mfx_s* o, const bmath_vfx_s* v, const bmath_vfx_s* b, bmath_vfx_s* r ); // r = o   * v + b  (in place for b==r)
void bmath_mfx_s_(htp_mul_vec_add)( const bmath_mfx_s* o, const bmath_vfx_s* v, const bmath_vfx_s* b, bmath_vfx_s* r ); // r = o^T * v + b  (in place for b==r)

//----------------------------------------------------------------------------------------------------------------------
// matrix * scalar + matrix --> matrix; supports folded r

void bmath_mfx_s_(mul_scl)(     const bmath_mfx_s* o, const fx_t* b,                       bmath_mfx_s* r ); // r = o * b
void bmath_mfx_s_(mul_scl_add)( const bmath_mfx_s* o, const fx_t* b, const bmath_mfx_s* c, bmath_mfx_s* r ); // r = o * b + c

static inline void bmath_mfx_s_(mul_scl_fx)(     const bmath_mfx_s* o, fx_t b,                       bmath_mfx_s* r ) { bmath_mfx_s_(mul_scl)(     o, &b,    r ); }
static inline void bmath_mfx_s_(mul_scl_fx_add)( const bmath_mfx_s* o, fx_t b, const bmath_mfx_s* c, bmath_mfx_s* r ) { bmath_mfx_s_(mul_scl_add)( o, &b, c, r ); }

//----------------------------------------------------------------------------------------------------------------------
// matrix * unary_map --> matrix; supports folded r

void bmath_mfx_s_(eop_map)(     const bmath_mfx_s* o, bmath_fp_fx_(ar1) b, bmath_mfx_s* r ); // r_ij = b( o_ij )
void bmath_mfx_s_(eop_map_mul)( const bmath_mfx_s* o, bmath_fp_fx_(ar1) b, const bmath_mfx_s* c, bmath_mfx_s* r ); // r_ij = b( o_ij ) * c_ij

//----------------------------------------------------------------------------------------------------------------------
// matrix * matrix --> matrix (see also bmath_mfx_mul.h)

static inline void bmath_mfx_s_(mul_set)(     const bmath_mfx_s* o, const bmath_mfx_s* m, bmath_mfx_s* r ) { bmath_mfx_s_(set_size)( r, o->rows, m->cols ); bmath_mfx_s_(mul)( o, m, r ); }
static inline void bmath_mfx_s_(mul_htp_set)( const bmath_mfx_s* o, const bmath_mfx_s* m, bmath_mfx_s* r ) { bmath_mfx_s_(set_size)( r, o->rows, m->rows ); bmath_mfx_s_(mul_htp)( o, m, r ); }
static inline void bmath_mfx_s_(htp_mul_set)( const bmath_mfx_s* o, const bmath_mfx_s* m, bmath_mfx_s* r ) { bmath_mfx_s_(set_size)( r, o->cols, m->cols ); bmath_mfx_s_(htp_mul)( o, m, r ); }

//----------------------------------------------------------------------------------------------------------------------
// matrix o matrix --> matrix (hadamard product)

void bmath_mfx_s_(mul_hdm)( const bmath_mfx_s* a, const bmath_mfx_s* b, bmath_mfx_s* r ); /// a o b -> r; (elementwise operation)

//----------------------------------------------------------------------------------------------------------------------
// inversion; pseudo-inversion;

/** Function require a certain form or well-behaveness of the input matrix to succeed.
    In case of failure, false is returned and the resulting matrix, though numerically valid,
    not necessarily a valid result of the intended operation.
 */

bl_t bmath_mfx_s_(inv)(     const bmath_mfx_s* o, bmath_mfx_s* res ); // res = o^-1
bl_t bmath_mfx_s_(inv_htp)( const bmath_mfx_s* o, bmath_mfx_s* res ); // res = (o^-1)T
bl_t bmath_mfx_s_(pdf_inv)( const bmath_mfx_s* o, bmath_mfx_s* res ); // res = o^-1 in case o is positive definite (3x faster than bmath_mfx_s_inv)

/** Pseudo-Inversion:
 *  Inversion via SVD/EVD by setting near-zero singular values to zero (cut-off).
 *  A singular value s is cut off when abs( s ) < eps * max( { abs(s_0), ..., abs(s_n-1) } ).
 *  Returns success of underlying SVD/EVD.
 *  In case of non-success res contains a result but it might not be the true pseudo inverse.
 */
bl_t bmath_mfx_s_(piv)(     const bmath_mfx_s* o, fx_t eps, bmath_mfx_s* res ); // general matrix o
bl_t bmath_mfx_s_(hsm_piv)( const bmath_mfx_s* o, fx_t eps, bmath_mfx_s* res ); // faster inversion in case o is symmetric

/** Affine inversion.
 *  res = av1-inverse of o.
 *  o and res deemed to be av1-transformations
 */
bl_t bmath_mfx_s_(inv_av1)(     const bmath_mfx_s* o, bmath_mfx_s* res );
bl_t bmath_mfx_s_(pdf_inv_av1)( const bmath_mfx_s* o, bmath_mfx_s* res );           // o positive definite
bl_t bmath_mfx_s_(hsm_piv_av1)( const bmath_mfx_s* o, fx_t eps, bmath_mfx_s* res ); // pseudo inversion; o symmetric
bl_t bmath_mfx_s_(div)(         const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res );

/**********************************************************************************************************************/
/// element-access; col-access; row-access; sub-matrix

void bmath_mfx_s_(set_row_by_data)( bmath_mfx_s* o, uz_t idx, const fx_t* data );
void bmath_mfx_s_(set_col_by_data)( bmath_mfx_s* o, uz_t idx, const fx_t* data );
void bmath_mfx_s_(set_dag_by_data)( bmath_mfx_s* o,           const fx_t* data );
void bmath_mfx_s_(set_row_by_vec )( bmath_mfx_s* o, uz_t idx, const bmath_vfx_s* vec );
void bmath_mfx_s_(set_col_by_vec )( bmath_mfx_s* o, uz_t idx, const bmath_vfx_s* vec );
void bmath_mfx_s_(set_dag_by_vec )( bmath_mfx_s* o,           const bmath_vfx_s* vec );
void bmath_mfx_s_(get_row_vec)( const bmath_mfx_s* o, uz_t idx, bmath_vfx_s* res );
void bmath_mfx_s_(get_col_vec)( const bmath_mfx_s* o, uz_t idx, bmath_vfx_s* res );
void bmath_mfx_s_(get_dag_vec)( const bmath_mfx_s* o,           bmath_vfx_s* res );

void bmath_mfx_s_(swap_row)( bmath_mfx_s* o, uz_t i, uz_t j ); // swaps rows i, j
void bmath_mfx_s_(swap_col)( bmath_mfx_s* o, uz_t i, uz_t j ); // swaps cols i, j

void bmath_mfx_s_(mul_fx_to_row)( bmath_mfx_s* o, fx_t v, uz_t i ); // multiplies v to all values in row i
void bmath_mfx_s_(mul_fx_to_col)( bmath_mfx_s* o, fx_t v, uz_t i ); // multiplies v to all values in col i

static inline
void bmath_mfx_s_(set_fx)( bmath_mfx_s* o, uz_t row, uz_t col, fx_t v )
{
    assert( row < o->rows && col <= o->cols );
    o->data[ row * o->stride + col ] = v;
}

static inline
fx_t bmath_mfx_s_(get_fx)( const bmath_mfx_s* o, uz_t row, uz_t col )
{
    assert( row < o->rows && col <= o->cols );
    return o->data[ row * o->stride + col ];
}

/**********************************************************************************************************************/
/// Triangular decompositions, operations and solvers

/** Cholesky decomposition. Returns succcess.
 *  Succeeds when o is positive definite.
 *  If false is returned res is lower triangular but not a valid cholesky decomposition of o.
 *  Only the lower triangle of o is evaluated.
 *  res: represents the lower-triangular version
 *  It is o = res * resT.
 */
bl_t bmath_mfx_s_(decompose_cholesky)( const bmath_mfx_s* o, bmath_mfx_s* res );

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
bl_t bmath_mfx_s_(decompose_luc)( const bmath_mfx_s* o, bmath_mfx_s* res );

/** Inversion and h-transposition of lower triangular matrix.
 *  o is deemed lower triangular (only lower triangular elements are evaluated)
 *  res is upper triangular
 *  It is: 1 = o * resT = resT * o
 *  Algorithm always finishes.
 *  If o singular, incomputable elements are set to zero.
 */
bl_t bmath_mfx_s_(ltr_inv_htp)( const bmath_mfx_s* o, bmath_mfx_s* res );

/** Multiplication of o with o(transposed) of triangular matrix. (res = o * oT)
 *  Only triangular elements of o are evaluated
 *  res is symmetric
 */
void bmath_mfx_s_(ltr_mul_htp)( const bmath_mfx_s* o, bmath_mfx_s* res ); // lower triangular
void bmath_mfx_s_(utr_mul_htp)( const bmath_mfx_s* o, bmath_mfx_s* res ); // upper triangular

/** Multiplication of special matrix with a vector res = o * op.
 *  Only relevant triangular elements of o are evaluated.
 *  In-Place for op==res.
 */
void bmath_mfx_s_(ltr_mul_vec)( const bmath_mfx_s* o, const bmath_vfx_s* op, bmath_vfx_s* res );
void bmath_mfx_s_(lt1_mul_vec)( const bmath_mfx_s* o, const bmath_vfx_s* op, bmath_vfx_s* res );
void bmath_mfx_s_(utr_mul_vec)( const bmath_mfx_s* o, const bmath_vfx_s* op, bmath_vfx_s* res );
void bmath_mfx_s_(luc_mul_vec)( const bmath_mfx_s* o, const bmath_vfx_s* op, bmath_vfx_s* res );

/** Multiplication of special matrix with a transposed matrix, transposed result: resT = o * opT.
 *  Only relevant triangular elements of o are evaluated.
 *  In-Place for op==res.
 */
void bmath_mfx_s_(ltr_mul_htp_htp)( const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res );
void bmath_mfx_s_(lt1_mul_htp_htp)( const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res );
void bmath_mfx_s_(utr_mul_htp_htp)( const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res );
void bmath_mfx_s_(luc_mul_htp_htp)( const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res );

/** Solves multiplication of special matrix with a vector: op = o * res.
 *  Only relevant triangular elements of o are evaluated.
 *  In-Place for op==res.
 */
void bmath_mfx_s_(ltr_solve_vec)( const bmath_mfx_s* o, const bmath_vfx_s* op, bmath_vfx_s* res );
void bmath_mfx_s_(lt1_solve_vec)( const bmath_mfx_s* o, const bmath_vfx_s* op, bmath_vfx_s* res );
void bmath_mfx_s_(utr_solve_vec)( const bmath_mfx_s* o, const bmath_vfx_s* op, bmath_vfx_s* res );
void bmath_mfx_s_(luc_solve_vec)( const bmath_mfx_s* o, const bmath_vfx_s* op, bmath_vfx_s* res );

/** Solves multiplication of special matrix with a transposed matrix, transposed result: opT = o * resT.
 *  Only relevant triangular elements of o are evaluated.
 *  In-Place for op==res.
 */
void bmath_mfx_s_(ltr_solve_htp_htp)( const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res );
void bmath_mfx_s_(lt1_solve_htp_htp)( const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res );
void bmath_mfx_s_(utr_solve_htp_htp)( const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res );
void bmath_mfx_s_(luc_solve_htp_htp)( const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res );

/**********************************************************************************************************************/
/// Other decompositions

/** Stable in-place tri-diagonal decomposition for a symmetric matrix.
 *  Based on Givens rotations.
 *  Input:  a  (symmetric),    v  (preallocated or NULL)
 *  Output: a' (tri-diagonal), v' (rotation or NULL)
 *  It is vT * a * v = v'T * a' * v'.
 */
void bmath_mfx_s_(trd)(     bmath_mfx_s* a, bmath_mfx_s* v );
void bmath_mfx_s_(trd_htp)( bmath_mfx_s* a, bmath_mfx_s* v );

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
void bmath_mfx_s_(ubd)( bmath_mfx_s* u, bmath_mfx_s* a, bmath_mfx_s* v ); // upper bidiagonal
void bmath_mfx_s_(lbd)( bmath_mfx_s* u, bmath_mfx_s* a, bmath_mfx_s* v ); // lower bidiagonal

/** Turns an upper bidiagonal matrix into the lower bidiagonal form a -> a'
 *  by postmultiplying a unitary matrix.
 *  If v is provided, it is modified v -> v' such that a * vT = a' * v'T
 *  (v may be full, thin or NULL)
 */
void bmath_mfx_s_(ubd_to_lbd)( bmath_mfx_s* a, bmath_mfx_s* v );

/** Turns a lower bidiagonal matrix into the upper bidiagonal form a -> a'
 *  by pre-multiplying a unitary matrix.
 *  If u is provided, it is modified u -> u' such that u * a = u' * a'
 *  (u may be full, thin or NULL)
 */
void bmath_mfx_s_(lbd_to_ubd)( bmath_mfx_s* u, bmath_mfx_s* a );

/** ubd_to_lbd with v deemed transposed. */
void bmath_mfx_s_(ubd_to_lbd_htp)( bmath_mfx_s* a, bmath_mfx_s* v );

/** lbd_to_vbd with u deemed transposed. */
void bmath_mfx_s_(lbd_to_ubd_htp)( bmath_mfx_s* u, bmath_mfx_s* a );

/**********************************************************************************************************************/
/// Covariance

/** Sets o to the covariance matrix of a section of arr_vec:
 *  oij = E( E( vi - E( vi ) )E( vj - E( vj ) ) )
 */
void bmath_mfx_s_(set_covariance_on_section_fast)( bmath_mfx_s* o, bmath_arr_vfx_s* arr_vec, uz_t start, uz_t end ); // fast
void bmath_mfx_s_(set_covariance_on_section_sprc)( bmath_mfx_s* o, bmath_arr_vfx_s* arr_vec, uz_t start, uz_t end ); // stochastically precise

/**********************************************************************************************************************/
/// Givens Rotations

/// rotate two adjacent rows
static inline void bmath_mfx_s_(arow_rotate)( bmath_mfx_s* o, uz_t idx, const bmath_grt_fx_s* grt, uz_t col_start, uz_t col_end )
{
    if( grt->s != 0 && col_end > col_start )
    {
        fx_t* row_a = o->data + o->stride * idx;
        fx_t* row_b = row_a + o->stride;
//        for( uz_t i = col_start; i < col_end; i++ ) bmath_grt_fx_s_(rotate)( grt, row_a + i, row_b + i );
        bmath_simd_fx_(row_rotate)( row_a + col_start, row_b + col_start, col_end - col_start, grt );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/// rotate two distant rows
static inline void bmath_mfx_s_(drow_rotate)( bmath_mfx_s* o, uz_t a_idx, uz_t b_idx, const bmath_grt_fx_s* grt, uz_t col_start, uz_t col_end )
{
    if( grt->s != 0 && col_end > col_start )
    {
        fx_t* row_a = o->data + o->stride * a_idx;
        fx_t* row_b = o->data + o->stride * b_idx;
//        for( uz_t i = col_start; i < col_end; i++ ) bmath_grt_fx_s_(rotate)( grt, row_a + i, row_b + i );
        bmath_simd_fx_(row_rotate)( row_a + col_start, row_b + col_start, col_end - col_start, grt );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/// rotate two adjacent cols
static inline void bmath_mfx_s_(acol_rotate)( bmath_mfx_s* o, uz_t idx, const bmath_grt_fx_s* grt, uz_t row_start, uz_t row_end )
{
    if( grt->s != 0 && row_start < row_end )
    {
        fx_t* col_a = o->data + idx;
        fx_t* col_b = col_a + 1;

//        for( uz_t i = row_start; i < row_end; i++ ) bmath_grt_fx_s_(rotate)( grt, col_a + i * o->stride, col_b + i * o->stride );

        bmath_simd_fx_(col_rotate)( col_a + row_start * o->stride, col_b + row_start * o->stride, o->stride, row_end - row_start, grt );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/// rotate two distant cols
static inline void bmath_mfx_s_(dcol_rotate)( bmath_mfx_s* o, uz_t a_idx, uz_t b_idx, const bmath_grt_fx_s* grt, uz_t row_start, uz_t row_end )
{
    if( grt->s != 0 && row_start < row_end )
    {
        fx_t* col_a = o->data + a_idx;
        fx_t* col_b = o->data + b_idx;

//        for( uz_t i = row_start; i < row_end; i++ ) bmath_grt_fx_s_(rotate)( grt, col_a + i * o->stride, col_b + i * o->stride );

        bmath_simd_fx_(col_rotate)( col_a + row_start * o->stride, col_b + row_start * o->stride, o->stride, row_end - row_start, grt );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/// apply a forward row-swipe (start --> end; end - start rotations)
static inline void bmath_mfx_s_(arow_swipe_fwd)( bmath_mfx_s* o, uz_t idx, const bmath_arr_grt_fx_s* grt, uz_t col_start, uz_t col_end )
{
    assert( grt->size >= col_end - 1 );
    fx_t* row = o->data + o->stride * idx;
    for( uz_t i = col_start; i < col_end; i++ ) bmath_grt_fx_s_(rotate)( grt->data + i, row + i, row + i + 1 );
}

//----------------------------------------------------------------------------------------------------------------------

/// apply a reverse adjacent row-swipe (end --> start; end - start rotations)
static inline void bmath_mfx_s_(arow_swipe_rev)( bmath_mfx_s* o, uz_t idx, const bmath_arr_grt_fx_s* grt, uz_t col_start, uz_t col_end )
{
    assert( grt->size >= col_end - 1 );
    fx_t* row = o->data + o->stride * idx;
    for( uz_t i = col_end; i > col_start; i-- ) bmath_grt_fx_s_(rotate)( grt->data + i - 1, row + i - 1, row + i );
}

//----------------------------------------------------------------------------------------------------------------------

/// forward sweep of adjacent row rotations (row_start --> row_end)
void bmath_mfx_s_(sweep_arow_rotate_fwd)( bmath_mfx_s* o, uz_t row_start, uz_t row_end, const bmath_arr_grt_fx_s* grt, uz_t col_start, uz_t col_end );

/// reverse sweep of adjacent row rotations (row_end --> row_start)
void bmath_mfx_s_(sweep_arow_rotate_rev)( bmath_mfx_s* o, uz_t row_start, uz_t row_end, const bmath_arr_grt_fx_s* grt, uz_t col_start, uz_t col_end );

/// forward sweep of adjacent col rotations (col_start --> col_end)
void bmath_mfx_s_(sweep_acol_rotate_fwd)( bmath_mfx_s* o, uz_t col_start, uz_t col_end, const bmath_arr_grt_fx_s* grt, uz_t row_start, uz_t row_end );

/// reverse sweep of distant col rotations (col_end --> col_start)
void bmath_mfx_s_(sweep_dcol_rotate_rev)( bmath_mfx_s* o, uz_t col_start, uz_t col_end, const bmath_arr_grt_fx_s* grt, uz_t row_start, uz_t row_end );

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// Convolution

/** Turns the matrix into a convolution-operand on which a multi-kernel convolution can be performed via matrix multiplication.
 *  Matrix is first interpreted as vector of size rows * cols (all rows catenated).
 *  After execution cols = kernel_size, rows = number of convolution steps. Rows overlap with step_size as offset.
 *  Note that the resulting matrix stores fewer actual values than its size (rows * cols) might indicate.
 *  No matrix data is actually moved in memory by this function.
 */
void bmath_mfx_s_(to_conv_operand)( bmath_mfx_s* o, sz_t kernel_size, sz_t step_size );

/**********************************************************************************************************************/
/// Development support

// for easy inspection

void bmath_mfx_s_(to_sink)(   const bmath_mfx_s* o, bcore_sink* sink );
void bmath_mfx_s_(to_image)(  const bmath_mfx_s* o, bmath_fp_u2_argb_from_fx fp, vd_t arg, bcore_img_u2_s* image );

static inline void bmath_mfx_s_(to_string)( const bmath_mfx_s* o, st_s* string ) { bmath_mfx_s_(to_sink)( o, ( bcore_sink* )string ); }
static inline void bmath_mfx_s_(to_stdout)( const bmath_mfx_s* o ) { bmath_mfx_s_(to_sink)( o, BCORE_STDOUT ); }

/**********************************************************************************************************************/
// closing template

#include "bmath_template_fx_end.h"

/**********************************************************************************************************************/
