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

#include "bcore.h"
#include "bcore_plant_compiler.h"

//----------------------------------------------------------------------------------------------------------------------

void beth_plant_set_verbosity( sz_t verbosity )
{
    bcore_plant_compiler_set_verbosity( verbosity );
}

//----------------------------------------------------------------------------------------------------------------------

bl_t beth_plant_compile_all( void )
{
    return bcore_plant_compiler_compile_all_registered_plants();
}

//----------------------------------------------------------------------------------------------------------------------

void beth_plant_compile( sc_t name )
{
    bcore_plant_compiler_compile_registered_plant( name );
}

//----------------------------------------------------------------------------------------------------------------------

bl_t beth_plant_update_required( void )
{
    return bcore_plant_compiler_update_required();
}

//----------------------------------------------------------------------------------------------------------------------

bl_t beth_plant_update( void )
{
    return bcore_plant_compiler_update_planted_files();
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

vd_t bcore_plant_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_plant" ) ) )
    {
        case TYPEOF_init1:
        {
        }
        break;

        case TYPEOF_down1:
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

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

