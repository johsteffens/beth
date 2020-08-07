/** This file was generated from beth-plant source code.
 *  Compiling Agent : xoico_compiler (C) 2020 J.B.Steffens
 *  Last File Update: 2020-08-07T21:23:06Z
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
 *  bhpt_tutor_language.h
 *
 */

#include "bhpt_xoila_out.h"
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
    "private bhvm_holor_s* axon;"
    "private bhvm_holor_s* grad;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhpt_adaptor_probe_s )
"aware bcore_array"
"{"
    "bhpt_adaptor_node_s [];"
"}";

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
    "feature aware bhpt_adaptive : rebind_holors;"
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
    "feature strict aware bhpt_tutor : create_adaptor;"
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

BCORE_DEFINE_OBJECT_INST_P( bhpt_frame_s )
"aware bcore_main"
"{"
    "aware bhpt_tutor => tutor;"
    "hidden bhpt_frame_thread_base_s => thread_base;"
    "private bcore_main_frame_s* main_frame;"
    "sz_t threads = 1;"
    "sz_t cycle_adapt = 1;"
    "sz_t cycle_test = 1000;"
    "sz_t cycle_backup = 1000;"
    "sz_t cycle_finish = 1000000;"
    "sz_t verbosity = 1;"
    "hidden bhpt_frame_state_s => state;"
    "hidden bhvm_stats_s stats_grad;"
    "st_s state_path;"
    "hidden aware bcore_sink -> log;"
    "func ^:main;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhpt_frame_thread

BCORE_DEFINE_OBJECT_INST_P( bhpt_frame_thread_share_s )
"aware bhpt_frame_thread"
"{"
    "aware bhpt_tutor -> tutor;"
    "aware bhpt_adaptive -> adaptive;"
    "bcore_condition_s => condition_item;"
    "bcore_mutex_s => mutex;"
    "sz_t finished_count = 0;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhpt_frame_thread_item_s )
"aware bhpt_frame_thread"
"{"
    "bl_t running = false;"
    "sz_t prime_cycles = 0;"
    "bcore_thread_s => thread;"
    "bcore_mutex_s => mutex;"
    "bhpt_frame_thread_share_s -> share;"
    "aware bhpt_adaptive => adaptive;"
    "func bcore_inst_call:down_e;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhpt_frame_thread_ads_s )
"aware bcore_array"
"{"
    "bhpt_frame_thread_item_s [];"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhpt_frame_thread_base_s )
"aware bhpt_frame_thread"
"{"
    "bhpt_frame_thread_ads_s ads;"
    "bhpt_frame_thread_share_s => share;"
    "func bcore_inst_call:down_e;"
    "func bcore_inst_call:copy_e;"
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
}

BCORE_DEFINE_OBJECT_INST_P( bhpt_adaptor_reg_l2_s )
"aware bhpt_adaptor"
"{"
    "f3_t lambda;"
    "func ^:reset;"
    "func ^:adapt;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhpt_adaptor_zro_grad_s )
"aware bhpt_adaptor"
"{"
    "func ^:adapt;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhpt_adaptor_list_s )
"aware bcore_array"
"{"
    "aware bhpt_adaptor => [];"
    "func bhpt_adaptor:reset;"
    "func bhpt_adaptor:adapt;"
"}";

void bhpt_adaptor_list_s_reset( bhpt_adaptor_list_s* o )
{
    BFOR_SIZE( i, o->size ) bhpt_adaptor_a_reset( o->data[ i ] );
}

void bhpt_adaptor_list_s_adapt( bhpt_adaptor_list_s* o, const bhpt_adaptor_node_s* node )
{
    BFOR_SIZE( i, o->size ) bhpt_adaptor_a_adapt( o->data[ i ], node );
}

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

