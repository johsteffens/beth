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

#ifndef BCORE_ARR_H
#define BCORE_ARR_H

#include "bcore_flect.h"
#include "bcore_features.h"
#include "bcore_st.h"
#include "bcore_sr.h"

/** Collection of frequently used specific arrays compatible with bcore_array_s
 *  Standard functions are directly implemented.
 */

/**********************************************************************************************************************/

typedef struct bcore_arr_uz_s
{
    aware_t _;
    union
    {
        bcore_array_dyn_solid_static_s arr;
        struct
        {
            uz_t* data;
            uz_t size, space;
        };
    };
} bcore_arr_uz_s;

BCORE_DECLARE_FUNCTIONS_OBJ(    bcore_arr_uz_s )

void bcore_arr_uz_s_clear(      bcore_arr_uz_s* o ); // sets size to zero
void bcore_arr_uz_s_set_space(  bcore_arr_uz_s* o, uz_t space );
void bcore_arr_uz_s_fill(       bcore_arr_uz_s* o, uz_t size, uz_t v ); // creates filled array of size <size>
void bcore_arr_uz_s_step_fill(  bcore_arr_uz_s* o, uz_t v_start, s3_t step, uz_t size ); // creates filled array according to stepping
void bcore_arr_uz_s_push(       bcore_arr_uz_s* o, uz_t v );
uz_t bcore_arr_uz_s_pop(        bcore_arr_uz_s* o );
void bcore_arr_uz_s_sort(       bcore_arr_uz_s* o, s2_t order ); // stable
void bcore_arr_uz_s_reorder(    bcore_arr_uz_s* o, const bcore_arr_uz_s* order );
uz_t bcore_arr_uz_s_find( const bcore_arr_uz_s* o, uz_t start, uz_t end, uz_t v ); // behaves like st_s_find_*

uz_t bcore_arr_uz_s_max(     const bcore_arr_uz_s* o );
uz_t bcore_arr_uz_s_min(     const bcore_arr_uz_s* o );
uz_t bcore_arr_uz_s_idx_max( const bcore_arr_uz_s* o );
uz_t bcore_arr_uz_s_idx_min( const bcore_arr_uz_s* o );

bcore_arr_uz_s* bcore_arr_uz_s_create_random_permutation( u2_t ( *rg )( u2_t ), u2_t seed, uz_t size );

/**********************************************************************************************************************/

typedef struct bcore_arr_u3_s
{
    aware_t _;
    union
    {
        bcore_array_dyn_solid_static_s arr;
        struct
        {
            u3_t* data;
            uz_t size, space;
        };
    };
} bcore_arr_u3_s;

BCORE_DECLARE_FUNCTIONS_OBJ( bcore_arr_u3_s )

void bcore_arr_u3_s_clear(     bcore_arr_u3_s* o ); // sets size to zero
void bcore_arr_u3_s_set_space( bcore_arr_u3_s* o, uz_t space );
void bcore_arr_u3_s_fill(      bcore_arr_u3_s* o, uz_t size, u3_t v ); // creates filled array of size <size>
void bcore_arr_u3_s_step_fill( bcore_arr_u3_s* o, u3_t v_start, u3_t step, uz_t size ); // creates filled array according to stepping
void bcore_arr_u3_s_push(      bcore_arr_u3_s* o, u3_t v );
u3_t bcore_arr_u3_s_pop(       bcore_arr_u3_s* o );
void bcore_arr_u3_s_sort(      bcore_arr_u3_s* o, s2_t order ); // stable
void bcore_arr_u3_s_reorder(   bcore_arr_u3_s* o, const bcore_arr_uz_s* order );

/**********************************************************************************************************************/

typedef struct bcore_arr_tp_s
{
    aware_t _;
    union
    {
        bcore_array_dyn_solid_static_s arr;
        struct
        {
            tp_t* data;
            uz_t size, space;
        };
    };
} bcore_arr_tp_s;

BCORE_DECLARE_FUNCTIONS_OBJ(    bcore_arr_tp_s )

void bcore_arr_tp_s_clear(      bcore_arr_tp_s* o ); // sets size to zero
void bcore_arr_tp_s_set_space(  bcore_arr_tp_s* o, uz_t space );
void bcore_arr_tp_s_fill(       bcore_arr_tp_s* o, uz_t size, tp_t v ); // creates filled array of size <size>
void bcore_arr_tp_s_push(       bcore_arr_tp_s* o, tp_t v );
tp_t bcore_arr_tp_s_pop(        bcore_arr_tp_s* o );
void bcore_arr_tp_s_sort(       bcore_arr_tp_s* o, s2_t order ); // stable
void bcore_arr_tp_s_reorder(    bcore_arr_tp_s* o, const bcore_arr_uz_s* order );
uz_t bcore_arr_tp_s_find( const bcore_arr_tp_s* o, uz_t start, uz_t end, tp_t v ); // behaves like st_s_find_*

