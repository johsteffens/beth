/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_bml.h"
#include "bcore_life.h"
#include "bcore_spect_via.h"
#include "bcore_spect_sink.h"
#include "bcore_spect_source.h"
#include "bcore_spect_array.h"
#include "bcore_quicktypes.h"
#include "bcore_sources.h"
#include "bcore_sinks.h"
#include "bcore_spect_translator.h"
#include "bcore_spect_interpreter.h"
#include "bcore_spect_compare.h"

/**********************************************************************************************************************/

void bcore_bml_translator_s_init( bcore_bml_translator_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = typeof( "bcore_bml_translator_s" );
    o->tab_size = 4;
    o->suppress_aware = true;
    o->break_leaf = false;
}

void bcore_bml_translator_s_down( bcore_bml_translator_s* o )
{
    bcore_inst_aware_down( o );
}

void bcore_bml_translator_s_copy( bcore_bml_translator_s* o, const bcore_bml_translator_s* src )
{
    bcore_inst_aware_copy( o, src );
}

bcore_bml_translator_s* bcore_bml_translator_s_create()
{
    return bcore_inst_typed_create( typeof( "bcore_bml_translator_s" ) );
}

bcore_bml_translator_s* bcore_bml_translator_s_clone( const bcore_bml_translator_s* o )
{
    return bcore_inst_aware_clone( o );
}

void bcore_bml_translator_s_discard( bcore_bml_translator_s* o )
{
    bcore_inst_aware_discard( o );
}

static bcore_flect_self_s* translator_s_create_self( void )
{
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( "bcore_bml_translator_s = { aware_t _; sz_t tab_size; bl_t suppress_aware; bl_t break_leaf;}", sizeof( bcore_bml_translator_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_bml_translator_s_init,        "bcore_fp_init",             "init"             );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_bml_translator_s_translate_object, "bcore_fp_translate_object", "translate_object" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_bml_translator_s_translate_body,   "bcore_fp_translate_body",   "translate_body"   );
    return self;
}

/**********************************************************************************************************************/

static void translate_object( const bcore_bml_translator_s* o, tp_t type, vc_t obj, vd_t dst, sz_t indent );

