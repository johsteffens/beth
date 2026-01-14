//  Last update: 2026-01-14T11:41:04Z (UTC)
/** This file was generated from xoila source code.
 *  Compiling Agent : XOICO (C) 2020 ... 2025 J.B.Steffens
 *  Note that any manual changes in this file can be erased or overwritten by XOICO.
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
 *  bcore_parse.h
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
 *  bcore_parse.x
 *  bcore_sink_inexpandable.x
 *  bcore_source_inexpandable.x
 *  bcore_sr_inexpandable.x
 *  bcore_st_inexpandable.x
 *  bcore_via_inexpandable.x
 *  bcore_x_btcl.x
 *  bcore_x_btcl_builtin.x
 *  bcore_x_btcl_functor.x
 *  bcore_x_btcl_functor_f3.x
 *  bcore_x_btcl_net.x
 *  bcore_x_btcl_op.x
 *  bcore_x_btcl_operator.x
 *  bcore_x_btcl_operator_f3.x
 *  bcore_x_btcl_random.x
 *  bcore_x_btcl_test.x
 *  bcore_x_hmap.x
 *
 */

#include "bcore.xo.h"
#include "bcore_spect.h"
#include "bcore_spect_inst.h"
#include "bcore_sr.h"
#include "bcore_const_manager.h"

// To force a rebuild of this target by xoico, reset the hash key value below to 0.
// HKEYOF_bcore 0x9ECD3EF1E3F58E29ull

/**********************************************************************************************************************/
// source: bcore_x_root_inexpandable.h
#include "bcore_x_root_inexpandable.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_x_root_expandable

XOILA_DEFINE_SPECT_NASC_BEGIN( bcore_inst, bcore_x_root_expandable )
"{",
    "bcore_spect_header_s header;",
"}",
XOILA_DEFINE_SPECT_NASC_END( bcore_inst, bcore_x_root_expandable )

/**********************************************************************************************************************/
// source: bcore_x_inst.h
#include "bcore_x_inst.h"

//----------------------------------------------------------------------------------------------------------------------
// group: x_inst
#include "bcore_file.h"

XOILA_DEFINE_SPECT_NASC_BEGIN( bcore_inst, x_inst )
"{",
    "bcore_spect_header_s header;",
"}",
XOILA_DEFINE_SPECT_NASC_END( bcore_inst, x_inst )

x_inst* x_inst_create( tp_t type )
{
    // bcore_x_inst.h:97:1
    
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

XOILA_DEFINE_SPECT_NASC_BEGIN( bcore_inst, x_compare )
"{",
    "bcore_spect_header_s header;",
"}",
XOILA_DEFINE_SPECT_NASC_END( bcore_inst, x_compare )

/**********************************************************************************************************************/
// source: bcore_x_source.h
#include "bcore_x_source.h"

//----------------------------------------------------------------------------------------------------------------------
// group: x_source
#include "bcore_x_sink.h"

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_source_point_s )
"aware x_inst",
"{",
    "aware x_source -> source;",
    "s3_t index;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_source_point_s )

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

XOILA_DEFINE_SPECT_NASC_BEGIN( x_inst, x_source )
"{",
    "bcore_spect_header_s header;",
"}",
XOILA_DEFINE_SPECT_NASC_END( x_inst, x_source )

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

XOILA_DEFINE_SPECT_NASC_BEGIN( x_inst, x_sink )
"{",
    "bcore_spect_header_s header;",
"}",
XOILA_DEFINE_SPECT_NASC_END( x_inst, x_sink )

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

XOILA_DEFINE_SPECT_NASC_BEGIN( x_inst, x_array )
"{",
    "bcore_spect_header_s header;",
"}",
XOILA_DEFINE_SPECT_NASC_END( x_inst, x_array )

x_inst* x_array_t_push_d( x_array* o, tp_t t, x_inst* v )
{
    // bcore_x_array.h:210:1
    
    
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
    // bcore_x_array.h:244:1
    
    
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
    // bcore_x_array.h:280:1
    
    
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
    // bcore_x_array.h:308:1
    
    
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
    // bcore_x_array.h:352:1
    
    
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
    // bcore_x_array.h:385:1
    
    
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
    // bcore_x_array.h:419:1
    
    
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

x_inst* x_array_t_insert_d( x_array* o, tp_t t, sz_t index, x_inst* v )
{
    // bcore_x_array.h:445:1
    
    
        const bcore_array_s* p = bcore_array_s_get_typed( t );
    
        if( p->item_p )
        {
            bcore_array_p_insert( p, ( bcore_array* )o, index, sr_psm( p->item_p, v ) );
        }
        else
        {
            bcore_array_p_insert( p, ( bcore_array* )o, index, sr_asm( v ) );
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

x_inst* x_array_t_insert_c( x_array* o, tp_t t, sz_t index, const x_inst* v )
{
    // bcore_x_array.h:478:1
    
    
        const bcore_array_s* p = bcore_array_s_get_typed( t );
    
        if( p->item_p )
        {
            bcore_array_p_insert( p, ( bcore_array* )o, index, sr_pwc( p->item_p, v ) );
        }
        else
        {
            bcore_array_p_insert( p, ( bcore_array* )o, index, sr_awc( v ) );
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

x_inst* x_array_t_insert_t( x_array* o, tp_t t, sz_t index, tp_t val_type )
{
    // bcore_x_array.h:512:1
    
    
        const bcore_array_s* p = bcore_array_s_get_typed( t );
        bcore_array_p_insert( p, ( bcore_array* )o, index, sr_t_create( val_type ) );
    
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
    // bcore_x_array.h:538:1
    
    sr_s sr = bcore_array_t_get( t, ((const bcore_array*)(o)), index );
    if( sr_s_is_weak(&(sr)) ) sr_s_set_const(&(sr),true );
    return  sr;
}

//----------------------------------------------------------------------------------------------------------------------
// group: x_array_feature

XOILA_DEFINE_SPECT_NASC_BEGIN( x_array, x_array_feature )
"{",
    "bcore_spect_header_s header;",
    "feature aware x_array_feature : c_get_wrapped_array = x_array_feature_c_get_wrapped_array_default;",
    "feature aware x_array_feature : m_get_wrapped_array = x_array_feature_m_get_wrapped_array_default;",
"}",
XOILA_DEFINE_SPECT_NASC_END( x_array, x_array_feature )

/**********************************************************************************************************************/
// source: bcore_x_group.h
#include "bcore_x_group.h"

//----------------------------------------------------------------------------------------------------------------------
// group: x_group

XOILA_DEFINE_SPECT_NASC_BEGIN( x_inst, x_group )
"{",
    "bcore_spect_header_s header;",
"}",
XOILA_DEFINE_SPECT_NASC_END( x_inst, x_group )

/**********************************************************************************************************************/
// source: bcore_x_stamp.h
#include "bcore_x_stamp.h"

//----------------------------------------------------------------------------------------------------------------------
// group: x_stamp

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_stamp_path_s )
"aware x_inst",
"{",
    "tp_t [];",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_stamp_path_s )

void x_stamp_path_s_push_index( x_stamp_path_s* o, sz_t index )
{
    // bcore_x_stamp.h:198:5
    
    x_stamp_path_s_push(o,TYPEOF_x_stamp_path_s_array_index );
    x_stamp_path_s_push(o,index );
}

const x_stamp_path_s* x_stamp_path_s_to_sink( const x_stamp_path_s* o, bcore_sink* sink )
{
    // bcore_x_stamp.h:245:1
    
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
    // bcore_x_stamp.h:265:1
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
    // bcore_x_stamp.h:294:1
    
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
    // bcore_x_stamp.h:343:1
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

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_stamp_path_adl_s )
"aware x_array",
"{",
    "x_stamp_path_s => [];",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_stamp_path_adl_s )

XOILA_DEFINE_SPECT_NASC_BEGIN( x_inst, x_stamp )
"{",
    "bcore_spect_header_s header;",
"}",
XOILA_DEFINE_SPECT_NASC_END( x_inst, x_stamp )

sr_s x_stamp_t_m_get_sr( x_stamp* o, tp_t t, tp_t name )
{
    // bcore_x_stamp.h:409:1
    
    const bcore_via_s* p = bcore_via_s_get_typed( t );
    sz_t index = bcore_via_p_nget_index( p, ((const bcore_via*)(o)), name );
    if( index < 0 ) return  sr_null();
    return  bcore_via_p_iget( p, ((const bcore_via*)(o)), index );
}

sr_s x_stamp_t_m_get_sr_i( x_stamp* o, tp_t t, sz_t index )
{
    // bcore_x_stamp.h:419:1
    
    const bcore_via_s* p = bcore_via_s_get_typed( t );
    if( index < 0 || index >= bcore_via_p_get_size( p, ((const bcore_via*)(o)) ) ) return  sr_null();
    return  bcore_via_p_iget( p, ((const bcore_via*)(o)), index );
}

er_t x_stamp_t_set_sr( x_stamp* o, tp_t t, tp_t name, sr_s sr_src )
{
    // bcore_x_stamp.h:432:1
    
    const bcore_via_s* p = bcore_via_s_get_typed( t );
    sz_t index = bcore_via_p_nget_index( p, ((const bcore_via*)(o)), name );
    if( index < 0 )
    {
        sr_s_down(&(sr_src));
        return  0;
    }
    return  bcore_via_p_iset( p, ((bcore_via*)(o)), index, sr_src );
}

sr_s x_stamp_t_set_sr_ret( x_stamp* o, tp_t t, tp_t name, sr_s sr_src )
{
    // bcore_x_stamp.h:450:1
    
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

er_t x_stamp_t_set_sr_i( x_stamp* o, tp_t t, sz_t index, sr_s sr_src )
{
    // bcore_x_stamp.h:471:1
    
    const bcore_via_s* p = bcore_via_s_get_typed( t );
    if( index < 0 || index >= bcore_via_p_get_size( p, ((const bcore_via*)(o)) ) )
    {
        sr_s_down(&(sr_src));
        return  0;
    }
    return  bcore_via_p_iset( p, ((bcore_via*)(o)), index, sr_src );
}

sr_s x_stamp_t_set_sr_ret_i( x_stamp* o, tp_t t, sz_t index, sr_s sr_src )
{
    // bcore_x_stamp.h:488:1
    
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
    // bcore_x_stamp.h:505:1
    
    sr_s sr = x_stamp_t_m_get_sr(((x_stamp*)(o)),t, name );
    if( sr_s_is_weak(&(sr)) ) sr_s_set_const(&(sr),true );
    return  sr;
}

sr_s x_stamp_t_c_get_sr_i( const x_stamp* o, tp_t t, sz_t index )
{
    // bcore_x_stamp.h:515:1
    
    sr_s sr = x_stamp_t_m_get_sr_i(((x_stamp*)(o)),t, index );
    if( sr_s_is_weak(&(sr)) ) sr_s_set_const(&(sr),true );
    return  sr;
}

void x_stamp_selftest( void )
{
    // bcore_x_stamp.h:528:1
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

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_lock_s )
"aware x",
"{",
    "private x_mutex_s * mutex;",
    "func bcore_inst_call:down_e;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_lock_s )

x_lock_s* x_lock_s_set( x_lock_s* o, x_mutex_s* mutex )
{
    // bcore_x_threads.h:52:5
    
    x_lock_s_release(o);
    o->mutex = mutex;
    x_mutex_s_lock(o->mutex);
    return o;
}

x_lock_s* x_lock_s_release( x_lock_s* o )
{
    // bcore_x_threads.h:59:5
    
    if( o->mutex ) x_mutex_s_unlock(o->mutex);
    o->mutex = NULL;
    return o;
}

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_unlock_s )
"aware x",
"{",
    "private x_mutex_s * mutex;",
    "func bcore_inst_call:down_e;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_unlock_s )

x_unlock_s* x_unlock_s_set( x_unlock_s* o, x_mutex_s* mutex )
{
    // bcore_x_threads.h:77:5
    
    x_unlock_s_release(o);
    o->mutex = mutex;
    x_mutex_s_unlock(o->mutex);
    return o;
}

x_unlock_s* x_unlock_s_release( x_unlock_s* o )
{
    // bcore_x_threads.h:84:5
    
    if( o->mutex ) x_mutex_s_lock(o->mutex);
    o->mutex = NULL;
    return o;
}

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_mutex_s )
"aware x",
"{",
    "private pthread_mutex_t _mutex;",
    "func bcore_inst_call:init_x;",
    "func bcore_inst_call:down_e;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_mutex_s )

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_condition_s )
"aware x",
"{",
    "private pthread_cond_t _cond;",
    "func bcore_inst_call:init_x;",
    "func bcore_inst_call:down_e;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_condition_s )

XOILA_DEFINE_SPECT_NASC_BEGIN( x_inst, x )
"{",
    "bcore_spect_header_s header;",
"}",
XOILA_DEFINE_SPECT_NASC_END( x_inst, x )

//----------------------------------------------------------------------------------------------------------------------
// group: x_thread

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_thread_s )
"aware x_thread",
"{",
    "private pthread_t _thread;",
    "private bl_t _join;",
    "func bcore_inst_call:down_e;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_thread_s )

XOILA_DEFINE_SPECT_NASC_BEGIN( x, x_thread )
"{",
    "bcore_spect_header_s header;",
    "feature aware x_thread : m_thread_func;",
    "feature aware x_thread : c_thread_func;",
"}",
XOILA_DEFINE_SPECT_NASC_END( x, x_thread )

//----------------------------------------------------------------------------------------------------------------------
// group: x_thread_result

XOILA_DEFINE_SPECT_NASC_BEGIN( x_thread, x_thread_result )
"{",
    "bcore_spect_header_s header;",
"}",
XOILA_DEFINE_SPECT_NASC_END( x_thread, x_thread_result )

/**********************************************************************************************************************/
// source: bcore_file.h
#include "bcore_file.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_file

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_file_path_s )
"aware bcore_inst",
"{",
    "st_s name;",
    "hidden st_s => root;",
    "hidden st_s => full;",
    "func bcore_via_call:source;",
    "func bcore_fp:copy_typed;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_file_path_s )

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

XOILA_DEFINE_SPECT_NASC_BEGIN( bcore_inst, bcore_file )
"{",
    "bcore_spect_header_s header;",
"}",
XOILA_DEFINE_SPECT_NASC_END( bcore_inst, bcore_file )

/**********************************************************************************************************************/
// source: bcore_spect_inst_call.h
#include "bcore_spect_inst_call.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_inst_call

XOILA_DEFINE_SPECT_NASC_BEGIN( bcore_inst, bcore_inst_call )
"{",
    "bcore_spect_header_s header;",
    "feature bcore_inst_call : init_x;",
    "feature bcore_inst_call : down_e;",
    "feature bcore_inst_call : copy_e;",
    "feature bcore_inst_call : copy_x;",
    "feature bcore_inst_call : discard_e;",
"}",
XOILA_DEFINE_SPECT_NASC_END( bcore_inst, bcore_inst_call )

/**********************************************************************************************************************/
// source: bcore_spect_via_call.h
#include "bcore_spect_via_call.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_via_call

XOILA_DEFINE_SPECT_NASC_BEGIN( bcore_inst, bcore_via_call )
"{",
    "bcore_spect_header_s header;",
    "feature aware bcore_via_call : source;",
    "feature aware bcore_via_call : mutated;",
    "feature aware bcore_via_call : shelve;",
"}",
XOILA_DEFINE_SPECT_NASC_END( bcore_inst, bcore_via_call )

/**********************************************************************************************************************/
// source: bcore_main.h
#include "bcore_main.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_main

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_main_frame_s )
"aware bcore_inst",
"{",
    "bl_t first_argument_is_path_to_object = true;",
    "bl_t next_argument_is_path_to_script = true;",
    "bl_t create_log_file = false;",
    "st_s log_file_extension = \"log\";",
    "st_s local_path = \"beth.config\";",
    "bl_t local_path_descend = false;",
    "st_s global_path;",
    "tp_t object_default_type = 0;",
    "sc_t object_interpreter = \"x_btcl\";",
    "bcore_arr_st_s args;",
    "hidden bcore_mutex_s mutex;",
    "hidden aware bcore_source -> source;",
    "hidden aware bcore_sink -> sink;",
    "hidden aware bcore_sink -> log;",
    "sr_s object_sr;",
    "st_s object_path;",
    "st_s script_path;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_main_frame_s )

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_main_arr_s )
"aware x_array",
"{",
    "aware bcore_main => [];",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_main_arr_s )

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_main_set_s )
"aware bcore_main",
"{",
    "bcore_main_arr_s arr;",
    "private obliv bcore_main* current_object;",
    "bcore_mutex_s mutex_current_object;",
    "func x_array_feature:c_get_wrapped_array;",
    "func x_array_feature:m_get_wrapped_array;",
    "func ^:main;",
    "func ^:on_termination;",
    "func ^:on_interrupt;",
    "func ^:on_suspend;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_main_set_s )

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

XOILA_DEFINE_SPECT_NASC_BEGIN( bcore_inst, bcore_main )
"{",
    "bcore_spect_header_s header;",
    "feature aware bcore_main : main;",
    "feature aware bcore_main : on_termination = bcore_main_on_termination_default;",
    "feature aware bcore_main : on_interrupt = bcore_main_on_interrupt_default;",
    "feature aware bcore_main : on_suspend = bcore_main_on_suspend_default;",
"}",
XOILA_DEFINE_SPECT_NASC_END( bcore_inst, bcore_main )

/**********************************************************************************************************************/
// source: bcore_hmap_name.h
#include "bcore_hmap_name.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_hmap_name

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_hmap_name_s )
"aware bcore_inst",
"{",
    "bcore_hmap_tp_sr_s map;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_hmap_name_s )

XOILA_DEFINE_SPECT_NASC_BEGIN( bcore_inst, bcore_hmap_name )
"{",
    "bcore_spect_header_s header;",
"}",
XOILA_DEFINE_SPECT_NASC_END( bcore_inst, bcore_hmap_name )

/**********************************************************************************************************************/
// source: bcore_hmap_tp_st.h
#include "bcore_hmap_tp_st.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_hmap_tp_st

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_hmap_tp_st_s )
"aware bcore_inst",
"{",
    "bcore_hmap_tpaw_s map;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_hmap_tp_st_s )

XOILA_DEFINE_SPECT_NASC_BEGIN( x_inst, bcore_hmap_tp_st )
"{",
    "bcore_spect_header_s header;",
"}",
XOILA_DEFINE_SPECT_NASC_END( x_inst, bcore_hmap_tp_st )

/**********************************************************************************************************************/
// source: bcore_cday.h
#include "bcore_cday.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_cday

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_cday_ymd_s )
"aware bcore_cday",
"{",
    "s2_t y;",
    "s2_t m;",
    "s2_t d;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_cday_ymd_s )

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_cday_utc_s )
"aware bcore_cday",
"{",
    "s2_t cday;",
    "s2_t ms;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_cday_utc_s )

XOILA_DEFINE_SPECT_NASC_BEGIN( bcore_inst, bcore_cday )
"{",
    "bcore_spect_header_s header;",
"}",
XOILA_DEFINE_SPECT_NASC_END( bcore_inst, bcore_cday )

/**********************************************************************************************************************/
// source: bcore_error_manager.h
#include "bcore_error_manager.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_error
#include "bcore_x_array.h"

XOILA_DEFINE_SPECT_NASC_BEGIN( bcore_inst, bcore_error )
"{",
    "bcore_spect_header_s header;",
"}",
XOILA_DEFINE_SPECT_NASC_END( bcore_inst, bcore_error )

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_error_manager

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_error_manager_error_s )
"aware bcore_error_manager",
"{",
    "er_t id;",
    "st_s msg;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_error_manager_error_s )

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_error_manager_error_adl_s )
"aware x_array",
"{",
    "bcore_error_manager_error_s => [];",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_error_manager_error_adl_s )

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

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_error_manager_context_s )
"aware bcore_error_manager",
"{",
    "bcore_error_manager_error_adl_s adl;",
    "bcore_mutex_s mutex;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_error_manager_context_s )

XOILA_DEFINE_SPECT_NASC_BEGIN( bcore_error, bcore_error_manager )
"{",
    "bcore_spect_header_s header;",
"}",
XOILA_DEFINE_SPECT_NASC_END( bcore_error, bcore_error_manager )

/**********************************************************************************************************************/
// source: bcore_prsg.h
#include "bcore_prsg.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_prsg

XOILA_DEFINE_SPECT_NASC_BEGIN( bcore_inst, bcore_prsg )
"{",
    "bcore_spect_header_s header;",
    "feature strict aware bcore_prsg : bits;",
    "feature strict aware bcore_prsg : max_u3;",
    "feature strict aware bcore_prsg : min_u3;",
    "feature strict aware bcore_prsg : state_u3;",
    "feature strict aware bcore_prsg : state_bits_u3;",
    "feature strict aware bcore_prsg : state_f3;",
    "feature aware bcore_prsg : state_bl = bcore_prsg_state_bl_default;",
    "feature strict aware bcore_prsg : gen;",
    "feature strict aware bcore_prsg : gen_bits_u3;",
    "feature strict aware bcore_prsg : gen_u3;",
    "feature strict aware bcore_prsg : gen_f3;",
    "feature aware bcore_prsg : gen_bl = bcore_prsg_gen_bl_default;",
    "feature strict aware bcore_prsg : set_state_u3;",
    "feature aware bcore_prsg : set_state_f3 = bcore_prsg_set_state_f3_default;",
    "feature aware bcore_prsg : set_state_mix = bcore_prsg_set_state_mix_default;",
    "feature aware bcore_prsg : reseed = bcore_prsg_reseed_default;",
"}",
XOILA_DEFINE_SPECT_NASC_END( bcore_inst, bcore_prsg )

bcore_prsg* bcore_prsg_set_state_f3_default( bcore_prsg* o, f3_t seed )
{
    // bcore_prsg.h:74:1
    
    int exp = 0;
    s3_t seed_s3 = frexp( seed, (&(exp)) ) * 0x7FFFFFFFFFFFFFFF;
    seed_s3 *= 27362149; // some mixing
    if( exp != 0 ) seed_s3 *= exp;  // to make sure seed is not repeated at different exponents
    bcore_prsg_a_set_state_u3(o,seed_s3 );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_prsg_lcg

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_prsg_lcg_u2_00_s )
"aware bcore_prsg",
"{",
    "u2_t state = 16437;",
    "func ^:gen_u3;",
    "func ^:state_bits_u3;",
    "func ^:gen_bits_u3;",
    "func ^:state_f3;",
    "func ^:gen_f3;",
    "func ^:bits;",
    "func ^:max_u3;",
    "func ^:min_u3;",
    "func ^:state_u3;",
    "func ^:set_state_u3;",
    "func ^:gen;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_prsg_lcg_u2_00_s )

u3_t bcore_prsg_lcg_u2_00_s_state_bits_u3( const bcore_prsg_lcg_u2_00_s* o, sz_t bits )
{
    // bcore_prsg.h:99:5
    
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

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_prsg_lcg_u2_01_s )
"aware bcore_prsg",
"{",
    "u2_t state = 16437;",
    "func ^:gen_u3;",
    "func ^:state_bits_u3;",
    "func ^:gen_bits_u3;",
    "func ^:state_f3;",
    "func ^:gen_f3;",
    "func ^:bits;",
    "func ^:max_u3;",
    "func ^:min_u3;",
    "func ^:state_u3;",
    "func ^:set_state_u3;",
    "func ^:gen;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_prsg_lcg_u2_01_s )

u3_t bcore_prsg_lcg_u2_01_s_state_bits_u3( const bcore_prsg_lcg_u2_01_s* o, sz_t bits )
{
    // bcore_prsg.h:99:5
    
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

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_prsg_lcg_u2_02_s )
"aware bcore_prsg",
"{",
    "u2_t state = 16437;",
    "func ^:gen_u3;",
    "func ^:state_bits_u3;",
    "func ^:gen_bits_u3;",
    "func ^:state_f3;",
    "func ^:gen_f3;",
    "func ^:bits;",
    "func ^:max_u3;",
    "func ^:min_u3;",
    "func ^:state_u3;",
    "func ^:set_state_u3;",
    "func ^:gen;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_prsg_lcg_u2_02_s )

u3_t bcore_prsg_lcg_u2_02_s_state_bits_u3( const bcore_prsg_lcg_u2_02_s* o, sz_t bits )
{
    // bcore_prsg.h:99:5
    
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

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_prsg_lcg_u2_03_s )
"aware bcore_prsg",
"{",
    "u2_t state = 16437;",
    "func ^:gen_u3;",
    "func ^:state_bits_u3;",
    "func ^:gen_bits_u3;",
    "func ^:state_f3;",
    "func ^:gen_f3;",
    "func ^:bits;",
    "func ^:max_u3;",
    "func ^:min_u3;",
    "func ^:state_u3;",
    "func ^:set_state_u3;",
    "func ^:gen;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_prsg_lcg_u2_03_s )

u3_t bcore_prsg_lcg_u2_03_s_state_bits_u3( const bcore_prsg_lcg_u2_03_s* o, sz_t bits )
{
    // bcore_prsg.h:99:5
    
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

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_prsg_lcg_u2_04_s )
"aware bcore_prsg",
"{",
    "u2_t state = 16437;",
    "func ^:gen_u3;",
    "func ^:state_bits_u3;",
    "func ^:gen_bits_u3;",
    "func ^:state_f3;",
    "func ^:gen_f3;",
    "func ^:bits;",
    "func ^:max_u3;",
    "func ^:min_u3;",
    "func ^:state_u3;",
    "func ^:set_state_u3;",
    "func ^:gen;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_prsg_lcg_u2_04_s )

u3_t bcore_prsg_lcg_u2_04_s_state_bits_u3( const bcore_prsg_lcg_u2_04_s* o, sz_t bits )
{
    // bcore_prsg.h:99:5
    
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

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_prsg_lcg_u2_05_s )
"aware bcore_prsg",
"{",
    "u2_t state = 16437;",
    "func ^:gen_u3;",
    "func ^:state_bits_u3;",
    "func ^:gen_bits_u3;",
    "func ^:state_f3;",
    "func ^:gen_f3;",
    "func ^:bits;",
    "func ^:max_u3;",
    "func ^:min_u3;",
    "func ^:state_u3;",
    "func ^:set_state_u3;",
    "func ^:gen;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_prsg_lcg_u2_05_s )

u3_t bcore_prsg_lcg_u2_05_s_state_bits_u3( const bcore_prsg_lcg_u2_05_s* o, sz_t bits )
{
    // bcore_prsg.h:99:5
    
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

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_prsg_lcg_u3_00_s )
"aware bcore_prsg",
"{",
    "u3_t state = 16437;",
    "func ^:gen_u3;",
    "func ^:state_bits_u3;",
    "func ^:gen_bits_u3;",
    "func ^:state_f3;",
    "func ^:gen_f3;",
    "func ^:bits;",
    "func ^:max_u3;",
    "func ^:min_u3;",
    "func ^:state_u3;",
    "func ^:set_state_u3;",
    "func ^:gen;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_prsg_lcg_u3_00_s )

u3_t bcore_prsg_lcg_u3_00_s_state_bits_u3( const bcore_prsg_lcg_u3_00_s* o, sz_t bits )
{
    // bcore_prsg.h:99:5
    
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

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_prsg_lcg_u3_01_s )
"aware bcore_prsg",
"{",
    "u3_t state = 16437;",
    "func ^:gen_u3;",
    "func ^:state_bits_u3;",
    "func ^:gen_bits_u3;",
    "func ^:state_f3;",
    "func ^:gen_f3;",
    "func ^:bits;",
    "func ^:max_u3;",
    "func ^:min_u3;",
    "func ^:state_u3;",
    "func ^:set_state_u3;",
    "func ^:gen;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_prsg_lcg_u3_01_s )

u3_t bcore_prsg_lcg_u3_01_s_state_bits_u3( const bcore_prsg_lcg_u3_01_s* o, sz_t bits )
{
    // bcore_prsg.h:99:5
    
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

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_prsg_lcg_u3_02_s )
"aware bcore_prsg",
"{",
    "u3_t state = 16437;",
    "func ^:gen_u3;",
    "func ^:state_bits_u3;",
    "func ^:gen_bits_u3;",
    "func ^:state_f3;",
    "func ^:gen_f3;",
    "func ^:bits;",
    "func ^:max_u3;",
    "func ^:min_u3;",
    "func ^:state_u3;",
    "func ^:set_state_u3;",
    "func ^:gen;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_prsg_lcg_u3_02_s )

u3_t bcore_prsg_lcg_u3_02_s_state_bits_u3( const bcore_prsg_lcg_u3_02_s* o, sz_t bits )
{
    // bcore_prsg.h:99:5
    
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

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_prsg_lcg_u3_03_s )
"aware bcore_prsg",
"{",
    "u3_t state = 16437;",
    "func ^:gen_u3;",
    "func ^:state_bits_u3;",
    "func ^:gen_bits_u3;",
    "func ^:state_f3;",
    "func ^:gen_f3;",
    "func ^:bits;",
    "func ^:max_u3;",
    "func ^:min_u3;",
    "func ^:state_u3;",
    "func ^:set_state_u3;",
    "func ^:gen;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_prsg_lcg_u3_03_s )

u3_t bcore_prsg_lcg_u3_03_s_state_bits_u3( const bcore_prsg_lcg_u3_03_s* o, sz_t bits )
{
    // bcore_prsg.h:99:5
    
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

XOILA_DEFINE_SPECT_NASC_BEGIN( bcore_prsg, bcore_prsg_lcg )
"{",
    "bcore_spect_header_s header;",
"}",
XOILA_DEFINE_SPECT_NASC_END( bcore_prsg, bcore_prsg_lcg )

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_prsg_xsg

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_prsg_xsg_u2_00_s )
"aware bcore_prsg",
"{",
    "u2_t state = 16432;",
    "func ^:gen_u3;",
    "func ^:state_bits_u3;",
    "func ^:gen_bits_u3;",
    "func ^:state_f3;",
    "func ^:gen_f3;",
    "func ^:bits;",
    "func ^:max_u3;",
    "func ^:min_u3;",
    "func ^:state_u3;",
    "func ^:set_state_u3;",
    "func ^:gen;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_prsg_xsg_u2_00_s )

u3_t bcore_prsg_xsg_u2_00_s_state_bits_u3( const bcore_prsg_xsg_u2_00_s* o, sz_t bits )
{
    // bcore_prsg.h:99:5
    
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

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_prsg_xsg_u2_01_s )
"aware bcore_prsg",
"{",
    "u2_t state = 16432;",
    "func ^:gen_u3;",
    "func ^:state_bits_u3;",
    "func ^:gen_bits_u3;",
    "func ^:state_f3;",
    "func ^:gen_f3;",
    "func ^:bits;",
    "func ^:max_u3;",
    "func ^:min_u3;",
    "func ^:state_u3;",
    "func ^:set_state_u3;",
    "func ^:gen;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_prsg_xsg_u2_01_s )

u3_t bcore_prsg_xsg_u2_01_s_state_bits_u3( const bcore_prsg_xsg_u2_01_s* o, sz_t bits )
{
    // bcore_prsg.h:99:5
    
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

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_prsg_xsg_u2_02_s )
"aware bcore_prsg",
"{",
    "u2_t state = 16432;",
    "func ^:gen_u3;",
    "func ^:state_bits_u3;",
    "func ^:gen_bits_u3;",
    "func ^:state_f3;",
    "func ^:gen_f3;",
    "func ^:bits;",
    "func ^:max_u3;",
    "func ^:min_u3;",
    "func ^:state_u3;",
    "func ^:set_state_u3;",
    "func ^:gen;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_prsg_xsg_u2_02_s )

u3_t bcore_prsg_xsg_u2_02_s_state_bits_u3( const bcore_prsg_xsg_u2_02_s* o, sz_t bits )
{
    // bcore_prsg.h:99:5
    
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

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_prsg_xsg_u2_03_s )
"aware bcore_prsg",
"{",
    "u2_t state = 16432;",
    "func ^:gen_u3;",
    "func ^:state_bits_u3;",
    "func ^:gen_bits_u3;",
    "func ^:state_f3;",
    "func ^:gen_f3;",
    "func ^:bits;",
    "func ^:max_u3;",
    "func ^:min_u3;",
    "func ^:state_u3;",
    "func ^:set_state_u3;",
    "func ^:gen;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_prsg_xsg_u2_03_s )

u3_t bcore_prsg_xsg_u2_03_s_state_bits_u3( const bcore_prsg_xsg_u2_03_s* o, sz_t bits )
{
    // bcore_prsg.h:99:5
    
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

XOILA_DEFINE_SPECT_NASC_BEGIN( bcore_prsg, bcore_prsg_xsg )
"{",
    "bcore_spect_header_s header;",
"}",
XOILA_DEFINE_SPECT_NASC_END( bcore_prsg, bcore_prsg_xsg )

/**********************************************************************************************************************/
// source: bcore_shell.h
#include "bcore_shell.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_shell

XOILA_DEFINE_SPECT_NASC_BEGIN( x_inst, bcore_shell )
"{",
    "bcore_spect_header_s header;",
    "feature aware bcore_shell : loop_callback = bcore_shell_loop_callback_default;",
    "feature aware bcore_shell : op_group = bcore_shell_op_group_default;",
    "feature aware bcore_shell : push_op_groups = bcore_shell_push_op_groups_default;",
"}",
XOILA_DEFINE_SPECT_NASC_END( x_inst, bcore_shell )

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

XOILA_DEFINE_SPECT_NASC_BEGIN( bcore_shell, bcore_shell_op )
"{",
    "bcore_spect_header_s header;",
    "feature aware bcore_shell_op : key;",
    "feature aware bcore_shell_op : get_key = bcore_shell_op_get_key_default;",
    "feature aware bcore_shell_op : info;",
    "feature aware bcore_shell_op : get_info = bcore_shell_op_get_info_default;",
    "feature aware bcore_shell_op : run;",
"}",
XOILA_DEFINE_SPECT_NASC_END( bcore_shell, bcore_shell_op )

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
                        BLM_TRY_EXIT(x_stamp_set_sr(stamp,tp_name, sr_bl(true ) ))
                    }
                    else if( x_source_parse_bl(source,"#?'false'" ) )
                    {
                        BLM_TRY_EXIT(x_stamp_set_sr(stamp,tp_name, sr_bl(false ) ))
                    }
                    else
                    {
                        BLM_TRY_EXIT(x_stamp_set_sr(stamp,tp_name, sr_bl(true ) ))
                    }
                }
                break;
    
                case ((tp_t)(TYPEOF_sz_t)):
                case ((tp_t)(TYPEOF_uz_t)):
                case ((tp_t)(TYPEOF_f3_t)):
                {
                    f3_t v_f3 = 0;
                    BLM_TRY_EXIT(x_source_parse_fa(source,"#<f3_t*>", &v_f3 ))
                    BLM_TRY_EXIT(x_stamp_set_sr(stamp,tp_name, sr_f3(v_f3 ) ))
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
                    BLM_TRY_EXIT(x_stamp_set_sr(stamp,tp_name, sr_awc(((const x_inst*)(&( st )))) ))
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
    
            BLM_TRY_EXIT(x_stamp_set_sr_i(stamp,direct_index, sr_tsm(type, ((x_inst*)bcore_fork(inst)) ) ))
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

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_shell_control_s )
"aware bcore_shell_control",
"{",
    "hidden bcore_shell_control_s* parent;",
    "aware x_sink -> sink;",
    "aware x_source -> source;",
    "bcore_hmap_name_s hmap_name;",
    "x_hmap_tp_st_s hmap_alias;",
    "bl_t exit_loop;",
    "st_s path;",
    "st_s prompt;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_shell_control_s )

