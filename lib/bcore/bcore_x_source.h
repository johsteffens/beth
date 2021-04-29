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

/** Data stream source */

#ifndef BCORE_X_SOURCE_H
#define BCORE_X_SOURCE_H

#include "bcore_spect_source.h"
#include "bcore_sources.h"
#include "bcore_file.h"

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( x_source, x_inst )

#ifdef XOILA_SECTION

include 'c' "bcore_x_sink.h";

forward x_sink;
forward x_inst;

/**********************************************************************************************************************/
/// interface

//----------------------------------------------------------------------------------------------------------------------

/** Create functions: Create a stream
 *  create_from_file returns NULL in case the file does not exist.
 */
func (d x_source* create_from_file(   sc_t path )) = { return bcore_file_exists( path ) ? bcore_file_open_source( path ) : NULL; };
func (d x_source* create_from_st(     st_s* st  )) = { return bcore_source_string_s_create_from_string( st ); };
func (d x_source* create_from_st_d( d st_s* st  )) = { return bcore_source_string_s_create_from_string_d( st ); };
func (d x_source* create_from_sc(     sc_t  sc  )) = { return bcore_source_string_s_create_from_sc( sc ); };
func (d x_source* create_from_fv( sc_t format, va_list args )) = { return bcore_source_string_s_create_fv( format, args ); };
func (d x_source* create_from_fa( sc_t format, ... )) = { va_list a; va_start( a, format ); d x_source* r = :create_from_fv( format, a ); va_end( a ); return r; };

//----------------------------------------------------------------------------------------------------------------------

/// Returns 'true' when end of stream was reached
func (bl_t eos( m @*o )) = { return o.cast( m bcore_source* ).eos(); };

/** Read functions:
 *  - 'data'    Reads 'size' bytes into 'data' or less in case stream ends early. Returns number of bytes read.
 *  - 'u0,char' Reads a single byte. Retruns 0 in case stream ends early.
 *  Inspect-functions perform the read without changing the read index
 */
func (sz_t get_data(     m @*o, m obliv x_inst* data, sz_t size )) = { return o.cast( m bcore_source* ).get_data( data, size ); };
func (sz_t inspect_data( m @*o, m obliv x_inst* data, sz_t size )) = { return o.cast( m bcore_source* ).inspect_data( data, size ); };
func (u0_t get_u0(       m @*o )) = { return o.cast( m bcore_source* ).get_u0(); };
func (u0_t inspect_u0(   c @*o )) = { return o.cast( m bcore_source* ).inspect_u0(); };
func (char get_char(     c @*o )) = { return o.cast( m bcore_source* ).get_char(); };
func (char inspect_char( c @*o )) = { return o.cast( m bcore_source* ).inspect_char(); };

//----------------------------------------------------------------------------------------------------------------------

/** Random access:
 *  Functions set_index, get_index provide random access within a specific stream configuration.
 *  These functions may not be supported by a given stream. (They usually are supported)
 *  Unsupported set_index produces an error. Unsupported get_index returns always 0.
 *  Index values and value range is stream dependent and may also be negative.
 *  When supported: set_index is able to reset the stream to any read-position previously obtained via get_index.
 */
func (s3_t get_index( c @*o             )) = { return o.cast( c bcore_source* ).get_index(); };
func (void set_index( m @*o, s3_t index )) = { o.cast( m bcore_source* ).set_index( index ); };

//----------------------------------------------------------------------------------------------------------------------

/// Returns the boolean of a formatted parse. 'format' must yield just one bool, which is returned.
func (bl_t parse_bl( m @* o, sc_t format )) = { return o.cast( m bcore_source* ).parse_bl( format ); };

/// parses stream according to format string; returns != 0 in case of parse error. (Error message on stack)
signature er_t parse_fv( m @*o, sc_t format, va_list args );
signature er_t parse_fa( m @*o, sc_t format, ... );
func parse_fv = { return o.cast( m bcore_source* ).parse_em_fv( format, args ); };
func parse_fa = { va_list a; va_start( a, format ); er_t r = o.parse_fv( format, a ); va_end( a ); return r; };

/// generates a parse error message and pushes it to the error stack (see bcore_error_manager.h); returns err_id
signature er_t parse_error_fv( c @*o, sc_t format, va_list args );
signature er_t parse_error_fa( c @*o, sc_t format, ... );
func parse_error_fv = { return o.cast( bcore_source* ).parse_error_fv( format, args ); };
func parse_error_fa = { va_list a; va_start( a, format ); er_t r = o.parse_error_fv( format, a ); va_end( a ); return r; };

