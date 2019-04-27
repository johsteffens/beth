/** This file was generated by the beth-precode-compiler. Not suitable for manual editing.
 *
 *  Copyright and License:
 *    If a specification exists for the repository or folder in which the file is located,
 *    that specification shall apply. Otherwise, the Copyright and License of the associated
 *    source code files containing beth-precode shall apply.
 */

#include "bmath_precoded.h"
#include "bcore_spect.h"
#include "bcore_spect_inst.h"
#include "bcore_sr.h"
#include "bcore_const_manager.h"


/**********************************************************************************************************************/
/// source: bmath_mf3_sx
#include "bmath_mf3_sx.h"

//----------------------------------------------------------------------------------------------------------------------
/// group: bmath_mf3_sx

BCORE_DEFINE_OBJECT_INST( bcore_inst, bmath_mf3_sx_s )
"{"
    "aware_t _;"
    "sz_t slos;"
    "sz_t xons;"
    "sz_t rows;"
    "sz_t i_stride;"
    "sz_t [] i;"
    "f3_t [] v;"
"}";

/**********************************************************************************************************************/
/// source: bmath_mf3_sf
#include "bmath_mf3_sf.h"

//----------------------------------------------------------------------------------------------------------------------
/// group: bmath_mf3_sf

BCORE_DEFINE_OBJECT_INST( bcore_inst, bmath_mf3_sf_s )
"{"
    "aware_t _;"
    "sz_t cols;"
    "sz_t rows;"
    "sz_t i_stride;"
    "sz_t [] i;"
    "f3_t [] v;"
"}";

/**********************************************************************************************************************/
/// source: bmath_adaptive
#include "bmath_adaptive.h"

//----------------------------------------------------------------------------------------------------------------------
/// group: bmath_adaptive

BCORE_DEFINE_SPECT( bcore_inst, bmath_adaptive )
"{"
    "bcore_spect_header_s header;"
    "feature bmath_adaptive : get_in_size = bmath_adaptive_get_in_size_default;"
    "feature bmath_adaptive : set_in_size = bmath_adaptive_set_in_size_default;"
    "feature bmath_adaptive : get_out_size = bmath_adaptive_get_out_size_default;"
    "feature bmath_adaptive : set_out_size = bmath_adaptive_set_out_size_default;"
    "feature bmath_adaptive : get_step = bmath_adaptive_get_step_default;"
    "feature bmath_adaptive : set_step = bmath_adaptive_set_step_default;"
    "feature bmath_adaptive : get_decay = bmath_adaptive_get_decay_default;"
    "feature bmath_adaptive : set_decay = bmath_adaptive_set_decay_default;"
    "feature bmath_adaptive : setup = bmath_adaptive_setup_default;"
    "feature bmath_adaptive : set_untrained = bmath_adaptive_set_untrained_default;"
    "feature bmath_adaptive : arc_to_sink = bmath_adaptive_arc_to_sink_default;"
    "feature bmath_adaptive : query = bmath_adaptive_query_default;"
    "feature bmath_adaptive : adapt = bmath_adaptive_adapt_default;"
"}";

static sz_t bmath_adaptive_get_in_size_default( const bmath_adaptive* o )
{
    ERR_fa( "Feature has no binding." );
    return *(sz_t*)NULL;
}

static void bmath_adaptive_set_in_size_default( bmath_adaptive* o, sz_t size )
{
    ERR_fa( "Feature has no binding." );
}

static sz_t bmath_adaptive_get_out_size_default( const bmath_adaptive* o )
{
    ERR_fa( "Feature has no binding." );
    return *(sz_t*)NULL;
}

static void bmath_adaptive_set_out_size_default( bmath_adaptive* o, sz_t size )
{
    ERR_fa( "Feature has no binding." );
}

static f3_t bmath_adaptive_get_step_default( const bmath_adaptive* o )
{
    ERR_fa( "Feature has no binding." );
    return *(f3_t*)NULL;
}

