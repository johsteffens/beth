//  Last update: 2024-12-09T11:18:14Z
/** This file was generated from xoila source code.
 *  Compiling Agent : XOICO (C) 2020 ... 2024 J.B.Steffens
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
 *  bcore_x_compare.h
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
 *  bcore_x_btcl.h
 *  bcore_x_hmap.h
 *  bcore_x_deque.h
 *  bcore_arr_inexpandable.x
 *  bcore_const_manager_inexpandable.x
 *  bcore_flect_inexpandable.x
 *  bcore_folder_inexpandable.x
 *  bcore_function_manager_inexpandable.x
 *  bcore_global_manager_inexpandable.x
 *  bcore_hmap_inexpandable.x
 *  bcore_hmap_tp_sr_inexpandable.x
 *  bcore_huffman.x
 *  bcore_img_inexpandable.x
 *  bcore_name_manager_inexpandable.x
 *  bcore_sink_inexpandable.x
 *  bcore_source_inexpandable.x
 *  bcore_sr_inexpandable.x
 *  bcore_st_inexpandable.x
 *  bcore_via_inexpandable.x
 *  bcore_x_btcl.x
 *  bcore_x_btcl_bop.x
 *  bcore_x_btcl_builtin.x
 *  bcore_x_btcl_export.x
 *  bcore_x_btcl_net.x
 *  bcore_x_hmap.x
 *
 */

#include "bcore.xo.h"
#include "bcore_spect.h"
#include "bcore_spect_inst.h"
#include "bcore_sr.h"
#include "bcore_const_manager.h"

// To force a rebuild of this target by xoico, reset the hash key value below to 0.
// HKEYOF_bcore 0xEF148E3D78A00306ull

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
// source: bcore_x_compare.h
#include "bcore_x_compare.h"

//----------------------------------------------------------------------------------------------------------------------
// group: x_compare

XOILA_DEFINE_SPECT( bcore_inst, x_compare )
"{"
    "bcore_spect_header_s header;"
"}";

s2_t x_compare_t_type_dominant( tp_t ta, const x_inst* a, tp_t tb, const x_inst* b )
{
    // bcore_x_compare.h:66:1
    
    return  ( ta == tb ) ? bcore_compare_typed( ta, a, b ) : ( ta < tb ) ? 1 : -1;
}

s2_t x_compare_t_num_dominant( tp_t ta, const x_inst* a, tp_t tb, const x_inst* b )
{
    // bcore_x_compare.h:77:1
    
    if( bcore_tp_is_numeric( ta ) && bcore_tp_is_numeric( tb ) && a && b )
    {
        sr_s sr_a = sr_twc(ta, a );
        sr_s sr_b = sr_twc(tb, b );
        if( sr_s_is_float(&(sr_a)) || sr_s_is_float(&(sr_b)) )
        {
            f3_t fa = sr_s_to_f3(&(sr_a));
            f3_t fb = sr_s_to_f3(&(sr_b));
            return  ( fa == fb ) ? 0 : ( fa < fb ) ? 1 : -1;
        }
        else if( sr_s_is_unsigned(&(sr_a)) )
        {
            u3_t ua = sr_s_to_u3(&(sr_a));
            if( sr_s_is_unsigned(&(sr_b)) )
            {
                u3_t ub = sr_s_to_u3(&(sr_b));
                return  ( ua == ub ) ? 0 : ( ua < ub ) ? 1 : -1;
            }
            else
            {
                s3_t sb = sr_s_to_s3(&(sr_b));
                return  ( ua == sb ) ? 0 : ( ua < sb ) ? 1 : -1;
            }
        }
        else
        {
            s3_t sa = sr_s_to_s3(&(sr_a));
            if( sr_s_is_unsigned(&(sr_b)) )
            {
                u3_t ub = sr_s_to_u3(&(sr_b));
                return  ( sa == ub ) ? 0 : ( sa < ub ) ? 1 : -1;
            }
            else
            {
                s3_t sb = sr_s_to_s3(&(sr_b));
                return  ( sa == sb ) ? 0 : ( sa < sb ) ? 1 : -1;
            }
        }
    }
    else
    {
        return  x_compare_t_type_dominant(ta, a, tb, b );
    }
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
    // bcore_x_source.h:204:1
    
    x_source_point_s_parse_msg_to_sink_fv(o,x_sink_stdout(), format, args );
}

x_source* x_source_point_s_clone_source( const x_source_point_s* o )
{
    // bcore_x_source.h:211:1
    
    x_source* source = x_source_a_clone(o->source);
    x_source_set_index(source,o->index );
    return  source;
}

void x_source_point_s_parse_msg_to_sink_fv( const x_source_point_s* o, x_sink* sink, sc_t format, va_list args )
{
    // bcore_x_source.h:220:1
    
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
    // bcore_x_source.h:237:1
    
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
    // bcore_x_source.h:256:1
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

st_s* x_source_get_d_dir( const x_source* o )
{
    // bcore_x_source.h:158:1
    
    sc_t file = x_source_get_file(o);
    if( !file ) return  NULL;
    return  bcore_file_folder_path(file );
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
    // bcore_x_array.h:170:1
    
    
        const bcore_array_s* p = bcore_array_s_get_typed( t );
    
        if( p->item_p )
        {
            bcore_array_p_push( p, ( bcore_array* )o, sr_psm( p->item_p, v ) );
        }
        else
        {
            bcore_array_p_push( p, ( bcore_array* )o, sr_asm( v ) );
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
    // bcore_x_array.h:204:1
    
    
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
    // bcore_x_array.h:240:1
    
    
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
    // bcore_x_array.h:268:1
    
    
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
    // bcore_x_array.h:312:1
    
    
        const bcore_array_s* p = bcore_array_s_get_typed( t );
    
        if( p->item_p )
        {
            bcore_array_p_set( p, ( bcore_array* )o, index, sr_psm( p->item_p, v ) );
        }
        else
        {
            bcore_array_p_set( p, ( bcore_array* )o, index, sr_asm( v ) );
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
    // bcore_x_array.h:345:1
    
    
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
    // bcore_x_array.h:379:1
    
    
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
    // bcore_x_array.h:405:1
    
    sr_s sr = bcore_array_t_get( t, ((const bcore_array*)(o)), index );
    if( sr_s_is_weak(&(sr)) ) sr_s_set_const(&(sr),true );
    return  sr;
}

//----------------------------------------------------------------------------------------------------------------------
// group: x_array_feature

XOILA_DEFINE_SPECT( x_array, x_array_feature )
"{"
    "bcore_spect_header_s header;"
    "feature aware x_array_feature : c_get_wrapped_array = x_array_feature_c_get_wrapped_array_default;"
    "feature aware x_array_feature : m_get_wrapped_array = x_array_feature_m_get_wrapped_array_default;"
"}";

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
    // bcore_x_stamp.h:179:5
    
    x_stamp_path_s_push(o,TYPEOF_x_stamp_path_s_array_index );
    x_stamp_path_s_push(o,index );
}

const x_stamp_path_s* x_stamp_path_s_to_sink( const x_stamp_path_s* o, bcore_sink* sink )
{
    // bcore_x_stamp.h:226:1
    
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
    // bcore_x_stamp.h:246:1
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
    // bcore_x_stamp.h:275:1
    
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
    // bcore_x_stamp.h:324:1
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
    // bcore_x_stamp.h:390:1
    
    const bcore_via_s* p = bcore_via_s_get_typed( t );
    sz_t index = bcore_via_p_nget_index( p, ((const bcore_via*)(o)), name );
    if( index < 0 ) return  sr_null();
    return  bcore_via_p_iget( p, ((const bcore_via*)(o)), index );
}

sr_s x_stamp_t_m_get_sr_i( x_stamp* o, tp_t t, sz_t index )
{
    // bcore_x_stamp.h:400:1
    
    const bcore_via_s* p = bcore_via_s_get_typed( t );
    if( index < 0 || index >= bcore_via_p_get_size( p, ((const bcore_via*)(o)) ) ) return  sr_null();
    return  bcore_via_p_iget( p, ((const bcore_via*)(o)), index );
}

void x_stamp_t_set_sr( x_stamp* o, tp_t t, tp_t name, sr_s sr_src )
{
    // bcore_x_stamp.h:412:1
    
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
    // bcore_x_stamp.h:430:1
    
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
    // bcore_x_stamp.h:450:1
    
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
    // bcore_x_stamp.h:467:1
    
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
    // bcore_x_stamp.h:484:1
    
    sr_s sr = x_stamp_t_m_get_sr(((x_stamp*)(o)),t, name );
    if( sr_s_is_weak(&(sr)) ) sr_s_set_const(&(sr),true );
    return  sr;
}

sr_s x_stamp_t_c_get_sr_i( const x_stamp* o, tp_t t, sz_t index )
{
    // bcore_x_stamp.h:494:1
    
    sr_s sr = x_stamp_t_m_get_sr_i(((x_stamp*)(o)),t, index );
    if( sr_s_is_weak(&(sr)) ) sr_s_set_const(&(sr),true );
    return  sr;
}

void x_stamp_selftest( void )
{
    // bcore_x_stamp.h:507:1
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

er_t bcore_file_path_s_copy_typed( bcore_file_path_s* o, tp_t type, vc_t src )
{
    // bcore_file.h:66:5
    
    if( type == TYPEOF_st_s )
    {
        st_s_copy(&(o->name),( st_s* )src );
    }
    else
    {
        return  bcore_error_push_fa(TYPEOF_conversion_error, "Cannot copy from '#<sc_t>'\n", bnameof( type ) );
    }
    return  0;
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
    "bl_t next_argument_is_path_to_script = true;"
    "bl_t create_log_file = false;"
    "st_s log_file_extension = \"log\";"
    "st_s local_path = \"beth.config\";"
    "bl_t local_path_descend = false;"
    "st_s global_path;"
    "tp_t object_default_type = 0;"
    "sc_t object_interpreter = \"x_btcl\";"
    "bcore_arr_st_s args;"
    "hidden bcore_mutex_s mutex;"
    "hidden aware bcore_source -> source;"
    "hidden aware bcore_sink -> sink;"
    "hidden aware bcore_sink -> log;"
    "sr_s object_sr;"
    "st_s object_path;"
    "st_s script_path;"
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
    "func x_array_feature:c_get_wrapped_array;"
    "func x_array_feature:m_get_wrapped_array;"
    "func ^:main;"
    "func ^:on_termination;"
    "func ^:on_interrupt;"
    "func ^:on_suspend;"
"}";

er_t bcore_main_set_s_main( bcore_main_set_s* o, bcore_main_frame_s* frame )
{
    // bcore_main.h:141:5
    
    {const bcore_main_arr_s* __a=&(o->arr );if(__a)for(sz_t __i=0;__i<__a->size;__i++){bcore_main* e=__a->data[__i];
    {
        {BLM_INIT_LEVEL(5);
            bcore_lock_s lock;BLM_T_INIT_SPUSH(bcore_lock_s, &lock);bcore_lock_s_set(&(lock),&(o->mutex_current_object ));
            o->current_object = e;
        BLM_DOWN();}
        if( !bcore_main_defines_main(e) ) return  bcore_error_push_fa(TYPEOF_general_error, "#<sc_t>.main: Object '#<sc_t>' does not define callback feature bcore_main.main", ifnameof( ((tp_t)(TYPEOF_bcore_main_set_s)) ), ifnameof( e->_ ) );
        BLM_TRY(bcore_main_a_main(e,frame ) )
    };}}
    o->current_object = NULL;
    return  0;
}

bl_t bcore_main_set_s_on_termination( bcore_main_set_s* o, const bcore_main_frame_s* frame )
{
    // bcore_main.h:156:5
    BLM_INIT_LEVEL(0);
    bcore_lock_s lock;BLM_T_INIT_SPUSH(bcore_lock_s, &lock);bcore_lock_s_set(&(lock),&(o->mutex_current_object ));
    BLM_RETURNV(bl_t, o->current_object ? bcore_main_a_on_termination(o->current_object,frame ) : false)
}

bl_t bcore_main_set_s_on_interrupt( bcore_main_set_s* o, const bcore_main_frame_s* frame )
{
    // bcore_main.h:162:5
    BLM_INIT_LEVEL(0);
    bcore_lock_s lock;BLM_T_INIT_SPUSH(bcore_lock_s, &lock);bcore_lock_s_set(&(lock),&(o->mutex_current_object ));
    BLM_RETURNV(bl_t, o->current_object ? bcore_main_a_on_interrupt(o->current_object,frame ) : false)
}

bl_t bcore_main_set_s_on_suspend( bcore_main_set_s* o, const bcore_main_frame_s* frame )
{
    // bcore_main.h:168:5
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
    // bcore_shell.h:307:1
    
    bcore_arr_tp_s_push(list,bcore_shell_a_op_group(o) );
    if( !bcore_arr_tp_s_exists(list,((tp_t)(TYPEOF_bcore_shell_op_default)) ) ) bcore_arr_tp_s_push(list,((tp_t)(TYPEOF_bcore_shell_op_default)) );
}

bcore_arr_tp_s* bcore_shell_get_op_stamps( const bcore_shell* o )
{
    // bcore_shell.h:315:1
    BLM_INIT_LEVEL(0);
    bcore_arr_tp_s op_groups;BLM_T_INIT_SPUSH(bcore_arr_tp_s, &op_groups);;
    bcore_shell_a_push_op_groups(o,&(op_groups ));
    bcore_arr_tp_s* op_stamps = bcore_arr_tp_s_create();
    {const bcore_arr_tp_s* __a=&(op_groups );if(__a)for(sz_t __i=0;__i<__a->size;__i++){tp_t t=__a->data[__i]; bcore_arr_tp_s_push_arr(op_stamps,x_group_get_arr_traitline_stamps(t ) );}}
    BLM_RETURNV(bcore_arr_tp_s*, op_stamps)
}

void bcore_shell_help_to_sink( bcore_shell* o, const bcore_shell_control_s* control, bcore_sink* sink )
{
    // bcore_shell.h:326:1
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
    // bcore_shell.h:382:1
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
    // bcore_shell.h:422:1
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
    // bcore_shell.h:87:5
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
    // bcore_shell.h:112:5
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
    // bcore_shell.h:126:5
    
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
    
            x_stamp_set_sr_i(stamp,direct_index, sr_tsm(type, ((x_inst*)bcore_fork(inst)) ) );
            direct_index++;
        BLM_DOWN();}
        BLM_TRY_EXIT(x_source_parse_fa(source,"#skip' \t'" ))
    }
    return  true;
}

void bcore_shell_op_arg_signature_to_sink( bcore_shell_op* o, bcore_sink* sink )
{
    // bcore_shell.h:227:5
    
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
    // bcore_shell.h:479:9
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
    // bcore_shell.h:532:9
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
    // bcore_shell.h:580:9
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
    // bcore_shell.h:600:9
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
            BLM_TRY_EXIT(x_inst_t_copy_typed(sr.o,sr_s_type(&( sr )), type,((const x_inst*)( val ))))
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
    // bcore_shell.h:635:9
    
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
    // bcore_x_btml.h:110:1
    
    sr_s sr = sr_null();
    BLM_TRY(x_btml_parse_create_object(source, NULL, 0, (&(sr)) ))
    BLM_TRY(x_inst_t_copy_typed(((x_inst*)( o)),t, sr_s_o_type( (&(sr)) ), sr.o ))
    sr_s_down(&( sr ));
    return  0;
}

x_btml* x_btml_create_from_source_t( x_source* source, tp_t* type )
{
    // bcore_x_btml.h:121:1
    
    if( !source )
    {
        (*(type)) = 0;
        return  NULL;
    }
    sr_s sr = sr_null();
    if( x_btml_parse_create_object(source, NULL, 0, (&(sr)) ) ) return  NULL;
    if( sr.o && type ) (*(type)) = sr_s_o_type(&( sr ));
    return  ((x_btml*)(sr.o)); // sr.o is NULL in case of error
}

x_btml* x_btml_create_from_source( x_source* source )
{
    // bcore_x_btml.h:136:1
    
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
    // bcore_x_btml.h:149:1
    
    x_btml_t_translate_recursive(o,t, 0, true, sink, 0 );
    return sink;
}

st_s* x_btml_parse_name( x_source* source )
{
    // bcore_x_btml.h:165:1
    BLM_INIT_LEVEL(0);
    st_s* st_name = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
    BLM_TRY_EXIT(x_source_parse_fa(source," #name", st_name ))
    BLM_RETURNV(st_s*, ((st_s*)bcore_fork(st_name)))
}

sc_t x_btml_name_of( tp_t type, st_s* buf )
{
    // bcore_x_btml.h:174:1
    
    sc_t n = bcore_name_try_name(type );
    if( n ) return  n;
    BLM_TRY_EXIT(st_s_copy_typed(buf,((tp_t)(TYPEOF_tp_t)), (&(type)) ))
    return  buf->sc;
}

tp_t x_btml_type_of( const st_s* name )
{
    // bcore_x_btml.h:184:1
    
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
    // bcore_x_btml.h:204:1
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
            if( type_string->size > 0 )
            {
                if( type_string->data[ type_string->size - 1 ] == '/' ) st_s_pop_char(type_string);
            }
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
    // bcore_x_btml.h:234:1
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

er_t x_btml_parse_create_object( x_source* source, const sr_s* default_obj, tp_t default_tp, sr_s* obj )
{
    // bcore_x_btml.h:262:1
    BLM_INIT_LEVEL(0);
    er_t er = 0;
    st_s* type_string = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
    BLM_TRY(x_source_parse_fa(source," " )) // take whitespaces
    
    if( x_source_parse_bl(source,"#?'<'" ) ) // type specifier
    {
        BLM_TRY(x_source_parse_fa(source,"#until'>'>", type_string ))
        bl_t compact = false; // compact version of <mystamp_s></> is <mystamp_s/>
        tp_t type = 0;
        if( type_string->size > 0 && type_string->data[ type_string->size - 1 ] =='/' )
        {
            st_s_pop_char(type_string);
            compact = true;
        }
    
        if( type_string->size == 1 && type_string->data[ 0 ] =='*' )
        {
            if( default_tp == 0 )
            {
                BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Context does not provide an inheritable type." ))
            }
            type = default_tp;
        }
        else
        {
            type = x_btml_type_of(type_string );
        }
    
        if( type )
        {
            if( bcore_flect_exists( type ) )
            {
                if( x_source_parse_bl(source," #?'NULL'" ) ) // no instance
                {
                    if( !compact) BLM_TRY(x_source_parse_fa(source," </>" ))
                    obj->p = bcore_inst_s_get_typed( type );
                }
                else
                {
                    x_inst* inst = ((x_inst*)BLM_LEVEL_TV_PUSH(0,type ,x_inst_t_create(type )));
                    if( sr_s_type(default_obj) == type ) x_inst_t_copy( inst,type, default_obj->o );
                    if( !compact)
                    {
                        BLM_TRY(x_btml_t_parse_body(((x_btml*)( inst)),type, source ))
                        BLM_TRY(x_source_parse_fa(source," </>" ))
                    }
                    else
                    {
                        x_stamp_t_source(((x_stamp*)(inst)),type, source );
                    }
                    (*(obj)) = sr_tsm(type, ((x_inst*)bcore_fork(inst)) );
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
                            BLM_TRY(x_btml_parse_create_object(((x_source*)(((bcore_source*)BLM_LEVEL_A_PUSH(6,bcore_file_open_source(path->sc ))))), NULL, 0, obj ))
                        BLM_DOWN();}
                        else
                        {
                            BLM_RETURNV(er_t, x_source_parse_error_fa(source,"File '#<sc_t>' not found.", path->sc ))
                        }
                    }
                    else
                    {
                        (*(obj)) = sr_asm(((x_inst*)( ((st_s*)bcore_fork(path)) )));
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
        (*(obj)) = sr_tsm(((tp_t)(TYPEOF_st_s)),((x_inst*)( ((st_s*)bcore_fork(st)) )));
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
    // bcore_x_btml.h:405:1
    
    sr_s default_element = sr_null();
    const sr_s* default_sr = NULL;
    
    if( x_source_parse_bl(source,"#?w'#default:'" ) )
    {
        BLM_TRY(x_btml_parse_create_object(source, NULL, 0, (&(default_element)) ))
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
    {
        switch( t )
        {
            case ((tp_t)(TYPEOF_tp_t)):
            case ((tp_t)(TYPEOF_er_t)):
            case ((tp_t)(TYPEOF_aware_t)):
            {
                if( x_btml_is_identifier(source ) )
                {BLM_INIT_LEVEL(8);
                    (*(((tp_t*)(o)))) = btypeof( ((st_s*)BLM_LEVEL_T_PUSH(8,st_s,x_btml_parse_name(source )))->sc );
                BLM_DOWN();}
                else
                {BLM_INIT_LEVEL(8);
                    BLM_TRY(x_source_parse_fa(source,((st_s*)BLM_LEVEL_T_PUSH(8,st_s,st_s_create_fa(" ##<#<sc_t>*>", x_btml_name_of(t, ((st_s*)BLM_LEVEL_T_PUSH(8,st_s,st_s_create())) ) )))->sc, o ))
                BLM_DOWN();}
            }
            break;
    
            default:
            {BLM_INIT_LEVEL(5);
                BLM_TRY(x_source_parse_fa(source,((st_s*)BLM_LEVEL_T_PUSH(5,st_s,st_s_create_fa(" ##<#<sc_t>*>", x_btml_name_of(t, ((st_s*)BLM_LEVEL_T_PUSH(5,st_s,st_s_create())) ) )))->sc, o ))
            BLM_DOWN();}
            break;
        }
    }
    else if( x_stamp_t_is_pure_array(t ) )
    {
        x_array* arr =((x_array*)( stamp));
        if( x_array_t_is_mutable_mono_typed(t ) )
        {
            tp_t type = 0;
            if( x_source_parse_bl(source,"#?'type:'" ) )
            {
                sr_s sr = sr_null();
                BLM_TRY(x_btml_parse_create_object(source, default_sr, 0, (&(sr)) ))
                type = sr_to_tp( sr );
            }
            x_array_t_set_gtype(arr,t, type );
        }
    
        tp_t mono_type = x_array_t_get_mono_type(arr,t ); // mono type is 0 in case array has no mono type
    
        if( x_array_t_is_fixed(t ) )
        {
            sz_t arr_size = x_array_t_size(arr,t );
            sz_t arr_count = 0;
    
            while( !x_source_parse_bl(source," #=?'</>'" ) )
            {
                sr_s sr = sr_null();
                BLM_TRY(x_btml_parse_create_object(source, default_sr, mono_type, (&(sr)) ))
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
                BLM_TRY(x_btml_parse_create_object(source, default_sr, mono_type, (&(sr)) ))
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
            // non existing member variables are parsed but not assigned
            if( x_stamp_t_exists(t, tp_name ) )
            {
                BLM_TRY(x_btml_parse_create_object(source, default_sr, x_stamp_t_type(stamp,t, tp_name ), (&(sr)) ))
                x_stamp_t_set_sr(stamp,t, tp_name, sr );
            }
            else
            {
                BLM_TRY(x_btml_parse_create_object(source, default_sr, 0, (&(sr)) )) // skip object and remove it
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
    // bcore_x_btml.h:548:1
    
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
    // bcore_x_btml.h:573:1
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
            switch( t )
            {
                case ((tp_t)(TYPEOF_tp_t)):
                case ((tp_t)(TYPEOF_er_t)):
                case ((tp_t)(TYPEOF_aware_t)):
                {
                    sc_t sc_name = bnameof( *( tp_t* )o );
                    if( sc_name )
                    {
                        x_sink_push_fa(sink,"#<sc_t>", sc_name );
                    }
                    else
                    {
                        x_sink_push_st_d(sink,st_s_create_typed(t, o ) );
                    }
                }
                break;
    
                default:
                {
                    x_sink_push_st_d(sink,st_s_create_typed(t, o ) );
                }
                break;
            }
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
    // bcore_x_btml.h:685:1
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
    // bcore_x_btml.h:719:1
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
    BLM_TRY(x_inst_t_copy_typed(((x_inst*)( o)),t, sr_s_o_type( (&(sr)) ), sr.o ))
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
                (*(obj)) = sr_tsm(type, ((x_inst*)bcore_fork(inst)) );
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
    // bcore_x_bcml.h:111:1
    
    sr_s sr = sr_null();
    BLM_TRY(x_bcml_parse_create_object(true, 0, source, (&(sr)) ))
    BLM_TRY(x_inst_t_copy_typed(((x_inst*)( o)),t, sr_s_o_type( (&(sr)) ), sr.o ))
    sr_s_down(&( sr ));
    return  bcore_error_last();
}

x_bcml* x_bcml_create_from_source_t( x_source* source, tp_t* type )
{
    // bcore_x_bcml.h:122:1
    
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
    // bcore_x_bcml.h:137:1
    
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
    // bcore_x_bcml.h:150:1
    
    x_bcml_t_translate_recursive(o,t, true, true, true, sink );
    return sink;
}

bl_t x_bcml_appears_valid( x_source* source )
{
    // bcore_x_bcml.h:167:1
    
    sz_t index = x_source_get_index(source);
    tp_t type = x_bcml_parse_type(source );
    bl_t valid = bcore_flect_exists( type );
    x_source_set_index(source,index );
    return  valid;
}

er_t x_bcml_parse_create_object( bl_t parse_existence, tp_t given_type, x_source* source, sr_s* obj )
{
    // bcore_x_bcml.h:181:1
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
                (*(obj)) = sr_tsm(type, ((x_inst*)bcore_fork(inst)) );
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
    // bcore_x_bcml.h:213:1
    
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
    // bcore_x_bcml.h:240:1
    
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
    // bcore_x_bcml.h:267:1
    
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
    // bcore_x_bcml.h:343:1
    
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
    // bcore_x_bcml.h:367:1
    
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
    // bcore_x_bcml.h:391:1
    
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
    // bcore_x_bcml.h:463:1
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
    // bcore_x_bcml.h:498:1
    BLM_INIT_LEVEL(0);
    sr_s zoo;BLM_T_INIT_SPUSH(sr_s, &zoo);; zoo = bcore_spect_via_create_zoo( 1000 );
    
    clock_t time = clock();
    sz_t size = x_bcml_t_test_transfer(((const x_bcml*)(zoo.o)),sr_s_o_type(&( zoo )) );
    time = clock() - time;
    
    bcore_msg_fa( "x_bcml transfer #<f3_t>s; size: #<sz_t> bytes\n", ( f3_t )time/CLOCKS_PER_SEC, size );
    BLM_DOWN();
}

/**********************************************************************************************************************/
// source: bcore_x_btcl.h
#include "bcore_x_btcl.h"

//----------------------------------------------------------------------------------------------------------------------
// group: x_btcl; embeds: bcore_x_btcl.x bcore_x_btcl_builtin.x bcore_x_btcl_bop.x bcore_x_btcl_export.x bcore_x_btcl_net.x

BCORE_DEFINE_OBJECT_INST_P( x_btcl_btcl_function_stamp_s )
"aware x_btcl"
"{"
    "f3_t additive;"
    "func ^:btcl_function_arity;"
    "func ^:btcl_function;"
"}";

sz_t x_btcl_btcl_function_stamp_s_btcl_function_arity( const x_btcl_btcl_function_stamp_s* o, tp_t name )
{
    // bcore_x_btcl.h:97:5
    
    switch( name )
    {
        case TYPEOF_add_a_b: return  2;
        case TYPEOF_add_a  : return  1;
        default: break;
    }
    return  -1; // return -1 to indicate that a function of given name does not exist
}

er_t x_btcl_btcl_function_stamp_s_btcl_function( const x_btcl_btcl_function_stamp_s* o, tp_t name, const bcore_arr_sr_s* args, sr_s* result )
{
    // bcore_x_btcl.h:108:5
    
    switch( name )
    {
        case TYPEOF_add_a_b: sr_s_from_f3(result,o->additive + sr_s_to_f3(&(args->data[0])) + sr_s_to_f3(&(args->data[1])) ); return  0;
        case TYPEOF_add_a  : sr_s_from_f3(result,o->additive + sr_s_to_f3(&(args->data[0])) ); return  0;
        default: break;
    }
    return  bcore_error_push_fa(TYPEOF_general_error, "Unhandled function." ); // should never occur
}

BCORE_DEFINE_OBJECT_INST_P( x_btcl_context_s )
"aware x_btcl"
"{"
    "bcore_hmap_name_s hmap_reserved_key;"
    "bcore_hmap_name_s hmap_reserved_func;"
    "bcore_hmap_name_s hmap_reserved_const;"
    "bcore_hmap_tp_s constructible_operator;"
"}";

x_btcl_context_s* x_btcl_context_s_setup( x_btcl_context_s* o )
{
    // bcore_x_btcl.x:223:5
    
    x_btcl_context_s_set_reserved_keys(o);
    x_btcl_context_s_set_reserved_funcs(o);
    x_btcl_context_s_set_reserved_consts(o);
    return o;
}

x_btcl_context_s* x_btcl_context_s_set_reserved_keys( x_btcl_context_s* o )
{
    // bcore_x_btcl.x:232:5
    
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_key),"if" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_key),"else" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_key),"self" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_key),"func" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_key),"embed" );
    return o;
}

bl_t x_btcl_context_s_is_reserved( const x_btcl_context_s* o, tp_t name )
{
    // bcore_x_btcl.x:250:5
    
    return  x_btcl_context_s_is_reserved_key(o,name ) ||
    x_btcl_context_s_is_reserved_func(o,name ) ||
    x_btcl_context_s_is_reserved_const(o,name );
}

sc_t x_btcl_context_s_sc_reserved( const x_btcl_context_s* o, tp_t name )
{
    // bcore_x_btcl.x:257:5
    
    if( x_btcl_context_s_is_reserved_key(o,name ) ) return  x_btcl_context_s_sc_reserved_key(o,name );
    if( x_btcl_context_s_is_reserved_func(o,name ) ) return  x_btcl_context_s_sc_reserved_func(o,name );
    if( x_btcl_context_s_is_reserved_const(o,name ) ) return  x_btcl_context_s_sc_reserved_const(o,name );
    return  NULL;
}

er_t x_btcl_context_s_check_reserved( const x_btcl_context_s* o, tp_t name, x_source* source )
{
    // bcore_x_btcl.x:265:5
    
    if( x_btcl_context_s_is_reserved(o,name ) ) return  x_source_parse_error_fa(source,"#<sc_t> is a reserved keyword or function.\n", x_btcl_context_s_sc_reserved(o,name ) );
    return  0;
}

x_btcl_context_s* x_btcl_context_s_set_reserved_funcs( x_btcl_context_s* o )
{
    // bcore_x_btcl_builtin.x:49:1
    
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_func),"SIZE" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_func),"EXP" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_func),"LOG" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_func),"LOG2" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_func),"LOG10" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_func),"SIN" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_func),"COS" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_func),"TAN" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_func),"TANH" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_func),"SIGN" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_func),"SQRT" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_func),"ABS" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_func),"CEIL" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_func),"FLOOR" );
    return o;
}

