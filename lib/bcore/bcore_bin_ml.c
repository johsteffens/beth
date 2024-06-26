/** Author and Copyright 2017 Johannes Bernhard Steffens
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

#include "bcore_bin_ml.h"
#include "bcore_life.h"
#include "bcore_sinks.h"
#include "bcore_sources.h"
#include "bcore_spect_via.h"
#include "bcore_spect_array.h"
#include "bcore_spect_sink.h"
#include "bcore_spect_translator.h"
#include "bcore_spect_interpreter.h"
#include "bcore_signal.h"
#include "bcore_spect_compare.h"
#include "bcore_spect.h"

BCORE_DEFINE_FUNCTIONS_OBJ_INST( bcore_bin_ml_translator_s )

static inline void push_type( sr_s sink, tp_t type )
{
    bcore_sink_x_push_data( sink, &type, sizeof( type ) );
}

static inline void push_flag( const sr_s* sink, bl_t flag )
{
    u0_t v = flag ? 1 : 0;
    bcore_sink_r_push_data( sink, &v, 1 );
}

static void translate( const bcore_bin_ml_translator_s* o, tp_t name, sr_s obj, sr_s sink, uz_t depth )
{
    bcore_life_s* l = bcore_life_s_create();
    sr_s sink_l = sr_cl( sr_cp( sink, TYPEOF_bcore_sink_s ), l );
    sr_s obj_l  = sr_cl( sr_cp( obj, TYPEOF_bcore_via_s ), l );

    if( name ) push_type( sink_l, name );

    // shelving obj_l
    if( obj_l.o && bcore_via_r_defines_shelve( &obj_l ) )
    {
        obj_l = sr_cl( sr_clone( obj_l ), l );
        bcore_via_r_shelve( &obj_l );
    }

    if( !obj_l.o ) // NULL
    {
        if( obj_l.p )
        {
            push_type( sink_l, sr_s_type( &obj_l ) );
            push_flag( &sink_l, false );
            // 'false' specifies that the object is not instantiated.
            // This is to differentiate from the case where the object was instantiated
            // without additional parameters.
        }
        else
        {
            push_type( sink_l, 0 );
        }
    }
    else
    {
        push_type( sink_l, sr_s_type( &obj_l ) );
        push_flag( &sink_l, true ); // true signals that the object is to be instantiated

        if( sr_s_type( &obj_l ) == TYPEOF_st_s ) // strings
        {
            const st_s* string = obj_l.o;
            bcore_sink_x_push_data( sink_l, string->data, string->size + 1 ); // push string including terminating 0
        }
        else if( bcore_via_x_is_leaf( obj_l ) )
        {
            switch( sr_s_type( &obj_l ) )
            {
                case TYPEOF_aware_t : bcore_sink_x_push_data( sink_l, obj_l.o, sizeof( aware_t ) ); break;
                case TYPEOF_bl_t    : bcore_sink_x_push_data( sink_l, obj_l.o, sizeof( bl_t ) ); break;
                case TYPEOF_f2_t    : bcore_sink_x_push_data( sink_l, obj_l.o, sizeof( f2_t ) ); break;
                case TYPEOF_f3_t    : bcore_sink_x_push_data( sink_l, obj_l.o, sizeof( f3_t ) ); break;
                case TYPEOF_s0_t    : bcore_sink_x_push_data( sink_l, obj_l.o, sizeof( s0_t ) ); break;
                case TYPEOF_s1_t    : bcore_sink_x_push_data( sink_l, obj_l.o, sizeof( s1_t ) ); break;
                case TYPEOF_s2_t    : bcore_sink_x_push_data( sink_l, obj_l.o, sizeof( s2_t ) ); break;
                case TYPEOF_s3_t    : bcore_sink_x_push_data( sink_l, obj_l.o, sizeof( s3_t ) ); break;
                case TYPEOF_sc_t    : bcore_sink_x_push_data( sink_l, obj_l.o, bcore_strlen( obj_l.o ) ); break;
                case TYPEOF_sd_t    : bcore_sink_x_push_data( sink_l, obj_l.o, bcore_strlen( obj_l.o ) ); break;
                case TYPEOF_sz_t    : bcore_sink_x_push_data( sink_l, obj_l.o, sizeof( sz_t ) ); break;
                case TYPEOF_uz_t    : bcore_sink_x_push_data( sink_l, obj_l.o, sizeof( uz_t ) ); break;
                case TYPEOF_tp_t    : bcore_sink_x_push_data( sink_l, obj_l.o, sizeof( tp_t ) ); break;
                case TYPEOF_u0_t    : bcore_sink_x_push_data( sink_l, obj_l.o, sizeof( u0_t ) ); break;
                case TYPEOF_u1_t    : bcore_sink_x_push_data( sink_l, obj_l.o, sizeof( u1_t ) ); break;
                case TYPEOF_u2_t    : bcore_sink_x_push_data( sink_l, obj_l.o, sizeof( u2_t ) ); break;
                case TYPEOF_u3_t    : bcore_sink_x_push_data( sink_l, obj_l.o, sizeof( u3_t ) ); break;
                default: ERR( "Cannot convert type '%s' into a binary stream", ifnameof( sr_s_type( &obj_l ) ) );
            }
        }
        else
        {
            if( bcore_via_x_is_pure_array( obj_l ) )
            {
                sr_s arr_l = sr_cp( obj_l, TYPEOF_bcore_array_s );
                uz_t size = bcore_array_x_get_size( arr_l );
                bcore_sink_x_push_data( sink_l, &size, sizeof( uz_t ) );

                if( bcore_array_x_is_mutable_mono_typed( arr_l ) ) translate( o, 0, sr_tp( bcore_array_x_get_mono_type( arr_l ) ), sink_l, depth + 1 );

                for( uz_t i = 0; i < size; i++ )
                {
                    translate( o, 0, bcore_array_x_get( arr_l, i ), sink_l, depth + 1 );
                }
            }
            else
            {
                uz_t size = bcore_via_p_get_size( obj_l.p, NULL );
                bcore_sink_x_push_data( sink_l, &size, sizeof( uz_t ) );
                for( uz_t i = 0; i < size; i++ )
                {
                    translate( o, bcore_via_x_iget_name( obj_l, i ), bcore_via_x_iget( obj_l, i ), sink_l, depth + 1 );
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

static bcore_self_s* translator_s_create_self( void )
{
    bcore_self_s* self = BCORE_SELF_S_BUILD_PARSE_SC( "bcore_bin_ml_translator_s = bcore_translator { aware_t _; }", bcore_bin_ml_translator_s );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_bin_ml_translator_s_translate, "bcore_fp_translate", "translate" );
    return self;
}

/**********************************************************************************************************************/

