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

 /** Specifies the placement (x,y, width, height) of the main widget of a frame
  *  For normal widgets: x,y coordinates are relative to the parent window
  *  For windows: x,y are global coordinates
  *  This object is tailored around the configure-event, which gives reliable
  *  placement information but seems only available for special widgets, like
  *  window and drawable.
  *  For other widgets and asynchronous location access use bgfe_location_s.
  */


/**********************************************************************************************************************/

type GdkEventConfigure, gboolean, GtkWidget;
type GtkAllocation;

identifier TRUE, FALSE;

forward bgfe_frame;

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp :s
{
    // on a window these are screen coordinates
    sz_t x;
    sz_t y;

    // size of widget
    sz_t width;
    sz_t height;

    func void from_event_configure( m@* o, GdkEventConfigure* event );
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) from_event_configure
{
    verbatim_C
    {
        o->x = event->x;
        o->y = event->y;
        o->width  = event->width;
        o->height = event->height;
    };
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp :shell_s
{
    /// parameters
    bl_t broadcast_distraction; // proadcasts distraction in case placement changes

    /// internals
    :s placement;
    bl_t modified; // modified flag raised by configure event
    hidden bgfe_frame* parent; // parent frame
    hidden bgfe_rte_s* rte;
    hidden GtkWidget* rtt_widget;
    x_mutex_s mutex;

    /// interface functions
    func er_t setup( m@* o, m bgfe_frame* parent )
    {
        o.parent = parent;
        bgfe_rte_get( &o.rte );
        = 0;
    }

    func er_t rtt_open ( m@* o, m GtkWidget* rtt_widget );
    func er_t rtt_close( m@* o );
    func er_t get_placement( m@* o, m :s* placement );
    func er_t set_placement( m@* o, c :s* placement );
    func er_t cycle( m@* o, m bgfe_frame* frame ); // frame: frame to be notified about related events
}

//----------------------------------------------------------------------------------------------------------------------

func (:shell_s) gboolean rtt_signal_configure_event( m GtkWidget* widget, GdkEventConfigure* event, m@* o )
{
    o.mutex.lock();
    o.placement.from_event_configure( event );
    o.rtt_widget = widget;
    o.modified = true;
    o.mutex.unlock();
    = FALSE;
}

//----------------------------------------------------------------------------------------------------------------------

func (:shell_s) rtt_open
{
    if( rtt_widget == NULL ) = 0;
    o.rtt_widget = rtt_widget;
    g_signal_connect( o.rtt_widget, "configure-event", G_CALLBACK( o.rtt_signal_configure_event ), o );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:shell_s) rtt_close
{
    if( o.rtt_widget == NULL ) = 0;
    g_signal_handlers_disconnect_by_data( o.rtt_widget, o );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:shell_s) get_placement
{
    o.mutex.lock();
    if( placement ) placement.copy( o.placement );
    o.mutex.unlock();
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:shell_s) set_placement
{
    o.mutex.lock();
    if( placement ) o.placement.copy( placement );
    o.mutex.unlock();
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:shell_s) cycle
{
    o.mutex.lock();
    :s* placement = o.modified ? o.placement.clone()^ : NULL;
    o.modified = false;
    o.mutex.unlock();

    if( placement )
    {
        tp_t action_type = escalate~;
        frame.client_placement_changed( frame, placement, action_type );

        if( o.broadcast_distraction )
        {
            tp_t action_type = escalate~;
            frame.client_distraction( frame, action_type );
        }
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

