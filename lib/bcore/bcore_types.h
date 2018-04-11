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

#ifndef BCORE_TYPES_H
#define BCORE_TYPES_H

#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>
#include <float.h>
#include <stdbool.h>
#include <stdatomic.h>

typedef float  flt32_t;
typedef double flt64_t;

// leaf types
typedef int64_t     s3_t;
typedef int32_t     s2_t;
typedef int16_t     s1_t;
typedef int8_t      s0_t;
typedef uint64_t    u3_t;
typedef uint32_t    u2_t;
typedef uint16_t    u1_t;
typedef uint8_t     u0_t;
typedef flt64_t     f3_t;
typedef flt32_t     f2_t;
typedef size_t      sz_t;
typedef bool        bl_t;

typedef uintmax_t   umax_t;
typedef intmax_t    smax_t;

typedef char*       sd_t; // dynamic zero terminated string
typedef const char* sc_t; // const zero terminated string
typedef void*       vd_t; // dynamic void pointer
typedef const void* vc_t; // const void pointer
typedef void*       nd_t; // pointer to a nucleus (signature of an amoeba)

/** General function pointer (also for functions with different signature)
 *  Function pointers need to be casted to the right signature before use.
 *  We avoid a definition like void (*fp_t)() (old style arguments)
 *  because old style calls can cause undesired/unexpected (platform specific) argument
 *  propagation.
 */
typedef void (*fp_t)( void );

// types for special purposes
typedef u2_t tp_t;    // object type identifier
typedef tp_t aware_t;  // self-awareness type (first element in self-aware object)
typedef sz_t offset_t; // used as address offsets into an object

// types used for atomic operations
typedef _Atomic tp_t atomic_tp_t;
typedef _Atomic vc_t atomic_vc_t;

// extended scan and print format specifiers
#define PRIs0_t PRIi8
#define PRIs1_t PRIi16
#define PRIs2_t PRIi32
#define PRIs3_t PRIi64
#define PRIsmax_t PRIiMAX

#define PRIu0_t PRIu8
#define PRIu1_t PRIu16
#define PRIu2_t PRIu32
#define PRIu3_t PRIu64
#define PRIumax_t PRIuMAX

#define PRIf3_t "lg"
#define PRIf2_t "g"
#define PRIsz_t "zu"
#define PRIoffset_t "zu"
#define PRItp_t PRIu32

/// special purpose specifiers (do not use for other purposes)
// NPX  - used locally as name prefix in *.c files (e.g. #define NPX(name) bcore_translator_##name  ... #undef NPX)
// st_s - reserved for strings

#endif // BCORE_TYPES_H

