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

#include "bcore_sinks.h"
#include "bcore_spect_inst.h"
#include "bcore_signal.h"
#include "bcore_spect_array.h"
#include "bcore_spect_translator.h"
#include "bcore_spect_interpreter.h"
#include "bcore_life.h"

#include <stdio.h>

/**********************************************************************************************************************/
/// bcore_sink_chain_s
/**********************************************************************************************************************/

static void chain_init_a( bcore_nucleus_s* nc )
{
    nc->default_handler( nc );
    bcore_sink_chain_s* o = nc->client;
    o->_ = TYPEOF_bcore_sink_chain_s;
}

void bcore_sink_chain_s_init( bcore_sink_chain_s* o )
{
    bcore_inst_t_init( TYPEOF_bcore_sink_chain_s, (bcore_inst*)o );
}

static void chain_down_a( bcore_nucleus_s* nc )
{
    bcore_sink_chain_s* o = nc->client;
    bcore_sink_chain_s_flush( o );
    nc->default_handler( nc );
}

void bcore_sink_chain_s_down( bcore_sink_chain_s* o )
{
    bcore_inst_t_down( TYPEOF_bcore_sink_chain_s, ( bcore_inst* )o );
}

static void chain_copy_a( bcore_nucleus_s* nc )
{
    nc->default_handler( nc );
    bcore_sink_chain_s* o = nc->client;
    for( uz_t i = 1; i < o->size; i++ ) bcore_sink_a_set_consumer( o->data[ i ], o->data[ i - 1 ] );
}

void bcore_sink_chain_s_copy( bcore_sink_chain_s* o, const bcore_sink_chain_s* src )
{
    bcore_inst_t_copy( TYPEOF_bcore_sink_chain_s, (bcore_inst*)o, src );
}

void bcore_sink_chain_s_discard( bcore_sink_chain_s* o )
{
    bcore_inst_t_discard( TYPEOF_bcore_sink_chain_s, (bcore_inst*)o );
}

bcore_sink_chain_s* bcore_sink_chain_s_create()
{
    return bcore_inst_t_create( TYPEOF_bcore_sink_chain_s );
}

bcore_sink_chain_s* bcore_sink_chain_s_clone( const bcore_sink_chain_s* o )
{
    return bcore_inst_t_clone( TYPEOF_bcore_sink_chain_s, (bcore_inst*)o );
}

static void chain_interpret_body_a( vd_t nc )
{
    struct { ap_t a; vc_t p; vc_t inter; vd_t sink; tp_t type; vd_t obj; } * nc_l = nc;
    nc_l->a( nc ); // default
    bcore_sink_chain_s* o = nc_l->obj;
    for( uz_t i = 1; i < o->size; i++ ) bcore_sink_a_set_consumer( o->data[ i ], o->data[ i - 1 ] );
}

void bcore_sink_chain_s_push_d( bcore_sink_chain_s* o, vd_t sink )
{
    const bcore_array_s* arr_p = bcore_array_s_get_typed( TYPEOF_bcore_sink_chain_s );
    bcore_array_p_push( arr_p, (bcore_array*)o, sr_asd( sink ) );
    if( o->size > 1 ) bcore_sink_a_set_consumer( o->data[ o->size - 1 ], o->data[ o->size - 2 ] );
}

void bcore_sink_chain_s_push_type( bcore_sink_chain_s* o, tp_t type )
{
    bcore_sink_chain_s_push_d( o, bcore_inst_t_create( type ) );
}

void bcore_sink_chain_s_flush( bcore_sink_chain_s* o )
{
    for( uz_t i = o->size; i > 0; i-- ) bcore_sink_a_flush( o->data[ i - 1 ] );
}

static uz_t chain_flow_snk( bcore_sink_chain_s* o, vc_t data, uz_t size )
{
    return ( o->size > 0 ) ? bcore_sink_a_push_data( o->data[ o->size - 1 ], data, size ) : 0;
}

