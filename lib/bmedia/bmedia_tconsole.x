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

/** Terminal (Text-based) User Interface (text, keyboard, mouse)
 *  - uses ncurses API
 *  - linker: -lncurses
 *
 *  Usage:
 *    - Instantiate bmedia_tconsole_s;
 *       - Calling 'setup' is optional. It is useful to control at which point the terminal is switched into GUI mode.
 *         Use argument 'NULL', if no input is desired.
 *       - Calling 'close' is optional. It is useful to temporarily leave the GUI mode.
 *
 *    - For text output use window-functions: w_...
 *
 *    - For input
 *       - (if needed) overload bmedia_event.on_keyboard
 *       - (if needed) overload bmedia_event.on_mouse
 *       - call 'cycle' periodically (e.g. in a loop)
 *       - see group 'event' for more details
 *
 *  Thread safe encapsulation of ncurses interface.
 *  -> Multiple instances of bmedia_tconsole_s in different threads are allowed.
 *
 *  Issue while bmedia_console_s is active:
 *     In messages send to the terminal via stdout or stderr a newline is not treated as
 *     carriage return. This can lead to unexpected text-indentations.
 *
 *     Remedies (choose a suitable option)
 *       - bmedia_console_s is also a x_sink perspective with correct newline handling.
 *       - Use member functions msg_fa, err_fa.
 *       - Call function 'close' or destroy the instance (this resets the terminal). If needed, re-open it later.
 *
 *
 *  Simple Example

    stamp example_s
    {
        bmedia_tconsole_s console;
        bl_t exit;

        func bmedia_tconsole_event.on_keyboard
        {
            if( event.chr == 'q' ) o.exit = true;
            event.to_sink( o.console );
            = 0;
        }

        // displays action type at mouse position
        func bmedia_tconsole_event.on_mouse
        {
            o.console.w_clear();
            o.console.w_color( yellow~, blue~, 3 );
            o.console.w_text_fa( event.x, event.y, "#name", event.action );
            o.console.w_show();
            = 0;
        }

        func er_t run( m@* o )
        {
            while( !o.exit )
            {
                o.console.cycle( o );
                x_threads_sleep_ms( 10 );
            }
            = 0;
        }
    }

 */

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

