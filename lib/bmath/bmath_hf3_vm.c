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

#include "bmath_hf3_vm.h"

/**********************************************************************************************************************/

void bmath_hf3_vm_frame_s_run_proc( bmath_hf3_vm_frame_s* o, tp_t name )
{
    if( !bcore_hmap_tpuz_s_exists( &o->map_proc, name ) )
    {
        ERR_fa( "'#<sc_t>' does not represent a procedure.", bcore_hmap_name_s_get_sc( &o->map_name, name ) );
    }
    sz_t idx = *bcore_hmap_tpuz_s_get( &o->map_proc, name );
    bmath_hf3_vm_proc_s* proc = &o->arr_proc.data[ idx ];
    bmath_hf3_vm_proc_s_run( proc, &o->arr_holor );
}

// ---------------------------------------------------------------------------------------------------------------------

bmath_hf3_s* bmath_hf3_vm_frame_s_get_holor( bmath_hf3_vm_frame_s* o, tp_t name )
{
    if( !bcore_hmap_tpuz_s_exists( &o->map_holor, name ) )
    {
        ERR_fa( "'#<sc_t>' does not represent a holor.", bcore_hmap_name_s_get_sc( &o->map_name, name ) );
    }
    sz_t idx = *bcore_hmap_tpuz_s_get( &o->map_proc, name );
    return &o->arr_holor.data[ idx ];
}

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

vd_t bmath_hf3_vm_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_hf3_vm" ) ) )
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
            return NULL;
        }
        break;

        case TYPEOF_plant:
        {
            bcore_plant_compile( "bmath_planted", __FILE__ );
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

