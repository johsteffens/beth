/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_txt_ml.h"
#include "bcore_life.h"
#include "bcore_sinks.h"
#include "bcore_spect_via.h"
#include "bcore_spect_array.h"
#include "bcore_spect_sink.h"
#include "bcore_spect_translator.h"
#include "bcore_quicktypes.h"

DEFINE_FUNCTION_INIT_SPECT( bcore_txt_ml_translator_s )
DEFINE_FUNCTION_DOWN_SPECT( bcore_txt_ml_translator_s )
DEFINE_FUNCTION_COPY_SPECT( bcore_txt_ml_translator_s )
DEFINE_FUNCTION_CREATE(     bcore_txt_ml_translator_s )
DEFINE_FUNCTION_DISCARD(    bcore_txt_ml_translator_s )
DEFINE_FUNCTION_CLONE(      bcore_txt_ml_translator_s )

static void translate_body( const bcore_txt_ml_translator_s* o, sc_t name, rf_s obj, vd_t sink, sz_t depth )
{
    const bcore_sink_s* sink_p = bcore_sink_s_get_aware( sink );
    const bcore_via_s* via_p   = obj.t ? bcore_via_s_get_typed( obj.t ) : NULL;

    bcore_string_s* ind = bcore_string_s_push_char_n( bcore_string_s_create(), ' ', depth * 4 );

    sink_p->pushf( sink_p, sink, "%s", ind->sc );
    if( name ) sink_p->pushf( sink_p, sink, "%s:", name );

    if( !obj.o ) // NULL
    {
        sink_p->pushf( sink_p, sink, "<NIL/>\n" );
    }
    else if( obj.t == TYPEOF_bcore_string_s ) // strings
    {
        bcore_string_s* string = bcore_string_s_clone( ( const bcore_string_s* )obj.o );
        bcore_string_s_replace_char_sc( string, '\"', "\\\"" );
        sink_p->pushf( sink_p, sink, "\"%s\"\n", string->sc );
        bcore_string_s_discard( string );
    }
    else if( bcore_via_spect_is_leaf( via_p ) )
    {
        sink_p->pushf( sink_p, sink, "<%s>", ifnameof( obj.t ) );
        bcore_string_s* string = bcore_string_s_create_typed( obj.t, obj.o );
        sink_p->push_string_d( sink_p, sink, string );
        sink_p->push_sc( sink_p, sink, "</>\n" );
    }
    else
    {
        sink_p->pushf( sink_p, sink, "<%s>\n", ifnameof( obj.t ) );
        if( bcore_via_spect_is_pure_array( via_p ) )
        {
            const bcore_array_s* arr_p = bcore_array_s_get_typed( obj.t );
            sz_t size = arr_p->get_size( arr_p, obj.o );
            for( sz_t i = 0; i < size; i++ )
            {
                translate_body( o, NULL, arr_p->get( arr_p, obj.o, i ), sink, depth + 1 );
            }
        }
        else
        {
            for( sz_t i = 0; i < via_p->size; i++ )
            {
                translate_body( o, ifnameof( bcore_via_spect_iget_name( via_p, i ) ), bcore_via_spect_iget( via_p, obj.o, i ), sink, depth + 1 );
            }
        }
        sink_p->pushf( sink_p, sink, "%s</>\n", ind->sc );
    }

    rf_s_down( obj );
    bcore_string_s_discard( ind );
}

void bcore_txt_ml_translator_s_translate_body( const bcore_txt_ml_translator_s* o, tp_t type, vc_t obj, vd_t sink )
{
    translate_body( o, NULL, rf_wc( obj, type ), sink, 0 );
}

void bcore_txt_ml_translator_s_translate_object( const bcore_txt_ml_translator_s* o, tp_t type, vc_t obj, vd_t sink )
{
    translate_body( o, NULL, rf_wc( obj, type ), sink, 0 );
}

