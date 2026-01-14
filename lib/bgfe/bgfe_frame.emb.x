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
    }
    else if( o.arrange == TYPEOF_hor_golden_ratio )
    {
        f3_t ver_log_ratio = f3_log( ver_h_complexity ) - f3_log( ver_v_complexity );
        f3_t hor_log_ratio = f3_log( hor_h_complexity ) - f3_log( hor_v_complexity );
        o.vertical = ( f3_sqr( ver_log_ratio - f3_log( 1.618 ) ) < f3_sqr( hor_log_ratio - f3_log( 1.618 ) ) );
    }
    else if( o.arrange == TYPEOF_ver_golden_ratio )
    {
        f3_t ver_log_ratio = f3_log( ver_v_complexity ) - f3_log( ver_h_complexity );
        f3_t hor_log_ratio = f3_log( hor_v_complexity ) - f3_log( hor_h_complexity );
        o.vertical = ( f3_sqr( ver_log_ratio - f3_log( 1.618 ) ) < f3_sqr( hor_log_ratio - f3_log( 1.618 ) ) );
    }
    else if( o.arrange == TYPEOF_min_volume )
    {
        f3_t ver_volume = ver_v_complexity * ver_h_complexity;
        f3_t hor_volume = hor_v_complexity * hor_h_complexity;
        o.vertical = ( ver_volume < hor_volume );
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

    o.rte.run( o.rtt_open.cast( bgfe_rte_fp_rtt ), o, NULL );

    foreach( m$* e in o.window_list ) if( !e.is_open() ) e.open( o );

    o.is_open = true;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

identifier gtk_box_new, gtk_flow_box_new, gtk_widget_modify_bg, gtk_label_set_angle, gtk_box_pack_end, gtk_box_pack_start;
identifier gtk_widget_set_state_flags, GTK_STATE_FLAG_INSENSITIVE;
type GdkColor, GtkBox, GTK_BOX;

func (:s) er_t rtt_open( m@* o, vd_t unused )
{
    bl_t vertical = o.vertical;

    o.rtt_attach_widget( gtk_box_new( GTK_ORIENTATION_VERTICAL, 0 ), o.rtt_main_box );
    gtk_widget_show( o.rtt_main_box );

    m$* box_name = st_s!^;
    box_name.push_sc( o.widget_name ? o.widget_name.sc : "bgfe_frame" );

    if( o.show_border ) box_name.push_fa( "_border_style#<sz_t>", ( sz_t )( o.nesting_level % 4 ) );

    gtk_widget_set_name( o.rtt_main_box, box_name.sc );
    gtk_widget_set_size_request( o.rtt_main_box, o.width, o.height );
    o.rtt_widget = o.rtt_main_box;

    sc_t label_name = NULL;

    if( o.title )
    {
        label_name = o.title.sc;
    }
    else if( o.show_client_name && o.client_name )
    {
        label_name = bnameof( o.client_name );
    }

    if( label_name )
    {
        m GtkWidget* label = gtk_label_new( label_name );
        if( !label ) = GERR_fa( "'gtk_label_new' failed\n" );
        gtk_widget_set_name( label, "client_name" );
        gtk_box_pack_start( GTK_BOX( o.rtt_main_box ), label, false, false, 0 );
        gtk_widget_show( label );
    }

    o.rtt_attach_widget( gtk_box_new( vertical ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL, o.spacing ), o.rtt_content_box );
    if( o.insensitive ) gtk_widget_set_state_flags( o.rtt_content_box, GTK_STATE_FLAG_INSENSITIVE, false );
    gtk_widget_show( o.rtt_content_box );
    gtk_box_pack_start( GTK_BOX( o.rtt_main_box ), o.rtt_content_box, true, true, 0 );


    foreach( m$* e in o.content_list )
    {
        bl_t fill = o.stretch || ( o.stretch_first && ( __i == 0 ) ) || ( o.stretch_last && ( __i == o.content_list.size - 1 ) );
        bl_t expand = o.center || fill;
        if( o.end_bound )
        {
            gtk_box_pack_end( GTK_BOX( o.rtt_content_box ), e.rtt_widget(), expand, fill, 0 );
        }
        else
        {
            gtk_box_pack_start( GTK_BOX( o.rtt_content_box ), e.rtt_widget(), expand, fill, 0 );
        }
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) close
{
    if( !o.is_open ) = 0;

    foreach( m$* e in o.window_list  ) e.close();
    foreach( m$* e in o.content_list ) e.close();

    o.rte.run( o.rtt_close.cast( bgfe_rte_fp_rtt ), o, NULL );
    o.is_open = false;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t rtt_close( m@* o, vd_t arg )
{
    o.rtt_detach_widget( o.rtt_content_box );
    o.rtt_detach_widget( o.rtt_main_box );
    o.rtt_widget = NULL;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) cycle
{
    if( !o.is_open ) = 0; // no error because frame window could have been closed
    foreach( m$* e in o.content_list ) e.cycle( action_type );

    if( o.window_list )
    {
        bl_t any_closed = false;
        for( sz_t i = 0; i < o.window_list.size; i++ )
        {
            o.window_list.[ i ].cycle( action_type );
            if( !o.window_list.[ i ].is_open() )
            {
                o.window_list.[ i ] =< NULL;
                any_closed = true;
            }
        }
        if( any_closed ) o.window_list.remove_null_elements();
    }

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

func (:s) client_close_request
{
    if( action_type.0 == approve~ || action_type.0 == reject~ ) = 0;
    if( o.client ) o.client.t_bgfe_close_request( o.client_type, initiator, action_type );

    foreach( m$* e in o.content_list )
    {
        if( action_type.0 == approve~ || action_type.0 == reject~ ) = 0;
        e.client_close_request( initiator, action_type );
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) client_close_confirm
{
    if( action_type.0 == approve~ ) = 0;
    if( o.client ) o.client.t_bgfe_close_confirm( o.client_type, initiator, action_type );
    foreach( m$* e in o.content_list )
    {
        if( action_type.0 == approve~ ) = 0;
        e.client_close_confirm( initiator, action_type );
    }
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) client_distraction
{
    if( action_type.0 == approve~ ) = 0;
    if( o.client ) o.client.t_bgfe_distraction( o.client_type, initiator, action_type );
    foreach( m$* e in o.content_list )
    {
        if( action_type.0 == approve~ ) = 0;
        e.client_distraction( initiator, action_type );
    }
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) :.open_window_request
{
    switch( o.window_policy )
    {
        case any~ : action_type.0 = approve~; break;
        case zero~: action_type.0 = reject~;  break;
        case one~:
        {
            foreach( m$* e in o.content_list ) e.close_window_request();
            action_type.0 = approve~;
            break;
        }
        break;

        default: break;
    }
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) set_client_t
{
    if( o.is_open ) = GERR_fa( "Frame is open." );

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
    if( o.manual_content ) = 0;

    o.clear_content();

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
        frame_array.set_nesting_level( o.nesting_level + o.show_border );
        frame_array.set_client_t( content, content_type, content_name );
        tp_t action_type = escapprove~;
        frame_array.set_parent( o );
        o.client_edit_frame( content, content_type, content_name, action_type, frame_array );
        o.content_list!.push_d( frame_array.fork() );
    }
    else
    {
        tp_t frame_type = bgfe_frame_default_frame_type( content_type );
        tp_t action_type = escapprove~;
        o.client_edit_frame_type( content, content_type, content_name, action_type.1, frame_type );
        m bgfe_frame* frame = x_inst_create( frame_type ).cast( d bgfe_frame* )^;
        frame.set_nesting_level( o.nesting_level + o.show_border );
        action_type = escapprove~;
        if( frame )
        {
            frame.set_parent( o );
            frame.set_client_t( content, content_type, content_name );
            o.client_edit_frame( content, content_type, content_name, action_type.1, frame );
            frame.set_client_with_content_t( content, content_type, content_name );
            o.content_list!.push_d( frame.fork() );
        }
    }
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) add_linked_content_with_frame
{
    m bgfe_client* client = NULL;
    tp_t client_type = 0;
    o.get_nearest_client( client, client_type );

    if( !client || !client_type )
    {
        WRN_fa( "No client defined. Call set_client first.\n" );
        = 0;
    }

    if( !x_stamp_t_exists( client_type, content_name ) ) = GERR_fa( "'#name' is not a member of '#name'\n", content_name, client_type );

    m$* frame_link = frame ? frame.clone()^ : :link_s!^;
    frame_link.set_nesting_level( o.nesting_level + o.show_border );

    m$* content_sr = sr_s!^;
    content_sr.0 = x_stamp_t_m_get_sr( client, client_type, content_name );

    if( content_sr.o )
    {
        frame_link.set_client_t( content_sr.o, content_sr.type(), content_name );
    }
    else
    {
        tp_t content_type = x_stamp_t_type( client, client_type, content_name );
        frame_link.set_client_t( NULL, content_type, content_name );
    }

    frame_link.set_holder_t( client, client_type, o.client_name, false, 0 );

    tp_t action_type = escapprove~;
    frame_link.set_parent( o );
    o.client_edit_frame( content_sr.o, content_sr.type(), content_name, action_type.1, frame_link );

    o.content_list!.push_d( frame_link.fork() );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) add_frame
{
    if( o.is_open )
    {
        = GERR_fa( "Frame is open. Close it first." );
    }
    else
    {
        frame.set_parent( o );
        frame.set_nesting_level( o.nesting_level + o.show_border );
        o.content_list!.push_d( frame.fork() );
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) clear_content
{
    if( o.is_open ) = GERR_fa( "Frame is open. Close it first." );
    o.content_list =< NULL;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
