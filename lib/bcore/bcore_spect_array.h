/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

/** Array Perspective
 *  The array perspective addresses the first array element of an object and provide array
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

typedef struct bcore_array_s bcore_array_s;

typedef struct bcore_array_s
{
    aware_t p_type; // type of perspective
    tp_t    o_type; // type of object

    tp_t caps_type;
    sz_t caps_offset;
    const bcore_inst_s* item_p;  // item-perspective; NULL for typed or aware arrays

    sz_t ( *get_size    )( const bcore_array_s* p, vc_t o );             // returns size
    sz_t ( *get_space   )( const bcore_array_s* p, vc_t o );             // returns space
    rf_s ( *get         )( const bcore_array_s* p, vc_t o, sz_t index ); // returns indexed item; NULL reference if index is out of range or the linked item is NULL
    rf_s ( *set         )( const bcore_array_s* p, vd_t o, sz_t index, rf_s src ); // sets item at indexed position; if index is out of size, size is increased
//    vc_t ( *get_c       )( const bcore_array_s* p, vc_t o, sz_t index ); // returns indexed item; NULL if index is out of range or the linked item is NULL
//    vd_t ( *get_d       )( const bcore_array_s* p, vd_t o, sz_t index ); // returns indexed item; NULL if index is out of range or the linked item is NULL
//    vd_t ( *set_c       )( const bcore_array_s* p, vd_t o, sz_t index, vc_t src ); // sets item at indexed position by copying src if src != NULL; if index is out of size, size is increased
//    vd_t ( *set_typed_c )( const bcore_array_s* p, vd_t o, sz_t index, tp_t otp, vc_t src ); // sets item at indexed position by copying and converting src if src != NULL; if index is out of size, size is increased
//    vd_t ( *set_d       )( const bcore_array_s* p, vd_t o, sz_t index, vd_t src ); // sets item at indexed position by taking ownership of src (catenated arrays copy and discard src if src != NULL)

} bcore_array_s;

bcore_flect_self_s* bcore_array_s_create_self( void );

const bcore_array_s* bcore_array_s_get_typed( tp_t type );

static inline const bcore_array_s* bcore_array_s_get_aware( vc_t obj )
{
    return bcore_array_s_get_typed( *( const aware_t* )obj );
}

/**********************************************************************************************************************/
// array construction

/** Constructs and returns an array of type if not already existing.
 *  The array type constructed is
 *  <typename>__static_array = { <typename> []; }
 */
tp_t bcore_static_array_type_of( tp_t type );

/** Constructs and returns an array of type if not already existing.
 *  The array type constructed is
 *  <typename>__static_link_array = { <typename> * []; }
 */
tp_t bcore_static_link_array_type_of( tp_t type );

/**********************************************************************************************************************/

