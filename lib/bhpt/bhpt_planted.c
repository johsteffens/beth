/** This file was generated from beth-plant source code.
 *  Compiling Agent : bcore_plant_compiler (C) 2019 J.B.Steffens
 *  Last File Update: 2020-04-25T15:39:53Z
 *
 *  Copyright and License of this File:
 *
 *  Generated code inherits the copyright and license of the underlying beth-plant source code.
 *  Source code defining this file is distributed across following files:
 *
 *  bhpt_sketch.h
 *  bhpt_frame.h
 *  bhpt_adaptor.h
 *  bhpt_tutor_sampler.h
 *
 */

#include "bhpt_planted.h"
#include "bcore_spect.h"
#include "bcore_spect_inst.h"
#include "bcore_sr.h"
#include "bcore_const_manager.h"


/**********************************************************************************************************************/
// source: bhpt_sketch.h
#include "bhpt_sketch.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bhpt

//----------------------------------------------------------------------------------------------------------------------
// group: bhpt_adaptor

BCORE_DEFINE_OBJECT_INST_P( bhpt_adaptor_node_s )
"aware bhpt_adaptor"
"{"
    "bhvm_holor_s -> axon;"
    "bhvm_holor_s -> grad;"
"}";

void bhpt_adaptor_node_s_get_min_max( const bhpt_adaptor_node_s* o, f3_t* min, f3_t* max )
{
    if( o->axon )
    {
        if( min ) *min = bhvm_value_s_get_min_f3( &o->axon->v );
        if( max ) *max = bhvm_value_s_get_max_f3( &o->axon->v );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhpt_adaptor_probe_s )
"aware bcore_array"
"{"
    "bhpt_adaptor_node_s [];"
"}";

void bhpt_adaptor_probe_s_get_min_max( const bhpt_adaptor_probe_s* o, f3_t* min, f3_t* max )
{
    f3_t min_l = 0, max_l = 0;
    BFOR_EACH( i, o )
    {
        bhpt_adaptor_node_s_get_min_max( &o->data[ i ], &min_l, &max_l );
        if( min ) *min = ( i > 0 ) ? f3_min( *min, min_l ) : min_l;
        if( max ) *max = ( i > 0 ) ? f3_max( *max, max_l ) : max_l;
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhpt_adaptor_adl_s )
"aware bcore_array"
"{"
    "aware bhpt_adaptor=> [];"
"}";

BCORE_DEFINE_SPECT( bhpt, bhpt_adaptor )
"{"
    "bcore_spect_header_s header;"
    "feature aware bhpt_adaptor : reset;"
    "feature aware bhpt_adaptor : adapt;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhpt_adaptive

BCORE_DEFINE_SPECT( bhpt, bhpt_adaptive )
"{"
    "bcore_spect_header_s header;"
    "feature strict aware bhpt_adaptive : get_format_en;"
    "feature strict aware bhpt_adaptive : get_format_ex;"
    "feature aware bhpt_adaptive : axon_pass = bhpt_adaptive_axon_pass__;"
    "feature aware bhpt_adaptive : dendrite_pass = bhpt_adaptive_dendrite_pass__;"
    "feature aware bhpt_adaptive : cyclic_reset = bhpt_adaptive_cyclic_reset__;"
    "feature aware bhpt_adaptive : get_adaptor_probe = bhpt_adaptive_get_adaptor_probe__;"
    "feature aware bhpt_adaptive : status_to_sink = bhpt_adaptive_status_to_sink__;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhpt_builder

BCORE_DEFINE_SPECT( bhpt, bhpt_builder )
"{"
    "bcore_spect_header_s header;"
    "feature strict aware bhpt_builder : set_format_en;"
    "feature strict aware bhpt_builder : set_format_ex;"
    "feature strict aware bhpt_builder : create_adaptive;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhpt_tutor

BCORE_DEFINE_SPECT( bhpt, bhpt_tutor )
"{"
    "bcore_spect_header_s header;"
    "feature strict aware bhpt_tutor : create_adaptive;"
    "feature aware bhpt_tutor : reset = bhpt_tutor_reset__;"
    "feature aware bhpt_tutor : prime = bhpt_tutor_prime__;"
    "feature aware bhpt_tutor : test = bhpt_tutor_test__;"
    "feature aware bhpt_tutor : status_to_sink = bhpt_tutor_status_to_sink__;"
"}";

/**********************************************************************************************************************/
// source: bhpt_frame.h
#include "bhpt_frame.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bhpt_frame

BCORE_DEFINE_OBJECT_INST_P( bhpt_frame_state_s )
"aware bhpt_frame"
"{"
    "sz_t cycle_number;"
    "sz_t last_cycle_adapt;"
    "sz_t last_cycle_test;"
    "sz_t last_cycle_backup;"
    "aware bhpt_adaptive => adaptive;"
    "bhpt_adaptor_adl_s => adaptor_adl;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhpt_frame_thread_s )
"aware bhpt_frame"
"{"
    "bl_t flag;"
    "bl_t running = false;"
    "sz_t prime_cycles = 0;"
    "aware bhpt_adaptive => adaptive;"
    "bhpt_adaptor_probe_s => probe;"
    "aware bhpt_tutor -> tutor;"
    "bcore_thread_s => thread;"
    "bcore_mutex_s => mutex;"
    "bcore_condition_s -> condition;"
    "func bcore_inst_call:down_e;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhpt_frame_thread_ads_s )
"aware bcore_array"
"{"
    "bhpt_frame_thread_s [];"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhpt_frame_thread_base_s )
"aware bhpt_frame"
"{"
    "bhpt_frame_thread_ads_s thread_ads;"
    "bcore_condition_s => condition;"
    "func bcore_inst_call:down_e;"
    "func bcore_inst_call:copy_e;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhpt_frame_s )
"aware bcore_main"
"{"
    "aware bhpt_tutor => tutor;"
    "aware bhpt_builder => builder;"
    "aware bhpt_adaptor => adaptor;"
    "hidden bhpt_frame_thread_base_s => thread_base;"
    "sz_t threads = 1;"
    "sz_t prime_cycles_per_thread = 1;"
    "sz_t cycle_test = 1000;"
    "sz_t cycle_backup = 1000;"
    "sz_t cycle_finish = 1000000;"
    "sz_t verbosity = 1;"
    "hidden bhpt_frame_state_s => state;"
    "st_s state_path;"
    "hidden aware bcore_sink -> log;"
    "func ^:main;"
"}";

/**********************************************************************************************************************/
// source: bhpt_adaptor.h
#include "bhpt_adaptor.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bhpt_adaptor_stamp

BCORE_DEFINE_OBJECT_INST_P( bhpt_adaptor_epsilon_s )
"aware bhpt_adaptor"
"{"
    "f3_t epsilon;"
    "func ^:reset;"
    "func ^:adapt;"
"}";

void bhpt_adaptor_epsilon_s_adapt( bhpt_adaptor_epsilon_s* o, const bhpt_adaptor_node_s* node )
{
    assert( bhvm_shape_s_is_equal( &node->axon->s, &node->grad->s ) );
    bhvm_value_s_mul_scl_f3_acc( &node->grad->v, o->epsilon, &node->axon->v );
    bhvm_value_s_zro( &node->grad->v );
}

BCORE_DEFINE_OBJECT_INST_P( bhpt_adaptor_list_s )
"aware bhpt_adaptor"
"{"
    "bhpt_adaptor_adl_s adl;"
    "func ^:reset;"
    "func ^:adapt;"
"}";

/**********************************************************************************************************************/
// source: bhpt_tutor_sampler.h
#include "bhpt_tutor_sampler.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bhpt_sampler

BCORE_DEFINE_OBJECT_INST_P( bhpt_sampler_sine_random_s )
"aware bhpt_sampler"
"{"
    "sz_t size_en = 32;"
    "f3_t pos_tgt = 0.9;"
    "f3_t neg_tgt = -0.9;"
    "func ^:fetch;"
    "func ^:get_size_en;"
    "func ^:get_size_ex;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhpt_sampler_binary_add_s )
"aware bhpt_sampler"
"{"
    "sz_t bits = 4;"
    "f3_t val_h = 0.9;"
    "f3_t val_l = -0.9;"
    "func ^:fetch;"
    "func ^:get_size_en;"
    "func ^:get_size_ex;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhpt_sampler_binary_mul_s )
"aware bhpt_sampler"
"{"
    "sz_t bits = 4;"
    "f3_t val_h = 0.9;"
    "f3_t val_l = -0.9;"
    "func ^:fetch;"
    "func ^:get_size_en;"
    "func ^:get_size_ex;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhpt_sampler_binary_xsg3_s )
"aware bhpt_sampler"
"{"
    "sz_t bits = 4;"
    "f3_t val_h = 0.9;"
    "f3_t val_l = -0.9;"
    "func ^:fetch;"
    "func ^:get_size_en;"
    "func ^:get_size_ex;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhpt_sampler_binary_hash_s )
"aware bhpt_sampler"
"{"
    "sz_t bits = 4;"
    "f3_t val_h = 0.9;"
    "f3_t val_l = -0.9;"
    "bl_t reverse = false;"
    "func ^:fetch;"
    "func ^:get_size_en;"
    "func ^:get_size_ex;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhpt_sampler_polynom_s )
"aware bhpt_sampler"
"{"
    "sz_t size_en = 32;"
    "sz_t size_ex = 3;"
    "f3_t range = 1.0;"
    "f3_t noise_level = 0;"
    "func ^:fetch;"
    "func ^:get_size_en;"
    "func ^:get_size_ex;"
"}";

BCORE_DEFINE_SPECT( bcore_inst, bhpt_sampler )
"{"
    "bcore_spect_header_s header;"
    "feature strict aware bhpt_sampler : fetch;"
    "feature strict aware bhpt_sampler : get_size_en;"
    "feature strict aware bhpt_sampler : get_size_ex;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhpt_tutor_sampler

BCORE_DEFINE_OBJECT_INST_P( bhpt_tutor_sampler_s )
"aware bhpt_tutor"
"{"
    "aware bhpt_sampler => sampler;"
    "u2_t rval_prime = 1234;"
    "u2_t rval_test = 5342;"
    "sz_t test_size = 1000;"
    "hidden bcore_mutex_s mutex;"
    "func ^:reset;"
    "func ^:create_adaptive;"
    "func ^:prime;"
    "func ^:test;"
    "func ^:status_to_sink;"
"}";

void bhpt_tutor_sampler_s_status_to_sink( const bhpt_tutor_sampler_s* o, sz_t verbosity, bcore_sink* sink )
{
    if( verbosity > 0 )
    {
        bcore_sink_a_push_fa( sink, "#<sc_t> : #<sc_t>", ifnameof( o->_ ), ifnameof( o->sampler ? o->sampler->_ : 0 ) );
    }
}

/**********************************************************************************************************************/

vd_t bhpt_planted_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bhpt_planted" ) ) )
    {
        case TYPEOF_init1:
        {
            // Comment or remove line below to rebuild this target.
            bcore_const_x_set_d( typeof( "bhpt_planted_hash" ), sr_tp( 2165840483 ) );

            // --------------------------------------------------------------------
            // source: bhpt_sketch.h

            // group: bhpt
            BCORE_REGISTER_TRAIT( bhpt, bcore_inst );

            // group: bhpt_adaptor
            BCORE_REGISTER_OBJECT( bhpt_adaptor_node_s );
            BCORE_REGISTER_OBJECT( bhpt_adaptor_probe_s );
            BCORE_REGISTER_FEATURE( bhpt_adaptor_reset );
            BCORE_REGISTER_FEATURE( bhpt_adaptor_adapt );
            BCORE_REGISTER_OBJECT( bhpt_adaptor_adl_s );
            BCORE_REGISTER_SPECT( bhpt_adaptor );

            // group: bhpt_adaptive
            BCORE_REGISTER_FEATURE( bhpt_adaptive_get_format_en );
            BCORE_REGISTER_FEATURE( bhpt_adaptive_get_format_ex );
            BCORE_REGISTER_FEATURE( bhpt_adaptive_axon_pass );
            BCORE_REGISTER_FFUNC( bhpt_adaptive_axon_pass, bhpt_adaptive_axon_pass__ );
            BCORE_REGISTER_FEATURE( bhpt_adaptive_dendrite_pass );
            BCORE_REGISTER_FFUNC( bhpt_adaptive_dendrite_pass, bhpt_adaptive_dendrite_pass__ );
            BCORE_REGISTER_FEATURE( bhpt_adaptive_cyclic_reset );
            BCORE_REGISTER_FFUNC( bhpt_adaptive_cyclic_reset, bhpt_adaptive_cyclic_reset__ );
            BCORE_REGISTER_FEATURE( bhpt_adaptive_get_adaptor_probe );
            BCORE_REGISTER_FFUNC( bhpt_adaptive_get_adaptor_probe, bhpt_adaptive_get_adaptor_probe__ );
            BCORE_REGISTER_FEATURE( bhpt_adaptive_status_to_sink );
            BCORE_REGISTER_FFUNC( bhpt_adaptive_status_to_sink, bhpt_adaptive_status_to_sink__ );
            BCORE_REGISTER_SPECT( bhpt_adaptive );

            // group: bhpt_builder
            BCORE_REGISTER_FEATURE( bhpt_builder_set_format_en );
            BCORE_REGISTER_FEATURE( bhpt_builder_set_format_ex );
            BCORE_REGISTER_FEATURE( bhpt_builder_create_adaptive );
            BCORE_REGISTER_SPECT( bhpt_builder );

            // group: bhpt_tutor
            BCORE_REGISTER_FEATURE( bhpt_tutor_create_adaptive );
            BCORE_REGISTER_FEATURE( bhpt_tutor_reset );
            BCORE_REGISTER_FFUNC( bhpt_tutor_reset, bhpt_tutor_reset__ );
            BCORE_REGISTER_FEATURE( bhpt_tutor_prime );
            BCORE_REGISTER_FFUNC( bhpt_tutor_prime, bhpt_tutor_prime__ );
            BCORE_REGISTER_FEATURE( bhpt_tutor_test );
            BCORE_REGISTER_FFUNC( bhpt_tutor_test, bhpt_tutor_test__ );
            BCORE_REGISTER_FEATURE( bhpt_tutor_status_to_sink );
            BCORE_REGISTER_FFUNC( bhpt_tutor_status_to_sink, bhpt_tutor_status_to_sink__ );
            BCORE_REGISTER_SPECT( bhpt_tutor );

            // --------------------------------------------------------------------
            // source: bhpt_frame.h

            // group: bhpt_frame
            BCORE_REGISTER_OBJECT( bhpt_frame_state_s );
            BCORE_REGISTER_FFUNC( bcore_inst_call_down_e, bhpt_frame_thread_s_down_e );
            BCORE_REGISTER_OBJECT( bhpt_frame_thread_s );
            BCORE_REGISTER_OBJECT( bhpt_frame_thread_ads_s );
            BCORE_REGISTER_FFUNC( bcore_inst_call_down_e, bhpt_frame_thread_base_s_down_e );
            BCORE_REGISTER_FFUNC( bcore_inst_call_copy_e, bhpt_frame_thread_base_s_copy_e );
            BCORE_REGISTER_OBJECT( bhpt_frame_thread_base_s );
            BCORE_REGISTER_FFUNC( bcore_main_main, bhpt_frame_s_main );
            BCORE_REGISTER_OBJECT( bhpt_frame_s );
            BCORE_REGISTER_TRAIT( bhpt_frame, bcore_inst );

            // --------------------------------------------------------------------
            // source: bhpt_adaptor.h

            // group: bhpt_adaptor_stamp
            BCORE_REGISTER_FFUNC( bhpt_adaptor_reset, bhpt_adaptor_epsilon_s_reset );
            BCORE_REGISTER_FFUNC( bhpt_adaptor_adapt, bhpt_adaptor_epsilon_s_adapt );
            BCORE_REGISTER_OBJECT( bhpt_adaptor_epsilon_s );
            BCORE_REGISTER_FFUNC( bhpt_adaptor_reset, bhpt_adaptor_list_s_reset );
            BCORE_REGISTER_FFUNC( bhpt_adaptor_adapt, bhpt_adaptor_list_s_adapt );
            BCORE_REGISTER_OBJECT( bhpt_adaptor_list_s );
            BCORE_REGISTER_TRAIT( bhpt_adaptor_stamp, bhpt_adaptor );

            // --------------------------------------------------------------------
            // source: bhpt_tutor_sampler.h

            // group: bhpt_sampler
            BCORE_REGISTER_FEATURE( bhpt_sampler_fetch );
            BCORE_REGISTER_FEATURE( bhpt_sampler_get_size_en );
            BCORE_REGISTER_FEATURE( bhpt_sampler_get_size_ex );
            BCORE_REGISTER_FFUNC( bhpt_sampler_fetch, bhpt_sampler_sine_random_s_fetch );
            BCORE_REGISTER_FFUNC( bhpt_sampler_get_size_en, bhpt_sampler_sine_random_s_get_size_en );
            BCORE_REGISTER_FFUNC( bhpt_sampler_get_size_ex, bhpt_sampler_sine_random_s_get_size_ex );
            BCORE_REGISTER_OBJECT( bhpt_sampler_sine_random_s );
            BCORE_REGISTER_FFUNC( bhpt_sampler_fetch, bhpt_sampler_binary_add_s_fetch );
            BCORE_REGISTER_FFUNC( bhpt_sampler_get_size_en, bhpt_sampler_binary_add_s_get_size_en );
            BCORE_REGISTER_FFUNC( bhpt_sampler_get_size_ex, bhpt_sampler_binary_add_s_get_size_ex );
            BCORE_REGISTER_OBJECT( bhpt_sampler_binary_add_s );
            BCORE_REGISTER_FFUNC( bhpt_sampler_fetch, bhpt_sampler_binary_mul_s_fetch );
            BCORE_REGISTER_FFUNC( bhpt_sampler_get_size_en, bhpt_sampler_binary_mul_s_get_size_en );
            BCORE_REGISTER_FFUNC( bhpt_sampler_get_size_ex, bhpt_sampler_binary_mul_s_get_size_ex );
            BCORE_REGISTER_OBJECT( bhpt_sampler_binary_mul_s );
            BCORE_REGISTER_FFUNC( bhpt_sampler_fetch, bhpt_sampler_binary_xsg3_s_fetch );
            BCORE_REGISTER_FFUNC( bhpt_sampler_get_size_en, bhpt_sampler_binary_xsg3_s_get_size_en );
            BCORE_REGISTER_FFUNC( bhpt_sampler_get_size_ex, bhpt_sampler_binary_xsg3_s_get_size_ex );
            BCORE_REGISTER_OBJECT( bhpt_sampler_binary_xsg3_s );
            BCORE_REGISTER_FFUNC( bhpt_sampler_fetch, bhpt_sampler_binary_hash_s_fetch );
            BCORE_REGISTER_FFUNC( bhpt_sampler_get_size_en, bhpt_sampler_binary_hash_s_get_size_en );
            BCORE_REGISTER_FFUNC( bhpt_sampler_get_size_ex, bhpt_sampler_binary_hash_s_get_size_ex );
            BCORE_REGISTER_OBJECT( bhpt_sampler_binary_hash_s );
            BCORE_REGISTER_FFUNC( bhpt_sampler_fetch, bhpt_sampler_polynom_s_fetch );
            BCORE_REGISTER_FFUNC( bhpt_sampler_get_size_en, bhpt_sampler_polynom_s_get_size_en );
            BCORE_REGISTER_FFUNC( bhpt_sampler_get_size_ex, bhpt_sampler_polynom_s_get_size_ex );
            BCORE_REGISTER_OBJECT( bhpt_sampler_polynom_s );
            BCORE_REGISTER_SPECT( bhpt_sampler );

            // group: bhpt_tutor_sampler
            BCORE_REGISTER_FFUNC( bhpt_tutor_reset, bhpt_tutor_sampler_s_reset );
            BCORE_REGISTER_FFUNC( bhpt_tutor_create_adaptive, bhpt_tutor_sampler_s_create_adaptive );
            BCORE_REGISTER_FFUNC( bhpt_tutor_prime, bhpt_tutor_sampler_s_prime );
            BCORE_REGISTER_FFUNC( bhpt_tutor_test, bhpt_tutor_sampler_s_test );
            BCORE_REGISTER_FFUNC( bhpt_tutor_status_to_sink, bhpt_tutor_sampler_s_status_to_sink );
            BCORE_REGISTER_OBJECT( bhpt_tutor_sampler_s );
            BCORE_REGISTER_TRAIT( bhpt_tutor_sampler, bcore_inst );
        }
        break;
        default: break;
    }
    return NULL;
}