static void bmath_adaptive_set_step_default( bmath_adaptive* o, f3_t val )
{
    ERR_fa( "Feature has no binding." );
}

static f3_t bmath_adaptive_get_decay_default( const bmath_adaptive* o )
{
    ERR_fa( "Feature has no binding." );
    return *(f3_t*)NULL;
}

static void bmath_adaptive_set_decay_default( bmath_adaptive* o, f3_t val )
{
    ERR_fa( "Feature has no binding." );
}

static void bmath_adaptive_setup_default( bmath_adaptive* o, bl_t training )
{
    ERR_fa( "Feature has no binding." );
}

static void bmath_adaptive_set_untrained_default( bmath_adaptive* o )
{
    ERR_fa( "Feature has no binding." );
}

static void bmath_adaptive_arc_to_sink_default( const bmath_adaptive* o, bcore_sink* sink )
{
    ERR_fa( "Feature has no binding." );
}

static void bmath_adaptive_query_default( bmath_adaptive* o, const bmath_vf3_s* in, bmath_vf3_s* out )
{
    ERR_fa( "Feature has no binding." );
}

static void bmath_adaptive_adapt_default( bmath_adaptive* o, const bmath_vf3_s* in, const bmath_vf3_s* target, bmath_vf3_s* out )
{
    ERR_fa( "Feature has no binding." );
}

/**********************************************************************************************************************/
/// source: bmath_activation
#include "bmath_activation.h"

//----------------------------------------------------------------------------------------------------------------------
/// group: bmath_activation

BCORE_DEFINE_SPECT( bcore_inst, bmath_activation )
"{"
    "bcore_spect_header_s header;"
    "feature bmath_activation : fx = bmath_activation_fx_default;"
    "feature bmath_activation : dy = bmath_activation_dy_default;"
"}";

static f3_t bmath_activation_fx_default( const bmath_activation* o, f3_t x )
{
    ERR_fa( "Feature has no binding." );
    return *(f3_t*)NULL;
}

static f3_t bmath_activation_dy_default( const bmath_activation* o, f3_t y )
{
    ERR_fa( "Feature has no binding." );
    return *(f3_t*)NULL;
}

//----------------------------------------------------------------------------------------------------------------------
/// group: bmath_activation_objects

BCORE_DEFINE_OBJECT_INST( bmath_activation, bmath_activation_tanh_s )
"{"
    "aware_t _;"
    "func bmath_activation : fx;"
    "func bmath_activation : dy;"
"}";

BCORE_DEFINE_OBJECT_INST( bmath_activation, bmath_activation_relu_s )
"{"
    "aware_t _;"
    "func bmath_activation : fx;"
    "func bmath_activation : dy;"
"}";

BCORE_DEFINE_OBJECT_INST( bmath_activation, bmath_activation_leaky_relu_s )
"{"
    "aware_t _;"
    "func bmath_activation : fx;"
    "func bmath_activation : dy;"
"}";

BCORE_DEFINE_OBJECT_INST( bmath_activation, bmath_activation_softplus_s )
"{"
    "aware_t _;"
    "func bmath_activation : fx;"
    "func bmath_activation : dy;"
"}";

/**********************************************************************************************************************/
/// source: bmath_adaptive_cnn_1d
#include "bmath_adaptive_cnn_1d.h"

//----------------------------------------------------------------------------------------------------------------------
/// group: bmath_adaptive_cnn_1d

