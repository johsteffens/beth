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

 /** Provides support for asynchronous location and navigation around a specified widget.
  *  For synchronous (event driven) location information, use bgfe_placement_s
  */


/**********************************************************************************************************************/

type gboolean, GtkWidget;
type GtkAllocation;

identifier TRUE, FALSE;

forward bgfe_frame;

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp :s
{
    sz_t x;
    sz_t y;

    // size of widget
    sz_t width;
    sz_t height;

    func void from_allocation( m@* o, GtkAllocation* allocation );
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) from_allocation
{
    verbatim_C
    {
        o->x = allocation->x;
        o->y = allocation->y;
        o->width  = allocation->width;
        o->height = allocation->height;
    };
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp :shell_s
{
    hidden bgfe_frame* parent;     // parent frame
    hidden bgfe_rte_s* rte;        // runtime environment

    /// interface functions
    func er_t setup( m@* o, m bgfe_frame* parent )
    {
        o.parent = parent;
        bgfe_rte_get( &o.rte );
        = 0;
    }

    func er_t get_location( m@* o, m GtkWidget* widget, m :s* location );

    func er_t get_size( m@* o, m GtkWidget* widget, m sz_t* width, m sz_t* height );

    /// upper left corner of widget in window coordinates
    func er_t window_coordinates( m@* o, m GtkWidget* widget, m sz_t* x, m sz_t* y );

    /// upper left corner of widget in screen coordinates (might not be portable in future)
    func er_t screen_coordinates( m@* o, m GtkWidget* widget, m sz_t* x, m sz_t* y );

    /// sets frame position to upper right corner of widget
    func er_t place_frame_at_upper_right( m@* o, m GtkWidget* widget, m bgfe_frame* frame );

    /// sets frame position to lower left corner of widget
    func er_t place_frame_at_lower_left( m@* o, m GtkWidget* widget, m bgfe_frame* frame );
}

//----------------------------------------------------------------------------------------------------------------------

stamp :get_location_arg_s { private GtkWidget* widget; private :s* location; }

func (:shell_s) get_location
{
    :get_location_arg_s^ arg;
    arg.widget = widget;
    arg.location = location;

    o.rte.run( o.rtt_get_location.cast( bgfe_rte_fp_rtt ), o, (vd_t)&arg );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:shell_s) get_size
{
    :s^ location;
    o.get_location( widget, location );
    if( width.1 ) width.0 = location.width;
    if( height.1 ) height.0 = location.height;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------



type GtkAllocation;
identifier gtk_widget_get_allocation;
func (:shell_s) er_t rtt_get_location( m@* o, m :get_location_arg_s* arg )
{
    GtkAllocation allocation;
    gtk_widget_get_allocation( arg.widget, &allocation );
    arg.location.from_allocation( &allocation );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

stamp :window_coordinates_arg_s { private GtkWidget* widget; sz_t x; sz_t y; }

func (:shell_s) window_coordinates
{
    :window_coordinates_arg_s^ arg;
    arg.widget = widget;
    o.rte.run( o.rtt_window_coordinates.cast( bgfe_rte_fp_rtt ), o, (vd_t)&arg );
    if( x.1 ) x.0 = arg.x;
    if( y.1 ) y.0 = arg.y;
    = 0;
}

func (:shell_s) er_t rtt_window_coordinates( m@* o, m :window_coordinates_arg_s* arg )
{
    m GtkWidget* src_widget = arg.widget;
    m bgfe_window_s* window = o.parent.nearest_window();
    if( window == NULL ) = GERR_fa( "parent: #name: o.parent.nearest_window() returned NULL.", o.parent._ );
    m GtkWidget* dst_widget = window.rtt_widget();
    if( ( src_widget == NULL ) || ( dst_widget == NULL ) ) = GERR_fa( "parent: #name: Could not obtain widgets.", o.parent._ );

    if( src_widget == dst_widget )
    {
        arg.x = 0;
        arg.y = 0;
        = 0;
    }

    bl_t success = false;

    verbatim_C
    {
        gint x = 0;
        gint y = 0;
        success = gtk_widget_translate_coordinates( src_widget, dst_widget, 0, 0, &x, &y );
        arg->x = x;
        arg->y = y;
    }

    if( !success ) = GERR_fa( "parent: #name: gtk_widget_translate_coordinates failed", o.parent._ );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:shell_s) screen_coordinates
{
    o.window_coordinates( widget, x, y );
    m bgfe_window_s* window = o.parent.nearest_window();

    bgfe_placement_s^ placement;
    window.placement_shell.get_placement( &placement );

    if( x.1 ) x.0 += placement.x;
    if( y.1 ) y.0 += placement.y;

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:shell_s) place_frame_at_upper_right
{
    sz_t width = 0;
    sz_t x = 0;
    sz_t y = 0;
    o.get_size( widget, &width, NULL );
    o.screen_coordinates( widget, x, y );
    frame.set_x( x + width );
    frame.set_y( y );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:shell_s) place_frame_at_lower_left
{
    sz_t width = 0;
    sz_t height = 0;
    sz_t x = 0;
    sz_t y = 0;
    o.get_size( widget, &width, &height );
    o.screen_coordinates( widget, x, y );
    frame.set_x( x );
    frame.set_y( y + height );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

