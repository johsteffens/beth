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

#include "bmath_mfx_eval.h"

/**********************************************************************************************************************/

BCORE_DEFINE_OBJECT_INST( bcore_inst, bmath_mfx_eval_s )
"{"
    "aware_t _;"
    "sz_t rows       = 1000;"
    "sz_t cols       = 1000;"
    "sz_t dim3       =   -1;"   // third dimension (e.g. in multiplication tests); -1: use default

    "u2_t seed       = 1234567;"
    "f3_t density    = 1.0;"
    "bl_t full       = false;"
    "f3_t near_limit_f2 = 1E-3;" // limit for near-assertions (f2_t)
    "f3_t near_limit_f3 = 1E-6;" // limit for near-assertions (f3_t)
    "f3_t eps        = 1E-8;"  // for function requiring an epsilon

    "bl_t create_a_log = false;" // log matrix a after conversion
    "bl_t create_u_log = false;" // log matrix u after conversion
    "bl_t create_v_log = false;" // log matrix v after conversion

    "st_s a_img_file;" // create image file of matrix a after conversion
    "st_s u_img_file;" // create image file of matrix u after conversion
    "st_s v_img_file;" // create image file of matrix v after conversion

    "bl_t assert_all   = true; " // asserts correct matrix and computation result

    "bl_t test0 = true;"  // runs minimal parameter test
    "bl_t test1 = true;"  // runs default parameter test
"}";

//----------------------------------------------------------------------------------------------------------------------

BCORE_DEFINE_OBJECT_INST( bcore_inst, bmath_mfx_eval_result_s )
"{"
    "aware_t _;"
    "st_s label;"    // test label
    "tp_t fp_type;"
    "sz_t rows;"
    "sz_t cols;"
    "sz_t dim3;"

    "f3_t density;"

    "st_s a_log;"           // log of matrix a
    "st_s u_log;"           // log of matrix u
    "st_s v_log;"           // log of matrix v

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

static void bmath_mfx_eval_result_s_set_defaults_from_eval( bmath_mfx_eval_result_s* o, const bmath_mfx_eval_s* v )
{
    o->rows    = v->rows;
    o->cols    = v->cols;
    o->dim3    = v->dim3;
    o->density = v->density;
}

//----------------------------------------------------------------------------------------------------------------------

