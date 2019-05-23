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

#ifndef BADAPT_PROBLEM_H
#define BADAPT_PROBLEM_H

#include "bmath_std.h"
#include "badapt_features.h"
#include "badapt_training.h"

/**********************************************************************************************************************/

BETH_PRECODE( badapt_problem_objects )
#ifdef BETH_PRECODE_SECTION

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** Type: Classifier
 *  Method: Differentiate between a sine wave of arbitrary amplitude and frequency from a random walk curve.
 */
self badapt_problem_sine_random_s = badapt_supplier
{
    aware_t _;
    sz_t input_size = 32;
    u2_t rval       = 1234;
    f3_t pos_tgt    =  0.9;
    f3_t neg_tgt    = -0.9;
    aware badapt_loss* preferred_loss = badapt_loss_l2_s;

    func badapt_supplier : preferred_loss;
    func badapt_supplier : get_in_size;
    func badapt_supplier : get_out_size;
    func badapt_supplier : fetch_sample_tio;
    func badapt_supplier : fetch_sample_vio;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** Type: Regression
 *  Method: Estimate polynomial coefficients for a signal within range -1, 1
 */
self badapt_problem_polynom_s = badapt_supplier
{
    aware_t _;
    sz_t input_size  = 32;
    sz_t output_size = 3;   // polynomial order + 1
    f3_t range       = 1.0; // +/- range of coefficients
    u2_t rval        = 1234;
    f3_t noise_level = 0;   // additive noise to input signal
    aware badapt_loss* preferred_loss = badapt_loss_l2_s;

    func badapt_supplier : preferred_loss;
    func badapt_supplier : get_in_size;
    func badapt_supplier : get_out_size;
    func badapt_supplier : fetch_sample_tio;
    func badapt_supplier : fetch_sample_vio;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** Type: Encoder
 *  Method: learning to add two unsigned integer variables
 */
self badapt_problem_binary_add_s = badapt_supplier
{
    aware_t _;
    sz_t bits  = 4; // input size = bits * 2; output size = bits + 1
    u2_t rval  = 1234;
    f3_t val_h =  0.9;
    f3_t val_l = -0.9;
    aware badapt_loss* preferred_loss = badapt_loss_l2_s;

    func badapt_supplier : preferred_loss;
    func badapt_supplier : get_in_size;
    func badapt_supplier : get_out_size;
    func badapt_supplier : fetch_sample_tio;
    func badapt_supplier : fetch_sample_vio;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** Type: Encoder
 *  Method: learning to multiply two unsigned integer variables
 */
self badapt_problem_binary_mul_s = badapt_supplier
{
    aware_t _;
    sz_t bits  = 4; // input size = bits * 2; output size = bits * 2
    u2_t rval  = 1234;
    f3_t val_h =  0.9;
    f3_t val_l = -0.9;
    aware badapt_loss* preferred_loss = badapt_loss_l2_s;

    func badapt_supplier : preferred_loss;
    func badapt_supplier : get_in_size;
    func badapt_supplier : get_out_size;
    func badapt_supplier : fetch_sample_tio;
    func badapt_supplier : fetch_sample_vio;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** Type: Encoder
 *  Method: forward randomization
 */
self badapt_problem_binary_xsg3_s = badapt_supplier
{
    aware_t _;
    sz_t bits  = 4; // input size = bits; output size = bits
    u2_t rval  = 1234;
    f3_t val_h =  0.9;
    f3_t val_l = -0.9;
    aware badapt_loss* preferred_loss = badapt_loss_l2_s;

    func badapt_supplier : preferred_loss;
    func badapt_supplier : get_in_size;
    func badapt_supplier : get_out_size;
    func badapt_supplier : fetch_sample_tio;
    func badapt_supplier : fetch_sample_vio;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** Type: Encoder
 *  Method: hashing
 */
self badapt_problem_binary_hash_s = badapt_supplier
{
    aware_t _;
    sz_t bits  = 4; // input size = bits; output size = bits
    u2_t rval  = 1234;
    f3_t val_h =  0.9;
    f3_t val_l = -0.9;
    bl_t reverse = false;
    aware badapt_loss* preferred_loss = badapt_loss_l2_s;

    func badapt_supplier : preferred_loss;
    func badapt_supplier : get_in_size;
    func badapt_supplier : get_out_size;
    func badapt_supplier : fetch_sample_tio;
    func badapt_supplier : fetch_sample_vio;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // BETH_PRECODE_SECTION

/**********************************************************************************************************************/

vd_t badapt_problem_signal_handler( const bcore_signal_s* o );

#endif  // BADAPT_PROBLEM_H

