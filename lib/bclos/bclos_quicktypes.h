/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

// Quickly obtainable types.

#ifndef BCLOS_QUICKTYPES_H
#define BCLOS_QUICKTYPES_H

#include "bcore_name_manager.h"

/// This function computes and formats defines below to stdout
void bclos_quicktypes_to_stdout( tp_t (*hash)( sc_t name ) );

/** List of predefined types for quick access.
 *  Created via bclos_quicktypes_to_stdout( NULL );
 */

#define TYPEOF_bclos_arg_sig_s      1403124995
#define TYPEOF_bclos_args_s         2174318074
#define TYPEOF_bclos_closure_s      1095539800
#define TYPEOF_bclos_env_s          1590357050
#define TYPEOF_bclos_proc_binding_s 1894874949
#define TYPEOF_bclos_proc_s          282422711
#define TYPEOF_bclos_procedure_s    3531902884
#define TYPEOF_bclos_signature_s    3005312691
#define TYPEOF_bclos_statement_s    4269937196
#define TYPEOF_bclos_tree_s         1583508963

#endif // BCLOS_QUICKTYPES_H