sr_s bcore_bin_ml_x_to_sink_buffer( sr_s obj )
{
    bcore_sink_buffer_s* buffer = bcore_sink_buffer_s_create();
    bcore_translate_x( sr_asm( bcore_bin_ml_translator_s_create() ), obj, sr_awm( buffer ) );
    return sr_asm( buffer );
}

void bcore_bin_ml_x_to_sink( sr_s obj, bcore_sink* sink )
{
    bcore_translate_x( sr_asm( bcore_bin_ml_translator_s_create() ), obj, sr_awm( sink ) );
}

void bcore_bin_ml_x_to_stdout( sr_s obj )
{
    bcore_translate_x( sr_asm( bcore_bin_ml_translator_s_create() ), obj, sr_awm( BCORE_STDOUT ) );
}

void bcore_bin_ml_x_to_string( sr_s obj, st_s* string )
{
    bcore_translate_x( sr_asm( bcore_bin_ml_translator_s_create() ), obj, sr_awm( string ) );
}

void bcore_bin_ml_x_to_file( sr_s obj, sc_t file )
{
    bcore_life_s* l = bcore_life_s_create();
    bcore_sink_chain_s* chain = bcore_life_s_push_aware( l, bcore_sink_chain_s_create() );
    bcore_sink_chain_s_push_d( chain, bcore_sink_file_s_create_name( file ) );
    bcore_sink_chain_s_push_d( chain, bcore_inst_t_create( typeof( "bcore_sink_buffer_s" ) ) );
    bcore_translate_x( sr_asm( bcore_bin_ml_translator_s_create() ), obj, sr_awm( chain ) );
    bcore_life_s_discard( l );
}

sr_s bcore_bin_ml_a_to_sink_buffer( vc_t obj )
{
    return bcore_bin_ml_x_to_sink_buffer( sr_awc( obj ) );
}

void bcore_bin_ml_r_to_sink( const sr_s* obj, bcore_sink* sink ) { bcore_bin_ml_x_to_sink( sr_cw( *obj ),  sink ); }
void bcore_bin_ml_a_to_sink( vc_t o,          bcore_sink* sink ) { bcore_bin_ml_x_to_sink( sr_awc( o ),    sink ); }
void bcore_bin_ml_t_to_sink( tp_t t, vc_t o,  bcore_sink* sink ) { bcore_bin_ml_x_to_sink( sr_twc( t, o ), sink ); }
void bcore_bin_ml_default_to_sink( tp_t t,    bcore_sink* sink ) { bcore_bin_ml_x_to_sink( sr_create( t ), sink ); }

