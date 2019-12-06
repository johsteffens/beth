/** Author and Copyright 2019 Johannes Bernhard Steffens
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

#ifndef BHVM_HF3_H
#define BHVM_HF3_H

/**********************************************************************************************************************/

/** Holor of f3_t.
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
 *  bhvm_hf3_s is organized as array of dimensions (d_data) and array of associated value data (v_data).
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
 *  Relationhip to Merates, Valence (Moon et al)
 *  These are terms describing the aspects of a holor's shape.
 *    Merates (as number) is the number of independent values. It is the same as v_size for
 *    a determined holor, or d_product otherwise.
 *    We call this value 'Volume'
 *
 *    Valence is the number of independent dimensions.
 *    We call this value 'order' or d_size.
 *
 *  Shape
 *    We define the shape of a holor by the entire array d_data. Two holors of the same shape
 *    have the same Valence and same number of Merates, but valence and number of merates alone
 *    are generally not sufficient to define shape.
 *
 */

/**********************************************************************************************************************/

#include "bcore_std.h"
#include "bmath_std.h"
#include "bhvm_planted.h"

/**********************************************************************************************************************/

#ifdef TYPEOF_bhvm_hf3

PLANT_GROUP( bhvm_hf3, bcore_inst )
#ifdef PLANT_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stamp : = aware bcore_inst
{
    sz_t [] d; // shape
    f3_t [] v; // values
    bl_t htp;  // holor is transposed
};

/// array of dynamic links
stamp :adl = aware bcore_array { :s => []; };

#endif // PLANT_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**********************************************************************************************************************/

/// Initializes Holor weakly; no shutdown necessary
static inline bhvm_hf3_s bhvm_hf3_init_weak( sz_t* d_data, sz_t d_size, f3_t* v_data, sz_t v_size, bl_t htp )
{
    bhvm_hf3_s o;
    bhvm_hf3_s_init( &o );
    o.d_data = d_data;
    o.d_size = d_size;
    o.v_data = v_data;
    o.v_size = v_size;
    o.htp = htp;
    return o;
}

/// Initializes Holor by forking references; normal shutdown required
static inline bhvm_hf3_s bhvm_hf3_init_fork( sz_t* d_data, sz_t d_size, sz_t d_space, f3_t* v_data, sz_t v_size, sz_t v_space, bl_t htp )
{
    bhvm_hf3_s o;
    bhvm_hf3_s_init( &o );
    o.d_data  = bcore_fork( d_data );
    o.d_size  = d_size;
    o.d_space = d_space;
    o.v_data  = bcore_fork( v_data );
    o.v_size  = v_size;
    o.v_space = v_space;
    o.htp = htp;
    return o;
}

/// turns a weak d-array into a strong one (no effect if it is strong)
void bhvm_hf3_s_d_make_strong( bhvm_hf3_s* o );

/// turns a weak v-array into a strong one (no effect if it is strong)
void bhvm_hf3_s_v_make_strong( bhvm_hf3_s* o );

/// make_strong on d-array and v-array (in case either is weak)
void bhvm_hf3_s_make_strong( bhvm_hf3_s* o );

/// sets number of values (initialized to zero)
void bhvm_hf3_s_set_v_size( bhvm_hf3_s* o, sz_t size );

/// sets number of dimensions (initialized to zero)
void bhvm_hf3_s_set_d_size( bhvm_hf3_s* o, sz_t size );

/// clears number of values
void bhvm_hf3_s_clear_v_data( bhvm_hf3_s* o );

/// clears number of dimensions (makes holor of scalar type)
void bhvm_hf3_s_clear_d_data( bhvm_hf3_s* o );

/// clears holor
void bhvm_hf3_s_clear( bhvm_hf3_s* o );

/// changes number of values preserving content (pads zero)
void bhvm_hf3_s_resize_v_size( bhvm_hf3_s* o, sz_t size );

/// changes number of dimensions preserving content (pads zero)
void bhvm_hf3_s_resize_d_size( bhvm_hf3_s* o, sz_t size );

/// sets number of values (initialized to zero)
void bhvm_hf3_s_set_v_size( bhvm_hf3_s* o, sz_t size );

/// fits number of values to given dimensions (initialized to zero)
void bhvm_hf3_s_fit_v_size( bhvm_hf3_s* o );

