/** Copyright 2018 Johannes Bernhard Steffens
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

#include "bmath_complex.h"

/**********************************************************************************************************************/

BCORE_DEFINE_OBJECT_FLAT_( bmath_ring, bmath_cf3_s )
"{"
    "f3_t [2] v;"
    "func bmath_fp:add;"
    "func bmath_fp:zro;"
    "func bmath_fp:neg;"
    "func bmath_fp:sub;"
    "func bmath_fp:mul;"
    "func bmath_fp:one;"
    "func bmath_fp:inv;"
    "func bmath_fp:div;"
"}";

/**********************************************************************************************************************/

static vd_t selftest( void )
{
    bmath_cf3_s c1 = { .v[0] = 1, .v[1] = 2 };
    bmath_cf3_s c2 = { .v[0] = 3, .v[1] = 4 };
    bmath_cf3_s c3 = { .v[0] = 0, .v[1] = 0 };
    bmath_cf3_s c4 = { .v[0] = 0, .v[1] = 0 };


//    bcore_txt_ml_to_stdout( sr_twc( TYPEOF_bmath_cf3_s, &c3 ) );
    bmath_cf3_s_div( &c1, &c2, &c3 );
    bmath_ring_t_div( TYPEOF_bmath_cf3_s, (bmath_ring*)&c1, (bmath_ring*)&c2, (bmath_ring*)&c4 );
    ASSERT( bcore_compare_typed( TYPEOF_bmath_cf3_s, &c3, &c4 ) == 0 );

    return NULL;
}

/**********************************************************************************************************************/

vd_t bmath_complex_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_complex" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_OBJECT( bmath_cf3_s );
            BCORE_REGISTER_FFUNC( bmath_fp_add, bmath_cf3_s_add );
            BCORE_REGISTER_FFUNC( bmath_fp_zro, bmath_cf3_s_zro );
            BCORE_REGISTER_FFUNC( bmath_fp_neg, bmath_cf3_s_neg );
            BCORE_REGISTER_FFUNC( bmath_fp_sub, bmath_cf3_s_sub );
            BCORE_REGISTER_FFUNC( bmath_fp_mul, bmath_cf3_s_mul );
            BCORE_REGISTER_FFUNC( bmath_fp_one, bmath_cf3_s_one );
            BCORE_REGISTER_FFUNC( bmath_fp_inv, bmath_cf3_s_inv );
            BCORE_REGISTER_FFUNC( bmath_fp_div, bmath_cf3_s_div );
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
