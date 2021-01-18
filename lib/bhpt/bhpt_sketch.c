/** Author and Copyright 2020 Johannes Bernhard Steffens
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

#include "bhpt_sketch.h"

#ifdef TYPEOF_bhpt

/**********************************************************************************************************************/
/// bhpt_adaptor

// ---------------------------------------------------------------------------------------------------------------------

void bhpt_adaptor_node_s_get_min_max( const bhpt_adaptor_node_s* o, f3_t* min, f3_t* max )
{
    if( o->axon )
    {
        if( min ) *min = bhvm_value_s_get_min_f3( &o->axon->v );
        if( max ) *max = bhvm_value_s_get_max_f3( &o->axon->v );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhpt_adaptor_probe_s_get_min_max( const bhpt_adaptor_probe_s* o, f3_t* min, f3_t* max )
{
    f3_t min_l = 0, max_l = 0;
    BFOR_EACH( i, o )
    {
        bhpt_adaptor_node_s_get_min_max( &o->data[ i ], &min_l, &max_l );
        if( min ) *min = ( i > 0 ) ? f3_min( *min, min_l ) : min_l;
        if( max ) *max = ( i > 0 ) ? f3_max( *max, max_l ) : max_l;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhpt_adaptor_node_s_acc_stats( const bhpt_adaptor_node_s* o, bhvm_stats_s* axon, bhvm_stats_s* grad )
{
    if( axon && o->axon ) bhvm_value_s_stats_acc( &o->axon->v, axon );
    if( grad && o->grad ) bhvm_value_s_stats_acc( &o->grad->v, grad );
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

#endif // TYPEOF_bhpt

/**********************************************************************************************************************/

