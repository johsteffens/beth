/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCLOS_PROCEDURE_H
#define BCLOS_PROCEDURE_H

#include "bcore_flect.h"
#include "bcore_ref.h"
#include "bclos_env.h"
#include "bclos_args.h"

/// A procedure is a closure defined as array of statements.


typedef struct bclos_procedure_s bclos_procedure_s;

DECLARE_FUNCTION_INIT(    bclos_procedure_s )
DECLARE_FUNCTION_DOWN(    bclos_procedure_s )
DECLARE_FUNCTION_COPY(    bclos_procedure_s )
DECLARE_FUNCTION_CREATE(  bclos_procedure_s )
DECLARE_FUNCTION_DISCARD( bclos_procedure_s )
DECLARE_FUNCTION_CLONE(   bclos_procedure_s )

void bclos_procedure_s_push( bclos_procedure_s* o, sr_s statement );

void bclos_procedure_s_def(  bclos_procedure_s* o, bclos_env_s* env );
sr_s bclos_procedure_s_call( bclos_procedure_s* o, bclos_env_s* env, const bclos_args_s* args );
sr_s bclos_procedure_s_sig(  const bclos_procedure_s* o );

void bclos_procedure_define_self_creators( void );

/**********************************************************************************************************************/

#endif // BCLOS_PROCEDURE_H
