/** Author and Copyright 2020 Johannes Bernhard Steffens
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

#ifndef BHPT_FRAME_H
#define BHPT_FRAME_H

/**********************************************************************************************************************/

/** Global Interface Sketch of this Library*/

/**********************************************************************************************************************/

#include "bmath_std.h"
#include "bhvm_std.h"
#include "bhpt_sketch.h"
#include "bhpt_planted.h"

/**********************************************************************************************************************/

#ifdef TYPEOF_bhpt_frame

PLANT_GROUP( bhpt_frame, bcore_inst )
#ifdef PLANT_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// Adaptive framework
stamp :state = aware :
{
    sz_t cycle_number;
    sz_t last_cycle_adapt;
    sz_t last_cycle_test;
    sz_t last_cycle_backup;
    aware bhpt_adaptive => adaptive;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// Adaptive framework
stamp : = aware bcore_main
{
    /// ========= parameters ================
    aware bhpt_tutor   => tutor;
    aware bhpt_builder => builder;
    aware bhpt_adaptor => adaptor;

    // A single single cycle represents an adaptive prime

    // cycle number at which the adaptor is applied (mini-batch size)
    sz_t cycle_adapt  = 1;

    // cycle number at which the network is tested
    sz_t cycle_test   = 1000;

    // cycle number at which a new backup is generated
    sz_t cycle_backup = 1000;

    // cycle number at which training ends
    sz_t cycle_finish = 1000000;

    // log verbosity
    sz_t verbosity = 1;
    /// ========= end parameters  ==========

    /// current state of frame
    hidden :state_s => state;
    hidden bhpt_adaptor_adl_s => adaptor_adl;

    st_s state_path; // backup file for state

    hidden aware bcore_sink -> log;

    func bcore_main : main;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // PLANT_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // TYPEOF_bhpt_frame

/**********************************************************************************************************************/

vd_t bhpt_frame_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BHPT_FRAME_H
