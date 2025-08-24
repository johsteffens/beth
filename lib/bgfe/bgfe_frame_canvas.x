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

/// small text frame based on gtk label

/** Canvas frame associated with a drawable area and an image-like client
 */

//----------------------------------------------------------------------------------------------------------------------

stamp :s bgfe_frame
{
    /// parameters
    sz_t width;  // optional preferred width of label  (actual size is calculated from the ext)
    sz_t height; // optional preferred height of label (actual size is calculated from the ext)
    bl_t insensitive; // insensitive: does not react to user actions
    st_s => widget_name;   // optional gtk widget name overrides default widget name
    bl_t apply_alpha;
    bl_t track_placement = true; // traces placement (positioning, size; creates associated client-events)
    bl_t track_mouse;            // traces mouse     (buttons,movement; creates associated client-events)

    func bgfe_frame.set_width { o.width = value; = 0; }
    func bgfe_frame.set_height{ o.height = value; = 0; }
    func bgfe_frame.set_insensitive { o.insensitive = flag; = 0; }
    func bgfe_frame.set_widget_name{ o.widget_name!.copy_sc( text ); = 0; }
    func bgfe_frame.set_track_placement{ o.track_placement = flag; = 0; }
    func bgfe_frame.set_track_mouse    { o.track_mouse = flag; = 0; }

    /// internals
    bcodec_image_bgra_s => rts_image_bgra;

    bl_t rts_attention; // attention in cycle required

    bgfe_placement_shell_s => placement_shell;
    bgfe_mouse_shell_s     => mouse_shell;

    hidden bgfe_client* client; // client
    hidden tp_t client_type; // to cover obliv clients
    hidden tp_t client_name; // name of the client (under which the client's owner holds the client)
    hidden bgfe_frame* parent;
    func bgfe_frame.client = o.client;
    func bgfe_frame.client_type = o.client_type;
    func bgfe_frame.client_name = o.client_name;
    func bgfe_frame.parent = o.parent;
    func bgfe_frame.set_parent o.parent = parent;
    func bgfe_frame.is_open = o.is_open;
    func bgfe_frame.h_complexity = 2;
    func bgfe_frame.v_complexity = 0.5;
    func bgfe_frame.is_compact = true;

    hidden st_s => tooltip_text;  // tooltip text
    hidden bgfe_rte_s* rte;
    hidden x_mutex_s mutex;
    hidden GtkWidget*  rtt_widget;
    hidden GtkWidget*  rtt_drawing_area;

    hidden bl_t is_open;

    func bcore_inst_call.down_e o.close();

    func bgfe_frame.rtt_widget = o.rtt_widget;
    func bgfe_frame.open;
    func bgfe_frame.close;

    /// interface functions ...
    func bgfe_frame.set_client_t;
    func bgfe_frame.cycle;
    func bgfe_frame.upsync;
    func bgfe_frame.downsync;
}

//----------------------------------------------------------------------------------------------------------------------

identifier gtk_widget_queue_draw;
func (:s) int rtt_redraw( m @* o )
{
    if( o.rtt_drawing_area ) verbatim_C { gtk_widget_queue_draw( o->rtt_drawing_area ); };
    = 0;
}

identifier cairo_image_surface_create_for_data, cairo_set_source_surface, cairo_paint, cairo_surface_destroy;
identifier CAIRO_FORMAT_ARGB32, CAIRO_FORMAT_RGB24;
type cairo_t, cairo_surface_t, cairo_format_t;

