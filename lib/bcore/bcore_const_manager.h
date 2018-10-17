/** Copyright 2018 Johannes Bernhard Steffens
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

/** Associative storage and manager for (lock-free) global constants.
    Global constants are objects created during the init-signal cycle and
    destroyed during the down-signal cycle of a program.
    Between these cycles, they are treated constant and never modified,
    added or removed. Therefore concurrent read-access can be kept lock-free.
 */

#ifndef BCORE_CONST_MANAGER_H
#define BCORE_CONST_MANAGER_H

#include "bcore_control.h"
#include "bcore_signal.h"

/**********************************************************************************************************************/

/** Set function (Use only during init or down cycles)
  * Defining an existing key causes error and abort.
  * (Check via bcore_const_exists).
  */
void bcore_const_t_set_d( tp_t key, tp_t t, vd_t v ); // assumes ownership of v
void bcore_const_a_set_d( tp_t key,         vd_t v ); // assumes ownership of v
void bcore_const_x_set_d( tp_t key,         sr_s v ); // owns, forks depending on sr_s flags
void bcore_const_t_set_c( tp_t key, tp_t t, vc_t v ); // clones v
void bcore_const_a_set_c( tp_t key,         vc_t v ); // clones v
void bcore_const_x_set_c( tp_t key,         sr_s v ); // clones v

/// Removes key if existing. (Use only during init or down cycles)
void bcore_const_remove( tp_t key );

/// Get function (Do not modify referenced object); returns NULL if not existing
vc_t        bcore_const_get_o( tp_t key );
sr_s        bcore_const_get_x( tp_t key );
const sr_s* bcore_const_get_r( tp_t key );

/// Checks existence
bl_t bcore_const_exists( tp_t key );

/// Number of registered constants
sz_t bcore_const_size();

/**********************************************************************************************************************/

vd_t bcore_const_manager_signal_handler( const bcore_signal_s* o );

#endif  // BCORE_CONST_MANAGER_H

