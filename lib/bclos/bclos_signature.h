/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCLOS_SIGNATURE_H
#define BCLOS_SIGNATURE_H

#include "bcore_flect.h"
#include "bcore_ref.h"

typedef struct bclos_signature_arg_s
{
    tp_t name;
    tp_t type;
    bl_t is_const;
} bclos_signature_arg_s;

DECLARE_FUNCTION_INIT(    bclos_signature_arg_s )
DECLARE_FUNCTION_DOWN(    bclos_signature_arg_s )
DECLARE_FUNCTION_COPY(    bclos_signature_arg_s )
DECLARE_FUNCTION_CREATE(  bclos_signature_arg_s )
DECLARE_FUNCTION_DISCARD( bclos_signature_arg_s )
DECLARE_FUNCTION_CLONE(   bclos_signature_arg_s )

typedef struct bclos_signature_s
{
    union
    {
        bcore_static_array_s arr;
        struct
        {
            bclos_signature_arg_s* data;
            sz_t size, space;
        };
    };
    tp_t name; // name of closure
    tp_t ret;  // return type: 0: no return;
} bclos_signature_s;

DECLARE_FUNCTION_INIT(    bclos_signature_s )
DECLARE_FUNCTION_DOWN(    bclos_signature_s )
DECLARE_FUNCTION_COPY(    bclos_signature_s )
DECLARE_FUNCTION_CREATE(  bclos_signature_s )
DECLARE_FUNCTION_DISCARD( bclos_signature_s )
DECLARE_FUNCTION_CLONE(   bclos_signature_s )

void bclos_signature_s_push( bclos_signature_s* o, bclos_signature_arg_s arg );

// Format: [type] <name>( [const] < type> <name>, [const] <type> <name>, ... );
bclos_signature_s* bclos_signature_s_parse_from_source( sr_s source );
bclos_signature_s* bclos_signature_s_parse_from_sc( sc_t sc );

/**********************************************************************************************************************/

void bclos_signature_define_self_creators( void );
vd_t bclos_signature_signal( tp_t target, tp_t signal, vd_t object );

/**********************************************************************************************************************/

#endif // BCLOS_SIGNATURE_H
