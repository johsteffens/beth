/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#ifndef BCLOS_LIBRARY_H
#define BCLOS_LIBRARY_H

#include "bcore_flect.h"
#include "bcore_hmap_tp_sr.h"

/** The library object is an intermediate object code state from which a frame can be constructed */
typedef struct bclos_library_arr_s bclos_library_arr_s;

typedef struct bclos_library_s
{
    aware_t _;
    bcore_hmap_tp_sr_s map;
    bclos_library_arr_s* deps; // dependencies
} bclos_library_s;

DECLARE_FUNCTION_INIT(    bclos_library_s )
DECLARE_FUNCTION_DOWN(    bclos_library_s )
DECLARE_FUNCTION_COPY(    bclos_library_s )
DECLARE_FUNCTION_CREATE(  bclos_library_s )
DECLARE_FUNCTION_DISCARD( bclos_library_s )
DECLARE_FUNCTION_CLONE(   bclos_library_s )

typedef struct bclos_library_arr_s
{
    aware_t _;
    union
    {
        bcore_static_link_array_s arr;
        struct
        {
            bclos_library_s** data;
            sz_t size, space;
        };
    };
} bclos_library_arr_s;

DECLARE_FUNCTION_INIT(    bclos_library_arr_s )
DECLARE_FUNCTION_DOWN(    bclos_library_arr_s )
DECLARE_FUNCTION_COPY(    bclos_library_arr_s )
DECLARE_FUNCTION_CREATE(  bclos_library_arr_s )
DECLARE_FUNCTION_DISCARD( bclos_library_arr_s )
DECLARE_FUNCTION_CLONE(   bclos_library_arr_s )

#endif // BCLOS_LIBRARY_H

