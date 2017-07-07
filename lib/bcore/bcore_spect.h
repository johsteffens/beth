/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCORE_SPECT_H
#define BCORE_SPECT_H

/** Perspective management framework.
 *  The perspective establishes abstraction on via reflection and specified features.
 *  It is an approach to generalize functionality and can be seen as a hybrid of
 *  generic programming and abstract interfacing.
 *
 *  Perspective instances are managed here.
 *  Following requirements need to be satisfied:
 *    A perspective is self-aware
 *    A perspective instance owns a unique signature type by which it is identified.
 *    A perspective provides init and down functions in its reflection.
 */

#include "bcore_features.h"
#include "bcore_control.h"
#include "bcore_signature.h"

/// Perspective features
typedef bcore_signature_s* (*bcore_spect_fp_create_signature )( vc_t* spect );

/// initializes perspective instance manager
void bcore_spect_manager_open();

/// closes perspective instance manager
void bcore_spect_manager_close();

/// enrolls perspective instance (thread safe)
void bcore_spect_enroll_d( vd_t spect );

/// returns perspective according to its signature (NULL if not enrolled)   (thread safe)
vc_t bcore_spect_try( tp_t sig );

/// returns perspective according to its signature (error if not enrolled)  (thread safe)
vc_t bcore_spect_get( tp_t sig );

/// removes perspective according to its signature; no effect when not enrolled (thread safe)
void bcore_spect_remove( tp_t sig );

#endif // BCORE_SPECT_H

