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

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

group bcore_source = x_inst
{
    feature uz_t get_data( m @* o, vd_t data, uz_t size );
    feature uz_t inspect_data( m @* o, vd_t data, uz_t size );
    feature u0_t get_u0( m @* o );
    feature char get_char( m @* o );
    feature u0_t inspect_u0( m @* o );
    feature char inspect_char( m @* o );

    feature void parse_fv( m @* o, sc_t format, va_list args );
    feature void parse_fa( m @* o, sc_t format, ... );
    feature void parse_msg_fv( m @* o, sc_t format, va_list args );
    feature void parse_msg_fa( m @* o, sc_t format, ... );
    feature void parse_msg_to_sink_fa( m @* o, m bcore_sink* sink, sc_t format, ... );
    feature void parse_err_fa( m @* o, sc_t format, ... );
    feature er_t parse_em_fa( m @* o, sc_t format, ... );
    feature er_t parse_error_fa( m @* o, sc_t format, ... );
    feature bl_t parse_bl( m @* o, sc_t format );

    feature bl_t eos( c @* o );
    feature sc_t get_file( c @* o );
    feature s3_t get_index( c @* o );
    feature void set_index( m @* o, s3_t index );
    feature void move_index( m @* o, s3_t delta ); // set_index( get_index() + delta );


    stamp :string_s = aware :
    {
        /* format not disclosed */

        func( d @* create_from_string( c st_s* string ) );
        func( d @* create_from_string_d( m st_s* string ) );
        func( d @* create_from_sc( sc_t sc ) ); // same as ...create_sc
        func( d @* create_sc( sc_t sc ) );
        func( d @* create_fa( sc_t format, ... ) );
        func( d @* create_fv( sc_t format, va_list args ) );
        func( d @* create_parse_line( m bcore_source* source ) );

        func( m @* setup_from_string( m @* o, c st_s* string ) );
        func( m @* setup_from_string_d( m @* o, m st_s* string ) );
        func( m @* setup_from_sc( m @* o, sc_t sc ) ); // same as ...setup_sc
        func( m @* setup_sc( m @* o, sc_t sc ) );
        func( m @* setup_fa( m @* o, sc_t format, ... ) );
        func( m @* setup_fv( m @* o, sc_t format, va_list args ) );
        func( m @* setup_parse_line( m @* o, m bcore_source* source ) );
    };


    stamp :point_s = aware :
    {
        bcore_source -> source; // shallow reference
        s3_t index;

        func( void set( m @* o, m bcore_source* source ) );
        func( void parse_err_fv( c @* o, sc_t format, va_list args ) );
        func( void parse_err_fa( c @* o, sc_t format, ... ) );
        func( void parse_msg_fv( c @* o, sc_t format, va_list args ) );
        func( void parse_msg_fa( c @* o, sc_t format, ... ) );
        func( void parse_msg_to_sink_fv( c @* o, m bcore_sink* sink, sc_t format, va_list args ) );
        func( void parse_msg_to_sink_fa( c @* o, m bcore_sink* sink, sc_t format, ... ) );

        /// generates a parse error message and pushes it to the error stack (see bcore_error_manager.h); returns err_id
        func( er_t parse_error_fv( c @* o, sc_t format, va_list args ) );
        func( er_t parse_error_fa( c @* o, sc_t format, ... ) );

        /// creates a cloned source pointing to the index position of source point
        func( d bcore_source* clone_source( c @* o ) );

        /// write a source reference to sink in the form <file_path>:<line>:<col>
        func( void source_reference_to_sink( c @* o, bl_t file_name_only, m bcore_sink* sink ) );
    };
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