/// turns holor into vacant state without affecting d_data
void bhvm_hf3_s_set_vacant( bhvm_hf3_s* o );

/// sets d_data, d_size to array (does not change v_size or v_data)
void bhvm_hf3_s_set_d_data( bhvm_hf3_s* o, const sz_t* d_data, sz_t d_size );

/// sets v_data, v_size to array (does not change d_size or d_data)
void bhvm_hf3_s_set_v_data( bhvm_hf3_s* o, const f3_t* v_data, sz_t v_size );

/// feature copy_typed (use in inst perspective)
void bhvm_hf3_s_copy_typed( bhvm_hf3_s* o, tp_t type, vc_t src );

/// copies d_data from src (does not change v_size or v_data)
void bhvm_hf3_s_copy_d_data( bhvm_hf3_s* o, const bhvm_hf3_s* src );

/// copy_shape == copy d_data and htp flag
static inline void bhvm_hf3_s_copy_shape( bhvm_hf3_s* o, const bhvm_hf3_s* src ) { bhvm_hf3_s_copy_d_data( o, src ); o->htp = src->htp; }

/// copies v_data from src (does not change d_size or d_data)
void bhvm_hf3_s_copy_v_data( bhvm_hf3_s* o, const bhvm_hf3_s* src );

/// copies v_data from src (does not change d_size or d_data)
void bhvm_hf3_s_copy_v_data_from_vf3( bhvm_hf3_s* o, const bmath_vf3_s* src );

/// copies v_data from src (does not change d_size or d_data)
void bhvm_hf3_s_copy_v_data_to_vf3( const bhvm_hf3_s* o, bmath_vf3_s* dst );

/// sets size of entire holor including v_data; (v_data initialized to zero)
void bhvm_hf3_s_set_size( bhvm_hf3_s* o, const sz_t* d_data, sz_t d_size );

/// copies d data and v size (including vacant state) from src;
void bhvm_hf3_s_copy_size( bhvm_hf3_s* o, const bhvm_hf3_s* src );

/// sets d_data via variadic arguments
void bhvm_hf3_s_set_d_data_nv( bhvm_hf3_s* o, sz_t d_size, va_list sz_t_args );
void bhvm_hf3_s_set_d_data_na( bhvm_hf3_s* o, sz_t d_size, ... );

/// sets v_data via variadic arguments
void bhvm_hf3_s_set_v_data_nv( bhvm_hf3_s* o, sz_t v_size, va_list sz_t_args );
void bhvm_hf3_s_set_v_data_na( bhvm_hf3_s* o, sz_t v_size, ... );

/// sets size via variadic arguments
void bhvm_hf3_s_set_size_nv( bhvm_hf3_s* o, sz_t d_size, va_list sz_t_args );
void bhvm_hf3_s_set_size_na( bhvm_hf3_s* o, sz_t d_size, ... );

/**   product of all dimensions
  * = number of independent quantities
  * = v_size in case of for a determined holor
  */
sz_t bhvm_hf3_s_d_product( const bhvm_hf3_s* o );  // always calculates d-product from d_data
sz_t bhvm_hf3_s_volume(    const bhvm_hf3_s* o );  // returns v_size if > 0 otherwise d_product

/// sets shape to scalar (no change on v_data)
void bhvm_hf3_s_set_shape_scalar( bhvm_hf3_s* o );

/// sets holor to scalar with given value or to vacant scalar
void bhvm_hf3_s_set_scalar_pf3( bhvm_hf3_s* o, f3_t* v );

/// sets holor to scalar with given value
void bhvm_hf3_s_set_scalar_f3( bhvm_hf3_s* o, f3_t v );

/**********************************************************************************************************************/

/// parsing (generic syntax)
void bhvm_hf3_s_parse( bhvm_hf3_s* o, bcore_source* source );

/**********************************************************************************************************************/
/// status

