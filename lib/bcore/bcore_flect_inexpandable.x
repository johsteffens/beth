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

group bcore_flect = x_inst
{

    stamp bcore_self_item_s = obliv :
    {
        tp_t type; // hash of type
        tp_t name; // hash of name
        u2_t caps; // data encapsulation
        bcore_flect_flags_s flags;
        umax_t default_umax;
        bcore_self_item_s => child_item;
    };

    type bcore_self_body_s;

    stamp bcore_self_s = aware :
    {
        /// type of object being reflected
        tp_t type;

        /// trait of object
        tp_t trait;

        /// parent (-trait) of object  (used by perspectives)
        tp_t parent;

        /** size
         *  Represents sizeof(object-type);
         *  Mandatory for leaf-types (which are basically types without body)
         *  For types with body:
         *   = 0: sizeof(type) is calculated in the instance perspective (e.g. for runtime generated types)
         *   > 0: sizeof(type) is calculated in the instance perspective and compared to bcore_self_s::size
         *        The perspective can produce a descriptive error in case of a mismatch.
         *        This feature is useful to detect the error that object's compile-time definition is out of
         *        sync with its reflection.
         */
        uz_t size;

        /** align
         *  Represents alignof(object-type);
         *  Mandatory for leaf-types (which are basically types without body)
         *  For types with body:
         *   = 0: alignof(type) is calculated in the instance perspective (e.g. for runtime generated types)
         *   > 0: alignof(type) is calculated in the instance perspective and compared to bcore_self_s::align
         *        The perspective can produce a descriptive error in case of a mismatch.
         *        This feature is useful to detect the error that object's computed alignment is does not
         *        match the compiler's alignment.
         */
        uz_t align;

        /** Body of type. Exposing the body is optional.
         *  If the body is exposed, declarations must be complete up to the last declared element.
         *  Otherwise, a perspective can not correctly compute the alignment of elements.
         *  A leaf type is defined as object without body.
         */
        bcore_self_body_s => body;

        func uz_t items_size( c @* o );
        func c bcore_self_item_s* get_item( c @* o, uz_t index );
        func c bcore_self_item_s* get_item_by_name( c @* o, tp_t name ); // returns NULL in case of no match
        func c bcore_self_item_s* get_first_array_item( c @* o ); // returns first item where caps is an array or NULL in case of no match
        func c bcore_self_item_s* get_first_anonymous_array_item( c @* o ); // returns first item where caps is an array and type == 0 or NULL in case of no match
    };

};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

