/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_signal.h"
#include "bclos_signal.h"

#include "bcore_btree.h"
#include "bcore_control.h"
#include "bcore_flect.h"
#include "bcore_hmap.h"
#include "bcore_hmap_tp_inst.h"
#include "bcore_hmap_tp_sr.h"
#include "bcore_life.h"
#include "bcore_memory_manager.h"
#include "bcore_name_manager.h"
#include "bcore_ref.h"
#include "bcore_signature.h"
#include "bcore_sinks.h"
#include "bcore_sources.h"
#include "bcore_spect.h"
#include "bcore_spect_array.h"
#include "bcore_spect_closure.h"
#include "bcore_spect_compare.h"
#include "bcore_spect_inst.h"
#include "bcore_spect_interpreter.h"
#include "bcore_spect_sink.h"
#include "bcore_spect_source.h"
#include "bcore_spect_translator.h"
#include "bcore_spect_via.h"
#include "bcore_string.h"
#include "bcore_threads.h"
#include "bcore_trait.h"
#include "bcore_txt_ml.h"

vd_t bcore_signal( tp_t t, tp_t s, vd_t o )
{
    vd_t ret = NULL;
    if( ( ret = bcore_btree_signal             ( t, s, o ) ) ) return ret;
    if( ( ret = bcore_control_signal           ( t, s, o ) ) ) return ret;
    if( ( ret = bcore_flect_signal             ( t, s, o ) ) ) return ret;
    if( ( ret = bcore_hmap_signal              ( t, s, o ) ) ) return ret;
    if( ( ret = bcore_hmap_tp_inst_signal      ( t, s, o ) ) ) return ret;
    if( ( ret = bcore_hmap_tp_sr_signal        ( t, s, o ) ) ) return ret;
    if( ( ret = bcore_life_signal              ( t, s, o ) ) ) return ret;
    if( ( ret = bcore_memory_manager_signal    ( t, s, o ) ) ) return ret;
    if( ( ret = bcore_name_manager_signal      ( t, s, o ) ) ) return ret;
    if( ( ret = bcore_ref_signal               ( t, s, o ) ) ) return ret;
    if( ( ret = bcore_signature_signal         ( t, s, o ) ) ) return ret;
    if( ( ret = bcore_sinks_signal             ( t, s, o ) ) ) return ret;
    if( ( ret = bcore_sources_signal           ( t, s, o ) ) ) return ret;
    if( ( ret = bcore_spect_signal             ( t, s, o ) ) ) return ret;
    if( ( ret = bcore_spect_array_signal       ( t, s, o ) ) ) return ret;
    if( ( ret = bcore_spect_closure_signal     ( t, s, o ) ) ) return ret;
    if( ( ret = bcore_spect_compare_signal     ( t, s, o ) ) ) return ret;
    if( ( ret = bcore_spect_inst_signal        ( t, s, o ) ) ) return ret;
    if( ( ret = bcore_spect_interpreter_signal ( t, s, o ) ) ) return ret;
    if( ( ret = bcore_spect_sink_signal        ( t, s, o ) ) ) return ret;
    if( ( ret = bcore_spect_source_signal      ( t, s, o ) ) ) return ret;
    if( ( ret = bcore_spect_translator_signal  ( t, s, o ) ) ) return ret;
    if( ( ret = bcore_spect_via_signal         ( t, s, o ) ) ) return ret;
    if( ( ret = bcore_string_signal            ( t, s, o ) ) ) return ret;
    if( ( ret = bcore_threads_signal           ( t, s, o ) ) ) return ret;
    if( ( ret = bcore_trait_signal             ( t, s, o ) ) ) return ret;
    if( ( ret = bcore_txt_ml_signal            ( t, s, o ) ) ) return ret;

    /// other sub-libraries
    if( ( ret = bclos_signal                   ( t, s, o ) ) ) return ret;
    return ret;
}