BCORE_DEFINE_OBJECT_INST_P( bhpt_sampler_binary_lcg00_s )
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
    "aware bhpt_builder => builder;"
    "aware bhpt_adaptor => adaptor;"
    "aware bhpt_sampler => sampler;"
    "aware bcore_prsg => prsg_priming = bcore_prsg_lcg_u3_00_s;"
    "aware bcore_prsg => prsg_testing = bcore_prsg_lcg_u3_01_s;"
    "sz_t test_size = 1000;"
    "hidden bcore_mutex_s mutex;"
    "func ^:reset;"
    "func ^:create_adaptive;"
    "func ^:create_adaptor;"
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
// source: bhpt_tutor_language.h
#include "bhpt_tutor_language.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bhpt_tutor_language

BCORE_DEFINE_OBJECT_INST_P( bhpt_tutor_language_utf8_chatter_s )
"aware bhpt_tutor_language"
"{"
    "bl_t cyclic_reset = false;"
    "st_s trigger = \"The chatter triggerbhpt_tutor_language\";"
    "sz_t size_line = 100;"
    "sz_t size_lines = 4;"
    "bl_t newline_to_space = true;"
    "f3_t heat = 0.2;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhpt_tutor_language_utf8_s )
"aware bhpt_tutor"
"{"
    "aware bhpt_builder => builder;"
    "aware bhpt_adaptor => adaptor;"
    "aware => src;"
    "hidden st_s => st;"
    "aware bcore_prsg => prsg_priming = bcore_prsg_lcg_u3_00_s;"
    "aware bcore_prsg => prsg_testing = bcore_prsg_lcg_u3_01_s;"
    "sz_t size_trans = 20;"
    "sz_t size_prime = 200;"
    "sz_t size_test = 1000;"
    "f3_t tgt_pos = 1.0;"
    "f3_t tgt_neg = 0.0;"
    "bhpt_tutor_language_utf8_chatter_s => chatter;"
    "hidden bcore_mutex_s mutex;"
    "func ^:reset;"
    "func ^:create_adaptive;"
    "func ^:create_adaptor;"
    "func ^:prime;"
    "func ^:test;"
    "func ^:status_to_sink;"
"}";

void bhpt_tutor_language_utf8_s_status_to_sink( const bhpt_tutor_language_utf8_s* o, sz_t verbosity, bcore_sink* sink )
{
    if( verbosity > 0 )
    {
        bcore_sink_a_push_fa( sink, "#<sc_t>", ifnameof( o->_ ) );
    }
}

/**********************************************************************************************************************/

vd_t bhvm_general_signal_handler( const bcore_signal_s* o );

