//  Last update: 2023-08-28T14:06:55Z
/** This file was generated from xoila source code.
 *  Compiling Agent : XOICO (C) 2020 ... 2022 J.B.Steffens
 *  Note that any changes of this file can be erased or overwritten by XOICO.
 *
 *  Copyright and License of this File:
 *
 *  Unless explicitly stated otherwise in governing license terms, this file inherits the
 *  copyright and license terms of the immediate source code from which it was compiled.
 *
 *  The immediate source code is distributed across following files:
 *  bcore_x_root_inexpandable.h
 *  bcore_x_inst.h
 *  bcore_x_source.h
 *  bcore_x_sink.h
 *  bcore_x_array.h
 *  bcore_x_group.h
 *  bcore_x_stamp.h
 *  bcore_x_threads.h
 *  bcore_file.h
 *  bcore_spect_inst_call.h
 *  bcore_spect_via_call.h
 *  bcore_main.h
 *  bcore_hmap_name.h
 *  bcore_hmap_tp_st.h
 *  bcore_cday.h
 *  bcore_error_manager.h
 *  bcore_prsg.h
 *  bcore_shell.h
 *  bcore_huffman.h
 *  bcore_indexer.h
 *  bcore_x_btml.h
 *  bcore_x_bbml.h
 *  bcore_x_bcml.h
 *  bcore_x_hmap.h
 *  bcore_x_deque.h
 *  bcore_arr_inexpandable.x
 *  bcore_const_manager_inexpandable.x
 *  bcore_flect_inexpandable.x
 *  bcore_folder_inexpandable.x
 *  bcore_global_manager_inexpandable.x
 *  bcore_hmap_inexpandable.x
 *  bcore_huffman.x
 *  bcore_img_inexpandable.x
 *  bcore_sink_inexpandable.x
 *  bcore_source_inexpandable.x
 *  bcore_sr_inexpandable.x
 *  bcore_st_inexpandable.x
 *  bcore_via_inexpandable.x
 *  bcore_x_hmap.x
 *
 */

#include "bcore.xo.h"
#include "bcore_spect.h"
#include "bcore_spect_inst.h"
#include "bcore_sr.h"
#include "bcore_const_manager.h"

// To force a rebuild of this target by xoico, reset the hash key value below to 0.
// HKEYOF_bcore 0xD7356BC0F11DF8B3ull

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
#include "bcore_file.h"

XOILA_DEFINE_SPECT( bcore_inst, x_inst )
"{"
    "bcore_spect_header_s header;"
"}";

x_inst* x_inst_create( tp_t type )
{
    // bcore_x_inst.h:92:1
    
    if( type != 0 && !x_stamp_t_is_aware(type ) )
    {
        ERR_fa( "Function 'x_inst:create' can only be used for aware types. Use 'x_inst:t_create' instead." );
    }
    return ((x_inst*)( bcore_inst_t_create(type )));
}

/**********************************************************************************************************************/
// source: bcore_x_source.h
#include "bcore_x_source.h"

//----------------------------------------------------------------------------------------------------------------------
// group: x_source
#include "bcore_x_sink.h"

BCORE_DEFINE_OBJECT_INST_P( x_source_point_s )
"aware x_inst"
"{"
    "aware x_source -> source;"
    "s3_t index;"
"}";

void x_source_point_s_parse_msg_fv( const x_source_point_s* o, sc_t format, va_list args )
{
    // bcore_x_source.h:195:1
    
    x_source_point_s_parse_msg_to_sink_fv(o,x_sink_stdout(), format, args );
}

x_source* x_source_point_s_clone_source( const x_source_point_s* o )
{
    // bcore_x_source.h:202:1
    
    x_source* source = x_source_a_clone(o->source);
    x_source_set_index(source,o->index );
    return  source;
}

void x_source_point_s_parse_msg_to_sink_fv( const x_source_point_s* o, x_sink* sink, sc_t format, va_list args )
{
    // bcore_x_source.h:211:1
    
    if( o->source )
    {
        s3_t index = x_source_get_index(o->source);
        x_source_set_index(o->source,o->index );
        x_source_parse_msg_to_sink_fv(o->source,sink, format, args );
        x_source_set_index(o->source,index );
    }
    else
    {
        x_sink_push_fv(sink,format, args );
    }
}

er_t x_source_point_s_parse_error_fv( const x_source_point_s* o, sc_t format, va_list args )
{
    // bcore_x_source.h:228:1
    
    er_t err = 0;
    if( o->source )
    {
        s3_t index = x_source_get_index(o->source);
        x_source_set_index(o->source,o->index );
        err = x_source_parse_error_fv(o->source,format, args );
        x_source_set_index(o->source,index );
    }
    else
    {
        err = bcore_error_push_fv(TYPEOF_parse_error, format, args );
    }
    return  err;
}

void x_source_point_s_source_reference_to_sink( const x_source_point_s* o, bl_t file_name_only, x_sink* sink )
{
    // bcore_x_source.h:247:1
    BLM_INIT_LEVEL(0);
    if( !o->source ) BLM_RETURN();
    s3_t index = x_source_get_index(o->source);
    x_source_set_index(o->source,o->index );
    
    bcore_source_context_s* context = ((bcore_source_context_s*)BLM_LEVEL_T_PUSH(0,bcore_source_context_s,bcore_source_context_s_create()));
    bcore_source_a_get_context(((const bcore_source*)(o->source)),context );
    
    if( context->file_path )
    {BLM_INIT_LEVEL(1);
        st_s* file = ((st_s*)BLM_LEVEL_T_PUSH(1,st_s,st_s_clone(context->file_path)));
        if( file_name_only ) st_s_copy_sc(file,bcore_file_name(file->sc ) );
        x_sink_push_fa(sink,"#<sc_t>", file->sc );
    BLM_DOWN();}
    
    x_sink_push_fa(sink,":#<sz_t>:#<sz_t>", context->line, context->col );
    x_source_set_index(o->source,index );
    BLM_DOWN();
}

XOILA_DEFINE_SPECT( x_inst, x_source )
"{"
    "bcore_spect_header_s header;"
"}";

x_source* x_source_check_create_from_file( sc_t path )
{
    // bcore_x_source.h:53:1
    
    if( !bcore_file_exists(path ) ) ERR_fa( "File '#<sc_t>' not found.", path );
    return ((x_source*)( bcore_file_open_source(path )));
}

void x_source_parse_msg_fv( const x_source* o, sc_t format, va_list args )
{
    // bcore_x_source.h:146:1
    
    x_source_parse_msg_to_sink_fv(o,x_sink_stdout(), format, args );
}

/**********************************************************************************************************************/
// source: bcore_x_sink.h
#include "bcore_x_sink.h"

//----------------------------------------------------------------------------------------------------------------------
// group: x_sink

XOILA_DEFINE_SPECT( x_inst, x_sink )
"{"
    "bcore_spect_header_s header;"
"}";

x_sink* x_sink_check_create_from_file( sc_t path )
{
    // bcore_x_sink.h:41:1
    
    x_sink* sink =((x_sink*)( bcore_file_try_open_sink(path )));
    if( !sink ) ERR_fa( "Could no open file '#<sc_t>' as sink.", path );
    return  sink;
}

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
    // bcore_x_array.h:159:1
    
    
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
    // bcore_x_array.h:193:1
    
    
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
    // bcore_x_array.h:229:1
    
    
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
    // bcore_x_array.h:257:1
    
    
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

x_inst* x_array_t_set_d( x_array* o, tp_t t, sz_t index, x_inst* v )
{
    // bcore_x_array.h:301:1
    
    
        const bcore_array_s* p = bcore_array_s_get_typed( t );
    
        if( p->item_p )
        {
            bcore_array_p_set( p, ( bcore_array* )o, index, sr_psd( p->item_p, v ) );
        }
        else
        {
            bcore_array_p_set( p, ( bcore_array* )o, index, sr_asd( v ) );
        }
    
        if( p->is_of_links )
        {
            return v;
        }
        else if( p->is_static )
        {
            u0_t* data = bcore_array_p_get_d_data( p, o );
            return ( x_inst* )( data + p->item_p->size * index );
        }
        else
        {
            return bcore_array_p_get( p, ( bcore_array* )o, index ).o;
        }
}

x_inst* x_array_t_set_c( x_array* o, tp_t t, sz_t index, const x_inst* v )
{
    // bcore_x_array.h:334:1
    
    
        const bcore_array_s* p = bcore_array_s_get_typed( t );
    
        if( p->item_p )
        {
            bcore_array_p_set( p, ( bcore_array* )o, index, sr_pwc( p->item_p, v ) );
        }
        else
        {
            bcore_array_p_set( p, ( bcore_array* )o, index, sr_awc( v ) );
        }
    
        if( p->is_of_links )
        {
            x_inst** data = bcore_array_p_get_d_data( p, o );
            return data[ index ];
        }
        else if( p->is_static )
        {
            u0_t* data = bcore_array_p_get_d_data( p, o );
            return ( x_inst* )( data + p->item_p->size * index );
        }
        else
        {
            return bcore_array_p_get( p, ( bcore_array* )o, index ).o;
        }
}

x_inst* x_array_t_set_t( x_array* o, tp_t t, sz_t index, tp_t val_type )
{
    // bcore_x_array.h:368:1
    
    
        const bcore_array_s* p = bcore_array_s_get_typed( t );
        bcore_array_p_set( p, ( bcore_array* )o, index, sr_t_create( val_type ) );
    
        if( p->is_of_links )
        {
            x_inst** data = bcore_array_p_get_d_data( p, o );
            return data[ index ];
        }
        else if( p->is_static )
        {
            u0_t* data = bcore_array_p_get_d_data( p, o );
            return ( x_inst* )( data + p->item_p->size * index );
        }
        else
        {
            return bcore_array_p_get( p, ( bcore_array* )o, index ).o;
        }
}

sr_s x_array_t_c_get_sr( const x_array* o, tp_t t, sz_t index )
{
    // bcore_x_array.h:394:1
    
    sr_s sr = bcore_array_t_get( t, ((const bcore_array*)(o)), index );
    if( sr_s_is_weak(&(sr)) ) sr_s_set_const(&(sr),true );
    return  sr;
}

/**********************************************************************************************************************/
// source: bcore_x_group.h
#include "bcore_x_group.h"

//----------------------------------------------------------------------------------------------------------------------
// group: x_group

XOILA_DEFINE_SPECT( x_inst, x_group )
"{"
    "bcore_spect_header_s header;"
"}";

/**********************************************************************************************************************/
// source: bcore_x_stamp.h
#include "bcore_x_stamp.h"

//----------------------------------------------------------------------------------------------------------------------
// group: x_stamp

BCORE_DEFINE_OBJECT_INST_P( x_stamp_path_s )
"aware x_inst"
"{"
    "tp_t [];"
"}";

void x_stamp_path_s_push_index( x_stamp_path_s* o, sz_t index )
{
    // bcore_x_stamp.h:165:5
    
    x_stamp_path_s_push(o,TYPEOF_x_stamp_path_s_array_index );
    x_stamp_path_s_push(o,index );
}

