/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

// Quickly obtainable types.

#ifndef BCORE_QUICKTYPES_H
#define BCORE_QUICKTYPES_H

#include "bcore_name_manager.h"

/// This function computes and formats defines below to stdout
void bcore_quicktypes_to_stdout( tp_t (*hash)( sc_t name ) );

/// identifies leaf types according to definition nbelow
bl_t bcore_type_is_leaf( tp_t type );

/** List of predefined types for quick access.
 *  Created via bcore_quicktypes_to_stdout( NULL );
 */

// leaf types
#define TYPEOF_s3_t    1748356928
#define TYPEOF_s2_t     219039883
#define TYPEOF_s1_t    1151894370
#define TYPEOF_s0_t    3938332973
#define TYPEOF_u3_t     629390130
#define TYPEOF_u2_t    3405486141
#define TYPEOF_u1_t    1184275984
#define TYPEOF_u0_t    3960371995
#define TYPEOF_f3_t    3646813157
#define TYPEOF_f2_t    1418046714
#define TYPEOF_sz_t    2631659827
#define TYPEOF_sd_t    4188968313
#define TYPEOF_sc_t    3542660944
#define TYPEOF_vd_t    1618651012
#define TYPEOF_vc_t     283133669
#define TYPEOF_fp_t    2940508440
#define TYPEOF_tp_t     908158082
#define TYPEOF_bl_t    1848869304
#define TYPEOF_aware_t 1507719906

// other types
#define TYPEOF_bcore_array_s              3532252466
#define TYPEOF_bcore_closure_s            3994021118
#define TYPEOF_bcore_compare_s             310299276
#define TYPEOF_bcore_flect_self_s          556415048
#define TYPEOF_bcore_inst_s               1413406849
#define TYPEOF_bcore_interpreter_s         695223265
#define TYPEOF_bcore_sink_buffer_s        4153788729
#define TYPEOF_bcore_sink_chain_s         4212754772
#define TYPEOF_bcore_sink_file_s          3697603753
#define TYPEOF_bcore_sink_s               3577023038
#define TYPEOF_bcore_source_buffer_s      3062724539
#define TYPEOF_bcore_source_chain_s       1759345018
#define TYPEOF_bcore_source_file_s           3236919
#define TYPEOF_bcore_source_s             3124187480
#define TYPEOF_bcore_source_string_s      1049426656
#define TYPEOF_bcore_string_s             2419449576
#define TYPEOF_bcore_translator_s         1770092211
#define TYPEOF_bcore_txt_ml_interpreter_s 2340656236
#define TYPEOF_bcore_txt_ml_translator_s  2915184580
#define TYPEOF_bcore_via_s                2457308211

#endif // BCORE_QUICKTYPES_H
