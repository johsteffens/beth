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

/** Training a language model. Intended for recurrent networks.
 */

#ifndef BHPT_TUTOR_LANGUAGE_H
#define BHPT_TUTOR_LANGUAGE_H

/**********************************************************************************************************************/

#include "bmath_std.h"
#include "bhvm_std.h"
#include "bhpt_sketch.h"

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( bhpt_tutor_language, bcore_inst )
#ifdef XOILA_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** Generates automatic text from a trigger
 */
stamp :utf8_chatter = aware :
{
    bl_t cyclic_reset = false;
    st_s trigger    = "The chatter trigger:";
    sz_t size_line  = 100;
    sz_t size_lines = 4;
    bl_t newline_to_space = true;
    f3_t heat       = 0.2;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** UTF8 character tutor.
 *  Inspired by Andrey Karpathy's work on training language models with RNN:
 *  "The Unreasonable Effectiveness of Recurrent Neural Networks", May, 2015,
 *  http://karpathy.github.io/2015/05/21/rnn-effectiveness/
 */
stamp :utf8 = aware bhpt_tutor
{
    aware bhpt_builder => builder;
    aware bhpt_adaptor => adaptor;

    aware => src; // text source (bcore_file_path_s or st_s with inline code)

    hidden st_s => st; // buffered text

    /// updated after priming
    aware bcore_prsg => prsg_priming = bcore_prsg_lcg_u3_00_s;

    /// stays constant (-> same sequence for each test)
    aware bcore_prsg => prsg_testing = bcore_prsg_lcg_u3_01_s;

    /** Transient size: Number of characters fed after cyclic reset in order to obtain
     *  a representative cyclic network state. Used for cyclic networks.
     */

    sz_t size_trans =   20; // transient size
    sz_t size_prime =  200; // prime samples
    sz_t size_test  = 1000; // test samples

    f3_t tgt_pos = 1.0;
    f3_t tgt_neg = 0.0;

    :utf8_chatter_s => chatter;

    hidden bcore_mutex_s mutex;

    func bhpt_tutor : reset = { bcore_prsg_a_attach( &o->prsg_priming, bcore_prsg_t_create( TYPEOF_bcore_prsg_lcg_u3_00_s ) ); };
    func bhpt_tutor : create_adaptive;
    func bhpt_tutor : create_adaptor = { return bhpt_adaptor_a_clone( o->adaptor ); };
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

#endif // XOILA_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**********************************************************************************************************************/

vd_t bhpt_tutor_language_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BHPT_TUTOR_LANGUAGE_H
