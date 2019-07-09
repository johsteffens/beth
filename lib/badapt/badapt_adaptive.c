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

#include "badapt_adaptive.h"

/**********************************************************************************************************************/
// badapt_dynamics

void badapt_dynamics_std_s_weights_adapt( const badapt_dynamics_std_s* o, const bmath_vf3_s* in, bmath_mf3_s* w, const bmath_vf3_s* grad_out, f3_t epsilon_factor )
{
    assert( w->rows == grad_out->size );
    assert( w->cols == in->size );

    f3_t epsilon = o->epsilon * epsilon_factor;
    f3_t l2_reg_factor = ( 1.0 - o->lambda_l2  * epsilon );
    f3_t l1_reg_offset = o->lambda_l1 * epsilon;

    for( sz_t i = 0; i < w->rows; i++ )
    {
        f3_t* wr = w->data + i * w->stride;
        f3_t gi = epsilon * grad_out->data[ i ];
        for( sz_t j = 0; j < w->cols; j++ ) wr[ j ] = ( wr[ j ] + in->data[ j ] * gi ) * l2_reg_factor;
        if( l1_reg_offset > 0 )
        {
            for( sz_t j = 0; j < w->cols; j++ ) wr[ j ] += ( wr[ j ] > 0 ) ? -l1_reg_offset : l1_reg_offset;
        }
    }
    assert( !bmath_mf3_s_is_nan( w ) );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

vd_t badapt_adaptive_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "badapt_adaptive" ) ) )
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

        case TYPEOF_plant:
        {
            bcore_plant_compile( "badapt_planted", __FILE__ );
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

