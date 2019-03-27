/** Copyright 2019 Johannes Bernhard Steffens
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

#ifndef BMATH_SPECT_ADAPTIVE_H
#define BMATH_SPECT_ADAPTIVE_H

/// An adaptive object is trainable (e.g. neural network)

#include "bmath_mf3.h"
#include "bmath_vector.h"
#include "bmath_quicktypes.h"

/**********************************************************************************************************************/
/// features

BCORE_FORWARD_OBJECT( bmath_adaptive );

/// input vector size
typedef sz_t (*bmath_fp_adaptive_get_in_size )( const bmath_adaptive* o );
typedef void (*bmath_fp_adaptive_set_in_size )(       bmath_adaptive* o, sz_t size );

/// output vector size
typedef sz_t (*bmath_fp_adaptive_get_out_size )( const bmath_adaptive* o );
typedef void (*bmath_fp_adaptive_set_out_size )(       bmath_adaptive* o, sz_t size );

/// adaption step
typedef f3_t (*bmath_fp_adaptive_get_step )( const bmath_adaptive* o );
typedef void (*bmath_fp_adaptive_set_step )(       bmath_adaptive* o, f3_t val );

/// adaption decay
typedef f3_t (*bmath_fp_adaptive_get_decay )( const bmath_adaptive* o );
typedef void (*bmath_fp_adaptive_set_decay )(       bmath_adaptive* o, f3_t val );

/// setup of the internal architecture (usually, this function needs not be explicitly called)
typedef void (*bmath_fp_adaptive_setup )( bmath_adaptive* o, bl_t training );

/// resets adaptive to the untrained state
typedef void (*bmath_fp_adaptive_set_untrained )( bmath_adaptive* o );

/// outputs architecture to text sink (for easy inspection)
typedef void (*bmath_fp_adaptive_arc_to_sink )( const bmath_adaptive* o, bcore_sink* sink );

/** query == inference
 *  Note that querying can change the internal state of the adaptive.
 *  It might not be reentrant (e.g. recurrent network) and is usually not concurrent.
 */
typedef void (*bmath_fp_adaptive_query )( bmath_adaptive* o, const bmath_vf3_s* in, bmath_vf3_s* out );

/// adaptation step (out can be NULL)
typedef void (*bmath_fp_adaptive_adapt )( bmath_adaptive* o, const bmath_vf3_s* in, const bmath_vf3_s* target, bmath_vf3_s* out );

/**********************************************************************************************************************/

BCORE_DECLARE_SPECT( bmath_adaptive )
{
    bcore_spect_header_s header;

    // adaptive features
    bmath_fp_adaptive_get_in_size   get_in_size;
    bmath_fp_adaptive_set_in_size   set_in_size;
    bmath_fp_adaptive_get_out_size  get_out_size;
    bmath_fp_adaptive_set_out_size  set_out_size;
    bmath_fp_adaptive_get_step      get_step;
    bmath_fp_adaptive_set_step      set_step;
    bmath_fp_adaptive_get_decay     get_decay;
    bmath_fp_adaptive_set_decay     set_decay;
    bmath_fp_adaptive_setup         setup;
    bmath_fp_adaptive_set_untrained set_untrained;
    bmath_fp_adaptive_arc_to_sink   arc_to_sink;
    bmath_fp_adaptive_query         query;
    bmath_fp_adaptive_adapt         adapt;
};

// features
BCORE_FUNC_SPECT_CONST1_RET1_ARG0_MAP1( bmath_adaptive, get_in_size,  sz_t )
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP1( bmath_adaptive, set_in_size,  sz_t, size )
BCORE_FUNC_SPECT_CONST1_RET1_ARG0_MAP1( bmath_adaptive, get_out_size, sz_t )
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP1( bmath_adaptive, set_out_size, sz_t, size )
BCORE_FUNC_SPECT_CONST1_RET1_ARG0_MAPX( bmath_adaptive, get_step,     f3_t )
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAPX( bmath_adaptive, set_step,     f3_t, val )
BCORE_FUNC_SPECT_CONST1_RET1_ARG0_MAPX( bmath_adaptive, get_decay,    f3_t )
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAPX( bmath_adaptive, set_decay,    f3_t, val )
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAPX( bmath_adaptive, setup,        bl_t, training )
BCORE_FUNC_SPECT_CONST0_RET0_ARG0_MAP1( bmath_adaptive, set_untrained )
BCORE_FUNC_SPECT_CONST1_RET0_ARG1_MAPX( bmath_adaptive, arc_to_sink, bcore_sink*, sink )
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP1( bmath_adaptive, query, const bmath_vf3_s*, in,                             bmath_vf3_s*, out )
BCORE_FUNC_SPECT_CONST0_RET0_ARG3_MAP1( bmath_adaptive, adapt, const bmath_vf3_s*, in, const bmath_vf3_s*, target, bmath_vf3_s*, out )

/// simplified query for output size == 1; returns output activation.
BCORE_FUNC_SPECT_CONST0_RET1_ARG1_MAP0( bmath_adaptive, query_1, f3_t, const bmath_vf3_s*, in )

/// simplified adapt for output size == 1; returns output activation.
BCORE_FUNC_SPECT_CONST0_RET1_ARG2_MAP0( bmath_adaptive, adapt_1, f3_t, const bmath_vf3_s*, in, f3_t, target )

/**********************************************************************************************************************/

/// runs a standard training test for a given adaptive
void bmath_adaptive_a_test( const bmath_adaptive* o );

/**********************************************************************************************************************/

vd_t bmath_spect_adaptive_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_SPECT_ADAPTIVE_H

