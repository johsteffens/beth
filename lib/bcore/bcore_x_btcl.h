/** Author and Copyright 2024 Johannes Bernhard Steffens
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

/**

BTCL: Beth text constructive language (interpreter).
Backward compatible to BTML.

Usage: See beth/doc/bcore/btcl.md

*/

#ifndef BCORE_X_BTCL_H
#define BCORE_X_BTCL_H

#include "bcore_sr.h"
#include "bcore.xo.h"
#include "bcore_hmap_name.h"
#include "bcore_x_source.h"
#include "bcore_x_sink.h"
#include "bcore_x_btml.h"

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( x_btcl, x_inst )

#ifdef XOILA_SECTION

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// Main Parser Interface

//----------------------------------------------------------------------------------------------------------------------

stamp :s
{
    /// Search paths for embedding other files.
    bcore_arr_st_s => eval_path_arr;

    /** Reads full object from source.
     *  If o implements copy_typed, automatic type conversion is used.
     *  Allows recovering from parse errors and conversion errors.
     *  Checks error-stack after copy_typed.
     *  In case of a parse error o is not being changed.
     */
    func er_t t_from_source( @* o, m:* obj, tp_t t, m x_source* source );
    func er_t   from_source( @* o, m:* obj,         m x_source* source ) = o.t_from_source( obj, obj._, source );
    func er_t t_from_file  ( @* o, m:* obj, tp_t t,   sc_t file ) = o.t_from_source( obj, t, x_source_check_create_from_file( file )^ );
    func er_t   from_file  ( @* o, m:* obj,           sc_t file ) = o.t_from_file  ( obj, obj._, file );
    func er_t t_from_st    ( @* o, m:* obj, tp_t t, c st_s* st  ) = o.t_from_source( obj, t, x_source_create_from_st( st )^ );
    func er_t   from_st    ( @* o, m:* obj,         c st_s* st  ) = o.t_from_st    ( obj, obj._, st );
    func er_t t_from_sc    ( @* o, m:* obj, tp_t t,   sc_t  sc  ) = o.t_from_source( obj, t, x_source_create_from_sc( sc )^ );
    func er_t   from_sc    ( @* o, m:* obj,           sc_t  sc  ) = o.t_from_sc    ( obj, obj._, sc );

    /** Reads and creates object from source.
     *  Returns NULL in case of parse error (check error-stack).
     *  If type is != NULL Sets type.0 to object's type.
     */
    func d obliv :* create_from_source_t( @* o, m x_source* source, m tp_t* type );
    func d obliv :* create_from_st_t    ( @* o, c st_s* st, m tp_t* type ) = o.create_from_source_t( x_source_create_from_st( st )^, type );
    func d obliv :* create_from_sc_t    ( @* o,   sc_t  sc, m tp_t* type ) = o.create_from_source_t( x_source_create_from_sc( sc )^, type );
    func d aware :* create_from_source  ( @* o, m x_source* source );
    func d aware :* create_from_st      ( @* o, c st_s* st )  = o.create_from_source( x_source_create_from_st( st )^ );
    func d aware :* create_from_sc      ( @* o,   sc_t  sc )  = o.create_from_source( x_source_create_from_sc( sc )^ );
    func d aware :* create_from_file    ( @* o, sc_t file )   = o.create_from_source( x_source_create_from_file( file )^ );

    /** Executes script as standalone.
     *  Discards returned object.
     */
    func er_t run_from_source( @* o, m x_source* source );
    func er_t run_from_st    ( @* o, c st_s* st ) = o.run_from_source( x_source_create_from_st( st )^ );
    func er_t run_from_sc    ( @* o,   sc_t  sc ) = o.run_from_source( x_source_create_from_sc( sc )^ );
    func er_t run_from_file  ( @* o,  sc_t file ) = o.run_from_source( x_source_create_from_file( file )^ );

    /** On entering, obj should be sr_null
     *  In case of error obj need not be discarded
     *  if default_obj is defined, obj copies from default_obj before parsing the body
     */
    func er_t parse_create_object( @* o, m x_source* source, m sr_s* obj );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// Direct Interface (creates x_btcl_s default instance on the fly)

//----------------------------------------------------------------------------------------------------------------------

/** Reads full object from source.
 *  If o implements copy_typed, automatic type conversion is used.
 *  Allows recovering from parse errors and conversion errors.
 *  Checks error-stack after copy_typed.
 *  In case of a parse error o is not being changed.
 */
func er_t t_from_source( m@* o, tp_t t, m x_source* source ) = :s!^.t_from_source( o, t, source );
func er_t   from_source( m@* o,         m x_source* source ) = o.t_from_source( o._, source );
func er_t t_from_file  ( m@* o, tp_t t,   sc_t file ) = o.t_from_source( t, x_source_check_create_from_file( file )^ );
func er_t   from_file  ( m@* o,           sc_t file ) = o.t_from_file( o._, file );
func er_t t_from_st    ( m@* o, tp_t t, c st_s* st  ) = o.t_from_source( t, x_source_create_from_st( st )^ );
func er_t   from_st    ( m@* o,         c st_s* st  ) = o.t_from_st( o._, st );
func er_t t_from_sc    ( m@* o, tp_t t,   sc_t  sc  ) = o.t_from_source( t, x_source_create_from_sc( sc )^ );
func er_t   from_sc    ( m@* o,           sc_t  sc  ) = o.t_from_sc( o._, sc );

/** Reads and creates object from source.
 *  Returns NULL in case of parse error (check error-stack).
 *  If type is != NULL Sets type.0 to object's type.
 */
func d obliv @* create_from_source_t( m x_source* source, m tp_t* type ) = :s!^.create_from_source_t( source, type );
func d obliv @* create_from_st_t( c st_s* st, m tp_t* type ) = :create_from_source_t( x_source_create_from_st( st )^, type );
func d obliv @* create_from_sc_t(   sc_t  sc, m tp_t* type ) = :create_from_source_t( x_source_create_from_sc( sc )^, type );
func d aware @* create_from_source( m x_source* source ) = :s!^.create_from_source( source );
func d aware @* create_from_st( c st_s* st )  = :create_from_source( x_source_create_from_st( st )^ );
func d aware @* create_from_sc(   sc_t  sc )  = :create_from_source( x_source_create_from_sc( sc )^ );
func d aware @* create_from_file( sc_t file ) = :create_from_source( x_source_create_from_file( file )^ );

/** Executes script as standalone.
 *  Discards returned object.
 */
func er_t run_from_source( m x_source* source ) = :s!^.run_from_source( source );
func er_t run_from_st( c st_s* st )  = :run_from_source( x_source_create_from_st( st )^ );
func er_t run_from_sc(   sc_t  sc )  = :run_from_source( x_source_create_from_sc( sc )^ );
func er_t run_from_file( sc_t file ) = :run_from_source( x_source_create_from_file( file )^ );

/** On entering, obj should be sr_null
 *  In case of error obj need not be discarded
 *  if default_obj is defined, obj copies from default_obj before parsing the body
 */
func er_t parse_create_object( m x_source* source, m sr_s* obj ) = :s!^.parse_create_object( source, obj );

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// External Functions

//----------------------------------------------------------------------------------------------------------------------

/** Implement features below to define member functions of a stamp which shall be accessible by bctl code.
 *  See btcl_x_test.x for an example implementation
 */

// (required) return -1 when function 'name' is not defined
feature 'at' sz_t btcl_function_arity( @* o, tp_t name ) = -1;

// (optional) indicates if calling the btcl function changes the underlying instance;
// If true, btcl creates a copy before calling.
feature 'at' bl_t btcl_function_mutable( @* o, tp_t name ) = false;

// Depending on mutability one or both of the following functions must be implemented.
// All names declared by arity must be handled in one of these functions.

// Overload for all const functions
feature 'at' er_t btcl_function( @* o, tp_t name, x_source_point_s* sp, m :frame_s* lexical_frame, bcore_arr_sr_s* args, m sr_s* result );

// Overload for all mutable functions
feature 'at' er_t m_btcl_function( m@* o, tp_t name, x_source_point_s* sp, m :frame_s* lexical_frame, bcore_arr_sr_s* args, m sr_s* result );

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/** External Parser
 *  Overloading these features lets BTCL pass parsing control to an external parse function.
 *  The external code section ends before an unmatched closing bracket ')'.
 *  The external parse function must either consume all characters before that bracket or generate an error condition.
 *  In BTCL source code, the parse function is called as member function to the object using keyword 'parse'
 *  See btcl_x_test.x for an example implementation.
 */

//----------------------------------------------------------------------------------------------------------------------

// Overload one of these features depending on whether o is mutable or not.
feature 'at' er_t   btcl_external_parse( c@* o, m x_source* source, m :frame_s* lexical_frame, m sr_s* result );
feature 'at' er_t m_btcl_external_parse( m@* o, m x_source* source, m :frame_s* lexical_frame, m sr_s* result );

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// Functor

//----------------------------------------------------------------------------------------------------------------------

/// features of functors (overloaded by x_btcl_functor_s and x_btcl_functor_f3_s)
feature 'at' f3_t nullary_f3( @* o );
feature 'at' f3_t unary_f3  ( @* o, f3_t x );
feature 'at' f3_t binary_f3 ( @* o, f3_t x, f3_t y );
feature 'at' f3_t ternary_f3( @* o, f3_t x, f3_t y, f3_t z );

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func (:s) t_from_source
{
    m$* sr = sr_s!^;
    o.parse_create_object( source, sr );
    x_inst_t_copy_typed( obj, t, sr_s_o_type( sr ), sr.o );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) create_from_source_t
{
    type.0 = 0;
    if( !source ) = NULL;
    m$* sr = sr_s!^;
    if( o.parse_create_object( source, sr ) )
    {
        = NULL;
    }
    if( sr.o && type ) type.0 = sr.o_type();
    m x_inst* obj = sr.o;
    = ( obj ) ? obj.fork() : NULL; // sr.o is NULL in case of error
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) create_from_source
{
    tp_t t = 0;
    d :* obj = o.create_from_source_t( source, t.1 );
    if( t )
    {
        ASSERT( x_stamp_t_is_aware( t ) );
    }
    = obj;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) run_from_source
{
    if( !source ) = 0;
    = o.parse_create_object( source, sr_s!^ );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

embed "bcore_x_btcl.x";

#endif // XOILA_SECTION

#endif  // BCORE_X_BTCL_H
