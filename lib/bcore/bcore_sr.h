/** Author and Copyright 2017 Johannes Bernhard Steffens
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

#ifndef BCORE_SR_H
#define BCORE_SR_H

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

// After termination, the content sr_s is deemed invalid if it was a strong reference.
// Termination can be skipped when the reference is 'weak'

/// smart perspective based reference
typedef struct
{
    vd_t o; // object
    vc_t p; // perspective
    tp_t f; // flags
} sr_s;

/**********************************************************************************************************************/
// forward declarations

typedef struct bcore_inst_s bcore_inst_s;
typedef struct bcore_life_s bcore_life_s;

vc_t bcore_spect_get_typed( tp_t p_type, tp_t o_type );

const bcore_inst_s* bcore_inst_s_get_typed_wrap( tp_t type );
vd_t bcore_inst_t_create_wrap( tp_t type );
vd_t bcore_inst_p_create_wrap( const bcore_inst_s* p );

void bcore_inst_x_discard_wrap( sr_s o );
vc_t ch_spect_p( vc_t p, tp_t spect_type );
vd_t bcore_fork( vd_t ptr );

/**********************************************************************************************************************/
// embedded usage

#define CONST_f  ((tp_t)1)  // const reference
#define STRONG_f ((tp_t)2)  // strong reference (receiver assumes responsibility for managing lifetime)

static inline sr_s sr_null(                                             ) { return ( sr_s ){ .o = NULL, .p = NULL, .f = 0 }; }
static inline sr_s sr_pocs( vc_t p, vd_t o, bl_t const_f, bl_t strong_f ) { return ( sr_s ){ .o = o, .p = p, .f = ( const_f * CONST_f ) | ( strong_f * STRONG_f ) }; }

static inline sr_s sr_twc( tp_t t, vc_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = t ? bcore_inst_s_get_typed_wrap( t )         : NULL, .f = CONST_f  }; }
static inline sr_s sr_twd( tp_t t, vd_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = t ? bcore_inst_s_get_typed_wrap( t )         : NULL, .f = 0        }; }
static inline sr_s sr_tsd( tp_t t, vd_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = t ? bcore_inst_s_get_typed_wrap( t )         : NULL, .f = STRONG_f }; }
static inline sr_s sr_awc(         vc_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = o ? bcore_inst_s_get_typed_wrap( *(tp_t*)o ) : NULL, .f = CONST_f  }; }
static inline sr_s sr_awd(         vd_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = o ? bcore_inst_s_get_typed_wrap( *(tp_t*)o ) : NULL, .f = 0        }; }
static inline sr_s sr_asd(         vd_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = o ? bcore_inst_s_get_typed_wrap( *(tp_t*)o ) : NULL, .f = STRONG_f }; }
static inline sr_s sr_qwc(         vc_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = *(vc_t*)o , .f = CONST_f  }; }
static inline sr_s sr_qwd(         vd_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = *(vc_t*)o , .f = 0        }; }
static inline sr_s sr_qsd(         vd_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = *(vc_t*)o , .f = STRONG_f }; }
static inline sr_s sr_pwc( vc_t p, vc_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = p         , .f = CONST_f  }; }
static inline sr_s sr_pwd( vc_t p, vd_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = p         , .f = 0        }; }
static inline sr_s sr_psd( vc_t p, vd_t o ) { return ( sr_s ){ .o = ( vd_t )o, .p = p         , .f = STRONG_f }; }

static inline sr_s sr_cw( sr_s o ) { o.f &= ~STRONG_f; return o; } // turns a reference into a weak one;
static inline sr_s sr_cc( sr_s o ) { o.f |=  CONST_f; return o; } // turns a reference into a const one;
static inline sr_s sr_cp( sr_s o, tp_t spect_type ) { o.p = ch_spect_p( o.p, spect_type ); return o; } // changes perspective
              sr_s sr_cl( sr_s o, bcore_life_s* l ); // assigns to lifetime manager

// returns a strong reference; terminates o
static inline sr_s sr_fork( sr_s o ) { return ( sr_s ) { .o = ( o.f & STRONG_f ) ? o.o : bcore_fork( o.o ), .p = o.p, .f = o.f | STRONG_f }; }

// deep clone
sr_s sr_clone( sr_s o );

