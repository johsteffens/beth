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

#include "badapt_signal.h"
#include "badapt_precoded.h"
#include "badapt_features.h"
#include "badapt_adaptive.h"
#include "badapt_problem.h"
#include "badapt_activator.h"
#include "badapt_loss.h"

#include "badapt_mlp.h"
#include "badapt_c1d.h"

#include "badapt_training.h"
#include "badapt_trainer.h"

vd_t badapt_signal_handler( const bcore_signal_s* o )
{
    vd_t ret = NULL;

    // non-local targets
    if( o->target != TYPEOF_local )
    {
        /// nothing yet
    }

    if( ret ) return ret;

    // local targets
    {
        bcore_fp_signal_handler arr[] =
        {
            badapt_precoded_signal_handler,
            badapt_features_signal_handler,
            badapt_adaptive_signal_handler,
            badapt_problem_signal_handler,
            badapt_activator_signal_handler,
            badapt_loss_signal_handler,

            badapt_mlp_signal_handler,
            badapt_c1d_signal_handler,

            badapt_training_signal_handler,
            badapt_trainer_signal_handler,
        };

        ret = bcore_signal_s_broadcast( o, arr, sizeof( arr ) / sizeof( bcore_fp_signal_handler ) );
    }

    return ret;
}

/**********************************************************************************************************************/
