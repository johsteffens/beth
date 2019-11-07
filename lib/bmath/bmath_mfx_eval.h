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

#ifndef BMATH_MFX_EVAL_H
#define BMATH_MFX_EVAL_H

/**********************************************************************************************************************/

/** Collection of evaluation routines for bmath_matrix
 */

/**********************************************************************************************************************/

#include "bcore_std.h"
#include "bmath_quicktypes.h"

/**********************************************************************************************************************/

BCORE_DECLARE_OBJECT( bmath_mfx_eval_s )
{
    aware_t _;
    sz_t rows;
    sz_t cols;
    sz_t dim3;       // third dimension (e.g. in multiplication tests); -1: use default

    u2_t seed;       // random seed
    f3_t density;    // random matrix density
    bl_t full;       // full vs thin decomposition (where applicable)
    f3_t near_limit_f2; // limit for near-assertions (f2_t)
    f3_t near_limit_f3; // limit for near-assertions (f3_t)
    f3_t eps;        // for functions requiring an epsilon

    bl_t create_a_log; // log matrix a after conversion
    bl_t create_u_log; // log matrix u after conversion
    bl_t create_v_log; // log matrix v after conversion

    st_s a_img_file; // create image file of matrix a after conversion
    st_s u_img_file; // create image file of matrix u after conversion
    st_s v_img_file; // create image file of matrix v after conversion

    bl_t assert_all; // asserts correct matrix and computation result

    bl_t test0;      // runs minimal parameter test (ifapplicable)
    bl_t test1;      // runs default parameter test
};

//----------------------------------------------------------------------------------------------------------------------

BCORE_DECLARE_OBJECT( bmath_mfx_eval_result_s )
{
    aware_t _;
    st_s label;    // test label
    tp_t fp_type;
    sz_t rows;
    sz_t cols;
    sz_t dim3;     // third dimension (e.g. in multiplication tests); -1: use default
    f3_t density;  // random matrix density

    st_s a_log;    // log of matrix a
    st_s u_log;    // log of matrix u
    st_s v_log;    // log of matrix v

    bl_t assert_a; // assertion of shape a where applicable
    bl_t assert_u; // assertion of shape u where applicable
    bl_t assert_v; // assertion of shape v where applicable
    bl_t assert_m; // assertion of m equals u * a * vT

    f3_t fdev_a;   // fdev of a from desired shape
    f3_t fdev_u;   // fdev of u from desired shape
    f3_t fdev_v;   // fdev of v from desired shape
    f3_t fdev_m;   // fdev of m from equality

    bl_t success0; // success with minimal arguments
    bl_t success1; // success with all arguments

    f3_t time0;    // operation time with minimal arguments
    f3_t time1;    // operation time with all arguments
};

void bmath_mfx_eval_result_s_to_string( const bmath_mfx_eval_result_s* o, st_s* string );
void bmath_mfx_eval_result_s_to_stdout( const bmath_mfx_eval_result_s* o );

//----------------------------------------------------------------------------------------------------------------------

/// runs evaluation and logs results if desired (log can be NULL)
void bmath_mfx_eval_s_run(           const bmath_mfx_eval_s* o, tp_t fp_type, fp_t fp, bmath_mfx_eval_result_s* res ); // res can be NULL
void bmath_mfx_eval_s_run_to_log(    const bmath_mfx_eval_s* o, tp_t fp_type, fp_t fp, st_s* log );
void bmath_mfx_eval_s_run_to_stdout( const bmath_mfx_eval_s* o, tp_t fp_type, fp_t fp  );

void bmath_mfx_eval_s_label_run(           const bmath_mfx_eval_s* o, sc_t label, tp_t fp_type, fp_t fp, bmath_mfx_eval_result_s* res ); // res can be NULL
void bmath_mfx_eval_s_label_run_to_log(    const bmath_mfx_eval_s* o, sc_t label, tp_t fp_type, fp_t fp, st_s* log );
void bmath_mfx_eval_s_label_run_to_stdout( const bmath_mfx_eval_s* o, sc_t label, tp_t fp_type, fp_t fp  );

#define BMATH_MFX_EVAL_S_RUN(           o, bmath_fp_name, fp ) bmath_mfx_eval_s_label_run(           o, #fp, TYPEOF_bmath_fp_##bmath_fp_name, (fp_t)fp )
#define BMATH_MFX_EVAL_S_RUN_TO_LOG(    o, bmath_fp_name, fp ) bmath_mfx_eval_s_label_run_to_log(    o, #fp, TYPEOF_bmath_fp_##bmath_fp_name, (fp_t)fp )
#define BMATH_MFX_EVAL_S_RUN_TO_STDOUT( o, bmath_fp_name, fp ) bmath_mfx_eval_s_label_run_to_stdout( o, #fp, TYPEOF_bmath_fp_##bmath_fp_name, (fp_t)fp )

/**********************************************************************************************************************/

BCORE_DECLARE_OBJECT( bmath_arr_mfx_eval_s )
{
    aware_t _;
    BCORE_ARRAY_DYN_SOLID_STATIC_S( bmath_mfx_eval_s, );
};

static inline void bmath_arr_mfx_eval_s_set_size( bmath_arr_mfx_eval_s* o, uz_t size )
{
    bcore_array_a_set_size( ( bcore_array* )o, size );
}

static inline void bmath_arr_mfx_eval_s_push( bmath_arr_mfx_eval_s* o, const bmath_mfx_eval_s* v )
{
    bcore_array_a_push( ( bcore_array* )o, sr_twc( TYPEOF_bmath_mfx_eval_s, v ) );
}

void bmath_arr_mfx_eval_s_run(           const bmath_arr_mfx_eval_s* o, tp_t fp_type, fp_t fp ); // no logging
void bmath_arr_mfx_eval_s_run_to_log(    const bmath_arr_mfx_eval_s* o, tp_t fp_type, fp_t fp, st_s* log ); // using fp instead of o->fp
void bmath_arr_mfx_eval_s_run_to_stdout( const bmath_arr_mfx_eval_s* o, tp_t fp_type, fp_t fp );

void bmath_arr_mfx_eval_s_label_run(           const bmath_arr_mfx_eval_s* o, sc_t label, tp_t fp_type, fp_t fp ); // no logging
void bmath_arr_mfx_eval_s_label_run_to_log(    const bmath_arr_mfx_eval_s* o, sc_t label, tp_t fp_type, fp_t fp, st_s* log ); // using fp instead of o->fp
void bmath_arr_mfx_eval_s_label_run_to_stdout( const bmath_arr_mfx_eval_s* o, sc_t label, tp_t fp_type, fp_t fp );

#define BMATH_ARR_MFX_EVAL_S_RUN(           o, bmath_fp_name, fp ) bmath_arr_mfx_eval_s_label_run(           o, #fp, TYPEOF_bmath_fp_##bmath_fp_name, (fp_t)fp )
#define BMATH_ARR_MFX_EVAL_S_RUN_TO_LOG(    o, bmath_fp_name, fp ) bmath_arr_mfx_eval_s_label_run_to_log(    o, #fp, TYPEOF_bmath_fp_##bmath_fp_name, (fp_t)fp )
#define BMATH_ARR_MFX_EVAL_S_RUN_TO_STDOUT( o, bmath_fp_name, fp ) bmath_arr_mfx_eval_s_label_run_to_stdout( o, #fp, TYPEOF_bmath_fp_##bmath_fp_name, (fp_t)fp )

/**********************************************************************************************************************/

vd_t bmath_mfx_eval_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif  // BMATH_MFX_EVAL_H
