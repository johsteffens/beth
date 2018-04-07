/** Copyright 2017 Johannes Bernhard Steffens
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

/** Function management framework.
 *  This framework links types to functions.
 *  Type value 0 is reserved and not allowed to be associated with a function.
 */

#ifndef BCORE_FUNCTION_MANAGER_H
#define BCORE_FUNCTION_MANAGER_H

#include "bcore_control.h"

/// enrolls function in global manager (thread safe); checks for collisions;
void bcore_function_set( tp_t t, fp_t f );
tp_t bcore_function_set_sc( sc_t name, fp_t f );

/// returns function associated with t or NULL if not enrolled (thread safe)
fp_t bcore_function_get( tp_t t );

/// removes t if existing
void bcore_function_remove( tp_t t );

vd_t bcore_function_manager_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

// Macros
#define BCORE_REGISTER_FUNC( name ) bcore_function_set( entypeof( #name ), ( fp_t )name );

#endif // BCORE_FUNCTION_MANAGER_H
