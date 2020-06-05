/** This file was generated from beth-plant source code.
 *  Compiling Agent : bcore_plant_compiler (C) 2019, 2020 J.B.Steffens
 *  Last File Update: 2020-06-04T15:36:35Z
 *
 *  Copyright and License of this File:
 *
 *  Generated code inherits the copyright and license of the underlying beth-plant source code.
 *  Source code defining this file is distributed across following files:
 *
 *  bcore_file.h
 *  bcore_plant_builder.h
 *  bcore_plant_inexpandable.h
 *  bcore_plant_sample.h
 *  bcore_spect_inst_call.h
 *  bcore_spect_via_call.h
 *  bcore_main.h
 *  bcore_hmap_name.h
 *  bcore_cday.h
 *  bcore_rand.h
 *  bcore_error_manager.h
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
// source: bcore_plant_builder.h
#include "bcore_plant_builder.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_plant_builder

BCORE_DEFINE_OBJECT_INST_P( bcore_plant_builder_target_s )
"aware bcore_plant_builder"
"{"
    "st_s => name;"
    "st_s => root;"
    "bcore_arr_st_s dependencies;"
    "bcore_arr_st_s sources;"
    "st_s => signal_handler;"
    "func bcore_via_call:source;"
    "func ^:build;"
"}";

void bcore_plant_builder_target_s_source( bcore_plant_builder_target_s* o, bcore_source* source )
{
    if( !o->root )
    {
        o->root = bcore_file_folder_path( bcore_source_a_get_file( source ) );
        st_s_attach( &o->root, bcore_file_path_minimized( o->root->sc ) );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bcore_plant_builder_target_adl_s )
"aware bcore_array"
"{"
    "bcore_plant_builder_target_s => [];"
"}";

BCORE_DEFINE_SPECT( bcore_inst, bcore_plant_builder )
"{"
    "bcore_spect_header_s header;"
    "feature aware bcore_plant_builder : build;"
"}";

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

BCORE_DEFINE_OBJECT_INST_P( bcore_main_arr_s )
"aware bcore_array"
"{"
    "aware bcore_main* [];"
"}";

BCORE_DEFINE_OBJECT_INST_P( bcore_main_set_s )
"aware bcore_main"
"{"
    "bcore_main_arr_s arr;"
    "func ^:main;"
"}";

s2_t bcore_main_set_s_main( bcore_main_set_s* o, const bcore_arr_st_s* args )
{
    s2_t r = 0;
    BFOR_EACH( i, &o->arr ) { if( ( r = bcore_main_a_main( o->arr.data[ i ], args ) ) ) break; }
    return r;
}

BCORE_DEFINE_SPECT( bcore_inst, bcore_main )
"{"
    "bcore_spect_header_s header;"
    "feature strict aware bcore_main : main;"
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
// source: bcore_rand.h
#include "bcore_rand.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_rand

BCORE_DEFINE_OBJECT_INST_P( bcore_rand_lcg_s )
"aware bcore_rand"
"{"
    "u2_t state = 16432;"
    "func ^:max_u3;"
    "func ^:min_u3;"
    "func ^:gen_u3;"
    "func ^:gen_f3;"
    "func ^:reseed;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bcore_rand_xsg_s )
"aware bcore_rand"
"{"
    "u2_t state = 13754;"
    "func ^:max_u3;"
    "func ^:min_u3;"
    "func ^:gen_u3;"
    "func ^:gen_f3;"
    "func ^:reseed;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bcore_rand_lxm_s )
"aware bcore_rand"
"{"
    "u3_t l0 = 27458;"
    "u3_t l1 = 33296;"
    "u3_t x0 = 23532;"
    "u3_t x1 = 10954;"
"}";

BCORE_DEFINE_SPECT( bcore_inst, bcore_rand )
"{"
    "bcore_spect_header_s header;"
    "feature aware bcore_rand : max_u3;"
    "feature aware bcore_rand : min_u3;"
    "feature aware bcore_rand : gen_u3;"
    "feature aware bcore_rand : gen_f3;"
    "feature aware bcore_rand : reseed;"
"}";

/**********************************************************************************************************************/
// source: bcore_error_manager.h
#include "bcore_error_manager.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_error_manager

BCORE_DEFINE_OBJECT_INST_P( bcore_error_manager_error_s )
"aware bcore_error_manager"
"{"
    "er_t id;"
    "st_s msg;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bcore_error_manager_error_adl_s )
"aware bcore_array"
"{"
    "bcore_error_manager_error_s => [];"
