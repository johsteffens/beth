/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_source_string.h"
#include "bcore_spect_inst.h"
#include "bcore_quicktypes.h"

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

bcore_source_string_s* bcore_source_string_s_create_string( const bcore_string_s* string )
{
    bcore_source_string_s* o = bcore_source_string_s_create();
    o->string = bcore_string_s_clone( string );
    return o;
}

bcore_source_string_s* bcore_source_string_s_create_string_d( bcore_string_s* string )
{
    bcore_source_string_s* o = bcore_source_string_s_create();
    o->string = string;
    return o;
}

bcore_source_string_s* bcore_source_string_s_create_sc( sc_t sc )
{
    bcore_source_string_s* o = bcore_source_string_s_create();
    o->string = bcore_string_s_create_sc( sc );
    return o;
}

/**********************************************************************************************************************/

static void refill( bcore_source_string_s* o, sz_t size )
{
    if( o->string->size - o->index <= size )
    {
        if( o->index > 0 )
        {
            o->preceding_lines += bcore_string_s_count_char( o->string, 0, o->index, '\n' );
            bcore_memmove( o->string->data, o->string->data + o->index, o->string->size - o->index );
            o->string->size -= o->index;
            o->string->data[ o->string->size ] = 0;
        }
        sz_t refill_size = size - o->string->size > o->prefetch_size ? size - o->string->size : o->prefetch_size;
        if( o->string->space <= o->string->size + refill_size + 1 )
        {
            bcore_string_s_push_char_n( o->string, 0, o->string->size + refill_size + 1 - o->string->space );
        }
        sz_t bytes_received = bcore_source_get_data( o->ext_supplier, o->string->data + o->string->size, refill_size );
        if( bytes_received < refill_size ) o->ext_supplier = NULL; // detach supplier when empty
        o->string->size += bytes_received;
        o->string->data[ o->string->size ] = 0;
    }
}

static sz_t flow_src( bcore_source_string_s* o, vd_t data, sz_t size )
{
    if( size > o->string->size - o->index )
    {
        // first empty buffer
        sz_t size1 = o->string->size - o->index;
        bcore_memcpy( data, o->string->data + o->index, size1 );
        o->index += size1;
        data = (u0_t*)data + size1;
        size -= size1;
        if( o->ext_supplier ) refill( o, size ); // refill
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

static void p_errorvf( bcore_source_string_s* o, sc_t format, va_list args )
{
    bcore_string_s* context = bcore_string_s_show_line_context( o->string, o->index );
    bcore_string_s* msg     = bcore_string_s_createvf( format, args );
    sz_t line = bcore_string_s_lineof( o->string, o->index );
    sz_t col  = bcore_string_s_colof( o->string, o->index );
    bcore_err( "bcore_source_string_s parse error at line %zu, col %zu:\n%s%s\n", line + o->preceding_lines, col, context->sc, msg->sc );
    bcore_string_s_discard( msg );
    bcore_string_s_discard( context );
}

static void parsevf( bcore_source_string_s* o, sc_t format, va_list args )
{
    if( o->ext_supplier ) refill( o, o->refill_limit );
    if( !o->string ) ERR( "No string defined." );
    o->index = bcore_string_s_parsevf( o->string, o->index, o->string->size, format, args );
}

bcore_flect_self_s* bcore_source_string_s_create_self()
{
    sc_t def =
    "bcore_source_string_s = "
    "{ "
      "aware_t _; "
      "bcore_string_s* string; "
      "sz_t index; "
      "vd_t supplier; "
      "sz_t preceding_lines; "
      "sz_t refill_limit;    "
      "sz_t prefetch_size;   "
    "}";
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bcore_source_string_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_source_string_s_init,  "bcore_fp_init", "init" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )flow_src,  "bcore_fp_flow_src",       "flow_src"  );
    bcore_flect_self_s_push_external_func( self, ( fp_t )p_errorvf, "bcore_fp_logvf",          "p_errorvf" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )parsevf,   "bcore_source_fp_parsevf", "parsevf"   );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_source_string_s_set_supplier,   "bcore_source_fp_set_supplier", "set_supplier"   );
    return self;
}

void bcore_source_string_s_set_supplier( bcore_source_string_s* o, vd_t supplier )
{
    o->ext_supplier = supplier;
}

/**********************************************************************************************************************/