x_btcl_context_s* x_btcl_context_s_set_reserved_consts( x_btcl_context_s* o )
{
    // bcore_x_btcl_builtin.x:69:1
    
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_const),"true" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_const),"false" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_const),"TRUE" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_const),"FALSE" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_const),"PI" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_const),"PATH" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_const),"DIR" );
    return o;
}

BCORE_DEFINE_OBJECT_INST_P( x_btcl_frame_s )
"aware x_btcl"
"{"
    "private x_btcl_frame_s* parent;"
    "x_btcl_context_s -> context;"
    "sz_t depth;"
    "hidden bcore_arr_sr_s obj_pool;"
    "bcore_hmap_name_s hmap_name;"
    "hidden bcore_hmap_tp_sr_s var_map;"
"}";

x_btcl_frame_s* x_btcl_frame_s_setup_as_root( x_btcl_frame_s* o, x_btcl_context_s* context )
{
    // bcore_x_btcl.x:310:5
    
    x_btcl_frame_s_attach( &(o->parent ),  NULL);
    
    if( context )
    {
        x_btcl_context_s_attach( &(o->context ),  ((x_btcl_context_s*)bcore_fork(context)));
    }
    else
    {
        x_btcl_context_s_attach( &(o->context ),  x_btcl_context_s_setup(x_btcl_context_s_create()));
    }
    return o;
}

x_btcl_frame_s* x_btcl_frame_s_setup( x_btcl_frame_s* o, const x_btcl_frame_s* parent )
{
    // bcore_x_btcl.x:325:5
    
    if( !parent ) ERR_fa( "No parent: Call setup_as_root\n" );
    o->parent = ((x_btcl_frame_s*)(parent));
    x_btcl_context_s_attach( &(o->context ),  ((x_btcl_context_s*)bcore_fork(o->parent->context)));
    o->depth = o->parent->depth + 1;
    return o;
}

x_btcl_frame_s* x_btcl_frame_s_preserve_and_set_weak( x_btcl_frame_s* o, sr_s* sr )
{
    // bcore_x_btcl.x:335:5
    
    bcore_arr_sr_s_push_sr(&(o->obj_pool),*(sr ));
    sr_s_set_strong(sr,false );
    return o;
}

sc_t x_btcl_frame_s_nameof( const x_btcl_frame_s* o, tp_t type )
{
    // bcore_x_btcl.x:344:5
    
    sc_t name = bcore_hmap_name_s_get_sc(&(o->hmap_name),type ); return  name ? name : bnameof( type );
    if( !name && o->parent ) name = x_btcl_frame_s_nameof(o->parent,type );
    if( !name && x_btcl_context_s_is_reserved(o->context,type ) ) name = x_btcl_context_s_sc_reserved(o->context,type );
    if( !name ) name = bnameof( type );
    return  name;
}

bl_t x_btcl_frame_s_var_exists( const x_btcl_frame_s* o, tp_t name )
{
    // bcore_x_btcl.x:355:5
    
    if( bcore_hmap_tp_sr_s_exists(&(o->var_map),name ) ) return  true;
    return  o->parent ? x_btcl_frame_s_var_exists(o->parent,name ) : false;
}

sr_s* x_btcl_frame_s_var_get( const x_btcl_frame_s* o, tp_t name )
{
    // bcore_x_btcl.x:361:5
    
    sr_s* sr = bcore_hmap_tp_sr_s_get(&(o->var_map),name );
    if( sr ) return  sr;
    return  o->parent ? x_btcl_frame_s_var_get(o->parent,name ) : NULL;
}

sr_s* x_btcl_frame_s_var_set( x_btcl_frame_s* o, tp_t name, sr_s sr )
{
    // bcore_x_btcl.x:368:5
    
    return  bcore_hmap_tp_sr_s_set(&(o->var_map),name, sr );
}

tp_t x_btcl_frame_s_get_identifier( x_btcl_frame_s* o, x_source* source, bl_t take_from_source )
{
    // bcore_x_btcl.x:784:1
    
    tp_t tp_identifier = 0;
    if( x_btcl_is_identifier(source ) )
    {BLM_INIT_LEVEL(1);
        sz_t source_index = take_from_source ? 0 : x_source_get_index(source);
        st_s* st_name = ((st_s*)BLM_LEVEL_T_PUSH(1,st_s,st_s_create()));
        BLM_TRY_EXIT(x_source_parse_fa(source," #name", st_name ))
        tp_identifier = x_btcl_frame_s_entypeof(o,st_name->sc );
        if( !take_from_source ) x_source_set_index(source,source_index );
    BLM_DOWN();}
    
    return  tp_identifier;
}

er_t x_btcl_frame_s_eval_number_literal( x_btcl_frame_s* o, x_source* source, sr_s* sr )
{
    // bcore_x_btcl.x:802:1
    BLM_INIT_LEVEL(0);
    bl_t is_hex = false;
    bl_t is_float = false;
    st_s st;BLM_T_INIT_SPUSH(st_s, &st);;
    
    if( x_source_parse_bl(source,"#?'0x'" ) )
    {
        st_s_push_sc(&(st),"0x" );
        is_hex = true;
    }
    else if( x_source_parse_bl(source,"#?'0X'" ) )
    {
        st_s_push_sc(&(st),"0X" );
        is_hex = true;
    }
    
    if( is_hex )
    {
        while( x_source_parse_bl(source,"#?(([0]>='0'&&[0]<='9')||([0]>='a'&&[0]<='f')||([0]>='A'&&[0]<='F'))" ) )
        {
            st_s_push_char(&(st),x_source_get_char(source) );
        }
    }
    else // decimal
    {
        while( x_source_parse_bl(source,"#?([0]>='0'&&[0]<='9')" ) ) st_s_push_char(&(st),x_source_get_char(source) );
    }
    
    if( !is_hex && x_source_parse_bl(source,"#?([0]=='.')" ) )
    {
        st_s_push_char(&(st),x_source_get_char(source) );
        while( x_source_parse_bl(source,"#?([0]>='0'&&[0]<='9')" ) ) st_s_push_char(&(st),x_source_get_char(source) );
        is_float = true;
    }
    
    bl_t exponent = false;
    
    if( !is_hex && x_source_parse_bl(source,"#?([0]=='e'||[0]=='E')" ) )
    {
        st_s_push_char(&(st),x_source_get_char(source) );
        exponent = true;
        is_float = true;
    }
    
    if( exponent )
    {
        if( x_source_parse_bl(source,"#?([0]=='+'||[0]=='-')" ) )
        {
            st_s_push_char(&(st),x_source_get_char(source) );
        }
    
        while( x_source_parse_bl(source,"#?([0]>='0'&&[0]<='9')" ) )
        {
            st_s_push_char(&(st),x_source_get_char(source) );
        }
    }
    
    if( is_float )
    {
        f3_t f3 = 0;
        st_s_parse_fa(&(st),0, -1, "#<f3_t*>", (&(f3)) );
        sr_s_const_from_f3(sr,f3 );
    }
    else
    {
        s3_t s3 = 0;
        st_s_parse_fa(&(st),0, -1, "#<s3_t*>", (&(s3)) );
        sr_s_const_from_s3(sr,s3 );
    }
    
    BLM_RETURNV(er_t, 0)
}

er_t x_btcl_frame_s_eval_condition( x_btcl_frame_s* o, x_source* source, bl_t* condition )
{
    // bcore_x_btcl.x:879:1
    BLM_INIT_LEVEL(0);
    sr_s* sb = ((sr_s*)BLM_LEVEL_T_PUSH(0,sr_s,sr_s_create()));
    BLM_TRY(x_btcl_frame_s_eval(o,0, source, sb ))
    if( !sr_s_is_numeric(sb) )  BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Internal error: Expression does not evaluate to a condition.\n" ))
    if( condition ) (*(condition)) = ( sr_s_to_f3(sb) != 0 );
    BLM_RETURNV(er_t, 0)
}

er_t x_btcl_frame_s_negate( x_btcl_frame_s* o, x_source* source, sr_s* sr )
{
    // bcore_x_btcl.x:895:1
    
    if( sr_s_is_integer(sr) )
    {
        sr_s_const_from_s3(sr,-sr_s_to_s3(sr) );
    }
    else if( sr_s_is_float(sr) )
    {
        sr_s_const_from_f3(sr,-sr_s_to_f3(sr) );
    }
    else
    {
        return  x_source_parse_error_fa(source,"Negation of #<sc_t> is not defined.\n", bnameof( sr_s_o_type(sr) ) );
    }
    
    return  0;
}

er_t x_btcl_frame_s_logic_not( x_btcl_frame_s* o, x_source* source, sr_s* sr )
{
    // bcore_x_btcl.x:915:1
    
    if( sr_s_is_numeric(sr) )
    {
        sr_s_const_from_bl(sr,!sr_s_to_bl(sr) );
    }
    else
    {
        return  x_source_parse_error_fa(source,"Negation of #<sc_t> is not defined.\n", bnameof( sr_s_o_type(sr) ) );
    }
    return  0;
}

er_t x_btcl_frame_s_eval_in_frame( const x_btcl_frame_s* o, s2_t priority, x_source* source, sr_s* obj )
{
    // bcore_x_btcl.x:957:1
    BLM_INIT_LEVEL(0);
    x_btcl_frame_s* frame = x_btcl_frame_s_setup(((x_btcl_frame_s*)BLM_LEVEL_T_PUSH(0,x_btcl_frame_s,x_btcl_frame_s_create())),o );
    BLM_TRY(x_btcl_frame_s_eval(frame,priority, source, obj ))
    x_btcl_clone_if_weak(obj );
    BLM_RETURNV(er_t, 0)
}

