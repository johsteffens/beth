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
type GdkEvent;

//----------------------------------------------------------------------------------------------------------------------

func (:s) void rtt_signal_destroy( m GtkWidget* win, m@* o ) o.rtt_widget = NULL;

// user destroy request
func (:s) gboolean rtt_signal_delete_event( m GtkWidget* win, m GdkEvent* event, m@* o )
{
    bcore_msg_fa( "rtt_signal_delete_event\n" );
    o.mutex.lock();
    o.rts_close_requested = true;
    o.mutex.unlock();
    verbatim_C{ return TRUE; } // prevents the default handler destroying the window
}

//----------------------------------------------------------------------------------------------------------------------

stamp :open_args_s
{
    sc_t sc_title;
    bl_t keep_above;
    private GtkWidget* child_widget;
    private :s* nearest_window;

    func o _( m@* o, m :s* f )
    {
        o.sc_title = f.title ? f.title.sc : NULL;
        o.child_widget = f.frame ? f.frame.rtt_widget() : NULL;
        o.keep_above = f.keep_above;
        o.nearest_window = ( f.frame && f.frame.parent ) ? f.frame.parent.nearest_window() : NULL;
    }
}

func (:s) open
{
    if( o.is_open ) = 0;

    tp_t action_type = escapprove~;

    if( o.frame.client ) o.frame.client_edit_frame( o.frame.client, o.frame.client_type, o.frame.client_name, action_type.1, o.frame );

    bgfe_rte_get( &o.rte );

    o.frame.window = o;
    o.frame.open( parent );

    o.mutex.lock();
    m$* rts_open_args = :open_args_s!^( o );
    o.mutex.unlock();

    o.rte.run( o.rtt_open.cast( bgfe_rte_fp_rtt ), o, rts_open_args );
    o.is_open = true;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------
name gtk_window_set_transient_for;

func (:s) er_t rtt_open( m@* o, :open_args_s* rts_open_args )
{
    o.rtt_widget = gtk_window_new( GTK_WINDOW_TOPLEVEL );
    if( !o.rtt_widget ) = GERR_fa( "'gtk_window_new' failed\n" );

    gtk_widget_set_hexpand( o.rtt_widget, true );
    gtk_widget_set_vexpand( o.rtt_widget, true );

    g_signal_connect( o.rtt_widget, "destroy", G_CALLBACK( bgfe_window_s_rtt_signal_destroy ), o );
    g_signal_connect( o.rtt_widget, "delete-event", G_CALLBACK( bgfe_window_s_rtt_signal_delete_event ), o );

    o.mutex.lock();
    if( rts_open_args.sc_title ) gtk_window_set_title( GTK_WINDOW( o.rtt_widget ), rts_open_args.sc_title );

    if( rts_open_args.keep_above && rts_open_args.nearest_window )
    {
        gtk_window_set_transient_for( GTK_WINDOW( o.rtt_widget ), GTK_WINDOW( rts_open_args.nearest_window.rtt_widget ) );
    }

    o.mutex.unlock();

    gtk_window_set_position( GTK_WINDOW( o->rtt_widget ), GTK_WIN_POS_CENTER );

    if( rts_open_args.child_widget ) gtk_container_add( GTK_CONTAINER( o.rtt_widget ), rts_open_args.child_widget );

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
        //gtk_window_close( GTK_WINDOW( o.rtt_widget ) );
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
        bcore_msg_fa( "#name: close_requested\n", o._ );
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

/**********************************************************************************************************************/
