/** Author and Copyright 2019 Johannes Bernhard Steffens
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

/** bcore_main is a functional generalizing the main function of a program.
 *
 *  In function int main( int argc, char** argv ):
 *    - Instantiate and setup bcore_main_frame_s
 *    - call int bcore_main_s_main( bcore_main_s* o, sz_t argc, sc_t* argv )
 *         return value is intended to be the return of the program.

 *  In a suitable object:
 *    - overload feature bcore_main : main
 *    - access argument list via frame->args
 *    - poll for termination via bcore_main_frame_s_exit_required( frame )
 *
 *  bcore_main_frame_s_main does the following
 *    - loads an object from a config file implementing bcore_main : main.
 *    - redirects signals SIGINT and SIGTERM; occurrence can be polled via function 'exit_required'
 *    - calls the feature of this object
 *    - returns the result of that call
 *    - if launch criteria were not met, returns immediately with value TYPEOF_general_error and a message of the error stack;
 *
 */

#ifndef BCORE_MAIN_H
#define BCORE_MAIN_H

#include "bcore_xoila.h"
#include "bcore_trait.h"
#include "bcore_spect_inst.h"
#include "bcore.xo.h"
#include "bcore_txt_ml.h"
#include "bcore_spect_interpreter.h"
#include "bcore_spect_array.h"
#include "bcore_arr.h"

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( bcore_main, bcore_inst )
#ifdef XOILA_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// This function should be polled to determine if running routine is being required to exit.
signature bl_t exit_required( c @* o );

stamp :frame_s = aware bcore_inst
{
    /// interpreter used to decode the config file; If NULL, frame tries to guess the correct interpreter from the file's content
    aware bcore_interpreter => interpreter;

    /// program arguments
    bcore_arr_st_s args;

    hidden bcore_mutex_s mutex;

    /// The following criteria are processed in given order until one matches or all fail
    bl_t first_argument_is_path_to_config = true;   // path to config file is expected as first argument
    sc_t local_file = "beth.config";  // config file in current folder
    sc_t global_file;                 // global path to config file

    /// This implementation allows 'o' to be NULL in which case it returns always false.
    func :.exit_required;

    func (er_t exec( m @* o, bcore_arr_st_s* args ));

    /// to be called from main function
    func (er_t main( mutable bcore_main_frame_s* o, sz_t argc, char** argv ));
};

feature strict 'ar' er_t main( m @* o, m :frame_s* frame );

stamp :arr_s = aware x_array
{
    aware : => [];
};

stamp :set_s = aware :
{
    :arr_s arr;
    func : . main =
    {
        foreach( m $* e in o->arr ) try( e.main( frame ) );
        return 0;
    };
};

//----------------------------------------------------------------------------------------------------------------------

#endif // XOILA_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**********************************************************************************************************************/

#endif  // BCORE_MAIN_H
