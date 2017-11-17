/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

/** Reflection Framework.
 *  This framework managers self reflection of objects.
 *  Objects with self-reflection can participate in the
 *  'Perspective Framework' (see bcore_spect.h)
 *  Where appropriate we abbreviate 'reflection' as 'flect'
 */

#ifndef BCORE_FLECT_H
#define BCORE_FLECT_H

#include "bcore_first.h"
#include "bcore_types.h"
#include "bcore_control.h"
#include "bcore_name_manager.h"
#include "bcore_function_manager.h"
#include "bcore_st.h"
#include "bcore_features.h"

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


/** bcore_array_head_s matches the beginning of every array structure below
 *  ptr has different meanings (either vd_t or vd_t*)
 */
typedef struct { vd_t   ptr; sz_t size; sz_t space;            } bcore_array_head_s;

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

* Use hashtable for (extended) name management, or
* think about a simpler (faster) mapping function in case there are only few items to be considered

*/

/// conversion between enum and string
sc_t bcore_flect_caps_e_sc( u2_t caps );
u2_t bcore_flect_caps_e_u2( sc_t sc );

/// alignment and size retrieval
sz_t bcore_flect_caps_e_size( u2_t caps );
sz_t bcore_flect_caps_e_align( u2_t caps );

/// checks if encapsulation is an array
bl_t bcore_flect_caps_is_array( u2_t caps );

typedef struct bcore_flect_item_s
{
    tp_t type; // hash of type
    tp_t name; // hash of name
    u2_t caps; // data encapsulation

    union
    {
        tp_t flags; // collection of attribute flags
        struct
        {
            // invisible to all perspectives (inst spect may only initialize the field)
            unsigned f_private  : 1;

            // interpreted type (no physical representation)
            // invisible to inst spect
            // used by via spect
            unsigned f_shell    : 1; // shell flag

            // visible to inst spect
            // hidden from via spect
            unsigned f_hidden   : 1; // hidden flag
        };
    };

    union
    {
        s3_t default_s3; // serves s0_t ... s3_t
        u3_t default_u3; // serves u0_t ... u3_t, bl_t
        f3_t default_f3; // serves f2_t ... f3_t
        tp_t default_tp; // serves tp_t and external functions
    };

} bcore_flect_item_s;

void                bcore_flect_item_s_init( bcore_flect_item_s* o );
void                bcore_flect_item_s_down( bcore_flect_item_s* o );
void                bcore_flect_item_s_copy( bcore_flect_item_s* o, const bcore_flect_item_s* src );
void                bcore_flect_item_s_move( bcore_flect_item_s* o,       bcore_flect_item_s* src );
bcore_flect_item_s* bcore_flect_item_s_create();
bcore_flect_item_s* bcore_flect_item_s_create_plain( u2_t caps, tp_t type, tp_t name );
bcore_flect_item_s* bcore_flect_item_s_create_func( sc_t fname, fp_t func, sc_t type, sc_t name );
void                bcore_flect_item_s_discard( bcore_flect_item_s* o );
bcore_flect_item_s* bcore_flect_item_s_clone( const bcore_flect_item_s* o );
st_s*               bcore_flect_item_s_show( const bcore_flect_item_s* o );
tp_t                bcore_flect_item_s_fold_tp( const bcore_flect_item_s* o, tp_t tp );
s2_t                bcore_flect_item_s_cmp( const bcore_flect_item_s* o1, const bcore_flect_item_s* o2 );        // compares two items
void                bcore_flect_item_s_check_integrity( const bcore_flect_item_s* o );

/**********************************************************************************************************************/

typedef struct bcore_flect_body_s
{
    union
    {
        bcore_static_array_s arr;
        struct
        {
            bcore_flect_item_s* data;
            sz_t size, space;
        };
    };

    // A complete body enables inst perspective to calculate the
    // full object's size and alignment from the bodies structure.
    // An incomplete body has unspecified trailing elements. (intermediate unspecified elements are not allowed)
    bl_t complete;
} bcore_flect_body_s;

