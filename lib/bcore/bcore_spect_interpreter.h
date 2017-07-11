/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

/** The interpreter-perspective provides the interface for object de-serialization,
 *  generic object construction, data injection and function execution.
 *  It is a bi-perspective on a interpreter and the object to be modified.
 *  Interpreters are aware, objects need not be aware.
 *  The interpreter is required to implement the interpret feature. The object
 *  may implement the interpret feature, in which case it takes precedence.
 *  Object-side interpretation is intended to solve object specific conditions
 *  or requirements  where the default behavior is insufficient.
 *  It is not intended for the purpose of defining a new general way of interpretation.
 *  The interpreter-object should redirect interpretation of nested objects as indicated.
 *  The object can not redirect interpretation of itself back to the interpreter-object.
 */

#ifndef BCORE_SPECT_INTERPRETER_H
#define BCORE_SPECT_INTERPRETER_H

#include "bcore_flect.h"
#include "bcore_features.h"

typedef struct bcore_interpreter_s
{
    aware_t p_type; // perspective
    tp_t    t_type; // interpreter
    tp_t    o_type; // object
    bcore_fp_interpret_object interpret_object;
    bcore_fp_interpret_body   interpret_body;
} bcore_interpreter_s;

bcore_flect_self_s* bcore_interpreter_s_create_self();

const bcore_interpreter_s* bcore_interpreter_s_get_typed( tp_t t_type, tp_t o_type );

void bcore_interpret_typed_body(   vd_t intrp, vd_t source, tp_t o_type, vd_t obj );
void bcore_interpret_aware_body(   vd_t intrp, vd_t source,              vd_t obj );
dt_p bcore_interpret_object(       vd_t intrp, vd_t source );

#endif  // BCORE_SPECT_INTERPRETER_H

