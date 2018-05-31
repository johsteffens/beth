/** Copyright 2017 Johannes Bernhard Steffens
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

#include "bcore_arr.h"
#include "bcore_signal.h"
#include "bcore_spect_inst.h"
#include "bcore_spect_array.h"
#include "bcore_sr.h"
#include "bcore_spect_compare.h"
#include "bcore_signal.h"

/**********************************************************************************************************************/
// bcore_arr_sz_s

void bcore_arr_sz_s_init( bcore_arr_sz_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = TYPEOF_bcore_arr_sz_s;
}

void bcore_arr_sz_s_down( bcore_arr_sz_s* o )
{
    if( o->space > 0 ) bcore_un_alloc( sizeof( sz_t ), o->data, o->space, 0, NULL );
}

void bcore_arr_sz_s_copy( bcore_arr_sz_s* o, const bcore_arr_sz_s* src )
{
    if( o->space < src->size )
    {
        if( o->space > 0 ) o->data = bcore_un_alloc( sizeof( sz_t ), o->data, o->space, 0, &o->space );
        o->data = bcore_un_alloc( sizeof( sz_t ), NULL, o->space, src->size, &o->space );
    }
    bcore_u_memcpy( sizeof( sz_t ), o->data, src->data, src->size );
    o->size = src->size;
}

BCORE_DEFINE_FUNCTION_CREATE(  bcore_arr_sz_s )
BCORE_DEFINE_FUNCTION_DISCARD( bcore_arr_sz_s )
BCORE_DEFINE_FUNCTION_CLONE(   bcore_arr_sz_s )

static bcore_self_s* arr_sz_s_create_self( void )
{
    bcore_self_s* self = bcore_self_s_build_parse_sc( "bcore_arr_sz_s = bcore_array { aware_t _; sz_t [] arr; }", sizeof( bcore_arr_sz_s ) );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_sz_s_init, "bcore_fp_init", "init" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_sz_s_down, "bcore_fp_down", "down" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_sz_s_copy, "bcore_fp_copy", "copy" );
    return self;
}

void bcore_arr_sz_s_clear( bcore_arr_sz_s* o )
{
    o->size = 0;
    if( o->space == 0 ) o->data = NULL; // in case array is referencing external data
}

void bcore_arr_sz_s_set_space( bcore_arr_sz_s* o, sz_t space )
{
    if( o->space == 0 )
    {
        o->data = bcore_u_alloc( sizeof( sz_t ), NULL, space, &o->space );
    }
    else
    {
        o->data = bcore_un_alloc( sizeof( sz_t ), o->data, o->space, space, &o->space );
    }
    if( o->size > space ) o->size = space;
}

void bcore_arr_sz_s_fill( bcore_arr_sz_s* o, sz_t size, sz_t v )
{
    bcore_arr_sz_s_set_space( o, size );
    for( sz_t i = 0; i < size; i++ ) o->data[ i ] = v;
    o->size = size;
}

void bcore_arr_sz_s_step_fill( bcore_arr_sz_s* o, sz_t v_start, s3_t step, sz_t size )
{
    bcore_arr_sz_s_set_space( o, size );
    sz_t v = v_start;
    for( sz_t i = 0; i < size; i++ )
    {
        o->data[ i ] = v;
        v += step;
    }
    o->size = size;
}

void bcore_arr_sz_s_make_strong( bcore_arr_sz_s* o )
{
    if( o->size > o->space )
    {
        sz_t* data = o->data;
        o->data = bcore_u_alloc( sizeof( sz_t ), NULL, o->size, &o->space );
        bcore_u_memcpy( sizeof( sz_t ), o->data, data, o->size );
    }
}

void bcore_arr_sz_s_push( bcore_arr_sz_s* o, sz_t v )
{
    if( o->size >  o->space ) bcore_arr_sz_s_make_strong( o );
    if( o->size == o->space )
    {
        o->data = bcore_un_alloc( sizeof( sz_t ), o->data, o->space, o->space > 0 ? o->space * 2 : 1, &o->space );
    }
    o->data[ o->size++ ] = v;
}

sz_t bcore_arr_sz_s_pop( bcore_arr_sz_s* o )
{
    if( o->size == 0 ) return 0;
    o->size--;
    return o->data[ o->size ];
}

static void sz_sort( sz_t* data, sz_t size, sz_t* buf, s2_t order )
{
    if( size < 2 ) return;
    sz_t size1 = size >> 1;
    sz_sort( data,         size1       , buf, order );
    sz_sort( data + size1, size - size1, buf, order );
    bcore_u_memcpy( sizeof( sz_t ), buf, data, size1 );
    if( order > 0 )
    {
        for( sz_t i = 0, w = 0, r = size1; i < size1; )
        {
            data[ w++ ] = ( r == size || buf[ i ] <= data[ r ] ) ? buf[ i++ ] : data[ r++ ];
        }
    }
    else
    {
        for( sz_t i = 0, w = 0, r = size1; i < size1; )
        {
            data[ w++ ] = ( r == size || buf[ i ] >= data[ r ] ) ? buf[ i++ ] : data[ r++ ];
        }
    }
}

void bcore_arr_sz_s_sort( bcore_arr_sz_s* o, s2_t order ) // stable mergesort
{
    if( o->space < o->size ) bcore_arr_sz_s_make_strong( o );
    sz_t* buf = bcore_u_alloc( sizeof( sz_t ), NULL, o->size >> 1, NULL );
    sz_sort( o->data, o->size, buf, order );
    bcore_free( buf );
}

