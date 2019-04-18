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

/** Precoder Framework (Automatic code generation)
 *
 *  Precoder translates beth-precode source code into c-source code, which is
 *  written to dedicated precoded-files, which can be included and compiled with
 *  other c-source code.
 *
 *  A beth-source participates in precoding when its signal-handler executes upon
 *  TYPEOF_precoder the function bcore_precoder_compile( <precoded-file>, __FILE__ );
 *
 *  <precoded-file> is the name of the output (precoded) file given without path or extension.
 *  <precoded-file>.c and <precoded-file>.h are generated in the folder of the beth-source.
 *
 *  During development, bl_t bcore_precoder_run_globally() should be executed.
 *  If it returns 'true' a rebuild is required.
 */

#ifndef BCORE_PRECODER_H
#define BCORE_PRECODER_H

#include "bcore_control.h"
#include "bcore_signal.h"
#include "bcore_flect.h"
#include "bcore_spect_array.h"
#include "bcore_hmap.h"

/**********************************************************************************************************************/

/// compiles precode into target files: directory( file )/target{ .c | .h }
void bcore_precoder_compile( sc_t target_name, sc_t source_path );

/// updates precoded files if necessary; returns true in case a file was modified
bl_t bcore_precoder_expand();

/** Runs precoder globally (compiling, expansion);
 *  Returns true in case a precoded-file was generated or modified.
 */
bl_t bcore_precoder_run_globally();

/**********************************************************************************************************************/

vd_t bcore_precoder_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/
/// Macros
#define BETH_PRECODE( name ) \
    BETH_EXPAND_GROUP_##name

#endif  // BCORE_PRECODER_H
