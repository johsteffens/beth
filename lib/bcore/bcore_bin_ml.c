/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#include "bcore_bin_ml.h"
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
#include "bcore_spect.h"

DEFINE_FUNCTION_INIT_SPECT( bcore_bin_ml_translator_s )
DEFINE_FUNCTION_DOWN_SPECT( bcore_bin_ml_translator_s )
DEFINE_FUNCTION_COPY_SPECT( bcore_bin_ml_translator_s )
DEFINE_FUNCTION_CREATE(     bcore_bin_ml_translator_s )
DEFINE_FUNCTION_DISCARD(    bcore_bin_ml_translator_s )
DEFINE_FUNCTION_CLONE(      bcore_bin_ml_translator_s )

static inline void push_type( sr_s sink, tp_t type )
{
    bcore_sink_push_data( sink, &type, sizeof( type ) );
}

static void translate( const bcore_bin_ml_translator_s* o, tp_t name, sr_s obj, sr_s sink, sz_t depth )
{
    bcore_life_s* l = bcore_life_s_create();
    sr_s sink_l         = sr_cl( sr_cp( sink, TYPEOF_bcore_sink_s ), l );
    sr_s obj_l          = sr_cl( sr_cp( obj,  TYPEOF_bcore_via_s ),  l );

    if( name ) push_type( sink_l, name );

    if( !obj_l.o ) // NULL
    {
        push_type( sink_l, 0 );
    }
    else
    {
        push_type( sink_l, sr_s_type( &obj_l ) );
        if( sr_s_type( &obj_l ) == TYPEOF_st_s ) // strings
        {
            const st_s* string = obj_l.o;
            bcore_sink_push_data( sink_l, string->data, string->size + 1 ); // push string including terminating 0
        }
        else if( bcore_via_is_leaf( obj_l ) )
        {
            switch( sr_s_type( &obj_l ) )
            {
                case TYPEOF_aware_t : bcore_sink_push_data( sink_l, obj_l.o, sizeof( aware_t ) ); break;
                case TYPEOF_bl_t    : bcore_sink_push_data( sink_l, obj_l.o, sizeof( bl_t ) ); break;
                case TYPEOF_f2_t    : bcore_sink_push_data( sink_l, obj_l.o, sizeof( f2_t ) ); break;
                case TYPEOF_f3_t    : bcore_sink_push_data( sink_l, obj_l.o, sizeof( f3_t ) ); break;
                case TYPEOF_s0_t    : bcore_sink_push_data( sink_l, obj_l.o, sizeof( s0_t ) ); break;
                case TYPEOF_s1_t    : bcore_sink_push_data( sink_l, obj_l.o, sizeof( s1_t ) ); break;
                case TYPEOF_s2_t    : bcore_sink_push_data( sink_l, obj_l.o, sizeof( s2_t ) ); break;
                case TYPEOF_s3_t    : bcore_sink_push_data( sink_l, obj_l.o, sizeof( s3_t ) ); break;
                case TYPEOF_sc_t    : bcore_sink_push_data( sink_l, obj_l.o, bcore_strlen( obj_l.o ) ); break;
                case TYPEOF_sd_t    : bcore_sink_push_data( sink_l, obj_l.o, bcore_strlen( obj_l.o ) ); break;
                case TYPEOF_sz_t    : bcore_sink_push_data( sink_l, obj_l.o, sizeof( sz_t ) ); break;
                case TYPEOF_tp_t    : bcore_sink_push_data( sink_l, obj_l.o, sizeof( tp_t ) ); break;
                case TYPEOF_u0_t    : bcore_sink_push_data( sink_l, obj_l.o, sizeof( u0_t ) ); break;
                case TYPEOF_u1_t    : bcore_sink_push_data( sink_l, obj_l.o, sizeof( u1_t ) ); break;
                case TYPEOF_u2_t    : bcore_sink_push_data( sink_l, obj_l.o, sizeof( u2_t ) ); break;
                case TYPEOF_u3_t    : bcore_sink_push_data( sink_l, obj_l.o, sizeof( u3_t ) ); break;
                default: ERR( "Cannot convert type '%s' into a binary stream", ifnameof( sr_s_type( &obj_l ) ) );
            }
        }
        else
        {
            if( bcore_via_is_pure_array( obj_l ) )
            {
                sr_s arr_l = sr_cp( obj_l, TYPEOF_bcore_array_s );
                sz_t size = bcore_array_get_size( arr_l );
                bcore_sink_push_data( sink_l, &size, sizeof( sz_t ) );
                for( sz_t i = 0; i < size; i++ )
                {
                    translate( o, 0, bcore_array_get( arr_l, i ), sink_l, depth + 1 );
                }
            }
            else
            {
                sz_t size = bcore_via_spect_get_size( obj_l.p );
                bcore_sink_push_data( sink_l, &size, sizeof( sz_t ) );
                for( sz_t i = 0; i < size; i++ )
                {
                    translate( o, bcore_via_iget_name( obj_l, i ), bcore_via_iget( obj_l, i ), sink_l, depth + 1 );
                }
            }
        }
    }
    bcore_life_s_discard( l );
}

