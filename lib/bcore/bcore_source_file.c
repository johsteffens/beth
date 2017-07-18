/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_source_file.h"
#include "bcore_spect_inst.h"
#include "bcore_quicktypes.h"

#include <stdio.h>

/**********************************************************************************************************************/

void init_a( vd_t nc )
{
    struct { ap_t a; vc_t p; bcore_source_file_s* o; } * nc_l = nc;
    nc_l->a( nc ); // default
    nc_l->o->_ = TYPEOF_bcore_source_file_s;
}

void bcore_source_file_s_init( bcore_source_file_s* o )
{
    bcore_inst_typed_init( TYPEOF_bcore_source_file_s, o );
}

void down_a( vd_t nc )
{
    struct { ap_t a; vc_t p; bcore_source_file_s* o; } * nc_l = nc;
    bcore_source_file_s_close( nc_l->o );
    nc_l->a( nc ); // default
}

void bcore_source_file_s_down( bcore_source_file_s* o )
{
    bcore_inst_typed_down( TYPEOF_bcore_source_file_s, o );
}

void copy_a( vd_t nc )
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
    o->name = bcore_string_s_create_sc( name );
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
        bcore_string_s* msg = bcore_string_s_createf( "Error opening file %s:\n", o->name->sc );
        perror( msg->sc );
        bcore_string_s_discard( msg );
        abort();
    }
}

void bcore_source_file_s_close( bcore_source_file_s* o )
{
    if( !o->handle ) return;
    if( !fclose( o->handle ) )
    {
        bcore_string_s* msg = bcore_string_s_createf( "Error closing file %s:\n", o->name->sc );
        perror( msg->sc );
        bcore_string_s_discard( msg );
        abort();
    }
    o->handle = NULL;
}

void bcore_source_file_s_flush( bcore_source_file_s* o )
{
    if( !o->handle ) return;
    if( !fflush( o->handle ) )
    {
        bcore_string_s* msg = bcore_string_s_createf( "Error flushing file %s:\n", o->name->sc );
        perror( msg->sc );
        bcore_string_s_discard( msg );
        abort();
    }
}

static void interpret_body_a( vd_t nc )
{
    struct { ap_t a; vc_t p; vc_t inter; vd_t source; tp_t type; vd_t obj; } * nc_l = nc;
    bcore_source_file_s_close( nc_l->obj );
    nc_l->a( nc ); // default
}

/**********************************************************************************************************************/

static sz_t flow_src( bcore_source_file_s* o, vd_t data, sz_t size )
{
    if( !o->handle ) bcore_source_file_s_open( o );
    sz_t rsize = fread( data, 1, size, o->handle );
    if( rsize != size )
    {
        if( ferror( o->handle ) )
        {
            bcore_string_s* msg = bcore_string_s_createf( "Error reading file %s:\n", o->name->sc );
            perror( msg->sc );
            bcore_string_s_discard( msg );
        }
    }
    return rsize;
}

static void p_errorvf( bcore_source_file_s* o, sc_t format, va_list args )
{
    ERR( "File name: %s\n%s\n", o->name->sc, bcore_string_s_createvf( format, args )->sc );
}

bcore_flect_self_s* bcore_source_file_s_create_self()
{
    sc_t def =
    "bcore_source_file_s = "
    "{ "
      "aware_t _; "
      "bcore_string_s* name; "
      "private vd_t handle; "
    "}";

    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bcore_source_file_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )init_a, "ap_t", "init" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )down_a, "ap_t", "down" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )copy_a, "ap_t", "copy" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )interpret_body_a, "ap_t", "interpret_body" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )flow_src,  "bcore_fp_flow_src", "flow_src"  );
    bcore_flect_self_s_push_external_func( self, ( fp_t )p_errorvf, "bcore_fp_logvf",    "p_errorvf" );
    return self;
}

/**********************************************************************************************************************/

