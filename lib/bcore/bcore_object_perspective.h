/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCORE_OBJECT_PERSPECTIVE_H
#define BCORE_OBJECT_PERSPECTIVE_H

#include "bcore_perspective.h"
#include "bcore_flect.h"
#include "bcore_features.h"

/**********************************************************************************************************************/

typedef struct bcore_object_body_s bcore_object_body_s;

typedef struct bcore_object_s
{
    bcore_perspective_s _;

    bcore_object_body_s* body; // primitive objects have no body

    size_t size;    // size of object in memory
    bool aware;     // true; object stores its type as first element
    bool init_flat; // flat initialization possible
    bool down_flat; // flat shut down possible
    bool copy_flat; // flat copying possible

    /// these functions are only defined if they are part of object's reflection
    bcore_fp_init    init_o;
    bcore_fp_down    down_o;
    bcore_fp_copy    copy_o;
    bcore_fp_move    move_o;
    bcore_fp_create  create_o;
    bcore_fp_discard discard_o;
    bcore_fp_clone   clone_o;

    /// these functions are always defined
    void  ( *init    )( const struct bcore_object_s* i, void* o );
    void  ( *down    )( const struct bcore_object_s* i, void* o );
    void  ( *copy    )( const struct bcore_object_s* i, void* o, const void* src );
    void  ( *move    )( const struct bcore_object_s* i, void* o, void* src );
    void* ( *create  )( const struct bcore_object_s* i );
    void  ( *discard )( const struct bcore_object_s* i, void* o );
    void* ( *clone   )( const struct bcore_object_s* i, const void* o );
} bcore_object_s;

/// creates interface from object's self reflection
const bcore_object_s* bcore_object_s_get_typed( u2_t type );

static inline const bcore_object_s* bcore_object_s_get_aware( vc_t obj )
{
    return bcore_object_s_get_typed( *( const aware_t* )obj );
}

static inline void bcore_object_spect_init( const bcore_object_s* o, vd_t obj )
{
    o->init( o, obj );
}

static inline void bcore_object_typed_init( u2_t type, vd_t obj )
{
    const bcore_object_s* o = bcore_object_s_get_typed( type );
    o->init( o, obj );
}

static inline void bcore_object_spect_down( const bcore_object_s* o, vd_t obj )
{
    o->down( o, obj );
}

static inline void bcore_object_typed_down( u2_t type, vd_t obj )
{
    const bcore_object_s* o = bcore_object_s_get_typed( type );
    o->down( o, obj );
}

static inline void bcore_object_aware_down( vd_t obj )
{
    const bcore_object_s* o = bcore_object_s_get_aware( obj  );
    o->down( o, obj );
}

static inline void bcore_object_spect_copy( const bcore_object_s* o, vd_t dst, vc_t src )
{
    o->copy( o, dst, src );
}

static inline void bcore_object_typed_copy( u2_t type, vd_t dst, vc_t src )
{
    const bcore_object_s* o = bcore_object_s_get_typed( type );
    o->copy( o, dst, src );
}

static inline void bcore_object_aware_copy( vd_t dst, vc_t src )
{
    const bcore_object_s* o = bcore_object_s_get_aware( src );
    o->copy( o, dst, src );
}

static inline void bcore_object_spect_move( const bcore_object_s* o, vd_t dst, vd_t src )
{
    o->move( o, dst, src );
}

static inline void bcore_object_typed_move( u2_t type, vd_t dst, vd_t src )
{
    const bcore_object_s* o = bcore_object_s_get_typed( type );
    o->move( o, dst, src );
}

static inline void bcore_object_aware_move( vd_t dst, vd_t src )
{
    const bcore_object_s* o = bcore_object_s_get_aware( src );
    o->move( o, dst, src );
}

static inline vd_t bcore_object_spect_create( const bcore_object_s* o )
{
    return o->create( o );
}

static inline vd_t bcore_object_typed_create( u2_t type )
{
    const bcore_object_s* o = bcore_object_s_get_typed( type );
    return o->create( o );
}

static inline void bcore_object_spect_discard( const bcore_object_s* o, vd_t obj )
{
    o->discard( o, obj );
}

static inline void bcore_object_typed_discard( u2_t type, vd_t obj )
{
    const bcore_object_s* o = bcore_object_s_get_typed( type );
    o->discard( o, obj );
}

static inline void bcore_object_aware_discard( vd_t obj )
{
    const bcore_object_s* o = bcore_object_s_get_aware( obj  );
    o->discard( o, obj );
}

static inline vd_t bcore_object_spect_clone( const bcore_object_s* o, vd_t obj )
{
    return o->clone( o, obj );
}

static inline vd_t bcore_object_typed_clone( u2_t type, vd_t obj )
{
    const bcore_object_s* o = bcore_object_s_get_typed( type );
    return o->clone( o, obj );
}

static inline vd_t bcore_object_aware_clone( vd_t obj )
{
    const bcore_object_s* o = bcore_object_s_get_aware( obj  );
    return o->clone( o, obj );
}

/**********************************************************************************************************************/
// testing, debugging

bcore_string_s* bcore_object_perspective_selftest();

#endif // BCORE_OBJECT_PERSPECTIVE_H

