/** This file was generated from beth-plant source code.
 *  Compiling Agent : bcore_plant_compiler (C) 2019 J.B.Steffens
 *  Last File Update: 2019-12-06T20:06:12Z
 *
 *  Copyright and License of this File:
 *
 *  Generated code inherits the copyright and license of the underlying beth-plant source code.
 *  Source code defining this file is distributed across following files:
 *
 *  bmath_f3_op.h
 *  bmath_xsmf2.h
 *  bmath_xsmf3.h
 *  bmath_asmf2.h
 *  bmath_asmf3.h
 *
 */

#include "bmath_planted.h"
#include "bcore_spect.h"
#include "bcore_spect_inst.h"
#include "bcore_sr.h"
#include "bcore_const_manager.h"


/**********************************************************************************************************************/
// source: bmath_f3_op.h
#include "bmath_f3_op.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bmath_f3_op

BCORE_DEFINE_SPECT( bcore_inst, bmath_f3_op )
"{"
    "bcore_spect_header_s header;"
    "feature aware bmath_f3_op : get_arity;"
    "feature aware bmath_f3_op : aofx;"
    "feature aware bmath_f3_op : aogx;"
    "feature aware bmath_f3_op : aogy;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bmath_f3_op_ar0

BCORE_DEFINE_OBJECT_INST_P( bmath_f3_op_ar0_zero_s )
"aware bmath_f3_op_ar0"
"{"
    "func ^:f;"
    "func bmath_f3_op:get_arity;"
    "func bmath_f3_op_ar0:of;"
    "func bmath_f3_op:aofx;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bmath_f3_op_ar0_one_s )
"aware bmath_f3_op_ar0"
"{"
    "func ^:f;"
    "func bmath_f3_op:get_arity;"
    "func bmath_f3_op_ar0:of;"
    "func bmath_f3_op:aofx;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bmath_f3_op_ar0_literal_s )
"aware bmath_f3_op_ar0"
"{"
    "f3_t v;"
    "func ^:of;"
    "func bmath_f3_op:get_arity;"
    "func bmath_f3_op_ar0:f;"
    "func bmath_f3_op:aofx;"
"}";

BCORE_DEFINE_SPECT( bmath_f3_op, bmath_f3_op_ar0 )
"{"
    "bcore_spect_header_s header;"
    "feature aware bmath_f3_op_ar0 : of;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bmath_f3_op_ar1

BCORE_DEFINE_OBJECT_INST_P( bmath_f3_op_ar1_zero_s )
"aware bmath_f3_op_ar1"
"{"
    "func ^:fx;"
    "func ^:gy;"
    "func bmath_f3_op:get_arity;"
    "func bmath_f3_op_ar1:ofx;"
    "func bmath_f3_op_ar1:ogy;"
    "func bmath_f3_op:aofx;"
    "func bmath_f3_op:aogy;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bmath_f3_op_ar1_one_s )
"aware bmath_f3_op_ar1"
"{"
    "func ^:fx;"
    "func ^:gy;"
    "func bmath_f3_op:get_arity;"
    "func bmath_f3_op_ar1:ofx;"
    "func bmath_f3_op_ar1:ogy;"
    "func bmath_f3_op:aofx;"
    "func bmath_f3_op:aogy;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bmath_f3_op_ar1_identity_s )
"aware bmath_f3_op_ar1"
"{"
    "func ^:fx;"
    "func ^:gy;"
    "func bmath_f3_op:get_arity;"
    "func bmath_f3_op_ar1:ofx;"
    "func bmath_f3_op_ar1:ogy;"
    "func bmath_f3_op:aofx;"
    "func bmath_f3_op:aogy;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bmath_f3_op_ar1_neg_s )
"aware bmath_f3_op_ar1"
"{"
    "func ^:fx;"
    "func ^:gy;"
    "func bmath_f3_op:get_arity;"
    "func bmath_f3_op_ar1:ofx;"
    "func bmath_f3_op_ar1:ogy;"
    "func bmath_f3_op:aofx;"
    "func bmath_f3_op:aogy;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bmath_f3_op_ar1_floor_s )
"aware bmath_f3_op_ar1"
"{"
    "func ^:fx;"
    "func ^:gy;"
    "func bmath_f3_op:get_arity;"
    "func bmath_f3_op_ar1:ofx;"
    "func bmath_f3_op_ar1:ogy;"
    "func bmath_f3_op:aofx;"
    "func bmath_f3_op:aogy;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bmath_f3_op_ar1_ceil_s )