static inline void sr_down( sr_s o ) { if( o.f & STRONG_f ) bcore_inst_x_discard_wrap( o ); }  // explicit termination

/// creates a new instance
static inline sr_s sr_create( tp_t t ) { return sr_tsd( t, bcore_inst_t_create_wrap( t ) ); } // sames as sr_t_create

static inline
sr_s sr_t_create( tp_t t ) { return sr_tsd( t, bcore_inst_t_create_wrap( t ) ); }
sr_s sr_p_create( vc_t p );

/** XOILA-versions of t_create and p_create */
static inline sr_s sr_create_from_type( tp_t type ) { return sr_tsd( type, bcore_inst_t_create_wrap( type ) ); }
static inline sr_s sr_create_from_spect( vc_t spect ) { return sr_p_create( spect ); }

/// creates a strong reference of a typed object (by cloning the object)
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
sr_s sr_uz( uz_t v );
sr_s sr_tp( tp_t v );
sr_s sr_bl( bl_t v );

sr_s sr_string_sc( sc_t v ); // converts to st_s

/**********************************************************************************************************************/
// object usage

void  sr_s_init(          sr_s* o );
void  sr_s_down(          sr_s* o );
void  sr_s_copy(          sr_s* o, const sr_s* src ); // deep copy:  (clones the object if present; becomes a strong reference; perspective is retained)
sr_s* sr_s_create(                );
sr_s* sr_s_clone(   const sr_s* o );                  // deep clone: (clones the object if present; returns a strong reference; perspective is retained)
void  sr_s_discard(       sr_s* o );

static inline sr_s* sr_s_twc( sr_s* o, tp_t t, vc_t b ) { sr_s_down( o ); *o = sr_twc( t, b ); return o; }
static inline sr_s* sr_s_twd( sr_s* o, tp_t t, vd_t b ) { sr_s_down( o ); *o = sr_twd( t, b ); return o; }
static inline sr_s* sr_s_tsd( sr_s* o, tp_t t, vd_t b ) { sr_s_down( o ); *o = sr_tsd( t, b ); return o; }
static inline sr_s* sr_s_awc( sr_s* o,         vc_t b ) { sr_s_down( o ); *o = sr_awc(    b ); return o; }
static inline sr_s* sr_s_awd( sr_s* o,         vd_t b ) { sr_s_down( o ); *o = sr_awd(    b ); return o; }
static inline sr_s* sr_s_asd( sr_s* o,         vd_t b ) { sr_s_down( o ); *o = sr_asd(    b ); return o; }
static inline sr_s* sr_s_qwc( sr_s* o,         vc_t b ) { sr_s_down( o ); *o = sr_qwc(    b ); return o; }
static inline sr_s* sr_s_qwd( sr_s* o,         vd_t b ) { sr_s_down( o ); *o = sr_qwd(    b ); return o; }
static inline sr_s* sr_s_qsd( sr_s* o,         vd_t b ) { sr_s_down( o ); *o = sr_qsd(    b ); return o; }
static inline sr_s* sr_s_pwc( sr_s* o, vc_t p, vc_t b ) { sr_s_down( o ); *o = sr_pwc( p, b ); return o; }
static inline sr_s* sr_s_pwd( sr_s* o, vc_t p, vd_t b ) { sr_s_down( o ); *o = sr_pwd( p, b ); return o; }
static inline sr_s* sr_s_psd( sr_s* o, vc_t p, vd_t b ) { sr_s_down( o ); *o = sr_psd( p, b ); return o; }

static inline tp_t sr_s_p_type( const sr_s* o ) { return o ? ( o->p ? ( (tp_t*)o->p )[0] : 0 ) : 0; }
static inline tp_t sr_s_o_type( const sr_s* o ) { return o ? ( o->p ? ( (tp_t*)o->p )[1] : 0 ) : 0; }
static inline tp_t sr_s_type(   const sr_s* o ) { return sr_s_o_type( o ); }

static inline bl_t sr_s_is_weak(   const sr_s* o ) { return ( o->f & STRONG_f ) ? false : true;  }
static inline bl_t sr_s_is_strong( const sr_s* o ) { return ( o->f & STRONG_f ) ? true  : false; }
static inline bl_t sr_s_is_const(  const sr_s* o ) { return ( o->f & CONST_f  ) ? true  : false; }

