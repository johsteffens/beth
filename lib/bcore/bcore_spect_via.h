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
#include <stdarg.h>

// Fetaures
// optional bcore_fp_get and bcore_fp_set for explicit element access
// If desired, the object must define the function under the name 'get_<name>', 'set_<name>'

typedef struct bcore_array_s bcore_array_s;
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
        tp_t flags; // collection of attribute flags
        struct
        {
            unsigned f_shell : 1; // shell flag
        };
    };
    sz_t offs;                 // offset to member element
    const bcore_via_s* via_p;  // via perspective of this item
    bcore_fp_get fp_get;       // optional explicit getter
    bcore_fp_set fp_set;       // optional explicit setter
} bcore_vitem_s;

typedef struct bcore_via_s
{
    aware_t p_type;             // type of perspective
    tp_t    o_type;             // type of object
    const bcore_inst_s* inst_p; // inst perspective of this item
    sz_t  size;                 // number of elements
    bl_t  is_leaf;
    bcore_vitem_s* vitem_arr;   // array of vitem
} bcore_via_s;

bcore_flect_self_s* bcore_via_s_create_self( void );

const bcore_via_s* bcore_via_s_get_typed( u2_t type );
const bcore_via_s* bcore_via_s_get_aware( vc_t obj  );

/// Access by index. Error when index is out of range.
sz_t                 bcore_via_spect_get_size      ( const bcore_via_s* p                     ); // Number of items
tp_t                 bcore_via_spect_iget_name     ( const bcore_via_s* p,         sz_t index ); // Returns name for given index
sr_s                 bcore_via_spect_iget          ( const bcore_via_s* p, vc_t o, sz_t index           ); // Returns indexed reference
void                 bcore_via_spect_iset          ( const bcore_via_s* p, vd_t o, sz_t index, sr_s src ); // Sets indexed const item.
tp_t                 bcore_via_spect_iget_type     ( const bcore_via_s* p, vc_t o, sz_t index           ); // Returns type of object

void                 bcore_via_spect_iset_s3       ( const bcore_via_s* p, vd_t o, sz_t index, s3_t val ); // Sets (internal) item by converting s3_t into target type
void                 bcore_via_spect_iset_u3       ( const bcore_via_s* p, vd_t o, sz_t index, u3_t val ); // Sets (internal) item by converting u3_t into target type
void                 bcore_via_spect_iset_f3       ( const bcore_via_s* p, vd_t o, sz_t index, f3_t val ); // Sets (internal) item by converting f3_t into target type
void                 bcore_via_spect_iset_sz       ( const bcore_via_s* p, vd_t o, sz_t index, sz_t val ); // Sets (internal) item by converting sz_t into target type
void                 bcore_via_spect_iset_sc       ( const bcore_via_s* p, vd_t o, sz_t index, sc_t val ); // Sets (internal) item by converting sc_t into target type
void                 bcore_via_spect_iset_bl       ( const bcore_via_s* p, vd_t o, sz_t index, bl_t val ); // Sets (internal) item by converting bl_t into target type
void                 bcore_via_spect_iset_tp       ( const bcore_via_s* p, vd_t o, sz_t index, tp_t val ); // Sets (internal) item by converting tp_t into target type
const bcore_vitem_s* bcore_via_spect_iget_vitem    ( const bcore_via_s* p,         sz_t index ); // Returns bcore_vitem_s structure;
const bcore_via_s*   bcore_via_spect_iget_via      ( const bcore_via_s* p,         sz_t index ); // Returns via perspective for item
const bcore_array_s* bcore_via_spect_iget_array    ( const bcore_via_s* p,         sz_t index ); // Returns array perspective for item; return NULL when item is no array
vc_t                 bcore_via_spect_iget_spect    ( const bcore_via_s* p, vc_t o, sz_t index, tp_t stp );

