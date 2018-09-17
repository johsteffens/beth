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

#include "bmath_mf3_eval.h"

/**********************************************************************************************************************/

BCORE_DEFINE_OBJECT_INST( bcore_inst, bmath_mf3_eval_s )
"{"
    "aware_t _;"
    "uz_t rows       = 1000;"
    "uz_t cols       = 1000;"

    "u2_t seed       = 1234567;"
    "f3_t density    = 1.0;"
    "bl_t full       = false;"
    "f3_t near_limit = 1E-8;"
    "f3_t eps        = 1E-8;"  // for function requiring an epsilon

    "bl_t log_a      = false;" // log matrix a after conversion
    "bl_t log_u      = false;" // log matrix u after conversion
    "bl_t log_v      = false;" // log matrix v after conversion
    "bl_t assert_all = true; " // asserts correct matrix and computation result

    "bl_t test0 = true;"  // runs minimal parameter test
    "bl_t test1 = true;"  // runs default parameter test
"}";

//----------------------------------------------------------------------------------------------------------------------

BCORE_DEFINE_OBJECT_INST( bcore_inst, bmath_mf3_eval_result_s )
"{"
    "aware_t _;"
    "tp_t fp_type;"
    "uz_t rows;"
    "uz_t cols;"
    "f3_t density;"

    "st_s log_a;"           // log of matrix a
    "st_s log_u;"           // log of matrix u
    "st_s log_v;"           // log of matrix v
    "bl_t assert_a = true;" // assertion of shape a where applicable
    "bl_t assert_u = true;" // assertion of shape u where applicable
    "bl_t assert_v = true;" // assertion of shape v where applicable
    "bl_t assert_m = true;" // assertion of m equals u * a * vT
    "f3_t fdev_a = 0;"      // fdev of a from desired shape
    "f3_t fdev_u = 0;"      // fdev of u from desired shape
    "f3_t fdev_v = 0;"      // fdev of v from desired shape
    "f3_t fdev_m = 0;"      // fdev of m from equality
    "bl_t success0 = true;" // success with minimal arguments
    "bl_t success1 = true;" // success with all arguments
    "f3_t time0  = 0;"      // operation time with minimal arguments
    "f3_t time1  = 0;"      // operation time with all arguments
"}";

//----------------------------------------------------------------------------------------------------------------------

