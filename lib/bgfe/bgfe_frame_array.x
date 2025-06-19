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

/** Frame for displaying/editing arrays.
 */

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp :s bgfe_frame
{
    /// parameters
    sz_t width;   // optional preset width
    sz_t height;  // optional preset height
    sz_t min_content_width  = 80; // applies for horizontal orientation
    sz_t min_content_height = 80; // applies for vertical orientation
    sz_t max_content_width  = 200; // applies for horizontal orientation
    sz_t max_content_height = 200; // applies for vertical orientation
    bl_t hide_client_name; // true: does not display client name
    tp_t arrange; // (0 == auto) | vertical | horizontal

    /// internals
    hidden bgfe_client* client; // client
    hidden tp_t client_type; // to cover obliv clients
    hidden tp_t client_name; // name under which the client's owner holds the client
    hidden bgfe_frame* parent;
    func bgfe_frame.client = o.client;
    func bgfe_frame.client_type = o.client_type;
    func bgfe_frame.client_name = o.client_name;
    func bgfe_frame.parent = o.parent;
    func bgfe_frame.is_open = o.is_open;

    hidden bgfe_frame_list_s => content_list;
    hidden bgfe_rte_s* rte;
    hidden x_mutex_s mutex;
    hidden GtkWidget* rtt_widget;
    hidden GtkWidget* rtt_gtk_box;
    hidden GtkWidget* rtt_gtk_scrolled_window;
    hidden bl_t is_open;

    func bcore_inst_call.down_e o.close();
    func bgfe_frame.rtt_widget = o.rtt_widget;
    func bgfe_frame.open;
    func bgfe_frame.close;
    func bgfe_frame.close_ok;
    func bgfe_frame.close_confirm;
    func bgfe_frame.arrangement;

    /// Interface functions ...

    func bgfe_frame.set_client_t;
    func bgfe_frame.cycle;
    func bgfe_frame.downsync;
    func bgfe_frame.upsync;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func (:s) void rtt_signal_destroy_gtk_box            ( m GtkWidget* win, m@* o ) o.rtt_gtk_box = NULL;
func (:s) void rtt_signal_destroy_gtk_scrolled_window( m GtkWidget* win, m@* o ) o.rtt_gtk_scrolled_window = NULL;

//----------------------------------------------------------------------------------------------------------------------

stamp :open_args_s
{
    hidden bgfe_frame_list_s* content_list;
    sz_t width;
    sz_t height;
    sz_t min_content_width;
    sz_t min_content_height;
    sz_t max_content_width;
    sz_t max_content_height;
    tp_t label_name;

    func o _( m@* o, m :s* f )
    {
        o.width  = f.width;
        o.height = f.height;
        o.min_content_width  = f.min_content_width;
        o.min_content_height = f.min_content_height;
        o.max_content_width  = f.max_content_width;
        o.max_content_height = f.max_content_height;
        o.label_name = ( !f.hide_client_name && f.client_name && bnameof( f.client_name ) ) ? f.client_name : 0;
        o.content_list = f.content_list;
    }
}

func (:s) open
{
    if( o.is_open ) = 0;
    if( !o.client ) = GERR_fa( "No client defined. Call 'set_client' first." );

    bgfe_rte_get( &o.rte );

    o.parent = parent;

    o.mutex.lock();

    m$* rts_open_args = :open_args_s!^( o );

    o.mutex.unlock();

    foreach( m$* e in o.content_list ) e.open( o );

    o.rte.run( o.rtt_open.cast( bgfe_rte_fp_rtt ), o, rts_open_args );

    o.is_open = true;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

identifier gtk_box_new, gtk_flow_box_new, gtk_widget_modify_bg, gtk_label_set_angle, gtk_box_pack_end, gtk_box_pack_start, gtk_scrolled_window_new;
identifier gtk_scrolled_window_set_propagate_natural_width, gtk_scrolled_window_set_propagate_natural_height;
identifier gtk_scrolled_window_set_max_content_width, gtk_scrolled_window_set_max_content_height;
identifier gtk_scrolled_window_set_min_content_width, gtk_scrolled_window_set_min_content_height;
type GdkColor, GtkBox, GtkScrolledWindow, GTK_BOX, GTK_SCROLLED_WINDOW;

func (:s) er_t rtt_open( m@* o, :open_args_s* args )
{
    o.mutex.create_lock()^;

    tp_t arrangement = o.arrangement();

    bl_t vertical = arrangement == TYPEOF_vertical;

    o.rtt_gtk_box = gtk_box_new( vertical ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL, 0 );
    if( !o.rtt_gtk_box ) = GERR_fa( "'gtk_box_new' failed\n" );
    if( G_IS_INITIALLY_UNOWNED( o.rtt_gtk_box ) ) o.rtt_gtk_box = g_object_ref_sink( o.rtt_gtk_box );
    gtk_widget_show( o.rtt_gtk_box );

    if( args.label_name )
    {
        m GtkWidget* label = gtk_label_new( bnameof( args.label_name ) );
        if( !label ) = GERR_fa( "'gtk_label_new' failed\n" );
        gtk_widget_set_name( label, "client_name" );
        gtk_label_set_angle( GTK_LABEL( label ), vertical ? 0 : 90 );
        gtk_box_pack_start( GTK_BOX( o.rtt_gtk_box ), label, false, false, 0 );
        gtk_widget_show( label );
    }

    o.rtt_gtk_scrolled_window = gtk_scrolled_window_new( NULL, NULL );
    if( !o.rtt_gtk_scrolled_window ) = GERR_fa( "'gtk_scrolled_window_new' failed\n" );
    if( G_IS_INITIALLY_UNOWNED( o.rtt_gtk_scrolled_window ) ) o.rtt_gtk_scrolled_window = g_object_ref_sink( o.rtt_gtk_scrolled_window );

    gtk_scrolled_window_set_propagate_natural_width( GTK_SCROLLED_WINDOW( o.rtt_gtk_scrolled_window ), vertical ? true : false );
    gtk_scrolled_window_set_propagate_natural_height( GTK_SCROLLED_WINDOW( o.rtt_gtk_scrolled_window ), vertical ? false : true );

    if( vertical )
    {
        gtk_scrolled_window_set_min_content_height( GTK_SCROLLED_WINDOW( o.rtt_gtk_scrolled_window ), args.min_content_height );
        gtk_scrolled_window_set_max_content_height( GTK_SCROLLED_WINDOW( o.rtt_gtk_scrolled_window ), args.max_content_height );
    }
    else
    {
        gtk_scrolled_window_set_min_content_width( GTK_SCROLLED_WINDOW( o.rtt_gtk_scrolled_window ), args.min_content_width );
        gtk_scrolled_window_set_max_content_width( GTK_SCROLLED_WINDOW( o.rtt_gtk_scrolled_window ), args.max_content_width );
    }

    gtk_widget_show( o.rtt_gtk_scrolled_window );

    gtk_widget_set_name( o.rtt_gtk_box, ifnameof( o._ ) );
    gtk_widget_set_size_request( o.rtt_gtk_box, args.width, args.height );

    g_signal_connect( o.rtt_gtk_box,             "destroy", G_CALLBACK( :s_rtt_signal_destroy_gtk_box             ), o );
    g_signal_connect( o.rtt_gtk_scrolled_window, "destroy", G_CALLBACK( :s_rtt_signal_destroy_gtk_scrolled_window ), o );

    gtk_box_pack_start( GTK_BOX( o.rtt_gtk_box ), o.rtt_gtk_scrolled_window, true, true, 0 );

    m GtkWidget* list_box = gtk_box_new( vertical ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL, 0 );
    if( !list_box ) = GERR_fa( "'gtk_box_new' failed\n" );
    gtk_widget_show( list_box );

    gtk_container_add( GTK_CONTAINER( o.rtt_gtk_scrolled_window ), list_box );

    foreach( m$* e in args.content_list )
    {
        gtk_box_pack_start( GTK_BOX( list_box ), e.rtt_widget(), false, false, 0 );
    }

    o.rtt_widget = o.rtt_gtk_box;

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) close
{
    if( !o.is_open ) = 0;

    foreach( m$* e in o.content_list ) e.close();

    o.rte.run( o.rtt_close.cast( bgfe_rte_fp_rtt ), o, NULL );
    o.is_open = false;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t rtt_close( m@* o, vd_t arg )
{
    if( o.rtt_gtk_box )
    {
        g_signal_handlers_disconnect_by_data( o.rtt_gtk_box, o );
        g_object_unref( o.rtt_gtk_box );
        o.rtt_gtk_box = NULL;
    }

    if( o.rtt_gtk_scrolled_window )
    {
        g_signal_handlers_disconnect_by_data( o.rtt_gtk_scrolled_window, o );
        g_object_unref( o.rtt_gtk_scrolled_window );
        o.rtt_gtk_scrolled_window = NULL;
    }

    o.rtt_widget = 0;

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) cycle
{
    if( !o.is_open ) = 0; // no error because frame window could have been closed
    foreach( m$* e in o.content_list ) e.cycle( action_type );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) downsync
{
    if( !o.is_open ) = 0; // no error because frame window could have been closed
    foreach( m$* e in o.content_list ) e.downsync();
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) upsync
{
    if( !o.is_open ) = 0; // no error because frame window could have been closed
    foreach( m$* e in o.content_list ) e.upsync();
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) close_ok
{
    if( o.client )
    {
        if( !o.client.t_frame_close_ok( o.client_type ) ) = false;
    }

    foreach( m$* e in o.content_list )
    {
        if( !e.close_ok() ) = false;
    }

    = true;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) close_confirm
{
    if( o.client ) o.client.t_frame_close_confirm( o.client_type );
    foreach( m$* e in o.content_list ) e.close_confirm();
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) arrangement
{
    if( o.arrange ) = o.arrange;

    /// >0 horizontal; < 0 vertical
    sz_t clients_tendency = 0;

    foreach( m$* e in o.content_list )
    {
        switch( e.arrangement() )
        {
            case TYPEOF_horizontal: clients_tendency++; break;
            case TYPEOF_vertical  : clients_tendency--; break;
            default: break;
        }
    }

    // the chosen arrangement is the opposite of the clients tendency;
    = ( clients_tendency >= 0 ) ? TYPEOF_vertical : TYPEOF_horizontal;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) set_client_t
{
    if( o.is_open ) = GERR_fa( "Frame is open. Close it first." );

    if( !client ) // reset
    {
        o.client = NULL;
        o.client_type = 0;
        o.client_name = 0;
        = 0;
    }

    o.client      = client;
    o.client_type = client_type;
    o.client_name = client_name;

    m x_array* array = o.client.cast( m x_array* );

    sz_t size = array.t_size( o.client_type );

    for( sz_t i = 0; i < size; i++ )
    {
        m$* content_sr = sr_s!^;
        content_sr.0 = array.t_m_get_sr( o.client_type, i );

        // only add weak references; strong references will not survive
        if( content_sr.o && content_sr.is_weak() )
        {
            d bgfe_frame* d_frame = NULL;
            bgfe_frame_create_default_frame_t( content_sr.o, content_sr.type(), 0, d_frame.2 );
            m$* frame = d_frame.1^;
            if( frame ) o.content_list!.push_d( frame.fork() );
        }
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

