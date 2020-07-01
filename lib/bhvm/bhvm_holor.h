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
#include "bhvm_planted.h"
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

PLANT_GROUP( bhvm, bcore_inst )
#ifdef PLANT_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stamp :shape = bcore_array
{
    sz_t [];
};

stamp :value = bcore_array
{
    typed [];
};

stamp :holor = aware bcore_inst
{
    :shape_s s;
    :value_s v;
    func bcore_fp : copy_typed;
};

stamp :holor_adl = aware bcore_array { :holor_s     => [];  func bcore_fp : copy_typed; }; // dynamic array of links
stamp :holor_ads = aware bcore_array { :holor_s        []; }; // dynamic array of solids
stamp :holor_mdl = aware bcore_array { :holor_adl_s => []; }; // dynamic matrix of links

/// value statistics
stamp :stats  = aware :
{
    f3_t min;
    f3_t max;
    f3_t sum;
    f3_t sqr_sum;
    sz_t size;
};

#endif // PLANT_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**********************************************************************************************************************/
/// shape

/// weak reference; no shutdown required
static inline void bhvm_shape_s_init_weak( bhvm_shape_s* o, sz_t* data, sz_t size )
{
    bhvm_shape_s_init( o );
    o->data = data;
    o->size = size;
}

/// weak reference; no shutdown required
static inline void bhvm_shape_s_init_weak_from_shape( bhvm_shape_s* o, bhvm_shape_s* src )
{
    bhvm_shape_s_init_weak( o, src->data, src->size );
}

/// forked reference; shutdown required
static inline void bhvm_shape_s_init_fork( bhvm_shape_s* o, sz_t* data, sz_t size, sz_t space )
{
    bhvm_shape_s_init_weak( o, bcore_fork( data ), size );
    o->space = space;
}

/// forked reference if src is strong, otherwise weak reference
static inline void bhvm_shape_s_fork( bhvm_shape_s* o, bhvm_shape_s* src )
{
    bhvm_shape_s_clear( o );
    assert( o->space == 0 );
    o->data  = ( src->space > 0 ) ? bcore_fork( src->data ) : src->data;
    o->size  = src->size;
    o->space = src->space;
}

/// weak reference;
static inline void bhvm_shape_s_weak( bhvm_shape_s* o, bhvm_shape_s* src )
{
    bhvm_shape_s_clear( o );
    assert( o->space == 0 );
    o->data  = src->data;
    o->size  = src->size;
}

static inline void bhvm_shape_s_make_strong( bhvm_shape_s* o ) { bcore_array_t_make_strong( TYPEOF_bhvm_shape_s, ( bcore_array* )o ); }

static inline sz_t bhvm_shape_s_get_volume( const bhvm_shape_s* o ) { sz_t v = 1; BFOR_EACH( i, o ) v *= o->data[ i ]; return v; }

/// sets data by copying
void bhvm_shape_s_set_data(    bhvm_shape_s* o, const sz_t* data, sz_t size );
void bhvm_shape_s_set_data_nv( bhvm_shape_s* o, sz_t size, va_list sz_t_args );
void bhvm_shape_s_set_data_na( bhvm_shape_s* o, sz_t size, ... );

/// sets shape to scalar
void bhvm_shape_s_set_scalar( bhvm_shape_s* o );

/// sets shape to vector ( dim [ #)
void bhvm_shape_s_set_vector( bhvm_shape_s* o, sz_t dim );

/// status
static inline bl_t bhvm_shape_s_is_weak(  const bhvm_shape_s* o ) { return ( o->space == 0 ) && ( o->size > 0 ); }
bl_t bhvm_shape_s_is_equal( const bhvm_shape_s* o, const bhvm_shape_s* b );

// b is a sub-shape of o
bl_t bhvm_shape_s_is_sub(   const bhvm_shape_s* o, const bhvm_shape_s* b );

