/** Author and Copyright 2019 Johannes Bernhard Steffens
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

#include "bhvm_signal.h"
#include "bhvm_planted.h"
#include "bhvm_hf3.h"
#include "bhvm_hf3_op.h"

vd_t bhvm_signal_handler( const bcore_signal_s* o )
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
            bhvm_planted_signal_handler,
            bhvm_hf3_signal_handler,
            bhvm_hf3_op_signal_handler,
        };

        ret = bcore_signal_s_broadcast( o, arr, sizeof( arr ) / sizeof( bcore_fp_signal_handler ) );
    }

    return ret;
}

/**********************************************************************************************************************/
