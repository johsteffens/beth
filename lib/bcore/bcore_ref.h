/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCORE_REF_H
#define BCORE_REF_H

/// Smart references

#include "bcore_types.h"

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


// Smart reference framework
// Any function receiving a sr_s by value must terminate it.
// A sr_s is terminated by passing it to another function by value (except sr_fork, sr_type); function taking a reference of sr_s do not terminate it
// After termination the instance of sr_s is invalid.
// As long as there is no reference counting, the code must ensure that a strong reference does not terminate while its forks are in use.

/// smart perspective based reference
typedef struct
{
    vd_t o; // object
    vc_t p; // perspective
    tp_t f; // flags
} sr_s;

#define C_f ((tp_t)1)  // const reference
#define S_f ((tp_t)2)  // strong reference (receiver assumes responsibility for managing lifetime)

typedef struct bcore_inst_s bcore_inst_s;
const bcore_inst_s* bcore_inst_s_get_typed(   tp_t type );
void bcore_inst_discard( sr_s o );
vc_t ch_spect( vc_t p, tp_t spect_type );

static inline sr_s sr_nul(                ) { return ( sr_s ){ .o = NULL, .p = NULL, .f = 0                                          }; }
static inline sr_s sr_twc( tp_t t, vc_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = t ? bcore_inst_s_get_typed( t )         : NULL, .f = C_f }; }
static inline sr_s sr_twd( tp_t t, vd_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = t ? bcore_inst_s_get_typed( t )         : NULL, .f = 0   }; }
static inline sr_s sr_tsd( tp_t t, vd_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = t ? bcore_inst_s_get_typed( t )         : NULL, .f = S_f }; }
static inline sr_s sr_awc(         vc_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = o ? bcore_inst_s_get_typed( *(tp_t*)o ) : NULL, .f = C_f }; }
static inline sr_s sr_awd(         vd_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = o ? bcore_inst_s_get_typed( *(tp_t*)o ) : NULL, .f = 0   }; }
static inline sr_s sr_asd(         vd_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = o ? bcore_inst_s_get_typed( *(tp_t*)o ) : NULL, .f = S_f }; }
static inline sr_s sr_pwc( vc_t p, vc_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = p                                             , .f = C_f }; }
static inline sr_s sr_pwd( vc_t p, vd_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = p                                             , .f = 0   }; }
static inline sr_s sr_psd( vc_t p, vd_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = p                                             , .f = S_f }; }

static inline sr_s sr_rf(   rf_s o ) { return ( sr_s ){ .o = ( vd_t )o.o, .p = o.t ? bcore_inst_s_get_typed( o.t ) : NULL, .f = ( o.c ? C_f : 0 ) | ( o.s ? S_f : 0 ) }; }
static inline sr_s sr_fork( sr_s o ) { o.f &= ~S_f; return o;              } // forks a reference; returned reference is weak; does not terminate o
static inline tp_t sr_type( sr_s o ) { return o.p ? ( (tp_t*)o.p )[1] : 0; } // returns type; does not terminate o

static inline void sr_down( sr_s o ) { if( o.f & S_f ) bcore_inst_discard( o ); }  // explicit termination

#define TYPEOF_sr( sr ) ( sr.p ? ( ( tp_t* )sr.p )[ 1 ] : 0 )

#endif // BCORE_REF_H