/// constructive catenation
bl_t bhvm_shape_s_cat_can(  const bhvm_shape_s* a, const bhvm_shape_s* b );
bl_t bhvm_shape_s_cat_fits( const bhvm_shape_s* a, const bhvm_shape_s* b, const bhvm_shape_s* r );
bl_t bhvm_shape_s_is_cat(   const bhvm_shape_s* a, const bhvm_shape_s* b, const bhvm_shape_s* r );
void bhvm_shape_s_cat(      const bhvm_shape_s* a, const bhvm_shape_s* b,       bhvm_shape_s* r );
void bhvm_shape_s_cat_set(  const bhvm_shape_s* a, const bhvm_shape_s* b,       bhvm_shape_s* r );

/// conservative catenation
bl_t bhvm_shape_s_ccat_can(  const bhvm_shape_s* a, const bhvm_shape_s* b );
bl_t bhvm_shape_s_ccat_fits( const bhvm_shape_s* a, const bhvm_shape_s* b, const bhvm_shape_s* r );
bl_t bhvm_shape_s_is_ccat(   const bhvm_shape_s* a, const bhvm_shape_s* b, const bhvm_shape_s* r );
void bhvm_shape_s_ccat(      const bhvm_shape_s* a, const bhvm_shape_s* b,       bhvm_shape_s* r );
void bhvm_shape_s_ccat_set(  const bhvm_shape_s* a, const bhvm_shape_s* b,       bhvm_shape_s* r );

/// isovolumetric conversion to a vector (order 1); o == src allowed
bhvm_shape_s* bhvm_shape_s_copy_vector_isovol( bhvm_shape_s* o, const bhvm_shape_s* src );

/**********************************************************************************************************************/
/// value

/// weak reference; no shutdown required
static inline void bhvm_value_s_init_weak( bhvm_value_s* o, tp_t type, vd_t* data, sz_t size )
{
    bhvm_value_s_init( o );
    o->type = type;
    o->data = data;
    o->size = size;
}

/// weak reference; no shutdown required
static inline void bhvm_value_s_init_weak_from_value( bhvm_value_s* o, bhvm_value_s* src )
{
    bhvm_value_s_init_weak( o, src->type, src->data, src->size );
}

/// forked reference if src is strong, otherwise weak reference
static inline void bhvm_value_s_fork( bhvm_value_s* o, bhvm_value_s* src )
{
    bhvm_value_s_clear( o );
    assert( o->space == 0 );
    o->type  = src->type;
    o->data  = ( src->space > 0 ) ? bcore_fork( src->data ) : src->data;
    o->size  = src->size;
    o->space = src->space;
}

/// weak reference;
static inline void bhvm_value_s_weak( bhvm_value_s* o, bhvm_value_s* src )
{
    bhvm_value_s_clear( o );
    assert( o->space == 0 );
    o->type = src->type;
    o->data = src->data;
    o->size = src->size;
}

static inline void bhvm_value_s_make_strong( bhvm_value_s* o ) { bcore_array_t_make_strong( TYPEOF_bhvm_value_s, ( bcore_array* )o ); }

/// sets type; preexisting type (if any) is converted
void bhvm_value_s_set_type( bhvm_value_s* o, tp_t type );

/// sets type and size
void bhvm_value_s_set_type_size( bhvm_value_s* o, tp_t type, sz_t size );

/// sets type and data by copying/converting
void bhvm_value_s_set_type_data( bhvm_value_s* o, tp_t dst_type, tp_t src_type, vc_t src_data, sz_t size );

/// copies data in place; converts type; no allocation; o->type must exits
void bhvm_value_s_cpy_data( bhvm_value_s* o, tp_t src_type, vc_t src_data, sz_t size );
static inline void bhvm_value_s_cpy( const bhvm_value_s* o, bhvm_value_s* dst ) { bhvm_value_s_cpy_data( dst, o->type, o->data, o->size ); }

/// sets all values zero
void bhvm_value_s_zro( bhvm_value_s* o );

