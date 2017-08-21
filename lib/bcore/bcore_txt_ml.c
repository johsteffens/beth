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

static sc_t name_of( tp_t type, bcore_string_s* buf )
{
    sc_t n = bcore_name_try_name( type );
    if( n ) return n;
    bcore_string_s_copy_typed( buf, TYPEOF_tp_t, &type );
    return buf->sc;
}

static void translate_body( const bcore_txt_ml_translator_s* o, tp_t name, sr_s obj, sr_s sink, sz_t depth )
{
    bcore_life_s* l = bcore_life_s_create();
    sr_s sink_l         = sr_cl( sr_cp( sink, TYPEOF_bcore_sink_s ), l );
    sr_s obj_l          = sr_cl( sr_cp( obj,  TYPEOF_bcore_via_s ),  l );
    bcore_string_s* ind = bcore_string_s_push_char_n( bcore_string_s_create_l( l ), ' ', depth * 4 );
    bcore_string_s* buf = bcore_string_s_create_l( l );

    bcore_sink_pushf( sink_l, "%s", ind->sc );
    if( name ) bcore_sink_pushf( sink_l, "%s:", name_of( name, buf ) );

    if( !obj_l.o ) // NULL
    {
        bcore_sink_pushf( sink_l, "<NIL/>\n" );
    }
    else if( sr_type( obj_l ) == TYPEOF_bcore_string_s ) // strings
    {
        bcore_string_s* string = bcore_string_s_clone( ( const bcore_string_s* )obj_l.o );
        bcore_string_s_replace_char_sc( string, '\"', "\\\"" );
        bcore_sink_pushf( sink_l, "\"%s\"\n", string->sc );
        bcore_string_s_discard( string );
    }
    else if( bcore_via_is_leaf( obj_l ) )
    {
        bcore_sink_pushf( sink_l, "<%s>", name_of( sr_type( obj_l ), buf ) );
        bcore_string_s* string = bcore_string_s_create_typed( sr_type( obj_l ), obj_l.o );
        bcore_sink_push_string_d( sink_l, string );
        bcore_sink_push_sc( sink_l, "</>\n" );
    }
    else
    {
        bcore_sink_pushf( sink_l, "<%s>\n", name_of( sr_type( obj_l ), buf ) );
        if( bcore_via_is_pure_array( obj_l ) )
        {
            sr_s arr_l = sr_cp( obj_l, TYPEOF_bcore_array_s );
            sz_t size = bcore_array_get_size( arr_l );
            for( sz_t i = 0; i < size; i++ )
            {
                translate_body( o, 0, bcore_array_get( arr_l, i ), sink_l, depth + 1 );
            }
        }
        else
        {
            sz_t size = bcore_via_spect_get_size( obj_l.p );
            for( sz_t i = 0; i < size; i++ )
            {
                translate_body( o, bcore_via_iget_name( obj_l, i ), bcore_via_iget( obj_l, i ), sink_l, depth + 1 );
            }
        }
        bcore_sink_pushf( sink_l, "%s</>\n", ind->sc );
    }
    bcore_life_s_discard( l );
}

void bcore_txt_ml_translator_s_translate_body( const bcore_txt_ml_translator_s* o, tp_t type, vc_t obj, vd_t sink )
{
    translate_body( o, 0, sr_twc( type, obj ), sr_awd( sink ), 0 );
}

void bcore_txt_ml_translator_s_translate_object( const bcore_txt_ml_translator_s* o, tp_t type, vc_t obj, vd_t sink )
{
    translate_body( o, 0, sr_twc( type, obj ), sr_awd( sink ), 0 );
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
    bcore_via_aware_nset_sc( zoo, typeof( "name" ), "Mesamurial" );
    sr_s compounds = bcore_life_s_push_sr( l, bcore_via_aware_nget( zoo, typeof( "compounds" ) ) );
    {
        sr_s compound = bcore_life_s_push_sr( l, sr_asd( bcore_inst_typed_create( t_compound ) ) );
        bcore_via_nset_u3( compound, typeof( "id" ), 254 );
        bcore_via_nset_sz( compound, typeof( "area" ), 1000 );
        sr_s animals = bcore_life_s_push_sr( l, bcore_via_nget( compound, typeof( "animals" ) ) );
        {
            sr_s bird = bcore_life_s_push_sr( l, sr_asd( bcore_inst_typed_create( t_animal ) ) );
            bcore_via_nset_sc( bird, typeof( "type" ), "Bird" );
            bcore_via_nset_f3( bird, typeof( "weight" ), 2.5 );
            sr_s features = bcore_life_s_push_sr( l, bcore_via_nget( bird, typeof( "features" ) ) );
            {
                bcore_array_push_sc( features, "Owl" );
                bcore_array_push_sc( features, "Night active" );
            }
            bcore_array_push( animals, bird );
        }
        {
            sr_s bird = bcore_life_s_push_sr( l, sr_asd( bcore_inst_typed_create( t_animal ) ) );
            bcore_via_nset_sc( bird, typeof( "type" ), "Bird" );
            bcore_via_nset_f3( bird, typeof( "weight" ), 2.5 );
            sr_s features = bcore_life_s_push_sr( l, bcore_via_nget( bird, typeof( "features" ) ) );
            {
                bcore_array_push_sc( features, "Pidgin" );
                bcore_array_push_sc( features, "Small" );
            }
            bcore_array_push( animals, bird );
        }
        bcore_array_push( compounds, compound );
        bcore_array_push( compounds, sr_null() );
        bcore_array_push( compounds, compound );
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
