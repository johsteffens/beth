/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_txt_ml.h"
#include "bcore_life.h"
#include "bcore_sinks.h"
#include "bcore_sources.h"
#include "bcore_spect_via.h"
#include "bcore_spect_array.h"
#include "bcore_spect_sink.h"
#include "bcore_spect_translator.h"
#include "bcore_spect_interpreter.h"
#include "bcore_quicktypes.h"
#include "bcore_spect_compare.h"

void bcore_txt_ml_translator_s_init( bcore_txt_ml_translator_s* o )
{
    o->_ = typeof( "bcore_txt_ml_translator_s" );
    o->indent = 4;
}

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

static tp_t type_of( const bcore_string_s* name )
{
    tp_t tp = 0;
    if( name->size == 0 )
    {
        return 0;
    }
    else if( name->sc[ 0 ] >= '0' && name->sc[ 0 ] <= '9' )
    {
        bcore_string_s_parsef( name, 0, -1, "#tp_t", &tp );
    }
    else
    {
        tp = typeof( name->sc );
    }
    return tp;
}

static void translate( const bcore_txt_ml_translator_s* o, tp_t name, sr_s obj, sr_s sink, sz_t depth )
{
    bcore_life_s* l = bcore_life_s_create();
    sr_s sink_l         = sr_cl( sr_cp( sink, TYPEOF_bcore_sink_s ), l );
    sr_s obj_l          = sr_cl( sr_cp( obj,  TYPEOF_bcore_via_s ),  l );
    bcore_string_s* ind = bcore_string_s_push_char_n( bcore_string_s_create_l( l ), ' ', depth * o->indent );
    bcore_string_s* buf = bcore_string_s_create_l( l );

    bcore_sink_pushf( sink_l, "%s", ind->sc );
    if( name ) bcore_sink_pushf( sink_l, "%s:", name_of( name, buf ) );

    if( !obj_l.o ) // NULL
    {
        bcore_sink_pushf( sink_l, "<></>\n" );
    }
    else
    {
        bcore_sink_pushf( sink_l, "<%s>", name_of( sr_type( obj_l ), buf ) );
        if( bcore_via_is_leaf( obj_l ) )
        {
            if( sr_type( obj_l ) == TYPEOF_bcore_string_s ) // strings
            {
                bcore_string_s* string = bcore_string_s_clone( ( const bcore_string_s* )obj_l.o );
                bcore_string_s_replace_char_sc( string, '\"', "\\\"" );
                bcore_sink_pushf( sink_l, "\"%s\"", string->sc );
                bcore_string_s_discard( string );
            }
            else
            {
                bcore_sink_push_string_d( sink_l, bcore_string_s_create_typed( sr_type( obj_l ), obj_l.o ) );
            }
        }
        else
        {
            bcore_sink_push_char( sink_l, '\n' );
            if( bcore_via_is_pure_array( obj_l ) )
            {
                sr_s arr_l = sr_cp( obj_l, TYPEOF_bcore_array_s );
                sz_t size = bcore_array_get_size( arr_l );
                for( sz_t i = 0; i < size; i++ )
                {
                    translate( o, 0, bcore_array_get( arr_l, i ), sink_l, depth + 1 );
                }
            }
            else
            {
                sz_t size = bcore_via_spect_get_size( obj_l.p );
                for( sz_t i = 0; i < size; i++ )
                {
                    translate( o, bcore_via_iget_name( obj_l, i ), bcore_via_iget( obj_l, i ), sink_l, depth + 1 );
                }
            }
            bcore_sink_pushf( sink_l, "%s", ind->sc );
        }
        bcore_sink_push_sc( sink_l, "</>\n" );
    }
    bcore_life_s_discard( l );
}

void bcore_txt_ml_translator_s_translate_body( const bcore_txt_ml_translator_s* o, tp_t type, vc_t obj, vd_t sink )
{
    translate( o, 0, sr_twc( type, obj ), sr_awd( sink ), 0 );
}

void bcore_txt_ml_translator_s_translate_object( const bcore_txt_ml_translator_s* o, tp_t type, vc_t obj, vd_t sink )
{
    translate( o, 0, sr_twc( type, obj ), sr_awd( sink ), 0 );
}

