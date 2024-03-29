/** Author and Copyright 2021 Johannes Bernhard Steffens
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

/** Data stream sink. */

#ifndef BCORE_X_SINK_H
#define BCORE_X_SINK_H

#include "bcore_spect_sink.h"
#include "bcore_sinks.h"
#include "bcore_file.h"

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( x_sink, x_inst )

#ifdef XOILA_SECTION

/**********************************************************************************************************************/
/// interface

/** Create functions: Create a stream
 *  create_from_file returns NULL in case the file could not be opened for writing.
 */
func d x_sink* create_from_file( sc_t path ) = bcore_file_try_open_sink( path );

/// error in case file does not exist
func d x_sink* check_create_from_file( sc_t path )
{
    d x_sink* sink = bcore_file_try_open_sink( path );
    if( !sink ) ERR_fa( "Could no open file '#<sc_t>' as sink.", path );
    = sink;
}

//----------------------------------------------------------------------------------------------------------------------

func sz_t push_data( m @* o, c obliv x_inst* data, sz_t size ) = o.cast( m bcore_sink* ).push_data( data, size );
func o push_char( m @* o,    char c  ) = o.cast( m bcore_sink* ).push_char( c );
func o push_sc(   m @* o,    sc_t sc ) = o.cast( m bcore_sink* ).push_sc( sc );
func o push_st(   m @* o, c st_s* st ) = o.cast( m bcore_sink* ).push_string( st );
func o push_st_d( m @* o, d st_s* st ) = o.cast( m bcore_sink* ).push_string_d( st );
func o push_fv(   m @* o, sc_t format, va_list args ) = o.cast( m bcore_sink* ).push_fv( format, args );
func o push_fa(   m @* o, sc_t format, ... ) { va_list a; va_start( a, format ); o.push_fv( format, a ); va_end( a ); };
func o flush(     m @* o             ) = o.cast( m bcore_sink* ).flush();

func o push_s3( m @* o, s3_t v ) o.push_data( v.1, sizeof( v ) );
func o push_s2( m @* o, s2_t v ) o.push_data( v.1, sizeof( v ) );
func o push_s1( m @* o, s1_t v ) o.push_data( v.1, sizeof( v ) );
func o push_s0( m @* o, s0_t v ) o.push_data( v.1, sizeof( v ) );
func o push_u3( m @* o, u3_t v ) o.push_data( v.1, sizeof( v ) );
func o push_u2( m @* o, u2_t v ) o.push_data( v.1, sizeof( v ) );
func o push_u1( m @* o, u1_t v ) o.push_data( v.1, sizeof( v ) );
func o push_u0( m @* o, u0_t v ) = o.cast( m bcore_sink* ).push_u0( v );
func o push_f3( m @* o, f3_t v ) o.push_data( v.1, sizeof( v ) );
func o push_f2( m @* o, f2_t v ) o.push_data( v.1, sizeof( v ) );
func o push_bl( m @* o, bl_t v ) = o.cast( m bcore_sink* ).push_u0( v ); // bl treated as u0 to avoid cross platform inconsistencies
func o push_tp( m @* o, tp_t v ) o.push_data( v.1, sizeof( v ) );

// sz, uz are stored as s3,u3 to avoid cross platform inconsistencies
func o push_sz( m @* o, sz_t v ) o.push_s3( v );
func o push_uz( m @* o, uz_t v ) o.push_u3( v );

//----------------------------------------------------------------------------------------------------------------------

func m x_sink* stdout() = ( x_sink* )BCORE_STDOUT;
func m x_sink* stderr() = ( x_sink* )BCORE_STDERR;

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#endif // XOILA_SECTION

#endif  // BCORE_X_SINK_H
