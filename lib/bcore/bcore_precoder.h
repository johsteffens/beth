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
 *  Automatic code is generated for a folder of code (e.g. representing a library)
 *  A folder is identified by a prefix (typically the folder name)
 *
 *  *_precoded.{c,h}
 *  A folder supporting precoder contains two files: 'prefix'_precoded.h and 'prefix'_precoded.c
 *  These file are solely created/maintained by the precoder framework and not intended for
 *  manual editing. *_precoded.c implements a signal handler which must be added to the signal dispatcher
 *  of the library.
 *
 *  BCORE_PRECODER_SIGNAL_HANDLER
 *
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
void bcore_precoder_compile( const char* file, const char* target_name );

/// updates precoded files if necessary; returns true in case a file was modified
bl_t bcore_precoder_finalize();

/// global run of precoder (compiling, finalizing); returns true in case a 'decode-file' was modified
bl_t bcore_precoder_run_globally();

/**********************************************************************************************************************/

vd_t bcore_precoder_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/
/// Macros

#endif  // BCORE_PRECODER_H
