/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

/** Beth-Markup-Language for marshaling and creation of objects.
 *  Implements features translate, interpret and supports corresponding perspectives.
 */

#ifndef BCORE_BML_H
#define BCORE_BML_H

#include "bcore_flect.h"
#include "bcore_features.h"

/**********************************************************************************************************************/

typedef struct bcore_bml_translator_s
{
    aware_t _;
    sz_t tab_size;
    bool suppress_aware; // suppress (usually redundant) aware_t
    bool break_leaf;     // line break before leaftypes (default: false);
} bcore_bml_translator_s;

void                    bcore_bml_translator_s_init(            bcore_bml_translator_s* o );
void                    bcore_bml_translator_s_down(            bcore_bml_translator_s* o );
void                    bcore_bml_translator_s_copy(            bcore_bml_translator_s* o, const bcore_bml_translator_s* src );
bcore_bml_translator_s* bcore_bml_translator_s_create();
bcore_bml_translator_s* bcore_bml_translator_s_clone(     const bcore_bml_translator_s* o );
void                    bcore_bml_translator_s_discard(         bcore_bml_translator_s* o );
bcore_flect_self_s*     bcore_bml_translator_s_create_self();

void bcore_bml_translator_s_translate_object( const bcore_bml_translator_s* o, tp_t type, vc_t obj, vd_t sink );
void bcore_bml_translator_s_translate_body(   const bcore_bml_translator_s* o, tp_t type, vc_t obj, vd_t sink );

/**********************************************************************************************************************/

typedef struct bcore_bml_interpreter_s
{
    aware_t _;
} bcore_bml_interpreter_s;

void                     bcore_bml_interpreter_s_init(            bcore_bml_interpreter_s* o );
void                     bcore_bml_interpreter_s_down(            bcore_bml_interpreter_s* o );
void                     bcore_bml_interpreter_s_copy(            bcore_bml_interpreter_s* o, const bcore_bml_interpreter_s* src );
bcore_bml_interpreter_s* bcore_bml_interpreter_s_create();
bcore_bml_interpreter_s* bcore_bml_interpreter_s_clone(     const bcore_bml_interpreter_s* o );
void                     bcore_bml_interpreter_s_discard(         bcore_bml_interpreter_s* o );
bcore_flect_self_s*      bcore_bml_interpreter_s_create_self();

dt_p bcore_bml_interpreter_s_interpret_object( const bcore_bml_interpreter_s* o, vd_t source );
dt_p bcore_bml_interpreter_s_interpret_typed(  const bcore_bml_interpreter_s* o, vd_t source, tp_t type );
dt_p bcore_bml_interpreter_s_interpret_body(   const bcore_bml_interpreter_s* o, vd_t source, tp_t type, vd_t obj );

/**********************************************************************************************************************/
// testing, debugging

bcore_string_s* bcore_bml_selftest();

#endif // BCORE_BML_H