/// Access by name. Error when object has no element of given name.
sz_t                 bcore_via_spect_nget_index    ( const bcore_via_s* p,         tp_t name ); // Returns index for given name
sr_s                 bcore_via_spect_nget          ( const bcore_via_s* p, vc_t o, tp_t name           );
void                 bcore_via_spect_nset          ( const bcore_via_s* p, vd_t o, tp_t name, sr_s src );
void                 bcore_via_spect_nset_s3       ( const bcore_via_s* p, vd_t o, tp_t name, s3_t val );
void                 bcore_via_spect_nset_u3       ( const bcore_via_s* p, vd_t o, tp_t name, u3_t val );
void                 bcore_via_spect_nset_f3       ( const bcore_via_s* p, vd_t o, tp_t name, f3_t val );
void                 bcore_via_spect_nset_sz       ( const bcore_via_s* p, vd_t o, tp_t name, sz_t val );
void                 bcore_via_spect_nset_sc       ( const bcore_via_s* p, vd_t o, tp_t name, sc_t val );
void                 bcore_via_spect_nset_bl       ( const bcore_via_s* p, vd_t o, tp_t name, bl_t val );
void                 bcore_via_spect_nset_tp       ( const bcore_via_s* p, vd_t o, tp_t name, tp_t val );
const bcore_vitem_s* bcore_via_spect_nget_vitem    ( const bcore_via_s* p,         tp_t name );
const bcore_via_s*   bcore_via_spect_nget_via      ( const bcore_via_s* p,         tp_t name );
const bcore_array_s* bcore_via_spect_nget_array    ( const bcore_via_s* p,         tp_t name );
vc_t                 bcore_via_spect_nget_spect    ( const bcore_via_s* p, vc_t o, tp_t name, tp_t stp );

bl_t bcore_via_spect_is_leaf(       const bcore_via_s* p             ); // checks if object is a leaf (no elements)
bl_t bcore_via_spect_is_pure_array( const bcore_via_s* p             ); // checks if object is an array without additional elements (pure arrays are not leafs)
bl_t bcore_via_spect_iis_array(     const bcore_via_s* p, sz_t index ); // checks if element is an array
bl_t bcore_via_spect_iis_static(    const bcore_via_s* p, sz_t index ); // checks if element is static (type need not be recorded)
bl_t bcore_via_spect_iis_link(      const bcore_via_s* p, sz_t index ); // checks if element is a link (means that it can be NULL); an array is a distinct static object -> not a link)


/// Access by name for typed objects. Error when object has no element of given name.
sz_t                 bcore_via_typed_get_size      ( tp_t tp                              );
sr_s                 bcore_via_typed_nget          ( tp_t tp, vc_t o, tp_t name           );
void                 bcore_via_typed_nset          ( tp_t tp, vd_t o, tp_t name, sr_s src );
void                 bcore_via_typed_nset_s3       ( tp_t tp, vd_t o, tp_t name, s3_t val );
void                 bcore_via_typed_nset_u3       ( tp_t tp, vd_t o, tp_t name, u3_t val );
void                 bcore_via_typed_nset_f3       ( tp_t tp, vd_t o, tp_t name, f3_t val );
void                 bcore_via_typed_nset_sz       ( tp_t tp, vd_t o, tp_t name, sz_t val );
void                 bcore_via_typed_nset_sc       ( tp_t tp, vd_t o, tp_t name, sc_t val );
void                 bcore_via_typed_nset_bl       ( tp_t tp, vd_t o, tp_t name, bl_t val );
void                 bcore_via_typed_nset_tp       ( tp_t tp, vd_t o, tp_t name, tp_t val );
const bcore_vitem_s* bcore_via_typed_nget_vitem    ( tp_t tp,         tp_t name );
const bcore_via_s*   bcore_via_typed_nget_via      ( tp_t tp,         tp_t name );
const bcore_array_s* bcore_via_typed_nget_array    ( tp_t tp,         tp_t name );
vc_t                 bcore_via_typed_nget_spect    ( tp_t tp, vc_t o, tp_t name, tp_t stp );

