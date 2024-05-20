/** Author and Copyright 2018 Johannes Bernhard Steffens
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

/**********************************************************************************************************************/

/** Template: Array of Vector */

/**********************************************************************************************************************/

#include "bmath_template_fx_begin.h"

/**********************************************************************************************************************/

void BCATU(bmath_arr_vfx_s,clear)(      bmath_arr_vfx_s* o ); // sets size to zero
void BCATU(bmath_arr_vfx_s,set_space)(  bmath_arr_vfx_s* o, uz_t space );
void BCATU(bmath_arr_vfx_s,set_size)(   bmath_arr_vfx_s* o, uz_t size );
void BCATU(bmath_arr_vfx_s,push)(       bmath_arr_vfx_s* o, const bmath_vfx_s* vec );
void BCATU(bmath_arr_vfx_s,push_arr)(   bmath_arr_vfx_s* o, const bmath_arr_vfx_s* arr );
void BCATU(bmath_arr_vfx_s,pop)(        bmath_arr_vfx_s* o );
void BCATU(bmath_arr_vfx_s,reorder)(    bmath_arr_vfx_s* o, const bcore_arr_uz_s* order );

/** Following functions apply a bmath_vfx_s operation on consecutive elements indexed start, ..., end-1.
 *  start, end are automatically limited to o->size;
 *  Where res is in use, the same index is applied to o and res.
 *     Requires bounded( end ) <= res->size (tested).
 *     o == res allowed (except where stated otherwise)
 *  end == -1 assumes end == o->size
 */

void BCATU(bmath_arr_vfx_s,on_section_set_size)(      bmath_arr_vfx_s* o, uz_t start, uz_t end, uz_t size );
void BCATU(bmath_arr_vfx_s,on_section_fill)(          bmath_arr_vfx_s* o, uz_t start, uz_t end, fx_t val );
void BCATU(bmath_arr_vfx_s,on_section_set_random)(    bmath_arr_vfx_s* o, uz_t start, uz_t end, fx_t density, fx_t min, fx_t max, bcore_prsg* prsg );
void BCATU(bmath_arr_vfx_s,on_section_set_random_u3)( bmath_arr_vfx_s* o, uz_t start, uz_t end, fx_t density, fx_t min, fx_t max, u3_t* rval );
void BCATU(bmath_arr_vfx_s,on_section_zro)(           bmath_arr_vfx_s* o, uz_t start, uz_t end );
void BCATU(bmath_arr_vfx_s,on_section_neg)(     const bmath_arr_vfx_s* o, uz_t start, uz_t end,                        bmath_arr_vfx_s* res );
void BCATU(bmath_arr_vfx_s,on_section_cpy)(     const bmath_arr_vfx_s* o, uz_t start, uz_t end,                        bmath_arr_vfx_s* res );
void BCATU(bmath_arr_vfx_s,on_section_add)(     const bmath_arr_vfx_s* o, uz_t start, uz_t end, const bmath_vfx_s* op, bmath_arr_vfx_s* res );
void BCATU(bmath_arr_vfx_s,on_section_sub)(     const bmath_arr_vfx_s* o, uz_t start, uz_t end, const bmath_vfx_s* op, bmath_arr_vfx_s* res );
void BCATU(bmath_arr_vfx_s,on_section_mul_fx)(  const bmath_arr_vfx_s* o, uz_t start, uz_t end, fx_t op,               bmath_arr_vfx_s* res );
void BCATU(bmath_arr_vfx_s,on_section_set_sqr)(       bmath_arr_vfx_s* o, uz_t start, uz_t end, fx_t val );
void BCATU(bmath_arr_vfx_s,on_section_set_sum)(       bmath_arr_vfx_s* o, uz_t start, uz_t end, fx_t val );
void BCATU(bmath_arr_vfx_s,on_section_set_avg)(       bmath_arr_vfx_s* o, uz_t start, uz_t end, fx_t val );
fx_t BCATU(bmath_arr_vfx_s,on_section_fx_sum_sprec)( const bmath_arr_vfx_s* o, uz_t start, uz_t end, uz_t index ); // stochastically precise summing of indexed element

// stochastically precise summing of co-product (vi - ai) * (vj - aj)
fx_t BCATU(bmath_arr_vfx_s,on_section_fx_sum_coprd_sprec)( const bmath_arr_vfx_s* o, uz_t start, uz_t end, fx_t ai, fx_t aj, uz_t i, uz_t j );

void BCATU(bmath_arr_vfx_s,on_section_get_sum_sprc)( const bmath_arr_vfx_s* o, uz_t start, uz_t end, bmath_vfx_s* res ); // stochastically precise summing
void BCATU(bmath_arr_vfx_s,on_section_get_avg_sprc)( const bmath_arr_vfx_s* o, uz_t start, uz_t end, bmath_vfx_s* res ); // stochastically precise averaging
void BCATU(bmath_arr_vfx_s,on_section_get_sum_fast)( const bmath_arr_vfx_s* o, uz_t start, uz_t end, bmath_vfx_s* res ); // fast summing   (loss of accuracy possible)
void BCATU(bmath_arr_vfx_s,on_section_get_avg_fast)( const bmath_arr_vfx_s* o, uz_t start, uz_t end, bmath_vfx_s* res ); // fast averaging (loss of accuracy possible)

static inline void BCATU(bmath_arr_vfx_s,on_section_get_sum)( const bmath_arr_vfx_s* o, uz_t start, uz_t end, bmath_vfx_s* res ) { BCATU(bmath_arr_vfx_s,on_section_get_sum_sprc)( o, start, end, res ); }
static inline void BCATU(bmath_arr_vfx_s,on_section_get_avg)( const bmath_arr_vfx_s* o, uz_t start, uz_t end, bmath_vfx_s* res ) { BCATU(bmath_arr_vfx_s,on_section_get_avg_sprc)( o, start, end, res ); }

/**********************************************************************************************************************/
// type conversion

er_t BCATU(bmath_arr_vfx_s,copy_typed)( bmath_arr_vfx_s* o, tp_t type, vc_t src );

static inline void BCATU(bmath_arr_vfx_s,copy_t)( bmath_arr_vfx_s* o, tp_t type, vc_t src )
{
    BCATU(bmath_arr_vfx_s,copy_typed)( o, type, src );
}

static inline void BCATU(bmath_arr_vfx_s,copy_a)( bmath_arr_vfx_s* o, vc_t src )
{
    if( src )
    {
        BCATU(bmath_arr_vfx_s,copy_t)( o, *(aware_t*)src, src );
    }
    else
    {
        BCATU(bmath_arr_vfx_s,clear)( o );
    }
}

static inline bmath_arr_vfx_s* BCATU(bmath_arr_vfx_s,clone_t)( tp_t type, vc_t src )
{
    if( !src ) return NULL;
    bmath_arr_vfx_s* o = BCATU(bmath_arr_vfx_s,create)();
    BCATU(bmath_arr_vfx_s,copy_t)( o, type, src );
    return o;
}

static inline bmath_arr_vfx_s* BCATU(bmath_arr_vfx_s,clone_a)( vc_t src )
{
    if( !src ) return NULL;
    bmath_arr_vfx_s* o = BCATU(bmath_arr_vfx_s,create)();
    BCATU(bmath_arr_vfx_s,copy_a)( o, src );
    return o;
}

/**********************************************************************************************************************/

#include "bmath_template_fx_end.h"

/**********************************************************************************************************************/

