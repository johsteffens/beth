//  Last update: 2021-03-18T11:08:32Z
/** This file was generated from xoila source code.
 *  Compiling Agent : xoico_compiler (C) 2020 ... 2021 J.B.Steffens
 *
 *  Copyright and License of this File:
 *
 *  Generated code inherits the copyright and license of the underlying xoila source code.
 *  Source code defining this file is distributed across following files:
 *
 *  bcore_x_root_inexpandable.h
 *  bcore_x_inst.h
 *  bcore_x_array.h
 *  bcore_x_via.h
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
 *  bcore_main.x
 *  bcore_sink_inexpandable.x
 *  bcore_source_inexpandable.x
 *  bcore_sr_inexpandable.x
 *  bcore_st_inexpandable.x
 *  bcore_via_inexpandable.x
 *
 */

#include "bcore.xo.h"
#include "bcore_spect.h"
#include "bcore_spect_inst.h"
#include "bcore_sr.h"
#include "bcore_const_manager.h"

// To force a rebuild of this target by xoico, reset the hash key value below to 0.
// HKEYOF_bcore 0xF7DFF0478DC5F154ull

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
    // bcore_x_array.h:51:76
    
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
    // bcore_x_array.h:82:76
    
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
// source: bcore_x_via.h
#include "bcore_x_via.h"

//----------------------------------------------------------------------------------------------------------------------
// group: x_via

BCORE_DEFINE_OBJECT_INST_P( x_via_path_s )
"aware x_inst"
"{"
    "tp_t [];"
"}";

void x_via_path_s_push_index( x_via_path_s* o, sz_t index )
{
    // bcore_x_via.h:98:5
    
    x_via_path_s_push(o,TYPEOF_x_via_path_s_array_index );
    x_via_path_s_push(o,index );
}

const x_via_path_s* x_via_path_s_to_sink( const x_via_path_s* o, bcore_sink* sink )
{
    // bcore_x_via.h:141:1
    
    for(sz_t i = 0; i < o->size; i++ )
    {
        if( i > 0 ) bcore_sink_a_push_fa(sink,".", o->data[ i ] );
        if( o->data[ i ] == TYPEOF_x_via_path_s_array_index )
        {
            i++;
            bcore_sink_a_push_fa(sink,"[#<tp_t>]", o->data[ i ] );
        }
        else
        {
            bcore_sink_a_push_fa(sink,"#<sc_t>", bnameof( o->data[ i ] ) );
        }
    }
    return  o;
}

x_via_path_s* x_via_path_s_parse( x_via_path_s* o, bcore_source* source )
{
    // bcore_x_via.h:161:1
    BLM_INIT_LEVEL(0);
    ((x_via_path_s*)(x_via_path_s_clear(o)));
    st_s name;BLM_T_INIT_SPUSH(st_s, &name);;
    
    while( !bcore_source_a_eos(source) )
    {
        st_s_clear(&(name));
        if( bcore_source_a_parse_bl(source," #?'['" ) )
        {
            uz_t index = 0;
            bcore_source_a_parse_fa(source," #<uz_t*>", &index );
            bcore_source_a_parse_fa(source," ]" );
            x_via_path_s_push_index(o,index );
        }
        else
        {
            bcore_source_a_parse_fa(source," #name", (&(name)) );
            if( name.size == 0 ) break;
            x_via_path_s_push(o,btypeof( name.sc ) );
        }
        if( !bcore_source_a_parse_bl(source," #?'.'" ) ) break;
    }
    
    BLM_RETURNV(x_via_path_s*, o)
}

