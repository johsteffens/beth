/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#include "bcore_sources.h"
#include "bcore_spect_inst.h"
#include "bcore_quicktypes.h"
#include "bcore_spect_array.h"
#include "bcore_spect_translator.h"
#include "bcore_spect_interpreter.h"
#include "bcore_life.h"
#include "bcore_sinks.h"

#include <stdio.h>

/**********************************************************************************************************************/
/// bcore_source_chain_s
/**********************************************************************************************************************/

static void chain_init_a( vd_t nc )
{
    struct { ap_t a; vc_t p; bcore_source_chain_s* o; } * nc_l = nc;
    nc_l->a( nc ); // default
    nc_l->o->_ = TYPEOF_bcore_source_chain_s;
}

void bcore_source_chain_s_init( bcore_source_chain_s* o )
{
    bcore_inst_typed_init( TYPEOF_bcore_source_chain_s, o );
}

static void chain_down_a( vd_t nc )
{
    struct { ap_t a; vc_t p; bcore_source_chain_s* o; } * nc_l = nc;
    nc_l->a( nc ); // default
}

void bcore_source_chain_s_down( bcore_source_chain_s* o )
{
    bcore_inst_typed_down( TYPEOF_bcore_source_chain_s, o );
}

static void chain_copy_a( vd_t nc )
{
    struct { ap_t a; vc_t p; bcore_source_chain_s* dst; } * nc_l = nc;
    nc_l->a( nc ); // default
    bcore_source_chain_s* o = nc_l->dst;
    for( sz_t i = 1; i < o->size; i++ ) bcore_source_aware_set_supplier( o->data[ i ], o->data[ i - 1 ] );
}

void bcore_source_chain_s_copy( bcore_source_chain_s* o, const bcore_source_chain_s* src )
{
    bcore_inst_typed_copy( TYPEOF_bcore_source_chain_s, o, src );
}

void bcore_source_chain_s_discard( bcore_source_chain_s* o )
{
    bcore_inst_typed_discard( TYPEOF_bcore_source_chain_s, o );
}

bcore_source_chain_s* bcore_source_chain_s_create()
{
    return bcore_inst_typed_create( TYPEOF_bcore_source_chain_s );
}

bcore_source_chain_s* bcore_source_chain_s_clone( const bcore_source_chain_s* o )
{
    return bcore_inst_typed_clone( TYPEOF_bcore_source_chain_s, o );
}

static void chain_interpret_body_a( vd_t nc )
{
    struct { ap_t a; vc_t p; vc_t inter; vd_t source; tp_t type; vd_t obj; } * nc_l = nc;
    nc_l->a( nc ); // default
    bcore_source_chain_s* o = nc_l->obj;
    for( sz_t i = 1; i < o->size; i++ ) bcore_source_aware_set_supplier( o->data[ i ], o->data[ i - 1 ] );
}

void bcore_source_chain_s_push_d( bcore_source_chain_s* o, vd_t source )
{
    const bcore_array_s* arr_p = bcore_array_s_get_typed( TYPEOF_bcore_source_chain_s );
    bcore_array_spect_push( arr_p, o, sr_asd( source ) );
    if( o->size > 1 ) bcore_source_aware_set_supplier( o->data[ o->size - 1 ], o->data[ o->size - 2 ] );
}

void bcore_source_chain_s_push_type( bcore_source_chain_s* o, tp_t type )
{
    bcore_source_chain_s_push_d( o, bcore_inst_typed_create( type ) );
}

static sz_t chain_flow_src( bcore_source_chain_s* o, vd_t data, sz_t size )
{
    return ( o->size > 0 ) ? bcore_source_aware_get_data( o->data[ o->size - 1 ], data, size ) : 0;
}

sz_t bcore_source_chain_s_get_data(  bcore_source_chain_s* o, vd_t data, sz_t size )
{
    return chain_flow_src( o, data, size );
}

