/** Copyright 2019 Johannes Bernhard Steffens
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

#include "badapt_loss.h"

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

f3_t badapt_loss_l2_s_loss( const badapt_loss_l2_s* o, const bmath_vf3_s* out, const bmath_vf3_s* target )
{
    return bmath_vf3_s_f3_sub_sqr( target, out );
}

//----------------------------------------------------------------------------------------------------------------------

f3_t badapt_loss_l2_s_loss_f3( const badapt_loss_l2_s* o, f3_t out, f3_t target )
{
    return f3_sqr( target - out );
}

//----------------------------------------------------------------------------------------------------------------------

void badapt_loss_l2_s_bgrad( const badapt_loss_l2_s* o, const bmath_vf3_s* out, const bmath_vf3_s* target, bmath_vf3_s* grad )
{
    bmath_vf3_s_sub( target, out, grad );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

vd_t badapt_loss_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "badapt_loss" ) ) )
    {
        case TYPEOF_init1:
        {
        }
        break;

        case TYPEOF_selftest:
        {
            return NULL;
        }
        break;

        case TYPEOF_precoder:
        {
            bcore_precoder_compile( "badapt_precoded", __FILE__ );
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

