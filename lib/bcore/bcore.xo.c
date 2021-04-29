//  Last update: 2021-04-28T10:10:03Z
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
 *  bcore_x_source.h
 *  bcore_x_sink.h
 *  bcore_x_array.h
 *  bcore_x_group.h
 *  bcore_x_stamp.h
 *  bcore_file.h
 *  bcore_spect_inst_call.h
 *  bcore_spect_via_call.h
 *  bcore_main.h
 *  bcore_hmap_name.h
 *  bcore_cday.h
 *  bcore_error_manager.h
 *  bcore_prsg.h
 *  bcore_shell.h
 *  bcore_x_btml.h
 *  bcore_x_bbml.h
 *  bcore_arr_inexpandable.x
 *  bcore_flect_inexpandable.x
 *  bcore_hmap_inexpandable.x
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
// HKEYOF_bcore 0xD9796A1AB2C2C0BBull

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
    // bcore_x_inst.h:87:1
    
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
    // bcore_x_source.h:156:1
    
    x_source_point_s_parse_msg_to_sink_fv(o,x_sink_stdout(), format, args );
}

x_source* x_source_point_s_clone_source( const x_source_point_s* o )
{
    // bcore_x_source.h:163:1
    
    x_source* source = x_source_a_clone(o->source);
    x_source_set_index(source,o->index );
    return  source;
}

void x_source_point_s_parse_msg_to_sink_fv( const x_source_point_s* o, x_sink* sink, sc_t format, va_list args )
{
    // bcore_x_source.h:172:1
    
    if( o->source )
    {
        s3_t index = x_source_get_index(o->source);
        x_source_set_index(o->source,o->index );
        x_source_parse_msg_to_sink_fv(o->source,sink, format, args );
        x_source_set_index(o->source,index );
    }
    else
    {
        ((x_sink*)(x_sink_push_fv(sink,format, args )));
    }
}

er_t x_source_point_s_parse_error_fv( const x_source_point_s* o, sc_t format, va_list args )
{
    // bcore_x_source.h:189:1
    
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
    // bcore_x_source.h:208:1
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
        ((x_sink*)(x_sink_push_fa(sink,"#<sc_t>", file->sc )));
    BLM_DOWN();}
    
    ((x_sink*)(x_sink_push_fa(sink,":#<sz_t>:#<sz_t>", context->line, context->col )));
    x_source_set_index(o->source,index );
    BLM_DOWN();
}

XOILA_DEFINE_SPECT( x_inst, x_source )
"{"
    "bcore_spect_header_s header;"
"}";

void x_source_parse_msg_fv( const x_source* o, sc_t format, va_list args )
{
    // bcore_x_source.h:107:1
    
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
    // bcore_x_array.h:137:17
    
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
    // bcore_x_array.h:168:17
    
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
    // bcore_x_array.h:201:17
    
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
    // bcore_x_array.h:226:15
    
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
    // bcore_x_array.h:267:16
    
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
    // bcore_x_array.h:297:16
    
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
    // bcore_x_array.h:328:16
    
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
    // bcore_x_array.h:352:1
    
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
    // bcore_x_stamp.h:141:5
    
    x_stamp_path_s_push(o,TYPEOF_x_stamp_path_s_array_index );
    x_stamp_path_s_push(o,index );
}

const x_stamp_path_s* x_stamp_path_s_to_sink( const x_stamp_path_s* o, bcore_sink* sink )
{
    // bcore_x_stamp.h:188:1
    
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
    // bcore_x_stamp.h:208:1
    BLM_INIT_LEVEL(0);
    ((x_stamp_path_s*)(x_stamp_path_s_clear(o)));
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
    // bcore_x_stamp.h:238:1
    
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
    // bcore_x_stamp.h:287:1
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
    // bcore_x_stamp.h:353:1
    
    const bcore_via_s* p = bcore_via_s_get_typed( t );
    sz_t index = bcore_via_p_nget_index( p, ((const bcore_via*)(o)), name );
    if( index < 0 ) return  sr_null();
    return  bcore_via_p_iget( p, ((const bcore_via*)(o)), index );
}

sr_s x_stamp_t_m_get_sr_i( x_stamp* o, tp_t t, sz_t index )
{
    // bcore_x_stamp.h:363:1
    
    const bcore_via_s* p = bcore_via_s_get_typed( t );
    if( index < 0 || index >= bcore_via_p_get_size( p, ((const bcore_via*)(o)) ) ) return  sr_null();
    return  bcore_via_p_iget( p, ((const bcore_via*)(o)), index );
}

