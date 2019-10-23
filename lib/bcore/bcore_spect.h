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

#include "bcore_feature.h"
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

BCORE_DECLARE_FUNCTIONS_OBJ( bcore_spect_header_s )

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

/** Creates a trade of nameof( p_self->type ) without trailing '_s'
 *  which is child to parent_trade.
 * 'parent_trade' is usually the trade of a perspective that could be considered a base-class
 *  in a traditional class hierarchy.
 *  Example: 'bcore_array_s' defines trade 'bcore_array' parent of 'bcore_inst'
 *
 *  This function also checks if p_self fulfills the requirements of a perspective
 *  It is to be called during the system-init-cycle.
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
sz_t  bcore_spect_size();
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

/** Spect declaration, definition and registration:
  * The 'name' must be specified without '_s', which will be appended automatically.
  * The procedure will define the following types and traits during system init:
  *   typedef struct name name;
  *   typedef struct name_s name_s;
  *   bcore_trait_set( entypeof( "name" ), entypeof( "bcore_inst" ) );
  */

// Optionally conclude with semicolon and continue
// struct-body via BCORE_DECLARE_SPECT_BODY
#define BCORE_DECLARE_SPECT( name ) \
    typedef struct name name; \
    typedef struct name##_s name##_s; \
    BCORE_DEFINE_INLINE_SPECT_GET_TYPED_CACHED( name##_s ) \
    BCORE_DEFINE_INLINE_SPECT_GET_AWARE( name##_s ) \
    struct name##_s

// Body definition only
#define BCORE_DECLARE_SPECT_BODY( name ) \
    struct name##_s

#define BCORE_DEFINE_SPECT( parent_type, name ) \
    static sc_t name##_s_def_g; \
    BCORE_DEFINE_SPECT_CACHE( name##_s ); \
    BCORE_DEFINE_CREATE_SELF( name##_s, name##_s_def_g ) \
    static sc_t name##_s_def_g = #name "_s = spect " #parent_type

#define BCORE_REGISTER_SPECT( name )\
    bcore_spect_setup_cache( &name##_s_cache_g ); \
    bcore_spect_define_creator( typeof( #name "_s" ), name##_s_create_self );

/**********************************************************************************************************************/
// perspective functions

#define BCORE_FUNC_SPECT_TARX_R0( spect, name, tret, to, no ) \
    static inline tret spect##_t_##name( tp_t t, to no )  { return spect##_p_##name( spect##_s_get_typed( t ), no ); } \
    static inline tret spect##_a_##name( to no )          { return spect##_p_##name( spect##_s_get_typed( *(aware_t*)no ), no ); } \
    static inline tret spect##_r_##name( const sr_s* no ) { return spect##_p_##name( spect_tp_o( no->p ) == TYPEOF_##spect##_s ? no->p : spect##_s_get_typed( spect_tp_o( no->p ) ), no->o ); } \
    static inline tret spect##_x_##name( sr_s no )        { tret ret = spect##_p_##name( spect_tp_o( no.p ) == TYPEOF_##spect##_s ? no.p : spect##_s_get_typed( spect_tp_o( no.p ) ), no.o ); sr_down( no ); return ret; } \

#define BCORE_FUNC_SPECT_TARX_V0( spect, name, to, no ) \
    static inline void spect##_t_##name( tp_t t, to no )  { spect##_p_##name( spect##_s_get_typed( t ), no ); } \
    static inline void spect##_a_##name( to no )          { spect##_p_##name( spect##_s_get_typed( *(aware_t*)no ), no ); } \
    static inline void spect##_r_##name( const sr_s* no ) { spect##_p_##name( spect_tp_o( no->p ) == TYPEOF_##spect##_s ? no->p : spect##_s_get_typed( spect_tp_o( no->p ) ), no->o ); } \
    static inline void spect##_x_##name( sr_s no )        { spect##_p_##name( spect_tp_o( no.p ) == TYPEOF_##spect##_s ? no.p : spect##_s_get_typed( spect_tp_o( no.p ) ), no.o ); sr_down( no ); } \

#define BCORE_FUNC_SPECT_TARX_R1( spect, name, tret, to, no, t1, n1 ) \
    static inline tret spect##_t_##name( tp_t t, to no,  t1 n1 ) { return spect##_p_##name( spect##_s_get_typed( t ), no,            n1 ); } \
    static inline tret spect##_a_##name( to no,          t1 n1 ) { return spect##_p_##name( spect##_s_get_typed( *(aware_t*)no ), no, n1 ); } \
    static inline tret spect##_r_##name( const sr_s* no, t1 n1 ) { return spect##_p_##name( spect_tp_o( no->p ) == TYPEOF_##spect##_s ? no->p : spect##_s_get_typed( spect_tp_o( no->p ) ), no->o, n1 ); } \
    static inline tret spect##_x_##name( sr_s no,        t1 n1 ) { tret ret = spect##_p_##name( spect_tp_o( no.p ) == TYPEOF_##spect##_s ? no.p : spect##_s_get_typed( spect_tp_o( no.p ) ), no.o, n1 ); sr_down( no ); return ret; } \

#define BCORE_FUNC_SPECT_TARX_V1( spect, name,       to, no, t1, n1 ) \
    static inline void spect##_t_##name( tp_t t, to no,  t1 n1 ) { spect##_p_##name( spect##_s_get_typed( t ), no,            n1 ); } \
    static inline void spect##_a_##name( to no,          t1 n1 ) { spect##_p_##name( spect##_s_get_typed( *(aware_t*)no ), no, n1 ); } \
    static inline void spect##_r_##name( const sr_s* no, t1 n1 ) { spect##_p_##name( spect_tp_o( no->p ) == TYPEOF_##spect##_s ? no->p : spect##_s_get_typed( spect_tp_o( no->p ) ), no->o, n1 ); } \
    static inline void spect##_x_##name( sr_s no,        t1 n1 ) { spect##_p_##name( spect_tp_o( no.p ) == TYPEOF_##spect##_s ? no.p : spect##_s_get_typed( spect_tp_o( no.p ) ), no.o, n1 ); sr_down( no ); } \

#define BCORE_FUNC_SPECT_TARX_R2( spect, name, tret, to, no, t1, n1, t2, n2 ) \
    static inline tret spect##_t_##name( tp_t t, to no,  t1 n1, t2 n2 ) { return spect##_p_##name( spect##_s_get_typed( t ), no,            n1, n2 ); } \
    static inline tret spect##_a_##name( to no,          t1 n1, t2 n2 ) { return spect##_p_##name( spect##_s_get_typed( *(aware_t*)no ), no, n1, n2 ); } \
    static inline tret spect##_r_##name( const sr_s* no, t1 n1, t2 n2 ) { return spect##_p_##name( spect_tp_o( no->p ) == TYPEOF_##spect##_s ? no->p : spect##_s_get_typed( spect_tp_o( no->p ) ), no->o, n1, n2 ); } \
    static inline tret spect##_x_##name( sr_s no,        t1 n1, t2 n2 ) { tret ret = spect##_p_##name( spect_tp_o( no.p ) == TYPEOF_##spect##_s ? no.p : spect##_s_get_typed( spect_tp_o( no.p ) ), no.o, n1, n2 ); sr_down( no ); return ret; } \

#define BCORE_FUNC_SPECT_TARX_V2( spect, name,       to, no, t1, n1, t2, n2 ) \
    static inline void spect##_t_##name( tp_t t, to no,  t1 n1, t2 n2 ) { spect##_p_##name( spect##_s_get_typed( t ), no,            n1, n2 ); } \
    static inline void spect##_a_##name( to no,          t1 n1, t2 n2 ) { spect##_p_##name( spect##_s_get_typed( *(aware_t*)no ), no, n1, n2 ); } \
    static inline void spect##_r_##name( const sr_s* no, t1 n1, t2 n2 ) { spect##_p_##name( spect_tp_o( no->p ) == TYPEOF_##spect##_s ? no->p : spect##_s_get_typed( spect_tp_o( no->p ) ), no->o, n1, n2 ); } \
    static inline void spect##_x_##name( sr_s no,        t1 n1, t2 n2 ) { spect##_p_##name( spect_tp_o( no.p ) == TYPEOF_##spect##_s ? no.p : spect##_s_get_typed( spect_tp_o( no.p ) ), no.o, n1, n2 ); sr_down( no ); } \

#define BCORE_FUNC_SPECT_TARX_R3( spect, name, tret, to, no, t1, n1, t2, n2, t3, n3 ) \
    static inline tret spect##_t_##name( tp_t t, to no,  t1 n1, t2 n2, t3 n3 ) { return spect##_p_##name( spect##_s_get_typed( t ), no,            n1, n2, n3 ); } \
    static inline tret spect##_a_##name( to no,          t1 n1, t2 n2, t3 n3 ) { return spect##_p_##name( spect##_s_get_typed( *(aware_t*)no ), no, n1, n2, n3 ); } \
    static inline tret spect##_r_##name( const sr_s* no, t1 n1, t2 n2, t3 n3 ) { return spect##_p_##name( spect_tp_o( no->p ) == TYPEOF_##spect##_s ? no->p : spect##_s_get_typed( spect_tp_o( no->p ) ), no->o, n1, n2, n3 ); } \
    static inline tret spect##_x_##name( sr_s no,        t1 n1, t2 n2, t3 n3 ) { tret ret = spect##_p_##name( spect_tp_o( no.p ) == TYPEOF_##spect##_s ? no.p : spect##_s_get_typed( spect_tp_o( no.p ) ), no.o, n1, n2, n3 ); sr_down( no ); return ret; } \

#define BCORE_FUNC_SPECT_TARX_V3( spect, name,       to, no, t1, n1, t2, n2, t3, n3 ) \
    static inline void spect##_t_##name( tp_t t, to no,  t1 n1, t2 n2, t3 n3 ) { spect##_p_##name( spect##_s_get_typed( t ), no,            n1, n2, n3 ); } \
    static inline void spect##_a_##name( to no,          t1 n1, t2 n2, t3 n3 ) { spect##_p_##name( spect##_s_get_typed( *(aware_t*)no ), no, n1, n2, n3 ); } \
    static inline void spect##_r_##name( const sr_s* no, t1 n1, t2 n2, t3 n3 ) { spect##_p_##name( spect_tp_o( no->p ) == TYPEOF_##spect##_s ? no->p : spect##_s_get_typed( spect_tp_o( no->p ) ), no->o, n1, n2, n3 ); } \
    static inline void spect##_x_##name( sr_s no,        t1 n1, t2 n2, t3 n3 ) { spect##_p_##name( spect_tp_o( no.p ) == TYPEOF_##spect##_s ? no.p : spect##_s_get_typed( spect_tp_o( no.p ) ), no.o, n1, n2, n3 ); sr_down( no ); } \

#define BCORE_FUNC_SPECT_TARX_R4( spect, name, tret, to, no, t1, n1, t2, n2, t3, n3, t4, n4 ) \
    static inline tret spect##_t_##name( tp_t t, to no,  t1 n1, t2 n2, t3 n3, t4 n4 ) { return spect##_p_##name( spect##_s_get_typed( t ), no,            n1, n2, n3, n4 ); } \
    static inline tret spect##_a_##name( to no,          t1 n1, t2 n2, t3 n3, t4 n4 ) { return spect##_p_##name( spect##_s_get_typed( *(aware_t*)no ), no, n1, n2, n3, n4 ); } \
    static inline tret spect##_r_##name( const sr_s* no, t1 n1, t2 n2, t3 n3, t4 n4 ) { return spect##_p_##name( spect_tp_o( no->p ) == TYPEOF_##spect##_s ? no->p : spect##_s_get_typed( spect_tp_o( no->p ) ), no->o, n1, n2, n3, n4 ); } \
    static inline tret spect##_x_##name( sr_s no,        t1 n1, t2 n2, t3 n3, t4 n4 ) { tret ret = spect##_p_##name( spect_tp_o( no.p ) == TYPEOF_##spect##_s ? no.p : spect##_s_get_typed( spect_tp_o( no.p ) ), no.o, n1, n2, n3, n4 ); sr_down( no ); return ret; } \

#define BCORE_FUNC_SPECT_TARX_V4( spect, name,       to, no, t1, n1, t2, n2, t3, n3, t4, n4 ) \
    static inline void spect##_t_##name( tp_t t, to no,  t1 n1, t2 n2, t3 n3, t4 n4 ) { spect##_p_##name( spect##_s_get_typed( t ), no,            n1, n2, n3, n4 ); } \
    static inline void spect##_a_##name( to no,          t1 n1, t2 n2, t3 n3, t4 n4 ) { spect##_p_##name( spect##_s_get_typed( *(aware_t*)no ), no, n1, n2, n3, n4 ); } \
    static inline void spect##_r_##name( const sr_s* no, t1 n1, t2 n2, t3 n3, t4 n4 ) { spect##_p_##name( spect_tp_o( no->p ) == TYPEOF_##spect##_s ? no->p : spect##_s_get_typed( spect_tp_o( no->p ) ), no->o, n1, n2, n3, n4 ); } \
    static inline void spect##_x_##name( sr_s no,        t1 n1, t2 n2, t3 n3, t4 n4 ) { spect##_p_##name( spect_tp_o( no.p ) == TYPEOF_##spect##_s ? no.p : spect##_s_get_typed( spect_tp_o( no.p ) ), no.o, n1, n2, n3, n4 ); sr_down( no ); } \


#define BCORE_FUNC_SPECT_MAP0_R0( spect, name, tret, to, no ) \
    tret spect##_default_##name( const spect##_s* p, to no ); \
    static inline tret spect##_p_##name( const spect##_s* p, to no ) { return spect##_default_##name( p, no ); } \
    BCORE_FUNC_SPECT_TARX_R0( spect, name, tret, to, no )

#define BCORE_FUNC_SPECT_MAP0_V0( spect, name, to, no ) \
    void spect##_default_##name( const spect##_s* p, to no ); \
    static inline void spect##_p_##name( const spect##_s* p, to no ) { spect##_default_##name( p, no ); } \
    BCORE_FUNC_SPECT_TARX_V0( spect, name, to, no )

#define BCORE_FUNC_SPECT_MAP0_R1( spect, name, tret, to, no, t1, n1 ) \
    tret spect##_default_##name( const spect##_s* p, to no,         t1 n1 ); \
    static inline tret spect##_p_##name( const spect##_s* p, to no, t1 n1 ) { return spect##_default_##name( p, no, n1 ); } \
    BCORE_FUNC_SPECT_TARX_R1( spect, name, tret, to, no, t1, n1 )

#define BCORE_FUNC_SPECT_MAP0_V1( spect, name, to, no, t1, n1 ) \
    void spect##_default_##name( const spect##_s* p, to no,         t1 n1 ); \
    static inline void spect##_p_##name( const spect##_s* p, to no, t1 n1 ) { spect##_default_##name( p, no, n1 ); } \
    BCORE_FUNC_SPECT_TARX_V1( spect, name, to, no, t1, n1 )

#define BCORE_FUNC_SPECT_MAP0_R2( spect, name, tret, to, no, t1, n1, t2, n2 ) \
    tret spect##_default_##name( const spect##_s* p, to no,         t1 n1, t2 n2 ); \
    static inline tret spect##_p_##name( const spect##_s* p, to no, t1 n1, t2 n2 ) { return spect##_default_##name( p, no, n1, n2 ); } \
    BCORE_FUNC_SPECT_TARX_R2( spect, name, tret, to, no, t1, n1, t2, n2 )

#define BCORE_FUNC_SPECT_MAP0_V2( spect, name, to, no, t1, n1, t2, n2 ) \
    void spect##_default_##name( const spect##_s* p, to no,         t1 n1, t2 n2 ); \
    static inline void spect##_p_##name( const spect##_s* p, to no, t1 n1, t2 n2 ) { spect##_default_##name( p, no, n1, n2 ); } \
    BCORE_FUNC_SPECT_TARX_V2( spect, name, to, no, t1, n1, t2, n2 )

#define BCORE_FUNC_SPECT_MAP0_R3( spect, name, tret, to, no, t1, n1, t2, n2, t3, n3 ) \
    tret spect##_default_##name( const spect##_s* p, to no,         t1 n1, t2 n2, t3 n3 ); \
    static inline tret spect##_p_##name( const spect##_s* p, to no, t1 n1, t2 n2, t3 n3 ) { return spect##_default_##name( p, no, n1, n2, n3 ); } \
    BCORE_FUNC_SPECT_TARX_R3( spect, name, tret, to, no, t1, n1, t2, n2, t3, n3 )

#define BCORE_FUNC_SPECT_MAP0_V3( spect, name, to, no, t1, n1, t2, n2, t3, n3 ) \
    void spect##_default_##name( const spect##_s* p, to no,         t1 n1, t2 n2, t3 n3 ); \
    static inline void spect##_p_##name( const spect##_s* p, to no, t1 n1, t2 n2, t3 n3 ) { spect##_default_##name( p, no, n1, n2, n3 ); } \
    BCORE_FUNC_SPECT_TARX_V3( spect, name, to, no, t1, n1, t2, n2, t3, n3 )

#define BCORE_FUNC_SPECT_MAP0_R4( spect, name, tret, to, no, t1, n1, t2, n2, t3, n3, t4, n4 ) \
    tret spect##_default_##name( const spect##_s* p, to no,         t1 n1, t2 n2, t3 n3, t4 n4 ); \
    static inline tret spect##_p_##name( const spect##_s* p, to no, t1 n1, t2 n2, t3 n3, t4 n4 ) { return spect##_default_##name( p, no, n1, n2, n3, n4 ); } \
    BCORE_FUNC_SPECT_TARX_R4( spect, name, tret, to, no, t1, n1, t2, n2, t3, n3, t4, n4 )

#define BCORE_FUNC_SPECT_MAP0_V4( spect, name, to, no, t1, n1, t2, n2, t3, n3, t4, n4 ) \
    void spect##_default_##name( const spect##_s* p, to no,         t1 n1, t2 n2, t3 n3, t4 n4 ); \
    static inline void spect##_p_##name( const spect##_s* p, to no, t1 n1, t2 n2, t3 n3, t4 n4 ) { spect##_default_##name( p, no, n1, n2, n3, n4 ); } \
    BCORE_FUNC_SPECT_TARX_V4( spect, name, to, no, t1, n1, t2, n2, t3, n3, t4, n4 )

#define BCORE_FUNC_SPECT_MAPX_R0( spect, name, tret, to, no ) \
    tret spect##_default_##name( const spect##_s* p, to no ); \
    static inline tret spect##_p_##name( const spect##_s* p, to no ) { return p->name ? p->name( (vd_t)no ) : spect##_default_##name( p, no ); } \
    BCORE_FUNC_SPECT_TARX_R0( spect, name, tret, to, no )

#define BCORE_FUNC_SPECT_MAPX_V0( spect, name, to, no ) \
    void spect##_default_##name( const spect##_s* p, to no ); \
    static inline void spect##_p_##name( const spect##_s* p, to no ) { if( p->name ) p->name( (vd_t)no ); else spect##_default_##name( p, no ); } \
    BCORE_FUNC_SPECT_TARX_V0( spect, name, to, no )

#define BCORE_FUNC_SPECT_MAPX_R1( spect, name, tret, to, no, t1, n1 ) \
    tret spect##_default_##name( const spect##_s* p, to no,         t1 n1 ); \
    static inline tret spect##_p_##name( const spect##_s* p, to no, t1 n1 ) { return p->name ? p->name( (vd_t)no, n1 ) : spect##_default_##name( p, no, n1 ); } \
    BCORE_FUNC_SPECT_TARX_R1( spect, name, tret, to, no, t1, n1 )

#define BCORE_FUNC_SPECT_MAPX_V1( spect, name, to, no, t1, n1 ) \
    void spect##_default_##name( const spect##_s* p, to no,         t1 n1 ); \
    static inline void spect##_p_##name( const spect##_s* p, to no, t1 n1 ) { if( p->name ) p->name( (vd_t)no, n1 ); else spect##_default_##name( p, no, n1 ); } \
    BCORE_FUNC_SPECT_TARX_V1( spect, name, to, no, t1, n1 )

#define BCORE_FUNC_SPECT_MAPX_R2( spect, name, tret, to, no, t1, n1, t2, n2 ) \
    tret spect##_default_##name( const spect##_s* p, to no,         t1 n1, t2 n2 ); \
    static inline tret spect##_p_##name( const spect##_s* p, to no, t1 n1, t2 n2 ) { return p->name ? p->name( (vd_t)no, n1, n2 ) : spect##_default_##name( p, no, n1, n2 ); } \
    BCORE_FUNC_SPECT_TARX_R2( spect, name, tret, to, no, t1, n1, t2, n2 )

#define BCORE_FUNC_SPECT_MAPX_V2( spect, name, to, no, t1, n1, t2, n2 ) \
    void spect##_default_##name( const spect##_s* p, to no,         t1 n1, t2 n2 ); \
    static inline void spect##_p_##name( const spect##_s* p, to no, t1 n1, t2 n2 ) { if( p->name ) p->name( (vd_t)no, n1, n2 ); else spect##_default_##name( p, no, n1, n2 ); } \
    BCORE_FUNC_SPECT_TARX_V2( spect, name, to, no, t1, n1, t2, n2 )

#define BCORE_FUNC_SPECT_MAPX_R3( spect, name, tret, to, no, t1, n1, t2, n2, t3, n3 ) \
    tret spect##_default_##name( const spect##_s* p, to no,         t1 n1, t2 n2, t3 n3 ); \
    static inline tret spect##_p_##name( const spect##_s* p, to no, t1 n1, t2 n2, t3 n3 ) { return p->name ? p->name( (vd_t)no, n1, n2, n3 ) : spect##_default_##name( p, no, n1, n2, n3 ); } \
    BCORE_FUNC_SPECT_TARX_R3( spect, name, tret, to, no, t1, n1, t2, n2, t3, n3 )

#define BCORE_FUNC_SPECT_MAPX_V3( spect, name, to, no, t1, n1, t2, n2, t3, n3 ) \
    void spect##_default_##name( const spect##_s* p, to no,         t1 n1, t2 n2, t3 n3 ); \
    static inline void spect##_p_##name( const spect##_s* p, to no, t1 n1, t2 n2, t3 n3 ) { if( p->name ) p->name( (vd_t)no, n1, n2, n3 ); else spect##_default_##name( p, no, n1, n2, n3 ); } \
    BCORE_FUNC_SPECT_TARX_V3( spect, name, to, no, t1, n1, t2, n2, t3, n3 )

#define BCORE_FUNC_SPECT_MAPX_R4( spect, name, tret, to, no, t1, n1, t2, n2, t3, n3, t4, n4 ) \
    tret spect##_default_##name( const spect##_s* p, to no,         t1 n1, t2 n2, t3 n3, t4 n4 ); \
    static inline tret spect##_p_##name( const spect##_s* p, to no, t1 n1, t2 n2, t3 n3, t4 n4 ) { return p->name ? p->name( (vd_t)no, n1, n2, n3, n4 ) : spect##_default_##name( p, no, n1, n2, n3, n4 ); } \
    BCORE_FUNC_SPECT_TARX_R4( spect, name, tret, to, no, t1, n1, t2, n2, t3, n3, t4, n4 )

#define BCORE_FUNC_SPECT_MAPX_V4( spect, name, to, no, t1, n1, t2, n2, t3, n3, t4, n4 ) \
    void spect##_default_##name( const spect##_s* p, to no,         t1 n1, t2 n2, t3 n3, t4 n4 ); \
    static inline void spect##_p_##name( const spect##_s* p, to no, t1 n1, t2 n2, t3 n3, t4 n4 ) { if( p->name ) p->name( (vd_t)no, n1, n2, n3, n4 ); else spect##_default_##name( p, no, n1, n2, n3, n4 ); } \
    BCORE_FUNC_SPECT_TARX_V4( spect, name, to, no, t1, n1, t2, n2, t3, n3, t4, n4 )


#define BCORE_FUNC_SPECT_MAP1_R0( spect, name, tret, to, no ) \
    static inline tret spect##_p_##name( const spect##_s* p, to no ) { if( !p->name ) bcore_spect_missing_err( p, #name ); return p->name( (vd_t)no ); } \
    BCORE_FUNC_SPECT_TARX_R0( spect, name, tret, to, no )

#define BCORE_FUNC_SPECT_MAP1_V0( spect, name, to, no ) \
    static inline void spect##_p_##name( const spect##_s* p, to no ) { if( !p->name ) bcore_spect_missing_err( p, #name );  p->name( (vd_t)no ); } \
    BCORE_FUNC_SPECT_TARX_V0( spect, name, to, no )

#define BCORE_FUNC_SPECT_MAP1_R1( spect, name, tret, to, no, t1, n1 ) \
    static inline tret spect##_p_##name( const spect##_s* p, to no, t1 n1 ) { if( !p->name ) bcore_spect_missing_err( p, #name ); return p->name( (vd_t)no, n1 ); } \
    BCORE_FUNC_SPECT_TARX_R1( spect, name, tret, to, no, t1, n1 )

#define BCORE_FUNC_SPECT_MAP1_V1( spect, name, to, no, t1, n1 ) \
    static inline void spect##_p_##name( const spect##_s* p, to no, t1 n1 ) {  if( !p->name ) bcore_spect_missing_err( p, #name ); p->name( (vd_t)no, n1 ); } \
    BCORE_FUNC_SPECT_TARX_V1( spect, name, to, no, t1, n1 )

#define BCORE_FUNC_SPECT_MAP1_R2( spect, name, tret, to, no, t1, n1, t2, n2 ) \
    static inline tret spect##_p_##name( const spect##_s* p, to no, t1 n1, t2 n2 ) { if( !p->name ) bcore_spect_missing_err( p, #name ); return p->name( (vd_t)no, n1, n2 ); } \
    BCORE_FUNC_SPECT_TARX_R2( spect, name, tret, to, no, t1, n1, t2, n2 )

#define BCORE_FUNC_SPECT_MAP1_V2( spect, name, to, no, t1, n1, t2, n2 ) \
    static inline void spect##_p_##name( const spect##_s* p, to no, t1 n1, t2 n2 ) { if( !p->name ) bcore_spect_missing_err( p, #name );  p->name( (vd_t)no, n1, n2 ); } \
    BCORE_FUNC_SPECT_TARX_V2( spect, name, to, no, t1, n1, t2, n2 )

#define BCORE_FUNC_SPECT_MAP1_R3( spect, name, tret, to, no, t1, n1, t2, n2, t3, n3 ) \
    static inline tret spect##_p_##name( const spect##_s* p, to no, t1 n1, t2 n2, t3 n3 ) { if( !p->name ) bcore_spect_missing_err( p, #name ); return p->name( (vd_t)no, n1, n2, n3 ); } \
    BCORE_FUNC_SPECT_TARX_R3( spect, name, tret, to, no, t1, n1, t2, n2, t3, n3 )

#define BCORE_FUNC_SPECT_MAP1_V3( spect, name, to, no, t1, n1, t2, n2, t3, n3 ) \
    static inline void spect##_p_##name( const spect##_s* p, to no, t1 n1, t2 n2, t3 n3 ) { if( !p->name ) bcore_spect_missing_err( p, #name ); p->name( (vd_t)no, n1, n2, n3 ); } \
    BCORE_FUNC_SPECT_TARX_V3( spect, name, to, no, t1, n1, t2, n2, t3, n3 )

#define BCORE_FUNC_SPECT_MAP1_R4( spect, name, tret, to, no, t1, n1, t2, n2, t3, n3, t4, n4 ) \
    static inline tret spect##_p_##name( const spect##_s* p, to no, t1 n1, t2 n2, t3 n3, t4 n4 ) { if( !p->name ) bcore_spect_missing_err( p, #name ); return p->name( (vd_t)no, n1, n2, n3, n4 ); } \
    BCORE_FUNC_SPECT_TARX_R4( spect, name, tret, to, no, t1, n1, t2, n2, t3, n3, t4, n4 )

#define BCORE_FUNC_SPECT_MAP1_V4( spect, name, to, no, t1, n1, t2, n2, t3, n3, t4, n4 ) \
    static inline void spect##_p_##name( const spect##_s* p, to no, t1 n1, t2 n2, t3 n3, t4 n4 ) { if( !p->name ) bcore_spect_missing_err( p, #name ); p->name( (vd_t)no, n1, n2, n3, n4 ); } \
    BCORE_FUNC_SPECT_TARX_V4( spect, name, to, no, t1, n1, t2, n2, t3, n3, t4, n4 )

/**
   MAP0: spect has a default implementation which is always used
   MAP1: spect has a pointer that is always used (strict feature)
   MAPX: spect has a pointer and default implementation; default implementation is used when the pointer is NULL
*/

#define BCORE_FUNC_SPECT_CONST1_RET1_ARG0_MAPX( spect, name, tret ) BCORE_FUNC_SPECT_MAPX_R0( spect, name, tret, const spect *, o )
#define BCORE_FUNC_SPECT_CONST0_RET1_ARG0_MAPX( spect, name, tret ) BCORE_FUNC_SPECT_MAPX_R0( spect, name, tret,       spect *, o )
#define BCORE_FUNC_SPECT_CONST1_RET0_ARG0_MAPX( spect, name       ) BCORE_FUNC_SPECT_MAPX_V0( spect, name,       const spect *, o )
#define BCORE_FUNC_SPECT_CONST0_RET0_ARG0_MAPX( spect, name       ) BCORE_FUNC_SPECT_MAPX_V0( spect, name,             spect *, o )
#define BCORE_FUNC_SPECT_CONST1_RET1_ARG0_MAP1( spect, name, tret ) BCORE_FUNC_SPECT_MAP1_R0( spect, name, tret, const spect *, o )
#define BCORE_FUNC_SPECT_CONST0_RET1_ARG0_MAP1( spect, name, tret ) BCORE_FUNC_SPECT_MAP1_R0( spect, name, tret,       spect *, o )
#define BCORE_FUNC_SPECT_CONST1_RET0_ARG0_MAP1( spect, name       ) BCORE_FUNC_SPECT_MAP1_V0( spect, name,       const spect *, o )
#define BCORE_FUNC_SPECT_CONST0_RET0_ARG0_MAP1( spect, name       ) BCORE_FUNC_SPECT_MAP1_V0( spect, name,             spect *, o )
#define BCORE_FUNC_SPECT_CONST1_RET1_ARG0_MAP0( spect, name, tret ) BCORE_FUNC_SPECT_MAP0_R0( spect, name, tret, const spect *, o )
#define BCORE_FUNC_SPECT_CONST0_RET1_ARG0_MAP0( spect, name, tret ) BCORE_FUNC_SPECT_MAP0_R0( spect, name, tret,       spect *, o )
#define BCORE_FUNC_SPECT_CONST1_RET0_ARG0_MAP0( spect, name       ) BCORE_FUNC_SPECT_MAP0_V0( spect, name,       const spect *, o )
#define BCORE_FUNC_SPECT_CONST0_RET0_ARG0_MAP0( spect, name       ) BCORE_FUNC_SPECT_MAP0_V0( spect, name,             spect *, o )

#define BCORE_FUNC_SPECT_CONST1_RET1_ARG1_MAPX( spect, name, tret, t1, n1 ) BCORE_FUNC_SPECT_MAPX_R1( spect, name, tret, const spect *, o, t1, n1 )
#define BCORE_FUNC_SPECT_CONST0_RET1_ARG1_MAPX( spect, name, tret, t1, n1 ) BCORE_FUNC_SPECT_MAPX_R1( spect, name, tret,       spect *, o, t1, n1 )
#define BCORE_FUNC_SPECT_CONST1_RET0_ARG1_MAPX( spect, name      , t1, n1 ) BCORE_FUNC_SPECT_MAPX_V1( spect, name,       const spect *, o, t1, n1 )
#define BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAPX( spect, name      , t1, n1 ) BCORE_FUNC_SPECT_MAPX_V1( spect, name,             spect *, o, t1, n1 )
#define BCORE_FUNC_SPECT_CONST1_RET1_ARG1_MAP1( spect, name, tret, t1, n1 ) BCORE_FUNC_SPECT_MAP1_R1( spect, name, tret, const spect *, o, t1, n1 )
#define BCORE_FUNC_SPECT_CONST0_RET1_ARG1_MAP1( spect, name, tret, t1, n1 ) BCORE_FUNC_SPECT_MAP1_R1( spect, name, tret,       spect *, o, t1, n1 )
#define BCORE_FUNC_SPECT_CONST1_RET0_ARG1_MAP1( spect, name      , t1, n1 ) BCORE_FUNC_SPECT_MAP1_V1( spect, name,       const spect *, o, t1, n1 )
#define BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP1( spect, name      , t1, n1 ) BCORE_FUNC_SPECT_MAP1_V1( spect, name,             spect *, o, t1, n1 )
#define BCORE_FUNC_SPECT_CONST1_RET1_ARG1_MAP0( spect, name, tret, t1, n1 ) BCORE_FUNC_SPECT_MAP0_R1( spect, name, tret, const spect *, o, t1, n1 )
#define BCORE_FUNC_SPECT_CONST0_RET1_ARG1_MAP0( spect, name, tret, t1, n1 ) BCORE_FUNC_SPECT_MAP0_R1( spect, name, tret,       spect *, o, t1, n1 )
#define BCORE_FUNC_SPECT_CONST1_RET0_ARG1_MAP0( spect, name      , t1, n1 ) BCORE_FUNC_SPECT_MAP0_V1( spect, name,       const spect *, o, t1, n1 )
#define BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP0( spect, name      , t1, n1 ) BCORE_FUNC_SPECT_MAP0_V1( spect, name,             spect *, o, t1, n1 )

#define BCORE_FUNC_SPECT_CONST1_RET1_ARG2_MAPX( spect, name, tret, t1, n1, t2, n2 ) BCORE_FUNC_SPECT_MAPX_R2( spect, name, tret, const spect *, o, t1, n1, t2, n2 )
#define BCORE_FUNC_SPECT_CONST0_RET1_ARG2_MAPX( spect, name, tret, t1, n1, t2, n2 ) BCORE_FUNC_SPECT_MAPX_R2( spect, name, tret,       spect *, o, t1, n1, t2, n2 )
#define BCORE_FUNC_SPECT_CONST1_RET0_ARG2_MAPX( spect, name      , t1, n1, t2, n2 ) BCORE_FUNC_SPECT_MAPX_V2( spect, name,       const spect *, o, t1, n1, t2, n2 )
#define BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAPX( spect, name      , t1, n1, t2, n2 ) BCORE_FUNC_SPECT_MAPX_V2( spect, name,             spect *, o, t1, n1, t2, n2 )
#define BCORE_FUNC_SPECT_CONST1_RET1_ARG2_MAP1( spect, name, tret, t1, n1, t2, n2 ) BCORE_FUNC_SPECT_MAP1_R2( spect, name, tret, const spect *, o, t1, n1, t2, n2 )
#define BCORE_FUNC_SPECT_CONST0_RET1_ARG2_MAP1( spect, name, tret, t1, n1, t2, n2 ) BCORE_FUNC_SPECT_MAP1_R2( spect, name, tret,       spect *, o, t1, n1, t2, n2 )
#define BCORE_FUNC_SPECT_CONST1_RET0_ARG2_MAP1( spect, name      , t1, n1, t2, n2 ) BCORE_FUNC_SPECT_MAP1_V2( spect, name,       const spect *, o, t1, n1, t2, n2 )
#define BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP1( spect, name      , t1, n1, t2, n2 ) BCORE_FUNC_SPECT_MAP1_V2( spect, name,             spect *, o, t1, n1, t2, n2 )
#define BCORE_FUNC_SPECT_CONST1_RET1_ARG2_MAP0( spect, name, tret, t1, n1, t2, n2 ) BCORE_FUNC_SPECT_MAP0_R2( spect, name, tret, const spect *, o, t1, n1, t2, n2 )
#define BCORE_FUNC_SPECT_CONST0_RET1_ARG2_MAP0( spect, name, tret, t1, n1, t2, n2 ) BCORE_FUNC_SPECT_MAP0_R2( spect, name, tret,       spect *, o, t1, n1, t2, n2 )
#define BCORE_FUNC_SPECT_CONST1_RET0_ARG2_MAP0( spect, name      , t1, n1, t2, n2 ) BCORE_FUNC_SPECT_MAP0_V2( spect, name,       const spect *, o, t1, n1, t2, n2 )
#define BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( spect, name      , t1, n1, t2, n2 ) BCORE_FUNC_SPECT_MAP0_V2( spect, name,             spect *, o, t1, n1, t2, n2 )

#define BCORE_FUNC_SPECT_CONST1_RET1_ARG3_MAPX( spect, name, tret, t1, n1, t2, n2, t3, n3 ) BCORE_FUNC_SPECT_MAPX_R3( spect, name, tret, const spect *, o, t1, n1, t2, n2, t3, n3 )
#define BCORE_FUNC_SPECT_CONST0_RET1_ARG3_MAPX( spect, name, tret, t1, n1, t2, n2, t3, n3 ) BCORE_FUNC_SPECT_MAPX_R3( spect, name, tret,       spect *, o, t1, n1, t2, n2, t3, n3 )
#define BCORE_FUNC_SPECT_CONST1_RET0_ARG3_MAPX( spect, name      , t1, n1, t2, n2, t3, n3 ) BCORE_FUNC_SPECT_MAPX_V3( spect, name,       const spect *, o, t1, n1, t2, n2, t3, n3 )
#define BCORE_FUNC_SPECT_CONST0_RET0_ARG3_MAPX( spect, name      , t1, n1, t2, n2, t3, n3 ) BCORE_FUNC_SPECT_MAPX_V3( spect, name,             spect *, o, t1, n1, t2, n2, t3, n3 )
#define BCORE_FUNC_SPECT_CONST1_RET1_ARG3_MAP1( spect, name, tret, t1, n1, t2, n2, t3, n3 ) BCORE_FUNC_SPECT_MAP1_R3( spect, name, tret, const spect *, o, t1, n1, t2, n2, t3, n3 )
#define BCORE_FUNC_SPECT_CONST0_RET1_ARG3_MAP1( spect, name, tret, t1, n1, t2, n2, t3, n3 ) BCORE_FUNC_SPECT_MAP1_R3( spect, name, tret,       spect *, o, t1, n1, t2, n2, t3, n3 )
#define BCORE_FUNC_SPECT_CONST1_RET0_ARG3_MAP1( spect, name      , t1, n1, t2, n2, t3, n3 ) BCORE_FUNC_SPECT_MAP1_V3( spect, name,       const spect *, o, t1, n1, t2, n2, t3, n3 )
#define BCORE_FUNC_SPECT_CONST0_RET0_ARG3_MAP1( spect, name      , t1, n1, t2, n2, t3, n3 ) BCORE_FUNC_SPECT_MAP1_V3( spect, name,             spect *, o, t1, n1, t2, n2, t3, n3 )
#define BCORE_FUNC_SPECT_CONST1_RET1_ARG3_MAP0( spect, name, tret, t1, n1, t2, n2, t3, n3 ) BCORE_FUNC_SPECT_MAP0_R3( spect, name, tret, const spect *, o, t1, n1, t2, n2, t3, n3 )
#define BCORE_FUNC_SPECT_CONST0_RET1_ARG3_MAP0( spect, name, tret, t1, n1, t2, n2, t3, n3 ) BCORE_FUNC_SPECT_MAP0_R3( spect, name, tret,       spect *, o, t1, n1, t2, n2, t3, n3 )
#define BCORE_FUNC_SPECT_CONST1_RET0_ARG3_MAP0( spect, name      , t1, n1, t2, n2, t3, n3 ) BCORE_FUNC_SPECT_MAP0_V3( spect, name,       const spect *, o, t1, n1, t2, n2, t3, n3 )
#define BCORE_FUNC_SPECT_CONST0_RET0_ARG3_MAP0( spect, name      , t1, n1, t2, n2, t3, n3 ) BCORE_FUNC_SPECT_MAP0_V3( spect, name,             spect *, o, t1, n1, t2, n2, t3, n3 )

#define BCORE_FUNC_SPECT_CONST1_RET1_ARG4_MAPX( spect, name, tret, t1, n1, t2, n2, t3, n3, t4, n4 ) BCORE_FUNC_SPECT_MAPX_R4( spect, name, tret, const spect *, o, t1, n1, t2, n2, t3, n3, t4, n4 )
#define BCORE_FUNC_SPECT_CONST0_RET1_ARG4_MAPX( spect, name, tret, t1, n1, t2, n2, t3, n3, t4, n4 ) BCORE_FUNC_SPECT_MAPX_R4( spect, name, tret,       spect *, o, t1, n1, t2, n2, t3, n3, t4, n4 )
#define BCORE_FUNC_SPECT_CONST1_RET0_ARG4_MAPX( spect, name      , t1, n1, t2, n2, t3, n3, t4, n4 ) BCORE_FUNC_SPECT_MAPX_V4( spect, name,       const spect *, o, t1, n1, t2, n2, t3, n3, t4, n4 )
#define BCORE_FUNC_SPECT_CONST0_RET0_ARG4_MAPX( spect, name      , t1, n1, t2, n2, t3, n3, t4, n4 ) BCORE_FUNC_SPECT_MAPX_V4( spect, name,             spect *, o, t1, n1, t2, n2, t3, n3, t4, n4 )
#define BCORE_FUNC_SPECT_CONST1_RET1_ARG4_MAP1( spect, name, tret, t1, n1, t2, n2, t3, n3, t4, n4 ) BCORE_FUNC_SPECT_MAP1_R4( spect, name, tret, const spect *, o, t1, n1, t2, n2, t3, n3, t4, n4 )
#define BCORE_FUNC_SPECT_CONST0_RET1_ARG4_MAP1( spect, name, tret, t1, n1, t2, n2, t3, n3, t4, n4 ) BCORE_FUNC_SPECT_MAP1_R4( spect, name, tret,       spect *, o, t1, n1, t2, n2, t3, n3, t4, n4 )
#define BCORE_FUNC_SPECT_CONST1_RET0_ARG4_MAP1( spect, name      , t1, n1, t2, n2, t3, n3, t4, n4 ) BCORE_FUNC_SPECT_MAP1_V4( spect, name,       const spect *, o, t1, n1, t2, n2, t3, n3, t4, n4 )
#define BCORE_FUNC_SPECT_CONST0_RET0_ARG4_MAP1( spect, name      , t1, n1, t2, n2, t3, n3, t4, n4 ) BCORE_FUNC_SPECT_MAP1_V4( spect, name,             spect *, o, t1, n1, t2, n2, t3, n3, t4, n4 )
#define BCORE_FUNC_SPECT_CONST1_RET1_ARG4_MAP0( spect, name, tret, t1, n1, t2, n2, t3, n3, t4, n4 ) BCORE_FUNC_SPECT_MAP0_R4( spect, name, tret, const spect *, o, t1, n1, t2, n2, t3, n3, t4, n4 )
#define BCORE_FUNC_SPECT_CONST0_RET1_ARG4_MAP0( spect, name, tret, t1, n1, t2, n2, t3, n3, t4, n4 ) BCORE_FUNC_SPECT_MAP0_R4( spect, name, tret,       spect *, o, t1, n1, t2, n2, t3, n3, t4, n4 )
#define BCORE_FUNC_SPECT_CONST1_RET0_ARG4_MAP0( spect, name      , t1, n1, t2, n2, t3, n3, t4, n4 ) BCORE_FUNC_SPECT_MAP0_V4( spect, name,       const spect *, o, t1, n1, t2, n2, t3, n3, t4, n4 )
#define BCORE_FUNC_SPECT_CONST0_RET0_ARG4_MAP0( spect, name      , t1, n1, t2, n2, t3, n3, t4, n4 ) BCORE_FUNC_SPECT_MAP0_V4( spect, name,             spect *, o, t1, n1, t2, n2, t3, n3, t4, n4 )

/** nomenclature
 *  CONST{1,0}: object is (not) const
 *  RET{1,0}: returns a type or void
 *  ARG{x}: number of arguments
 *  MAP{x}:
 *  0: strictly maps to default;
 *  1: strictly maps to object implementation (default not required);
 *  X: maps to default when object has no implementation
 */

#endif // BCORE_SPECT_H