void bcore_arr_sz_s_reorder( bcore_arr_sz_s* o, const bcore_arr_sz_s* order )
{
    if( o->space < o->size ) bcore_arr_sz_s_make_strong( o );
    sz_t buf_space = 0;
    sz_t* buf = bcore_u_alloc( sizeof( sz_t ), NULL, order->size, &buf_space );
    for( sz_t i = 0; i < order->size; i++ )
    {
        assert( order->data[ i ] < o->size );
        buf[ i ] = o->data[ order->data[ i ] ];
    }
    bcore_un_alloc( sizeof( sz_t ), o->data, o->space, 0, &o->space );
    o->data = buf;
    o->space = buf_space;
    o->size = order->size;
}

bcore_arr_sz_s* bcore_arr_sz_s_create_random_permutation( u2_t ( *rg )( u2_t ), u2_t rval, sz_t size )
{
    bcore_arr_sz_s* o = bcore_arr_sz_s_create();
    if( size == 0 ) return o;

    bcore_arr_sz_s_step_fill( o, 0, 1, size );
    sz_t idx = 0;

    for( sz_t i = 0; i < size; i++ )
    {
        idx = i + ( ( idx + ( rval = rg( rval ) ) ) % ( size - i ) );
        sz_t t = o->data[ i ];
        o->data[ i ] = o->data[ idx ];
        o->data[ idx ] = t;
    }

    return o;
}

sz_t bcore_arr_sz_s_find( const bcore_arr_sz_s* o, sz_t start, sz_t end, sz_t v )
{
    if( end >= start )
    {
        sz_t end_l = end < o->size ? end : o->size;
        for( sz_t j = start; j < end_l; j++ ) if( o->data[ j ] == v ) return j;
        return end_l;
    }
    else
    {
        sz_t start_l = start < o->size ? start : o->size;
        for( sz_t j = start_l - 1;  j < start_l && j >= end; j-- ) if( o->data[ j ] == v ) return j;
        return start_l;
    }
}

/**********************************************************************************************************************/
// bcore_arr_u3_s

void bcore_arr_u3_s_init( bcore_arr_u3_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = TYPEOF_bcore_arr_u3_s;
}

void bcore_arr_u3_s_down( bcore_arr_u3_s* o )
{
    if( o->space > 0 ) bcore_un_alloc( sizeof( u3_t ), o->data, o->space, 0, NULL );
}

void bcore_arr_u3_s_copy( bcore_arr_u3_s* o, const bcore_arr_u3_s* src )
{
    if( o->space < src->size )
    {
        if( o->space > 0 ) o->data = bcore_un_alloc( sizeof( u3_t ), o->data, o->space, 0, &o->space );
        o->data = bcore_un_alloc( sizeof( u3_t ), NULL, o->space, src->size, &o->space );
    }
    bcore_u_memcpy( sizeof( u3_t ), o->data, src->data, src->size );
    o->size = src->size;
}

BCORE_DEFINE_FUNCTION_CREATE(  bcore_arr_u3_s )
BCORE_DEFINE_FUNCTION_DISCARD( bcore_arr_u3_s )
BCORE_DEFINE_FUNCTION_CLONE(   bcore_arr_u3_s )

static bcore_self_s* arr_u3_s_create_self( void )
{
    bcore_self_s* self = bcore_self_s_build_parse_sc( "bcore_arr_u3_s = bcore_array { aware_t _; u3_t [] arr; }", sizeof( bcore_arr_u3_s ) );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_u3_s_init, "bcore_fp_init", "init" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_u3_s_down, "bcore_fp_down", "down" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_u3_s_copy, "bcore_fp_copy", "copy" );
    return self;
}

void bcore_arr_u3_s_clear( bcore_arr_u3_s* o )
{
    o->size = 0;
    if( o->space == 0 ) o->data = NULL; // in case array is referencing external data
}

void bcore_arr_u3_s_set_space( bcore_arr_u3_s* o, sz_t space )
{
    if( o->space == 0 )
    {
        o->data = bcore_u_alloc( sizeof( u3_t ), NULL, space, &o->space );
    }
    else
    {
        o->data = bcore_un_alloc( sizeof( u3_t ), o->data, o->space, space, &o->space );
    }
    if( o->size > space ) o->size = space;
}

void bcore_arr_u3_s_fill( bcore_arr_u3_s* o, sz_t size, u3_t v )
{
    bcore_arr_u3_s_set_space( o, size );
    for( sz_t i = 0; i < size; i++ ) o->data[ i ] = v;
    o->size = size;
}

void bcore_arr_u3_s_step_fill( bcore_arr_u3_s* o, u3_t v_start, u3_t step, sz_t size )
{
    bcore_arr_u3_s_set_space( o, size );
    u3_t v = v_start;
    for( sz_t i = 0; i < size; i++ )
    {
        o->data[ i ] = v;
        v += step;
    }
    o->size = size;
}

void bcore_arr_u3_s_make_strong( bcore_arr_u3_s* o )
{
    if( o->size > o->space )
    {
        u3_t* data = o->data;
        o->data = bcore_u_alloc( sizeof( u3_t ), NULL, o->size, &o->space );
        bcore_u_memcpy( sizeof( u3_t ), o->data, data, o->size );
    }
}

void bcore_arr_u3_s_push( bcore_arr_u3_s* o, u3_t v )
{
    if( o->size >  o->space ) bcore_arr_u3_s_make_strong( o );
    if( o->size == o->space )
    {
        o->data = bcore_un_alloc( sizeof( u3_t ), o->data, o->space, o->space > 0 ? o->space * 2 : 1, &o->space );
    }
    o->data[ o->size++ ] = v;
}

u3_t bcore_arr_u3_s_pop( bcore_arr_u3_s* o )
{
    if( o->size == 0 ) return 0;
    o->size--;
    return o->data[ o->size ];
}