XOILA_DEFINE_SPECT_NASC_BEGIN( bcore_shell, bcore_shell_control )
"{",
    "bcore_spect_header_s header;",
"}",
XOILA_DEFINE_SPECT_NASC_END( bcore_shell, bcore_shell_control )

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_shell_op_default

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_shell_op_default_help_s )
"aware bcore_shell_op_default",
"{",
    "func bcore_shell_op:key;",
    "func bcore_shell_op:info;",
    "func bcore_shell_op:run;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_shell_op_default_help_s )

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_shell_op_default_ls_s )
"aware bcore_shell_op_default",
"{",
    "st_s path;",
    "func bcore_shell_op:get_key;",
    "func bcore_shell_op:get_info;",
    "func bcore_shell_op:run;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_shell_op_default_ls_s )

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

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_shell_op_default_enter_s )
"aware bcore_shell_op_default",
"{",
    "st_s path;",
    "func bcore_shell_op:key;",
    "func bcore_shell_op:info;",
    "func bcore_shell_op:run;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_shell_op_default_enter_s )

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

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_shell_op_default_exit_s )
"aware bcore_shell_op_default",
"{",
    "bl_t a = false;",
    "func bcore_shell_op:key;",
    "func bcore_shell_op:info;",
    "func bcore_shell_op:run;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_shell_op_default_exit_s )

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_shell_op_default_get_s )
"aware bcore_shell_op_default",
"{",
    "st_s path;",
    "func bcore_shell_op:key;",
    "func bcore_shell_op:info;",
    "func bcore_shell_op:run;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_shell_op_default_get_s )

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

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_shell_op_default_set_s )
"aware bcore_shell_op_default",
"{",
    "st_s path;",
    "st_s source;",
    "func bcore_shell_op:key;",
    "func bcore_shell_op:info;",
    "func bcore_shell_op:run;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_shell_op_default_set_s )

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

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_shell_op_default_alias_s )
"aware bcore_shell_op_default",
"{",
    "st_s key;",
    "st_s expression;",
    "func bcore_shell_op:key;",
    "func bcore_shell_op:info;",
    "func bcore_shell_op:run;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_shell_op_default_alias_s )

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

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_shell_op_default_stdin_s )
"aware bcore_shell_op_default",
"{",
    "func bcore_shell_op:key;",
    "func bcore_shell_op:info;",
    "func bcore_shell_op:run;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_shell_op_default_stdin_s )

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_shell_op_default_stdout_s )
"aware bcore_shell_op_default",
"{",
    "func bcore_shell_op:key;",
    "func bcore_shell_op:info;",
    "func bcore_shell_op:run;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_shell_op_default_stdout_s )

XOILA_DEFINE_SPECT_NASC_BEGIN( bcore_shell, bcore_shell_op_default )
"{",
    "bcore_spect_header_s header;",
"}",
XOILA_DEFINE_SPECT_NASC_END( bcore_shell, bcore_shell_op_default )

/**********************************************************************************************************************/
// source: bcore_huffman.h
#include "bcore_huffman.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_huffman; embeds: bcore_huffman.x

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_huffman_codec_s )
"aware bcore_huffman",
"{",
    "bcore_huffman_hist_s => hist;",
    "bcore_huffman_count_map_s => count_map;",
    "bcore_huffman_tree_s => tree;",
    "bcore_huffman_index_s => leaf_index;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_huffman_codec_s )

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

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_huffman_bit_buffer_s )
"aware x_array",
"{",
    "u3_t bits;",
    "u0_t [];",
    "func x_bbml:bbml_body_to_sink;",
    "func x_bbml:bbml_body_from_source;",
    "func x_bcml:bcml_body_to_sink;",
    "func x_bcml:bcml_body_from_source;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_huffman_bit_buffer_s )

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

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_huffman_bit_buffer_iterator_s )
"aware bcore_huffman",
"{",
    "hidden bcore_huffman_bit_buffer_s* bit_buffer;",
    "sz_t bit_index;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_huffman_bit_buffer_iterator_s )

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

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_huffman_hist_s )
"aware bcore_huffman",
"{",
    "bcore_hmap_tpuz_s hmap_tpuz;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_huffman_hist_s )

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_huffman_index_s )
"aware bcore_huffman",
"{",
    "bcore_hmap_tpuz_s hmap_tpuz;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_huffman_index_s )

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_huffman_node_s )
"bcore_huffman",
"{",
    "u3_t c;",
    "uz_t p;",
    "uz_t b0;",
    "uz_t b1;",
    "u2_t v;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_huffman_node_s )

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_huffman_count_node_s )
"bcore_huffman",
"{",
    "u3_t c;",
    "u2_t v;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_huffman_count_node_s )

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_huffman_count_map_s )
"aware x_array",
"{",
    "bcore_huffman_count_node_s [];",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_huffman_count_map_s )

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

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_huffman_tree_s )
"aware x_array",
"{",
    "bcore_huffman_node_s [];",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_huffman_tree_s )

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

XOILA_DEFINE_SPECT_NASC_BEGIN( x_inst, bcore_huffman )
"{",
    "bcore_spect_header_s header;",
"}",
XOILA_DEFINE_SPECT_NASC_END( x_inst, bcore_huffman )

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

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_indexer_io_s )
"bcore_indexer",
"{",
    "s3_t i;",
    "s3_t o;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_indexer_io_s )

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_indexer_io_arr_s )
"aware x_array",
"{",
    "bcore_indexer_io_s [];",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_indexer_io_arr_s )

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_indexer_cs_s )
"bcore_indexer",
"{",
    "s3_t c;",
    "s3_t s;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_indexer_cs_s )

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_indexer_cs_arr_s )
"aware x_array",
"{",
    "bcore_indexer_cs_s [];",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_indexer_cs_arr_s )

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( bcore_indexer_s )
"aware bcore_indexer",
"{",
    "bcore_indexer_io_arr_s io_arr;",
    "bcore_indexer_cs_arr_s cs_arr;",
    "s0_t mask_bits;",
    "s3_t size;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( bcore_indexer_s )

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

XOILA_DEFINE_SPECT_NASC_BEGIN( x_inst, bcore_indexer )
"{",
    "bcore_spect_header_s header;",
"}",
XOILA_DEFINE_SPECT_NASC_END( x_inst, bcore_indexer )

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
// source: bcore_parse.h
#include "bcore_parse.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_parse; embeds: bcore_parse.x

XOILA_DEFINE_SPECT_NASC_BEGIN( x_inst, bcore_parse )
"{",
    "bcore_spect_header_s header;",
"}",
XOILA_DEFINE_SPECT_NASC_END( x_inst, bcore_parse )

er_t bcore_parse_number_literal( x_source* source, sr_s* sr )
{
    // bcore_parse.x:24:1
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
    
    f3_t factor = 1.0;
    bl_t use_suffix = true;
    
    char c = x_source_inspect_char(source);
    switch( c )
    {
        case 'd': factor = 1E-1;  break; // deci
        case 'c': factor = 1E-2;  break; // centi
        case 'm': factor = 1E-3;  break; // milli
        case 'u': factor = 1E-6;  break; // micro
        case 'n': factor = 1E-9;  break; // nano
        case 'p': factor = 1E-12; break; // pico
        case 'f': factor = 1E-15; break; // femto
        case 'a': factor = 1E-18; break; // atto
        case 'z': factor = 1E-21; break; // zepto
        case 'y': factor = 1E-24; break; // yocto
        case 'r': factor = 1E-27; break; // ronto
        case 'q': factor = 1E-30; break; // quecto
    
        case 'D': factor = 1E+1;  break; // deca
        case 'C': factor = 1E+2;  break; // cento
        case 'K': factor = 1E+3;  break; // kilo
        case 'M': factor = 1E+6;  break; // mega
        case 'G': factor = 1E+9;  break; // giga
        case 'T': factor = 1E+12; break; // tera
        case 'P': factor = 1E+15; break; // peta
        case 'X': factor = 1E+18; break; // exa
        case 'Z': factor = 1E+21; break; // zetta
        case 'Y': factor = 1E+24; break; // yotta
        case 'R': factor = 1E+27; break; // ronna
        case 'Q': factor = 1E+30; break; // quetta
    
        default: use_suffix = false; break;
    }
    
    if( use_suffix ) x_source_get_char(source);
    
    if( is_float )
    {
        f3_t f3 = 0;
        st_s_parse_fa(&(st),0, -1, "#<f3_t*>", (&(f3)) );
        sr_s_const_from_f3(sr,f3 * factor );
    }
    else
    {
        s3_t s3 = 0;
        st_s_parse_fa(&(st),0, -1, "#<s3_t*>", (&(s3)) );
        if( use_suffix )
        {
            sr_s_const_from_f3(sr,s3 * factor );
        }
        else
        {
            sr_s_const_from_s3(sr,s3 );
        }
    }
    
    BLM_RETURNV(er_t, 0)
}

/**********************************************************************************************************************/
// source: bcore_x_btml.h
#include "bcore_x_btml.h"

//----------------------------------------------------------------------------------------------------------------------
// group: x_btml

XOILA_DEFINE_SPECT_NASC_BEGIN( x_inst, x_btml )
"{",
    "bcore_spect_header_s header;",
    "feature aware x_btml : btml_body_from_source;",
    "feature aware x_btml : btml_body_to_sink;",
"}",
XOILA_DEFINE_SPECT_NASC_END( x_inst, x_btml )

er_t x_btml_t_to_file( const x_btml* o, tp_t t, sc_t file )
{
    // bcore_x_btml.h:96:1
    BLM_INIT_LEVEL(0);
    x_sink* sink =((x_sink*)( ((bcore_sink*)BLM_LEVEL_A_PUSH(0,bcore_file_try_open_sink(file )))));
    if( !sink ) BLM_RETURNV(er_t, GERR_fa("Could not open '#<sc_t>'\n", file ))
    x_btml_t_to_sink(o,t,((x_sink*)( ((bcore_sink*)BLM_LEVEL_A_PUSH(0,bcore_file_open_sink(file ))) )));
    BLM_RETURNV(er_t, 0)
}

er_t x_btml_t_from_source( x_btml* o, tp_t t, x_source* source )
{
    // bcore_x_btml.h:120:1
    
    sr_s sr = sr_null();
    BLM_TRY(x_btml_parse_create_object(source, NULL, 0, (&(sr)) ))
    BLM_TRY(x_inst_t_copy_typed(((x_inst*)( o)),t, sr_s_o_type( (&(sr)) ), sr.o ))
    sr_s_down(&( sr ));
    return  0;
}

x_btml* x_btml_create_from_source_t( x_source* source, tp_t* type )
{
    // bcore_x_btml.h:131:1
    
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
    // bcore_x_btml.h:146:1
    
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
    // bcore_x_btml.h:159:1
    
    x_btml_t_translate_recursive(o,t, 0, true, sink, 0 );
    return sink;
}

st_s* x_btml_parse_name( x_source* source )
{
    // bcore_x_btml.h:175:1
    BLM_INIT_LEVEL(0);
    st_s* st_name = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
    BLM_TRY_EXIT(x_source_parse_fa(source," #name", st_name ))
    BLM_RETURNV(st_s*, ((st_s*)bcore_fork(st_name)))
}

sc_t x_btml_name_of( tp_t type, st_s* buf )
{
    // bcore_x_btml.h:184:1
    
    sc_t n = bcore_name_try_name(type );
    if( n ) return  n;
    BLM_TRY_EXIT(st_s_copy_typed(buf,((tp_t)(TYPEOF_tp_t)), (&(type)) ))
    return  buf->sc;
}

tp_t x_btml_type_of( const st_s* name )
{
    // bcore_x_btml.h:194:1
    
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
    // bcore_x_btml.h:214:1
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
    // bcore_x_btml.h:244:1
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
    // bcore_x_btml.h:272:1
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
    
        if( type_string->size == 0 && !x_source_parse_bl(source," #=?'</>'" ) )
        {
            if( default_tp == 0 ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Context does not provide an inheritable type." ))
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
    // bcore_x_btml.h:412:1
    
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
                BLM_TRY(x_stamp_t_set_sr(stamp,t, tp_name, sr ))
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
    // bcore_x_btml.h:555:1
    
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
    // bcore_x_btml.h:580:1
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
    // bcore_x_btml.h:692:1
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
    // bcore_x_btml.h:726:1
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

XOILA_DEFINE_SPECT_NASC_BEGIN( x_inst, x_bbml )
"{",
    "bcore_spect_header_s header;",
    "feature aware x_bbml : bbml_body_from_source;",
    "feature aware x_bbml : bbml_body_to_sink;",
"}",
XOILA_DEFINE_SPECT_NASC_END( x_inst, x_bbml )

er_t x_bbml_t_from_source( x_bbml* o, tp_t t, x_source* source )
{
    // bcore_x_bbml.h:116:1
    
    sr_s sr = sr_null();
    BLM_TRY(x_bbml_parse_create_object(source, (&(sr)) ))
    BLM_TRY(x_inst_t_copy_typed(((x_inst*)( o)),t, sr_s_o_type( (&(sr)) ), sr.o ))
    sr_s_down(&( sr ));
    return  bcore_error_last();
}

x_bbml* x_bbml_create_from_source_t( x_source* source, tp_t* type )
{
    // bcore_x_bbml.h:127:1
    
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
    // bcore_x_bbml.h:142:1
    
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
    // bcore_x_bbml.h:155:1
    
    x_bbml_t_translate_recursive(o,t, 0, true, sink );
    return sink;
}

bl_t x_bbml_appears_valid( x_source* source )
{
    // bcore_x_bbml.h:172:1
    
    sz_t index = x_source_get_index(source);
    tp_t type = x_bbml_parse_type(source );
    bl_t valid = bcore_flect_exists( type );
    x_source_set_index(source,index );
    return  valid;
}

er_t x_bbml_parse_create_object( x_source* source, sr_s* obj )
{
    // bcore_x_bbml.h:186:1
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
    // bcore_x_bbml.h:218:1
    
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
    // bcore_x_bbml.h:245:1
    
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
                BLM_TRY(x_stamp_t_set_sr(stamp,t, tp_name, sr ))
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
    // bcore_x_bbml.h:333:1
    
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
    // bcore_x_bbml.h:357:1
    
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
    // bcore_x_bbml.h:425:1
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
    // bcore_x_bbml.h:460:1
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

XOILA_DEFINE_SPECT_NASC_BEGIN( x_inst, x_bcml )
"{",
    "bcore_spect_header_s header;",
    "feature aware x_bcml : bcml_body_from_source;",
    "feature aware x_bcml : bcml_body_to_sink;",
"}",
XOILA_DEFINE_SPECT_NASC_END( x_inst, x_bcml )

er_t x_bcml_t_from_source( x_bcml* o, tp_t t, x_source* source )
{
    // bcore_x_bcml.h:113:1
    
    sr_s sr = sr_null();
    BLM_TRY(x_bcml_parse_create_object(true, 0, source, (&(sr)) ))
    BLM_TRY(x_inst_t_copy_typed(((x_inst*)( o)),t, sr_s_o_type( (&(sr)) ), sr.o ))
    sr_s_down(&( sr ));
    return  bcore_error_last();
}

x_bcml* x_bcml_create_from_source_t( x_source* source, tp_t* type )
{
    // bcore_x_bcml.h:124:1
    
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
    // bcore_x_bcml.h:139:1
    
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
    // bcore_x_bcml.h:152:1
    
    x_bcml_t_translate_recursive(o,t, true, true, true, sink );
    return sink;
}

bl_t x_bcml_appears_valid( x_source* source )
{
    // bcore_x_bcml.h:169:1
    
    sz_t index = x_source_get_index(source);
    tp_t type = x_bcml_parse_type(source );
    bl_t valid = bcore_flect_exists( type );
    x_source_set_index(source,index );
    return  valid;
}

er_t x_bcml_parse_create_object( bl_t parse_existence, tp_t given_type, x_source* source, sr_s* obj )
{
    // bcore_x_bcml.h:183:1
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
    // bcore_x_bcml.h:215:1
    
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
    // bcore_x_bcml.h:242:1
    
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
    // bcore_x_bcml.h:269:1
    
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
            BLM_TRY(x_stamp_t_set_sr_i(stamp,t, i, sr ))
        }
    }
    
    x_stamp_t_source(stamp,t, source );
    x_stamp_t_mutated(stamp,t );
    
    return  0;
}

void x_bcml_push_leaf( x_sink* sink, tp_t t, const x_inst* leaf )
{
    // bcore_x_bcml.h:345:1
    
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
    // bcore_x_bcml.h:369:1
    
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
    // bcore_x_bcml.h:393:1
    
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
    // bcore_x_bcml.h:465:1
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
    // bcore_x_bcml.h:500:1
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
// group: x_btcl; embeds: bcore_x_btcl.x bcore_x_btcl_builtin.x bcore_x_btcl_op.x

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_btcl_s )
"aware x_btcl",
"{",
    "bcore_arr_st_s => eval_path_arr;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_btcl_s )

er_t x_btcl_s_t_from_source( const x_btcl_s* o, x_btcl* obj, tp_t t, x_source* source )
{
    // bcore_x_btcl.h:211:1
    BLM_INIT_LEVEL(0);
    sr_s* sr = ((sr_s*)BLM_LEVEL_T_PUSH(0,sr_s,sr_s_create()));
    BLM_TRY(x_btcl_s_parse_create_object(o,source, sr ))
    BLM_TRY(x_inst_t_copy_typed(((x_inst*)( obj)),t, sr_s_o_type( sr ), sr->o ))
    BLM_RETURNV(er_t, 0)
}

x_btcl* x_btcl_s_create_from_source_t( const x_btcl_s* o, x_source* source, tp_t* type )
{
    // bcore_x_btcl.h:221:1
    BLM_INIT_LEVEL(0);
    (*(type)) = 0;
    if( !source ) BLM_RETURNV(x_btcl*, NULL)
    sr_s* sr = ((sr_s*)BLM_LEVEL_T_PUSH(0,sr_s,sr_s_create()));
    if( x_btcl_s_parse_create_object(o,source, sr ) )
    {
        BLM_RETURNV(x_btcl*, NULL)
    }
    if( sr->o && type ) (*(type)) = sr_s_o_type(sr);
    x_inst* obj = sr->o;
    BLM_RETURNV(x_btcl*,((x_btcl*)( ( obj ) ? ((x_inst*)bcore_fork(obj)) : NULL))) // sr.o is NULL in case of error
}

x_btcl* x_btcl_s_create_from_source( const x_btcl_s* o, x_source* source )
{
    // bcore_x_btcl.h:237:1
    
    tp_t t = 0;
    x_btcl* obj = x_btcl_s_create_from_source_t(o,source, (&(t)) );
    if( t )
    {
        ASSERT( x_stamp_t_is_aware(t ) );
    }
    return  obj;
}

er_t x_btcl_s_run_from_source( const x_btcl_s* o, x_source* source )
{
    // bcore_x_btcl.h:250:1
    BLM_INIT_LEVEL(0);
    if( !source ) BLM_RETURNV(er_t, 0)
    BLM_RETURNV(er_t, x_btcl_s_parse_create_object(o,source, ((sr_s*)BLM_LEVEL_T_PUSH(0,sr_s,sr_s_create())) ))
}

er_t x_btcl_s_parse_create_object( const x_btcl_s* o, x_source* source, sr_s* obj )
{
    // bcore_x_btcl.x:1478:1
    BLM_INIT_LEVEL(0);
    x_btcl_context_s* context = x_btcl_context_s_setup(((x_btcl_context_s*)BLM_LEVEL_T_PUSH(0,x_btcl_context_s,x_btcl_context_s_create())));
    bcore_arr_st_s_attach( &(context->eval_path_arr ),  ((bcore_arr_st_s*)bcore_fork(o->eval_path_arr)));
    x_btcl_frame_s* frame = x_btcl_frame_s_setup_as_root(((x_btcl_frame_s*)BLM_LEVEL_T_PUSH(0,x_btcl_frame_s,x_btcl_frame_s_create())),context );
    BLM_RETURNV(er_t, x_btcl_frame_s_parse_create_final_object(frame,source, obj ))
}

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_btcl_context_s )
"aware x_btcl",
"{",
    "bcore_arr_st_s => eval_path_arr;",
    "bcore_hmap_name_s hmap_reserved_key;",
    "bcore_hmap_name_s hmap_reserved_func;",
    "bcore_hmap_name_s hmap_reserved_const;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_btcl_context_s )

x_btcl_context_s* x_btcl_context_s_setup( x_btcl_context_s* o )
{
    // bcore_x_btcl.x:239:5
    
    x_btcl_context_s_set_reserved_keys(o);
    x_btcl_context_s_set_reserved_funcs(o);
    x_btcl_context_s_set_reserved_consts(o);
    return o;
}

x_btcl_context_s* x_btcl_context_s_set_reserved_keys( x_btcl_context_s* o )
{
    // bcore_x_btcl.x:248:5
    
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_key),"if" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_key),"else" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_key),"self" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_key),"func" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_key),"embed" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_key),"eval_file" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_key),"eval_string" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_key),"embed_file" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_key),"embed_string" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_key),"prefix" );
    return o;
}

bl_t x_btcl_context_s_is_reserved( const x_btcl_context_s* o, tp_t name )
{
    // bcore_x_btcl.x:271:5
    
    return  x_btcl_context_s_is_reserved_key(o,name ) ||
    x_btcl_context_s_is_reserved_func(o,name ) ||
    x_btcl_context_s_is_reserved_const(o,name );
}

sc_t x_btcl_context_s_sc_reserved( const x_btcl_context_s* o, tp_t name )
{
    // bcore_x_btcl.x:278:5
    
    if( x_btcl_context_s_is_reserved_key(o,name ) ) return  x_btcl_context_s_sc_reserved_key(o,name );
    if( x_btcl_context_s_is_reserved_func(o,name ) ) return  x_btcl_context_s_sc_reserved_func(o,name );
    if( x_btcl_context_s_is_reserved_const(o,name ) ) return  x_btcl_context_s_sc_reserved_const(o,name );
    return  NULL;
}

er_t x_btcl_context_s_check_reserved( const x_btcl_context_s* o, tp_t name, x_source* source )
{
    // bcore_x_btcl.x:286:5
    
    if( x_btcl_context_s_is_reserved(o,name ) ) return  x_source_parse_error_fa(source,"#<sc_t> is a reserved keyword or function.\n", x_btcl_context_s_sc_reserved(o,name ) );
    return  0;
}

er_t x_btcl_context_s_get_embedding_file_path( const x_btcl_context_s* o, x_source* source, sc_t in_path, st_s* out_path )
{
    // bcore_x_btcl.x:294:1
    BLM_INIT_LEVEL(0);
    st_s* try_path = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
    
    if( in_path[ 0 ] == '/' )
    {
        st_s_copy_sc(try_path,in_path );
    }
    else
    {BLM_INIT_LEVEL(1);
        st_s* folder = ((st_s*)BLM_LEVEL_T_PUSH(1,st_s,bcore_file_folder_path(x_source_get_file(source) )));
        if( folder->size == 0 ) st_s_push_char(folder,'.' );
        st_s_copy_fa(try_path,"#<sc_t>/#<sc_t>", folder->sc, in_path );
    
        if( !bcore_file_exists(try_path->sc ) )
        {
            if( o->eval_path_arr )
            {
                {const bcore_arr_st_s* __a=o->eval_path_arr ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){const st_s* st=__a->data[__i];
                {
                    st_s_copy_fa(try_path,"#<sc_t>/#<sc_t>", st->sc, in_path );
                    if( bcore_file_exists(try_path->sc ) ) break;
                }
            }}}
        }
    BLM_DOWN();}
    
    if( !bcore_file_exists(try_path->sc ) )
    {
        BLM_RETURNV(er_t, x_source_parse_error_fa(source,"embed: File '#<sc_t>' not found.", in_path ))
    }
    
    st_s_copy(out_path,try_path );
    
    BLM_RETURNV(er_t, 0)
}

x_btcl_context_s* x_btcl_context_s_set_reserved_funcs( x_btcl_context_s* o )
{
    // bcore_x_btcl_builtin.x:67:1
    
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
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_func),"MAX" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_func),"MIN" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_func),"IFE" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_func),"PRINT" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_func),"PRINTLN" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_func),"PRINTX" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_func),"ASSERT" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_func),"MKDIR" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_func),"RMDIR" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_func),"FILE_EXISTS" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_func),"TO_FILE" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_func),"TO_FILE_BTML" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_func),"TO_FILE_BBML" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_func),"TO_FILE_BCML" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_func),"FROM_FILE" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_func),"FROM_FILE_BTML" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_func),"FROM_FILE_BBML" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_func),"FROM_FILE_BCML" );
    return o;
}

x_btcl_context_s* x_btcl_context_s_set_reserved_consts( x_btcl_context_s* o )
{
    // bcore_x_btcl_builtin.x:105:1
    
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_const),"true" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_const),"false" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_const),"TRUE" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_const),"FALSE" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_const),"PI" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_const),"PATH" );
    bcore_hmap_name_s_set_sc(&(o->hmap_reserved_const),"DIR" );
    return o;
}

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_btcl_frame_s )
"aware x_btcl",
"{",
    "private x_btcl_frame_s* parent;",
    "x_btcl_context_s -> context;",
    "sz_t depth;",
    "sz_t eval_depth;",
    "hidden bcore_arr_sr_s obj_pool;",
    "bcore_hmap_name_s hmap_name;",
    "hidden bcore_hmap_tp_sr_s var_map;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_btcl_frame_s )

x_btcl_frame_s* x_btcl_frame_s_setup_as_root( x_btcl_frame_s* o, x_btcl_context_s* context )
{
    // bcore_x_btcl.x:378:5
    
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
    // bcore_x_btcl.x:393:5
    
    if( !parent ) ERR_fa( "No parent: Call setup_as_root\n" );
    o->parent = ((x_btcl_frame_s*)(parent));
    x_btcl_context_s_attach( &(o->context ),  ((x_btcl_context_s*)bcore_fork(o->parent->context)));
    o->depth = o->parent->depth + 1;
    return o;
}

x_btcl_frame_s* x_btcl_frame_s_preserve_and_set_weak( x_btcl_frame_s* o, sr_s* sr )
{
    // bcore_x_btcl.x:403:5
    
    bcore_arr_sr_s_push_sr(&(o->obj_pool),*(sr ));
    sr_s_set_strong(sr,false );
    return o;
}

sc_t x_btcl_frame_s_nameof( const x_btcl_frame_s* o, tp_t type )
{
    // bcore_x_btcl.x:412:5
    
    sc_t name = bcore_hmap_name_s_get_sc(&(o->hmap_name),type ); return  name ? name : bnameof( type );
    if( !name && o->parent ) name = x_btcl_frame_s_nameof(o->parent,type );
    if( !name && x_btcl_context_s_is_reserved(o->context,type ) ) name = x_btcl_context_s_sc_reserved(o->context,type );
    if( !name ) name = bnameof( type );
    return  name;
}

bl_t x_btcl_frame_s_var_exists( const x_btcl_frame_s* o, tp_t name )
{
    // bcore_x_btcl.x:423:5
    
    if( bcore_hmap_tp_sr_s_exists(&(o->var_map),name ) ) return  true;
    return  o->parent ? x_btcl_frame_s_var_exists(o->parent,name ) : false;
}

sr_s* x_btcl_frame_s_var_get( const x_btcl_frame_s* o, tp_t name )
{
    // bcore_x_btcl.x:429:5
    
    sr_s* sr = bcore_hmap_tp_sr_s_get(&(o->var_map),name );
    if( sr ) return  sr;
    return  o->parent ? x_btcl_frame_s_var_get(o->parent,name ) : NULL;
}

sr_s* x_btcl_frame_s_var_set( x_btcl_frame_s* o, tp_t name, sr_s sr )
{
    // bcore_x_btcl.x:436:5
    
    return  bcore_hmap_tp_sr_s_set(&(o->var_map),name, sr );
}

x_btcl_frame_s* x_btcl_frame_s_import_forked_vars( x_btcl_frame_s* o, tp_t prefix, x_btcl_frame_s* src_frame )
{
    // bcore_x_btcl.x:442:5
    
    sz_t size = src_frame->var_map.size;
    for(sz_t i = 0; i < size; i++ )
    {
        tp_t key = bcore_hmap_tp_sr_s_idx_key(&(src_frame->var_map),i );
        if( key )
        {
            tp_t name = key;
            if( prefix )
            {BLM_INIT_LEVEL(4);
                sc_t prefix_name = x_btcl_frame_s_nameof(o,prefix );
                name = x_btcl_frame_s_entypeof(o,st_s_copy_fa(((st_s*)BLM_LEVEL_T_PUSH(4,st_s,st_s_create())),"#<sc_t>_#<sc_t>", prefix_name, x_btcl_frame_s_nameof(src_frame,key ) )->sc ) ;
            BLM_DOWN();}
            const sr_s* src_sr = bcore_hmap_tp_sr_s_idx_val(&(src_frame->var_map),i );
            x_btcl_frame_s_var_set(o,name, sr_tsm(sr_s_type(src_sr), ((x_inst*)bcore_fork(src_sr->o)) ) );
        }
    }
    return o;
}

er_t x_btcl_frame_s_generic_copy( x_btcl_frame_s* o, const x_source_point_s* sp, sr_s* sr, const sr_s* sb )
{
    // bcore_x_btcl.x:463:5
    
    /// convert functions into functor
    if( sr_s_type(sb) == ((tp_t)(TYPEOF_x_btcl_function_s)) )
    {BLM_INIT_LEVEL(1);
        x_btcl_function_s* function = ((x_btcl_function_s*)(sb->o));
        x_btcl_functor_s* functor = ((x_btcl_functor_s*)BLM_LEVEL_T_PUSH(1,x_btcl_functor_s,x_btcl_functor_s_create()));
        BLM_TRY(x_btcl_functor_s_setup(functor,sp, function, o ))
        sr_s functor_sr = sr_awc(((const x_inst*)( functor )));
        BLM_TRY(x_btcl_generic_copy(sr,&( functor_sr )))
    BLM_DOWN();}
    else
    {
        BLM_TRY(x_btcl_generic_copy(sr, sb ))
    }
    return  0;
}

tp_t x_btcl_frame_s_get_identifier( x_btcl_frame_s* o, x_source* source, bl_t take_from_source )
{
    // bcore_x_btcl.x:906:1
    
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
    // bcore_x_btcl.x:924:1
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
    
    f3_t factor = 1.0;
    bl_t use_suffix = true;
    
    char c = x_source_inspect_char(source);
    switch( c )
    {
        case 'd': factor = 1E-1;  break; // deci
        case 'c': factor = 1E-2;  break; // centi
        case 'm': factor = 1E-3;  break; // milli
        case 'u': factor = 1E-6;  break; // micro
        case 'n': factor = 1E-9;  break; // nano
        case 'p': factor = 1E-12; break; // pico
        case 'f': factor = 1E-15; break; // femto
        case 'a': factor = 1E-18; break; // atto
        case 'z': factor = 1E-21; break; // zepto
        case 'y': factor = 1E-24; break; // yocto
        case 'r': factor = 1E-27; break; // ronto
        case 'q': factor = 1E-30; break; // quecto
    
        case 'D': factor = 1E+1;  break; // deca
        case 'C': factor = 1E+2;  break; // cento
        case 'K': factor = 1E+3;  break; // kilo
        case 'M': factor = 1E+6;  break; // mega
        case 'G': factor = 1E+9;  break; // giga
        case 'T': factor = 1E+12; break; // tera
        case 'P': factor = 1E+15; break; // peta
        case 'X': factor = 1E+18; break; // exa
        case 'Z': factor = 1E+21; break; // zetta
        case 'Y': factor = 1E+24; break; // yotta
        case 'R': factor = 1E+27; break; // ronna
        case 'Q': factor = 1E+30; break; // quetta
    
        default: use_suffix = false; break;
    }
    
    if( use_suffix ) x_source_get_char(source);
    
    if( is_float )
    {
        f3_t f3 = 0;
        st_s_parse_fa(&(st),0, -1, "#<f3_t*>", (&(f3)) );
        sr_s_const_from_f3(sr,f3 * factor );
    }
    else
    {
        s3_t s3 = 0;
        st_s_parse_fa(&(st),0, -1, "#<s3_t*>", (&(s3)) );
        if( use_suffix )
        {
            sr_s_const_from_f3(sr,s3 * factor );
        }
        else
        {
            sr_s_const_from_s3(sr,s3 );
        }
    }
    
    BLM_RETURNV(er_t, 0)
}

er_t x_btcl_frame_s_eval_condition( x_btcl_frame_s* o, x_source* source, bl_t* condition )
{
    // bcore_x_btcl.x:1045:1
    BLM_INIT_LEVEL(0);
    sr_s* sb = ((sr_s*)BLM_LEVEL_T_PUSH(0,sr_s,sr_s_create()));
    BLM_TRY(x_btcl_frame_s_eval(o,0, source, sb ))
    if( !sr_s_is_numeric(sb) )  BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Internal error: Expression does not evaluate to a condition.\n" ))
    if( condition ) (*(condition)) = ( sr_s_to_f3(sb) != 0 );
    BLM_RETURNV(er_t, 0)
}

er_t x_btcl_frame_s_eval_in_frame( const x_btcl_frame_s* o, s2_t priority, x_source* source, sr_s* obj )
{
    // bcore_x_btcl.x:1088:1
    BLM_INIT_LEVEL(0);
    x_btcl_frame_s* frame = x_btcl_frame_s_setup(((x_btcl_frame_s*)BLM_LEVEL_T_PUSH(0,x_btcl_frame_s,x_btcl_frame_s_create())),o );
    BLM_TRY(x_btcl_frame_s_eval(frame,priority, source, obj ))
    x_btcl_clone_if_weak(obj );
    BLM_RETURNV(er_t, 0)
}

