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

#ifndef BCORE_PLANT_COMPILER_H
#define BCORE_PLANT_COMPILER_H

#include "bcore_control.h"
#include "bcore_signal.h"
#include "bcore_flect.h"
#include "bcore_spect_array.h"
#include "bcore_hmap.h"

/**********************************************************************************************************************/
/// Plant interface

/// Compiles plants into target files: directory( file )/target{ .c | .h }
void bcore_plant_compile( sc_t target_name, sc_t source_path );

/// Updates planted files if necessary; returns true in case a file was modified
bl_t bcore_plant_expand();

/** Runs plant compiler globally (compiling, expansion);
 *  Returns true in case a precoded-file was generated or modified.
 */
bl_t bcore_plant_run_globally();

/**********************************************************************************************************************/

vd_t bcore_plant_compiler_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

/// Macros
#define BETH_PRECODE( name ) \
    BETH_EXPAND_GROUP_##name

#define PLANT_GROUP( name, trait ) \
    BETH_EXPAND_GROUP_##name

#endif // BCORE_PLANT_COMPILER_H
