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
    sz_t indent; // indentation
} bcore_txt_ml_translator_s;

DECLARE_FUNCTION_INIT(    bcore_txt_ml_translator_s )
DECLARE_FUNCTION_DOWN(    bcore_txt_ml_translator_s )
DECLARE_FUNCTION_COPY(    bcore_txt_ml_translator_s )
DECLARE_FUNCTION_CREATE(  bcore_txt_ml_translator_s )
DECLARE_FUNCTION_DISCARD( bcore_txt_ml_translator_s )
DECLARE_FUNCTION_CLONE(   bcore_txt_ml_translator_s )

void bcore_txt_ml_translator_s_translate( const bcore_txt_ml_translator_s* o, sr_s obj, sr_s snk );

/**********************************************************************************************************************/

/// syntactic sugar
void bcore_txt_ml_to_stdout( sr_s obj );
void bcore_txt_ml_to_file(   sr_s obj, sc_t file );
void bcore_txt_ml_to_string( sr_s obj, bcore_string_s* string );

/**********************************************************************************************************************/

typedef struct bcore_txt_ml_interpreter_s
{
    aware_t _;
} bcore_txt_ml_interpreter_s;

void                        bcore_txt_ml_interpreter_s_init(            bcore_txt_ml_interpreter_s* o );
void                        bcore_txt_ml_interpreter_s_down(            bcore_txt_ml_interpreter_s* o );
void                        bcore_txt_ml_interpreter_s_copy(            bcore_txt_ml_interpreter_s* o, const bcore_txt_ml_interpreter_s* src );
bcore_txt_ml_interpreter_s* bcore_txt_ml_interpreter_s_create();
bcore_txt_ml_interpreter_s* bcore_txt_ml_interpreter_s_clone(     const bcore_txt_ml_interpreter_s* o );
void                        bcore_txt_ml_interpreter_s_discard(         bcore_txt_ml_interpreter_s* o );

sr_s bcore_txt_ml_interpreter_s_interpret( const bcore_txt_ml_interpreter_s* o, sr_s src );

/**********************************************************************************************************************/

/// syntactic sugar
sr_s bcore_txt_ml_from_file( sc_t file );
sr_s bcore_txt_ml_from_string( const bcore_string_s* string );

/**********************************************************************************************************************/

void bcore_txt_ml_define_self_creators( void );

/**********************************************************************************************************************/
// testing, debugging

bcore_string_s* bcore_txt_ml_selftest( void );

#endif // BCORE_TXT_ML_H

