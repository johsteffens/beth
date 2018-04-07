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

#ifndef BCORE_SPECT_H
#define BCORE_SPECT_H

/** Perspective management framework.
 *  The perspective establishes abstraction on via reflection and specified features.
 *  Perspective instances are managed here.
 *
 *  A perspective needs to satisfy following requirements:
 *    * self-awareness
 *    * object-awareness (value after aware_t must be tp_t of the object's type)
 *    * Providing a reflection
 *    * Registering following features in its reflection:
 *      * bcore_fp_init
 *      * bcore_fp_down
 *      * bcore_fp_discard
 *      * bcore_spect_fp_create_from_self
 */

#include "bcore_features.h"
#include "bcore_control.h"
#include "bcore_flect.h"

/**********************************************************************************************************************/

/// common header of all perspectives
typedef struct bcore_spect_header_s
{
    aware_t p_type; // type of perspective
    tp_t    o_type; // type of object
} bcore_spect_header_s;

BCORE_DECLARE_OBJECT( bcore_spect_header_s )

/**********************************************************************************************************************/

/// perspective-instance creation from object reflections
typedef bcore_self_s* (*bcore_spect_fp_create_from_self )( const bcore_self_s* self );
typedef bl_t                (*bcore_spect_fp_supports         )( const bcore_self_s* self );

/** Tests if perspective indicated by spect_trait is supported by object o_type. (thread safe)
 *  This function is fast and does not actually construct the perspective.
 *  Returns true if p_type or o_type exist and support each other.
 *  Returns false otherwise.
 */
bl_t bcore_spect_trait_supported( tp_t spect_trait, tp_t o_type );

/** Returns perspective according to perspective and object types. Enrolls automatically if needed. (thread safe)
 *  Error if combination is not supported.
 *  p_type : type of perspective
 *  o_type : type of object
 */
vc_t bcore_spect_get_typed( tp_t p_type, tp_t o_type );

/**********************************************************************************************************************/

/** Generic perspective functions assuming all information is stored in reflections.
 *  These functions make use of bcore_inst_s
 */
void bcore_spect_define_trait(     const bcore_self_s* p_self );
vd_t bcore_spect_create_from_self( const bcore_self_s* p_self, const bcore_self_s* o_self );
vd_t bcore_spect_create_from_self_typed( tp_t p_type, tp_t o_type );

/** Defining reflection of perspective via creation function
 *  This function create a temporary instance of self and calls
 *  bcore_spect_define_trait
 */
void bcore_spect_define_creator( tp_t type, bcore_flect_create_self_fp creator );

/**********************************************************************************************************************/

/// returns status information (thread safe)
st_s* bcore_spect_status();

/**********************************************************************************************************************/

// syntactic sugar
vc_t ch_spect_p( vc_t p, tp_t p_type ); // if needed changes perspective to spect_type; returns NULL if p is NULL
vc_t ch_spect_o( vc_t p, tp_t o_type ); // if needed changes perspective to object_type; returns NULL if p is NULL

static inline tp_t spect_tp_p( vc_t p ) { return ( ( tp_t* )p )[ 0 ]; } // type of perspective
static inline tp_t spect_tp_o( vc_t p ) { return ( ( tp_t* )p )[ 1 ]; } // type of object

vd_t bcore_spect_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/
// macros

#define BCORE_REGISTER_SPECT( name )\
    bcore_spect_define_creator( typeof( #name ), name##_create_self )

#define BCORE_DEFINE_INLINE_SPECT_GET_TYPED( name ) \
    static inline const name * name##_get_typed( tp_t o_type ) \
    { \
        return bcore_spect_get_typed( TYPEOF_##name, o_type ); \
    }

#define BCORE_DEFINE_INLINE_SPECT_GET_AWARE( name ) \
    static inline const name* name##_get_aware( vc_t obj ) \
    { \
        return name##_get_typed( *( const aware_t* )obj ); \
    }

#endif // BCORE_SPECT_H