void bcore_bin_ml_r_to_stdout( const sr_s* obj ) { bcore_bin_ml_x_to_stdout( sr_cw( *obj ) ); }
void bcore_bin_ml_a_to_stdout( vc_t o          ) { bcore_bin_ml_x_to_stdout( sr_awc( o ) ); }
void bcore_bin_ml_t_to_stdout( tp_t t, vc_t o  ) { bcore_bin_ml_x_to_stdout( sr_twc( t, o ) ); }
void bcore_bin_ml_default_to_stdout( tp_t t )    { bcore_bin_ml_x_to_stdout( sr_create( t ) ); }

void bcore_bin_ml_r_to_file( const sr_s* obj, sc_t file ) { bcore_bin_ml_x_to_file( sr_cw( *obj ),  file ); }
void bcore_bin_ml_a_to_file( vc_t o, sc_t file          ) { bcore_bin_ml_x_to_file( sr_awc( o ),    file ); }
void bcore_bin_ml_t_to_file( tp_t t, vc_t o, sc_t file  ) { bcore_bin_ml_x_to_file( sr_twc( t, o ), file ); }
void bcore_bin_ml_default_to_file( tp_t t, sc_t file    ) { bcore_bin_ml_x_to_file( sr_create( t ), file ); }

void bcore_bin_ml_r_to_string( const sr_s* obj, st_s* string ) { bcore_bin_ml_x_to_string( sr_cw( *obj ),  string ); }
void bcore_bin_ml_a_to_string( vc_t o, st_s* string          ) { bcore_bin_ml_x_to_string( sr_awc( o ),    string ); }
void bcore_bin_ml_t_to_string( tp_t t, vc_t o, st_s* string  ) { bcore_bin_ml_x_to_string( sr_twc( t, o ), string ); }
void bcore_bin_ml_default_to_string( tp_t t, st_s* string    ) { bcore_bin_ml_x_to_string( sr_create( t ), string ); }

/**********************************************************************************************************************/
/// bcore_bin_ml_interpreter
/**********************************************************************************************************************/

BCORE_DEFINE_FUNCTIONS_OBJ_INST( bcore_bin_ml_interpreter_s )

static inline tp_t get_type( const sr_s* source )
{
    tp_t type = 0;
    bcore_source_r_get_data( source, &type, sizeof( type ) );
    return type;
}

static inline bl_t get_flag( const sr_s* source )
{
    u0_t v = 0;
    bcore_source_r_get_data( source, &v, 1 );
    if( v == 0 ) return false;
    if( v == 1 ) return true;
    bcore_source_r_parse_err_fa( source, "Invalid flag value '#<u0_t>'.", v );
    return true;
}

