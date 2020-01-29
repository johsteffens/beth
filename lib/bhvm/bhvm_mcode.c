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

#include "bhvm_mcode.h"

#ifdef TYPEOF_bhvm_mcode

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_mcode_track_s_check_index( const bhvm_mcode_track_s* o, sz_t hbase_size )
{
    BFOR_EACH( i, o )
    {
        const bhvm_vop* vop = o->data[ i ].vop;
        sz_t arity = bhvm_vop_a_arity( vop );
        for( sz_t i = 0; i <= arity; i++ )
        {
            sz_t idx = bhvm_vop_a_get_index( vop, i );
            if( idx <  0 || idx >= hbase_size )
            {
                ERR_fa( "vop '#<sc_t>' index #<sz_t> of value #<sz_t> is out of range [0, #<sz_t>].\n", ifnameof( vop->_ ), i, idx, hbase_size - 1 );
            }
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_mcode_frame_s_check_integrity( const bhvm_mcode_frame_s* o )
{
    if( !o->lib ) return;
    if( o->lib && !o->hbase ) ERR_fa( "hbase missing" );

    BFOR_EACH( i, &o->lib->arr )
    {
        const bhvm_mcode_track_s* track = o->lib->arr.data[ i ];
        bhvm_mcode_track_s_check_index( track, o->hbase->holor_ads.size );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

#endif // TYPEOF_bhvm_mcode

vd_t bhvm_mcode_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bhvm_mcode" ) ) )
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
            bcore_plant_compile( "bhvm_planted", __FILE__ );
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

