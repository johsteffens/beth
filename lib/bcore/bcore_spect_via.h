/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

/** The via-perspective provides access to object's elements.
 *  An object can be either be seen as container with 'size' ordered elements which
 *  are accessed by index or as associative storage mapping the element's name to
 *  the location of the respective instance.
 *  This perspective handles access for both aspects.
 *
 *  Functions are referenced as pointers to function pointers.
 */

#ifndef BCORE_SPECT_VIA_H
#define BCORE_SPECT_VIA_H

#include "bcore_spect_inst.h"
#include "bcore_features.h"

typedef struct bcore_via_s bcore_via_s;

typedef struct bcore_vitem_s
{
    /** Type of element.
     *  For arrays this value represents a (generic) array type.
     *  Static arrays types are constructed when bcore_via_s is constructed.
     */
    tp_t type;
    tp_t name;
    u2_t caps;
    union
    {
        sz_t offs;   // offset to member element
        vc_t d_ptr;  // external data pointer
        fp_t f_ptr;  // external function pointer
    };
    const bcore_via_s* via; // via of this item; NULL for leaf types
} bcore_vitem_s;

typedef struct bcore_via_s
{
    aware_t p_type; // type of perspective
    tp_t    o_type; // type of object

    sz_t                     size;       // number of elements
    bcore_vitem_s*           vitem_arr;  // array of vitem
    const bcore_inst_s** inst_arr;   // pointers to instance perspectives

    /// Access by index. Error when index is out of range.
    tp_t                 ( *iget_name     )( const bcore_via_s* p,         sz_t index ); // Returns name for given index
    tp_t                 ( *iget_type     )( const bcore_via_s* p, vc_t o, sz_t index ); // Returns type of element
    vc_t                 ( *iget_c        )( const bcore_via_s* p, vc_t o, sz_t index ); // Returns indexed const item.
    vd_t                 ( *iget_d        )( const bcore_via_s* p, vd_t o, sz_t index ); // Returns indexed item.
    vd_t                 ( *iset_c        )( const bcore_via_s* p, vd_t o, sz_t index, vc_t src ); // Sets (internal) item at indexed position by copying src. External data by assigning the pointer
    vd_t                 ( *iset_d        )( const bcore_via_s* p, vd_t o, sz_t index, vd_t src ); // Sets item at indexed position by taking ownership of src.
    const bcore_vitem_s* ( *iget_vitem    )( const bcore_via_s* p,         sz_t index ); // Returns bcore_vitem_s structure;
    const bcore_via_s*   ( *iget_via      )( const bcore_via_s* p,         sz_t index ); // Returns bcore_vitem_s structure;
    vd_t                 ( *icreate       )( const bcore_via_s* p, vd_t o, sz_t index ); // creates item. No effect if already created. Error when type info is not available.
    vd_t                 ( *ityped_create )( const bcore_via_s* p, vd_t o, sz_t index, tp_t type ); // creates item at given type. Error for static types.
    void                 ( *idiscard      )( const bcore_via_s* p, vd_t o, sz_t index ); // Discards discardable element (do effect otherwise)
    vd_t                 ( *idetach       )( const bcore_via_s* p, vd_t o, sz_t index ); // moves ownership of item to caller. Error if not detachable

    /// Access by name. Error when object has no element of given name.
    sz_t                 ( *nget_index    )( const bcore_via_s* p,         tp_t name ); // Returns index for given name
    tp_t                 ( *nget_type     )( const bcore_via_s* p, vc_t o, tp_t name );
    vc_t                 ( *nget_c        )( const bcore_via_s* p, vc_t o, tp_t name );
    vd_t                 ( *nget_d        )( const bcore_via_s* p, vd_t o, tp_t name );
    vd_t                 ( *nset_c        )( const bcore_via_s* p, vd_t o, tp_t name, vc_t src );
    vd_t                 ( *nset_d        )( const bcore_via_s* p, vd_t o, tp_t name, vd_t src );
    const bcore_vitem_s* ( *nget_vitem    )( const bcore_via_s* p,         tp_t name );
    const bcore_via_s*   ( *nget_via      )( const bcore_via_s* p,         tp_t name );
    vd_t                 ( *ncreate       )( const bcore_via_s* p, vd_t o, tp_t name );
    vd_t                 ( *ntyped_create )( const bcore_via_s* p, vd_t o, tp_t name, tp_t type );
    void                 ( *ndiscard      )( const bcore_via_s* p, vd_t o, tp_t name );
    vd_t                 ( *ndetach       )( const bcore_via_s* p, vd_t o, tp_t name );

} bcore_via_s;

bcore_flect_self_s* bcore_via_s_create_self();

const bcore_via_s* bcore_via_s_get_typed( u2_t type );

static inline const bcore_via_s* bcore_via_s_get_aware( vc_t obj )
{
    return bcore_via_s_get_typed( *( const aware_t* )obj );
}

/**********************************************************************************************************************/
// testing, debugging

bcore_string_s* bcore_spect_via_selftest();

#endif
