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
    bgfe_frame => frame;

    /// internals
    hidden bgfe_client* client; // client
    hidden tp_t client_type; // to cover obliv clients
    hidden tp_t client_name; // name of the client (under which the client's owner holds the client)
    private GtkWidget* rtt_widget;
    hidden bgfe_rte_s* rte;
    hidden x_mutex_s mutex;
    hidden bl_t is_open;
    hidden bl_t rts_close_requested;

    func bcore_inst_call.down_e o.close();
    func bgfe_frame.rtt_widget = o.rtt_widget;

    /// setup compact function
    func o _( m@* o, sc_t title ) o.title!.push_sc( title );

    /// interface functions ...
    func er_t open ( m@* o );
    func er_t close( m@* o );

    func bgfe_frame.set_client_t;
    func bgfe_frame.cycle;
    func bgfe_frame.upsync;
    func bgfe_frame.downsync;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

embed "bgfe_window.emb.x";
