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

/** Training a language model based on utf8-text. Intended for recurrent networks.
 *
 *  This tutor was inspired by Andrey Karpathy's work on training language models with RNN:
 *  "The Unreasonable Effectiveness of Recurrent Neural Networks", May, 2015,
 *  http://karpathy.github.io/2015/05/21/rnn-effectiveness/
 */

#ifndef BHPT_TUTOR_UTF8_H
#define BHPT_TUTOR_UTF8_H

/**********************************************************************************************************************/

#include "bmath_std.h"
#include "bhvm_std.h"
#include "bhpt_sketch.h"

/**********************************************************************************************************************/

#ifdef TYPEOF_bhpt_tutor_utf8

PLANT_GROUP( bhpt_tutor_utf8, bcore_inst )
#ifdef PLANT_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** Generates automatic text from a trigger
 */
stamp :chatter = aware :
{
    bl_t cyclic_reset = false;
    st_s trigger   = "The chatter trigger:";
    sz_t size_text = 400;
    sz_t size_line = 100;
    f3_t heat      = 0.2;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stamp : = aware bhpt_tutor
{
    aware => src; // source (bcore_file_path_s or st_s with inline code)

    hidden st_s => st; // buffered text

    /// rval_prime is updated after priming
    u2_t rval_prime = 1234;

    /// rval_test stays constant
    u2_t rval_test = 5342;

    /** Transient size: Number of characters fed after cyclic reset in order to obtain
     *  a representative cyclic network state. Used for cyclic networks.
     */

    sz_t size_trans =   20; // transient size
    sz_t size_prime =  200; // prime samples
    sz_t size_test  = 1000; // test samples

    f3_t tgt_pos = 1.0;
    f3_t tgt_neg = 0.0;

    :chatter_s => chatter;

    hidden bcore_mutex_s mutex;

    func bhpt_tutor : reset = { o->rval_prime = 1234; };
    func bhpt_tutor : create_adaptive;
    func bhpt_tutor : prime;
    func bhpt_tutor : test;
    func bhpt_tutor : status_to_sink =
    {
        if( verbosity > 0 )
        {
            bcore_sink_a_push_fa( sink, "#<sc_t>", ifnameof( o->_ ) );
        }
    };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // PLANT_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // TYPEOF_bhpt_tutor_utf8

/**********************************************************************************************************************/

vd_t bhpt_tutor_utf8_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BHPT_TUTOR_UTF8_H
