/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#ifndef BCLOS_PROC_H
#define BCLOS_PROC_H

#include "bcore_hmap_tp_sr.h"
#include "bcore_spect_closure.h"

/// Procedure - closure

typedef struct bclos_proc_s bclos_proc_s;

DECLARE_FUNCTION_INIT(    bclos_proc_s )
DECLARE_FUNCTION_DOWN(    bclos_proc_s )
DECLARE_FUNCTION_COPY(    bclos_proc_s )
DECLARE_FUNCTION_CREATE(  bclos_proc_s )
DECLARE_FUNCTION_DISCARD( bclos_proc_s )
DECLARE_FUNCTION_CLONE(   bclos_proc_s )

void bclos_proc_s_push_arg(      bclos_proc_s* o, tp_t type, tp_t name, sc_t flags );  // flags of { "", "r", "w", "rw" }
void bclos_proc_s_set_ret(       bclos_proc_s* o, tp_t type, tp_t name );
void bclos_proc_s_push_op(       bclos_proc_s* o, tp_t func_name, tp_t ret_name, tp_t* arg_names, sz_t n_args );

vd_t bclos_proc_s_func   (       bclos_proc_s* o, const vd_t* args, sz_t n_args );
sz_t bclos_proc_s_n_args ( const bclos_proc_s* o );
ad_s bclos_proc_s_d_arg  ( const bclos_proc_s* o, sz_t index );
tp_t bclos_proc_s_t_ret  ( const bclos_proc_s* o );
void bclos_proc_s_set_env(       bclos_proc_s* o, vd_t env );

vd_t bclos_proc_signal( tp_t target, tp_t signal, vd_t object );

#endif // BCLOS_PROC_H

