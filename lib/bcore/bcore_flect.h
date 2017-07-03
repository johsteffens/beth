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
typedef struct { vd_t  link;                                   } bcore_static_link_s;
typedef struct { vd_t  link; tp_t type;                        } bcore_typed_link_s;
typedef struct { vd_t  link;                                   } bcore_aware_link_s;
typedef struct { vd_t  data; sz_t size; sz_t space;            } bcore_static_array_s;
typedef struct { vd_t  data; sz_t size; sz_t space; tp_t type; } bcore_typed_array_s;
typedef struct { vd_t* data; sz_t size; sz_t space;            } bcore_static_link_array_s;
typedef struct { vd_t* data; sz_t size; sz_t space; tp_t type; } bcore_typed_link_array_s;
typedef struct { vd_t* data; sz_t size; sz_t space;            } bcore_aware_link_array_s;

/*
PLANNING:

* (Might not be feasible) An element of a structure may be anonymous. An anonymous structure merges with its parent structure.
  --> as if elements of the child structure had been defined in given order inside parent structure.

* Name management: Ensure all names in a structure have unique names (already at definition level).
  --> should multiple unnamed (special purpose) members be allowed ? (aware_t, parent-pointer, ... ?)

* Use hashtable for (extended) name management.

* Use type qualifiers (possibly flags). Qualifier may be of a general nature (e.g. 'private')
  or more specific (e.g. private for instance and interpreter perspectives), flags <-> features.
  -> truly universal flag managements probably requires handling an arbitrary amount of
     different flags in arbitrary combinations.

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
    tp_t type; // hash of type
    tp_t name; // hash of name
    tp_t attr; // hash of type attribute (type qualifier)

    u2_t caps; // data encapsulation

    union
    {
        vc_t d_ptr;  // external data pointer
        fp_t f_ptr;  // external function pointer
    };

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

/**********************************************************************************************************************/

typedef struct bcore_flect_self_s
{
    aware_t _;
    tp_t type;   // type is needed for self aware objects constructed by the interface; (whether this might cause problems with aliases is to be determined)

    /** size
     *  Represets sizeof(object-type);
     *  Mandatory for leaf-types (which are basically types without body)
     *  Optional for types with body:
     *   = 0: sizeof(type) is calculated in the instance perspective (e.g. for runtime generated types)
     *   > 0: sizeof(type) is calculated in the instance perspective and checked against bcore_flect_self_s::size
     *        Perspective can produce a descriptive error in case of a mismatch.
     *        This feature is useful to detect the error that object's compile-time definition is out of
     *        sync with its reflection.
     */
    sz_t size;

    /** Body of type. Exposing the body is optional.
     *  If the body is exposed, it must be complete. Otherwise, a perspective can not correctly compute the alignment of elements.
     *  A leaf type never has a body.
     */
    bcore_flect_body_s* body;
} bcore_flect_self_s;

void                     bcore_flect_self_s_init( bcore_flect_self_s* o );
void                     bcore_flect_self_s_init_plain( bcore_flect_self_s* o, tp_t type, sz_t size );
void                     bcore_flect_self_s_down( bcore_flect_self_s* o );
void                     bcore_flect_self_s_copy( bcore_flect_self_s* o, const bcore_flect_self_s* src );
bcore_flect_self_s*      bcore_flect_self_s_create();
bcore_flect_self_s*      bcore_flect_self_s_create_plain( tp_t type, sz_t size ); // plain (primitive) self contained type
bcore_flect_self_s*      bcore_flect_self_s_clone( const bcore_flect_self_s* o );
void                     bcore_flect_self_s_discard( bcore_flect_self_s* o );
void                     bcore_flect_self_s_push( bcore_flect_self_s* o, const bcore_flect_item_s* item );
void                     bcore_flect_self_s_push_d( bcore_flect_self_s* o, bcore_flect_item_s* item );
void                     bcore_flect_self_s_push_external_data( bcore_flect_self_s* o, vc_t data, sc_t type, sc_t name );
void                     bcore_flect_self_s_push_external_func( bcore_flect_self_s* o, fp_t func, sc_t type, sc_t name );
bcore_string_s*          bcore_flect_self_s_show( const bcore_flect_self_s* o );
bcore_flect_self_s*      bcore_flect_self_s_build_parse( const bcore_string_s* text, sz_t* p_idx, sz_t size_of );
bcore_flect_self_s*      bcore_flect_self_s_build_parse_sc( sc_t text, sz_t size_of );

/// reflection on bcore_flect_self_s
bcore_flect_self_s*      bcore_flect_self_s_create_self();

/**********************************************************************************************************************/

/// Global reflection manager (thread safe)
void bcore_flect_open();          // opens manager
void bcore_flect_define_basics(); // defines basic types
void bcore_flect_close();         // closes manager

void bcore_flect_define_self_d(       bcore_flect_self_s* self ); // takes over control of self
void bcore_flect_define_self_c( const bcore_flect_self_s* self ); // stores a copy of self
sz_t bcore_flect_parse(         const bcore_string_s* string, sz_t idx );
sc_t bcore_flect_parse_sc(      sc_t sc );

void bcore_flect_define_alias(     u2_t alias, tp_t type ); // also enrolls name in name manager
void bcore_flect_define_alias_sc(  sc_t alias, sc_t type ); // also enrolls name in name manager

bool  bcore_flect_exists(                       tp_t type ); // checks existence of type
const bcore_flect_self_s* bcore_flect_try_self( tp_t type ); // returns NULL when type does not exist
const bcore_flect_self_s* bcore_flect_get_self( tp_t type ); // error when type does not exits

/**********************************************************************************************************************/

bcore_string_s* bcore_flect_selftest();

#endif // BCORE_FLECT_H