static void u3_sort( u3_t* data, sz_t size, u3_t* buf, s2_t order )
{
    if( size < 2 ) return;
    sz_t size1 = size >> 1;
    u3_sort( data,         size1       , buf, order );
    u3_sort( data + size1, size - size1, buf, order );
    bcore_u_memcpy( sizeof( u3_t ), buf, data, size1 );
    if( order > 0 )
    {
        for( sz_t i = 0, w = 0, r = size1; i < size1; )
        {
            data[ w++ ] = ( r == size || buf[ i ] <= data[ r ] ) ? buf[ i++ ] : data[ r++ ];
        }
    }
    else
    {
        for( sz_t i = 0, w = 0, r = size1; i < size1; )
        {
            data[ w++ ] = ( r == size || buf[ i ] >= data[ r ] ) ? buf[ i++ ] : data[ r++ ];
        }
    }
}

void bcore_arr_u3_s_sort( bcore_arr_u3_s* o, s2_t order ) // stable mergesort
{
    if( o->space < o->size ) bcore_arr_u3_s_make_strong( o );
    u3_t* buf = bcore_u_alloc( sizeof( u3_t ), NULL, o->size >> 1, NULL );
    u3_sort( o->data, o->size, buf, order );
    bcore_free( buf );
}

void bcore_arr_u3_s_reorder( bcore_arr_u3_s* o, const bcore_arr_sz_s* order )
{
    if( o->space < o->size ) bcore_arr_u3_s_make_strong( o );
    sz_t buf_space = 0;
    u3_t* buf = bcore_u_alloc( sizeof( u3_t ), NULL, order->size, &buf_space );
    for( sz_t i = 0; i < order->size; i++ )
    {
        assert( order->data[ i ] < o->size );
        buf[ i ] = o->data[ order->data[ i ] ];
    }
    bcore_un_alloc( sizeof( u3_t ), o->data, o->space, 0, &o->space );
    o->data = buf;
    o->space = buf_space;
    o->size = order->size;
}

/**********************************************************************************************************************/
// bcore_arr_tp_s

void bcore_arr_tp_s_init( bcore_arr_tp_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = TYPEOF_bcore_arr_tp_s;
}

void bcore_arr_tp_s_down( bcore_arr_tp_s* o )
{
    if( o->space > 0 ) bcore_un_alloc( sizeof( tp_t ), o->data, o->space, 0, NULL );
}

void bcore_arr_tp_s_copy( bcore_arr_tp_s* o, const bcore_arr_tp_s* src )
{
    if( o->space < src->size )
    {
        if( o->space > 0 ) o->data = bcore_un_alloc( sizeof( tp_t ), o->data, o->space, 0, &o->space );
        o->data = bcore_un_alloc( sizeof( tp_t ), NULL, o->space, src->size, &o->space );
    }
    bcore_u_memcpy( sizeof( tp_t ), o->data, src->data, src->size );
    o->size = src->size;
}

BCORE_DEFINE_FUNCTION_CREATE(  bcore_arr_tp_s )
BCORE_DEFINE_FUNCTION_DISCARD( bcore_arr_tp_s )
BCORE_DEFINE_FUNCTION_CLONE(   bcore_arr_tp_s )

static bcore_self_s* arr_tp_s_create_self( void )
{
    bcore_self_s* self = bcore_self_s_build_parse_sc( "bcore_arr_tp_s = bcore_array { aware_t _; tp_t [] arr; }", sizeof( bcore_arr_tp_s ) );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_tp_s_init, "bcore_fp_init", "init" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_tp_s_down, "bcore_fp_down", "down" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_tp_s_copy, "bcore_fp_copy", "copy" );
    return self;
}

void bcore_arr_tp_s_clear( bcore_arr_tp_s* o )
{
    o->size = 0;
    if( o->space == 0 ) o->data = NULL; // in case array is referencing external data
}

void bcore_arr_tp_s_set_space( bcore_arr_tp_s* o, sz_t space )
{
    if( o->space == 0 )
    {
        o->data = bcore_u_alloc( sizeof( tp_t ), NULL, space, &o->space );
    }
    else
    {
        o->data = bcore_un_alloc( sizeof( tp_t ), o->data, o->space, space, &o->space );
    }
    if( o->size > space ) o->size = space;
}

void bcore_arr_tp_s_fill( bcore_arr_tp_s* o, sz_t size, tp_t v )
{
    bcore_arr_tp_s_set_space( o, size );
    for( sz_t i = 0; i < size; i++ ) o->data[ i ] = v;
    o->size = size;
}

void bcore_arr_tp_s_make_strong( bcore_arr_tp_s* o )
{
    if( o->size > o->space )
    {
        tp_t* data = o->data;
        o->data = bcore_u_alloc( sizeof( tp_t ), NULL, o->size, &o->space );
        bcore_u_memcpy( sizeof( tp_t ), o->data, data, o->size );
    }
}

void bcore_arr_tp_s_push( bcore_arr_tp_s* o, tp_t v )
{
    if( o->size >  o->space ) bcore_arr_tp_s_make_strong( o );
    if( o->size == o->space )
    {
        o->data = bcore_un_alloc( sizeof( tp_t ), o->data, o->space, o->space > 0 ? o->space * 2 : 1, &o->space );
    }
    o->data[ o->size++ ] = v;
}

tp_t bcore_arr_tp_s_pop( bcore_arr_tp_s* o )
{
    if( o->size == 0 ) return 0;
    o->size--;
    return o->data[ o->size ];
}

