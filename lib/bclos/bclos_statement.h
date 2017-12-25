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

#ifndef BCLOS_STATEMENT_H
#define BCLOS_STATEMENT_H

#include "bcore_flect.h"
#include "bcore_ref.h"
#include "bclos_frame.h"
#include "bclos_arguments.h"
#include "bclos_quicktypes.h"

/** A statement has the form: operation( arg1, arg2, ... ) -> [define] <target>;
 *  Specifiers are considered bindings (==names) referring to a given frame.
 *  'define' indicates whether target is to be taken from the frame (==0) or to
 *  be newly instantiated in the local frame (!=0).
 *  In the latter case it specifies the type of the target, allowing runtime type checking.
 *  'operation' specifies a closure, followed by the assigned arguments.
 */

typedef struct bclos_statement_s
{
    bclos_expression_s expression;
    tp_t          def_type; // 0 when target is not defined
    bclos_address_s target;
} bclos_statement_s;

DECLARE_FUNCTION_INIT(    bclos_statement_s )
DECLARE_FUNCTION_DOWN(    bclos_statement_s )
DECLARE_FUNCTION_COPY(    bclos_statement_s )
DECLARE_FUNCTION_CREATE(  bclos_statement_s )
DECLARE_FUNCTION_DISCARD( bclos_statement_s )
DECLARE_FUNCTION_CLONE(   bclos_statement_s )

sr_s               bclos_statement_s_run(         const bclos_statement_s* o, bclos_frame_s* frm );

// Format:  <closure>( <name>, <name>, ... ) -> [<def-type>] <target>;
void bclos_statement_s_parse_from_source( bclos_statement_s* o, sr_s source );

bclos_statement_s* bclos_statement_s_create_sc( sc_t sc );

vd_t bclos_statement_signal( tp_t target, tp_t signal, vd_t object );

/**********************************************************************************************************************/

#endif // BCLOS_STATEMENT_H
