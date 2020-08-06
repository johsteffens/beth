/** Author and Copyright 2017 Johannes Bernhard Steffens
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

#include "bcore_quicktypes.h"
#include "bcore_st.h"
#include "bcore_spect_inst.h"
#include "bcore_spect_array.h"

/**********************************************************************************************************************/

typedef tp_t (*hf)( sc_t );


static st_s* get_def_quicktype( hf hash, sr_s string, uz_t align )
{
    sc_t name = ( ( st_s* )string.o )->sc;
    st_s* s = st_s_createf( "#define TYPEOF_%s", name );
    uz_t pad = s->size < align ? align - s->size : 1;
    st_s_push_char_n( s, ' ', pad );
    st_s_pushf( s, "% 10"PRItp_t, hash( name ) );
    st_s_push_char( s, '\n' );
    sr_down( string );
    return s;
}

static sr_s leaf_typelist()
{
    sr_s list = bcore_inst_t_create_sr( bcore_flect_type_parse_fa( "{ st_s * [] arr; }" ) );
    bcore_array_r_push_sc( &list, "char" );
    bcore_array_r_push_sc( &list, "s0_t" );
    bcore_array_r_push_sc( &list, "s1_t" );
    bcore_array_r_push_sc( &list, "s2_t" );
    bcore_array_r_push_sc( &list, "s3_t" );
    bcore_array_r_push_sc( &list, "smax_t" );
    bcore_array_r_push_sc( &list, "u0_t" );
    bcore_array_r_push_sc( &list, "u1_t" );
    bcore_array_r_push_sc( &list, "u2_t" );
    bcore_array_r_push_sc( &list, "u3_t" );
    bcore_array_r_push_sc( &list, "umax_t" );
    bcore_array_r_push_sc( &list, "f2_t" );
    bcore_array_r_push_sc( &list, "f3_t" );
    bcore_array_r_push_sc( &list, "sz_t" );
    bcore_array_r_push_sc( &list, "uz_t" );
    bcore_array_r_push_sc( &list, "sd_t" );
    bcore_array_r_push_sc( &list, "sc_t" );
    bcore_array_r_push_sc( &list, "vd_t" );
    bcore_array_r_push_sc( &list, "vc_t" );
    bcore_array_r_push_sc( &list, "fp_t" );
    bcore_array_r_push_sc( &list, "tp_t" );
    bcore_array_r_push_sc( &list, "er_t" );
    bcore_array_r_push_sc( &list, "bl_t" );
    bcore_array_r_push_sc( &list, "st_s" );
    bcore_array_r_push_sc( &list, "aware_t" );
    bcore_array_r_push_sc( &list, "offset_t" );
    bcore_array_r_push_sc( &list, "leaf" );
    bcore_array_r_push_sc( &list, "num" );
    bcore_array_r_push_sc( &list, "type" );
    bcore_array_r_push_sc( &list, "root" );
    bcore_array_r_push_sc( &list, "spect" );
    bcore_array_r_push_sc( &list, "function_pointer" );

    bcore_array_r_push_sc( &list, "ap_t" );
    bcore_array_r_push_sc( &list, "init" );
    bcore_array_r_push_sc( &list, "down" );
    bcore_array_r_push_sc( &list, "copy" );
    bcore_array_r_push_sc( &list, "bcore_fp_init" );
    bcore_array_r_push_sc( &list, "bcore_fp_down" );
    bcore_array_r_push_sc( &list, "bcore_fp_copy" );
    bcore_array_r_push_sc( &list, "bcore_fp_copy_typed" );
    bcore_array_r_push_sc( &list, "bcore_fp_create" );
    bcore_array_r_push_sc( &list, "bcore_fp_create_typed" );
    bcore_array_r_push_sc( &list, "bcore_fp_discard" );
    bcore_array_r_push_sc( &list, "bcore_fp_clone" );


    bcore_array_r_sort( &list, 0, -1, 1 );
    return list;
}

