/** This file was generated from xoila source code.
 *  Compiling Agent : xoico_compiler (C) 2020 J.B.Steffens
 *  Last File Update: 2021-01-27T11:32:28Z
 *
 *  Copyright and License of this File:
 *
 *  Generated code inherits the copyright and license of the underlying xoila source code.
 *  Source code defining this file is distributed across following files:
 *
 *  bcore_x_root_inexpandable.h
 *  bcore_x_inst.h
 *  bcore_x_array.h
 *  bcore_file.h
 *  bcore_spect_inst_call.h
 *  bcore_spect_via_call.h
 *  bcore_main.h
 *  bcore_hmap_name.h
 *  bcore_cday.h
 *  bcore_error_manager.h
 *  bcore_prsg.h
 *  bcore_arr_inexpandable.x
 *  bcore_flect_inexpandable.x
 *  bcore_hmap_inexpandable.x
 *  bcore_sink_inexpandable.x
 *  bcore_source_inexpandable.x
 *  bcore_st_inexpandable.x
 *
 */

#include "bcore.xo.h"
#include "bcore_spect.h"
#include "bcore_spect_inst.h"
#include "bcore_sr.h"
#include "bcore_const_manager.h"


/**********************************************************************************************************************/
// source: bcore_x_root_inexpandable.h
#include "bcore_x_root_inexpandable.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_x_root_expandable

XOILA_DEFINE_SPECT( bcore_inst, bcore_x_root_expandable )
"{"
    "bcore_spect_header_s header;"
"}";

/**********************************************************************************************************************/
// source: bcore_x_inst.h
#include "bcore_x_inst.h"

//----------------------------------------------------------------------------------------------------------------------
// group: x_inst

XOILA_DEFINE_SPECT( bcore_inst, x_inst )
"{"
    "bcore_spect_header_s header;"
"}";

/**********************************************************************************************************************/
// source: bcore_x_array.h
#include "bcore_x_array.h"

//----------------------------------------------------------------------------------------------------------------------
// group: x_array

XOILA_DEFINE_SPECT( x_inst, x_array )
"{"
    "bcore_spect_header_s header;"
"}";

x_inst* x_array_t_push_d( x_array* o, tp_t t, x_inst* v )
{
    // bcore_x_array.h:51:78
    
    const bcore_array_s* p = bcore_array_s_get_typed( t );
    
    if( p->item_p )
    {
        bcore_array_p_push( p, ( bcore_array* )o, sr_psd( p->item_p, v ) );
    }
    else
    {
        bcore_array_p_push( p, ( bcore_array* )o, sr_asd( v ) );
    }
    
    if( p->is_of_links )
    {
        return v;
    }
    else if( p->is_static )
    {
        uz_t size = 0;
        u0_t* data = bcore_array_p_get_d_data_size( p, o, &size );
        return ( x_inst* )( data + p->item_p->size * ( size - 1 ) );
    }
    else
    {
        return bcore_array_p_get_last( p, ( bcore_array* )o ).o;
    }
}

x_inst* x_array_t_push_c( x_array* o, tp_t t, const x_inst* v )
{
    // bcore_x_array.h:82:78
    
    const bcore_array_s* p = bcore_array_s_get_typed( t );
    
    if( p->item_p )
    {
        bcore_array_p_push( p, ( bcore_array* )o, sr_pwc( p->item_p, v ) );
    }
    else
    {
        bcore_array_p_push( p, ( bcore_array* )o, sr_awc( v ) );
    }
    
    if( p->is_of_links )
    {
        uz_t size = 0;
        x_inst** data = bcore_array_p_get_d_data_size( p, o, &size );
        return data[ size - 1 ];
    }
    else if( p->is_static )
    {
        uz_t size = 0;
        u0_t* data = bcore_array_p_get_d_data_size( p, o, &size );
        return ( x_inst* )( data + p->item_p->size * ( size - 1 ) );
    }
    else
    {
        return bcore_array_p_get_last( p, ( bcore_array* )o ).o;
    }
}

x_inst* x_array_t_push_t( x_array* o, tp_t t, tp_t val_type )
{
    // bcore_x_array.h:115:67
    
    const bcore_array_s* p = bcore_array_s_get_typed( t );
    bcore_array_p_push( p, ( bcore_array* )o, sr_t_create( val_type ) );
    
    if( p->is_of_links )
    {
        uz_t size = 0;
        x_inst** data = bcore_array_p_get_d_data_size( p, o, &size );
        return data[ size - 1 ];
    }
    else if( p->is_static )
    {
        uz_t size = 0;
        u0_t* data = bcore_array_p_get_d_data_size( p, o, &size );
        return ( x_inst* )( data + p->item_p->size * ( size - 1 ) );
    }
    else
    {
        return bcore_array_p_get_last( p, ( bcore_array* )o ).o;
    }
}

x_inst* x_array_t_push( x_array* o, tp_t t )
{
    // bcore_x_array.h:140:50
    
    const bcore_array_s* p = bcore_array_s_get_typed( t );
    
    if( p->item_p )
    {
        if( p->is_of_links )
        {
            tp_t item_type = p->item_p->header.o_type;
            bcore_array_p_push( p, ( bcore_array* )o, sr_t_create( item_type ) );
        }
        else
        {
            bcore_array_p_push( p, ( bcore_array* )o, sr_null() );
        }
    }
    else
    {
        ERR_fa( "Unspecified push to a dynamically typed array. Use push_d, push_c or push_t." );
    }
    
    if( p->is_of_links )
    {
        uz_t size = 0;
        x_inst** data = bcore_array_p_get_d_data_size( p, o, &size );
        return data[ size - 1 ];
    }
    else if( p->is_static )
    {
        uz_t size = 0;
        u0_t* data = bcore_array_p_get_d_data_size( p, o, &size );
        return ( x_inst* )( data + p->item_p->size * ( size - 1 ) );
    }
    else
    {
        return bcore_array_p_get_last( p, ( bcore_array* )o ).o;
    }
}

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
    // bcore_file.h:52:5
    
    st_s_attach( &(o->full ),  NULL);
    st_s_copy_sc(&(o->name),name );
    if( name[ 0 ] != '/' && o->root != NULL )
    {
        o->full = st_s_create_fa("#<sc_t>/#<sc_t>", o->root->sc, o->name.sc );
    }
}

XOILA_DEFINE_SPECT( bcore_inst, bcore_file )
"{"
    "bcore_spect_header_s header;"
"}";

/**********************************************************************************************************************/
// source: bcore_spect_inst_call.h
#include "bcore_spect_inst_call.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_inst_call

XOILA_DEFINE_SPECT( bcore_inst, bcore_inst_call )
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

XOILA_DEFINE_SPECT( bcore_inst, bcore_via_call )
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
"aware x_array"
"{"
    "aware bcore_main=> [];"