sr_s x_via_path_s_get_sr_in_t( const x_via_path_s* o, tp_t t, const x_inst* inst )
{
    // bcore_x_via.h:191:1
    
    sr_s sr0 = sr_twc(t, inst );
    if( !sr0.o ) return  sr_null();
    
    const bcore_via_s* p = NULL;
    sz_t index = -1;
    
    bl_t found = true;
    
    for(sz_t i = 0; i < o->size; i++ )
    {
        const bcore_via* via = ((const bcore_via*)(sr0.o));
        p = ( p && index >= 0 ) ? bcore_via_p_iget_via( p, via, index ) : NULL;
        if( !p ) p = bcore_via_s_get_typed( sr_s_type( &sr0 ) );
        if( !p ) { found = false; break; }
        sr_s sr1 = sr_null();
    
        if( o->data[ i ] == TYPEOF_x_via_path_s_array_index )
        {
            if( !bcore_spect_trait_supported( TYPEOF_bcore_array, sr_s_type( &sr0 ) ) ) { found = false; break; }
            const bcore_array_s* p_array = bcore_array_s_get_typed( sr_s_type( &sr0 ) );
            const bcore_array* array = ( const bcore_array* )via;
            i++;
            uz_t arr_index = o->data[ i ];
            uz_t arr_size = bcore_array_p_get_size( p_array, array );
            if( arr_index >= arr_size ) { found = false; break; }
            sr1 = bcore_array_p_get( p_array, array, arr_index );
            p = NULL;
        }
        else
        {
            index = bcore_via_p_nget_index( p, via, o->data[ i ] );
            if( index < 0 ) { found = false; break; }
            sr1 = bcore_via_p_iget( p, via, index );
        }
        sr_down( sr0 );
        sr0 = sr1;
        if( !sr0.o ) return  sr_null();
    }
    
    if( !found ) sr_s_clear(&(sr0));
    
    return  sr0;
}

sr_s x_via_path_s_set_sr_in_t( const x_via_path_s* o, tp_t t, x_inst* inst, sr_s sr_src )
{
    // bcore_x_via.h:240:1
    BLM_INIT_LEVEL(0);
    sr_s sr0 = sr_twc(t, inst );
    
    sr_s src_hook;BLM_T_INIT_SPUSH(sr_s, &src_hook);;
    src_hook = sr_src;
    sr_s_set_strong(&(sr_src),false );
    
    if( !sr0.o ) BLM_RETURNV(sr_s, sr_null())
    
    if( o->size == 0 && sr_src.o ) bcore_inst_x_copy_typed( sr0, sr_s_type(&(sr_src)), sr_src.o );
    
    const bcore_via_s* p = NULL;
    sz_t index = -1;
    
    bl_t found = true;
    
    for(sz_t i = 0; i < o->size; i++ )
    {
        bcore_via* via = ((bcore_via*)(sr0.o));
        p = ( p && index >= 0 ) ? bcore_via_p_iget_via( p, via, index ) : NULL;
        if( !p ) p = bcore_via_s_get_typed( sr_s_type( &sr0 ) );
        if( !p ) { found = false; break; }
    
        sr_s sr1 = sr_null();
    
        if( o->data[ i ] == TYPEOF_x_via_path_s_array_index )
        {
            if( !bcore_spect_trait_supported( TYPEOF_bcore_array, sr_s_type( &sr0 ) ) ) { found = false; break; }
            const bcore_array_s* p_array = bcore_array_s_get_typed( sr_s_type( &sr0 ) );
            bcore_array* array =((bcore_array*)( via));
            i++;
            uz_t arr_index = o->data[ i ];
            uz_t arr_size = bcore_array_p_get_size( p_array, array );
            if( arr_index >= arr_size ) { found = false; break; }
    
            if( i + 1 == o->size ) bcore_array_p_set( p_array, array, arr_index, sr_src );
    
            sr1 = bcore_array_p_get( p_array, array, arr_index );
            p = NULL;
        }
        else
        {
            index = bcore_via_p_nget_index( p, via, o->data[ i ] );
            if( index < 0 ) { found = false; break; }
    
            if( i + 1 == o->size ) bcore_via_p_iset( p, via, index, sr_src );
    
            sr1 = bcore_via_p_iget( p, via, index );
        }
        sr_down( sr0 );
        sr0 = sr1;
        if( !sr0.o ) BLM_RETURNV(sr_s, sr_null())
    }
    
    if( !found ) sr_s_clear(&(sr0));
    
    BLM_RETURNV(sr_s, sr0)
}

BCORE_DEFINE_OBJECT_INST_P( x_via_path_adl_s )
"aware x_array"
"{"
    "x_via_path_s => [];"
