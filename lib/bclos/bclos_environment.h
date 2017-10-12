/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#ifndef BCLOS_ENVIRONMENT_H
#define BCLOS_ENVIRONMENT_H

#include "bcore_hmap_tp_sr.h"

/// Environment of a closure

typedef struct bclos_environment_s bclos_environment_s;
typedef struct bclos_environment_s
{
    aware_t _;
    bclos_environment_s* external; // not owned by env
    bcore_hmap_tp_sr_s map;
} bclos_environment_s;

void         bclos_environment_s_init( bclos_environment_s* o );
void         bclos_environment_s_down( bclos_environment_s* o );
void         bclos_environment_s_copy( bclos_environment_s* o, const bclos_environment_s* src );
bclos_environment_s* bclos_environment_s_create();
bclos_environment_s* bclos_environment_s_clone( const bclos_environment_s* o );
void         bclos_environment_s_discard( bclos_environment_s* o );

sr_s*        bclos_environment_s_get(    bclos_environment_s* o, tp_t name ); // returns NULL when not existing
sr_s*        bclos_environment_s_set(    bclos_environment_s* o, tp_t name, sr_s val );
sr_s         bclos_environment_s_remove( bclos_environment_s* o, tp_t name ); // removes element, returns value; call sr_down( remove(...) ) to eliminate entry

vd_t bclos_environment_signal( tp_t target, tp_t signal, vd_t object );

/**********************************************************************************************************************/

#endif // BCLOS_ENVIRONMENT_H

