/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCLOS_STATEMENT_H
#define BCLOS_STATEMENT_H

#include "bcore_flect.h"
#include "bcore_ref.h"
#include "bclos_env.h"
#include "bclos_args.h"

/** A statement has the form: [define] <target> = operation( arg1, arg2, ... )
 *  Specifiers are considered bindings (==names) referring to a given environment.
 *  'define' indicates whether target is to be found in the environment (==0) or to
 *  be newly instantiated in the local environment (!=0).
 *  In the latter case it specifies the type of the target, allowing runtime type checking.
 *  'operation' specifies a closure, followed by the assigned arguments.
 */
typedef struct bclos_statement_s
{
    tp_t define;
    tp_t target;
    tp_t operation;
    union
    {
        bcore_static_array_s args;
        struct
        {
            tp_t* args_data;
            sz_t args_size, args_space;
        };
    };
} bclos_statement_s;

DECLARE_FUNCTION_INIT(    bclos_statement_s )
DECLARE_FUNCTION_DOWN(    bclos_statement_s )
DECLARE_FUNCTION_COPY(    bclos_statement_s )
DECLARE_FUNCTION_CREATE(  bclos_statement_s )
DECLARE_FUNCTION_DISCARD( bclos_statement_s )
DECLARE_FUNCTION_CLONE(   bclos_statement_s )

void bclos_statement_s_push_arg( bclos_statement_s* o, tp_t arg );

bclos_args_s* bclos_statement_s_create_args( const bclos_statement_s* o, bclos_env_s* env );
void bclos_statement_s_run( const bclos_statement_s* o, bclos_env_s* env );

void bclos_statement_define_self_creators( void );

/**********************************************************************************************************************/

#endif // BCLOS_STATEMENT_H
