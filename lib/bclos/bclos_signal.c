/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bclos_signal.h"

#include "bclos_leaf.h"
#include "bclos_proc.h"
#include "bclos_env.h"
#include "bclos_args.h"
#include "bclos_signature.h"
#include "bclos_spect_closure.h"
#include "bclos_statement.h"
#include "bclos_procedure.h"
#include "bclos_tree.h"

vd_t bclos_signal( tp_t t, tp_t s, vd_t o )
{
    vd_t ret = NULL;
    if( ( ret = bclos_args_signal          ( t, s, o ) ) ) return ret;
    if( ( ret = bclos_env_signal           ( t, s, o ) ) ) return ret;
    if( ( ret = bclos_leaf_signal          ( t, s, o ) ) ) return ret;
    if( ( ret = bclos_proc_signal          ( t, s, o ) ) ) return ret;
    if( ( ret = bclos_procedure_signal     ( t, s, o ) ) ) return ret;
    if( ( ret = bclos_signature_signal     ( t, s, o ) ) ) return ret;
    if( ( ret = bclos_spect_closure_signal ( t, s, o ) ) ) return ret;
    if( ( ret = bclos_statement_signal     ( t, s, o ) ) ) return ret;
    if( ( ret = bclos_tree_signal          ( t, s, o ) ) ) return ret;
    return ret;
}
