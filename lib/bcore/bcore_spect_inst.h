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

#ifndef BCORE_SPECT_INST_H
#define BCORE_SPECT_INST_H

#include "bcore_features.h"
#include "bcore_flect.h"
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
    sz_t     size;
    sz_t     align;
    const bcore_self_item_s*   self_item;
    const struct bcore_inst_s* inst_p;
    bl_t no_trace;    // no_trace means: do not recursively follow this item (could be private, cyclic or external)
} bcore_inst_item_s;

static inline vd_t bcore_inst_item_s_get_obj( const bcore_inst_item_s* o, vd_t parent_obj )
{
    return ( u0_t* )parent_obj + o->offset;
}

static inline
sz_t bcore_inst_item_s_array_fix_size( const bcore_inst_item_s* o )
{
    return o->self_item->array_fix_size;
}

typedef struct bcore_inst_body_s
{
    bcore_inst_item_s* data;
    sz_t size, space;
} bcore_inst_body_s;

/// returns corresponding instance item of self_item_s or NULL if not existing
const bcore_inst_item_s* bcore_inst_body_s_get_item_from_self_item( const bcore_inst_body_s* o, const bcore_self_item_s* item );

typedef struct bcore_inst_s bcore_inst_s;

typedef struct bcore_inst_o
{
    const bcore_inst_s* p;
    vd_t o;
} bcore_inst_o;

typedef struct bcore_inst_s
{
    aware_t p_type; // type of inst_p
    tp_t    o_type; // type of object

    bcore_inst_body_s* body;

    sz_t size;   // sizeof(type);
    sz_t align;  // alignof(type)
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

    /// these functions are always defined and represent the service this perspective offers
    void ( *init         )( const bcore_inst_s* p, vd_t o );
    void ( *down         )( const bcore_inst_s* p, vd_t o );
    void ( *copy         )( const bcore_inst_s* p, vd_t o,         vc_t src );
    void ( *copy_typed   )( const bcore_inst_s* p, vd_t o, tp_t t, vc_t src );
    void ( *move         )( const bcore_inst_s* p, vd_t o,         vd_t src );
    vd_t ( *create       )( const bcore_inst_s* p );
    vd_t ( *create_typed )( const bcore_inst_s* p, tp_t t, vc_t src );
    void ( *discard      )( const bcore_inst_s* p, vd_t o );
    vd_t ( *clone        )( const bcore_inst_s* p, vc_t o );
} bcore_inst_s;

void bcore_inst_s_discard( bcore_inst_s* o );

sz_t bcore_inst_s_get_items_size( const bcore_inst_s* o );
const bcore_inst_item_s* bcore_inst_s_get_item( const bcore_inst_s* o, sz_t index );
const bcore_inst_item_s* bcore_inst_s_get_item_from_self_item( const bcore_inst_s* o, const bcore_self_item_s* item ); // returns NULL if not found

const bcore_inst_s* bcore_inst_s_get_typed( tp_t o_type );

static inline
const bcore_inst_s* bcore_inst_s_get_aware( vc_t obj )
{
    return bcore_inst_s_get_typed( *( const aware_t* )obj );
}

void bcore_inst_spect_init(         const bcore_inst_s* o, vd_t obj );
void bcore_inst_spect_down(         const bcore_inst_s* o, vd_t obj );
void bcore_inst_spect_copy(         const bcore_inst_s* o, vd_t dst, vc_t src );
void bcore_inst_spect_copy_typed(   const bcore_inst_s* o, vd_t dst, tp_t src_type, vc_t src );
void bcore_inst_spect_move(         const bcore_inst_s* o, vd_t dst, vd_t src );
vd_t bcore_inst_spect_create(       const bcore_inst_s* o );
vd_t bcore_inst_spect_create_typed( const bcore_inst_s* o, tp_t otp, vc_t obj );
void bcore_inst_spect_discard(      const bcore_inst_s* o, vd_t obj );
vd_t bcore_inst_spect_clone(        const bcore_inst_s* o, vc_t obj );

