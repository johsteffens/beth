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

/** Perspective for dynamic and fixed size arrays.
 *  The array perspective takes control over the first array-element of an object according to its reflection.
 *  'size'  specifies the number of valid elements in the array
 *  'space' represents internally pre-allocated memory area.
 *  It is size <= space when the array owns its data.                     (--> strong array)
 *  It is space == 0 at size > 0 when the array references external data. (--> weak array)
 *  Changing size does not change space unless the new size exceeds previous space.
 *  Changing space does not change size unless the new space is lower than previous size.
 *  Changes in size or space preserve the array's elements up to the lesser of the
 *  old and new size. Array elements might be relocated in memory when space changes.
 *  In linked arrays NULL is a valid element.
 */

#ifndef BCORE_SPECT_ARRAY_H
#define BCORE_SPECT_ARRAY_H

#include "bcore_spect_inst.h"
#include "bcore_flect.h"
#include "bcore_feature.h"
#include "bcore_arr.h"
#include "bcore_spect.h"

BCORE_DECLARE_SPECT( bcore_array )
{
    bcore_spect_header_s header;

    tp_t type_caps;
    uz_t size_fix; // >0 indicating fixed size array
    uz_t caps_offset;

    const bcore_inst_s* item_p;  // item-perspective; NULL for typed or aware arrays

    sr_s ( *get )( const bcore_array_s* p, vc_t o, uz_t index ); // returns indexed item; NULL reference if index is out of range or the linked item is NULL
    void ( *set )( const bcore_array_s* p, vd_t o, uz_t index, sr_s src ); // sets item at indexed position; if index is out of size, size is increased

    /// quick-access state parameters
    bl_t is_static;     // elements are static (type of elements need not be recorded)
    bl_t is_of_aware;   // elements are all self-aware
    bl_t is_of_links;   // elements are links (means they can be NULL);
    bl_t is_mono_typed; // elements have all the same type
};

/**********************************************************************************************************************/
// array type construction

/** Returns the type of a static solid dynamic array of 'type'.
 *  If not already existing, the array type is generated as
 *  { <typename> []; }
 */
tp_t bcore_array_dyn_solid_static_type_of( tp_t type );

/** Returns the type of a static link dynamic array of 'type'.
 *  If not already existing, the array type is generated as
 *  { <typename> * []; }
 */
tp_t bcore_array_dyn_link_static_type_of( tp_t type );

/** Returns the type of a static solid fixed array of 'type' and 'size'.
 *  If not already existing, the array type is generated as
 *  { <typename> [size]; }
 */
tp_t bcore_array_fix_solid_static_type_of( tp_t type, uz_t size );

/** Returns the type of a static link fixed array of 'type' and 'size'.
 *  If not already existing, the array type is generated as
 *  { <typename> * [size]; }
 */
tp_t bcore_array_fix_link_static_type_of( tp_t type, uz_t size );

/** Returns the type of a static link fixed array of 'type' and 'size'.
 *  If not already existing, the array type is generated as
 *  { aware * [size]; }
 */
tp_t bcore_array_fix_link_aware_type_of( uz_t size );

/**********************************************************************************************************************/

