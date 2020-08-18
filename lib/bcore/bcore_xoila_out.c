/** This file was generated from xoila source code.
 *  Compiling Agent : xoico_compiler (C) 2020 J.B.Steffens
 *  Last File Update: 2020-08-18T11:24:40Z
 *
 *  Copyright and License of this File:
 *
 *  Generated code inherits the copyright and license of the underlying xoila source code.
 *  Source code defining this file is distributed across following files:
 *
 *  bcore_file.h
 *  bcore_xoila_inexpandable.h
 *  bcore_spect_inst_call.h
 *  bcore_spect_via_call.h
 *  bcore_main.h
 *  bcore_hmap_name.h
 *  bcore_cday.h
 *  bcore_error_manager.h
 *  bcore_prsg.h
 *
 */

#include "bcore_xoila_out.h"
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
// source: bcore_xoila_inexpandable.h
#include "bcore_xoila_inexpandable.h"

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
    "bcore_arr_st_s args;"
    "hidden bcore_mutex_s mutex;"
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

er_t bcore_main_set_s_main( bcore_main_set_s* o, bcore_main_frame_s* frame )
{
    er_t r = 0;
    BFOR_EACH( i, &o->arr )
    {
        if( ( r = bcore_main_a_main( o->arr.data[ i ], frame ) ) ) break;
    }
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
// source: bcore_prsg.h
#include "bcore_prsg.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_prsg

BCORE_DEFINE_SPECT( bcore_inst, bcore_prsg )
"{"
    "bcore_spect_header_s header;"
    "feature strict aware bcore_prsg : bits;"
    "feature strict aware bcore_prsg : max_u3;"
    "feature strict aware bcore_prsg : min_u3;"
    "feature strict aware bcore_prsg : state_u3;"
    "feature strict aware bcore_prsg : state_bits_u3;"
    "feature strict aware bcore_prsg : state_f3;"
    "feature aware bcore_prsg : state_bl = bcore_prsg_state_bl__;"
    "feature strict aware bcore_prsg : gen;"
    "feature strict aware bcore_prsg : gen_bits_u3;"
    "feature strict aware bcore_prsg : gen_u3;"
    "feature strict aware bcore_prsg : gen_f3;"
    "feature aware bcore_prsg : gen_bl = bcore_prsg_gen_bl__;"
    "feature strict aware bcore_prsg : set_state_u3;"
    "feature aware bcore_prsg : set_state_mix = bcore_prsg_set_state_mix__;"
    "feature aware bcore_prsg : reseed = bcore_prsg_reseed__;"
"}";


bcore_prsg* bcore_prsg_set_state_mix__( bcore_prsg* o, const bcore_prsg* a, const bcore_prsg* b )
{
    /* Different mixing methods are thinkable:
     * Adding, multiplying or xoring should all be suitable.
     * We should just stick to the same method.
     */
    return bcore_prsg_a_set_state_u3( o, bcore_prsg_a_state_u3( a ) + bcore_prsg_a_state_u3( b ) );
}
//----------------------------------------------------------------------------------------------------------------------
// group: bcore_prsg_lcg

BCORE_DEFINE_OBJECT_INST_P( bcore_prsg_lcg_u2_00_s )
"aware bcore_prsg"
"{"
    "func ^:gen_u3;"
    "func ^:state_bits_u3;"
    "func ^:gen_bits_u3;"
    "func ^:state_f3;"
    "func ^:gen_f3;"
    "u2_t state = 16437;"
    "func ^:bits;"
    "func ^:max_u3;"
    "func ^:min_u3;"
    "func ^:state_u3;"
    "func ^:set_state_u3;"
    "func ^:gen;"
"}";

u3_t bcore_prsg_lcg_u2_00_s_gen_u3( bcore_prsg_lcg_u2_00_s* o )
{
    bcore_prsg_lcg_u2_00_s_gen( o );
    return bcore_prsg_lcg_u2_00_s_state_u3( o );
}

u3_t bcore_prsg_lcg_u2_00_s_state_bits_u3( const bcore_prsg_lcg_u2_00_s* o, sz_t bits )
{
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_lcg_u2_00_s_bits( o );
    if( o_bits >= bits )
    {
        return bcore_prsg_lcg_u2_00_s_state_u3( o ) >> ( o_bits - bits );
    }
    else
    {
        return bcore_prsg_lcg_u2_00_s_state_u3( o ) << ( bits - o_bits );
    }
}

u3_t bcore_prsg_lcg_u2_00_s_gen_bits_u3( bcore_prsg_lcg_u2_00_s* o, sz_t bits )
{
    bcore_prsg_lcg_u2_00_s_gen( o );
    return bcore_prsg_lcg_u2_00_s_state_bits_u3( o, bits );
}

f3_t bcore_prsg_lcg_u2_00_s_state_f3( const bcore_prsg_lcg_u2_00_s* o, f3_t min, f3_t max )
{
    return min + ( max - min ) * ( ( f3_t )( bcore_prsg_lcg_u2_00_s_state_u3( o ) - bcore_prsg_lcg_u2_00_s_min_u3( o ) ) / ( bcore_prsg_lcg_u2_00_s_max_u3( o ) - bcore_prsg_lcg_u2_00_s_min_u3( o ) ) );
}

f3_t bcore_prsg_lcg_u2_00_s_gen_f3( bcore_prsg_lcg_u2_00_s* o, f3_t min, f3_t max )
{
    bcore_prsg_lcg_u2_00_s_gen( o );
    return bcore_prsg_lcg_u2_00_s_state_f3( o, min, max );
}

BCORE_DEFINE_OBJECT_INST_P( bcore_prsg_lcg_u2_01_s )
"aware bcore_prsg"
"{"
    "func ^:gen_u3;"
    "func ^:state_bits_u3;"
    "func ^:gen_bits_u3;"
    "func ^:state_f3;"
    "func ^:gen_f3;"
    "u2_t state = 16437;"
    "func ^:bits;"
    "func ^:max_u3;"
    "func ^:min_u3;"
    "func ^:state_u3;"
    "func ^:set_state_u3;"
    "func ^:gen;"
"}";

u3_t bcore_prsg_lcg_u2_01_s_gen_u3( bcore_prsg_lcg_u2_01_s* o )
{
    bcore_prsg_lcg_u2_01_s_gen( o );
    return bcore_prsg_lcg_u2_01_s_state_u3( o );
}

u3_t bcore_prsg_lcg_u2_01_s_state_bits_u3( const bcore_prsg_lcg_u2_01_s* o, sz_t bits )
{
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_lcg_u2_01_s_bits( o );
    if( o_bits >= bits )
    {
        return bcore_prsg_lcg_u2_01_s_state_u3( o ) >> ( o_bits - bits );
    }
    else
    {
        return bcore_prsg_lcg_u2_01_s_state_u3( o ) << ( bits - o_bits );
    }
}

u3_t bcore_prsg_lcg_u2_01_s_gen_bits_u3( bcore_prsg_lcg_u2_01_s* o, sz_t bits )
{
    bcore_prsg_lcg_u2_01_s_gen( o );
    return bcore_prsg_lcg_u2_01_s_state_bits_u3( o, bits );
}

f3_t bcore_prsg_lcg_u2_01_s_state_f3( const bcore_prsg_lcg_u2_01_s* o, f3_t min, f3_t max )
{
    return min + ( max - min ) * ( ( f3_t )( bcore_prsg_lcg_u2_01_s_state_u3( o ) - bcore_prsg_lcg_u2_01_s_min_u3( o ) ) / ( bcore_prsg_lcg_u2_01_s_max_u3( o ) - bcore_prsg_lcg_u2_01_s_min_u3( o ) ) );
}

f3_t bcore_prsg_lcg_u2_01_s_gen_f3( bcore_prsg_lcg_u2_01_s* o, f3_t min, f3_t max )
{
    bcore_prsg_lcg_u2_01_s_gen( o );
    return bcore_prsg_lcg_u2_01_s_state_f3( o, min, max );
}

BCORE_DEFINE_OBJECT_INST_P( bcore_prsg_lcg_u2_02_s )
"aware bcore_prsg"
"{"
    "func ^:gen_u3;"
    "func ^:state_bits_u3;"
    "func ^:gen_bits_u3;"
    "func ^:state_f3;"
    "func ^:gen_f3;"
    "u2_t state = 16437;"
    "func ^:bits;"
    "func ^:max_u3;"
    "func ^:min_u3;"
    "func ^:state_u3;"
    "func ^:set_state_u3;"
    "func ^:gen;"
"}";

u3_t bcore_prsg_lcg_u2_02_s_gen_u3( bcore_prsg_lcg_u2_02_s* o )
{
    bcore_prsg_lcg_u2_02_s_gen( o );
    return bcore_prsg_lcg_u2_02_s_state_u3( o );
}

u3_t bcore_prsg_lcg_u2_02_s_state_bits_u3( const bcore_prsg_lcg_u2_02_s* o, sz_t bits )
{
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_lcg_u2_02_s_bits( o );
    if( o_bits >= bits )
    {
        return bcore_prsg_lcg_u2_02_s_state_u3( o ) >> ( o_bits - bits );
    }
    else
    {
        return bcore_prsg_lcg_u2_02_s_state_u3( o ) << ( bits - o_bits );
    }
}

u3_t bcore_prsg_lcg_u2_02_s_gen_bits_u3( bcore_prsg_lcg_u2_02_s* o, sz_t bits )
{
    bcore_prsg_lcg_u2_02_s_gen( o );
    return bcore_prsg_lcg_u2_02_s_state_bits_u3( o, bits );
}

f3_t bcore_prsg_lcg_u2_02_s_state_f3( const bcore_prsg_lcg_u2_02_s* o, f3_t min, f3_t max )
{
    return min + ( max - min ) * ( ( f3_t )( bcore_prsg_lcg_u2_02_s_state_u3( o ) - bcore_prsg_lcg_u2_02_s_min_u3( o ) ) / ( bcore_prsg_lcg_u2_02_s_max_u3( o ) - bcore_prsg_lcg_u2_02_s_min_u3( o ) ) );
}

f3_t bcore_prsg_lcg_u2_02_s_gen_f3( bcore_prsg_lcg_u2_02_s* o, f3_t min, f3_t max )
{
    bcore_prsg_lcg_u2_02_s_gen( o );
    return bcore_prsg_lcg_u2_02_s_state_f3( o, min, max );
}

BCORE_DEFINE_OBJECT_INST_P( bcore_prsg_lcg_u2_03_s )
"aware bcore_prsg"
"{"
    "func ^:gen_u3;"
    "func ^:state_bits_u3;"
    "func ^:gen_bits_u3;"
    "func ^:state_f3;"
    "func ^:gen_f3;"
    "u2_t state = 16437;"
    "func ^:bits;"
    "func ^:max_u3;"
    "func ^:min_u3;"
    "func ^:state_u3;"
    "func ^:set_state_u3;"
    "func ^:gen;"
"}";

u3_t bcore_prsg_lcg_u2_03_s_gen_u3( bcore_prsg_lcg_u2_03_s* o )
{
    bcore_prsg_lcg_u2_03_s_gen( o );
    return bcore_prsg_lcg_u2_03_s_state_u3( o );
}

u3_t bcore_prsg_lcg_u2_03_s_state_bits_u3( const bcore_prsg_lcg_u2_03_s* o, sz_t bits )
{
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_lcg_u2_03_s_bits( o );
    if( o_bits >= bits )
    {
        return bcore_prsg_lcg_u2_03_s_state_u3( o ) >> ( o_bits - bits );
    }
    else
    {
        return bcore_prsg_lcg_u2_03_s_state_u3( o ) << ( bits - o_bits );
    }
}

u3_t bcore_prsg_lcg_u2_03_s_gen_bits_u3( bcore_prsg_lcg_u2_03_s* o, sz_t bits )
{
    bcore_prsg_lcg_u2_03_s_gen( o );
    return bcore_prsg_lcg_u2_03_s_state_bits_u3( o, bits );
}

f3_t bcore_prsg_lcg_u2_03_s_state_f3( const bcore_prsg_lcg_u2_03_s* o, f3_t min, f3_t max )
{
    return min + ( max - min ) * ( ( f3_t )( bcore_prsg_lcg_u2_03_s_state_u3( o ) - bcore_prsg_lcg_u2_03_s_min_u3( o ) ) / ( bcore_prsg_lcg_u2_03_s_max_u3( o ) - bcore_prsg_lcg_u2_03_s_min_u3( o ) ) );
}

f3_t bcore_prsg_lcg_u2_03_s_gen_f3( bcore_prsg_lcg_u2_03_s* o, f3_t min, f3_t max )
{
    bcore_prsg_lcg_u2_03_s_gen( o );
    return bcore_prsg_lcg_u2_03_s_state_f3( o, min, max );
}

BCORE_DEFINE_OBJECT_INST_P( bcore_prsg_lcg_u2_04_s )
"aware bcore_prsg"
"{"
    "func ^:gen_u3;"
    "func ^:state_bits_u3;"
    "func ^:gen_bits_u3;"
    "func ^:state_f3;"
    "func ^:gen_f3;"
    "u2_t state = 16437;"
    "func ^:bits;"
    "func ^:max_u3;"
    "func ^:min_u3;"
    "func ^:state_u3;"
    "func ^:set_state_u3;"
    "func ^:gen;"
"}";

u3_t bcore_prsg_lcg_u2_04_s_gen_u3( bcore_prsg_lcg_u2_04_s* o )
{
    bcore_prsg_lcg_u2_04_s_gen( o );
    return bcore_prsg_lcg_u2_04_s_state_u3( o );
}

u3_t bcore_prsg_lcg_u2_04_s_state_bits_u3( const bcore_prsg_lcg_u2_04_s* o, sz_t bits )
{
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_lcg_u2_04_s_bits( o );
    if( o_bits >= bits )
    {
        return bcore_prsg_lcg_u2_04_s_state_u3( o ) >> ( o_bits - bits );
    }
    else
    {
        return bcore_prsg_lcg_u2_04_s_state_u3( o ) << ( bits - o_bits );
    }
}

u3_t bcore_prsg_lcg_u2_04_s_gen_bits_u3( bcore_prsg_lcg_u2_04_s* o, sz_t bits )
{
    bcore_prsg_lcg_u2_04_s_gen( o );
    return bcore_prsg_lcg_u2_04_s_state_bits_u3( o, bits );
}

f3_t bcore_prsg_lcg_u2_04_s_state_f3( const bcore_prsg_lcg_u2_04_s* o, f3_t min, f3_t max )
{
    return min + ( max - min ) * ( ( f3_t )( bcore_prsg_lcg_u2_04_s_state_u3( o ) - bcore_prsg_lcg_u2_04_s_min_u3( o ) ) / ( bcore_prsg_lcg_u2_04_s_max_u3( o ) - bcore_prsg_lcg_u2_04_s_min_u3( o ) ) );
}

f3_t bcore_prsg_lcg_u2_04_s_gen_f3( bcore_prsg_lcg_u2_04_s* o, f3_t min, f3_t max )
{
    bcore_prsg_lcg_u2_04_s_gen( o );
    return bcore_prsg_lcg_u2_04_s_state_f3( o, min, max );
}

BCORE_DEFINE_OBJECT_INST_P( bcore_prsg_lcg_u2_05_s )
"aware bcore_prsg"
"{"
    "func ^:gen_u3;"
    "func ^:state_bits_u3;"
    "func ^:gen_bits_u3;"
    "func ^:state_f3;"
    "func ^:gen_f3;"
    "u2_t state = 16437;"
    "func ^:bits;"
    "func ^:max_u3;"
    "func ^:min_u3;"
    "func ^:state_u3;"
    "func ^:set_state_u3;"
    "func ^:gen;"
"}";

u3_t bcore_prsg_lcg_u2_05_s_gen_u3( bcore_prsg_lcg_u2_05_s* o )
{
    bcore_prsg_lcg_u2_05_s_gen( o );
    return bcore_prsg_lcg_u2_05_s_state_u3( o );
}

u3_t bcore_prsg_lcg_u2_05_s_state_bits_u3( const bcore_prsg_lcg_u2_05_s* o, sz_t bits )
{
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_lcg_u2_05_s_bits( o );
    if( o_bits >= bits )
    {
        return bcore_prsg_lcg_u2_05_s_state_u3( o ) >> ( o_bits - bits );
    }
    else
    {
        return bcore_prsg_lcg_u2_05_s_state_u3( o ) << ( bits - o_bits );
    }
}

u3_t bcore_prsg_lcg_u2_05_s_gen_bits_u3( bcore_prsg_lcg_u2_05_s* o, sz_t bits )
{
    bcore_prsg_lcg_u2_05_s_gen( o );
    return bcore_prsg_lcg_u2_05_s_state_bits_u3( o, bits );
}

f3_t bcore_prsg_lcg_u2_05_s_state_f3( const bcore_prsg_lcg_u2_05_s* o, f3_t min, f3_t max )
{
    return min + ( max - min ) * ( ( f3_t )( bcore_prsg_lcg_u2_05_s_state_u3( o ) - bcore_prsg_lcg_u2_05_s_min_u3( o ) ) / ( bcore_prsg_lcg_u2_05_s_max_u3( o ) - bcore_prsg_lcg_u2_05_s_min_u3( o ) ) );
}

f3_t bcore_prsg_lcg_u2_05_s_gen_f3( bcore_prsg_lcg_u2_05_s* o, f3_t min, f3_t max )
{
    bcore_prsg_lcg_u2_05_s_gen( o );
    return bcore_prsg_lcg_u2_05_s_state_f3( o, min, max );
}

BCORE_DEFINE_OBJECT_INST_P( bcore_prsg_lcg_u3_00_s )
"aware bcore_prsg"
"{"
    "func ^:gen_u3;"
    "func ^:state_bits_u3;"
    "func ^:gen_bits_u3;"
    "func ^:state_f3;"
    "func ^:gen_f3;"
    "u3_t state = 16437;"
    "func ^:bits;"
    "func ^:max_u3;"
    "func ^:min_u3;"
    "func ^:state_u3;"
    "func ^:set_state_u3;"
    "func ^:gen;"
"}";

u3_t bcore_prsg_lcg_u3_00_s_gen_u3( bcore_prsg_lcg_u3_00_s* o )
{
    bcore_prsg_lcg_u3_00_s_gen( o );
    return bcore_prsg_lcg_u3_00_s_state_u3( o );
}

u3_t bcore_prsg_lcg_u3_00_s_state_bits_u3( const bcore_prsg_lcg_u3_00_s* o, sz_t bits )
{
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_lcg_u3_00_s_bits( o );
    if( o_bits >= bits )
    {
        return bcore_prsg_lcg_u3_00_s_state_u3( o ) >> ( o_bits - bits );
    }
    else
    {
        return bcore_prsg_lcg_u3_00_s_state_u3( o ) << ( bits - o_bits );
    }
}

u3_t bcore_prsg_lcg_u3_00_s_gen_bits_u3( bcore_prsg_lcg_u3_00_s* o, sz_t bits )
{
    bcore_prsg_lcg_u3_00_s_gen( o );
    return bcore_prsg_lcg_u3_00_s_state_bits_u3( o, bits );
}

f3_t bcore_prsg_lcg_u3_00_s_state_f3( const bcore_prsg_lcg_u3_00_s* o, f3_t min, f3_t max )
{
    return min + ( max - min ) * ( ( f3_t )( bcore_prsg_lcg_u3_00_s_state_u3( o ) - bcore_prsg_lcg_u3_00_s_min_u3( o ) ) / ( bcore_prsg_lcg_u3_00_s_max_u3( o ) - bcore_prsg_lcg_u3_00_s_min_u3( o ) ) );
}

f3_t bcore_prsg_lcg_u3_00_s_gen_f3( bcore_prsg_lcg_u3_00_s* o, f3_t min, f3_t max )
{
    bcore_prsg_lcg_u3_00_s_gen( o );
    return bcore_prsg_lcg_u3_00_s_state_f3( o, min, max );
}

BCORE_DEFINE_OBJECT_INST_P( bcore_prsg_lcg_u3_01_s )
"aware bcore_prsg"
"{"
    "func ^:gen_u3;"
    "func ^:state_bits_u3;"
    "func ^:gen_bits_u3;"
    "func ^:state_f3;"
    "func ^:gen_f3;"
    "u3_t state = 16437;"
    "func ^:bits;"
    "func ^:max_u3;"
    "func ^:min_u3;"
    "func ^:state_u3;"
    "func ^:set_state_u3;"
    "func ^:gen;"
"}";

u3_t bcore_prsg_lcg_u3_01_s_gen_u3( bcore_prsg_lcg_u3_01_s* o )
{
    bcore_prsg_lcg_u3_01_s_gen( o );
    return bcore_prsg_lcg_u3_01_s_state_u3( o );
}

u3_t bcore_prsg_lcg_u3_01_s_state_bits_u3( const bcore_prsg_lcg_u3_01_s* o, sz_t bits )
{
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_lcg_u3_01_s_bits( o );
    if( o_bits >= bits )
    {
        return bcore_prsg_lcg_u3_01_s_state_u3( o ) >> ( o_bits - bits );
    }
    else
    {
        return bcore_prsg_lcg_u3_01_s_state_u3( o ) << ( bits - o_bits );
    }
}

u3_t bcore_prsg_lcg_u3_01_s_gen_bits_u3( bcore_prsg_lcg_u3_01_s* o, sz_t bits )
{
    bcore_prsg_lcg_u3_01_s_gen( o );
    return bcore_prsg_lcg_u3_01_s_state_bits_u3( o, bits );
}

f3_t bcore_prsg_lcg_u3_01_s_state_f3( const bcore_prsg_lcg_u3_01_s* o, f3_t min, f3_t max )
{
    return min + ( max - min ) * ( ( f3_t )( bcore_prsg_lcg_u3_01_s_state_u3( o ) - bcore_prsg_lcg_u3_01_s_min_u3( o ) ) / ( bcore_prsg_lcg_u3_01_s_max_u3( o ) - bcore_prsg_lcg_u3_01_s_min_u3( o ) ) );
}

f3_t bcore_prsg_lcg_u3_01_s_gen_f3( bcore_prsg_lcg_u3_01_s* o, f3_t min, f3_t max )
{
    bcore_prsg_lcg_u3_01_s_gen( o );
    return bcore_prsg_lcg_u3_01_s_state_f3( o, min, max );
}

BCORE_DEFINE_OBJECT_INST_P( bcore_prsg_lcg_u3_02_s )
"aware bcore_prsg"
"{"
    "func ^:gen_u3;"
    "func ^:state_bits_u3;"
    "func ^:gen_bits_u3;"
    "func ^:state_f3;"
    "func ^:gen_f3;"
    "u3_t state = 16437;"
    "func ^:bits;"
    "func ^:max_u3;"
    "func ^:min_u3;"
    "func ^:state_u3;"
    "func ^:set_state_u3;"
    "func ^:gen;"
"}";

u3_t bcore_prsg_lcg_u3_02_s_gen_u3( bcore_prsg_lcg_u3_02_s* o )
{
    bcore_prsg_lcg_u3_02_s_gen( o );
    return bcore_prsg_lcg_u3_02_s_state_u3( o );
}

u3_t bcore_prsg_lcg_u3_02_s_state_bits_u3( const bcore_prsg_lcg_u3_02_s* o, sz_t bits )
{
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_lcg_u3_02_s_bits( o );
    if( o_bits >= bits )
    {
        return bcore_prsg_lcg_u3_02_s_state_u3( o ) >> ( o_bits - bits );
    }
    else
    {
        return bcore_prsg_lcg_u3_02_s_state_u3( o ) << ( bits - o_bits );
    }
}

u3_t bcore_prsg_lcg_u3_02_s_gen_bits_u3( bcore_prsg_lcg_u3_02_s* o, sz_t bits )
{
    bcore_prsg_lcg_u3_02_s_gen( o );
    return bcore_prsg_lcg_u3_02_s_state_bits_u3( o, bits );
}

f3_t bcore_prsg_lcg_u3_02_s_state_f3( const bcore_prsg_lcg_u3_02_s* o, f3_t min, f3_t max )
{
    return min + ( max - min ) * ( ( f3_t )( bcore_prsg_lcg_u3_02_s_state_u3( o ) - bcore_prsg_lcg_u3_02_s_min_u3( o ) ) / ( bcore_prsg_lcg_u3_02_s_max_u3( o ) - bcore_prsg_lcg_u3_02_s_min_u3( o ) ) );
}

f3_t bcore_prsg_lcg_u3_02_s_gen_f3( bcore_prsg_lcg_u3_02_s* o, f3_t min, f3_t max )
{
    bcore_prsg_lcg_u3_02_s_gen( o );
    return bcore_prsg_lcg_u3_02_s_state_f3( o, min, max );
}

BCORE_DEFINE_OBJECT_INST_P( bcore_prsg_lcg_u3_03_s )
"aware bcore_prsg"
"{"
    "func ^:gen_u3;"
    "func ^:state_bits_u3;"
    "func ^:gen_bits_u3;"
    "func ^:state_f3;"
    "func ^:gen_f3;"
    "u3_t state = 16437;"
    "func ^:bits;"
    "func ^:max_u3;"
    "func ^:min_u3;"
    "func ^:state_u3;"
    "func ^:set_state_u3;"
    "func ^:gen;"
"}";

u3_t bcore_prsg_lcg_u3_03_s_gen_u3( bcore_prsg_lcg_u3_03_s* o )
{
    bcore_prsg_lcg_u3_03_s_gen( o );
    return bcore_prsg_lcg_u3_03_s_state_u3( o );
}

u3_t bcore_prsg_lcg_u3_03_s_state_bits_u3( const bcore_prsg_lcg_u3_03_s* o, sz_t bits )
{
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_lcg_u3_03_s_bits( o );
    if( o_bits >= bits )
    {
        return bcore_prsg_lcg_u3_03_s_state_u3( o ) >> ( o_bits - bits );
    }
    else
    {
        return bcore_prsg_lcg_u3_03_s_state_u3( o ) << ( bits - o_bits );
    }
}

u3_t bcore_prsg_lcg_u3_03_s_gen_bits_u3( bcore_prsg_lcg_u3_03_s* o, sz_t bits )
{
    bcore_prsg_lcg_u3_03_s_gen( o );
    return bcore_prsg_lcg_u3_03_s_state_bits_u3( o, bits );
}

f3_t bcore_prsg_lcg_u3_03_s_state_f3( const bcore_prsg_lcg_u3_03_s* o, f3_t min, f3_t max )
{
    return min + ( max - min ) * ( ( f3_t )( bcore_prsg_lcg_u3_03_s_state_u3( o ) - bcore_prsg_lcg_u3_03_s_min_u3( o ) ) / ( bcore_prsg_lcg_u3_03_s_max_u3( o ) - bcore_prsg_lcg_u3_03_s_min_u3( o ) ) );
}

f3_t bcore_prsg_lcg_u3_03_s_gen_f3( bcore_prsg_lcg_u3_03_s* o, f3_t min, f3_t max )
{
    bcore_prsg_lcg_u3_03_s_gen( o );
    return bcore_prsg_lcg_u3_03_s_state_f3( o, min, max );
}

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_prsg_xsg

BCORE_DEFINE_OBJECT_INST_P( bcore_prsg_xsg_u2_00_s )
"aware bcore_prsg"
"{"
    "func ^:gen_u3;"
    "func ^:state_bits_u3;"
    "func ^:gen_bits_u3;"
    "func ^:state_f3;"
    "func ^:gen_f3;"
    "u2_t state = 16432;"
    "func ^:bits;"
    "func ^:max_u3;"
    "func ^:min_u3;"
    "func ^:state_u3;"
    "func ^:set_state_u3;"
    "func ^:gen;"
"}";

u3_t bcore_prsg_xsg_u2_00_s_gen_u3( bcore_prsg_xsg_u2_00_s* o )
{
    bcore_prsg_xsg_u2_00_s_gen( o );
    return bcore_prsg_xsg_u2_00_s_state_u3( o );
}

u3_t bcore_prsg_xsg_u2_00_s_state_bits_u3( const bcore_prsg_xsg_u2_00_s* o, sz_t bits )
{
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_xsg_u2_00_s_bits( o );
    if( o_bits >= bits )
    {
        return bcore_prsg_xsg_u2_00_s_state_u3( o ) >> ( o_bits - bits );
    }
    else
    {
        return bcore_prsg_xsg_u2_00_s_state_u3( o ) << ( bits - o_bits );
    }
}

u3_t bcore_prsg_xsg_u2_00_s_gen_bits_u3( bcore_prsg_xsg_u2_00_s* o, sz_t bits )
{
    bcore_prsg_xsg_u2_00_s_gen( o );
    return bcore_prsg_xsg_u2_00_s_state_bits_u3( o, bits );
}

f3_t bcore_prsg_xsg_u2_00_s_state_f3( const bcore_prsg_xsg_u2_00_s* o, f3_t min, f3_t max )
{
    return min + ( max - min ) * ( ( f3_t )( bcore_prsg_xsg_u2_00_s_state_u3( o ) - bcore_prsg_xsg_u2_00_s_min_u3( o ) ) / ( bcore_prsg_xsg_u2_00_s_max_u3( o ) - bcore_prsg_xsg_u2_00_s_min_u3( o ) ) );
}

f3_t bcore_prsg_xsg_u2_00_s_gen_f3( bcore_prsg_xsg_u2_00_s* o, f3_t min, f3_t max )
{
    bcore_prsg_xsg_u2_00_s_gen( o );
    return bcore_prsg_xsg_u2_00_s_state_f3( o, min, max );
}

BCORE_DEFINE_OBJECT_INST_P( bcore_prsg_xsg_u2_01_s )
"aware bcore_prsg"
"{"
    "func ^:gen_u3;"
    "func ^:state_bits_u3;"
    "func ^:gen_bits_u3;"
    "func ^:state_f3;"
    "func ^:gen_f3;"
    "u2_t state = 16432;"
    "func ^:bits;"
    "func ^:max_u3;"
    "func ^:min_u3;"
    "func ^:state_u3;"
    "func ^:set_state_u3;"
    "func ^:gen;"
"}";

u3_t bcore_prsg_xsg_u2_01_s_gen_u3( bcore_prsg_xsg_u2_01_s* o )
{
    bcore_prsg_xsg_u2_01_s_gen( o );
    return bcore_prsg_xsg_u2_01_s_state_u3( o );
}

u3_t bcore_prsg_xsg_u2_01_s_state_bits_u3( const bcore_prsg_xsg_u2_01_s* o, sz_t bits )
{
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_xsg_u2_01_s_bits( o );
    if( o_bits >= bits )
    {
        return bcore_prsg_xsg_u2_01_s_state_u3( o ) >> ( o_bits - bits );
    }
    else
    {
        return bcore_prsg_xsg_u2_01_s_state_u3( o ) << ( bits - o_bits );
    }
}

u3_t bcore_prsg_xsg_u2_01_s_gen_bits_u3( bcore_prsg_xsg_u2_01_s* o, sz_t bits )
{
    bcore_prsg_xsg_u2_01_s_gen( o );
    return bcore_prsg_xsg_u2_01_s_state_bits_u3( o, bits );
}

f3_t bcore_prsg_xsg_u2_01_s_state_f3( const bcore_prsg_xsg_u2_01_s* o, f3_t min, f3_t max )
{
    return min + ( max - min ) * ( ( f3_t )( bcore_prsg_xsg_u2_01_s_state_u3( o ) - bcore_prsg_xsg_u2_01_s_min_u3( o ) ) / ( bcore_prsg_xsg_u2_01_s_max_u3( o ) - bcore_prsg_xsg_u2_01_s_min_u3( o ) ) );
}

f3_t bcore_prsg_xsg_u2_01_s_gen_f3( bcore_prsg_xsg_u2_01_s* o, f3_t min, f3_t max )
{
    bcore_prsg_xsg_u2_01_s_gen( o );
    return bcore_prsg_xsg_u2_01_s_state_f3( o, min, max );
}

BCORE_DEFINE_OBJECT_INST_P( bcore_prsg_xsg_u2_02_s )
"aware bcore_prsg"
"{"
    "func ^:gen_u3;"
    "func ^:state_bits_u3;"
    "func ^:gen_bits_u3;"
    "func ^:state_f3;"
    "func ^:gen_f3;"
    "u2_t state = 16432;"
    "func ^:bits;"
    "func ^:max_u3;"
    "func ^:min_u3;"
    "func ^:state_u3;"
    "func ^:set_state_u3;"
    "func ^:gen;"
"}";

u3_t bcore_prsg_xsg_u2_02_s_gen_u3( bcore_prsg_xsg_u2_02_s* o )
{
    bcore_prsg_xsg_u2_02_s_gen( o );
    return bcore_prsg_xsg_u2_02_s_state_u3( o );
}

u3_t bcore_prsg_xsg_u2_02_s_state_bits_u3( const bcore_prsg_xsg_u2_02_s* o, sz_t bits )
{
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_xsg_u2_02_s_bits( o );
    if( o_bits >= bits )
    {
        return bcore_prsg_xsg_u2_02_s_state_u3( o ) >> ( o_bits - bits );
    }
    else
    {
        return bcore_prsg_xsg_u2_02_s_state_u3( o ) << ( bits - o_bits );
    }
}

u3_t bcore_prsg_xsg_u2_02_s_gen_bits_u3( bcore_prsg_xsg_u2_02_s* o, sz_t bits )
{
    bcore_prsg_xsg_u2_02_s_gen( o );
    return bcore_prsg_xsg_u2_02_s_state_bits_u3( o, bits );
}

f3_t bcore_prsg_xsg_u2_02_s_state_f3( const bcore_prsg_xsg_u2_02_s* o, f3_t min, f3_t max )
{
    return min + ( max - min ) * ( ( f3_t )( bcore_prsg_xsg_u2_02_s_state_u3( o ) - bcore_prsg_xsg_u2_02_s_min_u3( o ) ) / ( bcore_prsg_xsg_u2_02_s_max_u3( o ) - bcore_prsg_xsg_u2_02_s_min_u3( o ) ) );
}

f3_t bcore_prsg_xsg_u2_02_s_gen_f3( bcore_prsg_xsg_u2_02_s* o, f3_t min, f3_t max )
{
    bcore_prsg_xsg_u2_02_s_gen( o );
    return bcore_prsg_xsg_u2_02_s_state_f3( o, min, max );
}

BCORE_DEFINE_OBJECT_INST_P( bcore_prsg_xsg_u2_03_s )
"aware bcore_prsg"
"{"
    "func ^:gen_u3;"
    "func ^:state_bits_u3;"
    "func ^:gen_bits_u3;"
    "func ^:state_f3;"
    "func ^:gen_f3;"
    "u2_t state = 16432;"
    "func ^:bits;"
    "func ^:max_u3;"
    "func ^:min_u3;"
    "func ^:state_u3;"
    "func ^:set_state_u3;"
    "func ^:gen;"
"}";

u3_t bcore_prsg_xsg_u2_03_s_gen_u3( bcore_prsg_xsg_u2_03_s* o )
{
    bcore_prsg_xsg_u2_03_s_gen( o );
    return bcore_prsg_xsg_u2_03_s_state_u3( o );
}

u3_t bcore_prsg_xsg_u2_03_s_state_bits_u3( const bcore_prsg_xsg_u2_03_s* o, sz_t bits )
{
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_xsg_u2_03_s_bits( o );
    if( o_bits >= bits )
    {
        return bcore_prsg_xsg_u2_03_s_state_u3( o ) >> ( o_bits - bits );
    }
    else
    {
        return bcore_prsg_xsg_u2_03_s_state_u3( o ) << ( bits - o_bits );
    }
}

u3_t bcore_prsg_xsg_u2_03_s_gen_bits_u3( bcore_prsg_xsg_u2_03_s* o, sz_t bits )
{
    bcore_prsg_xsg_u2_03_s_gen( o );
    return bcore_prsg_xsg_u2_03_s_state_bits_u3( o, bits );
}

f3_t bcore_prsg_xsg_u2_03_s_state_f3( const bcore_prsg_xsg_u2_03_s* o, f3_t min, f3_t max )
{
    return min + ( max - min ) * ( ( f3_t )( bcore_prsg_xsg_u2_03_s_state_u3( o ) - bcore_prsg_xsg_u2_03_s_min_u3( o ) ) / ( bcore_prsg_xsg_u2_03_s_max_u3( o ) - bcore_prsg_xsg_u2_03_s_min_u3( o ) ) );
}

f3_t bcore_prsg_xsg_u2_03_s_gen_f3( bcore_prsg_xsg_u2_03_s* o, f3_t min, f3_t max )
{
    bcore_prsg_xsg_u2_03_s_gen( o );
    return bcore_prsg_xsg_u2_03_s_state_f3( o, min, max );
}

/**********************************************************************************************************************/


vd_t bcore_xoila_out_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_xoila_out" ) ) )
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
            // source: bcore_xoila_inexpandable.h

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
            BCORE_REGISTER_OBJECT( bcore_main_frame_s );
            BCORE_REGISTER_FEATURE( bcore_main_main );
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

            // --------------------------------------------------------------------
            // source: bcore_prsg.h

            // group: bcore_prsg
            BCORE_REGISTER_FEATURE( bcore_prsg_bits );
            BCORE_REGISTER_FEATURE( bcore_prsg_max_u3 );
            BCORE_REGISTER_FEATURE( bcore_prsg_min_u3 );
            BCORE_REGISTER_FEATURE( bcore_prsg_state_u3 );
            BCORE_REGISTER_FEATURE( bcore_prsg_state_bits_u3 );
            BCORE_REGISTER_FEATURE( bcore_prsg_state_f3 );
            BCORE_REGISTER_FEATURE( bcore_prsg_state_bl );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_bl, bcore_prsg_state_bl__ );
            BCORE_REGISTER_FEATURE( bcore_prsg_gen );
            BCORE_REGISTER_FEATURE( bcore_prsg_gen_bits_u3 );
            BCORE_REGISTER_FEATURE( bcore_prsg_gen_u3 );
            BCORE_REGISTER_FEATURE( bcore_prsg_gen_f3 );
            BCORE_REGISTER_FEATURE( bcore_prsg_gen_bl );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_bl, bcore_prsg_gen_bl__ );
            BCORE_REGISTER_FEATURE( bcore_prsg_set_state_u3 );
            BCORE_REGISTER_FEATURE( bcore_prsg_set_state_mix );
            BCORE_REGISTER_FFUNC( bcore_prsg_set_state_mix, bcore_prsg_set_state_mix__ );
            BCORE_REGISTER_FEATURE( bcore_prsg_reseed );
            BCORE_REGISTER_FFUNC( bcore_prsg_reseed, bcore_prsg_reseed__ );
            BCORE_REGISTER_SPECT( bcore_prsg );

            // group: bcore_prsg_lcg
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_u3, bcore_prsg_lcg_u2_00_s_gen_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_bits_u3, bcore_prsg_lcg_u2_00_s_state_bits_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_bits_u3, bcore_prsg_lcg_u2_00_s_gen_bits_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_f3, bcore_prsg_lcg_u2_00_s_state_f3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_f3, bcore_prsg_lcg_u2_00_s_gen_f3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_bits, bcore_prsg_lcg_u2_00_s_bits );
            BCORE_REGISTER_FFUNC( bcore_prsg_max_u3, bcore_prsg_lcg_u2_00_s_max_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_min_u3, bcore_prsg_lcg_u2_00_s_min_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_u3, bcore_prsg_lcg_u2_00_s_state_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_set_state_u3, bcore_prsg_lcg_u2_00_s_set_state_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen, bcore_prsg_lcg_u2_00_s_gen );
            BCORE_REGISTER_OBJECT( bcore_prsg_lcg_u2_00_s );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_u3, bcore_prsg_lcg_u2_01_s_gen_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_bits_u3, bcore_prsg_lcg_u2_01_s_state_bits_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_bits_u3, bcore_prsg_lcg_u2_01_s_gen_bits_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_f3, bcore_prsg_lcg_u2_01_s_state_f3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_f3, bcore_prsg_lcg_u2_01_s_gen_f3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_bits, bcore_prsg_lcg_u2_01_s_bits );
            BCORE_REGISTER_FFUNC( bcore_prsg_max_u3, bcore_prsg_lcg_u2_01_s_max_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_min_u3, bcore_prsg_lcg_u2_01_s_min_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_u3, bcore_prsg_lcg_u2_01_s_state_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_set_state_u3, bcore_prsg_lcg_u2_01_s_set_state_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen, bcore_prsg_lcg_u2_01_s_gen );
            BCORE_REGISTER_OBJECT( bcore_prsg_lcg_u2_01_s );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_u3, bcore_prsg_lcg_u2_02_s_gen_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_bits_u3, bcore_prsg_lcg_u2_02_s_state_bits_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_bits_u3, bcore_prsg_lcg_u2_02_s_gen_bits_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_f3, bcore_prsg_lcg_u2_02_s_state_f3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_f3, bcore_prsg_lcg_u2_02_s_gen_f3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_bits, bcore_prsg_lcg_u2_02_s_bits );
            BCORE_REGISTER_FFUNC( bcore_prsg_max_u3, bcore_prsg_lcg_u2_02_s_max_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_min_u3, bcore_prsg_lcg_u2_02_s_min_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_u3, bcore_prsg_lcg_u2_02_s_state_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_set_state_u3, bcore_prsg_lcg_u2_02_s_set_state_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen, bcore_prsg_lcg_u2_02_s_gen );
            BCORE_REGISTER_OBJECT( bcore_prsg_lcg_u2_02_s );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_u3, bcore_prsg_lcg_u2_03_s_gen_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_bits_u3, bcore_prsg_lcg_u2_03_s_state_bits_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_bits_u3, bcore_prsg_lcg_u2_03_s_gen_bits_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_f3, bcore_prsg_lcg_u2_03_s_state_f3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_f3, bcore_prsg_lcg_u2_03_s_gen_f3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_bits, bcore_prsg_lcg_u2_03_s_bits );
            BCORE_REGISTER_FFUNC( bcore_prsg_max_u3, bcore_prsg_lcg_u2_03_s_max_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_min_u3, bcore_prsg_lcg_u2_03_s_min_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_u3, bcore_prsg_lcg_u2_03_s_state_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_set_state_u3, bcore_prsg_lcg_u2_03_s_set_state_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen, bcore_prsg_lcg_u2_03_s_gen );
            BCORE_REGISTER_OBJECT( bcore_prsg_lcg_u2_03_s );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_u3, bcore_prsg_lcg_u2_04_s_gen_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_bits_u3, bcore_prsg_lcg_u2_04_s_state_bits_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_bits_u3, bcore_prsg_lcg_u2_04_s_gen_bits_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_f3, bcore_prsg_lcg_u2_04_s_state_f3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_f3, bcore_prsg_lcg_u2_04_s_gen_f3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_bits, bcore_prsg_lcg_u2_04_s_bits );
            BCORE_REGISTER_FFUNC( bcore_prsg_max_u3, bcore_prsg_lcg_u2_04_s_max_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_min_u3, bcore_prsg_lcg_u2_04_s_min_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_u3, bcore_prsg_lcg_u2_04_s_state_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_set_state_u3, bcore_prsg_lcg_u2_04_s_set_state_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen, bcore_prsg_lcg_u2_04_s_gen );
            BCORE_REGISTER_OBJECT( bcore_prsg_lcg_u2_04_s );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_u3, bcore_prsg_lcg_u2_05_s_gen_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_bits_u3, bcore_prsg_lcg_u2_05_s_state_bits_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_bits_u3, bcore_prsg_lcg_u2_05_s_gen_bits_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_f3, bcore_prsg_lcg_u2_05_s_state_f3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_f3, bcore_prsg_lcg_u2_05_s_gen_f3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_bits, bcore_prsg_lcg_u2_05_s_bits );
            BCORE_REGISTER_FFUNC( bcore_prsg_max_u3, bcore_prsg_lcg_u2_05_s_max_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_min_u3, bcore_prsg_lcg_u2_05_s_min_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_u3, bcore_prsg_lcg_u2_05_s_state_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_set_state_u3, bcore_prsg_lcg_u2_05_s_set_state_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen, bcore_prsg_lcg_u2_05_s_gen );
            BCORE_REGISTER_OBJECT( bcore_prsg_lcg_u2_05_s );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_u3, bcore_prsg_lcg_u3_00_s_gen_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_bits_u3, bcore_prsg_lcg_u3_00_s_state_bits_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_bits_u3, bcore_prsg_lcg_u3_00_s_gen_bits_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_f3, bcore_prsg_lcg_u3_00_s_state_f3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_f3, bcore_prsg_lcg_u3_00_s_gen_f3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_bits, bcore_prsg_lcg_u3_00_s_bits );
            BCORE_REGISTER_FFUNC( bcore_prsg_max_u3, bcore_prsg_lcg_u3_00_s_max_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_min_u3, bcore_prsg_lcg_u3_00_s_min_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_u3, bcore_prsg_lcg_u3_00_s_state_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_set_state_u3, bcore_prsg_lcg_u3_00_s_set_state_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen, bcore_prsg_lcg_u3_00_s_gen );
            BCORE_REGISTER_OBJECT( bcore_prsg_lcg_u3_00_s );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_u3, bcore_prsg_lcg_u3_01_s_gen_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_bits_u3, bcore_prsg_lcg_u3_01_s_state_bits_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_bits_u3, bcore_prsg_lcg_u3_01_s_gen_bits_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_f3, bcore_prsg_lcg_u3_01_s_state_f3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_f3, bcore_prsg_lcg_u3_01_s_gen_f3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_bits, bcore_prsg_lcg_u3_01_s_bits );
            BCORE_REGISTER_FFUNC( bcore_prsg_max_u3, bcore_prsg_lcg_u3_01_s_max_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_min_u3, bcore_prsg_lcg_u3_01_s_min_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_u3, bcore_prsg_lcg_u3_01_s_state_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_set_state_u3, bcore_prsg_lcg_u3_01_s_set_state_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen, bcore_prsg_lcg_u3_01_s_gen );
            BCORE_REGISTER_OBJECT( bcore_prsg_lcg_u3_01_s );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_u3, bcore_prsg_lcg_u3_02_s_gen_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_bits_u3, bcore_prsg_lcg_u3_02_s_state_bits_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_bits_u3, bcore_prsg_lcg_u3_02_s_gen_bits_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_f3, bcore_prsg_lcg_u3_02_s_state_f3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_f3, bcore_prsg_lcg_u3_02_s_gen_f3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_bits, bcore_prsg_lcg_u3_02_s_bits );
            BCORE_REGISTER_FFUNC( bcore_prsg_max_u3, bcore_prsg_lcg_u3_02_s_max_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_min_u3, bcore_prsg_lcg_u3_02_s_min_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_u3, bcore_prsg_lcg_u3_02_s_state_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_set_state_u3, bcore_prsg_lcg_u3_02_s_set_state_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen, bcore_prsg_lcg_u3_02_s_gen );
            BCORE_REGISTER_OBJECT( bcore_prsg_lcg_u3_02_s );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_u3, bcore_prsg_lcg_u3_03_s_gen_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_bits_u3, bcore_prsg_lcg_u3_03_s_state_bits_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_bits_u3, bcore_prsg_lcg_u3_03_s_gen_bits_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_f3, bcore_prsg_lcg_u3_03_s_state_f3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_f3, bcore_prsg_lcg_u3_03_s_gen_f3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_bits, bcore_prsg_lcg_u3_03_s_bits );
            BCORE_REGISTER_FFUNC( bcore_prsg_max_u3, bcore_prsg_lcg_u3_03_s_max_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_min_u3, bcore_prsg_lcg_u3_03_s_min_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_u3, bcore_prsg_lcg_u3_03_s_state_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_set_state_u3, bcore_prsg_lcg_u3_03_s_set_state_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen, bcore_prsg_lcg_u3_03_s_gen );
            BCORE_REGISTER_OBJECT( bcore_prsg_lcg_u3_03_s );
            BCORE_REGISTER_TRAIT( bcore_prsg_lcg, bcore_prsg );

            // group: bcore_prsg_xsg
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_u3, bcore_prsg_xsg_u2_00_s_gen_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_bits_u3, bcore_prsg_xsg_u2_00_s_state_bits_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_bits_u3, bcore_prsg_xsg_u2_00_s_gen_bits_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_f3, bcore_prsg_xsg_u2_00_s_state_f3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_f3, bcore_prsg_xsg_u2_00_s_gen_f3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_bits, bcore_prsg_xsg_u2_00_s_bits );
            BCORE_REGISTER_FFUNC( bcore_prsg_max_u3, bcore_prsg_xsg_u2_00_s_max_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_min_u3, bcore_prsg_xsg_u2_00_s_min_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_u3, bcore_prsg_xsg_u2_00_s_state_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_set_state_u3, bcore_prsg_xsg_u2_00_s_set_state_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen, bcore_prsg_xsg_u2_00_s_gen );
            BCORE_REGISTER_OBJECT( bcore_prsg_xsg_u2_00_s );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_u3, bcore_prsg_xsg_u2_01_s_gen_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_bits_u3, bcore_prsg_xsg_u2_01_s_state_bits_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_bits_u3, bcore_prsg_xsg_u2_01_s_gen_bits_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_f3, bcore_prsg_xsg_u2_01_s_state_f3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_f3, bcore_prsg_xsg_u2_01_s_gen_f3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_bits, bcore_prsg_xsg_u2_01_s_bits );
            BCORE_REGISTER_FFUNC( bcore_prsg_max_u3, bcore_prsg_xsg_u2_01_s_max_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_min_u3, bcore_prsg_xsg_u2_01_s_min_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_u3, bcore_prsg_xsg_u2_01_s_state_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_set_state_u3, bcore_prsg_xsg_u2_01_s_set_state_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen, bcore_prsg_xsg_u2_01_s_gen );
            BCORE_REGISTER_OBJECT( bcore_prsg_xsg_u2_01_s );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_u3, bcore_prsg_xsg_u2_02_s_gen_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_bits_u3, bcore_prsg_xsg_u2_02_s_state_bits_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_bits_u3, bcore_prsg_xsg_u2_02_s_gen_bits_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_f3, bcore_prsg_xsg_u2_02_s_state_f3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_f3, bcore_prsg_xsg_u2_02_s_gen_f3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_bits, bcore_prsg_xsg_u2_02_s_bits );
            BCORE_REGISTER_FFUNC( bcore_prsg_max_u3, bcore_prsg_xsg_u2_02_s_max_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_min_u3, bcore_prsg_xsg_u2_02_s_min_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_u3, bcore_prsg_xsg_u2_02_s_state_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_set_state_u3, bcore_prsg_xsg_u2_02_s_set_state_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen, bcore_prsg_xsg_u2_02_s_gen );
            BCORE_REGISTER_OBJECT( bcore_prsg_xsg_u2_02_s );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_u3, bcore_prsg_xsg_u2_03_s_gen_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_bits_u3, bcore_prsg_xsg_u2_03_s_state_bits_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_bits_u3, bcore_prsg_xsg_u2_03_s_gen_bits_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_f3, bcore_prsg_xsg_u2_03_s_state_f3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_f3, bcore_prsg_xsg_u2_03_s_gen_f3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_bits, bcore_prsg_xsg_u2_03_s_bits );
            BCORE_REGISTER_FFUNC( bcore_prsg_max_u3, bcore_prsg_xsg_u2_03_s_max_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_min_u3, bcore_prsg_xsg_u2_03_s_min_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_state_u3, bcore_prsg_xsg_u2_03_s_state_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_set_state_u3, bcore_prsg_xsg_u2_03_s_set_state_u3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen, bcore_prsg_xsg_u2_03_s_gen );
            BCORE_REGISTER_OBJECT( bcore_prsg_xsg_u2_03_s );
            BCORE_REGISTER_TRAIT( bcore_prsg_xsg, bcore_prsg );
        }
        break;
        default: break;
    }
    return NULL;
}
// XOILA_OUT_SIGNATURE 0x3F79DB38A50CF935ull