static void bmath_mf3_eval_result_s_set_defaults_from_eval( bmath_mf3_eval_result_s* o, const bmath_mf3_eval_s* v )
{
    o->rows    = v->rows;
    o->cols    = v->cols;
    o->density = v->density;
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mf3_eval_result_s_to_string( const bmath_mf3_eval_result_s* o, st_s* string )
{
    if( !string ) return;

    st_s_push_fa( string, "fp_type .... #<sc_t> (#<uz_t> x #<uz_t>) density = #<f3_t>\n", ifnameof( o->fp_type ), o->rows, o->cols, o->density );
    if( o->fdev_a > 0 ) st_s_push_fa( string, "fdev_a ..... #<f3_t>\n", o->fdev_a );
    if( o->fdev_u > 0 ) st_s_push_fa( string, "fdev_u ..... #<f3_t>\n", o->fdev_u );
    if( o->fdev_v > 0 ) st_s_push_fa( string, "fdev_v ..... #<f3_t>\n", o->fdev_v );
    if( o->fdev_m > 0 ) st_s_push_fa( string, "fdev_m ..... #<f3_t>\n", o->fdev_m );
    if( o->time0  > 0 ) st_s_push_fa( string, "time0 ...... #<f3_t>s\n", o->time0 );
    if( o->time1  > 0 ) st_s_push_fa( string, "time1 ...... #<f3_t>s\n", o->time1 );

    if( !o->success0 ) st_s_push_fa( string, "success0 ... false\n" );
    if( !o->success1 ) st_s_push_fa( string, "success1 ... false\n" );
    if( !o->assert_a ) st_s_push_fa( string, "assert_a ... false\n" );
    if( !o->assert_u ) st_s_push_fa( string, "assert_u ... false\n" );
    if( !o->assert_v ) st_s_push_fa( string, "assert_v ... false\n" );
    if( !o->assert_m ) st_s_push_fa( string, "assert_m ... false\n" );

    if( o->log_a.size > 0 ) st_s_push_fa( string, "Matrix a #<sc_t>", o->log_a.sc );
    if( o->log_u.size > 0 ) st_s_push_fa( string, "Matrix u #<sc_t>", o->log_u.sc );
    if( o->log_v.size > 0 ) st_s_push_fa( string, "Matrix v #<sc_t>", o->log_v.sc );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mf3_eval_result_s_to_stdout( const bmath_mf3_eval_result_s* o )
{
    st_s* s = st_s_create();
    bmath_mf3_eval_result_s_to_string( o, s );
    bcore_msg_fa( "#<sc_t>\n", s->sc );
    st_s_discard( s );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mf3_eval_s_run_mul( const bmath_mf3_eval_s* o, tp_t fp_type, fp_t fp, bmath_mf3_eval_result_s* res )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );

    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( bmath_mf3_s, m0 );
    BCORE_LIFE_CREATE( bmath_mf3_s, m1 );
    BCORE_LIFE_CREATE( bmath_mf3_s, m2 );
    BCORE_LIFE_CREATE( bmath_mf3_s, m3 );
    BCORE_LIFE_CREATE( bmath_mf3_eval_result_s, r );
    bmath_mf3_eval_result_s_set_defaults_from_eval( r, o );
    r->fp_type = fp_type;

    uz_t m = o->rows;
    uz_t n = o->cols;

    bmath_mf3_s_set_size( m0, m, n );
    bmath_mf3_s_set_size( m1, n, m );
    bmath_mf3_s_set_size( m2, m, m );
    bmath_mf3_s_set_size( m3, m, m );

    u2_t rval = o->seed;
    bmath_mf3_s_set_random( m0, false, false, 0, o->density, -1.0, 1.0, &rval );
    bmath_mf3_s_set_random( m1, false, false, 0, o->density, -1.0, 1.0, &rval );


    if( fp_type == TYPEOF_bmath_fp_mf3_s_mul )
    {
        if( o->test1 )
        {
            CPU_TIME_OF( ( ( bmath_fp_mf3_s_mul )fp )( m0, m1, m2 ), r->time1 );
            bmath_mf3_s_mul_esp( m0, m1, m3 );
            if( o->log_a ) bmath_mf3_s_to_string( m2, &r->log_a );
            if( o->log_a ) bmath_mf3_s_to_string( m3, &r->log_a );
            r->assert_m = r->assert_m && bmath_mf3_s_is_near_equ( m2, m3, o->near_limit );
            r->fdev_m   = bmath_mf3_s_fdev_equ( m2, m3 );
        }
    }
    else
    {
        ERR_fa( "Invalid fp_type `#<sc_t>`\n", ifnameof( fp_type ) );
    }


    if( res ) bmath_mf3_eval_result_s_copy( res, r );

    BCORE_LIFE_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mf3_eval_s_run_mul_htp( const bmath_mf3_eval_s* o, tp_t fp_type, fp_t fp, bmath_mf3_eval_result_s* res )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );

    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( bmath_mf3_s, m0 );
    BCORE_LIFE_CREATE( bmath_mf3_s, m1 );
    BCORE_LIFE_CREATE( bmath_mf3_s, m2 );
    BCORE_LIFE_CREATE( bmath_mf3_s, m3 );
    BCORE_LIFE_CREATE( bmath_mf3_eval_result_s, r );
    bmath_mf3_eval_result_s_set_defaults_from_eval( r, o );
    r->fp_type = fp_type;

    uz_t m = o->rows;
    uz_t n = o->cols;

    bmath_mf3_s_set_size( m0, m, n );
    bmath_mf3_s_set_size( m1, m, n );
    bmath_mf3_s_set_size( m2, m, m );
    bmath_mf3_s_set_size( m3, m, m );

    u2_t rval = o->seed;
    bmath_mf3_s_set_random( m0, false, false, 0, o->density, -1.0, 1.0, &rval );
    bmath_mf3_s_set_random( m1, false, false, 0, o->density, -1.0, 1.0, &rval );


    if( fp_type == TYPEOF_bmath_fp_mf3_s_mul_htp )
    {
        if( o->test0 )
        {
            CPU_TIME_OF( ( ( bmath_fp_mf3_s_mul_htp )fp )( m0, m0, m2 ), r->time0 );
            bmath_mf3_s_mul_htp_esp( m0, m0, m3 );
            if( o->log_a ) bmath_mf3_s_to_string( m2, &r->log_a );
            if( o->log_a ) bmath_mf3_s_to_string( m3, &r->log_a );
            r->assert_m = bmath_mf3_s_is_near_equ( m2, m3, o->near_limit );
            r->fdev_m   = bmath_mf3_s_fdev_equ( m2, m3 );
        }

        if( o->test1 )
        {
            CPU_TIME_OF( ( ( bmath_fp_mf3_s_mul_htp )fp )( m0, m1, m2 ), r->time1 );
            bmath_mf3_s_mul_htp_esp( m0, m1, m3 );
            if( o->log_a ) bmath_mf3_s_to_string( m2, &r->log_a );
            if( o->log_a ) bmath_mf3_s_to_string( m3, &r->log_a );
            r->assert_m = r->assert_m && bmath_mf3_s_is_near_equ( m2, m3, o->near_limit );
            r->fdev_m   = bmath_mf3_s_fdev_equ( m2, m3 );
        }
    }
    else
    {
        ERR_fa( "Invalid fp_type `#<sc_t>`\n", ifnameof( fp_type ) );
    }


    if( res ) bmath_mf3_eval_result_s_copy( res, r );

    BCORE_LIFE_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mf3_eval_s_run_uav( const bmath_mf3_eval_s* o, tp_t fp_type, fp_t fp, bmath_mf3_eval_result_s* res )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );

    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( bmath_mf3_s, m0 );
    BCORE_LIFE_CREATE( bmath_mf3_s, u );
    BCORE_LIFE_CREATE( bmath_mf3_s, a );
    BCORE_LIFE_CREATE( bmath_mf3_s, v );
    BCORE_LIFE_CREATE( bmath_mf3_s, m2 );
    BCORE_LIFE_CREATE( bmath_mf3_eval_result_s, r );
    bmath_mf3_eval_result_s_set_defaults_from_eval( r, o );
    r->fp_type = fp_type;

    uz_t m = o->rows;
    uz_t n = o->cols;

    bmath_mf3_s_set_size( m0, m, n );
    bmath_mf3_s_set_size( a,  m, n );
    u2_t rval = o->seed;
    bmath_mf3_s_set_random( m0, false, false, 0, o->density, -1.0, 1.0, &rval );
    bmath_mf3_s_set_size( u, m, o->full ? m : uz_min( m, n ) );
    bmath_mf3_s_set_size( v, n, o->full ? n : uz_min( m, n ) );

    bmath_mf3_s_zro( a );
    bmath_mf3_s_zro( u );
    bmath_mf3_s_zro( v );

    if( fp_type == TYPEOF_bmath_fp_mf3_s_svd )
    {
        if( o->test0 )
        {
            bmath_mf3_s_cpy( m0, a );
            CPU_TIME_OF( r->success1 = ( ( bmath_fp_mf3_s_svd )fp )( NULL, a, NULL ), r->time0 );
            r->assert_a = bmath_mf3_s_is_dag( a );
        }

        if( o->test1 )
        {
            bmath_mf3_s_cpy( m0, a );
            CPU_TIME_OF( r->success1 = ( ( bmath_fp_mf3_s_svd )fp )( u, a, v ), r->time1 );
            r->assert_a = r->assert_a && bmath_mf3_s_is_dag( a );
        }
    }
    else if( fp_type == TYPEOF_bmath_fp_mf3_s_ubd )
    {
        if( o->test0 )
        {
            bmath_mf3_s_cpy( m0, a );
            CPU_TIME_OF( ( ( bmath_fp_mf3_s_ubd )fp )( NULL, a, NULL ), r->time0 );
            r->assert_a = bmath_mf3_s_is_ubd( a );
        }

        if( o->test1 )
        {
            bmath_mf3_s_cpy( m0, a );
            CPU_TIME_OF( ( ( bmath_fp_mf3_s_ubd )fp )( u, a, v ), r->time1 );
            r->assert_a = r->assert_a && bmath_mf3_s_is_ubd( a );
        }
    }
    else if( fp_type == TYPEOF_bmath_fp_mf3_s_lbd )
    {
        if( o->test0 )
        {
            bmath_mf3_s_cpy( m0, a );
            CPU_TIME_OF( ( ( bmath_fp_mf3_s_lbd )fp )( NULL, a, NULL ), r->time0 );
            r->assert_a = bmath_mf3_s_is_lbd( a );
        }

        if( o->test1 )
        {
            bmath_mf3_s_cpy( m0, a );
            CPU_TIME_OF( ( ( bmath_fp_mf3_s_lbd )fp )( u, a, v ), r->time1 );
            r->assert_a = r->assert_a && bmath_mf3_s_is_lbd( a );
        }
    }
    else
    {
        ERR_fa( "Invalid fp_type `#<sc_t>`\n", ifnameof( fp_type ) );
    }

    if( o->log_a ) bmath_mf3_s_to_string( a, &r->log_a );

    if( o->test1 )
    {
        r->fdev_u = bmath_mf3_s_fdev_otn( u );
        r->fdev_v = bmath_mf3_s_fdev_otn( v );
        if( o->log_u ) bmath_mf3_s_to_string( u, &r->log_u );
        if( o->log_v ) bmath_mf3_s_to_string( v, &r->log_v );
        r->assert_u = bmath_mf3_s_is_near_otn( u, o->near_limit );
        r->assert_v = bmath_mf3_s_is_near_otn( v, o->near_limit );

        bmath_mf3_s_set_size( m2, u->rows, v->rows );
        bmath_mf3_s_mul_utv_htp_esp( u, a, v, m2 );

        r->assert_m = bmath_mf3_s_is_near_equ( m0, m2, o->near_limit );
        r->fdev_m   = bmath_mf3_s_fdev_equ( m0, m2 );
    }

    if( res ) bmath_mf3_eval_result_s_copy( res, r );

    BCORE_LIFE_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mf3_eval_s_run_ua( const bmath_mf3_eval_s* o, tp_t fp_type, fp_t fp, bmath_mf3_eval_result_s* res )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );

    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( bmath_mf3_s, m0 );
    BCORE_LIFE_CREATE( bmath_mf3_s, u );
    BCORE_LIFE_CREATE( bmath_mf3_s, a );
    BCORE_LIFE_CREATE( bmath_mf3_s, m2 );
    BCORE_LIFE_CREATE( bmath_mf3_eval_result_s, r );
    bmath_mf3_eval_result_s_set_defaults_from_eval( r, o );
    r->fp_type = fp_type;

    uz_t m = o->rows;
    uz_t n = o->cols;

    bmath_mf3_s_set_size( m0, m, n );
    bmath_mf3_s_set_size( a,  m, n );
    u2_t rval = o->seed;
    bmath_mf3_s_set_random( m0, false, false, 0, o->density, -1.0, 1.0, &rval );

    bmath_mf3_s_set_size( u, m, o->full ? m : uz_min( m, n ) );
    bmath_mf3_s_zro( a );
    bmath_mf3_s_zro( u );

    if( fp_type == TYPEOF_bmath_fp_mf3_s_qrd )
    {
        if( o->test0 )
        {
            bmath_mf3_s_cpy( m0, a );
            CPU_TIME_OF( ( ( bmath_fp_mf3_s_qrd )fp )( NULL, a ), r->time0 );
            r->assert_a = bmath_mf3_s_is_utr( a );
        }

        if( o->test1 )
        {
            bmath_mf3_s_cpy( m0, a );
            CPU_TIME_OF( ( ( bmath_fp_mf3_s_qrd )fp )( u, a ), r->time1 );
            r->assert_a = r->assert_a && bmath_mf3_s_is_utr( a );
        }

        if( o->log_a ) bmath_mf3_s_to_string( a, &r->log_a );
    }
    else if( fp_type == TYPEOF_bmath_fp_mf3_s_qrd_pmt )
    {
        BCORE_LIFE_CREATE( bmath_pmt_s, p );
        bmath_pmt_s_set_size( p, a->cols );
        bmath_pmt_s_one( p );

        if( o->test0 )
        {
            bmath_mf3_s_cpy( m0, a );
            CPU_TIME_OF( ( ( bmath_fp_mf3_s_qrd_pmt )fp )( NULL, a, NULL ), r->time0 );
            r->assert_a = bmath_mf3_s_is_utr( a );
        }

        if( o->test1 )
        {

            bmath_mf3_s_cpy( m0, a );
            CPU_TIME_OF( ( ( bmath_fp_mf3_s_qrd_pmt )fp )( u, a, p ), r->time1 );
            r->assert_a = r->assert_a && bmath_mf3_s_is_utr( a );
        }

        // assert monotonic descending non-negative diagonal elements
        uz_t n = uz_min( a->rows, a->cols );
        for( uz_t i = 1; i < n; i++ )
        {
            if( a->data[ i * ( a->stride + 1 ) ] > a->data[ ( i - 1 ) * ( a->stride + 1 ) ] ) r->assert_a = false;
            if( a->data[ i * ( a->stride + 1 ) ] < 0 ) r->assert_a = false;
        }

        if( o->log_a )
        {
            bmath_mf3_s_to_string( a, &r->log_a );
            bmath_pmt_s_to_string( p, &r->log_a );
        }
        bmath_mf3_s_mul_pmt_htp( a, p, a );
    }
    else
    {
        ERR_fa( "Invalid fp_type `#<sc_t>`\n", ifnameof( fp_type ) );
    }

    if( o->test1 )
    {
        r->fdev_u = bmath_mf3_s_fdev_otn( u );
        if( o->log_u ) bmath_mf3_s_to_string( u, &r->log_u );
        r->assert_u = bmath_mf3_s_is_near_otn( u, o->near_limit );

        bmath_mf3_s_set_size( m2, u->rows, a->cols );
        bmath_mf3_s_mul_esp( u, a, m2 );

        r->assert_m = bmath_mf3_s_is_near_equ( m0, m2, o->near_limit );
        r->fdev_m   = bmath_mf3_s_fdev_equ( m0, m2 );
    }

    if( res ) bmath_mf3_eval_result_s_copy( res, r );

    BCORE_LIFE_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mf3_eval_s_run_av( const bmath_mf3_eval_s* o, tp_t fp_type, fp_t fp, bmath_mf3_eval_result_s* res )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );

    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( bmath_mf3_s, m0 );
    BCORE_LIFE_CREATE( bmath_mf3_s, a );
    BCORE_LIFE_CREATE( bmath_mf3_s, v );
    BCORE_LIFE_CREATE( bmath_mf3_s, m2 );
    BCORE_LIFE_CREATE( bmath_mf3_eval_result_s, r );
    bmath_mf3_eval_result_s_set_defaults_from_eval( r, o );
    r->fp_type = fp_type;

    uz_t m = o->rows;
    uz_t n = o->cols;

    bmath_mf3_s_set_size( m0, m, n );
    bmath_mf3_s_set_size( a,  m, n );
    u2_t rval = o->seed;
    bmath_mf3_s_set_random( m0, false, false, 0, o->density, -1.0, 1.0, &rval );

    bmath_mf3_s_set_size( v, n, o->full ? n : uz_min( m, n ) );

    bmath_mf3_s_zro( a );
    bmath_mf3_s_zro( v );

    if( fp_type == TYPEOF_bmath_fp_mf3_s_lqd )
    {
        if( o->test0 )
        {
            bmath_mf3_s_cpy( m0, a );
            CPU_TIME_OF( ( ( bmath_fp_mf3_s_lqd )fp )( a, NULL ), r->time0 );
            r->assert_a = bmath_mf3_s_is_ltr( a );
        }

        if( o->test1 )
        {
            bmath_mf3_s_cpy( m0, a );
            CPU_TIME_OF( ( ( bmath_fp_mf3_s_lqd )fp )( a, v ), r->time1 );
            r->assert_a = r->assert_a && bmath_mf3_s_is_ltr( a );
        }

        if( o->log_a ) bmath_mf3_s_to_string( a, &r->log_a );
    }
    else if( fp_type == TYPEOF_bmath_fp_mf3_s_pmt_lqd )
    {
        BCORE_LIFE_CREATE( bmath_pmt_s, p );
        bmath_pmt_s_set_size( p, a->rows );
        bmath_pmt_s_one( p );

        if( o->test0 )
        {
            bmath_mf3_s_cpy( m0, a );
            CPU_TIME_OF( ( ( bmath_fp_mf3_s_pmt_lqd )fp )( NULL, a, NULL ), r->time0 );
            r->assert_a = bmath_mf3_s_is_ltr( a );
        }

        if( o->test1 )
        {
            bmath_mf3_s_cpy( m0, a );
            CPU_TIME_OF( ( ( bmath_fp_mf3_s_pmt_lqd )fp )( p, a, v ), r->time1 );
            r->assert_a = r->assert_a && bmath_mf3_s_is_ltr( a );
        }

        // assert monotonic descending non-negative diagonal elements
        uz_t n = uz_min( a->rows, a->cols );
        for( uz_t i = 1; i < n; i++ )
        {
            if( a->data[ i * ( a->stride + 1 ) ] > a->data[ ( i - 1 ) * ( a->stride + 1 ) ] ) r->assert_a = false;
            if( a->data[ i * ( a->stride + 1 ) ] < 0 ) r->assert_a = false;
        }

        if( o->log_a )
        {
            bmath_pmt_s_to_string( p, &r->log_a );
            bmath_mf3_s_to_string( a, &r->log_a );
        }
        bmath_mf3_s_pmt_mul( a, p, a );
    }
    else
    {
        ERR_fa( "Invalid fp_type `#<sc_t>`\n", ifnameof( fp_type ) );
    }

    if( o->test1 )
    {
        r->fdev_v = bmath_mf3_s_fdev_otn( v );
        if( o->log_v ) bmath_mf3_s_to_string( v, &r->log_v );
        r->assert_v = bmath_mf3_s_is_near_otn( v, o->near_limit );

        bmath_mf3_s_set_size( m2, a->rows, v->rows );
        bmath_mf3_s_mul_htp_esp( a, v, m2 );

        r->assert_m = bmath_mf3_s_is_near_equ( m0, m2, o->near_limit );
        r->fdev_m   = bmath_mf3_s_fdev_equ( m0, m2 );
    }
    if( res ) bmath_mf3_eval_result_s_copy( res, r );

    BCORE_LIFE_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mf3_eval_s_run_sym_uau_htp( const bmath_mf3_eval_s* o, tp_t fp_type, fp_t fp, bmath_mf3_eval_result_s* res )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );
    ASSERT( o->rows == o->cols );

    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( bmath_mf3_s, m0 );
    BCORE_LIFE_CREATE( bmath_mf3_s, a );
    BCORE_LIFE_CREATE( bmath_mf3_s, v );
    BCORE_LIFE_CREATE( bmath_mf3_s, m2 );
    BCORE_LIFE_CREATE( bmath_mf3_eval_result_s, r );
    bmath_mf3_eval_result_s_set_defaults_from_eval( r, o );
    r->fp_type = fp_type;

    uz_t n = o->rows;

    bmath_mf3_s_set_size( m0, n, n );
    bmath_mf3_s_set_size( a,  n, n );
    u2_t rval = o->seed;
    bmath_mf3_s_set_random( m0, true, false, 0, o->density, -1.0, 1.0, &rval );

    bmath_mf3_s_set_size( v, n, n );
    bmath_mf3_s_zro( a );
    bmath_mf3_s_zro( v );

    if( fp_type == TYPEOF_bmath_fp_mf3_s_trd_htp )
    {
        if( o->test0 )
        {
            bmath_mf3_s_cpy( m0, a );
            CPU_TIME_OF( ( ( bmath_fp_mf3_s_trd_htp )fp )( a, NULL ), r->time0 );
            r->assert_a = bmath_mf3_s_is_trd( a );
        }

        if( o->test1 )
        {
            bmath_mf3_s_cpy( m0, a );
            CPU_TIME_OF( ( ( bmath_fp_mf3_s_trd_htp )fp )( a, v ), r->time1 );
            r->assert_a = r->assert_a && bmath_mf3_s_is_trd( a );
            bmath_mf3_s_htp( v, v );
        }
    }
    else if( fp_type == TYPEOF_bmath_fp_mf3_s_trd )
    {
        if( o->test0 )
        {
            bmath_mf3_s_cpy( m0, a );
            CPU_TIME_OF( ( ( bmath_fp_mf3_s_trd )fp )( a, NULL ), r->time0 );
            r->assert_a = bmath_mf3_s_is_trd( a );
        }

        if( o->test1 )
        {
            bmath_mf3_s_cpy( m0, a );
            CPU_TIME_OF( ( ( bmath_fp_mf3_s_trd )fp )( a, v ), r->time1 );
            r->assert_a = r->assert_a && bmath_mf3_s_is_trd( a );
        }
    }
    else if( fp_type == TYPEOF_bmath_fp_mf3_s_evd_htp )
    {
        if( o->test0 )
        {
            bmath_mf3_s_cpy( m0, a );
            CPU_TIME_OF( ( ( bmath_fp_mf3_s_evd_htp )fp )( a, NULL ), r->time0 );
            r->assert_a = bmath_mf3_s_is_dag( a );
        }

        if( o->test1 )
        {
            bmath_mf3_s_cpy( m0, a );
            CPU_TIME_OF( ( ( bmath_fp_mf3_s_evd_htp )fp )( a, v ), r->time1 );
            r->assert_a = r->assert_a && bmath_mf3_s_is_dag( a );
            bmath_mf3_s_htp( v, v );
        }
    }
    else
    {
        ERR_fa( "Invalid fp_type `#<sc_t>`\n", ifnameof( fp_type ) );
    }

    if( o->test1 )
    {
        r->fdev_v = bmath_mf3_s_fdev_otn( v );
        if( o->log_a ) bmath_mf3_s_to_string( a, &r->log_a );
        if( o->log_v ) bmath_mf3_s_to_string( v, &r->log_v );
        r->assert_v = bmath_mf3_s_is_near_otn( v, o->near_limit );

        bmath_mf3_s_set_size( m2, n, n );

        bmath_mf3_s_mul_utv_htp_esp( v, a, v, m2 );

        r->assert_m = bmath_mf3_s_is_near_equ( m0, m2, o->near_limit );
        r->fdev_m   = bmath_mf3_s_fdev_equ( m0, m2 );
    }
    if( res ) bmath_mf3_eval_result_s_copy( res, r );

    BCORE_LIFE_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

