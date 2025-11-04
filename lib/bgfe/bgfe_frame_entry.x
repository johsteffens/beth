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

/** Text line editor based on GTK 'Entry'
 *  Associated types: st_s and convertible types
 *  Change event:
 *    While editing/typing, a downsync request is triggered on all edit inputs that result in a modified text.
 */

//----------------------------------------------------------------------------------------------------------------------

stamp :s bgfe_frame
{
    sz_t width;  // optional preset width
    sz_t height; // optional preset height
    bl_t insensitive; // insensitive: does not react to user actions
    f3_t => min; // optional minimum value; only for numeric clients
    f3_t => max; // optional maximum value; only for numeric clients
    sz_t accuracy_digits = 5; // number of digits for accuracy limitations (typically number of decimals after point when converting from float)
    st_s => text;   // editable text
    st_s => widget_name;   // optional gtk widget name overrides default widget name
    st_s => tooltip;     // external tooltip (if NULL an internal tooltip is used)
    bl_t show_tooltip = true;
    tp_t take_action_on = modified; // condition under which frame takes action on client: modified : activate

    func bgfe_frame.set_width { o.width = value; = 0; }
    func bgfe_frame.set_height{ o.height = value; = 0; }
    func bgfe_frame.set_insensitive { o.insensitive = flag; = 0; }
    func bgfe_frame.set_min { o.min!.0 = value; = 0; }
    func bgfe_frame.set_max { o.max!.0 = value; = 0; }
    func bgfe_frame.set_accuracy_digits { o.accuracy_digits = value; = 0; }
    func bgfe_frame.set_text  { o.text!.copy_sc( text ); = 0; }
    func bgfe_frame.set_widget_name{ o.widget_name!.copy_sc( text ); = 0; }
    func bgfe_frame.set_tooltip{ o.tooltip!.copy_sc( text ); = 0; }
    func bgfe_frame.set_show_tooltip{ o.show_tooltip = flag; = 0; }
    func bgfe_frame.set_take_action_on{ o.take_action_on = name; = 0; }

    /// internals
    hidden bgfe_client* client; // client
    hidden tp_t client_type; // to cover obliv clients
    hidden tp_t client_name; // name of the client (under which the client's owner holds the client)
    hidden bgfe_frame* parent;
    func bgfe_frame.client = o.client;
    func bgfe_frame.client_type = o.client_type;
    func bgfe_frame.client_name = o.client_name;
    func bgfe_frame.parent = o.parent;
    func bgfe_frame.set_parent o.parent = parent;
    func bgfe_frame.is_open = o.is_open;
    func bgfe_frame.h_complexity = 4;
    func bgfe_frame.v_complexity = 1;
    func bgfe_frame.is_compact = true;

    hidden st_s => rts_text;  // current entry text
    hidden bl_t rts_modified; // text was modified by the front end
    hidden bl_t rts_activate; // activate signal (typically enter was pressed)
    hidden bl_t rts_lost_focus; // activated when widget lost focus
    hidden st_s => tooltip_text;  // tooltip text
    hidden bgfe_rte_s* rte;
    hidden x_mutex_s mutex;
    hidden GtkWidget*  rtt_widget;
    hidden bl_t client_is_numeric;
    hidden bl_t is_open;
    hidden bl_t rtt_ignore_signal_value_changed;
    hidden tp_t last_cycle_action_type = reject; // action type of last cycle
    hidden bl_t modified; // text was modified by the front-end but changes were not yet downsynched
    hidden bl_t activate; // front end has signaled activation but changes were not yet downsynched
    hidden bl_t lost_focus; // activated when widget lost focus

    func bcore_inst_call.down_e o.close();

    func bgfe_frame.rtt_widget = o.rtt_widget;
    func bgfe_frame.open;
    func bgfe_frame.close;
    func bgfe_frame.client_close_request;

    /// interface functions ...
    func bgfe_frame.set_client_t;
    func bgfe_frame.cycle;
    func bgfe_frame.upsync;
    func bgfe_frame.downsync;
}

//----------------------------------------------------------------------------------------------------------------------

identifier gtk_entry_get_text, gtk_entry_set_text;
identifier GTK_ENTRY, GTK_EDITABLE;

