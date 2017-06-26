/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_bml.h"
#include "bcore_via_perspective.h"
#include "bcore_sink_perspective.h"
#include "bcore_source_perspective.h"
#include "bcore_array_perspective.h"
#include "bcore_quicktypes.h"
#include "bcore_string_source.h"

/**********************************************************************************************************************/

void bcore_bml_translator_s_init( bcore_bml_translator_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = typeof( "bcore_bml_translator_s" );
    o->tab_size = 4;
    o->suppress_aware = true;
    bcore_instance_typed_check_sizeof( o->_, sizeof( bcore_bml_translator_s ) );
}

void bcore_bml_translator_s_down( bcore_bml_translator_s* o )
{
    bcore_instance_aware_down( o );
}

void bcore_bml_translator_s_copy( bcore_bml_translator_s* o, const bcore_bml_translator_s* src )
{
    bcore_instance_aware_copy( o, src );
}

bcore_bml_translator_s* bcore_bml_translator_s_create()
{
    return bcore_instance_typed_create( typeof( "bcore_bml_translator_s" ) );
}

bcore_bml_translator_s* bcore_bml_translator_s_clone( const bcore_bml_translator_s* o )
{
    return bcore_instance_aware_clone( o );
}

void bcore_bml_translator_s_discard( bcore_bml_translator_s* o )
{
    bcore_instance_aware_discard( o );
}

bcore_flect_self_s* bcore_bml_translator_s_create_self()
{
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( "bcore_bml_translator_s = { aware_t _; sz_t tab_size; bool suppress_aware; }"   );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_bml_translator_s_init,             "bcore_fp_init",             "init"      );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_bml_translator_s_translate_object, "bcore_fp_translate_object", "translate_object" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_bml_translator_s_translate_body,   "bcore_fp_translate_body",   "translate_body"   );
    return self;
}

/**********************************************************************************************************************/

