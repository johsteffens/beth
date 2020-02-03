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

#include "bhvm_holor.h"
#include "bhvm_hop.h"

#ifdef TYPEOF_bhvm

/**********************************************************************************************************************/
/// shape

// ---------------------------------------------------------------------------------------------------------------------

bl_t bhvm_shape_s_is_consistent( const bhvm_shape_s* o )
{
    if( ( o->data == NULL ) && ( o->size != 0 ) ) return false;
    BFOR_EACH( i, o )
    {
        if( o->data[ i ] <= 0 ) return false;
    }
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_shape_s_check_integrity( const bhvm_shape_s* o )
{
    if( ( o->data == NULL ) && ( o->size != 0 ) ) ERR_fa( "size == #<sz_t> but data not allocated.", o->size );
    BFOR_EACH( i, o )
    {
        if( o->data[ i ] <= 0 ) ERR_fa( "Dimension '#<sz_t>' is '#<sz_t>'", i, o->data[ i ] );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_shape_s_set_data( bhvm_shape_s* o, const sz_t* data, sz_t size )
{
    bhvm_shape_s_set_size( o, size );
    BFOR_EACH( i, o ) o->data[ i ] = data[ i ];
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_shape_s_set_scalar_vacant( bhvm_shape_s* o )
{
    bhvm_shape_s_set_size( o, 0 );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_shape_s_set_data_nv( bhvm_shape_s* o, sz_t size, va_list sz_t_args )
{
    bhvm_shape_s_set_size( o, size );
    BFOR_EACH( i, o )
    {
        sz_t v = va_arg( sz_t_args, sz_t );
        assert( v > 0 );
        if( !BHVM_SHAPE_DIM_VALID( v ) ) ERR_fa( "Dimension '#<sz_t>' is '#<sz_t>', with is out of range.", i, v );
        o->data[ i ] = v;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_shape_s_set_data_na( bhvm_shape_s* o, sz_t size, ... )
{
    va_list args;
    va_start( args, size );
    bhvm_shape_s_set_data_nv( o, size, args );
    va_end( args );
}

// ---------------------------------------------------------------------------------------------------------------------

bl_t bhvm_shape_s_is_equal( const bhvm_shape_s* o, const bhvm_shape_s* b )
{
    if( o->size != b->size ) return false;
    BFOR_EACH( i, o ) if( o->data[ i ] != b->data[ i ] ) return false;
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------

bl_t bhvm_shape_s_is_sub( const bhvm_shape_s* o, const bhvm_shape_s* b )
{
    if( o->size < b->size ) return false;
    BFOR_EACH( i, b ) if( o->data[ i ] != b->data[ i ] ) return false;
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_shape_s_inc_order( bhvm_shape_s* o, sz_t dim )
{
    bhvm_shape_s_make_strong( o );
    bhvm_shape_s_set_size( o, o->size + 1 );
    o->data[ o->size - 1 ] = dim;
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_shape_s_inc_order_prepend( bhvm_shape_s* o, sz_t dim )
{
    bhvm_shape_s_make_strong( o );
    bhvm_shape_s_set_size( o, o->size + 1 );
    for( sz_t i = o->size - 1; i > 0; i-- ) o->data[ i ] = o->data[ i - 1 ];
    o->data[ 0 ] = dim;
}

// ---------------------------------------------------------------------------------------------------------------------

bl_t bhvm_shape_s_cat_can( const bhvm_shape_s* a, const bhvm_shape_s* b )
{
    if( a->size == b->size ) // a, b have equal shape
    {
        if( !bhvm_shape_s_is_equal( a, b ) ) return false;
    }
    else if( a->size == b->size + 1 )
    {
        for( sz_t i = 0; i < b->size; i++ ) if( a->data[ i ] != b->data[ i ] ) return false;
    }
    else if( a->size + 1 == b->size )
    {
        for( sz_t i = 0; i < a->size; i++ ) if( a->data[ i ] != b->data[ i ] ) return false;
    }
    else
    {
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------

bl_t bhvm_shape_s_cat_fits( const bhvm_shape_s* a, const bhvm_shape_s* b, const bhvm_shape_s* r )
{
    if( !bhvm_shape_s_cat_can( a, b ) ) return false;
    if( a->size == b->size ) // a, b have equal shape
    {
        if( r->size != a->size + 1 ) return false;
    }
    else if( a->size == b->size + 1 )
    {
        if( r->size != a->size ) return false;
    }
    else if( a->size + 1 == b->size )
    {
        if( r->size != b->size ) return false;
    }
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------

bl_t bhvm_shape_s_is_cat( const bhvm_shape_s* a, const bhvm_shape_s* b, const bhvm_shape_s* r )
{
    if( !bhvm_shape_s_cat_fits( a, b, r ) ) return false;

    if( a->size == b->size ) // a, b have equal shape
    {
        BFOR_EACH( i, a ) if( r->data[ i ] != a->data[ i ] ) return false;
        if( r->data[ r->size - 1 ] != 2 ) return false;
    }
    else if( a->size == b->size + 1 )
    {
        BFOR_EACH( i, b ) if( r->data[ i ] != b->data[ i ] ) return false;
        if( r->data[ r->size - 1 ] != a->data[ a->size - 1 ] + 1 ) return false;
    }
    else if( a->size + 1 == b->size )
    {
        BFOR_EACH( i, a ) if( r->data[ i ] != a->data[ i ] ) return false;
        if( r->data[ r->size - 1 ] != b->data[ b->size - 1 ] + 1 ) return false;
    }
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_shape_s_cat( const bhvm_shape_s* a, const bhvm_shape_s* b, bhvm_shape_s* r )
{
    ASSERT( bhvm_shape_s_cat_can( a, b ) );

    if( a->size == b->size ) // a, b have equal shape
    {
        ASSERT( r->size == a->size + 1 );
        BFOR_EACH( i, a ) r->data[ i ] = a->data[ i ];
        r->data[ r->size - 1 ] = 2;
    }
    else if( a->size == b->size + 1 )
    {
        ASSERT( r->size == a->size );
        BFOR_EACH( i, b ) r->data[ i ] = b->data[ i ];
        r->data[ r->size - 1 ] = a->data[ a->size - 1 ] + 1;
    }
    else if( a->size + 1 == b->size )
    {
        ASSERT( r->size == b->size );
        BFOR_EACH( i, a ) r->data[ i ] = a->data[ i ];
        r->data[ r->size - 1 ] = b->data[ b->size - 1 ] + 1;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_shape_s_cat_set( const bhvm_shape_s* a, const bhvm_shape_s* b, bhvm_shape_s* r )
{
    if( a == r || b == r )
    {
        bhvm_shape_s* buf = bhvm_shape_s_create();
        bhvm_shape_s_cat_set( a, b, buf );
        bhvm_shape_s_copy( r, buf );
        bhvm_shape_s_discard( buf );
        return;
    }

    ASSERT( bhvm_shape_s_cat_can( a, b ) );

    if( a->size == b->size ) // a, b have equal shape
    {
        bhvm_shape_s_set_size( r, a->size + 1 );
        bcore_u_memcpy( sizeof( sz_t ), r->data, a->data, a->size );
        r->data[ r->size - 1 ] = 2;
    }
    else if( a->size == b->size + 1 )
    {
        bhvm_shape_s_set_size( r, a->size );
        bcore_u_memcpy( sizeof( sz_t ), r->data, a->data, a->size );
        r->data[ r->size - 1 ] += 1;
    }
    else if( a->size + 1 == b->size )
    {
        bhvm_shape_s_set_size( r, b->size );
        bcore_u_memcpy( sizeof( sz_t ), r->data, b->data, b->size );
        r->data[ r->size - 1 ] += 1;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_shape_s_to_vector(  const bhvm_shape_s* a, bhvm_shape_s* r )
{
    sz_t volume = bhvm_shape_s_get_volume( a );
    bhvm_shape_s_set_data( r, &volume, 1 );
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// value

// ---------------------------------------------------------------------------------------------------------------------

bl_t bhvm_value_s_is_consistent( const bhvm_value_s* o )
{
    if
    (
        ( o->type != 0 ) &&
        ( o->type != TYPEOF_f2_t ) &&
        ( o->type != TYPEOF_f3_t )
    )
    return false;

    if( o->size != 0 )
    {
        if( o->type == 0 )             return false;
        if( o->data == NULL )          return false;
        if( bhvm_value_s_is_nan( o ) ) return false;
    }
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_value_s_check_integrity( const bhvm_value_s* o )
{
    if
    (
        ( o->type != 0 ) &&
        ( o->type != TYPEOF_f2_t ) &&
        ( o->type != TYPEOF_f3_t )
    )
    ERR_fa( "Invalid type '#<sc_t>' specified.", ifnameof( o->type ) );

    if( o->size != 0 )
    {
        if( o->type == 0 )             ERR_fa( "size == #<sz_t> but type is not specified.", o->size );
        if( o->data == NULL )          ERR_fa( "size == #<sz_t> but data not allocated.", o->size );
        if( bhvm_value_s_is_nan( o ) ) ERR_fa( "Value is NAN." );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_value_s_set_type( bhvm_value_s* o, tp_t type )
{
    if( o->type == type ) return;
    if( o->type == 0 )
    {
        bhvm_value_s_clear( o );
        o->type = type;
        return;
    }

    if( o->space == 0 )
    {
        if( o->size > 0 )
        {
            bhvm_value_s_make_strong( o );
        }
        else
        {
            o->type = type;
            return;
        }
    }

    if( type == TYPEOF_f2_t ) // convert in-place; no reallocation; alignment is guaranteed
    {
        ASSERT( o->type == TYPEOF_f3_t );
        f3_t* src = o->data;
        f2_t* dst = o->data;
        for( sz_t i = 0; i < o->size; i++ ) dst[ i ] = src[ i ];
        o->space *= sizeof( f3_t ) / sizeof( f2_t );
        o->type = type;
        return;
    }

    if( type == TYPEOF_f3_t ) // Reallocate even if space would be sufficient. This enforces correct alignment.
    {
        ASSERT( o->type == TYPEOF_f2_t );
        f2_t* src = o->data;
        f3_t* dst = bcore_un_alloc( sizeof( f3_t ), NULL, 0, o->size, &o->space );
        for( sz_t i = 0; i < o->size; i++ ) dst[ i ] = src[ i ];
        bcore_free( src );
        o->type = type;
        return;
    }

    ERR_fa( "Array has invalid type '#<sc_t>'.", ifnameof( o->type ) );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_value_s_set_type_size( bhvm_value_s* o, tp_t type, sz_t size )
{
    bhvm_value_s_set_type( o, type );
    bhvm_value_s_set_size( o, size );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_value_s_set_type_data( bhvm_value_s* o, tp_t dst_type, tp_t src_type, vc_t data, sz_t size )
{
    bhvm_value_s_set_type( o, dst_type );
    bhvm_value_s_set_size( o, size );
    switch( BKNIT_FA2( dst_type, src_type ) )
    {
        case BKNIT_F22: BFOR_EACH( i, o ) ( ( f2_t* )o->data )[ i ] = ( ( f2_t* )data )[ i ]; break;
        case BKNIT_F23: BFOR_EACH( i, o ) ( ( f2_t* )o->data )[ i ] = ( ( f3_t* )data )[ i ]; break;
        case BKNIT_F32: BFOR_EACH( i, o ) ( ( f3_t* )o->data )[ i ] = ( ( f2_t* )data )[ i ]; break;
        case BKNIT_F33: BFOR_EACH( i, o ) ( ( f3_t* )o->data )[ i ] = ( ( f3_t* )data )[ i ]; break;
        default: BKNIT_FA2_ERR( dst_type, src_type ); break;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_value_s_cpy_data( bhvm_value_s* o, tp_t src_type, vc_t data, sz_t size )
{
    assert( o->type );
    assert( o->size == size );
    switch( BKNIT_FA2( o->type, src_type ) )
    {
        case BKNIT_F22: BFOR_EACH( i, o ) ( ( f2_t* )o->data )[ i ] = ( ( f2_t* )data )[ i ]; break;
        case BKNIT_F23: BFOR_EACH( i, o ) ( ( f2_t* )o->data )[ i ] = ( ( f3_t* )data )[ i ]; break;
        case BKNIT_F32: BFOR_EACH( i, o ) ( ( f3_t* )o->data )[ i ] = ( ( f2_t* )data )[ i ]; break;
        case BKNIT_F33: BFOR_EACH( i, o ) ( ( f3_t* )o->data )[ i ] = ( ( f3_t* )data )[ i ]; break;
        default: BKNIT_FA2_ERR( o->type, src_type ); break;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_value_s_zro( bhvm_value_s* o )
{
    if( o->size == 0 ) return;
    switch( BKNIT_FA1( o->type ) )
    {
        case BKNIT_F2: BFOR_EACH( i, o ) ( ( f2_t* )o->data )[ i ] = 0; break;
        case BKNIT_F3: BFOR_EACH( i, o ) ( ( f3_t* )o->data )[ i ] = 0; break;
        default: BKNIT_FA1_ERR( o->type ); break;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_value_s_set_data( bhvm_value_s* o, tp_t src_type, vc_t data, sz_t size )
{
    if( !o->type ) o->type = src_type;
    bhvm_value_s_set_size( o, size );
    bhvm_value_s_cpy_data( o, src_type, data, size );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_value_s_fork_data( bhvm_value_s* o, tp_t src_type, vd_t src_data, sz_t size )
{
    bhvm_value_s_clear( o );
    o->type = src_type;
    o->data = bcore_fork( src_data );
    o->size = size;
    o->space = size;
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_value_s_push_data( bhvm_value_s* o, tp_t src_type, vc_t data, sz_t size )
{
    if( !o->type ) o->type = src_type;
    sz_t size0 = o->size;
    bhvm_value_s_set_size( o, size0 + size );
    switch( BKNIT_FA2( o->type, src_type ) )
    {
        case BKNIT_F22: for( sz_t i = 0; i < size; i++ ) ( ( f2_t* )o->data )[ i + size0 ] = ( ( f2_t* )data )[ i ]; break;
        case BKNIT_F23: for( sz_t i = 0; i < size; i++ ) ( ( f2_t* )o->data )[ i + size0 ] = ( ( f3_t* )data )[ i ]; break;
        case BKNIT_F32: for( sz_t i = 0; i < size; i++ ) ( ( f3_t* )o->data )[ i + size0 ] = ( ( f2_t* )data )[ i ]; break;
        case BKNIT_F33: for( sz_t i = 0; i < size; i++ ) ( ( f3_t* )o->data )[ i + size0 ] = ( ( f3_t* )data )[ i ]; break;
        default: BKNIT_FA2_ERR( o->type, src_type ); break;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_value_s_push_value( bhvm_value_s* o, const bhvm_value_s* src )
{
    bhvm_value_s_push_data( o, src->type, src->data, src->size );
}

// ---------------------------------------------------------------------------------------------------------------------

bl_t bhvm_value_s_is_nan( const bhvm_value_s* o )
{
    switch( BKNIT_FA1( o->type ) )
    {
        case BKNIT_F2: BFOR_EACH( i, o ) if( f2_is_nan( ( ( f2_t* )o->data )[ i ] ) ) return false; break;
        case BKNIT_F3: BFOR_EACH( i, o ) if( f3_is_nan( ( ( f2_t* )o->data )[ i ] ) ) return false; break;
        default: BKNIT_FA1_ERR( o->type ); break;
    }
    return false;
}

// ---------------------------------------------------------------------------------------------------------------------

bl_t bhvm_value_s_is_equal( const bhvm_value_s* o, const bhvm_value_s* b )
{
    if( o->type != b->type ) return false;
    if( o->size != b->size ) return false;

    switch( BKNIT_FA1( o->type ) )
    {
        case BKNIT_F2: BFOR_EACH( i, o ) if( ( ( f2_t* )o->data )[ i ] != ( ( f2_t* )b->data )[ i ] ) return false; break;
        case BKNIT_F3: BFOR_EACH( i, o ) if( ( ( f3_t* )o->data )[ i ] != ( ( f3_t* )b->data )[ i ] ) return false; break;
        default: BKNIT_FA1_ERR( o->type ); break;
    }

    return true;
}

// ---------------------------------------------------------------------------------------------------------------------

bl_t bhvm_value_s_cat_can( const bhvm_value_s* a, const bhvm_value_s* b )
{
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------

bl_t bhvm_value_s_cat_fits( const bhvm_value_s* a, const bhvm_value_s* b, const bhvm_value_s* r )
{
    if( !bhvm_value_s_cat_can( a, b ) ) return false;
    return ( r->size == a->size + b->size );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_value_s_cat( const bhvm_value_s* a, const bhvm_value_s* b, bhvm_value_s* r )
{
    ASSERT( bhvm_value_s_cat_fits( a, b, r ) );
    if( r->size == 0 ) return;

    switch( BKNIT_FA3( a->type, b->type, r->type ) )
    {
        case BKNIT_F222: for( sz_t i = 0; i < r->size; i++ ) ( ( f2_t* )r->data )[ i ] = i < a->size ? ( ( f2_t* )a->data )[ i ] : ( ( f2_t* )b->data )[ i - a->size ]; break;
        case BKNIT_F223: for( sz_t i = 0; i < r->size; i++ ) ( ( f3_t* )r->data )[ i ] = i < a->size ? ( ( f2_t* )a->data )[ i ] : ( ( f2_t* )b->data )[ i - a->size ]; break;
        case BKNIT_F232: for( sz_t i = 0; i < r->size; i++ ) ( ( f2_t* )r->data )[ i ] = i < a->size ? ( ( f2_t* )a->data )[ i ] : ( ( f3_t* )b->data )[ i - a->size ]; break;
        case BKNIT_F233: for( sz_t i = 0; i < r->size; i++ ) ( ( f3_t* )r->data )[ i ] = i < a->size ? ( ( f2_t* )a->data )[ i ] : ( ( f3_t* )b->data )[ i - a->size ]; break;
        case BKNIT_F322: for( sz_t i = 0; i < r->size; i++ ) ( ( f2_t* )r->data )[ i ] = i < a->size ? ( ( f3_t* )a->data )[ i ] : ( ( f2_t* )b->data )[ i - a->size ]; break;
        case BKNIT_F323: for( sz_t i = 0; i < r->size; i++ ) ( ( f3_t* )r->data )[ i ] = i < a->size ? ( ( f3_t* )a->data )[ i ] : ( ( f2_t* )b->data )[ i - a->size ]; break;
        case BKNIT_F332: for( sz_t i = 0; i < r->size; i++ ) ( ( f2_t* )r->data )[ i ] = i < a->size ? ( ( f3_t* )a->data )[ i ] : ( ( f3_t* )b->data )[ i - a->size ]; break;
        case BKNIT_F333: for( sz_t i = 0; i < r->size; i++ ) ( ( f3_t* )r->data )[ i ] = i < a->size ? ( ( f3_t* )a->data )[ i ] : ( ( f3_t* )b->data )[ i - a->size ]; break;
        default: BKNIT_FA3_ERR( a->type, b->type, r->type ); break;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_value_s_cat_set( const bhvm_value_s* a, const bhvm_value_s* b, bhvm_value_s* r )
{
    if( a == r || b == r )
    {
        bhvm_value_s* buf = bhvm_value_s_create();
        bhvm_value_s_cat_set( a, b, buf );
        bhvm_value_s_copy( r, buf );
        bhvm_value_s_discard( buf );
        return;
    }

    ASSERT( bhvm_value_s_cat_can( a, b ) );

    tp_t r_type = ( a->type == TYPEOF_f2_t ) && ( b->type == TYPEOF_f2_t ) ? TYPEOF_f2_t : TYPEOF_f3_t;
    sz_t r_size = ( a->size  > 0           ) && ( b->size  > 0           ) ? a->size + b->size : 0;

    bhvm_value_s_set_type_size( r, r_type, r_size );
    if( r_size > 0 ) bhvm_value_s_cat( a, b, r );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_value_s_set_random( bhvm_value_s* o, f3_t density, f3_t min, f3_t max, u2_t* p_rval )
{
    switch( o->type )
    {
        case TYPEOF_f2_t: { bmath_vf2_s v = bhvm_value_s_get_weak_vf2( o ); bmath_vf2_s_set_random( &v, density, min, max, p_rval ); }; break;
        case TYPEOF_f3_t: { bmath_vf3_s v = bhvm_value_s_get_weak_vf3( o ); bmath_vf3_s_set_random( &v, density, min, max, p_rval ); }; break;
        default: break;
    }
}

//----------------------------------------------------------------------------------------------------------------------

f3_t bhvm_value_s_fdev_equ( const bhvm_value_s* a, const bhvm_value_s* b )
{
    ASSERT( a->size == b->size );
    f3_t sqr_sum = 0;
    switch( BKNIT_FA2( a->type, b->type ) )
    {
        case BKNIT_F22: BFOR_EACH( i, a ) sqr_sum += f3_sqr( ( ( f2_t* )a->data )[ i ] - ( ( f2_t* )b->data )[ i ] ); break;
        case BKNIT_F23: BFOR_EACH( i, a ) sqr_sum += f3_sqr( ( ( f2_t* )a->data )[ i ] - ( ( f3_t* )b->data )[ i ] ); break;
        case BKNIT_F32: BFOR_EACH( i, a ) sqr_sum += f3_sqr( ( ( f3_t* )a->data )[ i ] - ( ( f2_t* )b->data )[ i ] ); break;
        case BKNIT_F33: BFOR_EACH( i, a ) sqr_sum += f3_sqr( ( ( f3_t* )a->data )[ i ] - ( ( f3_t* )b->data )[ i ] ); break;
        default: BKNIT_FA2_ERR( a->type, b->type ); break;
    }
    return f3_srt( sqr_sum );
}

//----------------------------------------------------------------------------------------------------------------------

f3_t bhvm_value_s_fdev_zro( const bhvm_value_s* o )
{
    f3_t sqr_sum = 0;
    switch( o->type )
    {
        case TYPEOF_f2_t: BFOR_EACH( i, o ) sqr_sum += f3_sqr( ( ( f2_t* )o->data )[ i ] ); break;
        case TYPEOF_f3_t: BFOR_EACH( i, o ) sqr_sum += f3_sqr( ( ( f3_t* )o->data )[ i ] ); break;
        default: ERR_fa( "Unhandled type '#<sc_t>'", ifnameof( o->type ) );
    }
    return f3_srt( sqr_sum );
}

//----------------------------------------------------------------------------------------------------------------------

sz_t bhvm_value_s_get_sz( const bhvm_value_s* a, sz_t index )
{
    assert( index >= 0 && index < a->size );
    switch( a->type )
    {
        case TYPEOF_f2_t: return ( ( f2_t* )a->data )[ index ];
        case TYPEOF_f3_t: return ( ( f3_t* )a->data )[ index ];
        default: ERR_fa( "Invalid type" );
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

f3_t bhvm_value_s_get_f3( const bhvm_value_s* a, sz_t index )
{
    assert( index >= 0 && index < a->size );
    switch( a->type )
    {
        case TYPEOF_f2_t: return ( ( f2_t* )a->data )[ index ];
        case TYPEOF_f3_t: return ( ( f3_t* )a->data )[ index ];
        default: ERR_fa( "Invalid type" );
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

void bhvm_value_s_set_f3( const bhvm_value_s* a, sz_t index, f3_t v )
{
    assert( index >= 0 && index < a->size );
    switch( a->type )
    {
        case TYPEOF_f2_t: ( ( f2_t* )a->data )[ index ] = v; break;
        case TYPEOF_f3_t: ( ( f3_t* )a->data )[ index ] = v; break;
        default: ERR_fa( "Invalid type" );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void bhvm_value_s_order_inc( const bhvm_value_s* o, sz_t dim, bhvm_value_s* r )
{
    // safe for o == r
    assert( r->size == o->size * dim );
    switch( BKNIT_FA2( o->type, r->type ) )
    {
        case BKNIT_F22: for( sz_t i = 0; i < dim; i++ ) { f2_t* dst = ( ( f2_t* )r->data ) + i * o->size;  BFOR_EACH( i, o ) dst[ i ] = ( ( f2_t* )o->data )[ i ]; } break;
        case BKNIT_F23: for( sz_t i = 0; i < dim; i++ ) { f3_t* dst = ( ( f3_t* )r->data ) + i * o->size;  BFOR_EACH( i, o ) dst[ i ] = ( ( f2_t* )o->data )[ i ]; } break;
        case BKNIT_F32: for( sz_t i = 0; i < dim; i++ ) { f2_t* dst = ( ( f2_t* )r->data ) + i * o->size;  BFOR_EACH( i, o ) dst[ i ] = ( ( f3_t* )o->data )[ i ]; } break;
        case BKNIT_F33: for( sz_t i = 0; i < dim; i++ ) { f3_t* dst = ( ( f3_t* )r->data ) + i * o->size;  BFOR_EACH( i, o ) dst[ i ] = ( ( f3_t* )o->data )[ i ]; } break;
        default: BKNIT_FA2_ERR( o->type, r->type ); break;
    }
}

//----------------------------------------------------------------------------------------------------------------------

void bhvm_value_s_order_inc_set( const bhvm_value_s* o, sz_t dim, bhvm_value_s* r )
{
    if( o == r )
    {
        BLM_INIT();
        bhvm_value_s_order_inc_set( BLM_CLONE( bhvm_value_s, o), dim, r );
        BLM_RETURN();
    }
    bhvm_value_s_set_type_size( r, o->type, o->size * dim );
    bhvm_value_s_order_inc( o, dim, r );
}

//----------------------------------------------------------------------------------------------------------------------

void bhvm_value_s_order_dec( const bhvm_value_s* o, sz_t dim, sz_t idx, bhvm_value_s* r )
{
    // safe for o == r
    assert( o->size == r->size * dim );
    assert( idx >= 0 && idx < dim );

    sz_t offs = ( o->size / dim ) * idx;
    switch( BKNIT_FA2( o->type, r->type ) )
    {
        case BKNIT_F22: BFOR_EACH( i, r ) ( ( f2_t* )r->data )[ i ] = ( ( f2_t* )o->data )[ i + offs ]; break;
        case BKNIT_F23: BFOR_EACH( i, r ) ( ( f3_t* )r->data )[ i ] = ( ( f2_t* )o->data )[ i + offs ]; break;
        case BKNIT_F32: BFOR_EACH( i, r ) ( ( f2_t* )r->data )[ i ] = ( ( f3_t* )o->data )[ i + offs ]; break;
        case BKNIT_F33: BFOR_EACH( i, r ) ( ( f3_t* )r->data )[ i ] = ( ( f3_t* )o->data )[ i + offs ]; break;
        default: BKNIT_FA2_ERR( o->type, r->type ); break;
    }
}

//----------------------------------------------------------------------------------------------------------------------

void bhvm_value_s_order_dec_set( const bhvm_value_s* o, sz_t dim, sz_t idx, bhvm_value_s* r )
{
    if( o == r )
    {
        BLM_INIT();
        bhvm_value_s_order_dec_set( BLM_CLONE( bhvm_value_s, o), dim, idx, r );
        BLM_RETURN();
    }

    bhvm_value_s_set_type_size( r, o->type, o->size / dim );
    bhvm_value_s_order_dec( o, dim, idx, r );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// holor

// ---------------------------------------------------------------------------------------------------------------------

bl_t bhvm_holor_s_is_consistent( const bhvm_holor_s* o )
{
    if( !bhvm_shape_s_is_consistent( &o->s ) ) return false;
    if( !bhvm_value_s_is_consistent( &o->v ) ) return false;
    sz_t volume = bhvm_shape_s_get_volume( &o->s );
    if( o->v.size != 0 && o->v.size != volume ) return false;
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_holor_s_check_integrity( const bhvm_holor_s* o )
{
    bhvm_shape_s_check_integrity( &o->s );
    bhvm_value_s_check_integrity( &o->v );
    sz_t volume = bhvm_shape_s_get_volume( &o->s );
    if( o->v.size != 0 && o->v.size != volume ) ERR_fa( "( volume == #<sz_t> ) != ( v.size == #<sz_t> )", volume, o->v.size );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_holor_s_copy_typed( bhvm_holor_s* o, tp_t type, vc_t src )
{
    switch( type )
    {
        case TYPEOF_bhvm_holor_s:
        {
            assert( *(aware_t*)src == TYPEOF_bhvm_holor_s );
            bhvm_holor_s_copy( o, src );
        }
        break;

        case TYPEOF_bmath_mf2_s:
        {
            assert( *(aware_t*)src == TYPEOF_bmath_mf2_s );
            const bmath_mf2_s* m = src;
            bhvm_shape_s_set_data_na( &o->s, 2, m->cols, m->rows );
            bhvm_value_s_set_type_data( &o->v, TYPEOF_f2_t, TYPEOF_f2_t, m->data, m->cols * m->rows );
        }
        break;

        case TYPEOF_bmath_mf3_s:
        {
            assert( *(aware_t*)src == TYPEOF_bmath_mf3_s );
            const bmath_mf3_s* m = src;
            bhvm_shape_s_set_data_na( &o->s, 2, m->cols, m->rows );
            bhvm_value_s_set_type_data( &o->v, TYPEOF_f3_t, TYPEOF_f2_t, m->data, m->cols * m->rows );
        }
        break;

        case TYPEOF_bmath_vf2_s:
        {
            assert( *(aware_t*)src == TYPEOF_bmath_vf2_s );
            const bmath_vf2_s* v = src;
            bhvm_shape_s_set_data_na( &o->s, 1, v->size );
            bhvm_value_s_set_type_data( &o->v, TYPEOF_f2_t, TYPEOF_f2_t, v->data, v->size );
        }
        break;

        case TYPEOF_bmath_vf3_s:
        {
            assert( *(aware_t*)src == TYPEOF_bmath_vf3_s );
            const bmath_vf3_s* v = src;
            bhvm_shape_s_set_data_na( &o->s, 1, v->size );
            bhvm_value_s_set_type_data( &o->v, TYPEOF_f3_t, TYPEOF_f2_t, v->data, v->size );
        }
        break;

        case TYPEOF_f2_t:
        {
            bhvm_holor_s_set_type_scalar( o, TYPEOF_f2_t, *( const f2_t* )src );
        }
        break;

        case TYPEOF_f3_t:
        {
            bhvm_holor_s_set_type_scalar( o, TYPEOF_f3_t, *( const f3_t* )src );
        }
        break;

        case TYPEOF_st_s:
        {
            assert( *(aware_t*)src == TYPEOF_st_s );
            bcore_source* source = ( bcore_source* )bcore_source_string_s_create_from_string( src );
            bhvm_holor_s_parse( o, source );
            bcore_source_a_discard( source );
        }
        break;

        case TYPEOF_sc_t:
        {
            bcore_source* source = ( bcore_source* )bcore_source_string_s_create_sc( src );
            bhvm_holor_s_parse( o, source );
            bcore_source_a_discard( source );
        }
        break;

        default:
        {
            ERR_fa( "Cannot convert '#<sc_t>' to 'bhvm_holor_s'", ifnameof( type ) );
        }
        break;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

bhvm_holor_s* bhvm_holor_s_copy_t( bhvm_holor_s* o, tp_t type, vc_t src )
{
    bhvm_holor_s_copy_typed( o, type, src );
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bhvm_holor_s* bhvm_holor_s_copy_shape( bhvm_holor_s* o, const bhvm_shape_s* src )
{
    bhvm_shape_s_copy( &o->s, src );
    bhvm_value_s_clear( &o->v );
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bhvm_holor_s* bhvm_holor_s_copy_shape_type( bhvm_holor_s* o, const bhvm_holor_s* src )
{
    bhvm_holor_s_copy_shape( o, &src->s );
    bhvm_value_s_set_type( &o->v, src->v.type );
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bhvm_holor_s* bhvm_holor_s_set_type( bhvm_holor_s* o, tp_t type )
{
    bhvm_value_s_set_type( &o->v, type );
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bhvm_holor_s* bhvm_holor_s_fit_size( bhvm_holor_s* o )
{
    bhvm_value_s_set_type_size( &o->v, o->v.type ? o->v.type : TYPEOF_f3_t, bhvm_shape_s_get_volume( &o->s ) );
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bhvm_holor_s* bhvm_holor_s_fit_type_size( bhvm_holor_s* o, tp_t t )
{
    bhvm_value_s_set_type_size( &o->v, t, bhvm_shape_s_get_volume( &o->s ) ); return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bhvm_holor_s* bhvm_holor_s_set_type_scalar_vacant( bhvm_holor_s* o, tp_t t )
{
    bhvm_shape_s_set_scalar_vacant( &o->s );
    bhvm_value_s_clear( &o->v );
    bhvm_value_s_set_type( &o->v, t );
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_holor_s_set_type_scalar_pf( bhvm_holor_s* o, tp_t t, tp_t t_src, vc_t v )
{
    bhvm_shape_s_set_scalar_vacant( &o->s );
    if( v )
    {
        bhvm_value_s_set_type_size( &o->v, t, 1 );
        switch( BKNIT_FA2( t, t_src ) )
        {
            case BKNIT_F22: *( f2_t* )o->v.data = *( f2_t* )v; break;
            case BKNIT_F23: *( f2_t* )o->v.data = *( f3_t* )v; break;
            case BKNIT_F32: *( f3_t* )o->v.data = *( f2_t* )v; break;
            case BKNIT_F33: *( f3_t* )o->v.data = *( f3_t* )v; break;
            default: BKNIT_FA2_ERR( t, t_src ); break;
        }
    }
    else
    {
        bhvm_value_s_set_type( &o->v, t );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_holor_s_set_scalar_pf( bhvm_holor_s* o, tp_t t_src, vc_t v )
{
    bhvm_holor_s_set_type_scalar_pf( o, o->v.type ? o->v.type : t_src, t_src, v );
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// holor specific operations

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_holor_s_inc_order( bhvm_holor_s* o, sz_t dim )
{
    ASSERT( dim > 0 );
    bhvm_shape_s_inc_order( &o->s, dim );
    if( o->v.size > 0 && dim > 1 )
    {
        BLM_INIT();
        bhvm_value_s_make_strong( &o->v );
        bhvm_value_s* v = BLM_CREATE( bhvm_value_s );
        bhvm_value_s_fork( v, &o->v );
        bhvm_value_s_clear( &o->v );
        bhvm_holor_s_fit_size( o );
        ASSERT( o->v.size == v->size * dim );
        sz_t u_size = SIZEOF_FX( o->v.type );
        for( sz_t i = 0; i < dim; i++ ) bcore_u_memcpy( u_size, ( u0_t* )o->v.data + v->size * u_size * i, v->data, v->size );
        BLM_DOWN();
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_holor_s_inc_order_prepend( bhvm_holor_s* o, sz_t dim )
{
    ASSERT( dim > 0 );
    bhvm_shape_s_inc_order_prepend( &o->s, dim );
    if( o->v.size > 0 && dim > 1 )
    {
        BLM_INIT();
        bhvm_value_s_make_strong( &o->v );
        bhvm_value_s* v = BLM_CREATE( bhvm_value_s );
        bhvm_value_s_fork( v, &o->v );
        bhvm_value_s_clear( &o->v );
        bhvm_holor_s_fit_size( o );
        ASSERT( o->v.size == v->size * dim );
        sz_t u_size = SIZEOF_FX( o->v.type );
        for( sz_t i = 0; i < v->size; i++ )
        {
            u0_t* dst = ( u0_t* )o->v.data + i * u_size * dim;
            u0_t* src = ( u0_t* )v->data   + i * u_size;
            for( sz_t j = 0; j < dim; i++ ) bcore_u_memcpy( u_size, dst + u_size * j, src, 1 );
        }
        BLM_DOWN();
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_holor_s_push( bhvm_holor_s* o, const bhvm_holor_s* src )
{
    ASSERT( ( src->s.size == o->s.size     ) ||
            ( src->s.size == o->s.size - 1 ) );
    for( sz_t i = 0; i < src->s.size; i++ ) ASSERT( o->s.data[ i ] == src->s.data[ i ] );
    if( src->s.size == o->s.size ) bhvm_holor_s_inc_order( o, 1 );
    o->s.data[ o->s.size - 1 ]++;

    if( o->v.size != 0 )
    {
        ASSERT( src->v.size != 0 );
        bhvm_value_s_push_value( &o->v, &src->v );
    }
    else
    {
        ASSERT( src->v.size == 0 );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

bl_t bhvm_holor_s_cat_can( const bhvm_holor_s* a, const bhvm_holor_s* b )
{
    if( !bhvm_shape_s_cat_can( &a->s, &b->s ) ) return false;
    if( a->v.size > 0 && b->v.size > 0 )
    {
        if( !bhvm_value_s_cat_can( &a->v, &b->v ) ) return false;
    }
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_holor_s_cat( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    ASSERT( bhvm_shape_s_is_cat( &a->s, &b->s, &r->s ) );
    if( a->v.size > 0 && b->v.size > 0 )
    {
        ASSERT( bhvm_value_s_cat_fits( &a->v, &b->v, &r->v ) );
        bhvm_value_s_cat( &a->v, &b->v, &r->v );
    }
    else
    {
        ASSERT( r->v.size == 0 );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_holor_s_cat_set( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    bhvm_shape_s_cat_set( &a->s, &b->s, &r->s );
    bhvm_value_s_cat_set( &a->v, &b->v, &r->v );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_holor_s_order_inc_set( const bhvm_holor_s* o, sz_t dim, bhvm_holor_s* r )
{
    if( o == r )
    {
        BLM_INIT();
        bhvm_holor_s_order_inc_set( BLM_CLONE( bhvm_holor_s, o ), dim, r );
        BLM_RETURN();
    }

    bhvm_shape_s_copy( &r->s, &o->s );
    *bhvm_shape_s_push( &r->s ) = dim;

    if( o->v.size > 0 )
    {
        bhvm_value_s_order_inc_set( &o->v, dim, &r->v );
    }
    else
    {
        bhvm_value_s_clear( &r->v );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_holor_s_order_inc( const bhvm_holor_s* o, sz_t dim, bhvm_holor_s* r )
{
    assert( o->s.size + 1 == r->s.size );
    assert( r->s.data[ r->s.size - 1 ] == dim );
    #ifdef RTCHECKS
        BFOR_EACH( i, &o->s ) ASSERT( o->s.data[ i ] == r->s.data[ i ] );
    #endif

    if( o->v.size > 0 )
    {
        bhvm_value_s_order_inc( &o->v, dim, &r->v );
    }
    else
    {
        assert( r->v.size == 0 );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_holor_s_order_dec_set(  const bhvm_holor_s* o, sz_t idx, bhvm_holor_s* r )
{
    if( o == r )
    {
        BLM_INIT();
        bhvm_holor_s_order_dec_set( BLM_CLONE( bhvm_holor_s, o ), idx, r );
        BLM_RETURN();
    }

    assert( o->s.size > 0 );
    sz_t dim = o->s.data[ o->s.size - 1 ];
    assert( idx >= 0 && idx < dim );

    bhvm_shape_s_copy( &r->s, &o->s );
    r->s.size--;

    if( o->v.size > 0 )
    {
        bhvm_value_s_order_dec_set( &o->v, dim, idx, &r->v );
    }
    else
    {
        bhvm_value_s_clear( &r->v );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_holor_s_order_dec( const bhvm_holor_s* o, sz_t idx, bhvm_holor_s* r )
{
    assert( o->s.size == r->s.size + 1 );
    #ifdef RTCHECKS
        BFOR_EACH( i, &r->s ) ASSERT( o->s.data[ i ] == r->s.data[ i ] );
    #endif
    sz_t dim = o->s.data[ o->s.size - 1 ];
    assert( idx >= 0 && idx < dim );

    if( o->v.size > 0 )
    {
        bhvm_value_s_order_dec( &o->v, dim, idx, &r->v );
    }
    else
    {
        assert( r->v.size == 0 );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_holor_s_order_dec_fork( bhvm_holor_s* o, sz_t idx, bhvm_holor_s* r )
{
    assert( o != r );
    assert( o->s.size > 0 );
    sz_t dim = o->s.data[ o->s.size - 1 ];
    assert( idx >= 0 && idx < dim );

    bhvm_holor_s_fork( r, o );
    r->s.size--;

    if( o->v.size > 0 )
    {
        sz_t offs = idx * dim;
        switch( BKNIT_FA1( r->v.type ) )
        {
            case BKNIT_F2: r->v.data = ( ( f2_t* )r->v.data ) + offs; break;
            case BKNIT_F3: r->v.data = ( ( f3_t* )r->v.data ) + offs; break;
            default: BKNIT_FA1_ERR( r->v.type );
        }

        r->v.size /= dim;
        r->v.space = o->v.size;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

static void to_sink_recursive( const bhvm_holor_s* o, sz_t indent, bl_t formatted, bcore_sink* sink, vc_t v_data, sz_t d_idx )
{
    if( d_idx == 0 )
    {
        for( sz_t i = 0; i < o->s.data[ 0 ]; i++ )
        {
            if( i > 0 ) bcore_sink_a_push_fa( sink, ":" );
            switch( o->v.type )
            {
                case TYPEOF_f2_t: bcore_sink_a_push_fa( sink, "#<f2_t>", ( ( f2_t* )v_data )[ i ] ); break;
                case TYPEOF_f3_t: bcore_sink_a_push_fa( sink, "#<f3_t>", ( ( f3_t* )v_data )[ i ] ); break;
                default: break;
            }
        }
    }
    else
    {
        sz_t sub_volume = 1;
        for( sz_t i = 0; i < d_idx; i++ ) sub_volume *= o->s.data[ i ];
        sz_t dim = o->s.data[ d_idx ];
        sz_t sub_dim = o->s.data[ d_idx - 1 ];
        for( sz_t i = 0; i < dim; i++ )
        {
            if( formatted && d_idx > 0 ) bcore_sink_a_push_fa( sink, "\n#rn{ }", indent );
            bcore_sink_a_push_fa( sink, ( sub_dim == 1 ) ? "1[" : "(" );
            switch( o->v.type )
            {
                case TYPEOF_f2_t: to_sink_recursive( o, indent + 2, formatted, sink, ( f2_t* )v_data + i * sub_volume, d_idx - 1 ); break;
                case TYPEOF_f3_t: to_sink_recursive( o, indent + 2, formatted, sink, ( f3_t* )v_data + i * sub_volume, d_idx - 1 ); break;
                default: break;
            }
            if( formatted && d_idx > 1 ) bcore_sink_a_push_fa( sink, "\n#rn{ }", indent );
            if( ( sub_dim > 1 ) ) bcore_sink_a_push_fa( sink, ")" );
            if( i < dim - 1 ) bcore_sink_a_push_fa( sink, ":" );
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

static void holor_s_to_sink( const bhvm_holor_s* o, sz_t max_size, bcore_sink* sink )
{
    bhvm_holor_s_check_integrity( o );
    bcore_sink_a_push_fa( sink, "<#<sc_t>>", ifnameof( o->v.type ) );
    if( o->v.size > 0 && ( max_size < 0 || o->v.size <= max_size ) )
    {
        if( o->s.size > 0 )
        {
            sz_t sub_dim = o->s.data[ o->s.size - 1 ];
            bcore_sink_a_push_fa( sink, ( sub_dim == 1 ) ? "1[" : "(" );
            to_sink_recursive( o, 0, false, sink, o->v.data, o->s.size - 1 );
            if( sub_dim > 1 ) bcore_sink_a_push_fa( sink, ")" );
        }
        else
        {
            switch( o->v.type )
            {
                case TYPEOF_f2_t: bcore_sink_a_push_fa( sink, "#<f2_t>", ( ( f2_t* )o->v.data )[ 0 ] ); break;
                case TYPEOF_f3_t: bcore_sink_a_push_fa( sink, "#<f3_t>", ( ( f3_t* )o->v.data )[ 0 ] ); break;
                default: break;
            }
        }
    }
    else
    {
        for( sz_t i = o->s.size - 1; i >= 0; i-- )
        {
            bcore_sink_a_push_fa( sink, "#<sz_t>[", o->s.data[ i ] );
        }
        if( o->v.size == 0 )
        {
            bcore_sink_a_push_fa( sink, "##" );
        }
        else
        {
            bcore_sink_a_push_fa( sink, "D" );
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_holor_s_to_sink( const bhvm_holor_s* o, bcore_sink* sink )
{
    holor_s_to_sink( o, -1, sink );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_holor_s_brief_to_sink( const bhvm_holor_s* o, bcore_sink* sink )
{
    holor_s_to_sink( o, 32, sink );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_holor_s_formatted_to_sink( const bhvm_holor_s* o, bcore_sink* sink )
{
    if( o->v.data )
    {
        if( o->s.size > 0 )
        {
            sz_t sub_dim = o->s.data[ o->s.size - 1 ];
            bcore_sink_a_push_fa( sink, ( sub_dim == 1 ) ? "1[" : "(" );
            to_sink_recursive( o, 2, true, sink, o->v.data, o->s.size - 1 );
            if( o->s.size > 1 ) bcore_sink_a_push_fa( sink, "\n" );
            if( sub_dim > 1 ) bcore_sink_a_push_fa( sink, ")\n" );
        }
        else
        {
            switch( o->v.type )
            {
                case TYPEOF_f2_t: bcore_sink_a_push_fa( sink, "#<f2_t>", ( ( f2_t* )o->v.data )[ 0 ] ); break;
                case TYPEOF_f3_t: bcore_sink_a_push_fa( sink, "#<f3_t>", ( ( f3_t* )o->v.data )[ 0 ] ); break;
                default: break;
            }
        }
    }
    else
    {
        for( sz_t i = o->s.size - 1; i >= 0; i-- )
        {
            bcore_sink_a_push_fa( sink, "[#<sz_t>]", o->s.data[ i ] );
        }
        bcore_sink_a_push_fa( sink, "##" );
        bcore_sink_a_push_fa( sink, "\n" );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_holor_s_to_sink_nl( const bhvm_holor_s* o, bcore_sink* sink )
{
    bhvm_holor_s_to_sink( o, sink );
    bcore_sink_a_push_fa( sink, "\n" );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_holor_s_to_stdout( const bhvm_holor_s* o )
{
    bhvm_holor_s_to_sink( o, BCORE_STDOUT );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_holor_s_brief_to_stdout( const bhvm_holor_s* o )
{
    bhvm_holor_s_brief_to_sink( o, BCORE_STDOUT );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_holor_s_to_stdout_nl( const bhvm_holor_s* o )
{
    bhvm_holor_s_to_sink_nl( o, BCORE_STDOUT );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_holor_s_formatted_to_stdout( const bhvm_holor_s* o )
{
    bhvm_holor_s_formatted_to_sink( o, BCORE_STDOUT );
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// parse

// ---------------------------------------------------------------------------------------------------------------------

bhvm_holor_s* bhvm_holor_s_parse( bhvm_holor_s* o, bcore_source* source )
{
    bhvm_holor_s_clear( o );

    if( bcore_source_a_parse_bl_fa( source, " #?'<f2_t>'" ) )
    {
        bhvm_holor_s_parse( o, source );
        bhvm_value_s_set_type( &o->v, TYPEOF_f2_t );
    }
    else if( bcore_source_a_parse_bl_fa( source, " #?'<f3_t>'" ) )
    {
        bhvm_holor_s_parse( o, source );
        bhvm_value_s_set_type( &o->v, TYPEOF_f3_t );
    }
    else if( bcore_source_a_parse_bl_fa( source, " #?'#'" ) ) // undetermined scalar
    {
        if( !o->v.type ) o->v.type = TYPEOF_f3_t;
    }
    else if( bcore_source_a_parse_bl_fa( source, " #?(([0]>='0'&&[0]<='9')||([0]=='-'&&([1]>='0'&&[1]<='9')))" ) )
    {
        f3_t val = 0;
        bcore_source_a_parse_fa( source, " #<f3_t*>", &val );
        bhvm_holor_s_set_type_scalar( o, o->v.type ? o->v.type : TYPEOF_f3_t, val );
    }
    else if( bcore_source_a_parse_bl_fa( source, " #?'('" ) )
    {
        bhvm_holor_s_parse( o, source );
        bcore_source_a_parse_fa( source, " )" );
    }
    else
    {
        bcore_source_a_parse_err_fa( source, "Syntax error." );
    }

    if( bcore_source_a_parse_bl_fa( source, " #?':'" ) )
    {
        bhvm_holor_s* h = bhvm_holor_s_create();
        bhvm_holor_s_parse( h, source );
        bhvm_holor_s_cat_set( o, h, o );
        bhvm_holor_s_discard( h );
    }
    else if( bcore_source_a_parse_bl_fa( source, " #?'['" ) )
    {
        if( o->s.size != 0 || o->v.size == 0 ) bcore_source_a_parse_err_fa( source, "[: lvalue must be a scalar literal." );
        sz_t dim = bhvm_value_s_get_sz( &o->v, 0 );
        if( dim <= 0 ) bcore_source_a_parse_err_fa( source, "[: lvalue must be >=1." );
        bhvm_holor_s_parse( o, source );
        bhvm_holor_s_inc_order( o, dim );
    }

    bhvm_holor_s_check_integrity( o );

    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bhvm_holor_s* bhvm_holor_s_parse_st( bhvm_holor_s* o, const st_s* st )
{
    BLM_INIT();
    bhvm_holor_s_parse( o, BLM_A_PUSH( bcore_source_string_s_create_from_string( st ) ) );
    BLM_DOWN();
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bhvm_holor_s*bhvm_holor_s_parse_sc( bhvm_holor_s* o, sc_t sc )
{
    BLM_INIT();
    bhvm_holor_s_parse( o, BLM_A_PUSH( bcore_source_string_s_create_from_string_d( st_s_create_sc( sc ) ) ) );
    BLM_DOWN();
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bhvm_holor_s* bhvm_holor_s_create_parse( bcore_source* source )
{
    return bhvm_holor_s_parse( bhvm_holor_s_create(), source );
}

// ---------------------------------------------------------------------------------------------------------------------

bhvm_holor_s* bhvm_holor_s_create_parse_st( const st_s* st )
{
    return bhvm_holor_s_parse_st( bhvm_holor_s_create(), st );
}

// ---------------------------------------------------------------------------------------------------------------------

bhvm_holor_s* bhvm_holor_s_create_parse_sc( sc_t sc )
{
    return bhvm_holor_s_parse_sc( bhvm_holor_s_create(), sc );
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

static void selftest( void )
{
    BLM_INIT();

    bhvm_holor_s* h1 = BLM_CREATE( bhvm_holor_s );
    bhvm_holor_s* h2 = BLM_CREATE( bhvm_holor_s );
    bhvm_holor_s* h3 = BLM_CREATE( bhvm_holor_s );
//    bhvm_holor_s* h2 = BLM_CREATE( bhvm_holor_s );
    st_s* s = BLM_CREATE( st_s );

    bhvm_holor_s_parse( h1, BLM_A_PUSH( bcore_source_string_s_create_sc( "<f2_t>2[(( 1:1 ):( 7:2 ))" ) ) );
    bhvm_holor_s_to_sink( h1, ( bcore_sink* )s );
    ASSERT( st_s_equal_sc( s, "<f2_t>(((1:1):(7:2)):((1:1):(7:2)))" ) );

    bhvm_holor_s_parse( h1, BLM_A_PUSH( bcore_source_string_s_create_sc( "<f2_t>( 1:2 )" ) ) );
    bhvm_holor_s_parse( h2, BLM_A_PUSH( bcore_source_string_s_create_sc( "<f3_t>( 2:2 )" ) ) );
    bhvm_holor_s_parse( h3, BLM_A_PUSH( bcore_source_string_s_create_sc( "<f2_t>2[0" ) ) );

    bhvm_hop_ar2_eci_mul_s_f( h1, h2, h3 );

    st_s_clear( s );
    bhvm_holor_s_to_sink( h3, ( bcore_sink* )s );
    ASSERT( st_s_equal_sc( s, "<f2_t>(2:4)" ) );

/*
    bhvm_holor_s_set_size_na( h1, 3, 2, 2, 7 );
    bhvm_holor_s_copy_size( h2, h1 );

    u2_t rval = 123;
    bhvm_holor_s_set_random( h1, 1.0, 0, 1, &rval );
    bhvm_holor_s_cpy( h1, h2 );

    bhvm_holor_s_mul_scl_f3_add( h1, 2, h2, h2 );

    ASSERT( f3_abs( ( bhvm_holor_s_f3_sum( h2 ) / bhvm_holor_s_f3_sum( h1 ) ) - 3.0 ) < 1E-10 );
*/
    BLM_DOWN();
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

#endif // TYPEOF_bhvm_h

vd_t bhvm_holor_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bhvm_holor" ) ) )
    {
        case TYPEOF_init1:
        {
        }
        break;

        case TYPEOF_get_quicktypes:
        {
        }
        break;

        case TYPEOF_selftest:
        {
            selftest();
        }
        break;

        case TYPEOF_plant:
        {
            bcore_plant_compile( "bhvm_planted", __FILE__ );
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

