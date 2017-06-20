/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

/** Beth-Markup-Language for marshaling and creation of objects.
 *  Implements features translate, interpret and supports corresponding perspectives.
 */

#ifndef BCORE_BML_H
#define BCORE_BML_H

#include "bcore_perspective.h"
#include "bcore_flect.h"
#include "bcore_features.h"

/**********************************************************************************************************************/

typedef struct bcore_bml_translator_s
{
    aware_t _;
    sz_t tab_size;
    bool suppress_aware; // suppress (usually redundant) aware_t
} bcore_bml_translator_s;

void                    bcore_bml_translator_s_init(            bcore_bml_translator_s* o );
void                    bcore_bml_translator_s_down(            bcore_bml_translator_s* o );
void                    bcore_bml_translator_s_copy(            bcore_bml_translator_s* o, const bcore_bml_translator_s* src );
bcore_bml_translator_s* bcore_bml_translator_s_create();
bcore_bml_translator_s* bcore_bml_translator_s_clone(     const bcore_bml_translator_s* o );
void                    bcore_bml_translator_s_discard(         bcore_bml_translator_s* o );
void                    bcore_bml_translator_s_translate( const bcore_bml_translator_s* o, tp_t type, vc_t obj, vd_t flow_sink );
bcore_flect_self_s*     bcore_bml_translator_s_create_self();

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
void                     bcore_bml_interpreter_s_interpret( const bcore_bml_interpreter_s* o, tp_t obj_type, vd_t obj, bcore_fp_flow_src src, vd_t flow_o );
bcore_flect_self_s*      bcore_bml_interpreter_s_create_self();

/**********************************************************************************************************************/
// testing, debugging

bcore_string_s* bcore_bml_selftest();

#endif // BCORE_BML_H

