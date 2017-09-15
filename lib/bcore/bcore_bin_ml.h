/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

/** Binary markup language for marshaling and creation of objects.
 *  Implements features translate, interpret and supports corresponding perspectives.
 */

#ifndef BCORE_BIN_ML_H
#define BCORE_BIN_ML_H

#include "bcore_flect.h"
#include "bcore_features.h"

/**********************************************************************************************************************/

typedef struct bcore_bin_ml_translator_s
{
    aware_t _;
} bcore_bin_ml_translator_s;

DECLARE_FUNCTION_INIT(    bcore_bin_ml_translator_s )
DECLARE_FUNCTION_DOWN(    bcore_bin_ml_translator_s )
DECLARE_FUNCTION_COPY(    bcore_bin_ml_translator_s )
DECLARE_FUNCTION_CREATE(  bcore_bin_ml_translator_s )
DECLARE_FUNCTION_DISCARD( bcore_bin_ml_translator_s )
DECLARE_FUNCTION_CLONE(   bcore_bin_ml_translator_s )

void bcore_bin_ml_translator_s_translate( const bcore_bin_ml_translator_s* o, sr_s obj, sr_s snk );

/**********************************************************************************************************************/

/// syntactic sugar
sr_s bcore_bin_ml_to_sink_buffer( sr_s obj ); // returns bcore_sink_buffer_s
void bcore_bin_ml_to_file( sr_s obj, sc_t file );

/**********************************************************************************************************************/

typedef struct bcore_bin_ml_interpreter_s
{
    aware_t _;
} bcore_bin_ml_interpreter_s;

void                        bcore_bin_ml_interpreter_s_init(            bcore_bin_ml_interpreter_s* o );
void                        bcore_bin_ml_interpreter_s_down(            bcore_bin_ml_interpreter_s* o );
void                        bcore_bin_ml_interpreter_s_copy(            bcore_bin_ml_interpreter_s* o, const bcore_bin_ml_interpreter_s* src );
bcore_bin_ml_interpreter_s* bcore_bin_ml_interpreter_s_create();
bcore_bin_ml_interpreter_s* bcore_bin_ml_interpreter_s_clone(     const bcore_bin_ml_interpreter_s* o );
void                        bcore_bin_ml_interpreter_s_discard(         bcore_bin_ml_interpreter_s* o );

sr_s bcore_bin_ml_interpreter_s_interpret( const bcore_bin_ml_interpreter_s* o, sr_s src );

/**********************************************************************************************************************/

/// syntactic sugar
sr_s bcore_bin_ml_from_file( sc_t file );

/**********************************************************************************************************************/
// testing, debugging

/// converts object into stream; interprets stream and compares result to original
void bcore_bin_ml_transfer_test( sr_s obj );

/**********************************************************************************************************************/

vd_t bcore_bin_ml_signal( tp_t target, tp_t signal, vd_t object );

#endif // BCORE_BIN_ML_H

