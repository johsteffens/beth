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

#ifndef BHVM_HOLOR_H
#define BHVM_HOLOR_H

/**********************************************************************************************************************/

/** Holor of flexible value type (f3_t or f2_t).
 *
 *  A Holor represents the canonic extension of a matrix. (It can also be seen as generalization of a tensor).
 *  The name was coined by Moon & Spencer (P.Moon, D.E.Spencer: "Theory of Holors", 1986. ISBN 0-521-01900-1 )
 *  We use our own nomenclature for holor properties, which deviates from M&S's choice of terms.
 *
 *  The term holor in this scope is represented as a multidimensional array, where jagged arrays are explicitly excluded.
 *  Meaning that a (n+1)-holor is an array of n-holors of the same data-layout but independent values.
 *
 *  The number of dimensions is called the order of a holor.
 *
 *  A n-holor is understood as holor or order n (meaning n separate dimensions).
 *  The number of values a holor holds is equal to the product of its dimensions.
 *
 *  bhvm_holor_s is organized as array of dimensions (managed in the 'shape' object) and array of associated
 *  value data (managed in the 'value' object).
 *  Dimension-values are arranged from lowest to highest level in the hsape-object.
 *  The product of all shape-values is called 'volume'. Note that the empty 'shape' represents volume 1.
 *
 *  A given holor is called 'determined' when 'value' is defined according to its order and dimensionality.
 *  'value' being empty is a valid holor state which is called 'vacant'.
 *
 *  Examples:
 *    A 0-holor represents a scalar.
 *    A 1-holor represents a vector; vectors-size == s.data[ 0 ];
 *    A 2-holor represents a matrix: cols == s.data[ 0 ]; rows == s.data[ 1 ];
 *    A n-holor represents an array of k (n-1)-holors with k == s.data[ n - 1 ].
 *
 */

/**********************************************************************************************************************/

#include "bcore_std.h"
#include "bmath_std.h"
#include "bhvm.xo.h"
#include "bhvm_lop.h"

/**********************************************************************************************************************/
// constants

/** We define an upper limit for a holor dimension in order to simplify detecting invalid shapes
 *  This limit is not relevant for general functionality and can be increased or removed
 *  once the code has been well established.
 */
#define BHVM_SHAPE_DIM_VALID( dim ) ( ( dim > 0 ) && ( ( ((u3_t)dim) >> 40 ) == 0 ) )

/**********************************************************************************************************************/

#ifdef TYPEOF_bhvm

XOILA_DEFINE_GROUP( bhvm, bcore_inst )
#ifdef XOILA_SECTION

/**********************************************************************************************************************/