void bcore_inst_typed_init(         tp_t type, vd_t obj );
void bcore_inst_typed_down(         tp_t type, vd_t obj );
void bcore_inst_typed_copy(         tp_t type, vd_t dst, vc_t src );
void bcore_inst_typed_copy_typed(   tp_t type, vd_t dst, tp_t src_type, vc_t src );
vd_t bcore_inst_typed_create(       tp_t type );
vd_t bcore_inst_typed_create_typed( tp_t type, tp_t otp, vc_t obj );
sr_s bcore_inst_typed_create_sr(    tp_t type );
void bcore_inst_typed_move(         tp_t type, vd_t dst, vd_t src );
void bcore_inst_typed_discard(      tp_t type, vd_t obj );
vd_t bcore_inst_typed_clone(        tp_t type, vc_t obj );
sr_s bcore_inst_typed_clone_sr(     tp_t type, vc_t obj );

void bcore_inst_aware_down(         vd_t obj );
void bcore_inst_aware_copy(         vd_t dst, vc_t src );
void bcore_inst_aware_copy_typed(   vd_t dst, tp_t src_type, vc_t src );
void bcore_inst_aware_move(         vd_t dst, vd_t src );
void bcore_inst_aware_discard(      vd_t obj );
vd_t bcore_inst_aware_clone(        vc_t obj );

void bcore_inst_discard(  sr_s o ); // only discards when o is a strong reference; does nothing otherwise
vd_t bcore_inst_clone(    sr_s o );
sr_s bcore_inst_clone_sr( sr_s o ); // returns perspective of o

vd_t bcore_inst_q_clone(    const sr_s* o );
sr_s bcore_inst_q_clone_sr( const sr_s* o ); // returns perspective of o

/** This function checks the instance's size with c-style sizeof( object ).
 *  It can be used as low-level safeguard against changing the c-structure
 *  but forgetting to update the self reflection.
 */
void bcore_inst_spect_check_sizeof( const bcore_inst_s* o, sz_t size );
void bcore_inst_typed_check_sizeof(             tp_t type, sz_t size );

#define BCORE_DEFINE_FUNCTION_INIT_INST( name ) void name##_init( name* o ) { bcore_inst_typed_init( TYPEOF_##name, o ); }
#define BCORE_DEFINE_FUNCTION_DOWN_INST( name ) void name##_down( name* o ) { bcore_inst_typed_down( TYPEOF_##name, o ); }
#define BCORE_DEFINE_FUNCTION_COPY_INST( name ) void name##_copy( name* o, const name* src ) \
{ \
    if( o == src ) return; \
    bcore_inst_typed_copy( TYPEOF_##name, o, src ); \
}

#define BCORE_DEFINE_FUNCTIONS_IDC_INST( name ) \
    BCORE_DEFINE_FUNCTION_INIT_INST( name )\
    BCORE_DEFINE_FUNCTION_DOWN_INST( name )\
    BCORE_DEFINE_FUNCTION_COPY_INST( name )\

#define BCORE_DEFINE_FUNCTION_CREATE_INST( name ) name* name##_create() { return bcore_inst_typed_create( TYPEOF_##name ); }
#define BCORE_DEFINE_FUNCTION_DISCARD_INST( name ) void name##_discard( name* o ) { bcore_inst_typed_discard( TYPEOF_##name, o ); }
#define BCORE_DEFINE_FUNCTION_CLONE_INST( name ) \
name* name##_clone( const name* o ) \
{ \
    if( !o ) return NULL; \
    return bcore_inst_typed_clone( TYPEOF_##name, o ); \
}

#define BCORE_DEFINE_FUNCTIONS_CDC_INST( name ) \
    BCORE_DEFINE_FUNCTION_CREATE_INST( name )\
    BCORE_DEFINE_FUNCTION_DISCARD_INST( name )\
    BCORE_DEFINE_FUNCTION_CLONE_INST( name )

#define BCORE_DEFINE_FUNCTIONS_OBJ_INST( name ) \
    BCORE_DEFINE_FUNCTIONS_IDC_INST( name )\
    BCORE_DEFINE_FUNCTIONS_CDC_INST( name )

#define BCORE_DEFINE_OBJECT_INST( name, def ) \
    BCORE_DEFINE_FUNCTIONS_OBJ_INST( name ) \
    BCORE_DEFINE_CREATE_SELF( name, def )

/**********************************************************************************************************************/

vd_t bcore_spect_inst_signal_handler( const bcore_signal_s* o );

#endif // BCORE_SPECT_INST_H