void x_stamp_t_set_sr( x_stamp* o, tp_t t, tp_t name, sr_s sr_src )
{
    // bcore_x_stamp.h:375:1
    
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
    // bcore_x_stamp.h:393:1
    
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
    // bcore_x_stamp.h:413:1
    
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
    // bcore_x_stamp.h:430:1
    
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
    // bcore_x_stamp.h:447:1
    
    sr_s sr = x_stamp_t_m_get_sr(((x_stamp*)(o)),t, name );
    if( sr_s_is_weak(&(sr)) ) sr_s_set_const(&(sr),true );
    return  sr;
}

sr_s x_stamp_t_c_get_sr_i( const x_stamp* o, tp_t t, sz_t index )
{
    // bcore_x_stamp.h:457:1
    
    sr_s sr = x_stamp_t_m_get_sr_i(((x_stamp*)(o)),t, index );
    if( sr_s_is_weak(&(sr)) ) sr_s_set_const(&(sr),true );
    return  sr;
}

void x_stamp_selftest( void )
{
    // bcore_x_stamp.h:470:1
    BLM_INIT_LEVEL(0);
    bcore_prsg_lcg_u3_00_s lcg;BLM_T_INIT_SPUSH(bcore_prsg_lcg_u3_00_s, &lcg);;
    
    ASSERT( x_stamp_exists(((const x_stamp*)(&(lcg))),btypeof( "state" ) ) );
    
    u3_t state =*( ((const u3_t*)(x_stamp_c_get(((const x_stamp*)(&(lcg))),btypeof( "state" ) ))));
    ASSERT( state == 16437 );
    
    x_stamp_set_sr_ret(((x_stamp*)(&( lcg))),btypeof( "state" ), sr_u3(12345 ) );
    state = (*(((const u3_t*)(x_stamp_c_get(((const x_stamp*)(&( lcg))),btypeof( "state" ) )))));
    ASSERT( state == 12345 );
    
    const x_inst* inst = x_stamp_path_s_c_get_in(((x_stamp_path_s*)(x_stamp_path_s_parse_sc(((x_stamp_path_s*)BLM_LEVEL_T_PUSH(0,x_stamp_path_s,x_stamp_path_s_create())),"state" ))),((const x_inst*)(&(lcg ))));
    ASSERT( inst );
    ASSERT( (*(((const u3_t*)(inst)))) == 12345 );
    
    x_inst* zoo = ((x_inst*)BLM_LEVEL_A_PUSH(0,((x_inst*)(bcore_spect_via_create_zoo( 2 ).o))));
    
    sr_s sr = sr_null();
    
    sr = x_stamp_path_s_get_sr_in(((x_stamp_path_s*)(x_stamp_path_s_parse_sc(((x_stamp_path_s*)BLM_LEVEL_T_PUSH(0,x_stamp_path_s,x_stamp_path_s_create())),"[0].[1].vdata.data2.[1]" ))),zoo );
    ASSERT( sr_s_type(&(sr)) == TYPEOF_u3_t );
    ASSERT( sr_s_to_u3(&(sr)) == 0 );
    
    sr = x_stamp_path_s_set_sr_ret_in(((x_stamp_path_s*)(x_stamp_path_s_parse_sc(((x_stamp_path_s*)BLM_LEVEL_T_PUSH(0,x_stamp_path_s,x_stamp_path_s_create())),"[0].[1].vdata.data2.[1]" ))),zoo, sr_u3(20 ) );
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
    // bcore_main.h:120:5
    
    {const bcore_main_arr_s* __a=&(o->arr );if(__a)for(sz_t __i=0; __i<__a->size; __i++){bcore_main* e=__a->data[__i];
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
    // bcore_main.h:134:5
    BLM_INIT_LEVEL(0);
    bcore_lock_s lock;BLM_T_INIT_SPUSH(bcore_lock_s, &lock);bcore_lock_s_set(&(lock),&(o->mutex_current_object ));
    BLM_RETURNV(bl_t, o->current_object ? bcore_main_a_on_termination(o->current_object,frame ) : false)
}

bl_t bcore_main_set_s_on_interrupt( bcore_main_set_s* o, const bcore_main_frame_s* frame )
{
    // bcore_main.h:140:5
    BLM_INIT_LEVEL(0);
    bcore_lock_s lock;BLM_T_INIT_SPUSH(bcore_lock_s, &lock);bcore_lock_s_set(&(lock),&(o->mutex_current_object ));
    BLM_RETURNV(bl_t, o->current_object ? bcore_main_a_on_interrupt(o->current_object,frame ) : false)
}

bl_t bcore_main_set_s_on_suspend( bcore_main_set_s* o, const bcore_main_frame_s* frame )
{
    // bcore_main.h:146:5
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
// source: bcore_shell.h
#include "bcore_shell.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_shell

XOILA_DEFINE_SPECT( x_inst, bcore_shell )
"{"
    "bcore_spect_header_s header;"
    "feature aware bcore_shell : op_group = bcore_shell_op_group_default;"
    "feature aware bcore_shell : push_op_groups = bcore_shell_push_op_groups_default;"
"}";

void bcore_shell_push_op_groups_default( const bcore_shell* o, bcore_arr_tp_s* list )
{
    // bcore_shell.h:272:1
    
    bcore_arr_tp_s_push(list,bcore_shell_a_op_group(o) );
    if( !bcore_arr_tp_s_exists(list,((tp_t)(TYPEOF_bcore_shell_op_default)) ) ) bcore_arr_tp_s_push(list,((tp_t)(TYPEOF_bcore_shell_op_default)) );
}

bcore_arr_tp_s* bcore_shell_get_op_stamps( const bcore_shell* o )
{
    // bcore_shell.h:280:1
    BLM_INIT_LEVEL(0);
    bcore_arr_tp_s op_groups;BLM_T_INIT_SPUSH(bcore_arr_tp_s, &op_groups);;
    bcore_shell_a_push_op_groups(o,&(op_groups ));
    bcore_arr_tp_s* op_stamps = bcore_arr_tp_s_create();
    {const bcore_arr_tp_s* __a=&(op_groups );if(__a)for(sz_t __i=0; __i<__a->size; __i++){tp_t t=__a->data[__i]; bcore_arr_tp_s_push_arr(op_stamps,x_group_get_arr_traitline_stamps(t ) );}}
    BLM_RETURNV(bcore_arr_tp_s*, op_stamps)
}

void bcore_shell_help_to_sink( bcore_shell* o, bcore_sink* sink )
{
    // bcore_shell.h:291:1
    BLM_INIT_LEVEL(0);
    bcore_arr_tp_s op_groups;BLM_T_INIT_SPUSH(bcore_arr_tp_s, &op_groups);;
    bcore_shell_a_push_op_groups(o,&(op_groups ));
    
    {const bcore_arr_tp_s* __a=&(op_groups );if(__a)for(sz_t __i=0; __i<__a->size; __i++){tp_t tp_group=__a->data[__i];
    {BLM_INIT_LEVEL(4);
        bcore_sink_a_push_fa(sink,"#p80'*'{******** #<sc_t> }\n", bnameof( tp_group ) );
        bcore_arr_st_s table;BLM_T_INIT_SPUSH(bcore_arr_st_s, &table);;
        {const bcore_arr_tp_s* __a=x_group_get_arr_traitline_stamps(tp_group ) ;if(__a)for(sz_t __i=0; __i<__a->size; __i++){tp_t t=__a->data[__i];
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
    
    }}BLM_DOWN();
}

void bcore_shell_loop( bcore_shell* o, const bcore_main_frame_s* frame, x_source* source, bcore_sink* sink, bcore_shell_control_s* control )
{
    // bcore_shell.h:330:1
    BLM_INIT_LEVEL(0);
    if( !control ) control = ((bcore_shell_control_s*)BLM_LEVEL_T_PUSH(0,bcore_shell_control_s,bcore_shell_control_s_create()));
    while( !x_source_eos(source) && !bcore_shell_control_s_exit_loop(control) )
    {BLM_INIT_LEVEL(1);
        bcore_sink_a_flush(bcore_sink_a_push_fa(sink,"\n#<sc_t>#<sc_t>(#<sc_t>)> ", control->path.sc, control->path.size ? " " : "", bnameof( o->_ ) ));
        ((bcore_shell_control_s*)(bcore_shell_control_s_reset(control)));
    
        st_s line;BLM_T_INIT_SPUSH(st_s, &line);;
        BLM_TRY_EXIT(x_source_parse_fa(source," #until'\n'#skip'\n'", (&(line)) ))
        x_source* line_source = ((x_source*)BLM_LEVEL_A_PUSH(1,x_source_create_from_st(&(line ))));
    
        if( line.size > 0 )
        {
            bl_t found = false;
    
            {BLM_INIT_LEVEL(3);const bcore_arr_tp_s* __a=((bcore_arr_tp_s*)BLM_LEVEL_T_PUSH(3,bcore_arr_tp_s,bcore_shell_get_op_stamps(o))) ;if(__a)for(sz_t __i=0; __i<__a->size; __i++){tp_t t=__a->data[__i];
            {
                bcore_shell_op* op =((bcore_shell_op*)( ((x_inst*)BLM_LEVEL_TV_PUSH(0,t ,x_inst_t_create(t )))));
                if( bcore_shell_op_parse_match(op,line_source ) )
                {
                    if( bcore_shell_op_parse_param(op,line_source, sink ) )
                    {
                        bcore_shell_op_a_run(op,o, frame, source, sink, control );
                    }
                    found = true;
                    break;
                }
            }
    
            }BLM_DOWN();}if( !found )
            {
                bcore_sink_a_push_fa(sink,"Syntax error.\n" );
                bcore_shell_help_to_sink(o,sink );
            }
        }
    BLM_DOWN();}
    
    control->exit_loop = false;
    BLM_DOWN();
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
    // bcore_shell.h:59:5
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
    // bcore_shell.h:84:5
    BLM_INIT_LEVEL(0);
    st_s key;BLM_T_INIT_SPUSH(st_s, &key);;
    BLM_TRY_EXIT(x_source_parse_fa(source,"#=until' '", (&(key)) ))
    if( key.size == 0 ) BLM_RETURNV(bl_t, false)
    if( !bcore_shell_op_key_match(o,key.sc ) ) BLM_RETURNV(bl_t, false)
    BLM_TRY_EXIT(x_source_parse_fa(source,"#-until' '" ))
    BLM_RETURNV(bl_t, true)
}

bl_t bcore_shell_op_parse_param( bcore_shell_op* o, x_source* source, bcore_sink* sink )
{
    // bcore_shell.h:98:5
    
    sz_t direct_index = 0;
    BLM_TRY_EXIT(x_source_parse_fa(source,"#skip' \t'" ))
    while( !x_source_eos(source) && !x_source_parse_bl(source,"#?'\n'" ) )
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
    // bcore_shell.h:200:5
    
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
    "bl_t exit_loop;"
    "st_s path;"
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
    // bcore_shell.h:388:9
    BLM_INIT_LEVEL(0);
    x_stamp_path_s path;BLM_T_INIT_SPUSH(x_stamp_path_s, &path);((x_stamp_path_s*)(x_stamp_path_s_parse(&(path),((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_sc(o->path.sc ))) )));
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
                {BLM_INIT_LEVEL(17);
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
    // bcore_shell.h:440:9
    BLM_INIT_LEVEL(0);
    const x_stamp_path_s* path = ((x_stamp_path_s*)(x_stamp_path_s_parse(((x_stamp_path_s*)BLM_LEVEL_T_PUSH(0,x_stamp_path_s,x_stamp_path_s_create())),((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_st(&(o->path )))) )));
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
        bcore_shell_loop(shell_o,main_frame, source, sink, control_child );
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
    // bcore_shell.h:486:9
    BLM_INIT_LEVEL(0);
    sr_s sr = x_stamp_path_s_get_sr_in(((x_stamp_path_s*)(x_stamp_path_s_parse(((x_stamp_path_s*)BLM_LEVEL_T_PUSH(0,x_stamp_path_s,x_stamp_path_s_create())),((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_st(&(o->path )))) ))),((const x_inst*)(obj )));
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
    // bcore_shell.h:506:9
    BLM_INIT_LEVEL(0);
    sr_s sr = x_stamp_path_s_get_sr_in(((x_stamp_path_s*)(x_stamp_path_s_parse(((x_stamp_path_s*)BLM_LEVEL_T_PUSH(0,x_stamp_path_s,x_stamp_path_s_create())),((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_st(&(o->path )))) ))),((const x_inst*)(obj )));
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
            ((x_inst*)(x_inst_t_copy_typed(sr.o,sr_s_type(&( sr )), type,((const x_inst*)( val )))));
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

XOILA_DEFINE_SPECT( bcore_shell, bcore_shell_op_default )
"{"
    "bcore_spect_header_s header;"
"}";

/**********************************************************************************************************************/
// source: bcore_x_btml.h
#include "bcore_x_btml.h"

//----------------------------------------------------------------------------------------------------------------------
// group: x_btml

XOILA_DEFINE_SPECT( x_inst, x_btml )
"{"
    "bcore_spect_header_s header;"
    "feature aware x_btml : feature_body_from_source;"
    "feature aware x_btml : feature_body_to_sink;"
"}";

er_t x_btml_t_from_source( x_btml* o, tp_t t, x_source* source )
{
    // bcore_x_btml.h:144:1
    
    sr_s sr = sr_null();
    BLM_TRY(x_btml_parse_create_object(source, (&(sr)) ))
    ((x_btml*)(x_inst_t_copy_typed(((x_inst*)( o)),t, sr_s_o_type( (&(sr)) ), sr.o )));
    sr_s_down(&( sr ));
    return  bcore_error_last();
}

x_btml* x_btml_create_from_source_t( x_source* source, tp_t* type )
{
    // bcore_x_btml.h:155:1
    
    sr_s sr = sr_null();
    BLM_TRY_EXIT(x_btml_parse_create_object(source, (&(sr)) ))
    if( sr.o && type ) (*(type)) = sr_s_o_type(&( sr ));
    return  ((x_btml*)(sr.o)); // sr.o is NULL in case of error
}

x_btml* x_btml_create_from_source( x_source* source )
{
    // bcore_x_btml.h:165:1
    
    tp_t t = 0;
    x_btml* o = x_btml_create_from_source_t(source, (&(t)) );
    if( t )
    {
        ASSERT( x_stamp_t_is_aware(t ) );
    }
    return  o;
}

void x_btml_t_to_sink( const x_btml* o, tp_t t, x_sink* sink )
{
    // bcore_x_btml.h:178:1
    
    x_btml_t_translate_recursive(o,t, 0, true, sink, 0 );
}

sc_t x_btml_name_of( tp_t type, st_s* buf )
{
    // bcore_x_btml.h:190:1
    
    sc_t n = bcore_name_try_name( type );
    if( n ) return  n;
    st_s_copy_typed(buf,((tp_t)(TYPEOF_tp_t)), (&(type)) );
    return  buf->sc;
}

tp_t x_btml_type_of( const st_s* name )
{
    // bcore_x_btml.h:200:1
    
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
    // bcore_x_btml.h:220:1
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
        }
    }
    
    x_source_set_index(source,index );
    BLM_RETURNV(bl_t, valid)
}

er_t x_btml_parse_create_object( x_source* source, sr_s* obj )
{
    // bcore_x_btml.h:248:1
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
                    BLM_TRY(x_btml_t_parse_body(((x_btml*)( inst)),type, source ))
                    BLM_TRY(x_source_parse_fa(source," </>" ))
                    (*(obj)) = sr_tsd(type, ((x_inst*)bcore_fork(inst)) );
                }
            }
            else if( type == btypeof( "#file" ) )
            {BLM_INIT_LEVEL(4);
                st_s* file = ((st_s*)BLM_LEVEL_T_PUSH(4,st_s,st_s_create()));
                BLM_TRY(x_source_parse_fa(source," #string </>", file ))
    
                if( file->data[ 0 ] != '/' ) // make path relative to current file path
                {BLM_INIT_LEVEL(5);
                    st_s* cur_file = ((st_s*)BLM_LEVEL_T_PUSH(5,st_s,st_s_create_sc(x_source_get_file(source) )));
                    if( cur_file->size > 0 )
                    {
                        sz_t idx = st_s_find_char(cur_file,cur_file->size, 0, '/' );
                        if( idx < cur_file->size )
                        {BLM_INIT_LEVEL(7);
                            cur_file->data[ idx ] = 0;
                            st_s_copy(file,((st_s*)BLM_LEVEL_T_PUSH(7,st_s,st_s_create_fa("#<sc_t>/#<sc_t>", cur_file->sc, file->sc ))) );
                        BLM_DOWN();}
                    }
                BLM_DOWN();}
    
                if( bcore_file_exists(file->sc ) )
                {BLM_INIT_LEVEL(5);
                    BLM_TRY(x_btml_parse_create_object(((x_source*)(((bcore_source*)BLM_LEVEL_A_PUSH(5,bcore_file_open_source(file->sc ))))), obj ))
                BLM_DOWN();}
                else
                {
                    BLM_RETURNV(er_t, x_source_parse_error_fa(source,"File '#<sc_t>' not found.", file->sc ))
                }
            BLM_DOWN();}
            else
            {
                BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Type '#<sc_t>' has no reflection.", type_string->sc ))
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
    // bcore_x_btml.h:351:1
    
    x_stamp* stamp =((x_stamp*)( o));
    if( x_btml_t_defines_feature_body_from_source(t ) )
    {
        BLM_TRY(x_btml_t_feature_body_from_source(o,t, source ))
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
                BLM_TRY(x_btml_parse_create_object(source, (&(sr)) ))
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
                BLM_TRY(x_btml_parse_create_object(source, (&(sr)) ))
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
                BLM_TRY(x_btml_parse_create_object(source, (&(sr)) ))
                x_array_t_push_sr(arr,t, sr );
            }
        }
    }
    else
    {
        while( !x_source_parse_bl(source," #=?'</>'" ) )
        {BLM_INIT_LEVEL(2);
            // non existing member variables are parsed but not assigned
            st_s* name = ((st_s*)BLM_LEVEL_T_PUSH(2,st_s,st_s_create()));
            BLM_TRY(x_source_parse_fa(source," #name :", name ))
            tp_t tp_name = btypeof( name->sc );
            sr_s sr = sr_null();
            BLM_TRY(x_btml_parse_create_object(source, (&(sr)) ))
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
    
    return  0;
}

er_t x_btml_skip_body( x_source* source )
{
    // bcore_x_btml.h:457:1
    
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
    // bcore_x_btml.h:482:1
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
    
    ((x_sink*)(x_sink_push_fa(sink,"#rn{ }", indent )));
    if( name ) ((x_sink*)(x_sink_push_fa(sink,"#<sc_t>:", x_btml_name_of(name, buf ) )));
    
    if( !o ) // o == NULL ...
    {
        if( t )
        {
            // Explicit NULL specifies that the object is not instantiated.
            // This is to differentiate from the case where the object was instantiated
            // without additional parameters.
            ((x_sink*)(x_sink_push_fa(sink,"<#<sc_t>>NULL</>\n", x_btml_name_of(t, buf ) )));
        }
        else
        {
            ((x_sink*)(x_sink_push_fa(sink,"<></>\n" )));
        }
    }
    else // o != NULL ...
    {
        ((x_sink*)(x_sink_push_fa(sink,"<#<sc_t>>", x_btml_name_of(t, buf ) )));
    
        if( x_btml_t_defines_feature_body_to_sink(t ) )
        {
            x_btml_t_feature_body_to_sink(o,t, sink );
        }
        else if( t == TYPEOF_st_s ) // strings
        {BLM_INIT_LEVEL(3);
            st_s* string = ((st_s*)BLM_LEVEL_T_PUSH(3,st_s,st_s_clone(((const st_s*)(o)))));
            st_s_replace_char_sc(string,'\"', "\\\"" );
            ((x_sink*)(x_sink_push_fa(sink,"\"#<sc_t>\"", string->sc )));
        BLM_DOWN();}
        else if( x_stamp_t_is_leaf(t ) )
        {
            ((x_sink*)(x_sink_push_st_d(sink,st_s_create_typed(t, o ) )));
        }
        else
        {
            ((x_sink*)(x_sink_push_char(sink,'\n' )));
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
            ((x_sink*)(x_sink_push_fa(sink,"#rn{ }", indent )));
        }
        ((x_sink*)(x_sink_push_sc(sink,"</>\n" )));
    }
    BLM_DOWN();
}

void x_btml_t_test_transfer( const x_btml* o, tp_t t )
{
    // bcore_x_btml.h:571:1
    BLM_INIT_LEVEL(0);
    st_s* string = st_s_create();
    x_btml_t_to_sink(o,t,((x_sink*)( string )));
    
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
    BLM_DOWN();
}

void x_btml_selftest( void )
{
    // bcore_x_btml.h:604:1
    BLM_INIT_LEVEL(0);
    sr_s zoo;BLM_T_INIT_SPUSH(sr_s, &zoo);; zoo = bcore_spect_via_create_zoo( 1000 );
    
    clock_t time = clock();
    x_btml_t_test_transfer(((const x_btml*)(zoo.o)),sr_s_o_type(&( zoo )) );
    time = clock() - time;
    
    bcore_msg( "x_bbml transfer %5.3fs\n", ( double )time/CLOCKS_PER_SEC );
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
    "feature aware x_bbml : feature_body_from_source;"
    "feature aware x_bbml : feature_body_to_sink;"
"}";

er_t x_bbml_t_from_source( x_bbml* o, tp_t t, x_source* source )
{
    // bcore_x_bbml.h:97:1
    
    sr_s sr = sr_null();
    BLM_TRY(x_bbml_parse_create_object(source, (&(sr)) ))
    ((x_bbml*)(x_inst_t_copy_typed(((x_inst*)( o)),t, sr_s_o_type( (&(sr)) ), sr.o )));
    sr_s_down(&( sr ));
    return  bcore_error_last();
}

x_bbml* x_bbml_create_from_source_t( x_source* source, tp_t* type )
{
    // bcore_x_bbml.h:108:1
    
    sr_s sr = sr_null();
    BLM_TRY_EXIT(x_bbml_parse_create_object(source, (&(sr)) ))
    if( sr.o && type ) (*(type)) = sr_s_o_type(&( sr ));
    return  ((x_bbml*)(sr.o)); // sr.o is NULL in case of error
}

x_bbml* x_bbml_create_from_source( x_source* source )
{
    // bcore_x_bbml.h:118:1
    
    tp_t t = 0;
    x_bbml* o = x_bbml_create_from_source_t(source, (&(t)) );
    if( t )
    {
        ASSERT( x_stamp_t_is_aware(t ) );
    }
    return  o;
}

void x_bbml_t_to_sink( const x_bbml* o, tp_t t, x_sink* sink )
{
    // bcore_x_bbml.h:131:1
    
    x_bbml_t_translate_recursive(o,t, 0, true, sink );
}

bl_t x_bbml_appears_valid( x_source* source )
{
    // bcore_x_bbml.h:148:1
    
    sz_t index = x_source_get_index(source);
    tp_t type = x_bbml_parse_type(source );
    bl_t valid = bcore_flect_exists( type );
    x_source_set_index(source,index );
    return  valid;
}

er_t x_bbml_parse_create_object( x_source* source, sr_s* obj )
{
    // bcore_x_bbml.h:162:1
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

er_t x_bbml_t_parse_body( x_bbml* o, tp_t t, x_source* source )
{
    // bcore_x_bbml.h:194:1
    
    x_stamp* stamp =((x_stamp*)( o));
    if( x_bbml_t_defines_feature_body_from_source(t ) )
    {
        BLM_TRY(x_bbml_t_feature_body_from_source(o,t, source ))
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
        switch( t )
        {
            case TYPEOF_aware_t : x_source_get_data(source,((x_inst*)(o)), sizeof( aware_t ) ); break;
            case TYPEOF_bl_t    : x_source_get_data(source,((x_inst*)(o)), sizeof( bl_t ) ); break;
            case TYPEOF_f2_t    : x_source_get_data(source,((x_inst*)(o)), sizeof( f2_t ) ); break;
            case TYPEOF_f3_t    : x_source_get_data(source,((x_inst*)(o)), sizeof( f3_t ) ); break;
            case TYPEOF_s0_t    : x_source_get_data(source,((x_inst*)(o)), sizeof( s0_t ) ); break;
            case TYPEOF_s1_t    : x_source_get_data(source,((x_inst*)(o)), sizeof( s1_t ) ); break;
            case TYPEOF_s2_t    : x_source_get_data(source,((x_inst*)(o)), sizeof( s2_t ) ); break;
            case TYPEOF_s3_t    : x_source_get_data(source,((x_inst*)(o)), sizeof( s3_t ) ); break;
            case TYPEOF_sc_t    : break; // c-style constant strings are skipped over
            case TYPEOF_sz_t    : x_source_get_data(source,((x_inst*)(o)), sizeof( sz_t ) ); break;
            case TYPEOF_uz_t    : x_source_get_data(source,((x_inst*)(o)), sizeof( uz_t ) ); break;
            case TYPEOF_tp_t    : x_source_get_data(source,((x_inst*)(o)), sizeof( tp_t ) ); break;
            case TYPEOF_u0_t    : x_source_get_data(source,((x_inst*)(o)), sizeof( u0_t ) ); break;
            case TYPEOF_u1_t    : x_source_get_data(source,((x_inst*)(o)), sizeof( u1_t ) ); break;
            case TYPEOF_u2_t    : x_source_get_data(source,((x_inst*)(o)), sizeof( u2_t ) ); break;
            case TYPEOF_u3_t    : x_source_get_data(source,((x_inst*)(o)), sizeof( u3_t ) ); break;
            default: ERR_fa( "Cannot convert type '#<sc_t>' from a binary stream.", bnameof( t ) );
        }
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

void x_bbml_t_translate_recursive( const x_bbml* o, tp_t t, tp_t name, bl_t shelve, x_sink* sink )
{
    // bcore_x_bbml.h:300:1
    
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
        if( x_bbml_t_defines_feature_body_to_sink(t ) )
        {
            x_bbml_t_feature_body_to_sink(o,t, sink );
        }
        else if( t == TYPEOF_st_s ) // strings
        {
            const st_s* string = ((const st_s*)(o));
            x_sink_push_data(sink,( vc_t )string->data, string->size + 1 ); // push string including terminating 0
        }
        else if( x_stamp_t_is_leaf(t ) )
        {
            switch( t )
            {
                case TYPEOF_aware_t : x_sink_push_data(sink,((const x_inst*)(o)), sizeof( aware_t ) ); break;
                case TYPEOF_bl_t    : x_sink_push_data(sink,((const x_inst*)(o)), sizeof( bl_t ) ); break;
                case TYPEOF_f2_t    : x_sink_push_data(sink,((const x_inst*)(o)), sizeof( f2_t ) ); break;
                case TYPEOF_f3_t    : x_sink_push_data(sink,((const x_inst*)(o)), sizeof( f3_t ) ); break;
                case TYPEOF_s0_t    : x_sink_push_data(sink,((const x_inst*)(o)), sizeof( s0_t ) ); break;
                case TYPEOF_s1_t    : x_sink_push_data(sink,((const x_inst*)(o)), sizeof( s1_t ) ); break;
                case TYPEOF_s2_t    : x_sink_push_data(sink,((const x_inst*)(o)), sizeof( s2_t ) ); break;
                case TYPEOF_s3_t    : x_sink_push_data(sink,((const x_inst*)(o)), sizeof( s3_t ) ); break;
                case TYPEOF_sc_t    : break; // c-style constant strings are skipped over
                case TYPEOF_sz_t    : x_sink_push_data(sink,((const x_inst*)(o)), sizeof( sz_t ) ); break;
                case TYPEOF_uz_t    : x_sink_push_data(sink,((const x_inst*)(o)), sizeof( uz_t ) ); break;
                case TYPEOF_tp_t    : x_sink_push_data(sink,((const x_inst*)(o)), sizeof( tp_t ) ); break;
                case TYPEOF_u0_t    : x_sink_push_data(sink,((const x_inst*)(o)), sizeof( u0_t ) ); break;
                case TYPEOF_u1_t    : x_sink_push_data(sink,((const x_inst*)(o)), sizeof( u1_t ) ); break;
                case TYPEOF_u2_t    : x_sink_push_data(sink,((const x_inst*)(o)), sizeof( u2_t ) ); break;
                case TYPEOF_u3_t    : x_sink_push_data(sink,((const x_inst*)(o)), sizeof( u3_t ) ); break;
                default: ERR_fa( "Cannot convert type '#<sc_t>' into a binary stream.", bnameof( t ) );
            }
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

void x_bbml_t_test_transfer( const x_bbml* o, tp_t t )
{
    // bcore_x_bbml.h:387:1
    BLM_INIT_LEVEL(0);
    st_s* string = st_s_create();
    x_bbml_t_to_sink(o,t,((x_sink*)( string )));
    
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
    BLM_DOWN();
}

void x_bbml_selftest( void )
{
    // bcore_x_bbml.h:420:1
    BLM_INIT_LEVEL(0);
    sr_s zoo;BLM_T_INIT_SPUSH(sr_s, &zoo);; zoo = bcore_spect_via_create_zoo( 1000 );
    
    clock_t time = clock();
    x_bbml_t_test_transfer(((const x_bbml*)(zoo.o)),sr_s_o_type(&( zoo )) );
    time = clock() - time;
    
    bcore_msg( "x_bbml transfer %5.3fs\n", ( double )time/CLOCKS_PER_SEC );
    BLM_DOWN();
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
            XOILA_REGISTER_SPECT( bcore_shell_op_default );
            {
               bcore_arr_tp_s* arr = bcore_arr_tp_s_create();
               bcore_arr_tp_s_push( arr, TYPEOF_bcore_shell_op_default_help_s );
               bcore_arr_tp_s_push( arr, TYPEOF_bcore_shell_op_default_ls_s );
               bcore_arr_tp_s_push( arr, TYPEOF_bcore_shell_op_default_enter_s );
               bcore_arr_tp_s_push( arr, TYPEOF_bcore_shell_op_default_exit_s );
               bcore_arr_tp_s_push( arr, TYPEOF_bcore_shell_op_default_get_s );
               bcore_arr_tp_s_push( arr, TYPEOF_bcore_shell_op_default_set_s );
               bcore_xoila_set_arr_traitline_stamps_d( TYPEOF_bcore_shell_op_default, arr );
            }

            // --------------------------------------------------------------------
            // source: bcore_x_btml.h

            // group: x_btml
            BCORE_REGISTER_FEATURE( x_btml_feature_body_from_source );
            BCORE_REGISTER_FEATURE( x_btml_feature_body_to_sink );
            XOILA_REGISTER_SPECT( x_btml );

            // --------------------------------------------------------------------
            // source: bcore_x_bbml.h

            // group: x_bbml
            BCORE_REGISTER_FEATURE( x_bbml_feature_body_from_source );
            BCORE_REGISTER_FEATURE( x_bbml_feature_body_to_sink );
            XOILA_REGISTER_SPECT( x_bbml );
        }
        break;
        default: break;
    }
    return NULL;
}
// XOICO_BODY_SIGNATURE 0xF3547890F779880C
// XOICO_FILE_SIGNATURE 0x236BC7F2030AF565