"aware bmath_f3_op_ar1"
"{"
    "func ^:fx;"
    "func ^:gy;"
    "func bmath_f3_op:get_arity;"
    "func bmath_f3_op_ar1:ofx;"
    "func bmath_f3_op_ar1:ogy;"
    "func bmath_f3_op:aofx;"
    "func bmath_f3_op:aogy;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bmath_f3_op_ar1_exp_s )
"aware bmath_f3_op_ar1"
"{"
    "func ^:fx;"
    "func ^:gy;"
    "func bmath_f3_op:get_arity;"
    "func bmath_f3_op_ar1:ofx;"
    "func bmath_f3_op_ar1:ogy;"
    "func bmath_f3_op:aofx;"
    "func bmath_f3_op:aogy;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bmath_f3_op_ar1_inv_s )
"aware bmath_f3_op_ar1"
"{"
    "func ^:fx;"
    "func ^:gy;"
    "func bmath_f3_op:get_arity;"
    "func bmath_f3_op_ar1:ofx;"
    "func bmath_f3_op_ar1:ogy;"
    "func bmath_f3_op:aofx;"
    "func bmath_f3_op:aogy;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bmath_f3_op_ar1_lgst_s )
"aware bmath_f3_op_ar1"
"{"
    "func ^:fx;"
    "func ^:gy;"
    "func bmath_f3_op:get_arity;"
    "func bmath_f3_op_ar1:ofx;"
    "func bmath_f3_op_ar1:ogy;"
    "func bmath_f3_op:aofx;"
    "func bmath_f3_op:aogy;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bmath_f3_op_ar1_lgst_hard_s )
"aware bmath_f3_op_ar1"
"{"
    "func ^:fx;"
    "func ^:gy;"
    "func bmath_f3_op:get_arity;"
    "func bmath_f3_op_ar1:ofx;"
    "func bmath_f3_op_ar1:ogy;"
    "func bmath_f3_op:aofx;"
    "func bmath_f3_op:aogy;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bmath_f3_op_ar1_lgst_leaky_s )
"aware bmath_f3_op_ar1"
"{"
    "func ^:fx;"
    "func ^:gy;"
    "func bmath_f3_op:get_arity;"
    "func bmath_f3_op_ar1:ofx;"
    "func bmath_f3_op_ar1:ogy;"
    "func bmath_f3_op:aofx;"
    "func bmath_f3_op:aogy;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bmath_f3_op_ar1_tanh_s )
"aware bmath_f3_op_ar1"
"{"
    "func ^:fx;"
    "func ^:gy;"
    "func bmath_f3_op:get_arity;"
    "func bmath_f3_op_ar1:ofx;"
    "func bmath_f3_op_ar1:ogy;"
    "func bmath_f3_op:aofx;"
    "func bmath_f3_op:aogy;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bmath_f3_op_ar1_tanh_hard_s )
"aware bmath_f3_op_ar1"
"{"
    "func ^:fx;"
    "func ^:gy;"
    "func bmath_f3_op:get_arity;"
    "func bmath_f3_op_ar1:ofx;"
    "func bmath_f3_op_ar1:ogy;"
    "func bmath_f3_op:aofx;"
    "func bmath_f3_op:aogy;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bmath_f3_op_ar1_tanh_leaky_s )
"aware bmath_f3_op_ar1"
"{"
    "func ^:fx;"
    "func ^:gy;"
    "func bmath_f3_op:get_arity;"
    "func bmath_f3_op_ar1:ofx;"
    "func bmath_f3_op_ar1:ogy;"
    "func bmath_f3_op:aofx;"
    "func bmath_f3_op:aogy;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bmath_f3_op_ar1_softplus_s )
"aware bmath_f3_op_ar1"
"{"
    "func ^:fx;"
    "func ^:gy;"
    "func bmath_f3_op:get_arity;"
    "func bmath_f3_op_ar1:ofx;"
    "func bmath_f3_op_ar1:ogy;"
    "func bmath_f3_op:aofx;"
    "func bmath_f3_op:aogy;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bmath_f3_op_ar1_relu_s )
