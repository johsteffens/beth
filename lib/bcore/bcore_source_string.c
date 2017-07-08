/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_source_string.h"
#include "bcore_spect_inst.h"
#include "bcore_quicktypes.h"

/**********************************************************************************************************************/

void bcore_source_string_s_init(    bcore_source_string_s* o ) { bcore_inst_typed_init(    TYPEOF_bcore_source_string_s, o ); }
void bcore_source_string_s_down(    bcore_source_string_s* o ) { bcore_inst_typed_down(    TYPEOF_bcore_source_string_s, o ); }
void bcore_source_string_s_discard( bcore_source_string_s* o ) { bcore_inst_typed_discard( TYPEOF_bcore_source_string_s, o ); }

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

void bcore_source_string_s_rewind( bcore_source_string_s* o )
{
    o->index = 0;
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

static sz_t flow_src( bcore_source_string_s* o, vd_t data, sz_t size )
{
    if( !o->string ) return 0;
    sz_t size_l = size < o->string->size - o->index ? size : o->string->size - o->index;
    bcore_memcpy( data, o->string->data, size_l );
    return size_l;
}

static void p_errorvf( bcore_source_string_s* o, sc_t format, va_list args )
{
    bcore_string_s* context = bcore_string_s_show_line_context( o->string, o->index );
    bcore_string_s* msg     = bcore_string_s_createvf( format, args );
    bcore_err( "bcore_source_string_s parse error:\n%s\n%s\n", context->sc, msg->sc );
    bcore_string_s_discard( msg );
    bcore_string_s_discard( context );
}

static void parsevf( bcore_source_string_s* o, sc_t format, va_list args )
{
    if( !o->string ) ERR( "No string defined." );
    o->index = bcore_string_s_parsevf( o->string, o->index, o->string->size, format, args );
}

bcore_flect_self_s* bcore_source_string_s_create_self()
{
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( " bcore_source_string_s =  { aware_t _; bcore_string_s* string; sz_t index; }", sizeof( bcore_source_string_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )flow_src,  "bcore_fp_flow_src",       "flow_src"  );
    bcore_flect_self_s_push_external_func( self, ( fp_t )p_errorvf, "bcore_fp_logvf",          "p_errorvf" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )parsevf,   "bcore_source_fp_parsevf", "parsevf"   );
    return self;
}

/**********************************************************************************************************************/

