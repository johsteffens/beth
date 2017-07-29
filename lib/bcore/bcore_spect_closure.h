/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCORE_SPECT_CLOSURE_H
#define BCORE_SPECT_CLOSURE_H

/** Closure perspective
 */

#include "bcore_flect.h"
#include "bcore_features.h"
#include "bcore_spect_via.h"

// Features:

// Closure function.
// The function creates the return object and passes ownership to caller.
// The returned object pointer can be NULL;
typedef vd_t (*bcore_closure_fp_func   )( vd_t o, const vc_t* args, sz_t n_args );
typedef sz_t (*bcore_closure_fp_n_args )( vc_t o ); // number of required arguments
typedef tp_t (*bcore_closure_fp_t_arg  )( vc_t o, sz_t index ); // type of indexed argument
typedef tp_t (*bcore_closure_fp_t_ret  )( vc_t o );             // type return object

typedef struct bcore_closure_s bcore_closure_s;
typedef struct bcore_closure_s
{
    aware_t p_type; // type of perspective
    tp_t    o_type; // type of object
    bcore_closure_fp_func   fp_func;   // closure function
    bcore_closure_fp_n_args fp_n_args; // number of arguments
    bcore_closure_fp_t_arg  fp_t_arg;  // type of argument
    bcore_closure_fp_t_ret  fp_t_ret;  // type of return
} bcore_closure_s;

bcore_flect_self_s*    bcore_closure_s_create_self( void );
const bcore_closure_s* bcore_closure_s_get_typed( tp_t type );
const bcore_closure_s* bcore_closure_s_get_aware( vc_t o );

vd_t bcore_closure_spect_func  ( const bcore_closure_s* p, vd_t o, const vc_t* args, sz_t n_args );
sz_t bcore_closure_spect_n_args( const bcore_closure_s* p, vc_t o ); // number of expected args
tp_t bcore_closure_spect_t_arg(  const bcore_closure_s* p, vc_t o, sz_t index );
tp_t bcore_closure_spect_t_ret(  const bcore_closure_s* p, vc_t o );

#endif // BCORE_SPECT_CLOSURE_H
