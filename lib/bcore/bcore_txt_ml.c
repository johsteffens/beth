/** Copyright 2017 Johannes Bernhard Steffens
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "bcore_txt_ml.h"
#include "bcore_life.h"
#include "bcore_st.h"
#include "bcore_sinks.h"
#include "bcore_sources.h"
#include "bcore_spect_via.h"
#include "bcore_spect_array.h"
#include "bcore_spect_sink.h"
#include "bcore_spect_translator.h"
#include "bcore_spect_interpreter.h"
#include "bcore_spect_compare.h"
#include "bcore_spect.h"
#include "bcore_signal.h"

void bcore_txt_ml_translator_s_init( bcore_txt_ml_translator_s* o )
{
    o->_ = TYPEOF_bcore_txt_ml_translator_s;
    o->indent = 4;
}

BCORE_DEFINE_FUNCTION_DOWN_INST( bcore_txt_ml_translator_s )
BCORE_DEFINE_FUNCTION_COPY_INST( bcore_txt_ml_translator_s )
BCORE_DEFINE_FUNCTION_CREATE(    bcore_txt_ml_translator_s )
BCORE_DEFINE_FUNCTION_DISCARD(   bcore_txt_ml_translator_s )
BCORE_DEFINE_FUNCTION_CLONE(     bcore_txt_ml_translator_s )

static sc_t name_of( tp_t type, st_s* buf )
{
    sc_t n = bcore_name_try_name( type );
    if( n ) return n;
    st_s_copy_typed( buf, TYPEOF_tp_t, &type );
    return buf->sc;
}

static tp_t type_of( const st_s* name )
{
    tp_t tp = 0;
    if( name->size == 0 )
    {
        return 0;
    }
    else if( name->sc[ 0 ] >= '0' && name->sc[ 0 ] <= '9' )
    {
        st_s_parse_fa( name, 0, -1, "#tp_t", &tp );
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
    st_s* ind = st_s_push_char_n( st_s_create_l( l ), ' ', depth * o->indent );
    st_s* buf = st_s_create_l( l );

    bcore_sink_pushf( sink_l, "%s", ind->sc );
    if( name ) bcore_sink_pushf( sink_l, "%s:", name_of( name, buf ) );

    if( !obj_l.o ) // NULL
    {
        if( obj_l.p )
        {
            // Explicit NULL specifies that the object is not instantiated.
            // This is to differentiate from the case where the object was instantiated
            // without additional parameters.
            bcore_sink_pushf( sink_l, "<%s>NULL</>\n", name_of( sr_s_type( &obj_l ), buf ) );
        }
        else
        {
            bcore_sink_pushf( sink_l, "<></>\n" );
        }
    }
    else
    {
        bcore_sink_pushf( sink_l, "<%s>", name_of( sr_s_type( &obj_l ), buf ) );
        if( sr_s_type( &obj_l ) == TYPEOF_st_s ) // strings
        {
            st_s* string = st_s_clone( ( const st_s* )obj_l.o );
            st_s_replace_char_sc( string, '\"', "\\\"" );
            bcore_sink_pushf( sink_l, "\"%s\"", string->sc );
            st_s_discard( string );
        }
        else if( bcore_via_is_leaf( obj_l ) )
        {
            bcore_sink_push_string_d( sink_l, st_s_create_typed( sr_s_type( &obj_l ), obj_l.o ) );
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

void bcore_txt_ml_translator_s_translate( const bcore_txt_ml_translator_s* o, sr_s obj, sr_s snk )
{
    translate( o, 0, obj, snk, 0 );
}

static bcore_self_s* translator_s_create_self( void )
{
    bcore_self_s* self = bcore_self_s_build_parse_sc( "bcore_txt_ml_translator_s = bcore_translator { aware_t _; sz_t indent; }", sizeof( bcore_txt_ml_translator_s ) );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_txt_ml_translator_s_init,             "bcore_fp_init", "init" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_txt_ml_translator_s_translate,        "bcore_fp_translate",        "translate"        );
    return self;
}

/**********************************************************************************************************************/

void bcore_txt_ml_to_stdout( sr_s obj )
{
    st_s* out = st_s_create();
    bcore_translate( sr_asd( bcore_txt_ml_translator_s_create() ), obj, sr_awd( out ) );
    st_s_print_d( out );
}

void bcore_txt_ml_to_file( sr_s obj, sc_t file )
{
    bcore_life_s* l = bcore_life_s_create();
    bcore_sink_chain_s* chain = bcore_life_s_push_aware( l, bcore_sink_chain_s_create() );
    bcore_sink_chain_s_push_d( chain, bcore_sink_file_s_create_name( file ) );
    bcore_sink_chain_s_push_d( chain, bcore_inst_typed_create( typeof( "bcore_sink_buffer_s" ) ) );
    bcore_translate( sr_asd( bcore_txt_ml_translator_s_create() ), obj, sr_awd( chain ) );
    bcore_life_s_discard( l );
}

