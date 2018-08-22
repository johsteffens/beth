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

/** SIMD Operations
 *  Collection of SIMD enhanced functions with numerically identical fallback.
 */

#ifndef BMATH_SIMD_H
#define BMATH_SIMD_H

#include "bcore_std.h"
#include "bmath_leaf.h"

#include <x86intrin.h>

//---------------------------------------------------------------------------------------------------------------------

f3_t bmath_simd_f3_mul_vec(  const f3_t* v1, const f3_t* v2, sz_t size ); // classic dot product      sum+=v1[i]*v2[i];
f3_t bmath_simd_f3_mul_vec3( const f3_t* v1, const f3_t* v2, const f3_t* v3, sz_t size );          // sum+=v1[i]*v2[i]*v3[i];

//---------------------------------------------------------------------------------------------------------------------

vd_t bmath_simd_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_SIMD_H
