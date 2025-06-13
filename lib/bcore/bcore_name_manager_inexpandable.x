/** Author and Copyright 2023 Johannes Bernhard Steffens
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

/** Names and Name-Spaces
 *  Each name has an associated name-space.
 *  The root name space (default) has type 0.
 *  Each name-type can be used as name-space.
 *  Functions with postfix _s explicitly consider name spaces:
 *     Functions returning a string construct a full name using ':' to catenate name spaces.
 *     Functions returning a type expect the name space as argument
 *  It is
 *  typeof_s( typeof( "mynamespace" ), "myname" ) == typeof( "mynamespace:myname" ).
 *
 *  Accordingly...
 *    tp_t type = entypeof_s( typeof( "mynamespace" ), "myname" )
 *  or
 *    tp_t type = entypeof( "mynamespace:myname" )
 *  ... produce the same value of type.
 *  However, nameof( type ) would yield different names.
 *  Therefore, trying to enroll both, causes a collision error.
 *  Best practice: Avoid colons inside regular names.
 *
 *
 *  LIST OF KNOWN COLLISIONS:
 *    - so far none is known -
 */

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

identifier ifnameof;
identifier btypeof;
identifier bentypeof;
identifier bnameof;

group bcore_name = x_inst
{
    /** Enrolls name in global manager (thread safe); returns hash; no effect if name is already enrolled
     *  Checks for collisions; In case of collision, program aborts with message to stderr.
     *  For a more controlled collision check, you can use functions 'check_collision' below.
     */
    func tp_t enroll(                     sc_t name );             // enrolls name
    func tp_t enroll_n(                   sc_t name, uz_t n );     // enrolls first n characters of name
    func tp_t enroll_s(  tp_t name_space, sc_t name );             // enrolls name in a namespace
    func tp_t enroll_sn( tp_t name_space, sc_t name, uz_t n );     // enrolls first n characters of name in a namespace

    /** Check for collision. Does not change state of name manager.
     *  Also checks for hashing to '0' or invalid name space.
     *  Returns 0 in case of no collision. Otherwise sends an error condition to the error manager.
     */
    func er_t check_collision(                     sc_t name );         // checks name
    func er_t check_collision_n(                   sc_t name, uz_t n ); // checks first n characters of name
    func er_t check_collision_s(  tp_t name_space, sc_t name );         // checks name in a namespace
    func er_t check_collision_sn( tp_t name_space, sc_t name, uz_t n ); // checks first n characters of name in a namespace

    /// hash --> name; returns NULL when not enrolled (thread safe)
    func sc_t try_name( tp_t type );
    func bl_t exists  ( tp_t type );

    /// hash --> name; returns error when not enrolled (thread safe)
    func sc_t get_name( tp_t type );

    /// hash --> full name; returns NULL when not enrolled (thread safe); passes ownership
    func st_s* try_name_s( tp_t type );

    /// hash --> full name; returns error when not enrolled (thread safe); passes ownership
    func st_s* get_name_s( tp_t type );

    /// removes hash and associated name; no effect when not enrolled (thread safe)
    func void remove( tp_t type );

    /// pushes all names to name_map (thread safe)
    func void push_all( bcore_hmap_name_s* name_map );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

