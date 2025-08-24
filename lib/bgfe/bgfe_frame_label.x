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

/// small text frame based on gtk label

/** Text frame (const) based on GTK 'Label'
 *  If Client feature bgfe_get_glimpse is overloaded it will be used for text generation.
 *  Text can also be set manually 'set_text' which disables changes via upsync.
 *  Otherwise associated types: st_s and convertible types
 */

//----------------------------------------------------------------------------------------------------------------------

stamp :s bgfe_frame
{
    /// parameters
    $ st_s => text;   // label text
    sz_t width;  // optional preferred width of label  (actual size is calculated from the ext)
    sz_t height; // optional preferred height of label (actual size is calculated from the ext)
    st_s => widget_name;   // optional gtk widget name overrides default widget name
    st_s => tooltip;     // external tooltip (if NULL an internal tooltip is used)
    bl_t show_tooltip = false;
    bl_t no_upsync;
    f3_t text_xalign = 0.5; // gradual text alignment: 0: left, 0.5: center, 1.0 right
    f3_t text_yalign = 0.5; // gradual text alignment: 0: top, 0.5: center, 1.0 bottom

    func bgfe_frame.set_tooltip{ o.tooltip!.copy_sc( text ); = 0; }
    func bgfe_frame.set_show_tooltip{ o.show_tooltip = flag; = 0; }
    func bgfe_frame.set_width { o.width = value; = 0; }
    func bgfe_frame.set_height{ o.height = value; = 0; }
    func bgfe_frame.set_text  { o.text!.copy_sc( text ); o.no_upsync = true; = 0; }
    func bgfe_frame.set_widget_name{ o.widget_name!.copy_sc( text ); = 0; }
    func bgfe_frame.set_text_xalign { o.text_xalign = f3_max( 0, f3_min( 1, value ) ); = 0; }
    func bgfe_frame.set_text_yalign { o.text_yalign = f3_max( 0, f3_min( 1, value ) ); = 0; }

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
    func bgfe_frame.h_complexity = 2;
    func bgfe_frame.v_complexity = 0.5;
    func bgfe_frame.is_compact = true;

    hidden st_s => tooltip_text;  // tooltip text
    hidden bgfe_rte_s* rte;
    hidden x_mutex_s mutex;
    hidden GtkWidget*  rtt_widget;
    hidden bl_t is_open;

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

identifier gtk_label_set_text;
identifier GTK_LABEL;
func (:s) er_t rtt_set_text( m@* o, st_s* rts_text )
{
    o.mutex.lock();
    gtk_label_set_text( GTK_LABEL( o.rtt_widget ), rts_text ? rts_text.sc : "" );
    o.mutex.unlock();
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t client_to_st( @* o, m st_s* st )
{
    if( !o.client ) = 0;
    if( o.client_type == tp_t~ || o.client_type == aware_t~ || o.client_type == er_t~ )
    {
        sc_t name = bnameof( o.client.cast( tp_t* ).0 );
        if( name )
        {
            st.copy_sc( name );
            = 0;
        }
    }


    if( o.client_type == st_s~ )
    {
        st.copy( o.client.cast( st_s* ) );
    }
    else if( o.client_type == sc_t~ )
    {
        st.copy_sc( o.client.cast( sc_t* ).0 );
    }
    else
    {
        o.client_get_glimpse( st );
    }


    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) open
{
    if( o.is_open ) = 0;
    bgfe_rte_get( &o.rte );

    ASSERT( parent );
    o.parent = parent;

    if( !o.text ) o.client_to_st( o.text! );

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

identifier gtk_label_new, gtk_label_set_xalign, gtk_label_set_yalign;

func (:s) er_t rtt_open( m@* o, vd_t unused )
{
    o.rtt_attach_widget( gtk_label_new( o.text ? o.text.sc : "" ), o.rtt_widget );
    gtk_label_set_xalign( GTK_LABEL( o.rtt_widget ), o.text_xalign );
    gtk_label_set_yalign( GTK_LABEL( o.rtt_widget ), o.text_yalign );
    gtk_widget_set_name( o.rtt_widget, o.widget_name ? o.widget_name.sc : ifnameof( o._ ) );
    if( o.width > 0 || o.height > 0 ) gtk_widget_set_size_request( o.rtt_widget, o.width, o.height );
    if( o.show_tooltip && o.tooltip_text ) gtk_widget_set_tooltip_text( o.rtt_widget, o.tooltip_text.sc );
    gtk_widget_show( o.rtt_widget );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) close
{
    if( !o.is_open ) = 0;
    o.rte.run( o.rtt_close.cast( bgfe_rte_fp_rtt ), o, NULL );
    o.is_open = false;
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

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) bgfe_frame.client_change_request = 0;
func (:s) bgfe_frame.client_change_confirm = 0;

//----------------------------------------------------------------------------------------------------------------------

func (:s) bgfe_frame.cycle
{
    if( !o.is_open ) = 0; // no error because frame window could have been closed
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) bgfe_frame.downsync
{
    if( !o.is_open ) = 0; // no error because frame window could have been closed
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) bgfe_frame.upsync
{
    if( !o.is_open ) = 0; // no error because frame window could have been closed
    if( !o.client ) = 0;
    if( o.no_upsync ) = 0;


    m$* client_text = st_s!^;
    o.client_to_st( client_text );

    if( !client_text.equal( o.text ) )
    {
        o.text =< client_text.clone();
        o.mutex.lock();
        m$* rts_text = client_text.clone()^;
        o.mutex.unlock();
        o.rte.run( o.rtt_set_text.cast( bgfe_rte_fp_rtt ), o, rts_text );
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
