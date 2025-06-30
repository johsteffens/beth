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

/** boolean toggle based on GTK 'CheckButton'
 *  Associated types: bl_t and all convertible leaf types
 */

//----------------------------------------------------------------------------------------------------------------------

stamp :s bgfe_frame
{
    sz_t width;  // optional preset width
    sz_t height; // optional preset height
    bl_t show_client_name = true;
    bl_t show_tooltip = true;
    st_s => widget_name;   // optional gtk widget name overrides default widget name

    func bgfe_frame.set_width   { o.width   = value; = 0; }
    func bgfe_frame.set_height  { o.height  = value; = 0; }
    func bgfe_frame.set_widget_name{ o.widget_name!.copy_sc( text ); = 0; }
    func bgfe_frame.set_show_client_name { o.show_client_name = flag; = 0; }
    func bgfe_frame.set_show_tooltip{ o.show_tooltip = flag; = 0; }

    bl_t value;        // check value
    bl_t modified;     // value was modified by the front-end

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

    hidden bl_t rts_value;    // current scale value
    hidden bl_t rts_modified; // scale value was modified by the front end

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

identifier gtk_toggle_button_get_active, gtk_toggle_button_set_active;
identifier GTK_CHECK_BUTTON, GTK_TOGGLE_BUTTON;

func (:s) void rtt_signal_value_changed( m GtkWidget* win, m@* o )
{
    if( o.rtt_ignore_signal_value_changed ) return;
    o.mutex.lock();
    o.rts_value = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( o.rtt_widget ) );
    o.rts_modified = true;
    o.mutex.unlock();
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t rtt_set_value( m@* o, f3_t* rts_value )
{
    o.rtt_ignore_signal_value_changed = true;
    o.mutex.lock();
    gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( o.rtt_widget ), rts_value.0 );
    o.mutex.unlock();
    o.rtt_ignore_signal_value_changed = false;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

stamp :open_args_s
{
    sz_t width;
    sz_t height;
    bl_t value;

    func o _( m@* o, m :s* f )
    {
        o.width  = f.width;
        o.height = f.height;
        o.value  = f.value;
    }
}

func (:s) open
{
    if( o.is_open ) = 0;
    if( !o.client ) = GERR_fa( "No client defined. Call set_client or set_client_t first." );
    bgfe_rte_get( &o.rte );

    ASSERT( parent );
    o.parent = parent;

    bgfe_client_t_bgfe_copy_to_typed( o.client, o.client_type, o.client_type, TYPEOF_bl_t, o.value.1.cast( m x_inst* ) );

    o.mutex.lock();
    o.rts_value = o.value;
    m$* rts_open_args = :open_args_s!^( o );
    o.mutex.unlock();

    o.rte.run( o.rtt_open.cast( bgfe_rte_fp_rtt ), o, rts_open_args );
    o.is_open = true;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

identifier gtk_check_button_new, gtk_check_button_new_with_label;
func (:s) er_t rtt_open( m@* o, :open_args_s* args )
{
    {
        o.mutex.create_lock()^;
        sc_t sc_client_name = bnameof( o.client_name );
        if( sc_client_name )
        {
            o.rtt_widget = gtk_check_button_new_with_label( sc_client_name );
        }
        else
        {
            o.rtt_widget = gtk_check_button_new();
        }
        if( !o.rtt_widget ) = GERR_fa( "'gtk_check_button_new' failed\n" );
        if( G_IS_INITIALLY_UNOWNED( o.rtt_widget ) ) o.rtt_widget = g_object_ref_sink( o.rtt_widget );
        gtk_widget_set_name( o.rtt_widget, o.widget_name ? o.widget_name.sc : ifnameof( o._ ) );
        gtk_widget_set_size_request( o.rtt_widget, args.width, args.height );
        gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( o.rtt_widget ), args.value );
        gtk_widget_show( o.rtt_widget );
    }

    g_signal_connect( o.rtt_widget, "destroy", G_CALLBACK( :s_rtt_signal_destroy ),       o );
    g_signal_connect( o.rtt_widget, "toggled", G_CALLBACK( :s_rtt_signal_value_changed ), o );

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
    if( o.rtt_widget )
    {
        g_signal_handlers_disconnect_by_data( o.rtt_widget, o );
        g_object_unref( o.rtt_widget );
        o.rtt_widget = NULL;
    }
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
