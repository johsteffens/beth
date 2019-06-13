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

/** bcore_main is a main functional generalizing the main function of a program.
 *
 *  Usage (in function main):
 *    - Instantiate and setup bcore_main_frame_s
 *    - call int bcore_main_s_main( bcore_main_s* o, sz_t argc, sc_t* argv )
 *         return value is intended to be the return of the program.
 *
 *  bcore_main_frame_s_main does the following
 *    - loads an object from a config file implementing bcore_main : main.
 *    - calls the feature of this object
 *    - returns the result of that call
 *    - if launch criteria were not met, returns immediately with value -1;
 *
 */

#ifndef BCORE_MAIN_H
#define BCORE_MAIN_H

#include "bcore_precoder.h"
#include "bcore_trait.h"
#include "bcore_precoded.h"
#include "bcore_txt_ml.h"
#include "bcore_spect_interpreter.h"
#include "bcore_arr.h"

/**********************************************************************************************************************/

#ifdef TYPEOF_bcore_main
BETH_PRECODE( bcore_main )
#ifdef BETH_PRECODE_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    feature strict 'r' s2_t main( mutable, const bcore_arr_st_s* args );

    stamp :frame = aware bcore_inst
    {
        /// interpreter used to decode the config file
        aware bcore_interpreter => interpreter = bcore_txt_ml_interpreter_s;

        /// The following criteria are processed in given order until one matches or all fail
        bl_t use_first_argument = true;   // path to config file is expected as first argument
        sc_t local_file = "beth.config";  // config file in current folder
        sc_t global_file;                 // global path to config file
    };
#endif // BETH_PRECODE_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif // optional enclosure

s2_t bcore_main_frame_s_main( bcore_main_frame_s* o, sz_t argc, char** argv );

/**********************************************************************************************************************/

vd_t bcore_main_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif  // BCORE_MAIN_H
