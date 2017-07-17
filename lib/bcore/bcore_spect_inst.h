/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCORE_SPECT_INST_H
#define BCORE_SPECT_INST_H

#include "bcore_flect.h"
#include "bcore_features.h"

/** The instance perspective is concerned with the representation of an object in memory and
 *  provides basic object related functionality covering construction, destruction, copying
 *  and (type-) conversion.
 */

/**********************************************************************************************************************/

typedef struct bcore_inst_item_s
{
    sz_t offset;
    sz_t size;
    sz_t align;
    bool no_trace; // do not recursively follow this item (could be private, cyclic or external)
    const bcore_flect_item_s*      flect_item;
    const struct bcore_inst_s* perspective;
} bcore_inst_item_s;

typedef struct bcore_inst_body_s
{
    bcore_inst_item_s* data;
    sz_t size, space;
} bcore_inst_body_s;

typedef struct bcore_inst_s bcore_inst_s;

typedef struct bcore_inst_s
{
    aware_t p_type; // type of perspective
    tp_t    o_type; // type of object

    bcore_inst_body_s* body;

    sz_t size;   // sizeof(type);
    sz_t align;  // alignof(type)
    bool aware;  // object is self-aware meaning it defines its type (aware_t) as first element

    bool init_flat; // flat initialization possible
    bool down_flat; // flat shut down possible
    bool copy_flat; // flat copying possible
    bool move_flat; // init_flat && copy_flat && down_flat

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
    bcore_fp_check_sanity check_sanity_o;

    /// these functions are always defined and represent the service this perspective offers
    void ( *init         )( const bcore_inst_s* p, vd_t o );
    void ( *down         )( const bcore_inst_s* p, vd_t o );
    void ( *copy         )( const bcore_inst_s* p, vd_t o,         vc_t src );
    void ( *copy_typed   )( const bcore_inst_s* p, vd_t o, tp_t t, vc_t src );
    void ( *copy_aware   )( const bcore_inst_s* p, vd_t o,         vc_t src );
    void ( *move         )( const bcore_inst_s* p, vd_t o,         vd_t src );
    vd_t ( *create       )( const bcore_inst_s* p );
    vd_t ( *create_typed )( const bcore_inst_s* p, tp_t t, vc_t src );
    vd_t ( *create_aware )( const bcore_inst_s* p,         vc_t src );
    void ( *discard      )( const bcore_inst_s* p, vd_t o );
    vd_t ( *clone        )( const bcore_inst_s* p, vc_t o );
    void ( *check_sanity )( const bcore_inst_s* p, vc_t o );
} bcore_inst_s;

bcore_flect_self_s* bcore_inst_s_create_self();

const bcore_inst_s* bcore_inst_s_get_typed( tp_t type );
const bcore_inst_s* bcore_inst_s_get_aware( vc_t obj );

void bcore_inst_spect_init(  const bcore_inst_s* o, vd_t obj );
void bcore_inst_typed_init(              tp_t type, vd_t obj );

void bcore_inst_spect_down(  const bcore_inst_s* o, vd_t obj );
void bcore_inst_typed_down(              tp_t type, vd_t obj );
void bcore_inst_aware_down(                         vd_t obj );

void bcore_inst_spect_copy(  const bcore_inst_s* o, vd_t dst, vc_t src );
void bcore_inst_typed_copy(              tp_t type, vd_t dst, vc_t src );
void bcore_inst_aware_copy(                         vd_t dst, vc_t src );

// copy with type conversion
void bcore_inst_spect_copy_typed( const bcore_inst_s* o, vd_t dst, tp_t src_type, vc_t src );
void bcore_inst_typed_copy_typed(             tp_t type, vd_t dst, tp_t src_type, vc_t src );
void bcore_inst_aware_copy_typed(                        vd_t dst, tp_t src_type, vc_t src );

void bcore_inst_spect_move( const bcore_inst_s* o, vd_t dst, vd_t src );
void bcore_inst_typed_move(             tp_t type, vd_t dst, vd_t src );
void bcore_inst_aware_move(                        vd_t dst, vd_t src );

vd_t bcore_inst_spect_create( const bcore_inst_s* o );
vd_t bcore_inst_typed_create( tp_t type );

void bcore_inst_spect_discard( const bcore_inst_s* o, vd_t obj );
void bcore_inst_typed_discard(             tp_t type, vd_t obj );
void bcore_inst_aware_discard(                        vd_t obj );

vd_t bcore_inst_spect_clone( const bcore_inst_s* o, vc_t obj );
vd_t bcore_inst_typed_clone(             tp_t type, vc_t obj );
vd_t bcore_inst_aware_clone(                        vc_t obj );

void bcore_inst_spect_check_sanity( const bcore_inst_s* o, vc_t obj );
void bcore_inst_typed_check_sanity(             tp_t type, vc_t obj );
void bcore_inst_aware_check_sanity(                        vc_t obj );

/** This function checks the instance's size with c-style sizeof( object ).
 *  It can be used as low-level safeguard against changing the c-structure
 *  but forgetting to update the self reflection.
 */
void bcore_inst_spect_check_sizeof( const bcore_inst_s* o, sz_t size );
void bcore_inst_typed_check_sizeof(             tp_t type, sz_t size );

/**********************************************************************************************************************/
// testing, debugging

bcore_string_s* bcore_spect_inst_selftest();

#endif // BCORE_SPECT_INST_H