bl_t bhvm_hf3_s_d_equal(       const bhvm_hf3_s* o, const bhvm_hf3_s* src ); // compares d_data
bl_t bhvm_hf3_s_v_equal(       const bhvm_hf3_s* o, const bhvm_hf3_s* src ); // compares v_data (independently of d_data)
bl_t bhvm_hf3_s_shape_equal(   const bhvm_hf3_s* o, const bhvm_hf3_s* src ); // compares d_data and htp
bl_t bhvm_hf3_s_is_equal(      const bhvm_hf3_s* o, const bhvm_hf3_s* src );
bl_t bhvm_hf3_s_is_scalar(     const bhvm_hf3_s* o );
bl_t bhvm_hf3_s_is_nan(        const bhvm_hf3_s* o ); // Holor is nan if at least one value-element is nan

/// Overall consistency; all valid states return true;
bl_t bhvm_hf3_s_is_consistent( const bhvm_hf3_s* o );

/// Overall consistency; Invalid state produces an error.
void bhvm_hf3_s_check_integrity( const bhvm_hf3_s* o );

static inline bl_t bhvm_hf3_s_d_is_weak( const bhvm_hf3_s* o ) { return ( o->d_space == 0 ) && ( o->d_size > 0 ); }
static inline bl_t bhvm_hf3_s_v_is_weak( const bhvm_hf3_s* o ) { return ( o->v_space == 0 ) && ( o->v_size > 0 ); }
static inline bl_t bhvm_hf3_s_is_weak( const bhvm_hf3_s* o ) { return bhvm_hf3_s_d_is_weak( o ) || bhvm_hf3_s_v_is_weak( o ); }

/**********************************************************************************************************************/
/// weak conversion

// ---------------------------------------------------------------------------------------------------------------------

/// holor -> matrix
static inline bmath_mf3_s bhvm_hf3_s_get_weak_mat( const bhvm_hf3_s* o )
{
    assert( o->d_size == 2 );
    sz_t cols = o->d_data[ 0 ];
    sz_t rows = o->d_data[ 1 ];
    assert( o->v_size == cols * rows );
    return bmath_mf3_init_weak( rows, cols, cols, o->v_data );
}

// ---------------------------------------------------------------------------------------------------------------------

/// holor -> vector
static inline bmath_vf3_s bhvm_hf3_s_get_weak_vec( const bhvm_hf3_s* o )
{
    assert( o->d_size == 1 );
    sz_t size = o->d_data[ 0 ];
    assert( o->v_size == size );
    return bmath_vf3_init_weak( o->v_data, size );
}

// ---------------------------------------------------------------------------------------------------------------------