/// sets data by converting to holor type (sets target type if not set)
void bhvm_value_s_set_data( bhvm_value_s* o, tp_t src_type, vc_t src_data, sz_t size );

/// forks data (type = src_type)
void bhvm_value_s_fork_data( bhvm_value_s* o, tp_t src_type, vd_t src_data, sz_t size );

/// weakly references data (type = src_type)
void bhvm_value_s_weak_data( bhvm_value_s* o, tp_t src_type, vd_t src_data, sz_t size );

/// pushes data by converting to o->type
void bhvm_value_s_push_data( bhvm_value_s* o, tp_t src_type, vc_t src_data, sz_t size );

/// pushes data of value converting to o->type
void bhvm_value_s_push_value( bhvm_value_s* o, const bhvm_value_s* src );

/// status
static inline bl_t bhvm_value_s_is_weak(   const bhvm_value_s* o ) { return ( o->space == 0 ) && ( o->size > 0 ); }
static inline bl_t bhvm_value_s_is_vacant( const bhvm_value_s* o ) { return ( o->size == 0 ); }
bl_t bhvm_value_s_is_nan(   const bhvm_value_s* o );
bl_t bhvm_value_s_is_equal( const bhvm_value_s* o, const bhvm_value_s* b );

/// constructive (==conservative) concatenation
bl_t bhvm_value_s_cat_can ( const bhvm_value_s* a, const bhvm_value_s* b );
bl_t bhvm_value_s_cat_fits( const bhvm_value_s* a, const bhvm_value_s* b, const bhvm_value_s* r );
void bhvm_value_s_cat     ( const bhvm_value_s* a, const bhvm_value_s* b,       bhvm_value_s* r );
void bhvm_value_s_cat_set ( const bhvm_value_s* a, const bhvm_value_s* b,       bhvm_value_s* r );

/// value -> vector
static inline bmath_vf2_s bhvm_value_s_get_weak_vf2( const bhvm_value_s* o )
{
    ASSERT( o->type == TYPEOF_f2_t );
    return bmath_vf2_init_weak( ( f2_t* )o->data, o->size );
}

static inline bmath_vf3_s bhvm_value_s_get_weak_vf3( const bhvm_value_s* o )
{
    ASSERT( o->type == TYPEOF_f3_t );
    return bmath_vf3_init_weak( ( f3_t* )o->data, o->size );
}


/** Sets all vector elements to random values.
 *  Random generator:
 *    Parameters density, min, max, p_rval apply to the random generator.
 *      rval: Pointer to running variable of random generator.
 *            If NULL, an internal fixed random seed is used.
 *
 *     density (range [0.0, 1.0]) specifies the rate at which the random generator
 *     creates a non-zero value.
 */
void bhvm_value_s_set_random_u3( bhvm_value_s* o, f3_t density, f3_t min, f3_t max, u3_t* p_rval );

/** Frobenius Norm
 *  See bmath_mf3_fdev... for more details
 */
f3_t bhvm_value_s_fdev_equ( const bhvm_value_s* a, const bhvm_value_s* b );
f3_t bhvm_value_s_fdev_zro( const bhvm_value_s* o );

sz_t bhvm_value_s_get_sz( const bhvm_value_s* a, sz_t index );
f3_t bhvm_value_s_get_f3( const bhvm_value_s* a, sz_t index );
void bhvm_value_s_set_f3( const bhvm_value_s* a, sz_t index, f3_t v );

f3_t bhvm_value_s_get_max_f3( const bhvm_value_s* o );
f3_t bhvm_value_s_get_min_f3( const bhvm_value_s* o );

/// order increment by duplication (in place)
void bhvm_value_s_order_inc(     const bhvm_value_s* o, sz_t dim, bhvm_value_s* r ); // in place
void bhvm_value_s_order_inc_set( const bhvm_value_s* o, sz_t dim, bhvm_value_s* r ); // allocating, order_inc

