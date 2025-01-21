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

#ifndef BCORE_SPECT_INST_H
#define BCORE_SPECT_INST_H

#include "bcore_feature.h"
#include "bcore_flect.h"
#include "bcore_trait.h"
#include "bcore_spect.h"
#include "bcore_quicktypes.h"

/** The instance inst_p is concerned with the representation of an object in memory and
 *  provides basic object related functionality covering construction, destruction, copying
 *  and (type-) conversion.
 *
 *  Amoebic features: init, down, copy
 */

/**********************************************************************************************************************/

typedef struct bcore_inst_item_s
{
    offset_t offset;
    uz_t     size;
    uz_t     align;
    const bcore_self_item_s*   self_item;
    const struct bcore_inst_s* inst_p;
    bl_t no_trace;    // no_trace means: do not recursively follow this item (could be private, cyclic or external)
} bcore_inst_item_s;

static inline vd_t bcore_inst_item_s_get_obj( const bcore_inst_item_s* o, vd_t parent_obj )
{
    return ( u0_t* )parent_obj + o->offset;
}

static inline
uz_t bcore_inst_item_s_array_fix_size( const bcore_inst_item_s* o )
{
    return o->self_item->array_fix_size;
}

typedef struct bcore_inst_body_s
{
    bcore_inst_item_s* data;
    uz_t size, space;
} bcore_inst_body_s;

/// returns corresponding instance item of self_item_s or NULL if not existing
const bcore_inst_item_s* bcore_inst_body_s_get_item_from_self_item( const bcore_inst_body_s* o, const bcore_self_item_s* item );

typedef struct bcore_inst_s bcore_inst_s;

typedef struct bcore_inst_o
{
    const bcore_inst_s* p;
    vd_t o;
} bcore_inst_o;

BCORE_FORWARD_OBJECT( bcore_inst_call_spect_s );

BCORE_DECLARE_SPECT( bcore_inst )
{
    bcore_spect_header_s header;

    bcore_inst_body_s* body;

    uz_t size;   // sizeof(type);
    uz_t align;  // alignof(type)
    bl_t aware;  // object is self-aware, meaning it defines its type (aware_t) as first element

    bl_t init_flat; // flat initialization possible
    bl_t down_flat; // flat shut down possible
    bl_t copy_flat; // flat copying possible
    bl_t move_flat; // init_flat && copy_flat && down_flat

    /// these functions are only defined if they are part of object's reflection
    fp_t init_o; // bcore_fp_init or ap_t
    fp_t down_o; // bcore_fp_down or ap_t
    fp_t copy_o; // bcore_fp_copy or ap_t
    bcore_fp_copy_typed   copy_typed_o;
    bcore_fp_move         move_o;
    bcore_fp_create       create_o;
    bcore_fp_create_typed create_typed_o;
    bcore_fp_discard      discard_o;
    bcore_fp_clone        clone_o;

    /// callback perspective
    const bcore_inst_call_spect_s* inst_call_p;

    /// these functions are always defined and represent the service this perspective offers
    void ( *init         )( const bcore_inst_s* p, vd_t o );
    void ( *down         )( const bcore_inst_s* p, vd_t o );
    void ( *copy         )( const bcore_inst_s* p, vd_t o,         vc_t src );
    er_t ( *copy_typed   )( const bcore_inst_s* p, vd_t o, tp_t t, vc_t src );
    void ( *move         )( const bcore_inst_s* p, vd_t o,         vd_t src );
    vd_t ( *create       )( const bcore_inst_s* p );
    vd_t ( *create_typed )( const bcore_inst_s* p, tp_t t, vc_t src );
    void ( *discard      )( const bcore_inst_s* p, vd_t o );
    vd_t ( *clone        )( const bcore_inst_s* p, vc_t o );
};

void bcore_inst_s_discard( bcore_inst_s* o );

