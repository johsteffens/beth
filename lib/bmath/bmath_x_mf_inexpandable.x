/** Author and Copyright 2021 Johannes Bernhard Steffens
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
 *    tss: total sum of squares
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
 *    udv: recombination of a diagonal matrix (reverse of SVD): U * D * V^T
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

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/** Group of signatures with function descriptions.
 *  Refer to this group to look up function descriptions.
 *  All functions are implemented in bmath_mf3 and bmath_mf2.
 *  In bmath_mf2 some functions have type adapted arguments (f2_t, bmath_vf2_s, ...)
 *  but exhibit the same principal behavior.
 */
group bmath_mf =
{
    /******************************************************************************************************************/
    /// Basic matrix operations

    signature void clear( m@* o );
    signature void set_size( m@* o, uz_t rows, uz_t cols );

    /** Sets all matrix elements to random values.
     *  hsm: true: Creates a symmetric matrix
     *  pdf: true: Creates a positive definite matrix
     *  rank_deficit > 0: Embeds a rank deficit
     *     If rank_deficit < min( cols, rows ) at density == 1.0, the matrix-rank can be expected to be equal to min( cols, rows ) - rank_deficit.
     *     If rank_deficit >= min( cols, rows ), a zero-matrix is created.
     *     If density is < 1.0, it is matrix-rank <= min( cols, rows ) - rank_deficit.
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
     *
     *  Note:
     *  Even with rank_deficit == 0, a large matrix can be near enough to singular
     *  such that an attempt of matrix inversion fails. Use set_random_full_rank
     *  in order to create a full rank square matrix with better control over singular
     *  value distribution.
     */
    signature void set_random(    m@* o, bl_t hsm, bl_t pdf, sz_t rank_deficit, f3_t density, f3_t min, f3_t max, m bcore_prsg* prsg );
    signature void set_random_u3( m@* o, bl_t hsm, bl_t pdf, sz_t rank_deficit, f3_t density, f3_t min, f3_t max, u3_t* p_rval );

    /** Creates a random full rank square matrix
     *  pdf: true: Creates a positive definite matrix
     *  eps: Specifies the ratio of minimal to maximal magnitude of singular values.
     */
    signature void set_random_full_rank(    m@* o, bl_t pdf, f3_t eps, m bcore_prsg* prsg );
    signature void set_random_full_rank_u3( m@* o, bl_t pdf, f3_t eps, u3_t* p_rval );

    signature m@* create_set_size   ( uz_t rows, uz_t cols );
    signature m@* create_fill_random( uz_t rows, uz_t cols, f3_t min, f3_t max, u2_t* rval );

    signature void set_size_to( @* o, m@* res );
    signature bl_t is_equ_size( @* o,  @* op  );
    signature bl_t is_square  ( @* o );
    signature bl_t is_folded  ( @* o );

    /******************************************************************************************************************/
    /// weak init & conversion (Returned object does not own its data and need not be shut down unless it is resized)

    signature void init_weak( m@* o, uz_t rows, uz_t cols, uz_t stride, m f3_t* data );
    signature  d@* create_weak(      uz_t rows, uz_t cols, uz_t stride, m f3_t* data );

    /// Returns a weak row vector.
    signature bmath_vf3_s get_row_weak_vec( @* o, uz_t idx );

    /******************************************************************************************************************/
    /// checks, deviations

    /** Near-state means: For each matrix element the absolute difference
     *  to the specified state is less or equal max_dev.
     *  Hence, max_dev == 0 tests for the exact state (s. below).
     */
    signature bl_t is_near_equ( @* o, @* op, f3_t max_dev ); // equality
    signature bl_t is_near_one( @* o, f3_t max_dev ); // one (== identity)
    signature bl_t is_near_zro( @* o, f3_t max_dev ); // zero
    signature bl_t is_near_otn( @* o, f3_t max_dev ); // near orthonormal
    signature bl_t is_near_uni( @* o, f3_t max_dev ); // near unitary
    signature bl_t is_near_dag( @* o, f3_t max_dev ); // diagonal
    signature bl_t is_near_trd( @* o, f3_t max_dev ); // symmetric tri-diagonal
    signature bl_t is_near_utr( @* o, f3_t max_dev ); // upper triangle
    signature bl_t is_near_ltr( @* o, f3_t max_dev ); // lower triangle
    signature bl_t is_near_hsm( @* o, f3_t max_dev ); // symmetric
    signature bl_t is_near_ubd( @* o, f3_t max_dev ); // upper bi-diagonal
    signature bl_t is_near_lbd( @* o, f3_t max_dev ); // lower bi-diagonal

    signature bl_t is_equ( @* o, @* op );
    signature bl_t is_one( @* o );
    signature bl_t is_zro( @* o );
    signature bl_t is_otn( @* o );
    signature bl_t is_uni( @* o );
    signature bl_t is_dag( @* o );
    signature bl_t is_trd( @* o );
    signature bl_t is_utr( @* o );
    signature bl_t is_ltr( @* o );
    signature bl_t is_hsm( @* o );
    signature bl_t is_ubd( @* o );
    signature bl_t is_lbd( @* o );

    /// Matrix is nan if at least one element is nan
    signature bl_t is_nan( @* o );

    /// total sum of squares
    signature f3_t tss( @* o );

    ///------------------------------------------------------------------------------------------------------------------
    /// Frobenius norm

    /** fdev = ||f(o) - x||
     *  '|| ... ||' = Frobenius norm  ( sqrt(sum over squares) )
     *  f is either idenity (o-->o) or a specifed function
     *  Matrix x is a specified (or implied) state.
     *  Note: By this definition fdev_zro is the Frobenius norm of o.
     */
    signature f3_t fdev_equ( @* o, @* op );
    signature f3_t fdev_zro( @* o );
    signature f3_t fdev_one( @* o );
    signature f3_t fdev_otn( @* o ); // f = o * oT or oT * o (whichever smaller); x = I

    /******************************************************************************************************************/
    /// Retrieving basic properties

    signature f3_t fx_max( @* o ); // maximum value
    signature f3_t fx_min( @* o ); // minimum value
    signature f3_t fx_sum( @* o ); // sum of all elements

    signature f3_t fx_trc( @* o ); // trace
    signature f3_t fx_sub_sqr( @* o, @* op ); // ( o - op )^2

    /******************************************************************************************************************/
    /// Initializations

    /// functions below support folded target
    signature void zro(       m@* o ); // set zero;                             supports folded matrix
    signature void one(       m@* o ); // set diagonal elements one, rest zero; supports folded matrix
    signature void neg( @* o, m@* r ); // negate
    signature void cpy( @* o, m@* r ); // copies content o -> r  (does not change allocation of res)

    ///------------------------------------------------------------------------------------------------------------------
    /// transposition / permutation

    signature void htp        ( @* o, m@* res );
    signature void pmt_mul    ( @* o, const bmath_pmt_s* p, m@* res ); // B = P * A   ; row_p[k]( B ) = row_k( A )
    signature void pmt_htp_mul( @* o, const bmath_pmt_s* p, m@* res ); // B = P^T * A ; row_k( B ) = row_p[k]( A )
    signature void mul_pmt    ( @* o, const bmath_pmt_s* p, m@* res ); // B = A * P   ; col_k( B ) = col_p[k]( A )
    signature void mul_pmt_htp( @* o, const bmath_pmt_s* p, m@* res ); // B = A * P^T ; col_p[k]( B ) = col_k( A )
    signature void htp_set    ( @* o, m@* res );

    ///------------------------------------------------------------------------------------------------------------------
    /// addition, subtraction

    signature void sub( @* o, @* b, m@* r ); // supports folded target
    signature void add( @* o, @* b, m@* r ); // supports folded target

    ///------------------------------------------------------------------------------------------------------------------
    /// outer vector product

    signature void opd(         m@* o, bmath_vf3_s* a, bmath_vf3_s* b );               // opd( a, b )     -> o
    signature void opd_add(     m@* o, bmath_vf3_s* a, bmath_vf3_s* b,         @* c ); // opd( a, b ) + c -> o
    signature void opd_mul_add( m@* o, bmath_vf3_s* a, bmath_vf3_s* b, f3_t w, @* c ); // opd( a, b ) * w + c -> o

    ///------------------------------------------------------------------------------------------------------------------
    /// matrix * vector [ + vector] --> vector

    signature void mul_vec    ( @* o, bmath_vf3_s* v,   m bmath_vf3_s* r ); // r = o * v
    signature void mul_av1    ( @* o, bmath_vf3_s* av1, m bmath_vf3_s* r ); // affine transformation (see nomenclature 'av1')
    signature void htp_mul_vec( @* o, bmath_vf3_s* v,   m bmath_vf3_s* r ); // r = o^T * v

    signature void mul_vec_add    ( @* o, bmath_vf3_s* v, bmath_vf3_s* b, m bmath_vf3_s* r ); // r = o   * v + b  (in place for b==r)
    signature void htp_mul_vec_add( @* o, bmath_vf3_s* v, bmath_vf3_s* b, m bmath_vf3_s* r ); // r = o^T * v + b  (in place for b==r)

    ///------------------------------------------------------------------------------------------------------------------
    /// matrix * scalar + matrix --> matrix; supports folded r

    signature void mul_scl(     @* o, f3_t* b,       m@* r ); // r = o * b
    signature void mul_scl_add( @* o, f3_t* b, @* c, m@* r ); // r = o * b + c

    signature void mul_scl_fx(     @* o, f3_t b,       m@* r );
    signature void mul_scl_fx_add( @* o, f3_t b, @* c, m@* r );

    ///------------------------------------------------------------------------------------------------------------------
    /// matrix multiplication functions (see bmath_template_mfx_mul_h.h for details)

    /// o * m -> r
    signature void mul(     @* o, @* m, m@* r ); // r can be folded
    signature void mul_esp( @* o, @* m, m@* r );

    /// o * m^T -> r
    signature void mul_htp(     @* o, @* m, m@* r ); // r can be folded
    signature void mul_htp_esp( @* o, @* m, m@* r );

    /// o^T * m -> r
    signature void htp_mul(     @* o, @* m, m@* r ); // r can be folded
    signature void htp_mul_esp( @* o, @* m, m@* r );

    /// o^T * m^T -> r
    signature void htp_mul_htp( @* o, @* m, m@* r ); // r can be folded

    ///  u * d * u^T -> r
    signature void mul_udu_htp(     @* u, const bmath_vf3_s* d, m@* r );
    signature void mul_udu_htp_esp( @* u, const bmath_vf3_s* d, m@* r );

    ///  u * d * v^T -> r
    signature void mul_udv_htp(     @* u, const bmath_vf3_s* d, @* v, m@* r );
    signature void mul_udv_htp_esp( @* u, const bmath_vf3_s* d, @* v, m@* r );

    ///  u * t * v^T -> r with t being tridiagonal
    signature void mul_utv_htp(     @* u, @* t, @* v, m@* r );
    signature void mul_utv_htp_esp( @* u, @* t, @* v, m@* r );

    /// ( o * b ) + c -> r
    signature void mul_add( @* o, @* b, @* c, m@* r );

    /// ( o * b^T ) + c -> r
    signature void mul_htp_add( @* o, @* b, @* c, m@* r );

    /// ( o^T * b ) + c -> r
    signature void htp_mul_add( @* o, @* b, @* c, m@* r );

    /// ( o^T * b^T ) + c -> r
    signature void htp_mul_htp_add( @* o, @* b, @* c, m@* r );

    /** composite multiply-add function. Satisfies functionality of BLAS:DGEMM. Supports folded r.
     *  op(a) * op(b) * c + d * e -> r
     *  op(x) = htpx ? x^T : x;
     *  c, e are scalar.
     *  d can be NULL
     */
    signature void mul_add_cps( bl_t htpa, @* a, bl_t htpb, @* b, f3_t c, @* d, f3_t e, m@* r );

    //------------------------------------------------------------------------------------------------------------------
    /// matrix * matrix --> matrix (result matrix is allocated)

    signature void mul_set(     @* o, @* m, m@* r );
    signature void mul_htp_set( @* o, @* m, m@* r );
    signature void htp_mul_set( @* o, @* m, m@* r );

    ///------------------------------------------------------------------------------------------------------------------
    /// matrix o matrix --> matrix (hadamard product)

    signature void mul_hdm( @* a, @* b, m@* r ); /// a o b -> r; (elementwise operation)

    ///------------------------------------------------------------------------------------------------------------------
    /// inversion; pseudo-inversion;

    /** Hard inversion (inv, pdf_inv) functions fail in case o is close to singular.
        Inv and pdf_inv are based on Cholesky decomposition.
        In case of failure, false is returned and the resulting matrix, though numerically valid,
        may not necessarily be a valid result of the intended operation.
        If the matrix is known to be positive definite, pdf_inv is faster and more accurate than inv.
        If stability is more important than speed, pseudo inversion ...piv (below) should be preferred over
        hard inversion.
     */
    signature bl_t inv(     @* o, m@* res ); // res = o^-1
    signature bl_t inv_htp( @* o, m@* res ); // res = (o^-1)T
    signature bl_t pdf_inv( @* o, m@* res ); // res = o^-1 in case o is positive definite (faster than bmath_mfx_s_inv)

    signature bl_t inv_via_cld(     @* o, m@* res ); // inversion via Cholesky decomposition

    /** Pseudo-Inversion:
     *  Inversion via SVD/EVD by setting near-zero singular values to zero (cut-off).
     *  A singular value s is cut off when abs( s ) < eps * max( { abs(s_0), ..., abs(s_n-1) } ).
     *  Returns success of underlying SVD/EVD.
     *  In case of non-success res contains a result but it might not be the true pseudo inverse.
     */
    signature bl_t piv(     @* o, f3_t eps, m@* res ); // general matrix o
    signature bl_t hsm_piv( @* o, f3_t eps, m@* res ); // faster inversion in case o is symmetric

    /** Affine inversion.
     *  res = av1-inverse of o.
     *  o and res deemed to be av1-transformations
     */
    signature bl_t inv_av1(     @* o, m@* res );
    signature bl_t pdf_inv_av1( @* o, m@* res );           // o positive definite
    signature bl_t hsm_piv_av1( @* o, f3_t eps, m@* res ); // pseudo inversion; o symmetric

    /// res = o * op^-1 via hard inversion; returns success of inversion.
    signature bl_t div( @* o, @* op, m@* res );

    /******************************************************************************************************************/
    /// element-access; col-access; row-access; sub-matrix

    signature void set_row_by_data( m@* o, uz_t idx, f3_t* data );
    signature void set_col_by_data( m@* o, uz_t idx, f3_t* data );
    signature void set_dag_by_data( m@* o,           f3_t* data );
    signature void set_row_by_vec ( m@* o, uz_t idx, bmath_vf3_s* vec );
    signature void set_col_by_vec ( m@* o, uz_t idx, bmath_vf3_s* vec );
    signature void set_dag_by_vec ( m@* o,           bmath_vf3_s* vec );
    signature void get_row_vec( @* o, uz_t idx, m bmath_vf3_s* res );
    signature void get_col_vec( @* o, uz_t idx, m bmath_vf3_s* res );
    signature void get_dag_vec( @* o,           m bmath_vf3_s* res );

    signature void swap_row( m@* o, uz_t i, uz_t j ); // swaps rows i, j
    signature void swap_col( m@* o, uz_t i, uz_t j ); // swaps cols i, j

    signature void mul_fx_to_row( m@* o, f3_t v, uz_t i ); // multiplies v to all values in row i
    signature void mul_fx_to_col( m@* o, f3_t v, uz_t i ); // multiplies v to all values in col i

    signature void set_fx( m@* o, uz_t row, uz_t col, f3_t v );

    signature f3_t get_fx( @* o, uz_t row, uz_t col );

    /******************************************************************************************************************/
    /// Triangular decompositions, operations and solvers

    /** Cholesky decomposition. Returns succcess.
     *  Requires o to be positive definite.
     *  If false is returned res is lower triangular but not a valid cholesky decomposition of o.
     *  res: represents the lower-triangular version (upper triangle is set to zero)
     *  It is o = res * resT.
     */
    signature bl_t decompose_cholesky( @* o, m@* res );

    /** Inversion of lower triangular matrix.
     *  o is deemed lower triangular (only lower triangular elements are evaluated)
     *  res is lower triangular
     *  It is: 1 = o * res = res * o
     *  Algorithm always finishes.
     *  If o is singular, incomputable elements are set to zero.
     */
    signature bl_t ltr_inv( @* o, m@* res );

    /** Inversion and h-transposition of lower triangular matrix.
     *  o is deemed lower triangular (only lower triangular elements are evaluated)
     *  res is upper triangular
     *  It is: 1 = o * resT = resT * o
     *  Algorithm always finishes.
     *  If o is singular, incomputable elements are set to zero.
     */
    signature bl_t ltr_inv_htp( @* o, m@* res );

    /** Multiplication of o with o(transposed) of triangular matrix. (res = o * oT)
     *  Only triangular elements of o are evaluated
     *  res is symmetric
     */
    signature void ltr_mul_htp( @* o, m@* res ); // lower triangular
    signature void utr_mul_htp( @* o, m@* res ); // upper triangular

    /** Multiplication of special matrix with a vector res = o * op.
     *  Only relevant triangular elements of o are evaluated.
     *  In-Place for op==res.
     */
    signature void ltr_mul_vec( @* o, bmath_vf3_s* op, m bmath_vf3_s* res );
    signature void lt1_mul_vec( @* o, bmath_vf3_s* op, m bmath_vf3_s* res );
    signature void utr_mul_vec( @* o, bmath_vf3_s* op, m bmath_vf3_s* res );
    signature void luc_mul_vec( @* o, bmath_vf3_s* op, m bmath_vf3_s* res );

    /** Multiplication of special matrix with a transposed matrix, transposed result: resT = o * opT.
     *  Only relevant triangular elements of o are evaluated.
     *  In-Place for op==res.
     */
    signature void ltr_mul_htp_htp( @* o, @* op, m@* res );
    signature void lt1_mul_htp_htp( @* o, @* op, m@* res );
    signature void utr_mul_htp_htp( @* o, @* op, m@* res );
    signature void luc_mul_htp_htp( @* o, @* op, m@* res );

    /** Solves multiplication of special matrix with a vector: op = o * res.
     *  Only relevant triangular elements of o are evaluated.
     *  In-Place for op==res.
     */
    signature void ltr_solve_vec( @* o, bmath_vf3_s* op, m bmath_vf3_s* res );
    signature void lt1_solve_vec( @* o, bmath_vf3_s* op, m bmath_vf3_s* res );
    signature void utr_solve_vec( @* o, bmath_vf3_s* op, m bmath_vf3_s* res );
    signature void luc_solve_vec( @* o, bmath_vf3_s* op, m bmath_vf3_s* res );

    /** Solves multiplication of special matrix with a transposed matrix, transposed result: opT = o * resT.
     *  Only relevant triangular elements of o are evaluated.
     *  In-Place for op==res.
     */
    signature void ltr_solve_htp_htp( @* o, @* op, m@* res );
    signature void lt1_solve_htp_htp( @* o, @* op, m@* res );
    signature void utr_solve_htp_htp( @* o, @* op, m@* res );
    signature void luc_solve_htp_htp( @* o, @* op, m@* res );

    /******************************************************************************************************************/
    /// Bi/tri-diagonal decompositions

    /** Stable in-place tri-diagonal decomposition for a symmetric matrix.
     *  Based on Givens rotations.
     *  Input:  a  (symmetric),    v  (preallocated or NULL)
     *  Output: a' (tri-diagonal), v' (rotation or NULL)
     *  It is vT * a * v = v'T * a' * v'.
     */
    signature void trd(     m@* a, m@* v );
    signature void trd_htp( m@* a, m@* v );

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
    signature void ubd( m@* u, m@* a, m@* v ); // upper bidiagonal
    signature void lbd( m@* u, m@* a, m@* v ); // lower bidiagonal

    /** Turns an upper bidiagonal matrix into the lower bidiagonal form a -> a'
     *  by postmultiplying a unitary matrix.
     *  If v is provided, it is modified v -> v' such that a * vT = a' * v'T
     *  (v may be full, thin or NULL)
     */
    signature void ubd_to_lbd( m@* a, m@* v );

    /** Turns a lower bidiagonal matrix into the upper bidiagonal form a -> a'
     *  by pre-multiplying a unitary matrix.
     *  If u is provided, it is modified u -> u' such that u * a = u' * a'
     *  (u may be full, thin or NULL)
     */
    signature void lbd_to_ubd( m@* u, m@* a );

    /** ubd_to_lbd with v deemed transposed. */
    signature void ubd_to_lbd_htp( m@* a, m@* v );

    /** lbd_to_vbd with u deemed transposed. */
    signature void lbd_to_ubd_htp( m@* u, m@* a );

    /******************************************************************************************************************/
    /// (QRD) QR-Decomposition, (LQD) LQ-Decomposition,

    /** Stable QR decomposition for a general mxn-matrix a -> a'. Based on givens rotations.
     *  It is a = u * a', with u being unitary.
     *      * Supports any n,m configuration
     *      * Supports full and thin decomposition.
     *      * Highly efficient on dense and sparse matrices. (Automatically detects and exploits sparsity)
     *
     *  If matrix u is desired, its size has to be preset but it need not be preinitialized.
     *  The routine does not change the actual allocation of either matrix, hence u, a may
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
     *  If thin decomposition is chosen, then a' is returned as (thin) nxn square matrix.
     *
     */
    signature void qrd( m@* u, m@* a );

    //------------------------------------------------------------------------------------------------------------------

    /** Stable rank revealing QRP decomposition for a general mxn-matrix a -> a'. Based on givens rotations.
     *  It is a = u * a' * p^T, with u being unitary.
     *  Diagonal elements of a' are non-negative and sorted in descending manner.
     *      * Supports any n,m configuration
     *      * Supports full and thin decomposition.
     *      * Highly efficient on dense and sparse matrices. (Automatically detects and exploits sparsity)
     *
     *  If matrix u is desired, its size has to be preset but it need not be preinitialized.
     *  The routine does not change the actual allocation of either matrix, hence u, a may
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
     *     p:
     *         NULL         -  p is not computed
     *         n-permutation
     *
     *  If thin decomposition is chosen, then a' is returned as (thin) nxn square matrix.
     *
     */
    signature void qrd_pmt( m@* u, m@* a, m@* p );

    //------------------------------------------------------------------------------------------------------------------

    /** Stable LQ decomposition for a general mxn-matrix a -> a'. Based on givens rotations.
     *  It is a = a' * v^T, with v being unitary.
     *      * Supports any n,m configuration
     *      * Supports full and thin decomposition.
     *      * Highly efficient on dense and sparse matrices. (Automatically detects and exploits sparsity)
     *
     *  If matrix v is desired, its size has to be preset but it need not be preinitialized.
     *  The routine does not change the actual allocation of either matrix, hence v, a may
     *  safely reference external data.
     *  Whether full or thin decomposition is computed depends on how matrices have been preset:
     *
     *     a:  mxn input matrix to be diagonalized
     *
     *     v:
     *         NULL         -  v is not computed
     *         nxn matrix   -  full decomposition
     *         nxm matrix   -  thin decomposition for m < n
     *
     *  If thin decomposition is chosen, then a' is returned as (thin) mxm square matrix.
     *
     */
    signature void lqd( m@* a, m@* v );

    //------------------------------------------------------------------------------------------------------------------

    /** Stable rank revealing PLQ decomposition for a general mxn-matrix a -> a'. Based on givens rotations.
     *  It is a = p * a' * v^T, with v being unitary.
     *  Diagonal elements of a' are non-negative and sorted in descending manner.
     *      * Supports any n,m configuration
     *      * Supports full and thin decomposition.
     *      * Efficient on dense and sparse matrices. (Automatically detects and exploits sparsity)
     *
     *  If matrix v is desired, its size has to be preset but it need not be preinitialized.
     *  The routine does not change the actual allocation of either matrix, hence a, v may
     *  safely reference external data.
     *  Whether full or thin decomposition is computed depends on how matrices have been preset:
     *
     *     a:  mxn input matrix to be diagonalized
     *
     *     v:
     *         NULL         -  v is not computed
     *         nxn matrix   -  full decomposition
     *         nxm matrix   -  thin decomposition for m < n
     *
     *     p:
     *         NULL         -  p is not computed
     *         m-permutation
     *
     *  If thin decomposition is chosen, then a' is returned as (thin) mxm square matrix.
     *
     */
    signature void pmt_lqd( m@* p, m@* a, m@* v );

    /******************************************************************************************************************/
    /// (EVD) Eigenvalue Decomposition of a Symmetric Matrix

    /** Stable in-place EVD for a symmetric matrix. Jacobi Method.
     *  Input:  a  (symmetric), v  (preallocated or NULL)
     *  Output: a' (diagonal),  v' (rotation) with a = v'T * a' * v'.
     *  Returns 'true' on successful convergence, 'false' otherwise with a' likely not being diagonal.
     *  (Convergence failure is very rare.)
     *  Diagonal elements are sorted in descending value order.
     *  v == NULL allowed, in which case only a' is computed.
     */
    signature bl_t evd_htp_jacobi( m@* a, m@* v );

    /** In-place EVD for a symmetric matrix.
     *  Approach: TRD, QR with explicit shifting. (Variant of Francis' QR-Algorithm; Developed by J.B.Steffens)
     *  Input:  a  (symmetric), v  (preallocated or NULL)
     *  Output: a' (diagonal),  v' (rotation) with a = v'T * a' * v'.
     *  Returns 'true' on successful convergence, 'false' otherwise with a' likely not being diagonal.
     *  (Convergence failure is very rare.)
     *  Diagonal elements are sorted in descending value order.
     *  v == NULL allowed, in which case only a' is computed.
     */
    signature bl_t evd_htp( m@* a, m@* v );

    /******************************************************************************************************************/
    /// (SVD) Singular Value Decomposition

    /** Stable singular value decomposition for a general mxn-matrix a -> a'.
     *  Method: Bi-diagonalization by givens rotations and QR-chasing with stabilized implicit shift.
     *          (Variant of Golub-Reinsch-Algorithm; Developed by J.B.Steffens)
     *
     *      * Supports any n,m configuration
     *      * Supports full and thin decomposition.
     *      * Automatically detects and exploits sparsity
     *
     *  It is a = u * a' * vT, with u, v being unitary.
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
     *  Returns false in case convergence could not be achieved.
     */
    signature bl_t svd( m@* u, m@* a, m@* v );

    /******************************************************************************************************************/
    /// Covariance

    /** Sets o to the covariance matrix of a section of arr_vec:
     *  oij = E( E( vi - E( vi ) )E( vj - E( vj ) ) )
     */
    signature void set_covariance_on_section_fast( m@* o, m bmath_arr_vf3_s* arr_vec, uz_t start, uz_t end ); // fast
    signature void set_covariance_on_section_sprc( m@* o, m bmath_arr_vf3_s* arr_vec, uz_t start, uz_t end ); // stochastically precise

    /******************************************************************************************************************/
    /// Givens Rotations

    /// rotate two adjacent rows
    signature void arow_rotate( m@* o, uz_t idx, bmath_grt_f3_s* grt, uz_t col_start, uz_t col_end );

    /// rotate two distant rows
    signature void drow_rotate( m@* o, uz_t a_idx, uz_t b_idx, bmath_grt_f3_s* grt, uz_t col_start, uz_t col_end );

    /// rotate two adjacent cols
    signature void acol_rotate( m@* o, uz_t idx, bmath_grt_f3_s* grt, uz_t row_start, uz_t row_end );

    /// rotate two distant cols
    signature void dcol_rotate( m@* o, uz_t a_idx, uz_t b_idx, bmath_grt_f3_s* grt, uz_t row_start, uz_t row_end );

    /// apply a forward row-swipe (start --> end; end - start rotations)
    signature void arow_swipe_fwd( m@* o, uz_t idx, bmath_arr_grt_f3_s* grt, uz_t col_start, uz_t col_end );

    /// apply a reverse adjacent row-swipe (end --> start; end - start rotations)
    signature void arow_swipe_rev( m@* o, uz_t idx, bmath_arr_grt_f3_s* grt, uz_t col_start, uz_t col_end );

    /// forward sweep of adjacent row rotations (row_start --> row_end)
    signature void sweep_arow_rotate_fwd( m@* o, uz_t row_start, uz_t row_end, bmath_arr_grt_f3_s* grt, uz_t col_start, uz_t col_end );

    /// reverse sweep of adjacent row rotations (row_end --> row_start)
    signature void sweep_arow_rotate_rev( m@* o, uz_t row_start, uz_t row_end, bmath_arr_grt_f3_s* grt, uz_t col_start, uz_t col_end );

    /// forward sweep of adjacent col rotations (col_start --> col_end)
    signature void sweep_acol_rotate_fwd( m@* o, uz_t col_start, uz_t col_end, bmath_arr_grt_f3_s* grt, uz_t row_start, uz_t row_end );

    /// reverse sweep of distant col rotations (col_end --> col_start)
    signature void sweep_dcol_rotate_rev( m@* o, uz_t col_start, uz_t col_end, bmath_arr_grt_f3_s* grt, uz_t row_start, uz_t row_end );

    /******************************************************************************************************************/
    /// Convolution

    /** Turns the matrix into a convolution-operand on which a multi-kernel convolution can be performed via matrix multiplication.
     *  Matrix is first interpreted as vector of size rows * cols (all rows catenated).
     *  After execution cols = kernel_size, rows = number of convolution steps. Rows overlap with step_size as offset.
     *  Note that the resulting matrix stores fewer actual values than its size (rows * cols) might indicate.
     *  No matrix data is actually moved in memory by this function.
     */
    signature void to_conv_operand( m@* o, sz_t kernel_size, sz_t step_size );

    /******************************************************************************************************************/
    /// Development support

    // for easy inspection
    signature void to_sink(   @* o, m bcore_sink* sink );
    signature void to_string( @* o, m st_s* string );
    signature void to_stdout( @* o );

    /******************************************************************************************************************/
    /// Type conversion

    signature void copy_typed( m@* o, tp_t type, vc_t src );
    signature void copy_t(     m@* o, tp_t type, vc_t src );
    signature void copy_a(     m@* o,            vc_t src );
    signature d@* clone_t(            tp_t type, vc_t src );
    signature d@* clone_a(                       vc_t src );
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp bmath_mf3_s = bmath_mf
{
    uz_t rows;
    uz_t cols;
    uz_t stride;
    f3_t [];

    /******************************************************************************************************************/
    /// Basic matrix operations

    func bmath_mf.clear;
    func bmath_mf.set_size;
    func bmath_mf.set_random;
    func bmath_mf.set_random_u3;
    func bmath_mf.set_random_full_rank;
    func bmath_mf.set_random_full_rank_u3;
    func bmath_mf.create_set_size;
    func bmath_mf.create_fill_random;
    func bmath_mf.set_size_to;
    func bmath_mf.is_equ_size;
    func bmath_mf.is_square;
    func bmath_mf.is_folded;

    /******************************************************************************************************************/
    /// weak init & conversion (Returned object does not own its data and need not be shut down unless it is resized)

    func bmath_mf.init_weak;
    func bmath_mf.create_weak;
    func bmath_mf.get_row_weak_vec;

    /******************************************************************************************************************/
    /// checks, deviations

    func bmath_mf.is_near_equ;
    func bmath_mf.is_near_one;
    func bmath_mf.is_near_zro;
    func bmath_mf.is_near_otn;
    func bmath_mf.is_near_uni;
    func bmath_mf.is_near_dag;
    func bmath_mf.is_near_trd;
    func bmath_mf.is_near_utr;
    func bmath_mf.is_near_ltr;
    func bmath_mf.is_near_hsm;
    func bmath_mf.is_near_ubd;
    func bmath_mf.is_near_lbd;

    func bmath_mf.is_equ;
    func bmath_mf.is_one;
    func bmath_mf.is_zro;
    func bmath_mf.is_otn;
    func bmath_mf.is_uni;
    func bmath_mf.is_dag;
    func bmath_mf.is_trd;
    func bmath_mf.is_utr;
    func bmath_mf.is_ltr;
    func bmath_mf.is_hsm;
    func bmath_mf.is_ubd;
    func bmath_mf.is_lbd;
    func bmath_mf.is_nan;
    func bmath_mf.tss;

    ///------------------------------------------------------------------------------------------------------------------
    /// Frobenius norm

    func bmath_mf.fdev_equ;
    func bmath_mf.fdev_zro;
    func bmath_mf.fdev_one;
    func bmath_mf.fdev_otn;

    /******************************************************************************************************************/
    /// Retrieving basic properties

    func bmath_mf.fx_max;
    func bmath_mf.fx_min;
    func bmath_mf.fx_sum;

    func bmath_mf.fx_trc;
    func bmath_mf.fx_sub_sqr;

    /******************************************************************************************************************/
    /// Initializations

    func bmath_mf.zro;
    func bmath_mf.one;
    func bmath_mf.neg;
    func bmath_mf.cpy;

    ///------------------------------------------------------------------------------------------------------------------
    /// transposition / permutation

    func bmath_mf.htp;
    func bmath_mf.pmt_mul;
    func bmath_mf.pmt_htp_mul;
    func bmath_mf.mul_pmt;
    func bmath_mf.mul_pmt_htp;
    func bmath_mf.htp_set;

    ///------------------------------------------------------------------------------------------------------------------
    /// addition, subtraction

    func bmath_mf.sub;
    func bmath_mf.add;

    ///------------------------------------------------------------------------------------------------------------------
    /// outer vector product

    func bmath_mf.opd;
    func bmath_mf.opd_add;
    func bmath_mf.opd_mul_add;

    ///------------------------------------------------------------------------------------------------------------------
    /// matrix * vector [ + vector] --> vector

    func bmath_mf.mul_vec;
    func bmath_mf.mul_av1;
    func bmath_mf.htp_mul_vec;
    func bmath_mf.mul_vec_add;
    func bmath_mf.htp_mul_vec_add;

    ///------------------------------------------------------------------------------------------------------------------
    /// matrix * scalar + matrix --> matrix; supports folded r

    func bmath_mf.mul_scl;
    func bmath_mf.mul_scl_add;
    func bmath_mf.mul_scl_fx;
    func bmath_mf.mul_scl_fx_add;

    ///------------------------------------------------------------------------------------------------------------------
    /// matrix multiplication functions (see bmath_template_mfx_mul_h.h for details)

    func bmath_mf.mul;
    func bmath_mf.mul_esp;
    func bmath_mf.mul_htp;
    func bmath_mf.mul_htp_esp;
    func bmath_mf.htp_mul;
    func bmath_mf.htp_mul_esp;
    func bmath_mf.htp_mul_htp;
    func bmath_mf.mul_udu_htp;
    func bmath_mf.mul_udu_htp_esp;
    func bmath_mf.mul_udv_htp;
    func bmath_mf.mul_udv_htp_esp;
    func bmath_mf.mul_utv_htp;
    func bmath_mf.mul_utv_htp_esp;
    func bmath_mf.mul_add;
    func bmath_mf.mul_htp_add;
    func bmath_mf.htp_mul_add;
    func bmath_mf.htp_mul_htp_add;
    func bmath_mf.mul_add_cps;

    //------------------------------------------------------------------------------------------------------------------
    /// matrix * matrix --> matrix (result matrix is allocated)

    func bmath_mf.mul_set;
    func bmath_mf.mul_htp_set;
    func bmath_mf.htp_mul_set;

    ///------------------------------------------------------------------------------------------------------------------
    /// matrix o matrix --> matrix (hadamard product)

    func bmath_mf.mul_hdm;

    ///------------------------------------------------------------------------------------------------------------------
    /// inversion; pseudo-inversion;

    func bmath_mf.inv;
    func bmath_mf.inv_htp;
    func bmath_mf.pdf_inv;
    func bmath_mf.inv_via_cld;
    func bmath_mf.piv;
    func bmath_mf.hsm_piv;
    func bmath_mf.inv_av1;
    func bmath_mf.pdf_inv_av1;
    func bmath_mf.hsm_piv_av1;
    func bmath_mf.div;

    /******************************************************************************************************************/
    /// element-access; col-access; row-access; sub-matrix

    func bmath_mf.set_row_by_data;
    func bmath_mf.set_col_by_data;
    func bmath_mf.set_dag_by_data;
    func bmath_mf.set_row_by_vec;
    func bmath_mf.set_col_by_vec;
    func bmath_mf.set_dag_by_vec;
    func bmath_mf.get_row_vec;
    func bmath_mf.get_col_vec;
    func bmath_mf.get_dag_vec;
    func bmath_mf.swap_row;
    func bmath_mf.swap_col;
    func bmath_mf.mul_fx_to_row;
    func bmath_mf.mul_fx_to_col;
    func bmath_mf.set_fx;
    func bmath_mf.get_fx;

    /******************************************************************************************************************/
    /// Triangular decompositions, operations and solvers

    func bmath_mf.decompose_cholesky;
    func bmath_mf.ltr_inv;
    func bmath_mf.ltr_inv_htp;
    func bmath_mf.ltr_mul_htp;
    func bmath_mf.utr_mul_htp;
    func bmath_mf.ltr_mul_vec;
    func bmath_mf.lt1_mul_vec;
    func bmath_mf.utr_mul_vec;
    func bmath_mf.luc_mul_vec;
    func bmath_mf.ltr_mul_htp_htp;
    func bmath_mf.lt1_mul_htp_htp;
    func bmath_mf.utr_mul_htp_htp;
    func bmath_mf.luc_mul_htp_htp;
    func bmath_mf.ltr_solve_vec;
    func bmath_mf.lt1_solve_vec;
    func bmath_mf.utr_solve_vec;
    func bmath_mf.luc_solve_vec;
    func bmath_mf.ltr_solve_htp_htp;
    func bmath_mf.lt1_solve_htp_htp;
    func bmath_mf.utr_solve_htp_htp;
    func bmath_mf.luc_solve_htp_htp;

    /******************************************************************************************************************/
    /// Bi/tri-diagonal decompositions

    func bmath_mf.trd;
    func bmath_mf.trd_htp;
    func bmath_mf.ubd;
    func bmath_mf.lbd;
    func bmath_mf.ubd_to_lbd;
    func bmath_mf.lbd_to_ubd;
    func bmath_mf.ubd_to_lbd_htp;
    func bmath_mf.lbd_to_ubd_htp;

    /******************************************************************************************************************/
    /// (QRD) QR-Decomposition

    func bmath_mf.qrd;
    func bmath_mf.qrd_pmt;
    func bmath_mf.lqd;
    func bmath_mf.pmt_lqd;

    /******************************************************************************************************************/
    /// (EVD) Eigenvalue Decomposition of a Symmetric Matrix

    func bmath_mf.evd_htp_jacobi;
    func bmath_mf.evd_htp;

    /******************************************************************************************************************/
    /// (SVD) Singular Value Decomposition

    func bmath_mf.svd;

    /******************************************************************************************************************/
    /// Covariance

    func bmath_mf.set_covariance_on_section_fast;
    func bmath_mf.set_covariance_on_section_sprc;

    /******************************************************************************************************************/
    /// Givens Rotations

    func bmath_mf.arow_rotate;
    func bmath_mf.drow_rotate;
    func bmath_mf.acol_rotate;
    func bmath_mf.dcol_rotate;
    func bmath_mf.arow_swipe_fwd;
    func bmath_mf.arow_swipe_rev;
    func bmath_mf.sweep_arow_rotate_fwd;
    func bmath_mf.sweep_arow_rotate_rev;
    func bmath_mf.sweep_acol_rotate_fwd;
    func bmath_mf.sweep_dcol_rotate_rev;

    /******************************************************************************************************************/
    /// Convolution

    func bmath_mf.to_conv_operand;

    /******************************************************************************************************************/
    /// Development support

    func bmath_mf.to_sink;
    func bmath_mf.to_string;
    func bmath_mf.to_stdout;

    /******************************************************************************************************************/
    /// Type conversion

    func bmath_mf.copy_typed;
    func bmath_mf.copy_t;
    func bmath_mf.copy_a;
    func bmath_mf.clone_t;
    func bmath_mf.clone_a;
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp bmath_mf2_s = bmath_mf
{
    uz_t rows;
    uz_t cols;
    uz_t stride;
    f2_t [];

    /******************************************************************************************************************/
    /// Basic matrix operations

    func (void clear( m@* o ));
    func (void set_size( m@* o, uz_t rows, uz_t cols ));
    func (void set_random(    m@* o, bl_t hsm, bl_t pdf, sz_t rank_deficit, f2_t density, f2_t min, f2_t max, m bcore_prsg* prsg ));
    func (void set_random_u3( m@* o, bl_t hsm, bl_t pdf, sz_t rank_deficit, f2_t density, f2_t min, f2_t max, u3_t* p_rval ));
    func (void set_random_full_rank(    m@* o, bl_t pdf, f2_t eps, m bcore_prsg* prsg ));
    func (void set_random_full_rank_u3( m@* o, bl_t pdf, f2_t eps, u3_t* p_rval ));
    func (m@* create_set_size   ( uz_t rows, uz_t cols ));
    func (m@* create_fill_random( uz_t rows, uz_t cols, f2_t min, f2_t max, u2_t* rval ));
    func (void set_size_to( @* o, m@* res ));
    func (bl_t is_equ_size( @* o,  @* op  ));
    func (bl_t is_square  ( @* o ));
    func (bl_t is_folded  ( @* o ));

    /******************************************************************************************************************/
    /// weak init & conversion (Returned object does not own its data and need not be shut down unless it is resized)

    func (void init_weak( m@* o, uz_t rows, uz_t cols, uz_t stride, m f2_t* data ));
    func ( d@* create_weak(      uz_t rows, uz_t cols, uz_t stride, m f2_t* data ));
    func (bmath_vf2_s get_row_weak_vec( @* o, uz_t idx ));

    /******************************************************************************************************************/
    /// checks, deviations

    func (bl_t is_near_equ( @* o, @* op, f2_t max_dev )); // equality
    func (bl_t is_near_one( @* o, f2_t max_dev )); // one (== identity)
    func (bl_t is_near_zro( @* o, f2_t max_dev )); // zero
    func (bl_t is_near_otn( @* o, f2_t max_dev )); // near orthonormal
    func (bl_t is_near_uni( @* o, f2_t max_dev )); // near unitary
    func (bl_t is_near_dag( @* o, f2_t max_dev )); // diagonal
    func (bl_t is_near_trd( @* o, f2_t max_dev )); // symmetric tri-diagonal
    func (bl_t is_near_utr( @* o, f2_t max_dev )); // upper triangle
    func (bl_t is_near_ltr( @* o, f2_t max_dev )); // lower triangle
    func (bl_t is_near_hsm( @* o, f2_t max_dev )); // symmetric
    func (bl_t is_near_ubd( @* o, f2_t max_dev )); // upper bi-diagonal
    func (bl_t is_near_lbd( @* o, f2_t max_dev )); // lower bi-diagonal
    func (bl_t is_equ( @* o, @* op ));
    func (bl_t is_one( @* o ));
    func (bl_t is_zro( @* o ));
    func (bl_t is_otn( @* o ));
    func (bl_t is_uni( @* o ));
    func (bl_t is_dag( @* o ));
    func (bl_t is_trd( @* o ));
    func (bl_t is_utr( @* o ));
    func (bl_t is_ltr( @* o ));
    func (bl_t is_hsm( @* o ));
    func (bl_t is_ubd( @* o ));
    func (bl_t is_lbd( @* o ));
    func (bl_t is_nan( @* o ));
    func (f2_t tss( @* o ));

    ///------------------------------------------------------------------------------------------------------------------
    /// Frobenius norm

    func (f2_t fdev_equ( @* o, @* op ));
    func (f2_t fdev_zro( @* o ));
    func (f2_t fdev_one( @* o ));
    func (f2_t fdev_otn( @* o )); // f = o * oT or oT * o (whichever smaller); x = I

    /******************************************************************************************************************/
    /// Retrieving basic properties

    func (f2_t fx_max( @* o )); // maximum value
    func (f2_t fx_min( @* o )); // minimum value
    func (f2_t fx_sum( @* o )); // sum of all elements
    func (f2_t fx_trc( @* o )); // trace
    func (f2_t fx_sub_sqr( @* o, @* op )); // ( o - op )^2

    ///------------------------------------------------------------------------------------------------------------------
    /// Initializations

    func bmath_mf.zro;
    func bmath_mf.one;
    func bmath_mf.neg;
    func bmath_mf.cpy;

    ///------------------------------------------------------------------------------------------------------------------
    /// transposition / permutation

    func bmath_mf.htp;
    func bmath_mf.pmt_mul;
    func bmath_mf.pmt_htp_mul;
    func bmath_mf.mul_pmt;
    func bmath_mf.mul_pmt_htp;
    func bmath_mf.htp_set;

    ///------------------------------------------------------------------------------------------------------------------
    /// addition, subtraction

    func bmath_mf.sub;
    func bmath_mf.add;

    ///------------------------------------------------------------------------------------------------------------------
    /// outer vector product

    func (void opd(         m@* o, bmath_vf2_s* a, bmath_vf2_s* b ));               // opd( a, b )     -> o
    func (void opd_add(     m@* o, bmath_vf2_s* a, bmath_vf2_s* b,         @* c )); // opd( a, b ) + c -> o
    func (void opd_mul_add( m@* o, bmath_vf2_s* a, bmath_vf2_s* b, f3_t w, @* c )); // opd( a, b ) * w + c -> o

    ///------------------------------------------------------------------------------------------------------------------
    /// matrix * vector [ + vector] --> vector

    func (void mul_vec    ( @* o, bmath_vf2_s* v,   m bmath_vf2_s* r )); // r = o * v
    func (void mul_av1    ( @* o, bmath_vf2_s* av1, m bmath_vf2_s* r )); // affine transformation (see nomenclature 'av1')
    func (void htp_mul_vec( @* o, bmath_vf2_s* v,   m bmath_vf2_s* r )); // r = o^T * v
    func (void mul_vec_add    ( @* o, bmath_vf2_s* v, bmath_vf2_s* b, m bmath_vf2_s* r )); // r = o   * v + b  (in place for b==r)
    func (void htp_mul_vec_add( @* o, bmath_vf2_s* v, bmath_vf2_s* b, m bmath_vf2_s* r )); // r = o^T * v + b  (in place for b==r)

    ///------------------------------------------------------------------------------------------------------------------
    /// matrix * scalar + matrix --> matrix; supports folded r

    func (void mul_scl(     @* o, f2_t* b,       m@* r )); // r = o * b
    func (void mul_scl_add( @* o, f2_t* b, @* c, m@* r )); // r = o * b + c
    func (void mul_scl_fx(     @* o, f2_t b,       m@* r ));
    func (void mul_scl_fx_add( @* o, f2_t b, @* c, m@* r ));

    ///------------------------------------------------------------------------------------------------------------------
    /// matrix multiplication functions (see bmath_template_mfx_mul_h.h for details)

    func bmath_mf.mul;
    func bmath_mf.mul_esp;
    func bmath_mf.mul_htp;
    func bmath_mf.mul_htp_esp;
    func bmath_mf.htp_mul;
    func bmath_mf.htp_mul_esp;
    func bmath_mf.htp_mul_htp;

    func (void mul_udu_htp(     @* u, const bmath_vf2_s* d, m@* r ));
    func (void mul_udu_htp_esp( @* u, const bmath_vf2_s* d, m@* r ));
    func (void mul_udv_htp(     @* u, const bmath_vf2_s* d, @* v, m@* r ));
    func (void mul_udv_htp_esp( @* u, const bmath_vf2_s* d, @* v, m@* r ));

    func bmath_mf.mul_utv_htp;
    func bmath_mf.mul_utv_htp_esp;
    func bmath_mf.mul_add;
    func bmath_mf.mul_htp_add;
    func bmath_mf.htp_mul_add;
    func bmath_mf.htp_mul_htp_add;

    func (void mul_add_cps( bl_t htpa, @* a, bl_t htpb, @* b, f2_t c, @* d, f2_t e, m@* r ));

    //------------------------------------------------------------------------------------------------------------------
    /// matrix * matrix --> matrix (result matrix is allocated)

    func bmath_mf.mul_set;
    func bmath_mf.mul_htp_set;
    func bmath_mf.htp_mul_set;

    //------------------------------------------------------------------------------------------------------------------
    /// matrix o matrix --> matrix (hadamard product)

    func bmath_mf.mul_hdm;

    //------------------------------------------------------------------------------------------------------------------
    /// inversion; pseudo-inversion;

    func (bl_t inv(     @* o, m@* res )); // res = o^-1
    func (bl_t inv_htp( @* o, m@* res )); // res = (o^-1)T
    func (bl_t pdf_inv( @* o, m@* res )); // res = o^-1 in case o is positive definite (faster than bmath_mfx_s_inv)
    func (bl_t inv_via_cld(     @* o, m@* res )); // inversion via Cholesky decomposition
    func (bl_t piv(     @* o, f2_t eps, m@* res )); // general matrix o
    func (bl_t hsm_piv( @* o, f2_t eps, m@* res )); // faster inversion in case o is symmetric
    func (bl_t inv_av1(     @* o, m@* res ));
    func (bl_t pdf_inv_av1( @* o, m@* res ));           // o positive definite
    func (bl_t hsm_piv_av1( @* o, f2_t eps, m@* res )); // pseudo inversion; o symmetric
    func (bl_t div( @* o, @* op, m@* res ));

    /******************************************************************************************************************/
    /// element-access; col-access; row-access; sub-matrix

    func (void set_row_by_data( m@* o, uz_t idx, f2_t* data ));
    func (void set_col_by_data( m@* o, uz_t idx, f2_t* data ));
    func (void set_dag_by_data( m@* o,           f2_t* data ));
    func (void set_row_by_vec ( m@* o, uz_t idx, bmath_vf2_s* vec ));
    func (void set_col_by_vec ( m@* o, uz_t idx, bmath_vf2_s* vec ));
    func (void set_dag_by_vec ( m@* o,           bmath_vf2_s* vec ));
    func (void get_row_vec( @* o, uz_t idx, m bmath_vf2_s* res ));
    func (void get_col_vec( @* o, uz_t idx, m bmath_vf2_s* res ));
    func (void get_dag_vec( @* o,           m bmath_vf2_s* res ));
    func (void swap_row( m@* o, uz_t i, uz_t j )); // swaps rows i, j
    func (void swap_col( m@* o, uz_t i, uz_t j )); // swaps cols i, j
    func (void mul_fx_to_row( m@* o, f2_t v, uz_t i )); // multiplies v to all values in row i
    func (void mul_fx_to_col( m@* o, f2_t v, uz_t i )); // multiplies v to all values in col i
    func (void set_fx( m@* o, uz_t row, uz_t col, f2_t v ));
    func (f2_t get_fx( @* o, uz_t row, uz_t col ));

    /******************************************************************************************************************/
    /// Triangular decompositions, operations and solvers

    func (bl_t decompose_cholesky( @* o, m@* res ));
    func (bl_t ltr_inv( @* o, m@* res ));
    func (bl_t ltr_inv_htp( @* o, m@* res ));
    func (void ltr_mul_htp( @* o, m@* res )); // lower triangular
    func (void utr_mul_htp( @* o, m@* res )); // upper triangular
    func (void ltr_mul_vec( @* o, bmath_vf2_s* op, m bmath_vf2_s* res ));
    func (void lt1_mul_vec( @* o, bmath_vf2_s* op, m bmath_vf2_s* res ));
    func (void utr_mul_vec( @* o, bmath_vf2_s* op, m bmath_vf2_s* res ));
    func (void luc_mul_vec( @* o, bmath_vf2_s* op, m bmath_vf2_s* res ));
    func (void ltr_mul_htp_htp( @* o, @* op, m@* res ));
    func (void lt1_mul_htp_htp( @* o, @* op, m@* res ));
    func (void utr_mul_htp_htp( @* o, @* op, m@* res ));
    func (void luc_mul_htp_htp( @* o, @* op, m@* res ));
    func (void ltr_solve_vec( @* o, bmath_vf2_s* op, m bmath_vf2_s* res ));
    func (void lt1_solve_vec( @* o, bmath_vf2_s* op, m bmath_vf2_s* res ));
    func (void utr_solve_vec( @* o, bmath_vf2_s* op, m bmath_vf2_s* res ));
    func (void luc_solve_vec( @* o, bmath_vf2_s* op, m bmath_vf2_s* res ));
    func (void ltr_solve_htp_htp( @* o, @* op, m@* res ));
    func (void lt1_solve_htp_htp( @* o, @* op, m@* res ));
    func (void utr_solve_htp_htp( @* o, @* op, m@* res ));
    func (void luc_solve_htp_htp( @* o, @* op, m@* res ));

    /******************************************************************************************************************/
    /// Bi/tri-diagonal decompositions

    func bmath_mf.trd;
    func bmath_mf.trd_htp;
    func bmath_mf.ubd;
    func bmath_mf.lbd;
    func bmath_mf.ubd_to_lbd;
    func bmath_mf.lbd_to_ubd;
    func bmath_mf.ubd_to_lbd_htp;
    func bmath_mf.lbd_to_ubd_htp;

    /******************************************************************************************************************/
    /// (QRD) QR-Decomposition

    func bmath_mf.qrd;
    func bmath_mf.qrd_pmt;
    func bmath_mf.lqd;
    func bmath_mf.pmt_lqd;

    /******************************************************************************************************************/
    /// (EVD) Eigenvalue Decomposition of a Symmetric Matrix

    func bmath_mf.evd_htp_jacobi;
    func bmath_mf.evd_htp;

    /******************************************************************************************************************/
    /// (SVD) Singular Value Decomposition

    func bmath_mf.svd;

    /******************************************************************************************************************/
    /// Covariance

    func (void set_covariance_on_section_fast( m@* o, m bmath_arr_vf2_s* arr_vec, uz_t start, uz_t end )); // fast
    func (void set_covariance_on_section_sprc( m@* o, m bmath_arr_vf2_s* arr_vec, uz_t start, uz_t end )); // stochastically precise

    /******************************************************************************************************************/
    /// Givens Rotations

    func (void arow_rotate( m@* o, uz_t idx, bmath_grt_f2_s* grt, uz_t col_start, uz_t col_end ));
    func (void drow_rotate( m@* o, uz_t a_idx, uz_t b_idx, bmath_grt_f2_s* grt, uz_t col_start, uz_t col_end ));
    func (void acol_rotate( m@* o, uz_t idx, bmath_grt_f2_s* grt, uz_t row_start, uz_t row_end ));
    func (void dcol_rotate( m@* o, uz_t a_idx, uz_t b_idx, bmath_grt_f2_s* grt, uz_t row_start, uz_t row_end ));
    func (void arow_swipe_fwd( m@* o, uz_t idx, bmath_arr_grt_f2_s* grt, uz_t col_start, uz_t col_end ));
    func (void arow_swipe_rev( m@* o, uz_t idx, bmath_arr_grt_f2_s* grt, uz_t col_start, uz_t col_end ));
    func (void sweep_arow_rotate_fwd( m@* o, uz_t row_start, uz_t row_end, bmath_arr_grt_f2_s* grt, uz_t col_start, uz_t col_end ));
    func (void sweep_arow_rotate_rev( m@* o, uz_t row_start, uz_t row_end, bmath_arr_grt_f2_s* grt, uz_t col_start, uz_t col_end ));
    func (void sweep_acol_rotate_fwd( m@* o, uz_t col_start, uz_t col_end, bmath_arr_grt_f2_s* grt, uz_t row_start, uz_t row_end ));
    func (void sweep_dcol_rotate_rev( m@* o, uz_t col_start, uz_t col_end, bmath_arr_grt_f2_s* grt, uz_t row_start, uz_t row_end ));

    /******************************************************************************************************************/
    /// Convolution

    func bmath_mf.to_conv_operand;

    /******************************************************************************************************************/
    /// Development support

    func bmath_mf.to_sink;
    func bmath_mf.to_string;
    func bmath_mf.to_stdout;

    /******************************************************************************************************************/
    /// Type conversion

    func bmath_mf.copy_typed;
    func bmath_mf.copy_t;
    func bmath_mf.copy_a;
    func bmath_mf.clone_t;
    func bmath_mf.clone_a;
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