void bcore_source_chain_s_set_supplier( bcore_source_chain_s* o, vd_t supplier )
{
    if( o->size == 0 ) ERR( "chain is empty" );
    bcore_source_aware_set_supplier( o->data[ 0 ], supplier );
}

/**********************************************************************************************************************/

static void chain_p_errorvf( bcore_source_chain_s* o, sc_t format, va_list args )
{
    if( o->size > 0 )
    {
        st_s* msg       = st_s_create();
        st_s_pushf( msg, "Parse error" );
        if( o->data[ 0 ] && ( *( aware_t* )o->data[ 0 ] ) == TYPEOF_bcore_source_file_s )
        {
            bcore_source_file_s* fo = o->data[ 0 ];
            st_s_pushf( msg, " in file '%s'", fo->name->sc );
        }

        if( o->data[ o->size - 1 ] && ( *( aware_t* )o->data[ o->size - 1 ] ) == TYPEOF_bcore_source_string_s )
        {
            bcore_source_string_s* so = o->data[ o->size - 1 ];
            st_s* context   = st_s_show_line_context( so->string, so->index );
            sz_t line = st_s_lineof( so->string, so->index );
            sz_t col  = st_s_colof( so->string, so->index );
            st_s_pushf( msg, " at line %zu, col %zu", line + so->preceding_lines, col );
            st_s_pushf( msg, "\n%s", context->sc );
            st_s_discard( context );
        }

        st_s_push_st_d( msg, st_s_createvf( format, args ) );
        bcore_err( "\n%s", msg->sc );
        st_s_discard( msg );
    }
    else
    {
        ERR( "bcore_source_chain_s:\n%s\n", st_s_createvf( format, args )->sc );
    }
}

static void chain_parse_fv( bcore_source_chain_s* o, sc_t format, va_list args )
{
    if( o->size > 0 )
    {
        const bcore_source_s* source_p = bcore_source_s_get_aware( o->data[ o->size - 1 ] );
        if( source_p->fp_parse_fv )
        {
            bcore_source_spect_parse_fv( source_p, o->data[ o->size - 1 ], format, args );
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

static bcore_flect_self_s* chain_s_create_self( void )
{
    sc_t def =

    "bcore_source_chain_s = "
    "{ "
      "aware_t _; "
      "aware * [] data; "
    "}";

    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bcore_source_chain_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )chain_init_a, "ap_t", "init" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )chain_down_a, "ap_t", "down" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )chain_copy_a, "ap_t", "copy" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )chain_interpret_body_a, "ap_t", "interpret_body" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )chain_flow_src,  "bcore_fp_flow_src", "flow_src"  );
    bcore_flect_self_s_push_external_func( self, ( fp_t )chain_p_errorvf, "bcore_fp_logvf",    "p_errorvf" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )chain_parse_fv,   "bcore_source_fp_parse_fv", "parse_fv" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_source_chain_s_set_supplier, "bcore_source_fp_set_supplier", "set_supplier" );
    return self;
}

/**********************************************************************************************************************/
/// bcore_source_buffer_s
/**********************************************************************************************************************/

void bcore_source_buffer_s_init( bcore_source_buffer_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = TYPEOF_bcore_source_buffer_s;
    o->prefetch_size = 4096;
}

void bcore_source_buffer_s_down( bcore_source_buffer_s* o )
{
    bcore_inst_typed_down( TYPEOF_bcore_source_buffer_s, o );
}

void bcore_source_buffer_s_discard( bcore_source_buffer_s* o )
{
    bcore_inst_typed_discard( TYPEOF_bcore_source_buffer_s, o );
}

void bcore_source_buffer_s_copy( bcore_source_buffer_s* o, const bcore_source_buffer_s* src )
{
    bcore_inst_typed_copy( TYPEOF_bcore_source_buffer_s, o, src );
}

bcore_source_buffer_s* bcore_source_buffer_s_create()
{
    return bcore_inst_typed_create( TYPEOF_bcore_source_buffer_s );
}

bcore_source_buffer_s* bcore_source_buffer_s_clone( const bcore_source_buffer_s* o )
{
    return bcore_inst_typed_clone( TYPEOF_bcore_source_buffer_s, o );
}