stamp :shape_s = x_array
{
    sz_t [];
    func x_array.clear    = { return ( @*    )o.cast( x_array* ).t_clear( TYPEOF_@ ); };
    func x_array.set_size = { return ( @*    )o.cast( x_array* ).t_set_size( TYPEOF_@, size ); };
    func x_array.push     = { return ( sz_t* )o.cast( x_array* ).t_push( TYPEOF_@ ); };


    /// weak reference; no shutdown required
    func (void init_weak( mutable, sz_t* data, sz_t size )) =
    {
        o.init();
        o.data = data;
        o.size = size;
    };

    /// weak reference; no shutdown required
    func (void init_weak_from_shape( mutable, @* src )) = { o.init_weak( src.data, src.size ); };

    /// forked reference; shutdown required
    func (void init_fork( mutable, sz_t* data, sz_t size, sz_t space )) =
    {
        o.init_weak( data.fork(), size );
        o.space = space;
    };

    /// forked reference if src is strong, otherwise weak reference
    func (void fork_from( mutable, @* src )) =
    {
        o.clear();
        assert( o.space == 0 );
        o.data  = ( src->space > 0 ) ? src.data.fork() : src.data;
        o.size  = src->size;
        o.space = src->space;
    };

    /// weak reference;
    func (void weak( mutable, @* src )) =
    {
        o.clear();
        assert( o.space == 0 );
        o.data = src.data;
        o.size = src.size;
    };

    func (void make_strong( mutable )) = { bcore_array_t_make_strong( TYPEOF_@, ( bcore_array* )o ); };
    func (sz_t get_volume( const )) = { sz_t v = 1; foreach( sz_t e in o ) v *= e; return v; };

    /// sets data by copying
    func (void set_data(    mutable, const sz_t* data, sz_t size ));
    func (void set_data_nv( mutable, sz_t size, va_list sz_t_args ));
    func (void set_data_na( mutable, sz_t size, ... ));

    /// sets shape to scalar
    func (void set_scalar( mutable ));

    /// sets shape to vector ( dim [ #)
    func (void set_vector( mutable, sz_t dim ));

    /// status
    func (bl_t is_weak( const )) = { return ( o.space == 0 ) && ( o.size > 0 ); };
    func (bl_t is_equal( const, const @* b ));

    // b is a sub-shape of o
    func (bl_t is_sub( const, const @* b ));

    /// constructive catenation
    func (bl_t cat_can(  const, const @* b ));
    func (bl_t cat_fits( const, const @* b, const @* r ));
    func (bl_t is_cat(   const, const @* b, const @* r ));
    func (void cat(      const, const @* b,       @* r ));
    func (void cat_set(  const, const @* b,       @* r ));

    /// conservative catenation
    func (bl_t ccat_can(  const, const @* b ));
    func (bl_t ccat_fits( const, const @* b, const @* r ));
    func (bl_t is_ccat(   const, const @* b, const @* r ));
    func (void ccat(      const, const @* b,       @* r ));
    func (void ccat_set(  const, const @* b,       @* r ));

    /// isovolumetric conversion to a vector (order 1); o == src allowed
    func (@* copy_vector_isovol( mutable, const @* src ));
};

/**********************************************************************************************************************/