static void tp_sort( tp_t* data, sz_t size, tp_t* buf, s2_t order )
{
    if( size < 2 ) return;
    sz_t size1 = size >> 1;
    tp_sort( data,         size1       , buf, order );
    tp_sort( data + size1, size - size1, buf, order );
    bcore_u_memcpy( sizeof( tp_t ), buf, data, size1 );
    if( order > 0 )
    {
        for( sz_t i = 0, w = 0, r = size1; i < size1; )
        {
            data[ w++ ] = ( r == size || buf[ i ] <= data[ r ] ) ? buf[ i++ ] : data[ r++ ];
        }
    }
    else
    {
        for( sz_t i = 0, w = 0, r = size1; i < size1; )
        {
            data[ w++ ] = ( r == size || buf[ i ] >= data[ r ] ) ? buf[ i++ ] : data[ r++ ];
        }
    }
}

void bcore_arr_tp_s_sort( bcore_arr_tp_s* o, s2_t order ) // stable mergesort
{
    if( o->space < o->size ) bcore_arr_tp_s_make_strong( o );
    tp_t* buf = bcore_u_alloc( sizeof( tp_t ), NULL, o->size >> 1, NULL );
    tp_sort( o->data, o->size, buf, order );
    bcore_free( buf );
}

void bcore_arr_tp_s_reorder( bcore_arr_tp_s* o, const bcore_arr_sz_s* order )
{
    if( o->space < o->size ) bcore_arr_tp_s_make_strong( o );
    sz_t buf_space = 0;
    tp_t* buf = bcore_u_alloc( sizeof( tp_t ), NULL, order->size, &buf_space );
    for( sz_t i = 0; i < order->size; i++ )
    {
        assert( order->data[ i ] < o->size );
        buf[ i ] = o->data[ order->data[ i ] ];
    }
    bcore_un_alloc( sizeof( tp_t ), o->data, o->space, 0, &o->space );
    o->data = buf;
    o->space = buf_space;
    o->size = order->size;
}

sz_t bcore_arr_tp_s_find( const bcore_arr_tp_s* o, sz_t start, sz_t end, tp_t v )
{
    if( end >= start )
    {
        sz_t end_l = end < o->size ? end : o->size;
        for( sz_t j = start; j < end_l; j++ ) if( o->data[ j ] == v ) return j;
        return end_l;
    }
    else
    {
        sz_t start_l = start < o->size ? start : o->size;
        for( sz_t j = start_l - 1;  j < start_l && j >= end; j-- ) if( o->data[ j ] == v ) return j;
        return start_l;
    }
}

/**********************************************************************************************************************/

BCORE_DEFINE_FUNCTION_INIT_INST( bcore_arr_st_s )
BCORE_DEFINE_FUNCTION_DOWN_INST( bcore_arr_st_s )
BCORE_DEFINE_FUNCTION_COPY_INST( bcore_arr_st_s )
BCORE_DEFINE_FUNCTION_CREATE(    bcore_arr_st_s )
BCORE_DEFINE_FUNCTION_DISCARD(   bcore_arr_st_s )
BCORE_DEFINE_FUNCTION_CLONE(     bcore_arr_st_s )

static bcore_self_s* arr_st_s_create_self( void )
{
    return bcore_self_s_build_parse_sc( "bcore_arr_st_s = bcore_array { aware_t _; st_s* [] arr; }", sizeof( bcore_arr_st_s ) );
}

void bcore_arr_st_s_clear( bcore_arr_st_s* o )
{
    if( o->space == 0 )
    {
        o->data = NULL; // in case array is referencing external data
        o->size = 0;
    }

    for( sz_t i = 0; i < o->size; i++ ) st_s_discard( o->data[ i ] );
    o->size = 0;
}

void bcore_arr_st_s_set_space( bcore_arr_st_s* o, sz_t space )
{
    bcore_array_a_set_space( (bcore_array*)o, space );
}

void bcore_arr_st_s_make_strong( bcore_arr_st_s* o )
{
    if( o->size > o->space )
    {
        st_s** data = o->data;
        o->data = bcore_u_alloc( sizeof( st_s* ), NULL, o->size, &o->space );
        for( sz_t i = 0; i < o->size; i++ )
        {
            st_s_copy( o->data[ i ], data[ i ] );
        }
    }
}

st_s* bcore_arr_st_s_push_st( bcore_arr_st_s* o, const st_s* st )
{
    if( o->size >  o->space ) bcore_arr_st_s_make_strong( o );
    if( o->size == o->space )
    {
        o->data = bcore_un_alloc( sizeof( st_s* ), o->data, o->space, o->space > 0 ? o->space * 2 : 1, &o->space );
    }
    o->data[ o->size++ ] = st_s_clone( st );
    return o->data[ o->size - 1 ];
}

st_s* bcore_arr_st_s_push_st_d( bcore_arr_st_s* o, st_s* st )
{
    if( o->size >  o->space ) bcore_arr_st_s_make_strong( o );
    if( o->size == o->space )
    {
        o->data = bcore_un_alloc( sizeof( st_s* ), o->data, o->space, o->space > 0 ? o->space * 2 : 1, &o->space );
    }
    o->data[ o->size++ ] = st;
    return o->data[ o->size - 1 ];
}

st_s* bcore_arr_st_s_push_sc( bcore_arr_st_s* o, sc_t sc )
{
    return bcore_arr_st_s_push_st_d( o, st_s_create_sc( sc ) );
}

void bcore_arr_st_s_pop( bcore_arr_st_s* o )
{
    if( o->size == 0 ) return;
    o->size--;
    st_s_discard( o->data[ o->size ] );
}

static void st_sort( st_s** data, sz_t size, st_s** buf, s2_t order )
{
    if( size < 2 ) return;
    sz_t size1 = size >> 1;
    st_sort( data,         size1       , buf, order );
    st_sort( data + size1, size - size1, buf, order );
    bcore_u_memcpy( sizeof( st_s* ), buf, data, size1 );
    for( sz_t i = 0, w = 0, r = size1; i < size1; )
    {
        data[ w++ ] = ( r == size || bcore_strcmp( buf[ i ]->sc, data[ r ]->sc ) * order >= 0 ) ? buf[ i++ ] : data[ r++ ];
    }
}

