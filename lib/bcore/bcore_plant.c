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

#include "bcore_plant.h"
#include "bcore_plant_compiler.h"

//----------------------------------------------------------------------------------------------------------------------

void beth_plant_build_from_file( sc_t path )
{
    bcore_plant_builder_build_from_file( path );
}

//----------------------------------------------------------------------------------------------------------------------

void beth_plant_build_from_rel_file( sc_t root_path, sc_t path )
{
    bcore_plant_builder_build_from_rel_file( root_path, path );
}

//----------------------------------------------------------------------------------------------------------------------

bl_t beth_plant_update_required( void )
{
    return bcore_plant_builder_update_required();
}

//----------------------------------------------------------------------------------------------------------------------

bl_t beth_plant_update( void )
{
    return bcore_plant_builder_update();
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