stamp :value_s = x_array
{
    typed [];
    func x_array.clear    = { return ( @* )o.cast( x_array* ).t_clear( TYPEOF_@ ); };
    func x_array.set_size = { return ( @* )o.cast( x_array* ).t_set_size( TYPEOF_@, size ); };

    /// weak reference; no shutdown required
    func (void init_weak( mutable, tp_t type, vd_t data, sz_t size )) =
    {
        o.init();
        o.type = type;
        o.data = data;
        o.size = size;
    };

    /// weak reference; no shutdown required
    func (void init_weak_from_value( mutable, @* src )) = { o.init_weak( src.type, src.data, src.size ); };

    /// forked reference if src is strong, otherwise weak reference
    func (void fork_from( mutable, @* src )) =
    {
        o.clear();
        assert( o.space == 0 );
        o.type  = src.type;
        o.data  = ( src.space > 0 ) ? src.data.fork() : src.data;
        o.size  = src.size;
        o.space = src.space;
    };

    /// weak reference;
    func (void weak( mutable, @* src )) =
    {
        o.clear();
        assert( o.space == 0 );
        o.type = src.type;
        o.data = src.data;
        o.size = src.size;
    };

    func (void make_strong( mutable )) = { bcore_array_t_make_strong( TYPEOF_@, ( bcore_array* )o ); };


    /// sets type; preexisting type (if any) is converted
    func (void set_type( mutable, tp_t type ));

    /// sets type and size
    func (void set_type_size( mutable, tp_t type, sz_t size ));

    /// sets type and data by copying/converting
    func (void set_type_data( mutable, tp_t dst_type, tp_t src_type, vc_t src_data, sz_t size ));

    /// copies data in place; converts type; no allocation; o->type must exits
    func (void cpy_data( mutable, tp_t src_type, vc_t src_data, sz_t size ));
    func (void cpy( const, @* dst )) = { dst.cpy_data( o.type, o.data, o.size ); };

    /// sets all values zero
    func (void zro( mutable ));

    /// sets data by converting to holor type (sets target type if not set)
    func (void set_data( mutable, tp_t src_type, vc_t src_data, sz_t size ));

    /// forks data (type = src_type)
    func (void fork_from_data( mutable, tp_t src_type, vd_t src_data, sz_t size ));

    /// weakly references data (type = src_type)
    func (void weak_data( mutable, tp_t src_type, vd_t src_data, sz_t size ));

    /// pushes data by converting to o->type
    func (void push_data( mutable, tp_t src_type, vc_t src_data, sz_t size ));

    /// pushes data of value converting to o->type
    func (void push_value( mutable, const @* src ));

    /// status
    func (bl_t is_weak(   const )) = { return ( o.space == 0 ) && ( o.size > 0 ); };
    func (bl_t is_vacant( const )) = { return ( o.size == 0 ); };
    func (bl_t is_nan( const ));
    func (bl_t is_equal( const, const @* b ));

    /// constructive (==conservative) concatenation
    func (bl_t cat_can ( const, const @* b ));
    func (bl_t cat_fits( const, const @* b, const @* r ));
    func (void cat     ( const, const @* b,       @* r ));
    func (void cat_set ( const, const @* b,       @* r ));

    /// value -> vector
    func (bmath_vf2_s get_weak_vf2( const )) =
    {
        ASSERT( o.type == TYPEOF_f2_t );
        return bmath_vf2_init_weak( ( f2_t* )o.data, o.size );
    };

    func (bmath_vf3_s get_weak_vf3( const )) =
    {
        ASSERT( o.type == TYPEOF_f3_t );
        return bmath_vf3_init_weak( ( f3_t* )o.data, o.size );
    };


    /** Sets all vector elements to random values.
     *  Random generator:
     *    Parameters density, min, max, p_rval apply to the random generator.
     *      rval: Pointer to running variable of random generator.
     *            If NULL, an internal fixed random seed is used.
     *
     *     density (range [0.0, 1.0]) specifies the rate at which the random generator
     *     creates a non-zero value.
     */
    func (void set_random(    mutable, f3_t density, f3_t min, f3_t max, bcore_prsg* prsg ));
    func (void set_random_u3( mutable, f3_t density, f3_t min, f3_t max, u3_t* p_rval ));

    /** Frobenius Norm
     *  See bmath_mf3_fdev... for more details
     */
    func (f3_t fdev_equ( const, const @* b ));
    func (f3_t fdev_zro( const ));

    func (sz_t get_sz( const, sz_t index ));
    func (f3_t get_f3( const, sz_t index ));
    func (void set_f3( const, sz_t index, f3_t v ));

    func (f3_t get_max_f3( const ));
    func (f3_t get_min_f3( const ));

    /// order increment by duplication (in place)
    func (void order_inc(     const, sz_t dim, @* r )); // in place
    func (void order_inc_set( const, sz_t dim, @* r )); // allocating, order_inc

    /// order decrement by indexing (in place); dim is leading dimension of o
    func (void order_dec(     const, sz_t dim, sz_t idx, @* r )); // in place
    func (void order_dec_set( const, sz_t dim, sz_t idx, @* r )); // allocating, order_dec

    /// accumulates values in stats; returns stats
    func (bhvm_stats_s* stats_acc( const, bhvm_stats_s* stats ));

    /******************************************************************************************************************/
    /// general mathematics

    /// o[ i ] += a[ i ]
    func (void acc( mutable, const @* a ));

    /// returns sum{ ( o[i] ) }
    func (f3_t sum( const ));

    /// returns sum{ ( o[i]^2 ) } (dot product)
    func (f3_t sum_sqr( const ));

    /// returns sum{ ( a[i] - b[i] )^2 }
    func (f3_t sub_sqr_sum( const, const @* b ));

    /// r[ i ] = a[ i ] * scl
    func (void mul_scl_f3( const, f3_t scl, @* r ));

    /// r[ i ] += a[ i ] * scl
    func (void mul_scl_f3_acc( const, f3_t scl, @* r ));

    /// r[ i + r_offs ] += a[ i + a_offs ]
    func (void acc_offs( const, sz_t a_offs, @* r, sz_t r_offs, sz_t size ));

};

