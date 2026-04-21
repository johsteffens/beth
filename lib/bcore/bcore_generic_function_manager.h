/** Author and Copyright 2026 Johannes Bernhard Steffens
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

/** Generic Function Manager
 *  This framework stores the complete reflection of a functions and makes it available at runtime.
 *  A function registered with this manager is called 'generic'.
 *  A generic function has a runtime-registered signature and a uniform caller.
 *  It can be discovered and executed from another generic framework at runtime (e.g. an interpreter such as BTCL)
 *
 *  A generic function has two implementations:
 *    1. The regular C implementation
 *    2. A generic wrapper implementation compatible to bcore_fp_generic_function calling the regular function
 *
 *  The generic wrapper function has the name of the regular function with suffix '__bgwf' (acronym for 'beth-generic-wrapper-function')
 */

#ifndef BCORE_GENERIC_FUNCTION_MANAGER_H
#define BCORE_GENERIC_FUNCTION_MANAGER_H

#include "bcore_flect.h"
#include "bcore_feature.h"
#include "bcore_threads.h"
#include "bcore_hmap.h"

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/// Generic function pointer
typedef void (*bcore_fp_generic_wrapper)( vd_t ret, vd_t args[] );

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/// item represents the reflection of a function's argument or a function's return-typespec
BCORE_DECLARE_OBJECT( bcore_generic_function_item_s )
{
    aware_t _;
    tp_t type;
    tp_t name; // optional name of argument (name of regular c function in case of return-typespec)
    tp_t access_class; // 'const|mutable|discardable'

    s2_t indirection;
    bl_t flag_obliv;    // obliv indicates that this type can reference obliv objects (extends range of implicit casts)
    bl_t flag_aware;    // aware object is required (allows extended tests)
    bl_t flag_restrict; // C restrict keyword was used in declaration
};

BCORE_FORWARD_OBJECT( bcore_source );

//----------------------------------------------------------------------------------------------------------------------

/** Parses a C-style or extended xoila style declaration and sets parameters accordingly
 *  Examples
 *   "er_t f"        -> type = er_t, indirection = 0, access_type = mutable, name = f
 *   "const st_s* a" -> type = st_s, indirection = 1, access_type = const,   name = a
 *   "st_s** a"      -> type = st_s, indirection = 2, access_type = mutable, name = a
 *   "st_s**"        -> type = st_s, indirection = 2, access_type = mutable, name = 0
 */
er_t bcore_generic_function_item_s_parse_cx_code   ( bcore_generic_function_item_s* o, bcore_source* source );
er_t bcore_generic_function_item_s_parse_cx_code_sc( bcore_generic_function_item_s* o,       sc_t  sc );
er_t bcore_generic_function_item_s_parse_cx_code_st( bcore_generic_function_item_s* o, const st_s* st );

/// complementary to parse_c_code
er_t bcore_generic_function_item_s_push_c_code( const bcore_generic_function_item_s* o, st_s* st );
er_t bcore_generic_function_item_s_push_x_code( const bcore_generic_function_item_s* o, st_s* st ); // xoila-style declaration (contains all parameters)

/// push_c_code without 'name'
er_t bcore_generic_function_item_s_push_c_code_raw( const bcore_generic_function_item_s* o, st_s* st );

/// cast prefix for code generation
er_t bcore_generic_function_item_s_push_c_code_vd_arg_cast( const bcore_generic_function_item_s* o, st_s* st );
er_t bcore_generic_function_item_s_push_c_code_vd_ret_cast( const bcore_generic_function_item_s* o, st_s* st );

//----------------------------------------------------------------------------------------------------------------------

bcore_generic_function_item_s* bcore_generic_function_item_s_create_parse_cx_code   ( bcore_source* source );
bcore_generic_function_item_s* bcore_generic_function_item_s_create_parse_cx_code_sc( sc_t sc );
bcore_generic_function_item_s* bcore_generic_function_item_s_create_parse_cx_code_st( const st_s* st );

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

