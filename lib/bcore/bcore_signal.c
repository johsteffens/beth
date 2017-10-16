/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#include "bcore_signal.h"
#include "bclos_signal.h"

#include "bcore_btree.h"
#include "bcore_control.h"
#include "bcore_flect.h"
#include "bcore_hmap.h"
#include "bcore_hmap_tp_inst.h"
#include "bcore_hmap_tp_sr.h"
#include "bcore_life.h"
#include "bcore_tbman.h"
#include "bcore_name_manager.h"
#include "bcore_ref.h"
#include "bcore_signature.h"
#include "bcore_sinks.h"
#include "bcore_sources.h"
#include "bcore_spect.h"
#include "bcore_spect_array.h"
#include "bcore_spect_compare.h"
#include "bcore_spect_inst.h"
#include "bcore_spect_interpreter.h"
#include "bcore_spect_sink.h"
#include "bcore_spect_source.h"
#include "bcore_spect_translator.h"
#include "bcore_spect_via.h"
#include "bcore_st.h"
#include "bcore_threads.h"
#include "bcore_trait.h"
#include "bcore_txt_ml.h"
#include "bcore_bin_ml.h"
#include "bcore_arr.h"

vd_t bcore_signal( tp_t target, tp_t signal, vd_t object )
{
    bcore_fp_signal arr[] =
    {
        /// system critical items (keep order)
        bcore_tbman_signal,
        bcore_name_manager_signal,
        bcore_signature_signal,
        bcore_flect_signal,
        bcore_spect_signal,
        bcore_trait_signal,
        bcore_st_signal,

        /// other items
        bcore_btree_signal,
        bcore_control_signal,
        bcore_hmap_signal,
        bcore_hmap_tp_inst_signal,
        bcore_hmap_tp_sr_signal,
        bcore_life_signal,
        bcore_ref_signal,
        bcore_sinks_signal,
        bcore_sources_signal,
        bcore_spect_array_signal,
        bcore_spect_compare_signal,
        bcore_spect_inst_signal,
        bcore_spect_interpreter_signal,
        bcore_spect_sink_signal,
        bcore_spect_source_signal,
        bcore_spect_translator_signal,
        bcore_spect_via_signal,
        bcore_threads_signal,
        bcore_txt_ml_signal,
        bcore_bin_ml_signal,
        bcore_arr_signal,

        /// sub libraries
        bclos_signal
    };

    sz_t size = sizeof( arr ) / sizeof( bcore_fp_signal );
    vd_t ret = NULL;

    if( signal == typeof( "down0" ) || signal == typeof( "down1" ) )
    {
        for( sz_t i = size; i > 0; i-- ) if( ( ret = arr[ i - 1 ]( target, signal, object ) ) ) return ret;
    }
    else
    {
        for( sz_t i = 0; i < size; i++ ) if( ( ret = arr[ i     ]( target, signal, object ) ) ) return ret;
    }

    return ret;
}
