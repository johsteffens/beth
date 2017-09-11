/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_spect_closure.h"
#include "bcore_spect.h"
#include "bcore_quicktypes.h"

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

/**********************************************************************************************************************/

static bl_t supports( const bcore_flect_self_s* self )
{
    if( !bcore_flect_self_s_try_external_fp( self, typeof( "bcore_closure_fp_func"   ), 0 ) ) return false;
    if( !bcore_flect_self_s_try_external_fp( self, typeof( "bcore_closure_fp_n_args" ), 0 ) ) return false;
    if( !bcore_flect_self_s_try_external_fp( self, typeof( "bcore_closure_fp_d_arg"  ), 0 ) ) return false;
    if( !bcore_flect_self_s_try_external_fp( self, typeof( "bcore_closure_fp_t_ret"  ), 0 ) ) return false;
    return true;
}

static bcore_closure_s* create_from_self( const bcore_flect_self_s* self )
{
    assert( self != NULL );
    assert( bcore_flect_self_s_is_aware( self ) );
    bcore_closure_s* o = closure_s_create();
    o->o_type = self->type;
    o->fp_func    = ( bcore_closure_fp_func    )bcore_flect_self_s_get_external_fp( self, bcore_name_enroll( "bcore_closure_fp_func"    ), 0 );
    o->fp_n_args  = ( bcore_closure_fp_n_args  )bcore_flect_self_s_get_external_fp( self, bcore_name_enroll( "bcore_closure_fp_n_args"  ), 0 );
    o->fp_d_arg   = ( bcore_closure_fp_d_arg   )bcore_flect_self_s_get_external_fp( self, bcore_name_enroll( "bcore_closure_fp_d_arg"   ), 0 );
    o->fp_t_ret   = ( bcore_closure_fp_t_ret   )bcore_flect_self_s_get_external_fp( self, bcore_name_enroll( "bcore_closure_fp_t_ret"   ), 0 );
    o->fp_set_env = ( bcore_closure_fp_set_env )bcore_flect_self_s_try_external_fp( self, bcore_name_enroll( "bcore_closure_fp_set_env" ), 0 );
    return o;
}

static bcore_flect_self_s* closure_s_create_self( void )
{
    sc_t def = "bcore_closure_s = spect { aware_t p_type; tp_t o_type; ... }";
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bcore_closure_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )closure_s_init,             "bcore_fp_init",                    "init"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )closure_s_down,             "bcore_fp_down",                    "down"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )closure_s_create,           "bcore_fp_create",                  "create"       );
    bcore_flect_self_s_push_external_func( self, ( fp_t )closure_s_discard,          "bcore_fp_discard",                 "discard"      );
    bcore_flect_self_s_push_external_func( self, ( fp_t )supports,                   "bcore_spect_fp_supports",          "supports"     );
    bcore_flect_self_s_push_external_func( self, ( fp_t )create_from_self,           "bcore_spect_fp_create_from_self",  "create_from_self" );
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

vd_t bcore_closure_spect_func( const bcore_closure_s* p, vd_t o, const vd_t* args, sz_t n_args )
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

