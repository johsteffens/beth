/** Copyright 2017 Johannes Bernhard Steffens
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "bclos_closures.h"
#include "bclos_spect_closure.h"
#include "bclos_quicktypes.h"
#include "bcore_trait.h"
#include "bcore_txt_ml.h"
#include "bcore_signal.h"

/**********************************************************************************************************************/
// assign_s

/** Makes a (right-)assignment [0] -> [1] ensuring that the destination is not (strongly) referenced
 *  elsewhere (outside the argument list).
 *  The function analyzes the reference state of source. A deep copy is only induced if the above
 *  cannot be guaranteed by passing the reference.
 *  The target will be a strong reference.
 *  Returns a weak reference to the target .
 */
static sr_s assign_s_func( vc_t o, bclos_frame_s* frm, const bclos_arguments_s* args )
{
    ASSERT( args->size >= 2 );
    sr_s* dst = &args->data[ 1 ];
    if( dst && sr_s_type( dst ) == TYPEOF_bclos_address_s )
    {
        sr_s src = bclos_arguments_s_get( args, 0, frm );
        tp_t dst_name = ( ( const bclos_address_s* )dst->o )->name;
        sr_s* dst_sr  = bclos_frame_s_get( frm, dst_name );
        if( !dst_sr ) ERR( "Target '%s' was not defined.", ifnameof( dst_name ) );
        if( sr_s_is_strong( &src ) && sr_s_references( &src ) <= 1 )
        {
            sr_down( *dst_sr );
            *dst_sr = sr_s_fork( &src );
        }
        else
        {
            sr_s_copy( dst_sr, &src );
        }
        sr_down( src );
        return sr_cw( *dst );
    }
    else
    {
        ERR( "Target must be an address." );
    }
    return sr_null();
}

static vd_t assign_s_create_static_sig()
{
    return bclos_signature_s_parse_from_sc( "bcore_inst_s bclos_assign_s( bcore_inst_s src, bclos_address_s dst )" );
}

static bcore_self_s* assign_s_create_self( void )
{
    bcore_self_s* self = bcore_self_s_build_parse_sc( "bclos_assign_s = bclos_language_closure {}", 0 );
    bcore_self_s_push_ns_func( self, ( fp_t )assign_s_func, "bclos_closure_fp_call", "call" );
    bcore_self_s_push_ns_func( self, ( fp_t )assign_s_create_static_sig, "bclos_closure_fp_create_static_sig", "static_sig" );
    return self;
}

/**********************************************************************************************************************/

/// sends object to stdout with ending newline
static sr_s writeln_s_func( vc_t o, bclos_frame_s* frm, const bclos_arguments_s* args )
{
    ASSERT( args->size >= 1 );
    sr_s sr = bclos_arguments_s_get( args, 0, frm );
    if( sr.o )
    {
        switch( sr_s_type( &sr ) )
        {
            case TYPEOF_st_s:
            case TYPEOF_sc_t:
            case TYPEOF_sd_t:
            case TYPEOF_s0_t:
            case TYPEOF_s1_t:
            case TYPEOF_s2_t:
            case TYPEOF_s3_t:
            case TYPEOF_u0_t:
            case TYPEOF_u1_t:
            case TYPEOF_u2_t:
            case TYPEOF_u3_t:
            case TYPEOF_f2_t:
            case TYPEOF_f3_t:
            case TYPEOF_sz_t:
            case TYPEOF_uz_t:
            case TYPEOF_bl_t:
            case TYPEOF_tp_t:
            case TYPEOF_aware_t:
            {
                st_s* s = st_s_create_typed( sr_s_type( &sr ), sr.o );
                st_s_print_d( st_s_create_fa( "#<sc_t>\n", s->sc ) );
                st_s_discard( s );
            }
            break;

            default:
            {
                bcore_txt_ml_x_to_stdout( sr_cw( sr ) );
                st_s_print_d( st_s_create_fa( "\n" ) );
            }
            break;
        }
    }
    else
    {
        st_s_print_d( st_s_create_fa( "NULL\n" ) );
    }
    sr_down( sr );
    return sr_null();
}

static vd_t writeln_s_create_static_sig()
{
    return bclos_signature_s_parse_from_sc( "void bclos_writeln_s( bcore_inst_s obj )" );
}