static void translate_body( const bcore_bml_translator_s* o, tp_t type, vc_t obj, vd_t dst, sz_t indent )
{
    const bcore_sink_s*       snk   = bcore_sink_s_get_aware( dst );
    const bcore_translator_s* trans = bcore_translator_s_get_typed( o->_, type );
    const bcore_inst_s*       inst  = bcore_inst_s_get_typed( type );

    if( (fp_t)trans->translate_body != (fp_t)bcore_bml_translator_s_translate_body )
    {
        trans->translate_body( o, type, obj, dst );
    }
    else if( type == TYPEOF_bcore_string_s ) // treat string as leaf type
    {
        const bcore_string_s* s0 = obj;
        bcore_string_s* s1 = bcore_string_s_clone( s0 );
        bcore_string_s_replace_char_sc( s1, '\"', "\\\"" );
        if( o->break_leaf )
        {
            bcore_sink_spect_push_char( snk, dst, '\n' );
            for( sz_t i = 0; i < o->tab_size * indent; i++ ) bcore_sink_spect_push_char( snk, dst, ' ' );
        }
        bcore_sink_spect_push_char( snk, dst, '"' );
        bcore_sink_spect_push_string_d( snk, dst, s1 );
        bcore_sink_spect_push_char( snk, dst, '"' );
    }
    else if( inst->body )
    {
        bcore_string_s* prefix = bcore_string_s_create_sc( "\n" );
        bcore_string_s_push_char_n( prefix, ' ', o->tab_size * indent );
        bcore_sink_spect_pushf( snk, dst, "%s{", prefix->sc );
        bcore_string_s_push_char_n( prefix, ' ', o->tab_size );

        for( sz_t i = 0; i < inst->body->size; i++ )
        {
            const bcore_inst_item_s* inst_item = &inst->body->data[ i ];
            if( inst_item->no_trace ) continue;

            const bcore_flect_item_s* flect_item = inst_item->flect_item;

            if( i == 0 && o->suppress_aware )
            {
                if( flect_item->type == TYPEOF_aware_t ) continue;
            }

            vc_t element = ( u0_t* )obj + inst_item->offset;
            bcore_sink_spect_pushf( snk, dst, "%s%s", prefix->sc, nameof( flect_item->name ) );

            switch( flect_item->caps )
            {
                case BCORE_CAPS_STATIC:
                {
                    bcore_sink_spect_pushf( snk, dst, ":" );
                    translate_body( o, flect_item->type, element, dst, indent + 1 );
                }
                break;

                case BCORE_CAPS_STATIC_LINK:
                {
                    const bcore_static_link_s* src = element;
                    bcore_sink_spect_pushf( snk, dst, "=" );
                    translate_object( o, flect_item->type, src->link, dst, indent + 1 );
                }
                break;

                case BCORE_CAPS_TYPED_LINK:
                {
                    const bcore_typed_link_s* src = element;
                    tp_t src_type = src->type;
                    if( src_type ) bcore_sink_spect_pushf( snk, dst, "!%s", nameof( src_type ) );
                    bcore_sink_spect_pushf( snk, dst, "=" );
                    translate_object( o, src_type, src->link, dst, indent + 1 );
                }
                break;

                case BCORE_CAPS_AWARE_LINK:
                {
                    const bcore_aware_link_s* src = element;
                    bcore_sink_spect_pushf( snk, dst, "=" );
                    tp_t src_type  = src->link ? *( aware_t* )src->link : 0;
                    translate_object( o, src_type, src->link, dst, indent + 1 );
                }
                break;

                case BCORE_CAPS_STATIC_ARRAY:
                {
                    const bcore_static_array_s* src = element;
                    bcore_sink_spect_pushf( snk, dst, "[%zu]:", src->size );
                    tp_t src_type = flect_item->type;
                    sz_t unit_size = src_type ? bcore_inst_s_get_typed( src_type )->size : 0;
                    vc_t item = src->data;
                    for( sz_t i = 0; i < src->size; i++ )
                    {
                        if( i > 0 ) bcore_sink_spect_push_char( snk, dst, ',' );
                        translate_body( o, src_type, item, dst, indent + 1 );
                        item = ( u0_t* )item + unit_size;
                    }
                    bcore_sink_spect_push_char( snk, dst, ';' );
                }
                break;

                case BCORE_CAPS_TYPED_ARRAY:
                {
                    const bcore_typed_array_s* src = element;
                    tp_t src_type = src->type;
                    if( src_type ) bcore_sink_spect_pushf( snk, dst, "!%s", nameof( src_type ) );
                    bcore_sink_spect_pushf( snk, dst, "[%zu]:", src->size );
                    sz_t unit_size = src_type ? bcore_inst_s_get_typed( src_type )->size : 0;
                    vc_t item = src->data;
                    for( sz_t i = 0; i < src->size; i++ )
                    {
                        if( i > 0 ) bcore_sink_spect_push_char( snk, dst, ',' );
                        translate_body( o, src_type, item, dst, indent + 1 );
                        item = ( u0_t* )item + unit_size;
                    }
                    bcore_sink_spect_push_char( snk, dst, ';' );
                }
                break;

                case BCORE_CAPS_STATIC_LINK_ARRAY:
                {
                    const bcore_static_link_array_s* src = element;
                    bcore_sink_spect_pushf( snk, dst, "[%zu]:", src->size );
                    tp_t src_type = flect_item->type;
                    for( sz_t i = 0; i < src->size; i++ )
                    {
                        if( i > 0 ) bcore_sink_spect_push_char( snk, dst, ',' );
                        translate_object( o, src_type, src->data[ i ], dst, indent + 1 );
                    }
                    bcore_sink_spect_push_char( snk, dst, ';' );
                }
                break;

                case BCORE_CAPS_TYPED_LINK_ARRAY:
                {
                    const bcore_typed_link_array_s* src = element;
                    tp_t src_type = src->type;
                    if( src_type ) bcore_sink_spect_pushf( snk, dst, "!%s", nameof( src_type ) );
                    bcore_sink_spect_pushf( snk, dst, "[%zu]:", src->size );
                    for( sz_t i = 0; i < src->size; i++ )
                    {
                        if( i > 0 ) bcore_sink_spect_push_char( snk, dst, ',' );
                        translate_object( o, src_type, src->data[ i ], dst, indent + 1 );
                    }
                    bcore_sink_spect_push_char( snk, dst, ';' );
                }
                break;

                case BCORE_CAPS_AWARE_LINK_ARRAY:
                {
                    const bcore_static_link_array_s* src = element;
                    bcore_sink_spect_pushf( snk, dst, "[%zu]:", src->size );
                    for( sz_t i = 0; i < src->size; i++ )
                    {
                        if( i > 0 ) bcore_sink_spect_push_char( snk, dst, ',' );
                        tp_t src_type = *( aware_t* )src->data[ i ];
                        translate_object( o, src_type, src->data[ i ], dst, indent + 1 );
                    }
                    bcore_sink_spect_push_char( snk, dst, ';' );
                }
                break;

                default: break;
            }
        }
        bcore_string_s_pop_n( prefix, o->tab_size );
        bcore_sink_spect_pushf( snk, dst, "%s}", prefix->sc );
        bcore_string_s_discard( prefix );
    }
    else // leaf type
    {
        if( o->break_leaf )
        {
            bcore_sink_spect_push_char( snk, dst, '\n' );
            for( sz_t i = 0; i < o->tab_size * indent; i++ ) bcore_sink_spect_push_char( snk, dst, ' ' );
        }
        bcore_sink_spect_push_string_d( snk, dst, bcore_string_s_create_typed( type, obj ) );
    }
}

