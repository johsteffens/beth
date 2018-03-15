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

#include "bcore_signal.h"
#include "bclos_signal.h"

#include "bclos_frame.h"
#include "bclos_arguments.h"
#include "bclos_signature.h"
#include "bclos_spect_closure.h"
#include "bclos_statement.h"
#include "bclos_closures.h"
#include "bclos_objects.h"
#include "bclos_procedure.h"
#include "bclos_machine.h"

vd_t bclos_signal_handler( const bcore_signal_s* o )
{
    bcore_fp_signal_handler arr[] =
    {
        bclos_arguments_signal_handler,
        bclos_frame_signal_handler,
        bclos_objects_signal_handler,
        bclos_closures_signal_handler,
        bclos_procedure_signal_handler,
        bclos_signature_signal_handler,
        bclos_spect_closure_signal_handler,
        bclos_statement_signal_handler,
        bclos_machine_signal_handler,
    };

    return bcore_signal_s_broadcast( o, arr, sizeof( arr ) / sizeof( bcore_fp_signal_handler ) );
}

