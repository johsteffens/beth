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

/** Binary markup language for marshaling and creation of objects.
 *  Implements features translate, interpret and supports corresponding perspectives.
 *
 *  The interpreter sends bcore_spect_via_call:mutated to object after alteration.
 */

#ifndef BCORE_BIN_ML_H
#define BCORE_BIN_ML_H

#include "bcore_flect.h"
#include "bcore_feature.h"

/**********************************************************************************************************************/

typedef struct bcore_bin_ml_translator_s
{
    aware_t _;
} bcore_bin_ml_translator_s;

BCORE_DECLARE_FUNCTION_INIT(    bcore_bin_ml_translator_s )
BCORE_DECLARE_FUNCTION_DOWN(    bcore_bin_ml_translator_s )
BCORE_DECLARE_FUNCTION_COPY(    bcore_bin_ml_translator_s )
BCORE_DECLARE_FUNCTION_CREATE(  bcore_bin_ml_translator_s )
BCORE_DECLARE_FUNCTION_DISCARD( bcore_bin_ml_translator_s )
BCORE_DECLARE_FUNCTION_CLONE(   bcore_bin_ml_translator_s )

void bcore_bin_ml_translator_s_translate( const bcore_bin_ml_translator_s* o, sr_s obj, sr_s snk );

/**********************************************************************************************************************/

/// syntactic sugar
sr_s bcore_bin_ml_x_to_sink_buffer( sr_s obj ); // returns bcore_sink_buffer_s
sr_s bcore_bin_ml_a_to_sink_buffer( vc_t obj ); // returns bcore_sink_buffer_s

void bcore_bin_ml_x_to_sink(   sr_s obj, bcore_sink* sink );
void bcore_bin_ml_x_to_stdout( sr_s obj );
void bcore_bin_ml_x_to_file(   sr_s obj, sc_t file );
void bcore_bin_ml_x_to_string( sr_s obj, st_s* string );

void bcore_bin_ml_r_to_sink(   const sr_s* obj, bcore_sink* sink );
void bcore_bin_ml_r_to_stdout( const sr_s* obj );
void bcore_bin_ml_r_to_file(   const sr_s* obj, sc_t file );
void bcore_bin_ml_r_to_string( const sr_s* obj, st_s* string );

void bcore_bin_ml_a_to_sink(   vc_t o, bcore_sink* sink );
void bcore_bin_ml_a_to_stdout( vc_t o );
void bcore_bin_ml_a_to_file(   vc_t o, sc_t file );
void bcore_bin_ml_a_to_string( vc_t o, st_s* string );

void bcore_bin_ml_t_to_sink(   tp_t t, vc_t o, bcore_sink* sink );
void bcore_bin_ml_t_to_stdout( tp_t t, vc_t o );
void bcore_bin_ml_t_to_file(   tp_t t, vc_t o, sc_t file );
void bcore_bin_ml_t_to_string( tp_t t, vc_t o, st_s* string );


/**********************************************************************************************************************/

typedef struct bcore_bin_ml_interpreter_s
{
    aware_t _;
} bcore_bin_ml_interpreter_s;

void                        bcore_bin_ml_interpreter_s_init(            bcore_bin_ml_interpreter_s* o );
void                        bcore_bin_ml_interpreter_s_down(            bcore_bin_ml_interpreter_s* o );
void                        bcore_bin_ml_interpreter_s_copy(            bcore_bin_ml_interpreter_s* o, const bcore_bin_ml_interpreter_s* src );
bcore_bin_ml_interpreter_s* bcore_bin_ml_interpreter_s_create();
bcore_bin_ml_interpreter_s* bcore_bin_ml_interpreter_s_clone(     const bcore_bin_ml_interpreter_s* o );
void                        bcore_bin_ml_interpreter_s_discard(         bcore_bin_ml_interpreter_s* o );

sr_s bcore_bin_ml_interpreter_s_interpret( const bcore_bin_ml_interpreter_s* o, sr_s src );

/**********************************************************************************************************************/

/// syntactic sugar
sr_s bcore_bin_ml_from_file( sc_t file );
void bcore_bin_ml_a_from_file( vd_t o,  sc_t file );
void bcore_bin_ml_r_from_file( sr_s* o, sc_t file );
void bcore_bin_ml_t_from_file( tp_t t, vd_t o, sc_t file );
void bcore_bin_ml_a_from_source( vd_t o, bcore_source* source );

/**********************************************************************************************************************/
// testing, debugging

/// writes object to stream; interprets stream and returns interpreted object
sr_s bcore_bin_ml_transfer( sr_s obj );

/** Copies aware object via a bin_ml transfer (writes src to binary sink, reads o from source (of sink)
 *  This function can be used to test correct transfer
 */
void bcore_bin_ml_a_copy( vd_t o, vc_t src );

/// converts object into stream; interprets stream and compares result to original
void bcore_bin_ml_transfer_test( sr_s obj );


/**********************************************************************************************************************/

vd_t bcore_bin_ml_signal_handler( const bcore_signal_s* o );

#endif // BCORE_BIN_ML_H