void bcore_arr_st_s_sort( bcore_arr_st_s* o, s2_t order ) // stable mergesort
{
    if( o->space < o->size ) bcore_arr_st_s_make_strong( o );
    st_s** buf = bcore_u_alloc( sizeof( st_s* ), NULL, o->size >> 1, NULL );
    st_sort( o->data, o->size, buf, order );
    bcore_free( buf );
}

void bcore_arr_st_s_reorder( bcore_arr_st_s* o, const bcore_arr_sz_s* order )
{
    if( o->space < o->size ) bcore_arr_st_s_make_strong( o );
    sz_t buf_space = 0;
    st_s** buf = bcore_u_alloc( sizeof( st_s* ), NULL, order->size, &buf_space );
    for( sz_t i = 0; i < order->size; i++ )
    {
        assert( order->data[ i ] < o->size );
        buf[ i ] = bcore_fork( o->data[ order->data[ i ] ] );
    }
    for( sz_t i = 0; i < o->size; i++ ) st_s_discard( o->data[ i ] );
    bcore_un_alloc( sizeof( st_s* ), o->data, o->space, 0, &o->space );
    o->data = buf;
    o->space = buf_space;
    o->size = order->size;
}

/**********************************************************************************************************************/

// bcore_arr_vd_s

void bcore_arr_vd_s_init( bcore_arr_vd_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = TYPEOF_bcore_arr_vd_s;
}

void bcore_arr_vd_s_down( bcore_arr_vd_s* o )
{
    if( o->space > 0 ) bcore_un_alloc( sizeof( vd_t ), o->data, o->space, 0, NULL );
}

void bcore_arr_vd_s_copy( bcore_arr_vd_s* o, const bcore_arr_vd_s* src )
{
    if( o->space < src->size )
    {
        if( o->space > 0 ) o->data = bcore_un_alloc( sizeof( vd_t ), o->data, o->space, 0, &o->space );
        o->data = bcore_un_alloc( sizeof( vd_t ), NULL, o->space, src->size, &o->space );
    }
    bcore_u_memcpy( sizeof( vd_t ), o->data, src->data, src->size );
    o->size = src->size;
}

BCORE_DEFINE_FUNCTION_CREATE(  bcore_arr_vd_s )
BCORE_DEFINE_FUNCTION_DISCARD( bcore_arr_vd_s )
BCORE_DEFINE_FUNCTION_CLONE(   bcore_arr_vd_s )

static bcore_self_s* arr_vd_s_create_self( void )
{
    bcore_self_s* self = bcore_self_s_build_parse_sc( "bcore_arr_vd_s = bcore_array { aware_t _; vd_t [] arr; }", sizeof( bcore_arr_vd_s ) );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_vd_s_init, "bcore_fp_init", "init" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_vd_s_down, "bcore_fp_down", "down" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_vd_s_copy, "bcore_fp_copy", "copy" );
    return self;
}

void bcore_arr_vd_s_clear( bcore_arr_vd_s* o )
{
    o->size = 0;
    if( o->space == 0 ) o->data = NULL; // in case array is referencing external data
}

void bcore_arr_vd_s_set_space( bcore_arr_vd_s* o, sz_t space )
{
    if( o->space == 0 )
    {
        o->data = bcore_u_alloc( sizeof( vd_t ), NULL, space, &o->space );
    }
    else
    {
        o->data = bcore_un_alloc( sizeof( vd_t ), o->data, o->space, space, &o->space );
    }
    if( o->size > space ) o->size = space;
}

void bcore_arr_vd_s_fill( bcore_arr_vd_s* o, sz_t size, vd_t v )
{
    bcore_arr_vd_s_set_space( o, size );
    for( sz_t i = 0; i < size; i++ ) o->data[ i ] = v;
    o->size = size;
}

void bcore_arr_vd_s_make_strong( bcore_arr_vd_s* o )
{
    if( o->size > o->space )
    {
        vd_t* data = o->data;
        o->data = bcore_u_alloc( sizeof( vd_t ), NULL, o->size, &o->space );
        bcore_u_memcpy( sizeof( vd_t ), o->data, data, o->size );
    }
}

void bcore_arr_vd_s_push( bcore_arr_vd_s* o, vd_t v )
{
    if( o->size >  o->space ) bcore_arr_vd_s_make_strong( o );
    if( o->size == o->space )
    {
        o->data = bcore_un_alloc( sizeof( vd_t ), o->data, o->space, o->space > 0 ? o->space * 2 : 1, &o->space );
    }
    o->data[ o->size++ ] = v;
}

vd_t bcore_arr_vd_s_pop( bcore_arr_vd_s* o )
{
    if( o->size == 0 ) return 0;
    o->size--;
    return o->data[ o->size ];
}

void bcore_arr_vd_s_reorder( bcore_arr_vd_s* o, const bcore_arr_sz_s* order )
{
    if( o->space < o->size ) bcore_arr_vd_s_make_strong( o );
    sz_t buf_space = 0;
    vd_t* buf = bcore_u_alloc( sizeof( vd_t ), NULL, order->size, &buf_space );
    for( sz_t i = 0; i < order->size; i++ )
    {
        assert( order->data[ i ] < o->size );
        buf[ i ] = o->data[ order->data[ i ] ];
    }
    bcore_un_alloc( sizeof( vd_t ), o->data, o->space, 0, &o->space );
    o->data = buf;
    o->space = buf_space;
    o->size = order->size;
}

