/** This file was generated from beth-plant source code.
 *  Compiling Agent : bcore_plant_compiler (C) 2019 J.B.Steffens
 *  Last File Update: 2020-04-17T16:45:00Z
 *
 *  Copyright and License of this File:
 *
 *  Generated code inherits the copyright and license of the underlying beth-plant source code.
 *  Source code defining this file is distributed across following files:
 *
 *  bhpt_sketch.h
 *  bhpt_frame.h
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

BCORE_DEFINE_OBJECT_INST_P( bhpt_hprobe_s )
"aware bcore_array"
"{"
    "bhvm_holor_s -> [];"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhpt_adaptor

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
    "feature strict aware bhpt_adaptive : get_shape_en;"
    "feature strict aware bhpt_adaptive : get_shape_ex;"
    "feature aware bhpt_adaptive : axon_pass = bhpt_adaptive_axon_pass__;"
    "feature aware bhpt_adaptive : dendrite_pass = bhpt_adaptive_dendrite_pass__;"
    "feature aware bhpt_adaptive : cyclic_reset = bhpt_adaptive_cyclic_reset__;"
    "feature aware bhpt_adaptive : get_hprobe_accugrad = bhpt_adaptive_get_hprobe_accugrad__;"
    "feature aware bhpt_adaptive : get_hprobe_adaptive = bhpt_adaptive_get_hprobe_adaptive__;"
    "feature aware bhpt_adaptive : status_to_sink = bhpt_adaptive_status_to_sink__;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhpt_builder

BCORE_DEFINE_SPECT( bhpt, bhpt_builder )
"{"
    "bcore_spect_header_s header;"
    "feature strict aware bhpt_builder : set_shape_en;"
    "feature strict aware bhpt_builder : set_shape_ex;"
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
"}";

BCORE_DEFINE_OBJECT_INST_P( bhpt_frame_s )
"aware bcore_main"
"{"
    "aware bhpt_tutor => tutor;"
    "aware bhpt_builder => builder;"
    "aware bhpt_adaptor => adaptor;"
    "sz_t cycle_adapt = 10;"
    "sz_t cycle_test = 1000;"
    "sz_t cycle_backup = 1000;"
    "sz_t cycle_finish = 1000000;"
    "sz_t verbosity;"
    "hidden bhpt_frame_state_s => state;"
    "hidden bhpt_adaptor_adl_s => adaptor_adl;"
    "st_s state_path;"
    "hidden aware bcore_sink -> log;"
    "func ^:main;"
"}";

/**********************************************************************************************************************/

vd_t bhpt_planted_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bhpt_planted" ) ) )
    {
        case TYPEOF_init1:
        {
            // Comment or remove line below to rebuild this target.
            bcore_const_x_set_d( typeof( "bhpt_planted_hash" ), sr_tp( 1294865093 ) );

            // --------------------------------------------------------------------
            // source: bhpt_sketch.h

            // group: bhpt
            BCORE_REGISTER_OBJECT( bhpt_hprobe_s );
            BCORE_REGISTER_TRAIT( bhpt, bcore_inst );

            // group: bhpt_adaptor
            BCORE_REGISTER_FEATURE( bhpt_adaptor_reset );
            BCORE_REGISTER_FEATURE( bhpt_adaptor_adapt );
            BCORE_REGISTER_OBJECT( bhpt_adaptor_adl_s );
            BCORE_REGISTER_SPECT( bhpt_adaptor );

            // group: bhpt_adaptive
            BCORE_REGISTER_FEATURE( bhpt_adaptive_get_shape_en );
            BCORE_REGISTER_FEATURE( bhpt_adaptive_get_shape_ex );
            BCORE_REGISTER_FEATURE( bhpt_adaptive_axon_pass );
            BCORE_REGISTER_FFUNC( bhpt_adaptive_axon_pass, bhpt_adaptive_axon_pass__ );
            BCORE_REGISTER_FEATURE( bhpt_adaptive_dendrite_pass );
            BCORE_REGISTER_FFUNC( bhpt_adaptive_dendrite_pass, bhpt_adaptive_dendrite_pass__ );
            BCORE_REGISTER_FEATURE( bhpt_adaptive_cyclic_reset );
            BCORE_REGISTER_FFUNC( bhpt_adaptive_cyclic_reset, bhpt_adaptive_cyclic_reset__ );
            BCORE_REGISTER_FEATURE( bhpt_adaptive_get_hprobe_accugrad );
            BCORE_REGISTER_FFUNC( bhpt_adaptive_get_hprobe_accugrad, bhpt_adaptive_get_hprobe_accugrad__ );
            BCORE_REGISTER_FEATURE( bhpt_adaptive_get_hprobe_adaptive );
            BCORE_REGISTER_FFUNC( bhpt_adaptive_get_hprobe_adaptive, bhpt_adaptive_get_hprobe_adaptive__ );
            BCORE_REGISTER_FEATURE( bhpt_adaptive_status_to_sink );
            BCORE_REGISTER_FFUNC( bhpt_adaptive_status_to_sink, bhpt_adaptive_status_to_sink__ );
            BCORE_REGISTER_SPECT( bhpt_adaptive );

            // group: bhpt_builder
            BCORE_REGISTER_FEATURE( bhpt_builder_set_shape_en );
            BCORE_REGISTER_FEATURE( bhpt_builder_set_shape_ex );
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
            BCORE_REGISTER_SPECT( bhpt_tutor );

            // --------------------------------------------------------------------
            // source: bhpt_frame.h

            // group: bhpt_frame
            BCORE_REGISTER_OBJECT( bhpt_frame_state_s );
            BCORE_REGISTER_FFUNC( bcore_main_main, bhpt_frame_s_main );
            BCORE_REGISTER_OBJECT( bhpt_frame_s );
            BCORE_REGISTER_TRAIT( bhpt_frame, bcore_inst );
        }
        break;
        default: break;
    }
    return NULL;
}