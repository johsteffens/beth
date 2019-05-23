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

//----------------------------------------------------------------------------------------------------------------------

void badapt_adaptive_arc_to_sink_fallback( const badapt_adaptive* o, bcore_sink* sink )
{
    bcore_txt_ml_a_to_sink( o, sink );
}

//----------------------------------------------------------------------------------------------------------------------

f3_t badapt_adaptive_infer_f3_fallback( const badapt_adaptive* o, const bmath_vf3_s* in )
{
    bmath_vf3_s v_out;
    f3_t out = 0;
    bmath_vf3_s_init_weak( &v_out, &out, 1 );
    badapt_adaptive_a_infer( o, in, &v_out );
    return out;
}

//----------------------------------------------------------------------------------------------------------------------

void badapt_adaptive_adapt_loss_fallback( badapt_adaptive* o, const badapt_loss* loss, const bmath_vf3_s* in, const bmath_vf3_s* target, bmath_vf3_s* out )
{
    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( bmath_vf3_s, grad );
    ASSERT( out != NULL );
    bmath_vf3_s_set_size( grad, out->size );
    badapt_adaptive_a_minfer( o, in, out );
    badapt_loss_a_bgrad( loss, out, target, grad );
    badapt_adaptive_a_bgrad_adapt( o, NULL, grad );
    BCORE_LIFE_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

f3_t badapt_adaptive_adapt_loss_f3_fallback( badapt_adaptive* o, const badapt_loss* loss, const bmath_vf3_s* in, f3_t target )
{
    bmath_vf3_s v_target;
    bmath_vf3_s v_out;
    f3_t out = 0;

    bmath_vf3_s_init_weak( &v_target, &target, 1 );
    bmath_vf3_s_init_weak( &v_out, &out, 1 );
    badapt_adaptive_a_adapt_loss( o, loss, in, &v_target, &v_out );
    return out;
}

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

