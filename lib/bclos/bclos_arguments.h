/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#ifndef BCLOS_ARGS_H
#define BCLOS_ARGS_H

#include "bcore_flect.h"
#include "bcore_ref.h"

typedef struct bclos_arguments_s
{
    union
    {
        bcore_static_array_s arr;
        struct
        {
            sr_s* data;
            sz_t size, space;
        };
    };
} bclos_arguments_s;

DECLARE_FUNCTION_INIT(    bclos_arguments_s )
DECLARE_FUNCTION_DOWN(    bclos_arguments_s )
DECLARE_FUNCTION_COPY(    bclos_arguments_s )
DECLARE_FUNCTION_CREATE(  bclos_arguments_s )
DECLARE_FUNCTION_DISCARD( bclos_arguments_s )
DECLARE_FUNCTION_CLONE(   bclos_arguments_s )

bclos_arguments_s* bclos_arguments_s_create_nv( sz_t n, va_list args );
bclos_arguments_s* bclos_arguments_s_create_na( sz_t n, ... );

void bclos_arguments_s_push( bclos_arguments_s* o, sr_s sr );

bclos_arguments_s bclos_arguments_s_weak_crop( const bclos_arguments_s* src, sz_t start, sz_t end );

vd_t bclos_arguments_signal( tp_t target, tp_t signal, vd_t object );

/**********************************************************************************************************************/

#endif // BCLOS_ARGS_H
