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

typedef char*       sd_t; // dynamic zero terminated string
typedef const char* sc_t; // const zero terminated string
typedef void*       vd_t; // dynamic void pointer
typedef const void* vc_t; // const void pointer
typedef void*       nd_t; // pointer to a nucleus (signature of an amoeba)

typedef void (*fp_t)();   // general function pointer (also for functions with different signature)
typedef void (*ap_t)( nd_t nc ); // amoebal function pointer

// types for special purposes
typedef u2_t tp_t;    // object type identifier
typedef tp_t aware_t; // self-awareness type (first element in self-aware object)
typedef struct { vd_t o; tp_t t; } dt_p; // pair of object reference and type
typedef struct { vc_t o; tp_t t; } ct_p; // pair of const object reference and type

// extended scan and print format specifiers
#define PRIs3_t PRIi64
#define PRIs2_t PRIi32
#define PRIs1_t PRIi16
#define PRIs0_t PRIi8
#define PRIu3_t PRIu64
#define PRIu2_t PRIu32
#define PRIu1_t PRIu16
#define PRIu0_t PRIu8
#define PRIf3_t "lg"
#define PRIf2_t "g"
#define PRIsz_t "zu"
#define PRItp_t PRIu32

#endif // BCORE_TYPE_H