static void translate( const bcore_bml_translator_s* o, bool link, tp_t type, vc_t obj, vd_t dst, sz_t indent )
{
    const bcore_instance_s* inst = bcore_instance_s_get_typed( type );
    const bcore_sink_s* snk = bcore_sink_s_get_aware( dst );
    if( !obj )
    {
       snk->pushf( snk, dst, "NULL" );
       return;
    }

    // special types
    if( type == TYPEOF_bcore_string_s ) // treat as leaf type
    {
        if( link ) snk->pushf( snk, dst, "!%s:", nameof( type ) );
        const bcore_string_s* s0 = obj;
        bcore_string_s* s1 = bcore_string_s_clone( s0 );
        bcore_string_s_replace_char_sc( s1, '\"', "\\\"" );
        bcore_string_s_replace_char_sc( s1, '\n', "\\n" );
        bcore_string_s_replace_char_sc( s1, '\r', "\\r" );
        snk->push_char( snk, dst, '"' );
        snk->push_string_d( snk, dst, s1 );
        snk->push_char( snk, dst, '"' );
    }
    else if( inst->body )
    {
        bcore_string_s* prefix = bcore_string_s_create_sc( "\n" );
        bcore_string_s_push_char_n( prefix, ' ', o->tab_size * indent );

        if( link )
        {
            snk->pushf( snk, dst, "%s!%s:%s{ ", prefix->sc, nameof( type ), prefix->sc );
        }
        else
        {
            snk->pushf( snk, dst, "%s{", prefix->sc );
        }

        for( sz_t i = 0; i < inst->body->size; i++ )
        {
            const bcore_instance_item_s* inst_item = &inst->body->data[ i ];
            const bcore_flect_item_s*    flect_item = inst_item->flect_item;

            if( i == 0 && o->suppress_aware )
            {
                if( flect_item->type == TYPEOF_aware_t ) continue;
            }

            vc_t element = ( u0_t* )obj + inst_item->offset;
            snk->pushf( snk, dst, "%s  %s", prefix->sc, nameof( flect_item->name ) );

            switch( flect_item->caps )
            {
                case BCORE_CAPS_STATIC:
                {
                    snk->pushf( snk, dst, ":" );
                    translate( o, false, flect_item->type, element, dst, indent + 1 );
                }
                break;

                case BCORE_CAPS_STATIC_LINK:
                {
                    const bcore_static_link_s* src = element;
                    snk->pushf( snk, dst, "=" );
                    translate( o, true, flect_item->type, src->link, dst, indent + 1 );
                }
                break;

                case BCORE_CAPS_TYPED_LINK:
                {
                    const bcore_typed_link_s* src = element;
                    tp_t src_type = src->type;
                    if( src_type ) snk->pushf( snk, dst, "!%s", nameof( src_type ) );
                    snk->pushf( snk, dst, "=" );
                    translate( o, true, src_type, src->link, dst, indent + 1 );
                }
                break;

                case BCORE_CAPS_AWARE_LINK:
                {
                    const bcore_aware_link_s* src = element;
                    snk->pushf( snk, dst, "=" );
                    tp_t src_type  = src->link ? *( aware_t* )src->link : 0;
                    translate( o, true, src_type, src->link, dst, indent + 1 );
                }
                break;

                case BCORE_CAPS_STATIC_ARRAY:
                {
                    const bcore_static_array_s* src = element;
                    snk->pushf( snk, dst, "[%zu]:", src->size );
                    tp_t src_type = flect_item->type;
                    sz_t unit_size = src_type ? bcore_instance_s_get_typed( src_type )->size : 0;
                    vc_t item = src->data;
                    for( sz_t i = 0; i < src->size; i++ )
                    {
                        translate( o, false, src_type, item, dst, indent + 1 );
                        snk->push_char( snk, dst, ( i + 1 == src->size ) ? ';' : ',' );
                        item = ( u0_t* )item + unit_size;
                    }
                }
                break;

                case BCORE_CAPS_TYPED_ARRAY:
                {
                    const bcore_typed_array_s* src = element;
                    tp_t src_type = src->type;
                    if( src_type ) snk->pushf( snk, dst, "!%s", nameof( src_type ) );
                    snk->pushf( snk, dst, "[%zu]:", src->size );
                    sz_t unit_size = src_type ? bcore_instance_s_get_typed( src_type )->size : 0;
                    vc_t item = src->data;
                    for( sz_t i = 0; i < src->size; i++ )
                    {
                        translate( o, false, src_type, item, dst, indent + 1 );
                        snk->push_char( snk, dst, ( i + 1 == src->size ) ? ';' : ',' );
                        item = ( u0_t* )item + unit_size;
                    }
                }
                break;

                case BCORE_CAPS_STATIC_LINK_ARRAY:
                {
                    const bcore_static_link_array_s* src = element;
                    snk->pushf( snk, dst, "[%zu]:", src->size );
                    tp_t src_type = flect_item->type;
                    for( sz_t i = 0; i < src->size; i++ )
                    {
                        translate( o, true, src_type, src->data[ i ], dst, indent + 1 );
                        snk->push_char( snk, dst, ( i + 1 == src->size ) ? ';' : ',' );
                    }
                }
                break;

                case BCORE_CAPS_TYPED_LINK_ARRAY:
                {
                    const bcore_typed_link_array_s* src = element;
                    tp_t src_type = src->type;
                    if( src_type ) snk->pushf( snk, dst, "!%s", nameof( src_type ) );
                    snk->pushf( snk, dst, "[%zu]:", src->size );
                    for( sz_t i = 0; i < src->size; i++ )
                    {
                        translate( o, true, src_type, src->data[ i ], dst, indent + 1 );
                        snk->push_char( snk, dst, ( i + 1 == src->size ) ? ';' : ',' );
                    }
                }
                break;

                case BCORE_CAPS_AWARE_LINK_ARRAY:
                {
                    const bcore_static_link_array_s* src = element;
                    snk->pushf( snk, dst, "[%zu]:", src->size );
                    for( sz_t i = 0; i < src->size; i++ )
                    {
                        tp_t src_type = *( aware_t* )src->data[ i ];
                        translate( o, true, src_type, src->data[ i ], dst, indent + 1 );
                        snk->push_char( snk, dst, ( i + 1 == src->size ) ? ';' : ',' );
                    }
                }
                break;

                default: break;
            }
        }
        snk->pushf( snk, dst, "%s}", prefix->sc );
        bcore_string_s_discard( prefix );
    }
    else // leaf type
    {
        if( link ) snk->pushf( snk, dst, "!%s: ", nameof( type ) );
        snk->push_string_d( snk, dst, bcore_string_s_create_typed( type, obj ) );
    }
}

