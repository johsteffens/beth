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

#ifndef BADAPT_LOSS_H
#define BADAPT_LOSS_H

#include "bmath_std.h"
#include "badapt_precoded.h"
#include "badapt_features.h"

/**********************************************************************************************************************/

BETH_PRECODE( badapt_loss_objects )
#ifdef BETH_PRECODE_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

self badapt_loss_l2_s = badapt_loss
{
    aware_t _;
    func badapt_loss : loss;
    func badapt_loss : loss_f3;
    func badapt_loss : bgrad;
};

#endif // BETH_PRECODE_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


/**********************************************************************************************************************/

vd_t badapt_loss_signal_handler( const bcore_signal_s* o );

#endif  // BADAPT_LOSS_H