// evaluates cholesky decomposition
void bmath_mf3_eval_s_run_cld( const bmath_mf3_eval_s* o, tp_t fp_type, fp_t fp, bmath_mf3_eval_result_s* res )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );
    ASSERT( o->rows == o->cols );

    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( bmath_mf3_s, m0 );
    BCORE_LIFE_CREATE( bmath_mf3_s, a );
    BCORE_LIFE_CREATE( bmath_mf3_s, m2 );
    BCORE_LIFE_CREATE( bmath_mf3_eval_result_s, r );
    bmath_mf3_eval_result_s_set_defaults_from_eval( r, o );
    r->fp_type = fp_type;

    uz_t n = o->rows;

    bmath_mf3_s_set_size( m0, n, n );
    bmath_mf3_s_set_size( a,  n, n );
    u2_t rval = o->seed;
    bmath_mf3_s_set_random( m0, true, true, 0, o->density, -1.0, 1.0, &rval );

    bmath_mf3_s_zro( a );

    if( fp_type == TYPEOF_bmath_fp_mf3_s_cld )
    {
        CPU_TIME_OF( r->success1 = ( ( bmath_fp_mf3_s_cld )fp )( m0, a ), r->time1 );
        r->assert_a = bmath_mf3_s_is_ltr( a );
    }
    else
    {
        ERR_fa( "Invalid fp_type `#<sc_t>`\n", ifnameof( fp_type ) );
    }

    if( o->log_a ) bmath_mf3_s_to_string( a, &r->log_a );

    bmath_mf3_s_set_size( m2, n, n );
    bmath_mf3_s_mul_htp_esp( a, a, m2 );

    r->assert_m = bmath_mf3_s_is_near_equ( m0, m2, o->near_limit );
    r->fdev_m   = bmath_mf3_s_fdev_equ( m0, m2 );
    if( res ) bmath_mf3_eval_result_s_copy( res, r );

    BCORE_LIFE_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