"}";

BCORE_DEFINE_OBJECT_INST_P( bcore_main_set_s )
"aware bcore_main"
"{"
    "bcore_main_arr_s arr;"
    "func ^:main;"
"}";

er_t bcore_main_set_s_main( bcore_main_set_s* o, bcore_main_frame_s* frame )
{
    // bcore_main.h:87:9
    
    {const bcore_main_arr_s* __a=&(o->arr );if(__a)for(sz_t __i=0; __i<__a->size; __i++){bcore_main* e=__a->data[__i]; BLM_TRY(bcore_main_a_main(e,frame ) )
    }}return  0;
}

XOILA_DEFINE_SPECT( bcore_inst, bcore_main )
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

XOILA_DEFINE_SPECT( bcore_inst, bcore_hmap_name )
"{"
    "bcore_spect_header_s header;"
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

XOILA_DEFINE_SPECT( bcore_inst, bcore_cday )
"{"
    "bcore_spect_header_s header;"
"}";

/**********************************************************************************************************************/
// source: bcore_error_manager.h
#include "bcore_error_manager.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_error_manager
#include "bcore_x_array.h"

BCORE_DEFINE_OBJECT_INST_P( bcore_error_manager_error_s )
"aware bcore_error_manager"
"{"
    "er_t id;"
    "st_s msg;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bcore_error_manager_error_adl_s )
"aware x_array"
"{"
    "bcore_error_manager_error_s => [];"
"}";

bcore_error_manager_error_adl_s* bcore_error_manager_error_adl_s_set_size( bcore_error_manager_error_adl_s* o, sz_t size )
{
    // bcore_error_manager.h:48:5
    
    return  ((bcore_error_manager_error_adl_s*)(x_array_set_size(((x_array*)( o)),size )));
}

bcore_error_manager_error_adl_s* bcore_error_manager_error_adl_s_clear( bcore_error_manager_error_adl_s* o )
{
    // bcore_error_manager.h:53:5
    
    return  ((bcore_error_manager_error_adl_s*)(x_array_clear(((x_array*)( o )))));
}

bcore_error_manager_error_s* bcore_error_manager_error_adl_s_push_d( bcore_error_manager_error_adl_s* o, bcore_error_manager_error_s* v )
{
    // bcore_error_manager.h:58:5
    
    return  ((bcore_error_manager_error_s*)(x_array_push_d(((x_array*)( o)),((x_inst*)(v )))));
}

BCORE_DEFINE_OBJECT_INST_P( bcore_error_manager_context_s )
"aware bcore_error_manager"
"{"
    "bcore_error_manager_error_adl_s adl;"
    "bcore_mutex_s mutex;"
"}";

XOILA_DEFINE_SPECT( bcore_inst, bcore_error_manager )
"{"
    "bcore_spect_header_s header;"
"}";

/**********************************************************************************************************************/
// source: bcore_prsg.h
#include "bcore_prsg.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_prsg

XOILA_DEFINE_SPECT( bcore_inst, bcore_prsg )
"{"
    "bcore_spect_header_s header;"
    "feature strict aware bcore_prsg : bits;"
    "feature strict aware bcore_prsg : max_u3;"
    "feature strict aware bcore_prsg : min_u3;"
    "feature strict aware bcore_prsg : state_u3;"
    "feature strict aware bcore_prsg : state_bits_u3;"
    "feature strict aware bcore_prsg : state_f3;"
    "feature aware bcore_prsg : state_bl = bcore_prsg_state_bl_default;"
    "feature strict aware bcore_prsg : gen;"
    "feature strict aware bcore_prsg : gen_bits_u3;"
    "feature strict aware bcore_prsg : gen_u3;"
    "feature strict aware bcore_prsg : gen_f3;"
    "feature aware bcore_prsg : gen_bl = bcore_prsg_gen_bl_default;"
    "feature strict aware bcore_prsg : set_state_u3;"
    "feature aware bcore_prsg : set_state_mix = bcore_prsg_set_state_mix_default;"
    "feature aware bcore_prsg : reseed = bcore_prsg_reseed_default;"
"}";

bcore_prsg* bcore_prsg_set_state_mix_default( bcore_prsg* o, const bcore_prsg* a, const bcore_prsg* b )
{
    // bcore_prsg.h:74:1
    
    /* Different mixing methods are thinkable:
     * Adding, multiplying or xoring should all be suitable.
     * We should just stick to the same method.
     */
    return  bcore_prsg_a_set_state_u3( o,bcore_prsg_a_state_u3( a ) + bcore_prsg_a_state_u3( b ) );
}

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_prsg_lcg

BCORE_DEFINE_OBJECT_INST_P( bcore_prsg_lcg_u2_00_s )
"aware bcore_prsg"
"{"
    "u2_t state = 16437;"
    "func ^:gen_u3;"
    "func ^:state_bits_u3;"
    "func ^:gen_bits_u3;"
    "func ^:state_f3;"
    "func ^:gen_f3;"
    "func ^:bits;"
    "func ^:max_u3;"
    "func ^:min_u3;"
    "func ^:state_u3;"
    "func ^:set_state_u3;"
    "func ^:gen;"
"}";

u3_t bcore_prsg_lcg_u2_00_s_gen_u3( bcore_prsg_lcg_u2_00_s* o )
{
    // bcore_prsg.h:90:5
    
    bcore_prsg_lcg_u2_00_s_gen( o );
    return  bcore_prsg_lcg_u2_00_s_state_u3( o );
}

u3_t bcore_prsg_lcg_u2_00_s_state_bits_u3( const bcore_prsg_lcg_u2_00_s* o, sz_t bits )
{
    // bcore_prsg.h:96:5
    
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_lcg_u2_00_s_bits( o );
    if( o_bits >= bits )
    {
        return  bcore_prsg_lcg_u2_00_s_state_u3( o ) >> ( o_bits - bits );
    }
    else
    {
        return  bcore_prsg_lcg_u2_00_s_state_u3( o ) << ( bits - o_bits );
    }
}

u3_t bcore_prsg_lcg_u2_00_s_gen_bits_u3( bcore_prsg_lcg_u2_00_s* o, sz_t bits )
{
    // bcore_prsg.h:110:5
    
    bcore_prsg_lcg_u2_00_s_gen( o );
    return  bcore_prsg_lcg_u2_00_s_state_bits_u3( o,bits );
}

f3_t bcore_prsg_lcg_u2_00_s_state_f3( const bcore_prsg_lcg_u2_00_s* o, f3_t min, f3_t max )
{
    // bcore_prsg.h:116:5
    
    return  min + ( max - min ) * ( ( f3_t )( bcore_prsg_lcg_u2_00_s_state_u3( o ) - bcore_prsg_lcg_u2_00_s_min_u3( o ) ) / ( bcore_prsg_lcg_u2_00_s_max_u3( o ) - bcore_prsg_lcg_u2_00_s_min_u3( o ) ) );
}

