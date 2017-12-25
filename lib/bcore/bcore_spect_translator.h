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

