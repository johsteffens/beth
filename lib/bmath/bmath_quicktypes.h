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

// Quickly obtainable types.

#ifndef BMATH_QUICKTYPES_H
#define BMATH_QUICKTYPES_H

#include "bcore_name_manager.h"
#include "bcore_quicktypes.h"

/// This function computes and formats defines below to stdout
void bmath_quicktypes_to_stdout( tp_t (*hash)( sc_t name ) );

/** List of predefined types for quick access.
 *  Created via bclos_quicktypes_to_stdout( NULL );
 */


#endif // BMATH_QUICKTYPES_H
