/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCORE_SPECT_CLOSURE_H
#define BCORE_SPECT_CLOSURE_H

/** Closure perspective
 */

#include "bcore_flect.h"
#include "bcore_features.h"
#include "bcore_spect_via.h"

/// required features:

// Closure function. The function creates return object and pases control to caller.
typedef vd_t (*bcore_closure_fp_func   )( vd_t o, const vc_t* args, sz_t n_args );
typedef sz_t (*bcore_closure_fp_n_args )( vc_t o );             // number of expected args
typedef tp_t (*bcore_closure_fp_tp_ret )( vc_t o );             // return type
typedef tp_t (*bcore_closure_fp_tp_arg )( vc_t o, sz_t index ); // type of indexed argument

typedef struct bcore_closure_s bcore_closure_s;
typedef struct bcore_closure_s
{
    aware_t  p_type;   // type of perspective
    tp_t     o_type;   // type of object
    bcore_closure_fp_func   fp_func;   // closure function
    bcore_closure_fp_n_args fp_n_args; // number of arguments
    bcore_closure_fp_tp_ret fp_tp_ret; // return type
    bcore_closure_fp_tp_arg fp_tp_arg; // argument type
} bcore_closure_s;

bcore_flect_self_s* bcore_closure_s_create_self();
const bcore_closure_s* bcore_closure_s_get_typed( tp_t type );
const bcore_closure_s* bcore_closure_s_get_aware( vc_t o );

vd_t bcore_closure_spect_func  ( const bcore_closure_s* p, vd_t o, const vc_t* args, sz_t n_args );
sz_t bcore_closure_spect_n_args( const bcore_closure_s* p, vc_t o );             // number of expected args
tp_t bcore_closure_spect_tp_ret( const bcore_closure_s* p, vc_t o );             // return type
tp_t bcore_closure_spect_tp_arg( const bcore_closure_s* p, vc_t o, sz_t index ); // type of indexed argument

/**********************************************************************************************************************/
// testing, debugging

bcore_string_s* bcore_spect_closure_selftest();

#endif // BCORE_SPECT_CLOSURE_H