uz_t bcore_inst_s_get_items_size( const bcore_inst_s* o );
const bcore_inst_item_s* bcore_inst_s_get_item( const bcore_inst_s* o, uz_t index );
const bcore_inst_item_s* bcore_inst_s_get_item_from_self_item( const bcore_inst_s* o, const bcore_self_item_s* item ); // returns NULL if not found

static inline void bcore_inst_default_init(  const bcore_inst_s* p, bcore_inst* obj ) { p->init( p, obj ); }
static inline void bcore_inst_default_down(  const bcore_inst_s* p, bcore_inst* obj ) { p->down( p, obj ); }
static inline void bcore_inst_default_copy(  const bcore_inst_s* p, bcore_inst* dst, vc_t src ) { if( dst != src ) p->copy( p, dst, src ); }
static inline void bcore_inst_default_move(  const bcore_inst_s* p, bcore_inst* dst, vd_t src ) { if( dst != src ) p->move( p, dst, src ); }
static inline vd_t bcore_inst_default_clone( const bcore_inst_s* p, const bcore_inst* obj ) { return obj ? p->clone( p, obj ) : NULL; }

static inline er_t bcore_inst_default_copy_typed( const bcore_inst_s* p, bcore_inst* dst, tp_t src_type, vc_t src )
{
    if( dst != src )
    {
        if( p->header.o_type == src_type )
        {
            p->copy( p, dst, src );
            return 0;
        }
        else
        {
            return p->copy_typed( p, dst, src_type, src );
        }
    }
    return 0;
}

// functions below are exempted from generic framework for inst-specific reasons
static inline void bcore_inst_p_discard(      const bcore_inst_s* p, vd_t obj )           { if( obj ) p->discard( p, obj ); }

// creates target and copies from typed obj; returns NULL in case of conversion error (check bcore_error in case)
static inline vd_t bcore_inst_p_create_typed( const bcore_inst_s* p, tp_t otp, vc_t obj ) { return p->create_typed( p, otp, obj ); }

static inline vd_t bcore_inst_p_create( const bcore_inst_s* p )    { return p->create( p ); }
static inline vd_t bcore_inst_t_create( tp_t type )                { return type != 0 ? bcore_inst_p_create( bcore_inst_s_get_typed( type ) ) : NULL; }
static inline sr_s bcore_inst_p_create_sr( const bcore_inst_s* p ) { return sr_psm( p, p->create( p ) ); }
static inline sr_s bcore_inst_t_create_sr( tp_t type )             { return bcore_inst_p_create_sr( bcore_inst_s_get_typed( type ) ); }

BCORE_FUNC_SPECT_CONST0_RET0_ARG0_MAP0( bcore_inst, init )
BCORE_FUNC_SPECT_CONST0_RET0_ARG0_MAP0( bcore_inst, down )
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP0( bcore_inst, copy, vc_t, src )
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP0( bcore_inst, move, vd_t, src )
BCORE_FUNC_SPECT_CONST0_RET1_ARG2_MAP0( bcore_inst, copy_typed, er_t, tp_t, src_type, vc_t, src )

static inline vd_t bcore_inst_p_clone( const bcore_inst_s* p, const bcore_inst* o ) { return bcore_inst_default_clone( p, o ); }
static inline vd_t bcore_inst_t_clone( tp_t t,                const bcore_inst* o ) { return bcore_inst_p_clone( bcore_inst_s_get_typed( t ), o ); }
static inline vd_t bcore_inst_a_clone(                        const bcore_inst* o ) { return o ? bcore_inst_t_clone( *(aware_t*)o, o ) : NULL; }

vd_t bcore_inst_t_create_typed( tp_t type, tp_t otp, vc_t obj );
sr_s bcore_inst_t_create_sr(    tp_t type );
sr_s bcore_inst_t_clone_sr(     tp_t type, vc_t obj );

static inline void bcore_inst_t_discard( tp_t type, vd_t obj )
{
    if( !obj ) return;
    const bcore_inst_s* o = bcore_inst_s_get_typed( type );
    o->discard( o, obj );
}