group :attributes
{
    /// available colors
    name black, blue, green, cyan, red, magenta, yellow, white;

    stamp :s
    {
        u0_t brightness = 2; // 0: invisible, 1: dim, 2: normal, 3: bold
        tp_t foreground_color = white;
        tp_t background_color = black;
        bl_t underline;
        bl_t blink;

        func void set_color( m@* o, tp_t foreground, tp_t background, u0_t brightness /* [0,3] */ );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

group :event
{
    /// Overload these features as needed to capture the associated events
    feature er_t on_keyboard( m@* o, c :keyboard_s* event );
    feature er_t on_mouse   ( m@* o, c :mouse_s*    event );

    /** Key Descriptors
     *  Not all of the listed combinations below are actually possible for a given terminal.
     *  Certain functional combinations are empirically discovered for the terminals tested.
     *
     *  Type 'chr' indicates that a character-key was pressed. In that case the value chr in :keyboard_s holds
     *  the ASCII value of the associated character.
     */
    name       chr,       up,       down,       left,       right,       page_up,       page_down,       home,       end,       insert;
    name shift_chr, shift_up, shift_down, shift_left, shift_right, shift_page_up, shift_page_down, shift_home, shift_end, shift_insert;
    name  ctrl_chr,  ctrl_up,  ctrl_down,  ctrl_left,  ctrl_right,  ctrl_page_up,  ctrl_page_down,  ctrl_home,  ctrl_end,  ctrl_insert;
    name   alt_chr,   alt_up,   alt_down,   alt_left,   alt_right,   alt_page_up,   alt_page_down,   alt_home,   alt_end,   alt_insert;

    name       delete,       backspace,       enter,       escape,       print,       break,       tab;
    name shift_delete, shift_backspace, shift_enter, shift_escape, shift_print, shift_break, shift_tab;
    name  ctrl_delete,  ctrl_backspace,  ctrl_enter,  ctrl_escape,  ctrl_print,  ctrl_break,  ctrl_tab;
    name   alt_delete,   alt_backspace,   alt_enter,   alt_escape,   alt_print,   alt_break,   alt_tab;

    name       f1,       f2,       f3,       f4,       f5,       f6,       f7,       f8,       f9,       f10,       f11,       f12;
    name shift_f1, shift_f2, shift_f3, shift_f4, shift_f5, shift_f6, shift_f7, shift_f8, shift_f9, shift_f10, shift_f11, shift_f12;
    name  ctrl_f1,  ctrl_f2,  ctrl_f3,  ctrl_f4,  ctrl_f5,  ctrl_f6,  ctrl_f7,  ctrl_f8,  ctrl_f9,  ctrl_f10,  ctrl_f11,  ctrl_f12;
    name   alt_f1,   alt_f2,   alt_f3,   alt_f4,   alt_f5,   alt_f6,   alt_f7,   alt_f8,   alt_f9,   alt_f10,   alt_f11,   alt_f12;

    /// outputs compact event status (single line)
    feature void to_sink( @* o, m x_sink* sink );

    /// events of the keyboard
    stamp :keyboard_s
    {
        /// Key Descriptor (s. list above)
        tp_t key;

        /** Associated character when key is 'chr' or 'c_chr'
         *  0 for all other key events
         */
        u0_t chr;

        /// outputs compact event status (single line)
        func void to_sink( @* o, m x_sink* sink );
    }

    /// Mouse Action Types
    name moved;
    name button1_down, button1_up;
    name button2_down, button2_up;
    name button3_down, button3_up;
    name wheel_fwd; // wheel forward (away from user)
    name wheel_bwd; // wheel backward (towards user)

    /// events of the mouse
    stamp :mouse_s
    {
        /** Mouse Action Type (see list above)
         *  This value indicates what kind of mouse action has triggered the event.
         */
        tp_t action;

        /** Mouse position at given event.
         *  These are character coordinates in the terminal-window.
         *  The upper-left position is (0,0)
         */
        s2_t x;
        s2_t y;

        /// State of ctl/alt/shift keyboard keys at given event. (true == pressed)
        bl_t shift;
        bl_t ctrl;
        bl_t alt;

        /// outputs compact event status (single line)
        func void to_sink( @* o, m x_sink* sink );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp :s
{
    /// parameters -----------------------------

    /** Attributes for text output (functions w_...)
     *  Will be modified by functions w_attr and w_color
     */
    :attributes_s attributes;

    /// set 'true' to explore unhandled signals
    bl_t report_unhandled_keys_to_stdout;

    /// ----------------------------------------

    hidden WINDOW* window;
    hidden SCREEN* screen;
    hidden :event* callback;
    hidden x_mutex_s* curses_mutex; // global mutex protecting ncurses interface at concurrent access

    /// last recorded event
    hidden :event_keyboard_s => event_keyboard;
    hidden :event_mouse_s    => event_mouse;

    /** Optional setup (also called by cycle in case the callback changed)
     *  callback can be NULL if not needed or unknown at the time of setup.
     */
    func er_t setup( m@* o, m :event* callback );

    /** Optional close
     *  - cleans up terminal
     *  - cleans up ncurses
     *  - is called on destruction
     */
    func er_t close( m@* o );

    /** To be called periodically to capture key and mouse events (no explicit setup needed)
     *  'callback' can be NULL, if already specified during setup.
     *  'callback' can also change, in which case a new setup with new callback is triggered.
     *  This function empties the event buffer (if any), hence multiple callbacks can occur per cycle.
     *  If the event buffer is empty, this function returns immediately without error.
     */
    func er_t cycle( m@* o, m :event* callback );

    /** Allows this object to be used as (text-based) x_sink.
     *  - converts '\n' to '\r\n' for proper display on the terminal
     */
    func bcore_fp.flow_snk;

    /** Window operations below provide GUI text controls
     *  Use preferably for text output.
     */

    /// obtains window dimensions
    func er_t w_get_size( m@* o, m sz_t* width, m sz_t* height );

    /// sets text attributes (see attributes_s)
    func er_t w_attr( m@* o, :attributes_s* attr );

    /// sets text color and brightness (see attributes_s color names)
    func er_t w_color( m@* o, tp_t foreground, tp_t background, u0_t brightness /* [0,3] */ );

    /// outputs text at given position (might not be visible until 'w_show' is called)
    func er_t w_text   ( m@* o, sz_t x, sz_t y, sc_t text );
    func er_t w_text_fv( m@* o, sz_t x, sz_t y, sc_t format, va_list args );
    func er_t w_text_fa( m@* o, sz_t x, sz_t y, sc_t format, ... );

    /// clears content
    func er_t w_clear( m@* o );

    /// displays all changes from above functions
    func er_t w_show( m@* o );

    /** (Standard) text output at cursor position with specified attributes.
     *  Use these for simple messaging while console is active.
     */
    func er_t msg_fv( c @* o, sc_t format, va_list args );
    func er_t msg_fa( c @* o, sc_t format, ... );
    func er_t err_fv( c @* o, sc_t format, va_list args );
    func er_t err_fa( c @* o, sc_t format, ... );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

embed "bmedia_tconsole.emb.x";