void bcore_txt_ml_translator_s_translate( const bcore_txt_ml_translator_s* o, sr_s obj, sr_s snk )
{
    translate( o, 0, obj, snk, 0 );
}

static bcore_flect_self_s* translator_s_create_self( void )
{
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( "bcore_txt_ml_translator_s = { aware_t _; sz_t indent; }", sizeof( bcore_txt_ml_translator_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_txt_ml_translator_s_init,             "bcore_fp_init", "init" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_txt_ml_translator_s_translate_object, "bcore_fp_translate_object", "translate_object" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_txt_ml_translator_s_translate_body,   "bcore_fp_translate_body",   "translate_body"   );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_txt_ml_translator_s_translate,        "bcore_fp_translate",        "translate"        );
    return self;
}

/**********************************************************************************************************************/

void bcore_txt_ml_to_stdout( sr_s obj )
{
    bcore_txt_ml_translator_s* trans = bcore_txt_ml_translator_s_create();
    bcore_string_s* out = bcore_string_s_create();
    bcore_translate_typed_object( trans, sr_type( obj ), obj.o, out );
    bcore_string_s_print_d( out );
    bcore_txt_ml_translator_s_discard( trans );
    sr_down( obj );
}

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
/// bcore_txt_ml_interpreter
/**********************************************************************************************************************/

void bcore_txt_ml_interpreter_s_init( bcore_txt_ml_interpreter_s* o )
{
    o->_ = typeof( "bcore_txt_ml_interpreter_s" );
}

DEFINE_FUNCTION_DOWN_SPECT( bcore_txt_ml_interpreter_s )
DEFINE_FUNCTION_COPY_SPECT( bcore_txt_ml_interpreter_s )
DEFINE_FUNCTION_CREATE(     bcore_txt_ml_interpreter_s )
DEFINE_FUNCTION_DISCARD(    bcore_txt_ml_interpreter_s )
DEFINE_FUNCTION_CLONE(      bcore_txt_ml_interpreter_s )

static sr_s interpret( const bcore_txt_ml_interpreter_s* o, sr_s obj, sr_s source )
{
    bcore_life_s* l = bcore_life_s_create();
    sr_s src_l = sr_cl( sr_cp( source, TYPEOF_bcore_source_s ), l );

    if( !obj.o )
    {
        bcore_string_s* type_string = bcore_string_s_create_l( l );
        bcore_source_parsef( src_l, " <#until'>'>", type_string );
        tp_t type = type_of( type_string );
        if( type )
        {
            obj = interpret( o, bcore_inst_typed_create_sr( type ), src_l );
        }
        else
        {
            bcore_source_parsef( src_l, " </>" );
        }
    }
    else
    {
        obj = sr_cp( obj, TYPEOF_bcore_via_s );
        sr_s obj_l = sr_cw( obj );
        bcore_string_s* buf = bcore_string_s_create_l( l );
        if( bcore_via_is_leaf( obj_l ) )
        {
            if( sr_type( obj_l ) == TYPEOF_bcore_string_s )
            {
                bcore_source_parsef( src_l, " #string", obj_l.o );
            }
            else
            {
                bcore_source_parsef( src_l, bcore_string_s_createf_l( l, " #%s", name_of( sr_type( obj_l ), buf ) )->sc, obj_l.o );
            }
            bcore_source_parsef( src_l, " </>" );
        }
        else
        {
            if( bcore_via_is_pure_array( obj_l ) )
            {
                sr_s arr_l = sr_cp( obj_l, TYPEOF_bcore_array_s );
                bcore_array_set_size( arr_l, 0 );
                while( !bcore_source_parse_boolf( src_l, " #?'</>'" ) ) bcore_array_push( arr_l, interpret( o, sr_null(), src_l ) );
            }
            else
            {
                bcore_string_s* buf = bcore_string_s_create_l( l );
                while( !bcore_source_parse_boolf( src_l, " #?'</>'" ) )
                {
                    bcore_string_s* name = bcore_string_s_create_l( l );
                    bcore_source_parsef( src_l, " #name :", name );
                    sz_t idx = bcore_via_nget_index( obj_l, typeof( name->sc ) );
                    if( bcore_via_iis_link( obj_l, idx ) )
                    {
                        bcore_via_iset( obj_l, idx, interpret( o, sr_null(), src_l ) );
                    }
                    else
                    {
                        sr_s item = bcore_via_iget( obj_l, idx );
                        if( item.o ) bcore_source_parsef( src_l, bcore_string_s_createf_l( l, " <%s>", name_of( sr_type( item ), buf ) )->sc );
                        if( item.f & S_f )  // if item is detached --> refeed it
                        {
                            bcore_via_iset( obj_l, idx, interpret( o, item, src_l ) );
                        }
                        else
                        {
                            sr_down( interpret( o, item, src_l ) );
                        }
                    }
                }
            }
        }
    }
    bcore_life_s_discard( l );
    return obj;
}

