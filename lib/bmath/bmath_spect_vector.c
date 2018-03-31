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

#include "bmath_spect_vector.h"
#include "bmath_quicktypes.h"

/**********************************************************************************************************************/

sz_t bmath_vector_spect_get_dim( const bmath_vector_s* p, vc_t o )
{
    return bcore_array_spect_get_size( p->array, o );
}

void bmath_vector_spect_add( const bmath_vector_s* p, vc_t o, vc_t op, vd_t result )
{
    sz_t dim = bmath_vector_spect_get_dim( p, o );
    ASSERT( dim == bmath_vector_spect_get_dim( p, op ) );
    ASSERT( dim == bmath_vector_spect_get_dim( p, result ) );
    for( sz_t i = 0; i < dim; i++ )
    {
        sr_s sr_o   = bcore_array_spect_get( p->array, o, i );
        sr_s sr_op  = bcore_array_spect_get( p->array, op, i );
        sr_s sr_res = bcore_array_spect_get( p->array, result, i );
        bmath_ring_spect_add( p->ring, sr_res.o, sr_o.o, sr_op.o );
        sr_down( sr_o );
        sr_down( sr_op );
        sr_down( sr_res );
    }
}

void bmath_vector_spect_sub(     const bmath_vector_s* p, vc_t o, vc_t op, vd_t result );
void bmath_vector_spect_dot_prd( const bmath_vector_s* p, vc_t o, vc_t op, vd_t result );

/**********************************************************************************************************************/

vd_t bmath_spect_vector_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_spect_vector" ) ) )
    {
        case TYPEOF_init1:
        {
            // features
        }
        break;

        case TYPEOF_selftest:
        {
//            return selftest();
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