// evaluates LU decomposition
void bmath_mf3_eval_s_run_lud( const bmath_mf3_eval_s* o, tp_t fp_type, fp_t fp, bmath_mf3_eval_result_s* res )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );
    ASSERT( o->rows == o->cols );

    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( bmath_mf3_s, m0 );
    BCORE_LIFE_CREATE( bmath_mf3_s, a );
    BCORE_LIFE_CREATE( bmath_mf3_s, m1 );
    BCORE_LIFE_CREATE( bmath_mf3_s, m2 );
    BCORE_LIFE_CREATE( bmath_mf3_s, m3 );
    BCORE_LIFE_CREATE( bmath_mf3_eval_result_s, r );
    bmath_mf3_eval_result_s_set_defaults_from_eval( r, o );
    r->fp_type = fp_type;

    uz_t n = o->rows;

    bmath_mf3_s_set_size( m0, n, n );
    bmath_mf3_s_set_size( a,  n, n );
    u2_t rval = o->seed;
    bmath_mf3_s_set_random( m0, false, false, 0, o->density, -1.0, 1.0, &rval );

    bmath_mf3_s_zro( a );

    if( fp_type == TYPEOF_bmath_fp_mf3_s_lud )
    {
        CPU_TIME_OF( r->success1 = ( ( bmath_fp_mf3_s_lud )fp )( m0, a ), r->time1 );
    }
    else
    {
        ERR_fa( "Invalid fp_type `#<sc_t>`\n", ifnameof( fp_type ) );
    }

    if( o->log_a ) bmath_mf3_s_to_string( a, &r->log_a );

    bmath_mf3_s_set_size( m1, n, n );
    bmath_mf3_s_set_random( m1, false, false, 0, o->density, -1.0, 1.0, &rval );

    bmath_mf3_s_set_size( m2, n, n );
    bmath_mf3_s_set_size( m3, n, n );

    bmath_mf3_s_mul_htp_esp(        m0, m1, m2 );
    bmath_mf3_s_luc_mul_htp_htp( a, m1, m3 );
    bmath_mf3_s_htp( m3, m3 );

    r->assert_m = bmath_mf3_s_is_near_equ( m2, m3, o->near_limit );
    r->fdev_m   = bmath_mf3_s_fdev_equ( m2, m3 );
    if( res ) bmath_mf3_eval_result_s_copy( res, r );

    BCORE_LIFE_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