void bcore_closure_spect_set_env(  const bcore_closure_s* p, vd_t o, vd_t env )
{
    if( p->fp_set_env ) p->fp_set_env( o, env );
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

void bcore_closure_aware_set_env( vd_t o, vd_t env )
{
    const bcore_closure_s* p = bcore_closure_s_get_aware( o );
    if( p->fp_set_env ) p->fp_set_env( o, env );
}

inline static const bcore_closure_s* w_p( sr_s o ) { if( sr_s_is_const( &o ) ) ERR( "Attempt to modify a constant object" ); return ch_spect( o.p, TYPEOF_bcore_closure_s ); }
inline static const bcore_closure_s* r_p( sr_s o ) {                                                                         return ch_spect( o.p, TYPEOF_bcore_closure_s ); }

vd_t bcore_closure_func   ( sr_s o, const vd_t* a, sz_t n ){ vd_t r = w_p( o )->fp_func   ( o.o, a, n ); sr_down( o ); return r; }
sz_t bcore_closure_n_args ( sr_s o                        ){ sz_t r = r_p( o )->fp_n_args ( o.o );       sr_down( o ); return r; }
ad_s bcore_closure_d_arg  ( sr_s o, sz_t i                ){ ad_s r = r_p( o )->fp_d_arg  ( o.o, i );    sr_down( o ); return r; }
tp_t bcore_closure_t_ret  ( sr_s o                        ){ tp_t r = r_p( o )->fp_t_ret  ( o.o );       sr_down( o ); return r; }
void bcore_closure_set_env( sr_s o, vd_t e                ){          w_p( o )->fp_set_env( o.o, e );    sr_down( o );           }

inline static const bcore_closure_s* w_qp( const sr_s* o ) { assert( *(tp_t*)o == TYPEOF_bcore_closure_s ); assert( !sr_s_is_const( o ) ); return o->p; }
inline static const bcore_closure_s* r_qp( const sr_s* o ) { assert( *(tp_t*)o == TYPEOF_bcore_closure_s );                                 return o->p; }

vd_t bcore_closure_q_func   ( const sr_s* o, const vd_t* a, sz_t n ){ return w_qp( o )->fp_func   ( o->o, a, n ); }
sz_t bcore_closure_q_n_args ( const sr_s* o                        ){ return r_qp( o )->fp_n_args ( o->o       ); }
ad_s bcore_closure_q_d_arg  ( const sr_s* o, sz_t i                ){ return r_qp( o )->fp_d_arg  ( o->o, i    ); }
tp_t bcore_closure_q_t_ret  ( const sr_s* o                        ){ return r_qp( o )->fp_t_ret  ( o->o       ); }
void bcore_closure_q_set_env( const sr_s* o, vd_t e                ){        w_qp( o )->fp_set_env( o->o, e    ); }

/**********************************************************************************************************************/
// bcore_closure_r

void bcore_closure_r_init( bcore_closure_r* o )
{
    o->p = o->o = NULL;
}

void bcore_closure_r_down( bcore_closure_r* o )
{
    if( o->o ) bcore_inst_typed_discard( spect_tp_o( o->p ), o->o );
}

void bcore_closure_r_copy( bcore_closure_r* o, const bcore_closure_r* src )
{
    bcore_closure_r_down( o );
    o->p = src->p;
    o->o = src->o ? bcore_inst_typed_clone( spect_tp_o( src->p ), src->o ) : NULL;
}

void bcore_closure_r_set( bcore_closure_r* o, sr_s src )
{
    bcore_closure_r_down( o );
    o->p = ch_spect( src.p, TYPEOF_bcore_closure_s );
    if( sr_s_is_strong( &src ) )
    {
        o->o = src.o;
        src = sr_cw( src );
    }
    else
    {
        o->o = bcore_inst_clone( src );
    }
    sr_down( src );
}

sr_s bcore_closure_r_get( bcore_closure_r* o )
{
    return sr_pwd( o->p, o->o );
}

vd_t bcore_closure_r_func(    const bcore_closure_r* o, const vd_t* a, sz_t n ) { assert( o->p ); return o->p->fp_func  ( o->o, a, n );               }
sz_t bcore_closure_r_n_args(  const bcore_closure_r* o                        ) { assert( o->p ); return o->p->fp_n_args( o->o );                     }
ad_s bcore_closure_r_d_arg(   const bcore_closure_r* o, sz_t index            ) { assert( o->p ); return o->p->fp_d_arg ( o->o, index );              }
tp_t bcore_closure_r_t_ret (  const bcore_closure_r* o                        ) { assert( o->p ); return o->p->fp_t_ret ( o->o );                     }
void bcore_closure_r_set_env( const bcore_closure_r* o, vd_t e                ) { assert( o->p ); if( o->p->fp_set_env ) o->p->fp_set_env( o->o, e ); }

static bcore_flect_self_s* closure_r_create_self( void )
{
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( "bcore_closure_r = { private vd_t o; private bcore_closure_r* p; shell typed * obj; }", sizeof( bcore_closure_r ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_closure_r_init, "bcore_fp_init", "init" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_closure_r_down, "bcore_fp_down", "down" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_closure_r_copy, "bcore_fp_copy", "copy" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_closure_r_set,  "bcore_fp_set",  "set_obj" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_closure_r_get,  "bcore_fp_get",  "get_obj" );
    return self;
}

/**********************************************************************************************************************/

vd_t bcore_spect_closure_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bcore_spect_closure" ) ) return NULL;

    if( signal == typeof( "init1" ) )
    {
        bcore_flect_define_creator( typeof( "bcore_closure_s"  ), closure_s_create_self  );
        bcore_flect_define_creator( typeof( "bcore_closure_r" ), closure_r_create_self );
    }

    return NULL;
}

/**********************************************************************************************************************/
