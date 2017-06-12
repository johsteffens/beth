/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

/** Reflection Framework.
 *  This framework managers self reflection of objects.
 *  Objects with self-reflection can participate in the
 *  'Perspective Framework' (see bcore_perspective.h)
 *  Where appropriate we abbreviate 'reflection' as 'flect'
 */

#ifndef BCORE_FLECT_H
#define BCORE_FLECT_H

#include "bcore_first.h"
#include "bcore_types.h"
#include "bcore_control.h"
#include "bcore_name_manager.h"
#include "bcore_string.h"

/// data encapsulation methods
enum
{
    BCORE_CAPS_STATIC = 0,
    BCORE_CAPS_STATIC_LINK,
    BCORE_CAPS_TYPED_LINK,
    BCORE_CAPS_AWARE_LINK,
    BCORE_CAPS_STATIC_ARRAY,
    BCORE_CAPS_TYPED_ARRAY,
    BCORE_CAPS_STATIC_LINK_ARRAY,
    BCORE_CAPS_TYPED_LINK_ARRAY,
    BCORE_CAPS_AWARE_LINK_ARRAY,
    BCORE_CAPS_EXTERNAL_DATA,
    BCORE_CAPS_EXTERNAL_FUNC
};

/// encapsulation structures
typedef struct { vd_t  link;                                   } bcore_flect_caps_static_link_s;
typedef struct { vd_t  link; tp_t type;                        } bcore_flect_caps_typed_link_s;
typedef struct { vd_t  link;                                   } bcore_flect_caps_aware_link_s;
typedef struct { vd_t  data; sz_t size; sz_t space;            } bcore_flect_caps_static_array_s;
typedef struct { vd_t  data; sz_t size; sz_t space; tp_t type; } bcore_flect_caps_typed_array_s;
typedef struct { vd_t* data; sz_t size; sz_t space;            } bcore_flect_caps_static_link_array_s;
typedef struct { vd_t* data; sz_t size; sz_t space; tp_t type; } bcore_flect_caps_typed_link_array_s;
typedef struct { vd_t* data; sz_t size; sz_t space;            } bcore_flect_caps_aware_link_array_s;
/*
  Note:
  The reflection framework assumes above structures 'emplaced' in governing structures.
  Except for trivial cases (such as structures with one element), parallel structures should also emplace
  above structures rather than emplacing their respective elements (anonymous unions may be possible).
  Not doing so can cause platform sensitive misalignments depending on the organization of the
  enveloping structure.
*/

/// conversion between enum and string
sc_t bcore_flect_caps_e_sc( u2_t caps );
u2_t bcore_flect_caps_e_u2( sc_t sc );

/// alignment and size retrieval
sz_t bcore_flect_caps_e_size( u2_t caps );
sz_t bcore_flect_caps_e_align( u2_t caps );

/// checks if encapsulation is an array
bool bcore_flect_caps_is_array( u2_t caps );

typedef struct bcore_flect_item_s
{
    u2_t caps;   // type of embedding

    /// to be removed...
    sz_t offset;
    sz_t size;
    sz_t align;


    union
    {
        vc_t d_ptr;  // external data pointer
        fp_t f_ptr;  // external function pointer
    };

    u2_t type; // hash of type (primitive, structure, aware-mutative, typed-mutative)
    u2_t name; // hash of name
} bcore_flect_item_s;

void                bcore_flect_item_s_init( bcore_flect_item_s* o );
void                bcore_flect_item_s_down( bcore_flect_item_s* o );
void                bcore_flect_item_s_copy( bcore_flect_item_s* o, const bcore_flect_item_s* src );
void                bcore_flect_item_s_move( bcore_flect_item_s* o,       bcore_flect_item_s* src );
bcore_flect_item_s* bcore_flect_item_s_create();
bcore_flect_item_s* bcore_flect_item_s_create_external_data( vc_t data, sc_t type, sc_t name );
bcore_flect_item_s* bcore_flect_item_s_create_external_func( fp_t func, sc_t type, sc_t name );
void                bcore_flect_item_s_discard( bcore_flect_item_s* o );
bcore_flect_item_s* bcore_flect_item_s_clone( const bcore_flect_item_s* o );
bcore_string_s*     bcore_flect_item_s_show( const bcore_flect_item_s* o );

// calculates the offset from alignment and raw offset
sz_t bcore_flect_aligned_offset( sz_t align, sz_t raw_offset );

/**********************************************************************************************************************/