static bcore_self_s* writeln_s_create_self( void )
{
    bcore_self_s* self = bcore_self_s_build_parse_sc( "bclos_writeln_s = bclos_language_closure {}", 0 );
    bcore_self_s_push_ns_func( self, ( fp_t )writeln_s_func, "bclos_closure_fp_call", "call" );
    bcore_self_s_push_ns_func( self, ( fp_t )writeln_s_create_static_sig, "bclos_closure_fp_create_static_sig", "static_sig" );
    return self;
}

/**********************************************************************************************************************/

/// Identity closure (takes one argument which is deemed to be the return value)
static sr_s identity_func( vc_t o, bclos_frame_s* frm, const bclos_arguments_s* args )
{
    ASSERT( args->size >= 1 );
    return bclos_arguments_s_get( args, 0, frm );
}

static bcore_self_s* identity_s_create_self( void )
{
    bcore_self_s* self = bcore_self_s_build_parse_sc( "bclos_identity_s = bclos_closure {}", 0 );
    bcore_self_s_push_ns_func( self, ( fp_t )identity_func, "bclos_closure_fp_call", "call" );
    return self;
}

/**********************************************************************************************************************/

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
    sr_s cond = bclos_arguments_s_get( args, 0, frm );
    assert( sr_s_type( &cond ) == TYPEOF_bl_t );
    bl_t flag = *( bl_t* )cond.o;
    sr_down( cond );
    return ( flag ) ? bclos_arguments_s_get( args, 1, frm ) : bclos_arguments_s_get( args, 2, frm );
}

static bcore_self_s* branch_create_self( void )
{
    bcore_self_s* self = bcore_self_s_build_parse_sc( "bclos_branch = bclos_closure {}", 0 );
    bcore_self_s_push_ns_func( self, ( fp_t )branch_func, "bclos_closure_fp_call", "call" );
    bcore_self_s_push_ns_func( self, ( fp_t )branch_create_static_sig, "bclos_closure_fp_create_static_sig", "static_sig" );
    return self;
}

/**********************************************************************************************************************/

/** Loop Closure
 *  Signature: bcore_inst_s loop( bclos_closure_s closure )
 *  closure is executed until it returns any object.
 *  That object is also returned by the loop closure
 */

static vd_t loop_create_static_sig()
{
    return bclos_signature_s_parse_from_sc( "bcore_inst_s loop( bcore_closure_s obj )" );
}

static sr_s loop_func( vc_t o, bclos_frame_s* frm, const bclos_arguments_s* args )
{
    sr_s closure = bclos_arguments_s_get( args, 0, frm );
    assert( bcore_trait_satisfied_type( TYPEOF_bclos_closure_s, sr_s_type( &closure ), NULL ) );
    bclos_arguments_s trail_args = bclos_arguments_s_weak_crop( args, 1, -1 );
    sr_s ret = sr_null();
    while( !( ret = bclos_closure_r_call( &closure, frm, &trail_args ) ).p ) {}
    sr_down( closure );
    return ret;
}

static bcore_self_s* loop_create_self( void )
{
    bcore_self_s* self = bcore_self_s_build_parse_sc( "bclos_loop = bclos_closure {}", 0 );
    bcore_self_s_push_ns_func( self, ( fp_t )loop_func, "bclos_closure_fp_call", "call" );
    bcore_self_s_push_ns_func( self, ( fp_t )loop_create_static_sig, "bclos_closure_fp_create_static_sig", "static_sig" );
    return self;
}

/**********************************************************************************************************************/

vd_t bclos_closures_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bclos_closures" ) ) )
    {
        case TYPEOF_init1:
        {
            bcore_flect_define_creator( typeof( "bclos_assign_s"  ),  assign_s_create_self );
            bcore_flect_define_creator( typeof( "bclos_writeln_s"  ), writeln_s_create_self );
            bcore_flect_define_creator( typeof( "bclos_identity_s" ), identity_s_create_self );
            bcore_flect_define_creator( typeof( "bclos_branch_s"   ), branch_create_self );
            bcore_flect_define_creator( typeof( "bclos_loop_s"     ), loop_create_self );
        }
        break;

        case TYPEOF_selftest:
        {
        }
        break;

        default: break;
    }

    return NULL;
}

