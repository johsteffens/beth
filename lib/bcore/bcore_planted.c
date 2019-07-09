/** This file was generated by the plant-compiler. Not suitable for manual editing.
 *
 *  Copyright and License:
 *    If a specification exists for the repository or folder in which the file is located,
 *    that specification shall apply. Otherwise, the Copyright and License of the associated
 *    source code files containing beth plant-code shall apply.
 */

#include "bcore_planted.h"
#include "bcore_spect.h"
#include "bcore_spect_inst.h"
#include "bcore_sr.h"
#include "bcore_const_manager.h"


/**********************************************************************************************************************/
// source: bcore_plant_sample
#include "bcore_plant_sample.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_plant_sample_signatures

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_plant_sample_features

BCORE_DEFINE_SPECT( bcore_inst, bcore_plant_sample_features )
"{"
    "bcore_spect_header_s header;"
    "feature strict aware bcore_plant_sample_features : setup;"
    "feature aware bcore_plant_sample_features : to_stdout = bcore_plant_sample_features_to_stdout__;"
    "feature aware bcore_plant_sample_features : get_size;"
"}";


void bcore_plant_sample_features_to_stdout__( const bcore_plant_sample_features* o )
{
    bcore_msg_fa( "Called to_stdout__\n" );
}
//----------------------------------------------------------------------------------------------------------------------
// group: bcore_plant_sample

BCORE_DEFINE_OBJECT_INST_P( bcore_plant_sample_object_s )
"aware bcore_inst"
"{"
    "st_s name1;"
    "sz_t => [] arr;"
    "func bcore_plant_sample_features:setup;"
    "func bcore_plant_sample_features:to_stdout;"
    "func bcore_plant_sample_features:get_size;"
"}";

void bcore_plant_sample_object_s_to_stdout( const bcore_plant_sample_object_s* o )
{ bcore_txt_ml_a_to_stdout( o ); }

BCORE_DEFINE_OBJECT_INST_P( bcore_plant_sample_object2_s )
"bcore_inst"
"{"
    "aware_t _;"
    "bcore_plant_sample_object_s object;"
    "st_s name2;"
    "func bcore_plant_sample_features:setup;"
    "func bcore_plant_sample_features:to_stdout;"
"}";

/**********************************************************************************************************************/
// source: bcore_spect_inst_call
#include "bcore_spect_inst_call.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_inst_call

BCORE_DEFINE_SPECT( bcore_inst, bcore_inst_call )
"{"
    "bcore_spect_header_s header;"
    "feature bcore_inst_call : init_x;"
    "feature bcore_inst_call : down_e;"
    "feature bcore_inst_call : copy_e;"
    "feature bcore_inst_call : copy_x;"
    "feature bcore_inst_call : discard_e;"
"}";

/**********************************************************************************************************************/
// source: bcore_spect_via_call
#include "bcore_spect_via_call.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_via_call

BCORE_DEFINE_SPECT( bcore_inst, bcore_via_call )
"{"
    "bcore_spect_header_s header;"
    "feature bcore_via_call : mutated;"
"}";

/**********************************************************************************************************************/
// source: bcore_main
#include "bcore_main.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_main

BCORE_DEFINE_OBJECT_INST_P( bcore_main_frame_s )
"aware bcore_inst"
"{"
    "aware bcore_interpreter => interpreter = bcore_txt_ml_interpreter_s;"
    "bl_t use_first_argument = true;"
    "sc_t local_file = \"beth.config\";"
    "sc_t global_file;"
"}";

BCORE_DEFINE_SPECT( bcore_inst, bcore_main )
"{"
    "bcore_spect_header_s header;"
    "feature strict bcore_main : main;"
"}";

/**********************************************************************************************************************/
// source: bcore_hmap_name
#include "bcore_hmap_name.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_hmap_name

BCORE_DEFINE_OBJECT_INST_P( bcore_hmap_name_s )
"aware bcore_inst"
"{"
    "bcore_hmap_tp_sr_s map;"
"}";

/**********************************************************************************************************************/

vd_t bcore_planted_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_planted" ) ) )
    {
        case TYPEOF_init1:
        {
            // Comment or remove line below to rebuild this target.
            bcore_const_x_set_d( typeof( "bcore_planted_hash" ), sr_tp( 723307205 ) );
            BCORE_REGISTER_TRAIT( bcore_plant_sample_signatures, bcore_inst );
            BCORE_REGISTER_FEATURE( bcore_plant_sample_features_setup );
            BCORE_REGISTER_FEATURE( bcore_plant_sample_features_to_stdout );
            BCORE_REGISTER_FFUNC( bcore_plant_sample_features_to_stdout, bcore_plant_sample_features_to_stdout__ );
            BCORE_REGISTER_FEATURE( bcore_plant_sample_features_get_size );
            BCORE_REGISTER_NAME( anyglobalname );
            BCORE_REGISTER_SPECT( bcore_plant_sample_features );
            BCORE_REGISTER_FFUNC( bcore_plant_sample_features_setup, bcore_plant_sample_object_s_setup );
            BCORE_REGISTER_FFUNC( bcore_plant_sample_features_to_stdout, bcore_plant_sample_object_s_to_stdout );
            BCORE_REGISTER_FFUNC( bcore_plant_sample_features_get_size, bcore_plant_sample_object_s_get_size );
            BCORE_REGISTER_OBJECT( bcore_plant_sample_object_s );
            BCORE_REGISTER_FFUNC( bcore_plant_sample_features_setup, bcore_plant_sample_object2_s_setup );
            BCORE_REGISTER_FFUNC( bcore_plant_sample_features_to_stdout, bcore_plant_sample_object2_s_to_stdout );
            BCORE_REGISTER_OBJECT( bcore_plant_sample_object2_s );
            BCORE_REGISTER_TRAIT( bcore_plant_sample, bcore_inst );
            BCORE_REGISTER_FEATURE( bcore_inst_call_init_x );
            BCORE_REGISTER_FEATURE( bcore_inst_call_down_e );
            BCORE_REGISTER_FEATURE( bcore_inst_call_copy_e );
            BCORE_REGISTER_FEATURE( bcore_inst_call_copy_x );
            BCORE_REGISTER_FEATURE( bcore_inst_call_discard_e );
            BCORE_REGISTER_NAME( bcore_inst_call_init_x );
            BCORE_REGISTER_NAME( bcore_inst_call_down_e );
            BCORE_REGISTER_NAME( bcore_inst_call_copy_e );
            BCORE_REGISTER_NAME( bcore_inst_call_copy_x );
            BCORE_REGISTER_NAME( bcore_inst_call_discard_e );
            BCORE_REGISTER_SPECT( bcore_inst_call );
            BCORE_REGISTER_FEATURE( bcore_via_call_mutated );
            BCORE_REGISTER_SPECT( bcore_via_call );
            BCORE_REGISTER_FEATURE( bcore_main_main );
            BCORE_REGISTER_OBJECT( bcore_main_frame_s );
            BCORE_REGISTER_SPECT( bcore_main );
            BCORE_REGISTER_OBJECT( bcore_hmap_name_s );
            BCORE_REGISTER_TRAIT( bcore_hmap_name, bcore_inst );
        }
        break;
        default: break;
    }
    return NULL;
}
