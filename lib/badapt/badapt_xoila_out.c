/** This file was generated from beth-plant source code.
 *  Compiling Agent : xoico_compiler (C) 2020 J.B.Steffens
 *  Last File Update: 2020-08-07T21:18:46Z
 *
 *  Copyright and License of this File:
 *
 *  Generated code inherits the copyright and license of the underlying beth-plant source code.
 *  Source code defining this file is distributed across following files:
 *
 *  badapt_adaptive.h
 *  badapt_problem.h
 *  badapt_activator.h
 *  badapt_loss.h
 *  badapt_mlp.h
 *  badapt_c1d.h
 *  badapt_ern.h
 *  badapt_lstm.h
 *  badapt_training.h
 *  badapt_trainer.h
 *
 */

#include "badapt_xoila_out.h"
#include "bcore_spect.h"
#include "bcore_spect_inst.h"
#include "bcore_sr.h"
#include "bcore_const_manager.h"


/**********************************************************************************************************************/
// source: badapt_adaptive.h
#include "badapt_adaptive.h"

//----------------------------------------------------------------------------------------------------------------------
// group: badapt_dynamics

BCORE_DEFINE_OBJECT_INST_P( badapt_dynamics_std_s )
"aware badapt_dynamics"
"{"
    "f3_t epsilon;"
    "f3_t lambda_l1;"
    "f3_t lambda_l2;"
    "func ^:weights_adapt;"
"}";

BCORE_DEFINE_SPECT( bcore_inst, badapt_dynamics )
"{"
    "bcore_spect_header_s header;"
    "feature strict aware badapt_dynamics : weights_adapt;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: badapt_adaptive

BCORE_DEFINE_SPECT( bcore_inst, badapt_adaptive )
"{"
    "bcore_spect_header_s header;"
    "feature strict aware badapt_adaptive : get_in_size;"
    "feature strict aware badapt_adaptive : get_out_size;"
    "feature aware badapt_adaptive : infer;"
    "feature strict aware badapt_adaptive : minfer;"
    "feature aware badapt_adaptive : bgrad;"
    "feature strict aware badapt_adaptive : bgrad_adapt;"
    "feature aware badapt_adaptive : reset;"
    "feature aware badapt_adaptive : get_weights_min_max;"
    "feature aware badapt_adaptive : get_dynamics_std;"
    "feature aware badapt_adaptive : set_dynamics_std;"
    "feature aware badapt_adaptive : arc_to_sink = badapt_adaptive_arc_to_sink__;"
    "feature aware badapt_adaptive : infer_f3 = badapt_adaptive_infer_f3__;"
    "feature aware badapt_adaptive : adapt_loss = badapt_adaptive_adapt_loss__;"
    "feature aware badapt_adaptive : adapt_loss_f3 = badapt_adaptive_adapt_loss_f3__;"
"}";


f3_t badapt_adaptive_infer_f3__( const badapt_adaptive* o, const bmath_vf3_s* in )
{
    bmath_vf3_s v_out;
    f3_t out = 0;
    bmath_vf3_s_init_weak( &v_out, &out, 1 );
    badapt_adaptive_a_infer( o, in, &v_out );
    return out;
}

void badapt_adaptive_adapt_loss__( badapt_adaptive* o, const badapt_loss* loss, const bmath_vf3_s* in, const bmath_vf3_s* target, bmath_vf3_s* out )
{
    ASSERT( out != NULL );
    bmath_vf3_s* grad = bmath_vf3_s_create();
    bmath_vf3_s_set_size( grad, out->size );
    badapt_adaptive_a_minfer( o, in, out );
    badapt_loss_a_bgrad( loss, out, target, grad );
    badapt_adaptive_a_bgrad_adapt( o, NULL, grad );
    bmath_vf3_s_discard( grad );
}

f3_t badapt_adaptive_adapt_loss_f3__( badapt_adaptive* o, const badapt_loss* loss, const bmath_vf3_s* in, f3_t target )
{
    bmath_vf3_s v_target, v_out;
    f3_t out = 0;
    bmath_vf3_s_init_weak( &v_target, &target, 1 );
    bmath_vf3_s_init_weak( &v_out, &out, 1 );
    badapt_adaptive_a_adapt_loss( o, loss, in, &v_target, &v_out );
    return out;
}
//----------------------------------------------------------------------------------------------------------------------
// group: badapt_builder

BCORE_DEFINE_SPECT( bcore_inst, badapt_builder )
"{"
    "bcore_spect_header_s header;"
    "feature strict aware badapt_builder : get_in_size;"
    "feature strict aware badapt_builder : set_in_size;"
    "feature strict aware badapt_builder : get_out_size;"
    "feature strict aware badapt_builder : set_out_size;"
    "feature strict aware badapt_builder : build;"
"}";

/**********************************************************************************************************************/
// source: badapt_problem.h
#include "badapt_problem.h"

//----------------------------------------------------------------------------------------------------------------------
// group: badapt_problem

BCORE_DEFINE_OBJECT_INST_P( badapt_problem_sine_random_s )
"aware badapt_supplier"
"{"
    "sz_t input_size = 32;"
    "u3_t rval = 1234;"
    "f3_t pos_tgt = 0.9;"
    "f3_t neg_tgt = -0.9;"
    "aware badapt_loss* preferred_loss = badapt_loss_l2_s;"
    "func ^:preferred_loss;"
    "func ^:get_in_size;"
    "func ^:get_out_size;"
    "func ^:fetch_sample_tio;"
    "func ^:fetch_sample_vio;"
"}";

BCORE_DEFINE_OBJECT_INST_P( badapt_problem_binary_add_s )
"aware badapt_supplier"
"{"
    "sz_t bits = 4;"
    "u3_t rval = 1234;"
    "f3_t val_h = 0.9;"
    "f3_t val_l = -0.9;"
    "aware badapt_loss* preferred_loss = badapt_loss_l2_s;"
    "func ^:preferred_loss;"
    "func ^:get_in_size;"
    "func ^:get_out_size;"
    "func ^:fetch_sample_tio;"
    "func ^:fetch_sample_vio;"
"}";

BCORE_DEFINE_OBJECT_INST_P( badapt_problem_binary_mul_s )
"aware badapt_supplier"
"{"
    "sz_t bits = 4;"
    "u3_t rval = 1234;"
    "f3_t val_h = 0.9;"
    "f3_t val_l = -0.9;"
    "aware badapt_loss* preferred_loss = badapt_loss_l2_s;"
    "func ^:preferred_loss;"
    "func ^:get_in_size;"
    "func ^:get_out_size;"
    "func ^:fetch_sample_tio;"
    "func ^:fetch_sample_vio;"
"}";

BCORE_DEFINE_OBJECT_INST_P( badapt_problem_binary_xsg3_s )
"aware badapt_supplier"
"{"
    "sz_t bits = 4;"
    "u3_t rval = 1234;"
    "f3_t val_h = 0.9;"
    "f3_t val_l = -0.9;"
    "aware badapt_loss* preferred_loss = badapt_loss_l2_s;"
    "func ^:preferred_loss;"
    "func ^:get_in_size;"
    "func ^:get_out_size;"
    "func ^:fetch_sample_tio;"
    "func ^:fetch_sample_vio;"
"}";

BCORE_DEFINE_OBJECT_INST_P( badapt_problem_binary_hash_s )
"aware badapt_supplier"
"{"
    "sz_t bits = 4;"
    "u3_t rval = 1234;"
    "f3_t val_h = 0.9;"
    "f3_t val_l = -0.9;"
    "bl_t reverse = false;"
    "aware badapt_loss* preferred_loss = badapt_loss_l2_s;"
    "func ^:preferred_loss;"
    "func ^:get_in_size;"
    "func ^:get_out_size;"
    "func ^:fetch_sample_tio;"
    "func ^:fetch_sample_vio;"
"}";