/// holor -> transposed holor
static inline bhvm_hf3_s bhvm_hf3_s_get_weak_htp( const bhvm_hf3_s* o )
{
    return bhvm_hf3_init_weak( o->d_data, o->d_size, o->v_data, o->v_size, !o->htp );
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// casts  (make the target weakly reference source data)

/// cast to transposed holor
static inline void bhvm_hf3_s_cast_htp( const bhvm_hf3_s* o, bhvm_hf3_s* r )
{
    bhvm_hf3_s_clear( r );
    r->d_data = o->d_data;
    r->d_size = o->d_size;
    r->v_data = o->v_data;
    r->v_size = o->v_size;
    r->htp    = !o->htp;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// holor specific operations

static inline sz_t bhvm_hf3_s_get_order( const bhvm_hf3_s* o ) { return o->d_size; }

/** Canonic increment of order by appending one dimension 'dim' :
 *  If the holder holds data, the data is duplicated 'dim' times.
 */
void bhvm_hf3_s_inc_order( bhvm_hf3_s* o, sz_t dim );

/** Canonic increment of order by prepending one dimension 'dim' :
 *  If the holder holds data, the data is duplicated on element-level 'dim' times.
 */
void bhvm_hf3_s_inc_order_prepend( bhvm_hf3_s* o, sz_t dim );

/** Canonic data append of a sub-holor
 *  Requirements (checked):
 *  * o and src must either be both vacant or both determined.
 *  * o->d_size == src->d_size || o->d_size == src->d_size + 1
 *  * src->d_data must match (lower significant) o->d_data
 */
void bhvm_hf3_s_push( bhvm_hf3_s* o, const bhvm_hf3_s* src );

/** Canonic catenation of two holors.
 *  r->v_data is catenated from a->v_data and b->v_data
 *  Shape of r:
 *  If a, b have equal shape then r has shape [2]a
 *  If a[0] has shape of b, then r[0] has shape b and dimof(r) == dimof(a) + 1
 *  If b[0] has shape of a, then r[0] has shape a and dimof(r) == dimof(b) + 1
 *  Other constellations are not allowed.
 */

/// sets d-data for catenation; does not change v-data; returns false if not possible (a,b have incorrect shape)
bl_t bhvm_hf3_s_set_shape_cat( const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r );

/// performs catenation of data; only checks merates
void bhvm_hf3_s_cat( const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r );

void bhvm_hf3_s_to_sink(      const bhvm_hf3_s* o, bcore_sink* sink );
void bhvm_hf3_s_to_sink_nl(   const bhvm_hf3_s* o, bcore_sink* sink ); // appends newline
void bhvm_hf3_s_to_stdout(    const bhvm_hf3_s* o );
void bhvm_hf3_s_to_stdout_nl( const bhvm_hf3_s* o ); // appends newline

/** compacted version, single line */
void bhvm_hf3_s_brief_to_sink(       const bhvm_hf3_s* o, bcore_sink* sink );
void bhvm_hf3_s_brief_to_stdout(     const bhvm_hf3_s* o );

/** multiline version */
void bhvm_hf3_s_formatted_to_sink(   const bhvm_hf3_s* o, bcore_sink* sink );
void bhvm_hf3_s_formatted_to_stdout( const bhvm_hf3_s* o );

/** Sets all vector elements to random values.
 *  Random generator:
 *    Parameters density, min, max, p_rval apply to the random generator.
 *      rval: Pointer to running variable of random generator.
 *            If NULL, an internal fixed random seed is used.
 *
 *     density (range [0.0, 1.0]) specifies the rate at which the random generator
 *     creates a non-zero value.
 */
void bhvm_hf3_s_set_random( bhvm_hf3_s* o, f3_t density, f3_t min, f3_t max, u2_t* p_rval );

//----------------------------------------------------------------------------------------------------------------------
/** Holor-Generalized Frobenius Norm
 *  See bmath_mf3_fdev... for more details
 */
f3_t bhvm_hf3_s_fdev_equ( const bhvm_hf3_s* o, const bhvm_hf3_s* op );
f3_t bhvm_hf3_s_fdev_zro( const bhvm_hf3_s* o );

/**********************************************************************************************************************/
/// elementwise operations

/// direct implementations

/// o = {0}
static inline void bhvm_hf3_s_zro( const bhvm_hf3_s* o ) { for( sz_t i = 0; i < o->v_size; i++ ) o->v_data[ i ] = 0; }

void bhvm_hf3_adl_s_zro( const bhvm_hf3_adl_s* o );

/// o = {v}
static inline void bhvm_hf3_s_set_f3( const bhvm_hf3_s* o, f3_t v ) { for( sz_t i = 0; i < o->v_size; i++ ) o->v_data[ i ] = v; }

/// o -> r
void bhvm_hf3_s_cpy(     const bhvm_hf3_s*     o, bhvm_hf3_s*     r );
void bhvm_hf3_adl_s_cpy( const bhvm_hf3_adl_s* o, bhvm_hf3_adl_s* r );

/// exp(o) -> r
void bhvm_hf3_s_exp( const bhvm_hf3_s* o, bhvm_hf3_s* r );

/// tanh(o) -> r
void bhvm_hf3_s_tanh( const bhvm_hf3_s* o, bhvm_hf3_s* r );

/// o^t -> r
void bhvm_hf3_s_htp( const bhvm_hf3_s* o, bhvm_hf3_s* r );

/// o + m -> r
void bhvm_hf3_s_add(     const bhvm_hf3_s*     o, const bhvm_hf3_s*     m, bhvm_hf3_s*     r );
void bhvm_hf3_adl_s_add( const bhvm_hf3_adl_s* o, const bhvm_hf3_adl_s* m, bhvm_hf3_adl_s* r );

/// o - m -> r
void bhvm_hf3_s_sub(     const bhvm_hf3_s*     o, const bhvm_hf3_s*     m, bhvm_hf3_s*     r );
void bhvm_hf3_adl_s_sub( const bhvm_hf3_adl_s* o, const bhvm_hf3_adl_s* m, bhvm_hf3_adl_s* r );

/// (o - m)^2 -> r
void bhvm_hf3_s_sub_sqr(    const bhvm_hf3_s* o, const bhvm_hf3_s* m, f3_t* r );
f3_t bhvm_hf3_s_f3_sub_sqr( const bhvm_hf3_s* o, const bhvm_hf3_s* m );

void bhvm_hf3_adl_s_sub_sqr(    const bhvm_hf3_adl_s* o, const bhvm_hf3_adl_s* m, f3_t* r );
f3_t bhvm_hf3_adl_s_f3_sub_sqr( const bhvm_hf3_adl_s* o, const bhvm_hf3_adl_s* m );

/// o * m [ + b] -> r (hadamard product)
void bhvm_hf3_s_hmul(     const bhvm_hf3_s* o, const bhvm_hf3_s* m, bhvm_hf3_s* r );
void bhvm_hf3_s_hmul_add( const bhvm_hf3_s* o, const bhvm_hf3_s* m, const bhvm_hf3_s* b, bhvm_hf3_s* r );

void bhvm_hf3_adl_s_hmul( const bhvm_hf3_adl_s* o, const bhvm_hf3_adl_s* m, bhvm_hf3_adl_s* r );

/// o * b [ + c] -> r (b scalar)
void bhvm_hf3_s_mul_scl(     const bhvm_hf3_s* o, const f3_t* b,                       bhvm_hf3_s* r ); // o * b     -> r
void bhvm_hf3_s_mul_scl_add( const bhvm_hf3_s* o, const f3_t* b, const bhvm_hf3_s* c, bhvm_hf3_s* r ); // o * b + c -> r

static inline void bhvm_hf3_s_mul_scl_f3(     const bhvm_hf3_s* o, f3_t b,                       bhvm_hf3_s* r ) { bhvm_hf3_s_mul_scl(     o, &b,    r ); }
static inline void bhvm_hf3_s_mul_scl_f3_add( const bhvm_hf3_s* o, f3_t b, const bhvm_hf3_s* c, bhvm_hf3_s* r ) { bhvm_hf3_s_mul_scl_add( o, &b, c, r ); }

void bhvm_hf3_adl_s_mul_scl(    const bhvm_hf3_adl_s* o, const f3_t* b, bhvm_hf3_adl_s* r );
void bhvm_hf3_adl_s_mul_scl_f3( const bhvm_hf3_adl_s* o,       f3_t  b, bhvm_hf3_adl_s* r );

/// cast o, b as vector and performs a dot product
f3_t bhvm_hf3_s_f3_vec_mul_vec( const bhvm_hf3_s* o, const bhvm_hf3_s* b ); // dot product
void bhvm_hf3_s_vec_mul_vec(     const bhvm_hf3_s* o, const bhvm_hf3_s* b, f3_t* r ); // dot product

f3_t bhvm_hf3_s_f3_max( const bhvm_hf3_s* o );
f3_t bhvm_hf3_s_f3_min( const bhvm_hf3_s* o );
f3_t bhvm_hf3_s_f3_sum( const bhvm_hf3_s* o );

/// operators
void bhvm_hf3_s_fp_f3_ar0(       bhvm_hf3_s* o,                      bmath_fp_f3_ar0 fp );
void bhvm_hf3_s_fp_f3_ar1( const bhvm_hf3_s* a,                      bmath_fp_f3_ar1 fp, bhvm_hf3_s* r );
void bhvm_hf3_s_fp_f3_ar2( const bhvm_hf3_s* a, const bhvm_hf3_s* b, bmath_fp_f3_ar2 fp, bhvm_hf3_s* r );

void bhvm_hf3_s_fp_f3_op_ar0(       bhvm_hf3_s* o,                      bmath_fp_f3_op_ar0 fp, vc_t op );
void bhvm_hf3_s_fp_f3_op_ar1( const bhvm_hf3_s* a,                      bmath_fp_f3_op_ar1 fp, vc_t op, bhvm_hf3_s* r );
void bhvm_hf3_s_fp_f3_op_ar2( const bhvm_hf3_s* a, const bhvm_hf3_s* b, bmath_fp_f3_op_ar2 fp, vc_t op, bhvm_hf3_s* r );

/// ... madd: applies operator, elementwise multiplies 'm' and adds result to 'r'
void bhvm_hf3_s_fp_f3_ar0_madd(                                           bmath_fp_f3_ar0 fp, const bhvm_hf3_s* m, bhvm_hf3_s* r );
void bhvm_hf3_s_fp_f3_ar1_madd( const bhvm_hf3_s* a,                      bmath_fp_f3_ar1 fp, const bhvm_hf3_s* m, bhvm_hf3_s* r );
void bhvm_hf3_s_fp_f3_ar2_madd( const bhvm_hf3_s* a, const bhvm_hf3_s* b, bmath_fp_f3_ar2 fp, const bhvm_hf3_s* m, bhvm_hf3_s* r );

/**********************************************************************************************************************/
/** bmul: Specific holor * holor multiplication for holors up to order 2
 *  These operations are mapped to corresponding matrix (M), vector (V), scalar (S) operations
 *  depending on given holor-order and transposition state.
 *  Note: bmul only allows operand combinations that have canonical dendrite-passes.
 *        For that reason scalar multiplication of matrices is excluded in this context.
 *
 *  A dendrite pass can be expressed as bmul of different transposition:
 *  A  ** B  = C: GA  = GC ** Bt and GB  = At ** GC
 *  A  ** Bt = C: GA  = GC ** B  and GBt = At ** GC
 *  At ** B  = C: GAt = GC ** Bt and GB  = A  ** GC
 *  At ** Bt = C: GAt = GC ** B  and GBt = A  ** GC
 *
 *  In case of vectors, the correct transposition-choice is important ...
 *  Vt ** V  -> S  is the vector-dot-product
 *  V  ** Vt -> M  is the vector-outer-product
 *  M  ** V  -> V  is a valid vector transformation
 *  Vt ** M  -> Vt is a valid vector transformation
 *  V  ** S  -> V
 *  S  ** vt -> Vt
 *
 *  Undefined compositions are ...
 *  S  ** V
 *  Vt ** S
 *  V  ** V
 *  Vt ** Vt
 *  V  ** M
 *  M  ** Vt
 *  M(t) ** S(t) (scalar multiplication is handled in separate function)
 *  S(t) ** M(t) (scalar multiplication is handled in separate function)
 *
 *  hs_code, bhs_code, ths_code:
 *  Configuration encoding used in bmul functions.
 */

void bhvm_hf3_s_bmul(     const bhvm_hf3_s* o, const bhvm_hf3_s* b,                      bhvm_hf3_s* r ); // o * b     -> r
void bhvm_hf3_s_bmul_add( const bhvm_hf3_s* o, const bhvm_hf3_s* b, const bhvm_hf3_s* c, bhvm_hf3_s* r ); // o * b + c -> r

/** sets shape on r for bmul operation; returns true in case if success
 *  Does not change v_data
 */
bl_t bhvm_hf3_s_set_shape_bmul( const bhvm_hf3_s* o, const bhvm_hf3_s* b, bhvm_hf3_s* r ); // o * b -> r

/** Composite multiply-add function.
 *  a * b * c + d * e -> r
 *  c, e are scalar.
 *  d can be NULL
 */
void bhvm_hf3_s_bmul_add_cps( const bhvm_hf3_s* a, const bhvm_hf3_s* b, f3_t c, const bhvm_hf3_s* d, f3_t e, bhvm_hf3_s* r );

/// hs_code retrieval
s2_t bhvm_hf3_s_hs_code(  const bhvm_hf3_s* o                                           );
s2_t bhvm_hf3_s_bhs_code( const bhvm_hf3_s* o, const bhvm_hf3_s* b                      );
s2_t bhvm_hf3_s_ths_code( const bhvm_hf3_s* o, const bhvm_hf3_s* b, const bhvm_hf3_s* r );

/// hs_code to descriptive string
void bhvm_hf3_st_s_push_hs_code(  st_s* o, s2_t  hs_code );
void bhvm_hf3_st_s_push_bhs_code( st_s* o, s2_t bhs_code );
void bhvm_hf3_st_s_push_ths_code( st_s* o, s2_t ths_code );

/// checks if bmul is defined for given bhs_code
bl_t bhvm_hf3_s_bmul_bhs_code_allowed( s2_t bhs_code );

#endif // TYPEOF_bhvm_hf3

/**********************************************************************************************************************/

vd_t bhvm_hf3_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BHVM_HF3_H