dt_p bcore_txt_ml_interpreter_s_interpret_body( const bcore_txt_ml_interpreter_s* o, vd_t fsrc, tp_t type, vd_t obj )
{
    bcore_life_s* l = bcore_life_s_create();
    sr_s src_l = sr_awd( fsrc );
    bcore_string_s* type_string = bcore_string_s_create_l( l );
    bcore_source_parsef( src_l, " <#until'>'>", type_string );
    if( type_of( type_string ) != type ) ERR ("type mismatch" );
    sr_s obj_l = interpret( o, sr_twd( type, obj ), src_l );
    bcore_life_s_discard( l );
    return ( dt_p ){ .o = obj_l.o, .t = sr_type( obj_l ) };
}

dt_p bcore_txt_ml_interpreter_s_interpret_object( const bcore_txt_ml_interpreter_s* o, vd_t fsrc )
{
    sr_s obj_l = interpret( o, sr_null(), sr_awd( fsrc ) );
    return ( dt_p ){ .o = obj_l.o, .t = sr_type( obj_l ) };
}

sr_s bcore_txt_ml_interpreter_s_interpret( const bcore_txt_ml_interpreter_s* o, sr_s src )
{
    return interpret( o, sr_null(), src );
}

static bcore_flect_self_s* interpreter_s_create_self( void )
{
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( "bcore_txt_ml_interpreter_s = { aware_t _; }", sizeof( bcore_txt_ml_interpreter_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_txt_ml_interpreter_s_init,             "bcore_fp_init", "init" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_txt_ml_interpreter_s_interpret_object, "bcore_fp_interpret_object", "interpret_object" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_txt_ml_interpreter_s_interpret_body,   "bcore_fp_interpret_body",   "interpret_body"   );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_txt_ml_interpreter_s_interpret,        "bcore_fp_interpret",        "interpret"        );
    return self;
}

/**********************************************************************************************************************/

dt_p bcore_txt_ml_object_from_file( sc_t file )
{
    bcore_life_s* l = bcore_life_s_create();
    bcore_source_chain_s* chain = bcore_life_s_push_aware( l, bcore_source_chain_s_create() );
    bcore_source_chain_s_push_d( chain, bcore_source_file_s_create_name( file ) );
    bcore_source_chain_s_push_d( chain, bcore_inst_typed_create( typeof( "bcore_source_string_s" ) ) );
    dt_p dt = bcore_interpret_object( bcore_life_s_push_aware( l, bcore_txt_ml_interpreter_s_create() ), chain );
    bcore_life_s_discard( l );
    return dt;
}

dt_p bcore_txt_ml_object_from_string( const bcore_string_s* string )
{
    bcore_life_s* l = bcore_life_s_create();
    bcore_source_chain_s* chain = bcore_life_s_push_aware( l, bcore_source_chain_s_create() );
    bcore_source_chain_s_push_d( chain, bcore_source_string_s_create_string( string ) );
    dt_p dt = bcore_interpret_object( bcore_life_s_push_aware( l, bcore_txt_ml_interpreter_s_create() ), chain );
    bcore_life_s_discard( l );
    return dt;
}

/**********************************************************************************************************************/