const x_stamp_path_s* x_stamp_path_s_to_sink( const x_stamp_path_s* o, bcore_sink* sink )
{
    // bcore_x_stamp.h:212:1
    
    for(sz_t i = 0; i < o->size; i++ )
    {
        if( i > 0 ) bcore_sink_a_push_fa(sink,".", o->data[ i ] );
        if( o->data[ i ] == TYPEOF_x_stamp_path_s_array_index )
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

x_stamp_path_s* x_stamp_path_s_parse( x_stamp_path_s* o, x_source* source )
{
    // bcore_x_stamp.h:232:1
    BLM_INIT_LEVEL(0);
    x_stamp_path_s_clear(o);
    st_s name;BLM_T_INIT_SPUSH(st_s, &name);;
    
    while( !x_source_eos(source) )
    {
        st_s_clear(&(name));
        if( x_source_parse_bl(source," #?'['" ) )
        {
            uz_t index = 0;
            BLM_TRY_EXIT(x_source_parse_fa(source," #<uz_t*>", &index ))
            BLM_TRY_EXIT(x_source_parse_fa(source," ]" ))
            x_stamp_path_s_push_index(o,index );
        }
        else
        {
            BLM_TRY_EXIT(x_source_parse_fa(source," #name", (&(name)) ))
            if( name.size == 0 ) break;
            x_stamp_path_s_push(o,btypeof( name.sc ) );
        }
        if( !x_source_parse_bl(source," #?'.'" ) ) break;
    }
    BLM_RETURNV(x_stamp_path_s*, o)
}

sr_s x_stamp_path_s_get_sr_in_t( const x_stamp_path_s* o, tp_t t, const x_inst* inst )
{
    // bcore_x_stamp.h:261:1
    
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
    
        if( o->data[ i ] == TYPEOF_x_stamp_path_s_array_index )
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

sr_s x_stamp_path_s_set_sr_ret_in_t( const x_stamp_path_s* o, tp_t t, x_inst* inst, sr_s sr_src )
{
    // bcore_x_stamp.h:310:1
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
    
        if( o->data[ i ] == TYPEOF_x_stamp_path_s_array_index )
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

BCORE_DEFINE_OBJECT_INST_P( x_stamp_path_adl_s )
"aware x_array"
"{"
    "x_stamp_path_s => [];"
"}";

XOILA_DEFINE_SPECT( x_inst, x_stamp )
"{"
    "bcore_spect_header_s header;"
"}";

sr_s x_stamp_t_m_get_sr( x_stamp* o, tp_t t, tp_t name )
{
    // bcore_x_stamp.h:376:1
    
    const bcore_via_s* p = bcore_via_s_get_typed( t );
    sz_t index = bcore_via_p_nget_index( p, ((const bcore_via*)(o)), name );
    if( index < 0 ) return  sr_null();
    return  bcore_via_p_iget( p, ((const bcore_via*)(o)), index );
}

sr_s x_stamp_t_m_get_sr_i( x_stamp* o, tp_t t, sz_t index )
{
    // bcore_x_stamp.h:386:1
    
    const bcore_via_s* p = bcore_via_s_get_typed( t );
    if( index < 0 || index >= bcore_via_p_get_size( p, ((const bcore_via*)(o)) ) ) return  sr_null();
    return  bcore_via_p_iget( p, ((const bcore_via*)(o)), index );
}

void x_stamp_t_set_sr( x_stamp* o, tp_t t, tp_t name, sr_s sr_src )
{
    // bcore_x_stamp.h:398:1
    
    const bcore_via_s* p = bcore_via_s_get_typed( t );
    sz_t index = bcore_via_p_nget_index( p, ((const bcore_via*)(o)), name );
    if( index < 0 )
    {
        sr_s_down(&(sr_src));
        return ;
    }
    bcore_via_p_iset( p, ((bcore_via*)(o)), index, sr_src );
}

sr_s x_stamp_t_set_sr_ret( x_stamp* o, tp_t t, tp_t name, sr_s sr_src )
{
    // bcore_x_stamp.h:416:1
    
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

void x_stamp_t_set_sr_i( x_stamp* o, tp_t t, sz_t index, sr_s sr_src )
{
    // bcore_x_stamp.h:436:1
    
    const bcore_via_s* p = bcore_via_s_get_typed( t );
    if( index < 0 || index >= bcore_via_p_get_size( p, ((const bcore_via*)(o)) ) )
    {
        sr_s_down(&(sr_src));
        return ;
    }
    bcore_via_p_iset( p, ((bcore_via*)(o)), index, sr_src );
}

sr_s x_stamp_t_set_sr_ret_i( x_stamp* o, tp_t t, sz_t index, sr_s sr_src )
{
    // bcore_x_stamp.h:453:1
    
    const bcore_via_s* p = bcore_via_s_get_typed( t );
    if( index < 0 || index >= bcore_via_p_get_size( p, ((const bcore_via*)(o)) ) )
    {
        sr_s_down(&(sr_src));
        return  sr_null();
    }
    bcore_via_p_iset( p, ((bcore_via*)(o)), index, sr_src );
    sr_s sr = bcore_via_p_iget( p, ((const bcore_via*)(o)), index );
    if( sr_s_is_strong(&(sr)) ) sr_s_clear(&(sr));
    return  sr;
}

sr_s x_stamp_t_c_get_sr( const x_stamp* o, tp_t t, tp_t name )
{
    // bcore_x_stamp.h:470:1
    
    sr_s sr = x_stamp_t_m_get_sr(((x_stamp*)(o)),t, name );
    if( sr_s_is_weak(&(sr)) ) sr_s_set_const(&(sr),true );
    return  sr;
}

sr_s x_stamp_t_c_get_sr_i( const x_stamp* o, tp_t t, sz_t index )
{
    // bcore_x_stamp.h:480:1
    
    sr_s sr = x_stamp_t_m_get_sr_i(((x_stamp*)(o)),t, index );
    if( sr_s_is_weak(&(sr)) ) sr_s_set_const(&(sr),true );
    return  sr;
}

void x_stamp_selftest( void )
{
    // bcore_x_stamp.h:493:1
    BLM_INIT_LEVEL(0);
    bcore_prsg_lcg_u3_00_s lcg;BLM_T_INIT_SPUSH(bcore_prsg_lcg_u3_00_s, &lcg);;
    
    ASSERT( x_stamp_exists(((const x_stamp*)(&(lcg))),btypeof( "state" ) ) );
    
    u3_t state =*( ((const u3_t*)(x_stamp_c_get(((const x_stamp*)(&(lcg))),btypeof( "state" ) ))));
    ASSERT( state == 16437 );
    
    x_stamp_set_sr_ret(((x_stamp*)(&( lcg))),btypeof( "state" ), sr_u3(12345 ) );
    state = (*(((const u3_t*)(x_stamp_c_get(((const x_stamp*)(&( lcg))),btypeof( "state" ) )))));
    ASSERT( state == 12345 );
    
    const x_inst* inst = x_stamp_path_s_c_get_in(x_stamp_path_s_parse_sc(((x_stamp_path_s*)BLM_LEVEL_T_PUSH(0,x_stamp_path_s,x_stamp_path_s_create())),"state" ),((const x_inst*)(&(lcg ))));
    ASSERT( inst );
    ASSERT( (*(((const u3_t*)(inst)))) == 12345 );
    
    x_inst* zoo = ((x_inst*)BLM_LEVEL_A_PUSH(0,((x_inst*)(bcore_spect_via_create_zoo( 2 ).o))));
    
    sr_s sr = sr_null();
    
    sr = x_stamp_path_s_get_sr_in(x_stamp_path_s_parse_sc(((x_stamp_path_s*)BLM_LEVEL_T_PUSH(0,x_stamp_path_s,x_stamp_path_s_create())),"[0].[1].vdata.data2.[1]" ),zoo );
    ASSERT( sr_s_type(&(sr)) == TYPEOF_u3_t );
    ASSERT( sr_s_to_u3(&(sr)) == 0 );
    
    sr = x_stamp_path_s_set_sr_ret_in(x_stamp_path_s_parse_sc(((x_stamp_path_s*)BLM_LEVEL_T_PUSH(0,x_stamp_path_s,x_stamp_path_s_create())),"[0].[1].vdata.data2.[1]" ),zoo, sr_u3(20 ) );
    ASSERT( sr_s_type(&(sr)) == TYPEOF_u3_t );
    ASSERT( sr_s_to_u3(&(sr)) == 20 );
    BLM_DOWN();
}

/**********************************************************************************************************************/
// source: bcore_x_threads.h
#include "bcore_x_threads.h"

//----------------------------------------------------------------------------------------------------------------------
// group: x

BCORE_DEFINE_OBJECT_INST_P( x_lock_s )
"aware x"
"{"
    "private x_mutex_s * mutex;"
    "func bcore_inst_call:down_e;"
"}";

x_lock_s* x_lock_s_set( x_lock_s* o, x_mutex_s* mutex )
{
    // bcore_x_threads.h:47:5
    
    x_lock_s_release(o);
    o->mutex = mutex;
    x_mutex_s_lock(o->mutex);
    return o;
}

x_lock_s* x_lock_s_release( x_lock_s* o )
{
    // bcore_x_threads.h:54:5
    
    if( o->mutex ) x_mutex_s_unlock(o->mutex);
    o->mutex = NULL;
    return o;
}

BCORE_DEFINE_OBJECT_INST_P( x_unlock_s )
"aware x"
"{"
    "private x_mutex_s * mutex;"
    "func bcore_inst_call:down_e;"
"}";

x_unlock_s* x_unlock_s_set( x_unlock_s* o, x_mutex_s* mutex )
{
    // bcore_x_threads.h:72:5
    
    x_unlock_s_release(o);
    o->mutex = mutex;
    x_mutex_s_unlock(o->mutex);
    return o;
}

x_unlock_s* x_unlock_s_release( x_unlock_s* o )
{
    // bcore_x_threads.h:79:5
    
    if( o->mutex ) x_mutex_s_lock(o->mutex);
    o->mutex = NULL;
    return o;
}

BCORE_DEFINE_OBJECT_INST_P( x_mutex_s )
"aware x"
"{"
    "private pthread_mutex_t _mutex;"
    "func bcore_inst_call:init_x;"
    "func bcore_inst_call:down_e;"
"}";

BCORE_DEFINE_OBJECT_INST_P( x_condition_s )
"aware x"
"{"
    "private pthread_cond_t _cond;"
    "func bcore_inst_call:init_x;"
    "func bcore_inst_call:down_e;"
"}";

XOILA_DEFINE_SPECT( x_inst, x )
"{"
    "bcore_spect_header_s header;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: x_thread

BCORE_DEFINE_OBJECT_INST_P( x_thread_s )
"aware x_thread"
"{"
    "private pthread_t _thread;"
    "bl_t _join;"
    "func bcore_inst_call:down_e;"
"}";

XOILA_DEFINE_SPECT( x, x_thread )
"{"
    "bcore_spect_header_s header;"
    "feature aware x_thread : m_thread_func;"
    "feature aware x_thread : c_thread_func;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: x_thread_result

XOILA_DEFINE_SPECT( x_thread, x_thread_result )
"{"
    "bcore_spect_header_s header;"
"}";

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
    "func bcore_fp:copy_typed;"
"}";

bcore_file_path_s* bcore_file_path_s_set_sc( bcore_file_path_s* o, sc_t name )
{
    // bcore_file.h:55:5
    
    st_s_attach( &(o->full ),  NULL);
    st_s_copy_sc(&(o->name),name );
    if( name[ 0 ] != '/' && o->root != NULL )
    {
        o->full = st_s_create_fa("#<sc_t>/#<sc_t>", o->root->sc, o->name.sc );
    }
    return  o;
}

void bcore_file_path_s_copy_typed( bcore_file_path_s* o, tp_t type, vc_t src )
{
    // bcore_file.h:66:5
    
    if( type == TYPEOF_st_s )
    {
        st_s_copy(&(o->name),( st_s* )src );
    }
    else
    {
        ERR_fa( "Cannot copy from '#<sc_t>'\n", bnameof( type ) );
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
    "feature aware bcore_via_call : source;"
    "feature aware bcore_via_call : mutated;"
    "feature aware bcore_via_call : shelve;"
"}";

/**********************************************************************************************************************/
// source: bcore_main.h
#include "bcore_main.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_main

BCORE_DEFINE_OBJECT_INST_P( bcore_main_frame_s )
"aware bcore_inst"
"{"
    "bl_t first_argument_is_path_to_object = true;"
    "bl_t second_argument_is_path_to_script = true;"
    "st_s local_path = \"beth.config\";"
    "bl_t local_path_descend = false;"
    "st_s global_path;"
    "bcore_arr_st_s args;"
    "hidden bcore_mutex_s mutex;"
    "hidden aware bcore_source -> source;"
    "hidden aware bcore_sink -> sink;"
    "sr_s object_sr;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bcore_main_arr_s )
"aware x_array"
"{"
    "aware bcore_main => [];"
"}";

BCORE_DEFINE_OBJECT_INST_P( bcore_main_set_s )
"aware bcore_main"
"{"
    "bcore_main_arr_s arr;"
    "private obliv bcore_main* current_object;"
    "bcore_mutex_s mutex_current_object;"
    "func ^:main;"
    "func ^:on_termination;"
    "func ^:on_interrupt;"
    "func ^:on_suspend;"
"}";

er_t bcore_main_set_s_main( bcore_main_set_s* o, bcore_main_frame_s* frame )
{
    // bcore_main.h:121:5
    
    {const bcore_main_arr_s* __a=&(o->arr );if(__a)for(sz_t __i=0;__i<__a->size;__i++){bcore_main* e=__a->data[__i];
    {
        {BLM_INIT_LEVEL(5);
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
// source: bcore_hmap_tp_st.h
#include "bcore_hmap_tp_st.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_hmap_tp_st

BCORE_DEFINE_OBJECT_INST_P( bcore_hmap_tp_st_s )
"aware bcore_inst"
"{"
    "bcore_hmap_tpaw_s map;"
"}";

XOILA_DEFINE_SPECT( x_inst, bcore_hmap_tp_st )
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
// group: bcore_error
#include "bcore_x_array.h"

XOILA_DEFINE_SPECT( bcore_inst, bcore_error )
"{"
    "bcore_spect_header_s header;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_error_manager

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
    // bcore_error_manager.h:47:9
    
    return  ((bcore_error_manager_error_adl_s*)(x_array_set_size(((x_array*)( o)),size )));
}

bcore_error_manager_error_adl_s* bcore_error_manager_error_adl_s_clear( bcore_error_manager_error_adl_s* o )
{
    // bcore_error_manager.h:52:9
    
    return  ((bcore_error_manager_error_adl_s*)(x_array_clear(((x_array*)( o )))));
}

bcore_error_manager_error_s* bcore_error_manager_error_adl_s_push_d( bcore_error_manager_error_adl_s* o, bcore_error_manager_error_s* v )
{
    // bcore_error_manager.h:57:9
    
    return  ((bcore_error_manager_error_s*)(x_array_push_d(((x_array*)( o)),((x_inst*)(v )))));
}

BCORE_DEFINE_OBJECT_INST_P( bcore_error_manager_context_s )
"aware bcore_error_manager"
"{"
    "bcore_error_manager_error_adl_s adl;"
    "bcore_mutex_s mutex;"
"}";

XOILA_DEFINE_SPECT( bcore_error, bcore_error_manager )
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

u3_t bcore_prsg_lcg_u2_00_s_state_bits_u3( const bcore_prsg_lcg_u2_00_s* o, sz_t bits )
{
    // bcore_prsg.h:89:5
    
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_lcg_u2_00_s_bits(o);
    if( o_bits >= bits )
    {
        return  bcore_prsg_lcg_u2_00_s_state_u3(o) >> ( o_bits - bits );
    }
    else
    {
        return  bcore_prsg_lcg_u2_00_s_state_u3(o) << ( bits - o_bits );
    }
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

u3_t bcore_prsg_lcg_u2_01_s_state_bits_u3( const bcore_prsg_lcg_u2_01_s* o, sz_t bits )
{
    // bcore_prsg.h:89:5
    
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_lcg_u2_01_s_bits(o);
    if( o_bits >= bits )
    {
        return  bcore_prsg_lcg_u2_01_s_state_u3(o) >> ( o_bits - bits );
    }
    else
    {
        return  bcore_prsg_lcg_u2_01_s_state_u3(o) << ( bits - o_bits );
    }
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

u3_t bcore_prsg_lcg_u2_02_s_state_bits_u3( const bcore_prsg_lcg_u2_02_s* o, sz_t bits )
{
    // bcore_prsg.h:89:5
    
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_lcg_u2_02_s_bits(o);
    if( o_bits >= bits )
    {
        return  bcore_prsg_lcg_u2_02_s_state_u3(o) >> ( o_bits - bits );
    }
    else
    {
        return  bcore_prsg_lcg_u2_02_s_state_u3(o) << ( bits - o_bits );
    }
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

u3_t bcore_prsg_lcg_u2_03_s_state_bits_u3( const bcore_prsg_lcg_u2_03_s* o, sz_t bits )
{
    // bcore_prsg.h:89:5
    
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_lcg_u2_03_s_bits(o);
    if( o_bits >= bits )
    {
        return  bcore_prsg_lcg_u2_03_s_state_u3(o) >> ( o_bits - bits );
    }
    else
    {
        return  bcore_prsg_lcg_u2_03_s_state_u3(o) << ( bits - o_bits );
    }
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

u3_t bcore_prsg_lcg_u2_04_s_state_bits_u3( const bcore_prsg_lcg_u2_04_s* o, sz_t bits )
{
    // bcore_prsg.h:89:5
    
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_lcg_u2_04_s_bits(o);
    if( o_bits >= bits )
    {
        return  bcore_prsg_lcg_u2_04_s_state_u3(o) >> ( o_bits - bits );
    }
    else
    {
        return  bcore_prsg_lcg_u2_04_s_state_u3(o) << ( bits - o_bits );
    }
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

u3_t bcore_prsg_lcg_u2_05_s_state_bits_u3( const bcore_prsg_lcg_u2_05_s* o, sz_t bits )
{
    // bcore_prsg.h:89:5
    
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_lcg_u2_05_s_bits(o);
    if( o_bits >= bits )
    {
        return  bcore_prsg_lcg_u2_05_s_state_u3(o) >> ( o_bits - bits );
    }
    else
    {
        return  bcore_prsg_lcg_u2_05_s_state_u3(o) << ( bits - o_bits );
    }
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

u3_t bcore_prsg_lcg_u3_00_s_state_bits_u3( const bcore_prsg_lcg_u3_00_s* o, sz_t bits )
{
    // bcore_prsg.h:89:5
    
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_lcg_u3_00_s_bits(o);
    if( o_bits >= bits )
    {
        return  bcore_prsg_lcg_u3_00_s_state_u3(o) >> ( o_bits - bits );
    }
    else
    {
        return  bcore_prsg_lcg_u3_00_s_state_u3(o) << ( bits - o_bits );
    }
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

u3_t bcore_prsg_lcg_u3_01_s_state_bits_u3( const bcore_prsg_lcg_u3_01_s* o, sz_t bits )
{
    // bcore_prsg.h:89:5
    
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_lcg_u3_01_s_bits(o);
    if( o_bits >= bits )
    {
        return  bcore_prsg_lcg_u3_01_s_state_u3(o) >> ( o_bits - bits );
    }
    else
    {
        return  bcore_prsg_lcg_u3_01_s_state_u3(o) << ( bits - o_bits );
    }
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

u3_t bcore_prsg_lcg_u3_02_s_state_bits_u3( const bcore_prsg_lcg_u3_02_s* o, sz_t bits )
{
    // bcore_prsg.h:89:5
    
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_lcg_u3_02_s_bits(o);
    if( o_bits >= bits )
    {
        return  bcore_prsg_lcg_u3_02_s_state_u3(o) >> ( o_bits - bits );
    }
    else
    {
        return  bcore_prsg_lcg_u3_02_s_state_u3(o) << ( bits - o_bits );
    }
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

u3_t bcore_prsg_lcg_u3_03_s_state_bits_u3( const bcore_prsg_lcg_u3_03_s* o, sz_t bits )
{
    // bcore_prsg.h:89:5
    
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_lcg_u3_03_s_bits(o);
    if( o_bits >= bits )
    {
        return  bcore_prsg_lcg_u3_03_s_state_u3(o) >> ( o_bits - bits );
    }
    else
    {
        return  bcore_prsg_lcg_u3_03_s_state_u3(o) << ( bits - o_bits );
    }
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

u3_t bcore_prsg_xsg_u2_00_s_state_bits_u3( const bcore_prsg_xsg_u2_00_s* o, sz_t bits )
{
    // bcore_prsg.h:89:5
    
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_xsg_u2_00_s_bits(o);
    if( o_bits >= bits )
    {
        return  bcore_prsg_xsg_u2_00_s_state_u3(o) >> ( o_bits - bits );
    }
    else
    {
        return  bcore_prsg_xsg_u2_00_s_state_u3(o) << ( bits - o_bits );
    }
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

u3_t bcore_prsg_xsg_u2_01_s_state_bits_u3( const bcore_prsg_xsg_u2_01_s* o, sz_t bits )
{
    // bcore_prsg.h:89:5
    
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_xsg_u2_01_s_bits(o);
    if( o_bits >= bits )
    {
        return  bcore_prsg_xsg_u2_01_s_state_u3(o) >> ( o_bits - bits );
    }
    else
    {
        return  bcore_prsg_xsg_u2_01_s_state_u3(o) << ( bits - o_bits );
    }
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

u3_t bcore_prsg_xsg_u2_02_s_state_bits_u3( const bcore_prsg_xsg_u2_02_s* o, sz_t bits )
{
    // bcore_prsg.h:89:5
    
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_xsg_u2_02_s_bits(o);
    if( o_bits >= bits )
    {
        return  bcore_prsg_xsg_u2_02_s_state_u3(o) >> ( o_bits - bits );
    }
    else
    {
        return  bcore_prsg_xsg_u2_02_s_state_u3(o) << ( bits - o_bits );
    }
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

u3_t bcore_prsg_xsg_u2_03_s_state_bits_u3( const bcore_prsg_xsg_u2_03_s* o, sz_t bits )
{
    // bcore_prsg.h:89:5
    
    assert( bits >= 0 && bits <= 64 );
    sz_t o_bits = bcore_prsg_xsg_u2_03_s_bits(o);
    if( o_bits >= bits )
    {
        return  bcore_prsg_xsg_u2_03_s_state_u3(o) >> ( o_bits - bits );
    }
    else
    {
        return  bcore_prsg_xsg_u2_03_s_state_u3(o) << ( bits - o_bits );
    }
}

XOILA_DEFINE_SPECT( bcore_prsg, bcore_prsg_xsg )
"{"
    "bcore_spect_header_s header;"
"}";

/**********************************************************************************************************************/
// source: bcore_shell.h
#include "bcore_shell.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_shell

XOILA_DEFINE_SPECT( x_inst, bcore_shell )
"{"
    "bcore_spect_header_s header;"
    "feature aware bcore_shell : loop_callback = bcore_shell_loop_callback_default;"
    "feature aware bcore_shell : op_group = bcore_shell_op_group_default;"
    "feature aware bcore_shell : push_op_groups = bcore_shell_push_op_groups_default;"
"}";

void bcore_shell_push_op_groups_default( const bcore_shell* o, bcore_arr_tp_s* list )
{
    // bcore_shell.h:288:1
    
    bcore_arr_tp_s_push(list,bcore_shell_a_op_group(o) );
    if( !bcore_arr_tp_s_exists(list,((tp_t)(TYPEOF_bcore_shell_op_default)) ) ) bcore_arr_tp_s_push(list,((tp_t)(TYPEOF_bcore_shell_op_default)) );
}

bcore_arr_tp_s* bcore_shell_get_op_stamps( const bcore_shell* o )
{
    // bcore_shell.h:296:1
    BLM_INIT_LEVEL(0);
    bcore_arr_tp_s op_groups;BLM_T_INIT_SPUSH(bcore_arr_tp_s, &op_groups);;
    bcore_shell_a_push_op_groups(o,&(op_groups ));
    bcore_arr_tp_s* op_stamps = bcore_arr_tp_s_create();
    {const bcore_arr_tp_s* __a=&(op_groups );if(__a)for(sz_t __i=0;__i<__a->size;__i++){tp_t t=__a->data[__i]; bcore_arr_tp_s_push_arr(op_stamps,x_group_get_arr_traitline_stamps(t ) );}}
    BLM_RETURNV(bcore_arr_tp_s*, op_stamps)
}

void bcore_shell_help_to_sink( bcore_shell* o, const bcore_shell_control_s* control, bcore_sink* sink )
{
    // bcore_shell.h:307:1
    BLM_INIT_LEVEL(0);
    bcore_arr_tp_s op_groups;BLM_T_INIT_SPUSH(bcore_arr_tp_s, &op_groups);;
    bcore_shell_a_push_op_groups(o,&(op_groups ));
    
    {const bcore_arr_tp_s* __a=&(op_groups );if(__a)for(sz_t __i=0;__i<__a->size;__i++){tp_t tp_group=__a->data[__i];
    {BLM_INIT_LEVEL(4);
        bcore_sink_a_push_fa(sink,"#p80'*'{******** #<sc_t> }\n", bnameof( tp_group ) );
        bcore_arr_st_s table;BLM_T_INIT_SPUSH(bcore_arr_st_s, &table);;
        {const bcore_arr_tp_s* __a=x_group_get_arr_traitline_stamps(tp_group ) ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){tp_t t=__a->data[__i];
        {BLM_INIT_LEVEL(8);
            bcore_shell_op* op =((bcore_shell_op*)( ((x_inst*)BLM_LEVEL_TV_PUSH(0,t ,x_inst_t_create(t )))));
            st_s key;BLM_T_INIT_SPUSH(st_s, &key);;
            bcore_shell_op_a_get_key(op,&(key ));
            st_s_replace_char_char(&(key),' ', ',' );
            st_s sign;BLM_T_INIT_SPUSH(st_s, &sign);;
            st_s_push_fa(&(sign),"#<sc_t> ", key.sc );
            bcore_shell_op_arg_signature_to_sink(op,((bcore_sink*)(&(sign ))));
            st_s info;BLM_T_INIT_SPUSH(st_s, &info);;
            bcore_shell_op_a_get_info(op,&(info ));
            bcore_arr_st_s_push_fa(&(table),"#<st_s*>", (&(sign)) );
            bcore_arr_st_s_push_fa(&(table),": #<st_s*>", (&(info)) );
        BLM_DOWN();}
        }}bcore_arr_st_s_table_to_sink(&(table),-1, 2, sink );
    BLM_DOWN();}
    
    }}if( x_hmap_tp_keys(((const x_hmap_tp*)(&(control->hmap_alias)))) > 0 )
    {BLM_INIT_LEVEL(1);
        bcore_sink_a_push_fa(sink,"#p80'*'{******** #<sc_t> }\n", "ALIAS" );
        bcore_arr_st_s table;BLM_T_INIT_SPUSH(bcore_arr_st_s, &table);;
        for(sz_t i = 0; i < x_hmap_tp_size(((const x_hmap_tp*)(&(control->hmap_alias)))); i++ )
        {
            tp_t key = x_hmap_tp_idx_key(((const x_hmap_tp*)(&(control->hmap_alias))),i );
            if( key )
            {
                const st_s* expression = ((const st_s*)(x_hmap_tp_c_idx_val(((const x_hmap_tp*)(&(control->hmap_alias))),i )));
                bcore_arr_st_s_push_fa(&(table),"#<st_s*>", bcore_hmap_name_s_get(&(control->hmap_name),key ) );
                bcore_arr_st_s_push_fa(&(table),": #<st_s*>", expression );
            }
        }
        bcore_arr_st_s_table_to_sink(&(table),-1, 2, sink );
    BLM_DOWN();}
    BLM_DOWN();
}

bl_t bcore_shell_run_expression( bcore_shell* o, const bcore_main_frame_s* frame, bcore_shell_control_s* control, x_source* expression )
{
    // bcore_shell.h:363:1
    BLM_INIT_LEVEL(0);
    bl_t found = false;
    
    BLM_TRY_EXIT(x_source_parse_fa(expression," #-?';' " ))
    
    {BLM_INIT_LEVEL(1);const bcore_arr_tp_s* __a=((bcore_arr_tp_s*)BLM_LEVEL_T_PUSH(1,bcore_arr_tp_s,bcore_shell_get_op_stamps(o))) ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){tp_t t=__a->data[__i];
    {
        bcore_shell_op* op =((bcore_shell_op*)( ((x_inst*)BLM_LEVEL_TV_PUSH(0,t ,x_inst_t_create(t )))));
        if( bcore_shell_op_parse_match(op,expression ) )
        {
            if( bcore_shell_op_parse_param(op,expression,((bcore_sink*)( control->sink ))) )
            {
                bcore_shell_op_a_run(op,o,
                    frame,
                    control ? control->source : x_source_stdin(),((bcore_sink*)(
                    control ? control->sink   : x_sink_stdout())),
                    control
                );
            }
            found = true;
            break;
        }
    }
    
    }BLM_DOWN();}BLM_TRY_EXIT(x_source_parse_fa(expression," " ))
    BLM_RETURNV(bl_t, ( found && !x_source_eos(expression) ) ? bcore_shell_run_expression(o,frame, control, expression ) || found : found)
}

er_t bcore_shell_loop( bcore_shell* o, const bcore_main_frame_s* frame, bcore_shell_control_s* control )
{
    // bcore_shell.h:403:1
    BLM_INIT_LEVEL(0);
    if( !control        ) control = ((bcore_shell_control_s*)BLM_LEVEL_T_PUSH(0,bcore_shell_control_s,bcore_shell_control_s_create()));
    if( !control->source ) x_source_a_attach( &(control->source ), (x_source*)( ((bcore_source*)bcore_fork(frame->source))));
    if( !control->sink   ) x_sink_a_attach( &(control->sink   ), (x_sink*)( ((bcore_sink*)bcore_fork(frame->sink))));
    
    while( !x_source_eos(control->source) && !bcore_shell_control_s_exit_loop(control) && !bcore_shell_a_loop_callback(o) )
    {BLM_INIT_LEVEL(1);
        if( control->prompt.size > 0 )
        {
            x_sink_flush(x_sink_push_fa(control->sink,"\n#<sc_t>> ", control->prompt.sc ));
        }
        else
        {
            x_sink_flush(x_sink_push_fa(control->sink,"\n#<sc_t>#<sc_t>(#<sc_t>)> ", control->path.sc, control->path.size ? " " : "", bnameof( o->_ ) ));
        }
        bcore_shell_control_s_reset(control);
    
        st_s expression;BLM_T_INIT_SPUSH(st_s, &expression);;
        BLM_TRY(x_source_parse_fa(control->source," #until'\n'#skip'\n'", (&(expression)) ))
    
        if( expression.size > 0 )
        {BLM_INIT_LEVEL(2);
            if( x_hmap_tp_exists(((const x_hmap_tp*)(&(control->hmap_alias))),btypeof( expression.sc ) ) )
            {
                st_s_copy(&(expression),((const st_s*)(x_hmap_tp_c_get(((const x_hmap_tp*)(&(control->hmap_alias))),btypeof( expression.sc ) ))) );
            }
    
            if( !bcore_shell_run_expression(o,frame, control, ((x_source*)BLM_LEVEL_A_PUSH(2,x_source_create_from_st(&(expression )))) ) )
            {
                x_sink_push_fa(control->sink,"Syntax error in expression '#<sc_t>'\n", expression.sc );
                bcore_shell_help_to_sink(o,control,((bcore_sink*)( control->sink )));
            }
        BLM_DOWN();}
    BLM_DOWN();}
    
    control->exit_loop = false;
    BLM_RETURNV(er_t, 0)
}

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_shell_op

XOILA_DEFINE_SPECT( bcore_shell, bcore_shell_op )
"{"
    "bcore_spect_header_s header;"
    "feature aware bcore_shell_op : key;"
    "feature aware bcore_shell_op : get_key = bcore_shell_op_get_key_default;"
    "feature aware bcore_shell_op : info;"
    "feature aware bcore_shell_op : get_info = bcore_shell_op_get_info_default;"
    "feature aware bcore_shell_op : run;"
"}";

bl_t bcore_shell_op_key_match( const bcore_shell_op* o, sc_t key )
{
    // bcore_shell.h:68:5
    BLM_INIT_LEVEL(0);
    st_s st_key;BLM_T_INIT_SPUSH(st_s, &st_key);;
    bcore_shell_op_a_get_key(o,(&(st_key)) );
    sz_t index = 0;
    while( index < st_key.size )
    {BLM_INIT_LEVEL(1);
        st_s name;BLM_T_INIT_SPUSH(st_s, &name);;
        char c = 0;
        while( index < st_key.size )
        {
            c = st_key.data[ index++ ];
            if( c == ' ' || c== ',' ) break;
            st_s_push_char(&(name),c );
        }
        if( name.size == 0 ) BLM_RETURNV(bl_t, false)
        if( st_s_equal_sc(&(name),key ) ) BLM_RETURNV(bl_t, true)
    BLM_DOWN();}
    BLM_RETURNV(bl_t, false)
}

bl_t bcore_shell_op_parse_match( bcore_shell_op* o, x_source* source )
{
    // bcore_shell.h:93:5
    BLM_INIT_LEVEL(0);
    st_s key;BLM_T_INIT_SPUSH(st_s, &key);;
    BLM_TRY_EXIT(x_source_parse_fa(source,"#=name", (&(key)) ))
    if( key.size == 0 ) BLM_RETURNV(bl_t, false)
    if( !bcore_shell_op_key_match(o,key.sc ) ) BLM_RETURNV(bl_t, false)
    BLM_TRY_EXIT(x_source_parse_fa(source,"#-name" ))
    BLM_RETURNV(bl_t, true)
}

bl_t bcore_shell_op_parse_param( bcore_shell_op* o, x_source* source, bcore_sink* sink )
{
    // bcore_shell.h:107:5
    
    sz_t direct_index = 0;
    BLM_TRY_EXIT(x_source_parse_fa(source,"#skip' \t'" ))
    while( !x_source_eos(source) && !x_source_parse_bl(source,"#?([0]==';'||[0]=='\n')" ) )
    {
        x_stamp* stamp =((x_stamp*)( o));
    
        /// addressing parameter via name
        if( x_source_parse_bl(source,"#?([0]=='-'&&(([1]>='A'&&[1]<='Z')||([1]>='a'&&[1]<='z')))" ) )
        {BLM_INIT_LEVEL(2);
            st_s name;BLM_T_INIT_SPUSH(st_s, &name);;
            BLM_TRY_EXIT(x_source_parse_fa(source,"-#name", &name ))
            tp_t tp_name = btypeof( name.sc );
            sz_t index = 0;
            for(index = 0; index < x_stamp_size(stamp); index++ ) if( x_stamp_name(stamp,index ) == tp_name ) break;
            if( index == x_stamp_size(stamp) )
            {
                bcore_sink_a_push_fa(sink,"Invalid parameter '#<sc_t>'\n", name.sc );
                BLM_RETURNV(bl_t, false)
            }
            BLM_TRY_EXIT(x_source_parse_fa(source,"#skip' '" ))
    
            tp_t type = x_stamp_type(stamp,tp_name );
    
            switch( type )
            {
                case ((tp_t)(TYPEOF_bl_t)):
                {
                    if( x_source_parse_bl(source,"#?'true'" ) )
                    {
                        x_stamp_set_sr(stamp,tp_name, sr_bl(true ) );
                    }
                    else if( x_source_parse_bl(source,"#?'false'" ) )
                    {
                        x_stamp_set_sr(stamp,tp_name, sr_bl(false ) );
                    }
                    else
                    {
                        x_stamp_set_sr(stamp,tp_name, sr_bl(true ) );
                    }
                }
                break;
    
                case ((tp_t)(TYPEOF_sz_t)):
                case ((tp_t)(TYPEOF_uz_t)):
                case ((tp_t)(TYPEOF_f3_t)):
                {
                    f3_t v_f3 = 0;
                    BLM_TRY_EXIT(x_source_parse_fa(source,"#<f3_t*>", &v_f3 ))
                    x_stamp_set_sr(stamp,tp_name, sr_f3(v_f3 ) );
                }
                break;
    
                case ((tp_t)(TYPEOF_st_s)):
                {BLM_INIT_LEVEL(5);
                    st_s st;BLM_T_INIT_SPUSH(st_s, &st);;
                    if( x_source_parse_bl(source,"#=?'\"'" ) )
                    {
                        BLM_TRY_EXIT(x_source_parse_fa(source,"#string", (&(st)) ))
                    }
                    else
                    {
                        BLM_TRY_EXIT(x_source_parse_fa(source,"#name", (&(st)) ))
                    }
                    x_stamp_set_sr(stamp,tp_name, sr_awc(((const x_inst*)(&( st )))) );
                BLM_DOWN();}
                break;
    
                default:
                {
                    bcore_sink_a_push_fa(sink,"Parameter '#<sc_t>' is of unhandled type '#<sc_t>'.\n", name.sc, bnameof( type ) );
                    BLM_RETURNV(bl_t, false)
                }
            }
        BLM_DOWN();}
        else // addressing via index counter
        {BLM_INIT_LEVEL(2);
            if( direct_index >= x_stamp_size(stamp) )
            {
                bcore_sink_a_push_fa(sink,"Passing excess argument. This command has only #<sz_t> arguments.\n", x_stamp_size(stamp) );
                BLM_RETURNV(bl_t, false)
            }
    
            tp_t type = x_stamp_type_i(stamp,direct_index );
            x_inst* inst = ((x_inst*)BLM_LEVEL_TV_PUSH(2,type,x_inst_t_create(type )));
            if( x_btml_t_body_from_source(((x_btml*)( inst)),type, source ) )
            {
                bcore_error_pop_all_to_sink(sink );
                BLM_RETURNV(bl_t, false)
            }
    
            x_stamp_set_sr_i(stamp,direct_index, sr_tsd(type, ((x_inst*)bcore_fork(inst)) ) );
            direct_index++;
        BLM_DOWN();}
        BLM_TRY_EXIT(x_source_parse_fa(source,"#skip' \t'" ))
    }
    return  true;
}

void bcore_shell_op_arg_signature_to_sink( bcore_shell_op* o, bcore_sink* sink )
{
    // bcore_shell.h:208:5
    
    x_stamp* v =((x_stamp*)( o));
    for(sz_t i = 0; i < x_stamp_size(v); i++ )
    {
        tp_t name = x_stamp_name(v,i );
        tp_t type = x_stamp_type(v,name );
        switch( type )
        {
            case ((tp_t)(TYPEOF_f3_t)): case ((tp_t)(TYPEOF_sz_t)): case ((tp_t)(TYPEOF_uz_t)): case ((tp_t)(TYPEOF_s3_t)): case ((tp_t)(TYPEOF_u3_t)):
            {
                bcore_sink_a_push_fa(sink,"[<#<sc_t>>(number)] ", bnameof( name ) );
            }
            break;
    
            case ((tp_t)(TYPEOF_st_s)):
            {
                bcore_sink_a_push_fa(sink,"[<#<sc_t>>(text)] ", bnameof( name ) );
            }
            break;
    
            case ((tp_t)(TYPEOF_bl_t)):
            {
                bcore_sink_a_push_fa(sink,"[-#<sc_t>] ", bnameof( name ) );
            }
            break;
    
            default:
            {
                bcore_sink_a_push_fa(sink,"[<#<sc_t>>(#<sc_t> via BTML)] ", bnameof( name ), bnameof( type ) );
            }
            break;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_shell_control

BCORE_DEFINE_OBJECT_INST_P( bcore_shell_control_s )
"aware bcore_shell_control"
"{"
    "hidden bcore_shell_control_s* parent;"
    "aware x_sink -> sink;"
    "aware x_source -> source;"
    "bcore_hmap_name_s hmap_name;"
    "x_hmap_tp_st_s hmap_alias;"
    "bl_t exit_loop;"
    "st_s path;"
    "st_s prompt;"
"}";

XOILA_DEFINE_SPECT( bcore_shell, bcore_shell_control )
"{"
    "bcore_spect_header_s header;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_shell_op_default

BCORE_DEFINE_OBJECT_INST_P( bcore_shell_op_default_help_s )
"aware bcore_shell_op_default"
"{"
    "func bcore_shell_op:key;"
    "func bcore_shell_op:info;"
    "func bcore_shell_op:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bcore_shell_op_default_ls_s )
"aware bcore_shell_op_default"
"{"
    "st_s path;"
    "func bcore_shell_op:get_key;"
    "func bcore_shell_op:get_info;"
    "func bcore_shell_op:run;"
"}";

void bcore_shell_op_default_ls_s_run( bcore_shell_op_default_ls_s* o, bcore_shell* obj, const bcore_main_frame_s* main_frame, x_source* source, bcore_sink* sink, bcore_shell_control_s* control )
{
    // bcore_shell.h:460:9
    BLM_INIT_LEVEL(0);
    x_stamp_path_s path;BLM_T_INIT_SPUSH(x_stamp_path_s, &path);x_stamp_path_s_parse(&(path),((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_sc(o->path.sc ))) );
    sr_s sr = x_stamp_path_s_get_sr_in(&(path),((const x_inst*)(obj )));
    
    if( sr.o )
    {BLM_INIT_LEVEL(1);
        tp_t t = sr_s_type(&( sr ));
        const x_stamp* v =((const x_stamp*)( sr.o));
        sz_t size = x_stamp_t_size(t );
        bcore_arr_st_s table;BLM_T_INIT_SPUSH(bcore_arr_st_s, &table);;
        for(sz_t i = 0; i < size; i++ )
        {
            tp_t name = x_stamp_t_name(t, i );
            tp_t type = x_stamp_t_type( v,t, name );
            bcore_arr_st_s_push_fa(&(table),"#<sc_t>", bnameof( name ) );
            bcore_arr_st_s_push_fa(&(table),": #<sc_t>", bnameof( type ) );
            st_s* st = bcore_arr_st_s_push(&(table));
    
            switch( type )
            {
                case ((tp_t)(TYPEOF_u0_t)): case ((tp_t)(TYPEOF_u1_t)): case ((tp_t)(TYPEOF_u2_t)): case ((tp_t)(TYPEOF_u3_t)):
                case ((tp_t)(TYPEOF_s0_t)): case ((tp_t)(TYPEOF_s1_t)): case ((tp_t)(TYPEOF_s2_t)): case ((tp_t)(TYPEOF_s3_t)):
                case ((tp_t)(TYPEOF_bl_t)): case ((tp_t)(TYPEOF_uz_t)): case ((tp_t)(TYPEOF_sz_t)): case ((tp_t)(TYPEOF_tp_t)):
                case ((tp_t)(TYPEOF_f2_t)): case ((tp_t)(TYPEOF_f3_t)):
                {BLM_INIT_LEVEL(19);
                    sr_s sr;BLM_T_INIT_SPUSH(sr_s, &sr);;
                    sr = x_stamp_t_c_get_sr( v,t, name );
                    if( sr.o )
                    {
                        st_s_push_sc(st,": " );
                        st_s_push_st_d(st,st_s_create_typed(type, sr.o ) );
                    }
                BLM_DOWN();}
                break;
    
                default: break;
            }
        }
        bcore_arr_st_s_table_to_sink(&(table),size, -1, sink );
    BLM_DOWN();}
    else
    {
        bcore_sink_a_push_fa(sink,"Path not found.\n" );
    }
    BLM_DOWN();
}

BCORE_DEFINE_OBJECT_INST_P( bcore_shell_op_default_enter_s )
"aware bcore_shell_op_default"
"{"
    "st_s path;"
    "func bcore_shell_op:key;"
    "func bcore_shell_op:info;"
    "func bcore_shell_op:run;"
"}";

void bcore_shell_op_default_enter_s_run( bcore_shell_op_default_enter_s* o, bcore_shell* obj, const bcore_main_frame_s* main_frame, x_source* source, bcore_sink* sink, bcore_shell_control_s* control )
{
    // bcore_shell.h:513:9
    BLM_INIT_LEVEL(0);
    const x_stamp_path_s* path = x_stamp_path_s_parse(((x_stamp_path_s*)BLM_LEVEL_T_PUSH(0,x_stamp_path_s,x_stamp_path_s_create())),((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_st(&(o->path )))) );
    if( path->size == 0 )
    {
        bcore_sink_a_push_fa(sink,"Path '#<sc_t>' not found.\n", o->path.sc );
        BLM_RETURN();
    }
    sr_s sr;BLM_T_INIT_SPUSH(sr_s, &sr);;
    sr = x_stamp_path_s_get_sr_in(path,((const x_inst*)(obj )));
    bcore_shell* shell_o =((bcore_shell*)( sr.o));
    tp_t shell_t = sr_s_type(&(sr));
    
    if( shell_o )
    {BLM_INIT_LEVEL(1);
        if( !x_stamp_t_is_aware(shell_t ) )
        {
            bcore_sink_a_push_fa(sink,"Selected object '#<sc_t>' is oblivious. Cannot enter.\n", bnameof( shell_t ) );
            BLM_RETURN();
        }
    
        bcore_shell_control_s* control_child = ((bcore_shell_control_s*)BLM_LEVEL_T_PUSH(1,bcore_shell_control_s,bcore_shell_control_s_spawn(control)));
        if( control_child->path.size ) st_s_push_sc(&(control_child->path),"|" );
        st_s_push_st(&(control_child->path),&(o->path ));
        x_source_a_attach( &(control_child->source ), (x_source*)( ((x_source*)bcore_fork(source))));
        x_sink_a_attach( &(control_child->sink ), (x_sink*)( ((bcore_sink*)bcore_fork(sink))));
        BLM_TRY_EXIT(bcore_shell_loop(shell_o,main_frame, control_child ))
    BLM_DOWN();}
    else
    {
        bcore_sink_a_push_fa(sink,"Path '#<sc_t>' not found.\n", o->path.sc );
    }
    BLM_DOWN();
}

BCORE_DEFINE_OBJECT_INST_P( bcore_shell_op_default_exit_s )
"aware bcore_shell_op_default"
"{"
    "bl_t a = false;"
    "func bcore_shell_op:key;"
    "func bcore_shell_op:info;"
    "func bcore_shell_op:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bcore_shell_op_default_get_s )
"aware bcore_shell_op_default"
"{"
    "st_s path;"
    "func bcore_shell_op:key;"
    "func bcore_shell_op:info;"
    "func bcore_shell_op:run;"
"}";

void bcore_shell_op_default_get_s_run( bcore_shell_op_default_get_s* o, bcore_shell* obj, const bcore_main_frame_s* main_frame, x_source* source, bcore_sink* sink, bcore_shell_control_s* control )
{
    // bcore_shell.h:561:9
    BLM_INIT_LEVEL(0);
    sr_s sr = x_stamp_path_s_get_sr_in(x_stamp_path_s_parse(((x_stamp_path_s*)BLM_LEVEL_T_PUSH(0,x_stamp_path_s,x_stamp_path_s_create())),((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_st(&(o->path )))) ),((const x_inst*)(obj )));
    if( sr.o )
    {
        x_btml_t_to_sink(((const x_btml*)( sr.o)),sr_s_type(&( sr )),((x_sink*)( sink )));
    }
    else
    {
        bcore_sink_a_push_fa(sink,"Path '#<sc_t>' not found.\n", o->path.sc );
    }
    BLM_DOWN();
}

BCORE_DEFINE_OBJECT_INST_P( bcore_shell_op_default_set_s )
"aware bcore_shell_op_default"
"{"
    "st_s path;"
    "st_s source;"
    "func bcore_shell_op:key;"
    "func bcore_shell_op:info;"
    "func bcore_shell_op:run;"
"}";

void bcore_shell_op_default_set_s_run( bcore_shell_op_default_set_s* o, bcore_shell* obj, const bcore_main_frame_s* main_frame, x_source* source, bcore_sink* sink, bcore_shell_control_s* control )
{
    // bcore_shell.h:581:9
    BLM_INIT_LEVEL(0);
    sr_s sr = x_stamp_path_s_get_sr_in(x_stamp_path_s_parse(((x_stamp_path_s*)BLM_LEVEL_T_PUSH(0,x_stamp_path_s,x_stamp_path_s_create())),((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_st(&(o->path )))) ),((const x_inst*)(obj )));
    if( sr.o )
    {
        if( o->source.size == 0 )
        {
            bcore_sink_a_push_fa(sink,"No value specified.\n" );
            BLM_RETURN();
        }
        tp_t type = 0;
        x_btml* val = ((x_btml*)BLM_LEVEL_TV_PUSH(0,type ,x_btml_create_from_st_t(&(o->source), (&(type)) )));
        if( type )
        {
            x_inst_t_copy_typed(sr.o,sr_s_type(&( sr )), type,((const x_inst*)( val )));
        }
        else
        {
            bcore_error_pop_all_to_sink(sink );
            bcore_sink_a_push_fa(sink,"Could not assign value.\n" );
        }
    }
    else
    {
        bcore_sink_a_push_fa(sink,"Path '#<sc_t>' not found.\n", o->path.sc );
    }
    BLM_DOWN();
}

BCORE_DEFINE_OBJECT_INST_P( bcore_shell_op_default_alias_s )
"aware bcore_shell_op_default"
"{"
    "st_s key;"
    "st_s expression;"
    "func bcore_shell_op:key;"
    "func bcore_shell_op:info;"
    "func bcore_shell_op:run;"
"}";

void bcore_shell_op_default_alias_s_run( bcore_shell_op_default_alias_s* o, bcore_shell* obj, const bcore_main_frame_s* main_frame, x_source* source, bcore_sink* sink, bcore_shell_control_s* control )
{
    // bcore_shell.h:616:9
    
    if( o->key.size == 0 )
    {
        bcore_sink_a_push_fa(sink,"No key defined.\n" );
        return ;
    }
    tp_t tp_key = bcore_hmap_name_s_set_st_c(&(control->hmap_name),&(o->key ));
    if( o->expression.size > 0 )
    {
        x_hmap_tp_set_c(((x_hmap_tp*)(&(control->hmap_alias))),tp_key,((const x_inst*)(&( o->expression ))));
    }
    else
    {
        x_hmap_tp_remove(((x_hmap_tp*)(&(control->hmap_alias))),tp_key );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bcore_shell_op_default_stdin_s )
"aware bcore_shell_op_default"
"{"
    "func bcore_shell_op:key;"
    "func bcore_shell_op:info;"
    "func bcore_shell_op:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bcore_shell_op_default_stdout_s )
"aware bcore_shell_op_default"
"{"
    "func bcore_shell_op:key;"
    "func bcore_shell_op:info;"
    "func bcore_shell_op:run;"
"}";

XOILA_DEFINE_SPECT( bcore_shell, bcore_shell_op_default )
"{"
    "bcore_spect_header_s header;"
"}";

/**********************************************************************************************************************/
// source: bcore_huffman.h
#include "bcore_huffman.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_huffman; embeds: bcore_huffman.x

BCORE_DEFINE_OBJECT_INST_P( bcore_huffman_codec_s )
"aware bcore_huffman"
"{"
    "bcore_huffman_hist_s => hist;"
    "bcore_huffman_count_map_s => count_map;"
    "bcore_huffman_tree_s => tree;"
    "bcore_huffman_index_s => leaf_index;"
"}";

bcore_huffman_codec_s* bcore_huffman_codec_s_clear( bcore_huffman_codec_s* o )
{
    // bcore_huffman.x:47:1
    
    bcore_huffman_hist_s_attach( &(o->hist ),  NULL);
    bcore_huffman_tree_s_attach( &(o->tree ),  NULL);
    bcore_huffman_count_map_s_attach( &(o->count_map ),  NULL);
    bcore_huffman_index_s_attach( &(o->leaf_index ),  NULL);
    return  o;
}

bcore_huffman_codec_s* bcore_huffman_codec_s_scan_start( bcore_huffman_codec_s* o )
{
    // bcore_huffman.x:58:1
    
    bcore_huffman_codec_s_clear(o);
    bcore_huffman_hist_s_clear(BCORE_PASS_CREATE(bcore_huffman_hist_s,o->hist));
    return  o;
}

bcore_huffman_codec_s* bcore_huffman_codec_s_scan_u2( bcore_huffman_codec_s* o, u2_t val )
{
    // bcore_huffman.x:67:1
    
    if( !o->hist ) ERR_fa( "Enclose scanning with 'scan_start()' ... 'scan_end()'." );
    bcore_huffman_hist_s_count(o->hist,val );
    return  o;
}

bcore_huffman_codec_s* bcore_huffman_codec_s_scan_u3( bcore_huffman_codec_s* o, u3_t val )
{
    // bcore_huffman.x:76:1
    
    return  bcore_huffman_codec_s_scan_u2(bcore_huffman_codec_s_scan_u2(o,val ),val >> 32 );
}

bcore_huffman_codec_s* bcore_huffman_codec_s_scan_end( bcore_huffman_codec_s* o )
{
    // bcore_huffman.x:88:1
    
    if( !o->hist ) ERR_fa( "No histogram. scan_end() called before scan_start()?." );
    bcore_huffman_count_map_s_attach( &(o->count_map ),  ((bcore_huffman_count_map_s*)(x_array_sort(((x_array*)(bcore_huffman_count_map_s_from_hist(bcore_huffman_count_map_s_create(),o->hist ))),1 ))));
    bcore_huffman_hist_s_attach( &(o->hist ),  NULL);
    bcore_huffman_tree_s_build(BCORE_PASS_CREATE(bcore_huffman_tree_s,o->tree),o->count_map, BCORE_PASS_CREATE(bcore_huffman_index_s,o->leaf_index) );
    return  o;
}

const bcore_huffman_codec_s* bcore_huffman_codec_s_encode_u2( const bcore_huffman_codec_s* o, u2_t val, bcore_huffman_bit_buffer_s* bit_buffer )
{
    // bcore_huffman.x:99:1
    
    if( !o->tree ) ERR_fa( "No tree! Load a codec or run scanning first." );
    sz_t idx = bcore_huffman_index_s_get(o->leaf_index,val );
    if( idx == 0 ) ERR_fa( "An encoding for value #<u2_t> does not exist.", val );
    
    const bcore_huffman_node_s* node =&( o->tree->data[ idx ]);
    
    u3_t stack = 0;
    sz_t bits = 0;
    
    while( node->p )
    {
        bl_t bit = ( o->tree->data[ node->p ].b1 == idx );
        idx = node->p;
        node = (&(o->tree->data[ idx ]));
    
        stack = ( stack << 1 ) | ( ( bit ) ? 1 : 0 );
        bits++;
    }
    
    if( bits > 64 )  ERR_fa( "Encoding of value #<u2_t> consumes more than 64 bits.", val );
    
    for(sz_t i = 0; i < bits; i++ )
    {
        bcore_huffman_bit_buffer_s_push_bl(bit_buffer,( stack & 1 ) );
        stack = stack >> 1;
    }
    
    return  o;
}

u2_t bcore_huffman_codec_s_decode_u2( const bcore_huffman_codec_s* o, bcore_huffman_bit_buffer_iterator_s* iterator )
{
    // bcore_huffman.x:133:1
    
    if( !o->tree ) ERR_fa( "No tree! Load a codec or run scanning first." );
    const bcore_huffman_node_s* node =&( o->tree->data[ bcore_huffman_tree_s_root_index(o->tree) ]);
    
    while( !bcore_huffman_bit_buffer_iterator_s_eos(iterator) && !bcore_huffman_node_s_is_leaf(node) )
    {
        bl_t bit = bcore_huffman_bit_buffer_iterator_s_read_bl(iterator);
        uz_t idx = ( bit ) ? node->b1 : node->b0;
        node = (&(o->tree->data[ idx ]));
    }
    
    if( !bcore_huffman_node_s_is_leaf(node) )
    {
        ERR_fa( "Decoding error. Bit buffer appears corrupted." );
    }
    
    return  node->v;
}

const bcore_huffman_codec_s* bcore_huffman_codec_s_encode_u3( const bcore_huffman_codec_s* o, u3_t val, bcore_huffman_bit_buffer_s* bit_buffer )
{
    // bcore_huffman.x:155:1
    
    return  bcore_huffman_codec_s_encode_u2(bcore_huffman_codec_s_encode_u2(o,val, bit_buffer ),val >> 32, bit_buffer );
}

u3_t bcore_huffman_codec_s_decode_u3( const bcore_huffman_codec_s* o, bcore_huffman_bit_buffer_iterator_s* iterator )
{
    // bcore_huffman.x:162:1
    
    return  ((u3_t)(bcore_huffman_codec_s_decode_u2(o,iterator ))) | ( ((u3_t)(bcore_huffman_codec_s_decode_u2(o,iterator ))) << 32 );
}

const bcore_huffman_codec_s* bcore_huffman_codec_s_encode( const bcore_huffman_codec_s* o, bcore_huffman_bit_buffer_s* bit_buffer )
{
    // bcore_huffman.x:176:1
    
    if( !o->count_map ) return  o;
    bcore_huffman_count_map_s_encode(o->count_map,bit_buffer );
    return  o;
}

bcore_huffman_codec_s* bcore_huffman_codec_s_decode( bcore_huffman_codec_s* o, bcore_huffman_bit_buffer_iterator_s* iterator )
{
    // bcore_huffman.x:185:1
    
    bcore_huffman_codec_s_clear(o);
    bcore_huffman_count_map_s_decode(BCORE_PASS_CREATE(bcore_huffman_count_map_s,o->count_map),iterator );
    bcore_huffman_tree_s_build(BCORE_PASS_CREATE(bcore_huffman_tree_s,o->tree),o->count_map, BCORE_PASS_CREATE(bcore_huffman_index_s,o->leaf_index) );
    return  o;
}

BCORE_DEFINE_OBJECT_INST_P( bcore_huffman_bit_buffer_s )
"aware x_array"
"{"
    "u3_t bits;"
    "u0_t [];"
    "func x_bbml:bbml_body_to_sink;"
    "func x_bbml:bbml_body_from_source;"
    "func x_bcml:bcml_body_to_sink;"
    "func x_bcml:bcml_body_from_source;"
"}";

bcore_huffman_bit_buffer_s* bcore_huffman_bit_buffer_s_clear( bcore_huffman_bit_buffer_s* o )
{
    // bcore_huffman.x:200:1
    
    x_array_clear(((x_array*)(o)));
    o->bits = 0;
    return  o;
}

bcore_huffman_bit_buffer_s* bcore_huffman_bit_buffer_s_push_bl( bcore_huffman_bit_buffer_s* o, bl_t bit )
{
    // bcore_huffman.x:209:1
    
    if( o->bits == o->size * 8 ) (*(((u0_t*)(x_array_push(((x_array*)(o))))))) = 0;
    sz_t idx = o->bits / 8;
    if( bit )
    {
        u0_t m = 1 << ( o->bits - ( idx * 8 ) );
        o->data[ idx ] |= m;
    }
    o->bits++;
    return  o;
}

bcore_huffman_bit_buffer_s* bcore_huffman_bit_buffer_s_push_u3( bcore_huffman_bit_buffer_s* o, u3_t val, sz_t bits )
{
    // bcore_huffman.x:224:1
    
    for(sz_t i = 0; i < bits; i++ ) bcore_huffman_bit_buffer_s_push_bl(o,( val >> i ) & 1 );
    return  o;
}

bcore_huffman_bit_buffer_s* bcore_huffman_bit_buffer_s_push_packed_u3( bcore_huffman_bit_buffer_s* o, u3_t val )
{
    // bcore_huffman.x:232:1
    
    sz_t bits = sz_max( 1, bcore_huffman_min_bits(val, 64 ) );
    return  bcore_huffman_bit_buffer_s_push_u3(bcore_huffman_bit_buffer_s_push_u3(o,bits - 1, 6 ),val, bits );
}

bcore_huffman_bit_buffer_s* bcore_huffman_bit_buffer_s_push_s3( bcore_huffman_bit_buffer_s* o, s3_t val, sz_t bits )
{
    // bcore_huffman.x:240:1
    
    bcore_huffman_bit_buffer_s_push_u3(o,val, bits );
    return o;
}

bcore_huffman_bit_buffer_s* bcore_huffman_bit_buffer_s_push_packed_s3( bcore_huffman_bit_buffer_s* o, s3_t val )
{
    // bcore_huffman.x:247:1
    
    bcore_huffman_bit_buffer_s_push_bl(o,val >= 0 ? true : false );
    bcore_huffman_bit_buffer_s_push_packed_u3(o,val >= 0 ? val : -val );
    return  o;
}

bcore_huffman_bit_buffer_s* bcore_huffman_bit_buffer_s_push_f3( bcore_huffman_bit_buffer_s* o, f3_t v, sz_t bits )
{
    // bcore_huffman.x:256:1
    
    ASSERT( bits <= 31 );
    int int_exp = 0;
    s3_t man = llrint( frexp( v, (&(int_exp)) ) * ( ( 1 << bits ) - 1 ) );
    s3_t exp = ((s3_t)(int_exp));
    bcore_huffman_bit_buffer_s_push_s3(o,man, bits );
    bcore_huffman_bit_buffer_s_push_packed_s3(o,exp );
    return  o;
}

bcore_huffman_bit_buffer_s* bcore_huffman_bit_buffer_s_push_bit_buffer( bcore_huffman_bit_buffer_s* o, const bcore_huffman_bit_buffer_s* src )
{
    // bcore_huffman.x:269:1
    
    u3_t bits = src->bits;
    for(sz_t i = 0; ( i < src->size ) && ( bits > 0 ); i++ )
    {
        bcore_huffman_bit_buffer_s_push_u3(o,src->data[ i ], u3_min( 8, bits ) );
        bits -= u3_min( 8, bits );
    }
    return o;
}

tp_t bcore_huffman_bit_buffer_s_get_hash( const bcore_huffman_bit_buffer_s* o )
{
    // bcore_huffman.x:281:1
    
    tp_t hash = bcore_tp_init();
    hash = bcore_tp_fold_u3( hash, o->bits );
    {const bcore_huffman_bit_buffer_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){u0_t e=__a->data[__i]; hash = bcore_tp_fold_u0( hash, e );}}
    return  hash;
}

void bcore_huffman_bit_buffer_s_bcml_body_to_sink( const bcore_huffman_bit_buffer_s* o, x_sink* sink )
{
    // bcore_huffman.x:291:1
    
    u3_t bits = o->bits;
    x_sink_push_u3(sink,bits );
    sz_t size = ( bits / 8 ) + ( ( bits % 8 ) ? 1 : 0 );
    x_sink_push_data(sink,((const x_inst*)(o->data)), size * sizeof( u0_t ) );
}

er_t bcore_huffman_bit_buffer_s_bcml_body_from_source( bcore_huffman_bit_buffer_s* o, x_source* source )
{
    // bcore_huffman.x:301:1
    
    u3_t bits = x_source_get_u3(source);
    sz_t size = ( bits / 8 ) + ( ( bits % 8 ) ? 1 : 0 );
    x_array_set_size(((x_array*)(o)),size );
    o->bits = bits;
    x_source_get_data(source,((x_inst*)(o->data)), size * sizeof( u0_t ) );
    return  0;
}

BCORE_DEFINE_OBJECT_INST_P( bcore_huffman_bit_buffer_iterator_s )
"aware bcore_huffman"
"{"
    "hidden bcore_huffman_bit_buffer_s* bit_buffer;"
    "sz_t bit_index;"
"}";

bl_t bcore_huffman_bit_buffer_iterator_s_read_bl( bcore_huffman_bit_buffer_iterator_s* o )
{
    // bcore_huffman.x:317:1
    
    if( !o->bit_buffer ) ERR_fa( "No buffer assigned. Call setup first." );
    if( o->bit_index >= o->bit_buffer->bits ) ERR_fa( "Reading past end of buffer" );
    sz_t idx = o->bit_index / 8;
    u0_t m = 1 << ( o->bit_index - ( idx * 8 ) );
    o->bit_index++;
    return  ( o->bit_buffer->data[ idx ] & m ) ? true : false;
}

u3_t bcore_huffman_bit_buffer_iterator_s_read_u3( bcore_huffman_bit_buffer_iterator_s* o, sz_t bits )
{
    // bcore_huffman.x:329:1
    
    u3_t val = 0;
    for(sz_t i = 0; i < bits; i++ ) val = val | ( ((u3_t)(bcore_huffman_bit_buffer_iterator_s_read_bl(o))) << i );
    return  val;
}

u3_t bcore_huffman_bit_buffer_iterator_s_read_packed_u3( bcore_huffman_bit_buffer_iterator_s* o )
{
    // bcore_huffman.x:338:1
    
    return  bcore_huffman_bit_buffer_iterator_s_read_u3(o,bcore_huffman_bit_buffer_iterator_s_read_u3(o,6 ) + 1 );
}

s3_t bcore_huffman_bit_buffer_iterator_s_read_s3( bcore_huffman_bit_buffer_iterator_s* o, sz_t bits )
{
    // bcore_huffman.x:345:1
    
    u3_t u3 = bcore_huffman_bit_buffer_iterator_s_read_u3(o,bits );
    if( bits > 0 )
    {
        if( ( bits > 0 ) && ( u3 >> ( bits - 1 ) ) ) u3 = ( 0xFFFFFFFFFFFFFFFFull << bits ) | u3;
    }
    return  u3;
}

u3_t bcore_huffman_bit_buffer_iterator_s_read_packed_s3( bcore_huffman_bit_buffer_iterator_s* o )
{
    // bcore_huffman.x:357:1
    
    s3_t sign = bcore_huffman_bit_buffer_iterator_s_read_bl(o) ? 1 : -1;
    s3_t val = bcore_huffman_bit_buffer_iterator_s_read_packed_u3(o);
    return  val * sign;
}

f3_t bcore_huffman_bit_buffer_iterator_s_read_f3( bcore_huffman_bit_buffer_iterator_s* o, sz_t bits )
{
    // bcore_huffman.x:366:1
    
    ASSERT( bits <= 31 );
    s3_t man = bcore_huffman_bit_buffer_iterator_s_read_s3(o,bits );
    s3_t exp = bcore_huffman_bit_buffer_iterator_s_read_packed_s3(o);
    return  ( 1.0 / ( ( 1ull << bits ) - 1 ) ) * man * pow( 2.0, exp );
}

BCORE_DEFINE_OBJECT_INST_P( bcore_huffman_hist_s )
"aware bcore_huffman"
"{"
    "bcore_hmap_tpuz_s hmap_tpuz;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bcore_huffman_index_s )
"aware bcore_huffman"
"{"
    "bcore_hmap_tpuz_s hmap_tpuz;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bcore_huffman_node_s )
"bcore_huffman"
"{"
    "u3_t c;"
    "uz_t p;"
    "uz_t b0;"
    "uz_t b1;"
    "u2_t v;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bcore_huffman_count_node_s )
"bcore_huffman"
"{"
    "u3_t c;"
    "u2_t v;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bcore_huffman_count_map_s )
"aware x_array"
"{"
    "bcore_huffman_count_node_s [];"
"}";

bcore_huffman_count_map_s* bcore_huffman_count_map_s_from_hist( bcore_huffman_count_map_s* o, const bcore_huffman_hist_s* hist )
{
    // bcore_huffman.x:426:5
    
    x_array_clear(((x_array*)(o)));
    for(sz_t i = 0; i < hist->hmap_tpuz.size; i++ )
    {
        tp_t key = bcore_hmap_tpuz_s_idx_key(&(hist->hmap_tpuz),i );
        if( key > 0 )
        {
            bcore_huffman_count_node_s* node = ((bcore_huffman_count_node_s*)(x_array_push(((x_array*)(o)))));
            node->c = bcore_hmap_tpuz_s_idx_val(&(hist->hmap_tpuz),i );
            node->v = bcore_huffman_hist_s_val(hist,key );
        }
    }
    return  o;
}

bl_t bcore_huffman_count_map_s_is_sorted( const bcore_huffman_count_map_s* o )
{
    // bcore_huffman.x:442:5
    
    for(sz_t i = 1; i < o->size; i++ ) if( o->data[ i - 1 ].c > o->data[ i ].c ) return  false;
    return  true;
}

bl_t bcore_huffman_count_map_s_is_equal( const bcore_huffman_count_map_s* o, const bcore_huffman_count_map_s* b )
{
    // bcore_huffman.x:448:5
    
    if( o->size != b->size ) return  false;
    for(sz_t i = 1; i < o->size; i++ ) if( !bcore_huffman_count_node_s_is_equal(&(o->data[ i ]),&(b->data[ i ] )) ) return  false;
    return  true;
}

bcore_huffman_count_map_s* bcore_huffman_count_map_s_encode( bcore_huffman_count_map_s* o, bcore_huffman_bit_buffer_s* out )
{
    // bcore_huffman.x:458:1
    
    if( !bcore_huffman_count_map_s_is_sorted(o) ) ERR_fa( "Map is not sorted." );
    
    bcore_huffman_bit_buffer_s_push_packed_u3(out,o->size );
    
    sz_t idx = 0;
    u3_t count = 0;
    
    {
        for(; idx < o->size && o->data[ idx ].c == 1 ; idx++ ) count++;
        bcore_huffman_bit_buffer_s_push_packed_u3(out,count );
    }
    
    sz_t bits = 2;
    while( idx < o->size )
    {
        count = 0;
        for(sz_t i = idx; i < o->size && bcore_huffman_min_bits(o->data[ i ].c, 64 ) == bits ; i++ ) count++;
        if( count > 0 )
        {
            bcore_huffman_bit_buffer_s_push_u3(out,bits - 1, 6 );
            bcore_huffman_bit_buffer_s_push_packed_u3(out,count );
            for(sz_t i = 0; i < count; i++ ) bcore_huffman_bit_buffer_s_push_u3(out,o->data[ idx + i ].c, bits - 1 );
        }
        idx += count;
        bits++;
    }
    
    sz_t max_bits = 0;
    for(sz_t i = 0; i < o->size; i++ ) max_bits = sz_max( max_bits, bcore_huffman_min_bits(o->data[ i ].v, 32 ) );
    bcore_huffman_bit_buffer_s_push_u3(out,max_bits, 6 );
    for(sz_t i = 0; i < o->size; i++ ) bcore_huffman_bit_buffer_s_push_u3(out,o->data[ i ].v, max_bits );
    
    return  o;
}

bcore_huffman_count_map_s* bcore_huffman_count_map_s_decode( bcore_huffman_count_map_s* o, bcore_huffman_bit_buffer_iterator_s* in )
{
    // bcore_huffman.x:497:1
    
    x_array_clear(((x_array*)(o)));
    sz_t size = bcore_huffman_bit_buffer_iterator_s_read_packed_u3(in);
    x_array_set_size(((x_array*)(o)),size );
    
    sz_t idx = 0;
    u3_t count = bcore_huffman_bit_buffer_iterator_s_read_packed_u3(in);
    ASSERT( count <= o->size );
    for(sz_t i = 0; i < count; i++ ) o->data[ idx++ ].c = 1;
    
    while( idx < o->size )
    {
        sz_t bits_m1 = bcore_huffman_bit_buffer_iterator_s_read_u3(in,6 );
        u3_t count = bcore_huffman_bit_buffer_iterator_s_read_packed_u3(in);
        ASSERT( idx + count <= o->size );
        for(sz_t i = 0; i < count; i++ ) o->data[ idx + i ].c = bcore_huffman_bit_buffer_iterator_s_read_u3(in,bits_m1 ) + ( 1ull << bits_m1 );
        idx += count;
    }
    
    sz_t max_bits = bcore_huffman_bit_buffer_iterator_s_read_u3(in,6 );
    for(sz_t i = 0; i < o->size; i++ ) o->data[ i ].v = bcore_huffman_bit_buffer_iterator_s_read_u3(in,max_bits );
    
    return  o;
}

BCORE_DEFINE_OBJECT_INST_P( bcore_huffman_tree_s )
"aware x_array"
"{"
    "bcore_huffman_node_s [];"
"}";

bcore_huffman_tree_s* bcore_huffman_tree_s_build( bcore_huffman_tree_s* o, const bcore_huffman_count_map_s* count_map, bcore_huffman_index_s* leaf_index )
{
    // bcore_huffman.x:530:5
    
    x_array_set_size(((x_array*)(o)),1 );
    for(sz_t i = 0; i < count_map->size; i++ )
    {
        bcore_huffman_node_s* node = ((bcore_huffman_node_s*)(x_array_push(((x_array*)(o)))));
        node->c = count_map->data[ i ].c;
        node->v = count_map->data[ i ].v;
        if( leaf_index ) bcore_huffman_index_s_set(leaf_index,node->v, o->size - 1 );
    }
    
    sz_t idx_a = 1;
    sz_t end_a = o->size;
    sz_t idx_b = o->size;
    while( end_a - idx_a + o->size - idx_b >= 2 )
    {
        u3_t b1 = ( idx_a < end_a && ( ( idx_b == o->size ) || ( o->data[ idx_a ].c <= o->data[ idx_b ].c ) ) ) ? idx_a++ : idx_b++;
        u3_t b0 = ( idx_a < end_a && ( ( idx_b == o->size ) || ( o->data[ idx_a ].c <= o->data[ idx_b ].c ) ) ) ? idx_a++ : idx_b++;
        o->data[ b1 ].p = o->data[ b0 ].p = o->size;
        bcore_huffman_node_s* node = ((bcore_huffman_node_s*)(x_array_push(((x_array*)(o)))));
        node->b1 = b1;
        node->b0 = b0;
        node->c = o->data[ b1 ].c + o->data[ b0 ].c;
    }
    
    return  o;
}

XOILA_DEFINE_SPECT( x_inst, bcore_huffman )
"{"
    "bcore_spect_header_s header;"
"}";

sz_t bcore_huffman_min_bits( u3_t v, sz_t n )
{
    // bcore_huffman.x:23:1
    
    sz_t n1 = n >> 1;
    u3_t v1 = v >> n1;
    return  ( v1 ? n1 : 0 ) + ( n1 ? bcore_huffman_min_bits(v1 ? v1 : v, n1 ) : ((sz_t)(v)) );
}

void bcore_huffman_selftest( void )
{
    // bcore_huffman.x:567:1
    BLM_INIT_LEVEL(0);
    bcore_prsg_lcg_u3_00_s prsg;BLM_T_INIT_SPUSH(bcore_prsg_lcg_u3_00_s, &prsg);;
    
    sz_t n = 100000;
    
    bcore_arr_s3_s arr_s3;BLM_T_INIT_SPUSH(bcore_arr_s3_s, &arr_s3);;
    for(sz_t i = 0; i < n; i++ )
    {
        f3_t v = bcore_prsg_lcg_u3_00_s_gen_f3(&(prsg),0.0, 1.0 );
        s3_t x = pow( sin( v * 3.141 * 2.0 ), 10 + 1 ) * 1000;
        bcore_arr_s3_s_push(&(arr_s3),x );
    }
    
    
    /// huffman codec
    {BLM_INIT_LEVEL(1);
        bcore_huffman_codec_s codec;BLM_T_INIT_SPUSH(bcore_huffman_codec_s, &codec);;
        bcore_huffman_codec_s_scan_start(&(codec));
        for(sz_t i = 0; i < n; i++ ) bcore_huffman_codec_s_scan_s3(&(codec),arr_s3.data[ i ] );
        bcore_huffman_codec_s_scan_end(&(codec));
    
        bcore_huffman_bit_buffer_s bit_buffer;BLM_T_INIT_SPUSH(bcore_huffman_bit_buffer_s, &bit_buffer);;
    
        bcore_huffman_codec_s_encode(&(codec),&(bit_buffer ));
        for(sz_t i = 0; i < n; i++ ) bcore_huffman_codec_s_encode_s3(&(codec),arr_s3.data[ i ],&( bit_buffer ));
    
        bcore_huffman_bit_buffer_iterator_s iterator;BLM_T_INIT_SPUSH(bcore_huffman_bit_buffer_iterator_s, &iterator);bcore_huffman_bit_buffer_iterator_s_setup(&(iterator),&(bit_buffer ));
    
        bcore_huffman_codec_s codec2;BLM_T_INIT_SPUSH(bcore_huffman_codec_s, &codec2);bcore_huffman_codec_s_decode(&(codec2),&(iterator ));
        for(sz_t i = 0; i < n; i++ ) ASSERT( bcore_huffman_codec_s_decode_s3(&(codec2),&(iterator )) == arr_s3.data[ i ] );
    BLM_DOWN();}
    
    /// direct s3 codec
    {BLM_INIT_LEVEL(1);
        s3_t max = bcore_arr_s3_s_max(&(arr_s3));
        s3_t min = bcore_arr_s3_s_min(&(arr_s3));
        sz_t bits = 0;
        while( ( max >> bits ) > 0 || ( min >> bits ) < -1 ) bits++;
        bits++;
    
        bcore_huffman_bit_buffer_s bit_buffer;BLM_T_INIT_SPUSH(bcore_huffman_bit_buffer_s, &bit_buffer);;
        for(sz_t i = 0; i < n; i++ ) bcore_huffman_bit_buffer_s_push_s3(&(bit_buffer),arr_s3.data[ i ], bits );
    
        bcore_huffman_bit_buffer_iterator_s iterator;BLM_T_INIT_SPUSH(bcore_huffman_bit_buffer_iterator_s, &iterator);bcore_huffman_bit_buffer_iterator_s_setup(&(iterator),&(bit_buffer ));
        for(sz_t i = 0; i < n; i++ ) ASSERT( bcore_huffman_bit_buffer_iterator_s_read_s3(&(iterator),bits ) == arr_s3.data[ i ] );
    BLM_DOWN();}
    
    BLM_DOWN();
}

/**********************************************************************************************************************/
// source: bcore_indexer.h
#include "bcore_indexer.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_indexer

BCORE_DEFINE_OBJECT_INST_P( bcore_indexer_io_s )
"bcore_indexer"
"{"
    "s3_t i;"
    "s3_t o;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bcore_indexer_io_arr_s )
"aware x_array"
"{"
    "bcore_indexer_io_s [];"
"}";

BCORE_DEFINE_OBJECT_INST_P( bcore_indexer_cs_s )
"bcore_indexer"
"{"
    "s3_t c;"
    "s3_t s;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bcore_indexer_cs_arr_s )
"aware x_array"
"{"
    "bcore_indexer_cs_s [];"
"}";

BCORE_DEFINE_OBJECT_INST_P( bcore_indexer_s )
"aware bcore_indexer"
"{"
    "bcore_indexer_io_arr_s io_arr;"
    "bcore_indexer_cs_arr_s cs_arr;"
    "s0_t mask_bits;"
    "s3_t size;"
"}";

bcore_indexer_s* bcore_indexer_s_setup( bcore_indexer_s* o, const bcore_arr_s3_s* size_arr )
{
    // bcore_indexer.h:60:5
    
    o->size = 0;
    x_array_set_size(((x_array*)(&(o->cs_arr))),size_arr->size );
    {const bcore_indexer_cs_arr_s* __a=&(o->cs_arr );if(__a)for(sz_t __i=0;__i<__a->size;__i++){bcore_indexer_cs_s* e=&(__a->data[__i]);
    {
        e->c = o->size;
        e->s = size_arr->data[ __i ];
        o->size += e->s;
    };}}
    
    sz_t io_size = 1;
    while( io_size < size_arr->size ) io_size <<= 1;
    o->mask_bits = 0;
    while( ( ( o->size - 1 ) >> o->mask_bits ) >= io_size ) o->mask_bits++;
    
    bcore_indexer_io_s io = { 0 };
    x_array_set_size(((x_array*)(x_array_clear(((x_array*)(&(o->io_arr)))))),io_size );
    {const bcore_indexer_io_arr_s* __a=&(o->io_arr );if(__a)for(sz_t __i=0;__i<__a->size;__i++){bcore_indexer_io_s* e=&(__a->data[__i]);
    {
        sz_t c = ((s3_t)(__i)) << o->mask_bits;
        if( c >= o->size ) break;
        io.o = c - o->cs_arr.data[ io.i ].c;
        while( io.o >= o->cs_arr.data[ io.i ].s )
        {
            io.o -= o->cs_arr.data[ io.i ].s;
            io.i++;
        }
        (*(e)) = io;
    }
    
    }}return  o;
}

bl_t bcore_indexer_s_get_io( const bcore_indexer_s* o, s3_t index, bcore_indexer_io_s* io )
{
    // bcore_indexer.h:97:5
    
    if( index < 0 || index >= o->size ) return  false;
    (*(io)) = o->io_arr.data[ index >> o->mask_bits ];
    io->o += index & ( ( 1 << o->mask_bits ) - 1 );
    while( io->o >= o->cs_arr.data[ io->i ].s )
    {
        io->o -= o->cs_arr.data[ io->i ].s;
        io->i++;
    }
    return  true;
}

XOILA_DEFINE_SPECT( x_inst, bcore_indexer )
"{"
    "bcore_spect_header_s header;"
"}";

void bcore_indexer_selftest( void )
{
    // bcore_indexer.h:113:1
    BLM_INIT_LEVEL(0);
    bcore_prsg_lcg_u3_00_s prsg;BLM_T_INIT_SPUSH(bcore_prsg_lcg_u3_00_s, &prsg);;
    
    sz_t n = 100;   // containers
    sz_t m = 1000;  // container size-range
    sz_t t = 10000; // test samples
    
    bcore_arr_s3_s arr;BLM_T_INIT_SPUSH(bcore_arr_s3_s, &arr);bcore_arr_s3_s_set_size(&(arr),n );
    {const bcore_arr_s3_s* __a=&(arr );if(__a)for(sz_t __i=0;__i<__a->size;__i++){s3_t* e=&(__a->data[__i]); { (*(e)) = bcore_prsg_lcg_u3_00_s_gen_f3(&(prsg),0.0, m ); }
    
    }}bcore_indexer_s indexer;BLM_T_INIT_SPUSH(bcore_indexer_s, &indexer);bcore_indexer_s_setup(&(indexer),&(arr ));
    
    s3_t sum = 0;
    {const bcore_arr_s3_s* __a=&(arr );if(__a)for(sz_t __i=0;__i<__a->size;__i++){s3_t* e=&(__a->data[__i]); { sum += (*(e)); }
    }}ASSERT( sum == indexer.size );
    
    for(sz_t i = 0; i < t; i++ )
    {
        s3_t c = bcore_prsg_lcg_u3_00_s_gen_f3(&(prsg),-indexer.size, 2 * indexer.size );
    
        bcore_indexer_io_s io0 = { 0 };
        s3_t sum = 0;
        {const bcore_arr_s3_s* __a=&(arr );if(__a)for(sz_t __i=0;__i<__a->size;__i++){const s3_t* e=&(__a->data[__i]);
        {
            if( sum + (*(e)) > c )
            {
                io0.i = __i;
                io0.o = c - sum;
                break;
            }
            sum += (*(e));
        }
    
        }}bcore_indexer_io_s io1;
        if( bcore_indexer_s_get_io(&(indexer),c,&( io1 )) )
        {
            if( io0.i != io1.i )
            {
                bcore_indexer_s_get_io(&(indexer),c,&( io1 ));
            }
            ASSERT( io0.i == io1.i );
            ASSERT( io0.o == io1.o );
        }
        else
        {
            ASSERT( c < 0 || c >= indexer.size );
        }
    }
    
    BLM_DOWN();
}

/**********************************************************************************************************************/
// source: bcore_x_btml.h
#include "bcore_x_btml.h"

//----------------------------------------------------------------------------------------------------------------------
// group: x_btml

XOILA_DEFINE_SPECT( x_inst, x_btml )
"{"
    "bcore_spect_header_s header;"
    "feature aware x_btml : btml_body_from_source;"
    "feature aware x_btml : btml_body_to_sink;"
"}";

er_t x_btml_t_from_source( x_btml* o, tp_t t, x_source* source )
{
    // bcore_x_btml.h:199:1
    
    sr_s sr = sr_null();
    BLM_TRY(x_btml_parse_create_object(source, NULL, (&(sr)) ))
    x_inst_t_copy_typed(((x_inst*)( o)),t, sr_s_o_type( (&(sr)) ), sr.o );
    sr_s_down(&( sr ));
    return  bcore_error_last();
}

x_btml* x_btml_create_from_source_t( x_source* source, tp_t* type )
{
    // bcore_x_btml.h:210:1
    
    if( !source )
    {
        (*(type)) = 0;
        return  NULL;
    }
    sr_s sr = sr_null();
    BLM_TRY_EXIT(x_btml_parse_create_object(source, NULL, (&(sr)) ))
    if( sr.o && type ) (*(type)) = sr_s_o_type(&( sr ));
    return  ((x_btml*)(sr.o)); // sr.o is NULL in case of error
}

x_btml* x_btml_create_from_source( x_source* source )
{
    // bcore_x_btml.h:225:1
    
    tp_t t = 0;
    x_btml* o = x_btml_create_from_source_t(source, (&(t)) );
    if( t )
    {
        ASSERT( x_stamp_t_is_aware(t ) );
    }
    return  o;
}

x_sink* x_btml_t_to_sink( const x_btml* o, tp_t t, x_sink* sink )
{
    // bcore_x_btml.h:238:1
    
    x_btml_t_translate_recursive(o,t, 0, true, sink, 0 );
    return sink;
}

sc_t x_btml_name_of( tp_t type, st_s* buf )
{
    // bcore_x_btml.h:250:1
    
    sc_t n = bcore_name_try_name( type );
    if( n ) return  n;
    st_s_copy_typed(buf,((tp_t)(TYPEOF_tp_t)), (&(type)) );
    return  buf->sc;
}

tp_t x_btml_type_of( const st_s* name )
{
    // bcore_x_btml.h:260:1
    
    tp_t tp = 0;
    if( name->size == 0 )
    {
        return  0;
    }
    else if( name->data[ 0 ] >= '0' && name->data[ 0 ] <= '9' )
    {
        st_s_parse_fa(name,0, -1, "#tp_t", &tp );
    }
    else
    {
        tp = btypeof( name->sc );
    }
    return  tp;
}

bl_t x_btml_appears_valid( x_source* source )
{
    // bcore_x_btml.h:280:1
    BLM_INIT_LEVEL(0);
    bl_t valid = false;
    sz_t index = x_source_get_index(source);
    st_s* type_string = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
    if( x_source_parse_bl(source," #?'<'" ) ) // type specifier
    {
        if( x_source_parse_fa(source,"#until'>'>", type_string ) )
        {
            bcore_error_remove_last();
        }
        else
        {
            tp_t type = x_btml_type_of(type_string );
            if( bcore_flect_exists( type ) )      valid = true;
            else if( type == btypeof( "#file" ) ) valid = true;
            else if( type == btypeof( "#path" ) ) valid = true;
        }
    }
    
    x_source_set_index(source,index );
    BLM_RETURNV(bl_t, valid)
}

bl_t x_btml_t_appears_valid( tp_t type, x_source* source )
{
    // bcore_x_btml.h:306:1
    BLM_INIT_LEVEL(0);
    bl_t valid = false;
    sz_t index = x_source_get_index(source);
    st_s* type_string = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
    if( x_source_parse_bl(source," #?'<'" ) ) // type specifier
    {
        if( x_source_parse_fa(source,"#until'>'>", type_string ) )
        {
            bcore_error_remove_last();
        }
        else
        {
            if( x_btml_type_of(type_string ) == type ) valid = true;
        }
    }
    
    x_source_set_index(source,index );
    BLM_RETURNV(bl_t, valid)
}

er_t x_btml_parse_create_object( x_source* source, const sr_s* default_obj, sr_s* obj )
{
    // bcore_x_btml.h:333:1
    BLM_INIT_LEVEL(0);
    er_t er = 0;
    st_s* type_string = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
    BLM_TRY(x_source_parse_fa(source," " )) // take whitespaces
    
    if( x_source_parse_bl(source,"#?'<'" ) ) // type specifier
    {
        BLM_TRY(x_source_parse_fa(source,"#until'>'>", type_string ))
        tp_t type = x_btml_type_of(type_string );
        if( type )
        {
            if( bcore_flect_exists( type ) )
            {
                if( x_source_parse_bl(source," #?'NULL'" ) ) // no instance
                {
                    BLM_TRY(x_source_parse_fa(source," </>" ))
                    obj->p = bcore_inst_s_get_typed( type );
                }
                else
                {
                    x_inst* inst = ((x_inst*)BLM_LEVEL_TV_PUSH(0,type ,x_inst_t_create(type )));
                    if( sr_s_type(default_obj) == type ) x_inst_t_copy( inst,type, default_obj->o );
                    BLM_TRY(x_btml_t_parse_body(((x_btml*)( inst)),type, source ))
                    BLM_TRY(x_source_parse_fa(source," </>" ))
                    (*(obj)) = sr_tsd(type, ((x_inst*)bcore_fork(inst)) );
                }
            }
            else
            {
                tp_t typeof_file = btypeof( "#file" );
                tp_t typeof_path = btypeof( "#path" );
                if( type == typeof_file || type == typeof_path )
                {BLM_INIT_LEVEL(4);
                    st_s* path = ((st_s*)BLM_LEVEL_T_PUSH(4,st_s,st_s_create()));
                    BLM_TRY(x_source_parse_fa(source," #string #-?'</>'", path ))
    
                    if( path->data[ 0 ] != '/' ) // make path relative to current file path
                    {BLM_INIT_LEVEL(5);
                        st_s* cur_file = ((st_s*)BLM_LEVEL_T_PUSH(5,st_s,st_s_create_sc(x_source_get_file(source) )));
                        if( cur_file->size > 0 )
                        {
                            sz_t idx = st_s_find_char(cur_file,cur_file->size, 0, '/' );
                            if( idx < cur_file->size )
                            {BLM_INIT_LEVEL(7);
                                cur_file->data[ idx ] = 0;
                                st_s_copy(path,((st_s*)BLM_LEVEL_T_PUSH(7,st_s,st_s_create_fa("#<sc_t>/#<sc_t>", cur_file->sc, path->sc ))) );
                            BLM_DOWN();}
                        }
                    BLM_DOWN();}
    
                    if( type == typeof_file )
                    {
                        if( bcore_file_exists(path->sc ) )
                        {BLM_INIT_LEVEL(6);
                            BLM_TRY(x_btml_parse_create_object(((x_source*)(((bcore_source*)BLM_LEVEL_A_PUSH(6,bcore_file_open_source(path->sc ))))), NULL, obj ))
                        BLM_DOWN();}
                        else
                        {
                            BLM_RETURNV(er_t, x_source_parse_error_fa(source,"File '#<sc_t>' not found.", path->sc ))
                        }
                    }
                    else
                    {
                        (*(obj)) = sr_asd(((x_inst*)( ((st_s*)bcore_fork(path)) )));
                    }
                BLM_DOWN();}
                else
                {
                    BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Type '#<sc_t>' has no reflection.", type_string->sc ))
                }
            }
        }
        else
        {
            BLM_TRY(x_source_parse_fa(source," </>" ))
        }
    }
    
    // simplified encoding for certain leaf types ...
    
    // string
    else if( x_source_parse_bl(source,"#=?'\"'" ) )
    {BLM_INIT_LEVEL(2);
        st_s* st = ((st_s*)BLM_LEVEL_T_PUSH(2,st_s,st_s_create()));
        BLM_TRY(x_source_parse_fa(source," #string", st ))
        (*(obj)) = sr_tsd(((tp_t)(TYPEOF_st_s)),((x_inst*)( ((st_s*)bcore_fork(st)) )));
    BLM_DOWN();}
    // number
    else if( x_source_parse_bl(source,"#?(([0]>='0'&&[0]<='9')||[0]=='+'||[0]=='-')" ) )
    {
        f3_t v;
        BLM_TRY(x_source_parse_fa(source,"#<f3_t*>", &v ))
        (*(obj)) = sr_f3(v );
    }
    // boolean
    else if( x_source_parse_bl(source,"#?w'true'" ) )
    {
        (*(obj)) = sr_bl(true );
    }
    // boolean
    else if( x_source_parse_bl(source,"#?w'false'" ) )
    {
        (*(obj)) = sr_bl(false );
    }
    else
    {
        BLM_TRY(x_btml_skip_body(source ))
        BLM_TRY(x_source_parse_fa(source," </>" ))
    }
    
    BLM_RETURNV(er_t, er)
}

er_t x_btml_t_parse_body( x_btml* o, tp_t t, x_source* source )
{
    // bcore_x_btml.h:449:1
    
    sr_s default_element = sr_null();
    const sr_s* default_sr = NULL;
    
    if( x_source_parse_bl(source,"#?w'#default:'" ) )
    {
        BLM_TRY(x_btml_parse_create_object(source, NULL, (&(default_element)) ))
        default_sr = (&(default_element));
    }
    
    x_stamp* stamp =((x_stamp*)( o));
    if( x_btml_t_defines_btml_body_from_source(t ) )
    {
        BLM_TRY(x_btml_t_btml_body_from_source(o,t, source ))
    }
    
    /// supported string formats: '"..."' or any text terminated by whitespace or '</>'
    else if( t == ((tp_t)(TYPEOF_st_s)) )
    {
        st_s* st = ((st_s*)(o));
        if( x_source_parse_bl(source," #=?'\"'" ) )
        {
            BLM_TRY(x_source_parse_fa(source," #string", st ))
        }
        else
        {
            st_s_clear(st);
            while( !x_source_eos(source) )
            {
                char c = x_source_inspect_char(source);
                if( c == ' ' || c == '\t' || c == '\r' || c == '\n' ) break;
                if( c == '<' && x_source_parse_bl(source," #=?'</>'" ) ) break;
                st_s_push_char(st,x_source_get_char(source) );
            }
        }
    }
    else if( x_stamp_t_is_leaf(t ) )
    {BLM_INIT_LEVEL(2);
        BLM_TRY(x_source_parse_fa(source,((st_s*)BLM_LEVEL_T_PUSH(2,st_s,st_s_create_fa(" ##<#<sc_t>*>", x_btml_name_of(t, ((st_s*)BLM_LEVEL_T_PUSH(2,st_s,st_s_create())) ) )))->sc, o ))
    BLM_DOWN();}
    else if( x_stamp_t_is_pure_array(t ) )
    {
        x_array* arr =((x_array*)( stamp));
        if( x_array_t_is_mutable_mono_typed(t ) )
        {
            tp_t type = 0;
            if( x_source_parse_bl(source,"#?'type:'" ) )
            {
                sr_s sr = sr_null();
                BLM_TRY(x_btml_parse_create_object(source, default_sr, (&(sr)) ))
                type = sr_to_tp( sr );
            }
            x_array_t_set_gtype(arr,t, type );
        }
    
        if( x_array_t_is_fixed(t ) )
        {
            sz_t arr_size = x_array_t_size(arr,t );
            sz_t arr_count = 0;
            while( !x_source_parse_bl(source," #=?'</>'" ) )
            {
                sr_s sr = sr_null();
                BLM_TRY(x_btml_parse_create_object(source, default_sr, (&(sr)) ))
                if( arr_count < arr_size )
                {
                    x_array_t_set_sr(arr,t, arr_count, sr );
                }
                else
                {
                    sr_down( sr );
                }
                arr_count++;
            }
        }
        else
        {
            x_array_t_set_size(arr,t, 0 );
            while( !x_source_parse_bl(source," #=?'</>'" ) )
            {
                sr_s sr = sr_null();
                BLM_TRY(x_btml_parse_create_object(source, default_sr, (&(sr)) ))
                x_array_t_push_sr(arr,t, sr );
            }
        }
    }
    else
    {
        while( !x_source_parse_bl(source," #=?'</>'" ) )
        {BLM_INIT_LEVEL(2);
            st_s* name = ((st_s*)BLM_LEVEL_T_PUSH(2,st_s,st_s_create()));
            BLM_TRY(x_source_parse_fa(source," #name :", name ))
            tp_t tp_name = btypeof( name->sc );
            sr_s sr = sr_null();
            BLM_TRY(x_btml_parse_create_object(source, default_sr, (&(sr)) ))
            // non existing member variables are parsed but not assigned
            if( x_stamp_t_exists(t, tp_name ) )
            {
                x_stamp_t_set_sr(stamp,t, tp_name, sr );
            }
            else
            {
                sr_down( sr );
            }
        BLM_DOWN();}
    }
    
    x_stamp_t_source(stamp,t, source );
    x_stamp_t_mutated(stamp,t );
    
    sr_down( default_element );
    
    return  0;
}

er_t x_btml_skip_body( x_source* source )
{
    // bcore_x_btml.h:566:1
    
    while( !x_source_eos(source) )
    {
        if( x_source_parse_bl(source," #=?'</>'" ) ) break;
        if( x_source_parse_bl(source,"#?'<'" ) )
        {
            BLM_TRY(x_source_parse_fa(source,"#-until'>'>" ))
            BLM_TRY(x_btml_skip_body(source ))
            BLM_TRY(x_source_parse_fa(source,"</>" ))
        }
        else if( x_source_parse_bl(source,"#=?'\"'" ) ) // strings
        {
            BLM_TRY(x_source_parse_fa(source,"#-string" ))
        }
        else
        {
            x_source_get_char(source);
        }
    }
    return  0;
}

void x_btml_t_translate_recursive( const x_btml* o, tp_t t, tp_t name, bl_t shelve, x_sink* sink, sz_t depth )
{
    // bcore_x_btml.h:591:1
    BLM_INIT_LEVEL(0);
    sz_t indent = 4 * depth;
    st_s* buf = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
    
    // shelving obj_l
    if( o && shelve && bcore_via_call_t_defines_shelve(t ) )
    {
        x_btml* o_clone =((x_btml*)( x_inst_t_clone(((const x_inst*)(o)),t ))); // o_clone is obliv
        x_stamp_t_shelve(((x_stamp*)(o_clone)),t );
        x_btml_t_translate_recursive(o_clone,t, name, false, sink, depth );
        x_inst_t_discard(((x_inst*)(o_clone)),t );
        BLM_RETURN();
    }
    
    x_sink_push_fa(sink,"#rn{ }", indent );
    if( name ) x_sink_push_fa(sink,"#<sc_t>:", x_btml_name_of(name, buf ) );
    
    if( !o ) // o == NULL ...
    {
        if( t )
        {
            // Explicit NULL specifies that the object is not instantiated.
            // This is to differentiate from the case where the object was instantiated
            // without additional parameters.
            x_sink_push_fa(sink,"<#<sc_t>>NULL</>\n", x_btml_name_of(t, buf ) );
        }
        else
        {
            x_sink_push_fa(sink,"<></>\n" );
        }
    }
    else // o != NULL ...
    {
        x_sink_push_fa(sink,"<#<sc_t>>", x_btml_name_of(t, buf ) );
    
        if( x_btml_t_defines_btml_body_to_sink(t ) )
        {
            x_btml_t_btml_body_to_sink(o,t, sink );
        }
        else if( t == TYPEOF_st_s ) // strings
        {BLM_INIT_LEVEL(3);
            st_s* string = ((st_s*)BLM_LEVEL_T_PUSH(3,st_s,st_s_clone(((const st_s*)(o)))));
            st_s_replace_char_sc(string,'\"', "\\\"" );
            x_sink_push_fa(sink,"\"#<sc_t>\"", string->sc );
        BLM_DOWN();}
        else if( x_stamp_t_is_leaf(t ) )
        {
            x_sink_push_st_d(sink,st_s_create_typed(t, o ) );
        }
        else
        {
            x_sink_push_char(sink,'\n' );
            if( x_stamp_t_is_pure_array(t ) )
            {
                const x_array* arr =((const x_array*)( o));
                if( x_array_t_is_mutable_mono_typed(t ) )
                {
                    tp_t mono_type = x_array_t_get_mono_type(arr,t );
                    x_btml_t_translate_recursive( ((const x_btml*)(&(mono_type))),((tp_t)(TYPEOF_tp_t)), TYPEOF_type, false, sink, depth + 1 );
                }
                sz_t size = x_array_t_size(arr,t );
                for(sz_t i = 0; i < size; i++ )
                {
                    sr_s sr = x_array_t_c_get_sr(arr,t, i );
                    x_btml_t_translate_recursive(((const x_btml*)( sr.o)),sr_s_o_type(&(sr)), 0, true, sink, depth + 1 );
                    sr_down( sr );
                }
            }
            else
            {
                const x_stamp* stamp =((const x_stamp*)( o));
                sz_t size = x_stamp_t_size(t );
                for(sz_t i = 0; i < size; i++ )
                {
                    sr_s sr = x_stamp_t_c_get_sr_i(stamp,t, i );
                    tp_t name = x_stamp_t_name(t, i );
                    x_btml_t_translate_recursive(((const x_btml*)( sr.o)),sr_s_o_type(&(sr)), name, true, sink, depth + 1 );
                    sr_down( sr );
                }
            }
            x_sink_push_fa(sink,"#rn{ }", indent );
        }
        x_sink_push_sc(sink,"</>\n" );
    }
    BLM_DOWN();
}

sz_t x_btml_t_test_transfer( const x_btml* o, tp_t t )
{
    // bcore_x_btml.h:680:1
    BLM_INIT_LEVEL(0);
    st_s* string = st_s_create();
    x_btml_t_to_sink(o,t,((x_sink*)( string )));
    sz_t file_size = string->size;
    x_source* source = ((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_st_d(string )));
    
    x_btml* o2 =((x_btml*)( x_inst_t_create(t )));
    if( x_btml_t_from_source(o2,t, source ) )
    {
        bcore_error_pop_all_to_stderr();
        ERR_fa( "\n" );
    }
    
    s2_t c = bcore_compare_bityped( t, o, t, o2 );
    if( c != 0 )
    {BLM_INIT_LEVEL(1);
        const st_s* diff = ((st_s*)BLM_LEVEL_T_PUSH(1,st_s,((st_s*)(bcore_diff_bityped( t, o, t, o2 )))));
        if( diff )
        {
            ERR( "Comparison returned '%"PRIi32"':\n%s\n", c, diff->sc );
        }
        else
        {
            ERR( "Comparison returned '%"PRIi32"' but diff returned 'NULL'\n", c );
        }
    BLM_DOWN();}
    
    x_inst_t_discard(((x_inst*)(o2)),t );
    BLM_RETURNV(sz_t, file_size)
}

void x_btml_selftest( void )
{
    // bcore_x_btml.h:714:1
    BLM_INIT_LEVEL(0);
    sr_s zoo;BLM_T_INIT_SPUSH(sr_s, &zoo);; zoo = bcore_spect_via_create_zoo( 1000 );
    
    clock_t time = clock();
    sz_t size = x_btml_t_test_transfer(((const x_btml*)(zoo.o)),sr_s_o_type(&( zoo )) );
    time = clock() - time;
    
    bcore_msg_fa( "x_btml transfer #<f3_t>s; size: #<sz_t> bytes\n", ( f3_t )time/CLOCKS_PER_SEC, size );
    BLM_DOWN();
}

/**********************************************************************************************************************/
// source: bcore_x_bbml.h
#include "bcore_x_bbml.h"

//----------------------------------------------------------------------------------------------------------------------
// group: x_bbml

XOILA_DEFINE_SPECT( x_inst, x_bbml )
"{"
    "bcore_spect_header_s header;"
    "feature aware x_bbml : bbml_body_from_source;"
    "feature aware x_bbml : bbml_body_to_sink;"
"}";

er_t x_bbml_t_from_source( x_bbml* o, tp_t t, x_source* source )
{
    // bcore_x_bbml.h:115:1
    
    sr_s sr = sr_null();
    BLM_TRY(x_bbml_parse_create_object(source, (&(sr)) ))
    x_inst_t_copy_typed(((x_inst*)( o)),t, sr_s_o_type( (&(sr)) ), sr.o );
    sr_s_down(&( sr ));
    return  bcore_error_last();
}

x_bbml* x_bbml_create_from_source_t( x_source* source, tp_t* type )
{
    // bcore_x_bbml.h:126:1
    
    if( !source )
    {
        (*(type)) = 0;
        return  NULL;
    }
    sr_s sr = sr_null();
    BLM_TRY_EXIT(x_bbml_parse_create_object(source, (&(sr)) ))
    if( sr.o && type ) (*(type)) = sr_s_o_type(&( sr ));
    return  ((x_bbml*)(sr.o)); // sr.o is NULL in case of error
}

x_bbml* x_bbml_create_from_source( x_source* source )
{
    // bcore_x_bbml.h:141:1
    
    tp_t t = 0;
    x_bbml* o = x_bbml_create_from_source_t(source, (&(t)) );
    if( t )
    {
        ASSERT( x_stamp_t_is_aware(t ) );
    }
    return  o;
}

x_sink* x_bbml_t_to_sink( const x_bbml* o, tp_t t, x_sink* sink )
{
    // bcore_x_bbml.h:154:1
    
    x_bbml_t_translate_recursive(o,t, 0, true, sink );
    return sink;
}

bl_t x_bbml_appears_valid( x_source* source )
{
    // bcore_x_bbml.h:171:1
    
    sz_t index = x_source_get_index(source);
    tp_t type = x_bbml_parse_type(source );
    bl_t valid = bcore_flect_exists( type );
    x_source_set_index(source,index );
    return  valid;
}

er_t x_bbml_parse_create_object( x_source* source, sr_s* obj )
{
    // bcore_x_bbml.h:185:1
    BLM_INIT_LEVEL(0);
    er_t er = 0;
    tp_t type = x_bbml_parse_type(source );
    bl_t flag = x_bbml_parse_flag(source );
    
    if( type )
    {
        if( bcore_flect_exists( type ) )
        {
            if( flag )
            {
                x_inst* inst = ((x_inst*)BLM_LEVEL_TV_PUSH(0,type ,x_inst_t_create(type )));
                BLM_TRY(x_bbml_t_parse_body(((x_bbml*)( inst)),type, source ))
                (*(obj)) = sr_tsd(type, ((x_inst*)bcore_fork(inst)) );
            }
            else // no instance
            {
                obj->p = bcore_inst_s_get_typed( type );
            }
        }
        else
        {
            BLM_RETURNV(er_t, bcore_error_push_fa(TYPEOF_parse_error, "Type '#<sc_t>' has no reflection.", bnameof( type ) ))
        }
    }
    
    BLM_RETURNV(er_t, er)
}

er_t x_bbml_t_parse_leaf_body( x_bbml* o, tp_t t, x_source* source )
{
    // bcore_x_bbml.h:217:1
    
    switch( t )
    {
        case TYPEOF_aware_t : (*(((tp_t*)(o)))) = x_source_get_tp(source); break;
        case TYPEOF_bl_t    : (*(((bl_t*)(o)))) = x_source_get_bl(source); break;
        case TYPEOF_f2_t    : (*(((f2_t*)(o)))) = x_source_get_f2(source); break;
        case TYPEOF_f3_t    : (*(((f3_t*)(o)))) = x_source_get_f3(source); break;
        case TYPEOF_s0_t    : (*(((s0_t*)(o)))) = x_source_get_s0(source); break;
        case TYPEOF_s1_t    : (*(((s1_t*)(o)))) = x_source_get_s1(source); break;
        case TYPEOF_s2_t    : (*(((s2_t*)(o)))) = x_source_get_s2(source); break;
        case TYPEOF_s3_t    : (*(((s3_t*)(o)))) = x_source_get_s3(source); break;
        case TYPEOF_sc_t    : break; // c-style constant strings are skipped over
        case TYPEOF_sz_t    : (*(((sz_t*)(o)))) = x_source_get_sz(source); break;
        case TYPEOF_uz_t    : (*(((uz_t*)(o)))) = x_source_get_uz(source); break;
        case TYPEOF_tp_t    : (*(((tp_t*)(o)))) = x_source_get_tp(source); break;
        case TYPEOF_u0_t    : (*(((u0_t*)(o)))) = x_source_get_u0(source); break;
        case TYPEOF_u1_t    : (*(((u1_t*)(o)))) = x_source_get_u1(source); break;
        case TYPEOF_u2_t    : (*(((u2_t*)(o)))) = x_source_get_u2(source); break;
        case TYPEOF_u3_t    : (*(((u3_t*)(o)))) = x_source_get_u3(source); break;
        default: ERR_fa( "Cannot convert type '#<sc_t>' from a binary stream.", bnameof( t ) );
    }
    return  0;
}

er_t x_bbml_t_parse_body( x_bbml* o, tp_t t, x_source* source )
{
    // bcore_x_bbml.h:244:1
    
    x_stamp* stamp =((x_stamp*)( o));
    if( x_bbml_t_defines_bbml_body_from_source(t ) )
    {
        BLM_TRY(x_bbml_t_bbml_body_from_source(o,t, source ))
    }
    else if( t == ((tp_t)(TYPEOF_st_s)) )
    {
        st_s* st = ((st_s*)(o));
        st_s_clear(st);
        char c = 0;
        while( (c = x_source_get_char(source)) != 0 ) st_s_push_char(st,c );
    }
    else if( x_stamp_t_is_leaf(t ) )
    {
        BLM_TRY(x_bbml_t_parse_leaf_body( o,t, source ))
    }
    else if( x_stamp_t_is_pure_array(t ) )
    {
        x_array* arr =((x_array*)( stamp));
    
        if( x_array_t_is_mutable_mono_typed(t ) ) x_array_t_set_gtype(arr,t, x_bbml_parse_type(source ) );
    
        sz_t size = x_bbml_parse_size(source );
    
        if( x_array_t_is_fixed(t ) )
        {
            sz_t arr_size = x_array_t_size(arr,t );
    
            for(sz_t i = 0; i < size; i++ )
            {
                sr_s sr = sr_null();
                BLM_TRY(x_bbml_parse_create_object(source, (&(sr)) ))
                if( i < arr_size )
                {
                    x_array_t_set_sr(arr,t, i, sr );
                }
                else
                {
                    sr_down( sr );
                }
            }
        }
        else
        {
            x_array_t_set_size(arr,t, size );
            for(sz_t i = 0; i < size; i++ )
            {
                sr_s sr = sr_null();
                BLM_TRY(x_bbml_parse_create_object(source, (&(sr)) ))
                x_array_t_set_sr(arr,t, i, sr );
            }
        }
    }
    else
    {
        while( !x_source_eos(source) )
        {
            // non existing member variables are parsed but not assigned
            tp_t tp_name = x_bbml_parse_type(source );
            if( !tp_name ) break;
    
            sr_s sr = sr_null();
            BLM_TRY(x_bbml_parse_create_object(source, (&(sr)) ))
            if( x_stamp_t_exists(t, tp_name ) )
            {
                x_stamp_t_set_sr(stamp,t, tp_name, sr );
            }
            else
            {
                sr_down( sr );
            }
        }
    }
    
    x_stamp_t_source(stamp,t, source );
    x_stamp_t_mutated(stamp,t );
    
    return  0;
}

void x_bbml_push_leaf( x_sink* sink, tp_t t, const x_inst* leaf )
{
    // bcore_x_bbml.h:332:1
    
    switch( t )
    {
        case TYPEOF_aware_t : x_sink_push_tp(sink,*(((const tp_t*)(leaf)) )); break;
        case TYPEOF_bl_t    : x_sink_push_bl(sink,*(((const bl_t*)(leaf)) )); break;
        case TYPEOF_f2_t    : x_sink_push_f2(sink,*(((const f2_t*)(leaf)) )); break;
        case TYPEOF_f3_t    : x_sink_push_f3(sink,*(((const f3_t*)(leaf)) )); break;
        case TYPEOF_s0_t    : x_sink_push_s0(sink,*(((const s0_t*)(leaf)) )); break;
        case TYPEOF_s1_t    : x_sink_push_s1(sink,*(((const s1_t*)(leaf)) )); break;
        case TYPEOF_s2_t    : x_sink_push_s2(sink,*(((const s2_t*)(leaf)) )); break;
        case TYPEOF_s3_t    : x_sink_push_s3(sink,*(((const s3_t*)(leaf)) )); break;
        case TYPEOF_sc_t    : break; // c-style constant strings are skipped over
        case TYPEOF_sz_t    : x_sink_push_sz(sink,*(((const sz_t*)(leaf)) )); break;
        case TYPEOF_uz_t    : x_sink_push_uz(sink,*(((const uz_t*)(leaf)) )); break;
        case TYPEOF_tp_t    : x_sink_push_tp(sink,*(((const tp_t*)(leaf)) )); break;
        case TYPEOF_u0_t    : x_sink_push_u0(sink,*(((const u0_t*)(leaf)) )); break;
        case TYPEOF_u1_t    : x_sink_push_u1(sink,*(((const u1_t*)(leaf)) )); break;
        case TYPEOF_u2_t    : x_sink_push_u2(sink,*(((const u2_t*)(leaf)) )); break;
        case TYPEOF_u3_t    : x_sink_push_u3(sink,*(((const u3_t*)(leaf)) )); break;
        default: ERR_fa( "Cannot convert type '#<sc_t>' into a binary stream.", bnameof( t ) );
    }
}

void x_bbml_t_translate_recursive( const x_bbml* o, tp_t t, tp_t name, bl_t shelve, x_sink* sink )
{
    // bcore_x_bbml.h:356:1
    
    // shelving obj_l
    if( o && shelve && bcore_via_call_t_defines_shelve(t ) )
    {
        x_bbml* o_clone =((x_bbml*)( x_inst_t_clone(((const x_inst*)(o)),t ))); // no scoping (o_clone is obliv)
        x_stamp_t_shelve(((x_stamp*)(o_clone)),t );
        x_bbml_t_translate_recursive(o_clone,t, name, false, sink );
        x_inst_discard(((x_inst*)(o_clone)));
        return ;
    }
    
    if( name ) x_bbml_push_type(sink, name );
    
    x_bbml_push_type(sink, t );
    x_bbml_push_flag(sink, o != NULL );
    
    if( o )
    {
        if( x_bbml_t_defines_bbml_body_to_sink(t ) )
        {
            x_bbml_t_bbml_body_to_sink(o,t, sink );
        }
        else if( t == TYPEOF_st_s ) // strings
        {
            const st_s* string = ((const st_s*)(o));
            x_sink_push_data(sink,( vc_t )string->data, string->size + 1 ); // push string including terminating 0
        }
        else if( x_stamp_t_is_leaf(t ) )
        {
            x_bbml_push_leaf(sink, t,((const x_inst*)( o )));
        }
        else
        {
            if( x_stamp_t_is_pure_array(t ) )
            {
                const x_array* arr =((const x_array*)( o));
                if( x_array_t_is_mutable_mono_typed(t ) ) x_bbml_push_type(sink, x_array_t_get_mono_type(arr,t ) );
                sz_t size = x_array_t_size(arr,t );
                x_bbml_push_size(sink, size );
                for(sz_t i = 0; i < size; i++ )
                {
                    sr_s sr = x_array_t_c_get_sr(arr,t, i );
                    x_bbml_t_translate_recursive(((const x_bbml*)( sr.o)),sr_s_o_type(&(sr)), 0, true, sink );
                    sr_down( sr );
                }
            }
            else
            {
                const x_stamp* stamp =((const x_stamp*)( o));
                sz_t size = x_stamp_t_size(t );
                for(sz_t i = 0; i < size; i++ )
                {
                    sr_s sr = x_stamp_t_c_get_sr_i(stamp,t, i );
                    tp_t name = x_stamp_t_name(t, i );
                    x_bbml_t_translate_recursive(((const x_bbml*)( sr.o)),sr_s_o_type(&(sr)), name, true, sink );
                    sr_down( sr );
                }
    
                /// we push a terminating '0' to indicate the end of the parameter list
                x_bbml_push_type(sink, 0 );
            }
        }
    }
}

sz_t x_bbml_t_test_transfer( const x_bbml* o, tp_t t )
{
    // bcore_x_bbml.h:424:1
    BLM_INIT_LEVEL(0);
    st_s* string = st_s_create();
    x_bbml_t_to_sink(o,t,((x_sink*)( string )));
    sz_t file_size = string->size;
    
    x_source* source = ((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_st_d(string )));
    
    x_bbml* o2 =((x_bbml*)( x_inst_t_create(t )));
    if( x_bbml_t_from_source(o2,t, source ) )
    {
        bcore_error_pop_all_to_stderr();
        ERR_fa( "\n" );
    }
    
    s2_t c = bcore_compare_bityped( t, o, t, o2 );
    if( c != 0 )
    {BLM_INIT_LEVEL(1);
        const st_s* diff = ((st_s*)BLM_LEVEL_T_PUSH(1,st_s,((st_s*)(bcore_diff_bityped( t, o, t, o2 )))));
        if( diff )
        {
            ERR( "Comparison returned '%"PRIi32"':\n%s\n", c, diff->sc );
        }
        else
        {
            ERR( "Comparison returned '%"PRIi32"' but diff returned 'NULL'\n", c );
        }
    BLM_DOWN();}
    
    x_inst_t_discard(((x_inst*)(o2)),t );
    BLM_RETURNV(sz_t, file_size)
}

void x_bbml_selftest( void )
{
    // bcore_x_bbml.h:459:1
    BLM_INIT_LEVEL(0);
    sr_s zoo;BLM_T_INIT_SPUSH(sr_s, &zoo);; zoo = bcore_spect_via_create_zoo( 1000 );
    
    clock_t time = clock();
    sz_t size = x_bbml_t_test_transfer(((const x_bbml*)(zoo.o)),sr_s_o_type(&( zoo )) );
    time = clock() - time;
    
    bcore_msg_fa( "x_bbml transfer #<f3_t>s; size: #<sz_t> bytes\n", ( f3_t )time/CLOCKS_PER_SEC, size );
    BLM_DOWN();
}

/**********************************************************************************************************************/
// source: bcore_x_bcml.h
#include "bcore_x_bcml.h"

//----------------------------------------------------------------------------------------------------------------------
// group: x_bcml

XOILA_DEFINE_SPECT( x_inst, x_bcml )
"{"
    "bcore_spect_header_s header;"
    "feature aware x_bcml : bcml_body_from_source;"
    "feature aware x_bcml : bcml_body_to_sink;"
"}";

er_t x_bcml_t_from_source( x_bcml* o, tp_t t, x_source* source )
{
    // bcore_x_bcml.h:104:1
    
    sr_s sr = sr_null();
    BLM_TRY(x_bcml_parse_create_object(true, 0, source, (&(sr)) ))
    x_inst_t_copy_typed(((x_inst*)( o)),t, sr_s_o_type( (&(sr)) ), sr.o );
    sr_s_down(&( sr ));
    return  bcore_error_last();
}

x_bcml* x_bcml_create_from_source_t( x_source* source, tp_t* type )
{
    // bcore_x_bcml.h:115:1
    
    if( !source )
    {
        (*(type)) = 0;
        return  NULL;
    }
    sr_s sr = sr_null();
    BLM_TRY_EXIT(x_bcml_parse_create_object(true, 0, source, (&(sr)) ))
    if( sr.o && type ) (*(type)) = sr_s_o_type(&( sr ));
    return  ((x_bcml*)(sr.o)); // sr.o is NULL in case of error
}

x_bcml* x_bcml_create_from_source( x_source* source )
{
    // bcore_x_bcml.h:130:1
    
    tp_t t = 0;
    x_bcml* o = x_bcml_create_from_source_t(source, (&(t)) );
    if( t )
    {
        ASSERT( x_stamp_t_is_aware(t ) );
    }
    return  o;
}

x_sink* x_bcml_t_to_sink( const x_bcml* o, tp_t t, x_sink* sink )
{
    // bcore_x_bcml.h:143:1
    
    x_bcml_t_translate_recursive(o,t, true, true, true, sink );
    return sink;
}

bl_t x_bcml_appears_valid( x_source* source )
{
    // bcore_x_bcml.h:160:1
    
    sz_t index = x_source_get_index(source);
    tp_t type = x_bcml_parse_type(source );
    bl_t valid = bcore_flect_exists( type );
    x_source_set_index(source,index );
    return  valid;
}

er_t x_bcml_parse_create_object( bl_t parse_existence, tp_t given_type, x_source* source, sr_s* obj )
{
    // bcore_x_bcml.h:174:1
    BLM_INIT_LEVEL(0);
    er_t er = 0;
    tp_t type = given_type ? given_type : x_bcml_parse_type(source );
    bl_t flag = parse_existence ? x_bcml_parse_flag(source ) : true;
    
    if( type )
    {
        if( bcore_flect_exists( type ) )
        {
            if( flag )
            {
                x_inst* inst = ((x_inst*)BLM_LEVEL_TV_PUSH(0,type ,x_inst_t_create(type )));
                BLM_TRY(x_bcml_t_parse_body(((x_bcml*)( inst)),type, source ))
                (*(obj)) = sr_tsd(type, ((x_inst*)bcore_fork(inst)) );
            }
            else // no instance
            {
                obj->p = bcore_inst_s_get_typed( type );
            }
        }
        else
        {
            BLM_RETURNV(er_t, bcore_error_push_fa(TYPEOF_parse_error, "Type '#<sc_t>' has no reflection.", bnameof( type ) ))
        }
    }
    
    BLM_RETURNV(er_t, er)
}

er_t x_bcml_t_parse_leaf_body( x_bcml* o, tp_t t, x_source* source )
{
    // bcore_x_bcml.h:206:1
    
    switch( t )
    {
        case TYPEOF_aware_t : (*(((tp_t*)(o)))) = x_source_get_tp(source); break;
        case TYPEOF_bl_t    : (*(((bl_t*)(o)))) = x_source_get_bl(source); break;
        case TYPEOF_f2_t    : (*(((f2_t*)(o)))) = x_source_get_f2(source); break;
        case TYPEOF_f3_t    : (*(((f3_t*)(o)))) = x_source_get_f3(source); break;
        case TYPEOF_s0_t    : (*(((s0_t*)(o)))) = x_source_get_s0(source); break;
        case TYPEOF_s1_t    : (*(((s1_t*)(o)))) = x_source_get_s1(source); break;
        case TYPEOF_s2_t    : (*(((s2_t*)(o)))) = x_source_get_s2(source); break;
        case TYPEOF_s3_t    : (*(((s3_t*)(o)))) = x_source_get_s3(source); break;
        case TYPEOF_sc_t    : break; // c-style constant strings are skipped over
        case TYPEOF_sz_t    : (*(((sz_t*)(o)))) = x_source_get_sz(source); break;
        case TYPEOF_uz_t    : (*(((uz_t*)(o)))) = x_source_get_uz(source); break;
        case TYPEOF_tp_t    : (*(((tp_t*)(o)))) = x_source_get_tp(source); break;
        case TYPEOF_u0_t    : (*(((u0_t*)(o)))) = x_source_get_u0(source); break;
        case TYPEOF_u1_t    : (*(((u1_t*)(o)))) = x_source_get_u1(source); break;
        case TYPEOF_u2_t    : (*(((u2_t*)(o)))) = x_source_get_u2(source); break;
        case TYPEOF_u3_t    : (*(((u3_t*)(o)))) = x_source_get_u3(source); break;
        default: ERR_fa( "Cannot convert type '#<sc_t>' from a binary stream.", bnameof( t ) );
    }
    return  0;
}

er_t x_bcml_t_parse_leaf_arr_body( x_bcml* o, tp_t t, sz_t size, x_source* source )
{
    // bcore_x_bcml.h:233:1
    
    switch( t )
    {
        case TYPEOF_aware_t : x_source_get_data(source,((x_inst*)(o)), size * sizeof( aware_t ) ); break;
        case TYPEOF_bl_t    : { for(sz_t i = 0; i < size; i++ ) { ((bl_t*)(o))[ i ] = x_source_get_bl(source); } } break; // bl is converted
        case TYPEOF_f2_t    : x_source_get_data(source,((x_inst*)(o)), size * sizeof( f2_t ) ); break;
        case TYPEOF_f3_t    : x_source_get_data(source,((x_inst*)(o)), size * sizeof( f3_t ) ); break;
        case TYPEOF_s0_t    : x_source_get_data(source,((x_inst*)(o)), size * sizeof( s0_t ) ); break;
        case TYPEOF_s1_t    : x_source_get_data(source,((x_inst*)(o)), size * sizeof( s1_t ) ); break;
        case TYPEOF_s2_t    : x_source_get_data(source,((x_inst*)(o)), size * sizeof( s2_t ) ); break;
        case TYPEOF_s3_t    : x_source_get_data(source,((x_inst*)(o)), size * sizeof( s3_t ) ); break;
        case TYPEOF_sc_t    : break; // c-style constant strings are skipped over
        case TYPEOF_sz_t    : { for(sz_t i = 0; i < size; i++ ) { ((sz_t*)(o))[ i ] = x_source_get_sz(source); } } break; // sz is converted
        case TYPEOF_uz_t    : { for(sz_t i = 0; i < size; i++ ) { ((uz_t*)(o))[ i ] = x_source_get_uz(source); } } break; // uz is converted
        case TYPEOF_tp_t    : x_source_get_data(source,((x_inst*)(o)), size * sizeof( tp_t ) ); break;
        case TYPEOF_u0_t    : x_source_get_data(source,((x_inst*)(o)), size * sizeof( u0_t ) ); break;
        case TYPEOF_u1_t    : x_source_get_data(source,((x_inst*)(o)), size * sizeof( u1_t ) ); break;
        case TYPEOF_u2_t    : x_source_get_data(source,((x_inst*)(o)), size * sizeof( u2_t ) ); break;
        case TYPEOF_u3_t    : x_source_get_data(source,((x_inst*)(o)), size * sizeof( u3_t ) ); break;
        default: ERR_fa( "Cannot convert array of type '#<sc_t>' from a binary stream.", bnameof( t ) );
    }
    return  0;
}

er_t x_bcml_t_parse_body( x_bcml* o, tp_t t, x_source* source )
{
    // bcore_x_bcml.h:260:1
    
    x_stamp* stamp =((x_stamp*)( o));
    if( x_bcml_t_defines_bcml_body_from_source(t ) )
    {
        BLM_TRY(x_bcml_t_bcml_body_from_source(o,t, source ))
    }
    else if( t == ((tp_t)(TYPEOF_st_s)) )
    {
        st_s* st = ((st_s*)(o));
        st_s_clear(st);
        char c = 0;
        while( (c = x_source_get_char(source)) != 0 ) st_s_push_char(st,c );
    }
    else if( x_stamp_t_is_leaf(t ) )
    {
        BLM_TRY(x_bcml_t_parse_leaf_body( o,t, source ))
    }
    else if( x_stamp_t_is_pure_array(t ) )
    {
        x_array* arr =((x_array*)( stamp));
    
        if( x_array_t_is_mutable_mono_typed(t ) ) x_array_t_set_gtype(arr,t, x_bcml_parse_type(source ) );
        bl_t is_of_links = x_array_t_is_of_links(t );
        tp_t static_type = x_array_t_get_static_type(t );
    
        sz_t size = x_bcml_parse_size(source );
    
        if( x_array_t_is_fixed(t ) )
        {
            sz_t arr_size = x_array_t_size(arr,t );
            if( arr_size != size ) ERR_fa( "Fixed array size (#<sz_t>) differs from stored size (#<sz_t>).", arr_size, size );
        }
        else
        {
            x_array_t_set_size(arr,t, size );
        }
    
        if( ( !is_of_links ) && ( static_type != 0 ) && x_stamp_t_is_leaf(static_type ) )
        {
            BLM_TRY(x_bcml_t_parse_leaf_arr_body(((x_bcml*)( x_array_t_get_data_m(arr,t ))),static_type, size, source ))
        }
        else
        {
            for(sz_t i = 0; i < size; i++ )
            {
                sr_s sr = sr_null();
                BLM_TRY(x_bcml_parse_create_object(is_of_links, static_type, source, (&(sr)) ))
                x_array_t_set_sr(arr,t, i, sr );
            }
        }
    }
    else
    {
        sz_t size = x_stamp_t_size(t );
        for(sz_t i = 0; i < size; i++ )
        {
            sr_s sr = sr_null();
            tp_t type = x_stamp_t_is_static_i(t, i ) ? x_stamp_t_type_i(stamp,t, i ) : 0;
            BLM_TRY(x_bcml_parse_create_object(x_stamp_t_is_link_i(t, i ), type, source, (&(sr)) ))
            x_stamp_t_set_sr_i(stamp,t, i, sr );
        }
    }
    
    x_stamp_t_source(stamp,t, source );
    x_stamp_t_mutated(stamp,t );
    
    return  0;
}

void x_bcml_push_leaf( x_sink* sink, tp_t t, const x_inst* leaf )
{
    // bcore_x_bcml.h:336:1
    
    switch( t )
    {
        case TYPEOF_aware_t : x_sink_push_tp(sink,*(((const tp_t*)(leaf)) )); break;
        case TYPEOF_bl_t    : x_sink_push_bl(sink,*(((const bl_t*)(leaf)) )); break;
        case TYPEOF_f2_t    : x_sink_push_f2(sink,*(((const f2_t*)(leaf)) )); break;
        case TYPEOF_f3_t    : x_sink_push_f3(sink,*(((const f3_t*)(leaf)) )); break;
        case TYPEOF_s0_t    : x_sink_push_s0(sink,*(((const s0_t*)(leaf)) )); break;
        case TYPEOF_s1_t    : x_sink_push_s1(sink,*(((const s1_t*)(leaf)) )); break;
        case TYPEOF_s2_t    : x_sink_push_s2(sink,*(((const s2_t*)(leaf)) )); break;
        case TYPEOF_s3_t    : x_sink_push_s3(sink,*(((const s3_t*)(leaf)) )); break;
        case TYPEOF_sc_t    : break; // c-style constant strings are skipped over
        case TYPEOF_sz_t    : x_sink_push_sz(sink,*(((const sz_t*)(leaf)) )); break;
        case TYPEOF_uz_t    : x_sink_push_uz(sink,*(((const uz_t*)(leaf)) )); break;
        case TYPEOF_tp_t    : x_sink_push_tp(sink,*(((const tp_t*)(leaf)) )); break;
        case TYPEOF_u0_t    : x_sink_push_u0(sink,*(((const u0_t*)(leaf)) )); break;
        case TYPEOF_u1_t    : x_sink_push_u1(sink,*(((const u1_t*)(leaf)) )); break;
        case TYPEOF_u2_t    : x_sink_push_u2(sink,*(((const u2_t*)(leaf)) )); break;
        case TYPEOF_u3_t    : x_sink_push_u3(sink,*(((const u3_t*)(leaf)) )); break;
        default: ERR_fa( "Cannot convert type '#<sc_t>' into a binary stream.", bnameof( t ) );
    }
}

void x_bcml_push_leaf_arr( x_sink* sink, tp_t t, const x_inst* leaf, sz_t size )
{
    // bcore_x_bcml.h:360:1
    
    switch( t )
    {
        case TYPEOF_aware_t : x_sink_push_data(sink,leaf, size * sizeof( aware_t ) ); break;
        case TYPEOF_bl_t    : { for(sz_t i = 0; i < size; i++ ) x_sink_push_bl(sink,((const bl_t*)(leaf))[ i ] ); } break; // bl is converted
        case TYPEOF_f2_t    : x_sink_push_data(sink,leaf, size * sizeof( f2_t ) ); break;
        case TYPEOF_f3_t    : x_sink_push_data(sink,leaf, size * sizeof( f3_t ) ); break;
        case TYPEOF_s0_t    : x_sink_push_data(sink,leaf, size * sizeof( s0_t ) ); break;
        case TYPEOF_s1_t    : x_sink_push_data(sink,leaf, size * sizeof( s1_t ) ); break;
        case TYPEOF_s2_t    : x_sink_push_data(sink,leaf, size * sizeof( s2_t ) ); break;
        case TYPEOF_s3_t    : x_sink_push_data(sink,leaf, size * sizeof( s3_t ) ); break;
        case TYPEOF_sc_t    : break; // c-style constant strings are skipped over
        case TYPEOF_sz_t    : { for(sz_t i = 0; i < size; i++ ) x_sink_push_s3(sink,((const sz_t*)(leaf))[ i ] ); } break; // sz is converted
        case TYPEOF_uz_t    : { for(sz_t i = 0; i < size; i++ ) x_sink_push_u3(sink,((const uz_t*)(leaf))[ i ] ); } break; // uz is converted
        case TYPEOF_tp_t    : x_sink_push_data(sink,leaf, size * sizeof( tp_t ) ); break;
        case TYPEOF_u0_t    : x_sink_push_data(sink,leaf, size * sizeof( u0_t ) ); break;
        case TYPEOF_u1_t    : x_sink_push_data(sink,leaf, size * sizeof( u1_t ) ); break;
        case TYPEOF_u2_t    : x_sink_push_data(sink,leaf, size * sizeof( u2_t ) ); break;
        case TYPEOF_u3_t    : x_sink_push_data(sink,leaf, size * sizeof( u3_t ) ); break;
        default: ERR_fa( "Cannot convert array of type '#<sc_t>' into a binary stream.", bnameof( t ) );
    }
}

void x_bcml_t_translate_recursive( const x_bcml* o, tp_t t, bl_t push_existence, bl_t push_type, bl_t shelve, x_sink* sink )
{
    // bcore_x_bcml.h:384:1
    
    // shelving obj_l
    if( o && shelve && bcore_via_call_t_defines_shelve(t ) )
    {
        x_bcml* o_clone =((x_bcml*)( x_inst_t_clone(((const x_inst*)(o)),t ))); // no scoping (o_clone is obliv)
        x_stamp_t_shelve(((x_stamp*)(o_clone)),t );
        x_bcml_t_translate_recursive(o_clone,t, push_existence, push_type, false, sink );
        x_inst_discard(((x_inst*)(o_clone)));
        return ;
    }
    
    if( push_type )      x_bcml_push_type(sink, t );
    if( push_existence ) x_bcml_push_flag(sink, o != NULL );
    
    if( o )
    {
        if( x_bcml_t_defines_bcml_body_to_sink(t ) )
        {
            x_bcml_t_bcml_body_to_sink(o,t, sink );
        }
        else if( t == TYPEOF_st_s ) // strings
        {
            const st_s* string = ((const st_s*)(o));
            x_sink_push_data(sink,( vc_t )string->data, string->size + 1 ); // push string including terminating 0
        }
        else if( x_stamp_t_is_leaf(t ) )
        {
            x_bcml_push_leaf(sink, t,((const x_inst*)( o )));
        }
        else
        {
            if( x_stamp_t_is_pure_array(t ) )
            {
                const x_array* arr =((const x_array*)( o));
                if( x_array_t_is_mutable_mono_typed(t ) ) x_bcml_push_type(sink, x_array_t_get_mono_type(arr,t ) );
                sz_t size = x_array_t_size(arr,t );
                x_bcml_push_size(sink, size );
                bl_t is_of_links = x_array_t_is_of_links(t );
                tp_t static_type = x_array_t_get_static_type(t );
    
                if( ( !is_of_links ) && ( static_type != 0 ) && x_stamp_t_is_leaf(static_type ) )
                {
                    x_bcml_push_leaf_arr(sink, static_type, x_array_t_get_data_c(arr,t ), size );
                }
                else
                {
                    for(sz_t i = 0; i < size; i++ )
                    {
                        sr_s sr = x_array_t_c_get_sr(arr,t, i );
                        x_bcml_t_translate_recursive(((const x_bcml*)( sr.o)),sr_s_o_type(&(sr)), is_of_links, static_type == 0, true, sink );
                        sr_down( sr );
                    }
                }
            }
            else
            {
                const x_stamp* stamp =((const x_stamp*)( o));
                sz_t size = x_stamp_t_size(t );
                for(sz_t i = 0; i < size; i++ )
                {
                    sr_s sr = x_stamp_t_c_get_sr_i(stamp,t, i );
                    x_bcml_t_translate_recursive(((const x_bcml*)( sr.o)),sr_s_o_type(&(sr)), x_stamp_t_is_link_i(t, i ), !x_stamp_t_is_static_i(t, i ), true, sink );
                    sr_down( sr );
                }
            }
        }
    }
}

sz_t x_bcml_t_test_transfer( const x_bcml* o, tp_t t )
{
    // bcore_x_bcml.h:456:1
    BLM_INIT_LEVEL(0);
    st_s* string = st_s_create();
    x_bcml_t_to_sink(o,t,((x_sink*)( string )));
    sz_t file_size = string->size;
    
    x_source* source = ((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_st_d(string )));
    
    x_bcml* o2 =((x_bcml*)( x_inst_t_create(t )));
    if( x_bcml_t_from_source(o2,t, source ) )
    {
        bcore_error_pop_all_to_stderr();
        ERR_fa( "\n" );
    }
    
    s2_t c = bcore_compare_bityped( t, o, t, o2 );
    if( c != 0 )
    {BLM_INIT_LEVEL(1);
        const st_s* diff = ((st_s*)BLM_LEVEL_T_PUSH(1,st_s,((st_s*)(bcore_diff_bityped( t, o, t, o2 )))));
        if( diff )
        {
            ERR( "Comparison returned '%"PRIi32"':\n%s\n", c, diff->sc );
        }
        else
        {
            ERR( "Comparison returned '%"PRIi32"' but diff returned 'NULL'\n", c );
        }
    BLM_DOWN();}
    
    x_inst_t_discard(((x_inst*)(o2)),t );
    BLM_RETURNV(sz_t, file_size)
}

void x_bcml_selftest( void )
{
    // bcore_x_bcml.h:491:1
    BLM_INIT_LEVEL(0);
    sr_s zoo;BLM_T_INIT_SPUSH(sr_s, &zoo);; zoo = bcore_spect_via_create_zoo( 1000 );
    
    clock_t time = clock();
    sz_t size = x_bcml_t_test_transfer(((const x_bcml*)(zoo.o)),sr_s_o_type(&( zoo )) );
    time = clock() - time;
    
    bcore_msg_fa( "x_bcml transfer #<f3_t>s; size: #<sz_t> bytes\n", ( f3_t )time/CLOCKS_PER_SEC, size );
    BLM_DOWN();
}

/**********************************************************************************************************************/
// source: bcore_x_hmap.h
#include "bcore_x_hmap.h"

//----------------------------------------------------------------------------------------------------------------------
// group: x_hmap; embeds: bcore_x_hmap.x

XOILA_DEFINE_SPECT( x_inst, x_hmap )
"{"
    "bcore_spect_header_s header;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: x_hmap_tp

BCORE_DEFINE_OBJECT_INST_P( x_hmap_tp_s )
"aware x_hmap_tp"
"{"
    "bcore_hmap_tpaw_s map;"
"}";

bcore_arr_tp_s* x_hmap_tp_s_get_key_arr( const x_hmap_tp_s* o, bcore_arr_tp_s* key_arr )
{
    // bcore_x_hmap.h:124:5
    
    bcore_arr_tp_s_set_size(key_arr,0 );
    sz_t size = x_hmap_tp_s_size(o);
    tp_t key = 0;
    for(sz_t i = 0; i < size; i++ ) if( (key = x_hmap_tp_s_idx_key(o,i )) ) bcore_arr_tp_s_push(key_arr,key );
    return  key_arr;
}

x_array* x_hmap_tp_s_m_get_val_arr( x_hmap_tp_s* o, x_array* val_arr )
{
    // bcore_x_hmap.h:135:5
    
    x_array_set_size(val_arr,0 );
    sz_t size = x_hmap_tp_s_size(o);
    for(sz_t i = 0; i < size; i++ ) if( x_hmap_tp_s_idx_key(o,i ) ) x_array_push_d(val_arr,((x_inst*)bcore_fork(x_hmap_tp_s_m_idx_val(o,i ))) );
    return  val_arr;
}

x_array* x_hmap_tp_s_c_get_val_arr( const x_hmap_tp_s* o, x_array* val_arr )
{
    // bcore_x_hmap.h:143:5
    
    x_array_set_size(val_arr,0 );
    sz_t size = x_hmap_tp_s_size(o);
    for(sz_t i = 0; i < size; i++ ) if( x_hmap_tp_s_idx_key(o,i ) ) x_array_push_d(val_arr,x_inst_clone(x_hmap_tp_s_c_idx_val(o,i )) );
    return  val_arr;
}

BCORE_DEFINE_OBJECT_INST_P( x_hmap_tp_st_s )
"aware x_hmap_tp"
"{"
    "x_hmap_tp_s map;"
    "func ^:TE;"
"}";

st_s* x_hmap_tp_st_s_create_st_status( const x_hmap_tp_st_s* o )
{
    // bcore_x_hmap.x:23:1
    
    st_s* string = st_s_create();
    st_s_push_fa(string,"keys ........... #<sz_t>\n", x_hmap_tp_keys(((const x_hmap_tp*)(o))) );
    st_s_push_fa(string,"nodes .......... #<sz_t>\n", x_hmap_tp_size(((const x_hmap_tp*)(o))) );
    st_s_push_fa(string,"keys/nodes ..... #<f3_t>\n", x_hmap_tp_size(((const x_hmap_tp*)(o))) > 0 ? ( f3_t )( x_hmap_tp_keys(((const x_hmap_tp*)(o))) ) / x_hmap_tp_size(((const x_hmap_tp*)(o))) : 0 );
    return  string;
}

XOILA_DEFINE_SPECT( x_hmap, x_hmap_tp )
"{"
    "bcore_spect_header_s header;"
    "feature aware x_hmap_tp : TE = x_hmap_tp_TE_default;"
"}";

x_hmap_tp_s* x_hmap_tp_m_map_( x_hmap_tp* o )
{
    // bcore_x_hmap.h:153:5
    
    assert( x_stamp_is_aware(((const x_stamp*)(o))) );
    x_inst* map = x_stamp_m_get_i(((x_stamp*)(o)),0 );
    if( !map || map->_ != TYPEOF_x_hmap_tp_s ) ERR_fa( "First element of '#<sc_t>' must be of type '#<sc_t>'", bnameof( o->_ ), bnameof( ((tp_t)(TYPEOF_x_hmap_tp_s)) ) );
    return  ((x_hmap_tp_s*)(map));
}

const x_hmap_tp_s* x_hmap_tp_c_map_( const x_hmap_tp* o )
{
    // bcore_x_hmap.h:161:5
    
    assert( x_stamp_is_aware(((const x_stamp*)(o))) );
    const x_inst* map = x_stamp_c_get_i(((const x_stamp*)(o)),0 );
    if( !map || map->_ != TYPEOF_x_hmap_tp_s ) ERR_fa( "First element of '#<sc_t>' must be of type '#<sc_t>'", bnameof( o->_ ), bnameof( ((tp_t)(TYPEOF_x_hmap_tp_s)) ) );
    return  ((const x_hmap_tp_s*)(map));
}

x_inst* x_hmap_tp_m_getf( x_hmap_tp* o, tp_t key )
{
    // bcore_x_hmap.h:186:5
    
    x_inst* inst = x_hmap_tp_s_m_get(x_hmap_tp_m_map_(o),key );
    if( !inst ) inst = x_hmap_tp_s_set_d(x_hmap_tp_m_map_(o),key, x_inst_create(x_hmap_tp_a_TE(o) ) );
    if( !inst ) ERR_fa( "'#<sc_t>.m_getf:' stamp must define transient type 'TE'.", bnameof( o->_ ) );
    return  inst;
}

//----------------------------------------------------------------------------------------------------------------------
// group: x_hmap_tp_test

BCORE_DEFINE_OBJECT_INST_P( x_hmap_tp_test_kv_s )
"x_hmap_tp_test"
"{"
    "tp_t key;"
    "u3_t val;"
"}";

BCORE_DEFINE_OBJECT_INST_P( x_hmap_tp_test_kv_arr_s )
"aware x_array"
"{"
    "x_hmap_tp_test_kv_s [];"
"}";

BCORE_DEFINE_OBJECT_INST_P( x_hmap_tp_test_val_s )
"aware x_hmap_tp_test"
"{"
    "u3_t val;"
"}";

BCORE_DEFINE_OBJECT_INST_P( x_hmap_tp_test_map_s )
"aware x_hmap_tp"
"{"
    "x_hmap_tp_s map;"
    "func ^:TE;"
"}";

st_s* x_hmap_tp_test_map_s_create_st_status( const x_hmap_tp_test_map_s* o )
{
    // bcore_x_hmap.x:43:5
    
    st_s* string = st_s_create();
    st_s_push_fa(string,"keys ........... #<sz_t>\n", x_hmap_tp_keys(((const x_hmap_tp*)(o))) );
    st_s_push_fa(string,"nodes .......... #<sz_t>\n", x_hmap_tp_size(((const x_hmap_tp*)(o))) );
    st_s_push_fa(string,"keys/nodes ..... #<f3_t>\n", x_hmap_tp_size(((const x_hmap_tp*)(o))) > 0 ? ( f3_t )( x_hmap_tp_keys(((const x_hmap_tp*)(o))) ) / x_hmap_tp_size(((const x_hmap_tp*)(o))) : 0 );
    return  string;
}

XOILA_DEFINE_SPECT( x_hmap, x_hmap_tp_test )
"{"
    "bcore_spect_header_s header;"
"}";

void x_hmap_tp_test_selftest( void )
{
    // bcore_x_hmap.x:54:5
    BLM_INIT_LEVEL(0);
    x_sink* log = x_sink_stdout();
    x_sink_push_fa(log,"#p80'='{== #<sc_t>_selftest }\n", bnameof( ((tp_t)(TYPEOF_x_hmap_tp_test)) ) );
    
    sz_t cycles = 200000;
    
    x_hmap_tp_test_map_s* map    = ((x_hmap_tp_test_map_s*)BLM_LEVEL_T_PUSH(0,x_hmap_tp_test_map_s,x_hmap_tp_test_map_s_create()));
    x_hmap_tp_test_kv_arr_s* kv_arr = ((x_hmap_tp_test_kv_arr_s*)BLM_LEVEL_T_PUSH(0,x_hmap_tp_test_kv_arr_s,x_hmap_tp_test_kv_arr_s_create()));
    x_array_set_space(((x_array*)(kv_arr)),cycles );
    
    clock_t time = clock();
    x_sink_push_fa(log,"Mixed access: " );
    
    x_hmap_tp_test_val_s mval;BLM_T_INIT_SPUSH(x_hmap_tp_test_val_s, &mval);;
    
    {
        u3_t rval1 = 1;
        u3_t rval2 = 12345;
        for(sz_t i = 0; i < cycles; i++ )
        {
            rval1 = bcore_lcg00_u3( rval1 );
            rval2 = bcore_lcg00_u3( rval2 );
            x_hmap_tp_test_kv_s* kv = ((x_hmap_tp_test_kv_s*)(x_array_push(((x_array*)(kv_arr)))));
            kv->key = ( tp_t )rval1;
            kv->val = rval2;
            mval.val = kv->val;
            x_hmap_tp_set_c(((x_hmap_tp*)(map)),kv->key,((const x_inst*)(&( mval ))));
    
            // retrieve
            rval1 = bcore_lcg00_u3( rval1 );
            kv = (&(kv_arr->data[ rval1 % kv_arr->size ]));
    
            u3_t val = ((const x_hmap_tp_test_val_s*)(x_hmap_tp_c_get(((const x_hmap_tp*)(map)),kv->key )))->val;
            if( kv->val != val ) ERR_fa( "value mismatch (#<u3_t> vs #<u3_t>)", kv->val, val );
    
            // delete
            rval1 = bcore_lcg00_u3( rval1 );
            if( ( ( rval1 >> 10 ) & 1 ) == 1 )
            {
                rval1 = bcore_lcg00_u3( rval1 );
                sz_t idx = rval1 % kv_arr->size;
                const x_hmap_tp_test_kv_s* kv =&( kv_arr->data[ idx ]);
                if( !((const x_hmap_tp_test_val_s*)(x_hmap_tp_c_get(((const x_hmap_tp*)(map)),kv->key ))) ) ERR_fa( "key #<tp_t> not found", kv->key );
                x_hmap_tp_remove(((x_hmap_tp*)(map)),kv->key );
                if( ((const x_hmap_tp_test_val_s*)(x_hmap_tp_c_get(((const x_hmap_tp*)(map)),kv->key ))) )  ERR_fa( "deleted key #<tp_t> still exists", kv->key );
                kv_arr->data[ idx ] = kv_arr->data[ kv_arr->size - 1 ];
                x_array_set_size(((x_array*)(kv_arr)),kv_arr->size - 1 );
            }
        }
    }
    
    time = clock() - time;
    x_sink_push_fa(log,"#<f3_t>s\n", ( f3_t )time/CLOCKS_PER_SEC );
    x_sink_push_st_d(log,x_hmap_tp_test_map_s_create_st_status(map) );
    
    time = clock();
    const x_hmap_tp_test_map_s* map_cloned = ((x_hmap_tp_test_map_s*)BLM_LEVEL_T_PUSH(0,x_hmap_tp_test_map_s,x_hmap_tp_test_map_s_clone(map)));
    time = clock() - time;
    x_sink_push_fa(log,"Clone .......... #<f3_t>s\n", ( f3_t )time/CLOCKS_PER_SEC );
    
    time = clock();
    if( bcore_compare_aware( map, map_cloned ) != 0 )
    {
        ERR( "comparison failed:\n%s", bcore_diff_aware( map, map_cloned )->sc );
    }
    
    time = clock() - time;
    x_sink_push_fa(log,"Comparison ..... #<f3_t>s\n", ( f3_t )time/CLOCKS_PER_SEC );
    
    {BLM_INIT_LEVEL(1);
        time = clock();
        x_hmap_tp_test_map_s* map_trans = ((x_hmap_tp_test_map_s*)BLM_LEVEL_T_PUSH(1,x_hmap_tp_test_map_s,x_hmap_tp_test_map_s_create()));
        x_stamp_set_sr(((x_stamp*)(map_trans)),btypeof( "map" ), x_stamp_c_get_sr(((x_stamp*)(map)),btypeof( "map" ) ) );
        time = clock() - time;
        x_sink_push_fa(log,"Stamp-assign ... #<f3_t>s\n", ( f3_t )time/CLOCKS_PER_SEC );
        time = clock();
        sz_t map_size = x_hmap_tp_size(((const x_hmap_tp*)(map)));
        for(uz_t i = 0; i < map_size; i++ )
        {
            tp_t key = x_hmap_tp_idx_key(((const x_hmap_tp*)(map)),i );
            if( key != 0 )
            {
                ASSERT( x_hmap_tp_exists(((const x_hmap_tp*)(map_trans)),key ) );
                ASSERT( ((const x_hmap_tp_test_val_s*)(x_hmap_tp_c_get(((const x_hmap_tp*)(map)),key )))->val == ((const x_hmap_tp_test_val_s*)(x_hmap_tp_c_get(((const x_hmap_tp*)(map_trans)),key )))->val );
            }
        }
        time = clock() - time;
        x_sink_push_fa(log,"verify assign .. #<f3_t>s\n", ( f3_t )time/CLOCKS_PER_SEC );
    BLM_DOWN();}
    
    {BLM_INIT_LEVEL(1);
        time = clock();
        x_hmap_tp_test_map_s* map_trans = ((x_hmap_tp_test_map_s*)BLM_LEVEL_T_PUSH(1,x_hmap_tp_test_map_s,x_hmap_tp_test_map_s_create()));
        st_s* buf = st_s_create();
        x_bbml_to_sink(((const x_bbml*)(map)),((x_sink*)(buf )));
        BLM_TRY_EXIT(x_bbml_from_source(((x_bbml*)(map_trans)),((x_source*)BLM_LEVEL_A_PUSH(1,x_source_create_from_st_d(buf ))) ))
        time = clock() - time;
        x_sink_push_fa(log,"BBML Transfer .. #<f3_t>s\n", ( f3_t )time/CLOCKS_PER_SEC );
        time = clock();
        sz_t map_size = x_hmap_tp_size(((const x_hmap_tp*)(map)));
        for(uz_t i = 0; i < map_size; i++ )
        {
            tp_t key = x_hmap_tp_idx_key(((const x_hmap_tp*)(map)),i );
            if( key != 0 )
            {
                ASSERT( x_hmap_tp_exists(((const x_hmap_tp*)(map_trans)),key ) );
                ASSERT( ((const x_hmap_tp_test_val_s*)(x_hmap_tp_c_get(((const x_hmap_tp*)(map)),key )))->val == ((const x_hmap_tp_test_val_s*)(x_hmap_tp_c_get(((const x_hmap_tp*)(map_trans)),key )))->val );
            }
        }
        time = clock() - time;
        x_sink_push_fa(log,"verify transfer  #<f3_t>s\n", ( f3_t )time/CLOCKS_PER_SEC );
    BLM_DOWN();}
    
    time = clock();
    x_sink_push_fa(log,"\nRead-access of #<sz_t> keys: ", kv_arr->size );
    uz_t read_cycles = 20;
    for(uz_t j = 0; j < read_cycles; j++ )
    {
        for(uz_t i = 0; i < kv_arr->size; i++ )
        {
            u3_t val = ((const x_hmap_tp_test_val_s*)(x_hmap_tp_c_get(((const x_hmap_tp*)(map)),kv_arr->data[ i ].key )))->val;
            if( val != kv_arr->data[ i ].val ) ERR_fa( "value mismatch (#<u3_t> vs #<u3_t>)", val, kv_arr->data[ i ].val );
        }
    }
    time = clock() - time;
    x_sink_push_fa(log,"(#<f3_t> per access)\n", ( ( f3_t )time/CLOCKS_PER_SEC ) / ( kv_arr->size * read_cycles ) );
    
    x_sink_push_fa(log,"\n" );
    x_sink_push_fa(log,"Removal: " );
    time = clock();
    while( kv_arr->size )
    {
        const x_hmap_tp_test_kv_s* kv =&( kv_arr->data[ kv_arr->size - 1 ]);
        if( !((const x_hmap_tp_test_val_s*)(x_hmap_tp_c_get(((const x_hmap_tp*)(map)),kv->key ))) ) ERR_fa( "key #<tp_t> not found", kv->key );
        x_hmap_tp_remove(((x_hmap_tp*)(map)),kv->key );
        if( ((const x_hmap_tp_test_val_s*)(x_hmap_tp_c_get(((const x_hmap_tp*)(map)),kv->key ))) ) ERR_fa( "deleted key #<tp_t> still exists", kv->key );
        x_array_set_size(((x_array*)(kv_arr)),kv_arr->size - 1 );
    }
    time = clock() - time;
    x_sink_push_fa(log,"(#<f3_t>s)\n", ( f3_t )time/CLOCKS_PER_SEC );
    x_sink_push_st_d(log,x_hmap_tp_test_map_s_create_st_status(map) );
    x_sink_push_fa(log,"#r80{=}\n" );
    BLM_DOWN();
}

/**********************************************************************************************************************/
// source: bcore_x_deque.h
#include "bcore_x_deque.h"

//----------------------------------------------------------------------------------------------------------------------
// group: x_deque

BCORE_DEFINE_OBJECT_INST_P( x_deque_inst_adl_s )
"aware x_array"
"{"
    "aware x_inst => [];"
"}";

BCORE_DEFINE_OBJECT_INST_P( x_deque_inst_s )
"aware x_deque"
"{"
    "x_deque_inst_adl_s adl;"
    "sz_t size;"
    "sz_t first;"
"}";

x_deque_inst_s* x_deque_inst_s_set_space( x_deque_inst_s* o, sz_t space )
{
    // bcore_x_deque.h:85:5
    
    ASSERT( space >= o->size );
    x_array_set_space(((x_array*)(&(o->adl))),space );
    return o;
}

x_deque_inst_s* x_deque_inst_s_clear( x_deque_inst_s* o )
{
    // bcore_x_deque.h:91:5
    
    x_array_clear(((x_array*)(&(o->adl))));
    o->first = o->size = 0;
    return o;
}

x_deque_inst_s* x_deque_inst_s_inc_adl_size( x_deque_inst_s* o )
{
    // bcore_x_deque.h:105:5
    
    x_array_set_size(((x_array*)(&(o->adl))),sz_max( 1, o->adl.size * 2 ) );
    for(sz_t i = 0; i < o->first; i++ )
    {
        o->adl.data[ i + o->size ] = o->adl.data[ i ];
        o->adl.data[ i          ] = NULL;
    }
    return o;
}

x_inst* x_deque_inst_s_push_last_d( x_deque_inst_s* o, x_inst* inst )
{
    // bcore_x_deque.h:115:5
    
    if( o->size == o->adl.size ) x_deque_inst_s_inc_adl_size(o);
    x_inst_a_attach( &(o->adl.data[ ( o->first + o->size ) % o->adl.size ] ), (x_inst*)( inst));
    o->size++;
    return  ((x_inst*)(inst));
}

x_inst* x_deque_inst_s_push_first_d( x_deque_inst_s* o, x_inst* inst )
{
    // bcore_x_deque.h:123:5
    
    if( o->size == o->adl.size )  x_deque_inst_s_inc_adl_size(o);
    o->first = ( o->first == 0 ) ? o->adl.size - 1 : o->first - 1;
    x_inst_a_attach( &(o->adl.data[ o->first ] ), (x_inst*)( inst));
    o->size++;
    return  ((x_inst*)(inst));
}

x_inst* x_deque_inst_s_d_pop_last( x_deque_inst_s* o )
{
    // bcore_x_deque.h:132:5
    
    if( o->size == 0 ) return  NULL;
    sz_t adl_idx = x_deque_inst_s_adl_idx(o,o->size - 1 );
    x_inst* inst = ((x_inst*)(o->adl.data[ adl_idx ]));
    o->adl.data[ adl_idx ] = NULL;
    o->size--;
    return  inst;
}

x_inst* x_deque_inst_s_d_pop_first( x_deque_inst_s* o )
{
    // bcore_x_deque.h:142:5
    
    if( o->size == 0 ) return  NULL;
    x_inst* inst = ((x_inst*)(o->adl.data[ o->first ]));
    o->adl.data[ o->first ] = NULL;
    o->size--;
    o->first = ( o->first + 1 ) % o->adl.size;
    return  inst;
}

x_deque_inst_s* x_deque_inst_s_remove_first_n( x_deque_inst_s* o, sz_t n )
{
    // bcore_x_deque.h:155:5
    
    for(sz_t i = 0; i < n && o->size > 0; i++ ) x_deque_inst_s_remove_first(o);
    return o;
}

x_deque_inst_s* x_deque_inst_s_remove_last_n( x_deque_inst_s* o, sz_t n )
{
    // bcore_x_deque.h:160:5
    
    for(sz_t i = 0; i < n && o->size > 0; i++ ) x_deque_inst_s_remove_last(o);
    return o;
}

XOILA_DEFINE_SPECT( x_inst, x_deque )
"{"
    "bcore_spect_header_s header;"
"}";

x_deque_inst_s* x_deque_m_inst_( x_deque* o )
{
    // bcore_x_deque.h:168:1
    
    assert( x_stamp_is_aware(((const x_stamp*)(o))) );
    x_inst* que = x_stamp_m_get_i(((x_stamp*)(o)),0 );
    if( !que || que->_ != TYPEOF_x_deque_inst_s ) ERR_fa( "First element of '#<sc_t>' must be of type '#<sc_t>'", bnameof( o->_ ), bnameof( ((tp_t)(TYPEOF_x_deque_inst_s)) ) );
    return  ((x_deque_inst_s*)(que));
}

const x_deque_inst_s* x_deque_c_inst_( const x_deque* o )
{
    // bcore_x_deque.h:176:1
    
    assert( x_stamp_is_aware(((const x_stamp*)(o))) );
    const x_inst* que = x_stamp_c_get_i(((x_stamp*)(o)),0 );
    if( !que || que->_ != TYPEOF_x_deque_inst_s ) ERR_fa( "First element of '#<sc_t>' must be of type '#<sc_t>'", bnameof( o->_ ), bnameof( ((tp_t)(TYPEOF_x_deque_inst_s)) ) );
    return  ((const x_deque_inst_s*)(que));
}

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
            // source: bcore_x_source.h

            // group: x_source
            BCORE_REGISTER_OBJECT( x_source_point_s );
            XOILA_REGISTER_SPECT( x_source );

            // --------------------------------------------------------------------
            // source: bcore_x_sink.h

            // group: x_sink
            XOILA_REGISTER_SPECT( x_sink );

            // --------------------------------------------------------------------
            // source: bcore_x_array.h

            // group: x_array
            BCORE_REGISTER_NAME( size );
            BCORE_REGISTER_NAME( space );
            BCORE_REGISTER_NAME( data );
            XOILA_REGISTER_SPECT( x_array );

            // --------------------------------------------------------------------
            // source: bcore_x_group.h

            // group: x_group
            XOILA_REGISTER_SPECT( x_group );

            // --------------------------------------------------------------------
            // source: bcore_x_stamp.h

            // group: x_stamp
            BCORE_REGISTER_NAME( x_stamp_path_s_array_index );
            BCORE_REGISTER_NAME( sr_s );
            BCORE_REGISTER_OBJECT( x_stamp_path_s );
            BCORE_REGISTER_OBJECT( x_stamp_path_adl_s );
            XOILA_REGISTER_SPECT( x_stamp );

            // --------------------------------------------------------------------
            // source: bcore_x_threads.h

            // group: x
            BCORE_REGISTER_FFUNC( bcore_inst_call_down_e, x_lock_s_down_e );
            BCORE_REGISTER_OBJECT( x_lock_s );
            BCORE_REGISTER_FFUNC( bcore_inst_call_down_e, x_unlock_s_down_e );
            BCORE_REGISTER_OBJECT( x_unlock_s );
            BCORE_REGISTER_FFUNC( bcore_inst_call_init_x, x_mutex_s_init_x );
            BCORE_REGISTER_FFUNC( bcore_inst_call_down_e, x_mutex_s_down_e );
            BCORE_REGISTER_OBJECT( x_mutex_s );
            BCORE_REGISTER_FFUNC( bcore_inst_call_init_x, x_condition_s_init_x );
            BCORE_REGISTER_FFUNC( bcore_inst_call_down_e, x_condition_s_down_e );
            BCORE_REGISTER_OBJECT( x_condition_s );
            XOILA_REGISTER_SPECT( x );

            // group: x_thread
            BCORE_REGISTER_FEATURE( x_thread_m_thread_func );
            BCORE_REGISTER_FEATURE( x_thread_c_thread_func );
            BCORE_REGISTER_FFUNC( bcore_inst_call_down_e, x_thread_s_down_e );
            BCORE_REGISTER_OBJECT( x_thread_s );
            XOILA_REGISTER_SPECT( x_thread );

            // group: x_thread_result
            XOILA_REGISTER_SPECT( x_thread_result );

            // --------------------------------------------------------------------
            // source: bcore_file.h

            // group: bcore_file
            BCORE_REGISTER_FFUNC( bcore_via_call_source, bcore_file_path_s_source );
            BCORE_REGISTER_FFUNC( bcore_fp_copy_typed, bcore_file_path_s_copy_typed );
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
            XOILA_REGISTER_SPECT( bcore_main );

            // --------------------------------------------------------------------
            // source: bcore_hmap_name.h

            // group: bcore_hmap_name
            BCORE_REGISTER_OBJECT( bcore_hmap_name_s );
            XOILA_REGISTER_SPECT( bcore_hmap_name );

            // --------------------------------------------------------------------
            // source: bcore_hmap_tp_st.h

            // group: bcore_hmap_tp_st
            BCORE_REGISTER_OBJECT( bcore_hmap_tp_st_s );
            XOILA_REGISTER_SPECT( bcore_hmap_tp_st );

            // --------------------------------------------------------------------
            // source: bcore_cday.h

            // group: bcore_cday
            BCORE_REGISTER_OBJECT( bcore_cday_ymd_s );
            BCORE_REGISTER_OBJECT( bcore_cday_utc_s );
            XOILA_REGISTER_SPECT( bcore_cday );

            // --------------------------------------------------------------------
            // source: bcore_error_manager.h

            // group: bcore_error
            BCORE_REGISTER_NAME( general_error );
            BCORE_REGISTER_NAME( parse_error );
            BCORE_REGISTER_NAME( plant_error );
            BCORE_REGISTER_NAME( error_stack );
            XOILA_REGISTER_SPECT( bcore_error );

            // group: bcore_error_manager
            BCORE_REGISTER_OBJECT( bcore_error_manager_error_s );
            BCORE_REGISTER_OBJECT( bcore_error_manager_error_adl_s );
            BCORE_REGISTER_OBJECT( bcore_error_manager_context_s );
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

            // --------------------------------------------------------------------
            // source: bcore_shell.h

            // group: bcore_shell
            BCORE_REGISTER_FEATURE( bcore_shell_loop_callback );
            BCORE_REGISTER_FFUNC( bcore_shell_loop_callback, bcore_shell_loop_callback_default );
            BCORE_REGISTER_FEATURE( bcore_shell_op_group );
            BCORE_REGISTER_FFUNC( bcore_shell_op_group, bcore_shell_op_group_default );
            BCORE_REGISTER_FEATURE( bcore_shell_push_op_groups );
            BCORE_REGISTER_FFUNC( bcore_shell_push_op_groups, bcore_shell_push_op_groups_default );
            XOILA_REGISTER_SPECT( bcore_shell );

            // group: bcore_shell_op
            BCORE_REGISTER_FEATURE( bcore_shell_op_key );
            BCORE_REGISTER_FEATURE( bcore_shell_op_get_key );
            BCORE_REGISTER_FFUNC( bcore_shell_op_get_key, bcore_shell_op_get_key_default );
            BCORE_REGISTER_FEATURE( bcore_shell_op_info );
            BCORE_REGISTER_FEATURE( bcore_shell_op_get_info );
            BCORE_REGISTER_FFUNC( bcore_shell_op_get_info, bcore_shell_op_get_info_default );
            BCORE_REGISTER_FEATURE( bcore_shell_op_run );
            XOILA_REGISTER_SPECT( bcore_shell_op );

            // group: bcore_shell_control
            BCORE_REGISTER_OBJECT( bcore_shell_control_s );
            XOILA_REGISTER_SPECT( bcore_shell_control );

            // group: bcore_shell_op_default
            BCORE_REGISTER_FFUNC( bcore_shell_op_key, bcore_shell_op_default_help_s_key );
            BCORE_REGISTER_FFUNC( bcore_shell_op_info, bcore_shell_op_default_help_s_info );
            BCORE_REGISTER_FFUNC( bcore_shell_op_run, bcore_shell_op_default_help_s_run );
            BCORE_REGISTER_OBJECT( bcore_shell_op_default_help_s );
            BCORE_REGISTER_FFUNC( bcore_shell_op_get_key, bcore_shell_op_default_ls_s_get_key );
            BCORE_REGISTER_FFUNC( bcore_shell_op_get_info, bcore_shell_op_default_ls_s_get_info );
            BCORE_REGISTER_FFUNC( bcore_shell_op_run, bcore_shell_op_default_ls_s_run );
            BCORE_REGISTER_OBJECT( bcore_shell_op_default_ls_s );
            BCORE_REGISTER_FFUNC( bcore_shell_op_key, bcore_shell_op_default_enter_s_key );
            BCORE_REGISTER_FFUNC( bcore_shell_op_info, bcore_shell_op_default_enter_s_info );
            BCORE_REGISTER_FFUNC( bcore_shell_op_run, bcore_shell_op_default_enter_s_run );
            BCORE_REGISTER_OBJECT( bcore_shell_op_default_enter_s );
            BCORE_REGISTER_FFUNC( bcore_shell_op_key, bcore_shell_op_default_exit_s_key );
            BCORE_REGISTER_FFUNC( bcore_shell_op_info, bcore_shell_op_default_exit_s_info );
            BCORE_REGISTER_FFUNC( bcore_shell_op_run, bcore_shell_op_default_exit_s_run );
            BCORE_REGISTER_OBJECT( bcore_shell_op_default_exit_s );
            BCORE_REGISTER_FFUNC( bcore_shell_op_key, bcore_shell_op_default_get_s_key );
            BCORE_REGISTER_FFUNC( bcore_shell_op_info, bcore_shell_op_default_get_s_info );
            BCORE_REGISTER_FFUNC( bcore_shell_op_run, bcore_shell_op_default_get_s_run );
            BCORE_REGISTER_OBJECT( bcore_shell_op_default_get_s );
            BCORE_REGISTER_FFUNC( bcore_shell_op_key, bcore_shell_op_default_set_s_key );
            BCORE_REGISTER_FFUNC( bcore_shell_op_info, bcore_shell_op_default_set_s_info );
            BCORE_REGISTER_FFUNC( bcore_shell_op_run, bcore_shell_op_default_set_s_run );
            BCORE_REGISTER_OBJECT( bcore_shell_op_default_set_s );
            BCORE_REGISTER_FFUNC( bcore_shell_op_key, bcore_shell_op_default_alias_s_key );
            BCORE_REGISTER_FFUNC( bcore_shell_op_info, bcore_shell_op_default_alias_s_info );
            BCORE_REGISTER_FFUNC( bcore_shell_op_run, bcore_shell_op_default_alias_s_run );
            BCORE_REGISTER_OBJECT( bcore_shell_op_default_alias_s );
            BCORE_REGISTER_FFUNC( bcore_shell_op_key, bcore_shell_op_default_stdin_s_key );
            BCORE_REGISTER_FFUNC( bcore_shell_op_info, bcore_shell_op_default_stdin_s_info );
            BCORE_REGISTER_FFUNC( bcore_shell_op_run, bcore_shell_op_default_stdin_s_run );
            BCORE_REGISTER_OBJECT( bcore_shell_op_default_stdin_s );
            BCORE_REGISTER_FFUNC( bcore_shell_op_key, bcore_shell_op_default_stdout_s_key );
            BCORE_REGISTER_FFUNC( bcore_shell_op_info, bcore_shell_op_default_stdout_s_info );
            BCORE_REGISTER_FFUNC( bcore_shell_op_run, bcore_shell_op_default_stdout_s_run );
            BCORE_REGISTER_OBJECT( bcore_shell_op_default_stdout_s );
            XOILA_REGISTER_SPECT( bcore_shell_op_default );
            {
               bcore_arr_tp_s* arr = bcore_arr_tp_s_create();
               bcore_arr_tp_s_push( arr, TYPEOF_bcore_shell_op_default_help_s );
               bcore_arr_tp_s_push( arr, TYPEOF_bcore_shell_op_default_ls_s );
               bcore_arr_tp_s_push( arr, TYPEOF_bcore_shell_op_default_enter_s );
               bcore_arr_tp_s_push( arr, TYPEOF_bcore_shell_op_default_exit_s );
               bcore_arr_tp_s_push( arr, TYPEOF_bcore_shell_op_default_get_s );
               bcore_arr_tp_s_push( arr, TYPEOF_bcore_shell_op_default_set_s );
               bcore_arr_tp_s_push( arr, TYPEOF_bcore_shell_op_default_alias_s );
               bcore_arr_tp_s_push( arr, TYPEOF_bcore_shell_op_default_stdin_s );
               bcore_arr_tp_s_push( arr, TYPEOF_bcore_shell_op_default_stdout_s );
               bcore_xoila_set_arr_traitline_stamps_d( TYPEOF_bcore_shell_op_default, arr );
            }

            // --------------------------------------------------------------------
            // source: bcore_huffman.h

            // group: bcore_huffman
            BCORE_REGISTER_OBJECT( bcore_huffman_codec_s );
            BCORE_REGISTER_FFUNC( x_bbml_bbml_body_to_sink, bcore_huffman_bit_buffer_s_bbml_body_to_sink );
            BCORE_REGISTER_FFUNC( x_bbml_bbml_body_from_source, bcore_huffman_bit_buffer_s_bbml_body_from_source );
            BCORE_REGISTER_FFUNC( x_bcml_bcml_body_to_sink, bcore_huffman_bit_buffer_s_bcml_body_to_sink );
            BCORE_REGISTER_FFUNC( x_bcml_bcml_body_from_source, bcore_huffman_bit_buffer_s_bcml_body_from_source );
            BCORE_REGISTER_OBJECT( bcore_huffman_bit_buffer_s );
            BCORE_REGISTER_OBJECT( bcore_huffman_bit_buffer_iterator_s );
            BCORE_REGISTER_OBJECT( bcore_huffman_hist_s );
            BCORE_REGISTER_OBJECT( bcore_huffman_index_s );
            BCORE_REGISTER_OBJECT( bcore_huffman_node_s );
            BCORE_REGISTER_OBJECT( bcore_huffman_count_node_s );
            BCORE_REGISTER_OBJECT( bcore_huffman_count_map_s );
            BCORE_REGISTER_OBJECT( bcore_huffman_tree_s );
            XOILA_REGISTER_SPECT( bcore_huffman );

            // --------------------------------------------------------------------
            // source: bcore_indexer.h

            // group: bcore_indexer
            BCORE_REGISTER_OBJECT( bcore_indexer_io_s );
            BCORE_REGISTER_OBJECT( bcore_indexer_io_arr_s );
            BCORE_REGISTER_OBJECT( bcore_indexer_cs_s );
            BCORE_REGISTER_OBJECT( bcore_indexer_cs_arr_s );
            BCORE_REGISTER_OBJECT( bcore_indexer_s );
            XOILA_REGISTER_SPECT( bcore_indexer );

            // --------------------------------------------------------------------
            // source: bcore_x_btml.h

            // group: x_btml
            BCORE_REGISTER_FEATURE( x_btml_btml_body_from_source );
            BCORE_REGISTER_FEATURE( x_btml_btml_body_to_sink );
            XOILA_REGISTER_SPECT( x_btml );

            // --------------------------------------------------------------------
            // source: bcore_x_bbml.h

            // group: x_bbml
            BCORE_REGISTER_FEATURE( x_bbml_bbml_body_from_source );
            BCORE_REGISTER_FEATURE( x_bbml_bbml_body_to_sink );
            XOILA_REGISTER_SPECT( x_bbml );

            // --------------------------------------------------------------------
            // source: bcore_x_bcml.h

            // group: x_bcml
            BCORE_REGISTER_FEATURE( x_bcml_bcml_body_from_source );
            BCORE_REGISTER_FEATURE( x_bcml_bcml_body_to_sink );
            XOILA_REGISTER_SPECT( x_bcml );

            // --------------------------------------------------------------------
            // source: bcore_x_hmap.h

            // group: x_hmap
            XOILA_REGISTER_SPECT( x_hmap );

            // group: x_hmap_tp
            BCORE_REGISTER_FEATURE( x_hmap_tp_TE );
            BCORE_REGISTER_FFUNC( x_hmap_tp_TE, x_hmap_tp_TE_default );
            BCORE_REGISTER_OBJECT( x_hmap_tp_s );
            BCORE_REGISTER_FFUNC( x_hmap_tp_TE, x_hmap_tp_st_s_TE );
            BCORE_REGISTER_OBJECT( x_hmap_tp_st_s );
            XOILA_REGISTER_SPECT( x_hmap_tp );

            // group: x_hmap_tp_test
            BCORE_REGISTER_OBJECT( x_hmap_tp_test_kv_s );
            BCORE_REGISTER_OBJECT( x_hmap_tp_test_kv_arr_s );
            BCORE_REGISTER_OBJECT( x_hmap_tp_test_val_s );
            BCORE_REGISTER_FFUNC( x_hmap_tp_TE, x_hmap_tp_test_map_s_TE );
            BCORE_REGISTER_OBJECT( x_hmap_tp_test_map_s );
            XOILA_REGISTER_SPECT( x_hmap_tp_test );

            // --------------------------------------------------------------------
            // source: bcore_x_deque.h

            // group: x_deque
            BCORE_REGISTER_OBJECT( x_deque_inst_adl_s );
            BCORE_REGISTER_OBJECT( x_deque_inst_s );
            XOILA_REGISTER_SPECT( x_deque );
        }
        break;
        default: break;
    }
    return NULL;
}
// XOICO_BODY_SIGNATURE 0xC7319851F6F0DFE4
// XOICO_FILE_SIGNATURE 0x33D718FB404B36E4