BCORE_FUNC_SPECT_CONST1_RET1_ARG0_MAP0( bcore_array, get_size,  uz_t ) // returns size
BCORE_FUNC_SPECT_CONST1_RET1_ARG0_MAP0( bcore_array, get_space, uz_t ) // returns space
BCORE_FUNC_SPECT_CONST1_RET1_ARG1_MAP0( bcore_array, get, sr_s, uz_t, index ) // returns indexed item; returns sr_null() if index is out of range or the linked item is NULL
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_array, set,    uz_t, index, sr_s, src ) // sets item at indexed position
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_array, set_s3, uz_t, index, s3_t, val ) // Sets item by converting s3_t into target type
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_array, set_u3, uz_t, index, u3_t, val ) // Sets item by converting u3_t into target type
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_array, set_f3, uz_t, index, f3_t, val ) // Sets item by converting f3_t into target type
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_array, set_sz, uz_t, index, sz_t, val ) // Sets item by converting sz_t into target type
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_array, set_uz, uz_t, index, uz_t, val ) // Sets item by converting uz_t into target type
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_array, set_sc, uz_t, index, sc_t, val ) // Sets item by converting sc_t into target type
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_array, set_bl, uz_t, index, bl_t, val ) // Sets item by converting bl_t into target type
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP0( bcore_array, set_size,  uz_t, size  ) // changes array size (keeping previous data); for linked arrays new items are NULL; otherwise new items are default initialized
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP0( bcore_array, set_space, uz_t, space ) // changes space (can affect size; set space to zero means clearing the array)
BCORE_FUNC_SPECT_CONST0_RET0_ARG0_MAP0( bcore_array, make_strong )            // turns a weak array reference into a strong one; no effect if array is strong
BCORE_FUNC_SPECT_CONST1_RET1_ARG0_MAP0( bcore_array, get_first, sr_s )        // returns first item; NULL if array is empty
BCORE_FUNC_SPECT_CONST1_RET1_ARG0_MAP0( bcore_array, get_last, sr_s )         // returns last item; NULL if array is empty
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP0( bcore_array, push,    sr_s, src )    // push
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP0( bcore_array, push_s3, s3_t, val )    // pushes (converted) value
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP0( bcore_array, push_u3, u3_t, val )    // pushes (converted) value
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP0( bcore_array, push_f3, f3_t, val )    // pushes (converted) value
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP0( bcore_array, push_sz, sz_t, val )    // pushes (converted) value
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP0( bcore_array, push_uz, uz_t, val )    // pushes (converted) value
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP0( bcore_array, push_sc, sc_t, val )    // pushes (converted) value
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP0( bcore_array, push_bl, bl_t, val )    // pushes (converted) value
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP0( bcore_array, push_array, sr_s, src ) // assumes src represents array (appends array)
BCORE_FUNC_SPECT_CONST0_RET0_ARG0_MAP0( bcore_array, pop )                   // removes last element from array
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP0( bcore_array, set_gtype, tp_t, type ) // changes global item-type on empty arrays;

static inline bl_t bcore_array_p_is_fixed     ( const bcore_array_s* p ) { return p->size_fix > 0; }  // checks if array has fixed size
static inline bl_t bcore_array_p_is_static    ( const bcore_array_s* p ) { return p->is_static; }     // checks if elements are static (type of elements need not be recorded)
static inline bl_t bcore_array_p_is_of_aware  ( const bcore_array_s* p ) { return p->is_of_aware; }   // checks if elements are all self-aware
static inline bl_t bcore_array_p_is_of_links  ( const bcore_array_s* p ) { return p->is_of_links; }   // checks if elements are links (means they can be NULL);
static inline bl_t bcore_array_p_is_mono_typed( const bcore_array_s* p ) { return p->is_mono_typed; } // checks if elements have all the same type

bl_t bcore_array_p_is_mutable_mono_typed( const bcore_array_s* p ); // checks if mono_typed and type can be changed (non-static)
bl_t bcore_array_p_is_multi_typed       ( const bcore_array_s* p ); // checks if elements can have different types
bl_t bcore_array_p_is_weak              ( const bcore_array_s* p, vc_t o ); // checks if array is a weak reference (defined by size > space)
tp_t bcore_array_p_get_static_type      ( const bcore_array_s* p );         // returns type if static, 0 otherwise;
tp_t bcore_array_p_get_mono_type        ( const bcore_array_s* p, vc_t o ); // returns type if monotyped, 0 otherwise;
tp_t bcore_array_p_get_type             ( const bcore_array_s* p, vc_t o, uz_t index ); // returns type of indexed element; returns 0 when type cannot be determined

/// Direct data access (data pointer dereferencing and stepping depends on array structure)
vc_t bcore_array_p_get_c_data    ( const bcore_array_s* p, vc_t o ); // returns arr_caps->data (note that this is either vc_t or vc_t* depending on linkage-indirection
vd_t bcore_array_p_get_d_data    ( const bcore_array_s* p, vd_t o ); // returns arr_caps->data (note that this is either vd_t or vd_t* depending on linkage-indirection
uz_t bcore_array_p_get_unit_size ( const bcore_array_s* p, vc_t o ); // spacing between data elements (item_p->size or sizeof(vd_t))

/// gets data and size at once (p_size muts point to a valid object)
vc_t bcore_array_p_get_c_data_size( const bcore_array_s* p, vc_t o, uz_t* p_size );
vd_t bcore_array_p_get_d_data_size( const bcore_array_s* p, vd_t o, uz_t* p_size );

