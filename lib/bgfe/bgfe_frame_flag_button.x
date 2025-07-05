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

/** boolean only-switch-on toggle based on 'GtkButton'
 *  Associated types: bl_t and all convertible leaf types
 */

//----------------------------------------------------------------------------------------------------------------------

stamp :s bgfe_frame
{
    sz_t width;  // optional preset width
    sz_t height; // optional preset height
    st_s => text;   // text on button; NULL: use client name as text
    st_s => widget_name;   // optional gtk widget name overrides default widget name
    bl_t show_tooltip = true;

    func bgfe_frame.set_width { o.width = value; = 0; }
    func bgfe_frame.set_height{ o.height = value; = 0; }
    func bgfe_frame.set_text  { o.text!.copy_sc( text ); = 0; }
    func bgfe_frame.set_widget_name{ o.widget_name!.copy_sc( text ); = 0; }
    func bgfe_frame.set_show_tooltip{ o.show_tooltip = flag; = 0; }

    bl_t value;     // flag value
    bl_t modified;  // value was modified by the front-end


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
    func bgfe_frame.h_complexity = 2;
    func bgfe_frame.v_complexity = 1;
    func bgfe_frame.is_compact = true;

    hidden bl_t rts_clicked; // button was clicked

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

func (:s) void rtt_signal_destroy( m GtkWidget* win, m@* o ) o.rtt_widget = NULL;

//----------------------------------------------------------------------------------------------------------------------

identifier gtk_button_set_label;
identifier GTK_BUTTON;

func (:s) void rtt_signal_clicked( m GtkWidget* win, m@* o )
{
    o.mutex.lock();
    o.rts_clicked = true;
    o.mutex.unlock();
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t rtt_set_text( m@* o, st_s* rts_text )
{
    o.mutex.lock();
    gtk_button_set_label( GTK_BUTTON( o.rtt_widget ), rts_text ? rts_text.sc : "" );
    o.mutex.unlock();
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

    bgfe_client_t_bgfe_copy_to_typed( o.client, o.client_type, o.client_type, TYPEOF_bl_t, o.value.1.cast( m x_inst* ) );

    o.rte.run( o.rtt_open.cast( bgfe_rte_fp_rtt ), o, NULL );
    o.is_open = true;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

identifier gtk_button_new, gtk_button_new_with_label, gtk_button_set_relief;
type GTK_RELIEF_NORMAL;

func (:s) er_t rtt_open( m@* o, vd_t unused )
{
    sc_t sc_label = bnameof( o.client_name );
    if( o.text ) sc_label = o.text.sc;

    o.rtt_attach_widget( sc_label ? gtk_button_new_with_label( sc_label ) : gtk_button_new(), o.rtt_widget );
    gtk_widget_set_name( o.rtt_widget, o.widget_name ? o.widget_name.sc : ifnameof( o._ ) );
    gtk_widget_set_size_request( o.rtt_widget, o.width, o.height );
    gtk_widget_show( o.rtt_widget );
    g_signal_connect( o.rtt_widget, "clicked", G_CALLBACK( :s_rtt_signal_clicked ), o );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) close
{
    if( !o.is_open ) = 0;
    o.rte.run( o.rtt_close.cast( bgfe_rte_fp_rtt ), o, NULL );
    o.is_open = false;
    o.client_close_confirm();
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
    if( !o.value && o.rts_clicked )
    {
        o.value = true;
        o.modified = true;
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
        bgfe_client_t_bgfe_copy_from_typed( o.client, o.client_type, o.client_type, TYPEOF_bl_t, o.value.1.cast( x_inst* ) );

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

    bl_t client_value = 0;
    bgfe_client_t_bgfe_copy_to_typed( o.client, o.client_type, o.client_type, TYPEOF_bl_t, client_value.1.cast( m x_inst* ) );

    if( client_value != o.value )
    {
        o.value = client_value;
        o.modified = false;
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