sz_t bcore_arr_vd_s_find( const bcore_arr_vd_s* o, sz_t start, sz_t end, vd_t v )
{
    if( end >= start )
    {
        sz_t end_l = end < o->size ? end : o->size;
        for( sz_t j = start; j < end_l; j++ ) if( o->data[ j ] == v ) return j;
        return end_l;
    }
    else
    {
        sz_t start_l = start < o->size ? start : o->size;
        for( sz_t j = start_l - 1;  j < start_l && j >= end; j-- ) if( o->data[ j ] == v ) return j;
        return start_l;
    }
}

/**********************************************************************************************************************/

// bcore_arr_fp_s

void bcore_arr_fp_s_init( bcore_arr_fp_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = TYPEOF_bcore_arr_fp_s;
}

void bcore_arr_fp_s_down( bcore_arr_fp_s* o )
{
    if( o->space > 0 ) bcore_un_alloc( sizeof( fp_t ), o->data, o->space, 0, NULL );
}

void bcore_arr_fp_s_copy( bcore_arr_fp_s* o, const bcore_arr_fp_s* src )
{
    if( o->space < src->size )
    {
        if( o->space > 0 ) o->data = bcore_un_alloc( sizeof( fp_t ), o->data, o->space, 0, &o->space );
        o->data = bcore_un_alloc( sizeof( fp_t ), NULL, o->space, src->size, &o->space );
    }
    bcore_u_memcpy( sizeof( fp_t ), o->data, src->data, src->size );
    o->size = src->size;
}

BCORE_DEFINE_FUNCTION_CREATE(  bcore_arr_fp_s )
BCORE_DEFINE_FUNCTION_DISCARD( bcore_arr_fp_s )
BCORE_DEFINE_FUNCTION_CLONE(   bcore_arr_fp_s )

static bcore_self_s* arr_fp_s_create_self( void )
{
    bcore_self_s* self = bcore_self_s_build_parse_sc( "bcore_arr_fp_s = bcore_array { aware_t _; fp_t [] arr; }", sizeof( bcore_arr_fp_s ) );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_fp_s_init, "bcore_fp_init", "init" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_fp_s_down, "bcore_fp_down", "down" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_fp_s_copy, "bcore_fp_copy", "copy" );
    return self;
}

void bcore_arr_fp_s_clear( bcore_arr_fp_s* o )
{
    o->size = 0;
    if( o->space == 0 ) o->data = NULL; // in case array is referencing external data
}

void bcore_arr_fp_s_set_space( bcore_arr_fp_s* o, sz_t space )
{
    if( o->space == 0 )
    {
        o->data = bcore_u_alloc( sizeof( fp_t ), NULL, space, &o->space );
    }
    else
    {
        o->data = bcore_un_alloc( sizeof( fp_t ), o->data, o->space, space, &o->space );
    }
    if( o->size > space ) o->size = space;
}

void bcore_arr_fp_s_fill( bcore_arr_fp_s* o, sz_t size, fp_t v )
{
    bcore_arr_fp_s_set_space( o, size );
    for( sz_t i = 0; i < size; i++ ) o->data[ i ] = v;
    o->size = size;
}

void bcore_arr_fp_s_make_strong( bcore_arr_fp_s* o )
{
    if( o->size > o->space )
    {
        fp_t* data = o->data;
        o->data = bcore_u_alloc( sizeof( fp_t ), NULL, o->size, &o->space );
        bcore_u_memcpy( sizeof( fp_t ), o->data, data, o->size );
    }
}

void bcore_arr_fp_s_push( bcore_arr_fp_s* o, fp_t v )
{
    if( o->size >  o->space ) bcore_arr_fp_s_make_strong( o );
    if( o->size == o->space )
    {
        o->data = bcore_un_alloc( sizeof( fp_t ), o->data, o->space, o->space > 0 ? o->space * 2 : 1, &o->space );
    }
    o->data[ o->size++ ] = v;
}

fp_t bcore_arr_fp_s_pop( bcore_arr_fp_s* o )
{
    if( o->size == 0 ) return 0;
    o->size--;
    return o->data[ o->size ];
}

void bcore_arr_fp_s_reorder( bcore_arr_fp_s* o, const bcore_arr_sz_s* order )
{
    if( o->space < o->size ) bcore_arr_fp_s_make_strong( o );
    sz_t buf_space = 0;
    fp_t* buf = bcore_u_alloc( sizeof( fp_t ), NULL, order->size, &buf_space );
    for( sz_t i = 0; i < order->size; i++ )
    {
        assert( order->data[ i ] < o->size );
        buf[ i ] = o->data[ order->data[ i ] ];
    }
    bcore_un_alloc( sizeof( fp_t ), o->data, o->space, 0, &o->space );
    o->data = buf;
    o->space = buf_space;
    o->size = order->size;
}

sz_t bcore_arr_fp_s_find( const bcore_arr_fp_s* o, sz_t start, sz_t end, fp_t v )
{
    if( end >= start )
    {
        sz_t end_l = end < o->size ? end : o->size;
        for( sz_t j = start; j < end_l; j++ ) if( o->data[ j ] == v ) return j;
        return end_l;
    }
    else
    {
        sz_t start_l = start < o->size ? start : o->size;
        for( sz_t j = start_l - 1;  j < start_l && j >= end; j-- ) if( o->data[ j ] == v ) return j;
        return start_l;
    }
}

/**********************************************************************************************************************/
// bcore_arr_sr_s

BCORE_DEFINE_FUNCTION_INIT_INST( bcore_arr_sr_s )
BCORE_DEFINE_FUNCTION_DOWN_INST( bcore_arr_sr_s )
BCORE_DEFINE_FUNCTION_COPY_INST( bcore_arr_sr_s )
BCORE_DEFINE_FUNCTION_CREATE(    bcore_arr_sr_s )
BCORE_DEFINE_FUNCTION_DISCARD(   bcore_arr_sr_s )
BCORE_DEFINE_FUNCTION_CLONE(     bcore_arr_sr_s )