/**********************************************************************************************************************/

stamp :holor_s = aware bcore_inst
{
    :shape_s s;
    :value_s v;
    func bcore_fp . copy_typed;

    /// weak reference; no shutdown required
    func (void init_weak( mutable, sz_t* s_data, sz_t s_size, tp_t v_type, vd_t v_data, sz_t v_size )) =
    {
        o.init();
        o.s.init_weak( s_data, s_size );
        o.v.init_weak( v_type, v_data, v_size );
    };

    /// weak reference; no shutdown required
    func (void init_weak_from_holor( mutable, @* src )) =
    {
        o.init();
        o.s.init_weak_from_shape( src.s );
        o.v.init_weak_from_value( src.v );
    };

    /// forked reference; (shutdown required)
    func (void fork_from( mutable, @* src )) =
    {
        o.s.fork_from( src.s );
        o.v.fork_from( src.v );
    };

    /// fork or copy according to flags
    func (void fork_or_copy( mutable, @* src, bl_t fork_shape, bl_t fork_value )) =
    {
        if( fork_shape ) o.s.fork_from( src.s ); else o.s.copy( src.s );
        if( fork_value ) o.v.fork_from( src.v ); else o.v.copy( src.v );
    };

    /// copies value and converts shape to vector
    func (@* copy_vector_isovol( mutable, const @* src )) =
    {
        o.s.copy_vector_isovol( src.s );
        o.v.copy( src.v );
        return o;
    };

    /// forks value and converts shape to vector
    func (@* fork_from_vector_isovol( mutable, @* src )) =
    {
        o.s.copy_vector_isovol( src.s );
        o.v.fork_from( src.v );
        return o;
    };

    /// clears entire holor
    func (@* clear( mutable )) =
    {
        o.s.clear();
        o.v.clear();
        return o;
    };

    func (@* copy_t( mutable, tp_t type, vc_t src ));

    /// Copies shape; clears value
    func (@* copy_shape( mutable, const bhvm_shape_s* src ));

    /// Copies shape; clears value; copies type
    func (@* copy_shape_type( mutable, const @* src ));

    func (@* set_type( mutable, tp_t type ));
    func (@* fit_size( mutable ));
    func (@* fit_type_size( mutable, tp_t t ));

    /// sets holor to scalar with given value or to vacant scalar
    func (void set_type_scalar_pf( mutable, tp_t t, tp_t t_src, vc_t v ));
    func (void set_scalar_pf( mutable, tp_t t_src, vc_t v ));
    func (void set_scalar_f3( mutable, f3_t v )) = { o.set_scalar_pf( TYPEOF_f3_t, &v ); };

    func (f3_t f3_get_scalar( const )) =
    {
        assert( o->v.size == 1 );
        return ( o->v.type == TYPEOF_f3_t ) ? ( ( f3_t* )o->v.data )[ 0 ] : o->v.type == TYPEOF_f2_t ? ( ( f2_t* )o->v.data )[ 0 ] : 0;
    };

    /// sets holor to scalar from f3 value
    func (void set_type_scalar( mutable, tp_t t, f3_t v )) = { o.set_type_scalar_pf( t, TYPEOF_f3_t, &v ); };

    /// sets holor to vacant scalar or vector
    func (@* set_type_scalar_vacant( mutable, tp_t type ));
    func (@* set_type_vector_vacant( mutable, tp_t type, sz_t dim ));

    /// Overall consistency; all valid states return true;
    func (bl_t is_consistent( const ));

    /// Overall consistency; Invalid state produces an error.
    func (void check_integrity( const ));

    /// sets holor from text source
    func (@* parse(    mutable, bcore_source* source ));
    func (@* parse_st( mutable, const st_s* st ));
    func (@* parse_sc( mutable, sc_t sc ));

    func (@* create_parse(    bcore_source* source ));
    func (@* create_parse_st( const st_s* st ));
    func (@* create_parse_sc( sc_t sc ));

    func (bl_t is_equal( const, const @* src )) =
    {
        if( !o.s.is_equal( src.s ) ) return false;
        if( !o.v.is_equal( src.v ) ) return false;
        return true;
    };

    /******************************************************************************************************************/
    /// weak conversion

    /// holor -> matrix
    func (bmath_mf2_s get_weak_mf2( const )) =
    {
        assert( o.v.type == TYPEOF_f2_t );
        assert( o.s.size == 2 );
        sz_t cols = o.s.[ 0 ];
        sz_t rows = o.s.[ 1 ];
        assert( o.v.size == cols * rows );
        return bmath_mf2_init_weak( rows, cols, cols, ( f2_t* )o.v.data );
    };

    /// holor -> matrix
    func (bmath_mf3_s get_weak_mf3( const )) =
    {
        assert( o.v.type == TYPEOF_f3_t );
        assert( o.s.size == 2 );
        sz_t cols = o.s.[ 0 ];
        sz_t rows = o.s.[ 1 ];
        assert( o.v.size == cols * rows );
        return bmath_mf3_init_weak( rows, cols, cols, ( f3_t* )o.v.data );
    };

    func (vd_t mfx_create_weak( const )) =
    {
        assert( o.s.size == 2 );
        sz_t cols = o.s.[ 0 ];
        sz_t rows = o.s.[ 1 ];
        assert( o.v.size == cols * rows );
        switch( o.v.type )
        {
            case TYPEOF_f2_t: return bmath_mf2_s_create_weak( rows, cols, cols, ( f2_t* )o.v.data );
            case TYPEOF_f3_t: return bmath_mf3_s_create_weak( rows, cols, cols, ( f3_t* )o.v.data );
            default: ERR_fa( "Invalid type" );
        }
        return NULL;
    };

    /**********************************************************************************************************************/
    /// general mathematics

    /// sets all values zero
    func (void zro(     mutable ));
    func (void zro_set( mutable )); // allocates o->v if necessary

    /// returns sum{ ( o[i] ) }
    func (f3_t sum( const ));

    /// returns sum{ ( o[i]^2 ) } (dot product)
    func (f3_t sum_sqr( const ));

    /// returns sum{ ( a[i] - b[i] )^2 }
    func (f3_t sub_sqr_sum( const, const @* b ));

    // o[i] += a[i];
    func (void acc(     mutable, const @* a ));
    func (void acc_set( mutable, const @* a )); // allocates value and initializes zero if necessary

    /**********************************************************************************************************************/
    /// holor specific operations

    func (sz_t get_order( const )) = { return o->s.size; };

    /** Canonic increment of order by appending one dimension 'dim' :
     *  If the holder holds data, the data is duplicated 'dim' times.
     */
    func (void inc_order( mutable, sz_t dim ));

    /** Canonic increment of order by prepending one dimension 'dim' :
     *  If the holder holds data, the data is duplicated on element-level 'dim' times.
     */
    func (void inc_order_prepend( mutable, sz_t dim ));

    /** Canonic data append of a sub-holor
     *  Requirements (checked):
     *  * o and src must either be both vacant or both determined.
     *  * o->d_size == src->d_size || o->d_size == src->d_size + 1
     *  * src->d_data must match (lower significant) o->d_data
     */
    func (void push( mutable, const @* src ));

    /** Constructive catenation of two holors.
     *  valua data is catenated from a b
     *
     *  Shape of r:
     *  If a, b have equal shape then r has shape 2[a
     *  If a]0 has shape of b, then r]0 has shape b and dimof(r) == dimof(a) + 1
     *  If b]0 has shape of a, then r]0 has shape a and dimof(r) == dimof(b) + 1
     *  Other shape constellations are not allowed.
     *
     *  Type and Vacancy:
     *  r inherits the common or more precise type of a,b
     *  r is vacant iff at least one of a,b is vacant.
     */
    func (bl_t cat_can( const, const @* b                  )); // test
    func (void cat_set( const, const @* b, @* r )); // cat with allocating r
    func (void cat    ( const, const @* b, @* r )); // in place

    /** Conservative catenation of two holors.
     *  valua data is catenated from a b
     *  Requirements:
     *    order(a) == order(b)
     *    order(a) >= 1
     *    a]0, b]0 have equal shape
     *
     *  Shape of r:
     *  r]0 = a]0
     *  dimof( r ) == dimof( a ) + dimof( b )
     *
     *  Type and Vacancy:
     *  r inherits the common or more precise type of a,b
     *  r is vacant iff at least one of a,b is vacant.
     */
    func (bl_t ccat_can( const, const @* b                  )); // test
    func (void ccat_set( const, const @* b, @* r )); // cat with allocating r
    func (void ccat    ( const, const @* b, @* r )); // in place

    /** Order increment
     *  A new leading dimension is appended to shape.
     *  holor value is duplicated dim-times
     */
    func (void order_inc_set(  const, sz_t dim, @* r )); // order increment allocating r
    func (void order_inc    (  const, sz_t dim, @* r )); // order increment in place

    /** Order decrement
     *  Leading dimension is removed from shape.
     *  indexed sub-holor is copied or referenced
     */
    func (void order_dec_set(  const, sz_t idx, @* r )); // order decrement by indexing into leading dimension allocating r
    func (void order_dec    (  const, sz_t idx, @* r )); // order decrement by indexing into leading dimension in place
    func (void order_dec_weak( mutable, sz_t idx, @* r )); // order decrement by indexing into leading dimension r is weakly referencing a

    func (void to_sink(      const, bcore_sink* sink ));
    func (void to_sink_nl(   const, bcore_sink* sink )); // appends newline
    func (void to_stdout(    const ));
    func (void to_stdout_nl( const )); // appends newline

    /** compacted version, single line */
    func (void compact_to_sink( const, sz_t max_size, bcore_sink* sink ));
    func (void brief_to_sink(   const, bcore_sink* sink ));
    func (void brief_to_stdout( const ));

    /** multiline version */
    func (void formatted_to_sink(   const, bcore_sink* sink ));
    func (void formatted_to_stdout( const ));
};