static void translate_object( const bcore_bml_translator_s* o, tp_t type, vc_t obj, vd_t dst, sz_t indent )
{
    const bcore_sink_s* snk = bcore_sink_s_get_aware( dst );
    if( !obj )
    {
       bcore_sink_spect_pushf( snk, dst, "NULL" );
       return;
    }
    bcore_sink_spect_pushf( snk, dst, "!%s:", nameof( type ) );
    translate_body( o, type, obj, dst, indent );
}

void bcore_bml_translator_s_translate_body( const bcore_bml_translator_s* o, tp_t type, vc_t obj, vd_t fsnk )
{
    translate_body( o, type, obj, fsnk, 0 );
    const bcore_sink_s* snk_p = bcore_sink_s_get_aware( fsnk );
    bcore_sink_spect_pushf( snk_p, fsnk, "\n" );
}

void bcore_bml_translator_s_translate_object( const bcore_bml_translator_s* o, tp_t type, vc_t obj, vd_t fsnk )
{
    translate_object( o, type, obj, fsnk, 0 );
    const bcore_sink_s* snk_p = bcore_sink_s_get_aware( fsnk );
    bcore_sink_spect_pushf( snk_p, fsnk, "\n" );
}

/**********************************************************************************************************************/

void bcore_bml_typed_to_stdout( tp_t type, vc_t obj )
{
    bcore_bml_translator_s* trans = bcore_bml_translator_s_create();
    bcore_string_s* out = bcore_string_s_create();
    bcore_translate_typed_object( trans, type, obj, out );
    bcore_string_s_print_d( out );
    bcore_bml_translator_s_discard( trans );
}

void bcore_bml_aware_to_stdout( vc_t obj )
{
    bcore_bml_typed_to_stdout( *( aware_t* )obj, obj );
}

void bcore_bml_typed_to_file( tp_t type, vc_t obj, sc_t file )
{
    bcore_life_s* l = bcore_life_s_create();
    bcore_sink_chain_s* chain = bcore_life_s_push_aware( l, bcore_sink_chain_s_create() );
    bcore_sink_chain_s_push_d( chain, bcore_sink_file_s_create_name( file ) );
    bcore_sink_chain_s_push_d( chain, bcore_inst_typed_create( typeof( "bcore_sink_buffer_s" ) ) );
    bcore_translate_typed_object( bcore_life_s_push_aware( l, bcore_bml_translator_s_create() ), type, obj, chain );
    bcore_life_s_discard( l );
}

void bcore_bml_aware_to_file( vc_t obj, sc_t file )
{
    bcore_bml_typed_to_file( *( aware_t* )obj, obj, file );
}

void bcore_bml_typed_to_string( tp_t type, vc_t obj, bcore_string_s* string )
{
    bcore_life_s* l = bcore_life_s_create();
    bcore_translate_typed_object( bcore_life_s_push_aware( l, bcore_bml_translator_s_create() ), type, obj, string );
    bcore_life_s_discard( l );
}

void bcore_bml_aware_to_string( vc_t obj, bcore_string_s* string )
{
    bcore_bml_typed_to_string( *( aware_t* )obj, obj, string );
}

