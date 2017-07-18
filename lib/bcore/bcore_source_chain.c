/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_source_chain.h"
#include "bcore_spect_inst.h"
#include "bcore_quicktypes.h"
#include "bcore_spect_array.h"

#include <stdio.h>

/**********************************************************************************************************************/

void init_a( vd_t nc )
{
    struct { ap_t a; vc_t p; bcore_source_chain_s* o; } * nc_l = nc;
    nc_l->a( nc ); // default
    nc_l->o->_ = TYPEOF_bcore_source_chain_s;
}

void bcore_source_chain_s_init( bcore_source_chain_s* o )
{
    bcore_inst_typed_init( TYPEOF_bcore_source_chain_s, o );
}

void down_a( vd_t nc )
{
    struct { ap_t a; vc_t p; bcore_source_chain_s* o; } * nc_l = nc;
    nc_l->a( nc ); // default
}

void bcore_source_chain_s_down( bcore_source_chain_s* o )
{
    bcore_inst_typed_down( TYPEOF_bcore_source_chain_s, o );
}

void copy_a( vd_t nc )
{
    struct { ap_t a; vc_t p; bcore_source_chain_s* dst; } * nc_l = nc;
    nc_l->a( nc ); // default
    bcore_source_chain_s* o = nc_l->dst;
    for( sz_t i = 1; i < o->size; i++ ) bcore_source_set_supplier( o->data[ i ], o->data[ i - 1 ] );
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

static void interpret_body_a( vd_t nc )
{
    struct { ap_t a; vc_t p; vc_t inter; vd_t source; tp_t type; vd_t obj; } * nc_l = nc;
    nc_l->a( nc ); // default
    bcore_source_chain_s* o = nc_l->obj;
    for( sz_t i = 1; i < o->size; i++ ) bcore_source_set_supplier( o->data[ i ], o->data[ i - 1 ] );
}

void bcore_source_chain_s_push_d( bcore_source_chain_s* o, vd_t source )
{
    const bcore_array_s* arr_p = bcore_array_s_get_typed( TYPEOF_bcore_source_chain_s );
    arr_p->push_d( arr_p, o, source );
    if( o->size > 1 ) bcore_source_set_supplier( o->data[ o->size - 1 ], o->data[ o->size - 2 ] );
}

void bcore_source_chain_s_push_type( bcore_source_chain_s* o, tp_t type )
{
    bcore_source_chain_s_push_d( o, bcore_inst_typed_create( type ) );
}

/**********************************************************************************************************************/

static sz_t flow_src( bcore_source_chain_s* o, vd_t data, sz_t size )
{
    return ( o->size > 0 ) ? bcore_source_get_data( o->data[ o->size - 1 ], data, size ) : 0;
}

static void p_errorvf( bcore_source_chain_s* o, sc_t format, va_list args )
{
    if( o->size > 0 )
    {
        bcore_source_parse_errvf( o->data[ o->size - 1 ], format, args );
    }
    else
    {
        ERR( "bcore_source_chain_s:\n%s\n", bcore_string_s_createvf( format, args )->sc );
    }
}

static void parsevf( bcore_source_chain_s* o, sc_t format, va_list args )
{
    if( o->size > 0 )
    {
        const bcore_source_s* source_p = bcore_source_s_get_aware( o->data[ o->size - 1 ] );
        if( source_p->fp_parsevf )
        {
            source_p->parsevf( source_p, o->data[ o->size - 1 ], format, args );
        }
        else
        {
            bcore_source_chain_s_push_type( o, TYPEOF_bcore_source_string_s );
            parsevf( o, format, args );
        }
    }
    else
    {
        ERR( "Chain is empty" );
    }
}

bcore_flect_self_s* bcore_source_chain_s_create_self()
{
    sc_t def =
    "bcore_source_chain_s = "
    "{ "
      "aware_t _; "
      "bcore_string_s* name; "
      "private vd_t handle; "
    "}";

    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bcore_source_chain_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )init_a, "ap_t", "init" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )down_a, "ap_t", "down" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )copy_a, "ap_t", "copy" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )interpret_body_a, "ap_t", "interpret_body" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )flow_src,  "bcore_fp_flow_src", "flow_src"  );
    bcore_flect_self_s_push_external_func( self, ( fp_t )p_errorvf, "bcore_fp_logvf",    "p_errorvf" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )parsevf,   "bcore_source_fp_parsevf", "parsevf" );
    return self;
}

/**********************************************************************************************************************/

