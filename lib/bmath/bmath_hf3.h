/** Copyright 2019 Johannes Bernhard Steffens
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

#ifndef BMATH_HF3_H
#define BMATH_HF3_H

/**********************************************************************************************************************/

/** Holor of f3_t
 *
 *  A holor represents the canonic extension of a matrix (or generalization of a tensor) as inspired by
 *  Moon & Spencer's: "Theory of Holors".
 *
 *  The term holor in this scope is represented as a multidimensional array, where jagged arrays are explicitly excluded.
 *  Meaning that a (n+1)-holor is an array of n-holors of the same data-layout but independent values.
 *
 *  The number of dimensions is called the order of a holor.
 *
 *  A n-holor is understood as holor or order n (meaning n separate dimensions).
 *  The number of values a holor holds is equal to the product of its dimensions.
 *
 *  bmath_hf3_t is organized as array of dimensions (d_data) and array of associated value data (v_data).
 *  Dimension-values are arranged from lowest to highest level.
 *
 *  A given holor is called 'determined' when v_data is set according to its order and dimensionality.
 *  v_data = NULL is a valid state which is called 'vacant'.
 *
 *  Examples:
 *    A 0-holor represents a scalar.
 *    A 1-holor represents a vector; vectors-size == d_data[ 0 ];
 *    A 2-holor represents a matrix: cols == d_data[ 0 ]; rows == d_data[ 1 ];
 *    A n-holor represents an array of k (n-1)-holors with k == d_data[ n - 1 ].
 *
 *  The value-data layout is such that any n-holor can be converted to a mf3-matrix referencing the
 *  value data by grouping the dimensions into cols and rows given an k with 0 <= k <= n, such that
 *    cols == d_data[ 0 ] * .... * d_data[ k - 1 ] and
 *    rows == d_data[ k ] * .... * d_data[ n - 1 ] .
 *    The empty product resolves to 1.
 *
 */

/**********************************************************************************************************************/

#include "bcore_std.h"
#include "bmath_planted.h"
#include "bmath_hwflags.h"
#include "bmath_feature.h"
#include "bmath_f3.h"
#include "bmath_mf3.h"

/**********************************************************************************************************************/

PLANT_GROUP( bmath_hf3, bcore_inst )
#ifdef PLANT_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stamp : = aware bcore_inst
{
    sz_t [] d; // dimension data
    f3_t [] v; // value data
};

/// array dynamic of links
stamp :adl = aware bcore_array { :s => []; };

#endif // PLANT_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**********************************************************************************************************************/

/// turns a weak d-array into a strone one (no effect if it is strong)
void bmath_hf3_s_d_make_strong( bmath_hf3_s* o );

/// turns a weak v-array into a strone one (no effect if it is strong)
void bmath_hf3_s_v_make_strong( bmath_hf3_s* o );

/// sets number of values (initialized to zero)
void bmath_hf3_s_set_v_size( bmath_hf3_s* o, sz_t size );

/// sets number of dimensions (initialized to zero)
void bmath_hf3_s_set_d_size( bmath_hf3_s* o, sz_t size );

/// clears number of values
void bmath_hf3_s_clear_v_data( bmath_hf3_s* o );

/// clears number of dimensions (makes holor of scalar type)
void bmath_hf3_s_clear_d_data( bmath_hf3_s* o );

/// clears holor
void bmath_hf3_s_clear( bmath_hf3_s* o );

/// changes number of values preserving content (pads zero)
void bmath_hf3_s_resize_v_size( bmath_hf3_s* o, sz_t size );

/// changes number of dimensions preserving content (pads zero)
void bmath_hf3_s_resize_d_size( bmath_hf3_s* o, sz_t size );

/// sets number of values (initialized to zero)
void bmath_hf3_s_set_v_size( bmath_hf3_s* o, sz_t size );

/// fits number of values to given dimensions (initialized to zero)
void bmath_hf3_s_fit_v_size( bmath_hf3_s* o );

/// turns holor into vacant state without affecting d_data
void bmath_hf3_s_set_vacant( bmath_hf3_s* o );

/// sets d_data, d_size to array (does not change v_size or v_data)
void bmath_hf3_s_set_d_data( bmath_hf3_s* o, const sz_t* d_data, sz_t d_size );