func (:s) void rtt_signal_changed( m GtkWidget* win, m@* o )
{
    if( o.rtt_ignore_signal_value_changed ) return;
    o.mutex.lock();
    o.rts_text!.copy_sc( gtk_entry_get_text( GTK_ENTRY( o.rtt_widget ) ) );
    o.rts_modified = true;
    o.mutex.unlock();
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) void rtt_signal_activate( m GtkWidget* win, m@* o )
{
    if( o.rtt_ignore_signal_value_changed ) return;
    o.mutex.lock();
    o.rts_activate = true;
    o.mutex.unlock();
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t rtt_set_text( m@* o, st_s* rts_text )
{
    o.rtt_ignore_signal_value_changed = true;
    o.mutex.lock();
    gtk_entry_set_text( GTK_ENTRY( o.rtt_widget ), rts_text ? rts_text.sc : "" );
    o.mutex.unlock();
    o.rtt_ignore_signal_value_changed = false;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) gboolean rtt_signal_focus_out_event( m GtkWidget* win, m GdkEvent* event, m@* o )
{
    o.mutex.lock();
    o.rts_lost_focus = true;
    o.mutex.unlock();
    verbatim_C{ return FALSE; }
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t client_to_st( @* o, m st_s* st )
{
    if( o.client_type == tp_t~ || o.client_type == aware_t~ || o.client_type == er_t~ )
    {
        sc_t name = bnameof( o.client.cast( tp_t* ).0 );
        if( name )
        {
            st.copy_sc( name );
            = 0;
        }
        else
        {
            bgfe_client_t_bgfe_copy_to_typed( o.client, o.client_type, o.client_type, TYPEOF_st_s, st.cast( m x_inst* ) );
        }
    }
    else if( o.client_type == f2_t~ || o.client_type == f3_t~ )
    {
        f3_t val = ( o.client_type == f2_t~ ) ? o.client.cast( f2_t* ).0 : ( o.client_type == f3_t~ ) ? o.client.cast( f3_t* ).0 : 0;
        st.copyf( st_s!^.push_fa( "%.#<sz_t>g", o.accuracy_digits ).sc, val );
    }
    else
    {
        bgfe_client_t_bgfe_copy_to_typed( o.client, o.client_type, o.client_type, TYPEOF_st_s, st.cast( m x_inst* ) );
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t st_to_client( @* o, st_s* st )
{
    if( o.client_is_numeric && ( o.max || o.min ) )
    {
        f3_t value = 0;
        x_inst_t_copy_typed( value.1, f3_t~, TYPEOF_st_s, st );
        if( o.max && value > o.max.0 ) = bcore_error_push_fa( TYPEOF_general_error, "Value '#<f3_t>' is beyond maximum '#<f3_t>'", value, o.max.0 );
        if( o.min && value < o.min.0 ) = bcore_error_push_fa( TYPEOF_general_error, "Value '#<f3_t>' is beyond minimum '#<f3_t>'", value, o.min.0 );
    }

    if( o.client_type == tp_t~ || o.client_type == aware_t~ || o.client_type == er_t~ )
    {
        bcore_name_check_collision( st.sc );
        tp_t type_name = bcore_name_enroll( st.sc );
        o.client.cast( m tp_t* ).0 = type_name;
    }
    else
    {
        bgfe_client_t_bgfe_copy_from_typed( o.client, o.client_type, o.client_type, TYPEOF_st_s, st );
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) open
{
    if( o.is_open ) = 0;
    if( !o.client ) = GERR_fa( "No client defined. Call set_client or set_client_t first." );
    bgfe_rte_get( &o.rte );

    ASSERT( parent );
    o.parent = parent;

    o.client_to_st( o.text! );

    o.mutex.lock();
    o.rts_text =< o.text.clone();
    o.mutex.unlock();

    if( o.tooltip )
    {
        o.tooltip_text!.push_st( o.tooltip );
    }
    else
    {
        if( bnameof( o.client_name ) ) o.tooltip_text!.push_fa( "#<sc_t>", bnameof( o.client_name ) );
        if( bnameof( o.client_type ) )
        {
            if( o.tooltip_text ) o.tooltip_text!.push_fa( " " );
            o.tooltip_text!.push_fa( "<#<sc_t>>", bnameof( o.client_type ) );
        }
    }

    o.rte.run( o.rtt_open.cast( bgfe_rte_fp_rtt ), o, NULL );
    o.is_open = true;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

identifier gtk_entry_new, g_object_ref_sink, gtk_widget_set_tooltip_text;
func (:s) er_t rtt_open( m@* o, vd_t unused )
{
    o.rtt_attach_widget( gtk_entry_new(), o.rtt_widget );
    if( o.insensitive ) gtk_widget_set_state_flags( o.rtt_widget, GTK_STATE_FLAG_INSENSITIVE, false );

    gtk_widget_set_name( o.rtt_widget, o.widget_name ? o.widget_name.sc : ifnameof( o._ ) );
    gtk_widget_set_size_request( o.rtt_widget, o.width, o.height );
    if( o.text ) gtk_entry_set_text( GTK_ENTRY( o.rtt_widget ), o.text.sc );

    if( o.show_tooltip && o.tooltip_text ) gtk_widget_set_tooltip_text( o.rtt_widget, o.tooltip_text.sc );

    gtk_widget_show( o.rtt_widget );

    g_signal_connect( o.rtt_widget, "changed", G_CALLBACK( :s_rtt_signal_changed ), o );
    g_signal_connect( o.rtt_widget, "activate", G_CALLBACK( :s_rtt_signal_activate ), o );
    g_signal_connect( o.rtt_widget, "focus-out-event", G_CALLBACK( :s_rtt_signal_focus_out_event ), o );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) close
{
    if( !o.is_open ) = 0;
    o.rte.run( o.rtt_close.cast( bgfe_rte_fp_rtt ), o, NULL );
    o.is_open = false;
    o.tooltip_text =< NULL;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t rtt_close( m@* o, vd_t arg )
{
    o.rtt_detach_widget( o.rtt_widget );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) set_client_t
{
    if( o.is_open ) = GERR_fa( "Frame is open." );

    o.client      = client;
    o.client_type = client_type;
    o.client_name = client_name;

    if( bcore_tp_is_numeric( client_type ) )
    {
        o.client_is_numeric = true;
        o.take_action_on = activate~;
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t take_action( m@* o, tp_t action_type )
{
    if( o.modified )
    {
        if( action_type == approve~ ) = o.downsync();
        if( action_type == reject~  ) = o.upsync();
        o.client_change_request( o, action_type.1 );
        if( action_type == approve~ ) = o.downsync();
        if( action_type == reject~  ) = o.upsync();
        o.modified = false;
    }

    if( o.lost_focus )
    {
        o.lost_focus = false;
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) bgfe_frame.cycle
{
    if( !o.is_open ) = 0; // no error because frame window could have been closed

    o.last_cycle_action_type = action_type;

    o.mutex.lock();
    o.modified   = o.modified   || o.rts_modified;
    o.activate   = o.activate   || o.rts_activate;
    o.lost_focus = o.lost_focus || o.rts_lost_focus;
    if( o.modified ) o.text =< o.rts_text.clone();
    o.rts_activate = false;
    o.rts_modified = false;
    o.rts_lost_focus = false;
    o.mutex.unlock();

    bl_t take_action = ( o.modified && o.take_action_on == modified~ ) || ( ( o.activate || o.lost_focus ) && o.take_action_on == activate~ );
    if( take_action ) o.take_action( action_type );

    if( o.activate )
    {
        tp_t action_type = escalate~;
        o.client_activate( o, action_type.1 );
        o.activate = false;
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) bgfe_frame.downsync
{
    if( !o.is_open ) = 0; // no error because frame window could have been closed

    if( o.modified )
    {
        if( o.st_to_client( o.text! ) )
        {
            {
                m$* msg = st_s!^;
                msg.push_fa( "Converting \"#<sc_t>\" into <#name>:\n", o.text.sc, o.client_type );
                bcore_error_pop_all_to_st( msg );
                bgfe_popup_message_s!^.run( o, "Error", msg.sc );
            }
            o.upsync();
        }
        else
        {
            tp_t action_type = TYPEOF_escalate;
            o.client_change_confirm( o, action_type.1 );
        }
        o.modified = false;
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) bgfe_frame.upsync
{
    if( !o.is_open ) = 0; // no error because frame window could have been closed

    m$* client_text = st_s!^;
    o.client_to_st( client_text );

    if( !o.text || !client_text.equal( o.text ) )
    {
        o.text =< client_text.clone();
        o.mutex.lock();
        o.rts_text =< o.text.clone();
        o.mutex.unlock();

        o.rte.run( o.rtt_set_text.cast( bgfe_rte_fp_rtt ), o, o.rts_text );
        o.modified = false;
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) bgfe_frame.client_close_request
{
    if( o.modified && o.take_action_on == activate~ ) o.take_action( o.last_cycle_action_type );
    = o.client_close_request_default( initiator, action_type );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
