/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

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

bcore_flect_self_s* bcore_interpreter_s_create_self( void );

const bcore_interpreter_s* bcore_interpreter_s_get_typed( tp_t type );

sr_s bcore_interpret( sr_s o, sr_s source );
sr_s bcore_interpret_q( const sr_s* o, sr_s source );

#endif  // BCORE_SPECT_INTERPRETER_H