BCORE_DEFINE_OBJECT_INST( bmath_adaptive, bmath_adaptive_cnn_1d_s )
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
    "f3_t adapt_step = 0.0001;"
    "f3_t decay_step = 0;"
    "sr_s act_mid;"
    "sr_s act_out;"
    "u2_t random_state = 1234;"
    "bmath_arr_mf3_s arr_w;"
    "hidden bmath_arr_mf3_s arr_a;"
    "hidden bmath_arr_mf3_s arr_b;"
    "hidden bmath_vf3_s buf_ab;"
    "hidden bmath_arr_mf3_s arr_ga;"
    "hidden bmath_arr_mf3_s arr_gb;"
    "hidden bmath_vf3_s buf_gab;"
    "hidden bcore_arr_sr_s arr_activation;"
    "hidden bmath_vf3_s in;"
    "hidden bmath_vf3_s out;"
    "hidden bmath_vf3_s gout;"
    "func bmath_adaptive: get_in_size;"
    "func bmath_adaptive: set_in_size;"
    "func bmath_adaptive: get_out_size;"
    "func bmath_adaptive: set_out_size;"
    "func bmath_adaptive: get_step;"
    "func bmath_adaptive: set_step;"
    "func bmath_adaptive: get_decay;"
    "func bmath_adaptive: set_decay;"
    "func bmath_adaptive: setup;"
    "func bmath_adaptive: set_untrained;"
    "func bmath_adaptive: arc_to_sink;"
    "func bmath_adaptive: query;"
    "func bmath_adaptive: adapt;"
"}";

/**********************************************************************************************************************/
/// source: bmath_adaptive_mlp
#include "bmath_adaptive_mlp.h"

//----------------------------------------------------------------------------------------------------------------------
/// group: bmath_adaptive_mlp

BCORE_DEFINE_OBJECT_INST( bmath_adaptive, bmath_adaptive_mlp_s )
"{"
    "aware_t _;"
    "sz_t input_size;"
    "sz_t input_kernels = 8;"
    "sz_t output_kernels = 1;"
    "sz_t layers = 2;"
    "f3_t kernels_rate = 0;"
    "f3_t adapt_step = 0.0001;"
    "f3_t decay_step = 0;"
    "sr_s act_mid;"
    "sr_s act_out;"
    "u2_t random_state =1234;"
    "bmath_arr_mf3_s arr_w;"
    "hidden bmath_arr_vf3_s arr_a;"
    "hidden bmath_arr_vf3_s arr_b;"
    "hidden bmath_vf3_s buf_ab;"
    "hidden bmath_arr_vf3_s arr_ga;"
    "hidden bmath_arr_vf3_s arr_gb;"
    "hidden bmath_vf3_s buf_gab;"
    "hidden bcore_arr_sr_s arr_activation;"
    "hidden bmath_vf3_s in;"
    "hidden bmath_vf3_s out;"
    "hidden bmath_vf3_s gout;"
    "func bmath_adaptive: get_in_size;"
    "func bmath_adaptive: set_in_size;"
    "func bmath_adaptive: get_out_size;"
    "func bmath_adaptive: set_out_size;"
    "func bmath_adaptive: get_step;"
    "func bmath_adaptive: set_step;"
    "func bmath_adaptive: get_decay;"
    "func bmath_adaptive: set_decay;"
    "func bmath_adaptive: setup;"
    "func bmath_adaptive: set_untrained;"
    "func bmath_adaptive: arc_to_sink;"
    "func bmath_adaptive: query;"
    "func bmath_adaptive: adapt;"
"}";

/**********************************************************************************************************************/

