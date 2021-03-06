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

#ifndef BCLOS_ARGS_H
#define BCLOS_ARGS_H

#include "bcore_flect.h"
#include "bcore_sr.h"
#include "bclos_frame.h"
#include "bclos_quicktypes.h"

/** Arguments are represented by an array of objects.
 *  If an argument is bclos_expression_s, the intended closure-argument is the evaluation
 *  of the expression.
 *  If an argument is bclos_address_s, the intended closure-argument is the evaluation
 *  of the address.
 *  Closures are supposed to retrieve evaluated arguments via function bclos_arguments_s_get.
 */
typedef struct bclos_arguments_s
{
    union
    {
        bcore_array_dyn_solid_static_s arr;
        struct
        {
            sr_s* data;
            uz_t size, space;
        };
    };
} bclos_arguments_s;

BCORE_DECLARE_FUNCTIONS_OBJ( bclos_arguments_s )

void bclos_arguments_s_clear( bclos_arguments_s* o );
bclos_arguments_s* bclos_arguments_s_create_nv( uz_t n, va_list args );
bclos_arguments_s* bclos_arguments_s_create_na( uz_t n, ... );

void bclos_arguments_s_push( bclos_arguments_s* o, sr_s sr );

bclos_arguments_s bclos_arguments_s_weak_crop( const bclos_arguments_s* src, uz_t start, uz_t end );

// Format: ( <expression>, <expression>, ... )
void bclos_arguments_s_parse_from_source( bclos_arguments_s* o, sr_s source );

/**********************************************************************************************************************/

/** A expression represents an operation (closure) with arguments.
 *  If an argument is an expression, the intended closure-argument is the evaluation
 *  of the expression. An the expression-object occurring in the argument list is never
 *  the intended closure-argument itself but the carrier/producer of the argument.
 *  Closures are supposed to evaluate expressions.
 */
typedef struct bclos_expression_s
{
    sr_s closure; // reference | expression | address (all yielding a closure)
    bclos_arguments_s args;
} bclos_expression_s;

BCORE_DECLARE_FUNCTIONS_OBJ( bclos_expression_s )

void bclos_expression_s_clear( bclos_expression_s* o );

// Format: <expression>( <name>, <name>, ... )
void bclos_expression_s_parse_from_source( bclos_expression_s* o, sr_s source );

sr_s bclos_expression_s_run( const bclos_expression_s* o, bclos_frame_s* frm );

static inline sr_s bclos_arguments_s_get( const bclos_arguments_s* o, uz_t idx, bclos_frame_s* frm )
{
    assert( idx < o->size );
    sr_s* sr = &o->data[ idx ];
    tp_t type = sr_s_type( sr );
    switch( type )
    {
        case TYPEOF_bclos_address_s:    return sr_cw( *bclos_frame_s_get( frm, ( ( const bclos_address_s* )sr->o )->name ) );
        case TYPEOF_bclos_expression_s: return bclos_expression_s_run( sr->o, frm );
        default: break;
    }
    return sr_cw( *sr );
}

/**********************************************************************************************************************/

vd_t bclos_arguments_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BCLOS_ARGS_H
