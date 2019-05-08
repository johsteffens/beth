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
#include "bcore_amoeba.h"
#include "bcore_control.h"
#include "bcore_name_manager.h"
#include "bcore_function_manager.h"
#include "bcore_st.h"
#include "bcore_feature.h"

/**********************************************************************************************************************/

BCORE_FORWARD_OBJECT( bcore_sink );
BCORE_FORWARD_OBJECT( bcore_source );

/**********************************************************************************************************************/

typedef struct bcore_arr_tp_s bcore_arr_tp_s;

/// data encapsulation methods
enum
{
    BCORE_CAPS_START = 0,

    BCORE_CAPS_SOLID_STATIC,
    BCORE_CAPS_LINK_STATIC,
    BCORE_CAPS_LINK_TYPED,
    BCORE_CAPS_LINK_AWARE,
    BCORE_CAPS_ARRAY_DYN_SOLID_STATIC,
    BCORE_CAPS_ARRAY_DYN_SOLID_TYPED,
    BCORE_CAPS_ARRAY_DYN_LINK_STATIC,
    BCORE_CAPS_ARRAY_DYN_LINK_TYPED,
    BCORE_CAPS_ARRAY_DYN_LINK_AWARE,
    BCORE_CAPS_ARRAY_FIX_SOLID_STATIC,
    BCORE_CAPS_ARRAY_FIX_LINK_STATIC,
    BCORE_CAPS_ARRAY_FIX_LINK_AWARE,
    BCORE_CAPS_EXTERNAL_FUNC,      // specifies a static external function for perspective only (no data occupation in object)

    BCORE_CAPS_END,
};

/// encapsulation structures
typedef struct { vd_t  link;                                   } bcore_link_static_s;
typedef struct { vd_t  link; tp_t type;                        } bcore_link_typed_s;
typedef struct { vd_t  link;                                   } bcore_link_aware_s;

/** bcore_array_dyn_head_s matches the beginning of every dynamic array structure
 *  ptr has different meanings (either vd_t or vd_t*)
 */
typedef struct { vd_t   ptr; uz_t size; uz_t space;            } bcore_array_dyn_head_s;

typedef struct { vd_t  data; uz_t size; uz_t space;            } bcore_array_dyn_solid_static_s;
typedef struct { vd_t  data; uz_t size; uz_t space; tp_t type; } bcore_array_dyn_solid_typed_s;
typedef struct { vd_t* data; uz_t size; uz_t space;            } bcore_array_dyn_link_static_s;
typedef struct { vd_t* data; uz_t size; uz_t space; tp_t type; } bcore_array_dyn_link_typed_s;
typedef struct { vd_t* data; uz_t size; uz_t space;            } bcore_array_dyn_link_aware_s;

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

/// checks if encapsulation is an array
bl_t bcore_flect_caps_is_array( u2_t caps );
bl_t bcore_flect_caps_is_array_dyn( u2_t caps );
bl_t bcore_flect_caps_is_array_fix( u2_t caps );

//----------------------------------------------------------------------------------------------------------------------

// attribute flags
typedef struct bcore_flect_flags_s
{
    union
    {
        tp_t data; // collection of flags
        struct
        {
            // invisible to all perspectives (inst spect may only initialize and destroy the field)
            unsigned f_private   : 1;

            // interpreted type (no physical representation)
            // invisible to inst spect
            // used by via spect
            unsigned f_shell     : 1; // shell flag

            // visible to inst spect
            // hidden from via spect
            unsigned f_hidden    : 1; // hidden flag

            // visible to inst spect (automatically coupled with f_private)
            unsigned f_spect     : 1; // static link to perspective of this item

            // constant; requires initializer; no physical representation in object
            unsigned f_const     : 1;

            // item represents a feature (features are used in perspectives; features are not traced in inst perspective)
            unsigned f_feature   : 1;

            // used in conjunction with 'feature' to determine if the feature is always existing (either by default or by object-sided implementation)
            unsigned f_strict    : 1;

            // type represents a function pointer
            unsigned f_fp        : 1;

            // In instance perspective copy:
            //    * true: linked objects are deeply copied
            //    * false: linked objects are forked
            unsigned f_deep_copy : 1;

            // type is self aware; 'type' value of self_item can be 0 or represent a trait
            unsigned f_aware : 1;
        };
    };
} bcore_flect_flags_s;

