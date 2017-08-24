/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCORE_SPECT_H
#define BCORE_SPECT_H

/** Perspective management framework.
 *  The perspective establishes abstraction on via reflection and specified features.
 *  Perspective instances are managed here.
 *
 *  A perspective needs to satisfy following requirements:
 *    * self-awareness
 *    * Providing a reflection
 *    * Registering following features in its reflection:
 *      * bcore_fp_init
 *      * bcore_fp_down
 *      * bcore_spect_fp_create_from_self
 */

#include "bcore_features.h"
#include "bcore_control.h"
#include "bcore_signature.h"

/// Features...

/// Signature creation from a perspective-instance
// typedef bcore_signature_s* (*bcore_spect_fp_create_signature )( vc_t* spect );

/// perspective-instance creation from object reflections
typedef bcore_flect_self_s* (*bcore_spect_fp_create_from_self )( const bcore_flect_self_s* self );

/// initializes perspective instance manager
void bcore_spect_manager_open();

/// closes perspective instance manager
void bcore_spect_manager_close();

/// enrolls perspective instance (thread safe)
void bcore_spect_enroll_d( vd_t spect );

/// returns perspective according to its signature (NULL if not enrolled)  (thread safe)
vc_t bcore_spect_try( tp_t sig );

/// returns perspective according to its signature (error if not enrolled) (thread safe)
vc_t bcore_spect_get( tp_t sig );

/// removes perspective instance according to its signature; no effect when not enrolled (thread safe)
void bcore_spect_remove( tp_t sig );

/// returns status information (thread safe)
bcore_string_s* bcore_spect_status();

/** returns perspective according to perspective and object types. Enrolls automatically if needed. (thread safe)
 *  p_type : type of perspective
 *  o_type : type of object
 */
vc_t bcore_spect_get_typed( tp_t p_type, tp_t o_type );

/**********************************************************************************************************************/

// syntactic sugar
vc_t ch_spect( vc_t p, tp_t spect_type ); // if needed changes perspective to spect_type; returns NULL if p is NULL

#endif // BCORE_SPECT_H

