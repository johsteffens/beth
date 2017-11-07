/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#include "bclos_flow_control.h"
#include "bclos_spect_closure.h"
#include "bclos_quicktypes.h"
#include "bcore_trait.h"

/** Branch Closure
 *  Signature: inst branch( condition, true-obj, false-obj, ... )
 *  Extra arguments are passed to executing closure.
 *  Returns true or false object depending on condition
 */

static vd_t branch_create_static_sig()
{
    return bclos_signature_s_parse_from_sc( "bcore_inst_s branch( bl_t cond, bcore_inst_s true_obj, bcore_inst_s false_obj )" );
}

static sr_s branch_func( vc_t o, bclos_frame_s* frm, const bclos_arguments_s* args )
{
    ASSERT( args->size >= 3 );
    assert( sr_s_type( &args->data[ 0 ] ) == TYPEOF_bl_t );
    return ( *( bl_t* )args->data[ 0 ].o ) ? args->data[ 1 ] : args->data[ 2 ];
}

static bcore_flect_self_s* branch_create_self( void )
{
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( "bclos_branch = bclos_closure_s {}", 0 );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )branch_func, "bclos_closure_fp_call", "call" );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )branch_create_static_sig, "bclos_closure_fp_create_static_sig", "static_sig" );
    return self;
}

/**********************************************************************************************************************/

/** Loop Closure
 *  Signature: bcore_inst_s loop( bclos_closure_s closure )
 *  closure is executed until it returns an object
 *  which is also returned by the loop closure
 */

static vd_t loop_create_static_sig()
{
    return bclos_signature_s_parse_from_sc( "bcore_inst_s loop( bcore_closure_s obj )" );
}

static sr_s loop_func( vc_t o, bclos_frame_s* frm, const bclos_arguments_s* args )
{
    ASSERT( args->size >= 1 );
    sr_s* q_closure = &args->data[ 0 ];
    assert( bcore_trait_satisfied_type( TYPEOF_bclos_closure_s, sr_s_type( q_closure ), NULL ) );
    bclos_arguments_s trail_args = bclos_arguments_s_weak_crop( args, 1, -1 );
    sr_s ret = sr_null();
    while( !( ret = bclos_closure_q_call( q_closure, frm, &trail_args ) ).p ) {}
    return ret;
}

static bcore_flect_self_s* loop_create_self( void )
{
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( "bclos_loop = bclos_closure_s {}", 0 );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )loop_func, "bclos_closure_fp_call", "call" );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )loop_create_static_sig, "bclos_closure_fp_create_static_sig", "static_sig" );
    return self;
}

/**********************************************************************************************************************/

vd_t bclos_flow_control_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bclos_flow_control" ) ) return NULL;

    if( signal == typeof( "init1" ) )
    {
        bcore_flect_define_creator( typeof( "bclos_branch_s" ), branch_create_self );
        bcore_flect_define_creator( typeof( "bclos_loop_s"   ), loop_create_self );
    }

    return NULL;
}

