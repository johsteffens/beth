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

/** Frame for displaying/editing arrays.
 */

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

forward :s;
forward bgfe_choice_s;
forward bgfe_choice_item_s;
forward bgfe_choice_client;

/// frame for an array element
stamp :item_s bgfe_frame
{
    /// parameters
    sz_t width;  // optional preset width
    sz_t height; // optional preset height
    sz_t index = -1;  // array index (call setup to set index)
    bl_t has_select_button;
    bl_t show_index = true;

    func bgfe_frame.set_width   { o.width   = value; = 0; }
    func bgfe_frame.set_height  { o.height  = value; = 0; }

    /// selected status
    hidden bl_t selected;

    /// sets selected status; updates display
    func er_t set_selected( m@* o, bl_t flag );

    hidden bgfe_rte_s* rte;
    hidden x_mutex_s mutex;
    hidden GtkWidget* rtt_widget;
    hidden GtkWidget* rtt_index_label;
    hidden GtkWidget* rtt_select_button;
    hidden :s* parent;
    hidden bgfe_frame => client_frame;
    hidden bl_t is_vertical;
    hidden bl_t is_open;
    hidden bl_t rts_selected;
    hidden bl_t rtt_ignore_select_button_toggled;

    func bgfe_frame.parent = o.parent;
    func bgfe_frame.rtt_widget = o.rtt_widget;
    func bgfe_frame.open;
    func bgfe_frame.close;
    func bgfe_frame.arrangement = o.is_vertical ? vertical~ : horizontal~;
    func bgfe_frame.client_close_request;
    func bgfe_frame.client_close_confirm;
    func bgfe_frame.client_distraction;

    /// Interface functions ...

    func er_t setup( m@* o, m :s* parent, sz_t index );
    func bgfe_frame.cycle;
    func bgfe_frame.downsync;
    func bgfe_frame.upsync;
}

//----------------------------------------------------------------------------------------------------------------------