BCORE_DEFINE_OBJECT_INST_P( badapt_problem_polynom_s )
"aware badapt_supplier"
"{"
    "sz_t input_size = 32;"
    "sz_t output_size = 3;"
    "f3_t range = 1.0;"
    "u3_t rval = 1234;"
    "f3_t noise_level = 0;"
    "aware badapt_loss* preferred_loss = badapt_loss_l2_s;"
    "func ^:preferred_loss;"
    "func ^:get_in_size;"
    "func ^:get_out_size;"
    "func ^:fetch_sample_tio;"
    "func ^:fetch_sample_vio;"
"}";

/**********************************************************************************************************************/
// source: badapt_activator.h
#include "badapt_activator.h"

//----------------------------------------------------------------------------------------------------------------------
// group: badapt_activation

BCORE_DEFINE_OBJECT_INST_P( badapt_activation_zero_s )
"aware badapt_activation"
"{"
    "func ^:fx;"
    "func ^:dy;"
"}";

BCORE_DEFINE_OBJECT_INST_P( badapt_activation_one_s )
"aware badapt_activation"
"{"
    "func ^:fx;"
    "func ^:dy;"
"}";

BCORE_DEFINE_OBJECT_INST_P( badapt_activation_linear_s )
"aware badapt_activation"
"{"
    "func ^:fx;"
    "func ^:dy;"
"}";

BCORE_DEFINE_OBJECT_INST_P( badapt_activation_sigm_s )
"aware badapt_activation"
"{"
    "func ^:fx;"
    "func ^:dy;"
"}";

BCORE_DEFINE_OBJECT_INST_P( badapt_activation_sigm_hard_s )
"aware badapt_activation"
"{"
    "func ^:fx;"
    "func ^:dy;"
"}";

BCORE_DEFINE_OBJECT_INST_P( badapt_activation_sigm_leaky_s )
"aware badapt_activation"
"{"
    "func ^:fx;"
    "func ^:dy;"
"}";

BCORE_DEFINE_OBJECT_INST_P( badapt_activation_tanh_s )
"aware badapt_activation"
"{"
    "func ^:fx;"
    "func ^:dy;"
"}";

BCORE_DEFINE_OBJECT_INST_P( badapt_activation_tanh_hard_s )
"aware badapt_activation"
"{"
    "func ^:fx;"
    "func ^:dy;"
"}";

BCORE_DEFINE_OBJECT_INST_P( badapt_activation_tanh_leaky_s )
"aware badapt_activation"
"{"
    "func ^:fx;"
    "func ^:dy;"
"}";

BCORE_DEFINE_OBJECT_INST_P( badapt_activation_softplus_s )
"aware badapt_activation"
"{"
    "func ^:fx;"
    "func ^:dy;"
"}";

BCORE_DEFINE_OBJECT_INST_P( badapt_activation_relu_s )
"aware badapt_activation"
"{"
    "func ^:fx;"
    "func ^:dy;"
"}";

BCORE_DEFINE_OBJECT_INST_P( badapt_activation_leaky_relu_s )
"aware badapt_activation"
"{"
    "func ^:fx;"
    "func ^:dy;"
"}";

BCORE_DEFINE_SPECT( bcore_inst, badapt_activation )
"{"
    "bcore_spect_header_s header;"
    "feature strict aware badapt_activation : fx;"
    "feature strict aware badapt_activation : dy;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: badapt_activator

BCORE_DEFINE_OBJECT_INST_P( badapt_activator_plain_s )
"aware badapt_activator"
"{"
    "aware badapt_activation => activation;"
    "func ^:infer;"
    "func ^:bgrad;"
    "func ^:set_activation;"
    "func ^:get_activation;"
"}";

void badapt_activator_plain_s_infer( const badapt_activator_plain_s* o, const bmath_vf3_s* in, bmath_vf3_s* out )
{
    assert( in->size == out->size );
    const badapt_activation_s* activation_p = badapt_activation_s_get_aware( o->activation );
    for( sz_t i = 0; i < out->size; i++ )
    {
        out->data[ i ] = badapt_activation_p_fx( activation_p, o->activation, in->data[ i ] );
    }
}

void badapt_activator_plain_s_bgrad( const badapt_activator_plain_s* o, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out, const bmath_vf3_s* out )
{
    assert( grad_in->size == grad_out->size );
    assert( grad_in->size ==      out->size );
    const badapt_activation_s* activation_p = badapt_activation_s_get_aware( o->activation );
    for( sz_t i = 0; i < out->size; i++ )
    {
        grad_in->data[ i ] = badapt_activation_p_dy( activation_p, o->activation, out->data[ i ] ) * grad_out->data[ i ];
    }
}

BCORE_DEFINE_OBJECT_INST_P( badapt_activator_softmax_s )
"aware badapt_activator"
"{"
    "func ^:infer;"
    "func ^:bgrad;"
"}";

void badapt_activator_softmax_s_infer( const badapt_activator_softmax_s* o, const bmath_vf3_s* in, bmath_vf3_s* out )
{
    f3_t max = bmath_vf3_s_max( in );
    f3_t sum = 0;
    for( sz_t i = 0; i < out->size; i++ )
    {
        f3_t v = exp( in->data[ i ] - max );
        sum += v;
        out->data[ i ] = v;
    }
    bmath_vf3_s_mul_f3( out, 1.0 / sum, out );
}

void badapt_activator_softmax_s_bgrad( const badapt_activator_softmax_s* o, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out, const bmath_vf3_s* out )
{
    f3_t dpd = bmath_vf3_s_f3_mul_vec( grad_out, out );
    for( sz_t i = 0; i < grad_in->size; i++ )
    {
        grad_in->data[ i ] = out->data[ i ] * ( grad_out->data[ i ] - dpd );
    }
}

BCORE_DEFINE_OBJECT_INST_P( badapt_layer_activator_s )
"aware bcore_inst"
"{"
    "sz_t layer;"
    "aware badapt_activator => activator;"
"}";

BCORE_DEFINE_OBJECT_INST_P( badapt_arr_layer_activator_s )
"aware bcore_array"
"{"
    "badapt_layer_activator_s [] arr;"
"}";

BCORE_DEFINE_OBJECT_INST_P( badapt_arr_activator_s )
"aware bcore_array"
"{"
    "aware badapt_activator => [] arr;"
"}";

BCORE_DEFINE_SPECT( bcore_inst, badapt_activator )
"{"
    "bcore_spect_header_s header;"
    "feature aware badapt_activator : get_activation = badapt_activator_get_activation__;"
    "feature aware badapt_activator : set_activation = badapt_activator_set_activation__;"
    "feature strict aware badapt_activator : infer;"
    "feature strict aware badapt_activator : bgrad;"
"}";

/**********************************************************************************************************************/
// source: badapt_loss.h
#include "badapt_loss.h"

//----------------------------------------------------------------------------------------------------------------------
// group: badapt_loss

BCORE_DEFINE_OBJECT_INST_P( badapt_loss_l2_s )
"aware badapt_loss"
"{"
    "func ^:loss;"
    "func ^:loss_f3;"
    "func ^:bgrad;"
"}";

BCORE_DEFINE_OBJECT_INST_P( badapt_loss_log_s )
"aware badapt_loss"
"{"
    "func ^:loss;"
    "func ^:loss_f3;"
    "func ^:bgrad;"
"}";

f3_t badapt_loss_log_s_loss( const badapt_loss_log_s* o, const bmath_vf3_s* out, const bmath_vf3_s* target )
{
    assert( target->size == out->size );
    f3_t sum = 0;
    for( sz_t i = 0; i < target->size; i++ )
    {
        sum += log( 1.0 + exp( -target->data[ i ] * out->data[ i ] ) );
    }
    return sum;
}

f3_t badapt_loss_log_s_loss_f3( const badapt_loss_log_s* o, f3_t out, f3_t target )
{
    return log( 1.0 + exp( -target * out ) );
}

