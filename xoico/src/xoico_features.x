/** Author and Copyright 2022 Johannes Bernhard Steffens
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

signature bl_t exists_from_name( c @* o, tp_t name );
signature sz_t get_index_from_name( c @* o, tp_t name ); // returns -1 if not found

stamp :s = aware x_array
{
    xoico_feature_s => [];

    func :.get_index_from_name
    {
        foreach( m $* e in o ) if( e.signature.name == name ) return __i;
        return -1;
    };

    func :.exists_from_name
    {
        return ( o.get_index_from_name( name ) >= 0 );
    };
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
