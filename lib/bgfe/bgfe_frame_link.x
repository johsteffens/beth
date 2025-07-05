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

/** Button with text label representing a link. The referenced object is displayed in a dedicated dialog window.
 *  The frame opens the dialog-window when the button is pressed in order to edit/create the underlying link of the parent's client.
 *  The client of the link frame is the linked object.
 *  The parents client is the object holding the link.
 *  The link maintains a strong reference to its client because the link-holder can delete the link, rendering the linked object invalid.
 */

//----------------------------------------------------------------------------------------------------------------------

forward :type_dialog_s;
stamp :s bgfe_frame
{
    /// parameters
    sz_t width;  // optional preferred width of button  (actual size is calculated from the text)
    sz_t height; // optional preferred height of button (actual size is calculated from the text)
    bl_t show_client_name = true; // client name on button
    bl_t show_tooltip     = true; // tooltip with client name
    bl_t show_glimpse     = true; // shows glimpse on button if available
    func bgfe_frame.set_show_tooltip{ o.show_tooltip = flag; = 0; }
    func bgfe_frame.set_show_client_name{ o.show_client_name = flag; = 0; }
    func bgfe_frame.set_show_glimpse{ o.show_glimpse = flag; = 0; }

    func bgfe_frame.set_width { o.width = value; = 0; }
    func bgfe_frame.set_height{ o.height = value; = 0; }

    /// internals
    hidden sr_s client; // client (as strong smart reference)
    hidden tp_t client_name;  // name of the client (under which the link holder adresses the client); 0 in case the link holder is an array
    hidden sz_t client_index; // index of the client (under which the link holder (array) adresses the client);
    hidden bgfe_frame* parent;

    hidden bl_t link_changed; // link directly changed (applied in downsync)

    // link_holder: client that holds the link
    hidden bgfe_client* link_holder;
    hidden tp_t link_holder_type;

    hidden bl_t holder_is_array; // true in case the link is an element of an array

    func bgfe_frame.client = o.client.o;
    func bgfe_frame.client_type = o.client.type();
    func bgfe_frame.client_name = o.client_name;
    func bgfe_frame.parent = o.parent;
    func bgfe_frame.is_open = o.is_open;
    func bgfe_frame.h_complexity = 3;
    func bgfe_frame.v_complexity = 1;
    func bgfe_frame.is_compact = true;

    hidden bgfe_frame    => embedded_frame;
    hidden bgfe_window_s => window;

    hidden :type_dialog_s => type_dialog;

    hidden bl_t rts_main_clicked; // main button was clicked
    hidden bl_t rts_clear_clicked; // clear button was clicked

    hidden st_s button_main_text;  // button text
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

    func er_t set_holder_t( m@* o, m bgfe_client* holder, tp_t holder_type, tp_t holder_name, bl_t holder_is_array, sz_t client_index );

    func bgfe_frame.cycle;
    func bgfe_frame.upsync;
    func bgfe_frame.downsync;
}

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
    if( !o.rtt_button_main ) = 0;
    o.mutex.lock();
    gtk_button_set_label( GTK_BUTTON( o.rtt_button_main ), rts_text.sc );
    o.mutex.unlock();
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t set_text( m@* o, sc_t text )
{
    if( sc_t_equal( text, o.button_main_text.sc ) ) = 0;

    o.mutex.lock();
    o.button_main_text.copy_sc( text );
    o.mutex.unlock();
    o.rte.run( o.rtt_set_text.cast( bgfe_rte_fp_rtt ), o, o.button_main_text.1 );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t set_text_from_client_status( m@* o )
{
    m$* text = st_s!^;

    if( o.show_client_name && o.client_name ) text.push_fa( "#name: ", o.client_name );

    if( o.client.o )
    {
        if( o.show_glimpse && o.client.o.cast( bgfe_client* ).t_defines_bgfe_get_glimpse( o.client.type() ) )
        {
            o.client.o.cast( bgfe_client* ).t_bgfe_get_glimpse( o.client.type(), text );
        }
        else
        {
            text.push_fa( "<#<sc_t>>", bnameof( o.client.type() ) );
        }
    }
    else
    {
        text.push_fa( "<NULL>" );
    }
    o.set_text( text.sc );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) open
{
    if( o.is_open ) = 0;
    bgfe_rte_get( &o.rte );

    if( !o.link_holder ) = GERR_fa( "No holder defined." );
    if( !parent ) = GERR_fa( "Parent is NULL." );
    o.parent = parent;

    if( o.client_name && bnameof( o.client_name ) ) o.rts_tooltip_text!.push_fa( "#<sc_t>", bnameof( o.client_name ) );
    if( bnameof( o.client.type() ) )
    {
        if( o.rts_tooltip_text ) o.rts_tooltip_text!.push_fa( " " );
        o.rts_tooltip_text!.push_fa( "<#<sc_t>>", bnameof( o.client.type() ) );
    }

    o.rte.run( o.rtt_open.cast( bgfe_rte_fp_rtt ), o, NULL );

    o.rebuild();

    o.is_open = true;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

identifier gtk_button_new, gtk_button_new_with_label, gtk_button_set_relief;
type GTK_RELIEF_NORMAL;

func (:s) er_t rtt_open( m@* o, vd_t unused )
{
    o.rtt_attach_widget( gtk_box_new( GTK_ORIENTATION_HORIZONTAL, 0 ), o.rtt_widget );
    gtk_widget_set_name( o.rtt_widget, ifnameof( o._ ) );
    if( o.width > 0 || o.height > 0 ) gtk_widget_set_size_request( o.rtt_widget, o.width, o.height );
    gtk_widget_show( o.rtt_widget );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t rebuild( m@* o )
{
    if( o.window )
    {
        o.window.close();
        o.window =< NULL;
    }

    if( o.embedded_frame )
    {
        o.embedded_frame.close();
        o.embedded_frame =< NULL;
    }

    if( o.client.o )
    {
        tp_t frame_type = bgfe_frame_default_frame_type( o.client.type() );
        tp_t action_type = escapprove~;
        o.client_edit_frame_type( o.client.o, o.client.type(), o.client_name, action_type.1, frame_type );
        if( bgfe_frame_t_is_compact( NULL, frame_type ) )
        {
            m bgfe_frame* frame = x_inst_t_create( frame_type ).cast( d bgfe_frame* )^;
            action_type = escapprove~;
            o.client_edit_frame( o.client.o, o.client.type(), o.client_name, action_type.1, frame );
            if( frame )
            {
                o.embedded_frame =< frame.fork();
                o.embedded_frame.set_client_with_content_t( o.client.o, o.client.type(), o.client_name );
                o.embedded_frame.open( o.parent );
            }
        }
    }

    o.rte.run( o.rtt_rebuild.cast( bgfe_rte_fp_rtt ), o, NULL );

    o.set_text_from_client_status();
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func void rtt_remove_from_container( m GtkWidget* widget, vd_t container )
{
    gtk_container_remove( GTK_CONTAINER( container ), widget );
}

//----------------------------------------------------------------------------------------------------------------------

name gtk_container_foreach;
func (:s) er_t rtt_rebuild( m@* o, vd_t unused )
{
    o.rtt_remove_all_widgets_from_container( o.rtt_widget );

    o.rtt_detach_widget( o.rtt_button_main );
    o.rtt_detach_widget( o.rtt_button_clear );

    if( o.embedded_frame )
    {
        gtk_box_pack_start( GTK_BOX( o.rtt_widget ), o.embedded_frame.rtt_widget(),  true,  true,  0 );
    }
    else
    {
        /// rtt_button_main
        o.rtt_attach_widget( gtk_button_new(), o.rtt_button_main );
        g_signal_connect( o.rtt_button_main, "clicked", G_CALLBACK( :s_rtt_button_main_signal_clicked ), o );
        gtk_button_set_relief( GTK_BUTTON( o.rtt_button_main ), GTK_RELIEF_NORMAL );
        gtk_widget_set_name( o.rtt_button_main,  ifnameof( o._ ) );
        if( o.show_tooltip && o.rts_tooltip_text ) gtk_widget_set_tooltip_text( o.rtt_button_main, o.rts_tooltip_text.sc );
        gtk_widget_show( o.rtt_button_main );
        gtk_box_pack_start( GTK_BOX( o.rtt_widget ), o.rtt_button_main,  true,  true,  0 );
    }


    /// rtt_button_clear
    o.rtt_attach_widget( gtk_button_new_with_label( "X" ), o.rtt_button_clear );
    g_signal_connect( o.rtt_button_clear, "clicked", G_CALLBACK( :s_rtt_button_clear_signal_clicked ), o );
    gtk_button_set_relief( GTK_BUTTON( o.rtt_button_clear ), GTK_RELIEF_NORMAL );
    gtk_widget_set_name( o.rtt_button_clear, ifnameof( o._ ) );
    gtk_widget_set_tooltip_text( o.rtt_button_clear, "clear" );
    gtk_widget_show( o.rtt_button_clear );
    gtk_box_pack_start( GTK_BOX( o.rtt_widget ), o.rtt_button_clear, false, false, 0 );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) close
{
    if( !o.is_open ) = 0;
    if( o.window ) o.window =< NULL;
    o.rte.run( o.rtt_close.cast( bgfe_rte_fp_rtt ), o, NULL );
    o.is_open = false;
    o.client_close_confirm();
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t rtt_close( m@* o, vd_t arg )
{
    o.rtt_detach_widget( o.rtt_button_main );
    o.rtt_detach_widget( o.rtt_button_clear );
    o.rtt_detach_widget( o.rtt_widget );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) set_client_t
{
    if( o.is_open ) = GERR_fa( "Frame is open." );

    o.client.tsm( client_type, client.fork() );
    o.client_name = client_name;

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) set_holder_t
{
    if( o.is_open ) = GERR_fa( "Frame is open." );

    o.link_holder = holder;
    o.link_holder_type = holder_type;
    o.holder_is_array = holder_is_array;
    o.client_index = client_index;

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) bgfe_frame.client_change_request = 0;
func (:s) bgfe_frame.client_change_confirm = 0;

//----------------------------------------------------------------------------------------------------------------------

func (:s) bgfe_notify_receiver.notify_receive
{
    if( o.is_open )
    {
        if( notify_type == change_confirm~ ) o.set_text_from_client_status();
    }
    = 0;
}

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

    bl_t link_changed = false;
    if( main_clicked )
    {
        if( !o.client.o )
        {
            if( !x_inst_is_creatable( o.client.type() ) )
            {
                o.type_dialog!.run();
                if( o.type_dialog.apply() )
                {
                    tp_t type = btypeof( o.type_dialog.text.sc );
                    if( x_stamp_t_is_stamp( type ) && x_stamp_t_is_aware( type ) ) o.client.tsm( type, NULL );
                }
            }

            if( x_inst_is_creatable( o.client.type() ) )
            {
                o.client.tsm( o.client.type(), x_inst_t_create( o.client.type() ) );
                link_changed = true;
                o.rebuild();
            }
        }

        if( o.client.o )
        {
            if( !o.embedded_frame )
            {
                if( !o.window )
                {
                    o.window!.keep_above = true;
                    o.window.set_client_with_content_t( o.client.o, o.client.type(), o.client_name );
                    o.window.open( o.parent );
                    if( o.window.supports_notify() ) o.window.cast( m bgfe_notify_sender* ).set_notify( o, change_confirm~ );
                }
                else
                {
                    o.window.present();
                }
            }
        }
    }

    if( clear_clicked )
    {
        if( o.client.o )
        {
            o.client.clear();

            if( o.holder_is_array )
            {
                if( x_array_t_is_static( o.link_holder_type ) )
                {
                    tp_t type = x_array_t_get_static_type( o.link_holder_type );
                    o.client.tsm( type, NULL );
                }
            }
            else
            {
                if( x_stamp_t_is_static_link( o.link_holder_type, o.client_name ) )
                {
                    tp_t type = x_stamp_t_type( o.link_holder, o.link_holder_type, o.client_name );
                    o.client.tsm( type, NULL );
                }
            }
            o.rebuild();
            link_changed = true;
        }
    }

    if( link_changed )
    {
        o.link_changed = true;
        tp_t link_action_type = action_type;
        if( link_action_type != approve~ ) o.parent.client_change_request( o, link_action_type.1 );
        if( link_action_type == approve~ ) o.downsync();
        o.set_text_from_client_status();
    }

    if( o.embedded_frame )
    {
        o.embedded_frame.cycle( action_type );
    }
    else if( o.window )
    {
        o.window.cycle( action_type );
        if( !o.window.is_open )
        {
            o.window =< NULL;
            o.set_text_from_client_status();
        }
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) bgfe_frame.downsync
{
    if( !o.is_open ) = 0; // no error because frame window could have been closed

    bcore_msg_fa( "#name: downsync\n", o._ );

    if( o.link_changed && o.link_holder )
    {
        if( o.holder_is_array )
        {
            x_array_t_set_sr( o.link_holder, o.link_holder_type, o.client_index, sr_tsm( o.client.type(), o.client.o.fork() ) );
        }
        else
        {
            x_stamp_t_set_sr( o.link_holder, o.link_holder_type, o.client_name, sr_tsm( o.client.type(), o.client.o.fork() ) );
        }

        tp_t action_type = escalate~;
        o.parent.client_change_confirm( o, action_type.1 );
        o.link_changed = false;
    }

    if( o.embedded_frame )
    {
        o.embedded_frame.downsync();
    }
    else if( o.window )
    {
        o.window.downsync();
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) bgfe_frame.upsync
{
    if( !o.is_open ) = 0; // no error because frame window could have been closed

    m$* sr = sr_s!^;
    if( o.holder_is_array )
    {
        sr.0 = x_array_t_m_get_sr( o.link_holder, o.link_holder_type, o.client_index );
    }
    else
    {
        sr.0 = x_stamp_t_m_get_sr( o.link_holder, o.link_holder_type, o.client_name );
    }

    if( sr.o != o.client.o )
    {
        o.client.tsm( sr.type(), sr.o.fork() );
        o.rebuild();
    }
    else if( o.embedded_frame )
    {
        o.embedded_frame.upsync();
    }
    else if( o.window )
    {
        o.window.upsync();
    }

    o.set_text_from_client_status();

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

group :type_dialog
{
    stamp :buttons_s
    {
        bl_t Apply;
        bl_t Cancel;

        func void reset( m@* o ) { o.Apply = o.Cancel = false; }

        func bgfe_client.bgfe_edit_frame_type
        {
            bcore_msg_fa( "#name: bgfe_edit_frame_type for #name\n", o._, content_type );
            if( content_type == bl_t~ )
            {
                frame_type.0 =  bgfe_frame_flag_button_s~;
                action_type.0 = approve~;
            }
            = 0;
        }

        func bgfe_client.bgfe_edit_frame
        {
            if( content == (vd_t)o )
            {
                frame.set_arrange( horizontal~ );
                frame.set_show_client_name( false );
                frame.set_widget_name( "" );
                frame.set_spacing( 5 );
                frame.set_center( true );
                frame.set_stretch( true );
                action_type.0 = approve~;
            }
            = 0;
        }

        func bl_t exit( m@* o ) = o.Apply || o.Cancel;
    }

    stamp :s
    {
        sc_t prompt = "Enter a type:";
        st_s text;
        sc_t status = "invalid";

        :buttons_s buttons;

        func bl_t exit( m@* o ) = o.buttons.exit();
        func bl_t apply( m@* o ) = o.buttons.Apply;

        func void reset( m@* o ) { o.buttons.reset(); }

        hidden bgfe_window_s => window;

        func bgfe_client.bgfe_enter_pressed { o.buttons.Apply = true; action_type.0 = 0; = 0; }

        func bgfe_client.bgfe_change_confirm
        {
            if( initiator.client() == o.text.1.cast( bgfe_client* ) )
            {
                tp_t type = btypeof( o.text.sc );
                bl_t is_aware = x_stamp_t_is_stamp( type ) && x_stamp_t_is_aware( type );
                o.status = is_aware ? "VALID" : "invalid";
                o.window.upsync();
            }
            action_type.0 = 0;
            = 0;
        }

        func bgfe_client.bgfe_edit_frame
        {
            if( content == (vd_t)o )
            {
                frame.set_show_client_name( false );
                frame.set_spacing( 5 );
                action_type.0 = approve~;
            }
            else
            {
                frame.set_show_tooltip( false );
                action_type.0 = approve~;
            }

            = 0;
        }

        func er_t run( m@* o )
        {
            o.reset();
            o.window!.set_client_with_content( o, 0 );
            o.window.open( NULL );
            while( o.window.is_open && !o.exit() ) o.window.cycle_sleep_ms( escapprove~, 10 );
            o.window.close();
            = 0;
        }
    }

}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