void badapt_loss_log_s_bgrad( const badapt_loss_log_s* o, const bmath_vf3_s* out, const bmath_vf3_s* target, bmath_vf3_s* grad )
{
    assert( target->size == out ->size );
    assert( target->size == grad->size );
    for( sz_t i = 0; i < target->size; i++ )
    {
        f3_t v_t = target->data[ i ];
        f3_t v_o = out->data[ i ];
        grad->data[ i ] = v_t / ( 1.0 + exp( v_t * v_o ) );
    }
}

BCORE_DEFINE_SPECT( bcore_inst, badapt_loss )
"{"
    "bcore_spect_header_s header;"
    "feature strict aware badapt_loss : loss;"
    "feature strict aware badapt_loss : loss_f3;"
    "feature strict aware badapt_loss : bgrad;"
"}";

/**********************************************************************************************************************/
// source: badapt_mlp.h
#include "badapt_mlp.h"

//----------------------------------------------------------------------------------------------------------------------
// group: badapt_mlp

BCORE_DEFINE_OBJECT_INST_P( badapt_mlp_layer_s )
"aware bcore_inst"
"{"
    "sz_t input_size;"
    "sz_t kernels;"
    "bmath_mf3_s w;"
    "bmath_vf3_s b;"
    "aware badapt_activator => a;"
    "hidden bmath_vf3_s o;"
"}";

BCORE_DEFINE_OBJECT_INST_P( badapt_mlp_arr_layer_s )
"aware bcore_array"
"{"
    "badapt_mlp_layer_s [] arr;"
"}";

BCORE_DEFINE_OBJECT_INST_P( badapt_mlp_s )
"aware badapt_adaptive"
"{"
    "badapt_dynamics_std_s dynamics;"
    "badapt_mlp_arr_layer_s arr_layer;"
    "sz_t max_buffer_size;"
    "hidden bmath_vf3_s in;"
    "func ^:get_in_size;"
    "func ^:get_out_size;"
    "func ^:get_dynamics_std;"
    "func ^:set_dynamics_std;"
    "func ^:arc_to_sink;"
    "func ^:infer;"
    "func ^:minfer;"
    "func ^:bgrad;"
    "func ^:bgrad_adapt;"
"}";

BCORE_DEFINE_OBJECT_INST_P( badapt_builder_mlp_funnel_s )
"aware badapt_builder"
"{"
    "sz_t input_size;"
    "sz_t input_kernels = 8;"
    "sz_t output_kernels = 1;"
    "sz_t layers = 2;"
    "f3_t kernels_rate = 0;"
    "u2_t random_seed = 1234;"
    "badapt_dynamics_std_s dynamics;"
    "badapt_arr_layer_activator_s arr_layer_activator;"
    "func ^:get_in_size;"
    "func ^:set_in_size;"
    "func ^:get_out_size;"
    "func ^:set_out_size;"
    "func ^:build;"
"}";

/**********************************************************************************************************************/
// source: badapt_c1d.h
#include "badapt_c1d.h"

//----------------------------------------------------------------------------------------------------------------------
// group: badapt_c1d

BCORE_DEFINE_OBJECT_INST_P( badapt_c1d_layer_s )
"bcore_inst"
"{"
    "aware_t _;"
    "sz_t input_size;"
    "sz_t stride;"
    "sz_t steps;"
    "sz_t kernel_size;"
    "sz_t kernels;"
    "bmath_mf3_s w;"
    "bmath_vf3_s b;"
    "aware badapt_activator => a;"
    "hidden bmath_vf3_s o;"
"}";

BCORE_DEFINE_OBJECT_INST_P( badapt_c1d_arr_layer_s )
"aware bcore_array"
"{"
    "badapt_c1d_layer_s [] arr;"
"}";

BCORE_DEFINE_OBJECT_INST_P( badapt_c1d_s )
"badapt_adaptive"
"{"
    "aware_t _;"
    "badapt_dynamics_std_s dynamics;"
    "badapt_c1d_arr_layer_s arr_layer;"
    "sz_t max_buffer_size;"
    "hidden bmath_vf3_s in;"
    "func ^:get_in_size;"
    "func ^:get_out_size;"
    "func ^:get_dynamics_std;"
    "func ^:set_dynamics_std;"
    "func ^:arc_to_sink;"
    "func ^:infer;"
    "func ^:minfer;"
    "func ^:bgrad;"
    "func ^:bgrad_adapt;"
"}";

BCORE_DEFINE_OBJECT_INST_P( badapt_builder_c1d_funnel_s )
"badapt_builder"
"{"
    "aware_t _;"
    "sz_t input_size;"
    "sz_t input_step = 1;"
    "sz_t input_convolution_size = 2;"
    "sz_t input_kernels = 8;"
    "sz_t output_kernels = 1;"
    "f3_t kernels_rate = 0;"
    "sz_t reduction_step = 2;"
    "sz_t convolution_size = 2;"
    "u2_t random_seed = 1234;"
    "badapt_dynamics_std_s dynamics;"
    "badapt_arr_layer_activator_s arr_layer_activator;"
    "func ^:get_in_size;"
    "func ^:set_in_size;"
    "func ^:get_out_size;"
    "func ^:set_out_size;"
    "func ^:build;"
"}";

/**********************************************************************************************************************/
// source: badapt_ern.h
#include "badapt_ern.h"

//----------------------------------------------------------------------------------------------------------------------
// group: badapt_ern

BCORE_DEFINE_OBJECT_INST_P( badapt_ern_layer_s )
"bcore_inst"
"{"
    "hidden bmath_vf3_s v_x;"
    "hidden bmath_vf3_s v_c;"
    "hidden bmath_vf3_s v_h;"
"}";

BCORE_DEFINE_OBJECT_INST_P( badapt_ern_arr_layer_s )
"aware bcore_array"
"{"
    "badapt_ern_layer_s => [] arr;"
"}";

BCORE_DEFINE_OBJECT_INST_P( badapt_ern_s )
"aware badapt_adaptive"
"{"
    "sz_t size_input;"
    "sz_t size_hidden;"
    "sz_t size_output;"
    "sz_t size_unfolded;"
    "badapt_dynamics_std_s dynamics;"
    "bmath_mf3_s w_hx;"
    "bmath_mf3_s w_hc;"
    "bmath_mf3_s w_oh;"
    "bmath_vf3_s b_h;"
    "bmath_vf3_s b_o;"
    "aware badapt_activator => a_h;"
    "aware badapt_activator => a_o;"
    "hidden bmath_vf3_s v_o;"
    "hidden bmath_vf3_s d_v_o;"
    "hidden bmath_vf3_s d_v_c;"
    "hidden bmath_vf3_s d_v_h;"
    "hidden bmath_mf3_s d_w_hx;"
    "hidden bmath_mf3_s d_w_hc;"
    "hidden bmath_vf3_s d_b_h;"
    "hidden badapt_ern_arr_layer_s arr_layer;"
    "func ^:get_in_size;"
    "func ^:get_out_size;"
    "func ^:get_dynamics_std;"
    "func ^:set_dynamics_std;"
    "func ^:arc_to_sink;"
    "func ^:minfer;"
    "func ^:bgrad_adapt;"
    "func ^:get_weights_min_max;"
"}";

BCORE_DEFINE_OBJECT_INST_P( badapt_ern_builder_s )
"aware badapt_builder"
"{"
    "sz_t size_input;"
    "sz_t size_hidden = 8;"
    "sz_t size_output = 1;"
    "sz_t size_unfolded = 1;"
    "badapt_dynamics_std_s dynamics;"
    "u2_t random_seed = 1234;"
    "aware badapt_activator => a_h;"
    "aware badapt_activator => a_o;"
    "func ^:get_in_size;"
    "func ^:set_in_size;"
    "func ^:get_out_size;"
    "func ^:set_out_size;"
    "func ^:build;"
"}";

/**********************************************************************************************************************/
// source: badapt_lstm.h
#include "badapt_lstm.h"

//----------------------------------------------------------------------------------------------------------------------
// group: badapt_lstm

