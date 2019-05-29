/** Copyright 2019 Johannes Bernhard Steffens
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

/** Elman Recurrent Network
 *
 *  Schematics (activation omitted):
 *
 *       vec_input---->wgt_input-----.
 *                                  (+)-->vec_hidden-->wgt_hidden-->vec_output
 *    .->vec_context-->wgt_context---'        |
 *    |                                       |
 *    '---------------------------------------'
 *
 */

#ifndef BADAPT_ERN_H
#define BADAPT_ERN_H

#include "bcore_std.h"
#include "badapt_activator.h"
#include "badapt_adaptive.h"
#include "badapt_precoded.h"

/**********************************************************************************************************************/

BETH_PRECODE( badapt_ern )
#ifdef BETH_PRECODE_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

self badapt_ern_s = badapt_adaptive
{
    aware_t _;

    // === architecture parameters ================================
    sz_t size_input;
    sz_t size_output;

    badapt_dynamics_s dynamics;

    bmath_mf3_s wgt_input;
    bmath_mf3_s wgt_context;
    bmath_mf3_s wgt_hidden;

    f3_t context_epsilon_factor = 1.0;

    aware badapt_activator => activator_hidden;
    aware badapt_activator => activator_output;

    // === runtime data =============================================

    hidden bmath_vf3_s vec_input;
    hidden bmath_vf3_s vec_context;  // recurred from vec_hidden
    hidden bmath_vf3_s vec_hidden;
    hidden bmath_vf3_s vec_output;

    // ==============================================================

    // === adaptive functions =======================================
    func : get_in_size;
    func : get_out_size;

    func : get_dynamics;
    func : set_dynamics;

    func : arc_to_sink;
    func : minfer;

    func : bgrad;
    func : bgrad_adapt;
    // ==============================================================
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

self badapt_builder_ern_s = badapt_builder
{
    aware_t _;

    sz_t size_input;
    sz_t size_hidden = 8;
    sz_t size_output = 1;
    badapt_dynamics_s dynamics;

    f3_t context_epsilon_factor = 1.0;

    u2_t random_seed     = 1234;   // random seed variable (for random initialization)

    aware badapt_activator => activator_hidden;
    aware badapt_activator => activator_output;

    // === builder functions =======================================

    /// input vector size
    func : get_in_size;
    func : set_in_size;

    /// output vector size
    func : get_out_size;
    func : set_out_size;

    /// builds adaptive ready to be trained; passes ownership
    func : build;

    // ==============================================================
};

#endif // BETH_PRECODE_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**********************************************************************************************************************/

vd_t badapt_ern_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BADAPT_ERN_H
