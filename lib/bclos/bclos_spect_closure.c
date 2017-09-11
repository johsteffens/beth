/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bclos_spect_closure.h"
#include "bclos_quicktypes.h"
#include "bcore_spect_inst.h"
#include "bcore_spect.h"

/**********************************************************************************************************************/
// bclos_closure_s

void bclos_closure_s_init( bclos_closure_s* o )
{
    bcore_memzero( o, sizeof( bclos_closure_s ) );
    o->p_type = TYPEOF_bclos_closure_s;
}

DEFINE_FUNCTION_DOWN_FLAT(  bclos_closure_s )
DEFINE_FUNCTION_CREATE(     bclos_closure_s )
DEFINE_FUNCTION_DISCARD(    bclos_closure_s )

/**********************************************************************************************************************/

static bl_t supports( const bcore_flect_self_s* self )
{
    if( !bcore_flect_self_s_try_external_fp( self, typeof( "bclos_closure_fp_call" ), 0 ) ) return false;
    return true;
}

static bclos_closure_s* create_from_self( const bcore_flect_self_s* self )
{
    assert( self != NULL );
    bclos_closure_s* o = bclos_closure_s_create();
    o->o_type = self->type;
    o->fp_def  = ( bclos_closure_fp_def  )bcore_flect_self_s_try_external_fp( self, bcore_name_enroll( "bclos_closure_fp_def"  ), 0 );
    o->fp_call = ( bclos_closure_fp_call )bcore_flect_self_s_get_external_fp( self, bcore_name_enroll( "bclos_closure_fp_call" ), 0 );
    o->fp_sig  = ( bclos_closure_fp_sig  )bcore_flect_self_s_try_external_fp( self, bcore_name_enroll( "bclos_closure_fp_sig"  ), 0 );
    return o;
}

static bcore_flect_self_s* closure_s_create_self( void )
{
    sc_t def = "bclos_closure_s = spect { aware_t p_type; tp_t o_type; ... }";
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bclos_closure_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bclos_closure_s_init,    "bcore_fp_init",                   "init"              );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bclos_closure_s_down,    "bcore_fp_down",                    "down"             );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bclos_closure_s_discard, "bcore_fp_discard",                 "discard"          );
    bcore_flect_self_s_push_external_func( self, ( fp_t )supports,                "bcore_spect_fp_supports",          "supports"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )create_from_self,        "bcore_spect_fp_create_from_self",  "create_from_self" );
    return self;
}

/**********************************************************************************************************************/

const bclos_closure_s* bclos_closure_s_get_typed( tp_t o_type )
{
    return bcore_spect_get_typed( TYPEOF_bclos_closure_s, o_type );
}

const bclos_closure_s* bclos_closure_s_get_aware( vc_t obj )
{
    return bclos_closure_s_get_typed( *( const aware_t* )obj );
}

/**********************************************************************************************************************/

void bclos_closure_spect_def(  const bclos_closure_s* p, vd_t o, bclos_env_s* env )
{
    if( p->fp_def ) p->fp_def( o, env );
}

sr_s bclos_closure_spect_call( const bclos_closure_s* p, vc_t o, bclos_env_s* env, const bclos_args_s* args )
{
    assert( p->fp_call );
    return p->fp_call( o, env, args );
}

sr_s bclos_closure_spect_sig(  const bclos_closure_s* p, vc_t o )
{
    return p->fp_sig ? p->fp_sig( o ) : sr_null();
}

sr_s bclos_closure_spect_call_nv( const bclos_closure_s* p, vc_t o, bclos_env_s* env, sz_t n, va_list v_args )
{
    bclos_args_s* args = bclos_args_s_create_nv( n, v_args );
    sr_s ret = bclos_closure_spect_call( p, o, env, args );
    bclos_args_s_discard( args );
    return ret;
}

sr_s bclos_closure_spect_call_na( const bclos_closure_s* p, vc_t o, bclos_env_s* env, sz_t n, ... )
{
    va_list args;
    va_start( args, n );
    sr_s ret = bclos_closure_spect_call_nv( p, o, env, n, args );
    va_end( args );
    return ret;
}

inline static const bclos_closure_s* w_qp( const sr_s* o ) { assert( !sr_s_is_const( o ) ); return ch_spect( o->p, TYPEOF_bclos_closure_s ); }
inline static const bclos_closure_s* r_qp( const sr_s* o ) {                                return ch_spect( o->p, TYPEOF_bclos_closure_s ); }

void bclos_closure_q_def( const sr_s* o, bclos_env_s* env )
{
    bclos_closure_spect_def( w_qp( o ), o->o, env );
}

sr_s bclos_closure_q_call( const sr_s* o, bclos_env_s* env, const bclos_args_s* args )
{
    return bclos_closure_spect_call( r_qp( o ), o->o, env, args );
}

sr_s bclos_closure_q_sig( const sr_s* o )
{
    return bclos_closure_spect_sig( r_qp( o ), o->o );
}

sr_s bclos_closure_q_call_nv( const sr_s* o, bclos_env_s* env, sz_t n, va_list args )
{
    return bclos_closure_spect_call_nv( r_qp( o ), o->o, env, n, args );
}

sr_s bclos_closure_q_call_na( const sr_s* o, bclos_env_s* env, sz_t n, ... )
{
    va_list args;
    va_start( args, n );
    sr_s ret = bclos_closure_spect_call_nv( r_qp( o ), o->o, env, n, args );
    va_end( args );
    return ret;
}

/**********************************************************************************************************************/

vd_t bclos_spect_closure_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bclos_spect_closure" ) ) return NULL;

    if( signal == typeof( "init1" ) )
    {
        bcore_flect_define_creator( typeof( "bclos_closure_s" ), closure_s_create_self );
    }

    return NULL;
}

/**********************************************************************************************************************/


