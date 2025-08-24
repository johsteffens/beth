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

/** Button with text label.
 *  This frame behaves like a label with additional button-press capability.
 *  Button press initiates bgfe_frame_button_pressed feature on client.
 */

//----------------------------------------------------------------------------------------------------------------------

stamp :s bgfe_frame
{
    /// parameters
    sz_t width;  // optional preferred width of button  (actual size is calculated from the ext)
    sz_t height; // optional preferred height of button (actual size is calculated from the ext)
    bl_t insensitive; // insensitive: does not react to user actions
    st_s => text;   // text on button
    st_s => widget_name; // optional gtk widget name overrides default widget name
    st_s => tooltip;     // external tooltip (if NULL an internal tooltip is used)
    bl_t show_tooltip = true;
    bl_t no_upsync;
    f3_t text_xalign = 0.5; // gradual text alignment: 0: left, 0.5: center, 1.0 right
    f3_t text_yalign = 0.5; // gradual text alignment: 0: top, 0.5: center, 1.0 bottom

    func bgfe_frame.set_width { o.width = value; = 0; }
    func bgfe_frame.set_height{ o.height = value; = 0; }
    func bgfe_frame.set_insensitive { o.insensitive = flag; = 0; }
    func bgfe_frame.set_text  { o.text!.copy_sc( text ); o.no_upsync = true; = 0; }
    func bgfe_frame.set_widget_name{ o.widget_name!.copy_sc( text ); = 0; }
    func bgfe_frame.set_tooltip{ o.tooltip!.copy_sc( text ); = 0; }
    func bgfe_frame.set_show_tooltip{ o.show_tooltip = flag; = 0; }
    func bgfe_frame.set_text_xalign { o.text_xalign = f3_max( 0, f3_min( 1, value ) ); = 0; }
    func bgfe_frame.set_text_yalign { o.text_yalign = f3_max( 0, f3_min( 1, value ) ); = 0; }

    /// internals

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
    func bgfe_frame.v_complexity = 1;
    func bgfe_frame.is_compact = true;

    hidden bl_t rts_clicked; // button was clicked
    hidden bl_t rts_pressed; // button was pressed

    hidden st_s => tooltip_text;  // tooltip text
    hidden bgfe_rte_s* rte;
    hidden x_mutex_s mutex;
    hidden GtkWidget* rtt_widget; // button widget
    hidden GtkWidget* rtt_label; // label widget
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

func (:s) void rtt_signal_destroy( m GtkWidget* win, m@* o ) o.rtt_widget = NULL;

//----------------------------------------------------------------------------------------------------------------------

func (:s) void rtt_signal_clicked( m GtkWidget* win, m@* o )
{
    o.mutex.lock();
    o.rts_clicked = true;
    o.mutex.unlock();
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) gboolean rtt_signal_button_press_event( m GtkWidget* drw, GdkEventButton* event, m@* o )
{
    if( verbatim_C{ event->button == 1 } )
    {
        o.mutex.lock();
        o.rts_pressed = true;
        o.mutex.unlock();
    }
    = FALSE;
}

//----------------------------------------------------------------------------------------------------------------------

identifier gtk_label_set_text;
identifier GTK_BUTTON;
func (:s) er_t rtt_set_text( m@* o, st_s* rts_text )
{
    o.mutex.lock();
    gtk_label_set_text( GTK_LABEL( o.rtt_label ), rts_text ? rts_text.sc : "" );
    o.mutex.unlock();
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t client_to_st( @* o, m st_s* st )
{
    if( !o.client ) = 0;
    if( o.client_type == tp_t~ || o.client_type == aware_t~ || o.client_type == er_t~ )
    {
        sc_t name = bnameof( o.client.cast( tp_t* ).0 );
        if( name )
        {
            st.copy_sc( name );
            = 0;
        }
    }

    // no else

    if( bgfe_client_t_defines_bgfe_get_glimpse( o.client_type ) )
    {
        st.clear();
        o.client.t_bgfe_get_glimpse( o.client_type, st );
    }
    else
    {
        bgfe_client_t_bgfe_copy_to_typed( o.client, o.client_type, o.client_type, TYPEOF_st_s, st.cast( m x_inst* ) );
    }

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

    if( !o.text ) o.client_to_st( o.text! );

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

    o.rte.run( o.rtt_open.cast( bgfe_rte_fp_rtt ), o, NULL );
    o.is_open = true;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

identifier gtk_button_new, gtk_button_new_with_label, gtk_button_set_relief, gtk_label_set_xalign, gtk_label_set_yalign;
type GTK_RELIEF_NORMAL, GTK_LABEL;

func (:s) er_t rtt_open( m@* o, vd_t unused )
{
    o.mutex.create_lock()^;
    o.rtt_attach_widget( gtk_button_new(), o.rtt_widget );
    o.rtt_attach_widget( gtk_label_new( o.text ? o.text.sc : "" ), o.rtt_label );

    gtk_label_set_xalign( GTK_LABEL( o.rtt_label ), o.text_xalign );
    gtk_label_set_yalign( GTK_LABEL( o.rtt_label ), o.text_yalign );

    gtk_container_add( GTK_CONTAINER( o.rtt_widget ), o.rtt_label );
    gtk_widget_show( o.rtt_widget );
    gtk_widget_show( o.rtt_label );

    if( o.insensitive ) gtk_widget_set_state_flags( o.rtt_widget, GTK_STATE_FLAG_INSENSITIVE, false );
    gtk_button_set_relief( GTK_BUTTON( o.rtt_widget ), GTK_RELIEF_NORMAL );
    gtk_widget_set_name( o.rtt_widget, "bgfe_button_compact" );
    if( o.width > 0 || o.height > 0 ) gtk_widget_set_size_request( o.rtt_widget, o.width, o.height );
    if( o.show_tooltip && o.tooltip_text ) gtk_widget_set_tooltip_text( o.rtt_widget, o.tooltip_text.sc );
    g_signal_connect( o.rtt_widget, "destroy", G_CALLBACK( :s_rtt_signal_destroy ), o );
    g_signal_connect( o.rtt_widget, "clicked", G_CALLBACK( :s_rtt_signal_clicked ), o );

    g_signal_connect( o.rtt_widget, "button-press-event",   G_CALLBACK( o.rtt_signal_button_press_event   ), o );
    gtk_widget_add_events( o.rtt_widget, GDK_BUTTON_PRESS_MASK );

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
    o.rtt_detach_widget( o.rtt_label );
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

func (:s) bgfe_frame.client_change_request = 0;
func (:s) bgfe_frame.client_change_confirm = 0;

//----------------------------------------------------------------------------------------------------------------------

func (:s) bgfe_frame.cycle
{
    if( !o.is_open ) = 0; // no error because frame window could have been closed

    o.mutex.lock();
    bl_t pressed = o.rts_pressed;
    bl_t clicked = o.rts_clicked;
    o.rts_pressed = false;
    o.rts_clicked = false;
    o.mutex.unlock();

    if( pressed ) o.client_button_pressed( o, action_type.1 );
    if( clicked ) o.client_button_clicked( o, action_type.1 );

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
    if( !o.is_open ) = 0; // no error because frame window could have been closed
    if( o.no_upsync ) = 0;

    m$* client_text = st_s!^;
    o.client_to_st( client_text );

    if( !client_text.equal( o.text ) )
    {
        o.text =< client_text.clone();
        o.mutex.lock();
        m$* rts_text = client_text.clone()^;
        o.mutex.unlock();
        o.rte.run( o.rtt_set_text.cast( bgfe_rte_fp_rtt ), o, rts_text );
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
