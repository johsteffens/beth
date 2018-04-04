/** Copyright 2017 Johannes Bernhard Steffens
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

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
#define TYPEOF_aware_t          1507719906
#define TYPEOF_bl_t             1848869304
#define TYPEOF_char             2823553821
#define TYPEOF_f2_t             1418046714
#define TYPEOF_f3_t             3646813157
#define TYPEOF_fp_t             2940508440
#define TYPEOF_function_pointer 3430229891
#define TYPEOF_leaf              642305365
#define TYPEOF_num               242376115
#define TYPEOF_offset_t         1818129441
#define TYPEOF_root              553455173
#define TYPEOF_s0_t             3938332973
#define TYPEOF_s1_t             1151894370
#define TYPEOF_s2_t              219039883
#define TYPEOF_s3_t             1748356928
#define TYPEOF_sc_t             3542660944
#define TYPEOF_sd_t             4188968313
#define TYPEOF_smax_t            177359151
#define TYPEOF_st_s             3358332604
#define TYPEOF_sz_t             2631659827
#define TYPEOF_tp_t              908158082
#define TYPEOF_type             1361572173
#define TYPEOF_u0_t             3960371995
#define TYPEOF_u1_t             1184275984
#define TYPEOF_u2_t             3405486141
#define TYPEOF_u3_t              629390130
#define TYPEOF_umax_t           3191476325
#define TYPEOF_vc_t              283133669
#define TYPEOF_vd_t             1618651012

// other types
#define TYPEOF_all                         321211332
#define TYPEOF_bcore_arr_fp_s              534232891
#define TYPEOF_bcore_arr_sr_s             3772348668
#define TYPEOF_bcore_arr_st_s             1663834730
#define TYPEOF_bcore_arr_sz_s             1469651844
#define TYPEOF_bcore_arr_tp_s             3894726581
#define TYPEOF_bcore_arr_u3_s             3372156401
#define TYPEOF_bcore_arr_vd_s              814426351
#define TYPEOF_bcore_array_s              3532252466
#define TYPEOF_bcore_bin_ml_interpreter_s 3834633675
#define TYPEOF_bcore_bin_ml_translator_s  1158670837
#define TYPEOF_bcore_compare_s             310299276
#define TYPEOF_bcore_hmap_tp_s            3944836652
#define TYPEOF_bcore_hmap_tp_sr_s         2644016932
#define TYPEOF_bcore_hmap_tpfp_s          2363371350
#define TYPEOF_bcore_hmap_tpsz_s           232868893
#define TYPEOF_bcore_hmap_tpto_s          3958767233
#define TYPEOF_bcore_hmap_tptp_s          1873137304
#define TYPEOF_bcore_hmap_u2vd_s          1475406309
#define TYPEOF_bcore_inst_s               1413406849
#define TYPEOF_bcore_interpreter_s         695223265
#define TYPEOF_bcore_matrix_s              686911304
#define TYPEOF_bcore_name_map_s           2156406683
#define TYPEOF_bcore_name_s               2158870146
#define TYPEOF_bcore_self_s                422978351
#define TYPEOF_bcore_sink_buffer_s        4153788729
#define TYPEOF_bcore_sink_chain_s         4212754772
#define TYPEOF_bcore_sink_file_s          3697603753
#define TYPEOF_bcore_sink_s               3577023038
#define TYPEOF_bcore_source_buffer_s      3062724539
#define TYPEOF_bcore_source_chain_s       1759345018
#define TYPEOF_bcore_source_file_s           3236919
#define TYPEOF_bcore_source_s             3124187480
#define TYPEOF_bcore_source_string_s      1049426656
#define TYPEOF_bcore_spect_header_s       2111812658
#define TYPEOF_bcore_translator_s         1770092211
#define TYPEOF_bcore_txt_ml_interpreter_s 2340656236
#define TYPEOF_bcore_txt_ml_translator_s  2915184580
#define TYPEOF_bcore_via_s                2457308211
#define TYPEOF_down0                      1348367679
#define TYPEOF_down1                      1331590060
#define TYPEOF_init0                      4243559033
#define TYPEOF_init1                      4226781414
#define TYPEOF_none                       2913447899
#define TYPEOF_selftest                   2526214595
#define TYPEOF_st_s                       3358332604

#endif // BCORE_QUICKTYPES_H
