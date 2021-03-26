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

/// beth-xoila interface functions and macros

#ifndef BCORE_XOILA_H
#define BCORE_XOILA_H

#include "bcore_trait.h"
#include "bcore_spect.h"
#include "bcore_spect_inst.h"
#include "bcore_hmap.h"
#include "bcore_arr.h"

/**********************************************************************************************************************/

/** group-stamp map
 *  This map allows retrieving all stamps that have a specified group in their trait-line.
 *  Requirement: the group must be 'retrievable'
 *  The map is a const object. Changes are only allowed during init or down cycles.
 */
bcore_hmap_tpto_s* bcore_xoila_get_group_stamp_map( void );

/** group-stamp list
 *  List of all stamps that have 'group' in their traitline.
 *  Retruns null in case group is not 'retrievable' or does not exist
 */
static inline const bcore_arr_tp_s* bcore_xoila_get_arr_traitline_stamps( tp_t group )
{
    vd_t* ptr = bcore_hmap_tpto_s_get( bcore_xoila_get_group_stamp_map(), group );
    return ptr ? *ptr : NULL;
}

void bcore_xoila_set_arr_traitline_stamps_d( tp_t group, bcore_arr_tp_s* arr );

/**********************************************************************************************************************/

/// Macros

#ifdef XOILA_SECTION
    #error "Keyword XOILA_SECTION is reserved for embedded xoila-code. Do not use it as macro."
#endif

/** Opening a xoila section by defining a group in the header file.
 *  This macro also defines the signal_handler
 *  Note: The variadic list contain additional arguments: trait_name, optional_include_file which are
 *        evaluated by the xoila-compiler
 *  Examples:
 *    XOILA_DEFINE_GROUP( bhvm_vop, bcore_inst )               - continues with #ifdef XOILA_SECTION enclosing xoila code
 *    XOILA_DEFINE_GROUP( bhvm_vop, bcore_inst, "bhvm_vop.x" ) - all related xoila code is embedded from "bhvm_vop.x"
 */
#define XOILA_DEFINE_GROUP( group_name, ... ) \
    BETH_EXPAND_GROUP_##group_name

/// XOILA perspective macros (perspective types have a name *_spect_s instead of *_s)

// Optionally conclude with semicolon and continue
// struct-body via BCORE_DECLARE_SPECT_BODY
#define XOILA_DECLARE_SPECT( group_name ) \
    typedef struct group_name group_name; \
    typedef struct group_name##_spect_s group_name##_spect_s; \
    BCORE_DEFINE_INLINE_SPECT_GET_TYPED_CACHED( group_name##_spect_s ) \
    BCORE_DEFINE_INLINE_SPECT_GET_AWARE( group_name##_spect_s ) \
    struct group_name##_spect_s

// Body definition only
#define XOILA_DECLARE_SPECT_BODY( group_name ) \
    struct group_name##_spect_s

#define XOILA_DEFINE_SPECT( parent_type, group_name ) \
    static sc_t group_name##_spect_s_def_g; \
    BCORE_DEFINE_SPECT_CACHE( group_name##_spect_s ); \
    BCORE_DEFINE_CREATE_SELF( group_name##_spect_s, group_name##_spect_s_def_g ) \
    static sc_t group_name##_spect_s_def_g = #group_name "_spect_s = spect " #parent_type

#define XOILA_REGISTER_SPECT( group_name )\
    bcore_spect_setup_cache( &group_name##_spect_s_cache_g ); \
    bcore_spect_define_creator( bentypeof( #group_name "_spect_s" ), bentypeof( #group_name ), group_name##_spect_s_create_self );

/// Declares general signal handler of *_signal.c
#define BETH_DECLARE_GENERAL_SIGNAL_HANDLER( target_name ) \
    vd_t target_name##_general_signal_handler( const bcore_signal_s* o );

/// Inside *signal.c: Begin of a xoila registry for a specific plane name
#define BETH_SIGNAL_OPEN( target_name ) \
    vd_t target_name##_general_signal_handler( const bcore_signal_s* o ) \
    { \
        BLM_INIT(); \
        bcore_arr_fp_s* arr_fp = BLM_CREATE( bcore_arr_fp_s ); \
        bcore_arr_fp_s_push( arr_fp, ( fp_t )target_name##_xo_signal_handler );

/// Inside *signal.c: Registering a source inside a given xoila registry
#define BETH_SIGNAL_USE_SOURCE( source_name ) \
    bcore_arr_fp_s_push( arr_fp, ( fp_t )source_name##_signal_handler );

/// Inside *signal.c: End of a xoila registry for a specific plane name
#define BETH_SIGNAL_CLOSE() \
        BLM_RETURNV( vd_t, bcore_signal_s_broadcast( o, ( bcore_fp_signal_handler* )arr_fp->data, arr_fp->size ) ); \
    }

#define BETH_SIGNAL_DEFINE( target_name ) \
    BETH_SIGNAL_OPEN( target_name ) \
    BETH_SIGNAL_CLOSE()

/// Beginning of main function: registering target signal handler
#define BETH_USE( target_name ) \
    BETH_DECLARE_GENERAL_SIGNAL_HANDLER( target_name ) \
    bcore_register_signal_handler_with_deep_dependencies( target_name##_general_signal_handler );

/// general closer (end of main)
#define BETH_CLOSEV( verbose ) \
    bcore_down( verbose );

/// general closer (end of main)
#define BETH_CLOSE() \
    bcore_down( false );

#endif // BCORE_XOILA_H