//----------------------------------------------------------------------------------------------------------------------

typedef struct bcore_self_item_s bcore_self_item_s;
typedef struct bcore_self_item_s
{
    tp_t type; // hash of type
    tp_t name; // hash of name
    u2_t caps; // data encapsulation
    bcore_flect_flags_s flags;
    union
    {
        s3_t default_s3; // serves s0_t ... s3_t
        u3_t default_u3; // serves u0_t ... u3_t
        f3_t default_f3; // serves f2_t ... f3_t
        tp_t default_tp; // serves tp_t and external functions
        sz_t default_sz; // serves sz_t
        uz_t default_uz; // serves uz_t, offset_t
        uz_t default_bl; // serves bl_t
        uz_t array_fix_size; // size for fixed arrays
        umax_t default_umax;
        smax_t default_smax;
    };
    bcore_self_item_s* child_item;
} bcore_self_item_s;

void               bcore_self_item_s_init( bcore_self_item_s* o );
void               bcore_self_item_s_down( bcore_self_item_s* o );
void               bcore_self_item_s_copy( bcore_self_item_s* o, const bcore_self_item_s* src );
void               bcore_self_item_s_move( bcore_self_item_s* o,       bcore_self_item_s* src );
bcore_self_item_s* bcore_self_item_s_create();
bcore_self_item_s* bcore_self_item_s_create_plain( u2_t caps, tp_t type, tp_t name );
bcore_self_item_s* bcore_self_item_s_create_func( sc_t fname, fp_t func, sc_t type, sc_t name );
void               bcore_self_item_s_discard( bcore_self_item_s* o );
bcore_self_item_s* bcore_self_item_s_clone( const bcore_self_item_s* o );
st_s*              bcore_self_item_s_show( const bcore_self_item_s* o );
tp_t               bcore_self_item_s_fold_tp( const bcore_self_item_s* o, tp_t tp );
s2_t               bcore_self_item_s_cmp( const bcore_self_item_s* o1, const bcore_self_item_s* o2 );        // compares two items

bl_t bcore_self_item_s_has_default_value( const bcore_self_item_s* o );

/// returns instantiation size or 0 if size is not computable at this point
uz_t bcore_self_item_s_inst_size( const bcore_self_item_s* o );

/// returns instantiation alignments or 0 if value is not computable at this point
uz_t bcore_self_item_s_inst_align( const bcore_self_item_s* o );

void bcore_self_item_s_check_integrity( const bcore_self_item_s* o );

/**********************************************************************************************************************/

typedef struct bcore_self_body_s bcore_self_body_s;

void               bcore_self_body_s_init( bcore_self_body_s* o );
void               bcore_self_body_s_down( bcore_self_body_s* o );
bcore_self_body_s* bcore_self_body_s_create();
void               bcore_self_body_s_discard( bcore_self_body_s* o );
bcore_self_item_s* bcore_self_body_s_push( bcore_self_body_s* o, const bcore_self_item_s* item );
bcore_self_item_s* bcore_self_body_s_push_d( bcore_self_body_s* o, bcore_self_item_s* item );
st_s*              bcore_self_body_s_show( const bcore_self_body_s* o );
tp_t               bcore_self_body_s_fold_tp( const bcore_self_body_s* o, tp_t tp );
s2_t               bcore_self_body_s_cmp( const bcore_self_body_s* o1, const bcore_self_body_s* o2 );
void               bcore_self_body_s_set_complete( bcore_self_body_s* o, bl_t complete );
bl_t               bcore_self_body_s_get_complete( const bcore_self_body_s* o );
void               bcore_self_body_s_check_integrity( const bcore_self_body_s* o );

/**********************************************************************************************************************/

typedef struct bcore_self_s
{
    aware_t _;

    /// type of object being reflected
    tp_t type;

    /// trait of object
    tp_t trait;

    /// parent (-trait) of object  (used by perspectives)
    tp_t parent;

    /** size
     *  Represents sizeof(object-type);
     *  Mandatory for leaf-types (which are basically types without body)
     *  Optional for types with body:
     *   = 0: sizeof(type) is calculated in the instance perspective (e.g. for runtime generated types)
     *   > 0: sizeof(type) is calculated in the instance perspective and checked against bcore_self_s::size
     *        Perspective can produce a descriptive error in case of a mismatch.
     *        This feature is useful to detect the error that object's compile-time definition is out of
     *        sync with its reflection.
     */
    uz_t size;

    /** Body of type. Exposing the body is optional.
     *  If the body is exposed, declarations must be complete up to the last declared element.
     *  Otherwise, a perspective can not correctly compute the alignment of elements.
     *  A leaf type is defined as object without body.
     */
    bcore_self_body_s* body;
} bcore_self_s;