"aware bmath_f3_op_ar1"
"{"
    "func ^:fx;"
    "func ^:gy;"
    "func bmath_f3_op:get_arity;"
    "func bmath_f3_op_ar1:ofx;"
    "func bmath_f3_op_ar1:ogy;"
    "func bmath_f3_op:aofx;"
    "func bmath_f3_op:aogy;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bmath_f3_op_ar1_relu_leaky_s )
"aware bmath_f3_op_ar1"
"{"
    "func ^:fx;"
    "func ^:gy;"
    "func bmath_f3_op:get_arity;"
    "func bmath_f3_op_ar1:ofx;"
    "func bmath_f3_op_ar1:ogy;"
    "func bmath_f3_op:aofx;"
    "func bmath_f3_op:aogy;"
"}";

BCORE_DEFINE_SPECT( bmath_f3_op, bmath_f3_op_ar1 )
"{"
    "bcore_spect_header_s header;"
    "feature aware bmath_f3_op_ar1 : ofx;"
    "feature aware bmath_f3_op_ar1 : ogy;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bmath_f3_op_ar2

BCORE_DEFINE_OBJECT_INST_P( bmath_f3_op_ar2_add_s )
"aware bmath_f3_op_ar2"
"{"
    "func ^:fx;"
    "func ^:gxa;"
    "func ^:gxb;"
    "func bmath_f3_op_ar2:ofx;"
    "func bmath_f3_op_ar2:ogxa;"
    "func bmath_f3_op_ar2:ogxb;"
    "func bmath_f3_op:get_arity;"
    "func bmath_f3_op:aofx;"
    "func bmath_f3_op:aogx;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bmath_f3_op_ar2_sub_s )
"aware bmath_f3_op_ar2"
"{"
    "func ^:fx;"
    "func ^:gxa;"
    "func ^:gxb;"
    "func bmath_f3_op_ar2:ofx;"
    "func bmath_f3_op_ar2:ogxa;"
    "func bmath_f3_op_ar2:ogxb;"
    "func bmath_f3_op:get_arity;"
    "func bmath_f3_op:aofx;"
    "func bmath_f3_op:aogx;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bmath_f3_op_ar2_mul_s )
"aware bmath_f3_op_ar2"
"{"
    "func ^:fx;"
    "func ^:gxa;"
    "func ^:gxb;"
    "func bmath_f3_op_ar2:ofx;"
    "func bmath_f3_op_ar2:ogxa;"
    "func bmath_f3_op_ar2:ogxb;"
    "func bmath_f3_op:get_arity;"
    "func bmath_f3_op:aofx;"
    "func bmath_f3_op:aogx;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bmath_f3_op_ar2_div_s )
"aware bmath_f3_op_ar2"
"{"
    "func ^:fx;"
    "func ^:gxa;"
    "func ^:gxb;"
    "func bmath_f3_op_ar2:ofx;"
    "func bmath_f3_op_ar2:ogxa;"
    "func bmath_f3_op_ar2:ogxb;"
    "func bmath_f3_op:get_arity;"
    "func bmath_f3_op:aofx;"
    "func bmath_f3_op:aogx;"
"}";

BCORE_DEFINE_SPECT( bmath_f3_op, bmath_f3_op_ar2 )
"{"
    "bcore_spect_header_s header;"
    "feature aware bmath_f3_op_ar2 : ofx;"
    "feature aware bmath_f3_op_ar2 : ogxa;"
    "feature aware bmath_f3_op_ar2 : ogxb;"
"}";

/**********************************************************************************************************************/
// source: bmath_xsmf2.h
#include "bmath_xsmf2.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bmath_xsmf2

BCORE_DEFINE_OBJECT_INST_P( bmath_xsmf2_s )
"aware bcore_inst"
"{"
    "sz_t slos;"
    "sz_t xons;"
    "sz_t rows;"
    "sz_t i_stride;"
    "sz_t [] i;"
    "f2_t [] v;"
    "func bcore_fp:copy_typed;"
"}";

/**********************************************************************************************************************/
// source: bmath_xsmf3.h
#include "bmath_xsmf3.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bmath_xsmf3

BCORE_DEFINE_OBJECT_INST_P( bmath_xsmf3_s )
"aware bcore_inst"
"{"
    "sz_t slos;"
    "sz_t xons;"
    "sz_t rows;"
    "sz_t i_stride;"
    "sz_t [] i;"
    "f3_t [] v;"
    "func bcore_fp:copy_typed;"
