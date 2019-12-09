/** This file was generated from beth-plant source code.
 *  Compiling Agent : bcore_plant_compiler (C) 2019 J.B.Steffens
 *  Last File Update: 2019-12-09T11:12:48Z
 *
 *  Copyright and License of this File:
 *
 *  Generated code inherits the copyright and license of the underlying beth-plant source code.
 *  Source code defining this file is distributed across following files:
 *
 *  bcore_file.h
 *  bcore_plant_inexpandable.h
 *  bcore_plant_sample.h
 *  bcore_spect_inst_call.h
 *  bcore_spect_via_call.h
 *  bcore_main.h
 *  bcore_hmap_name.h
 *  bcore_cday.h
 *
 */

#include "bcore_planted.h"
#include "bcore_spect.h"
#include "bcore_spect_inst.h"
#include "bcore_sr.h"
#include "bcore_const_manager.h"


/**********************************************************************************************************************/
// source: bcore_file.h
#include "bcore_file.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_file

BCORE_DEFINE_OBJECT_INST_P( bcore_file_path_s )
"aware bcore_inst"
"{"
    "st_s name;"
    "hidden st_s => root;"
    "hidden st_s => full;"
    "func bcore_via_call:source;"
"}";

void bcore_file_path_s_set_sc( bcore_file_path_s* o, sc_t name )
{
    st_s_detach( &o->full );
    st_s_copy_sc( &o->name, name );
    if( name[ 0 ] != '/' && o->root != NULL )
    {
        o->full = st_s_create_fa( "#<sc_t>/#<sc_t>", o->root->sc, o->name.sc );
    }
}

/**********************************************************************************************************************/
// source: bcore_plant_inexpandable.h
#include "bcore_plant_inexpandable.h"

/**********************************************************************************************************************/
// source: bcore_plant_sample.h
#include "bcore_plant_sample.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_plant_sample

BCORE_DEFINE_OBJECT_INST_P( bcore_plant_sample_object2_s )
"bcore_inst"
"{"
    "aware_t _;"
    "bcore_plant_sample_features_object_s object;"
    "st_s name2;"
    "func bcore_plant_sample_features:setup;"
    "func bcore_plant_sample_features:to_stdout;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_plant_sample_signatures

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_plant_sample_features

BCORE_DEFINE_OBJECT_INST_P( bcore_plant_sample_features_object_s )
"aware bcore_inst"
"{"
    "st_s name1;"
    "sz_t => [] arr;"
    "func bcore_plant_sample_features:setup;"
    "func bcore_plant_sample_features:to_stdout;"
    "func bcore_plant_sample_features:get_size;"
"}";

BCORE_DEFINE_SPECT( bcore_plant_sample, bcore_plant_sample_features )
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
/**********************************************************************************************************************/
// source: bcore_spect_inst_call.h
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
// source: bcore_spect_via_call.h
#include "bcore_spect_via_call.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_via_call

BCORE_DEFINE_SPECT( bcore_inst, bcore_via_call )
"{"
    "bcore_spect_header_s header;"
    "feature bcore_via_call : source;"
    "feature bcore_via_call : mutated;"
    "feature bcore_via_call : shelve;"
"}";

/**********************************************************************************************************************/
// source: bcore_main.h
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
// source: bcore_hmap_name.h
#include "bcore_hmap_name.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_hmap_name

BCORE_DEFINE_OBJECT_INST_P( bcore_hmap_name_s )
"aware bcore_inst"
"{"
    "bcore_hmap_tp_sr_s map;"
"}";

/**********************************************************************************************************************/
// source: bcore_cday.h
#include "bcore_cday.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_cday

BCORE_DEFINE_OBJECT_INST_P( bcore_cday_ymd_s )
"aware bcore_cday"
"{"
    "s2_t y;"
    "s2_t m;"
    "s2_t d;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bcore_cday_utc_s )
"aware bcore_cday"
"{"
    "s2_t cday;"
    "s2_t ms;"
"}";

/**********************************************************************************************************************/

