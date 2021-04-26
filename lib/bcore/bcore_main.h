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
#include "bcore_file.h"
#include "bcore_x_inst.h"

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( bcore_main, bcore_inst )
#ifdef XOILA_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stamp :frame_s = aware bcore_inst
{
    /// ==== Parameters ====

    /// The following criteria are processed in given order until one matches or all fail
    bl_t  first_argument_is_path_to_object = true; // path to object's config file is expected as first argument
    bl_t second_argument_is_path_to_script = true; // path to scipt file file is expected as first argument (only when feature 'main_parse' is implemented)
    sc_t local_file = "beth.config";  // config file in current folder
    sc_t global_file;                 // global path to config file

    /// ==== Internal data ====

    /// program arguments
    bcore_arr_st_s args;
    hidden bcore_mutex_s mutex;

    /// source for parsing or interactivity
    hidden aware bcore_source -> source;

    /// sink for general runtime output or interactivity
    hidden aware bcore_sink -> sink;

    /// object wrapped by this frame
    sr_s object_sr;

    func (er_t exec( m @* o, bcore_arr_st_s* args ));

    /// to be called from main function
    func (er_t main( mutable bcore_main_frame_s* o, sz_t argc, char** argv ));
};

/// If available, this feature is called by function frame_s_main after loading the object.
feature 'ar' er_t main( m @* o, m :frame_s* frame );

/** On exit is called when a exit is required (typically when SIGTERM was received)
 *  The implementation should return 'true' when the signal was handled and consumed.
 */
feature 'ar' bl_t on_termination( mutable @* o, :frame_s* frame ) = { return false; };

/** On interrupt is called on SIGINT (C-c).
 *  This is typically used to cleanly exit and close the program.
 *  The implementation should return 'true' when the signal was handled and consumed.
 */
feature 'ar' bl_t on_interrupt( mutable @* o, :frame_s* frame ) = { return false; };

/** On suspend is called on SIGTSTP (C-z).
 *  This is typically used to cleanly leave a long running function for interactive purposes.
 *  The implementation should return 'true' when the signal was handled and consumed.
 */
feature 'ar' bl_t on_suspend( mutable @* o, :frame_s* frame ) = { return false; };

stamp :arr_s = aware x_array
{
    aware : => [];
};

stamp :set_s = aware :
{
    :arr_s arr;

    private obliv :* current_object;
    bcore_mutex_s mutex_current_object;

    func :.main =
    {
        foreach( m $* e in o->arr )
        {
            {
                bcore_lock_s^ lock.set( o.mutex_current_object );
                o.current_object = e;
            }
            try( e.main( frame ) );
        };
        o.current_object = NULL;
        return 0;
    };

    func :.on_termination =
    {
        bcore_lock_s^ lock.set( o.mutex_current_object );
        return o.current_object ? o.current_object.on_termination( frame ) : false;
    };

    func :.on_interrupt =
    {
        bcore_lock_s^ lock.set( o.mutex_current_object );
        return o.current_object ? o.current_object.on_interrupt( frame ) : false;
    };

    func :.on_suspend =
    {
        bcore_lock_s^ lock.set( o.mutex_current_object );
        return o.current_object ? o.current_object.on_suspend( frame ) : false;
    };
};

//----------------------------------------------------------------------------------------------------------------------

#endif // XOILA_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**********************************************************************************************************************/

#endif  // BCORE_MAIN_H
