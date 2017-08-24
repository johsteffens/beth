/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

/** The translator-perspective provides the interface for object serialization.
 *  It is a bi-perspective on a translator an an object (to be translated).
 *  Translators are aware, objects need not be aware.
 *  The translator is required to implement the translate feature. The object
 *  may implement the translate feature, in which case it takes precedence.
 *  Object-side translation is intended to solve object specific conditions
 *  or requirements where the default behavior is insufficient.
 *  The translator-object should redirect translation of nested objects as indicated.
 *  The object can not redirect translation of itself back to the translation-object.
 *
 *  Object-Amoebic features: translate_body
 */

#ifndef BCORE_SPECT_TRANSLATOR_H
#define BCORE_SPECT_TRANSLATOR_H

#include "bcore_flect.h"
#include "bcore_features.h"

typedef struct bcore_translator_s
{
    aware_t p_type; // perspective
    tp_t    t_type; // translator
    tp_t    o_type; // object

    // do not call these functions from outside
    bcore_fp_translate_object translate_object;
    bcore_fp_translate_body   translate_body;
    ap_t                      translate_body_amoeba;

    bcore_fp_translate        fp_translate;
} bcore_translator_s;

bcore_flect_self_s* bcore_translator_s_create_self( void );

const bcore_translator_s* bcore_translator_s_get_typed( tp_t t_type, tp_t o_type );

// use functions below from outside
void bcore_translate_spect_body( const bcore_translator_s* spect, vc_t trans, vc_t obj, vd_t sink );
void bcore_translate_typed_body( vc_t trans, tp_t o_type, vc_t obj, vd_t sink );
void bcore_translate_aware_body( vc_t trans, vc_t obj, vd_t sink );

void bcore_translate_spect_object( const bcore_translator_s* spect, vc_t trans, vc_t obj, vd_t sink );
void bcore_translate_typed_object( vc_t trans, tp_t o_type, vc_t obj, vd_t sink );
void bcore_translate_aware_object( vc_t trans, vc_t obj, vd_t sink );

void bcore_translate( sr_s o, sr_s obj, sr_s sink );

#endif  // BCORE_SPECT_TRANSLATOR_H