BCORE_DECLARE_OBJECT( bcore_generic_function_s )
{
    aware_t _;
    bcore_fp_generic_wrapper fp;
    bcore_generic_function_item_s base; // function name and return type
    BCORE_ARRAY_DYN_LINK_STATIC_S( bcore_generic_function_item_s, ); // arguments
};

bcore_generic_function_s*      bcore_generic_function_s_clear      ( bcore_generic_function_s* o ); // sets size to zero
bcore_generic_function_s*      bcore_generic_function_s_set_size   ( bcore_generic_function_s* o, uz_t size ); // resize
bcore_generic_function_item_s* bcore_generic_function_s_push       ( bcore_generic_function_s* o );
bcore_generic_function_item_s* bcore_generic_function_s_push_item  ( bcore_generic_function_s* o, const bcore_generic_function_item_s* item );
bcore_generic_function_item_s* bcore_generic_function_s_push_item_d( bcore_generic_function_s* o,       bcore_generic_function_item_s* item );
void                           bcore_generic_function_s_pop        ( bcore_generic_function_s* o );

/** Parses a C-style function prototype (without semicolon) (also allows xoila style type qualifiers)
 *  Example: "s2_t foo( const st_s* st, s2_t a, s2_t b )"
 */
er_t bcore_generic_function_s_parse_cx_code   ( bcore_generic_function_s* o, bcore_source* source );
er_t bcore_generic_function_s_parse_cx_code_sc( bcore_generic_function_s* o,       sc_t  sc );
er_t bcore_generic_function_s_parse_cx_code_st( bcore_generic_function_s* o, const st_s* st );

/// complementary to parse_c_code (generates c-code)
er_t bcore_generic_function_s_push_c_code( const bcore_generic_function_s* o, st_s* st );
er_t bcore_generic_function_s_push_x_code( const bcore_generic_function_s* o, st_s* st ); // xoila-style declaration (contains all parameters)

/** Generates the prototype of the '__bgwf' function.
 *  Example: "void foo__bgwf( vd_t r, vd_t a[] )"
 */
er_t bcore_generic_function_s_push_c_code_bgwf_declaration( const bcore_generic_function_s* o, st_s* st );

/** generates the implementation of the '__bgwf' function as one-liner.
 *  Example: "void foo__bgwf( vd_t r, vd_t a[] ) { *(s2_t*)r = foo( (const st_s*)a[0], *(s2_t*)a[1], *(s2_t*)a[2] ); }"
 */
er_t bcore_generic_function_s_push_c_code_bgwf_definition_one_line( const bcore_generic_function_s* o, st_s* st );

/// Indicates that the regular function returns a value
bl_t bcore_generic_function_s_returns_value( const bcore_generic_function_s* o );

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/** Global generic function Manager (Thread Safe)
 *  Retrieval Key is the regular function name (same as 'name' in bcore_generic_function_s)
 */
BCORE_DECLARE_OBJECT( bcore_generic_function_manager_s )
{
    aware_t _;
    bcore_mutex_s mutex;
    bcore_hmap_tpaw_s hmap;
};

//----------------------------------------------------------------------------------------------------------------------

bcore_generic_function_s*       bcore_generic_function_manager_set   ( tp_t name );
void                            bcore_generic_function_manager_set_d (       bcore_generic_function_s* function );
void                            bcore_generic_function_manager_set_c ( const bcore_generic_function_s* function );
void                            bcore_generic_function_manager_register_func( sc_t x_code, bcore_fp_generic_wrapper fp ); // compact registration (for the init-cycle)

sz_t                            bcore_generic_function_manager_size  ( void );
bl_t                            bcore_generic_function_manager_exists( tp_t name );
const bcore_generic_function_s* bcore_generic_function_manager_get   ( tp_t name ); // returns NULL in case name is not registered
void                            bcore_generic_function_manager_remove( tp_t name );

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

vd_t bcore_generic_function_manager_signal_handler( const bcore_signal_s* o );


#endif // BCORE_GENERIC_FUNCTION_MANAGER_H