f3_t bcore_prsg_lcg_u2_00_s_gen_f3( bcore_prsg_lcg_u2_00_s* o, f3_t min, f3_t max )
{
    // bcore_prsg.h:121:5
    
    bcore_prsg_lcg_u2_00_s_gen( o );
    return  bcore_prsg_lcg_u2_00_s_state_f3( o,min, max );
}

BCORE_DEFINE_OBJECT_INST_P( bcore_prsg_lcg_u2_01_s )
"aware bcore_prsg"
"{"
    "u2_t state = 16437;"
    "func ^:gen_u3;"
    "func ^:state_bits_u3;"
    "func ^:gen_bits_u3;"
    "func ^:state_f3;"
    "func ^:gen_f3;"
    "func ^:bits;"
    "func ^:max_u3;"
    "func ^:min_u3;"
    "func ^:state_u3;"
    "func ^:set_state_u3;"
    "func ^:gen;"
"}";

u3_t bcore_prsg_lcg_u2_01_s_gen_u3( bcore_prsg_lcg_u2_01_s* o )
{
    // bcore_prsg.h:90:5
    
    bcore_prsg_lcg_u2_01_s_gen( o );
    return  bcore_prsg_lcg_u2_01_s_state_u3( o );
}

u3_t bcore_prsg_lcg_u2_01_s_state_bits_u3( const bcore_prsg_lcg_u2_01_s* o, sz_t bits )
{
    // bcore_prsg.h:96:5
    
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_lcg_u2_01_s_bits( o );
    if( o_bits >= bits )
    {
        return  bcore_prsg_lcg_u2_01_s_state_u3( o ) >> ( o_bits - bits );
    }
    else
    {
        return  bcore_prsg_lcg_u2_01_s_state_u3( o ) << ( bits - o_bits );
    }
}

u3_t bcore_prsg_lcg_u2_01_s_gen_bits_u3( bcore_prsg_lcg_u2_01_s* o, sz_t bits )
{
    // bcore_prsg.h:110:5
    
    bcore_prsg_lcg_u2_01_s_gen( o );
    return  bcore_prsg_lcg_u2_01_s_state_bits_u3( o,bits );
}

f3_t bcore_prsg_lcg_u2_01_s_state_f3( const bcore_prsg_lcg_u2_01_s* o, f3_t min, f3_t max )
{
    // bcore_prsg.h:116:5
    
    return  min + ( max - min ) * ( ( f3_t )( bcore_prsg_lcg_u2_01_s_state_u3( o ) - bcore_prsg_lcg_u2_01_s_min_u3( o ) ) / ( bcore_prsg_lcg_u2_01_s_max_u3( o ) - bcore_prsg_lcg_u2_01_s_min_u3( o ) ) );
}

f3_t bcore_prsg_lcg_u2_01_s_gen_f3( bcore_prsg_lcg_u2_01_s* o, f3_t min, f3_t max )
{
    // bcore_prsg.h:121:5
    
    bcore_prsg_lcg_u2_01_s_gen( o );
    return  bcore_prsg_lcg_u2_01_s_state_f3( o,min, max );
}

BCORE_DEFINE_OBJECT_INST_P( bcore_prsg_lcg_u2_02_s )
"aware bcore_prsg"
"{"
    "u2_t state = 16437;"
    "func ^:gen_u3;"
    "func ^:state_bits_u3;"
    "func ^:gen_bits_u3;"
    "func ^:state_f3;"
    "func ^:gen_f3;"
    "func ^:bits;"
    "func ^:max_u3;"
    "func ^:min_u3;"
    "func ^:state_u3;"
    "func ^:set_state_u3;"
    "func ^:gen;"
"}";

u3_t bcore_prsg_lcg_u2_02_s_gen_u3( bcore_prsg_lcg_u2_02_s* o )
{
    // bcore_prsg.h:90:5
    
    bcore_prsg_lcg_u2_02_s_gen( o );
    return  bcore_prsg_lcg_u2_02_s_state_u3( o );
}

u3_t bcore_prsg_lcg_u2_02_s_state_bits_u3( const bcore_prsg_lcg_u2_02_s* o, sz_t bits )
{
    // bcore_prsg.h:96:5
    
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_lcg_u2_02_s_bits( o );
    if( o_bits >= bits )
    {
        return  bcore_prsg_lcg_u2_02_s_state_u3( o ) >> ( o_bits - bits );
    }
    else
    {
        return  bcore_prsg_lcg_u2_02_s_state_u3( o ) << ( bits - o_bits );
    }
}

u3_t bcore_prsg_lcg_u2_02_s_gen_bits_u3( bcore_prsg_lcg_u2_02_s* o, sz_t bits )
{
    // bcore_prsg.h:110:5
    
    bcore_prsg_lcg_u2_02_s_gen( o );
    return  bcore_prsg_lcg_u2_02_s_state_bits_u3( o,bits );
}

f3_t bcore_prsg_lcg_u2_02_s_state_f3( const bcore_prsg_lcg_u2_02_s* o, f3_t min, f3_t max )
{
    // bcore_prsg.h:116:5
    
    return  min + ( max - min ) * ( ( f3_t )( bcore_prsg_lcg_u2_02_s_state_u3( o ) - bcore_prsg_lcg_u2_02_s_min_u3( o ) ) / ( bcore_prsg_lcg_u2_02_s_max_u3( o ) - bcore_prsg_lcg_u2_02_s_min_u3( o ) ) );
}

f3_t bcore_prsg_lcg_u2_02_s_gen_f3( bcore_prsg_lcg_u2_02_s* o, f3_t min, f3_t max )
{
    // bcore_prsg.h:121:5
    
    bcore_prsg_lcg_u2_02_s_gen( o );
    return  bcore_prsg_lcg_u2_02_s_state_f3( o,min, max );
}

BCORE_DEFINE_OBJECT_INST_P( bcore_prsg_lcg_u2_03_s )
"aware bcore_prsg"
"{"
    "u2_t state = 16437;"
    "func ^:gen_u3;"
    "func ^:state_bits_u3;"
    "func ^:gen_bits_u3;"
    "func ^:state_f3;"
    "func ^:gen_f3;"
    "func ^:bits;"
    "func ^:max_u3;"
    "func ^:min_u3;"
    "func ^:state_u3;"
    "func ^:set_state_u3;"
    "func ^:gen;"
"}";

u3_t bcore_prsg_lcg_u2_03_s_gen_u3( bcore_prsg_lcg_u2_03_s* o )
{
    // bcore_prsg.h:90:5
    
    bcore_prsg_lcg_u2_03_s_gen( o );
    return  bcore_prsg_lcg_u2_03_s_state_u3( o );
}

