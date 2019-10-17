/** Copyright 2018 Johannes Bernhard Steffens
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

#include <stdio.h>

#include "bmath_arr_vf3.h"

/**********************************************************************************************************************/
// array of vf3_s

BCORE_DEFINE_OBJECT_INST( bcore_array, bmath_arr_vf3_s )
"{"
    "aware_t _;"
    "bmath_vf3_s [] arr;"
"}";

//----------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_clear( bmath_arr_vf3_s* o )
{
    bcore_array_a_set_space( ( bcore_array* )o, 0 );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_set_space(  bmath_arr_vf3_s* o, uz_t space )
{
    bcore_array_a_set_space( ( bcore_array* )o, space );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_set_size( bmath_arr_vf3_s* o, uz_t size )
{
    bcore_array_a_set_size( ( bcore_array* )o, size );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_push( bmath_arr_vf3_s* o, const bmath_vf3_s* vec )
{
    bcore_array_a_push( ( bcore_array* )o, sr_awc( vec ) );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_push_arr( bmath_arr_vf3_s* o, const bmath_arr_vf3_s* arr )
{
    bcore_array_a_push_array( ( bcore_array* )o, sr_awc( arr ) );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_pop( bmath_arr_vf3_s* o )
{
    bcore_array_a_pop( ( bcore_array* )o );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_reorder( bmath_arr_vf3_s* o, const bcore_arr_uz_s* order )
{
    bcore_array_a_reorder( ( bcore_array* )o, order );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_on_section_set_size( bmath_arr_vf3_s* o, uz_t start, uz_t end, uz_t size )
{
    end = end < o->size ? end : o->size;
    for( uz_t i = start; i < end; i++ ) bmath_vf3_s_set_size( &o->data[ i ], size );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_on_section_fill( bmath_arr_vf3_s* o, uz_t start, uz_t end, f3_t val )
{
    end = end < o->size ? end : o->size;
    for( uz_t i = start; i < end; i++ ) bmath_vf3_s_fill( &o->data[ i ], val );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_on_section_set_random( bmath_arr_vf3_s* o, uz_t start, uz_t end, f3_t density, f3_t min, f3_t max, u2_t* rval )
{
    end = end < o->size ? end : o->size;
    for( uz_t i = start; i < end; i++ ) bmath_vf3_s_set_random( &o->data[ i ], density, min, max, rval );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_swapr( bmath_vf3_s* o, bmath_vf3_s* v )
{
    f3_t* data  = o->data;
    uz_t  space = o->space;
    uz_t  size  = o->size;

    o->data  = v->data;
    o->size  = v->size;
    o->space = v->space;

    v->data  = data;
    v->size  = size;
    v->space = space;
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_on_section_zro( bmath_arr_vf3_s* o, uz_t start, uz_t end )
{
    end = end < o->size ? end : o->size;
    for( uz_t i = start; i < end; i++ ) bmath_vf3_s_zro( &o->data[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_on_section_neg( const bmath_arr_vf3_s* o, uz_t start, uz_t end, bmath_arr_vf3_s* res )
{
    end = end < o->size ? end : o->size;
    ASSERT( res->size >= end );
    for( uz_t i = start; i < end; i++ ) bmath_vf3_s_neg( &o->data[ i ], &res->data[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_on_section_cpy( const bmath_arr_vf3_s* o, uz_t start, uz_t end, bmath_arr_vf3_s* res )
{
    end = end < o->size ? end : o->size;
    ASSERT( res->size >= end );
    for( uz_t i = start; i < end; i++ ) bmath_vf3_s_cpy( &o->data[ i ], &res->data[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_on_section_add( const bmath_arr_vf3_s* o, uz_t start, uz_t end, const bmath_vf3_s* op, bmath_arr_vf3_s* res )
{
    end = end < o->size ? end : o->size;
    ASSERT( res->size >= end );
    for( uz_t i = start; i < end; i++ ) bmath_vf3_s_add( &o->data[ i ], op, &res->data[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_on_section_sub( const bmath_arr_vf3_s* o, uz_t start, uz_t end, const bmath_vf3_s* op, bmath_arr_vf3_s* res )
{
    end = end < o->size ? end : o->size;
    ASSERT( res->size >= end );
    for( uz_t i = start; i < end; i++ ) bmath_vf3_s_sub( &o->data[ i ], op, &res->data[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_on_section_mul_f3(  const bmath_arr_vf3_s* o, uz_t start, uz_t end, f3_t op, bmath_arr_vf3_s* res )
{
    end = end < o->size ? end : o->size;
    ASSERT( res->size >= end );
    for( uz_t i = start; i < end; i++ ) bmath_vf3_s_mul_f3( &o->data[ i ], op, &res->data[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_on_section_set_sqr( bmath_arr_vf3_s* o, uz_t start, uz_t end, f3_t val )
{
    end = end < o->size ? end : o->size;
    for( uz_t i = start; i < end; i++ ) bmath_vf3_s_set_sqr( &o->data[ i ], val );
}

void bmath_arr_vf3_s_on_section_set_sum( bmath_arr_vf3_s* o, uz_t start, uz_t end, f3_t val )
{
    end = end < o->size ? end : o->size;
    for( uz_t i = start; i < end; i++ ) bmath_vf3_s_set_sum( &o->data[ i ], val );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_on_section_set_avg( bmath_arr_vf3_s* o, uz_t start, uz_t end, f3_t val )
{
    end = end < o->size ? end : o->size;
    for( uz_t i = start; i < end; i++ ) bmath_vf3_s_set_avg( &o->data[ i ], val );
}

//----------------------------------------------------------------------------------------------------------------------

f3_t bmath_arr_vf3_s_on_section_f3_sum_sprec( const bmath_arr_vf3_s* o, uz_t start, uz_t end, uz_t index )
{
    end   = end < o->size ? end : o->size;
    start = start > end ? end : start;
    switch( end - start )
    {
        case 0: return 0;
        case 1: return bmath_vf3_s_get_f3( &o->data[ start ], index );

        case 2: return bmath_vf3_s_get_f3( &o->data[ start     ], index ) +
                       bmath_vf3_s_get_f3( &o->data[ start + 1 ], index );

        case 3: return bmath_vf3_s_get_f3( &o->data[ start     ], index ) +
                       bmath_vf3_s_get_f3( &o->data[ start + 1 ], index ) +
                       bmath_vf3_s_get_f3( &o->data[ start + 2 ], index );

        case 4: return bmath_vf3_s_get_f3( &o->data[ start     ], index ) +
                       bmath_vf3_s_get_f3( &o->data[ start + 1 ], index ) +
                       bmath_vf3_s_get_f3( &o->data[ start + 2 ], index ) +
                       bmath_vf3_s_get_f3( &o->data[ start + 3 ], index );

        default: break;
    }

    uz_t mid = start + ( ( end - start ) >> 1 );

    return bmath_arr_vf3_s_on_section_f3_sum_sprec( o, start, mid, index ) +
           bmath_arr_vf3_s_on_section_f3_sum_sprec( o, mid,   end, index );
}

//----------------------------------------------------------------------------------------------------------------------

f3_t bmath_arr_vf3_s_on_section_f3_sum_coprd_sprec( const bmath_arr_vf3_s* o, uz_t start, uz_t end, f3_t ai, f3_t aj, uz_t i, uz_t j )
{
    end   = end < o->size ? end : o->size;
    start = start > end ? end : start;
    switch( end - start )
    {
        case 0: return 0;
        case 1:
        {
            const bmath_vf3_s* v = &o->data[ start ];
            ASSERT( i < v->size && j < v->size );
            return ( v->data[ i ] - ai ) * ( v->data[ j ] - aj );
        }
        break;

        case 2:
        {
            const bmath_vf3_s* v1 = &o->data[ start     ];
            const bmath_vf3_s* v2 = &o->data[ start + 1 ];
            ASSERT( i < v1->size && j < v1->size );
            ASSERT( i < v2->size && j < v2->size );
            return ( v1->data[ i ] - ai ) * ( v1->data[ j ] - aj ) +
                   ( v2->data[ i ] - ai ) * ( v2->data[ j ] - aj );
        }
        break;

        case 3:
        {
            const bmath_vf3_s* v1 = &o->data[ start     ];
            const bmath_vf3_s* v2 = &o->data[ start + 1 ];
            const bmath_vf3_s* v3 = &o->data[ start + 2 ];
            ASSERT( i < v1->size && j < v1->size );
            ASSERT( i < v2->size && j < v2->size );
            ASSERT( i < v3->size && j < v3->size );
            return ( v1->data[ i ] - ai ) * ( v1->data[ j ] - aj ) +
                   ( v2->data[ i ] - ai ) * ( v2->data[ j ] - aj ) +
                   ( v3->data[ i ] - ai ) * ( v3->data[ j ] - aj );
        }
        break;

        case 4:
        {
            const bmath_vf3_s* v1 = &o->data[ start     ];
            const bmath_vf3_s* v2 = &o->data[ start + 1 ];
            const bmath_vf3_s* v3 = &o->data[ start + 2 ];
            const bmath_vf3_s* v4 = &o->data[ start + 3 ];
            ASSERT( i < v1->size && j < v1->size );
            ASSERT( i < v2->size && j < v2->size );
            ASSERT( i < v3->size && j < v3->size );
            ASSERT( i < v4->size && j < v4->size );
            return ( v1->data[ i ] - ai ) * ( v1->data[ j ] - aj ) +
                   ( v2->data[ i ] - ai ) * ( v2->data[ j ] - aj ) +
                   ( v3->data[ i ] - ai ) * ( v3->data[ j ] - aj ) +
                   ( v4->data[ i ] - ai ) * ( v4->data[ j ] - aj );
        }
        break;

        default: break;
    }

    uz_t mid = start + ( ( end - start ) >> 1 );

    return bmath_arr_vf3_s_on_section_f3_sum_coprd_sprec( o, start, mid, ai, aj, i, j ) +
           bmath_arr_vf3_s_on_section_f3_sum_coprd_sprec( o, mid,   end, ai, aj, i, j );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_on_section_get_sum_sprc( const bmath_arr_vf3_s* o, uz_t start, uz_t end, bmath_vf3_s* res )
{
    for( uz_t i = 0; i < res->size; i++ ) res->data[ i ] = bmath_arr_vf3_s_on_section_f3_sum_sprec( o, start, end, i );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_on_section_get_avg_sprc( const bmath_arr_vf3_s* o, uz_t start, uz_t end, bmath_vf3_s* res )
{
    end = end < o->size ? end : o->size;
    start = start > end ? end : start;
    bmath_arr_vf3_s_on_section_get_sum_sprc( o, start, end, res );
    if( end > start ) bmath_vf3_s_mul_f3( res, 1.0 / (end - start), res );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_on_section_get_sum_fast( const bmath_arr_vf3_s* o, uz_t start, uz_t end, bmath_vf3_s* res )
{
    end = end < o->size ? end : o->size;
    bmath_vf3_s_zro( res );
    for( uz_t i = start; i < end; i++ ) bmath_vf3_s_add( &o->data[ i ], res, res );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_on_section_get_avg_fast( const bmath_arr_vf3_s* o, uz_t start, uz_t end, bmath_vf3_s* res )
{
    end = end < o->size ? end : o->size;
    start = start > end ? end : start;
    bmath_arr_vf3_s_on_section_get_sum_fast( o, start, end, res );
    if( end > start ) bmath_vf3_s_mul_f3( res, 1.0 / (end - start), res );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

static vd_t selftest( void )
{
    bcore_life_s* l = bcore_life_s_create();

    /// bmath_arr_vf3_s
    {
        uz_t size = 10;
        uz_t n    = 5;
        bmath_arr_vf3_s* a1 = bcore_life_s_push_aware( l, bmath_arr_vf3_s_create() );
        bmath_vf3_s*     v1 = bcore_life_s_push_aware( l, bmath_vf3_s_create() );
        bmath_vf3_s*     v2 = bcore_life_s_push_aware( l, bmath_vf3_s_create() );

        bmath_arr_vf3_s_set_size( a1, size );
        bmath_vf3_s_set_size( v1, n );
        bmath_vf3_s_set_size( v2, n );

        bmath_arr_vf3_s_on_section_set_size( a1, 0, -1, n );
        u2_t rval = 123;
        bmath_arr_vf3_s_on_section_set_random( a1, 0, -1, 1.0, -1, 1, &rval );
        bmath_arr_vf3_s_on_section_set_sqr( a1, 0, -1, 4 );
        for( uz_t i = 0; i < size; i++ ) ASSERT( f3_abs( bmath_vf3_s_f3_sqr( &a1->data[ i ] ) - 4.0 ) < 1E-10 );
        bmath_arr_vf3_s_on_section_set_avg( a1, 0, -1, 2 );
        bmath_arr_vf3_s_on_section_get_avg_fast( a1, 0, -1, v1 );
        ASSERT( f3_abs( bmath_vf3_s_f3_avg( v1 ) - 2 ) < 1E-10 );
        bmath_arr_vf3_s_on_section_get_avg_sprc( a1, 0, -1, v2 );
        ASSERT( bmath_vf3_s_is_near_equ( v1, v2, 1E-10 ) );
    }

    bcore_life_s_discard( l );

    return NULL;
}

/**********************************************************************************************************************/

vd_t bmath_arr_vf3_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_arr_vf3" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_OBJECT( bmath_arr_vf3_s );
        }
        break;

        case TYPEOF_selftest:
        {
            return selftest();
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