static bcore_flect_self_s* translator_s_create_self( void )
{
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( "bcore_txt_ml_translator_s = { aware_t _; }", sizeof( bcore_txt_ml_translator_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_txt_ml_translator_s_translate_object, "bcore_fp_translate_object", "translate_object" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_txt_ml_translator_s_translate_body,   "bcore_fp_translate_body",   "translate_body"   );
    return self;
}

/**********************************************************************************************************************/

void bcore_txt_ml_typed_to_stdout( tp_t type, vc_t obj )
{
    bcore_txt_ml_translator_s* trans = bcore_txt_ml_translator_s_create();
    bcore_string_s* out = bcore_string_s_create();
    bcore_translate_typed_object( trans, type, obj, out );
    bcore_string_s_print_d( out );
    bcore_txt_ml_translator_s_discard( trans );
}

void bcore_txt_ml_aware_to_stdout( vc_t obj )
{
    bcore_txt_ml_typed_to_stdout( *( aware_t* )obj, obj );
}

void bcore_txt_ml_typed_to_file( tp_t type, vc_t obj, sc_t file )
{
    bcore_life_s* l = bcore_life_s_create();
    bcore_sink_chain_s* chain = bcore_life_s_push_aware( l, bcore_sink_chain_s_create() );
    bcore_sink_chain_s_push_d( chain, bcore_sink_file_s_create_name( file ) );
    bcore_sink_chain_s_push_d( chain, bcore_inst_typed_create( typeof( "bcore_sink_buffer_s" ) ) );
    bcore_translate_typed_object( bcore_life_s_push_aware( l, bcore_txt_ml_translator_s_create() ), type, obj, chain );
    bcore_life_s_discard( l );
}

void bcore_txt_ml_aware_to_file( vc_t obj, sc_t file )
{
    bcore_txt_ml_typed_to_file( *( aware_t* )obj, obj, file );
}

void bcore_txt_ml_typed_to_string( tp_t type, vc_t obj, bcore_string_s* string )
{
    bcore_life_s* l = bcore_life_s_create();
    bcore_translate_typed_object( bcore_life_s_push_aware( l, bcore_txt_ml_translator_s_create() ), type, obj, string );
    bcore_life_s_discard( l );
}

void bcore_txt_ml_aware_to_string( vc_t obj, bcore_string_s* string )
{
    bcore_txt_ml_typed_to_string( *( aware_t* )obj, obj, string );
}

/**********************************************************************************************************************/

void bcore_txt_ml_define_self_creators( void )
{
    bcore_flect_define_creator( typeof( "bcore_txt_ml_translator_s"  ), translator_s_create_self  );
}

/**********************************************************************************************************************/

static bcore_string_s* translate_selftest( void )
{
    bcore_life_s* l = bcore_life_s_create();
    bcore_string_s* log = bcore_string_s_create();

    tp_t t_animal   = bcore_flect_type_parse_sc( "animal = { aware_t _; bcore_string_s* type; f3_t weight; bcore_string_s * [] features; }" );
    tp_t t_compound = bcore_flect_type_parse_sc( "compound = { aware_t _; u3_t id; sz_t area; animal * [] animals; }" );
    tp_t t_zoo      = bcore_flect_type_parse_sc( "zoo = { aware_t _; bcore_string_s* name; typed * [] compounds; }" );

    vd_t zoo = bcore_life_s_push_aware( l, bcore_inst_typed_create( t_zoo ) );
    bcore_via_aware_nset_sc( zoo, typeof( "name" ), "Lori" );
    rf_s compounds = bcore_via_aware_nget( zoo, typeof( "compounds" ) );
    {
        vd_t compound = bcore_life_s_push_aware( l, bcore_inst_typed_create( t_compound ) );
        bcore_via_aware_nset_u3( compound, typeof( "id" ), 254 );
        bcore_via_aware_nset_sz( compound, typeof( "area" ), 1000 );
        rf_s animals = bcore_via_aware_nget( compound, typeof( "animals" ) );
        {
            vd_t bird = bcore_life_s_push_aware( l, bcore_inst_typed_create( t_animal ) );
            bcore_via_aware_nset_sc( bird, typeof( "type" ), "Bird" );
            bcore_via_aware_nset_f3( bird, typeof( "weight" ), 2.5 );
            rf_s features = bcore_via_aware_nget( bird, typeof( "features" ) );
            {
                const bcore_array_s* arr_p = bcore_array_s_get_typed( features.t );
                bcore_array_spect_push_sc( arr_p, features.o, "Owl" );
                bcore_array_spect_push_sc( arr_p, features.o, "Night active" );
            }
            const bcore_array_s* arr_p = bcore_array_s_get_typed( animals.t );
            bcore_array_spect_push( arr_p, animals.o, rf_awc( bird ) );
        }
        {
            vd_t bird = bcore_life_s_push_aware( l, bcore_inst_typed_create( t_animal ) );
            bcore_via_aware_nset_sc( bird, typeof( "type" ), "Bird" );
            bcore_via_aware_nset_f3( bird, typeof( "weight" ), 2.5 );
            rf_s features = bcore_via_aware_nget( bird, typeof( "features" ) );
            {
                const bcore_array_s* arr_p = bcore_array_s_get_typed( features.t );
                bcore_array_spect_push_sc( arr_p, features.o, "Pidgin" );
                bcore_array_spect_push_sc( arr_p, features.o, "Small" );
            }
            const bcore_array_s* arr_p = bcore_array_s_get_typed( animals.t );
            bcore_array_spect_push( arr_p, animals.o, rf_awc( bird ) );
        }
        const bcore_array_s* arr_p = bcore_array_s_get_typed( compounds.t );
        bcore_array_spect_push( arr_p, compounds.o, rf_awc( compound ) );
        bcore_array_spect_push( arr_p, compounds.o, rf_null() );
        bcore_array_spect_push( arr_p, compounds.o, rf_awc( compound ) );
    }

    bcore_txt_ml_translator_s* ttxt_ml = bcore_life_s_push_aware( l, bcore_txt_ml_translator_s_create() );
    bcore_translate_aware_object( ttxt_ml, zoo, log );

    bcore_life_s_discard( l );
    return log;
}

bcore_string_s* bcore_txt_ml_selftest( void )
{
    return translate_selftest();
}