/**********************************************************************************************************************/
/// bcore_bml_interpreter
/**********************************************************************************************************************/

void bcore_bml_interpreter_s_init( bcore_bml_interpreter_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = typeof( "bcore_bml_interpreter_s" );
    bcore_inst_typed_check_sizeof( o->_, sizeof( bcore_bml_interpreter_s ) );
}

void bcore_bml_interpreter_s_down( bcore_bml_interpreter_s* o )
{
    bcore_inst_aware_down( o );
}

void bcore_bml_interpreter_s_copy( bcore_bml_interpreter_s* o, const bcore_bml_interpreter_s* src )
{
    bcore_inst_aware_copy( o, src );
}

bcore_bml_interpreter_s* bcore_bml_interpreter_s_create()
{
    return bcore_inst_typed_create( typeof( "bcore_bml_interpreter_s" ) );
}

bcore_bml_interpreter_s* bcore_bml_interpreter_s_clone( const bcore_bml_interpreter_s* o )
{
    return bcore_inst_aware_clone( o );
}

void bcore_bml_interpreter_s_discard( bcore_bml_interpreter_s* o )
{
    bcore_inst_aware_discard( o );
}

static bcore_flect_self_s* interpreter_s_create_self( void )
{
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( "bcore_bml_interpreter_s = { aware_t _; }", sizeof( bcore_bml_interpreter_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_bml_interpreter_s_init,      "bcore_fp_init",                "init"      );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_bml_interpreter_s_interpret_object, "bcore_fp_interpret_object", "interpret_object" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_bml_interpreter_s_interpret_typed,  "bcore_fp_interpret_typed",  "interpret_typed"  );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_bml_interpreter_s_interpret_body,   "bcore_fp_interpret_body",   "interpret_body"   );
    return self;
}

/**********************************************************************************************************************/

static dt_p interpret_object( const bcore_bml_interpreter_s* o, vd_t src );
static dt_p interpret_typed( const bcore_bml_interpreter_s* o, vd_t src, tp_t type );
static void enter_body( const bcore_bml_interpreter_s* o, vd_t src, dt_p dst );

