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

/** SIMD Operations
 *  Collection of SIMD enhanced functions with numerically identical fallback.
 */

#ifndef BMATH_SIMD_H
#define BMATH_SIMD_H

#include "bcore_std.h"
#include "bmath_hwflags.h"
#include "bmath_leaf.h"

typedef struct bmath_grt_f3_s bmath_grt_f3_s;

//----------------------------------------------------------------------------------------------------------------------

/// givens row rotation
void bmath_simd_f3_row_rotate( f3_t* v1, f3_t* v2, sz_t size, const bmath_grt_f3_s* grt );

/// givens column rotation
void bmath_simd_f3_col_rotate( f3_t* v1, f3_t* v2, sz_t stride, sz_t size, const bmath_grt_f3_s* grt );

/// see bmath_mf3_s_drow_swipe_rev
void bmath_simd_f3_drow_swipe_rev( f3_t* row, const bmath_grt_f3_s* grt, sz_t size );

/// bmath_simd_f3_drow_swipe_rev for 4 successive rows
void bmath_simd_f3_4drow_swipe_rev( f3_t* row, sz_t stride, const bmath_grt_f3_s* grt, sz_t size );

//----------------------------------------------------------------------------------------------------------------------

vd_t bmath_simd_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_SIMD_H
