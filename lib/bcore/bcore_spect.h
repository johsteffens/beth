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

#ifndef BCORE_SPECT_H
#define BCORE_SPECT_H

/** Perspective management framework.
 *  The perspective establishes abstraction on via reflection and specified features.
 *  Perspective instances are managed here.
 *
 *  A perspective needs to satisfy following requirements:
 *    * self-awareness
 *    * object-awareness (value after aware_t must be tp_t of the object's type)
 *    * Providing a reflection
 *    * Registering following features in its reflection:
 *      * bcore_spect_fp_create_from_self
 */

#include "bcore_features.h"
#include "bcore_control.h"
#include "bcore_flect.h"
#include "bcore_tp_fastmap.h"

/**********************************************************************************************************************/

/// perspective-instance creation from object reflections
typedef bcore_self_s* (*bcore_spect_fp_create_from_self )( const bcore_self_s* self );
typedef bl_t          (*bcore_spect_fp_supports         )( const bcore_self_s* self );

/**********************************************************************************************************************/

/// common header of all perspectives
typedef struct bcore_spect_header_s
{
    aware_t p_type; // type of perspective
    tp_t    o_type; // type of object
} bcore_spect_header_s;

BCORE_DECLARE_OBJECT( bcore_spect_header_s )

/**********************************************************************************************************************/


/** Tests if perspective indicated by spect_trait is supported by object o_type. (thread safe)
 *  This function is fast and does not actually construct the perspective.
 *  Returns true if p_type or o_type exist and support each other.
 *  Returns false otherwise.
 */
bl_t bcore_spect_trait_supported( tp_t spect_trait, tp_t o_type );

/** Returns perspective according to perspective and object types. Enrolls automatically if needed. (thread safe)
 *  Error if combination is not supported.
 *  p_type : type of perspective
 *  o_type : type of object
 */
vc_t bcore_spect_get_typed( tp_t p_type, tp_t o_type );

static inline
vc_t bcore_spect_get_aware( tp_t p_type, vc_t o )
{
    return bcore_spect_get_typed( p_type, *( aware_t* )o );
}

/// inits cache and registers it for automatic shut down (used by perspectives)
void bcore_spect_setup_cache( bcore_tp_fastmap_s* cache );

/**********************************************************************************************************************/

/** Generic perspective functions assuming all information is stored in reflections.
 *  These functions make use of bcore_inst_s
 */
void bcore_spect_define_trait(     const bcore_self_s* p_self );
vd_t bcore_spect_create_from_self( const bcore_self_s* p_self, const bcore_self_s* o_self );
vd_t bcore_spect_create_from_self_typed( tp_t p_type, tp_t o_type );

/** Defining reflection of perspective via creation function
 *  This function create a temporary instance of self and calls
 *  bcore_spect_define_trait
 */
void bcore_spect_define_creator( tp_t type, bcore_flect_create_self_fp creator );

/**********************************************************************************************************************/

/// returns status information (thread safe)
st_s* bcore_spect_status();

/**********************************************************************************************************************/

// syntactic sugar
vc_t ch_spect_p( vc_t p, tp_t p_type ); // changes perspective to spect_type; returns NULL if p is NULL
vc_t ch_spect_o( vc_t p, tp_t o_type ); // changes perspective to object_type; returns NULL if p is NULL

static inline tp_t spect_tp_p( vc_t p ) { return ( ( tp_t* )p )[ 0 ]; } // type of perspective
static inline tp_t spect_tp_o( vc_t p ) { return ( ( tp_t* )p )[ 1 ]; } // type of object

vd_t bcore_spect_signal_handler( const bcore_signal_s* o );

/// error condition used by macros below
void bcore_spect_missing_err( vc_t p, sc_t name );

/**********************************************************************************************************************/
// macros