BCORE_DEFINE_OBJECT_INST_P( badapt_lstm_layer_s )
"bcore_inst"
"{"
    "bmath_vf3_s v_x;"
    "bmath_vf3_s v_f;"
    "bmath_vf3_s v_i;"
    "bmath_vf3_s v_o;"
    "bmath_vf3_s v_q;"
    "bmath_vf3_s v_c;"
    "bmath_vf3_s v_d;"
    "bmath_vf3_s v_h;"
"}";

BCORE_DEFINE_OBJECT_INST_P( badapt_lstm_arr_layer_s )
"aware bcore_array"
"{"
    "badapt_lstm_layer_s => [] arr;"
"}";

BCORE_DEFINE_OBJECT_INST_P( badapt_lstm_s )
"aware badapt_adaptive"
"{"
    "sz_t size_input;"
    "sz_t size_hidden;"
    "sz_t size_output;"
    "sz_t size_unfolded;"
    "badapt_dynamics_std_s dynamics;"
    "bmath_mf3_s w_fx;"
    "bmath_mf3_s w_fh;"
    "bmath_mf3_s w_ix;"
    "bmath_mf3_s w_ih;"
    "bmath_mf3_s w_ox;"
    "bmath_mf3_s w_oh;"
    "bmath_mf3_s w_qx;"
    "bmath_mf3_s w_qh;"
    "bmath_mf3_s w_rh;"
    "bmath_vf3_s b_f;"
    "bmath_vf3_s b_i;"
    "bmath_vf3_s b_o;"
    "bmath_vf3_s b_q;"
    "bmath_vf3_s b_r;"
    "aware badapt_activator => a_f;"
    "aware badapt_activator => a_i;"
    "aware badapt_activator => a_o;"
    "aware badapt_activator => a_q;"
    "aware badapt_activator => a_d;"
    "aware badapt_activator => a_r;"
    "hidden bmath_vf3_s v_r;"
    "hidden bmath_vf3_s d_v_r;"
    "hidden bmath_vf3_s d_v_f;"
    "hidden bmath_vf3_s d_v_i;"
    "hidden bmath_vf3_s d_v_o;"
    "hidden bmath_vf3_s d_v_q;"
    "hidden bmath_vf3_s d_v_c;"
    "hidden bmath_vf3_s d_v_d;"
    "hidden bmath_vf3_s d_v_h;"
    "hidden bmath_mf3_s d_w_fx;"
    "hidden bmath_mf3_s d_w_fh;"
    "hidden bmath_mf3_s d_w_ix;"
    "hidden bmath_mf3_s d_w_ih;"
    "hidden bmath_mf3_s d_w_ox;"
    "hidden bmath_mf3_s d_w_oh;"
    "hidden bmath_mf3_s d_w_qx;"
    "hidden bmath_mf3_s d_w_qh;"
    "hidden bmath_vf3_s d_b_f;"
    "hidden bmath_vf3_s d_b_i;"
    "hidden bmath_vf3_s d_b_o;"
    "hidden bmath_vf3_s d_b_q;"
    "hidden badapt_lstm_arr_layer_s arr_layer;"
    "func ^:get_in_size;"
    "func ^:get_out_size;"
    "func ^:get_dynamics_std;"
    "func ^:set_dynamics_std;"
    "func ^:arc_to_sink;"
    "func ^:minfer;"
    "func ^:bgrad_adapt;"
    "func ^:reset;"
    "func ^:get_weights_min_max;"
"}";

BCORE_DEFINE_OBJECT_INST_P( badapt_lstm_builder_s )
"aware badapt_builder"
"{"
    "sz_t size_input;"
    "sz_t size_hidden = 8;"
    "sz_t size_output = 1;"
    "sz_t size_unfolded = 2;"
    "badapt_dynamics_std_s dynamics;"
    "u2_t random_seed = 1234;"
    "f3_t random_range = 0.5;"
    "aware badapt_activator => a_f;"
    "aware badapt_activator => a_i;"
    "aware badapt_activator => a_o;"
    "aware badapt_activator => a_q;"
    "aware badapt_activator => a_d;"
    "aware badapt_activator => a_r;"
    "func bcore_inst_call:init_x;"
    "func ^:get_in_size;"
    "func ^:set_in_size;"
    "func ^:get_out_size;"
    "func ^:set_out_size;"
    "func ^:build;"
"}";

/**********************************************************************************************************************/
// source: badapt_training.h
#include "badapt_training.h"

//----------------------------------------------------------------------------------------------------------------------
// group: badapt_supplier

BCORE_DEFINE_SPECT( bcore_inst, badapt_supplier )
"{"
    "bcore_spect_header_s header;"
    "feature strict aware badapt_supplier : get_in_size;"
    "feature strict aware badapt_supplier : get_out_size;"
    "feature aware badapt_supplier : fetch_sample_tin;"
    "feature aware badapt_supplier : fetch_sample_tio;"
    "feature aware badapt_supplier : fetch_sample_vin;"
    "feature aware badapt_supplier : fetch_sample_vio;"
    "feature aware badapt_supplier : preferred_loss;"
    "feature aware badapt_supplier : setup_builder = badapt_supplier_setup_builder_default;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: badapt_guide

BCORE_DEFINE_SPECT( bcore_inst, badapt_guide )
"{"
    "bcore_spect_header_s header;"
    "feature strict aware badapt_guide : callback;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: badapt_training_state

BCORE_DEFINE_SPECT( bcore_inst, badapt_training_state )
"{"
    "bcore_spect_header_s header;"
    "feature aware badapt_training_state : set_adaptive;"
    "feature aware badapt_training_state : get_adaptive;"
    "feature aware badapt_training_state : set_supplier;"
    "feature aware badapt_training_state : get_supplier;"
    "feature aware badapt_training_state : set_progress;"
    "feature aware badapt_training_state : get_progress;"
    "feature aware badapt_training_state : set_guide;"
    "feature aware badapt_training_state : get_guide;"
    "feature aware badapt_training_state : set_backup_path;"
    "feature aware badapt_training_state : get_backup_path;"
    "feature aware badapt_training_state : backup = badapt_training_state_backup__;"
    "feature aware badapt_training_state : recover = badapt_training_state_recover__;"
"}";


bl_t badapt_training_state_backup__( const badapt_training_state* o )
{
    sc_t path = badapt_training_state_a_get_backup_path( o );
    if( !path[ 0 ] ) return false;
    st_s* tmp = st_s_create_fa( "#<sc_t>.tmp", path );
    bcore_bin_ml_a_to_file( o, tmp->sc );
    bcore_file_rename( tmp->sc, path );
    st_s_discard( tmp );
    return true;
}

bl_t badapt_training_state_recover__( badapt_training_state* o )
{
    sc_t path = badapt_training_state_a_get_backup_path( o );
    if( !path || !path[ 0 ]        ) return false;
    if( !bcore_file_exists( path ) ) return false;
    bcore_bin_ml_a_from_file( o, path );
    return true;
}
//----------------------------------------------------------------------------------------------------------------------
// group: badapt_trainer

BCORE_DEFINE_SPECT( bcore_inst, badapt_trainer )
"{"
    "bcore_spect_header_s header;"
    "feature strict aware badapt_trainer : run;"
    "feature strict aware badapt_trainer : create_state;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: badapt_training_objects

BCORE_DEFINE_OBJECT_INST_P( badapt_progress_s )
"bcore_inst"
"{"
    "sz_t iteration = 0;"
    "f3_t error = 0;"
    "f3_t improved = 0;"
    "f3_t bias = 0;"
"}";

BCORE_DEFINE_OBJECT_INST_P( badapt_training_state_std_s )
"aware badapt_training_state"
"{"
    "badapt_progress_s progress;"
    "aware badapt_adaptive => adaptive;"
    "aware badapt_supplier => supplier;"
    "aware badapt_guide => guide = badapt_guide_std_s;"
    "st_s backup_file_name;"
    "func ^:set_adaptive;"
    "func ^:get_adaptive;"
    "func ^:set_supplier;"
    "func ^:get_supplier;"
    "func ^:set_progress;"
    "func ^:get_progress;"
    "func ^:set_guide;"
    "func ^:get_guide;"
    "func ^:set_backup_path;"
    "func ^:get_backup_path;"
