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
#include "bmath.xo.h"

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( bmath_mfx_eval, bcore_inst )
#ifdef XOILA_SECTION

//----------------------------------------------------------------------------------------------------------------------

stamp :result_s = aware bcore_inst
{
    st_s label;    // test label
    tp_t fp_type;
    sz_t rows;
    sz_t cols;
    sz_t dim3;

    f3_t density;

    st_s a_log;           // log of matrix a
    st_s u_log;           // log of matrix u
    st_s v_log;           // log of matrix v

    bl_t assert_a = true; // assertion of shape a where applicable
    bl_t assert_u = true; // assertion of shape u where applicable
    bl_t assert_v = true; // assertion of shape v where applicable
    bl_t assert_m = true; // assertion of m equals u * a * vT
    f3_t dev_a = 0;       // fdev of a from desired shape
    f3_t dev_u = 0;       // fdev of u from desired shape
    f3_t dev_v = 0;       // fdev of v from desired shape
    f3_t dev_m = 0;       // fdev of m from equality
    bl_t success0 = true; // success with minimal arguments
    bl_t success1 = true; // success with all arguments
    f3_t time0  = 0;      // operation time with minimal arguments
    f3_t time1  = 0;      // operation time with all arguments

    func void to_string( c@* o, m st_s* string );
    func void to_stdout( c@* o ) { st_s^ txt; o.to_string( txt ); txt.to_stdout(); }
};

//----------------------------------------------------------------------------------------------------------------------

stamp :s = aware bcore_inst
{
    sz_t rows       = 1000;
    sz_t cols       = 1000;
    sz_t dim3       =   -1;   // third dimension (e.g. in multiplication tests); -1: use default

    // random generator
    aware bcore_prsg => prsg = bcore_prsg_lcg_u3_00_s;

    f3_t density    = 1.0;
    bl_t thin_decomposition = true; // thin (default) vs. full decomposition (in decomposition tests where applicable)
    f3_t near_limit_f2 = 1E-3; // limit for near-assertions (f2_t)
    f3_t near_limit_f3 = 1E-6; // limit for near-assertions (f3_t)
    f3_t eps           = 1E-6;  // for function requiring an epsilon

    bl_t create_a_log = false; // log matrix a after conversion
    bl_t create_u_log = false; // log matrix u after conversion
    bl_t create_v_log = false; // log matrix v after conversion

    st_s a_img_file; // create image file of matrix a after conversion
    st_s u_img_file; // create image file of matrix u after conversion
    st_s v_img_file; // create image file of matrix v after conversion

    bl_t assert_all      = true; // asserts correct matrix and computation result
    bl_t prefer_eps_eval = true; // prefers more precise but slower eps algorithms for evaluation
    bl_t test0           = true; // runs minimal parameter test
    bl_t test1           = true; // runs default parameter test

    /// runs evaluation and logs results if desired (log can be NULL)
    func void label_run(           c@* o, sc_t label, tp_t fp_type, fp_t fp, m :result_s* res ); // res can be NULL
    func void label_run_to_log(    c@* o, sc_t label, tp_t fp_type, fp_t fp, m st_s* log );
    func void label_run_to_stdout( c@* o, sc_t label, tp_t fp_type, fp_t fp  );

    func void run(           c@* o, tp_t fp_type, fp_t fp, m :result_s* res ) { o.label_run          ( NULL, fp_type, fp, res ); }
    func void run_to_log(    c@* o, tp_t fp_type, fp_t fp, m st_s* log      ) { o.label_run_to_log   ( NULL, fp_type, fp, log ); }
    func void run_to_stdout( c@* o, tp_t fp_type, fp_t fp                   ) { o.label_run_to_stdout( NULL, fp_type, fp ); }
};

//----------------------------------------------------------------------------------------------------------------------

stamp bmath_arr_mfx_eval_s = aware x_array
{
    bmath_mfx_eval_s [];

    func void push( m@* o, :s* v ) { o.push_c( v ); }

    func void label_run          ( @* o, sc_t label, tp_t fp_type, fp_t fp              ) { foreach( $* e in o ) e.label_run          ( label, fp_type, fp, NULL ); }
    func void label_run_to_log   ( @* o, sc_t label, tp_t fp_type, fp_t fp, m st_s* log ) { foreach( $* e in o ) e.label_run_to_log   ( label, fp_type, fp, log  ); }
    func void label_run_to_stdout( @* o, sc_t label, tp_t fp_type, fp_t fp              ) { foreach( $* e in o ) e.label_run_to_stdout( label, fp_type, fp       ); }

    func void run(           @* o, tp_t fp_type, fp_t fp              ) { o.label_run          ( NULL, fp_type, fp      ); }
    func void run_to_log(    @* o, tp_t fp_type, fp_t fp, m st_s* log ) { o.label_run_to_log   ( NULL, fp_type, fp, log ); }
    func void run_to_stdout( @* o, tp_t fp_type, fp_t fp              ) { o.label_run_to_stdout( NULL, fp_type, fp      ); }
}

//----------------------------------------------------------------------------------------------------------------------

