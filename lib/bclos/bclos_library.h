/** Author and Copyright 2017 Johannes Bernhard Steffens
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

BCORE_DECLARE_FUNCTION_INIT(    bclos_library_s )
BCORE_DECLARE_FUNCTION_DOWN(    bclos_library_s )
BCORE_DECLARE_FUNCTION_COPY(    bclos_library_s )
BCORE_DECLARE_FUNCTION_CREATE(  bclos_library_s )
BCORE_DECLARE_FUNCTION_DISCARD( bclos_library_s )
BCORE_DECLARE_FUNCTION_CLONE(   bclos_library_s )

typedef struct bclos_library_arr_s
{
    aware_t _;
    union
    {
        bcore_array_dyn_link_static_s arr;
        struct
        {
            bclos_library_s** data;
            uz_t size, space;
        };
    };
} bclos_library_arr_s;

BCORE_DECLARE_FUNCTION_INIT(    bclos_library_arr_s )
BCORE_DECLARE_FUNCTION_DOWN(    bclos_library_arr_s )
BCORE_DECLARE_FUNCTION_COPY(    bclos_library_arr_s )
BCORE_DECLARE_FUNCTION_CREATE(  bclos_library_arr_s )
BCORE_DECLARE_FUNCTION_DISCARD( bclos_library_arr_s )
BCORE_DECLARE_FUNCTION_CLONE(   bclos_library_arr_s )

vd_t bclos_library_signal_handler( const bcore_signal_s* o );

#endif // BCLOS_LIBRARY_H