void                bcore_self_s_init( bcore_self_s* o );
void                bcore_self_s_init_plain( bcore_self_s* o, tp_t type, tp_t trait, uz_t size );
void                bcore_self_s_down( bcore_self_s* o );
void                bcore_self_s_copy( bcore_self_s* o, const bcore_self_s* src );
bcore_self_s* bcore_self_s_create();
bcore_self_s* bcore_self_s_clone( const bcore_self_s* o );
void                bcore_self_s_discard( bcore_self_s* o );

uz_t                     bcore_self_s_items_size( const bcore_self_s* o );
const bcore_self_item_s* bcore_self_s_get_item( const bcore_self_s* o, uz_t index );

bcore_self_item_s* bcore_self_s_push( bcore_self_s* o, const bcore_self_item_s* item );
bcore_self_item_s* bcore_self_s_push_d( bcore_self_s* o, bcore_self_item_s* item );
bcore_self_item_s* bcore_self_s_push_func( bcore_self_s* o, sc_t fname, fp_t func, sc_t type, sc_t name );

// namespace: function address constructed from object name and function name
bcore_self_item_s* bcore_self_s_push_ns_func( bcore_self_s* o, fp_t func, sc_t type, sc_t name );
bcore_self_item_s* bcore_self_s_push_ns_amoeba( bcore_self_s* o, bcore_amoebic_t func, sc_t name );

bcore_self_item_s* bcore_self_s_push_fp_set( bcore_self_s* o, bcore_fp_set func, sc_t name );
bcore_self_item_s* bcore_self_s_push_fp_get( bcore_self_s* o, bcore_fp_get func, sc_t name );
st_s*              bcore_self_s_show( const bcore_self_s* o );
void               bcore_self_s_check_integrity( const bcore_self_s* o );

/// special reflections
bcore_self_s* bcore_self_s_create_plain( tp_t type, tp_t trait, uz_t size ); // plain (primitive) self contained type

// creates anonymous array type ...
bcore_self_s* bcore_self_s_create_array_dyn_solid_static( tp_t item_type );
bcore_self_s* bcore_self_s_create_array_dyn_link_static(  tp_t item_type );
bcore_self_s* bcore_self_s_create_array_fix_solid_static( tp_t item_type, uz_t size );
bcore_self_s* bcore_self_s_create_array_fix_link_static(  tp_t item_type, uz_t size );
bcore_self_s* bcore_self_s_create_array_fix_link_aware(   uz_t size );

