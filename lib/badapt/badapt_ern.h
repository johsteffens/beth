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

stamp :layer = bcore_inst
{
    hidden bmath_vf3_s v_x;
    hidden bmath_vf3_s v_c;
    hidden bmath_vf3_s v_h;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stamp :arr_layer = aware bcore_array{ badapt_ern_layer_s => [] arr; };

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stamp : = aware badapt_adaptive
{
    // === architecture parameters ================================
    sz_t size_input;
    sz_t size_hidden;
    sz_t size_output;
    sz_t size_unfolded;

    badapt_dynamics_std_s dynamics;

    bmath_mf3_s w_hx;
    bmath_mf3_s w_hc;
    bmath_mf3_s w_oh;

    aware badapt_activator => a_h;
    aware badapt_activator => a_o;

    // === runtime data =============================================
    hidden bmath_vf3_s v_o;

    /// gradients
    hidden bmath_vf3_s v_go;
    hidden bmath_vf3_s v_gc;
    hidden bmath_vf3_s v_gh;
    hidden bmath_mf3_s gw_hx;
    hidden bmath_mf3_s gw_hc;

    hidden badapt_ern_arr_layer_s arr_layer;
    // ==============================================================

    // === adaptive functions =======================================
    func : get_in_size      = { return o->size_input; };
    func : get_out_size     = { return o->size_output; };
    func : get_dynamics_std = { badapt_dynamics_std_s_copy( dynamics, &o->dynamics ); };
    func : set_dynamics_std = { badapt_dynamics_std_s_copy( &o->dynamics, dynamics ); };
    func : arc_to_sink;
    func : minfer;
    func : bgrad_adapt;
    func : get_weights_min_max;
    // ==============================================================
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stamp :builder = aware badapt_builder
{
    sz_t size_input;
    sz_t size_hidden = 8;
    sz_t size_output = 1;
    sz_t size_unfolded = 1;  // number of unfolded time steps during learning

    badapt_dynamics_std_s dynamics;

    u2_t random_seed = 1234;   // random seed variable (for random initialization)

    aware badapt_activator => a_h;
    aware badapt_activator => a_o;

    // === builder functions =======================================

    /// input vector size
    func : get_in_size = { return o->size_input; };
    func : set_in_size = { o->size_input = size; };

    /// output vector size
    func : get_out_size = { return o->size_output; };
    func : set_out_size = { o->size_output = size; };

    /// builds adaptive ready to be trained; passes ownership
    func : build;

    // ==============================================================
};

#endif // BETH_PRECODE_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**********************************************************************************************************************/

vd_t badapt_ern_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BADAPT_ERN_H
