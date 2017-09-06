/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCLOS_ARGS_H
#define BCLOS_ARGS_H

#include "bcore_flect.h"
#include "bcore_ref.h"

typedef struct bclos_args_s
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
} bclos_args_s;

DECLARE_FUNCTION_INIT(    bclos_args_s )
DECLARE_FUNCTION_DOWN(    bclos_args_s )
DECLARE_FUNCTION_COPY(    bclos_args_s )
DECLARE_FUNCTION_CREATE(  bclos_args_s )
DECLARE_FUNCTION_DISCARD( bclos_args_s )
DECLARE_FUNCTION_CLONE(   bclos_args_s )

bclos_args_s* bclos_args_s_create_nv( sz_t n, va_list args );
bclos_args_s* bclos_args_s_create_na( sz_t n, ... );

void          bclos_args_s_push(            bclos_args_s* o, sr_s sr );
bclos_args_s  bclos_args_s_weak_crop( const bclos_args_s* src, sz_t start, sz_t end );

void bclos_args_define_self_creators( void );

/**********************************************************************************************************************/

#endif // BCLOS_ARGS_H