/** Array operations
 *  The specified limits cover the index range [start, end-1]
 *  When end is larger than the array size, it is truncated to the array size.
 *  Using '-1' as argument for 'end' is allowed, which sets 'end' equal to 'size'
 *  When using non-negative values and start >= end the effective range is zero.
 *  Operations (max, min, sort, ...) use the compare perspective.
 */

// max/min/(merge-)sort within range (for minimum, set direction == -1)
BCORE_FUNC_SPECT_CONST1_RET1_ARG4_MAP0( bcore_array, max_f,       vc_t, uz_t, start, uz_t, end, bcore_cmp_f, cmp, s2_t, direction )
BCORE_FUNC_SPECT_CONST1_RET1_ARG4_MAP0( bcore_array, max_index_f, uz_t, uz_t, start, uz_t, end, bcore_cmp_f, cmp, s2_t, direction )
BCORE_FUNC_SPECT_CONST0_RET0_ARG4_MAP0( bcore_array, sort_f,            uz_t, start, uz_t, end, bcore_cmp_f, cmp, s2_t, direction )

BCORE_FUNC_SPECT_CONST1_RET1_ARG3_MAP0( bcore_array, max,       vc_t, uz_t, start, uz_t, end, s2_t, direction )
BCORE_FUNC_SPECT_CONST1_RET1_ARG3_MAP0( bcore_array, max_index, uz_t, uz_t, start, uz_t, end, s2_t, direction )
BCORE_FUNC_SPECT_CONST0_RET0_ARG3_MAP0( bcore_array, sort,            uz_t, start, uz_t, end, s2_t, direction )

/** Running a function on all elements; the function is deemed to be of signature void (*)( vd_t element_ptr );
 *  'element_ptr' is a pointer to the actual array element (no smart reference)
 */
BCORE_FUNC_SPECT_CONST0_RET0_ARG3_MAP0( bcore_array, do,              uz_t, start, uz_t, end, fp_t, func )

/** Running a function on all elements; the function is deemed to be of signature void (*)( vd_t arg, vd_t element_ptr );
 *  'element_ptr' is a pointer to the actual array element (no smart reference)
 */
BCORE_FUNC_SPECT_CONST0_RET0_ARG4_MAP0( bcore_array, do_arg,          uz_t, start, uz_t, end, fp_t, func, vd_t, arg )

/** Computes a sorted order from the array (array itself remains unchanged).
 *  The returned order has size end - start and refers to the array section
 *  specified by start and end.
 *  Computing an order can be more efficient than sorting the array itself
 *  when the array consists of nested elements.
 */
BCORE_FUNC_SPECT_CONST1_RET1_ARG4_MAP0( bcore_array, create_sorted_order_f, bcore_arr_uz_s*, uz_t, start, uz_t, end, bcore_cmp_f, cmp, s2_t, direction )
BCORE_FUNC_SPECT_CONST1_RET1_ARG3_MAP0( bcore_array, create_sorted_order,   bcore_arr_uz_s*, uz_t, start, uz_t, end,                   s2_t, direction )
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP0( bcore_array, reorder, const bcore_arr_uz_s*, order )

bl_t bcore_array_t_is_fixed             ( tp_t tp );
bl_t bcore_array_t_is_static            ( tp_t tp );
bl_t bcore_array_t_is_mono_typed        ( tp_t tp );
bl_t bcore_array_t_is_mutable_mono_typed( tp_t tp );
bl_t bcore_array_t_is_multi_typed       ( tp_t tp );
bl_t bcore_array_t_is_of_aware          ( tp_t tp );
bl_t bcore_array_t_is_of_links          ( tp_t tp );
tp_t bcore_array_t_get_static_type      ( tp_t tp );
tp_t bcore_array_t_get_mono_type        ( tp_t tp, vc_t o );
tp_t bcore_array_t_get_type             ( tp_t tp, vc_t o, uz_t index );
vc_t bcore_array_t_get_c_data           ( tp_t tp, vc_t o );
vd_t bcore_array_t_get_d_data           ( tp_t tp, vd_t o );
uz_t bcore_array_t_get_unit_size        ( tp_t tp, vc_t o );