void bcore_bin_ml_translator_s_translate( const bcore_bin_ml_translator_s* o, sr_s obj, sr_s snk )
{
    translate( o, 0, obj, snk, 0 );
}

static bcore_flect_self_s* translator_s_create_self( void )
{
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( "bcore_bin_ml_translator_s = { aware_t _; }", sizeof( bcore_bin_ml_translator_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_bin_ml_translator_s_translate, "bcore_fp_translate", "translate"        );
    return self;
}

/**********************************************************************************************************************/

sr_s bcore_bin_ml_to_sink_buffer( sr_s obj )
{
    bcore_sink_buffer_s* buffer = bcore_sink_buffer_s_create();
    bcore_translate( sr_asd( bcore_bin_ml_translator_s_create() ), obj, sr_awd( buffer ) );
    return sr_asd( buffer );
}

void bcore_bin_ml_to_file( sr_s obj, sc_t file )
{
    bcore_life_s* l = bcore_life_s_create();
    bcore_sink_chain_s* chain = bcore_life_s_push_aware( l, bcore_sink_chain_s_create() );
    bcore_sink_chain_s_push_d( chain, bcore_sink_file_s_create_name( file ) );
    bcore_sink_chain_s_push_d( chain, bcore_inst_typed_create( typeof( "bcore_sink_buffer_s" ) ) );
    bcore_translate( sr_asd( bcore_bin_ml_translator_s_create() ), obj, sr_awd( chain ) );
    bcore_life_s_discard( l );
}

/**********************************************************************************************************************/
/// bcore_bin_ml_interpreter
/**********************************************************************************************************************/

DEFINE_FUNCTION_INIT_SPECT( bcore_bin_ml_interpreter_s )
DEFINE_FUNCTION_DOWN_SPECT( bcore_bin_ml_interpreter_s )
DEFINE_FUNCTION_COPY_SPECT( bcore_bin_ml_interpreter_s )
DEFINE_FUNCTION_CREATE(     bcore_bin_ml_interpreter_s )
DEFINE_FUNCTION_DISCARD(    bcore_bin_ml_interpreter_s )
DEFINE_FUNCTION_CLONE(      bcore_bin_ml_interpreter_s )

static inline tp_t get_type( sr_s source )
{
    tp_t type = 0;
    bcore_source_get_data( source, &type, sizeof( type ) );
    return type;
}

static sr_s interpret( const bcore_bin_ml_interpreter_s* o, sr_s obj, sr_s source )
{
    bcore_life_s* l = bcore_life_s_create();
    sr_s src_l = sr_cl( sr_cp( source, TYPEOF_bcore_source_s ), l );

    if( !obj.o )
    {
        tp_t type = get_type( src_l );
        if( type )
        {
            obj = interpret( o, bcore_inst_typed_create_sr( type ), src_l );
        }
    }
    else
    {
        obj = sr_cp( obj, TYPEOF_bcore_via_s );
        sr_s obj_l = sr_cw( obj );
        if( sr_s_type( &obj_l ) == TYPEOF_st_s )
        {
            st_s* string = obj_l.o;
            u0_t c = 0;
            bcore_source_get_data( src_l, &c, sizeof( c ) );
            while( c != 0 )
            {
                st_s_push_char( string, c );
                bcore_source_get_data( src_l, &c, sizeof( c ) );
            }
        }
        else if( bcore_via_is_leaf( obj_l ) )
        {
            switch( sr_s_type( &obj_l ) )
            {
                case TYPEOF_aware_t : bcore_source_get_data( src_l, obj_l.o, sizeof( aware_t ) ); break;
                case TYPEOF_bl_t    : bcore_source_get_data( src_l, obj_l.o, sizeof( bl_t ) ); break;
                case TYPEOF_f2_t    : bcore_source_get_data( src_l, obj_l.o, sizeof( f2_t ) ); break;
                case TYPEOF_f3_t    : bcore_source_get_data( src_l, obj_l.o, sizeof( f3_t ) ); break;
                case TYPEOF_s0_t    : bcore_source_get_data( src_l, obj_l.o, sizeof( s0_t ) ); break;
                case TYPEOF_s1_t    : bcore_source_get_data( src_l, obj_l.o, sizeof( s1_t ) ); break;
                case TYPEOF_s2_t    : bcore_source_get_data( src_l, obj_l.o, sizeof( s2_t ) ); break;
                case TYPEOF_s3_t    : bcore_source_get_data( src_l, obj_l.o, sizeof( s3_t ) ); break;
                case TYPEOF_sz_t    : bcore_source_get_data( src_l, obj_l.o, sizeof( sz_t ) ); break;
                case TYPEOF_tp_t    : bcore_source_get_data( src_l, obj_l.o, sizeof( tp_t ) ); break;
                case TYPEOF_u0_t    : bcore_source_get_data( src_l, obj_l.o, sizeof( u0_t ) ); break;
                case TYPEOF_u1_t    : bcore_source_get_data( src_l, obj_l.o, sizeof( u1_t ) ); break;
                case TYPEOF_u2_t    : bcore_source_get_data( src_l, obj_l.o, sizeof( u2_t ) ); break;
                case TYPEOF_u3_t    : bcore_source_get_data( src_l, obj_l.o, sizeof( u3_t ) ); break;
                default: ERR( "Cannot read type '%s' from a binary stream", ifnameof( sr_s_type( &obj_l ) ) );
            }
        }
        else
        {
            sz_t size = 0;
            bcore_source_get_data( src_l, &size, sizeof( sz_t ) );
            if( bcore_via_is_pure_array( obj_l ) )
            {
                sr_s arr_l = sr_cp( obj_l, TYPEOF_bcore_array_s );
                bcore_array_set_size( arr_l, size );
                for( sz_t i = 0; i < size; i++ ) bcore_array_set( arr_l, i, interpret( o, sr_null(), src_l ) );
            }
            else
            {

                for( sz_t i = 0; i < size; i++ )
                {
                    tp_t name = get_type( src_l );
                    sz_t idx = bcore_via_nget_index( obj_l, name );
                    if( bcore_via_iis_link( obj_l, idx ) )
                    {
                        bcore_via_iset( obj_l, idx, interpret( o, sr_null(), src_l ) );
                    }
                    else
                    {
                        sr_s item = bcore_via_iget( obj_l, idx );
                        if( item.o ) ASSERT( sr_s_type( &item ) == get_type( src_l ) );
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

sr_s bcore_bin_ml_interpreter_s_interpret( const bcore_bin_ml_interpreter_s* o, sr_s src )
{
    return interpret( o, sr_null(), src );
}

static bcore_flect_self_s* interpreter_s_create_self( void )
{
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( "bcore_bin_ml_interpreter_s = { aware_t _; }", sizeof( bcore_bin_ml_interpreter_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_bin_ml_interpreter_s_interpret, "bcore_fp_interpret", "interpret" );
    return self;
}

/**********************************************************************************************************************/

sr_s bcore_bin_ml_from_source( sr_s source )
{
    return bcore_interpret( bcore_inst_typed_create_sr( TYPEOF_bcore_bin_ml_interpreter_s ), source );
}

sr_s bcore_bin_ml_from_file( sc_t file )
{
    sr_s chain = sr_asd( bcore_source_chain_s_create() );
    bcore_source_chain_s_push_d( chain.o, bcore_source_file_s_create_name( file ) );
    bcore_source_chain_s_push_d( chain.o, bcore_inst_typed_create( typeof( "bcore_source_string_s" ) ) );
    sr_s ret = bcore_interpret( bcore_inst_typed_create_sr( typeof( "bcore_bin_ml_interpreter_s" ) ), chain );
    return ret;
}

/**********************************************************************************************************************/

void bcore_bin_ml_transfer_test( sr_s obj )
{
    bcore_life_s* l = bcore_life_s_create();
    obj = bcore_life_s_push_sr( l, obj );

    bcore_sink_buffer_s* sink_buffer = bcore_life_s_push_sr( l, bcore_bin_ml_to_sink_buffer( obj ) ).o;
    bcore_source_buffer_s* source_buffer = bcore_source_buffer_s_create_from_data( sink_buffer->data, sink_buffer->size );
    sr_s sr = bcore_life_s_push_sr( l, bcore_bin_ml_from_source( sr_asd( source_buffer ) ) );

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

static st_s* bin_ml_selftest( void )
{
    ASSERT( bcore_spect_supported( typeof( "bcore_interpreter_s" ), typeof( "bcore_bin_ml_interpreter_s" ) ) );
    ASSERT( bcore_spect_supported( typeof( "bcore_translator_s" ), typeof( "bcore_bin_ml_translator_s" ) ) );

    bcore_life_s* l = bcore_life_s_create();
    st_s* log = st_s_create();
    sr_s zoo = bcore_life_s_push_sr( l, bcore_spect_via_create_zoo( 1000 ) );
    clock_t time = clock();
    bcore_bin_ml_transfer_test( zoo );
    time = clock() - time;
    st_s_pushf( log, "bin transfer %5.3fs\n", ( double )time/CLOCKS_PER_SEC );

    bcore_life_s_discard( l );
    return log;
}

/**********************************************************************************************************************/
// signal

vd_t bcore_bin_ml_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bcore_bin_ml" ) ) return NULL;

    if( signal == typeof( "init1" ) )
    {
        bcore_flect_define_creator( typeof( "bcore_bin_ml_translator_s"  ), translator_s_create_self  );
        bcore_flect_define_creator( typeof( "bcore_bin_ml_interpreter_s" ), interpreter_s_create_self );
    }
    else if( signal == typeof( "selftest" ) )
    {
        st_s_print_d( bin_ml_selftest() );
    }

    return NULL;
}
