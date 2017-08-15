/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

/** Text based markup language for marshaling and creation of objects.
 *  Implements features translate, interpret and supports corresponding perspectives.
 */

#ifndef BCORE_TXT_ML_H
#define BCORE_TXT_ML_H

#include "bcore_flect.h"
#include "bcore_features.h"

/**********************************************************************************************************************/

typedef struct bcore_txt_ml_translator_s
{
    aware_t _;
} bcore_txt_ml_translator_s;

DECLARE_FUNCTION_INIT(    bcore_txt_ml_translator_s )
DECLARE_FUNCTION_DOWN(    bcore_txt_ml_translator_s )
DECLARE_FUNCTION_COPY(    bcore_txt_ml_translator_s )
DECLARE_FUNCTION_CREATE(  bcore_txt_ml_translator_s )
DECLARE_FUNCTION_DISCARD( bcore_txt_ml_translator_s )
DECLARE_FUNCTION_CLONE(   bcore_txt_ml_translator_s )

void bcore_txt_ml_translator_s_translate_object( const bcore_txt_ml_translator_s* o, tp_t type, vc_t obj, vd_t sink );
void bcore_txt_ml_translator_s_translate_body(   const bcore_txt_ml_translator_s* o, tp_t type, vc_t obj, vd_t sink );

/**********************************************************************************************************************/

/// syntactic sugar
void bcore_txt_ml_typed_to_stdout( tp_t type, vc_t obj );
void bcore_txt_ml_aware_to_stdout(            vc_t obj );
void bcore_txt_ml_typed_to_file(   tp_t type, vc_t obj, sc_t file );
void bcore_txt_ml_aware_to_file(              vc_t obj, sc_t file );
void bcore_txt_ml_typed_to_string( tp_t type, vc_t obj, bcore_string_s* string );
void bcore_txt_ml_aware_to_string(            vc_t obj, bcore_string_s* string );

/**********************************************************************************************************************/

void bcore_txt_ml_define_self_creators( void );

/**********************************************************************************************************************/
// testing, debugging

bcore_string_s* bcore_txt_ml_selftest( void );

#endif // BCORE_TXT_ML_H