"}";

BCORE_DEFINE_OBJECT_INST_P( badapt_guide_std_s )
"aware badapt_guide"
"{"
    "f3_t annealing_factor = 0.99;"
    "hidden aware bcore_sink -> log;"
    "func bcore_inst_call:init_x;"
    "func ^:callback;"
"}";

/**********************************************************************************************************************/
// source: badapt_trainer.h
#include "badapt_trainer.h"

//----------------------------------------------------------------------------------------------------------------------
// group: badapt_trainer_objects

BCORE_DEFINE_OBJECT_INST_P( badapt_trainer_batch_s )
"aware badapt_trainer"
"{"
    "aware badapt_loss => loss;"
    "sz_t batch_size = 10000;"
    "sz_t batch_cycles_per_fetch = 1;"
    "sz_t fetch_cycles_per_iteration = 4;"
    "sz_t valid_size = 10000;"
    "sz_t max_iterations = 10;"
    "func ^:run;"
    "func ^:create_state;"
"}";

BCORE_DEFINE_OBJECT_INST_P( badapt_sample_batch_s )
"bcore_inst"
"{"
    "bmath_vf3_s in;"
    "bmath_vf3_s out;"
"}";

BCORE_DEFINE_OBJECT_INST_P( badapt_arr_sample_batch_s )
"aware bcore_array"
"{"
    "badapt_sample_batch_s [] arr;"
"}";

BCORE_DEFINE_OBJECT_INST_P( badapt_trainer_main_s )
"aware bcore_main"
"{"
    "aware badapt_supplier => problem;"
    "aware badapt_builder => builder;"
    "aware badapt_guide => guide;"
    "aware badapt_trainer => trainer;"
    "func ^:main;"
"}";

/**********************************************************************************************************************/

vd_t bmath_general_signal_handler( const bcore_signal_s* o );

