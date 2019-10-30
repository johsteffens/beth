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

#include <stdio.h>

#include "bmath_vf2.h"
#include "bmath_vf3.h"

#define BMATH_TEMPLATE_FX_PREC 2
#include "bmath_template_vfx_c.h"

#include "bmath_spect_vector.h"

/**********************************************************************************************************************/

BCORE_DEFINE_OBJECT_INST_AUT( bmath_vector, bmath_vf2_s )
"{"
    "aware_t _;"
    "f2_t [];"
    "func bmath_fp:add;"
    "func bmath_fp:zro;"
    "func bmath_fp:neg;"
    "func bmath_fp:sub;"
    "func bmath_fp:cpy;"
    "func bmath_fp_vector:mul_scl = bmath_vf2_s_mul_scl;"
    "func bmath_fp_vector:mul_vec = bmath_vf2_s_mul_vec;"
    "func bcore_fp_copy_typed     = bmath_vf2_s_copy_typed;"
"}";

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

vd_t bmath_vf2_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_vf2" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_FFUNC( bmath_fp_add,            bmath_vf2_s_add );
            BCORE_REGISTER_FFUNC( bmath_fp_zro,            bmath_vf2_s_zro );
            BCORE_REGISTER_FFUNC( bmath_fp_neg,            bmath_vf2_s_neg );
            BCORE_REGISTER_FFUNC( bmath_fp_sub,            bmath_vf2_s_sub );
            BCORE_REGISTER_FFUNC( bmath_fp_cpy,            bmath_vf2_s_cpy );
            BCORE_REGISTER_FFUNC( bmath_fp_vector_mul_vec, bmath_vf2_s_mul_vec );
            BCORE_REGISTER_FFUNC( bmath_fp_vector_mul_scl, bmath_vf2_s_mul_scl );
            BCORE_REGISTER_FFUNC( bcore_fp_copy_typed,     bmath_vf2_s_copy_typed );
            BCORE_REGISTER_OBJECT( bmath_vf2_s );
        }
        break;

        case TYPEOF_get_quicktypes:
        {
            BCORE_REGISTER_QUICKTYPE( bmath_vf2_s );
        }
        break;

        case TYPEOF_selftest:
        {
            selftest();
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