static void bmath_mfx_eval_s_create_image_file( const bmath_mfx_eval_s* v, vc_t mat, const st_s* file )
{
    if( file->size > 0 )
    {
        bcore_msg_fa( "creating '#<sc_t>'\n", file->sc );
        BLM_INIT();
        bcore_img_u2_s* img = BLM_CREATE( bcore_img_u2_s );
        switch( *(aware_t*)mat )
        {
            case TYPEOF_bmath_mf3_s: bmath_mf3_s_to_image( mat, bmath_u2_argb_from_f3, NULL, img ); break;
            case TYPEOF_bmath_mf2_s: bmath_mf2_s_to_image( mat, bmath_u2_argb_from_f2, NULL, img ); break;
            default: ERR_fa( "Invalid matrix." );
        }
        bcore_img_u2_s_pnm_to_file( img, file->sc );
        BLM_DOWN();
    }
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mfx_eval_result_s_to_string( const bmath_mfx_eval_result_s* o, st_s* string )
{
    if( !string ) return;

    if( o->label.size > 0 )
    {
        st_s_push_fa( string, "#<sc_t>: ", o->label.sc );
    }

    st_s_push_fa( string, "#<sc_t> (#<uz_t> x #<uz_t>) density = #<f3_t>\n", ifnameof( o->fp_type ), o->rows, o->cols, o->density );
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

    if( o->a_log.size > 0 ) st_s_push_fa( string, "Matrix a #<sc_t>", o->a_log.sc );
    if( o->u_log.size > 0 ) st_s_push_fa( string, "Matrix u #<sc_t>", o->u_log.sc );
    if( o->v_log.size > 0 ) st_s_push_fa( string, "Matrix v #<sc_t>", o->v_log.sc );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mfx_eval_result_s_to_stdout( const bmath_mfx_eval_result_s* o )
{
    st_s* s = st_s_create();
    bmath_mfx_eval_result_s_to_string( o, s );
    bcore_msg_fa( "#<sc_t>\n", s->sc );
    st_s_discard( s );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mfx_eval_s_run_mul( const bmath_mfx_eval_s* o, tp_t fp_type, fp_t fp, bmath_mfx_eval_result_s* res )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );

    BLM_INIT();
    bmath_mf3_s* m0 = BLM_CREATE( bmath_mf3_s );
    bmath_mf3_s* m1 = BLM_CREATE( bmath_mf3_s );
    bmath_mf3_s* m2 = BLM_CREATE( bmath_mf3_s );
    bmath_mf3_s* m3 = BLM_CREATE( bmath_mf3_s );
    bmath_mfx_eval_result_s* r = BLM_CREATE( bmath_mfx_eval_result_s );
    bmath_mfx_eval_result_s_set_defaults_from_eval( r, o );
    r->fp_type = fp_type;

    sz_t m = o->rows;
    sz_t n = o->cols;
    sz_t p = o->dim3 >=0 ? o->dim3 : m;

    bmath_mf3_s_set_size( m0, m, n );
    bmath_mf3_s_set_size( m1, n, p );
    bmath_mf3_s_set_size( m2, m, p );
    bmath_mf3_s_set_size( m3, m, p );

    u2_t rval = o->seed;
    bmath_mf3_s_set_random( m0, false, false, 0, o->density, -1.0, 1.0, &rval );
    bmath_mf3_s_set_random( m1, false, false, 0, o->density, -1.0, 1.0, &rval );

    if( fp_type == TYPEOF_bmath_fp_mf2_s_mul )
    {
        bmath_mf2_s* _m0 = BLM_A_PUSH( bmath_mf2_s_clone_a( m0 ) );
        bmath_mf2_s* _m1 = BLM_A_PUSH( bmath_mf2_s_clone_a( m1 ) );
        bmath_mf2_s* _m2 = BLM_A_PUSH( bmath_mf2_s_clone_a( m2 ) );

        if( o->test1 )
        {
            CPU_TIME_OF( ( ( bmath_fp_mf2_s_mul )fp )( _m0, _m1, _m2 ), r->time1 );
            bmath_mf3_s_copy_a( m2, _m2 );

            bmath_mf3_s_mul_esp( m0, m1, m3 );
            if( o->create_a_log ) bmath_mf3_s_to_string( m2, &r->a_log );
            if( o->create_a_log ) bmath_mf3_s_to_string( m3, &r->a_log );
            bmath_mfx_eval_s_create_image_file( o, m3, &o->a_img_file );
            r->assert_m = r->assert_m && bmath_mf3_s_is_near_equ( m2, m3, o->near_limit_f2 );
            r->fdev_m   = bmath_mf3_s_fdev_equ( m2, m3 );
        }
    }
    else if( fp_type == TYPEOF_bmath_fp_mf3_s_mul )
    {
        if( o->test1 )
        {
            CPU_TIME_OF( ( ( bmath_fp_mf3_s_mul )fp )( m0, m1, m2 ), r->time1 );
            bmath_mf3_s_mul_esp( m0, m1, m3 );
            if( o->create_a_log ) bmath_mf3_s_to_string( m2, &r->a_log );
            if( o->create_a_log ) bmath_mf3_s_to_string( m3, &r->a_log );
            bmath_mfx_eval_s_create_image_file( o, m3, &o->a_img_file );
            r->assert_m = r->assert_m && bmath_mf3_s_is_near_equ( m2, m3, o->near_limit_f3 );
            r->fdev_m   = bmath_mf3_s_fdev_equ( m2, m3 );
        }
    }
    else
    {
        ERR_fa( "Invalid fp_type `#<sc_t>`\n", ifnameof( fp_type ) );
    }


    if( res ) bmath_mfx_eval_result_s_copy( res, r );

    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mfx_eval_s_run_mul_htp( const bmath_mfx_eval_s* o, tp_t fp_type, fp_t fp, bmath_mfx_eval_result_s* res )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );

    BLM_INIT();
    bmath_mf3_s* m0 = BLM_CREATE( bmath_mf3_s );
    bmath_mf3_s* m1 = BLM_CREATE( bmath_mf3_s );
    bmath_mf3_s* m2 = BLM_CREATE( bmath_mf3_s );
    bmath_mf3_s* m3 = BLM_CREATE( bmath_mf3_s );
    bmath_mfx_eval_result_s* r = BLM_CREATE( bmath_mfx_eval_result_s );
    bmath_mfx_eval_result_s_set_defaults_from_eval( r, o );
    r->fp_type = fp_type;

    if( fp_type == TYPEOF_bmath_fp_mf2_s_mul_htp )
    {
        bmath_mf2_s* _m0 = BLM_A_PUSH( bmath_mf2_s_create() );
        bmath_mf2_s* _m1 = BLM_A_PUSH( bmath_mf2_s_create() );
        bmath_mf2_s* _m2 = BLM_A_PUSH( bmath_mf2_s_create() );

        if( o->test0 )
        {
            sz_t m = o->rows;
            sz_t n = o->cols;

            bmath_mf3_s_set_size( m0, m, n );
            bmath_mf3_s_set_size( m2, m, m );
            bmath_mf3_s_set_size( m3, m, m );

            u2_t rval = o->seed;
            bmath_mf3_s_set_random( m0, false, false, 0, o->density, -1.0, 1.0, &rval );

            bmath_mf2_s_copy_a( _m0, m0 );
            bmath_mf2_s_copy_a( _m2, m2 );
            CPU_TIME_OF( ( ( bmath_fp_mf2_s_mul_htp )fp )( _m0, _m0, _m2 ), r->time0 );
            bmath_mf3_s_copy_a( m2, _m2 );

            bmath_mf3_s_mul_htp_esp( m0, m0, m3 );
            if( o->create_a_log ) bmath_mf3_s_to_string( m2, &r->a_log );
            if( o->create_a_log ) bmath_mf3_s_to_string( m3, &r->a_log );
            bmath_mfx_eval_s_create_image_file( o, m3, &o->a_img_file );
            r->assert_m = bmath_mf3_s_is_near_equ( m2, m3, o->near_limit_f2 );
            r->fdev_m   = bmath_mf3_s_fdev_equ( m2, m3 );
        }

        if( o->test1 )
        {
            sz_t m = o->rows;
            sz_t n = o->cols;
            sz_t p = o->dim3 >=0 ? o->dim3 : m;

            bmath_mf3_s_set_size( m0, m, n );
            bmath_mf3_s_set_size( m1, p, n );
            bmath_mf3_s_set_size( m2, m, p );
            bmath_mf3_s_set_size( m3, m, p );

            u2_t rval = o->seed;
            bmath_mf3_s_set_random( m0, false, false, 0, o->density, -1.0, 1.0, &rval );
            bmath_mf3_s_set_random( m1, false, false, 0, o->density, -1.0, 1.0, &rval );

            bmath_mf2_s_copy_a( _m0, m0 );
            bmath_mf2_s_copy_a( _m1, m1 );
            bmath_mf2_s_copy_a( _m2, m2 );
            CPU_TIME_OF( ( ( bmath_fp_mf2_s_mul_htp )fp )( _m0, _m1, _m2 ), r->time1 );
            bmath_mf3_s_copy_a( m2, _m2 );

            bmath_mf3_s_mul_htp_esp( m0, m1, m3 );
            if( o->create_a_log ) bmath_mf3_s_to_string( m2, &r->a_log );
            if( o->create_a_log ) bmath_mf3_s_to_string( m3, &r->a_log );
            bmath_mfx_eval_s_create_image_file( o, m3, &o->a_img_file );
            r->assert_m = r->assert_m && bmath_mf3_s_is_near_equ( m2, m3, o->near_limit_f2 );
            r->fdev_m   = bmath_mf3_s_fdev_equ( m2, m3 );
        }
    }
    else if( fp_type == TYPEOF_bmath_fp_mf3_s_mul_htp )
    {
        if( o->test0 )
        {
            sz_t m = o->rows;
            sz_t n = o->cols;

            bmath_mf3_s_set_size( m0, m, n );
            bmath_mf3_s_set_size( m2, m, m );
            bmath_mf3_s_set_size( m3, m, m );

            u2_t rval = o->seed;
            bmath_mf3_s_set_random( m0, false, false, 0, o->density, -1.0, 1.0, &rval );

            CPU_TIME_OF( ( ( bmath_fp_mf3_s_mul_htp )fp )( m0, m0, m2 ), r->time0 );
            bmath_mf3_s_mul_htp_esp( m0, m0, m3 );
            if( o->create_a_log ) bmath_mf3_s_to_string( m2, &r->a_log );
            if( o->create_a_log ) bmath_mf3_s_to_string( m3, &r->a_log );
            bmath_mfx_eval_s_create_image_file( o, m3, &o->a_img_file );
            r->assert_m = bmath_mf3_s_is_near_equ( m2, m3, o->near_limit_f3 );
            r->fdev_m   = bmath_mf3_s_fdev_equ( m2, m3 );
        }

        if( o->test1 )
        {
            sz_t m = o->rows;
            sz_t n = o->cols;
            sz_t p = o->dim3 >=0 ? o->dim3 : m;

            bmath_mf3_s_set_size( m0, m, n );
            bmath_mf3_s_set_size( m1, p, n );
            bmath_mf3_s_set_size( m2, m, p );
            bmath_mf3_s_set_size( m3, m, p );

            u2_t rval = o->seed;
            bmath_mf3_s_set_random( m0, false, false, 0, o->density, -1.0, 1.0, &rval );
            bmath_mf3_s_set_random( m1, false, false, 0, o->density, -1.0, 1.0, &rval );

            CPU_TIME_OF( ( ( bmath_fp_mf3_s_mul_htp )fp )( m0, m1, m2 ), r->time1 );
            bmath_mf3_s_mul_htp_esp( m0, m1, m3 );
            if( o->create_a_log ) bmath_mf3_s_to_string( m2, &r->a_log );
            if( o->create_a_log ) bmath_mf3_s_to_string( m3, &r->a_log );
            bmath_mfx_eval_s_create_image_file( o, m3, &o->a_img_file );
            r->assert_m = r->assert_m && bmath_mf3_s_is_near_equ( m2, m3, o->near_limit_f3 );
            r->fdev_m   = bmath_mf3_s_fdev_equ( m2, m3 );
        }
    }
    else
    {
        ERR_fa( "Invalid fp_type `#<sc_t>`\n", ifnameof( fp_type ) );
    }


    if( res ) bmath_mfx_eval_result_s_copy( res, r );

    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mfx_eval_s_run_htp_mul( const bmath_mfx_eval_s* o, tp_t fp_type, fp_t fp, bmath_mfx_eval_result_s* res )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );

    BLM_INIT();
    bmath_mf3_s* m0  = BLM_CREATE( bmath_mf3_s );
    bmath_mf3_s* m0t = BLM_CREATE( bmath_mf3_s );
    bmath_mf3_s* m1  = BLM_CREATE( bmath_mf3_s );
    bmath_mf3_s* m2  = BLM_CREATE( bmath_mf3_s );
    bmath_mf3_s* m3  = BLM_CREATE( bmath_mf3_s );
    bmath_mfx_eval_result_s* r = BLM_CREATE( bmath_mfx_eval_result_s );
    bmath_mfx_eval_result_s_set_defaults_from_eval( r, o );
    r->fp_type = fp_type;

    if( fp_type == TYPEOF_bmath_fp_mf2_s_htp_mul )
    {
        bmath_mf2_s* _m0 = BLM_A_PUSH( bmath_mf2_s_create() );
        bmath_mf2_s* _m1 = BLM_A_PUSH( bmath_mf2_s_create() );
        bmath_mf2_s* _m2 = BLM_A_PUSH( bmath_mf2_s_create() );

        if( o->test0 )
        {
            sz_t m = o->rows;
            sz_t n = o->cols;

            bmath_mf3_s_set_size( m0,  m, n );
            bmath_mf3_s_set_size( m0t, n, m );
            bmath_mf3_s_set_size( m2, n, n );
            bmath_mf3_s_set_size( m3, n, n );

            u2_t rval = o->seed;
            bmath_mf3_s_set_random( m0, false, false, 0, o->density, -1.0, 1.0, &rval );
            bmath_mf3_s_set_random( m1, false, false, 0, o->density, -1.0, 1.0, &rval );

            bmath_mf2_s_copy_a( _m0, m0 );
            bmath_mf2_s_copy_a( _m2, m2 );
            CPU_TIME_OF( ( ( bmath_fp_mf2_s_htp_mul )fp )( _m0, _m0, _m2 ), r->time0 );
            bmath_mf3_s_copy_a( m2, _m2 );

            // bmath_mf3_s_htp_mul_esp is very slow --> we use explicit transposition instead
            bmath_mf3_s_htp( m0, m0t );
            bmath_mf3_s_mul_esp( m0t, m0, m3 );

            if( o->create_a_log ) bmath_mf3_s_to_string( m2, &r->a_log );
            if( o->create_a_log ) bmath_mf3_s_to_string( m3, &r->a_log );
            bmath_mfx_eval_s_create_image_file( o, m3, &o->a_img_file );
            r->assert_m = bmath_mf3_s_is_near_equ( m2, m3, o->near_limit_f2 );
            r->fdev_m   = bmath_mf3_s_fdev_equ( m2, m3 );
        }

        if( o->test1 )
        {
            sz_t m = o->rows;
            sz_t n = o->cols;
            sz_t p = o->dim3 >=0 ? o->dim3 : n;

            bmath_mf3_s_set_size( m0,  m, n );
            bmath_mf3_s_set_size( m0t, n, m );
            bmath_mf3_s_set_size( m1, m, p );
            bmath_mf3_s_set_size( m2, n, p );
            bmath_mf3_s_set_size( m3, n, p );

            u2_t rval = o->seed;
            bmath_mf3_s_set_random( m0, false, false, 0, o->density, -1.0, 1.0, &rval );
            bmath_mf3_s_set_random( m1, false, false, 0, o->density, -1.0, 1.0, &rval );

            bmath_mf2_s_copy_a( _m0, m0 );
            bmath_mf2_s_copy_a( _m1, m1 );
            bmath_mf2_s_copy_a( _m2, m2 );
            CPU_TIME_OF( ( ( bmath_fp_mf2_s_htp_mul )fp )( _m0, _m1, _m2 ), r->time1 );
            bmath_mf3_s_copy_a( m2, _m2 );

            // bmath_mf3_s_htp_mul_esp is very slow --> we use explicit transposition instead
            bmath_mf3_s_htp( m0, m0t );
            bmath_mf3_s_mul_esp( m0t, m1, m3 );

            if( o->create_a_log ) bmath_mf3_s_to_string( m2, &r->a_log );
            if( o->create_a_log ) bmath_mf3_s_to_string( m3, &r->a_log );
            bmath_mfx_eval_s_create_image_file( o, m3, &o->a_img_file );
            r->assert_m = r->assert_m && bmath_mf3_s_is_near_equ( m2, m3, o->near_limit_f2 );
            r->fdev_m   = bmath_mf3_s_fdev_equ( m2, m3 );
        }
    }
    else if( fp_type == TYPEOF_bmath_fp_mf3_s_htp_mul )
    {
        if( o->test0 )
        {
            sz_t m = o->rows;
            sz_t n = o->cols;

            bmath_mf3_s_set_size( m0,  m, n );
            bmath_mf3_s_set_size( m0t, n, m );
            bmath_mf3_s_set_size( m2, n, n );
            bmath_mf3_s_set_size( m3, n, n );

            u2_t rval = o->seed;
            bmath_mf3_s_set_random( m0, false, false, 0, o->density, -1.0, 1.0, &rval );
            bmath_mf3_s_set_random( m1, false, false, 0, o->density, -1.0, 1.0, &rval );

            CPU_TIME_OF( ( ( bmath_fp_mf3_s_htp_mul )fp )( m0, m0, m2 ), r->time0 );

            // bmath_mf3_s_htp_mul_esp is very slow --> we use explicit transposition instead
            bmath_mf3_s_htp( m0, m0t );
            bmath_mf3_s_mul_esp( m0t, m0, m3 );

            if( o->create_a_log ) bmath_mf3_s_to_string( m2, &r->a_log );
            if( o->create_a_log ) bmath_mf3_s_to_string( m3, &r->a_log );
            bmath_mfx_eval_s_create_image_file( o, m3, &o->a_img_file );
            r->assert_m = bmath_mf3_s_is_near_equ( m2, m3, o->near_limit_f3 );
            r->fdev_m   = bmath_mf3_s_fdev_equ( m2, m3 );
        }

        if( o->test1 )
        {
            sz_t m = o->rows;
            sz_t n = o->cols;
            sz_t p = o->dim3 >=0 ? o->dim3 : n;

            bmath_mf3_s_set_size( m0,  m, n );
            bmath_mf3_s_set_size( m0t, n, m );
            bmath_mf3_s_set_size( m1, m, p );
            bmath_mf3_s_set_size( m2, n, p );
            bmath_mf3_s_set_size( m3, n, p );

            u2_t rval = o->seed;
            bmath_mf3_s_set_random( m0, false, false, 0, o->density, -1.0, 1.0, &rval );
            bmath_mf3_s_set_random( m1, false, false, 0, o->density, -1.0, 1.0, &rval );

            CPU_TIME_OF( ( ( bmath_fp_mf3_s_htp_mul )fp )( m0, m1, m2 ), r->time1 );

            // bmath_mf3_s_htp_mul_esp is very slow --> we use explicit transposition instead
            bmath_mf3_s_htp( m0, m0t );
            bmath_mf3_s_mul_esp( m0t, m1, m3 );

            if( o->create_a_log ) bmath_mf3_s_to_string( m2, &r->a_log );
            if( o->create_a_log ) bmath_mf3_s_to_string( m3, &r->a_log );
            bmath_mfx_eval_s_create_image_file( o, m3, &o->a_img_file );
            r->assert_m = r->assert_m && bmath_mf3_s_is_near_equ( m2, m3, o->near_limit_f3 );
            r->fdev_m   = bmath_mf3_s_fdev_equ( m2, m3 );
        }
    }
    else
    {
        ERR_fa( "Invalid fp_type `#<sc_t>`\n", ifnameof( fp_type ) );
    }


    if( res ) bmath_mfx_eval_result_s_copy( res, r );

    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mfx_eval_s_run_htp_mul_htp( const bmath_mfx_eval_s* o, tp_t fp_type, fp_t fp, bmath_mfx_eval_result_s* res )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );

    BLM_INIT();
    bmath_mf3_s* m0 = BLM_CREATE( bmath_mf3_s );
    bmath_mf3_s* m1 = BLM_CREATE( bmath_mf3_s );
    bmath_mf3_s* m2 = BLM_CREATE( bmath_mf3_s );
    bmath_mf3_s* m3 = BLM_CREATE( bmath_mf3_s );
    bmath_mf3_s* m4 = BLM_CREATE( bmath_mf3_s );
    bmath_mfx_eval_result_s* r = BLM_CREATE( bmath_mfx_eval_result_s );
    bmath_mfx_eval_result_s_set_defaults_from_eval( r, o );
    r->fp_type = fp_type;

    sz_t m = o->rows;
    sz_t n = o->cols;
    sz_t p = o->dim3 >=0 ? o->dim3 : n;

    bmath_mf3_s_set_size( m0, m, n );
    bmath_mf3_s_set_size( m1, p, m );
    bmath_mf3_s_set_size( m2, n, p );
    bmath_mf3_s_set_size( m3, n, p );
    bmath_mf3_s_set_size( m4, p, n );

    u2_t rval = o->seed;
    bmath_mf3_s_set_random( m0, false, false, 0, o->density, -1.0, 1.0, &rval );
    bmath_mf3_s_set_random( m1, false, false, 0, o->density, -1.0, 1.0, &rval );


    if( fp_type == TYPEOF_bmath_fp_mf2_s_htp_mul_htp )
    {
        bmath_mf2_s* _m0 = BLM_A_PUSH( bmath_mf2_s_create() );
        bmath_mf2_s* _m1 = BLM_A_PUSH( bmath_mf2_s_create() );
        bmath_mf2_s* _m2 = BLM_A_PUSH( bmath_mf2_s_create() );
        if( o->test1 )
        {
            /// TOTO: implement bmath_mf3_s_htp_mul_htp_esp
            bmath_mf3_s_mul_esp( m1, m0, m4 );
            bmath_mf3_s_htp( m4, m3 );

            bmath_mf2_s_copy_a( _m0, m0 );
            bmath_mf2_s_copy_a( _m1, m1 );
            bmath_mf2_s_copy_a( _m2, m2 );
            CPU_TIME_OF( ( ( bmath_fp_mf2_s_htp_mul_htp )fp )( _m0, _m1, _m2 ), r->time1 );
            bmath_mf3_s_copy_a( m2, _m2 );

            if( o->create_a_log ) bmath_mf3_s_to_string( m2, &r->a_log );
            if( o->create_a_log ) bmath_mf3_s_to_string( m3, &r->a_log );
            bmath_mfx_eval_s_create_image_file( o, m3, &o->a_img_file );
            r->assert_m = r->assert_m && bmath_mf3_s_is_near_equ( m2, m3, o->near_limit_f2 );
            r->fdev_m   = bmath_mf3_s_fdev_equ( m2, m3 );
        }
    }
    else if( fp_type == TYPEOF_bmath_fp_mf3_s_htp_mul_htp )
    {
        if( o->test1 )
        {
            /// TOTO: implement bmath_mf3_s_htp_mul_htp_esp
            bmath_mf3_s_mul_esp( m1, m0, m4 );
            bmath_mf3_s_htp( m4, m3 );

            CPU_TIME_OF( ( ( bmath_fp_mf3_s_htp_mul_htp )fp )( m0, m1, m2 ), r->time1 );
            if( o->create_a_log ) bmath_mf3_s_to_string( m2, &r->a_log );
            if( o->create_a_log ) bmath_mf3_s_to_string( m3, &r->a_log );
            bmath_mfx_eval_s_create_image_file( o, m3, &o->a_img_file );
            r->assert_m = r->assert_m && bmath_mf3_s_is_near_equ( m2, m3, o->near_limit_f3 );
            r->fdev_m   = bmath_mf3_s_fdev_equ( m2, m3 );
        }
    }
    else
    {
        ERR_fa( "Invalid fp_type `#<sc_t>`\n", ifnameof( fp_type ) );
    }

    if( res ) bmath_mfx_eval_result_s_copy( res, r );

    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mfx_eval_s_run_uav_f2( const bmath_mfx_eval_s* o, tp_t fp_type, fp_t fp, bmath_mfx_eval_result_s* res )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );

    BLM_INIT();
    BCORE_LIFE_CREATE( bmath_mf2_s, m0 );
    BCORE_LIFE_CREATE( bmath_mf2_s, u );
    BCORE_LIFE_CREATE( bmath_mf2_s, a );
    BCORE_LIFE_CREATE( bmath_mf2_s, v );
    BCORE_LIFE_CREATE( bmath_mf2_s, m2 );
    BCORE_LIFE_CREATE( bmath_mfx_eval_result_s, r );
    bmath_mfx_eval_result_s_set_defaults_from_eval( r, o );
    r->fp_type = fp_type;

    sz_t m = o->rows;
    sz_t n = o->cols;

    bmath_mf2_s_set_size( m0, m, n );
    bmath_mf2_s_set_size( a,  m, n );
    u2_t rval = o->seed;
    bmath_mf2_s_set_random( m0, false, false, 0, o->density, -1.0, 1.0, &rval );
    bmath_mf2_s_set_size( u, m, o->full ? m : uz_min( m, n ) );
    bmath_mf2_s_set_size( v, n, o->full ? n : uz_min( m, n ) );

    bmath_mf2_s_zro( a );
    bmath_mf2_s_zro( u );
    bmath_mf2_s_zro( v );

    if( fp_type == TYPEOF_bmath_fp_mf2_s_uav )
    {
        if( o->test0 )
        {
            bmath_mf2_s_cpy( m0, a );
            CPU_TIME_OF( ( ( bmath_fp_mf2_s_uav )fp )( NULL, a, NULL ), r->time0 );
        }

        if( o->test1 )
        {
            bmath_mf2_s_cpy( m0, a );
            CPU_TIME_OF( ( ( bmath_fp_mf2_s_uav )fp )( u, a, v ), r->time1 );
        }
    }
    else if( fp_type == TYPEOF_bmath_fp_mf2_s_svd )
    {
        if( o->test0 )
        {
            bmath_mf2_s_cpy( m0, a );
            CPU_TIME_OF( r->success0 = ( ( bmath_fp_mf2_s_svd )fp )( NULL, a, NULL ), r->time0 );
            r->assert_a = bmath_mf2_s_is_dag( a );
        }

        if( o->test1 )
        {
            bmath_mf2_s_cpy( m0, a );
            CPU_TIME_OF( r->success1 = ( ( bmath_fp_mf2_s_svd )fp )( u, a, v ), r->time1 );
            r->assert_a = r->assert_a && bmath_mf2_s_is_dag( a );
        }
    }
    else if( fp_type == TYPEOF_bmath_fp_mf2_s_ubd )
    {
        if( o->test0 )
        {
            bmath_mf2_s_cpy( m0, a );
            CPU_TIME_OF( ( ( bmath_fp_mf2_s_ubd )fp )( NULL, a, NULL ), r->time0 );
            r->assert_a = bmath_mf2_s_is_ubd( a );
        }

        if( o->test1 )
        {
            bmath_mf2_s_cpy( m0, a );
            CPU_TIME_OF( ( ( bmath_fp_mf2_s_ubd )fp )( u, a, v ), r->time1 );
            r->assert_a = r->assert_a && bmath_mf2_s_is_ubd( a );
        }
    }
    else if( fp_type == TYPEOF_bmath_fp_mf2_s_lbd )
    {
        if( o->test0 )
        {
            bmath_mf2_s_cpy( m0, a );
            CPU_TIME_OF( ( ( bmath_fp_mf2_s_lbd )fp )( NULL, a, NULL ), r->time0 );
            r->assert_a = bmath_mf2_s_is_lbd( a );
        }

        if( o->test1 )
        {
            bmath_mf2_s_cpy( m0, a );
            CPU_TIME_OF( ( ( bmath_fp_mf2_s_lbd )fp )( u, a, v ), r->time1 );
            r->assert_a = r->assert_a && bmath_mf2_s_is_lbd( a );
        }
    }
    else
    {
        ERR_fa( "Invalid fp_type `#<sc_t>`\n", ifnameof( fp_type ) );
    }

    if( o->create_a_log ) bmath_mf2_s_to_string( a, &r->a_log );
    bmath_mfx_eval_s_create_image_file( o, a, &o->a_img_file );

    if( o->test1 )
    {
        r->fdev_u = bmath_mf2_s_fdev_otn( u );
        r->fdev_v = bmath_mf2_s_fdev_otn( v );
        if( o->create_u_log ) bmath_mf2_s_to_string( u, &r->u_log );
        if( o->create_v_log ) bmath_mf2_s_to_string( v, &r->v_log );
        bmath_mfx_eval_s_create_image_file( o, u, &o->u_img_file );
        bmath_mfx_eval_s_create_image_file( o, v, &o->v_img_file );
        r->assert_u = bmath_mf2_s_is_near_otn( u, o->near_limit_f2 );
        r->assert_v = bmath_mf2_s_is_near_otn( v, o->near_limit_f2 );

        bmath_mf2_s_set_size( m2, u->rows, v->rows );
        bmath_mf2_s_mul_utv_htp_esp( u, a, v, m2 );

        r->assert_m = bmath_mf2_s_is_near_equ( m0, m2, o->near_limit_f2 );
        r->fdev_m   = bmath_mf2_s_fdev_equ( m0, m2 );
    }

    if( res ) bmath_mfx_eval_result_s_copy( res, r );

    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mfx_eval_s_run_uav_f3( const bmath_mfx_eval_s* o, tp_t fp_type, fp_t fp, bmath_mfx_eval_result_s* res )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );

    BLM_INIT();
    BCORE_LIFE_CREATE( bmath_mf3_s, m0 );
    BCORE_LIFE_CREATE( bmath_mf3_s, u );
    BCORE_LIFE_CREATE( bmath_mf3_s, a );
    BCORE_LIFE_CREATE( bmath_mf3_s, v );
    BCORE_LIFE_CREATE( bmath_mf3_s, m2 );
    BCORE_LIFE_CREATE( bmath_mfx_eval_result_s, r );
    bmath_mfx_eval_result_s_set_defaults_from_eval( r, o );
    r->fp_type = fp_type;

    sz_t m = o->rows;
    sz_t n = o->cols;

    bmath_mf3_s_set_size( m0, m, n );
    bmath_mf3_s_set_size( a,  m, n );
    u2_t rval = o->seed;
    bmath_mf3_s_set_random( m0, false, false, 0, o->density, -1.0, 1.0, &rval );
    bmath_mf3_s_set_size( u, m, o->full ? m : uz_min( m, n ) );
    bmath_mf3_s_set_size( v, n, o->full ? n : uz_min( m, n ) );

    bmath_mf3_s_zro( a );
    bmath_mf3_s_zro( u );
    bmath_mf3_s_zro( v );

    if( fp_type == TYPEOF_bmath_fp_mf3_s_uav )
    {
        if( o->test0 )
        {
            bmath_mf3_s_cpy( m0, a );
            CPU_TIME_OF( ( ( bmath_fp_mf3_s_uav )fp )( NULL, a, NULL ), r->time0 );
        }

        if( o->test1 )
        {
            bmath_mf3_s_cpy( m0, a );
            CPU_TIME_OF( ( ( bmath_fp_mf3_s_uav )fp )( u, a, v ), r->time1 );
        }
    }
    else if( fp_type == TYPEOF_bmath_fp_mf3_s_svd )
    {
        if( o->test0 )
        {
            bmath_mf3_s_cpy( m0, a );
            CPU_TIME_OF( r->success0 = ( ( bmath_fp_mf3_s_svd )fp )( NULL, a, NULL ), r->time0 );
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

    if( o->create_a_log ) bmath_mf3_s_to_string( a, &r->a_log );
    bmath_mfx_eval_s_create_image_file( o, a, &o->a_img_file );

    if( o->test1 )
    {
        r->fdev_u = bmath_mf3_s_fdev_otn( u );
        r->fdev_v = bmath_mf3_s_fdev_otn( v );
        if( o->create_u_log ) bmath_mf3_s_to_string( u, &r->u_log );
        if( o->create_v_log ) bmath_mf3_s_to_string( v, &r->v_log );
        bmath_mfx_eval_s_create_image_file( o, u, &o->u_img_file );
        bmath_mfx_eval_s_create_image_file( o, v, &o->v_img_file );
        r->assert_u = bmath_mf3_s_is_near_otn( u, o->near_limit_f3 );
        r->assert_v = bmath_mf3_s_is_near_otn( v, o->near_limit_f3 );

        bmath_mf3_s_set_size( m2, u->rows, v->rows );
        bmath_mf3_s_mul_utv_htp_esp( u, a, v, m2 );

        r->assert_m = bmath_mf3_s_is_near_equ( m0, m2, o->near_limit_f3 );
        r->fdev_m   = bmath_mf3_s_fdev_equ( m0, m2 );
    }

    if( res ) bmath_mfx_eval_result_s_copy( res, r );

    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mfx_eval_s_run_ua( const bmath_mfx_eval_s* o, tp_t fp_type, fp_t fp, bmath_mfx_eval_result_s* res )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );

    BLM_INIT();
    BCORE_LIFE_CREATE( bmath_mf3_s, m0 );
    BCORE_LIFE_CREATE( bmath_mf3_s, u );
    BCORE_LIFE_CREATE( bmath_mf3_s, a );
    BCORE_LIFE_CREATE( bmath_mf3_s, m2 );
    BCORE_LIFE_CREATE( bmath_mfx_eval_result_s, r );
    bmath_mfx_eval_result_s_set_defaults_from_eval( r, o );
    r->fp_type = fp_type;

    sz_t m = o->rows;
    sz_t n = o->cols;

    bmath_mf3_s_set_size( m0, m, n );
    bmath_mf3_s_set_size( a,  m, n );
    u2_t rval = o->seed;
    bmath_mf3_s_set_random( m0, false, false, 0, o->density, -1.0, 1.0, &rval );

    bmath_mf3_s_set_size( u, m, o->full ? m : uz_min( m, n ) );
    bmath_mf3_s_zro( a );
    bmath_mf3_s_zro( u );

    if( fp_type == TYPEOF_bmath_fp_mf3_s_ua )
    {
        if( o->test0 )
        {
            bmath_mf3_s_cpy( m0, a );
            CPU_TIME_OF( ( ( bmath_fp_mf3_s_ua )fp )( NULL, a ), r->time0 );
        }

        if( o->test1 )
        {
            bmath_mf3_s_cpy( m0, a );
            CPU_TIME_OF( ( ( bmath_fp_mf3_s_ua )fp )( u, a ), r->time1 );
        }

        if( o->create_a_log ) bmath_mf3_s_to_string( a, &r->a_log );
    }
    else if( fp_type == TYPEOF_bmath_fp_mf3_s_qrd )
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

        if( o->create_a_log ) bmath_mf3_s_to_string( a, &r->a_log );
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
        sz_t n = uz_min( a->rows, a->cols );
        for( uz_t i = 1; i < n; i++ )
        {
            if( a->data[ i * ( a->stride + 1 ) ] > a->data[ ( i - 1 ) * ( a->stride + 1 ) ] ) r->assert_a = false;
            if( a->data[ i * ( a->stride + 1 ) ] < 0 ) r->assert_a = false;
        }

        if( o->create_a_log )
        {
            bmath_mf3_s_to_string( a, &r->a_log );
            bmath_pmt_s_to_string( p, &r->a_log );
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
        if( o->create_u_log ) bmath_mf3_s_to_string( u, &r->u_log );
        bmath_mfx_eval_s_create_image_file( o, a, &o->a_img_file );
        bmath_mfx_eval_s_create_image_file( o, u, &o->u_img_file );
        r->assert_u = bmath_mf3_s_is_near_otn( u, o->near_limit_f3 );

        bmath_mf3_s_set_size( m2, u->rows, a->cols );
        bmath_mf3_s_mul_esp( u, a, m2 );

        r->assert_m = bmath_mf3_s_is_near_equ( m0, m2, o->near_limit_f3 );
        r->fdev_m   = bmath_mf3_s_fdev_equ( m0, m2 );
    }

    if( res ) bmath_mfx_eval_result_s_copy( res, r );

    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mfx_eval_s_run_av( const bmath_mfx_eval_s* o, tp_t fp_type, fp_t fp, bmath_mfx_eval_result_s* res )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );

    BLM_INIT();
    BCORE_LIFE_CREATE( bmath_mf3_s, m0 );
    BCORE_LIFE_CREATE( bmath_mf3_s, a );
    BCORE_LIFE_CREATE( bmath_mf3_s, v );
    BCORE_LIFE_CREATE( bmath_mf3_s, m2 );
    BCORE_LIFE_CREATE( bmath_mfx_eval_result_s, r );
    bmath_mfx_eval_result_s_set_defaults_from_eval( r, o );
    r->fp_type = fp_type;

    sz_t m = o->rows;
    sz_t n = o->cols;

    bmath_mf3_s_set_size( m0, m, n );
    bmath_mf3_s_set_size( a,  m, n );
    u2_t rval = o->seed;
    bmath_mf3_s_set_random( m0, false, false, 0, o->density, -1.0, 1.0, &rval );

    bmath_mf3_s_set_size( v, n, o->full ? n : uz_min( m, n ) );

    bmath_mf3_s_zro( a );
    bmath_mf3_s_zro( v );

    if( fp_type == TYPEOF_bmath_fp_mf3_s_av )
    {
        if( o->test0 )
        {
            bmath_mf3_s_cpy( m0, a );
            CPU_TIME_OF( ( ( bmath_fp_mf3_s_av )fp )( a, NULL ), r->time0 );
        }

        if( o->test1 )
        {
            bmath_mf3_s_cpy( m0, a );
            CPU_TIME_OF( ( ( bmath_fp_mf3_s_av )fp )( a, v ), r->time1 );
        }

        if( o->create_a_log ) bmath_mf3_s_to_string( a, &r->a_log );
    }
    else if( fp_type == TYPEOF_bmath_fp_mf3_s_lqd )
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

        if( o->create_a_log ) bmath_mf3_s_to_string( a, &r->a_log );
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
        sz_t n = uz_min( a->rows, a->cols );
        for( uz_t i = 1; i < n; i++ )
        {
            if( a->data[ i * ( a->stride + 1 ) ] > a->data[ ( i - 1 ) * ( a->stride + 1 ) ] ) r->assert_a = false;
            if( a->data[ i * ( a->stride + 1 ) ] < 0 ) r->assert_a = false;
        }

        if( o->create_a_log )
        {
            bmath_pmt_s_to_string( p, &r->a_log );
            bmath_mf3_s_to_string( a, &r->a_log );
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
        if( o->create_v_log ) bmath_mf3_s_to_string( v, &r->v_log );
        bmath_mfx_eval_s_create_image_file( o, a, &o->a_img_file );
        bmath_mfx_eval_s_create_image_file( o, v, &o->v_img_file );
        r->assert_v = bmath_mf3_s_is_near_otn( v, o->near_limit_f3 );

        bmath_mf3_s_set_size( m2, a->rows, v->rows );
        bmath_mf3_s_mul_htp_esp( a, v, m2 );

        r->assert_m = bmath_mf3_s_is_near_equ( m0, m2, o->near_limit_f3 );
        r->fdev_m   = bmath_mf3_s_fdev_equ( m0, m2 );
    }
    if( res ) bmath_mfx_eval_result_s_copy( res, r );

    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mfx_eval_s_run_sym_uau_htp( const bmath_mfx_eval_s* o, tp_t fp_type, fp_t fp, bmath_mfx_eval_result_s* res )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );
    ASSERT( o->rows == o->cols );

    BLM_INIT();
    BCORE_LIFE_CREATE( bmath_mf3_s, m0 );
    BCORE_LIFE_CREATE( bmath_mf3_s, a );
    BCORE_LIFE_CREATE( bmath_mf3_s, v );
    BCORE_LIFE_CREATE( bmath_mf3_s, m2 );
    BCORE_LIFE_CREATE( bmath_mfx_eval_result_s, r );
    bmath_mfx_eval_result_s_set_defaults_from_eval( r, o );
    r->fp_type = fp_type;

    sz_t n = o->rows;

    bmath_mf3_s_set_size( m0, n, n );
    bmath_mf3_s_set_size( a,  n, n );
    u2_t rval = o->seed;
    bmath_mf3_s_set_random( m0, true, false, 0, o->density, -1.0, 1.0, &rval );

    bmath_mf3_s_set_size( v, n, n );
    bmath_mf3_s_zro( a );
    bmath_mf3_s_zro( v );

    if( fp_type == TYPEOF_bmath_fp_mf3_s_uau )
    {
        if( o->test0 )
        {
            bmath_mf3_s_cpy( m0, a );
            CPU_TIME_OF( ( ( bmath_fp_mf3_s_uau )fp )( NULL, a ), r->time0 );
        }

        if( o->test1 )
        {
            bmath_mf3_s_cpy( m0, a );
            CPU_TIME_OF( ( ( bmath_fp_mf3_s_uau )fp )( v, a ), r->time1 );
            bmath_mf3_s_htp( v, v );
        }
    }
    else if( fp_type == TYPEOF_bmath_fp_mf3_s_vav )
    {
        if( o->test0 )
        {
            bmath_mf3_s_cpy( m0, a );
            CPU_TIME_OF( ( ( bmath_fp_mf3_s_vav )fp )( a, NULL ), r->time0 );
        }

        if( o->test1 )
        {
            bmath_mf3_s_cpy( m0, a );
            CPU_TIME_OF( ( ( bmath_fp_mf3_s_vav )fp )( a, v ), r->time1 );
        }
    }
    else if( fp_type == TYPEOF_bmath_fp_mf3_s_trd_htp )
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
        if( o->create_a_log ) bmath_mf3_s_to_string( a, &r->a_log );
        if( o->create_v_log ) bmath_mf3_s_to_string( v, &r->v_log );
        bmath_mfx_eval_s_create_image_file( o, a, &o->a_img_file );
        bmath_mfx_eval_s_create_image_file( o, v, &o->v_img_file );
        r->assert_v = bmath_mf3_s_is_near_otn( v, o->near_limit_f3 );

        bmath_mf3_s_set_size( m2, n, n );

        bmath_mf3_s_mul_utv_htp_esp( v, a, v, m2 );

        r->assert_m = bmath_mf3_s_is_near_equ( m0, m2, o->near_limit_f3 );
        r->fdev_m   = bmath_mf3_s_fdev_equ( m0, m2 );
    }
    if( res ) bmath_mfx_eval_result_s_copy( res, r );

    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

