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

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

group bcore_via = x_inst
{
    feature 'a' uz_t get_size ( c@* o );
    feature 'a' tp_t iget_name( c@* o, uz_t index );
    feature 'a' sr_s iget     ( c@* o, uz_t index );
    feature 'a' void iset     ( m@* o, uz_t index, sr_s val );
    feature 'a' tp_t iget_type( c@* o, uz_t index );

    feature 'a' void iset_s3( m@* o, uz_t index, s3_t val );
    feature 'a' void iset_u3( m@* o, uz_t index, u3_t val );
    feature 'a' void iset_f3( m@* o, uz_t index, f3_t val );
    feature 'a' void iset_sz( m@* o, uz_t index, sz_t val );
    feature 'a' void iset_uz( m@* o, uz_t index, uz_t val );
    feature 'a' void iset_sc( m@* o, uz_t index, sc_t val );
    feature 'a' void iset_bl( m@* o, uz_t index, bl_t val );
    feature 'a' void iset_tp( m@* o, uz_t index, tp_t val );

    feature 'a' bl_t nexists(    c@* o, tp_t name );
    feature 'a' uz_t nget_index( c@* o, tp_t name ); // returns (uz_t)-1 if not found
    feature 'a' sr_s nget(       c@* o, tp_t name );
    feature 'a' tp_t nget_type(  c@* o, tp_t name );
    feature 'a' void nset(       m@* o, tp_t name, sr_s val );
    feature 'a' void nset_s3(    m@* o, tp_t name, s3_t val );
    feature 'a' void nset_u3(    m@* o, tp_t name, u3_t val );
    feature 'a' void nset_f3(    m@* o, tp_t name, f3_t val );
    feature 'a' void nset_sz(    m@* o, tp_t name, sz_t val );
    feature 'a' void nset_uz(    m@* o, tp_t name, uz_t val );
    feature 'a' void nset_sc(    m@* o, tp_t name, sc_t val );
    feature 'a' void nset_bl(    m@* o, tp_t name, bl_t val );
    feature 'a' void nset_tp(    m@* o, tp_t name, tp_t val );

    feature 'a' bl_t is_leaf(       c@* o );             // Leaf type according to function bcore_type_is_leaf
    feature 'a' bl_t is_pure_array( c@* o );             // Checks if object is an array without additional elements (pure arrays are not leafs)
    feature 'a' bl_t iis_array(     c@* o, uz_t index ); // Checks if element is an array
    feature 'a' bl_t iis_static(    c@* o, uz_t index ); // Checks if element is static (type need not be recorded)
    feature 'a' bl_t iis_link(      c@* o, uz_t index ); // Checks if element is a link (means that it can be NULL); (Note: An array is a distinct static object -> not a link)

};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

