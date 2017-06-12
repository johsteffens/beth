/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCORE_INSTANCE_PERSPECTIVE_H
#define BCORE_INSTANCE_PERSPECTIVE_H

#include "bcore_perspective.h"
#include "bcore_flect.h"
#include "bcore_features.h"

/** The instance perspective provide basic functionality around the object covering
 *  construction, destruction, copying and conversion.
 */

/**********************************************************************************************************************/

typedef struct bcore_instance_item_s
{
    sz_t offset;
    sz_t size;
    sz_t align;
    const bcore_flect_item_s*      flect_item;
    const struct bcore_instance_s* perspective;
} bcore_instance_item_s;

typedef struct bcore_instance_body_s
{
    bcore_instance_item_s* data;
    sz_t size, space;
} bcore_instance_body_s;

typedef struct bcore_instance_s bcore_instance_s;

typedef struct bcore_instance_s
{
    bcore_perspective_s _;

    bcore_instance_body_s* body; // primitive objects have no body

    sz_t size;   // sizeof(type);
    sz_t align;  // alignof(type)
    bool aware;  // object is self-aware meaning it defines its type as first element

    bool init_flat; // flat initialization possible
    bool down_flat; // flat shut down possible
    bool copy_flat; // flat copying possible
    bool move_flat; // init_flat && copy_flat && down_flat

    /// these functions are only defined if they are part of object's reflection
    bcore_fp_init         init_o;
    bcore_fp_down         down_o;
    bcore_fp_copy         copy_o;
    bcore_fp_copy_typed   copy_typed_o;
    bcore_fp_move         move_o;
    bcore_fp_create       create_o;
    bcore_fp_create_typed create_typed_o;
    bcore_fp_discard      discard_o;
    bcore_fp_clone        clone_o;

    /// these functions are always defined and represent the service this perspective offers
    void ( *init         )( const bcore_instance_s* i, vd_t o );
    void ( *down         )( const bcore_instance_s* i, vd_t o );
    void ( *copy         )( const bcore_instance_s* i, vd_t o,         vc_t src );
    void ( *copy_typed   )( const bcore_instance_s* i, vd_t o, tp_t t, vc_t src );
    void ( *copy_aware   )( const bcore_instance_s* i, vd_t o,         vc_t src );
    void ( *move         )( const bcore_instance_s* i, vd_t o,         vd_t src );
    vd_t ( *create       )( const bcore_instance_s* i );
    vd_t ( *create_typed )( const bcore_instance_s* i, tp_t t, vc_t src );
    vd_t ( *create_aware )( const bcore_instance_s* i,         vc_t src );
    void ( *discard      )( const bcore_instance_s* i, vd_t o );
    vd_t ( *clone        )( const bcore_instance_s* i, vc_t o );
} bcore_instance_s;

const bcore_instance_s* bcore_instance_s_get_typed( u2_t type );

static inline const bcore_instance_s* bcore_instance_s_get_aware( vc_t obj )
{
    return bcore_instance_s_get_typed( *( const aware_t* )obj );
}

static inline void bcore_instance_spect_init( const bcore_instance_s* o, vd_t obj )
{
    o->init( o, obj );
}

static inline void bcore_instance_typed_init( u2_t type, vd_t obj )
{
    const bcore_instance_s* o = bcore_instance_s_get_typed( type );
    o->init( o, obj );
}

static inline void bcore_instance_spect_down( const bcore_instance_s* o, vd_t obj )
{
    o->down( o, obj );
}

static inline void bcore_instance_typed_down( u2_t type, vd_t obj )
{
    const bcore_instance_s* o = bcore_instance_s_get_typed( type );
    o->down( o, obj );
}

static inline void bcore_instance_aware_down( vd_t obj )
{
    const bcore_instance_s* o = bcore_instance_s_get_aware( obj  );
    o->down( o, obj );
}

static inline void bcore_instance_spect_copy( const bcore_instance_s* o, vd_t dst, vc_t src )
{
    if( dst == src ) return;
    o->copy( o, dst, src );
}

static inline void bcore_instance_typed_copy( u2_t type, vd_t dst, vc_t src )
{
    if( dst == src ) return;
    const bcore_instance_s* o = bcore_instance_s_get_typed( type );
    o->copy( o, dst, src );
}

static inline void bcore_instance_aware_copy( vd_t dst, vc_t src )
{
    if( dst == src ) return;
    const bcore_instance_s* o = bcore_instance_s_get_aware( src );
    o->copy( o, dst, src );
}

static inline void bcore_instance_spect_move( const bcore_instance_s* o, vd_t dst, vd_t src )
{
    if( dst == src ) return;
    o->move( o, dst, src );
}

static inline void bcore_instance_typed_move( u2_t type, vd_t dst, vd_t src )
{
    if( dst == src ) return;
    const bcore_instance_s* o = bcore_instance_s_get_typed( type );
    o->move( o, dst, src );
}

static inline void bcore_instance_aware_move( vd_t dst, vd_t src )
{
    if( dst == src ) return;
    const bcore_instance_s* o = bcore_instance_s_get_aware( src );
    o->move( o, dst, src );
}

static inline vd_t bcore_instance_spect_create( const bcore_instance_s* o )
{
    return o->create( o );
}

static inline vd_t bcore_instance_typed_create( u2_t type )
{
    const bcore_instance_s* o = bcore_instance_s_get_typed( type );
    return o->create( o );
}

static inline void bcore_instance_spect_discard( const bcore_instance_s* o, vd_t obj )
{
    if( !obj ) return;
    o->discard( o, obj );
}

static inline void bcore_instance_typed_discard( u2_t type, vd_t obj )
{
    if( !obj ) return;
    const bcore_instance_s* o = bcore_instance_s_get_typed( type );
    o->discard( o, obj );
}

static inline void bcore_instance_aware_discard( vd_t obj )
{
    if( !obj ) return;
    const bcore_instance_s* o = bcore_instance_s_get_aware( obj  );
    o->discard( o, obj );
}

static inline vd_t bcore_instance_spect_clone( const bcore_instance_s* o, vc_t obj )
{
    if( !obj ) return NULL;
    return o->clone( o, obj );
}

static inline vd_t bcore_instance_typed_clone( u2_t type, vc_t obj )
{
    if( !obj ) return NULL;
    const bcore_instance_s* o = bcore_instance_s_get_typed( type );
    return o->clone( o, obj );
}

static inline vd_t bcore_instance_aware_clone( vc_t obj )
{
    if( !obj ) return NULL;
    const bcore_instance_s* o = bcore_instance_s_get_aware( obj  );
    return o->clone( o, obj );
}

/**********************************************************************************************************************/
// testing, debugging

bcore_string_s* bcore_instance_perspective_selftest();

#endif // BCORE_INSTANCE_PERSPECTIVE_H

