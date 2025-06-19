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

/** Button with text label representing a link.
 *  This frame opens a dialog box when the button is pressed in order to edit/create the underlying link of the parent's client.
 *  The client of the link frame is the linked object.
 *  The parents client is the object holding the link.
 *  The link maintains a strong reference to its client because the link-holder can delete the link, rendering the linked object invalid.
 */

//----------------------------------------------------------------------------------------------------------------------

stamp :s bgfe_frame
{
    /// parameters
    sz_t width;  // optional preferred width of button  (actual size is calculated from the ext)
    sz_t height; // optional preferred height of button (actual size is calculated from the ext)

    /// internals
    hidden sr_s client; // client (as strong smart reference)
    hidden tp_t client_name; // name of the client (under which the client's owner holds the client)
    hidden bgfe_frame* parent;
    func bgfe_frame.client = o.client.o;
    func bgfe_frame.client_type = o.client.type();
    func bgfe_frame.client_name = o.client_name;
    func bgfe_frame.parent = o.parent;
    func bgfe_frame.is_open = o.is_open;

    hidden bgfe_window_s => window;

    hidden bl_t rts_main_clicked; // main button was clicked
    hidden bl_t rts_clear_clicked; // clear button was clicked

    hidden st_s => rts_tooltip_text;  // tooltip text
    hidden bgfe_rte_s* rte;
    hidden x_mutex_s mutex;
    hidden GtkWidget* rtt_widget;
    hidden GtkWidget* rtt_button_main;
    hidden GtkWidget* rtt_button_clear;
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

func (:s) void rtt_signal_destroy_widget      ( m GtkWidget* win, m@* o ) o.rtt_widget = NULL;
func (:s) void rtt_signal_destroy_button_main ( m GtkWidget* win, m@* o ) o.rtt_button_main = NULL;
func (:s) void rtt_signal_destroy_button_clear( m GtkWidget* win, m@* o ) o.rtt_button_clear = NULL;

//----------------------------------------------------------------------------------------------------------------------

func (:s) void rtt_button_main_signal_clicked( m GtkWidget* win, m@* o )
{
    o.mutex.lock();
    o.rts_main_clicked = true;
    o.mutex.unlock();
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) void rtt_button_clear_signal_clicked( m GtkWidget* win, m@* o )
{
    o.mutex.lock();
    o.rts_clear_clicked = true;
    o.mutex.unlock();
}

//----------------------------------------------------------------------------------------------------------------------

identifier gtk_button_set_label;
identifier GTK_BUTTON;
func (:s) er_t rtt_set_text( m@* o, st_s* rts_text )
{
    o.mutex.lock();
    gtk_button_set_label( GTK_BUTTON( o.rtt_button_main ), rts_text.sc );
    o.mutex.unlock();
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t set_text( m@* o, sc_t text )
{
    o.mutex.lock();
    m$* rts_text = st_s!^.copy_sc( text );
    o.mutex.unlock();
    o.rte.run( o.rtt_set_text.cast( bgfe_rte_fp_rtt ), o, rts_text );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t set_text_from_client_status( m@* o )
{
    m$* text = st_s!^;
    if( o.client.o )
    {
        text.copy_fa( "<#<sc_t>>", bnameof( o.client.type() ) );
    }
    else
    {
        text.copy_fa( "NULL" );
    }
    o.set_text( text.sc );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

stamp :open_args_s
{
    sz_t width;
    sz_t height;

    func o _( m@* o, m :s* f )
    {
        o.width  = f.width;
        o.height = f.height;
    }
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) open
{
    if( o.is_open ) = 0;
    bgfe_rte_get( &o.rte );

    ASSERT( parent );
    o.parent = parent;

    o.mutex.lock();
    if( bnameof( o.client_name ) ) o.rts_tooltip_text!.push_fa( "#<sc_t>", bnameof( o.client_name ) );
    if( bnameof( o.client.type() ) )
    {
        if( o.rts_tooltip_text ) o.rts_tooltip_text!.push_fa( " " );
        o.rts_tooltip_text!.push_fa( "<#<sc_t>>", bnameof( o.client.type() ) );
    }
    m$* rts_open_args = :open_args_s!^( o );
    o.mutex.unlock();

    o.rte.run( o.rtt_open.cast( bgfe_rte_fp_rtt ), o, rts_open_args );
    o.set_text_from_client_status();
    o.is_open = true;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

identifier gtk_button_new, gtk_button_new_with_label, gtk_button_set_relief;
type GTK_RELIEF_NORMAL;

func (:s) er_t rtt_open( m@* o, :open_args_s* args )
{
    o.mutex.create_lock()^;
    o.rtt_widget = gtk_box_new( GTK_ORIENTATION_HORIZONTAL, 0 );
    if( !o.rtt_widget ) = GERR_fa( "'gtk_box_new' failed\n" );
    if( G_IS_INITIALLY_UNOWNED( o.rtt_widget ) ) o.rtt_widget = g_object_ref_sink( o.rtt_widget );

    o.rtt_button_main = gtk_button_new();
    if( !o.rtt_button_main ) = GERR_fa( "'gtk_button_new' for rtt_button_main failed\n" );
    if( G_IS_INITIALLY_UNOWNED( o.rtt_button_main ) ) o.rtt_button_main = g_object_ref_sink( o.rtt_button_main );

    o.rtt_button_clear = gtk_button_new_with_label( "X" );
    if( !o.rtt_button_clear ) = GERR_fa( "'gtk_button_new' for rtt_button_clear failed\n" );
    if( G_IS_INITIALLY_UNOWNED( o.rtt_button_clear ) ) o.rtt_button_clear = g_object_ref_sink( o.rtt_button_clear );

    gtk_box_pack_start( GTK_BOX( o.rtt_widget ), o.rtt_button_main,  true, true, 0 );
    gtk_box_pack_start( GTK_BOX( o.rtt_widget ), o.rtt_button_clear, false, false, 0 );

    gtk_button_set_relief( GTK_BUTTON( o.rtt_button_main ), GTK_RELIEF_NORMAL );
    gtk_button_set_relief( GTK_BUTTON( o.rtt_button_clear ), GTK_RELIEF_NORMAL );

    gtk_widget_set_name( o.rtt_button_main,  ifnameof( o._ ) );
    gtk_widget_set_name( o.rtt_button_clear, ifnameof( o._ ) );
    gtk_widget_set_name( o.rtt_widget,       ifnameof( o._ ) );

    if( args.width > 0 || args.height > 0 ) gtk_widget_set_size_request( o.rtt_widget, args.width, args.height );
    if( o.rts_tooltip_text ) gtk_widget_set_tooltip_text( o.rtt_button_main, o.rts_tooltip_text.sc );
    gtk_widget_set_tooltip_text( o.rtt_button_clear, "clear" );

    gtk_widget_show( o.rtt_widget );
    gtk_widget_show( o.rtt_button_main );
    gtk_widget_show( o.rtt_button_clear );
    g_signal_connect( o.rtt_widget, "destroy", G_CALLBACK( :s_rtt_signal_destroy_widget ), o );
    g_signal_connect( o.rtt_button_main, "destroy", G_CALLBACK( :s_rtt_signal_destroy_button_main ), o );
    g_signal_connect( o.rtt_button_clear, "destroy", G_CALLBACK( :s_rtt_signal_destroy_button_clear ), o );

    g_signal_connect( o.rtt_button_main, "clicked", G_CALLBACK( :s_rtt_button_main_signal_clicked ), o );
    g_signal_connect( o.rtt_button_clear, "clicked", G_CALLBACK( :s_rtt_button_clear_signal_clicked ), o );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) close
{
    if( !o.is_open ) = 0;
    if( o.window ) o.window =< NULL;
    o.rte.run( o.rtt_close.cast( bgfe_rte_fp_rtt ), o, NULL );
    o.is_open = false;
    o.close_confirm();
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t rtt_close( m@* o, vd_t arg )
{
    if( o.rtt_button_main )
    {
        g_signal_handlers_disconnect_by_data( o.rtt_button_main, o );
        g_object_unref( o.rtt_button_main );
        o.rtt_button_main = NULL;
    }

    if( o.rtt_button_clear )
    {
        g_signal_handlers_disconnect_by_data( o.rtt_button_clear, o );
        g_object_unref( o.rtt_button_clear );
        o.rtt_button_clear = NULL;
    }

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
    if( o.is_open ) = GERR_fa( "Frame is open. Close it first." );

    o.client.tsm( client_type, client.fork() );
    o.client_name = client_name;

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) bgfe_frame.downsync_request = 0;
func (:s) bgfe_frame.downsync_confirm = 0;

//----------------------------------------------------------------------------------------------------------------------

func (:s) bgfe_frame.cycle
{
    if( !o.is_open ) = 0; // no error because frame window could have been closed

    o.mutex.lock();
    bl_t main_clicked = o.rts_main_clicked;
    bl_t clear_clicked = o.rts_clear_clicked;
    o.rts_main_clicked = false;
    o.rts_clear_clicked = false;
    o.mutex.unlock();

    if( main_clicked )
    {
        if( !o.client.o && o.client.type() )
        {
            tp_t link_action_type = action_type;
            if( link_action_type != approve~ )
            {
                if( o.parent ) o.parent().link_change_request( o, link_action_type.1 );
            }

            if( link_action_type == approve~ )
            {
                if( o.parent && o.parent.client() )
                {
                    o.client.tsm( o.client.type(), x_inst_create( o.client.type() ) );
                    x_stamp_t_set_sr( o.parent.client(), o.parent.client_type(), o.client_name, sr_tsm( o.client.type(), o.client.o.fork() ) );
                    o.set_text( st_s!^.copy_fa( "<#name>", o.client.type() ).sc );
                }
            }
        }

        if( !o.window && o.client.o )
        {
            o.window!.keep_above = true;
            o.window.set_client_with_content_t( o.client.o, o.client.type(), o.client_name );
            o.window.open( o.parent );
        }
    }

    if( clear_clicked )
    {
        if( o.window )
        {
            o.window.close();
            o.window =< NULL;
        }

        if( o.client.o && o.client.type() )
        {
            tp_t link_action_type = action_type;
            if( link_action_type != approve~ )
            {
                if( o.parent ) o.parent().link_change_request( o, link_action_type.1 );
            }

            if( link_action_type == approve~ )
            {
                if( o.parent && o.parent.client() )
                {
                    o.client.tsm( o.client.type(), NULL );
                    x_stamp_t_set_sr( o.parent.client(), o.parent.client_type(), o.client_name, sr_tsm( o.client.type(), o.client.o.fork() ) );
                    o.set_text_from_client_status();
                }
            }
        }
    }

    if( o.window )
    {
        o.window.cycle( action_type );
        if( !o.window.is_open ) o.window =< NULL;
    }
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) bgfe_frame.downsync
{
    if( !o.is_open ) = 0; // no error because frame window could have been closed
    if( o.window ) o.window.downsync();
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) bgfe_frame.upsync
{
    if( !o.is_open ) = 0; // no error because frame window could have been closed
    if( o.window ) o.window.upsync();
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