er_t x_btcl_frame_s_eval( x_btcl_frame_s* o, s2_t exit_priority, x_source* source, sr_s* obj )
{
    // bcore_x_btcl.x:1098:1
    
    o->eval_depth++;
    
    if( o->eval_depth >= x_btcl_max_frame_eval_depth() ) return  x_source_parse_error_fa(source,"Evaluation depth exceeded. Check for unguarded recursive expressions.\n" );
    
    ASSERT( obj->o == NULL );
    
    /// prefix operators
    if     ( x_source_parse_bl(source," #?'+'" ) ) BLM_TRY(x_btcl_operator_eval_uop_type(o, ((tp_t)(TYPEOF_identity)), x_btcl_priority_c(), source, false, obj ))
    else if( x_source_parse_bl(source," #?'-'" ) ) BLM_TRY(x_btcl_operator_eval_uop_type(o, ((tp_t)(TYPEOF_neg)),      x_btcl_priority_c(), source, false, obj ))
    else if( x_source_parse_bl(source," #?'!'" ) ) BLM_TRY(x_btcl_operator_eval_uop_type(o, ((tp_t)(TYPEOF_not)),      x_btcl_priority_c(), source, false, obj ))
    
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
    
    /// labels (hashed string)
    else if( x_source_parse_bl(source," #=?|'|" ) )
    {BLM_INIT_LEVEL(2);
        st_s* st = ((st_s*)BLM_LEVEL_T_PUSH(2,st_s,st_s_create()));
        BLM_TRY(x_source_parse_fa(source," #label", st ))
        sr_s_from_tp(obj,bentypeof( st->sc ) );
    BLM_DOWN();}
    
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
                case TYPEOF_embed_file:
                case TYPEOF_eval_file:
                {BLM_INIT_LEVEL(8);
                    sr_s* sb = ((sr_s*)BLM_LEVEL_T_PUSH(8,sr_s,sr_s_create()));
                    BLM_TRY(x_source_parse_fa(source," (" ))
                    BLM_TRY(x_btcl_frame_s_eval(o,0, source, sb ))
                    BLM_TRY(x_source_parse_fa(source," )" ))
                    if( sr_s_type(sb) != ((tp_t)(TYPEOF_st_s)) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Keyword 'eval_file': Expression must evaluate to a string, which can be used as file-path.\n" ))
                    st_s* path = ((st_s*)BLM_LEVEL_T_PUSH(8,st_s,st_s_create()));
                    BLM_TRY(x_btcl_context_s_get_embedding_file_path(o->context,source, ((const st_s*)(sb->o))->sc, path ))
                    x_source* emb_source =((x_source*)( ((bcore_source*)BLM_LEVEL_A_PUSH(8,bcore_file_open_source(path->sc )))));
                    BLM_TRY(x_btcl_frame_s_eval(o,0, emb_source, obj ))
    
                    BLM_TRY(x_source_parse_fa(emb_source," " ))
                    if( !x_source_eos(emb_source) ) BLM_TRY(x_source_parse_error_fa(emb_source,"Unexpected expression. Semicolon ';' missing on previous expression?\n" ))
                BLM_DOWN();}
                break;
    
                case TYPEOF_embed_string:
                case TYPEOF_eval_string:
                {BLM_INIT_LEVEL(7);
                    sr_s* sb = ((sr_s*)BLM_LEVEL_T_PUSH(7,sr_s,sr_s_create()));
                    BLM_TRY(x_source_parse_fa(source," (" ))
                    BLM_TRY(x_btcl_frame_s_eval(o,0, source, sb ))
                    BLM_TRY(x_source_parse_fa(source," )" ))
                    if( sr_s_type(sb) != ((tp_t)(TYPEOF_st_s)) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Keyword 'eval_string': Expression must evaluate to a string.\n" ))
                    x_source* emb_source = ((x_source*)BLM_LEVEL_A_PUSH(7,x_source_create_from_st(((const st_s*)(sb->o)) )));
                    BLM_TRY(x_btcl_frame_s_eval(o,0, emb_source, obj ))
                    BLM_TRY(x_source_parse_fa(emb_source," " ))
                    if( !x_source_eos(emb_source) ) BLM_TRY(x_source_parse_error_fa(emb_source,"Unexpected expression. Semicolon ';' missing on previous expression?\n" ))
                BLM_DOWN();}
                break;
    
                /// Prefixing
                case TYPEOF_prefix:
                {BLM_INIT_LEVEL(6);
                    BLM_TRY(x_source_parse_fa(source," (" ))
    
                    sr_s* result = ((sr_s*)BLM_LEVEL_T_PUSH(6,sr_s,sr_s_create()));
                    x_btcl_frame_s* frame = x_btcl_frame_s_setup(((x_btcl_frame_s*)BLM_LEVEL_T_PUSH(6,x_btcl_frame_s,x_btcl_frame_s_create())),o );
                    BLM_TRY(x_btcl_frame_s_eval(frame,0, source, result ))
    
                    st_s* st_prefix = ((st_s*)BLM_LEVEL_T_PUSH(6,st_s,st_s_create()));
                    tp_t prefix_name = 0;
    
                    if( sr_s_type(result) == ((tp_t)(TYPEOF_st_s)) )
                    {
                        st_s_copy(st_prefix,((const st_s*)(result->o)) );
                        if( st_prefix->size == 0 ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Keyword 'prefix': Prefix is the empty string.\n" ))
                        prefix_name = x_btcl_frame_s_entypeof(o,st_prefix->sc );
                    }
                    else if( sr_s_type(result) == ((tp_t)(TYPEOF_tp_t)) )
                    {
                        prefix_name = (*(((const tp_t*)(result->o))));
                        sc_t sc_prefix = bnameof( prefix_name );
                        if( !sc_prefix ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Keyword 'prefix': Prefix identifier is tp_t without a registered name.\n" ))
                        st_s_copy_sc(st_prefix,sc_prefix );
                    }
                    else
                    {
                        BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Keyword 'prefix': Prefix identifier must evaluate to a string or label.\n" ))
                    }
    
                    if( st_s_find_any_sc(st_prefix,0, st_prefix->size, " \t\r\n" ) < st_prefix->size )
                    {
                        BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Keyword 'prefix': Prefix contains whitespaces.\n" ))
                    }
    
                    BLM_TRY(x_source_parse_fa(source," ," ))
    
                    sr_s_clear(result);
    
                    BLM_TRY(x_btcl_frame_s_eval(frame,0, source, result ))
                    x_btcl_clone_if_weak(result );
    
                    x_btcl_frame_s_import_forked_vars(o,prefix_name, frame );
    
                    if( result->o )
                    {
                        x_btcl_frame_s_var_set(o,prefix_name, sr_tsm(sr_s_type(result), ((x_inst*)bcore_fork(result->o)) ) );
                    }
    
                    BLM_TRY(x_source_parse_fa(source," )" ))
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
            BLM_TRY(x_btcl_frame_s_eval_reserved_func(o,name, source, false, obj ))
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
    
    /// Exportable object
    else if( x_source_parse_bl(source," #?'@'" ) )
    {BLM_INIT_LEVEL(2);
        const x_source_point_s* sp = x_source_point_s_setup_from_source(((x_source_point_s*)BLM_LEVEL_T_PUSH(2,x_source_point_s,x_source_point_s_create())),source );
        if( x_source_parse_bl(source," #?'~'" ) ) // wire
        {
            tp_t rack_name = 0;
            if( x_btcl_is_identifier(source ) )
            {
                rack_name = bcore_name_enroll(x_btcl_frame_s_nameof(o,x_btcl_frame_s_get_identifier(o,source, true ) ) );
            }
    
            tp_t wire_name = 0;
            if( x_source_parse_bl(source," #?':' " ) )
            {
                if( !x_btcl_is_identifier(source ) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Socket identifier expected.\n" ))
                wire_name = bcore_name_enroll(x_btcl_frame_s_nameof(o,x_btcl_frame_s_get_identifier(o,source, true ) ) );
            }
    
            sr_s_asm(obj,x_btcl_net_node_s_setup_wire(x_btcl_net_node_s_create(),rack_name, wire_name, sp ) );
        }
        else
        {
            tp_t node_type = 0; // generic
            if( x_source_parse_bl(source," #?':'" ) ) node_type = ((tp_t)(TYPEOF_rack));
            tp_t node_name = 0;
            if( x_btcl_is_identifier(source ) )
            {
                node_name = bcore_name_enroll(x_btcl_frame_s_nameof(o,x_btcl_frame_s_get_identifier(o,source, true ) ) );
            }
    
            sr_s_asm(obj,x_btcl_net_node_s_setup(x_btcl_net_node_s_create(),node_type, node_name, sp ) );
        }
    BLM_DOWN();}
    else
    {
        return  x_source_parse_error_fa(source,"Expression does not evaluate to an object.\n" );
    }
    
    // operators (if any)
    BLM_TRY(x_btcl_frame_s_eval_op(o,exit_priority, source, obj ))
    
    o->eval_depth--;
    
    return  0;
}

er_t x_btcl_frame_s_parse_create_final_object( x_btcl_frame_s* o, x_source* source, sr_s* obj )
{
    // bcore_x_btcl.x:1429:1
    BLM_INIT_LEVEL(0);
    sr_s* sr = ((sr_s*)BLM_LEVEL_T_PUSH(0,sr_s,sr_s_create()));
    BLM_TRY(x_btcl_frame_s_eval(o,0, source, sr ))
    
    BLM_TRY(x_source_parse_fa(source," " ))
    
    if( !x_source_eos(source) )
    {
        BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Isolated expression: Operator or semicolon expected." ))
    }
    
    if( obj )
    {
        if( sr->o )
        {
            /// convert function into functor
            if( sr_s_type(sr) == ((tp_t)(TYPEOF_x_btcl_function_s)) )
            {BLM_INIT_LEVEL(3);
                x_btcl_functor_s* functor = ((x_btcl_functor_s*)BLM_LEVEL_T_PUSH(3,x_btcl_functor_s,x_btcl_functor_s_create()));
                BLM_TRY(x_btcl_functor_s_setup(functor,x_source_point_s_setup_from_source(((x_source_point_s*)BLM_LEVEL_T_PUSH(3,x_source_point_s,x_source_point_s_create())),source ), ((x_btcl_function_s*)(sr->o)), o ))
                sr_s_asm(sr,((x_btcl_functor_s*)bcore_fork(functor)) );
            BLM_DOWN();}
    
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
    // bcore_x_btcl_builtin.x:118:1
    
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
            if( dir->size == 0 ) st_s_push_fa(dir,"." );
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

er_t x_btcl_frame_s_eval_reserved_func( x_btcl_frame_s* o, tp_t name, x_source* source, bl_t postfix, sr_s* sr )
{
    // bcore_x_btcl_builtin.x:163:1
    
    switch( name )
    {
        case ((tp_t)(TYPEOF_SIZE)):  BLM_TRY(x_btcl_operator_eval_uop_type(o, ((tp_t)(TYPEOF_size)),  0, source, postfix, sr )) break;
        case ((tp_t)(TYPEOF_EXP)):   BLM_TRY(x_btcl_operator_eval_uop_type(o, ((tp_t)(TYPEOF_exp)),   0, source, postfix, sr )) break;
        case ((tp_t)(TYPEOF_LOG)):   BLM_TRY(x_btcl_operator_eval_uop_type(o, ((tp_t)(TYPEOF_log)),   0, source, postfix, sr )) break;
        case ((tp_t)(TYPEOF_LOG2)):  BLM_TRY(x_btcl_operator_eval_uop_type(o, ((tp_t)(TYPEOF_log2)),  0, source, postfix, sr )) break;
        case ((tp_t)(TYPEOF_LOG10)): BLM_TRY(x_btcl_operator_eval_uop_type(o, ((tp_t)(TYPEOF_log10)), 0, source, postfix, sr )) break;
        case ((tp_t)(TYPEOF_SIN)):   BLM_TRY(x_btcl_operator_eval_uop_type(o, ((tp_t)(TYPEOF_sin)),   0, source, postfix, sr )) break;
        case ((tp_t)(TYPEOF_COS)):   BLM_TRY(x_btcl_operator_eval_uop_type(o, ((tp_t)(TYPEOF_cos)),   0, source, postfix, sr )) break;
        case ((tp_t)(TYPEOF_TAN)):   BLM_TRY(x_btcl_operator_eval_uop_type(o, ((tp_t)(TYPEOF_tan)),   0, source, postfix, sr )) break;
        case ((tp_t)(TYPEOF_TANH)):  BLM_TRY(x_btcl_operator_eval_uop_type(o, ((tp_t)(TYPEOF_tanh)),  0, source, postfix, sr )) break;
        case ((tp_t)(TYPEOF_SIGN)):  BLM_TRY(x_btcl_operator_eval_uop_type(o, ((tp_t)(TYPEOF_sign)),  0, source, postfix, sr )) break;
        case ((tp_t)(TYPEOF_SQRT)):  BLM_TRY(x_btcl_operator_eval_uop_type(o, ((tp_t)(TYPEOF_sqrt)),  0, source, postfix, sr )) break;
        case ((tp_t)(TYPEOF_ABS)):   BLM_TRY(x_btcl_operator_eval_uop_type(o, ((tp_t)(TYPEOF_abs)),   0, source, postfix, sr )) break;
        case ((tp_t)(TYPEOF_CEIL)):  BLM_TRY(x_btcl_operator_eval_uop_type(o, ((tp_t)(TYPEOF_ceil)),  0, source, postfix, sr )) break;
        case ((tp_t)(TYPEOF_FLOOR)): BLM_TRY(x_btcl_operator_eval_uop_type(o, ((tp_t)(TYPEOF_floor)), 0, source, postfix, sr )) break;
    
        case ((tp_t)(TYPEOF_MAX)):
        {BLM_INIT_LEVEL(3);
            sr_s sa;BLM_T_INIT_SPUSH(sr_s, &sa);;
            if( postfix )
            {
                sr_s_twc(&(sa),sr_s_type(sr), sr->o );
            }
            else
            {
                BLM_TRY(x_btcl_frame_s_eval(o,0, source,&( sa )))
            }
            BLM_TRY(x_source_parse_fa(source," , " ))
            BLM_TRY(x_btcl_operator_eval_bop_type(o, ((tp_t)(TYPEOF_max)), 0, source,&( sa), sr ))
        BLM_DOWN();}
        break;
    
        case ((tp_t)(TYPEOF_MIN)):
        {BLM_INIT_LEVEL(3);
            sr_s sa;BLM_T_INIT_SPUSH(sr_s, &sa);;
            if( postfix )
            {
                sr_s_twc(&(sa),sr_s_type(sr), sr->o );
            }
            else
            {
                BLM_TRY(x_btcl_frame_s_eval(o,0, source,&( sa )))
            }
            BLM_TRY(x_source_parse_fa(source," , " ))
            BLM_TRY(x_btcl_operator_eval_bop_type(o, ((tp_t)(TYPEOF_min)), 0, source,&( sa), sr ))
        BLM_DOWN();}
        break;
    
        case ((tp_t)(TYPEOF_IFE)):
        {BLM_INIT_LEVEL(3);
            sr_s sa;BLM_T_INIT_SPUSH(sr_s, &sa);;
            if( postfix )
            {
                sr_s_twc(&(sa),sr_s_type(sr), sr->o );
            }
            else
            {
                BLM_TRY(x_btcl_frame_s_eval(o,0, source,&( sa )))
            }
            BLM_TRY(x_source_parse_fa(source," , " ))
    
            sr_s sb;BLM_T_INIT_SPUSH(sr_s, &sb);;
            BLM_TRY(x_btcl_frame_s_eval(o,0, source,&( sb )))
    
            BLM_TRY(x_source_parse_fa(source," , " ))
    
            sr_s sc;BLM_T_INIT_SPUSH(sr_s, &sc);;
            BLM_TRY(x_btcl_frame_s_eval(o,0, source,&( sc )))
    
    
            tp_t op_type = ((tp_t)(TYPEOF_conditional));
            x_btcl_operator_top_s* top = x_btcl_operator_top_s__(((x_btcl_operator_top_s*)BLM_LEVEL_T_PUSH(3,x_btcl_operator_top_s,x_btcl_operator_top_s_create())),op_type,&( sa),&( sb),&( sc), x_source_point_s__(((x_source_point_s*)BLM_LEVEL_T_PUSH(3,x_source_point_s,x_source_point_s_create())),source ) );
            bl_t success = false;
            BLM_TRY(x_btcl_operator_top_s_solve(top,o, sr,&( success )))
            if( !success )
            {
                BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Operator IFE( #<sc_t>, #<sc_t>, #<sc_t> ) is not defined.\n", bnameof( sr_s_type(&(sa)) ), bnameof( sr_s_type(&(sb)) ), bnameof( sr_s_type(&(sc)) ) ))
            }
        BLM_DOWN();}
        break;
    
        case ((tp_t)(TYPEOF_PRINT)):
        case ((tp_t)(TYPEOF_PRINTLN)):
        case ((tp_t)(TYPEOF_PRINTX)):
        {BLM_INIT_LEVEL(5);
            sr_s* sb = ((sr_s*)BLM_LEVEL_T_PUSH(5,sr_s,sr_s_create()));
            if( postfix )
            {
                (*(sb)) = (*(sr));
                (*(sr)) = sr_null();
            }
            else
            {
                sr_s_clear(sr);
            }
    
            BLM_TRY(x_btcl_frame_s_eval(o,0, source, sb ))
    
            st_s* st = ((st_s*)BLM_LEVEL_T_PUSH(5,st_s,st_s_create()));
            BLM_TRY(x_btcl_to_sink(( name == ((tp_t)(TYPEOF_PRINTX)) ), sb,((x_sink*)( st ))))
            if( name == ((tp_t)(TYPEOF_PRINTLN)) )
            {
                if( st->size == 0 || st->data[ st->size - 1 ] != '\n' ) st_s_push_char(st,'\n' );
            }
            st_s_to_stdout(st);
    
            (*(sr)) = (*(sb));
            (*(sb)) = sr_null();
        BLM_DOWN();}
        break;
    
        case ((tp_t)(TYPEOF_ASSERT)):
        {BLM_INIT_LEVEL(3);
            sr_s* sb = ((sr_s*)BLM_LEVEL_T_PUSH(3,sr_s,sr_s_create()));
            BLM_TRY(x_btcl_frame_s_eval(o,0, source, sb ))
    
            if( !sr_s_is_numeric(sb) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Expression does not represent a condition.\n" ))
            if( !sr_s_to_bl(sb)      ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Assertion failed.\n" ))
            sr_s_from_bl(sr,true );
        BLM_DOWN();}
        break;
    
        case ((tp_t)(TYPEOF_MKDIR)):
        {BLM_INIT_LEVEL(3);
            sr_s* sb = ((sr_s*)BLM_LEVEL_T_PUSH(3,sr_s,sr_s_create()));
            BLM_TRY(x_btcl_frame_s_eval(o,0, source, sb ))
            if( sr_s_type(sb) != ((tp_t)(TYPEOF_st_s)) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Expression must represent a string.\n" ))
            sr_s_from_bl(sr,bcore_folder_create(((const st_s*)(sb->o))->sc ) );
        BLM_DOWN();}
        break;
    
        case ((tp_t)(TYPEOF_RMDIR)):
        {BLM_INIT_LEVEL(3);
            sr_s* sb = ((sr_s*)BLM_LEVEL_T_PUSH(3,sr_s,sr_s_create()));
            BLM_TRY(x_btcl_frame_s_eval(o,0, source, sb ))
            if( sr_s_type(sb) != ((tp_t)(TYPEOF_st_s)) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Expression must represent a string.\n" ))
            sr_s_from_bl(sr,bcore_folder_delete(((const st_s*)(sb->o))->sc ) );
        BLM_DOWN();}
        break;
    
        case ((tp_t)(TYPEOF_FILE_EXISTS)):
        {BLM_INIT_LEVEL(3);
            sr_s* sb = ((sr_s*)BLM_LEVEL_T_PUSH(3,sr_s,sr_s_create()));
            BLM_TRY(x_btcl_frame_s_eval(o,0, source, sb ))
            if( sr_s_type(sb) != ((tp_t)(TYPEOF_st_s)) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Expression must represent a string.\n" ))
            sr_s_from_bl(sr,bcore_file_exists(((const st_s*)(sb->o))->sc ) );
        BLM_DOWN();}
        break;
    
        case ((tp_t)(TYPEOF_TO_FILE)):
        case ((tp_t)(TYPEOF_TO_FILE_BTML)):
        {BLM_INIT_LEVEL(4);
            sr_s* sa = ((sr_s*)BLM_LEVEL_T_PUSH(4,sr_s,sr_s_create()));
            BLM_TRY(x_btcl_frame_s_eval(o,0, source, sa ))
            if( sr_s_type(sa) != ((tp_t)(TYPEOF_st_s)) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"First argument must represent a string.\n" ))
    
            BLM_TRY(x_source_parse_fa(source," , " ))
    
            sr_s* sb = ((sr_s*)BLM_LEVEL_T_PUSH(4,sr_s,sr_s_create()));
            BLM_TRY(x_btcl_frame_s_eval(o,0, source, sb ))
            bl_t success = false;
    
            if( sb->o )
            {
                BLM_TRY(x_btml_t_to_file( ((const x_btml*)(sb->o)),sr_s_type(sb), ((const st_s*)(sa->o))->sc ))
                success = true;
            }
    
            sr_s_from_bl(sr,success );
        BLM_DOWN();}
        break;
    
        case ((tp_t)(TYPEOF_TO_FILE_BBML)):
        {BLM_INIT_LEVEL(3);
            sr_s* sa = ((sr_s*)BLM_LEVEL_T_PUSH(3,sr_s,sr_s_create()));
            BLM_TRY(x_btcl_frame_s_eval(o,0, source, sa ))
            if( sr_s_type(sa) != ((tp_t)(TYPEOF_st_s)) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"First argument must represent a string.\n" ))
    
            BLM_TRY(x_source_parse_fa(source," , " ))
    
            sr_s* sb = ((sr_s*)BLM_LEVEL_T_PUSH(3,sr_s,sr_s_create()));
            BLM_TRY(x_btcl_frame_s_eval(o,0, source, sb ))
            bl_t success = false;
    
            if( sb->o )
            {
                x_bbml_t_to_file(((const x_bbml*)( ((const x_btml*)(sb->o)))),sr_s_type(sb), ((const st_s*)(sa->o))->sc );
                success = true;
            }
    
            sr_s_from_bl(sr,success );
        BLM_DOWN();}
        break;
    
        case ((tp_t)(TYPEOF_TO_FILE_BCML)):
        {BLM_INIT_LEVEL(3);
            sr_s* sa = ((sr_s*)BLM_LEVEL_T_PUSH(3,sr_s,sr_s_create()));
            BLM_TRY(x_btcl_frame_s_eval(o,0, source, sa ))
            if( sr_s_type(sa) != ((tp_t)(TYPEOF_st_s)) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"First argument must represent a string.\n" ))
    
            BLM_TRY(x_source_parse_fa(source," , " ))
    
            sr_s* sb = ((sr_s*)BLM_LEVEL_T_PUSH(3,sr_s,sr_s_create()));
            BLM_TRY(x_btcl_frame_s_eval(o,0, source, sb ))
            bl_t success = false;
    
            if( sb->o )
            {
                x_bcml_t_to_file(((const x_bcml*)( ((const x_btml*)(sb->o)))),sr_s_type(sb), ((const st_s*)(sa->o))->sc );
                success = true;
            }
    
            sr_s_from_bl(sr,success );
        BLM_DOWN();}
        break;
    
        case ((tp_t)(TYPEOF_FROM_FILE)):
        {BLM_INIT_LEVEL(3);
            sr_s* sa = ((sr_s*)BLM_LEVEL_T_PUSH(3,sr_s,sr_s_create()));
            BLM_TRY(x_btcl_frame_s_eval(o,0, source, sa ))
            if( sr_s_type(sa) != ((tp_t)(TYPEOF_st_s)) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"First argument must represent a string.\n" ))
    
            BLM_TRY(x_source_parse_fa(source," , " ))
            BLM_TRY(x_btcl_frame_s_eval(o,0, source, sr ))
    
            x_source* file_source = ((x_source*)BLM_LEVEL_A_PUSH(3,x_source_create_from_file(((const st_s*)(sa->o))->sc )));
    
            if( file_source )
            {BLM_INIT_LEVEL(4);
                tp_t type = 0;
                x_inst* obj = ((x_inst*)BLM_LEVEL_A_PUSH(4,((x_inst*)(x_btcl_create_from_source_t(file_source,&( type ))))));
                if( obj ) sr_s_tsm(sr,type, ((x_inst*)bcore_fork(obj)) );
            BLM_DOWN();}
        BLM_DOWN();}
        break;
    
        case ((tp_t)(TYPEOF_FROM_FILE_BTML)):
        {BLM_INIT_LEVEL(3);
            sr_s* sa = ((sr_s*)BLM_LEVEL_T_PUSH(3,sr_s,sr_s_create()));
            BLM_TRY(x_btcl_frame_s_eval(o,0, source, sa ))
            if( sr_s_type(sa) != ((tp_t)(TYPEOF_st_s)) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"First argument must represent a string.\n" ))
            BLM_TRY(x_source_parse_fa(source," , " ))
            BLM_TRY(x_btcl_frame_s_eval(o,0, source, sr ))
    
            x_source* file_source = ((x_source*)BLM_LEVEL_A_PUSH(3,x_source_create_from_file(((const st_s*)(sa->o))->sc )));
    
            if( file_source )
            {BLM_INIT_LEVEL(4);
                tp_t type = 0;
                x_inst* obj = ((x_inst*)BLM_LEVEL_A_PUSH(4,((x_inst*)(x_btml_create_from_source_t(file_source,&( type ))))));
                if( obj ) sr_s_tsm(sr,type, ((x_inst*)bcore_fork(obj)) );
            BLM_DOWN();}
        BLM_DOWN();}
        break;
    
        case ((tp_t)(TYPEOF_FROM_FILE_BBML)):
        {BLM_INIT_LEVEL(3);
            sr_s* sa = ((sr_s*)BLM_LEVEL_T_PUSH(3,sr_s,sr_s_create()));
            BLM_TRY(x_btcl_frame_s_eval(o,0, source, sa ))
            if( sr_s_type(sa) != ((tp_t)(TYPEOF_st_s)) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"First argument must represent a string.\n" ))
            BLM_TRY(x_source_parse_fa(source," , " ))
            BLM_TRY(x_btcl_frame_s_eval(o,0, source, sr ))
    
            x_source* file_source = ((x_source*)BLM_LEVEL_A_PUSH(3,x_source_create_from_file(((const st_s*)(sa->o))->sc )));
            if( file_source )
            {BLM_INIT_LEVEL(4);
                tp_t type = 0;
                x_inst* obj = ((x_inst*)BLM_LEVEL_A_PUSH(4,((x_inst*)(x_bbml_create_from_source_t(file_source,&( type ))))));
                if( obj ) sr_s_tsm(sr,type, ((x_inst*)bcore_fork(obj)) );
            BLM_DOWN();}
        BLM_DOWN();}
        break;
    
        case ((tp_t)(TYPEOF_FROM_FILE_BCML)):
        {BLM_INIT_LEVEL(3);
            sr_s* sa = ((sr_s*)BLM_LEVEL_T_PUSH(3,sr_s,sr_s_create()));
            BLM_TRY(x_btcl_frame_s_eval(o,0, source, sa ))
            if( sr_s_type(sa) != ((tp_t)(TYPEOF_st_s)) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"First argument must represent a string.\n" ))
            BLM_TRY(x_source_parse_fa(source," , " ))
            BLM_TRY(x_btcl_frame_s_eval(o,0, source, sr ))
    
            x_source* file_source = ((x_source*)BLM_LEVEL_A_PUSH(3,x_source_create_from_file(((const st_s*)(sa->o))->sc )));
            if( file_source )
            {BLM_INIT_LEVEL(4);
                tp_t type = 0;
                x_inst* obj = ((x_inst*)BLM_LEVEL_A_PUSH(4,((x_inst*)(x_bcml_create_from_source_t(file_source,&( type ))))));
                if( obj ) sr_s_tsm(sr,type, ((x_inst*)bcore_fork(obj)) );
            BLM_DOWN();}
        BLM_DOWN();}
        break;
    
        default:
        {
            return  x_source_parse_error_fa(source,"Unknown reserved function.\n", x_btcl_frame_s_nameof(o,name ) );
        }
        break;
    }
    
    return  0;
}

er_t x_btcl_frame_s_eval_op_member( x_btcl_frame_s* o, x_source* source, sr_s* sr )
{
    // bcore_x_btcl_op.x:26:1
    
    if( sr_s_type(sr) == ((tp_t)(TYPEOF_x_btcl_net_node_s)) ) return  x_btcl_net_eval_node_member(o, source, sr );
    
    bl_t is_const = sr_s_is_const(sr);
    
    /// Identifier
    if( x_source_parse_bl(source," #?(([0]>='A'&&[0]<='Z')||([0]>='a'&&[0]<='z')||[0]=='_')" ) )
    {
        tp_t name = x_btcl_frame_s_get_identifier(o,source, true );
    
        if( x_btcl_frame_s_is_reserved_func(o,name ) )
        {
            BLM_TRY(x_source_parse_fa(source," (" ))
            BLM_TRY(x_btcl_frame_s_eval_reserved_func(o,name, source, true, sr ))
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
        else if( x_btcl_t_btcl_function_arity(((const x_btcl*)( sr->o)),sr_s_type(sr), name ) >= 0 )
        {
            s2_t arity = x_btcl_t_btcl_function_arity(((const x_btcl*)( sr->o)),sr_s_type(sr), name );
            bl_t is_mutable = x_btcl_t_btcl_function_mutable(((const x_btcl*)( sr->o)),sr_s_type(sr), name );
            sr_s_asm(sr,x_btcl_function_s_setup_external_function(x_btcl_function_s_create(),name, arity, is_mutable, ((const x_btcl*)(sr->o)) ) );
        }
        else if( name == TYPEOF_parse )
        {BLM_INIT_LEVEL(3);
            sr_s* result = ((sr_s*)BLM_LEVEL_T_PUSH(3,sr_s,sr_s_create()));
            BLM_TRY(x_source_parse_fa(source," (" ))
            if( x_btcl_t_defines_btcl_external_parse(sr_s_type(sr) ) )
            {
                BLM_TRY(x_btcl_t_btcl_external_parse(((const x_btcl*)( sr->o)),sr_s_type(sr), source, o, result ))
            }
            else if( x_btcl_t_defines_m_btcl_external_parse(sr_s_type(sr) ) )
            {BLM_INIT_LEVEL(5);
                BLM_TRY(x_btcl_t_m_btcl_external_parse(((x_btcl*)( ((x_inst*)BLM_LEVEL_A_PUSH(5,x_inst_clone(sr->o))))),sr_s_type(sr), source, o, result ))
            BLM_DOWN();}
            BLM_TRY(x_source_parse_fa(source," )" ))
            sr_s_tsm(sr,sr_s_type(result), ((x_inst*)bcore_fork(result->o)) );
        BLM_DOWN();}
        else if( name == TYPEOF_parse_file )
        {BLM_INIT_LEVEL(3);
            sr_s* sb = ((sr_s*)BLM_LEVEL_T_PUSH(3,sr_s,sr_s_create()));
            BLM_TRY(x_source_parse_fa(source," (" ))
            BLM_TRY(x_btcl_frame_s_eval(o,0, source, sb ))
            BLM_TRY(x_source_parse_fa(source," )" ))
            if( sr_s_type(sb) != ((tp_t)(TYPEOF_st_s)) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Member function 'parse_file': Expression must evaluate to a string.\n" ))
            st_s* path = ((st_s*)BLM_LEVEL_T_PUSH(3,st_s,st_s_create()));
            BLM_TRY(x_btcl_context_s_get_embedding_file_path(o->context,source, ((const st_s*)(sb->o))->sc, path ))
            x_source* emb_source =((x_source*)( ((bcore_source*)BLM_LEVEL_A_PUSH(3,bcore_file_open_source(path->sc )))));
            sr_s* result = ((sr_s*)BLM_LEVEL_T_PUSH(3,sr_s,sr_s_create()));
            if( x_btcl_t_defines_btcl_external_parse(sr_s_type(sr) ) )
            {
                BLM_TRY(x_btcl_t_btcl_external_parse(((const x_btcl*)( sr->o)),sr_s_type(sr), emb_source, o, result ))
            }
            else if( x_btcl_t_defines_m_btcl_external_parse(sr_s_type(sr) ) )
            {BLM_INIT_LEVEL(5);
                BLM_TRY(x_btcl_t_m_btcl_external_parse(((x_btcl*)( ((x_inst*)BLM_LEVEL_A_PUSH(5,x_inst_clone(sr->o))))),sr_s_type(sr), emb_source, o, result ))
            BLM_DOWN();}
            BLM_TRY(x_source_parse_fa(emb_source," " ))
            if( !x_source_eos(emb_source) ) BLM_TRY(x_source_parse_error_fa(emb_source,"Unexpected expression found.\n" ))
            sr_s_tsm(sr,sr_s_type(result), ((x_inst*)bcore_fork(result->o)) );
        BLM_DOWN();}
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

er_t x_btcl_frame_s_eval_op_functional( x_btcl_frame_s* o, x_source* source, sr_s* sr )
{
    // bcore_x_btcl_op.x:175:1
    
    if( sr_s_o_type(sr) == ((tp_t)(TYPEOF_x_btcl_function_s)) )
    {BLM_INIT_LEVEL(1);
        BLM_TRY(x_btcl_function_s_call_via_evaluation(((x_btcl_function_s*)BLM_LEVEL_T_PUSH(1,x_btcl_function_s,((x_btcl_function_s*)bcore_fork(((x_btcl_function_s*)(sr->o)))))),source, o, sr ))
    BLM_DOWN();}
    else if( sr_s_o_type(sr) == ((tp_t)(TYPEOF_x_btcl_block_s)) )
    {
        return  x_source_parse_error_fa(source,"Attempt to evaluate a block as function. Join the block with a signature to create a function.\n" );
    }
    else if( sr_s_o_type(sr) == ((tp_t)(TYPEOF_x_btcl_net_node_s)) )
    {
        if( !x_source_parse_bl(source," #=?')'" ) )
        {BLM_INIT_LEVEL(3);
            BLM_TRY(x_btcl_net_eval_node_modifier(x_btcl_frame_s_setup(((x_btcl_frame_s*)BLM_LEVEL_T_PUSH(3,x_btcl_frame_s,x_btcl_frame_s_create())),o ), source, sr ))
        BLM_DOWN();}
        x_btcl_clone_if_weak(sr );
    }
    else if( sr_s_o_type(sr) == ((tp_t)(TYPEOF_x_btcl_functor_s)) )
    {
        if( !x_source_parse_bl(source," #=?')'" ) )
        {BLM_INIT_LEVEL(3);
            BLM_TRY(x_btcl_functor_eval_modifier(x_btcl_frame_s_setup(((x_btcl_frame_s*)BLM_LEVEL_T_PUSH(3,x_btcl_frame_s,x_btcl_frame_s_create())),o ), source, sr ))
        BLM_DOWN();}
        x_btcl_clone_if_weak(sr );
    }
    else
    {
        if( !x_source_parse_bl(source," #=?')'" ) )
        {BLM_INIT_LEVEL(2);
            BLM_TRY(x_btcl_frame_s_eval_op_modifier(x_btcl_frame_s_setup(((x_btcl_frame_s*)BLM_LEVEL_T_PUSH(2,x_btcl_frame_s,x_btcl_frame_s_create())),o ),source, sr ))
        BLM_DOWN();}
        x_btcl_clone_if_weak(sr );
    }
    return  0;
}

er_t x_btcl_frame_s_eval_op_modifier( x_btcl_frame_s* o, x_source* source, sr_s* sr )
{
    // bcore_x_btcl_op.x:214:1
    
    (*(sr)) = sr_clone( (*(sr)) );
    
    bl_t do_loop = true;
    while( do_loop )
    {
        if( x_source_parse_bl(source," #=?'.'") )
        {
            sr_s sr_weak = sr_cw( (*(sr)) );
            BLM_TRY(x_btcl_frame_s_eval_op(o,0, source,&( sr_weak )))
        }
        else
        {BLM_INIT_LEVEL(2);
            sr_s* sb = ((sr_s*)BLM_LEVEL_T_PUSH(2,sr_s,sr_s_create()));
            BLM_TRY(x_btcl_frame_s_eval(o,0, source, sb ))
            er_t err = x_btcl_frame_s_generic_copy(o,x_source_point_s_setup_from_source(((x_source_point_s*)BLM_LEVEL_T_PUSH(2,x_source_point_s,x_source_point_s_create())),source ), sr, sb );
            if( err ) {BLM_INIT_LEVEL(3); BLM_RETURNV(er_t, x_source_parse_error_fa(source,"#<sc_t>\n", bcore_error_pop_all_to_st(((st_s*)BLM_LEVEL_T_PUSH(3,st_s,st_s_create())) )->sc )) BLM_DOWN();}
        BLM_DOWN();}
    
        do_loop = false;
    
        if( x_source_parse_bl(source," #?','"  ) ) do_loop = true;
        if( x_source_parse_bl(source," #=?')'" ) ) do_loop = false;
    }
    
    x_stamp_t_mutated(((x_stamp*)(sr->o)),sr_s_type(sr) );
    
    return  0;
}

er_t x_btcl_frame_s_eval_op_func_list_unfold( x_btcl_frame_s* o, s2_t op_priority, x_source* source, sr_s* sr )
{
    // bcore_x_btcl_op.x:247:1
    BLM_INIT_LEVEL(0);
    sr_s sb;BLM_T_INIT_SPUSH(sr_s, &sb);; BLM_TRY(x_btcl_frame_s_eval(o,op_priority, source,&( sb )))
    
    if( sr_s_type(sr) == ((tp_t)(TYPEOF_x_btcl_function_s)) && sr_s_type(&(sb)) == ((tp_t)(TYPEOF_x_btcl_list_s)) )
    {BLM_INIT_LEVEL(1);
        x_btcl_function_s* f = ((x_btcl_function_s*)BLM_LEVEL_T_PUSH(1,x_btcl_function_s,((x_btcl_function_s*)bcore_fork(((x_btcl_function_s*)(sr->o))))));
        x_btcl_list_s* list = ((x_btcl_list_s*)BLM_LEVEL_T_PUSH(1,x_btcl_list_s,((x_btcl_list_s*)bcore_fork(((x_btcl_list_s*)(sb.o))))));
        BLM_TRY(x_btcl_function_s_call_via_arg_list(f,x_source_point_s__(((x_source_point_s*)BLM_LEVEL_T_PUSH(1,x_source_point_s,x_source_point_s_create())),source ), o, list, sr ))
        BLM_RETURNV(er_t, 0)
    BLM_DOWN();}
    
    BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Operator #<sc_t> * #<sc_t> is not defined.\n", bnameof( sr_s_o_type(sr) ), bnameof( sr_s_o_type(&(sb)) ) ))
}

er_t x_btcl_frame_s_eval_op_func_list_transform( x_btcl_frame_s* o, s2_t op_priority, x_source* source, sr_s* sr )
{
    // bcore_x_btcl_op.x:264:1
    BLM_INIT_LEVEL(0);
    sr_s sb;BLM_T_INIT_SPUSH(sr_s, &sb);; BLM_TRY(x_btcl_frame_s_eval(o,op_priority, source,&( sb )))
    
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

er_t x_btcl_frame_s_eval_op_func_list_unfold_transform( x_btcl_frame_s* o, s2_t op_priority, x_source* source, sr_s* sr )
{
    // bcore_x_btcl_op.x:290:1
    BLM_INIT_LEVEL(0);
    sr_s sb;BLM_T_INIT_SPUSH(sr_s, &sb);; BLM_TRY(x_btcl_frame_s_eval(o,op_priority, source,&( sb )))
    
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

er_t x_btcl_frame_s_eval_op_assign( x_btcl_frame_s* o, s2_t op_priority, x_source* source, sr_s* sr )
{
    // bcore_x_btcl_op.x:340:1
    BLM_INIT_LEVEL(0);
    if( sr_s_is_const(sr) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Assignment to a const object.\n" ))
    
    sr_s sb;BLM_T_INIT_SPUSH(sr_s, &sb);; BLM_TRY(x_btcl_frame_s_eval(o,op_priority, source,&( sb )))
    
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
        {BLM_INIT_LEVEL(3);
            sr_s sr1 = x_btcl_null_member_s_set_sr(((x_btcl_null_member_s*)(sr->o)),o, x_source_point_s_setup_from_source(((x_source_point_s*)BLM_LEVEL_T_PUSH(3,x_source_point_s,x_source_point_s_create())),source ),&( sb ));
            sr_s_down(sr);
            (*(sr)) = sr1;
        BLM_DOWN();}
        break;
    
        case ((tp_t)(TYPEOF_x_btcl_null_arr_element_s)):
        {BLM_INIT_LEVEL(3);
            sr_s sr1 = x_btcl_null_arr_element_s_set_sr(((x_btcl_null_arr_element_s*)(sr->o)),o, x_source_point_s_setup_from_source(((x_source_point_s*)BLM_LEVEL_T_PUSH(3,x_source_point_s,x_source_point_s_create())),source ),&( sb ));
            sr_s_down(sr);
            (*(sr)) = sr1;
        BLM_DOWN();}
        break;
    
        default:
        {
            if( sr->o )
            {BLM_INIT_LEVEL(4);
                er_t err = x_btcl_frame_s_generic_copy(o,x_source_point_s_setup_from_source(((x_source_point_s*)BLM_LEVEL_T_PUSH(4,x_source_point_s,x_source_point_s_create())),source ), sr,&( sb ));
                if( err ) {BLM_INIT_LEVEL(5); BLM_RETURNV(er_t, x_source_parse_error_fa(source,"operator '=': #<sc_t>\n", bcore_error_pop_all_to_st(((st_s*)BLM_LEVEL_T_PUSH(5,st_s,st_s_create())) )->sc )) BLM_DOWN();}
            BLM_DOWN();}
        }
        break;
    }
    
    BLM_RETURNV(er_t, 0)
}

er_t x_btcl_frame_s_eval_op_continuation( x_btcl_frame_s* o, s2_t op_priority, x_source* source, sr_s* sr )
{
    // bcore_x_btcl_op.x:390:1
    
    // if the last expression in a file, block or frame has a trailing semicolon, the continuation is ignored
    if( x_source_parse_bl(source," #=?([0]=='}'||[0]==')')" ) ) return  0;
    if( x_source_eos(source) ) return  0;
    
    sr_s_clear(sr);
    BLM_TRY(x_btcl_frame_s_eval(o,op_priority, source, sr ))
    return  0;
}

er_t x_btcl_frame_s_eval_op( x_btcl_frame_s* o, s2_t exit_priority, x_source* source, sr_s* obj )
{
    // bcore_x_btcl_op.x:407:1
    
    // operators in descending order of priority
    
    /// priority group a ---------------------
    
    s2_t op_priority = x_btcl_priority_a();
    
    if( op_priority <= exit_priority ) return  0;
    
    while( x_source_parse_bl(source," #?([0]=='.'||[0]=='(')" ) )
    {
        if( x_source_parse_bl(source," #?'.'" ) )
        {
            BLM_TRY(x_btcl_frame_s_eval_op_member(o,source, obj ))
        }
    
        if( x_source_parse_bl(source," #?'('" ) )
        {
            BLM_TRY(x_btcl_frame_s_eval_op_functional(o,source, obj ))
            BLM_TRY(x_source_parse_fa(source," )" ))
        }
    }
    op_priority--;
    
    /// priority group c ---------------------
    
    op_priority = x_btcl_priority_c();
    
    if( op_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'^'" ) ) BLM_TRY(x_btcl_operator_eval_bop_type(o, ((tp_t)(TYPEOF_pow)), op_priority, source, obj, obj ))
    op_priority--;
    
    if( op_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'/'" ) ) BLM_TRY(x_btcl_operator_eval_bop_type(o, ((tp_t)(TYPEOF_div)), op_priority, source, obj, obj ))
    op_priority--;
    
    if( op_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'%'" ) ) BLM_TRY(x_btcl_operator_eval_bop_type(o, ((tp_t)(TYPEOF_mod)), op_priority, source, obj, obj ))
    op_priority--;
    
    if( op_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'**'" ) ) BLM_TRY(x_btcl_operator_eval_bop_type(o, ((tp_t)(TYPEOF_chain)), op_priority, source, obj, obj ))
    op_priority--;
    
    if( op_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'*.:'" ) ) BLM_TRY(x_btcl_frame_s_eval_op_func_list_unfold_transform(o,op_priority, source, obj ))
    op_priority--;
    
    if( op_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'*.'" ) ) BLM_TRY(x_btcl_frame_s_eval_op_func_list_unfold(o,op_priority, source, obj ))
    op_priority--;
    
    if( op_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'*:'" ) ) BLM_TRY(x_btcl_frame_s_eval_op_func_list_transform(o,op_priority, source, obj ))
    op_priority--;
    
    if( op_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'*'" ) ) BLM_TRY(x_btcl_operator_eval_bop_type(o, ((tp_t)(TYPEOF_mul)), op_priority, source, obj, obj ))
    op_priority--;
    
    if( op_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'-'" ) ) BLM_TRY(x_btcl_operator_eval_bop_type(o, ((tp_t)(TYPEOF_sub)), op_priority, source, obj, obj ))
    op_priority--;
    
    if( op_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'+'" ) ) BLM_TRY(x_btcl_operator_eval_bop_type(o, ((tp_t)(TYPEOF_add)), op_priority, source, obj, obj ))
    op_priority--;
    
    if( op_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'!='" ) ) BLM_TRY(x_btcl_operator_eval_bop_type(o, ((tp_t)(TYPEOF_unequal)), op_priority, source, obj, obj ))
    op_priority--;
    
    if( op_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'=='" ) ) BLM_TRY(x_btcl_operator_eval_bop_type(o, ((tp_t)(TYPEOF_equal)), op_priority, source, obj, obj ))
    op_priority--;
    
    if( op_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'>='" ) ) BLM_TRY(x_btcl_operator_eval_bop_type(o, ((tp_t)(TYPEOF_larger_equal)), op_priority, source, obj, obj ))
    op_priority--;
    
    if( op_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?([0]=='>'&&[1]!='>')" ) ) { x_source_get_char(source); BLM_TRY(x_btcl_operator_eval_bop_type(o, ((tp_t)(TYPEOF_larger)), op_priority, source, obj, obj )) }
    op_priority--;
    
    if( op_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'<='" ) ) BLM_TRY(x_btcl_operator_eval_bop_type(o, ((tp_t)(TYPEOF_smaller_equal)), op_priority, source, obj, obj ))
    op_priority--;
    
    if( op_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?([0]=='<'&&[1]!='<')" ) ) { x_source_get_char(source); BLM_TRY(x_btcl_operator_eval_bop_type(o, ((tp_t)(TYPEOF_smaller)), op_priority, source, obj, obj )) }
    op_priority--;
    
    if( op_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'&&'" ) ) BLM_TRY(x_btcl_operator_eval_bop_type(o, ((tp_t)(TYPEOF_and)), op_priority, source, obj, obj ))
    op_priority--;
    
    if( op_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'||'" ) ) BLM_TRY(x_btcl_operator_eval_bop_type(o, ((tp_t)(TYPEOF_or)), op_priority, source, obj, obj ))
    op_priority--;
    
    if( op_priority <= exit_priority ) return  0;
    op_priority--; // extra priority decrease to allow conditional reoccurring in a branch
    /*no loop here:*/ if( x_source_parse_bl(source," #?'?'" ) ) BLM_TRY(x_btcl_operator_eval_top_type(o, ((tp_t)(TYPEOF_conditional)), op_priority, source, obj, obj ))
    op_priority--;
    
    if( op_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'::'" ) ) BLM_TRY(x_btcl_operator_eval_bop_type(o, ((tp_t)(TYPEOF_spawn)), op_priority, source, obj, obj ))
    op_priority--;
    
    if( op_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?':'" ) ) BLM_TRY(x_btcl_operator_eval_bop_type(o, ((tp_t)(TYPEOF_cat)), op_priority, source, obj, obj ))
    op_priority--;
    
    // !!! '<<' evaluates in RL-order !!!
    if( op_priority < /*not '<=' */ exit_priority ) return  0; // op_priority < exit_priority ensures evaluation from right to left
    while( x_source_parse_bl(source," #?'<<'" ) ) BLM_TRY(x_btcl_operator_eval_bop_type(o, ((tp_t)(TYPEOF_shift_left)), op_priority, source, obj, obj ))
    op_priority--;
    
    if( op_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?'='" ) ) BLM_TRY(x_btcl_frame_s_eval_op_assign(o,op_priority, source, obj ))
    op_priority--;
    
    /// priority group e ---------------------
    
    op_priority = x_btcl_priority_e();
    
    if( op_priority <= exit_priority ) return  0;
    while( x_source_parse_bl(source," #?';'" ) ) BLM_TRY(x_btcl_frame_s_eval_op_continuation(o,op_priority, source, obj ))
    op_priority--;
    
    return  0;
}

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_btcl_null_variable_s )
"aware x_btcl",
"{",
    "tp_t tp_name;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_btcl_null_variable_s )

x_btcl_null_variable_s* x_btcl_null_variable_s__( x_btcl_null_variable_s* o, tp_t tp_name )
{
    // :2:1
    
    o->tp_name = tp_name;
    return  o;
}

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_btcl_null_member_s )
"aware x_btcl",
"{",
    "sr_s base;",
    "tp_t tp_name;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_btcl_null_member_s )

x_btcl_null_member_s* x_btcl_null_member_s_setup( x_btcl_null_member_s* o, sr_s* base, tp_t tp_name )
{
    // bcore_x_btcl.x:497:5
    
    sr_s_twm(&(o->base),sr_s_o_type(base), base->o );
    o->tp_name = tp_name;
    return o;
}

sr_s x_btcl_null_member_s_set_sr( x_btcl_null_member_s* o, x_btcl_frame_s* frame, const x_source_point_s* sp, sr_s* src )
{
    // bcore_x_btcl.x:503:5
    
    uz_t index = x_stamp_t_index(sr_s_o_type(&(o->base)), o->tp_name );
    
    if( x_stamp_t_is_static_i(sr_s_o_type(&(o->base)), index ) && src->o )
    {
        tp_t dst_type = x_stamp_t_type_i(((const x_stamp*)( o->base.o)),sr_s_o_type(&(o->base)), index );
        sr_s sr = sr_tsm(dst_type, x_inst_t_create(dst_type ) );
        BLM_TRY_EXIT(x_btcl_frame_s_generic_copy(frame,sp,&( sr), src ))
        BLM_TRY_EXIT(x_stamp_t_set_sr(((x_stamp*)( o->base.o)),sr_s_o_type(&(o->base)), o->tp_name, sr ))
    }
    else
    {
        BLM_TRY_EXIT(x_stamp_t_set_sr(((x_stamp*)( o->base.o)),sr_s_o_type(&(o->base)), o->tp_name, sr_cw(*( src )) ))
    }
    
    return  x_stamp_t_m_get_sr(((x_stamp*)( o->base.o)),sr_s_o_type(&(o->base)), o->tp_name );
}

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_btcl_null_arr_element_s )
"aware x_btcl",
"{",
    "sr_s base;",
    "s3_t index;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_btcl_null_arr_element_s )

x_btcl_null_arr_element_s* x_btcl_null_arr_element_s_setup( x_btcl_null_arr_element_s* o, sr_s* base, tp_t index )
{
    // bcore_x_btcl.x:531:5
    
    sr_s_twm(&(o->base),sr_s_o_type(base), base->o );
    o->index = index;
    return o;
}

sr_s x_btcl_null_arr_element_s_set_sr( x_btcl_null_arr_element_s* o, x_btcl_frame_s* frame, const x_source_point_s* sp, sr_s* src )
{
    // bcore_x_btcl.x:537:5
    
    if( x_array_t_is_static(sr_s_o_type(&(o->base)) ) && src->o )
    {
        tp_t dst_type = x_array_t_get_static_type(sr_s_o_type(&(o->base)) );
        sr_s sr = sr_tsm(dst_type, x_inst_t_create(dst_type ) );
        BLM_TRY_EXIT(x_btcl_frame_s_generic_copy(frame,sp,&( sr), src ))
        x_array_t_set_sr(((x_array*)( ((x_stamp*)(o->base.o)))),sr_s_o_type(&(o->base)), o->index, sr );
    }
    else
    {
        x_array_t_set_sr(((x_array*)( ((x_stamp*)(o->base.o)))),sr_s_o_type(&(o->base)), o->index, sr_cw(*( src )) );
    }
    
    return  x_array_t_m_get_sr(((x_array*)( ((x_stamp*)(o->base.o)))),sr_s_o_type(&(o->base)), o->index );
}

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_btcl_list_s )
"aware x_btcl",
"{",
    "bcore_arr_sr_s arr;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_btcl_list_s )

er_t x_btcl_list_s_to_sink( const x_btcl_list_s* o, bl_t detailed, x_sink* sink )
{
    // bcore_x_btcl.x:573:5
    
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

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_btcl_block_s )
"aware x_btcl",
"{",
    "hidden x_source_point_s source_point;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_btcl_block_s )

er_t x_btcl_block_s_parse( x_btcl_block_s* o, x_btcl_frame_s* frame, x_source* source )
{
    // bcore_x_btcl.x:610:1
    
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
    // bcore_x_btcl.x:653:1
    
    x_source* source = o->source_point.source;
    s3_t index = x_source_get_index(source);
    x_source_set_index(source,o->source_point.index );
    BLM_TRY(x_btcl_frame_s_eval_in_frame(parent_frame,0, source, obj ))
    BLM_TRY(x_source_parse_fa(source," }" ))
    x_source_set_index(source,index );
    return  0;
}

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_btcl_signature_s )
"aware x_btcl",
"{",
    "hidden x_source_point_s source_point;",
    "bcore_arr_tp_s arg_list;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_btcl_signature_s )

er_t x_btcl_signature_s_parse( x_btcl_signature_s* o, x_btcl_frame_s* frame, x_source* source )
{
    // bcore_x_btcl.x:682:1
    
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

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_btcl_external_function_s )
"aware x_btcl",
"{",
    "tp_t name;",
    "bl_t is_mutable;",
    "aware x_btcl -> object;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_btcl_external_function_s )

x_btcl_external_function_s* x_btcl_external_function_s_setup( x_btcl_external_function_s* o, tp_t name, bl_t is_mutable, const x_btcl* object )
{
    // bcore_x_btcl.x:719:5
    
    o->name = name;
    o->is_mutable = is_mutable;
    x_btcl_a_attach( &(o->object ), (x_btcl*)( ((x_btcl*)bcore_fork(((x_btcl*)(object))))));
    return o;
}

er_t x_btcl_external_function_s_execute( x_btcl_external_function_s* o, const x_source_point_s* sp, x_btcl_frame_s* lexical_frame, const bcore_arr_sr_s* args, sr_s* sr )
{
    // bcore_x_btcl.x:726:5
    
    sr_s_from_f3(sr,0 );
    if( o->is_mutable )
    {BLM_INIT_LEVEL(1);
        BLM_TRY(x_btcl_a_m_btcl_function(((x_btcl*)BLM_LEVEL_A_PUSH(1,x_btcl_a_clone(o->object))),o->name, sp, lexical_frame, args, sr ))
    BLM_DOWN();}
    else
    {
        BLM_TRY(x_btcl_a_btcl_function(o->object,o->name, sp, lexical_frame, args, sr ))
    }
    return  0;
}

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_btcl_function_s )
"aware x_btcl",
"{",
    "x_btcl_signature_s -> signature;",
    "x_btcl_block_s -> block;",
    "x_btcl_external_function_s -> external_function;",
    "x_btcl_function_s -> wrapped_function;",
    "bcore_arr_sr_s -> wrapped_arg_list;",
    "x_btcl_function_s => tail;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_btcl_function_s )

x_btcl_function_s* x_btcl_function_s_setup( x_btcl_function_s* o, x_btcl_signature_s* signature, x_btcl_block_s* block, const x_btcl_function_s* tail )
{
    // bcore_x_btcl.x:754:5
    
    x_btcl_signature_s_attach( &(o->signature ),  ((x_btcl_signature_s*)bcore_fork(signature)));
    x_btcl_block_s_attach( &(o->block     ),  ((x_btcl_block_s*)bcore_fork(block)));
    x_btcl_function_s_attach( &(o->tail      ),  x_btcl_function_s_clone(tail));
    x_btcl_external_function_s_attach( &(o->external_function ),  NULL);
    return o;
}

x_btcl_function_s* x_btcl_function_s_setup_external_function( x_btcl_function_s* o, tp_t name, s2_t arity, bl_t is_mutable, const x_btcl* object )
{
    // bcore_x_btcl.x:762:5
    
    x_btcl_signature_s_attach( &(o->signature ),  x_btcl_signature_s_create());
    bcore_arr_tp_s_set_size(&(o->signature->arg_list),arity );
    for(sz_t i = 0; i < arity; i++ ) o->signature->arg_list.data[ i ] = i + 1;
    x_btcl_external_function_s_attach( &(o->external_function ),  x_btcl_external_function_s_setup(x_btcl_external_function_s_create(),name, is_mutable, object ));
    x_btcl_block_s_attach( &(o->block ),  NULL);
    x_btcl_function_s_attach( &(o->tail  ),  NULL);
    return  o;
}

x_btcl_function_s* x_btcl_function_s_setup_wrapped_function( x_btcl_function_s* o, x_btcl_function_s* wrapped_function, bcore_arr_sr_s* wrapped_arg_list )
{
    // bcore_x_btcl.x:773:5
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
    // bcore_x_btcl.x:788:5
    
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
    // bcore_x_btcl.x:810:1
    
    if( lexical_frame->depth >= x_btcl_max_frame_depth() ) return  x_source_point_s_parse_error_fa(source_point,"Maximum frame depth (#<sz_t>) exceeded. Check for unguarded recursions.\n", x_btcl_max_frame_depth() );
    x_btcl_frame_s_setup(frame,lexical_frame );
    x_btcl_frame_s_var_set(frame,TYPEOF_self, sr_asm(((x_inst*)( ((x_btcl_function_s*)bcore_fork(o)) ))) );
    
    return  0;
}

er_t x_btcl_function_s_call( x_btcl_function_s* o, x_source_point_s* source_point, x_btcl_frame_s* lexical_frame, bcore_arr_sr_s* arg_list, sr_s* result )
{
    // bcore_x_btcl.x:822:1
    
    const x_btcl_signature_s* signature = o->signature;
    if( arg_list->size < signature->arg_list.size )
    {
        sr_s_asm(result,x_btcl_function_s_setup_wrapped_function(x_btcl_function_s_create(),o, arg_list) );
        return  0;
    }
    else if( arg_list->size == signature->arg_list.size )
    {
        if( o->block )
        {BLM_INIT_LEVEL(3);
            x_btcl_frame_s* frame = ((x_btcl_frame_s*)BLM_LEVEL_T_PUSH(3,x_btcl_frame_s,x_btcl_frame_s_create()));
            BLM_TRY(x_btcl_function_s_setup_frame(o,lexical_frame, source_point, frame ))
            for(sz_t i = 0; i < signature->arg_list.size; i++ ) sr_s_fork_from(x_btcl_frame_s_var_set(frame,signature->arg_list.data[ i ], sr_null() ),&(arg_list->data[ i ] ));
            sr_s_clear(result);
            BLM_TRY(x_btcl_block_s_eval(o->block,frame, result ))
            x_btcl_clone_if_weak(result );
        BLM_DOWN();}
        else if( o->external_function )
        {
            BLM_TRY(x_btcl_external_function_s_execute(o->external_function,source_point, lexical_frame, arg_list, result ))
        }
        else if( o->wrapped_function )
        {BLM_INIT_LEVEL(4);
            bcore_arr_sr_s* full_arg_list = ((bcore_arr_sr_s*)BLM_LEVEL_T_PUSH(4,bcore_arr_sr_s,bcore_arr_sr_s_create()));
            for(sz_t i = 0; i < o->wrapped_arg_list->size; i++ ) sr_s_fork_from(bcore_arr_sr_s_push_sr(full_arg_list,sr_null() ),&(o->wrapped_arg_list->data[ i ] ));
            for(sz_t i = 0; i < arg_list->size          ; i++ ) sr_s_fork_from(bcore_arr_sr_s_push_sr(full_arg_list,sr_null() ),&(arg_list->data[ i ] ));
            BLM_TRY(x_btcl_function_s_call(o->wrapped_function,source_point, lexical_frame, full_arg_list, result ))
        BLM_DOWN();}
        else
        {
            BLM_TRY(x_source_point_s_parse_error_fa(source_point,"Internal error: Function is incomplete.\n" ))
        }
    
        // chain of functions ...
        if( o->tail )
        {
            BLM_TRY(x_btcl_function_s_call_unary(o->tail,source_point, lexical_frame, result, result ))
        }
        return  0;
    }
    else
    {
        BLM_TRY(x_source_point_s_parse_error_fa(source_point,"Too many arguments. Function accepts #<sz_t> argument#<sc_t>.\n", signature->arg_list.size, signature->arg_list.size != 1 ? "s" : "" ))
    }
    return  0;
}

er_t x_btcl_function_s_call_unary( x_btcl_function_s* o, x_source_point_s* source_point, x_btcl_frame_s* lexical_frame, sr_s* s_arg, sr_s* result )
{
    // bcore_x_btcl.x:873:1
    BLM_INIT_LEVEL(0);
    bcore_arr_sr_s* arg_list = ((bcore_arr_sr_s*)BLM_LEVEL_T_PUSH(0,bcore_arr_sr_s,bcore_arr_sr_s_create()));
    sr_s_fork_from(bcore_arr_sr_s_push_sr(arg_list,sr_null() ),s_arg );
    BLM_RETURNV(er_t, x_btcl_function_s_call(o,source_point, lexical_frame, arg_list, result ))
}

er_t x_btcl_function_s_call_via_arg_list( x_btcl_function_s* o, x_source_point_s* source_point, x_btcl_frame_s* lexical_frame, x_btcl_list_s* arg_list1, sr_s* result )
{
    // bcore_x_btcl.x:882:1
    BLM_INIT_LEVEL(0);
    bcore_arr_sr_s* arg_list = bcore_arr_sr_s_set_size(((bcore_arr_sr_s*)BLM_LEVEL_T_PUSH(0,bcore_arr_sr_s,bcore_arr_sr_s_create())),x_btcl_list_s_size(arg_list1) );
    for(sz_t i = 0; i < arg_list->size; i++ ) sr_s_fork_from(&(arg_list->data[ i ]),&(arg_list1->arr.data[ i ] ));
    BLM_RETURNV(er_t, x_btcl_function_s_call(o,source_point, lexical_frame, arg_list, result ))
}

er_t x_btcl_function_s_call_via_evaluation( x_btcl_function_s* o, x_source* source, x_btcl_frame_s* lexical_frame, sr_s* sr )
{
    // bcore_x_btcl.x:891:1
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

XOILA_DEFINE_SPECT_NASC_BEGIN( x_inst, x_btcl )
"{",
    "bcore_spect_header_s header;",
    "feature aware x_btcl : btcl_function_arity = x_btcl_btcl_function_arity_default;",
    "feature aware x_btcl : btcl_function;",
    "feature aware x_btcl : m_btcl_function;",
    "feature aware x_btcl : btcl_function_mutable = x_btcl_btcl_function_mutable_default;",
    "feature aware x_btcl : btcl_external_parse;",
    "feature aware x_btcl : m_btcl_external_parse;",
    "feature aware x_btcl : nullary_f3;",
    "feature aware x_btcl : unary_f3;",
    "feature aware x_btcl : binary_f3;",
    "feature aware x_btcl : ternary_f3;",
"}",
XOILA_DEFINE_SPECT_NASC_END( x_inst, x_btcl )

sc_t x_btcl_operator_symbol( tp_t type )
{
    // bcore_x_btcl.x:50:1
    
    switch( type )
    {
        // Group A, binary
        case ((tp_t)(TYPEOF_member)): return  ".";
        case ((tp_t)(TYPEOF_frame)):  return  "(";
    
        // builtin functions
        case ((tp_t)(TYPEOF_exp)):     return  "EXP";
        case ((tp_t)(TYPEOF_log)):     return  "LOG";
        case ((tp_t)(TYPEOF_log2)):    return  "LOG2";
        case ((tp_t)(TYPEOF_log10)):   return  "LOG10";
        case ((tp_t)(TYPEOF_sin)):     return  "SIN";
        case ((tp_t)(TYPEOF_cos)):     return  "COS";
        case ((tp_t)(TYPEOF_tan)):     return  "TAN";
        case ((tp_t)(TYPEOF_tanh)):    return  "TANH";
        case ((tp_t)(TYPEOF_sign)):    return  "SIGN";
        case ((tp_t)(TYPEOF_sqrt)):    return  "SQRT";
        case ((tp_t)(TYPEOF_abs)):     return  "ABS";
        case ((tp_t)(TYPEOF_ceil)):    return  "CEIL";
        case ((tp_t)(TYPEOF_floor)):   return  "FLOOR";
        case ((tp_t)(TYPEOF_max)):     return  "MAX";
        case ((tp_t)(TYPEOF_min)):     return  "MIN";
        case ((tp_t)(TYPEOF_ife)):     return  "IFE";
        case ((tp_t)(TYPEOF_size)):    return  "SIZE";
        case ((tp_t)(TYPEOF_print)):   return  "PRINT";
        case ((tp_t)(TYPEOF_println)): return  "PRINTLN";
        case ((tp_t)(TYPEOF_printx)):  return  "PRINTX";
        case ((tp_t)(TYPEOF_assert)):  return  "ASSERT";
        case ((tp_t)(TYPEOF_mkdir)):   return  "MKDIR";
        case ((tp_t)(TYPEOF_rmdir)):   return  "RMDIR";
        case ((tp_t)(TYPEOF_file_exists)):  return  "FILE_EXISTS";
        case ((tp_t)(TYPEOF_to_file)):      return  "TO_FILE";
        case ((tp_t)(TYPEOF_to_file_btml)): return  "TO_FILE_BTML";
        case ((tp_t)(TYPEOF_to_file_bbml)): return  "TO_FILE_BBML";
        case ((tp_t)(TYPEOF_to_file_bcml)): return  "TO_FILE_BCML";
        case ((tp_t)(TYPEOF_from_file)):      return  "FROM_FILE";
        case ((tp_t)(TYPEOF_from_file_btml)): return  "FROM_FILE_BTML";
        case ((tp_t)(TYPEOF_from_file_bbml)): return  "FROM_FILE_BBML";
        case ((tp_t)(TYPEOF_from_file_bcml)): return  "FROM_FILE_BCML";
    
        // Group B, unary
        case ((tp_t)(TYPEOF_identity)): return  "+";
        case ((tp_t)(TYPEOF_neg)):  return  "-";
        case ((tp_t)(TYPEOF_not)):  return  "!";
    
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
    
        case ((tp_t)(TYPEOF_equal)):         return  "==";
        case ((tp_t)(TYPEOF_unequal)):       return  "!=";
        case ((tp_t)(TYPEOF_larger_equal)):  return  ">=";
        case ((tp_t)(TYPEOF_larger)):        return  ">";
        case ((tp_t)(TYPEOF_smaller_equal)): return  "<=";
        case ((tp_t)(TYPEOF_smaller)):       return  "<";
    
        case ((tp_t)(TYPEOF_and)):         return  "&";
        case ((tp_t)(TYPEOF_or)):          return  "|";
        case ((tp_t)(TYPEOF_conditional)): return  "?";
        case ((tp_t)(TYPEOF_spawn)):       return  "::";
        case ((tp_t)(TYPEOF_cat)):         return  ":";
        case ((tp_t)(TYPEOF_shift_left)):  return  "<<";
        case ((tp_t)(TYPEOF_assign)):      return  "=";
    
        // Group E, binary
        case ((tp_t)(TYPEOF_continuation)): return  ";";
    
        default: break;
    }
    
    ERR_fa( "Unhandled operator type '#<sc_t>'.", bnameof( type ) );
    
    return  0;
}

void x_btcl_clone_if_weak( sr_s* sr )
{
    // bcore_x_btcl.x:149:1
    
    if( sr_s_is_weak(sr) )
    {
        bl_t is_const = sr_s_is_const(sr);
        (*(sr)) = sr_clone(*( sr ));
        sr_s_set_const(sr,is_const );
    }
}

void x_btcl_clone_if_weak_or_twice_referenced( sr_s* sr )
{
    // bcore_x_btcl.x:165:1
    
    if( sr_s_is_weak(sr) || sr_s_references(sr) >= 2 )
    {
        bl_t is_const = sr_s_is_const(sr);
        (*(sr)) = sr_clone(*( sr ));
        sr_s_set_const(sr,is_const );
    }
}

er_t x_btcl_generic_copy( sr_s* sr, const sr_s* sb )
{
    // bcore_x_btcl.x:178:1
    
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
    // bcore_x_btcl.x:1061:1
    
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

er_t x_btcl_bop_cat_ab( sr_s* a, sr_s* b, sr_s* sr )
{
    // bcore_x_btcl_op.x:316:1
    
    x_btcl_clone_if_weak(a );
    x_btcl_clone_if_weak(b );
    if( sr_s_type(a) == ((tp_t)(TYPEOF_x_btcl_signature_s)) && sr_s_type(b) == ((tp_t)(TYPEOF_x_btcl_block_s)) )
    {BLM_INIT_LEVEL(1);
        x_btcl_signature_s* signature = ((x_btcl_signature_s*)(a->o));
        x_btcl_block_s* block     = ((x_btcl_block_s*)(b->o));
        x_btcl_function_s* function = x_btcl_function_s_setup(((x_btcl_function_s*)BLM_LEVEL_T_PUSH(1,x_btcl_function_s,x_btcl_function_s_create())),signature, block, NULL );
        sr_s_asc(sr,((x_btcl_function_s*)bcore_fork(function)) );
    BLM_DOWN();}
    else
    {BLM_INIT_LEVEL(1);
        x_btcl_list_s* list = ((x_btcl_list_s*)BLM_LEVEL_T_PUSH(1,x_btcl_list_s,x_btcl_list_s_create()));
        if( sr_s_type(a) == ((tp_t)(TYPEOF_x_btcl_list_s)) ) x_btcl_list_s_push_list_fork(list,((x_btcl_list_s*)(a->o)) ); else x_btcl_list_s_push_fork(list,a );
        if( sr_s_type(b) == ((tp_t)(TYPEOF_x_btcl_list_s)) ) x_btcl_list_s_push_list_fork(list,((x_btcl_list_s*)(b->o)) ); else x_btcl_list_s_push_fork(list,b );
        sr_s_asc(sr,((x_btcl_list_s*)bcore_fork(list)) );
    BLM_DOWN();}
    
    return  0;
}

void x_btcl_selftest( sc_t file )
{
    // bcore_x_btcl.x:1510:1
    BLM_INIT_LEVEL(0);
    sr_s* obj = ((sr_s*)BLM_LEVEL_T_PUSH(0,sr_s,sr_s_create()));
    BLM_TRY_EXIT(x_btcl_parse_create_object(((x_source*)BLM_LEVEL_A_PUSH(0,x_source_check_create_from_file(file ))), obj ))
    ASSERT( obj->o );
    ASSERT( sr_s_is_numeric(obj) );
    ASSERT( sr_s_to_s3(obj) == 0 );
    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------
// group: x_btcl_operator; embeds: bcore_x_btcl_operator.x

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_btcl_operator_uop_s )
"aware x_btcl_operator",
"{",
    "tp_t type;",
    "sr_s a;",
    "hidden x_source_point_s sp;",
    "func ^:is_operator;",
    "func ^:is_exportable_operand;",
    "func ^:signal;",
    "func ^:solve;",
    "func ^:execute;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_btcl_operator_uop_s )

x_btcl_operator_uop_s* x_btcl_operator_uop_s__( x_btcl_operator_uop_s* o, tp_t type, sr_s* a, x_source_point_s* source_point )
{
    // bcore_x_btcl_operator.x:88:5
    
    o->type = type;
    sr_s_tsm(&(o->a),sr_s_type(a), ((x_inst*)bcore_fork(a->o)) );
    if( source_point ) x_source_point_s_copy(&(o->sp),source_point );
    return o;
}

x_btcl_operator_uop_s* x_btcl_operator_uop_s_signal( x_btcl_operator_uop_s* o, tp_t name, x_inst* arg )
{
    // bcore_x_btcl_operator.x:98:5
    
    if( x_btcl_operator_sr_is_operator(&(o->a )) ) x_btcl_operator_a_signal(((x_btcl_operator*)(o->a.o)),name, arg );
    return o;
}

er_t x_btcl_operator_uop_s_solve_exportable_a( const x_btcl_operator_uop_s* o, const sr_s* a, sr_s* result, bl_t* success )
{
    // bcore_x_btcl_operator.x:107:1
    
    (*(success)) = true;
    
    switch( o->type )
    {
        case ((tp_t)(TYPEOF_identity)): sr_s_tsc(result,sr_s_type(a), ((x_inst*)bcore_fork(a->o)) ); return  0;
    
        case ((tp_t)(TYPEOF_neg)):
        {
            if( sr_s_is_integer(a) ) { sr_s_const_from_s3(result,-sr_s_to_s3(a) ); return  0; }
            if( sr_s_is_float(a)   ) { sr_s_const_from_f3(result,-sr_s_to_f3(a) ); return  0; }
        }
        break;
    
        case ((tp_t)(TYPEOF_size)):
        {
            if( sr_s_type(a) == ((tp_t)(TYPEOF_x_btcl_list_s))          ) { sr_s_const_from_s3(result,((const x_btcl_list_s*)(a->o))->arr.size ); return  0; }
            if( x_array_t_is_array(sr_s_type(a) ) ) { sr_s_const_from_s3(result,x_array_t_size(((const x_array*)( a->o)),sr_s_type(a) ) ); return  0; }
        }
        break;
    
        case ((tp_t)(TYPEOF_not)):   if( sr_s_is_numeric(a) ) { sr_s_const_from_bl(result,!sr_s_to_bl(a) ); return  0; } break;
        case ((tp_t)(TYPEOF_exp)):   if( sr_s_is_numeric(a) ) { sr_s_const_from_f3(result,f3_exp(   sr_s_to_f3(a) ) ); return  0; } break;
        case ((tp_t)(TYPEOF_log)):   if( sr_s_is_numeric(a) ) { sr_s_const_from_f3(result,f3_log(   sr_s_to_f3(a) ) ); return  0; } break;
        case ((tp_t)(TYPEOF_log2)):  if( sr_s_is_numeric(a) ) { sr_s_const_from_f3(result,f3_log2(  sr_s_to_f3(a) ) ); return  0; } break;
        case ((tp_t)(TYPEOF_log10)): if( sr_s_is_numeric(a) ) { sr_s_const_from_f3(result,f3_log10( sr_s_to_f3(a) ) ); return  0; } break;
        case ((tp_t)(TYPEOF_sin)):   if( sr_s_is_numeric(a) ) { sr_s_const_from_f3(result,f3_sin(   sr_s_to_f3(a) ) ); return  0; } break;
        case ((tp_t)(TYPEOF_cos)):   if( sr_s_is_numeric(a) ) { sr_s_const_from_f3(result,f3_cos(   sr_s_to_f3(a) ) ); return  0; } break;
        case ((tp_t)(TYPEOF_tan)):   if( sr_s_is_numeric(a) ) { sr_s_const_from_f3(result,f3_tan(   sr_s_to_f3(a) ) ); return  0; } break;
        case ((tp_t)(TYPEOF_tanh)):  if( sr_s_is_numeric(a) ) { sr_s_const_from_f3(result,f3_tanh(  sr_s_to_f3(a) ) ); return  0; } break;
        case ((tp_t)(TYPEOF_sqrt)):  if( sr_s_is_numeric(a) ) { sr_s_const_from_f3(result,f3_sqrt(  sr_s_to_f3(a) ) ); return  0; } break;
        case ((tp_t)(TYPEOF_sign)):  if( sr_s_is_numeric(a) ) { sr_s_const_from_s3(result,f3_sign(  sr_s_to_f3(a) ) ); return  0; } break;
    
        case ((tp_t)(TYPEOF_abs)):
        {
            if( sr_s_is_integer(a) ) { sr_s_const_from_s3(result,s3_abs( sr_s_to_s3(a) ) ); return  0; }
            if( sr_s_is_numeric(a) ) { sr_s_const_from_f3(result,f3_abs( sr_s_to_f3(a) ) ); return  0; }
        }
        break;
    
        case ((tp_t)(TYPEOF_ceil)):  if( sr_s_is_numeric(a) ) { sr_s_const_from_f3(result,f3_ceil(  sr_s_to_f3(a) ) ); return  0; } break;
        case ((tp_t)(TYPEOF_floor)): if( sr_s_is_numeric(a) ) { sr_s_const_from_f3(result,f3_floor( sr_s_to_f3(a) ) ); return  0; } break;
    
        default: break;
    }
    
    (*(success)) = false;
    return  0;
}

er_t x_btcl_operator_uop_s_solve( x_btcl_operator_uop_s* o, x_btcl_frame_s* frame, sr_s* result, bl_t* success )
{
    // bcore_x_btcl_operator.x:160:1
    
    BLM_TRY(x_btcl_operator_uop_s_solve_exportable_a(o,&(o->a), result, success ))
    if( (*(success)) ) return  0;
    
    if( x_btcl_operator_sr_is_exportable_operand(&(o->a )) )
    {
        sr_s_asc(result,((x_btcl_operator_uop_s*)bcore_fork(o)) );
        (*(success)) = true;
    }
    
    return  0;
}

er_t x_btcl_operator_uop_s_execute( const x_btcl_operator_uop_s* o, sr_s* result )
{
    // bcore_x_btcl_operator.x:176:1
    BLM_INIT_LEVEL(0);
    const sr_s* a = NULL;
    if( x_btcl_operator_sr_is_operator(&(o->a )) )
    {
        sr_s* r = ((sr_s*)BLM_LEVEL_T_PUSH(0,sr_s,sr_s_create()));
        BLM_TRY(x_btcl_operator_a_execute(((const x_btcl_operator*)(o->a.o)),r ))
        a = r;
    }
    else
    {
        a = (&(o->a));
    }
    
    bl_t success = false;
    BLM_TRY(x_btcl_operator_uop_s_solve_exportable_a(o,a, result,&( success )))
    if( success ) BLM_RETURNV(er_t, 0)
    
    BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(&(o->sp),"Operator #<sc_t> #<sc_t> is not executable.\n", x_btcl_operator_symbol(o->type ), bnameof( sr_s_type(a) ) ))
}

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_btcl_operator_bop_s )
"aware x_btcl_operator",
"{",
    "tp_t type;",
    "sr_s a;",
    "sr_s b;",
    "hidden x_source_point_s sp;",
    "func ^:is_operator;",
    "func ^:is_exportable_operand;",
    "func ^:signal;",
    "func ^:solve;",
    "func ^:execute;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_btcl_operator_bop_s )

x_btcl_operator_bop_s* x_btcl_operator_bop_s__( x_btcl_operator_bop_s* o, tp_t type, sr_s* a, sr_s* b, x_source_point_s* source_point )
{
    // bcore_x_btcl_operator.x:244:5
    
    o->type = type;
    sr_s_tsm(&(o->a),sr_s_type(a), ((x_inst*)bcore_fork(a->o)) );
    sr_s_tsm(&(o->b),sr_s_type(b), ((x_inst*)bcore_fork(b->o)) );
    if( source_point ) x_source_point_s_copy(&(o->sp),source_point );
    return o;
}

x_btcl_operator_bop_s* x_btcl_operator_bop_s_signal( x_btcl_operator_bop_s* o, tp_t name, x_inst* arg )
{
    // bcore_x_btcl_operator.x:255:5
    
    if( x_btcl_operator_sr_is_operator(&(o->a )) ) x_btcl_operator_a_signal(((x_btcl_operator*)(o->a.o)),name, arg );
    if( x_btcl_operator_sr_is_operator(&(o->b )) ) x_btcl_operator_a_signal(((x_btcl_operator*)(o->b.o)),name, arg );
    return o;
}

er_t x_btcl_operator_bop_s_solve_exportable_a_b( const x_btcl_operator_bop_s* o, const sr_s* a, const sr_s* b, sr_s* result, bl_t* success )
{
    // bcore_x_btcl_operator.x:291:1
    
    (*(success)) = true;
    
    switch( o->type )
    {
        case ((tp_t)(TYPEOF_pow)):
        {
            if( bcore_tp_is_numeric( sr_s_type(a) ) && bcore_tp_is_numeric( sr_s_type(b) ) )
            {
                sr_s_const_from_f3(result,pow( sr_s_to_f3(a), sr_s_to_f3(b) ) );
                return  0;
            }
        }
        break;
    
        case ((tp_t)(TYPEOF_div)):
        {
            if( bcore_tp_is_numeric( sr_s_type(a) ) && bcore_tp_is_numeric( sr_s_type(b) ) )
            {
                if( sr_s_to_f3(b) == 0 ) return  x_source_point_s_parse_error_fa(&(o->sp),"Division by zero.\n" );
                sr_s_const_from_f3(result,sr_s_to_f3(a) / sr_s_to_f3(b) );
                return  0;
            }
        }
        break;
    
        case ((tp_t)(TYPEOF_mod)):
        {
            if( bcore_tp_is_integer( sr_s_type(a) ) && bcore_tp_is_integer( sr_s_type(b) ) )
            {
                if( sr_s_to_s3(b) == 0 ) return  x_source_point_s_parse_error_fa(&(o->sp),"Modulo division by zero.\n" );
                sr_s_const_from_s3(result,sr_s_to_s3(a) % sr_s_to_s3(b) );
                return  0;
            }
        }
        break;
    
        case ((tp_t)(TYPEOF_mul)):
        {
            if( bcore_tp_is_numeric( sr_s_type(a) ) && bcore_tp_is_numeric( sr_s_type(b) ) )
            {
                if( bcore_tp_is_integer( sr_s_type(a) ) && bcore_tp_is_integer( sr_s_type(b) ) )
                {
                    sr_s_const_from_s3(result,sr_s_to_s3(a) * sr_s_to_s3(b) );
                }
                else
                {
                    sr_s_const_from_f3(result,sr_s_to_f3(a) * sr_s_to_f3(b) );
                }
                return  0;
            }
            else if( sr_s_type(a) == ((tp_t)(TYPEOF_x_btcl_list_s)) )
            {
                const x_btcl_list_s* list_a = ((const x_btcl_list_s*)(a->o));
                if( sr_s_type(b) == ((tp_t)(TYPEOF_x_btcl_list_s)) )
                {BLM_INIT_LEVEL(6);
                    const x_btcl_list_s* list_b = ((const x_btcl_list_s*)(b->o));
                    x_btcl_list_s* list_r = ((x_btcl_list_s*)BLM_LEVEL_T_PUSH(6,x_btcl_list_s,x_btcl_list_s_create()));
                    x_btcl_list_s_set_size(list_r,x_btcl_list_s_size(list_a) * x_btcl_list_s_size(list_b) );
                    for(sz_t i = 0; i < x_btcl_list_s_size(list_a); i++ )
                    {
                        sr_s* sa =&( list_a->arr.data[ i ]);
    
                        for(sz_t j = 0; j < x_btcl_list_s_size(list_b); j++ )
                        {
                            sr_s* sb =&( list_b->arr.data[ j ]);
                            BLM_TRY(x_btcl_bop_cat_ab(sa, sb,&( list_r->arr.data[ i * x_btcl_list_s_size(list_b) + j ] )))
                        }
                    }
                    sr_s_asc(result,((x_btcl_list_s*)bcore_fork(list_r)) );
                    BLM_RETURNV(er_t, 0)
                BLM_DOWN();}
            }
        }
        break;
    
        case ((tp_t)(TYPEOF_sub)):
        {
            if( bcore_tp_is_numeric( sr_s_type(a) ) && bcore_tp_is_numeric( sr_s_type(b) ) )
            {
                if( bcore_tp_is_integer( sr_s_type(a) ) && bcore_tp_is_integer( sr_s_type(b) ) )
                {
                    sr_s_const_from_s3(result,sr_s_to_s3(a) - sr_s_to_s3(b) );
                }
                else
                {
                    sr_s_const_from_f3(result,sr_s_to_f3(a) - sr_s_to_f3(b) );
                }
                return  0;
            }
        }
        break;
    
        case ((tp_t)(TYPEOF_max)):
        {
            if( bcore_tp_is_numeric( sr_s_type(a) ) && bcore_tp_is_numeric( sr_s_type(b) ) )
            {
                if( bcore_tp_is_integer( sr_s_type(a) ) && bcore_tp_is_integer( sr_s_type(b) ) )
                {
                    sr_s_const_from_s3(result,s3_max( sr_s_to_s3(a), sr_s_to_s3(b) ) );
                }
                else
                {
                    sr_s_const_from_f3(result,f3_max( sr_s_to_f3(a), sr_s_to_f3(b) ) );
                }
                return  0;
            }
        }
        break;
    
        case ((tp_t)(TYPEOF_min)):
        {
            if( bcore_tp_is_numeric( sr_s_type(a) ) && bcore_tp_is_numeric( sr_s_type(b) ) )
            {
                if( bcore_tp_is_integer( sr_s_type(a) ) && bcore_tp_is_integer( sr_s_type(b) ) )
                {
                    sr_s_const_from_s3(result,s3_min( sr_s_to_s3(a), sr_s_to_s3(b) ) );
                }
                else
                {
                    sr_s_const_from_f3(result,f3_min( sr_s_to_f3(a), sr_s_to_f3(b) ) );
                }
                return  0;
            }
        }
        break;
    
        case ((tp_t)(TYPEOF_add)):
        {
            if( bcore_tp_is_numeric( sr_s_type(a) ) )
            {
                if( bcore_tp_is_numeric( sr_s_type(b) ) )
                {
                    if( bcore_tp_is_integer( sr_s_type(a) ) && bcore_tp_is_integer( sr_s_type(b) ) )
                    {
                        sr_s_const_from_s3(result,sr_s_to_s3(a) + sr_s_to_s3(b) );
                    }
                    else
                    {
                        sr_s_const_from_f3(result,sr_s_to_f3(a) + sr_s_to_f3(b) );
                    }
                    return  0;
                }
            }
    
            if( sr_s_type(a) == ((tp_t)(TYPEOF_st_s)) || sr_s_type(b) == ((tp_t)(TYPEOF_st_s)) )
            {BLM_INIT_LEVEL(4);
                st_s* st1 = ((st_s*)BLM_LEVEL_T_PUSH(4,st_s,st_s_create()));
                st_s* st2 = ((st_s*)BLM_LEVEL_T_PUSH(4,st_s,st_s_create()));
                if( st_s_copy_typed(st1,sr_s_type(a), a->o ) ) {BLM_INIT_LEVEL(5); BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(&(o->sp),"operator '+': #<sc_t>\n", bcore_error_pop_all_to_st(((st_s*)BLM_LEVEL_T_PUSH(5,st_s,st_s_create())) )->sc )) BLM_DOWN();}
                if( st_s_copy_typed(st2,sr_s_type(b), b->o ) ) {BLM_INIT_LEVEL(5); BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(&(o->sp),"operator '+': #<sc_t>\n", bcore_error_pop_all_to_st(((st_s*)BLM_LEVEL_T_PUSH(5,st_s,st_s_create())) )->sc )) BLM_DOWN();}
                st_s_push_st(st1,st2 );
    
                sr_s_asc(result,((st_s*)bcore_fork(st1)) );
                BLM_RETURNV(er_t, 0)
            BLM_DOWN();}
        }
        break;
    
        case ((tp_t)(TYPEOF_spawn)):
        {
            if( sr_s_is_numeric(a) && sr_s_type(b) != ((tp_t)(TYPEOF_x_btcl_function_s)) )
            {BLM_INIT_LEVEL(4);
                x_btcl_list_s* list = ((x_btcl_list_s*)BLM_LEVEL_T_PUSH(4,x_btcl_list_s,x_btcl_list_s_create()));
                bcore_arr_sr_s_set_size(&(list->arr),sr_s_to_s3(a) );
                for(sz_t i = 0; i < list->arr.size; i++ )
                {
                    sr_s_tsc(&(list->arr.data[ i ]),sr_s_type(b), x_inst_t_clone( b->o,sr_s_type(b) ) );
                }
                sr_s_asc(result,((x_btcl_list_s*)bcore_fork(list)) );
                BLM_RETURNV(er_t, 0)
            BLM_DOWN();}
        }
        break;
    
        case ((tp_t)(TYPEOF_cat)): return  x_btcl_bop_cat_ab(((sr_s*)(a)), ((sr_s*)(b)), result );
    
        case ((tp_t)(TYPEOF_equal)):
        {
            if( x_btcl_operator_is_comparable(a, b ) )
            {
                sr_s_from_bl(result,0 == x_compare_t_num_dominant(sr_s_type(a), a->o, sr_s_type(b), b->o ) );
                return  0;
            }
        }
        break;
    
        case ((tp_t)(TYPEOF_unequal)):
        {
            if( x_btcl_operator_is_comparable(a, b ) )
            {
                sr_s_from_bl(result,0 != x_compare_t_num_dominant(sr_s_type(a), a->o, sr_s_type(b), b->o ) );
                return  0;
            }
        }
        break;
    
        case ((tp_t)(TYPEOF_larger_equal)):
        {
            if( x_btcl_operator_is_comparable(a, b ) )
            {
                sr_s_from_bl(result,0 >= x_compare_t_num_dominant(sr_s_type(a), a->o, sr_s_type(b), b->o ) );
                return  0;
            }
        }
        break;
    
    
        case ((tp_t)(TYPEOF_larger)):
        {
            if( x_btcl_operator_is_comparable(a, b ) )
            {
                sr_s_from_bl(result,0 >  x_compare_t_num_dominant(sr_s_type(a), a->o, sr_s_type(b), b->o ) );
                return  0;
            }
        }
        break;
    
        case ((tp_t)(TYPEOF_smaller_equal)):
        {
            if( x_btcl_operator_is_comparable(a, b ) )
            {
                sr_s_from_bl(result,0 <= x_compare_t_num_dominant(sr_s_type(a), a->o, sr_s_type(b), b->o ) );
                return  0;
            }
        }
        break;
    
        case ((tp_t)(TYPEOF_smaller)):
        {
            if( x_btcl_operator_is_comparable(a, b ) )
            {
                sr_s_from_bl(result,0 <  x_compare_t_num_dominant(sr_s_type(a), a->o, sr_s_type(b), b->o ) );
                return  0;
            }
        }
        break;
    
        case ((tp_t)(TYPEOF_and)): if( sr_s_is_numeric(a) && sr_s_is_numeric(b) ) { sr_s_from_bl(result,sr_s_to_bl(a) && sr_s_to_bl(b) ); return  0; } break;
        case ((tp_t)(TYPEOF_or)):  if( sr_s_is_numeric(a) && sr_s_is_numeric(b) ) { sr_s_from_bl(result,sr_s_to_bl(a) || sr_s_to_bl(b) ); return  0; } break;
    
        case ((tp_t)(TYPEOF_shift_left)):
        {
            if( sr_s_type(a) == ((tp_t)(TYPEOF_x_btcl_net_node_s)) )
            {BLM_INIT_LEVEL(4);
                x_btcl_net_node_s* node = ((x_btcl_net_node_s*)BLM_LEVEL_T_PUSH(4,x_btcl_net_node_s,x_btcl_net_node_s_clone(((x_btcl_net_node_s*)(a->o)))));
                x_btcl_net_node_s_push_plain_branch(node,0, false,&( o->sp), ((sr_s*)(b)) );
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

er_t x_btcl_operator_bop_s_solve( x_btcl_operator_bop_s* o, x_btcl_frame_s* frame, sr_s* result, bl_t* success )
{
    // bcore_x_btcl_operator.x:554:1
    
    // exportable operations
    BLM_TRY(x_btcl_operator_bop_s_solve_exportable_a_b(o,&(o->a),&( o->b), result, success ))
    if( (*(success)) ) return  0;
    
    (*(success)) = true;
    
    // residual (not exportable) operations
    switch( o->type )
    {
        case ((tp_t)(TYPEOF_chain)):
        {
            if( sr_s_type(&(o->a)) == ((tp_t)(TYPEOF_x_btcl_function_s)) && sr_s_type(&(o->b)) == ((tp_t)(TYPEOF_x_btcl_function_s)) )
            {BLM_INIT_LEVEL(4);
                x_btcl_function_s* fa = ((x_btcl_function_s*)BLM_LEVEL_T_PUSH(4,x_btcl_function_s,((x_btcl_function_s*)bcore_fork(((x_btcl_function_s*)(o->a.o))))));
                x_btcl_function_s* fb = ((x_btcl_function_s*)BLM_LEVEL_T_PUSH(4,x_btcl_function_s,((x_btcl_function_s*)bcore_fork(((x_btcl_function_s*)(o->b.o))))));
    
                if( !x_btcl_function_s_is_unary(fa) ) BLM_TRY(x_source_point_s_parse_error_fa(&(o->sp),"Operator *: Left argument is not a unary function.\n" ))
    
                x_btcl_function_s* fc = x_btcl_function_s_setup(((x_btcl_function_s*)BLM_LEVEL_T_PUSH(4,x_btcl_function_s,x_btcl_function_s_create())),fb->signature, fb->block, fb->tail );
                x_btcl_function_s_append_tail(fc,fa );
    
                sr_s_asm(result,((x_btcl_function_s*)bcore_fork(fc)) );
                BLM_RETURNV(er_t, 0)
            BLM_DOWN();}
        }
        break;
    
        case ((tp_t)(TYPEOF_mul)):
        {
            if( sr_s_type(&(o->a)) == ((tp_t)(TYPEOF_x_btcl_function_s)) )
            {BLM_INIT_LEVEL(4);
                x_btcl_function_s* f = ((x_btcl_function_s*)BLM_LEVEL_T_PUSH(4,x_btcl_function_s,((x_btcl_function_s*)bcore_fork(((x_btcl_function_s*)(o->a.o))))));
                bcore_arr_sr_s* arg_list = ((bcore_arr_sr_s*)BLM_LEVEL_T_PUSH(4,bcore_arr_sr_s,bcore_arr_sr_s_create()));
                sr_s_fork_from(bcore_arr_sr_s_push_sr(arg_list,sr_null() ),&(o->b ));
                BLM_TRY(x_btcl_function_s_call(f,&(o->sp), frame, arg_list, result ))
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
                    if( x_btcl_function_s_args(function) != 1 ) BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(&(o->sp),"Operator #<sc_t> :: #<sc_t>: Right operand must be unary (single argument).\n", bnameof( sr_s_type(&(o->a)) ), bnameof( sr_s_type(&(o->b)) ) ))
                    tp_t arg_name = x_btcl_function_s_arg_name(function,0 );
                    for(sz_t i = 0; i < list->arr.size; i++ )
                    {
                        x_btcl_frame_s_var_set(local_frame,arg_name, sr_s3(i ) );
                        BLM_TRY(x_btcl_block_s_eval(function->block,local_frame,&( list->arr.data[ i ] )))
                    }
                    sr_s_asc(result,((x_btcl_list_s*)bcore_fork(list)) );
                    BLM_RETURNV(er_t, 0)
                BLM_DOWN();}
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
                            BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(&(o->sp),"Operator #<sc_t> :: #<sc_t>: Left operand must be a list of size >= 1.\n", bnameof( sr_s_type(&(o->a)) ), bnameof( sr_s_type(&(o->b)) ) ))
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
                        BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(&(o->sp),"Operator #<sc_t> :: #<sc_t>: Right operand must be unary (one argument) or binary (two arguments).\n", bnameof( sr_s_type(&(o->a)) ), bnameof( sr_s_type(&(o->b)) ) ))
                    }
                BLM_DOWN();}
                else if( sr_s_type(&(o->b)) == ((tp_t)(TYPEOF_x_btcl_list_s)) )
                {BLM_INIT_LEVEL(7);
                    const x_btcl_list_s* list_b = ((const x_btcl_list_s*)(o->b.o));
                    x_btcl_list_s* list_r = ((x_btcl_list_s*)BLM_LEVEL_T_PUSH(7,x_btcl_list_s,x_btcl_list_s_create()));
    
                    sz_t smin = sz_min( x_btcl_list_s_size(list_a), x_btcl_list_s_size(list_b) );
                    sz_t smax = sz_max( x_btcl_list_s_size(list_a), x_btcl_list_s_size(list_b) );
    
                    x_btcl_list_s_set_size(list_r,smax );
    
                    for(sz_t i = 0; i < smin; i++ ) BLM_TRY(x_btcl_bop_cat_ab(&(list_a->arr.data[ i ]),&( list_b->arr.data[ i ]),&( list_r->arr.data[ i ] )))
    
                    if( x_btcl_list_s_size(list_a) != x_btcl_list_s_size(list_b) )
                    {BLM_INIT_LEVEL(8);
    
                        sr_s* sr_list_empty = sr_s_asm(((sr_s*)BLM_LEVEL_T_PUSH(8,sr_s,sr_s_create())),x_btcl_list_s_create() );
                        for(sz_t i = smin; i < x_btcl_list_s_size(list_a); i++ ) BLM_TRY(x_btcl_bop_cat_ab(&(list_a->arr.data[ i ]), sr_list_empty,&(    list_r->arr.data[ i ] )))
                        for(sz_t i = smin; i < x_btcl_list_s_size(list_b); i++ ) BLM_TRY(x_btcl_bop_cat_ab(sr_list_empty,&(    list_b->arr.data[ i ]),&( list_r->arr.data[ i ] )))
                    BLM_DOWN();}
    
                    sr_s_asm(result,((x_btcl_list_s*)bcore_fork(list_r)) );
                    BLM_RETURNV(er_t, 0)
                BLM_DOWN();}
            }
        }
        break;
    
        case ((tp_t)(TYPEOF_shift_left)):
        {
            if( sr_s_type(&(o->a)) == ((tp_t)(TYPEOF_x_btcl_function_s)) )
            {BLM_INIT_LEVEL(4);
                x_btcl_function_s* f = ((x_btcl_function_s*)BLM_LEVEL_T_PUSH(4,x_btcl_function_s,((x_btcl_function_s*)bcore_fork(((x_btcl_function_s*)(o->a.o))))));
                bcore_arr_sr_s* arg_list = ((bcore_arr_sr_s*)BLM_LEVEL_T_PUSH(4,bcore_arr_sr_s,bcore_arr_sr_s_create()));
                sr_s_fork_from(bcore_arr_sr_s_push_sr(arg_list,sr_null() ),&(o->b ));
                BLM_TRY(x_btcl_function_s_call(f,&(o->sp), frame, arg_list, result ))
                BLM_RETURNV(er_t, 0)
            BLM_DOWN();}
        }
        break;
    
        default: break;
    }
    
    if( x_btcl_operator_sr_is_exportable_operand(&(o->a )) )
    {
        if( x_btcl_operator_sr_is_exportable_operand(&(o->b )) ) { sr_s_asc(result,((x_btcl_operator_bop_s*)bcore_fork(o)) ); return  0; }
        if( sr_s_is_numeric(&(o->b)) ) { sr_s_asc(result,((x_btcl_operator_bop_s*)bcore_fork(o)) ); return  0; }
    }
    
    if( x_btcl_operator_sr_is_exportable_operand(&(o->b )) )
    {
        if( sr_s_is_numeric(&(o->a)) ) { sr_s_asc(result,((x_btcl_operator_bop_s*)bcore_fork(o)) ); return  0; }
    }
    
    (*(success)) = false;
    return  0;
}

er_t x_btcl_operator_bop_s_execute( const x_btcl_operator_bop_s* o, sr_s* result )
{
    // bcore_x_btcl_operator.x:725:1
    BLM_INIT_LEVEL(0);
    const sr_s* a = NULL;
    const sr_s* b = NULL;
    if( x_btcl_operator_sr_is_operator(&(o->a )) )
    {
        sr_s* r = ((sr_s*)BLM_LEVEL_T_PUSH(0,sr_s,sr_s_create()));
        BLM_TRY(x_btcl_operator_a_execute(((const x_btcl_operator*)(o->a.o)),r ))
        a = r;
    }
    else
    {
        a = (&(o->a));
    }
    
    if( x_btcl_operator_sr_is_operator(&(o->b )) )
    {
        sr_s* r = ((sr_s*)BLM_LEVEL_T_PUSH(0,sr_s,sr_s_create()));
        BLM_TRY(x_btcl_operator_a_execute(((const x_btcl_operator*)(o->b.o)),r ))
        b = r;
    }
    else
    {
        b = (&(o->b));
    }
    
    bl_t success = false;
    BLM_TRY(x_btcl_operator_bop_s_solve_exportable_a_b(o,a, b, result,&( success )))
    if( success ) BLM_RETURNV(er_t, 0)
    
    BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(&(o->sp),"Operator #<sc_t> #<sc_t> #<sc_t> is not executable.\n", bnameof( sr_s_type(a) ), x_btcl_operator_symbol(o->type ), bnameof( sr_s_type(b) ) ))
}

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_btcl_operator_top_s )
"aware x_btcl_operator",
"{",
    "tp_t type;",
    "sr_s a;",
    "sr_s b;",
    "sr_s c;",
    "hidden x_source_point_s sp;",
    "func ^:is_operator;",
    "func ^:is_exportable_operand;",
    "func ^:signal;",
    "func ^:solve;",
    "func ^:execute;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_btcl_operator_top_s )

x_btcl_operator_top_s* x_btcl_operator_top_s__( x_btcl_operator_top_s* o, tp_t type, sr_s* a, sr_s* b, sr_s* c, x_source_point_s* source_point )
{
    // bcore_x_btcl_operator.x:778:5
    
    o->type = type;
    sr_s_tsm(&(o->a),sr_s_type(a), ((x_inst*)bcore_fork(a->o)) );
    sr_s_tsm(&(o->b),sr_s_type(b), ((x_inst*)bcore_fork(b->o)) );
    sr_s_tsm(&(o->c),sr_s_type(c), ((x_inst*)bcore_fork(c->o)) );
    if( source_point ) x_source_point_s_copy(&(o->sp),source_point );
    return o;
}

x_btcl_operator_top_s* x_btcl_operator_top_s_signal( x_btcl_operator_top_s* o, tp_t name, x_inst* arg )
{
    // bcore_x_btcl_operator.x:790:5
    
    if( x_btcl_operator_sr_is_operator(&(o->a )) ) x_btcl_operator_a_signal(((x_btcl_operator*)(o->a.o)),name, arg );
    if( x_btcl_operator_sr_is_operator(&(o->b )) ) x_btcl_operator_a_signal(((x_btcl_operator*)(o->b.o)),name, arg );
    if( x_btcl_operator_sr_is_operator(&(o->c )) ) x_btcl_operator_a_signal(((x_btcl_operator*)(o->c.o)),name, arg );
    return o;
}

er_t x_btcl_operator_top_s_solve_exportable_a_b_c( const x_btcl_operator_top_s* o, const sr_s* a, const sr_s* b, const sr_s* c, sr_s* result, bl_t* success )
{
    // bcore_x_btcl_operator.x:822:1
    
    (*(success)) = true;
    
    switch( o->type )
    {
        case ((tp_t)(TYPEOF_conditional)):
        {
            if( sr_s_is_numeric(a) )
            {
                if( sr_s_to_bl(a) )
                {
                    sr_s_tsc(result,sr_s_type(b), ((x_inst*)bcore_fork(b->o)) );
                }
                else
                {
                    sr_s_tsc(result,sr_s_type(c), ((x_inst*)bcore_fork(c->o)) );
                }
                return  0;
            }
        }
        break;
    
        default: break;
    }
    
    (*(success)) = false;
    return  0;
}

er_t x_btcl_operator_top_s_solve( x_btcl_operator_top_s* o, x_btcl_frame_s* frame, sr_s* result, bl_t* success )
{
    // bcore_x_btcl_operator.x:854:1
    
    // exportable operations
    BLM_TRY(x_btcl_operator_top_s_solve_exportable_a_b_c(o,&(o->a),&( o->b),&( o->c), result, success ))
    if( (*(success)) ) return  0;
    
    (*(success)) = true;
    
    // residual (not exportable) operations
    switch( o->type )
    {
        default: break;
    }
    
    if
    (
           ( x_btcl_operator_sr_is_exportable_operand(&(o->a )) || sr_s_is_numeric(&(o->a)) )
        && ( x_btcl_operator_sr_is_exportable_operand(&(o->b )) || sr_s_is_numeric(&(o->b)) )
        && ( x_btcl_operator_sr_is_exportable_operand(&(o->c )) || sr_s_is_numeric(&(o->c)) )
    )
    {
        sr_s_asc(result,((x_btcl_operator_top_s*)bcore_fork(o)) );
        return  0;
    }
    
    (*(success)) = false;
    return  0;
}

er_t x_btcl_operator_top_s_execute( const x_btcl_operator_top_s* o, sr_s* result )
{
    // bcore_x_btcl_operator.x:885:1
    BLM_INIT_LEVEL(0);
    const sr_s* a = NULL;
    const sr_s* b = NULL;
    const sr_s* c = NULL;
    if( x_btcl_operator_sr_is_operator(&(o->a )) )
    {
        sr_s* r = ((sr_s*)BLM_LEVEL_T_PUSH(0,sr_s,sr_s_create()));
        BLM_TRY(x_btcl_operator_a_execute(((const x_btcl_operator*)(o->a.o)),r ))
        a = r;
    }
    else
    {
        a = (&(o->a));
    }
    
    if( x_btcl_operator_sr_is_operator(&(o->b )) )
    {
        sr_s* r = ((sr_s*)BLM_LEVEL_T_PUSH(0,sr_s,sr_s_create()));
        BLM_TRY(x_btcl_operator_a_execute(((const x_btcl_operator*)(o->b.o)),r ))
        b = r;
    }
    else
    {
        b = (&(o->b));
    }
    
    if( x_btcl_operator_sr_is_operator(&(o->c )) )
    {
        sr_s* r = ((sr_s*)BLM_LEVEL_T_PUSH(0,sr_s,sr_s_create()));
        BLM_TRY(x_btcl_operator_a_execute(((const x_btcl_operator*)(o->c.o)),r ))
        c = r;
    }
    else
    {
        c = (&(o->c));
    }
    
    bl_t success = false;
    BLM_TRY(x_btcl_operator_top_s_solve_exportable_a_b_c(o,a, b, c, result,&( success )))
    if( success ) BLM_RETURNV(er_t, 0)
    
    BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(&(o->sp),"Operator '#<sc_t> #<sc_t> #<sc_t> '<div-operator>' #<sc_t>' is not executable.\n", bnameof( sr_s_type(a) ), x_btcl_operator_symbol(o->type ), bnameof( sr_s_type(b) ), bnameof( sr_s_type(c) ) ))
}

XOILA_DEFINE_SPECT_NASC_BEGIN( x_btcl, x_btcl_operator )
"{",
    "bcore_spect_header_s header;",
    "feature aware x_btcl_operator : is_operator = x_btcl_operator_is_operator_default;",
    "feature aware x_btcl_operator : is_exportable_operand = x_btcl_operator_is_exportable_operand_default;",
    "feature aware x_btcl_operator : solve = x_btcl_operator_solve_default;",
    "feature aware x_btcl_operator : signal = x_btcl_operator_signal_default;",
    "feature aware x_btcl_operator : execute = x_btcl_operator_execute_default;",
"}",
XOILA_DEFINE_SPECT_NASC_END( x_btcl, x_btcl_operator )

er_t x_btcl_operator_solve_default( x_btcl_operator* o, x_btcl_frame_s* frame, sr_s* result, bl_t* success )
{
    // bcore_x_btcl_operator.x:53:1
    
    return  EM_ERR_fa("'#<sc_t>' is no operator.", bnameof( o->_ ) );
}

x_btcl_operator* x_btcl_operator_signal_default( x_btcl_operator* o, tp_t name, x_inst* arg )
{
    // bcore_x_btcl_operator.x:59:1
    
    ERR_fa( "'#<sc_t>' is no operator.", bnameof( o->_ ) );
    return o;
}

er_t x_btcl_operator_execute_default( const x_btcl_operator* o, sr_s* result )
{
    // bcore_x_btcl_operator.x:65:1
    
    return  EM_ERR_fa("'#<sc_t>' is no operator.", bnameof( o->_ ) );
}

er_t x_btcl_operator_eval_uop_type( x_btcl_frame_s* frame, tp_t type, s2_t priority, x_source* source, bl_t postfix, sr_s* result )
{
    // bcore_x_btcl_operator.x:200:1
    BLM_INIT_LEVEL(0);
    x_source_point_s* source_point = x_source_point_s__(((x_source_point_s*)BLM_LEVEL_T_PUSH(0,x_source_point_s,x_source_point_s_create())),source );
    sr_s* sa = ((sr_s*)BLM_LEVEL_T_PUSH(0,sr_s,sr_s_create()));
    
    if( postfix )
    {
        (*(sa)) = (*(result));
        (*(result)) = sr_null();
    }
    else
    {
        BLM_TRY(x_btcl_frame_s_eval(frame,priority, source, sa ))
    }
    
    x_btcl_operator_uop_s* uop = x_btcl_operator_uop_s__(((x_btcl_operator_uop_s*)BLM_LEVEL_T_PUSH(0,x_btcl_operator_uop_s,x_btcl_operator_uop_s_create())),type, sa, source_point );
    bl_t success = false;
    BLM_TRY(x_btcl_operator_uop_s_solve(uop,frame, result,&( success )))
    if( !success )
    {
        BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(source_point,"Unary operator '#<sc_t>' on '#<sc_t>' is not defined.\n", x_btcl_operator_symbol(type ), bnameof( sr_s_o_type(sa) ) ))
    }
    BLM_RETURNV(er_t, 0)
}

er_t x_btcl_operator_eval_bop_type( x_btcl_frame_s* frame, tp_t type, s2_t priority, x_source* source, sr_s* sa, sr_s* result )
{
    // bcore_x_btcl_operator.x:264:1
    BLM_INIT_LEVEL(0);
    x_source_point_s* source_point = x_source_point_s__(((x_source_point_s*)BLM_LEVEL_T_PUSH(0,x_source_point_s,x_source_point_s_create())),source );
    sr_s sb;BLM_T_INIT_SPUSH(sr_s, &sb);; BLM_TRY(x_btcl_frame_s_eval(frame,priority, source,&( sb )))
    x_btcl_operator_bop_s* bop = x_btcl_operator_bop_s__(((x_btcl_operator_bop_s*)BLM_LEVEL_T_PUSH(0,x_btcl_operator_bop_s,x_btcl_operator_bop_s_create())),type, sa,&( sb), source_point );
    bl_t success = false;
    BLM_TRY(x_btcl_operator_bop_s_solve(bop,frame, result,&( success )))
    if( !success )
    {
        BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(source_point,"Operator '#<sc_t> #<sc_t> #<sc_t>' is not defined.\n", bnameof( sr_s_o_type(sa) ), x_btcl_operator_symbol(type ), bnameof( sr_s_o_type(&(sb)) ) ))
    }
    BLM_RETURNV(er_t, 0)
}

bl_t x_btcl_operator_is_comparable( const sr_s* a, const sr_s* b )
{
    // bcore_x_btcl_operator.x:280:1
    
    return 
    ( sr_s_is_numeric(a) && sr_s_is_numeric(b) ) ||
    ( ( sr_s_type(a) == ((tp_t)(TYPEOF_st_s)) ) && ( sr_s_type(b) == ((tp_t)(TYPEOF_st_s)) ) ) ||
    ( ( sr_s_type(a) == ((tp_t)(TYPEOF_x_btcl_list_s)) ) && ( sr_s_type(b) == ((tp_t)(TYPEOF_x_btcl_list_s)) ) );
}

er_t x_btcl_operator_eval_top_type( x_btcl_frame_s* frame, tp_t type, s2_t priority, x_source* source, sr_s* sa, sr_s* result )
{
    // bcore_x_btcl_operator.x:800:1
    BLM_INIT_LEVEL(0);
    x_source_point_s* source_point = x_source_point_s__(((x_source_point_s*)BLM_LEVEL_T_PUSH(0,x_source_point_s,x_source_point_s_create())),source );
    sr_s sb;BLM_T_INIT_SPUSH(sr_s, &sb);; BLM_TRY(x_btcl_frame_s_eval(frame,priority, source,&( sb )))
    
    BLM_TRY(x_source_parse_fa(source," :" ))
    
    sr_s sc;BLM_T_INIT_SPUSH(sr_s, &sc);; BLM_TRY(x_btcl_frame_s_eval(frame,priority, source,&( sc )))
    
    x_btcl_operator_top_s* top = x_btcl_operator_top_s__(((x_btcl_operator_top_s*)BLM_LEVEL_T_PUSH(0,x_btcl_operator_top_s,x_btcl_operator_top_s_create())),type, sa,&( sb),&( sc), source_point );
    bl_t success = false;
    BLM_TRY(x_btcl_operator_top_s_solve(top,frame, result,&( success )))
    if( !success )
    {
        BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(source_point,"Operator '#<sc_t> #<sc_t> #<sc_t> '<div-operator>' #<sc_t>' is not defined.\n", bnameof( sr_s_type(sa) ), x_btcl_operator_symbol(type ), bnameof( sr_s_type(&(sb)) ), bnameof( sr_s_type(&(sc)) ) ))
    }
    BLM_RETURNV(er_t, 0)
}

//----------------------------------------------------------------------------------------------------------------------
// group: x_btcl_operator_f3; embeds: bcore_x_btcl_operator_f3.x

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_btcl_operator_f3_arg_s )
"aware x_btcl_operator_f3",
"{",
    "f3_t val;",
    "tp_t name;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_btcl_operator_f3_arg_s )

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_btcl_operator_f3_arg_arr_s )
"aware x_array",
"{",
    "x_btcl_operator_f3_arg_s [];",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_btcl_operator_f3_arg_arr_s )

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_btcl_operator_f3_arg_nop_s )
"aware x_btcl_operator_f3",
"{",
    "sz_t index = -1;",
    "tp_t name;",
    "private f3_t* val;",
    "hidden x_source_point_s sp;",
    "func ^:set_args;",
    "func ^:get;",
    "func ^:check_consistency;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_btcl_operator_f3_arg_nop_s )

er_t x_btcl_operator_f3_arg_nop_s_set_args( x_btcl_operator_f3_arg_nop_s* o, x_btcl_operator_f3_arg_arr_s* arg_arr )
{
    // bcore_x_btcl_operator_f3.x:89:5
    
    if( o->index < 0 ) BLM_TRY(GERR_fa("index has not been set." ))
    if( o->index >= arg_arr->size ) BLM_TRY(GERR_fa("index is out of range." ))
    
    o->name = arg_arr->data[ o->index ].name;
    o->val = (&(arg_arr->data[ o->index ].val));
    return  0;
}

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_btcl_operator_f3_const_nop_s )
"aware x_btcl_operator_f3",
"{",
    "f3_t val;",
    "func ^:get;",
    "func ^:check_consistency;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_btcl_operator_f3_const_nop_s )

x_btcl_operator_f3_const_nop_s* x_btcl_operator_f3_const_nop_s__( x_btcl_operator_f3_const_nop_s* o, f3_t val )
{
    // :2:1
    
    o->val = val;
    return  o;
}

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_btcl_operator_f3_uop_s )
"aware x_btcl_operator_f3",
"{",
    "tp_t type;",
    "aware x_btcl_operator_f3 => a;",
    "hidden x_btcl_operator_f3_spect_s* sa;",
    "hidden x_btcl_operator_f3_fp_f3_ar1 fp_get;",
    "hidden x_source_point_s sp;",
    "func ^:setup;",
    "func ^:set_args;",
    "func ^:get;",
    "func ^:check_consistency;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_btcl_operator_f3_uop_s )

er_t x_btcl_operator_f3_uop_s_setup( x_btcl_operator_f3_uop_s* o )
{
    // bcore_x_btcl_operator_f3.x:129:5
    
    o->sa = ( x_btcl_operator_f3_spect_s* )x_btcl_operator_f3_spect_s_get_aware( o->a );
    switch( o->type )
    {
        case ((tp_t)(TYPEOF_identity)): o->fp_get = f3_ident; break;
        case ((tp_t)(TYPEOF_neg)):      o->fp_get = f3_neg;   break;
        case ((tp_t)(TYPEOF_exp)):      o->fp_get = f3_exp;   break;
        case ((tp_t)(TYPEOF_log)):      o->fp_get = f3_log;   break;
        case ((tp_t)(TYPEOF_log2)):     o->fp_get = f3_log2;  break;
        case ((tp_t)(TYPEOF_log10)):    o->fp_get = f3_log10; break;
        case ((tp_t)(TYPEOF_sin)):      o->fp_get = f3_sin;   break;
        case ((tp_t)(TYPEOF_cos)):      o->fp_get = f3_cos;   break;
        case ((tp_t)(TYPEOF_tan)):      o->fp_get = f3_tan;   break;
        case ((tp_t)(TYPEOF_tanh)):     o->fp_get = f3_tanh;  break;
        case ((tp_t)(TYPEOF_sqrt)):     o->fp_get = f3_sqrt;  break;
        case ((tp_t)(TYPEOF_sign)):     o->fp_get = f3_sign;  break;
        case ((tp_t)(TYPEOF_abs)):      o->fp_get = f3_abs;   break;
        case ((tp_t)(TYPEOF_ceil)):     o->fp_get = f3_ceil;  break;
        case ((tp_t)(TYPEOF_floor)):    o->fp_get = f3_floor; break;
        default: return  x_source_point_s_parse_error_fa(&(o->sp),"Unhandled operator type: #<sc_t>.", bnameof( o->type ) ); break;
    }
    BLM_TRY(x_btcl_operator_f3_a_setup(o->a))
    return  0;
}

er_t x_btcl_operator_f3_uop_s_set_args( x_btcl_operator_f3_uop_s* o, x_btcl_operator_f3_arg_arr_s* arg_arr )
{
    // bcore_x_btcl_operator_f3.x:155:5
    
    BLM_TRY(x_btcl_operator_f3_a_set_args(o->a,arg_arr ))
    return  0;
}

er_t x_btcl_operator_f3_uop_s_check_consistency( const x_btcl_operator_f3_uop_s* o )
{
    // bcore_x_btcl_operator_f3.x:163:5
    
    BLM_TRY(EM_ASSERT(o->a ))
    BLM_TRY(EM_ASSERT(o->sa ))
    BLM_TRY(EM_ASSERT(o->fp_get != NULL ))
    BLM_TRY(x_btcl_operator_f3_a_check_consistency(o->a))
    return  0;
}

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_btcl_operator_f3_bop_s )
"aware x_btcl_operator_f3",
"{",
    "tp_t type;",
    "aware x_btcl_operator_f3 => a;",
    "aware x_btcl_operator_f3 => b;",
    "hidden x_btcl_operator_f3_spect_s* sa;",
    "hidden x_btcl_operator_f3_spect_s* sb;",
    "hidden x_btcl_operator_f3_fp_f3_ar2 fp_get;",
    "hidden x_source_point_s sp;",
    "func ^:setup;",
    "func ^:set_args;",
    "func ^:get;",
    "func ^:check_consistency;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_btcl_operator_f3_bop_s )

er_t x_btcl_operator_f3_bop_s_setup( x_btcl_operator_f3_bop_s* o )
{
    // bcore_x_btcl_operator_f3.x:191:5
    
    o->sa = ( x_btcl_operator_f3_spect_s* )x_btcl_operator_f3_spect_s_get_aware( o->a );
    o->sb = ( x_btcl_operator_f3_spect_s* )x_btcl_operator_f3_spect_s_get_aware( o->b );
    switch( o->type )
    {
        case ((tp_t)(TYPEOF_pow)):           o->fp_get = x_btcl_operator_f3_pow_get; break;
        case ((tp_t)(TYPEOF_div)):           o->fp_get = x_btcl_operator_f3_div_get; break;
        case ((tp_t)(TYPEOF_mul)):           o->fp_get = x_btcl_operator_f3_mul_get; break;
        case ((tp_t)(TYPEOF_sub)):           o->fp_get = x_btcl_operator_f3_sub_get; break;
        case ((tp_t)(TYPEOF_add)):           o->fp_get = x_btcl_operator_f3_add_get; break;
        case ((tp_t)(TYPEOF_equal)):         o->fp_get = x_btcl_operator_f3_equal_get; break;
        case ((tp_t)(TYPEOF_unequal)):       o->fp_get = x_btcl_operator_f3_unequal_get; break;
        case ((tp_t)(TYPEOF_larger_equal)):  o->fp_get = x_btcl_operator_f3_larger_equal_get; break;
        case ((tp_t)(TYPEOF_larger)):        o->fp_get = x_btcl_operator_f3_larger_get; break;
        case ((tp_t)(TYPEOF_smaller_equal)): o->fp_get = x_btcl_operator_f3_smaller_equal_get; break;
        case ((tp_t)(TYPEOF_smaller)):       o->fp_get = x_btcl_operator_f3_smaller_get; break;
        case ((tp_t)(TYPEOF_and)):           o->fp_get = x_btcl_operator_f3_and_get; break;
        case ((tp_t)(TYPEOF_or)):            o->fp_get = x_btcl_operator_f3_or_get; break;
        default: return  x_source_point_s_parse_error_fa(&(o->sp),"Unhandled operator type: #<sc_t>.", bnameof( o->type ) ); break;
    }
    BLM_TRY(x_btcl_operator_f3_a_setup(o->a))
    BLM_TRY(x_btcl_operator_f3_a_setup(o->b))
    return  0;
}

er_t x_btcl_operator_f3_bop_s_set_args( x_btcl_operator_f3_bop_s* o, x_btcl_operator_f3_arg_arr_s* arg_arr )
{
    // bcore_x_btcl_operator_f3.x:217:5
    
    BLM_TRY(x_btcl_operator_f3_a_set_args(o->a,arg_arr ))
    BLM_TRY(x_btcl_operator_f3_a_set_args(o->b,arg_arr ))
    return  0;
}

er_t x_btcl_operator_f3_bop_s_check_consistency( const x_btcl_operator_f3_bop_s* o )
{
    // bcore_x_btcl_operator_f3.x:226:5
    
    BLM_TRY(EM_ASSERT(o->a ))
    BLM_TRY(EM_ASSERT(o->b ))
    BLM_TRY(EM_ASSERT(o->sa ))
    BLM_TRY(EM_ASSERT(o->sb ))
    BLM_TRY(EM_ASSERT(o->fp_get != NULL ))
    BLM_TRY(x_btcl_operator_f3_a_check_consistency(o->a))
    BLM_TRY(x_btcl_operator_f3_a_check_consistency(o->b))
    return  0;
}

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_btcl_operator_f3_top_s )
"aware x_btcl_operator_f3",
"{",
    "tp_t type;",
    "aware x_btcl_operator_f3 => a;",
    "aware x_btcl_operator_f3 => b;",
    "aware x_btcl_operator_f3 => c;",
    "hidden x_btcl_operator_f3_spect_s* sa;",
    "hidden x_btcl_operator_f3_spect_s* sb;",
    "hidden x_btcl_operator_f3_spect_s* sc;",
    "hidden x_btcl_operator_f3_fp_f3_ar3 fp_get;",
    "hidden x_source_point_s sp;",
    "func ^:setup;",
    "func ^:set_args;",
    "func ^:get;",
    "func ^:check_consistency;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_btcl_operator_f3_top_s )

er_t x_btcl_operator_f3_top_s_setup( x_btcl_operator_f3_top_s* o )
{
    // bcore_x_btcl_operator_f3.x:260:5
    
    o->sa = ( x_btcl_operator_f3_spect_s* )x_btcl_operator_f3_spect_s_get_aware( o->a );
    o->sb = ( x_btcl_operator_f3_spect_s* )x_btcl_operator_f3_spect_s_get_aware( o->b );
    o->sc = ( x_btcl_operator_f3_spect_s* )x_btcl_operator_f3_spect_s_get_aware( o->c );
    switch( o->type )
    {
        case ((tp_t)(TYPEOF_conditional)): o->fp_get = x_btcl_operator_f3_conditional_get; break;
        default: return  x_source_point_s_parse_error_fa(&(o->sp),"Unhandled operator type: #<sc_t>.", bnameof( o->type ) ); break;
    }
    BLM_TRY(x_btcl_operator_f3_a_setup(o->a))
    BLM_TRY(x_btcl_operator_f3_a_setup(o->b))
    BLM_TRY(x_btcl_operator_f3_a_setup(o->c))
    return  0;
}

er_t x_btcl_operator_f3_top_s_set_args( x_btcl_operator_f3_top_s* o, x_btcl_operator_f3_arg_arr_s* arg_arr )
{
    // bcore_x_btcl_operator_f3.x:276:5
    
    BLM_TRY(x_btcl_operator_f3_a_set_args(o->a,arg_arr ))
    BLM_TRY(x_btcl_operator_f3_a_set_args(o->b,arg_arr ))
    BLM_TRY(x_btcl_operator_f3_a_set_args(o->c,arg_arr ))
    return  0;
}

er_t x_btcl_operator_f3_top_s_check_consistency( const x_btcl_operator_f3_top_s* o )
{
    // bcore_x_btcl_operator_f3.x:286:5
    
    BLM_TRY(EM_ASSERT(o->a ))
    BLM_TRY(EM_ASSERT(o->b ))
    BLM_TRY(EM_ASSERT(o->c ))
    BLM_TRY(EM_ASSERT(o->sa ))
    BLM_TRY(EM_ASSERT(o->sb ))
    BLM_TRY(EM_ASSERT(o->sc ))
    BLM_TRY(EM_ASSERT(o->fp_get != NULL ))
    BLM_TRY(x_btcl_operator_f3_a_check_consistency(o->a))
    BLM_TRY(x_btcl_operator_f3_a_check_consistency(o->b))
    BLM_TRY(x_btcl_operator_f3_a_check_consistency(o->c))
    return  0;
}

XOILA_DEFINE_SPECT_NASC_BEGIN( x_btcl, x_btcl_operator_f3 )
"{",
    "bcore_spect_header_s header;",
    "feature aware x_btcl_operator_f3 : setup = x_btcl_operator_f3_setup_default;",
    "feature aware x_btcl_operator_f3 : get = x_btcl_operator_f3_get_default;",
    "feature aware x_btcl_operator_f3 : set_args = x_btcl_operator_f3_set_args_default;",
    "feature aware x_btcl_operator_f3 : check_consistency = x_btcl_operator_f3_check_consistency_default;",
"}",
XOILA_DEFINE_SPECT_NASC_END( x_btcl, x_btcl_operator_f3 )

void x_btcl_operator_f3_group_signal_init1( void )
{
    // bcore_x_btcl_operator_f3.x:27:1
    
    
        BCORE_REGISTER_TYPE( function_pointer, x_btcl_operator_f3_fp_f3_ar0 );
        BCORE_REGISTER_TYPE( function_pointer, x_btcl_operator_f3_fp_f3_ar1 );
        BCORE_REGISTER_TYPE( function_pointer, x_btcl_operator_f3_fp_f3_ar2 );
        BCORE_REGISTER_TYPE( function_pointer, x_btcl_operator_f3_fp_f3_ar3 );
}

x_btcl_operator_f3* x_btcl_operator_f3_create_op( const sr_s* sr )
{
    // bcore_x_btcl_operator_f3.x:305:1
    
    if( sr_s_type(sr) == ((tp_t)(TYPEOF_x_btcl_functor_arg_uop_s)) )
    {BLM_INIT_LEVEL(1);
        const x_btcl_functor_arg_uop_s* src = ((const x_btcl_functor_arg_uop_s*)(sr->o));
        x_btcl_operator_f3_arg_nop_s* op = ((x_btcl_operator_f3_arg_nop_s*)BLM_LEVEL_T_PUSH(1,x_btcl_operator_f3_arg_nop_s,x_btcl_operator_f3_arg_nop_s_create()));
        op->index = src->index;
        BLM_RETURNV(x_btcl_operator_f3*,((x_btcl_operator_f3*)( ((x_btcl_operator_f3_arg_nop_s*)bcore_fork(op)))))
    BLM_DOWN();}
    
    if( sr_s_type(sr) == ((tp_t)(TYPEOF_x_btcl_operator_uop_s)) )
    {BLM_INIT_LEVEL(1);
        const x_btcl_operator_uop_s* src = ((const x_btcl_operator_uop_s*)(sr->o));
        x_btcl_operator_f3_uop_s* op = ((x_btcl_operator_f3_uop_s*)BLM_LEVEL_T_PUSH(1,x_btcl_operator_f3_uop_s,x_btcl_operator_f3_uop_s_create()));
        op->type = src->type;
        ASSERT( src->a.o );
        x_btcl_operator_f3_a_attach( &(op->a ), (x_btcl_operator_f3*)( x_btcl_operator_f3_create_op(&(src->a ))));
        x_source_point_s_copy(&(op->sp),&(src->sp ));
        BLM_RETURNV(x_btcl_operator_f3*,((x_btcl_operator_f3*)( ((x_btcl_operator_f3_uop_s*)bcore_fork(op)))))
    BLM_DOWN();}
    
    if( sr_s_type(sr) == ((tp_t)(TYPEOF_x_btcl_operator_bop_s)) )
    {BLM_INIT_LEVEL(1);
        const x_btcl_operator_bop_s* src = ((const x_btcl_operator_bop_s*)(sr->o));
        x_btcl_operator_f3_bop_s* op = ((x_btcl_operator_f3_bop_s*)BLM_LEVEL_T_PUSH(1,x_btcl_operator_f3_bop_s,x_btcl_operator_f3_bop_s_create()));
        op->type = src->type;
        ASSERT( src->a.o );
        ASSERT( src->b.o );
        x_btcl_operator_f3_a_attach( &(op->a ), (x_btcl_operator_f3*)( x_btcl_operator_f3_create_op(&(src->a ))));
        x_btcl_operator_f3_a_attach( &(op->b ), (x_btcl_operator_f3*)( x_btcl_operator_f3_create_op(&(src->b ))));
        x_source_point_s_copy(&(op->sp),&(src->sp ));
        BLM_RETURNV(x_btcl_operator_f3*,((x_btcl_operator_f3*)( ((x_btcl_operator_f3_bop_s*)bcore_fork(op)))))
    BLM_DOWN();}
    
    if( sr_s_type(sr) == ((tp_t)(TYPEOF_x_btcl_operator_top_s)) )
    {BLM_INIT_LEVEL(1);
        const x_btcl_operator_top_s* src = ((const x_btcl_operator_top_s*)(sr->o));
        x_btcl_operator_f3_top_s* op = ((x_btcl_operator_f3_top_s*)BLM_LEVEL_T_PUSH(1,x_btcl_operator_f3_top_s,x_btcl_operator_f3_top_s_create()));
        op->type = src->type;
        ASSERT( src->a.o );
        ASSERT( src->b.o );
        ASSERT( src->c.o );
        x_btcl_operator_f3_a_attach( &(op->a ), (x_btcl_operator_f3*)( x_btcl_operator_f3_create_op(&(src->a ))));
        x_btcl_operator_f3_a_attach( &(op->b ), (x_btcl_operator_f3*)( x_btcl_operator_f3_create_op(&(src->b ))));
        x_btcl_operator_f3_a_attach( &(op->c ), (x_btcl_operator_f3*)( x_btcl_operator_f3_create_op(&(src->c ))));
        x_source_point_s_copy(&(op->sp),&(src->sp ));
        BLM_RETURNV(x_btcl_operator_f3*,((x_btcl_operator_f3*)( ((x_btcl_operator_f3_top_s*)bcore_fork(op)))))
    BLM_DOWN();}
    
    if( sr_s_is_numeric(sr) )
    {BLM_INIT_LEVEL(1);
        x_btcl_operator_f3_const_nop_s* op = ((x_btcl_operator_f3_const_nop_s*)BLM_LEVEL_T_PUSH(1,x_btcl_operator_f3_const_nop_s,x_btcl_operator_f3_const_nop_s_create()));
        op->val = sr_s_to_f3(sr);
        BLM_RETURNV(x_btcl_operator_f3*,((x_btcl_operator_f3*)( ((x_btcl_operator_f3_const_nop_s*)bcore_fork(op)))))
    BLM_DOWN();}
    
    ERR_fa( "Cannot convert from type #<sc_t>", bnameof( sr_s_type(sr) ) );
    return  NULL;
}

//----------------------------------------------------------------------------------------------------------------------
// group: x_btcl_net; embeds: bcore_x_btcl_net.x

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_btcl_net_plain_branch_s )
"aware x_btcl_net",
"{",
    "tp_t name;",
    "sr_s sr;",
    "hidden x_source_point_s => sp;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_btcl_net_plain_branch_s )

x_btcl_net_plain_branch_s* x_btcl_net_plain_branch_s_setup( x_btcl_net_plain_branch_s* o, tp_t name, const x_source_point_s* sp, sr_s* sr )
{
    // bcore_x_btcl_net.x:33:5
    
    o->name = name;
    x_source_point_s_copy(BCORE_PASS_CREATE(x_source_point_s,o->sp),sp );
    sr_s_clear(&(o->sr));
    if( sr ) sr_s_tsm(&(o->sr),sr_s_type(sr), ((x_inst*)bcore_fork(sr->o)) );
    return o;
}

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_btcl_net_plain_branch_arr_s )
"aware x_array",
"{",
    "x_btcl_net_plain_branch_s [];",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_btcl_net_plain_branch_arr_s )

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_btcl_net_socket_branch_s )
"aware x_btcl_net",
"{",
    "tp_t name;",
    "sr_s sr;",
    "hidden x_source_point_s => sp;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_btcl_net_socket_branch_s )

x_btcl_net_socket_branch_s* x_btcl_net_socket_branch_s_setup( x_btcl_net_socket_branch_s* o, tp_t name, const x_source_point_s* sp, sr_s* sr )
{
    // bcore_x_btcl_net.x:57:5
    
    o->name = name;
    x_source_point_s_copy(BCORE_PASS_CREATE(x_source_point_s,o->sp),sp );
    sr_s_clear(&(o->sr));
    if( sr ) sr_s_tsm(&(o->sr),sr_s_type(sr), ((x_inst*)bcore_fork(sr->o)) );
    return o;
}

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_btcl_net_socket_branch_arr_s )
"aware x_array",
"{",
    "x_btcl_net_socket_branch_s [];",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_btcl_net_socket_branch_arr_s )

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_btcl_net_node_s )
"aware x_btcl_net",
"{",
    "tp_t type;",
    "tp_t name;",
    "tp_t sub_name;",
    "x_btcl_net_plain_branch_arr_s => plain_branch_arr;",
    "x_btcl_net_socket_branch_arr_s => socket_branch_arr;",
    "hidden x_source_point_s => sp;",
    "func x_btcl_operator:is_exportable_operand;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_btcl_net_node_s )

x_btcl_net_node_s* x_btcl_net_node_s_setup( x_btcl_net_node_s* o, tp_t type, tp_t name, const x_source_point_s* sp )
{
    // bcore_x_btcl_net.x:90:5
    
    o->type = type;
    o->name = name;
    x_source_point_s_copy(BCORE_PASS_CREATE(x_source_point_s,o->sp),sp );
    return  o;
}

x_btcl_net_node_s* x_btcl_net_node_s_setup_wire( x_btcl_net_node_s* o, tp_t rack_name, tp_t wire_name, const x_source_point_s* sp )
{
    // bcore_x_btcl_net.x:98:5
    
    x_btcl_net_node_s_setup(o,((tp_t)(TYPEOF_wire)), rack_name, sp );
    o->sub_name = wire_name;
    return  o;
}

bl_t x_btcl_net_node_s_plain_branch_exists( x_btcl_net_node_s* o, tp_t branch_name )
{
    // bcore_x_btcl_net.x:105:5
    
    if( !o->plain_branch_arr ) return  false;
    {const x_btcl_net_plain_branch_arr_s* __a=o->plain_branch_arr ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){x_btcl_net_plain_branch_s* e=&(__a->data[__i]); if( e->name == branch_name ) return  true;
    }}return  false;
}

bl_t x_btcl_net_node_s_socket_branch_exists( x_btcl_net_node_s* o, tp_t branch_name )
{
    // bcore_x_btcl_net.x:112:5
    
    if( !o->socket_branch_arr ) return  false;
    {const x_btcl_net_socket_branch_arr_s* __a=o->socket_branch_arr ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){x_btcl_net_socket_branch_s* e=&(__a->data[__i]); if( e->name == branch_name ) return  true;
    }}return  false;
}

x_btcl_net_node_s* x_btcl_net_node_s_push_plain_branch( x_btcl_net_node_s* o, tp_t name, bl_t replace, const x_source_point_s* sp, sr_s* sr )
{
    // bcore_x_btcl_net.x:132:5
    
    x_btcl_net_plain_branch_s* branch = NULL;
    if( replace ) branch = x_btcl_net_node_s_m_plain_branch_by_name(o,name );
    if( !branch ) branch = ((x_btcl_net_plain_branch_s*)(x_array_push(((x_array*)(BCORE_PASS_CREATE(x_btcl_net_plain_branch_arr_s,o->plain_branch_arr))))));
    x_btcl_net_plain_branch_s_setup(branch,name, sp, sr );
    return  o;
}

x_btcl_net_node_s* x_btcl_net_node_s_push_socket_branch( x_btcl_net_node_s* o, tp_t name, bl_t replace, const x_source_point_s* sp, sr_s* sr )
{
    // bcore_x_btcl_net.x:142:5
    
    x_btcl_net_socket_branch_s* branch = NULL;
    if( replace ) branch = x_btcl_net_node_s_m_socket_branch_by_name(o,name );
    if( !branch ) branch = ((x_btcl_net_socket_branch_s*)(x_array_push(((x_array*)(BCORE_PASS_CREATE(x_btcl_net_socket_branch_arr_s,o->socket_branch_arr))))));
    x_btcl_net_socket_branch_s_setup(branch,name, sp, sr );
    return  o;
}

XOILA_DEFINE_SPECT_NASC_BEGIN( x_btcl, x_btcl_net )
"{",
    "bcore_spect_header_s header;",
"}",
XOILA_DEFINE_SPECT_NASC_END( x_btcl, x_btcl_net )

er_t x_btcl_net_eval_node_modifier( x_btcl_frame_s* frame, x_source* source, sr_s* node_sr )
{
    // bcore_x_btcl_net.x:154:1
    BLM_INIT_LEVEL(0);
    (*(node_sr)) = sr_clone( (*(node_sr)) );
    x_btcl_net_node_s* node = ((x_btcl_net_node_s*)BLM_LEVEL_T_PUSH(0,x_btcl_net_node_s,((x_btcl_net_node_s*)bcore_fork(((x_btcl_net_node_s*)(node_sr->o))))));
    
    bl_t do_loop = true;
    while( do_loop )
    {BLM_INIT_LEVEL(1);
        sr_s* branch_sr = ((sr_s*)BLM_LEVEL_T_PUSH(1,sr_s,sr_s_create()));
        const x_source_point_s* sp = x_source_point_s_setup_from_source(((x_source_point_s*)BLM_LEVEL_T_PUSH(1,x_source_point_s,x_source_point_s_create())),source );
    
        if( x_source_parse_bl(source," #?'.'") )
        {
            if( !x_btcl_is_identifier(source ) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Identifier expected.\n" ))
            tp_t branch_name = bcore_name_enroll(x_btcl_frame_s_nameof(frame,x_btcl_frame_s_get_identifier(frame,source, true ) ) );
            BLM_TRY(x_source_parse_fa(source," =" ))
            BLM_TRY(x_btcl_frame_s_eval(frame,0, source, branch_sr ))
    
            // convert functions to functors
            if( sr_s_type(branch_sr) == ((tp_t)(TYPEOF_x_btcl_function_s)) )
            {BLM_INIT_LEVEL(3);
                x_btcl_functor_s* functor = ((x_btcl_functor_s*)BLM_LEVEL_T_PUSH(3,x_btcl_functor_s,x_btcl_functor_s_create()));
                BLM_TRY(x_btcl_functor_s_setup(functor,((x_source_point_s*)BLM_LEVEL_T_PUSH(3,x_source_point_s,x_source_point_s__(x_source_point_s_create(),source ))), ((x_btcl_function_s*)(branch_sr->o)), frame ))
                sr_s_asm(branch_sr,((x_btcl_functor_s*)bcore_fork(functor)) );
            BLM_DOWN();}
    
            x_btcl_net_node_s_push_plain_branch(node,branch_name, true, sp, branch_sr );
        }
        else if( x_source_parse_bl(source," #?':'") )
        {
            if( !x_btcl_is_identifier(source ) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Identifier expected.\n" ))
            tp_t branch_name = bcore_name_enroll(x_btcl_frame_s_nameof(frame,x_btcl_frame_s_get_identifier(frame,source, true ) ) );
            if( x_source_parse_bl(source," #?'='") )
            {
                BLM_TRY(x_btcl_frame_s_eval(frame,0, source, branch_sr ))
                x_btcl_net_node_s_push_socket_branch(node,branch_name, true, sp, branch_sr );
            }
            else
            {BLM_INIT_LEVEL(4);
                /// sockets without explicit assignment are initialized with constant '0'
                x_btcl_net_node_s_push_socket_branch(node,branch_name, true, sp, sr_s_from_f3(((sr_s*)BLM_LEVEL_T_PUSH(4,sr_s,sr_s_create())),0 ) );
            BLM_DOWN();}
        }
        else
        {
            BLM_TRY(x_btcl_frame_s_eval(frame,0, source, branch_sr ))
    
            // convert functions to functors
            if( sr_s_type(branch_sr) == ((tp_t)(TYPEOF_x_btcl_function_s)) )
            {BLM_INIT_LEVEL(3);
                x_btcl_functor_s* functor = ((x_btcl_functor_s*)BLM_LEVEL_T_PUSH(3,x_btcl_functor_s,x_btcl_functor_s_create()));
                BLM_TRY(x_btcl_functor_s_setup(functor,((x_source_point_s*)BLM_LEVEL_T_PUSH(3,x_source_point_s,x_source_point_s__(x_source_point_s_create(),source ))), ((x_btcl_function_s*)(branch_sr->o)), frame ))
                sr_s_asm(branch_sr,((x_btcl_functor_s*)bcore_fork(functor)) );
            BLM_DOWN();}
    
            x_btcl_net_node_s_push_plain_branch(node,0, false, sp, branch_sr );
        }
    
        do_loop = false;
    
        if( x_source_parse_bl(source," #?','"  ) ) do_loop = true;
        if( x_source_parse_bl(source," #=?')'" ) ) do_loop = false;
    BLM_DOWN();}
    
    x_stamp_t_mutated(((x_stamp*)(node_sr->o)),sr_s_type(node_sr) );
    BLM_RETURNV(er_t, 0)
}

er_t x_btcl_net_eval_node_member( x_btcl_frame_s* frame, x_source* source, sr_s* sr )
{
    // bcore_x_btcl_net.x:224:1
    BLM_INIT_LEVEL(0);
    x_btcl_net_node_s* node = ((x_btcl_net_node_s*)BLM_LEVEL_T_PUSH(0,x_btcl_net_node_s,((x_btcl_net_node_s*)bcore_fork(((x_btcl_net_node_s*)(sr->o))))));
    
    /// Identifier
    if( !x_btcl_is_identifier(source ) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Member name '<literal>' expected." ))
    
    tp_t name = x_btcl_frame_s_get_identifier(frame,source, true );
    x_btcl_net_plain_branch_s* branch = x_btcl_net_node_s_m_plain_branch_by_name(node,name );
    if( !branch ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Branch name '#<sc_t>' not found.", x_btcl_frame_s_nameof(frame,name ) ))
    
    if( sr_s_is_strong(sr) ) x_btcl_frame_s_preserve_and_set_weak(frame,sr );
    
    sr_s_tsm(sr,sr_s_type(&(branch->sr)), ((x_inst*)bcore_fork(branch->sr.o)) );
    sr_s_set_const(sr,true );
    
    BLM_RETURNV(er_t, 0)
}

//----------------------------------------------------------------------------------------------------------------------
// group: x_btcl_functor; embeds: bcore_x_btcl_functor.x

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_btcl_functor_arg_val_s )
"aware x_btcl_functor",
"{",
    "sr_s sr;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_btcl_functor_arg_val_s )

f3_t* x_btcl_functor_arg_val_s_get_f3_ptr( x_btcl_functor_arg_val_s* o, f3_t v_init )
{
    // bcore_x_btcl_functor.x:33:5
    
    if( sr_s_type(&(o->sr)) != ((tp_t)(TYPEOF_f3_t)) ) sr_s_from_f3(&(o->sr),v_init );
    return  ((f3_t*)(o->sr.o));
}

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_btcl_functor_arg_uop_s )
"aware x_btcl_functor",
"{",
    "sz_t index;",
    "x_btcl_functor_arg_val_s -> val;",
    "func x_btcl_operator:is_operator;",
    "func x_btcl_operator:is_exportable_operand;",
    "func x_btcl_operator:solve;",
    "func x_btcl_operator:execute;",
    "func x_btcl_operator:signal;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_btcl_functor_arg_uop_s )

er_t x_btcl_functor_arg_uop_s_solve( x_btcl_functor_arg_uop_s* o, x_btcl_frame_s* frame, sr_s* result, bl_t* success )
{
    // bcore_x_btcl_functor.x:53:5
    
    success = false;
    return  0;
}

er_t x_btcl_functor_arg_uop_s_execute( const x_btcl_functor_arg_uop_s* o, sr_s* result )
{
    // bcore_x_btcl_functor.x:59:5
    
    if( o->val )
    {
        sr_s_tsc(result,sr_s_type(&(o->val->sr)), ((x_inst*)bcore_fork(o->val->sr.o)) );
    }
    else
    {
        sr_s_clear(result);
    }
    return  0;
}

x_btcl_functor_arg_uop_s* x_btcl_functor_arg_uop_s_signal( x_btcl_functor_arg_uop_s* o, tp_t name, x_inst* arg )
{
    // bcore_x_btcl_functor.x:72:5
    
    if( name == ((tp_t)(TYPEOF_signal_arg_uop_update_val)) )
    {
        ASSERT( arg->_ == ((tp_t)(TYPEOF_x_btcl_functor_arg_arr_s)) );
        x_btcl_functor_arg_arr_s* arg_arr = ((x_btcl_functor_arg_arr_s*)(arg));
        if( o->index < 0 || o->index >= arg_arr->size ) ERR_fa( "index (#<sz_t>) is out of range [0,#<sz_t>]", o->index, arg_arr->size - 1 );
        x_btcl_functor_arg_val_s_attach( &(o->val ),  ((x_btcl_functor_arg_val_s*)bcore_fork(arg_arr->data[ o->index ].val)));
    }
    return o;
}

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_btcl_functor_arg_s )
"aware x_btcl_functor",
"{",
    "tp_t name;",
    "x_btcl_functor_arg_val_s => val;",
    "x_btcl_functor_arg_uop_s => uop;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_btcl_functor_arg_s )

x_btcl_functor_arg_uop_s* x_btcl_functor_arg_s_get_uop( x_btcl_functor_arg_s* o )
{
    // bcore_x_btcl_functor.x:101:5
    
    x_btcl_functor_arg_val_s_attach( &(BCORE_PASS_CREATE(x_btcl_functor_arg_uop_s,o->uop)->val ),  ((x_btcl_functor_arg_val_s*)bcore_fork(BCORE_PASS_CREATE(x_btcl_functor_arg_val_s,o->val))));
    return  o->uop;
}

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_btcl_functor_arg_arr_s )
"aware x_array",
"{",
    "x_btcl_functor_arg_s [];",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_btcl_functor_arg_arr_s )

x_btcl_functor_arg_arr_s* x_btcl_functor_arg_arr_s_set_size( x_btcl_functor_arg_arr_s* o, sz_t size )
{
    // bcore_x_btcl_functor.x:114:5
    
    x_array_set_size(((x_array*)(o)),size );
    for(sz_t i = 0; i < size; i++ )
    {
        x_btcl_functor_arg_s_get_uop(&(o->data[ i ]))->index = i;
        x_btcl_functor_arg_s_set_f3(&(o->data[ i ]),0 );
    }
    return o;
}

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_btcl_functor_s )
"aware x_btcl_functor",
"{",
    "x_btcl_functor_arg_arr_s arg_arr;",
    "sr_s => op_tree;",
    "hidden x_source_point_s sp;",
    "func bcore_inst_call:copy_x;",
    "func bcore_via_call:mutated;",
    "func x_btcl:btcl_function_arity;",
    "func x_btcl:btcl_function;",
    "func x_btcl:nullary_f3;",
    "func x_btcl:unary_f3;",
    "func x_btcl:binary_f3;",
    "func x_btcl:ternary_f3;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_btcl_functor_s )