/// sets v_data, v_size to array (does not change d_size or d_data)
void bmath_hf3_s_set_v_data( bmath_hf3_s* o, const f3_t* v_data, sz_t v_size );

/// copies d_data from src (does not change v_size or v_data)
void bmath_hf3_s_copy_d_data( bmath_hf3_s* o, const bmath_hf3_s* src );

/// copies v_data from src (does not change d_size or d_data)
void bmath_hf3_s_copy_v_data( bmath_hf3_s* o, const bmath_hf3_s* src );

/// copies v_data from src (does not change d_size or d_data)
void bmath_hf3_s_copy_v_data_from_vf3( bmath_hf3_s* o, const bmath_vf3_s* src );

/// copies v_data from src (does not change d_size or d_data)
void bmath_hf3_s_copy_v_data_to_vf3( const bmath_hf3_s* o, bmath_vf3_s* dst );

/// sets size of entire holor including v_data; (v_data initialized to zero)
void bmath_hf3_s_set_size( bmath_hf3_s* o, const sz_t* d_data, sz_t d_size );

/// copies d data and v size (including vacant state) from src;
void bmath_hf3_s_copy_size( bmath_hf3_s* o, const bmath_hf3_s* src );

/// sets d_data via variadic arguments
void bmath_hf3_s_set_d_data_nv( bmath_hf3_s* o, sz_t d_size, va_list sz_t_args );
void bmath_hf3_s_set_d_data_na( bmath_hf3_s* o, sz_t d_size, ... );

/// sets size via variadic arguments
void bmath_hf3_s_set_size_nv( bmath_hf3_s* o, sz_t d_size, va_list sz_t_args );
void bmath_hf3_s_set_size_na( bmath_hf3_s* o, sz_t d_size, ... );

/// returns product of all dimensions
sz_t bmath_hf3_s_d_product( const bmath_hf3_s* o );

/// sets d_data to scalar (no change on v_data)
void bmath_hf3_s_set_d_scalar( bmath_hf3_s* o );

/// sets holor to scalar with given value or to vacant scalar
void bmath_hf3_s_set_scalar_pf3( bmath_hf3_s* o, f3_t* v );

/// sets holor to scalar with given value
void bmath_hf3_s_set_scalar_f3( bmath_hf3_s* o, f3_t v );

/**********************************************************************************************************************/
/// status

bl_t bmath_hf3_s_d_equal( const bmath_hf3_s* o, const bmath_hf3_s* src ); // compares d_data
bl_t bmath_hf3_s_v_equal( const bmath_hf3_s* o, const bmath_hf3_s* src ); // compares v_data (independently of d_data)

bl_t bmath_hf3_s_is_equal(  const bmath_hf3_s* o, const bmath_hf3_s* src );
bl_t bmath_hf3_s_is_scalar( const bmath_hf3_s* o );

/**********************************************************************************************************************/
/// weak conversion

static inline bmath_mf3_s bmath_hf3_s_get_weak_mat( const bmath_hf3_s* o )
{
    assert( o->d_size == 2 );
    sz_t cols = o->d_data[ 0 ];
    sz_t rows = o->d_data[ 1 ];
    assert( o->v_size == cols * rows );
    return bmath_mf3_init_weak( rows, cols, cols, o->v_data );
}

// ---------------------------------------------------------------------------------------------------------------------

