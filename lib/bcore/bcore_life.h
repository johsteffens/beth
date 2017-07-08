/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCORE_LIFE_H
#define BCORE_LIFE_H

#include "bcore_features.h"

/** Simple lifetime manager for objects.
 *  bcore_life assumes ownership of objects with respect to managing their lifetime.
 *  Objects remain alive until the instance of bcore_life terminates.
 */

typedef struct bcore_life_item_s
{
    fp_t discard;
    tp_t type;  // 0 for untyped objects
    vd_t object;
} bcore_life_item_s;

typedef struct bcore_life_s
{
    aware_t _;
    bcore_life_item_s* data;
    sz_t size; sz_t space;
} bcore_life_s;

void                 bcore_life_s_init(      bcore_life_s* o );
void                 bcore_life_s_down(      bcore_life_s* o );
struct bcore_life_s* bcore_life_s_create();
void                 bcore_life_s_discard(   bcore_life_s* o );

vd_t bcore_life_s_push(         bcore_life_s* o, fp_t discard, vd_t object ); // explicit discard
vd_t bcore_life_s_push_typed(   bcore_life_s* o, tp_t type,    vd_t object ); // discard via bcore_inst_typed_discard
vd_t bcore_life_s_push_aware(   bcore_life_s* o,               vd_t object ); // object is aware
vd_t bcore_life_s_push_free(    bcore_life_s* o,               vd_t object ); // uses bcore_free as discard function
vd_t bcore_life_s_typed_create( bcore_life_s* o, tp_t type                 ); // creates new object and manages its lifetime

/// self reflection
typedef struct bcore_flect_self_s bcore_flect_self_s;
bcore_flect_self_s* bcore_life_s_create_self();

#endif // BCORE_LIFE_H
