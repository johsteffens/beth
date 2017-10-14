/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#ifndef BCORE_ARR_H
#define BCORE_ARR_H

#include "bcore_flect.h"
#include "bcore_features.h"

/** Collection of frequently used specific arrays compatible with bcore_array_s */

/**********************************************************************************************************************/

typedef struct bcore_arr_sz_s
{
    aware_t _;
    union
    {
        bcore_static_array_s arr;
        struct
        {
            sz_t* data;
            sz_t size, space;
        };
    };
} bcore_arr_sz_s;

DECLARE_FUNCTION_INIT(    bcore_arr_sz_s )
DECLARE_FUNCTION_DOWN(    bcore_arr_sz_s )
DECLARE_FUNCTION_COPY(    bcore_arr_sz_s )
DECLARE_FUNCTION_CREATE(  bcore_arr_sz_s )
DECLARE_FUNCTION_DISCARD( bcore_arr_sz_s )
DECLARE_FUNCTION_CLONE(   bcore_arr_sz_s )

void bcore_arr_sz_s_clear(     bcore_arr_sz_s* o ); // sets size to zero
void bcore_arr_sz_s_set_space( bcore_arr_sz_s* o, sz_t space );
void bcore_arr_sz_s_fill(      bcore_arr_sz_s* o, sz_t size, sz_t v ); // creates filled array of size <size>
void bcore_arr_sz_s_step_fill( bcore_arr_sz_s* o, sz_t v_start, s3_t step, sz_t size ); // creates filled array according to stepping
void bcore_arr_sz_s_push(      bcore_arr_sz_s* o, sz_t v );
sz_t bcore_arr_sz_s_pop(       bcore_arr_sz_s* o );
void bcore_arr_sz_s_sort(      bcore_arr_sz_s* o, s2_t order ); // stable
void bcore_arr_sz_s_reorder(   bcore_arr_sz_s* o, const bcore_arr_sz_s* order );

bcore_arr_sz_s* bcore_arr_sz_s_create_random_permutation( u2_t ( *rg )( u2_t ), u2_t seed, sz_t size );

/**********************************************************************************************************************/

vd_t bcore_arr_signal( tp_t target, tp_t signal, vd_t object );

/**********************************************************************************************************************/

#endif // BCORE_ARR_H