x_btcl_functor_s* x_btcl_functor_s_signal( x_btcl_functor_s* o, tp_t name, x_inst* arg )
{
    // bcore_x_btcl_functor.x:140:5
    
    if( x_btcl_functor_s_is_operator(o) ) x_btcl_operator_a_signal(x_btcl_functor_s_operator(o),name, arg );
    return o;
}

void x_btcl_functor_s_copy_x( x_btcl_functor_s* o, const x_btcl_functor_s* src )
{
    // bcore_x_btcl_functor.x:145:5
    
    x_btcl_functor_s_signal(o,((tp_t)(TYPEOF_signal_arg_uop_update_val)),((x_inst*)(&( o->arg_arr ))));
}

void x_btcl_functor_s_mutated( x_btcl_functor_s* o )
{
    // bcore_x_btcl_functor.x:150:5
    
    x_btcl_functor_s_signal(o,((tp_t)(TYPEOF_signal_arg_uop_update_val)),((x_inst*)(&( o->arg_arr ))));
}

er_t x_btcl_functor_s_setup( x_btcl_functor_s* o, const x_source_point_s* sp, x_btcl_function_s* function, x_btcl_frame_s* lexical_frame )
{
    // bcore_x_btcl_functor.x:155:5
    BLM_INIT_LEVEL(0);
    x_source_point_s_copy(&(o->sp),sp );
    sz_t args = function->signature->arg_list.size;
    x_btcl_functor_arg_arr_s_set_size(&(o->arg_arr),args );
    bcore_arr_sr_s* arr_sr = bcore_arr_sr_s_set_size(((bcore_arr_sr_s*)BLM_LEVEL_T_PUSH(0,bcore_arr_sr_s,bcore_arr_sr_s_create())),args );
    for(sz_t i = 0; i < args; i++ )
    {
        sr_s_asc(&(arr_sr->data[ i ]),((x_btcl_functor_arg_uop_s*)bcore_fork(x_btcl_functor_arg_s_get_uop(&(o->arg_arr.data[ i ])))) );
        o->arg_arr.data[ i ].name = x_btcl_signature_s_arg_name(function->signature,i );
    }
    BLM_TRY(x_btcl_function_s_call(function,&(o->sp), lexical_frame, arr_sr, BCORE_PASS_CREATE(sr_s,o->op_tree) ))
    BLM_RETURNV(er_t, 0)
}

