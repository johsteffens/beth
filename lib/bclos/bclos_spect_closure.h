/** Author and Copyright 2017 Johannes Bernhard Steffens
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

typedef struct bclos_closure bclos_closure;
/// Features

// (optional) defines a closure: (re)binds instance; frm deemed lexical frame
typedef void (*bclos_closure_fp_def )( bclos_closure* o, bclos_frame_s* frm );

// calls closure function; frm deemed dynamic frame (optional);
typedef sr_s (*bclos_closure_fp_call )( const bclos_closure* o, bclos_frame_s* frm, const bclos_arguments_s* args );

// (optional) returns (dymanic) signature of closure: number of arguments, type of arguments (types could be perspectives)
typedef sr_s (*bclos_closure_fp_sig )( const bclos_closure* o );

// (optional) creates a static signature of closure: number of arguments, type of arguments (types could be perspectives)
typedef bclos_signature_s* (*bclos_closure_fp_create_static_sig )( void );

/**********************************************************************************************************************/

BCORE_DECLARE_SPECT( bclos_closure )
{
    bcore_spect_header_s  header;
    bclos_signature_s*    static_sig; // static signature
    bclos_closure_fp_def  def;
    bclos_closure_fp_call call;
    bclos_closure_fp_sig  sig; // dynamic signature
};

BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAPX( bclos_closure, def, bclos_frame_s*, frm )
BCORE_FUNC_SPECT_CONST1_RET1_ARG2_MAP1( bclos_closure, call, sr_s, bclos_frame_s*, frm, const bclos_arguments_s*, args )
BCORE_FUNC_SPECT_CONST1_RET1_ARG0_MAPX( bclos_closure, sig,  sr_s )

sr_s bclos_closure_p_call_nv( const bclos_closure_s* p, const bclos_closure* o, bclos_frame_s* frm, uz_t n, va_list args ); // call with n arguments
sr_s bclos_closure_p_call_na( const bclos_closure_s* p, const bclos_closure* o, bclos_frame_s* frm, uz_t n, ...          ); // call with n arguments
sr_s bclos_closure_r_call_nv( const sr_s* o, bclos_frame_s* frm, uz_t n, va_list args ); // call with n arguments
sr_s bclos_closure_r_call_na( const sr_s* o, bclos_frame_s* frm, uz_t n, ...          ); // call with n arguments

/**********************************************************************************************************************/

vd_t bclos_spect_closure_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/
// macros

#define BCLOS_DEFINE_STD_CLOSURE( name, signature, function )\
static vd_t name##_static_signature() { return bclos_signature_s_parse_from_sc( signature ); }\
static bcore_self_s* name##_create_self( void )\
{\
    bcore_self_s* self = bcore_self_s_build_parse_sc( #name " = bclos_closure {}", 0 );\
    bcore_self_s_push_ns_func( self, ( fp_t )function, "bclos_closure_fp_call", "call" );\
    bcore_self_s_push_ns_func( self, ( fp_t )name##_static_signature,  "bclos_closure_fp_create_static_sig", "static_sig" );\
    return self;\
}

#endif // BCLOS_SPECT_CLOSURE_H
