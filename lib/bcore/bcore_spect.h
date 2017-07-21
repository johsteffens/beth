/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCORE_SPECT_H
#define BCORE_SPECT_H

/** Perspective management framework.
 *  The perspective establishes abstraction on via reflection and specified features.
 *  It is an approach to generalize functionality and can be seen as a hybrid of
 *  generic programming and abstract interfacing.
 *
 *  Perspective instances are managed here.
 *
 *  A perspective needs to satisfy following requirements:
 *    * self-awareness
 *    * Providing a reflection
 *    * Registering following features in its reflection:
 *      * bcore_fp_init
 *      * bcore_fp_down
 *      * bcore_spect_fp_create_signature
 *      * bcore_spect_fp_create_from_self
 */

#include "bcore_features.h"
#include "bcore_control.h"
#include "bcore_signature.h"

/// Features...

/// Signature creation from a perspective-instance
typedef bcore_signature_s* (*bcore_spect_fp_create_signature )( vc_t* spect );

/// perspective-instance creation from object reflections
typedef bcore_flect_self_s* (*bcore_spect_fp_create_from_self )( const bcore_flect_self_s** self_arr );

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

/// removes perspective according to its signature; no effect when not enrolled (thread safe)
void bcore_spect_remove( tp_t sig );

/** returns perspective according to perspective and object types. Enrolls automatically if needed. (thread safe)
 *  p_type : type of perspective
 *  n      : number of objects the perspective governs
 *  arr    : respective types as array maintained by caller
 */
vc_t bcore_spect_get_typed_n( tp_t p_type, sz_t n, const tp_t* arr );
vc_t bcore_spect_get_typed( tp_t p_type, tp_t o_type ); // for n == 1

#endif // BCORE_SPECT_H