// function pointer names
name bmath_fp_mf2_s_mul;
name bmath_fp_mf2_s_mul_htp;
name bmath_fp_mf2_s_htp_mul;
name bmath_fp_mf2_s_htp_mul_htp;
name bmath_fp_mf2_s_uav;
name bmath_fp_mf2_s_ua;
name bmath_fp_mf2_s_av;
name bmath_fp_mf2_s_uau;
name bmath_fp_mf2_s_vav;
name bmath_fp_mf2_s_trd_htp;
name bmath_fp_mf2_s_trd;
name bmath_fp_mf2_s_evd_htp;
name bmath_fp_mf2_s_svd;
name bmath_fp_mf2_s_ubd;
name bmath_fp_mf2_s_lbd;
name bmath_fp_mf2_s_qrd;
name bmath_fp_mf2_s_qrd_pmt;
name bmath_fp_mf2_s_lqd;
name bmath_fp_mf2_s_pmt_lqd;
name bmath_fp_mf2_s_cld;
name bmath_fp_mf2_s_lud;
name bmath_fp_mf2_s_inv;
name bmath_fp_mf2_s_pdf_inv;
name bmath_fp_mf2_s_piv;
name bmath_fp_mf2_s_hsm_piv;
name bmath_fp_mf2_s_htp;

name bmath_fp_mf3_s_mul;
name bmath_fp_mf3_s_mul_htp;
name bmath_fp_mf3_s_htp_mul;
name bmath_fp_mf3_s_htp_mul_htp;

name bmath_fp_mf3_s_uav;
name bmath_fp_mf3_s_ua;
name bmath_fp_mf3_s_av;
name bmath_fp_mf3_s_uau;
name bmath_fp_mf3_s_vav;

name bmath_fp_mf3_s_trd_htp;
name bmath_fp_mf3_s_trd;
name bmath_fp_mf3_s_evd_htp;
name bmath_fp_mf3_s_svd;
name bmath_fp_mf3_s_ubd;
name bmath_fp_mf3_s_lbd;
name bmath_fp_mf3_s_qrd;
name bmath_fp_mf3_s_qrd_pmt;
name bmath_fp_mf3_s_lqd;
name bmath_fp_mf3_s_pmt_lqd;
name bmath_fp_mf3_s_cld;
name bmath_fp_mf3_s_lud;
name bmath_fp_mf3_s_inv;
name bmath_fp_mf3_s_pdf_inv;
name bmath_fp_mf3_s_piv;
name bmath_fp_mf3_s_hsm_piv;
name bmath_fp_mf3_s_htp;

name bmath_fp_asmf3_s_mul;
name bmath_fp_asmf3_s_mul_htp;
name bmath_fp_asmf3_s_htp_mul;
name bmath_fp_asmf3_s_htp_mul_htp;
name bmath_fp_asmf2_s_mul;
name bmath_fp_asmf2_s_mul_htp;
name bmath_fp_asmf2_s_htp_mul;
name bmath_fp_asmf2_s_htp_mul_htp;
name bmath_fp_xsmf2_s_mul;
name bmath_fp_xsmf2_s_mul_htp;
name bmath_fp_xsmf2_s_htp_mul;
name bmath_fp_xsmf2_s_htp_mul_htp;
name bmath_fp_xsmf3_s_mul;
name bmath_fp_xsmf3_s_mul_htp;
name bmath_fp_xsmf3_s_htp_mul;
name bmath_fp_xsmf3_s_htp_mul_htp;

#endif // XOILA_SECTION

/**********************************************************************************************************************/

#define BMATH_MFX_EVAL_S_RUN(           o, bmath_fp_name, fp ) bmath_mfx_eval_s_label_run(           o, #fp, TYPEOF_bmath_fp_##bmath_fp_name, (fp_t)fp )
#define BMATH_MFX_EVAL_S_RUN_TO_LOG(    o, bmath_fp_name, fp ) bmath_mfx_eval_s_label_run_to_log(    o, #fp, TYPEOF_bmath_fp_##bmath_fp_name, (fp_t)fp )
#define BMATH_MFX_EVAL_S_RUN_TO_STDOUT( o, bmath_fp_name, fp ) bmath_mfx_eval_s_label_run_to_stdout( o, #fp, TYPEOF_bmath_fp_##bmath_fp_name, (fp_t)fp )

#define BMATH_ARR_MFX_EVAL_S_RUN(           o, bmath_fp_name, fp ) bmath_arr_mfx_eval_s_label_run(           o, #fp, TYPEOF_bmath_fp_##bmath_fp_name, (fp_t)fp )
#define BMATH_ARR_MFX_EVAL_S_RUN_TO_LOG(    o, bmath_fp_name, fp ) bmath_arr_mfx_eval_s_label_run_to_log(    o, #fp, TYPEOF_bmath_fp_##bmath_fp_name, (fp_t)fp )
#define BMATH_ARR_MFX_EVAL_S_RUN_TO_STDOUT( o, bmath_fp_name, fp ) bmath_arr_mfx_eval_s_label_run_to_stdout( o, #fp, TYPEOF_bmath_fp_##bmath_fp_name, (fp_t)fp )

/**********************************************************************************************************************/

vd_t bmath_mfx_eval_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif  // BMATH_MFX_EVAL_H
