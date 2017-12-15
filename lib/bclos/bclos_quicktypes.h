/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

// Quickly obtainable types.

#ifndef BCLOS_QUICKTYPES_H
#define BCLOS_QUICKTYPES_H

#include "bcore_name_manager.h"
#include "bcore_quicktypes.h"

/// This function computes and formats defines below to stdout
void bclos_quicktypes_to_stdout( tp_t (*hash)( sc_t name ) );

/** List of predefined types for quick access.
 *  Created via bclos_quicktypes_to_stdout( NULL );
 */

#define TYPEOF_bclos_address_s      2730951097
#define TYPEOF_bclos_arg_sig_s      1403124995
#define TYPEOF_bclos_arguments_s      94569669
#define TYPEOF_bclos_closure_s      1095539800
#define TYPEOF_bclos_closure        typeof( "bclos_closure" )
#define TYPEOF_bclos_expression_s   3421005121
#define TYPEOF_bclos_frame_s        2127241660
#define TYPEOF_bclos_identity_s       73605353
#define TYPEOF_bclos_library_arr_s  3282907526
#define TYPEOF_bclos_library_s       945809950
#define TYPEOF_bclos_proc_binding_s 1894874949
#define TYPEOF_bclos_procedure_s    3531902884
#define TYPEOF_bclos_return         3014526027
#define TYPEOF_bclos_signature_s    3005312691
#define TYPEOF_bclos_statement_s    4269937196
#define TYPEOF_bclos_syntax_s       1644286864

#endif // BCLOS_QUICKTYPES_H