static sz_t buffer_flow_src( bcore_source_buffer_s* o, vd_t data, sz_t size )
{
    if( size < o->size - o->index )
    {
        bcore_memcpy( data, o->data + o->index, size );
        o->index += size;
        return size;
    }

    sz_t size1 = o->size - o->index;
    bcore_memcpy( data, o->data + o->index, size1 );
    o->index += size1;
    data = ( u0_t* )data + size1;
    size -= size1;
    o->size = o->index = 0;
    if( !o->ext_supplier ) return size1;

    // the code below handles the case with supplier

    if( size > o->prefetch_size ) return bcore_source_aware_get_data( o->ext_supplier, data, size ) + size1;

    if( o->space < o->prefetch_size )
    {
        o->data = bcore_bn_alloc( o->data, o->space,                0, &o->space );
        o->data = bcore_bn_alloc( o->data, o->space, o->prefetch_size, &o->space );
    }
    o->size = bcore_source_aware_get_data( o->ext_supplier, o->data, o->prefetch_size );

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

bcore_source_buffer_s* bcore_source_buffer_s_create_from_data( vc_t data, sz_t size )
{
    bcore_source_buffer_s* buffer = bcore_source_buffer_s_create();
    buffer->data = bcore_b_alloc( buffer->data, size, &buffer->space );
    bcore_memcpy( buffer->data, data, size );
    buffer->size = size;
    return buffer;
}

sz_t bcore_source_buffer_s_get_data(  bcore_source_buffer_s* o, vd_t data, sz_t size )
{
    return buffer_flow_src( o, data, size );
}

/**********************************************************************************************************************/

static void buffer_p_errorvf( bcore_source_buffer_s* o, sc_t format, va_list args )
{
    if( o->ext_supplier )
    {
        bcore_source_aware_parse_errvf( o->ext_supplier, format, args );
    }
    else
    {
        bcore_errv( "bcore_source_buffer_s error:\n%s\n", args );
    }
}

static bcore_flect_self_s* buffer_s_create_self( void )
{
    sc_t def =
    "bcore_source_buffer_s = "
    "{ "
      "aware_t _; "
      "u0_t [] data; "
      "sz_t index; "
      "private vd_t supplier; "
      "sz_t prefetch_size; "
    "}";
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bcore_source_buffer_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_source_buffer_s_init,  "bcore_fp_init",     "init"  );
    bcore_flect_self_s_push_external_func( self, ( fp_t )buffer_flow_src,             "bcore_fp_flow_src", "flow_src"  );
    bcore_flect_self_s_push_external_func( self, ( fp_t )buffer_p_errorvf,            "bcore_fp_logvf",    "p_errorvf" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_source_buffer_s_set_supplier, "bcore_source_fp_set_supplier", "set_supplier" );
    return self;
}

void bcore_source_buffer_s_set_supplier( bcore_source_buffer_s* o, vd_t supplier )
{
    o->ext_supplier = supplier;
}

/**********************************************************************************************************************/
/// bcore_source_string_s
/**********************************************************************************************************************/

void bcore_source_string_s_init( bcore_source_string_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = TYPEOF_bcore_source_string_s;
    o->prefetch_size = 16384;
    o->refill_limit  = 4096;
}

void bcore_source_string_s_down( bcore_source_string_s* o )
{
    bcore_inst_typed_down( TYPEOF_bcore_source_string_s, o );
}

void bcore_source_string_s_discard( bcore_source_string_s* o )
{
    bcore_inst_typed_discard( TYPEOF_bcore_source_string_s, o );
}

void bcore_source_string_s_copy( bcore_source_string_s* o, const bcore_source_string_s* src )
{
    bcore_inst_typed_copy( TYPEOF_bcore_source_string_s, o, src );
}

bcore_source_string_s* bcore_source_string_s_create()
{
    return bcore_inst_typed_create( TYPEOF_bcore_source_string_s );
}

