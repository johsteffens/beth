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

group bcore_generic_function = x_inst
{

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/// item represents the reflection of a function's argument or a function's return-typespec
stamp :item_s
{
    tp_t type;
    tp_t name; // optional name of argument (name of regular c function in case item represents the return-typespec)
    tp_t access_class; // 'const|mutable|discardable'
    s2_t indirection;
    bl_t flag_obliv;    // obliv indicates that this type can reference obliv objects (extends range of implicit casts)
    bl_t flag_aware;    // aware object is required (allows extended tests)
    bl_t flag_restrict; // C restrict keyword was used in declaration


    /** Parses a C-style or extended xoila style declaration and sets parameters accordingly
     *  Examples
     *   "er_t f"        -> type = er_t, indirection = 0, access_type = mutable, name = f
     *   "const st_s* a" -> type = st_s, indirection = 1, access_type = const,   name = a
     *   "st_s** a"      -> type = st_s, indirection = 2, access_type = mutable, name = a
     *   "st_s**"        -> type = st_s, indirection = 2, access_type = mutable, name = 0
     */
    func er_t parse_cx_code   ( m@* o, bcore_source* source );
    func er_t parse_cx_code_sc( m@* o,   sc_t  sc );
    func er_t parse_cx_code_st( m@* o, c st_s* st );

    /// complementary to parse_c_code
    func er_t push_c_code( c@* o, m st_s* st );
    func er_t push_x_code( c@* o, m st_s* st ); // xoila-style declaration (contains all parameters)

    /// push_c_code without 'name'
    func er_t push_c_code_raw( c@* o, st_s* st );

    /// cast prefix for code generation
    func er_t push_c_code_vd_arg_cast( c@* o, st_s* st );
    func er_t push_c_code_vd_ret_cast( c@* o, st_s* st );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp :s
{
    hidden fp_t fp; // castable to bcore_fp_generic_wrapper
    bcore_generic_function_item_s base;
    bcore_generic_function_item_s => [];

    func o clear   ( m@* o ); // sets size to zero
    func o set_size( m@* o, uz_t size ); // resize
    func m :item_s* push       ( m@* o );
    func m :item_s* push_item  ( m@* o, c :item_s* item );
    func m :item_s* push_item_d( m@* o, d :item_s* item );
    func void pop( m@* o );

    /** Parses a C-style function prototype (without semicolon) (also allows xoila style type qualifiers)
     *  Example: "s2_t foo( const st_s* st, s2_t a, s2_t b )"
     */
    func er_t parse_cx_code   ( m@* o, m bcore_source* source );
    func er_t parse_cx_code_sc( m@* o,   sc_t  sc );
    func er_t parse_cx_code_st( m@* o, c st_s* st );

    /// complementary to parse_c_code (generates c-code)
    func er_t push_c_code( c@* o, m st_s* st );
    func er_t push_x_code( c@* o, m st_s* st ); // xoila-style declaration (contains all parameters)

    /** Generates the prototype of the '__bgwf' function.
     *  Example: "void foo__bgwf( vd_t r, vd_t a[] )"
     */
    func er_t push_c_code_bgwf_declaration( c@* o, m st_s* st );

    /** generates the implementation of the '__bgwf' function as one-liner.
     *  Example: "void foo__bgwf( vd_t r, vd_t a[] ) { *(s2_t*)r = foo( (const st_s*)a[0], *(s2_t*)a[1], *(s2_t*)a[2] ); }"
     */
    func er_t push_c_code_bgwf_definition_one_line( c@* o, m st_s* st );

    /// Indicates that the regular function returns a value
    func bl_t returns_value( c@* o );
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func m :s* manager_set   ( tp_t name );
func void  manager_set_d ( d :s* function );
func void  manager_set_c ( c :s* function );
func void  manager_register_func( sc_t x_code, fp_t fp /*bgwf-function*/ ); // compact registration (for the init-cycle)
func sz_t  manager_size  ();
func bl_t  manager_exists( tp_t name );
func c :s* manager_get   ( tp_t name ); // returns NULL in case name is not registered
func void  manager_remove( tp_t name );

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

}
