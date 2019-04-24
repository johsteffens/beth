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

#include "bmath_activation.h"

/**********************************************************************************************************************/

f3_t bmath_activation_tanh_s_fx( const bmath_activation_tanh_s* o, f3_t x ) { return 1.0 - ( 2.0 / ( exp( 2.0 * x ) + 1.0 ) ); }
f3_t bmath_activation_tanh_s_dy( const bmath_activation_tanh_s* o, f3_t y ) { return 1.0 - f3_sqr( y ); }

f3_t bmath_activation_relu_s_fx( const bmath_activation_relu_s* o, f3_t x ) { return x > 0 ? x : 0; }
f3_t bmath_activation_relu_s_dy( const bmath_activation_relu_s* o, f3_t y ) { return y > 0 ? 1 : 0; }

f3_t bmath_activation_leaky_relu_s_fx( const bmath_activation_leaky_relu_s* o, f3_t x ) { return x > 0 ? x : x * 0.01; }
f3_t bmath_activation_leaky_relu_s_dy( const bmath_activation_leaky_relu_s* o, f3_t y ) { return y > 0 ? 1 : 0.01; }

f3_t bmath_activation_softplus_s_fx( const bmath_activation_softplus_s* o, f3_t x ) { return log( 1.0 + exp( x ) ); }
f3_t bmath_activation_softplus_s_dy( const bmath_activation_softplus_s* o, f3_t y ) { f3_t u = exp( y ); return ( u - 1.0 ) / u; }

/**********************************************************************************************************************/

vd_t bmath_activation_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_activation" ) ) )
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
            bcore_precoder_compile( "bmath_precoded", __FILE__ );
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