static inline void bcore_inst_a_discard( vd_t obj )
{
    if( !obj ) return;
    const bcore_inst_s* o = bcore_inst_s_get_aware( obj );
    o->discard( o, obj );
}

static inline void bcore_inst_t_detach( tp_t type, bcore_inst** o )
{
    if( !o ) return;
    bcore_inst_t_discard( type, *o );
    *o = NULL;
}

static inline void bcore_inst_a_detach( bcore_inst** o )
{
    if( !o ) return;
    bcore_inst_a_discard( *o );
    *o = NULL;
}

static inline bcore_inst* bcore_inst_t_attach( tp_t type, bcore_inst** o, bcore_inst* src )
{
    if( !o ) return NULL;
    bcore_inst_t_discard( type, *o );
    *o = src;
    return *o;
}

static inline bcore_inst* bcore_inst_a_attach( bcore_inst** o, bcore_inst* src )
{
    if( !o ) return NULL;
    bcore_inst_a_discard( *o );
    *o = src;
    return *o;
}

void bcore_inst_x_discard(      sr_s o ); // only discards when o is a strong reference; does nothing otherwise
sr_s bcore_inst_x_clone_sr(     sr_s o ); // returns perspective of o

sr_s bcore_inst_r_clone_sr( const sr_s* o ); // returns perspective of o

static inline bcore_inst* bcore_inst_p_replicate( const bcore_inst_s* p, bcore_inst** o, const bcore_inst* src )
{
    assert( o );
    bcore_inst_p_discard( p, *o );
    *o = ( bcore_inst* )bcore_inst_p_clone( p, src );
    return *o;
}

static inline bcore_inst* bcore_inst_t_replicate( tp_t t, bcore_inst** o, const bcore_inst* src )
{
    return bcore_inst_p_replicate( bcore_inst_s_get_typed( t ), o, src );
}

static inline bcore_inst* bcore_inst_a_replicate( bcore_inst** o, const bcore_inst* src )
{
    assert( o );
    bcore_inst_a_discard( *o );
    *o = ( bcore_inst* )bcore_inst_a_clone( src );
    return *o;
}

static inline vd_t bcore_inst_x_clone( sr_s o )
{
    if( !o.p ) return NULL;
    vd_t ret = bcore_inst_p_clone( ch_spect_p( o.p, TYPEOF_bcore_inst_s ), o.o );
    sr_down( o );
    return ret;
}

static inline vd_t bcore_inst_r_clone( const sr_s* o )
{
    if( !o ) return NULL;
    if( !o->p ) return NULL;
    vd_t ret = bcore_inst_p_clone( ch_spect_p( o->p, TYPEOF_bcore_inst_s ), o->o );
    return ret;
}

/** This function checks the instance's size with c-style sizeof( object ).
 *  It can be used as low-level safeguard against changing the c-structure
 *  but forgetting to update the self reflection.
 */
void bcore_inst_p_check_sizeof( const bcore_inst_s* o, uz_t size );
void bcore_inst_t_check_sizeof(             tp_t type, uz_t size );

/**********************************************************************************************************************/
// debugging

/** This function tests basic functionality all types supported by the bcore_inst perspective.
  * explicit test: create, discard
  * implicit test: init, down
  */
void bcore_inst_test_all_types();

/**********************************************************************************************************************/
// macros

/** Default vs. Autonomous macros:
 *  Many default macros make use of predefined information (e.g. TYPEOF_ ). Since this may impair development flow and
 *  locality, there is an autonomous macro with prefix '_AUT' not requiring such predefined but executing slower.
 *  For rapid prototyping one might start with _AUT macros and turn them later into the default version.
 */

