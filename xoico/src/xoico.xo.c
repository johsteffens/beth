//  Last update: 2024-12-07T12:31:33Z
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
 *  xoico.x
 *  xoico_host.x
 *  xoico_name.x
 *  xoico_forward.x
 *  xoico_typespec.x
 *  xoico_transient_map.x
 *  xoico_arg.x
 *  xoico_args.x
 *  xoico_signature.x
 *  xoico_body.x
 *  xoico_func.x
 *  xoico_funcs.x
 *  xoico_feature.x
 *  xoico_features.x
 *  xoico_embedded_data.x
 *  xoico_group.x
 *  xoico_wrap.x
 *  xoico_stamp.x
 *  xoico_source.x
 *  xoico_target.x
 *  xoico_compiler.x
 *  xoico_cengine.x
 *  xoico_che.x
 *  xoico_builder.x
 *  xoico_main.x
 *  xoico_che_builtin.x
 *  xoico_che_control.x
 *  xoico_che_function.x
 *  xoico_che_result.x
 *  xoico_che_trans_typespec.x
 *
 */

#include "xoico.xo.h"
#include "bcore_spect.h"
#include "bcore_spect_inst.h"
#include "bcore_sr.h"
#include "bcore_const_manager.h"

// To force a rebuild of this target by xoico, reset the hash key value below to 0.
// HKEYOF_xoico 0xBB28D2937D13AAFBull

/**********************************************************************************************************************/
// source: xoico.x

//----------------------------------------------------------------------------------------------------------------------
// group: xoico

XOILA_DEFINE_SPECT( x_inst, xoico )
"{"
    "bcore_spect_header_s header;"
    "feature aware xoico : convert_transient_types;"
    "feature aware xoico : get_hash;"
    "feature aware xoico : get_global_name_tp;"
    "feature aware xoico : parse;"
    "feature aware xoico : finalize = xoico_finalize_default;"
    "feature aware xoico : expand_setup = xoico_expand_setup_default;"
    "feature aware xoico : expand_forward = xoico_expand_forward_default;"
    "feature aware xoico : expand_indef_typedef = xoico_expand_indef_typedef_default;"
    "feature aware xoico : expand_spect_declaration = xoico_expand_spect_declaration_default;"
    "feature aware xoico : expand_spect_definition = xoico_expand_spect_definition_default;"
    "feature aware xoico : expand_declaration = xoico_expand_declaration_default;"
    "feature aware xoico : expand_indef_declaration = xoico_expand_indef_declaration_default;"
    "feature aware xoico : expand_definition = xoico_expand_definition_default;"
    "feature aware xoico : expand_init1 = xoico_expand_init1_default;"
    "feature aware xoico : expand_down1 = xoico_expand_down1_default;"
    "feature aware xoico : expand_manifesto = xoico_expand_manifesto_default;"
    "feature aware xoico : get_source_point = xoico_get_source_point_default;"
"}";

er_t xoico_parse_f( x_source* source, sc_t format )
{
    // xoico.x:84:1
    
    return  x_source_parse_fa(source,format );
}

er_t xoico_embed_file_open( x_source* parent, sc_t file_name, x_source** include_source )
{
    // xoico.x:92:1
    BLM_INIT_LEVEL(0);
    st_s* folder = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,bcore_file_folder_path(x_source_get_file(parent) )));
    if( folder->size == 0 ) st_s_push_char(folder,'.' );
    
    st_s* path = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
    if( file_name[ 0 ] == '/' )
    {
        st_s_copy_sc(path,file_name );
    }
    else
    {
        st_s_copy_fa(path,"#<sc_t>/#<sc_t>", folder->sc, file_name );
    }
    
    if( !bcore_file_exists(path->sc ) )
    {
        BLM_RETURNV(er_t, x_source_parse_error_fa(parent,"Xoico: File '#<sc_t>' not found.", path->sc ))
    }
    
    (*(include_source)) =((x_source*)( bcore_file_open_source(path->sc )));
    BLM_RETURNV(er_t, 0)
}

er_t xoico_parse_sc( xoico* o, const xoico_host* host, sc_t sc )
{
    // xoico.x:118:1
    BLM_INIT_LEVEL(0);
    BLM_RETURNV(er_t, xoico_a_parse(o,host, ((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_sc(sc ))) ))
}

er_t xoico_parse_fa( xoico* o, const xoico_host* host, sc_t format, ... )
{
    // xoico.x:125:1
    BLM_INIT_LEVEL(0);
    va_list args;
    va_start( args, format );
    er_t ret = xoico_a_parse(o,host, ((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_fv(format, args ))) );
    va_end( args );
    BLM_RETURNV(er_t, ret)
}

/**********************************************************************************************************************/
// source: xoico_host.x

//----------------------------------------------------------------------------------------------------------------------
// group: xoico_host

XOILA_DEFINE_SPECT( x_inst, xoico_host )
"{"
    "bcore_spect_header_s header;"
    "feature aware xoico_host : parse_name_st = xoico_host_parse_name_st_default;"
    "feature aware xoico_host : parse_name_tp = xoico_host_parse_name_tp_default;"
    "feature aware xoico_host : compiler = xoico_host_compiler_default;"
    "feature aware xoico_host : cengine = xoico_host_cengine_default;"
    "feature aware xoico_host : obj_type = xoico_host_obj_type_default;"
    "feature aware xoico_host : create_spect_name = xoico_host_create_spect_name_default;"
    "feature aware xoico_host : transient_map;"
    "feature aware xoico_host : nameof = xoico_host_nameof_default;"
    "feature aware xoico_host : entypeof = xoico_host_entypeof_default;"
"}";

er_t xoico_host_parse_name_tp_default( const xoico_host* o, x_source* source, tp_t* name )
{
    // xoico_host.x:24:1
    BLM_INIT_LEVEL(0);
    st_s* s = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
    BLM_TRY(xoico_host_a_parse_name_st(o,source, s ))
    if( s->size == 0 ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Identifier expected." ))
    tp_t t = xoico_compiler_s_entypeof(xoico_host_a_compiler(o),s->sc );
    if( name ) (*(name)) = t;
    BLM_RETURNV(er_t, 0)
}

sc_t xoico_host_nameof_default( const xoico_host* o, tp_t type )
{
    // xoico_host.x:41:1
    
    return  xoico_compiler_s_nameof(xoico_host_a_compiler(o),type );
}

tp_t xoico_host_entypeof_default( const xoico_host* o, sc_t name )
{
    // xoico_host.x:46:1
    
    return  xoico_compiler_s_entypeof(xoico_host_a_compiler(o),name );
}

/**********************************************************************************************************************/
// source: xoico_name.x

//----------------------------------------------------------------------------------------------------------------------
// group: xoico_name

BCORE_DEFINE_OBJECT_INST_P( xoico_name_s )
"aware xoico_name"
"{"
    "tp_t name;"
    "x_source_point_s source_point;"
    "func xoico:parse;"
    "func xoico:get_hash;"
    "func xoico:get_global_name_tp;"
    "func xoico:expand_declaration;"
    "func xoico:expand_init1;"
"}";

er_t xoico_name_s_parse( xoico_name_s* o, const xoico_host* host, x_source* source )
{
    // xoico_name.x:24:5
    
    x_source_point_s_setup_from_source(&(o->source_point),source );
    BLM_TRY(xoico_host_a_parse_name_tp(host,source, (&(o->name)) ))
    //source.parse_fa( " ; " );
    return  0;
}

tp_t xoico_name_s_get_hash( const xoico_name_s* o )
{
    // xoico_name.x:32:5
    
    tp_t hash = bcore_tp_fold_tp( bcore_tp_init(), o->_ );
    hash = bcore_tp_fold_tp( hash, o->name );
    return  hash;
}

er_t xoico_name_s_expand_declaration( const xoico_name_s* o, const xoico_host* host, sz_t indent, x_sink* sink )
{
    // xoico_name.x:41:5
    
    x_sink_push_fa(sink,"#rn{ }##define TYPEOF_#<sc_t> 0x#pl16'0'{#X<tp_t>}ull\n", indent, xoico_compiler_s_nameof(xoico_host_a_compiler(host),o->name ), o->name );
    return  0;
}

er_t xoico_name_s_expand_init1( const xoico_name_s* o, const xoico_host* host, sz_t indent, x_sink* sink )
{
    // xoico_name.x:47:5
    
    x_sink_push_fa(sink,"#rn{ }BCORE_REGISTER_NAME( #<sc_t> );\n", indent, xoico_compiler_s_nameof(xoico_host_a_compiler(host),o->name ) );
    return  0;
}

XOILA_DEFINE_SPECT( xoico, xoico_name )
"{"
    "bcore_spect_header_s header;"
"}";

/**********************************************************************************************************************/
// source: xoico_forward.x

//----------------------------------------------------------------------------------------------------------------------
// group: xoico_forward

BCORE_DEFINE_OBJECT_INST_P( xoico_forward_s )
"aware xoico_forward"
"{"
    "tp_t name;"
    "hidden aware xoico_group_s* group;"
    "x_source_point_s source_point;"
    "func xoico:parse;"
    "func xoico:get_hash;"
    "func xoico:get_global_name_tp;"
    "func xoico:expand_declaration;"
    "func xoico:expand_forward;"
    "func xoico:expand_init1;"
    "func xoico:get_source_point;"
"}";

er_t xoico_forward_s_parse( xoico_forward_s* o, const xoico_host* host, x_source* source )
{
    // xoico_forward.x:27:5
    
    x_source_point_s_setup_from_source(&(o->source_point),source );
    BLM_TRY(xoico_host_a_parse_name_tp(host,source, (&(o->name)) ))
    //source.parse_fa( " ; " );
    return  0;
}

tp_t xoico_forward_s_get_hash( const xoico_forward_s* o )
{
    // xoico_forward.x:35:5
    
    tp_t hash = bcore_tp_fold_tp( bcore_tp_init(), o->_ );
    hash = bcore_tp_fold_tp( hash, o->name );
    return  hash;
}

er_t xoico_forward_s_expand_declaration( const xoico_forward_s* o, const xoico_host* host, sz_t indent, x_sink* sink )
{
    // xoico_forward.x:44:5
    
    x_sink_push_fa(sink,"#rn{ }##define TYPEOF_#<sc_t> 0x#pl16'0'{#X<tp_t>}ull\n", indent, xoico_host_a_nameof(host,o->name ), o->name );
    return  0;
}

er_t xoico_forward_s_expand_forward( const xoico_forward_s* o, const xoico_host* host, sz_t indent, x_sink* sink )
{
    // xoico_forward.x:50:5
    
    x_sink_push_fa(sink," \\\n#rn{ }BCORE_FORWARD_OBJECT( #<sc_t> );", indent, xoico_host_a_nameof(host,o->name ) );
    return  0;
}

XOILA_DEFINE_SPECT( xoico, xoico_forward )
"{"
    "bcore_spect_header_s header;"
"}";

/**********************************************************************************************************************/
// source: xoico_typespec.x

//----------------------------------------------------------------------------------------------------------------------
// group: xoico_typespec

BCORE_DEFINE_OBJECT_INST_P( xoico_typespec_transient_s )
"aware xoico_typespec"
"{"
    "tp_t class;"
    "tp_t cast_to_var;"
    "func xoico:get_hash;"
"}";

tp_t xoico_typespec_transient_s_get_hash( const xoico_typespec_transient_s* o )
{
    // xoico_typespec.x:44:5
    
    tp_t hash = bcore_tp_fold_tp( bcore_tp_init(), o->_ );
    hash = bcore_tp_fold_tp( hash, o->class );
    return  hash;
}

BCORE_DEFINE_OBJECT_INST_P( xoico_typespec_s )
"aware xoico_typespec"
"{"
    "tp_t type;"
    "tp_t access_class;"
    "xoico_typespec_transient_s => transient;"
    "sz_t indirection;"
    "bl_t flag_static;"
    "bl_t flag_volatile;"
    "bl_t flag_restrict;"
    "bl_t flag_obliv;"
    "bl_t flag_aware;"
    "bl_t flag_scope;"
    "bl_t flag_addressable = true;"
    "bl_t flag_variadic;"
    "func xoico:convert_transient_types;"
    "func xoico:get_hash;"
"}";

er_t xoico_typespec_s_relent( xoico_typespec_s* o, const xoico_host* host, tp_t tp_obj_type )
{
    // xoico_typespec.x:72:5
    
    if( o->type == ((tp_t)(TYPEOF_type_object)) ) o->type = tp_obj_type;
    return  0;
}

er_t xoico_typespec_s_convert_transient_types( xoico_typespec_s* o, const xoico_host* host, const xoico_transient_map_s* map )
{
    // xoico_typespec.x:78:5
    
    if( o->transient )
    {
        tp_t type = xoico_transient_map_s_get(map,o->transient->class );
        if( type ) o->type = type;
    }
    return  0;
}

void xoico_typespec_s_reset( xoico_typespec_s* o )
{
    // xoico_typespec.x:93:5
    
    o->access_class = 0;
    xoico_typespec_transient_s_attach( &(o->transient ),  NULL);
    o->type = 0;
    o->indirection = 0;
    o->flag_static   = false;
    o->flag_volatile = false;
    o->flag_restrict = false;
    o->flag_aware    = false;
    o->flag_obliv    = false;
    o->flag_scope    = false;
    o->flag_addressable = true;  // object can have a pointer ('false' for objects returned by a function)
}

er_t xoico_typespec_s_parse( xoico_typespec_s* o, const xoico_host* host, x_source* source )
{
    // xoico_typespec.x:117:1
    BLM_INIT_LEVEL(0);
    xoico_typespec_s_reset(o);
    xoico_compiler_s* compiler = xoico_host_a_compiler(host);
    
    if( x_source_parse_bl(source,"#?'...' " ) )
    {
        o->flag_variadic = true;
        BLM_RETURNV(er_t, 0)
    }
    
    tp_t access_class = 0;
    
    if(      x_source_parse_bl(source," #?w'c'" ) || x_source_parse_bl(source," #?w'const'"       ) ) access_class = TYPEOF_const;
    else if( x_source_parse_bl(source," #?w'm'" ) || x_source_parse_bl(source," #?w'mutable'"     ) ) access_class = TYPEOF_mutable;
    else if( x_source_parse_bl(source," #?w'd'" ) || x_source_parse_bl(source," #?w'discardable'" ) ) access_class = TYPEOF_discardable;
    
    while( !x_source_eos(source) )
    {
        if     ( x_source_parse_bl(source," #?w'static'"   ) && !o->flag_static ) o->flag_static = true;
        else if( x_source_parse_bl(source," #?w'volatile'" ) && !o->flag_volatile ) o->flag_volatile = true;
        else if( x_source_parse_bl(source," #?w'obliv'"    ) && !o->flag_obliv ) o->flag_obliv = true;
        else if( x_source_parse_bl(source," #?w'aware'"    ) && !o->flag_aware ) o->flag_aware = true;
        else break;
    }
    
    BLM_TRY(x_source_parse_fa(source," " ))
    
    if( x_source_parse_bl(source,"#?'(' " ) )
    {
        st_s* s = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
        BLM_TRY(x_source_parse_fa(source,"#name ", s ))
        if( s->size == 0 ) BLM_TRY(x_source_parse_error_fa(source,"Transient class: Identifier expected." ))
        BCORE_PASS_CREATE(xoico_typespec_transient_s,o->transient)->class = xoico_compiler_s_entypeof(compiler,s->sc );
        BLM_TRY(x_source_parse_fa(source," ) " ))
    }
    
    if( x_source_parse_bl(source,"#=?':'" ) )
    {
        BLM_TRY(xoico_host_a_parse_name_tp(host,source, (&(o->type)) ))
        BLM_TRY(x_source_parse_fa(source," " ))
    }
    else if( x_source_parse_bl(source,"#?'@' " ) )
    {
        o->type = ((tp_t)(TYPEOF_type_object));
    }
    else if( x_source_parse_bl(source,"#?'$' " ) )
    {
        o->type = ((tp_t)(TYPEOF_type_deduce));
    }
    else
    {
        st_s* s = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
        BLM_TRY(x_source_parse_fa(source,"#name ", s ))
        if( s->size == 0 ) BLM_TRY(x_source_parse_error_fa(source,"Argument: Type expected." ))
        o->type = xoico_compiler_s_entypeof( compiler,s->sc );
    }
    
    if( x_source_parse_bl(source,"#?'.' " ) )
    {
        if( !x_source_parse_bl(source,"#?([0]>='0'&&[0]<='9') " ) ) BLM_TRY(x_source_parse_error_fa(source,"Argument: Indirection literal expected." ))
        sz_t indirection = 0;
        BLM_TRY(x_source_parse_fa(source,"#<sz_t*> ", (&(indirection)) ))
        o->indirection = indirection;
    }
    else
    {
        while( x_source_parse_bl(source,"#?'*' " ) ) o->indirection++;
    }
    
    if( x_source_parse_bl(source," #?'restrict' " ) ) o->flag_restrict = true;
    
    if( o->indirection > 0 && access_class == 0 )
    {
        access_class = TYPEOF_const;
    }
    
    o->access_class = access_class;
    BLM_RETURNV(er_t, 0)
}

tp_t xoico_typespec_s_get_hash( const xoico_typespec_s* o )
{
    // xoico_typespec.x:200:1
    
    tp_t hash = bcore_tp_fold_tp( bcore_tp_init(), o->_ );
    hash = bcore_tp_fold_tp( hash, o->type );
    hash = bcore_tp_fold_tp( hash, o->access_class );
    hash = bcore_tp_fold_tp( hash, o->transient ? xoico_typespec_transient_s_get_hash(o->transient) : 0 );
    hash = bcore_tp_fold_bl( hash, o->flag_static );
    hash = bcore_tp_fold_bl( hash, o->flag_volatile );
    hash = bcore_tp_fold_bl( hash, o->flag_restrict );
    hash = bcore_tp_fold_bl( hash, o->flag_scope );
    hash = bcore_tp_fold_u3( hash, o->indirection );
    return  hash;
}

er_t xoico_typespec_s_expand( const xoico_typespec_s* o, const xoico_host* host, x_sink* sink )
{
    // xoico_typespec.x:216:1
    BLM_INIT_LEVEL(0);
    if( o->flag_variadic )
    {
        x_sink_push_fa(sink,"..." );
        BLM_RETURNV(er_t, 0)
    }
    
    xoico_compiler_s* compiler = xoico_host_a_compiler(host);
    
    tp_t type = o->type;
    
    if( type == ((tp_t)(TYPEOF_type_object)) )
    {
        //if( !sc_obj_type ) ERR_fa( "Cannot resolve 'type_object' at this point." );
        type = xoico_host_a_obj_type(host); //compiler.entypeof( sc_obj_type );
    }
    else if( type == ((tp_t)(TYPEOF_type_deduce)) )
    {
        ERR_fa( "Cannot resolve 'type_deduce' at this point." );
    }
    
    st_s* st_type = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create_sc(xoico_compiler_s_nameof(compiler,type ) )));
    
    sc_t sc_type = st_type->sc;
    if( o->flag_static   ) x_sink_push_fa(sink,"static " );
    if( o->access_class == TYPEOF_const ) x_sink_push_fa(sink,"const " );
    if( o->flag_volatile ) x_sink_push_fa(sink,"volatile " );
    x_sink_push_fa(sink,"#<sc_t>", sc_type );
    
    for(sz_t i = 0; i < o->indirection; i++ ) x_sink_push_fa(sink,"*" );
    if( o->flag_restrict ) x_sink_push_fa(sink," restrict " );
    
    BLM_RETURNV(er_t, 0)
}

er_t xoico_typespec_s_expand_x( const xoico_typespec_s* o, const xoico_host* host, x_sink* sink )
{
    // xoico_typespec.x:254:1
    BLM_INIT_LEVEL(0);
    if( o->flag_variadic )
    {
        x_sink_push_fa(sink,"..." );
        BLM_RETURNV(er_t, 0)
    }
    
    xoico_compiler_s* compiler = xoico_host_a_compiler(host);
    
    tp_t type = o->type;
    
    if( type == ((tp_t)(TYPEOF_type_object)) )
    {
        type = xoico_host_a_obj_type(host); //compiler.entypeof( sc_obj_type );
    }
    else if( type == ((tp_t)(TYPEOF_type_deduce)) )
    {
        ERR_fa( "Cannot resolve 'type_deduce' at this point." );
    }
    
    st_s* st_type = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create_sc(xoico_compiler_s_nameof(compiler,type ) )));
    
    switch( o->access_class )
    {
        case TYPEOF_const:       x_sink_push_fa(sink,"c " ); break;
        case TYPEOF_mutable:     x_sink_push_fa(sink,"m " ); break;
        case TYPEOF_discardable: x_sink_push_fa(sink,"d " ); break;
        default: break;
    }
    
    if( o->flag_static   ) x_sink_push_fa(sink,"static " );
    if( o->flag_volatile ) x_sink_push_fa(sink,"volatile " );
    sc_t sc_type = st_type->sc;
    
    if( o->transient )
    {
        x_sink_push_fa(sink,"(#<sc_t>)", xoico_host_a_nameof(host,o->transient->class ) );
    }
    
    x_sink_push_fa(sink,"#<sc_t>", sc_type );
    
    if( o->indirection > 0 ) x_sink_push_fa(sink,".#<sz_t>", o->indirection );
    if( o->flag_restrict ) x_sink_push_fa(sink," restrict " );
    
    BLM_RETURNV(er_t, 0)
}

bl_t xoico_typespec_s_converts_to( const xoico_typespec_s* o, const xoico_typespec_s* b )
{
    // xoico_typespec.x:336:1
    
    if( o->type == b->type )
    {
        if( o->indirection == b->indirection )
        {
            if( o->access_class == TYPEOF_const )
            {
                return  ( b->access_class == TYPEOF_const );
            }
            else
            {
                return  true;
            }
        }
        else
        {
            return  false;
        }
    }
    else
    {
        if( o->indirection == 0 && b->indirection == 0 )
        {
            if( xoico_typespec_is_numeric(o->type ) && xoico_typespec_is_numeric(b->type ) )
            {
                return  true;
            }
            else if( o->type == ((tp_t)(TYPEOF_vd_t)) && b->type == ((tp_t)(TYPEOF_vc_t)) )
            {
                return  true;
            }
            else if( o->type == ((tp_t)(TYPEOF_sd_t)) && b->type == ((tp_t)(TYPEOF_sc_t)) )
            {
                return  true;
            }
            else
            {
                return  false;
            }
        }
        else if( b->type == ((tp_t)(TYPEOF_vc_t)) && b->indirection == 0 )
        {
            if( o->indirection == 1 )
            {
                return  true;
            }
            else
            {
                return  false;
            }
        }
        else if( b->type == ((tp_t)(TYPEOF_vd_t)) && b->indirection == 0 )
        {
            if( o->indirection == 1 && ( o->access_class != TYPEOF_const ) )
            {
                return  true;
            }
            else
            {
                return  false;
            }
        }
        else if( b->type == ((tp_t)(TYPEOF_bl_t)) && b->indirection == 0 )
        {
            if( o->indirection > 0 )
            {
                return  true;
            }
            else if( o->type == ((tp_t)(TYPEOF_vd_t)) || o->type == ((tp_t)(TYPEOF_vc_t)) || o->type == ((tp_t)(TYPEOF_sc_t)) || o->type == ((tp_t)(TYPEOF_sd_t)) )
            {
                return  ( o->indirection == 0 );
            }
            else
            {
                return  false;
            }
        }
        else
        {
            return  false;
        }
    }
}

bl_t xoico_typespec_s_is_ptr_type( const xoico_typespec_s* o )
{
    // xoico_typespec.x:423:1
    
    return  o->type == TYPEOF_vd_t ||
           o->type == TYPEOF_vc_t ||
           o->type == TYPEOF_sc_t ||
           o->type == TYPEOF_sd_t;
}

XOILA_DEFINE_SPECT( xoico, xoico_typespec )
"{"
    "bcore_spect_header_s header;"
"}";

bl_t xoico_typespec_is_numeric( tp_t type )
{
    // xoico_typespec.x:304:1
    
    switch( type )
    {
        case ((tp_t)(TYPEOF_u0_t)): return  true;
        case ((tp_t)(TYPEOF_u1_t)): return  true;
        case ((tp_t)(TYPEOF_u2_t)): return  true;
        case ((tp_t)(TYPEOF_u3_t)): return  true;
        case ((tp_t)(TYPEOF_s0_t)): return  true;
        case ((tp_t)(TYPEOF_s1_t)): return  true;
        case ((tp_t)(TYPEOF_s2_t)): return  true;
        case ((tp_t)(TYPEOF_s3_t)): return  true;
        case ((tp_t)(TYPEOF_f2_t)): return  true;
        case ((tp_t)(TYPEOF_f3_t)): return  true;
        case ((tp_t)(TYPEOF_sz_t)): return  true;
        case ((tp_t)(TYPEOF_uz_t)): return  true;
        case ((tp_t)(TYPEOF_tp_t)): return  true;
        case ((tp_t)(TYPEOF_aware_t)): return  true;
        case ((tp_t)(TYPEOF_er_t)): return  true;
        case ((tp_t)(TYPEOF_bl_t)): return  true;
        case ((tp_t)(TYPEOF_char)): return  true;
        case ((tp_t)(TYPEOF_int)):    return  true;
        case ((tp_t)(TYPEOF_long)):   return  true;
        case ((tp_t)(TYPEOF_float)):  return  true;
        case ((tp_t)(TYPEOF_double)): return  true;
        default: break;
    }
    return  false;
}

/**********************************************************************************************************************/
// source: xoico_transient_map.x

//----------------------------------------------------------------------------------------------------------------------
// group: xoico_transient_map

BCORE_DEFINE_OBJECT_INST_P( xoico_transient_map_s )
"aware xoico_transient_map"
"{"
    "bcore_hmap_tptp_s map;"
    "func xoico:parse;"
"}";

bcore_arr_tp_s* xoico_transient_map_s_create_key_arr( const xoico_transient_map_s* o )
{
    // xoico_transient_map.x:33:5
    
    bcore_arr_tp_s* arr_tp = bcore_arr_tp_s_create();
    sz_t size = bcore_hmap_tptp_s_size(&(o->map));
    for(sz_t i = 0; i < size; i++ ) if( bcore_hmap_tptp_s_idx_key(&(o->map),i ) ) bcore_arr_tp_s_push(arr_tp,bcore_hmap_tptp_s_idx_key(&(o->map),i ) );
    return  arr_tp;
}

xoico_transient_map_s* xoico_transient_map_s_update( xoico_transient_map_s* o, const xoico_transient_map_s* src )
{
    // xoico_transient_map.x:44:1
    
    for(sz_t i = 0; i < bcore_hmap_tptp_s_size(&(src->map)); i++ )
    {
        tp_t key = bcore_hmap_tptp_s_idx_key(&(src->map),i );
        if( key ) bcore_hmap_tptp_s_set(&(o->map),key, bcore_hmap_tptp_s_idx_val(&(src->map),i ) );
    }
    return o;
}

er_t xoico_transient_map_s_parse_update( xoico_transient_map_s* o, const xoico_host* host, x_source* source )
{
    // xoico_transient_map.x:56:1
    BLM_INIT_LEVEL(0);
    xoico_compiler_s* compiler = xoico_host_a_compiler(host);
    BLM_TRY(x_source_parse_fa(source,"( " ))
    st_s* s = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
    
    while( !x_source_eos(source) )
    {
        BLM_TRY(x_source_parse_fa(source,"#name ", s ))
        if( s->size == 0 ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Transient class name expected." ))
        tp_t key = xoico_compiler_s_entypeof(compiler,s->sc );
        tp_t type = 0;
        BLM_TRY(xoico_host_a_parse_name_tp(host,source, (&(type)) ))
        if( !type ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Type name expected." ))
        bcore_hmap_tptp_s_set(&(o->map),key, type );
        if( x_source_parse_bl(source," #?')' " ) ) break;
        BLM_TRY(x_source_parse_fa(source," , " ))
    }
    
    BLM_RETURNV(er_t, 0)
}

er_t xoico_transient_map_s_parse( xoico_transient_map_s* o, const xoico_host* host, x_source* source )
{
    // xoico_transient_map.x:80:1
    
    bcore_hmap_tptp_s_clear(&(o->map));
    return  xoico_transient_map_s_parse_update(o,host, source );
}

XOILA_DEFINE_SPECT( xoico, xoico_transient_map )
"{"
    "bcore_spect_header_s header;"
"}";

/**********************************************************************************************************************/
// source: xoico_arg.x

//----------------------------------------------------------------------------------------------------------------------
// group: xoico_arg

BCORE_DEFINE_OBJECT_INST_P( xoico_arg_s )
"aware xoico_arg"
"{"
    "x_source_point_s source_point;"
    "xoico_typespec_s typespec;"
    "tp_t name;"
    "func xoico:get_source_point;"
    "func xoico:parse;"
    "func xoico:get_hash;"
    "func xoico:convert_transient_types;"
"}";

er_t xoico_arg_s_parse( xoico_arg_s* o, const xoico_host* host, x_source* source )
{
    // xoico_arg.x:33:1
    BLM_INIT_LEVEL(0);
    xoico_compiler_s* compiler = xoico_host_a_compiler(host);
    x_source_point_s_setup_from_source(&(o->source_point),source );
    BLM_TRY(xoico_typespec_s_parse(&(o->typespec),host, source ))
    
    if( o->typespec.type == ((tp_t)(TYPEOF_type_deduce)) )
    {
        BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(&(o->source_point),"'$' (type_deduce) can not be used in an argument." ))
    }
    
    if( o->typespec.flag_variadic ) BLM_RETURNV(er_t, 0)
    
    if( o->typespec.type == ((tp_t)(TYPEOF_void)) && o->typespec.indirection == 0 )
    {
        BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(&(o->source_point),"'void' is misplaced here." ))
    }
    
    st_s* s = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
    BLM_TRY(x_source_parse_fa(source,"#name ", s ))
    if( s->size == 0 )
    {
        BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Argument: Name expected." ))
    }
    o->name = xoico_compiler_s_entypeof(compiler,s->sc );
    
    BLM_RETURNV(er_t, 0)
}

tp_t xoico_arg_s_get_hash( const xoico_arg_s* o )
{
    // xoico_arg.x:64:1
    
    tp_t hash = bcore_tp_fold_tp( bcore_tp_init(), o->_ );
    hash = bcore_tp_fold_tp( hash, xoico_typespec_s_get_hash(&(o->typespec)) );
    hash = bcore_tp_fold_tp( hash, o->name );
    return  hash;
}

er_t xoico_arg_s_relent( xoico_arg_s* o, const xoico_host* host, tp_t tp_obj_type )
{
    // xoico_arg.x:74:1
    
    return  xoico_typespec_s_relent(&(o->typespec),host, tp_obj_type );
}

er_t xoico_arg_s_expand( const xoico_arg_s* o, const xoico_host* host, x_sink* sink )
{
    // xoico_arg.x:86:1
    
    BLM_TRY(xoico_typespec_s_expand(&(o->typespec),host, sink ) )
    if( o->name )
    {
        x_sink_push_fa(sink," " );
        BLM_TRY(xoico_arg_s_expand_name(o,host, sink ))
    }
    return  0;
}

er_t xoico_arg_s_expand_x( const xoico_arg_s* o, const xoico_host* host, x_sink* sink )
{
    // xoico_arg.x:99:1
    
    BLM_TRY(xoico_typespec_s_expand_x(&(o->typespec),host, sink ) )
    if( o->name )
    {
        x_sink_push_fa(sink," " );
        BLM_TRY(xoico_arg_s_expand_name(o,host, sink ))
    }
    return  0;
}

er_t xoico_arg_s_expand_name( const xoico_arg_s* o, const xoico_host* host, x_sink* sink )
{
    // xoico_arg.x:112:1
    
    if( !o->name ) return  0;
    x_sink_push_fa(sink,"#<sc_t>", xoico_compiler_s_nameof(xoico_host_a_compiler(host),o->name ) );
    return  0;
}

er_t xoico_arg_s_to_self_item_st( const xoico_arg_s* o, const xoico_host* host, st_s* st )
{
    // xoico_arg.x:121:1
    
    st_s_clear(st);
    if( o->typespec.indirection > 1 ) return  x_source_point_s_parse_error_fa(&(o->source_point),"Indirection > 1 is not allowed as functor argument." );
    if( o->typespec.transient       ) return  x_source_point_s_parse_error_fa(&(o->source_point),"Transient type is not allowed as functor argument." );
    
    if( o->typespec.indirection == 1 && o->typespec.access_class == TYPEOF_mutable )
    {
        st_s_push_fa(st,"hidden " );
    }
    
    if( o->typespec.flag_aware )
    {
        st_s_push_fa(st,"aware " );
    }
    else if( o->typespec.flag_obliv )
    {
        st_s_push_fa(st,"obliv " );
    }
    
    st_s_push_fa(st,"#<sc_t> ", xoico_host_a_nameof(host,o->typespec.type ) );
    
    if( o->typespec.indirection == 1 )
    {
        switch( o->typespec.access_class )
        {
            case TYPEOF_discardable: st_s_push_fa(st,"=> " ); break;
            case TYPEOF_mutable:     st_s_push_fa(st,"* "  ); break;
            case TYPEOF_const:       st_s_push_fa(st,"=> " ); break;
            default: return  x_source_point_s_parse_error_fa(&(o->source_point),"Functor argument: Unhandled access class." ); break;
        }
    }
    
    st_s_push_fa(st,"#<sc_t>", xoico_host_a_nameof(host,o->name ) );
    st_s_push_fa(st,";" );
    
    return  0;
}

XOILA_DEFINE_SPECT( xoico, xoico_arg )
"{"
    "bcore_spect_header_s header;"
"}";

/**********************************************************************************************************************/
// source: xoico_args.x

//----------------------------------------------------------------------------------------------------------------------
// group: xoico_args

BCORE_DEFINE_OBJECT_INST_P( xoico_args_s )
"aware x_array"
"{"
    "xoico_arg_s => [];"
    "func xoico:parse;"
    "func xoico:convert_transient_types;"
    "func xoico:get_hash;"
"}";

er_t xoico_args_s_append( xoico_args_s* o, const xoico_host* host, x_source* source )
{
    // xoico_args.x:33:1
    BLM_INIT_LEVEL(0);
    bl_t first = true;
    while( !x_source_parse_bl(source," #=?')' " ) ) // args follow
    {
        if( xoico_args_s_is_variadic(o) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Cannot append to variadic argument list." ))
        if( !first ) BLM_TRY(xoico_parse_f(source, " , " ))
        xoico_arg_s* arg = ((xoico_arg_s*)BLM_LEVEL_T_PUSH(0,xoico_arg_s,xoico_arg_s_create()));
        BLM_TRY(xoico_arg_s_parse(arg,host, source ))
        x_array_push_d(((x_array*)(o)),((x_inst*)(((xoico_arg_s*)bcore_fork(arg)) )));
        first = false;
    }
    BLM_RETURNV(er_t, 0)
}

er_t xoico_args_s_relent( xoico_args_s* o, const xoico_host* host, tp_t tp_obj_type )
{
    // xoico_args.x:50:1
    
    {const xoico_args_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_arg_s* arg=__a->data[__i]; BLM_TRY(xoico_arg_s_relent(arg,host, tp_obj_type ))
    }}return  0;
}

er_t xoico_args_s_convert_transient_types( xoico_args_s* o, const xoico_host* host, const xoico_transient_map_s* map )
{
    // xoico_args.x:58:1
    
    {const xoico_args_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_arg_s* arg=__a->data[__i]; BLM_TRY(xoico_arg_s_convert_transient_types(arg,host, map ))
    }}return   0;
}

er_t xoico_args_s_expand( const xoico_args_s* o, const xoico_host* host, bl_t first, x_sink* sink )
{
    // xoico_args.x:66:1
    
    {const xoico_args_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_arg_s* arg=__a->data[__i];
    {
        if( !first ) x_sink_push_fa(sink,", " );
        first = false;
        BLM_TRY(xoico_arg_s_expand(arg,host, sink ))
    }
    }}return  0;
}

er_t xoico_args_s_expand_x( const xoico_args_s* o, const xoico_host* host, bl_t first, x_sink* sink )
{
    // xoico_args.x:79:1
    
    {const xoico_args_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_arg_s* arg=__a->data[__i];
    {
        if( !first ) x_sink_push_fa(sink,", " );
        first = false;
        BLM_TRY(xoico_arg_s_expand_x(arg,host, sink ))
    }
    }}return  0;
}

er_t xoico_args_s_expand_name( const xoico_args_s* o, const xoico_host* host, bl_t first, x_sink* sink )
{
    // xoico_args.x:92:1
    
    {const xoico_args_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_arg_s* arg=__a->data[__i];
    {
        if( !first ) x_sink_push_fa(sink,", " );
        first = false;
        BLM_TRY(xoico_arg_s_expand_name(arg,host, sink ))
    }
    }}return  0;
}

tp_t xoico_args_s_get_hash( const xoico_args_s* o )
{
    // xoico_args.x:105:1
    
    tp_t hash = bcore_tp_fold_tp( bcore_tp_init(), o->_ );
    {const xoico_args_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_arg_s* arg=__a->data[__i]; hash = bcore_tp_fold_tp( hash, xoico_arg_s_get_hash(arg) );}}
    return  hash;
}

const xoico_arg_s* xoico_args_s_get_arg_by_name( const xoico_args_s* o, tp_t name )
{
    // xoico_args.x:114:1
    
    {const xoico_args_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){const xoico_arg_s* arg=__a->data[__i]; if( arg->name == name ) return  arg;
    }}return  NULL;
}

XOILA_DEFINE_SPECT( xoico, xoico_args )
"{"
    "bcore_spect_header_s header;"
"}";

/**********************************************************************************************************************/
// source: xoico_signature.x

//----------------------------------------------------------------------------------------------------------------------
// group: xoico_signature

BCORE_DEFINE_OBJECT_INST_P( xoico_signature_s )
"aware xoico_signature"
"{"
    "tp_t name;"
    "tp_t base_name;"
    "tp_t global_name;"
    "xoico_typespec_s typespec_ret;"
    "xoico_arg_s => arg_o;"
    "xoico_args_s args;"
    "xoico_arg_s -> direct_return_arg;"
    "x_source_point_s source_point;"
    "func xoico:convert_transient_types;"
    "func xoico:get_global_name_tp;"
    "func xoico:get_source_point;"
    "func xoico:get_hash;"
    "func xoico:parse;"
"}";

er_t xoico_signature_s_convert_transient_types( xoico_signature_s* o, const xoico_host* host, const xoico_transient_map_s* map )
{
    // xoico_signature.x:45:5
    
    if( o->arg_o )
    {
        BLM_TRY(xoico_arg_s_convert_transient_types(o->arg_o,host, map ))
        if
        (
            o->arg_o->typespec.transient &&
            o->typespec_ret.transient &&
            o->arg_o->typespec.transient->class == o->typespec_ret.transient->class
        )
        {
            o->typespec_ret.type = xoico_host_a_obj_type(host);
        }
    }
    
    BLM_TRY(xoico_args_s_convert_transient_types(&(o->args),host, map ))
    BLM_TRY(xoico_typespec_s_convert_transient_types(&(o->typespec_ret),host, map ))
    return   0;
}

er_t xoico_signature_s_relent( xoico_signature_s* o, const xoico_host* host, tp_t tp_obj_type )
{
    // xoico_signature.x:68:5
    
    if( o->arg_o ) BLM_TRY(xoico_arg_s_relent(o->arg_o,host, tp_obj_type ))
    BLM_TRY(xoico_args_s_relent(&(o->args),host, tp_obj_type ))
    BLM_TRY(xoico_typespec_s_relent(&(o->typespec_ret),host, tp_obj_type ))
    return  0;
}

er_t xoico_signature_s_expand_ret( const xoico_signature_s* o, const xoico_host* host, x_sink* sink )
{
    // xoico_signature.x:86:5
    
    BLM_TRY(xoico_typespec_s_expand(&(o->typespec_ret),host, sink ))
    return  0;
}

er_t xoico_signature_s_expand_ret_x( const xoico_signature_s* o, const xoico_host* host, x_sink* sink )
{
    // xoico_signature.x:92:5
    
    if( o->typespec_ret.transient && o->typespec_ret.transient->cast_to_var )
    {
        tp_t var_name = o->typespec_ret.transient->cast_to_var;
        x_sink_push_fa(sink,"#<sc_t> ", xoico_host_a_nameof(host,var_name ) );
    }
    else
    {
        BLM_TRY(xoico_typespec_s_expand_x(&(o->typespec_ret),host, sink ))
    }
    return  0;
}

const xoico_arg_s* xoico_signature_s_get_arg_by_name( const xoico_signature_s* o, tp_t name )
{
    // xoico_signature.x:106:5
    
    return  ( o->arg_o && o->arg_o->name == name ) ? o->arg_o : xoico_args_s_get_arg_by_name(&(o->args),name );
}

tp_t xoico_signature_s_get_hash( const xoico_signature_s* o )
{
    // xoico_signature.x:120:1
    
    tp_t hash = bcore_tp_fold_tp( bcore_tp_init(), o->_ );
    hash = bcore_tp_fold_tp( hash, o->global_name );
    hash = bcore_tp_fold_tp( hash, xoico_typespec_s_get_hash(&(o->typespec_ret)) );
    hash = bcore_tp_fold_tp( hash, o->arg_o ? xoico_arg_s_get_hash(o->arg_o) : 0 );
    hash = bcore_tp_fold_tp( hash, xoico_args_s_get_hash(&(o->args)) );
    return  hash;
}

er_t xoico_signature_s_set_global_name( xoico_signature_s* o, const xoico_host* host )
{
    // xoico_signature.x:132:1
    BLM_INIT_LEVEL(0);
    o->base_name = xoico_host_a_obj_type(host);
    o->global_name = xoico_host_a_entypeof(host,((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create_fa("#<sc_t>_#<sc_t>", xoico_host_a_nameof(host,o->base_name ), xoico_host_a_nameof(host,o->name ) )))->sc );
    BLM_RETURNV(er_t, 0)
}

er_t xoico_signature_s_parse( xoico_signature_s* o, const xoico_host* host, x_source* source )
{
    // xoico_signature.x:141:1
    BLM_INIT_LEVEL(0);
    xoico_compiler_s* compiler = xoico_host_a_compiler(host);
    st_s* name_buf = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
    
    if( x_source_parse_bl(source," #?'extending'" ) )
    {
        tp_t tp_name = 0;
        BLM_TRY(xoico_host_a_parse_name_tp(host,source, (&(tp_name)) ))
        const xoico_signature_s* signature = xoico_compiler_s_get_signature(compiler,tp_name );
        if( !signature ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Could not find predefined signature '#<sc_t>'.", xoico_host_a_nameof(host,tp_name ) ))
        xoico_signature_s_copy(o,signature );
        x_source_point_s_setup_from_source(&(o->source_point),source );
        BLM_TRY(x_source_parse_fa(source," #name", name_buf ))  // empty name is allowed
        o->name = xoico_compiler_s_entypeof(compiler,name_buf->sc );
        BLM_TRY(x_source_parse_fa(source," (" ))
        BLM_TRY(xoico_args_s_append(&(o->args),host, source ))
        BLM_TRY(x_source_parse_fa(source," )" ))
    }
    else
    {
        x_source_point_s_setup_from_source(&(o->source_point),source );
        BLM_TRY(xoico_typespec_s_parse(&(o->typespec_ret),host, source ))
        o->typespec_ret.flag_addressable = false;
    
        // get name
        BLM_TRY(x_source_parse_fa(source," #name", name_buf ))
        o->name = xoico_compiler_s_entypeof(compiler,name_buf->sc );
    
        BLM_TRY(x_source_parse_fa(source," (" ))
    
        if( !x_source_parse_bl(source," #=?')'" ) )
        {BLM_INIT_LEVEL(2);
            xoico_arg_s* arg = ((xoico_arg_s*)BLM_LEVEL_T_PUSH(2,xoico_arg_s,xoico_arg_s_create()));
            BLM_TRY(xoico_arg_s_parse(arg,host, source ))
            if( !x_source_parse_bl(source," #=?')'" ) ) BLM_TRY(x_source_parse_fa(source,", " ))
            if( arg->name == TYPEOF_o )
            {
                xoico_arg_s_attach( &(o->arg_o ),  ((xoico_arg_s*)bcore_fork(arg)));
                BLM_TRY(xoico_args_s_parse(&(o->args),host, source ))
            }
            else
            {
                x_array_push_d(((x_array*)(&(o->args))),((x_inst*)(((xoico_arg_s*)bcore_fork(arg)) )));
                BLM_TRY(xoico_args_s_append(&(o->args),host, source ))
            }
        BLM_DOWN();}
        else
        {
            BLM_TRY(xoico_args_s_parse(&(o->args),host, source ))
        }
    
        BLM_TRY(x_source_parse_fa(source," )" ))
    }
    
    /// if return type is a name in the argument list, copy argument typespec to return typespec
    const xoico_arg_s* ret_arg = ( o->typespec_ret.indirection == 0 ) ? xoico_signature_s_get_arg_by_name(o,o->typespec_ret.type ) : NULL;
    
    if( ret_arg )
    {
        if( o->typespec_ret.access_class != 0  || o->typespec_ret.transient || o->typespec_ret.indirection != 0 )
        {
            BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Return typespec: Argument name used as type." ))
        }
        xoico_typespec_s_copy(&(o->typespec_ret),&(ret_arg->typespec ));
        BCORE_PASS_CREATE(xoico_typespec_transient_s,o->typespec_ret.transient)->cast_to_var = ret_arg->name;
        xoico_arg_s_attach( &(o->direct_return_arg ),  ((xoico_arg_s*)bcore_fork(((xoico_arg_s*)(ret_arg)))));
    }
    
    BLM_TRY(xoico_signature_s_set_global_name(o,host ))
    
    BLM_RETURNV(er_t, 0)
}

er_t xoico_signature_s_expand_declaration( const xoico_signature_s* o, const xoico_host* host, sc_t sc_func_global_name, sz_t indent, x_sink* sink )
{
    // xoico_signature.x:217:1
    
    BLM_TRY(xoico_signature_s_expand_ret(o,host, sink ))
    x_sink_push_fa(sink," #<sc_t>( ", sc_func_global_name );
    
    if( o->arg_o )
    {
        BLM_TRY(xoico_arg_s_expand(o->arg_o,host, sink ))
        BLM_TRY(xoico_args_s_expand(&(o->args),host, false, sink ))
        x_sink_push_fa(sink," )" );
    }
    else
    {
        if( o->args.size > 0 )
        {
            BLM_TRY(xoico_args_s_expand(&(o->args),host, true, sink ))
        }
        else
        {
            x_sink_push_fa(sink,"void" );
        }
        x_sink_push_fa(sink," )" );
    }
    return  0;
}

bl_t xoico_signature_s_fits_transient_type_feature( const xoico_signature_s* o )
{
    // xoico_signature.x:245:1
    
    if( o->typespec_ret.type != TYPEOF_tp_t ) return  false;
    if( o->typespec_ret.indirection != 0 )    return  false;
    if( !xoico_signature_s_as_member(o) )                     return  false;
    return  true;
}

XOILA_DEFINE_SPECT( xoico, xoico_signature )
"{"
    "bcore_spect_header_s header;"
"}";

/**********************************************************************************************************************/
// source: xoico_body.x

//----------------------------------------------------------------------------------------------------------------------
// group: xoico_body

BCORE_DEFINE_OBJECT_INST_P( xoico_body_code_s )
"aware xoico_body"
"{"
    "bl_t single_line;"
    "tp_t hash_source;"
    "x_source_point_s source_point;"
    "func xoico:get_source_point;"
    "func xoico:parse;"
    "func xoico:get_hash;"
"}";

er_t xoico_body_code_s_parse( xoico_body_code_s* o, const xoico_host* host, x_source* source )
{
    // xoico_body.x:68:1
    
    tp_t hash = bcore_tp_init();
    
    x_source_point_s_setup_from_source(&(o->source_point),source );
    
    if( x_source_parse_bl(source," #?'('" ) )
    {
        while( !x_source_eos(source) && x_source_inspect_char(source) != ')' ) hash = bcore_tp_fold_u0( hash, x_source_get_char(source) );
        BLM_TRY(x_source_parse_fa(source,")" ))
    }
    
    BLM_TRY(x_source_parse_fa(source," {" ))
    
    sz_t nest_count = 1;
    bl_t exit_loop = false;
    o->single_line = true;
    
    while( x_source_parse_bl(source,"#?' '" ) ); // skip leading spaces
    
    while( !x_source_eos(source) && !exit_loop )
    {
        u0_t c =  x_source_get_u0(source);
        switch( c )
        {
            case '{':
            {
                nest_count++;
            }
            break;
    
            case '}':
            {
                nest_count--;
                if( !nest_count )
                {
                    c = 0;
                    exit_loop = true;
                }
            }
            break;
    
            case '"': // string literal
            {
                hash = bcore_tp_fold_u0( hash, c );
                while( !x_source_eos(source) && ((c = x_source_get_char(source)) != '"') )
                {
                    hash = bcore_tp_fold_u0( hash, c );
                    if( c == '\\' ) hash = bcore_tp_fold_u0( hash, x_source_get_u0(source) );
                    if( c == '\n' ) return  x_source_parse_error_fa(source,"Newline in string literal." );
                }
                c = 0;
            }
            break;
    
            case '\'': // char literal
            {
                hash = bcore_tp_fold_u0( hash, c );
                while( !x_source_eos(source) && ((c = x_source_get_char(source)) != '\'') )
                {
                    hash = bcore_tp_fold_u0( hash, c );
                    if( c == '\\' ) hash = bcore_tp_fold_u0( hash, x_source_get_u0(source) );
                    if( c == '\n' ) return  x_source_parse_error_fa(source,"Newline in char literal." );
                }
                c = 0;
            }
            break;
    
            case '/': // comment
            {
                hash = bcore_tp_fold_u0( hash, c );
                if( x_source_inspect_char(source) == '/' )
                {
                    while( !x_source_eos(source) )
                    {
                        c = x_source_get_char(source);
                        hash = bcore_tp_fold_u0( hash, c );
                        if( c == '\n' )
                        {
                            o->single_line = false;
                            break;
                        }
                    }
                }
                else if( x_source_inspect_char(source) == '*' )
                {
                    while( !x_source_eos(source) )
                    {
                        if( x_source_parse_bl(source,"#?'*/'" ) )
                        {
                            hash = bcore_tp_fold_sc( hash, "*/" );
                            break;
                        }
                        else
                        {
                            c = x_source_get_char(source);
                            if( c == '\n' ) o->single_line = false;
                            hash = bcore_tp_fold_u0( hash, c );
                        }
                    }
                }
                c = 0;
            }
            break;
    
            case '\n' :
            {
                o->single_line = false;
                break;
            }
    
            default: break;
        }
        hash = bcore_tp_fold_u0( hash, c );
    }
    
    o->hash_source = hash;
    return  0;
}

er_t xoico_body_code_s_parse_single_statement( xoico_body_code_s* o, const xoico_host* host, x_source* source )
{
    // xoico_body.x:190:1
    
    tp_t hash = bcore_tp_init();
    
    x_source_point_s_setup_from_source(&(o->source_point),source );
    
    sz_t nest_count = 0;
    bl_t exit_loop = false;
    o->single_line = true;
    
    while( x_source_parse_bl(source,"#?' '" ) ); // skip leading spaces
    
    while( !x_source_eos(source) && !exit_loop )
    {
        u0_t c =  x_source_get_u0(source);
        switch( c )
        {
            case '{':
            case '}':
                return  x_source_parse_error_fa(source,"Braces are not allowed in single statement." );
            break;
    
            case '(':
            {
                nest_count++;
            }
            break;
    
            case ')':
            {
                if( nest_count == 0 )
                {
                    return  x_source_parse_error_fa(source,"Unmatched closing bracket ')'." );
                }
                nest_count--;
            }
            break;
    
            case ';':
            {
                if( nest_count == 0 ) exit_loop = true;
            }
            break;
    
            case '"': // string literal
            {
                hash = bcore_tp_fold_u0( hash, c );
                while( !x_source_eos(source) && ((c = x_source_get_char(source)) != '"') )
                {
                    hash = bcore_tp_fold_u0( hash, c );
                    if( c == '\\' ) hash = bcore_tp_fold_u0( hash, x_source_get_u0(source) );
                    if( c == '\n' ) return  x_source_parse_error_fa(source,"Newline in string literal." );
                }
                c = 0;
            }
            break;
    
            case '\'': // char literal
            {
                hash = bcore_tp_fold_u0( hash, c );
                while( !x_source_eos(source) && ((c = x_source_get_char(source)) != '\'') )
                {
                    hash = bcore_tp_fold_u0( hash, c );
                    if( c == '\\' ) hash = bcore_tp_fold_u0( hash, x_source_get_u0(source) );
                    if( c == '\n' ) return  x_source_parse_error_fa(source,"Newline in char literal." );
                }
                c = 0;
            }
            break;
    
            case '/': // comment
            {
                hash = bcore_tp_fold_u0( hash, c );
                if( x_source_inspect_char(source) == '/' )
                {
                    while( !x_source_eos(source) )
                    {
                        c = x_source_get_char(source);
                        hash = bcore_tp_fold_u0( hash, c );
                        if( c == '\n' )
                        {
                            o->single_line = false;
                            break;
                        }
                    }
                }
                else if( x_source_inspect_char(source) == '*' )
                {
                    while( !x_source_eos(source) )
                    {
                        if( x_source_parse_bl(source,"#?'*/'" ) )
                        {
                            hash = bcore_tp_fold_sc( hash, "*/" );
                            break;
                        }
                        else
                        {
                            c = x_source_get_char(source);
                            if( c == '\n' ) o->single_line = false;
                            hash = bcore_tp_fold_u0( hash, c );
                        }
                    }
                }
                c = 0;
            }
            break;
    
            case '\n' :
            {
                o->single_line = false;
                break;
            }
    
            default: break;
        }
        hash = bcore_tp_fold_u0( hash, c );
    }
    
    o->hash_source = hash;
    return  0;
}

tp_t xoico_body_code_s_get_hash( const xoico_body_code_s* o )
{
    // xoico_body.x:314:1
    
    tp_t hash = bcore_tp_fold_tp( bcore_tp_init(), o->_ );
    hash = bcore_tp_fold_tp( hash, o->hash_source );
    return  hash;
}

BCORE_DEFINE_OBJECT_INST_P( xoico_body_s )
"aware xoico_body"
"{"
    "tp_t name;"
    "tp_t global_name;"
    "xoico_body_code_s => code;"
    "bl_t go_inline;"
    "x_source_point_s source_point;"
    "func xoico:get_global_name_tp;"
    "func xoico:get_hash;"
    "func xoico:parse;"
"}";

tp_t xoico_body_s_get_hash( const xoico_body_s* o )
{
    // xoico_body.x:327:1
    
    tp_t hash = bcore_tp_fold_tp( bcore_tp_init(), o->_ );
    hash = bcore_tp_fold_tp( hash, o->name );
    if( o->code ) hash = bcore_tp_fold_tp( hash, xoico_body_code_s_get_hash(o->code) );
    hash = bcore_tp_fold_bl( hash, o->go_inline );
    return  hash;
}

er_t xoico_body_s_parse_expression( xoico_body_s* o, const xoico_host* host, x_source* source )
{
    // xoico_body.x:338:1
    
    if( x_source_parse_bl(source," #=?'{'" ) )
    {
        xoico_body_code_s_attach( &(o->code ),  xoico_body_code_s_create());
        BLM_TRY(xoico_body_code_s_parse(o->code,host, source ))
        o->go_inline = o->code->single_line;
    }
    else
    {BLM_INIT_LEVEL(1);
        s3_t index = x_source_get_index(source);
    
        xoico_compiler_s* compiler = xoico_host_a_compiler(host);
    
        st_s st_name;BLM_T_INIT_SPUSH(st_s, &st_name);;
        BLM_TRY(xoico_host_a_parse_name_st(host,source,&( st_name )))
        tp_t tp_name = btypeof( st_name.sc );
    
        // if name_buf refers to another body
        if( xoico_compiler_s_is_body(compiler,tp_name ) )
        {
            const xoico_body_s* body = xoico_compiler_s_get_body(compiler,tp_name );
            xoico_body_code_s_attach( &(o->code ),  xoico_body_code_s_clone(body->code));
            o->go_inline = body->go_inline;
        }
        else // at this point we assume a single statement body
        {
            x_source_set_index(source,index );
            xoico_body_code_s_attach( &(o->code ),  xoico_body_code_s_create());
            BLM_TRY(xoico_body_code_s_parse_single_statement(o->code,host, source ))
            o->go_inline = o->code->single_line;
        }
    BLM_DOWN();}
    return  0;
}

er_t xoico_body_s_parse( xoico_body_s* o, const xoico_host* host, x_source* source )
{
    // xoico_body.x:376:1
    BLM_INIT_LEVEL(0);
    x_source_point_s_setup_from_source(&(o->source_point),source );
    
    bl_t force_inline = x_source_parse_bl(source," #?w'inline'" );
    bl_t force_export = x_source_parse_bl(source," #?w'export'" );
    
    if( x_source_parse_bl(source," #=?'='" ) )
    {
        s3_t index = x_source_get_index(source);
        if( x_source_parse_bl(source,"= #?'{'" ) )
        {
            BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Explicit block assignment '=' is deprecated. Remove assignment operator.\n" ))
        }
        else // assignment can be used as completion in a single-statement body
        {
            x_source_set_index(source,index );
        }
    }
    
    BLM_TRY(xoico_body_s_parse_expression(o,host, source ))
    o->go_inline = force_inline ? true : force_export ? false : o->go_inline;
    
    xoico_compiler_s* compiler = xoico_host_a_compiler(host);
    o->global_name = xoico_host_a_entypeof(host,((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create_fa("#<sc_t>_#<sc_t>", xoico_compiler_s_nameof(compiler,xoico_host_a_obj_type(host) ), xoico_compiler_s_nameof(compiler,o->name ) )))->sc );
    BLM_RETURNV(er_t, 0)
}

er_t xoico_body_s_expand( const xoico_body_s* o, const xoico_host* host, const xoico_signature_s* signature, sz_t indent, x_sink* sink )
{
    // xoico_body.x:406:1
    BLM_INIT_LEVEL(0);
    const st_s* final_code = NULL;
    st_s* st_out = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
    
    xoico_cengine* cengine = xoico_host_a_cengine(host);
    ASSERT( cengine );
    
    if( o->code )
    {
        BLM_TRY(xoico_cengine_a_translate(cengine,host, o, signature, ( x_sink* )st_out ))
        final_code = st_out;
    }
    
    if( o->go_inline )
    {
        x_sink_push_fa(sink,"{#<sc_t>}", final_code->sc );
    }
    else
    {
        x_sink_push_fa(sink,"{\n#rn{ }", indent + 4 );
        for(sz_t i = 0; i < final_code->size; i++ )
        {
            char c = final_code->sc[ i ];
            x_sink_push_char(sink,c );
            if( c == '\n' ) x_sink_push_fa(sink,"#rn{ }", indent + 4 );
        }
        x_sink_push_fa(sink,"\n#rn{ }}", indent );
    }
    BLM_RETURNV(er_t, 0)
}

XOILA_DEFINE_SPECT( xoico, xoico_body )
"{"
    "bcore_spect_header_s header;"
"}";

/**********************************************************************************************************************/
// source: xoico_func.x

//----------------------------------------------------------------------------------------------------------------------
// group: xoico_func

BCORE_DEFINE_OBJECT_INST_P( xoico_func_s )
"aware xoico_func"
"{"
    "tp_t name;"
    "tp_t global_name;"
    "tp_t obj_type;"
    "tp_t signature_global_name;"
    "bl_t expandable = true;"
    "bl_t overloadable = false;"
    "bl_t declare_in_expand_forward = true;"
    "bl_t register_in_function_manager = false;"
    "tp_t pre_hash = 0;"
    "xoico_body_s => body;"
    "xoico_signature_s => signature;"
    "x_source_point_s source_point;"
    "func xoico:get_source_point;"
    "func xoico:parse;"
    "func xoico:finalize;"
    "func xoico:expand_forward;"
    "func xoico:expand_declaration;"
    "func xoico:expand_definition;"
    "func xoico:expand_init1;"
"}";

bl_t xoico_func_s_reflectable( const xoico_func_s* o, const xoico_host* host )
{
    // xoico_func.x:53:5
    
    return  o->expandable && xoico_compiler_s_is_feature(xoico_host_a_compiler(host),o->signature_global_name );
}

tp_t xoico_func_s_get_hash( const xoico_func_s* o )
{
    // xoico_func.x:75:1
    
    tp_t hash = bcore_tp_fold_tp( bcore_tp_init(), o->_ );
    hash = bcore_tp_fold_tp( hash, o->pre_hash );
    hash = bcore_tp_fold_tp( hash, o->name );
    hash = bcore_tp_fold_tp( hash, o->global_name );
    hash = bcore_tp_fold_tp( hash, o->obj_type );
    hash = bcore_tp_fold_tp( hash, o->signature_global_name );
    hash = bcore_tp_fold_bl( hash, o->expandable );
    hash = bcore_tp_fold_bl( hash, o->overloadable );
    hash = bcore_tp_fold_bl( hash, o->register_in_function_manager );
    
    if( o->body      ) hash = bcore_tp_fold_tp( hash, xoico_body_s_get_hash(o->body) );
    if( o->signature ) hash = bcore_tp_fold_tp( hash, xoico_signature_s_get_hash(o->signature) );
    return  hash;
}

er_t xoico_func_s_freeze_global_name( xoico_func_s* o, const xoico_host* host )
{
    // xoico_func.x:94:1
    BLM_INIT_LEVEL(0);
    if( o->global_name ) BLM_RETURNV(er_t, 0)
    xoico_compiler_s* compiler = xoico_host_a_compiler(host);
    o->global_name = xoico_compiler_s_entypeof(compiler,((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create_fa("#<sc_t>_#<sc_t>", xoico_compiler_s_nameof(compiler,xoico_host_a_obj_type(host) ), xoico_compiler_s_nameof(compiler,o->name ) )))->sc );
    BLM_RETURNV(er_t, 0)
}

er_t xoico_func_s_parse( xoico_func_s* o, const xoico_host* host, x_source* source )
{
    // xoico_func.x:104:1
    BLM_INIT_LEVEL(0);
    xoico_compiler_s* compiler = xoico_host_a_compiler(host);
    
    x_source_point_s_setup_from_source(&(o->source_point),source );
    
    if( x_source_parse_bl(source," #?'('" ) ) // old style enclosed signature
    {
        BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Signature-enclosure '( ...sig... )' is deprecated." ))
    }
    
    // We first try parsing a direct signature. If that fails, we assume the signature is referenced by an identifier.
    // This allows parsing without old style bracket enclosing. If that is a save practice under all circumstances is to be seen.
    s3_t index = x_source_get_index(source);
    xoico_signature_s* signature = ((xoico_signature_s*)BLM_LEVEL_T_PUSH(0,xoico_signature_s,xoico_signature_s_create()));
    
    if( xoico_signature_s_parse(signature,host, source ) != 0 )
    {
        bcore_error_remove_last();
        x_source_set_index(source,index );
        signature = NULL;
    }
    
    if( signature )
    {
        BLM_TRY(xoico_compiler_s_life_a_push(compiler,((bcore_inst*)(((xoico_signature_s*)bcore_fork(signature)) ))))
        o->pre_hash = bcore_tp_fold_tp( o->pre_hash, xoico_signature_s_get_hash(signature) );
        o->name = signature->name;
        BLM_TRY(xoico_compiler_s_register_item(compiler,((const xoico*)(signature ))))
        o->signature_global_name = signature->global_name;
        xoico_signature_s_attach( &(o->signature ),  xoico_signature_s_clone(signature));
        BLM_TRY(xoico_signature_s_relent(o->signature,host, xoico_host_a_obj_type(host) ))
        if( xoico_host_defines_transient_map(host) ) BLM_TRY(xoico_signature_s_convert_transient_types(o->signature,host, xoico_host_a_transient_map(host) ))
    }
    else // use of external signature definition
    {
        tp_t tp_signature_base_name = 0;
        tp_t tp_signature_global_name = 0;
    
        if( x_source_parse_bl(source," #?'^'" ) )
        {
            if( host->_ == ((tp_t)(TYPEOF_xoico_stamp_s)) )
            {
                tp_signature_base_name = ((const xoico_stamp_s*)(host))->trait_name;
            }
            else
            {
                BLM_RETURNV(er_t, x_source_parse_error_fa(source,"'^' can only be used inside a stamp." ))
            }
            BLM_TRY(x_source_parse_fa(source," ." ))
        }
        else
        {
            tp_t name = 0;
            BLM_TRY(xoico_host_a_parse_name_tp(host,source, (&(name)) ))
            if( x_source_parse_bl(source," #?'.'" ) )
            {
                tp_signature_base_name = name;
            }
            else
            {
                sz_t idx = -1;
                if( host->_ == ((tp_t)(TYPEOF_xoico_stamp_s)) && ( idx = xoico_funcs_s_get_index_from_name(&(((const xoico_stamp_s*)(host))->funcs),name ) ) >= 0 )
                {
                    tp_signature_global_name = ((const xoico_stamp_s*)(host))->funcs.data[ idx ]->signature_global_name;
                }
                else
                {
                    tp_signature_base_name = xoico_a_get_global_name_tp(((const xoico*)(host)));
                }
                o->name = name;
            }
        }
    
        if( !o->name )
        {BLM_INIT_LEVEL(2);
            st_s st_name;BLM_T_INIT_SPUSH(st_s, &st_name);;
            BLM_TRY(x_source_parse_fa(source," #name", (&(st_name)) )) // empty name is allowed
            o->name = xoico_compiler_s_entypeof(compiler,st_name.sc );
        BLM_DOWN();}
    
        if( !tp_signature_global_name )
        {BLM_INIT_LEVEL(2);
            tp_signature_global_name = xoico_compiler_s_entypeof(compiler,((st_s*)BLM_LEVEL_T_PUSH(2,st_s,st_s_create_fa("#<sc_t>_#<sc_t>", xoico_compiler_s_nameof(compiler,tp_signature_base_name ), xoico_compiler_s_nameof(compiler,o->name ) )))->sc );
        BLM_DOWN();}
    
        o->signature_global_name = tp_signature_global_name;
    }
    
    if( !x_source_parse_bl(source," #=?';'" ) ) BLM_TRY(xoico_body_s_parse(BCORE_PASS_CREATE(xoico_body_s,o->body),host, source ))
    
    BLM_TRY(x_source_parse_fa(source," #-?';' " )) // closing semicolon is optional
    
    BLM_RETURNV(er_t, 0)
}

er_t xoico_func_s_push_flect_decl_to_sink( const xoico_func_s* o, const xoico_host* host, x_sink* sink )
{
    // xoico_func.x:202:1
    
    xoico_compiler_s* compiler = xoico_host_a_compiler(host);
    x_sink_push_sc(sink,"func " );
    if( host->_ == ((tp_t)(TYPEOF_xoico_stamp_s)) && o->signature->base_name == ((const xoico_stamp_s*)(host))->trait_name )
    {
        x_sink_push_fa(sink,"^:#<sc_t>", xoico_compiler_s_nameof(compiler,o->name ) );
    }
    else
    {
        x_sink_push_fa(sink,"#<sc_t>:#<sc_t>", xoico_compiler_s_nameof(compiler,o->signature->base_name ), xoico_compiler_s_nameof(compiler,o->name ) );
    }
    x_sink_push_sc(sink,";" );
    return  0;
}

er_t xoico_func_s_finalize( xoico_func_s* o, const xoico_host* host )
{
    // xoico_func.x:220:1
    
    xoico_compiler_s* compiler = xoico_host_a_compiler(host);
    BLM_TRY(xoico_func_s_freeze_global_name(o,host ))
    o->obj_type = xoico_host_a_obj_type(host);
    
    if( !o->signature )
    {
        const xoico_signature_s* signature = xoico_compiler_s_get_signature(compiler,o->signature_global_name );
        if( !signature )
        {
            return  x_source_point_s_parse_error_fa(&(o->source_point),"Function #<sc_t>: Could not find signature #<sc_t>",
                xoico_compiler_s_nameof(compiler,o->name ),
                xoico_compiler_s_nameof(compiler,o->signature_global_name )
            );
        }
    
        o->signature = xoico_signature_s_clone(signature);
        BLM_TRY(xoico_signature_s_relent(o->signature,host, xoico_host_a_obj_type(host) ))
        if( xoico_host_defines_transient_map(host) ) BLM_TRY(xoico_signature_s_convert_transient_types(o->signature,host, xoico_host_a_transient_map(host) ))
    }
    
    if( o->body ) BLM_TRY(xoico_body_s_finalize(o->body,host ))
    
    return  0;
}

er_t xoico_func_s_expand_forward( const xoico_func_s* o, const xoico_host* host, sz_t indent, x_sink* sink )
{
    // xoico_func.x:251:1
    
    if( !o->expandable ) return  0;
    if( !o->declare_in_expand_forward ) return  0;
    
    xoico_compiler_s* compiler = xoico_host_a_compiler(host);
    x_sink_push_fa(sink," \\\n#rn{ }", indent );
    bl_t go_inline = o->body && o->body->go_inline;
    const xoico_signature_s* signature = o->signature;
    ASSERT( signature );
    if( go_inline )
    {
        x_sink_push_fa(sink,"static inline " );
        BLM_TRY(xoico_signature_s_expand_declaration(signature,host, xoico_compiler_s_nameof(compiler,o->global_name ), indent, sink ))
    }
    else
    {
        BLM_TRY(xoico_signature_s_expand_declaration(signature,host, xoico_compiler_s_nameof(compiler,o->global_name ), indent, sink ))
    }
    x_sink_push_fa(sink,";" );
    
    return  0;
}

er_t xoico_func_s_expand_declaration( const xoico_func_s* o, const xoico_host* host, sz_t indent, x_sink* sink )
{
    // xoico_func.x:277:1
    
    if( !o->expandable ) return  0;
    
    bl_t go_inline = o->body && o->body->go_inline;
    const xoico_signature_s* signature = o->signature;
    ASSERT( signature );
    xoico_compiler_s* compiler = xoico_host_a_compiler(host);
    
    if( go_inline )
    {
        x_sink_push_fa(sink," \\\n#rn{ }", indent );
        x_sink_push_fa(sink,"static inline " );
        BLM_TRY(xoico_signature_s_expand_declaration(signature,host, xoico_compiler_s_nameof( compiler,o->global_name ), indent, sink ))
        BLM_TRY(xoico_body_s_expand(o->body,host, signature, indent, sink ))
    }
    else if( !o->declare_in_expand_forward )
    {
        x_sink_push_fa(sink," \\\n#rn{ }", indent );
        BLM_TRY(xoico_signature_s_expand_declaration(signature,host, xoico_compiler_s_nameof( compiler,o->global_name ), indent, sink ))
        x_sink_push_fa(sink,";" );
    }
    
    return  0;
}

er_t xoico_func_s_expand_definition( const xoico_func_s* o, const xoico_host* host, sz_t indent, x_sink* sink )
{
    // xoico_func.x:305:1
    
    if( !o->expandable ) return  0;
    xoico_compiler_s* compiler = xoico_host_a_compiler(host);
    
    const xoico_signature_s* signature = o->signature;
    ASSERT( signature );
    if( o->body && !o->body->go_inline )
    {
        x_sink_push_fa(sink,"\n" );
        x_sink_push_fa(sink,"#rn{ }", indent );
        BLM_TRY(xoico_signature_s_expand_declaration(signature,host, xoico_compiler_s_nameof(compiler,o->global_name ), indent, sink ))
        x_sink_push_fa(sink,"\n" );
        BLM_TRY(xoico_body_s_expand(o->body,host, signature, indent, sink ))
        x_sink_push_fa(sink,"\n" );
    }
    
    return  0;
}

er_t xoico_func_s_expand_init1( const xoico_func_s* o, const xoico_host* host, sz_t indent, x_sink* sink )
{
    // xoico_func.x:327:1
    
    if( !o->expandable ) return  0;
    if( !o->register_in_function_manager ) return  0;
    
    sc_t sc_global_name = xoico_compiler_s_nameof(xoico_host_a_compiler(host),o->global_name );
    
    x_sink_push_fa(sink,"#rn{ }BCORE_REGISTER_FUNC( #<sc_t> );\n",
        indent,
        sc_global_name
    );
    
    return  0;
}

XOILA_DEFINE_SPECT( xoico, xoico_func )
"{"
    "bcore_spect_header_s header;"
"}";

/**********************************************************************************************************************/
// source: xoico_funcs.x

//----------------------------------------------------------------------------------------------------------------------
// group: xoico_funcs

BCORE_DEFINE_OBJECT_INST_P( xoico_funcs_s )
"aware x_array"
"{"
    "xoico_func_s => [];"
"}";

sz_t xoico_funcs_s_get_index_from_signature_global_name( const xoico_funcs_s* o, tp_t signature_global_name )
{
    // xoico_funcs.x:34:5
    
    {const xoico_funcs_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_func_s* e=__a->data[__i]; if( e->signature_global_name == signature_global_name ) return  __i;
    }}return  -1;
}

bl_t xoico_funcs_s_exists_from_signature_global_name( const xoico_funcs_s* o, tp_t signature_global_name )
{
    // xoico_funcs.x:40:5
    
    return  ( xoico_funcs_s_get_index_from_signature_global_name(o,signature_global_name ) >= 0 );
}

sz_t xoico_funcs_s_get_index_from_name( const xoico_funcs_s* o, tp_t name )
{
    // xoico_funcs.x:45:5
    
    {const xoico_funcs_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_func_s* e=__a->data[__i]; if( e->name == name ) return  __i;
    }}return  -1;
}

bl_t xoico_funcs_s_exists_from_name( const xoico_funcs_s* o, tp_t name )
{
    // xoico_funcs.x:51:5
    
    return  ( xoico_funcs_s_get_index_from_name(o,name ) >= 0 );
}

xoico_func_s* xoico_funcs_s_get_func_from_signature_global_name( const xoico_funcs_s* o, tp_t signature_global_name )
{
    // xoico_funcs.x:56:5
    
    sz_t idx = xoico_funcs_s_get_index_from_signature_global_name(o,signature_global_name );
    return  ( idx >= 0 ) ? o->data[ idx ] : NULL;
}

xoico_func_s* xoico_funcs_s_get_func_from_name( const xoico_funcs_s* o, tp_t name )
{
    // xoico_funcs.x:62:5
    
    sz_t idx = xoico_funcs_s_get_index_from_name(o,name );
    return  ( idx >= 0 ) ? o->data[ idx ] : NULL;
}

er_t xoico_funcs_s_replace_d( xoico_funcs_s* o, sz_t idx, xoico_func_s* func )
{
    // xoico_funcs.x:68:5
    
    ASSERT( idx >= 0 && idx < o->size );
    xoico_func_s_attach( &(o->data[ idx ] ),  NULL);
    for(sz_t i = idx + 1; i < o->size; i++ ) o->data[ i - 1 ] = o->data[ i ];
    o->data[ o->size - 1 ] = func;
    return  0;
}

tp_t xoico_funcs_s_get_hash( const xoico_funcs_s* o )
{
    // xoico_funcs.x:77:5
    
    tp_t hash = bcore_tp_fold_tp( bcore_tp_init(), o->_ );
    {const xoico_funcs_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_func_s* func=__a->data[__i]; hash = bcore_tp_fold_tp( hash, xoico_func_s_get_hash(func) );}}
    return  hash;
}

XOILA_DEFINE_SPECT( xoico, xoico_funcs )
"{"
    "bcore_spect_header_s header;"
"}";

/**********************************************************************************************************************/
// source: xoico_feature.x

//----------------------------------------------------------------------------------------------------------------------
// group: xoico_feature

BCORE_DEFINE_OBJECT_INST_P( xoico_feature_s )
"aware xoico_feature"
"{"
    "xoico_signature_s signature;"
    "xoico_funcs_s funcs;"
    "xoico_funcs_s funcs_return_to_group;"
    "tp_t function_pointer_name;"
    "st_s st_default_func_name;"
    "xoico_body_s => default_body;"
    "bl_t strict;"
    "bl_t flag_p;"
    "bl_t flag_t;"
    "bl_t flag_a;"
    "bl_t flag_r;"
    "bl_t expandable = true;"
    "private aware xoico_group_s* group;"
    "x_source_point_s source_point;"
    "func xoico:get_global_name_tp;"
    "func xoico:get_source_point;"
    "func xoico:get_hash;"
    "func xoico:parse;"
    "func xoico:finalize;"
    "func xoico:expand_forward;"
    "func xoico:expand_indef_typedef;"
    "func xoico:expand_spect_declaration;"
    "func xoico:expand_spect_definition;"
    "func xoico:expand_indef_declaration;"
    "func xoico:expand_definition;"
    "func xoico:expand_init1;"
"}";

tp_t xoico_feature_s_get_hash( const xoico_feature_s* o )
{
    // xoico_feature.x:65:1
    
    tp_t hash = bcore_tp_fold_tp( bcore_tp_init(), o->_ );
    hash = bcore_tp_fold_tp( hash, xoico_signature_s_get_hash(&(o->signature)) );
    hash = bcore_tp_fold_tp( hash, o->default_body ? xoico_body_s_get_hash(o->default_body) : 0 );
    hash = bcore_tp_fold_bl( hash, o->strict );
    hash = bcore_tp_fold_bl( hash, o->flag_p );
    hash = bcore_tp_fold_bl( hash, o->flag_t );
    hash = bcore_tp_fold_bl( hash, o->flag_a );
    hash = bcore_tp_fold_bl( hash, o->flag_r );
    hash = bcore_tp_fold_sc( hash, o->st_default_func_name.sc );
    return  hash;
}

er_t xoico_feature_s_parse( xoico_feature_s* o, const xoico_host* host, x_source* source )
{
    // xoico_feature.x:81:1
    BLM_INIT_LEVEL(0);
    xoico_compiler_s* compiler = xoico_host_a_compiler(host);
    
    x_source_point_s_setup_from_source(&(o->source_point),source );
    o->strict = x_source_parse_bl(source," #?w'strict' " );
    
    if( x_source_parse_bl(source," #?|'|" ) )
    {
        st_s* flags = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
        BLM_TRY(x_source_parse_fa(source," #until{'}", flags ))
        for(sz_t i = 0; i < flags->size; i++ )
        {
            switch( flags->data[ i ] )
            {
                case 'p': o->flag_p = true; break;
                case 't': o->flag_t = true; break;
                case 'a': o->flag_a = true; break;
                case 'r': o->flag_r = true; break;
                default: BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Feature: Flag '#<char>' not handled. Choose from 'ptar'.", flags->data[ i ] ))
            }
        }
        BLM_TRY(x_source_parse_fa(source,"' " ))
    }
    else
    {
        /// the default feature is an aware one
        o->flag_a = true;
    }
    
    xoico_signature_s* signature =&( o->signature);
    BLM_TRY(xoico_signature_s_parse(signature,host, source ))
    
    if( xoico_signature_s_is_anonymous(signature) )
    {
        BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Feature: Anonymous features are not supported." ))
    }
    
    if( !signature->arg_o )
    {
        BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Feature: A feature must have a first argument of name 'o'." ))
    }
    
    if( !signature->name )
    {
        BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Feature: A feature must have a first argument of name 'o'." ))
    }
    
    if( x_source_parse_bl(source," #?w'extern' " ) )
    {
        st_s_clear(&(o->st_default_func_name));
        BLM_TRY(x_source_parse_fa(source,"#name ", &o->st_default_func_name ))
        if( o->st_default_func_name.size == 0 ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Feature: Default function: Global function name expected." ))
    }
    else if( !x_source_parse_bl(source," #=?';' " ) )
    {
        if( o->strict ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Feature is 'strict'. Default function would have no effect." ))
        o->default_body = xoico_body_s_create();
        BLM_TRY(xoico_body_s_parse(o->default_body,host, source ))
        st_s_copy_fa(&(o->st_default_func_name),"#<sc_t>_default", xoico_compiler_s_nameof(compiler,o->signature.name ) );
    }
    
    BLM_TRY(x_source_parse_fa(source," #-?';' " )) // closing semicolon is optional
    
    o->function_pointer_name = o->signature.global_name;
    
    BLM_TRY(xoico_feature_s_setup_functions(o,host ))
    BLM_RETURNV(er_t, 0)
}

xoico_func_s* xoico_feature_s_create_func_from_sc( const xoico_feature_s* o, const xoico_host* host, sc_t sc )
{
    // xoico_feature.x:153:1
    
    xoico_func_s* func = xoico_func_s_create();
    BLM_TRY_EXIT(xoico_parse_sc(((xoico*)(func)),host, sc ))
    x_source_point_s_copy(&(func->source_point),(&(o->source_point)) );
    return  func;
}

xoico_func_s* xoico_feature_s_push_func_from_sc( xoico_feature_s* o, const xoico_host* host, sc_t sc )
{
    // xoico_feature.x:163:1
    
    return  ((xoico_func_s*)(x_array_push_d(((x_array*)(&(o->funcs))),((x_inst*)(xoico_feature_s_create_func_from_sc(o,host, sc ) )))));
}

er_t xoico_feature_s_setup_functions( xoico_feature_s* o, const xoico_host* host )
{
    // xoico_feature.x:170:1
    BLM_INIT_LEVEL(0);
    xoico_compiler_s* compiler = xoico_host_a_compiler(host);
    sc_t sc_name = xoico_compiler_s_nameof(compiler,o->signature.name );
    sc_t sc_obj_type = xoico_compiler_s_nameof(compiler,xoico_host_a_obj_type(host) );
    sc_t sc_spect_name = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,xoico_host_a_create_spect_name(host)))->sc;
    
    st_s* st_ret_typespec = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
    
    bl_t has_ret = xoico_signature_s_returns_a_value(&(o->signature));
    BLM_TRY(xoico_signature_s_expand_ret_x(&(o->signature),host,((x_sink*)( st_ret_typespec ))))
    
    sc_t sc_ret_typespec = st_ret_typespec->sc;
    bl_t flag_const = o->signature.arg_o->typespec.access_class == TYPEOF_const;
    bl_t flag_discardable = o->signature.arg_o->typespec.access_class == TYPEOF_discardable;
    
    bl_t always_defined = ( o->strict || o->default_body || o->st_default_func_name.size > 0 );
    
    if( o->flag_a )
    {
        st_s* st = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
        st_s_push_fa(st,"#<sc_t> a_#<sc_t>( ", sc_ret_typespec, sc_name );
        st_s_push_fa(st,flag_const ? "c" : flag_discardable ? "d" : "m" );
        st_s_push_fa(st," @* o" );
        BLM_TRY(xoico_args_s_expand_x(&(o->signature.args),host, false,((x_sink*)( st ))))
        st_s_push_fa(st," ) { verbatim_C{" );
        st_s_push_fa(st,"const #<sc_t>* p = #<sc_t>_get_aware( o ); ", sc_spect_name, sc_spect_name );
        st_s_push_fa(st,"assert( p->#<sc_t> ); ", sc_name );
        if( has_ret ) st_s_push_fa(st,"return " );
        st_s_push_fa(st,"p->#<sc_t>( o", sc_name );
        BLM_TRY(xoico_args_s_expand_name(&(o->signature.args),host, false,((x_sink*)( st ))))
        st_s_push_fa(st," ); } };" );
    
        xoico_func_s* func = xoico_feature_s_push_func_from_sc(o,host, st->sc );
        func->declare_in_expand_forward = false;
    
        xoico_func_s* func_to_group = xoico_func_s_clone(func);
        xoico_body_s_attach( &(func_to_group->body ),  NULL);
        BLM_TRY(xoico_func_s_freeze_global_name(func_to_group,host )) // set global name before local name is changed
        func_to_group->name = o->signature.name;
        func_to_group->expandable = false;
        x_array_push_d(((x_array*)(&(o->funcs_return_to_group))),((x_inst*)(func_to_group )));
    }
    
    if( o->flag_a )
    {
        st_s* st = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
        st_s_push_fa(st,"bl_t defines_#<sc_t>( c @* o ) ", sc_name );
        if( always_defined )
        {
            st_s_push_fa(st,"{ return true; };", sc_name );
        }
        else
        {
            st_s_push_fa(st,"{ verbatim_C { return #<sc_t>_get_aware( o )->#<sc_t> != NULL; } };", sc_spect_name, sc_name );
        }
    
        xoico_func_s* func = xoico_feature_s_push_func_from_sc(o,host, st->sc );
        func->declare_in_expand_forward = false;
    
        xoico_func_s* func_to_group = xoico_func_s_clone(func);
        xoico_body_s_attach( &(func_to_group->body ),  NULL);
        func_to_group->expandable = false;
        x_array_push_d(((x_array*)(&(o->funcs_return_to_group))),((x_inst*)(func_to_group )));
    }
    
    if( o->flag_t )
    {
        /// new
        st_s* st = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
        st_s_push_fa(st,"#<sc_t> t_#<sc_t>( ", sc_ret_typespec, sc_name );
        st_s_push_fa(st,flag_const ? "c" : flag_discardable ? "d" : "m" );
        st_s_push_fa(st," #<sc_t>* o, tp_t t", sc_obj_type );
        BLM_TRY(xoico_args_s_expand_x(&(o->signature.args),host, false,((x_sink*)( st ))))
        st_s_push_fa(st," ) { verbatim_C{" );
        st_s_push_fa(st,"const #<sc_t>* p = #<sc_t>_get_typed( t ); ", sc_spect_name, sc_spect_name );
        st_s_push_fa(st,"assert( p->#<sc_t> ); ", sc_name );
        if( has_ret ) st_s_push_fa(st,"return " );
        st_s_push_fa(st,"p->#<sc_t>( o", sc_name );
        BLM_TRY(xoico_args_s_expand_name(&(o->signature.args),host, false,((x_sink*)( st ))))
        st_s_push_fa(st," ); } };" );
    
        xoico_func_s* func = xoico_feature_s_push_func_from_sc(o,host, st->sc );
        func->declare_in_expand_forward = false;
    
        xoico_func_s* func_to_group = xoico_func_s_clone(func);
        xoico_body_s_attach( &(func_to_group->body ),  NULL);
        func_to_group->expandable = false;
        x_array_push_d(((x_array*)(&(o->funcs_return_to_group))),((x_inst*)(func_to_group )));
    }
    
    if( o->flag_t )
    {
        st_s* st = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
        st_s_push_fa(st,"bl_t t_defines_#<sc_t>( tp_t t ) ", sc_name );
        if( always_defined )
        {
            st_s_push_fa(st,"{ return true; };", sc_name );
        }
        else
        {
            st_s_push_fa(st,"{ verbatim_C{ return #<sc_t>_get_typed( t )->#<sc_t> != NULL; } };", sc_spect_name, sc_name );
        }
    
        xoico_func_s* func = xoico_feature_s_push_func_from_sc(o,host, st->sc );
        func->declare_in_expand_forward = false;
    
        xoico_func_s* func_to_group = xoico_func_s_clone(func);
        xoico_body_s_attach( &(func_to_group->body ),  NULL);
        func_to_group->expandable = false;
        x_array_push_d(((x_array*)(&(o->funcs_return_to_group))),((x_inst*)(func_to_group )));
    }
    
    if( o->flag_p )
    {
        st_s* st = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
        st_s_push_fa(st,"#<sc_t> p_#<sc_t>( c #<sc_t>* p, ", sc_ret_typespec, sc_name, sc_spect_name );
        st_s_push_fa(st,flag_const ? "c" : flag_discardable ? "d" : "m" );
        st_s_push_fa(st," #<sc_t>* o", sc_obj_type );
        BLM_TRY(xoico_args_s_expand_x(&(o->signature.args),host, false,((x_sink*)( st ))))
        st_s_push_fa(st," ) { verbatim_C {" );
        st_s_push_fa(st,"assert( p->#<sc_t> ); ", sc_name );
        if( has_ret ) st_s_push_fa(st,"return " );
        st_s_push_fa(st,"p->#<sc_t>( o", sc_name );
        BLM_TRY(xoico_args_s_expand_name(&(o->signature.args),host, false,((x_sink*)( st ))))
        st_s_push_fa(st," ); } };" );
    
        xoico_func_s* func = xoico_feature_s_push_func_from_sc(o,host, st->sc );
        func->declare_in_expand_forward = false;
    }
    
    if( o->flag_p )
    {
        st_s* st = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
        st_s_push_fa(st,"bl_t p_defines_#<sc_t>( c #<sc_t>* p ) ", sc_name, sc_spect_name );
        if( always_defined )
        {
            st_s_push_fa(st,"{ return true; };", sc_name );
        }
        else
        {
            st_s_push_fa(st,"{ verbatim_C{ return p->#<sc_t> != NULL; } };", sc_name );
        }
    
        xoico_func_s* func = xoico_feature_s_push_func_from_sc(o,host, st->sc );
        func->declare_in_expand_forward = false;
    }
    
    if( o->flag_r )
    {
        st_s* st = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
        st_s_push_fa(st,"#<sc_t> r_#<sc_t>(", sc_ret_typespec, sc_name );
        st_s_push_fa(st," c sr_s* o" );
        BLM_TRY(xoico_args_s_expand_x(&(o->signature.args),host, false,((x_sink*)( st ))))
        st_s_push_fa(st," ) { verbatim_C {" );
        if( !flag_const ) st_s_push_fa(st,"ASSERT( !sr_s_is_const( o ) ); " );
        st_s_push_fa(st,"const #<sc_t>* p = (const #<sc_t>*)ch_spect_p( o->p, TYPEOF_#<sc_t> ); ",
            sc_spect_name,
            sc_spect_name,
            sc_spect_name
        );
    
        st_s_push_fa(st,"assert( p->#<sc_t> ); ", sc_name );
        if( has_ret ) st_s_push_fa(st,"return " );
        st_s_push_fa(st,"p->#<sc_t>( o->o", sc_name );
        BLM_TRY(xoico_args_s_expand_name(&(o->signature.args),host, false,((x_sink*)( st ))))
        st_s_push_fa(st," ); } };" );
    
        xoico_func_s* func = xoico_feature_s_push_func_from_sc(o,host, st->sc );
        func->declare_in_expand_forward = false;
    }
    
    if( o->flag_r )
    {
        st_s* st = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
    
        st_s_push_fa(st,"bl_t r_defines_#<sc_t>( c sr_s* o ) ", sc_name );
        if( always_defined )
        {
            st_s_push_fa(st,"{ return true; };", sc_name );
        }
        else
        {
            st_s_push_fa(st,"{ verbatim_C{ return ( (#<sc_t>*)ch_spect_p( o->p, TYPEOF_#<sc_t> ) )->#<sc_t> != NULL; } };", sc_spect_name, sc_spect_name, sc_name );
        }
    
        xoico_func_s* func = xoico_feature_s_push_func_from_sc(o,host, st->sc );
        func->declare_in_expand_forward = false;
    }
    
    if( o->st_default_func_name.size > 0 )
    {BLM_INIT_LEVEL(1);
        st_s* st = ((st_s*)BLM_LEVEL_T_PUSH(1,st_s,st_s_create()));
    
        if( o->signature.direct_return_arg )
        {
            st_s_push_fa(st,"#<sc_t>", xoico_compiler_s_nameof(compiler,o->signature.direct_return_arg->name ) );
        }
        else
        {
            st_s_push_fa(st,"#<sc_t>", sc_ret_typespec );
        }
    
        st_s_push_fa(st," #<sc_t>(", o->st_default_func_name.sc );
        st_s_push_fa(st,flag_const ? " c" : " m" );
        st_s_push_fa(st," #<sc_t>* o", sc_obj_type );
        BLM_TRY(xoico_args_s_expand_x(&(o->signature.args),host, false,((x_sink*)( st ))))
        st_s_push_fa(st," );" );
    
        xoico_func_s* func = xoico_feature_s_push_func_from_sc(o,host, st->sc );
        func->declare_in_expand_forward = false;
        if( o->default_body ) xoico_body_s_attach( &(func->body ),  ((xoico_body_s*)bcore_fork(o->default_body)));
    BLM_DOWN();}
    
    BLM_RETURNV(er_t, 0)
}

er_t xoico_feature_s_finalize( xoico_feature_s* o, const xoico_host* host )
{
    // xoico_feature.x:391:1
    
    {const xoico_funcs_s* __a=&(o->funcs );if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_func_s* func=__a->data[__i]; BLM_TRY(xoico_func_s_finalize(func,host ))
    }}return  0;
}

er_t xoico_feature_s_expand_forward( const xoico_feature_s* o, const xoico_host* host, sz_t indent, x_sink* sink )
{
    // xoico_feature.x:399:1
    
    {const xoico_funcs_s* __a=&(o->funcs );if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_func_s* func=__a->data[__i]; BLM_TRY(xoico_func_s_expand_forward(func,host, indent, sink ))
    }}return  0;
}

er_t xoico_feature_s_expand_indef_typedef( const xoico_feature_s* o, const xoico_host* host, sz_t indent, x_sink* sink )
{
    // xoico_feature.x:407:1
    
    if( !o->expandable ) return  0;
    xoico_compiler_s* compiler = xoico_host_a_compiler(host);
    x_sink_push_fa(sink," \\\n#rn{ }  typedef ", indent );
    BLM_TRY(xoico_signature_s_expand_ret(&(o->signature),host, sink ))
    x_sink_push_fa(sink," (*#<sc_t>)(", xoico_compiler_s_nameof(compiler,o->function_pointer_name ) );
    BLM_TRY(xoico_arg_s_expand(o->signature.arg_o,host, sink ))
    BLM_TRY(xoico_args_s_expand(&(o->signature.args),host, false, sink ))
    x_sink_push_fa(sink," );" );
    return  0;
}

er_t xoico_feature_s_expand_spect_declaration( const xoico_feature_s* o, const xoico_host* host, sz_t indent, x_sink* sink )
{
    // xoico_feature.x:422:1
    
    if( !o->expandable ) return  0;
    xoico_compiler_s* compiler = xoico_host_a_compiler(host);
    x_sink_push_fa(sink," \\\n#rn{ }#<sc_t> #<sc_t>;", indent, xoico_compiler_s_nameof(compiler,o->function_pointer_name ), xoico_compiler_s_nameof(compiler,o->signature.name ) );
    return  0;
}

er_t xoico_feature_s_expand_spect_definition( const xoico_feature_s* o, const xoico_host* host, sz_t indent, x_sink* sink )
{
    // xoico_feature.x:432:1
    
    if( !o->expandable ) return  0;
    xoico_compiler_s* compiler = xoico_host_a_compiler(host);
    x_sink_push_fa(sink,"#rn{ }\"feature ", indent );
    if( o->strict ) x_sink_push_fa(sink,"strict " );
    if( o->flag_a ) x_sink_push_fa(sink,"aware " );
    
    x_sink_push_fa(sink,"#<sc_t> : #<sc_t>", xoico_compiler_s_nameof(compiler,xoico_host_a_obj_type(host) ), xoico_compiler_s_nameof(compiler,o->signature.name ) );
    if( o->st_default_func_name.size > 0 )
    {
        x_sink_push_fa(sink," = #<sc_t>_#<sc_t>", xoico_compiler_s_nameof(compiler,xoico_host_a_obj_type(host) ), o->st_default_func_name.sc );
    }
    x_sink_push_fa(sink,";\"\n" );
    return  0;
}

er_t xoico_feature_s_expand_indef_declaration( const xoico_feature_s* o, const xoico_host* host, sz_t indent, x_sink* sink )
{
    // xoico_feature.x:451:1
    
    if( !o->expandable ) return  0;
    {const xoico_funcs_s* __a=&(o->funcs );if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_func_s* func=__a->data[__i]; BLM_TRY(xoico_func_s_expand_declaration(func,host, indent + 2, sink ))
    }}return  0;
}

er_t xoico_feature_s_expand_definition( const xoico_feature_s* o, const xoico_host* host, sz_t indent, x_sink* sink )
{
    // xoico_feature.x:460:1
    
    if( !o->expandable ) return  0;
    {const xoico_funcs_s* __a=&(o->funcs );if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_func_s* func=__a->data[__i]; BLM_TRY(xoico_func_s_expand_definition(func,host, indent, sink ))
    }}return  0;
}

er_t xoico_feature_s_expand_init1( const xoico_feature_s* o, const xoico_host* host, sz_t indent, x_sink* sink )
{
    // xoico_feature.x:469:1
    
    if( !o->expandable ) return  0;
    xoico_compiler_s* compiler = xoico_host_a_compiler(host);
    sc_t sc_global_name = xoico_compiler_s_nameof(compiler,o->signature.global_name );
    
    x_sink_push_fa(sink,"#rn{ }BCORE_REGISTER_FEATURE( #<sc_t> );\n", indent, sc_global_name );
    if( o->st_default_func_name.size > 0 )
    {
        x_sink_push_fa(sink,"#rn{ }BCORE_REGISTER_FFUNC( #<sc_t>, #<sc_t>_#<sc_t> );\n",
            indent,
            sc_global_name,
            xoico_compiler_s_nameof(compiler,xoico_host_a_obj_type(host) ),
            o->st_default_func_name.sc
        );
    }
    return  0;
}

XOILA_DEFINE_SPECT( xoico, xoico_feature )
"{"
    "bcore_spect_header_s header;"
"}";

/**********************************************************************************************************************/
// source: xoico_features.x

//----------------------------------------------------------------------------------------------------------------------
// group: xoico_features

BCORE_DEFINE_OBJECT_INST_P( xoico_features_s )
"aware x_array"
"{"
    "xoico_feature_s => [];"
"}";

sz_t xoico_features_s_get_index_from_name( const xoico_features_s* o, tp_t name )
{
    // xoico_features.x:28:5
    
    {const xoico_features_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_feature_s* e=__a->data[__i]; if( e->signature.name == name ) return  __i;
    }}return  -1;
}

bl_t xoico_features_s_exists_from_name( const xoico_features_s* o, tp_t name )
{
    // xoico_features.x:34:5
    
    return  ( xoico_features_s_get_index_from_name(o,name ) >= 0 );
}

XOILA_DEFINE_SPECT( xoico, xoico_features )
"{"
    "bcore_spect_header_s header;"
"}";

/**********************************************************************************************************************/
// source: xoico_embedded_data.x

//----------------------------------------------------------------------------------------------------------------------
// group: xoico_embedded_data

BCORE_DEFINE_OBJECT_INST_P( xoico_embedded_data_s )
"aware xoico_embedded_data"
"{"
    "hidden aware xoico_group_s* group;"
    "bcore_arr_u0_s => arr;"
    "tp_t embed_method;"
    "tp_t function_name;"
    "func xoico:parse;"
    "func xoico:get_hash;"
    "func xoico:expand_declaration;"
    "func xoico:expand_definition;"
"}";

er_t xoico_embedded_data_s_parse( xoico_embedded_data_s* o, const xoico_host* host, x_source* source )
{
    // xoico_embedded_data.x:30:5
    
    bcore_arr_u0_s_attach( &(o->arr ),  bcore_arr_u0_s_create());
    while( !x_source_eos(source) ) bcore_arr_u0_s_push(o->arr,x_source_get_u0(source) );
    return  0;
}

tp_t xoico_embedded_data_s_get_hash( const xoico_embedded_data_s* o )
{
    // xoico_embedded_data.x:37:5
    
    tp_t hash = bcore_tp_fold_tp( bcore_tp_init(), o->_ );
    hash = bcore_tp_fold_tp( hash, o->embed_method );
    hash = bcore_tp_fold_tp( hash, o->function_name );
    for(sz_t i = 0; i < o->arr->size; i++ ) hash = bcore_tp_fold_tp( hash, o->arr->data[ i ] );
    return  hash;
}

er_t xoico_embedded_data_s_expand_declaration( const xoico_embedded_data_s* o, const xoico_host* host, sz_t indent, x_sink* sink )
{
    // xoico_embedded_data.x:46:5
    
    if( o->embed_method == TYPEOF_as_string )
    {
    //            sink.push_fa( " \\\n#rn{ }", indent );
    //            sink.push_fa( "st_s* #<sc_t>( void );", host.nameof( o.function_name ) );
    }
    return  0;
}

er_t xoico_embedded_data_s_expand_definition( const xoico_embedded_data_s* o, const xoico_host* host, sz_t indent, x_sink* sink )
{
    // xoico_embedded_data.x:56:5
    
    if( o->embed_method == TYPEOF_as_string )
    {
        x_sink_push_fa(sink,"#rn{ }st_s* #<sc_t>( void )\n", indent, xoico_host_a_nameof(host,o->function_name ) );
        x_sink_push_fa(sink,"#rn{ }{\n"                            , indent );
    
        x_sink_push_fa(sink,"#rn{ }    sc_t sc[] =\n", indent );
        x_sink_push_fa(sink,"#rn{ }    {\n" , indent );
        x_sink_push_fa(sink,"#rn{ }        \"" , indent );
    
        sz_t lines = 0;
    
        for(sz_t i = 0; i < o->arr->size; i++ )
        {
            u0_t c = o->arr->data[ i ];
            switch( c )
            {
                case '\\': x_sink_push_sc(sink,"\\\\" ); break;
                case '"' : x_sink_push_sc(sink,"\\\"" ); break;
                case '\n':
                {
                    if( i + 1 < o->arr->size )
                    {
                        x_sink_push_fa(sink,"\\n\",\n" );
                        x_sink_push_fa(sink,"#rn{ }        \"" , indent );
                        lines++;
                    }
                }
                break;
    
                default: x_sink_push_char(sink,c );  break;
            }
        }
        x_sink_push_fa(sink,"\\n\"\n" );
        x_sink_push_fa(sink,"#rn{ }    };\n", indent );
        lines++;
    
        x_sink_push_fa(sink,"#rn{ }    st_s* st = st_s_create();\n", indent );
        x_sink_push_fa(sink,"#rn{ }    for( sz_t i = 0; i < #<sz_t>; i++ )\n", indent, lines );
        x_sink_push_fa(sink,"#rn{ }    {\n", indent );
        x_sink_push_fa(sink,"#rn{ }        st_s_push_sc( st, sc[ i ] );\n", indent );
        x_sink_push_fa(sink,"#rn{ }    }\n", indent );
        x_sink_push_fa(sink,"#rn{ }    return st;\n", indent );
        x_sink_push_fa(sink,"#rn{ }}\n", indent );
    }
    
    return  0;
}

XOILA_DEFINE_SPECT( xoico, xoico_embedded_data )
"{"
    "bcore_spect_header_s header;"
"}";

/**********************************************************************************************************************/
// source: xoico_group.x

//----------------------------------------------------------------------------------------------------------------------
// group: xoico_group

BCORE_DEFINE_OBJECT_INST_P( xoico_group_source_stack_s )
"aware x_array"
"{"
    "aware x_source -> [];"
"}";

BCORE_DEFINE_OBJECT_INST_P( xoico_group_s )
"aware xoico_group"
"{"
    "aware xoico => [];"
    "private xoico_group_s* lexical_parent;"
    "bcore_arr_st_s includes_in_declaration;"
    "bcore_arr_st_s includes_in_definition;"
    "bcore_arr_st_s explicit_embeddings;"
    "st_s st_name;"
    "tp_t tp_name;"
    "tp_t trait_name = bcore_inst;"
    "tp_t pre_hash;"
    "tp_t beta = 0;"
    "bl_t expandable = true;"
    "bl_t is_manifesto;"
    "bl_t is_retrievable;"
    "bcore_arr_tp_s => retrievable_stamps;"
    "bl_t short_spect_name;"
    "bl_t defines_group_signal_init1;"
    "bl_t defines_group_signal_down1;"
    "xoico_transient_map_s transient_map;"
    "private xoico_stamp_s* extending_stamp;"
    "xoico_funcs_s funcs;"
    "private aware xoico_source_s* xoico_source;"
    "hidden aware xoico_compiler_s* compiler;"
    "x_source_point_s => source_point;"
    "hidden bcore_hmap_tpvd_s hmap_feature;"
    "hidden bcore_hmap_tpvd_s hmap_func;"
    "func xoico:get_global_name_tp;"
    "func xoico:expand_setup;"
    "func xoico:expand_manifesto;"
    "func xoico_host:compiler;"
    "func xoico_host:cengine;"
    "func xoico_host:obj_type;"
    "func xoico_host:transient_map;"
    "func xoico:get_source_point;"
    "func xoico:get_hash;"
    "func xoico_host:create_spect_name;"
    "func xoico_host:parse_name_st;"
    "func xoico_host:parse_name_tp;"
    "func xoico:finalize;"
"}";

void xoico_group_s_set_name_sc( xoico_group_s* o, const xoico_host* host, sc_t name )
{
    // xoico_group.x:72:5
    
    st_s_copy_sc(&(o->st_name),name );
    o->tp_name = xoico_host_a_entypeof(host,name );
}

er_t xoico_group_s_expand_setup( xoico_group_s* o, const xoico_host* host )
{
    // xoico_group.x:132:5
    
    {const xoico_group_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico* e=__a->data[__i]; BLM_TRY(xoico_a_expand_setup(e,((const xoico_host*)(o ))) )
    }}return  0;
}

er_t xoico_group_s_expand_manifesto( const xoico_group_s* o, const xoico_host* host, sz_t indent, x_sink* sink )
{
    // xoico_group.x:144:5
    
    if( !o->expandable || !o->is_manifesto ) return  0;
    x_sink_push_fa(sink,"#rn{ }BETH_EXPAND_GROUP_#<sc_t>\n", indent, o->st_name.sc );
    return  0;
}

const xoico_group_s* xoico_group_s_get_trait_group( const xoico_group_s* o )
{
    // xoico_group.x:153:5
    
    return  ( o->trait_name != o->tp_name ) ? xoico_compiler_s_get_group(o->compiler,o->trait_name ) : NULL;
}

const xoico_feature_s* xoico_group_s_get_traitline_feature_from_name( const xoico_group_s* o, tp_t name )
{
    // xoico_group.x:158:5
    
    if( !o ) return  NULL;
    const xoico_feature_s** p_feature = ( const xoico_feature_s** )bcore_hmap_tpvd_s_get(&(o->hmap_feature),name );
    return  p_feature ? *p_feature : xoico_group_s_get_traitline_feature_from_name(xoico_group_s_get_trait_group(o),name );
}

const xoico_func_s* xoico_group_s_get_traitline_func_from_name( const xoico_group_s* o, tp_t name )
{
    // xoico_group.x:165:5
    
    if( !o ) return  NULL;
    const xoico_func_s** p_func = ( const xoico_func_s** )bcore_hmap_tpvd_s_get(&(o->hmap_func),name );
    return  p_func ? *p_func : xoico_group_s_get_traitline_func_from_name(xoico_group_s_get_trait_group(o),name );
}

const xoico_func_s* xoico_group_s_get_traitline_member_func_from_name( const xoico_group_s* o, tp_t name )
{
    // xoico_group.x:172:5
    
    if( !o ) return  NULL;
    const xoico_func_s** p_func = ( const xoico_func_s** )bcore_hmap_tpvd_s_get(&(o->hmap_func),name );
    if( p_func && xoico_func_s_as_member(*(p_func)) ) return  (*(p_func));
    return  xoico_group_s_get_traitline_member_func_from_name(xoico_group_s_get_trait_group(o),name );
}

const xoico_feature_s* xoico_group_s_get_feature( const xoico_group_s* o, tp_t name )
{
    // xoico_group.x:180:5
    
    const xoico_feature_s** p_feature = ( const xoico_feature_s** )bcore_hmap_tpvd_s_get(&(o->hmap_feature),name );
    return  p_feature ? *p_feature : NULL;
}

const xoico_func_s* xoico_group_s_get_func( const xoico_group_s* o, tp_t name )
{
    // xoico_group.x:186:5
    
    const xoico_func_s** p_func = ( const xoico_func_s** )bcore_hmap_tpvd_s_get(&(o->hmap_func),name );
    return  p_func ? *p_func : NULL;
}

xoico_cengine* xoico_group_s_cengine( const xoico_group_s* o )
{
    // xoico_group.x:198:5
    
    return  o->xoico_source->target->cengine;
}

tp_t xoico_group_s_get_hash( const xoico_group_s* o )
{
    // xoico_group.x:243:1
    
    tp_t hash = o->pre_hash;
    hash = bcore_tp_fold_tp( hash, o->tp_name );
    hash = bcore_tp_fold_tp( hash, o->trait_name );
    hash = bcore_tp_fold_bl( hash, o->is_retrievable );
    hash = bcore_tp_fold_bl( hash, o->expandable );
    hash = bcore_tp_fold_bl( hash, o->short_spect_name );
    hash = bcore_tp_fold_tp( hash, o->beta );
    hash = bcore_tp_fold_tp( hash, xoico_funcs_s_get_hash(&(o->funcs)) );
    {const xoico_group_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico* e=__a->data[__i]; hash = bcore_tp_fold_tp( hash, xoico_a_get_hash(e) );}}
    {const bcore_arr_st_s* __a=&(o->includes_in_declaration );if(__a)for(sz_t __i=0;__i<__a->size;__i++){st_s* e=__a->data[__i]; hash = bcore_tp_fold_sc( hash, e->sc );}}
    {const bcore_arr_st_s* __a=&(o->includes_in_definition  );if(__a)for(sz_t __i=0;__i<__a->size;__i++){st_s* e=__a->data[__i]; hash = bcore_tp_fold_sc( hash, e->sc );}}
    {const bcore_arr_st_s* __a=&(o->explicit_embeddings     );if(__a)for(sz_t __i=0;__i<__a->size;__i++){st_s* e=__a->data[__i]; hash = bcore_tp_fold_sc( hash, e->sc );}}
    return  hash;
}

st_s* xoico_group_s_create_spect_name( const xoico_group_s* o )
{
    // xoico_group.x:262:1
    
    if( o->short_spect_name )
    {
        return  st_s_create_fa("#<sc_t>_s", o->st_name.sc );
    }
    else
    {
        return  st_s_create_fa("#<sc_t>_spect_s", o->st_name.sc );
    }
}

er_t xoico_group_s_parse_name_recursive( const xoico_group_s* o, x_source* source, st_s* name )
{
    // xoico_group.x:276:1
    
    if( x_source_parse_bl(source,"#?':'" ) )
    {
        if( o->lexical_parent )
        {
            BLM_TRY(xoico_group_s_parse_name_recursive(o->lexical_parent,source, name ))
        }
        else
        {
            return  x_source_parse_error_fa(source,"Too many colons: Namespace depth exceeded." );
        }
    }
    else
    {BLM_INIT_LEVEL(1);
        st_s_copy(name,&(o->st_name ));
        st_s* s = ((st_s*)BLM_LEVEL_T_PUSH(1,st_s,st_s_create()));
        BLM_TRY(x_source_parse_fa(source,"#name", s ))
        if( s->size > 0 ) st_s_push_fa(name,"_#<sc_t>", s->sc );
    BLM_DOWN();}
    return  0;
}

er_t xoico_group_s_parse_name_st( const xoico_group_s* o, x_source* source, st_s* name )
{
    // xoico_group.x:301:1
    
    if( x_source_parse_bl(source," #?':'" ) )
    {
        BLM_TRY(xoico_group_s_parse_name_recursive(o,source, name ))
    }
    else
    {
        BLM_TRY(x_source_parse_fa(source," #name", name ))
    }
    
    if( name->size > 0 ) xoico_compiler_s_entypeof(o->compiler,name->sc );
    
    return  0;
}

er_t xoico_group_s_parse_name_tp( const xoico_group_s* o, x_source* source, tp_t* name )
{
    // xoico_group.x:319:1
    BLM_INIT_LEVEL(0);
    st_s* s = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
    
    if( x_source_parse_bl(source," #?':'" ) )
    {
        BLM_TRY(xoico_group_s_parse_name_recursive(o,source, s ))
    }
    else
    {
        BLM_TRY(x_source_parse_fa(source," #name", s ))
    }
    
    if( s->size == 0 ) BLM_TRY(x_source_parse_error_fa(source,"Identifier expected." ))
    
    if( (name) ) (*(name)) = xoico_compiler_s_entypeof(o->compiler,s->sc );
    
    BLM_RETURNV(er_t, 0)
}

er_t xoico_group_s_push_inexpandable_feature_from_sc( xoico_group_s* o, sc_t sc )
{
    // xoico_group.x:341:1
    BLM_INIT_LEVEL(0);
    xoico_compiler_s* compiler = o->compiler;
    xoico_feature_s* feature = ((xoico_feature_s*)BLM_LEVEL_T_PUSH(0,xoico_feature_s,xoico_feature_s_create()));
    feature->expandable = false;
    BLM_TRY(xoico_feature_s_parse(feature,((const xoico_host*)(o)), ((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_sc(sc ))) ))
    
    if( !xoico_compiler_s_is_item(compiler,xoico_feature_s_get_global_name_tp(feature) ) )
    {
        {const xoico_funcs_s* __a=&(feature->funcs_return_to_group );if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_func_s* func=__a->data[__i]; x_array_push_d(((x_array*)(&(o->funcs))),((x_inst*)(((xoico_func_s*)bcore_fork(func)) )));}}
        x_array_clear(((x_array*)(&(feature->funcs_return_to_group))));
        BLM_TRY(xoico_compiler_s_register_item(compiler,((const xoico*)(feature ))))
        bcore_hmap_tpvd_s_set(&(o->hmap_feature),feature->signature.name, ( vd_t )feature );
        xoico_group_s_push_item_d(o,((xoico*)(((xoico_feature_s*)bcore_fork(feature)) )));
    }
    
    BLM_RETURNV(er_t, 0)
}

er_t xoico_group_s_push_inexpandable_func_from_sc( xoico_group_s* o, sc_t sc )
{
    // xoico_group.x:362:1
    BLM_INIT_LEVEL(0);
    xoico_func_s* func = ((xoico_func_s*)BLM_LEVEL_T_PUSH(0,xoico_func_s,xoico_func_s_create()));
    func->expandable = false;
    BLM_TRY(xoico_func_s_parse(func,((const xoico_host*)(o)), ((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_sc(sc ))) ))
    BLM_TRY(xoico_group_s_push_func_d(o,((xoico_func_s*)bcore_fork(func)) ))
    BLM_RETURNV(er_t, 0)
}

er_t xoico_group_s_push_func_from_sc( xoico_group_s* o, sc_t sc )
{
    // xoico_group.x:373:1
    BLM_INIT_LEVEL(0);
    xoico_func_s* func = ((xoico_func_s*)BLM_LEVEL_T_PUSH(0,xoico_func_s,xoico_func_s_create()));
    func->expandable = true;
    BLM_TRY(xoico_func_s_parse(func,((const xoico_host*)(o)), ((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_sc(sc ))) ))
    BLM_TRY(xoico_group_s_push_func_d(o,((xoico_func_s*)bcore_fork(func)) ))
    BLM_RETURNV(er_t, 0)
}

er_t xoico_group_s_parse_func( xoico_group_s* o, x_source* source, bl_t register_in_function_manager )
{
    // xoico_group.x:384:1
    BLM_INIT_LEVEL(0);
    xoico_func_s* func = ((xoico_func_s*)BLM_LEVEL_T_PUSH(0,xoico_func_s,xoico_func_s_create()));
    BLM_TRY(xoico_func_s_parse(func,((const xoico_host*)(o)), source ))
    func->register_in_function_manager = register_in_function_manager;
    
    if( func->name == ((tp_t)(TYPEOF_group_signal_init1)) )
    {
        o->defines_group_signal_init1 = true;
    }
    
    if( func->name == ((tp_t)(TYPEOF_group_signal_down1)) )
    {
        o->defines_group_signal_down1 = true;
    }
    
    BLM_TRY(xoico_group_s_push_func_d(o,((xoico_func_s*)bcore_fork(func)) ))
    if( func->signature_global_name == ((tp_t)(TYPEOF_x_inst_main)) || func->signature_global_name == ((tp_t)(TYPEOF_x_inst_main_c)) )
    {
        BLM_TRY(xoico_target_s_set_main_function(o->xoico_source->target,func ))
    }
    BLM_RETURNV(er_t, 0)
}

er_t xoico_group_s_push_func_d( xoico_group_s* o, xoico_func_s* func )
{
    // xoico_group.x:410:1
    
    sz_t idx = xoico_funcs_s_get_index_from_name(&(o->funcs),func->name );
    
    xoico_compiler_s* compiler = o->compiler;
    
    if( idx >= 0 )
    {
        xoico_func_s* prex_func = o->funcs.data[ idx ];
        if( ( prex_func->signature_global_name == func->signature_global_name ) )
        {
            if( !func->body )
            {
                return  x_source_point_s_parse_error_fa(&(func->source_point),"Function '#<sc_t>' has already been declared.", xoico_compiler_s_nameof(compiler,func->name ) );
            }
            else if( prex_func->body )
            {
                return  x_source_point_s_parse_error_fa(&(func->source_point),"Function '#<sc_t>' has already a body.", xoico_compiler_s_nameof(compiler,func->name ) );
            }
            else
            {
                BLM_TRY(xoico_funcs_s_replace_d(&(o->funcs),idx, func ))
            }
        }
        else
        {
            return  x_source_point_s_parse_error_fa(&(func->source_point),"Function '#<sc_t>' has already been declared.", xoico_compiler_s_nameof(compiler,func->name ) );
        }
    }
    else
    {
        x_array_push_d(((x_array*)(&(o->funcs))),((x_inst*)(func )));
    }
    
    return  0;
}

er_t xoico_group_s_parse( xoico_group_s* o, const xoico_host* host, bl_t parse_block, x_source* source )
{
    // xoico_group.x:449:1
    BLM_INIT_LEVEL(0);
    xoico_compiler_s* compiler = o->compiler;
    xoico_group_source_stack_s* stack = ((xoico_group_source_stack_s*)BLM_LEVEL_T_PUSH(0,xoico_group_source_stack_s,xoico_group_source_stack_s_create()));
    x_array_push_d(((x_array*)(stack)),((x_inst*)(((x_source*)bcore_fork(source)) )));
    
    sc_t group_termination = NULL;
    
    if( !o->source_point )
    {
        x_source_point_s_setup_from_source(BCORE_PASS_CREATE(x_source_point_s,o->source_point),source );
        o->pre_hash = bcore_tp_init();
        if( !o->tp_name ) o->tp_name = xoico_compiler_s_entypeof(compiler,o->st_name.sc );
    }
    
    if( parse_block ) // this group is nested in another group, the group body is enclosed in { ... }
    {
        BLM_TRY(x_source_parse_fa(source," {" ))
        group_termination = " #?'}'";
    }
    else if( x_source_parse_bl(source," #?'#ifdef XOILA_SECTION'" ) ) // this group is root
    {
        group_termination = " #?'#endif'";
    }
    
    bl_t extend_stump = false;
    
    while
    (
        stack->size >= 2 ||
        ( group_termination ? !x_source_parse_bl(source,group_termination ) : !x_source_eos(source) )
    )
    {
        if( x_source_eos(source) )
        {
            if( stack->size >= 2 )
            {
                x_source_a_attach( &(stack->data[ stack->size - 1 ] ), (x_source*)( NULL));
                source = stack->data[ stack->size - 2 ];
                x_array_set_size(((x_array*)(stack)),stack->size - 1 );
            }
            else if( group_termination )
            {
                BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Unexpected end of file." ))
            }
            else
            {
                break;
            }
        }
        else if( x_source_parse_bl(source," #?w'stamp' " ) )
        {BLM_INIT_LEVEL(3);
            xoico_stamp_s* stamp = ((xoico_stamp_s*)BLM_LEVEL_T_PUSH(3,xoico_stamp_s,xoico_stamp_s_create()));
            stamp->group = o;
            BLM_TRY(xoico_stamp_s_parse(stamp,((const xoico_host*)(o)), source ))
            BLM_TRY(xoico_stamp_s_push_inexpandable_funcs(stamp))
            BLM_TRY(xoico_compiler_s_register_item(compiler,xoico_group_s_push_item_d(o,((xoico*)(((xoico_stamp_s*)bcore_fork(stamp)) ))) ))
        BLM_DOWN();}
    
        /// stumps are inexpandable stamps. They can be used as template.
        else if( x_source_parse_bl(source," #?w'stump' " ) )
        {BLM_INIT_LEVEL(3);
            xoico_stamp_s* stump = ((xoico_stamp_s*)BLM_LEVEL_T_PUSH(3,xoico_stamp_s,xoico_stamp_s_create()));
            stump->group = o;
            BLM_TRY(xoico_stamp_s_parse(stump,((const xoico_host*)(o)), source ))
            //stump.make_funcs_overloadable();
            BLM_TRY(xoico_compiler_s_register_item(compiler,((const xoico*)(stump ))))
            BLM_TRY(xoico_compiler_s_life_a_push(compiler,((bcore_inst*)(((xoico_stamp_s*)bcore_fork(stump)) ))))
            if( extend_stump )
            {
                o->extending_stamp = stump;
                extend_stump = false;
            }
        BLM_DOWN();}
        else if( x_source_parse_bl(source," #?w'signature' " ) )
        {BLM_INIT_LEVEL(3);
            xoico_signature_s* signature = ((xoico_signature_s*)BLM_LEVEL_T_PUSH(3,xoico_signature_s,xoico_signature_s_create()));
            BLM_TRY(xoico_signature_s_parse(signature,((const xoico_host*)(o)), source ))
            BLM_TRY(x_source_parse_fa(source," ; " ))
            BLM_TRY(xoico_compiler_s_register_item(compiler,xoico_group_s_push_item_d(o,((xoico*)(((xoico_signature_s*)bcore_fork(signature)) ))) ))
        BLM_DOWN();}
        else if( x_source_parse_bl(source," #?w'body' " ) )
        {BLM_INIT_LEVEL(3);
            xoico_body_s* body = ((xoico_body_s*)BLM_LEVEL_T_PUSH(3,xoico_body_s,xoico_body_s_create()));
            st_s body_name;BLM_T_INIT_SPUSH(st_s, &body_name);;
            BLM_TRY(xoico_group_s_parse_name_st(o,source,&( body_name )))
            if( body_name.size == 0 ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Body name expected." ))
            body->name = xoico_host_a_entypeof(host,body_name.sc );
    
            BLM_TRY(xoico_body_s_parse(body,((const xoico_host*)(o)), source ))
    
            BLM_TRY(x_source_parse_fa(source," #-?';' " )) // closing semicolon is optional
            BLM_TRY(xoico_compiler_s_register_item(compiler,xoico_group_s_push_item_d(o,((xoico*)(((xoico_body_s*)bcore_fork(body)) ))) ))
        BLM_DOWN();}
        else if( x_source_parse_bl(source," #?w'feature' " ) )
        {BLM_INIT_LEVEL(3);
            xoico_feature_s* feature = ((xoico_feature_s*)BLM_LEVEL_T_PUSH(3,xoico_feature_s,xoico_feature_s_create()));
            feature->group = o;
            BLM_TRY(xoico_feature_s_parse(feature,((const xoico_host*)(o)), source ))
            BLM_TRY(xoico_compiler_s_register_item(compiler,xoico_group_s_push_item_d(o,((xoico*)(((xoico_feature_s*)bcore_fork(feature)) ))) ))
            bcore_hmap_tpvd_s_set(&(o->hmap_feature),feature->signature.name, ( vd_t )feature );
            {const xoico_funcs_s* __a=&(feature->funcs_return_to_group );if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_func_s* func=__a->data[__i]; x_array_push_d(((x_array*)(&(o->funcs))),((x_inst*)(((xoico_func_s*)bcore_fork(func)) )));}}
            x_array_clear(((x_array*)(&(feature->funcs_return_to_group))));
        BLM_DOWN();}
        else if( x_source_parse_bl(source," #?w'func' " ) )
        {
            bl_t is_group_function = true;
            if( x_source_parse_bl(source," #=?'('" ) )
            {BLM_INIT_LEVEL(4);
                sz_t index = x_source_get_index(source);
                BLM_TRY(x_source_parse_fa(source,"(" ))
                st_s* stamp_name = ((st_s*)BLM_LEVEL_T_PUSH(4,st_s,st_s_create()));
                BLM_TRY(xoico_group_s_parse_name_st(o,source, stamp_name ))
                if( x_source_parse_bl(source," #?')'" ) )
                {
                    if( !st_s_ends_in_sc(stamp_name,"_s" ) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Stamp name '#<sc_t>' must end in '_s'.", stamp_name->sc ))
                    tp_t tp_stamp_name = btypeof( stamp_name->sc );
                    if( !xoico_compiler_s_is_stamp(compiler,tp_stamp_name ) )
                    {
                        BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Cannot associate type name '#<sc_t>' with a stamp.", stamp_name->sc ))
                    }
    
                    xoico_stamp_s* stamp = xoico_compiler_s_get_stamp(compiler,tp_stamp_name );
    
                    if( stamp->finalized )
                    {
                        BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Stamp '#<sc_t>' has already been finalized. Attempting to extend stamp outside its declarative scope.",
                            stamp_name->sc
                        ))
                    }
    
                    BLM_TRY(xoico_stamp_s_parse_func(stamp,source ))
                    o->pre_hash = bcore_tp_fold_tp( o->pre_hash, xoico_stamp_s_get_hash(stamp) );
                    is_group_function = false;
                }
                else
                {
                    x_source_set_index(source,index );
                    is_group_function = true;
                }
            BLM_DOWN();}
    
            if( is_group_function )
            {
                BLM_TRY(xoico_group_s_parse_func(o,source, true )) // group functions are registered
            }
        }
        else if( x_source_parse_bl(source," #?w'name' " ) )
        {
            while( !x_source_eos(source) )
            {BLM_INIT_LEVEL(4);
                xoico_name_s* name = ((xoico_name_s*)BLM_LEVEL_T_PUSH(4,xoico_name_s,xoico_name_s_create()));
                BLM_TRY(xoico_name_s_parse(name,((const xoico_host*)(o)), source ))
                xoico_group_s_push_item_d(o,((xoico*)(((xoico_name_s*)bcore_fork(name)) )));
                bcore_hmap_tp_s_set(&(o->compiler->hmap_declared_name),name->name );
                if( x_source_parse_bl(source," #?';'" ) ) BLM_BREAK_LEVEL(4);
                BLM_TRY(x_source_parse_fa(source," ," ))
            BLM_DOWN();}
        }
        else if( x_source_parse_bl(source," #?w'type' " ) )
        {
            while( !x_source_eos(source) )
            {BLM_INIT_LEVEL(4);
                xoico_name_s name;BLM_T_INIT_SPUSH(xoico_name_s, &name);xoico_name_s_parse(&(name),((const xoico_host*)(o)), source );
                BLM_TRY(xoico_compiler_s_register_external_type(compiler,name.name ))
                o->pre_hash = bcore_tp_fold_tp( o->pre_hash, xoico_name_s_get_hash(&(name)) );
                if( x_source_parse_bl(source," #?';'" ) ) BLM_BREAK_LEVEL(4);
                BLM_TRY(x_source_parse_fa(source," ," ))
            BLM_DOWN();}
        }
        else if( x_source_parse_bl(source," #?w'identifier' " ) )
        {
            while( !x_source_eos(source) )
            {BLM_INIT_LEVEL(4);
                xoico_name_s name;BLM_T_INIT_SPUSH(xoico_name_s, &name);xoico_name_s_parse(&(name),((const xoico_host*)(o)), source );
                BLM_TRY(xoico_compiler_s_register_external_identifier(compiler,name.name ))
                o->pre_hash = bcore_tp_fold_tp( o->pre_hash, xoico_name_s_get_hash(&(name)) );
                if( x_source_parse_bl(source," #?';'" ) ) BLM_BREAK_LEVEL(4);
                BLM_TRY(x_source_parse_fa(source," ," ))
            BLM_DOWN();}
        }
        else if( x_source_parse_bl(source," #?w'forward' " ) )
        {
            while( !x_source_eos(source) )
            {BLM_INIT_LEVEL(4);
                xoico_forward_s* forward = ((xoico_forward_s*)BLM_LEVEL_T_PUSH(4,xoico_forward_s,xoico_forward_s_create()));
                forward->group = o;
                BLM_TRY(xoico_forward_s_parse(forward,((const xoico_host*)(o)), source ))
                xoico_group_s_push_item_d(o,((xoico*)(((xoico_forward_s*)bcore_fork(forward)) )));
                if( x_source_parse_bl(source," #?';'" ) ) BLM_BREAK_LEVEL(4);
                BLM_TRY(x_source_parse_fa(source," ," ))
            BLM_DOWN();}
        }
        else if( x_source_parse_bl(source," #?w'extending'" ) )
        {
            o->extending_stamp = NULL;
            if( x_source_parse_bl(source," #=?';'" ) )
            {
                // just reset o->extending - nothing else
            }
            else if( x_source_parse_bl(source," #=?w'stump'" ) )
            {
                extend_stump = true;
            }
            else
            {BLM_INIT_LEVEL(4);
                st_s* templ_name = ((st_s*)BLM_LEVEL_T_PUSH(4,st_s,st_s_create()));
                BLM_TRY(xoico_group_s_parse_name_st(o,source, templ_name ))
                if( !st_s_ends_in_sc(templ_name,"_s" ) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Stamp name '#<sc_t>' must end in '_s'.", templ_name->sc ))
                const xoico* item = xoico_compiler_s_get_const_item(compiler,btypeof( templ_name->sc ) );
                if( !item ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Template #<sc_t> not found.", templ_name->sc ))
                if( item->_ != ((tp_t)(TYPEOF_xoico_stamp_s)) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Template #<sc_t> is no stamp.", templ_name->sc ))
                o->extending_stamp = ((xoico_stamp_s*)(item));
                BLM_TRY(x_source_parse_fa(source," ;" ))
            BLM_DOWN();}
        }
        else if( x_source_parse_bl(source," #?w'group' " ) )
        {BLM_INIT_LEVEL(3);
            st_s st_group_name;BLM_T_INIT_SPUSH(st_s, &st_group_name);;
            BLM_TRY(xoico_group_s_parse_name_st(o,source,&( st_group_name )))
            bl_t retrievable = false;
    
            // using assignment symbol is optional
            if( x_source_parse_bl(source," #?'=' " ) )
            {
                // nothing (error in case use of assignment becomes illegal)
            }
    
            // flags
            if( x_source_parse_bl(source," #?w'retrievable' " ) ) retrievable = true;
    
            xoico_transient_map_s* transient_map = ((xoico_transient_map_s*)BLM_LEVEL_T_PUSH(3,xoico_transient_map_s,xoico_transient_map_s_clone(&(o->transient_map))));
            tp_t tp_trait_name = o->tp_name;
            st_s* st_trait_name = ((st_s*)BLM_LEVEL_T_PUSH(3,st_s,st_s_create()));
    
            // first test for 'trans' used instead of trait_name
            if( x_source_parse_bl(source," #?w'trans' " ) )
            {
                BLM_TRY(xoico_transient_map_s_parse_update(transient_map,((const xoico_host*)(o)), source ))
            }
            else
            {
                BLM_TRY(xoico_group_s_parse_name_st(o,source, st_trait_name ))
                if( st_trait_name->size > 0 )
                {
                    tp_trait_name = xoico_compiler_s_entypeof(compiler,st_trait_name->sc );
                }
                else
                {
                    st_s_copy_sc(st_trait_name,xoico_compiler_s_nameof(compiler,tp_trait_name ) );
                }
    
                // second test for trans in case it occurs after trait_name
                if( x_source_parse_bl(source," #?w'trans' " ) ) BLM_TRY(xoico_transient_map_s_parse_update(transient_map,((const xoico_host*)(o)), source ))
            }
    
            xoico_group_s* group = NULL;
            BLM_TRY(xoico_source_s_get_group_if_preexsting(o->xoico_source,host, source, st_group_name.sc, st_trait_name->sc, (&(group)) ))
    
            if( !group )
            {BLM_INIT_LEVEL(4);
                group = xoico_source_s_push_d(o->xoico_source,xoico_group_s_create() );
                group->lexical_parent = o;
                group->xoico_source = o->xoico_source;
                group->compiler = o->compiler;
                group->extending_stamp = o->extending_stamp;
                group->expandable = o->expandable;
                xoico_group_s_set_name_sc(group,host, st_group_name.sc );
                group->is_retrievable = retrievable;
                group->trait_name = tp_trait_name;
                xoico_transient_map_s_copy(&(group->transient_map),transient_map );
                BLM_TRY(xoico_group_s_parse(group,((const xoico_host*)(o)), true, source ))
                BLM_TRY(xoico_compiler_s_register_group(compiler,group ))
                xoico_nested_group_s* nested_group = ((xoico_nested_group_s*)BLM_LEVEL_T_PUSH(4,xoico_nested_group_s,xoico_nested_group_s_create()));
                nested_group->group = group;
                xoico_group_s_push_item_d(o,((xoico*)(((xoico_nested_group_s*)bcore_fork(nested_group)) )));
            BLM_DOWN();}
            else
            {
                xoico_transient_map_s_update(&(group->transient_map),transient_map );
                BLM_TRY(xoico_group_s_parse(group,((const xoico_host*)(o)), true, source ))
            }
    
            BLM_TRY(x_source_parse_fa(source," #-?';' " )) // closing semicolon is optional
        BLM_DOWN();}
        else if( x_source_parse_bl(source," #?w'set' " ) )
        {
            if     ( x_source_parse_bl(source," #?w'retrievable' "      ) ) o->is_retrievable = true;
            else if( x_source_parse_bl(source," #?w'inexpandable' "     ) ) o->expandable = false;
            else if( x_source_parse_bl(source," #?w'short_spect_name' " ) ) o->short_spect_name = true;
            else if( x_source_parse_bl(source," #?w'beta' "             ) ) BLM_TRY(x_source_parse_fa(source," = #<tp_t*>", (&(o->beta)) ))
            BLM_TRY(x_source_parse_fa(source," ;" ))
        }
        else if( x_source_parse_bl(source," #?w'embed' " ) )
        {BLM_INIT_LEVEL(3);
            st_s* folder = ((st_s*)BLM_LEVEL_T_PUSH(3,st_s,bcore_file_folder_path(x_source_get_file(source) )));
            if( folder->size == 0 ) st_s_push_char(folder,'.' );
            st_s* embed_file = ((st_s*)BLM_LEVEL_T_PUSH(3,st_s,st_s_create()));
            BLM_TRY(x_source_parse_fa(source," #string" , embed_file ))
            BLM_TRY(x_source_parse_fa(source," ;" ))
            bcore_arr_st_s_push_st(&(o->explicit_embeddings),embed_file );
            x_source* embed_source = NULL;
            BLM_TRY(xoico_embed_file_open(source, embed_file->sc, (&(embed_source)) ) )
    
            // check for cyclic inclusions
            {const xoico_group_source_stack_s* __a=stack ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){x_source* e=__a->data[__i];
            {
                sc_t path = x_source_get_file(embed_source);
                if( sc_t_equal( path, x_source_get_file(e) ) )
                {
                    BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Xoico: Cyclic inclusion." ))
                }
            }
    
            }}x_array_push_d(((x_array*)(stack)),((x_inst*)(embed_source )));
            source = embed_source;
        BLM_DOWN();}
        else if( x_source_parse_bl(source," #?w'include' " ) )
        {BLM_INIT_LEVEL(3);
            bl_t in_definition = false;
            if( x_source_parse_bl(source," #?([0]==''')" ) )
            {
                char c = 0;
                BLM_TRY(x_source_parse_fa(source,"'#<char*>'", &c ))
                if     ( c == 'c' ) in_definition = true;
                else if( c == 'h' ) in_definition = false;
                else BLM_RETURNV(er_t, x_source_parse_error_fa(source,"include: 'c' or 'h' expected." ))
            }
            else if( x_source_parse_bl(source," #?w'deferred' " ) )
            {
                BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Syntax \"include deferred\" is deprecated. Use \"include 'c'\" instead." ))
            }
    
            st_s* include_file = ((st_s*)BLM_LEVEL_T_PUSH(3,st_s,st_s_create()));
    
            if( x_source_parse_bl(source," #?'\"'" ) )
            {
                st_s_push_char(include_file,'"' );
                BLM_TRY(x_source_parse_fa(source,"#:until'\"'\"", include_file ))
                st_s_push_char(include_file,'"' );
            }
            else if( x_source_parse_bl(source," #?'<'" ) )
            {
                st_s_push_char(include_file,'<' );
                BLM_TRY(x_source_parse_fa(source,"#:until'>'>", include_file ))
                st_s_push_char(include_file,'>' );
            }
            else
            {
                BLM_RETURNV(er_t, x_source_parse_error_fa(source,"'include <filepath>' or 'include \"filepath\"' expected." ))
            }
            BLM_TRY(x_source_parse_fa(source," ;" ))
    
            if( in_definition )
            {
                bcore_arr_st_s_push_st(&(o->includes_in_definition),include_file );
            }
            else
            {
                bcore_arr_st_s_push_st(&(o->includes_in_declaration),include_file );
            }
        BLM_DOWN();}
        else if( x_source_parse_bl(source," #?w'trans' " ) )
        {
            BLM_TRY(xoico_transient_map_s_parse_update(&(o->transient_map),((const xoico_host*)(o)), source ))
            BLM_TRY(x_source_parse_fa(source," ;" ))
        }
        else
        {
            BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Xoico: syntax error." ))
        }
    
        BLM_TRY(x_source_parse_fa(source," " ))// consume whitespaces
    }
    
    if( stack->size > 1 )
    {
        BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Xoico: Unexpected end of group reached." ))
    }
    
    BLM_RETURNV(er_t, 0)
}

er_t xoico_group_s_parse_embed( xoico_group_s* o, const xoico_host* host, x_source* source, tp_t embed_method )
{
    // xoico_group.x:836:1
    
    if( embed_method == TYPEOF_as_string )
    {BLM_INIT_LEVEL(1);
        xoico_embedded_data_s* embedded_data = ((xoico_embedded_data_s*)BLM_LEVEL_T_PUSH(1,xoico_embedded_data_s,xoico_embedded_data_s_create()));
    
        embedded_data->group = o;
        embedded_data->embed_method = embed_method;
    
        const st_s* full_function_name = st_s_copy_fa(((st_s*)BLM_LEVEL_T_PUSH(1,st_s,st_s_create())),"#<sc_t>_as_string", o->st_name.sc );
    
        embedded_data->function_name = xoico_host_a_entypeof(host,full_function_name->sc );
        embedded_data->embed_method = embed_method;
        BLM_TRY(xoico_embedded_data_s_parse(embedded_data,((const xoico_host*)(o)), source ))
    
        xoico_group_s_push_item_d(o,((xoico*)(((xoico_embedded_data_s*)bcore_fork(embedded_data)) )));
    
        BLM_TRY(xoico_group_s_push_func_from_sc(o,st_s_copy_fa(((st_s*)BLM_LEVEL_T_PUSH(1,st_s,st_s_create())),"d st_s* as_string();" )->sc ))
    BLM_DOWN();}
    else
    {
        BLM_TRY(GERR_fa("Invalid embed_method." ))
    }
    return  0;
}

er_t xoico_group_s_finalize( xoico_group_s* o, const xoico_host* host )
{
    // xoico_group.x:864:1
    
    /// default features
    BLM_TRY(xoico_group_s_push_inexpandable_feature_from_sc(o,"d @* clone( c @* o );" ))
    BLM_TRY(xoico_group_s_push_inexpandable_feature_from_sc(o,"void copy( m @* o, c @* src );" ))
    BLM_TRY(xoico_group_s_push_inexpandable_feature_from_sc(o,"void discard( m @* o );" ))
    BLM_TRY(xoico_group_s_push_inexpandable_func_from_sc(o,"d obliv @* t_create( tp_t t );" ))
    
    // check validity of trait name
    if( !xoico_compiler_s_is_group(xoico_host_a_compiler(host),o->trait_name ) )
    {
        return  x_source_point_s_parse_error_fa(o->source_point,"In group '#<sc_t>': Trait name '#<sc_t>' is not a group.", o->st_name.sc, xoico_host_a_nameof(host,o->trait_name ) );
    }
    
    {const xoico_group_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico* e=__a->data[__i]; BLM_TRY(xoico_a_finalize(e,((const xoico_host*)(o ))))
    }}{const xoico_funcs_s* __a=&(o->funcs );if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_func_s* func=__a->data[__i];
    {
        BLM_TRY(xoico_func_s_finalize(func,((const xoico_host*)(o ))))
        BLM_TRY(xoico_compiler_s_register_func(o->compiler,func ))
        bcore_hmap_tpvd_s_set(&(o->hmap_func),func->name, ( vd_t )func );
    }
    }}return  0;
}

er_t xoico_group_s_expand_forward( const xoico_group_s* o, sz_t indent, x_sink* sink )
{
    // xoico_group.x:890:1
    
    if( !o->expandable ) return  0;
    x_sink_push_fa(sink," \\\n#rn{ }BCORE_FORWARD_OBJECT( #<sc_t> );", indent, o->st_name.sc );
    {const xoico_group_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico* e=__a->data[__i]; BLM_TRY(xoico_a_expand_forward(e,((const xoico_host*)(o)), indent, sink ))
    }}{const xoico_funcs_s* __a=&(o->funcs );if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_func_s* func=__a->data[__i]; BLM_TRY(xoico_func_s_expand_forward(func,((const xoico_host*)(o)), indent, sink ))
    }}return  0;
}

er_t xoico_group_s_expand_spect_declaration( const xoico_group_s* o, sz_t indent, x_sink* sink )
{
    // xoico_group.x:901:1
    
    if( !o->expandable ) return  0;
    if( o->short_spect_name )
    {
        x_sink_push_fa(sink," \\\n#rn{ }BCORE_DECLARE_SPECT( #<sc_t> )", indent, o->st_name.sc );
    }
    else
    {
        x_sink_push_fa(sink," \\\n#rn{ }XOILA_DECLARE_SPECT( #<sc_t> )", indent, o->st_name.sc );
    }
    
    x_sink_push_fa(sink," \\\n#rn{ }{", indent );
    x_sink_push_fa(sink," \\\n#rn{ }    bcore_spect_header_s header;", indent );
    {const xoico_group_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico* e=__a->data[__i]; BLM_TRY(xoico_a_expand_spect_declaration(e,((const xoico_host*)(o)), indent + 4, sink ))
    }}x_sink_push_fa(sink," \\\n#rn{ }};", indent );
    
    x_sink_push_fa(sink," \\\n#rn{ }"
        "BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( #<sc_t> )",
        indent,
        o->st_name.sc
    );
    
    return  0;
}

er_t xoico_group_s_expand_declaration( const xoico_group_s* o, sz_t indent, x_sink* sink )
{
    // xoico_group.x:931:1
    BLM_INIT_LEVEL(0);
    if( !o->expandable ) BLM_RETURNV(er_t, 0)
    
    x_sink_push_fa(sink,"\n" );
    x_sink_push_fa(sink,"#rn{ }//#rn{-}\n", indent, sz_max( 0, 118 - indent ) );
    x_sink_push_fa(sink,"#rn{ }// group: #<sc_t>", indent, o->st_name.sc );
    if( o->explicit_embeddings.size > 0 )
    {
        x_sink_push_fa(sink,"; embeds:" );
        {const bcore_arr_st_s* __a=&(o->explicit_embeddings );if(__a)for(sz_t __i=0;__i<__a->size;__i++){st_s* st=__a->data[__i]; x_sink_push_fa(sink," #<st_s*>", st );}}
    }
    x_sink_push_fa(sink,"\n" );
    
    {const bcore_arr_st_s* __a=&(o->includes_in_declaration );if(__a)for(sz_t __i=0;__i<__a->size;__i++){st_s* e=__a->data[__i]; x_sink_push_fa(sink,"##include #<sc_t>\n", e->sc );}}
    
    x_sink_push_fa(sink,"\n" );
    x_sink_push_fa(sink,"#rn{ }##define TYPEOF_#<sc_t> 0x#pl16'0'{#X<tp_t>}ull\n", indent, o->st_name.sc, btypeof( o->st_name.sc ) );
    
    st_s* st_spect_name = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,xoico_group_s_create_spect_name( o )));
    sc_t sc_spect_name = st_spect_name->sc;
    
    x_sink_push_fa(sink,"#rn{ }##define TYPEOF_#<sc_t> 0x#pl16'0'{#X<tp_t>}ull\n", indent, sc_spect_name, btypeof( sc_spect_name ) );
    
    {const xoico_group_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico* e=__a->data[__i]; BLM_TRY(xoico_a_expand_declaration(e,((const xoico_host*)(o)), indent, sink ))
    }}x_sink_push_fa(sink,"#rn{ }##define BETH_EXPAND_GROUP_#<sc_t>", indent, o->st_name.sc );
    
    BLM_TRY(xoico_group_s_expand_forward(o,indent + 2, sink ))
    
    {const xoico_group_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico* e=__a->data[__i]; BLM_TRY(xoico_a_expand_indef_typedef(e,((const xoico_host*)(o)), indent, sink ))
    }}BLM_TRY(xoico_group_s_expand_spect_declaration(o,indent + 2, sink ))
    
    {const xoico_group_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico* e=__a->data[__i]; BLM_TRY(xoico_a_expand_indef_declaration(e,((const xoico_host*)(o)), indent, sink ))
    }}{const xoico_funcs_s* __a=&(o->funcs );if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_func_s* func=__a->data[__i]; BLM_TRY(xoico_func_s_expand_declaration(func,((const xoico_host*)(o)), indent + 2, sink ))
    }}x_sink_push_fa(sink,"\n" );
    
    BLM_RETURNV(er_t, 0)
}

er_t xoico_group_s_expand_spect_definition( const xoico_group_s* o, sz_t indent, x_sink* sink )
{
    // xoico_group.x:972:1
    
    xoico_compiler_s* compiler = o->compiler;
    if( !o->expandable ) return  0;
    x_sink_push_fa(sink,"\n" );
    if( o->short_spect_name )
    {
        x_sink_push_fa(sink,"#rn{ }BCORE_DEFINE_SPECT( #<sc_t>, #<sc_t> )\n", indent, xoico_compiler_s_nameof(compiler,o->trait_name ), o->st_name.sc );
    }
    else
    {
        x_sink_push_fa(sink,"#rn{ }XOILA_DEFINE_SPECT( #<sc_t>, #<sc_t> )\n", indent, xoico_compiler_s_nameof(compiler,o->trait_name ), o->st_name.sc );
    }
    
    x_sink_push_fa(sink,"#rn{ }\"{\"\n", indent );
    x_sink_push_fa(sink,"#rn{ }    \"bcore_spect_header_s header;\"\n", indent );
    {const xoico_group_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico* e=__a->data[__i]; BLM_TRY(xoico_a_expand_spect_definition(e,((const xoico_host*)(o)), indent + 4, sink ))
    }}x_sink_push_fa(sink,"#rn{ }\"}\";\n", indent );
    return  0;
}

er_t xoico_group_s_expand_definition( const xoico_group_s* o, sz_t indent, x_sink* sink )
{
    // xoico_group.x:995:1
    
    if( !o->expandable ) return  0;
    x_sink_push_fa(sink,"\n" );
    x_sink_push_fa(sink,"#rn{ }//#rn{-}\n", indent, sz_max( 0, 118 - indent ) );
    x_sink_push_fa(sink,"#rn{ }// group: #<sc_t>", indent, o->st_name.sc );
    if( o->explicit_embeddings.size > 0 )
    {
        x_sink_push_fa(sink,"; embeds:" );
        {const bcore_arr_st_s* __a=&(o->explicit_embeddings );if(__a)for(sz_t __i=0;__i<__a->size;__i++){st_s* st=__a->data[__i]; x_sink_push_fa(sink," #<st_s*>", st );}}
    }
    x_sink_push_fa(sink,"\n" );
    
    {const bcore_arr_st_s* __a=&(o->includes_in_definition );if(__a)for(sz_t __i=0;__i<__a->size;__i++){st_s* e=__a->data[__i]; x_sink_push_fa(sink,"##include #<sc_t>\n", e->sc );}}
    
    // non-features
    {const xoico_group_s* __a=o;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico* e=__a->data[__i];if(e->_ != ((tp_t)(TYPEOF_xoico_feature_s)) ){ BLM_TRY(xoico_a_expand_definition(e,((const xoico_host*)(o)), indent, sink ))
    
    }}}BLM_TRY(xoico_group_s_expand_spect_definition(o,indent, sink ))
    
    // just-features
    {const xoico_group_s* __a=o;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico* e=__a->data[__i];if(e->_ == ((tp_t)(TYPEOF_xoico_feature_s)) ){ BLM_TRY(xoico_a_expand_definition(e,((const xoico_host*)(o)), indent, sink ))
    
    }}}{const xoico_funcs_s* __a=&(o->funcs );if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_func_s* func=__a->data[__i]; BLM_TRY(xoico_func_s_expand_definition(func,((const xoico_host*)(o)), indent, sink ))
    
    }}return  0;
}

er_t xoico_group_s_expand_init1( const xoico_group_s* o, sz_t indent, x_sink* sink )
{
    // xoico_group.x:1025:1
    
    if( !o->expandable ) return  0;
    x_sink_push_fa(sink,"\n" );
    x_sink_push_fa(sink,"#rn{ }// group: #<sc_t>\n", indent, o->st_name.sc );
    {const xoico_group_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico* e=__a->data[__i]; BLM_TRY(xoico_a_expand_init1(e,((const xoico_host*)(o)), indent, sink ))
    }}{const xoico_funcs_s* __a=&(o->funcs );if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_func_s* func=__a->data[__i]; BLM_TRY(xoico_func_s_expand_init1(func,((const xoico_host*)(o)), indent, sink ))
    
    }}if( o->short_spect_name )
    {
        x_sink_push_fa(sink,"#rn{ }BCORE_REGISTER_SPECT( #<sc_t> );\n", indent, o->st_name.sc );
    }
    else
    {
        x_sink_push_fa(sink,"#rn{ }XOILA_REGISTER_SPECT( #<sc_t> );\n", indent, o->st_name.sc );
    }
    
    if( o->is_retrievable )
    {
        x_sink_push_fa(sink,"#rn{ }{\n", indent );
        x_sink_push_fa(sink,"#rn{ }   bcore_arr_tp_s* arr = bcore_arr_tp_s_create();\n", indent );
        {const bcore_arr_tp_s* __a=o->retrievable_stamps ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){tp_t type=__a->data[__i];
        {
            x_sink_push_fa(sink,"#rn{ }   bcore_arr_tp_s_push( arr, TYPEOF_#<sc_t> );\n", indent, xoico_compiler_s_nameof(o->compiler,type ) );
        }
        }}x_sink_push_fa(sink,"#rn{ }   bcore_xoila_set_arr_traitline_stamps_d( TYPEOF_#<sc_t>, arr );\n", indent, xoico_compiler_s_nameof(o->compiler,o->tp_name ) );
        x_sink_push_fa(sink,"#rn{ }}\n", indent );
    }
    
    
    if( o->defines_group_signal_init1 )
    {
        x_sink_push_fa(sink,"#rn{ }#<sc_t>_group_signal_init1();\n", indent, o->st_name.sc );
    }
    
    return  0;
}

er_t xoico_group_s_expand_down1( const xoico_group_s* o, sz_t indent, x_sink* sink )
{
    // xoico_group.x:1065:1
    
    if( !o->expandable ) return  0;
    if( o->defines_group_signal_down1 )
    {
        x_sink_push_fa(sink,"#rn{ }#<sc_t>_group_signal_down1();\n", indent, o->st_name.sc );
    }
    
    return  0;
}

BCORE_DEFINE_OBJECT_INST_P( xoico_nested_group_s )
"aware xoico_group"
"{"
    "hidden xoico_group_s* group;"
    "func xoico:get_hash;"
    "func xoico:expand_forward;"
    "func xoico:expand_indef_declaration;"
    "func xoico:get_source_point;"
"}";

tp_t xoico_nested_group_s_get_hash( const xoico_nested_group_s* o )
{
    // xoico_group.x:215:5
    
    return  o->group ? xoico_group_s_get_hash(o->group) : 0;
}

er_t xoico_nested_group_s_expand_forward( const xoico_nested_group_s* o, const xoico_host* host, sz_t indent, x_sink* sink )
{
    // xoico_group.x:220:5
    
    if( !o->group->expandable ) return  0;
    x_sink_push_fa(sink," \\\n#rn{ }BCORE_FORWARD_OBJECT( #<sc_t> );", indent, o->group->st_name.sc );
    return  0;
}

er_t xoico_nested_group_s_expand_indef_declaration( const xoico_nested_group_s* o, const xoico_host* host, sz_t indent, x_sink* sink )
{
    // xoico_group.x:227:5
    
    if( !o->group->expandable ) return  0;
    x_sink_push_fa(sink," \\\n#rn{ }  BETH_EXPAND_GROUP_#<sc_t>", indent, o->group->st_name.sc );
    return  0;
}

XOILA_DEFINE_SPECT( xoico, xoico_group )
"{"
    "bcore_spect_header_s header;"
"}";

/**********************************************************************************************************************/
// source: xoico_wrap.x

//----------------------------------------------------------------------------------------------------------------------
// group: xoico_wrap

BCORE_DEFINE_OBJECT_INST_P( xoico_wrap_s )
"aware xoico_wrap"
"{"
    "tp_t group_name;"
    "tp_t func_name;"
    "x_source_point_s source_point;"
    "xoico_signature_s => signature;"
    "xoico_func_s => func;"
    "func xoico:parse;"
    "func xoico:finalize;"
    "func xoico:get_source_point;"
"}";

er_t xoico_wrap_s_parse( xoico_wrap_s* o, const xoico_host* host, x_source* source )
{
    // xoico_wrap.x:30:5
    BLM_INIT_LEVEL(0);
    BLM_TRY(x_source_parse_fa(source," " ))
    BLM_TRY(xoico_host_a_parse_name_tp(host,source, (&(o->group_name)) ))
    st_s* st = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
    BLM_TRY(x_source_parse_fa(source," . #name ", st ))
    if( st->size == 0 ) BLM_TRY(x_source_parse_error_fa(source,"Function name expected." ))
    o->func_name = xoico_host_a_entypeof(host,st->sc );
    BLM_TRY(x_source_parse_fa(source,";" ))
    BLM_RETURNV(er_t, 0)
}

er_t xoico_wrap_s_finalize( xoico_wrap_s* o, const xoico_host* host )
{
    // xoico_wrap.x:42:5
    BLM_INIT_LEVEL(0);
    xoico_compiler_s* compiler = xoico_host_a_compiler(host);
    const xoico_group_s* group = xoico_compiler_s_get_group(compiler,o->group_name );
    if( !group ) BLM_TRY(x_source_point_s_parse_error_fa(&(o->source_point),"Wrap: '#<sc_t>' is not a group.", xoico_compiler_s_nameof(compiler,o->group_name ) ))
    const xoico_func_s* func = xoico_group_s_get_func(group,o->func_name );
    if( !func ) BLM_TRY(x_source_point_s_parse_error_fa(&(o->source_point),"Wrap: '#<sc_t>' is not a function in group '#<sc_t>'.", xoico_compiler_s_nameof(compiler,o->func_name ), xoico_compiler_s_nameof(compiler,o->group_name ) ))
    
    xoico_signature_s_attach( &(o->signature ),  xoico_signature_s_clone(xoico_compiler_s_get_signature(compiler,func->signature->global_name )));
    BLM_TRY(xoico_signature_s_set_global_name(o->signature,host ))
    BLM_TRY(xoico_signature_s_convert_transient_types(o->signature,host, xoico_host_a_transient_map(host) ))
    BLM_TRY(xoico_compiler_s_register_item(compiler,((const xoico*)(o->signature ))))
    
    BCORE_PASS_CREATE(xoico_func_s,o->func);
    o->func->name = o->signature->name;
    o->func->signature_global_name = o->signature->global_name;
    
    st_s* st = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
    st_s_push_fa(st,"{" );
    if( xoico_signature_s_returns_a_value(o->signature) ) st_s_push_sc(st,"return " );
    
    st_s_push_fa(st,"#<sc_t>_#<sc_t>(", xoico_host_a_nameof(host,group->tp_name ), xoico_host_a_nameof(host,func->name ) );
    
    if( o->signature->arg_o )
    {
        BLM_TRY(xoico_arg_s_expand_name(o->signature->arg_o,host,((x_sink*)( st ))))
        BLM_TRY(xoico_args_s_expand_name(&(o->signature->args),host, false,((x_sink*)( st ))))
    }
    else
    {
        BLM_TRY(xoico_args_s_expand_name(&(o->signature->args),host, true,((x_sink*)( st ))))
    }
    
    st_s_push_fa(st,");};" );
    
    BCORE_PASS_CREATE(xoico_body_s,o->func->body);
    BLM_TRY(xoico_body_s_parse(o->func->body,host, ((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_sc(st->sc ))) ))
    
    BLM_RETURNV(er_t, 0)
}

BCORE_DEFINE_OBJECT_INST_P( xoico_wraps_s )
"aware x_array"
"{"
    "xoico_wrap_s => [];"
"}";

XOILA_DEFINE_SPECT( xoico, xoico_wrap )
"{"
    "bcore_spect_header_s header;"
"}";

/**********************************************************************************************************************/
// source: xoico_stamp.x

//----------------------------------------------------------------------------------------------------------------------
// group: xoico_stamp

BCORE_DEFINE_OBJECT_INST_P( xoico_stamp_s )
"aware xoico_stamp"
"{"
    "st_s st_name;"
    "tp_t tp_name;"
    "tp_t trait_name;"
    "bl_t is_aware = true;"
    "bl_t has_compact_initializer = false;"
    "bl_t finalized;"
    "xoico_stamp_arr_self_item_s => arr_self_item;"
    "st_s => self_source;"
    "bcore_self_s => self;"
    "xoico_funcs_s funcs;"
    "xoico_wraps_s wraps;"
    "xoico_transient_map_s transient_map;"
    "private aware xoico_group_s* group;"
    "x_source_point_s source_point;"
    "func xoico:get_hash;"
    "func xoico:get_global_name_tp;"
    "func xoico:expand_setup;"
    "func xoico:expand_forward;"
    "func xoico:expand_indef_declaration;"
    "func xoico_host:parse_name_st;"
    "func xoico_host:compiler;"
    "func xoico_host:cengine;"
    "func xoico_host:obj_type;"
    "func xoico_host:transient_map;"
    "func xoico:get_source_point;"
    "func xoico:parse;"
    "func xoico:finalize;"
    "func xoico:expand_declaration;"
    "func xoico:expand_definition;"
    "func xoico:expand_init1;"
"}";

tp_t xoico_stamp_s_get_hash( const xoico_stamp_s* o )
{
    // xoico_stamp.x:54:5
    
    tp_t hash = bcore_tp_fold_tp( bcore_tp_init(), o->_ );
    hash = bcore_tp_fold_sc( hash, o->st_name.sc );
    hash = bcore_tp_fold_sc( hash, o->self_source ? o->self_source->sc : "" );
    hash = bcore_tp_fold_tp( hash, xoico_funcs_s_get_hash(&(o->funcs)) );
    return  hash;
}

er_t xoico_stamp_s_expand_forward( const xoico_stamp_s* o, const xoico_host* host, sz_t indent, x_sink* sink )
{
    // xoico_stamp.x:68:5
    
    x_sink_push_fa(sink," \\\n#rn{ }BCORE_FORWARD_OBJECT( #<sc_t> );", indent, o->st_name.sc );
    return  0;
}

er_t xoico_stamp_s_expand_indef_declaration( const xoico_stamp_s* o, const xoico_host* host, sz_t indent, x_sink* sink )
{
    // xoico_stamp.x:74:5
    
    x_sink_push_fa(sink," \\\n#rn{ }  BETH_EXPAND_ITEM_#<sc_t>", indent, o->st_name.sc );
    return  0;
}

er_t xoico_stamp_s_make_funcs_overloadable( xoico_stamp_s* o )
{
    // xoico_stamp.x:87:5
    
    {const xoico_funcs_s* __a=&(o->funcs );if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_func_s* func=__a->data[__i]; func->overloadable = true;}}
    return  0;
}

const xoico_func_s* xoico_stamp_s_get_traitline_func_from_name( const xoico_stamp_s* o, tp_t name )
{
    // xoico_stamp.x:95:5
    
    const xoico_func_s* func = xoico_funcs_s_get_func_from_name(&(o->funcs),name );
    if( !func )
    {
        func = xoico_group_s_get_traitline_func_from_name(xoico_compiler_s_get_group(o->group->compiler,o->trait_name ),name );
    }
    return  func;
}

const xoico_feature_s* xoico_stamp_s_get_traitline_feature_from_name( const xoico_stamp_s* o, tp_t name )
{
    // xoico_stamp.x:105:5
    
    return  xoico_group_s_get_traitline_feature_from_name(xoico_compiler_s_get_group(o->group->compiler,o->trait_name ),name );
}

const xoico_func_s* xoico_stamp_s_get_traitline_member_func_from_name( const xoico_stamp_s* o, tp_t name )
{
    // xoico_stamp.x:110:5
    
    const xoico_func_s* func = xoico_funcs_s_get_func_from_name(&(o->funcs),name );
    if( !func )
    {
        func = xoico_group_s_get_traitline_member_func_from_name(xoico_compiler_s_get_group(o->group->compiler,o->trait_name ),name );
    }
    return  func;
}

sc_t xoico_stamp_s_get_rel_name_sc( const xoico_stamp_s* o )
{
    // xoico_stamp.x:298:1
    
    sc_t group_name = o->group->st_name.sc;
    sc_t stamp_name = o->st_name.sc;
    
    sz_t i = 0;
    while( group_name[ i ] == stamp_name[ i ] && group_name[ i ] != 0 ) i++;
    if( group_name[ i ] == 0 && stamp_name[ i ] == '_' ) i++;
    
    return  stamp_name + i;
}

er_t xoico_stamp_s_parse_func( xoico_stamp_s* o, x_source* source )
{
    // xoico_stamp.x:312:1
    BLM_INIT_LEVEL(0);
    xoico_compiler_s* compiler = o->group->compiler;
    xoico_func_s* func = ((xoico_func_s*)BLM_LEVEL_T_PUSH(0,xoico_func_s,xoico_func_s_create()));
    BLM_TRY(xoico_func_s_parse(func,((const xoico_host*)(o)), source ))
    
    sz_t idx = xoico_funcs_s_get_index_from_name(&(o->funcs),func->name );
    
    if( idx >= 0 )
    {
        xoico_func_s* prex_func = o->funcs.data[ idx ];
        if( ( prex_func->signature_global_name == func->signature_global_name ) )
        {
            if( !func->body )
            {
                BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Function '#<sc_t>' has already been declared.", xoico_compiler_s_nameof(compiler,func->name ) ))
            }
            else if( prex_func->body )
            {
                BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(&(func->source_point),"Function '#<sc_t>' has already a body.", xoico_compiler_s_nameof(compiler,func->name ) ))
            }
            else
            {
                BLM_TRY(xoico_funcs_s_replace_d(&(o->funcs),idx, ((xoico_func_s*)bcore_fork(func)) ))
            }
        }
        else if( prex_func->overloadable )
        {
            BLM_TRY(xoico_funcs_s_replace_d(&(o->funcs),idx, ((xoico_func_s*)bcore_fork(func)) ))
        }
        else
        {
            BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(&(func->source_point),"Function '#<sc_t>' has already been declared and is not overloadable.", xoico_compiler_s_nameof(compiler,func->name ) ))
        }
    }
    else
    {
        x_array_push_d(((x_array*)(&(o->funcs))),((x_inst*)(((xoico_func_s*)bcore_fork(func)) )));
    }
    
    if( func->signature_global_name == ((tp_t)(TYPEOF_x_inst_main)) ) BLM_TRY(xoico_target_s_set_main_function(o->group->xoico_source->target,func ))
    
    BLM_RETURNV(er_t, 0)
}

er_t xoico_stamp_s_parse_wrap( xoico_stamp_s* o, x_source* source )
{
    // xoico_stamp.x:359:1
    BLM_INIT_LEVEL(0);
    xoico_wrap_s* wrap = ((xoico_wrap_s*)BLM_LEVEL_T_PUSH(0,xoico_wrap_s,xoico_wrap_s_create()));
    BLM_TRY(xoico_wrap_s_parse(wrap,((const xoico_host*)(o)), source ))
    x_array_push_d(((x_array*)(&(o->wraps))),((x_inst*)(((xoico_wrap_s*)bcore_fork(wrap)) )));
    BLM_RETURNV(er_t, 0)
}

er_t xoico_stamp_s_parse_extend( xoico_stamp_s* o, x_source* source )
{
    // xoico_stamp.x:369:1
    BLM_INIT_LEVEL(0);
    st_s* buf = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
    bcore_self_s* self = ((bcore_self_s*)BLM_LEVEL_T_PUSH(0,bcore_self_s,bcore_self_s_create()));
    self->type = o->tp_name;
    self->trait = o->trait_name;
    
    if( x_source_parse_bl(source," #?'{'" ) )
    {
        while( !x_source_eos(source) && !x_source_parse_bl(source," #?'}'" ) )
        {
            if( x_source_parse_bl(source," #?w'func'" ) )
            {
                BLM_TRY(xoico_stamp_s_parse_func(o,source ))
            }
            else if( x_source_parse_bl(source," #?w'wrap'" ) )
            {
                BLM_TRY(xoico_stamp_s_parse_wrap(o,source ))
            }
            else
            {BLM_INIT_LEVEL(3);
                bl_t exit = false;
    
                st_s_clear(buf);
                bl_t arg_of_initializer = false;
    
                while( !exit && !x_source_eos(source) )
                {
                    u0_t c =  x_source_get_u0(source);
                    switch( c )
                    {
                        case ':':
                        {BLM_INIT_LEVEL(7);
                            st_s* name = ((st_s*)BLM_LEVEL_T_PUSH(7,st_s,st_s_create()));
                            BLM_TRY(xoico_group_s_parse_name_recursive(o->group,source, name ))
                            st_s_push_st(buf,name );
                        BLM_DOWN();}
                        break;
    
                        case '$':
                        {
                            arg_of_initializer = true;
                        }
                        break;
    
                        case '@':
                        {
                            st_s_push_st(buf,&(o->st_name ));
                        }
                        break;
    
                        case ';':
                        {
                            st_s_push_char(buf,c );
                            exit = true;
                        }
                        break;
    
                        case '"': // string literal
                        {
                            st_s_push_char(buf,c );
                            while( !x_source_eos(source) && ((c = x_source_get_char(source)) != '"') )
                            {
                                st_s_push_char(buf,c );
                                if( c == '\\' ) st_s_push_char(buf,x_source_get_char(source) );
                                if( c == '\n' ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Newline in string literal." ))
                            }
                            if( x_source_eos(source) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"End of file in string literal." ))
                            st_s_push_char(buf,c );
                        }
                        break;
    
                        default:
                        {
                            st_s_push_char(buf,c );
                        }
                        break;
                    }
                }
    
                if( arg_of_initializer ) o->has_compact_initializer = true;
    
                xoico_stamp_self_item_s* self_item = ((xoico_stamp_self_item_s*)BLM_LEVEL_T_PUSH(3,xoico_stamp_self_item_s,xoico_stamp_self_item_s_create()));
                self_item->arg_of_initializer = arg_of_initializer;
                st_s_copy(&(self_item->st),buf );
                x_source_point_s_setup_from_source(&(self_item->source_point),source );
                x_array_push_d(((x_array*)(BCORE_PASS_CREATE(xoico_stamp_arr_self_item_s,o->arr_self_item))),((x_inst*)(((xoico_stamp_self_item_s*)bcore_fork(self_item)) )));
            BLM_DOWN();}
        }
    }
    else
    {
        if( x_source_parse_bl(source," #?w'func'" ) )
        {
            BLM_TRY(xoico_stamp_s_parse_func(o,source ))
        }
        else  // a non-block must close with semicolon
        {
            BLM_TRY(x_source_parse_fa(source," ;" ))
        }
    }
    
    BLM_TRY(x_source_parse_fa(source," #-?';' " )) // closing semicolon is optional
    
    BLM_RETURNV(er_t, 0)
}

er_t xoico_stamp_s_push_inexpandable_func_from_sc( xoico_stamp_s* o, sc_t sc )
{
    // xoico_stamp.x:478:1
    BLM_INIT_LEVEL(0);
    xoico_compiler_s* compiler = o->group->compiler;
    xoico_func_s* func = ((xoico_func_s*)BLM_LEVEL_T_PUSH(0,xoico_func_s,xoico_func_s_create()));
    func->overloadable = false;
    func->expandable = false;
    
    BLM_TRY(xoico_parse_sc(((xoico*)(func)),((const xoico_host*)(o)), sc ))
    
    sz_t idx = xoico_funcs_s_get_index_from_signature_global_name(&(o->funcs),func->signature_global_name );
    
    if( idx >= 0 )
    {
        BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(&(o->source_point),"Function '#<sc_t>' conflicts with a default function for this stamp.", xoico_compiler_s_nameof(compiler,func->name ) ))
    }
    else
    {
        x_array_push_d(((x_array*)(&(o->funcs))),((x_inst*)(((xoico_func_s*)bcore_fork(func)) )));
    }
    
    BLM_RETURNV(er_t, 0)
}

er_t xoico_stamp_s_push_internal_func_from_sc( xoico_stamp_s* o, sc_t sc )
{
    // xoico_stamp.x:503:1
    BLM_INIT_LEVEL(0);
    xoico_compiler_s* compiler = o->group->compiler;
    xoico_func_s* func = ((xoico_func_s*)BLM_LEVEL_T_PUSH(0,xoico_func_s,xoico_func_s_create()));
    func->overloadable = false;
    func->expandable = true;
    
    BLM_TRY(xoico_parse_sc(((xoico*)(func)),((const xoico_host*)(o)), sc ))
    
    sz_t idx = xoico_funcs_s_get_index_from_signature_global_name(&(o->funcs),func->signature_global_name );
    
    if( idx >= 0 )
    {
        BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(&(o->source_point),"Function '#<sc_t>' conflicts with an internal function for this stamp of the same name.", xoico_compiler_s_nameof(compiler,func->name ) ))
    }
    else
    {
        x_array_push_d(((x_array*)(&(o->funcs))),((x_inst*)(((xoico_func_s*)bcore_fork(func)) )));
    }
    
    BLM_RETURNV(er_t, 0)
}

er_t xoico_stamp_s_push_inexpandable_funcs( xoico_stamp_s* o )
{
    // xoico_stamp.x:528:1
    
    BLM_TRY(xoico_stamp_s_push_inexpandable_func_from_sc(o,"bcore_stamp_funcs.init;" ))
    BLM_TRY(xoico_stamp_s_push_inexpandable_func_from_sc(o,"bcore_stamp_funcs.down;" ))
    BLM_TRY(xoico_stamp_s_push_inexpandable_func_from_sc(o,"bcore_stamp_funcs.copy;" ))
    BLM_TRY(xoico_stamp_s_push_inexpandable_func_from_sc(o,"bcore_stamp_funcs.create;" ))
    BLM_TRY(xoico_stamp_s_push_inexpandable_func_from_sc(o,"bcore_stamp_funcs.discard;" ))
    BLM_TRY(xoico_stamp_s_push_inexpandable_func_from_sc(o,"bcore_stamp_funcs.clone;" ))
    return  0;
}

er_t xoico_stamp_s_push_compact_initializer_func( xoico_stamp_s* o )
{
    // xoico_stamp.x:541:1
    BLM_INIT_LEVEL(0);
    xoico_compiler_s* compiler = o->group->compiler;
    
    st_s sig;BLM_T_INIT_SPUSH(st_s, &sig);;
    st_s body;BLM_T_INIT_SPUSH(st_s, &body);;
    st_s_push_fa(&(sig),"o _(m@* o" );
    
    bcore_self_s* self = ((bcore_self_s*)BLM_LEVEL_T_PUSH(0,bcore_self_s,bcore_self_s_create())); self->type = o->tp_name; self->trait = o->trait_name;
    
    sc_t err_prefix = "Cannot setup member for compact initialization";
    
    {const xoico_stamp_arr_self_item_s* __a=o->arr_self_item ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){const xoico_stamp_self_item_s* self_item=&(__a->data[__i]);
    {
        if( self_item->arg_of_initializer )
        {BLM_INIT_LEVEL(5);
            bcore_self_item_s* item = ((bcore_self_item_s*)BLM_LEVEL_T_PUSH(5,bcore_self_item_s,bcore_self_item_s_create()));
            BLM_TRY(xoico_stamp_self_item_s_to_bcore_self_item(self_item,self, item ))
    
            if( !item->type ) BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(&(self_item->source_point),"#<sc_t>: Item has no type.", err_prefix ))
            if( !item->name ) BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(&(self_item->source_point),"#<sc_t>: Item has no name.", err_prefix ))
            if( bcore_flect_caps_is_array( item->caps ) ) BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(&(self_item->source_point),"#<sc_t>: Item is an array.", err_prefix ))
            if( bcore_flect_caps_is_typed( item->caps ) ) BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(&(self_item->source_point),"#<sc_t>: Item is typed.", err_prefix ))
            sz_t indirection = bcore_flect_caps_get_indirection( item->caps );
            bl_t is_leaf = bcore_type_is_leaf( item->type );
            bl_t is_pointer = ( item->caps == BCORE_CAPS_POINTER );
    
            sc_t sc_type = bnameof( item->type );
            sc_t sc_name = bnameof( item->name );
    
            if( indirection == 0 )
            {
                if( is_leaf )
                {
                    st_s_push_fa(&(sig),", #<sc_t> #<sc_t>", sc_type, sc_name );
                    st_s_push_fa(&(body),"    o.#<sc_t> = #<sc_t>;\n", sc_name, sc_name );
                }
                else
                {
                    st_s_push_fa(&(sig),", c #<sc_t>* #<sc_t>", sc_type, sc_name );
                    st_s_push_fa(&(body),"    o.#<sc_t>.copy( #<sc_t> );\n", sc_name, sc_name );
                }
            }
            else
            {
                if( is_leaf || is_pointer )
                {
                    st_s_push_fa(&(sig),", m #<sc_t>* #<sc_t>", sc_type, sc_name );
                    st_s_push_fa(&(body),"    o.#<sc_t> = #<sc_t>;\n", sc_name, sc_name );
                }
                else if( self_item->copy_from_initializer )
                {
                    st_s_push_fa(&(sig),", c #<sc_t>* #<sc_t>", sc_type, sc_name );
                    st_s_push_fa(&(body),"    o.#<sc_t> =< #<sc_t>.clone();\n", sc_name, sc_name );
                }
                else
                {
                    st_s_push_fa(&(sig),", d #<sc_t>* #<sc_t>", sc_type, sc_name );
                    st_s_push_fa(&(body),"    o.#<sc_t> =< #<sc_t>;\n", sc_name, sc_name );
                }
            }
        BLM_DOWN();}
    }
    }}st_s_push_fa(&(sig),")" );
    
    st_s st_func;BLM_T_INIT_SPUSH(st_s, &st_func);st_s_push_fa(&(st_func),"#<sc_t>\n"
        "{\n"
        "#<sc_t>"
        "    return o;\n"
        "};",
        sig.sc,
        body.sc
    );
    
    sz_t idx = xoico_funcs_s_get_index_from_name(&(o->funcs),xoico_compiler_s_entypeof(compiler,"_" ) );
    if( idx >= 0 )
    {
        BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(&(o->funcs.data[ idx ]->source_point),"Explicit compact function declaration not possible because this stamp has one or more member elements marked for compact initialization."
        ))
    }
    
    xoico_func_s* func = ((xoico_func_s*)BLM_LEVEL_T_PUSH(0,xoico_func_s,xoico_func_s_create()));
    BLM_TRY(xoico_parse_sc(((xoico*)(func)),((const xoico_host*)(o)), st_func.sc ))
    x_array_push_d(((x_array*)(&(o->funcs))),((x_inst*)(((xoico_func_s*)bcore_fork(func)) )));
    
    BLM_RETURNV(er_t, 0)
}

er_t xoico_stamp_s_parse( xoico_stamp_s* o, const xoico_host* host, x_source* source )
{
    // xoico_stamp.x:635:1
    BLM_INIT_LEVEL(0);
    xoico_compiler_s* compiler = o->group->compiler;
    bl_t verbatim = x_source_parse_bl(source," #?w'verbatim'" );
    
    st_s* st_stamp_name = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
    
    x_source_point_s_setup_from_source(&(o->source_point),source );
    
    xoico_transient_map_s_copy(&(o->transient_map),&(o->group->transient_map ));
    
    BLM_TRY(xoico_group_s_parse_name_st(o->group,source, st_stamp_name ))
    
    if( !st_s_ends_in_sc(st_stamp_name,"_s" ) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Stamp name '#<sc_t>' must end in '_s'.", st_stamp_name->sc ))
    
    if( x_source_parse_bl(source," #?'('" ) ) // functor args
    {BLM_INIT_LEVEL(1);
        xoico_args_s* args = ((xoico_args_s*)BLM_LEVEL_T_PUSH(1,xoico_args_s,xoico_args_s_create()));
        BLM_TRY(xoico_args_s_parse(args,host, source ))
        BLM_TRY(x_source_parse_fa(source," )" ))
    
        {const xoico_args_s* __a=args ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){const xoico_arg_s* e=__a->data[__i];
        {BLM_INIT_LEVEL(5);
            xoico_stamp_self_item_s* self_item = ((xoico_stamp_self_item_s*)BLM_LEVEL_T_PUSH(5,xoico_stamp_self_item_s,xoico_stamp_self_item_s_create()));
            self_item->arg_of_initializer = true;
            self_item->copy_from_initializer = ( e->typespec.access_class == TYPEOF_const );
    
            BLM_TRY(xoico_arg_s_to_self_item_st(e,((const xoico_host*)(o)),&( self_item->st )))
            x_source_point_s_copy(&(self_item->source_point),&(e->source_point ));
            x_array_push_d(((x_array*)(BCORE_PASS_CREATE(xoico_stamp_arr_self_item_s,o->arr_self_item))),((x_inst*)(((xoico_stamp_self_item_s*)bcore_fork(self_item)) )));
            o->has_compact_initializer = true;
        BLM_DOWN();}
    }}BLM_DOWN();}
    
    // using assignment symbol is optional
    if( x_source_parse_bl(source," #?'=' " ) )
    {
        // nothing (error in case use of assignment becomes illegal)
    }
    
    if( x_source_parse_bl(source," #?w'extending'" ) )
    {BLM_INIT_LEVEL(1);
        st_s* templ_name = ((st_s*)BLM_LEVEL_T_PUSH(1,st_s,st_s_create()));
        BLM_TRY(xoico_group_s_parse_name_st(o->group,source, templ_name ))
    
        if( !st_s_ends_in_sc(templ_name,"_s" ) )
        {
            BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Extending: Stamp name '#<sc_t>' must end in '_s'.", templ_name->sc ))
        }
    
        const xoico* item = xoico_compiler_s_get_const_item(compiler,btypeof( templ_name->sc ) );
        if( !item ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Template #<sc_t> not found.", templ_name->sc ))
        if( item->_ != ((tp_t)(TYPEOF_xoico_stamp_s)) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Template #<sc_t> is no stamp.", templ_name->sc ))
        xoico_stamp_s_copy(o,((xoico_stamp_s*)(item)) );
    BLM_DOWN();}
    else if( !verbatim && o->group->extending_stamp )
    {
        xoico_stamp_s_copy(o,o->group->extending_stamp );
    }
    else
    {
        /// 'is_aware' is true by default
        if      ( x_source_parse_bl(source," #?w'obliv'" ) ) o->is_aware = false;
        else if ( x_source_parse_bl(source," #?w'aware'" ) ) o->is_aware = true;
        o->trait_name = o->group->tp_name;
    
        if( !x_source_parse_bl(source," #=?w'func'" ) && !x_source_parse_bl(source," #=?w'trans'" ) ) // if func follows, the brief-functor format was chosen
        {BLM_INIT_LEVEL(2);
            st_s st_trait_name;BLM_T_INIT_SPUSH(st_s, &st_trait_name);;
            BLM_TRY(xoico_group_s_parse_name_st(o->group,source,&( st_trait_name )))
            if( st_trait_name.size > 0 ) o->trait_name = xoico_compiler_s_entypeof(compiler,st_trait_name.sc );
        BLM_DOWN();}
    
        if( x_source_parse_bl(source," #?w'trans' " ) )
        {
            BLM_TRY(xoico_transient_map_s_parse_update(&(o->transient_map),((const xoico_host*)(o)), source ))
        }
    
    }
    
    st_s_copy(&(o->st_name),st_stamp_name );
    o->tp_name = xoico_compiler_s_entypeof(compiler,st_stamp_name->sc );
    BLM_TRY(xoico_stamp_s_parse_extend(o,source ))
    if( o->has_compact_initializer ) BLM_TRY(xoico_stamp_s_push_compact_initializer_func(o))
    
    BLM_RETURNV(er_t, 0)
}

er_t xoico_stamp_s_finalize( xoico_stamp_s* o, const xoico_host* host )
{
    // xoico_stamp.x:725:1
    BLM_INIT_LEVEL(0);
    //if( o.has_compact_initializer ) o.push_compact_initializer_func();
    
    if( o->finalized ) BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(&(o->source_point),"In stamp '#<sc_t>': Attempting to finalize stamp twice.", o->st_name.sc ))
    
    xoico_compiler_s* compiler = o->group->compiler;
    st_s self_buf;BLM_T_INIT_SPUSH(st_s, &self_buf);;
    
    bcore_self_item_s* first_array_item = NULL; // !=NULL if stamp has an array;
    {BLM_INIT_LEVEL(1);
        bcore_self_s* self = ((bcore_self_s*)BLM_LEVEL_T_PUSH(1,bcore_self_s,bcore_self_s_create())); self->type = o->tp_name; self->trait = o->trait_name;
        {const xoico_stamp_arr_self_item_s* __a=o->arr_self_item ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_stamp_self_item_s* self_item=&(__a->data[__i]);
        {BLM_INIT_LEVEL(5);
            bcore_self_item_s* item = ((bcore_self_item_s*)BLM_LEVEL_T_PUSH(5,bcore_self_item_s,bcore_self_item_s_create()));
            BLM_TRY(xoico_stamp_self_item_s_to_bcore_self_item(self_item,self, item ))
    
            if( xoico_compiler_s_is_group(compiler,item->type ) )
            {
                if( bcore_flect_caps_get_indirection( item->caps ) == 0 )
                {
                    BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(&(self_item->source_point),"Element is a group at indirection 0. Was a pointer intended?\n" ))
                }
    
                if(  !item->flags.f_aware  &&  !item->flags.f_obliv  && !bcore_flect_caps_is_typed( item->caps ) )
                {
                    /// group reference: when type awareness is not specified but 'aware' is a legal option, assume 'aware'.
                    st_s_insert_sc(&(self_item->st),0, "aware " );
                }
            }
    
            if( bcore_flect_caps_is_array( item->caps ) && !first_array_item ) first_array_item = ((bcore_self_item_s*)BLM_LEVEL_T_PUSH(0,bcore_self_item_s,bcore_self_item_s_clone(item)));
            st_s_push_st(&(self_buf),&(self_item->st ));
        BLM_DOWN();}
    }}BLM_DOWN();}
    
    // set transient classes for x_array
    if( o->trait_name == ((tp_t)(TYPEOF_x_array)) || o->trait_name == ((tp_t)(TYPEOF_bcore_array)) )
    {
        if( !first_array_item )
        {
            BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(&(o->source_point),"In stamp '#<sc_t>': Stamp is of trait 'x_array' but contains no array", o->st_name.sc ))
        }
        xoico_transient_map_s_set(&(o->transient_map),xoico_compiler_s_entypeof(compiler,"TO" ), o->tp_name );
        if( first_array_item->type ) xoico_transient_map_s_set(&(o->transient_map),xoico_compiler_s_entypeof(compiler,"TE" ), first_array_item->type );
    }
    
    // overload transient features
    {BLM_INIT_LEVEL(1);const bcore_arr_tp_s* __a=((bcore_arr_tp_s*)BLM_LEVEL_T_PUSH(1,bcore_arr_tp_s,xoico_transient_map_s_create_key_arr(&(o->transient_map)))) ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){tp_t key=__a->data[__i];
    {
        const xoico_feature_s* feature = xoico_stamp_s_get_traitline_feature_from_name(o,key );
        if( feature )
        {BLM_INIT_LEVEL(5);
            if( !xoico_signature_s_fits_transient_type_feature(&(feature->signature)) )
            {BLM_INIT_LEVEL(6);
                st_s feature_reference;BLM_T_INIT_SPUSH(st_s, &feature_reference);;
                x_source_point_s_source_reference_to_sink(&(feature->source_point),false,((x_sink*)(&( feature_reference ))));
                BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(&(o->source_point),"In stamp '#<sc_t>': Stamp uses transient feature '#<sc_t>' with triggers automatic overload of a feature with this name in the stamps traitline.\n"
                    "A matching feature was found in '#<sc_t>' but it does not satisfy signature-requirements for such an overload.\n"
                    "Suggestions:\n"
                    "   - Define feature in the form 'feature tp_t #<sc_t>( @*o );'. Or\n"
                    "   - If the feature has a different purpose:\n"
                    "       - Rename the feature. Or\n"
                    "       - Rename the transient type identifier '#<sc_t>'.\n",
                    o->st_name.sc,
                    xoico_compiler_s_nameof(compiler,feature->signature.name ),
                    feature_reference.sc,
                    xoico_compiler_s_nameof(compiler,feature->signature.name ),
                    xoico_compiler_s_nameof(compiler,feature->signature.name )
                ))
            BLM_DOWN();}
    
            tp_t type = xoico_transient_map_s_get(&(o->transient_map),key );
    
            st_s func_code;BLM_T_INIT_SPUSH(st_s, &func_code);st_s_push_fa(&(func_code),"#<sc_t>.#<sc_t> { = TYPEOF_#<sc_t>; }",
                xoico_compiler_s_nameof(compiler,feature->group->tp_name ),
                xoico_compiler_s_nameof(compiler,feature->signature.name ),
                xoico_compiler_s_nameof(compiler,type )
            );
    
            BLM_TRY(xoico_stamp_s_push_internal_func_from_sc(o,func_code.sc ))
        BLM_DOWN();}
    }
    
    }BLM_DOWN();}{const xoico_wraps_s* __a=&(o->wraps );if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_wrap_s* wrap=__a->data[__i];
    {
        BLM_TRY(xoico_wrap_s_finalize(wrap,((const xoico_host*)(o ))))
        x_array_push_d(((x_array*)(&(o->funcs))),((x_inst*)(((xoico_func_s*)bcore_fork(wrap->func)) )));
    }
    
    }}{const xoico_funcs_s* __a=&(o->funcs );if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_func_s* func=__a->data[__i];
    {
        BLM_TRY(xoico_func_s_finalize(func,((const xoico_host*)(o ))))
        if( xoico_func_s_reflectable(func,((const xoico_host*)(o ))) ) BLM_TRY(xoico_func_s_push_flect_decl_to_sink(func,((const xoico_host*)(o)),((x_sink*)(&( self_buf )))))
        BLM_TRY(xoico_compiler_s_register_func(compiler,func ))
    }
    
    }}st_s_attach( &(o->self_source ),  st_s_create());
    st_s_push_fa(o->self_source,"#<sc_t> =", o->st_name.sc );
    if( o->is_aware ) st_s_push_sc(o->self_source,"aware " );
    st_s_push_sc(o->self_source,xoico_compiler_s_nameof(compiler,o->trait_name ) );
    st_s_push_fa(o->self_source,"{#<st_s*>}", (&(self_buf)) );
    
    bcore_self_s_attach( &(o->self ),  bcore_self_s_parse_source( ( bcore_source* )((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_st(o->self_source ))), 0, 0, o->group->st_name.sc, false ));
    
    // checking for repetitions in o.self (non-functions)
    bcore_hmap_tp_s* hmap_name = ((bcore_hmap_tp_s*)BLM_LEVEL_T_PUSH(0,bcore_hmap_tp_s,bcore_hmap_tp_s_create()));
    sz_t self_items = bcore_self_s_items_size( o->self );
    for(sz_t i = 0; i < self_items; i++ )
    {
        const bcore_self_item_s* item = bcore_self_s_get_item(o->self,i );
        if( item->name && ( item->caps != BCORE_CAPS_EXTERNAL_FUNC ) )
        {
            if( bcore_hmap_tp_s_exists(hmap_name,item->name ) )
            {
                BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(&(o->source_point),"In stamp '#<sc_t>': Repeated use of member name '#<sc_t>'.", o->st_name.sc, xoico_compiler_s_nameof(compiler,item->name ) ))
            }
    
            bcore_hmap_tp_s_set(hmap_name,item->name );
        }
    }
    
    // check validity of trait name
    if( !xoico_compiler_s_is_group(compiler,o->trait_name ) )
    {
        BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(&(o->source_point),"In stamp '#<sc_t>': Trait name '#<sc_t>' is not a group.", o->st_name.sc, xoico_compiler_s_nameof(compiler,o->trait_name ) ))
    }
    
    // update retrievability of groups in traitline
    for(tp_t trait_name = o->trait_name; trait_name != 0; )
    {
        const xoico_group_s* group = xoico_compiler_s_get_group(compiler,trait_name );
        trait_name = group->trait_name != group->tp_name ? group->trait_name : 0;
        if( group->is_retrievable ) bcore_arr_tp_s_push(BCORE_PASS_CREATE(bcore_arr_tp_s,group->retrievable_stamps),o->tp_name );
    }
    
    o->finalized = true;
    
    BLM_RETURNV(er_t, 0)
}

er_t xoico_stamp_s_expand_declaration( const xoico_stamp_s* o, const xoico_host* host, sz_t indent, x_sink* sink )
{
    // xoico_stamp.x:872:1
    
    sc_t sc_name = o->st_name.sc;
    
    x_sink_push_fa(sink,"#rn{ }##define TYPEOF_#<sc_t> 0x#pl16'0'{#X<tp_t>}ull\n", indent, sc_name, btypeof( sc_name ) );
    
    x_sink_push_fa(sink,"#rn{ }##define BETH_EXPAND_ITEM_#<sc_t>", indent, sc_name, sc_name );
    x_sink_push_fa(sink," \\\n#rn{ }  BCORE_DECLARE_OBJECT( #<sc_t> )", indent, sc_name );
    x_sink_push_fa(sink," \\\n" );
    
    bcore_self_s_struct_body_to_sink_newline_escaped( o->self, indent + 2, ( bcore_sink* )sink );
    x_sink_push_fa(sink,";" );
    
    {const xoico_funcs_s* __a=&(o->funcs );if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_func_s* func=__a->data[__i]; BLM_TRY(xoico_func_s_expand_forward(func,((const xoico_host*)(o)), indent + 2, sink )) // expands all prototypes
    }}{const xoico_funcs_s* __a=&(o->funcs );if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_func_s* func=__a->data[__i]; BLM_TRY(xoico_func_s_expand_declaration(func,((const xoico_host*)(o)), indent + 2, sink )) // only expands static inline functions
    
    }}x_sink_push_fa(sink,"\n" );
    return  0;
}

er_t xoico_stamp_s_expand_definition( const xoico_stamp_s* o, const xoico_host* host, sz_t indent, x_sink* sink )
{
    // xoico_stamp.x:894:1
    BLM_INIT_LEVEL(0);
    st_s* embedded_string = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,xoico_stamp_create_embedded_string(o->self_source )));
    
    // 4095 is the C99-limit for string literals
    if( embedded_string->size > 4095 )
    {
        BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(&(o->source_point),"Reflection embedding failed. (Stamp is too large)\n"
            "The embedded code would require a string literal larger than 4095 characters.\n"
            "This exceeds the limit defined in C99.\n"
        ))
    }
    
    sz_t idx = st_s_find_char(embedded_string,0, -1, '=' );
    sc_t self_def = "";
    if( idx < embedded_string->size )
    {
        self_def = embedded_string->sc + idx + 1;
    }
    
    x_sink_push_fa(sink,"\n" );
    x_sink_push_fa(sink,"#rn{ }BCORE_DEFINE_OBJECT_INST_P( #<sc_t> )\n", indent, o->st_name.sc );
    
    st_s* multiline_string = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,xoico_stamp_create_structured_multiline_string(self_def, indent )));
    x_sink_push_fa(sink,"#<sc_t>;\n", multiline_string->sc );
    
    {const xoico_funcs_s* __a=&(o->funcs );if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_func_s* func=__a->data[__i]; BLM_TRY(xoico_func_s_expand_definition(func,((const xoico_host*)(o)), indent, sink ))
    
    }}BLM_RETURNV(er_t, 0)
}

er_t xoico_stamp_s_expand_init1( const xoico_stamp_s* o, const xoico_host* host, sz_t indent, x_sink* sink )
{
    // xoico_stamp.x:929:1
    
    xoico_compiler_s* compiler = o->group->compiler;
    
    {const xoico_funcs_s* __a=&(o->funcs );if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_func_s* func=__a->data[__i];
    {
        if( xoico_func_s_reflectable(func,host ) )
        {
            const xoico_signature_s* signature = func->signature;
            x_sink_push_fa(sink,"#rn{ }BCORE_REGISTER_FFUNC( #<sc_t>, #<sc_t>_#<sc_t> );\n",
                indent,
                xoico_compiler_s_nameof(compiler,signature->global_name ),
                o->st_name.sc,
                xoico_compiler_s_nameof(compiler,func->name )
            );
        }
    }
    }}x_sink_push_fa(sink,"#rn{ }BCORE_REGISTER_OBJECT( #<sc_t> );\n", indent, o->st_name.sc );
    return  0;
}

BCORE_DEFINE_OBJECT_INST_P( xoico_stamp_self_item_s )
"aware xoico_stamp"
"{"
    "x_source_point_s source_point;"
    "st_s st;"
    "bl_t arg_of_initializer;"
    "bl_t copy_from_initializer;"
"}";

er_t xoico_stamp_self_item_s_to_bcore_self_item( const xoico_stamp_self_item_s* o, const bcore_self_s* self, bcore_self_item_s* item )
{
    // xoico_stamp.x:140:5
    BLM_INIT_LEVEL(0);
    er_t er = bcore_self_item_s_parse_src( item, sr_awc(((const x_inst*)( ((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_st(&(o->st )))) ))), self, false );
    
    if( er )
    {BLM_INIT_LEVEL(1);
        st_s* msg = ((st_s*)BLM_LEVEL_T_PUSH(1,st_s,st_s_create()));
        bcore_error_pop_st((&(er)), msg );
        BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(&(o->source_point),"#<sc_t>\n", msg->sc ))
    BLM_DOWN();}
    BLM_RETURNV(er_t, 0)
}

BCORE_DEFINE_OBJECT_INST_P( xoico_stamp_arr_self_item_s )
"aware x_array"
"{"
    "xoico_stamp_self_item_s [];"
"}";

XOILA_DEFINE_SPECT( xoico, xoico_stamp )
"{"
    "bcore_spect_header_s header;"
"}";

st_s* xoico_stamp_create_embedded_string( const st_s* s )
{
    // xoico_stamp.x:163:1
    
    st_s* out = st_s_create();
    for(sz_t i = 0; i < s->size; i++ )
    {
        if( s->data[ i ] == '/' && s->data[ i + 1 ] == '/' )
        {
            while( i < s->size && s->data[ i ] != '\n' ) i++;
        }
        else if( s->data[ i ] == '/' && s->data[ i + 1 ] == '*' )
        {
            while( i < s->size )
            {
                if( s->data[ i ] == '*' && s->data[ i + 1 ] == '/' )
                {
                    if( out->data[ out->size - 1 ] != ' ' ) st_s_push_char(out,' ' );
                    i += 2;
                    break;
                }
                i++;
            }
        }
        else if( s->data[ i ] == '"' )
        {
            st_s_push_char(out,'\\' );
            st_s_push_char(out,'"' );
            i++;
            while( i < s->size && s->data[ i ] != '"' ) st_s_push_char(out,s->data[ i++ ] );
            if( s->data[ i ] == '"' )
            {
                st_s_push_char(out,'\\' );
                st_s_push_char(out,'"' );
            }
        }
        else if( s->data[ i ] == '\n' )
        {
            if( out->data[ out->size - 1 ] != ' ' ) st_s_push_char(out,' ' );
        }
        else if( s->data[ i ] == '\t' )
        {
            if( out->data[ out->size - 1 ] != ' ' ) st_s_push_char(out,' ' );
        }
        else if( s->data[ i ] == ' ' )
        {
            if( out->data[ out->size - 1 ] != ' ' ) st_s_push_char(out,' ' );
        }
        else
        {
            st_s_push_char(out,s->data[ i ] );
        }
    }
    if( out->size > 0 && out->data[ out->size - 1 ] == ' ' )
    {
        out->data[ out->size - 1 ] = 0;
        out->size--;
    }
    return  out;
}

st_s* xoico_stamp_create_structured_multiline_string( const sc_t s, sz_t indent )
{
    // xoico_stamp.x:227:1
    
    st_s* out = st_s_create();
    sz_t ind = indent;
    bl_t newline = true;
    for(sz_t i = 0; s[ i ] != 0; i++ )
    {
        char c = s[ i ];
        if( c == ';' )
        {
            if( newline ) st_s_push_fa(out,"#rn{ }\"", ind );
            st_s_push_char(out,c );
            st_s_push_fa(out,"\"\n" );
            newline = true;
        }
        else if( c == '{' )
        {
            if( newline )
            {
                st_s_push_fa(out,"#rn{ }\"{\"", ind );
            }
            else
            {
                st_s_push_fa(out,"\"\n#rn{ }\"{\"", ind );
            }
            if( s[ i + 1 ] != 0 ) st_s_push_char(out,'\n' );
            ind += 4;
            newline = true;
    
        }
        else if( c == '}' )
        {
            ind -= 4;
            if( newline )
            {
                st_s_push_fa(out,"#rn{ }\"}\"", ind );
            }
            else
            {
                st_s_push_fa(out,"\"\n#rn{ }\"}\"", ind );
            }
            if( s[ i + 1 ] != 0 ) st_s_push_char(out,'\n' );
            newline = true;
        }
        else if( c == ' ' )
        {
            if( !newline )
            {
                if( s[ i + 1 ] != ';' && s[ i + 1 ] != '{' && s[ i + 1 ] != '}' && s[ i + 1 ] != 0 )
                {
                    st_s_push_char(out,c );
                }
            }
        }
        else
        {
            if( newline )
            {
                st_s_push_fa(out,"#rn{ }\"", ind );
                newline = false;
            }
            st_s_push_char(out,c );
        }
    }
    
    if( !newline ) st_s_push_char(out,'"' );
    return  out;
}

/**********************************************************************************************************************/
// source: xoico_source.x

//----------------------------------------------------------------------------------------------------------------------
// group: xoico_source

BCORE_DEFINE_OBJECT_INST_P( xoico_source_s )
"aware xoico_source"
"{"
    "st_s full_path;"
    "st_s name;"
    "st_s ext;"
    "xoico_group_s => [];"
    "bcore_arr_u0_s => plain_data;"
    "bl_t embed_plain_data_as_string_function;"
    "hidden aware xoico_target_s* target;"
    "func xoico:expand_setup;"
    "func xoico:get_hash;"
    "func xoico:finalize;"
    "func xoico:expand_declaration;"
    "func xoico:expand_definition;"
    "func xoico:expand_init1;"
    "func xoico:expand_down1;"
    "func xoico:expand_manifesto;"
    "func xoico_host:compiler;"
"}";

er_t xoico_source_s_expand_setup( xoico_source_s* o, const xoico_host* host )
{
    // xoico_source.x:43:5
    
    {const xoico_source_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_group_s* e=__a->data[__i]; BLM_TRY(xoico_group_s_expand_setup(e,((const xoico_host*)(o ))))
    }}return  0;
}

xoico_group_s* xoico_source_s_push_d( xoico_source_s* o, xoico_group_s* group )
{
    // xoico_source.x:49:5
    
    return  ((xoico_group_s*)(x_array_push_d(((x_array*)(o)),((x_inst*)(group )))));
}

tp_t xoico_source_s_get_hash( const xoico_source_s* o )
{
    // xoico_source.x:54:5
    
    tp_t hash = bcore_tp_fold_tp( bcore_tp_init(), o->_ );
    {const xoico_source_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_group_s* e=__a->data[__i]; hash = bcore_tp_fold_tp( hash, xoico_group_s_get_hash(e) );}}
    return  hash;
}

er_t xoico_source_s_finalize( xoico_source_s* o, const xoico_host* host )
{
    // xoico_source.x:63:5
    
    {const xoico_source_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_group_s* e=__a->data[__i]; BLM_TRY(xoico_group_s_finalize(e,((const xoico_host*)(o ))))
    }}return  0;
}

er_t xoico_source_s_expand_declaration( const xoico_source_s* o, const xoico_host* host, sz_t indent, x_sink* sink )
{
    // xoico_source.x:69:5
    
    if( o->size > 0 )
    {
        x_sink_push_fa(sink,"\n" );
        x_sink_push_fa(sink,"#rn{ }/*#rn{*}*/\n", indent, sz_max( 0, 116 - indent ) );
        x_sink_push_fa(sink,"#rn{ }// source: #<sc_t>.#<sc_t>\n", indent, o->name.sc, o->ext.sc );
        {const xoico_source_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_group_s* e=__a->data[__i]; BLM_TRY(xoico_group_s_expand_declaration(e,indent, sink ))
    }}}
    return  0;
}

er_t xoico_source_s_expand_definition( const xoico_source_s* o, const xoico_host* host, sz_t indent, x_sink* sink )
{
    // xoico_source.x:81:5
    
    if( o->size > 0 || st_s_equal_sc(&(o->ext),"h" ) )
    {
        x_sink_push_fa(sink,"\n" );
        x_sink_push_fa(sink,"#rn{ }/*#rn{*}*/\n", indent, sz_max( 0, 116 - indent ) );
        x_sink_push_fa(sink,"#rn{ }// source: #<sc_t>.#<sc_t>\n", indent, o->name.sc, o->ext.sc );
    
        if( st_s_equal_sc(&(o->ext),"h" ) )
        {
            x_sink_push_fa(sink,"#rn{ }##include \"#<sc_t>.#<sc_t>\"\n", indent, o->name.sc, o->ext.sc );
        }
        {const xoico_source_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_group_s* e=__a->data[__i]; BLM_TRY(xoico_group_s_expand_definition(e,indent, sink ))
    }}}
    
    if( o->embed_plain_data_as_string_function )
    {BLM_INIT_LEVEL(1);
        x_sink_push_fa(sink,"\n" );
        x_sink_push_fa(sink,"#rn{ }/*#rn{*}*/\n", indent, sz_max( 0, 116 - indent ) );
        x_sink_push_fa(sink,"#rn{ }// source: #<sc_t>.#<sc_t>\n", indent, o->name.sc, o->ext.sc );
        x_sink_push_fa(sink,"#rn{ }// Embedded as string function.\n", indent );
    
        st_s func_global_name;BLM_T_INIT_SPUSH(st_s, &func_global_name);st_s_push_fa(&(func_global_name),"#<sc_t>_#<sc_t>", o->name.sc, o->ext.sc );
        st_s_replace_sc_sc(&(func_global_name),".", "_" );
    
        x_sink_push_fa(sink,"#rn{ }st_s* #<sc_t>( void )\n", indent, func_global_name.sc );
        x_sink_push_fa(sink,"#rn{ }{\n"                            , indent );
    
        x_sink_push_fa(sink,"#rn{ }    sc_t sc[] =\n", indent );
        x_sink_push_fa(sink,"#rn{ }    {\n" , indent );
        x_sink_push_fa(sink,"#rn{ }        \"" , indent );
    
        sz_t lines = 0;
    
        for(sz_t i = 0; i < o->plain_data->size; i++ )
        {
            u0_t c = o->plain_data->data[ i ];
            switch( c )
            {
                case '\\': x_sink_push_sc(sink,"\\\\" ); break;
                case '"' : x_sink_push_sc(sink,"\\\"" ); break;
                case '\n':
                {
                    if( i + 1 < o->plain_data->size )
                    {
                        x_sink_push_fa(sink,"\\n\",\n" );
                        x_sink_push_fa(sink,"#rn{ }        \"" , indent );
                        lines++;
                    }
                }
                break;
    
                default: x_sink_push_char(sink,c );  break;
            }
        }
        x_sink_push_fa(sink,"\\n\"\n" );
        x_sink_push_fa(sink,"#rn{ }    };\n", indent );
        lines++;
    
        x_sink_push_fa(sink,"#rn{ }    st_s* st = st_s_create();\n", indent );
        x_sink_push_fa(sink,"#rn{ }    for( sz_t i = 0; i < #<sz_t>; i++ )\n", indent, lines );
        x_sink_push_fa(sink,"#rn{ }    {\n", indent );
        x_sink_push_fa(sink,"#rn{ }        st_s_push_sc( st, sc[ i ] );\n", indent );
        x_sink_push_fa(sink,"#rn{ }    }\n", indent );
        x_sink_push_fa(sink,"#rn{ }    return st;\n", indent, func_global_name.sc );
        x_sink_push_fa(sink,"#rn{ }}\n", indent, func_global_name.sc );
    BLM_DOWN();}
    
    return  0;
}

er_t xoico_source_s_expand_init1( const xoico_source_s* o, const xoico_host* host, sz_t indent, x_sink* sink )
{
    // xoico_source.x:152:5
    
    if( o->size > 0 )
    {
        x_sink_push_fa(sink,"\n" );
        x_sink_push_fa(sink,"#rn{ }// #rn{-}\n", indent, sz_max( 0, 80 - indent ) );
        x_sink_push_fa(sink,"#rn{ }// source: #<sc_t>.#<sc_t>\n", indent, o->name.sc, o->ext.sc );
        {const xoico_source_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_group_s* e=__a->data[__i]; BLM_TRY(xoico_group_s_expand_init1(e,indent, sink ))
    }}}
    return  0;
}

er_t xoico_source_s_expand_down1( const xoico_source_s* o, const xoico_host* host, sz_t indent, x_sink* sink )
{
    // xoico_source.x:164:5
    
    if( o->size > 0 )
    {
        // there is hardly any code in the down1-handler, so for the moment we need omit commenting the sources
    //            sink.push_fa( "\n" );
    //            sink.push_fa( "#rn{ }// #rn{-}\n", indent, sz_max( 0, 80 - indent ) );
    //            sink.push_fa( "#rn{ }// source: #<sc_t>.#<sc_t>\n", indent, o.name.sc, o.ext.sc );
        {const xoico_source_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_group_s* e=__a->data[__i]; BLM_TRY(xoico_group_s_expand_down1(e,indent, sink ))
    }}}
    return  0;
}

er_t xoico_source_s_expand_manifesto( const xoico_source_s* o, const xoico_host* host, sz_t indent, x_sink* sink )
{
    // xoico_source.x:177:5
    
    {const xoico_source_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_group_s* e=__a->data[__i]; BLM_TRY(xoico_group_s_expand_manifesto(e,host, indent, sink ))
    }}return  0;
}

void xoico_source_s_explicit_embeddings_push( const xoico_source_s* o, bcore_arr_st_s* arr )
{
    // xoico_source.x:183:5
    
    {const xoico_source_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_group_s* group=__a->data[__i]; xoico_group_s_explicit_embeddings_push(group,arr );}}
}

xoico_compiler_s* xoico_source_s_compiler( const xoico_source_s* o )
{
    // xoico_source.x:188:5
    
    return  o->target->compiler;
}

er_t xoico_source_s_get_group_if_preexsting( xoico_source_s* o, const xoico_host* host, x_source* source, sc_t group_name, sc_t trait_name, xoico_group_s** group )
{
    // xoico_source.x:199:1
    
    xoico_compiler_s* compiler = xoico_host_a_compiler(host);
    if( xoico_compiler_s_is_group(compiler,xoico_host_a_entypeof(host,group_name ) ) )
    {
        (*(group)) = xoico_compiler_s_get_group(compiler,xoico_host_a_entypeof(host,group_name ) );
    
        // Extending a preexisting group is only allowed inside the same target
        if( (**(group)).xoico_source->target != o->target )
        {
            return  x_source_parse_error_fa(source,"Group '#<sc_t>' was defined in target '#<sc_t>'. It cannot be extended in a different target.", group_name, (**(group)).xoico_source->target->name.sc );
        }
    
        if( (**(group)).trait_name != xoico_compiler_s_entypeof(compiler,trait_name ) )
        {
            return  x_source_parse_error_fa(source,"Extending group '#<sc_t>' of trait '#<sc_t>' with a different trait '#<sc_t>'.", group_name, xoico_host_a_nameof(host,(**(group)).trait_name ), trait_name );
        }
    
    }
    else
    {
        (*(group)) = NULL;
    }
    return  0;
}

er_t xoico_source_s_parse( xoico_source_s* o, const xoico_host* host, x_source* source, sc_t group_name, sc_t trait_name, tp_t embed_method )
{
    // xoico_source.x:227:1
    BLM_INIT_LEVEL(0);
    xoico_compiler_s* compiler = o->target->compiler;
    
    if( embed_method == TYPEOF_as_header )
    {
        while( !x_source_eos(source) )
        {
            if( x_source_parse_bl(source," #?w'XOILA_DEFINE_GROUP'" ) )
            {BLM_INIT_LEVEL(3);
                st_s st_trait_name;BLM_T_INIT_SPUSH(st_s, &st_trait_name);;
                st_s st_group_name;BLM_T_INIT_SPUSH(st_s, &st_group_name);;
                BLM_TRY(x_source_parse_fa(source," ( #name , #name", (&(st_group_name)), (&(st_trait_name)) ))
                xoico_group_s* group = NULL;
                BLM_TRY(xoico_source_s_get_group_if_preexsting(o,host, source, st_group_name.sc, st_trait_name.sc, (&(group)) ))
                if( !group )
                {
                    group = xoico_source_s_push_d(o,xoico_group_s_create() );
                    group->xoico_source = o;
                    group->compiler = compiler;
                    xoico_group_s_set_name_sc(group,host, st_group_name.sc );
                    group->trait_name = xoico_compiler_s_entypeof(compiler,st_trait_name.sc );
                }
    
                if( x_source_parse_bl(source," #=?','" ) )
                {
                    while( x_source_parse_bl(source," #?','" ) )
                    {BLM_INIT_LEVEL(5);
                        st_s* embed_file = ((st_s*)BLM_LEVEL_T_PUSH(5,st_s,st_s_create()));
                        BLM_TRY(x_source_parse_fa(source," #string", embed_file ))
                        x_source* embed_source = NULL;
                        BLM_TRY(xoico_embed_file_open(source, embed_file->sc, (&(embed_source)) ))
                        ((x_source*)BLM_LEVEL_A_PUSH(0,embed_source));
                        bcore_arr_st_s_push_st(&(group->explicit_embeddings),embed_file );
                        BLM_TRY(xoico_group_s_parse(group,((const xoico_host*)(o)), false, embed_source ))
                    BLM_DOWN();}
                    BLM_TRY(x_source_parse_fa(source," )" ))
                }
                else
                {
                    BLM_TRY(x_source_parse_fa(source," )" ))
                    BLM_TRY(xoico_group_s_parse(group,((const xoico_host*)(o)), false, source ))
                }
                BLM_TRY(xoico_compiler_s_register_group(o->target->compiler,group ))
            BLM_DOWN();}
            else
            {
                x_source_get_u0(source);
            }
        }
    }
    else
    {
        xoico_group_s* group = NULL;
    
        BLM_TRY(xoico_source_s_get_group_if_preexsting(o,host, source, group_name, trait_name, (&(group)) ))
        if( group )
        {
            if( group->xoico_source != o )
            {BLM_INIT_LEVEL(3);
                bcore_arr_st_s_push_st(&(group->explicit_embeddings),((st_s*)BLM_LEVEL_T_PUSH(3,st_s,st_s_create_fa("#<sc_t>.#<sc_t>", o->name.sc, o->ext.sc ))) );
            BLM_DOWN();}
        }
        else
        {
            group = xoico_source_s_push_d(o,xoico_group_s_create() );
            group->xoico_source = o;
            group->compiler = compiler;
            xoico_group_s_set_name_sc(group,host, group_name );
            group->trait_name = xoico_compiler_s_entypeof(compiler,trait_name );
            group->is_manifesto = true;
        }
    
        if( embed_method == TYPEOF_as_group )
        {
            BLM_TRY(xoico_group_s_parse(group,((const xoico_host*)(o)), false, source ))
        }
        else
        {
            BLM_TRY(xoico_group_s_parse_embed(group,((const xoico_host*)(o)), source, embed_method ))
        }
    
        BLM_TRY(xoico_compiler_s_register_group(compiler,group ))
    }
    
    BLM_RETURNV(er_t, 0)
}

XOILA_DEFINE_SPECT( xoico, xoico_source )
"{"
    "bcore_spect_header_s header;"
"}";

/**********************************************************************************************************************/
// source: xoico_target.x

//----------------------------------------------------------------------------------------------------------------------
// group: xoico_target

BCORE_DEFINE_OBJECT_INST_P( xoico_target_s )
"aware xoico_target"
"{"
    "st_s name;"
    "st_s ext;"
    "st_s => copyright_and_license_terms;"
    "bl_t update_target_on_body_signature = true;"
    "st_s include_path;"
    "st_s output_path;"
    "xoico_source_s => [];"
    "st_s signal_handler_name;"
    "bl_t define_signal_handler;"
    "bcore_arr_sz_s dependencies;"
    "bl_t flag;"
    "bl_t modified;"
    "bl_t readonly;"
    "st_s => target_h;"
    "st_s => target_c;"
    "st_s => target_state;"
    "tp_t body_signature_h;"
    "tp_t body_signature_c;"
    "tp_t pre_hash;"
    "aware xoico_cengine -> cengine;"
    "hidden aware xoico_compiler_s* compiler;"
    "hidden xoico_func_s* main_function;"
    "func xoico:finalize;"
    "func xoico:expand_setup;"
    "func xoico_host:compiler;"
    "func xoico_host:cengine;"
"}";

er_t xoico_target_s_finalize( xoico_target_s* o, const xoico_host* host )
{
    // xoico_target.x:76:5
    
    {const xoico_target_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_source_s* e=__a->data[__i]; BLM_TRY(xoico_source_s_finalize(e,((const xoico_host*)(o ))))
    }}return  0;
}

er_t xoico_target_s_expand_setup( xoico_target_s* o, const xoico_host* host )
{
    // xoico_target.x:82:5
    
    {const xoico_target_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_source_s* e=__a->data[__i]; BLM_TRY(xoico_source_s_expand_setup(e,((const xoico_host*)(o ))))
    }}return  0;
}

er_t xoico_target_s_set_main_function( xoico_target_s* o, const xoico_func_s* func )
{
    // xoico_target.x:92:5
    
    if( o->compiler->has_main_function ) return  x_source_point_s_parse_error_fa(&(func->source_point),"A main function was already declared." );
    o->compiler->has_main_function = true;
    o->main_function = ((xoico_func_s*)(func));
    return  0;
}

void xoico_target_s_push_d( xoico_target_s* o, xoico_source_s* source )
{
    // xoico_target.x:100:5
    
    x_array_push_d(((x_array*)(o)),((x_inst*)(source )));
}

void xoico_target_s_explicit_embeddings_push( const xoico_target_s* o, bcore_arr_st_s* arr )
{
    // xoico_target.x:105:5
    
    {const xoico_target_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_source_s* source=__a->data[__i]; xoico_source_s_explicit_embeddings_push(source,arr );}}
}

er_t xoico_target_s_parse_from_path( xoico_target_s* o, sc_t source_path, sc_t group_name, sc_t trait_name, tp_t embed_method )
{
    // xoico_target.x:120:1
    BLM_INIT_LEVEL(0);
    bl_t source_exists = false;
    
    {const xoico_target_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_source_s* e=__a->data[__i];
    {
        if( sc_t_equal( e->full_path.sc, source_path ) )
        {
            source_exists = true;
            break;
        }
    }
    
    }}if( !source_exists )
    {BLM_INIT_LEVEL(1);
        xoico_source_s* xsource = ((xoico_source_s*)BLM_LEVEL_T_PUSH(0,xoico_source_s,xoico_source_s_create()));
        xsource->target = o;
    
        sc_t file_extension = bcore_file_extension(source_path );
    
        st_s_copy_sc(&(xsource->full_path),source_path );
        st_s_copy_sc(&(xsource->name),((st_s*)BLM_LEVEL_T_PUSH(1,st_s,bcore_file_strip_extension(bcore_file_name(source_path ) )))->sc );
        st_s_copy_sc(&(xsource->ext),file_extension );
    
        if( bcore_file_exists(source_path ) )
        {
            BLM_TRY(xoico_source_s_parse(xsource,((const xoico_host*)(o)),((x_source*)( ((bcore_source*)BLM_LEVEL_A_PUSH(0,bcore_file_open_source(source_path ))))), group_name, trait_name, embed_method ))
        }
        else
        {
            BLM_RETURNV(er_t, bcore_error_push_fa(((tp_t)(TYPEOF_general_error)), "Could not open '#<sc_t>'.", source_path ))
        }
    
        xoico_target_s_push_d(o,((xoico_source_s*)bcore_fork(xsource)) );
    BLM_DOWN();}
    
    BLM_RETURNV(er_t, 0)
}

tp_t xoico_target_s_get_hash( const xoico_target_s* o )
{
    // xoico_target.x:161:1
    BLM_INIT_LEVEL(0);
    tp_t hash = bcore_tp_init();
    hash = bcore_tp_fold_tp( hash, o->pre_hash );
    hash = bcore_tp_fold_tp( hash, o->compiler->target_pre_hash );
    hash = bcore_tp_fold_tp( hash, o->_ );
    hash = bcore_tp_fold_sc( hash, o->name.sc );
    hash = bcore_tp_fold_sc( hash, o->ext.sc );
    hash = bcore_tp_fold_bl( hash, o->define_signal_handler );
    
    if( o->cengine ) hash = bcore_tp_fold_tp( hash, xoico_a_get_hash(((const xoico*)(o->cengine))) );
    if( o->copyright_and_license_terms )  hash = bcore_tp_fold_sc( hash, o->copyright_and_license_terms->sc );
    
    {const xoico_target_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_source_s* e=__a->data[__i]; hash = bcore_tp_fold_tp( hash, xoico_source_s_get_hash(e) );}}
    
    if( o->dependencies.size > 0 )
    {
        bcore_arr_tp_s* arr_tp = ((bcore_arr_tp_s*)BLM_LEVEL_T_PUSH(0,bcore_arr_tp_s,bcore_arr_tp_s_create()));
    
        {const bcore_arr_sz_s* __a=&(o->dependencies );if(__a)for(sz_t __i=0;__i<__a->size;__i++){sz_t target_idx=__a->data[__i]; bcore_arr_tp_s_push(arr_tp,xoico_target_s_get_hash(o->compiler->data[ target_idx ]) );}}
    
        // we sort to make the accrued hash independent of the order of dependencies
        bcore_arr_tp_s_sort(arr_tp,1 );
    
        {const bcore_arr_tp_s* __a=arr_tp ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){tp_t tp=__a->data[__i]; hash = bcore_tp_fold_tp( hash, tp );}}
    }
    
    BLM_RETURNV(tp_t, hash)
}

bl_t xoico_target_s_is_cyclic_recursive( xoico_target_s* o )
{
    // xoico_target.x:194:1
    
    if( o->flag ) return  true;
    o->flag = true;
    {const bcore_arr_sz_s* __a=&(o->dependencies );if(__a)for(sz_t __i=0;__i<__a->size;__i++){sz_t target_idx=__a->data[__i]; if( xoico_target_s_is_cyclic_recursive(o->compiler->data[ target_idx ]) ) return  true;
    }}o->flag = false;
    return  false;
}

bl_t xoico_target_s_is_cyclic( xoico_target_s* o )
{
    // xoico_target.x:205:1
    
    xoico_compiler_s_clear_flags(o->compiler);
    bl_t cyclic = xoico_target_s_is_cyclic_recursive(o);
    xoico_compiler_s_clear_flags(o->compiler);
    return  cyclic;
}

er_t xoico_target_s_set_dependencies( xoico_target_s* o, const bcore_arr_sz_s* dependencies )
{
    // xoico_target.x:215:1
    BLM_INIT_LEVEL(0);
    sz_t targets = o->compiler->size;
    
    /// sort, remove duplicates, copy
    bcore_arr_sz_s* dst =&( o->dependencies);
    bcore_arr_sz_s_set_size(dst,0 );
    bcore_arr_sz_s* src = bcore_arr_sz_s_sort(((bcore_arr_sz_s*)BLM_LEVEL_T_PUSH(0,bcore_arr_sz_s,bcore_arr_sz_s_clone(dependencies))),1 );
    for(sz_t i = 0; i < src->size; i++ )
    {
        if( i == 0 || src->data[ i ] != src->data[ i - 1 ] )
        {
            sz_t idx = src->data[ i ];
            ASSERT( idx >= 0 && idx < targets );
            bcore_arr_sz_s_push(dst,idx );
        }
    }
    
    if( xoico_target_s_is_cyclic(o) )
    {
        BLM_RETURNV(er_t, bcore_error_push_fa(((tp_t)(TYPEOF_general_error)), "Cyclic dependencies found in target '#<sc_t>'.", o->name.sc ))
    }
    
    BLM_RETURNV(er_t, 0)
}

er_t xoico_target_s_expand_update_time( const xoico_target_s* o, sz_t indent, x_sink* sink )
{
    // xoico_target.x:243:1
    BLM_INIT_LEVEL(0);
    bcore_cday_utc_s* time = ((bcore_cday_utc_s*)BLM_LEVEL_T_PUSH(0,bcore_cday_utc_s,bcore_cday_utc_s_create()));
    bcore_cday_utc_s_from_system( time );
    x_sink_push_fa(sink,"//  Last update: " ); bcore_cday_utc_s_to_sink( time, ( bcore_sink* )sink ); x_sink_push_fa(sink,"\n" );
    BLM_RETURNV(er_t, 0)
}

er_t xoico_target_s_expand_heading( const xoico_target_s* o, sz_t indent, x_sink* sink )
{
    // xoico_target.x:253:1
    BLM_INIT_LEVEL(0);
    x_sink_push_fa(sink,"/** This file was generated from xoila source code.\n" );
    x_sink_push_fa(sink," *  Compiling Agent : XOICO (C) 2020 ... 2024 J.B.Steffens\n" );
    x_sink_push_fa(sink," *  Note that any changes of this file can be erased or overwritten by XOICO.\n" );
    x_sink_push_fa(sink," *\n" );
    
    if( o->copyright_and_license_terms )
    {BLM_INIT_LEVEL(1);
        st_s st;BLM_T_INIT_SPUSH(st_s, &st);st_s_copy(&(st),o->copyright_and_license_terms );
        st_s_replace_sc_sc(&(st),"\n", " \n *  " );
        x_sink_push_fa(sink," *  #<st_s*>\n", (&(st)) );
    BLM_DOWN();}
    else
    {
        x_sink_push_fa(sink," *  Copyright and License of this File:\n" );
        x_sink_push_fa(sink," *\n" );
        x_sink_push_fa(sink," *  Unless explicitly stated otherwise in governing license terms, this file inherits the\n" );
        x_sink_push_fa(sink," *  copyright and license terms of the immediate source code from which it was compiled.\n" );
        x_sink_push_fa(sink," *\n" );
    }
    
    x_sink_push_fa(sink," *  The immediate source code is distributed across following files:\n" );
    
    {const xoico_target_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_source_s* e=__a->data[__i]; x_sink_push_fa(sink," *  #<sc_t>.#<sc_t>\n", e->name.sc, e->ext.sc );}}
    
    {
        bcore_arr_st_s* arr = ((bcore_arr_st_s*)BLM_LEVEL_T_PUSH(0,bcore_arr_st_s,bcore_arr_st_s_create()));
        xoico_target_s_explicit_embeddings_push(o,arr );
        bcore_arr_st_s_sort(arr,1 );
        if( arr->size > 0 )
        {
            st_s* prev_file = NULL;
            {const bcore_arr_st_s* __a=arr ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){st_s* file=__a->data[__i];
            {
                if( !st_s_equal_st(file,prev_file ) ) x_sink_push_fa(sink," *  #<sc_t>\n", file->sc );
                prev_file = file;
            }
        }}}
    }
    
    
    x_sink_push_fa(sink," *\n" );
    x_sink_push_fa(sink," */\n" );
    BLM_RETURNV(er_t, 0)
}

er_t xoico_target_s_expand_h( const xoico_target_s* o, sz_t indent, x_sink* sink, tp_t* body_signature )
{
    // xoico_target.x:302:1
    BLM_INIT_LEVEL(0);
    BLM_TRY(xoico_target_s_expand_update_time(o,indent, sink ))
    
    st_s sink_buf;BLM_T_INIT_SPUSH(st_s, &sink_buf);;
    
    BLM_TRY(xoico_target_s_expand_heading(o,indent, sink ))
    st_s_push_fa(&(sink_buf),"\n" );
    st_s_push_fa(&(sink_buf),"#rn{ }##ifndef __#<sc_t>_#<sc_t>_H\n", indent, o->name.sc, o->ext.sc );
    st_s_push_fa(&(sink_buf),"#rn{ }##define __#<sc_t>_#<sc_t>_H\n", indent, o->name.sc, o->ext.sc );
    
    st_s_push_fa(&(sink_buf),"\n" );
    st_s_push_fa(&(sink_buf),"#rn{ }##include \"bcore_control.h\"\n", indent );
    st_s_push_fa(&(sink_buf),"#rn{ }##include \"bcore_xoila.h\"\n", indent );
    
    /// include generated headers this target depends on
    {const bcore_arr_sz_s* __a=&(o->dependencies );if(__a)for(sz_t __i=0;__i<__a->size;__i++){sz_t target_idx=__a->data[__i];
    {
        const xoico_target_s* target = o->compiler->data[ target_idx ];
        st_s_push_fa(&(sink_buf),"#rn{ }##include \"#<sc_t>.h\"\n", indent, target->include_path.sc );
    }
    
    }}st_s_push_fa(&(sink_buf),"\n" );
    
    st_s_push_fa(&(sink_buf),"#rn{ }##define TYPEOF_#<sc_t> 0x#pl16'0'{#X<tp_t>}ull\n", indent, o->name.sc, btypeof( o->name.sc ) );
    
    {const xoico_target_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_source_s* e=__a->data[__i]; BLM_TRY(xoico_source_s_expand_declaration(e,((const xoico_host*)(o)), indent,((x_sink*)(&( sink_buf )))))
    
    }}st_s_push_fa(&(sink_buf),"\n" );
    st_s_push_fa(&(sink_buf),"#rn{ }/*#rn{*}*/\n", indent, sz_max( 0, 116 - indent ) );
    st_s_push_fa(&(sink_buf),"\n" );
    st_s_push_fa(&(sink_buf),"#rn{ }vd_t #<sc_t>_#<sc_t>_signal_handler( const bcore_signal_s* o );\n", indent, o->name.sc, o->ext.sc );
    
    st_s_push_fa(&(sink_buf),"\n" );
    st_s_push_fa(&(sink_buf),"#rn{ }/*#rn{*}*/\n", indent, sz_max( 0, 116 - indent ) );
    st_s_push_fa(&(sink_buf),"// Manifesto\n" );
    st_s_push_fa(&(sink_buf),"\n" );
    {const xoico_target_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_source_s* e=__a->data[__i]; BLM_TRY(xoico_source_s_expand_manifesto(e,((const xoico_host*)(o)), indent,((x_sink*)(&( sink_buf )))))
    
    }}st_s_push_fa(&(sink_buf),"\n" );
    st_s_push_fa(&(sink_buf),"#rn{ }##endif // __#<sc_t>_#<sc_t>_H\n", indent, o->name.sc, o->ext.sc );
    
    x_sink_push_data(sink,( vc_t )sink_buf.sc, sink_buf.size );
    
    tp_t body_hash = bcore_tp_fold_sc( bcore_tp_init(), sink_buf.sc );
    x_sink_push_fa(sink,"// XOICO_BODY_SIGNATURE 0x#pl16'0'{#X<tp_t>}\n", body_hash );
    if( body_signature ) (*(body_signature)) = body_hash;
    
    BLM_RETURNV(er_t, 0)
}

er_t xoico_target_s_expand_init1( const xoico_target_s* o, sz_t indent, x_sink* sink )
{
    // xoico_target.x:355:1
    
    return  0;
}

er_t xoico_target_s_expand_c( const xoico_target_s* o, sz_t indent, x_sink* sink, tp_t* body_signature )
{
    // xoico_target.x:362:1
    BLM_INIT_LEVEL(0);
    st_s sink_buf;BLM_T_INIT_SPUSH(st_s, &sink_buf);;
    tp_t body_hash = bcore_tp_init();
    
    BLM_TRY(xoico_target_s_expand_update_time(o,indent, sink ))
    
    BLM_TRY(xoico_target_s_expand_heading(o,indent,((x_sink*)(&( sink_buf )))))
    
    st_s_push_fa(&(sink_buf),"\n" );
    st_s_push_fa(&(sink_buf),"#rn{ }##include \"#<sc_t>.h\"\n", indent, o->include_path.sc );
    st_s_push_fa(&(sink_buf),"#rn{ }##include \"bcore_spect.h\"\n", indent );
    st_s_push_fa(&(sink_buf),"#rn{ }##include \"bcore_spect_inst.h\"\n", indent );
    st_s_push_fa(&(sink_buf),"#rn{ }##include \"bcore_sr.h\"\n", indent );
    st_s_push_fa(&(sink_buf),"#rn{ }##include \"bcore_const_manager.h\"\n", indent );
    st_s_push_fa(&(sink_buf),"\n" );
    
    body_hash = bcore_tp_fold_sc( body_hash, sink_buf.sc );
    x_sink_push_data(sink,( vc_t )sink_buf.sc, sink_buf.size );
    st_s_clear(&(sink_buf));
    
    x_sink_push_fa(sink,"#rn{ }// To force a rebuild of this target by xoico, reset the hash key value below to 0.\n", indent );
    x_sink_push_fa(sink,"#rn{ }// HKEYOF_#<sc_t> 0x#pl16'0'{#X<tp_t>}ull\n", indent, o->name.sc, xoico_target_s_get_hash(o) );
    
    /// definition section
    {const xoico_target_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_source_s* e=__a->data[__i]; BLM_TRY(xoico_source_s_expand_definition(e,((const xoico_host*)(o)), indent,((x_sink*)(&( sink_buf )))))
    
    /// signal section
    }}st_s_push_fa(&(sink_buf),"\n" );
    st_s_push_fa(&(sink_buf),"#rn{ }/*#rn{*}*/\n", indent, sz_max( 0, 116 - indent ) );
    st_s_push_fa(&(sink_buf),"\n" );
    
    /// prototypes of signal handlers this target depends on
    {const bcore_arr_sz_s* __a=&(o->dependencies );if(__a)for(sz_t __i=0;__i<__a->size;__i++){sz_t target_idx=__a->data[__i];
    {
        const xoico_target_s* target = o->compiler->data[ target_idx ];
        if( target->signal_handler_name.size > 0 )
        {
            st_s_push_fa(&(sink_buf),"#rn{ }vd_t #<sc_t>( const bcore_signal_s* o );\n", indent, target->signal_handler_name.sc );
        }
    }
    
    }}st_s_push_fa(&(sink_buf),"\n" );
    st_s_push_fa(&(sink_buf),"#rn{ }vd_t #<sc_t>_#<sc_t>_signal_handler( const bcore_signal_s* o )\n", indent, o->name.sc, o->ext.sc );
    st_s_push_fa(&(sink_buf),"#rn{ }{\n", indent );
    st_s_push_fa(&(sink_buf),"#rn{ }    switch( bcore_signal_s_handle_type( o, typeof( \"#<sc_t>_#<sc_t>\" ) ) )\n", indent, o->name.sc, o->ext.sc );
    st_s_push_fa(&(sink_buf),"#rn{ }    {\n", indent );
    
    st_s_push_fa(&(sink_buf),"#rn{ }        case TYPEOF_init1:\n", indent );
    st_s_push_fa(&(sink_buf),"#rn{ }        {\n", indent );
    BLM_TRY(xoico_target_s_expand_init1(o,indent + 12,((x_sink*)(&( sink_buf )))))
    {const xoico_target_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_source_s* e=__a->data[__i]; BLM_TRY(xoico_source_s_expand_init1(e,((const xoico_host*)(o)), indent + 12,((x_sink*)(&( sink_buf )))))
    }}st_s_push_fa(&(sink_buf),"#rn{ }        }\n", indent );
    st_s_push_fa(&(sink_buf),"#rn{ }        break;\n", indent );
    
    st_s_push_fa(&(sink_buf),"#rn{ }        case TYPEOF_down1:\n", indent );
    st_s_push_fa(&(sink_buf),"#rn{ }        {\n", indent );
    BLM_TRY(xoico_target_s_expand_init1(o,indent + 12,((x_sink*)(&( sink_buf )))))
    {const xoico_target_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_source_s* e=__a->data[__i]; BLM_TRY(xoico_source_s_expand_down1(e,((const xoico_host*)(o)), indent + 12,((x_sink*)(&( sink_buf )))))
    }}st_s_push_fa(&(sink_buf),"#rn{ }        }\n", indent );
    st_s_push_fa(&(sink_buf),"#rn{ }        break;\n", indent );
    
    if( o->dependencies.size > 0 )
    {
        st_s_push_fa(&(sink_buf),"#rn{ }        case TYPEOF_push_dependencies:\n", indent );
        st_s_push_fa(&(sink_buf),"#rn{ }        {\n", indent );
        st_s_push_fa(&(sink_buf),"#rn{ }            ASSERT( o->object && ( *( aware_t* )o->object ) == TYPEOF_bcore_arr_fp_s );\n", indent );
        st_s_push_fa(&(sink_buf),"#rn{ }            bcore_arr_fp_s* arr_fp = o->object;\n", indent );
        {const bcore_arr_sz_s* __a=&(o->dependencies );if(__a)for(sz_t __i=0;__i<__a->size;__i++){sz_t target_idx=__a->data[__i];
        {
            const xoico_target_s* target = o->compiler->data[ target_idx ];
            if( target->signal_handler_name.size > 0 )
            {
                st_s_push_fa(&(sink_buf),"#rn{ }            bcore_arr_fp_s_push( arr_fp, ( fp_t )#<sc_t> );\n", indent, target->signal_handler_name.sc );
            }
        }
        }}st_s_push_fa(&(sink_buf),"#rn{ }        }\n", indent );
        st_s_push_fa(&(sink_buf),"#rn{ }        break;\n", indent );
    }
    
    st_s_push_fa(&(sink_buf),"#rn{ }        default: break;\n", indent );
    st_s_push_fa(&(sink_buf),"#rn{ }    }\n", indent );
    st_s_push_fa(&(sink_buf),"#rn{ }    return NULL;\n", indent );
    st_s_push_fa(&(sink_buf),"#rn{ }}\n", indent );
    
    if( o->define_signal_handler )
    {
        st_s_push_fa(&(sink_buf),"#rn{ }BETH_SIGNAL_DEFINE( #<sc_t> )\n", indent, o->name.sc );
    }
    
    if( o->main_function )
    {
        bl_t c_style = ( o->main_function->signature->args.size == 2 );
    
        st_s_push_fa(&(sink_buf),"\n" );
        st_s_push_fa(&(sink_buf),"#rn{ }int main( int argc, char** argv )\n", indent );
        st_s_push_fa(&(sink_buf),"#rn{ }{\n", indent );
        st_s_push_fa(&(sink_buf),"#rn{ }    BETH_USE( #<sc_t> );\n", indent, o->name.sc );
    
    
        if( c_style )
        {
            st_s_push_fa(&(sink_buf),"#rn{ }    int retv = #<sc_t>( argc, argv );\n", indent, xoico_compiler_s_nameof(o->compiler,o->main_function->global_name ) );
        }
        else
        {
            st_s_push_fa(&(sink_buf),"#rn{ }    bcore_arr_st_s* args = bcore_arr_st_s_create();\n", indent );
            st_s_push_fa(&(sink_buf),"#rn{ }    for( sz_t i = 0; i < argc; i++ ) bcore_arr_st_s_push_sc( args, argv[ i ] );\n", indent );
            st_s_push_fa(&(sink_buf),"#rn{ }    int retv = #<sc_t>( args );\n", indent, xoico_compiler_s_nameof(o->compiler,o->main_function->global_name ) );
            st_s_push_fa(&(sink_buf),"#rn{ }    bcore_arr_st_s_discard( args );\n", indent );
        }
    
        st_s_push_fa(&(sink_buf),"#rn{ }    BETH_CLOSEV( 0 );\n", indent );
        st_s_push_fa(&(sink_buf),"#rn{ }    return retv;\n", indent );
        st_s_push_fa(&(sink_buf),"#rn{ }}\n", indent );
    }
    
    body_hash = bcore_tp_fold_sc( body_hash, sink_buf.sc );
    x_sink_push_data(sink,( vc_t )sink_buf.sc, sink_buf.size );
    st_s_clear(&(sink_buf));
    
    x_sink_push_fa(sink,"// XOICO_BODY_SIGNATURE 0x#pl16'0'{#X<tp_t>}\n", body_hash );
    if( body_signature ) (*(body_signature)) = body_hash;
    
    BLM_RETURNV(er_t, 0)
}

er_t xoico_target_s_expand_state( const xoico_target_s* o, x_sink* sink )
{
    // xoico_target.x:491:1
    
    x_sink_push_fa(sink,"HKEYOF_#<sc_t> 0x#pl16'0'{#X<tp_t>}\n", o->name.sc, xoico_target_s_get_hash(o) );
    return  0;
}

bl_t xoico_target_s_to_be_modified( const xoico_target_s* o )
{
    // xoico_target.x:499:1
    BLM_INIT_LEVEL(0);
    if( o->compiler->always_expand ) BLM_RETURNV(bl_t, true)
    
    bl_t to_be_modified = true;
    
    tp_t target_hash = xoico_target_s_get_hash(o);
    
    st_s* file = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create_fa("#<sc_t>.state", o->output_path.sc )));
    if( bcore_file_exists(file->sc ) )
    {
        st_s* key_defined = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create_fa("##?w'HKEYOF_#<sc_t>'", o->name.sc )));
        x_source* source =((x_source*)( ((bcore_source*)BLM_LEVEL_A_PUSH(0,bcore_file_open_source(file->sc )))));
        while( !x_source_eos(source) )
        {
            if( x_source_inspect_char(source) == 'H' )
            {
                if( x_source_parse_bl(source,key_defined->sc ) )
                {
                    tp_t key_val = 0;
                    BLM_TRY_EXIT(x_source_parse_fa(source," #<tp_t*>", &key_val ))
                    to_be_modified = ( key_val != target_hash );
                    break;
                }
            }
            else
            {
                x_source_get_char(source);
            }
        }
    }
    
    BLM_RETURNV(bl_t, to_be_modified)
}

er_t xoico_target_s_expand_phase1( xoico_target_s* o, bl_t* p_modified )
{
    // xoico_target.x:537:1
    BLM_INIT_LEVEL(0);
    st_s_attach( &(o->target_h ),  NULL);
    st_s_attach( &(o->target_c ),  NULL);
    st_s_attach( &(o->target_state ),  NULL);
    o->modified = false;
    
    if( xoico_target_s_to_be_modified(o) )
    {
        o->target_h = st_s_create();
        o->target_c = st_s_create();
        o->target_state = st_s_create();
        if( !o->compiler->dry_run )
        {
            BLM_TRY(xoico_target_s_expand_h(o,0,((x_sink*)( o->target_h)), (&(o->body_signature_h)) ))
            BLM_TRY(xoico_target_s_expand_c(o,0,((x_sink*)( o->target_c)), (&(o->body_signature_c)) ))
            BLM_TRY(xoico_target_s_expand_state(o,((x_sink*)(o->target_state ))))
        }
        else
        {
            st_s* buf = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
            BLM_TRY(xoico_target_s_expand_h(o,0,((x_sink*)( buf)), (&(o->body_signature_h)) ))
            BLM_TRY(xoico_target_s_expand_c(o,0,((x_sink*)( buf)), (&(o->body_signature_c)) ))
            BLM_TRY(xoico_target_s_expand_state(o,((x_sink*)(buf ))))
        }
        o->modified = true;
    }
    
    if( p_modified ) (*(p_modified)) = o->modified;
    
    BLM_RETURNV(er_t, 0)
}

er_t xoico_target_s_expand_phase2( xoico_target_s* o, bl_t* p_modified )
{
    // xoico_target.x:584:1
    BLM_INIT_LEVEL(0);
    if( !o->modified )
    {
        if( (p_modified) ) (*(p_modified)) = false;
        BLM_RETURNV(er_t, 0)
    }
    
    ASSERT( o->target_h );
    ASSERT( o->target_c );
    
    st_s* file_h = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create_fa("#<sc_t>.h", o->output_path.sc )));
    st_s* file_c = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create_fa("#<sc_t>.c", o->output_path.sc )));
    st_s* file_state = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create_fa("#<sc_t>.state", o->output_path.sc )));
    
    if( o->readonly )
    {
    //        bcore_msg_fa( "Affected: #<sc_t>\n", file_h.sc );
    //        bcore_msg_fa( "Affected: #<sc_t>\n", file_c.sc );
        if( (p_modified) ) (*(p_modified)) = false;
    }
    else
    {
        {
            bl_t clear_to_overwrite = false;
            tp_t body_signature = o->update_target_on_body_signature ? o->body_signature_h : 0;
            BLM_TRY(xoico_compiler_s_check_overwrite(o->compiler,file_h->sc, body_signature, (&(clear_to_overwrite)) ))
            if( clear_to_overwrite )
            {
                bcore_msg_fa( "Writing: #<sc_t>\n", file_h->sc );
                BLM_TRY(xoico_target_write_with_signature(file_h->sc, o->target_h ))
                if( (p_modified) ) (*(p_modified)) = true;
            }
        }
    
        {
            bl_t clear_to_overwrite = false;
            tp_t body_signature = o->update_target_on_body_signature ? o->body_signature_c : 0;
            BLM_TRY(xoico_compiler_s_check_overwrite(o->compiler,file_c->sc, body_signature, (&(clear_to_overwrite)) ))
            if( clear_to_overwrite )
            {
                bcore_msg_fa( "Writing: #<sc_t>\n", file_c->sc );
                BLM_TRY(xoico_target_write_with_signature(file_c->sc, o->target_c ))
                if( (p_modified) ) (*(p_modified)) = true;
            }
        }
    
        {
            bl_t clear_to_overwrite = false;
            BLM_TRY(xoico_compiler_s_check_overwrite(o->compiler,file_state->sc, 0, (&(clear_to_overwrite)) ))
            if( clear_to_overwrite )
            {
                bcore_msg_fa( "Writing: #<sc_t>\n", file_state->sc );
                BLM_TRY(xoico_target_write_with_signature(file_state->sc, o->target_state ))
                if( (p_modified) ) (*(p_modified)) = true;
            }
        }
    
    }
    BLM_RETURNV(er_t, 0)
}

XOILA_DEFINE_SPECT( xoico, xoico_target )
"{"
    "bcore_spect_header_s header;"
"}";

er_t xoico_target_write_with_signature( sc_t file, const st_s* data )
{
    // xoico_target.x:572:1
    BLM_INIT_LEVEL(0);
    tp_t hash = bcore_tp_fold_sc( bcore_tp_init(), data->sc );
    x_sink* sink =((x_sink*)( ((bcore_sink*)BLM_LEVEL_A_PUSH(0,bcore_file_open_sink(file )))));
    x_sink_push_data(sink,( vc_t )data->data, data->size );
    x_sink_push_fa(sink,"// XOICO_FILE_SIGNATURE 0x#pl16'0'{#X<tp_t>}\n", hash );
    BLM_RETURNV(er_t, 0)
}

/**********************************************************************************************************************/
// source: xoico_compiler.x

//----------------------------------------------------------------------------------------------------------------------
// group: xoico_compiler

BCORE_DEFINE_OBJECT_INST_P( xoico_compiler_type_info_s )
"aware xoico_compiler"
"{"
    "xoico_typespec_s typespec;"
    "hidden aware xoico* item;"
"}";

BCORE_DEFINE_OBJECT_INST_P( xoico_compiler_element_info_s )
"aware xoico_compiler"
"{"
    "xoico_compiler_type_info_s type_info;"
    "hidden xoico_func_s* func;"
"}";

BCORE_DEFINE_OBJECT_INST_P( xoico_compiler_s )
"aware xoico_compiler"
"{"
    "hidden xoico_target_s => [];"
    "hidden bcore_life_s life;"
    "hidden bcore_hmap_tpvd_s hmap_group;"
    "hidden bcore_hmap_tpvd_s hmap_item;"
    "hidden bcore_hmap_tpvd_s hmap_func;"
    "hidden bcore_hmap_tp_s hmap_external_type;"
    "hidden bcore_hmap_tp_s hmap_external_identifier;"
    "hidden bcore_hmap_tp_s hmap_declared_name;"
    "hidden bcore_hmap_name_s name_map;"
    "hidden bl_t has_main_function = false;"
    "tp_t target_pre_hash = 71;"
    "bl_t work_build_time_into_pre_hash = true;"
    "bl_t register_signatures = false;"
    "bl_t overwrite_unsigned_target_files = false;"
    "bl_t always_expand = false;"
    "bl_t dry_run = false;"
    "sz_t verbosity = 1;"
    "func bcore_inst_call:init_x;"
    "func xoico:finalize;"
    "func xoico:expand_setup;"
"}";

bl_t xoico_compiler_s_is_type( const xoico_compiler_s* o, tp_t name )
{
    // xoico_compiler.x:152:5
    
    if( xoico_compiler_s_is_group(o,name ) ) return  true;
    if( xoico_compiler_s_is_stamp(o,name ) ) return  true;
    if( bcore_hmap_tp_s_exists(&(o->hmap_external_type),name ) ) return  true;
    return  false;
}

bl_t xoico_compiler_s_is_identifier( const xoico_compiler_s* o, tp_t name )
{
    // xoico_compiler.x:160:5
    
    if( xoico_compiler_s_is_type(o,name ) ) return  true;
    if( bcore_hmap_tp_s_exists(&(o->hmap_declared_name),name ) ) return  true;
    if( bcore_hmap_tp_s_exists(&(o->hmap_external_identifier),name ) ) return  true;
    return  false;
}

bl_t xoico_compiler_s_is_name( const xoico_compiler_s* o, tp_t name )
{
    // xoico_compiler.x:168:5
    
    if( bcore_hmap_tp_s_exists(&(o->hmap_declared_name),name ) ) return  true;
    return  false;
}

bl_t xoico_compiler_s_is_stamp( const xoico_compiler_s* o, tp_t name )
{
    // xoico_compiler.x:174:5
    
    const xoico* item = xoico_compiler_s_get_const_item(o,name );
    if( item && ( item->_ == ((tp_t)(TYPEOF_xoico_stamp_s)) ) ) return  true;
    return  false;
}

bl_t xoico_compiler_s_is_body( const xoico_compiler_s* o, tp_t name )
{
    // xoico_compiler.x:181:5
    
    const xoico* item = xoico_compiler_s_get_const_item(o,name );
    if( item && ( item->_ == ((tp_t)(TYPEOF_xoico_body_s)) ) ) return  true;
    return  false;
}

bl_t xoico_compiler_s_is_signature( const xoico_compiler_s* o, tp_t name )
{
    // xoico_compiler.x:188:5
    
    const xoico* item = xoico_compiler_s_get_const_item(o,name );
    if( item && ( item->_ == ((tp_t)(TYPEOF_xoico_signature_s)) ) ) return  true;
    return  false;
}

bl_t xoico_compiler_s_is_signature_or_feature( const xoico_compiler_s* o, tp_t name )
{
    // xoico_compiler.x:195:5
    
    const xoico* item = xoico_compiler_s_get_const_item(o,name );
    if( !item ) return  NULL;
    if( item->_ == ((tp_t)(TYPEOF_xoico_signature_s)) || item->_ == ((tp_t)(TYPEOF_xoico_feature_s)) ) return  true;
    return  false;
}

bl_t xoico_compiler_s_is_feature( const xoico_compiler_s* o, tp_t name )
{
    // xoico_compiler.x:203:5
    
    const xoico* item = xoico_compiler_s_get_const_item(o,name );
    if( item && ( item->_ == ((tp_t)(TYPEOF_xoico_feature_s)) ) ) return  true;
    return  false;
}

const xoico* xoico_compiler_s_get_const_item( const xoico_compiler_s* o, tp_t name )
{
    // xoico_compiler.x:210:5
    
    const vd_t* ptr = bcore_hmap_tpvd_s_get(&(o->hmap_item),name );
    return  ptr ? ( const xoico* )*ptr : NULL;
}

xoico* xoico_compiler_s_get_item( xoico_compiler_s* o, tp_t name )
{
    // xoico_compiler.x:216:5
    
    const vd_t* ptr = bcore_hmap_tpvd_s_get(&(o->hmap_item),name );
    return  ptr ? ( xoico* )*ptr : NULL;
}

xoico_stamp_s* xoico_compiler_s_get_stamp( xoico_compiler_s* o, tp_t name )
{
    // xoico_compiler.x:222:5
    
    const xoico* item = xoico_compiler_s_get_const_item(o,name );
    return  ( item ) ? ( item->_ == ((tp_t)(TYPEOF_xoico_stamp_s)) ) ? ((xoico_stamp_s*)(item)) : NULL : NULL;
}

xoico_body_s* xoico_compiler_s_get_body( xoico_compiler_s* o, tp_t name )
{
    // xoico_compiler.x:228:5
    
    const xoico* item = xoico_compiler_s_get_const_item(o,name );
    return  ( item ) ? ( item->_ == ((tp_t)(TYPEOF_xoico_body_s)) ) ? ((xoico_body_s*)(item)) : NULL : NULL;
}

const xoico_feature_s* xoico_compiler_s_get_feature( const xoico_compiler_s* o, tp_t name )
{
    // xoico_compiler.x:234:5
    
    const xoico* item = xoico_compiler_s_get_const_item(o,name );
    return  ( item ) ? ( item->_ == ((tp_t)(TYPEOF_xoico_feature_s)) ) ? ((xoico_feature_s*)(item)) : NULL : NULL;
}

const xoico_signature_s* xoico_compiler_s_get_signature( const xoico_compiler_s* o, tp_t name )
{
    // xoico_compiler.x:240:5
    
    const xoico* item = xoico_compiler_s_get_const_item(o,name );
    if( !item ) return  NULL;
    if( item->_ == ((tp_t)(TYPEOF_xoico_signature_s)) ) return  ((xoico_signature_s*)(item));
    if( item->_ == ((tp_t)(TYPEOF_xoico_feature_s))   ) return &( ((xoico_feature_s*)(item))->signature);
    return  NULL;
}

xoico_group_s* xoico_compiler_s_get_group( xoico_compiler_s* o, tp_t name )
{
    // xoico_compiler.x:249:5
    
    const vd_t* ptr = bcore_hmap_tpvd_s_get(&(o->hmap_group),name );
    return  ptr ? ( xoico_group_s* )*ptr : NULL;
}

xoico_transient_map_s* xoico_compiler_s_get_transient_map( xoico_compiler_s* o, tp_t type )
{
    // xoico_compiler.x:255:5
    
    xoico_stamp_s* stamp = xoico_compiler_s_get_stamp(o,type );
    return  stamp ? (&(stamp->transient_map)) : NULL;
}

xoico_func_s* xoico_compiler_s_get_func( xoico_compiler_s* o, tp_t name )
{
    // xoico_compiler.x:261:5
    
    const vd_t* ptr = bcore_hmap_tpvd_s_get(&(o->hmap_func),name );
    return  ptr ? ( xoico_func_s* )*ptr : NULL;
}

er_t xoico_compiler_s_life_a_push( xoico_compiler_s* o, bcore_inst* object )
{
    // xoico_compiler.x:267:5
    
    bcore_life_s_push_aware( (&(o->life)), object );
    return  0;
}

sc_t xoico_compiler_s_nameof( const xoico_compiler_s* o, tp_t type )
{
    // xoico_compiler.x:288:5
    
    sc_t name = bcore_hmap_name_s_get_sc(&(o->name_map),type );
    if( !name ) name = ifnameof( type );
    return  name;
}

void xoico_compiler_s_init_x( xoico_compiler_s* o )
{
    // xoico_compiler.x:295:5
    
    if( o->work_build_time_into_pre_hash )
    {
        o->target_pre_hash = bcore_tp_fold_sc( o->target_pre_hash, __DATE__ );
        o->target_pre_hash = bcore_tp_fold_sc( o->target_pre_hash, __TIME__ );
    }
}

void xoico_compiler_s_push_d( xoico_compiler_s* o, xoico_target_s* target )
{
    // xoico_compiler.x:304:5
    
    x_array_push_d(((x_array*)(o)),((x_inst*)(target )));
}

er_t xoico_compiler_s_register_group( xoico_compiler_s* o, const xoico_group_s* group )
{
    // xoico_compiler.x:319:1
    
    if( bcore_hmap_tpvd_s_exists(&(o->hmap_group),group->tp_name ) ) return  0;
    bcore_hmap_tpvd_s_set(&(o->hmap_group),group->tp_name, ( vd_t )group );
    return  0;
}

er_t xoico_compiler_s_register_item( xoico_compiler_s* o, const xoico* item )
{
    // xoico_compiler.x:328:1
    
    tp_t global_id = xoico_a_get_global_name_tp(item);
    if( bcore_hmap_tpvd_s_exists(&(o->hmap_item),global_id ) )
    {
        return  x_source_point_s_parse_error_fa(((x_source_point_s*)(xoico_a_get_source_point(item))),"'#<sc_t>' was already registered\n", xoico_compiler_s_nameof(o,global_id ) );
    }
    bcore_hmap_tpvd_s_set(&(o->hmap_item),global_id, ( vd_t )item );
    return  0;
}

er_t xoico_compiler_s_register_func( xoico_compiler_s* o, const xoico_func_s* func )
{
    // xoico_compiler.x:341:1
    
    if( bcore_hmap_tpvd_s_exists(&(o->hmap_func),func->global_name ) )
    {
        xoico_func_s* func_registered =*( ((xoico_func_s**)(bcore_hmap_tpvd_s_get(&(o->hmap_func),func->global_name ))));
    
        if( func_registered == func )
        {
            return  x_source_point_s_parse_error_fa(&(
                func->source_point),"Global func name '#<sc_t>' was already registered from same instance.\n",
                xoico_compiler_s_nameof(o,func->global_name )
            );
        }
        else
        {
            return  x_source_point_s_parse_error_fa(&(
                func->source_point),"Global func name '#<sc_t>' was already registered from different instance.\n"
                "'#<sc_t>' vs '#<sc_t>'\n",
                xoico_compiler_s_nameof(o,func->global_name ),
                xoico_compiler_s_nameof(o,func->name ),
                xoico_compiler_s_nameof(o,func_registered->name )
            );
        }
    }
    
    bcore_hmap_tpvd_s_set(&(o->hmap_func),func->global_name, ( vd_t )func );
    return  0;
}

er_t xoico_compiler_s_check_overwrite( const xoico_compiler_s* o, sc_t file, tp_t body_signature, bl_t* clear_to_overwrite )
{
    // xoico_compiler.x:416:1
    BLM_INIT_LEVEL(0);
    if( clear_to_overwrite ) (*(clear_to_overwrite)) = true;
    if( !bcore_file_exists(file ) ) BLM_RETURNV(er_t, 0)
    
    x_source* source =((x_source*)( ((bcore_source*)BLM_LEVEL_A_PUSH(0,bcore_file_open_source(file )))));
    st_s* data = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
    while( !x_source_eos(source) ) st_s_push_char(data,x_source_get_u0(source) );
    
    if( !xoico_compiler_is_correctly_signed(data ) )
    {BLM_INIT_LEVEL(1);
        st_s* s = ((st_s*)BLM_LEVEL_T_PUSH(1,st_s,st_s_create()));
        st_s_push_fa(s,"Planted file #<sc_t>: Signature check failed.\n", file );
        st_s_push_fa(s,"This file might have been created or edited outside the xoico framework.\n" );
        if( o->overwrite_unsigned_target_files )
        {
            if( clear_to_overwrite ) (*(clear_to_overwrite)) = true;
            st_s_push_fa(s,"Flag 'overwrite_unsigned_target_files' is 'true'. The file will be overwritten.\n" );
            x_sink_push_fa(x_sink_stderr(),"\nWARNING: #<sc_t>\n", s->sc );
        }
        else
        {
            if( clear_to_overwrite ) (*(clear_to_overwrite)) = false;
            st_s_push_fa(s,"Xoico has currently no permission to overwrite unsigned target files.\n" );
            st_s_push_fa(s,"You can fix it in one of following ways:\n" );
            st_s_push_fa(s,"* Rename or (re)move the file.\n" );
            st_s_push_fa(s,"* Use command line flag '-f'.\n" );
            BLM_RETURNV(er_t, bcore_error_push_fa(((tp_t)(TYPEOF_general_error)), "\nERROR: #<sc_t>\n", s->sc ))
        }
    BLM_DOWN();}
    
    if( clear_to_overwrite && body_signature && !o->always_expand )
    {
        (*(clear_to_overwrite)) = xoico_compiler_body_signature(data ) != body_signature;
    }
    
    BLM_RETURNV(er_t, 0)
}

er_t xoico_compiler_s_parse( xoico_compiler_s* o, sc_t target_name, sc_t target_ext, const st_s* target_output_folder, sc_t source_path, sc_t group_name, sc_t trait_name, tp_t embed_method, sz_t* p_target_index )
{
    // xoico_compiler.x:457:1
    BLM_INIT_LEVEL(0);
    const st_s* source_folder = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,bcore_file_folder_path(source_path )));
    const st_s* output_folder = target_output_folder ? target_output_folder : source_folder;
    st_s* target_output_path = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create_fa("#<sc_t>/#<sc_t>.#<sc_t>", output_folder->sc, target_name, target_ext )));
    
    sz_t target_index = -1;
    for(sz_t i = 0; i < o->size; i++ )
    {
        if( sc_t_equal( target_name, o->data[ i ]->name.sc ) )
        {
            target_index = i;
            break;
        }
    }
    
    if( target_index == -1 )
    {
        xoico_target_s* target = xoico_target_s_create();
        target->compiler = o;
        st_s_copy_sc(&(target->name        ),target_name );
        st_s_copy_sc(&(target->ext         ),target_ext );
        st_s_copy_fa(&(target->include_path),"#<sc_t>.#<sc_t>", target_name, target_ext );
        st_s_copy(&(target->output_path),target_output_path );
        xoico_compiler_s_push_d(o,target );
        target_index = o->size - 1;
    }
    
    xoico_target_s* target = o->data[ target_index ];
    
    /// check paths and extensions
    if( !st_s_equal_sc(&(target->ext),target_ext ) )
    {
        ERR_fa( "Target '#<sc_t>': Assigned extension '#<sc_t>' differs from requested extension '#<sc_t>'.", target->name.sc, target->ext.sc, target_ext );
    }
    if( !st_s_equal_st(&(target->output_path),target_output_path ) )
    {
        ERR_fa( "Target '#<sc_t>': Assigned output path '#<sc_t>' differs from requested output path '#<sc_t>'.", target->name.sc, target->output_path.sc, target_output_path->sc );
    }
    
    BLM_TRY(xoico_target_s_parse_from_path(target,source_path, group_name, trait_name, embed_method ))
    if( p_target_index ) (*(p_target_index)) = target_index;
    
    BLM_RETURNV(er_t, 0)
}

er_t xoico_compiler_s_finalize( xoico_compiler_s* o, const xoico_host* host )
{
    // xoico_compiler.x:505:1
    
    {const xoico_compiler_s* __a=o ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_target_s* e=__a->data[__i]; BLM_TRY(xoico_target_s_finalize(e,((const xoico_host*)(o ))))
    }}return  0;
}

er_t xoico_compiler_s_expand_setup( xoico_compiler_s* o, const xoico_host* host )
{
    // xoico_compiler.x:513:1
    
    for(sz_t i = 0; i < o->size; i++ ) BLM_TRY(xoico_target_s_expand_setup(o->data[ i ],((const xoico_host*)(o ))))
    
    /* We obtain all names from the global map.
     * This is necessary because names of reflection elements are parsed outside
     * this framework and remembered by the global map.
     */
    bcore_name_push_all((&(o->name_map)) );
    
    return  0;
}

bl_t xoico_compiler_s_to_be_modified( const xoico_compiler_s* o )
{
    // xoico_compiler.x:529:1
    
    for(sz_t i = 0; i < o->size; i++ )
    {
        if( xoico_target_s_to_be_modified(o->data[ i ]) ) return  true;
    }
    return  false;
}

er_t xoico_compiler_s_expand( xoico_compiler_s* o, bl_t* p_modified )
{
    // xoico_compiler.x:541:1
    
    bl_t modified = false;
    
    BLM_TRY(xoico_compiler_s_expand_setup(o,((const xoico_host*)(o ))))
    for(sz_t i = 0; i < o->size; i++ ) BLM_TRY(xoico_target_s_expand_phase1(o->data[i],(&(modified)) ))
    for(sz_t i = 0; i < o->size; i++ ) BLM_TRY(xoico_target_s_expand_phase2(o->data[i],(&(modified)) ))
    
    if( p_modified ) (*(p_modified)) = modified;
    return  0;
}

bl_t xoico_compiler_s_get_self( const xoico_compiler_s* o, tp_t type, const bcore_self_s** self )
{
    // xoico_compiler.x:555:1
    
    const xoico* item = xoico_compiler_s_get_const_item(o,type );
    if( !item ) return  false;
    
    if( item->_ == ((tp_t)(TYPEOF_xoico_stamp_s)) )
    {
        const xoico_stamp_s* stamp = ((const xoico_stamp_s*)(item));
        if( self ) (*(self)) = stamp->self;
        return  true;
    }
    
    return  false;
}

bl_t xoico_compiler_s_get_type_info( const xoico_compiler_s* o, tp_t type, xoico_compiler_type_info_s* info )
{
    // xoico_compiler.x:572:1
    
    const xoico* item = xoico_compiler_s_get_const_item(o,type );
    if( !item ) return  false;
    ASSERT( info );
    if( item->_ == ((tp_t)(TYPEOF_xoico_stamp_s)) || item->_ == ((tp_t)(TYPEOF_xoico_group_s)) )
    {
        info->item = ((xoico*)(item));
        info->typespec.type = type;
        return  true;
    }
    
    return  false;
}

bl_t xoico_compiler_s_get_type_member_function_info( const xoico_compiler_s* o, tp_t type, tp_t name, xoico_compiler_element_info_s* info )
{
    // xoico_compiler.x:589:1
    
    const xoico* xoico_item = xoico_compiler_s_get_const_item(o,type );
    if( !xoico_item )
    {
        xoico_item =((const xoico*)( xoico_compiler_s_get_group(((xoico_compiler_s*)(o)),type )));
        if( !xoico_item ) return  false;
    }
    
    bl_t success = false;
    ASSERT( info );
    info->type_info.item = ((xoico*)(xoico_item));
    
    if( xoico_item->_ == ((tp_t)(TYPEOF_xoico_stamp_s)) )
    {
        const xoico_stamp_s* stamp = ((const xoico_stamp_s*)(xoico_item));
        const bcore_self_s* self = stamp->self;
        const bcore_self_item_s* self_item = NULL; // returns NULL in case of no match
    
        if( ( info->func = ((xoico_func_s*)(xoico_stamp_s_get_traitline_func_from_name(stamp,name ))) ) ) /// trait-line function
        {
            success = true;
        }
        else if( ( self_item = bcore_self_s_get_item_by_name(self,name ) ) )
        {
            if( self_item->caps == BCORE_CAPS_EXTERNAL_FUNC )
            {
                info->func = ((xoico_func_s*)(xoico_stamp_s_get_func_from_name(stamp,name )));
                ASSERT( info->func );
                success = true;
            }
        }
    }
    else if( xoico_item->_ == ((tp_t)(TYPEOF_xoico_group_s)) )
    {
        const xoico_group_s* group = ((xoico_group_s*)(xoico_item));
        info->func = ((xoico_func_s*)(xoico_group_s_get_traitline_func_from_name(group,name )));
        if( info->func ) success = true;
    }
    
    if( !success )
    {
        if( xoico_compiler_s_is_func(o,name ) )
        {
            info->func = xoico_compiler_s_get_func(((xoico_compiler_s*)(o)),name );
            success = true;
        }
    }
    
    return  success;
}

bl_t xoico_compiler_s_get_type_member_object_info( const xoico_compiler_s* o, tp_t type, tp_t name, xoico_compiler_element_info_s* info )
{
    // xoico_compiler.x:643:1
    
    const xoico* xoico_item = xoico_compiler_s_get_const_item(o,type );
    if( !xoico_item )
    {
        xoico_item =((const xoico*)( xoico_compiler_s_get_group(((xoico_compiler_s*)(o)),type )));
        if( !xoico_item ) return  false;
    }
    
    bl_t success = false;
    ASSERT( info );
    info->type_info.item = ((xoico*)(xoico_item));
    
    if( xoico_item->_ == ((tp_t)(TYPEOF_xoico_stamp_s)) )
    {
        const xoico_stamp_s* stamp = ((const xoico_stamp_s*)(xoico_item));
        const bcore_self_s* self = stamp->self;
        const bcore_self_item_s* self_item = NULL; // returns NULL in case of no match
    
        if( ( self_item = bcore_self_s_get_item_by_name(self,name ) ) )
        {
            if( bcore_flect_caps_is_array_fix( self_item->caps ) )
            {
                info->type_info.typespec.access_class = TYPEOF_mutable;
                info->type_info.typespec.type = self_item->type;
                info->type_info.typespec.indirection = 1;
                success = true;
            }
            else if( !bcore_flect_caps_is_array_dyn( self_item->caps ) ) // dynamic arrays are handled separately
            {
                sz_t indirection = bcore_flect_caps_get_indirection( self_item->caps );
                info->type_info.typespec.type = self_item->type;
                info->type_info.typespec.indirection = indirection;
                info->type_info.typespec.access_class = ( indirection > 0 ) ? TYPEOF_mutable : 0;
                success = true;
            }
        }
        else if( ( self_item = bcore_self_s_get_first_anonymous_array_item( self ) ) ) /// builtin elements for arrays
        {
            if( name == TYPEOF_size )
            {
                info->type_info.typespec.type = ((tp_t)(TYPEOF_uz_t));
                info->type_info.typespec.indirection = 0;
                info->type_info.typespec.access_class = 0;
                success = true;
            }
            else if( name == TYPEOF_space )
            {
                info->type_info.typespec.type = ((tp_t)(TYPEOF_uz_t));
                info->type_info.typespec.indirection = 0;
                info->type_info.typespec.access_class = 0;
                success = true;
            }
            else if( name == TYPEOF_data )
            {
                info->type_info.typespec.access_class = TYPEOF_mutable;
                info->type_info.typespec.type = self_item->type ? self_item->type : ((tp_t)(TYPEOF_x_inst));
                info->type_info.typespec.indirection = bcore_flect_caps_get_indirection( self_item->caps ) + 1;
                success = true;
            }
            else if( name == TYPEOF_type )
            {
                if( bcore_flect_caps_is_typed( self_item->caps ) )
                {
                    info->type_info.typespec.type = ((tp_t)(TYPEOF_tp_t));
                    info->type_info.typespec.indirection = 0;
                    info->type_info.typespec.access_class = 0;
                    success = true;
                }
            }
        }
    }
    else if( xoico_item->_ == ((tp_t)(TYPEOF_xoico_group_s)) )
    {
        if( name == TYPEOF__ ) // group builtin element '_'
        {
            info->type_info.typespec.type = ((tp_t)(TYPEOF_aware_t));
            info->type_info.typespec.indirection = 0;
            success = true;
        }
    }
    
    return  success;
}

bl_t xoico_compiler_s_get_type_array_element_info( const xoico_compiler_s* o, tp_t type, xoico_compiler_element_info_s* info )
{
    // xoico_compiler.x:730:1
    
    ASSERT( info );
    
    const xoico* xoico_item = xoico_compiler_s_get_const_item(o,type );
    if( !xoico_item ) return  false;
    bl_t success = false;
    info->type_info.item = ((xoico*)(xoico_item));
    
    if( xoico_item->_ == ((tp_t)(TYPEOF_xoico_stamp_s)) )
    {
        const xoico_stamp_s* stamp = ((const xoico_stamp_s*)(xoico_item));
        const bcore_self_item_s* self_item = bcore_self_s_get_first_anonymous_array_item( stamp->self );
        if( self_item )
        {
            info->type_info.typespec.type = self_item->type;
            info->type_info.typespec.indirection = bcore_flect_caps_get_indirection( self_item->caps );
            success = true;
        }
    }
    
    return  success;
}

er_t xoico_compiler_s_update_target_files( xoico_compiler_s* o, bl_t* p_modified )
{
    // xoico_compiler.x:761:1
    
    bl_t modified = false;
    bl_t verbosity = o->verbosity;
    f3_t time = 0;
    
    er_t er = 0;
    ABS_TIME_OF( er = xoico_compiler_s_expand(o,(&(modified)) ), time );
    BLM_TRY(er )
    
    if( modified )
    {
        if( verbosity > 0 ) bcore_msg_fa( "XOICO: Expanded in #<f3_t> sec.\n", time );
        if( verbosity > 0 ) bcore_msg_fa( "XOICO: Files were updated. Rebuild is necessary.\n" );
    }
    
    if( p_modified ) (*(p_modified)) = modified;
    
    return  er;
}

XOILA_DEFINE_SPECT( xoico, xoico_compiler )
"{"
    "bcore_spect_header_s header;"
"}";

bl_t xoico_compiler_is_correctly_signed( const st_s* data )
{
    // xoico_compiler.x:377:1
    
    if( data->size < bcore_strlen( "// XOICO_FILE_SIGNATURE" ) ) return  false;
    
    sz_t idx = data->size - 1;
    while( idx >= 0 && data->data[ idx ] != '/' ) idx--;
    if( idx > 0 ) idx--;
    
    if( st_s_find_sc(data,idx, -1, "// XOICO_FILE_SIGNATURE" ) != idx ) return  false;
    
    tp_t hash = 0;
    sz_t end_idx = st_s_parse_fa(data,idx, -1, "// XOICO_FILE_SIGNATURE #<tp_t*> ", (&(hash)) );
    
    if( end_idx != data->size ) return  false;
    
    data->data[ idx ] = 0;
    
    if( hash != bcore_tp_fold_sc( bcore_tp_init(), data->sc ) ) return  false;
    
    return  true;
}

tp_t xoico_compiler_body_signature( const st_s* data )
{
    // xoico_compiler.x:402:1
    
    // search from bottom upwards ...
    sz_t idx = 0;
    if( ( idx = st_s_find_sc(data,data->size, 0, "// XOICO_BODY_SIGNATURE" ) ) == data->size ) return  0;
    
    tp_t hash = 0;
    st_s_parse_fa(data,idx, -1, "// XOICO_BODY_SIGNATURE #<tp_t*>", (&(hash)) );
    
    return  hash;
}

/**********************************************************************************************************************/
// source: xoico_cengine.x

//----------------------------------------------------------------------------------------------------------------------
// group: xoico_cengine

XOILA_DEFINE_SPECT( xoico, xoico_cengine )
"{"
    "bcore_spect_header_s header;"
    "feature aware xoico_cengine : translate;"
    "feature aware xoico_cengine : is_reserved = xoico_cengine_is_reserved_default;"
"}";

/**********************************************************************************************************************/
// source: xoico_che.x

//----------------------------------------------------------------------------------------------------------------------
// group: xoico_che; embeds: xoico_che_trans_typespec.x xoico_che_function.x xoico_che_builtin.x xoico_che_control.x

BCORE_DEFINE_OBJECT_INST_P( xoico_che_s )
"aware xoico_che"
"{"
    "bl_t verbose = false;"
    "bl_t insert_source_reference = true;"
    "bl_t for_all_functions_enable_try = true;"
    "bl_t waive_unknown_member_variable = false;"
    "bl_t waive_unknown_member_function = false;"
    "bl_t waive_function_in_untraced_context = false;"
    "bl_t waive_unknown_type = false;"
    "bl_t waive_unknown_identifier = false;"
    "bl_t waive_local_scope_operator_creates_implicit_block = false;"
    "hidden aware xoico_host* host;"
    "hidden aware xoico_compiler_s* compiler;"
    "xoico_signature_s => signature;"
    "hidden sz_t block_level;"
    "hidden sz_t try_block_level;"
    "hidden xoico_che_stack_var_s stack_var;"
    "hidden xoico_che_stack_block_s stack_block;"
    "hidden bcore_hmap_name_s hmap_name;"
    "hidden bl_t has_completion;"
    "hidden bl_t has_verbatim_code;"
    "func xoico:get_hash;"
    "func xoico_cengine:is_reserved;"
    "func xoico_cengine:translate;"
"}";

tp_t xoico_che_s_get_hash( const xoico_che_s* o )
{
    // xoico_che.x:261:5
    
    tp_t hash = bcore_tp_init();
    
    hash = bcore_tp_fold_bl( hash, o->verbose );
    hash = bcore_tp_fold_bl( hash, o->insert_source_reference );
    hash = bcore_tp_fold_bl( hash, o->for_all_functions_enable_try );
    
    hash = bcore_tp_fold_bl( hash, o->waive_unknown_member_variable );
    hash = bcore_tp_fold_bl( hash, o->waive_unknown_member_function );
    hash = bcore_tp_fold_bl( hash, o->waive_function_in_untraced_context );
    hash = bcore_tp_fold_bl( hash, o->waive_unknown_type );
    hash = bcore_tp_fold_bl( hash, o->waive_unknown_identifier );
    hash = bcore_tp_fold_bl( hash, o->waive_local_scope_operator_creates_implicit_block );
    
    return  hash;
}

bl_t xoico_che_s_is_reserved( const xoico_che_s* o, tp_t tp_identifier )
{
    // xoico_che.x:281:5
    
    return  xoico_che_s_is_builtin_func(o,tp_identifier ) ||
           xoico_che_s_is_control_name(o,tp_identifier ) ||
           tp_identifier == TYPEOF_verbatim_C;
}

sc_t xoico_che_s_nameof( xoico_che_s* o, tp_t type )
{
    // xoico_che.x:290:5
    
    sc_t name = bcore_hmap_name_s_get_sc(&(o->hmap_name),type );
    if( !name ) name = xoico_compiler_s_nameof(o->compiler,type );
    return  name;
}

void xoico_che_s_init_level0( xoico_che_s* o )
{
    // xoico_che.x:297:5
    
    xoico_che_stack_block_s_clear(&(o->stack_block));
    xoico_che_stack_block_s_push(&(o->stack_block));
    o->block_level = 0;
}

void xoico_che_s_inc_block( xoico_che_s* o )
{
    // xoico_che.x:304:5
    
    xoico_che_stack_block_s_push(&(o->stack_block));
    o->block_level++;
    xoico_che_s_stack_block_get_top_unit(o)->level = o->block_level;
}

void xoico_che_s_dec_block( xoico_che_s* o )
{
    // xoico_che.x:311:5
    
    xoico_che_stack_var_s_pop_level(&(o->stack_var),o->block_level );
    o->block_level--;
    ASSERT( o->block_level >= 0 );
    xoico_che_stack_block_s_pop(&(o->stack_block));
}

xoico_che_stack_block_unit_s* xoico_che_s_stack_block_get_top_unit( xoico_che_s* o )
{
    // xoico_che.x:319:5
    
    return  o->stack_block.adl.data[ o->stack_block.adl.size - 1 ];
}

xoico_che_stack_block_unit_s* xoico_che_s_stack_block_get_bottom_unit( xoico_che_s* o )
{
    // xoico_che.x:324:5
    
    return  o->stack_block.adl.data[ 0 ];
}

xoico_che_stack_block_unit_s* xoico_che_s_stack_block_get_level_unit( xoico_che_s* o, sz_t level )
{
    // xoico_che.x:329:5
    
    {const xoico_che_stack_block_unit_adl_s* __a=&(o->stack_block.adl );if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_che_stack_block_unit_s* e=__a->data[__i]; if( e->level == level ) return  e;
    }}ERR_fa( "Level #<sz_t> not found.", level );
    return  NULL;
}

void xoico_che_s_push_typedecl( xoico_che_s* o, const xoico_typespec_s* typespec, tp_t name )
{
    // xoico_che.x:336:5
    BLM_INIT_LEVEL(0);
    xoico_che_stack_var_unit_s* unit = ((xoico_che_stack_var_unit_s*)BLM_LEVEL_T_PUSH(0,xoico_che_stack_var_unit_s,xoico_che_stack_var_unit_s_create()));
    unit->level = o->block_level;
    unit->name = name;
    xoico_typespec_s_copy(&(unit->typespec),typespec );
    xoico_che_stack_var_s_push_unit(&(o->stack_var),unit );
    BLM_DOWN();
}

void xoico_che_s_typespec_to_sink( xoico_che_s* o, const xoico_typespec_s* typespec, x_sink* sink )
{
    // xoico_che.x:346:5
    BLM_INIT_LEVEL(0);
    xoico_che_result_arr_s* result = ((xoico_che_result_arr_s*)BLM_LEVEL_T_PUSH(0,xoico_che_result_arr_s,xoico_che_result_arr_s_create()));
    BLM_TRY_EXIT(xoico_che_s_push_typespec(o,typespec,((xoico_che_result*)( result ))))
    BLM_TRY_EXIT(xoico_che_result_arr_s_to_sink(result,sink ))
    BLM_DOWN();
}

er_t xoico_che_s_trans( const xoico_che_s* o, x_source* source, sc_t format, xoico_che_result* result )
{
    // xoico_che.x:368:5
    
    BLM_TRY(x_source_parse_fa(source,format ))
    xoico_che_result_a_push_sc(result,format );
    return  0;
}

bl_t xoico_che_s_returns_er_t( const xoico_che_s* o )
{
    // xoico_che.x:376:5
    
    return  o->signature->typespec_ret.type == ((tp_t)(TYPEOF_er_t)) && o->signature->typespec_ret.indirection == 0;
}

tp_t xoico_che_s_get_identifier( xoico_che_s* o, x_source* source, bl_t take_from_source )
{
    // xoico_che.x:393:1
    BLM_INIT_LEVEL(0);
    tp_t tp_identifier = 0;
    if( x_source_parse_bl(source,"#?(([0]>='A'&&[0]<='Z')||([0]>='a'&&[0]<='z')||[0]=='_'||[0]=='@'||[0]=='$'||([0]==':'&&([1]!=' '&&[1]!='\t'&&[1]!='\n'&&[1]!='/')))" ) )
    {
        sz_t source_index = take_from_source ? 0 : x_source_get_index(source);
        switch( x_source_inspect_char(source) )
        {
            case '@':
            {
                x_source_get_char(source);
                st_s* st_name = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
                st_s_copy_sc(st_name,xoico_che_s_nameof(o,xoico_host_a_obj_type(o->host) ) );
                if( x_source_parse_bl(source,"#?(([0]>='A'&&[0]<='Z')||([0]>='a'&&[0]<='z')||[0]=='_'||([0]>='0'&&[0]<='9'))" ) )
                {
                    BLM_TRY_EXIT(x_source_parse_fa(source,"#:name", st_name ))
                }
                tp_identifier = xoico_che_s_entypeof(o,st_name->sc );
            }
            break;
    
            case '$':
            {
                x_source_get_char(source);
                tp_identifier = ((tp_t)(TYPEOF_type_deduce));
            }
            break;
    
            case ':':
            {
                BLM_TRY_EXIT(xoico_host_a_parse_name_tp(o->host,source, (&(tp_identifier)) ))
            }
            break;
    
            default:
            {BLM_INIT_LEVEL(4);
                st_s* st_name = ((st_s*)BLM_LEVEL_T_PUSH(4,st_s,st_s_create()));
                BLM_TRY_EXIT(x_source_parse_fa(source,"#name", st_name ))
                tp_identifier = xoico_che_s_entypeof(o,st_name->sc );
            BLM_DOWN();}
            break;
        }
    
        if( !take_from_source ) x_source_set_index(source,source_index );
    }
    
    BLM_RETURNV(tp_t, tp_identifier)
}

er_t xoico_che_s_trans_identifier( xoico_che_s* o, x_source* source, xoico_che_result* result, tp_t* tp_identifier )
{
    // xoico_che.x:451:1
    
    tp_t identifier = xoico_che_s_get_identifier(o,source, true );
    if( !identifier )
    {
        return  x_source_parse_error_fa(source,"Identifier expected" );
    }
    if( tp_identifier ) (*(tp_identifier)) = identifier;
    if( result ) BLM_TRY(xoico_che_result_a_push_sc(result,xoico_che_s_nameof(o,identifier ) ))
    return  0;
}

er_t xoico_che_s_trans_number_literal( xoico_che_s* o, x_source* source, xoico_che_result* result )
{
    // xoico_che.x:466:1
    
    bl_t hex = false;
    if( x_source_parse_bl(source,"#?'0x'" ) )
    {
        BLM_TRY(xoico_che_result_a_push_sc(result,"0x" ))
        hex = true;
    }
    else if( x_source_parse_bl(source,"#?'0X'" ) )
    {
        BLM_TRY(xoico_che_result_a_push_sc(result,"0X" ))
        hex = true;
    }
    
    if( hex )
    {
        while( x_source_parse_bl(source,"#?(([0]>='0'&&[0]<='9')||([0]>='a'&&[0]<='f')||([0]>='A'&&[0]<='F'))" ) )
        {
            BLM_TRY(xoico_che_result_a_push_char(result,x_source_get_char(source) ))
        }
    }
    else // decimal
    {
        while( x_source_parse_bl(source,"#?([0]>='0'&&[0]<='9')" ) ) BLM_TRY(xoico_che_result_a_push_char(result,x_source_get_char(source) ))
    }
    
    if( x_source_parse_bl(source,"#?([0]=='.')" ) )
    {
        BLM_TRY(xoico_che_result_a_push_char(result,x_source_get_char(source) ))
        while( x_source_parse_bl(source,"#?([0]>='0'&&[0]<='9')" ) ) BLM_TRY(xoico_che_result_a_push_char(result,x_source_get_char(source) ))
    }
    
    bl_t exponent = false;
    
    if( x_source_parse_bl(source,"#?([0]=='e'||[0]=='E')" ) )
    {
        BLM_TRY(xoico_che_result_a_push_char(result,x_source_get_char(source) ))
        exponent = true;
    }
    
    if( exponent )
    {
        if( x_source_parse_bl(source,"#?([0]=='+'||[0]=='-')" ) )
        {
            BLM_TRY(xoico_che_result_a_push_char(result,x_source_get_char(source) ))
        }
    
        while( x_source_parse_bl(source,"#?([0]>='0'&&[0]<='9')" ) )
        {
            BLM_TRY(xoico_che_result_a_push_char(result,x_source_get_char(source) ))
        }
    }
    
    // suffix
    while( x_source_parse_bl(source,"#?([0]=='l'||[0]=='L'||[0]=='u'||[0]=='U'||[0]=='f'||[0]=='F')" ) )
    {
        BLM_TRY(xoico_che_result_a_push_char(result,x_source_get_char(source) ))
    }
    
    return  0;
}

er_t xoico_che_s_trans_string_literal( xoico_che_s* o, x_source* source, xoico_che_result* result )
{
    // xoico_che.x:531:1
    
    BLM_TRY(xoico_che_s_trans(o,source, "\"", result ))
    
    while( !x_source_eos(source) && !x_source_parse_bl(source,"#=?'\"'" ) )
    {
        if( x_source_parse_bl(source,"#?'\\'" ) ) BLM_TRY(xoico_che_result_a_push_char(result,'\\' ))
        BLM_TRY(xoico_che_result_a_push_char(result,x_source_get_char(source) ))
    }
    
    BLM_TRY(xoico_che_s_trans(o,source, "\"", result ))
    return  0;
}

er_t xoico_che_s_trans_char_literal( xoico_che_s* o, x_source* source, xoico_che_result* result )
{
    // xoico_che.x:548:1
    
    BLM_TRY(xoico_che_s_trans(o,source, "'", result ))
    
    while( !x_source_eos(source) && !x_source_parse_bl(source,"#=?|'|" ) )
    {
        if( x_source_parse_bl(source,"#?'\\'" ) ) BLM_TRY(xoico_che_result_a_push_char(result,'\\' ))
        BLM_TRY(xoico_che_result_a_push_char(result,x_source_get_char(source) ))
    }
    
    BLM_TRY(xoico_che_s_trans(o,source, "'", result ))
    return  0;
}

er_t xoico_che_s_trans_whitespace( xoico_che_s* o, x_source* source, xoico_che_result* result )
{
    // xoico_che.x:565:1
    BLM_INIT_LEVEL(0);
    bl_t exit_loop = false;
    
    st_s* st = result ? ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create())) : NULL;
    
    while( !exit_loop && !x_source_eos(source) )
    {
        char c =  x_source_inspect_char(source);
        switch( c )
        {
            case ' ' :
            case '\t':
            case '\n':
            {
                char c =  x_source_get_char(source);
                if( st ) st_s_push_char(st,c );
            }
            break;
    
            case '/':
            {
                if( x_source_parse_bl(source,"#?'//'" ) )
                {
                    if( st ) st_s_push_sc(st,"//" );
                    while( !x_source_eos(source) )
                    {
                        char c =  x_source_get_char(source);
                        if( st ) st_s_push_char(st,c );
                        if( c == '\n' ) break;
                    }
                }
                else if( x_source_parse_bl(source,"#?'/*'" ) )
                {
                    if( st ) st_s_push_sc(st,"/*" );
                    while( !x_source_eos(source) )
                    {
                        if( x_source_parse_bl(source,"#?'*/'" ) )
                        {
                            if( st ) st_s_push_sc(st,"*/" );
                            break;
                        }
                        char c =  x_source_get_char(source);
                        if( st ) st_s_push_char(st,c );
                    }
                }
                else
                {
                    exit_loop = true;
                }
            }
            break;
    
            default:
            {
                exit_loop = true;
            }
            break;
        }
    }
    
    if( result && st->size > 0 ) xoico_che_result_a_push_result_d(result,((xoico_che_result*)(xoico_che_result_whitespace_s__(xoico_che_result_whitespace_s_create(),((st_s*)bcore_fork(st)) ) )));
    BLM_RETURNV(er_t, 0)
}

er_t xoico_che_s_trans_preprocessor( xoico_che_s* o, x_source* source, xoico_che_result* result )
{
    // xoico_che.x:632:1
    
    BLM_TRY(x_source_parse_fa(source,"##" ))
    BLM_TRY(xoico_che_result_a_push_sc(result,"#" ))
    while( !x_source_eos(source) )
    {
        char c = x_source_get_char(source);
        BLM_TRY(xoico_che_result_a_push_char(result,c ))
        if( c == '\\' && x_source_inspect_char(source) == '\n' ) BLM_TRY(xoico_che_result_a_push_char(result,x_source_get_char(source) ))
        if( c == '\n' ) break;
    }
    return  0;
}

tp_t xoico_che_s_trans_inert_operator( xoico_che_s* o, x_source* source, xoico_che_result* result )
{
    // xoico_che.x:649:1
    
    switch( x_source_inspect_char(source) )
    {
        case '+':
        case '-':
        case '*':
        case '/':
        case '=':
        case '>':
        case '<':
        case '!':
        case '|':
        case '&':
        case '^':
        case '~':
        case '%':
        {
            BLM_TRY_EXIT(xoico_che_result_a_push_char(result,x_source_get_char(source) ))
            return  1;
        }
        break;
    
        default: break;
    }
    
    return  false;
}

bl_t xoico_che_s_trans_operator( xoico_che_s* o, x_source* source, xoico_che_result* result )
{
    // xoico_che.x:680:1
    
    switch( x_source_inspect_char(source) )
    {
        case '+':
        {
            BLM_TRY_EXIT(xoico_che_result_a_push_char(result,x_source_get_char(source) ))
            char c = x_source_inspect_char(source);
            if( c == '=' || c == '+' ) BLM_TRY_EXIT(xoico_che_result_a_push_char(result,x_source_get_char(source) ))
        }
        return  true;
    
        case '-':
        {
            BLM_TRY_EXIT(xoico_che_result_a_push_char(result,x_source_get_char(source) ))
            char c = x_source_inspect_char(source);
            if( c == '=' || c == '-' ) BLM_TRY_EXIT(xoico_che_result_a_push_char(result,x_source_get_char(source) ))
        }
        return  true;
    
        case '*':
        case '/':
        case '=':
        case '!':
        case '^':
        case '%':
        {
            BLM_TRY_EXIT(xoico_che_result_a_push_char(result,x_source_get_char(source) ))
            if( x_source_inspect_char(source) == '=' ) BLM_TRY_EXIT(xoico_che_result_a_push_char(result,x_source_get_char(source) ))
        }
        return  true;
    
        case '>':
        {
            BLM_TRY_EXIT(xoico_che_result_a_push_char(result,x_source_get_char(source) ))
            char c = x_source_inspect_char(source);
            if( c == '=' || c == '>' ) BLM_TRY_EXIT(xoico_che_result_a_push_char(result,x_source_get_char(source) ))
            if( c == '>' && x_source_inspect_char(source) == '=' ) BLM_TRY_EXIT(xoico_che_result_a_push_char(result,x_source_get_char(source) ))
        }
        return  true;
    
        case '<':
        {
            BLM_TRY_EXIT(xoico_che_result_a_push_char(result,x_source_get_char(source) ))
            char c = x_source_inspect_char(source);
            if( c == '=' || c == '<' ) BLM_TRY_EXIT(xoico_che_result_a_push_char(result,x_source_get_char(source) ))
            if( c == '<' && x_source_inspect_char(source) == '=' ) BLM_TRY_EXIT(xoico_che_result_a_push_char(result,x_source_get_char(source) ))
        }
        return  true;
    
        case '|':
        {
            BLM_TRY_EXIT(xoico_che_result_a_push_char(result,x_source_get_char(source) ))
            char c = x_source_inspect_char(source);
            if( c == '=' || c == '|' ) BLM_TRY_EXIT(xoico_che_result_a_push_char(result,x_source_get_char(source) ))
        }
        return  true;
    
        case '&':
        {
            BLM_TRY_EXIT(xoico_che_result_a_push_char(result,x_source_get_char(source) ))
            char c = x_source_inspect_char(source);
            if( c == '=' || c == '&' ) BLM_TRY_EXIT(xoico_che_result_a_push_char(result,x_source_get_char(source) ))
        }
        return  true;
    
        case '~':
        {
            BLM_TRY_EXIT(xoico_che_result_a_push_char(result,x_source_get_char(source) ))
        }
        return  true;
    
        default: break;
    }
    
    return  false;
}

er_t xoico_che_s_adapt_expression_indirection( xoico_che_s* o, x_source* source, const xoico_typespec_s* typespec_expr, sz_t target_indirection, const xoico_che_result* result_expr, xoico_che_result* result )
{
    // xoico_che.x:768:1
    
    if( target_indirection == typespec_expr->indirection )
    {
        xoico_che_result_a_push_result_c(result,result_expr );
    }
    else if( target_indirection == typespec_expr->indirection + 1 )
    {
        if( typespec_expr->flag_addressable )
        {
            BLM_TRY(xoico_che_result_a_push_sc(result,"&(" ))
            xoico_che_result_a_push_result_c(result,result_expr );
            BLM_TRY(xoico_che_result_a_push_sc(result,")" ))
        }
        else
        {
            return  x_source_parse_error_fa(source,"Increasing indirection: Expression has no address." );
        }
    }
    else if( target_indirection < typespec_expr->indirection )
    {
        for(sz_t ind = typespec_expr->indirection; ind > target_indirection; ind-- ) BLM_TRY(xoico_che_result_a_push_sc(result,"*" ))
        BLM_TRY(xoico_che_result_a_push_sc(result,"(" ))
        xoico_che_result_a_push_result_c(result,result_expr );
        BLM_TRY(xoico_che_result_a_push_sc(result,")" ))
    }
    else
    {
        return  x_source_parse_error_fa(source,"Cannot adapt from expression's indirection '#<sz_t>' to target indirection '#<sz_t>'.",
            typespec_expr->indirection,
            target_indirection
        );
    }
    return  0;
}

er_t xoico_che_s_adapt_expression( xoico_che_s* o, x_source* source, const xoico_typespec_s* typespec_expr, const xoico_typespec_s* typespec_target, const xoico_che_result* result_expr, xoico_che_result* result )
{
    // xoico_che.x:816:1
    BLM_INIT_LEVEL(0);
    if( !typespec_expr->type )
    {
        BLM_RETURNV(er_t, x_source_parse_error_fa(source,"xoico_che_s_adapt_expression: no expression type specified.",
            typespec_expr->indirection,
            typespec_target->indirection
        ))
    }
    
    if( !typespec_target->type )
    {
        BLM_RETURNV(er_t, x_source_parse_error_fa(source,"xoico_che_s_adapt_expression: no target type specified.",
            typespec_expr->indirection,
            typespec_target->indirection
        ))
    }
    
    if( typespec_target->type == ((tp_t)(TYPEOF_type_deduce)) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"adapt_expression: typespec_target is 'type_deduce'" ))
    if( typespec_target->type == ((tp_t)(TYPEOF_type_object)) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"adapt_expression: typespec_target is 'type_object'" ))
    if( typespec_expr->type == ((tp_t)(TYPEOF_type_deduce)) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"adapt_expression: typespec_expr is 'type_deduce'" ))
    if( typespec_expr->type == ((tp_t)(TYPEOF_type_object)) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"adapt_expression: typespec_expr is 'type_object'" ))
    
    bl_t discarding_const =
        ( typespec_expr->access_class == TYPEOF_const && typespec_expr->access_class != typespec_target->access_class ) &&
        ( typespec_expr->indirection > 0 && typespec_target->indirection > 0 );
    
    if( discarding_const ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Discarding 'const' qualifier." ))
    
    if( typespec_target->access_class == TYPEOF_discardable && typespec_expr->access_class != TYPEOF_discardable )
    {
        BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Discardable expression expected." ))
    }
    
    if( xoico_typespec_s_converts_to(typespec_expr,typespec_target ) )
    {
        xoico_che_result_a_push_result_c(result,result_expr );
    }
    else
    {BLM_INIT_LEVEL(1);
        bl_t implicit_cast = false;
        st_s* fail_msg = NULL;
        if( typespec_target->type != typespec_expr->type )
        {
            if( xoico_che_s_is_group(o,typespec_target->type ) && typespec_target->indirection == 1 )
            {
                if( xoico_che_s_is_group(o,typespec_expr->type ) )
                {
                    implicit_cast = true;
                }
                else if( xoico_che_s_is_stamp(o,typespec_expr->type ) )
                {
                    const xoico_stamp_s* stamp = xoico_che_s_get_stamp(o,typespec_expr->type );
                    if( stamp->is_aware || typespec_target->flag_obliv )
                    {
                        implicit_cast = true;
                    }
                    else
                    {
                        fail_msg = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create_fa("'#<sc_t>' is obliv but the target typespec does not explicitly tolerate obliv objects.", xoico_che_s_nameof(o,typespec_expr->type ) )));
                    }
                }
                else if( xoico_che_s_is_type(o,typespec_expr->type ) )
                {
                    if( typespec_target->flag_obliv )
                    {
                        implicit_cast = true;
                    }
                    else
                    {
                        fail_msg = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create_fa("'#<sc_t>' is presumed obliv but the target typespec does not explicitly tolerate obliv objects.", xoico_che_s_nameof(o,typespec_expr->type ) )));
                    }
                }
                else
                {
                    fail_msg = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create_fa("Source type cannot be classified." )));
                }
            }
            else
            {
                fail_msg = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create_fa("Target is not a group or target indirection is != 1." )));
            }
        }
    
        if( fail_msg )
        {
            st_s* st_typespec_expr = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
            st_s* st_typespec_target = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
            xoico_che_s_typespec_to_sink(o,typespec_expr,((x_sink*)( st_typespec_expr )));
            xoico_che_s_typespec_to_sink(o,typespec_target,((x_sink*)( st_typespec_target )));
            st_s* msg = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
            st_s_push_fa(msg,"Implicit cast from '#<sc_t>' to '#<sc_t>' is not allowed. ", st_typespec_expr->sc, st_typespec_target->sc );
            st_s_push_fa(msg,"Reason: #<st_s*>\n", fail_msg );
            BLM_RETURNV(er_t, x_source_parse_error_fa(source,"#<st_s*>", msg ))
        }
    
        xoico_che_result* result_expression =((xoico_che_result*)( ((xoico_che_result_arr_s*)BLM_LEVEL_T_PUSH(1,xoico_che_result_arr_s,xoico_che_result_arr_s_create()))));
        BLM_TRY(xoico_che_s_adapt_expression_indirection(o,source, typespec_expr, typespec_target->indirection, result_expr, result_expression ))
    
        if( implicit_cast )
        {
            xoico_che_result_a_push_result_d(result,((xoico_che_result*)(xoico_che_result_cast_s_reduce(xoico_che_result_cast_s__(xoico_che_result_cast_s_create(),o, xoico_typespec_s_clone(typespec_target), ((xoico_che_result*)bcore_fork(result_expression)) )) )));
        }
        else
        {
            xoico_che_result_a_push_result_d(result,((xoico_che_result*)bcore_fork(result_expression)) );
        }
    
    BLM_DOWN();}
    BLM_RETURNV(er_t, 0)
}

er_t xoico_che_s_trans_member( xoico_che_s* o, x_source* source, xoico_che_result* result )
{
    // xoico_che.x:934:1
    
    if(      x_source_parse_bl(source,"#?'.'"  ) ) BLM_TRY(xoico_che_result_a_push_sc(result,"." ))
    else if( x_source_parse_bl(source,"#?'->'" ) ) BLM_TRY(xoico_che_result_a_push_sc(result,"->" ))
    
    BLM_TRY(xoico_che_s_trans_whitespace(o,source, result ))
    
    char c = x_source_inspect_char(source);
    
    if( c == '*' || c == '&' )
    {
        return  x_source_parse_error_fa(source,"Postfix operators '&' and '*' are deprecated.\n" );
    }
    
    if( c >= '0' && c <= '9' )
    {
        return  x_source_parse_error_fa(source,"Setting indirection: Expression not tractable\n" );
    }
    
    tp_t tp_identifier = xoico_che_s_get_identifier(o,source, false );
    
    if( xoico_che_s_is_builtin_func(o,tp_identifier ) )
    {
        return  x_source_parse_error_fa(source,"Expression before '#<sc_t>' not tractable\n", xoico_che_s_nameof(o,tp_identifier ) );
    }
    
    BLM_TRY(xoico_che_s_trans_identifier(o,source, result, NULL ))
    BLM_TRY(xoico_che_s_trans_whitespace(o,source, result ))
    if( x_source_parse_bl(source,"#?'('") )
    {
        return  x_source_parse_error_fa(source,"Untraced member function '#<sc_t>'\n", xoico_che_s_nameof(o,tp_identifier ) );
    }
    
    return  0;
}

er_t xoico_che_s_try_take_typespec( xoico_che_s* o, x_source* source, xoico_typespec_s* typespec, bl_t require_tractable_type, bl_t* success )
{
    // xoico_che.x:982:1
    
    if( success ) (*(success)) = false;
    
    sz_t index = x_source_get_index(source);
    
    BLM_TRY(xoico_che_s_trans_whitespace(o,source, NULL ))
    tp_t tp_identifier = xoico_che_s_get_identifier(o,source, true );
    
    if( !tp_identifier )
    {
        x_source_set_index(source,index );
        return  0;
    }
    
    BLM_TRY(xoico_che_s_trans_whitespace(o,source, NULL ))
    
    // take fails if identifier is used as function
    if( x_source_parse_bl(source,"#?'('" ) )
    {
        x_source_set_index(source,index );
        return  0;
    }
    
    tp_t access_class = 0;
    
    switch( tp_identifier )
    {
        case TYPEOF_c:
        case TYPEOF_const:
            access_class = TYPEOF_const;
            break;
    
        case TYPEOF_m:
        case TYPEOF_mutable:
            access_class = TYPEOF_mutable;
            break;
    
        case TYPEOF_d:
        case TYPEOF_discardable:
            access_class = TYPEOF_discardable;
            break;
    
        default: break;
    }
    
    if( access_class )
    {
        tp_identifier = xoico_che_s_get_identifier(o,source, true );
        if( !tp_identifier )
        {
            x_source_set_index(source,index );
            return  0;
        }
        BLM_TRY(xoico_che_s_trans_whitespace(o,source, NULL ))
    }
    
    while( tp_identifier )
    {
        if     ( tp_identifier == TYPEOF_static   && !typespec->flag_static   ) typespec->flag_static   = true;
        else if( tp_identifier == TYPEOF_volatile && !typespec->flag_volatile ) typespec->flag_volatile = true;
        else if( tp_identifier == TYPEOF_aware    && !typespec->flag_aware    ) typespec->flag_aware    = true;
        else if( tp_identifier == TYPEOF_obliv    && !typespec->flag_obliv    ) typespec->flag_obliv    = true;
        else break;
    
        tp_identifier = 0;
        BLM_TRY(xoico_che_s_trans_identifier(o,source, NULL,&( tp_identifier )))
        BLM_TRY(xoico_che_s_trans_whitespace(o,source, NULL ))
    }
    
    typespec->type = tp_identifier;
    
    if( x_source_parse_bl(source,"#?'.' " ) )
    {
        if( !x_source_parse_bl(source,"#?([0]>='0'&&[0]<='9') " ) )
        {
            x_source_set_index(source,index );
            return  0;
        }
    
        sz_t indirection = 0;
        BLM_TRY(x_source_parse_fa(source,"#<sz_t*> ", (&(indirection)) ))
        typespec->indirection = indirection;
    }
    else
    {
        while( x_source_parse_bl(source,"#?'*' " ) ) typespec->indirection++;
    }
    
    if( require_tractable_type )
    {
        if( !( tp_identifier == ((tp_t)(TYPEOF_type_deduce)) || xoico_che_s_is_type(o,tp_identifier ) ) )
        {
            x_source_set_index(source,index );
            return  0;
        }
    }
    
    if( x_source_parse_bl(source," #?'^'") ) typespec->flag_scope = true;
    
    if( x_source_parse_bl(source," #?w'restrict'") ) typespec->flag_restrict = true;
    
    if( typespec->indirection > 0 && access_class == 0 )
    {
        access_class = TYPEOF_const; // const is default
    }
    
    typespec->access_class = access_class;
    
    if( success ) (*(success)) = true;
    return  0;
}

er_t xoico_che_s_take_typespec( xoico_che_s* o, x_source* source, xoico_typespec_s* typespec, bl_t require_tractable_type )
{
    // xoico_che.x:1103:1
    
    bl_t success = false;
    BLM_TRY(xoico_che_s_try_take_typespec(o,source, typespec, require_tractable_type,&( success )))
    
    if( !success )
    {
        if( require_tractable_type )
        {
            return  x_source_parse_error_fa(source,"Typespec: Syntax error or intractable type." );
        }
        else
        {
            return  x_source_parse_error_fa(source,"Typespec: Syntax error." );
        }
    }
    
    return  0;
}

er_t xoico_che_s_push_typespec( xoico_che_s* o, const xoico_typespec_s* typespec, xoico_che_result* result )
{
    // xoico_che.x:1125:1
    BLM_INIT_LEVEL(0);
    tp_t type = typespec->type;
    
    if( type == ((tp_t)(TYPEOF_type_object)) )
    {
        ERR_fa( "Cannot resolve 'type_object' at this point." );
    }
    else if( type == ((tp_t)(TYPEOF_type_deduce)) )
    {
        ERR_fa( "Cannot resolve 'type_deduce' at this point." );
    }
    else if( type == 0 )
    {
        ERR_fa( "Type is 0." );
    }
    
    st_s* st_type = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create_sc(xoico_che_s_nameof(o,type ) )));
    
    if( st_type->size == 0 )
    {
        ERR_fa( "Type '#<tp_t>' has no name.", type );
    }
    
    sc_t sc_type = st_type->sc;
    if( typespec->flag_static   ) BLM_TRY(xoico_che_result_push_fa(result,"static " ))
    if( typespec->access_class == TYPEOF_const ) BLM_TRY(xoico_che_result_push_fa(result,"const " ))
    if( typespec->flag_volatile ) BLM_TRY(xoico_che_result_push_fa(result,"volatile " ))
    BLM_TRY(xoico_che_result_push_fa(result,"#<sc_t>", sc_type ))
    
    for(sz_t i = 0; i < typespec->indirection; i++ ) BLM_TRY(xoico_che_result_push_fa(result,"*" ))
    if( typespec->flag_restrict ) BLM_TRY(xoico_che_result_push_fa(result,"restrict " ))
    
    BLM_RETURNV(er_t, 0)
}

er_t xoico_che_s_trans_type( xoico_che_s* o, x_source* source, xoico_che_result* result, xoico_typespec_s* out_typespec )
{
    // xoico_che.x:1169:1
    BLM_INIT_LEVEL(0);
    xoico_che_result_arr_s* result_local = ((xoico_che_result_arr_s*)BLM_LEVEL_T_PUSH(0,xoico_che_result_arr_s,xoico_che_result_arr_s_create()));
    tp_t tp_identifier;
    BLM_TRY(xoico_che_s_trans_identifier(o,source,((xoico_che_result*)( result_local)),&( tp_identifier )))
    BLM_TRY(xoico_che_s_trans_whitespace(o,source,((xoico_che_result*)( result_local ))))
    if( x_source_parse_bl(source,"#?'!'" ) )
    {
        if( xoico_che_s_is_group(o,tp_identifier ) )
        {
            BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Operator '!': lvalue is a group." ))
        }
        xoico_che_result_arr_s_clear(result_local);
        BLM_TRY(xoico_che_result_push_fa(((xoico_che_result*)(result_local)),"#<sc_t>_create()", xoico_che_s_nameof(o,tp_identifier ) ))
    
        xoico_typespec_s* typespec = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_create()));
        typespec->type = tp_identifier;
        typespec->indirection = 1;
        typespec->flag_addressable = false;
        typespec->access_class = TYPEOF_discardable;
    
        if( x_source_parse_bl(source,"#=?'^'" ) )
        {
            BLM_TRY(xoico_che_s_trans_builtin_scope(o,source,((const xoico_che_result*)( result_local)), typespec, result, NULL ))
            typespec->access_class = TYPEOF_mutable;
        }
        else
        {
            xoico_che_result_a_push_result_d(result,((xoico_che_result*)(((xoico_che_result_arr_s*)bcore_fork(result_local)) )));
        }
    
        BLM_TRY(xoico_che_s_trans_typespec_expression(o,source, result, typespec, out_typespec ))
    }
    else if( x_source_parse_bl(source,"#?'~'" ) )
    {
        xoico_typespec_s* typespec = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_create()));
        typespec->type = ((tp_t)(TYPEOF_tp_t));
        typespec->indirection = 0;
        typespec->flag_addressable = false;
        typespec->access_class = 0;
        BLM_TRY(xoico_che_result_push_fa(result,"((tp_t)(TYPEOF_#<sc_t>))", xoico_che_s_nameof(o,tp_identifier ) ))
        BLM_TRY(xoico_che_s_trans_typespec_expression(o,source, result, typespec, out_typespec ))
    }
    else
    {
        xoico_che_result_a_push_result_d(result,((xoico_che_result*)(((xoico_che_result_arr_s*)bcore_fork(result_local)) )));
    }
    BLM_RETURNV(er_t, 0)
}

er_t xoico_che_s_trans_name( xoico_che_s* o, x_source* source, xoico_che_result* result, xoico_typespec_s* out_typespec )
{
    // xoico_che.x:1227:1
    BLM_INIT_LEVEL(0);
    xoico_che_result_arr_s* result_local = ((xoico_che_result_arr_s*)BLM_LEVEL_T_PUSH(0,xoico_che_result_arr_s,xoico_che_result_arr_s_create()));
    tp_t tp_identifier;
    BLM_TRY(xoico_che_s_trans_identifier(o,source,((xoico_che_result*)( result_local)),&( tp_identifier )))
    BLM_TRY(xoico_che_s_trans_whitespace(o,source,((xoico_che_result*)( result_local ))))
    if( x_source_parse_bl(source,"#?'~'" ) )
    {
        xoico_typespec_s* typespec = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_create()));
        typespec->type = ((tp_t)(TYPEOF_tp_t));
        typespec->indirection = 0;
        typespec->flag_addressable = false;
        typespec->access_class = 0;
        BLM_TRY(xoico_che_result_push_fa(result,"((tp_t)(TYPEOF_#<sc_t>))", xoico_che_s_nameof(o,tp_identifier ) ))
        BLM_TRY(xoico_che_s_trans_typespec_expression(o,source, result, typespec, out_typespec ))
    }
    else
    {
        xoico_che_result_a_push_result_d(result,((xoico_che_result*)(((xoico_che_result_arr_s*)bcore_fork(result_local)) )));
    }
    BLM_RETURNV(er_t, 0)
}

er_t xoico_che_s_trans_ternary_branch( xoico_che_s* o, x_source* source, xoico_che_result* result, xoico_typespec_s* out_typespec )
{
    // xoico_che.x:1258:1
    BLM_INIT_LEVEL(0);
    BLM_TRY(x_source_parse_fa(source,"?" ))
    BLM_TRY(xoico_che_result_a_push_sc(result,"?" ))
    xoico_typespec_s* typespec_true  = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_create()));
    xoico_typespec_s* typespec_false = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_create()));
    
    BLM_TRY(xoico_che_s_trans_expression(o,source, result, typespec_true ))
    BLM_TRY(x_source_parse_fa(source,": " ))
    BLM_TRY(xoico_che_result_a_push_sc(result,": " ))
    BLM_TRY(xoico_che_s_trans_expression(o,source, result, typespec_false ))
    
    if( typespec_true->type && typespec_false->type )
    {
        if( typespec_true->type != typespec_false->type )
        {
            BLM_TRY(x_source_parse_error_fa(source,"Ternary operator: Branches differ in type: ('#<sc_t>' : '#<sc_t>')",
                xoico_che_s_nameof(o,typespec_true->type ),
                xoico_che_s_nameof(o,typespec_false->type )
            ))
        }
    
        if( typespec_true->indirection != typespec_false->indirection )
        {
            BLM_TRY(x_source_parse_error_fa(source,"Ternary operator: Branches differ in levels of indirection: ('#<sz_t>' : '#<sz_t>')",
                typespec_true->indirection,
                typespec_false->indirection
            ))
        }
    }
    
    if( out_typespec && typespec_true->type )
    {
        xoico_typespec_s_copy(out_typespec,typespec_true );
        out_typespec->flag_addressable = false;
    }
    BLM_RETURNV(er_t, 0)
}

er_t xoico_che_s_trans_bracket( xoico_che_s* o, x_source* source, xoico_che_result* result, xoico_typespec_s* out_typespec )
{
    // xoico_che.x:1310:1
    
    BLM_TRY(x_source_parse_fa(source,"(" ))
    BLM_TRY(xoico_che_result_a_push_char(result,'(' ))
    while( !x_source_eos(source) )
    {
        BLM_TRY(xoico_che_s_trans_expression(o,source, result, out_typespec ))
        if     ( x_source_parse_bl(source,"#?')'" ) ) break;
        else if( x_source_parse_bl(source,"#?','" ) ) BLM_TRY(xoico_che_result_a_push_char(result,',' ))
        else return  x_source_parse_error_fa(source,"Syntax error in bracket expression." );
        if( out_typespec ) out_typespec->type = 0;
    }
    BLM_TRY(xoico_che_result_a_push_char(result,')' ))
    return  0;
}

er_t xoico_che_s_trans_array_subscript( xoico_che_s* o, x_source* source, xoico_che_result* result, xoico_typespec_s* out_typespec )
{
    // xoico_che.x:1334:1
    
    BLM_TRY(x_source_parse_fa(source,"[" ))
    BLM_TRY(xoico_che_result_a_push_sc(result,"[" ))
    BLM_TRY(xoico_che_s_trans_expression(o,source, result, NULL ))
    BLM_TRY(x_source_parse_fa(source,"]" ))
    BLM_TRY(xoico_che_result_a_push_sc(result,"]" ))
    return  0;
}

er_t xoico_che_s_trans_expression( xoico_che_s* o, x_source* source, xoico_che_result* result_out, xoico_typespec_s* out_typespec )
{
    // xoico_che.x:1352:1
    BLM_INIT_LEVEL(0);
    sc_t sc_bl_end_of_expression = "#?([0]==';'||[0]=='{'||[0]=='}'||[0]==')'||[0]==']'||[0]==','||([0]=='.'&&[1]=='.')||([0]==':'&&([1]==' '||[1]=='\t'||[1]=='\n'||[1]=='/')))";
    
    BLM_TRY(xoico_che_s_trans_whitespace(o,source, result_out ))
    
    xoico_che_result_arr_s* result = ((xoico_che_result_arr_s*)BLM_LEVEL_T_PUSH(0,xoico_che_result_arr_s,xoico_che_result_arr_s_create()));
    bl_t continuation = true;
    
    if( out_typespec ) xoico_typespec_s_reset(out_typespec);
    
    tp_t tp_identifier = xoico_che_s_get_identifier(o,source, false );
    
    if( tp_identifier )
    {
        // examples of builtin functions: scope, cast, try, fork
        if( xoico_che_s_is_builtin_func(o,tp_identifier ) )
        {
            xoico_typespec_s* typespec_builtin = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_create()));
            BLM_TRY(xoico_che_s_trans_builtin(o,tp_identifier, source, false, NULL, NULL,((xoico_che_result*)( result)), typespec_builtin ))
            BLM_TRY(xoico_che_s_trans_typespec_expression(o,source,((xoico_che_result*)( result)), typespec_builtin, out_typespec ))
        }
    
        else if( tp_identifier == TYPEOF_verbatim_C )
        {
            BLM_TRY(x_source_parse_fa(source,"verbatim_C {" ))
            BLM_TRY(xoico_che_s_trans_block_inside_verbatim_c(o,source,((xoico_che_result*)( result ))))
            BLM_TRY(x_source_parse_fa(source," }" ))
        }
    
        // a control name marks end of expression...
        else if( xoico_che_s_is_control_name(o,tp_identifier ) )
        {
            continuation = false;
        }
    
        // identifier represents a boolean literal
        else if( tp_identifier == TYPEOF_true || tp_identifier == TYPEOF_false )
        {
            BLM_TRY(xoico_che_s_trans_identifier(o,source,((xoico_che_result*)( result)), NULL ))
            BLM_TRY(xoico_che_s_trans_whitespace(o,source,((xoico_che_result*)( result ))))
            xoico_typespec_s* typespec = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_create()));
            typespec->type = ((tp_t)(TYPEOF_bl_t));
            typespec->indirection = 0;
            typespec->flag_addressable = false;
            BLM_TRY(xoico_che_s_trans_typespec_expression(o,source,((xoico_che_result*)( result)), typespec, out_typespec ))
        }
    
        // identifier represents a registered variable
        else if( xoico_che_s_is_var(o,tp_identifier ) )
        {
            BLM_TRY(xoico_che_s_trans_identifier(o,source,((xoico_che_result*)( result)), NULL ))
            BLM_TRY(xoico_che_s_trans_whitespace(o,source,((xoico_che_result*)( result ))))
            const xoico_typespec_s* typespec_var = xoico_che_stack_var_s_get_typespec(&(o->stack_var),tp_identifier );
            BLM_TRY(xoico_che_s_trans_typespec_expression(o,source,((xoico_che_result*)( result)), typespec_var, out_typespec ))
        }
    
        // identifier represents a type known to the compiler
        else if( xoico_che_s_is_type(o,tp_identifier ) )
        {
            BLM_TRY(xoico_che_s_trans_type(o,source,((xoico_che_result*)( result)), out_typespec ))
        }
    
        // identifier represents a type known to the compiler
        else if( xoico_che_s_is_name(o,tp_identifier ) )
        {
            BLM_TRY(xoico_che_s_trans_name(o,source,((xoico_che_result*)( result)), out_typespec ))
        }
    
        // identifier represents a (global) function name
        else if( xoico_che_s_is_func(o,tp_identifier ) )
        {
            BLM_TRY(xoico_che_s_trans_identifier(o,source, NULL, NULL ))
            sz_t source_index = x_source_get_index(source);
    
            if( x_source_parse_bl(source," #=?'('" ) ) // actual function call
            {
                const xoico_func_s* func = xoico_che_s_get_func(o,tp_identifier );
                xoico_typespec_s* typespec_ret = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_create()));
                BLM_TRY(xoico_che_s_trans_function(o,source, false, func, NULL, NULL,((xoico_che_result*)( result)), typespec_ret ))
                BLM_TRY(xoico_che_s_trans_typespec_expression(o,source,((xoico_che_result*)( result)), typespec_ret, out_typespec ))
            }
            else // function name used in untraced context
            {
                if( !o->waive_function_in_untraced_context ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Function #<sc_t> used in untraced context.\n.", xoico_che_s_nameof(o,tp_identifier ) ))
                BLM_TRY(xoico_che_result_arr_s_push_sc(result,xoico_che_s_nameof(o,tp_identifier ) ))
                x_source_set_index(source,source_index );
                BLM_TRY(xoico_che_s_trans_whitespace(o,source,((xoico_che_result*)( result ))))
            }
        }
        else // unknown/unspecified identifier
        {
            if( !o->waive_unknown_identifier )
            {
                if( !xoico_che_s_is_identifier(o,tp_identifier ) )
                {
                    sc_t sc_identifier = xoico_che_s_nameof(o,tp_identifier );
                    if( sc_t_cmp( "TYPEOF_", sc_identifier ) != 1 )
                    {
                        BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Unknwon identifier '#<sc_t>'\n.", xoico_che_s_nameof(o,tp_identifier ) ))
                    }
    
                    sc_identifier += bcore_strlen( "TYPEOF_" );
                    if( !xoico_che_s_is_identifier(o,btypeof( sc_identifier ) ) )
                    {
                        BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Unknwon identifier '#<sc_t>'\n.", xoico_che_s_nameof(o,tp_identifier ) ))
                    }
                }
            }
            BLM_TRY(xoico_che_s_trans_identifier(o,source,((xoico_che_result*)( result)), NULL ))
            BLM_TRY(xoico_che_s_trans_whitespace(o,source,((xoico_che_result*)( result ))))
    
            // assume untraced function call: bracket evaluation without internal type evaluation
            if( x_source_parse_bl(source,"#=?'('" ) )
            {
                BLM_TRY(xoico_che_s_trans_bracket(o,source,((xoico_che_result*)( result)), NULL ))
            }
        }
    }
    
    // end of expression...
    else if( x_source_parse_bl(source,sc_bl_end_of_expression ) )
    {
        continuation = false;
    }
    
    // literals and members
    else if( x_source_parse_bl(source,"#?([0]>='0'&&[0]<='9')" ) ) BLM_TRY(xoico_che_s_trans_number_literal(o,source,((xoico_che_result*)( result ))))
    else if( x_source_parse_bl(source,"#=?'\"'" )                ) BLM_TRY(xoico_che_s_trans_string_literal(o,source,((xoico_che_result*)( result ))))
    else if( x_source_parse_bl(source,"#=?|'|" )                 ) BLM_TRY(xoico_che_s_trans_char_literal(o,source,((xoico_che_result*)( result ))))
    else if( x_source_parse_bl(source,"#=?'.'" )                 ) BLM_TRY(xoico_che_s_trans_member(o,source,((xoico_che_result*)( result ))))
    else if( x_source_parse_bl(source,"#=?'->'" )                ) BLM_TRY(xoico_che_s_trans_member(o,source,((xoico_che_result*)( result ))))
    
    else if( x_source_parse_bl(source,"#=?'=<'" )                ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Attach operator: Expression not tractable." ))
    
    else if( xoico_che_s_trans_inert_operator(o,source,((xoico_che_result*)( result )))       ) {} // inert operators are not interpreted by che and passed to the c-compiler
    
    // ternary branch operator
    else if( x_source_parse_bl(source,"#=?'?'" ) )
    {
        BLM_TRY(xoico_che_s_trans_ternary_branch(o,source,((xoico_che_result*)( result)), out_typespec ))
    }
    
    // general bracket
    else if( x_source_parse_bl(source,"#=?'('" ) )
    {
        xoico_typespec_s* typespec_local = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_create()));
        BLM_TRY(xoico_che_s_trans_bracket(o,source,((xoico_che_result*)( result)), typespec_local ))
        if( typespec_local->type )
        {
            BLM_TRY(xoico_che_s_trans_typespec_expression(o,source,((xoico_che_result*)( result)), typespec_local, out_typespec ))
        }
    }
    
    // array subscript
    else if( x_source_parse_bl(source,"#=?'['" ) )
    {
        BLM_TRY(xoico_che_s_trans_array_subscript(o,source,((xoico_che_result*)( result)), out_typespec ))
    }
    
    // unhandled
    else
    {
        BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Syntax error after '#<sc_t>'", ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,xoico_che_result_a_create_st(((const xoico_che_result*)(result)))))->sc ))
    }
    
    if( continuation )
    {
        if( !x_source_parse_bl(source,sc_bl_end_of_expression ) )
        {
            if( out_typespec ) xoico_typespec_s_reset(out_typespec);
    
    
            BLM_TRY(xoico_che_s_trans_whitespace(o,source,((xoico_che_result*)( result ))))
            xoico_che_s_trans_operator(o,source,((xoico_che_result*)( result )));
    
    
            BLM_TRY(xoico_che_s_trans_expression(o,source,((xoico_che_result*)( result)), NULL ))
        }
    }
    
    if( result_out ) xoico_che_result_a_push_result_d(result_out,((xoico_che_result*)(((xoico_che_result_arr_s*)bcore_fork(result)) )));
    
    BLM_RETURNV(er_t, 0)
}

er_t xoico_che_s_try_trans_declaration( xoico_che_s* o, x_source* source, xoico_che_result* result_out, bl_t* success )
{
    // xoico_che.x:1549:1
    BLM_INIT_LEVEL(0);
    if( success ) (*(success)) = false;
    
    xoico_typespec_s* typespec_var = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_create()));
    
    sz_t index = x_source_get_index(source);
    
    xoico_che_result_arr_s* result_var = ((xoico_che_result_arr_s*)BLM_LEVEL_T_PUSH(0,xoico_che_result_arr_s,xoico_che_result_arr_s_create()));
    
    bl_t success_take_typespec = false;
    BLM_TRY(xoico_che_s_try_take_typespec(o,source, typespec_var, true, (&(success_take_typespec)) ))
    
    if( !success_take_typespec )
    {
        x_source_set_index(source,index );
        BLM_RETURNV(er_t, 0)
    }
    
    BLM_TRY(xoico_che_s_trans_whitespace(o,source,((xoico_che_result*)( result_var ))))
    
    if
    (
        typespec_var->type &&
        x_source_parse_bl(source,"#?(([0]>='A'&&[0]<='Z')||([0]>='a'&&[0]<='z')||[0]=='_')" )
    )
    {
        tp_t tp_identifier = 0;
        BLM_TRY(xoico_che_s_trans_identifier(o,source,((xoico_che_result*)( result_var)), (&(tp_identifier)) ))
        BLM_TRY(xoico_che_s_trans_whitespace(o,source,((xoico_che_result*)( result_var ))))
    
        if( x_source_parse_bl(source,"#?'='" ) ) // assignment
        {
            if( typespec_var->flag_scope ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Declaration-syntax: Stack-scope requested with subsequent assignment." ))
            bl_t pushed_typedecl = false;
            if( typespec_var->type != ((tp_t)(TYPEOF_type_deduce)) )
            {
                xoico_che_s_push_typedecl(o,typespec_var, tp_identifier );
                pushed_typedecl = true;
            }
    
            BLM_TRY(xoico_che_result_arr_s_push_sc(result_var,"=" ))
            xoico_typespec_s* typespec_expr = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_create()));
            xoico_che_result_arr_s* result_expr = ((xoico_che_result_arr_s*)BLM_LEVEL_T_PUSH(0,xoico_che_result_arr_s,xoico_che_result_arr_s_create()));
            BLM_TRY(xoico_che_s_trans_expression(o,source,((xoico_che_result*)( result_expr)), typespec_expr ))
    
            if( typespec_var->type == ((tp_t)(TYPEOF_type_deduce)) )
            {
                if( !typespec_expr->type )
                {
                    BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Declaration-syntax: Deduce requested but expression is intractable." ))
                }
                typespec_var->type = typespec_expr->type;
            }
    
            if( typespec_expr->type )
            {
                if( typespec_expr->access_class != typespec_var->access_class )
                {
                    if( typespec_expr->access_class == TYPEOF_discardable )
                    {
                        BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Declaration-syntax: Assignment: Conversion discards typespec 'discardable'." ))
                    }
    
                    if( typespec_var->access_class == TYPEOF_discardable )
                    {
                        BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Declaration-syntax: Assignment: Conversion requires typespec 'discardable'." ))
                    }
                }
                BLM_TRY(xoico_che_s_adapt_expression(o,source, typespec_expr, typespec_var,((const xoico_che_result*)( result_expr)),((xoico_che_result*)( result_var ))))
            }
            else
            {
                xoico_che_result_arr_s_push_result_d(result_var,((xoico_che_result*)(((xoico_che_result_arr_s*)bcore_fork(result_expr)) )));
            }
            BLM_TRY(xoico_che_s_push_typespec(o,typespec_var, result_out ))
            BLM_TRY(xoico_che_result_a_push_char(result_out,' ' ))
            xoico_che_result_a_push_result_d(result_out,((xoico_che_result*)(((xoico_che_result_arr_s*)bcore_fork(result_var)) )));
            if( !pushed_typedecl ) xoico_che_s_push_typedecl(o,typespec_var, tp_identifier );
        }
        else if( typespec_var->type == ((tp_t)(TYPEOF_type_deduce)) )
        {
            BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Declaration-syntax: Deduce requested without assignment." ))
        }
        else if( x_source_parse_bl(source,"#=?'['" ) ) // c-style array
        {
            if( typespec_var->flag_scope ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Declaration-syntax: Stack-scope requested on C-style array." ))
            BLM_TRY(xoico_che_s_push_typespec(o,typespec_var, result_out ))
            while( x_source_parse_bl(source,"#?'['" ) )
            {
                BLM_TRY(xoico_che_result_arr_s_push_char(result_var,'[' ))
                BLM_TRY(xoico_che_s_trans_expression(o,source,((xoico_che_result*)( result_var)), NULL ))
                BLM_TRY(x_source_parse_fa(source,"]" ))
                BLM_TRY(xoico_che_result_arr_s_push_char(result_var,']' ))
                typespec_var->indirection++;
            }
            BLM_TRY(xoico_che_result_a_push_char(result_out,' ' ))
            xoico_che_result_a_push_result_d(result_out,((xoico_che_result*)(((xoico_che_result_arr_s*)bcore_fork(result_var)) )));
            xoico_che_s_push_typedecl(o,typespec_var, tp_identifier );
    
            BLM_TRY(xoico_che_s_trans_whitespace(o,source,((xoico_che_result*)( result_var ))))
            if( x_source_parse_bl(source,"#?'='" ) ) // c-style initialization
            {
                BLM_TRY(xoico_che_result_arr_s_push_sc(result_var,"=" ))
                BLM_TRY(xoico_che_s_trans_whitespace(o,source,((xoico_che_result*)( result_var ))))
                BLM_TRY(xoico_che_s_trans_block(o,source,((xoico_che_result*)( result_var)), false ))
            }
        }
        else
        {
            BLM_TRY(xoico_che_s_push_typespec(o,typespec_var, result_out ))
            BLM_TRY(xoico_che_result_a_push_char(result_out,' ' ))
            xoico_che_result_a_push_result_d(result_out,((xoico_che_result*)(((xoico_che_result_arr_s*)bcore_fork(result_var)) )));
            xoico_che_s_push_typedecl(o,typespec_var, tp_identifier );
    
            if( typespec_var->flag_scope )
            {
                BLM_TRY(xoico_che_result_a_push_char(result_out,';' ))
                BLM_TRY(xoico_che_result_push_fa(result_out,"BLM_T_INIT_SPUSH(#<sc_t>, &#<sc_t>);", xoico_che_s_nameof(o,typespec_var->type ), xoico_che_s_nameof(o,tp_identifier ) ))
                o->stack_block.adl.data[ o->block_level ]->use_blm = true;
    
                // debug
                if( !x_source_parse_bl(source," #=?';'" ) )
                {
                    xoico_che_result_arr_s* result_local = ((xoico_che_result_arr_s*)BLM_LEVEL_T_PUSH(0,xoico_che_result_arr_s,xoico_che_result_arr_s_create()));
                    BLM_TRY(xoico_che_result_push_fa(((xoico_che_result*)(result_local)),"#<sc_t>", xoico_che_s_nameof(o,tp_identifier ) ))
                    BLM_TRY(xoico_che_s_trans_typespec_expression(o,source,((xoico_che_result*)( result_local)), typespec_var, NULL ))
                    xoico_che_result_a_push_result_d(result_out,((xoico_che_result*)(((xoico_che_result_arr_s*)bcore_fork(result_local)) )));
                }
            }
        }
    
        // catch group variable at indirection 0
        if( typespec_var->indirection == 0 && xoico_che_s_is_group(o,typespec_var->type ) )
        {
            BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Declaration-syntax: Variable of group-type has indirection 0." ))
        }
    
        if( success ) (*(success)) = true;
    }
    else // no declaration
    {
        x_source_set_index(source,index );
        BLM_RETURNV(er_t, 0)
    }
    
    if( success ) (*(success)) = true;
    
    BLM_RETURNV(er_t, 0)
}

er_t xoico_che_s_inspect_expression( xoico_che_s* o, x_source* source )
{
    // xoico_che.x:1702:1
    BLM_INIT_LEVEL(0);
    BLM_TRY(x_source_parse_fa(source,"\?\?" ))
    
    st_s* st = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
    xoico_che_result_arr_s* result_local = ((xoico_che_result_arr_s*)BLM_LEVEL_T_PUSH(0,xoico_che_result_arr_s,xoico_che_result_arr_s_create()));
    xoico_typespec_s* typespec = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_create()));
    BLM_TRY(x_source_parse_fa(source," #until';' ", st ))
    BLM_TRY(x_source_parse_fa(source,";" ))
    bcore_msg_fa( " \?? #<sc_t>;\n", st->sc );
    if( xoico_che_s_trans_expression(o,((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_fa("#<st_s*>;", st ))),((xoico_che_result*)( result_local)), typespec ) )
    {
        bcore_error_pop_to_sink(((bcore_sink*)(((x_sink*)(x_sink_stdout())) )));
        bcore_msg_fa( "\n" );
    }
    else
    {BLM_INIT_LEVEL(1);
        if( st->size == 0 ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Variable name expected." ))
        bcore_msg_fa( "--> #<sc_t>;\n", ((st_s*)BLM_LEVEL_T_PUSH(1,st_s,xoico_che_result_a_create_st(((const xoico_che_result*)(result_local)))))->sc );
    
        if( typespec->type )
        {
            bcore_msg_fa( "Expression yields typespec:\n" );
            bcore_msg_fa( "  access_class : #<sc_t>\n",  ifnameof( typespec->access_class ) );
            bcore_msg_fa( "  type         : #<sc_t>\n", xoico_che_s_nameof(o,typespec->type ) );
            bcore_msg_fa( "  indirection  : #<sz_t>\n", typespec->indirection );
            bcore_msg_fa( "  flags        : " );
            if( typespec->flag_static   ) bcore_msg_fa( "static " );
            if( typespec->flag_volatile ) bcore_msg_fa( "volatile " );
            if( typespec->flag_restrict ) bcore_msg_fa( "restrict " );
            if( typespec->flag_obliv    ) bcore_msg_fa( "obliv " );
            if( typespec->flag_aware    ) bcore_msg_fa( "aware " );
            if( typespec->flag_scope    ) bcore_msg_fa( "scope " );
            if( typespec->flag_addressable ) bcore_msg_fa( "addressable " );
            if( typespec->flag_variadic ) bcore_msg_fa( "variadic " );
            bcore_msg_fa( "\n" );
        }
        else
        {
            bcore_msg_fa( "Expression does not yield a typespec.\n" );
        }
    BLM_DOWN();}
    BLM_RETURNV(er_t, 0)
}

er_t xoico_che_s_trans_statement_expression( xoico_che_s* o, x_source* source, xoico_che_result* result )
{
    // xoico_che.x:1749:1
    BLM_INIT_LEVEL(0);
    xoico_che_result_statement_s* result_statement = xoico_che_result_statement_s__(((xoico_che_result_statement_s*)BLM_LEVEL_T_PUSH(0,xoico_che_result_statement_s,xoico_che_result_statement_s_create())),((xoico_che_result*)(xoico_che_result_arr_s_create() )));
    
    if( o->try_block_level > 0 )
    {
        xoico_typespec_s* typespec = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_create()));
        xoico_che_result_arr_s* result_expr = ((xoico_che_result_arr_s*)BLM_LEVEL_T_PUSH(0,xoico_che_result_arr_s,xoico_che_result_arr_s_create()));
        BLM_TRY(xoico_che_s_trans_expression(o,source,((xoico_che_result*)( result_expr)), typespec ))
        if
        (
            ( typespec->type == ((tp_t)(TYPEOF_er_t)) ) &&
            ( typespec->indirection == 0 ) &&
            ( typespec->flag_addressable == false )
        )
        {
            if( !x_source_parse_bl(source," #?';'" ) )
            {
                BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Inside a try-block: Expressions yielding 'er_t' must end with ';'" ))
            }
            if( xoico_che_s_returns_er_t(o) )
            {
                BLM_TRY(xoico_che_result_a_push_sc(result_statement->expression,"BLM_TRY(" ))
            }
            else
            {
                BLM_TRY(xoico_che_result_a_push_sc(result_statement->expression,"BLM_TRY_EXIT(" ))
            }
            xoico_che_result_a_push_result_d(result_statement->expression,((xoico_che_result*)(((xoico_che_result_arr_s*)bcore_fork(result_expr)) )));
            BLM_TRY(xoico_che_result_a_push_sc(result_statement->expression,")" ))
        }
        else
        {
            xoico_che_result_a_push_result_d(result_statement->expression,((xoico_che_result*)(((xoico_che_result_arr_s*)bcore_fork(result_expr)) )));
        }
    }
    else
    {
        BLM_TRY(xoico_che_s_trans_expression(o,source, result_statement->expression, NULL ))
    }
    
    // do not translate whitespaces here because it causes unwanted newline on single statement inline functions
    // o.trans_whitespace( source, result_statement.expression );
    
    xoico_che_result_a_push_result_d(result,((xoico_che_result*)(((xoico_che_result_statement_s*)bcore_fork(xoico_che_result_statement_s_reduce(result_statement))) )));
    
    BLM_RETURNV(er_t, 0)
}

er_t xoico_che_s_trans_statement( xoico_che_s* o, x_source* source, xoico_che_result* result )
{
    // xoico_che.x:1800:1
    
    BLM_TRY(xoico_che_s_trans_whitespace(o,source, result ))
    
    bl_t handled = true;
    
    char c = x_source_inspect_char(source);
    switch( c )
    {
        case '{':
        {
            BLM_TRY(xoico_che_s_trans_block(o,source, result, false ))
        }
        break;
    
        case '#':
        {
            BLM_TRY(xoico_che_s_trans_preprocessor(o,source, result ))
        }
        break;
    
        case ';': /// consume semicolon (!)
        {
            x_source_get_char(source);
            BLM_TRY(xoico_che_result_a_push_char(result,';' ))
        }
        break;
    
        case ',':
        {
            x_source_get_char(source);
            BLM_TRY(xoico_che_result_a_push_char(result,',' ))
        }
        break;
    
        case '=': // completion statement (same as return)
        {
            BLM_TRY(xoico_che_s_trans_control(o,TYPEOF_completion, source, result ))
        }
        break;
    
        case '?':
        {
            if( x_source_parse_bl(source,"#?([1]=='?')" ) )
            {
                BLM_TRY(xoico_che_s_inspect_expression(o,source ))
            }
            else
            {
                handled = false;
            }
        }
        break;
    
        case ')':
        {
            return  x_source_parse_error_fa(source,"Unmatched closing bracket." );
        }
        break;
    
        case '.':
        {
            return  x_source_parse_error_fa(source,"Incorrect use of delimiter '.'." );
        }
        break;
    
        case '}':
        {
            // nothing (caller is a block-handler)
        }
        break;
    
        default:
        {
            handled = false;
        }
        break;
    }
    
    if( !handled )
    {
        tp_t tp_identifier = xoico_che_s_get_identifier(o,source, false );
    
        if( tp_identifier && xoico_che_s_is_control_name(o,tp_identifier ) )
        {
            BLM_TRY(xoico_che_s_trans_control(o,tp_identifier, source, result ))
        }
        else
        {
            bl_t success_declaration = false;
            BLM_TRY(xoico_che_s_try_trans_declaration(o,source, result, (&(success_declaration)) ))
            if( !success_declaration ) BLM_TRY(xoico_che_s_trans_statement_expression(o,source, result ))
        }
    }
    
    return  0;
}

er_t xoico_che_s_trans_block_inside( xoico_che_s* o, x_source* source, xoico_che_result* result_out )
{
    // xoico_che.x:1900:1
    BLM_INIT_LEVEL(0);
    xoico_che_result_arr_s* result = ((xoico_che_result_arr_s*)BLM_LEVEL_T_PUSH(0,xoico_che_result_arr_s,xoico_che_result_arr_s_create()));
    
    s3_t source_index = x_source_get_index(source);
    
    while( !x_source_parse_bl(source,"#=?'}'" ) && !x_source_eos(source) )
    {
        BLM_TRY(xoico_che_s_trans_statement(o,source,((xoico_che_result*)( result ))))
    
        s3_t index = x_source_get_index(source);
        if( index == source_index && !x_source_eos(source) )
        {
            BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Internal error: Statement translator did not progress." ))
        }
        source_index = index;
    }
    
    if( xoico_che_s_stack_block_get_top_unit(o)->use_blm )
    {
        xoico_che_result_block_s* result_block = ((xoico_che_result_block_s*)BLM_LEVEL_T_PUSH(0,xoico_che_result_block_s,xoico_che_result_block_s__(xoico_che_result_block_s_create(),o->block_level, true )));
    
        xoico_che_result_block_s_push_result_d(result_block,((xoico_che_result*)(xoico_che_result_blm_init_s__(xoico_che_result_blm_init_s_create(),o->block_level ) )));
        xoico_che_result_block_s_push_result_d(result_block,((xoico_che_result*)(((xoico_che_result_arr_s*)bcore_fork(result)) )));
    
        if( ( o->block_level > 0 ) || !xoico_che_s_returns_a_value(o) )
        {
            xoico_che_result_block_s_push_result_d(result_block,((xoico_che_result*)(xoico_che_result_blm_down_s_create() )));
        }
    
        xoico_che_result_a_push_result_d(result_out,((xoico_che_result*)(((xoico_che_result_block_s*)bcore_fork(result_block)) )));
    }
    else
    {
        xoico_che_result_a_push_result_d(result_out,((xoico_che_result*)(((xoico_che_result_arr_s*)bcore_fork(result)) )));
    }
    
    BLM_RETURNV(er_t, 0)
}

er_t xoico_che_s_trans_block( xoico_che_s* o, x_source* source, xoico_che_result* result_out, bl_t is_break_ledge )
{
    // xoico_che.x:1942:1
    BLM_INIT_LEVEL(0);
    xoico_che_s_inc_block(o);
    xoico_che_result_arr_s* result = ((xoico_che_result_arr_s*)BLM_LEVEL_T_PUSH(0,xoico_che_result_arr_s,xoico_che_result_arr_s_create()));
    xoico_che_s_stack_block_get_top_unit(o)->break_ledge = is_break_ledge;
    BLM_TRY(xoico_che_s_trans_whitespace(o,source,((xoico_che_result*)( result ))))
    BLM_TRY(xoico_che_s_trans(o,source, "{",((xoico_che_result*)( result ))))
    BLM_TRY(xoico_che_s_trans_block_inside(o,source,((xoico_che_result*)( result ))))
    BLM_TRY(xoico_che_s_trans_whitespace(o,source,((xoico_che_result*)( result ))))
    BLM_TRY(xoico_che_s_trans(o,source, "}",((xoico_che_result*)( result ))))
    xoico_che_result_a_push_result_d(result_out,((xoico_che_result*)(((xoico_che_result_arr_s*)bcore_fork(result)) )));
    xoico_che_s_dec_block(o);
    BLM_RETURNV(er_t, 0)
}

er_t xoico_che_s_trans_statement_as_block( xoico_che_s* o, x_source* source, xoico_che_result* result_out, bl_t is_break_ledge )
{
    // xoico_che.x:1959:1
    BLM_INIT_LEVEL(0);
    xoico_che_result_arr_s* result = ((xoico_che_result_arr_s*)BLM_LEVEL_T_PUSH(0,xoico_che_result_arr_s,xoico_che_result_arr_s_create()));
    
    xoico_che_s_inc_block(o);
    
    {
        xoico_che_stack_block_unit_s* top_unit = xoico_che_s_stack_block_get_top_unit(o);
        top_unit->break_ledge = is_break_ledge;
        top_unit->statement_wrapped_as_block = true;
    }
    
    BLM_TRY(xoico_che_s_trans_statement(o,source,((xoico_che_result*)( result ))))
    BLM_TRY(xoico_che_s_trans_whitespace(o,source,((xoico_che_result*)( result ))))
    if( x_source_parse_bl(source,"#?';'" ) ) BLM_TRY(xoico_che_result_arr_s_push_sc(result,";" ))
    
    if( xoico_che_s_stack_block_get_top_unit(o)->use_blm )
    {
        xoico_che_result_block_s* result_block = ((xoico_che_result_block_s*)BLM_LEVEL_T_PUSH(0,xoico_che_result_block_s,xoico_che_result_block_s__(xoico_che_result_block_s_create(),o->block_level, true )));
        xoico_che_result_block_s_push_result_d(result_block,((xoico_che_result*)(xoico_che_result_blm_init_s__(xoico_che_result_blm_init_s_create(),o->block_level ) )));
        xoico_che_result_block_s_push_result_d(result_block,((xoico_che_result*)(((xoico_che_result_arr_s*)bcore_fork(result)) )));
        xoico_che_result_block_s_push_result_d(result_block,((xoico_che_result*)(xoico_che_result_blm_down_s_create() )));
    
        BLM_TRY(xoico_che_result_a_push_sc(result_out,"{" ))
        xoico_che_result_a_push_result_d(result_out,((xoico_che_result*)(((xoico_che_result_block_s*)bcore_fork(result_block)) )));
        BLM_TRY(xoico_che_result_a_push_sc(result_out,"}" ))
    }
    else
    {
        xoico_che_result_a_push_result_d(result_out,((xoico_che_result*)(((xoico_che_result_arr_s*)bcore_fork(result)) )));
    }
    
    xoico_che_s_dec_block(o);
    BLM_RETURNV(er_t, 0)
}

er_t xoico_che_s_trans_block_inside_verbatim_c( xoico_che_s* o, x_source* source, xoico_che_result* result )
{
    // xoico_che.x:1997:1
    
    o->has_verbatim_code = true;
    BLM_TRY(xoico_che_s_trans_whitespace(o,source, result ))
    while( !x_source_parse_bl(source,"#=?'}'" ) && !x_source_eos(source) )
    {
        switch( x_source_inspect_char(source) )
        {
            case '"':
            {
                BLM_TRY(xoico_che_s_trans_string_literal(o,source, result ))
            }
            break;
    
            case '\'':
            {
                BLM_TRY(xoico_che_s_trans_char_literal(o,source, result ))
            }
            break;
    
            case '{':
            {
                BLM_TRY(xoico_che_result_a_push_char(result,x_source_get_char(source) ))
                BLM_TRY(xoico_che_s_trans_block_inside_verbatim_c(o,source, result ))
                BLM_TRY(x_source_parse_fa(source,"}" ))
                BLM_TRY(xoico_che_result_a_push_char(result,'}' ))
            }
            break;
    
            default:
            {
                BLM_TRY(xoico_che_result_a_push_char(result,x_source_get_char(source) ))
            }
            break;
        }
        BLM_TRY(xoico_che_s_trans_whitespace(o,source, result ))
    }
    return  0;
}

er_t xoico_che_s_trans_level0_block( xoico_che_s* o, bl_t exit_after_first_statement, x_source* source, xoico_che_result* result_out )
{
    // xoico_che.x:2040:1
    BLM_INIT_LEVEL(0);
    xoico_che_result_arr_s* result = ((xoico_che_result_arr_s*)BLM_LEVEL_T_PUSH(0,xoico_che_result_arr_s,xoico_che_result_arr_s_create()));
    
    s3_t source_index = x_source_get_index(source);
    
    while( !x_source_parse_bl(source,"#=?'}'" ) && !x_source_eos(source) )
    {
        BLM_TRY(xoico_che_s_trans_statement(o,source,((xoico_che_result*)( result ))))
    
        if( exit_after_first_statement )
        {
            if( x_source_parse_bl(source,"#?';'" ) ) BLM_TRY(xoico_che_result_arr_s_push_sc(result,";" ))
            break;
        }
    
        s3_t index = x_source_get_index(source);
        if( index == source_index && !x_source_eos(source) )
        {
            BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Internal error: Statement translator did not progress." ))
        }
        source_index = index;
    }
    
    if( xoico_che_s_stack_block_get_top_unit(o)->use_blm )
    {
        xoico_che_result_block_s* result_block = ((xoico_che_result_block_s*)BLM_LEVEL_T_PUSH(0,xoico_che_result_block_s,xoico_che_result_block_s__(xoico_che_result_block_s_create(),o->block_level, true )));
    
        xoico_che_result_block_s_push_result_d(result_block,((xoico_che_result*)(xoico_che_result_blm_init_s__(xoico_che_result_blm_init_s_create(),o->block_level ) )));
        xoico_che_result_block_s_push_result_d(result_block,((xoico_che_result*)(((xoico_che_result_arr_s*)bcore_fork(result)) )));
    
        if( !xoico_che_s_returns_a_value(o) )
        {
            xoico_che_result_block_s_push_result_d(result_block,((xoico_che_result*)(xoico_che_result_blm_down_s_create() )));
        }
    
        xoico_che_result_a_push_result_d(result_out,((xoico_che_result*)(((xoico_che_result_block_s*)bcore_fork(result_block)) )));
    }
    else
    {
        xoico_che_result_a_push_result_d(result_out,((xoico_che_result*)(((xoico_che_result_arr_s*)bcore_fork(result)) )));
    }
    
    BLM_RETURNV(er_t, 0)
}

er_t xoico_che_s_setup( xoico_che_s* o, const xoico_host* host, const xoico_signature_s* signature )
{
    // xoico_che.x:2088:1
    BLM_INIT_LEVEL(0);
    xoico_signature_s_attach( &(o->signature ),  xoico_signature_s_clone(signature));
    BLM_TRY(xoico_signature_s_relent(o->signature,host, xoico_host_a_obj_type(host) ))
    
    o->host     = ((xoico_host*)(host));
    o->compiler = xoico_host_a_compiler(host);
    o->block_level     = 0;
    o->try_block_level = 0;
    xoico_che_stack_var_s_clear(&(o->stack_var));
    xoico_che_s_init_level0(o);
    
    if( o->signature->arg_o )
    {
        xoico_che_stack_var_unit_s* unit = ((xoico_che_stack_var_unit_s*)BLM_LEVEL_T_PUSH(0,xoico_che_stack_var_unit_s,xoico_che_stack_var_unit_s_create()));
        tp_t tp_member_obj_name  = xoico_che_s_entypeof(o,"o" );
    
        xoico_typespec_s_copy(&(unit->typespec),&(o->signature->arg_o->typespec ));
        unit->name = tp_member_obj_name;
        unit->level = o->block_level;
    
        if( !o->waive_unknown_type && !xoico_che_s_is_type(o,unit->typespec.type ) )
        {
            BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(&(signature->source_point),"Declaration: Argument '#<sc_t>' has an unknown type.", xoico_che_s_nameof(o,unit->name ) ))
        }
    
        xoico_che_stack_var_s_push_unit(&(o->stack_var),unit );
        bcore_hmap_name_s_set_sc(&(o->hmap_name),xoico_compiler_s_nameof(o->compiler,o->signature->arg_o->typespec.type ) );
        bcore_hmap_name_s_set_sc(&(o->hmap_name),xoico_compiler_s_nameof(o->compiler,tp_member_obj_name ) );
    }
    
    {const xoico_args_s* __a=&(o->signature->args );if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_arg_s* arg=__a->data[__i];
    {
        if( arg->typespec.type && arg->name )
        {
            xoico_che_stack_var_unit_s* unit = ((xoico_che_stack_var_unit_s*)BLM_LEVEL_T_PUSH(0,xoico_che_stack_var_unit_s,xoico_che_stack_var_unit_s_create()));
            xoico_typespec_s_copy(&(unit->typespec),&(arg->typespec ));
            unit->name = arg->name;
            unit->level = o->block_level;
    
            if( !o->waive_unknown_type && !xoico_che_s_is_type(o,unit->typespec.type ) )
            {
                BLM_RETURNV(er_t, x_source_point_s_parse_error_fa(&(signature->source_point),"Declaration: Argument '#<sc_t>' has an unknown type.", xoico_che_s_nameof(o,unit->name ) ))
            }
    
            xoico_che_stack_var_s_push_unit(&(o->stack_var),unit );
            bcore_hmap_name_s_set_sc(&(o->hmap_name),xoico_compiler_s_nameof(o->compiler,arg->typespec.type ) );
            bcore_hmap_name_s_set_sc(&(o->hmap_name),xoico_compiler_s_nameof(o->compiler,arg->name ) );
        }
    }
    }}BLM_RETURNV(er_t, 0)
}

sz_t xoico_che_s_assess_indentation( x_source* source )
{
    // xoico_che.x:2143:1
    
    sz_t index = x_source_get_index(source);
    while( !x_source_eos(source) ) if( x_source_get_char(source) == '\n' ) break;
    sz_t indentation = 0;
    while( !x_source_eos(source) )
    {
        if( x_source_get_char(source) != ' ' ) break;
        indentation++;
    }
    x_source_set_index(source,index );
    return  indentation;
}

void xoico_che_s_remove_indentation( st_s* string, sz_t indentation )
{
    // xoico_che.x:2159:1
    
    ASSERT( string->space >= string->size );
    
    sz_t j = 0;
    for(sz_t i = 0; i < string->size; i++ )
    {
        char c = string->data[ i ];
        string->data[ j++ ] = c;
        if( c == '\n' )
        {
            for(sz_t k = 0; k < indentation; k++ )
            {
                if( string->data[ i + 1 ] != ' ' ) break;
                i++;
            }
        }
    }
    
    string->data[ j ] = 0;
    string->size = j;
}

er_t xoico_che_s_translate_mutable( xoico_che_s* o, const xoico_host* host, const xoico_body_s* body, const xoico_signature_s* signature, x_sink* sink )
{
    // xoico_che.x:2184:1
    BLM_INIT_LEVEL(0);
    BLM_TRY(xoico_che_s_setup(o,host, signature ))
    
    x_source* source = ((x_source*)BLM_LEVEL_A_PUSH(0,x_source_point_s_clone_source(&(body->code->source_point))));
    
    xoico_che_result_arr_s* result = ((xoico_che_result_arr_s*)BLM_LEVEL_T_PUSH(0,xoico_che_result_arr_s,xoico_che_result_arr_s_create()));
    
    bl_t flag_verbatim_c = false;
    bl_t flag_try = o->for_all_functions_enable_try;
    
    if( x_source_parse_bl(source," #?'('" ) )
    {
        while( !x_source_parse_bl(source," #?')'" ) )
        {
            if( x_source_parse_bl(source," #?w'try'" ) )
            {
                BLM_RETURNV(er_t, x_source_parse_error_fa(source,"prefix (try) is deprecated." ))
                //flag_try = true;
            }
            else if( x_source_parse_bl(source," #?w'verbatim_C'" ) )
            {
                BLM_TRY(x_source_parse_error_fa(source,"prefix (verbatim_C) is deprecated." ))
                flag_verbatim_c = true;
            }
            else
            {
                BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Invalid c-engine expression." ))
            }
        }
    }
    
    sz_t indentation = 0;
    
    if( x_source_parse_bl(source," #?'{'" ) )
    {
        if( !body->go_inline ) indentation = xoico_che_s_assess_indentation(source );
    
        if( flag_verbatim_c )
        {
            BLM_TRY(xoico_che_s_trans_block_inside_verbatim_c(o,source,((xoico_che_result*)( result ))))
        }
        else
        {
            o->try_block_level += flag_try;
            BLM_TRY(xoico_che_s_trans_level0_block(o,false, source,((xoico_che_result*)( result ))))
            o->try_block_level -= flag_try;
        }
        BLM_TRY(x_source_parse_fa(source," }" ))
    }
    else // single statement body
    {
        if( !body->go_inline ) indentation = xoico_che_s_assess_indentation(source );
        o->try_block_level += flag_try;
        BLM_TRY(xoico_che_s_trans_level0_block(o,true, source,((xoico_che_result*)( result ))))
        o->try_block_level -= flag_try;
    }
    
    if( xoico_che_s_returns_a_value(o) && !o->has_completion && !o->has_verbatim_code )
    {
        if( o->signature->direct_return_arg )
        {BLM_INIT_LEVEL(2);
            xoico_che_result_arr_s_push_result_d(result,((xoico_che_result*)(xoico_che_result_return_s__(xoico_che_result_return_s_create(),o,((xoico_che_result*)( xoico_che_result_plain_s__(xoico_che_result_plain_s_create(),st_s_push_sc(((st_s*)BLM_LEVEL_T_PUSH(2,st_s,st_s_create())),xoico_che_s_nameof(o,o->signature->direct_return_arg->name ) ) ) ))) )));
        BLM_DOWN();}
        else
        {
            x_btml_to_stdout(((const x_btml*)(o->signature)));
            BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Function must return a value. Completion statement ('return' or '=') expected." ))
        }
    }
    
    xoico_che_result_block_s* result_block = ((xoico_che_result_block_s*)BLM_LEVEL_T_PUSH(0,xoico_che_result_block_s,xoico_che_result_block_s__(xoico_che_result_block_s_create(),o->block_level, xoico_che_s_stack_block_get_bottom_unit(o)->use_blm )));
    ((xoico_che_result_block_s*)(result_block))->is_root = true;
    xoico_che_result_block_s_push_result_d(result_block,((xoico_che_result*)(((xoico_che_result_arr_s*)bcore_fork(result)) )));
    
    st_s* buf = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
    BLM_TRY(xoico_che_result_block_s_to_sink(result_block,((x_sink*)(buf ))))
    
    //remove trailing whitespaces
    for(sz_t i = buf->size - 1; i >= 0; i-- )
    {
        if( buf->data[ i ] != ' ' && buf->data[ i ] != '\t' && buf->data[ i ] != '\n' ) break;
        buf->data[ i ] = 0;
        buf->size = i;
    }
    
    if( o->insert_source_reference && !body->code->single_line )
    {
        x_sink_push_fa(sink,"// " );
        x_source_point_s_source_reference_to_sink(&(body->code->source_point),true, sink );
        x_sink_push_fa(sink,"\n" );
    }
    
    if( indentation > 0 ) xoico_che_s_remove_indentation(buf, indentation );
    
    if( o->verbose )
    {
        x_sink_push_fa(x_sink_stdout(),"##############################\n" );
        x_sink_push_fa(x_sink_stdout(),"#<st_s*>\n", buf );
    }
    
    x_sink_push_sc(sink,buf->sc );
    
    BLM_RETURNV(er_t, 0)
}

er_t xoico_che_s_translate( const xoico_che_s* o, const xoico_host* host, const xoico_body_s* body, const xoico_signature_s* signature, x_sink* sink )
{
    // xoico_che.x:2292:1
    BLM_INIT_LEVEL(0);
    er_t er = xoico_che_s_translate_mutable(((xoico_che_s*)BLM_LEVEL_T_PUSH(0,xoico_che_s,xoico_che_s_clone(o))),host, body, signature, sink );
    BLM_RETURNV(er_t, er)
}

er_t xoico_che_s_trans_typespec_member( xoico_che_s* o, x_source* source, xoico_che_result* result, const xoico_typespec_s* in_typespec, xoico_typespec_s* out_typespec )
{
    // xoico_che_trans_typespec.x:28:1
    BLM_INIT_LEVEL(0);
    if( x_source_parse_bl(source,"#?'->'" ) )
    {
        if( in_typespec->indirection != 1 )
        {
            BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Given indirection is '#<sz_t>'. '->' can only be used at indirection '1'.", in_typespec->indirection ))
        }
    }
    else
    {
        BLM_TRY(x_source_parse_fa(source,"." ))
    }
    BLM_TRY(x_source_parse_fa(source," " ))
    xoico_compiler_element_info_s* info = ((xoico_compiler_element_info_s*)BLM_LEVEL_T_PUSH(0,xoico_compiler_element_info_s,xoico_compiler_element_info_s_create()));
    
    char c = x_source_inspect_char(source);
    
    if( c >= '0' && c <= '9' )
    {
        xoico_typespec_s* typespec_adapted = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_clone(in_typespec)));
        sz_t adapted_indirection = 0;
        BLM_TRY(x_source_parse_fa(source,"#<sz_t*>", &adapted_indirection ))
    
        typespec_adapted->indirection = adapted_indirection;
    
        xoico_che_result_arr_s* result_adapted = ((xoico_che_result_arr_s*)BLM_LEVEL_T_PUSH(0,xoico_che_result_arr_s,xoico_che_result_arr_s_create()));
        BLM_TRY(xoico_che_result_arr_s_push_sc(result_adapted,"(" ))
        BLM_TRY(xoico_che_s_adapt_expression(o,source, in_typespec, typespec_adapted, result,((xoico_che_result*)( result_adapted ))))
        BLM_TRY(xoico_che_result_arr_s_push_sc(result_adapted,")" ))
        xoico_che_result_a_clear(result);
        xoico_che_result_a_push_result_d(result,((xoico_che_result*)(((xoico_che_result_arr_s*)bcore_fork(result_adapted)) )));
        BLM_TRY(xoico_che_s_trans_typespec_expression(o,source, result, typespec_adapted, out_typespec ))
        BLM_RETURNV(er_t, 0)
    }
    
    if( c == '*' || c == '&' )
    {
        BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Postfix operators '&' and '*' are deprecated. Use numeric indirection level.\n" ))
    }
    
    if( x_source_parse_bl(source,"#=?'['" ) || x_source_parse_bl(source,"#=?'?['" ) ) // array subscript
    {
        bl_t bounds_check = false;
        if( x_source_parse_bl(source,"#=?'?'" ) )
        {
            BLM_TRY(x_source_parse_fa(source,"?[" ))
            bounds_check = true;
        }
        else
        {
            BLM_TRY(x_source_parse_fa(source,"[" ))
        }
    
        if( bounds_check ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Bounds check not yet available." ))
    
        if( in_typespec->indirection > 1 )
        {
            BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Resolving subscript: Indirection '#<sz_t>' is too large.",
                in_typespec->indirection
            ))
        }
    
        BLM_TRY(xoico_che_result_push_fa(result,"#<sc_t>data[", ( in_typespec->indirection == 1 ) ? "->" : "." ))
        BLM_TRY(xoico_che_s_trans_expression(o,source, result, NULL ))
        BLM_TRY(x_source_parse_fa(source,"]" ))
        BLM_TRY(xoico_che_result_a_push_sc(result,"]" ))
    
        if( xoico_compiler_s_get_type_array_element_info(o->compiler,in_typespec->type, info ) )
        {
            BLM_TRY(xoico_che_s_trans_typespec_expression(o,source, result,&( info->type_info.typespec), out_typespec ))
        }
        else
        {
            BLM_TRY(xoico_che_s_trans_expression(o,source, result, NULL ))
        }
    }
    else // member (object or function)
    {BLM_INIT_LEVEL(1);
        xoico_che_result_arr_s* result_local = ((xoico_che_result_arr_s*)BLM_LEVEL_T_PUSH(1,xoico_che_result_arr_s,xoico_che_result_arr_s_create()));
    
        /// function_without_brackets is used for shortcuts (e.g. .! or .#) that are expanded as unary member functions with implicit arguments
        bl_t function_without_brackets = false;
    
        tp_t tp_identifier = xoico_che_s_get_identifier(o,source, true );
        if( !tp_identifier )
        {
            if( x_source_parse_bl(source,"#=?'('" ) ) // anonymous function call
            {
                tp_identifier = xoico_che_s_entypeof(o,"" );
            }
            else if( x_source_parse_bl(source,"#?'!'" ) ) // function 'clone'
            {
                tp_identifier = xoico_che_s_entypeof(o,"clone" );
                function_without_brackets = true;
            }
            else if( x_source_parse_bl(source,"#?'#'" ) ) // function 'fork'
            {
                tp_identifier = xoico_che_s_entypeof(o,"fork" );
                function_without_brackets = true;
            }
            else
            {
                BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Identifier expected" ))
            }
        }
        BLM_TRY(xoico_che_result_arr_s_push_sc(result_local,xoico_che_s_nameof(o,tp_identifier ) ))
    
        BLM_TRY(xoico_che_s_trans_whitespace(o,source,((xoico_che_result*)( result_local ))))
    
        bl_t try_member_function = function_without_brackets || x_source_parse_bl(source,"#=?'('" );
        bl_t try_member_object   = !try_member_function;
    
        if( try_member_function )
        {
            if( xoico_che_s_is_builtin_func(o,tp_identifier ) )
            {
                xoico_typespec_s* typespec_builtin = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_create()));
                BLM_TRY(xoico_che_s_trans_builtin(o,tp_identifier, source, function_without_brackets, result, in_typespec,((xoico_che_result*)( result_local)), typespec_builtin ))
                xoico_che_result_a_copy(result,((const xoico_che_result*)(result_local )));
                BLM_TRY(xoico_che_s_trans_typespec_expression(o,source, result, typespec_builtin, out_typespec ))
            }
            else if( xoico_compiler_s_get_type_member_function_info(o->compiler,in_typespec->type, tp_identifier, info ) )
            {BLM_INIT_LEVEL(4);
                xoico_typespec_s* typespec_ret = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(4,xoico_typespec_s,xoico_typespec_s_create()));
                xoico_che_result* result_object_expr = ((xoico_che_result*)BLM_LEVEL_A_PUSH(4,xoico_che_result_a_clone(result)));
                xoico_che_result_a_clear(result);
                BLM_TRY(xoico_che_s_trans_function(o,source, function_without_brackets, info->func, result_object_expr, in_typespec, result, typespec_ret ))
                BLM_TRY(xoico_che_s_trans_typespec_expression(o,source, result, typespec_ret, out_typespec ))
            BLM_DOWN();}
            // possibly a compact function of a member object
            else if( xoico_compiler_s_get_type_member_object_info(o->compiler,in_typespec->type, tp_identifier, info ) )
            {
                try_member_object = true;
            }
            else // untraced member function
            {
                if( !o->waive_unknown_member_function )
                {
                    BLM_RETURNV(er_t, x_source_parse_error_fa(source,"'#<sc_t>' has no member function '#<sc_t>'.", xoico_che_s_nameof(o,in_typespec->type ), xoico_che_s_nameof(o,tp_identifier ) ))
                }
    
                xoico_che_result* result_arg_obj = ((xoico_che_result*)BLM_LEVEL_A_PUSH(0,xoico_che_result_a_clone(result)));
                xoico_che_result_a_clear(result);
    
                /// Untraced member functions of a group are always treated as 'aware'
                if( xoico_che_s_is_group(o,in_typespec->type ) )
                {
                    BLM_TRY(xoico_che_result_copy_fa(result,"#<sc_t>_a_#<sc_t>( ",
                        xoico_che_s_nameof(o,in_typespec->type ),
                        xoico_che_s_nameof(o,tp_identifier )
                    ))
                }
                else
                {
                    BLM_TRY(xoico_che_result_copy_fa(result,"#<sc_t>_#<sc_t>( ",
                        xoico_che_s_nameof(o,in_typespec->type ),
                        xoico_che_s_nameof(o,tp_identifier )
                    ))
                }
    
                {
                    xoico_typespec_s* typespec_obj = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_clone(in_typespec)));
                    typespec_obj->indirection = 1; // first argument of member functions
                    BLM_TRY(xoico_che_s_adapt_expression(o,source, in_typespec, typespec_obj, result_arg_obj, result ))
                }
    
                bl_t first = true;
                BLM_TRY(xoico_che_s_trans_whitespace(o,source,((xoico_che_result*)( result_local ))))
                while( !x_source_eos(source) )
                {BLM_INIT_LEVEL(4);
                    if( x_source_parse_bl(source,"#=?')'" ) ) BLM_BREAK_LEVEL(4);
    
                    xoico_che_result_arr_s* result_expr = ((xoico_che_result_arr_s*)BLM_LEVEL_T_PUSH(4,xoico_che_result_arr_s,xoico_che_result_arr_s_create()));
                    if( !first ) BLM_TRY(x_source_parse_fa(source,"," ))
                    BLM_TRY(xoico_che_s_trans_expression(o,source,((xoico_che_result*)( result_expr)), NULL ))
                    BLM_TRY(xoico_che_s_trans_whitespace(o,source,((xoico_che_result*)( result_expr ))))
                    BLM_TRY(xoico_che_result_push_fa(result,"," ))
                    xoico_che_result_a_push_result_d(result,((xoico_che_result*)(((xoico_che_result_arr_s*)bcore_fork(result_expr)) )));
                    first = false;
                BLM_DOWN();}
    
                BLM_TRY(x_source_parse_fa(source,")" ))
                BLM_TRY(xoico_che_result_a_push_sc(result,")" ))
    
                BLM_TRY(xoico_che_s_trans_expression(o,source, result, NULL ))
            }
        }
    
        if( try_member_object ) // member object
        {
            if( xoico_compiler_s_get_type_member_object_info(o->compiler,in_typespec->type, tp_identifier, info ) ) // traced member object
            {
                if( info->func )
                {
                    BLM_RETURNV(er_t, x_source_parse_error_fa(source,"'#<sc_t>' is a member function.", xoico_che_s_nameof(o,tp_identifier ) ))
                }
    
                if( in_typespec->indirection <= 1 )
                {
                    BLM_TRY(xoico_che_result_push_fa(result,"#<sc_t>", ( in_typespec->indirection == 1 ) ? "->" : "." ))
                }
                else
                {BLM_INIT_LEVEL(4);
                    xoico_che_result* result_object_expr = ((xoico_che_result*)BLM_LEVEL_A_PUSH(4,xoico_che_result_a_clone(result)));
                    xoico_che_result_a_clear(result);
                    BLM_TRY(xoico_che_result_push_fa(result,"(#rn{*}(", in_typespec->indirection ))
                    xoico_che_result_a_push_result_d(result,((xoico_che_result*)bcore_fork(result_object_expr)) );
                    BLM_TRY(xoico_che_result_push_fa(result,"))." ))
                BLM_DOWN();}
                xoico_che_result_a_push_result_d(result,((xoico_che_result*)(((xoico_che_result_arr_s*)bcore_fork(result_local)) )));
    
                BLM_TRY(xoico_che_s_trans_typespec_expression(o,source, result,&( info->type_info.typespec), out_typespec ))
            }
            else // untraced member object
            {
                if( !o->waive_unknown_member_variable )
                {
                    BLM_RETURNV(er_t, x_source_parse_error_fa(source,"'#<sc_t>' has no member '#<sc_t>'.", xoico_che_s_nameof(o,in_typespec->type ), xoico_che_s_nameof(o,tp_identifier ) ))
                }
                BLM_TRY(xoico_che_result_push_fa(result,"#<sc_t>", ( in_typespec->indirection == 1 ) ? "->" : "." ))
                xoico_che_result_a_push_result_d(result,((xoico_che_result*)(((xoico_che_result_arr_s*)bcore_fork(result_local)) )));
                BLM_TRY(xoico_che_s_trans_expression(o,source, result, NULL ))
            }
        }
    BLM_DOWN();}
    BLM_RETURNV(er_t, 0)
}

er_t xoico_che_s_trans_typespec_array_subscript( xoico_che_s* o, x_source* source, xoico_che_result* result, const xoico_typespec_s* in_typespec, xoico_typespec_s* out_typespec )
{
    // xoico_che_trans_typespec.x:272:1
    BLM_INIT_LEVEL(0);
    BLM_TRY(x_source_parse_fa(source,"[" ))
    
    BLM_TRY(xoico_che_result_a_push_sc(result,"[" ))
    BLM_TRY(xoico_che_s_trans_expression(o,source, result, NULL ))
    BLM_TRY(x_source_parse_fa(source,"]" ))
    BLM_TRY(xoico_che_result_a_push_sc(result,"]" ))
    
    xoico_typespec_s* typespec = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_clone(in_typespec)));
    
    if( typespec->indirection == 0 )
    {
        if( in_typespec->type == ((tp_t)(TYPEOF_sc_t)) || in_typespec->type == ((tp_t)(TYPEOF_sd_t)) )
        {
            typespec->type = ((tp_t)(TYPEOF_u0_t));
        }
        else
        {
            BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Array subscript requires indirection >= 1." ))
        }
    }
    else
    {
        typespec->indirection--;
    }
    
    BLM_TRY(xoico_che_s_trans_typespec_expression(o,source, result, typespec, out_typespec ))
    
    BLM_RETURNV(er_t, 0)
}

er_t xoico_che_s_trans_typespec_create( xoico_che_s* o, x_source* source, xoico_che_result* result, const xoico_typespec_s* in_typespec, xoico_typespec_s* out_typespec )
{
    // xoico_che_trans_typespec.x:313:1
    BLM_INIT_LEVEL(0);
    BLM_TRY(x_source_parse_fa(source,"!" ))
    
    //ignore in case indirection is 0;
    if( in_typespec->indirection > 0 )
    {
        if( !( in_typespec->indirection == 1 && in_typespec->flag_addressable ) )
        {
            BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Create-Operator requires lvalue with addressable indirection of 1." ))
        }
    
        if( xoico_che_s_is_group( o,in_typespec->type ) )
        {
            BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Create-Operator: lvalue is a group." ))
        }
    
        xoico_che_result* result_arg_obj = ((xoico_che_result*)BLM_LEVEL_A_PUSH(0,xoico_che_result_a_clone(result)));
        xoico_che_result_a_clear(result);
    
        sc_t sc_type = xoico_che_s_nameof(o,in_typespec->type );
        BLM_TRY(xoico_che_result_push_fa(result,"BCORE_PASS_CREATE(#<sc_t>,", sc_type ))
        xoico_che_result_a_push_result_d(result,((xoico_che_result*)bcore_fork(result_arg_obj)) );
        BLM_TRY(xoico_che_result_a_push_sc(result,")" ))
    }
    
    BLM_TRY(xoico_che_s_trans_typespec_expression(o,source, result, in_typespec, out_typespec ))
    BLM_RETURNV(er_t, 0)
}

er_t xoico_che_s_trans_typespec_test_presence( xoico_che_s* o, x_source* source, xoico_che_result* result, const xoico_typespec_s* in_typespec, xoico_typespec_s* out_typespec )
{
    // xoico_che_trans_typespec.x:352:1
    BLM_INIT_LEVEL(0);
    BLM_TRY(x_source_parse_fa(source,"?" ))
    
    //ignore in case indirection is 0;
    if( in_typespec->indirection > 0 )
    {
        if( in_typespec->indirection != 1 )
        {
            BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Test-Operator requires lvalue with indirection of 1." ))
        }
    
        xoico_che_result* result_arg_obj = ((xoico_che_result*)BLM_LEVEL_A_PUSH(0,xoico_che_result_a_clone(result)));
        xoico_che_result_a_clear(result);
    
        sc_t sc_type = xoico_che_s_nameof(o,in_typespec->type );
        BLM_TRY(xoico_che_result_push_fa(result,"BCORE_PASS_TEST(#<sc_t>,", sc_type ))
        xoico_che_result_a_push_result_d(result,((xoico_che_result*)bcore_fork(result_arg_obj)) );
        BLM_TRY(xoico_che_result_a_push_sc(result,")" ))
    }
    
    BLM_TRY(xoico_che_s_trans_typespec_expression(o,source, result, in_typespec, out_typespec ))
    BLM_RETURNV(er_t, 0)
}

er_t xoico_che_s_trans_typespec_attach( xoico_che_s* o, x_source* source, xoico_che_result* result, const xoico_typespec_s* in_typespec, xoico_typespec_s* out_typespec )
{
    // xoico_che_trans_typespec.x:386:1
    BLM_INIT_LEVEL(0);
    if( in_typespec->indirection != 1 )
    {
        BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Attach-Operator: lvalue with indirection of 1 expected." ))
    }
    
    if( !in_typespec->flag_addressable )
    {
        BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Attach-Operator: Addressable lvalue expected." ))
    }
    
    BLM_TRY(x_source_parse_fa(source,"=<" ))
    
    xoico_che_result* result_arg_obj = ((xoico_che_result*)BLM_LEVEL_A_PUSH(0,xoico_che_result_a_clone(result)));
    xoico_che_result_a_clear(result);
    
    sc_t sc_type = xoico_che_s_nameof(o,in_typespec->type );
    BLM_TRY(xoico_che_result_push_fa(result,"#<sc_t>", sc_type ))
    
    xoico_typespec_s* typespec_rval = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_create()));
    if( xoico_che_s_is_group(o,in_typespec->type ) )
    {
        BLM_TRY(xoico_che_result_a_push_sc(result,"_a" ))
        BLM_TRY(xoico_che_result_a_push_sc(result,"_attach( &(" ))
        xoico_che_result_a_push_result_d(result,((xoico_che_result*)bcore_fork(result_arg_obj)) );
        BLM_TRY(xoico_che_result_push_fa(result,"), (#<sc_t>*)", sc_type ))
        BLM_TRY(xoico_che_result_a_push_sc(result,"(" ))
        BLM_TRY(xoico_che_s_trans_expression(o,source, result, typespec_rval ))
        BLM_TRY(xoico_che_result_a_push_sc(result,"))" ))
    }
    else
    {
        BLM_TRY(xoico_che_result_a_push_sc(result,"_attach( &(" ))
        xoico_che_result_a_push_result_d(result,((xoico_che_result*)bcore_fork(result_arg_obj)) );
        BLM_TRY(xoico_che_result_a_push_sc(result,"), " ))
        BLM_TRY(xoico_che_s_trans_expression(o,source, result, typespec_rval ))
        BLM_TRY(xoico_che_result_a_push_sc(result,")" ))
    }
    
    if( typespec_rval->type )
    {
        if( typespec_rval->indirection != 1 )
        {
            BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Attach operator: rvalue with indirection '1' expected." ))
        }
    
        if( typespec_rval->access_class != TYPEOF_discardable )
        {
            BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Attach operator: Discardable rvalue expected." ))
        }
    }
    
    if( out_typespec ) xoico_typespec_s_copy(out_typespec,in_typespec );
    BLM_RETURNV(er_t, 0)
}

er_t xoico_che_s_trans_typespec_assign( xoico_che_s* o, x_source* source, xoico_che_result* result, const xoico_typespec_s* in_typespec, xoico_typespec_s* out_typespec )
{
    // xoico_che_trans_typespec.x:452:1
    BLM_INIT_LEVEL(0);
    BLM_TRY(x_source_parse_fa(source,"=" ))
    BLM_TRY(xoico_che_result_a_push_sc(result,"=" ))
    
    if( in_typespec->indirection > 0 && xoico_che_s_is_group(o,in_typespec->type ) )
    {BLM_INIT_LEVEL(1);
        xoico_typespec_s* typespec_rval = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_create()));
        xoico_che_result_arr_s* result_rval = ((xoico_che_result_arr_s*)BLM_LEVEL_T_PUSH(1,xoico_che_result_arr_s,xoico_che_result_arr_s_create()));
        BLM_TRY(xoico_che_s_trans_expression(o,source,((xoico_che_result*)( result_rval)), typespec_rval ))
    
        if
        (
            xoico_che_s_is_group(o,typespec_rval->type ) ||
            xoico_che_s_is_stamp(o,typespec_rval->type ) ||
            ( xoico_che_s_is_type(o,typespec_rval->type ) && !xoico_typespec_s_is_ptr_type(typespec_rval) )
        )
        {
            if( typespec_rval->indirection != in_typespec->indirection )
            {
                BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Non declarative assignment: Indirection mismatch." ))
            }
    
            if( typespec_rval->type != in_typespec->type )
            {
                BLM_TRY(xoico_che_s_adapt_expression(o,source, typespec_rval, in_typespec,((const xoico_che_result*)( result_rval)), result ))
            }
            else
            {
                xoico_che_result_a_push_result_d(result,((xoico_che_result*)(((xoico_che_result_arr_s*)bcore_fork(result_rval)) )));
            }
        }
        else
        {
            xoico_che_result_a_push_result_d(result,((xoico_che_result*)(((xoico_che_result_arr_s*)bcore_fork(result_rval)) )));
        }
    BLM_DOWN();}
    else
    {
        BLM_TRY(xoico_che_s_trans_expression(o,source, result, NULL ))
    }
    
    if( out_typespec ) xoico_typespec_s_copy(out_typespec,in_typespec );
    BLM_RETURNV(er_t, 0)
}

er_t xoico_che_s_trans_typespec_expression( xoico_che_s* o, x_source* source, xoico_che_result* result, const xoico_typespec_s* in_typespec, xoico_typespec_s* out_typespec )
{
    // xoico_che_trans_typespec.x:507:1
    BLM_INIT_LEVEL(0);
    if( out_typespec ) out_typespec->type = 0;
    BLM_TRY(xoico_che_s_trans_whitespace(o,source, result ))
    
    u0_t c[ 2 ];
    x_source_inspect_data(source,((x_inst*)(c)), sizeof( c ) );
    
    if( ( c[0] == '.' && c[1] != '.' ) || ( c[0] == '-' && c[1] == '>' ) )
    {
        BLM_TRY(xoico_che_s_trans_typespec_member(o,source, result, in_typespec, out_typespec ))
    }
    else if( c[0] == '[' )
    {
        BLM_TRY(xoico_che_s_trans_typespec_array_subscript(o,source, result, in_typespec, out_typespec ))
    }
    else if( c[0] == '(' )
    {BLM_INIT_LEVEL(2);
        xoico_compiler_element_info_s* info = ((xoico_compiler_element_info_s*)BLM_LEVEL_T_PUSH(2,xoico_compiler_element_info_s,xoico_compiler_element_info_s_create()));
        if( xoico_compiler_s_get_type_member_function_info(o->compiler,in_typespec->type, xoico_che_s_entypeof(o,"_" ), info ) )
        {BLM_INIT_LEVEL(3);
            xoico_typespec_s* typespec_ret = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(3,xoico_typespec_s,xoico_typespec_s_create()));
            xoico_che_result* result_object_expr = ((xoico_che_result*)BLM_LEVEL_A_PUSH(3,xoico_che_result_a_clone(result)));
            xoico_che_result_a_clear(result);
            BLM_TRY(xoico_che_s_trans_function(o,source, false, info->func, result_object_expr, in_typespec, result, typespec_ret ))
            BLM_TRY(xoico_che_s_trans_typespec_expression(o,source, result, typespec_ret, out_typespec ))
        BLM_DOWN();}
        else
        {
            BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Object has no compact function.\n" ))
        }
    BLM_DOWN();}
    // create if not present
    else if( c[0] =='!' && c[1] != '=' )
    {
        BLM_TRY(xoico_che_s_trans_typespec_create(o,source, result, in_typespec, out_typespec ))
    }
    // test for presence
    else if( c[0] == '?' && c[1] == '.' )
    {
        BLM_TRY(xoico_che_s_trans_typespec_test_presence(o,source, result, in_typespec, out_typespec ))
    }
    // attach (detach)
    else if( c[0] == '=' && c[1] == '<' )
    {
        BLM_TRY(xoico_che_s_trans_typespec_attach(o,source, result, in_typespec, out_typespec ))
    }
    // assign
    else if( c[0] == '=' && c[1] != '=' )
    {
        BLM_TRY(xoico_che_s_trans_typespec_assign(o,source, result, in_typespec, out_typespec ))
    }
    // ternary branch operator
    else if( c[0] == '?' )
    {
        BLM_TRY(xoico_che_s_trans_ternary_branch(o,source, result, out_typespec ))
    }
    // scope
    else if( c[0] == '^' && in_typespec->indirection != 0 ) // for indirection 0 '^' is the xor operator
    {BLM_INIT_LEVEL(2);
        xoico_che_result* result_local = ((xoico_che_result*)BLM_LEVEL_A_PUSH(0,xoico_che_result_a_clone(result)));
        xoico_che_result_a_clear(result);
        xoico_typespec_s* typespec_scope = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(2,xoico_typespec_s,xoico_typespec_s_create()));
        BLM_TRY(xoico_che_s_trans_builtin_scope(o,source, result_local, in_typespec, result, typespec_scope ))
        BLM_TRY(xoico_che_s_trans_typespec_expression(o,source, result, typespec_scope, out_typespec ))
    BLM_DOWN();}
    else if( out_typespec )
    {
        xoico_typespec_s_copy(out_typespec,in_typespec );
    }
    
    BLM_RETURNV(er_t, 0)
}

er_t xoico_che_s_trans_function_args( xoico_che_s* o, x_source* source, bl_t function_without_brackets, const xoico_func_s* func, const xoico_che_result* result_object_expr, const xoico_typespec_s* typespec_object, xoico_che_result* result, xoico_typespec_s* typespec_return )
{
    // xoico_che_function.x:32:1
    BLM_INIT_LEVEL(0);
    tp_t transient_return_type = 0;
    
    tp_t object_type = func->obj_type;
    const xoico_signature_s* signature = func->signature;
    typespec_return->type = 0;
    
    if( function_without_brackets )
    {
        BLM_TRY(xoico_che_result_a_push_sc(result,"(" ))
    }
    else
    {
        BLM_TRY(xoico_che_s_trans(o,source, "(", result ))
    }
    
    tp_t cast_to_var = signature->typespec_ret.transient ? signature->typespec_ret.transient->cast_to_var : 0;
    
    tp_t ret_transient_class = signature->typespec_ret.transient ? signature->typespec_ret.transient->class : 0;
    
    const xoico_transient_map_s* transient_map = NULL;
    
    if( signature->arg_o )
    {
        if( !result_object_expr )
        {
            xoico_che_result_arr_s* result_expr   = ((xoico_che_result_arr_s*)BLM_LEVEL_T_PUSH(0,xoico_che_result_arr_s,xoico_che_result_arr_s_create()));
            xoico_typespec_s* typespec_expr = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_create()));
            BLM_TRY(xoico_che_s_trans_expression(o,source,((xoico_che_result*)( result_expr)), typespec_expr ))
            result_object_expr =((const xoico_che_result*)( result_expr));
            typespec_object = typespec_expr;
            if( signature->args.size > 0 ) BLM_TRY(x_source_parse_fa(source," ," ))
        }
    
        transient_map = ( typespec_object ) ? xoico_che_s_get_transient_map(o,typespec_object->type ) : NULL;
    
        xoico_typespec_s* typespec_object_adapted = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_clone(&(signature->arg_o->typespec))));
    
        if( object_type ) BLM_TRY(xoico_typespec_s_relent(typespec_object_adapted,o->host, object_type ))
    
        if( typespec_object->type )
        {
            if
            (
                ret_transient_class &&
                signature->arg_o->typespec.transient &&
                signature->arg_o->typespec.transient->class == ret_transient_class
            )
            {
                transient_return_type = typespec_object->type;
            }
            BLM_TRY(xoico_che_s_adapt_expression(o,source, typespec_object, typespec_object_adapted, result_object_expr, result ))
        }
        else
        {
            xoico_che_result_a_push_result_c(result,result_object_expr );
        }
        if( signature->args.size > 0 ) BLM_TRY(xoico_che_result_a_push_sc(result,"," ))
    
        if( cast_to_var == signature->arg_o->name )
        {
            xoico_typespec_s_copy(typespec_return,typespec_object );
            typespec_return->indirection = typespec_object_adapted->indirection;
        }
    }
    
    if( transient_map && ret_transient_class )
    {
        if( !transient_return_type ) transient_return_type = xoico_transient_map_s_get(transient_map,ret_transient_class );
    }
    
    {const xoico_args_s* __a=&(signature->args );if(__a)for(sz_t __i=0;__i<__a->size;__i++){const xoico_arg_s* arg=__a->data[__i];
    {
        if( xoico_arg_s_is_variadic(arg) ) break;
    
        xoico_che_result_arr_s* result_expr = ((xoico_che_result_arr_s*)BLM_LEVEL_T_PUSH(0,xoico_che_result_arr_s,xoico_che_result_arr_s_create()));
        xoico_typespec_s* typespec_expr = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_create()));
        BLM_TRY(x_source_parse_fa(source," " ))
    
        if( __i > 0 )
        {
            BLM_TRY(x_source_parse_fa(source," ," ))
            BLM_TRY(xoico_che_result_a_push_sc(result,"," ))
        }
    
        BLM_TRY(xoico_che_s_trans_whitespace(o,source,((xoico_che_result*)( result_expr ))))
        if( x_source_parse_bl(source,"#?')'" ) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Function argument '#<sc_t>': No value passed.", xoico_che_s_nameof(o,arg->name ) ))
    
        BLM_TRY(xoico_che_s_trans_expression(o,source,((xoico_che_result*)( result_expr)), typespec_expr ))
    
        if( typespec_expr->type )
        {
            if( arg->typespec.transient )
            {
                if( transient_return_type == 0 && arg->typespec.transient->class == ret_transient_class )
                {
                    transient_return_type = typespec_expr->type;
                }
    
                if( transient_map )
                {
                    tp_t t = xoico_transient_map_s_get(transient_map,ret_transient_class );
                    if( xoico_che_s_is_stamp(o,t ) )
                    {
                        if( typespec_expr->type != t )
                        {
                            BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Function argument '#<sc_t>': Type '#<sc_t>' expected but type '#<sc_t>' passed.", xoico_che_s_nameof(o,arg->name ), xoico_che_s_nameof(o,t ), xoico_che_s_nameof(o,typespec_expr->type ) ))
                        }
                    }
                }
            }
    
            BLM_TRY(xoico_che_s_adapt_expression(o,source, typespec_expr,&( arg->typespec),((const xoico_che_result*)( result_expr)), result ))
        }
        else
        {
            xoico_che_result_a_push_result_d(result,((xoico_che_result*)(((xoico_che_result_arr_s*)bcore_fork(result_expr)) )));
        }
    
        if( cast_to_var == arg->name )
        {
            xoico_typespec_s_copy(typespec_return,typespec_expr );
            typespec_return->indirection = arg->typespec.indirection;
        }
    }
    
    }}if( xoico_args_s_is_variadic(&(signature->args)) )
    {
        while( !x_source_eos(source) && !x_source_parse_bl(source," #=?')'" ) )
        {
            BLM_TRY(x_source_parse_fa(source," ," ))
            BLM_TRY(xoico_che_result_a_push_sc(result,"," ))
            BLM_TRY(xoico_che_s_trans_expression(o,source, result, NULL ))
        }
    }
    
    if( function_without_brackets )
    {
        BLM_TRY(xoico_che_result_a_push_sc(result,")" ))
    }
    else
    {
        BLM_TRY(x_source_parse_fa(source," " ))
        BLM_TRY(xoico_che_s_trans(o,source, ")", result ))
    }
    
    
    if( typespec_return->type == 0 )
    {
        xoico_typespec_s_copy(typespec_return,&(signature->typespec_ret ));
        if( transient_return_type != 0 ) typespec_return->type = transient_return_type;
    }
    BLM_RETURNV(er_t, 0)
}

er_t xoico_che_s_trans_function( xoico_che_s* o, x_source* source, bl_t function_without_brackets, const xoico_func_s* func, const xoico_che_result* result_object_expr, const xoico_typespec_s* typespec_object, xoico_che_result* result, xoico_typespec_s* return_typespec )
{
    // xoico_che_function.x:200:1
    BLM_INIT_LEVEL(0);
    xoico_typespec_s* typespec_ret = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_create()));
    xoico_che_result_arr_s* result_args = ((xoico_che_result_arr_s*)BLM_LEVEL_T_PUSH(0,xoico_che_result_arr_s,xoico_che_result_arr_s_create()));
    
    BLM_TRY(xoico_che_s_trans_function_args(o,source, function_without_brackets, func, result_object_expr, typespec_object,((xoico_che_result*)( result_args)), typespec_ret ))
    
    xoico_che_result_arr_s* result_expression = ((xoico_che_result_arr_s*)BLM_LEVEL_T_PUSH(0,xoico_che_result_arr_s,xoico_che_result_arr_s_create()));
    BLM_TRY(xoico_che_result_arr_s_push_sc(result_expression,xoico_che_s_nameof(o,func->global_name ) ))
    xoico_che_result_arr_s_push_result_d(result_expression,((xoico_che_result*)(((xoico_che_result_arr_s*)bcore_fork(result_args)) )));
    
    /** Casting can become necessary when the functions declared return typespec is less specific than
     *  the expected return type due to transient types.
     */
    if
    (
        typespec_ret->type        != func->signature->typespec_ret.type ||
        typespec_ret->indirection != func->signature->typespec_ret.indirection
    )
    {
        xoico_che_result_a_push_result_d(result,((xoico_che_result*)(xoico_che_result_cast_s_reduce(xoico_che_result_cast_s__(xoico_che_result_cast_s_create(),o, xoico_typespec_s_clone(typespec_ret),((xoico_che_result*)( ((xoico_che_result_arr_s*)bcore_fork(result_expression)) )))) )));
    }
    else
    {
        xoico_che_result_a_push_result_d(result,((xoico_che_result*)(((xoico_che_result_arr_s*)bcore_fork(result_expression)) )));
    }
    
    if( return_typespec ) xoico_typespec_s_copy(return_typespec,typespec_ret );
    
    BLM_RETURNV(er_t, 0)
}

bl_t xoico_che_s_is_builtin_func( const xoico_che_s* o, tp_t tp_identifier )
{
    // xoico_che_builtin.x:21:1
    
    switch( tp_identifier )
    {
        case TYPEOF_cast:
        case TYPEOF_scope:
        case TYPEOF_t_scope:
        case TYPEOF_fork:
        case TYPEOF_try:
            return  true;
    
        default:
            return  false;
    }
}

er_t xoico_che_s_trans_builtin( xoico_che_s* o, tp_t tp_builtin, x_source* source, bl_t source_without_brackets, const xoico_che_result* result_expr, const xoico_typespec_s* typespec_expr, xoico_che_result* result_out, xoico_typespec_s* typespec_out )
{
    // xoico_che_builtin.x:49:1
    
    switch( tp_builtin )
    {
        case TYPEOF_cast :   return  xoico_che_s_trans_builtin_cast(o,source, result_expr, typespec_expr, result_out, typespec_out );
        case TYPEOF_scope:   return  xoico_che_s_trans_builtin_scope(o,source, result_expr, typespec_expr, result_out, typespec_out );
        case TYPEOF_t_scope: return  xoico_che_s_trans_builtin_t_scope(o,source, result_expr, typespec_expr, result_out, typespec_out );
        case TYPEOF_fork:    return  xoico_che_s_trans_builtin_fork(o,source, source_without_brackets, result_expr, typespec_expr, result_out, typespec_out );
        case TYPEOF_try:     return  xoico_che_s_trans_builtin_try(o,source, source_without_brackets, result_expr, typespec_expr, result_out, typespec_out );
        default: return  x_source_parse_error_fa(source,"Internal error: Invalid builtin type '#<sc_t>'", ifnameof( tp_builtin ) );
    }
}

er_t xoico_che_s_trans_builtin_cast( xoico_che_s* o, x_source* source, const xoico_che_result* result_expr, const xoico_typespec_s* typespec_expr, xoico_che_result* result_out, xoico_typespec_s* typespec_out )
{
    // xoico_che_builtin.x:72:1
    BLM_INIT_LEVEL(0);
    if( result_expr ) // member call
    {
        BLM_TRY(x_source_parse_fa(source," ( " ))
        xoico_che_result_a_clear(result_out);
    }
    else // direct call
    {
        BLM_TRY(x_source_parse_fa(source,"cast ( " ))
        xoico_che_result_arr_s* result = ((xoico_che_result_arr_s*)BLM_LEVEL_T_PUSH(0,xoico_che_result_arr_s,xoico_che_result_arr_s_create()));
        xoico_typespec_s* typespec = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_create()));
        BLM_TRY(xoico_che_s_trans_expression(o,source,((xoico_che_result*)( result)), typespec ))
        BLM_TRY(x_source_parse_fa(source," , " ))
        typespec_expr = typespec;
        result_expr =((const xoico_che_result*)( result));
    }
    
    BLM_TRY(xoico_che_result_a_push_sc(result_out,"((" ))
    xoico_typespec_s* typespec_cast = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_create()));
    
    BLM_TRY(xoico_typespec_s_parse(typespec_cast,o->host, source ))
    
    if( typespec_cast->type == ((tp_t)(TYPEOF_type_object)) )
    {
        typespec_cast->type = xoico_host_a_obj_type(o->host);
    }
    else if( typespec_cast->type == ((tp_t)(TYPEOF_type_deduce)) )
    {
        if( !typespec_expr->type )
        {
            BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Cast-syntax: Deduce requested but expression is intractable." ))
        }
        typespec_cast->type = typespec_expr->type;
    }
    
    if( !o->waive_unknown_type && !xoico_che_s_is_type(o,typespec_cast->type ) )
    {
        BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Cast: Unknown type name was used." ))
    }
    
    BLM_TRY(xoico_che_s_push_typespec(o,typespec_cast, result_out ))
    BLM_TRY(xoico_che_result_a_push_sc(result_out,")(" ))
    
    if
    (
        typespec_expr->type &&
        ( typespec_expr->type != ((tp_t)(TYPEOF_vc_t)) ) &&
        ( typespec_expr->type != ((tp_t)(TYPEOF_vd_t)) ) &&
        ( typespec_expr->type != ((tp_t)(TYPEOF_sc_t)) ) &&
        ( typespec_expr->type != ((tp_t)(TYPEOF_sd_t)) ) &&
        ( typespec_expr->type != TYPEOF_void )
    )
    {
        BLM_TRY(xoico_che_s_adapt_expression_indirection(o,source, typespec_expr, typespec_cast->indirection, result_expr, result_out ))
    }
    else
    {
        xoico_che_result_a_push_result_c(result_out,result_expr );
    }
    
    BLM_TRY(x_source_parse_fa(source," )" ))
    BLM_TRY(xoico_che_result_a_push_sc(result_out,"))" ))
    
    if( typespec_out ) xoico_typespec_s_copy(typespec_out,typespec_cast );
    
    if( typespec_cast->indirection > typespec_expr->indirection )
    {
        typespec_out->flag_addressable = false;
    }
    
    BLM_RETURNV(er_t, 0)
}

er_t xoico_che_s_trans_builtin_scope( xoico_che_s* o, x_source* source, const xoico_che_result* result_expr, const xoico_typespec_s* typespec_expr, xoico_che_result* result_out, xoico_typespec_s* typespec_out )
{
    // xoico_che_builtin.x:156:1
    BLM_INIT_LEVEL(0);
    bl_t has_arg = false;
    bl_t closing_bracket = true;
    
    sz_t level = 0;
    
    if( result_expr ) // member call
    {
        if( x_source_parse_bl(source,"#?'^'" ) )
        {
            level = x_source_parse_bl(source,"#?'^'" ) ? 0 : o->block_level;
    
            if( !o->waive_local_scope_operator_creates_implicit_block )
            {
                if( level > 0 && xoico_che_s_stack_block_get_top_unit(o)->statement_wrapped_as_block )
                {
                    BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Scope operator '^' creates an implicit block. Please use an explicit block  '{ ... }' or choose a different scope operator to clarify your intention ." ))
                }
            }
            closing_bracket = false;
        }
        else
        {
            BLM_TRY(x_source_parse_fa(source," ( " ))
            xoico_che_result_a_clear(result_out);
            has_arg = !x_source_parse_bl(source,"#=?')'" );
        }
    }
    else // direct call
    {
        BLM_TRY(x_source_parse_fa(source,"scope ( " ))
        xoico_che_result_arr_s* result = ((xoico_che_result_arr_s*)BLM_LEVEL_T_PUSH(0,xoico_che_result_arr_s,xoico_che_result_arr_s_create()));
        xoico_typespec_s* typespec = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_create()));
        BLM_TRY(xoico_che_s_trans_expression(o,source,((xoico_che_result*)( result)), typespec ))
        typespec_expr = typespec;
        result_expr =((const xoico_che_result*)( result));
        has_arg = x_source_parse_bl(source,"#?','" );
    }
    
    if( typespec_expr->type == 0 ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"scope: Expression not tractable." ))
    if( typespec_expr->access_class != TYPEOF_discardable ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"scope: Expression is not discardable." ))
    xoico_typespec_s* typespec_scope = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_clone(typespec_expr)));
    typespec_scope->access_class = TYPEOF_mutable;
    
    BLM_TRY(xoico_che_result_a_push_sc(result_out,"((" ))
    
    if( has_arg )
    {
        BLM_TRY(x_source_parse_fa(source," " ))
        tp_t tp_identifier = xoico_che_s_get_identifier(o,source, true );
    
        if( xoico_che_s_is_var(o,tp_identifier ) )
        {
            level = xoico_che_stack_var_s_get_level(&(o->stack_var),tp_identifier );
        }
        else if( tp_identifier == ((tp_t)(TYPEOF_scope_local)) )
        {
            level = o->block_level;
        }
        else if( tp_identifier == ((tp_t)(TYPEOF_scope_func)) )
        {
            level = 0;
        }
        else
        {
            BLM_RETURNV(er_t, x_source_parse_error_fa(source,"scope: identifier '#<sc_t>' does not represent a variable.", xoico_che_s_nameof(o,tp_identifier ) ))
        }
    }
    
    if( closing_bracket ) BLM_TRY(x_source_parse_fa(source," )" ))
    
    if( typespec_scope->indirection != 1 ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"scope: Expression's indirection != 1." ))
    if( typespec_scope->flag_scope )       BLM_RETURNV(er_t, x_source_parse_error_fa(source,"scope: Target is already scoped." ))
    
    BLM_TRY(xoico_che_s_push_typespec(o,typespec_scope, result_out ))
    
    if( xoico_che_s_is_group(o,typespec_scope->type ) )
    {
        if( typespec_scope->flag_obliv )
        {
            BLM_RETURNV(er_t, x_source_parse_error_fa(source,"scope: Expression yields an oblivious typespec of a group. Use t_scope to clarify which type is intended." ))
        }
        BLM_TRY(xoico_che_result_push_fa(result_out,")BLM_LEVEL_A_PUSH(#<sz_t>,", level ))
        xoico_che_result_a_push_result_c(result_out,result_expr );
        BLM_TRY(xoico_che_result_a_push_sc(result_out,"))" ))
    }
    else
    {
        BLM_TRY(xoico_che_result_push_fa(result_out,")BLM_LEVEL_T_PUSH(#<sz_t>,#<sc_t>,", level, xoico_che_s_nameof(o,typespec_scope->type ) ))
        xoico_che_result_a_push_result_c(result_out,result_expr );
        BLM_TRY(xoico_che_result_a_push_sc(result_out,"))" ))
    }
    
    o->stack_block.adl.data[ level ]->use_blm = true;
    
    if( typespec_out )
    {
        xoico_typespec_s_copy(typespec_out,typespec_scope );
        typespec_out->flag_scope = true;
    }
    
    BLM_RETURNV(er_t, 0)
}

er_t xoico_che_s_trans_builtin_t_scope( xoico_che_s* o, x_source* source, const xoico_che_result* result_expr, const xoico_typespec_s* typespec_expr, xoico_che_result* result_out, xoico_typespec_s* typespec_out )
{
    // xoico_che_builtin.x:271:1
    BLM_INIT_LEVEL(0);
    bl_t has_arg = false;
    bl_t closing_bracket = true;
    
    sz_t level = 0;
    
    xoico_che_result_arr_s* result_type_expr = ((xoico_che_result_arr_s*)BLM_LEVEL_T_PUSH(0,xoico_che_result_arr_s,xoico_che_result_arr_s_create()));
    
    if( result_expr ) // member call
    {
        BLM_TRY(x_source_parse_fa(source," ( " ))
        xoico_che_result_a_clear(result_out);
        BLM_TRY(xoico_che_s_trans_expression(o,source,((xoico_che_result*)( result_type_expr)), NULL ))
        has_arg = x_source_parse_bl(source,"#?','" );
    }
    else // direct call
    {
        BLM_TRY(x_source_parse_fa(source,"scope ( " ))
        xoico_che_result_arr_s* result = ((xoico_che_result_arr_s*)BLM_LEVEL_T_PUSH(0,xoico_che_result_arr_s,xoico_che_result_arr_s_create()));
        xoico_typespec_s* typespec = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_create()));
        BLM_TRY(xoico_che_s_trans_expression(o,source,((xoico_che_result*)( result)), typespec ))
        typespec_expr = typespec;
        result_expr =((const xoico_che_result*)( result));
    
        BLM_TRY(x_source_parse_fa(source," , " ))
        BLM_TRY(xoico_che_s_trans_expression(o,source,((xoico_che_result*)( result_type_expr)), NULL ))
    
        has_arg = x_source_parse_bl(source,"#?','" );
    }
    
    if( typespec_expr->type == 0 ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"scope: Expression not tractable." ))
    if( typespec_expr->access_class != TYPEOF_discardable ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"scope: Expression is not discardable." ))
    xoico_typespec_s* typespec_scope = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_clone(typespec_expr)));
    typespec_scope->access_class = TYPEOF_mutable;
    
    BLM_TRY(xoico_che_result_a_push_sc(result_out,"((" ))
    
    if( has_arg )
    {
        BLM_TRY(x_source_parse_fa(source," " ))
        tp_t tp_identifier = xoico_che_s_get_identifier(o,source, true );
    
        if( xoico_che_s_is_var(o,tp_identifier ) )
        {
            level = xoico_che_stack_var_s_get_level(&(o->stack_var),tp_identifier );
        }
        else if( tp_identifier == ((tp_t)(TYPEOF_scope_local)) )
        {
            level = o->block_level;
        }
        else if( tp_identifier == ((tp_t)(TYPEOF_scope_func)) )
        {
            level = 0;
        }
        else
        {
            BLM_RETURNV(er_t, x_source_parse_error_fa(source,"scope: identifier '#<sc_t>' does not represent a variable.", xoico_che_s_nameof(o,tp_identifier ) ))
        }
    }
    
    if( closing_bracket ) BLM_TRY(x_source_parse_fa(source," )" ))
    
    if( typespec_scope->indirection != 1 ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"scope: Expression's indirection != 1." ))
    if( typespec_scope->flag_scope )       BLM_RETURNV(er_t, x_source_parse_error_fa(source,"scope: Target is already scoped." ))
    
    BLM_TRY(xoico_che_s_push_typespec(o,typespec_scope, result_out ))
    
    BLM_TRY(xoico_che_result_push_fa(result_out,")BLM_LEVEL_TV_PUSH(#<sz_t>,", level ))
    xoico_che_result_a_push_result_c(result_out,((const xoico_che_result*)(result_type_expr )));
    BLM_TRY(xoico_che_result_push_fa(result_out,"," ))
    xoico_che_result_a_push_result_c(result_out,result_expr );
    BLM_TRY(xoico_che_result_a_push_sc(result_out,"))" ))
    
    o->stack_block.adl.data[ level ]->use_blm = true;
    
    if( typespec_out )
    {
        xoico_typespec_s_copy(typespec_out,typespec_scope );
        typespec_out->flag_scope = true;
    }
    
    BLM_RETURNV(er_t, 0)
}

er_t xoico_che_s_trans_builtin_fork( xoico_che_s* o, x_source* source, bl_t source_without_brackets, const xoico_che_result* result_expr, const xoico_typespec_s* typespec_expr, xoico_che_result* result_out, xoico_typespec_s* typespec_out )
{
    // xoico_che_builtin.x:367:1
    BLM_INIT_LEVEL(0);
    if( result_expr ) // member call
    {
        if( source_without_brackets )
        {
            BLM_TRY(x_source_parse_fa(source," " ))
        }
        else
        {
            BLM_TRY(x_source_parse_fa(source," ( " ))
        }
        xoico_che_result_a_clear(result_out);
    }
    else // direct call
    {BLM_INIT_LEVEL(1);
        BLM_TRY(x_source_parse_fa(source,"fork ( " ))
        xoico_che_result_arr_s* result = ((xoico_che_result_arr_s*)BLM_LEVEL_T_PUSH(1,xoico_che_result_arr_s,xoico_che_result_arr_s_create()));
        xoico_typespec_s* typespec = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_create()));
        BLM_TRY(xoico_che_s_trans_expression(o,source,((xoico_che_result*)( result)), typespec ))
        typespec_expr = typespec;
        result_expr =((const xoico_che_result*)( result));
    BLM_DOWN();}
    
    xoico_typespec_s* typespec_fork = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_clone(typespec_expr)));
    typespec_fork->access_class = TYPEOF_discardable;
    
    BLM_TRY(xoico_che_result_a_push_sc(result_out,"((" ))
    
    if( source_without_brackets )
    {
        BLM_TRY(x_source_parse_fa(source," " ))
    }
    else
    {
        BLM_TRY(x_source_parse_fa(source," )" ))
    }
    
    if( typespec_fork->type        == 0 ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Operator 'fork': Expression not tractable." ))
    if( typespec_fork->indirection != 1 ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Operator 'fork': Expression's indirection != 1." ))
    
    BLM_TRY(xoico_che_s_push_typespec(o,typespec_fork, result_out ))
    BLM_TRY(xoico_che_result_a_push_sc(result_out,")bcore_fork(" ))
    xoico_che_result_a_push_result_c(result_out,result_expr );
    BLM_TRY(xoico_che_result_a_push_sc(result_out,"))" ))
    
    if( typespec_out ) xoico_typespec_s_copy(typespec_out,typespec_fork );
    
    BLM_RETURNV(er_t, 0)
}

er_t xoico_che_s_trans_builtin_try( xoico_che_s* o, x_source* source, bl_t source_without_brackets, const xoico_che_result* result_expr, const xoico_typespec_s* typespec_expr, xoico_che_result* result_out, xoico_typespec_s* typespec_out )
{
    // xoico_che_builtin.x:429:1
    BLM_INIT_LEVEL(0);
    if( typespec_out ) xoico_typespec_s_reset(typespec_out);
    
    if( result_expr ) // member call
    {
        if( source_without_brackets )
        {
            BLM_TRY(x_source_parse_fa(source," " ))
        }
        else
        {
            BLM_TRY(x_source_parse_fa(source," ( " ))
        }
        xoico_che_result_a_clear(result_out);
    }
    else // direct call
    {
        BLM_TRY(x_source_parse_fa(source,"try " ))
    
        if( x_source_parse_bl(source,"#=?'{'" ) ) // try block
        {
            o->try_block_level++;
            BLM_TRY(xoico_che_s_trans_block(o,source, result_out, false ))
            o->try_block_level--;
            BLM_RETURNV(er_t, 0)
        }
    
        BLM_TRY(x_source_parse_fa(source,"( " ))
        xoico_che_result_arr_s* result = ((xoico_che_result_arr_s*)BLM_LEVEL_T_PUSH(0,xoico_che_result_arr_s,xoico_che_result_arr_s_create()));
        xoico_typespec_s* typespec = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_create()));
        BLM_TRY(xoico_che_s_trans_expression(o,source,((xoico_che_result*)( result)), typespec ))
        typespec_expr = typespec;
        result_expr =((const xoico_che_result*)( result));
    }
    
    const xoico_typespec_s* typespec_try = typespec_expr;
    
    if( source_without_brackets )
    {
        BLM_TRY(x_source_parse_fa(source," ;" ))
    }
    else
    {
        BLM_TRY(x_source_parse_fa(source," ) ;" ))
    }
    
    
    if( typespec_try->type != 0 )
    {
        if( typespec_try->type != ((tp_t)(TYPEOF_er_t)) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Operator 'try': Expression must yield er_t." ))
        if( typespec_try->indirection != 0    ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Operator 'try': Expression's indirection != 0." ))
    }
    else
    {
        // return source.parse_error_fa( "Operator 'try': Expression not tractable." );
    }
    
    if( xoico_che_s_returns_er_t(o) )
    {
        BLM_TRY(xoico_che_result_a_push_sc(result_out,"BLM_TRY(" ))
    }
    else
    {
        BLM_TRY(xoico_che_result_a_push_sc(result_out,"BLM_TRY_EXIT(" ))
    }
    xoico_che_result_a_push_result_c(result_out,result_expr );
    BLM_TRY(xoico_che_result_a_push_sc(result_out,")" ))
    
    BLM_RETURNV(er_t, 0)
}

bl_t xoico_che_s_is_control_name( const xoico_che_s* o, tp_t tp_identifier )
{
    // xoico_che_control.x:21:1
    
    switch( tp_identifier )
    {
        case TYPEOF_for:
        case TYPEOF_foreach:
        case TYPEOF_if:
        case TYPEOF_else:
        case TYPEOF_break:
        case TYPEOF_while:
        case TYPEOF_do:
        case TYPEOF_switch:
        case TYPEOF_case:
        case TYPEOF_default:
        case TYPEOF_return:
        case TYPEOF_continue:
        case TYPEOF_goto:
            return  true;
    
        default:
            return  false;
    }
}

er_t xoico_che_s_trans_control( xoico_che_s* o, tp_t tp_control, x_source* source, xoico_che_result* result )
{
    // xoico_che_control.x:47:1
    
    switch( tp_control )
    {
        case TYPEOF_for:     return  xoico_che_s_trans_control_for(o,source, result );
        case TYPEOF_foreach: return  xoico_che_s_trans_control_foreach(o,source, result );
        case TYPEOF_if:      return  xoico_che_s_trans_control_if(o,source, result );
        case TYPEOF_else:    return  xoico_che_s_trans_control_else(o,source, result );
        case TYPEOF_break:   return  xoico_che_s_trans_control_break(o,source, result );
        case TYPEOF_while:   return  xoico_che_s_trans_control_while(o,source, result );
        case TYPEOF_do:      return  xoico_che_s_trans_control_do(o,source, result );
        case TYPEOF_switch:  return  xoico_che_s_trans_control_switch(o,source, result );
        case TYPEOF_case:    return  xoico_che_s_trans_control_case(o,source, result );
        case TYPEOF_default: return  xoico_che_s_trans_control_default(o,source, result );
        case TYPEOF_return:     return  xoico_che_s_trans_control_return(o,tp_control, source, result );
        case TYPEOF_completion: return  xoico_che_s_trans_control_return(o,tp_control, source, result );
    
        // unsupported controls
        case TYPEOF_goto:
        case TYPEOF_continue:
            return  x_source_parse_error_fa(source,"Control statement '#<sc_t>' is not supported.", ifnameof( tp_control ) );
    
        default:
            return  x_source_parse_error_fa(source,"Internal error: Invalid control name '#<sc_t>'", ifnameof( tp_control ) );
    }
}

er_t xoico_che_s_trans_control_for( xoico_che_s* o, x_source* source, xoico_che_result* result )
{
    // xoico_che_control.x:76:1
    
    xoico_che_s_inc_block(o);
    xoico_che_s_stack_block_get_top_unit(o)->break_ledge = true;
    BLM_TRY(xoico_che_s_trans(o,source, "for", result ))
    BLM_TRY(xoico_che_s_trans_whitespace(o,source, result ))
    BLM_TRY(xoico_che_s_trans(o,source, "(", result ))
    if( !x_source_parse_bl(source," #=?';'" ) ) BLM_TRY(xoico_che_s_trans_statement(o,source, result )) // def
    BLM_TRY(xoico_che_s_trans(o,source, ";", result ))
    BLM_TRY(xoico_che_s_trans_expression(o,source, result, NULL )) // cond
    BLM_TRY(xoico_che_s_trans(o,source, ";", result ))
    BLM_TRY(xoico_che_s_trans_expression(o,source, result, NULL )) // update
    BLM_TRY(xoico_che_s_trans_whitespace(o,source, result ))
    BLM_TRY(xoico_che_s_trans(o,source, ")", result ))
    BLM_TRY(xoico_che_s_trans_whitespace(o,source, result ))
    if( x_source_parse_bl(source,"#=?'{'" ) )
    {
        BLM_TRY(xoico_che_s_trans_block(o,source, result, false ))
    }
    else
    {
        BLM_TRY(xoico_che_s_trans_statement_as_block(o,source, result, false ))
    }
    xoico_che_s_dec_block(o);
    return  0;
}

er_t xoico_che_s_trans_control_foreach( xoico_che_s* o, x_source* source, xoico_che_result* result )
{
    // xoico_che_control.x:109:1
    BLM_INIT_LEVEL(0);
    xoico_che_s_inc_block(o);
    BLM_TRY(x_source_parse_fa(source,"foreach ( " ))
    
    xoico_typespec_s* typespec_var = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_create() ));
    BLM_TRY(xoico_che_s_take_typespec(o,source, typespec_var, true ))
    
    tp_t tp_var_name = 0;
    
    if( x_source_parse_bl(source,"#?(([0]>='A'&&[0]<='Z')||([0]>='a'&&[0]<='z')||[0]=='_')" ) )
    {
        tp_var_name = xoico_che_s_get_identifier(o,source, true );
    }
    else
    {
        BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Variable name expected." ))
    }
    
    BLM_TRY(x_source_parse_fa(source," in " ))
    
    xoico_typespec_s* typespec_arr_expr = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_create()));
    xoico_che_result_arr_s* result_arr_expr = ((xoico_che_result_arr_s*)BLM_LEVEL_T_PUSH(0,xoico_che_result_arr_s,xoico_che_result_arr_s_create()));
    BLM_TRY(xoico_che_s_trans_expression(o,source,((xoico_che_result*)( result_arr_expr)), typespec_arr_expr ))
    
    if( !typespec_arr_expr->type )
    {
        BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Array expression not tractable." ))
    }
    
    xoico_compiler_element_info_s* array_element_info = ((xoico_compiler_element_info_s*)BLM_LEVEL_T_PUSH(0,xoico_compiler_element_info_s,xoico_compiler_element_info_s_create()));
    
    if( !xoico_compiler_s_get_type_array_element_info(o->compiler,typespec_arr_expr->type, array_element_info ) )
    {
        BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Expression does not evaluate to an array." ))
    }
    
    const xoico_typespec_s* typespec_array_element =&( array_element_info->type_info.typespec);
    xoico_typespec_s* typespec_access_element = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_create()));
    
    xoico_che_result* result_element_expr = ((xoico_che_result*)BLM_LEVEL_A_PUSH(0,xoico_che_result_create_from_sc("__a->data[__i]" )));
    
    /// sub-element
    if( x_source_parse_bl(source," #=?'..'" ) )
    {
        BLM_TRY(x_source_parse_fa(source,"." ))
        BLM_TRY(xoico_che_s_trans_typespec_expression(o,source, result_element_expr, typespec_array_element, typespec_access_element ))
    }
    else
    {
        xoico_typespec_s_copy(typespec_access_element,typespec_array_element );
    }
    
    if( typespec_var->type == ((tp_t)(TYPEOF_type_deduce)) ) typespec_var->type = typespec_access_element->type;
    
    xoico_che_result* condition_expr = NULL;
    
    xoico_typespec_s* typespec_arr = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_clone(typespec_arr_expr) ));
    typespec_arr->indirection = 1;
    typespec_arr->access_class = TYPEOF_const;
    
    xoico_typespec_s* typespec_idx = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_create() ));
    typespec_idx->type = ((tp_t)(TYPEOF_sz_t));
    
    xoico_che_s_push_typedecl(o,typespec_var, tp_var_name );
    xoico_che_s_push_typedecl(o,typespec_arr, xoico_che_s_entypeof(o,"__a" ) );
    xoico_che_s_push_typedecl(o,typespec_idx, xoico_che_s_entypeof(o,"__i" ) );
    
    /// condition
    if( x_source_parse_bl(source," #=?';'" ) )
    {
        BLM_TRY(x_source_parse_fa(source," ; " ))
        condition_expr =((xoico_che_result*)( ((xoico_che_result_arr_s*)BLM_LEVEL_T_PUSH(0,xoico_che_result_arr_s,xoico_che_result_arr_s_create()))));
        BLM_TRY(xoico_che_s_trans_expression(o,source, condition_expr, NULL ))
    }
    
    BLM_TRY(x_source_parse_fa(source," )" ))
    
    xoico_che_result_arr_s* result_statement = ((xoico_che_result_arr_s*)BLM_LEVEL_T_PUSH(0,xoico_che_result_arr_s,xoico_che_result_arr_s_create()));
    xoico_che_s_inc_block(o);
    xoico_che_s_stack_block_get_top_unit(o)->break_ledge = true;
    if( x_source_parse_bl(source,"#=?'{'" ) )
    {
        BLM_TRY(xoico_che_s_trans_block(o,source,((xoico_che_result*)( result_statement)), false ))
    }
    else
    {
        BLM_TRY(xoico_che_s_trans_statement_as_block(o,source,((xoico_che_result*)( result_statement)), false ))
    }
    xoico_che_s_dec_block(o);
    
    BLM_TRY(xoico_che_result_push_fa(result,"{" ))
    xoico_che_result_blm_init_s* blm_init = xoico_che_result_blm_init_s__(xoico_che_result_blm_init_s_create(),o->block_level );
    xoico_che_result_a_push_result_d(result,((xoico_che_result*)(blm_init )));
    
    BLM_TRY(xoico_che_s_push_typespec(o,typespec_arr, result ))
    
    BLM_TRY(xoico_che_result_push_fa(result," __a=" ))
    BLM_TRY(xoico_che_s_adapt_expression(o,source, typespec_arr_expr, typespec_arr,((const xoico_che_result*)( result_arr_expr)), result ))
    BLM_TRY(xoico_che_result_push_fa(result,";" ))
    
    BLM_TRY(xoico_che_result_push_fa(result,"if(__a)for(sz_t __i=0;__i<__a->size;__i++){" ))
    BLM_TRY(xoico_che_s_push_typespec(o,typespec_var, result ))
    BLM_TRY(xoico_che_result_push_fa(result," #<sc_t>=", xoico_che_s_nameof( o,tp_var_name ) ))
    
    BLM_TRY(xoico_che_s_adapt_expression(o,source, typespec_access_element, typespec_var, result_element_expr, result ))
    BLM_TRY(xoico_che_result_push_fa(result,";" ))
    
    if( condition_expr )
    {
        BLM_TRY(xoico_che_result_a_push_sc(result,"if(" ))
        xoico_che_result_a_push_result_d(result,((xoico_che_result*)bcore_fork(condition_expr)) );
        BLM_TRY(xoico_che_result_a_push_sc(result,"){" ))
        xoico_che_result_a_push_result_d(result,((xoico_che_result*)(((xoico_che_result_arr_s*)bcore_fork(result_statement)) )));
        BLM_TRY(xoico_che_result_a_push_sc(result,"}" ))
    }
    else
    {
        xoico_che_result_a_push_result_d(result,((xoico_che_result*)(((xoico_che_result_arr_s*)bcore_fork(result_statement)) )));
    }
    
    BLM_TRY(xoico_che_result_push_fa(result,"}" ))
    
    xoico_che_result_blm_down_s* blm_down = xoico_che_result_blm_down_s_create();
    
    if( !xoico_che_s_stack_block_get_top_unit(o)->use_blm )
    {
        xoico_che_result_blm_init_s_deactivate(blm_init);
        xoico_che_result_blm_down_s_deactivate(blm_down);
    }
    
    xoico_che_result_a_push_result_d(result,((xoico_che_result*)(blm_down )));
    BLM_TRY(xoico_che_result_push_fa(result,"}" ))
    xoico_che_s_dec_block(o);
    BLM_RETURNV(er_t, 0)
}

er_t xoico_che_s_trans_control_if( xoico_che_s* o, x_source* source, xoico_che_result* result )
{
    // xoico_che_control.x:248:1
    
    BLM_TRY(xoico_che_s_trans(o,source, "if", result ))
    BLM_TRY(xoico_che_s_trans_whitespace(o,source, result ))
    BLM_TRY(xoico_che_s_trans(o,source, "(", result ))
    BLM_TRY(xoico_che_s_trans_expression(o,source, result, NULL )) // def
    BLM_TRY(xoico_che_s_trans(o,source, ")", result ))
    BLM_TRY(xoico_che_s_trans_whitespace(o,source, result ))
    if( x_source_parse_bl(source,"#=?'{'" ) )
    {
        BLM_TRY(xoico_che_s_trans_block(o,source, result, false ))
    }
    else
    {
        BLM_TRY(xoico_che_s_trans_statement_as_block(o,source, result, false ))
    }
    return  0;
}

er_t xoico_che_s_trans_control_while( xoico_che_s* o, x_source* source, xoico_che_result* result )
{
    // xoico_che_control.x:269:1
    
    BLM_TRY(xoico_che_s_trans(o,source, "while", result ))
    BLM_TRY(xoico_che_s_trans_whitespace(o,source, result ))
    BLM_TRY(xoico_che_s_trans(o,source, "(", result ))
    BLM_TRY(xoico_che_s_trans_expression(o,source, result, NULL )) // def
    BLM_TRY(xoico_che_s_trans(o,source, ")", result ))
    BLM_TRY(xoico_che_s_trans_whitespace(o,source, result ))
    if( x_source_parse_bl(source,"#=?'{'" ) )
    {
        BLM_TRY(xoico_che_s_trans_block(o,source, result, true ))
    }
    else
    {
        BLM_TRY(xoico_che_s_trans_statement_as_block(o,source, result, true ))
    }
    return  0;
}

er_t xoico_che_s_trans_control_do( xoico_che_s* o, x_source* source, xoico_che_result* result )
{
    // xoico_che_control.x:290:1
    
    BLM_TRY(xoico_che_s_trans(o,source, "do", result ))
    BLM_TRY(xoico_che_s_trans_whitespace(o,source, result ))
    if( x_source_parse_bl(source,"#=?'{'" ) )
    {
        BLM_TRY(xoico_che_s_trans_block(o,source, result, true ))
    }
    else
    {
        BLM_TRY(xoico_che_s_trans_statement_as_block(o,source, result, true ))
    }
    BLM_TRY(xoico_che_s_trans(o,source, "while", result ))
    BLM_TRY(xoico_che_s_trans_whitespace(o,source, result ))
    BLM_TRY(xoico_che_s_trans(o,source, "(", result ))
    BLM_TRY(xoico_che_s_trans_expression(o,source, result, NULL )) // def
    BLM_TRY(xoico_che_s_trans(o,source, ")", result ))
    BLM_TRY(xoico_che_s_trans_whitespace(o,source, result ))
    BLM_TRY(xoico_che_s_trans(o,source, ";", result ))
    return  0;
}

er_t xoico_che_s_trans_control_else( xoico_che_s* o, x_source* source, xoico_che_result* result )
{
    // xoico_che_control.x:314:1
    
    BLM_TRY(xoico_che_s_trans(o,source, "else", result ))
    BLM_TRY(xoico_che_s_trans_whitespace(o,source, result ))
    if( x_source_parse_bl(source,"#=?'{'" ) )
    {
        BLM_TRY(xoico_che_s_trans_block(o,source, result, false ))
    }
    else
    {
        BLM_TRY(xoico_che_s_trans_statement_as_block(o,source, result, false ))
    }
    return  0;
}

er_t xoico_che_s_trans_control_switch( xoico_che_s* o, x_source* source, xoico_che_result* result )
{
    // xoico_che_control.x:331:1
    
    BLM_TRY(xoico_che_s_trans(o,source, "switch", result ))
    BLM_TRY(xoico_che_s_trans_whitespace(o,source, result ))
    BLM_TRY(xoico_che_s_trans(o,source, "(", result ))
    BLM_TRY(xoico_che_s_trans_expression(o,source, result, NULL )) // def
    BLM_TRY(xoico_che_s_trans(o,source, ")", result ))
    BLM_TRY(xoico_che_s_trans_whitespace(o,source, result ))
    BLM_TRY(xoico_che_s_trans_block(o,source, result, true ))
    return  0;
}

er_t xoico_che_s_trans_control_case( xoico_che_s* o, x_source* source, xoico_che_result* result )
{
    // xoico_che_control.x:345:1
    
    BLM_TRY(xoico_che_s_trans(o,source, "case", result ))
    BLM_TRY(xoico_che_s_trans_expression(o,source, result, NULL ))
    BLM_TRY(xoico_che_s_trans_whitespace(o,source, result ))
    BLM_TRY(xoico_che_s_trans(o,source, ":", result ))
    BLM_TRY(xoico_che_s_trans_whitespace(o,source, result ))
    BLM_TRY(xoico_che_s_trans_statement_as_block(o,source, result, false ))
    return  0;
}

er_t xoico_che_s_trans_control_default( xoico_che_s* o, x_source* source, xoico_che_result* result )
{
    // xoico_che_control.x:358:1
    
    BLM_TRY(xoico_che_s_trans(o,source, "default", result ))
    BLM_TRY(xoico_che_s_trans_whitespace(o,source, result ))
    BLM_TRY(xoico_che_s_trans(o,source, ":", result ))
    BLM_TRY(xoico_che_s_trans_whitespace(o,source, result ))
    BLM_TRY(xoico_che_s_trans_statement_as_block(o,source, result, false ))
    return  0;
}

er_t xoico_che_s_trans_control_break( xoico_che_s* o, x_source* source, xoico_che_result* result )
{
    // xoico_che_control.x:370:1
    
    BLM_TRY(x_source_parse_fa(source,"break ;" ))
    
    sz_t ledge_level = -1;
    bl_t use_blm = false;
    
    for(sz_t i = o->stack_block.adl.size - 1; i >= 0; i-- )
    {
        const xoico_che_stack_block_unit_s* unit = o->stack_block.adl.data[ i ];
        use_blm = use_blm || unit->use_blm;
        if( unit->break_ledge )
        {
            ledge_level = unit->level;
            break;
        }
    }
    
    if( ledge_level == -1 ) return  x_source_parse_error_fa(source,"'break' has no ledge." );
    
    xoico_che_result_a_push_result_d(result,((xoico_che_result*)(xoico_che_result_break_s__(xoico_che_result_break_s_create(),ledge_level ) )));
    
    return  0;
}

er_t xoico_che_s_trans_control_return( xoico_che_s* o, tp_t tp_control, x_source* source, xoico_che_result* result )
{
    // xoico_che_control.x:397:1
    BLM_INIT_LEVEL(0);
    o->has_completion = true;
    
    if( tp_control == TYPEOF_return ) BLM_TRY(x_source_parse_fa(source,"return" ))
    if( tp_control == TYPEOF_completion ) BLM_TRY(x_source_parse_fa(source,"=" ))
    
    xoico_che_result_arr_s* result_expr = ((xoico_che_result_arr_s*)BLM_LEVEL_T_PUSH(0,xoico_che_result_arr_s,xoico_che_result_arr_s_create()));
    
    xoico_typespec_s* typespec_expr = ((xoico_typespec_s*)BLM_LEVEL_T_PUSH(0,xoico_typespec_s,xoico_typespec_s_create() ));
    const xoico_typespec_s* typespec_ret =&( o->signature->typespec_ret);
    
    BLM_TRY(xoico_che_s_trans_expression(o,source,((xoico_che_result*)( result_expr)), typespec_expr ))
    
    if( typespec_expr->type )
    {
        if( !xoico_che_s_returns_a_value(o) )
        {
            BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Function does not return a value." ))
        }
    }
    
    BLM_TRY(xoico_che_s_trans_whitespace(o,source,((xoico_che_result*)( result_expr ))))
    
    if( !x_source_parse_bl(source,"#?';'" ) ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Missing ';' after completion statement." ))
    
    xoico_che_result_arr_s* result_expr_adapted = ((xoico_che_result_arr_s*)BLM_LEVEL_T_PUSH(0,xoico_che_result_arr_s,xoico_che_result_arr_s_create()));
    if( xoico_che_s_returns_a_value(o) && typespec_expr->type )
    {
        if( typespec_expr->access_class != typespec_ret->access_class )
        {
            if( typespec_expr->access_class == TYPEOF_discardable )
            {
                BLM_RETURNV(er_t, x_source_parse_error_fa(source,"return: Conversion discards typespec 'discardable'." ))
            }
    
            if( typespec_ret->access_class == TYPEOF_discardable )
            {
                BLM_RETURNV(er_t, x_source_parse_error_fa(source,"return: Conversion requires typespec 'discardable'." ))
            }
        }
    
        BLM_TRY(xoico_che_s_adapt_expression(o,source, typespec_expr, typespec_ret,((const xoico_che_result*)( result_expr)),((xoico_che_result*)( result_expr_adapted ))))
    }
    else
    {
        xoico_che_result_arr_s_push_result_d(result_expr_adapted,((xoico_che_result*)(((xoico_che_result_arr_s*)bcore_fork(result_expr)) )));
    }
    
    xoico_che_result_a_push_result_d(result,((xoico_che_result*)(xoico_che_result_return_s__(xoico_che_result_return_s_create(),o,((xoico_che_result*)( ((xoico_che_result_arr_s*)bcore_fork(result_expr_adapted)) ))) )));
    
    BLM_RETURNV(er_t, 0)
}

XOILA_DEFINE_SPECT( xoico_cengine, xoico_che )
"{"
    "bcore_spect_header_s header;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: xoico_che_result; embeds: xoico_che_result.x

BCORE_DEFINE_OBJECT_INST_P( xoico_che_result_whitespace_s )
"aware xoico_che_result"
"{"
    "st_s => st;"
    "func ^:to_sink;"
"}";

xoico_che_result_whitespace_s* xoico_che_result_whitespace_s__( xoico_che_result_whitespace_s* o, st_s* st )
{
    // :2:1
    
    st_s_attach( &(o->st ),  st);
    return  o;
}

BCORE_DEFINE_OBJECT_INST_P( xoico_che_result_plain_s )
"aware xoico_che_result"
"{"
    "st_s st;"
    "func ^:clear;"
    "func ^:push_char;"
    "func ^:push_sc;"
    "func ^:push_st;"
    "func ^:to_sink;"
    "func ^:create_st;"
"}";

xoico_che_result_plain_s* xoico_che_result_plain_s__( xoico_che_result_plain_s* o, const st_s* st )
{
    // :2:1
    
    st_s_copy(&(o->st),st );
    return  o;
}

BCORE_DEFINE_OBJECT_INST_P( xoico_che_result_adl_s )
"aware x_array"
"{"
    "aware xoico_che_result -> [];"
"}";

BCORE_DEFINE_OBJECT_INST_P( xoico_che_result_arr_s )
"aware xoico_che_result"
"{"
    "xoico_che_result_adl_s adl;"
    "bl_t active = true;"
    "func ^:clear;"
    "func ^:activate;"
    "func ^:deactivate;"
    "func ^:push_char;"
    "func ^:push_sc;"
    "func ^:push_st;"
    "func ^:push_result_d;"
    "func ^:push_result_c;"
    "func ^:to_sink;"
    "func ^:set_parent_block;"
    "func ^:get_cast;"
"}";

er_t xoico_che_result_arr_s_to_sink( const xoico_che_result_arr_s* o, x_sink* sink )
{
    // xoico_che_result.x:89:5
    
    if( o->active ) {const xoico_che_result_adl_s* __a=&(o->adl );if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_che_result* e=__a->data[__i]; BLM_TRY(xoico_che_result_a_to_sink(e,sink ))
    }}return 0;
}

void xoico_che_result_arr_s_set_parent_block( xoico_che_result_arr_s* o, xoico_che_result_block_s* parent )
{
    // xoico_che_result.x:95:5
    
    {const xoico_che_result_adl_s* __a=&(o->adl );if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_che_result* e=__a->data[__i]; xoico_che_result_a_set_parent_block(e,parent );}}
}

bl_t xoico_che_result_arr_s_get_cast( xoico_che_result_arr_s* o, xoico_che_result_cast_s** pp_cast )
{
    // xoico_che_result.x:100:5
    
    {const xoico_che_result_adl_s* __a=&(o->adl );if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_che_result* e=__a->data[__i]; if( e->_ != ((tp_t)(TYPEOF_xoico_che_result_whitespace_s)) ) return  xoico_che_result_a_get_cast(e,pp_cast );
    }}if( pp_cast ) (*(pp_cast)) = NULL;
    return  false;
}

BCORE_DEFINE_OBJECT_INST_P( xoico_che_result_block_s )
"aware xoico_che_result"
"{"
    "sz_t level;"
    "bl_t is_using_blm;"
    "xoico_che_result_arr_s arr;"
    "bl_t is_root = false;"
    "hidden xoico_che_result_block_s* parent;"
    "func ^:clear;"
    "func ^:push_char;"
    "func ^:push_sc;"
    "func ^:push_st;"
    "func ^:push_result_d;"
    "func ^:push_result_c;"
    "func ^:to_sink;"
    "func ^:set_parent_block;"
"}";

xoico_che_result* xoico_che_result_block_s_push_result_d( xoico_che_result_block_s* o, xoico_che_result* result )
{
    // xoico_che_result.x:120:5
    
    xoico_che_result* result_pushed = xoico_che_result_arr_s_push_result_d(&(o->arr),result );
    xoico_che_result_a_set_parent_block(result_pushed,o );
    return result_pushed;
}

xoico_che_result* xoico_che_result_block_s_push_result_c( xoico_che_result_block_s* o, const xoico_che_result* result )
{
    // xoico_che_result.x:127:5
    
    xoico_che_result* result_pushed = xoico_che_result_arr_s_push_result_c(&(o->arr),result );
    xoico_che_result_a_set_parent_block(result_pushed,o );
    return result_pushed;
}

bl_t xoico_che_result_block_s_is_using_blm_until_level( const xoico_che_result_block_s* o, sz_t level )
{
    // xoico_che_result.x:137:5
    
    if( level > o->level ) return  false;
    if( o->is_using_blm )  return  true;
    if( o->is_root )       return  false;
    
    ASSERT( o->parent );
    return  xoico_che_result_block_s_is_using_blm_until_level(o->parent,level );
}

xoico_che_result_block_s* xoico_che_result_block_s__( xoico_che_result_block_s* o, sz_t level, bl_t is_using_blm )
{
    // :2:1
    
    o->level = level;
    o->is_using_blm = is_using_blm;
    return  o;
}

BCORE_DEFINE_OBJECT_INST_P( xoico_che_result_blm_init_s )
"aware xoico_che_result"
"{"
    "sz_t level;"
    "bl_t active = true;"
    "func ^:to_sink;"
    "func ^:activate;"
    "func ^:deactivate;"
"}";

xoico_che_result_blm_init_s* xoico_che_result_blm_init_s__( xoico_che_result_blm_init_s* o, sz_t level )
{
    // :2:1
    
    o->level = level;
    return  o;
}

BCORE_DEFINE_OBJECT_INST_P( xoico_che_result_blm_down_s )
"aware xoico_che_result"
"{"
    "bl_t active = true;"
    "func ^:to_sink;"
    "func ^:activate;"
    "func ^:deactivate;"
"}";

BCORE_DEFINE_OBJECT_INST_P( xoico_che_result_cast_s )
"aware xoico_che_result"
"{"
    "hidden xoico_che_s * che;"
    "xoico_typespec_s => target_typespec;"
    "aware xoico_che_result => expression;"
    "bl_t active = true;"
    "func ^:activate;"
    "func ^:deactivate;"
    "func ^:get_cast;"
    "func ^:set_parent_block;"
    "func ^:to_sink;"
"}";

bl_t xoico_che_result_cast_s_get_cast( xoico_che_result_cast_s* o, xoico_che_result_cast_s** pp_cast )
{
    // xoico_che_result.x:176:5
    
    if( o->active )
    {
        if( pp_cast ) (*(pp_cast)) = o;
        return true;
    }
    else
    {
        return xoico_che_result_a_get_cast(o->expression,pp_cast );
    }
}

bl_t xoico_che_result_cast_s_overrides( const xoico_che_result_cast_s* o, const xoico_che_result_cast_s* a )
{
    // xoico_che_result.x:189:5
    
    const xoico_typespec_s* to = o->target_typespec;
    const xoico_typespec_s* ta = a->target_typespec;
    
    /* Currently we restrict to group and stamp pointers.
     * We might be less restrictive in future.
     */
    if( !xoico_che_s_is_group(o->che,to->type ) && !xoico_che_s_is_stamp(o->che,to->type ) ) return false;
    if( !xoico_che_s_is_group(o->che,ta->type ) && !xoico_che_s_is_stamp(o->che,ta->type ) ) return false;
    if( to->indirection != 1 ) return false;
    if( ta->indirection != 1 ) return false;
    
    return true;
}

xoico_che_result_cast_s* xoico_che_result_cast_s_reduce( xoico_che_result_cast_s* o )
{
    // xoico_che_result.x:206:5
    
    xoico_che_result_cast_s* prev_cast = NULL;
    if( xoico_che_result_a_get_cast(o->expression,&(prev_cast )) )
    {
        if( xoico_che_result_cast_s_overrides(o,prev_cast ) ) xoico_che_result_cast_s_deactivate(prev_cast);
    }
    return o;
}

er_t xoico_che_result_cast_s_to_sink( const xoico_che_result_cast_s* o, x_sink* sink )
{
    // xoico_che_result.x:218:5
    
    if( o->active )
    {
        x_sink_push_sc(sink,"((" );
        xoico_che_s_typespec_to_sink(o->che,o->target_typespec, sink );
        x_sink_push_sc(sink,")(" );
        BLM_TRY(xoico_che_result_a_to_sink(BCORE_PASS_TEST(xoico_che_result,o->expression),sink ))
        x_sink_push_sc(sink,"))" );
    }
    else
    {
        BLM_TRY(xoico_che_result_a_to_sink(BCORE_PASS_TEST(xoico_che_result,o->expression),sink ))
    }
    return 0;
}

xoico_che_result_cast_s* xoico_che_result_cast_s__( xoico_che_result_cast_s* o, xoico_che_s* che, xoico_typespec_s* target_typespec, xoico_che_result* expression )
{
    // :2:1
    
    o->che = che;
    xoico_typespec_s_attach( &(o->target_typespec ),  target_typespec);
    xoico_che_result_a_attach( &(o->expression ), (xoico_che_result*)( expression));
    return  o;
}

BCORE_DEFINE_OBJECT_INST_P( xoico_che_result_statement_s )
"aware xoico_che_result"
"{"
    "aware xoico_che_result => expression;"
    "func ^:get_cast;"
    "func ^:set_parent_block;"
    "func ^:to_sink;"
"}";

xoico_che_result_statement_s* xoico_che_result_statement_s_reduce( xoico_che_result_statement_s* o )
{
    // xoico_che_result.x:243:5
    
    xoico_che_result_cast_s* prev_cast = NULL;
    if( xoico_che_result_a_get_cast(o->expression,&(prev_cast )) ) xoico_che_result_cast_s_deactivate(prev_cast);
    return o;
}

xoico_che_result_statement_s* xoico_che_result_statement_s__( xoico_che_result_statement_s* o, xoico_che_result* expression )
{
    // :2:1
    
    xoico_che_result_a_attach( &(o->expression ), (xoico_che_result*)( expression));
    return  o;
}

BCORE_DEFINE_OBJECT_INST_P( xoico_che_result_break_s )
"aware xoico_che_result"
"{"
    "sz_t ledge_level;"
    "hidden xoico_che_result_block_s* parent;"
    "func ^:set_parent_block;"
    "func ^:to_sink;"
"}";

er_t xoico_che_result_break_s_to_sink( const xoico_che_result_break_s* o, x_sink* sink )
{
    // xoico_che_result.x:261:5
    
    if( !o->parent ) ERR_fa( "Parent missing." );
    if( xoico_che_result_block_s_is_using_blm_until_level(o->parent,o->ledge_level ) )
    {
        x_sink_push_fa(sink,"BLM_BREAK_LEVEL(#<sz_t>);", o->ledge_level );
    }
    else
    {
        x_sink_push_fa(sink,"break;" );
    }
    return 0;
}

xoico_che_result_break_s* xoico_che_result_break_s__( xoico_che_result_break_s* o, sz_t ledge_level )
{
    // :2:1
    
    o->ledge_level = ledge_level;
    return  o;
}

BCORE_DEFINE_OBJECT_INST_P( xoico_che_result_return_s )
"aware xoico_che_result"
"{"
    "hidden xoico_che_s * che;"
    "aware xoico_che_result => return_expression;"
    "hidden xoico_che_result_block_s* parent;"
    "func ^:set_parent_block;"
    "func ^:to_sink;"
"}";

er_t xoico_che_result_return_s_to_sink( const xoico_che_result_return_s* o, x_sink* sink )
{
    // xoico_che_result.x:283:5
    
    if( !o->parent ) ERR_fa( "Parent missing." );
    
    if( xoico_che_result_block_s_is_using_blm_until_level(o->parent,0 ) )
    {
        if( xoico_che_s_returns_a_value(o->che) )
        {
            x_sink_push_sc(sink,"BLM_RETURNV(" );
            xoico_che_s_typespec_to_sink(o->che,&(o->che->signature->typespec_ret), sink );
            x_sink_push_sc(sink,"," );
            BLM_TRY(xoico_che_result_a_to_sink(o->return_expression,sink ))
            x_sink_push_sc(sink,")" );  // do not terminate BLM_RETURNV macro with a semicolon, otherwise if-else statements might not be handled correctly
        }
        else
        {
            x_sink_push_sc(sink,"BLM_RETURN()" );
            BLM_TRY(xoico_che_result_a_to_sink(o->return_expression,sink )) //probably not necessary
            x_sink_push_sc(sink,";" );
        }
    }
    else
    {
        x_sink_push_sc(sink,"return " );
        BLM_TRY(xoico_che_result_a_to_sink(o->return_expression,sink ))
        x_sink_push_sc(sink,";" );
    }
    return 0;
}

xoico_che_result_return_s* xoico_che_result_return_s__( xoico_che_result_return_s* o, xoico_che_s* che, xoico_che_result* return_expression )
{
    // :2:1
    
    o->che = che;
    xoico_che_result_a_attach( &(o->return_expression ), (xoico_che_result*)( return_expression));
    return  o;
}

XOILA_DEFINE_SPECT( xoico_che, xoico_che_result )
"{"
    "bcore_spect_header_s header;"
    "feature aware xoico_che_result : clear = xoico_che_result_clear_default;"
    "feature aware xoico_che_result : push_char = xoico_che_result_push_char_default;"
    "feature aware xoico_che_result : push_sc = xoico_che_result_push_sc_default;"
    "feature aware xoico_che_result : push_st = xoico_che_result_push_st_default;"
    "feature aware xoico_che_result : push_result_c = xoico_che_result_push_result_c_default;"
    "feature aware xoico_che_result : push_result_d = xoico_che_result_push_result_d_default;"
    "feature aware xoico_che_result : activate = xoico_che_result_activate_default;"
    "feature aware xoico_che_result : deactivate = xoico_che_result_deactivate_default;"
    "feature aware xoico_che_result : to_sink;"
    "feature aware xoico_che_result : set_parent_block = xoico_che_result_set_parent_block_default;"
    "feature aware xoico_che_result : get_cast = xoico_che_result_get_cast_default;"
    "feature aware xoico_che_result : create_st = xoico_che_result_create_st_default;"
"}";

st_s* xoico_che_result_create_st_default( const xoico_che_result* o )
{
    // xoico_che_result.x:36:1
    
    st_s* st = st_s_create();
    BLM_TRY_EXIT(xoico_che_result_a_to_sink(o,((x_sink*)(st ))))
    return  st;
}

er_t xoico_che_result_push_fv( xoico_che_result* o, sc_t format, va_list args )
{
    // xoico_che_result.x:316:1
    
    st_s* st = st_s_create_fv(format, args );
    er_t ret = xoico_che_result_a_push_st(o,st );
    st_s_discard( st );
    return ret;
}

er_t xoico_che_result_push_fa( xoico_che_result* o, sc_t format, ... )
{
    // xoico_che_result.x:326:1
    
    va_list args;
    va_start( args, format );
    er_t ret = xoico_che_result_push_fv(o,format, args );
    va_end( args );
    return ret;
}

er_t xoico_che_result_copy_fv( xoico_che_result* o, sc_t format, va_list args )
{
    // xoico_che_result.x:337:1
    
    xoico_che_result_a_clear( o );
    return xoico_che_result_push_fv(o,format, args );
}

er_t xoico_che_result_copy_fa( xoico_che_result* o, sc_t format, ... )
{
    // xoico_che_result.x:345:1
    
    va_list args;
    va_start( args, format );
    er_t ret = xoico_che_result_copy_fv(o,format, args );
    va_end( args );
    return ret;
}

//----------------------------------------------------------------------------------------------------------------------
// group: xoico_che_stack_var

BCORE_DEFINE_OBJECT_INST_P( xoico_che_stack_var_unit_s )
"aware bcore_inst"
"{"
    "sz_t level;"
    "tp_t name;"
    "xoico_typespec_s typespec;"
"}";

BCORE_DEFINE_OBJECT_INST_P( xoico_che_stack_var_unit_adl_s )
"aware x_array"
"{"
    "xoico_che_stack_var_unit_s => [];"
"}";

BCORE_DEFINE_OBJECT_INST_P( xoico_che_stack_var_s )
"aware xoico_che_stack_var"
"{"
    "xoico_che_stack_var_unit_adl_s adl;"
    "bcore_hmap_tpuz_s hmap_name;"
"}";

void xoico_che_stack_var_s_rehash_names( xoico_che_stack_var_s* o )
{
    // xoico_che.x:57:9
    
    bcore_hmap_tpuz_s_clear(&(o->hmap_name));
    for(sz_t i = 0; i < o->adl.size; i++ ) bcore_hmap_tpuz_s_set(&(o->hmap_name),o->adl.data[i]->name, i );
}

xoico_che_stack_var_s* xoico_che_stack_var_s_push_unit( xoico_che_stack_var_s* o, const xoico_che_stack_var_unit_s* unit )
{
    // xoico_che.x:63:9
    
    x_array_push_c(((x_array*)(&(o->adl))),((const x_inst*)(unit )));
    bcore_hmap_tpuz_s_set(&(o->hmap_name),unit->name, o->adl.size - 1 );
    return  o;
}

xoico_che_stack_var_s* xoico_che_stack_var_s_pop_level( xoico_che_stack_var_s* o, sz_t level )
{
    // xoico_che.x:70:9
    
    sz_t size = o->adl.size;
    while( size > 0 && o->adl.data[ size - 1 ]->level >= level ) size--;
    x_array_set_size(((x_array*)(&(o->adl))),size );
    xoico_che_stack_var_s_rehash_names(o);
    return  o;
}

const xoico_typespec_s* xoico_che_stack_var_s_get_typespec( const xoico_che_stack_var_s* o, tp_t name )
{
    // xoico_che.x:79:9
    
    uz_t* p_idx = bcore_hmap_tpuz_s_get(&(o->hmap_name),name );
    if( !p_idx ) return  NULL;
    return &( o->adl.data[ (*(p_idx)) ]->typespec);
}

const sz_t xoico_che_stack_var_s_get_level( const xoico_che_stack_var_s* o, tp_t name )
{
    // xoico_che.x:87:9
    
    uz_t* p_idx = bcore_hmap_tpuz_s_get(&(o->hmap_name),name );
    if( !p_idx ) return  -1;
    return  o->adl.data[ (*(p_idx)) ]->level;
}

void xoico_che_stack_var_s_clear( xoico_che_stack_var_s* o )
{
    // xoico_che.x:94:9
    
    x_array_clear(((x_array*)(&(o->adl))));
    bcore_hmap_tpuz_s_clear(&(o->hmap_name));
}

XOILA_DEFINE_SPECT( xoico_che, xoico_che_stack_var )
"{"
    "bcore_spect_header_s header;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: xoico_che_stack_block

BCORE_DEFINE_OBJECT_INST_P( xoico_che_stack_block_unit_s )
"aware bcore_inst"
"{"
    "sz_t level;"
    "bl_t use_blm = false;"
    "bl_t break_ledge = false;"
    "bl_t statement_wrapped_as_block = false;"
"}";

BCORE_DEFINE_OBJECT_INST_P( xoico_che_stack_block_unit_adl_s )
"aware x_array"
"{"
    "xoico_che_stack_block_unit_s => [];"
"}";

BCORE_DEFINE_OBJECT_INST_P( xoico_che_stack_block_s )
"aware xoico_che_stack_block"
"{"
    "xoico_che_stack_block_unit_adl_s adl;"
"}";

xoico_che_stack_block_s* xoico_che_stack_block_s_pop( xoico_che_stack_block_s* o )
{
    // xoico_che.x:139:9
    
    x_array_set_size(((x_array*)(&(o->adl))),sz_max( o->adl.size - 1, 0 ) );
    return  o;
}

XOILA_DEFINE_SPECT( xoico_che, xoico_che_stack_block )
"{"
    "bcore_spect_header_s header;"
"}";

/**********************************************************************************************************************/
// source: xoico_builder.x

//----------------------------------------------------------------------------------------------------------------------
// group: xoico_builder

BCORE_DEFINE_OBJECT_INST_P( xoico_builder_arr_target_s )
"aware x_array"
"{"
    "xoico_builder_target_s => [];"
"}";

BCORE_DEFINE_OBJECT_INST_P( xoico_builder_target_s )
"aware xoico_builder"
"{"
    "st_s => name;"
    "st_s => extension = \"xo\";"
    "st_s => root_folder;"
    "bl_t readonly;"
    "st_s => output_folder;"
    "st_s => copyright_and_license_terms;"
    "bcore_arr_st_s dependencies;"
    "bcore_arr_st_s sources;"
    "st_s => signal_handler;"
    "bl_t define_signal_handler = true;"
    "aware xoico_cengine => cengine = xoico_che_s;"
    "private xoico_compiler_s* compiler;"
    "private xoico_builder_target_s* parent_;"
    "private xoico_builder_target_s* root_;"
    "hidden aware xoico_builder_arr_target_s => dependencies_target_;"
    "hidden st_s full_path_;"
    "hidden sz_t target_index_ = -1;"
    "hidden bcore_hmap_tpvd_s => hmap_built_target_;"
    "func bcore_via_call:source;"
    "func xoico:get_hash;"
"}";

const st_s* xoico_builder_target_s_root_output_folder( const xoico_builder_target_s* o )
{
    // xoico_builder.x:42:5
    
    const st_s* folder = ( o->parent_ ) ? xoico_builder_target_s_root_output_folder(o->parent_) : NULL;
    return  folder ? folder : o->output_folder;
}

void xoico_builder_target_s_source( xoico_builder_target_s* o, bcore_source* source )
{
    // xoico_builder.x:79:5
    
    if( !o->root_folder )
    {
        o->root_folder = bcore_file_folder_path(bcore_source_a_get_file(source) );
        st_s_attach( &(o->root_folder ),  bcore_file_path_minimized(o->root_folder->sc ));
    }
}

const xoico_builder_target_s* xoico_builder_target_s_name_match( const xoico_builder_target_s* o, sc_t name )
{
    // xoico_builder.x:88:5
    
    if( o->name && sc_t_equal( name, o->name->sc ) ) return  o;
    if( o->parent_ ) return  xoico_builder_target_s_name_match(o->parent_,name );
    return  NULL;
}

void xoico_builder_target_s_push_target_index_to_arr( const xoico_builder_target_s* o, bcore_arr_sz_s* arr )
{
    // xoico_builder.x:95:5
    
    if( o->target_index_ != -1 )
    {
         bcore_arr_sz_s_push(arr,o->target_index_ );
    }
    else
    {
        {const xoico_builder_arr_target_s* __a=o->dependencies_target_ ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_builder_target_s* e=__a->data[__i]; xoico_builder_target_s_push_target_index_to_arr(e,arr );}}
    }
}

tp_t xoico_builder_target_s_get_hash( const xoico_builder_target_s* o )
{
    // xoico_builder.x:107:5
    
    tp_t hash = bcore_tp_init();
    hash = o->name ? bcore_tp_fold_sc( hash, o->name->sc ) : hash;
    hash = o->extension ? bcore_tp_fold_sc( hash, o->extension->sc ) : hash;
    hash = o->root_folder ? bcore_tp_fold_sc( hash, o->root_folder->sc ) : hash;
    {const bcore_arr_st_s* __a=&(o->dependencies );if(__a)for(sz_t __i=0;__i<__a->size;__i++){const st_s* e=__a->data[__i]; hash = bcore_tp_fold_sc( hash, e->sc );}}
    {const bcore_arr_st_s* __a=&(o->sources );if(__a)for(sz_t __i=0;__i<__a->size;__i++){const st_s* e=__a->data[__i]; hash = bcore_tp_fold_sc( hash, e->sc );}}
    hash = o->signal_handler ? bcore_tp_fold_sc( hash, o->signal_handler->sc ) : hash;
    hash = bcore_tp_fold_bl( hash, o->define_signal_handler );
    hash = o->cengine ? bcore_tp_fold_bl( hash, xoico_a_get_hash(((const xoico*)(o->cengine))) ) : hash;
    return  hash;
}

er_t xoico_builder_target_s_load( xoico_builder_target_s* o, bl_t readonly, sc_t path )
{
    // xoico_builder.x:192:1
    BLM_INIT_LEVEL(0);
    st_s* st_path = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
    st_s_copy_sc(st_path,path );
    
    if( st_path->sc[ 0 ] != '/' )
    {
        st_s* current_folder = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create()));
        bcore_folder_get_current( current_folder );
        st_path = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create_fa("#<sc_t>/#<sc_t>", current_folder->sc, st_path->sc )));
    }
    
    st_path = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,bcore_file_path_minimized(st_path->sc )));
    
    BLM_TRY(x_btcl_from_file(((x_btcl*)(o)),st_path->sc ))
    st_s_copy(&(o->full_path_),st_path );
    if( readonly ) o->readonly = true;
    
    if( !o->name ) BLM_RETURNV(er_t, bcore_error_push_fa(((tp_t)(TYPEOF_general_error)), "In target file: '#<sc_t>'\nTarget name must be specified.", st_path->sc ))
    
    /// check for dependency cycles
    if( o->parent_ )
    {
        const xoico_builder_target_s* match = xoico_builder_target_s_name_match(o->parent_,o->name->sc );
        if( match )
        {
            if( st_s_equal_st(&(match->full_path_),&(o->full_path_ )) )
            {
                BLM_RETURNV(er_t, bcore_error_push_fa(((tp_t)(TYPEOF_general_error)), "In target file: '#<sc_t>'\nCyclic dependency detected.", st_path->sc ))
            }
            else
            {
                BLM_RETURNV(er_t, bcore_error_push_fa(((tp_t)(TYPEOF_general_error)), "Same target name '#<sc_t>' used in different target files:\n#<sc_t>\n#<sc_t>", o->name->sc, st_path->sc, match->full_path_.sc ))
            }
        }
    }
    
    {const bcore_arr_st_s* __a=&(o->dependencies );if(__a)for(sz_t __i=0;__i<__a->size;__i++){st_s* e=__a->data[__i];
    {BLM_INIT_LEVEL(4);
        if( !o->dependencies_target_ ) o->dependencies_target_ = xoico_builder_arr_target_s_create();
    
        st_s* file_path = ((st_s*)BLM_LEVEL_T_PUSH(4,st_s,st_s_create()));
        if( e->sc[ 0 ] != '/' )
        {
            if( o->root_folder ) st_s_push_fa(file_path,"#<sc_t>/", o->root_folder->sc );
        }
    
        bl_t dep_readonly = o->readonly;
    
        x_source* source = ((x_source*)BLM_LEVEL_A_PUSH(4,x_source_create_from_sc(e->sc )));
        BLM_TRY(x_source_parse_fa(source," #:until':'", file_path ))
    
        /// remove trailing spaces
        while( file_path->sc[ 0 ] == ' ' || file_path->sc[ 0 ] == '\t' ) st_s_pop_char(file_path);
    
        if( x_source_parse_bl(source,"#?':'" ) )
        {
            if( x_source_parse_bl(source," #?w'readonly'" ) )
            {
                dep_readonly = true;
            }
            else
            {
                BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Syntax error in dependency declaration." ))
            }
        }
    
        xoico_builder_target_s* target = ((xoico_builder_target_s*)(x_array_push_d(((x_array*)(o->dependencies_target_)),((x_inst*)(xoico_builder_target_s_create() )))));
        target->parent_ = o;
    
        BLM_TRY(xoico_builder_target_s_load(target,dep_readonly, file_path->sc ))
    BLM_DOWN();}
    
    }}BLM_RETURNV(er_t, 0)
}

er_t xoico_builder_target_s_build( xoico_builder_target_s* o )
{
    // xoico_builder.x:270:1
    BLM_INIT_LEVEL(0);
    if( !o->root_    ) o->root_    = ( o->parent_ ) ? o->parent_->root_    : o;
    if( !o->compiler ) o->compiler = ( o->parent_ ) ? o->parent_->compiler : NULL;
    
    if( o == o->root_ )
    {
        if( !o->hmap_built_target_ ) o->hmap_built_target_ = bcore_hmap_tpvd_s_create();
    }
    
    ASSERT( o->compiler );
    ASSERT( o->root_ );
    
    tp_t tp_target_name = bentypeof( o->name->sc );
    
    {const xoico_builder_arr_target_s* __a=o->dependencies_target_ ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_builder_target_s* e=__a->data[__i]; BLM_TRY(xoico_builder_target_s_build(e))
    
    }}if( bcore_hmap_tpvd_s_exists(o->root_->hmap_built_target_,tp_target_name ) )
    {
        xoico_builder_target_s* target = (*(((xoico_builder_target_s**)(bcore_hmap_tpvd_s_get(o->root_->hmap_built_target_,tp_target_name )))));
        o->target_index_ = target->target_index_;
        BLM_RETURNV(er_t, 0)
    }
    
    bcore_hmap_tpvd_s_set(o->root_->hmap_built_target_,tp_target_name, ( vd_t )o );
    
    o->target_index_ = -1;
    
    bcore_msg_fa( "XOICO: compiling #<sc_t>\n", o->full_path_.sc );
    
    {const bcore_arr_st_s* __a=&(o->sources );if(__a)for(sz_t __i=0;__i<__a->size;__i++){const st_s* e=__a->data[__i];
    {BLM_INIT_LEVEL(4);
        x_source* source = ((x_source*)BLM_LEVEL_A_PUSH(4,x_source_create_from_st(e )));
        st_s group_name;BLM_T_INIT_SPUSH(st_s, &group_name);;
        st_s trait_name;BLM_T_INIT_SPUSH(st_s, &trait_name);;
        st_s file_path;BLM_T_INIT_SPUSH(st_s, &file_path);;
    
        tp_t embed_method = 0;
    
        BLM_TRY(x_source_parse_fa(source," " )) // take whitespaces
    
        if( x_source_parse_bl(source,"#?w'group' " ) )
        {
            BLM_TRY(x_source_parse_fa(source,"#name ", (&(group_name)) ))
            if( group_name.size == 0 ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Group name expected in source declaration." ))
            BLM_TRY(x_source_parse_fa(source,"= #name ", (&(trait_name)) ))
            if( trait_name.size == 0 ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Trait name expected in source declaration." ))
        }
    
        if( x_source_parse_bl(source,"#?w'embed' " ) )
        {
            if( x_source_parse_bl(source,"#?w'as string' " ) )
            {
                embed_method = TYPEOF_as_string;
            }
            else
            {
                BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Specify embedding method before file path (e.g. 'as string')" ))
            }
        }
    
        while( !x_source_eos(source) )
        {
            char c = x_source_get_char(source);
            if( c == ':' ) break;
            st_s_push_char(&(file_path),c );
        }
    
        if( x_source_parse_bl(source," #?w'embed'" ) )
        {
            if( x_source_parse_bl(source," #?w'as_string'" ) )
            {
                embed_method = TYPEOF_as_string;
            }
            else
            {
                BLM_RETURNV(er_t, x_source_parse_error_fa(source,"Select a valid embedding method (e.g. 'as_string')" ))
            }
        }
    
        if( file_path.size == 0 ) BLM_RETURNV(er_t, x_source_parse_error_fa(source,"File name expected in source declaration." ))
    
        if( e->sc[ 0 ] != '/' && o->root_folder )
        {BLM_INIT_LEVEL(5);
            const st_s* tmp = ((st_s*)BLM_LEVEL_T_PUSH(5,st_s,st_s_clone(&(file_path))));
            st_s_copy_fa(&(file_path),"#<sc_t>/#<sc_t>", o->root_folder->sc, tmp->sc );
        BLM_DOWN();}
    
        if( embed_method == 0 )
        {
            if     ( st_s_ends_in_sc(&(file_path),".h" ) ) embed_method = TYPEOF_as_header;
            else if( st_s_ends_in_sc(&(file_path),".x" ) ) embed_method = TYPEOF_as_group;
            else                                    embed_method = TYPEOF_as_string;
        }
    
        if( group_name.size == 0 )
        {
            if( embed_method == TYPEOF_as_header )
            {
                /// nothing here
            }
            else if( embed_method == TYPEOF_as_group )
            {BLM_INIT_LEVEL(7);
                st_s_copy(&(group_name),((st_s*)BLM_LEVEL_T_PUSH(7,st_s,bcore_file_strip_extension(bcore_file_name(file_path.sc ) ))) );
            BLM_DOWN();}
            else
            {
                st_s_copy_sc(&(group_name),bcore_file_name(file_path.sc ) );
            }
            st_s_replace_char_char(&(group_name),'.', '_' );
            st_s_replace_char_char(&(group_name),' ', '_' );
            st_s_copy_sc(&(trait_name),"x_inst" );
        }
    
        ASSERT( o->name );
        ASSERT( o->extension );
        sz_t index = -1;
    
        BLM_TRY(xoico_compiler_s_parse(o->compiler,o->name->sc, o->extension->sc, xoico_builder_target_s_root_output_folder(o), file_path.sc, group_name.sc, trait_name.sc, embed_method, (&(index)) ))
    
        if( o->target_index_ == -1 ) o->target_index_ = index;
        if( index != o->target_index_ )
        {
            BLM_RETURNV(er_t, bcore_error_push_fa(((tp_t)(TYPEOF_general_error)),
                "Building source '#<sc_t>' in target '#<sc_t>':\n"
                "Target index mismatch.\n"
                "This problem occurs when a target defines sources from different directories.\n",
                e->sc,
                o->name->sc
            ))
        }
    BLM_DOWN();}
    
    }}if( o->target_index_ >= 0 )
    {BLM_INIT_LEVEL(1);
        ASSERT( o->target_index_ < o->compiler->size );
        bcore_arr_sz_s* dependencies = ((bcore_arr_sz_s*)BLM_LEVEL_T_PUSH(1,bcore_arr_sz_s,bcore_arr_sz_s_create()));
        {const xoico_builder_arr_target_s* __a=o->dependencies_target_ ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){xoico_builder_target_s* e=__a->data[__i]; xoico_builder_target_s_push_target_index_to_arr(e,dependencies );}}
    
        xoico_target_s* target = o->compiler->data[ o->target_index_ ];
    
        BLM_TRY(xoico_target_s_set_dependencies(target,dependencies ))
        st_s* signal_handler = ((st_s*)BLM_LEVEL_T_PUSH(0,st_s,st_s_create_fa("#<sc_t>_general_signal_handler", o->name->sc )));
        if( o->signal_handler ) st_s_copy(signal_handler,o->signal_handler );
        st_s_copy_sc(&(target->signal_handler_name),signal_handler->sc );
        target->define_signal_handler = o->define_signal_handler;
        target->readonly = o->readonly;
        xoico_cengine_a_attach( &(target->cengine ), (xoico_cengine*)( ((xoico_cengine*)bcore_fork(o->cengine))));
        target->pre_hash = xoico_builder_target_s_get_hash(o);
        st_s_attach( &(target->copyright_and_license_terms ),  ((st_s*)bcore_fork(o->copyright_and_license_terms)));
    BLM_DOWN();}
    
    BLM_RETURNV(er_t, 0)
}

BCORE_DEFINE_OBJECT_INST_P( xoico_builder_main_s )
"aware xoico_builder"
"{"
    "st_s => output_folder;"
    "xoico_compiler_s => compiler!;"
    "xoico_builder_target_s => target;"
"}";

er_t xoico_builder_main_s_set_dry_run( xoico_builder_main_s* o, bl_t v )
{
    // xoico_builder.x:152:5
    
    o->compiler->dry_run = v;
    return  0;
}

bl_t xoico_builder_main_s_get_dry_run( const xoico_builder_main_s* o )
{
    // xoico_builder.x:158:5
    
    return  o->compiler->dry_run;
}

er_t xoico_builder_main_s_set_always_expand( xoico_builder_main_s* o, bl_t v )
{
    // xoico_builder.x:163:5
    
    o->compiler->always_expand = v;
    return  0;
}

bl_t xoico_builder_main_s_get_always_expand( const xoico_builder_main_s* o )
{
    // xoico_builder.x:169:5
    
    return  o->compiler->always_expand;
}

er_t xoico_builder_main_s_set_overwrite_unsigned_target_files( xoico_builder_main_s* o, bl_t v )
{
    // xoico_builder.x:174:5
    
    o->compiler->overwrite_unsigned_target_files = v;
    return  0;
}

bl_t xoico_builder_main_s_get_overwrite_unsigned_target_files( const xoico_builder_main_s* o )
{
    // xoico_builder.x:180:5
    
    return  o->compiler->overwrite_unsigned_target_files;
}

er_t xoico_builder_main_s_build_from_file( xoico_builder_main_s* o, sc_t path )
{
    // xoico_builder.x:433:1
    
    xoico_builder_target_s_attach( &(o->target ),  xoico_builder_target_s_create());
    BLM_TRY(xoico_builder_target_s_load(o->target,false, path ))
    o->target->compiler = o->compiler;
    if( o->output_folder ) st_s_attach( &(o->target->output_folder ),  ((st_s*)bcore_fork(o->output_folder)));
    BLM_TRY(xoico_builder_target_s_build(o->target))
    BLM_TRY(xoico_compiler_s_finalize(o->compiler,((const xoico_host*)(o ))))
    return  0;
}

er_t xoico_builder_main_s_update( const xoico_builder_main_s* o )
{
    // xoico_builder.x:446:1
    
    if( bcore_error_stack_size() > 0 ) return  ((tp_t)(TYPEOF_error_stack));
    BLM_TRY(xoico_compiler_s_update_target_files(o->compiler,NULL ) )
    return  0;
}

XOILA_DEFINE_SPECT( xoico, xoico_builder )
"{"
    "bcore_spect_header_s header;"
"}";

/**********************************************************************************************************************/
// source: xoico_main.x

//----------------------------------------------------------------------------------------------------------------------
// group: xoico_main

XOILA_DEFINE_SPECT( x_inst, xoico_main )
"{"
    "bcore_spect_header_s header;"
"}";

void xoico_main_help( x_sink* sink )
{
    // xoico_main.x:21:1
    
    x_sink_push_sc(sink,"Xoila Compiler: (C) J.B.Steffens\n"
        "Usage:\n"
        "$ xoico [options] xoico <xoico-config-file> [<xoico-config-file> ...]   #normal operation\n"
        "$ xoico --help                                                          #displays this text\n"
        "\n"
        "Options:\n"
        "-d : Dry run. Target files are not modified.\n"
        "-e : Always expand: Expands a target even if its hash value has not changed.\n"
        "-f : Force overwrite target files. Use with care.\n"
        "-O <folder>: Output folder for this target and all dependencies.\n"
    );
}

s2_t xoico_main_main( const bcore_arr_st_s* args )
{
    // xoico_main.x:40:1
    BLM_INIT_LEVEL(0);
    xoico_builder_main_s builder_main;BLM_T_INIT_SPUSH(xoico_builder_main_s, &builder_main);;
    er_t er = 0;
    
    sz_t arg_idx = 1;
    
    if( args->size > 1 )
    {
        if( st_s_equal_sc(args->data[ arg_idx ],"--help" ) )
        {
            xoico_main_help(x_sink_stdout() );
        }
        else
        {
            while( arg_idx < args->size && args->data[ arg_idx ]->sc[ 0 ] == '-' )
            {
                if( st_s_equal_sc(args->data[ arg_idx ],"-d" ) )
                {
                    BLM_TRY_EXIT(xoico_builder_main_s_set_dry_run(&(builder_main),true ))
                }
                else if( st_s_equal_sc(args->data[ arg_idx ],"-e" ) )
                {
                    BLM_TRY_EXIT(xoico_builder_main_s_set_always_expand(&(builder_main),true ))
                }
                else if( st_s_equal_sc(args->data[ arg_idx ],"-f" ) )
                {
                    BLM_TRY_EXIT(xoico_builder_main_s_set_overwrite_unsigned_target_files(&(builder_main),true ))
                }
                else if( st_s_equal_sc(args->data[ arg_idx ],"-O" ) )
                {
                    arg_idx++;
                    if( args->size == arg_idx ) ERR_fa( "Output folder expected.\n" );
                    st_s_attach( &(builder_main.output_folder ),  st_s_clone(args->data[ arg_idx ]));
                }
                else
                {
                    ERR_fa( "Invalid option: #<sc_t>\n", args->data[ arg_idx ]->sc );
                }
                arg_idx++;
            }
    
            if( xoico_builder_main_s_get_dry_run(&(builder_main)) )
            {
                x_sink_push_fa(x_sink_stdout(),"Dry run ...\n" );
            }
    
            if( xoico_builder_main_s_get_always_expand(&(builder_main)) )
            {
                x_sink_push_fa(x_sink_stdout(),"Expanding all ...\n" );
            }
    
            if( arg_idx >= args->size ) xoico_main_help(x_sink_stdout() );
            clock_t time = clock();
    
            for(sz_t i = arg_idx; i < args->size; i++ )
            {
                if( ( er = xoico_builder_main_s_build_from_file(&(builder_main),args->data[ i ]->sc ) ) ) break;
            }
    
            if( !er && xoico_builder_main_s_update_required(&(builder_main)) )
            {
                er = xoico_builder_main_s_update(&(builder_main));
            }
    
            if( !er )
            {
                f3_t time_var = clock() - time;
                time_var /= CLOCKS_PER_SEC;
                x_sink_push_st_d(x_sink_stdout(),st_s_createf("Finished after %.3f seconds.\n", time_var ) );
            }
        }
    }
    else
    {BLM_INIT_LEVEL(1);
        er = 1;
        st_s st;BLM_T_INIT_SPUSH(st_s, &st);;
        xoico_main_help(((x_sink*)(&(st ))));
        BLM_TRY_EXIT(bcore_error_push_sc(er, st.sc ))
    BLM_DOWN();}
    
    if( er ) bcore_error_pop_all_to_stderr();
    
    BLM_RETURNV(s2_t, ( er > 0 ) ? 1 : 0)
}

/**********************************************************************************************************************/

vd_t bcore_general_signal_handler( const bcore_signal_s* o );

vd_t xoico_xo_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "xoico_xo" ) ) )
    {
        case TYPEOF_init1:
        {

            // --------------------------------------------------------------------
            // source: xoico.x

            // group: xoico
            BCORE_REGISTER_NAME( mutable );
            BCORE_REGISTER_NAME( const );
            BCORE_REGISTER_NAME( void );
            BCORE_REGISTER_NAME( int );
            BCORE_REGISTER_NAME( long );
            BCORE_REGISTER_NAME( float );
            BCORE_REGISTER_NAME( double );
            BCORE_REGISTER_NAME( type );
            BCORE_REGISTER_NAME( aware );
            BCORE_REGISTER_NAME( obliv );
            BCORE_REGISTER_NAME( _ );
            BCORE_REGISTER_NAME( o );
            BCORE_REGISTER_FEATURE( xoico_convert_transient_types );
            BCORE_REGISTER_FEATURE( xoico_get_hash );
            BCORE_REGISTER_FEATURE( xoico_get_global_name_tp );
            BCORE_REGISTER_FEATURE( xoico_parse );
            BCORE_REGISTER_FEATURE( xoico_finalize );
            BCORE_REGISTER_FFUNC( xoico_finalize, xoico_finalize_default );
            BCORE_REGISTER_FEATURE( xoico_expand_setup );
            BCORE_REGISTER_FFUNC( xoico_expand_setup, xoico_expand_setup_default );
            BCORE_REGISTER_FEATURE( xoico_expand_forward );
            BCORE_REGISTER_FFUNC( xoico_expand_forward, xoico_expand_forward_default );
            BCORE_REGISTER_FEATURE( xoico_expand_indef_typedef );
            BCORE_REGISTER_FFUNC( xoico_expand_indef_typedef, xoico_expand_indef_typedef_default );
            BCORE_REGISTER_FEATURE( xoico_expand_spect_declaration );
            BCORE_REGISTER_FFUNC( xoico_expand_spect_declaration, xoico_expand_spect_declaration_default );
            BCORE_REGISTER_FEATURE( xoico_expand_spect_definition );
            BCORE_REGISTER_FFUNC( xoico_expand_spect_definition, xoico_expand_spect_definition_default );
            BCORE_REGISTER_FEATURE( xoico_expand_declaration );
            BCORE_REGISTER_FFUNC( xoico_expand_declaration, xoico_expand_declaration_default );
            BCORE_REGISTER_FEATURE( xoico_expand_indef_declaration );
            BCORE_REGISTER_FFUNC( xoico_expand_indef_declaration, xoico_expand_indef_declaration_default );
            BCORE_REGISTER_FEATURE( xoico_expand_definition );
            BCORE_REGISTER_FFUNC( xoico_expand_definition, xoico_expand_definition_default );
            BCORE_REGISTER_FEATURE( xoico_expand_init1 );
            BCORE_REGISTER_FFUNC( xoico_expand_init1, xoico_expand_init1_default );
            BCORE_REGISTER_FEATURE( xoico_expand_down1 );
            BCORE_REGISTER_FFUNC( xoico_expand_down1, xoico_expand_down1_default );
            BCORE_REGISTER_FEATURE( xoico_expand_manifesto );
            BCORE_REGISTER_FFUNC( xoico_expand_manifesto, xoico_expand_manifesto_default );
            BCORE_REGISTER_FEATURE( xoico_get_source_point );
            BCORE_REGISTER_FFUNC( xoico_get_source_point, xoico_get_source_point_default );
            BCORE_REGISTER_FUNC( xoico_parse_f );
            BCORE_REGISTER_FUNC( xoico_embed_file_open );
            BCORE_REGISTER_FUNC( xoico_parse_sc );
            BCORE_REGISTER_FUNC( xoico_parse_fa );
            XOILA_REGISTER_SPECT( xoico );

            // --------------------------------------------------------------------
            // source: xoico_host.x

            // group: xoico_host
            BCORE_REGISTER_FEATURE( xoico_host_parse_name_st );
            BCORE_REGISTER_FFUNC( xoico_host_parse_name_st, xoico_host_parse_name_st_default );
            BCORE_REGISTER_FEATURE( xoico_host_parse_name_tp );
            BCORE_REGISTER_FFUNC( xoico_host_parse_name_tp, xoico_host_parse_name_tp_default );
            BCORE_REGISTER_FEATURE( xoico_host_compiler );
            BCORE_REGISTER_FFUNC( xoico_host_compiler, xoico_host_compiler_default );
            BCORE_REGISTER_FEATURE( xoico_host_cengine );
            BCORE_REGISTER_FFUNC( xoico_host_cengine, xoico_host_cengine_default );
            BCORE_REGISTER_FEATURE( xoico_host_obj_type );
            BCORE_REGISTER_FFUNC( xoico_host_obj_type, xoico_host_obj_type_default );
            BCORE_REGISTER_FEATURE( xoico_host_create_spect_name );
            BCORE_REGISTER_FFUNC( xoico_host_create_spect_name, xoico_host_create_spect_name_default );
            BCORE_REGISTER_FEATURE( xoico_host_transient_map );
            BCORE_REGISTER_FEATURE( xoico_host_nameof );
            BCORE_REGISTER_FFUNC( xoico_host_nameof, xoico_host_nameof_default );
            BCORE_REGISTER_FEATURE( xoico_host_entypeof );
            BCORE_REGISTER_FFUNC( xoico_host_entypeof, xoico_host_entypeof_default );
            XOILA_REGISTER_SPECT( xoico_host );

            // --------------------------------------------------------------------
            // source: xoico_name.x

            // group: xoico_name
            BCORE_REGISTER_FFUNC( xoico_parse, xoico_name_s_parse );
            BCORE_REGISTER_FFUNC( xoico_get_hash, xoico_name_s_get_hash );
            BCORE_REGISTER_FFUNC( xoico_get_global_name_tp, xoico_name_s_get_global_name_tp );
            BCORE_REGISTER_FFUNC( xoico_expand_declaration, xoico_name_s_expand_declaration );
            BCORE_REGISTER_FFUNC( xoico_expand_init1, xoico_name_s_expand_init1 );
            BCORE_REGISTER_OBJECT( xoico_name_s );
            XOILA_REGISTER_SPECT( xoico_name );

            // --------------------------------------------------------------------
            // source: xoico_forward.x

            // group: xoico_forward
            BCORE_REGISTER_FFUNC( xoico_parse, xoico_forward_s_parse );
            BCORE_REGISTER_FFUNC( xoico_get_hash, xoico_forward_s_get_hash );
            BCORE_REGISTER_FFUNC( xoico_get_global_name_tp, xoico_forward_s_get_global_name_tp );
            BCORE_REGISTER_FFUNC( xoico_expand_declaration, xoico_forward_s_expand_declaration );
            BCORE_REGISTER_FFUNC( xoico_expand_forward, xoico_forward_s_expand_forward );
            BCORE_REGISTER_FFUNC( xoico_expand_init1, xoico_forward_s_expand_init1 );
            BCORE_REGISTER_FFUNC( xoico_get_source_point, xoico_forward_s_get_source_point );
            BCORE_REGISTER_OBJECT( xoico_forward_s );
            XOILA_REGISTER_SPECT( xoico_forward );

            // --------------------------------------------------------------------
            // source: xoico_typespec.x

            // group: xoico_typespec
            BCORE_REGISTER_NAME( type_deduce );
            BCORE_REGISTER_NAME( type_object );
            BCORE_REGISTER_NAME( const );
            BCORE_REGISTER_NAME( mutable );
            BCORE_REGISTER_NAME( discardable );
            BCORE_REGISTER_NAME( c );
            BCORE_REGISTER_NAME( m );
            BCORE_REGISTER_NAME( d );
            BCORE_REGISTER_FFUNC( xoico_get_hash, xoico_typespec_transient_s_get_hash );
            BCORE_REGISTER_OBJECT( xoico_typespec_transient_s );
            BCORE_REGISTER_FFUNC( xoico_convert_transient_types, xoico_typespec_s_convert_transient_types );
            BCORE_REGISTER_FFUNC( xoico_get_hash, xoico_typespec_s_get_hash );
            BCORE_REGISTER_OBJECT( xoico_typespec_s );
            BCORE_REGISTER_FUNC( xoico_typespec_is_numeric );
            XOILA_REGISTER_SPECT( xoico_typespec );

            // --------------------------------------------------------------------
            // source: xoico_transient_map.x

            // group: xoico_transient_map
            BCORE_REGISTER_FFUNC( xoico_parse, xoico_transient_map_s_parse );
            BCORE_REGISTER_OBJECT( xoico_transient_map_s );
            XOILA_REGISTER_SPECT( xoico_transient_map );

            // --------------------------------------------------------------------
            // source: xoico_arg.x

            // group: xoico_arg
            BCORE_REGISTER_FFUNC( xoico_get_source_point, xoico_arg_s_get_source_point );
            BCORE_REGISTER_FFUNC( xoico_parse, xoico_arg_s_parse );
            BCORE_REGISTER_FFUNC( xoico_get_hash, xoico_arg_s_get_hash );
            BCORE_REGISTER_FFUNC( xoico_convert_transient_types, xoico_arg_s_convert_transient_types );
            BCORE_REGISTER_OBJECT( xoico_arg_s );
            XOILA_REGISTER_SPECT( xoico_arg );

            // --------------------------------------------------------------------
            // source: xoico_args.x

            // group: xoico_args
            BCORE_REGISTER_FFUNC( xoico_parse, xoico_args_s_parse );
            BCORE_REGISTER_FFUNC( xoico_convert_transient_types, xoico_args_s_convert_transient_types );
            BCORE_REGISTER_FFUNC( xoico_get_hash, xoico_args_s_get_hash );
            BCORE_REGISTER_OBJECT( xoico_args_s );
            XOILA_REGISTER_SPECT( xoico_args );

            // --------------------------------------------------------------------
            // source: xoico_signature.x

            // group: xoico_signature
            BCORE_REGISTER_FFUNC( xoico_convert_transient_types, xoico_signature_s_convert_transient_types );
            BCORE_REGISTER_FFUNC( xoico_get_global_name_tp, xoico_signature_s_get_global_name_tp );
            BCORE_REGISTER_FFUNC( xoico_get_source_point, xoico_signature_s_get_source_point );
            BCORE_REGISTER_FFUNC( xoico_get_hash, xoico_signature_s_get_hash );
            BCORE_REGISTER_FFUNC( xoico_parse, xoico_signature_s_parse );
            BCORE_REGISTER_OBJECT( xoico_signature_s );
            XOILA_REGISTER_SPECT( xoico_signature );

            // --------------------------------------------------------------------
            // source: xoico_body.x

            // group: xoico_body
            BCORE_REGISTER_FFUNC( xoico_get_source_point, xoico_body_code_s_get_source_point );
            BCORE_REGISTER_FFUNC( xoico_parse, xoico_body_code_s_parse );
            BCORE_REGISTER_FFUNC( xoico_get_hash, xoico_body_code_s_get_hash );
            BCORE_REGISTER_OBJECT( xoico_body_code_s );
            BCORE_REGISTER_FFUNC( xoico_get_global_name_tp, xoico_body_s_get_global_name_tp );
            BCORE_REGISTER_FFUNC( xoico_get_hash, xoico_body_s_get_hash );
            BCORE_REGISTER_FFUNC( xoico_parse, xoico_body_s_parse );
            BCORE_REGISTER_OBJECT( xoico_body_s );
            XOILA_REGISTER_SPECT( xoico_body );

            // --------------------------------------------------------------------
            // source: xoico_func.x

            // group: xoico_func
            BCORE_REGISTER_FFUNC( xoico_get_source_point, xoico_func_s_get_source_point );
            BCORE_REGISTER_FFUNC( xoico_parse, xoico_func_s_parse );
            BCORE_REGISTER_FFUNC( xoico_finalize, xoico_func_s_finalize );
            BCORE_REGISTER_FFUNC( xoico_expand_forward, xoico_func_s_expand_forward );
            BCORE_REGISTER_FFUNC( xoico_expand_declaration, xoico_func_s_expand_declaration );
            BCORE_REGISTER_FFUNC( xoico_expand_definition, xoico_func_s_expand_definition );
            BCORE_REGISTER_FFUNC( xoico_expand_init1, xoico_func_s_expand_init1 );
            BCORE_REGISTER_OBJECT( xoico_func_s );
            XOILA_REGISTER_SPECT( xoico_func );

            // --------------------------------------------------------------------
            // source: xoico_funcs.x

            // group: xoico_funcs
            BCORE_REGISTER_OBJECT( xoico_funcs_s );
            XOILA_REGISTER_SPECT( xoico_funcs );

            // --------------------------------------------------------------------
            // source: xoico_feature.x

            // group: xoico_feature
            BCORE_REGISTER_FFUNC( xoico_get_global_name_tp, xoico_feature_s_get_global_name_tp );
            BCORE_REGISTER_FFUNC( xoico_get_source_point, xoico_feature_s_get_source_point );
            BCORE_REGISTER_FFUNC( xoico_get_hash, xoico_feature_s_get_hash );
            BCORE_REGISTER_FFUNC( xoico_parse, xoico_feature_s_parse );
            BCORE_REGISTER_FFUNC( xoico_finalize, xoico_feature_s_finalize );
            BCORE_REGISTER_FFUNC( xoico_expand_forward, xoico_feature_s_expand_forward );
            BCORE_REGISTER_FFUNC( xoico_expand_indef_typedef, xoico_feature_s_expand_indef_typedef );
            BCORE_REGISTER_FFUNC( xoico_expand_spect_declaration, xoico_feature_s_expand_spect_declaration );
            BCORE_REGISTER_FFUNC( xoico_expand_spect_definition, xoico_feature_s_expand_spect_definition );
            BCORE_REGISTER_FFUNC( xoico_expand_indef_declaration, xoico_feature_s_expand_indef_declaration );
            BCORE_REGISTER_FFUNC( xoico_expand_definition, xoico_feature_s_expand_definition );
            BCORE_REGISTER_FFUNC( xoico_expand_init1, xoico_feature_s_expand_init1 );
            BCORE_REGISTER_OBJECT( xoico_feature_s );
            XOILA_REGISTER_SPECT( xoico_feature );

            // --------------------------------------------------------------------
            // source: xoico_features.x

            // group: xoico_features
            BCORE_REGISTER_OBJECT( xoico_features_s );
            XOILA_REGISTER_SPECT( xoico_features );

            // --------------------------------------------------------------------
            // source: xoico_embedded_data.x

            // group: xoico_embedded_data
            BCORE_REGISTER_FFUNC( xoico_parse, xoico_embedded_data_s_parse );
            BCORE_REGISTER_FFUNC( xoico_get_hash, xoico_embedded_data_s_get_hash );
            BCORE_REGISTER_FFUNC( xoico_expand_declaration, xoico_embedded_data_s_expand_declaration );
            BCORE_REGISTER_FFUNC( xoico_expand_definition, xoico_embedded_data_s_expand_definition );
            BCORE_REGISTER_OBJECT( xoico_embedded_data_s );
            XOILA_REGISTER_SPECT( xoico_embedded_data );

            // --------------------------------------------------------------------
            // source: xoico_group.x

            // group: xoico_group
            BCORE_REGISTER_NAME( x_inst_main );
            BCORE_REGISTER_NAME( x_inst_main_c );
            BCORE_REGISTER_NAME( group_signal_init1 );
            BCORE_REGISTER_NAME( group_signal_down1 );
            BCORE_REGISTER_OBJECT( xoico_group_source_stack_s );
            BCORE_REGISTER_FFUNC( xoico_get_global_name_tp, xoico_group_s_get_global_name_tp );
            BCORE_REGISTER_FFUNC( xoico_expand_setup, xoico_group_s_expand_setup );
            BCORE_REGISTER_FFUNC( xoico_expand_manifesto, xoico_group_s_expand_manifesto );
            BCORE_REGISTER_FFUNC( xoico_host_compiler, xoico_group_s_compiler );
            BCORE_REGISTER_FFUNC( xoico_host_cengine, xoico_group_s_cengine );
            BCORE_REGISTER_FFUNC( xoico_host_obj_type, xoico_group_s_obj_type );
            BCORE_REGISTER_FFUNC( xoico_host_transient_map, xoico_group_s_transient_map );
            BCORE_REGISTER_FFUNC( xoico_get_source_point, xoico_group_s_get_source_point );
            BCORE_REGISTER_FFUNC( xoico_get_hash, xoico_group_s_get_hash );
            BCORE_REGISTER_FFUNC( xoico_host_create_spect_name, xoico_group_s_create_spect_name );
            BCORE_REGISTER_FFUNC( xoico_host_parse_name_st, xoico_group_s_parse_name_st );
            BCORE_REGISTER_FFUNC( xoico_host_parse_name_tp, xoico_group_s_parse_name_tp );
            BCORE_REGISTER_FFUNC( xoico_finalize, xoico_group_s_finalize );
            BCORE_REGISTER_OBJECT( xoico_group_s );
            BCORE_REGISTER_FFUNC( xoico_get_hash, xoico_nested_group_s_get_hash );
            BCORE_REGISTER_FFUNC( xoico_expand_forward, xoico_nested_group_s_expand_forward );
            BCORE_REGISTER_FFUNC( xoico_expand_indef_declaration, xoico_nested_group_s_expand_indef_declaration );
            BCORE_REGISTER_FFUNC( xoico_get_source_point, xoico_nested_group_s_get_source_point );
            BCORE_REGISTER_OBJECT( xoico_nested_group_s );
            XOILA_REGISTER_SPECT( xoico_group );

            // --------------------------------------------------------------------
            // source: xoico_wrap.x

            // group: xoico_wrap
            BCORE_REGISTER_FFUNC( xoico_parse, xoico_wrap_s_parse );
            BCORE_REGISTER_FFUNC( xoico_finalize, xoico_wrap_s_finalize );
            BCORE_REGISTER_FFUNC( xoico_get_source_point, xoico_wrap_s_get_source_point );
            BCORE_REGISTER_OBJECT( xoico_wrap_s );
            BCORE_REGISTER_OBJECT( xoico_wraps_s );
            XOILA_REGISTER_SPECT( xoico_wrap );

            // --------------------------------------------------------------------
            // source: xoico_stamp.x

            // group: xoico_stamp
            BCORE_REGISTER_FFUNC( xoico_get_hash, xoico_stamp_s_get_hash );
            BCORE_REGISTER_FFUNC( xoico_get_global_name_tp, xoico_stamp_s_get_global_name_tp );
            BCORE_REGISTER_FFUNC( xoico_expand_setup, xoico_stamp_s_expand_setup );
            BCORE_REGISTER_FFUNC( xoico_expand_forward, xoico_stamp_s_expand_forward );
            BCORE_REGISTER_FFUNC( xoico_expand_indef_declaration, xoico_stamp_s_expand_indef_declaration );
            BCORE_REGISTER_FFUNC( xoico_host_parse_name_st, xoico_stamp_s_parse_name_st );
            BCORE_REGISTER_FFUNC( xoico_host_compiler, xoico_stamp_s_compiler );
            BCORE_REGISTER_FFUNC( xoico_host_cengine, xoico_stamp_s_cengine );
            BCORE_REGISTER_FFUNC( xoico_host_obj_type, xoico_stamp_s_obj_type );
            BCORE_REGISTER_FFUNC( xoico_host_transient_map, xoico_stamp_s_transient_map );
            BCORE_REGISTER_FFUNC( xoico_get_source_point, xoico_stamp_s_get_source_point );
            BCORE_REGISTER_FFUNC( xoico_parse, xoico_stamp_s_parse );
            BCORE_REGISTER_FFUNC( xoico_finalize, xoico_stamp_s_finalize );
            BCORE_REGISTER_FFUNC( xoico_expand_declaration, xoico_stamp_s_expand_declaration );
            BCORE_REGISTER_FFUNC( xoico_expand_definition, xoico_stamp_s_expand_definition );
            BCORE_REGISTER_FFUNC( xoico_expand_init1, xoico_stamp_s_expand_init1 );
            BCORE_REGISTER_OBJECT( xoico_stamp_s );
            BCORE_REGISTER_OBJECT( xoico_stamp_self_item_s );
            BCORE_REGISTER_OBJECT( xoico_stamp_arr_self_item_s );
            BCORE_REGISTER_FUNC( xoico_stamp_create_embedded_string );
            BCORE_REGISTER_FUNC( xoico_stamp_create_structured_multiline_string );
            XOILA_REGISTER_SPECT( xoico_stamp );

            // --------------------------------------------------------------------
            // source: xoico_source.x

            // group: xoico_source
            BCORE_REGISTER_FFUNC( xoico_expand_setup, xoico_source_s_expand_setup );
            BCORE_REGISTER_FFUNC( xoico_get_hash, xoico_source_s_get_hash );
            BCORE_REGISTER_FFUNC( xoico_finalize, xoico_source_s_finalize );
            BCORE_REGISTER_FFUNC( xoico_expand_declaration, xoico_source_s_expand_declaration );
            BCORE_REGISTER_FFUNC( xoico_expand_definition, xoico_source_s_expand_definition );
            BCORE_REGISTER_FFUNC( xoico_expand_init1, xoico_source_s_expand_init1 );
            BCORE_REGISTER_FFUNC( xoico_expand_down1, xoico_source_s_expand_down1 );
            BCORE_REGISTER_FFUNC( xoico_expand_manifesto, xoico_source_s_expand_manifesto );
            BCORE_REGISTER_FFUNC( xoico_host_compiler, xoico_source_s_compiler );
            BCORE_REGISTER_OBJECT( xoico_source_s );
            XOILA_REGISTER_SPECT( xoico_source );

            // --------------------------------------------------------------------
            // source: xoico_target.x

            // group: xoico_target
            BCORE_REGISTER_FFUNC( xoico_finalize, xoico_target_s_finalize );
            BCORE_REGISTER_FFUNC( xoico_expand_setup, xoico_target_s_expand_setup );
            BCORE_REGISTER_FFUNC( xoico_host_compiler, xoico_target_s_compiler );
            BCORE_REGISTER_FFUNC( xoico_host_cengine, xoico_target_s_cengine );
            BCORE_REGISTER_OBJECT( xoico_target_s );
            BCORE_REGISTER_FUNC( xoico_target_write_with_signature );
            XOILA_REGISTER_SPECT( xoico_target );

            // --------------------------------------------------------------------
            // source: xoico_compiler.x

            // group: xoico_compiler
            BCORE_REGISTER_OBJECT( xoico_compiler_type_info_s );
            BCORE_REGISTER_OBJECT( xoico_compiler_element_info_s );
            BCORE_REGISTER_NAME( as_header );
            BCORE_REGISTER_NAME( as_string );
            BCORE_REGISTER_NAME( as_group );
            BCORE_REGISTER_FFUNC( bcore_inst_call_init_x, xoico_compiler_s_init_x );
            BCORE_REGISTER_FFUNC( xoico_finalize, xoico_compiler_s_finalize );
            BCORE_REGISTER_FFUNC( xoico_expand_setup, xoico_compiler_s_expand_setup );
            BCORE_REGISTER_OBJECT( xoico_compiler_s );
            BCORE_REGISTER_FUNC( xoico_compiler_is_correctly_signed );
            BCORE_REGISTER_FUNC( xoico_compiler_body_signature );
            XOILA_REGISTER_SPECT( xoico_compiler );

            // --------------------------------------------------------------------
            // source: xoico_cengine.x

            // group: xoico_cengine
            BCORE_REGISTER_FEATURE( xoico_cengine_translate );
            BCORE_REGISTER_FEATURE( xoico_cengine_is_reserved );
            BCORE_REGISTER_FFUNC( xoico_cengine_is_reserved, xoico_cengine_is_reserved_default );
            XOILA_REGISTER_SPECT( xoico_cengine );

            // --------------------------------------------------------------------
            // source: xoico_che.x

            // group: xoico_che
            BCORE_REGISTER_NAME( static );
            BCORE_REGISTER_NAME( volatile );
            BCORE_REGISTER_NAME( cast );
            BCORE_REGISTER_NAME( verbatim_C );
            BCORE_REGISTER_NAME( scope );
            BCORE_REGISTER_NAME( t_scope );
            BCORE_REGISTER_NAME( scope_local );
            BCORE_REGISTER_NAME( scope_func );
            BCORE_REGISTER_NAME( fork );
            BCORE_REGISTER_NAME( try );
            BCORE_REGISTER_NAME( if );
            BCORE_REGISTER_NAME( else );
            BCORE_REGISTER_NAME( while );
            BCORE_REGISTER_NAME( do );
            BCORE_REGISTER_NAME( for );
            BCORE_REGISTER_NAME( foreach );
            BCORE_REGISTER_NAME( switch );
            BCORE_REGISTER_NAME( case );
            BCORE_REGISTER_NAME( default );
            BCORE_REGISTER_NAME( break );
            BCORE_REGISTER_NAME( return );
            BCORE_REGISTER_NAME( completion );
            BCORE_REGISTER_NAME( continue );
            BCORE_REGISTER_NAME( goto );
            BCORE_REGISTER_NAME( true );
            BCORE_REGISTER_NAME( false );
            BCORE_REGISTER_FFUNC( xoico_get_hash, xoico_che_s_get_hash );
            BCORE_REGISTER_FFUNC( xoico_cengine_is_reserved, xoico_che_s_is_reserved );
            BCORE_REGISTER_FFUNC( xoico_cengine_translate, xoico_che_s_translate );
            BCORE_REGISTER_OBJECT( xoico_che_s );
            XOILA_REGISTER_SPECT( xoico_che );

            // group: xoico_che_result
            BCORE_REGISTER_FEATURE( xoico_che_result_clear );
            BCORE_REGISTER_FFUNC( xoico_che_result_clear, xoico_che_result_clear_default );
            BCORE_REGISTER_FEATURE( xoico_che_result_push_char );
            BCORE_REGISTER_FFUNC( xoico_che_result_push_char, xoico_che_result_push_char_default );
            BCORE_REGISTER_FEATURE( xoico_che_result_push_sc );
            BCORE_REGISTER_FFUNC( xoico_che_result_push_sc, xoico_che_result_push_sc_default );
            BCORE_REGISTER_FEATURE( xoico_che_result_push_st );
            BCORE_REGISTER_FFUNC( xoico_che_result_push_st, xoico_che_result_push_st_default );
            BCORE_REGISTER_FEATURE( xoico_che_result_push_result_c );
            BCORE_REGISTER_FFUNC( xoico_che_result_push_result_c, xoico_che_result_push_result_c_default );
            BCORE_REGISTER_FEATURE( xoico_che_result_push_result_d );
            BCORE_REGISTER_FFUNC( xoico_che_result_push_result_d, xoico_che_result_push_result_d_default );
            BCORE_REGISTER_FEATURE( xoico_che_result_activate );
            BCORE_REGISTER_FFUNC( xoico_che_result_activate, xoico_che_result_activate_default );
            BCORE_REGISTER_FEATURE( xoico_che_result_deactivate );
            BCORE_REGISTER_FFUNC( xoico_che_result_deactivate, xoico_che_result_deactivate_default );
            BCORE_REGISTER_FEATURE( xoico_che_result_to_sink );
            BCORE_REGISTER_FEATURE( xoico_che_result_set_parent_block );
            BCORE_REGISTER_FFUNC( xoico_che_result_set_parent_block, xoico_che_result_set_parent_block_default );
            BCORE_REGISTER_FEATURE( xoico_che_result_get_cast );
            BCORE_REGISTER_FFUNC( xoico_che_result_get_cast, xoico_che_result_get_cast_default );
            BCORE_REGISTER_FEATURE( xoico_che_result_create_st );
            BCORE_REGISTER_FFUNC( xoico_che_result_create_st, xoico_che_result_create_st_default );
            BCORE_REGISTER_FFUNC( xoico_che_result_to_sink, xoico_che_result_whitespace_s_to_sink );
            BCORE_REGISTER_OBJECT( xoico_che_result_whitespace_s );
            BCORE_REGISTER_FFUNC( xoico_che_result_clear, xoico_che_result_plain_s_clear );
            BCORE_REGISTER_FFUNC( xoico_che_result_push_char, xoico_che_result_plain_s_push_char );
            BCORE_REGISTER_FFUNC( xoico_che_result_push_sc, xoico_che_result_plain_s_push_sc );
            BCORE_REGISTER_FFUNC( xoico_che_result_push_st, xoico_che_result_plain_s_push_st );
            BCORE_REGISTER_FFUNC( xoico_che_result_to_sink, xoico_che_result_plain_s_to_sink );
            BCORE_REGISTER_FFUNC( xoico_che_result_create_st, xoico_che_result_plain_s_create_st );
            BCORE_REGISTER_OBJECT( xoico_che_result_plain_s );
            BCORE_REGISTER_OBJECT( xoico_che_result_adl_s );
            BCORE_REGISTER_FFUNC( xoico_che_result_clear, xoico_che_result_arr_s_clear );
            BCORE_REGISTER_FFUNC( xoico_che_result_activate, xoico_che_result_arr_s_activate );
            BCORE_REGISTER_FFUNC( xoico_che_result_deactivate, xoico_che_result_arr_s_deactivate );
            BCORE_REGISTER_FFUNC( xoico_che_result_push_char, xoico_che_result_arr_s_push_char );
            BCORE_REGISTER_FFUNC( xoico_che_result_push_sc, xoico_che_result_arr_s_push_sc );
            BCORE_REGISTER_FFUNC( xoico_che_result_push_st, xoico_che_result_arr_s_push_st );
            BCORE_REGISTER_FFUNC( xoico_che_result_push_result_d, xoico_che_result_arr_s_push_result_d );
            BCORE_REGISTER_FFUNC( xoico_che_result_push_result_c, xoico_che_result_arr_s_push_result_c );
            BCORE_REGISTER_FFUNC( xoico_che_result_to_sink, xoico_che_result_arr_s_to_sink );
            BCORE_REGISTER_FFUNC( xoico_che_result_set_parent_block, xoico_che_result_arr_s_set_parent_block );
            BCORE_REGISTER_FFUNC( xoico_che_result_get_cast, xoico_che_result_arr_s_get_cast );
            BCORE_REGISTER_OBJECT( xoico_che_result_arr_s );
            BCORE_REGISTER_FFUNC( xoico_che_result_clear, xoico_che_result_block_s_clear );
            BCORE_REGISTER_FFUNC( xoico_che_result_push_char, xoico_che_result_block_s_push_char );
            BCORE_REGISTER_FFUNC( xoico_che_result_push_sc, xoico_che_result_block_s_push_sc );
            BCORE_REGISTER_FFUNC( xoico_che_result_push_st, xoico_che_result_block_s_push_st );
            BCORE_REGISTER_FFUNC( xoico_che_result_push_result_d, xoico_che_result_block_s_push_result_d );
            BCORE_REGISTER_FFUNC( xoico_che_result_push_result_c, xoico_che_result_block_s_push_result_c );
            BCORE_REGISTER_FFUNC( xoico_che_result_to_sink, xoico_che_result_block_s_to_sink );
            BCORE_REGISTER_FFUNC( xoico_che_result_set_parent_block, xoico_che_result_block_s_set_parent_block );
            BCORE_REGISTER_OBJECT( xoico_che_result_block_s );
            BCORE_REGISTER_FFUNC( xoico_che_result_to_sink, xoico_che_result_blm_init_s_to_sink );
            BCORE_REGISTER_FFUNC( xoico_che_result_activate, xoico_che_result_blm_init_s_activate );
            BCORE_REGISTER_FFUNC( xoico_che_result_deactivate, xoico_che_result_blm_init_s_deactivate );
            BCORE_REGISTER_OBJECT( xoico_che_result_blm_init_s );
            BCORE_REGISTER_FFUNC( xoico_che_result_to_sink, xoico_che_result_blm_down_s_to_sink );
            BCORE_REGISTER_FFUNC( xoico_che_result_activate, xoico_che_result_blm_down_s_activate );
            BCORE_REGISTER_FFUNC( xoico_che_result_deactivate, xoico_che_result_blm_down_s_deactivate );
            BCORE_REGISTER_OBJECT( xoico_che_result_blm_down_s );
            BCORE_REGISTER_FFUNC( xoico_che_result_activate, xoico_che_result_cast_s_activate );
            BCORE_REGISTER_FFUNC( xoico_che_result_deactivate, xoico_che_result_cast_s_deactivate );
            BCORE_REGISTER_FFUNC( xoico_che_result_get_cast, xoico_che_result_cast_s_get_cast );
            BCORE_REGISTER_FFUNC( xoico_che_result_set_parent_block, xoico_che_result_cast_s_set_parent_block );
            BCORE_REGISTER_FFUNC( xoico_che_result_to_sink, xoico_che_result_cast_s_to_sink );
            BCORE_REGISTER_OBJECT( xoico_che_result_cast_s );
            BCORE_REGISTER_FFUNC( xoico_che_result_get_cast, xoico_che_result_statement_s_get_cast );
            BCORE_REGISTER_FFUNC( xoico_che_result_set_parent_block, xoico_che_result_statement_s_set_parent_block );
            BCORE_REGISTER_FFUNC( xoico_che_result_to_sink, xoico_che_result_statement_s_to_sink );
            BCORE_REGISTER_OBJECT( xoico_che_result_statement_s );
            BCORE_REGISTER_FFUNC( xoico_che_result_set_parent_block, xoico_che_result_break_s_set_parent_block );
            BCORE_REGISTER_FFUNC( xoico_che_result_to_sink, xoico_che_result_break_s_to_sink );
            BCORE_REGISTER_OBJECT( xoico_che_result_break_s );
            BCORE_REGISTER_FFUNC( xoico_che_result_set_parent_block, xoico_che_result_return_s_set_parent_block );
            BCORE_REGISTER_FFUNC( xoico_che_result_to_sink, xoico_che_result_return_s_to_sink );
            BCORE_REGISTER_OBJECT( xoico_che_result_return_s );
            BCORE_REGISTER_FUNC( xoico_che_result_create_from_st );
            BCORE_REGISTER_FUNC( xoico_che_result_create_from_sc );
            BCORE_REGISTER_FUNC( xoico_che_result_push_fv );
            BCORE_REGISTER_FUNC( xoico_che_result_push_fa );
            BCORE_REGISTER_FUNC( xoico_che_result_copy_fv );
            BCORE_REGISTER_FUNC( xoico_che_result_copy_fa );
            XOILA_REGISTER_SPECT( xoico_che_result );

            // group: xoico_che_stack_var
            BCORE_REGISTER_OBJECT( xoico_che_stack_var_unit_s );
            BCORE_REGISTER_OBJECT( xoico_che_stack_var_unit_adl_s );
            BCORE_REGISTER_OBJECT( xoico_che_stack_var_s );
            XOILA_REGISTER_SPECT( xoico_che_stack_var );

            // group: xoico_che_stack_block
            BCORE_REGISTER_OBJECT( xoico_che_stack_block_unit_s );
            BCORE_REGISTER_OBJECT( xoico_che_stack_block_unit_adl_s );
            BCORE_REGISTER_OBJECT( xoico_che_stack_block_s );
            XOILA_REGISTER_SPECT( xoico_che_stack_block );

            // --------------------------------------------------------------------
            // source: xoico_builder.x

            // group: xoico_builder
            BCORE_REGISTER_OBJECT( xoico_builder_arr_target_s );
            BCORE_REGISTER_FFUNC( bcore_via_call_source, xoico_builder_target_s_source );
            BCORE_REGISTER_FFUNC( xoico_get_hash, xoico_builder_target_s_get_hash );
            BCORE_REGISTER_OBJECT( xoico_builder_target_s );
            BCORE_REGISTER_OBJECT( xoico_builder_main_s );
            XOILA_REGISTER_SPECT( xoico_builder );

            // --------------------------------------------------------------------
            // source: xoico_main.x

            // group: xoico_main
            BCORE_REGISTER_FUNC( xoico_main_help );
            BCORE_REGISTER_FUNC( xoico_main_main );
            XOILA_REGISTER_SPECT( xoico_main );
        }
        break;
        case TYPEOF_down1:
        {
        }
        break;
        case TYPEOF_push_dependencies:
        {
            ASSERT( o->object && ( *( aware_t* )o->object ) == TYPEOF_bcore_arr_fp_s );
            bcore_arr_fp_s* arr_fp = o->object;
            bcore_arr_fp_s_push( arr_fp, ( fp_t )bcore_general_signal_handler );
        }
        break;
        default: break;
    }
    return NULL;
}
BETH_SIGNAL_DEFINE( xoico )

int main( int argc, char** argv )
{
    BETH_USE( xoico );
    bcore_arr_st_s* args = bcore_arr_st_s_create();
    for( sz_t i = 0; i < argc; i++ ) bcore_arr_st_s_push_sc( args, argv[ i ] );
    int retv = xoico_main_main( args );
    bcore_arr_st_s_discard( args );
    BETH_CLOSEV( 0 );
    return retv;
}
// XOICO_BODY_SIGNATURE 0x1728BDA7F976759F
// XOICO_FILE_SIGNATURE 0x1686F304E9169946
