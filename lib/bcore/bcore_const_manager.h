/** Author and Copyright 2018 Johannes Bernhard Steffens
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

    String constants is a separate concurrent dedicated manager, which is lock-secured
    and memory conservative. String constants can be registered and used concurrently anywhere
    in the program.
 */

#ifndef BCORE_CONST_MANAGER_H
#define BCORE_CONST_MANAGER_H

#include "bcore_control.h"
#include "bcore_signal.h"

/**********************************************************************************************************************/
/// Global general constants

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

/// verified - get function (Do not modify referenced object); error if key is missing or of different type in case type != 0
vc_t        bcore_const_vget_o( tp_t key, tp_t type );
sr_s        bcore_const_vget_x( tp_t key, tp_t type );
const sr_s* bcore_const_vget_r( tp_t key, tp_t type );

/// Checks existence
bl_t bcore_const_exists( tp_t key );

/**********************************************************************************************************************/
/** String constants
 *  Dedicated string constants are kept in a separate map and are keyed by the canonic string hash (via typeof( <string> ))
 *  String registration/access is concurrent (lock-secured) an can be used anywhere;
 *  Strings are tested for collisions.
 */
tp_t        bcore_const_string_set_sc(          sc_t string ); // returns key; string collision tested and copied.
tp_t        bcore_const_string_set_st_c( const st_s* string ); // returns key; string collision tested and copied.
tp_t        bcore_const_string_set_st_d(       st_s* string ); // returns key; string collision tested; assumes ownership
sc_t        bcore_const_string_get_sc( tp_t key    ); // retuns NULL if not existing
const st_s* bcore_const_string_get_st( tp_t key    ); // retuns NULL if not existing
bl_t        bcore_const_string_exists( tp_t key    );

/**********************************************************************************************************************/

/// Number of registered constants
sz_t bcore_const_size();

/**********************************************************************************************************************/

vd_t bcore_const_manager_signal_handler( const bcore_signal_s* o );

#endif  // BCORE_CONST_MANAGER_H