bl_t bcore_array_a_is_fixed             ( vc_t o );
bl_t bcore_array_a_is_static            ( vc_t o );
bl_t bcore_array_a_is_mono_typed        ( vc_t o );
bl_t bcore_array_a_is_mutable_mono_typed( vc_t o );
bl_t bcore_array_a_is_multi_typed       ( vc_t o );
bl_t bcore_array_a_is_of_aware          ( vc_t o );
bl_t bcore_array_a_is_of_links          ( vc_t o );
tp_t bcore_array_a_get_static_type      ( vc_t o );
tp_t bcore_array_a_get_mono_type        ( vc_t o );
tp_t bcore_array_a_get_type             ( vc_t o, uz_t index );
vc_t bcore_array_a_get_c_data           ( vc_t o );
vd_t bcore_array_a_get_d_data           ( vd_t o );
uz_t bcore_array_a_get_unit_size        ( vc_t o );

bl_t bcore_array_x_is_fixed             ( sr_s o );
bl_t bcore_array_x_is_static            ( sr_s o );
bl_t bcore_array_x_is_mono_typed        ( sr_s o );
bl_t bcore_array_x_is_mutable_mono_typed( sr_s o );
bl_t bcore_array_x_is_multi_typed       ( sr_s o );
bl_t bcore_array_x_is_of_aware          ( sr_s o );
bl_t bcore_array_x_is_of_links          ( sr_s o );
tp_t bcore_array_x_get_static_type      ( sr_s o );
tp_t bcore_array_x_get_mono_type        ( sr_s o );
tp_t bcore_array_x_get_type             ( sr_s o, uz_t index );
vc_t bcore_array_x_get_c_data           ( sr_s o );
vd_t bcore_array_x_get_d_data           ( sr_s o );
uz_t bcore_array_x_get_unit_size        ( sr_s o );

bl_t bcore_array_r_is_fixed             ( const sr_s* o );
bl_t bcore_array_r_is_static            ( const sr_s* o );
bl_t bcore_array_r_is_mono_typed        ( const sr_s* o );
bl_t bcore_array_r_is_mutable_mono_typed( const sr_s* o );
bl_t bcore_array_r_is_multi_typed       ( const sr_s* o );
bl_t bcore_array_r_is_of_aware          ( const sr_s* o );
bl_t bcore_array_r_is_of_links          ( const sr_s* o );
tp_t bcore_array_r_get_static_type      ( const sr_s* o );
tp_t bcore_array_r_get_mono_type        ( const sr_s* o );
tp_t bcore_array_r_get_type             ( const sr_s* o, uz_t index );
vc_t bcore_array_r_get_c_data           ( const sr_s* o );
vd_t bcore_array_r_get_d_data           ( const sr_s* o );
uz_t bcore_array_r_get_unit_size        ( const sr_s* o );

/**********************************************************************************************************************/
/// macros for frequent array types

// ---------------------------------------------------------------------------------------------------------------------

#define BCORE_DECLARE_ARRAY_DYN_SOLID_STATIC( element_type, arr_name ) \
    BCORE_DECLARE_OBJECT( arr_name ) \
    { \
        aware_t _; \
        BCORE_ARRAY_DYN_SOLID_STATIC_S( element_type, ); \
    }; \
    void arr_name##_set_space( arr_name* o, uz_t space ); \
    void arr_name##_set_size(  arr_name* o, uz_t size  ); \
    static inline void arr_name##_clear( arr_name* o ) \
    { \
        arr_name##_set_size( o, 0 ); \
    } \
    void arr_name##_push(      arr_name* o, const element_type* v ); \
    void arr_name##_popv(      arr_name* o,       element_type* v ); \
    void arr_name##_popn(      arr_name* o )

#define BCORE_DEFINE_ARRAY_DYN_SOLID_STATIC_COMMON( element_type, arr_name ) \
    void arr_name##_set_space( arr_name* o, uz_t space ) { bcore_array_a_set_space( ( bcore_array* )o, space ); } \
    void arr_name##_set_size(  arr_name* o, uz_t size  ) { bcore_array_a_set_size(  ( bcore_array* )o, size  ); } \
    void arr_name##_popv(      arr_name* o,       element_type* v ) { if( o->size > 0 ) { o->size--; element_type##_copy( v, &o->data[ o->size ] ); element_type##_down( &o->data[ o->size ] ); } } \
    void arr_name##_popn(      arr_name* o                        ) { if( o->size > 0 ) { o->size--;                                                element_type##_down( &o->data[ o->size ] ); } }