func (:s) gboolean rtt_signal_draw( m GtkWidget* drw, m cairo_t* cairo, m@* o )
{
    o.mutex.create_lock()^;
    if( !o.rts_image_bgra ) = FALSE;

    cairo_format_t cairo_format = o.apply_alpha ? CAIRO_FORMAT_ARGB32 : CAIRO_FORMAT_RGB24;

    /// this function only references image->data but does not copy or modify it
    m cairo_surface_t* surface = cairo_image_surface_create_for_data
    (
        ( u0_t* )o.rts_image_bgra->data,
        cairo_format,
        o.rts_image_bgra.cols,
        o.rts_image_bgra.rows,
        o.rts_image_bgra.bytes_per_row
    );

    if( surface )
    {
        cairo_set_source_surface( cairo, surface, 0, 0 );
        cairo_paint( cairo );
    }
    cairo_surface_destroy( surface );
    = FALSE;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) open
{
    if( o.is_open ) = 0;
    if( !o.client ) = GERR_fa( "No client defined. Call set_client or set_client_t first." );
    bgfe_rte_get( &o.rte );

    ASSERT( parent );
    o.parent = parent;

    if( o.track_placement ) o.placement_shell!.setup( o );
    if( o.track_mouse     ) o.mouse_shell!.setup( o );

    if( o.client ) o.upsync();
    o.rte.run( o.rtt_open.cast( bgfe_rte_fp_rtt ), o, NULL );
    o.is_open = true;

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

type GtkGesture, GtkEventController;
identifier gtk_gesture_drag_new, gtk_gesture_single_set_button, gtk_event_controller_motion_new, gtk_event_controller_scroll_new;
identifier gtk_widget_add_events, gtk_drawing_area_new;
identifier GTK_GESTURE_SINGLE, GDK_BUTTON_PRIMARY, GTK_EVENT_CONTROLLER_SCROLL_VERTICAL, GDK_POINTER_MOTION_MASK, GDK_SCROLL_MASK;
identifier GDK_BUTTON_PRIMARY, GTK_EVENT_CONTROLLER_SCROLL_VERTICAL, GDK_POINTER_MOTION_MASK, GDK_SCROLL_MASK, GDK_BUTTON_PRESS_MASK, GDK_BUTTON_RELEASE_MASK;
identifier GDK_ENTER_NOTIFY_MASK, GDK_LEAVE_NOTIFY_MASK;

func (:s) er_t rtt_open( m@* o, vd_t unused )
{
    o.rtt_attach_widget( gtk_drawing_area_new(), o.rtt_drawing_area );
    if( o.insensitive ) gtk_widget_set_state_flags( o.rtt_drawing_area, GTK_STATE_FLAG_INSENSITIVE, false );

    sz_t width = o.width;
    sz_t height = o.height;

    if( o.rts_image_bgra )
    {
        width = sz_max( width, o.rts_image_bgra.cols );
        height = sz_max( height, o.rts_image_bgra.rows );
    }

    gtk_widget_set_size_request( o.rtt_drawing_area, width, height );
    gtk_widget_show( o.rtt_drawing_area );
    gtk_widget_set_name( o.rtt_drawing_area, o.widget_name ? o.widget_name.sc : ifnameof( o._ ) );

    g_signal_connect( o.rtt_drawing_area, "draw", G_CALLBACK( o.rtt_signal_draw ), o );

    if( o.placement_shell ) o.placement_shell.rtt_open( o.rtt_drawing_area );
    if( o.mouse_shell     ) o.mouse_shell    .rtt_open( o.rtt_drawing_area );

    o.rtt_widget = o.rtt_drawing_area;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) close
{
    if( !o.is_open ) = 0;
    o.rte.run( o.rtt_close.cast( bgfe_rte_fp_rtt ), o, NULL );
    o.is_open = false;
    o.placement_shell =< NULL;
    o.mouse_shell =< NULL;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t rtt_close( m@* o, vd_t arg )
{
    if( o.placement_shell ) o.placement_shell.rtt_close();
    if( o.mouse_shell     ) o.mouse_shell    .rtt_close();
    o.rtt_detach_widget( o.rtt_drawing_area );
    o.rtt_widget = NULL;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) set_client_t
{
    if( o.is_open ) = GERR_fa( "Frame is open." );

    o.client      = client;
    o.client_type = client_type;
    o.client_name = client_name;

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) bgfe_frame.client_change_request = 0;
func (:s) bgfe_frame.client_change_confirm = 0;

//----------------------------------------------------------------------------------------------------------------------

func (:s) bgfe_frame.cycle
{
    if( !o.is_open ) = 0; // no error because frame window could have been closed

    if( o.placement_shell ) o.placement_shell.cycle( o );
    if( o.mouse_shell     ) o.mouse_shell    .cycle( o );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) bgfe_frame.downsync
{
    if( !o.is_open ) = 0; // no error because frame window could have been closed
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) bgfe_frame.upsync
{

    if( o.client )
    {
        o.mutex.create_lock()^;
        o.rts_image_bgra!;

        bgfe_client_t_bgfe_copy_to_typed( o.client, o.client_type, o.client_type, bcodec_image_bgra_s~, o.rts_image_bgra.cast( m x_inst* ) );
    }

    if( o.is_open )
    {
        verbatim_C { g_idle_add( (int(*)(vd_t))bgfe_frame_canvas_s_rtt_redraw, o ) }; // g_idle_add is supposed to be thread safe
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