bcore_source_string_s* bcore_source_string_s_clone( const bcore_source_string_s* o )
{
    return bcore_inst_typed_clone( TYPEOF_bcore_source_string_s, o );
}

bcore_source_string_s* bcore_source_string_s_create_from_string( const st_s* string )
{
    bcore_source_string_s* o = bcore_source_string_s_create();
    o->string = st_s_clone( string );
    return o;
}

bcore_source_string_s* bcore_source_string_s_create_from_string_d( st_s* string )
{
    bcore_source_string_s* o = bcore_source_string_s_create();
    o->string = string;
    return o;
}

bcore_source_string_s* bcore_source_string_s_create_sc( sc_t sc )
{
    bcore_source_string_s* o = bcore_source_string_s_create();
    o->string = st_s_create_sc( sc );
    return o;
}

static void string_refill( bcore_source_string_s* o, sz_t min_remaining_size )
{
    if( !o->string ) o->string = st_s_create();
    if( o->string->size - o->index <= min_remaining_size )
    {
        if( o->index > 0 )
        {
            o->preceding_lines += st_s_count_char( o->string, 0, o->index, '\n' );
            bcore_memmove( o->string->data, o->string->data + o->index, o->string->size - o->index );
            o->string->size -= o->index;
            o->string->data[ o->string->size ] = 0;
            o->index = 0;
        }
        sz_t refill_size = min_remaining_size - o->string->size > o->prefetch_size ? min_remaining_size - o->string->size : o->prefetch_size;
        st_s_set_min_space( o->string, o->string->size + refill_size + 1 );
        sz_t bytes_received = bcore_source_aware_get_data( o->ext_supplier, o->string->data + o->string->size, refill_size );
        if( bytes_received < refill_size ) o->ext_supplier = NULL; // detach supplier when empty
        o->string->size += bytes_received;
        o->string->data[ o->string->size ] = 0;
    }
}

static sz_t string_flow_src( bcore_source_string_s* o, vd_t data, sz_t size )
{
    if( !o->string ) o->string = st_s_create();
    if( size > o->string->size - o->index )
    {
        // first send rest of buffer
        sz_t size1 = o->string->size - o->index;
        bcore_memcpy( data, o->string->data + o->index, size1 );
        o->index += size1;
        data = (u0_t*)data + size1;
        size -= size1;

        // refill to remaining size (if possible)
        if( o->ext_supplier ) string_refill( o, size );

        // send rest of data (if any)
        sz_t size2 = size < o->string->size - o->index ? size : o->string->size - o->index;
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

sz_t bcore_source_string_s_get_data(  bcore_source_string_s* o, vd_t data, sz_t size )
{
    return string_flow_src( o, data, size );
}

/**********************************************************************************************************************/

static void string_p_errorvf( bcore_source_string_s* o, sc_t format, va_list args )
{
    st_s* context   = st_s_show_line_context( o->string, o->index );
    st_s* msg       = st_s_create();
    st_s_pushf( msg, "Parse error" );
    sz_t line = st_s_lineof( o->string, o->index );
    sz_t col  = st_s_colof( o->string, o->index );
    st_s_pushf( msg, " at line %zu, col %zu:", line + o->preceding_lines, col );
    st_s_pushf( msg, "\n%s\n", context->sc );
    st_s_push_st_d( msg, st_s_createvf( format, args ) );
    bcore_err( "%s", msg->sc );
    st_s_discard( msg );
    st_s_discard( context );
}

static void string_parse_fv( bcore_source_string_s* o, sc_t format, va_list args )
{
    if( o->ext_supplier ) string_refill( o, o->refill_limit );
    if( !o->string ) ERR( "No string defined." );
    o->index = st_s_parse_fv( o->string, o->index, o->string->size, format, args );
}

static bcore_flect_self_s* string_s_create_self( void )
{
    sc_t def =
    "bcore_source_string_s = "
    "{ "
      "aware_t _; "
      "st_s* string; "
      "sz_t index; "
      "private vd_t supplier; "
      "sz_t preceding_lines; "
      "sz_t refill_limit;    "
      "sz_t prefetch_size;   "
    "}";
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bcore_source_string_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_source_string_s_init,  "bcore_fp_init", "init" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )string_flow_src,  "bcore_fp_flow_src",       "flow_src"  );
    bcore_flect_self_s_push_external_func( self, ( fp_t )string_p_errorvf, "bcore_fp_logvf",          "p_errorvf" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )string_parse_fv,   "bcore_source_fp_parse_fv", "parse_fv"   );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_source_string_s_set_supplier,   "bcore_source_fp_set_supplier", "set_supplier"   );
    return self;
}