// evaluates inversion
void bmath_mf3_eval_s_run_inv( const bmath_mf3_eval_s* o, tp_t fp_type, fp_t fp, bmath_mf3_eval_result_s* res )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );
    ASSERT( o->rows == o->cols );

    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( bmath_mf3_s, m0 );
    BCORE_LIFE_CREATE( bmath_mf3_s, a );
    BCORE_LIFE_CREATE( bmath_mf3_s, m2 );
    BCORE_LIFE_CREATE( bmath_mf3_eval_result_s, r );
    bmath_mf3_eval_result_s_set_defaults_from_eval( r, o );
    r->fp_type = fp_type;

    uz_t n = o->rows;

    bmath_mf3_s_set_size( m0, n, n );
    bmath_mf3_s_set_size( a,  n, n );
    u2_t rval = o->seed;
    bmath_mf3_s_set_random( m0, false, false, 0, o->density, -1.0, 1.0, &rval );

    bmath_mf3_s_zro( a );

    if( fp_type == TYPEOF_bmath_fp_mf3_s_inv )
    {
        CPU_TIME_OF( r->success1 = ( ( bmath_fp_mf3_s_inv )fp )( m0, a ), r->time1 );
    }
    else
    {
        ERR_fa( "Invalid fp_type `#<sc_t>`\n", ifnameof( fp_type ) );
    }

    if( o->log_a ) bmath_mf3_s_to_string( a, &r->log_a );

    bmath_mf3_s_set_size( m2, n, n );
    bmath_mf3_s_mul_esp( m0, a, m2 );

    r->assert_m = bmath_mf3_s_is_near_one( m2, o->near_limit );
    r->fdev_m   = bmath_mf3_s_fdev_one( m2 );
    if( res ) bmath_mf3_eval_result_s_copy( res, r );

    BCORE_LIFE_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

