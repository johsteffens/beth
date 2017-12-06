/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

/** The interpreter-perspective provides the interface for object de-serialization, */

#ifndef BCORE_SPECT_INTERPRETER_H
#define BCORE_SPECT_INTERPRETER_H

#include "bcore_flect.h"
#include "bcore_features.h"

typedef struct bcore_interpreter_s
{
    aware_t p_type; // perspective
    tp_t    o_type; // interpreter
    bcore_fp_interpret fp_interpret;
} bcore_interpreter_s;

const bcore_interpreter_s* bcore_interpreter_s_get_typed( tp_t type );

sr_s bcore_interpret( sr_s o, sr_s source );
sr_s bcore_interpret_q( const sr_s* o, sr_s source );

/** Automatically picks the right interpreter according to source content.
 *  In text or binary files:
 *  The first object is deemed to be the interpreter in the default format of bcore_txt_ml
 *  Subsequent data is interpreted by the specified interpreter (either binary or ascii)
 *
 *  Example:
 *  The stream may begin with "<bcore_bin_ml_interpreter_s></>".
 *  Data following this header will be interpreted in the syntax of bcore_bin_ml_interpreter_s
 */
sr_s bcore_interpret_auto( sr_s source );
sr_s bcore_interpret_auto_file( sc_t file );

vd_t bcore_spect_interpreter_signal( tp_t target, tp_t signal, vd_t object );

#endif  // BCORE_SPECT_INTERPRETER_H