// evaluates cholesky decomposition
void bmath_mfx_eval_s_run_cld( const bmath_mfx_eval_s* o, tp_t fp_type, fp_t fp, bmath_mfx_eval_result_s* res )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );
    ASSERT( o->rows == o->cols );

    BLM_INIT();
    BCORE_LIFE_CREATE( bmath_mf3_s, m0 );
    BCORE_LIFE_CREATE( bmath_mf3_s, a );
    BCORE_LIFE_CREATE( bmath_mf3_s, m2 );
    BCORE_LIFE_CREATE( bmath_mfx_eval_result_s, r );
    bmath_mfx_eval_result_s_set_defaults_from_eval( r, o );
    r->fp_type = fp_type;

    sz_t n = o->rows;

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

    if( o->create_a_log ) bmath_mf3_s_to_string( a, &r->a_log );
    bmath_mfx_eval_s_create_image_file( o, a, &o->a_img_file );

    bmath_mf3_s_set_size( m2, n, n );
    bmath_mf3_s_mul_htp_esp( a, a, m2 );

    r->assert_m = bmath_mf3_s_is_near_equ( m0, m2, o->near_limit_f3 );
    r->fdev_m   = bmath_mf3_s_fdev_equ( m0, m2 );
    if( res ) bmath_mfx_eval_result_s_copy( res, r );

    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

