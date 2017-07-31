/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCLOS_LANG_C_H
#define BCLOS_LANG_C_H

/// Closure translator & interpreter using C syntax

#include "bcore_flect.h"
#include "bcore_features.h"

typedef struct bclos_lang_c_translator_s bclos_lang_c_translator_s;

void                       bclos_lang_c_translator_s_init(            bclos_lang_c_translator_s* o );
void                       bclos_lang_c_translator_s_down(            bclos_lang_c_translator_s* o );
void                       bclos_lang_c_translator_s_copy(            bclos_lang_c_translator_s* o, const bclos_lang_c_translator_s* src );
bclos_lang_c_translator_s* bclos_lang_c_translator_s_create();
bclos_lang_c_translator_s* bclos_lang_c_translator_s_clone(     const bclos_lang_c_translator_s* o );
void                       bclos_lang_c_translator_s_discard(         bclos_lang_c_translator_s* o );

void bclos_lang_c_translator_s_translate_object( const bclos_lang_c_translator_s* o, tp_t type, vc_t obj, vd_t sink );
void bclos_lang_c_translator_s_translate_body(   const bclos_lang_c_translator_s* o, tp_t type, vc_t obj, vd_t sink );

/**********************************************************************************************************************/

void bclos_lang_c_define_self_creators( void );

/**********************************************************************************************************************/
// testing, debugging

bcore_string_s* bclos_lang_c_selftest( void );

#endif // BCLOS_LANG_C_H