u3_t bcore_prsg_lcg_u2_03_s_state_bits_u3( const bcore_prsg_lcg_u2_03_s* o, sz_t bits )
{
    // bcore_prsg.h:96:5
    
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_lcg_u2_03_s_bits( o );
    if( o_bits >= bits )
    {
        return  bcore_prsg_lcg_u2_03_s_state_u3( o ) >> ( o_bits - bits );
    }
    else
    {
        return  bcore_prsg_lcg_u2_03_s_state_u3( o ) << ( bits - o_bits );
    }
}

u3_t bcore_prsg_lcg_u2_03_s_gen_bits_u3( bcore_prsg_lcg_u2_03_s* o, sz_t bits )
{
    // bcore_prsg.h:110:5
    
    bcore_prsg_lcg_u2_03_s_gen( o );
    return  bcore_prsg_lcg_u2_03_s_state_bits_u3( o,bits );
}

f3_t bcore_prsg_lcg_u2_03_s_state_f3( const bcore_prsg_lcg_u2_03_s* o, f3_t min, f3_t max )
{
    // bcore_prsg.h:116:5
    
    return  min + ( max - min ) * ( ( f3_t )( bcore_prsg_lcg_u2_03_s_state_u3( o ) - bcore_prsg_lcg_u2_03_s_min_u3( o ) ) / ( bcore_prsg_lcg_u2_03_s_max_u3( o ) - bcore_prsg_lcg_u2_03_s_min_u3( o ) ) );
}

f3_t bcore_prsg_lcg_u2_03_s_gen_f3( bcore_prsg_lcg_u2_03_s* o, f3_t min, f3_t max )
{
    // bcore_prsg.h:121:5
    
    bcore_prsg_lcg_u2_03_s_gen( o );
    return  bcore_prsg_lcg_u2_03_s_state_f3( o,min, max );
}

BCORE_DEFINE_OBJECT_INST_P( bcore_prsg_lcg_u2_04_s )
"aware bcore_prsg"
"{"
    "u2_t state = 16437;"
    "func ^:gen_u3;"
    "func ^:state_bits_u3;"
    "func ^:gen_bits_u3;"
    "func ^:state_f3;"
    "func ^:gen_f3;"
    "func ^:bits;"
    "func ^:max_u3;"
    "func ^:min_u3;"
    "func ^:state_u3;"
    "func ^:set_state_u3;"
    "func ^:gen;"
"}";

u3_t bcore_prsg_lcg_u2_04_s_gen_u3( bcore_prsg_lcg_u2_04_s* o )
{
    // bcore_prsg.h:90:5
    
    bcore_prsg_lcg_u2_04_s_gen( o );
    return  bcore_prsg_lcg_u2_04_s_state_u3( o );
}

u3_t bcore_prsg_lcg_u2_04_s_state_bits_u3( const bcore_prsg_lcg_u2_04_s* o, sz_t bits )
{
    // bcore_prsg.h:96:5
    
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_lcg_u2_04_s_bits( o );
    if( o_bits >= bits )
    {
        return  bcore_prsg_lcg_u2_04_s_state_u3( o ) >> ( o_bits - bits );
    }
    else
    {
        return  bcore_prsg_lcg_u2_04_s_state_u3( o ) << ( bits - o_bits );
    }
}

u3_t bcore_prsg_lcg_u2_04_s_gen_bits_u3( bcore_prsg_lcg_u2_04_s* o, sz_t bits )
{
    // bcore_prsg.h:110:5
    
    bcore_prsg_lcg_u2_04_s_gen( o );
    return  bcore_prsg_lcg_u2_04_s_state_bits_u3( o,bits );
}

f3_t bcore_prsg_lcg_u2_04_s_state_f3( const bcore_prsg_lcg_u2_04_s* o, f3_t min, f3_t max )
{
    // bcore_prsg.h:116:5
    
    return  min + ( max - min ) * ( ( f3_t )( bcore_prsg_lcg_u2_04_s_state_u3( o ) - bcore_prsg_lcg_u2_04_s_min_u3( o ) ) / ( bcore_prsg_lcg_u2_04_s_max_u3( o ) - bcore_prsg_lcg_u2_04_s_min_u3( o ) ) );
}

f3_t bcore_prsg_lcg_u2_04_s_gen_f3( bcore_prsg_lcg_u2_04_s* o, f3_t min, f3_t max )
{
    // bcore_prsg.h:121:5
    
    bcore_prsg_lcg_u2_04_s_gen( o );
    return  bcore_prsg_lcg_u2_04_s_state_f3( o,min, max );
}

BCORE_DEFINE_OBJECT_INST_P( bcore_prsg_lcg_u2_05_s )
"aware bcore_prsg"
"{"
    "u2_t state = 16437;"
    "func ^:gen_u3;"
    "func ^:state_bits_u3;"
    "func ^:gen_bits_u3;"
    "func ^:state_f3;"
    "func ^:gen_f3;"
    "func ^:bits;"
    "func ^:max_u3;"
    "func ^:min_u3;"
    "func ^:state_u3;"
    "func ^:set_state_u3;"
    "func ^:gen;"
"}";

u3_t bcore_prsg_lcg_u2_05_s_gen_u3( bcore_prsg_lcg_u2_05_s* o )
{
    // bcore_prsg.h:90:5
    
    bcore_prsg_lcg_u2_05_s_gen( o );
    return  bcore_prsg_lcg_u2_05_s_state_u3( o );
}

u3_t bcore_prsg_lcg_u2_05_s_state_bits_u3( const bcore_prsg_lcg_u2_05_s* o, sz_t bits )
{
    // bcore_prsg.h:96:5
    
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_lcg_u2_05_s_bits( o );
    if( o_bits >= bits )
    {
        return  bcore_prsg_lcg_u2_05_s_state_u3( o ) >> ( o_bits - bits );
    }
    else
    {
        return  bcore_prsg_lcg_u2_05_s_state_u3( o ) << ( bits - o_bits );
    }
}

u3_t bcore_prsg_lcg_u2_05_s_gen_bits_u3( bcore_prsg_lcg_u2_05_s* o, sz_t bits )
{
    // bcore_prsg.h:110:5
    
    bcore_prsg_lcg_u2_05_s_gen( o );
    return  bcore_prsg_lcg_u2_05_s_state_bits_u3( o,bits );
}

f3_t bcore_prsg_lcg_u2_05_s_state_f3( const bcore_prsg_lcg_u2_05_s* o, f3_t min, f3_t max )
{
    // bcore_prsg.h:116:5
    
    return  min + ( max - min ) * ( ( f3_t )( bcore_prsg_lcg_u2_05_s_state_u3( o ) - bcore_prsg_lcg_u2_05_s_min_u3( o ) ) / ( bcore_prsg_lcg_u2_05_s_max_u3( o ) - bcore_prsg_lcg_u2_05_s_min_u3( o ) ) );
}

