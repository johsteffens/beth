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

group bcore_sink = x_inst
{
    feature uz_t push_data( m @* o, vc_t data, uz_t size );
    feature o flush( m @* o );
    feature o push_fv( m @* o, sc_t format, va_list args );
    feature o push_fa( m @* o, sc_t format, ... );
    feature o pushf(   m @* o, sc_t format, ... );
    feature o push_u0( m @* o, u0_t u );
    feature o push_char( m @* o, char c );
    feature o push_sc(   m @* o, sc_t sc );
    feature o push_string(   m @* o, c st_s* string );
    feature o push_string_d( m @* o, d st_s* string );

    stamp :buffer_s =
    {
        /* format not disclosed */

        func :.flush;
        func :.push_data;
    };

    stamp :file_s =
    {
        /* format not disclosed */

        func m@* create_name( sc_t name ); // file is opened lazily when needed
        func sc_t get_name( @* o );
        func void open(  m @* o ); // (re)opens file
        func void close( m @* o ); // closes file if open
        func :.flush;
        func :.push_data;
    };


};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

