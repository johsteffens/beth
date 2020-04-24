/** Author and Copyright 2020 Johannes Bernhard Steffens
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

 /** Special class of supervising tutors based on a sampling function
  *  generating (random) samples of training and testing pairs.
  */

#ifndef BHPT_TUTOR_SAMPLER_H
#define BHPT_TUTOR_SAMPLER_H

/**********************************************************************************************************************/

#include "bmath_std.h"
#include "bhvm_std.h"
#include "bhpt_sketch.h"

/**********************************************************************************************************************/

#ifdef TYPEOF_bhpt_sampler

PLANT_GROUP( bhpt_sampler, bcore_inst )
#ifdef PLANT_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// returns modified random variable
feature strict 'a' u2_t fetch( const, u2_t rval, bhvm_value_s* x, bhvm_value_s* y );
feature strict 'a' sz_t get_size_en( const );
feature strict 'a' sz_t get_size_ex( const );

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** Type: Classifier
 *  Problem: Distinguish between a sine wave of arbitrary amplitude and frequency from a random walk curve.
 */
stamp :sine_random = aware :
{
    sz_t size_en = 32;
    f3_t pos_tgt =  0.9;
    f3_t neg_tgt = -0.9;

    func : : fetch;
    func : : get_size_en = { return o->size_en; };
    func : : get_size_ex = { return 1; };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** Type: Encoder
 *  Problem: Add two unsigned integer variables.
 */
stamp :binary_add = aware :
{
    sz_t bits  = 4; // input size = bits * 2; output size = bits + 1
    f3_t val_h =  0.9;
    f3_t val_l = -0.9;

    func : : fetch;
    func : : get_size_en = { return o->bits * 2; };
    func : : get_size_ex = { return o->bits + 1; };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** Type: Encoder
 *  Problem: Multiply two unsigned integer variables.
 */
stamp :binary_mul = aware :
{
    sz_t bits  = 4; // input size = bits * 2; output size = bits * 2
    f3_t val_h =  0.9;
    f3_t val_l = -0.9;
    func : : fetch;
    func : : get_size_en = { return o->bits * 2; };
    func : : get_size_ex = { return o->bits * 2; };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** Type: Encoder
 *  Problem: Forward randomization.
 */
stamp :binary_xsg3 = aware :
{
    sz_t bits  = 4; // input size = bits; output size = bits
    f3_t val_h =  0.9;
    f3_t val_l = -0.9;
    func : : fetch;
    func : : get_size_en = { return o->bits; };
    func : : get_size_ex = { return o->bits; };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** Type: Encoder
 *  Problem: (Reverse) Hashing.
 */
stamp :binary_hash = aware :
{
    sz_t bits  = 4; // input size = bits; output size = bits
    f3_t val_h =  0.9;
    f3_t val_l = -0.9;
    bl_t reverse = false;
    func : : fetch;
    func : : get_size_en = { return o->bits; };
    func : : get_size_ex = { return o->bits; };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** Type: Regression
 *  Problem: Estimate polynomial coefficients for a signal within range -1, 1.
 */
stamp :polynom = aware :
{
    sz_t size_en = 32;
    sz_t size_ex = 3;   // polynomial order + 1
    f3_t range   = 1.0; // +/- range of coefficients
    f3_t noise_level = 0;   // additive noise to input signal
    func : : fetch;
    func : : get_size_en = { return o->size_en; };
    func : : get_size_ex = { return o->size_ex; };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // PLANT_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // TYPEOF_bhpt_sampler


#ifdef TYPEOF_bhpt_tutor_sampler

PLANT_GROUP( bhpt_tutor_sampler, bcore_inst )
#ifdef PLANT_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** Type: Classifier
 *  Method: Differentiate between a sine wave of arbitrary amplitude and frequency from a random walk curve.
 */
stamp : = aware bhpt_tutor
{
    aware bhpt_sampler => sampler;

    /// rval_prime is updated after priming
    u2_t rval_prime = 1234;

    /// rval_test stays constant
    u2_t rval_test = 5342;

    sz_t test_size = 1000;

    hidden bcore_mutex_s mutex;

    func bhpt_tutor : reset = {};
    func bhpt_tutor : create_adaptive;
    func bhpt_tutor : prime;
    func bhpt_tutor : test;
    func bhpt_tutor : status_to_sink =
    {
        if( verbosity > 0 )
        {
            bcore_sink_a_push_fa( sink, "#<sc_t> : #<sc_t>", ifnameof( o->_ ), ifnameof( o->sampler ? o->sampler->_ : 0 ) );
        }
    };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // PLANT_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // TYPEOF_bhpt_tutor_sampler

/**********************************************************************************************************************/

vd_t bhpt_tutor_sampler_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BHPT_TUTOR_SAMPLER_H
