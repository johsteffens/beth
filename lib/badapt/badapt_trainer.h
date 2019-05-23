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

#ifndef BADAPT_TRAINER_H
#define BADAPT_TRAINER_H

#include "bmath_std.h"
#include "badapt_training.h"

/**********************************************************************************************************************/

BETH_PRECODE( badapt_trainer_objects )
#ifdef BETH_PRECODE_SECTION


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// trainer_batch

self badapt_trainer_batch_s = badapt_trainer
{
    aware_t _;

    // loss function (optional; overrides preferred loss of supplier)
    aware badapt_loss => loss;

    // ~~~ training parameters ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    sz_t batch_size                 = 10000;
    sz_t batch_cycles_per_fetch     = 1;
    sz_t fetch_cycles_per_iteration = 4;
    sz_t valid_size                 = 10000; // validation happens after each iteration

    /// stopping criteria
    sz_t max_iterations = 10;

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    func badapt_trainer : run;
    func badapt_trainer : create_state;

};

self badapt_sample_batch_s     = bcore_inst  { bmath_vf3_s in; bmath_vf3_s out; };
self badapt_arr_sample_batch_s = bcore_array { aware_t _; badapt_sample_batch_s [] arr; };

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // BETH_PRECODE_SECTION

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

vd_t badapt_trainer_signal_handler( const bcore_signal_s* o );

#endif  // BADAPT_TRAINER_H