/** Creating a reflection by parsing a stream:
 *  Typical Format:
 *  <type-name> = [<trait-name> [<parent-name>]]
 *  {
 *      [<prefixes>] <type> [:] [<qualifier>] <name> [=<default>];
 *
 *      func <type> [:] <name> [= <ftype>];
 *
 *      [strict] feature <type> [:] <name> [ ~> <related_expression> ];
 *      ....
 *  }
 *
 *  <type>:
 *    <type name> | <type number> | typed
 *   ('typed' is to be deprecated an might not be globally supported)
 *
 *  colon:
 *    If ':' follows <type>, '_<name>' is appended to the type name. (In some constructions this reduces redundancy)
 *    Example: 'func bmath_fp:add' resolves to 'func bmath_fp_add add'
 *
 *  qualifier:
 *    *, =>   : (deep) link    // object is referenced and inst perspective takes full control incl. deep copy
 *       ->   : (shallow) link // object is referenced and inst perspective controls lifetime but only forks links (no cloning)
 *        []  : dynamic array
 *     [size] : fixed-size-array
 *
 *  prefixes: (multiple prefixes can be mixed)
 *     private : Invisible to perspectives (no tracing, no copying, no ownership). Exception: spect_inst may initialize the field
 *     shell   : No physical representation in object. values are provided by get, set functions (used by spect_via; invisible to spect_inst)
 *     hidden  : Invisible to spect_via (can be seen as complement of 'shell')
 *     spect   : Perspective of parent object (private shallow link). Initialized by spect_inst. Private to other perspectives.
 *     const   : Constant. Requires default value. No physical representation in object. Typically used as perspective-parameter.
 *     aware   : aware object (must be link or array of links); type can be a virtual type (e.g. trait); Example "aware bcore_inst* obj;"
 *
 *  Static Function:
 *    The reflection func indicates a function operating on or with the object of the reflection by taking
 *    a pointer to it as first argument. It does not occupy physical space in the object.
 *    func <type> <name> = <ftype>;  // ftype is the name for the function registered with BCORE_REGISTER_(F)FUNC
 *    func <type> <name>;            // ftype is the generic name: <object type>_<function name>
 *    Examples:
 *      func bmath_fp:add;
 *      func bmath_fp_add add;                                 // same as above
 *      func bmath_fp_add add = myobject_s_add;                // same as above in case object is 'myobject_s'
 *      func bmath_fp:vector_mul = bmath_vf3_s_mul;
 *      func bmath_fp_vector_mul vector_mul = bmath_vf3_s_mul; // same as above
 *
 *  Dynamic Function:
 *    A dynamic function is given by a function_pointer (fp_t or registered feature)
 *    It can optionally be initialized with a registered function.
 *    <type> <name> [= <ftype>];  // ftype is the name for the function registered with BCORE_REGISTER_(F)FUNC
 *    If <type> is a registered feature, then <ftype> must be of that feature. This is tested during parsing.
 *
 *    Examples:
 *      my_feature_fp my_func = my_registered_default_func;
 *
 *  Feature:
 *    [strict] feature  [<prefixes>] <type> [<qualifier>] <name> [=<default>] [~> <related expression> ];
 *    A feature governs dynamic binding between perspective and object.
 *    It is declared in the perspective and binds a perspective-item dynamically to the object-item.
 *    'strict' indicates that the relation must exist or the perspective construction produces an error.
 *    Otherwise the feature reverts to default in case binding could not be established.
 *    '~> <related expression>' specifies the exact binding desired. If left blank, canonic binding is used.
 *    Examples:
 *      feature bmath_fp_add fp_add ~> func bmath_fp_add add;
 *
 *    Canonic bindings:
 *      [strict] feature <spect-type> -> <name>;         // creates perspective of object
 *      [strict] feature <function-pointer-type> <name>; // binds to 'func <function-pointer-type> <name>'
 *
 *
 *  Special reflection cases:
 *    <type-name> = <assigned-name>                        // creates a copy of an existing reflection with new name
 *    <type-name> = [<trait-name>] { <expr>; <expr>; ... } // Specifying '...' marks the body as incomplete
 *        Caution: Incomplete body may only be used when the alignment from existing body elements is identical with
 *                 the alignment of the object.
 *
 *    No body: Allowed for certain leaf types, where alignment == object size.
 *
 */
bcore_self_s* bcore_self_s_parse_src(          sr_s src,             uz_t size_of, bl_t advanced_checks );
bcore_self_s* bcore_self_s_parse_source(       bcore_source* source, uz_t size_of, bl_t advanced_checks );

bcore_self_s* bcore_self_s_build_parse_src(    sr_s src,             uz_t size_of ); // with advanced checks
bcore_self_s* bcore_self_s_build_parse_source( bcore_source* source, uz_t size_of ); // with advanced checks
bcore_self_s* bcore_self_s_build_parse_sc(     sc_t text,            uz_t size_of ); // with advanced checks

//----------------------------------------------------------------------------------------------------------------------

tp_t bcore_self_s_fold_tp( const bcore_self_s* o, tp_t tp );
s2_t bcore_self_s_cmp( const bcore_self_s* o1, const bcore_self_s* o2 );

/// Query for external function of given type or name; either type or name may be 0 in which case it is interpreted as wildcard
fp_t bcore_self_s_try_external_fp( const bcore_self_s* o, tp_t type, tp_t name ); // returns NULL when not found
fp_t bcore_self_s_get_external_fp( const bcore_self_s* o, tp_t type, tp_t name ); // error when not found
bool bcore_self_s_is_aware(        const bcore_self_s* o                       ); // object has body and is aware

/** Returns a static object to be stored in the perspective.
 *  'type' specifies a function compatible to bcore_fp_create creating the static object.
 *  The static object is created in this function by calling bcore_fp_create.
 */
