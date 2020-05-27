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

/// beth-plant interface functions and macros

#ifndef BCORE_PLANT_BUILDER_H
#define BCORE_PLANT_BUILDER_H

#include "bcore_plant_compiler.h"
#include "bcore_trait.h"
#include "bcore_life.h"
#include "bcore_file.h"
#include "bcore_spect_source.h"
#include "bcore_txt_ml.h"
#include "bcore_planted.h"

/**********************************************************************************************************************/

#ifdef BETH_EXPAND_GROUP_bcore_plant_builder

PLANT_GROUP( bcore_plant_builder, bcore_inst )
#ifdef PLANT_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// returns index of target
feature 'a' sz_t build( const );

stamp :target = aware :
{
    st_s => name;                // target name
    st_s => root;                // root folder of subsequent file paths (used if they are relative)
    bcore_arr_st_s dependencies; // dependent target definitions
    bcore_arr_st_s sources;      // array of source files

    /** Function name of principal signal handler for this plant
     * If not defined, it is assumed that the name if <name>_general_signal_handler
     */
    st_s => signal_handler;

    func bcore_via_call : source =
    {
        if( !o->root )
        {
            o->root = bcore_file_folder_path( bcore_source_a_get_file( source ) );
            st_s_attach( &o->root, bcore_file_path_minimized( o->root->sc ) );
        }
    };

    func : : build;
};

stamp :target_adl = aware bcore_array { :target_s => []; };

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // PLANT_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**********************************************************************************************************************/
// bcore_plant_builder interface functions

/// build plant from configuration file (thread safe)
void bcore_plant_builder_build_from_file( sc_t path );

/** Builds from configuration file given in path.
 *  If path is relative, the root folder is take from root_path
 *  where root path specifies a file or folder inside the root path.
 *  Example:
 *     bcore_plant_builder_build_from_rel_file( __FILE__, "beth_plant.cfg" );
 *     --> beth_plant.cfg is assumed to be in the folder of beth_plant.cfg
 */
void bcore_plant_builder_build_from_rel_file( sc_t root_path, sc_t path );

/// Checks if compiled plants require an update of the corresponding panted files (thread safe)
bl_t bcore_plant_builder_update_required( void );

/// Updates all planted files that require an update; returns true if any file was modified. (thread safe)
bl_t bcore_plant_builder_update( void );

#endif // BETH_EXPAND_GROUP_bcore_plant

//---------------------------------------------------------------------------------------------------------------------

vd_t bcore_plant_builder_signal_handler( const bcore_signal_s* o );

#endif // BCORE_PLANT_BUILDER_H
