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

//----------------------------------------------------------------------------------------------------------------------

func (:s) void rtt_signal_destroy( m GtkWidget* win, m@* o ) o.rtt_widget = NULL;

//----------------------------------------------------------------------------------------------------------------------

stamp :open_args_s
{
    hidden :list_s* content_list;
    sz_t width;
    sz_t height;
    tp_t label_name;

    func o _( m@* o, m :s* f )
    {
        o.width  = f.width;
        o.height = f.height;
        o.label_name = ( !f.hide_client_name && f.client_name && bnameof( f.client_name ) ) ? f.client_name : 0;
        o.content_list = f.content_list;
    }
}

func (:s) open
{
    if( o.is_open ) = 0;
    if( !o.client ) = GERR_fa( "No client defined. Call 'setup' first." );

    bgfe_rte_get( &o.rte );

    ASSERT( parent );
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

identifier gtk_box_new, gtk_flow_box_new, gtk_widget_modify_bg, gtk_label_set_angle, gtk_box_pack_end, gtk_box_pack_start;
type GdkColor, GtkBox, GTK_BOX;

func (:s) er_t rtt_open( m@* o, :open_args_s* args )
{
    o.mutex.create_lock()^;

    tp_t arrangement = o.arrangement();

    bl_t vertical = arrangement == TYPEOF_vertical;

    o.rtt_widget = gtk_box_new( vertical ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL, 0 );
    if( !o.rtt_widget ) = GERR_fa( "'gtk_box_new' failed\n" );
    if( G_IS_INITIALLY_UNOWNED( o.rtt_widget ) ) o.rtt_widget = g_object_ref_sink( o.rtt_widget );
    gtk_widget_show( o.rtt_widget );

    if( args.label_name )
    {
        m GtkWidget* label = gtk_label_new( bnameof( args.label_name ) );
        if( !label ) = GERR_fa( "'gtk_label_new' failed\n" );
        gtk_widget_set_name( label, "client_name" );
        gtk_label_set_angle( GTK_LABEL( label ), vertical ? 0 : 90 );
        gtk_container_add( GTK_CONTAINER( o.rtt_widget ), label );
        gtk_widget_show( label );
    }

    gtk_widget_set_name( o.rtt_widget, ifnameof( o._ ) );
    gtk_widget_set_size_request( o.rtt_widget, args.width, args.height );
//    gtk_widget_set_hexpand( o.rtt_widget, true );
//    gtk_widget_set_vexpand( o.rtt_widget, true );

    g_signal_connect( o.rtt_widget, "destroy", G_CALLBACK( :s_rtt_signal_destroy ), o );

    foreach( m$* e in args.content_list )
    {
        gtk_box_pack_start( GTK_BOX( o.rtt_widget ), e.rtt_widget(), false, false, 0 );
    }

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
    if( o.rtt_widget )
    {
        g_signal_handlers_disconnect_by_data( o.rtt_widget, o );
        g_object_unref( o.rtt_widget );
        o.rtt_widget = NULL;
    }
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

func (:s) get_builder
{
    if     ( o.builder ) builder.1 = o.builder;
    else if( o.parent  ) o.parent.get_builder( builder );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) build_frame_t
{
    if( !o.local_builder )
    {
        m bgfe_builder* builder = NULL;
        o.get_builder( builder.2 );
        if( builder )
        {
            o.local_builder =< builder.fork();
        }
        else
        {
            o.local_builder =< bgfe_builder_s!;
        }
    }

    = o.local_builder.build_frame_t( client, client_type, client_name, frame );
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
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) set_client_with_content
{
    if( client )
    {
        = o.set_client_with_content_t( client, client._, client_name );
    }
    else
    {
        = o.set_client_with_content_t( NULL, 0, client_name ); // reset
    }
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) set_client_with_content_t
{
    if( !client ) = 0;

    o.set_client_t( client, client_type, client_name );

    sz_t size = x_stamp_t_size( client_type );
    for( sz_t i = 0; i < size; i++ )
    {
        tp_t content_name = x_stamp_t_name( client_type, i );
        m$* content_sr = sr_s!^;
        content_sr.0 = x_stamp_t_m_get_sr_i( client, client_type, i );

        // only add weak references; strong references will not survive
        if( content_sr.is_weak() ) o.add_content_t( content_sr.o, content_sr.type(), content_name );
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) add_content
{
    if( !content ) = 0;
    = o.add_content_t( content, content._, content_name );
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) add_content_t
{
    if( o.is_open ) = GERR_fa( "Frame is open. Close it first." );

    d :* d_frame = NULL;
    o.build_frame_t( content, content_type, content_name, d_frame.2 );
    m$* frame = d_frame.1^;
    if( frame ) o.content_list!.push_d( frame.fork() );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
