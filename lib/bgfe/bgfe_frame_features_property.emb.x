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
/// frame properties

//----------------------------------------------------------------------------------------------------------------------

feature er_t set_width             ( m@* o, sz_t value ) = 0;
feature er_t set_height            ( m@* o, sz_t value ) = 0;
feature er_t set_insensitive       ( m@* o, bl_t flag  ) = 0; // insensitive: frame is insensitive to user actions (possibly grayed out, etc)

// sets widget/window position; reference coordinate system depends on the widget
feature er_t set_x                 ( m@* o, sz_t value ) = 0;
feature er_t set_y                 ( m@* o, sz_t value ) = 0;

feature er_t set_arrange           ( m@* o, tp_t name  ) = 0;
name horizontal; // horizontal arrangement
name vertical;   // vertical arrangement
name automatic;  // automatic arrangement

feature er_t set_arr_editable      ( m@* o, bl_t flag  ) = 0; // allows/inhibits adding, removing and reordering on arrays
feature er_t set_scrollable        ( m@* o, bl_t flag  ) = 0; // typically supported on arrays
feature er_t set_min_content_width ( m@* o, sz_t value ) = 0; // for editable arrays: size allocation for content
feature er_t set_min_content_height( m@* o, sz_t value ) = 0; // for editable arrays: size allocation for content
feature er_t set_max_content_width ( m@* o, sz_t value ) = 0; // for scrollable editable arrays: size limit at which scroll bars appear
feature er_t set_max_content_height( m@* o, sz_t value ) = 0; // for scrollable editable arrays: size limit at which scroll bars appear
feature er_t set_nesting_level     ( m@* o, sz_t value ) = 0; // the nesting level is is automatically increased for embedded frames
feature er_t set_tooltip           ( m@* o, sc_t text  ) = 0; // defines external tooltip text (internal text might be appended)
feature er_t set_show_client_name  ( m@* o, bl_t flag  ) = 0; // true: displays client name (e.g. as label)
feature er_t set_show_tooltip      ( m@* o, bl_t flag  ) = 0; // shows tooltip (if any offered)
feature er_t set_show_border       ( m@* o, bl_t flag  ) = 0; // true: displays a border around composite frames
feature er_t set_show_index        ( m@* o, bl_t flag  ) = 0; // true: displays an index (arrays/lists)
feature er_t set_show_value        ( m@* o, bl_t flag  ) = 0; // where showing the current value is optional (e.g. scale)
feature er_t set_show_glimpse      ( m@* o, bl_t flag  ) = 0; // shows a glimpse where it is supported
feature er_t set_text              ( m@* o, sc_t text  ) = 0; // frames with text property: (e.g. button, label, entry)
feature er_t set_text_xalign       ( m@* o, f3_t value ) = 0; // frames with text property: gradual text alignment: 0: left, 0.5: center, 1.0 right
feature er_t set_text_yalign       ( m@* o, f3_t value ) = 0; // frames with text property: gradual text alignment: 0: top,  0.5: center, 1.0 bottom
feature er_t set_title             ( m@* o, sc_t text  ) = 0; // for frames with a title property (e.g. window, frame_s, array_s)
feature er_t set_min               ( m@* o, f3_t value ) = 0; // for value clamping (e.g. scale)
feature er_t set_max               ( m@* o, f3_t value ) = 0; // for value clamping (e.g. scale)
feature er_t set_step              ( m@* o, f3_t value ) = 0; // for value stepping (e.g. scale)
feature er_t set_widget_name       ( m@* o, sc_t text  ) = 0; // optional gtk widget name overrides default widget name
feature er_t set_spacing           ( m@* o, sz_t value ) = 0; // spacing between elements
feature er_t set_end_bound         ( m@* o, bl_t flag  ) = 0; // packs elements with reference to the end of the box (false: reference to the start)
feature er_t set_center            ( m@* o, bl_t flag  ) = 0; // centers widgets in an expanded space
feature er_t set_stretch           ( m@* o, bl_t flag  ) = 0; // stretches elements to fill expanded space
feature er_t set_embed_link        ( m@* o, bl_t flag  ) = 0; // when the client is a link, embed the frame rather than displaying it in a dedicated window.
feature er_t set_apply_alpha       ( m@* o, bl_t flag  ) = 0; // used for frames that can decode an alpha value from the client (e.g. canvas)

feature er_t set_window_policy     ( m@* o, tp_t name  ) = 0; // window policy applies to immediate children. type: any|one|zero
name any; // all clients may open a window
name one; // all clients may open a window; maximally one window is open at a time (prior windows are being closed)
name zero; // no client can open a window;

feature er_t set_track_placement( m@* o, bl_t flag ) = 0; // tracks placement (positioning, size; creates associated client-events)
feature er_t set_track_mouse    ( m@* o, bl_t flag ) = 0; // tracks mouse     (buttons,movement; creates associated client-events)

// window related properties
feature er_t set_keep_above         ( m@* o, bl_t flag ) = 0; // for floating frames (e.g. window)
feature er_t set_decorated          ( m@* o, bl_t flag ) = 0; // adds decoration, make window movable and resizable
feature er_t set_close_on_lost_focus( m@* o, bl_t flag ) = 0; // closes window when focus is lost
feature er_t set_fleeting           ( m@* o, bl_t flag ) = 0; // closes window on distraction

// Specifies the case under which the client is informed and/or modified about a change. (used by entry)
feature er_t set_take_action_on( m@* o, tp_t name ) = 0;
name modified; // front end detected a modification
name activate; // front received a signal to communicate a modification (e.g. editor: enter-pressed)

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
