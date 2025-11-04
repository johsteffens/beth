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

/** Button with text label representing a link. The referenced object is displayed in a dedicated dialog window.
 *  The frame opens the dialog-window when the button is clicked in order to edit/create the underlying link of the parent's client.
 *  The client of the link frame is the linked object.
 *  The parents client is the object holding the link.
 *  The link maintains a strong reference to its client because the link-holder can delete the link, rendering the linked object invalid.
 */

//----------------------------------------------------------------------------------------------------------------------

forward :type_dialog_s;
stamp :s bgfe_frame
{
    /// parameters
    sz_t width;  // optional preferred width of button  (actual size is calculated from the text)
    sz_t height; // optional preferred height of button (actual size is calculated from the text)
    bl_t insensitive; // insensitive: does not react to user actions
    bl_t show_client_name = true; // client name on button
    bl_t show_glimpse     = true; // shows glimpse on button if available
    bl_t clearable        = true; // displays clear button
    f3_t x_align = 0.5; // gradual text alignment: 0: left, 0.5: center, 1.0 right
    f3_t y_align = 0.5; // gradual text alignment: 0: top, 0.5: center, 1.0 bottom
    tp_t window_position = upper_right; // window position relative to reference frame
    bl_t allow_custom_type = true;
    bcore_arr_tp_s => type_list; // list of types to choose from

    func bgfe_frame.set_show_client_name{ o.show_client_name = flag; = 0; }
    func bgfe_frame.set_show_glimpse{ o.show_glimpse = flag; = 0; }
    func bgfe_frame.set_width { o.width = value; = 0; }
    func bgfe_frame.set_height{ o.height = value; = 0; }
    func bgfe_frame.set_insensitive { o.insensitive = flag; = 0; }
    func bgfe_frame.set_x_align { o.x_align = f3_max( 0, f3_min( 1, value ) ); = 0; }
    func bgfe_frame.set_y_align { o.y_align = f3_max( 0, f3_min( 1, value ) ); = 0; }
    func bgfe_frame.set_window_position { o.window_position = name; = 0; }
    func bgfe_frame.set_allow_custom_type { o.allow_custom_type = flag; = 0; }
    func bgfe_frame.set_type_list { o.type_list =< list.clone(); = 0; }

    /// internals
    hidden sr_s client; // client (as strong smart reference)
    hidden tp_t client_name;  // name of the client (under which the link holder adresses the client); 0 in case the link holder is an array
    hidden sz_t client_index; // index of the client (under which the link holder (array) adresses the client);
    hidden bgfe_frame* parent;

    hidden bl_t link_changed; // link directly changed (applied in downsync)

    // link_holder: client that holds the link
    hidden bgfe_client* link_holder;
    hidden tp_t link_holder_type;
    hidden tp_t link_holder_name;

    hidden bl_t holder_is_array; // true in case the link is an element of an array

    func bgfe_frame.client = o.client.o;
    func bgfe_frame.client_type = o.client.type();
    func bgfe_frame.client_name = o.client_name;
    func bgfe_frame.parent = o.parent;
    func bgfe_frame.set_parent o.parent = parent;
    func bgfe_frame.is_open = o.is_open;
    func bgfe_frame.h_complexity = 3;
    func bgfe_frame.v_complexity = 1;
    func bgfe_frame.is_compact = true;
    func bgfe_frame.grab_focus;

    hidden bgfe_frame    => embedded_frame;
    hidden bgfe_window_s => window;

    hidden bgfe_frame* window_locator; // != NULL: window is placed next to this frame

    hidden :type_dialog_s => type_dialog;

    hidden bl_t rts_main_clicked; // main button was clicked
    hidden bl_t rts_main_pressed; // main button was pressed
    hidden bl_t rts_clear_clicked; // clear button was clicked

    hidden bl_t ignore_clicked_once; // internal flag to correctly handle fleeting windows

    hidden st_s button_main_text;  // button text
    hidden bgfe_rte_s* rte;
    hidden x_mutex_s mutex;
    hidden GtkWidget* rtt_widget;
    hidden GtkWidget* rtt_button_main;
    hidden GtkWidget* rtt_label;
    hidden GtkWidget* rtt_button_clear;
    hidden bl_t is_open;

    func bcore_inst_call.down_e o.close();

    func bgfe_frame.rtt_widget = o.rtt_widget;
    func bgfe_frame.open;
    func bgfe_frame.close;
    func bgfe_frame.client_close_request;
    func bgfe_frame.client_close_confirm;
    func bgfe_frame.client_distraction;

    /// interface functions ...
    func bgfe_frame.set_client_t;

    func er_t set_holder_t( m@* o, m bgfe_client* holder, tp_t holder_type, tp_t holder_name, bl_t holder_is_array, sz_t client_index );

    func bgfe_frame.cycle;
    func bgfe_frame.upsync;
    func bgfe_frame.downsync;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

embed "bgfe_frame_link.emb.x";