// evaluates LU decomposition
void bmath_mfx_eval_s_run_lud( const bmath_mfx_eval_s* o, tp_t fp_type, fp_t fp, bmath_mfx_eval_result_s* res )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );
    ASSERT( o->rows == o->cols );

    BLM_INIT();
    BCORE_LIFE_CREATE( bmath_mf3_s, m0 );
    BCORE_LIFE_CREATE( bmath_mf3_s, a );
    BCORE_LIFE_CREATE( bmath_mf3_s, m1 );
    BCORE_LIFE_CREATE( bmath_mf3_s, m2 );
    BCORE_LIFE_CREATE( bmath_mf3_s, m3 );
    BCORE_LIFE_CREATE( bmath_mfx_eval_result_s, r );
    bmath_mfx_eval_result_s_set_defaults_from_eval( r, o );
    r->fp_type = fp_type;

    sz_t n = o->rows;

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

    if( o->create_a_log ) bmath_mf3_s_to_string( a, &r->a_log );
    bmath_mfx_eval_s_create_image_file( o, a, &o->a_img_file );

    bmath_mf3_s_set_size( m1, n, n );
    bmath_mf3_s_set_random( m1, false, false, 0, o->density, -1.0, 1.0, &rval );

    bmath_mf3_s_set_size( m2, n, n );
    bmath_mf3_s_set_size( m3, n, n );

    bmath_mf3_s_mul_htp_esp(        m0, m1, m2 );
    bmath_mf3_s_luc_mul_htp_htp( a, m1, m3 );
    bmath_mf3_s_htp( m3, m3 );

    r->assert_m = bmath_mf3_s_is_near_equ( m2, m3, o->near_limit_f3 );
    r->fdev_m   = bmath_mf3_s_fdev_equ( m2, m3 );
    if( res ) bmath_mfx_eval_result_s_copy( res, r );

    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