void                bcore_flect_body_s_init( bcore_flect_body_s* o );
void                bcore_flect_body_s_down( bcore_flect_body_s* o );
bcore_flect_body_s* bcore_flect_body_s_create();
void                bcore_flect_body_s_discard( bcore_flect_body_s* o );
bcore_flect_item_s* bcore_flect_body_s_push( bcore_flect_body_s* o, const bcore_flect_item_s* item );
bcore_flect_item_s* bcore_flect_body_s_push_d( bcore_flect_body_s* o, bcore_flect_item_s* item );
st_s*               bcore_flect_body_s_show( const bcore_flect_body_s* o );
tp_t                bcore_flect_body_s_fold_tp( const bcore_flect_body_s* o, tp_t tp );
s2_t                bcore_flect_body_s_cmp( const bcore_flect_body_s* o1, const bcore_flect_body_s* o2 );
void                bcore_flect_body_s_check_integrity( const bcore_flect_body_s* o );

/**********************************************************************************************************************/

typedef struct bcore_flect_self_s
{
    aware_t _;

    /// type of object being reflected
    tp_t type;

    /// trait of object
    tp_t trait;

    /** size
     *  Represents sizeof(object-type);
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
     *  A leaf type is defined as object without body.
     */
    bcore_flect_body_s* body;
} bcore_flect_self_s;

void                bcore_flect_self_s_init( bcore_flect_self_s* o );
void                bcore_flect_self_s_init_plain( bcore_flect_self_s* o, tp_t type, sz_t size );
void                bcore_flect_self_s_down( bcore_flect_self_s* o );
void                bcore_flect_self_s_copy( bcore_flect_self_s* o, const bcore_flect_self_s* src );
bcore_flect_self_s* bcore_flect_self_s_create();
bcore_flect_self_s* bcore_flect_self_s_clone( const bcore_flect_self_s* o );
void                bcore_flect_self_s_discard( bcore_flect_self_s* o );
bcore_flect_item_s* bcore_flect_self_s_push( bcore_flect_self_s* o, const bcore_flect_item_s* item );
bcore_flect_item_s* bcore_flect_self_s_push_d( bcore_flect_self_s* o, bcore_flect_item_s* item );
bcore_flect_item_s* bcore_flect_self_s_push_func( bcore_flect_self_s* o, sc_t fname, fp_t func, sc_t type, sc_t name );
// namespace: function address constructed from object name and function name
bcore_flect_item_s* bcore_flect_self_s_push_ns_func( bcore_flect_self_s* o, fp_t func, sc_t type, sc_t name );
bcore_flect_item_s* bcore_flect_self_s_push_fp_set( bcore_flect_self_s* o, bcore_fp_set func, sc_t name );
bcore_flect_item_s* bcore_flect_self_s_push_fp_get( bcore_flect_self_s* o, bcore_fp_get func, sc_t name );
st_s*               bcore_flect_self_s_show( const bcore_flect_self_s* o );
void                bcore_flect_self_s_check_integrity( const bcore_flect_self_s* o );

/// special reflections
bcore_flect_self_s* bcore_flect_self_s_create_plain( tp_t type, sz_t size ); // plain (primitive) self contained type
bcore_flect_self_s* bcore_flect_self_s_create_static_array( tp_t item_type ); // creates an (anonymous) static array of item_type
bcore_flect_self_s* bcore_flect_self_s_create_static_link_array( tp_t item_type ); // creates an (anonymous) static link array of item_type


/** Creating a reflection by parsing a stream:
 *  Typical Format:
 *  <type-name> = [<trait-name>]
 *  {
 *      [private] [shell] [hidden] <type> [<qualifiers>] <name> [ = default ];
 *
 *      func <type> <name> = <ftype>;
 *      ....
 *  }
 *
 *  <type>:
 *    <type name> | <type number> | typed | aware
 *
 *  qualifiers:
 *    * : link
 *    []: array
 *
 *  Special cases:
 *    <type-name> = <assigned-name> : creates a copy of an existing reflection with new name
 *    <type-name> = [<trait-name>] { <expr>; <expr>; ... } : Specifying '...' marks the body as incomplete
 *
 */
