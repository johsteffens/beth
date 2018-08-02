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

#ifndef BCLOS_PROCEDURE_H
#define BCLOS_PROCEDURE_H

#include "bcore_flect.h"
#include "bcore_sr.h"
#include "bclos_frame.h"
#include "bclos_arguments.h"
#include "bclos_statement.h"
#include "bclos_signature.h"

/// A procedure is a closure defined as array of statements.

typedef struct bclos_procedure_s
{
    aware_t _;
    union
    {
        bcore_array_dyn_link_static_s arr;
        struct
        {
            bclos_statement_s** data;
            uz_t size, space;
        };
    };

    bclos_signature_s* sig;
    bclos_frame_s* lexical; // (external) lexical frame
} bclos_procedure_s;


BCORE_DECLARE_FUNCTIONS_OBJ(    bclos_procedure_s )

void bclos_procedure_s_push(    bclos_procedure_s* o, sr_s statement );
void bclos_procedure_s_push_sc( bclos_procedure_s* o, sc_t statement );

void bclos_procedure_s_def(  bclos_procedure_s* o, bclos_frame_s* frm );
sr_s bclos_procedure_s_call( bclos_procedure_s* o, bclos_frame_s* frm, const bclos_arguments_s* args );
sr_s bclos_procedure_s_sig(  const bclos_procedure_s* o );

vd_t bclos_procedure_signal_handler( const bcore_signal_s* o );

#endif // BCLOS_PROCEDURE_H