// evaluates positive definite inversion
void bmath_mf3_eval_s_run_pdf_inv( const bmath_mf3_eval_s* o, tp_t fp_type, fp_t fp, bmath_mf3_eval_result_s* res )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );
    ASSERT( o->rows == o->cols );

    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( bmath_mf3_s, m0 );
    BCORE_LIFE_CREATE( bmath_mf3_s, a );
    BCORE_LIFE_CREATE( bmath_mf3_s, m2 );
    BCORE_LIFE_CREATE( bmath_mf3_eval_result_s, r );
    bmath_mf3_eval_result_s_set_defaults_from_eval( r, o );
    r->fp_type = fp_type;

    uz_t n = o->rows;

    bmath_mf3_s_set_size( m0, n, n );
    bmath_mf3_s_set_size( a,  n, n );
    u2_t rval = o->seed;
    bmath_mf3_s_set_random( m0, true, true, 0, o->density, -1.0, 1.0, &rval );

    bmath_mf3_s_zro( a );

    if( fp_type == TYPEOF_bmath_fp_mf3_s_pdf_inv )
    {
        CPU_TIME_OF( r->success1 = ( ( bmath_fp_mf3_s_pdf_inv )fp )( m0, a ), r->time1 );
    }
    else
    {
        ERR_fa( "Invalid fp_type `#<sc_t>`\n", ifnameof( fp_type ) );
    }

    if( o->log_a ) bmath_mf3_s_to_string( a, &r->log_a );

    bmath_mf3_s_set_size( m2, n, n );
    bmath_mf3_s_mul_esp( m0, a, m2 );

    r->assert_m = bmath_mf3_s_is_near_one( m2, o->near_limit );
    r->fdev_m   = bmath_mf3_s_fdev_one( m2 );
    if( res ) bmath_mf3_eval_result_s_copy( res, r );

    BCORE_LIFE_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mf3_eval_s_run_piv( const bmath_mf3_eval_s* o, tp_t fp_type, fp_t fp, bmath_mf3_eval_result_s* res )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );

    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( bmath_mf3_s, m0 );
    BCORE_LIFE_CREATE( bmath_mf3_s, a );
    BCORE_LIFE_CREATE( bmath_mf3_s, m1 );
    BCORE_LIFE_CREATE( bmath_mf3_s, m2 );
    BCORE_LIFE_CREATE( bmath_mf3_eval_result_s, r );
    bmath_mf3_eval_result_s_set_defaults_from_eval( r, o );
    r->fp_type = fp_type;

    uz_t m = o->rows;
    uz_t n = o->cols;

    bmath_mf3_s_set_size( m0, m, n );
    u2_t rval = o->seed;

    // full rank test
    {
        bmath_mf3_s_set_size( a,  n, m );
        bmath_mf3_s_set_random( m0, false, false, 0, o->density, -1.0, 1.0, &rval );

        bmath_mf3_s_zro( a );

        if( fp_type == TYPEOF_bmath_fp_mf3_s_piv )
        {
            CPU_TIME_OF( r->success1 = ( ( bmath_fp_mf3_s_piv )fp )( m0, o->eps, a ), r->time1 );
        }
        else
        {
            ERR_fa( "Invalid fp_type `#<sc_t>`\n", ifnameof( fp_type ) );
        }

        if( o->log_a ) bmath_mf3_s_to_string( a, &r->log_a );

        bmath_mf3_s_set_size( m2, uz_min( m, n ), uz_min( m, n ) );

        if( n >= m )
        {
            bmath_mf3_s_mul_esp( m0, a, m2 );
        }
        else
        {
            bmath_mf3_s_mul_esp( a, m0, m2 );
        }

        r->assert_m = bmath_mf3_s_is_near_one( m2, o->near_limit );
        r->fdev_m   = bmath_mf3_s_fdev_one( m2 );
    }

    // rank deficit --> verify Moore-Penrose-Conditions
    {
        bmath_mf3_s_set_size( a, n, m );
        uz_t rd = uz_min( o->rows, o->cols ) >> 1;
        bmath_mf3_s_set_random( m0, false, false, rd, o->density, -1.0, 1.0, &rval );

        bmath_mf3_s_zro( a );

        if( fp_type == TYPEOF_bmath_fp_mf3_s_piv )
        {
            CPU_TIME_OF( r->success1 = ( ( bmath_fp_mf3_s_piv )fp )( m0, o->eps, a ), r->time1 );
        }
        else
        {
            ERR_fa( "Invalid fp_type `#<sc_t>`\n", ifnameof( fp_type ) );
        }

        if( o->log_a ) bmath_mf3_s_to_string( a, &r->log_a );

        bmath_mf3_s_set_size( m1, m, m );
        bmath_mf3_s_set_size( m2, m, n );
        bmath_mf3_s_mul_esp( m0,  a, m1 );
        r->assert_u = bmath_mf3_s_is_near_hsm( m1, o->near_limit );  // assert A * A_piv hermitesh

        bmath_mf3_s_mul_esp( m1, m0, m2 );
        r->assert_u = r->assert_u && bmath_mf3_s_is_near_equ( m0, m2, o->near_limit ); // assert A * A_piv * A = A
        r->fdev_u   = bmath_mf3_s_fdev_equ( m0, m2 );

        bmath_mf3_s_set_size( m1, n, n );
        bmath_mf3_s_set_size( m2, n, m );
        bmath_mf3_s_mul_esp( a, m0, m1 );
        r->assert_v = bmath_mf3_s_is_near_hsm( m1, o->near_limit );  // assert A_piv * A hermitesh

        bmath_mf3_s_mul_esp( m1, a, m2 );
        r->assert_v = r->assert_v && bmath_mf3_s_is_near_equ( a, m2, o->near_limit ); // assert  A_piv * A * A_piv = A_piv
        r->fdev_v   = bmath_mf3_s_fdev_equ( a, m2 );
    }


    if( res ) bmath_mf3_eval_result_s_copy( res, r );



    BCORE_LIFE_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