void bcore_txt_ml_to_string( sr_s obj, st_s* string )
{
    bcore_translate( sr_asd( bcore_txt_ml_translator_s_create() ), obj, sr_awd( string ) );
}

void bcore_txt_ml_aware_to_stdout( vc_t o )
{
    bcore_txt_ml_to_stdout( sr_awc( o ) );
}

void bcore_txt_ml_aware_to_file(   vc_t o, sc_t file )
{
    bcore_txt_ml_to_file( sr_awc( o ), file );
}

void bcore_txt_ml_aware_to_string( vc_t o, st_s* string )
{
    bcore_txt_ml_to_string( sr_awc( o ), string );
}

/**********************************************************************************************************************/
/// bcore_txt_ml_interpreter
/**********************************************************************************************************************/

void bcore_txt_ml_interpreter_s_init( bcore_txt_ml_interpreter_s* o )
{
    o->_ = TYPEOF_bcore_txt_ml_interpreter_s;
}

BCORE_DEFINE_FUNCTION_DOWN_INST( bcore_txt_ml_interpreter_s )
BCORE_DEFINE_FUNCTION_COPY_INST( bcore_txt_ml_interpreter_s )
BCORE_DEFINE_FUNCTION_CREATE(    bcore_txt_ml_interpreter_s )
BCORE_DEFINE_FUNCTION_DISCARD(   bcore_txt_ml_interpreter_s )
BCORE_DEFINE_FUNCTION_CLONE(     bcore_txt_ml_interpreter_s )

