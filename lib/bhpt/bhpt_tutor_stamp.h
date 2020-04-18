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

 /// Collection of bhpt_tutor stamps

#ifndef BHPT_TUTOR_STAMP_H
#define BHPT_TUTOR_STAMP_H

/**********************************************************************************************************************/

#include "bmath_std.h"
#include "bhvm_std.h"
#include "bhpt_sketch.h"

/**********************************************************************************************************************/

#ifdef TYPEOF_bhpt_tutor_stamp

PLANT_GROUP( bhpt_tutor_stamp, bhpt_tutor )
#ifdef PLANT_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** Type: Classifier
 *  Method: Differentiate between a sine wave of arbitrary amplitude and frequency from a random walk curve.
 */
stamp bhpt_tutor_sine_random = aware bhpt_tutor
{
    sz_t input_size = 32;
    u2_t rval       = 1234;
    f3_t pos_tgt    =  0.9;
    f3_t neg_tgt    = -0.9;

    hidden bcore_mutex_s => mutex;
    func bcore_inst_call : init_x = { o->mutex = bcore_mutex_s_create(); };

    func bhpt_tutor : reset;
    func bhpt_tutor : create_adaptive;
    func bhpt_tutor : prime;
    func bhpt_tutor : test;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//stamp bhpt_tutor_test = bcore_inst
//{
//    bcore_mutex_s mutex;
//
////    func bcore_inst_call : init_x = { o->mutex = bcore_mutex_s_create(); };
////
////    func bhpt_tutor : reset;
////    func bhpt_tutor : create_adaptive;
////    func bhpt_tutor : prime;
////    func bhpt_tutor : test;
//};


#endif // PLANT_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // TYPEOF_bhpt_tutor_stamp

/**********************************************************************************************************************/

vd_t bhpt_tutor_stamp_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BHPT_TUTOR_STAMP_H