uz_t bcore_sink_chain_s_push_data( bcore_sink_chain_s* o, vc_t data, uz_t size )
{
    return chain_flow_snk( o, data, size );
}

/**********************************************************************************************************************/

static bcore_self_s* chain_s_create_self( void )
{
    sc_t def =
    "bcore_sink_chain_s = bcore_sink"
    "{ "
      "aware_t _; "
      "aware * [] data; "
    "}";

    bcore_self_s* self = bcore_self_s_build_parse_sc( def, sizeof( bcore_sink_chain_s ) );
    bcore_self_s_push_ns_amoeba( self, chain_init_a, "init" );
    bcore_self_s_push_ns_amoeba( self, chain_down_a, "down" );
    bcore_self_s_push_ns_amoeba( self, chain_copy_a, "copy" );
    bcore_self_s_push_ns_func( self, ( fp_t )chain_interpret_body_a,   "ap_t", "interpret_body" );
    bcore_self_s_push_ns_func( self, ( fp_t )chain_flow_snk,           "bcore_fp_flow_snk",   "flow_snk" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_sink_chain_s_set_consumer, "bcore_sink_fp_set_consumer", "set_consumer" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_sink_chain_s_flush, "bcore_sink_fp_flush", "flush"    );
    return self;
}

/**********************************************************************************************************************/
/// bcore_sink_buffer_s
/**********************************************************************************************************************/

void bcore_sink_buffer_s_init( bcore_sink_buffer_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = TYPEOF_bcore_sink_buffer_s;
    o->entrepot_size = 4096;
}

void bcore_sink_buffer_s_down( bcore_sink_buffer_s* o )
{
    bcore_inst_t_down( TYPEOF_bcore_sink_buffer_s, ( bcore_inst* )o );
}

void bcore_sink_buffer_s_discard( bcore_sink_buffer_s* o )
{
    bcore_inst_t_discard( TYPEOF_bcore_sink_buffer_s, (bcore_inst*)o );
}

void bcore_sink_buffer_s_copy( bcore_sink_buffer_s* o, const bcore_sink_buffer_s* src )
{
    bcore_inst_t_copy( TYPEOF_bcore_sink_buffer_s, (bcore_inst*)o, src );
}

bcore_sink_buffer_s* bcore_sink_buffer_s_create()
{
    return bcore_inst_t_create( TYPEOF_bcore_sink_buffer_s );
}

bcore_sink_buffer_s* bcore_sink_buffer_s_clone( const bcore_sink_buffer_s* o )
{
    return bcore_inst_t_clone( TYPEOF_bcore_sink_buffer_s, (bcore_inst*)o );
}

void bcore_sink_buffer_s_flush( bcore_sink_buffer_s* o )
{
    if( o->consumer && o->size > 0 )
    {
        uz_t pushed_size = bcore_sink_a_push_data( o->consumer, o->data, o->size );
        if( pushed_size != o->size ) ERR( "Consumer accepted %zu of %zu bytes.", pushed_size, o->size );
        o->size = 0;
    }
}

static uz_t buffer_flow_snk( bcore_sink_buffer_s* o, vc_t data, uz_t size )
{
    uz_t new_size = o->size + size;
    if( new_size > o->space )
    {
        uz_t new_space = ( o->space * 2 > new_size ) ? o->space * 2 : new_size;
        o->data = bcore_bn_alloc( o->data, o->space, new_space, &o->space );
    }
    bcore_memcpy( o->data + o->size, data, size );
    o->size += size;
    if( o->consumer && o->size >= o->entrepot_size ) bcore_sink_buffer_s_flush( o );
    return size;
}

uz_t bcore_sink_buffer_s_push_data( bcore_sink_buffer_s* o, vc_t data, uz_t size )
{
    return buffer_flow_snk( o, data, size );
}

