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

BTCL Documentation: beth/doc/bcore/btcl.md

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
/// Interface

//----------------------------------------------------------------------------------------------------------------------

/** Reads full object from source.
 *  If o implements copy_typed, automatic type conversion is used.
 *  Allows recovering from parse errors and conversion errors.
 *  Checks error-stack after copy_typed.
 *  In case of a parse error o is not being changed.
 */
func er_t t_from_source( m@* o, tp_t t, m x_source* source );
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
func d obliv @* create_from_source_t( m x_source* source, m tp_t* type );
func d obliv @* create_from_st_t( c st_s* st, m tp_t* type ) = :create_from_source_t( x_source_create_from_st( st )^, type );
func d obliv @* create_from_sc_t(   sc_t  sc, m tp_t* type ) = :create_from_source_t( x_source_create_from_sc( sc )^, type );
func d aware @* create_from_source( m x_source* source );
func d aware @* create_from_st( c st_s* st )  = :create_from_source( x_source_create_from_st( st )^ );
func d aware @* create_from_sc(   sc_t  sc )  = :create_from_source( x_source_create_from_sc( sc )^ );
func d aware @* create_from_file( sc_t file ) = :create_from_source( x_source_create_from_file( file )^ );

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func t_from_source
{
    m$* sr = sr_s!^;
    :parse_create_object( source, sr );
    x_inst_t_copy_typed( o, t, sr_s_o_type( sr ), sr.o );
    = bcore_error_last();
}

//----------------------------------------------------------------------------------------------------------------------

func create_from_source_t
{
    type.0 = 0;
    if( !source ) = NULL;
    m$* sr = sr_s!^;
    :parse_create_object( source, sr );
    if( sr.o && type ) type.0 = sr.o_type();
    m x_inst* obj = sr.o;
    = ( obj ) ? obj.fork() : NULL; // sr.o is NULL in case of error
}

//----------------------------------------------------------------------------------------------------------------------

func create_from_source
{
    tp_t t = 0;
    d @* o = :create_from_source_t( source, t.1 );
    if( t )
    {
        ASSERT( x_stamp_t_is_aware( t ) );
    }
    = o;
}

//----------------------------------------------------------------------------------------------------------------------

/** On entering, obj should be sr_null
 *  In case of error obj need not be discarded
 *  if default_obj is defined, obj copies from default_obj before parsing the body
 */
func er_t parse_create_object( m x_source* source, m sr_s* obj );

/**********************************************************************************************************************/

embed "bcore_x_btcl.x";

#endif // XOILA_SECTION

#endif  // BCORE_X_BTCL_H
