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
#include "bmath_asmf2.h"
#include "bmath_asmf3.h"

#define BMATH_TEMPLATE_FX_PREC 3
#include "bmath_template_asmfx_c.h"

#define BMATH_TEMPLATE_FX_PREC 3
#include "bmath_template_asmfx_mul_c.h"

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

vd_t bmath_asmf3_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_asmf3" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_FUNC( bmath_asmf3_s_mul );
            BCORE_REGISTER_FUNC( bmath_asmf3_s_mul_htp );
            BCORE_REGISTER_FUNC( bmath_asmf3_s_htp_mul );
            BCORE_REGISTER_FUNC( bmath_asmf3_s_htp_mul_htp );
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


