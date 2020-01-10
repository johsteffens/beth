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

#include "bhvm_vop.h"

#ifdef TYPEOF_bhvm_vop

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_vop_a_to_sink( bhvm_vop* o, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "#p28.{#<sc_t> } ", ifnameof( o->_ ) );
    sz_t arity = bhvm_vop_a_arity( o );
    bcore_sink_a_push_fa( sink, "#pl3 {#<sz_t>}", bhvm_vop_a_get_index( o, arity ) );
    for( sz_t i = 0; i < arity; i++ )
    {
        bcore_sink_a_push_fa( sink, ( i == 0 ) ? " <- " : "," );
        bcore_sink_a_push_fa( sink, "#pl3 {#<sz_t>}", bhvm_vop_a_get_index( o, i ) );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

#endif // TYPEOF_bhvm_vop

vd_t bhvm_vop_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bhvm_vop" ) ) )
    {
        case TYPEOF_init1:
        {
        }
        break;

        case TYPEOF_get_quicktypes:
        {
        }
        break;

        case TYPEOF_selftest:
        {
        }
        break;

        case TYPEOF_plant:
        {
            bcore_plant_compile( "bhvm_planted_vop", __FILE__ );
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

