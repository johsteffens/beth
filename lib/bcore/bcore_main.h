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
 *    - call er_t bcore_main_s_main( bcore_main_frame_s* o, sz_t argc, sc_t* argv )
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
#include "bcore_x_array.h"

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( bcore_main, bcore_inst )
#ifdef XOILA_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stamp :frame_s bcore_inst
{
    /// ==== Parameters ====

    bl_t first_argument_is_path_to_object = true; // if a first argument is path to a file, it is assumed to be the object's config file
    bl_t next_argument_is_path_to_script  = true; // if next argument is path to a file, it is assumed to be a script file (first or second argument)

    bl_t create_log_file = false;    // creates/overwrites a log file defined as <script_path ? script_path : object_path>.<log_extension>
    st_s log_file_extension = "log"; // extension appended to <object_path> to create a log file

    st_s local_path = "beth.config"; // path to local object file (only if object was not yet determined)
    bl_t local_path_descend = false; // descents tree from current folder to locate current file

    st_s global_path;                // global path to object file (only if object was not yet determined)

    tp_t object_default_type = 0;    // (fallback) type of object (only if object was not yet determined)

    /// ==== Internal data ====

    /// program arguments
    bcore_arr_st_s args;
    hidden bcore_mutex_s mutex;

    /// source for script or interactivity
    hidden aware bcore_source -> source;

    /// sink for general runtime output or interactivity (default is stdout)
    hidden aware bcore_sink -> sink;

    /// sink for dedicated logging file (only when create_log_file == true)
    hidden aware bcore_sink -> log;

    /// object wrapped by this frame
    sr_s object_sr;

    /// file path of object
    st_s object_path;

    /// file path of script
    st_s script_path;

    func er_t exec( m @* o, bcore_arr_st_s* args );

    /// to be called from main function
    func er_t main( mutable bcore_main_frame_s* o, sz_t argc, m char** argv );
};

/// If available, this feature is called by function frame_s_main after loading the object.
feature 'ar' er_t main( m @* o, m :frame_s* frame );

/** On exit is called when a exit is required (typically when SIGTERM was received)
 *  The implementation should return 'true' when the signal was handled and consumed.
 */
feature 'ar' bl_t on_termination( mutable @* o, :frame_s* frame ) = false;

/** On interrupt is called on SIGINT (C-c).
 *  This is typically used to cleanly exit and close the program.
 *  The implementation should return 'true' when the signal was handled and consumed.
 */
feature 'ar' bl_t on_interrupt( mutable @* o, :frame_s* frame ) = false;

/** On suspend is called on SIGTSTP (C-z).
 *  This is typically used to cleanly leave a long running function for interactive purposes.
 *  The implementation should return 'true' when the signal was handled and consumed.
 */
feature 'ar' bl_t on_suspend( mutable @* o, :frame_s* frame ) = false;

stamp :arr_s = x_array { : => []; }

stamp :set_s
{
    :arr_s arr;
    func x_array_feature.c_get_wrapped_array = o.arr;
    func x_array_feature.m_get_wrapped_array = o.arr;

    private obliv :* current_object;
    bcore_mutex_s mutex_current_object;

    func :.main
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
        = 0;
    }

    func :.on_termination
    {
        bcore_lock_s^ lock.set( o.mutex_current_object );
        = o.current_object ? o.current_object.on_termination( frame ) : false;
    }

    func :.on_interrupt
    {
        bcore_lock_s^ lock.set( o.mutex_current_object );
        = o.current_object ? o.current_object.on_interrupt( frame ) : false;
    }

    func :.on_suspend
    {
        bcore_lock_s^ lock.set( o.mutex_current_object );
        = o.current_object ? o.current_object.on_suspend( frame ) : false;
    }
}

//----------------------------------------------------------------------------------------------------------------------

#endif // XOILA_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**********************************************************************************************************************/

#endif  // BCORE_MAIN_H