er_t x_btcl_frame_s_eval( x_btcl_frame_s* o, s2_t exit_priority, x_source* source, sr_s* obj )
{
    // bcore_x_btcl.x:967:1
    
    ASSERT( obj->o == NULL );
    
    /// prefix operators
    if     ( x_source_parse_bl(source," #?'+'" ) ) BLM_TRY(x_btcl_frame_s_eval_export_uop_type(o,((tp_t)(TYPEOF_identity)), x_btcl_priority_c(), source, obj ))
    else if( x_source_parse_bl(source," #?'-'" ) ) BLM_TRY(x_btcl_frame_s_eval_export_uop_type(o,((tp_t)(TYPEOF_neg)),      x_btcl_priority_c(), source, obj ))
    else if( x_source_parse_bl(source," #?'!'" ) ) BLM_TRY(x_btcl_frame_s_eval_export_uop_type(o,((tp_t)(TYPEOF_not)),      x_btcl_priority_c(), source, obj ))
    else if( x_source_parse_bl(source," #?'?'" ) )
    {
        bl_t detail = x_source_parse_bl(source," #?'?'" );
        BLM_TRY(x_btcl_frame_s_eval(o,x_btcl_priority_c(), source, obj ))
        BLM_TRY(x_btcl_to_sink(detail, obj, x_sink_stdout() ))
        x_sink_flush(x_sink_stdout());
    }
    
    /// number literal
    else if( x_source_parse_bl(source," #?([0]>='0'&&[0]<='9')" ) )
    {
        BLM_TRY(x_btcl_frame_s_eval_number_literal(o,source, obj ))
    }
    
    /// string literal
    else if( x_source_parse_bl(source," #=?'\"'" ) )
    {
        BLM_TRY(x_source_parse_fa(source," #cstring", ((st_s*)(sr_s_asm(obj,st_s_create() )->o)) ))
    }
    
    /// Identifier
    else if( x_btcl_is_identifier(source ) )
    {
        tp_t name = x_btcl_frame_s_get_identifier(o,source, true );
    
        /// reserved keyword
        if( x_btcl_frame_s_is_reserved_key(o,name ) )
        {
            switch( name )
            {
                case TYPEOF_else: return  x_source_parse_error_fa(source,"Misplaced 'else'.\n" );
    
                case TYPEOF_if:
                {
                    bl_t condition = false;
                    BLM_TRY(x_source_parse_fa(source," (" ))
                    BLM_TRY(x_btcl_frame_s_eval_condition(o,source,&( condition )))
                    BLM_TRY(x_source_parse_fa(source," )" ))
                    {BLM_INIT_LEVEL(7);
                        x_btcl_block_s* block = ((x_btcl_block_s*)BLM_LEVEL_T_PUSH(7,x_btcl_block_s,x_btcl_block_s_create()));
                        BLM_TRY(x_btcl_block_s_parse(block,o, source ))
                        if( condition ) BLM_TRY(x_btcl_block_s_eval(block,o, obj ))
                    BLM_DOWN();}
                    if( x_source_parse_bl(source," #?w'else'" ) )
                    {BLM_INIT_LEVEL(7);
                        x_btcl_block_s* block = ((x_btcl_block_s*)BLM_LEVEL_T_PUSH(7,x_btcl_block_s,x_btcl_block_s_create()));
                        BLM_TRY(x_btcl_block_s_parse(block,o, source ))
                        if( !condition ) BLM_TRY(x_btcl_block_s_eval(block,o, obj ))
                    BLM_DOWN();}
                }
                break;
    
                /// Signature or Function definition
                case TYPEOF_func:
                {BLM_INIT_LEVEL(6);
                    x_btcl_signature_s* signature = ((x_btcl_signature_s*)BLM_LEVEL_T_PUSH(6,x_btcl_signature_s,x_btcl_signature_s_create()));
                    BLM_TRY(x_btcl_signature_s_parse(signature,o, source ))
    
                    if( x_source_parse_bl(source," #=?'{'" ) ) // block follows signature -> function
                    {BLM_INIT_LEVEL(7);
                        x_btcl_block_s* block = ((x_btcl_block_s*)BLM_LEVEL_T_PUSH(7,x_btcl_block_s,x_btcl_block_s_create()));
                        BLM_TRY(x_btcl_block_s_parse(block,o, source ))
    
                        x_btcl_function_s* function = x_btcl_function_s_setup(((x_btcl_function_s*)BLM_LEVEL_T_PUSH(7,x_btcl_function_s,x_btcl_function_s_create())),signature, block, NULL );
    
                        sr_s_asm(obj,((x_btcl_function_s*)bcore_fork(function)) );
                    BLM_DOWN();}
                    else
                    {
                        sr_s_asm(obj,((x_btcl_signature_s*)bcore_fork(signature)) );
                    }
                BLM_DOWN();}
                break;
    
                /// Recursion
                case TYPEOF_self:
                {
                    if( x_btcl_frame_s_var_exists(o,TYPEOF_self ) )
                    {
                        sr_s* sr = x_btcl_frame_s_var_get(o,name );
                        if( sr_s_o_type(sr) !=  ((tp_t)(TYPEOF_x_btcl_function_s)) ) return  x_source_parse_error_fa(source,"Internal error: Keyword 'self': Not linked to a function.\n" );
    
                        // we fork the object so it can survive beyond a local frame
                        sr_s_tsc(obj,sr_s_o_type(sr), bcore_fork( sr->o ) );
                    }
                    else
                    {
                        return  x_source_parse_error_fa(source,"Keyword 'self': Used outside a function.\n" );
                    }
                }
                break;
    
                /// Embedding
                case TYPEOF_embed:
                {BLM_INIT_LEVEL(6);
                    sr_s* sb = ((sr_s*)BLM_LEVEL_T_PUSH(6,sr_s,sr_s_create()));
                    BLM_TRY(x_source_parse_fa(source," (" ))
                    BLM_TRY(x_btcl_frame_s_eval(o,0, source, sb ))
                    BLM_TRY(x_source_parse_fa(source," )" ))
                    if( sr_s_type(sb) != ((tp_t)(TYPEOF_st_s)) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Keyword 'embed': Expression must evaluate to a string.\n" ))
                    st_s* path = ((st_s*)BLM_LEVEL_T_PUSH(6,st_s,st_s_create()));
                    BLM_TRY(x_btcl_get_embedding_file_path(source, ((const st_s*)(sb->o))->sc, path ))
                    x_source* emb_source =((x_source*)( ((bcore_source*)BLM_LEVEL_A_PUSH(6,bcore_file_open_source(path->sc )))));
                    BLM_TRY(x_btcl_frame_s_eval(o,0, emb_source, obj ))
                BLM_DOWN();}
                break;
    
                default: return  x_source_parse_error_fa(source,"Internal error: Keyword '#<sc_t>': missing implementation.\n", x_btcl_frame_s_sc_reserved(o,name ) );
            }
        }
        else if( x_btcl_frame_s_is_reserved_const(o,name ) )
        {
            BLM_TRY(x_btcl_frame_s_eval_reserved_const(o,name, source, obj ))
        }
        else if( x_btcl_frame_s_is_reserved_func(o,name ) )
        {
            BLM_TRY(x_source_parse_fa(source," (" ))
            BLM_TRY(x_btcl_frame_s_eval_reserved_func(o,name, source, obj ))
            BLM_TRY(x_source_parse_fa(source," )" ))
        }
        else if( x_source_parse_bl(source," #?([0]=='='&&[1]!='=')" ) ) // identifier with assignment --> variable declaration
        {
            sr_s_asm(obj,x_btcl_null_variable_s__(x_btcl_null_variable_s_create(),name ) );
        }
        else if( x_btcl_frame_s_var_exists(o,name ) )
        {
            sr_s* sr = x_btcl_frame_s_var_get(o,name );
    
            // we fork the object so it can survive beyond a local frame
            sr_s_tsc(obj,sr_s_o_type(sr), bcore_fork( sr->o ) );
        }
        else
        {
            return  x_source_parse_error_fa(source,"Unknown identifier #<sc_t>.\n", x_btcl_frame_s_nameof(o,name ) );
        }
    }
    
    /// array literal
    else if( x_source_parse_bl(source," #?'['" ) )
    {BLM_INIT_LEVEL(2);
        x_btcl_list_s* list = ((x_btcl_list_s*)BLM_LEVEL_T_PUSH(2,x_btcl_list_s,x_btcl_list_s_create()));
        for(sz_t i = 0; !x_source_eos(source) && !x_source_parse_bl(source," #=?']'" ); i++ )
        {BLM_INIT_LEVEL(4);
            if( i > 0 )
            {
                BLM_TRY(x_source_parse_fa(source," ," ))
                // a trailing comma is allowed on a non-empty list
                if( x_source_parse_bl(source," #=?']'" ) ) BLM_BREAK_LEVEL(3);
            }
    
            sr_s* sr = ((sr_s*)BLM_LEVEL_T_PUSH(4,sr_s,sr_s_create()));
            BLM_TRY(x_btcl_frame_s_eval(o,0, source, sr ))
            if( sr_s_is_strong(sr) )
            {
                x_btcl_list_s_push_fork(list,sr );
            }
            else
            {
                x_btcl_list_s_push_clone(list,sr );
            }
        BLM_DOWN();}
        BLM_TRY(x_source_parse_fa(source," ]" ))
    
        sr_s_tsc(obj,((tp_t)(TYPEOF_x_btcl_list_s)), ((x_btcl_list_s*)bcore_fork(list)) );
    BLM_DOWN();}
    
    /// BTML object
    else if( x_source_parse_bl(source," #=?'<'" ) )
    {
        tp_t o_type = 0;
        x_inst* o_inst =((x_inst*)( x_btml_create_from_source_t(source, (&(o_type)) )));
        if( !o_inst ) return  TYPEOF_parse_error; // if o_inst == 0 the error stack holds a descriptive message from x_btml_create_from_source_t
        (*(obj)) = sr_tsc(o_type, o_inst );
    }
    
    /// Bracket: content evaluated in dedicated frame
    else if( x_source_parse_bl(source," #?'('" ) )
    {
        BLM_TRY(x_btcl_frame_s_eval_in_frame(o,0, source, obj ))
        BLM_TRY(x_source_parse_fa(source," )" ))
    }
    
    /// Block { .... } definition
    else if( x_source_parse_bl(source," #=?'{'" ) )
    {BLM_INIT_LEVEL(2);
        x_btcl_block_s* block = ((x_btcl_block_s*)BLM_LEVEL_T_PUSH(2,x_btcl_block_s,x_btcl_block_s_create()));
        BLM_TRY(x_btcl_block_s_parse(block,o, source ))
        sr_s_asm(obj,((x_btcl_block_s*)bcore_fork(block)) );
    BLM_DOWN();}
    
    /// Network object
    else if( x_source_parse_bl(source," #?'&'" ) )
    {BLM_INIT_LEVEL(2);
        const x_source_point_s* sp = x_source_point_s_setup_from_source(((x_source_point_s*)BLM_LEVEL_T_PUSH(2,x_source_point_s,x_source_point_s_create())),source );
        if( x_source_parse_bl(source," #?'-'" ) ) // wire
        {
            if( !x_btcl_is_identifier(source ) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Rack identifier expected.\n" ))
            tp_t rack_name = bcore_name_enroll(x_btcl_frame_s_nameof(o,x_btcl_frame_s_get_identifier(o,source, true ) ) );
            tp_t wire_name = 0;
            if( x_source_parse_bl(source," #?'.' " ) )
            {
                if( !x_btcl_is_identifier(source ) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Branch identifier expected.\n" ))
                wire_name = bcore_name_enroll(x_btcl_frame_s_nameof(o,x_btcl_frame_s_get_identifier(o,source, true ) ) );
            }
            sr_s_asm(obj,x_btcl_net_node_s_setup_wire(x_btcl_net_node_s_create(),rack_name, wire_name, sp ) );
        }
        else
        {
            tp_t node_type = 0; // generic
            if( x_source_parse_bl(source," #?':'" ) ) node_type = ((tp_t)(TYPEOF_rack));
            if( !x_btcl_is_identifier(source ) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Identifier expected.\n" ))
            tp_t node_name = bcore_name_enroll(x_btcl_frame_s_nameof(o,x_btcl_frame_s_get_identifier(o,source, true ) ) );
            sr_s_asm(obj,x_btcl_net_node_s_setup(x_btcl_net_node_s_create(),node_type, node_name, sp ) );
        }
    BLM_DOWN();}
    
    else
    {
        return  x_source_parse_error_fa(source,"Expression does not evaluate to an object.\n" );
    }
    
    // binary operators if any
    BLM_TRY(x_btcl_frame_s_eval_bop(o,exit_priority, source, obj ))
    
    return  0;
}

er_t x_btcl_frame_s_parse_create_object( x_btcl_frame_s* o, x_source* source, sr_s* obj )
{
    // bcore_x_btcl.x:1210:1
    BLM_INIT_LEVEL(0);
    sr_s* sr = ((sr_s*)BLM_LEVEL_T_PUSH(0,sr_s,sr_s_create()));
    BLM_TRY(x_btcl_frame_s_eval(o,0, source, sr ))
    
    if( obj )
    {
        if( sr->o )
        {
            if( sr_s_is_strong(sr) )
            {
                sr_s_fork_from(obj,sr );
            }
            else
            {
                /** A weak reference might be an embedded member of an object.
                 *  We clone it here to be sure we don't get runtime issues.
                 *  If the object is not embedded, it could be forked (s. above)
                 */
                sr_s_clone_from(obj,sr );
            }
        }
        else
        {
            sr_s_clear(obj);
        }
    }
    BLM_RETURNV(er_t, 0)
}

er_t x_btcl_frame_s_eval_reserved_const( x_btcl_frame_s* o, tp_t name, x_source* source, sr_s* sr )
{
    // bcore_x_btcl_builtin.x:82:1
    
    switch( name )
    {
        case TYPEOF_true : sr_s_const_from_bl(sr,true ); break;
        case TYPEOF_false: sr_s_const_from_bl(sr,false ); break;
        case TYPEOF_TRUE : sr_s_const_from_bl(sr,true ); break;
        case TYPEOF_FALSE: sr_s_const_from_bl(sr,false ); break;
        case TYPEOF_PI:    sr_s_const_from_f3(sr,3.1415926535897932384626434 ); break;
    
        case TYPEOF_PATH:
        {
            sc_t path = x_source_get_file(source);
            if( !path )
            {
                return  x_source_parse_error_fa(source,"Cannot retrieve a file path from source.\n", x_btcl_frame_s_nameof(o,name ) );
            }
            sr_s_asc(sr,st_s_copy_sc(st_s_create(),path ) );
        }
        break;
    
        case TYPEOF_DIR:
        {BLM_INIT_LEVEL(3);
            st_s* dir = ((st_s*)BLM_LEVEL_T_PUSH(3,st_s,x_source_get_d_dir(source)));
            if( !dir )
            {
                BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Cannot retrieve a directory from source.\n", x_btcl_frame_s_nameof(o,name ) ))
            }
            sr_s_asc(sr,((st_s*)bcore_fork(dir)) );
        BLM_DOWN();}
        break;
    
        default:
        {
            return  x_source_parse_error_fa(source,"Unknown reserved constant.\n", x_btcl_frame_s_nameof(o,name ) );
        }
        break;
    }
    
    return  0;
}

er_t x_btcl_frame_s_eval_reserved_func( x_btcl_frame_s* o, tp_t name, x_source* source, sr_s* sr )
{
    // bcore_x_btcl_builtin.x:126:1
    BLM_INIT_LEVEL(0);
    sr_s* sb = ((sr_s*)BLM_LEVEL_T_PUSH(0,sr_s,sr_s_create()));
    (*(sb)) = (*(sr));
    (*(sr)) = sr_null();
    
    if( !sb->o ) BLM_TRY(x_btcl_frame_s_eval(o,0, source, sb ))
    
    switch( name )
    {
        case ((tp_t)(TYPEOF_SIZE)):
        {
            if( sr_s_o_type(sb) == ((tp_t)(TYPEOF_x_btcl_list_s)) )
            {
                sr_s_const_from_s3(sr,((const x_btcl_list_s*)(sb->o))->arr.size );
            }
            else if( x_array_t_is_array(sr_s_o_type(sb) ) )
            {
                sr_s_const_from_s3(sr,x_array_t_size(((const x_array*)( sb->o)),sr_s_o_type(sb) ) );
            }
            else
            {
                BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Function #<sc_t>: #<sc_t> is no array.\n", x_btcl_frame_s_nameof(o,name ), bnameof( sr_s_o_type(sb) ) ))
            }
        }
        break;
    
        case ((tp_t)(TYPEOF_EXP)):
        {
            if( sr_s_is_numeric(sb) )
            {
                sr_s_const_from_f3(sr,exp( sr_s_to_f3(sb) ) );
            }
            else
            {
                BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Function #<sc_t>: Argument '#<sc_t>' is not numeric.\n", x_btcl_frame_s_nameof(o,name ), bnameof( sr_s_o_type(sb) ) ))
            }
        }
        break;
    
        case ((tp_t)(TYPEOF_LOG)):
        {
            if( sr_s_is_numeric(sb) )
            {
                sr_s_const_from_f3(sr,log( sr_s_to_f3(sb) ) );
            }
            else
            {
                BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Function #<sc_t>: Argument '#<sc_t>' is not numeric.\n", x_btcl_frame_s_nameof(o,name ), bnameof( sr_s_o_type(sb) ) ))
            }
        }
        break;
    
        case ((tp_t)(TYPEOF_LOG2)):
        {
            if( sr_s_is_numeric(sb) )
            {
                sr_s_const_from_f3(sr,log2( sr_s_to_f3(sb) ) );
            }
            else
            {
                BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Function #<sc_t>: Argument '#<sc_t>' is not numeric.\n", x_btcl_frame_s_nameof(o,name ), bnameof( sr_s_o_type(sb) ) ))
            }
        }
        break;
    
        case ((tp_t)(TYPEOF_LOG10)):
        {
            if( sr_s_is_numeric(sb) )
            {
                sr_s_const_from_f3(sr,log10( sr_s_to_f3(sb) ) );
            }
            else
            {
                BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Function #<sc_t>: Argument '#<sc_t>' is not numeric.\n", x_btcl_frame_s_nameof(o,name ), bnameof( sr_s_o_type(sb) ) ))
            }
        }
        break;
    
        case ((tp_t)(TYPEOF_SIN)):
        {
            if( sr_s_is_numeric(sb) )
            {
                sr_s_const_from_f3(sr,sin( sr_s_to_f3(sb) ) );
            }
            else
            {
                BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Function #<sc_t>: Argument '#<sc_t>' is not numeric.\n", x_btcl_frame_s_nameof(o,name ), bnameof( sr_s_o_type(sb) ) ))
            }
        }
        break;
    
        case ((tp_t)(TYPEOF_COS)):
        {
            if( sr_s_is_numeric(sb) )
            {
                sr_s_const_from_f3(sr,cos( sr_s_to_f3(sb) ) );
            }
            else
            {
                BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Function #<sc_t>: Argument '#<sc_t>' is not numeric.\n", x_btcl_frame_s_nameof(o,name ), bnameof( sr_s_o_type(sb) ) ))
            }
        }
        break;
    
        case ((tp_t)(TYPEOF_TAN)):
        {
            if( sr_s_is_numeric(sb) )
            {
                sr_s_const_from_f3(sr,tan( sr_s_to_f3(sb) ) );
            }
            else
            {
                BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Function #<sc_t>: Argument '#<sc_t>' is not numeric.\n", x_btcl_frame_s_nameof(o,name ), bnameof( sr_s_o_type(sb) ) ))
            }
        }
        break;
    
        case ((tp_t)(TYPEOF_TANH)):
        {
            if( sr_s_is_numeric(sb) )
            {
                sr_s_const_from_f3(sr,tanh( sr_s_to_f3(sb) ) );
            }
            else
            {
                BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Function #<sc_t>: Argument '#<sc_t>' is not numeric.\n", x_btcl_frame_s_nameof(o,name ), bnameof( sr_s_o_type(sb) ) ))
            }
        }
        break;
    
        case ((tp_t)(TYPEOF_SIGN)):
        {
            if( sr_s_is_numeric(sb) )
            {
                sr_s_const_from_f3(sr,( sr_s_to_f3(sb) >= 0 ) ? 1 : -1 );
            }
            else
            {
                BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Function #<sc_t>: Argument '#<sc_t>' is not numeric.\n", x_btcl_frame_s_nameof(o,name ), bnameof( sr_s_o_type(sb) ) ))
            }
        }
        break;
    
        case ((tp_t)(TYPEOF_SQRT)):
        {
            if( sr_s_is_numeric(sb) )
            {
                sr_s_const_from_f3(sr,sqrt( sr_s_to_f3(sb) ) );
            }
            else
            {
                BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Function #<sc_t>: Argument '#<sc_t>' is not numeric.\n", x_btcl_frame_s_nameof(o,name ), bnameof( sr_s_o_type(sb) ) ))
            }
        }
        break;
    
        case ((tp_t)(TYPEOF_ABS)):
        {
            if( sr_s_is_numeric(sb) )
            {
                if( sr_s_is_integer(sb) )
                {
                    s3_t x = sr_s_to_s3(sb);
                    sr_s_const_from_s3(sr,( x >= 0 ) ? x : -x );
                }
                else
                {
                    f3_t x = sr_s_to_f3(sb);
                    sr_s_const_from_f3(sr,( x >= 0 ) ? x : -x );
                }
            }
            else
            {
                BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Function #<sc_t>: Argument '#<sc_t>' is not numeric.\n", x_btcl_frame_s_nameof(o,name ), bnameof( sr_s_o_type(sb) ) ))
            }
        }
        break;
    
        case ((tp_t)(TYPEOF_CEIL)):
        {
            if( sr_s_is_numeric(sb) )
            {
                sr_s_const_from_f3(sr,ceil( sr_s_to_f3(sb) ) );
            }
            else
            {
                BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Function #<sc_t>: Argument '#<sc_t>' is not numeric.\n", x_btcl_frame_s_nameof(o,name ), bnameof( sr_s_o_type(sb) ) ))
            }
        }
        break;
    
        case ((tp_t)(TYPEOF_FLOOR)):
        {
            if( sr_s_is_numeric(sb) )
            {
                sr_s_const_from_f3(sr,floor( sr_s_to_f3(sb) ) );
            }
            else
            {
                BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Function #<sc_t>: Argument '#<sc_t>' is not numeric.\n", x_btcl_frame_s_nameof(o,name ), bnameof( sr_s_o_type(sb) ) ))
            }
        }
        break;
    
        default:
        {
            BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Unknown reserved function.\n", x_btcl_frame_s_nameof(o,name ) ))
        }
        break;
    }
    
    BLM_RETURNV(er_t, 0)
}

er_t x_btcl_frame_s_eval_bop_member( x_btcl_frame_s* o, x_source* source, sr_s* sr )
{
    // bcore_x_btcl_bop.x:23:1
    
    if( sr_s_type(sr) == ((tp_t)(TYPEOF_x_btcl_net_node_s)) ) return  x_btcl_frame_s_eval_net_node_member(o,source, sr );
    
    bl_t is_const = sr_s_is_const(sr);
    
    /// Identifier
    if( x_source_parse_bl(source," #?(([0]>='A'&&[0]<='Z')||([0]>='a'&&[0]<='z')||[0]=='_')" ) )
    {
        tp_t name = x_btcl_frame_s_get_identifier(o,source, true );
    
        if( x_btcl_frame_s_is_reserved_func(o,name ) )
        {
            BLM_TRY(x_source_parse_fa(source," (" ))
            BLM_TRY(x_btcl_frame_s_eval_reserved_func(o,name, source, sr ))
            BLM_TRY(x_source_parse_fa(source," )" ))
        }
        else if( x_stamp_t_exists(sr_s_o_type(sr), name ) )
        {
            if( sr_s_is_strong(sr) ) x_btcl_frame_s_preserve_and_set_weak(o,sr );
            sr_s sc = x_stamp_t_m_get_sr(((x_stamp*)( sr->o)),sr_s_o_type(sr), name );
    
            if( sc.o )
            {
                (*(sr)) = sc;
            }
            else if( x_source_parse_bl(source," #?([0]=='='&&[1]!='=')" ) )  // identifier with assignment
            {
                sr_s_asm(sr,x_btcl_null_member_s_setup(x_btcl_null_member_s_create(),sr, name ) );
            }
            else
            {
                return  x_source_parse_error_fa(source,"#<sc_t>.#<sc_t> is NULL (use '=' to assign a value).\n", bnameof( sr_s_o_type(sr) ), x_btcl_frame_s_nameof(o,name ) );
            }
        }
        else if( x_btcl_a_btcl_function_arity(((const x_btcl*)(sr->o)),name ) >= 0 )
        {
            s2_t arity = x_btcl_a_btcl_function_arity(((const x_btcl*)(sr->o)),name );
            sr_s_asm(sr,x_btcl_function_s_setup_external_function(x_btcl_function_s_create(),name, arity, ((const x_btcl*)(sr->o)) ) );
        }
        else
        {
            return  x_source_parse_error_fa(source,"#<sc_t>.#<sc_t> does not exist.\n", bnameof( sr_s_o_type(sr) ), x_btcl_frame_s_nameof(o,name ) );
        }
    }
    else if( x_source_parse_bl(source," #?'['" ) ) // array index
    {BLM_INIT_LEVEL(2);
    
        sr_s* sb = ((sr_s*)BLM_LEVEL_T_PUSH(2,sr_s,sr_s_create()));
        BLM_TRY(x_btcl_frame_s_eval(o,0, source, sb ))
        BLM_TRY(x_source_parse_fa(source," ]" ))
        if( sr_s_is_numeric(sb) )
        {
            if( sr_s_is_strong(sr) ) x_btcl_frame_s_preserve_and_set_weak(o,sr );
            s3_t index = sr_s_to_s3(sb);
    
            s3_t size = 0;
            sr_s sc = sr_null();
    
            if( x_array_t_is_array(sr_s_o_type(sr) ) )
            {
                size = x_array_t_size(((const x_array*)( sr->o)),sr_s_o_type(sr) );
                if( index <     0 ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Array index '<s3_t>' < 0.\n", index ))
                if( index >= size ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Array index '<s3_t>' >= array size '<s3_t>'.\n", index, size ))
                sc = x_array_t_m_get_sr(((x_array*)( sr->o)),sr_s_o_type(sr), index );
            }
            else if( sr_s_o_type(sr) == ((tp_t)(TYPEOF_x_btcl_list_s)) )
            {
                const x_btcl_list_s* list = ((const x_btcl_list_s*)(sr->o));
                size = list->arr.size;
                if( index <     0 ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Array index '<s3_t>' < 0.\n", index ))
                if( index >= size ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Array index '<s3_t>' >= array size '<s3_t>'.\n", index, size ))
                sc = sr_cw( list->arr.data[ index ] );
                is_const = true;
            }
            else
            {
                BLM_RETURNV(er_t, x_source_parse_error_fa(source,"#<sc_t> is no array or list.\n", bnameof( sr_s_o_type(sr) ) ))
            }
    
            if( sc.o )
            {
                (*(sr)) = sc;
            }
            else if( x_source_parse_bl(source," #?([0]=='='&&[1]!='=')" ) )  // identifier with assignment
            {
                sr_s_asm(sr,x_btcl_null_arr_element_s_setup(x_btcl_null_arr_element_s_create(),sr, index ) );
            }
            else
            {
                BLM_RETURNV(er_t, x_source_parse_error_fa(source,"#<sc_t>.[#<s3_t>] is NULL (use '=' to assign a value).\n", bnameof( sr_s_o_type(sr) ), index ))
            }
        }
        else
        {
            BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Numeric index expected.\n" ))
        }
    BLM_DOWN();}
    else
    {
        return  x_source_parse_error_fa(source,"Member name '<literal>' or subscript '[<expr>]' expected." );
    }
    
    sr_s_set_const(sr,is_const );
    
    return  0;
}

er_t x_btcl_frame_s_eval_bop_functional( x_btcl_frame_s* o, x_source* source, sr_s* sr )
{
    // bcore_x_btcl_bop.x:133:1
    
    if( sr_s_o_type(sr) == ((tp_t)(TYPEOF_x_btcl_function_s)) )
    {BLM_INIT_LEVEL(1);
        BLM_TRY(x_btcl_function_s_call_via_evaluation(((x_btcl_function_s*)BLM_LEVEL_T_PUSH(1,x_btcl_function_s,((x_btcl_function_s*)bcore_fork(((x_btcl_function_s*)(sr->o)))))),source, o, sr ))
    BLM_DOWN();}
    else if( sr_s_o_type(sr) == ((tp_t)(TYPEOF_x_btcl_block_s)) )
    {
        return  x_source_parse_error_fa(source,"Attempt to evaluate a block as function. Join the block with a signature to create a function.\n" );
    }
    else if( sr_s_o_type(sr) == ((tp_t)(TYPEOF_x_btcl_net_node_s)) )
    {BLM_INIT_LEVEL(2);
        BLM_TRY(x_btcl_frame_s_eval_net_node_modifier(x_btcl_frame_s_setup(((x_btcl_frame_s*)BLM_LEVEL_T_PUSH(2,x_btcl_frame_s,x_btcl_frame_s_create())),o ),source, sr ))
        x_btcl_clone_if_weak(sr );
    BLM_DOWN();}
    else
    {BLM_INIT_LEVEL(1);
        BLM_TRY(x_btcl_frame_s_eval_bop_modifier(x_btcl_frame_s_setup(((x_btcl_frame_s*)BLM_LEVEL_T_PUSH(1,x_btcl_frame_s,x_btcl_frame_s_create())),o ),source, sr ))
        x_btcl_clone_if_weak(sr );
    BLM_DOWN();}
    return  0;
}

er_t x_btcl_frame_s_eval_bop_modifier( x_btcl_frame_s* o, x_source* source, sr_s* sr )
{
    // bcore_x_btcl_bop.x:158:1
    BLM_INIT_LEVEL(0);
    (*(sr)) = sr_clone( (*(sr)) );
    sr_s sr_weak;BLM_T_INIT_SPUSH(sr_s, &sr_weak);;
    sr_weak = sr_cw( (*(sr)) );
    if( x_source_parse_bl(source," #=?'.'") )
    {
        BLM_TRY(x_btcl_frame_s_eval_bop(o,0, source,&( sr_weak )))
    }
    else
    {BLM_INIT_LEVEL(1);
        sr_s* sb = ((sr_s*)BLM_LEVEL_T_PUSH(1,sr_s,sr_s_create()));
        BLM_TRY(x_btcl_frame_s_eval(o,0, source, sb ))
        er_t err = x_btcl_generic_copy(sr, sb );
        if( err ) {BLM_INIT_LEVEL(2); BLM_RETURNV(er_t, x_source_parse_error_fa(source,"#<sc_t>\n", bcore_error_pop_all_to_st(((st_s*)BLM_LEVEL_T_PUSH(2,st_s,st_s_create())) )->sc )) BLM_DOWN();}
    BLM_DOWN();}
    
    x_stamp_t_mutated(((x_stamp*)(sr->o)),sr_s_type(sr) );
    
    if( x_source_parse_bl(source," #?','" ) ) BLM_RETURNV(er_t, x_btcl_frame_s_eval_bop_modifier(o,source, sr ))
    BLM_RETURNV(er_t, 0)
}

er_t x_btcl_frame_s_eval_bop_func_list_unfold( x_btcl_frame_s* o, s2_t bop_priority, x_source* source, sr_s* sr )
{
    // bcore_x_btcl_bop.x:183:1
    BLM_INIT_LEVEL(0);
    sr_s sb;BLM_T_INIT_SPUSH(sr_s, &sb);; BLM_TRY(x_btcl_frame_s_eval(o,bop_priority, source,&( sb )))
    
    if( sr_s_type(sr) == ((tp_t)(TYPEOF_x_btcl_function_s)) && sr_s_type(&(sb)) == ((tp_t)(TYPEOF_x_btcl_list_s)) )
    {BLM_INIT_LEVEL(1);
        x_btcl_function_s* f = ((x_btcl_function_s*)BLM_LEVEL_T_PUSH(1,x_btcl_function_s,((x_btcl_function_s*)bcore_fork(((x_btcl_function_s*)(sr->o))))));
        x_btcl_list_s* list = ((x_btcl_list_s*)BLM_LEVEL_T_PUSH(1,x_btcl_list_s,((x_btcl_list_s*)bcore_fork(((x_btcl_list_s*)(sb.o))))));
        BLM_TRY(x_btcl_function_s_call_via_arg_list(f,x_source_point_s__(((x_source_point_s*)BLM_LEVEL_T_PUSH(1,x_source_point_s,x_source_point_s_create())),source ), o, list, sr ))
        BLM_RETURNV(er_t, 0)
    BLM_DOWN();}
    
    BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Operator #<sc_t> * #<sc_t> is not defined.\n", bnameof( sr_s_o_type(sr) ), bnameof( sr_s_o_type(&(sb)) ) ))
}

er_t x_btcl_frame_s_eval_bop_func_list_transform( x_btcl_frame_s* o, s2_t bop_priority, x_source* source, sr_s* sr )
{
    // bcore_x_btcl_bop.x:200:1
    BLM_INIT_LEVEL(0);
    sr_s sb;BLM_T_INIT_SPUSH(sr_s, &sb);; BLM_TRY(x_btcl_frame_s_eval(o,bop_priority, source,&( sb )))
    
    if( sr_s_type(sr) == ((tp_t)(TYPEOF_x_btcl_function_s)) && sr_s_type(&(sb)) == ((tp_t)(TYPEOF_x_btcl_list_s)) )
    {BLM_INIT_LEVEL(1);
        x_btcl_function_s* f = ((x_btcl_function_s*)BLM_LEVEL_T_PUSH(1,x_btcl_function_s,((x_btcl_function_s*)bcore_fork(((x_btcl_function_s*)(sr->o))))));
        if( !x_btcl_function_s_is_unary(f) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Operator '*:' : Function is not unary.\n" ))
        x_btcl_list_s* list = ((x_btcl_list_s*)BLM_LEVEL_T_PUSH(1,x_btcl_list_s,((x_btcl_list_s*)bcore_fork(((x_btcl_list_s*)(sb.o))))));
        x_btcl_list_s* list_r = ((x_btcl_list_s*)BLM_LEVEL_T_PUSH(1,x_btcl_list_s,x_btcl_list_s_create()));
        x_btcl_list_s_set_size(list_r,x_btcl_list_s_size(list) );
        for(sz_t i = 0; i < x_btcl_list_s_size(list); i++ )
        {BLM_INIT_LEVEL(3);
            BLM_TRY(x_btcl_function_s_call_unary(f,x_source_point_s__(((x_source_point_s*)BLM_LEVEL_T_PUSH(3,x_source_point_s,x_source_point_s_create())),source ), o,&( list->arr.data[ i ]),&( list_r->arr.data[ i ] )))
        BLM_DOWN();}
    
        sr_s_asm(sr,((x_btcl_list_s*)bcore_fork(list_r)) );
    
        BLM_RETURNV(er_t, 0)
    BLM_DOWN();}
    
    BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Operator #<sc_t> * #<sc_t> is not defined.\n", bnameof( sr_s_o_type(sr) ), bnameof( sr_s_o_type(&(sb)) ) ))
}

er_t x_btcl_frame_s_eval_bop_func_list_unfold_transform( x_btcl_frame_s* o, s2_t bop_priority, x_source* source, sr_s* sr )
{
    // bcore_x_btcl_bop.x:226:1
    BLM_INIT_LEVEL(0);
    sr_s sb;BLM_T_INIT_SPUSH(sr_s, &sb);; BLM_TRY(x_btcl_frame_s_eval(o,bop_priority, source,&( sb )))
    
    if( sr_s_type(sr) == ((tp_t)(TYPEOF_x_btcl_function_s)) && sr_s_type(&(sb)) == ((tp_t)(TYPEOF_x_btcl_list_s)) )
    {BLM_INIT_LEVEL(1);
        x_btcl_function_s* f = ((x_btcl_function_s*)BLM_LEVEL_T_PUSH(1,x_btcl_function_s,((x_btcl_function_s*)bcore_fork(((x_btcl_function_s*)(sr->o))))));
        x_btcl_list_s* list = ((x_btcl_list_s*)BLM_LEVEL_T_PUSH(1,x_btcl_list_s,((x_btcl_list_s*)bcore_fork(((x_btcl_list_s*)(sb.o))))));
        x_btcl_list_s* list_r = ((x_btcl_list_s*)BLM_LEVEL_T_PUSH(1,x_btcl_list_s,x_btcl_list_s_create()));
        x_btcl_list_s_set_size(list_r,x_btcl_list_s_size(list) );
        for(sz_t i = 0; i < x_btcl_list_s_size(list); i++ )
        {BLM_INIT_LEVEL(3);
            if( sr_s_type(&(list->arr.data[ i ])) != ((tp_t)(TYPEOF_x_btcl_list_s)) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Operator '*.:' : List element '#<sz_t>' of type '#<sc_t>' is not a list.\n", i, bnameof( sr_s_type(&(list->arr.data[ i ])) ) ))
            BLM_TRY(x_btcl_function_s_call_via_arg_list(f,x_source_point_s__(((x_source_point_s*)BLM_LEVEL_T_PUSH(3,x_source_point_s,x_source_point_s_create())),source ), o, ((x_btcl_list_s*)(list->arr.data[ i ].o)),&( list_r->arr.data[ i ] )))
        BLM_DOWN();}
    
        sr_s_asm(sr,((x_btcl_list_s*)bcore_fork(list_r)) );
    
        BLM_RETURNV(er_t, 0)
    BLM_DOWN();}
    
    BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Operator #<sc_t> * #<sc_t> is not defined.\n", bnameof( sr_s_o_type(sr) ), bnameof( sr_s_o_type(&(sb)) ) ))
}

er_t x_btcl_frame_s_bop_cat_ab( x_btcl_frame_s* o, sr_s* sa, sr_s* sb, sr_s* sr )
{
    // bcore_x_btcl_bop.x:252:1
    
    x_btcl_clone_if_weak(sa );
    x_btcl_clone_if_weak(sb );
    
    if( sr_s_type(sa) == ((tp_t)(TYPEOF_x_btcl_signature_s)) && sr_s_type(sb) == ((tp_t)(TYPEOF_x_btcl_block_s)) )
    {BLM_INIT_LEVEL(1);
        x_btcl_signature_s* signature = ((x_btcl_signature_s*)(sa->o));
        x_btcl_block_s* block     = ((x_btcl_block_s*)(sb->o));
        x_btcl_function_s* function = x_btcl_function_s_setup(((x_btcl_function_s*)BLM_LEVEL_T_PUSH(1,x_btcl_function_s,x_btcl_function_s_create())),signature, block, NULL );
        sr_s_asc(sr,((x_btcl_function_s*)bcore_fork(function)) );
    BLM_DOWN();}
    else
    {BLM_INIT_LEVEL(1);
        x_btcl_list_s* list = ((x_btcl_list_s*)BLM_LEVEL_T_PUSH(1,x_btcl_list_s,x_btcl_list_s_create()));
        if( sr_s_type(sa) == ((tp_t)(TYPEOF_x_btcl_list_s)) ) x_btcl_list_s_push_list_fork(list,((x_btcl_list_s*)(sa->o)) ); else x_btcl_list_s_push_fork(list,sa );
        if( sr_s_type(sb) == ((tp_t)(TYPEOF_x_btcl_list_s)) ) x_btcl_list_s_push_list_fork(list,((x_btcl_list_s*)(sb->o)) ); else x_btcl_list_s_push_fork(list,sb );
        sr_s_asc(sr,((x_btcl_list_s*)bcore_fork(list)) );
    BLM_DOWN();}
    
    return  0;
}

er_t x_btcl_frame_s_eval_bop_assign( x_btcl_frame_s* o, s2_t bop_priority, x_source* source, sr_s* sr )
{
    // bcore_x_btcl_bop.x:277:1
    BLM_INIT_LEVEL(0);
    if( sr_s_is_const(sr) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Assignment to a const object.\n" ))
    
    sr_s sb;BLM_T_INIT_SPUSH(sr_s, &sb);; BLM_TRY(x_btcl_frame_s_eval(o,bop_priority, source,&( sb )))
    
    sr_s_set_const(&(sb),true );
    
    switch( sr_s_o_type(sr) )
    {
        case ((tp_t)(TYPEOF_x_btcl_null_variable_s)):
        {
            x_btcl_clone_if_weak_or_twice_referenced(&(sb ));
            sr_s* sr1 = x_btcl_frame_s_var_set(o,((const x_btcl_null_variable_s*)(sr->o))->tp_name, sr_tsm(sr_s_o_type(&(sb)), bcore_fork( sb.o ) ) );
            sr_s_twc(sr,sr_s_o_type(sr1), sr1->o );
        }
        break;
    
        case ((tp_t)(TYPEOF_x_btcl_null_member_s)):
        {
            sr_s sr1 = x_btcl_null_member_s_set_sr(((x_btcl_null_member_s*)(sr->o)),&(sb ));
            sr_s_down(sr);
            (*(sr)) = sr1;
        }
        break;
    
        case ((tp_t)(TYPEOF_x_btcl_null_arr_element_s)):
        {
            sr_s sr1 = x_btcl_null_arr_element_s_set_sr(((x_btcl_null_arr_element_s*)(sr->o)),&(sb ));
            sr_s_down(sr);
            (*(sr)) = sr1;
        }
        break;
    
        default:
        {
            if( sr->o )
            {
                er_t err = x_btcl_generic_copy(sr,&( sb ));
                if( err ) {BLM_INIT_LEVEL(5); BLM_RETURNV(er_t, x_source_parse_error_fa(source,"operator '=': #<sc_t>\n", bcore_error_pop_all_to_st(((st_s*)BLM_LEVEL_T_PUSH(5,st_s,st_s_create())) )->sc )) BLM_DOWN();}
            }
        }
        break;
    }
    
    BLM_RETURNV(er_t, 0)
}

er_t x_btcl_frame_s_eval_bop_continuation( x_btcl_frame_s* o, s2_t bop_priority, x_source* source, sr_s* sr )
{
    // bcore_x_btcl_bop.x:327:1
    
    // if the last expression in a file, block or frame has a trailing semicolon, the continuation is ignored
    if( x_source_parse_bl(source," #=?([0]=='}'||[0]==')')" ) ) return  0;
    if( x_source_eos(source) ) return  0;
    
    sr_s_clear(sr);
    BLM_TRY(x_btcl_frame_s_eval(o,bop_priority, source, sr ))
    return  0;
}

er_t x_btcl_frame_s_eval_bop( x_btcl_frame_s* o, s2_t exit_priority, x_source* source, sr_s* obj )
{
    // bcore_x_btcl_bop.x:344:1
    
    // operators in descending order of priority
    
    /// priority group a ---------------------
    
    s2_t bop_priority = x_btcl_priority_a();
    
    if( bop_priority <= exit_priority ) return  0;
    
    while( x_source_parse_bl(source," #?([0]=='.'||[0]=='(')" ) )
    {
        if( x_source_parse_bl(source," #?'.'" ) )
        {
            BLM_TRY(x_btcl_frame_s_eval_bop_member(o,source, obj ))
        }
    
        if( x_source_parse_bl(source," #?'('" ) )
        {
            BLM_TRY(x_btcl_frame_s_eval_bop_functional(o,source, obj ))
            BLM_TRY(x_source_parse_fa(source," )" ))
        }
    }
    bop_priority--;
    
    /// priority group c ---------------------
    
    bop_priority = x_btcl_priority_c();
    
    if( bop_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'^'" ) ) BLM_TRY(x_btcl_frame_s_eval_export_bop_type(o,((tp_t)(TYPEOF_pow)), bop_priority, source, obj, obj ))
    bop_priority--;
    
    if( bop_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'/'" ) ) BLM_TRY(x_btcl_frame_s_eval_export_bop_type(o,((tp_t)(TYPEOF_div)), bop_priority, source, obj, obj ))
    bop_priority--;
    
    if( bop_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'%'" ) ) BLM_TRY(x_btcl_frame_s_eval_export_bop_type(o,((tp_t)(TYPEOF_mod)), bop_priority, source, obj, obj ))
    bop_priority--;
    
    if( bop_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'**'" ) ) BLM_TRY(x_btcl_frame_s_eval_export_bop_type(o,((tp_t)(TYPEOF_chain)), bop_priority, source, obj, obj ))
    bop_priority--;
    
    if( bop_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'*.:'" ) ) BLM_TRY(x_btcl_frame_s_eval_bop_func_list_unfold_transform(o,bop_priority, source, obj ))
    bop_priority--;
    
    if( bop_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'*.'" ) ) BLM_TRY(x_btcl_frame_s_eval_bop_func_list_unfold(o,bop_priority, source, obj ))
    bop_priority--;
    
    if( bop_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'*:'" ) ) BLM_TRY(x_btcl_frame_s_eval_bop_func_list_transform(o,bop_priority, source, obj ))
    bop_priority--;
    
    if( bop_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'*'" ) ) BLM_TRY(x_btcl_frame_s_eval_export_bop_type(o,((tp_t)(TYPEOF_mul)), bop_priority, source, obj, obj ))
    bop_priority--;
    
    if( bop_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'-'" ) ) BLM_TRY(x_btcl_frame_s_eval_export_bop_type(o,((tp_t)(TYPEOF_sub)), bop_priority, source, obj, obj ))
    bop_priority--;
    
    if( bop_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'+'" ) ) BLM_TRY(x_btcl_frame_s_eval_export_bop_type(o,((tp_t)(TYPEOF_add)), bop_priority, source, obj, obj ))
    bop_priority--;
    
    if( bop_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'::'" ) ) BLM_TRY(x_btcl_frame_s_eval_export_bop_type(o,((tp_t)(TYPEOF_spawn)), bop_priority, source, obj, obj ))
    bop_priority--;
    
    if( bop_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?':'" ) ) BLM_TRY(x_btcl_frame_s_eval_export_bop_type(o,((tp_t)(TYPEOF_cat)), bop_priority, source, obj, obj ))
    bop_priority--;
    
    if( bop_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'!='" ) ) BLM_TRY(x_btcl_frame_s_eval_export_bop_type(o,((tp_t)(TYPEOF_unequal)), bop_priority, source, obj, obj ))
    bop_priority--;
    
    if( bop_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'=='" ) ) BLM_TRY(x_btcl_frame_s_eval_export_bop_type(o,((tp_t)(TYPEOF_equal)), bop_priority, source, obj, obj ))
    bop_priority--;
    
    if( bop_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'>='" ) ) BLM_TRY(x_btcl_frame_s_eval_export_bop_type(o,((tp_t)(TYPEOF_larger_equal)), bop_priority, source, obj, obj ))
    bop_priority--;
    
    if( bop_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?([0]=='>'&&[1]!='>')" ) ) { x_source_get_char(source); BLM_TRY(x_btcl_frame_s_eval_export_bop_type(o,((tp_t)(TYPEOF_larger)), bop_priority, source, obj, obj )) }
    bop_priority--;
    
    if( bop_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'<='" ) ) BLM_TRY(x_btcl_frame_s_eval_export_bop_type(o,((tp_t)(TYPEOF_smaller_equal)), bop_priority, source, obj, obj ))
    bop_priority--;
    
    if( bop_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?([0]=='<'&&[1]!='<')" ) ) { x_source_get_char(source); BLM_TRY(x_btcl_frame_s_eval_export_bop_type(o,((tp_t)(TYPEOF_smaller)), bop_priority, source, obj, obj )) }
    bop_priority--;
    
    if( bop_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'&&'" ) ) BLM_TRY(x_btcl_frame_s_eval_export_bop_type(o,((tp_t)(TYPEOF_and)), bop_priority, source, obj, obj ))
    bop_priority--;
    
    if( bop_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'||'" ) ) BLM_TRY(x_btcl_frame_s_eval_export_bop_type(o,((tp_t)(TYPEOF_or)), bop_priority, source, obj, obj ))
    bop_priority--;
    
    if( bop_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'<<'" ) ) return  x_btcl_frame_s_eval_export_bop_type(o,((tp_t)(TYPEOF_shift_left)), bop_priority, source, obj, obj );
    bop_priority--;
    
    if( bop_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'='" ) ) BLM_TRY(x_btcl_frame_s_eval_bop_assign(o,bop_priority, source, obj ))
    bop_priority--;
    
    /// priority group e ---------------------
    
    bop_priority = x_btcl_priority_e();
    
    if( bop_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?';'" ) ) BLM_TRY(x_btcl_frame_s_eval_bop_continuation(o,bop_priority, source, obj ))
    bop_priority--;
    
    return  0;
}

er_t x_btcl_frame_s_eval_export_uop_type( x_btcl_frame_s* o, tp_t type, s2_t priority, x_source* source, sr_s* result )
{
    // bcore_x_btcl_export.x:99:1
    BLM_INIT_LEVEL(0);
    x_source_point_s* source_point = x_source_point_s__(((x_source_point_s*)BLM_LEVEL_T_PUSH(0,x_source_point_s,x_source_point_s_create())),source );
    sr_s sa;BLM_T_INIT_SPUSH(sr_s, &sa);; BLM_TRY(x_btcl_frame_s_eval(o,priority, source,&( sa )))
    x_btcl_export_uop_s* uop = x_btcl_export_uop_s__(((x_btcl_export_uop_s*)BLM_LEVEL_T_PUSH(0,x_btcl_export_uop_s,x_btcl_export_uop_s_create())),type,&( sa), source_point );
    bl_t reduced_success = false;
    BLM_TRY(x_btcl_export_uop_s_get_reduced_sr(uop,o, result,&( reduced_success )))
    if( !reduced_success )
    {
        if( x_btcl_frame_s_is_exportable(o,type ) )
        {
            sr_s_asc(result,((x_btcl_export_uop_s*)bcore_fork(uop)) );
        }
        else
        {
            BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(source_point,"Operator #<sc_t> #<sc_t> is not defined.\n", x_btcl_operator_symbol(type ), bnameof( sr_s_o_type(&(sa)) ) ))
        }
    }
    BLM_RETURNV(er_t, 0)
}

er_t x_btcl_frame_s_eval_export_bop_type( x_btcl_frame_s* o, tp_t type, s2_t priority, x_source* source, sr_s* sa, sr_s* result )
{
    // bcore_x_btcl_export.x:150:1
    BLM_INIT_LEVEL(0);
    x_source_point_s* source_point = x_source_point_s__(((x_source_point_s*)BLM_LEVEL_T_PUSH(0,x_source_point_s,x_source_point_s_create())),source );
    sr_s sb;BLM_T_INIT_SPUSH(sr_s, &sb);; BLM_TRY(x_btcl_frame_s_eval(o,priority, source,&( sb )))
    x_btcl_export_bop_s* bop = x_btcl_export_bop_s__(((x_btcl_export_bop_s*)BLM_LEVEL_T_PUSH(0,x_btcl_export_bop_s,x_btcl_export_bop_s_create())),type, sa,&( sb), source_point );
    bl_t reduced_success = false;
    BLM_TRY(x_btcl_export_bop_s_get_reduced_sr(bop,o, result,&( reduced_success )))
    if( !reduced_success )
    {
        if( x_btcl_frame_s_is_exportable(o,type ) )
        {
            sr_s_asc(result,((x_btcl_export_bop_s*)bcore_fork(bop)) );
        }
        else
        {
            BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(source_point,"Operator #<sc_t> #<sc_t> #<sc_t> is not defined.\n", bnameof( sr_s_o_type(sa) ), x_btcl_operator_symbol(type ), bnameof( sr_s_o_type(&(sb)) ) ))
        }
    }
    BLM_RETURNV(er_t, 0)
}

er_t x_btcl_frame_s_eval_net_node_modifier( x_btcl_frame_s* o, x_source* source, sr_s* node_sr )
{
    // bcore_x_btcl_net.x:95:1
    BLM_INIT_LEVEL(0);
    (*(node_sr)) = sr_clone( (*(node_sr)) );
    x_btcl_net_node_s* node = ((x_btcl_net_node_s*)BLM_LEVEL_T_PUSH(0,x_btcl_net_node_s,((x_btcl_net_node_s*)bcore_fork(((x_btcl_net_node_s*)(node_sr->o))))));
    
    const x_source_point_s* sp = x_source_point_s_setup_from_source(((x_source_point_s*)BLM_LEVEL_T_PUSH(0,x_source_point_s,x_source_point_s_create())),source );
    sr_s* branch_sr = ((sr_s*)BLM_LEVEL_T_PUSH(0,sr_s,sr_s_create()));
    
    if( x_source_parse_bl(source," #?'.'") )
    {
        if( !x_btcl_is_identifier(source ) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Identifier expected.\n" ))
        tp_t branch_name = bcore_name_enroll(x_btcl_frame_s_nameof(o,x_btcl_frame_s_get_identifier(o,source, true ) ) );
        BLM_TRY(x_source_parse_fa(source," ="))
        BLM_TRY(x_btcl_frame_s_eval(o,0, source, branch_sr ))
        x_btcl_net_node_s_push_branch(node,branch_name, true, sp, branch_sr );
    }
    else if( !x_source_parse_bl(source," #=?')'") )
    {
        BLM_TRY(x_btcl_frame_s_eval(o,0, source, branch_sr ))
        x_btcl_net_node_s_push_branch(node,0, false, sp, branch_sr );
    }
    
    x_stamp_t_mutated(((x_stamp*)(node_sr->o)),sr_s_type(node_sr) );
    if( x_source_parse_bl(source," #?','" ) ) BLM_RETURNV(er_t, x_btcl_frame_s_eval_net_node_modifier(o,source, node_sr ))
    BLM_RETURNV(er_t, 0)
}

er_t x_btcl_frame_s_eval_net_node_member( x_btcl_frame_s* o, x_source* source, sr_s* sr )
{
    // bcore_x_btcl_net.x:124:1
    BLM_INIT_LEVEL(0);
    x_btcl_net_node_s* node = ((x_btcl_net_node_s*)BLM_LEVEL_T_PUSH(0,x_btcl_net_node_s,((x_btcl_net_node_s*)bcore_fork(((x_btcl_net_node_s*)(sr->o))))));
    
    /// Identifier
    if( !x_btcl_is_identifier(source ) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Member name '<literal>' expected." ))
    
    tp_t name = x_btcl_frame_s_get_identifier(o,source, true );
    x_btcl_net_branch_s* branch = x_btcl_net_node_s_get_branch(node,name );
    if( !branch ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Branch name '#<sc_t>' not found.", x_btcl_frame_s_nameof(o,name ) ))
    
    if( sr_s_is_strong(sr) ) x_btcl_frame_s_preserve_and_set_weak(o,sr );
    
    sr_s_tsm(sr,sr_s_type(&(branch->sr)), ((x_inst*)bcore_fork(branch->sr.o)) );
    sr_s_set_const(sr,true );
    
    BLM_RETURNV(er_t, 0)
}

BCORE_DEFINE_OBJECT_INST_P( x_btcl_null_variable_s )
"aware x_btcl"
"{"
    "tp_t tp_name;"
"}";

x_btcl_null_variable_s* x_btcl_null_variable_s__( x_btcl_null_variable_s* o, tp_t tp_name )
{
    // :2:1
    
    o->tp_name = tp_name;
    return  o;
}

BCORE_DEFINE_OBJECT_INST_P( x_btcl_null_member_s )
"aware x_btcl"
"{"
    "sr_s base;"
    "tp_t tp_name;"
"}";

x_btcl_null_member_s* x_btcl_null_member_s_setup( x_btcl_null_member_s* o, sr_s* base, tp_t tp_name )
{
    // bcore_x_btcl.x:391:5
    
    sr_s_twm(&(o->base),sr_s_o_type(base), base->o );
    o->tp_name = tp_name;
    return o;
}

sr_s x_btcl_null_member_s_set_sr( x_btcl_null_member_s* o, sr_s* src )
{
    // bcore_x_btcl.x:397:5
    
    uz_t index = x_stamp_t_index(sr_s_o_type(&(o->base)), o->tp_name );
    
    if( x_stamp_t_is_static_i(sr_s_o_type(&(o->base)), index ) && src->o )
    {
        tp_t dst_type = x_stamp_t_type_i(((const x_stamp*)( o->base.o)),sr_s_o_type(&(o->base)), index );
        sr_s sr = sr_tsm(dst_type, x_inst_create(dst_type ) );
        BLM_TRY_EXIT(x_btcl_generic_copy(&(sr), src ))
        x_stamp_t_set_sr(((x_stamp*)( o->base.o)),sr_s_o_type(&(o->base)), o->tp_name, sr );
    }
    else
    {
        x_stamp_t_set_sr(((x_stamp*)( o->base.o)),sr_s_o_type(&(o->base)), o->tp_name, sr_cw(*( src )) );
    }
    
    return  x_stamp_t_m_get_sr(((x_stamp*)( o->base.o)),sr_s_o_type(&(o->base)), o->tp_name );
}

BCORE_DEFINE_OBJECT_INST_P( x_btcl_null_arr_element_s )
"aware x_btcl"
"{"
    "sr_s base;"
    "s3_t index;"
"}";

x_btcl_null_arr_element_s* x_btcl_null_arr_element_s_setup( x_btcl_null_arr_element_s* o, sr_s* base, tp_t index )
{
    // bcore_x_btcl.x:425:5
    
    sr_s_twm(&(o->base),sr_s_o_type(base), base->o );
    o->index = index;
    return o;
}

sr_s x_btcl_null_arr_element_s_set_sr( x_btcl_null_arr_element_s* o, sr_s* src )
{
    // bcore_x_btcl.x:431:5
    
    if( x_array_t_is_static(sr_s_o_type(&(o->base)) ) && src->o )
    {
        tp_t dst_type = x_array_t_get_static_type(sr_s_o_type(&(o->base)) );
        sr_s sr = sr_tsm(dst_type, x_inst_create(dst_type ) );
        BLM_TRY_EXIT(x_btcl_generic_copy(&(sr), src ))
        x_array_t_set_sr(((x_array*)( ((x_stamp*)(o->base.o)))),sr_s_o_type(&(o->base)), o->index, sr );
    }
    else
    {
        x_array_t_set_sr(((x_array*)( ((x_stamp*)(o->base.o)))),sr_s_o_type(&(o->base)), o->index, sr_cw(*( src )) );
    }
    
    return  x_array_t_m_get_sr(((x_array*)( ((x_stamp*)(o->base.o)))),sr_s_o_type(&(o->base)), o->index );
}

BCORE_DEFINE_OBJECT_INST_P( x_btcl_list_s )
"aware x_btcl"
"{"
    "bcore_arr_sr_s arr;"
"}";

er_t x_btcl_list_s_to_sink( const x_btcl_list_s* o, bl_t detailed, x_sink* sink )
{
    // bcore_x_btcl.x:466:5
    
    x_sink_push_fa(sink,"[" );
    if( detailed ) x_sink_push_fa(sink,"\n" );
    for(sz_t i = 0; i < o->arr.size; i++ )
    {
        BLM_TRY(x_btcl_to_sink(detailed,&( o->arr.data[ i ]), sink ))
        if( i < o->arr.size - 1 )
        {
            x_sink_push_fa(sink,"," );
            if( detailed ) x_sink_push_fa(sink,"\n" );
        }
    }
    x_sink_push_fa(sink,"]" );
    if( detailed ) x_sink_push_fa(sink,"\n" );
    return  0;
}

BCORE_DEFINE_OBJECT_INST_P( x_btcl_block_s )
"aware x_btcl"
"{"
    "hidden x_source_point_s source_point;"
"}";

er_t x_btcl_block_s_parse( x_btcl_block_s* o, x_btcl_frame_s* frame, x_source* source )
{
    // bcore_x_btcl.x:504:1
    
    BLM_TRY(x_source_parse_fa(source," {" ))
    
    x_source_point_s_setup_from_source(&(o->source_point),source );
    
    sz_t indent = 0;
    while( !x_source_eos(source) )
    {
        if( x_source_parse_bl(source," #=?'}'" ) )
        {
            if( indent > 0  )
            {
                x_source_get_char(source);
                indent--;
            }
            else
            {
                break;
            }
        }
        else if( x_source_parse_bl(source," #?'{'" ) )
        {
            indent++;
        }
        else if( x_source_parse_bl(source," #=?'\"'" ) )
        {
            BLM_TRY(x_source_parse_fa(source,"#-string" ))
        }
        else
        {
            x_source_get_char(source);
        }
    }
    
    BLM_TRY(x_source_parse_fa(source," }" ))
    
    return  0;
}

er_t x_btcl_block_s_eval( const x_btcl_block_s* o, const x_btcl_frame_s* parent_frame, sr_s* obj )
{
    // bcore_x_btcl.x:547:1
    
    x_source* source = o->source_point.source;
    s3_t index = x_source_get_index(source);
    x_source_set_index(source,o->source_point.index );
    BLM_TRY(x_btcl_frame_s_eval_in_frame(parent_frame,0, source, obj ))
    BLM_TRY(x_source_parse_fa(source," }" ))
    x_source_set_index(source,index );
    return  0;
}

BCORE_DEFINE_OBJECT_INST_P( x_btcl_signature_s )
"aware x_btcl"
"{"
    "hidden x_source_point_s source_point;"
    "bcore_arr_tp_s arg_list;"
"}";

er_t x_btcl_signature_s_parse( x_btcl_signature_s* o, x_btcl_frame_s* frame, x_source* source )
{
    // bcore_x_btcl.x:576:1
    
    x_source_point_s_setup_from_source(&(o->source_point),source );
    
    BLM_TRY(x_source_parse_fa(source," (" ))
    
    while( !x_source_eos(source) && !x_source_parse_bl(source," #=?')'" ) )
    {
        // identifier
        if( x_btcl_is_identifier(source ) )
        {
            tp_t name = x_btcl_frame_s_get_identifier(frame,source, true );
            BLM_TRY(x_btcl_frame_s_check_reserved(frame,name, source ))
            bcore_arr_tp_s_push(&(o->arg_list),name );
        }
        else
        {
            return  x_source_parse_error_fa(source,"Identifier expected.\n" );
        }
    
        if( !x_source_parse_bl(source," #?','" ) ) break;
    }
    
    BLM_TRY(x_source_parse_fa(source," )" ))
    
    return  0;
}

BCORE_DEFINE_OBJECT_INST_P( x_btcl_external_function_s )
"aware x_btcl"
"{"
    "tp_t name;"
    "aware x_btcl -> object;"
"}";

x_btcl_external_function_s* x_btcl_external_function_s_setup( x_btcl_external_function_s* o, tp_t name, const x_btcl* object )
{
    // bcore_x_btcl.x:612:5
    
    o->name = name;
    x_btcl_a_attach( &(o->object ), (x_btcl*)( ((x_btcl*)bcore_fork(((x_btcl*)(object))))));
    return o;
}

er_t x_btcl_external_function_s_execute( x_btcl_external_function_s* o, x_source_point_s* source_point, const bcore_arr_sr_s* args, sr_s* sr )
{
    // bcore_x_btcl.x:618:5
    
    if( x_btcl_a_btcl_function(o->object,o->name, args, sr ) ) {BLM_INIT_LEVEL(1); BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(source_point,"#<sc_t>", bcore_error_pop_all_to_st(((st_s*)BLM_LEVEL_T_PUSH(1,st_s,st_s_create())) )->sc )) BLM_DOWN();}
    return  0;
}

BCORE_DEFINE_OBJECT_INST_P( x_btcl_function_s )
"aware x_btcl"
"{"
    "x_btcl_signature_s -> signature;"
    "x_btcl_block_s -> block;"
    "x_btcl_external_function_s -> external_function;"
    "x_btcl_function_s -> wrapped_function;"
    "bcore_arr_sr_s -> wrapped_arg_list;"
    "x_btcl_function_s => tail;"
"}";

x_btcl_function_s* x_btcl_function_s_setup( x_btcl_function_s* o, x_btcl_signature_s* signature, x_btcl_block_s* block, const x_btcl_function_s* tail )
{
    // bcore_x_btcl.x:637:5
    
    x_btcl_signature_s_attach( &(o->signature ),  ((x_btcl_signature_s*)bcore_fork(signature)));
    x_btcl_block_s_attach( &(o->block     ),  ((x_btcl_block_s*)bcore_fork(block)));
    x_btcl_function_s_attach( &(o->tail      ),  x_btcl_function_s_clone(tail));
    x_btcl_external_function_s_attach( &(o->external_function ),  NULL);
    return o;
}

x_btcl_function_s* x_btcl_function_s_setup_external_function( x_btcl_function_s* o, tp_t name, s2_t arity, const x_btcl* object )
{
    // bcore_x_btcl.x:645:5
    
    x_btcl_signature_s_attach( &(o->signature ),  x_btcl_signature_s_create());
    bcore_arr_tp_s_set_size(&(o->signature->arg_list),arity );
    for(sz_t i = 0; i < arity; i++ ) o->signature->arg_list.data[ i ] = i + 1;
    x_btcl_external_function_s_attach( &(o->external_function ),  x_btcl_external_function_s_setup(x_btcl_external_function_s_create(),name, object ));
    x_btcl_block_s_attach( &(o->block ),  NULL);
    x_btcl_function_s_attach( &(o->tail  ),  NULL);
    return  o;
}

x_btcl_function_s* x_btcl_function_s_setup_wrapped_function( x_btcl_function_s* o, x_btcl_function_s* wrapped_function, bcore_arr_sr_s* wrapped_arg_list )
{
    // bcore_x_btcl.x:656:5
    BLM_INIT_LEVEL(0);
    x_btcl_function_s_attach( &(o->wrapped_function ),  ((x_btcl_function_s*)bcore_fork(wrapped_function)));
    bcore_arr_sr_s_attach( &(o->wrapped_arg_list ),  ((bcore_arr_sr_s*)bcore_fork(wrapped_arg_list)));
    
    x_btcl_signature_s* signature = ((x_btcl_signature_s*)BLM_LEVEL_T_PUSH(0,x_btcl_signature_s,x_btcl_signature_s_create()));
    for(sz_t i = wrapped_arg_list->size; i < o->wrapped_function->signature->arg_list.size; i++ )
    {
        bcore_arr_tp_s_push(&(signature->arg_list),o->wrapped_function->signature->arg_list.data[ i ] );
    }
    
    x_btcl_signature_s_attach( &(o->signature ),  ((x_btcl_signature_s*)bcore_fork(signature)));
    BLM_RETURNV(x_btcl_function_s*, o)
}

x_btcl_function_s* x_btcl_function_s_append_tail( x_btcl_function_s* o, const x_btcl_function_s* tail )
{
    // bcore_x_btcl.x:671:5
    
    if( !o->tail )
    {
        x_btcl_function_s_attach( &(o->tail ),  x_btcl_function_s_clone(tail));
    }
    else
    {
        x_btcl_function_s_append_tail(o->tail,tail );
    }
    return  o;
}

er_t x_btcl_function_s_setup_frame( x_btcl_function_s* o, x_btcl_frame_s* lexical_frame, x_source_point_s* source_point, x_btcl_frame_s* frame )
{
    // bcore_x_btcl.x:693:1
    
    if( lexical_frame->depth >= x_btcl_max_frame_depth() ) return  x_source_point_s_parse_error_fa(source_point,"Maximum frame depth (#<sz_t>) exceeded. Check for unlimited recursions.\n", x_btcl_max_frame_depth() );
    x_btcl_frame_s_setup(frame,lexical_frame );
    x_btcl_frame_s_var_set(frame,TYPEOF_self, sr_asm(((x_inst*)( ((x_btcl_function_s*)bcore_fork(o)) ))) );
    
    return  0;
}

er_t x_btcl_function_s_call( x_btcl_function_s* o, x_source_point_s* source_point, x_btcl_frame_s* lexical_frame, bcore_arr_sr_s* arg_list, sr_s* sr )
{
    // bcore_x_btcl.x:705:1
    
    const x_btcl_signature_s* signature = o->signature;
    if( arg_list->size < signature->arg_list.size )
    {
        sr_s_asm(sr,x_btcl_function_s_setup_wrapped_function(x_btcl_function_s_create(),o, arg_list) );
        return  0;
    }
    else
    {
        if( o->block )
        {BLM_INIT_LEVEL(2);
            x_btcl_frame_s* frame = ((x_btcl_frame_s*)BLM_LEVEL_T_PUSH(2,x_btcl_frame_s,x_btcl_frame_s_create()));
            BLM_TRY(x_btcl_function_s_setup_frame(o,lexical_frame, source_point, frame ))
            for(sz_t i = 0; i < signature->arg_list.size; i++ ) sr_s_fork_from(x_btcl_frame_s_var_set(frame,signature->arg_list.data[ i ], sr_null() ),&(arg_list->data[ i ] ));
            sr_s_clear(sr);
            BLM_TRY(x_btcl_block_s_eval(o->block,frame, sr ))
            x_btcl_clone_if_weak(sr );
        BLM_DOWN();}
        else if( o->external_function )
        {
            BLM_TRY(x_btcl_external_function_s_execute(o->external_function,source_point, arg_list, sr ))
        }
        else if( o->wrapped_function )
        {BLM_INIT_LEVEL(3);
            bcore_arr_sr_s* full_arg_list = ((bcore_arr_sr_s*)BLM_LEVEL_T_PUSH(3,bcore_arr_sr_s,bcore_arr_sr_s_create()));
            for(sz_t i = 0; i < o->wrapped_arg_list->size; i++ ) sr_s_fork_from(bcore_arr_sr_s_push_sr(full_arg_list,sr_null() ),&(o->wrapped_arg_list->data[ i ] ));
            for(sz_t i = 0; i < arg_list->size          ; i++ ) sr_s_fork_from(bcore_arr_sr_s_push_sr(full_arg_list,sr_null() ),&(arg_list->data[ i ] ));
            BLM_TRY(x_btcl_function_s_call(o->wrapped_function,source_point, lexical_frame, full_arg_list, sr ))
        BLM_DOWN();}
        else
        {
            BLM_TRY(x_source_point_s_parse_error_fa(source_point,"Internal error: Function is incomplete.\n" ))
        }
    
        // chain of functions ...
        if( o->tail )
        {
            BLM_TRY(x_btcl_function_s_call_unary(o->tail,source_point, lexical_frame, sr, sr ))
        }
        return  0;
    }
}

er_t x_btcl_function_s_call_unary( x_btcl_function_s* o, x_source_point_s* source_point, x_btcl_frame_s* lexical_frame, sr_s* s_arg, sr_s* sr )
{
    // bcore_x_btcl.x:751:1
    BLM_INIT_LEVEL(0);
    bcore_arr_sr_s* arg_list = ((bcore_arr_sr_s*)BLM_LEVEL_T_PUSH(0,bcore_arr_sr_s,bcore_arr_sr_s_create()));
    sr_s_fork_from(bcore_arr_sr_s_push_sr(arg_list,sr_null() ),s_arg );
    BLM_RETURNV(er_t, x_btcl_function_s_call(o,source_point, lexical_frame, arg_list, sr ))
}

er_t x_btcl_function_s_call_via_arg_list( x_btcl_function_s* o, x_source_point_s* source_point, x_btcl_frame_s* lexical_frame, x_btcl_list_s* arg_list1, sr_s* sr )
{
    // bcore_x_btcl.x:760:1
    BLM_INIT_LEVEL(0);
    bcore_arr_sr_s* arg_list = bcore_arr_sr_s_set_size(((bcore_arr_sr_s*)BLM_LEVEL_T_PUSH(0,bcore_arr_sr_s,bcore_arr_sr_s_create())),x_btcl_list_s_size(arg_list1) );
    for(sz_t i = 0; i < arg_list->size; i++ ) sr_s_fork_from(&(arg_list->data[ i ]),&(arg_list1->arr.data[ i ] ));
    BLM_RETURNV(er_t, x_btcl_function_s_call(o,source_point, lexical_frame, arg_list, sr ))
}

er_t x_btcl_function_s_call_via_evaluation( x_btcl_function_s* o, x_source* source, x_btcl_frame_s* lexical_frame, sr_s* sr )
{
    // bcore_x_btcl.x:769:1
    BLM_INIT_LEVEL(0);
    bcore_arr_sr_s* arg_list = ((bcore_arr_sr_s*)BLM_LEVEL_T_PUSH(0,bcore_arr_sr_s,bcore_arr_sr_s_create()));
    while( !x_source_eos(source) && !x_source_parse_bl(source," #=?')'" ) )
    {BLM_INIT_LEVEL(1);
        sr_s* sr_arg = ((sr_s*)BLM_LEVEL_T_PUSH(1,sr_s,sr_s_create()));
        BLM_TRY(x_btcl_frame_s_eval(lexical_frame,0, source, sr_arg ))
        sr_s_fork_from(bcore_arr_sr_s_push_sr(arg_list,sr_null() ),sr_arg );
        if( !x_source_parse_bl(source," #=?')'" ) ) BLM_TRY(x_source_parse_fa(source," ," ))
    BLM_DOWN();}
    BLM_RETURNV(er_t, x_btcl_function_s_call(o,x_source_point_s__(((x_source_point_s*)BLM_LEVEL_T_PUSH(0,x_source_point_s,x_source_point_s_create())),source ), lexical_frame, arg_list, sr ))
}

BCORE_DEFINE_OBJECT_INST_P( x_btcl_export_uop_s )
"aware x_btcl"
"{"
    "tp_t type;"
    "sr_s a;"
    "x_source_point_s source_point;"
"}";

x_btcl_export_uop_s* x_btcl_export_uop_s__( x_btcl_export_uop_s* o, tp_t type, sr_s* a, x_source_point_s* source_point )
{
    // bcore_x_btcl_export.x:51:5
    
    o->type = type;
    sr_s_tsm(&(o->a),sr_s_type(a), ((x_inst*)bcore_fork(a->o)) );
    if( source_point ) x_source_point_s_copy(&(o->source_point),source_point );
    return o;
}

er_t x_btcl_export_uop_s_get_reduced_sr( x_btcl_export_uop_s* o, x_btcl_frame_s* frame, sr_s* result, bl_t* success )
{
    // bcore_x_btcl_export.x:64:1
    
    (*(success)) = true;
    
    switch( o->type )
    {
        case ((tp_t)(TYPEOF_identity)):
        {
            sr_s_tsc(result,sr_s_type(&(o->a)), ((x_inst*)bcore_fork(o->a.o)) );
            return  0;
        }
        break;
    
        case ((tp_t)(TYPEOF_neg)):
        {
            if( sr_s_is_integer(&(o->a)) ) { sr_s_const_from_s3(result,-sr_s_to_s3(&(o->a)) ); return  0; }
            if( sr_s_is_float(&(o->a))   ) { sr_s_const_from_f3(result,-sr_s_to_f3(&(o->a)) ); return  0; }
        }
        break;
    
        case ((tp_t)(TYPEOF_not)):
        {
            if( sr_s_is_numeric(&(o->a)) ) { sr_s_const_from_bl(result,!sr_s_to_bl(&(o->a)) ); return  0; }
        }
        break;
    
        default: break;
    }
    
    (*(success)) = false;
    return  0;
}

BCORE_DEFINE_OBJECT_INST_P( x_btcl_export_bop_s )
"aware x_btcl"
"{"
    "tp_t type;"
    "sr_s a;"
    "sr_s b;"
    "x_source_point_s source_point;"
"}";

x_btcl_export_bop_s* x_btcl_export_bop_s__( x_btcl_export_bop_s* o, tp_t type, sr_s* a, sr_s* b, x_source_point_s* source_point )
{
    // bcore_x_btcl_export.x:136:5
    
    o->type = type;
    sr_s_tsm(&(o->a),sr_s_type(a), ((x_inst*)bcore_fork(a->o)) );
    sr_s_tsm(&(o->b),sr_s_type(b), ((x_inst*)bcore_fork(b->o)) );
    if( source_point ) x_source_point_s_copy(&(o->source_point),source_point );
    return o;
}

er_t x_btcl_export_bop_s_get_reduced_sr( x_btcl_export_bop_s* o, x_btcl_frame_s* frame, sr_s* result, bl_t* success )
{
    // bcore_x_btcl_export.x:173:1
    
    (*(success)) = true;
    
    switch( o->type )
    {
        case ((tp_t)(TYPEOF_pow)):
        {
            if( bcore_tp_is_numeric( sr_s_type(&(o->a)) ) && bcore_tp_is_numeric( sr_s_type(&(o->b)) ) )
            {
                sr_s_const_from_f3(result,pow( sr_s_to_f3(&(o->a)), sr_s_to_f3(&(o->b)) ) );
                return  0;
            }
        }
        break;
    
        case ((tp_t)(TYPEOF_div)):
        {
            if( bcore_tp_is_numeric( sr_s_type(&(o->a)) ) && bcore_tp_is_numeric( sr_s_type(&(o->b)) ) )
            {
                if( sr_s_to_f3(&(o->b)) == 0 ) return  x_source_point_s_parse_error_fa(&(o->source_point),"Division by zero.\n" );
    
                if( bcore_tp_is_integer( sr_s_type(&(o->a)) ) && bcore_tp_is_integer( sr_s_type(&(o->b)) ) )
                {
                    sr_s_const_from_s3(result,sr_s_to_s3(&(o->a)) / sr_s_to_s3(&(o->b)) );
                }
                else
                {
                    sr_s_const_from_f3(result,sr_s_to_f3(&(o->a)) / sr_s_to_f3(&(o->b)) );
                }
                return  0;
            }
        }
        break;
    
        case ((tp_t)(TYPEOF_mod)):
        {
            if( bcore_tp_is_integer( sr_s_type(&(o->a)) ) && bcore_tp_is_integer( sr_s_type(&(o->b)) ) )
            {
                if( sr_s_to_s3(&(o->b)) == 0 ) return  x_source_point_s_parse_error_fa(&(o->source_point),"Modulo division by zero.\n" );
                sr_s_const_from_s3(result,sr_s_to_s3(&(o->a)) % sr_s_to_s3(&(o->b)) );
                return  0;
            }
        }
        break;
    
        case ((tp_t)(TYPEOF_chain)):
        {
            if( sr_s_type(&(o->a)) == ((tp_t)(TYPEOF_x_btcl_function_s)) && sr_s_type(&(o->b)) == ((tp_t)(TYPEOF_x_btcl_function_s)) )
            {BLM_INIT_LEVEL(4);
                x_btcl_function_s* fa = ((x_btcl_function_s*)BLM_LEVEL_T_PUSH(4,x_btcl_function_s,((x_btcl_function_s*)bcore_fork(((x_btcl_function_s*)(o->a.o))))));
                x_btcl_function_s* fb = ((x_btcl_function_s*)BLM_LEVEL_T_PUSH(4,x_btcl_function_s,((x_btcl_function_s*)bcore_fork(((x_btcl_function_s*)(o->b.o))))));
    
                if( !x_btcl_function_s_is_unary(fa) ) BLM_TRY(x_source_point_s_parse_error_fa(&(o->source_point),"Operator *: Left argument is not a unary function.\n" ))
    
                x_btcl_function_s* fc = x_btcl_function_s_setup(((x_btcl_function_s*)BLM_LEVEL_T_PUSH(4,x_btcl_function_s,x_btcl_function_s_create())),fb->signature, fb->block, fb->tail );
                x_btcl_function_s_append_tail(fc,fa );
    
                sr_s_asm(result,((x_btcl_function_s*)bcore_fork(fc)) );
                BLM_RETURNV(er_t, 0)
            BLM_DOWN();}
        }
        break;
    
        case ((tp_t)(TYPEOF_mul)):
        {
            if( bcore_tp_is_numeric( sr_s_type(&(o->a)) ) && bcore_tp_is_numeric( sr_s_type(&(o->b)) ) )
            {
                if( bcore_tp_is_integer( sr_s_type(&(o->a)) ) && bcore_tp_is_integer( sr_s_type(&(o->b)) ) )
                {
                    sr_s_const_from_s3(result,sr_s_to_s3(&(o->a)) * sr_s_to_s3(&(o->b)) );
                }
                else
                {
                    sr_s_const_from_f3(result,sr_s_to_f3(&(o->a)) * sr_s_to_f3(&(o->b)) );
                }
                return  0;
            }
            else if( sr_s_type(&(o->a)) == ((tp_t)(TYPEOF_x_btcl_list_s)) )
            {
                const x_btcl_list_s* list_a = ((const x_btcl_list_s*)(o->a.o));
                if( sr_s_type(&(o->b)) == ((tp_t)(TYPEOF_x_btcl_list_s)) )
                {BLM_INIT_LEVEL(6);
                    const x_btcl_list_s* list_b = ((const x_btcl_list_s*)(o->b.o));
                    x_btcl_list_s* list_r = ((x_btcl_list_s*)BLM_LEVEL_T_PUSH(6,x_btcl_list_s,x_btcl_list_s_create()));
                    x_btcl_list_s_set_size(list_r,x_btcl_list_s_size(list_a) * x_btcl_list_s_size(list_b) );
                    for(sz_t i = 0; i < x_btcl_list_s_size(list_a); i++ )
                    {
                        sr_s* sa =&( list_a->arr.data[ i ]);
    
                        for(sz_t j = 0; j < x_btcl_list_s_size(list_b); j++ )
                        {
                            sr_s* sb =&( list_b->arr.data[ j ]);
                            BLM_TRY(x_btcl_frame_s_bop_cat_ab(frame,sa, sb,&( list_r->arr.data[ i * x_btcl_list_s_size(list_b) + j ] )))
                        }
                    }
                    sr_s_asc(result,((x_btcl_list_s*)bcore_fork(list_r)) );
                    BLM_RETURNV(er_t, 0)
                BLM_DOWN();}
            }
            else if( sr_s_type(&(o->a)) == ((tp_t)(TYPEOF_x_btcl_function_s)) )
            {BLM_INIT_LEVEL(5);
                x_btcl_function_s* f = ((x_btcl_function_s*)BLM_LEVEL_T_PUSH(5,x_btcl_function_s,((x_btcl_function_s*)bcore_fork(((x_btcl_function_s*)(o->a.o))))));
                bcore_arr_sr_s* arg_list = ((bcore_arr_sr_s*)BLM_LEVEL_T_PUSH(5,bcore_arr_sr_s,bcore_arr_sr_s_create()));
                sr_s_fork_from(bcore_arr_sr_s_push_sr(arg_list,sr_null() ),&(o->b ));
                BLM_TRY(x_btcl_function_s_call(f,&(o->source_point), frame, arg_list, result ))
                BLM_RETURNV(er_t, 0)
            BLM_DOWN();}
        }
        break;
    
        case ((tp_t)(TYPEOF_sub)):
        {
            if( bcore_tp_is_numeric( sr_s_type(&(o->a)) ) && bcore_tp_is_numeric( sr_s_type(&(o->b)) ) )
            {
                if( bcore_tp_is_integer( sr_s_type(&(o->a)) ) && bcore_tp_is_integer( sr_s_type(&(o->b)) ) )
                {
                    sr_s_const_from_s3(result,sr_s_to_s3(&(o->a)) - sr_s_to_s3(&(o->b)) );
                }
                else
                {
                    sr_s_const_from_f3(result,sr_s_to_f3(&(o->a)) - sr_s_to_f3(&(o->b)) );
                }
                return  0;
            }
        }
        break;
    
        case ((tp_t)(TYPEOF_add)):
        {
            if( bcore_tp_is_numeric( sr_s_type(&(o->a)) ) )
            {
                if( bcore_tp_is_numeric( sr_s_type(&(o->b)) ) )
                {
                    if( bcore_tp_is_integer( sr_s_type(&(o->a)) ) && bcore_tp_is_integer( sr_s_type(&(o->b)) ) )
                    {
                        sr_s_const_from_s3(result,sr_s_to_s3(&(o->a)) + sr_s_to_s3(&(o->b)) );
                    }
                    else
                    {
                        sr_s_const_from_f3(result,sr_s_to_f3(&(o->a)) + sr_s_to_f3(&(o->b)) );
                    }
                    return  0;
                }
            }
    
            if( sr_s_type(&(o->a)) == ((tp_t)(TYPEOF_st_s)) || sr_s_type(&(o->b)) == ((tp_t)(TYPEOF_st_s)) )
            {BLM_INIT_LEVEL(4);
                st_s* st1 = ((st_s*)BLM_LEVEL_T_PUSH(4,st_s,st_s_create()));
                st_s* st2 = ((st_s*)BLM_LEVEL_T_PUSH(4,st_s,st_s_create()));
                if( st_s_copy_typed(st1,sr_s_type(&(o->a)), o->a.o ) ) {BLM_INIT_LEVEL(5); BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(&(o->source_point),"operator '+': #<sc_t>\n", bcore_error_pop_all_to_st(((st_s*)BLM_LEVEL_T_PUSH(5,st_s,st_s_create())) )->sc )) BLM_DOWN();}
                if( st_s_copy_typed(st2,sr_s_type(&(o->b)), o->b.o ) ) {BLM_INIT_LEVEL(5); BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(&(o->source_point),"operator '+': #<sc_t>\n", bcore_error_pop_all_to_st(((st_s*)BLM_LEVEL_T_PUSH(5,st_s,st_s_create())) )->sc )) BLM_DOWN();}
                st_s_push_st(st1,st2 );
    
                sr_s_asc(result,((st_s*)bcore_fork(st1)) );
                BLM_RETURNV(er_t, 0)
            BLM_DOWN();}
        }
        break;
    
        case ((tp_t)(TYPEOF_spawn)):
        {
            if( sr_s_is_numeric(&(o->a)) )
            {BLM_INIT_LEVEL(4);
                x_btcl_list_s* list = ((x_btcl_list_s*)BLM_LEVEL_T_PUSH(4,x_btcl_list_s,x_btcl_list_s_create()));
                bcore_arr_sr_s_set_size(&(list->arr),sr_s_to_s3(&(o->a)) );
                if( sr_s_type(&(o->b)) == ((tp_t)(TYPEOF_x_btcl_function_s)) )
                {BLM_INIT_LEVEL(5);
                    x_btcl_frame_s* local_frame = x_btcl_frame_s_setup(((x_btcl_frame_s*)BLM_LEVEL_T_PUSH(5,x_btcl_frame_s,x_btcl_frame_s_create())),frame );
                    const x_btcl_function_s* function = ((const x_btcl_function_s*)(o->b.o));
                    if( x_btcl_function_s_args(function) != 1 ) BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(&(o->source_point),"Operator #<sc_t> :: #<sc_t>: Right operand must be unary (single argument).\n", bnameof( sr_s_type(&(o->a)) ), bnameof( sr_s_type(&(o->b)) ) ))
                    tp_t arg_name = x_btcl_function_s_arg_name(function,0 );
                    for(sz_t i = 0; i < list->arr.size; i++ )
                    {
                        x_btcl_frame_s_var_set(local_frame,arg_name, sr_s3(i ) );
                        BLM_TRY(x_btcl_block_s_eval(function->block,local_frame,&( list->arr.data[ i ] )))
                    }
                BLM_DOWN();}
                else
                {
                    for(sz_t i = 0; i < list->arr.size; i++ )
                    {
                        sr_s_tsc(&(list->arr.data[ i ]),sr_s_type(&(o->b)), x_inst_t_clone( o->b.o,sr_s_type(&(o->b)) ) );
                    }
                }
    
                sr_s_asc(result,((x_btcl_list_s*)bcore_fork(list)) );
    
                BLM_RETURNV(er_t, 0)
            BLM_DOWN();}
            else if( sr_s_type(&(o->a)) == ((tp_t)(TYPEOF_x_btcl_list_s)) )
            {
                const x_btcl_list_s* list_a = ((const x_btcl_list_s*)(o->a.o));
    
                if( sr_s_type(&(o->b)) == ((tp_t)(TYPEOF_x_btcl_function_s)) )
                {BLM_INIT_LEVEL(6);
                    x_btcl_frame_s* local_frame = x_btcl_frame_s_setup(((x_btcl_frame_s*)BLM_LEVEL_T_PUSH(6,x_btcl_frame_s,x_btcl_frame_s_create())),frame );
                    const x_btcl_function_s* function = ((const x_btcl_function_s*)(o->b.o));
    
                    if( x_btcl_function_s_args(function) == 1 )
                    {BLM_INIT_LEVEL(7);
                        x_btcl_list_s* list = ((x_btcl_list_s*)BLM_LEVEL_T_PUSH(7,x_btcl_list_s,x_btcl_list_s_create()));
                        x_btcl_list_s_set_size(list,x_btcl_list_s_size(list_a) );
                        tp_t arg_name = x_btcl_function_s_arg_name(function,0 );
                        for(sz_t i = 0; i < list->arr.size; i++ )
                        {
                            x_btcl_frame_s_var_set(local_frame,arg_name, sr_cw( list_a->arr.data[ i ] ) );
                            BLM_TRY(x_btcl_block_s_eval(function->block,local_frame,&( list->arr.data[ i ] )))
                        }
                        sr_s_asc(result,((x_btcl_list_s*)bcore_fork(list)) );
                        BLM_RETURNV(er_t, 0)
                    BLM_DOWN();}
                    else if( x_btcl_function_s_args(function) == 2 )
                    {BLM_INIT_LEVEL(8);
                        if( list_a->arr.size < 1 )
                        {
                            BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(&(o->source_point),"Operator #<sc_t> :: #<sc_t>: Left operand must be a list of size >= 1.\n", bnameof( sr_s_type(&(o->a)) ), bnameof( sr_s_type(&(o->b)) ) ))
                        }
    
                        sr_s* sa = ((sr_s*)BLM_LEVEL_T_PUSH(8,sr_s,sr_s_create()));
                        (*(sa)) = sr_cw( list_a->arr.data[ 0 ] );
    
                        for(sz_t i = 1; i < list_a->arr.size; i++ )
                        {
                            x_btcl_frame_s_var_set(local_frame,x_btcl_function_s_arg_name(function,0 ),*( sa ));
                            (*(sa)) = sr_null();
                            x_btcl_frame_s_var_set(local_frame,x_btcl_function_s_arg_name(function,1 ), sr_cw( list_a->arr.data[ i ] ) );
                            BLM_TRY(x_btcl_block_s_eval(function->block,local_frame, sa ))
                        }
    
                        sr_s_fork_from(result,sa );
                        BLM_RETURNV(er_t, 0)
                    BLM_DOWN();}
                    else
                    {
                        BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(&(o->source_point),"Operator #<sc_t> :: #<sc_t>: Right operand must be unary (one argument) or binary (two arguments).\n", bnameof( sr_s_type(&(o->a)) ), bnameof( sr_s_type(&(o->b)) ) ))
                    }
                BLM_DOWN();}
            }
        }
        break;
    
        case ((tp_t)(TYPEOF_cat)):            return  x_btcl_frame_s_bop_cat_ab(frame,&(o->a),&( o->b), result );
        case ((tp_t)(TYPEOF_equal)):         sr_s_from_bl(result,0 == x_compare_t_num_dominant(sr_s_type(&(o->a)), o->a.o, sr_s_type(&(o->b)), o->b.o ) ); return  0;
        case ((tp_t)(TYPEOF_unequal)):       sr_s_from_bl(result,0 != x_compare_t_num_dominant(sr_s_type(&(o->a)), o->a.o, sr_s_type(&(o->b)), o->b.o ) ); return  0;
        case ((tp_t)(TYPEOF_larger_equal)):  sr_s_from_bl(result,0 >= x_compare_t_num_dominant(sr_s_type(&(o->a)), o->a.o, sr_s_type(&(o->b)), o->b.o ) ); return  0;
        case ((tp_t)(TYPEOF_larger)):        sr_s_from_bl(result,0 >  x_compare_t_num_dominant(sr_s_type(&(o->a)), o->a.o, sr_s_type(&(o->b)), o->b.o ) ); return  0;
        case ((tp_t)(TYPEOF_smaller_equal)): sr_s_from_bl(result,0 <= x_compare_t_num_dominant(sr_s_type(&(o->a)), o->a.o, sr_s_type(&(o->b)), o->b.o ) ); return  0;
        case ((tp_t)(TYPEOF_smaller)):       sr_s_from_bl(result,0 <  x_compare_t_num_dominant(sr_s_type(&(o->a)), o->a.o, sr_s_type(&(o->b)), o->b.o ) ); return  0;
    
        case ((tp_t)(TYPEOF_and)): if( sr_s_is_numeric(&(o->a)) && sr_s_is_numeric(&(o->b)) ) { sr_s_from_bl(result,sr_s_to_bl(&(o->a)) && sr_s_to_bl(&(o->b)) ); return  0; } break;
        case ((tp_t)(TYPEOF_or)):  if( sr_s_is_numeric(&(o->a)) && sr_s_is_numeric(&(o->b)) ) { sr_s_from_bl(result,sr_s_to_bl(&(o->a)) || sr_s_to_bl(&(o->b)) ); return  0; } break;
    
        case ((tp_t)(TYPEOF_shift_left)):
        {
            if( sr_s_type(&(o->a)) == ((tp_t)(TYPEOF_x_btcl_function_s)) )
            {BLM_INIT_LEVEL(4);
                x_btcl_function_s* f = ((x_btcl_function_s*)BLM_LEVEL_T_PUSH(4,x_btcl_function_s,((x_btcl_function_s*)bcore_fork(((x_btcl_function_s*)(o->a.o))))));
                bcore_arr_sr_s* arg_list = ((bcore_arr_sr_s*)BLM_LEVEL_T_PUSH(4,bcore_arr_sr_s,bcore_arr_sr_s_create()));
                sr_s_fork_from(bcore_arr_sr_s_push_sr(arg_list,sr_null() ),&(o->b ));
                BLM_TRY(x_btcl_function_s_call(f,&(o->source_point), frame, arg_list, result ))
                BLM_RETURNV(er_t, 0)
            BLM_DOWN();}
            else if( sr_s_type(&(o->a)) == ((tp_t)(TYPEOF_x_btcl_net_node_s)) )
            {BLM_INIT_LEVEL(5);
                x_btcl_net_node_s* node = ((x_btcl_net_node_s*)BLM_LEVEL_T_PUSH(5,x_btcl_net_node_s,x_btcl_net_node_s_clone(((x_btcl_net_node_s*)(o->a.o)))));
                x_btcl_net_node_s_push_branch(node,0, false,&( o->source_point),&( o->b ));
                sr_s_asc(result,((x_btcl_net_node_s*)bcore_fork(node)) );
                BLM_RETURNV(er_t, 0)
            BLM_DOWN();}
        }
        break;
    
        default: break;
    }
    
    (*(success)) = false;
    return  0;
}

BCORE_DEFINE_OBJECT_INST_P( x_btcl_net_branch_s )
"aware x_btcl"
"{"
    "tp_t name;"
    "sr_s sr;"
    "hidden x_source_point_s sp;"
"}";

x_btcl_net_branch_s* x_btcl_net_branch_s_setup( x_btcl_net_branch_s* o, tp_t name, const x_source_point_s* sp, sr_s* sr )
{
    // bcore_x_btcl_net.x:33:5
    
    o->name = name;
    x_source_point_s_copy(&(o->sp),sp );
    sr_s_clear(&(o->sr));
    if( sr ) sr_s_tsm(&(o->sr),sr_s_type(sr), ((x_inst*)bcore_fork(sr->o)) );
    return o;
}

x_btcl_net_branch_s* x_btcl_net_branch_s__( x_btcl_net_branch_s* o, tp_t name, const sr_s* sr )
{
    // :2:1
    
    o->name = name;
    sr_s_copy(&(o->sr),sr );
    return  o;
}

BCORE_DEFINE_OBJECT_INST_P( x_btcl_net_node_s )
"aware x_array"
"{"
    "tp_t type;"
    "tp_t name;"
    "x_btcl_net_branch_s => [];"
    "hidden x_source_point_s sp;"
"}";

x_btcl_net_node_s* x_btcl_net_node_s_setup( x_btcl_net_node_s* o, tp_t type, tp_t name, const x_source_point_s* sp )
{
    // bcore_x_btcl_net.x:55:5
    
    o->type = type;
    o->name = name;
    x_source_point_s_copy(&(o->sp),sp );
    return  o;
}

x_btcl_net_node_s* x_btcl_net_node_s_setup_wire( x_btcl_net_node_s* o, tp_t rack_name, tp_t wire_name, const x_source_point_s* sp )
{
    // bcore_x_btcl_net.x:63:5
    
    return  x_btcl_net_node_s_push_branch(x_btcl_net_node_s_setup(o,((tp_t)(TYPEOF_wire)), rack_name, sp ),wire_name, true, sp, NULL );
}

bl_t x_btcl_net_node_s_exists( x_btcl_net_node_s* o, tp_t branch_name )
{
    // bcore_x_btcl_net.x:68:5
    
    {const x_btcl_net_node_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){x_btcl_net_branch_s* e=__a->data[__i]; if( e->name == branch_name ) return  true;
    }}return  false;
}

x_btcl_net_branch_s* x_btcl_net_node_s_get_branch( x_btcl_net_node_s* o, tp_t name )
{
    // bcore_x_btcl_net.x:75:5
    
    {const x_btcl_net_node_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){x_btcl_net_branch_s* e=__a->data[__i]; if( e->name == name ) return  e;
    }}return  NULL;
}

x_btcl_net_node_s* x_btcl_net_node_s_push_branch( x_btcl_net_node_s* o, tp_t name, bl_t replace, const x_source_point_s* sp, sr_s* sr )
{
    // bcore_x_btcl_net.x:82:5
    
    x_btcl_net_branch_s* branch = NULL;
    if( replace ) branch = x_btcl_net_node_s_get_branch(o,name );
    if( !branch ) branch = ((x_btcl_net_branch_s*)(x_array_push(((x_array*)(o)))));
    x_btcl_net_branch_s_setup(branch,name, sp, sr );
    return  o;
}

XOILA_DEFINE_SPECT( x_inst, x_btcl )
"{"
    "bcore_spect_header_s header;"
    "feature aware x_btcl : btcl_function_arity = x_btcl_btcl_function_arity_default;"
    "feature aware x_btcl : btcl_function;"
"}";

er_t x_btcl_t_from_source( x_btcl* o, tp_t t, x_source* source )
{
    // bcore_x_btcl.h:126:1
    BLM_INIT_LEVEL(0);
    sr_s* sr = ((sr_s*)BLM_LEVEL_T_PUSH(0,sr_s,sr_s_create()));
    BLM_TRY(x_btcl_parse_create_object(source, sr ))
    BLM_TRY(x_inst_t_copy_typed(((x_inst*)( o)),t, sr_s_o_type( sr ), sr->o ))
    BLM_RETURNV(er_t, 0)
}

x_btcl* x_btcl_create_from_source_t( x_source* source, tp_t* type )
{
    // bcore_x_btcl.h:136:1
    BLM_INIT_LEVEL(0);
    (*(type)) = 0;
    if( !source ) BLM_RETURNV(x_btcl*, NULL)
    sr_s* sr = ((sr_s*)BLM_LEVEL_T_PUSH(0,sr_s,sr_s_create()));
    if( x_btcl_parse_create_object(source, sr ) ) BLM_RETURNV(x_btcl*, NULL)
    if( sr->o && type ) (*(type)) = sr_s_o_type(sr);
    x_inst* obj = sr->o;
    BLM_RETURNV(x_btcl*,((x_btcl*)( ( obj ) ? ((x_inst*)bcore_fork(obj)) : NULL))) // sr.o is NULL in case of error
}

x_btcl* x_btcl_create_from_source( x_source* source )
{
    // bcore_x_btcl.h:149:1
    
    tp_t t = 0;
    x_btcl* o = x_btcl_create_from_source_t(source, (&(t)) );
    if( t )
    {
        ASSERT( x_stamp_t_is_aware(t ) );
    }
    return  o;
}

sc_t x_btcl_operator_symbol( tp_t type )
{
    // bcore_x_btcl.x:42:1
    
    switch( type )
    {
        // Group A, binary
        case ((tp_t)(TYPEOF_member)): return  ".";
        case ((tp_t)(TYPEOF_frame)):  return  "(";
    
        // Group B, unary
        case ((tp_t)(TYPEOF_identity)): return  "+";
        case ((tp_t)(TYPEOF_neg)):  return  "-";
        case ((tp_t)(TYPEOF_not)):  return  "!";
        case ((tp_t)(TYPEOF_print_compact)):  return  "?";
        case ((tp_t)(TYPEOF_print_detailed)):  return  "??";
    
        // Group C, unary
        case ((tp_t)(TYPEOF_pow)):           return  "^";
        case ((tp_t)(TYPEOF_div)):           return  "/";
        case ((tp_t)(TYPEOF_mod)):           return  "%";
        case ((tp_t)(TYPEOF_chain)):         return  "**";
        case ((tp_t)(TYPEOF_mul_dot_colon)): return  "*.:";
        case ((tp_t)(TYPEOF_mul_dot)):       return  "*.";
        case ((tp_t)(TYPEOF_mul_colon)):     return  "*:";
    
        case ((tp_t)(TYPEOF_mul)): return  "*";
        case ((tp_t)(TYPEOF_sub)): return  "-";
        case ((tp_t)(TYPEOF_add)): return  "+";
        case ((tp_t)(TYPEOF_spawn)): return  "::";
        case ((tp_t)(TYPEOF_cat)): return  ":";
    
        case ((tp_t)(TYPEOF_equal)):         return  "==";
        case ((tp_t)(TYPEOF_unequal)):       return  "!=";
        case ((tp_t)(TYPEOF_larger_equal)):  return  ">=";
        case ((tp_t)(TYPEOF_larger)):        return  ">";
        case ((tp_t)(TYPEOF_smaller_equal)): return  "<=";
        case ((tp_t)(TYPEOF_smaller)):       return  "<";
    
        case ((tp_t)(TYPEOF_and)):        return  "&";
        case ((tp_t)(TYPEOF_or)):         return  "|";
        case ((tp_t)(TYPEOF_shift_left)): return  "<<";
        case ((tp_t)(TYPEOF_assign)):     return  "=";
    
        // Group E, binary
        case ((tp_t)(TYPEOF_continuation)): return  ";";
    
        default: break;
    }
    
    ERR_fa( "Unhandled operator type '#<sc_t>'.", bnameof( type ) );
    
    return  0;
}

void x_btcl_clone_if_weak( sr_s* sr )
{
    // bcore_x_btcl.x:108:1
    
    if( sr_s_is_weak(sr) )
    {
        bl_t is_const = sr_s_is_const(sr);
        (*(sr)) = sr_clone(*( sr ));
        sr_s_set_const(sr,is_const );
    }
}

void x_btcl_clone_if_weak_or_twice_referenced( sr_s* sr )
{
    // bcore_x_btcl.x:124:1
    
    if( sr_s_is_weak(sr) || sr_s_references(sr) >= 2 )
    {
        bl_t is_const = sr_s_is_const(sr);
        (*(sr)) = sr_clone(*( sr ));
        sr_s_set_const(sr,is_const );
    }
}

er_t x_btcl_get_embedding_file_path( x_source* source, sc_t in_path, st_s* out_path )
{
    // bcore_x_btcl.x:137:1
    BLM_INIT_LEVEL(0);
    st_s* folder = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,bcore_file_folder_path(x_source_get_file(source) )));
    if( folder->size == 0 ) st_s_push_char(folder,'.' );
    
    if( in_path[ 0 ] == '/' )
    {
        st_s_copy_sc(out_path,in_path );
    }
    else
    {
        st_s_copy_fa(out_path,"#<sc_t>/#<sc_t>", folder->sc, in_path );
    }
    
    if( !bcore_file_exists(out_path->sc ) )
    {
        BLM_RETURNV(er_t, x_source_parse_error_fa(source,"embed: File '#<sc_t>' not found.", out_path->sc ))
    }
    
    BLM_RETURNV(er_t, 0)
}

er_t x_btcl_generic_copy( sr_s* sr, const sr_s* sb )
{
    // bcore_x_btcl.x:162:1
    
    er_t copy_typed_err = x_inst_t_copy_typed( sr->o,sr_s_o_type(sr), sr_s_o_type(sb), sb->o );
    if( copy_typed_err == ((tp_t)(TYPEOF_conversion_error)) )
    {
        if( sr_s_o_type(sb) == ((tp_t)(TYPEOF_x_btcl_list_s)) )
        {
            x_array* array = NULL;
            tp_t t_array = 0;
            if( x_array_t_is_array(sr_s_type(sr) ) )
            {
                array =((x_array*)( sr->o));
                t_array = sr_s_type(sr);
            }
            else if( x_stamp_t_is_aware(sr_s_type(sr) ) && ( array = x_array_feature_a_m_get_wrapped_array(((x_array_feature*)(sr->o))) ) )
            {
                t_array = array->_;
            }
    
            if( array )
            {
                const x_btcl_list_s* list = ((const x_btcl_list_s*)(sb->o));
                x_array_t_clear( array,t_array );
                for(sz_t i = 0; i < list->arr.size; i++ ) x_array_t_push_sr( array,t_array, sr_cw( list->arr.data[ i ] ) );
                bcore_error_remove_last_if_of(((tp_t)(TYPEOF_conversion_error)) );
                copy_typed_err = 0;
            }
        }
    }
    return  copy_typed_err;
}

er_t x_btcl_to_sink( bl_t detailed, const sr_s* sr, x_sink* sink )
{
    // bcore_x_btcl.x:930:1
    
    if( detailed )
    {
        if( sr_s_o_type(sr) == ((tp_t)(TYPEOF_x_btcl_list_s)) ) BLM_TRY(x_btcl_list_s_to_sink(((const x_btcl_list_s*)(sr->o)),detailed, sink ))
        else x_btml_t_to_sink( ((const x_btml*)(sr->o)),sr_s_o_type(sr), sink );
    }
    else
    {
        if     ( !sr->o ) x_sink_push_fa(sink,"NULL" );
        else if( sr_s_type(sr) == ((tp_t)(TYPEOF_bl_t)) ) x_sink_push_fa(sink,"#<sc_t>", sr_s_to_bl(sr) ? "true" : "false" );
        else if( sr_s_is_integer(sr) ) x_sink_push_fa(sink,"#<s3_t>", sr_s_to_s3(sr) );
        else if( sr_s_is_float(sr) )   x_sink_push_fa(sink,"#<f3_t>", sr_s_to_f3(sr) );
        else if( sr_s_o_type(sr) == ((tp_t)(TYPEOF_st_s)) ) x_sink_push_fa(sink,"#<sc_t>", ((const st_s*)(sr->o))->sc );
        else if( sr_s_o_type(sr) == ((tp_t)(TYPEOF_x_btcl_list_s)) ) BLM_TRY(x_btcl_list_s_to_sink(((const x_btcl_list_s*)(sr->o)),detailed, sink ))
        else x_btml_t_to_sink( ((const x_btml*)(sr->o)),sr_s_o_type(sr), sink );
    }
    return  0;
}

er_t x_btcl_parse_create_object( x_source* source, sr_s* obj )
{
    // bcore_x_btcl.x:1242:1
    BLM_INIT_LEVEL(0);
    x_btcl_frame_s* frame = x_btcl_frame_s_setup_as_root(((x_btcl_frame_s*)BLM_LEVEL_T_PUSH(0,x_btcl_frame_s,x_btcl_frame_s_create())),NULL );
    BLM_RETURNV(er_t, x_btcl_frame_s_parse_create_object(frame,source, obj ))
}

void x_btcl_selftest( sc_t file )
{
    // bcore_x_btcl.x:1255:1
    BLM_INIT_LEVEL(0);
    sr_s* obj = ((sr_s*)BLM_LEVEL_T_PUSH(0,sr_s,sr_s_create()));
    BLM_TRY_EXIT(x_btcl_parse_create_object(((x_source*)BLM_LEVEL_A_PUSH(0,x_source_check_create_from_file(file ))), obj ))
    ASSERT( obj->o );
    ASSERT( sr_s_is_numeric(obj) );
    ASSERT( sr_s_to_s3(obj) == 0 );
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
            BCORE_REGISTER_FUNC( x_inst_exists );
            BCORE_REGISTER_FUNC( x_inst_t_copy );
            BCORE_REGISTER_FUNC( x_inst_copy );
            BCORE_REGISTER_FUNC( x_inst_t_copy_typed );
            BCORE_REGISTER_FUNC( x_inst_copy_typed );
            BCORE_REGISTER_FUNC( x_inst_t_clone );
            BCORE_REGISTER_FUNC( x_inst_clone );
            BCORE_REGISTER_FUNC( x_inst_t_discard );
            BCORE_REGISTER_FUNC( x_inst_discard );
            BCORE_REGISTER_FUNC( x_inst_ifd );
            BCORE_REGISTER_FUNC( x_inst_ifc );
            BCORE_REGISTER_FUNC( x_inst_create );
            BCORE_REGISTER_FUNC( x_inst_attend );
            BCORE_REGISTER_FUNC( x_inst_attn );
            XOILA_REGISTER_SPECT( x_inst );

            // --------------------------------------------------------------------
            // source: bcore_x_compare.h

            // group: x_compare
            BCORE_REGISTER_FUNC( x_compare_t_type_dominant );
            BCORE_REGISTER_FUNC( x_compare_type_dominant );
            BCORE_REGISTER_FUNC( x_compare_t_num_dominant );
            BCORE_REGISTER_FUNC( x_compare_num_dominant );
            XOILA_REGISTER_SPECT( x_compare );

            // --------------------------------------------------------------------
            // source: bcore_x_source.h

            // group: x_source
            BCORE_REGISTER_OBJECT( x_source_point_s );
            BCORE_REGISTER_FUNC( x_source_create_from_file );
            BCORE_REGISTER_FUNC( x_source_create_from_st );
            BCORE_REGISTER_FUNC( x_source_create_from_st_d );
            BCORE_REGISTER_FUNC( x_source_create_from_sc );
            BCORE_REGISTER_FUNC( x_source_create_from_fv );
            BCORE_REGISTER_FUNC( x_source_create_from_fa );
            BCORE_REGISTER_FUNC( x_source_check_create_from_file );
            BCORE_REGISTER_FUNC( x_source_eos );
            BCORE_REGISTER_FUNC( x_source_get_data );
            BCORE_REGISTER_FUNC( x_source_inspect_data );
            BCORE_REGISTER_FUNC( x_source_get_char );
            BCORE_REGISTER_FUNC( x_source_inspect_char );
            BCORE_REGISTER_FUNC( x_source_get_s3 );
            BCORE_REGISTER_FUNC( x_source_get_s2 );
            BCORE_REGISTER_FUNC( x_source_get_s1 );
            BCORE_REGISTER_FUNC( x_source_get_s0 );
            BCORE_REGISTER_FUNC( x_source_get_u3 );
            BCORE_REGISTER_FUNC( x_source_get_u2 );
            BCORE_REGISTER_FUNC( x_source_get_u1 );
            BCORE_REGISTER_FUNC( x_source_get_u0 );
            BCORE_REGISTER_FUNC( x_source_get_f3 );
            BCORE_REGISTER_FUNC( x_source_get_f2 );
            BCORE_REGISTER_FUNC( x_source_get_bl );
            BCORE_REGISTER_FUNC( x_source_get_tp );
            BCORE_REGISTER_FUNC( x_source_get_sz );
            BCORE_REGISTER_FUNC( x_source_get_uz );
            BCORE_REGISTER_FUNC( x_source_inspect_s3 );
            BCORE_REGISTER_FUNC( x_source_inspect_s2 );
            BCORE_REGISTER_FUNC( x_source_inspect_s1 );
            BCORE_REGISTER_FUNC( x_source_inspect_s0 );
            BCORE_REGISTER_FUNC( x_source_inspect_u3 );
            BCORE_REGISTER_FUNC( x_source_inspect_u2 );
            BCORE_REGISTER_FUNC( x_source_inspect_u1 );
            BCORE_REGISTER_FUNC( x_source_inspect_u0 );
            BCORE_REGISTER_FUNC( x_source_inspect_f3 );
            BCORE_REGISTER_FUNC( x_source_inspect_f2 );
            BCORE_REGISTER_FUNC( x_source_inspect_bl );
            BCORE_REGISTER_FUNC( x_source_inspect_tp );
            BCORE_REGISTER_FUNC( x_source_inspect_sz );
            BCORE_REGISTER_FUNC( x_source_inspect_uz );
            BCORE_REGISTER_FUNC( x_source_get_index );
            BCORE_REGISTER_FUNC( x_source_set_index );
            BCORE_REGISTER_FUNC( x_source_parse_bl );
            BCORE_REGISTER_FUNC( x_source_parse_fv );
            BCORE_REGISTER_FUNC( x_source_parse_fa );
            BCORE_REGISTER_FUNC( x_source_parse_error_fv );
            BCORE_REGISTER_FUNC( x_source_parse_error_fa );
            BCORE_REGISTER_FUNC( x_source_parse_msg_to_sink_fv );
            BCORE_REGISTER_FUNC( x_source_parse_msg_to_sink_fa );
            BCORE_REGISTER_FUNC( x_source_parse_msg_fv );
            BCORE_REGISTER_FUNC( x_source_parse_msg_fa );
            BCORE_REGISTER_FUNC( x_source_get_file );
            BCORE_REGISTER_FUNC( x_source_get_d_dir );
            BCORE_REGISTER_FUNC( x_source_stdin );
            XOILA_REGISTER_SPECT( x_source );

            // --------------------------------------------------------------------
            // source: bcore_x_sink.h

            // group: x_sink
            BCORE_REGISTER_FUNC( x_sink_create_from_file );
            BCORE_REGISTER_FUNC( x_sink_check_create_from_file );
            BCORE_REGISTER_FUNC( x_sink_push_data );
            BCORE_REGISTER_FUNC( x_sink_push_char );
            BCORE_REGISTER_FUNC( x_sink_push_sc );
            BCORE_REGISTER_FUNC( x_sink_push_st );
            BCORE_REGISTER_FUNC( x_sink_push_st_d );
            BCORE_REGISTER_FUNC( x_sink_push_fv );
            BCORE_REGISTER_FUNC( x_sink_push_fa );
            BCORE_REGISTER_FUNC( x_sink_flush );
            BCORE_REGISTER_FUNC( x_sink_push_s3 );
            BCORE_REGISTER_FUNC( x_sink_push_s2 );
            BCORE_REGISTER_FUNC( x_sink_push_s1 );
            BCORE_REGISTER_FUNC( x_sink_push_s0 );
            BCORE_REGISTER_FUNC( x_sink_push_u3 );
            BCORE_REGISTER_FUNC( x_sink_push_u2 );
            BCORE_REGISTER_FUNC( x_sink_push_u1 );
            BCORE_REGISTER_FUNC( x_sink_push_u0 );
            BCORE_REGISTER_FUNC( x_sink_push_f3 );
            BCORE_REGISTER_FUNC( x_sink_push_f2 );
            BCORE_REGISTER_FUNC( x_sink_push_bl );
            BCORE_REGISTER_FUNC( x_sink_push_tp );
            BCORE_REGISTER_FUNC( x_sink_push_sz );
            BCORE_REGISTER_FUNC( x_sink_push_uz );
            BCORE_REGISTER_FUNC( x_sink_stdout );
            BCORE_REGISTER_FUNC( x_sink_stderr );
            XOILA_REGISTER_SPECT( x_sink );

            // --------------------------------------------------------------------
            // source: bcore_x_array.h

            // group: x_array
            BCORE_REGISTER_NAME( size );
            BCORE_REGISTER_NAME( space );
            BCORE_REGISTER_NAME( data );
            BCORE_REGISTER_FUNC( x_array_t_size );
            BCORE_REGISTER_FUNC( x_array_size );
            BCORE_REGISTER_FUNC( x_array_t_set_size );
            BCORE_REGISTER_FUNC( x_array_set_size );
            BCORE_REGISTER_FUNC( x_array_t_set_space );
            BCORE_REGISTER_FUNC( x_array_set_space );
            BCORE_REGISTER_FUNC( x_array_t_clear );
            BCORE_REGISTER_FUNC( x_array_clear );
            BCORE_REGISTER_FUNC( x_array_t_sort );
            BCORE_REGISTER_FUNC( x_array_sort );
            BCORE_REGISTER_FUNC( x_array_t_is_array );
            BCORE_REGISTER_FUNC( x_array_t_is_fixed );
            BCORE_REGISTER_FUNC( x_array_t_is_static );
            BCORE_REGISTER_FUNC( x_array_t_is_of_aware );
            BCORE_REGISTER_FUNC( x_array_t_is_of_links );
            BCORE_REGISTER_FUNC( x_array_t_is_mono_typed );
            BCORE_REGISTER_FUNC( x_array_is_array );
            BCORE_REGISTER_FUNC( x_array_is_fixed );
            BCORE_REGISTER_FUNC( x_array_is_static );
            BCORE_REGISTER_FUNC( x_array_is_of_aware );
            BCORE_REGISTER_FUNC( x_array_is_of_links );
            BCORE_REGISTER_FUNC( x_array_is_mono_typed );
            BCORE_REGISTER_FUNC( x_array_t_is_mutable_mono_typed );
            BCORE_REGISTER_FUNC( x_array_is_mutable_mono_typed );
            BCORE_REGISTER_FUNC( x_array_t_is_is_multi_typed );
            BCORE_REGISTER_FUNC( x_array_is_is_multi_typed );
            BCORE_REGISTER_FUNC( x_array_t_get_static_type );
            BCORE_REGISTER_FUNC( x_array_get_static_type );
            BCORE_REGISTER_FUNC( x_array_t_get_mono_type );
            BCORE_REGISTER_FUNC( x_array_get_mono_type );
            BCORE_REGISTER_FUNC( x_array_t_get_type );
            BCORE_REGISTER_FUNC( x_array_get_type );
            BCORE_REGISTER_FUNC( x_array_t_get_data_c );
            BCORE_REGISTER_FUNC( x_array_get_data_c );
            BCORE_REGISTER_FUNC( x_array_t_get_data_m );
            BCORE_REGISTER_FUNC( x_array_get_data_m );
            BCORE_REGISTER_FUNC( x_array_t_get_unit_size );
            BCORE_REGISTER_FUNC( x_array_get_unit_size );
            BCORE_REGISTER_FUNC( x_array_t_set_gtype );
            BCORE_REGISTER_FUNC( x_array_set_gtype );
            BCORE_REGISTER_FUNC( x_array_t_push_sr );
            BCORE_REGISTER_FUNC( x_array_push_d );
            BCORE_REGISTER_FUNC( x_array_push_c );
            BCORE_REGISTER_FUNC( x_array_push_t );
            BCORE_REGISTER_FUNC( x_array_push );
            BCORE_REGISTER_FUNC( x_array_push_sr );
            BCORE_REGISTER_FUNC( x_array_t_set_sr );
            BCORE_REGISTER_FUNC( x_array_set_d );
            BCORE_REGISTER_FUNC( x_array_set_c );
            BCORE_REGISTER_FUNC( x_array_set_t );
            BCORE_REGISTER_FUNC( x_array_set_sr );
            BCORE_REGISTER_FUNC( x_array_t_m_get_sr );
            BCORE_REGISTER_FUNC( x_array_m_get_sr );
            BCORE_REGISTER_FUNC( x_array_c_get_sr );
            BCORE_REGISTER_FUNC( x_array_c_get_f3 );
            BCORE_REGISTER_FUNC( x_array_c_get_u3 );
            BCORE_REGISTER_FUNC( x_array_c_get_s3 );
            BCORE_REGISTER_FUNC( x_array_c_get_bl );
            BCORE_REGISTER_FUNC( x_array_c_get_tp );
            BCORE_REGISTER_FUNC( x_array_t_push_d );
            BCORE_REGISTER_FUNC( x_array_t_push_c );
            BCORE_REGISTER_FUNC( x_array_t_push_t );
            BCORE_REGISTER_FUNC( x_array_t_push );
            BCORE_REGISTER_FUNC( x_array_t_set_d );
            BCORE_REGISTER_FUNC( x_array_t_set_c );
            BCORE_REGISTER_FUNC( x_array_t_set_t );
            BCORE_REGISTER_FUNC( x_array_t_c_get_sr );
            XOILA_REGISTER_SPECT( x_array );

            // group: x_array_feature
            BCORE_REGISTER_FEATURE( x_array_feature_c_get_wrapped_array );
            BCORE_REGISTER_FFUNC( x_array_feature_c_get_wrapped_array, x_array_feature_c_get_wrapped_array_default );
            BCORE_REGISTER_FEATURE( x_array_feature_m_get_wrapped_array );
            BCORE_REGISTER_FFUNC( x_array_feature_m_get_wrapped_array, x_array_feature_m_get_wrapped_array_default );
            XOILA_REGISTER_SPECT( x_array_feature );

            // --------------------------------------------------------------------
            // source: bcore_x_group.h

            // group: x_group
            BCORE_REGISTER_FUNC( x_group_get_arr_traitline_stamps );
            XOILA_REGISTER_SPECT( x_group );

            // --------------------------------------------------------------------
            // source: bcore_x_stamp.h

            // group: x_stamp
            BCORE_REGISTER_NAME( x_stamp_path_s_array_index );
            BCORE_REGISTER_NAME( sr_s );
            BCORE_REGISTER_OBJECT( x_stamp_path_s );
            BCORE_REGISTER_OBJECT( x_stamp_path_adl_s );
            BCORE_REGISTER_FUNC( x_stamp_size );
            BCORE_REGISTER_FUNC( x_stamp_t_size );
            BCORE_REGISTER_FUNC( x_stamp_is_leaf );
            BCORE_REGISTER_FUNC( x_stamp_t_is_leaf );
            BCORE_REGISTER_FUNC( x_stamp_is_aware );
            BCORE_REGISTER_FUNC( x_stamp_t_is_aware );
            BCORE_REGISTER_FUNC( x_stamp_is_array );
            BCORE_REGISTER_FUNC( x_stamp_t_is_array );
            BCORE_REGISTER_FUNC( x_stamp_is_pure_array );
            BCORE_REGISTER_FUNC( x_stamp_t_is_pure_array );
            BCORE_REGISTER_FUNC( x_stamp_name );
            BCORE_REGISTER_FUNC( x_stamp_t_name );
            BCORE_REGISTER_FUNC( x_stamp_index );
            BCORE_REGISTER_FUNC( x_stamp_t_index );
            BCORE_REGISTER_FUNC( x_stamp_exists );
            BCORE_REGISTER_FUNC( x_stamp_t_exists );
            BCORE_REGISTER_FUNC( x_stamp_is_static_i );
            BCORE_REGISTER_FUNC( x_stamp_t_is_static_i );
            BCORE_REGISTER_FUNC( x_stamp_is_static );
            BCORE_REGISTER_FUNC( x_stamp_t_is_static );
            BCORE_REGISTER_FUNC( x_stamp_is_link_i );
            BCORE_REGISTER_FUNC( x_stamp_t_is_link_i );
            BCORE_REGISTER_FUNC( x_stamp_is_link );
            BCORE_REGISTER_FUNC( x_stamp_t_is_link );
            BCORE_REGISTER_FUNC( x_stamp_is_array_i );
            BCORE_REGISTER_FUNC( x_stamp_t_is_array_i );
            BCORE_REGISTER_FUNC( x_stamp_is_array_n );
            BCORE_REGISTER_FUNC( x_stamp_t_is_array_n );
            BCORE_REGISTER_FUNC( x_stamp_type );
            BCORE_REGISTER_FUNC( x_stamp_t_type );
            BCORE_REGISTER_FUNC( x_stamp_type_i );
            BCORE_REGISTER_FUNC( x_stamp_t_type_i );
            BCORE_REGISTER_FUNC( x_stamp_m_get_sr );
            BCORE_REGISTER_FUNC( x_stamp_m_get_sr_i );
            BCORE_REGISTER_FUNC( x_stamp_c_get_sr );
            BCORE_REGISTER_FUNC( x_stamp_c_get_sr_i );
            BCORE_REGISTER_FUNC( x_stamp_t_c_get );
            BCORE_REGISTER_FUNC( x_stamp_t_m_get );
            BCORE_REGISTER_FUNC( x_stamp_t_c_get_i );
            BCORE_REGISTER_FUNC( x_stamp_t_m_get_i );
            BCORE_REGISTER_FUNC( x_stamp_c_get );
            BCORE_REGISTER_FUNC( x_stamp_m_get );
            BCORE_REGISTER_FUNC( x_stamp_c_get_i );
            BCORE_REGISTER_FUNC( x_stamp_m_get_i );
            BCORE_REGISTER_FUNC( x_stamp_get_f3 );
            BCORE_REGISTER_FUNC( x_stamp_get_s3 );
            BCORE_REGISTER_FUNC( x_stamp_get_u3 );
            BCORE_REGISTER_FUNC( x_stamp_get_tp );
            BCORE_REGISTER_FUNC( x_stamp_get_bl );
            BCORE_REGISTER_FUNC( x_stamp_set_sr );
            BCORE_REGISTER_FUNC( x_stamp_set_sr_i );
            BCORE_REGISTER_FUNC( x_stamp_set_sr_ret );
            BCORE_REGISTER_FUNC( x_stamp_set_sr_ret_i );
            BCORE_REGISTER_FUNC( x_stamp_set_c );
            BCORE_REGISTER_FUNC( x_stamp_set_d );
            BCORE_REGISTER_FUNC( x_stamp_set_t_c );
            BCORE_REGISTER_FUNC( x_stamp_set_t_d );
            BCORE_REGISTER_FUNC( x_stamp_set_f3 );
            BCORE_REGISTER_FUNC( x_stamp_set_s3 );
            BCORE_REGISTER_FUNC( x_stamp_set_u3 );
            BCORE_REGISTER_FUNC( x_stamp_set_tp );
            BCORE_REGISTER_FUNC( x_stamp_set_bl );
            BCORE_REGISTER_FUNC( x_stamp_set_st );
            BCORE_REGISTER_FUNC( x_stamp_t_mutated );
            BCORE_REGISTER_FUNC( x_stamp_mutated );
            BCORE_REGISTER_FUNC( x_stamp_t_shelve );
            BCORE_REGISTER_FUNC( x_stamp_shelve );
            BCORE_REGISTER_FUNC( x_stamp_t_source );
            BCORE_REGISTER_FUNC( x_stamp_source );
            BCORE_REGISTER_FUNC( x_stamp_t_m_get_sr );
            BCORE_REGISTER_FUNC( x_stamp_t_m_get_sr_i );
            BCORE_REGISTER_FUNC( x_stamp_t_set_sr );
            BCORE_REGISTER_FUNC( x_stamp_t_set_sr_ret );
            BCORE_REGISTER_FUNC( x_stamp_t_set_sr_i );
            BCORE_REGISTER_FUNC( x_stamp_t_set_sr_ret_i );
            BCORE_REGISTER_FUNC( x_stamp_t_c_get_sr );
            BCORE_REGISTER_FUNC( x_stamp_t_c_get_sr_i );
            BCORE_REGISTER_FUNC( x_stamp_selftest );
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
            BCORE_REGISTER_FUNC( x_threads_sleep_ns );
            BCORE_REGISTER_FUNC( x_threads_sleep_us );
            BCORE_REGISTER_FUNC( x_threads_sleep_ms );
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
            BCORE_REGISTER_FUNC( bcore_file_path_minimized );
            BCORE_REGISTER_FUNC( bcore_file_extension );
            BCORE_REGISTER_FUNC( bcore_file_name );
            BCORE_REGISTER_FUNC( bcore_file_strip_extension );
            BCORE_REGISTER_FUNC( bcore_file_folder_path );
            BCORE_REGISTER_FUNC( bcore_file_folder_name );
            BCORE_REGISTER_FUNC( bcore_file_exists );
            BCORE_REGISTER_FUNC( bcore_file_touch );
            BCORE_REGISTER_FUNC( bcore_file_delete );
            BCORE_REGISTER_FUNC( bcore_file_rename );
            BCORE_REGISTER_FUNC( bcore_file_last_modification_time_us );
            BCORE_REGISTER_FUNC( bcore_file_find_descend );
            BCORE_REGISTER_FUNC( bcore_file_open_source );
            BCORE_REGISTER_FUNC( bcore_file_open_source_path );
            BCORE_REGISTER_FUNC( bcore_file_open_sink );
            BCORE_REGISTER_FUNC( bcore_file_try_open_sink );
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
            BCORE_REGISTER_FFUNC( x_array_feature_c_get_wrapped_array, bcore_main_set_s_c_get_wrapped_array );
            BCORE_REGISTER_FFUNC( x_array_feature_m_get_wrapped_array, bcore_main_set_s_m_get_wrapped_array );
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
            BCORE_REGISTER_FUNC( bcore_hmap_tp_st_selftest );
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
            BCORE_REGISTER_NAME( conversion_error );
            BCORE_REGISTER_NAME( parse_error );
            BCORE_REGISTER_NAME( plant_error );
            BCORE_REGISTER_NAME( error_stack );
            BCORE_REGISTER_FUNC( bcore_error_stack_size );
            BCORE_REGISTER_FUNC( bcore_error_clear_stack );
            BCORE_REGISTER_FUNC( bcore_error_remove_last );
            BCORE_REGISTER_FUNC( bcore_error_remove_last_if_of );
            BCORE_REGISTER_FUNC( bcore_error_push_sc );
            BCORE_REGISTER_FUNC( bcore_error_push_fv );
            BCORE_REGISTER_FUNC( bcore_error_push_fa );
            BCORE_REGISTER_FUNC( bcore_error_push_ffl_fv );
            BCORE_REGISTER_FUNC( bcore_error_push_ffl_fa );
            BCORE_REGISTER_FUNC( bcore_error_push_gffl_fv );
            BCORE_REGISTER_FUNC( bcore_error_push_gffl_fa );
            BCORE_REGISTER_FUNC( bcore_error_last );
            BCORE_REGISTER_FUNC( bcore_error_pop_st );
            BCORE_REGISTER_FUNC( bcore_error_pop_to_sink );
            BCORE_REGISTER_FUNC( bcore_error_pop_to_stderr );
            BCORE_REGISTER_FUNC( bcore_error_pop_all_to_sink );
            BCORE_REGISTER_FUNC( bcore_error_pop_all_to_stderr );
            BCORE_REGISTER_FUNC( bcore_error_pop_all );
            BCORE_REGISTER_FUNC( bcore_error_pop_all_to_st );
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
            BCORE_REGISTER_FUNC( bcore_shell_get_op_stamps );
            BCORE_REGISTER_FUNC( bcore_shell_help_to_sink );
            BCORE_REGISTER_FUNC( bcore_shell_run_expression );
            BCORE_REGISTER_FUNC( bcore_shell_loop );
            XOILA_REGISTER_SPECT( bcore_shell );

            // group: bcore_shell_op
            BCORE_REGISTER_FEATURE( bcore_shell_op_key );
            BCORE_REGISTER_FEATURE( bcore_shell_op_get_key );
            BCORE_REGISTER_FFUNC( bcore_shell_op_get_key, bcore_shell_op_get_key_default );
            BCORE_REGISTER_FEATURE( bcore_shell_op_info );
            BCORE_REGISTER_FEATURE( bcore_shell_op_get_info );
            BCORE_REGISTER_FFUNC( bcore_shell_op_get_info, bcore_shell_op_get_info_default );
            BCORE_REGISTER_FEATURE( bcore_shell_op_run );
            BCORE_REGISTER_FUNC( bcore_shell_op_key_match );
            BCORE_REGISTER_FUNC( bcore_shell_op_parse_match );
            BCORE_REGISTER_FUNC( bcore_shell_op_parse_param );
            BCORE_REGISTER_FUNC( bcore_shell_op_arg_signature_to_sink );
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
            BCORE_REGISTER_FUNC( bcore_huffman_min_bits );
            BCORE_REGISTER_FUNC( bcore_huffman_u2_from_s2 );
            BCORE_REGISTER_FUNC( bcore_huffman_u3_from_s3 );
            BCORE_REGISTER_FUNC( bcore_huffman_s2_from_u2 );
            BCORE_REGISTER_FUNC( bcore_huffman_s3_from_u3 );
            BCORE_REGISTER_FUNC( bcore_huffman_selftest );
            XOILA_REGISTER_SPECT( bcore_huffman );

            // --------------------------------------------------------------------
            // source: bcore_indexer.h

            // group: bcore_indexer
            BCORE_REGISTER_OBJECT( bcore_indexer_io_s );
            BCORE_REGISTER_OBJECT( bcore_indexer_io_arr_s );
            BCORE_REGISTER_OBJECT( bcore_indexer_cs_s );
            BCORE_REGISTER_OBJECT( bcore_indexer_cs_arr_s );
            BCORE_REGISTER_OBJECT( bcore_indexer_s );
            BCORE_REGISTER_FUNC( bcore_indexer_selftest );
            XOILA_REGISTER_SPECT( bcore_indexer );

            // --------------------------------------------------------------------
            // source: bcore_x_btml.h

            // group: x_btml
            BCORE_REGISTER_FEATURE( x_btml_btml_body_from_source );
            BCORE_REGISTER_FEATURE( x_btml_btml_body_to_sink );
            BCORE_REGISTER_FUNC( x_btml_from_source );
            BCORE_REGISTER_FUNC( x_btml_t_from_file );
            BCORE_REGISTER_FUNC( x_btml_from_file );
            BCORE_REGISTER_FUNC( x_btml_t_from_st );
            BCORE_REGISTER_FUNC( x_btml_from_st );
            BCORE_REGISTER_FUNC( x_btml_t_from_sc );
            BCORE_REGISTER_FUNC( x_btml_from_sc );
            BCORE_REGISTER_FUNC( x_btml_t_body_from_source );
            BCORE_REGISTER_FUNC( x_btml_body_from_source );
            BCORE_REGISTER_FUNC( x_btml_t_body_from_file );
            BCORE_REGISTER_FUNC( x_btml_body_from_file );
            BCORE_REGISTER_FUNC( x_btml_t_body_from_st );
            BCORE_REGISTER_FUNC( x_btml_body_from_st );
            BCORE_REGISTER_FUNC( x_btml_t_body_from_sc );
            BCORE_REGISTER_FUNC( x_btml_body_from_sc );
            BCORE_REGISTER_FUNC( x_btml_create_from_st_t );
            BCORE_REGISTER_FUNC( x_btml_create_from_sc_t );
            BCORE_REGISTER_FUNC( x_btml_create_from_st );
            BCORE_REGISTER_FUNC( x_btml_create_from_sc );
            BCORE_REGISTER_FUNC( x_btml_create_from_file );
            BCORE_REGISTER_FUNC( x_btml_to_sink );
            BCORE_REGISTER_FUNC( x_btml_t_to_file );
            BCORE_REGISTER_FUNC( x_btml_to_file );
            BCORE_REGISTER_FUNC( x_btml_t_to_stdout );
            BCORE_REGISTER_FUNC( x_btml_to_stdout );
            BCORE_REGISTER_FUNC( x_btml_test_transfer );
            BCORE_REGISTER_FUNC( x_btml_t_from_source );
            BCORE_REGISTER_FUNC( x_btml_create_from_source_t );
            BCORE_REGISTER_FUNC( x_btml_create_from_source );
            BCORE_REGISTER_FUNC( x_btml_t_to_sink );
            BCORE_REGISTER_FUNC( x_btml_is_identifier );
            BCORE_REGISTER_FUNC( x_btml_parse_name );
            BCORE_REGISTER_FUNC( x_btml_name_of );
            BCORE_REGISTER_FUNC( x_btml_type_of );
            BCORE_REGISTER_FUNC( x_btml_appears_valid );
            BCORE_REGISTER_FUNC( x_btml_t_appears_valid );
            BCORE_REGISTER_FUNC( x_btml_parse_create_object );
            BCORE_REGISTER_FUNC( x_btml_t_parse_body );
            BCORE_REGISTER_FUNC( x_btml_skip_body );
            BCORE_REGISTER_FUNC( x_btml_t_translate_recursive );
            BCORE_REGISTER_FUNC( x_btml_t_test_transfer );
            BCORE_REGISTER_FUNC( x_btml_selftest );
            XOILA_REGISTER_SPECT( x_btml );

            // --------------------------------------------------------------------
            // source: bcore_x_bbml.h

            // group: x_bbml
            BCORE_REGISTER_FEATURE( x_bbml_bbml_body_from_source );
            BCORE_REGISTER_FEATURE( x_bbml_bbml_body_to_sink );
            BCORE_REGISTER_FUNC( x_bbml_from_source );
            BCORE_REGISTER_FUNC( x_bbml_t_from_file );
            BCORE_REGISTER_FUNC( x_bbml_from_file );
            BCORE_REGISTER_FUNC( x_bbml_t_from_st );
            BCORE_REGISTER_FUNC( x_bbml_from_st );
            BCORE_REGISTER_FUNC( x_bbml_t_from_sc );
            BCORE_REGISTER_FUNC( x_bbml_from_sc );
            BCORE_REGISTER_FUNC( x_bbml_create_from_st_t );
            BCORE_REGISTER_FUNC( x_bbml_create_from_sc_t );
            BCORE_REGISTER_FUNC( x_bbml_create_from_st );
            BCORE_REGISTER_FUNC( x_bbml_create_from_sc );
            BCORE_REGISTER_FUNC( x_bbml_create_from_file );
            BCORE_REGISTER_FUNC( x_bbml_to_sink );
            BCORE_REGISTER_FUNC( x_bbml_t_to_file );
            BCORE_REGISTER_FUNC( x_bbml_to_file );
            BCORE_REGISTER_FUNC( x_bbml_test_transfer );
            BCORE_REGISTER_FUNC( x_bbml_t_from_source );
            BCORE_REGISTER_FUNC( x_bbml_create_from_source_t );
            BCORE_REGISTER_FUNC( x_bbml_create_from_source );
            BCORE_REGISTER_FUNC( x_bbml_t_to_sink );
            BCORE_REGISTER_FUNC( x_bbml_parse_type );
            BCORE_REGISTER_FUNC( x_bbml_parse_flag );
            BCORE_REGISTER_FUNC( x_bbml_parse_size );
            BCORE_REGISTER_FUNC( x_bbml_appears_valid );
            BCORE_REGISTER_FUNC( x_bbml_parse_create_object );
            BCORE_REGISTER_FUNC( x_bbml_t_parse_leaf_body );
            BCORE_REGISTER_FUNC( x_bbml_t_parse_body );
            BCORE_REGISTER_FUNC( x_bbml_push_type );
            BCORE_REGISTER_FUNC( x_bbml_push_flag );
            BCORE_REGISTER_FUNC( x_bbml_push_size );
            BCORE_REGISTER_FUNC( x_bbml_push_leaf );
            BCORE_REGISTER_FUNC( x_bbml_t_translate_recursive );
            BCORE_REGISTER_FUNC( x_bbml_t_test_transfer );
            BCORE_REGISTER_FUNC( x_bbml_selftest );
            XOILA_REGISTER_SPECT( x_bbml );

            // --------------------------------------------------------------------
            // source: bcore_x_bcml.h

            // group: x_bcml
            BCORE_REGISTER_FEATURE( x_bcml_bcml_body_from_source );
            BCORE_REGISTER_FEATURE( x_bcml_bcml_body_to_sink );
            BCORE_REGISTER_FUNC( x_bcml_from_source );
            BCORE_REGISTER_FUNC( x_bcml_t_from_file );
            BCORE_REGISTER_FUNC( x_bcml_from_file );
            BCORE_REGISTER_FUNC( x_bcml_t_from_st );
            BCORE_REGISTER_FUNC( x_bcml_from_st );
            BCORE_REGISTER_FUNC( x_bcml_t_from_sc );
            BCORE_REGISTER_FUNC( x_bcml_from_sc );
            BCORE_REGISTER_FUNC( x_bcml_create_from_st_t );
            BCORE_REGISTER_FUNC( x_bcml_create_from_sc_t );
            BCORE_REGISTER_FUNC( x_bcml_create_from_st );
            BCORE_REGISTER_FUNC( x_bcml_create_from_sc );
            BCORE_REGISTER_FUNC( x_bcml_create_from_file );
            BCORE_REGISTER_FUNC( x_bcml_to_sink );
            BCORE_REGISTER_FUNC( x_bcml_t_to_file );
            BCORE_REGISTER_FUNC( x_bcml_to_file );
            BCORE_REGISTER_FUNC( x_bcml_test_transfer );
            BCORE_REGISTER_FUNC( x_bcml_t_from_source );
            BCORE_REGISTER_FUNC( x_bcml_create_from_source_t );
            BCORE_REGISTER_FUNC( x_bcml_create_from_source );
            BCORE_REGISTER_FUNC( x_bcml_t_to_sink );
            BCORE_REGISTER_FUNC( x_bcml_parse_type );
            BCORE_REGISTER_FUNC( x_bcml_parse_flag );
            BCORE_REGISTER_FUNC( x_bcml_parse_size );
            BCORE_REGISTER_FUNC( x_bcml_appears_valid );
            BCORE_REGISTER_FUNC( x_bcml_parse_create_object );
            BCORE_REGISTER_FUNC( x_bcml_t_parse_leaf_body );
            BCORE_REGISTER_FUNC( x_bcml_t_parse_leaf_arr_body );
            BCORE_REGISTER_FUNC( x_bcml_t_parse_body );
            BCORE_REGISTER_FUNC( x_bcml_push_type );
            BCORE_REGISTER_FUNC( x_bcml_push_flag );
            BCORE_REGISTER_FUNC( x_bcml_push_size );
            BCORE_REGISTER_FUNC( x_bcml_push_leaf );
            BCORE_REGISTER_FUNC( x_bcml_push_leaf_arr );
            BCORE_REGISTER_FUNC( x_bcml_t_translate_recursive );
            BCORE_REGISTER_FUNC( x_bcml_t_test_transfer );
            BCORE_REGISTER_FUNC( x_bcml_selftest );
            XOILA_REGISTER_SPECT( x_bcml );

            // --------------------------------------------------------------------
            // source: bcore_x_btcl.h

            // group: x_btcl
            BCORE_REGISTER_FEATURE( x_btcl_btcl_function_arity );
            BCORE_REGISTER_FFUNC( x_btcl_btcl_function_arity, x_btcl_btcl_function_arity_default );
            BCORE_REGISTER_FEATURE( x_btcl_btcl_function );
            BCORE_REGISTER_NAME( add_a_b );
            BCORE_REGISTER_NAME( add_a );
            BCORE_REGISTER_FFUNC( x_btcl_btcl_function_arity, x_btcl_btcl_function_stamp_s_btcl_function_arity );
            BCORE_REGISTER_FFUNC( x_btcl_btcl_function, x_btcl_btcl_function_stamp_s_btcl_function );
            BCORE_REGISTER_OBJECT( x_btcl_btcl_function_stamp_s );
            BCORE_REGISTER_NAME( member );
            BCORE_REGISTER_NAME( frame );
            BCORE_REGISTER_NAME( identity );
            BCORE_REGISTER_NAME( neg );
            BCORE_REGISTER_NAME( not );
            BCORE_REGISTER_NAME( print_compact );
            BCORE_REGISTER_NAME( print_detailed );
            BCORE_REGISTER_NAME( pow );
            BCORE_REGISTER_NAME( div );
            BCORE_REGISTER_NAME( mod );
            BCORE_REGISTER_NAME( chain );
            BCORE_REGISTER_NAME( mul_dot_colon );
            BCORE_REGISTER_NAME( mul_dot );
            BCORE_REGISTER_NAME( mul_colon );
            BCORE_REGISTER_NAME( mul );
            BCORE_REGISTER_NAME( sub );
            BCORE_REGISTER_NAME( add );
            BCORE_REGISTER_NAME( spawn );
            BCORE_REGISTER_NAME( cat );
            BCORE_REGISTER_NAME( equal );
            BCORE_REGISTER_NAME( unequal );
            BCORE_REGISTER_NAME( larger_equal );
            BCORE_REGISTER_NAME( larger );
            BCORE_REGISTER_NAME( smaller_equal );
            BCORE_REGISTER_NAME( smaller );
            BCORE_REGISTER_NAME( and );
            BCORE_REGISTER_NAME( or );
            BCORE_REGISTER_NAME( shift_left );
            BCORE_REGISTER_NAME( assign );
            BCORE_REGISTER_NAME( continuation );
            BCORE_REGISTER_NAME( if );
            BCORE_REGISTER_NAME( else );
            BCORE_REGISTER_NAME( self );
            BCORE_REGISTER_NAME( func );
            BCORE_REGISTER_NAME( embed );
            BCORE_REGISTER_OBJECT( x_btcl_context_s );
            BCORE_REGISTER_OBJECT( x_btcl_frame_s );
            BCORE_REGISTER_OBJECT( x_btcl_null_variable_s );
            BCORE_REGISTER_OBJECT( x_btcl_null_member_s );
            BCORE_REGISTER_OBJECT( x_btcl_null_arr_element_s );
            BCORE_REGISTER_OBJECT( x_btcl_list_s );
            BCORE_REGISTER_OBJECT( x_btcl_block_s );
            BCORE_REGISTER_OBJECT( x_btcl_signature_s );
            BCORE_REGISTER_OBJECT( x_btcl_external_function_s );
            BCORE_REGISTER_OBJECT( x_btcl_function_s );
            BCORE_REGISTER_NAME( SIZE );
            BCORE_REGISTER_NAME( EXP );
            BCORE_REGISTER_NAME( LOG );
            BCORE_REGISTER_NAME( LOG2 );
            BCORE_REGISTER_NAME( LOG10 );
            BCORE_REGISTER_NAME( SIN );
            BCORE_REGISTER_NAME( COS );
            BCORE_REGISTER_NAME( TAN );
            BCORE_REGISTER_NAME( TANH );
            BCORE_REGISTER_NAME( SIGN );
            BCORE_REGISTER_NAME( SQRT );
            BCORE_REGISTER_NAME( ABS );
            BCORE_REGISTER_NAME( CEIL );
            BCORE_REGISTER_NAME( FLOOR );
            BCORE_REGISTER_NAME( true );
            BCORE_REGISTER_NAME( false );
            BCORE_REGISTER_NAME( TRUE );
            BCORE_REGISTER_NAME( FALSE );
            BCORE_REGISTER_NAME( PI );
            BCORE_REGISTER_NAME( PATH );
            BCORE_REGISTER_NAME( DIR );
            BCORE_REGISTER_OBJECT( x_btcl_export_uop_s );
            BCORE_REGISTER_OBJECT( x_btcl_export_bop_s );
            BCORE_REGISTER_OBJECT( x_btcl_net_branch_s );
            BCORE_REGISTER_NAME( rack );
            BCORE_REGISTER_NAME( wire );
            BCORE_REGISTER_OBJECT( x_btcl_net_node_s );
            BCORE_REGISTER_FUNC( x_btcl_from_source );
            BCORE_REGISTER_FUNC( x_btcl_t_from_file );
            BCORE_REGISTER_FUNC( x_btcl_from_file );
            BCORE_REGISTER_FUNC( x_btcl_t_from_st );
            BCORE_REGISTER_FUNC( x_btcl_from_st );
            BCORE_REGISTER_FUNC( x_btcl_t_from_sc );
            BCORE_REGISTER_FUNC( x_btcl_from_sc );
            BCORE_REGISTER_FUNC( x_btcl_create_from_st_t );
            BCORE_REGISTER_FUNC( x_btcl_create_from_sc_t );
            BCORE_REGISTER_FUNC( x_btcl_create_from_st );
            BCORE_REGISTER_FUNC( x_btcl_create_from_sc );
            BCORE_REGISTER_FUNC( x_btcl_create_from_file );
            BCORE_REGISTER_FUNC( x_btcl_t_from_source );
            BCORE_REGISTER_FUNC( x_btcl_create_from_source_t );
            BCORE_REGISTER_FUNC( x_btcl_create_from_source );
            BCORE_REGISTER_FUNC( x_btcl_operator_symbol );
            BCORE_REGISTER_FUNC( x_btcl_is_identifier );
            BCORE_REGISTER_FUNC( x_btcl_clone_if_weak );
            BCORE_REGISTER_FUNC( x_btcl_clone_if_weak_or_twice_referenced );
            BCORE_REGISTER_FUNC( x_btcl_get_embedding_file_path );
            BCORE_REGISTER_FUNC( x_btcl_generic_copy );
            BCORE_REGISTER_FUNC( x_btcl_max_frame_depth );
            BCORE_REGISTER_FUNC( x_btcl_priority_a );
            BCORE_REGISTER_FUNC( x_btcl_priority_b );
            BCORE_REGISTER_FUNC( x_btcl_priority_c );
            BCORE_REGISTER_FUNC( x_btcl_priority_d );
            BCORE_REGISTER_FUNC( x_btcl_priority_e );
            BCORE_REGISTER_FUNC( x_btcl_to_sink );
            BCORE_REGISTER_FUNC( x_btcl_parse_create_object );
            BCORE_REGISTER_FUNC( x_btcl_selftest );
            XOILA_REGISTER_SPECT( x_btcl );

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
            BCORE_REGISTER_FUNC( x_hmap_tp_m_map_ );
            BCORE_REGISTER_FUNC( x_hmap_tp_c_map_ );
            BCORE_REGISTER_FUNC( x_hmap_tp_c_get );
            BCORE_REGISTER_FUNC( x_hmap_tp_m_get );
            BCORE_REGISTER_FUNC( x_hmap_tp_set_c );
            BCORE_REGISTER_FUNC( x_hmap_tp_set_d );
            BCORE_REGISTER_FUNC( x_hmap_tp_remove );
            BCORE_REGISTER_FUNC( x_hmap_tp_exists );
            BCORE_REGISTER_FUNC( x_hmap_tp_clear );
            BCORE_REGISTER_FUNC( x_hmap_tp_keys );
            BCORE_REGISTER_FUNC( x_hmap_tp_size );
            BCORE_REGISTER_FUNC( x_hmap_tp_idx_key );
            BCORE_REGISTER_FUNC( x_hmap_tp_c_idx_val );
            BCORE_REGISTER_FUNC( x_hmap_tp_m_idx_val );
            BCORE_REGISTER_FUNC( x_hmap_tp_get_key_arr );
            BCORE_REGISTER_FUNC( x_hmap_tp_m_get_val_arr );
            BCORE_REGISTER_FUNC( x_hmap_tp_c_get_val_arr );
            BCORE_REGISTER_FUNC( x_hmap_tp_all_keys );
            BCORE_REGISTER_FUNC( x_hmap_tp_m_getf );
            XOILA_REGISTER_SPECT( x_hmap_tp );

            // group: x_hmap_tp_test
            BCORE_REGISTER_OBJECT( x_hmap_tp_test_kv_s );
            BCORE_REGISTER_OBJECT( x_hmap_tp_test_kv_arr_s );
            BCORE_REGISTER_OBJECT( x_hmap_tp_test_val_s );
            BCORE_REGISTER_FFUNC( x_hmap_tp_TE, x_hmap_tp_test_map_s_TE );
            BCORE_REGISTER_OBJECT( x_hmap_tp_test_map_s );
            BCORE_REGISTER_FUNC( x_hmap_tp_test_selftest );
            XOILA_REGISTER_SPECT( x_hmap_tp_test );

            // --------------------------------------------------------------------
            // source: bcore_x_deque.h

            // group: x_deque
            BCORE_REGISTER_OBJECT( x_deque_inst_adl_s );
            BCORE_REGISTER_OBJECT( x_deque_inst_s );
            BCORE_REGISTER_FUNC( x_deque_m_inst_ );
            BCORE_REGISTER_FUNC( x_deque_c_inst_ );
            BCORE_REGISTER_FUNC( x_deque_set_space );
            BCORE_REGISTER_FUNC( x_deque_clear );
            BCORE_REGISTER_FUNC( x_deque_size );
            BCORE_REGISTER_FUNC( x_deque_m_get );
            BCORE_REGISTER_FUNC( x_deque_c_get );
            BCORE_REGISTER_FUNC( x_deque_m_get_first );
            BCORE_REGISTER_FUNC( x_deque_c_get_first );
            BCORE_REGISTER_FUNC( x_deque_m_get_last );
            BCORE_REGISTER_FUNC( x_deque_c_get_last );
            BCORE_REGISTER_FUNC( x_deque_push_last_d );
            BCORE_REGISTER_FUNC( x_deque_push_first_d );
            BCORE_REGISTER_FUNC( x_deque_d_pop_last );
            BCORE_REGISTER_FUNC( x_deque_d_pop_first );
            BCORE_REGISTER_FUNC( x_deque_remove_last );
            BCORE_REGISTER_FUNC( x_deque_remove_first );
            BCORE_REGISTER_FUNC( x_deque_remove_last_n );
            BCORE_REGISTER_FUNC( x_deque_remove_first_n );
            XOILA_REGISTER_SPECT( x_deque );
        }
        break;
        case TYPEOF_down1:
        {
        }
        break;
        default: break;
    }
    return NULL;
}
// XOICO_BODY_SIGNATURE 0x8C583AB4E3382A44
// XOICO_FILE_SIGNATURE 0x28AF567354EDE0C8