"}";

XOILA_DEFINE_SPECT( x_inst, x_via )
"{"
    "bcore_spect_header_s header;"
"}";

sr_s x_via_t_m_get_sr( tp_t t, x_via* o, tp_t name )
{
    // bcore_x_via.h:306:1
    
    const bcore_via_s* p = bcore_via_s_get_typed( t );
    sz_t index = bcore_via_p_nget_index( p, ((const bcore_via*)(o)), name );
    if( index < 0 ) return  sr_null();
    return  bcore_via_p_iget( p, ((const bcore_via*)(o)), index );
}

sr_s x_via_t_set_sr( tp_t t, x_via* o, tp_t name, sr_s sr_src )
{
    // bcore_x_via.h:317:1
    
    const bcore_via_s* p = bcore_via_s_get_typed( t );
    sz_t index = bcore_via_p_nget_index( p, ((const bcore_via*)(o)), name );
    if( index < 0 )
    {
        sr_s_down(&(sr_src));
        return  sr_null();
    }
    bcore_via_p_iset( p, ((bcore_via*)(o)), index, sr_src );
    sr_s sr = bcore_via_p_iget( p, ((const bcore_via*)(o)), index );
    if( sr_s_is_strong(&(sr)) ) sr_s_clear(&(sr));
    return  sr;
}

sr_s x_via_t_c_get_sr( tp_t t, const x_via* o, tp_t name )
{
    // bcore_x_via.h:335:1
    
    sr_s sr = x_via_t_m_get_sr(t, ((x_via*)(o)), name );
    if( sr_s_is_weak(&(sr)) ) sr_s_set_const(&(sr),true );
    return  sr;
}

