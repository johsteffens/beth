/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCORE_REF_H
#define BCORE_REF_H

/// Smart references

#include "bcore_types.h"

// Smart reference framework
// Structure sr_s can be used by reference as object or as nested value.
// Functions for nested usage have the form sr_<name>
// Functions for referenced usage have the canonic form sr_s_<name>
//
// Any function receiving a 'strong' sr_s by value must terminate it or return it.
// Termination may formally be done for any state to be sure capturing the strong ones.
// A sr_s is terminated by passing it to another function by value;
// Note: A few exceptions of the form sr_... never terminate the reference.

// After termination the content sr_s is deemed invalid if it was a strong reference.
// Termination can be skipped when the reference is 'weak'
// In principle sr_s could be extended to support reference counting.

/// smart perspective based reference
typedef struct
{
    vd_t o; // object
    vc_t p; // perspective
    tp_t f; // flags
} sr_s;

/**********************************************************************************************************************/
// embedded usage

#define CONST_f  ((tp_t)1)  // const reference
#define STRONG_f ((tp_t)2)  // strong reference (receiver assumes responsibility for managing lifetime)

typedef struct bcore_inst_s bcore_inst_s;
typedef struct bcore_life_s bcore_life_s;
const bcore_inst_s* bcore_inst_s_get_typed(    tp_t type );
vd_t bcore_inst_typed_create( tp_t type );
void bcore_inst_discard( sr_s o );
vc_t ch_spect( vc_t p, tp_t spect_type );

static inline sr_s sr_null(                                             ) { return ( sr_s ){ .o = NULL, .p = NULL, .f = 0                                }; }
static inline sr_s sr_pocs( vc_t p, vd_t o, bl_t const_f, bl_t strong_f ) { return ( sr_s ){ .o = o, .p = p, .f = ( const_f * CONST_f ) | ( strong_f * STRONG_f ) }; }

static inline sr_s sr_twc( tp_t t, vc_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = t ? bcore_inst_s_get_typed( t )         : NULL, .f = CONST_f  }; }
static inline sr_s sr_twd( tp_t t, vd_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = t ? bcore_inst_s_get_typed( t )         : NULL, .f = 0        }; }
static inline sr_s sr_tsd( tp_t t, vd_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = t ? bcore_inst_s_get_typed( t )         : NULL, .f = STRONG_f }; }
static inline sr_s sr_awc(         vc_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = o ? bcore_inst_s_get_typed( *(tp_t*)o ) : NULL, .f = CONST_f  }; }
static inline sr_s sr_awd(         vd_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = o ? bcore_inst_s_get_typed( *(tp_t*)o ) : NULL, .f = 0        }; }
static inline sr_s sr_asd(         vd_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = o ? bcore_inst_s_get_typed( *(tp_t*)o ) : NULL, .f = STRONG_f }; }
static inline sr_s sr_pwc( vc_t p, vc_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = p                                             , .f = CONST_f  }; }
static inline sr_s sr_pwd( vc_t p, vd_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = p                                             , .f = 0        }; }
static inline sr_s sr_psd( vc_t p, vd_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = p                                             , .f = STRONG_f }; }

static inline sr_s sr_cw( sr_s o ) { o.f &= ~STRONG_f; return o; } // turns a reference into a weak one;
static inline sr_s sr_cc( sr_s o ) { o.f &= ~CONST_f; return o; } // turns a reference into a const one;
static inline sr_s sr_cp( sr_s o, tp_t spect_type ) { o.p = ch_spect( o.p, spect_type ); return o; } // changes perspective
              sr_s sr_cl( sr_s o, bcore_life_s* l ); // assigns to lifetime manager

//static inline sr_s sr_fork( sr_s o ) { o.f &= ~STRONG_f; return o;         } // forks a reference; returned reference is weak; does not terminate o
//static inline tp_t sr_type( sr_s o ) { return o.p ? ( (tp_t*)o.p )[1] : 0; } // returns type; does not terminate o

static inline void sr_down( sr_s o )   { if( o.f & STRONG_f ) bcore_inst_discard( o ); }  // explicit termination

/// creates a strong reference of a typed object (by cloning the object)
static inline sr_s sr_create( tp_t t ) { return sr_tsd( t, bcore_inst_typed_create( t ) ); }
sr_s sr_create_strong_typed( tp_t type, vc_t obj );

/// copies specified leaf objects into a strong reference
sr_s sr_s0( s0_t v );
sr_s sr_s1( s1_t v );
sr_s sr_s2( s2_t v );
sr_s sr_s3( s3_t v );
sr_s sr_u0( u0_t v );
sr_s sr_u1( u1_t v );
sr_s sr_u2( u2_t v );
sr_s sr_u3( u3_t v );
sr_s sr_f2( f2_t v );
sr_s sr_f3( f3_t v );
sr_s sr_sz( sz_t v );
sr_s sr_tp( tp_t v );
sr_s sr_bl( bl_t v );

sr_s sr_string_sc( sc_t v ); // converts to bcore_string_s

/**********************************************************************************************************************/
// object usage

void  sr_s_init(          sr_s* o );
void  sr_s_down(          sr_s* o );
void  sr_s_copy(          sr_s* o, const sr_s* src ); // deep copy:  (clones the object if present; becomes a strong reference; perspective is retained)
sr_s* sr_s_create(                );
sr_s* sr_s_clone(   const sr_s* o );                  // deep clone: (clones the object if present; returns a strong reference; perspective is retained)
void  sr_s_discard(       sr_s* o );

static inline tp_t sr_s_type(      const sr_s* o ) { return o ? ( o->p ? ( (tp_t*)o->p )[1] : 0 ) : 0; }
static inline bl_t sr_s_is_weak(   const sr_s* o ) { return ( o->f & STRONG_f ) ? false : true;  }
static inline bl_t sr_s_is_strong( const sr_s* o ) { return ( o->f & STRONG_f ) ? true  : false; }
static inline bl_t sr_s_is_const(  const sr_s* o ) { return ( o->f & CONST_f  ) ? true  : false; }

static inline void sr_s_set_strong( sr_s* o, bl_t flag ) { o->f = flag ? ( o->f | STRONG_f ) : ( o->f & ~STRONG_f ); }
static inline void sr_s_set_const(  sr_s* o, bl_t flag ) { o->f = flag ? ( o->f | CONST_f  ) : ( o->f & ~CONST_f  ); }

static inline void sr_s_clear( sr_s* o ) { if( o ) { sr_down( *o ); o->o = NULL; o->p = NULL; o->f = 0; } }
static inline void sr_s_set(   sr_s* o, sr_s src ) { sr_s_clear( o ); if( sr_s_is_strong( &src ) ) { *o = src; } else { sr_s_copy( o, &src ); sr_down( src ); } }
static inline sr_s sr_s_get(   sr_s* o )           { return sr_cw( *o ); }

/**********************************************************************************************************************/

vd_t bcore_ref_signal( tp_t target, tp_t signal, vd_t object );

#endif // BCORE_REF_H