/// Access by name for aware objects. Error when object has no element of given name.
sz_t                 bcore_via_aware_get_size      ( vc_t o                      );
sr_s                 bcore_via_aware_nget          ( vc_t o, tp_t name           );
void                 bcore_via_aware_nset          ( vd_t o, tp_t name, sr_s src );
void                 bcore_via_aware_nset_s3       ( vd_t o, tp_t name, s3_t val );
void                 bcore_via_aware_nset_u3       ( vd_t o, tp_t name, u3_t val );
void                 bcore_via_aware_nset_f3       ( vd_t o, tp_t name, f3_t val );
void                 bcore_via_aware_nset_sz       ( vd_t o, tp_t name, sz_t val );
void                 bcore_via_aware_nset_sc       ( vd_t o, tp_t name, sc_t val );
void                 bcore_via_aware_nset_bl       ( vd_t o, tp_t name, bl_t val );
void                 bcore_via_aware_nset_tp       ( vd_t o, tp_t name, tp_t val );
const bcore_vitem_s* bcore_via_aware_nget_vitem    ( vc_t o, tp_t name );
const bcore_via_s*   bcore_via_aware_nget_via      ( vc_t o, tp_t name );
const bcore_array_s* bcore_via_aware_nget_array    ( vc_t o, tp_t name );
vc_t                 bcore_via_aware_nget_spect    ( vc_t o, tp_t name, tp_t stp );

sz_t                 bcore_via_get_size      ( sr_s o                       );
tp_t                 bcore_via_iget_name     ( sr_s o, sz_t index           );
sr_s                 bcore_via_iget          ( sr_s o, sz_t index           );
void                 bcore_via_iset          ( sr_s o, sz_t index, sr_s src );
void                 bcore_via_iset_s3       ( sr_s o, sz_t index, s3_t val );
void                 bcore_via_iset_u3       ( sr_s o, sz_t index, u3_t val );
void                 bcore_via_iset_f3       ( sr_s o, sz_t index, f3_t val );
void                 bcore_via_iset_sz       ( sr_s o, sz_t index, sz_t val );
void                 bcore_via_iset_sc       ( sr_s o, sz_t index, sc_t val );
void                 bcore_via_iset_bl       ( sr_s o, sz_t index, bl_t val );
void                 bcore_via_iset_tp       ( sr_s o, sz_t index, tp_t val );
const bcore_vitem_s* bcore_via_iget_vitem    ( sr_s o, sz_t index );
const bcore_via_s*   bcore_via_iget_via      ( sr_s o, sz_t index );
const bcore_array_s* bcore_via_iget_array    ( sr_s o, sz_t index );
vc_t                 bcore_via_iget_spect    ( sr_s o, sz_t index, tp_t stp );

sz_t                 bcore_via_nget_index    ( sr_s o, tp_t name );
sr_s                 bcore_via_nget          ( sr_s o, tp_t name           );
void                 bcore_via_nset          ( sr_s o, tp_t name, sr_s src );
void                 bcore_via_nset_s3       ( sr_s o, tp_t name, s3_t val );
void                 bcore_via_nset_u3       ( sr_s o, tp_t name, u3_t val );
void                 bcore_via_nset_f3       ( sr_s o, tp_t name, f3_t val );
void                 bcore_via_nset_sz       ( sr_s o, tp_t name, sz_t val );
void                 bcore_via_nset_sc       ( sr_s o, tp_t name, sc_t val );
void                 bcore_via_nset_bl       ( sr_s o, tp_t name, bl_t val );
void                 bcore_via_nset_tp       ( sr_s o, tp_t name, tp_t val );
const bcore_vitem_s* bcore_via_nget_vitem    ( sr_s o, tp_t name );
const bcore_via_s*   bcore_via_nget_via      ( sr_s o, tp_t name );
const bcore_array_s* bcore_via_nget_array    ( sr_s o, tp_t name );
vc_t                 bcore_via_nget_spect    ( sr_s o, tp_t name, tp_t stp );

bl_t bcore_via_is_leaf(       sr_s o             );
bl_t bcore_via_is_pure_array( sr_s o             );
bl_t bcore_via_iis_array(     sr_s o, sz_t index );
bl_t bcore_via_iis_static(    sr_s o, sz_t index );
bl_t bcore_via_iis_link(      sr_s o, sz_t index );

/**********************************************************************************************************************/
// testing, debugging

bcore_string_s* bcore_spect_via_selftest( void );

#endif