f3_t bcore_prsg_lcg_u2_05_s_gen_f3( bcore_prsg_lcg_u2_05_s* o, f3_t min, f3_t max )
{
    // bcore_prsg.h:121:5
    
    bcore_prsg_lcg_u2_05_s_gen( o );
    return  bcore_prsg_lcg_u2_05_s_state_f3( o,min, max );
}

BCORE_DEFINE_OBJECT_INST_P( bcore_prsg_lcg_u3_00_s )
"aware bcore_prsg"
"{"
    "u3_t state = 16437;"
    "func ^:gen_u3;"
    "func ^:state_bits_u3;"
    "func ^:gen_bits_u3;"
    "func ^:state_f3;"
    "func ^:gen_f3;"
    "func ^:bits;"
    "func ^:max_u3;"
    "func ^:min_u3;"
    "func ^:state_u3;"
    "func ^:set_state_u3;"
    "func ^:gen;"
"}";

u3_t bcore_prsg_lcg_u3_00_s_gen_u3( bcore_prsg_lcg_u3_00_s* o )
{
    // bcore_prsg.h:90:5
    
    bcore_prsg_lcg_u3_00_s_gen( o );
    return  bcore_prsg_lcg_u3_00_s_state_u3( o );
}

u3_t bcore_prsg_lcg_u3_00_s_state_bits_u3( const bcore_prsg_lcg_u3_00_s* o, sz_t bits )
{
    // bcore_prsg.h:96:5
    
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_lcg_u3_00_s_bits( o );
    if( o_bits >= bits )
    {
        return  bcore_prsg_lcg_u3_00_s_state_u3( o ) >> ( o_bits - bits );
    }
    else
    {
        return  bcore_prsg_lcg_u3_00_s_state_u3( o ) << ( bits - o_bits );
    }
}

u3_t bcore_prsg_lcg_u3_00_s_gen_bits_u3( bcore_prsg_lcg_u3_00_s* o, sz_t bits )
{
    // bcore_prsg.h:110:5
    
    bcore_prsg_lcg_u3_00_s_gen( o );
    return  bcore_prsg_lcg_u3_00_s_state_bits_u3( o,bits );
}

f3_t bcore_prsg_lcg_u3_00_s_state_f3( const bcore_prsg_lcg_u3_00_s* o, f3_t min, f3_t max )
{
    // bcore_prsg.h:116:5
    
    return  min + ( max - min ) * ( ( f3_t )( bcore_prsg_lcg_u3_00_s_state_u3( o ) - bcore_prsg_lcg_u3_00_s_min_u3( o ) ) / ( bcore_prsg_lcg_u3_00_s_max_u3( o ) - bcore_prsg_lcg_u3_00_s_min_u3( o ) ) );
}

f3_t bcore_prsg_lcg_u3_00_s_gen_f3( bcore_prsg_lcg_u3_00_s* o, f3_t min, f3_t max )
{
    // bcore_prsg.h:121:5
    
    bcore_prsg_lcg_u3_00_s_gen( o );
    return  bcore_prsg_lcg_u3_00_s_state_f3( o,min, max );
}

BCORE_DEFINE_OBJECT_INST_P( bcore_prsg_lcg_u3_01_s )
"aware bcore_prsg"
"{"
    "u3_t state = 16437;"
    "func ^:gen_u3;"
    "func ^:state_bits_u3;"
    "func ^:gen_bits_u3;"
    "func ^:state_f3;"
    "func ^:gen_f3;"
    "func ^:bits;"
    "func ^:max_u3;"
    "func ^:min_u3;"
    "func ^:state_u3;"
    "func ^:set_state_u3;"
    "func ^:gen;"
"}";

u3_t bcore_prsg_lcg_u3_01_s_gen_u3( bcore_prsg_lcg_u3_01_s* o )
{
    // bcore_prsg.h:90:5
    
    bcore_prsg_lcg_u3_01_s_gen( o );
    return  bcore_prsg_lcg_u3_01_s_state_u3( o );
}

u3_t bcore_prsg_lcg_u3_01_s_state_bits_u3( const bcore_prsg_lcg_u3_01_s* o, sz_t bits )
{
    // bcore_prsg.h:96:5
    
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_lcg_u3_01_s_bits( o );
    if( o_bits >= bits )
    {
        return  bcore_prsg_lcg_u3_01_s_state_u3( o ) >> ( o_bits - bits );
    }
    else
    {
        return  bcore_prsg_lcg_u3_01_s_state_u3( o ) << ( bits - o_bits );
    }
}

u3_t bcore_prsg_lcg_u3_01_s_gen_bits_u3( bcore_prsg_lcg_u3_01_s* o, sz_t bits )
{
    // bcore_prsg.h:110:5
    
    bcore_prsg_lcg_u3_01_s_gen( o );
    return  bcore_prsg_lcg_u3_01_s_state_bits_u3( o,bits );
}

f3_t bcore_prsg_lcg_u3_01_s_state_f3( const bcore_prsg_lcg_u3_01_s* o, f3_t min, f3_t max )
{
    // bcore_prsg.h:116:5
    
    return  min + ( max - min ) * ( ( f3_t )( bcore_prsg_lcg_u3_01_s_state_u3( o ) - bcore_prsg_lcg_u3_01_s_min_u3( o ) ) / ( bcore_prsg_lcg_u3_01_s_max_u3( o ) - bcore_prsg_lcg_u3_01_s_min_u3( o ) ) );
}

f3_t bcore_prsg_lcg_u3_01_s_gen_f3( bcore_prsg_lcg_u3_01_s* o, f3_t min, f3_t max )
{
    // bcore_prsg.h:121:5
    
    bcore_prsg_lcg_u3_01_s_gen( o );
    return  bcore_prsg_lcg_u3_01_s_state_f3( o,min, max );
}

BCORE_DEFINE_OBJECT_INST_P( bcore_prsg_lcg_u3_02_s )
"aware bcore_prsg"
"{"
    "u3_t state = 16437;"
    "func ^:gen_u3;"
    "func ^:state_bits_u3;"
    "func ^:gen_bits_u3;"
    "func ^:state_f3;"
    "func ^:gen_f3;"
    "func ^:bits;"
    "func ^:max_u3;"
    "func ^:min_u3;"
    "func ^:state_u3;"
    "func ^:set_state_u3;"
    "func ^:gen;"
"}";