tp_t x_btcl_functor_s_arg_name( const x_btcl_functor_s* o, sz_t index )
{
    // bcore_x_btcl_functor.x:172:5
    
    if( index < 0 || index >= x_btcl_functor_s_args(o) ) ERR_fa( "index (#<sz_t>) is out of range [0,#<sz_t>]", index, x_btcl_functor_s_args(o) - 1 );
    return  o->arg_arr.data[ index ].name;
}

sz_t x_btcl_functor_s_arg_index( const x_btcl_functor_s* o, tp_t name )
{
    // bcore_x_btcl_functor.x:179:5
    
    for(sz_t i = 0; i < o->arg_arr.size; i++ ) if( x_btcl_functor_s_arg_name(o,i ) == name ) return  i;
    return  -1;
}

const x_btcl_functor_s* x_btcl_functor_s_set_arg_sr( const x_btcl_functor_s* o, sz_t index, sr_s* sr )
{
    // bcore_x_btcl_functor.x:187:5
    
    if( index < 0 || index >= x_btcl_functor_s_args(o) ) ERR_fa( "index (#<sz_t>) is out of range [0,#<sz_t>]", index, x_btcl_functor_s_args(o) - 1 );
    x_btcl_functor_arg_s_set_val(&(o->arg_arr.data[ index ]),sr );
    return o;
}

