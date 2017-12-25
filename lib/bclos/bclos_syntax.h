/** Copyright 2017 Johannes Bernhard Steffens
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

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