vd_t bcore_self_s_get_static( const bcore_self_s* o, tp_t type, tp_t name ); // error when not found
vd_t bcore_self_s_try_static( const bcore_self_s* o, tp_t type, tp_t name ); // returns NULL when not found

/** Returns a const object to be stored in the perspective.
 *  'type' specifies a function compatible to bcore_fp_create creating the static object.
 *  The static object is created in this function by calling bcore_fp_create.
 */
vc_t bcore_self_s_get_const( const bcore_self_s* o, tp_t type, tp_t name ); // error when not found
vc_t bcore_self_s_try_const( const bcore_self_s* o, tp_t type, tp_t name ); // returns NULL when not found

/** Checks proper definition of dependencies (existence);
 *  This check should be executed at a central place not before the reflection is ready to be used for generating instances.
 *  Proper places: When reflection is registered or when the instance is being created.
 *  At earlier times the reflection may be inconsistent (e.g. not all dependencies exist yet).
 */
void bcore_self_s_check_integrity( const bcore_self_s* o );

/** Creates forward struct declaration. (Auto code generation)
 *  Format:
 *  typedef struct 'name' 'name';
 */
void bcore_self_s_forward_struct_to_sink( const bcore_self_s* o, sz_t indent, bcore_sink* sink );

/// Creates compatible C-Structure body of reflection. (Auto code generation)
void bcore_self_s_struct_body_to_sink( const bcore_self_s* o, sz_t indent, bcore_sink* sink );
void bcore_self_s_struct_body_to_sink_single_line( const bcore_self_s* o, bcore_sink* sink );

/** Creates compatible C-Structure of reflection. (Auto code generation)
 *  Format:
 *  typedef struct 'name' { ... } 'name';
 */
void bcore_self_s_struct_to_sink( const bcore_self_s* o, sz_t indent, bcore_sink* sink );

/**********************************************************************************************************************/
/// Global reflection manager (thread safe)

bl_t  bcore_flect_exists(                 tp_t type );   // checks existence of type  (thread safe)
const bcore_self_s* bcore_flect_try_self( tp_t type );   // returns NULL when type does not exist  (thread safe)
const bcore_self_s* bcore_flect_get_self( tp_t type );   // error when type does not exist  (thread safe)
sz_t  bcore_flect_size();                                // number of registered reflections
void  bcore_flect_push_all_types( bcore_arr_tp_s* arr ); // pushes all registered types to array (thread safe)

/** Define functions:
 *  Registers a self reflection once.
 *  Intended use for global registry.
 *  Error in case the same reflection type is registered twice.
 */
tp_t bcore_flect_define_self_d(       bcore_self_s* self ); // takes over control of self; error if same
tp_t bcore_flect_define_self_c( const bcore_self_s* self ); // stores a copy of self
tp_t bcore_flect_define_parse_src( sr_s src );
tp_t bcore_flect_define_parse_sc( sc_t sc );
tp_t bcore_flect_define_parse_fa( sc_t sc, ... );

/** Type functions:
 *  Reentrant define-function with advanced collision testing.
 *  Intended use in reentrant contexts (e.g type generators, local types, anonymous types)
 *  Thread safe with extended collision-awareness (--> Error in case of any form of collision).
 *  See flect_selftest for examples.
 */
tp_t bcore_flect_type_self_d(       bcore_self_s* self ); // takes over control of self
tp_t bcore_flect_type_self_c( const bcore_self_s* self ); // stores a copy of self
tp_t bcore_flect_type_parse_src( sr_s src );
tp_t bcore_flect_type_parse_sc(  sc_t sc );
tp_t bcore_flect_type_parse_fa(  sc_t format, ... );

/** Defining reflection via creation function
 *  The function releases a newly created instance of self.
 *  The creator is lazily invoked.
 */
typedef bcore_self_s* (*bcore_flect_create_self_fp)( void ); // function pointer to reflection creating function
void bcore_flect_define_creator( tp_t type, bcore_flect_create_self_fp creator );

/** Late push to an already defined reflection.
 *
 *  Intended use:
 *     Adding specific new properties in dependent libraries,
 *     which might be used in new perspectives but never used
 *     in perspectives of parent libraries.
 *
 *  Checked Requirements:
 *     * bcore_flect_exists( type ) returns 'true'.
 *     * Behavior of inst or via perspectives must not change.
 *     * Type must not be anonymous
 *
 *  Unchecked Requirements:
 *     * Affected perspectives must not be instantiated at this point.
 *
 *  Best practice:
 *     * Use only during init-cycle of a library.
 *     * Only add external constants (e.g. functions) or internal const types.
 *     * In item->type prefer types defined in hosting library
 *     * Prefer using prefix of hosting library in item->name
 *
 */
