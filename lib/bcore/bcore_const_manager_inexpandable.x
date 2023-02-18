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

/** Associative storage and manager for (lock-free) global constants and (lock-secured) strings.
    Global general constants are objects created during a init-signal and
    destroyed during the down-signal cycle of a program.
    Between these cycles, they are treated constant and never modified,
    added or removed. Therefore concurrent read-access can be kept lock-free.

    String constants are in a separate concurrent dedicated manager, which is lock-secured
    and memory-conservative. String constants can be registered and used concurrently anywhere
    in the program.
    A typical use case for string constants is the constant definition in a registry.
    Since registries can be created anywhere in the program, concurrency must be ensured.
 */

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

group bcore_const = x_inst
{

    /******************************************************************************************************************/
    /// Global general purpose constants

    /** Set function (Use only during init or down cycles)
      * Defining an existing key causes error and abort.
      * (Check via bcore_const_exists).
      */
    func void t_set_d( tp_t key, tp_t t, vd_t v ); // assumes ownership of v
    func void a_set_d( tp_t key,         vd_t v ); // assumes ownership of v
    func void x_set_d( tp_t key,         sr_s v ); // owns, forks depending on sr_s flags
    func void t_set_c( tp_t key, tp_t t, vc_t v ); // clones v
    func void a_set_c( tp_t key,         vc_t v ); // clones v
    func void x_set_c( tp_t key,         sr_s v ); // clones v

    /// Removes key if existing. (Use only during init or down cycles)
    func void remove( tp_t key );

    /// Get function (Modifying referenced object is only allowed during init or down cycles); returns NULL if not existing
    func vc_t        get_o( tp_t key );
    func sr_s        get_x( tp_t key );
    func const sr_s* get_r( tp_t key );

    /// verified - get function (Do not modify referenced object); error if key is missing or of different type in case type != 0
    func vc_t        vget_o( tp_t key, tp_t type );
    func sr_s        vget_x( tp_t key, tp_t type );
    func const sr_s* vget_r( tp_t key, tp_t type );

    /// Checks existence
    func bl_t exists( tp_t key );

    /******************************************************************************************************************/
    /** String constants
     *  Dedicated string constants are kept in a separate map and are keyed by the canonic string hash (via typeof( <string> ))
     *  String registration/access is concurrent (lock-secured) an can be used anywhere;
     *  Strings are tested for collisions.
     */
    func tp_t        string_set_sc(     sc_t  string ); // returns key; string collision tested and copied.
    func tp_t        string_set_st_c( c st_s* string ); // returns key; string collision tested and copied.
    func tp_t        string_set_st_d( m st_s* string ); // returns key; string collision tested; assumes ownership
    func sc_t        string_get_sc( tp_t key ); // retuns NULL if not existing
    func c st_s*     string_get_st( tp_t key ); // retuns NULL if not existing
    func bl_t        string_exists( tp_t key );

    /******************************************************************************************************************/

    /// Number of registered constants
    func sz_t size();
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

