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

#ifndef BCLOS_SIGNATURE_H
#define BCLOS_SIGNATURE_H

#include "bcore_flect.h"
#include "bcore_sr.h"

typedef struct bclos_signature_arg_s
{
    tp_t name;
    tp_t type;
    bl_t is_const;
} bclos_signature_arg_s;

BCORE_DECLARE_FUNCTION_INIT(    bclos_signature_arg_s )
BCORE_DECLARE_FUNCTION_DOWN(    bclos_signature_arg_s )
BCORE_DECLARE_FUNCTION_COPY(    bclos_signature_arg_s )
BCORE_DECLARE_FUNCTION_CREATE(  bclos_signature_arg_s )
BCORE_DECLARE_FUNCTION_DISCARD( bclos_signature_arg_s )
BCORE_DECLARE_FUNCTION_CLONE(   bclos_signature_arg_s )

typedef struct bclos_signature_s
{
    union
    {
        bcore_array_dyn_solid_static_s arr;
        struct
        {
            bclos_signature_arg_s* data;
            uz_t size, space;
        };
    };
    tp_t name; // name of closure
    tp_t ret;  // return type: 0: no return;
} bclos_signature_s;

BCORE_DECLARE_FUNCTION_INIT(    bclos_signature_s )
BCORE_DECLARE_FUNCTION_DOWN(    bclos_signature_s )
BCORE_DECLARE_FUNCTION_COPY(    bclos_signature_s )
BCORE_DECLARE_FUNCTION_CREATE(  bclos_signature_s )
BCORE_DECLARE_FUNCTION_DISCARD( bclos_signature_s )
BCORE_DECLARE_FUNCTION_CLONE(   bclos_signature_s )

void bclos_signature_s_push( bclos_signature_s* o, bclos_signature_arg_s arg );

// Format: [type] <name>( [const] <type> <name>, [const] <type> <name>, ... );
bclos_signature_s* bclos_signature_s_parse_from_source( sr_s source );
bclos_signature_s* bclos_signature_s_parse_from_sc( sc_t sc );

/**********************************************************************************************************************/

vd_t bclos_signature_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BCLOS_SIGNATURE_H
