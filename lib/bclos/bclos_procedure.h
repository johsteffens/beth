/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#ifndef BCLOS_PROCEDURE_H
#define BCLOS_PROCEDURE_H

#include "bcore_flect.h"
#include "bcore_ref.h"
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
        bcore_static_link_array_s arr;
        struct
        {
            bclos_statement_s** data;
            sz_t size, space;
        };
    };

    bclos_signature_s* sig;
    bclos_frame_s* lexical; // (external) lexical frame
} bclos_procedure_s;


DECLARE_FUNCTION_INIT(    bclos_procedure_s )
DECLARE_FUNCTION_DOWN(    bclos_procedure_s )
DECLARE_FUNCTION_COPY(    bclos_procedure_s )
DECLARE_FUNCTION_CREATE(  bclos_procedure_s )
DECLARE_FUNCTION_DISCARD( bclos_procedure_s )
DECLARE_FUNCTION_CLONE(   bclos_procedure_s )

void bclos_procedure_s_push(    bclos_procedure_s* o, sr_s statement );
void bclos_procedure_s_push_sc( bclos_procedure_s* o, sc_t statement );

void bclos_procedure_s_def(  bclos_procedure_s* o, bclos_frame_s* frm );
sr_s bclos_procedure_s_call( bclos_procedure_s* o, bclos_frame_s* frm, const bclos_arguments_s* args );
sr_s bclos_procedure_s_sig(  const bclos_procedure_s* o );

vd_t bclos_procedure_signal( tp_t target, tp_t signal, vd_t object );

#endif // BCLOS_PROCEDURE_H
