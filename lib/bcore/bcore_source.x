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

group bcore_source = bcore_inst
{
    feature uz_t get_data( mutable, vd_t data, uz_t size );
    feature uz_t inspect_data( mutable, vd_t data, uz_t size );
    feature u0_t get_u0( mutable );
    feature char get_char( mutable );
    feature u0_t inspect_u0( mutable );
    feature char inspect_char( mutable );

    feature void parse_fv( mutable, sc_t format, va_list args );
    feature void parse_fa( mutable, sc_t format, ... );
    feature void parse_msg_fv( mutable, sc_t format, va_list args );
    feature void parse_msg_fa( mutable, sc_t format, ... );
    feature void parse_msg_to_sink_fa( mutable, bcore_sink* sink, sc_t format, ... );
    feature void parse_err_fa( mutable, sc_t format, ... );
    feature er_t parse_em_fa( mutable, sc_t format, ... );
    feature er_t parse_error_fa( mutable, sc_t format, ... );
    feature bl_t parse_bl( mutable, sc_t format );

    feature bl_t eos( const );
    feature sc_t get_file( const );
    feature s3_t get_index( const );
    feature void set_index( mutable, s3_t index );
    feature void move_index( mutable, s3_t delta ); // set_index( get_index() + delta );


    stamp :string_s = aware :
    {
        /* format not disclosed */

        func( @* create_from_string( const st_s* string ) );
        func( @* create_from_string_d( st_s* string ) );
        func( @* create_from_sc( sc_t sc ) ); // same as ...create_sc
        func( @* create_sc( sc_t sc ) );
        func( @* create_fa( sc_t format, ... ) );
        func( @* create_fv( sc_t format, va_list args ) );
    };


    stamp :point_s = aware :
    {
        /* format not disclosed */

        func( void set( mutable, bcore_source* source ) );
        func( void parse_err_fv( const, sc_t format, va_list args ) );
        func( void parse_err_fa( const, sc_t format, ... ) );
        func( void parse_msg_fv( const, sc_t format, va_list args ) );
        func( void parse_msg_fa( const, sc_t format, ... ) );
        func( void parse_msg_to_sink_fv( const, bcore_sink* sink, sc_t format, va_list args ) );
        func( void parse_msg_to_sink_fa( const, bcore_sink* sink, sc_t format, ... ) );

        /// generates a parse error message and pushes it to the error stack (see bcore_error_manager.h); returns err_id
        func( er_t parse_error_fv( const, sc_t format, va_list args ) );
        func( er_t parse_error_fa( const, sc_t format, ... ) );

        /// creates a cloned source pointing to the index position of source point
        func( bcore_source* clone_source( const ) );

        /// write a source reference to sink in the form <file_path>:<line>:<col>
        func( void source_reference_to_sink( const, bl_t file_name_only, bcore_sink* sink ) );
    };
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