// target object is instantiated and filled with data
static void interpret_body( const bcore_bml_interpreter_s* o, vd_t src, dt_p dst )
{
    const bcore_source_s* src_p = bcore_source_s_get_aware( src );

    if( bcore_source_spect_parse_boolf( src_p, src, " #?'.'" ) ) // stepping outside body
    {
        enter_body( o, src, dst );
        return;
    }

    const bcore_interpreter_s* intrp_p = bcore_interpreter_s_get_typed( o->_, dst.t );
    if( (fp_t)intrp_p->interpret_body != (fp_t)bcore_bml_interpreter_s_interpret_body )
    {
        tp_t type = dst.t;
        vd_t obj  = dst.o;
        intrp_p->interpret_body( o, src, type, obj );
    }
    else if( dst.t == TYPEOF_bcore_string_s ) // treat as leaf type
    {
        if( ( *( aware_t* )dst.o ) != TYPEOF_bcore_string_s )  ERR( "dst.o is incorrect (must be bcore_string_s)" );
        bcore_source_spect_parsef( src_p, src, " #string", dst.o );
    }
    else
    {
        tp_t type = dst.t;
        vd_t obj  = dst.o;
        const bcore_inst_s* inst_p = bcore_inst_s_get_typed( type );
        const bcore_via_s*   via_p = bcore_via_s_get_typed( type );
        if( inst_p->body )
        {
            bcore_source_spect_parsef( src_p, src, " {" );
            while( !bcore_source_spect_parse_boolf( src_p, src, " #?'}'" ) )
            {
                sz_t index = 0;
                {
                    bcore_string_s* s = bcore_string_s_create();
                    bcore_source_spect_parsef( src_p, src, " #name", s );
                    tp_t tname = typeof( s->sc );
                    if( !bcore_name_try_name( tname ) ) bcore_source_spect_parse_errf( src_p, src, "Unknown specifier '%s'.", s->sc );
                    bcore_string_s_discard( s );
                    index = bcore_via_spect_nget_index( via_p, tname );
                }

                const bcore_vitem_s* vitem = bcore_via_spect_iget_vitem( via_p, index );

                vd_t element = ( u0_t* )obj + vitem->offs;
                bool array = false, link = false, typed = false;
                tp_t typed_type = 0;
                switch( vitem->caps )
                {
                    case BCORE_CAPS_STATIC_LINK:               link =         true; break;
                    case BCORE_CAPS_TYPED_LINK:                link = typed = true; break;
                    case BCORE_CAPS_AWARE_LINK:                link =         true; break;
                    case BCORE_CAPS_STATIC_ARRAY:      array =                true; break;
                    case BCORE_CAPS_TYPED_ARRAY:       array =        typed = true; break;
                    case BCORE_CAPS_STATIC_LINK_ARRAY: array = link =         true; break;
                    case BCORE_CAPS_TYPED_LINK_ARRAY:  array = link = typed = true; break;
                    case BCORE_CAPS_AWARE_LINK_ARRAY:  array = link =         true; break;
                    default: break;
                }

                if( bcore_source_spect_parse_boolf( src_p, src, " #?'!'" ) )
                {
                    bcore_string_s* s = bcore_string_s_create();
                    bcore_source_spect_parsef( src_p, src, " #name", s );
                    typed_type = typeof( s->sc );
                    if( !bcore_flect_try_self( typed_type ) ) bcore_source_spect_parse_errf( src_p, src, "Unknown type '%s'.", s->sc );
                    bcore_string_s_discard( s );
                }

                if( array )
                {
                    const bcore_array_s* arr_p = bcore_array_s_get_typed( vitem->type );
                    bcore_array_spect_set_size( arr_p, element, 0 );
                    bcore_source_spect_parsef( src_p, src, " [" );
                    if( !bcore_source_spect_parse_boolf( src_p, src, " #?']'" ) )
                    {
                        sz_t space = 0;
                        bcore_source_spect_parsef( src_p, src, " #sz_t ]", &space );
                        bcore_array_spect_set_space( arr_p, element, space );
                    }
                    bcore_source_spect_parsef( src_p, src, " :" );
                    bcore_array_spect_set_size( arr_p, element, 0 );
                    if( typed ) bcore_array_spect_set_gtype( arr_p, element, typed_type );
                    tp_t type_l = bcore_array_spect_get_mono_type( arr_p, element );
                    while( !bcore_source_spect_parse_boolf( src_p, src, " #?';'" ) )
                    {
                        if( bcore_array_spect_get_size( arr_p, element ) > 0 ) bcore_source_spect_parsef( src_p, src, " ," );
                        if( link )
                        {
                            dt_p obj;
                            if( type_l )
                            {
                                obj = interpret_typed( o, src, type_l );
                            }
                            else
                            {
                                obj = interpret_object( o, src );

                            }
                            bcore_array_spect_push( arr_p, element, sr_tsd( obj.t, obj.o ) );
                        }
                        else
                        {
                            dt_p dst;
                            dst.t = type_l;
                            bcore_array_spect_push( arr_p, element, sr_null() );
                            dst.o = bcore_array_spect_get_last( arr_p, element ).o;

                            interpret_body( o, src, dst );
                        }
                    }
                    if( typed && typed_type == 0 && bcore_array_spect_get_size( arr_p, element ) > 0 )
                    {
                        bcore_source_spect_parse_errf( src_p, src, "Missing type specifier for nonempty typed array '%s'", ifnameof( vitem->name ) );
                    }
                }
                else
                {
                    switch( vitem->caps )
                    {
                        case BCORE_CAPS_STATIC:
                        {
                            dt_p dst = { element, vitem->type };
                            enter_body( o, src, dst );
                        }
                        break;

                        case BCORE_CAPS_STATIC_LINK:
                        {
                            bcore_source_spect_parsef( src_p, src, " =" );
                            ( ( bcore_static_link_s* )element )->link = interpret_typed(  o, src, vitem->type ).o;
                        }
                        break;

                        case BCORE_CAPS_AWARE_LINK:
                        {
                            bcore_source_spect_parsef( src_p, src, " =" );
                            ( ( bcore_aware_link_s* )element )->link = interpret_object( o, src ).o;
                        }
                        break;

                        case BCORE_CAPS_TYPED_LINK:
                        {
                            bcore_source_spect_parsef( src_p, src, " =" );
                            bcore_typed_link_s* dst = element;
                            dst->type = typed_type;
                            dst->link = interpret_typed( o, src, dst->type ).o;
                            if( typed_type == 0 && dst->link != NULL )
                            {
                                bcore_source_spect_parse_errf( src_p, src, "Missing type specifier for nonzero typed object '%s'", ifnameof( vitem->name ) );
                            }
                        }
                        break;
                        default: break;
                    }
                }
                if( index == 0 && vitem->type == TYPEOF_aware_t )
                {
                    if( *(aware_t*)obj != type ) ERR( "Type of aware object '%s' different from expected type '%s'", ifnameof( *(aware_t*)obj ), type );
                }
            }
        }
        else // leaf type
        {
            if( !obj ) ERR( "obj is NULL" );
            bcore_string_s* format = bcore_string_s_createf( " #%s", nameof( type ) );
            bcore_source_spect_parsef( src_p, src, format->sc, obj );
            bcore_string_s_discard( format );
        }
    }
}