sz_t bcore_array_spect_get_size     ( const bcore_array_s* p, vc_t o );             // returns size
sz_t bcore_array_spect_get_space    ( const bcore_array_s* p, vc_t o );             // returns space
rf_s bcore_array_spect_get          ( const bcore_array_s* p, vc_t o, sz_t index ); // returns indexed item; NULL if index is out of range or the linked item is NULL
rf_s bcore_array_spect_set          ( const bcore_array_s* p, vd_t o, sz_t index, rf_s src ); // sets item at indexed position
rf_s bcore_array_spect_set_s3       ( const bcore_array_s* p, vd_t o, sz_t index, s3_t val ); // Sets item by converting s3_t into target type
rf_s bcore_array_spect_set_u3       ( const bcore_array_s* p, vd_t o, sz_t index, u3_t val ); // Sets item by converting u3_t into target type
rf_s bcore_array_spect_set_f3       ( const bcore_array_s* p, vd_t o, sz_t index, f3_t val ); // Sets item by converting f3_t into target type
rf_s bcore_array_spect_set_sz       ( const bcore_array_s* p, vd_t o, sz_t index, sz_t val ); // Sets item by converting sz_t into target type
rf_s bcore_array_spect_set_sc       ( const bcore_array_s* p, vd_t o, sz_t index, sc_t val ); // Sets item by converting sc_t into target type
rf_s bcore_array_spect_set_bl       ( const bcore_array_s* p, vd_t o, sz_t index, bl_t val ); // Sets item by converting bl_t into target type
void bcore_array_spect_set_size     ( const bcore_array_s* p, vd_t o, sz_t size  ); // changes array size (keeping previous data); for linked arrays new items are NULL
void bcore_array_spect_set_space    ( const bcore_array_s* p, vd_t o, sz_t space ); // changes space (can affect size; set space to zero means clearing the array)
rf_s bcore_array_spect_get_first    ( const bcore_array_s* p, vc_t o );             // returns first item; NULL if array is empty
rf_s bcore_array_spect_get_last     ( const bcore_array_s* p, vc_t o );             // returns last item; NULL if array is empty
rf_s bcore_array_spect_push         ( const bcore_array_s* p, vd_t o, rf_s src );   // push
rf_s bcore_array_spect_push_s3      ( const bcore_array_s* p, vd_t o, s3_t val );   // pushes (converted) value
rf_s bcore_array_spect_push_u3      ( const bcore_array_s* p, vd_t o, u3_t val );   // pushes (converted) value
rf_s bcore_array_spect_push_f3      ( const bcore_array_s* p, vd_t o, f3_t val );   // pushes (converted) value
rf_s bcore_array_spect_push_sz      ( const bcore_array_s* p, vd_t o, sz_t val );   // pushes (converted) value
rf_s bcore_array_spect_push_sc      ( const bcore_array_s* p, vd_t o, sc_t val );   // pushes (converted) value
rf_s bcore_array_spect_push_bl      ( const bcore_array_s* p, vd_t o, bl_t val );   // pushes (converted) value
void bcore_array_spect_pop          ( const bcore_array_s* p, vd_t o );             // removes last element from array
void bcore_array_spect_set_gtype    ( const bcore_array_s* p, vd_t o, tp_t type  ); // changes global item-type on empty arrays;

bl_t bcore_array_spect_is_static            ( const bcore_array_s* p ); // checks if elements are static (type of elements need not be recorded)
bl_t bcore_array_spect_is_mono_typed        ( const bcore_array_s* p ); // checks if elements have all the same type
bl_t bcore_array_spect_is_mutable_mono_typed( const bcore_array_s* p ); // checks if mono_typed and type can be changed (non-static)
bl_t bcore_array_spect_is_multi_typed       ( const bcore_array_s* p ); // checks if elements can have different types
bl_t bcore_array_spect_is_of_aware          ( const bcore_array_s* p ); // checks if elements are all self-aware
bl_t bcore_array_spect_is_of_links          ( const bcore_array_s* p ); // checks if elements are links (means they can be NULL);
tp_t bcore_array_spect_get_static_type      ( const bcore_array_s* p );         // returns type if static, 0 otherwise;
tp_t bcore_array_spect_get_mono_type        ( const bcore_array_s* p, vc_t o ); // returns type if monotyped, 0 otherwise;
tp_t bcore_array_spect_get_type             ( const bcore_array_s* p, vc_t o, sz_t index ); // returns type of indexed element; returns 0 when type cannot be determined

/// Direct data access (data pointer dereferencing and stepping depends on array structure)
vc_t bcore_array_spect_get_c_data    ( const bcore_array_s* p, vc_t o ); // returns arr_caps->data (note that this is either vc_t or vc_t* depending on linkage-indirection
vd_t bcore_array_spect_get_d_data    ( const bcore_array_s* p, vd_t o ); // returns arr_caps->data (note that this is either vd_t or vd_t* depending on linkage-indirection
sz_t bcore_array_spect_get_unit_size ( const bcore_array_s* p, vc_t o ); // spacing between data elements (item_p->size or sizeof(vd_t))