#define BCORE_DEFINE_FUNCTION_INIT_INST( name ) void name##_init( name* o ) { bcore_inst_t_init( TYPEOF_##name, (bcore_inst*)o ); }
#define BCORE_DEFINE_FUNCTION_INIT_INST_AUT( name ) void name##_init( name* o ) { bcore_inst_t_init( typeof( #name ), (bcore_inst*)o ); }

#define BCORE_DEFINE_FUNCTION_DOWN_INST( name ) void name##_down( name* o ) { bcore_inst_t_down( TYPEOF_##name, (bcore_inst*)o ); }
#define BCORE_DEFINE_FUNCTION_DOWN_INST_AUT( name ) void name##_down( name* o ) { bcore_inst_t_down( typeof( #name ), (bcore_inst*)o ); }

#define BCORE_DEFINE_FUNCTION_COPY_INST( name ) void name##_copy( name* o, const name* src ) \
{ \
    if( o == src ) return; \
    bcore_inst_t_copy( TYPEOF_##name, (bcore_inst*)o, src ); \
}

#define BCORE_DEFINE_FUNCTION_COPY_INST_AUT( name ) void name##_copy( name* o, const name* src ) \
{ \
    if( o == src ) return; \
    bcore_inst_t_copy( typeof( #name ), (bcore_inst*)o, src ); \
}

#define BCORE_DEFINE_FUNCTIONS_IDC_INST( name ) \
    BCORE_DEFINE_FUNCTION_INIT_INST( name )\
    BCORE_DEFINE_FUNCTION_DOWN_INST( name )\
    BCORE_DEFINE_FUNCTION_COPY_INST( name )\

#define BCORE_DEFINE_FUNCTIONS_IDC_INST_AUT( name ) \
    BCORE_DEFINE_FUNCTION_INIT_INST_AUT( name )\
    BCORE_DEFINE_FUNCTION_DOWN_INST_AUT( name )\
    BCORE_DEFINE_FUNCTION_COPY_INST_AUT( name )\

#define BCORE_DEFINE_FUNCTION_CREATE_INST( name ) name* name##_create() { return bcore_inst_t_create( TYPEOF_##name ); }
#define BCORE_DEFINE_FUNCTION_CREATE_INST_AUT( name ) name* name##_create() { return bcore_inst_t_create( typeof( #name ) ); }

#define BCORE_DEFINE_FUNCTION_DISCARD_INST( name ) void name##_discard( name* o ) { bcore_inst_t_discard( TYPEOF_##name, o ); }
#define BCORE_DEFINE_FUNCTION_DISCARD_INST_AUT( name ) void name##_discard( name* o ) { bcore_inst_t_discard( typeof( #name ), o ); }

#define BCORE_DEFINE_FUNCTION_CLONE_INST( name ) \
name* name##_clone( const name* o ) \
{ \
    if( !o ) return NULL; \
    return bcore_inst_t_clone( TYPEOF_##name, (bcore_inst*)o ); \
}

#define BCORE_DEFINE_FUNCTION_CLONE_INST_AUT( name ) \
name* name##_clone( const name* o ) \
{ \
    if( !o ) return NULL; \
    return bcore_inst_t_clone( typeof( #name ), (bcore_inst*)o ); \
}

#define BCORE_DEFINE_FUNCTIONS_CDC_INST( name ) \
    BCORE_DEFINE_FUNCTION_CREATE_INST( name )\
    BCORE_DEFINE_FUNCTION_DISCARD_INST( name )\
    BCORE_DEFINE_FUNCTION_CLONE_INST( name )

#define BCORE_DEFINE_FUNCTIONS_CDC_INST_AUT( name ) \
    BCORE_DEFINE_FUNCTION_CREATE_INST_AUT( name )\
    BCORE_DEFINE_FUNCTION_DISCARD_INST_AUT( name )\
    BCORE_DEFINE_FUNCTION_CLONE_INST_AUT( name )

#define BCORE_DEFINE_FUNCTIONS_OBJ_INST( name ) \
    BCORE_DEFINE_FUNCTIONS_IDC_INST( name )\
    BCORE_DEFINE_FUNCTIONS_CDC_INST( name )

#define BCORE_DEFINE_FUNCTIONS_OBJ_INST_AUT( name ) \
    BCORE_DEFINE_FUNCTIONS_IDC_INST_AUT( name )\
    BCORE_DEFINE_FUNCTIONS_CDC_INST_AUT( name )

/// deprecated: prefer BCORE_DEFINE_OBJECT_INST
#define BCORE_DEFINE_FUNCTIONS_SELF_OBJECT_INST( name, def ) \
    BCORE_DEFINE_FUNCTIONS_OBJ_INST( name ) \
    BCORE_DEFINE_CREATE_SELF( name, def )

#define BCORE_DEFINE_OBJECT_INST( trait, name ) \
    static sc_t name##_def_g; \
    BCORE_DEFINE_FUNCTIONS_OBJ_INST( name ) \
    BCORE_DEFINE_CREATE_SELF( name, name##_def_g ) \
    static sc_t name##_def_g = #name " =" #trait

#define BCORE_DEFINE_OBJECT_INST_AUT( trait, name ) \
    static sc_t name##_def_g; \
    BCORE_DEFINE_FUNCTIONS_OBJ_INST_AUT( name ) \
    BCORE_DEFINE_CREATE_SELF( name, name##_def_g ) \
    static sc_t name##_def_g = #name " =" #trait

/// preferably used by beth-plant
#define BCORE_DEFINE_OBJECT_INST_P( name ) \
    static sc_t name##_def_g; \
    BCORE_DEFINE_FUNCTIONS_OBJ_INST( name ) \
    BCORE_DEFINE_CREATE_SELF( name, name##_def_g ) \
    static sc_t name##_def_g = #name " ="

/**********************************************************************************************************************/
// virtual aware object

#define BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( name ) \
\
typedef struct name { aware_t _; } name; \
\
static inline name* name##_t_create( tp_t t ) \
{ \
    bcore_trait_assert_satisfied_type( TYPEOF_##name, t ); \
    return ( name* )bcore_inst_t_create( t ); \
} \
\
static inline name* name##_a_clone( const name* o ) \
{ \
    if( o ) bcore_trait_assert_satisfied_type( TYPEOF_##name, *(aware_t*)o ); \
    return ( name* )bcore_inst_a_clone( ( bcore_inst* )o ); \
} \
\
static inline void name##_a_copy( name* o, const name* src ) \
{ \
    assert( o ); \
    assert( src ); \
    bcore_trait_assert_satisfied_type( TYPEOF_##name, *(const aware_t*)src ); \
    bcore_inst_a_copy( ( bcore_inst* )o, ( const bcore_inst* )src ); \
} \
\
static inline void name##_a_discard( name* o ) \
{ \
    bcore_inst_a_discard( ( bcore_inst* )o ); \
} \
\
static inline void name##_a_detach( name** o ) \
{ \
    if( !o ) return; \
    bcore_inst_a_discard( ( bcore_inst* )*o ); \
    *o = NULL; \
} \
\
static inline name* name##_a_attach( name** o, name* src ) \
{ \
    if( src ) bcore_trait_assert_satisfied_type( TYPEOF_##name, *(aware_t*)src ); \
    bcore_inst_a_attach( ( bcore_inst** )o, ( bcore_inst* )src ); \
    return o ? *o : NULL; \
} \
\
static inline name* name##_a_replicate( name** o, const name* src ) \
{ \
    if( src ) bcore_trait_assert_satisfied_type( TYPEOF_##name, *(aware_t*)src ); \
    bcore_inst_a_replicate( ( bcore_inst** )o, ( bcore_inst* )src ); \
    return o ? *o : NULL; \
}

/**********************************************************************************************************************/

vd_t bcore_spect_inst_signal_handler( const bcore_signal_s* o );

#endif // BCORE_SPECT_INST_H

