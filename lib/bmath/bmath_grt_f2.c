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

#include "bmath_grt_f2.h"

#define BMATH_TEMPLATE_FX_PREC 2
#include "bmath_template_grt_fx_c.h"

BCORE_DEFINE_OBJECT_FLAT( bcore_inst, bmath_grt_f2_s ) "{ f3_t c; f3_t s; }";

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

BCORE_DEFINE_OBJECT_INST( bcore_inst, bmath_arr_grt_f2_s ) "{ aware_t _; bmath_grt_f2_s [] arr; }";

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

vd_t bmath_grt_f2_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_grt_f2" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_OBJECT( bmath_grt_f2_s );
            BCORE_REGISTER_OBJECT( bmath_arr_grt_f2_s );
        }
        break;

        case TYPEOF_get_quicktypes:
        {
            BCORE_REGISTER_QUICKTYPE( bmath_grt_f2_s );
            BCORE_REGISTER_QUICKTYPE( bmath_arr_grt_f2_s );
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

/**********************************************************************************************************************/