bcore_flect_self_s* bcore_flect_self_s_build_parse_src( sr_s src, sz_t size_of );
bcore_flect_self_s* bcore_flect_self_s_build_parse_sc( sc_t text, sz_t size_of );

tp_t                bcore_flect_self_s_fold_tp( const bcore_flect_self_s* o, tp_t tp );
s2_t                bcore_flect_self_s_cmp( const bcore_flect_self_s* o1, const bcore_flect_self_s* o2 );

/// Query for external function of given type or name; either type or name may be 0 in which case it is interpreted as wildcard
fp_t bcore_flect_self_s_try_external_fp( const bcore_flect_self_s* o, tp_t type, tp_t name ); // returns NULL when not found
fp_t bcore_flect_self_s_get_external_fp( const bcore_flect_self_s* o, tp_t type, tp_t name ); // error when not found
bool bcore_flect_self_s_is_aware(        const bcore_flect_self_s* o                       ); // object has body and is aware

/** Returns a static object to be stored in the perspective.
 *  'type' specifies a function compatible to bcore_fp_create creating the static object.
 *  The static object is created in this function by calling bcore_fp_create.
 */
vd_t bcore_flect_self_s_get_static( const bcore_flect_self_s* o, tp_t type, tp_t name ); // error when not found
vd_t bcore_flect_self_s_try_static( const bcore_flect_self_s* o, tp_t type, tp_t name ); // returns NULL when not found

/** Checks proper definition of dependencies (existence);
 *  This check should be executed at a central place not before the reflection is ready to be used for generating instances.
 *  Proper places: When reflection is registered or when the instance is being created.
 *  At earlier times the reflection may be inconsistent (e.g. not all dependencies exist yet).
 */
void bcore_flect_self_s_check_integrity( const bcore_flect_self_s* o );

/**********************************************************************************************************************/
/// Global reflection manager (thread safe)

/** Define functions:
 *  Registers a self reflection once.
 *  Intended use for global registry.
 *  Error in case the same reflection type is registered twice.
 */
tp_t bcore_flect_define_self_d(       bcore_flect_self_s* self ); // takes over control of self; error if same
tp_t bcore_flect_define_self_c( const bcore_flect_self_s* self ); // stores a copy of self
tp_t bcore_flect_define_parse_src( sr_s src );
tp_t bcore_flect_define_parse_sc( sc_t sc );
tp_t bcore_flect_define_parse_fa( sc_t sc, ... );

/** Type functions:
 *  Reentrant define-function with advanced collision testing.
 *  Intended use in reentrant contexts (e.g type generators, local types, anonymous types)
 *  Thread safe with extended collision-awareness (--> Error in case of any form of collision).
 */
tp_t bcore_flect_type_self_d(       bcore_flect_self_s* self ); // takes over control of self
tp_t bcore_flect_type_self_c( const bcore_flect_self_s* self ); // stores a copy of self
tp_t bcore_flect_type_parse_src( sr_s src );
tp_t bcore_flect_type_parse_sc(  sc_t sc );
tp_t bcore_flect_type_parse_fa(  sc_t format, ... );

/** Defining reflection via creation function
 *  The function releases a newly created instance of self.
 *  The creator is lazily invoked.
 */
typedef bcore_flect_self_s* (*bcore_flect_create_self_fp)( void ); // function pointer to reflection creating function
void bcore_flect_define_creator( tp_t type, bcore_flect_create_self_fp creator );

bl_t  bcore_flect_exists(                       tp_t type ); // checks existence of type  (thread safe)
const bcore_flect_self_s* bcore_flect_try_self( tp_t type ); // returns NULL when type does not exist  (thread safe)
const bcore_flect_self_s* bcore_flect_get_self( tp_t type ); // error when type does not exist  (thread safe)

vd_t bcore_flect_signal( tp_t target, tp_t signal, vd_t object );

// Macros
#define DEFINE_CREATE_SELF( name, def )\
  static bcore_flect_self_s* name##_create_self( void ) \
  {\
      return bcore_flect_self_s_build_parse_sc( def, sizeof( name ) ); \
  }\

/**********************************************************************************************************************/

#endif // BCORE_FLECT_H

