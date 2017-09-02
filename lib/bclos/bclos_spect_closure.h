/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCLOS_SPECT_CLOSURE_H
#define BCLOS_SPECT_CLOSURE_H

#include "bclos_args.h"
#include "bclos_env.h"
#include "bclos_sig.h"

// Features
typedef void (*bclos_closure_fp_def  )( vd_t o, bclos_env_s* env );                    // defines a closure: turns (re)binds instance; env deemed lexical environment (optional)
typedef sr_s (*bclos_closure_fp_call )( vc_t o, bclos_env_s* env, bclos_args_s args ); // calls closure function; env deemed dynamic environment (optional); function assumes control over args
typedef sr_s (*bclos_closure_fp_sig  )( vc_t o );                                      // returns signature of closure: number of arguments, type of arguments (types could be perspectives)

typedef struct bclos_closure_s
{
    aware_t p_type; // type of perspective
    tp_t    o_type; // type of object
    bclos_closure_fp_def  fp_def;
    bclos_closure_fp_call fp_call;
    bclos_closure_fp_sig  fp_sig;
} bclos_closure_s;

bcore_flect_self_s*    bclos_closure_s_create_self( void );
const bclos_closure_s* bclos_closure_s_get_typed( tp_t type );
const bclos_closure_s* bclos_closure_s_get_aware( vc_t o );

void bclos_spect_closure_define_self_creators( void );

#endif // BCLOS_SPECT_CLOSURE_H
