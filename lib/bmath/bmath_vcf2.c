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

#include "bmath_vcf2.h"
#include "bmath_spect_vector.h"
#include "bmath_fourier_f2.h"

#define BMATH_TEMPLATE_FX_PREC 2
#include "bmath_template_vcfx_c.h"

/**********************************************************************************************************************/
// bmath_vcf2_s

BCORE_DEFINE_OBJECT_INST( bmath_vector, bmath_vcf2_s )
"{"
    "aware_t _;"
    "bmath_cf2_s [];"
    "func bmath_fp:add;"
    "func bmath_fp:zro;"
    "func bmath_fp:neg;"
    "func bmath_fp:sub;"
    "func bmath_fp:cpy;"
    "func bmath_fp_vector:mul_scl = bmath_vcf2_s_mul_scl;"
    "func bmath_fp_vector:mul_vec = bmath_vcf2_s_mul_vec;"
"}";

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

vd_t bmath_vcf2_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_vcf2" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_FFUNC( bmath_fp_add,            bmath_vcf2_s_add );
            BCORE_REGISTER_FFUNC( bmath_fp_zro,            bmath_vcf2_s_zro );
            BCORE_REGISTER_FFUNC( bmath_fp_neg,            bmath_vcf2_s_neg );
            BCORE_REGISTER_FFUNC( bmath_fp_sub,            bmath_vcf2_s_sub );
            BCORE_REGISTER_FFUNC( bmath_fp_cpy,            bmath_vcf2_s_cpy );
            BCORE_REGISTER_FFUNC( bmath_fp_vector_mul_vec, bmath_vcf2_s_mul_vec );
            BCORE_REGISTER_FFUNC( bmath_fp_vector_mul_scl, bmath_vcf2_s_mul_scl );
            BCORE_REGISTER_OBJECT( bmath_vcf2_s );
        }
        break;

        case TYPEOF_get_quicktypes:
        {
            BCORE_REGISTER_QUICKTYPE( bmath_vcf2_s );
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

