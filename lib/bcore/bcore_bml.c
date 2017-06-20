/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_bml.h"
#include "bcore_via_perspective.h"
#include "bcore_sink_perspective.h"
#include "bcore_array_perspective.h"

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
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_bml_translator_s_init,      "bcore_fp_init",      "init"      );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_bml_translator_s_translate, "bcore_fp_translate", "translate" );
    return self;
}

/**********************************************************************************************************************/

static void translate( const bcore_bml_translator_s* o, bool link, tp_t type, vc_t obj, vd_t dst, sz_t indent )
{
    const bcore_instance_s* inst = bcore_instance_s_get_typed( type );
    const bcore_sink_s* snk = bcore_sink_s_get_aware( dst );
    if( !obj )
    {
       snk->pushf( snk, dst, "void" );
       return;
    }

    // special types
    if( type == BCORE_TYPEOF_bcore_string_s ) // treat as leaf type
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
                if( flect_item->type == BCORE_TYPEOF_aware_t ) continue;
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

void bcore_bml_translator_s_translate( const bcore_bml_translator_s* o, tp_t type, vc_t obj, vd_t flow_sink )
{
    const bcore_sink_s* snk = bcore_sink_s_get_aware( flow_sink );
    snk->pushf( snk, flow_sink, "!%s: ", nameof( type ) );
    translate( o, false, type, obj, flow_sink, 0 );
    snk->pushf( snk, flow_sink, "\n" );
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
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_bml_interpreter_s_init,      "bcore_fp_init",      "init"      );
//    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_bml_interpreter_s_interpret, "bcore_fp_interpret", "interpret" );
    return self;
}

/**********************************************************************************************************************/

bcore_string_s* bcore_bml_selftest()
{
    bcore_string_s* out = bcore_string_s_create();

    bcore_flect_parse_sc( "specs = { aware_t _; bcore_string_s* name; sz_t size; u2_t param1; s2_t param2; sz_t [] numarr;  bcore_string_s [] strarr; bool flag;}" );
    bcore_flect_parse_sc( "specs_arr = { aware_t _; aware * [] arr; }" );

    vd_t specs = bcore_instance_typed_create( typeof( "specs" ) );
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

    vd_t specs_arr = bcore_instance_typed_create( typeof( "specs_arr" ) );
    {
        const bcore_array_s * arr_p = bcore_array_s_get_aware( specs_arr );
        for( sz_t i = 0; i < 2; i++ )
        {
            arr_p->push_c( arr_p, specs_arr, specs );
        }
    }

    bcore_bml_translator_s* trans = bcore_bml_translator_s_create();

    bcore_bml_translator_s_translate( trans, typeof( "specs_arr" ), specs_arr, out );
    bcore_bml_translator_s_translate( trans, typeof( "bcore_bml_translator_s" ), trans, out );

    bcore_bml_translator_s_discard( trans );
    bcore_instance_aware_discard( specs );
    bcore_instance_aware_discard( specs_arr );
    return out;
}
