/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#include "bclos_signal.h"

#include "bclos_frame.h"
#include "bclos_arguments.h"
#include "bclos_signature.h"
#include "bclos_spect_closure.h"
#include "bclos_statement.h"
#include "bclos_closures.h"
#include "bclos_procedure.h"

vd_t bclos_signal( tp_t target, tp_t signal, vd_t object )
{
    bcore_fp_signal arr[] =
    {
        bclos_arguments_signal,
        bclos_frame_signal,
        bclos_closures_signal,
        bclos_procedure_signal,
        bclos_signature_signal,
        bclos_spect_closure_signal,
        bclos_statement_signal,
    };

    sz_t size = sizeof( arr ) / sizeof( bcore_fp_signal );
    vd_t ret = NULL;

    if( signal == typeof( "down0" ) || signal == typeof( "down1" ) )
    {
        for( sz_t i = size; i > 0; i-- ) if( ( ret = arr[ i - 1 ]( target, signal, object ) ) ) return ret;
    }
    else
    {
        for( sz_t i = 0; i < size; i++ ) if( ( ret = arr[ i     ]( target, signal, object ) ) ) return ret;
    }

    return ret;

}