void x_via_selftest( void )
{
    // bcore_x_via.h:348:1
    BLM_INIT_LEVEL(0);
    bcore_prsg_lcg_u3_00_s lcg;BLM_T_INIT_SPUSH(bcore_prsg_lcg_u3_00_s, &lcg);;
    
    ASSERT( x_via_exists(((const x_via*)(&(lcg))),btypeof( "state" ) ) );
    
    u3_t state =*( ((const u3_t*)(x_via_c_get(((const x_via*)(&(lcg))),btypeof( "state" ) ))));
    ASSERT( state == 16437 );
    
    x_via_set_sr(((x_via*)(&( lcg))),btypeof( "state" ), sr_u3(12345 ) );
    state = (*(((const u3_t*)(x_via_c_get(((const x_via*)(&( lcg))),btypeof( "state" ) )))));
    ASSERT( state == 12345 );
    
    const x_inst* inst = x_via_path_s_c_get_in(((x_via_path_s*)(x_via_path_s_parse_sc(((x_via_path_s*)BLM_LEVEL_T_PUSH(0,x_via_path_s,x_via_path_s_create())),"state" ))),((const x_inst*)(&(lcg ))));
    ASSERT( inst );
    ASSERT( (*(((const u3_t*)(inst)))) == 12345 );
    
    x_inst* zoo = ((x_inst*)BLM_LEVEL_A_PUSH(0,((x_inst*)(bcore_spect_via_create_zoo( 2 ).o))));
    
    sr_s sr = sr_null();
    
    sr = x_via_path_s_get_sr_in(((x_via_path_s*)(x_via_path_s_parse_sc(((x_via_path_s*)BLM_LEVEL_T_PUSH(0,x_via_path_s,x_via_path_s_create())),"[0].[1].vdata.data2.[1]" ))),zoo );
    ASSERT( sr_s_type(&(sr)) == TYPEOF_u3_t );
    ASSERT( sr_s_to_u3(&(sr)) == 0 );
    
    sr = x_via_path_s_set_sr_in(((x_via_path_s*)(x_via_path_s_parse_sc(((x_via_path_s*)BLM_LEVEL_T_PUSH(0,x_via_path_s,x_via_path_s_create())),"[0].[1].vdata.data2.[1]" ))),zoo, sr_u3(20 ) );
    ASSERT( sr_s_type(&(sr)) == TYPEOF_u3_t );
    ASSERT( sr_s_to_u3(&(sr)) == 20 );
    BLM_DOWN();
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
// group: bcore_main; embeds: bcore_main.x

BCORE_DEFINE_OBJECT_INST_P( bcore_main_frame_s )
"aware bcore_inst"
"{"
    "aware bcore_interpreter => interpreter;"
    "bl_t first_argument_is_path_to_object = true;"
    "bl_t second_argument_is_path_to_script = true;"
    "sc_t local_file = \"beth.config\";"
    "sc_t global_file;"
    "bcore_arr_st_s args;"
    "hidden bcore_mutex_s mutex;"
    "hidden aware bcore_source -> source;"
    "hidden aware bcore_sink -> sink;"
    "sr_s object_sr;"
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
    "private bcore_main* current_object;"
    "bcore_mutex_s mutex_current_object;"
    "func ^:main;"
    "func ^:on_termination;"
    "func ^:on_interrupt;"
    "func ^:on_suspend;"
"}";

er_t bcore_main_set_s_main( bcore_main_set_s* o, bcore_main_frame_s* frame )
{
    // bcore_main.h:121:5
    
    {const bcore_main_arr_s* __a=&(o->arr );if(__a)for(sz_t __i=0; __i<__a->size; __i++){bcore_main* e=__a->data[__i];
    {
        {BLM_INIT_LEVEL(4);
            bcore_lock_s lock;BLM_T_INIT_SPUSH(bcore_lock_s, &lock);bcore_lock_s_set(&(lock),&(o->mutex_current_object ));
            o->current_object = e;
        BLM_DOWN();}
        BLM_TRY(bcore_main_a_main(e,frame ) )
    };}}
    o->current_object = NULL;
    return  0;
}

bl_t bcore_main_set_s_on_termination( bcore_main_set_s* o, const bcore_main_frame_s* frame )
{
    // bcore_main.h:135:5
    BLM_INIT_LEVEL(0);
    bcore_lock_s lock;BLM_T_INIT_SPUSH(bcore_lock_s, &lock);bcore_lock_s_set(&(lock),&(o->mutex_current_object ));
    BLM_RETURNV(bl_t, o->current_object ? bcore_main_a_on_termination(o->current_object,frame ) : false)
}

bl_t bcore_main_set_s_on_interrupt( bcore_main_set_s* o, const bcore_main_frame_s* frame )
{
    // bcore_main.h:141:5
    BLM_INIT_LEVEL(0);
    bcore_lock_s lock;BLM_T_INIT_SPUSH(bcore_lock_s, &lock);bcore_lock_s_set(&(lock),&(o->mutex_current_object ));
    BLM_RETURNV(bl_t, o->current_object ? bcore_main_a_on_interrupt(o->current_object,frame ) : false)
}

bl_t bcore_main_set_s_on_suspend( bcore_main_set_s* o, const bcore_main_frame_s* frame )
{
    // bcore_main.h:147:5
    BLM_INIT_LEVEL(0);
    bcore_lock_s lock;BLM_T_INIT_SPUSH(bcore_lock_s, &lock);bcore_lock_s_set(&(lock),&(o->mutex_current_object ));
    BLM_RETURNV(bl_t, o->current_object ? bcore_main_a_on_suspend(o->current_object,frame ) : false)
}

XOILA_DEFINE_SPECT( bcore_inst, bcore_main )
"{"
    "bcore_spect_header_s header;"
    "feature aware bcore_main : main;"
    "feature aware bcore_main : on_termination = bcore_main_on_termination_default;"
    "feature aware bcore_main : on_interrupt = bcore_main_on_interrupt_default;"
    "feature aware bcore_main : on_suspend = bcore_main_on_suspend_default;"
    "feature aware bcore_main : shell = bcore_main_shell_default;"
    "feature aware bcore_main : shell_help = bcore_main_shell_help_default;"
"}";

er_t bcore_main_shell_loop( bcore_main* o, const bcore_main_frame_s* frame, bcore_source* source, bcore_sink* sink, bcore_main_control* control )
{
    // bcore_main.x:79:1
    BLM_INIT_LEVEL(0);
    if( !control ) control =((bcore_main_control*)( ((bcore_main_control_s*)BLM_LEVEL_T_PUSH(0,bcore_main_control_s,bcore_main_control_s_create()))));
    
    while( !bcore_source_a_eos(source) )
    {
        bcore_sink_a_flush(bcore_sink_a_push_fa(sink,"\n(#<sc_t>)> ", bnameof( o->_ ) ));
        bcore_source_a_parse_fa(source," " );
    
        bcore_main_control_a_reset(control);
    
        if( bcore_main_a_shell(o,frame, source, sink, control ) )
        {
            if( bcore_main_control_a_exit_loop(control) ) break;
        }
        else if( bcore_source_a_parse_bl(source,"#?w'xo'" ) )
        {
            break;
        }
        else if( bcore_source_a_parse_bl(source,"#?w'exit'" ) )
        {
            break;
        }
        else if( bcore_source_a_parse_bl(source,"#?'?'" ) || bcore_source_a_parse_bl(source,"#?'help'" ) )
        {BLM_INIT_LEVEL(3);
            bcore_main_helper_s_to_sink(((bcore_main_helper_s*)(bcore_main_helper_s_setup(((bcore_main_helper_s*)BLM_LEVEL_T_PUSH(3,bcore_main_helper_s,bcore_main_helper_s_create())),o ))),sink );
        BLM_DOWN();}
        else
        {BLM_INIT_LEVEL(2);
            bcore_source_a_parse_fa(source,"#-until'\n'" );
            bcore_sink_a_push_fa(sink,"Invalid command.\n\n" );
            bcore_sink_a_push_fa(sink,"List of available commands:\n" );
            bcore_main_helper_s_to_sink(((bcore_main_helper_s*)(bcore_main_helper_s_setup(((bcore_main_helper_s*)BLM_LEVEL_T_PUSH(2,bcore_main_helper_s,bcore_main_helper_s_create())),o ))),sink );
        BLM_DOWN();}
    }
    
    BLM_RETURNV(er_t, 0)
}

bl_t bcore_main_shell_default( bcore_main* o, const bcore_main_frame_s* frame, bcore_source* source, bcore_sink* sink, bcore_main_control* control )
{
    // bcore_main.x:120:1
    
    if( bcore_source_a_parse_bl(source,"#?w'ls'" ) )
    {BLM_INIT_LEVEL(1);
        bcore_source_a_parse_fa(source,"#skip' '" );
    
        bl_t extensive = bcore_source_a_parse_bl(source,"#?'-l'" );
    
        x_via_path_s path;BLM_T_INIT_SPUSH(x_via_path_s, &path);((x_via_path_s*)(x_via_path_s_parse(&(path),((bcore_source*)(((bcore_source_string_s*)BLM_LEVEL_T_PUSH(1,bcore_source_string_s,bcore_source_string_s_create_parse_line(source ))) )))));
        sr_s sr = x_via_path_s_get_sr_in(&(path),((const x_inst*)(o )));
    
        if( sr.o )
        {
            if( extensive )
            {
                ((x_inst*)(x_inst_t_to_sink_txt_ml(sr_s_type(&( sr )), sr.o, sink )));
            }
            else
            {
                tp_t t = sr_s_type(&( sr ));
                const x_via* v =((const x_via*)( sr.o));
                sz_t size = x_via_t_size(t );
                for(sz_t i = 0; i < size; i++ )
                {
                    tp_t name = x_via_t_name(t, i );
                    tp_t type = x_via_t_type(t, v, name );
                    bcore_sink_a_push_fa(sink,"#<sc_t>: #<sc_t>\n", bnameof( name ), bnameof( type ) );
                }
            }
        }
        else
        {
            bcore_sink_a_push_fa(sink,"Path not found.\n" );
        }
        BLM_RETURNV(bl_t, true)
    BLM_DOWN();}
    else if( bcore_source_a_parse_bl(source,"#?w'eo'" ) )
    {BLM_INIT_LEVEL(2);
        bcore_source_a_parse_fa(source,"#skip' '" );
    
        x_via_path_s path;BLM_T_INIT_SPUSH(x_via_path_s, &path);((x_via_path_s*)(x_via_path_s_parse(&(path),((bcore_source*)(((bcore_source_string_s*)BLM_LEVEL_T_PUSH(2,bcore_source_string_s,bcore_source_string_s_create_parse_line(source ))) )))));
        sr_s sr = x_via_path_s_get_sr_in(&(path),((const x_inst*)(o )));
    
        if( sr.o )
        {
            tp_t t = sr_s_type(&( sr ));
            if( !x_via_t_is_aware(t ) )
            {
                bcore_sink_a_push_fa(sink,"Selected object '#<sc_t>' is oblivious.\n", bnameof( t ) );
                BLM_RETURNV(bl_t, true)
            }
    
            bcore_main* mo =((bcore_main*)( sr.o));
            bcore_main_shell_loop(mo,frame, source, sink, NULL );
        }
        else
        {
            bcore_sink_a_push_fa(sink,"Path not found.\n" );
        }
    
        BLM_RETURNV(bl_t, true)
    BLM_DOWN();}
    return  false;
}

void bcore_main_shell_help_default( const bcore_main* o, bcore_main_helper* helper )
{
    // bcore_main.x:187:1
    BLM_INIT_LEVEL(0);
    bcore_main_helper_a_push(helper,"?, help",        "Prints this help message." );
    bcore_main_helper_a_push(helper,"ls [-l] <path>", "Lists all visible members. -l: txt_ml to stdout" );
    bcore_main_helper_a_push(helper,"eo <path>",      "(Enter Object): Enters interactive loop in specified object." );
    bcore_main_helper_a_push(helper,"xo, exit",       st_s_push_fa(((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create())),"(Exit Object):  Leaves interactive loop in '#<sc_t>'.", bnameof( o->_ ) )->sc );
    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_main_control

BCORE_DEFINE_OBJECT_INST_P( bcore_main_control_s )
"aware bcore_main_control"
"{"
    "bl_t exit_loop;"
    "func ^:reset;"
    "func ^:request_exit_loop;"
    "func ^:exit_loop;"
"}";

XOILA_DEFINE_SPECT( bcore_main, bcore_main_control )
"{"
    "bcore_spect_header_s header;"
    "feature aware bcore_main_control : request_exit_loop;"
    "feature aware bcore_main_control : exit_loop;"
    "feature aware bcore_main_control : reset;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_main_helper

BCORE_DEFINE_OBJECT_INST_P( bcore_main_helper_item_s )
"aware bcore_main_helper"
"{"
    "st_s command;"
    "st_s description;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bcore_main_helper_items_s )
"aware x_array"
"{"
    "bcore_main_helper_item_s => [];"
"}";

BCORE_DEFINE_OBJECT_INST_P( bcore_main_helper_s )
"aware bcore_main_helper"
"{"
    "bcore_main_helper_items_s items;"
    "func ^:push;"
    "func ^:to_sink;"
"}";

void bcore_main_helper_s_push( const bcore_main_helper_s* o, sc_t command, sc_t description )
{
    // bcore_main.x:58:9
    BLM_INIT_LEVEL(0);
    bcore_main_helper_item_s item;BLM_T_INIT_SPUSH(bcore_main_helper_item_s, &item);;
    st_s_copy_sc(&(item.command),command );
    st_s_copy_sc(&(item.description),description );
    ((bcore_main_helper_item_s*)(x_array_push_c(((x_array*)(&(o->items))),((const x_inst*)(&(item ))))));
    BLM_DOWN();
}

void bcore_main_helper_s_to_sink( const bcore_main_helper_s* o, bcore_sink* sink )
{
    // bcore_main.x:66:9
    
    sz_t size_cmd = 0; {const bcore_main_helper_items_s* __a=&(o->items );if(__a)for(sz_t __i=0; __i<__a->size; __i++){const bcore_main_helper_item_s* e=__a->data[__i]; size_cmd = sz_max( size_cmd, e->command.size );}}
    {const bcore_main_helper_items_s* __a=&(o->items );if(__a)for(sz_t __i=0; __i<__a->size; __i++){const bcore_main_helper_item_s* e=__a->data[__i];
    {
        bcore_sink_a_push_fa(sink,"#pn' '{#<st_s*>}   #<st_s*>\n", size_cmd, (&(e->command)), (&(e->description)) );
    }
    }}
}

XOILA_DEFINE_SPECT( bcore_main, bcore_main_helper )
"{"
    "bcore_spect_header_s header;"
    "feature aware bcore_main_helper : push;"
    "feature aware bcore_main_helper : to_sink;"
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
            // source: bcore_x_via.h

            // group: x_via
            BCORE_REGISTER_NAME( x_via_path_s_array_index );
            BCORE_REGISTER_NAME( sr_s );
            BCORE_REGISTER_OBJECT( x_via_path_s );
            BCORE_REGISTER_OBJECT( x_via_path_adl_s );
            XOILA_REGISTER_SPECT( x_via );

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
            BCORE_REGISTER_FEATURE( bcore_main_on_termination );
            BCORE_REGISTER_FFUNC( bcore_main_on_termination, bcore_main_on_termination_default );
            BCORE_REGISTER_FEATURE( bcore_main_on_interrupt );
            BCORE_REGISTER_FFUNC( bcore_main_on_interrupt, bcore_main_on_interrupt_default );
            BCORE_REGISTER_FEATURE( bcore_main_on_suspend );
            BCORE_REGISTER_FFUNC( bcore_main_on_suspend, bcore_main_on_suspend_default );
            BCORE_REGISTER_OBJECT( bcore_main_arr_s );
            BCORE_REGISTER_FFUNC( bcore_main_main, bcore_main_set_s_main );
            BCORE_REGISTER_FFUNC( bcore_main_on_termination, bcore_main_set_s_on_termination );
            BCORE_REGISTER_FFUNC( bcore_main_on_interrupt, bcore_main_set_s_on_interrupt );
            BCORE_REGISTER_FFUNC( bcore_main_on_suspend, bcore_main_set_s_on_suspend );
            BCORE_REGISTER_OBJECT( bcore_main_set_s );
            BCORE_REGISTER_FEATURE( bcore_main_shell );
            BCORE_REGISTER_FFUNC( bcore_main_shell, bcore_main_shell_default );
            BCORE_REGISTER_FEATURE( bcore_main_shell_help );
            BCORE_REGISTER_FFUNC( bcore_main_shell_help, bcore_main_shell_help_default );
            XOILA_REGISTER_SPECT( bcore_main );

            // group: bcore_main_control
            BCORE_REGISTER_FEATURE( bcore_main_control_request_exit_loop );
            BCORE_REGISTER_FEATURE( bcore_main_control_exit_loop );
            BCORE_REGISTER_FEATURE( bcore_main_control_reset );
            BCORE_REGISTER_FFUNC( bcore_main_control_reset, bcore_main_control_s_reset );
            BCORE_REGISTER_FFUNC( bcore_main_control_request_exit_loop, bcore_main_control_s_request_exit_loop );
            BCORE_REGISTER_FFUNC( bcore_main_control_exit_loop, bcore_main_control_s_exit_loop );
            BCORE_REGISTER_OBJECT( bcore_main_control_s );
            XOILA_REGISTER_SPECT( bcore_main_control );

            // group: bcore_main_helper
            BCORE_REGISTER_FEATURE( bcore_main_helper_push );
            BCORE_REGISTER_FEATURE( bcore_main_helper_to_sink );
            BCORE_REGISTER_OBJECT( bcore_main_helper_item_s );
            BCORE_REGISTER_OBJECT( bcore_main_helper_items_s );
            BCORE_REGISTER_FFUNC( bcore_main_helper_push, bcore_main_helper_s_push );
            BCORE_REGISTER_FFUNC( bcore_main_helper_to_sink, bcore_main_helper_s_to_sink );
            BCORE_REGISTER_OBJECT( bcore_main_helper_s );
            XOILA_REGISTER_SPECT( bcore_main_helper );

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
// XOICO_BODY_SIGNATURE 0xB2BF1A202AF84453
// XOICO_FILE_SIGNATURE 0x8E6C6EDAC5618DE5
