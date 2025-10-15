/** Author and Copyright 2017 Johannes Bernhard Steffens
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
#include "bcore_spect_sink.h"
#include "bcore_sr.h"
#include "bcore_spect_compare.h"
#include "bcore_signal.h"
#include "bcore_leaf.h"

/**********************************************************************************************************************/
// bcore_arr_uz_s

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_uz_s_init( bcore_arr_uz_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = TYPEOF_bcore_arr_uz_s;
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_uz_s_down( bcore_arr_uz_s* o )
{
    if( o->space > 0 ) bcore_un_alloc( sizeof( uz_t ), o->data, o->space, 0, NULL );
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_uz_s_copy( bcore_arr_uz_s* o, const bcore_arr_uz_s* src )
{
    if( o->space < src->size )
    {
        if( o->space > 0 ) o->data = bcore_un_alloc( sizeof( uz_t ), o->data, o->space, 0, &o->space );
        o->data = bcore_un_alloc( sizeof( uz_t ), NULL, o->space, src->size, &o->space );
    }
    bcore_u_memcpy( sizeof( uz_t ), o->data, src->data, src->size );
    o->size = src->size;
}

// ---------------------------------------------------------------------------------------------------------------------

BCORE_DEFINE_FUNCTION_CREATE(  bcore_arr_uz_s )
BCORE_DEFINE_FUNCTION_DISCARD( bcore_arr_uz_s )
BCORE_DEFINE_FUNCTION_CLONE(   bcore_arr_uz_s )

// ---------------------------------------------------------------------------------------------------------------------

static bcore_self_s* arr_uz_s_create_self( void )
{
    bcore_self_s* self = BCORE_SELF_S_BUILD_PARSE_SC( "bcore_arr_uz_s = bcore_array { aware_t _; uz_t [] arr; }", bcore_arr_uz_s );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_uz_s_init, "bcore_fp_init", "init" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_uz_s_down, "bcore_fp_down", "down" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_uz_s_copy, "bcore_fp_copy", "copy" );
    return self;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_uz_s* bcore_arr_uz_s_clear( bcore_arr_uz_s* o )
{
    o->size = 0;
    if( o->space == 0 ) o->data = NULL; // in case array is referencing external data
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_uz_s* bcore_arr_uz_s_set_space( bcore_arr_uz_s* o, uz_t space )
{
    if( o->space == 0 )
    {
        o->data = bcore_u_alloc( sizeof( uz_t ), NULL, space, &o->space );
    }
    else
    {
        o->data = bcore_un_alloc( sizeof( uz_t ), o->data, o->space, space, &o->space );
    }
    if( o->size > space ) o->size = space;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_uz_s* bcore_arr_uz_s_fill( bcore_arr_uz_s* o, uz_t size, uz_t v )
{
    bcore_arr_uz_s_set_space( o, size );
    for( uz_t i = 0; i < size; i++ ) o->data[ i ] = v;
    o->size = size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_uz_s* bcore_arr_uz_s_set_size( bcore_arr_uz_s* o, uz_t size )
{
    bcore_arr_uz_s_set_space( o, size );
    if( size > o->size )
    {
        for( sz_t i = o->size; i < size; i++ ) o->data[ i ] = 0;
    }
    o->size = size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_uz_s* bcore_arr_uz_s_step_fill( bcore_arr_uz_s* o, uz_t v_start, s3_t step, uz_t size )
{
    bcore_arr_uz_s_set_space( o, size );
    uz_t v = v_start;
    for( uz_t i = 0; i < size; i++ )
    {
        o->data[ i ] = v;
        v += step;
    }
    o->size = size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_uz_s_make_strong( bcore_arr_uz_s* o )
{
    if( o->size > o->space )
    {
        uz_t* data = o->data;
        o->data = bcore_u_alloc( sizeof( uz_t ), NULL, o->size, &o->space );
        bcore_u_memcpy( sizeof( uz_t ), o->data, data, o->size );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_uz_s_push( bcore_arr_uz_s* o, uz_t v )
{
    if( o->size >  o->space ) bcore_arr_uz_s_make_strong( o );
    if( o->size == o->space )
    {
        o->data = bcore_un_alloc( sizeof( uz_t ), o->data, o->space, o->space > 0 ? o->space * 2 : 1, &o->space );
    }
    o->data[ o->size++ ] = v;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_uz_s* bcore_arr_uz_s_push_left( bcore_arr_uz_s* o, uz_t v )
{
    bcore_arr_uz_s_push( o, 0 );
    for( sz_t i = o->size - 1; i > 0; i-- ) o->data[ i ] = o->data[ i - 1 ];
    o->data[ 0 ] = v;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_uz_s* bcore_arr_uz_s_push_arr( bcore_arr_uz_s* o, const bcore_arr_uz_s* arr )
{
    if( arr->size == 0 ) return o;
    if( o->size > o->space ) bcore_arr_uz_s_make_strong( o );
    if( o->size + arr->size >= o->space )
    {
        uz_t new_space = uz_max( o->space * 2, o->size + arr->size );
        o->data = bcore_un_alloc( sizeof( uz_t ), o->data, o->space, new_space, &o->space );
    }
    for( sz_t i = 0; i < arr->size; i++ ) o->data[ o->size++ ] = arr->data[ i ];
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_arr_uz_s_pop( bcore_arr_uz_s* o )
{
    if( o->size == 0 ) return 0;
    o->size--;
    return o->data[ o->size ];
}

// ---------------------------------------------------------------------------------------------------------------------

s2_t bcore_arr_uz_s_cmp( const bcore_arr_uz_s* o, const bcore_arr_uz_s* b )
{
    if( o->size == 0 ) return ( b->size == 0 ) ? 0 :  1;
    if( b->size == 0 ) return -1;
    uz_t min_size = uz_min( o->size, b->size );
    for( sz_t i = 0; i < min_size; i++ )
    {
        if( o->data[ i ] != b->data[ i ] ) return ( o->data[ i ] < b->data[ i ] ) ? 2 : -2;
    }
    return ( o->size == b->size ) ? 0 : ( o->size < b->size ) ? +1 : -1;
}

// ---------------------------------------------------------------------------------------------------------------------

static void uz_sort( uz_t* data, uz_t size, uz_t* buf, s2_t order )
{
    if( size < 2 ) return;
    uz_t size1 = size >> 1;
    uz_sort( data,         size1       , buf, order );
    uz_sort( data + size1, size - size1, buf, order );
    bcore_u_memcpy( sizeof( uz_t ), buf, data, size1 );
    if( order > 0 )
    {
        for( uz_t i = 0, w = 0, r = size1; i < size1; )
        {
            data[ w++ ] = ( r == size || buf[ i ] <= data[ r ] ) ? buf[ i++ ] : data[ r++ ];
        }
    }
    else
    {
        for( uz_t i = 0, w = 0, r = size1; i < size1; )
        {
            data[ w++ ] = ( r == size || buf[ i ] >= data[ r ] ) ? buf[ i++ ] : data[ r++ ];
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_uz_s* bcore_arr_uz_s_sort( bcore_arr_uz_s* o, s2_t order ) // stable mergesort
{
    if( o->space < o->size ) bcore_arr_uz_s_make_strong( o );
    uz_t* buf = bcore_u_alloc( sizeof( uz_t ), NULL, o->size >> 1, NULL );
    uz_sort( o->data, o->size, buf, order );
    bcore_free( buf );
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_uz_s* bcore_arr_uz_s_reorder( bcore_arr_uz_s* o, const bcore_arr_uz_s* order )
{
    if( o->space < o->size ) bcore_arr_uz_s_make_strong( o );
    uz_t buf_space = 0;
    uz_t* buf = bcore_u_alloc( sizeof( uz_t ), NULL, order->size, &buf_space );
    for( uz_t i = 0; i < order->size; i++ )
    {
        assert( order->data[ i ] < o->size );
        buf[ i ] = o->data[ order->data[ i ] ];
    }
    bcore_un_alloc( sizeof( uz_t ), o->data, o->space, 0, &o->space );
    o->data = buf;
    o->space = buf_space;
    o->size = order->size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_uz_s* bcore_arr_uz_s_create_random_permutation( u2_t ( *rg )( u2_t ), u2_t rval, uz_t size )
{
    bcore_arr_uz_s* o = bcore_arr_uz_s_create();
    if( size == 0 ) return o;

    bcore_arr_uz_s_step_fill( o, 0, 1, size );
    uz_t idx = 0;

    for( uz_t i = 0; i < size; i++ )
    {
        idx = i + ( ( idx + ( rval = rg( rval ) ) ) % ( size - i ) );
        uz_t t = o->data[ i ];
        o->data[ i ] = o->data[ idx ];
        o->data[ idx ] = t;
    }

    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_arr_uz_s_find( const bcore_arr_uz_s* o, uz_t start, uz_t end, uz_t v )
{
    if( end >= start )
    {
        uz_t end_l = end < o->size ? end : o->size;
        for( uz_t j = start; j < end_l; j++ ) if( o->data[ j ] == v ) return j;
        return end_l;
    }
    else
    {
        uz_t start_l = start < o->size ? start : o->size;
        for( uz_t j = start_l - 1;  j < start_l && j >= end; j-- ) if( o->data[ j ] == v ) return j;
        return start_l;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_arr_uz_s_max( const bcore_arr_uz_s* o )
{
    if( o->size == 0 ) return 0;
    uz_t max = o->data[ 0 ];
    for( uz_t i = 1; i < o->size; i++ ) max = o->data[ i ] > max ? o->data[ i ] : max;
    return max;
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_arr_uz_s_min( const bcore_arr_uz_s* o )
{
    if( o->size == 0 ) return 0;
    uz_t min = o->data[ 0 ];
    for( uz_t i = 1; i < o->size; i++ ) min = o->data[ i ] < min ? o->data[ i ] : min;
    return min;
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_arr_uz_s_idx_max( const bcore_arr_uz_s* o )
{
    if( o->size == 0 ) return 0;
    uz_t max = o->data[ 0 ];
    uz_t idx = 0;
    for( uz_t i = 1; i < o->size; i++ )
    {
        if( o->data[ i ] > max )
        {
            max = o->data[ i ];
            idx = i;
        }
    }
    return idx;
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_arr_uz_s_idx_min( const bcore_arr_uz_s* o )
{
    if( o->size == 0 ) return 0;
    uz_t min = o->data[ 0 ];
    uz_t idx = 0;
    for( uz_t i = 1; i < o->size; i++ )
    {
        if( o->data[ i ] < min )
        {
            min = o->data[ i ];
            idx = i;
        }
    }
    return idx;
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_arr_uz_s_count_equal( const bcore_arr_uz_s* o, uz_t val )
{
    uz_t count = 0;
    for( uz_t i = 0; i < o->size; i++ ) count += ( o->data[ i ] == val );
    return count;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// bcore_arr_sz_s

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_sz_s_init( bcore_arr_sz_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = TYPEOF_bcore_arr_sz_s;
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_sz_s_down( bcore_arr_sz_s* o )
{
    if( o->space > 0 ) bcore_un_alloc( sizeof( sz_t ), o->data, o->space, 0, NULL );
}

// ---------------------------------------------------------------------------------------------------------------------

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

// ---------------------------------------------------------------------------------------------------------------------

BCORE_DEFINE_FUNCTION_CREATE(  bcore_arr_sz_s )
BCORE_DEFINE_FUNCTION_DISCARD( bcore_arr_sz_s )
BCORE_DEFINE_FUNCTION_CLONE(   bcore_arr_sz_s )

// ---------------------------------------------------------------------------------------------------------------------

static bcore_self_s* arr_sz_s_create_self( void )
{
    bcore_self_s* self = BCORE_SELF_S_BUILD_PARSE_SC( "bcore_arr_sz_s = bcore_array { aware_t _; sz_t [] arr; }", bcore_arr_sz_s );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_sz_s_init, "bcore_fp_init", "init" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_sz_s_down, "bcore_fp_down", "down" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_sz_s_copy, "bcore_fp_copy", "copy" );
    return self;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_sz_s* bcore_arr_sz_s_clear( bcore_arr_sz_s* o )
{
    o->size = 0;
    if( o->space == 0 ) o->data = NULL; // in case array is referencing external data
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_sz_s* bcore_arr_sz_s_set_space( bcore_arr_sz_s* o, uz_t space )
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
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_sz_s* bcore_arr_sz_s_fill( bcore_arr_sz_s* o, uz_t size, sz_t v )
{
    bcore_arr_sz_s_set_space( o, size );
    for( uz_t i = 0; i < size; i++ ) o->data[ i ] = v;
    o->size = size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_sz_s* bcore_arr_sz_s_set_size( bcore_arr_sz_s* o, uz_t size )
{
    bcore_arr_sz_s_set_space( o, size );
    if( size > o->size )
    {
        for( sz_t i = o->size; i < size; i++ ) o->data[ i ] = 0;
    }
    o->size = size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_sz_s* bcore_arr_sz_s_step_fill( bcore_arr_sz_s* o, sz_t v_start, sz_t step, uz_t size )
{
    bcore_arr_sz_s_set_space( o, size );
    sz_t v = v_start;
    for( uz_t i = 0; i < size; i++ )
    {
        o->data[ i ] = v;
        v += step;
    }
    o->size = size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_sz_s_make_strong( bcore_arr_sz_s* o )
{
    if( o->size > o->space )
    {
        sz_t* data = o->data;
        o->data = bcore_u_alloc( sizeof( sz_t ), NULL, o->size, &o->space );
        bcore_u_memcpy( sizeof( sz_t ), o->data, data, o->size );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_sz_s_push( bcore_arr_sz_s* o, sz_t v )
{
    if( o->size >  o->space ) bcore_arr_sz_s_make_strong( o );
    if( o->size == o->space )
    {
        o->data = bcore_un_alloc( sizeof( sz_t ), o->data, o->space, o->space > 0 ? o->space * 2 : 1, &o->space );
    }
    o->data[ o->size++ ] = v;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_sz_s* bcore_arr_sz_s_push_left( bcore_arr_sz_s* o, sz_t v )
{
    bcore_arr_sz_s_push( o, 0 );
    for( sz_t i = o->size - 1; i > 0; i-- ) o->data[ i ] = o->data[ i - 1 ];
    o->data[ 0 ] = v;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_sz_s* bcore_arr_sz_s_push_arr( bcore_arr_sz_s* o, const bcore_arr_sz_s* arr )
{
    if( arr->size == 0 ) return o;
    if( o->size > o->space ) bcore_arr_sz_s_make_strong( o );
    if( o->size + arr->size >= o->space )
    {
        uz_t new_space = uz_max( o->space * 2, o->size + arr->size );
        o->data = bcore_un_alloc( sizeof( tp_t ), o->data, o->space, new_space, &o->space );
    }
    for( sz_t i = 0; i < arr->size; i++ ) o->data[ o->size++ ] = arr->data[ i ];
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

sz_t bcore_arr_sz_s_pop( bcore_arr_sz_s* o )
{
    if( o->size == 0 ) return 0;
    o->size--;
    return o->data[ o->size ];
}

// ---------------------------------------------------------------------------------------------------------------------

s2_t bcore_arr_sz_s_cmp( const bcore_arr_sz_s* o, const bcore_arr_sz_s* b )
{
    if( o->size == 0 ) return ( b->size == 0 ) ? 0 :  1;
    if( b->size == 0 ) return -1;
    uz_t min_size = uz_min( o->size, b->size );
    for( sz_t i = 0; i < min_size; i++ )
    {
        if( o->data[ i ] != b->data[ i ] ) return ( o->data[ i ] < b->data[ i ] ) ? 2 : -2;
    }
    return ( o->size == b->size ) ? 0 : ( o->size < b->size ) ? +1 : -1;
}

// ---------------------------------------------------------------------------------------------------------------------

static void sz_sort( sz_t* data, uz_t size, sz_t* buf, s2_t order )
{
    if( size < 2 ) return;
    uz_t size1 = size >> 1;
    sz_sort( data,         size1       , buf, order );
    sz_sort( data + size1, size - size1, buf, order );
    bcore_u_memcpy( sizeof( sz_t ), buf, data, size1 );
    if( order > 0 )
    {
        for( uz_t i = 0, w = 0, r = size1; i < size1; )
        {
            data[ w++ ] = ( r == size || buf[ i ] <= data[ r ] ) ? buf[ i++ ] : data[ r++ ];
        }
    }
    else
    {
        for( uz_t i = 0, w = 0, r = size1; i < size1; )
        {
            data[ w++ ] = ( r == size || buf[ i ] >= data[ r ] ) ? buf[ i++ ] : data[ r++ ];
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_sz_s* bcore_arr_sz_s_sort( bcore_arr_sz_s* o, s2_t order ) // stable mergesort
{
    if( o->space < o->size ) bcore_arr_sz_s_make_strong( o );
    sz_t* buf = bcore_u_alloc( sizeof( sz_t ), NULL, o->size >> 1, NULL );
    sz_sort( o->data, o->size, buf, order );
    bcore_free( buf );
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_sz_s* bcore_arr_sz_s_reorder( bcore_arr_sz_s* o, const bcore_arr_uz_s* order )
{
    if( o->space < o->size ) bcore_arr_sz_s_make_strong( o );
    uz_t buf_space = 0;
    sz_t* buf = bcore_u_alloc( sizeof( sz_t ), NULL, order->size, &buf_space );
    for( uz_t i = 0; i < order->size; i++ )
    {
        assert( order->data[ i ] < o->size );
        buf[ i ] = o->data[ order->data[ i ] ];
    }
    bcore_un_alloc( sizeof( sz_t ), o->data, o->space, 0, &o->space );
    o->data = buf;
    o->space = buf_space;
    o->size = order->size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_arr_sz_s_find( const bcore_arr_sz_s* o, uz_t start, uz_t end, sz_t v )
{
    if( end >= start )
    {
        uz_t end_l = end < o->size ? end : o->size;
        for( uz_t j = start; j < end_l; j++ ) if( o->data[ j ] == v ) return j;
        return end_l;
    }
    else
    {
        uz_t start_l = start < o->size ? start : o->size;
        for( uz_t j = start_l - 1;  j < start_l && j >= end; j-- ) if( o->data[ j ] == v ) return j;
        return start_l;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

sz_t bcore_arr_sz_s_max( const bcore_arr_sz_s* o )
{
    if( o->size == 0 ) return 0;
    sz_t max = o->data[ 0 ];
    for( uz_t i = 1; i < o->size; i++ ) max = o->data[ i ] > max ? o->data[ i ] : max;
    return max;
}

// ---------------------------------------------------------------------------------------------------------------------

sz_t bcore_arr_sz_s_min( const bcore_arr_sz_s* o )
{
    if( o->size == 0 ) return 0;
    sz_t min = o->data[ 0 ];
    for( uz_t i = 1; i < o->size; i++ ) min = o->data[ i ] < min ? o->data[ i ] : min;
    return min;
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_arr_sz_s_idx_max( const bcore_arr_sz_s* o )
{
    if( o->size == 0 ) return 0;
    sz_t max = o->data[ 0 ];
    uz_t idx = 0;
    for( uz_t i = 1; i < o->size; i++ )
    {
        if( o->data[ i ] > max )
        {
            max = o->data[ i ];
            idx = i;
        }
    }
    return idx;
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_arr_sz_s_idx_min( const bcore_arr_sz_s* o )
{
    if( o->size == 0 ) return 0;
    sz_t min = o->data[ 0 ];
    uz_t idx = 0;
    for( uz_t i = 1; i < o->size; i++ )
    {
        if( o->data[ i ] < min )
        {
            min = o->data[ i ];
            idx = i;
        }
    }
    return idx;
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_arr_sz_s_count_equal( const bcore_arr_sz_s* o, sz_t val )
{
    uz_t count = 0;
    for( uz_t i = 0; i < o->size; i++ ) count += ( o->data[ i ] == val );
    return count;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// bcore_arr_u0_s

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_u0_s_init( bcore_arr_u0_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = TYPEOF_bcore_arr_u0_s;
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_u0_s_down( bcore_arr_u0_s* o )
{
    if( o->space > 0 ) bcore_un_alloc( sizeof( s3_t ), o->data, o->space, 0, NULL );
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_u0_s_copy( bcore_arr_u0_s* o, const bcore_arr_u0_s* src )
{
    if( o->space < src->size )
    {
        if( o->space > 0 ) o->data = bcore_un_alloc( sizeof( u0_t ), o->data, o->space, 0, &o->space );
        o->data = bcore_un_alloc( sizeof( u0_t ), NULL, o->space, src->size, &o->space );
    }
    bcore_u_memcpy( sizeof( u0_t ), o->data, src->data, src->size );
    o->size = src->size;
}

// ---------------------------------------------------------------------------------------------------------------------

BCORE_DEFINE_FUNCTION_CREATE(  bcore_arr_u0_s )
BCORE_DEFINE_FUNCTION_DISCARD( bcore_arr_u0_s )
BCORE_DEFINE_FUNCTION_CLONE(   bcore_arr_u0_s )

// ---------------------------------------------------------------------------------------------------------------------

static bcore_self_s* arr_u0_s_create_self( void )
{
    bcore_self_s* self = BCORE_SELF_S_BUILD_PARSE_SC( "bcore_arr_u0_s = bcore_array { aware_t _; u0_t [] arr; }", bcore_arr_u0_s );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_u0_s_init, "bcore_fp_init", "init" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_u0_s_down, "bcore_fp_down", "down" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_u0_s_copy, "bcore_fp_copy", "copy" );
    return self;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_u0_s* bcore_arr_u0_s_clear( bcore_arr_u0_s* o )
{
    o->size = 0;
    if( o->space == 0 ) o->data = NULL; // in case array is referencing external data
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_u0_s* bcore_arr_u0_s_set_space( bcore_arr_u0_s* o, uz_t space )
{
    if( o->space == 0 )
    {
        o->data = bcore_u_alloc( sizeof( u0_t ), NULL, space, &o->space );
    }
    else
    {
        o->data = bcore_un_alloc( sizeof( u0_t ), o->data, o->space, space, &o->space );
    }
    if( o->size > space ) o->size = space;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_u0_s* bcore_arr_u0_s_fill( bcore_arr_u0_s* o, uz_t size, u0_t v )
{
    bcore_arr_u0_s_set_space( o, size );
    for( uz_t i = 0; i < size; i++ ) o->data[ i ] = v;
    o->size = size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_u0_s* bcore_arr_u0_s_set_size( bcore_arr_u0_s* o, uz_t size )
{
    bcore_arr_u0_s_set_space( o, size );
    if( size > o->size )
    {
        for( sz_t i = o->size; i < size; i++ ) o->data[ i ] = 0;
    }
    o->size = size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_u0_s* bcore_arr_u0_s_step_fill( bcore_arr_u0_s* o, u0_t v_start, s3_t step, uz_t size )
{
    bcore_arr_u0_s_set_space( o, size );
    u0_t v = v_start;
    for( uz_t i = 0; i < size; i++ )
    {
        o->data[ i ] = v;
        v += step;
    }
    o->size = size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_u0_s_make_strong( bcore_arr_u0_s* o )
{
    if( o->size > o->space )
    {
        u0_t* data = o->data;
        o->data = bcore_u_alloc( sizeof( u0_t ), NULL, o->size, &o->space );
        bcore_u_memcpy( sizeof( u0_t ), o->data, data, o->size );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_u0_s* bcore_arr_u0_s_push( bcore_arr_u0_s* o, u0_t v )
{
    if( o->size >  o->space ) bcore_arr_u0_s_make_strong( o );
    if( o->size == o->space )
    {
        o->data = bcore_un_alloc( sizeof( u0_t ), o->data, o->space, o->space > 0 ? o->space * 2 : 1, &o->space );
    }
    o->data[ o->size++ ] = v;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_u0_s* bcore_arr_u0_s_push_left( bcore_arr_u0_s* o, u0_t v )
{
    bcore_arr_u0_s_push( o, 0 );
    for( sz_t i = o->size - 1; i > 0; i-- ) o->data[ i ] = o->data[ i - 1 ];
    o->data[ 0 ] = v;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_u0_s* bcore_arr_u0_s_push_arr( bcore_arr_u0_s* o, const bcore_arr_u0_s* arr )
{
    if( arr->size == 0 ) return o;
    if( o->size > o->space ) bcore_arr_u0_s_make_strong( o );
    if( o->size + arr->size >= o->space )
    {
        uz_t new_space = uz_max( o->space * 2, o->size + arr->size );
        o->data = bcore_un_alloc( sizeof( tp_t ), o->data, o->space, new_space, &o->space );
    }
    for( sz_t i = 0; i < arr->size; i++ ) o->data[ o->size++ ] = arr->data[ i ];
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

u0_t bcore_arr_u0_s_pop( bcore_arr_u0_s* o )
{
    if( o->size == 0 ) return 0;
    o->size--;
    return o->data[ o->size ];
}

// ---------------------------------------------------------------------------------------------------------------------

s2_t bcore_arr_u0_s_cmp( const bcore_arr_u0_s* o, const bcore_arr_u0_s* b )
{
    if( o->size == 0 ) return ( b->size == 0 ) ? 0 :  1;
    if( b->size == 0 ) return -1;
    uz_t min_size = uz_min( o->size, b->size );
    for( sz_t i = 0; i < min_size; i++ )
    {
        if( o->data[ i ] != b->data[ i ] ) return ( o->data[ i ] < b->data[ i ] ) ? 2 : -2;
    }
    return ( o->size == b->size ) ? 0 : ( o->size < b->size ) ? +1 : -1;
}

// ---------------------------------------------------------------------------------------------------------------------

static void u0_sort( u0_t* data, uz_t size, u0_t* buf, s2_t order )
{
    if( size < 2 ) return;
    uz_t size1 = size >> 1;
    u0_sort( data,         size1       , buf, order );
    u0_sort( data + size1, size - size1, buf, order );
    bcore_u_memcpy( sizeof( u0_t ), buf, data, size1 );
    if( order > 0 )
    {
        for( uz_t i = 0, w = 0, r = size1; i < size1; )
        {
            data[ w++ ] = ( r == size || buf[ i ] <= data[ r ] ) ? buf[ i++ ] : data[ r++ ];
        }
    }
    else
    {
        for( uz_t i = 0, w = 0, r = size1; i < size1; )
        {
            data[ w++ ] = ( r == size || buf[ i ] >= data[ r ] ) ? buf[ i++ ] : data[ r++ ];
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_u0_s* bcore_arr_u0_s_sort( bcore_arr_u0_s* o, s2_t order ) // stable mergesort
{
    if( o->space < o->size ) bcore_arr_u0_s_make_strong( o );
    u0_t* buf = bcore_u_alloc( sizeof( u0_t ), NULL, o->size >> 1, NULL );
    u0_sort( o->data, o->size, buf, order );
    bcore_free( buf );
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_u0_s* bcore_arr_u0_s_reorder( bcore_arr_u0_s* o, const bcore_arr_uz_s* order )
{
    if( o->space < o->size ) bcore_arr_u0_s_make_strong( o );
    uz_t buf_space = 0;
    u0_t* buf = bcore_u_alloc( sizeof( u0_t ), NULL, order->size, &buf_space );
    for( uz_t i = 0; i < order->size; i++ )
    {
        assert( order->data[ i ] < o->size );
        buf[ i ] = o->data[ order->data[ i ] ];
    }
    bcore_un_alloc( sizeof( u0_t ), o->data, o->space, 0, &o->space );
    o->data = buf;
    o->space = buf_space;
    o->size = order->size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_arr_u0_s_count_equal( const bcore_arr_u0_s* o, u0_t val )
{
    uz_t count = 0;
    for( uz_t i = 0; i < o->size; i++ ) count += ( o->data[ i ] == val );
    return count;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// bcore_arr_u3_s

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_u3_s_init( bcore_arr_u3_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = TYPEOF_bcore_arr_u3_s;
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_u3_s_down( bcore_arr_u3_s* o )
{
    if( o->space > 0 ) bcore_un_alloc( sizeof( s3_t ), o->data, o->space, 0, NULL );
}

// ---------------------------------------------------------------------------------------------------------------------

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

// ---------------------------------------------------------------------------------------------------------------------

BCORE_DEFINE_FUNCTION_CREATE(  bcore_arr_u3_s )
BCORE_DEFINE_FUNCTION_DISCARD( bcore_arr_u3_s )
BCORE_DEFINE_FUNCTION_CLONE(   bcore_arr_u3_s )

// ---------------------------------------------------------------------------------------------------------------------

static bcore_self_s* arr_u3_s_create_self( void )
{
    bcore_self_s* self = BCORE_SELF_S_BUILD_PARSE_SC( "bcore_arr_u3_s = bcore_array { aware_t _; u3_t [] arr; }", bcore_arr_u3_s );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_u3_s_init, "bcore_fp_init", "init" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_u3_s_down, "bcore_fp_down", "down" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_u3_s_copy, "bcore_fp_copy", "copy" );
    return self;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_u3_s* bcore_arr_u3_s_clear( bcore_arr_u3_s* o )
{
    o->size = 0;
    if( o->space == 0 ) o->data = NULL; // in case array is referencing external data
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_u3_s* bcore_arr_u3_s_set_space( bcore_arr_u3_s* o, uz_t space )
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
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_u3_s* bcore_arr_u3_s_fill( bcore_arr_u3_s* o, uz_t size, u3_t v )
{
    bcore_arr_u3_s_set_space( o, size );
    for( uz_t i = 0; i < size; i++ ) o->data[ i ] = v;
    o->size = size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_u3_s* bcore_arr_u3_s_set_size( bcore_arr_u3_s* o, uz_t size )
{
    bcore_arr_u3_s_set_space( o, size );
    if( size > o->size )
    {
        for( sz_t i = o->size; i < size; i++ ) o->data[ i ] = 0;
    }
    o->size = size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_u3_s* bcore_arr_u3_s_step_fill( bcore_arr_u3_s* o, u3_t v_start, s3_t step, uz_t size )
{
    bcore_arr_u3_s_set_space( o, size );
    u3_t v = v_start;
    for( uz_t i = 0; i < size; i++ )
    {
        o->data[ i ] = v;
        v += step;
    }
    o->size = size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_u3_s_make_strong( bcore_arr_u3_s* o )
{
    if( o->size > o->space )
    {
        u3_t* data = o->data;
        o->data = bcore_u_alloc( sizeof( u3_t ), NULL, o->size, &o->space );
        bcore_u_memcpy( sizeof( u3_t ), o->data, data, o->size );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_u3_s* bcore_arr_u3_s_push( bcore_arr_u3_s* o, u3_t v )
{
    if( o->size >  o->space ) bcore_arr_u3_s_make_strong( o );
    if( o->size == o->space )
    {
        o->data = bcore_un_alloc( sizeof( u3_t ), o->data, o->space, o->space > 0 ? o->space * 2 : 1, &o->space );
    }
    o->data[ o->size++ ] = v;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_u3_s* bcore_arr_u3_s_push_left( bcore_arr_u3_s* o, u3_t v )
{
    bcore_arr_u3_s_push( o, 0 );
    for( sz_t i = o->size - 1; i > 0; i-- ) o->data[ i ] = o->data[ i - 1 ];
    o->data[ 0 ] = v;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_u3_s* bcore_arr_u3_s_push_arr( bcore_arr_u3_s* o, const bcore_arr_u3_s* arr )
{
    if( arr->size == 0 ) return o;
    if( o->size > o->space ) bcore_arr_u3_s_make_strong( o );
    if( o->size + arr->size >= o->space )
    {
        uz_t new_space = uz_max( o->space * 2, o->size + arr->size );
        o->data = bcore_un_alloc( sizeof( tp_t ), o->data, o->space, new_space, &o->space );
    }
    for( sz_t i = 0; i < arr->size; i++ ) o->data[ o->size++ ] = arr->data[ i ];
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

u3_t bcore_arr_u3_s_pop( bcore_arr_u3_s* o )
{
    if( o->size == 0 ) return 0;
    o->size--;
    return o->data[ o->size ];
}

// ---------------------------------------------------------------------------------------------------------------------

s2_t bcore_arr_u3_s_cmp( const bcore_arr_u3_s* o, const bcore_arr_u3_s* b )
{
    if( o->size == 0 ) return ( b->size == 0 ) ? 0 :  1;
    if( b->size == 0 ) return -1;
    uz_t min_size = uz_min( o->size, b->size );
    for( sz_t i = 0; i < min_size; i++ )
    {
        if( o->data[ i ] != b->data[ i ] ) return ( o->data[ i ] < b->data[ i ] ) ? 2 : -2;
    }
    return ( o->size == b->size ) ? 0 : ( o->size < b->size ) ? +1 : -1;
}

// ---------------------------------------------------------------------------------------------------------------------

static void u3_sort( u3_t* data, uz_t size, u3_t* buf, s2_t order )
{
    if( size < 2 ) return;
    uz_t size1 = size >> 1;
    u3_sort( data,         size1       , buf, order );
    u3_sort( data + size1, size - size1, buf, order );
    bcore_u_memcpy( sizeof( u3_t ), buf, data, size1 );
    if( order > 0 )
    {
        for( uz_t i = 0, w = 0, r = size1; i < size1; )
        {
            data[ w++ ] = ( r == size || buf[ i ] <= data[ r ] ) ? buf[ i++ ] : data[ r++ ];
        }
    }
    else
    {
        for( uz_t i = 0, w = 0, r = size1; i < size1; )
        {
            data[ w++ ] = ( r == size || buf[ i ] >= data[ r ] ) ? buf[ i++ ] : data[ r++ ];
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_u3_s* bcore_arr_u3_s_sort( bcore_arr_u3_s* o, s2_t order ) // stable mergesort
{
    if( o->space < o->size ) bcore_arr_u3_s_make_strong( o );
    u3_t* buf = bcore_u_alloc( sizeof( u3_t ), NULL, o->size >> 1, NULL );
    u3_sort( o->data, o->size, buf, order );
    bcore_free( buf );
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_u3_s* bcore_arr_u3_s_reorder( bcore_arr_u3_s* o, const bcore_arr_uz_s* order )
{
    if( o->space < o->size ) bcore_arr_u3_s_make_strong( o );
    uz_t buf_space = 0;
    u3_t* buf = bcore_u_alloc( sizeof( u3_t ), NULL, order->size, &buf_space );
    for( uz_t i = 0; i < order->size; i++ )
    {
        assert( order->data[ i ] < o->size );
        buf[ i ] = o->data[ order->data[ i ] ];
    }
    bcore_un_alloc( sizeof( u3_t ), o->data, o->space, 0, &o->space );
    o->data = buf;
    o->space = buf_space;
    o->size = order->size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_arr_u3_s_count_equal( const bcore_arr_u3_s* o, u3_t val )
{
    uz_t count = 0;
    for( uz_t i = 0; i < o->size; i++ ) count += ( o->data[ i ] == val );
    return count;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// bcore_arr_s0_s

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_s0_s_init( bcore_arr_s0_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = TYPEOF_bcore_arr_s0_s;
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_s0_s_down( bcore_arr_s0_s* o )
{
    if( o->space > 0 ) bcore_un_alloc( sizeof( s0_t ), o->data, o->space, 0, NULL );
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_s0_s_copy( bcore_arr_s0_s* o, const bcore_arr_s0_s* src )
{
    if( o->space < src->size )
    {
        if( o->space > 0 ) o->data = bcore_un_alloc( sizeof( s0_t ), o->data, o->space, 0, &o->space );
        o->data = bcore_un_alloc( sizeof( s0_t ), NULL, o->space, src->size, &o->space );
    }
    bcore_u_memcpy( sizeof( s0_t ), o->data, src->data, src->size );
    o->size = src->size;
}

// ---------------------------------------------------------------------------------------------------------------------

BCORE_DEFINE_FUNCTION_CREATE(  bcore_arr_s0_s )
BCORE_DEFINE_FUNCTION_DISCARD( bcore_arr_s0_s )
BCORE_DEFINE_FUNCTION_CLONE(   bcore_arr_s0_s )

// ---------------------------------------------------------------------------------------------------------------------

static bcore_self_s* arr_s0_s_create_self( void )
{
    bcore_self_s* self = BCORE_SELF_S_BUILD_PARSE_SC( "bcore_arr_s0_s = bcore_array { aware_t _; s0_t [] arr; }", bcore_arr_s0_s );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_s0_s_init, "bcore_fp_init", "init" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_s0_s_down, "bcore_fp_down", "down" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_s0_s_copy, "bcore_fp_copy", "copy" );
    return self;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_s0_s* bcore_arr_s0_s_clear( bcore_arr_s0_s* o )
{
    o->size = 0;
    if( o->space == 0 ) o->data = NULL; // in case array is referencing external data
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_s0_s* bcore_arr_s0_s_set_space( bcore_arr_s0_s* o, uz_t space )
{
    if( o->space == 0 )
    {
        o->data = bcore_u_alloc( sizeof( s0_t ), NULL, space, &o->space );
    }
    else
    {
        o->data = bcore_un_alloc( sizeof( s0_t ), o->data, o->space, space, &o->space );
    }
    if( o->size > space ) o->size = space;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_s0_s* bcore_arr_s0_s_fill( bcore_arr_s0_s* o, uz_t size, s0_t v )
{
    bcore_arr_s0_s_set_space( o, size );
    for( uz_t i = 0; i < size; i++ ) o->data[ i ] = v;
    o->size = size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_s0_s* bcore_arr_s0_s_set_size( bcore_arr_s0_s* o, uz_t size )
{
    bcore_arr_s0_s_set_space( o, size );
    if( size > o->size )
    {
        for( sz_t i = o->size; i < size; i++ ) o->data[ i ] = 0;
    }
    o->size = size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_s0_s* bcore_arr_s0_s_step_fill( bcore_arr_s0_s* o, s0_t v_start, s0_t step, uz_t size )
{
    bcore_arr_s0_s_set_space( o, size );
    s0_t v = v_start;
    for( uz_t i = 0; i < size; i++ )
    {
        o->data[ i ] = v;
        v += step;
    }
    o->size = size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_s0_s_make_strong( bcore_arr_s0_s* o )
{
    if( o->size > o->space )
    {
        s0_t* data = o->data;
        o->data = bcore_u_alloc( sizeof( s0_t ), NULL, o->size, &o->space );
        bcore_u_memcpy( sizeof( s0_t ), o->data, data, o->size );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_s0_s* bcore_arr_s0_s_push( bcore_arr_s0_s* o, s0_t v )
{
    if( o->size >  o->space ) bcore_arr_s0_s_make_strong( o );
    if( o->size == o->space )
    {
        o->data = bcore_un_alloc( sizeof( s0_t ), o->data, o->space, o->space > 0 ? o->space * 2 : 1, &o->space );
    }
    o->data[ o->size++ ] = v;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_s0_s* bcore_arr_s0_s_push_left( bcore_arr_s0_s* o, s0_t v )
{
    bcore_arr_s0_s_push( o, 0 );
    for( sz_t i = o->size - 1; i > 0; i-- ) o->data[ i ] = o->data[ i - 1 ];
    o->data[ 0 ] = v;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_s0_s* bcore_arr_s0_s_push_arr( bcore_arr_s0_s* o, const bcore_arr_s0_s* arr )
{
    if( arr->size == 0 ) return o;
    if( o->size > o->space ) bcore_arr_s0_s_make_strong( o );
    if( o->size + arr->size >= o->space )
    {
        uz_t new_space = uz_max( o->space * 2, o->size + arr->size );
        o->data = bcore_un_alloc( sizeof( tp_t ), o->data, o->space, new_space, &o->space );
    }
    for( sz_t i = 0; i < arr->size; i++ ) o->data[ o->size++ ] = arr->data[ i ];
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

s0_t bcore_arr_s0_s_pop( bcore_arr_s0_s* o )
{
    if( o->size == 0 ) return 0;
    o->size--;
    return o->data[ o->size ];
}

// ---------------------------------------------------------------------------------------------------------------------

s2_t bcore_arr_s0_s_cmp( const bcore_arr_s0_s* o, const bcore_arr_s0_s* b )
{
    if( o->size == 0 ) return ( b->size == 0 ) ? 0 :  1;
    if( b->size == 0 ) return -1;
    uz_t min_size = uz_min( o->size, b->size );
    for( sz_t i = 0; i < min_size; i++ )
    {
        if( o->data[ i ] != b->data[ i ] ) return ( o->data[ i ] < b->data[ i ] ) ? 2 : -2;
    }
    return ( o->size == b->size ) ? 0 : ( o->size < b->size ) ? +1 : -1;
}

// ---------------------------------------------------------------------------------------------------------------------

static void s0_sort( s0_t* data, uz_t size, s0_t* buf, s2_t order )
{
    if( size < 2 ) return;
    uz_t size1 = size >> 1;
    s0_sort( data,         size1       , buf, order );
    s0_sort( data + size1, size - size1, buf, order );
    bcore_u_memcpy( sizeof( s0_t ), buf, data, size1 );
    if( order > 0 )
    {
        for( uz_t i = 0, w = 0, r = size1; i < size1; )
        {
            data[ w++ ] = ( r == size || buf[ i ] <= data[ r ] ) ? buf[ i++ ] : data[ r++ ];
        }
    }
    else
    {
        for( uz_t i = 0, w = 0, r = size1; i < size1; )
        {
            data[ w++ ] = ( r == size || buf[ i ] >= data[ r ] ) ? buf[ i++ ] : data[ r++ ];
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_s0_s* bcore_arr_s0_s_sort( bcore_arr_s0_s* o, s2_t order ) // stable mergesort
{
    if( o->space < o->size ) bcore_arr_s0_s_make_strong( o );
    s0_t* buf = bcore_u_alloc( sizeof( s0_t ), NULL, o->size >> 1, NULL );
    s0_sort( o->data, o->size, buf, order );
    bcore_free( buf );
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_s0_s* bcore_arr_s0_s_reorder( bcore_arr_s0_s* o, const bcore_arr_uz_s* order )
{
    if( o->space < o->size ) bcore_arr_s0_s_make_strong( o );
    uz_t buf_space = 0;
    s0_t* buf = bcore_u_alloc( sizeof( s0_t ), NULL, order->size, &buf_space );
    for( uz_t i = 0; i < order->size; i++ )
    {
        assert( order->data[ i ] < o->size );
        buf[ i ] = o->data[ order->data[ i ] ];
    }
    bcore_un_alloc( sizeof( s0_t ), o->data, o->space, 0, &o->space );
    o->data = buf;
    o->space = buf_space;
    o->size = order->size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_arr_s0_s_count_equal( const bcore_arr_s0_s* o, s0_t val )
{
    uz_t count = 0;
    for( uz_t i = 0; i < o->size; i++ ) count += ( o->data[ i ] == val );
    return count;
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_arr_s0_s_find( const bcore_arr_s0_s* o, uz_t start, uz_t end, s0_t v )
{
    if( end >= start )
    {
        uz_t end_l = end < o->size ? end : o->size;
        for( uz_t j = start; j < end_l; j++ ) if( o->data[ j ] == v ) return j;
        return end_l;
    }
    else
    {
        uz_t start_l = start < o->size ? start : o->size;
        for( uz_t j = start_l - 1;  j < start_l && j >= end; j-- ) if( o->data[ j ] == v ) return j;
        return start_l;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

s0_t bcore_arr_s0_s_max( const bcore_arr_s0_s* o )
{
    if( o->size == 0 ) return 0;
    s0_t max = o->data[ 0 ];
    for( uz_t i = 1; i < o->size; i++ ) max = o->data[ i ] > max ? o->data[ i ] : max;
    return max;
}

// ---------------------------------------------------------------------------------------------------------------------

s0_t bcore_arr_s0_s_min( const bcore_arr_s0_s* o )
{
    if( o->size == 0 ) return 0;
    s0_t min = o->data[ 0 ];
    for( uz_t i = 1; i < o->size; i++ ) min = o->data[ i ] < min ? o->data[ i ] : min;
    return min;
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_arr_s0_s_idx_max( const bcore_arr_s0_s* o )
{
    if( o->size == 0 ) return 0;
    s0_t max = o->data[ 0 ];
    uz_t idx = 0;
    for( uz_t i = 1; i < o->size; i++ )
    {
        if( o->data[ i ] > max )
        {
            max = o->data[ i ];
            idx = i;
        }
    }
    return idx;
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_arr_s0_s_idx_min( const bcore_arr_s0_s* o )
{
    if( o->size == 0 ) return 0;
    s0_t min = o->data[ 0 ];
    uz_t idx = 0;
    for( uz_t i = 1; i < o->size; i++ )
    {
        if( o->data[ i ] < min )
        {
            min = o->data[ i ];
            idx = i;
        }
    }
    return idx;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// bcore_arr_s3_s

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_s3_s_init( bcore_arr_s3_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = TYPEOF_bcore_arr_s3_s;
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_s3_s_down( bcore_arr_s3_s* o )
{
    if( o->space > 0 ) bcore_un_alloc( sizeof( s3_t ), o->data, o->space, 0, NULL );
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_s3_s_copy( bcore_arr_s3_s* o, const bcore_arr_s3_s* src )
{
    if( o->space < src->size )
    {
        if( o->space > 0 ) o->data = bcore_un_alloc( sizeof( s3_t ), o->data, o->space, 0, &o->space );
        o->data = bcore_un_alloc( sizeof( s3_t ), NULL, o->space, src->size, &o->space );
    }
    bcore_u_memcpy( sizeof( s3_t ), o->data, src->data, src->size );
    o->size = src->size;
}

// ---------------------------------------------------------------------------------------------------------------------

BCORE_DEFINE_FUNCTION_CREATE(  bcore_arr_s3_s )
BCORE_DEFINE_FUNCTION_DISCARD( bcore_arr_s3_s )
BCORE_DEFINE_FUNCTION_CLONE(   bcore_arr_s3_s )

// ---------------------------------------------------------------------------------------------------------------------

static bcore_self_s* arr_s3_s_create_self( void )
{
    bcore_self_s* self = BCORE_SELF_S_BUILD_PARSE_SC( "bcore_arr_s3_s = bcore_array { aware_t _; s3_t [] arr; }", bcore_arr_s3_s );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_s3_s_init, "bcore_fp_init", "init" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_s3_s_down, "bcore_fp_down", "down" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_s3_s_copy, "bcore_fp_copy", "copy" );
    return self;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_s3_s* bcore_arr_s3_s_clear( bcore_arr_s3_s* o )
{
    o->size = 0;
    if( o->space == 0 ) o->data = NULL; // in case array is referencing external data
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_s3_s* bcore_arr_s3_s_set_space( bcore_arr_s3_s* o, uz_t space )
{
    if( o->space == 0 )
    {
        o->data = bcore_u_alloc( sizeof( s3_t ), NULL, space, &o->space );
    }
    else
    {
        o->data = bcore_un_alloc( sizeof( s3_t ), o->data, o->space, space, &o->space );
    }
    if( o->size > space ) o->size = space;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_s3_s* bcore_arr_s3_s_fill( bcore_arr_s3_s* o, uz_t size, s3_t v )
{
    bcore_arr_s3_s_set_space( o, size );
    for( uz_t i = 0; i < size; i++ ) o->data[ i ] = v;
    o->size = size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_s3_s* bcore_arr_s3_s_set_size( bcore_arr_s3_s* o, uz_t size )
{
    bcore_arr_s3_s_set_space( o, size );
    if( size > o->size )
    {
        for( sz_t i = o->size; i < size; i++ ) o->data[ i ] = 0;
    }
    o->size = size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_s3_s* bcore_arr_s3_s_step_fill( bcore_arr_s3_s* o, s3_t v_start, s3_t step, uz_t size )
{
    bcore_arr_s3_s_set_space( o, size );
    s3_t v = v_start;
    for( uz_t i = 0; i < size; i++ )
    {
        o->data[ i ] = v;
        v += step;
    }
    o->size = size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_s3_s_make_strong( bcore_arr_s3_s* o )
{
    if( o->size > o->space )
    {
        s3_t* data = o->data;
        o->data = bcore_u_alloc( sizeof( s3_t ), NULL, o->size, &o->space );
        bcore_u_memcpy( sizeof( s3_t ), o->data, data, o->size );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_s3_s* bcore_arr_s3_s_push( bcore_arr_s3_s* o, s3_t v )
{
    if( o->size >  o->space ) bcore_arr_s3_s_make_strong( o );
    if( o->size == o->space )
    {
        o->data = bcore_un_alloc( sizeof( s3_t ), o->data, o->space, o->space > 0 ? o->space * 2 : 1, &o->space );
    }
    o->data[ o->size++ ] = v;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_s3_s* bcore_arr_s3_s_push_left( bcore_arr_s3_s* o, s3_t v )
{
    bcore_arr_s3_s_push( o, 0 );
    for( sz_t i = o->size - 1; i > 0; i-- ) o->data[ i ] = o->data[ i - 1 ];
    o->data[ 0 ] = v;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_s3_s* bcore_arr_s3_s_push_arr( bcore_arr_s3_s* o, const bcore_arr_s3_s* arr )
{
    if( arr->size == 0 ) return o;
    if( o->size > o->space ) bcore_arr_s3_s_make_strong( o );
    if( o->size + arr->size >= o->space )
    {
        uz_t new_space = uz_max( o->space * 2, o->size + arr->size );
        o->data = bcore_un_alloc( sizeof( tp_t ), o->data, o->space, new_space, &o->space );
    }
    for( sz_t i = 0; i < arr->size; i++ ) o->data[ o->size++ ] = arr->data[ i ];
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

s3_t bcore_arr_s3_s_pop( bcore_arr_s3_s* o )
{
    if( o->size == 0 ) return 0;
    o->size--;
    return o->data[ o->size ];
}

// ---------------------------------------------------------------------------------------------------------------------

s2_t bcore_arr_s3_s_cmp( const bcore_arr_s3_s* o, const bcore_arr_s3_s* b )
{
    if( o->size == 0 ) return ( b->size == 0 ) ? 0 :  1;
    if( b->size == 0 ) return -1;
    uz_t min_size = uz_min( o->size, b->size );
    for( sz_t i = 0; i < min_size; i++ )
    {
        if( o->data[ i ] != b->data[ i ] ) return ( o->data[ i ] < b->data[ i ] ) ? 2 : -2;
    }
    return ( o->size == b->size ) ? 0 : ( o->size < b->size ) ? +1 : -1;
}

// ---------------------------------------------------------------------------------------------------------------------

static void s3_sort( s3_t* data, uz_t size, s3_t* buf, s2_t order )
{
    if( size < 2 ) return;
    uz_t size1 = size >> 1;
    s3_sort( data,         size1       , buf, order );
    s3_sort( data + size1, size - size1, buf, order );
    bcore_u_memcpy( sizeof( s3_t ), buf, data, size1 );
    if( order > 0 )
    {
        for( uz_t i = 0, w = 0, r = size1; i < size1; )
        {
            data[ w++ ] = ( r == size || buf[ i ] <= data[ r ] ) ? buf[ i++ ] : data[ r++ ];
        }
    }
    else
    {
        for( uz_t i = 0, w = 0, r = size1; i < size1; )
        {
            data[ w++ ] = ( r == size || buf[ i ] >= data[ r ] ) ? buf[ i++ ] : data[ r++ ];
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_s3_s* bcore_arr_s3_s_sort( bcore_arr_s3_s* o, s2_t order ) // stable mergesort
{
    if( o->space < o->size ) bcore_arr_s3_s_make_strong( o );
    s3_t* buf = bcore_u_alloc( sizeof( s3_t ), NULL, o->size >> 1, NULL );
    s3_sort( o->data, o->size, buf, order );
    bcore_free( buf );
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_s3_s* bcore_arr_s3_s_reorder( bcore_arr_s3_s* o, const bcore_arr_uz_s* order )
{
    if( o->space < o->size ) bcore_arr_s3_s_make_strong( o );
    uz_t buf_space = 0;
    s3_t* buf = bcore_u_alloc( sizeof( s3_t ), NULL, order->size, &buf_space );
    for( uz_t i = 0; i < order->size; i++ )
    {
        assert( order->data[ i ] < o->size );
        buf[ i ] = o->data[ order->data[ i ] ];
    }
    bcore_un_alloc( sizeof( s3_t ), o->data, o->space, 0, &o->space );
    o->data = buf;
    o->space = buf_space;
    o->size = order->size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_arr_s3_s_count_equal( const bcore_arr_s3_s* o, s3_t val )
{
    uz_t count = 0;
    for( uz_t i = 0; i < o->size; i++ ) count += ( o->data[ i ] == val );
    return count;
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_arr_s3_s_find( const bcore_arr_s3_s* o, uz_t start, uz_t end, s3_t v )
{
    if( end >= start )
    {
        uz_t end_l = end < o->size ? end : o->size;
        for( uz_t j = start; j < end_l; j++ ) if( o->data[ j ] == v ) return j;
        return end_l;
    }
    else
    {
        uz_t start_l = start < o->size ? start : o->size;
        for( uz_t j = start_l - 1;  j < start_l && j >= end; j-- ) if( o->data[ j ] == v ) return j;
        return start_l;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

s3_t bcore_arr_s3_s_max( const bcore_arr_s3_s* o )
{
    if( o->size == 0 ) return 0;
    s3_t max = o->data[ 0 ];
    for( uz_t i = 1; i < o->size; i++ ) max = o->data[ i ] > max ? o->data[ i ] : max;
    return max;
}

// ---------------------------------------------------------------------------------------------------------------------

s3_t bcore_arr_s3_s_min( const bcore_arr_s3_s* o )
{
    if( o->size == 0 ) return 0;
    s3_t min = o->data[ 0 ];
    for( uz_t i = 1; i < o->size; i++ ) min = o->data[ i ] < min ? o->data[ i ] : min;
    return min;
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_arr_s3_s_idx_max( const bcore_arr_s3_s* o )
{
    if( o->size == 0 ) return 0;
    s3_t max = o->data[ 0 ];
    uz_t idx = 0;
    for( uz_t i = 1; i < o->size; i++ )
    {
        if( o->data[ i ] > max )
        {
            max = o->data[ i ];
            idx = i;
        }
    }
    return idx;
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_arr_s3_s_idx_min( const bcore_arr_s3_s* o )
{
    if( o->size == 0 ) return 0;
    s3_t min = o->data[ 0 ];
    uz_t idx = 0;
    for( uz_t i = 1; i < o->size; i++ )
    {
        if( o->data[ i ] < min )
        {
            min = o->data[ i ];
            idx = i;
        }
    }
    return idx;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// bcore_arr_f3_s

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_f3_s_init( bcore_arr_f3_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = TYPEOF_bcore_arr_f3_s;
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_f3_s_down( bcore_arr_f3_s* o )
{
    if( o->space > 0 ) bcore_un_alloc( sizeof( f3_t ), o->data, o->space, 0, NULL );
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_f3_s_copy( bcore_arr_f3_s* o, const bcore_arr_f3_s* src )
{
    if( o->space < src->size )
    {
        if( o->space > 0 ) o->data = bcore_un_alloc( sizeof( f3_t ), o->data, o->space, 0, &o->space );
        o->data = bcore_un_alloc( sizeof( f3_t ), NULL, o->space, src->size, &o->space );
    }
    bcore_u_memcpy( sizeof( f3_t ), o->data, src->data, src->size );
    o->size = src->size;
}

// ---------------------------------------------------------------------------------------------------------------------

BCORE_DEFINE_FUNCTION_CREATE(  bcore_arr_f3_s )
BCORE_DEFINE_FUNCTION_DISCARD( bcore_arr_f3_s )
BCORE_DEFINE_FUNCTION_CLONE(   bcore_arr_f3_s )

// ---------------------------------------------------------------------------------------------------------------------

static bcore_self_s* arr_f3_s_create_self( void )
{
    bcore_self_s* self = BCORE_SELF_S_BUILD_PARSE_SC( "bcore_arr_f3_s = bcore_array { aware_t _; f3_t [] arr; }", bcore_arr_f3_s );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_f3_s_init, "bcore_fp_init", "init" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_f3_s_down, "bcore_fp_down", "down" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_f3_s_copy, "bcore_fp_copy", "copy" );
    return self;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_f3_s* bcore_arr_f3_s_clear( bcore_arr_f3_s* o )
{
    o->size = 0;
    if( o->space == 0 ) o->data = NULL; // in case array is referencing external data
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_f3_s* bcore_arr_f3_s_set_space( bcore_arr_f3_s* o, uz_t space )
{
    if( o->space == 0 )
    {
        o->data = bcore_u_alloc( sizeof( f3_t ), NULL, space, &o->space );
    }
    else
    {
        o->data = bcore_un_alloc( sizeof( f3_t ), o->data, o->space, space, &o->space );
    }
    if( o->size > space ) o->size = space;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_f3_s* bcore_arr_f3_s_fill( bcore_arr_f3_s* o, uz_t size, f3_t v )
{
    bcore_arr_f3_s_set_space( o, size );
    for( uz_t i = 0; i < size; i++ ) o->data[ i ] = v;
    o->size = size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_f3_s* bcore_arr_f3_s_set_size( bcore_arr_f3_s* o, uz_t size )
{
    bcore_arr_f3_s_set_space( o, size );
    if( size > o->size )
    {
        for( sz_t i = o->size; i < size; i++ ) o->data[ i ] = 0;
    }
    o->size = size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_f3_s* bcore_arr_f3_s_step_fill( bcore_arr_f3_s* o, f3_t v_start, f3_t step, uz_t size )
{
    bcore_arr_f3_s_set_space( o, size );
    f3_t v = v_start;
    for( uz_t i = 0; i < size; i++ )
    {
        o->data[ i ] = v;
        v += step;
    }
    o->size = size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_f3_s_make_strong( bcore_arr_f3_s* o )
{
    if( o->size > o->space )
    {
        f3_t* data = o->data;
        o->data = bcore_u_alloc( sizeof( f3_t ), NULL, o->size, &o->space );
        bcore_u_memcpy( sizeof( f3_t ), o->data, data, o->size );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_f3_s* bcore_arr_f3_s_push( bcore_arr_f3_s* o, f3_t v )
{
    if( o->size >  o->space ) bcore_arr_f3_s_make_strong( o );
    if( o->size == o->space )
    {
        o->data = bcore_un_alloc( sizeof( f3_t ), o->data, o->space, o->space > 0 ? o->space * 2 : 1, &o->space );
    }
    o->data[ o->size++ ] = v;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_f3_s* bcore_arr_f3_s_push_left( bcore_arr_f3_s* o, f3_t v )
{
    bcore_arr_f3_s_push( o, 0 );
    for( sz_t i = o->size - 1; i > 0; i-- ) o->data[ i ] = o->data[ i - 1 ];
    o->data[ 0 ] = v;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_f3_s* bcore_arr_f3_s_push_arr( bcore_arr_f3_s* o, const bcore_arr_f3_s* arr )
{
    if( arr->size == 0 ) return o;
    if( o->size > o->space ) bcore_arr_f3_s_make_strong( o );
    if( o->size + arr->size >= o->space )
    {
        uz_t new_space = uz_max( o->space * 2, o->size + arr->size );
        o->data = bcore_un_alloc( sizeof( tp_t ), o->data, o->space, new_space, &o->space );
    }
    for( sz_t i = 0; i < arr->size; i++ ) o->data[ o->size++ ] = arr->data[ i ];
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

f3_t bcore_arr_f3_s_pop( bcore_arr_f3_s* o )
{
    if( o->size == 0 ) return 0;
    o->size--;
    return o->data[ o->size ];
}

// ---------------------------------------------------------------------------------------------------------------------

s2_t bcore_arr_f3_s_cmp( const bcore_arr_f3_s* o, const bcore_arr_f3_s* b )
{
    if( o->size == 0 ) return ( b->size == 0 ) ? 0 :  1;
    if( b->size == 0 ) return -1;
    uz_t min_size = uz_min( o->size, b->size );
    for( sz_t i = 0; i < min_size; i++ )
    {
        if( o->data[ i ] != b->data[ i ] ) return ( o->data[ i ] < b->data[ i ] ) ? 2 : -2;
    }
    return ( o->size == b->size ) ? 0 : ( o->size < b->size ) ? +1 : -1;
}

// ---------------------------------------------------------------------------------------------------------------------

static void f3_sort( f3_t* data, uz_t size, f3_t* buf, s2_t order )
{
    if( size < 2 ) return;
    uz_t size1 = size >> 1;
    f3_sort( data,         size1       , buf, order );
    f3_sort( data + size1, size - size1, buf, order );
    bcore_u_memcpy( sizeof( f3_t ), buf, data, size1 );
    if( order > 0 )
    {
        for( uz_t i = 0, w = 0, r = size1; i < size1; )
        {
            data[ w++ ] = ( r == size || buf[ i ] <= data[ r ] ) ? buf[ i++ ] : data[ r++ ];
        }
    }
    else
    {
        for( uz_t i = 0, w = 0, r = size1; i < size1; )
        {
            data[ w++ ] = ( r == size || buf[ i ] >= data[ r ] ) ? buf[ i++ ] : data[ r++ ];
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_f3_s* bcore_arr_f3_s_sort( bcore_arr_f3_s* o, s2_t order ) // stable mergesort
{
    if( o->space < o->size ) bcore_arr_f3_s_make_strong( o );
    f3_t* buf = bcore_u_alloc( sizeof( f3_t ), NULL, o->size >> 1, NULL );
    f3_sort( o->data, o->size, buf, order );
    bcore_free( buf );
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_f3_s* bcore_arr_f3_s_reorder( bcore_arr_f3_s* o, const bcore_arr_uz_s* order )
{
    if( o->space < o->size ) bcore_arr_f3_s_make_strong( o );
    uz_t buf_space = 0;
    f3_t* buf = bcore_u_alloc( sizeof( f3_t ), NULL, order->size, &buf_space );
    for( uz_t i = 0; i < order->size; i++ )
    {
        assert( order->data[ i ] < o->size );
        buf[ i ] = o->data[ order->data[ i ] ];
    }
    bcore_un_alloc( sizeof( f3_t ), o->data, o->space, 0, &o->space );
    o->data = buf;
    o->space = buf_space;
    o->size = order->size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_arr_f3_s_count_equal( const bcore_arr_f3_s* o, f3_t val )
{
    uz_t count = 0;
    for( uz_t i = 0; i < o->size; i++ ) count += ( o->data[ i ] == val );
    return count;
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_arr_f3_s_find( const bcore_arr_f3_s* o, uz_t start, uz_t end, f3_t v )
{
    if( end >= start )
    {
        uz_t end_l = end < o->size ? end : o->size;
        for( uz_t j = start; j < end_l; j++ ) if( o->data[ j ] == v ) return j;
        return end_l;
    }
    else
    {
        uz_t start_l = start < o->size ? start : o->size;
        for( uz_t j = start_l - 1;  j < start_l && j >= end; j-- ) if( o->data[ j ] == v ) return j;
        return start_l;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

f3_t bcore_arr_f3_s_max( const bcore_arr_f3_s* o )
{
    if( o->size == 0 ) return 0;
    f3_t max = o->data[ 0 ];
    for( uz_t i = 1; i < o->size; i++ ) max = o->data[ i ] > max ? o->data[ i ] : max;
    return max;
}

// ---------------------------------------------------------------------------------------------------------------------

f3_t bcore_arr_f3_s_min( const bcore_arr_f3_s* o )
{
    if( o->size == 0 ) return 0;
    f3_t min = o->data[ 0 ];
    for( uz_t i = 1; i < o->size; i++ ) min = o->data[ i ] < min ? o->data[ i ] : min;
    return min;
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_arr_f3_s_idx_max( const bcore_arr_f3_s* o )
{
    if( o->size == 0 ) return 0;
    f3_t max = o->data[ 0 ];
    uz_t idx = 0;
    for( uz_t i = 1; i < o->size; i++ )
    {
        if( o->data[ i ] > max )
        {
            max = o->data[ i ];
            idx = i;
        }
    }
    return idx;
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_arr_f3_s_idx_min( const bcore_arr_f3_s* o )
{
    if( o->size == 0 ) return 0;
    f3_t min = o->data[ 0 ];
    uz_t idx = 0;
    for( uz_t i = 1; i < o->size; i++ )
    {
        if( o->data[ i ] < min )
        {
            min = o->data[ i ];
            idx = i;
        }
    }
    return idx;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// bcore_arr_tp_s

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_tp_s_init( bcore_arr_tp_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = TYPEOF_bcore_arr_tp_s;
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_tp_s_down( bcore_arr_tp_s* o )
{
    if( o->space > 0 ) bcore_un_alloc( sizeof( tp_t ), o->data, o->space, 0, NULL );
}

// ---------------------------------------------------------------------------------------------------------------------

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

// ---------------------------------------------------------------------------------------------------------------------

BCORE_DEFINE_FUNCTION_CREATE(  bcore_arr_tp_s )
BCORE_DEFINE_FUNCTION_DISCARD( bcore_arr_tp_s )
BCORE_DEFINE_FUNCTION_CLONE(   bcore_arr_tp_s )

// ---------------------------------------------------------------------------------------------------------------------

static bcore_self_s* arr_tp_s_create_self( void )
{
    bcore_self_s* self = BCORE_SELF_S_BUILD_PARSE_SC( "bcore_arr_tp_s = bcore_array { aware_t _; tp_t [] arr; }", bcore_arr_tp_s );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_tp_s_init, "bcore_fp_init", "init" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_tp_s_down, "bcore_fp_down", "down" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_tp_s_copy, "bcore_fp_copy", "copy" );
    return self;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_tp_s* bcore_arr_tp_s_clear( bcore_arr_tp_s* o )
{
    o->size = 0;
    if( o->space == 0 ) o->data = NULL; // in case array is referencing external data
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_tp_s* bcore_arr_tp_s_set_space( bcore_arr_tp_s* o, uz_t space )
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
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_tp_s* bcore_arr_tp_s_fill( bcore_arr_tp_s* o, uz_t size, tp_t v )
{
    bcore_arr_tp_s_set_space( o, size );
    for( uz_t i = 0; i < size; i++ ) o->data[ i ] = v;
    o->size = size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_tp_s* bcore_arr_tp_s_set_size( bcore_arr_tp_s* o, uz_t size )
{
    bcore_arr_tp_s_set_space( o, size );
    if( size > o->size )
    {
        for( sz_t i = o->size; i < size; i++ ) o->data[ i ] = 0;
    }
    o->size = size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_tp_s_make_strong( bcore_arr_tp_s* o )
{
    if( o->size > o->space )
    {
        tp_t* data = o->data;
        o->data = bcore_u_alloc( sizeof( tp_t ), NULL, o->size, &o->space );
        bcore_u_memcpy( sizeof( tp_t ), o->data, data, o->size );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_tp_s* bcore_arr_tp_s_push( bcore_arr_tp_s* o, tp_t v )
{
    if( o->size >  o->space ) bcore_arr_tp_s_make_strong( o );
    if( o->size == o->space )
    {
        o->data = bcore_un_alloc( sizeof( tp_t ), o->data, o->space, o->space > 0 ? o->space * 2 : 1, &o->space );
    }
    o->data[ o->size++ ] = v;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_tp_s* bcore_arr_tp_s_push_left( bcore_arr_tp_s* o, tp_t v )
{
    bcore_arr_tp_s_push( o, 0 );
    for( sz_t i = o->size - 1; i > 0; i-- ) o->data[ i ] = o->data[ i - 1 ];
    o->data[ 0 ] = v;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_tp_s* bcore_arr_tp_s_push_arr( bcore_arr_tp_s* o, const bcore_arr_tp_s* arr )
{
    if( arr->size == 0 ) return o;
    if( o->size > o->space ) bcore_arr_tp_s_make_strong( o );
    if( o->size + arr->size >= o->space )
    {
        uz_t new_space = uz_max( o->space * 2, o->size + arr->size );
        o->data = bcore_un_alloc( sizeof( tp_t ), o->data, o->space, new_space, &o->space );
    }
    for( sz_t i = 0; i < arr->size; i++ ) o->data[ o->size++ ] = arr->data[ i ];
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

tp_t bcore_arr_tp_s_pop( bcore_arr_tp_s* o )
{
    if( o->size == 0 ) return 0;
    o->size--;
    return o->data[ o->size ];
}

// ---------------------------------------------------------------------------------------------------------------------

s2_t bcore_arr_tp_s_cmp( const bcore_arr_tp_s* o, const bcore_arr_tp_s* b )
{
    if( o->size == 0 ) return ( b->size == 0 ) ? 0 :  1;
    if( b->size == 0 ) return -1;
    uz_t min_size = uz_min( o->size, b->size );
    for( sz_t i = 0; i < min_size; i++ )
    {
        if( o->data[ i ] != b->data[ i ] ) return ( o->data[ i ] < b->data[ i ] ) ? 2 : -2;
    }
    return ( o->size == b->size ) ? 0 : ( o->size < b->size ) ? +1 : -1;
}

// ---------------------------------------------------------------------------------------------------------------------

static void tp_sort( tp_t* data, uz_t size, tp_t* buf, s2_t order )
{
    if( size < 2 ) return;
    uz_t size1 = size >> 1;
    tp_sort( data,         size1       , buf, order );
    tp_sort( data + size1, size - size1, buf, order );
    bcore_u_memcpy( sizeof( tp_t ), buf, data, size1 );
    if( order > 0 )
    {
        for( uz_t i = 0, w = 0, r = size1; i < size1; )
        {
            data[ w++ ] = ( r == size || buf[ i ] <= data[ r ] ) ? buf[ i++ ] : data[ r++ ];
        }
    }
    else
    {
        for( uz_t i = 0, w = 0, r = size1; i < size1; )
        {
            data[ w++ ] = ( r == size || buf[ i ] >= data[ r ] ) ? buf[ i++ ] : data[ r++ ];
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_tp_s* bcore_arr_tp_s_sort( bcore_arr_tp_s* o, s2_t order ) // stable mergesort
{
    if( o->space < o->size ) bcore_arr_tp_s_make_strong( o );
    tp_t* buf = bcore_u_alloc( sizeof( tp_t ), NULL, o->size >> 1, NULL );
    tp_sort( o->data, o->size, buf, order );
    bcore_free( buf );
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_tp_s* bcore_arr_tp_s_reorder( bcore_arr_tp_s* o, const bcore_arr_uz_s* order )
{
    if( o->space < o->size ) bcore_arr_tp_s_make_strong( o );
    uz_t buf_space = 0;
    tp_t* buf = bcore_u_alloc( sizeof( tp_t ), NULL, order->size, &buf_space );
    for( uz_t i = 0; i < order->size; i++ )
    {
        assert( order->data[ i ] < o->size );
        buf[ i ] = o->data[ order->data[ i ] ];
    }
    bcore_un_alloc( sizeof( tp_t ), o->data, o->space, 0, &o->space );
    o->data = buf;
    o->space = buf_space;
    o->size = order->size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_arr_tp_s_find( const bcore_arr_tp_s* o, uz_t start, uz_t end, tp_t v )
{
    if( end >= start )
    {
        uz_t end_l = end < o->size ? end : o->size;
        for( uz_t j = start; j < end_l; j++ ) if( o->data[ j ] == v ) return j;
        return end_l;
    }
    else
    {
        uz_t start_l = start < o->size ? start : o->size;
        for( uz_t j = start_l - 1;  j < start_l && j >= end; j-- ) if( o->data[ j ] == v ) return j;
        return start_l;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_arr_tp_s_count_equal( const bcore_arr_tp_s* o, tp_t val )
{
    uz_t count = 0;
    for( uz_t i = 0; i < o->size; i++ ) count += ( o->data[ i ] == val );
    return count;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// bcore_arr_bl_s

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_bl_s_init( bcore_arr_bl_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = TYPEOF_bcore_arr_bl_s;
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_bl_s_down( bcore_arr_bl_s* o )
{
    if( o->space > 0 ) bcore_un_alloc( sizeof( bl_t ), o->data, o->space, 0, NULL );
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_bl_s_copy( bcore_arr_bl_s* o, const bcore_arr_bl_s* src )
{
    if( o->space < src->size )
    {
        if( o->space > 0 ) o->data = bcore_un_alloc( sizeof( bl_t ), o->data, o->space, 0, &o->space );
        o->data = bcore_un_alloc( sizeof( bl_t ), NULL, o->space, src->size, &o->space );
    }
    bcore_u_memcpy( sizeof( bl_t ), o->data, src->data, src->size );
    o->size = src->size;
}

// ---------------------------------------------------------------------------------------------------------------------

BCORE_DEFINE_FUNCTION_CREATE(  bcore_arr_bl_s )
BCORE_DEFINE_FUNCTION_DISCARD( bcore_arr_bl_s )
BCORE_DEFINE_FUNCTION_CLONE(   bcore_arr_bl_s )

// ---------------------------------------------------------------------------------------------------------------------

static bcore_self_s* arr_bl_s_create_self( void )
{
    bcore_self_s* self = BCORE_SELF_S_BUILD_PARSE_SC( "bcore_arr_bl_s = bcore_array { aware_t _; bl_t [] arr; }", bcore_arr_bl_s );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_bl_s_init, "bcore_fp_init", "init" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_bl_s_down, "bcore_fp_down", "down" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_bl_s_copy, "bcore_fp_copy", "copy" );
    return self;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_bl_s* bcore_arr_bl_s_clear( bcore_arr_bl_s* o )
{
    o->size = 0;
    if( o->space == 0 ) o->data = NULL; // in case array is referencing external data
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_bl_s* bcore_arr_bl_s_set_space( bcore_arr_bl_s* o, uz_t space )
{
    if( o->space == 0 )
    {
        o->data = bcore_u_alloc( sizeof( bl_t ), NULL, space, &o->space );
    }
    else
    {
        o->data = bcore_un_alloc( sizeof( bl_t ), o->data, o->space, space, &o->space );
    }
    if( o->size > space ) o->size = space;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_bl_s* bcore_arr_bl_s_fill( bcore_arr_bl_s* o, uz_t size, bl_t v )
{
    bcore_arr_bl_s_set_space( o, size );
    for( uz_t i = 0; i < size; i++ ) o->data[ i ] = v;
    o->size = size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_bl_s* bcore_arr_bl_s_set_size( bcore_arr_bl_s* o, uz_t size )
{
    bcore_arr_bl_s_set_space( o, size );
    if( size > o->size )
    {
        for( sz_t i = o->size; i < size; i++ ) o->data[ i ] = 0;
    }
    o->size = size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_bl_s_make_strong( bcore_arr_bl_s* o )
{
    if( o->size > o->space )
    {
        bl_t* data = o->data;
        o->data = bcore_u_alloc( sizeof( bl_t ), NULL, o->size, &o->space );
        bcore_u_memcpy( sizeof( bl_t ), o->data, data, o->size );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_bl_s* bcore_arr_bl_s_push( bcore_arr_bl_s* o, bl_t v )
{
    if( o->size >  o->space ) bcore_arr_bl_s_make_strong( o );
    if( o->size == o->space )
    {
        o->data = bcore_un_alloc( sizeof( bl_t ), o->data, o->space, o->space > 0 ? o->space * 2 : 1, &o->space );
    }
    o->data[ o->size++ ] = v;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_bl_s* bcore_arr_bl_s_push_left( bcore_arr_bl_s* o, bl_t v )
{
    bcore_arr_bl_s_push( o, false );
    for( sz_t i = o->size - 1; i > 0; i-- ) o->data[ i ] = o->data[ i - 1 ];
    o->data[ 0 ] = v;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_bl_s* bcore_arr_bl_s_push_arr( bcore_arr_bl_s* o, const bcore_arr_bl_s* arr )
{
    if( arr->size == 0 ) return o;
    if( o->size > o->space ) bcore_arr_bl_s_make_strong( o );
    if( o->size + arr->size >= o->space )
    {
        uz_t new_space = uz_max( o->space * 2, o->size + arr->size );
        o->data = bcore_un_alloc( sizeof( tp_t ), o->data, o->space, new_space, &o->space );
    }
    for( sz_t i = 0; i < arr->size; i++ ) o->data[ o->size++ ] = arr->data[ i ];
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bl_t bcore_arr_bl_s_pop( bcore_arr_bl_s* o )
{
    if( o->size == 0 ) return 0;
    o->size--;
    return o->data[ o->size ];
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_arr_bl_s_count_equal( const bcore_arr_bl_s* o, bl_t val )
{
    uz_t count = 0;
    for( uz_t i = 0; i < o->size; i++ ) count += ( o->data[ i ] == val );
    return count;
}

// ---------------------------------------------------------------------------------------------------------------------

s2_t bcore_arr_bl_s_cmp( const bcore_arr_bl_s* o, const bcore_arr_bl_s* b )
{
    if( o->size == 0 ) return ( b->size == 0 ) ? 0 :  1;
    if( b->size == 0 ) return -1;
    uz_t min_size = uz_min( o->size, b->size );
    for( sz_t i = 0; i < min_size; i++ )
    {
        if( o->data[ i ] != b->data[ i ] ) return ( o->data[ i ] < b->data[ i ] ) ? 2 : -2;
    }
    return ( o->size == b->size ) ? 0 : ( o->size < b->size ) ? +1 : -1;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

BCORE_DEFINE_FUNCTION_INIT_INST( bcore_arr_st_s )
BCORE_DEFINE_FUNCTION_DOWN_INST( bcore_arr_st_s )
BCORE_DEFINE_FUNCTION_COPY_INST( bcore_arr_st_s )
BCORE_DEFINE_FUNCTION_CREATE(    bcore_arr_st_s )
BCORE_DEFINE_FUNCTION_DISCARD(   bcore_arr_st_s )
BCORE_DEFINE_FUNCTION_CLONE(     bcore_arr_st_s )

// ---------------------------------------------------------------------------------------------------------------------

static bcore_self_s* arr_st_s_create_self( void )
{
    return BCORE_SELF_S_BUILD_PARSE_SC( "bcore_arr_st_s = bcore_array { aware_t _; st_s => [] arr; }", bcore_arr_st_s );
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_st_s* bcore_arr_st_s_clear( bcore_arr_st_s* o )
{
    if( o->space == 0 )
    {
        o->data = NULL; // in case array is referencing external data
        o->size = 0;
    }

    for( uz_t i = 0; i < o->size; i++ ) st_s_discard( o->data[ i ] );
    o->size = 0;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_st_s* bcore_arr_st_s_set_space( bcore_arr_st_s* o, uz_t space )
{
    bcore_array_a_set_space( (bcore_array*)o, space );
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_st_s* bcore_arr_st_s_set_size( bcore_arr_st_s* o, uz_t size )
{
    bcore_arr_st_s_set_space( o, size );
    if( size > o->size )
    {
        for( sz_t i = o->size; i < size; i++ ) o->data[ i ] = NULL;
    }
    o->size = size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_st_s_make_strong( bcore_arr_st_s* o )
{
    if( o->size > o->space )
    {
        st_s** data = o->data;
        o->data = bcore_u_alloc( sizeof( st_s* ), NULL, o->size, &o->space );
        for( uz_t i = 0; i < o->size; i++ )
        {
            st_s_copy( o->data[ i ], data[ i ] );
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

st_s* bcore_arr_st_s_push( bcore_arr_st_s* o )
{
    if( o->size >  o->space ) bcore_arr_st_s_make_strong( o );
    if( o->size == o->space )
    {
        o->data = bcore_un_alloc( sizeof( st_s* ), o->data, o->space, o->space > 0 ? o->space * 2 : 1, &o->space );
    }
    o->data[ o->size++ ] = st_s_create();
    return o->data[ o->size - 1 ];
}

// ---------------------------------------------------------------------------------------------------------------------

st_s* bcore_arr_st_s_push_st( bcore_arr_st_s* o, const st_s* st )
{
    st_s* str = bcore_arr_st_s_push( o );
    st_s_copy( str, st );
    return str;
}

// ---------------------------------------------------------------------------------------------------------------------

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

// ---------------------------------------------------------------------------------------------------------------------

st_s* bcore_arr_st_s_push_sc( bcore_arr_st_s* o, sc_t sc )
{
    return bcore_arr_st_s_push_st_d( o, st_s_create_sc( sc ) );
}

// ---------------------------------------------------------------------------------------------------------------------

st_s* bcore_arr_st_s_push_fv( bcore_arr_st_s* o, sc_t format, va_list args )
{
    return bcore_arr_st_s_push_st_d( o, st_s_create_fv( format, args ) );
}

// ---------------------------------------------------------------------------------------------------------------------

st_s* bcore_arr_st_s_push_fa( bcore_arr_st_s* o, sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    st_s* ret = bcore_arr_st_s_push_fv( o, format, args );
    va_end( args );
    return ret;
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_st_s_pop( bcore_arr_st_s* o )
{
    if( o->size == 0 ) return;
    o->size--;
    st_s_discard( o->data[ o->size ] );
}

// ---------------------------------------------------------------------------------------------------------------------

static void st_sort( st_s** data, uz_t size, st_s** buf, s2_t order )
{
    if( size < 2 ) return;
    uz_t size1 = size >> 1;
    st_sort( data,         size1       , buf, order );
    st_sort( data + size1, size - size1, buf, order );
    bcore_u_memcpy( sizeof( st_s* ), buf, data, size1 );
    for( uz_t i = 0, w = 0, r = size1; i < size1; )
    {
        data[ w++ ] = ( r == size || bcore_strcmp( buf[ i ]->sc, data[ r ]->sc ) * order >= 0 ) ? buf[ i++ ] : data[ r++ ];
    }
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_st_s* bcore_arr_st_s_sort( bcore_arr_st_s* o, s2_t order ) // stable mergesort
{
    if( o->space < o->size ) bcore_arr_st_s_make_strong( o );
    st_s** buf = bcore_u_alloc( sizeof( st_s* ), NULL, o->size >> 1, NULL );
    st_sort( o->data, o->size, buf, order );
    bcore_free( buf );
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_st_s* bcore_arr_st_s_reorder( bcore_arr_st_s* o, const bcore_arr_uz_s* order )
{
    if( o->space < o->size ) bcore_arr_st_s_make_strong( o );
    uz_t buf_space = 0;
    st_s** buf = bcore_u_alloc( sizeof( st_s* ), NULL, order->size, &buf_space );
    for( uz_t i = 0; i < order->size; i++ )
    {
        assert( order->data[ i ] < o->size );
        buf[ i ] = bcore_fork( o->data[ order->data[ i ] ] );
    }
    for( uz_t i = 0; i < o->size; i++ ) st_s_discard( o->data[ i ] );
    bcore_un_alloc( sizeof( st_s* ), o->data, o->space, 0, &o->space );
    o->data = buf;
    o->space = buf_space;
    o->size = order->size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_arr_st_s_find( const bcore_arr_st_s* o, uz_t start, uz_t end, const st_s* v )
{
    if( end >= start )
    {
        uz_t end_l = end < o->size ? end : o->size;
        for( uz_t j = start; j < end_l; j++ )
        {
            if( st_s_equal( o->data[ j ], v ) ) return j;
        }
        return end_l;
    }
    else
    {
        uz_t start_l = start < o->size ? start : o->size;
        for( uz_t j = start_l - 1;  j < start_l && j >= end; j-- )
        {
            if( st_s_equal( o->data[ j ], v ) ) return j;
        }
        return start_l;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_arr_st_s_count_equal( const bcore_arr_st_s* o, const st_s* val )
{
    uz_t count = 0;
    for( uz_t i = 0; i < o->size; i++ )
    {
        if( val )
        {
            count += st_s_equal_st( val, o->data[ i ] );
        }
        else
        {
            count += ( o->data[ i ] == NULL );
        }
    }
    return count;
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_st_s_table_to_sink( const bcore_arr_st_s* o, sz_t rows, sz_t cols, bcore_sink* sink )
{
    rows = ( rows < 0 ) ? ( ( cols > 0 ) ? o->size / cols : o->size ) : rows;
    cols = ( cols < 0 ) ? ( ( rows > 0 ) ? o->size / rows : o->size ) : cols;
    rows = ( cols * rows > o->size ) ? o->size / cols : rows;

    bcore_arr_sz_s* col_sz = bcore_arr_sz_s_create();
    bcore_arr_sz_s_fill( col_sz, cols, 0 );

    for( sz_t i = 0; i < o->size; i++ )
    {
        sz_t size = o->data[ i ] ? o->data[ i ]->size : 0;
        col_sz->data[ i % cols ] = sz_max( col_sz->data[ i % cols ], size );
    }

    for( sz_t i = 0; i < o->size; i++ )
    {
        sz_t size = o->data[ i ] ? o->data[ i ]->size : 0;
        sc_t sc = o->data[ i ] ? o->data[ i ]->sc : "";
        if( ( i + 1 ) % cols == 0 )
        {
            bcore_sink_a_push_fa( sink, "#<sc_t>\n", sc );
        }
        else
        {
            bcore_sink_a_push_fa( sink, "#<sc_t>#rn{ }", sc, col_sz->data[ i % cols ] - size );
        }
    }

    bcore_arr_sz_s_discard( col_sz );
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

// bcore_arr_vd_s

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_vd_s_init( bcore_arr_vd_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = TYPEOF_bcore_arr_vd_s;
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_vd_s_down( bcore_arr_vd_s* o )
{
    if( o->space > 0 ) bcore_un_alloc( sizeof( vd_t ), o->data, o->space, 0, NULL );
}

// ---------------------------------------------------------------------------------------------------------------------

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

// ---------------------------------------------------------------------------------------------------------------------

BCORE_DEFINE_FUNCTION_CREATE(  bcore_arr_vd_s )
BCORE_DEFINE_FUNCTION_DISCARD( bcore_arr_vd_s )
BCORE_DEFINE_FUNCTION_CLONE(   bcore_arr_vd_s )

// ---------------------------------------------------------------------------------------------------------------------

static bcore_self_s* arr_vd_s_create_self( void )
{
    bcore_self_s* self = BCORE_SELF_S_BUILD_PARSE_SC( "bcore_arr_vd_s = bcore_array { aware_t _; vd_t [] arr; }", bcore_arr_vd_s );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_vd_s_init, "bcore_fp_init", "init" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_vd_s_down, "bcore_fp_down", "down" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_vd_s_copy, "bcore_fp_copy", "copy" );
    return self;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_vd_s* bcore_arr_vd_s_clear( bcore_arr_vd_s* o )
{
    o->size = 0;
    if( o->space == 0 ) o->data = NULL; // in case array is referencing external data
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_vd_s* bcore_arr_vd_s_set_space( bcore_arr_vd_s* o, uz_t space )
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
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_vd_s* bcore_arr_vd_s_fill( bcore_arr_vd_s* o, uz_t size, vd_t v )
{
    bcore_arr_vd_s_set_space( o, size );
    for( uz_t i = 0; i < size; i++ ) o->data[ i ] = v;
    o->size = size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_vd_s* bcore_arr_vd_s_set_size( bcore_arr_vd_s* o, uz_t size )
{
    bcore_arr_vd_s_set_space( o, size );
    if( size > o->size )
    {
        for( sz_t i = o->size; i < size; i++ ) o->data[ i ] = NULL;
    }
    o->size = size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_vd_s_make_strong( bcore_arr_vd_s* o )
{
    if( o->size > o->space )
    {
        vd_t* data = o->data;
        o->data = bcore_u_alloc( sizeof( vd_t ), NULL, o->size, &o->space );
        bcore_u_memcpy( sizeof( vd_t ), o->data, data, o->size );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_vd_s* bcore_arr_vd_s_push( bcore_arr_vd_s* o, vd_t v )
{
    if( o->size >  o->space ) bcore_arr_vd_s_make_strong( o );
    if( o->size == o->space )
    {
        o->data = bcore_un_alloc( sizeof( vd_t ), o->data, o->space, o->space > 0 ? o->space * 2 : 1, &o->space );
    }
    o->data[ o->size++ ] = v;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_vd_s* bcore_arr_vd_s_push_left( bcore_arr_vd_s* o, vd_t v )
{
    bcore_arr_vd_s_push( o, NULL );
    for( sz_t i = o->size - 1; i > 0; i-- ) o->data[ i ] = o->data[ i - 1 ];
    o->data[ 0 ] = v;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

vd_t bcore_arr_vd_s_pop( bcore_arr_vd_s* o )
{
    if( o->size == 0 ) return 0;
    o->size--;
    return o->data[ o->size ];
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_vd_s* bcore_arr_vd_s_reorder( bcore_arr_vd_s* o, const bcore_arr_uz_s* order )
{
    if( o->space < o->size ) bcore_arr_vd_s_make_strong( o );
    uz_t buf_space = 0;
    vd_t* buf = bcore_u_alloc( sizeof( vd_t ), NULL, order->size, &buf_space );
    for( uz_t i = 0; i < order->size; i++ )
    {
        assert( order->data[ i ] < o->size );
        buf[ i ] = o->data[ order->data[ i ] ];
    }
    bcore_un_alloc( sizeof( vd_t ), o->data, o->space, 0, &o->space );
    o->data = buf;
    o->space = buf_space;
    o->size = order->size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_arr_vd_s_find( const bcore_arr_vd_s* o, uz_t start, uz_t end, vd_t v )
{
    if( end >= start )
    {
        uz_t end_l = end < o->size ? end : o->size;
        for( uz_t j = start; j < end_l; j++ ) if( o->data[ j ] == v ) return j;
        return end_l;
    }
    else
    {
        uz_t start_l = start < o->size ? start : o->size;
        for( uz_t j = start_l - 1;  j < start_l && j >= end; j-- ) if( o->data[ j ] == v ) return j;
        return start_l;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_arr_vd_s_count_equal( const bcore_arr_vd_s* o, vd_t val )
{
    uz_t count = 0;
    for( uz_t i = 0; i < o->size; i++ ) count += ( o->data[ i ] == val );
    return count;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

// bcore_arr_fp_s

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_fp_s_init( bcore_arr_fp_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = TYPEOF_bcore_arr_fp_s;
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_fp_s_down( bcore_arr_fp_s* o )
{
    if( o->space > 0 ) bcore_un_alloc( sizeof( fp_t ), o->data, o->space, 0, NULL );
}

// ---------------------------------------------------------------------------------------------------------------------

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

// ---------------------------------------------------------------------------------------------------------------------

static bcore_self_s* arr_fp_s_create_self( void )
{
    bcore_self_s* self = BCORE_SELF_S_BUILD_PARSE_SC( "bcore_arr_fp_s = bcore_array { aware_t _; fp_t [] arr; }", bcore_arr_fp_s );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_fp_s_init, "bcore_fp_init", "init" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_fp_s_down, "bcore_fp_down", "down" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_arr_fp_s_copy, "bcore_fp_copy", "copy" );
    return self;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_fp_s* bcore_arr_fp_s_clear( bcore_arr_fp_s* o )
{
    o->size = 0;
    if( o->space == 0 ) o->data = NULL; // in case array is referencing external data
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_fp_s* bcore_arr_fp_s_set_space( bcore_arr_fp_s* o, uz_t space )
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
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_fp_s* bcore_arr_fp_s_set_size( bcore_arr_fp_s* o, sz_t size )
{
    bcore_arr_fp_s_set_space( o, size );
    if( size > o->size )
    {
        for( sz_t i = o->size; i < size; i++ ) o->data[ i ] = NULL;
    }
    o->size = size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_fp_s* bcore_arr_fp_s_fill( bcore_arr_fp_s* o, uz_t size, fp_t v )
{
    bcore_arr_fp_s_set_space( o, size );
    for( uz_t i = 0; i < size; i++ ) o->data[ i ] = v;
    o->size = size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_fp_s_make_strong( bcore_arr_fp_s* o )
{
    if( o->size > o->space )
    {
        fp_t* data = o->data;
        o->data = bcore_u_alloc( sizeof( fp_t ), NULL, o->size, &o->space );
        bcore_u_memcpy( sizeof( fp_t ), o->data, data, o->size );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_fp_s* bcore_arr_fp_s_push( bcore_arr_fp_s* o, fp_t v )
{
    if( o->size >  o->space ) bcore_arr_fp_s_make_strong( o );
    if( o->size == o->space )
    {
        o->data = bcore_un_alloc( sizeof( fp_t ), o->data, o->space, o->space > 0 ? o->space * 2 : 1, &o->space );
    }
    o->data[ o->size++ ] = v;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_fp_s* bcore_arr_fp_s_push_left( bcore_arr_fp_s* o, fp_t v )
{
    bcore_arr_fp_s_push( o, NULL );
    for( sz_t i = o->size - 1; i > 0; i-- ) o->data[ i ] = o->data[ i - 1 ];
    o->data[ 0 ] = v;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

fp_t bcore_arr_fp_s_pop( bcore_arr_fp_s* o )
{
    if( o->size == 0 ) return 0;
    o->size--;
    return o->data[ o->size ];
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_fp_s* bcore_arr_fp_s_reorder( bcore_arr_fp_s* o, const bcore_arr_uz_s* order )
{
    if( o->space < o->size ) bcore_arr_fp_s_make_strong( o );
    uz_t buf_space = 0;
    fp_t* buf = bcore_u_alloc( sizeof( fp_t ), NULL, order->size, &buf_space );
    for( uz_t i = 0; i < order->size; i++ )
    {
        assert( order->data[ i ] < o->size );
        buf[ i ] = o->data[ order->data[ i ] ];
    }
    bcore_un_alloc( sizeof( fp_t ), o->data, o->space, 0, &o->space );
    o->data = buf;
    o->space = buf_space;
    o->size = order->size;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_arr_fp_s_find( const bcore_arr_fp_s* o, uz_t start, uz_t end, fp_t v )
{
    if( end >= start )
    {
        uz_t end_l = end < o->size ? end : o->size;
        for( uz_t j = start; j < end_l; j++ ) if( o->data[ j ] == v ) return j;
        return end_l;
    }
    else
    {
        uz_t start_l = start < o->size ? start : o->size;
        for( uz_t j = start_l - 1;  j < start_l && j >= end; j-- ) if( o->data[ j ] == v ) return j;
        return start_l;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_arr_fp_s_count_equal( const bcore_arr_fp_s* o, fp_t val )
{
    uz_t count = 0;
    for( uz_t i = 0; i < o->size; i++ ) count += ( o->data[ i ] == val );
    return count;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// bcore_arr_sr_s

// ---------------------------------------------------------------------------------------------------------------------

BCORE_DEFINE_FUNCTION_INIT_INST( bcore_arr_sr_s )
BCORE_DEFINE_FUNCTION_DOWN_INST( bcore_arr_sr_s )
BCORE_DEFINE_FUNCTION_COPY_INST( bcore_arr_sr_s )
BCORE_DEFINE_FUNCTION_CREATE(    bcore_arr_sr_s )
BCORE_DEFINE_FUNCTION_DISCARD(   bcore_arr_sr_s )
BCORE_DEFINE_FUNCTION_CLONE(     bcore_arr_sr_s )

// ---------------------------------------------------------------------------------------------------------------------

static bcore_self_s* arr_sr_s_create_self( void )
{
    bcore_self_s* self = BCORE_SELF_S_BUILD_PARSE_SC( "bcore_arr_sr_s = bcore_array { aware_t _; sr_s [] arr; }", bcore_arr_sr_s );
    return self;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_sr_s* bcore_arr_sr_s_clear( bcore_arr_sr_s* o )
{
    if( o->space == 0 )
    {
        o->data = NULL; // in case array is referencing external data
        o->size = 0;
    }

    for( uz_t i = 0; i < o->size; i++ ) sr_down( o->data[ i ] );
    o->size = 0;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_sr_s* bcore_arr_sr_s_set_space( bcore_arr_sr_s* o, uz_t space )
{
    bcore_array_a_set_space( (bcore_array*)o, space );
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_sr_s* bcore_arr_sr_s_set_size( bcore_arr_sr_s* o, uz_t size )
{
    bcore_array_a_set_size( (bcore_array*)o, size );
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_sr_s_make_strong( bcore_arr_sr_s* o )
{
    bcore_array_a_make_strong( (bcore_array*)o );
}

// ---------------------------------------------------------------------------------------------------------------------

sr_s* bcore_arr_sr_s_push( bcore_arr_sr_s* o )
{
    if( o->size > o->space ) bcore_arr_sr_s_make_strong( o );
    if( o->size == o->space )
    {
        o->data = bcore_un_alloc( sizeof( sr_s ), o->data, o->space, o->space > 0 ? o->space * 2 : 1, &o->space );
    }
    o->data[ o->size++ ] = sr_null();
    return &o->data[ o->size - 1 ];
}

// ---------------------------------------------------------------------------------------------------------------------

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

// ---------------------------------------------------------------------------------------------------------------------

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

// ---------------------------------------------------------------------------------------------------------------------

sr_s bcore_arr_sr_s_pop( bcore_arr_sr_s* o )
{
    if( o->size == 0 ) return sr_null();
    o->size--;
    return o->data[ o->size ];
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_arr_sr_s_sort( bcore_arr_sr_s* o, s2_t order )
{
    bcore_array_a_sort( (bcore_array*)o, 0, o->size, order );
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_sr_s* bcore_arr_sr_s_reorder( bcore_arr_sr_s* o, const bcore_arr_uz_s* order )
{
    bcore_array_a_reorder( (bcore_array*)o, order);
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_arr_sr_s* bcore_arr_sr_s_set_spect( bcore_arr_sr_s* o, tp_t spect_type )
{
    for( sz_t i = 0; i < o->size; i++ ) o->data[ i ] = sr_cp( o->data[ i ], spect_type );
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bl_t bcore_arr_sr_s_is_numeric( const bcore_arr_sr_s* o )
{
    for( sz_t i = 0; i < o->size; i++ ) if( !sr_s_is_numeric( &o->data[ i ] ) ) return false;
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------

bl_t bcore_arr_sr_s_is_float( const bcore_arr_sr_s* o )
{
    for( sz_t i = 0; i < o->size; i++ ) if( !sr_s_is_float( &o->data[ i ] ) ) return false;
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------

bl_t bcore_arr_sr_s_is_integer( const bcore_arr_sr_s* o )
{
    for( sz_t i = 0; i < o->size; i++ ) if( !sr_s_is_integer( &o->data[ i ] ) ) return false;
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------

bl_t bcore_arr_sr_s_is_unsigned( const bcore_arr_sr_s* o )
{
    for( sz_t i = 0; i < o->size; i++ ) if( !sr_s_is_unsigned( &o->data[ i ] ) ) return false;
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------

bl_t bcore_arr_sr_s_is_string( const bcore_arr_sr_s* o )
{
    for( sz_t i = 0; i < o->size; i++ ) if( !sr_s_is_string( &o->data[ i ] ) ) return false;
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------

const bcore_arr_sr_s* bcore_arr_sr_s_to_arr_s3( const bcore_arr_sr_s* o, bcore_arr_s3_s* arr )
{
    bcore_arr_s3_s_set_size( arr, o->size );
    for( uz_t i = 0; i < o->size; i++ ) arr->data[ i ] = sr_s_to_s3( &o->data[ i ] );
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

const bcore_arr_sr_s* bcore_arr_sr_s_to_arr_u3( const bcore_arr_sr_s* o, bcore_arr_u3_s* arr )
{
    bcore_arr_u3_s_set_size( arr, o->size );
    for( uz_t i = 0; i < o->size; i++ ) arr->data[ i ] = sr_s_to_u3( &o->data[ i ] );
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

const bcore_arr_sr_s* bcore_arr_sr_s_to_arr_f3( const bcore_arr_sr_s* o, bcore_arr_f3_s* arr )
{
    bcore_arr_f3_s_set_size( arr, o->size );
    for( uz_t i = 0; i < o->size; i++ ) arr->data[ i ] = sr_s_to_f3( &o->data[ i ] );
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

const bcore_arr_sr_s* bcore_arr_sr_s_to_arr_sz( const bcore_arr_sr_s* o, bcore_arr_sz_s* arr )
{
    bcore_arr_sz_s_set_size( arr, o->size );
    for( uz_t i = 0; i < o->size; i++ ) arr->data[ i ] = sr_s_to_s3( &o->data[ i ] );
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

const bcore_arr_sr_s* bcore_arr_sr_s_to_arr_st( const bcore_arr_sr_s* o, bcore_arr_st_s* arr )
{
    bcore_arr_st_s_clear( arr );
    bcore_arr_st_s_set_size( arr, o->size );
    for( uz_t i = 0; i < o->size; i++ )
    {
        if( sr_s_type( &o->data[ i ] ) == TYPEOF_st_s ) arr->data[ i ] = st_s_clone( ( const st_s* )o->data[ i ].o );
    }
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// selftests

#include <time.h>

st_s* bcore_arr_uz_selftest( void )
{
    st_s* log = st_s_createf( "== bcore_arr_uz_selftest " );
    st_s_push_char_n( log, '=', 80 - log->size );
    st_s_push_char( log, '\n' );

    bcore_arr_uz_s* sz_arr1 = bcore_arr_uz_s_create();
    bcore_arr_uz_s* sz_arr2 = bcore_arr_uz_s_create();
    uz_t size = 100000;
    {
        u2_t rval = 1;
        clock_t time = clock();
        for( uz_t i = 0; i < size; i++ )
        {
            rval = bcore_xsg_u2( rval );
            bcore_arr_uz_s_push( sz_arr1, rval );
        }
        time = clock() - time;
        st_s_push_fa( log, "#p28.{bcore_arr_uz_s_push } #<uz_t>ns\n", ( uz_t ) ( ( 1E9 * ( f3_t )time/CLOCKS_PER_SEC ) / size ) );
    }

    const bcore_array_s* array_p = bcore_array_s_get_typed( TYPEOF_bcore_arr_uz_s );
    const bcore_inst_s* u2_t_p = bcore_inst_s_get_typed( TYPEOF_u2_t );

    {
        u2_t rval = 1;
        clock_t time = clock();
        for( uz_t i = 0; i < size; i++ )
        {
            rval = bcore_xsg_u2( rval );
            bcore_array_p_push( array_p, (bcore_array*)sz_arr2, sr_pwc( u2_t_p, &rval ) );
        }
        time = clock() - time;
        st_s_push_fa( log, "#p28.{bcore_array_p_push } #<uz_t>ns\n", ( uz_t ) ( ( 1E9 * ( f3_t )time/CLOCKS_PER_SEC ) / size ) );
    }

    ASSERT( bcore_compare_aware( sz_arr1, sz_arr2 ) == 0 );

    {
        clock_t time = clock();
        bcore_arr_uz_s_sort( sz_arr1, 1 );
        time = clock() - time;
        st_s_push_fa( log, "#p28.{bcore_arr_uz_s_sort } #<uz_t>ns\n", ( uz_t ) ( ( 1E9 * ( f3_t )time/CLOCKS_PER_SEC ) / size ) );
    }

    {
        clock_t time = clock();
        bcore_array_a_sort( (bcore_array*)sz_arr2, 0, -1, 1 );
        time = clock() - time;
        st_s_push_fa( log, "#p28.{bcore_array_a_sort } #<uz_t>ns\n", ( uz_t ) ( ( 1E9 * ( f3_t )time/CLOCKS_PER_SEC ) / size ) );
    }

    bcore_arr_uz_s* permutation = bcore_arr_uz_s_create_random_permutation( bcore_xsg2_u2, 1, size );

    {
        clock_t time = clock();
        bcore_arr_uz_s_reorder( sz_arr1, permutation );
        time = clock() - time;
        st_s_push_fa( log, "#p28.{bcore_arr_uz_s_reorder } #<uz_t>ns\n", ( uz_t ) ( ( 1E9 * ( f3_t )time/CLOCKS_PER_SEC ) / size ) );
    }

    {
        clock_t time = clock();
        bcore_array_a_reorder( (bcore_array*)sz_arr2, permutation );
        time = clock() - time;
        st_s_push_fa( log, "#p28.{bcore_array_a_reorder } #<uz_t>ns\n", ( uz_t ) ( ( 1E9 * ( f3_t )time/CLOCKS_PER_SEC ) / size ) );
    }

    ASSERT( bcore_compare_aware( sz_arr1, sz_arr2 ) == 0 );

//    bcore_txt_ml_to_stdout( sr_awc( sz_arr1 ) );

    bcore_arr_uz_s_discard( permutation );
    bcore_arr_uz_s_discard( sz_arr1 );
    bcore_arr_uz_s_discard( sz_arr2 );

    st_s_push_char_n( log, '=', 80 );
    st_s_push_char( log, '\n' );
    return log;

}

// ---------------------------------------------------------------------------------------------------------------------

st_s* bcore_arr_st_selftest( void )
{
    st_s* log = st_s_createf( "== bcore_arr_st_selftest " );
    st_s_push_char_n( log, '=', 80 - log->size );
    st_s_push_char( log, '\n' );

    bcore_arr_st_s* st_arr1 = bcore_arr_st_s_create();
    bcore_arr_st_s* st_arr2 = bcore_arr_st_s_create();
    uz_t size = 100000;
    {
        u2_t rval = 1;
        clock_t time = clock();
        for( uz_t i = 0; i < size; i++ )
        {
            rval = bcore_xsg_u2( rval );
            bcore_arr_st_s_push_st_d( st_arr1, st_s_createf( "%u", rval ) );
        }
        time = clock() - time;
        st_s_push_fa( log, "#p28.{bcore_arr_st_s_push_st_d } #<uz_t>ns\n", ( uz_t ) ( ( 1E9 * ( f3_t )time/CLOCKS_PER_SEC ) / size ) );
    }

    {
        u2_t rval = 1;
        clock_t time = clock();
        for( uz_t i = 0; i < size; i++ )
        {
            rval = bcore_xsg_u2( rval );
            bcore_array_a_push( (bcore_array*)st_arr2, sr_asm( st_s_createf( "%u", rval ) ) );
        }
        time = clock() - time;
        st_s_push_fa( log, "#p28.{bcore_array_a_push } #<uz_t>ns\n", ( uz_t ) ( ( 1E9 * ( f3_t )time/CLOCKS_PER_SEC ) / size ) );
    }

    {
        clock_t time = clock();
        bcore_arr_st_s_sort( st_arr1, 1 );
        time = clock() - time;
        st_s_push_fa( log, "#p28.{bcore_arr_st_s_sort } #<uz_t>ns\n", ( uz_t ) ( ( 1E9 * ( f3_t )time/CLOCKS_PER_SEC ) / size ) );
    }

    {
        clock_t time = clock();
        bcore_array_a_sort( (bcore_array*)st_arr2, 0, -1, 1 );
        time = clock() - time;
        st_s_push_fa( log, "#p28.{bcore_array_a_sort } #<uz_t>ns\n", ( uz_t ) ( ( 1E9 * ( f3_t )time/CLOCKS_PER_SEC ) / size ) );
    }

    bcore_arr_uz_s* permutation = bcore_arr_uz_s_create_random_permutation( bcore_xsg2_u2, 1, size );

    {
        clock_t time = clock();
        bcore_arr_st_s_reorder( st_arr1, permutation );
        time = clock() - time;
        st_s_push_fa( log, "#p28.{bcore_arr_st_s_reorder } #<uz_t>ns\n", ( uz_t ) ( ( 1E9 * ( f3_t )time/CLOCKS_PER_SEC ) / size ) );
    }

    {
        clock_t time = clock();
        bcore_array_a_reorder( (bcore_array*)st_arr2, permutation );
        time = clock() - time;
        st_s_push_fa( log, "#p28.{bcore_array_a_reorder } #<uz_t>ns\n", ( uz_t ) ( ( 1E9 * ( f3_t )time/CLOCKS_PER_SEC ) / size ) );
    }

    ASSERT( bcore_compare_aware( st_arr1, st_arr2 ) == 0 );

//    bcore_txt_ml_to_stdout( sr_awc( st_arr1 ) );

    bcore_arr_uz_s_discard( permutation );
    bcore_arr_st_s_discard( st_arr1 );
    bcore_arr_st_s_discard( st_arr2 );

    st_s_push_char_n( log, '=', 80 );
    st_s_push_char( log, '\n' );
    return log;

}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

vd_t bcore_arr_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_arr" ) ) )
    {
        case TYPEOF_init1:
        {
            bcore_flect_define_creator( typeof( "bcore_arr_uz_s" ), arr_uz_s_create_self );
            bcore_flect_define_creator( typeof( "bcore_arr_sz_s" ), arr_sz_s_create_self );
            bcore_flect_define_creator( typeof( "bcore_arr_u0_s" ), arr_u0_s_create_self );
            bcore_flect_define_creator( typeof( "bcore_arr_u3_s" ), arr_u3_s_create_self );
            bcore_flect_define_creator( typeof( "bcore_arr_s0_s" ), arr_s0_s_create_self );
            bcore_flect_define_creator( typeof( "bcore_arr_s3_s" ), arr_s3_s_create_self );
            bcore_flect_define_creator( typeof( "bcore_arr_f3_s" ), arr_f3_s_create_self );
            bcore_flect_define_creator( typeof( "bcore_arr_tp_s" ), arr_tp_s_create_self );
            bcore_flect_define_creator( typeof( "bcore_arr_bl_s" ), arr_bl_s_create_self );
            bcore_flect_define_creator( typeof( "bcore_arr_st_s" ), arr_st_s_create_self );
            bcore_flect_define_creator( typeof( "bcore_arr_vd_s" ), arr_vd_s_create_self );
            bcore_flect_define_creator( typeof( "bcore_arr_fp_s" ), arr_fp_s_create_self );
            bcore_flect_define_creator( typeof( "bcore_arr_sr_s" ), arr_sr_s_create_self );
        }
        break;

        case TYPEOF_selftest:
        {
            st_s* log = st_s_create();
            st_s_push_st_d( log, bcore_arr_uz_selftest() );
            st_s_push_st_d( log, bcore_arr_st_selftest() );
            return log;
        }
        break;

        default: break;
    }

    return NULL;
}

// ---------------------------------------------------------------------------------------------------------------------

