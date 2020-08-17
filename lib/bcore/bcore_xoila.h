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

#ifndef BCORE_XOILA_H
#define BCORE_XOILA_H

#include "bcore_trait.h"
#include "bcore_spect.h"
#include "bcore_spect_inst.h"

/**********************************************************************************************************************/

/// Macros

#ifdef XOILA_SECTION
    #error "Keyword XOILA_SECTION is reserved for embedded xoila-code. Do not use it as macro."
#endif

/** Opening a plant section by defining a group in the header file.
 *  This macro also defines the signal_handler
 *  Note: The variadic list contain additional arguments: trait_name, optional_include_file which are
 *        evaluated by the xoila-compiler
 *  Examples:
 *    XOILA_DEFINE_GROUP( bhvm_vop, bcore_inst )               - continues with #ifdef XOILA_SECTION enclosing xoila code
 *    XOILA_DEFINE_GROUP( bhvm_vop, bcore_inst, "bhvm_vop.x" ) - all related xoila code is included from "bhvm_vop.x"
 */
#define XOILA_DEFINE_GROUP( group_name, ... ) \
    BETH_EXPAND_GROUP_##group_name

/// Declares general signal handler of *_signal.c
#define BETH_DECLARE_GENERAL_SIGNAL_HANDLER( target_name ) \
    vd_t target_name##_general_signal_handler( const bcore_signal_s* o );

/// Inside *signal.c: Begin of a plant registry for a specific plane name
#define BETH_SIGNAL_OPEN( target_name ) \
    vd_t target_name##_general_signal_handler( const bcore_signal_s* o ) \
    { \
        BLM_INIT(); \
        bcore_arr_fp_s* arr_fp = BLM_CREATE( bcore_arr_fp_s ); \
        bcore_arr_fp_s_push( arr_fp, ( fp_t )target_name##_xoila_out_signal_handler );

/// Inside *signal.c: Registering a source inside a given plant registry
#define BETH_SIGNAL_USE_SOURCE( source_name ) \
    bcore_arr_fp_s_push( arr_fp, ( fp_t )source_name##_signal_handler );

/// Inside *signal.c: End of a plant registry for a specific plane name
#define BETH_SIGNAL_CLOSE() \
        BLM_RETURNV( vd_t, bcore_signal_s_broadcast( o, ( bcore_fp_signal_handler* )arr_fp->data, arr_fp->size ) ); \
    }

#define BETH_SIGNAL_DEFINE( target_name ) \
    BETH_SIGNAL_OPEN( target_name ) \
    BETH_SIGNAL_CLOSE()

/// Beginning of main function: registering plant signal handler
#define BETH_USE( plant_name ) \
    BETH_DECLARE_GENERAL_SIGNAL_HANDLER( plant_name ) \
    bcore_register_signal_handler_with_deep_dependencies( plant_name##_general_signal_handler );

/// general closer (end of main)
#define BETH_CLOSEV( verbose ) \
    bcore_down( verbose );

/// general closer (end of main)
#define BETH_CLOSE() \
    bcore_down( false );

#endif // BCORE_XOILA_H
