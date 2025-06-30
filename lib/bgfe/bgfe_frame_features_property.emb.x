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

feature er_t set_width             ( m@* o, sz_t value   ) = 0;
feature er_t set_height            ( m@* o, sz_t value   ) = 0;
feature er_t set_arrange           ( m@* o, tp_t arrange ) = 0; // 0, horizontal~, vertical~
feature er_t set_min_content_width ( m@* o, sz_t value   ) = 0; // applies for horizontal orientation
feature er_t set_min_content_height( m@* o, sz_t value   ) = 0; // applies for vertical orientation
feature er_t set_max_content_width ( m@* o, sz_t value   ) = 0; // applies for horizontal orientation
feature er_t set_max_content_height( m@* o, sz_t value   ) = 0; // applies for vertical orientation
feature er_t set_show_client_name  ( m@* o, bl_t flag    ) = 0; // true: displays client name (e.g. as label)
feature er_t set_show_tooltip      ( m@* o, bl_t flag    ) = 0; // shows tooltip (if any offered)
feature er_t set_show_value        ( m@* o, bl_t flag    ) = 0; // where showing the current value is optional (e.g. scale)
feature er_t set_show_glimpse      ( m@* o, bl_t flag    ) = 0; // shows a glimpse where it is supported
feature er_t set_text              ( m@* o, sc_t text    ) = 0; // for frames with a text property (e.g. button, label, entry)
feature er_t set_min               ( m@* o, f3_t value   ) = 0; // for value clamping (e.g. scale)
feature er_t set_max               ( m@* o, f3_t value   ) = 0; // for value clamping (e.g. scale)
feature er_t set_step              ( m@* o, f3_t value   ) = 0; // for value stepping (e.g. scale)
feature er_t set_keep_above        ( m@* o, bl_t flag    ) = 0; // for floating frames (e.g. window)
feature er_t set_widget_name       ( m@* o, sc_t text    ) = 0; // optional gtk widget name overrides default widget name
feature er_t set_spacing           ( m@* o, sz_t value   ) = 0; // spacing between elements
feature er_t set_end_bound         ( m@* o, bl_t flag    ) = 0; // packs elements with reference to the end of the box (false: reference to the start)
feature er_t set_center            ( m@* o, bl_t flag    ) = 0; // centers widgets in an expanded space
feature er_t set_stretch           ( m@* o, bl_t flag    ) = 0; // stretches elements to fill expanded space
feature er_t set_embed_link        ( m@* o, bl_t flag    ) = 0; // when the client is a link, embed the frame rather than displaying it in a dedicated window.

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
