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

/** Runtime frame for embedded python code.
 *  The frame gets instantiated during the init1 cycle.
 *  Only one instance of byth_frame_s can exist.
 *  It can be obtained and used via function byth_frame_get()
 *  There are also directly routed convenience functions.
 */

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

name byth_frame_global_key;

//----------------------------------------------------------------------------------------------------------------------

/** Global frame.
 *  Sets up the global environment of the Python/C API.
 *  Only one instance exists as element in beth_global_mananger.
 *  You can obtain a reference via function byth_frame_get()
 */
stamp :s
{
    bl_t is_initialized;
    x_mutex_s mutex;

    func o initialize( m@* o );
    func o finalize  ( m@* o );
    func bcore_inst_call.init_x;
    func bcore_inst_call.down_e;

    func o lock  ( m@* o ) o.cast( m@* ).mutex.lock();
    func o unlock( m@* o ) o.cast( m@* ).mutex.unlock();

    /// creates lifetime bounded lock/unlock
    func d x_lock_s*   create_lock  ( m@* o ) = x_lock_s!  ( o.cast( m@* ).mutex );
    func d x_unlock_s* create_unlock( m@* o ) = x_unlock_s!( o.cast( m@* ).mutex );

    /** Runs a python program with optional input and output
     *  byth_in, byth_out are input and output data:
     *      Outside python they are selfaware stamps.
     *      Inside python they are dictionaries, with stamp members accessible via the associated member names.
     *  byth_in, byth_out can be NULL when not needed.
     */
    func er_t run( m@* o, sc_t program, c aware x_inst* byth_in, m aware x_inst* byth_out );


    /** Runs a python program with optional input and output via hashmaps
     *  'direct_in' is directly converted into a dictionary.
     *   The elements of direct_in are accessible as local variables inside the python program.
     *   If after execution the python program generates a disctionary 'byth_out', it is converted into
     *   the hashmap byth_out.
     */
    func er_t run_hmap( m@* o, sc_t program, c bcore_hmap_tp_sr_s* direct_in, m bcore_hmap_tp_sr_s* byth_out );
}

//----------------------------------------------------------------------------------------------------------------------

func m :s* get()
{
    m :s* frame = bcore_global_get_o( byth_frame_global_key~ ).cast( m :s* );
    frame.initialize();
    = frame;
}

func m :s* lock  () = :get().lock();
func m :s* unlock() = :get().unlock();

/// creates lifetime bounded lock/unlock
func d x_lock_s*   create_lock  ( @* o ) = :get().create_lock();
func d x_unlock_s* create_unlock( @* o ) = :get().create_unlock();

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
