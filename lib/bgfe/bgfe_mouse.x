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

 /// Mouse state.

type GdkEventButton;
type GdkEventCrossing;
type GdkEventMotion;

forward bgfe_frame;

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp :s
{
    // state of mouse buttons (true = down)
    bl_t button1;
    bl_t button2;
    bl_t button3;

    // state related keyboard keys (true = down)
    bl_t shift;
    bl_t ctl;
    bl_t alt;

    // mouse position (relative to window/widget where state was recorded)
    f3_t x;
    f3_t y;

    // mouse scroll wheel change after scroll event
    f3_t dy;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

// modify_type
name press;
name release;
name motion;
name scroll;

stamp :shell_s
{
    :s mouse;
    tp_t modified_type; // modifies type: see list above; 0: not modified
    hidden bgfe_frame* parent; // parent frame
    hidden GtkWidget* rtt_widget;
    x_mutex_s mutex;
    hidden GObject* rtt_event_controller_scroll;

    /// interface functions
    func er_t setup( m@* o, m bgfe_frame* parent ) { o.parent = parent; = 0; }
    func er_t rtt_open   ( m@* o, m GtkWidget* rtt_widget );
    func er_t rtt_close( m@* o );
    func er_t get_mouse( m@* o, m :s* mouse );
    func er_t set_mouse( m@* o, c :s* mouse );
    func er_t cycle( m@* o, m bgfe_frame* frame );

}

//----------------------------------------------------------------------------------------------------------------------

func (:shell_s) gboolean rtt_signal_button_press_event( m GtkWidget* drw, GdkEventButton* event, m@* o )
{
    o.mutex.lock();
    o.modified_type = press~;
    switch( verbatim_C{ event->button } )
    {
        case 1: o.mouse.button1 = true; break;
        case 2: o.mouse.button2 = true; break;
        case 3: o.mouse.button3 = true; break;
        default: break;
    }

    o.mouse.shift = verbatim_C{ ( event->state & GDK_SHIFT_MASK )   != 0 };
    o.mouse.ctl   = verbatim_C{ ( event->state & GDK_CONTROL_MASK ) != 0 };
    o.mouse.alt   = verbatim_C{ ( event->state & GDK_MOD1_MASK )    != 0 };
    o.mouse.dy = 0;
    o.mutex.unlock();
    = FALSE;
}

//----------------------------------------------------------------------------------------------------------------------

func (:shell_s) gboolean rtt_signal_button_release_event( m GtkWidget* drw, GdkEventButton* event, m@* o )
{
    o.mutex.lock();
    o.modified_type = release~;
    switch( verbatim_C{ event->button } )
    {
        case 1: o.mouse.button1 = false; break;
        case 2: o.mouse.button2 = false; break;
        case 3: o.mouse.button3 = false; break;
        default: break;
    }

    o.mouse.shift = verbatim_C{ ( event->state & GDK_SHIFT_MASK )   != 0 };
    o.mouse.ctl   = verbatim_C{ ( event->state & GDK_CONTROL_MASK ) != 0 };
    o.mouse.alt   = verbatim_C{ ( event->state & GDK_MOD1_MASK )    != 0 };
    o.mouse.dy = 0;
    o.mutex.unlock();
    = FALSE;
}

//----------------------------------------------------------------------------------------------------------------------

func (:shell_s) gboolean rtt_signal_enter_notify_event ( m GtkWidget* drw, GdkEventCrossing* event, m@* o )
{
    o.mutex.lock();
    o.modified_type = motion~;
    o.mouse.x = verbatim_C{ event->x };
    o.mouse.y = verbatim_C{ event->y };
    o.mouse.dy = 0;
    o.mutex.unlock();
    = FALSE;
}

//----------------------------------------------------------------------------------------------------------------------

func (:shell_s) gboolean rtt_signal_leave_notify_event ( m GtkWidget* drw, GdkEventCrossing* event, m@* o )
{
    o.mutex.lock();
    o.modified_type = motion~;
    o.mouse.x = verbatim_C{ event->x };
    o.mouse.y = verbatim_C{ event->y };
    o.mouse.dy = 0;
    o.mutex.unlock();
    = FALSE;
}

//----------------------------------------------------------------------------------------------------------------------

func (:shell_s) gboolean rtt_signal_motion_notify_event( m GtkWidget* drw, GdkEventMotion* event, m@* o )
{
    o.mutex.lock();
    o.modified_type = motion~;
    o.mouse.x = verbatim_C{ event->x };
    o.mouse.y = verbatim_C{ event->y };
    o.mouse.dy = 0;
    o.mutex.unlock();
    = FALSE;
}

//----------------------------------------------------------------------------------------------------------------------

func (:shell_s) void rtt_signal_scroll( m GtkWidget* wg, f3_t dx, f3_t dy, m@* o )
{
    o.mutex.lock();
    o.modified_type = scroll~;
    o.mouse.dy = dy;
    o.mutex.unlock();
}

//----------------------------------------------------------------------------------------------------------------------

func (:shell_s) rtt_open
{
    if( !rtt_widget ) = 0;
    o.rtt_widget = rtt_widget;
    g_signal_connect( o.rtt_widget, "button-press-event",   G_CALLBACK( o.rtt_signal_button_press_event   ), o );
    g_signal_connect( o.rtt_widget, "button-release-event", G_CALLBACK( o.rtt_signal_button_release_event ), o );
    g_signal_connect( o.rtt_widget, "enter-notify-event",   G_CALLBACK( o.rtt_signal_enter_notify_event   ), o );
    g_signal_connect( o.rtt_widget, "leave-notify-event",   G_CALLBACK( o.rtt_signal_leave_notify_event   ), o );
    g_signal_connect( o.rtt_widget, "motion-notify-event",  G_CALLBACK( o.rtt_signal_motion_notify_event  ), o );

    o.cast( m bgfe_frame* ).rtt_attach_object( G_OBJECT( gtk_event_controller_scroll_new( o.rtt_widget, GTK_EVENT_CONTROLLER_SCROLL_VERTICAL ) ), o.rtt_event_controller_scroll );
    g_signal_connect( o.rtt_event_controller_scroll, "scroll",  G_CALLBACK( o.rtt_signal_scroll ), o );

    gtk_widget_add_events( o.rtt_widget, GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK | GDK_ENTER_NOTIFY_MASK | GDK_LEAVE_NOTIFY_MASK | GDK_SCROLL_MASK );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:shell_s) rtt_close
{

    if( o.rtt_widget ) g_signal_handlers_disconnect_by_data( o.rtt_widget, o );
    o.cast( m bgfe_frame* ).rtt_detach_object( o.rtt_event_controller_scroll );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:shell_s) get_mouse
{
    o.mutex.lock();
    if( mouse ) mouse.copy( o.mouse );
    o.mutex.unlock();
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:shell_s) set_mouse
{
    o.mutex.lock();
    if( mouse ) o.mouse.copy( mouse );
    o.mutex.unlock();
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:shell_s) cycle
{
    o.mutex.lock();
    tp_t modified_type = o.modified_type;
    :s* mouse = modified_type ? o.mouse.clone()^ : NULL;
    o.modified_type = 0;
    o.mutex.unlock();

    if( mouse )
    {
        tp_t action_type = escalate~;
        frame.client_mouse_changed( frame, mouse, modified_type, action_type );
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