#define BCORE_DEFINE_ARRAY_DYN_SOLID_STATIC( element_type, arr_name ) \
    BCORE_DEFINE_ARRAY_DYN_SOLID_STATIC_COMMON( element_type, arr_name ) \
    void arr_name##_push(      arr_name* o, const element_type* v ) { bcore_array_a_push(  ( bcore_array* )o, sr_twc( TYPEOF_##element_type, v ) ); } \
    BCORE_DEFINE_OBJECT_INST( bcore_array, arr_name ) "{ aware_t _; " #element_type " [] arr; }"

#define BCORE_DEFINE_ARRAY_DYN_SOLID_STATIC_AUT( element_type, arr_name ) \
    BCORE_DEFINE_ARRAY_DYN_SOLID_STATIC_COMMON( element_type, arr_name ) \
    void arr_name##_push(      arr_name* o, const element_type* v ) { bcore_array_a_push(  ( bcore_array* )o, sr_twc( typeof( #element_type ), v ) ); } \
    BCORE_DEFINE_OBJECT_INST_AUT( bcore_array, arr_name ) "{ aware_t _; " #element_type " [] arr; }"

// ---------------------------------------------------------------------------------------------------------------------

#define BCORE_DECLARE_ARRAY_DYN_LINK_STATIC( element_type, arr_name ) \
    BCORE_DECLARE_OBJECT( arr_name ) \
    { \
        aware_t _; \
        BCORE_ARRAY_DYN_LINK_STATIC_S( element_type, ); \
    }; \
    void arr_name##_set_space( arr_name* o, uz_t space ); \
    void arr_name##_set_size(  arr_name* o, uz_t size  ); \
    static inline void arr_name##_clear( arr_name* o ) \
    { \
        arr_name##_set_size( o, 0 ); \
    } \
    void arr_name##_push( arr_name* o, const element_type* v ); \
    void arr_name##_popv( arr_name* o,       element_type* v ); \
    void arr_name##_popn( arr_name* o )

#define BCORE_DEFINE_ARRAY_DYN_LINK_STATIC_COMMON( element_type, arr_name ) \
    void arr_name##_set_space( arr_name* o, uz_t space ) { bcore_array_a_set_space( ( bcore_array* )o, space ); } \
    void arr_name##_set_size(  arr_name* o, uz_t size  ) { bcore_array_a_set_size(  ( bcore_array* )o, size  ); } \
    void arr_name##_popv(      arr_name* o, element_type* v ) { if( o->size > 0 ) { o->size--; element_type##_copy( v, o->data[ o->size ] ); element_type##_discard( o->data[ o->size ] ); o->data[ o->size ] = NULL; } } \
    void arr_name##_popn(      arr_name* o                  ) { if( o->size > 0 ) { o->size--;                                               element_type##_discard( o->data[ o->size ] ); o->data[ o->size ] = NULL; } }

#define BCORE_DEFINE_ARRAY_DYN_LINK_STATIC( element_type, arr_name ) \
    BCORE_DEFINE_ARRAY_DYN_LINK_STATIC_COMMON( element_type, arr_name ) \
    void arr_name##_push(   arr_name* o, const element_type* v ) { bcore_array_a_push( ( bcore_array* )o, sr_twc( TYPEOF_##element_type, v ) ); } \
    void arr_name##_push_d( arr_name* o,       element_type* v ) { bcore_array_a_push( ( bcore_array* )o, sr_tsm( TYPEOF_##element_type, v ) ); } \
    BCORE_DEFINE_OBJECT_INST( bcore_array, arr_name ) "{ aware_t _; " #element_type " => [] arr; }"

#define BCORE_DEFINE_ARRAY_DYN_LINK_STATIC_AUT( element_type, arr_name ) \
    BCORE_DEFINE_ARRAY_DYN_LINK_STATIC_COMMON( element_type, arr_name ) \
    void arr_name##_push(   arr_name* o, const element_type* v ) { bcore_array_a_push(( bcore_array* )o, sr_twc( typeof( #element_type ), v ) ); } \
    void arr_name##_push_d( arr_name* o,       element_type* v ) { bcore_array_a_push(( bcore_array* )o, sr_tsm( typeof( #element_type ), v ) ); } \
    BCORE_DEFINE_OBJECT_INST_AUT( bcore_array, arr_name ) "{ aware_t _; " #element_type " => [] arr; }"

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

vd_t bcore_spect_array_signal_handler( const bcore_signal_s* o );

#endif
