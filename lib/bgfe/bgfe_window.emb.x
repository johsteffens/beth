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

identifier gtk_window_new, gtk_window_close, gtk_window_set_position, gtk_window_set_title, rtt_signal_delete_event;
identifier GTK_WINDOW_TOPLEVEL, GTK_WINDOW, GTK_WIN_POS_CENTER;
type GdkEvent, GdkEventFocus;

//----------------------------------------------------------------------------------------------------------------------

func (:s) void rtt_signal_destroy( m GtkWidget* win, m@* o ) o.rtt_widget = NULL;

// user destroy request
func (:s) gboolean rtt_signal_delete_event( m GtkWidget* win, m GdkEvent* event, m@* o )
{
    o.mutex.lock();
    o.rts_close_requested = true;
    o.mutex.unlock();
    verbatim_C{ return TRUE; } // prevents the default handler destroying the window
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) gboolean rtt_signal_focus_out_event( m GtkWidget* win, m GdkEvent* event, m@* o )
{
    if( o.close_on_lost_focus )
    {
        o.mutex.lock();
        o.rts_close_requested = true;
        o.mutex.unlock();
    }
    verbatim_C{ return FALSE; }
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) open
{
    if( o.is_open ) = 0;
    if( !o.frame ) = GERR_fa( "No frame defined." );

    bgfe_rte_get( &o.rte );

    if( o.frame._ == bgfe_frame_s~ ) o.frame.cast( m bgfe_frame_s* ).window = o;
    o.parent = parent;
    o.frame.open( o );

    o.rte.run( o.rtt_open.cast( bgfe_rte_fp_rtt ), o, NULL );
    o.is_open = true;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

identifier gtk_window_set_transient_for, GTK_WIN_POS_MOUSE;
identifier gtk_window_set_decorated, GTK_WIN_POS_MOUSE;
identifier gtk_widget_set_events;
type GTK_WINDOW_POPUP, GDK_FOCUS_CHANGE_MASK;

func (:s) er_t rtt_open( m@* o, vd_t unused )
{
    o.rtt_widget = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    if( !o.rtt_widget ) = GERR_fa( "'gtk_window_new' failed\n" );

    gtk_widget_set_hexpand( o.rtt_widget, true );
    gtk_widget_set_vexpand( o.rtt_widget, true );


    g_signal_connect( o.rtt_widget, "destroy", G_CALLBACK( bgfe_window_s_rtt_signal_destroy ), o );
    g_signal_connect( o.rtt_widget, "delete-event", G_CALLBACK( bgfe_window_s_rtt_signal_delete_event ), o );

    if( o.close_on_lost_focus )
    {
        gtk_widget_set_events( o.rtt_widget, GDK_FOCUS_CHANGE_MASK );
        g_signal_connect( o.rtt_widget, "focus-out-event", G_CALLBACK( bgfe_window_s_rtt_signal_focus_out_event ), o );
    }

    if( o.title )
    {
        gtk_window_set_title( GTK_WINDOW( o.rtt_widget ), o.title.sc );
    }
    else if( o.frame )
    {
        sc_t sc_client_name = bnameof( o.frame.client_name() );
        if( sc_client_name ) gtk_window_set_title( GTK_WINDOW( o.rtt_widget ), sc_client_name );
    }

    if( o.keep_above && o.frame && o.frame.parent() )
    {
        gtk_window_set_transient_for( GTK_WINDOW( o.rtt_widget ), GTK_WINDOW( o.frame.parent().nearest_window().rtt_widget ) );
    }

    gtk_window_set_decorated( GTK_WINDOW( o->rtt_widget ), o.decorated );
    //gtk_window_set_position( GTK_WINDOW( o->rtt_widget ), GTK_WIN_POS_CENTER );
    gtk_window_set_position( GTK_WINDOW( o->rtt_widget ), GTK_WIN_POS_MOUSE );

    if( o.frame ) gtk_container_add( GTK_CONTAINER( o.rtt_widget ), o.frame.rtt_widget() );

    gtk_widget_show( o.rtt_widget );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) close
{
    if( !o.is_open ) = 0;

    // the frame must be closed before closing/destroying the container
    if( o.frame ) o.frame.close();

    o.rte.run( o.rtt_close.cast( bgfe_rte_fp_rtt ), o, NULL );
    o.mutex.lock();
    o.rts_close_requested = false;
    o.mutex.unlock();
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
        gtk_widget_destroy( o.rtt_widget );
        o.rtt_widget = NULL;
    }
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) cycle
{
    if( !o.is_open ) = 0;
    if( o.frame ) o.frame.cycle( action_type );

    bl_t close_requested = false;
    o.mutex.lock();
    close_requested = o.rts_close_requested;
    o.mutex.unlock();

    if( close_requested )
    {
        if( o.client_close_ok() )
        {
            o.close();
        }
        else
        {
            o.mutex.lock();
            o.rts_close_requested = false;
            o.mutex.unlock();
        }
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) present
{
    if( !o.is_open ) = 0;
    = o.rte.run( o.rtt_present.cast( bgfe_rte_fp_rtt ), o, NULL );
}

//----------------------------------------------------------------------------------------------------------------------

identifier gtk_window_present;
func (:s) er_t rtt_present( m@* o, vd_t unused ) { gtk_window_present( GTK_WINDOW( o.rtt_widget ) ); = 0; }

//----------------------------------------------------------------------------------------------------------------------

func (:s) set_frame
{
    o.frame =< frame.fork();
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) set_frame_from_client_t
{
    if( !client ) = 0;

    tp_t frame_type = bgfe_frame_default_frame_type( client_type );
    tp_t action_type = escapprove~;

    o.client_edit_frame_type( client, client_type, client_name, action_type.1, frame_type );

    m bgfe_frame* frame = x_inst_create( frame_type ).cast( d bgfe_frame* )^;
    frame.set_show_border( false );
    frame.set_show_client_name( false );

    action_type = escapprove~;
    o.client_edit_frame( client, client_type, client_name, action_type.1, frame );

    o.set_frame( frame );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) set_client_t
{
    if( !o.frame ) o.set_frame_from_client_t( client, client_type, client_name );
    if(  o.frame ) = o.frame.set_client_t( client, client_type, client_name );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) set_client_with_content_t
{
    if( !o.frame ) o.set_frame_from_client_t( client, client_type, client_name );
    if(  o.frame ) = o.frame.set_client_with_content_t( client, client_type, client_name );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) add_content_t
{
    if( !o.frame ) o.frame =< bgfe_frame_s!;
    o.frame.set_show_border( false );
    o.frame.set_show_client_name( false );
    = o.frame.add_content_t( content, content_type, content_name );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
