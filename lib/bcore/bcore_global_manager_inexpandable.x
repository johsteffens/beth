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

/** Associative storage and manager for global objects.

    Global objects are stored in a lock-secured dedicated tp_t based hash manager.
    The interface is similar to bcore_const_manager.

    Global objects can be registered, removed and read-acccessed concurrently anywhere in the program.
    Concurrent modification of existing objects should be done by overwriting an existing key with a new object.
    Otherwise write access should be manually thread-secured.

    A typical use case for global objects in an application-context which are initialized at the start of the app
    (e.g. explicitly or from a parameter file) and used anywhere in the app.
 */

group bcore_global = x_inst
{

    /**********************************************************************************************************************/
    /// Global general purpose objects

    /** Set function
      * Defining an existing key overwrites the previous object.
      * (Check existence via bcore_global_exists).
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
    func vd_t    get_o( tp_t key );
    func sr_s    get_x( tp_t key );
    func m sr_s* get_r( tp_t key );

    /// verified - get function (Do not modify referenced object); error if key is missing or of different type in case type != 0
    func vd_t    vget_o( tp_t key, tp_t type );
    func sr_s    vget_x( tp_t key, tp_t type );
    func m sr_s* vget_r( tp_t key, tp_t type );

    /// Checks existence
    func bl_t exists( tp_t key );

    /// Number of registered constants
    func sz_t size();
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

