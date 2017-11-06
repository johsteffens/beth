/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#ifndef BCLOS_SYNTAX_H
#define BCLOS_SYNTAX_H

#include "bcore_flect.h"
#include "bclos_library.h"

/** The syntax object defines the text language used to construct a library */
typedef struct bclos_syntax_s
{
    aware_t _;
} bclos_syntax_s;

DECLARE_FUNCTION_INIT(    bclos_syntax_s )
DECLARE_FUNCTION_DOWN(    bclos_syntax_s )
DECLARE_FUNCTION_COPY(    bclos_syntax_s )
DECLARE_FUNCTION_CREATE(  bclos_syntax_s )
DECLARE_FUNCTION_DISCARD( bclos_syntax_s )
DECLARE_FUNCTION_CLONE(   bclos_syntax_s )

bclos_library_s* bclos_syntax_s_create_library( const bclos_syntax_s* o, const bclos_library_arr_s* deps, sr_s src );

#endif // BCLOS_SYNTAX_H