static sr_s interpret( const bcore_bin_ml_interpreter_s* o, sr_s obj, sr_s source )
{
    bcore_life_s* l = bcore_life_s_create();
    sr_s src_l = sr_cl( sr_cp( source, TYPEOF_bcore_source_s ), l );

    if( !obj.o )
    {
        tp_t type = get_type( &src_l );
        if( type )
        {
            bl_t create_instance = get_flag( &src_l );
            if( create_instance )
            {
                obj = interpret( o, bcore_inst_t_create_sr( type ), src_l );
            }
            else
            {
                obj.p = bcore_inst_s_get_typed( type );
            }
        }
    }
    else
    {
        obj = sr_cp( obj, TYPEOF_bcore_via_s );
        sr_s obj_l = sr_cw( obj );
        if( sr_s_type( &obj_l ) == TYPEOF_st_s )
        {
            st_s* string = obj_l.o;
            st_s_clear( string );
            u0_t c = 0;
            bcore_source_x_get_data( src_l, &c, sizeof( c ) );
            while( c != 0 )
            {
                st_s_push_char( string, c );
                bcore_source_x_get_data( src_l, &c, sizeof( c ) );
            }
        }
        else if( bcore_via_x_is_leaf( obj_l ) )
        {
            switch( sr_s_type( &obj_l ) )
            {
                case TYPEOF_aware_t : bcore_source_x_get_data( src_l, obj_l.o, sizeof( aware_t ) ); break;
                case TYPEOF_bl_t    : bcore_source_x_get_data( src_l, obj_l.o, sizeof( bl_t ) ); break;
                case TYPEOF_f2_t    : bcore_source_x_get_data( src_l, obj_l.o, sizeof( f2_t ) ); break;
                case TYPEOF_f3_t    : bcore_source_x_get_data( src_l, obj_l.o, sizeof( f3_t ) ); break;
                case TYPEOF_s0_t    : bcore_source_x_get_data( src_l, obj_l.o, sizeof( s0_t ) ); break;
                case TYPEOF_s1_t    : bcore_source_x_get_data( src_l, obj_l.o, sizeof( s1_t ) ); break;
                case TYPEOF_s2_t    : bcore_source_x_get_data( src_l, obj_l.o, sizeof( s2_t ) ); break;
                case TYPEOF_s3_t    : bcore_source_x_get_data( src_l, obj_l.o, sizeof( s3_t ) ); break;
                case TYPEOF_sz_t    : bcore_source_x_get_data( src_l, obj_l.o, sizeof( sz_t ) ); break;
                case TYPEOF_uz_t    : bcore_source_x_get_data( src_l, obj_l.o, sizeof( uz_t ) ); break;
                case TYPEOF_tp_t    : bcore_source_x_get_data( src_l, obj_l.o, sizeof( tp_t ) ); break;
                case TYPEOF_u0_t    : bcore_source_x_get_data( src_l, obj_l.o, sizeof( u0_t ) ); break;
                case TYPEOF_u1_t    : bcore_source_x_get_data( src_l, obj_l.o, sizeof( u1_t ) ); break;
                case TYPEOF_u2_t    : bcore_source_x_get_data( src_l, obj_l.o, sizeof( u2_t ) ); break;
                case TYPEOF_u3_t    : bcore_source_x_get_data( src_l, obj_l.o, sizeof( u3_t ) ); break;
                default: ERR( "Cannot read type '%s' from a binary stream", ifnameof( sr_s_type( &obj_l ) ) );
            }
        }
        else
        {
            uz_t size = 0;
            bcore_source_x_get_data( src_l, &size, sizeof( uz_t ) );
            if( bcore_via_x_is_pure_array( obj_l ) )
            {
                sr_s arr_l = sr_cp( obj_l, TYPEOF_bcore_array_s );
                if( bcore_array_x_is_mutable_mono_typed( arr_l ) ) bcore_array_x_set_gtype( arr_l, sr_to_tp( interpret( o, sr_null(), src_l ) ) );
                bcore_array_x_set_size( arr_l, size );
                for( uz_t i = 0; i < size; i++ ) bcore_array_x_set( arr_l, i, interpret( o, sr_null(), src_l ) );
            }
            else
            {
                for( uz_t i = 0; i < size; i++ )
                {
                    tp_t name = get_type( &src_l );
                    uz_t idx = bcore_via_x_nget_index( obj_l, name );
                    if( bcore_via_x_iis_link( obj_l, idx ) )
                    {
                        bcore_via_x_iset( obj_l, idx, interpret( o, sr_null(), src_l ) );
                    }
                    else
                    {
                        sr_s item = bcore_via_x_iget( obj_l, idx );
                        if( item.o )
                        {
                            ASSERT( get_type( &src_l ) == sr_s_type( &item ) );
                            ASSERT( get_flag( &src_l ) == true );
                        }
                        if( sr_s_is_strong( &item ) )  // if item is detached --> refeed it
                        {
                            bcore_via_x_iset( obj_l, idx, interpret( o, item, src_l ) );
                        }
                        else
                        {
                            sr_down( interpret( o, item, src_l ) );
                        }
                    }
                }
            }
        }
        bcore_via_r_source(  &obj_l, source.o );
        bcore_via_r_mutated( &obj_l );
    }
    bcore_life_s_discard( l );
    return obj;
}

sr_s bcore_bin_ml_interpreter_s_interpret( const bcore_bin_ml_interpreter_s* o, sr_s src )
{
    return interpret( o, sr_null(), src );
}

static bcore_self_s* interpreter_s_create_self( void )
{
    bcore_self_s* self = BCORE_SELF_S_BUILD_PARSE_SC( "bcore_bin_ml_interpreter_s = bcore_interpreter { aware_t _; }", bcore_bin_ml_interpreter_s );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_bin_ml_interpreter_s_interpret, "bcore_fp_interpret", "interpret" );
    return self;
}

/**********************************************************************************************************************/