static void enter_body( const bcore_bml_interpreter_s* o, vd_t src, dt_p dst )
{
    const bcore_source_s* src_p = bcore_source_s_get_aware( src );
    if( bcore_source_spect_parse_boolf( src_p, src, " #?':'" ) )
    {
        interpret_body( o, src, dst );
    }
    else if( bcore_source_spect_parse_boolf( src_p, src, " #?'<-'" ) )
    {
        dt_p dt_l = interpret_object( o, src );
        bcore_inst_typed_copy_typed( dst.t, dst.o, dt_l.t, dt_l.o );
        bcore_inst_typed_discard( dt_l.t, dt_l.o );
    }
    else
    {
        /* do nothing */
    }
}

// requires instantiation in src or NULL
static dt_p interpret_object( const bcore_bml_interpreter_s* o, vd_t src )
{
    dt_p ret;
    ret.t = 0;
    ret.o = NULL;

    const bcore_source_s* src_p = bcore_source_s_get_aware( src );
    if( bcore_source_spect_parse_boolf( src_p, src, " #?'NULL'" ) ) return ret;

    if( bcore_source_spect_parse_boolf( src_p, src, " #?'!'" ) )
    {
        bcore_string_s* s = bcore_string_s_create();
        bcore_source_spect_parsef( src_p, src, "#name", s );
        ret.t = typeof( s->sc );
        if( !bcore_flect_exists( ret.t ) ) bcore_source_spect_parse_errf( src_p, src, "Unknown type '%s'.", s->sc );
        bcore_string_s_discard( s );
        ret.o = bcore_inst_typed_create( ret.t );
        enter_body( o, src, ret );
    }
    else
    {
        bcore_source_spect_parse_errf( src_p, src, "Instantiation (!<type>) expected." );
    }
    return ret;
}

// target type is given and object of that type is returned
// instantiation in src is optional, NULL allowed
// if instantiation type different from target type, automatic conversion is initiated
static dt_p interpret_typed( const bcore_bml_interpreter_s* o, vd_t src, tp_t type )
{
    dt_p ret;
    const bcore_source_s* src_p = bcore_source_s_get_aware( src );

    ret.t = type;
    ret.o = NULL;

    if( bcore_source_spect_parse_boolf( src_p, src, " #?'NULL'" ) ) return ret;

    if( bcore_source_spect_parse_boolf( src_p, src, " #?'!'" ) )
    {
        ret.o = bcore_inst_typed_create( ret.t );
        bcore_string_s* s = bcore_string_s_create();
        bcore_source_spect_parsef( src_p, src, "#name", s );
        tp_t type_l = typeof( s->sc );
        if( !bcore_flect_exists( type_l ) ) bcore_source_spect_parse_errf( src_p, src, "Unknown type '%s'.", s->sc );
        bcore_string_s_discard( s );

        if( type_l != type )
        {
            dt_p dt_l;
            dt_l.t = type_l;
            dt_l.o = bcore_inst_typed_create( dt_l.t );
            enter_body( o, src, dt_l );
            bcore_inst_typed_copy_typed( ret.t, ret.o, dt_l.t, dt_l.o );
            bcore_inst_typed_discard( dt_l.t, dt_l.o );
        }
        else
        {
            enter_body( o, src, ret );
        }
    }
    else
    {
        ret.o = bcore_inst_typed_create( ret.t );
        interpret_body( o, src, ret );
    }
    return ret;
}

dt_p bcore_bml_interpreter_s_interpret_body( const bcore_bml_interpreter_s* o, vd_t fsrc, tp_t type, vd_t obj )
{
    dt_p dst = { obj, type };
    interpret_body( o, fsrc, dst );
    return dst;
}