u3_t bcore_prsg_lcg_u3_02_s_gen_u3( bcore_prsg_lcg_u3_02_s* o )
{
    // bcore_prsg.h:90:5
    
    bcore_prsg_lcg_u3_02_s_gen( o );
    return  bcore_prsg_lcg_u3_02_s_state_u3( o );
}

u3_t bcore_prsg_lcg_u3_02_s_state_bits_u3( const bcore_prsg_lcg_u3_02_s* o, sz_t bits )
{
    // bcore_prsg.h:96:5
    
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_lcg_u3_02_s_bits( o );
    if( o_bits >= bits )
    {
        return  bcore_prsg_lcg_u3_02_s_state_u3( o ) >> ( o_bits - bits );
    }
    else
    {
        return  bcore_prsg_lcg_u3_02_s_state_u3( o ) << ( bits - o_bits );
    }
}

u3_t bcore_prsg_lcg_u3_02_s_gen_bits_u3( bcore_prsg_lcg_u3_02_s* o, sz_t bits )
{
    // bcore_prsg.h:110:5
    
    bcore_prsg_lcg_u3_02_s_gen( o );
    return  bcore_prsg_lcg_u3_02_s_state_bits_u3( o,bits );
}

f3_t bcore_prsg_lcg_u3_02_s_state_f3( const bcore_prsg_lcg_u3_02_s* o, f3_t min, f3_t max )
{
    // bcore_prsg.h:116:5
    
    return  min + ( max - min ) * ( ( f3_t )( bcore_prsg_lcg_u3_02_s_state_u3( o ) - bcore_prsg_lcg_u3_02_s_min_u3( o ) ) / ( bcore_prsg_lcg_u3_02_s_max_u3( o ) - bcore_prsg_lcg_u3_02_s_min_u3( o ) ) );
}

f3_t bcore_prsg_lcg_u3_02_s_gen_f3( bcore_prsg_lcg_u3_02_s* o, f3_t min, f3_t max )
{
    // bcore_prsg.h:121:5
    
    bcore_prsg_lcg_u3_02_s_gen( o );
    return  bcore_prsg_lcg_u3_02_s_state_f3( o,min, max );
}

BCORE_DEFINE_OBJECT_INST_P( bcore_prsg_lcg_u3_03_s )
"aware bcore_prsg"
"{"
    "u3_t state = 16437;"
    "func ^:gen_u3;"
    "func ^:state_bits_u3;"
    "func ^:gen_bits_u3;"
    "func ^:state_f3;"
    "func ^:gen_f3;"
    "func ^:bits;"
    "func ^:max_u3;"
    "func ^:min_u3;"
    "func ^:state_u3;"
    "func ^:set_state_u3;"
    "func ^:gen;"
"}";

u3_t bcore_prsg_lcg_u3_03_s_gen_u3( bcore_prsg_lcg_u3_03_s* o )
{
    // bcore_prsg.h:90:5
    
    bcore_prsg_lcg_u3_03_s_gen( o );
    return  bcore_prsg_lcg_u3_03_s_state_u3( o );
}

u3_t bcore_prsg_lcg_u3_03_s_state_bits_u3( const bcore_prsg_lcg_u3_03_s* o, sz_t bits )
{
    // bcore_prsg.h:96:5
    
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_lcg_u3_03_s_bits( o );
    if( o_bits >= bits )
    {
        return  bcore_prsg_lcg_u3_03_s_state_u3( o ) >> ( o_bits - bits );
    }
    else
    {
        return  bcore_prsg_lcg_u3_03_s_state_u3( o ) << ( bits - o_bits );
    }
}

u3_t bcore_prsg_lcg_u3_03_s_gen_bits_u3( bcore_prsg_lcg_u3_03_s* o, sz_t bits )
{
    // bcore_prsg.h:110:5
    
    bcore_prsg_lcg_u3_03_s_gen( o );
    return  bcore_prsg_lcg_u3_03_s_state_bits_u3( o,bits );
}

f3_t bcore_prsg_lcg_u3_03_s_state_f3( const bcore_prsg_lcg_u3_03_s* o, f3_t min, f3_t max )
{
    // bcore_prsg.h:116:5
    
    return  min + ( max - min ) * ( ( f3_t )( bcore_prsg_lcg_u3_03_s_state_u3( o ) - bcore_prsg_lcg_u3_03_s_min_u3( o ) ) / ( bcore_prsg_lcg_u3_03_s_max_u3( o ) - bcore_prsg_lcg_u3_03_s_min_u3( o ) ) );
}

f3_t bcore_prsg_lcg_u3_03_s_gen_f3( bcore_prsg_lcg_u3_03_s* o, f3_t min, f3_t max )
{
    // bcore_prsg.h:121:5
    
    bcore_prsg_lcg_u3_03_s_gen( o );
    return  bcore_prsg_lcg_u3_03_s_state_f3( o,min, max );
}

XOILA_DEFINE_SPECT( bcore_prsg, bcore_prsg_lcg )
"{"
    "bcore_spect_header_s header;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_prsg_xsg

BCORE_DEFINE_OBJECT_INST_P( bcore_prsg_xsg_u2_00_s )
"aware bcore_prsg"
"{"
    "u2_t state = 16432;"
    "func ^:gen_u3;"
    "func ^:state_bits_u3;"
    "func ^:gen_bits_u3;"
    "func ^:state_f3;"
    "func ^:gen_f3;"
    "func ^:bits;"
    "func ^:max_u3;"
    "func ^:min_u3;"
    "func ^:state_u3;"
    "func ^:set_state_u3;"
    "func ^:gen;"
"}";

u3_t bcore_prsg_xsg_u2_00_s_gen_u3( bcore_prsg_xsg_u2_00_s* o )
{
    // bcore_prsg.h:90:5
    
    bcore_prsg_xsg_u2_00_s_gen( o );
    return  bcore_prsg_xsg_u2_00_s_state_u3( o );
}

u3_t bcore_prsg_xsg_u2_00_s_state_bits_u3( const bcore_prsg_xsg_u2_00_s* o, sz_t bits )
{
    // bcore_prsg.h:96:5
    
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_xsg_u2_00_s_bits( o );
    if( o_bits >= bits )
    {
        return  bcore_prsg_xsg_u2_00_s_state_u3( o ) >> ( o_bits - bits );
    }
    else
    {
        return  bcore_prsg_xsg_u2_00_s_state_u3( o ) << ( bits - o_bits );
    }
}

u3_t bcore_prsg_xsg_u2_00_s_gen_bits_u3( bcore_prsg_xsg_u2_00_s* o, sz_t bits )
{
    // bcore_prsg.h:110:5
    
    bcore_prsg_xsg_u2_00_s_gen( o );
    return  bcore_prsg_xsg_u2_00_s_state_bits_u3( o,bits );
}

