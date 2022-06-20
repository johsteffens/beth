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

// parses an identifier with namespace-syntax
feature 'a' er_t parse_name_st( c @* o, m x_source* source, m st_s* name ) { ERR_fa( "Not implemented in '#<sc_t>'.", bnameof( o._ ) ); return 0; };

feature 'a' er_t parse_name_tp( c @* o, m x_source* source, m tp_t* name )
{
    m $* s = st_s!^^;
    o.parse_name_st( source, s );
    if( s.size == 0 ) return source.parse_error_fa( "Identifier expected." );
    tp_t t = o.compiler().entypeof( s.sc );
    if( name ) name.0 = t;
    return 0;
};

feature 'a' m xoico_compiler_s* compiler( c @* o ) { ERR_fa( "Not implemented in '#<sc_t>'.", bnameof( o._ ) ); return NULL; };
feature 'a' m xoico_cengine* cengine( c @* o )     { ERR_fa( "Not implemented in '#<sc_t>'.", bnameof( o._ ) ); return NULL; };
feature 'a' tp_t obj_type( c @* o )                { ERR_fa( "Not implemented in '#<sc_t>'.", bnameof( o._ ) ); return 0; };
feature 'a' d st_s* create_spect_name( c @* o )    { ERR_fa( "Not implemented in '#<sc_t>'.", bnameof( o._ ) ); return NULL; };

feature 'a' c xoico_transient_map_s* transient_map( c @* o );

feature 'a' sc_t nameof( c @* o, tp_t type )
{
    return o.compiler().nameof( type );
};

feature 'a' tp_t entypeof( c @* o, sc_t name )
{
    return o.compiler().entypeof( name );
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