/**********************************************************************************************************************/

typedef struct bcore_arr_st_s
{
    aware_t _;
    union
    {
        bcore_array_dyn_link_static_s arr;
        struct
        {
            st_s** data;
            uz_t size, space;
        };
    };
} bcore_arr_st_s;

BCORE_DECLARE_FUNCTIONS_OBJ( bcore_arr_st_s )

void  bcore_arr_st_s_clear(     bcore_arr_st_s* o ); // sets size to zero
void  bcore_arr_st_s_set_space( bcore_arr_st_s* o, uz_t space );
st_s* bcore_arr_st_s_push_st(   bcore_arr_st_s* o, const st_s* st );
st_s* bcore_arr_st_s_push_st_d( bcore_arr_st_s* o, st_s* st );
st_s* bcore_arr_st_s_push_sc(   bcore_arr_st_s* o, sc_t sc );
void  bcore_arr_st_s_pop(       bcore_arr_st_s* o );
void  bcore_arr_st_s_sort(      bcore_arr_st_s* o, s2_t order ); // stable
void  bcore_arr_st_s_reorder(   bcore_arr_st_s* o, const bcore_arr_uz_s* order );

/**********************************************************************************************************************/

// array does not own objects referenced by elements
typedef struct bcore_arr_vd_s
{
    aware_t _;
    union
    {
        bcore_array_dyn_solid_static_s arr;
        struct
        {
            vd_t* data;
            uz_t size, space;
        };
    };
} bcore_arr_vd_s;

BCORE_DECLARE_FUNCTIONS_OBJ( bcore_arr_vd_s )

void bcore_arr_vd_s_clear(      bcore_arr_vd_s* o ); // sets size to zero
void bcore_arr_vd_s_set_space(  bcore_arr_vd_s* o, uz_t space );
void bcore_arr_vd_s_fill(       bcore_arr_vd_s* o, uz_t size, vd_t v ); // creates filled array of size <size>
void bcore_arr_vd_s_push(       bcore_arr_vd_s* o, vd_t v );
vd_t bcore_arr_vd_s_pop(        bcore_arr_vd_s* o );
void bcore_arr_vd_s_reorder(    bcore_arr_vd_s* o, const bcore_arr_uz_s* order );
uz_t bcore_arr_vd_s_find( const bcore_arr_vd_s* o, uz_t start, uz_t end, vd_t v );  // behaves like st_s_find_*

/**********************************************************************************************************************/

// array does not own objects referenced by elements
typedef struct bcore_arr_fp_s
{
    aware_t _;
    union
    {
        bcore_array_dyn_solid_static_s arr;
        struct
        {
            fp_t* data;
            uz_t size, space;
        };
    };
} bcore_arr_fp_s;

BCORE_DECLARE_FUNCTIONS_OBJ( bcore_arr_fp_s )

void bcore_arr_fp_s_clear(      bcore_arr_fp_s* o ); // sets size to zero
void bcore_arr_fp_s_set_space(  bcore_arr_fp_s* o, uz_t space );
void bcore_arr_fp_s_fill(       bcore_arr_fp_s* o, uz_t size, fp_t v ); // creates filled array of size <size>
void bcore_arr_fp_s_push(       bcore_arr_fp_s* o, fp_t v );
fp_t bcore_arr_fp_s_pop(        bcore_arr_fp_s* o );
void bcore_arr_fp_s_reorder(    bcore_arr_fp_s* o, const bcore_arr_uz_s* order );
uz_t bcore_arr_fp_s_find( const bcore_arr_fp_s* o, uz_t start, uz_t end, fp_t v ); // behaves like st_s_find_*


/**********************************************************************************************************************/

typedef struct bcore_arr_sr_s
{
    aware_t _;
    union
    {
        bcore_array_dyn_solid_static_s arr;
        struct
        {
            sr_s* data;
            uz_t size, space;
        };
    };
} bcore_arr_sr_s;

BCORE_DECLARE_FUNCTIONS_OBJ( bcore_arr_sr_s )

void  bcore_arr_sr_s_clear(     bcore_arr_sr_s* o ); // sets size to zero
void  bcore_arr_sr_s_set_space( bcore_arr_sr_s* o, uz_t space );
void  bcore_arr_sr_s_set_size(  bcore_arr_sr_s* o, uz_t size  ); // conservative resize
sr_s* bcore_arr_sr_s_push_sr(   bcore_arr_sr_s* o, sr_s v );
sr_s* bcore_arr_sr_s_push_tp(   bcore_arr_sr_s* o, tp_t type );
sr_s  bcore_arr_sr_s_pop(       bcore_arr_sr_s* o );
void  bcore_arr_sr_s_reorder(   bcore_arr_sr_s* o, const bcore_arr_uz_s* order );

/**********************************************************************************************************************/

vd_t bcore_arr_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BCORE_ARR_H