const x_btcl_functor_s* x_btcl_functor_s_set_arg_sr_by_name( const x_btcl_functor_s* o, tp_t name, sr_s* sr )
{
    // bcore_x_btcl_functor.x:194:5
    
    sz_t index = x_btcl_functor_s_arg_index(o,name );
    if( index >= 0 ) x_btcl_functor_s_set_arg_sr(o,index, sr );
    return o;
}

f3_t* x_btcl_functor_s_get_arg_f3_ptr( const x_btcl_functor_s* o, sz_t index, f3_t v_init )
{
    // bcore_x_btcl_functor.x:202:5
    
    if( index < 0 || index >= x_btcl_functor_s_args(o) ) ERR_fa( "index (#<sz_t>) is out of range [0,#<sz_t>]", index, x_btcl_functor_s_args(o) - 1 );
    return  x_btcl_functor_arg_s_get_f3_ptr(&(o->arg_arr.data[ index ]),v_init );
}

const x_btcl_functor_s* x_btcl_functor_s_set_arg_f3_by_name( const x_btcl_functor_s* o, tp_t name, f3_t v )
{
    // bcore_x_btcl_functor.x:209:5
    
    sz_t index = x_btcl_functor_s_arg_index(o,name );
    if( index >= 0 ) x_btcl_functor_s_set_arg_f3(o,index, v );
    return o;
}

