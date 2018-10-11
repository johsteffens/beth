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

#ifndef BCORE_SPECT_SINK_H
#define BCORE_SPECT_SINK_H

#include "bcore_first.h"
#include "bcore_types.h"
#include "bcore_feature.h"
#include "bcore_spect.h"

/// optional sink features:
typedef void (*bcore_sink_fp_set_consumer)( vd_t o, vd_t consumer );  // to create a chain of sink units
typedef void (*bcore_sink_fp_flush       )( vd_t o                );  // flushes buffers

BCORE_DECLARE_SPECT( bcore_sink )
{
    bcore_spect_header_s header;
    bcore_fp_flow_snk          push_data;
    bcore_sink_fp_set_consumer set_consumer;
    bcore_sink_fp_flush        flush;
};

BCORE_FUNC_SPECT_CONST0_RET1_ARG2_MAP1( bcore_sink, push_data, uz_t, vc_t, data, uz_t, size ) // returns number of bytes written
BCORE_FUNC_SPECT_CONST0_RET0_ARG0_MAP1( bcore_sink, flush )
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_sink, pushvf,   sc_t, format, va_list, args )
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_sink, push_fv,  sc_t, format, va_list, args )
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP0( bcore_sink, push_char,         char, c )
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP0( bcore_sink, push_sc,           sc_t, sc )
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP0( bcore_sink, push_string, const st_s*, string )
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP0( bcore_sink, push_string_d,     st_s*, string )
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP1( bcore_sink, set_consumer,      vd_t, consumer )

void bcore_sink_p_pushf   ( const bcore_sink_s* p, vd_t o, sc_t format, ... );
void bcore_sink_p_push_fa ( const bcore_sink_s* p, vd_t o, sc_t format, ... );
void bcore_sink_a_pushf   ( vd_t o, sc_t format, ... );
void bcore_sink_a_push_fa ( vd_t o, sc_t format, ... );
void bcore_sink_x_pushf   ( sr_s o, sc_t format, ... );
void bcore_sink_x_push_fa ( sr_s o, sc_t format, ... );
void bcore_sink_r_pushf   ( const sr_s* o, sc_t format, ... );
void bcore_sink_r_push_fa ( const sr_s* o, sc_t format, ... );

vd_t bcore_spect_sink_signal_handler( const bcore_signal_s* o );

#endif // BCORE_SPECT_SINK_H