vd_t bcore_planted_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_planted" ) ) )
    {
        case TYPEOF_init1:
        {
            // Comment or remove line below to rebuild this target.
            bcore_const_x_set_d( typeof( "bcore_planted_hash" ), sr_tp( 2497427137 ) );

            // --------------------------------------------------------------------
            // source: bcore_file.h

            // group: bcore_file
            BCORE_REGISTER_FFUNC( bcore_via_call_source, bcore_file_path_s_source );
            BCORE_REGISTER_OBJECT( bcore_file_path_s );
            BCORE_REGISTER_TRAIT( bcore_file, bcore_inst );

            // --------------------------------------------------------------------
            // source: bcore_plant_inexpandable.h

            // --------------------------------------------------------------------
            // source: bcore_plant_sample.h

            // group: bcore_plant_sample
            BCORE_REGISTER_NAME( anyglobalname );
            BCORE_REGISTER_FFUNC( bcore_plant_sample_features_setup, bcore_plant_sample_object2_s_setup );
            BCORE_REGISTER_FFUNC( bcore_plant_sample_features_to_stdout, bcore_plant_sample_object2_s_to_stdout );
            BCORE_REGISTER_OBJECT( bcore_plant_sample_object2_s );
            BCORE_REGISTER_TRAIT( bcore_plant_sample, bcore_inst );

            // group: bcore_plant_sample_signatures
            BCORE_REGISTER_TRAIT( bcore_plant_sample_signatures, bcore_plant_sample );

            // group: bcore_plant_sample_features
            BCORE_REGISTER_FEATURE( bcore_plant_sample_features_setup );
            BCORE_REGISTER_FEATURE( bcore_plant_sample_features_to_stdout );
            BCORE_REGISTER_FFUNC( bcore_plant_sample_features_to_stdout, bcore_plant_sample_features_to_stdout__ );
            BCORE_REGISTER_FEATURE( bcore_plant_sample_features_get_size );
            BCORE_REGISTER_FFUNC( bcore_plant_sample_features_setup, bcore_plant_sample_features_object_s_setup );
            BCORE_REGISTER_FFUNC( bcore_plant_sample_features_to_stdout, bcore_plant_sample_features_object_s_to_stdout );
            BCORE_REGISTER_FFUNC( bcore_plant_sample_features_get_size, bcore_plant_sample_features_object_s_get_size );
            BCORE_REGISTER_OBJECT( bcore_plant_sample_features_object_s );
            BCORE_REGISTER_SPECT( bcore_plant_sample_features );

            // --------------------------------------------------------------------
            // source: bcore_spect_inst_call.h

            // group: bcore_inst_call
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

            // --------------------------------------------------------------------
            // source: bcore_spect_via_call.h

            // group: bcore_via_call
            BCORE_REGISTER_FEATURE( bcore_via_call_source );
            BCORE_REGISTER_FEATURE( bcore_via_call_mutated );
            BCORE_REGISTER_FEATURE( bcore_via_call_shelve );
            BCORE_REGISTER_SPECT( bcore_via_call );

            // --------------------------------------------------------------------
            // source: bcore_main.h

            // group: bcore_main
            BCORE_REGISTER_FEATURE( bcore_main_main );
            BCORE_REGISTER_OBJECT( bcore_main_frame_s );
            BCORE_REGISTER_SPECT( bcore_main );

            // --------------------------------------------------------------------
            // source: bcore_hmap_name.h

            // group: bcore_hmap_name
            BCORE_REGISTER_OBJECT( bcore_hmap_name_s );
            BCORE_REGISTER_TRAIT( bcore_hmap_name, bcore_inst );

            // --------------------------------------------------------------------
            // source: bcore_cday.h

            // group: bcore_cday
            BCORE_REGISTER_OBJECT( bcore_cday_ymd_s );
            BCORE_REGISTER_OBJECT( bcore_cday_utc_s );
            BCORE_REGISTER_TRAIT( bcore_cday, bcore_inst );
        }
        break;
        default: break;
    }
    return NULL;
}