static inline bmath_vf3_s bmath_hf3_s_get_weak_vec( const bmath_hf3_s* o )
{
    assert( o->d_size == 1 );
    sz_t size = o->d_data[ 0 ];
    assert( o->v_size == size );
    return bmath_vf3_init_weak( o->v_data, size );
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// holor specific operations

static inline sz_t bmath_hf3_s_get_order( const bmath_hf3_s* o ) { return o->d_size; }

/** Canonic increment of order by appending one dimension 'dim' :
 *  If the holder holds data, the data is duplicated 'dim' times.
 */
void bmath_hf3_s_inc_order( bmath_hf3_s* o, sz_t dim );

/** Canonic data append of a sub-holor
 *  Requirements (checked):
 *  * o and src must either be both vacant or both determined.
 *  * o->d_size == src->d_size || o->d_size == src->d_size + 1
 *  * src->d_data must match (lower significant) o->d_data
 */
void bmath_hf3_s_push( bmath_hf3_s* o, const bmath_hf3_s* src );

void bmath_hf3_s_to_sink(      const bmath_hf3_s* o, bcore_sink* sink );
void bmath_hf3_s_to_sink_nl(   const bmath_hf3_s* o, bcore_sink* sink ); // appends newline
void bmath_hf3_s_to_stdout(    const bmath_hf3_s* o );
void bmath_hf3_s_to_stdout_nl( const bmath_hf3_s* o ); // appends newline

/** compacted version, single line */
void bmath_hf3_s_brief_to_sink(        const bmath_hf3_s* o, bcore_sink* sink );
void bmath_hf3_s_brief_to_stdout(      const bmath_hf3_s* o );

/** multiline version */
void bmath_hf3_s_formatted_to_sink(    const bmath_hf3_s* o, bcore_sink* sink );
void bmath_hf3_s_formatted_to_stdout(  const bmath_hf3_s* o );

/** Sets all vector elements to random values.
 *  Random generator:
 *    Parameters density, min, max, p_rval apply to the random generator.
 *      rval: Pointer to running variable of random generator.
 *            If NULL, an internal fixed random seed is used.
 *
 *     density (range [0.0, 1.0]) specifies the rate at which the random generator
 *     creates a non-zero value.
 */
void bmath_hf3_s_set_random( bmath_hf3_s* o, f3_t density, f3_t min, f3_t max, u2_t* p_rval );

/**********************************************************************************************************************/
/// elementwise operations

/// direct implementations

/// o = {0}
void bmath_hf3_s_zro( const bmath_hf3_s* o );

/// o -> r
void bmath_hf3_s_cpy( const bmath_hf3_s* o, bmath_hf3_s* r );

/// exp(o) -> r
void bmath_hf3_s_exp( const bmath_hf3_s* o, bmath_hf3_s* r );

/// tanh(o) -> r
void bmath_hf3_s_tanh( const bmath_hf3_s* o, bmath_hf3_s* r );

/// relu(o) -> r  (f(x) = x > 0 ? x : 0)
void bmath_hf3_s_relu( const bmath_hf3_s* o, bmath_hf3_s* r );

/// leaky relu(o) -> r  (f(x) = x > 0 ? x : x * 0.01)
void bmath_hf3_s_lrelu( const bmath_hf3_s* o, bmath_hf3_s* r );

/// o + m -> r
void bmath_hf3_s_add( const bmath_hf3_s* o, const bmath_hf3_s* m, bmath_hf3_s* r );

/// o - m -> r
void bmath_hf3_s_sub( const bmath_hf3_s* o, const bmath_hf3_s* m, bmath_hf3_s* r );

/// o <*> m -> r (hadamard product)
void bmath_hf3_s_hmul( const bmath_hf3_s* o, const bmath_hf3_s* m, bmath_hf3_s* r );

void bmath_hf3_s_mul_scl(     const bmath_hf3_s* o, const f3_t* b,                       bmath_hf3_s* r ); // o * b     -> r
void bmath_hf3_s_mul_scl_add( const bmath_hf3_s* o, const f3_t* b, const bmath_hf3_s* c, bmath_hf3_s* r ); // o * b + c -> r

static inline void bmath_hf3_s_mul_scl_f3(     const bmath_hf3_s* o, f3_t b,                       bmath_hf3_s* r ) { bmath_hf3_s_mul_scl(     o, &b,    r ); }
static inline void bmath_hf3_s_mul_scl_f3_add( const bmath_hf3_s* o, f3_t b, const bmath_hf3_s* c, bmath_hf3_s* r ) { bmath_hf3_s_mul_scl_add( o, &b, c, r ); }

f3_t bmath_hf3_s_f3_max( const bmath_hf3_s* o );
f3_t bmath_hf3_s_f3_min( const bmath_hf3_s* o );
f3_t bmath_hf3_s_f3_sum( const bmath_hf3_s* o );

/// operators

/// bmath_fp_f3_unary(o) -> r
void bmath_hf3_s_f3_unary( const bmath_hf3_s* o, bmath_fp_f3_unary unary, bmath_hf3_s* r );

/// f3_op_ar0() -> o
void bmath_hf3_s_f3_op_ar0(       bmath_hf3_s* o, const bmath_f3_op_ar0* op );

/// f3_op_ar1(o) -> r
void bmath_hf3_s_f3_op_ar1( const bmath_hf3_s* o, const bmath_f3_op_ar1* op, bmath_hf3_s* r );

/// f3_op_ar2(a,b) -> r
void bmath_hf3_s_f3_op_ar2( const bmath_hf3_s* a, const bmath_hf3_s* b, const bmath_f3_op_ar2* op, bmath_hf3_s* r );

/**********************************************************************************************************************/
/** bmul: Specific holor * holor multiplication for holors up to order 2
 *  These operations are mapped to corresponding matrix (M), vector (V), scalar (S) operations depending on given holor order.
 *
 *  In case of vectors, the correct transposition-choice is important ...
 *    V^T * V   is the dot-product and served by htp_bmul
 *    V   * V^T is the outer-product and served by bmul_htp
 *    M   * V   is a valid vector transformation and served by bmul
 *    V^T * M   is a valid vector transformation and served by htp_bmul
 *
 *  Undefined compositions are ...
 *    V   * V,
 *    V^T * V^T
 *    V   * M
 *    M   * V^T
 */

void bmath_hf3_s_bmul(         const bmath_hf3_s* o, const bmath_hf3_s* b, bmath_hf3_s* r ); // o   * b   -> r
void bmath_hf3_s_bmul_htp(     const bmath_hf3_s* o, const bmath_hf3_s* b, bmath_hf3_s* r ); // o   * b^T -> r
void bmath_hf3_s_htp_bmul(     const bmath_hf3_s* o, const bmath_hf3_s* b, bmath_hf3_s* r ); // o^T * b   -> r
void bmath_hf3_s_htp_bmul_htp( const bmath_hf3_s* o, const bmath_hf3_s* b, bmath_hf3_s* r ); // o^T * b^T -> r

void bmath_hf3_s_bmul_add(         const bmath_hf3_s* o, const bmath_hf3_s* b, const bmath_hf3_s* c, bmath_hf3_s* r ); // o   * b   + c -> r
void bmath_hf3_s_bmul_htp_add(     const bmath_hf3_s* o, const bmath_hf3_s* b, const bmath_hf3_s* c, bmath_hf3_s* r ); // o   * b^T + c -> r
void bmath_hf3_s_htp_bmul_add(     const bmath_hf3_s* o, const bmath_hf3_s* b, const bmath_hf3_s* c, bmath_hf3_s* r ); // o^T * b   + c -> r
void bmath_hf3_s_htp_bmul_htp_add( const bmath_hf3_s* o, const bmath_hf3_s* b, const bmath_hf3_s* c, bmath_hf3_s* r ); // o^T * b^T + c -> r

/** sets d-data on r for bmul operation; returns true in case if success
 *  Does not change v_data
 */
bl_t bmath_hf3_s_set_d_bmul(         const bmath_hf3_s* o, const bmath_hf3_s* b, bmath_hf3_s* r ); // o   * b   -> r
bl_t bmath_hf3_s_set_d_bmul_htp(     const bmath_hf3_s* o, const bmath_hf3_s* b, bmath_hf3_s* r ); // o   * b^T -> r
bl_t bmath_hf3_s_set_d_htp_bmul(     const bmath_hf3_s* o, const bmath_hf3_s* b, bmath_hf3_s* r ); // o^T * b   -> r
bl_t bmath_hf3_s_set_d_htp_bmul_htp( const bmath_hf3_s* o, const bmath_hf3_s* b, bmath_hf3_s* r ); // o^T * b^T -> r

/** composite multiply-add function. Satisfies functionality of BLAS:DGEMM.
 *  op(a) * op(b) * c + d * e -> r
 *  op(x) = htpx ? x^T : x;
 *  c, e are scalar.
 *  d can be NULL
 */
void bmath_hf3_s_bmul_add_cps( bl_t htpa, const bmath_hf3_s* a, sz_t k, bl_t htpb, const bmath_hf3_s* b, f3_t c, const bmath_hf3_s* d, f3_t e, bmath_hf3_s* r );

/**********************************************************************************************************************/

vd_t bmath_hf3_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BMATH_HF3_H



