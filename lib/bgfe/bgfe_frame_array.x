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
    tp_t arrange = 0; // (0 == auto) | vertical | horizontal
    sz_t min_content_width  = 80; // applies for horizontal orientation
    sz_t min_content_height = 80; // applies for vertical orientation
    sz_t max_content_width  = 200; // applies for horizontal orientation
    sz_t max_content_height = 200; // applies for vertical orientation
    bl_t show_client_name = true;
    st_s => widget_name;   // optional gtk widget name overrides default widget name
    sz_t spacing;   // spacing between elements
    bl_t end_bound; // packs elements with reference to the end of the box (false: reference to the start)
    bl_t center;    // centers widgets in an expanded space
    bl_t stretch;   // stretches elements to fill expanded space

    func bgfe_frame.set_width   { o.width   = value; = 0; }
    func bgfe_frame.set_height  { o.height  = value; = 0; }
    func bgfe_frame.set_arrange { o.arrange = arrange; = 0; }
    func bgfe_frame.set_min_content_width { o.min_content_width  = value; = 0; }
    func bgfe_frame.set_min_content_height{ o.min_content_height = value; = 0; }
    func bgfe_frame.set_max_content_width { o.max_content_width  = value; = 0; }
    func bgfe_frame.set_max_content_height{ o.max_content_height = value; = 0; }
    func bgfe_frame.set_show_client_name { o.show_client_name = flag; = 0; }
    func bgfe_frame.set_widget_name{ o.widget_name!.copy_sc( text ); = 0; }
    func bgfe_frame.set_spacing  { o.spacing   = value; = 0; }
    func bgfe_frame.set_end_bound{ o.end_bound = flag; = 0; }
    func bgfe_frame.set_center   { o.center    = flag; = 0; }
    func bgfe_frame.set_stretch  { o.stretch   = flag; = 0; }

    /// internals
    hidden bgfe_client* client; // client
    hidden tp_t client_type; // to cover obliv clients
    hidden tp_t client_name; // name under which the client's owner holds the client
    hidden bgfe_frame* parent;
    hidden vd_t array_base_address; // address of first array element used to determine if the array has been relocated between cycles
    hidden bl_t is_vertical; // array elements are ordered verically (vs. horizontally)

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
    hidden GtkWidget* rtt_gtk_list_box;
    hidden GtkWidget* rtt_gtk_scrolled_window;
    hidden GtkWidget* rtt_gtk_append_button;
    hidden bl_t rts_append_button_clicked;
    hidden bl_t is_open;

    func bcore_inst_call.down_e o.close();
    func bgfe_frame.rtt_widget = o.rtt_widget;
    func bgfe_frame.open;
    func bgfe_frame.close;
    func bgfe_frame.arrangement;
    func bgfe_frame.client_close_ok;
    func bgfe_frame.client_close_confirm;

    /// Interface functions ...

    func bgfe_frame.set_client_t;
    func bgfe_frame.cycle;
    func bgfe_frame.downsync;
    func bgfe_frame.upsync;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func (:s) bl_t rebuild_array_is_necessary( @* o )
{
    m x_array* array = o.client.cast( m x_array* );
    if( array.t_size( o.client_type ) != o.content_list.size ) = true;
    if( array.t_get_data_m( o.client_type ) != o.array_base_address ) = true;
    = false;
}

//----------------------------------------------------------------------------------------------------------------------

