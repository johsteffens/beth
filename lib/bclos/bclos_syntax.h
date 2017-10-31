/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#ifndef BCLOS_SYNTAX_H
#define BCLOS_SYNTAX_H

#include "bcore_flect.h"

/** The syntax object defines the text language used to construct an frame */
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

#endif // BCLOS_SYNTAX_H
