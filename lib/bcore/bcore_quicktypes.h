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

// Quickly obtainable types.

#ifndef BCORE_QUICKTYPES_H
#define BCORE_QUICKTYPES_H

#include "bcore_name_manager.h"
#include "bcore_quicktypes_list.h"

/// This function computes and formats defines below to stdout
void bcore_quicktypes_to_stdout( tp_t (*hash)( sc_t name ) );

/// identifies leaf types
bl_t bcore_type_is_leaf( tp_t type );

/// Macros

/// Register quicktype in signal_handler at signal TYPEOF_get_quicktypes
#define BCORE_REGISTER_QUICKTYPE( name )\
    if( o->object ) bcore_array_a_push_sc( ( bcore_array* )o->object, #name )

#endif // BCORE_QUICKTYPES_H
