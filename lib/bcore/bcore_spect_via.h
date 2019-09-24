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
#include "bcore_feature.h"
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
    bcore_flect_flags_s flags; // reflection flags
    uz_t offs;                 // offset to member element
    const bcore_via_s* via_p;  // via perspective of this item
    bcore_fp_get fp_get;       // optional explicit getter
    bcore_fp_set fp_set;       // optional explicit setter
} bcore_vitem_s;

BCORE_FORWARD_OBJECT( bcore_via_call_s );

BCORE_DECLARE_SPECT( bcore_via )
{
    bcore_spect_header_s  header;
    const bcore_inst_s*     inst_p;     // inst perspective of this item
    const bcore_via_call_s* via_call_p; // via callback perspective of this item
    uz_t  size;                 // number of elements
    bl_t  is_leaf;
    bcore_vitem_s* vitem_arr;   // array of vitem
};

static inline
uz_t bcore_via_default_get_size( const bcore_via_s* p, const bcore_via* o )
{
    return p->size;
}

static inline
tp_t bcore_via_default_iget_name( const bcore_via_s* p, const bcore_via* o, uz_t index )
{
    if( index >= p->size ) ERR( "index (%zu) out of range (%zu)", index, p->size );
    return p->vitem_arr[ index ].name;
}

static inline
const bcore_vitem_s* bcore_via_default_iget_vitem( const bcore_via_s* p, const bcore_via* o, uz_t index )
{
    if( index >= p->size ) ERR( "index (%zu) out of range (%zu)", index, p->size );
    return &p->vitem_arr[ index ];
}

static inline
const bcore_via_s* bcore_via_default_iget_via( const bcore_via_s* p, const bcore_via* o, uz_t index )
{
    if( index >= p->size ) ERR( "index (%zu) out of range (%zu)", index, p->size );
    return p->vitem_arr[ index ].via_p;
}

BCORE_FUNC_SPECT_CONST1_RET1_ARG0_MAP0( bcore_via, get_size,  uz_t )                         // Number of items
BCORE_FUNC_SPECT_CONST1_RET1_ARG1_MAP0( bcore_via, iget_name, tp_t, uz_t, index )            // Returns name for given index
BCORE_FUNC_SPECT_CONST1_RET1_ARG1_MAP0( bcore_via, iget,      sr_s, uz_t, index )            // Returns indexed reference
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_via, iset,            uz_t, index, sr_s, src ) // Sets indexed const item.
BCORE_FUNC_SPECT_CONST1_RET1_ARG1_MAP0( bcore_via, iget_type, tp_t, uz_t, index )            // Returns type of object

BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_via, iset_s3,         uz_t, index, s3_t, val ) // Sets (internal) item by converting s3_t into target type
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_via, iset_u3,         uz_t, index, u3_t, val ) // Sets (internal) item by converting u3_t into target type
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_via, iset_f3,         uz_t, index, f3_t, val ) // Sets (internal) item by converting f3_t into target type
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_via, iset_sz,         uz_t, index, sz_t, val ) // Sets (internal) item by converting sz_t into target type
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_via, iset_uz,         uz_t, index, uz_t, val ) // Sets (internal) item by converting uz_t into target type
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_via, iset_sc,         uz_t, index, sc_t, val ) // Sets (internal) item by converting sc_t into target type
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_via, iset_bl,         uz_t, index, bl_t, val ) // Sets (internal) item by converting bl_t into target type
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_via, iset_tp,         uz_t, index, tp_t, val ) // Sets (internal) item by converting tp_t into target type

BCORE_FUNC_SPECT_CONST1_RET1_ARG1_MAP0( bcore_via, iget_vitem, const bcore_vitem_s*, uz_t, index ) // Returns bcore_vitem_s structure;
BCORE_FUNC_SPECT_CONST1_RET1_ARG1_MAP0( bcore_via, iget_via,   const bcore_via_s*,   uz_t, index ) // Returns via perspective for item
BCORE_FUNC_SPECT_CONST1_RET1_ARG1_MAP0( bcore_via, iget_array, const bcore_array_s*, uz_t, index ) // Returns array perspective for item; return NULL when item is no array
BCORE_FUNC_SPECT_CONST1_RET1_ARG2_MAP0( bcore_via, iget_spect,                 vc_t, uz_t, index, tp_t, spect_type )

