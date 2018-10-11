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

#include "bclos_spect_closure.h"
#include "bclos_quicktypes.h"
#include "bcore_spect_inst.h"
#include "bcore_spect.h"
#include "bcore_trait.h"
#include "bcore_signal.h"

/**********************************************************************************************************************/
// bclos_closure_s

void bclos_closure_s_init( bclos_closure_s* o )
{
    bcore_memzero( o, sizeof( bclos_closure_s ) );
    o->header.p_type = TYPEOF_bclos_closure_s;
}

void bclos_closure_s_down( bclos_closure_s* o )
{
    bclos_signature_s_discard( o->static_sig );
}

BCORE_DEFINE_FUNCTION_CREATE(  bclos_closure_s )
BCORE_DEFINE_FUNCTION_DISCARD( bclos_closure_s )

/**********************************************************************************************************************/

BCORE_DEFINE_SPECT( bcore_inst, bclos_closure )
"{\
    bcore_spect_header_s  header;\
    bclos_signature_s => static_sig;\
           feature bclos_closure_fp_def  def  ~> func bclos_closure_fp_def  def;\
    strict feature bclos_closure_fp_call call ~> func bclos_closure_fp_call call;\
           feature bclos_closure_fp_sig  sig  ~> func bclos_closure_fp_sig  sig;\
\
    func bcore_spect_fp:create_from_self;\
}";

static bclos_closure_s* bclos_closure_s_create_from_self( const bcore_self_s* self )
{
    bclos_closure_s* o = bcore_spect_create_from_self( bcore_flect_get_self( TYPEOF_bclos_closure_s ), self );
    o->static_sig = bcore_self_s_try_static( self, bcore_name_enroll( "bclos_closure_fp_create_static_sig"  ), 0 );
    return o;
}

/**********************************************************************************************************************/

void bclos_closure_default_def(  const bclos_closure_s* p, bclos_closure* o, bclos_frame_s* frm )
{
    // nothing to do
}

sr_s bclos_closure_default_sig(  const bclos_closure_s* p, const bclos_closure* o )
{
    return ( p->static_sig ? sr_twc( TYPEOF_bclos_signature_s, p->static_sig ) : sr_null() );
}

sr_s bclos_closure_p_call_nv( const bclos_closure_s* p, const bclos_closure* o, bclos_frame_s* frm, uz_t n, va_list v_args )
{
    bclos_arguments_s* args = bclos_arguments_s_create_nv( n, v_args );
    sr_s ret = bclos_closure_p_call( p, o, frm, args );
    bclos_arguments_s_discard( args );
    return ret;
}

sr_s bclos_closure_p_call_na( const bclos_closure_s* p, const bclos_closure* o, bclos_frame_s* frm, uz_t n, ... )
{
    va_list args;
    va_start( args, n );
    sr_s ret = bclos_closure_p_call_nv( p, o, frm, n, args );
    va_end( args );
    return ret;
}

inline static const bclos_closure_s* r_qp( const sr_s* o ) { return ch_spect_p( o->p, TYPEOF_bclos_closure_s ); }

sr_s bclos_closure_r_call_nv( const sr_s* o, bclos_frame_s* frm, uz_t n, va_list args )
{
    return bclos_closure_p_call_nv( r_qp( o ), o->o, frm, n, args );
}

sr_s bclos_closure_r_call_na( const sr_s* o, bclos_frame_s* frm, uz_t n, ... )
{
    va_list args;
    va_start( args, n );
    sr_s ret = bclos_closure_p_call_nv( r_qp( o ), o->o, frm, n, args );
    va_end( args );
    return ret;
}

/**********************************************************************************************************************/

vd_t bclos_spect_closure_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bclos_spect_closure" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_FEATURE( bclos_closure_fp_def );
            BCORE_REGISTER_FEATURE( bclos_closure_fp_call );
            BCORE_REGISTER_FEATURE( bclos_closure_fp_sig );
            BCORE_REGISTER_FEATURE( bclos_closure_fp_create_static_sig );

            BCORE_REGISTER_FFUNC( bcore_spect_fp_create_from_self, bclos_closure_s_create_from_self );
            BCORE_REGISTER_SPECT( bclos_closure );

            /// language specific closures
            bcore_trait_set( typeof( "bclos_language_closure" ), typeof( "bclos_closure" ) );
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

/**********************************************************************************************************************/