void bcore_source_string_s_set_supplier( bcore_source_string_s* o, vd_t supplier )
{
    o->ext_supplier = supplier;
}

/**********************************************************************************************************************/
/// bcore_source_file_s
/**********************************************************************************************************************/

static void file_init_a( vd_t nc )
{
    struct { ap_t a; vc_t p; bcore_source_file_s* o; } * nc_l = nc;
    nc_l->a( nc ); // default
    nc_l->o->_ = TYPEOF_bcore_source_file_s;
}

void bcore_source_file_s_init( bcore_source_file_s* o )
{
    bcore_inst_typed_init( TYPEOF_bcore_source_file_s, o );
}

static void file_down_a( vd_t nc )
{
    struct { ap_t a; vc_t p; bcore_source_file_s* o; } * nc_l = nc;
    bcore_source_file_s_close( nc_l->o );
    nc_l->a( nc ); // default
}

void bcore_source_file_s_down( bcore_source_file_s* o )
{
    bcore_inst_typed_down( TYPEOF_bcore_source_file_s, o );
}

static void file_copy_a( vd_t nc )
{
    struct { ap_t a; vc_t p; bcore_source_file_s* dst; } * nc_l = nc;
    bcore_source_file_s_close( nc_l->dst );
    nc_l->a( nc ); // default
}

void bcore_source_file_s_copy( bcore_source_file_s* o, const bcore_source_file_s* src )
{
    bcore_inst_typed_copy( TYPEOF_bcore_source_file_s, o, src );
}

void bcore_source_file_s_discard( bcore_source_file_s* o )
{
    bcore_inst_typed_discard( TYPEOF_bcore_source_file_s, o );
}

bcore_source_file_s* bcore_source_file_s_create()
{
    return bcore_inst_typed_create( TYPEOF_bcore_source_file_s );
}

bcore_source_file_s* bcore_source_file_s_create_name( sc_t name )
{
    bcore_source_file_s* o = bcore_source_file_s_create();
    o->name = st_s_create_sc( name );
    return o;
}

bcore_source_file_s* bcore_source_file_s_clone( const bcore_source_file_s* o )
{
    return bcore_inst_typed_clone( TYPEOF_bcore_source_file_s, o );
}

void bcore_source_file_s_open( bcore_source_file_s* o )
{
    if( !o->name ) ERR( "No file name specified." );
    if( o->handle ) bcore_source_file_s_close( o->handle );
    o->handle = fopen( o->name->sc, "rb" );
    if( !o->handle )
    {
        st_s* msg = st_s_createf( "Error opening file %s:\n", o->name->sc );
        perror( msg->sc );
        st_s_discard( msg );
        abort();
    }
}

void bcore_source_file_s_close( bcore_source_file_s* o )
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

static void file_interpret_body_a( vd_t nc )
{
    struct { ap_t a; vc_t p; vc_t inter; vd_t source; tp_t type; vd_t obj; } * nc_l = nc;
    bcore_source_file_s_close( nc_l->obj );
    nc_l->a( nc ); // default
}

