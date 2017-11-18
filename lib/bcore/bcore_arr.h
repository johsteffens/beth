/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#ifndef BCORE_ARR_H
#define BCORE_ARR_H

#include "bcore_flect.h"
#include "bcore_features.h"
#include "bcore_st.h"

/** Collection of frequently used specific arrays compatible with bcore_array_s
 *  Standard functions are directly implemented.
 */

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

DECLARE_FUNCTIONS_OBJ(    bcore_arr_sz_s )

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

typedef struct bcore_arr_st_s
{
    aware_t _;
    union
    {
        bcore_static_link_array_s arr;
        struct
        {
            st_s** data;
            sz_t size, space;
        };
    };
} bcore_arr_st_s;

DECLARE_FUNCTIONS_OBJ( bcore_arr_st_s )

void  bcore_arr_st_s_clear(     bcore_arr_st_s* o ); // sets size to zero
void  bcore_arr_st_s_set_space( bcore_arr_st_s* o, sz_t space );
st_s* bcore_arr_st_s_push_st(   bcore_arr_st_s* o, const st_s* st );
st_s* bcore_arr_st_s_push_st_d( bcore_arr_st_s* o, st_s* st );
st_s* bcore_arr_st_s_push_sc(   bcore_arr_st_s* o, sc_t sc );
void  bcore_arr_st_s_pop(       bcore_arr_st_s* o );
void  bcore_arr_st_s_sort(      bcore_arr_st_s* o, s2_t order ); // stable
void  bcore_arr_st_s_reorder(   bcore_arr_st_s* o, const bcore_arr_sz_s* order );

/**********************************************************************************************************************/

// array does not own objects referenced by elements
typedef struct bcore_arr_vd_s
{
    aware_t _;
    union
    {
        bcore_static_array_s arr;
        struct
        {
            vd_t* data;
            sz_t size, space;
        };
    };
} bcore_arr_vd_s;

DECLARE_FUNCTIONS_OBJ( bcore_arr_vd_s )

void bcore_arr_vd_s_clear(     bcore_arr_vd_s* o ); // sets size to zero
void bcore_arr_vd_s_set_space( bcore_arr_vd_s* o, sz_t space );
void bcore_arr_vd_s_fill(      bcore_arr_vd_s* o, sz_t size, vd_t v ); // creates filled array of size <size>
void bcore_arr_vd_s_push(      bcore_arr_vd_s* o, vd_t v );
vd_t bcore_arr_vd_s_pop(       bcore_arr_vd_s* o );
void bcore_arr_vd_s_reorder(   bcore_arr_vd_s* o, const bcore_arr_sz_s* order );

/**********************************************************************************************************************/

vd_t bcore_arr_signal( tp_t target, tp_t signal, vd_t object );

/**********************************************************************************************************************/

#endif // BCORE_ARR_H
