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

/** The interpreter-perspective provides the interface for object de-serialization, */

#ifndef BCORE_SPECT_INTERPRETER_H
#define BCORE_SPECT_INTERPRETER_H

#include "bcore_flect.h"
#include "bcore_features.h"

typedef struct bcore_interpreter_s
{
    aware_t p_type; // perspective
    tp_t    o_type; // interpreter
    bcore_fp_interpret fp_interpret;
} bcore_interpreter_s;

const bcore_interpreter_s* bcore_interpreter_s_get_typed( tp_t type );

sr_s bcore_interpret( sr_s o, sr_s source );
sr_s bcore_interpret_q( const sr_s* o, sr_s source );

/** Automatically picks the right interpreter according to source content.
 *  In text or binary files:
 *  The first object is deemed to be the interpreter in the default format of bcore_txt_ml
 *  Subsequent data is interpreted by the specified interpreter (either binary or ascii)
 *
 *  Example:
 *  The stream may begin with "<bcore_bin_ml_interpreter_s></>".
 *  Data following this header will be interpreted in the syntax of bcore_bin_ml_interpreter_s
 */
sr_s bcore_interpret_auto( sr_s source );
sr_s bcore_interpret_auto_file( sc_t file );

vd_t bcore_spect_interpreter_signal_handler( const bcore_signal_s* o );

#endif  // BCORE_SPECT_INTERPRETER_H