"}";

/**********************************************************************************************************************/
// source: bmath_asmf2.h
#include "bmath_asmf2.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bmath_asmf2

BCORE_DEFINE_OBJECT_INST_P( bmath_asmf2_s )
"aware bcore_inst"
"{"
    "sz_t cols;"
    "sz_t rows;"
    "sz_t i_stride;"
    "sz_t [] i;"
    "f2_t [] v;"
    "func bcore_fp:copy_typed;"
"}";

/**********************************************************************************************************************/
// source: bmath_asmf3.h
#include "bmath_asmf3.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bmath_asmf3

BCORE_DEFINE_OBJECT_INST_P( bmath_asmf3_s )
"aware bcore_inst"
"{"
    "sz_t cols;"
    "sz_t rows;"
    "sz_t i_stride;"
    "sz_t [] i;"
    "f3_t [] v;"
    "func bcore_fp:copy_typed;"
"}";

/**********************************************************************************************************************/

vd_t bmath_planted_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_planted" ) ) )
    {
        case TYPEOF_init1:
        {
            // Comment or remove line below to rebuild this target.
            bcore_const_x_set_d( typeof( "bmath_planted_hash" ), sr_tp( 2295039835 ) );

            // --------------------------------------------------------------------
            // source: bmath_f3_op.h

            // group: bmath_f3_op
            BCORE_REGISTER_FEATURE( bmath_f3_op_get_arity );
            BCORE_REGISTER_FEATURE( bmath_f3_op_aofx );
            BCORE_REGISTER_FEATURE( bmath_f3_op_aogx );
            BCORE_REGISTER_FEATURE( bmath_f3_op_aogy );
            BCORE_REGISTER_SPECT( bmath_f3_op );

            // group: bmath_f3_op_ar0
            BCORE_REGISTER_FEATURE( bmath_f3_op_ar0_of );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar0_f, bmath_f3_op_ar0_zero_s_f );
            BCORE_REGISTER_FFUNC( bmath_f3_op_get_arity, bmath_f3_op_ar0_zero_s_get_arity );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar0_of, bmath_f3_op_ar0_zero_s_of );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aofx, bmath_f3_op_ar0_zero_s_aofx );
            BCORE_REGISTER_OBJECT( bmath_f3_op_ar0_zero_s );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar0_f, bmath_f3_op_ar0_one_s_f );
            BCORE_REGISTER_FFUNC( bmath_f3_op_get_arity, bmath_f3_op_ar0_one_s_get_arity );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar0_of, bmath_f3_op_ar0_one_s_of );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aofx, bmath_f3_op_ar0_one_s_aofx );
            BCORE_REGISTER_OBJECT( bmath_f3_op_ar0_one_s );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar0_of, bmath_f3_op_ar0_literal_s_of );
            BCORE_REGISTER_FFUNC( bmath_f3_op_get_arity, bmath_f3_op_ar0_literal_s_get_arity );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar0_f, bmath_f3_op_ar0_literal_s_f );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aofx, bmath_f3_op_ar0_literal_s_aofx );
            BCORE_REGISTER_OBJECT( bmath_f3_op_ar0_literal_s );
            BCORE_REGISTER_SPECT( bmath_f3_op_ar0 );

            // group: bmath_f3_op_ar1
            BCORE_REGISTER_FEATURE( bmath_f3_op_ar1_ofx );
            BCORE_REGISTER_FEATURE( bmath_f3_op_ar1_ogy );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_fx, bmath_f3_op_ar1_zero_s_fx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_gy, bmath_f3_op_ar1_zero_s_gy );
            BCORE_REGISTER_FFUNC( bmath_f3_op_get_arity, bmath_f3_op_ar1_zero_s_get_arity );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_ofx, bmath_f3_op_ar1_zero_s_ofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_ogy, bmath_f3_op_ar1_zero_s_ogy );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aofx, bmath_f3_op_ar1_zero_s_aofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aogy, bmath_f3_op_ar1_zero_s_aogy );
            BCORE_REGISTER_OBJECT( bmath_f3_op_ar1_zero_s );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_fx, bmath_f3_op_ar1_one_s_fx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_gy, bmath_f3_op_ar1_one_s_gy );
            BCORE_REGISTER_FFUNC( bmath_f3_op_get_arity, bmath_f3_op_ar1_one_s_get_arity );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_ofx, bmath_f3_op_ar1_one_s_ofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_ogy, bmath_f3_op_ar1_one_s_ogy );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aofx, bmath_f3_op_ar1_one_s_aofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aogy, bmath_f3_op_ar1_one_s_aogy );
            BCORE_REGISTER_OBJECT( bmath_f3_op_ar1_one_s );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_fx, bmath_f3_op_ar1_identity_s_fx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_gy, bmath_f3_op_ar1_identity_s_gy );
            BCORE_REGISTER_FFUNC( bmath_f3_op_get_arity, bmath_f3_op_ar1_identity_s_get_arity );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_ofx, bmath_f3_op_ar1_identity_s_ofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_ogy, bmath_f3_op_ar1_identity_s_ogy );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aofx, bmath_f3_op_ar1_identity_s_aofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aogy, bmath_f3_op_ar1_identity_s_aogy );
            BCORE_REGISTER_OBJECT( bmath_f3_op_ar1_identity_s );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_fx, bmath_f3_op_ar1_neg_s_fx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_gy, bmath_f3_op_ar1_neg_s_gy );
            BCORE_REGISTER_FFUNC( bmath_f3_op_get_arity, bmath_f3_op_ar1_neg_s_get_arity );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_ofx, bmath_f3_op_ar1_neg_s_ofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_ogy, bmath_f3_op_ar1_neg_s_ogy );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aofx, bmath_f3_op_ar1_neg_s_aofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aogy, bmath_f3_op_ar1_neg_s_aogy );
            BCORE_REGISTER_OBJECT( bmath_f3_op_ar1_neg_s );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_fx, bmath_f3_op_ar1_floor_s_fx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_gy, bmath_f3_op_ar1_floor_s_gy );
            BCORE_REGISTER_FFUNC( bmath_f3_op_get_arity, bmath_f3_op_ar1_floor_s_get_arity );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_ofx, bmath_f3_op_ar1_floor_s_ofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_ogy, bmath_f3_op_ar1_floor_s_ogy );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aofx, bmath_f3_op_ar1_floor_s_aofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aogy, bmath_f3_op_ar1_floor_s_aogy );
            BCORE_REGISTER_OBJECT( bmath_f3_op_ar1_floor_s );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_fx, bmath_f3_op_ar1_ceil_s_fx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_gy, bmath_f3_op_ar1_ceil_s_gy );
            BCORE_REGISTER_FFUNC( bmath_f3_op_get_arity, bmath_f3_op_ar1_ceil_s_get_arity );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_ofx, bmath_f3_op_ar1_ceil_s_ofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_ogy, bmath_f3_op_ar1_ceil_s_ogy );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aofx, bmath_f3_op_ar1_ceil_s_aofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aogy, bmath_f3_op_ar1_ceil_s_aogy );
            BCORE_REGISTER_OBJECT( bmath_f3_op_ar1_ceil_s );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_fx, bmath_f3_op_ar1_exp_s_fx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_gy, bmath_f3_op_ar1_exp_s_gy );
            BCORE_REGISTER_FFUNC( bmath_f3_op_get_arity, bmath_f3_op_ar1_exp_s_get_arity );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_ofx, bmath_f3_op_ar1_exp_s_ofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_ogy, bmath_f3_op_ar1_exp_s_ogy );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aofx, bmath_f3_op_ar1_exp_s_aofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aogy, bmath_f3_op_ar1_exp_s_aogy );
            BCORE_REGISTER_OBJECT( bmath_f3_op_ar1_exp_s );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_fx, bmath_f3_op_ar1_inv_s_fx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_gy, bmath_f3_op_ar1_inv_s_gy );
            BCORE_REGISTER_FFUNC( bmath_f3_op_get_arity, bmath_f3_op_ar1_inv_s_get_arity );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_ofx, bmath_f3_op_ar1_inv_s_ofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_ogy, bmath_f3_op_ar1_inv_s_ogy );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aofx, bmath_f3_op_ar1_inv_s_aofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aogy, bmath_f3_op_ar1_inv_s_aogy );
            BCORE_REGISTER_OBJECT( bmath_f3_op_ar1_inv_s );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_fx, bmath_f3_op_ar1_lgst_s_fx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_gy, bmath_f3_op_ar1_lgst_s_gy );
            BCORE_REGISTER_FFUNC( bmath_f3_op_get_arity, bmath_f3_op_ar1_lgst_s_get_arity );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_ofx, bmath_f3_op_ar1_lgst_s_ofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_ogy, bmath_f3_op_ar1_lgst_s_ogy );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aofx, bmath_f3_op_ar1_lgst_s_aofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aogy, bmath_f3_op_ar1_lgst_s_aogy );
            BCORE_REGISTER_OBJECT( bmath_f3_op_ar1_lgst_s );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_fx, bmath_f3_op_ar1_lgst_hard_s_fx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_gy, bmath_f3_op_ar1_lgst_hard_s_gy );
            BCORE_REGISTER_FFUNC( bmath_f3_op_get_arity, bmath_f3_op_ar1_lgst_hard_s_get_arity );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_ofx, bmath_f3_op_ar1_lgst_hard_s_ofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_ogy, bmath_f3_op_ar1_lgst_hard_s_ogy );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aofx, bmath_f3_op_ar1_lgst_hard_s_aofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aogy, bmath_f3_op_ar1_lgst_hard_s_aogy );
            BCORE_REGISTER_OBJECT( bmath_f3_op_ar1_lgst_hard_s );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_fx, bmath_f3_op_ar1_lgst_leaky_s_fx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_gy, bmath_f3_op_ar1_lgst_leaky_s_gy );
            BCORE_REGISTER_FFUNC( bmath_f3_op_get_arity, bmath_f3_op_ar1_lgst_leaky_s_get_arity );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_ofx, bmath_f3_op_ar1_lgst_leaky_s_ofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_ogy, bmath_f3_op_ar1_lgst_leaky_s_ogy );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aofx, bmath_f3_op_ar1_lgst_leaky_s_aofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aogy, bmath_f3_op_ar1_lgst_leaky_s_aogy );
            BCORE_REGISTER_OBJECT( bmath_f3_op_ar1_lgst_leaky_s );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_fx, bmath_f3_op_ar1_tanh_s_fx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_gy, bmath_f3_op_ar1_tanh_s_gy );
            BCORE_REGISTER_FFUNC( bmath_f3_op_get_arity, bmath_f3_op_ar1_tanh_s_get_arity );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_ofx, bmath_f3_op_ar1_tanh_s_ofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_ogy, bmath_f3_op_ar1_tanh_s_ogy );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aofx, bmath_f3_op_ar1_tanh_s_aofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aogy, bmath_f3_op_ar1_tanh_s_aogy );
            BCORE_REGISTER_OBJECT( bmath_f3_op_ar1_tanh_s );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_fx, bmath_f3_op_ar1_tanh_hard_s_fx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_gy, bmath_f3_op_ar1_tanh_hard_s_gy );
            BCORE_REGISTER_FFUNC( bmath_f3_op_get_arity, bmath_f3_op_ar1_tanh_hard_s_get_arity );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_ofx, bmath_f3_op_ar1_tanh_hard_s_ofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_ogy, bmath_f3_op_ar1_tanh_hard_s_ogy );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aofx, bmath_f3_op_ar1_tanh_hard_s_aofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aogy, bmath_f3_op_ar1_tanh_hard_s_aogy );
            BCORE_REGISTER_OBJECT( bmath_f3_op_ar1_tanh_hard_s );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_fx, bmath_f3_op_ar1_tanh_leaky_s_fx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_gy, bmath_f3_op_ar1_tanh_leaky_s_gy );
            BCORE_REGISTER_FFUNC( bmath_f3_op_get_arity, bmath_f3_op_ar1_tanh_leaky_s_get_arity );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_ofx, bmath_f3_op_ar1_tanh_leaky_s_ofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_ogy, bmath_f3_op_ar1_tanh_leaky_s_ogy );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aofx, bmath_f3_op_ar1_tanh_leaky_s_aofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aogy, bmath_f3_op_ar1_tanh_leaky_s_aogy );
            BCORE_REGISTER_OBJECT( bmath_f3_op_ar1_tanh_leaky_s );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_fx, bmath_f3_op_ar1_softplus_s_fx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_gy, bmath_f3_op_ar1_softplus_s_gy );
            BCORE_REGISTER_FFUNC( bmath_f3_op_get_arity, bmath_f3_op_ar1_softplus_s_get_arity );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_ofx, bmath_f3_op_ar1_softplus_s_ofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_ogy, bmath_f3_op_ar1_softplus_s_ogy );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aofx, bmath_f3_op_ar1_softplus_s_aofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aogy, bmath_f3_op_ar1_softplus_s_aogy );
            BCORE_REGISTER_OBJECT( bmath_f3_op_ar1_softplus_s );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_fx, bmath_f3_op_ar1_relu_s_fx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_gy, bmath_f3_op_ar1_relu_s_gy );
            BCORE_REGISTER_FFUNC( bmath_f3_op_get_arity, bmath_f3_op_ar1_relu_s_get_arity );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_ofx, bmath_f3_op_ar1_relu_s_ofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_ogy, bmath_f3_op_ar1_relu_s_ogy );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aofx, bmath_f3_op_ar1_relu_s_aofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aogy, bmath_f3_op_ar1_relu_s_aogy );
            BCORE_REGISTER_OBJECT( bmath_f3_op_ar1_relu_s );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_fx, bmath_f3_op_ar1_relu_leaky_s_fx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_gy, bmath_f3_op_ar1_relu_leaky_s_gy );
            BCORE_REGISTER_FFUNC( bmath_f3_op_get_arity, bmath_f3_op_ar1_relu_leaky_s_get_arity );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_ofx, bmath_f3_op_ar1_relu_leaky_s_ofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar1_ogy, bmath_f3_op_ar1_relu_leaky_s_ogy );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aofx, bmath_f3_op_ar1_relu_leaky_s_aofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aogy, bmath_f3_op_ar1_relu_leaky_s_aogy );
            BCORE_REGISTER_OBJECT( bmath_f3_op_ar1_relu_leaky_s );
            BCORE_REGISTER_SPECT( bmath_f3_op_ar1 );

            // group: bmath_f3_op_ar2
            BCORE_REGISTER_FEATURE( bmath_f3_op_ar2_ofx );
            BCORE_REGISTER_FEATURE( bmath_f3_op_ar2_ogxa );
            BCORE_REGISTER_FEATURE( bmath_f3_op_ar2_ogxb );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar2_fx, bmath_f3_op_ar2_add_s_fx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar2_gxa, bmath_f3_op_ar2_add_s_gxa );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar2_gxb, bmath_f3_op_ar2_add_s_gxb );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar2_ofx, bmath_f3_op_ar2_add_s_ofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar2_ogxa, bmath_f3_op_ar2_add_s_ogxa );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar2_ogxb, bmath_f3_op_ar2_add_s_ogxb );
            BCORE_REGISTER_FFUNC( bmath_f3_op_get_arity, bmath_f3_op_ar2_add_s_get_arity );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aofx, bmath_f3_op_ar2_add_s_aofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aogx, bmath_f3_op_ar2_add_s_aogx );
            BCORE_REGISTER_OBJECT( bmath_f3_op_ar2_add_s );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar2_fx, bmath_f3_op_ar2_sub_s_fx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar2_gxa, bmath_f3_op_ar2_sub_s_gxa );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar2_gxb, bmath_f3_op_ar2_sub_s_gxb );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar2_ofx, bmath_f3_op_ar2_sub_s_ofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar2_ogxa, bmath_f3_op_ar2_sub_s_ogxa );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar2_ogxb, bmath_f3_op_ar2_sub_s_ogxb );
            BCORE_REGISTER_FFUNC( bmath_f3_op_get_arity, bmath_f3_op_ar2_sub_s_get_arity );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aofx, bmath_f3_op_ar2_sub_s_aofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aogx, bmath_f3_op_ar2_sub_s_aogx );
            BCORE_REGISTER_OBJECT( bmath_f3_op_ar2_sub_s );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar2_fx, bmath_f3_op_ar2_mul_s_fx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar2_gxa, bmath_f3_op_ar2_mul_s_gxa );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar2_gxb, bmath_f3_op_ar2_mul_s_gxb );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar2_ofx, bmath_f3_op_ar2_mul_s_ofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar2_ogxa, bmath_f3_op_ar2_mul_s_ogxa );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar2_ogxb, bmath_f3_op_ar2_mul_s_ogxb );
            BCORE_REGISTER_FFUNC( bmath_f3_op_get_arity, bmath_f3_op_ar2_mul_s_get_arity );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aofx, bmath_f3_op_ar2_mul_s_aofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aogx, bmath_f3_op_ar2_mul_s_aogx );
            BCORE_REGISTER_OBJECT( bmath_f3_op_ar2_mul_s );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar2_fx, bmath_f3_op_ar2_div_s_fx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar2_gxa, bmath_f3_op_ar2_div_s_gxa );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar2_gxb, bmath_f3_op_ar2_div_s_gxb );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar2_ofx, bmath_f3_op_ar2_div_s_ofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar2_ogxa, bmath_f3_op_ar2_div_s_ogxa );
            BCORE_REGISTER_FFUNC( bmath_f3_op_ar2_ogxb, bmath_f3_op_ar2_div_s_ogxb );
            BCORE_REGISTER_FFUNC( bmath_f3_op_get_arity, bmath_f3_op_ar2_div_s_get_arity );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aofx, bmath_f3_op_ar2_div_s_aofx );
            BCORE_REGISTER_FFUNC( bmath_f3_op_aogx, bmath_f3_op_ar2_div_s_aogx );
            BCORE_REGISTER_OBJECT( bmath_f3_op_ar2_div_s );
            BCORE_REGISTER_SPECT( bmath_f3_op_ar2 );

            // --------------------------------------------------------------------
            // source: bmath_xsmf2.h

            // group: bmath_xsmf2
            BCORE_REGISTER_FFUNC( bcore_fp_copy_typed, bmath_xsmf2_s_copy_typed );
            BCORE_REGISTER_OBJECT( bmath_xsmf2_s );
            BCORE_REGISTER_NAME( bmath_fp_xsmf2_s_mul );
            BCORE_REGISTER_NAME( bmath_fp_xsmf2_s_mul_htp );
            BCORE_REGISTER_NAME( bmath_fp_xsmf2_s_htp_mul );
            BCORE_REGISTER_NAME( bmath_fp_xsmf2_s_htp_mul_htp );
            BCORE_REGISTER_TRAIT( bmath_xsmf2, bcore_inst );

            // --------------------------------------------------------------------
            // source: bmath_xsmf3.h

            // group: bmath_xsmf3
            BCORE_REGISTER_FFUNC( bcore_fp_copy_typed, bmath_xsmf3_s_copy_typed );
            BCORE_REGISTER_OBJECT( bmath_xsmf3_s );
            BCORE_REGISTER_NAME( bmath_fp_xsmf3_s_mul );
            BCORE_REGISTER_NAME( bmath_fp_xsmf3_s_mul_htp );
            BCORE_REGISTER_NAME( bmath_fp_xsmf3_s_htp_mul );
            BCORE_REGISTER_NAME( bmath_fp_xsmf3_s_htp_mul_htp );
            BCORE_REGISTER_TRAIT( bmath_xsmf3, bcore_inst );

            // --------------------------------------------------------------------
            // source: bmath_asmf2.h

            // group: bmath_asmf2
            BCORE_REGISTER_FFUNC( bcore_fp_copy_typed, bmath_asmf2_s_copy_typed );
            BCORE_REGISTER_OBJECT( bmath_asmf2_s );
            BCORE_REGISTER_NAME( bmath_fp_asmf2_s_mul );
            BCORE_REGISTER_NAME( bmath_fp_asmf2_s_mul_htp );
            BCORE_REGISTER_NAME( bmath_fp_asmf2_s_htp_mul );
            BCORE_REGISTER_NAME( bmath_fp_asmf2_s_htp_mul_htp );
            BCORE_REGISTER_TRAIT( bmath_asmf2, bcore_inst );

            // --------------------------------------------------------------------
            // source: bmath_asmf3.h

            // group: bmath_asmf3
            BCORE_REGISTER_FFUNC( bcore_fp_copy_typed, bmath_asmf3_s_copy_typed );
            BCORE_REGISTER_OBJECT( bmath_asmf3_s );
            BCORE_REGISTER_NAME( bmath_fp_asmf3_s_mul );
            BCORE_REGISTER_NAME( bmath_fp_asmf3_s_mul_htp );
            BCORE_REGISTER_NAME( bmath_fp_asmf3_s_htp_mul );
            BCORE_REGISTER_NAME( bmath_fp_asmf3_s_htp_mul_htp );
            BCORE_REGISTER_TRAIT( bmath_asmf3, bcore_inst );
        }
        break;
        default: break;
    }
    return NULL;
}
