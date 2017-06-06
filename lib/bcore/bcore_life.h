/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCORE_LIFE_H
#define BCORE_LIFE_H

#include "bcore_features.h"

/** Simple lifetime manager for (heap-) objects
 *  bcore_life assumes ownership to objects
 *  with respect to managing their lifetime.
 *  Objects added are deemed to remain alive until
 *  the instance of bcore_life goes down.
 */

typedef struct bcore_life_item_s
{
    fp_t discard;
    void* object;
} bcore_life_item_s;

typedef struct bcore_life_s
{
    bcore_life_item_s* data;
    sz_t size; sz_t space;
} bcore_life_s;

void                 bcore_life_s_init(      bcore_life_s* o );
void                 bcore_life_s_down(      bcore_life_s* o );
struct bcore_life_s* bcore_life_s_create();
void                 bcore_life_s_discard(   bcore_life_s* o );
void*                bcore_life_s_push(      bcore_life_s* o, fp_t discard, void* object );
void*                bcore_life_s_push_free( bcore_life_s* o, void* object ); // uses bcore_free as discard function

#endif // BCORE_LIFE_H
