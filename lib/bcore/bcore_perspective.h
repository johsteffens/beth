/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

/** Perspective framework
 *  The perspective establishes abstraction on via reflection and specified features.
 *  It is an approach to generalize functionality and can be seen as a hybrid of
 *  generic programming and abstract interfacing.
 *  Were appropriate we abbreviate 'perspective' as 'spect'.
 */

#ifndef BCORE_PERSPECTIVE_H
#define BCORE_PERSPECTIVE_H

#include "bcore_first.h"
#include "bcore_types.h"
#include "bcore_features.h"

// perspectives define this structure as first element
typedef struct bcore_perspective_s
{
    u2_t p_type; // type of perspective
    u2_t o_type; // type of object
    fp_t p_down; // down function of perspective
} bcore_perspective_s;

void bcore_perspective_s_init( bcore_perspective_s* o, fp_t i_down );
void bcore_perspective_s_down( bcore_perspective_s* o );
void bcore_perspective_s_discard( bcore_perspective_s* o );

void bcore_perspective_open();
void bcore_perspective_enroll( u2_t i_type, u2_t o_type, bcore_perspective_s* perspective );
const bcore_perspective_s* bcore_perspective_try_perspective( u2_t i_type, u2_t o_type );
const bcore_perspective_s* bcore_perspective_get_perspective( u2_t i_type, u2_t o_type );
void bcore_perspective_close();

#endif // BCORE_PERSPECTIVE_H