sr_s bcore_bin_ml_from_source( bcore_source* source )
{
    bcore_bin_ml_interpreter_s* interpreter = bcore_bin_ml_interpreter_s_create();
    sr_s ret = interpret( interpreter, sr_null(), sr_awm( source ) );
    bcore_bin_ml_interpreter_s_discard( interpreter );
    return ret;
}

// ---------------------------------------------------------------------------------------------------------------------

sr_s bcore_bin_ml_from_file( sc_t file )
{
    sr_s chain = sr_asm( bcore_source_chain_s_create() );
    bcore_source_chain_s_push_d( chain.o, bcore_source_file_s_create_name( file ) );
    bcore_source_chain_s_push_d( chain.o, bcore_inst_t_create( typeof( "bcore_source_string_s" ) ) );
    sr_s ret = bcore_interpret_x( bcore_inst_t_create_sr( typeof( "bcore_bin_ml_interpreter_s" ) ), chain );
    return ret;
}

void bcore_bin_ml_a_from_file( vd_t o, sc_t file )
{
    bcore_bin_ml_t_from_file( *(aware_t*)o, o, file );
}

void bcore_bin_ml_r_from_file( sr_s* o, sc_t file )
{
    bcore_bin_ml_t_from_file( sr_s_type( o ), o->o, file );
}

void bcore_bin_ml_t_from_file( tp_t t, vd_t o, sc_t file )
{
    sr_s sr = bcore_bin_ml_from_file( file );
    bcore_inst_t_copy_typed( t, o, sr_s_type( &sr ), sr.o );
    sr_down( sr );
}

void bcore_bin_ml_a_from_source( vd_t o, bcore_source* source )
{
    sr_s sr = bcore_bin_ml_from_source( source );
    bcore_inst_t_copy_typed( *(aware_t*)o, o, sr_s_type( &sr ), sr.o );
    sr_down( sr );
}

void bcore_bin_ml_t_from_source( tp_t t, vd_t o, bcore_source* source )
{
    sr_s sr = bcore_bin_ml_from_source( source );
    bcore_inst_t_copy_typed( t, o, sr_s_type( &sr ), sr.o );
    sr_down( sr );
}

/**********************************************************************************************************************/

sr_s bcore_bin_ml_transfer( sr_s obj )
{
    bcore_life_s* l = bcore_life_s_create();
    obj = bcore_life_s_push_sr( l, obj );

    bcore_sink_buffer_s* sink_buffer = bcore_life_s_push_sr( l, bcore_bin_ml_x_to_sink_buffer( obj ) ).o;
    bcore_source_buffer_s* source_buffer = bcore_life_s_push_aware( l, bcore_source_buffer_s_create_from_data( sink_buffer->data, sink_buffer->size ) );
    sr_s out_sr = bcore_bin_ml_from_source( ( bcore_source* )source_buffer );
    bcore_life_s_discard( l );
    return out_sr;
}

void bcore_bin_ml_a_copy( vd_t o, vc_t obj )
{
    BLM_INIT();
    bcore_sink_buffer_s* sink_buffer = BLM_A_PUSH( bcore_bin_ml_a_to_sink_buffer( obj ).o );
    bcore_source* source = BLM_A_PUSH( bcore_source_buffer_s_create_from_data( sink_buffer->data, sink_buffer->size ) );
    bcore_bin_ml_a_from_source( o, source );
    BLM_DOWN();
}

/**********************************************************************************************************************/

void bcore_bin_ml_transfer_test( sr_s obj )
{
    bcore_life_s* l = bcore_life_s_create();
    obj = bcore_life_s_push_sr( l, obj );
    sr_s sr = bcore_life_s_push_sr( l, bcore_bin_ml_transfer( obj ) );

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
    ASSERT( bcore_spect_trait_supported( typeof( "bcore_interpreter" ), typeof( "bcore_bin_ml_interpreter_s" ) ) );
    ASSERT( bcore_spect_trait_supported( typeof( "bcore_translator" ), typeof( "bcore_bin_ml_translator_s" ) ) );

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

vd_t bcore_bin_ml_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_bin_ml" ) ) )
    {
        case TYPEOF_init1:
        {
            bcore_flect_define_creator( typeof( "bcore_bin_ml_translator_s"  ), translator_s_create_self  );
            bcore_flect_define_creator( typeof( "bcore_bin_ml_interpreter_s" ), interpreter_s_create_self );
        }
        break;

        case TYPEOF_selftest:
        {
            return bin_ml_selftest();
        }
        break;

        default: break;
    }
    return NULL;
}