stamp :item_arr_s x_array { :item_s => []; }

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp :s bgfe_frame
{
    /// parameters
    sz_t width;   // optional preset width
    sz_t height;  // optional preset height
    bl_t insensitive; // insensitive: does not react to user actions
    tp_t arrange = vertical; // vertical | horizontal // avoid 0 == auto
    bl_t arr_editable = true;  // indicates if array itself is editable (adding, removing, reordering); does not apply to editability of array elements
    bl_t show_border = true;
    bl_t show_index = true;
    sz_t min_content_width  = 0; // applies for horizontal orientation
    sz_t min_content_height = 0; // applies for vertical orientation
    sz_t max_content_width  = 480; // applies for horizontal orientation
    sz_t max_content_height = 480; // applies for vertical orientation
    bl_t show_client_name = true;
    st_s => title;       // optional title (overrides client name)
    st_s => widget_name;   // optional gtk widget name overrides default widget name
    sz_t spacing;   // spacing between elements
    bl_t end_bound; // packs elements with reference to the end of the box (false: reference to the start)
    bl_t center = true;    // centers widgets in an expanded space
    bl_t stretch = true;   // stretches elements to fill expanded space
    sz_t nesting_level;    // nesting level (embedded nesting)
    tp_t window_policy = one; // window-policy: any~ | one~ | none~;
    bl_t scrollable = true;

    func bgfe_frame.set_width   { o.width   = value; = 0; }
    func bgfe_frame.set_height  { o.height  = value; = 0; }
    func bgfe_frame.set_insensitive { o.insensitive = flag; = 0; }
    func bgfe_frame.set_arrange { o.arrange = name; = 0; }
    func bgfe_frame.set_arr_editable { o.arr_editable  = flag; = 0; }
    func bgfe_frame.set_show_border { o.show_border = flag; = 0; }
    func bgfe_frame.set_show_index  { o.show_index = flag; = 0; }
    func bgfe_frame.set_min_content_width { o.min_content_width  = value; = 0; }
    func bgfe_frame.set_min_content_height{ o.min_content_height = value; = 0; }
    func bgfe_frame.set_max_content_width { o.max_content_width  = value; = 0; }
    func bgfe_frame.set_max_content_height{ o.max_content_height = value; = 0; }
    func bgfe_frame.set_show_client_name { o.show_client_name = flag; = 0; }
    func bgfe_frame.set_title       { o.title!.copy_sc( text ); = 0; }
    func bgfe_frame.set_widget_name { o.widget_name!.copy_sc( text ); = 0; }
    func bgfe_frame.set_spacing  { o.spacing   = value; = 0; }
    func bgfe_frame.set_end_bound{ o.end_bound = flag; = 0; }
    func bgfe_frame.set_center   { o.center    = flag; = 0; }
    func bgfe_frame.set_stretch  { o.stretch   = flag; = 0; }
    func bgfe_frame.set_nesting_level { o.nesting_level = value; = 0; }
    func bgfe_frame.set_window_policy { o.window_policy = name; = 0; }
    func bgfe_frame.set_scrollable    { o.scrollable = flag; = 0; }

    /// internals
    hidden bgfe_client* client; // client
    hidden tp_t client_type; // to cover obliv clients
    hidden tp_t client_name; // name under which the client's owner holds the client
    hidden bgfe_frame* parent;
    hidden vd_t array_base_address; // address of first array element used to determine if the array has been relocated between cycles
    hidden bl_t is_vertical; // array elements are ordered verically (vs. horizontally)
    hidden bcore_arr_sr_s => copied_elements; // for copy, cut, paste
    hidden bgfe_choice_s => menu;
    hidden bgfe_frame_s => menu_frame;

    func bgfe_frame.h_complexity = o.min_content_width  / bgfe_frame_complexity_unit_size();
    func bgfe_frame.v_complexity = o.min_content_height / bgfe_frame_complexity_unit_size();
    func bgfe_frame.client = o.client;
    func bgfe_frame.client_type = o.client_type;
    func bgfe_frame.client_name = o.client_name;
    func bgfe_frame.parent = o.parent;
    func bgfe_frame.is_open = o.is_open;
    func bgfe_frame.is_compact = false;

    hidden :item_arr_s => item_arr;

    hidden bgfe_rte_s* rte;
    hidden x_mutex_s mutex;
    hidden GtkWidget* rtt_widget;
    hidden GtkWidget* rtt_main_box;
    hidden GtkWidget* rtt_top_bar;
    hidden GtkWidget* rtt_content_box;
    hidden GtkWidget* rtt_list_box;
    hidden GtkWidget* rtt_scrolled_container; // container used when scrollable == true
    hidden GtkWidget* rtt_fixed_container;    // container used when scrollable == false
    hidden bl_t is_open;

    func m x_array* client_array( c@* o ) = o.client.cast( m x_array* );

    func bcore_inst_call.down_e o.close();
    func bgfe_frame.rtt_widget = o.rtt_widget;
    func bgfe_frame.open;
    func bgfe_frame.close;
    func bgfe_frame.arrangement;
    func bgfe_frame.client_close_request;
    func bgfe_frame.client_close_confirm;
    func bgfe_frame.client_distraction;

    /// Interface functions ...

    func bgfe_frame.set_client_t;
    func bgfe_frame.cycle;
    func bgfe_frame.downsync;
    func bgfe_frame.upsync;
    func er_t close_all_item_windows( m@* o ); // sends a close request to client windows (only effective if any window is open)
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

identifier gtk_box_new, gtk_flow_box_new, gtk_widget_modify_bg, gtk_label_set_angle, gtk_box_pack_end, gtk_box_pack_start, gtk_scrolled_window_new;
identifier gtk_scrolled_window_set_propagate_natural_width, gtk_scrolled_window_set_propagate_natural_height;
identifier gtk_scrolled_window_set_max_content_width, gtk_scrolled_window_set_max_content_height;
identifier gtk_scrolled_window_set_min_content_width, gtk_scrolled_window_set_min_content_height;
identifier gtk_scrolled_window_set_policy, gtk_scrolled_window_set_overlay_scrolling;
identifier gtk_scrolled_window_set_kinetic_scrolling;
identifier gtk_container_remove;
identifier gtk_scrolled_window_get_vadjustment, gtk_adjustment_set_value, gtk_adjustment_get_upper, gtk_adjustment_get_lower, gtk_adjustment_get_page_size;
identifier gtk_scrolled_window_get_hadjustment, gtk_scrolled_window_set_vadjustment, gtk_scrolled_window_set_hadjustment;
identifier gtk_widget_grab_focus;
identifier gtk_toggle_button_get_active, gtk_toggle_button_set_active;
identifier GTK_CHECK_BUTTON, GTK_TOGGLE_BUTTON;
identifier gtk_check_button_new, gtk_check_button_new_with_label;

type GdkColor, GtkBox, GtkScrolledWindow, GTK_BOX, GTK_SCROLLED_WINDOW, GTK_POLICY_ALWAYS, GTK_POLICY_AUTOMATIC, GtkAdjustment;

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

embed "bgfe_frame_array_item.emb.x";
embed "bgfe_frame_array.emb.x";

