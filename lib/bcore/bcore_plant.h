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

#ifndef BCORE_PLANT_H
#define BCORE_PLANT_H

#include "bcore_plant_builder.h"

/**********************************************************************************************************************/
/// Beth-Plant Interface Functions

/// build plant from configuration file (thread safe)
er_t beth_plant_build_from_file( sc_t path );

/** Builds from configuration file given in path.
 *  If path is relative, the root folder is take from root_path
 *  where root path specifies a file or folder inside the root path.
 *  Example:
 *     beth_plant_build_from_rel_file( __FILE__, "beth_plant.cfg" );
 *     --> beth_plant.cfg is assumed to be in the folder of beth_plant.cfg
 */
er_t beth_plant_build_from_rel_file( sc_t root_path, sc_t path );

/// Checks if compiled plants require an update of the corresponding panted files (thread safe)
bl_t beth_plant_update_required( void );

/// Updates all planted files that require an update; returns true if any file was modified. (thread safe)
er_t beth_plant_update( bl_t* modified );

/**********************************************************************************************************************/

/// Macros

///--------------------------------------------------------------------------------------------------------------------
/// Dedicated Plant Setup Framework

/** Opening a plant section by defining a group in the header file.
 *  This macro also defines the signal_handler
 */
#define BETH_PLANT_DEFINE_GROUP( group_name, trait ) \
    PLANT_GROUP( group_name, trait ) \

/// Declares general signal handler of *.signal.c
#define BETH_PLANT_DECLARE_GENERAL_SIGNAL_HANDLER( plant_name ) \
    vd_t plant_name##_general_signal_handler( const bcore_signal_s* o );

/// Inside *signal.c: Begin of a plant registry for a specific plane name
#define BETH_PLANT_SIGNAL_OPEN_PLANT( plant_name ) \
    vd_t plant_name##_general_signal_handler( const bcore_signal_s* o ) \
    { \
        BLM_INIT(); \
        bcore_arr_fp_s* arr_fp = BLM_CREATE( bcore_arr_fp_s ); \
        bcore_arr_fp_s_push( arr_fp, ( fp_t )plant_name##_planted_signal_handler );

/// Inside *signal.c: Registering a source inside a given plant registry
#define BETH_PLANT_SIGNAL_USE_SOURCE( source_name ) \
    bcore_arr_fp_s_push( arr_fp, ( fp_t )source_name##_signal_handler );

/// Inside *signal.c: End of a plant registry for a specific plane name
#define BETH_PLANT_SIGNAL_CLOSE_PLANT( plant_name ) \
        BLM_RETURNV( vd_t, bcore_signal_s_broadcast( o, ( bcore_fp_signal_handler* )arr_fp->data, arr_fp->size ) ); \
    }

/// Beginning of main function: registering plant signal handler
#define BETH_PLANT_USE( plant_name ) \
    BETH_PLANT_DECLARE_GENERAL_SIGNAL_HANDLER( plant_name ) \
    bcore_register_signal_handler_with_deep_dependencies( plant_name##_general_signal_handler );

/// general closer (end of main)
#define BETH_PLANT_CLOSEV( verbose ) \
    bcore_down( verbose );

/// general closer (end of main)
#define BETH_PLANT_CLOSE() \
    bcore_down( false );

/// builds from default config file: "plant_builder.cfg"
#define BETH_PLANT_BUILD_FROM_DEFAULT() \
    beth_plant_build_from_rel_file( __FILE__, "plant_builder.cfg" );

#endif // BCORE_PLANT_H