BCORE_FUNC_SPECT_CONST1_RET1_ARG1_MAP0( bcore_via, nexists,    bl_t, tp_t, name ) // Checks if given name exists (object can be NULL)
BCORE_FUNC_SPECT_CONST1_RET1_ARG1_MAP0( bcore_via, nget_index, uz_t, tp_t, name ) // Returns index for given name (object can be NULL)
BCORE_FUNC_SPECT_CONST1_RET1_ARG1_MAP0( bcore_via, nget      , sr_s, tp_t, name )
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_via, nset      ,       tp_t, name, sr_s, src )
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_via, nset_s3   ,       tp_t, name, s3_t, val )
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_via, nset_u3   ,       tp_t, name, u3_t, val )
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_via, nset_f3   ,       tp_t, name, f3_t, val )
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_via, nset_sz   ,       tp_t, name, sz_t, val )
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_via, nset_uz   ,       tp_t, name, uz_t, val )
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_via, nset_sc   ,       tp_t, name, sc_t, val )
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_via, nset_bl   ,       tp_t, name, bl_t, val )
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_via, nset_tp   ,       tp_t, name, tp_t, val )
BCORE_FUNC_SPECT_CONST1_RET1_ARG1_MAP0( bcore_via, nget_vitem, const bcore_vitem_s*, tp_t, name )
BCORE_FUNC_SPECT_CONST1_RET1_ARG1_MAP0( bcore_via, nget_via,   const bcore_via_s*,   tp_t, name )
BCORE_FUNC_SPECT_CONST1_RET1_ARG1_MAP0( bcore_via, nget_array, const bcore_array_s*, tp_t, name )
BCORE_FUNC_SPECT_CONST1_RET1_ARG2_MAP0( bcore_via, nget_spect,                 vc_t, tp_t, name, tp_t, spect_type )

BCORE_FUNC_SPECT_CONST1_RET1_ARG0_MAP0( bcore_via, is_leaf,       bl_t )              // Leaf type according to function bcore_type_is_leaf
BCORE_FUNC_SPECT_CONST1_RET1_ARG0_MAP0( bcore_via, is_pure_array, bl_t )              // Checks if object is an array without additional elements (pure arrays are not leafs)
BCORE_FUNC_SPECT_CONST1_RET1_ARG1_MAP0( bcore_via, iis_array,     bl_t, uz_t, index ) // Checks if element is an array
BCORE_FUNC_SPECT_CONST1_RET1_ARG1_MAP0( bcore_via, iis_static,    bl_t, uz_t, index ) // Checks if element is static (type need not be recorded)
BCORE_FUNC_SPECT_CONST1_RET1_ARG1_MAP0( bcore_via, iis_link,      bl_t, uz_t, index ) // Checks if element is a link (means that it can be NULL); an array is a distinct static object -> not a link)

BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP0( bcore_via, source, bcore_source*, source ) // Sends signal 'bcore_spect_via_call : source' to object
BCORE_FUNC_SPECT_CONST0_RET0_ARG0_MAP0( bcore_via, mutated ) // Sends signal 'bcore_spect_via_call : mutated' to object
BCORE_FUNC_SPECT_CONST0_RET0_ARG0_MAP0( bcore_via, shelve )  // Sends signal 'bcore_spect_via_call : shelve' to object
BCORE_FUNC_SPECT_CONST0_RET1_ARG0_MAP0( bcore_via, defines_shelve, bl_t ) // checks if bcore_spect_via_call : shelve is implemented for given object

vd_t bcore_spect_via_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/
// testing, debugging

/// creates a structured object for testing purposes
sr_s bcore_spect_via_create_zoo( uz_t size );

#endif