// evaluates symmetric pseudo inversion
void bmath_mf3_eval_s_run_hsm_piv( const bmath_mf3_eval_s* o, tp_t fp_type, fp_t fp, bmath_mf3_eval_result_s* res )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );
    ASSERT( o->rows == o->cols );

    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( bmath_mf3_s, m0 );
    BCORE_LIFE_CREATE( bmath_mf3_s, a );
    BCORE_LIFE_CREATE( bmath_mf3_s, m2 );
    BCORE_LIFE_CREATE( bmath_mf3_eval_result_s, r );
    bmath_mf3_eval_result_s_set_defaults_from_eval( r, o );
    r->fp_type = fp_type;

    uz_t n = o->rows;

    bmath_mf3_s_set_size( m0, n, n );
    bmath_mf3_s_set_size( a,  n, n );
    u2_t rval = o->seed;
    bmath_mf3_s_set_random( m0, true, false, 0, o->density, -1.0, 1.0, &rval );

    bmath_mf3_s_zro( a );

    if( fp_type == TYPEOF_bmath_fp_mf3_s_hsm_piv )
    {
        CPU_TIME_OF( r->success1 = ( ( bmath_fp_mf3_s_hsm_piv )fp )( m0, o->eps, a ), r->time1 );
    }
    else
    {
        ERR_fa( "Invalid fp_type `#<sc_t>`\n", ifnameof( fp_type ) );
    }

    if( o->log_a ) bmath_mf3_s_to_string( a, &r->log_a );

    bmath_mf3_s_set_size( m2, n, n );
    bmath_mf3_s_mul_esp( m0, a, m2 );

    r->assert_m = bmath_mf3_s_is_near_one( m2, o->near_limit );
    r->fdev_m   = bmath_mf3_s_fdev_one( m2 );
    if( res ) bmath_mf3_eval_result_s_copy( res, r );

    BCORE_LIFE_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mf3_eval_s_run( const bmath_mf3_eval_s* o, tp_t fp_type, fp_t fp, st_s* log )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );

    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( bmath_mf3_eval_result_s, r );

    if(      fp_type == TYPEOF_bmath_fp_mf3_s_mul     ) bmath_mf3_eval_s_run_mul(         o, fp_type, fp, r );
    else if( fp_type == TYPEOF_bmath_fp_mf3_s_mul_htp ) bmath_mf3_eval_s_run_mul_htp(     o, fp_type, fp, r );
    else if( fp_type == TYPEOF_bmath_fp_mf3_s_svd     ) bmath_mf3_eval_s_run_uav(         o, fp_type, fp, r );
    else if( fp_type == TYPEOF_bmath_fp_mf3_s_ubd     ) bmath_mf3_eval_s_run_uav(         o, fp_type, fp, r );
    else if( fp_type == TYPEOF_bmath_fp_mf3_s_lbd     ) bmath_mf3_eval_s_run_uav(         o, fp_type, fp, r );
    else if( fp_type == TYPEOF_bmath_fp_mf3_s_qrd     ) bmath_mf3_eval_s_run_ua(          o, fp_type, fp, r );
    else if( fp_type == TYPEOF_bmath_fp_mf3_s_qrd_pmt ) bmath_mf3_eval_s_run_ua(          o, fp_type, fp, r );
    else if( fp_type == TYPEOF_bmath_fp_mf3_s_lqd     ) bmath_mf3_eval_s_run_av(          o, fp_type, fp, r );
    else if( fp_type == TYPEOF_bmath_fp_mf3_s_pmt_lqd ) bmath_mf3_eval_s_run_av(          o, fp_type, fp, r );
    else if( fp_type == TYPEOF_bmath_fp_mf3_s_trd_htp ) bmath_mf3_eval_s_run_sym_uau_htp( o, fp_type, fp, r );
    else if( fp_type == TYPEOF_bmath_fp_mf3_s_trd     ) bmath_mf3_eval_s_run_sym_uau_htp( o, fp_type, fp, r );
    else if( fp_type == TYPEOF_bmath_fp_mf3_s_evd_htp ) bmath_mf3_eval_s_run_sym_uau_htp( o, fp_type, fp, r );
    else if( fp_type == TYPEOF_bmath_fp_mf3_s_cld     ) bmath_mf3_eval_s_run_cld(         o, fp_type, fp, r );
    else if( fp_type == TYPEOF_bmath_fp_mf3_s_lud     ) bmath_mf3_eval_s_run_lud(         o, fp_type, fp, r );
    else if( fp_type == TYPEOF_bmath_fp_mf3_s_inv     ) bmath_mf3_eval_s_run_inv(         o, fp_type, fp, r );
    else if( fp_type == TYPEOF_bmath_fp_mf3_s_pdf_inv ) bmath_mf3_eval_s_run_pdf_inv(     o, fp_type, fp, r );
    else if( fp_type == TYPEOF_bmath_fp_mf3_s_hsm_piv ) bmath_mf3_eval_s_run_hsm_piv(     o, fp_type, fp, r );
    else if( fp_type == TYPEOF_bmath_fp_mf3_s_piv     ) bmath_mf3_eval_s_run_piv(         o, fp_type, fp, r );
    else ERR_fa( "Invalid fp_type `#<sc_t>`\n", ifnameof( fp_type ) );

    if( o->assert_all && ( !r->success0 || !r->success1 || !r->assert_a || !r->assert_u || !r->assert_v || !r->assert_m ) )
    {
        st_s* s = st_s_create();
        bmath_mf3_eval_result_s_to_string( r, s );
        ERR_fa( "\n#<sc_t>\nAssertion failed!\n", s->sc );
        st_s_discard( s );
    }

    if( log ) bmath_mf3_eval_result_s_to_string( r, log );

    BCORE_LIFE_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mf3_eval_s_run_to_stdout( const bmath_mf3_eval_s* o, tp_t fp_type, fp_t fp )
{
    st_s* s = st_s_create();
    bmath_mf3_eval_s_run( o, fp_type, fp, s );
    bcore_msg_fa( "#<sc_t>\n", s->sc );
    st_s_discard( s );
}