void bcore_bml_translator_s_translate_body( const bcore_bml_translator_s* o, tp_t type, vc_t obj, vd_t fsnk )
{
    translate( o, false, type, obj, fsnk, 0 );
    const bcore_sink_s* snk_p = bcore_sink_s_get_aware( fsnk );
    snk_p->pushf( snk_p, fsnk, "\n" );
}

void bcore_bml_translator_s_translate_object( const bcore_bml_translator_s* o, tp_t type, vc_t obj, vd_t fsnk )
{
    const bcore_sink_s* snk_p = bcore_sink_s_get_aware( fsnk );
    snk_p->pushf( snk_p, fsnk, "!%s: ", nameof( type ) );
    bcore_bml_translator_s_translate_body( o, type, obj, fsnk );
}

/**********************************************************************************************************************/

void bcore_bml_interpreter_s_init( bcore_bml_interpreter_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = typeof( "bcore_bml_interpreter_s" );
    bcore_instance_typed_check_sizeof( o->_, sizeof( bcore_bml_interpreter_s ) );
}

void bcore_bml_interpreter_s_down( bcore_bml_interpreter_s* o )
{
    bcore_instance_aware_down( o );
}

void bcore_bml_interpreter_s_copy( bcore_bml_interpreter_s* o, const bcore_bml_interpreter_s* src )
{
    bcore_instance_aware_copy( o, src );
}

bcore_bml_interpreter_s* bcore_bml_interpreter_s_create()
{
    return bcore_instance_typed_create( typeof( "bcore_bml_interpreter_s" ) );
}

bcore_bml_interpreter_s* bcore_bml_interpreter_s_clone( const bcore_bml_interpreter_s* o )
{
    return bcore_instance_aware_clone( o );
}

void bcore_bml_interpreter_s_discard( bcore_bml_interpreter_s* o )
{
    bcore_instance_aware_discard( o );
}

bcore_flect_self_s* bcore_bml_interpreter_s_create_self()
{
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( "bcore_bml_interpreter_s = { aware_t _; }"   );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_bml_interpreter_s_init,             "bcore_fp_init",             "init"             );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_bml_interpreter_s_interpret_object, "bcore_fp_interpret_object", "interpret_object" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_bml_interpreter_s_interpret_typed,  "bcore_fp_interpret_typed",  "interpret_typed" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_bml_interpreter_s_interpret_body,   "bcore_fp_interpret_body",   "interpret_body"   );
    return self;
}

/**********************************************************************************************************************/

static dt_p interpret_object( const bcore_bml_interpreter_s* o, vd_t src );
static dt_p interpret_typed( const bcore_bml_interpreter_s* o, vd_t src, tp_t type );