// evaluates inversion
void bmath_mfx_eval_s_run_inv( const bmath_mfx_eval_s* o, tp_t fp_type, fp_t fp, bmath_mfx_eval_result_s* res )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );
    ASSERT( o->rows == o->cols );

    BLM_INIT();
    BCORE_LIFE_CREATE( bmath_mf3_s, m0 );
    BCORE_LIFE_CREATE( bmath_mf3_s, a );
    BCORE_LIFE_CREATE( bmath_mf3_s, m2 );
    BCORE_LIFE_CREATE( bmath_mfx_eval_result_s, r );
    bmath_mfx_eval_result_s_set_defaults_from_eval( r, o );
    r->fp_type = fp_type;

    sz_t n = o->rows;

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

    if( o->create_a_log ) bmath_mf3_s_to_string( a, &r->a_log );
    bmath_mfx_eval_s_create_image_file( o, a, &o->a_img_file );

    bmath_mf3_s_set_size( m2, n, n );
    bmath_mf3_s_mul_esp( m0, a, m2 );

    r->assert_m = bmath_mf3_s_is_near_one( m2, o->near_limit_f3 );
    r->fdev_m   = bmath_mf3_s_fdev_one( m2 );
    if( res ) bmath_mfx_eval_result_s_copy( res, r );

    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