static sr_s object_typelist()
{
    sr_s list = bcore_inst_t_create_sr( bcore_flect_type_parse_fa( "{ aware_t _; st_s * [] arr; }" ) );
    bcore_array_r_push_sc( &list, "bcore_self_s"    );
    bcore_array_r_push_sc( &list, "bcore_spect_header_s"  );
    bcore_array_r_push_sc( &list, "bcore_inst"            );
    bcore_array_r_push_sc( &list, "bcore_inst_s"          );
    bcore_array_r_push_sc( &list, "bcore_array"           );
    bcore_array_r_push_sc( &list, "bcore_array_s"         );
    bcore_array_r_push_sc( &list, "bcore_via"             );
    bcore_array_r_push_sc( &list, "bcore_via_s"           );
    bcore_array_r_push_sc( &list, "bcore_source"          );
    bcore_array_r_push_sc( &list, "bcore_source_s"        );
    bcore_array_r_push_sc( &list, "bcore_sink"            );
    bcore_array_r_push_sc( &list, "bcore_sink_s"          );
    bcore_array_r_push_sc( &list, "bcore_compare"         );
    bcore_array_r_push_sc( &list, "bcore_compare_s"       );
    bcore_array_r_push_sc( &list, "bcore_hash"            );
    bcore_array_r_push_sc( &list, "bcore_hash_s"          );
    bcore_array_r_push_sc( &list, "bcore_interpreter"     );
    bcore_array_r_push_sc( &list, "bcore_interpreter_s"   );
    bcore_array_r_push_sc( &list, "bcore_translator"      );
    bcore_array_r_push_sc( &list, "bcore_translator_s"    );
    bcore_array_r_push_sc( &list, "st_s"                  );
    bcore_array_r_push_sc( &list, "bcore_source_string_s" );
    bcore_array_r_push_sc( &list, "bcore_source_buffer_s" );
    bcore_array_r_push_sc( &list, "bcore_source_file_s"   );
    bcore_array_r_push_sc( &list, "bcore_source_chain_s"  );
    bcore_array_r_push_sc( &list, "bcore_source_point_s"  );
    bcore_array_r_push_sc( &list, "bcore_sink_buffer_s"   );
    bcore_array_r_push_sc( &list, "bcore_sink_file_s"     );
    bcore_array_r_push_sc( &list, "bcore_sink_chain_s"    );
    bcore_array_r_push_sc( &list, "bcore_hmap_u2vd_s"     );
    bcore_array_r_push_sc( &list, "bcore_hmap_tpuz_s"     );
    bcore_array_r_push_sc( &list, "bcore_hmap_tptp_s"     );
    bcore_array_r_push_sc( &list, "bcore_hmap_tpvd_s"     );
    bcore_array_r_push_sc( &list, "bcore_hmap_tpto_s"     );
    bcore_array_r_push_sc( &list, "bcore_hmap_tpfp_s"     );
    bcore_array_r_push_sc( &list, "bcore_hmap_tp_s"       );
    bcore_array_r_push_sc( &list, "bcore_hmap_tp_sr_s"    );
    bcore_array_r_push_sc( &list, "bcore_txt_ml_translator_s"  );
    bcore_array_r_push_sc( &list, "bcore_txt_ml_interpreter_s" );
    bcore_array_r_push_sc( &list, "bcore_bin_ml_translator_s"  );
    bcore_array_r_push_sc( &list, "bcore_bin_ml_interpreter_s" );
    bcore_array_r_push_sc( &list, "bcore_arr_uz_s" );
    bcore_array_r_push_sc( &list, "bcore_arr_sz_s" );
    bcore_array_r_push_sc( &list, "bcore_arr_u3_s" );
    bcore_array_r_push_sc( &list, "bcore_arr_tp_s" );
    bcore_array_r_push_sc( &list, "bcore_arr_bl_s" );
    bcore_array_r_push_sc( &list, "bcore_arr_st_s" );
    bcore_array_r_push_sc( &list, "bcore_arr_vd_s" );
    bcore_array_r_push_sc( &list, "bcore_arr_fp_s" );
    bcore_array_r_push_sc( &list, "bcore_arr_sr_s" );
    bcore_array_r_push_sc( &list, "bcore_img_u2_s" );
    bcore_array_r_push_sc( &list, "bcore_img_u2_argb" ); // pixel encoding format
    bcore_array_r_push_sc( &list, "bcore_name_s" );
    bcore_array_r_push_sc( &list, "bcore_name_map_s" );
    bcore_array_r_push_sc( &list, "bcore_matrix_s" );
    bcore_array_r_push_sc( &list, "bcore_life_s" );
    bcore_array_r_push_sc( &list, "bcore_source_context_s" );


    // signals
    bcore_array_r_push_sc( &list, "all"   );             // (target) broadcast to all
    bcore_array_r_push_sc( &list, "local" );             // (target) broadcast to local library (interpreted by library's signal handler)
    bcore_array_r_push_sc( &list, "none"  );
    bcore_array_r_push_sc( &list, "init0" );             // (type) init cycle 0
    bcore_array_r_push_sc( &list, "init1" );             // (type) init cycle 1
    bcore_array_r_push_sc( &list, "down0" );             // (type) down cycle 0
    bcore_array_r_push_sc( &list, "down1" );             // (type) down cycle 1
    bcore_array_r_push_sc( &list, "selftest" );          // (type) selftest (returns NULL or st_s*)
    bcore_array_r_push_sc( &list, "get_quicktypes" );    // (type) object referencing an array accepting strings
    bcore_array_r_push_sc( &list, "push_dependencies" ); // appends to bcore_arr_ft_s all signal handlers the current handler depends on

    // quicktypes from TYPEOF_get_quicktypes
    bcore_signal_s signal = bcore_signal_init( TYPEOF_local, TYPEOF_get_quicktypes, list.o );
    bcore_general_signal_handler( &signal );

    bcore_array_r_sort( &list, 0, -1, 1 );
    return list;
}

