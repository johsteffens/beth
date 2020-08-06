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

#ifndef BADAPT_LOSS_H
#define BADAPT_LOSS_H

#include "bmath_std.h"
#include "badapt_xoi_out.h"

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( badapt_loss, bcore_inst )
#ifdef XOILA_SECTION

    // loss function
    feature strict 'pa' f3_t loss( const, const bmath_vf3_s* out, const bmath_vf3_s* target );

    // loss function on scalars
    feature strict 'pa' f3_t loss_f3( const, f3_t out, f3_t target );

    // computes loss minimizing backward gradient of x
    feature strict 'pa' void bgrad( const, const bmath_vf3_s* out, const bmath_vf3_s* target, bmath_vf3_s* grad );

    // l2 loss function
    stamp :l2 = aware badapt_loss
    {
        func badapt_loss:loss    = { return bmath_vf3_s_f3_sub_sqr( target, out ); };
        func badapt_loss:loss_f3 = { return f3_sqr( target - out ); };
        func badapt_loss:bgrad   = { bmath_vf3_s_sub( target, out, grad ); };
    };

    // logistic loss function
    stamp :log = aware badapt_loss
    {
        func badapt_loss:loss =
        {
            assert( target->size == out->size );
            f3_t sum = 0;
            for( sz_t i = 0; i < target->size; i++ )
            {
                sum += log( 1.0 + exp( -target->data[ i ] * out->data[ i ] ) );
            }
            return sum;
        };

        func badapt_loss:loss_f3 =
        {
            return log( 1.0 + exp( -target * out ) );
        };

        func badapt_loss:bgrad =
        {
            assert( target->size == out ->size );
            assert( target->size == grad->size );
            for( sz_t i = 0; i < target->size; i++ )
            {
                f3_t v_t = target->data[ i ];
                f3_t v_o = out->data[ i ];
                grad->data[ i ] = v_t / ( 1.0 + exp( v_t * v_o ) );
            }
        };
    };

#endif // XOILA_SECTION

/**********************************************************************************************************************/

vd_t badapt_loss_signal_handler( const bcore_signal_s* o );

#endif  // BADAPT_LOSS_H

