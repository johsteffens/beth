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
    feature void flush( m @* o );
    feature void push_fv( m @* o, sc_t format, va_list args );
    feature void push_fa( m @* o, sc_t format, ... );
    feature void push_u0( m @* o, u0_t u );
    feature void push_char( m @* o, char c );
    feature void push_sc(   m @* o, sc_t sc );
    feature void push_string(   m @* o, c st_s* string );
    feature void push_string_d( m @* o, m st_s* string );
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

