/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_ref.h"
#include "bcore_life.h"
#include "bcore_spect.h"
#include "bcore_spect_inst.h"

/**********************************************************************************************************************/
// embedded usage

sr_s sr_spect( sr_s o, tp_t spect_type )
{
    o.p = ch_spect( o.p, spect_type );
    return o;
}

sr_s sr_cl( sr_s o, bcore_life_s* l )
{
    return bcore_life_s_push_sr( l, o );
}

sr_s sr_create_strong_typed( tp_t type, vc_t obj )
{
    return bcore_inst_typed_clone_sr( type, obj );
}

/**********************************************************************************************************************/
// object usage

void sr_s_init( sr_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
}

void sr_s_down( sr_s* o )
{
    if( o ) sr_down( *o );
}

void sr_s_copy( sr_s* o, const sr_s* src )
{
    sr_s_clear( o );
    if( src->o )
    {
        *o = sr_psd( src->p, bcore_inst_typed_clone( spect_tp_o( src->p ), src->o ) );
    }
}

sr_s* sr_s_create()
{
    sr_s* o = bcore_u_alloc( sizeof( sr_s ), NULL, 1, NULL );
    sr_s_init( o );
    return o;
}

sr_s* sr_s_clone( const sr_s* src )
{
    if( !src ) return NULL;
    sr_s* o = sr_s_create();
    sr_s_copy( o, src );
    return o;
}

void sr_s_discard( sr_s* o )
{
    if( !o ) return;
    sr_s_down( o );
}

static sr_s sr_s_get_obj( sr_s* o )
{
    return sr_s_get( o );
}

static void sr_s_set_obj( sr_s* o, sr_s obj )
{
    sr_s_set( o, obj );
}

static bcore_flect_self_s* sr_s_create_self( void )
{
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( " sr_s = { private vd_t o; private vc_t p; private tp_t f; shell typed * obj; }", sizeof( sr_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )sr_s_init,    "bcore_fp_init", "init"    );
    bcore_flect_self_s_push_external_func( self, ( fp_t )sr_s_down,    "bcore_fp_down", "down"    );
    bcore_flect_self_s_push_external_func( self, ( fp_t )sr_s_copy,    "bcore_fp_copy", "copy"    );
    bcore_flect_self_s_push_external_func( self, ( fp_t )sr_s_get_obj, "bcore_fp_get",  "get_obj" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )sr_s_set_obj, "bcore_fp_set",  "set_obj" );
    return self;
}

/**********************************************************************************************************************/

void bcore_ref_define_self_creators( void )
{
    bcore_flect_define_creator( typeof( "sr_s"  ), sr_s_create_self  );
}

/**********************************************************************************************************************/

