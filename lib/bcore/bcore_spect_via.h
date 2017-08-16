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
    sz_t offs;   // offset to member element
    const bcore_via_s* via; // via of this item; NULL for leaf types
} bcore_vitem_s;

typedef struct bcore_via_s
{
    aware_t p_type; // type of perspective
    tp_t    o_type; // type of object
    sz_t                 size;       // number of elements
    bcore_vitem_s*       vitem_arr;  // array of vitem
    const bcore_inst_s** inst_arr;   // pointers to instance perspectives
} bcore_via_s;

bcore_flect_self_s* bcore_via_s_create_self( void );

const bcore_via_s* bcore_via_s_get_typed( u2_t type );
const bcore_via_s* bcore_via_s_get_aware( vc_t obj  );

/// Access by index. Error when index is out of range.
tp_t                 bcore_via_spect_iget_name     ( const bcore_via_s* p,         sz_t index ); // Returns name for given index
tp_t                 bcore_via_spect_iget_type     ( const bcore_via_s* p, vc_t o, sz_t index ); // Returns type of element
rf_s                 bcore_via_spect_irget         ( const bcore_via_s* p, vc_t o, sz_t index           ); // Returns indexed reference
rf_s                 bcore_via_spect_irset         ( const bcore_via_s* p, vd_t o, sz_t index, rf_s src ); // Sets indexed const item.
vc_t                 bcore_via_spect_iget_c        ( const bcore_via_s* p, vc_t o, sz_t index ); // Returns indexed const item.
vd_t                 bcore_via_spect_iget_d        ( const bcore_via_s* p, vd_t o, sz_t index ); // Returns indexed item.
rf_s                 bcore_via_spect_iset_s3       ( const bcore_via_s* p, vd_t o, sz_t index, s3_t val ); // Sets (internal) item by converting s3_t into target type
rf_s                 bcore_via_spect_iset_u3       ( const bcore_via_s* p, vd_t o, sz_t index, u3_t val ); // Sets (internal) item by converting u3_t into target type
rf_s                 bcore_via_spect_iset_f3       ( const bcore_via_s* p, vd_t o, sz_t index, f3_t val ); // Sets (internal) item by converting f3_t into target type
rf_s                 bcore_via_spect_iset_sz       ( const bcore_via_s* p, vd_t o, sz_t index, sz_t val ); // Sets (internal) item by converting sz_t into target type
rf_s                 bcore_via_spect_iset_sc       ( const bcore_via_s* p, vd_t o, sz_t index, sc_t val ); // Sets (internal) item by converting sc_t into target type
rf_s                 bcore_via_spect_iset_bl       ( const bcore_via_s* p, vd_t o, sz_t index, bl_t val ); // Sets (internal) item by converting bl_t into target type
const bcore_vitem_s* bcore_via_spect_iget_vitem    ( const bcore_via_s* p,         sz_t index ); // Returns bcore_vitem_s structure;
const bcore_via_s*   bcore_via_spect_iget_via      ( const bcore_via_s* p,         sz_t index ); // Returns via perspective for item
const bcore_array_s* bcore_via_spect_iget_array    ( const bcore_via_s* p,         sz_t index ); // Returns array perspective for item; return NULL when item is no array
vc_t                 bcore_via_spect_iget_spect    ( const bcore_via_s* p, vc_t o, sz_t index, tp_t stp );

/// Access by name. Error when object has no element of given name.
sz_t                 bcore_via_spect_nget_index    ( const bcore_via_s* p,         tp_t name ); // Returns index for given name
tp_t                 bcore_via_spect_nget_type     ( const bcore_via_s* p, vc_t o, tp_t name );
rf_s                 bcore_via_spect_nrget         ( const bcore_via_s* p, vc_t o, tp_t name           );
rf_s                 bcore_via_spect_nrset         ( const bcore_via_s* p, vd_t o, tp_t name, rf_s src );
vc_t                 bcore_via_spect_nget_c        ( const bcore_via_s* p, vc_t o, tp_t name );
vd_t                 bcore_via_spect_nget_d        ( const bcore_via_s* p, vd_t o, tp_t name );
rf_s                 bcore_via_spect_nset_s3       ( const bcore_via_s* p, vd_t o, tp_t name, s3_t val );
rf_s                 bcore_via_spect_nset_u3       ( const bcore_via_s* p, vd_t o, tp_t name, u3_t val );
rf_s                 bcore_via_spect_nset_f3       ( const bcore_via_s* p, vd_t o, tp_t name, f3_t val );
rf_s                 bcore_via_spect_nset_sz       ( const bcore_via_s* p, vd_t o, tp_t name, sz_t val );
rf_s                 bcore_via_spect_nset_sc       ( const bcore_via_s* p, vd_t o, tp_t name, sc_t val );
rf_s                 bcore_via_spect_nset_bl       ( const bcore_via_s* p, vd_t o, tp_t name, bl_t val );
const bcore_vitem_s* bcore_via_spect_nget_vitem    ( const bcore_via_s* p,         tp_t name );
const bcore_via_s*   bcore_via_spect_nget_via      ( const bcore_via_s* p,         tp_t name );
const bcore_array_s* bcore_via_spect_nget_array    ( const bcore_via_s* p,         tp_t name );
vc_t                 bcore_via_spect_nget_spect    ( const bcore_via_s* p, vc_t o, tp_t name, tp_t stp );