dt_p bcore_bml_interpreter_s_interpret_object( const bcore_bml_interpreter_s* o, vd_t fsrc )
{
    return interpret_object( o, fsrc );
}

dt_p bcore_bml_interpreter_s_interpret_typed( const bcore_bml_interpreter_s* o, vd_t fsrc, tp_t type )
{
    return interpret_typed( o, fsrc, type );
}

/**********************************************************************************************************************/

dt_p bcore_bml_object_from_file( sc_t file )
{
    bcore_life_s* l = bcore_life_s_create();
    bcore_source_chain_s* chain = bcore_life_s_push_aware( l, bcore_source_chain_s_create() );
    bcore_source_chain_s_push_d( chain, bcore_source_file_s_create_name( file ) );
    bcore_source_chain_s_push_d( chain, bcore_inst_typed_create( typeof( "bcore_source_string_s" ) ) );
    dt_p dt = bcore_interpret_object( bcore_life_s_push_aware( l, bcore_bml_interpreter_s_create() ), chain );
    bcore_life_s_discard( l );
    return dt;
}

dt_p bcore_bml_object_from_string( const bcore_string_s* string )
{
    bcore_life_s* l = bcore_life_s_create();
    bcore_source_chain_s* chain = bcore_life_s_push_aware( l, bcore_source_chain_s_create() );
    bcore_source_chain_s_push_d( chain, bcore_source_string_s_create_string( string ) );
    dt_p dt = bcore_interpret_object( bcore_life_s_push_aware( l, bcore_bml_interpreter_s_create() ), chain );
    bcore_life_s_discard( l );
    return dt;
}

/**********************************************************************************************************************/

void bcore_bml_define_self_creators( void )
{
    bcore_flect_define_creator( typeof( "bcore_bml_translator_s"  ), translator_s_create_self  );
    bcore_flect_define_creator( typeof( "bcore_bml_interpreter_s" ), interpreter_s_create_self );
}

/**********************************************************************************************************************/

void bcore_bml_transfer_test_typed( tp_t type, vd_t obj )
{
    bcore_life_s* l = bcore_life_s_create();
    bcore_string_s* string = bcore_life_s_push_aware( l, bcore_string_s_create() );
    bcore_bml_typed_to_string( type, obj, string );
    dt_p dt = bcore_bml_object_from_string( string );
    bcore_life_s_push_typed( l, dt.t, dt.o );
    s2_t c = bcore_compare_bityped( type, obj, dt.t, dt.o );
    if( c != 0 )
    {
        bcore_string_s* diff = bcore_life_s_push_aware( l, bcore_diff_bityped( type, obj, dt.t, dt.o ) );
        if( diff )
        {
            ERR( "Comparison returned '%"PRIi32"':\n%s\n", c, diff->sc );
        }
        else
        {
            ERR( "Comparison returned '%"PRIi32"' but diff returned 'NULL'\n", c );
        }
    }
    bcore_life_s_discard( l );
}

void bcore_bml_transfer_test_aware( vd_t obj )
{
    bcore_bml_transfer_test_typed( *( aware_t* )obj, obj );
}

/**********************************************************************************************************************/

