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

/** Array Perspective
 *  The array perspective addresses the first array-element of an object's reflection and provides array
 *  related functionality.
 *  'size'  specifies the number of valid elements in the array
 *  'space' represents internally preallocated memory area.
 *  It is size <= space.
 *  size and space changes preserve the array's content up to the lesser of the old and new size.
 *  In linked arrays NULL is a valid element.
 */

#ifndef BCORE_SPECT_ARRAY_H
#define BCORE_SPECT_ARRAY_H

#include "bcore_spect_inst.h"
#include "bcore_flect.h"
#include "bcore_features.h"
#include "bcore_arr.h"
#include "bcore_spect.h"

typedef struct bcore_array bcore_array;

BCORE_DECLARE_SPECT( bcore_array_s )
{
    bcore_spect_header_s header;

    tp_t type_caps;

    sz_t size_fix; // >0 indicating fixed size array

    sz_t caps_offset;
    const bcore_inst_s* item_p;  // item-perspective; NULL for typed or aware arrays

    sr_s ( *get )( const bcore_array_s* p, vc_t o, sz_t index ); // returns indexed item; NULL reference if index is out of range or the linked item is NULL
    void ( *set )( const bcore_array_s* p, vd_t o, sz_t index, sr_s src ); // sets item at indexed position; if index is out of size, size is increased

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
tp_t bcore_array_fix_solid_static_type_of( tp_t type, sz_t size );

/** Returns the type of a static link fixed array of 'type' and 'size'.
 *  If not already existing, the array type is generated as
 *  { <typename> * [size]; }
 */
tp_t bcore_array_fix_link_static_type_of( tp_t type, sz_t size );

/** Returns the type of a static link fixed array of 'type' and 'size'.
 *  If not already existing, the array type is generated as
 *  { aware * [size]; }
 */
tp_t bcore_array_fix_link_aware_type_of( sz_t size );

/**********************************************************************************************************************/

BCORE_FUNC_SPECT_CONST1_RET1_ARG0_MAP0( bcore_array, get_size,  sz_t ) // returns size
BCORE_FUNC_SPECT_CONST1_RET1_ARG0_MAP0( bcore_array, get_space, sz_t ) // returns space
BCORE_FUNC_SPECT_CONST1_RET1_ARG1_MAP0( bcore_array, get, sr_s, sz_t, index ) // returns indexed item; if index is out of range or the linked item is NULL
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_array, set,    sz_t, index, sr_s, src ) // sets item at indexed position
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_array, set_s3, sz_t, index, s3_t, val ) // Sets item by converting s3_t into target type
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_array, set_u3, sz_t, index, u3_t, val ) // Sets item by converting u3_t into target type
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_array, set_f3, sz_t, index, f3_t, val ) // Sets item by converting f3_t into target type
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_array, set_sz, sz_t, index, sz_t, val ) // Sets item by converting sz_t into target type
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_array, set_sc, sz_t, index, sc_t, val ) // Sets item by converting sc_t into target type
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_array, set_bl, sz_t, index, bl_t, val ) // Sets item by converting bl_t into target type
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP0( bcore_array, set_size,  sz_t, size  ) // changes array size (keeping previous data); for linked arrays new items are NULL
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP0( bcore_array, set_space, sz_t, space ) // changes space (can affect size; set space to zero means clearing the array)
BCORE_FUNC_SPECT_CONST0_RET0_ARG0_MAP0( bcore_array, make_strong )            // turns a weak array reference into a strong one; no effect if array is strong
BCORE_FUNC_SPECT_CONST1_RET1_ARG0_MAP0( bcore_array, get_first, sr_s )        // returns first item; NULL if array is empty
BCORE_FUNC_SPECT_CONST1_RET1_ARG0_MAP0( bcore_array, get_last, sr_s )         // returns last item; NULL if array is empty
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP0( bcore_array, push,    sr_s, src )    // push
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP0( bcore_array, push_s3, s3_t, val )    // pushes (converted) value
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP0( bcore_array, push_u3, u3_t, val )    // pushes (converted) value
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP0( bcore_array, push_f3, f3_t, val )    // pushes (converted) value
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP0( bcore_array, push_sz, sz_t, val )    // pushes (converted) value
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP0( bcore_array, push_sc, sc_t, val )    // pushes (converted) value
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP0( bcore_array, push_bl, bl_t, val )    // pushes (converted) value
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP0( bcore_array, push_array, sr_s, src ) // assumes src represents array (appends array)
BCORE_FUNC_SPECT_CONST0_RET0_ARG0_MAP0( bcore_array, pop )                   // removes last element from array
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP0( bcore_array, set_gtype, tp_t, type ) // changes global item-type on empty arrays;