f3_t* x_btcl_functor_s_get_arg_f3_ptr_by_name( const x_btcl_functor_s* o, tp_t name, f3_t v_init )
{
    // bcore_x_btcl_functor.x:216:5
    
    sz_t index = x_btcl_functor_s_arg_index(o,name );
    if( index >= 0 ) return  x_btcl_functor_s_get_arg_f3_ptr(o,index, v_init );
    return  NULL;
}

er_t x_btcl_functor_s_call( const x_btcl_functor_s* o, sr_s* result )
{
    // bcore_x_btcl_functor.x:224:5
    
    if( !o->op_tree ) return  EM_ERR_fa("Functor has not been set up." );
    if( x_btcl_operator_sr_is_operator(o->op_tree ) )
    {
        BLM_TRY(x_btcl_operator_a_execute(((const x_btcl_operator*)(o->op_tree->o)),result ))
    }
    else
    {
        sr_s_tsc(result,sr_s_type(o->op_tree), ((x_inst*)bcore_fork(o->op_tree->o)) );
    }
    return  0;
}

f3_t x_btcl_functor_s_call_to_f3( const x_btcl_functor_s* o )
{
    // bcore_x_btcl_functor.x:238:5
    BLM_INIT_LEVEL(0);
    sr_s* result = ((sr_s*)BLM_LEVEL_T_PUSH(0,sr_s,sr_s_create()));
    BLM_TRY_EXIT(x_btcl_functor_s_call(o,result ))
    BLM_RETURNV(f3_t, sr_s_to_f3(result))
}

sz_t x_btcl_functor_s_btcl_function_arity( const x_btcl_functor_s* o, tp_t name )
{
    // bcore_x_btcl_functor.x:245:5
    
    switch( name )
    {
        case ((tp_t)(TYPEOF_call)): return  x_btcl_functor_s_args(o); // add_a accepts one argument
        default:    return  -1;
    }
    return  -1;
}

er_t x_btcl_functor_s_btcl_function( const x_btcl_functor_s* o, tp_t name, const x_source_point_s* sp, x_btcl_frame_s* lexical_frame, const bcore_arr_sr_s* args, sr_s* result )
{
    // bcore_x_btcl_functor.x:255:5
    
    switch( name )
    {
        case ((tp_t)(TYPEOF_call)):
        {
            for(sz_t i = 0; i < x_btcl_functor_s_args(o); i++ ) x_btcl_functor_s_set_arg_sr(o,i,&( args->data[i] ));
            BLM_TRY(x_btcl_functor_s_call(o,result ))
        }
        break;
    
        default: break; // never reached
    }
    return  0;
}

f3_t x_btcl_functor_s_nullary_f3( const x_btcl_functor_s* o )
{
    // bcore_x_btcl_functor.x:271:5
    
    ASSERT( x_btcl_functor_s_args(o) == 0 );
    return  x_btcl_functor_s_call_to_f3(o);
}

f3_t x_btcl_functor_s_unary_f3( const x_btcl_functor_s* o, f3_t x )
{
    // bcore_x_btcl_functor.x:277:5
    
    ASSERT( x_btcl_functor_s_args(o) == 1 );
    x_btcl_functor_s_set_arg_f3(o,0, x );
    return  x_btcl_functor_s_call_to_f3(o);
}

f3_t x_btcl_functor_s_binary_f3( const x_btcl_functor_s* o, f3_t x, f3_t y )
{
    // bcore_x_btcl_functor.x:284:5
    
    ASSERT( x_btcl_functor_s_args(o) == 2 );
    x_btcl_functor_s_set_arg_f3(o,0, x );
    x_btcl_functor_s_set_arg_f3(o,1, y );
    return  x_btcl_functor_s_call_to_f3(o);
}

f3_t x_btcl_functor_s_ternary_f3( const x_btcl_functor_s* o, f3_t x, f3_t y, f3_t z )
{
    // bcore_x_btcl_functor.x:292:5
    
    ASSERT( x_btcl_functor_s_args(o) == 3 );
    x_btcl_functor_s_set_arg_f3(o,0, x );
    x_btcl_functor_s_set_arg_f3(o,1, y );
    x_btcl_functor_s_set_arg_f3(o,2, z );
    return  x_btcl_functor_s_call_to_f3(o);
}

XOILA_DEFINE_SPECT_NASC_BEGIN( x_btcl, x_btcl_functor )
"{",
    "bcore_spect_header_s header;",
"}",
XOILA_DEFINE_SPECT_NASC_END( x_btcl, x_btcl_functor )

er_t x_btcl_functor_eval_modifier( x_btcl_frame_s* frame, x_source* source, sr_s* functor_sr )
{
    // bcore_x_btcl_functor.x:304:1
    BLM_INIT_LEVEL(0);
    (*(functor_sr)) = sr_clone( (*(functor_sr)) );
    x_btcl_functor_s* functor = ((x_btcl_functor_s*)BLM_LEVEL_T_PUSH(0,x_btcl_functor_s,((x_btcl_functor_s*)bcore_fork(((x_btcl_functor_s*)(functor_sr->o))))));
    
    const x_source_point_s* sp = x_source_point_s_setup_from_source(((x_source_point_s*)BLM_LEVEL_T_PUSH(0,x_source_point_s,x_source_point_s_create())),source );
    sr_s* sb = ((sr_s*)BLM_LEVEL_T_PUSH(0,sr_s,sr_s_create()));
    BLM_TRY(x_btcl_frame_s_eval(frame,0, source, sb ))
    
    if( sr_s_type(sb) != ((tp_t)(TYPEOF_x_btcl_function_s)) )
    {
        BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Cannot convert '#<sc_t>' to a functor. A function was expected.\n", bnameof( sr_s_type(sb) ) ))
    }
    
    x_btcl_function_s* function = ((x_btcl_function_s*)(sb->o));
    
    BLM_TRY(x_btcl_functor_s_setup(functor,sp, function, frame ))
    BLM_RETURNV(er_t, 0)
}

//----------------------------------------------------------------------------------------------------------------------
// group: x_btcl_functor_f3; embeds: bcore_x_btcl_functor_f3.x

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_btcl_functor_f3_s )
"aware x_btcl_functor_f3",
"{",
    "x_btcl_operator_f3_arg_arr_s arg_arr;",
    "aware x_btcl_operator_f3 => op_tree;",
    "hidden x_source_point_s sp;",
    "func bcore_inst_call:copy_x;",
    "func bcore_fp:copy_typed;",
    "func bcore_via_call:mutated;",
    "func x_btcl:btcl_function_arity;",
    "func x_btcl:btcl_function;",
    "func x_btcl:nullary_f3;",
    "func x_btcl:unary_f3;",
    "func x_btcl:binary_f3;",
    "func x_btcl:ternary_f3;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_btcl_functor_f3_s )

void x_btcl_functor_f3_s_copy_x( x_btcl_functor_f3_s* o, const x_btcl_functor_f3_s* src )
{
    // bcore_x_btcl_functor_f3.x:39:5
    
    BLM_TRY_EXIT(x_btcl_functor_f3_s_setup_tree(o))
}

er_t x_btcl_functor_f3_s_copy_typed( x_btcl_functor_f3_s* o, tp_t type, vc_t src )
{
    // bcore_x_btcl_functor_f3.x:44:5
    
    switch( type )
    {
        case ((tp_t)(TYPEOF_x_btcl_functor_f3_s)): x_btcl_functor_f3_s_copy(o,((const x_btcl_functor_f3_s*)(src)) ); break;
        case ((tp_t)(TYPEOF_x_btcl_functor_s)): BLM_TRY(x_btcl_functor_f3_s_from_functor(o,((const x_btcl_functor_s*)(src)) )) break;
        default:
        {
            if( bcore_tp_is_numeric( type ) )
            {BLM_INIT_LEVEL(4);
                x_btcl_operator_f3_const_nop_s* nop = ((x_btcl_operator_f3_const_nop_s*)BLM_LEVEL_T_PUSH(4,x_btcl_operator_f3_const_nop_s,x_btcl_operator_f3_const_nop_s_create()));
                nop->val = sr_s_to_f3(sr_s_twc( ((sr_s*)BLM_LEVEL_T_PUSH(4,sr_s,sr_s_create())),type, src ));
                x_btcl_operator_f3_a_attach( &(o->op_tree ), (x_btcl_operator_f3*)( ((x_btcl_operator_f3_const_nop_s*)bcore_fork(nop))));
            BLM_DOWN();}
            else
            {
                BLM_TRY(GERR_fa("Cannot convert from #<sc_t>.", bnameof( type ) ))
            }
        }
        break;
    }
    return  0;
}

void x_btcl_functor_f3_s_mutated( x_btcl_functor_f3_s* o )
{
    // bcore_x_btcl_functor_f3.x:68:5
    
    BLM_TRY_EXIT(x_btcl_functor_f3_s_setup_tree(o))
}

er_t x_btcl_functor_f3_s_setup_tree( x_btcl_functor_f3_s* o )
{
    // bcore_x_btcl_functor_f3.x:73:5
    
    if( o->op_tree )
    {
        BLM_TRY(x_btcl_operator_f3_a_setup(o->op_tree))
        BLM_TRY(x_btcl_operator_f3_a_set_args(o->op_tree,&(o->arg_arr )))
        BLM_TRY(x_btcl_operator_f3_a_check_consistency(o->op_tree))
    }
    return  0;
}

er_t x_btcl_functor_f3_s_from_functor( x_btcl_functor_f3_s* o, const x_btcl_functor_s* functor )
{
    // bcore_x_btcl_functor_f3.x:84:5
    
    sz_t args = x_btcl_functor_s_args(functor);
    x_array_set_size(((x_array*)(&(o->arg_arr))),args );
    for(sz_t i = 0; i < args; i++ ) o->arg_arr.data[ i ].name = functor->arg_arr.data[ i ].name;
    x_source_point_s_copy(&(o->sp),&(functor->sp ));
    x_btcl_operator_f3_a_attach( &(o->op_tree ), (x_btcl_operator_f3*)( x_btcl_operator_f3_create_op(functor->op_tree )));
    BLM_TRY(x_btcl_functor_f3_s_setup_tree(o))
    return  0;
}

er_t x_btcl_functor_f3_s_setup( x_btcl_functor_f3_s* o, const x_source_point_s* sp, x_btcl_function_s* function, x_btcl_frame_s* lexical_frame )
{
    // bcore_x_btcl_functor_f3.x:95:5
    BLM_INIT_LEVEL(0);
    x_btcl_functor_s* functor = ((x_btcl_functor_s*)BLM_LEVEL_T_PUSH(0,x_btcl_functor_s,x_btcl_functor_s_create()));
    BLM_TRY(x_btcl_functor_s_setup(functor,sp, function, lexical_frame ))
    BLM_TRY(x_btcl_functor_f3_s_from_functor(o,functor ))
    BLM_RETURNV(er_t, 0)
}

tp_t x_btcl_functor_f3_s_arg_name( const x_btcl_functor_f3_s* o, sz_t index )
{
    // bcore_x_btcl_functor_f3.x:105:5
    
    if( index < 0 || index >= x_btcl_functor_f3_s_args(o) ) ERR_fa( "index (#<sz_t>) is out of range [0,#<sz_t>]", index, x_btcl_functor_f3_s_args(o) - 1 );
    return  o->arg_arr.data[ index ].name;
}

sz_t x_btcl_functor_f3_s_arg_index( const x_btcl_functor_f3_s* o, tp_t name )
{
    // bcore_x_btcl_functor_f3.x:112:5
    
    for(sz_t i = 0; i < o->arg_arr.size; i++ ) if( x_btcl_functor_f3_s_arg_name(o,i ) == name ) return  i;
    return  -1;
}

const x_btcl_functor_f3_s* x_btcl_functor_f3_s_set_arg_f3( const x_btcl_functor_f3_s* o, sz_t index, f3_t v )
{
    // bcore_x_btcl_functor_f3.x:120:5
    
    if( !o->op_tree ) ERR_fa( "o.op_tree == NULL. Functor has not been setup." );
    if( index < 0 || index >= x_btcl_functor_f3_s_args(o) ) ERR_fa( "index (#<sz_t>) is out of range [0,#<sz_t>]", index, x_btcl_functor_f3_s_args(o) - 1 );
    o->arg_arr.data[ index ].val = v;
    return o;
}

f3_t* x_btcl_functor_f3_s_get_arg_f3_ptr( const x_btcl_functor_f3_s* o, sz_t index )
{
    // bcore_x_btcl_functor_f3.x:127:5
    
    if( !o->op_tree ) ERR_fa( "o.op_tree == NULL. Functor has not been setup." );
    if( index < 0 || index >= x_btcl_functor_f3_s_args(o) ) ERR_fa( "index (#<sz_t>) is out of range [0,#<sz_t>]", index, x_btcl_functor_f3_s_args(o) - 1 );
    return  (&(o->arg_arr.data[ index ].val));
}

const x_btcl_functor_f3_s* x_btcl_functor_f3_s_set_arg_f3_by_name( const x_btcl_functor_f3_s* o, tp_t name, f3_t v )
{
    // bcore_x_btcl_functor_f3.x:135:5
    
    sz_t index = x_btcl_functor_f3_s_arg_index(o,name );
    if( index >= 0 ) x_btcl_functor_f3_s_set_arg_f3(o,index, v );
    return o;
}

f3_t* x_btcl_functor_f3_s_get_arg_f3_ptr_by_name( const x_btcl_functor_f3_s* o, tp_t name )
{
    // bcore_x_btcl_functor_f3.x:142:5
    
    sz_t index = x_btcl_functor_f3_s_arg_index(o,name );
    if( index >= 0 ) return  x_btcl_functor_f3_s_get_arg_f3_ptr(o,index );
    return  NULL;
}

sz_t x_btcl_functor_f3_s_btcl_function_arity( const x_btcl_functor_f3_s* o, tp_t name )
{
    // bcore_x_btcl_functor_f3.x:152:5
    
    switch( name )
    {
        case ((tp_t)(TYPEOF_call)): return  x_btcl_functor_f3_s_args(o);
        default:    return  -1;
    }
    return  -1;
}

er_t x_btcl_functor_f3_s_btcl_function( const x_btcl_functor_f3_s* o, tp_t name, const x_source_point_s* sp, x_btcl_frame_s* lexical_frame, const bcore_arr_sr_s* args, sr_s* result )
{
    // bcore_x_btcl_functor_f3.x:162:5
    
    switch( name )
    {
        case ((tp_t)(TYPEOF_call)):
        {
            for(sz_t i = 0; i < x_btcl_functor_f3_s_args(o); i++ ) x_btcl_functor_f3_s_set_arg_f3(o,i, sr_s_to_f3(&(args->data[i])) );
            sr_s_from_f3(result,x_btcl_functor_f3_s_call(o) );
        }
        break;
    
        default: break; // never reached
    }
    return  0;
}

f3_t x_btcl_functor_f3_s_nullary_f3( const x_btcl_functor_f3_s* o )
{
    // bcore_x_btcl_functor_f3.x:178:5
    
    ASSERT( x_btcl_functor_f3_s_args(o) == 0 );
    return  x_btcl_functor_f3_s_call(o);
}

f3_t x_btcl_functor_f3_s_unary_f3( const x_btcl_functor_f3_s* o, f3_t x )
{
    // bcore_x_btcl_functor_f3.x:184:5
    
    ASSERT( x_btcl_functor_f3_s_args(o) == 1 );
    x_btcl_functor_f3_s_set_arg_f3(o,0, x );
    return  x_btcl_functor_f3_s_call(o);
}

f3_t x_btcl_functor_f3_s_binary_f3( const x_btcl_functor_f3_s* o, f3_t x, f3_t y )
{
    // bcore_x_btcl_functor_f3.x:191:5
    
    ASSERT( x_btcl_functor_f3_s_args(o) == 2 );
    x_btcl_functor_f3_s_set_arg_f3(o,0, x );
    x_btcl_functor_f3_s_set_arg_f3(o,1, y );
    return  x_btcl_functor_f3_s_call(o);
}

f3_t x_btcl_functor_f3_s_ternary_f3( const x_btcl_functor_f3_s* o, f3_t x, f3_t y, f3_t z )
{
    // bcore_x_btcl_functor_f3.x:199:5
    
    ASSERT( x_btcl_functor_f3_s_args(o) == 3 );
    x_btcl_functor_f3_s_set_arg_f3(o,0, x );
    x_btcl_functor_f3_s_set_arg_f3(o,1, y );
    x_btcl_functor_f3_s_set_arg_f3(o,2, z );
    return  x_btcl_functor_f3_s_call(o);
}

XOILA_DEFINE_SPECT_NASC_BEGIN( x_btcl, x_btcl_functor_f3 )
"{",
    "bcore_spect_header_s header;",
"}",
XOILA_DEFINE_SPECT_NASC_END( x_btcl, x_btcl_functor_f3 )

//----------------------------------------------------------------------------------------------------------------------
// group: x_btcl_random; embeds: bcore_x_btcl_random.x

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_btcl_random_s )
"aware x_btcl_random",
"{",
    "aware bcore_prsg => prsg;",
    "u3_t seed = 1234;",
    "f3_t min = 0.0;",
    "f3_t max = 1.0;",
    "func x_btcl:btcl_function_arity;",
    "func x_btcl:btcl_function;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_btcl_random_s )

const x_btcl_random_s* x_btcl_random_s_fill_list( const x_btcl_random_s* o, sz_t size, x_btcl_list_s* list )
{
    // bcore_x_btcl_random.x:35:5
    BLM_INIT_LEVEL(0);
    x_btcl_list_s_set_size(list,size );
    bcore_prsg* prsg = ((bcore_prsg*)BLM_LEVEL_A_PUSH(0,bcore_prsg_a_clone(o->prsg)));
    if( !prsg ) prsg =((bcore_prsg*)( ((bcore_prsg_lcg_u3_00_s*)BLM_LEVEL_T_PUSH(0,bcore_prsg_lcg_u3_00_s,bcore_prsg_lcg_u3_00_s_create()))));
    
    bcore_prsg_a_set_state_u3(prsg,o->seed );
    
    for(sz_t i = 0; i < size; i++ )
    {
        sr_s_from_f3(&(list->arr.data[ i ]),bcore_prsg_a_gen_f3(prsg,o->min, o->max ) );
    }
    BLM_RETURNV(const x_btcl_random_s*,o)
}

sz_t x_btcl_random_s_btcl_function_arity( const x_btcl_random_s* o, tp_t name )
{
    // bcore_x_btcl_random.x:49:5
    
    switch( name )
    {
        case TYPEOF_list: return  1;
        default: break;
    }
    return  -1; // return -1 to indicate that a function of given name does not exist
}

er_t x_btcl_random_s_btcl_function( const x_btcl_random_s* o, tp_t name, const x_source_point_s* sp, x_btcl_frame_s* lexical_frame, const bcore_arr_sr_s* args, sr_s* result )
{
    // bcore_x_btcl_random.x:59:5
    
    switch( name )
    {
        case TYPEOF_list:
        {BLM_INIT_LEVEL(3);
            x_btcl_list_s* list = ((x_btcl_list_s*)BLM_LEVEL_T_PUSH(3,x_btcl_list_s,x_btcl_list_s_create()));
            x_btcl_random_s_fill_list(o,sz_max( 0, sr_s_to_f3(&(args->data[0])) ), list );
            sr_s_asm(result,((x_btcl_list_s*)bcore_fork(list)) );
            BLM_RETURNV(er_t, 0)
        BLM_DOWN();}
    
        default: return  x_source_point_s_parse_error_fa(sp,"Invalid function name '#<sc_t>'.", bnameof( name ) );
    }
    return  0;
}

XOILA_DEFINE_SPECT_NASC_BEGIN( x_btcl, x_btcl_random )
"{",
    "bcore_spect_header_s header;",
"}",
XOILA_DEFINE_SPECT_NASC_END( x_btcl, x_btcl_random )

//----------------------------------------------------------------------------------------------------------------------
// group: x_btcl_test; embeds: bcore_x_btcl_test.x

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_btcl_test_adder_s )
"aware x_btcl_test",
"{",
    "f3_t additive;",
    "func x_btcl:btcl_function_arity;",
    "func x_btcl:btcl_function;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_btcl_test_adder_s )

sz_t x_btcl_test_adder_s_btcl_function_arity( const x_btcl_test_adder_s* o, tp_t name )
{
    // bcore_x_btcl_test.x:28:5
    
    switch( name )
    {
        case TYPEOF_add_a_b: return  2;
        case TYPEOF_add_a  : return  1;
        default: break;
    }
    return  -1; // return -1 to indicate that a function of given name does not exist
}

er_t x_btcl_test_adder_s_btcl_function( const x_btcl_test_adder_s* o, tp_t name, const x_source_point_s* sp, x_btcl_frame_s* lexical_frame, const bcore_arr_sr_s* args, sr_s* result )
{
    // bcore_x_btcl_test.x:39:5
    
    switch( name )
    {
        case TYPEOF_add_a_b: sr_s_from_f3(result,o->additive + sr_s_to_f3(&(args->data[0])) + sr_s_to_f3(&(args->data[1])) ); break;
        case TYPEOF_add_a  : sr_s_from_f3(result,o->additive + sr_s_to_f3(&(args->data[0])) ); break;
        default: return  x_source_point_s_parse_error_fa(sp,"Invalid function name '#<sc_t>'.", bnameof( name ) );
    }
    return  0;
}

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_btcl_test_minimal_s )
"aware x_btcl_test",
"{",
    "func x_btcl:btcl_function_arity;",
    "func x_btcl:btcl_function;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_btcl_test_minimal_s )

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_btcl_test_parser_s )
"aware x_btcl_test",
"{",
    "func x_btcl:btcl_external_parse;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_btcl_test_parser_s )