static bcore_self_s* arr_sr_s_create_self( void )
{
    bcore_self_s* self = bcore_self_s_build_parse_sc( "bcore_arr_sr_s = bcore_array { aware_t _; sr_s [] arr; }", sizeof( bcore_arr_sr_s ) );
    return self;
}

void bcore_arr_sr_s_clear( bcore_arr_sr_s* o )
{
    if( o->space == 0 )
    {
        o->data = NULL; // in case array is referencing external data
        o->size = 0;
    }

    for( sz_t i = 0; i < o->size; i++ ) sr_down( o->data[ i ] );
    o->size = 0;
}

void bcore_arr_sr_s_set_space( bcore_arr_sr_s* o, sz_t space )
{
    bcore_array_a_set_space( (bcore_array*)o, space );
}

void bcore_arr_sr_s_set_size( bcore_arr_sr_s* o, sz_t size )
{
    bcore_array_a_set_size( (bcore_array*)o, size );
}

void bcore_arr_sr_s_make_strong( bcore_arr_sr_s* o )
{
    bcore_array_a_make_strong( (bcore_array*)o );
}

sr_s* bcore_arr_sr_s_push_sr( bcore_arr_sr_s* o, sr_s v )
{
    if( o->size > o->space ) bcore_arr_sr_s_make_strong( o );
    if( o->size == o->space )
    {
        o->data = bcore_un_alloc( sizeof( sr_s ), o->data, o->space, o->space > 0 ? o->space * 2 : 1, &o->space );
    }
    o->data[ o->size++ ] = v;
    return &o->data[ o->size - 1 ];
}

sr_s* bcore_arr_sr_s_push_tp( bcore_arr_sr_s* o, tp_t type )
{
    if( o->size > o->space ) bcore_arr_sr_s_make_strong( o );
    if( o->size == o->space )
    {
        o->data = bcore_un_alloc( sizeof( sr_s ), o->data, o->space, o->space > 0 ? o->space * 2 : 1, &o->space );
    }
    o->data[ o->size++ ] = sr_create( type );
    return &o->data[ o->size - 1 ];
}


sr_s bcore_arr_sr_s_pop( bcore_arr_sr_s* o )
{
    if( o->size == 0 ) return sr_null();
    o->size--;
    return o->data[ o->size ];
}

void bcore_arr_sr_s_sort( bcore_arr_sr_s* o, s2_t order )
{
    bcore_array_a_sort( (bcore_array*)o, 0, o->size, order );
}

void bcore_arr_sr_s_reorder( bcore_arr_sr_s* o, const bcore_arr_sz_s* order )
{
    bcore_array_a_reorder( (bcore_array*)o, order);
}

/**********************************************************************************************************************/
// selftests

#include <time.h>

st_s* bcore_arr_sz_selftest( void )
{
    st_s* log = st_s_createf( "== bcore_arr_sz_selftest " );
    st_s_push_char_n( log, '=', 120 - log->size );
    st_s_push_char( log, '\n' );

    bcore_arr_sz_s* sz_arr1 = bcore_arr_sz_s_create();
    bcore_arr_sz_s* sz_arr2 = bcore_arr_sz_s_create();
    sz_t size = 100000;
    {
        u2_t rval = 1;
        clock_t time = clock();
        for( sz_t i = 0; i < size; i++ )
        {
            rval = bcore_xsg_u2( rval );
            bcore_arr_sz_s_push( sz_arr1, rval );
        }
        time = clock() - time;
        st_s_pushf( log, "bcore_arr_sz_s_push ........ %zuns\n", ( sz_t ) ( ( 1E9 * ( f3_t )time/CLOCKS_PER_SEC ) / size ) );
    }

    const bcore_array_s* array_p = bcore_array_s_get_typed( TYPEOF_bcore_arr_sz_s );
    const bcore_inst_s* u2_t_p = bcore_inst_s_get_typed( TYPEOF_u2_t );

    {
        u2_t rval = 1;
        clock_t time = clock();
        for( sz_t i = 0; i < size; i++ )
        {
            rval = bcore_xsg_u2( rval );
            bcore_array_p_push( array_p, (bcore_array*)sz_arr2, sr_pwc( u2_t_p, &rval ) );
        }
        time = clock() - time;
        st_s_pushf( log, "bcore_array_p_push ..... %zuns\n", ( sz_t ) ( ( 1E9 * ( f3_t )time/CLOCKS_PER_SEC ) / size ) );
    }

    ASSERT( bcore_compare_aware( sz_arr1, sz_arr2 ) == 0 );

    {
        clock_t time = clock();
        bcore_arr_sz_s_sort( sz_arr1, 1 );
        time = clock() - time;
        st_s_pushf( log, "bcore_arr_sz_s_sort ........ %zuns\n", ( sz_t ) ( ( 1E9 * ( f3_t )time/CLOCKS_PER_SEC ) / size ) );
    }

    {
        clock_t time = clock();
        bcore_array_a_sort( (bcore_array*)sz_arr2, 0, -1, 1 );
        time = clock() - time;
        st_s_pushf( log, "bcore_array_a_sort ..... %zuns\n", ( sz_t ) ( ( 1E9 * ( f3_t )time/CLOCKS_PER_SEC ) / size ) );
    }

    bcore_arr_sz_s* permutation = bcore_arr_sz_s_create_random_permutation( bcore_xsg2_u2, 1, size );

    {
        clock_t time = clock();
        bcore_arr_sz_s_reorder( sz_arr1, permutation );
        time = clock() - time;
        st_s_pushf( log, "bcore_arr_sz_s_reorder ..... %zuns\n", ( sz_t ) ( ( 1E9 * ( f3_t )time/CLOCKS_PER_SEC ) / size ) );
    }

    {
        clock_t time = clock();
        bcore_array_a_reorder( (bcore_array*)sz_arr2, permutation );
        time = clock() - time;
        st_s_pushf( log, "bcore_array_a_reorder .. %zuns\n", ( sz_t ) ( ( 1E9 * ( f3_t )time/CLOCKS_PER_SEC ) / size ) );
    }

    ASSERT( bcore_compare_aware( sz_arr1, sz_arr2 ) == 0 );

//    bcore_txt_ml_to_stdout( sr_awc( sz_arr1 ) );

    bcore_arr_sz_s_discard( permutation );
    bcore_arr_sz_s_discard( sz_arr1 );
    bcore_arr_sz_s_discard( sz_arr2 );

    st_s_push_char_n( log, '=', 120 );
    st_s_push_char( log, '\n' );
    return log;

}