f3_t bcore_prsg_xsg_u2_00_s_state_f3( const bcore_prsg_xsg_u2_00_s* o, f3_t min, f3_t max )
{
    // bcore_prsg.h:116:5
    
    return  min + ( max - min ) * ( ( f3_t )( bcore_prsg_xsg_u2_00_s_state_u3( o ) - bcore_prsg_xsg_u2_00_s_min_u3( o ) ) / ( bcore_prsg_xsg_u2_00_s_max_u3( o ) - bcore_prsg_xsg_u2_00_s_min_u3( o ) ) );
}

f3_t bcore_prsg_xsg_u2_00_s_gen_f3( bcore_prsg_xsg_u2_00_s* o, f3_t min, f3_t max )
{
    // bcore_prsg.h:121:5
    
    bcore_prsg_xsg_u2_00_s_gen( o );
    return  bcore_prsg_xsg_u2_00_s_state_f3( o,min, max );
}

BCORE_DEFINE_OBJECT_INST_P( bcore_prsg_xsg_u2_01_s )
"aware bcore_prsg"
"{"
    "u2_t state = 16432;"
    "func ^:gen_u3;"
    "func ^:state_bits_u3;"
    "func ^:gen_bits_u3;"
    "func ^:state_f3;"
    "func ^:gen_f3;"
    "func ^:bits;"
    "func ^:max_u3;"
    "func ^:min_u3;"
    "func ^:state_u3;"
    "func ^:set_state_u3;"
    "func ^:gen;"
"}";

u3_t bcore_prsg_xsg_u2_01_s_gen_u3( bcore_prsg_xsg_u2_01_s* o )
{
    // bcore_prsg.h:90:5
    
    bcore_prsg_xsg_u2_01_s_gen( o );
    return  bcore_prsg_xsg_u2_01_s_state_u3( o );
}

u3_t bcore_prsg_xsg_u2_01_s_state_bits_u3( const bcore_prsg_xsg_u2_01_s* o, sz_t bits )
{
    // bcore_prsg.h:96:5
    
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_xsg_u2_01_s_bits( o );
    if( o_bits >= bits )
    {
        return  bcore_prsg_xsg_u2_01_s_state_u3( o ) >> ( o_bits - bits );
    }
    else
    {
        return  bcore_prsg_xsg_u2_01_s_state_u3( o ) << ( bits - o_bits );
    }
}

u3_t bcore_prsg_xsg_u2_01_s_gen_bits_u3( bcore_prsg_xsg_u2_01_s* o, sz_t bits )
{
    // bcore_prsg.h:110:5
    
    bcore_prsg_xsg_u2_01_s_gen( o );
    return  bcore_prsg_xsg_u2_01_s_state_bits_u3( o,bits );
}

f3_t bcore_prsg_xsg_u2_01_s_state_f3( const bcore_prsg_xsg_u2_01_s* o, f3_t min, f3_t max )
{
    // bcore_prsg.h:116:5
    
    return  min + ( max - min ) * ( ( f3_t )( bcore_prsg_xsg_u2_01_s_state_u3( o ) - bcore_prsg_xsg_u2_01_s_min_u3( o ) ) / ( bcore_prsg_xsg_u2_01_s_max_u3( o ) - bcore_prsg_xsg_u2_01_s_min_u3( o ) ) );
}

f3_t bcore_prsg_xsg_u2_01_s_gen_f3( bcore_prsg_xsg_u2_01_s* o, f3_t min, f3_t max )
{
    // bcore_prsg.h:121:5
    
    bcore_prsg_xsg_u2_01_s_gen( o );
    return  bcore_prsg_xsg_u2_01_s_state_f3( o,min, max );
}

BCORE_DEFINE_OBJECT_INST_P( bcore_prsg_xsg_u2_02_s )
"aware bcore_prsg"
"{"
    "u2_t state = 16432;"
    "func ^:gen_u3;"
    "func ^:state_bits_u3;"
    "func ^:gen_bits_u3;"
    "func ^:state_f3;"
    "func ^:gen_f3;"
    "func ^:bits;"
    "func ^:max_u3;"
    "func ^:min_u3;"
    "func ^:state_u3;"
    "func ^:set_state_u3;"
    "func ^:gen;"
"}";

u3_t bcore_prsg_xsg_u2_02_s_gen_u3( bcore_prsg_xsg_u2_02_s* o )
{
    // bcore_prsg.h:90:5
    
    bcore_prsg_xsg_u2_02_s_gen( o );
    return  bcore_prsg_xsg_u2_02_s_state_u3( o );
}

u3_t bcore_prsg_xsg_u2_02_s_state_bits_u3( const bcore_prsg_xsg_u2_02_s* o, sz_t bits )
{
    // bcore_prsg.h:96:5
    
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_xsg_u2_02_s_bits( o );
    if( o_bits >= bits )
    {
        return  bcore_prsg_xsg_u2_02_s_state_u3( o ) >> ( o_bits - bits );
    }
    else
    {
        return  bcore_prsg_xsg_u2_02_s_state_u3( o ) << ( bits - o_bits );
    }
}

u3_t bcore_prsg_xsg_u2_02_s_gen_bits_u3( bcore_prsg_xsg_u2_02_s* o, sz_t bits )
{
    // bcore_prsg.h:110:5
    
    bcore_prsg_xsg_u2_02_s_gen( o );
    return  bcore_prsg_xsg_u2_02_s_state_bits_u3( o,bits );
}

f3_t bcore_prsg_xsg_u2_02_s_state_f3( const bcore_prsg_xsg_u2_02_s* o, f3_t min, f3_t max )
{
    // bcore_prsg.h:116:5
    
    return  min + ( max - min ) * ( ( f3_t )( bcore_prsg_xsg_u2_02_s_state_u3( o ) - bcore_prsg_xsg_u2_02_s_min_u3( o ) ) / ( bcore_prsg_xsg_u2_02_s_max_u3( o ) - bcore_prsg_xsg_u2_02_s_min_u3( o ) ) );
}

f3_t bcore_prsg_xsg_u2_02_s_gen_f3( bcore_prsg_xsg_u2_02_s* o, f3_t min, f3_t max )
{
    // bcore_prsg.h:121:5
    
    bcore_prsg_xsg_u2_02_s_gen( o );
    return  bcore_prsg_xsg_u2_02_s_state_f3( o,min, max );
}

BCORE_DEFINE_OBJECT_INST_P( bcore_prsg_xsg_u2_03_s )
"aware bcore_prsg"
"{"
    "u2_t state = 16432;"
    "func ^:gen_u3;"
    "func ^:state_bits_u3;"
    "func ^:gen_bits_u3;"
    "func ^:state_f3;"
    "func ^:gen_f3;"
    "func ^:bits;"
    "func ^:max_u3;"
    "func ^:min_u3;"
    "func ^:state_u3;"
    "func ^:set_state_u3;"
    "func ^:gen;"
