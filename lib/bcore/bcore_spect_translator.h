/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

/** The translator-perspective provides the interface for object serialization. */

#ifndef BCORE_SPECT_TRANSLATOR_H
#define BCORE_SPECT_TRANSLATOR_H

#include "bcore_flect.h"
#include "bcore_features.h"

typedef struct bcore_translator_s
{
    aware_t p_type;
    tp_t    o_type;
    bcore_fp_translate fp_translate;
} bcore_translator_s;

const bcore_translator_s* bcore_translator_s_get_typed( tp_t type );
void bcore_translate(   sr_s o, sr_s obj, sr_s sink );
void bcore_translate_q( const sr_s* o, sr_s obj, sr_s sink );

vd_t bcore_spect_translator_signal( tp_t target, tp_t signal, vd_t object );

#endif  // BCORE_SPECT_TRANSLATOR_H