static sr_s interpret( const bcore_txt_ml_interpreter_s* o, sr_s obj, sr_s source )
{
    bcore_life_s* l = bcore_life_s_create();
    sr_s src_l = sr_cl( sr_cp( source, TYPEOF_bcore_source_s ), l );

    if( !obj.o )
    {
        st_s* type_string = st_s_create_l( l );
        bcore_source_parse_fa( src_l, " <#until'>'>", type_string );
        tp_t type = type_of( type_string );
        if( type )
        {
            if( !bcore_flect_exists( type ) )
            {
                bcore_source_q_parse_errf( &src_l, "Type '%s' has no reflection.", type_string->sc );
            }

            if( bcore_source_q_parse_bl_fa( &src_l, " #?'NULL'" ) ) // no instance
            {
                obj.p = bcore_inst_s_get_typed( type );
                bcore_source_q_parse_fa( &src_l, " </>" );
            }
            else
            {
                obj = interpret( o, bcore_inst_typed_create_sr( type ), src_l );
            }
        }
        else
        {
            bcore_source_q_parse_fa( &src_l, " </>" );
        }
    }
    else
    {
        obj = sr_cp( obj, TYPEOF_bcore_via_s );
        sr_s obj_l = sr_cw( obj );
        st_s* buf = st_s_create_l( l );
        if( sr_s_type( &obj_l ) == TYPEOF_st_s )
        {
            bcore_source_parse_fa( src_l, " #string", obj_l.o );
            bcore_source_parse_fa( src_l, " </>" );
        }
        else if( bcore_via_is_leaf( obj_l ) )
        {
            bcore_source_parse_fa( src_l, st_s_createf_l( l, " #%s", name_of( sr_s_type( &obj_l ), buf ) )->sc, obj_l.o );
            bcore_source_parse_fa( src_l, " </>" );
        }
        else
        {
            if( bcore_via_is_pure_array( obj_l ) )
            {
                sr_s arr_l = sr_cp( obj_l, TYPEOF_bcore_array_s );
                if( bcore_array_is_fixed( arr_l ) )
                {
                    sz_t arr_size = bcore_array_get_size( arr_l );
                    for( sz_t i = 0; i < arr_size; i++ ) bcore_array_set( arr_l, i, interpret( o, sr_null(), src_l ) );
                    bcore_source_parse_fa( src_l, " </>" );
                }
                else
                {
                    bcore_array_set_size( arr_l, 0 );
                    while( !bcore_source_parse_bl_fa( src_l, " #?'</>'" ) ) bcore_array_push( arr_l, interpret( o, sr_null(), src_l ) );
                }
            }
            else
            {
                st_s* buf = st_s_create_l( l );
                while( !bcore_source_parse_bl_fa( src_l, " #?'</>'" ) )
                {
                    st_s* name = st_s_create_l( l );
                    bcore_source_parse_fa( src_l, " #name :", name );

                    tp_t ntype = typeof( name->sc );
                    if( !bcore_via_q_nexists( &obj_l, ntype ) )
                    {
                        bcore_source_q_parse_errf( &src_l, "Object '%s' has no element of name '%s'.", ifnameof( sr_s_type( &obj_l ) ), name->sc );
                    }

                    sz_t idx = bcore_via_nget_index( obj_l, ntype );
                    if( bcore_via_iis_link( obj_l, idx ) )
                    {
                        bcore_via_iset( obj_l, idx, interpret( o, sr_null(), src_l ) );
                    }
                    else
                    {
                        sr_s item = bcore_via_iget( obj_l, idx );
                        if( item.o ) bcore_source_parse_fa( src_l, st_s_createf_l( l, " <%s>", name_of( sr_s_type( &item ), buf ) )->sc );
                        if( sr_s_is_strong( &item ) )  // if item is detached --> refeed it
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

sr_s bcore_txt_ml_interpreter_s_interpret( const bcore_txt_ml_interpreter_s* o, sr_s src )
{
    return interpret( o, sr_null(), src );
}

static bcore_self_s* interpreter_s_create_self( void )
{
    bcore_self_s* self = bcore_self_s_build_parse_sc( "bcore_txt_ml_interpreter_s = bcore_interpreter { aware_t _; }", sizeof( bcore_txt_ml_interpreter_s ) );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_txt_ml_interpreter_s_init,             "bcore_fp_init", "init" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_txt_ml_interpreter_s_interpret,        "bcore_fp_interpret",        "interpret"        );
    return self;
}

/**********************************************************************************************************************/

sr_s bcore_txt_ml_from_file( sc_t file )
{
    sr_s chain = sr_asd( bcore_source_chain_s_create() );
    bcore_source_chain_s_push_d( chain.o, bcore_source_file_s_create_name( file ) );
    bcore_source_chain_s_push_d( chain.o, bcore_inst_typed_create( typeof( "bcore_source_string_s" ) ) );
    sr_s ret = bcore_interpret( bcore_inst_typed_create_sr( TYPEOF_bcore_txt_ml_interpreter_s ), chain );
    return ret;
}

sr_s bcore_txt_ml_from_string( const st_s* string )
{
    sr_s chain = sr_asd( bcore_source_chain_s_create() );
    bcore_source_chain_s_push_d( chain.o, bcore_source_string_s_create_from_string( string ) );
    sr_s ret = bcore_interpret( bcore_inst_typed_create_sr( TYPEOF_bcore_txt_ml_interpreter_s ), chain );
    return ret;
}

/**********************************************************************************************************************/

void bcore_txt_ml_transfer_test( sr_s obj )
{
    bcore_life_s* l = bcore_life_s_create();
    obj = bcore_life_s_push_sr( l, obj );
    st_s* string = bcore_life_s_push_aware( l, st_s_create() );
    bcore_txt_ml_to_string( obj, string );
    sr_s sr = bcore_life_s_push_sr( l, bcore_txt_ml_from_string( string ) );
    s2_t c = bcore_compare_sr( obj, sr );
    if( c != 0 )
    {
        st_s* diff = bcore_life_s_push_aware( l, bcore_diff_sr( obj, sr ) );
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

#include <time.h>

static st_s* txt_ml_selftest( void )
{
    ASSERT( bcore_spect_trait_supported( typeof( "bcore_interpreter" ), TYPEOF_bcore_txt_ml_interpreter_s ) );
    ASSERT( bcore_spect_trait_supported( typeof( "bcore_translator" ), TYPEOF_bcore_txt_ml_translator_s ) );

    bcore_life_s* l = bcore_life_s_create();
    st_s* log = st_s_create();

    sr_s zoo = bcore_life_s_push_sr( l, bcore_spect_via_create_zoo( 1000 ) );
    clock_t time = clock();
    bcore_txt_ml_transfer_test( zoo );
    time = clock() - time;
    st_s_pushf( log, "txt transfer %5.3fs\n", ( double )time/CLOCKS_PER_SEC );

    bcore_life_s_discard( l );
    return log;
}

/**********************************************************************************************************************/
// signal

vd_t bcore_txt_ml_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_txt_ml" ) ) )
    {
        case TYPEOF_init1:
        {
            bcore_flect_define_creator( TYPEOF_bcore_txt_ml_translator_s,  translator_s_create_self  );
            bcore_flect_define_creator( TYPEOF_bcore_txt_ml_interpreter_s, interpreter_s_create_self );
        }
        break;

        case TYPEOF_selftest:
        {
            return txt_ml_selftest();
        }
        break;

        default: break;
    }
    return NULL;
}