"}";

u3_t bcore_prsg_xsg_u2_03_s_gen_u3( bcore_prsg_xsg_u2_03_s* o )
{
    // bcore_prsg.h:90:5
    
    bcore_prsg_xsg_u2_03_s_gen( o );
    return  bcore_prsg_xsg_u2_03_s_state_u3( o );
}

u3_t bcore_prsg_xsg_u2_03_s_state_bits_u3( const bcore_prsg_xsg_u2_03_s* o, sz_t bits )
{
    // bcore_prsg.h:96:5
    
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_xsg_u2_03_s_bits( o );
    if( o_bits >= bits )
    {
        return  bcore_prsg_xsg_u2_03_s_state_u3( o ) >> ( o_bits - bits );
    }
    else
    {
        return  bcore_prsg_xsg_u2_03_s_state_u3( o ) << ( bits - o_bits );
    }
}

u3_t bcore_prsg_xsg_u2_03_s_gen_bits_u3( bcore_prsg_xsg_u2_03_s* o, sz_t bits )
{
    // bcore_prsg.h:110:5
    
    bcore_prsg_xsg_u2_03_s_gen( o );
    return  bcore_prsg_xsg_u2_03_s_state_bits_u3( o,bits );
}

f3_t bcore_prsg_xsg_u2_03_s_state_f3( const bcore_prsg_xsg_u2_03_s* o, f3_t min, f3_t max )
{
    // bcore_prsg.h:116:5
    
    return  min + ( max - min ) * ( ( f3_t )( bcore_prsg_xsg_u2_03_s_state_u3( o ) - bcore_prsg_xsg_u2_03_s_min_u3( o ) ) / ( bcore_prsg_xsg_u2_03_s_max_u3( o ) - bcore_prsg_xsg_u2_03_s_min_u3( o ) ) );
}

f3_t bcore_prsg_xsg_u2_03_s_gen_f3( bcore_prsg_xsg_u2_03_s* o, f3_t min, f3_t max )
{
    // bcore_prsg.h:121:5
    
    bcore_prsg_xsg_u2_03_s_gen( o );
    return  bcore_prsg_xsg_u2_03_s_state_f3( o,min, max );
}

XOILA_DEFINE_SPECT( bcore_prsg, bcore_prsg_xsg )
"{"
    "bcore_spect_header_s header;"
"}";

/**********************************************************************************************************************/


vd_t bcore_xo_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_xo" ) ) )
    {
        case TYPEOF_init1:
        {

            // --------------------------------------------------------------------
            // source: bcore_x_root_inexpandable.h

            // group: bcore_x_root_expandable
            BCORE_REGISTER_NAME( bcore_self_item_s );
            XOILA_REGISTER_SPECT( bcore_x_root_expandable );

            // --------------------------------------------------------------------
            // source: bcore_x_inst.h

            // group: x_inst
            XOILA_REGISTER_SPECT( x_inst );

            // --------------------------------------------------------------------
            // source: bcore_x_array.h

            // group: x_array
            BCORE_REGISTER_NAME( size );
            BCORE_REGISTER_NAME( space );
            BCORE_REGISTER_NAME( data );
            XOILA_REGISTER_SPECT( x_array );

            // --------------------------------------------------------------------
            // source: bcore_file.h

            // group: bcore_file
            BCORE_REGISTER_FFUNC( bcore_via_call_source, bcore_file_path_s_source );
            BCORE_REGISTER_OBJECT( bcore_file_path_s );
            XOILA_REGISTER_SPECT( bcore_file );

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
            XOILA_REGISTER_SPECT( bcore_inst_call );

            // --------------------------------------------------------------------
            // source: bcore_spect_via_call.h

            // group: bcore_via_call
            BCORE_REGISTER_FEATURE( bcore_via_call_source );
            BCORE_REGISTER_FEATURE( bcore_via_call_mutated );
            BCORE_REGISTER_FEATURE( bcore_via_call_shelve );
            XOILA_REGISTER_SPECT( bcore_via_call );

            // --------------------------------------------------------------------
            // source: bcore_main.h

            // group: bcore_main
            BCORE_REGISTER_OBJECT( bcore_main_frame_s );
            BCORE_REGISTER_FEATURE( bcore_main_main );
            BCORE_REGISTER_OBJECT( bcore_main_arr_s );
            BCORE_REGISTER_FFUNC( bcore_main_main, bcore_main_set_s_main );
            BCORE_REGISTER_OBJECT( bcore_main_set_s );
            XOILA_REGISTER_SPECT( bcore_main );

            // --------------------------------------------------------------------
            // source: bcore_hmap_name.h

            // group: bcore_hmap_name
            BCORE_REGISTER_OBJECT( bcore_hmap_name_s );
            XOILA_REGISTER_SPECT( bcore_hmap_name );

            // --------------------------------------------------------------------
            // source: bcore_cday.h

            // group: bcore_cday
            BCORE_REGISTER_OBJECT( bcore_cday_ymd_s );
            BCORE_REGISTER_OBJECT( bcore_cday_utc_s );
            XOILA_REGISTER_SPECT( bcore_cday );

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
            XOILA_REGISTER_SPECT( bcore_error_manager );

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
            BCORE_REGISTER_FFUNC( bcore_prsg_state_bl, bcore_prsg_state_bl_default );
            BCORE_REGISTER_FEATURE( bcore_prsg_gen );
            BCORE_REGISTER_FEATURE( bcore_prsg_gen_bits_u3 );
            BCORE_REGISTER_FEATURE( bcore_prsg_gen_u3 );
            BCORE_REGISTER_FEATURE( bcore_prsg_gen_f3 );
            BCORE_REGISTER_FEATURE( bcore_prsg_gen_bl );
            BCORE_REGISTER_FFUNC( bcore_prsg_gen_bl, bcore_prsg_gen_bl_default );
            BCORE_REGISTER_FEATURE( bcore_prsg_set_state_u3 );
            BCORE_REGISTER_FEATURE( bcore_prsg_set_state_mix );
            BCORE_REGISTER_FFUNC( bcore_prsg_set_state_mix, bcore_prsg_set_state_mix_default );
            BCORE_REGISTER_FEATURE( bcore_prsg_reseed );
            BCORE_REGISTER_FFUNC( bcore_prsg_reseed, bcore_prsg_reseed_default );
            XOILA_REGISTER_SPECT( bcore_prsg );

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
            XOILA_REGISTER_SPECT( bcore_prsg_lcg );

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
            XOILA_REGISTER_SPECT( bcore_prsg_xsg );
        }
        break;
        default: break;
    }
    return NULL;
}
// XOILA_OUT_SIGNATURE 0xD474EAED7949E694ull