void bcore_sink_chain_s_set_consumer( bcore_sink_chain_s* o, vd_t consumer )
{
    if( o->size == 0 ) ERR( "chain is empty" );
    bcore_sink_a_set_consumer( o->data[ 0 ], consumer );
}

/**********************************************************************************************************************/

static bcore_self_s* buffer_s_create_self( void )
{
    sc_t def =
    "bcore_sink_buffer_s = bcore_sink"
    "{ "
      "aware_t _; "
      "u0_t [] data; "
      "private vd_t consumer; "
      "uz_t entrepot_size; "
    "}";
    bcore_self_s* self = bcore_self_s_build_parse_sc( def, sizeof( bcore_sink_buffer_s ) );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_sink_buffer_s_init,         "bcore_fp_init",              "init"  );
    bcore_self_s_push_ns_func( self, ( fp_t )buffer_flow_snk,                  "bcore_fp_flow_snk",          "flow_snk"  );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_sink_buffer_s_set_consumer, "bcore_sink_fp_set_consumer", "set_consumer" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_sink_buffer_s_flush,        "bcore_sink_fp_flush",        "flush" );
    return self;
}

void bcore_sink_buffer_s_set_consumer( bcore_sink_buffer_s* o, vd_t consumer )
{
    o->consumer = consumer;
}

/**********************************************************************************************************************/
/// bcore_sink_file_s
/**********************************************************************************************************************/

/** File data sink (copyable, serializable)
  * The 'open' state is not copied;
  */
typedef struct bcore_sink_file_s
{
    aware_t _;
    st_s* name;
    FILE* handle;
} bcore_sink_file_s;

static void file_init_a( bcore_nucleus_s* nc )
{
    bcore_sink_file_s* o = nc->client;
    nc->default_handler( nc );
    o->_ = TYPEOF_bcore_sink_file_s;
}

void bcore_sink_file_s_init( bcore_sink_file_s* o )
{
    bcore_inst_t_init( TYPEOF_bcore_sink_file_s, (bcore_inst*)o );
}

static void file_down_a( bcore_nucleus_s* nc )
{
    bcore_sink_file_s* o = nc->client;
    bcore_sink_file_s_close( o );
    nc->default_handler( nc );
}

void bcore_sink_file_s_down( bcore_sink_file_s* o )
{
    bcore_inst_t_down( TYPEOF_bcore_sink_file_s, ( bcore_inst* )o );
}

static void file_copy_a( bcore_nucleus_s* nc )
{
    bcore_sink_file_s* o = nc->client;
    bcore_sink_file_s_close( o );
    nc->default_handler( nc );
}

void bcore_sink_file_s_copy( bcore_sink_file_s* o, const bcore_sink_file_s* src )
{
    bcore_inst_t_copy( TYPEOF_bcore_sink_file_s, (bcore_inst*)o, src );
}

void bcore_sink_file_s_discard( bcore_sink_file_s* o )
{
    bcore_inst_t_discard( TYPEOF_bcore_sink_file_s, (bcore_inst*)o );
}

bcore_sink_file_s* bcore_sink_file_s_create()
{
    return bcore_inst_t_create( TYPEOF_bcore_sink_file_s );
}

bcore_sink_file_s* bcore_sink_file_s_create_name( sc_t name )
{
    bcore_sink_file_s* o = bcore_sink_file_s_create();
    o->name = st_s_create_sc( name );
    return o;
}

sc_t bcore_sink_file_s_get_name( const bcore_sink_file_s* o )
{
    return o->name->sc;
}

bcore_sink_file_s* bcore_sink_file_s_clone( const bcore_sink_file_s* o )
{
    return bcore_inst_t_clone( TYPEOF_bcore_sink_file_s, (bcore_inst*)o );
}

void bcore_sink_file_s_open( bcore_sink_file_s* o )
{
    if( !o->name ) ERR( "No file name specified." );
    if( o->handle ) bcore_sink_file_s_close( o );
    o->handle = fopen( o->name->sc, "wb" );
    if( !o->handle )
    {
        st_s* msg = st_s_createf( "Error opening file %s:\n", o->name->sc );
        perror( msg->sc );
        st_s_discard( msg );
        abort();
    }
}

