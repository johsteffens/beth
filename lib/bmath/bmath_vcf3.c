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

#include "bmath_vcf3.h"
#include "bmath_spect_vector.h"
#include "bmath_fourier.h"

/**********************************************************************************************************************/
// bmath_vcf3_s

BCORE_DEFINE_OBJECT_INST( bmath_vector, bmath_vcf3_s )
"{"
    "aware_t _;"
    "bmath_cf3_s [];"
    "func bmath_fp:add;"
    "func bmath_fp:zro;"
    "func bmath_fp:neg;"
    "func bmath_fp:sub;"
    "func bmath_fp:cpy;"
    "func bmath_fp_vector:mul_scl = bmath_vcf3_s_mul_scl;"
    "func bmath_fp_vector:mul_vec = bmath_vcf3_s_mul_vec;"
"}";

//----------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_move( bmath_vcf3_s* o, bmath_vcf3_s* src )
{
    if( o == src ) return;
    *o = *src;
    o->space = 0;
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_set_size( bmath_vcf3_s* o, uz_t size )
{
    bcore_array_a_set_size( (bcore_array*)o, size );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_set_space( bmath_vcf3_s* o, uz_t space )
{
    bcore_array_a_set_space( (bcore_array*)o, space );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_cvf3_s_push( bmath_vcf3_s* o, bmath_cf3_s val )
{
    if( o->size < o->space )
    {
        o->data[ o->size++ ] = val;
    }
    else
    {
        bcore_array_a_push( (bcore_array*)o, sr_twc( TYPEOF_bmath_cf3_s, &val ) );
    }
}

void bmath_vcf3_s_push_ri( bmath_vcf3_s* o, f3_t r, f3_t i )
{
    bmath_cvf3_s_push( o, bmath_cf3_init( r, i ) );
}

//----------------------------------------------------------------------------------------------------------------------

bmath_vcf3_s* bmath_vcf3_s_create_size( uz_t size )
{
    bmath_vcf3_s* o = bmath_vcf3_s_create();
    bcore_array_a_set_size( (bcore_array*)o, size );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

bmath_vcf3_s* bmath_vcf3_s_create_fill( bmath_cf3_s val, uz_t size )
{
    bmath_vcf3_s* o = bmath_vcf3_s_create();
    bcore_array_a_set_space( (bcore_array*)o, size );
    o->size = size;
    for( uz_t i = 0; i < size; i++ ) o->data[ i ] = val;
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_zro( bmath_vcf3_s* o )
{
    bcore_u_memzero( sizeof( bmath_cf3_s ), o->data, o->size );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_neg( const bmath_vcf3_s* o, bmath_vcf3_s* res )
{
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ ) bmath_cf3_s_neg( &o->data[ i ], &res->data[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_cnj( const bmath_vcf3_s* o, bmath_vcf3_s* res )
{
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ ) bmath_cf3_s_cnj( &o->data[ i ], &res->data[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_cpy( const bmath_vcf3_s* o, bmath_vcf3_s* res )
{
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ ) res->data[ i ] = o->data[ i ];
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_add( const bmath_vcf3_s* o, const bmath_vcf3_s* op, bmath_vcf3_s* res )
{
    ASSERT( o->size == op->size );
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ ) bmath_cf3_s_add( &o->data[ i ], &op->data[ i ], &res->data[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_sub( const bmath_vcf3_s* o, const bmath_vcf3_s* op, bmath_vcf3_s* res )
{
    ASSERT( o->size == op->size );
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ ) bmath_cf3_s_sub( &o->data[ i ], &op->data[ i ], &res->data[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_mul_scl( const bmath_vcf3_s* o, const bmath_cf3_s* op, bmath_vcf3_s* res )
{
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ ) bmath_cf3_s_mul( &o->data[ i ], op, &res->data[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_mul_cf3( const bmath_vcf3_s* o, bmath_cf3_s op, bmath_vcf3_s* res )
{
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ ) bmath_cf3_s_mul( &o->data[ i ], &op, &res->data[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_mul_f3( const bmath_vcf3_s* o, f3_t op, bmath_vcf3_s* res )
{
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ ) bmath_cf3_s_mul_f3( &o->data[ i ], op, &res->data[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_dft( const bmath_vcf3_s* o, bmath_vcf3_s* res )
{
    ASSERT( o->size == res->size );
    if( ( o->size & ( o->size - 1 ) ) == 0 )
    {
        bmath_fourier_fft_f3( o->data, res->data, o->size );
    }
    else
    {
        bmath_fourier_dft_f3( o->data, res->data, o->size );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_ift( const bmath_vcf3_s* o, bmath_vcf3_s* res )
{
    bmath_vcf3_s_cnj( o, res );
    bmath_vcf3_s_dft( res, res );
    bmath_vcf3_s_cnj( res, res );
    if( res->size > 0 ) bmath_vcf3_s_mul_f3( res, 1.0 / res->size, res );
}

//----------------------------------------------------------------------------------------------------------------------

static void vcf3_s_mul_vec( const bmath_cf3_s* v1, const bmath_cf3_s* v2, uz_t size, bmath_cf3_s* res )
{
    switch( size )
    {
        case 0:
        {
            bmath_cf3_s_zro( res );
            return;
        }

        case 1:
        {
            bmath_cf3_s_mul( &v1[0], &v2[0], res );
            return;
        }

        case 2:
        {
            bmath_cf3_s_mul( &v1[0], &v2[0], res );
            bmath_cf3_s_add_mul( res, &v1[1], &v2[1], res );
            return;
        }

        default: break;
    }

    uz_t sz1 = size >> 1;
    bmath_cf3_s val;
    vcf3_s_mul_vec( v1, v2, sz1, res );
    vcf3_s_mul_vec( v1 + sz1, v2 + sz1, size - sz1, &val );
    bmath_cf3_s_add( res, &val, res );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_mul_vec( const bmath_vcf3_s* o, const bmath_vcf3_s* op, bmath_cf3_s* res )
{
    ASSERT( o->size == op->size );
    vcf3_s_mul_vec( o->data, op->data, o->size, res );
}

//----------------------------------------------------------------------------------------------------------------------

static void vcf3_s_sqr( const bmath_cf3_s* o, uz_t size, bmath_cf3_s* res )
{
    switch( size )
    {
        case 0:
        {
            bmath_cf3_s_zro( res );
            return;
        }

        case 1:
        {
            bmath_cf3_s_sqr( &o[0], res );
            return;
        }

        case 2:
        {
            bmath_cf3_s_sqr( &o[0], res );
            bmath_cf3_s_sqr_add( &o[1], res, res );
            return;
        }

        default: break;
    }

    uz_t sz1 = size >> 1;
    bmath_cf3_s val;
    vcf3_s_sqr( o, sz1, res );
    vcf3_s_sqr( o + sz1, size - sz1, &val );
    bmath_cf3_s_add( res, &val, res );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_sqr( const bmath_vcf3_s* o, bmath_cf3_s* res )
{
    vcf3_s_sqr( o->data, o->size, res );
}

//----------------------------------------------------------------------------------------------------------------------

static void vcf3_s_sub_sqr( const bmath_cf3_s* v1, const bmath_cf3_s* v2, uz_t size, bmath_cf3_s* res )
{
    switch( size )
    {
        case 0:
        {
            bmath_cf3_s_zro( res );
            return;
        }

        case 1:
        {
            bmath_cf3_s_sub_sqr( &v1[0], &v2[0], res );
            return;
        }

        case 2:
        {
            bmath_cf3_s val;
            bmath_cf3_s_sub_sqr( &v1[0], &v2[0], res );
            bmath_cf3_s_sub_sqr( &v1[1], &v2[1], &val );
            bmath_cf3_s_add( res, &val, res );
            return;
        }

        default: break;
    }

    uz_t sz1 = size >> 1;
    bmath_cf3_s val;
    vcf3_s_sub_sqr( v1, v2, sz1, res );
    vcf3_s_sub_sqr( v1 + sz1, v2 + sz1, size - sz1, &val );
    bmath_cf3_s_add( res, &val, res );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_sub_sqr( const bmath_vcf3_s* o, const bmath_vcf3_s* op, bmath_cf3_s* res )
{
    ASSERT( o->size == op->size );
    vcf3_s_sub_sqr( o->data, op->data, o->size, res );
}

//----------------------------------------------------------------------------------------------------------------------

static void vcf3_s_sum( const bmath_cf3_s* v1, uz_t size, bmath_cf3_s* res )
{
    switch( size )
    {
        case 0:
        {
            bmath_cf3_s_zro( res );
            return;
        }

        case 1:
        {
            *res = v1[0];
            return;
        }

        case 2:
        {
            res->v[0]=v1[0].v[0]+v1[1].v[0];
            res->v[1]=v1[0].v[1]+v1[1].v[1];
            return;
        }

        case 3:
        {
            res->v[0]=v1[0].v[0]+v1[1].v[0]+v1[2].v[0];
            res->v[1]=v1[0].v[1]+v1[1].v[1]+v1[2].v[1];
            return;
        }

        case 4:
        {
            res->v[0]=v1[0].v[0]+v1[1].v[0]+v1[2].v[0]+v1[3].v[0];
            res->v[1]=v1[0].v[1]+v1[1].v[1]+v1[2].v[1]+v1[3].v[1];
            return;
        }

        default: break;
    }

    uz_t sz1 = size >> 1;
    bmath_cf3_s val;
    vcf3_s_sum( v1, sz1, res );
    vcf3_s_sum( v1 + sz1, size - sz1, &val );
    bmath_cf3_s_add( res, &val, res );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_sum( const bmath_vcf3_s* o, bmath_cf3_s* res )
{
    vcf3_s_sum( o->data, o->size, res );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_f3_avg( const bmath_vcf3_s* o, bmath_cf3_s* res )
{
    if( o->size > 0 )
    {
        bmath_vcf3_s_sum( o, res );
        bmath_cf3_s_mul_f3( res, 1.0 / o->size, res );
    }
    else
    {
        bmath_cf3_s_zro( res );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_to_stdout( const bmath_vcf3_s* o )
{
    const bmath_cf3_s* v = o->data;
    for( uz_t i = 0; i < o->size; i++ ) bmath_cf3_s_to_stdout( &v[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

static vd_t selftest( void )
{
    bcore_life_s* l = bcore_life_s_create();

    /// cvf3
    {
        uz_t size = 100;
        bmath_vcf3_s* v1 = bcore_life_s_push_aware( l, bmath_vcf3_s_create_size( size ) );
        for( uz_t i = 0; i < size; i++ )
        {
            v1->data[ i ].v[ 0 ] = i + 1;
            v1->data[ i ].v[ 1 ] = i;
        }

//        bcore_txt_ml_aware_to_stdout( v1 );

        bmath_cf3_s sum1 = bmath_cf3_zro();
        bmath_cf3_s sqr1 = bmath_cf3_zro();
        for( uz_t i = 0; i < size; i++ )
        {
            bmath_cf3_s_sqr_add( &v1->data[ i ], &sqr1, &sqr1 );
            bmath_cf3_s_add( &v1->data[ i ], &sum1, &sum1  );
        }

//        bcore_txt_ml_to_stdout( sr_twc( TYPEOF_bmath_cf3_s, &sum1 ) );
//        bcore_txt_ml_to_stdout( sr_twc( TYPEOF_bmath_cf3_s, &sqr1 ) );

        bmath_cf3_s sum2 = bmath_cf3_zro();
        bmath_cf3_s sqr2 = bmath_cf3_zro();
        bmath_vcf3_s_sum( v1, &sum2 );
        bmath_vcf3_s_sqr( v1, &sqr2 );

        ASSERT( bmath_cf3_equ( sum1, sum2 ) );
        ASSERT( bmath_cf3_equ( sqr1, sqr2 ) );

        bmath_vcf3_s* v2 = bcore_life_s_push_aware( l, bmath_vcf3_s_create_size( size ) );
        bmath_vcf3_s_cpy( v1, v2 );
        bmath_vcf3_s_sum( v1, &sum1 );
        bmath_vcf3_s_sum( v2, &sum2 );
        ASSERT( bmath_cf3_equ( sum1, sum2 ) );

        bmath_vcf3_s_neg( v1, v2 );
        bmath_vcf3_s_sum( v1, &sum1 );
        bmath_vcf3_s_sum( v2, &sum2 );
        ASSERT( bmath_cf3_equ( sum1, bmath_cf3_neg( sum2 ) ) );

        bmath_vcf3_s_mul_f3( v1, 2.0, v2 );
        bmath_vcf3_s_sum( v1, &sum1 );
        bmath_vcf3_s_sum( v2, &sum2 );
        ASSERT( bmath_cf3_equ( sum2, bmath_cf3_mul_f3( sum1, 2.0 ) ) );
        bmath_vcf3_s_sqr( v1, &sqr1 );
        bmath_vcf3_s_mul_vec( v1, v2, &sqr2 );
        ASSERT( bmath_cf3_equ( sqr2, bmath_cf3_mul_f3( sqr1, 2.0 ) ) );
        bmath_vector_a_mul_vec( ( const bmath_vector* )v1, ( const bmath_vector* )v2, ( bmath_ring* )&sqr2 );
        ASSERT( bmath_cf3_equ( sqr2, bmath_cf3_mul_f3( sqr1, 2.0 ) ) );

        bmath_vcf3_s_dft( v1, v2 );
        bmath_vcf3_s_ift( v2, v2 );
        bmath_vcf3_s_sub_sqr( v1, v2, &sqr1 );
        ASSERT( bmath_cf3_mag( sqr1 ) < 1E-14 );
    }

    bcore_life_s_discard( l );

    return NULL;
}

/**********************************************************************************************************************/

vd_t bmath_vcf3_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_vcf3" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_FFUNC( bmath_fp_add,            bmath_vcf3_s_add );
            BCORE_REGISTER_FFUNC( bmath_fp_zro,            bmath_vcf3_s_zro );
            BCORE_REGISTER_FFUNC( bmath_fp_neg,            bmath_vcf3_s_neg );
            BCORE_REGISTER_FFUNC( bmath_fp_sub,            bmath_vcf3_s_sub );
            BCORE_REGISTER_FFUNC( bmath_fp_cpy,            bmath_vcf3_s_cpy );
            BCORE_REGISTER_FFUNC( bmath_fp_vector_mul_vec, bmath_vcf3_s_mul_vec );
            BCORE_REGISTER_FFUNC( bmath_fp_vector_mul_scl, bmath_vcf3_s_mul_scl );
            BCORE_REGISTER_OBJECT( bmath_vcf3_s );
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

