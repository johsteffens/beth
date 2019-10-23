/** Author and Copyright 2018 Johannes Bernhard Steffens
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

/**********************************************************************************************************************/

/** Template: Array of Vector */

/**********************************************************************************************************************/

#include "bmath_template_fx_begin.h"

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_arr_vfx_s,clear)( bmath_arr_vfx_s* o )
{
    bcore_array_a_set_space( ( bcore_array* )o, 0 );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_arr_vfx_s,set_space)(  bmath_arr_vfx_s* o, uz_t space )
{
    bcore_array_a_set_space( ( bcore_array* )o, space );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_arr_vfx_s,set_size)( bmath_arr_vfx_s* o, uz_t size )
{
    bcore_array_a_set_size( ( bcore_array* )o, size );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_arr_vfx_s,push)( bmath_arr_vfx_s* o, const bmath_vfx_s* vec )
{
    bcore_array_a_push( ( bcore_array* )o, sr_awc( vec ) );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_arr_vfx_s,push_arr)( bmath_arr_vfx_s* o, const bmath_arr_vfx_s* arr )
{
    bcore_array_a_push_array( ( bcore_array* )o, sr_awc( arr ) );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_arr_vfx_s,pop)( bmath_arr_vfx_s* o )
{
    bcore_array_a_pop( ( bcore_array* )o );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_arr_vfx_s,reorder)( bmath_arr_vfx_s* o, const bcore_arr_uz_s* order )
{
    bcore_array_a_reorder( ( bcore_array* )o, order );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_arr_vfx_s,on_section_set_size)( bmath_arr_vfx_s* o, uz_t start, uz_t end, uz_t size )
{
    end = end < o->size ? end : o->size;
    for( uz_t i = start; i < end; i++ ) BCATU(bmath_vfx_s,set_size)( &o->data[ i ], size );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_arr_vfx_s,on_section_fill)( bmath_arr_vfx_s* o, uz_t start, uz_t end, fx_t val )
{
    end = end < o->size ? end : o->size;
    for( uz_t i = start; i < end; i++ ) BCATU(bmath_vfx_s,fill)( &o->data[ i ], val );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_arr_vfx_s,on_section_set_random)( bmath_arr_vfx_s* o, uz_t start, uz_t end, fx_t density, fx_t min, fx_t max, u2_t* rval )
{
    end = end < o->size ? end : o->size;
    for( uz_t i = start; i < end; i++ ) BCATU(bmath_vfx_s,set_random)( &o->data[ i ], density, min, max, rval );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,swapr)( bmath_vfx_s* o, bmath_vfx_s* v )
{
    fx_t* data  = o->data;
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

void BCATU(bmath_arr_vfx_s,on_section_zro)( bmath_arr_vfx_s* o, uz_t start, uz_t end )
{
    end = end < o->size ? end : o->size;
    for( uz_t i = start; i < end; i++ ) BCATU(bmath_vfx_s,zro)( &o->data[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_arr_vfx_s,on_section_neg)( const bmath_arr_vfx_s* o, uz_t start, uz_t end, bmath_arr_vfx_s* res )
{
    end = end < o->size ? end : o->size;
    ASSERT( res->size >= end );
    for( uz_t i = start; i < end; i++ ) BCATU(bmath_vfx_s,neg)( &o->data[ i ], &res->data[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_arr_vfx_s,on_section_cpy)( const bmath_arr_vfx_s* o, uz_t start, uz_t end, bmath_arr_vfx_s* res )
{
    end = end < o->size ? end : o->size;
    ASSERT( res->size >= end );
    for( uz_t i = start; i < end; i++ ) BCATU(bmath_vfx_s,cpy)( &o->data[ i ], &res->data[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_arr_vfx_s,on_section_add)( const bmath_arr_vfx_s* o, uz_t start, uz_t end, const bmath_vfx_s* op, bmath_arr_vfx_s* res )
{
    end = end < o->size ? end : o->size;
    ASSERT( res->size >= end );
    for( uz_t i = start; i < end; i++ ) BCATU(bmath_vfx_s,add)( &o->data[ i ], op, &res->data[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_arr_vfx_s,on_section_sub)( const bmath_arr_vfx_s* o, uz_t start, uz_t end, const bmath_vfx_s* op, bmath_arr_vfx_s* res )
{
    end = end < o->size ? end : o->size;
    ASSERT( res->size >= end );
    for( uz_t i = start; i < end; i++ ) BCATU(bmath_vfx_s,sub)( &o->data[ i ], op, &res->data[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_arr_vfx_s,on_section_mul_fx)(  const bmath_arr_vfx_s* o, uz_t start, uz_t end, fx_t op, bmath_arr_vfx_s* res )
{
    end = end < o->size ? end : o->size;
    ASSERT( res->size >= end );
    for( uz_t i = start; i < end; i++ ) BCATU(bmath_vfx_s,mul_fx)( &o->data[ i ], op, &res->data[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_arr_vfx_s,on_section_set_sqr)( bmath_arr_vfx_s* o, uz_t start, uz_t end, fx_t val )
{
    end = end < o->size ? end : o->size;
    for( uz_t i = start; i < end; i++ ) BCATU(bmath_vfx_s,set_sqr)( &o->data[ i ], val );
}

void BCATU(bmath_arr_vfx_s,on_section_set_sum)( bmath_arr_vfx_s* o, uz_t start, uz_t end, fx_t val )
{
    end = end < o->size ? end : o->size;
    for( uz_t i = start; i < end; i++ ) BCATU(bmath_vfx_s,set_sum)( &o->data[ i ], val );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_arr_vfx_s,on_section_set_avg)( bmath_arr_vfx_s* o, uz_t start, uz_t end, fx_t val )
{
    end = end < o->size ? end : o->size;
    for( uz_t i = start; i < end; i++ ) BCATU(bmath_vfx_s,set_avg)( &o->data[ i ], val );
}

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath_arr_vfx_s,on_section_fx_sum_sprec)( const bmath_arr_vfx_s* o, uz_t start, uz_t end, uz_t index )
{
    end   = end < o->size ? end : o->size;
    start = start > end ? end : start;
    switch( end - start )
    {
        case 0: return 0;
        case 1: return BCATU(bmath_vfx_s,get_fx)( &o->data[ start ], index );

        case 2: return BCATU(bmath_vfx_s,get_fx)( &o->data[ start     ], index ) +
                       BCATU(bmath_vfx_s,get_fx)( &o->data[ start + 1 ], index );

        case 3: return BCATU(bmath_vfx_s,get_fx)( &o->data[ start     ], index ) +
                       BCATU(bmath_vfx_s,get_fx)( &o->data[ start + 1 ], index ) +
                       BCATU(bmath_vfx_s,get_fx)( &o->data[ start + 2 ], index );

        case 4: return BCATU(bmath_vfx_s,get_fx)( &o->data[ start     ], index ) +
                       BCATU(bmath_vfx_s,get_fx)( &o->data[ start + 1 ], index ) +
                       BCATU(bmath_vfx_s,get_fx)( &o->data[ start + 2 ], index ) +
                       BCATU(bmath_vfx_s,get_fx)( &o->data[ start + 3 ], index );

        default: break;
    }

    uz_t mid = start + ( ( end - start ) >> 1 );

    return BCATU(bmath_arr_vfx_s,on_section_fx_sum_sprec)( o, start, mid, index ) +
           BCATU(bmath_arr_vfx_s,on_section_fx_sum_sprec)( o, mid,   end, index );
}

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath_arr_vfx_s,on_section_fx_sum_coprd_sprec)( const bmath_arr_vfx_s* o, uz_t start, uz_t end, fx_t ai, fx_t aj, uz_t i, uz_t j )
{
    end   = end < o->size ? end : o->size;
    start = start > end ? end : start;
    switch( end - start )
    {
        case 0: return 0;
        case 1:
        {
            const bmath_vfx_s* v = &o->data[ start ];
            ASSERT( i < v->size && j < v->size );
            return ( v->data[ i ] - ai ) * ( v->data[ j ] - aj );
        }
        break;

        case 2:
        {
            const bmath_vfx_s* v1 = &o->data[ start     ];
            const bmath_vfx_s* v2 = &o->data[ start + 1 ];
            ASSERT( i < v1->size && j < v1->size );
            ASSERT( i < v2->size && j < v2->size );
            return ( v1->data[ i ] - ai ) * ( v1->data[ j ] - aj ) +
                   ( v2->data[ i ] - ai ) * ( v2->data[ j ] - aj );
        }
        break;

        case 3:
        {
            const bmath_vfx_s* v1 = &o->data[ start     ];
            const bmath_vfx_s* v2 = &o->data[ start + 1 ];
            const bmath_vfx_s* v3 = &o->data[ start + 2 ];
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
            const bmath_vfx_s* v1 = &o->data[ start     ];
            const bmath_vfx_s* v2 = &o->data[ start + 1 ];
            const bmath_vfx_s* v3 = &o->data[ start + 2 ];
            const bmath_vfx_s* v4 = &o->data[ start + 3 ];
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

    return BCATU(bmath_arr_vfx_s,on_section_fx_sum_coprd_sprec)( o, start, mid, ai, aj, i, j ) +
           BCATU(bmath_arr_vfx_s,on_section_fx_sum_coprd_sprec)( o, mid,   end, ai, aj, i, j );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_arr_vfx_s,on_section_get_sum_sprc)( const bmath_arr_vfx_s* o, uz_t start, uz_t end, bmath_vfx_s* res )
{
    for( uz_t i = 0; i < res->size; i++ ) res->data[ i ] = BCATU(bmath_arr_vfx_s,on_section_fx_sum_sprec)( o, start, end, i );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_arr_vfx_s,on_section_get_avg_sprc)( const bmath_arr_vfx_s* o, uz_t start, uz_t end, bmath_vfx_s* res )
{
    end = end < o->size ? end : o->size;
    start = start > end ? end : start;
    BCATU(bmath_arr_vfx_s,on_section_get_sum_sprc)( o, start, end, res );
    if( end > start ) BCATU(bmath_vfx_s,mul_fx)( res, 1.0 / (end - start), res );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_arr_vfx_s,on_section_get_sum_fast)( const bmath_arr_vfx_s* o, uz_t start, uz_t end, bmath_vfx_s* res )
{
    end = end < o->size ? end : o->size;
    BCATU(bmath_vfx_s,zro)( res );
    for( uz_t i = start; i < end; i++ ) BCATU(bmath_vfx_s,add)( &o->data[ i ], res, res );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_arr_vfx_s,on_section_get_avg_fast)( const bmath_arr_vfx_s* o, uz_t start, uz_t end, bmath_vfx_s* res )
{
    end = end < o->size ? end : o->size;
    start = start > end ? end : start;
    BCATU(bmath_arr_vfx_s,on_section_get_sum_fast)( o, start, end, res );
    if( end > start ) BCATU(bmath_vfx_s,mul_fx)( res, 1.0 / (end - start), res );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

static vd_t selftest( void )
{
    bcore_life_s* l = bcore_life_s_create();

    /// bmath_arr_vfx_s
    {
        uz_t size = 10;
        uz_t n    = 5;
        bmath_arr_vfx_s* a1 = bcore_life_s_push_aware( l, BCATU(bmath_arr_vfx_s,create)() );
        bmath_vfx_s*     v1 = bcore_life_s_push_aware( l, BCATU(bmath_vfx_s,create)() );
        bmath_vfx_s*     v2 = bcore_life_s_push_aware( l, BCATU(bmath_vfx_s,create)() );

        BCATU(bmath_arr_vfx_s,set_size)( a1, size );
        BCATU(bmath_vfx_s,set_size)( v1, n );
        BCATU(bmath_vfx_s,set_size)( v2, n );

        fx_t max_dev = ( BMATH_TEMPLATE_FX_PREC == 2 ) ? 1E-6 : 1E-10;

        BCATU(bmath_arr_vfx_s,on_section_set_size)( a1, 0, -1, n );
        u2_t rval = 123;
        BCATU(bmath_arr_vfx_s,on_section_set_random)( a1, 0, -1, 1.0, -1, 1, &rval );
        BCATU(bmath_arr_vfx_s,on_section_set_sqr)( a1, 0, -1, 4 );
        for( uz_t i = 0; i < size; i++ ) ASSERT( BCATU(fx,abs)( BCATU(bmath_vfx_s,fx_sqr)( &a1->data[ i ] ) - 4.0 ) < max_dev );
        BCATU(bmath_arr_vfx_s,on_section_set_avg)( a1, 0, -1, 2 );
        BCATU(bmath_arr_vfx_s,on_section_get_avg_fast)( a1, 0, -1, v1 );
        ASSERT( BCATU(fx,abs)( BCATU(bmath_vfx_s,fx_avg)( v1 ) - 2 ) < max_dev );
        BCATU(bmath_arr_vfx_s,on_section_get_avg_sprc)( a1, 0, -1, v2 );
        ASSERT( BCATU(bmath_vfx_s,is_near_equ)( v1, v2, max_dev ) );
    }

    bcore_life_s_discard( l );

    return NULL;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#include "bmath_template_fx_end.h"

/**********************************************************************************************************************/

