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

/** Text based markup language for marshaling and creation of objects.
 *  Implements features translate, interpret and supports corresponding perspectives.
 */

#ifndef BCORE_TXT_ML_H
#define BCORE_TXT_ML_H

#include "bcore_flect.h"

/**********************************************************************************************************************/

typedef struct bcore_txt_ml_translator_s
{
    aware_t _;
    uz_t indent; // indentation
} bcore_txt_ml_translator_s;

BCORE_DECLARE_FUNCTION_INIT(    bcore_txt_ml_translator_s )
BCORE_DECLARE_FUNCTION_DOWN(    bcore_txt_ml_translator_s )
BCORE_DECLARE_FUNCTION_COPY(    bcore_txt_ml_translator_s )
BCORE_DECLARE_FUNCTION_CREATE(  bcore_txt_ml_translator_s )
BCORE_DECLARE_FUNCTION_DISCARD( bcore_txt_ml_translator_s )
BCORE_DECLARE_FUNCTION_CLONE(   bcore_txt_ml_translator_s )

void bcore_txt_ml_translator_s_translate( const bcore_txt_ml_translator_s* o, sr_s obj, sr_s snk );

/**********************************************************************************************************************/

/// syntactic sugar
void bcore_txt_ml_x_to_stdout( sr_s obj );
void bcore_txt_ml_x_to_file(   sr_s obj, sc_t file );
void bcore_txt_ml_x_to_string( sr_s obj, st_s* string );

void bcore_txt_ml_r_to_stdout( const sr_s* obj );
void bcore_txt_ml_r_to_file(   const sr_s* obj, sc_t file );
void bcore_txt_ml_r_to_string( const sr_s* obj, st_s* string );

void bcore_txt_ml_a_to_stdout( vc_t o );
void bcore_txt_ml_a_to_file(   vc_t o, sc_t file );
void bcore_txt_ml_a_to_string( vc_t o, st_s* string );

void bcore_txt_ml_t_to_stdout( tp_t t, vc_t o );
void bcore_txt_ml_t_to_file(   tp_t t, vc_t o, sc_t file );
void bcore_txt_ml_t_to_string( tp_t t, vc_t o, st_s* string );

/// sends a default instance of type 't' to sink
void bcore_txt_ml_default_to_stdout( tp_t t );
void bcore_txt_ml_default_to_file(   tp_t t, sc_t file );
void bcore_txt_ml_default_to_string( tp_t t, st_s* string );

/**********************************************************************************************************************/

typedef struct bcore_txt_ml_interpreter_s
{
    aware_t _;
} bcore_txt_ml_interpreter_s;

void                        bcore_txt_ml_interpreter_s_init(            bcore_txt_ml_interpreter_s* o );
void                        bcore_txt_ml_interpreter_s_down(            bcore_txt_ml_interpreter_s* o );
void                        bcore_txt_ml_interpreter_s_copy(            bcore_txt_ml_interpreter_s* o, const bcore_txt_ml_interpreter_s* src );
bcore_txt_ml_interpreter_s* bcore_txt_ml_interpreter_s_create();
bcore_txt_ml_interpreter_s* bcore_txt_ml_interpreter_s_clone(     const bcore_txt_ml_interpreter_s* o );
void                        bcore_txt_ml_interpreter_s_discard(         bcore_txt_ml_interpreter_s* o );

sr_s bcore_txt_ml_interpreter_s_interpret( const bcore_txt_ml_interpreter_s* o, sr_s src );

/**********************************************************************************************************************/

/// syntactic sugar
sr_s bcore_txt_ml_from_file( sc_t file );
sr_s bcore_txt_ml_from_string( const st_s* string );
sr_s bcore_txt_ml_from_sc(           sc_t  sc );

/**********************************************************************************************************************/
// testing, debugging

/// converts object into stream; interprets stream and compares result to original
void bcore_txt_ml_transfer_test( sr_s obj );

/**********************************************************************************************************************/

vd_t bcore_txt_ml_signal_handler( const bcore_signal_s* o );

#endif // BCORE_TXT_ML_H

