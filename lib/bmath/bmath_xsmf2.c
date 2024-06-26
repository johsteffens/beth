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

#include "bcore_std.h"
#include "bmath_std.h"
#include "bmath_xsmf2.h"
#include "bmath_xsmf3.h"

#define BMATH_TEMPLATE_FX_PREC 2
#include "bmath_template_xsmfx_c.h"

#define BMATH_TEMPLATE_FX_PREC 2
#include "bmath_template_xsmfx_mul_c.h"

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

vd_t bmath_xsmf2_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_xsmf2" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_FUNC( bmath_xsmf2_s_mul );
            BCORE_REGISTER_FUNC( bmath_xsmf2_s_mul_htp );
            BCORE_REGISTER_FUNC( bmath_xsmf2_s_htp_mul );
            BCORE_REGISTER_FUNC( bmath_xsmf2_s_htp_mul_htp );
        }
        break;

        case TYPEOF_selftest:
        {
            selftest();
            return NULL;
        }
        break;

        default: break;
    }

    return NULL;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/