er_t x_btcl_test_parser_s_btcl_external_parse( const x_btcl_test_parser_s* o, x_source* source, x_btcl_frame_s* lexical_frame, sr_s* result )
{
    // bcore_x_btcl_test.x:71:5
    BLM_INIT_LEVEL(0);
    st_s* string = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
    while( !x_source_eos(source) && ( x_source_inspect_char(source) != ')' ) )
    {
        st_s_push_char(string,x_source_get_char(source) );
    }
    sr_s_asm(result,((st_s*)bcore_fork(string)) );
    BLM_RETURNV(er_t, 0)
}

XOILA_DEFINE_SPECT_NASC_BEGIN( x_btcl, x_btcl_test )
"{",
    "bcore_spect_header_s header;",
"}",
XOILA_DEFINE_SPECT_NASC_END( x_btcl, x_btcl_test )

/**********************************************************************************************************************/
// source: bcore_x_hmap.h
#include "bcore_x_hmap.h"

//----------------------------------------------------------------------------------------------------------------------
// group: x_hmap; embeds: bcore_x_hmap.x

XOILA_DEFINE_SPECT_NASC_BEGIN( x_inst, x_hmap )
"{",
    "bcore_spect_header_s header;",
"}",
XOILA_DEFINE_SPECT_NASC_END( x_inst, x_hmap )

//----------------------------------------------------------------------------------------------------------------------
// group: x_hmap_tp

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_hmap_tp_s )
"aware x_hmap_tp",
"{",
    "bcore_hmap_tpaw_s map;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_hmap_tp_s )

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

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_hmap_tp_st_s )
"aware x_hmap_tp",
"{",
    "x_hmap_tp_s map;",
    "func ^:TE;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_hmap_tp_st_s )

st_s* x_hmap_tp_st_s_create_st_status( const x_hmap_tp_st_s* o )
{
    // bcore_x_hmap.x:23:1
    
    st_s* string = st_s_create();
    st_s_push_fa(string,"keys ........... #<sz_t>\n", x_hmap_tp_keys(((const x_hmap_tp*)(o))) );
    st_s_push_fa(string,"nodes .......... #<sz_t>\n", x_hmap_tp_size(((const x_hmap_tp*)(o))) );
    st_s_push_fa(string,"keys/nodes ..... #<f3_t>\n", x_hmap_tp_size(((const x_hmap_tp*)(o))) > 0 ? ( f3_t )( x_hmap_tp_keys(((const x_hmap_tp*)(o))) ) / x_hmap_tp_size(((const x_hmap_tp*)(o))) : 0 );
    return  string;
}

XOILA_DEFINE_SPECT_NASC_BEGIN( x_hmap, x_hmap_tp )
"{",
    "bcore_spect_header_s header;",
    "feature aware x_hmap_tp : TE = x_hmap_tp_TE_default;",
"}",
XOILA_DEFINE_SPECT_NASC_END( x_hmap, x_hmap_tp )

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

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_hmap_tp_test_kv_s )
"x_hmap_tp_test",
"{",
    "tp_t key;",
    "u3_t val;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_hmap_tp_test_kv_s )

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_hmap_tp_test_kv_arr_s )
"aware x_array",
"{",
    "x_hmap_tp_test_kv_s [];",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_hmap_tp_test_kv_arr_s )

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_hmap_tp_test_val_s )
"aware x_hmap_tp_test",
"{",
    "u3_t val;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_hmap_tp_test_val_s )

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_hmap_tp_test_map_s )
"aware x_hmap_tp",
"{",
    "x_hmap_tp_s map;",
    "func ^:TE;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_hmap_tp_test_map_s )

st_s* x_hmap_tp_test_map_s_create_st_status( const x_hmap_tp_test_map_s* o )
{
    // bcore_x_hmap.x:43:5
    
    st_s* string = st_s_create();
    st_s_push_fa(string,"keys ........... #<sz_t>\n", x_hmap_tp_keys(((const x_hmap_tp*)(o))) );
    st_s_push_fa(string,"nodes .......... #<sz_t>\n", x_hmap_tp_size(((const x_hmap_tp*)(o))) );
    st_s_push_fa(string,"keys/nodes ..... #<f3_t>\n", x_hmap_tp_size(((const x_hmap_tp*)(o))) > 0 ? ( f3_t )( x_hmap_tp_keys(((const x_hmap_tp*)(o))) ) / x_hmap_tp_size(((const x_hmap_tp*)(o))) : 0 );
    return  string;
}

XOILA_DEFINE_SPECT_NASC_BEGIN( x_hmap, x_hmap_tp_test )
"{",
    "bcore_spect_header_s header;",
"}",
XOILA_DEFINE_SPECT_NASC_END( x_hmap, x_hmap_tp_test )

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
        BLM_TRY_EXIT(x_stamp_set_sr(((x_stamp*)(map_trans)),btypeof( "map" ), x_stamp_c_get_sr(((x_stamp*)(map)),btypeof( "map" ) ) ))
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

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_deque_inst_adl_s )
"aware x_array",
"{",
    "aware x_inst => [];",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_deque_inst_adl_s )

BCORE_DEFINE_OBJECT_INST_P_NASC_BEGIN( x_deque_inst_s )
"aware x_deque",
"{",
    "x_deque_inst_adl_s adl;",
    "sz_t size;",
    "sz_t first;",
"}",
BCORE_DEFINE_OBJECT_INST_P_NASC_END( x_deque_inst_s )

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

XOILA_DEFINE_SPECT_NASC_BEGIN( x_inst, x_deque )
"{",
    "bcore_spect_header_s header;",
"}",
XOILA_DEFINE_SPECT_NASC_END( x_inst, x_deque )

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
            BCORE_REGISTER_FUNC( x_inst_is_creatable );
            BCORE_REGISTER_FUNC( x_inst_has_reflection );
            BCORE_REGISTER_FUNC( x_inst_t_copy );
            BCORE_REGISTER_FUNC( x_inst_copy );
            BCORE_REGISTER_FUNC( x_inst_fork );
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
            BCORE_REGISTER_FUNC( x_array_t_is_pure_array );
            BCORE_REGISTER_FUNC( x_array_t_is_fixed );
            BCORE_REGISTER_FUNC( x_array_t_is_static );
            BCORE_REGISTER_FUNC( x_array_t_is_of_aware );
            BCORE_REGISTER_FUNC( x_array_t_is_of_links );
            BCORE_REGISTER_FUNC( x_array_t_is_mono_typed );
            BCORE_REGISTER_FUNC( x_array_is_array );
            BCORE_REGISTER_FUNC( x_array_is_pure_array );
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
            BCORE_REGISTER_FUNC( x_array_t_insert_sr );
            BCORE_REGISTER_FUNC( x_array_t_remove );
            BCORE_REGISTER_FUNC( x_array_t_pop );
            BCORE_REGISTER_FUNC( x_array_t_swap );
            BCORE_REGISTER_FUNC( x_array_set_d );
            BCORE_REGISTER_FUNC( x_array_set_c );
            BCORE_REGISTER_FUNC( x_array_set_t );
            BCORE_REGISTER_FUNC( x_array_set_sr );
            BCORE_REGISTER_FUNC( x_array_insert_sr );
            BCORE_REGISTER_FUNC( x_array_remove );
            BCORE_REGISTER_FUNC( x_array_pop );
            BCORE_REGISTER_FUNC( x_array_swap );
            BCORE_REGISTER_FUNC( x_array_t_remove_null_elements );
            BCORE_REGISTER_FUNC( x_array_remove_null_elements );
            BCORE_REGISTER_FUNC( x_array_t_m_get_sr );
            BCORE_REGISTER_FUNC( x_array_m_get_sr );
            BCORE_REGISTER_FUNC( x_array_c_get_sr );
            BCORE_REGISTER_FUNC( x_array_c_get_f3 );
            BCORE_REGISTER_FUNC( x_array_c_get_u3 );
            BCORE_REGISTER_FUNC( x_array_c_get_s3 );
            BCORE_REGISTER_FUNC( x_array_c_get_bl );
            BCORE_REGISTER_FUNC( x_array_c_get_tp );
            BCORE_REGISTER_FUNC( x_array_t_reorder );
            BCORE_REGISTER_FUNC( x_array_reorder );
            BCORE_REGISTER_FUNC( x_array_t_push_d );
            BCORE_REGISTER_FUNC( x_array_t_push_c );
            BCORE_REGISTER_FUNC( x_array_t_push_t );
            BCORE_REGISTER_FUNC( x_array_t_push );
            BCORE_REGISTER_FUNC( x_array_t_set_d );
            BCORE_REGISTER_FUNC( x_array_t_set_c );
            BCORE_REGISTER_FUNC( x_array_t_set_t );
            BCORE_REGISTER_FUNC( x_array_t_insert_d );
            BCORE_REGISTER_FUNC( x_array_t_insert_c );
            BCORE_REGISTER_FUNC( x_array_t_insert_t );
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
            BCORE_REGISTER_FUNC( x_stamp_is_stamp );
            BCORE_REGISTER_FUNC( x_stamp_t_is_stamp );
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
            BCORE_REGISTER_FUNC( x_stamp_is_static_link_i );
            BCORE_REGISTER_FUNC( x_stamp_t_is_static_link_i );
            BCORE_REGISTER_FUNC( x_stamp_is_static_link );
            BCORE_REGISTER_FUNC( x_stamp_t_is_static_link );
            BCORE_REGISTER_FUNC( x_stamp_is_typed_link_i );
            BCORE_REGISTER_FUNC( x_stamp_t_is_typed_link_i );
            BCORE_REGISTER_FUNC( x_stamp_is_typed_link );
            BCORE_REGISTER_FUNC( x_stamp_t_is_typed_link );
            BCORE_REGISTER_FUNC( x_stamp_is_aware_link_i );
            BCORE_REGISTER_FUNC( x_stamp_t_is_aware_link_i );
            BCORE_REGISTER_FUNC( x_stamp_is_aware_link );
            BCORE_REGISTER_FUNC( x_stamp_t_is_aware_link );
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
            BCORE_REGISTER_FUNC( x_threads_get_id );
            BCORE_REGISTER_FUNC( x_threads_set_nice_level );
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
            BCORE_REGISTER_FUNC( bcore_cday_of_epoch );
            BCORE_REGISTER_FUNC( bcore_cday_to_wnum );
            BCORE_REGISTER_FUNC( bcore_cday_to_wday );
            BCORE_REGISTER_FUNC( bcore_cday_wday_to_sc );
            BCORE_REGISTER_FUNC( bcore_cday_to_sink );
            BCORE_REGISTER_FUNC( bcore_cday_to_string );
            BCORE_REGISTER_FUNC( bcore_cday_push_to_string );
            BCORE_REGISTER_FUNC( bcore_cday_from_source );
            BCORE_REGISTER_FUNC( bcore_cday_from_sc );
            BCORE_REGISTER_FUNC( bcore_cday_from_string );
            XOILA_REGISTER_SPECT( bcore_cday );

            // --------------------------------------------------------------------
            // source: bcore_error_manager.h

            // group: bcore_error
            BCORE_REGISTER_NAME( general_error );
            BCORE_REGISTER_NAME( conversion_error );
            BCORE_REGISTER_NAME( parse_error );
            BCORE_REGISTER_NAME( plant_error );
            BCORE_REGISTER_NAME( error_stack );
            BCORE_REGISTER_NAME( assert_error );
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
            BCORE_REGISTER_FEATURE( bcore_prsg_set_state_f3 );
            BCORE_REGISTER_FFUNC( bcore_prsg_set_state_f3, bcore_prsg_set_state_f3_default );
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
            // source: bcore_parse.h

            // group: bcore_parse
            BCORE_REGISTER_FUNC( bcore_parse_number_literal );
            XOILA_REGISTER_SPECT( bcore_parse );

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
            BCORE_REGISTER_FUNC( x_btml_create_from_file_t );
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
            BCORE_REGISTER_FUNC( x_bbml_create_from_file_t );
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
            BCORE_REGISTER_FUNC( x_bcml_create_from_file_t );
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
            BCORE_REGISTER_OBJECT( x_btcl_s );
            BCORE_REGISTER_FEATURE( x_btcl_btcl_function_arity );
            BCORE_REGISTER_FFUNC( x_btcl_btcl_function_arity, x_btcl_btcl_function_arity_default );
            BCORE_REGISTER_FEATURE( x_btcl_btcl_function );
            BCORE_REGISTER_FEATURE( x_btcl_m_btcl_function );
            BCORE_REGISTER_FEATURE( x_btcl_btcl_function_mutable );
            BCORE_REGISTER_FFUNC( x_btcl_btcl_function_mutable, x_btcl_btcl_function_mutable_default );
            BCORE_REGISTER_FEATURE( x_btcl_btcl_external_parse );
            BCORE_REGISTER_FEATURE( x_btcl_m_btcl_external_parse );
            BCORE_REGISTER_FEATURE( x_btcl_nullary_f3 );
            BCORE_REGISTER_FEATURE( x_btcl_unary_f3 );
            BCORE_REGISTER_FEATURE( x_btcl_binary_f3 );
            BCORE_REGISTER_FEATURE( x_btcl_ternary_f3 );
            BCORE_REGISTER_NAME( member );
            BCORE_REGISTER_NAME( frame );
            BCORE_REGISTER_NAME( exp );
            BCORE_REGISTER_NAME( log );
            BCORE_REGISTER_NAME( log2 );
            BCORE_REGISTER_NAME( log10 );
            BCORE_REGISTER_NAME( sin );
            BCORE_REGISTER_NAME( cos );
            BCORE_REGISTER_NAME( tan );
            BCORE_REGISTER_NAME( tanh );
            BCORE_REGISTER_NAME( sign );
            BCORE_REGISTER_NAME( sqrt );
            BCORE_REGISTER_NAME( abs );
            BCORE_REGISTER_NAME( ceil );
            BCORE_REGISTER_NAME( floor );
            BCORE_REGISTER_NAME( max );
            BCORE_REGISTER_NAME( min );
            BCORE_REGISTER_NAME( ife );
            BCORE_REGISTER_NAME( size );
            BCORE_REGISTER_NAME( identity );
            BCORE_REGISTER_NAME( neg );
            BCORE_REGISTER_NAME( not );
            BCORE_REGISTER_NAME( print );
            BCORE_REGISTER_NAME( println );
            BCORE_REGISTER_NAME( printx );
            BCORE_REGISTER_NAME( assert );
            BCORE_REGISTER_NAME( mkdir );
            BCORE_REGISTER_NAME( rmdir );
            BCORE_REGISTER_NAME( file_exists );
            BCORE_REGISTER_NAME( to_file );
            BCORE_REGISTER_NAME( to_file_btml );
            BCORE_REGISTER_NAME( to_file_bbml );
            BCORE_REGISTER_NAME( to_file_bcml );
            BCORE_REGISTER_NAME( from_file );
            BCORE_REGISTER_NAME( from_file_btml );
            BCORE_REGISTER_NAME( from_file_bbml );
            BCORE_REGISTER_NAME( from_file_bcml );
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
            BCORE_REGISTER_NAME( equal );
            BCORE_REGISTER_NAME( unequal );
            BCORE_REGISTER_NAME( larger_equal );
            BCORE_REGISTER_NAME( larger );
            BCORE_REGISTER_NAME( smaller_equal );
            BCORE_REGISTER_NAME( smaller );
            BCORE_REGISTER_NAME( and );
            BCORE_REGISTER_NAME( or );
            BCORE_REGISTER_NAME( conditional );
            BCORE_REGISTER_NAME( spawn );
            BCORE_REGISTER_NAME( cat );
            BCORE_REGISTER_NAME( shift_left );
            BCORE_REGISTER_NAME( assign );
            BCORE_REGISTER_NAME( continuation );
            BCORE_REGISTER_NAME( if );
            BCORE_REGISTER_NAME( else );
            BCORE_REGISTER_NAME( self );
            BCORE_REGISTER_NAME( func );
            BCORE_REGISTER_NAME( embed );
            BCORE_REGISTER_NAME( eval_file );
            BCORE_REGISTER_NAME( eval_string );
            BCORE_REGISTER_NAME( embed_file );
            BCORE_REGISTER_NAME( embed_string );
            BCORE_REGISTER_NAME( prefix );
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
            BCORE_REGISTER_NAME( MAX );
            BCORE_REGISTER_NAME( MIN );
            BCORE_REGISTER_NAME( IFE );
            BCORE_REGISTER_NAME( PRINT );
            BCORE_REGISTER_NAME( PRINTLN );
            BCORE_REGISTER_NAME( PRINTX );
            BCORE_REGISTER_NAME( ASSERT );
            BCORE_REGISTER_NAME( MKDIR );
            BCORE_REGISTER_NAME( RMDIR );
            BCORE_REGISTER_NAME( FILE_EXISTS );
            BCORE_REGISTER_NAME( TO_FILE );
            BCORE_REGISTER_NAME( TO_FILE_BTML );
            BCORE_REGISTER_NAME( TO_FILE_BBML );
            BCORE_REGISTER_NAME( TO_FILE_BCML );
            BCORE_REGISTER_NAME( FROM_FILE );
            BCORE_REGISTER_NAME( FROM_FILE_BTML );
            BCORE_REGISTER_NAME( FROM_FILE_BBML );
            BCORE_REGISTER_NAME( FROM_FILE_BCML );
            BCORE_REGISTER_NAME( true );
            BCORE_REGISTER_NAME( false );
            BCORE_REGISTER_NAME( TRUE );
            BCORE_REGISTER_NAME( FALSE );
            BCORE_REGISTER_NAME( PI );
            BCORE_REGISTER_NAME( PATH );
            BCORE_REGISTER_NAME( DIR );
            BCORE_REGISTER_NAME( parse );
            BCORE_REGISTER_NAME( parse_file );
            BCORE_REGISTER_FUNC( x_btcl_t_from_source );
            BCORE_REGISTER_FUNC( x_btcl_from_source );
            BCORE_REGISTER_FUNC( x_btcl_t_from_file );
            BCORE_REGISTER_FUNC( x_btcl_from_file );
            BCORE_REGISTER_FUNC( x_btcl_t_from_st );
            BCORE_REGISTER_FUNC( x_btcl_from_st );
            BCORE_REGISTER_FUNC( x_btcl_t_from_sc );
            BCORE_REGISTER_FUNC( x_btcl_from_sc );
            BCORE_REGISTER_FUNC( x_btcl_create_from_source_t );
            BCORE_REGISTER_FUNC( x_btcl_create_from_st_t );
            BCORE_REGISTER_FUNC( x_btcl_create_from_sc_t );
            BCORE_REGISTER_FUNC( x_btcl_create_from_file_t );
            BCORE_REGISTER_FUNC( x_btcl_create_from_source );
            BCORE_REGISTER_FUNC( x_btcl_create_from_st );
            BCORE_REGISTER_FUNC( x_btcl_create_from_sc );
            BCORE_REGISTER_FUNC( x_btcl_create_from_file );
            BCORE_REGISTER_FUNC( x_btcl_run_from_source );
            BCORE_REGISTER_FUNC( x_btcl_run_from_st );
            BCORE_REGISTER_FUNC( x_btcl_run_from_sc );
            BCORE_REGISTER_FUNC( x_btcl_run_from_file );
            BCORE_REGISTER_FUNC( x_btcl_parse_create_object );
            BCORE_REGISTER_FUNC( x_btcl_operator_symbol );
            BCORE_REGISTER_FUNC( x_btcl_is_identifier );
            BCORE_REGISTER_FUNC( x_btcl_clone_if_weak );
            BCORE_REGISTER_FUNC( x_btcl_clone_if_weak_or_twice_referenced );
            BCORE_REGISTER_FUNC( x_btcl_generic_copy );
            BCORE_REGISTER_FUNC( x_btcl_max_frame_depth );
            BCORE_REGISTER_FUNC( x_btcl_max_frame_eval_depth );
            BCORE_REGISTER_FUNC( x_btcl_priority_a );
            BCORE_REGISTER_FUNC( x_btcl_priority_b );
            BCORE_REGISTER_FUNC( x_btcl_priority_c );
            BCORE_REGISTER_FUNC( x_btcl_priority_d );
            BCORE_REGISTER_FUNC( x_btcl_priority_e );
            BCORE_REGISTER_FUNC( x_btcl_to_sink );
            BCORE_REGISTER_FUNC( x_btcl_bop_cat_ab );
            BCORE_REGISTER_FUNC( x_btcl_selftest );
            XOILA_REGISTER_SPECT( x_btcl );

            // group: x_btcl_operator
            BCORE_REGISTER_FEATURE( x_btcl_operator_is_operator );
            BCORE_REGISTER_FFUNC( x_btcl_operator_is_operator, x_btcl_operator_is_operator_default );
            BCORE_REGISTER_FEATURE( x_btcl_operator_is_exportable_operand );
            BCORE_REGISTER_FFUNC( x_btcl_operator_is_exportable_operand, x_btcl_operator_is_exportable_operand_default );
            BCORE_REGISTER_FEATURE( x_btcl_operator_solve );
            BCORE_REGISTER_FFUNC( x_btcl_operator_solve, x_btcl_operator_solve_default );
            BCORE_REGISTER_FEATURE( x_btcl_operator_signal );
            BCORE_REGISTER_FFUNC( x_btcl_operator_signal, x_btcl_operator_signal_default );
            BCORE_REGISTER_FEATURE( x_btcl_operator_execute );
            BCORE_REGISTER_FFUNC( x_btcl_operator_execute, x_btcl_operator_execute_default );
            BCORE_REGISTER_FFUNC( x_btcl_operator_is_operator, x_btcl_operator_uop_s_is_operator );
            BCORE_REGISTER_FFUNC( x_btcl_operator_is_exportable_operand, x_btcl_operator_uop_s_is_exportable_operand );
            BCORE_REGISTER_FFUNC( x_btcl_operator_signal, x_btcl_operator_uop_s_signal );
            BCORE_REGISTER_FFUNC( x_btcl_operator_solve, x_btcl_operator_uop_s_solve );
            BCORE_REGISTER_FFUNC( x_btcl_operator_execute, x_btcl_operator_uop_s_execute );
            BCORE_REGISTER_OBJECT( x_btcl_operator_uop_s );
            BCORE_REGISTER_FFUNC( x_btcl_operator_is_operator, x_btcl_operator_bop_s_is_operator );
            BCORE_REGISTER_FFUNC( x_btcl_operator_is_exportable_operand, x_btcl_operator_bop_s_is_exportable_operand );
            BCORE_REGISTER_FFUNC( x_btcl_operator_signal, x_btcl_operator_bop_s_signal );
            BCORE_REGISTER_FFUNC( x_btcl_operator_solve, x_btcl_operator_bop_s_solve );
            BCORE_REGISTER_FFUNC( x_btcl_operator_execute, x_btcl_operator_bop_s_execute );
            BCORE_REGISTER_OBJECT( x_btcl_operator_bop_s );
            BCORE_REGISTER_FFUNC( x_btcl_operator_is_operator, x_btcl_operator_top_s_is_operator );
            BCORE_REGISTER_FFUNC( x_btcl_operator_is_exportable_operand, x_btcl_operator_top_s_is_exportable_operand );
            BCORE_REGISTER_FFUNC( x_btcl_operator_signal, x_btcl_operator_top_s_signal );
            BCORE_REGISTER_FFUNC( x_btcl_operator_solve, x_btcl_operator_top_s_solve );
            BCORE_REGISTER_FFUNC( x_btcl_operator_execute, x_btcl_operator_top_s_execute );
            BCORE_REGISTER_OBJECT( x_btcl_operator_top_s );
            BCORE_REGISTER_FUNC( x_btcl_operator_sr_is_operator );
            BCORE_REGISTER_FUNC( x_btcl_operator_sr_is_exportable_operand );
            BCORE_REGISTER_FUNC( x_btcl_operator_eval_uop_type );
            BCORE_REGISTER_FUNC( x_btcl_operator_eval_bop_type );
            BCORE_REGISTER_FUNC( x_btcl_operator_is_comparable );
            BCORE_REGISTER_FUNC( x_btcl_operator_eval_top_type );
            XOILA_REGISTER_SPECT( x_btcl_operator );

            // group: x_btcl_operator_f3
            BCORE_REGISTER_OBJECT( x_btcl_operator_f3_arg_s );
            BCORE_REGISTER_OBJECT( x_btcl_operator_f3_arg_arr_s );
            BCORE_REGISTER_FEATURE( x_btcl_operator_f3_setup );
            BCORE_REGISTER_FFUNC( x_btcl_operator_f3_setup, x_btcl_operator_f3_setup_default );
            BCORE_REGISTER_FEATURE( x_btcl_operator_f3_get );
            BCORE_REGISTER_FFUNC( x_btcl_operator_f3_get, x_btcl_operator_f3_get_default );
            BCORE_REGISTER_FEATURE( x_btcl_operator_f3_set_args );
            BCORE_REGISTER_FFUNC( x_btcl_operator_f3_set_args, x_btcl_operator_f3_set_args_default );
            BCORE_REGISTER_FEATURE( x_btcl_operator_f3_check_consistency );
            BCORE_REGISTER_FFUNC( x_btcl_operator_f3_check_consistency, x_btcl_operator_f3_check_consistency_default );
            BCORE_REGISTER_FFUNC( x_btcl_operator_f3_set_args, x_btcl_operator_f3_arg_nop_s_set_args );
            BCORE_REGISTER_FFUNC( x_btcl_operator_f3_get, x_btcl_operator_f3_arg_nop_s_get );
            BCORE_REGISTER_FFUNC( x_btcl_operator_f3_check_consistency, x_btcl_operator_f3_arg_nop_s_check_consistency );
            BCORE_REGISTER_OBJECT( x_btcl_operator_f3_arg_nop_s );
            BCORE_REGISTER_FFUNC( x_btcl_operator_f3_get, x_btcl_operator_f3_const_nop_s_get );
            BCORE_REGISTER_FFUNC( x_btcl_operator_f3_check_consistency, x_btcl_operator_f3_const_nop_s_check_consistency );
            BCORE_REGISTER_OBJECT( x_btcl_operator_f3_const_nop_s );
            BCORE_REGISTER_FFUNC( x_btcl_operator_f3_setup, x_btcl_operator_f3_uop_s_setup );
            BCORE_REGISTER_FFUNC( x_btcl_operator_f3_set_args, x_btcl_operator_f3_uop_s_set_args );
            BCORE_REGISTER_FFUNC( x_btcl_operator_f3_get, x_btcl_operator_f3_uop_s_get );
            BCORE_REGISTER_FFUNC( x_btcl_operator_f3_check_consistency, x_btcl_operator_f3_uop_s_check_consistency );
            BCORE_REGISTER_OBJECT( x_btcl_operator_f3_uop_s );
            BCORE_REGISTER_FFUNC( x_btcl_operator_f3_setup, x_btcl_operator_f3_bop_s_setup );
            BCORE_REGISTER_FFUNC( x_btcl_operator_f3_set_args, x_btcl_operator_f3_bop_s_set_args );
            BCORE_REGISTER_FFUNC( x_btcl_operator_f3_get, x_btcl_operator_f3_bop_s_get );
            BCORE_REGISTER_FFUNC( x_btcl_operator_f3_check_consistency, x_btcl_operator_f3_bop_s_check_consistency );
            BCORE_REGISTER_OBJECT( x_btcl_operator_f3_bop_s );
            BCORE_REGISTER_FFUNC( x_btcl_operator_f3_setup, x_btcl_operator_f3_top_s_setup );
            BCORE_REGISTER_FFUNC( x_btcl_operator_f3_set_args, x_btcl_operator_f3_top_s_set_args );
            BCORE_REGISTER_FFUNC( x_btcl_operator_f3_get, x_btcl_operator_f3_top_s_get );
            BCORE_REGISTER_FFUNC( x_btcl_operator_f3_check_consistency, x_btcl_operator_f3_top_s_check_consistency );
            BCORE_REGISTER_OBJECT( x_btcl_operator_f3_top_s );
            BCORE_REGISTER_FUNC( x_btcl_operator_f3_group_signal_init1 );
            BCORE_REGISTER_FUNC( x_btcl_operator_f3_create_op );
            XOILA_REGISTER_SPECT( x_btcl_operator_f3 );
            x_btcl_operator_f3_group_signal_init1();

            // group: x_btcl_net
            BCORE_REGISTER_OBJECT( x_btcl_net_plain_branch_s );
            BCORE_REGISTER_OBJECT( x_btcl_net_plain_branch_arr_s );
            BCORE_REGISTER_OBJECT( x_btcl_net_socket_branch_s );
            BCORE_REGISTER_OBJECT( x_btcl_net_socket_branch_arr_s );
            BCORE_REGISTER_NAME( rack );
            BCORE_REGISTER_NAME( wire );
            BCORE_REGISTER_FFUNC( x_btcl_operator_is_exportable_operand, x_btcl_net_node_s_is_exportable_operand );
            BCORE_REGISTER_OBJECT( x_btcl_net_node_s );
            BCORE_REGISTER_FUNC( x_btcl_net_eval_node_modifier );
            BCORE_REGISTER_FUNC( x_btcl_net_eval_node_member );
            XOILA_REGISTER_SPECT( x_btcl_net );

            // group: x_btcl_functor
            BCORE_REGISTER_OBJECT( x_btcl_functor_arg_val_s );
            BCORE_REGISTER_NAME( signal_arg_uop_update_val );
            BCORE_REGISTER_FFUNC( x_btcl_operator_is_operator, x_btcl_functor_arg_uop_s_is_operator );
            BCORE_REGISTER_FFUNC( x_btcl_operator_is_exportable_operand, x_btcl_functor_arg_uop_s_is_exportable_operand );
            BCORE_REGISTER_FFUNC( x_btcl_operator_solve, x_btcl_functor_arg_uop_s_solve );
            BCORE_REGISTER_FFUNC( x_btcl_operator_execute, x_btcl_functor_arg_uop_s_execute );
            BCORE_REGISTER_FFUNC( x_btcl_operator_signal, x_btcl_functor_arg_uop_s_signal );
            BCORE_REGISTER_OBJECT( x_btcl_functor_arg_uop_s );
            BCORE_REGISTER_OBJECT( x_btcl_functor_arg_s );
            BCORE_REGISTER_OBJECT( x_btcl_functor_arg_arr_s );
            BCORE_REGISTER_NAME( call );
            BCORE_REGISTER_FFUNC( bcore_inst_call_copy_x, x_btcl_functor_s_copy_x );
            BCORE_REGISTER_FFUNC( bcore_via_call_mutated, x_btcl_functor_s_mutated );
            BCORE_REGISTER_FFUNC( x_btcl_btcl_function_arity, x_btcl_functor_s_btcl_function_arity );
            BCORE_REGISTER_FFUNC( x_btcl_btcl_function, x_btcl_functor_s_btcl_function );
            BCORE_REGISTER_FFUNC( x_btcl_nullary_f3, x_btcl_functor_s_nullary_f3 );
            BCORE_REGISTER_FFUNC( x_btcl_unary_f3, x_btcl_functor_s_unary_f3 );
            BCORE_REGISTER_FFUNC( x_btcl_binary_f3, x_btcl_functor_s_binary_f3 );
            BCORE_REGISTER_FFUNC( x_btcl_ternary_f3, x_btcl_functor_s_ternary_f3 );
            BCORE_REGISTER_OBJECT( x_btcl_functor_s );
            BCORE_REGISTER_FUNC( x_btcl_functor_eval_modifier );
            XOILA_REGISTER_SPECT( x_btcl_functor );

            // group: x_btcl_functor_f3
            BCORE_REGISTER_NAME( call );
            BCORE_REGISTER_FFUNC( bcore_inst_call_copy_x, x_btcl_functor_f3_s_copy_x );
            BCORE_REGISTER_FFUNC( bcore_fp_copy_typed, x_btcl_functor_f3_s_copy_typed );
            BCORE_REGISTER_FFUNC( bcore_via_call_mutated, x_btcl_functor_f3_s_mutated );
            BCORE_REGISTER_FFUNC( x_btcl_btcl_function_arity, x_btcl_functor_f3_s_btcl_function_arity );
            BCORE_REGISTER_FFUNC( x_btcl_btcl_function, x_btcl_functor_f3_s_btcl_function );
            BCORE_REGISTER_FFUNC( x_btcl_nullary_f3, x_btcl_functor_f3_s_nullary_f3 );
            BCORE_REGISTER_FFUNC( x_btcl_unary_f3, x_btcl_functor_f3_s_unary_f3 );
            BCORE_REGISTER_FFUNC( x_btcl_binary_f3, x_btcl_functor_f3_s_binary_f3 );
            BCORE_REGISTER_FFUNC( x_btcl_ternary_f3, x_btcl_functor_f3_s_ternary_f3 );
            BCORE_REGISTER_OBJECT( x_btcl_functor_f3_s );
            XOILA_REGISTER_SPECT( x_btcl_functor_f3 );

            // group: x_btcl_random
            BCORE_REGISTER_NAME( list );
            BCORE_REGISTER_FFUNC( x_btcl_btcl_function_arity, x_btcl_random_s_btcl_function_arity );
            BCORE_REGISTER_FFUNC( x_btcl_btcl_function, x_btcl_random_s_btcl_function );
            BCORE_REGISTER_OBJECT( x_btcl_random_s );
            XOILA_REGISTER_SPECT( x_btcl_random );

            // group: x_btcl_test
            BCORE_REGISTER_NAME( add_a_b );
            BCORE_REGISTER_NAME( add_a );
            BCORE_REGISTER_FFUNC( x_btcl_btcl_function_arity, x_btcl_test_adder_s_btcl_function_arity );
            BCORE_REGISTER_FFUNC( x_btcl_btcl_function, x_btcl_test_adder_s_btcl_function );
            BCORE_REGISTER_OBJECT( x_btcl_test_adder_s );
            BCORE_REGISTER_NAME( run );
            BCORE_REGISTER_FFUNC( x_btcl_btcl_function_arity, x_btcl_test_minimal_s_btcl_function_arity );
            BCORE_REGISTER_FFUNC( x_btcl_btcl_function, x_btcl_test_minimal_s_btcl_function );
            BCORE_REGISTER_OBJECT( x_btcl_test_minimal_s );
            BCORE_REGISTER_FFUNC( x_btcl_btcl_external_parse, x_btcl_test_parser_s_btcl_external_parse );
            BCORE_REGISTER_OBJECT( x_btcl_test_parser_s );
            XOILA_REGISTER_SPECT( x_btcl_test );

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
// XOICO_BODY_SIGNATURE 0x655DE19462E92CB1
// XOICO_FILE_SIGNATURE 0xD638B80227E1EDF6
