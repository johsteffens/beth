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

    st_s => text;   // editable text
    bl_t modified;  // text was modified by the front-end

    /// internals

    hidden bgfe_client* client; // client
    hidden tp_t client_type; // to cover obliv clients
    hidden tp_t client_name; // name of the client (under which the client's owner holds the client)
    hidden bgfe_frame* parent;
    func bgfe_frame.client = o.client;
    func bgfe_frame.client_type = o.client_type;
    func bgfe_frame.client_name = o.client_name;
    func bgfe_frame.parent = o.parent;
    func bgfe_frame.is_open = o.is_open;

    hidden st_s => rts_text;  // current entry text
    hidden bl_t rts_modified; // text was modified by the front end
    hidden st_s => rts_tooltip_text;  // tooltip text
    hidden bgfe_rte_s* rte;
    hidden x_mutex_s mutex;
    hidden GtkWidget*  rtt_widget;
    hidden bl_t is_open;
    hidden bl_t rtt_ignore_signal_value_changed;

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

func (:s) void rtt_signal_destroy( m GtkWidget* win, m@* o ) o.rtt_widget = NULL;

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
            bgfe_client_t_copy_to_typed( o.client, o.client_type, o.client_type, TYPEOF_st_s, st.cast( m x_inst* ) );
        }
    }
    else
    {
        bgfe_client_t_copy_to_typed( o.client, o.client_type, o.client_type, TYPEOF_st_s, st.cast( m x_inst* ) );
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t st_to_client( @* o, st_s* st )
{
    if( o.client_type == tp_t~ || o.client_type == aware_t~ || o.client_type == er_t~ )
    {
        bcore_name_check_collision( st.sc );
        tp_t type_name = bcore_name_enroll( st.sc );
        o.client.cast( m tp_t* ).0 = type_name;
    }
    else
    {
        bgfe_client_t_copy_from_typed( o.client, o.client_type, o.client_type, TYPEOF_st_s, st );
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

stamp :open_args_s
{
    sz_t width;
    sz_t height;
    st_s => text;

    func o _( m@* o, m :s* f )
    {
        o.width  = f.width;
        o.height = f.height;
        o.text   =< f.text.clone();
    }
}

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

    o.rts_tooltip_text =< NULL;

    if( bnameof( o.client_name ) ) o.rts_tooltip_text!.push_fa( "#<sc_t>", bnameof( o.client_name ) );
    if( bnameof( o.client_type ) )
    {
        if( o.rts_tooltip_text ) o.rts_tooltip_text!.push_fa( " " );
        o.rts_tooltip_text!.push_fa( "<#<sc_t>>", bnameof( o.client_type ) );
    }

    m$* rts_open_args = :open_args_s!^( o );

    o.mutex.unlock();

    o.rte.run( o.rtt_open.cast( bgfe_rte_fp_rtt ), o, rts_open_args );
    o.is_open = true;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

identifier gtk_entry_new, g_object_ref_sink, gtk_widget_set_tooltip_text;
identifier G_IS_INITIALLY_UNOWNED;
func (:s) er_t rtt_open( m@* o, :open_args_s* args )
{
    o.mutex.create_lock()^;
    o.rtt_widget = gtk_entry_new();
    if( !o.rtt_widget ) = GERR_fa( "'gtk_entry_new' failed\n" );
    if( G_IS_INITIALLY_UNOWNED( o.rtt_widget ) ) o.rtt_widget = g_object_ref_sink( o.rtt_widget );

    gtk_widget_set_name( o.rtt_widget, ifnameof( o._ ) );
    gtk_widget_set_size_request( o.rtt_widget, args.width, args.height );
    if( args.text ) gtk_entry_set_text( GTK_ENTRY( o.rtt_widget ), args.text ? args.text.sc : "" );

    if( o.rts_tooltip_text ) gtk_widget_set_tooltip_text( o.rtt_widget, o.rts_tooltip_text.sc );

    gtk_widget_show( o.rtt_widget );

    g_signal_connect( o.rtt_widget, "destroy", G_CALLBACK( :s_rtt_signal_destroy ), o );
    g_signal_connect( o.rtt_widget, "changed", G_CALLBACK( :s_rtt_signal_changed ), o );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) close
{
    if( !o.is_open ) = 0;
    o.rte.run( o.rtt_close.cast( bgfe_rte_fp_rtt ), o, NULL );
    o.is_open = false;
    o.close_confirm();
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

func (:s) set_client_t
{
    if( o.is_open ) = GERR_fa( "Frame is open. Close it first." );

    o.client      = client;
    o.client_type = client_type;
    o.client_name = client_name;

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) bgfe_frame.cycle
{
    if( !o.is_open ) = 0; // no error because frame window could have been closed

    o.mutex.lock();
    o.modified = o.rts_modified;
    if( o.modified )
    {
        o.text =< o.rts_text.clone();
        o.rts_modified = false;
    }
    o.mutex.unlock();

    if( o.modified )
    {
        if( action_type == approve~ ) = o.downsync();
        if( action_type == reject~  ) = o.upsync();
        o.downsync_request( o, action_type.1 );
        if( action_type == approve~ ) = o.downsync();
        if( action_type == reject~  ) = o.upsync();
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) bgfe_frame.downsync
{
    if( !o.is_open ) = 0; // no error because frame window could have been closed

    if( o.modified )
    {
        o.st_to_client( o.text! );

        o.modified = false;
        tp_t action_type = TYPEOF_escalate;
        o.downsync_confirm( o, action_type.1 );
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

/**********************************************************************************************************************/