void bcore_txt_ml_define_self_creators( void )
{
    bcore_flect_define_creator( typeof( "bcore_txt_ml_translator_s"  ), translator_s_create_self  );
    bcore_flect_define_creator( typeof( "bcore_txt_ml_interpreter_s"  ), interpreter_s_create_self  );
}

/**********************************************************************************************************************/

void bcore_txt_ml_transfer_test( sr_s obj )
{
    bcore_life_s* l = bcore_life_s_create();
    obj = bcore_life_s_push_sr( l, obj );
    bcore_string_s* string = bcore_life_s_push_aware( l, bcore_string_s_create() );
    bcore_txt_ml_typed_to_string( sr_type( obj ), obj.o, string );
    dt_p dt = bcore_txt_ml_object_from_string( string );
    bcore_life_s_push_typed( l, dt.t, dt.o );
    s2_t c = bcore_compare_bityped( sr_type( obj ), obj.o, dt.t, dt.o );
    if( c != 0 )
    {
        bcore_string_s* diff = bcore_life_s_push_aware( l, bcore_diff_bityped( sr_type( obj ), obj.o, dt.t, dt.o ) );
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

static sr_s create_zoo()
{
    bcore_life_s* l = bcore_life_s_create();
    tp_t t_animal   = bcore_flect_type_parse_sc( "animal = { bcore_string_s* type; f3_t weight; bcore_string_s * [] features; }" );
    tp_t t_compound = bcore_flect_type_parse_sc( "compound = { u3_t id; sz_t area; animal * [] animals; }" );
    tp_t t_zoo      = bcore_flect_type_parse_sc( "zoo = { bcore_string_s* name; typed * [] compounds; }" );

    sr_s ret = bcore_inst_typed_create_sr( t_zoo );
    sr_s zoo = sr_cw( ret );

    bcore_via_nset_sc( zoo, typeof( "name" ), "Mesamurial" );
    sr_s compounds = bcore_life_s_push_sr( l, bcore_via_nget( zoo, typeof( "compounds" ) ) );
    {
        sr_s compound = sr_cl( bcore_inst_typed_create_sr( t_compound ), l );
        bcore_via_nset_u3( compound, typeof( "id" ), 254 );
        bcore_via_nset_sz( compound, typeof( "area" ), 1000 );
        sr_s animals = bcore_life_s_push_sr( l, bcore_via_nget( compound, typeof( "animals" ) ) );
        {
            sr_s bird = sr_cl( bcore_inst_typed_create_sr( t_animal ), l );
            bcore_via_nset_sc( bird, typeof( "type" ), "Owl" );
            bcore_via_nset_f3( bird, typeof( "weight" ), 2.5 );
            sr_s features = sr_cl( bcore_via_nget( bird, typeof( "features" ) ), l );
            {
                bcore_array_push_sc( features, "Night active" );
                bcore_array_push_sc( features, "Can fly" );
            }
            bcore_array_push( animals, bird );
        }
        {
            sr_s bird = sr_cl( bcore_inst_typed_create_sr( t_animal ), l );
            bcore_via_nset_sc( bird, typeof( "type" ), "Pidgin" );
            bcore_via_nset_f3( bird, typeof( "weight" ), 0.5 );
            sr_s features = sr_cl( bcore_via_nget( bird, typeof( "features" ) ), l );
            {
                bcore_array_push_sc( features, "Day active" );
                bcore_array_push_sc( features, "Can fly" );
            }
            bcore_array_push( animals, bird );
        }
        bcore_array_push( compounds, compound );
        bcore_array_push( compounds, sr_null() );
        bcore_array_push( compounds, compound );
        bcore_array_push( compounds, sr_null() );
        bcore_array_push( compounds, sr_null() );
        //for( sz_t i = 0; i < 10000; i++ ) bcore_array_push( compounds, compound );
    }
    bcore_life_s_discard( l );

    return ret;
}

bcore_string_s* bcore_txt_ml_selftest( void )
{
    bcore_life_s* l = bcore_life_s_create();
    bcore_string_s* log = bcore_string_s_create();
    sr_s zoo = bcore_life_s_push_sr( l, create_zoo() );
    bcore_txt_ml_transfer_test( zoo );
    bcore_life_s_discard( l );
    return log;
}
