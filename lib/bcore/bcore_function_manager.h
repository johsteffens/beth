/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

/** Function management framework.
 *  This framework links types to functions.
 *  Type value 0 is reserved and not allowed to be associated with a function.
 */

#ifndef BCORE_FUNCTION_MANAGER_H
#define BCORE_FUNCTION_MANAGER_H

#include "bcore_control.h"

/// enrolls function in global manager (thread safe); checks for collisions;
void bcore_function_set( tp_t t, fp_t f );
tp_t bcore_function_set_sc( sc_t name, fp_t f );

/// returns function associated with t or NULL if not enrolled (thread safe)
fp_t bcore_function_get( tp_t t );

/// removes t if existing
void bcore_function_remove( tp_t t );

vd_t bcore_function_manager_signal( tp_t target, tp_t signal, vd_t object );

#endif // BCORE_FUNCTION_MANAGER_H