/// order decrement by indexing (in place); dim is leading dimension of o
void bhvm_value_s_order_dec(     const bhvm_value_s* o, sz_t dim, sz_t idx, bhvm_value_s* r ); // in place
void bhvm_value_s_order_dec_set( const bhvm_value_s* o, sz_t dim, sz_t idx, bhvm_value_s* r ); // allocating, order_dec

/// accumulates values in stats; returns stats
bhvm_stats_s* bhvm_value_s_stats_acc( const bhvm_value_s* o, bhvm_stats_s* stats );

/**********************************************************************************************************************/
/// general mathematics

/// o[ i ] += a[ i ]
void bhvm_value_s_acc( bhvm_value_s* o, const bhvm_value_s* a );

/// returns sum{ ( o[i] ) }
f3_t bhvm_value_s_sum( const bhvm_value_s* o );

/// returns sum{ ( o[i]^2 ) } (dot product)
f3_t bhvm_value_s_sum_sqr( const bhvm_value_s* o );

/// returns sum{ ( a[i] - b[i] )^2 }
f3_t bhvm_value_s_sub_sqr_sum( const bhvm_value_s* a, const bhvm_value_s* b );

/// r[ i ] = a[ i ] * scl
void bhvm_value_s_mul_scl_f3( const bhvm_value_s* a, f3_t scl, bhvm_value_s* r );

/// r[ i ] += a[ i ] * scl
void bhvm_value_s_mul_scl_f3_acc( const bhvm_value_s* a, f3_t scl, bhvm_value_s* r );

/// r[ i + r_offs ] += a[ i + a_offs ]
void bhvm_value_s_acc_offs( const bhvm_value_s* a, sz_t a_offs, bhvm_value_s* r, sz_t r_offs, sz_t size );

/**********************************************************************************************************************/
/// holor

/// weak reference; no shutdown required
static inline void bhvm_holor_s_init_weak( bhvm_holor_s* o, sz_t* s_data, sz_t s_size, tp_t v_type, vd_t* v_data, sz_t v_size )
{
    bhvm_holor_s_init( o );
    bhvm_shape_s_init_weak( &o->s, s_data, s_size );
    bhvm_value_s_init_weak( &o->v, v_type, v_data, v_size );
}

/// weak reference; no shutdown required
static inline void bhvm_holor_s_init_weak_from_holor( bhvm_holor_s* o, bhvm_holor_s* src )
{
    bhvm_holor_s_init( o );
    bhvm_shape_s_init_weak_from_shape( &o->s, &src->s );
    bhvm_value_s_init_weak_from_value( &o->v, &src->v );
}

/// forked reference; (shutdown required)
static inline void bhvm_holor_s_fork( bhvm_holor_s* o, bhvm_holor_s* src )
{
    bhvm_shape_s_fork( &o->s, &src->s );
    bhvm_value_s_fork( &o->v, &src->v );
}

/// fork or copy according to flags
static inline void bhvm_holor_s_fork_copy( bhvm_holor_s* o, bhvm_holor_s* src, bl_t fork_shape, bl_t fork_value )
{
    if( fork_shape ) bhvm_shape_s_fork( &o->s, &src->s ); else bhvm_shape_s_copy( &o->s, &src->s );
    if( fork_value ) bhvm_value_s_fork( &o->v, &src->v ); else bhvm_shape_s_copy( &o->s, &src->s );
}

/// copies value and converts shape to vector
static inline bhvm_holor_s* bhvm_holor_s_copy_vector_isovol( bhvm_holor_s* o, const bhvm_holor_s* src )
{
    bhvm_shape_s_copy_vector_isovol( &o->s, &src->s );
    bhvm_value_s_copy( &o->v, &src->v );
    return o;
}

