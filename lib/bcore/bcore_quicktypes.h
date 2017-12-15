/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

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
#define TYPEOF_aware_t 1507719906
#define TYPEOF_bl_t    1848869304
#define TYPEOF_char    2823553821
#define TYPEOF_f2_t    1418046714
#define TYPEOF_f3_t    3646813157
#define TYPEOF_fp_t    2940508440
#define TYPEOF_s0_t    3938332973
#define TYPEOF_s1_t    1151894370
#define TYPEOF_s2_t     219039883
#define TYPEOF_s3_t    1748356928
#define TYPEOF_sc_t    3542660944
#define TYPEOF_sd_t    4188968313
#define TYPEOF_st_s    3358332604
#define TYPEOF_sz_t    2631659827
#define TYPEOF_tp_t     908158082
#define TYPEOF_u0_t    3960371995
#define TYPEOF_u1_t    1184275984
#define TYPEOF_u2_t    3405486141
#define TYPEOF_u3_t     629390130
#define TYPEOF_vc_t     283133669
#define TYPEOF_vd_t    1618651012
#define TYPEOF_leaf    typeof( "leaf" )
#define TYPEOF_num     typeof( "num" )
#define TYPEOF_type    typeof( "type" )

// other types
#define TYPEOF_bcore_arr_sr_s             3772348668
#define TYPEOF_bcore_arr_st_s             1663834730
#define TYPEOF_bcore_arr_sz_s             1469651844
#define TYPEOF_bcore_arr_tp_s             typeof( "bcore_arr_tp_s" )
#define TYPEOF_bcore_arr_vd_s              814426351
#define TYPEOF_bcore_array_s              3532252466
#define TYPEOF_bcore_bin_ml_interpreter_s 3834633675
#define TYPEOF_bcore_bin_ml_translator_s  1158670837
#define TYPEOF_bcore_compare_s             310299276
#define TYPEOF_bcore_flect_self_s          556415048
#define TYPEOF_bcore_hmap_tp_s            3944836652
#define TYPEOF_bcore_hmap_tp_sr_s         2644016932
#define TYPEOF_bcore_hmap_tpfp_s          2363371350
#define TYPEOF_bcore_hmap_tpsz_s           232868893
#define TYPEOF_bcore_hmap_tpto_s          3958767233
#define TYPEOF_bcore_hmap_tptp_s          1873137304
#define TYPEOF_bcore_hmap_u2vd_s          1475406309
#define TYPEOF_bcore_inst_s               1413406849
#define TYPEOF_bcore_interpreter_s         695223265
#define TYPEOF_bcore_name_map_s           2156406683
#define TYPEOF_bcore_name_s               2158870146
#define TYPEOF_bcore_sink_buffer_s        4153788729
#define TYPEOF_bcore_sink_chain_s         4212754772
#define TYPEOF_bcore_sink_file_s          3697603753
#define TYPEOF_bcore_sink_s               3577023038
#define TYPEOF_bcore_source_buffer_s      3062724539
#define TYPEOF_bcore_source_chain_s       1759345018
#define TYPEOF_bcore_source_file_s           3236919
#define TYPEOF_bcore_source_s             3124187480
#define TYPEOF_bcore_source_string_s      1049426656
#define TYPEOF_bcore_translator_s         1770092211
#define TYPEOF_bcore_txt_ml_interpreter_s 2340656236
#define TYPEOF_bcore_txt_ml_translator_s  2915184580
#define TYPEOF_bcore_via_s                2457308211
#define TYPEOF_st_s                       3358332604

#endif // BCORE_QUICKTYPES_H