vd_t badapt_xoila_out_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "badapt_xoila_out" ) ) )
    {
        case TYPEOF_init1:
        {

            // --------------------------------------------------------------------
            // source: badapt_adaptive.h

            // group: badapt_dynamics
            BCORE_REGISTER_FEATURE( badapt_dynamics_weights_adapt );
            BCORE_REGISTER_FFUNC( badapt_dynamics_weights_adapt, badapt_dynamics_std_s_weights_adapt );
            BCORE_REGISTER_OBJECT( badapt_dynamics_std_s );
            BCORE_REGISTER_SPECT( badapt_dynamics );

            // group: badapt_adaptive
            BCORE_REGISTER_FEATURE( badapt_adaptive_get_in_size );
            BCORE_REGISTER_FEATURE( badapt_adaptive_get_out_size );
            BCORE_REGISTER_FEATURE( badapt_adaptive_infer );
            BCORE_REGISTER_FEATURE( badapt_adaptive_minfer );
            BCORE_REGISTER_FEATURE( badapt_adaptive_bgrad );
            BCORE_REGISTER_FEATURE( badapt_adaptive_bgrad_adapt );
            BCORE_REGISTER_FEATURE( badapt_adaptive_reset );
            BCORE_REGISTER_FEATURE( badapt_adaptive_get_weights_min_max );
            BCORE_REGISTER_FEATURE( badapt_adaptive_get_dynamics_std );
            BCORE_REGISTER_FEATURE( badapt_adaptive_set_dynamics_std );
            BCORE_REGISTER_FEATURE( badapt_adaptive_arc_to_sink );
            BCORE_REGISTER_FFUNC( badapt_adaptive_arc_to_sink, badapt_adaptive_arc_to_sink__ );
            BCORE_REGISTER_FEATURE( badapt_adaptive_infer_f3 );
            BCORE_REGISTER_FFUNC( badapt_adaptive_infer_f3, badapt_adaptive_infer_f3__ );
            BCORE_REGISTER_FEATURE( badapt_adaptive_adapt_loss );
            BCORE_REGISTER_FFUNC( badapt_adaptive_adapt_loss, badapt_adaptive_adapt_loss__ );
            BCORE_REGISTER_FEATURE( badapt_adaptive_adapt_loss_f3 );
            BCORE_REGISTER_FFUNC( badapt_adaptive_adapt_loss_f3, badapt_adaptive_adapt_loss_f3__ );
            BCORE_REGISTER_SPECT( badapt_adaptive );

            // group: badapt_builder
            BCORE_REGISTER_FEATURE( badapt_builder_get_in_size );
            BCORE_REGISTER_FEATURE( badapt_builder_set_in_size );
            BCORE_REGISTER_FEATURE( badapt_builder_get_out_size );
            BCORE_REGISTER_FEATURE( badapt_builder_set_out_size );
            BCORE_REGISTER_FEATURE( badapt_builder_build );
            BCORE_REGISTER_SPECT( badapt_builder );

            // --------------------------------------------------------------------
            // source: badapt_problem.h

            // group: badapt_problem
            BCORE_REGISTER_FFUNC( badapt_supplier_preferred_loss, badapt_problem_sine_random_s_preferred_loss );
            BCORE_REGISTER_FFUNC( badapt_supplier_get_in_size, badapt_problem_sine_random_s_get_in_size );
            BCORE_REGISTER_FFUNC( badapt_supplier_get_out_size, badapt_problem_sine_random_s_get_out_size );
            BCORE_REGISTER_FFUNC( badapt_supplier_fetch_sample_tio, badapt_problem_sine_random_s_fetch_sample_tio );
            BCORE_REGISTER_FFUNC( badapt_supplier_fetch_sample_vio, badapt_problem_sine_random_s_fetch_sample_vio );
            BCORE_REGISTER_OBJECT( badapt_problem_sine_random_s );
            BCORE_REGISTER_FFUNC( badapt_supplier_preferred_loss, badapt_problem_binary_add_s_preferred_loss );
            BCORE_REGISTER_FFUNC( badapt_supplier_get_in_size, badapt_problem_binary_add_s_get_in_size );
            BCORE_REGISTER_FFUNC( badapt_supplier_get_out_size, badapt_problem_binary_add_s_get_out_size );
            BCORE_REGISTER_FFUNC( badapt_supplier_fetch_sample_tio, badapt_problem_binary_add_s_fetch_sample_tio );
            BCORE_REGISTER_FFUNC( badapt_supplier_fetch_sample_vio, badapt_problem_binary_add_s_fetch_sample_vio );
            BCORE_REGISTER_OBJECT( badapt_problem_binary_add_s );
            BCORE_REGISTER_FFUNC( badapt_supplier_preferred_loss, badapt_problem_binary_mul_s_preferred_loss );
            BCORE_REGISTER_FFUNC( badapt_supplier_get_in_size, badapt_problem_binary_mul_s_get_in_size );
            BCORE_REGISTER_FFUNC( badapt_supplier_get_out_size, badapt_problem_binary_mul_s_get_out_size );
            BCORE_REGISTER_FFUNC( badapt_supplier_fetch_sample_tio, badapt_problem_binary_mul_s_fetch_sample_tio );
            BCORE_REGISTER_FFUNC( badapt_supplier_fetch_sample_vio, badapt_problem_binary_mul_s_fetch_sample_vio );
            BCORE_REGISTER_OBJECT( badapt_problem_binary_mul_s );
            BCORE_REGISTER_FFUNC( badapt_supplier_preferred_loss, badapt_problem_binary_xsg3_s_preferred_loss );
            BCORE_REGISTER_FFUNC( badapt_supplier_get_in_size, badapt_problem_binary_xsg3_s_get_in_size );
            BCORE_REGISTER_FFUNC( badapt_supplier_get_out_size, badapt_problem_binary_xsg3_s_get_out_size );
            BCORE_REGISTER_FFUNC( badapt_supplier_fetch_sample_tio, badapt_problem_binary_xsg3_s_fetch_sample_tio );
            BCORE_REGISTER_FFUNC( badapt_supplier_fetch_sample_vio, badapt_problem_binary_xsg3_s_fetch_sample_vio );
            BCORE_REGISTER_OBJECT( badapt_problem_binary_xsg3_s );
            BCORE_REGISTER_FFUNC( badapt_supplier_preferred_loss, badapt_problem_binary_hash_s_preferred_loss );
            BCORE_REGISTER_FFUNC( badapt_supplier_get_in_size, badapt_problem_binary_hash_s_get_in_size );
            BCORE_REGISTER_FFUNC( badapt_supplier_get_out_size, badapt_problem_binary_hash_s_get_out_size );
            BCORE_REGISTER_FFUNC( badapt_supplier_fetch_sample_tio, badapt_problem_binary_hash_s_fetch_sample_tio );
            BCORE_REGISTER_FFUNC( badapt_supplier_fetch_sample_vio, badapt_problem_binary_hash_s_fetch_sample_vio );
            BCORE_REGISTER_OBJECT( badapt_problem_binary_hash_s );
            BCORE_REGISTER_FFUNC( badapt_supplier_preferred_loss, badapt_problem_polynom_s_preferred_loss );
            BCORE_REGISTER_FFUNC( badapt_supplier_get_in_size, badapt_problem_polynom_s_get_in_size );
            BCORE_REGISTER_FFUNC( badapt_supplier_get_out_size, badapt_problem_polynom_s_get_out_size );
            BCORE_REGISTER_FFUNC( badapt_supplier_fetch_sample_tio, badapt_problem_polynom_s_fetch_sample_tio );
            BCORE_REGISTER_FFUNC( badapt_supplier_fetch_sample_vio, badapt_problem_polynom_s_fetch_sample_vio );
            BCORE_REGISTER_OBJECT( badapt_problem_polynom_s );
            BCORE_REGISTER_TRAIT( badapt_problem, bcore_inst );

            // --------------------------------------------------------------------
            // source: badapt_activator.h

            // group: badapt_activation
            BCORE_REGISTER_FEATURE( badapt_activation_fx );
            BCORE_REGISTER_FEATURE( badapt_activation_dy );
            BCORE_REGISTER_FFUNC( badapt_activation_fx, badapt_activation_zero_s_fx );
            BCORE_REGISTER_FFUNC( badapt_activation_dy, badapt_activation_zero_s_dy );
            BCORE_REGISTER_OBJECT( badapt_activation_zero_s );
            BCORE_REGISTER_FFUNC( badapt_activation_fx, badapt_activation_one_s_fx );
            BCORE_REGISTER_FFUNC( badapt_activation_dy, badapt_activation_one_s_dy );
            BCORE_REGISTER_OBJECT( badapt_activation_one_s );
            BCORE_REGISTER_FFUNC( badapt_activation_fx, badapt_activation_linear_s_fx );
            BCORE_REGISTER_FFUNC( badapt_activation_dy, badapt_activation_linear_s_dy );
            BCORE_REGISTER_OBJECT( badapt_activation_linear_s );
            BCORE_REGISTER_FFUNC( badapt_activation_fx, badapt_activation_sigm_s_fx );
            BCORE_REGISTER_FFUNC( badapt_activation_dy, badapt_activation_sigm_s_dy );
            BCORE_REGISTER_OBJECT( badapt_activation_sigm_s );
            BCORE_REGISTER_FFUNC( badapt_activation_fx, badapt_activation_sigm_hard_s_fx );
            BCORE_REGISTER_FFUNC( badapt_activation_dy, badapt_activation_sigm_hard_s_dy );
            BCORE_REGISTER_OBJECT( badapt_activation_sigm_hard_s );
            BCORE_REGISTER_FFUNC( badapt_activation_fx, badapt_activation_sigm_leaky_s_fx );
            BCORE_REGISTER_FFUNC( badapt_activation_dy, badapt_activation_sigm_leaky_s_dy );
            BCORE_REGISTER_OBJECT( badapt_activation_sigm_leaky_s );
            BCORE_REGISTER_FFUNC( badapt_activation_fx, badapt_activation_tanh_s_fx );
            BCORE_REGISTER_FFUNC( badapt_activation_dy, badapt_activation_tanh_s_dy );
            BCORE_REGISTER_OBJECT( badapt_activation_tanh_s );
            BCORE_REGISTER_FFUNC( badapt_activation_fx, badapt_activation_tanh_hard_s_fx );
            BCORE_REGISTER_FFUNC( badapt_activation_dy, badapt_activation_tanh_hard_s_dy );
            BCORE_REGISTER_OBJECT( badapt_activation_tanh_hard_s );
            BCORE_REGISTER_FFUNC( badapt_activation_fx, badapt_activation_tanh_leaky_s_fx );
            BCORE_REGISTER_FFUNC( badapt_activation_dy, badapt_activation_tanh_leaky_s_dy );
            BCORE_REGISTER_OBJECT( badapt_activation_tanh_leaky_s );
            BCORE_REGISTER_FFUNC( badapt_activation_fx, badapt_activation_softplus_s_fx );
            BCORE_REGISTER_FFUNC( badapt_activation_dy, badapt_activation_softplus_s_dy );
            BCORE_REGISTER_OBJECT( badapt_activation_softplus_s );
            BCORE_REGISTER_FFUNC( badapt_activation_fx, badapt_activation_relu_s_fx );
            BCORE_REGISTER_FFUNC( badapt_activation_dy, badapt_activation_relu_s_dy );
            BCORE_REGISTER_OBJECT( badapt_activation_relu_s );
            BCORE_REGISTER_FFUNC( badapt_activation_fx, badapt_activation_leaky_relu_s_fx );
            BCORE_REGISTER_FFUNC( badapt_activation_dy, badapt_activation_leaky_relu_s_dy );
            BCORE_REGISTER_OBJECT( badapt_activation_leaky_relu_s );
            BCORE_REGISTER_SPECT( badapt_activation );

            // group: badapt_activator
            BCORE_REGISTER_FEATURE( badapt_activator_get_activation );
            BCORE_REGISTER_FFUNC( badapt_activator_get_activation, badapt_activator_get_activation__ );
            BCORE_REGISTER_FEATURE( badapt_activator_set_activation );
            BCORE_REGISTER_FFUNC( badapt_activator_set_activation, badapt_activator_set_activation__ );
            BCORE_REGISTER_FEATURE( badapt_activator_infer );
            BCORE_REGISTER_FEATURE( badapt_activator_bgrad );
            BCORE_REGISTER_FFUNC( badapt_activator_infer, badapt_activator_plain_s_infer );
            BCORE_REGISTER_FFUNC( badapt_activator_bgrad, badapt_activator_plain_s_bgrad );
            BCORE_REGISTER_FFUNC( badapt_activator_set_activation, badapt_activator_plain_s_set_activation );
            BCORE_REGISTER_FFUNC( badapt_activator_get_activation, badapt_activator_plain_s_get_activation );
            BCORE_REGISTER_OBJECT( badapt_activator_plain_s );
            BCORE_REGISTER_FFUNC( badapt_activator_infer, badapt_activator_softmax_s_infer );
            BCORE_REGISTER_FFUNC( badapt_activator_bgrad, badapt_activator_softmax_s_bgrad );
            BCORE_REGISTER_OBJECT( badapt_activator_softmax_s );
            BCORE_REGISTER_OBJECT( badapt_layer_activator_s );
            BCORE_REGISTER_OBJECT( badapt_arr_layer_activator_s );
            BCORE_REGISTER_OBJECT( badapt_arr_activator_s );
            BCORE_REGISTER_SPECT( badapt_activator );

            // --------------------------------------------------------------------
            // source: badapt_loss.h

            // group: badapt_loss
            BCORE_REGISTER_FEATURE( badapt_loss_loss );
            BCORE_REGISTER_FEATURE( badapt_loss_loss_f3 );
            BCORE_REGISTER_FEATURE( badapt_loss_bgrad );
            BCORE_REGISTER_FFUNC( badapt_loss_loss, badapt_loss_l2_s_loss );
            BCORE_REGISTER_FFUNC( badapt_loss_loss_f3, badapt_loss_l2_s_loss_f3 );
            BCORE_REGISTER_FFUNC( badapt_loss_bgrad, badapt_loss_l2_s_bgrad );
            BCORE_REGISTER_OBJECT( badapt_loss_l2_s );
            BCORE_REGISTER_FFUNC( badapt_loss_loss, badapt_loss_log_s_loss );
            BCORE_REGISTER_FFUNC( badapt_loss_loss_f3, badapt_loss_log_s_loss_f3 );
            BCORE_REGISTER_FFUNC( badapt_loss_bgrad, badapt_loss_log_s_bgrad );
            BCORE_REGISTER_OBJECT( badapt_loss_log_s );
            BCORE_REGISTER_SPECT( badapt_loss );

            // --------------------------------------------------------------------
            // source: badapt_mlp.h

            // group: badapt_mlp
            BCORE_REGISTER_OBJECT( badapt_mlp_layer_s );
            BCORE_REGISTER_OBJECT( badapt_mlp_arr_layer_s );
            BCORE_REGISTER_FFUNC( badapt_adaptive_get_in_size, badapt_mlp_s_get_in_size );
            BCORE_REGISTER_FFUNC( badapt_adaptive_get_out_size, badapt_mlp_s_get_out_size );
            BCORE_REGISTER_FFUNC( badapt_adaptive_get_dynamics_std, badapt_mlp_s_get_dynamics_std );
            BCORE_REGISTER_FFUNC( badapt_adaptive_set_dynamics_std, badapt_mlp_s_set_dynamics_std );
            BCORE_REGISTER_FFUNC( badapt_adaptive_arc_to_sink, badapt_mlp_s_arc_to_sink );
            BCORE_REGISTER_FFUNC( badapt_adaptive_infer, badapt_mlp_s_infer );
            BCORE_REGISTER_FFUNC( badapt_adaptive_minfer, badapt_mlp_s_minfer );
            BCORE_REGISTER_FFUNC( badapt_adaptive_bgrad, badapt_mlp_s_bgrad );
            BCORE_REGISTER_FFUNC( badapt_adaptive_bgrad_adapt, badapt_mlp_s_bgrad_adapt );
            BCORE_REGISTER_OBJECT( badapt_mlp_s );
            BCORE_REGISTER_FFUNC( badapt_builder_get_in_size, badapt_builder_mlp_funnel_s_get_in_size );
            BCORE_REGISTER_FFUNC( badapt_builder_set_in_size, badapt_builder_mlp_funnel_s_set_in_size );
            BCORE_REGISTER_FFUNC( badapt_builder_get_out_size, badapt_builder_mlp_funnel_s_get_out_size );
            BCORE_REGISTER_FFUNC( badapt_builder_set_out_size, badapt_builder_mlp_funnel_s_set_out_size );
            BCORE_REGISTER_FFUNC( badapt_builder_build, badapt_builder_mlp_funnel_s_build );
            BCORE_REGISTER_OBJECT( badapt_builder_mlp_funnel_s );
            BCORE_REGISTER_TRAIT( badapt_mlp, bcore_inst );

            // --------------------------------------------------------------------
            // source: badapt_c1d.h

            // group: badapt_c1d
            BCORE_REGISTER_OBJECT( badapt_c1d_layer_s );
            BCORE_REGISTER_OBJECT( badapt_c1d_arr_layer_s );
            BCORE_REGISTER_FFUNC( badapt_adaptive_get_in_size, badapt_c1d_s_get_in_size );
            BCORE_REGISTER_FFUNC( badapt_adaptive_get_out_size, badapt_c1d_s_get_out_size );
            BCORE_REGISTER_FFUNC( badapt_adaptive_get_dynamics_std, badapt_c1d_s_get_dynamics_std );
            BCORE_REGISTER_FFUNC( badapt_adaptive_set_dynamics_std, badapt_c1d_s_set_dynamics_std );
            BCORE_REGISTER_FFUNC( badapt_adaptive_arc_to_sink, badapt_c1d_s_arc_to_sink );
            BCORE_REGISTER_FFUNC( badapt_adaptive_infer, badapt_c1d_s_infer );
            BCORE_REGISTER_FFUNC( badapt_adaptive_minfer, badapt_c1d_s_minfer );
            BCORE_REGISTER_FFUNC( badapt_adaptive_bgrad, badapt_c1d_s_bgrad );
            BCORE_REGISTER_FFUNC( badapt_adaptive_bgrad_adapt, badapt_c1d_s_bgrad_adapt );
            BCORE_REGISTER_OBJECT( badapt_c1d_s );
            BCORE_REGISTER_FFUNC( badapt_builder_get_in_size, badapt_builder_c1d_funnel_s_get_in_size );
            BCORE_REGISTER_FFUNC( badapt_builder_set_in_size, badapt_builder_c1d_funnel_s_set_in_size );
            BCORE_REGISTER_FFUNC( badapt_builder_get_out_size, badapt_builder_c1d_funnel_s_get_out_size );
            BCORE_REGISTER_FFUNC( badapt_builder_set_out_size, badapt_builder_c1d_funnel_s_set_out_size );
            BCORE_REGISTER_FFUNC( badapt_builder_build, badapt_builder_c1d_funnel_s_build );
            BCORE_REGISTER_OBJECT( badapt_builder_c1d_funnel_s );
            BCORE_REGISTER_TRAIT( badapt_c1d, bcore_inst );

            // --------------------------------------------------------------------
            // source: badapt_ern.h

            // group: badapt_ern
            BCORE_REGISTER_OBJECT( badapt_ern_layer_s );
            BCORE_REGISTER_OBJECT( badapt_ern_arr_layer_s );
            BCORE_REGISTER_FFUNC( badapt_adaptive_get_in_size, badapt_ern_s_get_in_size );
            BCORE_REGISTER_FFUNC( badapt_adaptive_get_out_size, badapt_ern_s_get_out_size );
            BCORE_REGISTER_FFUNC( badapt_adaptive_get_dynamics_std, badapt_ern_s_get_dynamics_std );
            BCORE_REGISTER_FFUNC( badapt_adaptive_set_dynamics_std, badapt_ern_s_set_dynamics_std );
            BCORE_REGISTER_FFUNC( badapt_adaptive_arc_to_sink, badapt_ern_s_arc_to_sink );
            BCORE_REGISTER_FFUNC( badapt_adaptive_minfer, badapt_ern_s_minfer );
            BCORE_REGISTER_FFUNC( badapt_adaptive_bgrad_adapt, badapt_ern_s_bgrad_adapt );
            BCORE_REGISTER_FFUNC( badapt_adaptive_get_weights_min_max, badapt_ern_s_get_weights_min_max );
            BCORE_REGISTER_OBJECT( badapt_ern_s );
            BCORE_REGISTER_FFUNC( badapt_builder_get_in_size, badapt_ern_builder_s_get_in_size );
            BCORE_REGISTER_FFUNC( badapt_builder_set_in_size, badapt_ern_builder_s_set_in_size );
            BCORE_REGISTER_FFUNC( badapt_builder_get_out_size, badapt_ern_builder_s_get_out_size );
            BCORE_REGISTER_FFUNC( badapt_builder_set_out_size, badapt_ern_builder_s_set_out_size );
            BCORE_REGISTER_FFUNC( badapt_builder_build, badapt_ern_builder_s_build );
            BCORE_REGISTER_OBJECT( badapt_ern_builder_s );
            BCORE_REGISTER_TRAIT( badapt_ern, bcore_inst );

            // --------------------------------------------------------------------
            // source: badapt_lstm.h

            // group: badapt_lstm
            BCORE_REGISTER_OBJECT( badapt_lstm_layer_s );
            BCORE_REGISTER_OBJECT( badapt_lstm_arr_layer_s );
            BCORE_REGISTER_FFUNC( badapt_adaptive_get_in_size, badapt_lstm_s_get_in_size );
            BCORE_REGISTER_FFUNC( badapt_adaptive_get_out_size, badapt_lstm_s_get_out_size );
            BCORE_REGISTER_FFUNC( badapt_adaptive_get_dynamics_std, badapt_lstm_s_get_dynamics_std );
            BCORE_REGISTER_FFUNC( badapt_adaptive_set_dynamics_std, badapt_lstm_s_set_dynamics_std );
            BCORE_REGISTER_FFUNC( badapt_adaptive_arc_to_sink, badapt_lstm_s_arc_to_sink );
            BCORE_REGISTER_FFUNC( badapt_adaptive_minfer, badapt_lstm_s_minfer );
            BCORE_REGISTER_FFUNC( badapt_adaptive_bgrad_adapt, badapt_lstm_s_bgrad_adapt );
            BCORE_REGISTER_FFUNC( badapt_adaptive_reset, badapt_lstm_s_reset );
            BCORE_REGISTER_FFUNC( badapt_adaptive_get_weights_min_max, badapt_lstm_s_get_weights_min_max );
            BCORE_REGISTER_OBJECT( badapt_lstm_s );
            BCORE_REGISTER_FFUNC( bcore_inst_call_init_x, badapt_lstm_builder_s_init_x );
            BCORE_REGISTER_FFUNC( badapt_builder_get_in_size, badapt_lstm_builder_s_get_in_size );
            BCORE_REGISTER_FFUNC( badapt_builder_set_in_size, badapt_lstm_builder_s_set_in_size );
            BCORE_REGISTER_FFUNC( badapt_builder_get_out_size, badapt_lstm_builder_s_get_out_size );
            BCORE_REGISTER_FFUNC( badapt_builder_set_out_size, badapt_lstm_builder_s_set_out_size );
            BCORE_REGISTER_FFUNC( badapt_builder_build, badapt_lstm_builder_s_build );
            BCORE_REGISTER_OBJECT( badapt_lstm_builder_s );
            BCORE_REGISTER_TRAIT( badapt_lstm, bcore_inst );

            // --------------------------------------------------------------------
            // source: badapt_training.h

            // group: badapt_supplier
            BCORE_REGISTER_FEATURE( badapt_supplier_get_in_size );
            BCORE_REGISTER_FEATURE( badapt_supplier_get_out_size );
            BCORE_REGISTER_FEATURE( badapt_supplier_fetch_sample_tin );
            BCORE_REGISTER_FEATURE( badapt_supplier_fetch_sample_tio );
            BCORE_REGISTER_FEATURE( badapt_supplier_fetch_sample_vin );
            BCORE_REGISTER_FEATURE( badapt_supplier_fetch_sample_vio );
            BCORE_REGISTER_FEATURE( badapt_supplier_preferred_loss );
            BCORE_REGISTER_FEATURE( badapt_supplier_setup_builder );
            BCORE_REGISTER_FFUNC( badapt_supplier_setup_builder, badapt_supplier_setup_builder_default );
            BCORE_REGISTER_SPECT( badapt_supplier );

            // group: badapt_guide
            BCORE_REGISTER_FEATURE( badapt_guide_callback );
            BCORE_REGISTER_SPECT( badapt_guide );

            // group: badapt_training_state
            BCORE_REGISTER_FEATURE( badapt_training_state_set_adaptive );
            BCORE_REGISTER_FEATURE( badapt_training_state_get_adaptive );
            BCORE_REGISTER_FEATURE( badapt_training_state_set_supplier );
            BCORE_REGISTER_FEATURE( badapt_training_state_get_supplier );
            BCORE_REGISTER_FEATURE( badapt_training_state_set_progress );
            BCORE_REGISTER_FEATURE( badapt_training_state_get_progress );
            BCORE_REGISTER_FEATURE( badapt_training_state_set_guide );
            BCORE_REGISTER_FEATURE( badapt_training_state_get_guide );
            BCORE_REGISTER_FEATURE( badapt_training_state_set_backup_path );
            BCORE_REGISTER_FEATURE( badapt_training_state_get_backup_path );
            BCORE_REGISTER_FEATURE( badapt_training_state_backup );
            BCORE_REGISTER_FFUNC( badapt_training_state_backup, badapt_training_state_backup__ );
            BCORE_REGISTER_FEATURE( badapt_training_state_recover );
            BCORE_REGISTER_FFUNC( badapt_training_state_recover, badapt_training_state_recover__ );
            BCORE_REGISTER_SPECT( badapt_training_state );

            // group: badapt_trainer
            BCORE_REGISTER_FEATURE( badapt_trainer_run );
            BCORE_REGISTER_FEATURE( badapt_trainer_create_state );
            BCORE_REGISTER_SPECT( badapt_trainer );

            // group: badapt_training_objects
            BCORE_REGISTER_OBJECT( badapt_progress_s );
            BCORE_REGISTER_FFUNC( badapt_training_state_set_adaptive, badapt_training_state_std_s_set_adaptive );
            BCORE_REGISTER_FFUNC( badapt_training_state_get_adaptive, badapt_training_state_std_s_get_adaptive );
            BCORE_REGISTER_FFUNC( badapt_training_state_set_supplier, badapt_training_state_std_s_set_supplier );
            BCORE_REGISTER_FFUNC( badapt_training_state_get_supplier, badapt_training_state_std_s_get_supplier );
            BCORE_REGISTER_FFUNC( badapt_training_state_set_progress, badapt_training_state_std_s_set_progress );
            BCORE_REGISTER_FFUNC( badapt_training_state_get_progress, badapt_training_state_std_s_get_progress );
            BCORE_REGISTER_FFUNC( badapt_training_state_set_guide, badapt_training_state_std_s_set_guide );
            BCORE_REGISTER_FFUNC( badapt_training_state_get_guide, badapt_training_state_std_s_get_guide );
            BCORE_REGISTER_FFUNC( badapt_training_state_set_backup_path, badapt_training_state_std_s_set_backup_path );
            BCORE_REGISTER_FFUNC( badapt_training_state_get_backup_path, badapt_training_state_std_s_get_backup_path );
            BCORE_REGISTER_OBJECT( badapt_training_state_std_s );
            BCORE_REGISTER_FFUNC( bcore_inst_call_init_x, badapt_guide_std_s_init_x );
            BCORE_REGISTER_FFUNC( badapt_guide_callback, badapt_guide_std_s_callback );
            BCORE_REGISTER_OBJECT( badapt_guide_std_s );
            BCORE_REGISTER_TRAIT( badapt_training_objects, bcore_inst );

            // --------------------------------------------------------------------
            // source: badapt_trainer.h

            // group: badapt_trainer_objects
            BCORE_REGISTER_FFUNC( badapt_trainer_run, badapt_trainer_batch_s_run );
            BCORE_REGISTER_FFUNC( badapt_trainer_create_state, badapt_trainer_batch_s_create_state );
            BCORE_REGISTER_OBJECT( badapt_trainer_batch_s );
            BCORE_REGISTER_OBJECT( badapt_sample_batch_s );
            BCORE_REGISTER_OBJECT( badapt_arr_sample_batch_s );
            BCORE_REGISTER_FFUNC( bcore_main_main, badapt_trainer_main_s_main );
            BCORE_REGISTER_OBJECT( badapt_trainer_main_s );
            BCORE_REGISTER_TRAIT( badapt_trainer_objects, bcore_inst );
        }
        break;
        case TYPEOF_push_dependencies:
        {
            ASSERT( o->object && ( *( aware_t* )o->object ) == TYPEOF_bcore_arr_fp_s );
            bcore_arr_fp_s* arr_fp = o->object;
            bcore_arr_fp_s_push( arr_fp, ( fp_t )bmath_general_signal_handler );
        }
        break;
        default: break;
    }
    return NULL;
}
// BETH_PLANT_SIGNATURE 16676368807250191312
