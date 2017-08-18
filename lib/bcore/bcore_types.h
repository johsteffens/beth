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

// general purpose reference (e.g. for via perspective)
typedef struct
{
    vd_t o; // object
    tp_t t; // type
    bl_t s; // strong/weak reference (receiver of strong reference owns the object and takes care of its lifetime)
    bl_t c; // const flag
} rf_s;

void bcore_inst_typed_discard( tp_t type, vd_t obj );
static inline rf_s rf_wc(  vc_t o, tp_t t ) { return ( rf_s ){ .o = ( vd_t )o, .t = t, .s = 0, .c = 1 }; } // weak const reference
static inline rf_s rf_wd(  vd_t o, tp_t t ) { return ( rf_s ){ .o = ( vd_t )o, .t = t, .s = 0, .c = 0 }; } // weak reference
static inline rf_s rf_sd(  vd_t o, tp_t t ) { return ( rf_s ){ .o = ( vd_t )o, .t = t, .s = 1, .c = 0 }; } // strong reference
static inline rf_s rf_awc( vc_t o )         { return ( rf_s ){ .o = ( vd_t )o, .t = o?*(aware_t*)o:0, .s = 0, .c = 1 }; } // aware weak const reference
static inline rf_s rf_awd( vd_t o )         { return ( rf_s ){ .o = ( vd_t )o, .t = o?*(aware_t*)o:0, .s = 0, .c = 0 }; } // aware weak reference
static inline rf_s rf_asd( vd_t o )         { return ( rf_s ){ .o = ( vd_t )o, .t = o?*(aware_t*)o:0, .s = 1, .c = 0 }; } // aware strong reference
static inline rf_s rf_null( void )          { return ( rf_s ){ .o = NULL, .t = 0, .s = 0, .c = 1 };  } // NULL reference
static inline void rf_s_down( rf_s o )      { if( o.s && o.o ) bcore_inst_typed_discard( o.t, o.o ); } // discards object in case of strong reference
static inline tp_t rf_s_type( rf_s o )      { rf_s_down( o ); return o.t;                            } // discards object in case of strong reference, returns type

/// smart perspective based reference
typedef struct
{
    vd_t o; // object
    vc_t p; // perspective
    tp_t f; // flags
} sr_r;

#define C_f 1  // const reference
#define S_f 2  // strong reference (receiver assumes responsibility for managing lifetime)

typedef struct bcore_inst_s bcore_inst_s;
const bcore_inst_s* bcore_inst_s_get_typed(   tp_t type );
void                bcore_inst_typed_discard( tp_t type, vd_t obj );
static inline sr_r sr_nul(                ) { return ( sr_r ){ .o = NULL, .p = NULL, .f = 0                                          }; }
static inline sr_r sr_twc( tp_t t, vc_t o ) { return ( sr_r ){ .o = ( vd_t )o, .p = t ? bcore_inst_s_get_typed( t )         : NULL, .f = C_f }; }
static inline sr_r sr_twd( tp_t t, vd_t o ) { return ( sr_r ){ .o = ( vd_t )o, .p = t ? bcore_inst_s_get_typed( t )         : NULL, .f = 0   }; }
static inline sr_r sr_tsd( tp_t t, vd_t o ) { return ( sr_r ){ .o = ( vd_t )o, .p = t ? bcore_inst_s_get_typed( t )         : NULL, .f = S_f }; }
static inline sr_r sr_awc(         vc_t o ) { return ( sr_r ){ .o = ( vd_t )o, .p = o ? bcore_inst_s_get_typed( *(tp_t*)o ) : NULL, .f = C_f }; }
static inline sr_r sr_awd(         vd_t o ) { return ( sr_r ){ .o = ( vd_t )o, .p = o ? bcore_inst_s_get_typed( *(tp_t*)o ) : NULL, .f = 0   }; }
static inline sr_r sr_asd(         vd_t o ) { return ( sr_r ){ .o = ( vd_t )o, .p = o ? bcore_inst_s_get_typed( *(tp_t*)o ) : NULL, .f = S_f }; }
static inline sr_r sr_pwc( vc_t p, vc_t o ) { return ( sr_r ){ .o = ( vd_t )o, .p = p                                             , .f = C_f }; }
static inline sr_r sr_pwd( vc_t p, vd_t o ) { return ( sr_r ){ .o = ( vd_t )o, .p = p                                             , .f = 0   }; }
static inline sr_r sr_psd( vc_t p, vd_t o ) { return ( sr_r ){ .o = ( vd_t )o, .p = p                                             , .f = S_f }; }

#define TYPEOF_sr( sr ) ( sr.p ? ( ( tp_t* )sr.p )[ 1 ] : 0 )

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

/// special purpose specifiers (do not use for other purposes)
// NPX - used locally as name prefix in *.c files (e.g. #define NPX(name) bcore_translator_##name  ... #undef NPX)

#endif // BCORE_TYPE_H

