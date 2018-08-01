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
    "sz_t rows       = 1000;"
    "sz_t cols       = 1000;"

    "u2_t seed       = 1234567;"
    "f3_t density    = 1.0;"
    "bl_t full       = false;"
    "f3_t near_limit = 1E-8;"

    "tp_t fp_type;"
    "private fp_t fp;"
"}";

//---------------------------------------------------------------------------------------------------------------------

void bmath_matrix_eval_s_run_uav_fp( const bmath_matrix_eval_s* o, tp_t fp_type, fp_t fp, st_s* log )
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

    sz_t m = o->rows;
    sz_t n = o->cols;

    bmath_mf3_s_set_size( m0, m, n );
    bmath_mf3_s_set_size( a,  m, n );
    u2_t rval = o->seed;
    bmath_mf3_s_fill_random_sparse( m0, -1, 1, o->density, &rval );

    bmath_mf3_s_set_size( u, m, o->full ? m : sz_min( m, n ) );
    bmath_mf3_s_set_size( v, n, o->full ? n : sz_min( m, n ) );

    bmath_mf3_s_zro( u );
    bmath_mf3_s_zro( v );

    bmath_mf3_s_cpy( m0, a );

    if( fp_type == typeof( "bmath_fp_svd" ) )
    {
        ABS_TIME_TO_STRING( ASSERT( ( ( bmath_fp_svd )fp )( u, a, v ) ), log );
        ASSERT( bmath_mf3_s_is_dag( a ) );
    }
    else if( fp_type == typeof( "bmath_fp_ubd" ) )
    {
        ABS_TIME_TO_STRING( ( ( bmath_fp_ubd )fp )( u, a, v ), log );
        ASSERT( bmath_mf3_s_is_ubd( a ) );
    }
    else if( fp_type == typeof( "bmath_fp_lbd" ) )
    {
        ABS_TIME_TO_STRING( ( ( bmath_fp_lbd )fp )( u, a, v ), log );
        ASSERT( bmath_mf3_s_is_lbd( a ) );
    }
    else
    {
        ERR_fa( "Invalid fp_type\n" );
    }

    if( log ) st_s_push_fa( log, "fdev_otn( u ) = #<f3_t>\n", f3_srt( bmath_mf3_s_fdev_otn( u ) ) );
    if( log ) st_s_push_fa( log, "fdev_otn( v ) = #<f3_t>\n", f3_srt( bmath_mf3_s_fdev_otn( v ) ) );
    ASSERT( bmath_mf3_s_is_near_otn( u, o->near_limit ) );
    ASSERT( bmath_mf3_s_is_near_otn( v, o->near_limit ) );

    bmath_mf3_s_set_size( m1, a->rows, v->rows );
    bmath_mf3_s_mul_htp( a, v, m1 );

    bmath_mf3_s_set_size( m2, u->rows, m1->cols );
    bmath_mf3_s_mul( u, m1, m2 );
    ASSERT( bmath_mf3_s_is_near_equ( m0, m2, o->near_limit ) );

    if( log ) st_s_push_fa( log, "fdev = #<f3_t>\n", f3_srt( bmath_mf3_s_fdev_equ( m0, m2 ) ) );

    BCORE_LIFE_DOWN();
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_matrix_eval_s_run_ua_fp( const bmath_matrix_eval_s* o, tp_t fp_type, fp_t fp, st_s* log )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );

    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( bmath_mf3_s, m0 );
    BCORE_LIFE_CREATE( bmath_mf3_s, u );
    BCORE_LIFE_CREATE( bmath_mf3_s, a );
    BCORE_LIFE_CREATE( bmath_mf3_s, m2 );

    sz_t m = o->rows;
    sz_t n = o->cols;

    bmath_mf3_s_set_size( m0, m, n );
    bmath_mf3_s_set_size( a,  m, n );
    u2_t rval = o->seed;
    bmath_mf3_s_fill_random_sparse( m0, -1, 1, o->density, &rval );

    bmath_mf3_s_set_size( u, m, o->full ? m : sz_min( m, n ) );
    bmath_mf3_s_zro( u );

    bmath_mf3_s_cpy( m0, a );

    if( fp_type == typeof( "bmath_fp_qrd" ) )
    {
        ABS_TIME_TO_STRING( ( ( bmath_fp_qrd )fp )( u, a ), log );
        ASSERT( bmath_mf3_s_is_utr( a ) );
    }
    else
    {
        ERR_fa( "Invalid fp_type\n" );
    }

    if( log ) st_s_push_fa( log, "fdev_otn( u ) = #<f3_t>\n", f3_srt( bmath_mf3_s_fdev_otn( u ) ) );
    ASSERT( bmath_mf3_s_is_near_otn( u, o->near_limit ) );

    bmath_mf3_s_set_size( m2, u->rows, a->cols );
    bmath_mf3_s_mul( u, a, m2 );
    ASSERT( bmath_mf3_s_is_near_equ( m0, m2, o->near_limit ) );

    if( log ) st_s_push_fa( log, "fdev = #<f3_t>\n", f3_srt( bmath_mf3_s_fdev_equ( m0, m2 ) ) );

    BCORE_LIFE_DOWN();
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_matrix_eval_s_run_av_fp( const bmath_matrix_eval_s* o, tp_t fp_type, fp_t fp, st_s* log )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );

    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( bmath_mf3_s, m0 );
    BCORE_LIFE_CREATE( bmath_mf3_s, a );
    BCORE_LIFE_CREATE( bmath_mf3_s, v );
    BCORE_LIFE_CREATE( bmath_mf3_s, m2 );

    sz_t m = o->rows;
    sz_t n = o->cols;

    bmath_mf3_s_set_size( m0, m, n );
    bmath_mf3_s_set_size( a,  m, n );
    u2_t rval = o->seed;
    bmath_mf3_s_fill_random_sparse( m0, -1, 1, o->density, &rval );

    bmath_mf3_s_set_size( v, n, o->full ? n : sz_min( m, n ) );

    bmath_mf3_s_zro( v );

    bmath_mf3_s_cpy( m0, a );

    if( fp_type == typeof( "bmath_fp_lqd" ) )
    {
        ABS_TIME_TO_STRING( ( ( bmath_fp_lqd )fp )( a, v ), log );
        ASSERT( bmath_mf3_s_is_ltr( a ) );
    }
    else
    {
        ERR_fa( "Invalid fp_type\n" );
    }

    if( log ) st_s_push_fa( log, "fdev_otn( v ) = #<f3_t>\n", f3_srt( bmath_mf3_s_fdev_otn( v ) ) );
    ASSERT( bmath_mf3_s_is_near_otn( v, o->near_limit ) );

    bmath_mf3_s_set_size( m2, a->rows, v->rows );
    bmath_mf3_s_mul_htp( a, v, m2 );

    ASSERT( bmath_mf3_s_is_near_equ( m0, m2, o->near_limit ) );

    if( log ) st_s_push_fa( log, "fdev = #<f3_t>\n", f3_srt( bmath_mf3_s_fdev_equ( m0, m2 ) ) );

    BCORE_LIFE_DOWN();
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_matrix_eval_s_run_fp( const bmath_matrix_eval_s* o, tp_t fp_type, fp_t fp, st_s* log )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );
    if( fp_type == typeof( "bmath_fp_svd" ) )
    {
        bmath_matrix_eval_s_run_uav_fp( o, fp_type, fp, log );
    }
    else if( fp_type == typeof( "bmath_fp_ubd" ) )
    {
        bmath_matrix_eval_s_run_uav_fp( o, fp_type, fp, log );
    }
    else if( fp_type == typeof( "bmath_fp_lbd" ) )
    {
        bmath_matrix_eval_s_run_uav_fp( o, fp_type, fp, log );
    }
    else if( fp_type == typeof( "bmath_fp_qrd" ) )
    {
        bmath_matrix_eval_s_run_ua_fp( o, fp_type, fp, log );
    }
    else if( fp_type == typeof( "bmath_fp_lqd" ) )
    {
        bmath_matrix_eval_s_run_av_fp( o, fp_type, fp, log );
    }
    else
    {
        ERR_fa( "Invalid fp_type\n" );
    }
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
    bcore_msg_fa( "#<sc_t>", s->sc );
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
    for( sz_t i = 0; i < o->size; i++ ) bmath_matrix_eval_s_run( &o->data[ i ], log );
}

void bmath_arr_matrix_eval_s_run_fp( const bmath_arr_matrix_eval_s* o, tp_t fp_type, fp_t fp, st_s* log )
{
    for( sz_t i = 0; i < o->size; i++ ) bmath_matrix_eval_s_run_fp( &o->data[ i ], fp_type, fp, log );
}

void bmath_arr_matrix_eval_s_run_to_stdout( const bmath_arr_matrix_eval_s* o )
{
    for( sz_t i = 0; i < o->size; i++ ) bmath_matrix_eval_s_run_to_stdout( &o->data[ i ] );
}

void bmath_arr_matrix_eval_s_run_fp_to_stdout( const bmath_arr_matrix_eval_s* o, tp_t fp_type, fp_t fp )
{
    for( sz_t i = 0; i < o->size; i++ ) bmath_matrix_eval_s_run_fp_to_stdout( &o->data[ i ], fp_type, fp );
}

/**********************************************************************************************************************/

vd_t bmath_matrix_eval_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_matrix_eval" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_OBJECT( bmath_matrix_eval_s );
            BCORE_REGISTER_OBJECT( bmath_arr_matrix_eval_s );
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

