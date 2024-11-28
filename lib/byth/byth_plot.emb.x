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

/** Thread safe runtime plot engine.
 *  The should only exist one global instance.
 *  The plot manager maintain a plot window which is opened upon a plot request or
 *  is updated in case it is already open.
 *  The window can be manually closed. Otherwise it stays open until the program ends.
 */
stamp :engine_s x_thread
{
    x_mutex_s mutex;
    x_thread_s thread;

    bl_t exit_thread_flag;
    bl_t thread_is_running;

    private :frame_arr_s* frame_arr;
    private :appearance_s* appearance;

    func bcore_inst_call.down_e o.close_thread();

    func o close_thread( m@* o )
    {
        o.mutex.lock();
        o.exit_thread_flag = true;
        o.mutex.unlock();
        o.thread.join();
        o.exit_thread_flag = false;
        o.thread_is_running = false;
    }

    func bl_t exit_thread( m@* o )
    {
        o.mutex.lock();
        bl_t flag = o.exit_thread_flag;
        o.mutex.unlock();
        = flag;
    }

    func x_thread.m_thread_func
    {
        m$* rte = byth_rte_s!^;
        rte.run_program( byth_plot_functions_py_as_string()^.sc );

        rte.run_statement( "plt = matplotlib.pyplot" );
        rte.run_statement( "fig = plt.figure()" );
        rte.set_global_u3( "keep_looping", 1 );

        rte.run_statement( "setup_plot( plt, fig )" );

        bl_t refresh = false;
        bl_t exit = false;

        // variable keep_looping is set 0 when the window is closed
        while( rte.get_global_u3( "keep_looping" ) && !exit )
        {
            o.mutex.lock();
            exit = o.exit_thread_flag;
            if( o.frame_arr && o.appearance )
            {
                m$* appearance = o.appearance;
                m$* frame_arr  = o.frame_arr;
                rte.set_local_inst( "frame_arr", frame_arr );
                rte.set_local_inst( "appearance", appearance );
                o.frame_arr = NULL;
                o.appearance = NULL;
                refresh = true;
            }
            o.mutex.unlock();

            if( refresh )
            {

                rte.run_statement( "update_appearance( plt, fig, appearance )" );
                rte.run_statement( "update_plot( plt, fig, frame_arr, appearance )" );
                refresh = false;
            }

            rte.run_statement( "plt.pause(0.1)" );
        }

        rte.run_statement( "plt.close()" );

        o.mutex.lock();
        o.thread_is_running = false;
        o.mutex.unlock();
        = NULL;
    }

    // shows plots in a dedicated window; appearance can be NULL
    func o show_frame_arr( m@* o, const :frame_arr_s* frame_arr, const :appearance_s* appearance )
    {
        o.mutex.lock();
        o.frame_arr = frame_arr.cast( m$* );
        if( appearance )
        {
            o.appearance = appearance.cast( m$* );
        }
        else
        {
            o.appearance = :appearance_s!^^;
        }
        o.mutex.unlock();

        if( !o.thread_is_running )
        {
            o.thread.call_m_thread_func( o );
            o.thread_is_running = true;
        }

        bl_t exit_loop = false;
        while( !exit_loop )
        {
            o.mutex.lock();
            exit_loop = ( o.frame_arr == NULL );
            o.mutex.unlock();
            x_threads_sleep_ms( 10 );
        }
    }

    // shows plots in a dedicated window; appearance can be NULL
    func o show_frame( m@* o, const :frame_s* frame, const :appearance_s* appearance )
    {
        m$* frame_arr = :frame_arr_s!^;
        frame_arr.push_d( frame.cast( m$* ).fork() );
        = o.show_frame_arr( frame_arr, appearance );
    }
}

//----------------------------------------------------------------------------------------------------------------------

name byth_plot_engine_s_global; // global keyword to handle one instance
func m :engine_s* get_global_engine()
{
    = bcore_global_get_ifnexists_set( byth_plot_engine_s_global~, byth_plot_engine_s~ ).cast( m byth_plot_engine_s* );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) show
{
    byth_plot_get_global_engine().show_frame( o, appearance );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_arr_s) show
{
    byth_plot_get_global_engine().show_frame_arr( o, appearance );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