/** Array operations
 *  The specified limits cover the index range [start, end-1]
 *  When end is larger than the array size, it is truncated to the array size.
 *  Using '-1' as argument for 'end' is allowed, which sets 'end' equal to 'size'
 *  When using non-negative values and start >= end the effective range is zero.
 *  Operations (max, min, sort, ...) requiring an order on objects use the compare perspective.
 */

// maximum/minimum within range (order = -1: minimum)
vc_t bcore_array_spect_max(       const bcore_array_s* p, vc_t o, sz_t start, sz_t end, s2_t order );
sz_t bcore_array_spect_max_index( const bcore_array_s* p, vc_t o, sz_t start, sz_t end, s2_t order );

// (merge-)sort within index range [start, end-1]
void bcore_array_spect_sort(      const bcore_array_s* p, vd_t o, sz_t start, sz_t end, s2_t order );


sz_t bcore_array_typed_get_size             ( tp_t tp, vc_t o );
sz_t bcore_array_typed_get_space            ( tp_t tp, vc_t o );
rf_s bcore_array_typed_get                  ( tp_t tp, vc_t o, sz_t index );
rf_s bcore_array_typed_set                  ( tp_t tp, vd_t o, sz_t index, rf_s src );
rf_s bcore_array_typed_set_s3               ( tp_t tp, vd_t o, sz_t index, s3_t val );
rf_s bcore_array_typed_set_u3               ( tp_t tp, vd_t o, sz_t index, u3_t val );
rf_s bcore_array_typed_set_f3               ( tp_t tp, vd_t o, sz_t index, f3_t val );
rf_s bcore_array_typed_set_sz               ( tp_t tp, vd_t o, sz_t index, sz_t val );
rf_s bcore_array_typed_set_sc               ( tp_t tp, vd_t o, sz_t index, sc_t val );
rf_s bcore_array_typed_set_bl               ( tp_t tp, vd_t o, sz_t index, bl_t val );
void bcore_array_typed_set_size             ( tp_t tp, vd_t o, sz_t size  );
void bcore_array_typed_set_space            ( tp_t tp, vd_t o, sz_t space );
rf_s bcore_array_typed_get_first            ( tp_t tp, vc_t o );
rf_s bcore_array_typed_get_last             ( tp_t tp, vc_t o );
rf_s bcore_array_typed_push                 ( tp_t tp, vd_t o, rf_s src );
rf_s bcore_array_typed_push_s3              ( tp_t tp, vd_t o, s3_t val );
rf_s bcore_array_typed_push_u3              ( tp_t tp, vd_t o, u3_t val );
rf_s bcore_array_typed_push_f3              ( tp_t tp, vd_t o, f3_t val );
rf_s bcore_array_typed_push_sz              ( tp_t tp, vd_t o, sz_t val );
rf_s bcore_array_typed_push_sc              ( tp_t tp, vd_t o, sc_t val );
rf_s bcore_array_typed_push_bl              ( tp_t tp, vd_t o, bl_t val );
void bcore_array_typed_pop                  ( tp_t tp, vd_t o );
void bcore_array_typed_set_gtype            ( tp_t tp, vd_t o, tp_t type  );
bl_t bcore_array_typed_is_static            ( tp_t tp );
bl_t bcore_array_typed_is_mono_typed        ( tp_t tp );
bl_t bcore_array_typed_is_mutable_mono_typed( tp_t tp );
bl_t bcore_array_typed_is_multi_typed       ( tp_t tp );
bl_t bcore_array_typed_is_of_aware          ( tp_t tp );
bl_t bcore_array_typed_is_of_links          ( tp_t tp );
tp_t bcore_array_typed_get_static_type      ( tp_t tp );
tp_t bcore_array_typed_get_mono_type        ( tp_t tp, vc_t o );
tp_t bcore_array_typed_get_type             ( tp_t tp, vc_t o, sz_t index );
vc_t bcore_array_typed_get_c_data           ( tp_t tp, vc_t o );
vd_t bcore_array_typed_get_d_data           ( tp_t tp, vd_t o );
sz_t bcore_array_typed_get_unit_size        ( tp_t tp, vc_t o );
vc_t bcore_array_typed_max                  ( tp_t tp, vc_t o, sz_t start, sz_t end, s2_t order );
sz_t bcore_array_typed_max_index            ( tp_t tp, vc_t o, sz_t start, sz_t end, s2_t order );
void bcore_array_typed_sort                 ( tp_t tp, vd_t o, sz_t start, sz_t end, s2_t order );