st_s* bcore_arr_st_selftest( void )
{
    st_s* log = st_s_createf( "== bcore_arr_st_selftest " );
    st_s_push_char_n( log, '=', 120 - log->size );
    st_s_push_char( log, '\n' );

    bcore_arr_st_s* st_arr1 = bcore_arr_st_s_create();
    bcore_arr_st_s* st_arr2 = bcore_arr_st_s_create();
    sz_t size = 100000;
    {
        u2_t rval = 1;
        clock_t time = clock();
        for( sz_t i = 0; i < size; i++ )
        {
            rval = bcore_xsg_u2( rval );
            bcore_arr_st_s_push_st_d( st_arr1, st_s_createf( "%u", rval ) );
        }
        time = clock() - time;
        st_s_pushf( log, "bcore_arr_st_s_push_st_d ... %zuns\n", ( sz_t ) ( ( 1E9 * ( f3_t )time/CLOCKS_PER_SEC ) / size ) );
    }

    {
        u2_t rval = 1;
        clock_t time = clock();
        for( sz_t i = 0; i < size; i++ )
        {
            rval = bcore_xsg_u2( rval );
            bcore_array_a_push( (bcore_array*)st_arr2, sr_asd( st_s_createf( "%u", rval ) ) );
        }
        time = clock() - time;
        st_s_pushf( log, "bcore_array_a_push ..... %zuns\n", ( sz_t ) ( ( 1E9 * ( f3_t )time/CLOCKS_PER_SEC ) / size ) );
    }

    {
        clock_t time = clock();
        bcore_arr_st_s_sort( st_arr1, 1 );
        time = clock() - time;
        st_s_pushf( log, "bcore_arr_st_s_sort ........ %zuns\n", ( sz_t ) ( ( 1E9 * ( f3_t )time/CLOCKS_PER_SEC ) / size ) );
    }

    {
        clock_t time = clock();
        bcore_array_a_sort( (bcore_array*)st_arr2, 0, -1, 1 );
        time = clock() - time;
        st_s_pushf( log, "bcore_array_a_sort ..... %zuns\n", ( sz_t ) ( ( 1E9 * ( f3_t )time/CLOCKS_PER_SEC ) / size ) );
    }

    bcore_arr_sz_s* permutation = bcore_arr_sz_s_create_random_permutation( bcore_xsg2_u2, 1, size );

    {
        clock_t time = clock();
        bcore_arr_st_s_reorder( st_arr1, permutation );
        time = clock() - time;
        st_s_pushf( log, "bcore_arr_st_s_reorder ..... %zuns\n", ( sz_t ) ( ( 1E9 * ( f3_t )time/CLOCKS_PER_SEC ) / size ) );
    }

    {
        clock_t time = clock();
        bcore_array_a_reorder( (bcore_array*)st_arr2, permutation );
        time = clock() - time;
        st_s_pushf( log, "bcore_array_a_reorder .. %zuns\n", ( sz_t ) ( ( 1E9 * ( f3_t )time/CLOCKS_PER_SEC ) / size ) );
    }

    ASSERT( bcore_compare_aware( st_arr1, st_arr2 ) == 0 );

//    bcore_txt_ml_to_stdout( sr_awc( st_arr1 ) );

    bcore_arr_sz_s_discard( permutation );
    bcore_arr_st_s_discard( st_arr1 );
    bcore_arr_st_s_discard( st_arr2 );

    st_s_push_char_n( log, '=', 120 );
    st_s_push_char( log, '\n' );
    return log;

}

/**********************************************************************************************************************/

vd_t bcore_arr_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_arr" ) ) )
    {
        case TYPEOF_init1:
        {
            bcore_flect_define_creator( typeof( "bcore_arr_sz_s" ), arr_sz_s_create_self );
            bcore_flect_define_creator( typeof( "bcore_arr_u3_s" ), arr_u3_s_create_self );
            bcore_flect_define_creator( typeof( "bcore_arr_tp_s" ), arr_tp_s_create_self );
            bcore_flect_define_creator( typeof( "bcore_arr_st_s" ), arr_st_s_create_self );
            bcore_flect_define_creator( typeof( "bcore_arr_vd_s" ), arr_vd_s_create_self );
            bcore_flect_define_creator( typeof( "bcore_arr_fp_s" ), arr_fp_s_create_self );
            bcore_flect_define_creator( typeof( "bcore_arr_sr_s" ), arr_sr_s_create_self );
        }
        break;

        case TYPEOF_selftest:
        {
            st_s* log = st_s_create();
            st_s_push_st_d( log, bcore_arr_sz_selftest() );
            st_s_push_st_d( log, bcore_arr_st_selftest() );
            return log;
        }
        break;

        default: break;
    }

    return NULL;
}
