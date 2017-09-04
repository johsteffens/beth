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

static bclos_closure_s* create_from_self( const bcore_flect_self_s* self )
{
    assert( self != NULL );
    assert( bcore_flect_self_s_is_aware( self ) );
    bclos_closure_s* o = bclos_closure_s_create();
    o->o_type = self->type;
    o->fp_def  = ( bclos_closure_fp_def  )bcore_flect_self_s_try_external_fp( self, bcore_name_enroll( "bclos_closure_fp_def"  ), 0 );
    o->fp_call = ( bclos_closure_fp_call )bcore_flect_self_s_get_external_fp( self, bcore_name_enroll( "bclos_closure_fp_call" ), 0 );
    o->fp_sig  = ( bclos_closure_fp_sig  )bcore_flect_self_s_get_external_fp( self, bcore_name_enroll( "bclos_closure_fp_sig"  ), 0 );
    return o;
}

static bcore_flect_self_s* closure_s_create_self( void )
{
    bcore_flect_self_s* self = bcore_flect_self_s_create_plain( bcore_name_enroll( "bclos_closure_s" ), sizeof( bclos_closure_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bclos_closure_s_init, "bcore_fp_init",                    "init"             );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bclos_closure_s_down, "bcore_fp_down",                    "down"             );
    bcore_flect_self_s_push_external_func( self, ( fp_t )create_from_self,     "bcore_spect_fp_create_from_self",  "create_from_self" );
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
    assert( p->fp_sig );
    return p->fp_sig( o );
}

/**********************************************************************************************************************/

void bclos_spect_closure_define_self_creators( void )
{
    bcore_flect_define_creator( typeof( "bclos_spect_closure_s" ), closure_s_create_self );
}

/**********************************************************************************************************************/


