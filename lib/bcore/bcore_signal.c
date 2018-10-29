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

#include "bcore_signal.h"

#include "bcore_arr.h"
#include "bcore_img.h"
#include "bcore_bin_ml.h"
#include "bcore_btree.h"
#include "bcore_control.h"
#include "bcore_flect.h"
#include "bcore_hmap.h"
#include "bcore_hmap_tp_sr.h"
#include "bcore_life.h"
#include "bcore_tbman.h"
#include "bcore_name.h"
#include "bcore_name_manager.h"
#include "bcore_function_manager.h"
#include "bcore_const_manager.h"
#include "bcore_sr.h"
#include "bcore_sinks.h"
#include "bcore_sources.h"
#include "bcore_spect.h"
#include "bcore_spect_array.h"
#include "bcore_spect_compare.h"
#include "bcore_spect_hash.h"
#include "bcore_spect_inst.h"
#include "bcore_spect_interpreter.h"
#include "bcore_spect_sink.h"
#include "bcore_spect_source.h"
#include "bcore_spect_translator.h"
#include "bcore_spect_via.h"
#include "bcore_spect_matrix.h"
#include "bcore_sc.h"
#include "bcore_st.h"
#include "bcore_tp.h"
#include "bcore_tp_fastmap.h"
#include "bcore_threads.h"
#include "bcore_trait.h"
#include "bcore_txt_ml.h"
#include "bcore_leaf.h"
#include "bcore_file.h"

BCORE_DEFINE_FUNCTIONS_OBJ_FLAT( bcore_signal_s )

tp_t bcore_signal_s_handle_type( const bcore_signal_s* o, tp_t target )
{
    if( ( o->target == TYPEOF_all ) || ( o->target == target ) ) return o->type;
    return TYPEOF_none;
}

/**********************************************************************************************************************/

vd_t bcore_signal_s_broadcast( const bcore_signal_s* o, bcore_fp_signal_handler* arr, uz_t size )
{
    vd_t ret = NULL;

    if( o->type == TYPEOF_down0 || o->type == TYPEOF_down1 )
    {
        for( uz_t i = size; i > 0; i-- ) if( ( ret = arr[ i - 1 ]( o ) ) ) return ret;
    }
    else
    {
        for( uz_t i = 0; i < size; i++ ) if( ( ret = arr[ i     ]( o ) ) ) return ret;
    }

    return ret;
}

bcore_signal_s bcore_signal_init( tp_t target, tp_t type, vd_t object )
{
    return ( bcore_signal_s ){ .target = target, .type = type, .object = object };
}

vd_t bcore_signal_handler( const bcore_signal_s* o )
{
    bcore_fp_signal_handler arr[] =
    {
        /// system critical items (keep order)
        bcore_tbman_signal_handler,
        bcore_tp_signal_handler,
        bcore_name_manager_signal_handler,
        bcore_function_manager_signal_handler,
        bcore_flect_signal_handler,
        bcore_feature_signal_handler,
        bcore_trait_signal_handler,
        bcore_spect_signal_handler,
        bcore_sc_signal_handler,
        bcore_st_signal_handler,
        bcore_spect_source_signal_handler,
        bcore_const_manager_signal_handler,

        /// other items
        bcore_control_signal_handler,
        bcore_name_signal_handler,
        bcore_tp_fastmap_signal_handler,
        bcore_spect_inst_signal_handler,
        bcore_spect_via_signal_handler,
        bcore_btree_signal_handler,
        bcore_hmap_signal_handler,
        bcore_hmap_tp_sr_signal_handler,
        bcore_life_signal_handler,
        bcore_ref_signal_handler,
        bcore_spect_array_signal_handler,
        bcore_sinks_signal_handler,
        bcore_sources_signal_handler,
        bcore_spect_compare_signal_handler,
        bcore_spect_sink_signal_handler,
        bcore_spect_interpreter_signal_handler,
        bcore_spect_translator_signal_handler,
        bcore_spect_matrix_signal_handler,
        bcore_threads_signal_handler,
        bcore_txt_ml_signal_handler,
        bcore_bin_ml_signal_handler,
        bcore_arr_signal_handler,
        bcore_img_signal_handler,
        bcore_leaf_signal_handler,
        bcore_file_signal_handler,
        bcore_spect_hash_signal_handler,
    };

    return bcore_signal_s_broadcast( o, arr, sizeof( arr ) / sizeof( bcore_fp_signal_handler ) );
}
