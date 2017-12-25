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

/// Features...

/// perspective-instance creation from object reflections
typedef bcore_flect_self_s* (*bcore_spect_fp_create_from_self )( const bcore_flect_self_s* self );
typedef bl_t                (*bcore_spect_fp_supports         )( const bcore_flect_self_s* self );

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

/// returns status information (thread safe)
st_s* bcore_spect_status();

/**********************************************************************************************************************/

// syntactic sugar
vc_t ch_spect_p( vc_t p, tp_t ptype ); // if needed changes perspective to spect_type; returns NULL if p is NULL
vc_t ch_spect_o( vc_t p, tp_t otype ); // if needed changes perspective to object_type; returns NULL if p is NULL

static inline tp_t spect_tp_p( vc_t p ) { return ( ( tp_t* )p )[ 0 ]; } // type of perspective
static inline tp_t spect_tp_o( vc_t p ) { return ( ( tp_t* )p )[ 1 ]; } // type of object

vd_t bcore_spect_signal( tp_t target, tp_t signal, vd_t object );

#endif // BCORE_SPECT_H

