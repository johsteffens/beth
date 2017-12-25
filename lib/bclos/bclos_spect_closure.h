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

#ifndef BCLOS_SPECT_CLOSURE_H
#define BCLOS_SPECT_CLOSURE_H

#include "bclos_arguments.h"
#include "bclos_frame.h"
#include "bclos_signature.h"

/// Features

// (optional) defines a closure: (re)binds instance; frm deemed lexical frame
typedef void (*bclos_closure_fp_def )( vd_t o, bclos_frame_s* frm );

// calls closure function; frm deemed dynamic frame (optional);
typedef sr_s (*bclos_closure_fp_call )( vc_t o, bclos_frame_s* frm, const bclos_arguments_s* args );

// (optional) returns (dymanic) signature of closure: number of arguments, type of arguments (types could be perspectives)
typedef sr_s (*bclos_closure_fp_sig )( vc_t o );

// (optional) creates a static signature of closure: number of arguments, type of arguments (types could be perspectives)
typedef bclos_signature_s* (*bclos_closure_fp_create_static_sig )( void );

typedef struct bclos_closure_s
{
    aware_t p_type; // type of perspective
    tp_t    o_type; // type of object
    bclos_signature_s*    static_sig; // static signature
    bclos_closure_fp_def  fp_def;
    bclos_closure_fp_call fp_call;
    bclos_closure_fp_sig  fp_sig; // dynamic signature
} bclos_closure_s;

const bclos_closure_s* bclos_closure_s_get_typed( tp_t type );
const bclos_closure_s* bclos_closure_s_get_aware( vc_t o );

void bclos_closure_spect_def(     const bclos_closure_s* p, vd_t o, bclos_frame_s* frm );
sr_s bclos_closure_spect_call(    const bclos_closure_s* p, vc_t o, bclos_frame_s* frm, const bclos_arguments_s* args );
sr_s bclos_closure_spect_sig(     const bclos_closure_s* p, vc_t o );
sr_s bclos_closure_spect_call_nv( const bclos_closure_s* p, vc_t o, bclos_frame_s* frm, sz_t n, va_list args ); // call with n arguments
sr_s bclos_closure_spect_call_na( const bclos_closure_s* p, vc_t o, bclos_frame_s* frm, sz_t n, ...          ); // call with n arguments

void bclos_closure_q_def(     const sr_s* o, bclos_frame_s* frm );
sr_s bclos_closure_q_call(    const sr_s* o, bclos_frame_s* frm, const bclos_arguments_s* args );
sr_s bclos_closure_q_sig(     const sr_s* o );
sr_s bclos_closure_q_call_nv( const sr_s* o, bclos_frame_s* frm, sz_t n, va_list args ); // call with n arguments
sr_s bclos_closure_q_call_na( const sr_s* o, bclos_frame_s* frm, sz_t n, ...          ); // call with n arguments

vd_t bclos_spect_closure_signal( tp_t target, tp_t signal, vd_t object );

#define DEFINE_STD_CLOSURE( name, signature, function )\
static vd_t name##_static_signature() { return bclos_signature_s_parse_from_sc( signature ); }\
static bcore_flect_self_s* name##_create_self( void )\
{\
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( #name " = bclos_closure {}", 0 );\
    bcore_flect_self_s_push_ns_func( self, ( fp_t )function, "bclos_closure_fp_call", "call" );\
    bcore_flect_self_s_push_ns_func( self, ( fp_t )name##_static_signature,  "bclos_closure_fp_create_static_sig", "static_sig" );\
    return self;\
}



#endif // BCLOS_SPECT_CLOSURE_H
