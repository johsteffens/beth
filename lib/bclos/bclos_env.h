/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCLOS_ENV_H
#define BCLOS_ENV_H

#include "bcore_hmap_tp_sr.h"

/// Environment of a closure

typedef struct bclos_env_s bclos_env_s;
typedef struct bclos_env_s
{
    aware_t _;
    bclos_env_s* external; // not owned by env
    bcore_hmap_tp_sr_s map;
} bclos_env_s;

void         bclos_env_s_init( bclos_env_s* o );
void         bclos_env_s_down( bclos_env_s* o );
void         bclos_env_s_copy( bclos_env_s* o, const bclos_env_s* src );
bclos_env_s* bclos_env_s_create();
bclos_env_s* bclos_env_s_clone( const bclos_env_s* o );
void         bclos_env_s_discard( bclos_env_s* o );

sr_s*        bclos_env_s_get(    bclos_env_s* o, tp_t name ); // returns NULL when not existing
sr_s*        bclos_env_s_set(    bclos_env_s* o, tp_t name, sr_s val );
sr_s         bclos_env_s_remove( bclos_env_s* o, tp_t name ); // removes element, returns value; call sr_down( remove(...) ) to eliminate entry

void bclos_env_define_self_creators( void );
vd_t bclos_env_signal( tp_t target, tp_t signal, vd_t object );

/**********************************************************************************************************************/

#endif // BCLOS_ENV_H