static sz_t file_flow_src( bcore_source_file_s* o, vd_t data, sz_t size )
{
    if( !o->handle ) bcore_source_file_s_open( o );
    sz_t rsize = fread( data, 1, size, o->handle );
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

sz_t bcore_source_file_s_get_data(  bcore_source_file_s* o, vd_t data, sz_t size )
{
    return file_flow_src( o, data, size );
}

/**********************************************************************************************************************/

static void file_p_errorvf( bcore_source_file_s* o, sc_t format, va_list args )
{
    ERR( "File name: %s\n%s\n", o->name->sc, st_s_createvf( format, args )->sc );
}

static bcore_flect_self_s* file_s_create_self( void )
{
    sc_t def =
    "bcore_source_file_s = "
    "{ "
      "aware_t _; "
      "st_s* name; "
      "private vd_t handle; "
    "}";

    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bcore_source_file_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )file_init_a, "ap_t", "init" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )file_down_a, "ap_t", "down" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )file_copy_a, "ap_t", "copy" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )file_interpret_body_a, "ap_t", "interpret_body" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )file_flow_src,  "bcore_fp_flow_src", "flow_src"  );
    bcore_flect_self_s_push_external_func( self, ( fp_t )file_p_errorvf, "bcore_fp_logvf",    "p_errorvf" );
    return self;
}

/**********************************************************************************************************************/
/// Testing
/**********************************************************************************************************************/

#include "bcore_spect_compare.h"

static st_s* sources_selftest( void )
{
    st_s* msg = st_s_create();
    bcore_life_s* l = bcore_life_s_create();

    bcore_flect_define_parse_sc( "chain_test_aware_arr = { aware_t _; st_s [] arr; }" );
    sr_s arr = bcore_life_s_push_sr( l, bcore_inst_typed_create_sr( typeof( "chain_test_aware_arr" ) ) );
    arr = sr_cp( arr, TYPEOF_bcore_array_s );
    for( sz_t i = 0; i < 20000; i++ ) bcore_array_push( arr, sr_asd( st_s_createf( "line of text %zu", i ) ) );

    // write object to file
    {
        bcore_sink_chain_s* chain = bcore_sink_chain_s_create();
        bcore_sink_chain_s_push_d( chain, bcore_sink_file_s_create_name( "test.txt" ) );
        bcore_sink_chain_s_push_d( chain, bcore_inst_typed_create( typeof( "bcore_sink_buffer_s" ) ) );
        bcore_translate( bcore_inst_typed_create_sr( typeof( "bcore_txt_ml_translator_s" ) ), arr, sr_awd( chain ) );
        bcore_sink_chain_s_discard( chain );
    }

    // create object from file
    {
        bcore_source_chain_s* chain = bcore_life_s_push_aware( l, bcore_source_chain_s_create() );
        bcore_source_chain_s_push_d( chain, bcore_source_file_s_create_name( "test.txt" ) );
        bcore_source_chain_s_push_d( chain, bcore_inst_typed_create( typeof( "bcore_source_buffer_s" ) ) );
        sr_s chain_clone = sr_awd( bcore_life_s_push_aware( l, bcore_inst_aware_clone( chain ) ) );
        sr_s sr = bcore_interpret( bcore_inst_typed_create_sr( typeof( "bcore_txt_ml_interpreter_s" ) ), chain_clone );
        sr = bcore_life_s_push_sr( l, sr );
        if( bcore_compare_sr( sr, arr ) != 0 ) ERR( "%s", bcore_diff_sr( sr, arr )->sc );
    }

    bcore_life_s_discard( l );
    return msg;
}

/**********************************************************************************************************************/

vd_t bcore_sources_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bcore_sources" ) ) return NULL;

    if( signal == typeof( "init1" ) )
    {
        bcore_flect_define_creator( typeof( "bcore_source_string_s"   ), string_s_create_self );
        bcore_flect_define_creator( typeof( "bcore_source_buffer_s"   ), buffer_s_create_self );
        bcore_flect_define_creator( typeof( "bcore_source_file_s"     ), file_s_create_self   );
        bcore_flect_define_creator( typeof( "bcore_source_chain_s"    ), chain_s_create_self  );
    }
    else if( signal == typeof( "selftest" ) )
    {
        return sources_selftest();
    }

    return NULL;
}

