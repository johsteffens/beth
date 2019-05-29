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

/** bcore_spect_inst_call is a perspective defining a set of callback functions
 *  called by bcore_inst upon certain low-level actions like init, copy, down.
 */

#ifndef BCORE_SPECT_INST_CALL_H
#define BCORE_SPECT_INST_CALL_H

#include "bcore_precoder.h"
#include "bcore_trait.h"
#include "bcore_precoded.h"

/**********************************************************************************************************************/

#ifdef TYPEOF_bcore_inst_call
BETH_PRECODE( bcore_inst_call )
#ifdef BETH_PRECODE_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    feature 'p' void init_x( mutable ); // called by bcore_inst when exiting initialization
    feature 'p' void down_e( mutable ); // called by bcore_inst when entering shutdown
    feature 'p' void copy_e( mutable ); // called by bcore_inst when entering copy
    feature 'p' void copy_x( mutable ); // called by bcore_inst when exiting copy

    name bcore_inst_call_init_x;
    name bcore_inst_call_down_e;
    name bcore_inst_call_copy_e;
    name bcore_inst_call_copy_x;
#endif // BETH_PRECODE_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif // optional enclosure

/**********************************************************************************************************************/

vd_t bcore_spect_inst_call_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif  // BCORE_SPECT_INST_CALL_H