bl_t bcore_array_p_is_fixed             ( const bcore_array_s* p ); // checks if array has fixed size
bl_t bcore_array_p_is_static            ( const bcore_array_s* p ); // checks if elements are static (type of elements need not be recorded)
bl_t bcore_array_p_is_mono_typed        ( const bcore_array_s* p ); // checks if elements have all the same type
bl_t bcore_array_p_is_mutable_mono_typed( const bcore_array_s* p ); // checks if mono_typed and type can be changed (non-static)
bl_t bcore_array_p_is_multi_typed       ( const bcore_array_s* p ); // checks if elements can have different types
bl_t bcore_array_p_is_of_aware          ( const bcore_array_s* p ); // checks if elements are all self-aware
bl_t bcore_array_p_is_of_links          ( const bcore_array_s* p ); // checks if elements are links (means they can be NULL);
bl_t bcore_array_p_is_weak              ( const bcore_array_s* p, vc_t o ); // checks if array is a weak reference (defined by size > space)
tp_t bcore_array_p_get_static_type      ( const bcore_array_s* p );         // returns type if static, 0 otherwise;
tp_t bcore_array_p_get_mono_type        ( const bcore_array_s* p, vc_t o ); // returns type if monotyped, 0 otherwise;
tp_t bcore_array_p_get_type             ( const bcore_array_s* p, vc_t o, sz_t index ); // returns type of indexed element; returns 0 when type cannot be determined

/// Direct data access (data pointer dereferencing and stepping depends on array structure)
vc_t bcore_array_p_get_c_data    ( const bcore_array_s* p, vc_t o ); // returns arr_caps->data (note that this is either vc_t or vc_t* depending on linkage-indirection
vd_t bcore_array_p_get_d_data    ( const bcore_array_s* p, vd_t o ); // returns arr_caps->data (note that this is either vd_t or vd_t* depending on linkage-indirection
sz_t bcore_array_p_get_unit_size ( const bcore_array_s* p, vc_t o ); // spacing between data elements (item_p->size or sizeof(vd_t))

/** Array operations
 *  The specified limits cover the index range [start, end-1]
 *  When end is larger than the array size, it is truncated to the array size.
 *  Using '-1' as argument for 'end' is allowed, which sets 'end' equal to 'size'
 *  When using non-negative values and start >= end the effective range is zero.
 *  Operations (max, min, sort, ...) use the compare perspective.
 */

// max/min/(merge-)sort within range (for minimum, set direction == -1)
BCORE_FUNC_SPECT_CONST1_RET1_ARG4_MAP0( bcore_array, max_f,       vc_t, sz_t, start, sz_t, end, bcore_cmp_f, cmp, s2_t, direction )
BCORE_FUNC_SPECT_CONST1_RET1_ARG4_MAP0( bcore_array, max_index_f, sz_t, sz_t, start, sz_t, end, bcore_cmp_f, cmp, s2_t, direction )
BCORE_FUNC_SPECT_CONST0_RET0_ARG4_MAP0( bcore_array, sort_f,            sz_t, start, sz_t, end, bcore_cmp_f, cmp, s2_t, direction )

BCORE_FUNC_SPECT_CONST1_RET1_ARG3_MAP0( bcore_array, max,       vc_t, sz_t, start, sz_t, end, s2_t, direction )
BCORE_FUNC_SPECT_CONST1_RET1_ARG3_MAP0( bcore_array, max_index, sz_t, sz_t, start, sz_t, end, s2_t, direction )
BCORE_FUNC_SPECT_CONST0_RET0_ARG3_MAP0( bcore_array, sort,            sz_t, start, sz_t, end, s2_t, direction )

// running a function on all elements; the function is deemed to be of signature void (*)( vd_t );
BCORE_FUNC_SPECT_CONST0_RET0_ARG3_MAP0( bcore_array, do,              sz_t, start, sz_t, end, fp_t, func )

/** Computes a sorted order from the array (array itself remains unchanged).
 *  The returned order has size end - start and refers to the array section
 *  specified by start and end.
 *  Computing an order can be more efficient than sorting the array itself
 *  when the array consists of nested elements.
 */