static uz_t max_len( const sr_s* list )
{
    uz_t len = 0;
    for( uz_t i = 0; i < bcore_array_r_get_size( list ); i++ )
    {
        uz_t size = ( ( st_s* )bcore_array_r_get( list, i ).o )->size;
        len = size > len ? size : len;
    }
    return len;
}

void bcore_quicktypes_to_stdout( tp_t (*hash)( sc_t name ) )
{
    hf hash_l = ( hash ) ? hash : typeof;
    sr_s list = leaf_typelist();
    bcore_msg( "// leaf types\n" );
    for( uz_t i = 0; i < bcore_array_r_get_size( &list ); i++ ) st_s_print_d( get_def_quicktype( hash_l, bcore_array_r_get( &list, i ), 16 + max_len( &list ) ) );
    sr_down( list );
    list = object_typelist();
    bcore_msg( "\n// other types\n" );
    for( uz_t i = 0; i < bcore_array_r_get_size( &list ); i++ ) st_s_print_d( get_def_quicktype( hash_l, bcore_array_r_get( &list, i ), 16 + max_len( &list ) ) );
    sr_down( list );
}

bl_t bcore_type_is_leaf( tp_t type )
{
    switch( type )
    {
        case TYPEOF_s3_t:
        case TYPEOF_s2_t:
        case TYPEOF_s1_t:
        case TYPEOF_s0_t:
        case TYPEOF_u3_t:
        case TYPEOF_u2_t:
        case TYPEOF_u1_t:
        case TYPEOF_u0_t:
        case TYPEOF_f3_t:
        case TYPEOF_f2_t:
        case TYPEOF_sz_t:
        case TYPEOF_uz_t:
        case TYPEOF_sd_t:
        case TYPEOF_sc_t:
        case TYPEOF_vd_t:
        case TYPEOF_vc_t:
        case TYPEOF_fp_t:
        case TYPEOF_tp_t:
        case TYPEOF_bl_t:
        case TYPEOF_aware_t:
            return true;
        default:
            return false;
    }
}

/**********************************************************************************************************************/

