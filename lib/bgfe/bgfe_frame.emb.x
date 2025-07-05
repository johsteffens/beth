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

func (:s) void rtt_signal_destroy_gtk_box( m GtkWidget* win, m@* o ) o.rtt_gtk_box = NULL;

//----------------------------------------------------------------------------------------------------------------------

func (:s) open
{
    if( o.is_open ) = 0;

    bgfe_rte_get( &o.rte );

    o.parent = parent;

    f3_t max_h_complexity = 0;
    f3_t max_v_complexity = 0;
    f3_t sum_h_complexity = 0;
    f3_t sum_v_complexity = 0;
    f3_t overhead_h_complexity = 0.5;
    f3_t overhead_v_complexity = 0.5;

    /// tendency of clients >0 horizontal; < 0 vertical
    sz_t h_tendency = 0;

    foreach( m$* e in o.content_list )
    {
        e.open( o );
        f3_t h_complexity = e.h_complexity();
        f3_t v_complexity = e.v_complexity();
        max_h_complexity = f3_max( max_h_complexity, h_complexity );
        max_v_complexity = f3_max( max_v_complexity, v_complexity );
        sum_h_complexity += h_complexity;
        sum_v_complexity += v_complexity;
        switch( e.arrangement() )
        {
            case TYPEOF_horizontal: h_tendency++; break;
            case TYPEOF_vertical  : h_tendency--; break;
            default: break;
        }
    }

    // complexity on vertical orientation
    f3_t ver_h_complexity = max_h_complexity + overhead_h_complexity * 1;
    f3_t ver_v_complexity = sum_v_complexity + overhead_v_complexity * 2;

    // complexity on horizontal orientation
    f3_t hor_h_complexity = sum_h_complexity + overhead_h_complexity * 2;
    f3_t hor_v_complexity = max_v_complexity + overhead_v_complexity * 1;

    if( o.arrange == TYPEOF_vertical )
    {
        o.vertical = true;
    }
    else if( o.arrange == TYPEOF_horizontal )
    {
        o.vertical = false;
    }
    else if( o.arrange == TYPEOF_square )
    {
        f3_t ver_ratio = f3_min( ver_h_complexity, ver_v_complexity ) / f3_max( ver_h_complexity, ver_v_complexity );
        f3_t hor_ratio = f3_min( hor_h_complexity, hor_v_complexity ) / f3_max( hor_h_complexity, hor_v_complexity );
        o.vertical = ( ver_ratio > hor_ratio );
        bcore_msg_fa( "#name: hor_ratio: #<f3_t>, ver_ratio: #<f3_t>\n", o.client_name, hor_ratio, ver_ratio );
    }
    else if( o.arrange == TYPEOF_hor_golden_ratio )
    {
        f3_t ver_log_ratio = f3_log( ver_h_complexity ) - f3_log( ver_v_complexity );
        f3_t hor_log_ratio = f3_log( hor_h_complexity ) - f3_log( hor_v_complexity );
        o.vertical = ( f3_sqr( ver_log_ratio - f3_log( 1.618 ) ) < f3_sqr( hor_log_ratio - f3_log( 1.618 ) ) );
        bcore_msg_fa( "#name: ver_log_ratio: #<f3_t>, hor_log_ratio: #<f3_t>\n", o.client_name, ver_log_ratio, hor_log_ratio );
    }
    else if( o.arrange == TYPEOF_ver_golden_ratio )
    {
        f3_t ver_log_ratio = f3_log( ver_v_complexity ) - f3_log( ver_h_complexity );
        f3_t hor_log_ratio = f3_log( hor_v_complexity ) - f3_log( hor_h_complexity );
        o.vertical = ( f3_sqr( ver_log_ratio - f3_log( 1.618 ) ) < f3_sqr( hor_log_ratio - f3_log( 1.618 ) ) );
        bcore_msg_fa( "#name: ver_log_ratio: #<f3_t>, hor_log_ratio: #<f3_t>\n", o.client_name, ver_log_ratio, hor_log_ratio );
    }
    else if( o.arrange == TYPEOF_min_volume )
    {
        f3_t ver_volume = ver_v_complexity * ver_h_complexity;
        f3_t hor_volume = hor_v_complexity * hor_h_complexity;
        o.vertical = ( ver_volume < hor_volume );
        bcore_msg_fa( "#name: ver_volume: #<f3_t>, hor_volume: #<f3_t>\n", o.client_name, ver_volume, hor_volume );
    }
    else
    {
        o.vertical = ( h_tendency >= 0 );
    }

    if( o.vertical )
    {
        o.h_complexity = ver_h_complexity;
        o.v_complexity = ver_v_complexity;
    }
    else
    {
        o.h_complexity = hor_h_complexity;
        o.v_complexity = hor_v_complexity;
    }

    bcore_msg_fa( "#name: h_complexity: #<f3_t>, v_complexity: #<f3_t>\n", o.client_name, o.h_complexity, o.v_complexity );

    o.rte.run( o.rtt_open.cast( bgfe_rte_fp_rtt ), o, NULL );

    o.is_open = true;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

identifier gtk_box_new, gtk_flow_box_new, gtk_widget_modify_bg, gtk_label_set_angle, gtk_box_pack_end, gtk_box_pack_start;
type GdkColor, GtkBox, GTK_BOX;

func (:s) er_t rtt_open( m@* o, vd_t unused )
{
    bl_t vertical = o.vertical;

    o.rtt_attach_widget( gtk_box_new( vertical ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL, o.spacing ), o.rtt_gtk_box );
    gtk_widget_show( o.rtt_gtk_box );

    tp_t label_name = ( o.show_client_name && o.client_name && bnameof( o.client_name ) ) ? o.client_name : 0;

    if( label_name )
    {
        m GtkWidget* label = gtk_label_new( bnameof( label_name ) );
        if( !label ) = GERR_fa( "'gtk_label_new' failed\n" );
        gtk_widget_set_name( label, "client_name" );
        gtk_label_set_angle( GTK_LABEL( label ), vertical ? 0 : 90 );
        gtk_box_pack_start( GTK_BOX( o.rtt_gtk_box ), label, false, false, 0 );
        gtk_widget_show( label );
    }

    gtk_widget_set_name( o.rtt_gtk_box, o.widget_name ? o.widget_name.sc : ifnameof( o._ ) );
    gtk_widget_set_size_request( o.rtt_gtk_box, o.width, o.height );

    g_signal_connect( o.rtt_gtk_box, "destroy", G_CALLBACK( :s_rtt_signal_destroy_gtk_box ), o );

    foreach( m$* e in o.content_list )
    {
        if( o.end_bound )
        {
            gtk_box_pack_end( GTK_BOX( o.rtt_gtk_box ), e.rtt_widget(), o.center, o.stretch, 0 );
        }
        else
        {
            gtk_box_pack_start( GTK_BOX( o.rtt_gtk_box ), e.rtt_widget(), o.center, o.stretch, 0 );
        }
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
    o.rtt_detach_widget( o.rtt_gtk_box );
    o.rtt_widget = NULL;
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
    o.set_client_t( client, client_type, client_name );

    if( !client ) = 0;

    sz_t size = x_stamp_t_size( client_type );
    for( sz_t i = 0; i < size; i++ )
    {
        tp_t content_name = x_stamp_t_name( client_type, i );

        if( x_stamp_t_is_link_i( client_type, i ) )
        {
            o.add_linked_content( content_name );
        }
        else
        {
            m$* content_sr = sr_s!^;
            content_sr.0 = x_stamp_t_m_get_sr_i( client, client_type, i );

            // only add weak references; strong references will not survive
            if( content_sr.is_weak() )
            {
                o.add_content_t( content_sr.o, content_sr.type(), content_name );
            }
        }
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

    if( x_stamp_t_is_pure_array( content_type ) )
    {
        m$* frame_array = :array_s!^;
        frame_array.set_client_t( content, content_type, content_name );
        tp_t action_type = escapprove~;
        o.client_edit_frame( content, content_type, content_name, action_type, frame_array );
        o.content_list!.push_d( frame_array.fork() );
    }
    else
    {
        tp_t frame_type = bgfe_frame_default_frame_type( content_type );
        tp_t action_type = escapprove~;
        o.client_edit_frame_type( content, content_type, content_name, action_type.1, frame_type );
        m bgfe_frame* frame = x_inst_create( frame_type ).cast( d bgfe_frame* )^;
        action_type = escapprove~;
        o.client_edit_frame( content, content_type, content_name, action_type.1, frame );
        if( frame )
        {
            frame.set_client_with_content_t( content, content_type, content_name );
            o.content_list!.push_d( frame.fork() );
        }
    }
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) add_linked_content
{
    if( !o.client ) = 0;

    m$* frame_link = :link_s!^;

    m$* content_sr = sr_s!^;
    content_sr.0 = x_stamp_t_m_get_sr( o.client, o.client_type, content_name );

    if( content_sr.o )
    {
        frame_link.set_client_t( content_sr.o, content_sr.type(), content_name );
    }
    else
    {
        tp_t content_type = x_stamp_t_type( o.client, o.client_type, content_name );
        frame_link.set_client_t( NULL, content_type, content_name );
    }

    frame_link.set_holder_t( o.client, o.client_type, o.client_name, false, 0 );

    o.content_list!.push_d( frame_link.fork() );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) add_frame
{
    o.content_list!.push_d( frame.fork() );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