//----------------------------------------------------------------------------------------------------------------------

static void bmath_mf3_eval_s_selftest( void )
{
    BCORE_LIFE_INIT();

    BCORE_LIFE_CREATE( bmath_mf3_eval_s, eval );
    eval->rows = 10;
    eval->cols = 10;
    bmath_mf3_eval_s_run( eval, typeof( "bmath_fp_mf3_s_svd" ), ( fp_t )bmath_mf3_s_svd, NULL );

    BCORE_LIFE_DOWN();
}

/**********************************************************************************************************************/

BCORE_DEFINE_OBJECT_INST( bcore_inst, bmath_arr_mf3_eval_s ) "{ aware_t _; bmath_mf3_eval_s [] arr; }";

//----------------------------------------------------------------------------------------------------------------------

void bmath_arr_mf3_eval_s_run( const bmath_arr_mf3_eval_s* o, tp_t fp_type, fp_t fp, st_s* log )
{
    for( uz_t i = 0; i < o->size; i++ ) bmath_mf3_eval_s_run( &o->data[ i ], fp_type, fp, log );
}

void bmath_arr_mf3_eval_s_run_to_stdout( const bmath_arr_mf3_eval_s* o, tp_t fp_type, fp_t fp )
{
    for( uz_t i = 0; i < o->size; i++ ) bmath_mf3_eval_s_run_to_stdout( &o->data[ i ], fp_type, fp );
}

/**********************************************************************************************************************/

vd_t bmath_matrix_eval_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_matrix_eval" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_OBJECT( bmath_mf3_eval_s );
            BCORE_REGISTER_OBJECT( bmath_mf3_eval_result_s );
            BCORE_REGISTER_OBJECT( bmath_arr_mf3_eval_s );
        }
        break;

        case TYPEOF_selftest:
        {
            bmath_mf3_eval_s_selftest();
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

