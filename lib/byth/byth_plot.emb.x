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

    bl_t exit_thread_request_flag;
    bl_t thread_was_opened;
    bl_t thread_was_closed;

    private :frame_arr_s* frame_arr;
    private :appearance_s* appearance;

    func bl_t exit_thread_request( m@* o )
    {
        o.mutex.lock();
        bl_t flag = o.exit_thread_request_flag;
        o.mutex.unlock();
        = flag;
    }

    func bcore_inst_call.down_e o.close_thread();

    func o close_thread( m@* o )
    {
        o.mutex.lock();
        o.exit_thread_request_flag = true;
        o.mutex.unlock();
        o.thread.join();
        o.exit_thread_request_flag = false;
    }

    func x_thread.m_thread_func
    {
        m$* rte = byth_rte_s!^;
        rte.run_program( byth_plot_functions_py_as_string()^.sc );

        bl_t refresh = false;
        bl_t open_window = true;
        bl_t window_is_open = false;
        bl_t keep_window_alive = true;

        rte.set_global_u3( "window_was_closed", 0 );

        while( !o.exit_thread_request() || ( keep_window_alive && window_is_open ) )
        {
            o.mutex.lock();
            if( o.frame_arr && o.appearance )
            {
                m$* appearance = o.appearance;
                m$* frame_arr  = o.frame_arr;
                rte.set_local_inst( "frame_arr", frame_arr );
                rte.set_local_inst( "appearance", appearance );
                refresh = true;
                if( rte.get_global_u3( "window_was_closed" ) )
                {
                    open_window = appearance.reopen_window;
                    rte.set_global_u3( "window_was_closed", 0 );
                }
                keep_window_alive = appearance.keep_window_alive;
                o.frame_arr = NULL;
                o.appearance = NULL;
            }
            o.mutex.unlock();

            if( open_window )
            {
                rte.run_statement( "plt = matplotlib.pyplot" );
                rte.run_statement( "fig = plt.figure()" );
                rte.run_statement( "setup_plot( plt, fig )" );
                open_window = false;
                window_is_open = true;
            }

            if( !rte.get_global_u3( "window_was_closed" ) )
            {
                if( refresh )
                {
                    rte.run_statement( "update_appearance( plt, fig, appearance )" );
                    rte.run_statement( "update_plot( plt, fig, frame_arr, appearance )" );
                    refresh = false;
                }

                rte.run_statement( "plt.pause(0.05)" );
            }
            else
            {
                window_is_open = false;
                x_threads_sleep_ms( 100 );
            }
        }

        rte.run_statement( "plt.close()" );

        o.mutex.lock();
        o.thread_was_closed = true;
        o.mutex.unlock();
        = NULL;
    }

    // shows plots in a dedicated window; appearance can be NULL
    func o show_frame_arr( m@* o, const :frame_arr_s* frame_arr, const :appearance_s* appearance )
    {
        {
            o.mutex.create_lock()^;
            if( o.thread_was_closed ) = o;
            o.frame_arr = frame_arr.cast( m$* );
            if( appearance )
            {
                o.appearance = appearance.cast( m$* );
            }
            else
            {
                o.appearance = :appearance_s!^^;
            }
        }

        if( !o.thread_was_opened )
        {
            o.thread.call_m_thread_func( o );
            o.thread_was_opened = true;
        }

        bl_t exit_loop = false;
        while( !exit_loop )
        {
            o.mutex.lock();
            exit_loop = ( o.frame_arr == NULL );
            o.mutex.unlock();
            x_threads_sleep_ms( 10 );
        }

        = o;
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

