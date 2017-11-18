/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

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

sr_s*          bclos_frame_s_get(    bclos_frame_s* o, tp_t name ); // returns NULL when not existing
sr_s*          bclos_frame_s_set(    bclos_frame_s* o, tp_t name, sr_s val );
sr_s           bclos_frame_s_remove( bclos_frame_s* o, tp_t name ); // removes element, returns value; call sr_down( remove(...) ) to eliminate entry

/**********************************************************************************************************************/

/// An address is a name intended to address an element of a frame
typedef struct bclos_address_s { tp_t name; } bclos_address_s;
DECLARE_FUNCTIONS_OBJ( bclos_address_s )

bclos_address_s* bclos_address_s_create_tp( tp_t name );

vd_t bclos_frame_signal( tp_t target, tp_t signal, vd_t object );

/**********************************************************************************************************************/

#endif // BCLOS_FRAME_H

