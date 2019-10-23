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

#include <stdio.h>

#include "bmath_arr_vf2.h"

#define BMATH_TEMPLATE_FX_PREC 2
#include "bmath_template_arr_vfx_c.h"

/**********************************************************************************************************************/

BCORE_DEFINE_OBJECT_INST( bcore_array, bmath_arr_vf2_s )
"{"
    "aware_t _;"
    "bmath_vf2_s [] arr;"
"}";

/**********************************************************************************************************************/

vd_t bmath_arr_vf2_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_arr_vf2" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_OBJECT( bmath_arr_vf2_s );
        }
        break;

        case TYPEOF_get_quicktypes:
        {
            BCORE_REGISTER_QUICKTYPE( bmath_arr_vf2_s );
        }
        break;

        case TYPEOF_selftest:
        {
            return selftest();
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

