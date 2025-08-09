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

/** Slider based on GTK 'Scale'
 *  Associated types: f3_t and convertible types
 */

//----------------------------------------------------------------------------------------------------------------------

stamp :s bgfe_frame
{
    /// slider is horizontal if width >= height; otherwise it is vertical
    sz_t width  = 100;
    sz_t height = 40;
    bl_t insensitive; // insensitive: does not react to user actions
    f3_t min   = 0.0;  // minimum value
    f3_t max   = 1.0;  // maximum value
    f3_t step  = 0.01; // value stepping
    bl_t show_value = true;  // displays the scale value along with the scale
    st_s => widget_name;   // optional gtk widget name overrides default widget name
    st_s => tooltip;     // external tooltip (if NULL an internal tooltip is used)
    bl_t show_tooltip = false;
    func bgfe_frame.set_show_tooltip{ o.show_tooltip = flag; = 0; }

    func bgfe_frame.set_width { o.width = value; = 0; }
    func bgfe_frame.set_height{ o.height = value; = 0; }
    func bgfe_frame.set_insensitive { o.insensitive = flag; = 0; }
    func bgfe_frame.set_min { o.min = value; = 0; }
    func bgfe_frame.set_max { o.max = value; = 0; }
    func bgfe_frame.set_step{ o.step = value; = 0; }
    func bgfe_frame.set_show_value{ o.show_value = flag; = 0; }
    func bgfe_frame.set_tooltip{ o.tooltip!.copy_sc( text ); = 0; }
    func bgfe_frame.set_widget_name{ o.widget_name!.copy_sc( text ); = 0; }

    hidden f3_t value;        // current scale value
    hidden bl_t modified;     // value was modified by the front-end

    /// internals

    hidden bgfe_client* client; // client
    hidden tp_t client_type; // to cover obliv clients
    hidden tp_t client_name; // name of the client (under which the client's owner holds the client)
    hidden bgfe_frame* parent;
    func bgfe_frame.client = o.client;
    func bgfe_frame.client_type = o.client_type;
    func bgfe_frame.client_name = o.client_name;
    func bgfe_frame.parent = o.parent;
    func bgfe_frame.is_open = o.is_open;
    func bgfe_frame.h_complexity = o.width / bgfe_frame_complexity_unit_size();
    func bgfe_frame.v_complexity = o.height / bgfe_frame_complexity_unit_size();
    func bgfe_frame.is_compact = true;

    hidden f3_t rts_value;    // current scale value
    hidden bl_t rts_modified; // scale value was modified by the front end

    hidden st_s => tooltip_text;  // tooltip text
    hidden bgfe_rte_s* rte;
    hidden x_mutex_s mutex;
    hidden GtkWidget*  rtt_widget;
    hidden bl_t is_open;
    hidden bl_t rtt_ignore_signal_value_changed;

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

identifier gtk_range_get_value;
identifier GTK_RANGE;

func (:s) void rtt_signal_value_changed( m GtkWidget* win, m@* o )
{
    if( o.rtt_ignore_signal_value_changed ) return;
    o.mutex.lock();
    o.rts_value = gtk_range_get_value( GTK_RANGE( o.rtt_widget ) );
    o.rts_modified = true;
    o.mutex.unlock();
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t rtt_set_value( m@* o, f3_t* rts_value )
{
    o.rtt_ignore_signal_value_changed = true;
    o.mutex.lock();
    gtk_range_set_value( GTK_RANGE( o.rtt_widget ), rts_value.0 );
    o.mutex.unlock();
    o.rtt_ignore_signal_value_changed = false;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) open
{
    if( o.is_open ) = 0;
    if( !o.client ) = GERR_fa( "No client defined. Call set_client or set_client_t first." );
    bgfe_rte_get( &o.rte );

    ASSERT( parent );
    o.parent = parent;

    bgfe_client_t_bgfe_copy_to_typed( o.client, o.client_type, o.client_type, TYPEOF_f3_t, o.value.1.cast( m x_inst* ) );

    if( o.tooltip )
    {
        o.tooltip_text!.push_st( o.tooltip );
    }
    else
    {
        if( bnameof( o.client_name ) ) o.tooltip_text!.push_fa( "#<sc_t>", bnameof( o.client_name ) );
        if( bnameof( o.client_type ) )
        {
            if( o.tooltip_text ) o.tooltip_text!.push_fa( " " );
            o.tooltip_text!.push_fa( "<#<sc_t>>", bnameof( o.client_type ) );
        }
    }

    o.rts_value = o.value;

    o.rte.run( o.rtt_open.cast( bgfe_rte_fp_rtt ), o, NULL );
    o.is_open = true;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

identifier gtk_scale_new_with_range;
identifier gtk_scale_set_draw_value;
identifier gtk_range_set_value, gtk_range_set_inverted;
identifier GTK_SCALE, GTK_RANGE;

func (:s) er_t rtt_open( m@* o, vd_t unused )
{
    bl_t vertical = ( o.height > o.width );
    o.rtt_attach_widget
    (
        gtk_scale_new_with_range
        (
            vertical ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL,
            o.min,
            o.max,
            o.step
        ),
        o.rtt_widget
    );

    if( o.insensitive ) gtk_widget_set_state_flags( o.rtt_widget, GTK_STATE_FLAG_INSENSITIVE, false );
    gtk_widget_set_name( o.rtt_widget, o.widget_name ? o.widget_name.sc : ifnameof( o._ ) );
    gtk_range_set_inverted( GTK_RANGE( o.rtt_widget ), vertical );
    gtk_scale_set_draw_value( GTK_SCALE( o.rtt_widget ), o.show_value );
    gtk_widget_set_size_request( o.rtt_widget, o.width, o.height );
    gtk_range_set_value( GTK_RANGE( o.rtt_widget ), o.value );
    if( o.show_tooltip && o.tooltip_text ) gtk_widget_set_tooltip_text( o.rtt_widget, o.tooltip_text.sc );
    gtk_widget_show( o.rtt_widget );
    g_signal_connect( o.rtt_widget, "value-changed", G_CALLBACK( :s_rtt_signal_value_changed ), o );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) close
{
    if( !o.is_open ) = 0;
    o.rte.run( o.rtt_close.cast( bgfe_rte_fp_rtt ), o, NULL );
    o.is_open = false;
    o.tooltip_text =< NULL;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t rtt_close( m@* o, vd_t arg )
{
    o.rtt_detach_widget( o.rtt_widget );
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

func (:s) bgfe_frame.cycle
{
    if( !o.is_open ) = 0; // no error because frame window could have been closed

    o.mutex.lock();
    o.modified = o.rts_modified;
    if( o.modified )
    {
        o.value = o.rts_value;
        o.rts_modified = false;
    }
    o.mutex.unlock();

    if( o.modified )
    {
        if( action_type == approve~ ) = o.downsync();
        if( action_type == reject~  ) = o.upsync();
        o.client_change_request( o, action_type.1 );
        if( action_type == approve~ ) = o.downsync();
        if( action_type == reject~  ) = o.upsync();
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) bgfe_frame.downsync
{
    if( !o.is_open ) = 0; // no error because frame window could have been closed

    if( o.modified )
    {
        bgfe_client_t_bgfe_copy_from_typed( o.client, o.client_type, o.client_type, TYPEOF_f3_t, o.value.1.cast( x_inst* ) );

        o.modified = false;
        tp_t action_type = TYPEOF_escalate;
        o.client_change_confirm( o, action_type.1 );
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) bgfe_frame.upsync
{
    if( !o.is_open ) = 0; // no error because frame window could have been closed

    f3_t client_value = 0;
    bgfe_client_t_bgfe_copy_to_typed( o.client, o.client_type, o.client_type, TYPEOF_f3_t, client_value.1.cast( m x_inst* ) );

    if( client_value != o.value )
    {
        o.value = client_value;
        o.mutex.lock();
        f3_t rts_value = o.value;
        o.mutex.unlock();

        o.rte.run( o.rtt_set_value.cast( bgfe_rte_fp_rtt ), o, rts_value.1 );
        o.modified = false;
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