typedef struct bcore_flect_body_s
{
    bcore_flect_item_s* data;
    sz_t size, space;
} bcore_flect_body_s;

void                bcore_flect_body_s_init( bcore_flect_body_s* o );
void                bcore_flect_body_s_down( bcore_flect_body_s* o );
bcore_flect_body_s* bcore_flect_body_s_create();
void                bcore_flect_body_s_discard( bcore_flect_body_s* o );
void                bcore_flect_body_s_push( bcore_flect_body_s* o, const bcore_flect_item_s* item );
void                bcore_flect_body_s_push_d( bcore_flect_body_s* o, bcore_flect_item_s* item );
bcore_string_s*     bcore_flect_body_s_show( const bcore_flect_body_s* o );

/// computes the offset for next item at given alignment
sz_t bcore_flect_body_s_get_next_offset( const bcore_flect_body_s* o, sz_t align );

/// Returns the body alignment
sz_t bcore_flect_body_s_get_align( const bcore_flect_body_s* o );

/// Returns the body size (offsets must be correctly set)
sz_t bcore_flect_body_s_get_size( const bcore_flect_body_s* o );

/**********************************************************************************************************************/

typedef struct bcore_flect_self_s
{
    u2_t type;   // type is needed for self aware objects constructed by the interface; (whether this might cause problems with aliases is to be determined)
    sz_t size;   // sizeof(type); Only for the predefined size of primitive types. Composite types have size calculated in the instanceperspective.

    /// to be removed...
    sz_t align;  // alignof(type)
    bool aware;  // object is self-aware meaning it defines its type as first element

    bcore_flect_body_s* body;
} bcore_flect_self_s;

void                     bcore_flect_self_s_init( bcore_flect_self_s* o );
void                     bcore_flect_self_s_init_head( bcore_flect_self_s* o, u2_t type, sz_t size );
void                     bcore_flect_self_s_init_plain( bcore_flect_self_s* o, u2_t type, sz_t size );
void                     bcore_flect_self_s_down( bcore_flect_self_s* o );
void                     bcore_flect_self_s_copy( bcore_flect_self_s* o, const bcore_flect_self_s* src );
bcore_flect_self_s*      bcore_flect_self_s_create();
bcore_flect_self_s*      bcore_flect_self_s_create_head( u2_t type, sz_t size ); // body follows
bcore_flect_self_s*      bcore_flect_self_s_create_plain( u2_t type, sz_t size ); // plain (primitive) self contained type
bcore_flect_self_s*      bcore_flect_self_s_clone( const bcore_flect_self_s* o );
void                     bcore_flect_self_s_discard( bcore_flect_self_s* o );
void                     bcore_flect_self_s_push( bcore_flect_self_s* o, const bcore_flect_item_s* item );
void                     bcore_flect_self_s_push_d( bcore_flect_self_s* o, bcore_flect_item_s* item );
void                     bcore_flect_self_s_push_external_data( bcore_flect_self_s* o, vc_t data, sc_t type, sc_t name );
void                     bcore_flect_self_s_push_external_func( bcore_flect_self_s* o, fp_t func, sc_t type, sc_t name );
bcore_string_s*          bcore_flect_self_s_show( const bcore_flect_self_s* o );
bcore_flect_self_s*      bcore_flect_self_s_build_parse( const bcore_string_s* text, sz_t* p_idx );
bcore_flect_self_s*      bcore_flect_self_s_build_parse_sc( sc_t text );

/**********************************************************************************************************************/

/// Global reflection manager (thread safe)
void bcore_flect_define_basics();
void bcore_flect_close();  // closes manager

void bcore_flect_define_self_d(       bcore_flect_self_s* self ); // takes over control of self
void bcore_flect_define_self_c( const bcore_flect_self_s* self ); // stores a copy of self
sz_t bcore_flect_parse(         const bcore_string_s* string, sz_t idx );
sc_t bcore_flect_parse_sc(      sc_t sc );

void bcore_flect_define_alias(     u2_t alias, u2_t type ); // also enrolls name in name manager
void bcore_flect_define_alias_sc(  sc_t alias, sc_t type ); // also enrolls name in name manager

const bcore_flect_self_s* bcore_flect_try_self( u2_t type ); // returns NULL when type does not exist
const bcore_flect_self_s* bcore_flect_get_self( u2_t type ); // error when type does not exits

/**********************************************************************************************************************/

bcore_string_s* bcore_flect_selftest();

#endif // BCORE_FLECT_H

