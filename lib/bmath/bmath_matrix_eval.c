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

#include "bmath_matrix_eval.h"

/**********************************************************************************************************************/

BCORE_DEFINE_OBJECT_INST( bcore_inst, bmath_matrix_eval_s )
"{"
    "aware_t _;"
    "uz_t rows       = 1000;"
    "uz_t cols       = 1000;"

    "u2_t seed       = 1234567;"
    "f3_t density    = 1.0;"
    "bl_t full       = false;"
    "f3_t near_limit = 1E-8;"

    "bl_t log_a      = false;" // log matrix a after conversion
    "bl_t log_u      = false;" // log matrix u after conversion
    "bl_t log_v      = false;" // log matrix v after conversion
    "bl_t assert_all = true; " // asserts correct matrix and computation result

    "bl_t test0 = true;"  // runs minimal parameter test
    "bl_t test1 = true;"  // runs default parameter test

    "tp_t fp_type;"
    "private fp_t fp;"
"}";

//---------------------------------------------------------------------------------------------------------------------

BCORE_DEFINE_OBJECT_INST( bcore_inst, bmath_matrix_eval_result_s )
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

//---------------------------------------------------------------------------------------------------------------------

static void bmath_matrix_eval_result_s_set_defaults_from_eval( bmath_matrix_eval_result_s* o, const bmath_matrix_eval_s* v )
{
    o->fp_type = v->fp_type;
    o->rows    = v->rows;
    o->cols    = v->cols;
    o->density = v->density;
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_matrix_eval_result_s_to_string( const bmath_matrix_eval_result_s* o, st_s* string )
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

//---------------------------------------------------------------------------------------------------------------------

void bmath_matrix_eval_result_s_to_stdout( const bmath_matrix_eval_result_s* o )
{
    st_s* s = st_s_create();
    bmath_matrix_eval_result_s_to_string( o, s );
    bcore_msg_fa( "#<sc_t>\n", s->sc );
    st_s_discard( s );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_matrix_eval_s_run_uav_fp( const bmath_matrix_eval_s* o, tp_t fp_type, fp_t fp, bmath_matrix_eval_result_s* res )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );

    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( bmath_mf3_s, m0 );
    BCORE_LIFE_CREATE( bmath_mf3_s, u );
    BCORE_LIFE_CREATE( bmath_mf3_s, a );
    BCORE_LIFE_CREATE( bmath_mf3_s, v );
    BCORE_LIFE_CREATE( bmath_mf3_s, m1 );
    BCORE_LIFE_CREATE( bmath_mf3_s, m2 );
    BCORE_LIFE_CREATE( bmath_matrix_eval_result_s, r );
    bmath_matrix_eval_result_s_set_defaults_from_eval( r, o );
    r->fp_type = fp_type;

    uz_t m = o->rows;
    uz_t n = o->cols;

    bmath_mf3_s_set_size( m0, m, n );
    bmath_mf3_s_set_size( a,  m, n );
    u2_t rval = o->seed;
    bmath_mf3_s_fill_random_sparse( m0, -1, 1, o->density, &rval );

    bmath_mf3_s_set_size( u, m, o->full ? m : uz_min( m, n ) );
    bmath_mf3_s_set_size( v, n, o->full ? n : uz_min( m, n ) );

    bmath_mf3_s_zro( a );
    bmath_mf3_s_zro( u );
    bmath_mf3_s_zro( v );

    if( fp_type == typeof( "bmath_fp_svd" ) )
    {
        if( o->test0 )
        {
            bmath_mf3_s_cpy( m0, a );
            ABS_TIME_OF( r->success1 = ( ( bmath_fp_svd )fp )( NULL, a, NULL ), r->time0 );
            r->assert_a = bmath_mf3_s_is_dag( a );
        }

        if( o->test1 )
        {
            bmath_mf3_s_cpy( m0, a );
            ABS_TIME_OF( r->success1 = ( ( bmath_fp_svd )fp )( u, a, v ), r->time1 );
            r->assert_a = bmath_mf3_s_is_dag( a );
        }
    }
    else if( fp_type == typeof( "bmath_fp_ubd" ) )
    {
        if( o->test0 )
        {
            bmath_mf3_s_cpy( m0, a );
            ABS_TIME_OF( ( ( bmath_fp_ubd )fp )( NULL, a, NULL ), r->time0 );
            r->assert_a = bmath_mf3_s_is_ubd( a );
        }

        if( o->test1 )
        {
            bmath_mf3_s_cpy( m0, a );
            ABS_TIME_OF( ( ( bmath_fp_ubd )fp )( u, a, v ), r->time1 );
            r->assert_a = bmath_mf3_s_is_ubd( a );
        }
    }
    else if( fp_type == typeof( "bmath_fp_lbd" ) )
    {
        if( o->test0 )
        {
            bmath_mf3_s_cpy( m0, a );
            ABS_TIME_OF( ( ( bmath_fp_lbd )fp )( NULL, a, NULL ), r->time0 );
            r->assert_a = bmath_mf3_s_is_lbd( a );
        }

        if( o->test1 )
        {
            bmath_mf3_s_cpy( m0, a );
            ABS_TIME_OF( ( ( bmath_fp_lbd )fp )( u, a, v ), r->time1 );
            r->assert_a = bmath_mf3_s_is_lbd( a );
        }
    }
    else
    {
        ERR_fa( "Invalid fp_type `#<sc_t>`\n", ifnameof( fp_type ) );
    }

    r->fdev_u = bmath_mf3_s_fdev_otn( u );
    r->fdev_v = bmath_mf3_s_fdev_otn( v );
    if( o->log_a ) bmath_mf3_s_to_string( a, &r->log_a );
    if( o->log_u ) bmath_mf3_s_to_string( u, &r->log_u );
    if( o->log_v ) bmath_mf3_s_to_string( v, &r->log_v );
    r->assert_u = bmath_mf3_s_is_near_otn( u, o->near_limit );
    r->assert_v = bmath_mf3_s_is_near_otn( v, o->near_limit );

    bmath_mf3_s_set_size( m1, a->rows, v->rows );
    bmath_mf3_s_mul_htp( a, v, m1 );
    bmath_mf3_s_set_size( m2, u->rows, m1->cols );
    bmath_mf3_s_mul( u, m1, m2 );

    r->assert_m = bmath_mf3_s_is_near_equ( m0, m2, o->near_limit );
    r->fdev_m   = bmath_mf3_s_fdev_equ( m0, m2 );
    if( res ) bmath_matrix_eval_result_s_copy( res, r );

    BCORE_LIFE_DOWN();
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_matrix_eval_s_run_ua_fp( const bmath_matrix_eval_s* o, tp_t fp_type, fp_t fp, bmath_matrix_eval_result_s* res )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );

    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( bmath_mf3_s, m0 );
    BCORE_LIFE_CREATE( bmath_mf3_s, u );
    BCORE_LIFE_CREATE( bmath_mf3_s, a );
    BCORE_LIFE_CREATE( bmath_mf3_s, m2 );
    BCORE_LIFE_CREATE( bmath_matrix_eval_result_s, r );
    bmath_matrix_eval_result_s_set_defaults_from_eval( r, o );
    r->fp_type = fp_type;

    uz_t m = o->rows;
    uz_t n = o->cols;

    bmath_mf3_s_set_size( m0, m, n );
    bmath_mf3_s_set_size( a,  m, n );
    u2_t rval = o->seed;
    bmath_mf3_s_fill_random_sparse( m0, -1, 1, o->density, &rval );

    bmath_mf3_s_set_size( u, m, o->full ? m : uz_min( m, n ) );
    bmath_mf3_s_zro( a );
    bmath_mf3_s_zro( u );

    if( fp_type == typeof( "bmath_fp_qrd" ) )
    {
        if( o->test0 )
        {
            bmath_mf3_s_cpy( m0, a );
            ABS_TIME_OF( ( ( bmath_fp_qrd )fp )( NULL, a ), r->time0 );
            r->assert_a = bmath_mf3_s_is_utr( a );
        }

        if( o->test1 )
        {
            bmath_mf3_s_cpy( m0, a );
            ABS_TIME_OF( ( ( bmath_fp_qrd )fp )( u, a ), r->time1 );
            r->assert_a = bmath_mf3_s_is_utr( a );
        }
    }
    else
    {
        ERR_fa( "Invalid fp_type `#<sc_t>`\n", ifnameof( fp_type ) );
    }

    r->fdev_u = bmath_mf3_s_fdev_otn( u );
    if( o->log_a ) bmath_mf3_s_to_string( a, &r->log_a );
    if( o->log_u ) bmath_mf3_s_to_string( u, &r->log_u );
    r->assert_u = bmath_mf3_s_is_near_otn( u, o->near_limit );

    bmath_mf3_s_set_size( m2, u->rows, a->cols );
    bmath_mf3_s_mul( u, a, m2 );

    r->assert_m = bmath_mf3_s_is_near_equ( m0, m2, o->near_limit );
    r->fdev_m   = bmath_mf3_s_fdev_equ( m0, m2 );
    if( res ) bmath_matrix_eval_result_s_copy( res, r );

    BCORE_LIFE_DOWN();
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_matrix_eval_s_run_av_fp( const bmath_matrix_eval_s* o, tp_t fp_type, fp_t fp, bmath_matrix_eval_result_s* res )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );

    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( bmath_mf3_s, m0 );
    BCORE_LIFE_CREATE( bmath_mf3_s, a );
    BCORE_LIFE_CREATE( bmath_mf3_s, v );
    BCORE_LIFE_CREATE( bmath_mf3_s, m2 );
    BCORE_LIFE_CREATE( bmath_matrix_eval_result_s, r );
    bmath_matrix_eval_result_s_set_defaults_from_eval( r, o );
    r->fp_type = fp_type;

    uz_t m = o->rows;
    uz_t n = o->cols;

    bmath_mf3_s_set_size( m0, m, n );
    bmath_mf3_s_set_size( a,  m, n );
    u2_t rval = o->seed;
    bmath_mf3_s_fill_random_sparse( m0, -1, 1, o->density, &rval );

    bmath_mf3_s_set_size( v, n, o->full ? n : uz_min( m, n ) );

    bmath_mf3_s_zro( a );
    bmath_mf3_s_zro( v );

    if( fp_type == typeof( "bmath_fp_lqd" ) )
    {
        if( o->test0 )
        {
            bmath_mf3_s_cpy( m0, a );
            ABS_TIME_OF( ( ( bmath_fp_lqd )fp )( a, NULL ), r->time0 );
            r->assert_a = bmath_mf3_s_is_ltr( a );
        }

        if( o->test1 )
        {
            bmath_mf3_s_cpy( m0, a );
            ABS_TIME_OF( ( ( bmath_fp_lqd )fp )( a, v ), r->time1 );
            r->assert_a = bmath_mf3_s_is_ltr( a );
        }
    }
    else
    {
        ERR_fa( "Invalid fp_type `#<sc_t>`\n", ifnameof( fp_type ) );
    }

    r->fdev_v = bmath_mf3_s_fdev_otn( v );
    if( o->log_a ) bmath_mf3_s_to_string( a, &r->log_a );
    if( o->log_v ) bmath_mf3_s_to_string( v, &r->log_v );
    r->assert_v = bmath_mf3_s_is_near_otn( v, o->near_limit );

    bmath_mf3_s_set_size( m2, a->rows, v->rows );
    bmath_mf3_s_mul_htp( a, v, m2 );

    r->assert_m = bmath_mf3_s_is_near_equ( m0, m2, o->near_limit );
    r->fdev_m   = bmath_mf3_s_fdev_equ( m0, m2 );
    if( res ) bmath_matrix_eval_result_s_copy( res, r );

    BCORE_LIFE_DOWN();
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_matrix_eval_s_run_sym_vav_htp_fp( const bmath_matrix_eval_s* o, tp_t fp_type, fp_t fp, bmath_matrix_eval_result_s* res )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );
    ASSERT( o->rows == o->cols );

    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( bmath_mf3_s, m0 );
    BCORE_LIFE_CREATE( bmath_mf3_s, a );
    BCORE_LIFE_CREATE( bmath_mf3_s, v );
    BCORE_LIFE_CREATE( bmath_mf3_s, m1 );
    BCORE_LIFE_CREATE( bmath_mf3_s, m2 );
    BCORE_LIFE_CREATE( bmath_matrix_eval_result_s, r );
    bmath_matrix_eval_result_s_set_defaults_from_eval( r, o );
    r->fp_type = fp_type;

    uz_t n = o->rows;

    bmath_mf3_s_set_size( m0, n, n );
    bmath_mf3_s_set_size( a,  n, n );
    u2_t rval = o->seed;