// evaluates positive definite inversion
void bmath_mfx_eval_s_run_pdf_inv( const bmath_mfx_eval_s* o, tp_t fp_type, fp_t fp, bmath_mfx_eval_result_s* res )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );
    ASSERT( o->rows == o->cols );

    BLM_INIT();
    BCORE_LIFE_CREATE( bmath_mf3_s, m0 );
    BCORE_LIFE_CREATE( bmath_mf3_s, a );
    BCORE_LIFE_CREATE( bmath_mf3_s, m2 );
    BCORE_LIFE_CREATE( bmath_mfx_eval_result_s, r );
    bmath_mfx_eval_result_s_set_defaults_from_eval( r, o );
    r->fp_type = fp_type;

    sz_t n = o->rows;

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

    if( o->create_a_log ) bmath_mf3_s_to_string( a, &r->a_log );
    bmath_mfx_eval_s_create_image_file( o, a, &o->a_img_file );

    bmath_mf3_s_set_size( m2, n, n );
    bmath_mf3_s_mul_esp( m0, a, m2 );

    r->assert_m = bmath_mf3_s_is_near_one( m2, o->near_limit_f3 );
    r->fdev_m   = bmath_mf3_s_fdev_one( m2 );
    if( res ) bmath_mfx_eval_result_s_copy( res, r );

    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mfx_eval_s_run_piv( const bmath_mfx_eval_s* o, tp_t fp_type, fp_t fp, bmath_mfx_eval_result_s* res )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );

    BLM_INIT();
    BCORE_LIFE_CREATE( bmath_mf3_s, m0 );
    BCORE_LIFE_CREATE( bmath_mf3_s, a );
    BCORE_LIFE_CREATE( bmath_mf3_s, m1 );
    BCORE_LIFE_CREATE( bmath_mf3_s, m2 );
    BCORE_LIFE_CREATE( bmath_mfx_eval_result_s, r );
    bmath_mfx_eval_result_s_set_defaults_from_eval( r, o );
    r->fp_type = fp_type;

    sz_t m = o->rows;
    sz_t n = o->cols;

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

        if( o->create_a_log ) bmath_mf3_s_to_string( a, &r->a_log );
        bmath_mfx_eval_s_create_image_file( o, a, &o->a_img_file );

        bmath_mf3_s_set_size( m2, uz_min( m, n ), uz_min( m, n ) );

        if( n >= m )
        {
            bmath_mf3_s_mul_esp( m0, a, m2 );
        }
        else
        {
            bmath_mf3_s_mul_esp( a, m0, m2 );
        }

        r->assert_m = bmath_mf3_s_is_near_one( m2, o->near_limit_f3 );
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

        if( o->create_a_log ) bmath_mf3_s_to_string( a, &r->a_log );
        bmath_mfx_eval_s_create_image_file( o, a, &o->a_img_file );

        bmath_mf3_s_set_size( m1, m, m );
        bmath_mf3_s_set_size( m2, m, n );
        bmath_mf3_s_mul_esp( m0,  a, m1 );
        r->assert_u = bmath_mf3_s_is_near_hsm( m1, o->near_limit_f3 );  // assert A * A_piv hermitesh

        bmath_mf3_s_mul_esp( m1, m0, m2 );
        r->assert_u = r->assert_u && bmath_mf3_s_is_near_equ( m0, m2, o->near_limit_f3 ); // assert A * A_piv * A = A
        r->fdev_u   = bmath_mf3_s_fdev_equ( m0, m2 );

        bmath_mf3_s_set_size( m1, n, n );
        bmath_mf3_s_set_size( m2, n, m );
        bmath_mf3_s_mul_esp( a, m0, m1 );
        r->assert_v = bmath_mf3_s_is_near_hsm( m1, o->near_limit_f3 );  // assert A_piv * A hermitesh

        bmath_mf3_s_mul_esp( m1, a, m2 );
        r->assert_v = r->assert_v && bmath_mf3_s_is_near_equ( a, m2, o->near_limit_f3 ); // assert  A_piv * A * A_piv = A_piv
        r->fdev_v   = bmath_mf3_s_fdev_equ( a, m2 );
    }


    if( res ) bmath_mfx_eval_result_s_copy( res, r );



    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