/// generates a parse message
signature void parse_msg_to_sink_fv( c @*o, m x_sink* sink, sc_t format, va_list args );
signature void parse_msg_to_sink_fa( c @*o, m x_sink* sink, sc_t format, ... );
func parse_msg_to_sink_fv = { o.cast( m bcore_source* ).parse_msg_to_sink_fv( sink, format, args ); };
func parse_msg_to_sink_fa = { va_list a; va_start( a, format ); o.parse_msg_to_sink_fv( sink, format, a ); va_end( a ); };

/// generates a parse message to stdout
signature void parse_msg_fv( c @*o, sc_t format, va_list args );
signature void parse_msg_fa( c @*o, sc_t format, ... );
func parse_msg_fv =
{
    o.parse_msg_to_sink_fv( x_sink_stdout(), format, args );
};
func parse_msg_fa = { va_list a; va_start( a, format ); o.parse_msg_fv( format, a ); va_end( a ); };

//----------------------------------------------------------------------------------------------------------------------

/// returns file path or NULL in case source is not associated with a file
func (sc_t get_file( c @*o )) = { return o.cast( bcore_source* ).get_file(); };

//----------------------------------------------------------------------------------------------------------------------

func (m x_source* stdin()) = { return ( x_source* )BCORE_STDIN; };

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// source_point

/** A source point is a runtime reference to a specific source location.
 *  This is often used for multi-pass parsing and descriptive message generation
 *  (e.g. parse error messages)\
 */

stamp :point_s = x_inst
{
    aware : -> source;
    s3_t index;

    func (void setup_from_source( m@* o, m x_source* source )) = { o.source =< source.fork(); o.index = source.get_index(); };

    /// creates a cloned source pointing to the index position of source point
    func (d x_source* clone_source( c@* o ));

    /// write a source reference to sink in the form <file_path>:<line>:<col>
    func (void source_reference_to_sink( @* o, bl_t file_name_only, m x_sink* sink ));

    func :.parse_msg_to_sink_fv;
    func :.parse_msg_fv;
    func :.parse_error_fv;

    func :.parse_msg_to_sink_fa = { va_list a; va_start( a, format ); o.parse_msg_to_sink_fv( sink, format, a ); va_end( a ); };
    func :.parse_msg_fa         = { va_list a; va_start( a, format );               o.parse_msg_fv( format, a ); va_end( a ); };
    func :.parse_error_fa       = { va_list a; va_start( a, format ); er_t err =  o.parse_error_fv( format, a ); va_end( a ); return err; };
};

//----------------------------------------------------------------------------------------------------------------------

func (:point_s) parse_msg_fv =
{
    o.parse_msg_to_sink_fv( x_sink_stdout(), format, args );
};

//----------------------------------------------------------------------------------------------------------------------

func (:point_s) clone_source =
{
    d x_source* source = o.source.clone();
    source.set_index( o.index );
    return source;
};

//----------------------------------------------------------------------------------------------------------------------

func (:point_s) :.parse_msg_to_sink_fv =
{
    if( o.source )
    {
        s3_t index = o.source.get_index();
        o.source.set_index( o.index );
        o.source.parse_msg_to_sink_fv( sink, format, args );
        o.source.set_index( index );
    }
    else
    {
        sink.push_fv( format, args );
    }
};

//----------------------------------------------------------------------------------------------------------------------

func (:point_s) :.parse_error_fv =
{
    er_t err = 0;
    if( o.source )
    {
        s3_t index = o.source.get_index();
        o.source.set_index( o.index );
        err = o.source.parse_error_fv( format, args );
        o.source.set_index( index );
    }
    else
    {
        err = bcore_error_push_fv( TYPEOF_parse_error, format, args );
    }
    return err;
};

//----------------------------------------------------------------------------------------------------------------------

func (:point_s) source_reference_to_sink =
{
    if( !o.source ) return;
    s3_t index = o.source.get_index();
    o.source.set_index( o.index );

    m bcore_source_context_s* context = bcore_source_context_s!^;
    o.source.cast( bcore_source* ).get_context( context );

    if( context.file_path )
    {
        m st_s* file = context.file_path.clone()^;
        if( file_name_only ) file.copy_sc( bcore_file_name( file.sc ) );
        sink.push_fa( "#<sc_t>", file.sc );
    }

    sink.push_fa( ":#<sz_t>:#<sz_t>", context.line, context.col );
    o.source.set_index( index );
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#endif // XOILA_SECTION

#endif  // BCORE_X_SOURCE_H