"}";

BCORE_DEFINE_OBJECT_INST_P( bcore_error_manager_context_s )
"aware bcore_error_manager"
"{"
    "bcore_error_manager_error_adl_s adl;"
    "bcore_mutex_s mutex;"
"}";

/**********************************************************************************************************************/


vd_t bcore_planted_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_planted" ) ) )
    {
        case TYPEOF_init1:
        {

            // --------------------------------------------------------------------
            // source: bcore_file.h

            // group: bcore_file
            BCORE_REGISTER_FFUNC( bcore_via_call_source, bcore_file_path_s_source );
            BCORE_REGISTER_OBJECT( bcore_file_path_s );
            BCORE_REGISTER_TRAIT( bcore_file, bcore_inst );

            // --------------------------------------------------------------------
            // source: bcore_plant_builder.h

            // group: bcore_plant_builder
            BCORE_REGISTER_FEATURE( bcore_plant_builder_build );
            BCORE_REGISTER_FFUNC( bcore_via_call_source, bcore_plant_builder_target_s_source );
            BCORE_REGISTER_FFUNC( bcore_plant_builder_build, bcore_plant_builder_target_s_build );
            BCORE_REGISTER_OBJECT( bcore_plant_builder_target_s );
            BCORE_REGISTER_OBJECT( bcore_plant_builder_target_adl_s );
            BCORE_REGISTER_SPECT( bcore_plant_builder );

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
            BCORE_REGISTER_OBJECT( bcore_main_arr_s );
            BCORE_REGISTER_FFUNC( bcore_main_main, bcore_main_set_s_main );
            BCORE_REGISTER_OBJECT( bcore_main_set_s );
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

            // --------------------------------------------------------------------
            // source: bcore_rand.h

            // group: bcore_rand
            BCORE_REGISTER_FEATURE( bcore_rand_max_u3 );
            BCORE_REGISTER_FEATURE( bcore_rand_min_u3 );
            BCORE_REGISTER_FEATURE( bcore_rand_gen_u3 );
            BCORE_REGISTER_FEATURE( bcore_rand_gen_f3 );
            BCORE_REGISTER_FEATURE( bcore_rand_reseed );
            BCORE_REGISTER_FFUNC( bcore_rand_max_u3, bcore_rand_lcg_s_max_u3 );
            BCORE_REGISTER_FFUNC( bcore_rand_min_u3, bcore_rand_lcg_s_min_u3 );
            BCORE_REGISTER_FFUNC( bcore_rand_gen_u3, bcore_rand_lcg_s_gen_u3 );
            BCORE_REGISTER_FFUNC( bcore_rand_gen_f3, bcore_rand_lcg_s_gen_f3 );
            BCORE_REGISTER_FFUNC( bcore_rand_reseed, bcore_rand_lcg_s_reseed );
            BCORE_REGISTER_OBJECT( bcore_rand_lcg_s );
            BCORE_REGISTER_FFUNC( bcore_rand_max_u3, bcore_rand_xsg_s_max_u3 );
            BCORE_REGISTER_FFUNC( bcore_rand_min_u3, bcore_rand_xsg_s_min_u3 );
            BCORE_REGISTER_FFUNC( bcore_rand_gen_u3, bcore_rand_xsg_s_gen_u3 );
            BCORE_REGISTER_FFUNC( bcore_rand_gen_f3, bcore_rand_xsg_s_gen_f3 );
            BCORE_REGISTER_FFUNC( bcore_rand_reseed, bcore_rand_xsg_s_reseed );
            BCORE_REGISTER_OBJECT( bcore_rand_xsg_s );
            BCORE_REGISTER_OBJECT( bcore_rand_lxm_s );
            BCORE_REGISTER_SPECT( bcore_rand );

            // --------------------------------------------------------------------
            // source: bcore_error_manager.h

            // group: bcore_error_manager
            BCORE_REGISTER_OBJECT( bcore_error_manager_error_s );
            BCORE_REGISTER_OBJECT( bcore_error_manager_error_adl_s );
            BCORE_REGISTER_OBJECT( bcore_error_manager_context_s );
            BCORE_REGISTER_NAME( general_error );
            BCORE_REGISTER_NAME( parse_error );
            BCORE_REGISTER_NAME( plant_error );
            BCORE_REGISTER_NAME( error_stack );
            BCORE_REGISTER_TRAIT( bcore_error_manager, bcore_inst );
        }
        break;
        default: break;
    }
    return NULL;
}
// BETH_PLANT_SIGNATURE  795136520