sz_t bcore_array_aware_get_size             ( vc_t o );
sz_t bcore_array_aware_get_space            ( vc_t o );
rf_s bcore_array_aware_get                  ( vc_t o, sz_t index );
rf_s bcore_array_aware_set                  ( vd_t o, sz_t index, rf_s src );
rf_s bcore_array_aware_set_c                ( vd_t o, sz_t index, rf_s src );
rf_s bcore_array_aware_set_s3               ( vd_t o, sz_t index, s3_t val );
rf_s bcore_array_aware_set_u3               ( vd_t o, sz_t index, u3_t val );
rf_s bcore_array_aware_set_f3               ( vd_t o, sz_t index, f3_t val );
rf_s bcore_array_aware_set_sz               ( vd_t o, sz_t index, sz_t val );
rf_s bcore_array_aware_set_sc               ( vd_t o, sz_t index, sc_t val );
rf_s bcore_array_aware_set_bl               ( vd_t o, sz_t index, bl_t val );
void bcore_array_aware_set_size             ( vd_t o, sz_t size  );
void bcore_array_aware_set_space            ( vd_t o, sz_t space );
rf_s bcore_array_aware_get_first            ( vc_t o );
rf_s bcore_array_aware_get_last             ( vc_t o );
rf_s bcore_array_aware_push                 ( vd_t o, rf_s src );
rf_s bcore_array_aware_push_s3              ( vd_t o, s3_t val );
rf_s bcore_array_aware_push_u3              ( vd_t o, u3_t val );
rf_s bcore_array_aware_push_f3              ( vd_t o, f3_t val );
rf_s bcore_array_aware_push_sz              ( vd_t o, sz_t val );
rf_s bcore_array_aware_push_sc              ( vd_t o, sc_t val );
rf_s bcore_array_aware_push_bl              ( vd_t o, bl_t val );
void bcore_array_aware_pop                  ( vd_t o );
void bcore_array_aware_set_gtype            ( vd_t o, tp_t type  );
bl_t bcore_array_aware_is_static            ( vc_t o );
bl_t bcore_array_aware_is_mono_typed        ( vc_t o );
bl_t bcore_array_aware_is_mutable_mono_typed( vc_t o );
bl_t bcore_array_aware_is_multi_typed       ( vc_t o );
bl_t bcore_array_aware_is_of_aware          ( vc_t o );
bl_t bcore_array_aware_is_of_links          ( vc_t o );
tp_t bcore_array_aware_get_static_type      ( vc_t o );
tp_t bcore_array_aware_get_mono_type        ( vc_t o );
tp_t bcore_array_aware_get_type             ( vc_t o, sz_t index );
vc_t bcore_array_aware_get_c_data           ( vc_t o );
vd_t bcore_array_aware_get_d_data           ( vd_t o );
sz_t bcore_array_aware_get_unit_size        ( vc_t o );
vc_t bcore_array_aware_max                  ( vc_t o, sz_t start, sz_t end, s2_t order );
sz_t bcore_array_aware_max_index            ( vc_t o, sz_t start, sz_t end, s2_t order );
void bcore_array_aware_sort                 ( vd_t o, sz_t start, sz_t end, s2_t order );

/**********************************************************************************************************************/
// testing, debugging

bcore_string_s* bcore_spect_array_selftest( void );

#endif
