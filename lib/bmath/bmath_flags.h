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

#ifndef BMATH_FLAGS_H
#define BMATH_FLAGS_H

/** Compile-time flags controlling optimization strategies,
 *  hardware specific settings (e.g. SIMD), etc.
 *
 */

/**********************************************************************************************************************/

/** User controllable space.
 *  The following flags can be defined/disabled in the compiler environment
 *  to control code generation.
 */

// Disabled explicit use of SIMD Vectorization in bmath code
// #define BMATH_DISABLE_SIMD
// #define BMATH_DISABLE_FMA      // disables use of FMA simd routines
// #define BMATH_STRICT_FALLBACK  // enforces hw-optimized and fallback routines produce binary identical results on identical input

#ifndef BMATH_DISABLE_SIMD
    #define BMATH_SSE2  // forces use of SSE2 intrinsics even when __SSE2__ was not defined
    #define BMATH_AVX   // forces use of AVX intrinsics even when __AVX__ was not defined
    #define BMATH_AVX2  // forces use of AVX2 intrinsics even when __AVX2__ was not defined
#endif // BMATH_DISABLE_SIMD

/**********************************************************************************************************************/

#ifndef BMATH_DISABLE_SIMD

    #ifdef __SSE2__
        #define BMATH_SSE2
    #endif

    #ifdef __AVX__
        #define BMATH_AVX
    #endif

    #ifdef __AVX2__
        #define BMATH_AVX2
    #endif // __AVX2__

    #if defined BMATH_AVX2 && !defined BMATH_DISABLE_FMA && !defined BMATH_STRICT_FALLBACK
        #define BMATH_AVX2_FMA
    #endif
#endif // BMATH_DISABLE_SIMD

/**********************************************************************************************************************/

#if defined BMATH_SSE2 || defined BMATH_AVX || defined BMATH_AVX2
    #include <x86intrin.h>
#endif

/**********************************************************************************************************************/

#endif // BMATH_FLAGS_H

