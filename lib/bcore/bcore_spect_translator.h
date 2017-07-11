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
    bcore_fp_translate_object translate_object;
    bcore_fp_translate_body   translate_body;
} bcore_translator_s;

bcore_flect_self_s* bcore_translator_s_create_self();

const bcore_translator_s* bcore_translator_s_get_typed( tp_t t_type, tp_t o_type );

void bcore_translate_typed_body(   vd_t trans, tp_t o_type, vc_t obj, vd_t sink );
void bcore_translate_aware_body(   vd_t trans,              vc_t obj, vd_t sink );
void bcore_translate_typed_object( vd_t trans, tp_t o_type, vc_t obj, vd_t sink );
void bcore_translate_aware_object( vd_t trans,              vc_t obj, vd_t sink );

#endif  // BCORE_SPECT_TRANSLATOR_H

