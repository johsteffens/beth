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

/**********************************************************************************************************************/

BETH_PRECODE( badapt_loss )
#ifdef BETH_PRECODE_SECTION
    feature strict 'pa' f3_t loss(    const, const bmath_vf3_s* out, const bmath_vf3_s* target );                    // loss function
    feature strict 'pa' f3_t loss_f3( const, f3_t               out, f3_t               target );                    // loss function on scalars
    feature strict 'pa' void bgrad(   const, const bmath_vf3_s* out, const bmath_vf3_s* target, bmath_vf3_s* grad ); // computes loss minimizing backward gradient of x

    // l2 loss function
    self :l2_s = aware badapt_loss { func :loss; func :loss_f3; func :bgrad; };
#endif // BETH_PRECODE_SECTION

/**********************************************************************************************************************/

vd_t badapt_loss_signal_handler( const bcore_signal_s* o );

#endif  // BADAPT_LOSS_H

