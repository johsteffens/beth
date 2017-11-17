/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

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
    bclos_closure_fp_def  fp_def;
    bclos_closure_fp_call fp_call;
    bclos_closure_fp_sig  fp_sig; // dynamic signature
    bclos_signature_s*    static_sig; // static signature
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

#endif // BCLOS_SPECT_CLOSURE_H