/// forks value and converts shape to vector
static inline bhvm_holor_s* bhvm_holor_s_fork_vector_isovol( bhvm_holor_s* o, bhvm_holor_s* src )
{
    bhvm_shape_s_copy_vector_isovol( &o->s, &src->s );
    bhvm_value_s_fork( &o->v, &src->v );
    return o;
}

/// clears entire holor
static inline bhvm_holor_s* bhvm_holor_s_clear( bhvm_holor_s* o )
{
    bhvm_shape_s_clear( &o->s );
    bhvm_value_s_clear( &o->v );
    return o;
}

void bhvm_holor_s_copy_typed( bhvm_holor_s* o, tp_t type, vc_t src );

bhvm_holor_s* bhvm_holor_s_copy_t( bhvm_holor_s* o, tp_t type, vc_t src );

/// Copies shape; clears value
bhvm_holor_s* bhvm_holor_s_copy_shape( bhvm_holor_s* o, const bhvm_shape_s* src );

/// Copies shape; clears value; copies type
bhvm_holor_s* bhvm_holor_s_copy_shape_type( bhvm_holor_s* o, const bhvm_holor_s* src );

bhvm_holor_s* bhvm_holor_s_set_type(      bhvm_holor_s* o, tp_t type );
bhvm_holor_s* bhvm_holor_s_fit_size(      bhvm_holor_s* o );
bhvm_holor_s* bhvm_holor_s_fit_type_size( bhvm_holor_s* o, tp_t t );

/// sets holor to scalar with given value or to vacant scalar
void bhvm_holor_s_set_type_scalar_pf( bhvm_holor_s* o, tp_t t, tp_t t_src, vc_t v );
void bhvm_holor_s_set_scalar_pf(      bhvm_holor_s* o,         tp_t t_src, vc_t v );
static inline void bhvm_holor_s_set_scalar_f3( bhvm_holor_s* o, f3_t v ) { bhvm_holor_s_set_scalar_pf( o, TYPEOF_f3_t, &v ); }

static inline f3_t bhvm_holor_s_f3_get_scalar( const bhvm_holor_s* o )
{
    assert( o->v.size == 1 );
    return o->v.type == TYPEOF_f3_t ? ( ( f3_t* )o->v.data )[ 0 ] : o->v.type == TYPEOF_f2_t ? ( ( f2_t* )o->v.data )[ 0 ] : 0;
}

/// sets holor to scalar from f3 value
static inline void bhvm_holor_s_set_type_scalar( bhvm_holor_s* o, tp_t t, f3_t v ) { bhvm_holor_s_set_type_scalar_pf( o, t, TYPEOF_f3_t, &v ); }

/// sets holor to vacant scalar or vector
bhvm_holor_s* bhvm_holor_s_set_type_scalar_vacant( bhvm_holor_s* o, tp_t type );
bhvm_holor_s* bhvm_holor_s_set_type_vector_vacant( bhvm_holor_s* o, tp_t type, sz_t dim );

/// Overall consistency; all valid states return true;
bl_t bhvm_holor_s_is_consistent( const bhvm_holor_s* o );

/// Overall consistency; Invalid state produces an error.
void bhvm_holor_s_check_integrity( const bhvm_holor_s* o );

/// sets holor from text source
bhvm_holor_s* bhvm_holor_s_parse(    bhvm_holor_s* o, bcore_source* source );
bhvm_holor_s* bhvm_holor_s_parse_st( bhvm_holor_s* o, const st_s* st );
bhvm_holor_s* bhvm_holor_s_parse_sc( bhvm_holor_s* o, sc_t sc );

bhvm_holor_s* bhvm_holor_s_create_parse(    bcore_source* source );
bhvm_holor_s* bhvm_holor_s_create_parse_st( const st_s* st );
bhvm_holor_s* bhvm_holor_s_create_parse_sc( sc_t sc );

static inline bl_t bhvm_holor_s_is_equal( const bhvm_holor_s* o, const bhvm_holor_s* src )
{
    if( !bhvm_shape_s_is_equal( &o->s, &src->s ) ) return false;
    if( !bhvm_value_s_is_equal( &o->v, &src->v ) ) return false;
    return true;
}