void bcore_sink_file_s_close( bcore_sink_file_s* o )
{
    if( !o->handle ) return;
    if( fclose( o->handle ) != 0 )
    {
        st_s* msg = st_s_createf( "Error closing file %s:\n", o->name->sc );
        perror( msg->sc );
        st_s_discard( msg );
        abort();
    }
    o->handle = NULL;
}

void bcore_sink_file_s_flush( bcore_sink_file_s* o )
{
    if( !o->handle ) return;
    if( fflush( o->handle ) != 0 )
    {
        st_s* msg = st_s_createf( "Error flushing file %s:\n", o->name->sc );
        perror( msg->sc );
        st_s_discard( msg );
        abort();
    }
}

static void file_interpret_body_a( vd_t nc )
{
    struct { ap_t a; vc_t p; vc_t inter; vd_t sink; tp_t type; vd_t obj; } * nc_l = nc;
    bcore_sink_file_s_close( nc_l->obj );
    nc_l->a( nc ); // default
}

static uz_t file_flow_snk( bcore_sink_file_s* o, vc_t data, uz_t size )
{
    if( !o->handle ) bcore_sink_file_s_open( o );
    uz_t wsize = fwrite( data, 1, size, o->handle );
    if( wsize != size )
    {
        if( ferror( o->handle ) )
        {
            st_s* msg = st_s_createf( "Error writing to file %s:\n", o->name->sc );
            perror( msg->sc );
            st_s_discard( msg );
        }
    }
    return wsize;
}

uz_t bcore_sink_file_s_push_data( bcore_sink_file_s* o, vc_t data, uz_t size )
{
    return file_flow_snk( o, data, size );
}

/**********************************************************************************************************************/

static bcore_self_s* file_s_create_self( void )
{
    sc_t def =
    "bcore_sink_file_s = bcore_sink"
    "{ "
      "aware_t _; "
      "st_s* name; "
      "private vd_t handle; "
    "}";

    bcore_self_s* self = bcore_self_s_build_parse_sc( def, sizeof( bcore_sink_file_s ) );
    bcore_self_s_push_ns_amoeba( self, file_init_a, "init" );
    bcore_self_s_push_ns_amoeba( self, file_down_a, "down" );
    bcore_self_s_push_ns_amoeba( self, file_copy_a, "copy" );
    bcore_self_s_push_ns_func( self, ( fp_t )file_interpret_body_a,   "ap_t", "interpret_body"        );
    bcore_self_s_push_ns_func( self, ( fp_t )file_flow_snk,           "bcore_fp_flow_snk", "flow_snk" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_sink_file_s_flush, "bcore_sink_fp_flush", "flush"  );
    return self;
}

/**********************************************************************************************************************/

bcore_sink_chain_s* bcore_sink_open_file( sc_t file_name )
{
    bcore_sink_chain_s* chain = bcore_sink_chain_s_create();
    bcore_sink_chain_s_push_d( chain, bcore_sink_file_s_create_name( file_name ) );
    bcore_sink_chain_s_push_d( chain, bcore_inst_t_create( typeof( "bcore_sink_buffer_s" ) ) );
    return chain;
}

/**********************************************************************************************************************/

vd_t bcore_sinks_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_sinks" ) ) )
    {
        case TYPEOF_init1:
        {
            bcore_flect_define_creator( typeof( "bcore_sink_buffer_s" ), buffer_s_create_self );
            bcore_flect_define_creator( typeof( "bcore_sink_file_s"   ), file_s_create_self   );
            bcore_flect_define_creator( typeof( "bcore_sink_chain_s"  ), chain_s_create_self  );
        }
        break;

        case TYPEOF_selftest:
        {
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

