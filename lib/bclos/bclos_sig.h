/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCLOS_SIG_H
#define BCLOS_SIG_H

#include "bcore_flect.h"
#include "bcore_ref.h"

typedef struct bclos_arg_sig_s
{
    tp_t name;
    tp_t type;
    bl_t is_const;
} bclos_arg_sig_s;

DECLARE_FUNCTION_INIT(    bclos_arg_sig_s )
DECLARE_FUNCTION_DOWN(    bclos_arg_sig_s )
DECLARE_FUNCTION_COPY(    bclos_arg_sig_s )
DECLARE_FUNCTION_CREATE(  bclos_arg_sig_s )
DECLARE_FUNCTION_DISCARD( bclos_arg_sig_s )
DECLARE_FUNCTION_CLONE(   bclos_arg_sig_s )

typedef struct bclos_sig_s
{
    union
    {
        bcore_static_array_s arr;
        struct
        {
            bclos_arg_sig_s* data;
            sz_t size, space;
        };
    };
    tp_t name; // name of closure
    tp_t ret;  // return type: 0: no return;
} bclos_sig_s;

DECLARE_FUNCTION_INIT(    bclos_sig_s )
DECLARE_FUNCTION_DOWN(    bclos_sig_s )
DECLARE_FUNCTION_COPY(    bclos_sig_s )
DECLARE_FUNCTION_CREATE(  bclos_sig_s )
DECLARE_FUNCTION_DISCARD( bclos_sig_s )
DECLARE_FUNCTION_CLONE(   bclos_sig_s )

void bclos_sig_s_push( bclos_sig_s* o, bclos_arg_sig_s arg_sig );

void bclos_sig_define_self_creators( void );

/**********************************************************************************************************************/

#endif // BCLOS_SIG_H