/**********************************************************************************************************************/
/// weak conversion

// ---------------------------------------------------------------------------------------------------------------------

/// holor -> matrix
static inline bmath_mf2_s bhvm_holor_s_get_weak_mf2( const bhvm_holor_s* o )
{
    assert( o->v.type == TYPEOF_f2_t );
    assert( o->s.size == 2 );
    sz_t cols = o->s.data[ 0 ];
    sz_t rows = o->s.data[ 1 ];
    assert( o->v.size == cols * rows );
    return bmath_mf2_init_weak( rows, cols, cols, ( f2_t* )o->v.data );
}

// ---------------------------------------------------------------------------------------------------------------------

/// holor -> matrix
static inline bmath_mf3_s bhvm_holor_s_get_weak_mf3( const bhvm_holor_s* o )
{
    assert( o->v.type == TYPEOF_f3_t );
    assert( o->s.size == 2 );
    sz_t cols = o->s.data[ 0 ];
    sz_t rows = o->s.data[ 1 ];
    assert( o->v.size == cols * rows );
    return bmath_mf3_init_weak( rows, cols, cols, ( f3_t* )o->v.data );
}

// ---------------------------------------------------------------------------------------------------------------------

static inline vd_t bhvm_holor_s_mfx_create_weak( const bhvm_holor_s* o )
{
    assert( o->s.size == 2 );
    sz_t cols = o->s.data[ 0 ];
    sz_t rows = o->s.data[ 1 ];
    assert( o->v.size == cols * rows );
    switch( o->v.type )
    {
        case TYPEOF_f2_t: return bmath_mf2_s_create_weak( rows, cols, cols, ( f2_t* )o->v.data );
        case TYPEOF_f3_t: return bmath_mf3_s_create_weak( rows, cols, cols, ( f3_t* )o->v.data );
        default: ERR_fa( "Invalid type" );
    }
    return NULL;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// general mathematics

/// sets all values zero
void bhvm_holor_s_zro(     bhvm_holor_s* o );
void bhvm_holor_s_zro_set( bhvm_holor_s* o ); // allocates o->v if necessary

/// returns sum{ ( o[i] ) }
f3_t bhvm_holor_s_sum( const bhvm_holor_s* o );

/// returns sum{ ( o[i]^2 ) } (dot product)
f3_t bhvm_holor_s_sum_sqr( const bhvm_holor_s* o );

/// returns sum{ ( a[i] - b[i] )^2 }
f3_t bhvm_holor_s_sub_sqr_sum( const bhvm_holor_s* a, const bhvm_holor_s* b );

// o[i] += a[i];
void bhvm_holor_s_acc(     bhvm_holor_s* o, const bhvm_holor_s* a );
void bhvm_holor_s_acc_set( bhvm_holor_s* o, const bhvm_holor_s* a ); // allocates value and initializes zero if necessary

/**********************************************************************************************************************/
/// holor specific operations

static inline sz_t bhvm_holor_s_get_order( const bhvm_holor_s* o ) { return o->s.size; }

/** Canonic increment of order by appending one dimension 'dim' :
 *  If the holder holds data, the data is duplicated 'dim' times.
 */
void bhvm_holor_s_inc_order( bhvm_holor_s* o, sz_t dim );

/** Canonic increment of order by prepending one dimension 'dim' :
 *  If the holder holds data, the data is duplicated on element-level 'dim' times.
 */
void bhvm_holor_s_inc_order_prepend( bhvm_holor_s* o, sz_t dim );

/** Canonic data append of a sub-holor
 *  Requirements (checked):
 *  * o and src must either be both vacant or both determined.
 *  * o->d_size == src->d_size || o->d_size == src->d_size + 1
 *  * src->d_data must match (lower significant) o->d_data
 */
void bhvm_holor_s_push( bhvm_holor_s* o, const bhvm_holor_s* src );

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
bl_t bhvm_holor_s_cat_can( const bhvm_holor_s* a, const bhvm_holor_s* b                  ); // test
void bhvm_holor_s_cat_set( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r ); // cat with allocating r
void bhvm_holor_s_cat    ( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r ); // in place

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
bl_t bhvm_holor_s_ccat_can( const bhvm_holor_s* a, const bhvm_holor_s* b                  ); // test
void bhvm_holor_s_ccat_set( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r ); // cat with allocating r
void bhvm_holor_s_ccat    ( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r ); // in place

/** Order increment
 *  A new leading dimension is appended to shape.
 *  holor value is duplicated dim-times
 */
void bhvm_holor_s_order_inc_set(  const bhvm_holor_s* o, sz_t dim, bhvm_holor_s* r ); // order increment allocating r
void bhvm_holor_s_order_inc    (  const bhvm_holor_s* o, sz_t dim, bhvm_holor_s* r ); // order increment in place

/** Order decrement
 *  Leading dimension is removed from shape.
 *  indexed sub-holor is copied or referenced
 */
void bhvm_holor_s_order_dec_set(  const bhvm_holor_s* o, sz_t idx, bhvm_holor_s* r ); // order decrement by indexing into leading dimension allocating r
void bhvm_holor_s_order_dec    (  const bhvm_holor_s* o, sz_t idx, bhvm_holor_s* r ); // order decrement by indexing into leading dimension in place
void bhvm_holor_s_order_dec_weak(       bhvm_holor_s* o, sz_t idx, bhvm_holor_s* r ); // order decrement by indexing into leading dimension r is weakly referencing a

void bhvm_holor_s_to_sink(      const bhvm_holor_s* o, bcore_sink* sink );
void bhvm_holor_s_to_sink_nl(   const bhvm_holor_s* o, bcore_sink* sink ); // appends newline
void bhvm_holor_s_to_stdout(    const bhvm_holor_s* o );
void bhvm_holor_s_to_stdout_nl( const bhvm_holor_s* o ); // appends newline

/** compacted version, single line */
void bhvm_holor_s_compact_to_sink( const bhvm_holor_s* o, sz_t max_size, bcore_sink* sink );
void bhvm_holor_s_brief_to_sink(   const bhvm_holor_s* o, bcore_sink* sink );
void bhvm_holor_s_brief_to_stdout( const bhvm_holor_s* o );

/** multiline version */
void bhvm_holor_s_formatted_to_sink(   const bhvm_holor_s* o, bcore_sink* sink );
void bhvm_holor_s_formatted_to_stdout( const bhvm_holor_s* o );

/**********************************************************************************************************************/
/// holor_adl

/// sets adl from text source. Format "<holor> , <holor> , ..."
bhvm_holor_adl_s* bhvm_holor_adl_s_parse( bhvm_holor_adl_s* o, bcore_source* source );

/**********************************************************************************************************************/
/// stats

void bhvm_stats_s_clear( bhvm_stats_s* o );       // clear accumulation
void bhvm_stats_s_acc( bhvm_stats_s* o, f3_t v ); // accumulates a value
void bhvm_stats_s_acc_stats( bhvm_stats_s* o, bhvm_stats_s* stats ); // accumulates stats to o
f3_t bhvm_stats_s_get_avg( const bhvm_stats_s* o ); // returns average
f3_t bhvm_stats_s_get_var( const bhvm_stats_s* o ); // returns variance
f3_t bhvm_stats_s_get_dev( const bhvm_stats_s* o ); // returns stddev
void bhvm_stats_s_to_sink( const bhvm_stats_s* o, bcore_sink* sink );

//----------------------------------------------------------------------------------------------------------------------

#endif // TYPEOF_bhvm

/**********************************************************************************************************************/

vd_t bhvm_holor_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BHVM_HOLOR_H



