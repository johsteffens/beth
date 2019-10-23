/** Author and Copyright 2018 Johannes Bernhard Steffens
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

#include "bmath_feature.h"
#include "bcore_signal.h"
#include "bcore_flect.h"

/**********************************************************************************************************************/

vd_t bmath_feature_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_feature" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_FEATURE( bmath_fp_u2_argb_from_f3 );
        }
        break;

        case TYPEOF_selftest:
        {
        }
        break;

        default: break;
    }
    return NULL;
}