BCORE_FUNC_SPECT_CONST1_RET1_ARG4_MAP0( bcore_array, create_sorted_order_f, bcore_arr_sz_s*, sz_t, start, sz_t, end, bcore_cmp_f, cmp, s2_t, direction )
BCORE_FUNC_SPECT_CONST1_RET1_ARG3_MAP0( bcore_array, create_sorted_order,   bcore_arr_sz_s*, sz_t, start, sz_t, end,                   s2_t, direction )
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP0( bcore_array, reorder, const bcore_arr_sz_s*, order )

bl_t bcore_array_t_is_fixed             ( tp_t tp );
bl_t bcore_array_t_is_static            ( tp_t tp );
bl_t bcore_array_t_is_mono_typed        ( tp_t tp );
bl_t bcore_array_t_is_mutable_mono_typed( tp_t tp );
bl_t bcore_array_t_is_multi_typed       ( tp_t tp );
bl_t bcore_array_t_is_of_aware          ( tp_t tp );
bl_t bcore_array_t_is_of_links          ( tp_t tp );
tp_t bcore_array_t_get_static_type      ( tp_t tp );
tp_t bcore_array_t_get_mono_type        ( tp_t tp, vc_t o );
tp_t bcore_array_t_get_type             ( tp_t tp, vc_t o, sz_t index );
vc_t bcore_array_t_get_c_data           ( tp_t tp, vc_t o );
vd_t bcore_array_t_get_d_data           ( tp_t tp, vd_t o );
sz_t bcore_array_t_get_unit_size        ( tp_t tp, vc_t o );


bl_t bcore_array_a_is_fixed             ( vc_t o );
bl_t bcore_array_a_is_static            ( vc_t o );
bl_t bcore_array_a_is_mono_typed        ( vc_t o );
bl_t bcore_array_a_is_mutable_mono_typed( vc_t o );
bl_t bcore_array_a_is_multi_typed       ( vc_t o );
bl_t bcore_array_a_is_of_aware          ( vc_t o );
bl_t bcore_array_a_is_of_links          ( vc_t o );
tp_t bcore_array_a_get_static_type      ( vc_t o );
tp_t bcore_array_a_get_mono_type        ( vc_t o );
tp_t bcore_array_a_get_type             ( vc_t o, sz_t index );
vc_t bcore_array_a_get_c_data           ( vc_t o );
vd_t bcore_array_a_get_d_data           ( vd_t o );
sz_t bcore_array_a_get_unit_size        ( vc_t o );

bl_t bcore_array_x_is_fixed             ( sr_s o );
bl_t bcore_array_x_is_static            ( sr_s o );
bl_t bcore_array_x_is_mono_typed        ( sr_s o );
bl_t bcore_array_x_is_mutable_mono_typed( sr_s o );
bl_t bcore_array_x_is_multi_typed       ( sr_s o );
bl_t bcore_array_x_is_of_aware          ( sr_s o );
bl_t bcore_array_x_is_of_links          ( sr_s o );
tp_t bcore_array_x_get_static_type      ( sr_s o );
tp_t bcore_array_x_get_mono_type        ( sr_s o );
tp_t bcore_array_x_get_type             ( sr_s o, sz_t index );
vc_t bcore_array_x_get_c_data           ( sr_s o );
vd_t bcore_array_x_get_d_data           ( sr_s o );
sz_t bcore_array_x_get_unit_size        ( sr_s o );

bl_t bcore_array_r_is_fixed             ( const sr_s* o );
bl_t bcore_array_r_is_static            ( const sr_s* o );
bl_t bcore_array_r_is_mono_typed        ( const sr_s* o );
bl_t bcore_array_r_is_mutable_mono_typed( const sr_s* o );
bl_t bcore_array_r_is_multi_typed       ( const sr_s* o );
bl_t bcore_array_r_is_of_aware          ( const sr_s* o );
bl_t bcore_array_r_is_of_links          ( const sr_s* o );
tp_t bcore_array_r_get_static_type      ( const sr_s* o );
tp_t bcore_array_r_get_mono_type        ( const sr_s* o );
tp_t bcore_array_r_get_type             ( const sr_s* o, sz_t index );
vc_t bcore_array_r_get_c_data           ( const sr_s* o );
vd_t bcore_array_r_get_d_data           ( const sr_s* o );
sz_t bcore_array_r_get_unit_size        ( const sr_s* o );

/**********************************************************************************************************************/

vd_t bcore_spect_array_signal_handler( const bcore_signal_s* o );

#endif
