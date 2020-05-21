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

#include "bcore_plant_compiler.h"

/**********************************************************************************************************************/
/// Beth-Plant Interface Functions

/** Sets verbosity level for beth_plant functions.
 *   > 0: Functions produce a report to stdout at different verbosity levels.
 *  == 0: All function run silently.
 */
void beth_plant_set_verbosity( sz_t verbosity );

/// Compiles all registered plants; returns beth_plant_update_required()
bl_t beth_plant_compile_all( void );

/// Compiles a specific registered plant;
void beth_plant_compile( sc_t name );

/// Checks if compiled plants require an update of the corresponding panted files
bl_t beth_plant_update_required( void );

/// Updates all planted files that require an update; returns true if any file was modified.
bl_t beth_plant_update( void );

/**********************************************************************************************************************/

vd_t bcore_plant_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

/// Macros

///--------------------------------------------------------------------------------------------------------------------
/// Dedicated Plant Setup Framework

/** Opening a plant section by defining a group in the header file.
 *  This macro also defines the signal_handler
 */
#define BETH_PLANT_DEFINE_GROUP( plant_name, group_name, trait ) \
    PLANT_GROUP( group_name, trait ) \
    static inline PLANT_DEFEINE_SIGNAL_HANDLER( plant_name, group_name )


/// Inside *signal.h: Begin of a plant registry for a specific plane name
#define BETH_PLANT_CONFIG_OPEN_PLANT( plant_name ) \
    static inline vd_t plant_name##_general_signal_handler( const bcore_signal_s* o ) \
    { \
        BLM_INIT(); \
        bcore_arr_fp_s* arr_fp = BLM_CREATE( bcore_arr_fp_s ); \
        bcore_arr_fp_s_push( arr_fp, ( fp_t )plant_name##_planted_signal_handler );

/// Inside *signal.h: Registering a group inside a given plant registry
#define BETH_PLANT_CONFIG_USE_GROUP( group_name ) \
    bcore_arr_fp_s_push( arr_fp, ( fp_t )group_name##_signal_handler );

/// Inside *signal.h: End of a plant registry for a specific plane name
#define BETH_PLANT_CONFIG_CLOSE_PLANT( plant_name ) \
        BLM_RETURNV( vd_t, bcore_signal_s_broadcast( o, ( bcore_fp_signal_handler* )arr_fp->data, arr_fp->size ) ); \
    }

/// general opener (beginning of main)
#define BETH_PLANT_OPEN /*nothing yet*/

/// Beginning of main function: registering plant signal handler
#define BETH_PLANT_USE( plant_name ) \
    bcore_register_signal_handler( plant_name##_general_signal_handler, typeof( #plant_name ) );

/// general closer (end of main)
#define BETH_PLANT_CLOSEV( verbose ) \
    bcore_down( verbose );

/// general closer (end of main)
#define BETH_PLANT_CLOSE \
    bcore_down( false );

/** Bootstrapping plant framework.
 *  When stating a new plant, create empty files <plant_name>_planted.h, <plant_name>_planted.c
 *
 *  <plant_name>_planted.h should contain:
 *  #include "bcore_std.h"
 *  BETH_PLANT_BOOTSTRAPPING_PLANTED_H( <plant_name> )
 *
 *  <plant_name>_planted.c should contain:
 *  #include "bcore_std.h"
 *  BETH_PLANT_BOOTSTRAPPING_PLANTED_C( <plant_name> )
 */

#define BETH_PLANT_BOOTSTRAPPING_PLANTED_H( plant_name ) \
    vd_t plant_name##_planted_signal_handler( const bcore_signal_s* o );

#define BETH_PLANT_BOOTSTRAPPING_PLANTED_C( plant_name ) \
    vd_t plant_name##_planted_signal_handler( const bcore_signal_s* o ) { return NULL; }

#endif // BCORE_PLANT_H