static bcore_string_s* translate_selftest( void )
{
    bcore_life_s* l = bcore_life_s_create();
    bcore_string_s* out = bcore_string_s_create();

    bcore_flect_define_parse_sc
    (
        "specs = "
        "{"
        "   aware_t _;"
        "   bcore_string_s* name;"
        "   sz_t size;"
        "   u2_t param1;"
        "   s2_t param2;"
        "   specs * child;"
        "   sz_t [] numarr;"
        "   bcore_string_s [] strarr; bl_t flag;"
        "}"
    );
    bcore_flect_define_parse_sc( "specs_arr = { aware_t _; aware * [] arr; }" );

    vd_t specs = bcore_life_s_push_aware( l, bcore_inst_typed_create( typeof( "specs" ) ) );

    {
        const bcore_via_s * specs_v = bcore_via_s_get_aware( specs );
        bcore_via_spect_nset_u3( specs_v, specs, typeof( "param1" ), 10 );
        bcore_via_spect_nset_s3( specs_v, specs, typeof( "param2" ), -3240 );
        bcore_via_spect_nset(    specs_v, specs, typeof( "name" ), sr_asd( bcore_string_s_createf( "\"my string\"" ) ) );

        {
            const bcore_array_s* numarr_p = bcore_via_spect_nget_array( specs_v, typeof( "numarr" ) );
            vd_t numarr = bcore_via_spect_nget_d( specs_v, specs, typeof( "numarr" ) );
            for( sz_t i = 0; i < 10; i++ ) bcore_array_spect_push_sz( numarr_p, numarr, i );
        }

        {
            const bcore_array_s* strarr_p = bcore_via_spect_nget_array( specs_v, typeof( "strarr" ) );
            vd_t strarr = bcore_via_spect_nget_d( specs_v, specs, typeof( "strarr" ) );
            for( sz_t i = 0; i < 10; i++ ) bcore_array_spect_push( strarr_p, strarr, sr_asd( bcore_string_s_createf( "<%zu>", i ) ) );
        }

        bcore_via_spect_nset( specs_v, specs, typeof( "child"  ), sr_asd( bcore_inst_aware_clone( specs ) ) );
    }

    vd_t specs_arr = bcore_life_s_push_aware( l, bcore_inst_typed_create( typeof( "specs_arr" ) ) );
    {
        const bcore_array_s * arr_p = bcore_array_s_get_aware( specs_arr );
        for( sz_t i = 0; i < 2; i++ )
        {
            bcore_array_spect_push( arr_p, specs_arr, sr_awc( specs ) );
        }
    }

    bcore_bml_translator_s* tbml = bcore_life_s_push_aware( l, bcore_bml_translator_s_create() );

    {
        bcore_string_s* buf1 = bcore_life_s_push_aware( l, bcore_string_s_create() );
        bcore_string_s* buf2 = bcore_life_s_push_aware( l, bcore_string_s_create() );
        bcore_translate_aware_object( tbml, specs_arr, buf1 );
        bcore_bml_interpreter_s* intrp = bcore_life_s_push_aware( l, bcore_bml_interpreter_s_create() );
        bcore_source_string_s* str_src = bcore_life_s_push_aware( l, bcore_source_string_s_create_string( buf1 ) );
        vd_t specs_arr_2 = bcore_life_s_push_aware( l, bcore_interpret_object( intrp, str_src ).o );
        bcore_translate_aware_object( tbml, specs_arr_2, buf2 );
        ASSERT( bcore_string_s_cmp_string( buf1, buf2 ) == 0 );
    }

    bcore_bml_transfer_test_aware( specs_arr );

    bcore_life_s_discard( l );
    return out;
}

static bcore_string_s* interpret_selftest( void )
{
    bcore_life_s* l = bcore_life_s_create();
    bcore_string_s* out = bcore_string_s_create();

    sc_t src =
    "!specs:                                         \n"
    "{                                               \n"
    "    name = !f2_t      // a comment              \n"
    "    /* another                                  \n"
    "       comment                                  \n"
    "    */                                          \n"
    "    param2 <- !f2_t: 1.9                        \n"
    "    param1:3456                                 \n"
    "    flag: true                                  \n"
    "    numarr[]: 4,4,5,6, . <- !f3_t: 3.7;         \n"
    "    strarr[]: \"fluff\", \"quirk\",.<-!s2_t:-6; \n"
    "}";

    bcore_bml_interpreter_s* intrp = bcore_life_s_push_aware( l, bcore_bml_interpreter_s_create() );
    bcore_source_string_s* str_src = bcore_life_s_push_aware( l, bcore_source_string_s_create_sc( src ) );
    vd_t specs                     = bcore_life_s_push_aware( l, bcore_interpret_object( intrp, str_src ).o );
    bcore_bml_translator_s* trans  = bcore_life_s_push_aware( l, bcore_bml_translator_s_create() );
    bcore_translate_aware_object( trans, specs, out );

    bcore_life_s_discard( l );

    return out;
}

bcore_string_s* bcore_bml_selftest( void )
{
    bcore_string_s* log = bcore_string_s_createf( "== bcore_bml_selftest " );
    bcore_string_s_push_char_n( log, '=', 120 - log->size );
    bcore_string_s_push_char( log, '\n' );
    bcore_string_s_push_string_d( log, translate_selftest() );
    bcore_string_s_push_string_d( log, interpret_selftest() );

    bcore_string_s_push_char_n( log, '=', 120 );
    bcore_string_s_push_char( log, '\n' );
    return log;
}