void bcore_flect_push_item_d(  tp_t type, bcore_self_item_s* item );
void bcore_flect_push_ns_func( tp_t type, fp_t func, sc_t func_type, sc_t func_name );

//----------------------------------------------------------------------------------------------------------------------
// debugging

/** Enforces parsing and instantiating of all reflections registered at his point.
 *  Usage: Detecting syntax errors in reflection definition.
 *  Reasoning:
 *    The reflection-instantiation is usually deferred to the point where the type is first needed,
 *    which might delay syntax error detection.
 */
void bcore_flect_parse_all_flects();

/**********************************************************************************************************************/

vd_t bcore_flect_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

// Macros
#define BCORE_DEFINE_CREATE_SELF( name, def )\
    static bcore_self_s* name##_create_self( void ) \
    {\
        return bcore_self_s_build_parse_sc( def, sizeof( name ) ); \
    }

#define BCORE_REGISTER_TYPE( trait, name )\
    bcore_flect_define_self_d( bcore_self_s_create_plain( entypeof( #name ), TYPEOF_##trait, sizeof( name ) ) )

#define BCORE_REGISTER_FEATURE( name )\
    BCORE_REGISTER_TYPE( function_pointer, name )

#define BCORE_LINK_TYPED_S( name ) \
    union \
    { \
        bcore_link_typed_s name##_struc; \
        struct \
        { \
            vd_t name; \
            tp_t name##_type; \
        }; \
    }

#define BCORE_ARRAY_DYN_SOLID_STATIC_S( type, prefix ) \
    union \
    { \
        bcore_array_dyn_solid_static_s prefix##arr; \
        struct \
        { \
            type* prefix##data; \
            uz_t prefix##size; \
            uz_t prefix##space; \
        }; \
    }

#define BCORE_ARRAY_DYN_SOLID_TYPED_S( prefix ) \
    union \
    { \
        bcore_array_dyn_solid_typed_s prefix##arr; \
        struct \
        { \
            vd_t prefix##data; \
            uz_t prefix##size; \
            uz_t prefix##space; \
            tp_t prefix##type; \
        }; \
    }

#define BCORE_ARRAY_DYN_LINK_STATIC_S( type, prefix ) \
    union \
    { \
        bcore_array_dyn_link_static_s prefix##arr; \
        struct \
        { \
            type** prefix##data; \
            uz_t prefix##size; \
            uz_t prefix##space; \
        }; \
    }

#define BCORE_ARRAY_DYN_LINK_TYPED_S( prefix ) \
    union \
    { \
        bcore_array_dyn_link_typed_s prefix##arr; \
        struct \
        { \
            vd_t* prefix##data; \
            uz_t prefix##size; \
            uz_t prefix##space; \
            tp_t prefix##type; \
        }; \
    }

#define BCORE_ARRAY_DYN_LINK_AWARE_S( prefix ) \
    union \
    { \
        bcore_array_dyn_link_aware_s prefix##arr; \
        struct \
        { \
            vd_t* prefix##data; \
            uz_t prefix##size; \
            uz_t prefix##space; \
        }; \
    }

#define BCORE_OFFSET( object, offset ) ( ( vd_t* )( ( u0_t* )object + offset ) )

/// deprecated: prefer BCORE_DEFINE_OBJECT_FLAT
#define BCORE_DEFINE_FUNCTIONS_SELF_OBJECT_FLAT( name, def ) \
    BCORE_DEFINE_FUNCTIONS_OBJ_FLAT( name ) \
    BCORE_DEFINE_CREATE_SELF( name, def )

#define BCORE_DEFINE_OBJECT_FLAT( trait, name ) \
    static sc_t name##_def_g; \
    BCORE_DEFINE_FUNCTIONS_OBJ_FLAT( name ) \
    BCORE_DEFINE_CREATE_SELF( name, name##_def_g ) \
    static sc_t name##_def_g = #name " =" #trait

#define BCORE_REGISTER_OBJECT( name )\
    bcore_flect_define_creator( typeof( #name ), name##_create_self )

/**********************************************************************************************************************/

#endif // BCORE_FLECT_H

