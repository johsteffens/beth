/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCORE_TYPE_H
#define BCORE_TYPE_H

#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>
#include <float.h>
#include <stdbool.h>

typedef float  flt32_t;
typedef double flt64_t;

// primitive types
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
typedef char*       sd_t; // dynamic zero terminated string
typedef const char* sc_t; // const zero terminated string
typedef size_t      sz_t;
typedef void*       vd_t; // dynamic void pointer
typedef const void* vc_t; // const void pointer

typedef void (*fp_t)();   // general function pointer (also for functions with different signature)

// types for special purposes
typedef u2_t tp_t;    // object type identifier
typedef tp_t aware_t; // self-awareness type (first element in self-aware object)

// pairs
typedef struct { u3_t a; u3_t b; } u3u3_p;
typedef struct { u3_t a; vd_t b; } u3vd_p;
typedef struct { u3_t a; sc_t b; } u3sc_p;

#endif // BCORE_TYPE_H

