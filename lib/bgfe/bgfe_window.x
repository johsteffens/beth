/** Author and Copyright 2025 Johannes Bernhard Steffens
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

/** Top level gfe-window.
 *
 *  Usage:
 *  - instantiate bgfe_window_s; number of instances is unlimited; each instance represents a different window
 *  - (optional) set parameters
 *  - use interface functions at will
 *
 */

//----------------------------------------------------------------------------------------------------------------------

stamp :s bgfe_frame
{
    /// parameters
    st_s => title;
    bl_t keep_above = false; // keeps window above the nearest to-root window
    st_s => widget_name;   // optional gtk widget name overrides default widget name

    func bgfe_frame.set_text       { o.title!.copy_sc( text ); = 0; }
    func bgfe_frame.set_keep_above { o.keep_above = flag; = 0; }
    func bgfe_frame.set_widget_name{ o.widget_name!.copy_sc( text ); = 0; }

    bgfe_frame => frame;

    /// internals
    func bgfe_frame.client      = o.frame ? o.frame.client() : 0;
    func bgfe_frame.client_type = o.frame ? o.frame.client_type() : 0;
    func bgfe_frame.client_name = o.frame ? o.frame.client_name() : 0;
    func bgfe_frame.parent      = o.frame ? o.frame.parent() : 0;
    func bgfe_frame.h_complexity = o.frame ? o.frame.h_complexity() : 1;
    func bgfe_frame.v_complexity = o.frame ? o.frame.v_complexity() : 1;

    private GtkWidget* rtt_widget;
    hidden bgfe_rte_s* rte;
    hidden x_mutex_s mutex;
    hidden bl_t is_open;
    hidden bl_t rts_close_requested;

    func bcore_inst_call.down_e o.close();

    /// setup compact function
    func o _( m@* o, sc_t title ) o.title!.push_sc( title );

    func bgfe_frame.rtt_widget           = o.rtt_widget;
    func bgfe_frame.client_close_ok      = o.frame ? o.frame.client_close_ok() : true;
    func bgfe_frame.client_close_confirm = o.frame ? o.frame.client_close_confirm() : 0;
    func bgfe_frame.arrangement          = o.frame ? o.frame.arrangement() : 0;

    func bgfe_notify_sender.supports_notify = o.frame ? o.frame.cast( m bgfe_notify_sender* ).supports_notify() : false;
    func bgfe_notify_sender.ligand_pool  = o.frame ? o.frame.cast( m bgfe_notify_sender* ).ligand_pool() : NULL;

    /// interface functions ...
    func bgfe_frame.open;
    func bgfe_frame.close;
    func bgfe_frame.cycle;
    func bgfe_frame.upsync   = o.frame ? o.frame.upsync() : 0;
    func bgfe_frame.downsync = o.frame ? o.frame.downsync() : 0;

    /// cycle with additional sleep
    func er_t cycle_sleep_ms( m@* o, tp_t action_type, sz_t ms )
    {
        o.cycle( action_type );
        x_threads_sleep_ms( ms );
        = 0;
    }

    /// cycle loop while window is open
    func er_t loop_while_open_sleep_ms( m@* o, tp_t action_type, sz_t ms )
    {
        while( o.is_open ) o.cycle_sleep_ms( action_type, ms );
        = 0;
    }

    /// presents window (e.g. unminimize if it was minimized)
    func er_t present( m@* o );

    func er_t set_frame( m@* o, m bgfe_frame* frame );

    func er_t set_frame_from_client_t( m@* o, m obliv bgfe_client* client, tp_t client_type, tp_t client_name );
    func er_t set_frame_from_client( m@* o, m aware bgfe_client* client, tp_t client_name ) = o.set_frame_from_client_t( client, client ? client._ : 0, client_name );

    func bgfe_frame.set_client_t;
    func bgfe_frame.set_client = o.set_client_t( client, client ? client._ : 0, client_name );

    func bgfe_frame.set_client_with_content_t;
    func bgfe_frame.set_client_with_content = o.set_client_with_content_t( client, client ? client._ : 0, client_name );

    func bgfe_frame.add_content_t;
    func bgfe_frame.add_content = o.add_content_t( content, content ? content._ : 0, content_name );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

embed "bgfe_window.emb.x";