/// Access by name for typed objects. Error when object has no element of given name.
tp_t                 bcore_via_typed_nget_type     ( tp_t tp, vc_t o, tp_t name );
rf_s                 bcore_via_typed_nrget         ( tp_t tp, vc_t o, tp_t name           );
rf_s                 bcore_via_typed_nrset         ( tp_t tp, vd_t o, tp_t name, rf_s src );
vc_t                 bcore_via_typed_nget_c        ( tp_t tp, vc_t o, tp_t name );
vd_t                 bcore_via_typed_nget_d        ( tp_t tp, vd_t o, tp_t name );
rf_s                 bcore_via_typed_nset_s3       ( tp_t tp, vd_t o, tp_t name, s3_t val );
rf_s                 bcore_via_typed_nset_u3       ( tp_t tp, vd_t o, tp_t name, u3_t val );
rf_s                 bcore_via_typed_nset_f3       ( tp_t tp, vd_t o, tp_t name, f3_t val );
rf_s                 bcore_via_typed_nset_sz       ( tp_t tp, vd_t o, tp_t name, sz_t val );
rf_s                 bcore_via_typed_nset_sc       ( tp_t tp, vd_t o, tp_t name, sc_t val );
rf_s                 bcore_via_typed_nset_bl       ( tp_t tp, vd_t o, tp_t name, bl_t val );
const bcore_vitem_s* bcore_via_typed_nget_vitem    ( tp_t tp,         tp_t name );
const bcore_via_s*   bcore_via_typed_nget_via      ( tp_t tp,         tp_t name );
const bcore_array_s* bcore_via_typed_nget_array    ( tp_t tp,         tp_t name );
vc_t                 bcore_via_typed_nget_spect    ( tp_t tp, vc_t o, tp_t name, tp_t stp );

/// Access by name for aware objects. Error when object has no element of given name.
tp_t                 bcore_via_aware_nget_type     ( vc_t o, tp_t name );
rf_s                 bcore_via_aware_nrget         ( vc_t o, tp_t name           );
rf_s                 bcore_via_aware_nrset         ( vd_t o, tp_t name, rf_s src );
vc_t                 bcore_via_aware_nget_c        ( vc_t o, tp_t name );
vd_t                 bcore_via_aware_nget_d        ( vd_t o, tp_t name );
rf_s                 bcore_via_aware_nset_s3       ( vd_t o, tp_t name, s3_t val );
rf_s                 bcore_via_aware_nset_u3       ( vd_t o, tp_t name, u3_t val );
rf_s                 bcore_via_aware_nset_f3       ( vd_t o, tp_t name, f3_t val );
rf_s                 bcore_via_aware_nset_sz       ( vd_t o, tp_t name, sz_t val );
rf_s                 bcore_via_aware_nset_sc       ( vd_t o, tp_t name, sc_t val );
rf_s                 bcore_via_aware_nset_bl       ( vd_t o, tp_t name, bl_t val );
const bcore_vitem_s* bcore_via_aware_nget_vitem    ( vc_t o, tp_t name );
const bcore_via_s*   bcore_via_aware_nget_via      ( vc_t o, tp_t name );
const bcore_array_s* bcore_via_aware_nget_array    ( vc_t o, tp_t name );
vc_t                 bcore_via_aware_nget_spect    ( vc_t o, tp_t name, tp_t stp );

bl_t bcore_via_spect_is_pure_array( const bcore_via_s* p             ); // checks if object is an array without additional elements
bl_t bcore_via_spect_iis_array(     const bcore_via_s* p, sz_t index ); // checks if element is an array
bl_t bcore_via_spect_iis_static(    const bcore_via_s* p, sz_t index ); // checks if element is static (type need not be recorded)
bl_t bcore_via_spect_iis_link(      const bcore_via_s* p, sz_t index ); // checks if element is a link (means that it can be NULL); an array is a distinct static object -> not a link)

/**********************************************************************************************************************/
// testing, debugging

bcore_string_s* bcore_spect_via_selftest( void );

#endif
