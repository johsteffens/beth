/** Copyright 2018 Johannes Bernhard Steffens
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

#ifndef BMATH_ARR_VF3_H
#define BMATH_ARR_VF3_H

/**********************************************************************************************************************/

#include "bcore_std.h"
#include "bmath_vf3.h"

/**********************************************************************************************************************/
// array of vf3_s

BCORE_DECLARE_OBJECT( bmath_arr_vf3_s )
{
    aware_t _;
    BCORE_ARRAY_DYN_SOLID_STATIC_S( bmath_vf3_s, );
};

void bmath_arr_vf3_s_clear(      bmath_arr_vf3_s* o ); // sets size to zero
void bmath_arr_vf3_s_set_space(  bmath_arr_vf3_s* o, uz_t space );
void bmath_arr_vf3_s_set_size(   bmath_arr_vf3_s* o, uz_t size );
void bmath_arr_vf3_s_push(       bmath_arr_vf3_s* o, const bmath_vf3_s* vec );
void bmath_arr_vf3_s_push_arr(   bmath_arr_vf3_s* o, const bmath_arr_vf3_s* arr );
void bmath_arr_vf3_s_pop(        bmath_arr_vf3_s* o );
void bmath_arr_vf3_s_reorder(    bmath_arr_vf3_s* o, const bcore_arr_uz_s* order );

/** Following functions apply a bmath_vf3_s operation on consecutive elements indexed start, ..., end-1.
 *  start, end are automatically limited to o->size;
 *  Where res is in use, the same index is applied to o and res.
 *     Requires bounded( end ) <= res->size (tested).
 *     o == res allowed (except where stated otherwise)
 *  end == -1 assumes end == o->size
 */

void bmath_arr_vf3_s_on_section_set_size(      bmath_arr_vf3_s* o, uz_t start, uz_t end, uz_t size );
void bmath_arr_vf3_s_on_section_fill(          bmath_arr_vf3_s* o, uz_t start, uz_t end, f3_t val );
void bmath_arr_vf3_s_on_section_set_random(    bmath_arr_vf3_s* o, uz_t start, uz_t end, f3_t density, f3_t min, f3_t max, u2_t* rval );
void bmath_arr_vf3_s_on_section_zro(           bmath_arr_vf3_s* o, uz_t start, uz_t end );
void bmath_arr_vf3_s_on_section_neg(     const bmath_arr_vf3_s* o, uz_t start, uz_t end,                        bmath_arr_vf3_s* res );
void bmath_arr_vf3_s_on_section_cpy(     const bmath_arr_vf3_s* o, uz_t start, uz_t end,                        bmath_arr_vf3_s* res );
void bmath_arr_vf3_s_on_section_add(     const bmath_arr_vf3_s* o, uz_t start, uz_t end, const bmath_vf3_s* op, bmath_arr_vf3_s* res );
void bmath_arr_vf3_s_on_section_sub(     const bmath_arr_vf3_s* o, uz_t start, uz_t end, const bmath_vf3_s* op, bmath_arr_vf3_s* res );
void bmath_arr_vf3_s_on_section_mul_f3(  const bmath_arr_vf3_s* o, uz_t start, uz_t end, f3_t op,               bmath_arr_vf3_s* res );
void bmath_arr_vf3_s_on_section_set_sqr(       bmath_arr_vf3_s* o, uz_t start, uz_t end, f3_t val );
void bmath_arr_vf3_s_on_section_set_sum(       bmath_arr_vf3_s* o, uz_t start, uz_t end, f3_t val );
void bmath_arr_vf3_s_on_section_set_avg(       bmath_arr_vf3_s* o, uz_t start, uz_t end, f3_t val );
f3_t bmath_arr_vf3_s_on_section_f3_sum_sprec( const bmath_arr_vf3_s* o, uz_t start, uz_t end, uz_t index ); // stochastically precise summing of indexed element

// stochastically precise summing of co-product (vi - ai) * (vj - aj)
f3_t bmath_arr_vf3_s_on_section_f3_sum_coprd_sprec( const bmath_arr_vf3_s* o, uz_t start, uz_t end, f3_t ai, f3_t aj, uz_t i, uz_t j );

void bmath_arr_vf3_s_on_section_get_sum_sprc( const bmath_arr_vf3_s* o, uz_t start, uz_t end, bmath_vf3_s* res ); // stochastically precise summing
void bmath_arr_vf3_s_on_section_get_avg_sprc( const bmath_arr_vf3_s* o, uz_t start, uz_t end, bmath_vf3_s* res ); // stochastically precise averaging
void bmath_arr_vf3_s_on_section_get_sum_fast( const bmath_arr_vf3_s* o, uz_t start, uz_t end, bmath_vf3_s* res ); // fast summing   (loss of accuracy possible)
void bmath_arr_vf3_s_on_section_get_avg_fast( const bmath_arr_vf3_s* o, uz_t start, uz_t end, bmath_vf3_s* res ); // fast averaging (loss of accuracy possible)

static inline void bmath_arr_vf3_s_on_section_get_sum( const bmath_arr_vf3_s* o, uz_t start, uz_t end, bmath_vf3_s* res ) { bmath_arr_vf3_s_on_section_get_sum_sprc( o, start, end, res ); }
static inline void bmath_arr_vf3_s_on_section_get_avg( const bmath_arr_vf3_s* o, uz_t start, uz_t end, bmath_vf3_s* res ) { bmath_arr_vf3_s_on_section_get_avg_sprc( o, start, end, res ); }

/**********************************************************************************************************************/

vd_t bmath_arr_vf3_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_ARR_VF3_H