#define BCORE_DEFINE_INLINE_SPECT_GET_TYPED( name ) \
    static inline const name * name##_get_typed( tp_t o_type ) \
    { \
        return bcore_spect_get_typed( TYPEOF_##name, o_type ); \
    }

#define BCORE_DEFINE_SPECT_CACHE( name ) \
    bcore_tp_fastmap_s name##_cache_g

#define BCORE_DEFINE_INLINE_SPECT_GET_TYPED_CACHED( name ) \
    extern bcore_tp_fastmap_s name##_cache_g; \
    static inline \
    const name* name##_get_typed( tp_t o_type ) \
    { \
        vc_t ret = bcore_tp_fastmap_s_get( &name##_cache_g, o_type ); \
        if( ret ) return ret; \
        ret = bcore_spect_get_typed( TYPEOF_##name, o_type ); \
        bcore_tp_fastmap_s_set( &name##_cache_g, o_type, ret ); \
        return ret; \
    }

#define BCORE_DEFINE_INLINE_SPECT_GET_AWARE( name ) \
    static inline const name* name##_get_aware( vc_t obj ) \
    { \
        return name##_get_typed( *( const aware_t* )obj ); \
    }

// Optionally conclude with semicolon and continue
// struct-body via BCORE_DECLARE_SPECT_BODY
#define BCORE_DECLARE_SPECT( name ) \
    typedef struct name name; \
    BCORE_DEFINE_INLINE_SPECT_GET_TYPED_CACHED( name ) \
    BCORE_DEFINE_INLINE_SPECT_GET_AWARE( name ) \
    struct name

// Body definition only
#define BCORE_DECLARE_SPECT_BODY( name ) \
    struct name

#define BCORE_DEFINE_SPECT( name ) \
    static sc_t name##_def_g; \
    BCORE_DEFINE_SPECT_CACHE( name ); \
    BCORE_DEFINE_CREATE_SELF( name, name##_def_g ) \
    static sc_t name##_def_g = #name " = spect"

#define BCORE_REGISTER_SPECT( name )\
    bcore_spect_setup_cache( &name##_cache_g ); \
    bcore_spect_define_creator( typeof( #name ), name##_create_self );


/**********************************************************************************************************************/
// perspective functions

#define BCORE_DECLARE_SPECT_TARX_R0( spect, name, tret, to ) \
    static inline tret spect##_t_##name( tp_t t, to o )  { return spect##_p_##name( spect##_s_get_typed( t ), o ); } \
    static inline tret spect##_a_##name( to o )          { return spect##_p_##name( spect##_s_get_typed( *(aware_t*)o ), o ); } \
    static inline tret spect##_r_##name( const sr_s* o ) { return spect##_p_##name( spect_tp_o( o->p ) == TYPEOF_##spect##_s ? o->p : spect##_s_get_typed( spect_tp_o( o->p ) ), o->o ); } \
    static inline tret spect##_x_##name( sr_s o )        { tret ret = spect##_p_##name( spect_tp_o( o.p ) == TYPEOF_##spect##_s ? o.p : spect##_s_get_typed( spect_tp_o( o.p ) ), o.o ); sr_down( o ); return ret; } \

#define BCORE_DECLARE_SPECT_TARX_V0( spect, name, to ) \
    static inline void spect##_t_##name( tp_t t, to o )  { spect##_p_##name( spect##_s_get_typed( t ), o ); } \
    static inline void spect##_a_##name( to o )          { spect##_p_##name( spect##_s_get_typed( *(aware_t*)o ), o ); } \
    static inline void spect##_r_##name( const sr_s* o ) { spect##_p_##name( spect_tp_o( o->p ) == TYPEOF_##spect##_s ? o->p : spect##_s_get_typed( spect_tp_o( o->p ) ), o->o ); } \
    static inline void spect##_x_##name( sr_s o )        { spect##_p_##name( spect_tp_o( o.p ) == TYPEOF_##spect##_s ? o.p : spect##_s_get_typed( spect_tp_o( o.p ) ), o.o ); sr_down( o ); } \

#define BCORE_DECLARE_SPECT_TARX_R1( spect, name, tret, to, t1, n1 ) \
    static inline tret spect##_t_##name( tp_t t, to o,  t1 n1 ) { return spect##_p_##name( spect##_s_get_typed( t ), o,            n1 ); } \
    static inline tret spect##_a_##name( to o,          t1 n1 ) { return spect##_p_##name( spect##_s_get_typed( *(aware_t*)o ), o, n1 ); } \
    static inline tret spect##_r_##name( const sr_s* o, t1 n1 ) { return spect##_p_##name( spect_tp_o( o->p ) == TYPEOF_##spect##_s ? o->p : spect##_s_get_typed( spect_tp_o( o->p ) ), o->o, n1 ); } \
    static inline tret spect##_x_##name( sr_s o,        t1 n1 ) { tret ret = spect##_p_##name( spect_tp_o( o.p ) == TYPEOF_##spect##_s ? o.p : spect##_s_get_typed( spect_tp_o( o.p ) ), o.o, n1 ); sr_down( o ); return ret; } \

#define BCORE_DECLARE_SPECT_TARX_V1( spect, name,       to, t1, n1 ) \
    static inline void spect##_t_##name( tp_t t, to o,  t1 n1 ) { spect##_p_##name( spect##_s_get_typed( t ), o,            n1 ); } \
    static inline void spect##_a_##name( to o,          t1 n1 ) { spect##_p_##name( spect##_s_get_typed( *(aware_t*)o ), o, n1 ); } \
    static inline void spect##_r_##name( const sr_s* o, t1 n1 ) { spect##_p_##name( spect_tp_o( o->p ) == TYPEOF_##spect##_s ? o->p : spect##_s_get_typed( spect_tp_o( o->p ) ), o->o, n1 ); } \
    static inline void spect##_x_##name( sr_s o,        t1 n1 ) { spect##_p_##name( spect_tp_o( o.p ) == TYPEOF_##spect##_s ? o.p : spect##_s_get_typed( spect_tp_o( o.p ) ), o.o, n1 ); sr_down( o ); } \

#define BCORE_DECLARE_SPECT_TARX_R2( spect, name, tret, to, t1, n1, t2, n2 ) \
    static inline tret spect##_t_##name( tp_t t, to o,  t1 n1, t2 n2 ) { return spect##_p_##name( spect##_s_get_typed( t ), o,            n1, n2 ); } \
    static inline tret spect##_a_##name( to o,          t1 n1, t2 n2 ) { return spect##_p_##name( spect##_s_get_typed( *(aware_t*)o ), o, n1, n2 ); } \
    static inline tret spect##_r_##name( const sr_s* o, t1 n1, t2 n2 ) { return spect##_p_##name( spect_tp_o( o->p ) == TYPEOF_##spect##_s ? o->p : spect##_s_get_typed( spect_tp_o( o->p ) ), o->o, n1, n2 ); } \
    static inline tret spect##_x_##name( sr_s o,        t1 n1, t2 n2 ) { tret ret = spect##_p_##name( spect_tp_o( o.p ) == TYPEOF_##spect##_s ? o.p : spect##_s_get_typed( spect_tp_o( o.p ) ), o.o, n1, n2 ); sr_down( o ); return ret; } \

#define BCORE_DECLARE_SPECT_TARX_V2( spect, name,       to, t1, n1, t2, n2 ) \
    static inline void spect##_t_##name( tp_t t, to o,  t1 n1, t2 n2 ) { spect##_p_##name( spect##_s_get_typed( t ), o,            n1, n2 ); } \
    static inline void spect##_a_##name( to o,          t1 n1, t2 n2 ) { spect##_p_##name( spect##_s_get_typed( *(aware_t*)o ), o, n1, n2 ); } \
    static inline void spect##_r_##name( const sr_s* o, t1 n1, t2 n2 ) { spect##_p_##name( spect_tp_o( o->p ) == TYPEOF_##spect##_s ? o->p : spect##_s_get_typed( spect_tp_o( o->p ) ), o->o, n1, n2 ); } \
    static inline void spect##_x_##name( sr_s o,        t1 n1, t2 n2 ) { spect##_p_##name( spect_tp_o( o.p ) == TYPEOF_##spect##_s ? o.p : spect##_s_get_typed( spect_tp_o( o.p ) ), o.o, n1, n2 ); sr_down( o ); } \

#define BCORE_DECLARE_SPECT_TARX_R3( spect, name, tret, to, t1, n1, t2, n2, t3, n3 ) \
    static inline tret spect##_t_##name( tp_t t, to o,  t1 n1, t2 n2, t3 n3 ) { return spect##_p_##name( spect##_s_get_typed( t ), o,            n1, n2, n3 ); } \
    static inline tret spect##_a_##name( to o,          t1 n1, t2 n2, t3 n3 ) { return spect##_p_##name( spect##_s_get_typed( *(aware_t*)o ), o, n1, n2, n3 ); } \
    static inline tret spect##_r_##name( const sr_s* o, t1 n1, t2 n2, t3 n3 ) { return spect##_p_##name( spect_tp_o( o->p ) == TYPEOF_##spect##_s ? o->p : spect##_s_get_typed( spect_tp_o( o->p ) ), o->o, n1, n2, n3 ); } \
    static inline tret spect##_x_##name( sr_s o,        t1 n1, t2 n2, t3 n3 ) { tret ret = spect##_p_##name( spect_tp_o( o.p ) == TYPEOF_##spect##_s ? o.p : spect##_s_get_typed( spect_tp_o( o.p ) ), o.o, n1, n2, n3 ); sr_down( o ); return ret; } \

#define BCORE_DECLARE_SPECT_TARX_V3( spect, name,       to, t1, n1, t2, n2, t3, n3 ) \
    static inline void spect##_t_##name( tp_t t, to o,  t1 n1, t2 n2, t3 n3 ) { spect##_p_##name( spect##_s_get_typed( t ), o,            n1, n2, n3 ); } \
    static inline void spect##_a_##name( to o,          t1 n1, t2 n2, t3 n3 ) { spect##_p_##name( spect##_s_get_typed( *(aware_t*)o ), o, n1, n2, n3 ); } \
    static inline void spect##_r_##name( const sr_s* o, t1 n1, t2 n2, t3 n3 ) { spect##_p_##name( spect_tp_o( o->p ) == TYPEOF_##spect##_s ? o->p : spect##_s_get_typed( spect_tp_o( o->p ) ), o->o, n1, n2, n3 ); } \
    static inline void spect##_x_##name( sr_s o,        t1 n1, t2 n2, t3 n3 ) { spect##_p_##name( spect_tp_o( o.p ) == TYPEOF_##spect##_s ? o.p : spect##_s_get_typed( spect_tp_o( o.p ) ), o.o, n1, n2, n3 ); sr_down( o ); } \



#define BCORE_DECLARE_SPECT_FUNC_R0( spect, name, tret, to ) \
    tret spect##_default_##name( const spect##_s* p, to o ); \
    static inline tret spect##_p_##name( const spect##_s* p, to o ) { return spect##_default_##name( p, o ); } \
    BCORE_DECLARE_SPECT_TARX_R0( spect, name, tret, to )

#define BCORE_DECLARE_SPECT_FUNC_V0( spect, name, to ) \
    void spect##_default_##name( const spect##_s* p, to o ); \
    static inline void spect##_p_##name( const spect##_s* p, to o ) { spect##_default_##name( p, o ); } \
    BCORE_DECLARE_SPECT_TARX_V0( spect, name, to )

#define BCORE_DECLARE_SPECT_FUNC_R1( spect, name, tret, to, t1, n1 ) \
    tret spect##_default_##name( const spect##_s* p, to o,         t1 n1 ); \
    static inline tret spect##_p_##name( const spect##_s* p, to o, t1 n1 ) { return spect##_default_##name( p, o, n1 ); } \
    BCORE_DECLARE_SPECT_TARX_R1( spect, name, tret, to, t1, n1 )

#define BCORE_DECLARE_SPECT_FUNC_V1( spect, name, to, t1, n1 ) \
    void spect##_default_##name( const spect##_s* p, to o,         t1 n1 ); \
    static inline void spect##_p_##name( const spect##_s* p, to o, t1 n1 ) { spect##_default_##name( p, o, n1 ); } \
    BCORE_DECLARE_SPECT_TARX_V1( spect, name, to, t1, n1 )

#define BCORE_DECLARE_SPECT_FUNC_R2( spect, name, tret, to, t1, n1, t2, n2 ) \
    tret spect##_default_##name( const spect##_s* p, to o,         t1 n1, t2 n2 ); \
    static inline tret spect##_p_##name( const spect##_s* p, to o, t1 n1, t2 n2 ) { return spect##_default_##name( p, o, n1, n2 ); } \
    BCORE_DECLARE_SPECT_TARX_R2( spect, name, tret, to, t1, n1, t2, n2 )

#define BCORE_DECLARE_SPECT_FUNC_V2( spect, name, to, t1, n1, t2, n2 ) \
    void spect##_default_##name( const spect##_s* p, to o,         t1 n1, t2 n2 ); \
    static inline void spect##_p_##name( const spect##_s* p, to o, t1 n1, t2 n2 ) { spect##_default_##name( p, o, n1, n2 ); } \
    BCORE_DECLARE_SPECT_TARX_V2( spect, name, to, t1, n1, t2, n2 )

#define BCORE_DECLARE_SPECT_FUNC_R3( spect, name, tret, to, t1, n1, t2, n2, t3, n3 ) \
    tret spect##_default_##name( const spect##_s* p, to o,         t1 n1, t2 n2, t3 n3 ); \
    static inline tret spect##_p_##name( const spect##_s* p, to o, t1 n1, t2 n2, t3 n3 ) { return spect##_default_##name( p, o, n1, n2, n3 ); } \
    BCORE_DECLARE_SPECT_TARX_R3( spect, name, tret, to, t1, n1, t2, n2, t3, n3 )

#define BCORE_DECLARE_SPECT_FUNC_V3( spect, name, to, t1, n1, t2, n2, t3, n3 ) \
    void spect##_default_##name( const spect##_s* p, to o,         t1 n1, t2 n2, t3 n3 ); \
    static inline void spect##_p_##name( const spect##_s* p, to o, t1 n1, t2 n2, t3 n3 ) { spect##_default_##name( p, o, n1, n2, n3 ); } \
    BCORE_DECLARE_SPECT_TARX_V3( spect, name, to, t1, n1, t2, n2, t3, n3 )



#define BCORE_DECLARE_SPECT_MAPD_R0( spect, name, tret, to ) \
    tret spect##_default_##name( const spect##_s* p, to o ); \
    static inline tret spect##_p_##name( const spect##_s* p, to o )                      { return p->name ? p->name( o )             : spect##_default_##name( p, o ); } \
    BCORE_DECLARE_SPECT_TARX_R0( spect, name, tret, to )

#define BCORE_DECLARE_SPECT_MAPD_V0( spect, name, to ) \
    void spect##_default_##name( const spect##_s* p, to o ); \
    static inline void spect##_p_##name( const spect##_s* p, to o )                      {        p->name ? p->name( o )             : spect##_default_##name( p, o ); } \
    BCORE_DECLARE_SPECT_TARX_V0( spect, name, to )

#define BCORE_DECLARE_SPECT_MAPD_R1( spect, name, tret, to, t1, n1 ) \
    tret spect##_default_##name( const spect##_s* p, to o,         t1 n1 ); \
    static inline tret spect##_p_##name( const spect##_s* p, to o, t1 n1 )               { return p->name ? p->name( o, n1 )         : spect##_default_##name( p, o, n1 ); } \
    BCORE_DECLARE_SPECT_TARX_R1( spect, name, tret, to, t1, n1 )

#define BCORE_DECLARE_SPECT_MAPD_V1( spect, name, to, t1, n1 ) \
    void spect##_default_##name( const spect##_s* p, to o,         t1 n1 ); \
    static inline void spect##_p_##name( const spect##_s* p, to o, t1 n1 )               {        p->name ? p->name( o, n1 )         : spect##_default_##name( p, o, n1 ); } \
    BCORE_DECLARE_SPECT_TARX_V1( spect, name, to, t1, n1 )

#define BCORE_DECLARE_SPECT_MAPD_R2( spect, name, tret, to, t1, n1, t2, n2 ) \
    tret spect##_default_##name( const spect##_s* p, to o,         t1 n1, t2 n2 ); \
    static inline tret spect##_p_##name( const spect##_s* p, to o, t1 n1, t2 n2 )        { return p->name ? p->name( o, n1, n2 )     : spect##_default_##name( p, o, n1, n2 ); } \
    BCORE_DECLARE_SPECT_TARX_R2( spect, name, tret, to, t1, n1, t2, n2 )

#define BCORE_DECLARE_SPECT_MAPD_V2( spect, name, to, t1, n1, t2, n2 ) \
    void spect##_default_##name( const spect##_s* p, to o,         t1 n1, t2 n2 ); \
    static inline void spect##_p_##name( const spect##_s* p, to o, t1 n1, t2 n2 )        {        p->name ? p->name( o, n1, n2 )     : spect##_default_##name( p, o, n1, n2 ); } \
    BCORE_DECLARE_SPECT_TARX_V2( spect, name, to, t1, n1, t2, n2 )

#define BCORE_DECLARE_SPECT_MAPD_R3( spect, name, tret, to, t1, n1, t2, n2, t3, n3 ) \
    tret spect##_default_##name( const spect##_s* p, to o,         t1 n1, t2 n2, t3 n3 ); \
    static inline tret spect##_p_##name( const spect##_s* p, to o, t1 n1, t2 n2, t3 n3 ) { return p->name ? p->name( o, n1, n2, n3 ) : spect##_default_##name( p, o, n1, n2, n3 ); } \
    BCORE_DECLARE_SPECT_TARX_R3( spect, name, tret, to, t1, n1, t2, n2, t3, n3 )

#define BCORE_DECLARE_SPECT_MAPD_V3( spect, name, to, t1, n1, t2, n2, t3, n3 ) \
    void spect##_default_##name( const spect##_s* p, to o,         t1 n1, t2 n2, t3 n3 ); \
    static inline void spect##_p_##name( const spect##_s* p, to o, t1 n1, t2 n2, t3 n3 ) {        p->name ? p->name( o, n1, n2, n3 ) : spect##_default_##name( p, o, n1, n2, n3 ); } \
    BCORE_DECLARE_SPECT_TARX_V3( spect, name, to, t1, n1, t2, n2, t3, n3 )



#define BCORE_DECLARE_SPECT_MAPF_R0( spect, name, tret, to ) \
    static inline tret spect##_p_##name( const spect##_s* p, to o )                      { return p->name ? p->name( o )                 : bcore_spect_missing_err( p, #name ); } \
    BCORE_DECLARE_SPECT_TARX_R0( spect, name, tret, to )

#define BCORE_DECLARE_SPECT_MAPF_V0( spect, name, to ) \
    static inline void spect##_p_##name( const spect##_s* p, to o )                      { if( p->name )    p->name( o ); else             bcore_spect_missing_err( p, #name ); } \
    BCORE_DECLARE_SPECT_TARX_V0( spect, name, to )

#define BCORE_DECLARE_SPECT_MAPF_R1( spect, name, tret, to, t1, n1 ) \
    static inline tret spect##_p_##name( const spect##_s* p, to o, t1 n1 )               { return p->name ? p->name( o, n1 )             : bcore_spect_missing_err( p, #name ); } \
    BCORE_DECLARE_SPECT_TARX_R1( spect, name, tret, to, t1, n1 )

#define BCORE_DECLARE_SPECT_MAPF_V1( spect, name, to, t1, n1 ) \
    static inline void spect##_p_##name( const spect##_s* p, to o, t1 n1 )               {  if( p->name )   p->name( o, n1 ); else         bcore_spect_missing_err( p, #name ); } \
    BCORE_DECLARE_SPECT_TARX_V1( spect, name, to, t1, n1 )

#define BCORE_DECLARE_SPECT_MAPF_R2( spect, name, tret, to, t1, n1, t2, n2 ) \
    static inline tret spect##_p_##name( const spect##_s* p, to o, t1 n1, t2 n2 )        { return p->name ? p->name( o, n1, n2 )         : bcore_spect_missing_err( p, #name ); } \
    BCORE_DECLARE_SPECT_TARX_R2( spect, name, tret, to, t1, n1, t2, n2 )

#define BCORE_DECLARE_SPECT_MAPF_V2( spect, name, to, t1, n1, t2, n2 ) \
    static inline void spect##_p_##name( const spect##_s* p, to o, t1 n1, t2 n2 )        {  if( p->name )   p->name( o, n1, n2 ); else     bcore_spect_missing_err( p, #name ); } \
    BCORE_DECLARE_SPECT_TARX_V2( spect, name, to, t1, n1, t2, n2 )

#define BCORE_DECLARE_SPECT_MAPF_R3( spect, name, tret, to, t1, n1, t2, n2, t3, n3 ) \
    static inline tret spect##_p_##name( const spect##_s* p, to o, t1 n1, t2 n2, t3 n3 ) { return p->name ? p->name( o, n1, n2, n3 )     : bcore_spect_missing_err( p, #name ); } \
    BCORE_DECLARE_SPECT_TARX_R3( spect, name, tret, to, t1, n1, t2, n2, t3, n3 )

#define BCORE_DECLARE_SPECT_MAPF_V3( spect, name, to, t1, n1, t2, n2, t3, n3 ) \
    static inline void spect##_p_##name( const spect##_s* p, to o, t1 n1, t2 n2, t3 n3 ) { if( p->name )    p->name( o, n1, n2, n3 ); else bcore_spect_missing_err( p, #name ); } \
    BCORE_DECLARE_SPECT_TARX_V3( spect, name, to, t1, n1, t2, n2, t3, n3 )

#endif // BCORE_SPECT_H