vd_t bmath_precoded_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_precoded" ) ) )
    {
        case TYPEOF_init1:
        {
            // Comment or remove line below to rebuild this target.
            bcore_const_x_set_d( typeof( "bmath_precoded_hash" ), sr_tp( 2840872550 ) );
            BCORE_REGISTER_OBJECT( bmath_mf3_sx_s );
            BCORE_REGISTER_OBJECT( bmath_mf3_sf_s );
            BCORE_REGISTER_FEATURE( bmath_adaptive_get_in_size );
            BCORE_REGISTER_FEATURE( bmath_adaptive_set_in_size );
            BCORE_REGISTER_FEATURE( bmath_adaptive_get_out_size );
            BCORE_REGISTER_FEATURE( bmath_adaptive_set_out_size );
            BCORE_REGISTER_FEATURE( bmath_adaptive_get_step );
            BCORE_REGISTER_FEATURE( bmath_adaptive_set_step );
            BCORE_REGISTER_FEATURE( bmath_adaptive_get_decay );
            BCORE_REGISTER_FEATURE( bmath_adaptive_set_decay );
            BCORE_REGISTER_FEATURE( bmath_adaptive_setup );
            BCORE_REGISTER_FEATURE( bmath_adaptive_set_untrained );
            BCORE_REGISTER_FEATURE( bmath_adaptive_arc_to_sink );
            BCORE_REGISTER_FEATURE( bmath_adaptive_query );
            BCORE_REGISTER_FEATURE( bmath_adaptive_adapt );
            BCORE_REGISTER_FFUNC( bmath_adaptive_get_in_size, bmath_adaptive_get_in_size_default );
            BCORE_REGISTER_FFUNC( bmath_adaptive_set_in_size, bmath_adaptive_set_in_size_default );
            BCORE_REGISTER_FFUNC( bmath_adaptive_get_out_size, bmath_adaptive_get_out_size_default );
            BCORE_REGISTER_FFUNC( bmath_adaptive_set_out_size, bmath_adaptive_set_out_size_default );
            BCORE_REGISTER_FFUNC( bmath_adaptive_get_step, bmath_adaptive_get_step_default );
            BCORE_REGISTER_FFUNC( bmath_adaptive_set_step, bmath_adaptive_set_step_default );
            BCORE_REGISTER_FFUNC( bmath_adaptive_get_decay, bmath_adaptive_get_decay_default );
            BCORE_REGISTER_FFUNC( bmath_adaptive_set_decay, bmath_adaptive_set_decay_default );
            BCORE_REGISTER_FFUNC( bmath_adaptive_setup, bmath_adaptive_setup_default );
            BCORE_REGISTER_FFUNC( bmath_adaptive_set_untrained, bmath_adaptive_set_untrained_default );
            BCORE_REGISTER_FFUNC( bmath_adaptive_arc_to_sink, bmath_adaptive_arc_to_sink_default );
            BCORE_REGISTER_FFUNC( bmath_adaptive_query, bmath_adaptive_query_default );
            BCORE_REGISTER_FFUNC( bmath_adaptive_adapt, bmath_adaptive_adapt_default );
            BCORE_REGISTER_SPECT( bmath_adaptive );
            BCORE_REGISTER_FEATURE( bmath_activation_fx );
            BCORE_REGISTER_FEATURE( bmath_activation_dy );
            BCORE_REGISTER_FFUNC( bmath_activation_fx, bmath_activation_fx_default );
            BCORE_REGISTER_FFUNC( bmath_activation_dy, bmath_activation_dy_default );
            BCORE_REGISTER_SPECT( bmath_activation );
            BCORE_REGISTER_FFUNC( bmath_activation_fx, bmath_activation_tanh_s_fx );
            BCORE_REGISTER_FFUNC( bmath_activation_dy, bmath_activation_tanh_s_dy );
            BCORE_REGISTER_OBJECT( bmath_activation_tanh_s );
            BCORE_REGISTER_FFUNC( bmath_activation_fx, bmath_activation_relu_s_fx );
            BCORE_REGISTER_FFUNC( bmath_activation_dy, bmath_activation_relu_s_dy );
            BCORE_REGISTER_OBJECT( bmath_activation_relu_s );
            BCORE_REGISTER_FFUNC( bmath_activation_fx, bmath_activation_leaky_relu_s_fx );
            BCORE_REGISTER_FFUNC( bmath_activation_dy, bmath_activation_leaky_relu_s_dy );
            BCORE_REGISTER_OBJECT( bmath_activation_leaky_relu_s );
            BCORE_REGISTER_FFUNC( bmath_activation_fx, bmath_activation_softplus_s_fx );
            BCORE_REGISTER_FFUNC( bmath_activation_dy, bmath_activation_softplus_s_dy );
            BCORE_REGISTER_OBJECT( bmath_activation_softplus_s );
            BCORE_REGISTER_FFUNC( bmath_adaptive_get_in_size, bmath_adaptive_cnn_1d_s_get_in_size );
            BCORE_REGISTER_FFUNC( bmath_adaptive_set_in_size, bmath_adaptive_cnn_1d_s_set_in_size );
            BCORE_REGISTER_FFUNC( bmath_adaptive_get_out_size, bmath_adaptive_cnn_1d_s_get_out_size );
            BCORE_REGISTER_FFUNC( bmath_adaptive_set_out_size, bmath_adaptive_cnn_1d_s_set_out_size );
            BCORE_REGISTER_FFUNC( bmath_adaptive_get_step, bmath_adaptive_cnn_1d_s_get_step );
            BCORE_REGISTER_FFUNC( bmath_adaptive_set_step, bmath_adaptive_cnn_1d_s_set_step );
            BCORE_REGISTER_FFUNC( bmath_adaptive_get_decay, bmath_adaptive_cnn_1d_s_get_decay );
            BCORE_REGISTER_FFUNC( bmath_adaptive_set_decay, bmath_adaptive_cnn_1d_s_set_decay );
            BCORE_REGISTER_FFUNC( bmath_adaptive_setup, bmath_adaptive_cnn_1d_s_setup );
            BCORE_REGISTER_FFUNC( bmath_adaptive_set_untrained, bmath_adaptive_cnn_1d_s_set_untrained );
            BCORE_REGISTER_FFUNC( bmath_adaptive_arc_to_sink, bmath_adaptive_cnn_1d_s_arc_to_sink );
            BCORE_REGISTER_FFUNC( bmath_adaptive_query, bmath_adaptive_cnn_1d_s_query );
            BCORE_REGISTER_FFUNC( bmath_adaptive_adapt, bmath_adaptive_cnn_1d_s_adapt );
            BCORE_REGISTER_OBJECT( bmath_adaptive_cnn_1d_s );
            BCORE_REGISTER_FFUNC( bmath_adaptive_get_in_size, bmath_adaptive_mlp_s_get_in_size );
            BCORE_REGISTER_FFUNC( bmath_adaptive_set_in_size, bmath_adaptive_mlp_s_set_in_size );
            BCORE_REGISTER_FFUNC( bmath_adaptive_get_out_size, bmath_adaptive_mlp_s_get_out_size );
            BCORE_REGISTER_FFUNC( bmath_adaptive_set_out_size, bmath_adaptive_mlp_s_set_out_size );
            BCORE_REGISTER_FFUNC( bmath_adaptive_get_step, bmath_adaptive_mlp_s_get_step );
            BCORE_REGISTER_FFUNC( bmath_adaptive_set_step, bmath_adaptive_mlp_s_set_step );
            BCORE_REGISTER_FFUNC( bmath_adaptive_get_decay, bmath_adaptive_mlp_s_get_decay );
            BCORE_REGISTER_FFUNC( bmath_adaptive_set_decay, bmath_adaptive_mlp_s_set_decay );
            BCORE_REGISTER_FFUNC( bmath_adaptive_setup, bmath_adaptive_mlp_s_setup );
            BCORE_REGISTER_FFUNC( bmath_adaptive_set_untrained, bmath_adaptive_mlp_s_set_untrained );
            BCORE_REGISTER_FFUNC( bmath_adaptive_arc_to_sink, bmath_adaptive_mlp_s_arc_to_sink );
            BCORE_REGISTER_FFUNC( bmath_adaptive_query, bmath_adaptive_mlp_s_query );
            BCORE_REGISTER_FFUNC( bmath_adaptive_adapt, bmath_adaptive_mlp_s_adapt );
            BCORE_REGISTER_OBJECT( bmath_adaptive_mlp_s );
        }
        break;
        default: break;
    }
    return NULL;
}