/**********************************************************************************************************************/

stamp :holor_adl_s = aware x_array
{
    :holor_s => [];
    func bcore_fp.copy_typed;
    wrap x_array.set_size;
    wrap x_array.clear;
    wrap x_array.push_c;
    wrap x_array.push_d;

    /// sets adl from text source. Format "<holor> , <holor> , ..."
    func (bhvm_holor_adl_s* parse( mutable, bcore_source* source ));

}; // dynamic array of links

stamp :holor_ads_s = aware x_array
{
    :holor_s [];
    wrap x_array.set_size;
    wrap x_array.clear;
    wrap x_array.push_c;
    wrap x_array.push_d;
}; // dynamic array of solids

stamp :holor_mdl_s = aware x_array
{
    :holor_adl_s => [];
    wrap x_array.set_size;
    wrap x_array.clear;
    wrap x_array.push_c;
    wrap x_array.push_d;
}; // dynamic matrix of links

/**********************************************************************************************************************/

/// value statistics
stamp :stats_s  = aware :
{
    f3_t min;
    f3_t max;
    f3_t sum;
    f3_t sqr_sum;
    sz_t size;

    func (void clear( mutable ));       // clear accumulation
    func (void acc( mutable, f3_t v )); // accumulates a value
    func (void acc_stats( mutable, bhvm_stats_s* stats )); // accumulates stats to o
    func (f3_t get_avg( const )); // returns average
    func (f3_t get_var( const )); // returns variance
    func (f3_t get_dev( const )); // returns stddev
    func (void to_sink( const, bcore_sink* sink ));
};

/**********************************************************************************************************************/

#endif // XOILA_SECTION

#endif // TYPEOF_bhvm

/**********************************************************************************************************************/

vd_t bhvm_holor_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BHVM_HOLOR_H