// target object is instantiated and filled with data
static void interpret_body( const bcore_bml_interpreter_s* o, vd_t src, dt_p dst )
{
    const bcore_source_s* src_p = bcore_source_s_get_aware( src );

    if( src_p->parse_boolf( src_p, src, " #?'<-'" ) ) // explicit type conversion requested
    {
        dt_p tmp = interpret_object( o, src );
        bcore_instance_typed_copy_typed( dst.t, dst.o, tmp.t, tmp.o );
        bcore_instance_typed_discard( tmp.t, tmp.o );
        return;
    }

    if( dst.t == TYPEOF_bcore_string_s ) // treat as leaf type
    {
        if( ( *( aware_t* )dst.o ) != TYPEOF_bcore_string_s )  ERR( "dst.o is incorrect (must be bcore_string_s)" );
        src_p->parsef( src_p, src, " #string", dst.o );
    }
    else
    {
        tp_t type = dst.t;
        vd_t obj  = dst.o;
        const bcore_instance_s* inst_p = bcore_instance_s_get_typed( type );
        const bcore_via_s* via_p = bcore_via_s_get_typed( type );
        if( inst_p->body )
        {
            src_p->parsef( src_p, src, " {" );
            while( !src_p->parse_boolf( src_p, src, " #?'}'" ) )
            {
                sz_t index = 0;
                {
                    bcore_string_s* s = bcore_string_s_create();
                    src_p->parsef( src_p, src, " #name", s );
                    tp_t tname = typeof( s->sc );
                    if( !bcore_name_try_name( tname ) ) src_p->parse_errf( src_p, src, "Unknown specifier '%s'.", s->sc );
                    bcore_string_s_discard( s );
                    index = via_p->nget_index( via_p, tname );
                }

                const bcore_vitem_s* vitem = via_p->iget_vitem( via_p, index );
                if( index == 0 && vitem->type == TYPEOF_aware_t )
                {
                    if( *(aware_t*)obj != type ) ERR( "Type of aware object '%s' different from expected type '%s'", ifnameof( *(aware_t*)obj ), type );
                    continue;
                }

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

                if( src_p->parse_boolf( src_p, src, " #?'!'" ) )
                {
                    bcore_string_s* s = bcore_string_s_create();
                    src_p->parsef( src_p, src, " #name :", s );
                    typed_type = typeof( s->sc );
                    if( !bcore_flect_try_self( typed_type ) ) src_p->parse_errf( src_p, src, "Unknown type '%s'.", s->sc );
                    bcore_string_s_discard( s );
                }

                if( array )
                {
                    const bcore_array_s* arr_p = bcore_array_s_get_typed( vitem->type );
                    arr_p->set_size( arr_p, element, 0 );
                    src_p->parsef( src_p, src, " [" );
                    if( !src_p->parse_boolf( src_p, src, " #?']'" ) )
                    {
                        sz_t space = 0;
                        src_p->parsef( src_p, src, " #sz_t ]", &space );
                        arr_p->set_space( arr_p, element, space );
                    }
                    src_p->parsef( src_p, src, " :" );
                    arr_p->set_size( arr_p, element, 0 );
                    if( typed ) arr_p->set_type( arr_p, element, typed_type );
                    tp_t type_l = arr_p->get_type( arr_p, element );
                    while( !src_p->parse_boolf( src_p, src, " #?';'" ) )
                    {
                        if( arr_p->get_size( arr_p, element ) > 0 ) src_p->parsef( src_p, src, " ," );
                        if( link )
                        {
                            vd_t obj = NULL;
                            if( type_l )
                            {
                                obj = interpret_typed( o, src, type_l ).o;
                            }
                            else
                            {
                                obj = interpret_object( o, src ).o;

                            }
                            arr_p->push_d( arr_p, element, obj );
                        }
                        else
                        {
                            dt_p dst;
                            dst.t = type_l;
                            dst.o = arr_p->push_c( arr_p, element, NULL );
                            interpret_body( o, src, dst );
                        }
                    }
                    if( typed && typed_type == 0 && arr_p->get_size( arr_p, element ) > 0 )
                    {
                        src_p->parse_errf( src_p, src, "Missing type specifier for nonempty typed array '%s'", ifnameof( vitem->name ) );
                    }
                }
                else
                {
                    src_p->parsef( src_p, src, link ? " =" : " :" );
                    switch( vitem->caps )
                    {
                        case BCORE_CAPS_STATIC:
                        {
                            dt_p dst = { element, vitem->type };
                            interpret_body( o, src, dst );
                        }
                        break;

                        case BCORE_CAPS_STATIC_LINK:
                        {
                            ( ( bcore_static_link_s* )element )->link = interpret_typed(  o, src, vitem->type ).o;
                        }
                        break;

                        case BCORE_CAPS_AWARE_LINK:
                        {
                            ( ( bcore_aware_link_s* )element )->link = interpret_object( o, src ).o;
                        }
                        break;

                        case BCORE_CAPS_TYPED_LINK:
                        {
                            bcore_typed_link_s* dst = element;
                            dst->type = typed_type;
                            dst->link = interpret_typed( o, src, dst->type ).o;
                            if( typed_type == 0 && dst->link != NULL )
                            {
                                src_p->parse_errf( src_p, src, "Missing type specifier for nonzero typed object '%s'", ifnameof( vitem->name ) );
                            }
                        }
                        break;
                        default: break;
                    }
                }
            }
        }
        else // leaf type
        {
            if( !obj ) ERR( "obj is NULL" );
            bcore_string_s* format = bcore_string_s_createf( " #%s", nameof( type ) );
            src_p->parsef( src_p, src, format->sc, obj );
            bcore_string_s_discard( format );
        }
    }
}

