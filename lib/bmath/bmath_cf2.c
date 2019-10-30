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

#include "bmath_cf2.h"
#include "bmath_cf3.h"

#define BMATH_TEMPLATE_FX_PREC 2
#include "bmath_template_cfx_c.h"

/**********************************************************************************************************************/

BCORE_DEFINE_OBJECT_FLAT( bmath_ring, bmath_cf2_s )
"{"
    "f2_t [2] v;"
    "func bmath_fp:is_equ;"
    "func bmath_fp:is_zro;"
    "func bmath_fp:is_one;"

    "func bmath_fp:add;"
    "func bmath_fp:zro;"
    "func bmath_fp:neg;"
    "func bmath_fp:sub;"
    "func bmath_fp:mul;"
    "func bmath_fp:one;"
    "func bmath_fp:inv;"
    "func bmath_fp:div;"

    "func bcore_fp_copy_typed = bmath_cf2_s_copy_typed;"
"}";

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

vd_t bmath_cf2_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_cf2" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_FFUNC( bcore_fp_copy_typed, bmath_cf2_s_copy_typed );
            BCORE_REGISTER_OBJECT( bmath_cf2_s );
            BCORE_REGISTER_FFUNC( bmath_fp_is_equ, bmath_cf2_s_is_equ );
            BCORE_REGISTER_FFUNC( bmath_fp_is_zro, bmath_cf2_s_is_zro );
            BCORE_REGISTER_FFUNC( bmath_fp_is_one, bmath_cf2_s_is_one );

            BCORE_REGISTER_FFUNC( bmath_fp_add, bmath_cf2_s_add );
            BCORE_REGISTER_FFUNC( bmath_fp_zro, bmath_cf2_s_zro );
            BCORE_REGISTER_FFUNC( bmath_fp_neg, bmath_cf2_s_neg );
            BCORE_REGISTER_FFUNC( bmath_fp_sub, bmath_cf2_s_sub );
            BCORE_REGISTER_FFUNC( bmath_fp_mul, bmath_cf2_s_mul );
            BCORE_REGISTER_FFUNC( bmath_fp_one, bmath_cf2_s_one );
            BCORE_REGISTER_FFUNC( bmath_fp_inv, bmath_cf2_s_inv );
            BCORE_REGISTER_FFUNC( bmath_fp_div, bmath_cf2_s_div );
        }
        break;

        case TYPEOF_get_quicktypes:
        {
            BCORE_REGISTER_QUICKTYPE( bmath_cf2_s );
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

