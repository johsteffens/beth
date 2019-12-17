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

#include "bhvm_hf3.h"

#ifdef TYPEOF_bhvm_hf3

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_d_make_strong( bhvm_hf3_s* o )
{
    if( o->d_size  == 0 ) return;
    if( o->d_space != 0 ) return;
    sz_t  d_size = o->d_size;
    sz_t* d_data = o->d_data;
    o->d_size = 0;
    o->d_data = NULL;
    bhvm_hf3_s_set_d_size( o, d_size );
    bcore_u_memcpy( sizeof( sz_t ), o->d_data, d_data, d_size );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_v_make_strong( bhvm_hf3_s* o )
{
    if( o->v_size  == 0 ) return;
    if( o->v_space != 0 ) return;
    sz_t  v_size = o->v_size;
    f3_t* v_data = o->v_data;
    o->v_size = 0;
    o->v_data = NULL;
    bhvm_hf3_s_set_v_size( o, v_size );
    bcore_u_memcpy( sizeof( f3_t ), o->v_data, v_data, v_size );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_make_strong( bhvm_hf3_s* o )
{
    bhvm_hf3_s_d_make_strong( o );
    bhvm_hf3_s_v_make_strong( o );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_resize_d_size( bhvm_hf3_s* o, sz_t size )
{
    sz_t old_size = o->d_size;
    bhvm_hf3_s_d_make_strong( o );
    o->d_size = size;
    o->d_data = bcore_un_alloc( sizeof( sz_t ), o->d_data, o->d_space, o->d_size, &o->d_space );
    for( sz_t i = old_size; i < size; i++ ) o->d_data[ i ] = 0;
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_resize_v_size( bhvm_hf3_s* o, sz_t size )
{
    sz_t old_size = o->v_size;
    bhvm_hf3_s_v_make_strong( o );
    o->v_size = size;
    o->v_data = bcore_un_alloc( sizeof( f3_t ), o->v_data, o->v_space, o->v_size, &o->v_space );
    for( sz_t i = old_size; i < size; i++ ) o->v_data[ i ] = 0;
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_set_d_size( bhvm_hf3_s* o, sz_t size )
{
    o->d_size = size;
    if( o->d_space > 0 )
    {
        o->d_data = bcore_un_alloc( sizeof( sz_t ), o->d_data, o->d_space, o->d_size, &o->d_space );
    }
    else
    {
        o->d_data = bcore_un_alloc( sizeof( sz_t ), NULL, 0, o->d_size, &o->d_space );
    }
    bcore_u_memzero( sizeof( sz_t ), o->d_data, o->d_size );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_set_v_size( bhvm_hf3_s* o, sz_t size )
{
    o->v_size = size;
    if( o->v_space > 0 )
    {
        o->v_data = bcore_un_alloc( sizeof( f3_t ), o->v_data, o->v_space, o->v_size, &o->v_space );
    }
    else
    {
        o->v_data = bcore_un_alloc( sizeof( f3_t ), NULL, 0, o->v_size, &o->v_space );
    }
    bcore_u_memzero( sizeof( f3_t ), o->v_data, o->v_size );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_clear_v_data( bhvm_hf3_s* o )
{
    if( o->v_space > 0 )
    {
        o->v_data = bcore_un_alloc( sizeof( f3_t ), o->v_data, o->v_space, 0, &o->v_space );
    }
    o->v_data = NULL;
    o->v_size = 0;
    o->v_space = 0;
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_clear_d_data( bhvm_hf3_s* o )
{
    if( o->d_space > 0 )
    {
        o->d_data = bcore_un_alloc( sizeof( sz_t ), o->d_data, o->d_space, 0, &o->d_space );
    }
    o->d_data = NULL;
    o->d_size = 0;
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_clear( bhvm_hf3_s* o )
{
    bhvm_hf3_s_clear_d_data( o );
    bhvm_hf3_s_clear_v_data( o );
    o->htp = false;
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_fit_v_size( bhvm_hf3_s* o )
{
    sz_t size = 1;
    for( sz_t i = 0; i < o->d_size; i++ ) size *= o->d_data[ i ];
    bhvm_hf3_s_set_v_size( o, size );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_set_vacant( bhvm_hf3_s* o )
{
    bhvm_hf3_s_clear_v_data( o );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_set_d_data( bhvm_hf3_s* o, const sz_t* d_data, sz_t d_size )
{
    bhvm_hf3_s_set_d_size( o, d_size );
    for( sz_t i = 0; i < o->d_size; i++ ) o->d_data[ i ] = d_data[ i ];
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_set_v_data( bhvm_hf3_s* o, const f3_t* v_data, sz_t v_size )
{
    bhvm_hf3_s_set_v_size( o, v_size );
    for( sz_t i = 0; i < o->v_size; i++ ) o->v_data[ i ] = v_data[ i ];
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_copy_typed( bhvm_hf3_s* o, tp_t type, vc_t src )
{
    switch( type )
    {
        case TYPEOF_bhvm_hf3_s:
        {
            assert( *(aware_t*)src == TYPEOF_bhvm_hf3_s );
            bhvm_hf3_s_copy( o, src );
        }
        break;

        case TYPEOF_bmath_mf3_s:
        {
            assert( *(aware_t*)src == TYPEOF_bmath_mf3_s );
            const bmath_mf3_s* m = src;
            bhvm_hf3_s_set_d_data_na( o, 2, m->cols, m->rows );
            bhvm_hf3_s_set_v_data( o, m->data, m->cols * m->rows );
            o->htp = false;
        }
        break;

        case TYPEOF_bmath_vf3_s:
        {
            assert( *(aware_t*)src == TYPEOF_bmath_vf3_s );
            const bmath_vf3_s* v = src;
            bhvm_hf3_s_set_d_data_na( o, 1, v->size );
            bhvm_hf3_s_set_v_data( o, v->data, v->size );
            o->htp = false;
        }
        break;

        case TYPEOF_f3_t:
        {
            bhvm_hf3_s_set_scalar_f3( o, *( const f3_t* )src );
            o->htp = false;
        }
        break;

        case TYPEOF_st_s:
        {
            assert( *(aware_t*)src == TYPEOF_st_s );
            bcore_source* source = ( bcore_source* )bcore_source_string_s_create_from_string( src );
            bhvm_hf3_s_parse( o, source );
            bcore_source_a_discard( source );
        }
        break;

        case TYPEOF_sc_t:
        {
            bcore_source* source = ( bcore_source* )bcore_source_string_s_create_sc( src );
            bhvm_hf3_s_parse( o, source );
            bcore_source_a_discard( source );
        }
        break;

        default:
        {
            ERR_fa( "Cannot convert '#<sc_t>' to 'bhvm_hf3_s'", ifnameof( type ) );
        }
        break;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_copy_d_data( bhvm_hf3_s* o, const bhvm_hf3_s* src )
{
    bhvm_hf3_s_set_d_data( o, src->d_data, src->d_size );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_copy_v_data( bhvm_hf3_s* o, const bhvm_hf3_s* src )
{
    bhvm_hf3_s_set_v_data( o, src->v_data, src->v_size );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_copy_v_data_from_vf3( bhvm_hf3_s* o, const bmath_vf3_s* src )
{
    bhvm_hf3_s_set_v_data( o, src->data, src->size );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_copy_v_data_to_vf3( const bhvm_hf3_s* o, bmath_vf3_s* dst )
{
    bmath_vf3_s_set_data( dst, o->v_data, o->v_size );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_set_size( bhvm_hf3_s* o, const sz_t* d_data, sz_t d_size )
{
    bhvm_hf3_s_set_d_data( o, d_data, d_size );
    bhvm_hf3_s_fit_v_size( o );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_copy_size( bhvm_hf3_s* o, const bhvm_hf3_s* src )
{
    bhvm_hf3_s_set_d_data( o, src->d_data, src->d_size );
    if( src->v_size )
    {
        bhvm_hf3_s_fit_v_size( o );
    }
    else
    {
        bhvm_hf3_s_clear_v_data( o );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_set_d_data_nv( bhvm_hf3_s* o, sz_t d_size, va_list sz_t_args )
{
    bhvm_hf3_s_set_d_size( o, d_size );
    for( uz_t i = 0; i < d_size; i++ )
    {
        o->d_data[ i ] = va_arg( sz_t_args, sz_t );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_set_d_data_na( bhvm_hf3_s* o, sz_t d_size, ... )
{
    va_list args;
    va_start( args, d_size );
    bhvm_hf3_s_set_d_data_nv( o, d_size, args );
    va_end( args );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_set_v_data_nv( bhvm_hf3_s* o, sz_t v_size, va_list sz_t_args )
{
    bhvm_hf3_s_set_v_size( o, v_size );
    for( uz_t i = 0; i < v_size; i++ )
    {
        o->v_data[ i ] = va_arg( sz_t_args, sz_t );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_set_v_data_na( bhvm_hf3_s* o, sz_t v_size, ... )
{
    va_list args;
    va_start( args, v_size );
    bhvm_hf3_s_set_v_data_nv( o, v_size, args );
    va_end( args );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_set_size_nv( bhvm_hf3_s* o, sz_t d_size, va_list sz_t_args )
{
    bhvm_hf3_s_set_d_data_nv( o, d_size, sz_t_args );
    bhvm_hf3_s_fit_v_size( o );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_set_size_na( bhvm_hf3_s* o, sz_t d_size, ... )
{
    va_list args;
    va_start( args, d_size );
    bhvm_hf3_s_set_size_nv( o, d_size, args );
    va_end( args );
}

// ---------------------------------------------------------------------------------------------------------------------

sz_t bhvm_hf3_s_d_product( const bhvm_hf3_s* o )
{
    sz_t prd = 1;
    for( sz_t i = 0; i < o->d_size; i++ ) prd *= o->d_data[ i ];
    return prd;
}

// ---------------------------------------------------------------------------------------------------------------------

sz_t bhvm_hf3_s_volume( const bhvm_hf3_s* o )
{
    if( o->v_size > 0 )
    {
        return o->v_size;
    }
    else
    {
        return bhvm_hf3_s_d_product( o );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_set_d_scalar( bhvm_hf3_s* o )
{
    bhvm_hf3_s_set_d_data( o, NULL, 0 );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_set_scalar_pf3( bhvm_hf3_s* o, f3_t* v )
{
    bhvm_hf3_s_set_d_scalar( o );
    if( v )
    {
        bhvm_hf3_s_fit_v_size( o );
        o->v_data[ 0 ] = *v;
    }
    else
    {
        bhvm_hf3_s_clear_v_data( o );
    }
    o->htp = false;
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_set_scalar_f3( bhvm_hf3_s* o, f3_t v )
{
    bhvm_hf3_s_set_scalar_pf3( o, &v );
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// parse

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_parse( bhvm_hf3_s* o, bcore_source* source )
{
    bhvm_hf3_s_clear( o );

    if( bcore_source_a_parse_bl_fa( source, " #?'#'" ) ) // undetermined scalar
    {
        /// nothing to do
    }
    else if( bcore_source_a_parse_bl_fa( source, " #?(([0]>='0'&&[0]<='9')||([0]=='-'&&([1]>='0'&&[1]<='9')))" ) )
    {
        f3_t val = 0;
        bcore_source_a_parse_fa( source, " #<f3_t*>", &val );
        bhvm_hf3_s_set_scalar_f3( o, val );
    }
    else if( bcore_source_a_parse_bl_fa( source, " #?'['" ) )
    {
        sz_t dim = -1;
        bcore_source_a_parse_fa( source, " #<sz_t*>", &dim );
        if( dim <= 0 ) bcore_source_a_parse_err_fa( source, "Incorrect dimension value '#<sz_t>' or syntax error.", dim );
        bcore_source_a_parse_fa( source, " ]" );

        bhvm_hf3_s_parse( o, source );
        bhvm_hf3_s_inc_order( o, dim );
    }
    else if( bcore_source_a_parse_bl_fa( source, " #?'('" ) )
    {
        bhvm_hf3_s* h = bhvm_hf3_s_create();
        while( !bcore_source_a_eos( source ) && !bcore_source_a_parse_bl_fa( source, " #=?')'" ) )
        {
            bhvm_hf3_s_parse( h, source );
            bhvm_hf3_s_push( o, h );
        }
        bcore_source_a_parse_fa( source, " )" );
        if( bcore_source_a_parse_bl_fa( source, " #?'^t'" ) ) o->htp = !o->htp;
        bhvm_hf3_s_discard( h );
    }
    else if( bcore_source_a_parse_bl_fa( source, " #?'htp'" ) )
    {
        bcore_source_a_parse_fa( source, " (" );
        bhvm_hf3_s_parse( o, source );
        bcore_source_a_parse_fa( source, " )" );
        o->htp = !o->htp;
    }
    else
    {
        bcore_source_a_parse_err_fa( source, "Syntax error." );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// status

// ---------------------------------------------------------------------------------------------------------------------

bl_t bhvm_hf3_s_d_equal( const bhvm_hf3_s* o, const bhvm_hf3_s* src )
{
    if( o->d_size != src->d_size ) return false;
    for( sz_t i = 0; i < o->d_size; i++ ) if( o->d_data[ i ] != src->d_data[ i ] ) return false;
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------

bl_t bhvm_hf3_s_v_equal( const bhvm_hf3_s* o, const bhvm_hf3_s* src )
{
    if( o->v_size != src->v_size ) return false;
    for( sz_t i = 0; i < o->v_size; i++ ) if( o->v_data[ i ] != src->v_data[ i ] ) return false;
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------

bl_t bhvm_hf3_s_shape_equal( const bhvm_hf3_s* o, const bhvm_hf3_s* src )
{
    if( o->htp != src->htp ) return false;
    if( !bhvm_hf3_s_d_equal( o, src ) ) return false;
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------

bl_t bhvm_hf3_s_is_equal( const bhvm_hf3_s* o, const bhvm_hf3_s* op )
{
    if( o->htp    != op->htp        ) return false;
    if( !bhvm_hf3_s_d_equal( o, op ) ) return false;
    if( !bhvm_hf3_s_v_equal( o, op ) ) return false;
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------

bl_t bhvm_hf3_s_is_scalar( const bhvm_hf3_s* o )
{
    return o->d_size == 0;
}

// ---------------------------------------------------------------------------------------------------------------------

bl_t bhvm_hf3_s_is_nan( const bhvm_hf3_s* o )
{
    for( sz_t i = 0; i < o->v_size; i++ ) if( f3_is_nan( o->v_data[ i ] ) ) return true;
    return false;
}

// ---------------------------------------------------------------------------------------------------------------------

bl_t bhvm_hf3_s_is_consistent( const bhvm_hf3_s* o )
{
    if( ( o->d_data == NULL ) && ( o->d_size != 0 ) ) return false;
    if( ( o->v_data == NULL ) && ( o->v_size != 0 ) ) return false;
    sz_t d_product = bhvm_hf3_s_d_product( o );
    if(  d_product < 0 ) return false;
    if( o->v_size != 0 && o->v_size != d_product ) return false;
    if( bhvm_hf3_s_is_nan( o ) ) return false;
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_check_integrity( const bhvm_hf3_s* o )
{
    if( ( o->d_data == NULL ) && ( o->d_size != 0 ) ) ERR_fa( "d_size == #<sz_t> but d_data not allocated.", o->d_size );
    if( ( o->v_data == NULL ) && ( o->v_size != 0 ) ) ERR_fa( "v_size == #<sz_t> but v_data not allocated.", o->v_size );
    sz_t d_product = bhvm_hf3_s_d_product( o );
    if(  d_product < 0 ) ERR_fa( "d_product is negative." );
    if( o->v_size != 0 && o->v_size != d_product ) ERR_fa( "( d_product == #<sz_t> ) != ( v_size == #<sz_t> )", d_product, o->v_size );
    if( bhvm_hf3_s_is_nan( o ) ) ERR_fa( "Holor is NAN." );
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_inc_order( bhvm_hf3_s* o, sz_t dim )
{
    bhvm_hf3_s_d_make_strong( o );

    o->d_size++;
    o->d_data = bcore_un_alloc( sizeof( sz_t ), o->d_data, o->d_space, o->d_size, &o->d_space );
    o->d_data[ o->d_size - 1 ] = dim;

    if( o->v_size > 0 )
    {
        bhvm_hf3_s_v_make_strong( o );

        f3_t* v_data  = o->v_data;
        sz_t  v_size  = o->v_size;
        sz_t  v_space = o->v_space;
        o->v_data = NULL;
        o->v_size = 0;
        o->v_space = 0;
        bhvm_hf3_s_fit_v_size( o );
        ASSERT( o->v_size == v_size * dim );

        for( sz_t i = 0; i < dim; i++ )
        {
            bcore_u_memcpy( sizeof( f3_t ), o->v_data + v_size * i, v_data, v_size );
        }

        bcore_un_alloc( sizeof( f3_t ), v_data, v_space, 0, NULL );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_inc_order_prepend( bhvm_hf3_s* o, sz_t dim )
{
    bhvm_hf3_s_d_make_strong( o );

    o->d_size++;
    o->d_data = bcore_un_alloc( sizeof( sz_t ), o->d_data, o->d_space, o->d_size, &o->d_space );
    for( sz_t i = o->d_size - 1; i > 0; i-- ) o->d_data[ i ] = o->d_data[ i - 1 ];
    o->d_data[ 0 ] = dim;

    if( o->v_size > 0 )
    {
        bhvm_hf3_s_v_make_strong( o );

        f3_t* v_data  = o->v_data;
        sz_t  v_size  = o->v_size;
        sz_t  v_space = o->v_space;
        o->v_data = NULL;
        o->v_size = 0;
        o->v_space = 0;
        bhvm_hf3_s_fit_v_size( o );
        ASSERT( o->v_size == v_size * dim );

        for( sz_t i = 0; i < v_size; i++ )
        {
            f3_t* dst = o->v_data + i * dim;
            f3_t  src = v_data[ i ];
            for( sz_t j = 0; j < dim; i++ ) dst[ j ] = src;
        }

        bcore_un_alloc( sizeof( f3_t ), v_data, v_space, 0, NULL );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_push( bhvm_hf3_s* o, const bhvm_hf3_s* src )
{
    if( o->d_size == 0 && o->v_size == 0 )
    {
        bhvm_hf3_s_copy( o, src );
        bhvm_hf3_s_inc_order( o, 1 );
        return;
    }

    ASSERT( src->d_size <= o->d_size );

    for( sz_t i = 0; i < src->d_size; i++ ) ASSERT( o->d_data[ i ] == src->d_data[ i ] );
    if( src->d_size == o->d_size ) bhvm_hf3_s_inc_order( o, 1 );

    ASSERT( o->d_size > 0 );

    bhvm_hf3_s_v_make_strong( o );
    sz_t sub_size = bhvm_hf3_s_d_product( src );

    o->d_data[ o->d_size -1 ]++;

    if( o->v_data )
    {
        ASSERT( src->v_data );
        bhvm_hf3_s_resize_v_size( o, o->v_size + sub_size );
        bcore_u_memcpy( sizeof( f3_t ), o->v_data + o->v_size - sub_size, src->v_data, sub_size );
    }
    else
    {
        ASSERT( !src->v_data );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_cat( const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r )
{
    // TODO: handle transpositions
    ASSERT( !a->htp );
    ASSERT( !b->htp );
    ASSERT( !r->htp );

    ASSERT( r->v_size == a->v_size + b->v_size );
    bcore_u_memcpy( sizeof( f3_t ), r->v_data,             a->v_data, a->v_size );
    bcore_u_memcpy( sizeof( f3_t ), r->v_data + a->v_size, b->v_data, b->v_size );
}

// ---------------------------------------------------------------------------------------------------------------------

bl_t bhvm_hf3_s_set_shape_cat( const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r )
{
    // TODO: handle transpositions
    if( a->htp ) return false;
    if( b->htp ) return false;
    if( r->htp ) return false;

    if( a->d_size == b->d_size ) // a, b have equal shape
    {
        if( !bhvm_hf3_s_d_equal( a, b ) ) return false;
        bhvm_hf3_s_set_d_size( r, a->d_size + 1 );
        bcore_u_memcpy( sizeof( sz_t ), r->d_data, a->d_data, a->d_size );
        r->d_data[ r->d_size - 1 ] = 2;
    }
    else if( a->d_size == b->d_size + 1 )
    {
        for( sz_t i = 0; i < b->d_size; i++ ) if( a->d_data[ i ] != b->d_data[ i ] ) return false;
        bhvm_hf3_s_set_d_size( r, a->d_size );
        bcore_u_memcpy( sizeof( sz_t ), r->d_data, a->d_data, a->d_size );
        r->d_data[ r->d_size - 1 ] += 1;
    }
    else if( a->d_size + 1 == b->d_size )
    {
        for( sz_t i = 0; i < a->d_size; i++ ) if( a->d_data[ i ] != b->d_data[ i ] ) return false;
        bhvm_hf3_s_set_d_size( r, b->d_size );
        bcore_u_memcpy( sizeof( sz_t ), r->d_data, b->d_data, b->d_size );
        r->d_data[ r->d_size - 1 ] += 1;
    }
    else
    {
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------

static void rec_to_sink( const bhvm_hf3_s* o, sz_t indent, bl_t formatted, bcore_sink* sink, f3_t* v_data, sz_t d_idx )
{
    if( d_idx == 0 )
    {
        for( sz_t i = 0; i < o->d_data[ 0 ]; i++ )
        {
            if( i > 0 ) bcore_sink_a_push_fa( sink, " " );
            bcore_sink_a_push_fa( sink, "#<f3_t>", v_data[ i ] );
        }
    }
    else
    {
        sz_t v_block = 1;
        for( sz_t i = 0; i < d_idx; i++ ) v_block *= o->d_data[ i ];
        sz_t dim = o->d_data[ d_idx ];
        for( sz_t i = 0; i < dim; i++ )
        {
            if( formatted && d_idx > 0 ) bcore_sink_a_push_fa( sink, "\n#rn{ }", indent );
            bcore_sink_a_push_fa( sink, "(" );
            rec_to_sink( o, indent + 2, formatted, sink, v_data + i * v_block, d_idx - 1 );
            if( formatted && d_idx > 1 ) bcore_sink_a_push_fa( sink, "\n#rn{ }", indent );
            bcore_sink_a_push_fa( sink, ")" );
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

static void hf3_s_to_sink( const bhvm_hf3_s* o, sz_t max_size, bcore_sink* sink )
{
    bhvm_hf3_s_check_integrity( o );
    if( o->htp ) bcore_sink_a_push_fa( sink, "htp(" );
    if( o->v_size > 0 && ( max_size < 0 || o->v_size <= max_size ) )
    {
        if( o->d_size > 0 )
        {
            bcore_sink_a_push_fa( sink, "(" );
            rec_to_sink( o, 0, false, sink, o->v_data, o->d_size - 1 );
            bcore_sink_a_push_fa( sink, ")" );
        }
        else
        {
            bcore_sink_a_push_fa( sink, "#<f3_t>", o->v_data[ 0 ] );
        }
    }
    else
    {
        for( sz_t i = o->d_size - 1; i >= 0; i-- )
        {
            bcore_sink_a_push_fa( sink, "[#<sz_t>]", o->d_data[ i ] );
        }
        if( o->v_size == 0 )
        {
            bcore_sink_a_push_fa( sink, "##" );
        }
        else
        {
            bcore_sink_a_push_fa( sink, "D" );
        }
    }
    if( o->htp ) bcore_sink_a_push_fa( sink, ")" );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_to_sink( const bhvm_hf3_s* o, bcore_sink* sink )
{
    hf3_s_to_sink( o, -1, sink );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_brief_to_sink( const bhvm_hf3_s* o, bcore_sink* sink )
{
    hf3_s_to_sink( o, 16, sink );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_formatted_to_sink( const bhvm_hf3_s* o, bcore_sink* sink )
{
    if( o->v_data )
    {
        if( o->d_size > 0 )
        {
            bcore_sink_a_push_fa( sink, "(" );
            rec_to_sink( o, 2, true, sink, o->v_data, o->d_size - 1 );
            if( o->d_size > 1 ) bcore_sink_a_push_fa( sink, "\n" );
            bcore_sink_a_push_fa( sink, ")" );
            if( o->htp ) bcore_sink_a_push_fa( sink, "^t" );
            bcore_sink_a_push_fa( sink, "\n" );
        }
        else
        {
            bcore_sink_a_push_fa( sink, "#<f3_t>\n", o->v_data[ 0 ] );
        }
    }
    else
    {
        if( o->htp ) bcore_sink_a_push_fa( sink, "htp(" );
        for( sz_t i = o->d_size - 1; i >= 0; i-- )
        {
            bcore_sink_a_push_fa( sink, "[#<sz_t>]", o->d_data[ i ] );
        }
        bcore_sink_a_push_fa( sink, "##" );
        if( o->htp ) bcore_sink_a_push_fa( sink, ")" );
        bcore_sink_a_push_fa( sink, "\n" );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_to_sink_nl( const bhvm_hf3_s* o, bcore_sink* sink )
{
    bhvm_hf3_s_to_sink( o, sink );
    bcore_sink_a_push_fa( sink, "\n" );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_to_stdout( const bhvm_hf3_s* o )
{
    bhvm_hf3_s_to_sink( o, BCORE_STDOUT );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_brief_to_stdout( const bhvm_hf3_s* o )
{
    bhvm_hf3_s_brief_to_sink( o, BCORE_STDOUT );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_to_stdout_nl( const bhvm_hf3_s* o )
{
    bhvm_hf3_s_to_sink_nl( o, BCORE_STDOUT );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_formatted_to_stdout( const bhvm_hf3_s* o )
{
    bhvm_hf3_s_formatted_to_sink( o, BCORE_STDOUT );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_set_random( bhvm_hf3_s* o, f3_t density, f3_t min, f3_t max, u2_t* p_rval )
{
    u2_t rval = p_rval ? *p_rval : 12345;
    f3_t range = max - min;
    for( uz_t i = 0; i < o->v_size; i++ )
    {
        if( f3_xsg1_pos( &rval ) < density )
        {
            o->v_data[ i ] = ( range * f3_xsg1_pos( &rval ) ) + min;
        }
        else
        {
            o->v_data[ i ] = 0;
        }
    }
    if( p_rval ) *p_rval = rval;
}

//----------------------------------------------------------------------------------------------------------------------

f3_t bhvm_hf3_s_fdev_equ( const bhvm_hf3_s* o, const bhvm_hf3_s* op )
{
    ASSERT( o->htp == op->htp );
    ASSERT( o->v_size == op->v_size );
    f3_t sum = 0;
    const f3_t* a = o ->v_data;
    const f3_t* b = op->v_data;
    for( sz_t i = 0; i < o->v_size; i++ ) sum += f3_sqr( a[ i ] - b[ i ] );
    return ( sum > 0 ) ? f3_srt( sum ) : 0;
}

//----------------------------------------------------------------------------------------------------------------------

f3_t bhvm_hf3_s_fdev_zro( const bhvm_hf3_s* o )
{
    f3_t sum = 0;
    const f3_t* a = o ->v_data;
    for( sz_t i = 0; i < o->v_size; i++ ) sum += f3_sqr( a[ i ] );
    return ( sum > 0 ) ? f3_srt( sum ) : 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// elementwise operations

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_adl_s_zro( const bhvm_hf3_adl_s* o )
{
    for( sz_t i = 0; i < o->size; i++ ) bhvm_hf3_s_zro( o->data[ i ] );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_cpy( const bhvm_hf3_s* o, bhvm_hf3_s* r )
{
    ASSERT( o->v_size == r->v_size );
    for( sz_t i = 0; i < o->v_size; i++ ) r->v_data[ i ] = o->v_data[ i ];
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_adl_s_cpy( const bhvm_hf3_adl_s* o, bhvm_hf3_adl_s* r )
{
    ASSERT( o->size == r->size );
    for( sz_t i = 0; i < o->size; i++ ) bhvm_hf3_s_cpy( o->data[ i ], r->data[ i ] );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_exp( const bhvm_hf3_s* o, bhvm_hf3_s* r )
{
    ASSERT( o->v_size == r->v_size );
    for( sz_t i = 0; i < o->v_size; i++ ) r->v_data[ i ] = exp( o->v_data[ i ] );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_htp( const bhvm_hf3_s* o, bhvm_hf3_s* r )
{
    bmath_mf3_s mo = bhvm_hf3_s_get_weak_mat( o );
    bmath_mf3_s mr = bhvm_hf3_s_get_weak_mat( o );
    bmath_mf3_s_htp( &mo, &mr );
    r->htp = o->htp;
}

// ---------------------------------------------------------------------------------------------------------------------

/// o + m -> r
void bhvm_hf3_s_add( const bhvm_hf3_s* o, const bhvm_hf3_s* m, bhvm_hf3_s* r )
{
    ASSERT( o->v_size == m->v_size );
    ASSERT( o->v_size == r->v_size );
    for( sz_t i = 0; i < o->v_size; i++ ) r->v_data[ i ] = o->v_data[ i ] + m->v_data[ i ];
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_adl_s_add( const bhvm_hf3_adl_s* o, const bhvm_hf3_adl_s* m, bhvm_hf3_adl_s* r )
{
    ASSERT( o->size == m->size );
    ASSERT( o->size == r->size );
    for( sz_t i = 0; i < o->size; i++ ) bhvm_hf3_s_add( o->data[ i ], m->data[ i ], r->data[ i ] );
}

// ---------------------------------------------------------------------------------------------------------------------

/// o - m -> r
void bhvm_hf3_s_sub( const bhvm_hf3_s* o, const bhvm_hf3_s* m, bhvm_hf3_s* r )
{
    ASSERT( o->v_size == m->v_size );
    ASSERT( o->v_size == r->v_size );
    for( sz_t i = 0; i < o->v_size; i++ ) r->v_data[ i ] = o->v_data[ i ] - m->v_data[ i ];
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_adl_s_sub( const bhvm_hf3_adl_s* o, const bhvm_hf3_adl_s* m, bhvm_hf3_adl_s* r )
{
    ASSERT( o->size == m->size );
    ASSERT( o->size == r->size );
    for( sz_t i = 0; i < o->size; i++ ) bhvm_hf3_s_sub( o->data[ i ], m->data[ i ], r->data[ i ] );
}

// ---------------------------------------------------------------------------------------------------------------------

f3_t bhvm_hf3_s_f3_sub_sqr( const bhvm_hf3_s* o, const bhvm_hf3_s* m )
{
    ASSERT( o->v_size == m->v_size );
    f3_t sum = 0;
    for( sz_t i = 0; i < o->v_size; i++ ) sum += f3_sqr( o->v_data[ i ] - m->v_data[ i ] );
    return sum;
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_sub_sqr( const bhvm_hf3_s* o, const bhvm_hf3_s* m, f3_t* r )
{
    ASSERT( r );
    *r = bhvm_hf3_s_f3_sub_sqr( o, m );
}

// ---------------------------------------------------------------------------------------------------------------------

f3_t bhvm_hf3_adl_s_f3_sub_sqr( const bhvm_hf3_adl_s* o, const bhvm_hf3_adl_s* m )
{
    ASSERT( o->size == m->size );
    f3_t sum = 0;
    for( sz_t i = 0; i < o->size; i++ ) sum += bhvm_hf3_s_f3_sub_sqr( o->data[ i ], m->data[ i ] );
    return sum;
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_adl_s_sub_sqr( const bhvm_hf3_adl_s* o, const bhvm_hf3_adl_s* m, f3_t* r )
{
    ASSERT( r );
    *r = bhvm_hf3_adl_s_f3_sub_sqr( o, m );
}

// ---------------------------------------------------------------------------------------------------------------------

/// o <*> m -> r (hadamard product)
void bhvm_hf3_s_hmul( const bhvm_hf3_s* o, const bhvm_hf3_s* m, bhvm_hf3_s* r )
{
    ASSERT( o->v_size == m->v_size );
    ASSERT( o->v_size == r->v_size );
    for( sz_t i = 0; i < o->v_size; i++ ) r->v_data[ i ] = o->v_data[ i ] * m->v_data[ i ];
}

/// o <*> m + b -> r (hadamard product)
void bhvm_hf3_s_hmul_add( const bhvm_hf3_s* o, const bhvm_hf3_s* m, const bhvm_hf3_s* b, bhvm_hf3_s* r )
{
    ASSERT( o->v_size == m->v_size );
    ASSERT( o->v_size == b->v_size );
    ASSERT( o->v_size == r->v_size );
    for( sz_t i = 0; i < o->v_size; i++ ) r->v_data[ i ] = b->v_data[ i ] + o->v_data[ i ] * m->v_data[ i ];
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_adl_s_hmul( const bhvm_hf3_adl_s* o, const bhvm_hf3_adl_s* m, bhvm_hf3_adl_s* r )
{
    ASSERT( o->size == m->size );
    ASSERT( o->size == r->size );
    for( sz_t i = 0; i < o->size; i++ ) bhvm_hf3_s_hmul( o->data[ i ], m->data[ i ], r->data[ i ] );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_mul_scl( const bhvm_hf3_s* o, const f3_t* b, bhvm_hf3_s* r )
{
    ASSERT( o->v_size == r->v_size );
    bmath_f3_t_vec_mul_scl( o->v_data, *b, r->v_data, o->v_size );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_adl_s_mul_scl_f3( const bhvm_hf3_adl_s* o, f3_t b, bhvm_hf3_adl_s* r )
{
    ASSERT( o->size == r->size );
    for( sz_t i = 0; i < o->size; i++ ) bhvm_hf3_s_mul_scl_f3( o->data[ i ], b, r->data[ i ] );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_adl_s_mul_scl( const bhvm_hf3_adl_s* o, const f3_t* b, bhvm_hf3_adl_s* r )
{
    bhvm_hf3_adl_s_mul_scl_f3( o, *b, r );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_mul_scl_add( const bhvm_hf3_s* o, const f3_t* b, const bhvm_hf3_s* c, bhvm_hf3_s* r )
{
    ASSERT( o->v_size == r->v_size );
    ASSERT( o->v_size == c->v_size );
    bmath_f3_t_vec_mul_scl_add( o->v_data, *b, c->v_data, r->v_data, o->v_size );
}

// ---------------------------------------------------------------------------------------------------------------------

f3_t bhvm_hf3_s_f3_vec_mul_vec( const bhvm_hf3_s* o, const bhvm_hf3_s* b )
{
    ASSERT( o->v_size == b->v_size );
    return bmath_f3_t_vec_mul_vec( o->v_data, b->v_data, o->v_size );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_vec_mul_vec( const bhvm_hf3_s* o, const bhvm_hf3_s* b, f3_t* r )
{
    *r = bhvm_hf3_s_f3_vec_mul_vec( o, b );
}

// ---------------------------------------------------------------------------------------------------------------------

static f3_t f3_s_f3_max( const f3_t* v1, uz_t size )
{
    f3_t max = ( size > 0 ) ? v1[ 0 ] : 0;
    for( uz_t i = 1; i < size; i++ ) max = f3_max( max, v1[ i ] );
    return max;
}

//----------------------------------------------------------------------------------------------------------------------

static f3_t f3_s_f3_min( const f3_t* v1, uz_t size )
{
    f3_t min = ( size > 0 ) ? v1[ 0 ] : 0;
    for( uz_t i = 1; i < size; i++ ) min = f3_min( min, v1[ i ] );
    return min;
}

//----------------------------------------------------------------------------------------------------------------------

f3_t bhvm_hf3_s_f3_sum( const bhvm_hf3_s* o )
{
    return bmath_f3_t_vec_sum( o->v_data, o->v_size );
}

//----------------------------------------------------------------------------------------------------------------------

f3_t bhvm_hf3_s_f3_avg( const bhvm_hf3_s* o )
{
    return o->v_size > 0 ? bhvm_hf3_s_f3_sum( o ) / o->v_size : 0;
}

//----------------------------------------------------------------------------------------------------------------------

f3_t bhvm_hf3_s_f3_max( const bhvm_hf3_s* o )
{
    return f3_s_f3_max( o->v_data, o->v_size );
}

//----------------------------------------------------------------------------------------------------------------------

f3_t bhvm_hf3_s_f3_min( const bhvm_hf3_s* o )
{
    return f3_s_f3_min( o->v_data, o->v_size );
}

//----------------------------------------------------------------------------------------------------------------------

/// operators

void bhvm_hf3_s_fp_f3_ar0( bhvm_hf3_s* o, bmath_fp_f3_ar0 fp )
{
    f3_t v = fp();
    for( sz_t i = 0; i < o->v_size; i++ ) o->v_data[ i ] = v;
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_fp_f3_ar1( const bhvm_hf3_s* a, bmath_fp_f3_ar1 fp, bhvm_hf3_s* r )
{
    ASSERT( a->v_size == r->v_size );
    for( sz_t i = 0; i < a->v_size; i++ ) r->v_data[ i ] = fp( a->v_data[ i ] );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_fp_f3_ar2( const bhvm_hf3_s* a, const bhvm_hf3_s* b, bmath_fp_f3_ar2 fp, bhvm_hf3_s* r )
{
    ASSERT( a->v_size == r->v_size );
    ASSERT( b->v_size == r->v_size );
    for( sz_t i = 0; i < a->v_size; i++ ) r->v_data[ i ] = fp( a->v_data[ i ], b->v_data[ i ] );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_fp_f3_op_ar0( bhvm_hf3_s* o, bmath_fp_f3_op_ar0 fp, vc_t op )
{
    f3_t v = fp( op );
    for( sz_t i = 0; i < o->v_size; i++ ) o->v_data[ i ] = v;
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_fp_f3_op_ar1( const bhvm_hf3_s* a, bmath_fp_f3_op_ar1 fp, vc_t op, bhvm_hf3_s* r )
{
    ASSERT( a->v_size == r->v_size );
    for( sz_t i = 0; i < a->v_size; i++ ) r->v_data[ i ] = fp( op, a->v_data[ i ] );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_fp_f3_op_ar2( const bhvm_hf3_s* a, const bhvm_hf3_s* b, bmath_fp_f3_op_ar2 fp, vc_t op, bhvm_hf3_s* r )
{
    ASSERT( a->v_size == r->v_size );
    ASSERT( b->v_size == r->v_size );
    for( sz_t i = 0; i < a->v_size; i++ ) r->v_data[ i ] = fp( op, a->v_data[ i ], b->v_data[ i ] );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_fp_f3_ar0_madd( bmath_fp_f3_ar0 fp, const bhvm_hf3_s* m, bhvm_hf3_s* r )
{
    ASSERT( m->v_size == r->v_size );
    f3_t c = fp();
    for( sz_t i = 0; i < r->v_size; i++ ) r->v_data[ i ] += c * m->v_data[ i ];
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_fp_f3_ar1_madd( const bhvm_hf3_s* a, bmath_fp_f3_ar1 fp, const bhvm_hf3_s* m, bhvm_hf3_s* r )
{
    ASSERT( a->v_size == r->v_size );
    ASSERT( m->v_size == r->v_size );
    for( sz_t i = 0; i < a->v_size; i++ ) r->v_data[ i ] += fp( a->v_data[ i ] ) * m->v_data[ i ];
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_fp_f3_ar2_madd( const bhvm_hf3_s* a, const bhvm_hf3_s* b, bmath_fp_f3_ar2 fp, const bhvm_hf3_s* m, bhvm_hf3_s* r )
{
    ASSERT( a->v_size == r->v_size );
    ASSERT( b->v_size == r->v_size );
    ASSERT( m->v_size == r->v_size );
    for( sz_t i = 0; i < a->v_size; i++ ) r->v_data[ i ] += fp( a->v_data[ i ], b->v_data[ i ] ) * m->v_data[ i ];
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// specific (max order 2) holor * holor multiplications

// ---------------------------------------------------------------------------------------------------------------------

#define _0H  0
#define _0Ht 1
#define _1H  2
#define _1Ht 3
#define _2H  4
#define _2Ht 5

#define MAX_D_SIZE 2
#define HT_CODE_RANGE ( 2                    )
#define HD_CODE_RANGE ( MAX_D_SIZE + 1       )
#define HS_CODE_RANGE ( HD_CODE_RANGE * HT_CODE_RANGE )

#define HT_CODE( h ) ( h->htp ? 1 : 0 )
#define BHT_CODE( HT_CODE1, HT_CODE2           ) (           HT_CODE1             * HT_CODE_RANGE + HT_CODE2 )
#define THT_CODE( HT_CODE1, HT_CODE2, HT_CODE3 ) ( BHT_CODE( HT_CODE1, HT_CODE2 ) * HT_CODE_RANGE + HT_CODE3 )

#define HD_CODE( h ) ( h->d_size )
#define BHD_CODE( HD_CODE1, HD_CODE2           ) (           HD_CODE1             * HD_CODE_RANGE + HD_CODE2 )
#define THD_CODE( HD_CODE1, HD_CODE2, HD_CODE3 ) ( BHD_CODE( HD_CODE1, HD_CODE2 ) * HD_CODE_RANGE + HD_CODE3 )

#define HS_CODE( h ) ( HD_CODE( h ) * HT_CODE_RANGE + HT_CODE( h ) )
#define BHS_CODE( HS_CODE1, HS_CODE2           ) (           HS_CODE1             * HS_CODE_RANGE + HS_CODE2 )
#define THS_CODE( HS_CODE1, HS_CODE2, HS_CODE3 ) ( BHS_CODE( HS_CODE1, HS_CODE2 ) * HS_CODE_RANGE + HS_CODE3 )

s2_t bhvm_hf3_s_ht_code(  const bhvm_hf3_s* o                                           ) { return  HT_CODE( o ); }
s2_t bhvm_hf3_s_bht_code( const bhvm_hf3_s* o, const bhvm_hf3_s* b                      ) { return BHT_CODE( HT_CODE( o ), HT_CODE( b ) ); }
s2_t bhvm_hf3_s_tht_code( const bhvm_hf3_s* o, const bhvm_hf3_s* b, const bhvm_hf3_s* r ) { return THT_CODE( HT_CODE( o ), HT_CODE( b ), HT_CODE( r ) ); }

s2_t bhvm_hf3_s_hd_code(  const bhvm_hf3_s* o                                           ) { return  HD_CODE( o ); }
s2_t bhvm_hf3_s_bhd_code( const bhvm_hf3_s* o, const bhvm_hf3_s* b                      ) { return BHD_CODE( HD_CODE( o ), HD_CODE( b ) ); }
s2_t bhvm_hf3_s_thd_code( const bhvm_hf3_s* o, const bhvm_hf3_s* b, const bhvm_hf3_s* r ) { return THD_CODE( HD_CODE( o ), HD_CODE( b ), HD_CODE( r ) ); }

s2_t bhvm_hf3_s_hs_code(  const bhvm_hf3_s* o                                           ) { return  HS_CODE( o ); }
s2_t bhvm_hf3_s_bhs_code( const bhvm_hf3_s* o, const bhvm_hf3_s* b                      ) { return BHS_CODE( HS_CODE( o ), HS_CODE( b ) ); }
s2_t bhvm_hf3_s_ths_code( const bhvm_hf3_s* o, const bhvm_hf3_s* b, const bhvm_hf3_s* r ) { return THS_CODE( HS_CODE( o ), HS_CODE( b ), HS_CODE( r ) ); }

void bhvm_hf3_st_s_push_hs_code( st_s* o, s2_t hs_code )
{
    sz_t order = hs_code >> 1;
    bl_t htp    = ( hs_code & 1 ) ? true : false;
    st_s_push_fa( o, "#<sz_t>H#<sc_t>", order, htp ? "t" : "" );
}

void bhvm_hf3_st_s_push_bhs_code( st_s* o, s2_t bhs_code )
{
    bhvm_hf3_st_s_push_hs_code( o, bhs_code / HS_CODE_RANGE );
    st_s_push_fa( o, "**" );
    bhvm_hf3_st_s_push_hs_code( o, bhs_code % HS_CODE_RANGE );
}

void bhvm_hf3_st_s_push_ths_code( st_s* o, s2_t ths_code )
{
    bhvm_hf3_st_s_push_bhs_code( o, ths_code / HS_CODE_RANGE );
    st_s_push_fa( o, "->" );
    bhvm_hf3_st_s_push_hs_code( o, ths_code % HS_CODE_RANGE );
}

#define BHS_CODE_ERR( code ) { st_s* s = st_s_create(); bhvm_hf3_st_s_push_bhs_code( s, code ); ERR_fa( "Undefined bmul operation: '#<sc_t>'", s->sc ); st_s_discard( s ); }
#define THS_CODE_ERR( code ) { st_s* s = st_s_create(); bhvm_hf3_st_s_push_ths_code( s, code ); ERR_fa( "Undefined bmul operation: '#<sc_t>'", s->sc ); st_s_discard( s ); }

void bhvm_hf3_s_bmul( const bhvm_hf3_s* o, const bhvm_hf3_s* b, bhvm_hf3_s* r )
{
    ASSERT( o->d_size <= 2 );
    ASSERT( b->d_size <= 2 );
    assert( o->v_size );
    assert( b->v_size );
    assert( r->v_size );

    const s2_t tht_code = THT_CODE( HT_CODE( o ), HT_CODE( b ), HT_CODE( r ) );
    const s2_t thd_code = THD_CODE( HD_CODE( o ), HD_CODE( b ), HD_CODE( r ) );

    switch( thd_code )
    {
        case THD_CODE( 0, 0, 0 ): // all scalars
        {
            r->v_data[ 0 ] = o->v_data[ 0 ] * b->v_data[ 0 ];
        }
        break;

        case THD_CODE( 1, 0, 1 ): // vec * scalar
        {
            ASSERT( o->d_data[ 0 ] == r->d_data[ 0 ] );
            switch( tht_code )
            {
                case THT_CODE( 0, 0, 0 ):
                case THT_CODE( 0, 1, 0 ):
                    bhvm_hf3_s_mul_scl( o, b->v_data, r );
                    break;

                default: THS_CODE_ERR( THS_CODE( HS_CODE( o ), HS_CODE( b ), HS_CODE( r ) ) ); break;
            }
        }
        break;

        case THD_CODE( 0, 1, 1 ): // scalar * vec^t
        {
            ASSERT( b->d_data[ 0 ] == r->d_data[ 0 ] );
            switch( tht_code )
            {
                case THT_CODE( 0, 1, 1 ):
                case THT_CODE( 1, 1, 1 ):
                    bhvm_hf3_s_mul_scl( b, o->v_data, r );
                    break;

                default: THS_CODE_ERR( THS_CODE( HS_CODE( o ), HS_CODE( b ), HS_CODE( r ) ) ); break;
            }
        }
        break;

        case THD_CODE( 1, 1, 0 ): // dot product
        {
            ASSERT( o->d_data[ 0 ] == b->d_data[ 0 ] );
            switch( tht_code )
            {
                case THT_CODE( 1, 0, 0 ):
                case THT_CODE( 1, 0, 1 ):
                    r->v_data[ 0 ] = bmath_f3_t_vec_mul_vec( o->v_data, b->v_data, o->v_size );
                    break;
                default: THS_CODE_ERR( THS_CODE( HS_CODE( o ), HS_CODE( b ), HS_CODE( r ) ) ); break;
            }
        }
        break;

        case THD_CODE( 1, 1, 2 ): // outer product
        {
            bmath_vf3_s vo = bhvm_hf3_s_get_weak_vec( o );
            bmath_vf3_s vb = bhvm_hf3_s_get_weak_vec( b );
            bmath_mf3_s mr = bhvm_hf3_s_get_weak_mat( r );
            switch( tht_code )
            {
                case THT_CODE( 0, 1, 0 ): bmath_mf3_s_opd( &mr, &vo, &vb ); break;
                case THT_CODE( 0, 1, 1 ): bmath_mf3_s_opd( &mr, &vb, &vo ); break;
                default: THS_CODE_ERR( THS_CODE( HS_CODE( o ), HS_CODE( b ), HS_CODE( r ) ) ); break;
            }
        }
        break;

        case THD_CODE( 1, 2, 1 ): // vec^t * mat
        {
            bmath_vf3_s vo = bhvm_hf3_s_get_weak_vec( o );
            bmath_mf3_s mb = bhvm_hf3_s_get_weak_mat( b );
            bmath_vf3_s vr = bhvm_hf3_s_get_weak_vec( r );
            bmath_vf3_s* po = ( o == r ) ? &vr : &vo;
            switch( tht_code )
            {
                case THT_CODE( 1, 0, 1 ): bmath_mf3_s_htp_mul_vec( &mb, po, &vr ); break;
                case THT_CODE( 1, 1, 1 ): bmath_mf3_s_mul_vec(     &mb, po, &vr ); break;
                default: THS_CODE_ERR( THS_CODE( HS_CODE( o ), HS_CODE( b ), HS_CODE( r ) ) ); break;
            }
        }
        break;

        case THD_CODE( 2, 1, 1 ): // mat * vec
        {
            bmath_mf3_s mo = bhvm_hf3_s_get_weak_mat( o );
            bmath_vf3_s vb = bhvm_hf3_s_get_weak_vec( b );
            bmath_vf3_s vr = bhvm_hf3_s_get_weak_vec( r );
            bmath_vf3_s* pb = ( b == r ) ? &vr : &vb;
            switch( tht_code )
            {
                case THT_CODE( 0, 0, 0 ): bmath_mf3_s_mul_vec(     &mo, pb, &vr ); break;
                case THT_CODE( 1, 0, 0 ): bmath_mf3_s_htp_mul_vec( &mo, pb, &vr ); break;
                default: THS_CODE_ERR( THS_CODE( HS_CODE( o ), HS_CODE( b ), HS_CODE( r ) ) ); break;
            }
        }
        break;

        case THD_CODE( 2, 2, 2 ): // mat * mat
        {
            bmath_mf3_s mo = bhvm_hf3_s_get_weak_mat( o );
            bmath_mf3_s mb = bhvm_hf3_s_get_weak_mat( b );
            bmath_mf3_s mr = bhvm_hf3_s_get_weak_mat( r );
            bmath_mf3_s* po = ( o == b ) ? ( ( o == r ) ? &mr : &mb ) : ( ( o == r ) ? &mr : &mo );
            bmath_mf3_s* pb = ( b == r ) ? &mr : &mb;
            switch( tht_code )
            {
                case THT_CODE( 0, 0, 0 ): bmath_mf3_s_mul(         po, pb, &mr ); break;
                case THT_CODE( 0, 0, 1 ): bmath_mf3_s_htp_mul_htp( pb, po, &mr ); break;
                case THT_CODE( 0, 1, 0 ): bmath_mf3_s_mul_htp(     po, pb, &mr ); break;
                case THT_CODE( 0, 1, 1 ): bmath_mf3_s_mul_htp(     pb, po, &mr ); break;
                case THT_CODE( 1, 0, 0 ): bmath_mf3_s_htp_mul(     po, pb, &mr ); break;
                case THT_CODE( 1, 0, 1 ): bmath_mf3_s_htp_mul(     pb, po, &mr ); break;
                case THT_CODE( 1, 1, 0 ): bmath_mf3_s_htp_mul_htp( po, pb, &mr ); break;
                case THT_CODE( 1, 1, 1 ): bmath_mf3_s_mul(         pb, po, &mr ); break;
                default: THS_CODE_ERR( THS_CODE( HS_CODE( o ), HS_CODE( b ), HS_CODE( r ) ) ); break;
            }
        }
        break;

        default: THS_CODE_ERR( THS_CODE( HS_CODE( o ), HS_CODE( b ), HS_CODE( r ) ) ); break;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_bmul_add( const bhvm_hf3_s* o, const bhvm_hf3_s* b, const bhvm_hf3_s* c, bhvm_hf3_s* r )
{
    ASSERT( o->d_size <= 2 );
    ASSERT( b->d_size <= 2 );
    ASSERT( bhvm_hf3_s_shape_equal( c, r ) );
    assert( o->v_size );
    assert( b->v_size );
    assert( c->v_size );
    assert( r->v_size );

    const s2_t tht_code = THT_CODE( HT_CODE( o ), HT_CODE( b ), HT_CODE( r ) );
    const s2_t thd_code = THD_CODE( HD_CODE( o ), HD_CODE( b ), HD_CODE( r ) );

    switch( thd_code )
    {
        case THD_CODE( 0, 0, 0 ): // all scalars
        {
            r->v_data[ 0 ] = o->v_data[ 0 ] * b->v_data[ 0 ] + c->v_data[ 0 ];
        }
        break;

        case THD_CODE( 1, 0, 1 ): // vec * scalar
        {
            ASSERT( o->d_data[ 0 ] == r->d_data[ 0 ] );
            switch( tht_code )
            {
                case THT_CODE( 0, 0, 0 ):
                case THT_CODE( 0, 1, 0 ):
                    bhvm_hf3_s_mul_scl_add( o, b->v_data, c, r );
                    break;

                default: THS_CODE_ERR( THS_CODE( HS_CODE( o ), HS_CODE( b ), HS_CODE( r ) ) ); break;
            }
        }
        break;

        case THD_CODE( 0, 1, 1 ): // scalar * vec^t
        {
            ASSERT( b->d_data[ 0 ] == r->d_data[ 0 ] );
            switch( tht_code )
            {
                case THT_CODE( 0, 1, 1 ):
                case THT_CODE( 1, 1, 1 ):
                    bhvm_hf3_s_mul_scl_add( b, o->v_data, c, r );
                    break;

                default: THS_CODE_ERR( THS_CODE( HS_CODE( o ), HS_CODE( b ), HS_CODE( r ) ) ); break;
            }
        }
        break;

        case THD_CODE( 1, 1, 0 ): // dot product
        {
            ASSERT( o->d_data[ 0 ] == b->d_data[ 0 ] );
            switch( tht_code )
            {
                case THT_CODE( 1, 0, 0 ):
                case THT_CODE( 1, 0, 1 ):
                    r->v_data[ 0 ] = bmath_f3_t_vec_mul_vec( o->v_data, b->v_data, o->v_size ) + c->v_data[ 0 ];
                    break;
                default: THS_CODE_ERR( THS_CODE( HS_CODE( o ), HS_CODE( b ), HS_CODE( r ) ) ); break;
            }
        }
        break;

        case THD_CODE( 1, 1, 2 ): // outer product
        {
            bmath_vf3_s vo = bhvm_hf3_s_get_weak_vec( o );
            bmath_vf3_s vb = bhvm_hf3_s_get_weak_vec( b );
            bmath_mf3_s mc = bhvm_hf3_s_get_weak_mat( c );
            bmath_mf3_s mr = bhvm_hf3_s_get_weak_mat( r );
            bmath_mf3_s* pc = ( c == r ) ? &mr : &mc;
            switch( tht_code )
            {
                case THT_CODE( 0, 1, 0 ): bmath_mf3_s_opd_add( &mr, &vo, &vb, pc ); break;
                case THT_CODE( 0, 1, 1 ): bmath_mf3_s_opd_add( &mr, &vb, &vo, pc ); break;
                default: THS_CODE_ERR( THS_CODE( HS_CODE( o ), HS_CODE( b ), HS_CODE( r ) ) ); break;
            }
        }
        break;

        case THD_CODE( 1, 2, 1 ): // vec^t * mat
        {
            bmath_vf3_s vo = bhvm_hf3_s_get_weak_vec( o );
            bmath_mf3_s mb = bhvm_hf3_s_get_weak_mat( b );
            bmath_vf3_s vc = bhvm_hf3_s_get_weak_vec( c );
            bmath_vf3_s vr = bhvm_hf3_s_get_weak_vec( r );
            bmath_vf3_s* po = ( o == r ) ? &vr : &vo;
            bmath_vf3_s* pc = ( c == r ) ? &vr : &vc;
            switch( tht_code )
            {
                case THT_CODE( 1, 0, 1 ): bmath_mf3_s_htp_mul_vec_add( &mb, po, pc, &vr ); break;
                case THT_CODE( 1, 1, 1 ): bmath_mf3_s_mul_vec_add(     &mb, po, pc, &vr ); break;
                default: THS_CODE_ERR( THS_CODE( HS_CODE( o ), HS_CODE( b ), HS_CODE( r ) ) ); break;
            }
        }
        break;

        case THD_CODE( 2, 1, 1 ): // mat * vec
        {
            bmath_mf3_s mo = bhvm_hf3_s_get_weak_mat( o );
            bmath_vf3_s vb = bhvm_hf3_s_get_weak_vec( b );
            bmath_vf3_s vc = bhvm_hf3_s_get_weak_vec( c );
            bmath_vf3_s vr = bhvm_hf3_s_get_weak_vec( r );
            bmath_vf3_s* pb = ( b == r ) ? &vr : &vb;
            bmath_vf3_s* pc = ( c == r ) ? &vr : &vc;
            switch( tht_code )
            {
                case THT_CODE( 0, 0, 0 ): bmath_mf3_s_mul_vec_add(     &mo, pb, pc, &vr ); break;
                case THT_CODE( 1, 0, 0 ): bmath_mf3_s_htp_mul_vec_add( &mo, pb, pc, &vr ); break;
                default: THS_CODE_ERR( THS_CODE( HS_CODE( o ), HS_CODE( b ), HS_CODE( r ) ) ); break;
            }
        }
        break;

        case THD_CODE( 2, 2, 2 ): // mat * mat
        {
            bmath_mf3_s mo = bhvm_hf3_s_get_weak_mat( o );
            bmath_mf3_s mb = bhvm_hf3_s_get_weak_mat( b );
            bmath_mf3_s mc = bhvm_hf3_s_get_weak_mat( c );
            bmath_mf3_s mr = bhvm_hf3_s_get_weak_mat( r );
            bmath_mf3_s* po = ( o == b ) ? ( ( o == r ) ? &mr : &mb ) : ( ( o == r ) ? &mr : &mo );
            bmath_mf3_s* pb = ( b == r ) ? &mr : &mb;
            bmath_mf3_s* pc = ( c == r ) ? &mr : &mc;
            switch( tht_code )
            {
                case THT_CODE( 0, 0, 0 ): bmath_mf3_s_mul_add(         po, pb, pc, &mr ); break;
                case THT_CODE( 0, 0, 1 ): bmath_mf3_s_htp_mul_htp_add( pb, po, pc, &mr ); break;
                case THT_CODE( 0, 1, 0 ): bmath_mf3_s_mul_htp_add(     po, pb, pc, &mr ); break;
                case THT_CODE( 0, 1, 1 ): bmath_mf3_s_mul_htp_add(     pb, po, pc, &mr ); break;
                case THT_CODE( 1, 0, 0 ): bmath_mf3_s_htp_mul_add(     po, pb, pc, &mr ); break;
                case THT_CODE( 1, 0, 1 ): bmath_mf3_s_htp_mul_add(     pb, po, pc, &mr ); break;
                case THT_CODE( 1, 1, 0 ): bmath_mf3_s_htp_mul_htp_add( po, pb, pc, &mr ); break;
                case THT_CODE( 1, 1, 1 ): bmath_mf3_s_mul_add(         pb, po, pc, &mr ); break;
                default: THS_CODE_ERR( THS_CODE( HS_CODE( o ), HS_CODE( b ), HS_CODE( r ) ) ); break;
            }
        }
        break;

        default: THS_CODE_ERR( THS_CODE( HS_CODE( o ), HS_CODE( b ), HS_CODE( r ) ) ); break;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

bl_t bhvm_hf3_s_bmul_bhs_code_allowed( s2_t bhs_code )
{
    switch( bhs_code )
    {
        case BHS_CODE(_0H ,_0H ): return true;
        case BHS_CODE(_1H ,_0H ): return true;
        case BHS_CODE(_2H ,_1H ): return true;
        case BHS_CODE(_2H ,_2H ): return true;
        case BHS_CODE(_0H ,_0Ht): return true;
        case BHS_CODE(_0H ,_1Ht): return true;
        case BHS_CODE(_1H ,_0Ht): return true;
        case BHS_CODE(_1H ,_1Ht): return true;
        case BHS_CODE(_2H ,_2Ht): return true;
        case BHS_CODE(_0Ht,_0H ): return true;
        case BHS_CODE(_1Ht,_1H ): return true;
        case BHS_CODE(_1Ht,_2H ): return true;
        case BHS_CODE(_2Ht,_1H ): return true;
        case BHS_CODE(_2Ht,_2H ): return true;
        case BHS_CODE(_0Ht,_0Ht): return true;
        case BHS_CODE(_0Ht,_1Ht): return true;
        case BHS_CODE(_1Ht,_2Ht): return true;
        case BHS_CODE(_2Ht,_2Ht): return true;
        default: return false;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

bl_t bhvm_hf3_s_set_shape_bmul( const bhvm_hf3_s* o, const bhvm_hf3_s* b, bhvm_hf3_s* r )
{
    if( o->d_size > 2 ) return false;
    if( b->d_size > 2 ) return false;
    s2_t bhs_code = BHS_CODE( HS_CODE( o ), HS_CODE( b ) );

    if( !bhvm_hf3_s_bmul_bhs_code_allowed( bhs_code ) ) return false;

    // within allowed: check for valid configuration
    switch( bhs_code )
    {
        case BHS_CODE(_2H ,_1H ): if( o->d_data[ 0 ] != b->d_data[ 0 ] ) return false; else break;
        case BHS_CODE(_2H ,_2H ): if( o->d_data[ 0 ] != b->d_data[ 1 ] ) return false; else break;
        case BHS_CODE(_2H ,_2Ht): if( o->d_data[ 0 ] != b->d_data[ 0 ] ) return false; else break;
        case BHS_CODE(_1Ht,_1H ): if( o->d_data[ 0 ] != b->d_data[ 0 ] ) return false; else break; /* dot product */
        case BHS_CODE(_2Ht,_1H ): if( o->d_data[ 1 ] != b->d_data[ 0 ] ) return false; else break;
        case BHS_CODE(_2Ht,_2H ): if( o->d_data[ 1 ] != b->d_data[ 1 ] ) return false; else break;
        case BHS_CODE(_1Ht,_2Ht): if( o->d_data[ 0 ] != b->d_data[ 0 ] ) return false; else break;
        case BHS_CODE(_2Ht,_2Ht): if( o->d_data[ 1 ] != b->d_data[ 0 ] ) return false; else break;
        default: return false;
    }

    // apply shape
    switch( bhs_code )
    {
        case BHS_CODE(_0H ,_0H ): /*->0H */ r->htp=false; bhvm_hf3_s_set_d_data_na( r, 0                                 ); break;
        case BHS_CODE(_1H ,_0H ): /*->1H */ r->htp=false; bhvm_hf3_s_set_d_data_na( r, 1, o->d_data[ 0 ]                 ); break;
        case BHS_CODE(_2H ,_1H ): /*->1H */ r->htp=false; bhvm_hf3_s_set_d_data_na( r, 1, o->d_data[ 1 ]                 ); break;
        case BHS_CODE(_2H ,_2H ): /*->2H */ r->htp=false; bhvm_hf3_s_set_d_data_na( r, 2, b->d_data[ 0 ], o->d_data[ 1 ] ); break;
        case BHS_CODE(_0H ,_0Ht): /*->0H */ r->htp=false; bhvm_hf3_s_set_d_data_na( r, 0                                 ); break;
        case BHS_CODE(_0H ,_1Ht): /*->1Ht*/ r->htp=true ; bhvm_hf3_s_set_d_data_na( r, 1, b->d_data[ 0 ]                 ); break;
        case BHS_CODE(_1H ,_0Ht): /*->1H */ r->htp=false; bhvm_hf3_s_set_d_data_na( r, 1, o->d_data[ 0 ]                 ); break;
        case BHS_CODE(_1H ,_1Ht): /*->2H */ r->htp=false; bhvm_hf3_s_set_d_data_na( r, 2, b->d_data[ 0 ], o->d_data[ 0 ] ); break; /* outer product */
        case BHS_CODE(_2H ,_2Ht): /*->2H */ r->htp=false; bhvm_hf3_s_set_d_data_na( r, 2, b->d_data[ 1 ], o->d_data[ 1 ] ); break;
        case BHS_CODE(_0Ht,_0H ): /*->0H */ r->htp=false; bhvm_hf3_s_set_d_data_na( r, 0                                 ); break;
        case BHS_CODE(_1Ht,_1H ): /*->0H */ r->htp=false; bhvm_hf3_s_set_d_data_na( r, 0                                 ); break; /* dot product */
        case BHS_CODE(_1Ht,_2H ): /*->1Ht*/ r->htp=true ; bhvm_hf3_s_set_d_data_na( r, 1, o->d_data[ 0 ]                 ); break;
        case BHS_CODE(_2Ht,_1H ): /*->1H */ r->htp=false; bhvm_hf3_s_set_d_data_na( r, 1, o->d_data[ 0 ]                 ); break;
        case BHS_CODE(_2Ht,_2H ): /*->2H */ r->htp=false; bhvm_hf3_s_set_d_data_na( r, 2, b->d_data[ 0 ], o->d_data[ 0 ] ); break;
        case BHS_CODE(_0Ht,_0Ht): /*->0H */ r->htp=false; bhvm_hf3_s_set_d_data_na( r, 0                                 ); break;
        case BHS_CODE(_0Ht,_1Ht): /*->1Ht*/ r->htp=true ; bhvm_hf3_s_set_d_data_na( r, 1, b->d_data[ 0 ]                 ); break;
        case BHS_CODE(_1Ht,_2Ht): /*->1Ht*/ r->htp=true ; bhvm_hf3_s_set_d_data_na( r, 1, b->d_data[ 1 ]                 ); break;
        case BHS_CODE(_2Ht,_2Ht): /*->2H */ r->htp=false; bhvm_hf3_s_set_d_data_na( r, 2, b->d_data[ 1 ], o->d_data[ 0 ] ); break;
        default: return false;
    }
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_s_bmul_add_cps( const bhvm_hf3_s* a, const bhvm_hf3_s* b, f3_t c, const bhvm_hf3_s* d, f3_t e, bhvm_hf3_s* r )
{
    if( d )
    {
        ASSERT( bhvm_hf3_s_d_equal( d, r ) );
        if( c != 0 )
        {
            if( c == e )
            {
                if( d != r ) bhvm_hf3_s_cpy( d, r );
            }
            else
            {
                bhvm_hf3_s_mul_scl_f3( d, e / c, r );
            }
        }
        else
        {
            if( e == 1.0 )
            {
                if( d != r ) bhvm_hf3_s_cpy( d, r );
            }
            else
            {
                bhvm_hf3_s_mul_scl_f3( d, e, r );
            }
        }
    }
    else
    {
        bhvm_hf3_s_zro( r );
    }

    if( c != 0 )
    {
        bhvm_hf3_s_bmul_add( a, b, r, r );
        if( c != 1.0 ) bhvm_hf3_s_mul_scl_f3( r, c, r );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

static void selftest( void )
{
    BLM_INIT();

    bhvm_hf3_s* h1 = BLM_CREATE( bhvm_hf3_s );
    bhvm_hf3_s* h2 = BLM_CREATE( bhvm_hf3_s );

    bhvm_hf3_s_set_size_na( h1, 3, 2, 2, 7 );
    bhvm_hf3_s_copy_size( h2, h1 );

    u2_t rval = 123;
    bhvm_hf3_s_set_random( h1, 1.0, 0, 1, &rval );
    bhvm_hf3_s_cpy( h1, h2 );

    bhvm_hf3_s_mul_scl_f3_add( h1, 2, h2, h2 );

    ASSERT( f3_abs( ( bhvm_hf3_s_f3_sum( h2 ) / bhvm_hf3_s_f3_sum( h1 ) ) - 3.0 ) < 1E-10 );

    BLM_DOWN();
}

// ---------------------------------------------------------------------------------------------------------------------

#endif // TYPEOF_bhvm_hf3

vd_t bhvm_hf3_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bhvm_hf3" ) ) )
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
            bcore_plant_compile( "bhvm_planted_hf3", __FILE__ );
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