// requires instantiation in src or NULL
static dt_p interpret_object( const bcore_bml_interpreter_s* o, vd_t src )
{
    dt_p ret;
    ret.t = 0;
    ret.o = NULL;

    const bcore_source_s* src_p = bcore_source_s_get_aware( src );
    if( src_p->parse_boolf( src_p, src, " #?'NULL'" ) ) return ret;

    if( src_p->parse_boolf( src_p, src, " #?'!'" ) )
    {
        bcore_string_s* s = bcore_string_s_create();
        src_p->parsef( src_p, src, "#name :", s );
        ret.t = typeof( s->sc );
        if( !bcore_flect_exists( ret.t ) ) src_p->parse_errf( src_p, src, "Unknown type '%s'.", s->sc );
        bcore_string_s_discard( s );
        ret.o = bcore_instance_typed_create( ret.t );
        interpret_body( o, src, ret );
    }
    else
    {
        src_p->parse_errf( src_p, src, "Instantiation (!<type>) expected." );
    }
    return ret;
}

// target type is given and object of that type is returned
// instantiation in src is optional, NULL allowed
// if instatiation type different from target type, automatic conversion is initiated
static dt_p interpret_typed( const bcore_bml_interpreter_s* o, vd_t src, tp_t type )
{
    dt_p ret;
    const bcore_source_s* src_p = bcore_source_s_get_aware( src );

    ret.t = type;
    ret.o = NULL;

    if( src_p->parse_boolf( src_p, src, " #?'NULL'" ) ) return ret;

    if( src_p->parse_boolf( src_p, src, " #?'!'" ) )
    {
        ret.o = bcore_instance_typed_create( ret.t );
        bcore_string_s* s = bcore_string_s_create();
        src_p->parsef( src_p, src, "#name :", s );
        tp_t type_l = typeof( s->sc );
        if( !bcore_flect_exists( type_l ) ) src_p->parse_errf( src_p, src, "Unknown type '%s'.", s->sc );
        bcore_string_s_discard( s );

        if( type_l != type )
        {
            dt_p dt_l;
            dt_l.t = type_l;
            dt_l.o = bcore_instance_typed_create( dt_l.t );
            interpret_body( o, src, dt_l );
            bcore_instance_typed_copy_typed( ret.t, ret.o, dt_l.t, dt_l.o );
            bcore_instance_typed_discard( dt_l.t, dt_l.o );
        }
        else
        {
            interpret_body( o, src, ret );
        }
    }
    else
    {
        ret.o = bcore_instance_typed_create( ret.t );
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

static bcore_string_s* translate_selftest()
{
    bcore_life_s* l = bcore_life_s_create();
    bcore_string_s* out = bcore_string_s_create();

    bcore_flect_parse_sc( "specs = { aware_t _; bcore_string_s* name; sz_t size; u2_t param1; s2_t param2; sz_t [] numarr;  bcore_string_s [] strarr; bool flag;}" );
    bcore_flect_parse_sc( "specs_arr = { aware_t _; aware * [] arr; }" );

    vd_t specs = bcore_life_s_push_aware( l, bcore_instance_typed_create( typeof( "specs" ) ) );

    {
        const bcore_via_s * specs_v = bcore_via_s_get_aware( specs );
        u2_t u2_v = 10;
        specs_v->nset_c( specs_v, specs, typeof( "param1" ), &u2_v );
        s3_t s3_v = -3240;
        specs_v->nset_c( specs_v, specs, typeof( "param2" ), &s3_v );

        bcore_string_s* s = specs_v->ncreate( specs_v, specs, typeof( "name" ) );
        bcore_string_s_pushf( s, "\"my string\n\"" );

        {
            const bcore_array_s* numarr_p = bcore_array_s_get_typed( specs_v->nget_type( specs_v, specs, typeof( "numarr" ) ) );
            vd_t numarr = specs_v->nget_d( specs_v, specs, typeof( "numarr" ) );
            for( sz_t i = 0; i < 10; i++ ) numarr_p->push_c( numarr_p, numarr, &i );
        }

        {
            const bcore_array_s* strarr_p = bcore_array_s_get_typed( specs_v->nget_type( specs_v, specs, typeof( "strarr" ) ) );
            vd_t strarr = specs_v->nget_d( specs_v, specs, typeof( "strarr" ) );
            for( sz_t i = 0; i < 10; i++ ) strarr_p->push_d( strarr_p, strarr, bcore_string_s_createf( "<%zu>", i ) );
        }
    }

    vd_t specs_arr = bcore_life_s_push_aware( l, bcore_instance_typed_create( typeof( "specs_arr" ) ) );
    {
        const bcore_array_s * arr_p = bcore_array_s_get_aware( specs_arr );
        for( sz_t i = 0; i < 2; i++ )
        {
            arr_p->push_c( arr_p, specs_arr, specs );
        }
    }

    bcore_bml_translator_s* trans = bcore_life_s_push_aware( l, bcore_bml_translator_s_create() );
    bcore_bml_translator_s_translate_body( trans, typeof( "specs_arr" ), specs_arr, out );
    bcore_bml_translator_s_translate_body( trans, typeof( "bcore_bml_translator_s" ), trans, out );

    {
        bcore_string_s* buf = bcore_life_s_push_aware( l, bcore_string_s_create() );
        bcore_bml_translator_s_translate_object( trans, typeof( "specs_arr" ), specs_arr, buf );
        bcore_bml_interpreter_s* intrp = bcore_life_s_push_aware( l, bcore_bml_interpreter_s_create() );
        bcore_string_source_s* str_src = bcore_life_s_push_aware( l, bcore_string_source_s_create_string( buf ) );
        vd_t specs_arr_2 = bcore_life_s_push_aware( l, bcore_bml_interpreter_s_interpret_object( intrp, str_src ).o );
        bcore_string_s_pushf( out, "\n =================specs_arr_2: ==================\n" );
        bcore_bml_translator_s_translate_body( trans, typeof( "specs_arr" ), specs_arr_2, out );
    }

    bcore_life_s_discard( l );
    return out;
}

static bcore_string_s* interpret_selftest()
{
    bcore_life_s* l = bcore_life_s_create();
    bcore_string_s* out = bcore_string_s_create();

    sc_t src = "\
    !specs:\n\
    { \n\
        name = !f2_t:12345E7 \n\
        param2:1234 \n\
        param1:3456 \n\
        flag: true\n\
        numarr[]: 4,4,5,6; \n\
        strarr[]: \"fluff\", \"quirk\", \"mint\"; \n\
    }";

    bcore_bml_interpreter_s* intrp = bcore_life_s_push_aware( l, bcore_bml_interpreter_s_create() );
    bcore_string_source_s* str_src = bcore_life_s_push_aware( l, bcore_string_source_s_create_sc( src ) );
    vd_t specs                     = bcore_life_s_push_aware( l, bcore_bml_interpreter_s_interpret_object( intrp, str_src ).o );
    bcore_bml_translator_s* trans  = bcore_life_s_push_aware( l, bcore_bml_translator_s_create() );
    bcore_bml_translator_s_translate_body( trans, typeof( "specs" ), specs, out );

    bcore_life_s_discard( l );

    return out;
}

bcore_string_s* bcore_bml_selftest()
{
    bcore_string_s* out = bcore_string_s_create();
    bcore_string_s_push_string_d( out, translate_selftest() );
    bcore_string_s_push_string_d( out, interpret_selftest() );
    return out;
}
