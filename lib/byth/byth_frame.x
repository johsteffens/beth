/** Author and Copyright 2024 Johannes Bernhard Steffens
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

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

include "byth_base.h";
include "bcore_std.h";
include "bmath_std.h";

/** Runtime frame for embedded python code.
 *  The frame gets instantiated during the init1 cycle.
 *  Only one instance of byth_frame_s can exist.
 *  It can be used via function byth_frame_get()
 *  There are also directly routed convenience functions.
 */

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

identifier Py_Initialize;
identifier Py_FinalizeEx;
name byth_frame_const_key;

//----------------------------------------------------------------------------------------------------------------------

stamp :s
{
    x_mutex_s mutex;

    func bcore_inst_call.init_x
    {
        if( bcore_const_exists( byth_frame_const_key~ ) )
        {
            GERR_fa
            (
                "Attempt to instantiate byth_frame_s.\n"
                "byth_frame_s is only instantiated once during init1 cycle and stored in bcore_const_manager.\n"
                "Use function byth_frame_s* byth_frame_get() to obtain a reference.\n"
            );
        }
    }

    func o lock  ( @* o ) o.cast( m@* ).mutex.lock();
    func o unlock( @* o ) o.cast( m@* ).mutex.unlock();

    /// creates lifetime bounded lock/unlock
    func d x_lock_s*   create_lock  ( @* o ) = x_lock_s!  ( o.cast( m@* ).mutex );
    func d x_unlock_s* create_unlock( @* o ) = x_unlock_s!( o.cast( m@* ).mutex );
}

//----------------------------------------------------------------------------------------------------------------------

func :s* get   () = bcore_const_get_o( byth_frame_const_key~ ).cast( :s* );
func :s* lock  () = :get().lock();
func :s* unlock() = :get().unlock();

/// creates lifetime bounded lock/unlock
func d x_lock_s*   create_lock  ( @* o ) = :get().create_lock();
func d x_unlock_s* create_unlock( @* o ) = :get().create_unlock();

//----------------------------------------------------------------------------------------------------------------------

/// init1 cycle callback
func void group_signal_init1()
{
    Py_Initialize();
    bcore_const_a_set_d( byth_frame_const_key~, :s! );
}

//----------------------------------------------------------------------------------------------------------------------

/// down1 cycle callback
func void group_signal_down1()
{
    bcore_const_remove( byth_frame_const_key~ );
    Py_FinalizeEx();
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