// appends an element to the array
func (:s) er_t append_element( m@* o )
{
    m x_array* array = o.client.cast( m x_array* );
    array.t_push( o.client_type );
    o.rebuild_array();
    o.rte.run( o.focus_to_end.cast( bgfe_rte_fp_rtt ), o, NULL );
    o.rte.run( o.rtt_scroll_to_end.cast( bgfe_rte_fp_rtt ), o, NULL );
    = false;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t rebuild_array( m@* o )
{
    o.content_list!.clear();

    m x_array* array = o.client.cast( m x_array* );
    sz_t size = array.t_size( o.client_type );
    o.array_base_address = array.t_get_data_m( o.client_type );

//    if( array.t_is_of_links( o.client_type ) )
//    {
//    }
//    else
//    {
        for( sz_t i = 0; i < size; i++ )
        {
            m$* sr_content = sr_s!^;
            sr_content.0 = array.t_m_get_sr( o.client_type, i );
            if( sr_content.o && sr_content.is_weak() )
            {
                tp_t frame_type = bgfe_frame_default_frame_type( sr_content.type() );
                tp_t action_type = escapprove~;

                o.client_edit_frame_type( sr_content.o, sr_content.type(), 0, action_type, frame_type );

                m bgfe_frame* frame = x_inst_create( frame_type ).cast( d bgfe_frame* )^;
                if( frame )
                {
                    frame.set_client_with_content_t( sr_content.o, sr_content.type(), 0 );
                    o.content_list!.push_d( frame.fork() );
                    frame.open( o );
                }
            }
        }
//    }


    o.rte.run( o.rtt_rebuild_array.cast( bgfe_rte_fp_rtt ), o, NULL );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

name gtk_container_remove;
func (:s) er_t rtt_rebuild_array( m@* o, vd_t unused )
{
    if( o.rtt_gtk_list_box != NULL )
    {
        gtk_container_remove( GTK_CONTAINER( o.rtt_gtk_scrolled_window ), o.rtt_gtk_list_box );
        g_signal_handlers_disconnect_by_data( o.rtt_gtk_list_box, o );
        g_object_unref( o.rtt_gtk_list_box );
        o.rtt_gtk_list_box = NULL;
    }

    {
        o.rtt_gtk_list_box = gtk_box_new( o.is_vertical ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL, o.spacing );
        if( !o.rtt_gtk_list_box ) = GERR_fa( "'gtk_box_new' failed\n" );
        if( G_IS_INITIALLY_UNOWNED( o.rtt_gtk_list_box ) ) o.rtt_gtk_list_box = g_object_ref_sink( o.rtt_gtk_list_box );
        gtk_widget_show( o.rtt_gtk_list_box );
        gtk_container_add( GTK_CONTAINER( o.rtt_gtk_scrolled_window ), o.rtt_gtk_list_box );
    }

    for( sz_t i = 0; i < o.content_list.size; i++ )
    {
        m$* frame = o.content_list.[ i ];
        if( frame )
        {
            if( o.end_bound )
            {
                gtk_box_pack_end( GTK_BOX( o.rtt_gtk_list_box ), frame.rtt_widget(), o.center, o.stretch, 0 );
            }
            else
            {
                gtk_box_pack_start( GTK_BOX( o.rtt_gtk_list_box ), frame.rtt_widget(), o.center, o.stretch, 0 );
            }
        }
    }

    // immediately updates the adjustment of scrolled window to the most recent size  (the automatic update seems to be delayed)
    gtk_scrolled_window_set_vadjustment( GTK_SCROLLED_WINDOW( o.rtt_gtk_scrolled_window ), NULL /*NULL creates a new adjustment*/ );
    gtk_scrolled_window_set_hadjustment( GTK_SCROLLED_WINDOW( o.rtt_gtk_scrolled_window ), NULL /*NULL creates a new adjustment*/ );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

type GtkAdjustment;
identifier gtk_scrolled_window_get_vadjustment, gtk_adjustment_set_value, gtk_adjustment_get_upper, gtk_adjustment_get_lower, gtk_adjustment_get_page_size;
identifier gtk_scrolled_window_get_hadjustment, gtk_scrolled_window_set_vadjustment, gtk_scrolled_window_set_hadjustment;
identifier gtk_widget_grab_focus;

func (:s) er_t rtt_scroll_to_end( m@* o, vd_t unused )
{
    if( o.is_vertical )
    {
        m GtkAdjustment* adjustment = gtk_scrolled_window_get_vadjustment( GTK_SCROLLED_WINDOW( o.rtt_gtk_scrolled_window ) );
        gtk_adjustment_set_value( adjustment, gtk_adjustment_get_upper( adjustment ) - gtk_adjustment_get_page_size( adjustment ) );
    }
    else
    {
        m GtkAdjustment* adjustment = gtk_scrolled_window_get_hadjustment( GTK_SCROLLED_WINDOW( o.rtt_gtk_scrolled_window ) );
        gtk_adjustment_set_value( adjustment, gtk_adjustment_get_upper( adjustment ) - gtk_adjustment_get_page_size( adjustment ) );
    }

    = 0;

}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t focus_to_end( m@* o, vd_t unused )
{
    if( o.content_list.size > 0 )
    {
        m bgfe_frame* frame  = o.content_list.[ o.content_list.size - 1 ];
        if( frame )
        {
            m GtkWidget* widget = frame.rtt_widget();
            gtk_widget_grab_focus( widget );
        }
    }
    = 0;
}

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
    bl_t fixed_size;

    func o _( m@* o, m :s* f )
    {
        o.width  = f.width;
        o.height = f.height;
        o.min_content_width  = f.min_content_width;
        o.min_content_height = f.min_content_height;
        o.max_content_width  = f.max_content_width;
        o.max_content_height = f.max_content_height;
        o.label_name = ( f.show_client_name && f.client_name && bnameof( f.client_name ) ) ? f.client_name : 0;
        o.content_list = f.content_list;
        o.fixed_size = x_array_t_is_fixed( f.client_type );
    }
}

func (:s) open
{
    if( o.is_open ) = 0;
    if( !o.client ) = GERR_fa( "No client defined. Call 'set_client' first." );

    bgfe_rte_get( &o.rte );

    o.parent = parent;

    tp_t arrangement = o.arrangement();

    o.mutex.lock();

    m$* rts_open_args = :open_args_s!^( o );
    o.is_vertical = ( arrangement == TYPEOF_vertical );

    o.mutex.unlock();

    o.rte.run( o.rtt_open.cast( bgfe_rte_fp_rtt ), o, rts_open_args );

    o.rebuild_array();
    o.is_open = true;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

identifier gtk_box_new, gtk_flow_box_new, gtk_widget_modify_bg, gtk_label_set_angle, gtk_box_pack_end, gtk_box_pack_start, gtk_scrolled_window_new;
identifier gtk_scrolled_window_set_propagate_natural_width, gtk_scrolled_window_set_propagate_natural_height;
identifier gtk_scrolled_window_set_max_content_width, gtk_scrolled_window_set_max_content_height;
identifier gtk_scrolled_window_set_min_content_width, gtk_scrolled_window_set_min_content_height;
identifier gtk_scrolled_window_set_policy, gtk_scrolled_window_set_overlay_scrolling;
identifier gtk_scrolled_window_set_kinetic_scrolling;
type GdkColor, GtkBox, GtkScrolledWindow, GTK_BOX, GTK_SCROLLED_WINDOW, GTK_POLICY_ALWAYS, GTK_POLICY_AUTOMATIC;

func (:s) void rtt_signal_append_button_clicked( m GtkWidget* win, m@* o )
{
    o.mutex.lock();
    o.rts_append_button_clicked = true;
    o.mutex.unlock();
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t rtt_open( m@* o, :open_args_s* args )
{
    o.mutex.create_lock()^;

    o.rtt_gtk_box = gtk_box_new( o.is_vertical ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL, 0 );
    if( !o.rtt_gtk_box ) = GERR_fa( "'gtk_box_new' failed\n" );
    if( G_IS_INITIALLY_UNOWNED( o.rtt_gtk_box ) ) o.rtt_gtk_box = g_object_ref_sink( o.rtt_gtk_box );
    gtk_widget_show( o.rtt_gtk_box );

    if( args.label_name )
    {
        m GtkWidget* label = gtk_label_new( bnameof( args.label_name ) );
        if( !label ) = GERR_fa( "'gtk_label_new' failed\n" );
        gtk_widget_set_name( label, "client_name" );
        gtk_label_set_angle( GTK_LABEL( label ), o.is_vertical ? 0 : 90 );
        gtk_box_pack_start( GTK_BOX( o.rtt_gtk_box ), label, false, false, 0 );
        gtk_widget_show( label );
    }

    o.rtt_gtk_scrolled_window = gtk_scrolled_window_new( NULL, NULL );
    if( !o.rtt_gtk_scrolled_window ) = GERR_fa( "'gtk_scrolled_window_new' failed\n" );
    if( G_IS_INITIALLY_UNOWNED( o.rtt_gtk_scrolled_window ) ) o.rtt_gtk_scrolled_window = g_object_ref_sink( o.rtt_gtk_scrolled_window );

    gtk_scrolled_window_set_propagate_natural_width( GTK_SCROLLED_WINDOW( o.rtt_gtk_scrolled_window ), o.is_vertical ? true : false );
    gtk_scrolled_window_set_propagate_natural_height( GTK_SCROLLED_WINDOW( o.rtt_gtk_scrolled_window ), o.is_vertical ? false : true );

    // overlay scrolling does not seem to work reliably in this context
    gtk_scrolled_window_set_overlay_scrolling( GTK_SCROLLED_WINDOW( o.rtt_gtk_scrolled_window ), false );
    gtk_scrolled_window_set_policy( GTK_SCROLLED_WINDOW( o.rtt_gtk_scrolled_window ), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC );

    if( o.is_vertical )
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

    gtk_widget_set_name( o.rtt_gtk_box, o.widget_name ? o.widget_name.sc : ifnameof( o._ ) );
    gtk_widget_set_size_request( o.rtt_gtk_box, args.width, args.height );

    gtk_box_pack_start( GTK_BOX( o.rtt_gtk_box ), o.rtt_gtk_scrolled_window, true, true, 0 );

    if( !args.fixed_size )
    {
        o.rtt_gtk_append_button = gtk_button_new_with_label( "+" );
        if( !o.rtt_gtk_append_button ) = GERR_fa( "'gtk_button_new_with_label' failed\n" );
        if( G_IS_INITIALLY_UNOWNED( o.rtt_gtk_append_button ) ) o.rtt_gtk_append_button = g_object_ref_sink( o.rtt_gtk_append_button );
        g_signal_connect( o.rtt_gtk_append_button, "clicked", G_CALLBACK( :s_rtt_signal_append_button_clicked ), o );
        gtk_widget_show( o.rtt_gtk_append_button );
    }

    gtk_box_pack_start( GTK_BOX( o.rtt_gtk_box ), o.rtt_gtk_append_button, false, false, 0 );

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

    if( o.rtt_gtk_list_box )
    {
        g_signal_handlers_disconnect_by_data( o.rtt_gtk_list_box, o );
        g_object_unref( o.rtt_gtk_list_box );
        o.rtt_gtk_list_box = NULL;
    }

    if( o.rtt_gtk_scrolled_window )
    {
        g_signal_handlers_disconnect_by_data( o.rtt_gtk_scrolled_window, o );
        g_object_unref( o.rtt_gtk_scrolled_window );
        o.rtt_gtk_scrolled_window = NULL;
    }

    if( o.rtt_gtk_append_button )
    {
        g_signal_handlers_disconnect_by_data( o.rtt_gtk_append_button, o );
        g_object_unref( o.rtt_gtk_append_button );
        o.rtt_gtk_append_button = NULL;
    }

    o.rtt_widget = 0;

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) cycle
{
    if( !o.is_open ) = 0; // no error because frame window could have been closed

    /** The array is automatically fully resynchronized when its size or memory location has changed
     *  between cycles. This is necessary to avoid temporary memory inconsistencies of client references
     *  in content frames
     */
    if( o.rebuild_array_is_necessary() ) o.rebuild_array();

    o.mutex.lock();
    bl_t append_button_clicked = o.rts_append_button_clicked;
    o.rts_append_button_clicked = false;
    o.mutex.unlock();

    if( append_button_clicked ) o.append_element(); // resyncs automatically

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

    if( o.rebuild_array_is_necessary() )
    {
        o.rebuild_array(); // this includes upsyncing the array
    }
    else
    {
        foreach( m$* e in o.content_list ) e.upsync(); // upsync only elements
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) client_close_ok
{
    if( o.client )
    {
        if( !o.client.t_bgfe_close_ok( o.client_type ) ) = false;
    }

    foreach( m$* e in o.content_list )
    {
        if( !e.client_close_ok() ) = false;
    }

    = true;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) client_close_confirm
{
    if( o.client ) o.client.t_bgfe_close_confirm( o.client_type );
    foreach( m$* e in o.content_list ) e.client_close_confirm();
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
    if( o.is_open ) = GERR_fa( "Frame is open." );

    o.content_list =< NULL;

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

    // the array content is added when the frame is opened

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

