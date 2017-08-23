/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCORE_REF_H
#define BCORE_REF_H

/// Smart references

#include "bcore_types.h"

// Smart reference framework
// Any function receiving a sr_s by value must terminate it or return it.
// (Excemted are immediate sr-control functions sr_*.)
// A sr_s is terminated by passing it to another function by value; function taking a reference of sr_s do not terminate it.
// After termination the content sr_s is deemed invalid.

// In principle sr_s could be extended to support reference counting.

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
typedef struct bcore_life_s bcore_life_s;
const bcore_inst_s* bcore_inst_s_get_typed(   tp_t type );
void bcore_inst_discard( sr_s o );
vc_t ch_spect( vc_t p, tp_t spect_type );

static inline sr_s sr_null(                                             ) { return ( sr_s ){ .o = NULL, .p = NULL, .f = 0                                }; }
static inline sr_s sr_pocs( vc_t p, vd_t o, bl_t const_f, bl_t strong_f ) { return ( sr_s ){ .o = o, .p = p, .f = ( const_f * C_f ) | ( strong_f * S_f ) }; }

static inline sr_s sr_twc( tp_t t, vc_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = t ? bcore_inst_s_get_typed( t )         : NULL, .f = C_f }; }
static inline sr_s sr_twd( tp_t t, vd_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = t ? bcore_inst_s_get_typed( t )         : NULL, .f = 0   }; }
static inline sr_s sr_tsd( tp_t t, vd_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = t ? bcore_inst_s_get_typed( t )         : NULL, .f = S_f }; }
static inline sr_s sr_awc(         vc_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = o ? bcore_inst_s_get_typed( *(tp_t*)o ) : NULL, .f = C_f }; }
static inline sr_s sr_awd(         vd_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = o ? bcore_inst_s_get_typed( *(tp_t*)o ) : NULL, .f = 0   }; }
static inline sr_s sr_asd(         vd_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = o ? bcore_inst_s_get_typed( *(tp_t*)o ) : NULL, .f = S_f }; }
static inline sr_s sr_pwc( vc_t p, vc_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = p                                             , .f = C_f }; }
static inline sr_s sr_pwd( vc_t p, vd_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = p                                             , .f = 0   }; }
static inline sr_s sr_psd( vc_t p, vd_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = p                                             , .f = S_f }; }

static inline sr_s sr_cw( sr_s o ) { o.f &= ~S_f; return o; } // turns a reference into a weak one;
static inline sr_s sr_cc( sr_s o ) { o.f &= ~C_f; return o; } // turns a reference into a const one;
static inline sr_s sr_cp( sr_s o, tp_t spect_type ) { o.p = ch_spect( o.p, spect_type ); return o; } // changes perspective
              sr_s sr_cl( sr_s o, bcore_life_s* l ); // assigns to lifetime manager

static inline sr_s sr_fork( sr_s o ) { o.f &= ~S_f; return o;              } // forks a reference; returned reference is weak; does not terminate o
static inline tp_t sr_type( sr_s o ) { return o.p ? ( (tp_t*)o.p )[1] : 0; } // returns type; does not terminate o

static inline void sr_down( sr_s o ) { if( o.f & S_f ) bcore_inst_discard( o ); }  // explicit termination

static inline bl_t sr_is_strong( sr_s o ) { return ( o.f & S_f ) ? true : false; }
static inline bl_t sr_is_const(  sr_s o ) { return ( o.f & C_f ) ? true : false; }


#define TYPEOF_sr( sr ) ( sr.p ? ( ( tp_t* )sr.p )[ 1 ] : 0 )

#endif // BCORE_REF_H
