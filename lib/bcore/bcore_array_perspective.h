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


#ifndef BCORE_ARRAY_PERSPECTIVE_H
#define BCORE_ARRAY_PERSPECTIVE_H

#include "bcore_instance_perspective.h"
#include "bcore_flect.h"
#include "bcore_features.h"

typedef struct bcore_array_s bcore_array_s;

typedef struct bcore_array_s
{
    bcore_perspective_s _;

    u2_t caps_type;
    sz_t caps_offset;
    const bcore_instance_s* item_p;  // item-perspective; NULL for typed or aware arrays

    sz_t ( *get_size    )( const bcore_array_s* p, vc_t o );             // returns size
    sz_t ( *get_space   )( const bcore_array_s* p, vc_t o );             // returns space
    void ( *set_size    )( const bcore_array_s* p, vd_t o, sz_t size  ); // changes array size (keeping previous data); for linked arrays new items are NULL
    void ( *set_space   )( const bcore_array_s* p, vd_t o, sz_t space ); // changes space
    vc_t ( *get_c_item  )( const bcore_array_s* p, vc_t o, sz_t index ); // returns indexed item; NULL if index is out of range or the linked item is NULL
    vd_t ( *get_d_item  )( const bcore_array_s* p, vd_t o, sz_t index ); // returns indexed item; NULL if index is out of range or the linked item is NULL
    vd_t ( *set_item_c  )( const bcore_array_s* p, vd_t o, sz_t index, vc_t src ); // sets item at indexed position by copying src; if index is out of size, size is increased
    vd_t ( *set_item_d  )( const bcore_array_s* p, vd_t o, sz_t index, vd_t src ); // sets item at indexed position by taking ownership of src (emplaced arrays copy and discard src)
    vc_t ( *get_c_first )( const bcore_array_s* p, vc_t o );             // returns first item; NULL if array is empty
    vd_t ( *get_d_first )( const bcore_array_s* p, vd_t o );             // returns first item; NULL if array is empty
    vc_t ( *get_c_last  )( const bcore_array_s* p, vc_t o );             // returns last item; NULL if array is empty
    vd_t ( *get_d_last  )( const bcore_array_s* p, vd_t o );             // returns last item; NULL if array is empty
    vd_t ( *push_c      )( const bcore_array_s* p, vd_t o, vc_t src );   // push by copying item; returns new entry
    vd_t ( *push_d      )( const bcore_array_s* p, vd_t o, vd_t src );   // push by taking ownership of item; returns new entry. (emplaced arrays copy the item and delete it)
    void ( *pop         )( const bcore_array_s* p, vd_t o );             // removes last element from array

    tp_t ( *get_type    )( const bcore_array_s* p, vc_t o            ); // retrieves item-type; returns 0 for aware-arrays
    void ( *set_type    )( const bcore_array_s* p, vd_t o, tp_t type ); // changes item-type on empty arrays;

    // direct data access (data pointer dereferencing and stepping depends on array structure)
    vc_t ( *get_c_data    )( const bcore_array_s* p, vc_t o ); // returns arr_caps->data (note that this is either vc_t or vc_t* depending on linkage-indirection
    vd_t ( *get_d_data    )( const bcore_array_s* p, vd_t o ); // returns arr_caps->data (note that this is either vd_t or vd_t* depending on linkage-indirection
    bool ( *is_linked     )( const bcore_array_s* p         ); // true in case of array of links; false otherwise
    bool ( *is_typed      )( const bcore_array_s* p         ); // true in case of typed-array; false otherwise
    sz_t ( *get_unit_size )( const bcore_array_s* p, vc_t o ); // spacing between data elements (item_p->size or sizeof(vd_t))

    /** Array operations
     *  The specified limits cover the index range [start, end-1]
     *  When end is larger than the array size, it is truncated to the array size.
     *  Using '-1' as argument for 'end' is allowed, which sets 'end' equal to 'size'
     *  When using non-negative values and start >= end the effective range is zero.
     */

    // maximum/minimum within range (order = -1: minimum)
    vc_t ( *max         )( const bcore_array_s* p, vc_t o, sz_t start, sz_t end, bcore_fp_cmp cmp, s2_t order );
    sz_t ( *max_index   )( const bcore_array_s* p, vc_t o, sz_t start, sz_t end, bcore_fp_cmp cmp, s2_t order );

    // creates a cropped sub-array from o
    vd_t ( *crop        )( const bcore_array_s* p, vc_t o, sz_t start, sz_t end );
    vd_t ( *crop_d      )( const bcore_array_s* p, vd_t o, sz_t start, sz_t end ); // discards o after cropping

    // (merge-)sort within index range [start, end-1]
    void ( *sort        )( const bcore_array_s* p, vd_t o, sz_t start, sz_t end, bcore_fp_cmp cmp, s2_t order );


} bcore_array_s;

const bcore_array_s* bcore_array_s_get_typed( u2_t type );

static inline const bcore_array_s* bcore_array_s_get_aware( vc_t obj )
{
    return bcore_array_s_get_typed( *( const aware_t* )obj );
}

/**********************************************************************************************************************/
// testing, debugging

bcore_string_s* bcore_array_perspective_selftest();

#endif