// evaluates symmetric pseudo inversion
void bmath_mfx_eval_s_run_hsm_piv( const bmath_mfx_eval_s* o, tp_t fp_type, fp_t fp, bmath_mfx_eval_result_s* res )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );
    ASSERT( o->rows == o->cols );

    BLM_INIT();
    BCORE_LIFE_CREATE( bmath_mf3_s, m0 );
    BCORE_LIFE_CREATE( bmath_mf3_s, a );
    BCORE_LIFE_CREATE( bmath_mf3_s, m2 );
    BCORE_LIFE_CREATE( bmath_mfx_eval_result_s, r );
    bmath_mfx_eval_result_s_set_defaults_from_eval( r, o );
    r->fp_type = fp_type;

    sz_t n = o->rows;

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

    if( o->create_a_log ) bmath_mf3_s_to_string( a, &r->a_log );
    bmath_mfx_eval_s_create_image_file( o, a, &o->a_img_file );

    bmath_mf3_s_set_size( m2, n, n );
    bmath_mf3_s_mul_esp( m0, a, m2 );

    r->assert_m = bmath_mf3_s_is_near_one( m2, o->near_limit_f3 );
    r->fdev_m   = bmath_mf3_s_fdev_one( m2 );
    if( res ) bmath_mfx_eval_result_s_copy( res, r );

    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mfx_eval_s_label_run( const bmath_mfx_eval_s* o, sc_t label, tp_t fp_type, fp_t fp, bmath_mfx_eval_result_s* res )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );

    BLM_INIT();
    BCORE_LIFE_CREATE( bmath_mfx_eval_result_s, r );
    switch( fp_type )
    {
        case TYPEOF_bmath_fp_mf2_s_mul         :
        case TYPEOF_bmath_fp_mf3_s_mul         : bmath_mfx_eval_s_run_mul(         o, fp_type, fp, r ); break;
        case TYPEOF_bmath_fp_mf2_s_mul_htp     :
        case TYPEOF_bmath_fp_mf3_s_mul_htp     : bmath_mfx_eval_s_run_mul_htp(     o, fp_type, fp, r ); break;
        case TYPEOF_bmath_fp_mf2_s_htp_mul     :
        case TYPEOF_bmath_fp_mf3_s_htp_mul     : bmath_mfx_eval_s_run_htp_mul(     o, fp_type, fp, r ); break;
        case TYPEOF_bmath_fp_mf2_s_htp_mul_htp :
        case TYPEOF_bmath_fp_mf3_s_htp_mul_htp : bmath_mfx_eval_s_run_htp_mul_htp( o, fp_type, fp, r ); break;

        case TYPEOF_bmath_fp_mf2_s_uav         : bmath_mfx_eval_s_run_uav_f2(      o, fp_type, fp, r ); break;
        case TYPEOF_bmath_fp_mf2_s_svd         : bmath_mfx_eval_s_run_uav_f2(      o, fp_type, fp, r ); break;
        case TYPEOF_bmath_fp_mf2_s_ubd         : bmath_mfx_eval_s_run_uav_f2(      o, fp_type, fp, r ); break;
        case TYPEOF_bmath_fp_mf2_s_lbd         : bmath_mfx_eval_s_run_uav_f2(      o, fp_type, fp, r ); break;

        case TYPEOF_bmath_fp_mf3_s_uav         : bmath_mfx_eval_s_run_uav_f3(      o, fp_type, fp, r ); break;
        case TYPEOF_bmath_fp_mf3_s_svd         : bmath_mfx_eval_s_run_uav_f3(      o, fp_type, fp, r ); break;
        case TYPEOF_bmath_fp_mf3_s_ubd         : bmath_mfx_eval_s_run_uav_f3(      o, fp_type, fp, r ); break;
        case TYPEOF_bmath_fp_mf3_s_lbd         : bmath_mfx_eval_s_run_uav_f3(      o, fp_type, fp, r ); break;

        case TYPEOF_bmath_fp_mf3_s_ua          : bmath_mfx_eval_s_run_ua(          o, fp_type, fp, r ); break;
        case TYPEOF_bmath_fp_mf3_s_av          : bmath_mfx_eval_s_run_av(          o, fp_type, fp, r ); break;
        case TYPEOF_bmath_fp_mf3_s_uau         : bmath_mfx_eval_s_run_sym_uau_htp( o, fp_type, fp, r ); break;
        case TYPEOF_bmath_fp_mf3_s_vav         : bmath_mfx_eval_s_run_sym_uau_htp( o, fp_type, fp, r ); break;
        case TYPEOF_bmath_fp_mf3_s_qrd         : bmath_mfx_eval_s_run_ua(          o, fp_type, fp, r ); break;
        case TYPEOF_bmath_fp_mf3_s_qrd_pmt     : bmath_mfx_eval_s_run_ua(          o, fp_type, fp, r ); break;
        case TYPEOF_bmath_fp_mf3_s_lqd         : bmath_mfx_eval_s_run_av(          o, fp_type, fp, r ); break;
        case TYPEOF_bmath_fp_mf3_s_pmt_lqd     : bmath_mfx_eval_s_run_av(          o, fp_type, fp, r ); break;
        case TYPEOF_bmath_fp_mf3_s_trd_htp     : bmath_mfx_eval_s_run_sym_uau_htp( o, fp_type, fp, r ); break;
        case TYPEOF_bmath_fp_mf3_s_trd         : bmath_mfx_eval_s_run_sym_uau_htp( o, fp_type, fp, r ); break;
        case TYPEOF_bmath_fp_mf3_s_evd_htp     : bmath_mfx_eval_s_run_sym_uau_htp( o, fp_type, fp, r ); break;
        case TYPEOF_bmath_fp_mf3_s_cld         : bmath_mfx_eval_s_run_cld(         o, fp_type, fp, r ); break;
        case TYPEOF_bmath_fp_mf3_s_lud         : bmath_mfx_eval_s_run_lud(         o, fp_type, fp, r ); break;
        case TYPEOF_bmath_fp_mf3_s_inv         : bmath_mfx_eval_s_run_inv(         o, fp_type, fp, r ); break;
        case TYPEOF_bmath_fp_mf3_s_pdf_inv     : bmath_mfx_eval_s_run_pdf_inv(     o, fp_type, fp, r ); break;
        case TYPEOF_bmath_fp_mf3_s_hsm_piv     : bmath_mfx_eval_s_run_hsm_piv(     o, fp_type, fp, r ); break;
        case TYPEOF_bmath_fp_mf3_s_piv         : bmath_mfx_eval_s_run_piv(         o, fp_type, fp, r ); break;
        default: ERR_fa( "Invalid fp_type `#<sc_t>`\n", ifnameof( fp_type ) ); break;
    }

    if( label ) st_s_copy_sc( &r->label, label );

    if( o->assert_all && ( !r->success0 || !r->success1 || !r->assert_a || !r->assert_u || !r->assert_v || !r->assert_m ) )
    {
        st_s* s = st_s_create();
        bmath_mfx_eval_result_s_to_string( r, s );
        ERR_fa( "\n#<sc_t>\nAssertion failed!\n", s->sc );
        st_s_discard( s );
    }

    if( res ) bmath_mfx_eval_result_s_copy( res, r );

    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mfx_eval_s_run( const bmath_mfx_eval_s* o, tp_t fp_type, fp_t fp, bmath_mfx_eval_result_s* res )
{
    bmath_mfx_eval_s_label_run( o, NULL, fp_type, fp, res );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mfx_eval_s_label_run_to_log( const bmath_mfx_eval_s* o, sc_t label, tp_t fp_type, fp_t fp, st_s* log )
{
    ASSERT( fp != NULL );
    ASSERT( fp_type != 0 );

    BLM_INIT();
    BCORE_LIFE_CREATE( bmath_mfx_eval_result_s, r );

    bmath_mfx_eval_s_label_run( o, label, fp_type, fp, r );

    if( log ) bmath_mfx_eval_result_s_to_string( r, log );

    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mfx_eval_s_run_to_log( const bmath_mfx_eval_s* o, tp_t fp_type, fp_t fp, st_s* log )
{
    bmath_mfx_eval_s_label_run_to_log( o, NULL, fp_type, fp, log );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mfx_eval_s_label_run_to_stdout( const bmath_mfx_eval_s* o, sc_t label, tp_t fp_type, fp_t fp )
{
    st_s* s = st_s_create();
    bmath_mfx_eval_s_label_run_to_log( o, label, fp_type, fp, s );
    bcore_msg_fa( "#<sc_t>\n", s->sc );
    st_s_discard( s );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mfx_eval_s_run_to_stdout( const bmath_mfx_eval_s* o, tp_t fp_type, fp_t fp )
{
    bmath_mfx_eval_s_label_run_to_stdout( o, NULL, fp_type, fp );
}

//----------------------------------------------------------------------------------------------------------------------

static void bmath_mfx_eval_s_selftest( void )
{
    BLM_INIT();

    BCORE_LIFE_CREATE( bmath_mfx_eval_s, eval );
    eval->rows = 10;
    eval->cols = 10;
    bmath_mfx_eval_s_run_to_log( eval, typeof( "bmath_fp_mf3_s_svd" ), ( fp_t )bmath_mf3_s_svd, NULL );

    BLM_DOWN();
}

/**********************************************************************************************************************/

BCORE_DEFINE_OBJECT_INST( bcore_inst, bmath_arr_mfx_eval_s ) "{ aware_t _; bmath_mfx_eval_s [] arr; }";

//----------------------------------------------------------------------------------------------------------------------

void bmath_arr_mfx_eval_s_label_run( const bmath_arr_mfx_eval_s* o, sc_t label, tp_t fp_type, fp_t fp )
{
    for( uz_t i = 0; i < o->size; i++ ) bmath_mfx_eval_s_label_run( &o->data[ i ], label, fp_type, fp, NULL );
}

void bmath_arr_mfx_eval_s_label_run_to_log( const bmath_arr_mfx_eval_s* o, sc_t label, tp_t fp_type, fp_t fp, st_s* log )
{
    for( uz_t i = 0; i < o->size; i++ ) bmath_mfx_eval_s_label_run_to_log( &o->data[ i ], label, fp_type, fp, log );
}

void bmath_arr_mfx_eval_s_label_run_to_stdout( const bmath_arr_mfx_eval_s* o, sc_t label, tp_t fp_type, fp_t fp )
{
    for( uz_t i = 0; i < o->size; i++ ) bmath_mfx_eval_s_label_run_to_stdout( &o->data[ i ], label, fp_type, fp );
}

void bmath_arr_mfx_eval_s_run( const bmath_arr_mfx_eval_s* o, tp_t fp_type, fp_t fp )
{
    bmath_arr_mfx_eval_s_label_run( o, NULL, fp_type, fp );
}

void bmath_arr_mfx_eval_s_run_to_log( const bmath_arr_mfx_eval_s* o, tp_t fp_type, fp_t fp, st_s* log )
{
    bmath_arr_mfx_eval_s_label_run_to_log( o, NULL, fp_type, fp, log );
}

void bmath_arr_mfx_eval_s_run_to_stdout( const bmath_arr_mfx_eval_s* o, tp_t fp_type, fp_t fp )
{
    bmath_arr_mfx_eval_s_label_run_to_stdout( o, NULL, fp_type, fp );
}

/**********************************************************************************************************************/

vd_t bmath_mfx_eval_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_mfx_eval" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_OBJECT( bmath_mfx_eval_s );
            BCORE_REGISTER_OBJECT( bmath_mfx_eval_result_s );
            BCORE_REGISTER_OBJECT( bmath_arr_mfx_eval_s );
        }
        break;

        case TYPEOF_selftest:
        {
            bmath_mfx_eval_s_selftest();
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