/// o references a numeric object
bl_t sr_s_is_numeric( const sr_s* o );
bl_t sr_s_is_float  ( const sr_s* o );
bl_t sr_s_is_integer( const sr_s* o );

static inline void sr_s_set_strong( sr_s* o, bl_t flag ) { o->f = flag ? ( o->f | STRONG_f ) : ( o->f & ~STRONG_f ); }
static inline void sr_s_set_const(  sr_s* o, bl_t flag ) { o->f = flag ? ( o->f | CONST_f  ) : ( o->f & ~CONST_f  ); }

static inline void sr_s_clear( sr_s* o ) { if( o ) { sr_down( *o ); o->o = NULL; o->p = NULL; o->f = 0; } }
static inline void sr_s_set(   sr_s* o, sr_s src ) { sr_s_clear( o ); *o = sr_fork( src ); }
static inline sr_s sr_s_get(   sr_s* o )           { return sr_cw( *o ); }

/// returns perspective: (p_type, sr_s_type( o ))
static inline vc_t sr_s_get_spect( tp_t p_type, const sr_s* o )
{
    if( !o->p ) return NULL;
    if( ((tp_t*)o->p)[0] == p_type ) return o->p;
    return bcore_spect_get_typed( p_type, ((tp_t*)o->p)[1] );
}

/* reference control */

/** Number of (strong) references to the object.
 *  Return value
 *    0: This reference is null
 *    1: Only one strong reference. If this reference is strong, it is the only existing.
 *   >1: Multible strong references exist.
 */
uz_t bcore_references( vc_t ptr );
static inline uz_t sr_s_references( const sr_s* o ) { return o->o ? bcore_references( o->o ) : 0; }

/** Fork seizes ownership via reference control. Even from a weak reference.
 *  This guarantees the lifetime of the object at least for the lifetime of the forked reference.
 *  The original object is always referenced (never copied).
 */
static inline sr_s sr_s_fork( sr_s* o ) { return ( sr_s ) { .o = bcore_fork( o->o ), .p = o->p, .f = o->f | STRONG_f }; }

/// converts from a leaf type
sr_s* sr_s_from_f3( sr_s* o, f3_t v );
sr_s* sr_s_from_u3( sr_s* o, u3_t v );
sr_s* sr_s_from_s3( sr_s* o, s3_t v );
sr_s* sr_s_from_bl( sr_s* o, bl_t v );
sr_s* sr_s_from_tp( sr_s* o, tp_t v );

/// converts from a leaf type and sets o const
sr_s* sr_s_const_from_f3( sr_s* o, f3_t v );
sr_s* sr_s_const_from_u3( sr_s* o, u3_t v );
sr_s* sr_s_const_from_s3( sr_s* o, s3_t v );
sr_s* sr_s_const_from_bl( sr_s* o, bl_t v );
sr_s* sr_s_const_from_tp( sr_s* o, tp_t v );

/// converts a reference to a numeric leaf type (0 in case o is not numeric)
f3_t sr_s_to_f3( const sr_s* o );
u3_t sr_s_to_u3( const sr_s* o );
s3_t sr_s_to_s3( const sr_s* o );
bl_t sr_s_to_bl( const sr_s* o );
tp_t sr_s_to_tp( const sr_s* o );

static inline f3_t sr_to_f3( sr_s o ) { f3_t r = sr_s_to_f3( &o ); sr_down( o ); return r; }
static inline u3_t sr_to_u3( sr_s o ) { u3_t r = sr_s_to_u3( &o ); sr_down( o ); return r; }
static inline s3_t sr_to_s3( sr_s o ) { s3_t r = sr_s_to_s3( &o ); sr_down( o ); return r; }
static inline bl_t sr_to_bl( sr_s o ) { bl_t r = sr_s_to_bl( &o ); sr_down( o ); return r; }
static inline tp_t sr_to_tp( sr_s o ) { tp_t r = sr_s_to_tp( &o ); sr_down( o ); return r; }

/**********************************************************************************************************************/

typedef struct bcore_signal_s bcore_signal_s;
vd_t bcore_ref_signal_handler( const bcore_signal_s* o );

#endif // BCORE_SR_H
