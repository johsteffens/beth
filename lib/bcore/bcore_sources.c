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

#include "bcore_sources.h"
#include "bcore_spect_inst.h"
#include "bcore_signal.h"
#include "bcore_spect_array.h"
#include "bcore_spect_translator.h"
#include "bcore_spect_interpreter.h"
#include "bcore_life.h"
#include "bcore_sinks.h"

#include <stdio.h>

/**********************************************************************************************************************/
/// bcore_source_chain_s
/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

static void chain_init_a( bcore_nucleus_s* nc )
{
    bcore_source_chain_s* o = nc->client;
    nc->default_handler( nc );
    o->_ = TYPEOF_bcore_source_chain_s;
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_chain_s_init( bcore_source_chain_s* o )
{
    bcore_inst_t_init( TYPEOF_bcore_source_chain_s, (bcore_inst*)o );
}

//----------------------------------------------------------------------------------------------------------------------

static void chain_down_a( bcore_nucleus_s* nc )
{
    nc->default_handler( nc );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_chain_s_down( bcore_source_chain_s* o )
{
    bcore_inst_t_down( TYPEOF_bcore_source_chain_s, ( bcore_inst* )o );
}

//----------------------------------------------------------------------------------------------------------------------

static void chain_copy_a( bcore_nucleus_s* nc )
{
    bcore_source_chain_s* o = nc->client;
    nc->default_handler( nc );
    for( uz_t i = 1; i < o->size; i++ ) bcore_source_a_set_supplier( o->data[ i ], o->data[ i - 1 ] );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_chain_s_copy( bcore_source_chain_s* o, const bcore_source_chain_s* src )
{
    bcore_inst_t_copy( TYPEOF_bcore_source_chain_s, (bcore_inst*)o, src );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_chain_s_discard( bcore_source_chain_s* o )
{
    bcore_inst_t_discard( TYPEOF_bcore_source_chain_s, (bcore_inst*)o );
}

//----------------------------------------------------------------------------------------------------------------------

bcore_source_chain_s* bcore_source_chain_s_create()
{
    return bcore_inst_t_create( TYPEOF_bcore_source_chain_s );
}

//----------------------------------------------------------------------------------------------------------------------

bcore_source_chain_s* bcore_source_chain_s_clone( const bcore_source_chain_s* o )
{
    return bcore_inst_t_clone( TYPEOF_bcore_source_chain_s, (bcore_inst*)o );
}

//----------------------------------------------------------------------------------------------------------------------

static void chain_interpret_body_a( vd_t nc )
{
    struct { ap_t a; vc_t p; vc_t inter; vd_t source; tp_t type; vd_t obj; } * nc_l = nc;
    nc_l->a( nc ); // default
    bcore_source_chain_s* o = nc_l->obj;
    for( uz_t i = 1; i < o->size; i++ ) bcore_source_a_set_supplier( o->data[ i ], o->data[ i - 1 ] );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_chain_s_push_d( bcore_source_chain_s* o, vd_t source )
{
    const bcore_array_s* arr_p = bcore_array_s_get_typed( TYPEOF_bcore_source_chain_s );
    bcore_array_p_push( arr_p, (bcore_array*)o, sr_asd( source ) );
    if( o->size > 1 ) bcore_source_a_set_supplier( o->data[ o->size - 1 ], o->data[ o->size - 2 ] );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_chain_s_push_type( bcore_source_chain_s* o, tp_t type )
{
    bcore_source_chain_s_push_d( o, bcore_inst_t_create( type ) );
}

//----------------------------------------------------------------------------------------------------------------------

static uz_t chain_flow_src( bcore_source_chain_s* o, vd_t data, uz_t size )
{
    return ( o->size > 0 ) ? bcore_source_a_get_data( o->data[ o->size - 1 ], data, size ) : 0;
}

//----------------------------------------------------------------------------------------------------------------------

uz_t bcore_source_chain_s_get_data(  bcore_source_chain_s* o, vd_t data, uz_t size )
{
    return chain_flow_src( o, data, size );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_chain_s_set_supplier( bcore_source_chain_s* o, vd_t supplier )
{
    if( o->size == 0 ) ERR( "chain is empty" );
    bcore_source_a_set_supplier( o->data[ 0 ], supplier );
    for( uz_t i = 1; i < o->size; i++ )
    {
        // resetting the supplier also resets of the internal state of each element
        bcore_source_a_set_supplier( o->data[ i ], o->data[ i - 1 ] );
    }
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_source_chain_s_eos( const bcore_source_chain_s* o )
{
    if( o->size == 0 ) return true;
    return bcore_source_a_eos( o->data[ o->size - 1 ] );
}

//----------------------------------------------------------------------------------------------------------------------

sc_t bcore_source_chain_s_get_file( const bcore_source_chain_s* o )
{
    if( o->size == 0 ) return "";
    return bcore_source_a_get_file( o->data[ 0 ] );
}

//----------------------------------------------------------------------------------------------------------------------

s3_t bcore_source_chain_s_get_index( const bcore_source_chain_s* o )
{
    s3_t index = 0;
    for( uz_t i = 0; i < o->size; i++ )
    {
        index += bcore_source_a_get_index( o->data[ i ] );
    }
    return index;
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_chain_s_set_index( bcore_source_chain_s* o, s3_t index )
{
    if( o->size == 0 ) ERR( "Chain is empty." );
    s3_t cur_index = bcore_source_chain_s_get_index( o );
    if( index == cur_index ) return;
    bcore_source_a_set_index( o->data[ 0 ], index );
    for( uz_t i = 1; i < o->size; i++ )
    {
        // resetting the supplier also resets of the internal state of each element
        bcore_source_a_set_supplier( o->data[ i ], o->data[ i - 1 ] );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

static void chain_context_to_sink( bcore_source_chain_s* o, bcore_sink* sink )
{
    if( o->size > 0 )
    {
        if( o->data[ 0 ] && ( *( aware_t* )o->data[ 0 ] ) == TYPEOF_bcore_source_file_s )
        {
            bcore_source_file_s* fo = o->data[ 0 ];
            bcore_sink_a_pushf( sink, "In file '%s'", bcore_source_file_s_get_name( fo ) );
            uz_t index = bcore_source_chain_s_get_index( o );
            uz_t line, col;
            st_s* context = st_s_create();
            bcore_source_file_s_get_line_col_context( fo, index, &line, &col, context );
            bcore_sink_a_pushf( sink, " at line %zu, col %zu", line, col );
            bcore_sink_a_pushf( sink, "\n%s", context->sc );
            st_s_discard( context );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void chain_parse_fv( bcore_source_chain_s* o, sc_t format, va_list args )
{
    if( o->size > 0 )
    {
        const bcore_source_s* source_p = bcore_source_s_get_aware( o->data[ o->size - 1 ] );
        if( source_p->parse_fv )
        {
            bcore_source_p_parse_fv( source_p, o->data[ o->size - 1 ], format, args );
        }
        else
        {
            bcore_source_chain_s_push_type( o, TYPEOF_bcore_source_string_s );
            chain_parse_fv( o, format, args );
        }
    }
    else
    {
        ERR( "Chain is empty" );
    }
}

//----------------------------------------------------------------------------------------------------------------------

static bcore_self_s* chain_s_create_self( void )
{
    sc_t def =

    "bcore_source_chain_s = bcore_source"
    "{ "
      "aware_t _; "
      "aware * [] data; "
    "}";

    bcore_self_s* self = bcore_self_s_build_parse_sc( def, sizeof( bcore_source_chain_s ) );
    bcore_self_s_push_ns_amoeba( self, chain_init_a, "init" );
    bcore_self_s_push_ns_amoeba( self, chain_down_a, "down" );
    bcore_self_s_push_ns_amoeba( self, chain_copy_a, "copy" );
    bcore_self_s_push_ns_func( self, ( fp_t )chain_interpret_body_a,            "ap_t",                            "interpret_body" );
    bcore_self_s_push_ns_func( self, ( fp_t )chain_flow_src,                    "bcore_fp_flow_src",               "flow_src"  );
    bcore_self_s_push_ns_func( self, ( fp_t )chain_context_to_sink,             "bcore_source_fp_context_to_sink", "context_to_sink" );
    bcore_self_s_push_ns_func( self, ( fp_t )chain_parse_fv,                    "bcore_source_fp_parse_fv",        "parse_fv" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_source_chain_s_set_supplier, "bcore_source_fp_set_supplier",    "set_supplier" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_source_chain_s_eos,          "bcore_source_fp_eos",             "eos" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_source_chain_s_get_file,     "bcore_source_fp_get_file",        "get_file" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_source_chain_s_get_index,    "bcore_source_fp_get_index",       "get_index" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_source_chain_s_set_index,    "bcore_source_fp_set_index",       "set_index" );
    return self;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// bcore_source_buffer_s
/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_buffer_s_init( bcore_source_buffer_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = TYPEOF_bcore_source_buffer_s;
    o->prefetch_size = 4096;
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_buffer_s_down( bcore_source_buffer_s* o )
{
    bcore_inst_t_down( TYPEOF_bcore_source_buffer_s, ( bcore_inst* )o );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_buffer_s_discard( bcore_source_buffer_s* o )
{
    bcore_inst_t_discard( TYPEOF_bcore_source_buffer_s, (bcore_inst*)o );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_buffer_s_copy( bcore_source_buffer_s* o, const bcore_source_buffer_s* src )
{
    bcore_inst_t_copy( TYPEOF_bcore_source_buffer_s, (bcore_inst*)o, src );
}

//----------------------------------------------------------------------------------------------------------------------

bcore_source_buffer_s* bcore_source_buffer_s_create()
{
    return bcore_inst_t_create( TYPEOF_bcore_source_buffer_s );
}

//----------------------------------------------------------------------------------------------------------------------

bcore_source_buffer_s* bcore_source_buffer_s_clone( const bcore_source_buffer_s* o )
{
    return bcore_inst_t_clone( TYPEOF_bcore_source_buffer_s, (bcore_inst*)o );
}

//----------------------------------------------------------------------------------------------------------------------

static uz_t buffer_flow_src( bcore_source_buffer_s* o, vd_t data, uz_t size )
{
    if( size < o->size - o->index )
    {
        bcore_memcpy( data, o->data + o->index, size );
        o->index += size;
        return size;
    }

    uz_t size1 = o->size - o->index;
    bcore_memcpy( data, o->data + o->index, size1 );
    o->index += size1;
    data = ( u0_t* )data + size1;
    size -= size1;
    o->size = o->index = 0;
    if( !o->ext_supplier ) return size1;

    // the code below handles the case with supplier

    if( size > o->prefetch_size ) return bcore_source_a_get_data( o->ext_supplier, data, size ) + size1;

    if( o->space < o->prefetch_size )
    {
        o->data = bcore_bn_alloc( o->data, o->space,                0, &o->space );
        o->data = bcore_bn_alloc( o->data, o->space, o->prefetch_size, &o->space );
    }
    o->size = bcore_source_a_get_data( o->ext_supplier, o->data, o->prefetch_size );

    if( o->size < o->prefetch_size ) o->ext_supplier = NULL;

    if( size < o->size )
    {
        bcore_memcpy( data, o->data, size );
        o->index += size;
        return size1 + size;
    }
    else
    {
        bcore_memcpy( data, o->data, o->size );
        o->size = o->index = 0;
        return size1 + o->size;
    }
}

//----------------------------------------------------------------------------------------------------------------------

bcore_source_buffer_s* bcore_source_buffer_s_create_from_data( vc_t data, uz_t size )
{
    bcore_source_buffer_s* buffer = bcore_source_buffer_s_create();
    buffer->data = bcore_b_alloc( buffer->data, size, &buffer->space );
    bcore_memcpy( buffer->data, data, size );
    buffer->size = size;
    return buffer;
}

//----------------------------------------------------------------------------------------------------------------------

uz_t bcore_source_buffer_s_get_data(  bcore_source_buffer_s* o, vd_t data, uz_t size )
{
    return buffer_flow_src( o, data, size );
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_source_buffer_s_eos( const bcore_source_buffer_s* o )
{
    if( o->index < o->size ) return false;
    if( o->ext_supplier    ) return bcore_source_a_eos( o->ext_supplier );
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

sc_t bcore_source_buffer_s_get_file( const bcore_source_buffer_s* o )
{
    if( o->ext_supplier ) return bcore_source_a_get_file( o->ext_supplier );
    return "";
}

//----------------------------------------------------------------------------------------------------------------------

s3_t bcore_source_buffer_s_get_index( const bcore_source_buffer_s* o )
{
    return o->index - o->size;
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_buffer_s_set_index( bcore_source_buffer_s* o, s3_t index )
{
    s3_t size = o->size;
    if( index < -size || index > 0 ) ERR( "Index is out of range." );
    o->index = index + size;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

static void buffer_context_to_sink( bcore_source_buffer_s* o, bcore_sink* sink )
{
    if( o->ext_supplier ) bcore_source_a_context_to_sink( o->ext_supplier, sink );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_buffer_s_set_supplier( bcore_source_buffer_s* o, vd_t supplier )
{
    o->ext_supplier = supplier;
    o->index = 0;
    o->size = 0;
}

//----------------------------------------------------------------------------------------------------------------------

static bcore_self_s* buffer_s_create_self( void )
{
    sc_t def =
    "bcore_source_buffer_s = bcore_source"
    "{ "
      "aware_t _; "
      "u0_t [] data; "
      "uz_t index; "
      "private vd_t supplier; "
      "uz_t prefetch_size; "
    "}";
    bcore_self_s* self = bcore_self_s_build_parse_sc( def, sizeof( bcore_source_buffer_s ) );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_source_buffer_s_init,         "bcore_fp_init",                   "init"  );
    bcore_self_s_push_ns_func( self, ( fp_t )buffer_flow_src,                    "bcore_fp_flow_src",               "flow_src"  );
    bcore_self_s_push_ns_func( self, ( fp_t )buffer_context_to_sink,             "bcore_source_fp_context_to_sink", "context_to_sink" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_source_buffer_s_set_supplier, "bcore_source_fp_set_supplier",    "set_supplier" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_source_buffer_s_eos,          "bcore_source_fp_eos",             "eos" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_source_buffer_s_get_file,     "bcore_source_fp_get_file",        "get_file" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_source_buffer_s_get_index,    "bcore_source_fp_get_index",       "get_index" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_source_buffer_s_set_index,    "bcore_source_fp_set_index",       "set_index" );
    return self;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// bcore_source_string_s
/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_string_s_init( bcore_source_string_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = TYPEOF_bcore_source_string_s;
    o->prefetch_size = 16384;
    o->refill_limit  = 4096;
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_string_s_down( bcore_source_string_s* o )
{
    bcore_inst_t_down( TYPEOF_bcore_source_string_s, ( bcore_inst* )o );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_string_s_discard( bcore_source_string_s* o )
{
    bcore_inst_t_discard( TYPEOF_bcore_source_string_s, (bcore_inst*)o );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_string_s_copy( bcore_source_string_s* o, const bcore_source_string_s* src )
{
    bcore_inst_t_copy( TYPEOF_bcore_source_string_s, (bcore_inst*)o, src );
}

//----------------------------------------------------------------------------------------------------------------------

bcore_source_string_s* bcore_source_string_s_create()
{
    return bcore_inst_t_create( TYPEOF_bcore_source_string_s );
}

//----------------------------------------------------------------------------------------------------------------------

bcore_source_string_s* bcore_source_string_s_clone( const bcore_source_string_s* o )
{
    return bcore_inst_t_clone( TYPEOF_bcore_source_string_s, (bcore_inst*)o );
}

//----------------------------------------------------------------------------------------------------------------------

bcore_source_string_s* bcore_source_string_s_create_from_string( const st_s* string )
{
    bcore_source_string_s* o = bcore_source_string_s_create();
    o->string = st_s_clone( string );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

bcore_source_string_s* bcore_source_string_s_create_from_string_d( st_s* string )
{
    bcore_source_string_s* o = bcore_source_string_s_create();
    o->string = string;
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

bcore_source_string_s* bcore_source_string_s_create_sc( sc_t sc )
{
    bcore_source_string_s* o = bcore_source_string_s_create();
    o->string = st_s_create_sc( sc );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

static void string_refill( bcore_source_string_s* o, uz_t min_remaining_size )
{
    if( !o->string ) o->string = st_s_create();
    if( o->string->size - o->index <= min_remaining_size )
    {
        if( o->index > 0 )
        {
            bcore_memmove( o->string->data, o->string->data + o->index, o->string->size - o->index );
            o->string->size -= o->index;
            o->string->data[ o->string->size ] = 0;
            o->index = 0;
        }
        uz_t refill_size = min_remaining_size - o->string->size > o->prefetch_size ? min_remaining_size - o->string->size : o->prefetch_size;
        st_s_set_min_space( o->string, o->string->size + refill_size + 1 );
        uz_t bytes_received = bcore_source_a_get_data( o->ext_supplier, o->string->data + o->string->size, refill_size );
        if( bytes_received < refill_size ) o->ext_supplier = NULL; // detach supplier when empty
        o->string->size += bytes_received;
        o->string->data[ o->string->size ] = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------

static uz_t string_flow_src( bcore_source_string_s* o, vd_t data, uz_t size )
{
    if( !o->string ) o->string = st_s_create();
    if( size > o->string->size - o->index )
    {
        // first send rest of buffer
        uz_t size1 = o->string->size - o->index;
        bcore_memcpy( data, o->string->data + o->index, size1 );
        o->index += size1;
        data = (u0_t*)data + size1;
        size -= size1;

        // refill to remaining size (if possible)
        if( o->ext_supplier ) string_refill( o, size );

        // send rest of data (if any)
        uz_t size2 = size < o->string->size - o->index ? size : o->string->size - o->index;
        bcore_memcpy( data, o->string->data + o->index, size2 );
        o->index += size2;

        return size1 + size2;
    }
    else
    {
        bcore_memcpy( data, o->string->data + o->index, size );
        o->index += size;
        return size;
    }
}

//----------------------------------------------------------------------------------------------------------------------

uz_t bcore_source_string_s_get_data(  bcore_source_string_s* o, vd_t data, uz_t size )
{
    return string_flow_src( o, data, size );
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_source_string_s_eos( const bcore_source_string_s* o )
{
    if( !o->string )
    {
        if( o->ext_supplier ) return bcore_source_a_eos( o->ext_supplier );
        return true;
    }

    if( o->index < o->string->size ) return false;
    if( o->ext_supplier ) return bcore_source_a_eos( o->ext_supplier );

    return true;
}

//----------------------------------------------------------------------------------------------------------------------

sc_t bcore_source_string_s_get_file( const bcore_source_string_s* o )
{
    if( o->ext_supplier ) return bcore_source_a_get_file( o->ext_supplier );
    return "";
}

//----------------------------------------------------------------------------------------------------------------------

s3_t bcore_source_string_s_get_index( const bcore_source_string_s* o )
{
    s3_t size = o->string ? o->string->size : 0;
    return o->index - size;
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_string_s_set_index( bcore_source_string_s* o, s3_t index )
{
    s3_t size = o->string ? o->string->size : 0;
    if( index < -size || index > 0 ) ERR( "Index is out of range." );
    o->index = index + size;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

static void string_context_to_sink( const bcore_source_string_s* o, bcore_sink* sink )
{
    if( o->chain )
    {
        if( o->chain->data[ 0 ] && ( *( aware_t* )o->chain->data[ 0 ] ) == TYPEOF_bcore_source_file_s )
        {
            bcore_source_file_s* fo = o->chain->data[ 0 ];
            bcore_sink_a_pushf( sink, "In file '%s'", bcore_source_file_s_get_name( fo ) );
            uz_t index = bcore_source_chain_s_get_index( o->chain );
            uz_t line, col;
            st_s* context = st_s_create();
            bcore_source_file_s_get_line_col_context( fo, index, &line, &col, context );
            bcore_sink_a_pushf( sink, " at line %zu, col %zu", line, col );
            bcore_sink_a_pushf( sink, "\n%s", context->sc );
            st_s_discard( context );
        }
    }
    else
    {
        st_s* context   = st_s_show_line_context( o->string, o->index );
        uz_t line = st_s_lineof( o->string, o->index );
        uz_t col  = st_s_colof( o->string, o->index );
        bcore_sink_a_pushf( sink, "At line %zu, col %zu:", line, col );
        bcore_sink_a_pushf( sink, "\n%s\n", context->sc );
        st_s_discard( context );
    }
}

//----------------------------------------------------------------------------------------------------------------------

static uz_t string_s_parse_err( vd_t arg, const st_s* string, uz_t idx, st_s* ext_msg )
{
    const bcore_source_string_s* o = arg;
    bcore_sink_a_push_fa( BCORE_STDERR, "Parse error: " );
    string_context_to_sink( o, BCORE_STDERR );
    bcore_sink_a_push_fa( BCORE_STDERR, "#<sc_t>\n", ext_msg->sc );
    bcore_exit( 1 );
    return idx;
}

//----------------------------------------------------------------------------------------------------------------------

static void string_parse_fv( bcore_source_string_s* o, sc_t format, va_list args )
{
    if( o->ext_supplier ) string_refill( o, o->refill_limit );
    if( !o->string ) ERR( "No string defined." );
    o->index = st_s_parse_efv( o->string, o->index, o->string->size, string_s_parse_err, o, format, args );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_string_s_set_supplier( bcore_source_string_s* o, vd_t supplier )
{
    o->ext_supplier = supplier;
    o->index = 0;
    if( o->string ) o->string->size = 0;
}

//----------------------------------------------------------------------------------------------------------------------

static bcore_self_s* string_s_create_self( void )
{
    sc_t def =
    "bcore_source_string_s = bcore_source "
    "{ "
      "aware_t _; "
      "st_s* string; "
      "uz_t index; "
      "private vd_t supplier; "
      "uz_t refill_limit;    "
      "uz_t prefetch_size;   "
      "private bcore_source_chain_s* chain;" // governing chain (if any)
    "}";
    bcore_self_s* self = bcore_self_s_build_parse_sc( def, sizeof( bcore_source_string_s ) );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_source_string_s_init,         "bcore_fp_init", "init" );
    bcore_self_s_push_ns_func( self, ( fp_t )string_flow_src,                    "bcore_fp_flow_src",               "flow_src"  );
    bcore_self_s_push_ns_func( self, ( fp_t )string_context_to_sink,             "bcore_source_fp_context_to_sink", "context_to_sink" );
    bcore_self_s_push_ns_func( self, ( fp_t )string_parse_fv,                    "bcore_source_fp_parse_fv",        "parse_fv"   );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_source_string_s_set_supplier, "bcore_source_fp_set_supplier",    "set_supplier"   );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_source_string_s_eos,          "bcore_source_fp_eos",             "eos" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_source_string_s_get_file,     "bcore_source_fp_get_file",        "get_file" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_source_string_s_get_index,    "bcore_source_fp_get_index",       "get_index" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_source_string_s_set_index,    "bcore_source_fp_set_index",       "set_index" );
    return self;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// bcore_source_file_s
/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

typedef struct bcore_source_file_s
{
    aware_t _;
    st_s* name;
    FILE* handle;
} bcore_source_file_s;

//----------------------------------------------------------------------------------------------------------------------

static void file_init_a( bcore_nucleus_s* nc )
{
    bcore_source_file_s* o = nc->client;
    nc->default_handler( nc );
    o->_ = TYPEOF_bcore_source_file_s;
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_file_s_init( bcore_source_file_s* o )
{
    bcore_inst_t_init( TYPEOF_bcore_source_file_s, (bcore_inst*)o );
}

//----------------------------------------------------------------------------------------------------------------------

static void file_down_a( bcore_nucleus_s* nc )
{
    bcore_source_file_s* o = nc->client;
    bcore_source_file_s_close( o );
    nc->default_handler( nc );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_file_s_down( bcore_source_file_s* o )
{
    bcore_inst_t_down( TYPEOF_bcore_source_file_s, ( bcore_inst* )o );
}

//----------------------------------------------------------------------------------------------------------------------

static void file_copy_a( bcore_nucleus_s* nc )
{
    bcore_source_file_s* o = nc->client;
    bcore_source_file_s_close( o );
    nc->default_handler( nc );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_file_s_copy( bcore_source_file_s* o, const bcore_source_file_s* src )
{
    bcore_inst_t_copy( TYPEOF_bcore_source_file_s, (bcore_inst*)o, src );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_file_s_discard( bcore_source_file_s* o )
{
    bcore_inst_t_discard( TYPEOF_bcore_source_file_s, (bcore_inst*)o );
}

//----------------------------------------------------------------------------------------------------------------------

bcore_source_file_s* bcore_source_file_s_create()
{
    return bcore_inst_t_create( TYPEOF_bcore_source_file_s );
}

//----------------------------------------------------------------------------------------------------------------------

bcore_source_file_s* bcore_source_file_s_create_name( sc_t name )
{
    bcore_source_file_s* o = bcore_source_file_s_create();
    o->name = st_s_create_sc( name );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

sc_t bcore_source_file_s_get_name( const bcore_source_file_s* o )
{
    return o->name->sc;
}

//----------------------------------------------------------------------------------------------------------------------

bcore_source_file_s* bcore_source_file_s_clone( const bcore_source_file_s* o )
{
    return bcore_inst_t_clone( TYPEOF_bcore_source_file_s, (bcore_inst*)o );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_file_s_open( bcore_source_file_s* o )
{
    if( !o->name ) ERR( "No file name specified." );
    if( o->handle ) bcore_source_file_s_close( o );
    o->handle = fopen( o->name->sc, "rb" );
    if( !o->handle )
    {
        st_s* msg = st_s_createf( "Error opening file %s:\n", o->name->sc );
        perror( msg->sc );
        st_s_discard( msg );
        bcore_abort();
    }
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_file_s_close( bcore_source_file_s* o )
{
    if( !o->handle ) return;
    if( fclose( o->handle ) != 0 )
    {
        st_s* msg = st_s_createf( "Error closing file %s:\n", o->name->sc );
        perror( msg->sc );
        st_s_discard( msg );
        bcore_abort();
    }
    o->handle = NULL;
}

//----------------------------------------------------------------------------------------------------------------------

static void file_interpret_body_a( vd_t nc )
{
    struct { ap_t a; vc_t p; vc_t inter; vd_t source; tp_t type; vd_t obj; } * nc_l = nc;
    bcore_source_file_s_close( nc_l->obj );
    nc_l->a( nc ); // default
}

//----------------------------------------------------------------------------------------------------------------------

static uz_t file_flow_src( bcore_source_file_s* o, vd_t data, uz_t size )
{
    if( !o->handle ) bcore_source_file_s_open( o );
    uz_t rsize = fread( data, 1, size, o->handle );
    if( rsize != size )
    {
        if( ferror( o->handle ) )
        {
            st_s* msg = st_s_createf( "Error reading file %s:\n", o->name->sc );
            perror( msg->sc );
            st_s_discard( msg );
        }
    }
    return rsize;
}

//----------------------------------------------------------------------------------------------------------------------

uz_t bcore_source_file_s_get_data(  bcore_source_file_s* o, vd_t data, uz_t size )
{
    return file_flow_src( o, data, size );
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_source_file_s_eos( const bcore_source_file_s* o )
{
    if( !o->handle )
    {
        if( o->name ) return false;
        return true;
    }

    return feof( o->handle );
}

//----------------------------------------------------------------------------------------------------------------------

sc_t bcore_source_file_s_get_file( const bcore_source_file_s* o )
{
    return o->name->sc;
}

//----------------------------------------------------------------------------------------------------------------------

s3_t bcore_source_file_s_get_index( const bcore_source_file_s* o )
{
    if( !o->handle ) return 0;
    return ftell( o->handle );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_file_s_set_index( bcore_source_file_s* o, s3_t index )
{
    if( !o->handle ) bcore_source_file_s_open( o );
    fseek( o->handle, index, SEEK_SET );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_file_s_get_line_col_context( bcore_source_file_s* o, s3_t index, uz_t* p_line, uz_t* p_col, st_s* context )
{
    if( !o->handle ) bcore_source_file_s_open( o );
    s3_t cur_index = ftell( o->handle );
    fseek( o->handle, 0, SEEK_SET );

    uz_t line = 1;
    uz_t col = 1;
    uz_t line_start = 0;

    for( s3_t i = 0; i < index; i++ )
    {
        char c = fgetc( o->handle );
        if( c == '\n' )
        {
            line++;
            col = 1;
            line_start = i + 1;
        }
        else
        {
            col++;
        }
    }

    if( p_line ) *p_line = line;
    if( p_col  ) *p_col  = col;

    if( context )
    {
        fseek( o->handle, line_start, SEEK_SET );
        for( uz_t i = 0; i < 128; i++ )
        {
            char c = fgetc( o->handle );
            if( c == '\n' || c <= 0) break;
            st_s_push_char( context, c );
        }
        st_s_push_char( context, '\n' );
        for( uz_t i = 1; i < col; i++ ) st_s_push_char( context, ' ' );
        st_s_push_char( context, '^' );
        st_s_push_char( context, '\n' );
    }

    fseek( o->handle, cur_index, SEEK_SET );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

static void file_context_to_sink( bcore_source_file_s* o, bcore_sink* sink )
{
    bcore_sink_a_pushf( sink, "File name: %s\n", o->name->sc );
}

//----------------------------------------------------------------------------------------------------------------------

static bcore_self_s* file_s_create_self( void )
{
    sc_t def =
    "bcore_source_file_s = bcore_source "
    "{ "
      "aware_t _; "
      "st_s* name; "
      "private vd_t handle; "
    "}";

    bcore_self_s* self = bcore_self_s_build_parse_sc( def, sizeof( bcore_source_file_s ) );
    bcore_self_s_push_ns_amoeba( self, file_init_a, "init" );
    bcore_self_s_push_ns_amoeba( self, file_down_a, "down" );
    bcore_self_s_push_ns_amoeba( self, file_copy_a, "copy" );
    bcore_self_s_push_ns_func( self, ( fp_t )file_interpret_body_a,         "ap_t",                            "interpret_body" );
    bcore_self_s_push_ns_func( self, ( fp_t )file_flow_src,                 "bcore_fp_flow_src",               "flow_src"  );
    bcore_self_s_push_ns_func( self, ( fp_t )file_context_to_sink,          "bcore_source_fp_context_to_sink", "context_to_sink" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_source_file_s_eos,       "bcore_source_fp_eos",             "eos" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_source_file_s_get_file,  "bcore_source_fp_get_file",        "get_file" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_source_file_s_get_index, "bcore_source_fp_get_index",       "get_index" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_source_file_s_set_index, "bcore_source_fp_set_index",       "set_index" );
    return self;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// bcore_source_point_s

//----------------------------------------------------------------------------------------------------------------------

BCORE_DEFINE_OBJECT_INST( bcore_inst, bcore_source_point_s )
"{"
    "aware_t _;"
    "aware bcore_source -> source;"
    "sz_t index;"
"}";

// ---------------------------------------------------------------------------------------------------------------------

void bcore_source_point_s_set( bcore_source_point_s* o, bcore_source* source )
{
    bcore_source_a_attach( &o->source, bcore_fork( source ) );
    o->index = bcore_source_a_get_index( source );
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_source_point_s_context_to_sink( const bcore_source_point_s* o, bcore_sink* sink )
{
    if( o->source )
    {
        s3_t index = bcore_source_a_get_index( o->source );
        bcore_source_a_set_index( o->source, o->index );
        bcore_source_a_context_to_sink( o->source, sink );
        bcore_source_a_set_index( o->source, index );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_source_point_s_parse_err_fv( const bcore_source_point_s* o, sc_t format, va_list args )
{
    if( o->source )
    {
        bcore_source_a_set_index( o->source, o->index );
        bcore_source_a_parse_err_fv( o->source, format, args );
    }
    else
    {
        bcore_err_fv( format, args );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_source_point_s_parse_err_fa( const bcore_source_point_s* o, sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    bcore_source_point_s_parse_err_fv( o, format, args );
    va_end( args );
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_source_point_s_parse_msg_to_sink_fv( const bcore_source_point_s* o, bcore_sink* sink, sc_t format, va_list args )
{
    if( o->source )
    {
        s3_t index = bcore_source_a_get_index( o->source );
        bcore_source_a_set_index( o->source, o->index );
        bcore_source_a_parse_msg_to_sink_fv( o->source, sink, format, args );
        bcore_source_a_set_index( o->source, index );
    }
    else
    {
        if( sink ) bcore_sink_a_push_fv( sink, format, args );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_source_point_s_parse_msg_to_sink_fa( const bcore_source_point_s* o, bcore_sink* sink, sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    bcore_source_point_s_parse_msg_to_sink_fv( o, sink, format, args );
    va_end( args );
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_source_point_s_parse_msg_fv( const bcore_source_point_s* o, sc_t format, va_list args )
{
    if( o->source )
    {
        s3_t index = bcore_source_a_get_index( o->source );
        bcore_source_a_set_index( o->source, o->index );
        bcore_source_a_parse_msg_fv( o->source, format, args );
        bcore_source_a_set_index( o->source, index );
    }
    else
    {
        bcore_msg_fv( format, args );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_source_point_s_parse_msg_fa( const bcore_source_point_s* o, sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    bcore_source_point_s_parse_msg_fv( o, format, args );
    va_end( args );
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

bcore_source_chain_s* bcore_source_open_file( sc_t file_name )
{
    bcore_source_chain_s* chain = bcore_source_chain_s_create();
    bcore_source_chain_s_push_d( chain, bcore_source_file_s_create_name( file_name ) );
    bcore_source_chain_s_push_d( chain, bcore_inst_t_create( TYPEOF_bcore_source_string_s ) );
    bcore_source_string_s* string = chain->data[ chain->size - 1 ];
    string->chain = chain;
    return chain;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// Testing
/**********************************************************************************************************************/

#include "bcore_spect_compare.h"

//----------------------------------------------------------------------------------------------------------------------

static st_s* sources_selftest( void )
{
    st_s* msg = st_s_create();
    bcore_life_s* l = bcore_life_s_create();

    bcore_flect_define_parse_sc( "chain_test_aware_arr = { aware_t _; st_s [] arr; }" );
    sr_s arr = bcore_life_s_push_sr( l, bcore_inst_t_create_sr( typeof( "chain_test_aware_arr" ) ) );
    arr = sr_cp( arr, TYPEOF_bcore_array_s );
    for( uz_t i = 0; i < 20000; i++ ) bcore_array_x_push( arr, sr_asd( st_s_createf( "line of text %zu", i ) ) );

    // write object to file
    {
        bcore_sink_chain_s* chain = bcore_sink_open_file( "test/test01.txt" );
        bcore_translate_x( bcore_inst_t_create_sr( typeof( "bcore_txt_ml_translator_s" ) ), arr, sr_awd( chain ) );
        bcore_sink_chain_s_discard( chain );
    }

    // create object from file
    {
        bcore_source_chain_s* chain = bcore_life_s_push_aware( l, bcore_source_open_file( "test/test01.txt" ) );
        sr_s chain_clone = sr_awd( bcore_life_s_push_aware( l, bcore_inst_a_clone( (bcore_inst*)chain ) ) );
        sr_s sr = bcore_interpret_x( bcore_inst_t_create_sr( typeof( "bcore_txt_ml_interpreter_s" ) ), chain_clone );
        sr = bcore_life_s_push_sr( l, sr );
        if( bcore_compare_sr( sr, arr ) != 0 ) ERR( "%s", bcore_diff_sr( sr, arr )->sc );
    }

    // random access
    {
        {
            bcore_sink_chain_s* chain = bcore_sink_open_file( "test/test02.txt" );
            for( uz_t i = 0; i < 10000; i++ ) bcore_sink_a_push_string_d( (bcore_sink*)chain, st_s_create_fa( "line of text #<uz_t>\n", i ) );
            bcore_sink_chain_s_discard( chain );
        }

        {
            bcore_source_chain_s* chain = bcore_source_open_file( "test/test02.txt" );
            bcore_arr_uz_s* arr = bcore_arr_uz_s_create();
            for( uz_t i = 0; i < 10000; i++ )
            {
                bcore_arr_uz_s_push( arr, bcore_source_a_get_index( (bcore_source*)chain ) );
                uz_t v = 0;
                bcore_source_a_parse_fa( (bcore_source*)chain, "line of text #<uz_t*>\n", &v );
                ASSERT( i == v );
            }

            u2_t rv = 1234;

            for( uz_t i = 0; i < 10000; i++ )
            {
                rv = bcore_xsg1_u2( rv );
                uz_t idx = rv % arr->size;
                bcore_source_a_set_index( (bcore_source*)chain, arr->data[ idx ] );
                uz_t v = 0;
                bcore_source_a_parse_fa( (bcore_source*)chain, "line of text #<uz_t*>\n", &v );
                ASSERT( idx == v );
            }

            bcore_arr_uz_s_discard( arr );
            bcore_source_chain_s_discard( chain );
        }
    }

    bcore_life_s_discard( l );
    return msg;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

vd_t bcore_sources_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_sources" ) ) )
    {
        case TYPEOF_init1:
        {
            bcore_flect_define_creator( typeof( "bcore_source_string_s"   ), string_s_create_self );
            bcore_flect_define_creator( typeof( "bcore_source_buffer_s"   ), buffer_s_create_self );
            bcore_flect_define_creator( typeof( "bcore_source_file_s"     ), file_s_create_self   );
            bcore_flect_define_creator( typeof( "bcore_source_chain_s"    ), chain_s_create_self  );

            BCORE_REGISTER_OBJECT( bcore_source_point_s );
        }
        break;

        case TYPEOF_selftest:
        {
            return sources_selftest();
        }
        break;

        default: break;
    }

    return NULL;
}

//----------------------------------------------------------------------------------------------------------------------