//    bmath_mf3_s_fill_random_sparse( m0, -1, 1, o->density, &rval );
    bmath_mf3_s_fill_random_sparse_hsm( m0, -1, 1, o->density, &rval );
//    bmath_mf3_s_mul_htp( m0, m0, m0 ); // makes m0 positive definite

    bmath_mf3_s_set_size( v, n, n );
    bmath_mf3_s_zro( a );
    bmath_mf3_s_zro( v );

    if( fp_type == typeof( "bmath_fp_trd_htp" ) )
    {
        if( o->test0 )
        {
            bmath_mf3_s_cpy( m0, a );
            ABS_TIME_OF( ( ( bmath_fp_trd_htp )fp )( a, NULL ), r->time0 );
            r->assert_a = bmath_mf3_s_is_trd( a );
        }

        if( o->test1 )
        {
            bmath_mf3_s_cpy( m0, a );
            ABS_TIME_OF( ( ( bmath_fp_trd_htp )fp )( a, v ), r->time1 );
            r->assert_a = bmath_mf3_s_is_trd( a );
            bmath_mf3_s_htp( v, v );
        }
    }
    else if( fp_type == typeof( "bmath_fp_trd" ) )
    {
        if( o->test0 )
        {
            bmath_mf3_s_cpy( m0, a );
            ABS_TIME_OF( ( ( bmath_fp_trd )fp )( a, NULL ), r->time0 );
            r->assert_a = bmath_mf3_s_is_trd( a );
        }

        if( o->test1 )
        {
            bmath_mf3_s_cpy( m0, a );
            ABS_TIME_OF( ( ( bmath_fp_trd )fp )( a, v ), r->time1 );
            r->assert_a = bmath_mf3_s_is_trd( a );
        }
    }
    else if( fp_type == typeof( "bmath_fp_evd_htp" ) )
    {
        if( o->test0 )
        {
            bmath_mf3_s_cpy( m0, a );
            ABS_TIME_OF( ( ( bmath_fp_evd_htp )fp )( a, NULL ), r->time0 );
            r->assert_a = bmath_mf3_s_is_dag( a );
        }

        if( o->test1 )
        {
            bmath_mf3_s_cpy( m0, a );
            ABS_TIME_OF( ( ( bmath_fp_evd_htp )fp )( a, v ), r->time1 );
            r->assert_a = bmath_mf3_s_is_dag( a );
            bmath_mf3_s_htp( v, v );
        }
    }
    else
    {
        ERR_fa( "Invalid fp_type `#<sc_t>`\n", ifnameof( fp_type ) );
    }

    r->fdev_v = bmath_mf3_s_fdev_otn( v );
    if( o->log_a ) bmath_mf3_s_to_string( a, &r->log_a );
    if( o->log_v ) bmath_mf3_s_to_string( v, &r->log_v );
    r->assert_v = bmath_mf3_s_is_near_otn( v, o->near_limit );

    bmath_mf3_s_set_size( m1, n, n );
    bmath_mf3_s_set_size( m2, n, n );
    bmath_mf3_s_mul_htp( a, v, m1 );
    bmath_mf3_s_mul( v, m1, m2 );

    r->assert_m = bmath_mf3_s_is_near_equ( m0, m2, o->near_limit );
    r->fdev_m   = bmath_mf3_s_fdev_equ( m0, m2 );
    if( res ) bmath_matrix_eval_result_s_copy( res, r );

    BCORE_LIFE_DOWN();
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_matrix_eval_s_run_fp( const bmath_matrix_eval_s* o, tp_t fp_type, fp_t fp, st_s* log )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );

    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( bmath_matrix_eval_result_s, r );

    if(      fp_type == typeof( "bmath_fp_svd"     ) ) bmath_matrix_eval_s_run_uav_fp( o, fp_type, fp, r );
    else if( fp_type == typeof( "bmath_fp_ubd"     ) ) bmath_matrix_eval_s_run_uav_fp( o, fp_type, fp, r );
    else if( fp_type == typeof( "bmath_fp_lbd"     ) ) bmath_matrix_eval_s_run_uav_fp( o, fp_type, fp, r );
    else if( fp_type == typeof( "bmath_fp_qrd"     ) ) bmath_matrix_eval_s_run_ua_fp(  o, fp_type, fp, r );
    else if( fp_type == typeof( "bmath_fp_lqd"     ) ) bmath_matrix_eval_s_run_av_fp(  o, fp_type, fp, r );
    else if( fp_type == typeof( "bmath_fp_trd_htp" ) ) bmath_matrix_eval_s_run_sym_vav_htp_fp( o, fp_type, fp, r );
    else if( fp_type == typeof( "bmath_fp_trd"     ) ) bmath_matrix_eval_s_run_sym_vav_htp_fp( o, fp_type, fp, r );
    else if( fp_type == typeof( "bmath_fp_evd_htp" ) ) bmath_matrix_eval_s_run_sym_vav_htp_fp( o, fp_type, fp, r );
    else ERR_fa( "Invalid fp_type `#<sc_t>`\n", ifnameof( fp_type ) );

    if( o->assert_all && ( !r->success0 || !r->success1 || !r->assert_a || !r->assert_u || !r->assert_v || !r->assert_m ) )
    {
        st_s* s = st_s_create();
        bmath_matrix_eval_result_s_to_string( r, s );
        ERR_fa( "\n#<sc_t>\nAssertion failed!\n", s->sc );
        st_s_discard( s );
    }

    if( log ) bmath_matrix_eval_result_s_to_string( r, log );

    BCORE_LIFE_DOWN();
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_matrix_eval_s_run( const bmath_matrix_eval_s* o, st_s* log )
{
    bmath_matrix_eval_s_run_fp( o, o->fp_type, o->fp, log );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_matrix_eval_s_run_fp_to_stdout( const bmath_matrix_eval_s* o, tp_t fp_type, fp_t fp )
{
    st_s* s = st_s_create();
    bmath_matrix_eval_s_run_fp( o, fp_type, fp, s );
    bcore_msg_fa( "#<sc_t>\n", s->sc );
    st_s_discard( s );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_matrix_eval_s_run_to_stdout( const bmath_matrix_eval_s* o )
{
    bmath_matrix_eval_s_run_fp_to_stdout( o, o->fp_type, o->fp );
}

//---------------------------------------------------------------------------------------------------------------------

static void bmath_matrix_eval_s_selftest( void )
{
    BCORE_LIFE_INIT();

    BCORE_LIFE_CREATE( bmath_matrix_eval_s, eval );
    eval->rows = 10;
    eval->cols = 10;
    eval->fp_type = typeof( "bmath_fp_svd" );
    eval->fp      = ( fp_t )bmath_mf3_s_svd;
    bmath_matrix_eval_s_run( eval, NULL );

    BCORE_LIFE_DOWN();
}

/**********************************************************************************************************************/

BCORE_DEFINE_OBJECT_INST( bcore_inst, bmath_arr_matrix_eval_s ) "{ aware_t _; bmath_matrix_eval_s [] arr; }";

//---------------------------------------------------------------------------------------------------------------------

void bmath_arr_matrix_eval_s_run( const bmath_arr_matrix_eval_s* o, st_s* log )
{
    for( uz_t i = 0; i < o->size; i++ ) bmath_matrix_eval_s_run( &o->data[ i ], log );
}

void bmath_arr_matrix_eval_s_run_fp( const bmath_arr_matrix_eval_s* o, tp_t fp_type, fp_t fp, st_s* log )
{
    for( uz_t i = 0; i < o->size; i++ ) bmath_matrix_eval_s_run_fp( &o->data[ i ], fp_type, fp, log );
}

void bmath_arr_matrix_eval_s_run_to_stdout( const bmath_arr_matrix_eval_s* o )
{
    for( uz_t i = 0; i < o->size; i++ ) bmath_matrix_eval_s_run_to_stdout( &o->data[ i ] );
}

void bmath_arr_matrix_eval_s_run_fp_to_stdout( const bmath_arr_matrix_eval_s* o, tp_t fp_type, fp_t fp )
{
    for( uz_t i = 0; i < o->size; i++ ) bmath_matrix_eval_s_run_fp_to_stdout( &o->data[ i ], fp_type, fp );
}

/**********************************************************************************************************************/

vd_t bmath_matrix_eval_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_matrix_eval" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_OBJECT( bmath_matrix_eval_s );
            BCORE_REGISTER_OBJECT( bmath_matrix_eval_result_s );
            BCORE_REGISTER_OBJECT( bmath_arr_matrix_eval_s );


            // features
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_trd_htp );
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_trd );
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_evd_htp );
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_svd );
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_ubd );
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_lbd );
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_qrd );
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_lqd );

        }
        break;

        case TYPEOF_selftest:
        {
            bmath_matrix_eval_s_selftest();
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

