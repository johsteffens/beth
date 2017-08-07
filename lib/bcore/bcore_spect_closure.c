/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_spect_closure.h"
#include "bcore_spect.h"

/**********************************************************************************************************************/
// bcore_closure_s

static void closure_s_down( bcore_closure_s* o );

static void closure_s_init( bcore_closure_s* o )
{
    bcore_memzero( o, sizeof( bcore_closure_s ) );
    o->p_type = typeof( "bcore_closure_s" );
}

static void closure_s_down( bcore_closure_s* o )
{
}

static bcore_closure_s* closure_s_create()
{
    bcore_closure_s* o = bcore_alloc( NULL, sizeof( bcore_closure_s ) );
    closure_s_init( o );
    return o;
}

static void closure_s_discard( bcore_closure_s* o )
{
    if( !o ) return;
    closure_s_down( o );
    bcore_free( o );
}

static bcore_signature_s* closure_s_create_signature( bcore_closure_s* o )
{
    return bcore_signature_s_create_an( 2, o->p_type, o->o_type );
}

/**********************************************************************************************************************/

static bcore_closure_s* create_from_self( const bcore_flect_self_s** p_self )
{
    assert( p_self != NULL );
    const bcore_flect_self_s* self = *p_self;
    assert( self != NULL );
    assert( bcore_flect_self_s_is_aware( self ) );
    bcore_closure_s* o = closure_s_create();
    o->o_type = self->type;
    o->fp_func   = ( bcore_closure_fp_func   )bcore_flect_self_s_get_external_fp( self, bcore_name_enroll( "bcore_closure_fp_func"   ), 0 );
    o->fp_n_args = ( bcore_closure_fp_n_args )bcore_flect_self_s_get_external_fp( self, bcore_name_enroll( "bcore_closure_fp_n_args" ), 0 );
    o->fp_d_arg  = ( bcore_closure_fp_d_arg  )bcore_flect_self_s_get_external_fp( self, bcore_name_enroll( "bcore_closure_fp_d_arg"  ), 0 );
    o->fp_t_ret  = ( bcore_closure_fp_t_ret  )bcore_flect_self_s_get_external_fp( self, bcore_name_enroll( "bcore_closure_fp_t_ret"  ), 0 );
    return o;
}

bcore_flect_self_s* bcore_closure_s_create_self( void )
{
    bcore_flect_self_s* self = bcore_flect_self_s_create_plain( bcore_name_enroll( "bcore_closure_s" ), sizeof( bcore_closure_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )closure_s_init,             "bcore_fp_init",                    "init"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )closure_s_down,             "bcore_fp_down",                    "down"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )closure_s_create,           "bcore_fp_create",                  "create"       );
    bcore_flect_self_s_push_external_func( self, ( fp_t )closure_s_discard,          "bcore_fp_discard",                 "discard"      );
    bcore_flect_self_s_push_external_func( self, ( fp_t )closure_s_create_signature, "bcore_spect_fp_create_signature",  "create_signature" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )create_from_self,          "bcore_spect_fp_create_from_self",  "create_from_self" );
    return self;
}

/**********************************************************************************************************************/

const bcore_closure_s* bcore_closure_s_get_typed( tp_t o_type )
{
    return bcore_spect_get_typed( typeof( "bcore_closure_s" ), o_type );
}

const bcore_closure_s* bcore_closure_s_get_aware( vc_t obj )
{
    return bcore_closure_s_get_typed( *( const aware_t* )obj );
}

/**********************************************************************************************************************/

vd_t bcore_closure_spect_func( const bcore_closure_s* p, vc_t o, const vd_t* args, sz_t n_args )
{
    return p->fp_func( o, args, n_args );
}

sz_t bcore_closure_spect_n_args( const bcore_closure_s* p, vc_t o )
{
    return p->fp_n_args( o );
}

ad_s bcore_closure_spect_d_arg(  const bcore_closure_s* p, vc_t o, sz_t index )
{
    return p->fp_d_arg( o, index );
}

tp_t bcore_closure_spect_t_ret(  const bcore_closure_s* p, vc_t o )
{
    return p->fp_t_ret( o );
}

vd_t bcore_closure_aware_func( vd_t o, const vd_t* args, sz_t n_args )
{
    const bcore_closure_s* p = bcore_closure_s_get_aware( o );
    return p->fp_func( o, args, n_args );
}

sz_t bcore_closure_aware_n_args( vc_t o )
{
    const bcore_closure_s* p = bcore_closure_s_get_aware( o );
    return p->fp_n_args( o );
}

ad_s bcore_closure_aware_d_arg( vc_t o, sz_t index )
{
    const bcore_closure_s* p = bcore_closure_s_get_aware( o );
    return p->fp_d_arg( o, index );
}

tp_t bcore_closure_aware_t_ret(  vc_t o )
{
    const bcore_closure_s* p = bcore_closure_s_get_aware( o );
    return p->fp_t_ret( o );
}

/**********************************************************************************************************************/

