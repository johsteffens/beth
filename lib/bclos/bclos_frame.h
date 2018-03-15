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

#ifndef BCLOS_FRAME_H
#define BCLOS_FRAME_H

#include "bcore_hmap_tp_sr.h"

/** A frame represents the environment for a closure, analogous to a stack frame for a function.
 *  A closure can create and destroy a frame on the fly while executing the closure function.
 */

typedef struct bclos_frame_s bclos_frame_s;
typedef struct bclos_frame_s
{
    aware_t _;
    bclos_frame_s* external; // not owned by frame
    bcore_hmap_tp_sr_s map;
} bclos_frame_s;

void           bclos_frame_s_init( bclos_frame_s* o );
void           bclos_frame_s_down( bclos_frame_s* o );
void           bclos_frame_s_copy( bclos_frame_s* o, const bclos_frame_s* src );
bclos_frame_s* bclos_frame_s_create();
bclos_frame_s* bclos_frame_s_clone( const bclos_frame_s* o );
void           bclos_frame_s_discard( bclos_frame_s* o );

sr_s*          bclos_frame_s_get(    const bclos_frame_s* o, tp_t name ); // returns NULL when not existing, searches first local than external scope
sr_s*          bclos_frame_s_set(          bclos_frame_s* o, tp_t name, sr_s val );
sr_s           bclos_frame_s_remove(       bclos_frame_s* o, tp_t name ); // removes element, returns value; call sr_down( remove(...) ) to eliminate entry
void           bclos_frame_s_clear( bclos_frame_s* o ); // clears map but not external

sr_s*          bclos_frame_s_get_local( const bclos_frame_s* o, tp_t name ); // returns NULL when not existing, searches only local scope

/**********************************************************************************************************************/

/// An address is a name intended to address an element of a frame
typedef struct bclos_address_s { tp_t name; } bclos_address_s;
BCORE_DECLARE_FUNCTIONS_OBJ( bclos_address_s )

bclos_address_s* bclos_address_s_create_tp( tp_t name );

vd_t bclos_frame_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BCLOS_FRAME_H

