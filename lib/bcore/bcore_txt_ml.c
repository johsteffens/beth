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

void bcore_txt_ml_translator_s_translate_body( const bcore_txt_ml_translator_s* o, tp_t type, vc_t obj, vd_t sink )
{
    const bcore_sink_s* sink_p = bcore_sink_s_get_aware( sink );
    if( obj == NULL ) // NULL
    {
        sink_p->push_sc( sink_p, sink, "NULL\n" );
        return;
    }

    if( type == TYPEOF_bcore_string_s ) // strings
    {
        bcore_string_s* string = bcore_string_s_clone( ( const bcore_string_s* )obj );
        bcore_string_s_replace_char_sc( string, '\"', "\\\"" );
        sink_p->pushf( sink_p, sink, "\"%s\"\n", string->sc );
        bcore_string_s_discard( string );
        return;
    }

    const bcore_via_s* via_p = bcore_via_s_get_typed( type );

    if( via_p->size == 0 ) // leaf
    {
        bcore_string_s* string = bcore_string_s_create_typed( type, obj );
        sink_p->push_string_d( sink_p, sink, string );
        sink_p->push_char( sink_p, sink, '\n' );
        return;
    }

    if( bcore_via_spect_is_pure_array( via_p ) )
    {
        const bcore_array_s* arr_p = bcore_array_s_get_typed( type );
        sz_t size = arr_p->get_size( arr_p, obj );
        if( bcore_array_spect_is_mono_typed( arr_p ) )
        {
            tp_t type_l = bcore_array_spect_get_mono_type( arr_p, obj );
            if( !bcore_array_spect_is_static( arr_p ) ) sink_p->pushf( sink_p, sink, "%s\n", ifnameof( type_l ) );
            for( sz_t i = 0; i < size; i++ )
            {
                bcore_txt_ml_translator_s_translate_body( o, type_l, arr_p->get_c( arr_p, obj, i ), sink );
            }
        }
        else
        {
            for( sz_t i = 0; i < size; i++ )
            {
                tp_t type_l = bcore_array_spect_get_type( arr_p, obj, i );
                sink_p->pushf( sink_p, sink, "%s\n", ifnameof( type_l ) );
                bcore_txt_ml_translator_s_translate_body( o, type_l, arr_p->get_c( arr_p, obj, i ), sink );
            }
        }
        sink_p->push_sc( sink_p, sink, ";\n" );
        return;
    }

    for( sz_t i = 0; i < via_p->size; i++ )
    {
        tp_t type_l = bcore_via_spect_iget_type( via_p, obj, i );
        if( i == 0 && type_l == TYPEOF_aware_t ) continue;
        vc_t obj_l  = bcore_via_spect_iget_c( via_p, obj, i );
        if( !bcore_via_spect_iis_static( via_p, i ) ) sink_p->pushf( sink_p, sink, "%s\n", ifnameof( type_l ) );
        bcore_txt_ml_translator_s_translate_body( o, type_l, obj_l, sink );
    }
}

void bcore_txt_ml_translator_s_translate_object( const bcore_txt_ml_translator_s* o, tp_t type, vc_t obj, vd_t sink )
{
    const bcore_sink_s* sink_p = bcore_sink_s_get_aware( sink );
    sink_p->pushf( sink_p, sink, "%s\n", ifnameof( type ) );
    bcore_txt_ml_translator_s_translate_body( o, type, obj, sink );
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

    tp_t t_specs = bcore_flect_type_parse_sc( "{ aware_t _; bcore_string_s* name; sz_t num; bcore_string_s [] arr; }" );
    vd_t specs = bcore_life_s_push_aware( l, bcore_inst_typed_create( t_specs ) );

    bcore_via_aware_nset_sc( specs, typeof( "name" ), "my string" );
    bcore_via_aware_nset_u3( specs, typeof( "num" ), 1235 );
    const bcore_array_s* arr_p = bcore_via_aware_nget_array( specs, typeof( "arr" ) );
    bcore_array_spect_set_s3( arr_p, bcore_via_aware_nget_d( specs, typeof( "arr" ) ), 5, -12 );
    bcore_array_spect_push_sc( arr_p, bcore_via_aware_nget_d( specs, typeof( "arr" ) ), "abc" );

    bcore_txt_ml_translator_s* ttxt_ml = bcore_life_s_push_aware( l, bcore_txt_ml_translator_s_create() );
    bcore_translate_aware_object( ttxt_ml, specs, log );

    bcore_life_s_discard( l );
    return log;
}

bcore_string_s* bcore_txt_ml_selftest( void )
{
    return translate_selftest();
}
