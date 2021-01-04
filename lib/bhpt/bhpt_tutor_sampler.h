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

XOILA_DEFINE_GROUP( bhpt_sampler, bcore_inst )
#ifdef XOILA_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

feature strict void fetch( c @* o, m bcore_prsg* prsg, m bhvm_value_s* x, m bhvm_value_s* y );
feature strict sz_t get_size_en( c @* o );
feature strict sz_t get_size_ex( c @* o );

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** Type: Classifier
 *  Problem: Distinguish between a sine wave of arbitrary amplitude and frequency from a random walk curve.
 */
stamp :sine_random_s = aware :
{
    sz_t size_en = 32;
    f3_t pos_tgt =  0.9;
    f3_t neg_tgt = -0.9;

    func : . fetch;
    func : . get_size_en = { return o.size_en; };
    func : . get_size_ex = { return 1; };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** Type: Encoder
 *  Problem: Add two unsigned integer variables.
 */
stamp :binary_add_s = aware :
{
    sz_t bits  = 4; // input size = bits * 2; output size = bits + 1
    f3_t val_h =  0.9;
    f3_t val_l = -0.9;

    func : . fetch;
    func : . get_size_en = { return o.bits * 2; };
    func : . get_size_ex = { return o.bits + 1; };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** Type: Encoder
 *  Problem: Multiply two unsigned integer variables.
 */
stamp :binary_mul_s = aware :
{
    sz_t bits  = 4; // input size = bits * 2; output size = bits * 2
    f3_t val_h =  0.9;
    f3_t val_l = -0.9;
    func : . fetch;
    func : . get_size_en = { return o.bits * 2; };
    func : . get_size_ex = { return o.bits * 2; };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** Type: Encoder
 *  Problem: Forward randomization.
 */
stamp :binary_lcg00_s = aware :
{
    sz_t bits  = 4; // input size = bits; output size = bits
    f3_t val_h =  0.9;
    f3_t val_l = -0.9;
    func : . fetch;
    func : . get_size_en = { return o.bits; };
    func : . get_size_ex = { return o.bits; };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** Type: Encoder
 *  Problem: (Reverse) Hashing.
 */
stamp :binary_hash_s = aware :
{
    sz_t bits  = 4; // input size = bits; output size = bits
    f3_t val_h =  0.9;
    f3_t val_l = -0.9;
    bl_t reverse = false;
    func : . fetch;
    func : . get_size_en = { return o.bits; };
    func : . get_size_ex = { return o.bits; };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** Type: Regression
 *  Problem: Estimate polynomial coefficients for a signal within range -1, 1.
 */
stamp :polynom_s = aware :
{
    sz_t size_en = 32;
    sz_t size_ex = 3;   // polynomial order + 1
    f3_t range   = 1.0; // +/- range of coefficients
    f3_t noise_level = 0;   // additive noise to input signal
    func : . fetch;
    func : . get_size_en = { return o.size_en; };
    func : . get_size_ex = { return o.size_ex; };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // XOILA_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // TYPEOF_bhpt_sampler


#ifdef TYPEOF_bhpt_tutor_sampler

XOILA_DEFINE_GROUP( bhpt_tutor_sampler, bcore_inst )
#ifdef XOILA_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stamp :s = aware bhpt_tutor
{
    aware bhpt_builder => builder;
    aware bhpt_adaptor => adaptor;
    aware bhpt_sampler => sampler;

    /// updated after priming
    aware bcore_prsg => prsg_priming = bcore_prsg_lcg_u3_00_s;

    /// stays constant (-> same sequence for each test)
    aware bcore_prsg => prsg_testing = bcore_prsg_lcg_u3_01_s;

    sz_t test_size = 1000;

    hidden bcore_mutex_s mutex;

    func bhpt_tutor . reset = {};
    func bhpt_tutor . create_adaptive;
    func bhpt_tutor . create_adaptor = { return o.adaptor.clone(); };
    func bhpt_tutor . prime;
    func bhpt_tutor . test;
    func bhpt_tutor . status_to_sink =
    {
        if( verbosity > 0 )
        {
            bcore_sink_a_push_fa( sink, "#<sc_t> : #<sc_t>", ifnameof( o._ ), ifnameof( o.sampler ? o.sampler._ : 0 ) );
        }
    };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // XOILA_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**********************************************************************************************************************/

vd_t bhpt_tutor_sampler_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BHPT_TUTOR_SAMPLER_H
