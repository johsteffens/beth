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

#ifndef BCORE_GLOBAL_MANAGER_H
#define BCORE_GLOBAL_MANAGER_H

#include "bcore_control.h"
#include "bcore_signal.h"

/**********************************************************************************************************************/
/// Global general purpose objects

/** Set function
  * Defining an existing key overwrites the previous object.
  * (Check existence via bcore_global_exists).
  */
void bcore_global_t_set_d( tp_t key, tp_t t, vd_t v ); // assumes ownership of v
void bcore_global_a_set_d( tp_t key,         vd_t v ); // assumes ownership of v
void bcore_global_x_set_d( tp_t key,         sr_s v ); // owns, forks depending on sr_s flags
void bcore_global_t_set_c( tp_t key, tp_t t, vc_t v ); // clones v
void bcore_global_a_set_c( tp_t key,         vc_t v ); // clones v
void bcore_global_x_set_c( tp_t key,         sr_s v ); // clones v

/// Removes key if existing. (Use only during init or down cycles)
void bcore_global_remove( tp_t key );

/// Get function returns NULL if not existing
vd_t  bcore_global_get_o( tp_t key );
sr_s  bcore_global_get_x( tp_t key );
sr_s* bcore_global_get_r( tp_t key );

/// verified - get function (Do not modify referenced object); error if key is missing or of different type in case type != 0
vd_t  bcore_global_vget_o( tp_t key, tp_t type );
sr_s  bcore_global_vget_x( tp_t key, tp_t type );
sr_s* bcore_global_vget_r( tp_t key, tp_t type );

/// Checks existence
bl_t bcore_global_exists( tp_t key );

/// Number of registered constants
sz_t bcore_global_size();

/**********************************************************************************************************************/

vd_t bcore_global_manager_signal_handler( const bcore_signal_s* o );

#endif  // BCORE_GLOBAL_MANAGER_H