vd_t bhpt_xoila_out_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bhpt_xoila_out" ) ) )
    {
        case TYPEOF_init1:
        {

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
            BCORE_REGISTER_FEATURE( bhpt_adaptive_rebind_holors );
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
            BCORE_REGISTER_FEATURE( bhpt_tutor_create_adaptor );
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
            BCORE_REGISTER_FFUNC( bcore_main_main, bhpt_frame_s_main );
            BCORE_REGISTER_OBJECT( bhpt_frame_s );
            BCORE_REGISTER_TRAIT( bhpt_frame, bcore_inst );

            // group: bhpt_frame_thread
            BCORE_REGISTER_OBJECT( bhpt_frame_thread_share_s );
            BCORE_REGISTER_FFUNC( bcore_inst_call_down_e, bhpt_frame_thread_item_s_down_e );
            BCORE_REGISTER_OBJECT( bhpt_frame_thread_item_s );
            BCORE_REGISTER_OBJECT( bhpt_frame_thread_ads_s );
            BCORE_REGISTER_FFUNC( bcore_inst_call_down_e, bhpt_frame_thread_base_s_down_e );
            BCORE_REGISTER_FFUNC( bcore_inst_call_copy_e, bhpt_frame_thread_base_s_copy_e );
            BCORE_REGISTER_OBJECT( bhpt_frame_thread_base_s );
            BCORE_REGISTER_TRAIT( bhpt_frame_thread, bhpt_frame );

            // --------------------------------------------------------------------
            // source: bhpt_adaptor.h

            // group: bhpt_adaptor_stamp
            BCORE_REGISTER_FFUNC( bhpt_adaptor_reset, bhpt_adaptor_epsilon_s_reset );
            BCORE_REGISTER_FFUNC( bhpt_adaptor_adapt, bhpt_adaptor_epsilon_s_adapt );
            BCORE_REGISTER_OBJECT( bhpt_adaptor_epsilon_s );
            BCORE_REGISTER_FFUNC( bhpt_adaptor_reset, bhpt_adaptor_reg_l2_s_reset );
            BCORE_REGISTER_FFUNC( bhpt_adaptor_adapt, bhpt_adaptor_reg_l2_s_adapt );
            BCORE_REGISTER_OBJECT( bhpt_adaptor_reg_l2_s );
            BCORE_REGISTER_FFUNC( bhpt_adaptor_adapt, bhpt_adaptor_zro_grad_s_adapt );
            BCORE_REGISTER_OBJECT( bhpt_adaptor_zro_grad_s );
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
            BCORE_REGISTER_FFUNC( bhpt_sampler_fetch, bhpt_sampler_binary_lcg00_s_fetch );
            BCORE_REGISTER_FFUNC( bhpt_sampler_get_size_en, bhpt_sampler_binary_lcg00_s_get_size_en );
            BCORE_REGISTER_FFUNC( bhpt_sampler_get_size_ex, bhpt_sampler_binary_lcg00_s_get_size_ex );
            BCORE_REGISTER_OBJECT( bhpt_sampler_binary_lcg00_s );
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
            BCORE_REGISTER_FFUNC( bhpt_tutor_create_adaptor, bhpt_tutor_sampler_s_create_adaptor );
            BCORE_REGISTER_FFUNC( bhpt_tutor_prime, bhpt_tutor_sampler_s_prime );
            BCORE_REGISTER_FFUNC( bhpt_tutor_test, bhpt_tutor_sampler_s_test );
            BCORE_REGISTER_FFUNC( bhpt_tutor_status_to_sink, bhpt_tutor_sampler_s_status_to_sink );
            BCORE_REGISTER_OBJECT( bhpt_tutor_sampler_s );
            BCORE_REGISTER_TRAIT( bhpt_tutor_sampler, bcore_inst );

            // --------------------------------------------------------------------
            // source: bhpt_tutor_language.h

            // group: bhpt_tutor_language
            BCORE_REGISTER_OBJECT( bhpt_tutor_language_utf8_chatter_s );
            BCORE_REGISTER_FFUNC( bhpt_tutor_reset, bhpt_tutor_language_utf8_s_reset );
            BCORE_REGISTER_FFUNC( bhpt_tutor_create_adaptive, bhpt_tutor_language_utf8_s_create_adaptive );
            BCORE_REGISTER_FFUNC( bhpt_tutor_create_adaptor, bhpt_tutor_language_utf8_s_create_adaptor );
            BCORE_REGISTER_FFUNC( bhpt_tutor_prime, bhpt_tutor_language_utf8_s_prime );
            BCORE_REGISTER_FFUNC( bhpt_tutor_test, bhpt_tutor_language_utf8_s_test );
            BCORE_REGISTER_FFUNC( bhpt_tutor_status_to_sink, bhpt_tutor_language_utf8_s_status_to_sink );
            BCORE_REGISTER_OBJECT( bhpt_tutor_language_utf8_s );
            BCORE_REGISTER_TRAIT( bhpt_tutor_language, bcore_inst );
        }
        break;
        case TYPEOF_push_dependencies:
        {
            ASSERT( o->object && ( *( aware_t* )o->object ) == TYPEOF_bcore_arr_fp_s );
            bcore_arr_fp_s* arr_fp = o->object;
            bcore_arr_fp_s_push( arr_fp, ( fp_t )bhvm_general_signal_handler );
        }
        break;
        default: break;
    }
    return NULL;
}
// BETH_PLANT_SIGNATURE 6003993111710006621
