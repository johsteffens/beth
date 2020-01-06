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
 *  A Holor represents the canonic extension of a matrix (or generalization of a tensor).
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
#define BHVM_SHAPE_DIM_VALID( dim ) ( ( dim > 0 ) && ( ( dim >> 40 ) == 0 ) )

/**********************************************************************************************************************/

#ifdef TYPEOF_bhvm

PLANT_GROUP( bhvm, bcore_inst )
#ifdef PLANT_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stamp :shape = bcore_array
{
    sz_t  [];
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

stamp :holor_adl = aware bcore_array { :holor_s => []; }; // dynamic array of links
stamp :holor_ads = aware bcore_array { :holor_s    []; }; // dynamic array of solids

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

/// forked reference; shutdown required
static inline void bhvm_shape_s_init_fork_from_shape( bhvm_shape_s* o, bhvm_shape_s* src )
{
    bhvm_shape_s_init_fork( o, src->data, src->size, src->space );
}

/// forked reference;
static inline void bhvm_shape_s_fork( bhvm_shape_s* o, bhvm_shape_s* src )
{
    bhvm_shape_s_clear( o );
    o->data  = bcore_fork( src->data );
    o->size  = src->size;
    o->space = src->space;
}

static inline void bhvm_shape_s_make_strong( bhvm_shape_s* o ) { bcore_array_t_make_strong( TYPEOF_bhvm_shape_s, ( bcore_array* )o ); }

static inline sz_t bhvm_shape_s_get_volume( const bhvm_shape_s* o ) { sz_t v = 1; BFOR_EACH( i, o ) v *= o->data[ i ]; return v; }

/// sets data by copying
void bhvm_shape_s_set_data(    bhvm_shape_s* o, const sz_t* data, sz_t size );
void bhvm_shape_s_set_data_nv( bhvm_shape_s* o, sz_t size, va_list sz_t_args );
void bhvm_shape_s_set_data_na( bhvm_shape_s* o, sz_t size, ... );

/// status
static inline bl_t bhvm_shape_s_is_weak(  const bhvm_shape_s* o ) { return ( o->space == 0 ) && ( o->size > 0 ); }
bl_t bhvm_shape_s_is_equal( const bhvm_shape_s* o, const bhvm_shape_s* b );

bl_t bhvm_shape_s_can_cat(  const bhvm_shape_s* a, const bhvm_shape_s* b );
bl_t bhvm_shape_s_fits_cat( const bhvm_shape_s* a, const bhvm_shape_s* b, const bhvm_shape_s* r );
bl_t bhvm_shape_s_is_cat(   const bhvm_shape_s* a, const bhvm_shape_s* b, const bhvm_shape_s* r );
void bhvm_shape_s_cat(      const bhvm_shape_s* a, const bhvm_shape_s* b,       bhvm_shape_s* r );
void bhvm_shape_s_cat_set(  const bhvm_shape_s* a, const bhvm_shape_s* b,       bhvm_shape_s* r );

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

/// forked reference; shutdown required
static inline void bhvm_value_s_init_fork( bhvm_value_s* o, tp_t type, vd_t* data, sz_t size, sz_t space )
{
    bhvm_value_s_init_weak( o, type, bcore_fork( data ), size );
    o->space = space;
}

/// forked reference; shutdown required
static inline void bhvm_value_s_init_fork_from_value( bhvm_value_s* o, bhvm_value_s* src )
{
    bhvm_value_s_init_fork( o, src->type, src->data, src->size, src->space );
}

/// forked reference;
static inline void bhvm_value_s_fork( bhvm_value_s* o, bhvm_value_s* src )
{
    bhvm_value_s_clear( o );
    o->type  = src->type;
    o->data  = bcore_fork( src->data );
    o->size  = src->size;
    o->space = src->space;
}

static inline void bhvm_value_s_make_strong( bhvm_value_s* o ) { bcore_array_t_make_strong( TYPEOF_bhvm_value_s, ( bcore_array* )o ); }

/// sets type; preexisting type (if any) is converted
void bhvm_value_s_set_type( bhvm_value_s* o, tp_t type );

/// sets type and size
void bhvm_value_s_set_type_size( bhvm_value_s* o, tp_t type, sz_t size );

/// sets type and data by copying/converting
void bhvm_value_s_set_type_data( bhvm_value_s* o, tp_t dst_type, tp_t src_type, vc_t src_data, sz_t size );

/// sets data by converting to holor type (sets target type if not set)
void bhvm_value_s_set_data( bhvm_value_s* o, tp_t src_type, vc_t src_data, sz_t size );

/// pushes data by converting to o->type
void bhvm_value_s_push_data( bhvm_value_s* o, tp_t src_type, vc_t src_data, sz_t size );

/// pushes data of value converting to o->type
void bhvm_value_s_push_value( bhvm_value_s* o, const bhvm_value_s* src );

/// status
static inline bl_t bhvm_value_s_is_weak(   const bhvm_value_s* o ) { return ( o->space == 0 ) && ( o->size > 0 ); }
static inline bl_t bhvm_value_s_is_vacant( const bhvm_value_s* o ) { return ( o->size == 0 ); }
bl_t bhvm_value_s_is_nan(   const bhvm_value_s* o );
bl_t bhvm_value_s_is_equal( const bhvm_value_s* o, const bhvm_value_s* b );

/// concatenation
bl_t bhvm_value_s_can_cat ( const bhvm_value_s* a, const bhvm_value_s* b );
bl_t bhvm_value_s_fits_cat( const bhvm_value_s* a, const bhvm_value_s* b, const bhvm_value_s* r );
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
void bhvm_value_s_set_random( bhvm_value_s* o, f3_t density, f3_t min, f3_t max, u2_t* p_rval );

/** Frobenius Norm
 *  See bmath_mf3_fdev... for more details
 */
f3_t bhvm_value_s_fdev_equ( const bhvm_value_s* a, const bhvm_value_s* b );
f3_t bhvm_value_s_fdev_zro( const bhvm_value_s* o );

f3_t bhvm_value_s_get_f3( const bhvm_value_s* a, sz_t index );
void bhvm_value_s_set_f3( const bhvm_value_s* a, sz_t index, f3_t v );

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

/// forked reference; shutdown required
static inline void bhvm_holor_s_init_fork( bhvm_holor_s* o, sz_t* s_data, sz_t s_size, sz_t s_space, tp_t v_type, vd_t* v_data, sz_t v_size, sz_t v_space )
{
    bhvm_holor_s_init( o );
    bhvm_shape_s_init_fork( &o->s, s_data, s_size, s_space );
    bhvm_value_s_init_fork( &o->v, v_type, v_data, v_size, v_space );
}

/// forked reference; shutdown required
static inline void bhvm_holor_s_init_fork_from_holor( bhvm_holor_s* o, bhvm_holor_s* src )
{
    bhvm_holor_s_init( o );
    bhvm_shape_s_init_fork_from_shape( &o->s, &src->s );
    bhvm_value_s_init_fork_from_value( &o->v, &src->v );
}

/// forked reference; shutdown required
static inline void bhvm_holor_s_fork( bhvm_holor_s* o, bhvm_holor_s* src )
{
    bhvm_shape_s_fork( &o->s, &src->s );
    bhvm_value_s_fork( &o->v, &src->v );
}

/// clears entire holor
static inline void bhvm_holor_s_clear( bhvm_holor_s* o )
{
    bhvm_shape_s_clear( &o->s );
    bhvm_value_s_clear( &o->v );
}

void bhvm_holor_s_copy_typed( bhvm_holor_s* o, tp_t type, vc_t src );

bhvm_holor_s* bhvm_holor_s_copy_t( bhvm_holor_s* o, tp_t type, vc_t src );

/// Copies shape; clears value
bhvm_holor_s* bhvm_holor_s_copy_shape( bhvm_holor_s* o, bhvm_shape_s* src );

/// Copies shape; clears value; copies type
bhvm_holor_s* bhvm_holor_s_copy_shape_type( bhvm_holor_s* o, bhvm_holor_s* src );

static inline bhvm_holor_s* bhvm_holor_s_set_type(      bhvm_holor_s* o, tp_t type ) { bhvm_value_s_set_type( &o->v, type ); return o; }
static inline bhvm_holor_s* bhvm_holor_s_fit_size(      bhvm_holor_s* o )            { bhvm_value_s_set_size(      &o->v,    bhvm_shape_s_get_volume( &o->s ) ); return o; }
static inline bhvm_holor_s* bhvm_holor_s_fit_type_size( bhvm_holor_s* o, tp_t t )    { bhvm_value_s_set_type_size( &o->v, t, bhvm_shape_s_get_volume( &o->s ) ); return o; }

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

/// Overall consistency; all valid states return true;
bl_t bhvm_holor_s_is_consistent( const bhvm_holor_s* o );

/// Overall consistency; Invalid state produces an error.
void bhvm_holor_s_check_integrity( const bhvm_holor_s* o );

/// sets holor from text source
void bhvm_holor_s_parse( bhvm_holor_s* o, bcore_source* source );

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
/// casts  (make the target weakly reference source data)

/// cast to transposed holor
static inline void bhvm_holor_s_cast_htp( const bhvm_holor_s* o, bhvm_holor_s* r )
{
    bhvm_holor_s_clear( r );
    bhvm_shape_s_init_weak( &r->s, o->s.data, o->s.size );
    bhvm_value_s_init_weak( &r->v, o->v.type, o->v.data, o->v.size );
}

// ---------------------------------------------------------------------------------------------------------------------

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

/** Canonic catenation of two holors.
 *  r->v_data is catenated from a->v_data and b->v_data
 *  Shape of r:
 *  If a, b have equal shape then r has shape [2]a
 *  If a[0] has shape of b, then r[0] has shape b and dimof(r) == dimof(a) + 1
 *  If b[0] has shape of a, then r[0] has shape a and dimof(r) == dimof(b) + 1
 *  Other constellations are not allowed.
 */
bl_t bhvm_holor_s_can_cat( const bhvm_holor_s* a, const bhvm_holor_s* b                  ); // test
void bhvm_holor_s_cat    ( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r ); // in place
void bhvm_holor_s_cat_set( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r ); // allocating r

void bhvm_holor_s_to_sink(      const bhvm_holor_s* o, bcore_sink* sink );
void bhvm_holor_s_to_sink_nl(   const bhvm_holor_s* o, bcore_sink* sink ); // appends newline
void bhvm_holor_s_to_stdout(    const bhvm_holor_s* o );
void bhvm_holor_s_to_stdout_nl( const bhvm_holor_s* o ); // appends newline

/** compacted version, single line */
void bhvm_holor_s_brief_to_sink(       const bhvm_holor_s* o, bcore_sink* sink );
void bhvm_holor_s_brief_to_stdout(     const bhvm_holor_s* o );

/** multiline version */
void bhvm_holor_s_formatted_to_sink(   const bhvm_holor_s* o, bcore_sink* sink );
void bhvm_holor_s_formatted_to_stdout( const bhvm_holor_s* o );

//----------------------------------------------------------------------------------------------------------------------

#endif // TYPEOF_bhvm

/**********************************************************************************************************************/

vd_t bhvm_holor_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BHVM_HOLOR_H



