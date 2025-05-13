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

include <ncurses.h>;
// ncurses-specific types and functions (declared in ncurses.h) are listed below to make them visible to xoico

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// supportive functions

//----------------------------------------------------------------------------------------------------------------------

func s2_t color_type_from_index( sz_t index )
{
    switch( index )
    {
        case 0: = COLOR_BLACK;
        case 1: = COLOR_BLUE;
        case 2: = COLOR_GREEN;
        case 3: = COLOR_CYAN;
        case 4: = COLOR_RED;
        case 5: = COLOR_MAGENTA;
        case 6: = COLOR_YELLOW;
        case 7: = COLOR_WHITE;
        default: GERR_fa( "Invalid color index" );
    }
    = COLOR_BLACK;
}

//----------------------------------------------------------------------------------------------------------------------

func s2_t color_index( tp_t type )
{
    switch( type )
    {
        case black~:   = 0;
        case blue~:    = 1;
        case green~:   = 2;
        case cyan~:    = 3;
        case red~:     = 4;
        case magenta~: = 5;
        case yellow~:  = 6;
        case white~:   = 7;
        default: GERR_fa( "Invalid color type '#name'", type );
    }
    = COLOR_BLACK;
}

//----------------------------------------------------------------------------------------------------------------------

/** Converts a color-pair into a color-pair index.
 *  color-pair index 0 is reserved for the default color of the terminal
 *  and cannot be customized
 */
func s2_t color_pair_index( sz_t foregound_index, sz_t background_index ) = 1 + ( background_index << 3 ) + foregound_index;

//----------------------------------------------------------------------------------------------------------------------

func er_t setup_color_palette()
{
    for( sz_t bci = 0; bci < 8; bci++ )
    {
        for( sz_t fci = 0; fci < 8; fci++ )
        {
            if( init_pair( :color_pair_index( fci, bci ), :color_type_from_index( fci ), :color_type_from_index( bci ) ) == ERR )
            {
                = GERR_fa( "ncurses: 'init_pair' failed" );
            }
        }
    }
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

// converts ncurses key code into a string for logging and debugging purposes; returns NULL in case of no match
func sc_t ncurses_key_name( s2_t key )
{
    switch( key )
    {
        case KEY_CODE_YES:  = "KEY_CODE_YES";
        case KEY_BREAK:     = "KEY_BREAK";
        case KEY_DOWN:      = "KEY_DOWN";
        case KEY_UP:        = "KEY_UP";
        case KEY_LEFT:      = "KEY_LEFT";
        case KEY_RIGHT:     = "KEY_RIGHT";
        case KEY_HOME:      = "KEY_HOME";
        case KEY_BACKSPACE: = "KEY_BACKSPACE";
        case KEY_F0:        = "KEY_F0";
        case KEY_DL:        = "KEY_DL";
        case KEY_IL:        = "KEY_IL";
        case KEY_DC:        = "KEY_DC";
        case KEY_IC:        = "KEY_IC";
        case KEY_EIC:       = "KEY_EIC";
        case KEY_CLEAR:     = "KEY_CLEAR";
        case KEY_EOS:       = "KEY_EOS";
        case KEY_EOL:       = "KEY_EOL";
        case KEY_SF:        = "KEY_SF";
        case KEY_SR:        = "KEY_SR";
        case KEY_NPAGE:     = "KEY_NPAGE";
        case KEY_PPAGE:     = "KEY_PPAGE";
        case KEY_STAB:      = "KEY_STAB";
        case KEY_CTAB:      = "KEY_CTAB";
        case KEY_CATAB:     = "KEY_CATAB";
        case KEY_ENTER:     = "KEY_ENTER";
        case KEY_SRESET:    = "KEY_SRESET";
        case KEY_RESET:     = "KEY_RESET";
        case KEY_PRINT:     = "KEY_PRINT";
        case KEY_LL:        = "KEY_LL";
        case KEY_A1:        = "KEY_A1";
        case KEY_A3:        = "KEY_A3";
        case KEY_B2:        = "KEY_B2";
        case KEY_C1:        = "KEY_C1";
        case KEY_C3:        = "KEY_C3";
        case KEY_BTAB:      = "KEY_BTAB";
        case KEY_BEG:       = "KEY_BEG";
        case KEY_CANCEL:    = "KEY_CANCEL";
        case KEY_CLOSE:     = "KEY_CLOSE";
        case KEY_COMMAND:   = "KEY_COMMAND";
        case KEY_COPY:      = "KEY_COPY";
        case KEY_CREATE:    = "KEY_CREATE";
        case KEY_END:       = "KEY_END";
        case KEY_EXIT:      = "KEY_EXIT";
        case KEY_FIND:      = "KEY_FIND";
        case KEY_HELP:      = "KEY_HELP";
        case KEY_MARK:      = "KEY_MARK";
        case KEY_MESSAGE:   = "KEY_MESSAGE";
        case KEY_MOVE:      = "KEY_MOVE";
        case KEY_NEXT:      = "KEY_NEXT";
        case KEY_OPEN:      = "KEY_OPEN";
        case KEY_OPTIONS:   = "KEY_OPTIONS";
        case KEY_PREVIOUS:  = "KEY_PREVIOUS";
        case KEY_REDO:      = "KEY_REDO";
        case KEY_REFERENCE: = "KEY_REFERENCE";
        case KEY_REFRESH:   = "KEY_REFRESH";
        case KEY_REPLACE:   = "KEY_REPLACE";
        case KEY_RESTART:   = "KEY_RESTART";
        case KEY_RESUME:    = "KEY_RESUME";
        case KEY_SAVE:      = "KEY_SAVE";
        case KEY_SBEG:      = "KEY_SBEG";
        case KEY_SCANCEL:   = "KEY_SCANCEL";
        case KEY_SCOMMAND:  = "KEY_SCOMMAND";
        case KEY_SCOPY:     = "KEY_SCOPY";
        case KEY_SCREATE:   = "KEY_SCREATE";
        case KEY_SDC:       = "KEY_SDC";
        case KEY_SDL:       = "KEY_SDL";
        case KEY_SELECT:    = "KEY_SELECT";
        case KEY_SEND:      = "KEY_SEND";
        case KEY_SEOL:      = "KEY_SEOL";
        case KEY_SEXIT:     = "KEY_SEXIT";
        case KEY_SFIND:     = "KEY_SFIND";
        case KEY_SHELP:     = "KEY_SHELP";
        case KEY_SHOME:     = "KEY_SHOME";
        case KEY_SIC:       = "KEY_SIC";
        case KEY_SLEFT:     = "KEY_SLEFT";
        case KEY_SMESSAGE:  = "KEY_SMESSAGE";
        case KEY_SMOVE:     = "KEY_SMOVE";
        case KEY_SNEXT:     = "KEY_SNEXT";
        case KEY_SOPTIONS:  = "KEY_SOPTIONS";
        case KEY_SPREVIOUS: = "KEY_SPREVIOUS";
        case KEY_SPRINT:    = "KEY_SPRINT";
        case KEY_SREDO:     = "KEY_SREDO";
        case KEY_SREPLACE:  = "KEY_SREPLACE";
        case KEY_SRIGHT:    = "KEY_SRIGHT";
        case KEY_SRSUME:    = "KEY_SRSUME";
        case KEY_SSAVE:     = "KEY_SSAVE";
        case KEY_SSUSPEND:  = "KEY_SSUSPEND";
        case KEY_SUNDO:     = "KEY_SUNDO";
        case KEY_SUSPEND:   = "KEY_SUSPEND";
        case KEY_UNDO:      = "KEY_UNDO";
        default: break;
    }

    = NULL;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func (:attributes_s) set_color
{
    o.foreground_color = foreground;
    o.background_color = background;
    o.brightness = brightness;
}

//----------------------------------------------------------------------------------------------------------------------

func (:attributes_s) er_t apply( @* o, m WINDOW* window )
{
    u2_t attr = 0;

    switch( o.brightness )
    {
        case 0: attr |= WA_INVIS; break;
        case 1: attr |= WA_DIM; break;
        case 2: break;
        case 3: attr |= WA_BOLD; break;
        default: break;
    }

    if( o.underline ) attr |= WA_UNDERLINE;
    if( o.blink     ) attr |= WA_BLINK;

    sz_t color_pair_index = ::color_pair_index( ::color_index( o.foreground_color ), ::color_index( o.background_color ) );

    if( wattr_set( window, attr, color_pair_index, NULL ) == ERR ) = GERR_fa( "ncurses: 'wattr_set' failed" );

    = 0;
}


//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

name :curses_mutex;

func (:s) setup
{
    if( o.window != NULL && callback == o.callback ) = 0;
    if( o.window != NULL ) o.close();
    o.callback = callback;
    o.curses_mutex = bcore_global_get_ifnexists_set( :curses_mutex~, x_mutex_s~ );
    o.curses_mutex.create_lock()^;

    // initialize ncurses

    o.screen = newterm( getenv("TERM"), stdout, stdin );
    if( !o.screen ) = GERR_fa( "ncurses: 'newterm' failed." );

    o.window = stdscr;

    //o.window = initscr();
    if( !o.window ) = GERR_fa( "ncurses: 'initscr' failed." );

    if( o.callback )
    {
        if( callback.defines_on_mouse() )
        {
            // xterm control sequence: Cell Mouse Motion Tracking
            // Details: https://www.xfree86.org/4.8.0/ctlseqs.html
            // Credits: https://gist.github.com/sylt/93d3f7b77e7f3a881603
            printf( "\x1b[?1002h\n" );
        }

        // no line buffering
        if( cbreak() == ERR ) = GERR_fa( "ncurses: 'cbreak' failed." );

         // enables control keys (F-keys, arrow-keys, ... )
        if( keypad( o.window, TRUE ) == ERR ) = GERR_fa( "ncurses: 'keypad' failed." );

        // does not print a keystroke on screen
        if( noecho() == ERR ) = GERR_fa( "ncurses: 'noecho' failed." );

        // prevents blocking getch
        if( nodelay( o.window, TRUE ) == ERR ) = GERR_fa( "ncurses: 'nodelay' failed." );

        if( callback.defines_on_mouse() )
        {
            if( mousemask( ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL ) == ERR )  = GERR_fa( "ncurses: 'mousemask' failed." );
            if( mouseinterval( 0 ) == ERR )  = GERR_fa( "ncurses: 'mouseinterval' failed." );
        }
    }

    // setup color handling
    if( has_colors() )
    {
        if( start_color() == ERR ) GERR_fa( "curses: start_color failed." );
        :setup_color_palette();
    }

    // makes cursor invisible
    if( curs_set( 0 ) == ERR ) = GERR_fa( "ncurses: 'curs_set' failed." );

    o.attributes.apply( o.window );

    // explicit refresh after setup seems necessary to keep ncurses from garbling messages
    if( refresh() == ERR ) = GERR_fa( "ncurses: 'refresh' failed." );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) close
{
    if( o.window )
    {

        o.curses_mutex.create_lock()^;

        if( endwin() == ERR ) = GERR_fa( "curses: endwin failed." );
        if( o.screen ) delscreen( o.screen );
        o.window = NULL;
        o.screen = NULL;

        if( o.callback.defines_on_mouse() )
        {
            // undo xterm control sequence (s. setup)
            printf( "\x1b[?1002l\n" );
        }
    }

    o.curses_mutex = NULL;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:event_keyboard_s) er_t setup( m@* o, tp_t key, u0_t chr )
{
    o.key = key;
    o.chr = chr;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

name :unhandled_key_code;

func (:event_keyboard_s) er_t from_key_code( m@* o, s2_t key_code )
{
    if( key_code >= 0x20 && key_code <= 0x7E ) = o.setup( TYPEOF_chr, key_code );

    /// CON_ ... values are empirically determined and might not work for all terminals
    #define CON_S_F0 0x0114
    #define CON_C_F0 0x0120
    #define CON_A_F0 0x0138

    switch( key_code )
    {
        case KEY_UP:        = o.setup( TYPEOF_up,        0 );
        case KEY_DOWN:      = o.setup( TYPEOF_down,      0 );
        case KEY_LEFT:      = o.setup( TYPEOF_left,      0 );
        case KEY_RIGHT:     = o.setup( TYPEOF_right,     0 );

        case KEY_SR:        = o.setup( TYPEOF_shift_up,    0 );
        case KEY_SF:        = o.setup( TYPEOF_shift_down,  0 );
        case KEY_SLEFT:     = o.setup( TYPEOF_shift_left,  0 );
        case KEY_SRIGHT:    = o.setup( TYPEOF_shift_right, 0 );

        case 0x0237:        = o.setup( TYPEOF_ctrl_up,     0 );
        case 0x020E:        = o.setup( TYPEOF_ctrl_down,   0 );
        case 0x0231:        = o.setup( TYPEOF_ctrl_right,  0 );
        case 0x0222:        = o.setup( TYPEOF_ctrl_left,   0 );

        case 0x0235:        = o.setup( TYPEOF_alt_up,      0 );
        case 0x020C:        = o.setup( TYPEOF_alt_down,    0 );
        case 0x0220:        = o.setup( TYPEOF_alt_right,   0 );
        case 0x022F:        = o.setup( TYPEOF_alt_left,    0 );

        case KEY_BACKSPACE: = o.setup( TYPEOF_backspace, 0 );

        case KEY_PRINT:     = o.setup( TYPEOF_print,     0 );
        case KEY_BREAK:     = o.setup( TYPEOF_break,     0 );

        case KEY_IC:        = o.setup( TYPEOF_insert,    0 );
        case KEY_DC:        = o.setup( TYPEOF_delete,    0 );
        case KEY_HOME:      = o.setup( TYPEOF_home,      0 );
        case KEY_END:       = o.setup( TYPEOF_end,       0 );
        case KEY_PPAGE:     = o.setup( TYPEOF_page_up,   0 );
        case KEY_NPAGE:     = o.setup( TYPEOF_page_down, 0 );

        case KEY_SDC:       = o.setup( TYPEOF_shift_delete,    0 );
        case KEY_SHOME:     = o.setup( TYPEOF_shift_home,      0 );
        case KEY_SEND:      = o.setup( TYPEOF_shift_end,       0 );
        case KEY_SPREVIOUS: = o.setup( TYPEOF_shift_page_up,   0 );
        case KEY_SNEXT:     = o.setup( TYPEOF_shift_page_down, 0 );

        case 0x0208:        = o.setup( TYPEOF_ctrl_delete,    0 );
        case 0x0218:        = o.setup( TYPEOF_ctrl_home,      0 );
        case 0x0213:        = o.setup( TYPEOF_ctrl_end,       0 );
        case 0x022C:        = o.setup( TYPEOF_ctrl_page_up,   0 );
        case 0x0227:        = o.setup( TYPEOF_ctrl_page_down, 0 );

        case 0x021B:        = o.setup( TYPEOF_alt_insert,    0 );
        case 0x0206:        = o.setup( TYPEOF_alt_delete,    0 );
        case 0x0216:        = o.setup( TYPEOF_alt_home,      0 );
        case 0x0211:        = o.setup( TYPEOF_alt_end,       0 );
        case 0x022A:        = o.setup( TYPEOF_alt_page_up,   0 );
        case 0x0225:        = o.setup( TYPEOF_alt_page_down, 0 );

        case 0x001B:        = o.setup( TYPEOF_escape,    0 );
        case 0x000A:        = o.setup( TYPEOF_enter,     0 );
        case KEY_ENTER:     = o.setup( TYPEOF_enter,     0 );

        case 0x0009:        = o.setup( TYPEOF_tab,       0 );
        case KEY_BTAB:      = o.setup( TYPEOF_shift_tab, 0 );

        case KEY_F0 +  1:   = o.setup( TYPEOF_f1,        0 );
        case KEY_F0 +  2:   = o.setup( TYPEOF_f2,        0 );
        case KEY_F0 +  3:   = o.setup( TYPEOF_f3,        0 );
        case KEY_F0 +  4:   = o.setup( TYPEOF_f4,        0 );
        case KEY_F0 +  5:   = o.setup( TYPEOF_f5,        0 );
        case KEY_F0 +  6:   = o.setup( TYPEOF_f6,        0 );
        case KEY_F0 +  7:   = o.setup( TYPEOF_f7,        0 );
        case KEY_F0 +  8:   = o.setup( TYPEOF_f8,        0 );
        case KEY_F0 +  9:   = o.setup( TYPEOF_f9,        0 );
        case KEY_F0 + 10:   = o.setup( TYPEOF_f10,       0 );
        case KEY_F0 + 11:   = o.setup( TYPEOF_f11,       0 );
        case KEY_F0 + 12:   = o.setup( TYPEOF_f12,       0 );

        case CON_S_F0 +  1: = o.setup( TYPEOF_shift_f1,  0 );
        case CON_S_F0 +  2: = o.setup( TYPEOF_shift_f2,  0 );
        case CON_S_F0 +  3: = o.setup( TYPEOF_shift_f3,  0 );
        case CON_S_F0 +  4: = o.setup( TYPEOF_shift_f4,  0 );
        case CON_S_F0 +  5: = o.setup( TYPEOF_shift_f5,  0 );
        case CON_S_F0 +  6: = o.setup( TYPEOF_shift_f6,  0 );
        case CON_S_F0 +  7: = o.setup( TYPEOF_shift_f7,  0 );
        case CON_S_F0 +  8: = o.setup( TYPEOF_shift_f8,  0 );
        case CON_S_F0 +  9: = o.setup( TYPEOF_shift_f9,  0 );
        case CON_S_F0 + 10: = o.setup( TYPEOF_shift_f10, 0 );
        case CON_S_F0 + 11: = o.setup( TYPEOF_shift_f11, 0 );
        case CON_S_F0 + 12: = o.setup( TYPEOF_shift_f12, 0 );

        case CON_C_F0 +  1: = o.setup( TYPEOF_ctrl_f1,   0 );
        case CON_C_F0 +  2: = o.setup( TYPEOF_ctrl_f2,   0 );
        case CON_C_F0 +  3: = o.setup( TYPEOF_ctrl_f3,   0 );
        case CON_C_F0 +  4: = o.setup( TYPEOF_ctrl_f4,   0 );
        case CON_C_F0 +  5: = o.setup( TYPEOF_ctrl_f5,   0 );
        case CON_C_F0 +  6: = o.setup( TYPEOF_ctrl_f6,   0 );
        case CON_C_F0 +  7: = o.setup( TYPEOF_ctrl_f7,   0 );
        case CON_C_F0 +  8: = o.setup( TYPEOF_ctrl_f8,   0 );
        case CON_C_F0 +  9: = o.setup( TYPEOF_ctrl_f9,   0 );
        case CON_C_F0 + 10: = o.setup( TYPEOF_ctrl_f10,  0 );
        case CON_C_F0 + 11: = o.setup( TYPEOF_ctrl_f11,  0 );
        case CON_C_F0 + 12: = o.setup( TYPEOF_ctrl_f12,  0 );

        case CON_A_F0 +  1: = o.setup( TYPEOF_alt_f1,    0 );
        case CON_A_F0 +  2: = o.setup( TYPEOF_alt_f2,    0 );
        case CON_A_F0 +  3: = o.setup( TYPEOF_alt_f3,    0 );
        case CON_A_F0 +  4: = o.setup( TYPEOF_alt_f4,    0 );
        case CON_A_F0 +  5: = o.setup( TYPEOF_alt_f5,    0 );
        case CON_A_F0 +  6: = o.setup( TYPEOF_alt_f6,    0 );
        case CON_A_F0 +  7: = o.setup( TYPEOF_alt_f7,    0 );
        case CON_A_F0 +  8: = o.setup( TYPEOF_alt_f8,    0 );
        case CON_A_F0 +  9: = o.setup( TYPEOF_alt_f9,    0 );
        case CON_A_F0 + 10: = o.setup( TYPEOF_alt_f10,   0 );
        case CON_A_F0 + 11: = o.setup( TYPEOF_alt_f11,   0 );
        case CON_A_F0 + 12: = o.setup( TYPEOF_alt_f12,   0 );

        default: break;
    }

    if( key_code >= 0x00 && key_code <= 0x1A ) = o.setup( TYPEOF_ctrl_chr, key_code + 0x60 );

    = TYPEOF_bmedia_tconsole_unhandled_key_code;
}

func (:event_keyboard_s) to_sink
{
    if( o.key == TYPEOF_chr )
    {
        sink.push_fa( "chr: #<char>", (char)o.chr );
    }
    else if( o.key == TYPEOF_ctrl_chr )
    {
        sink.push_fa( "c_chr: #<char>", (char)o.chr );
    }
    else
    {
        sink.push_fa( "#name", o.key );
    }

    sink.push_fa( "\n" );
}

//----------------------------------------------------------------------------------------------------------------------

func (:event_mouse_s) to_sink
{
    sink.push_fa( "#p20 {#name} (#pl3'0'{#<s2_t>},#pl3'0'{#<s2_t>})", o.action, o.x, o.y );
    if( o.shift ) sink.push_fa( " shift" );
    if( o.ctrl  ) sink.push_fa( " ctrl" );
    if( o.alt   ) sink.push_fa( " alt" );
    sink.push_fa( "\n" );
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t single_cycle( m@* o, m bl_t* success )
{
    o.curses_mutex.lock();
    u2_t key_code = wgetch( o.window );
    o.curses_mutex.unlock();

    if( key_code == ERR )
    {
        success.0 = false;
        = 0;
    }

    if( key_code == KEY_MOUSE )
    {
        if( o.callback.defines_on_mouse() )
        {
            o.event_mouse!;

            u2_t cerr = 0;

            o.curses_mutex.lock();

            verbatim_C
            {
                MEVENT mevent = {0};
                cerr = getmouse( &mevent );
            }

            o.curses_mutex.unlock();
            if( cerr == ERR ) = GERR_fa( "getmouse failed" );

            verbatim_C
            {
                s2_t x = mevent.x;
                s2_t y = mevent.y;
                // mevent.z is reserved for later use

                o->event_mouse->x = x;
                o->event_mouse->y = y;

                // mouse button event
                if     ( ( mevent.bstate & REPORT_MOUSE_POSITION ) ) o->event_mouse->action = TYPEOF_moved;
                else if( ( mevent.bstate & BUTTON1_PRESSED  ) ) o->event_mouse->action = TYPEOF_button1_down;
                else if( ( mevent.bstate & BUTTON1_RELEASED ) ) o->event_mouse->action = TYPEOF_button1_up;
                else if( ( mevent.bstate & BUTTON2_PRESSED  ) ) o->event_mouse->action = TYPEOF_button2_down;
                else if( ( mevent.bstate & BUTTON2_RELEASED ) ) o->event_mouse->action = TYPEOF_button2_up;
                else if( ( mevent.bstate & BUTTON3_PRESSED  ) ) o->event_mouse->action = TYPEOF_button3_down;
                else if( ( mevent.bstate & BUTTON3_RELEASED ) ) o->event_mouse->action = TYPEOF_button3_up;
                else if( ( mevent.bstate & BUTTON4_PRESSED  ) ) o->event_mouse->action = TYPEOF_wheel_fwd;
                else if( ( mevent.bstate & BUTTON5_PRESSED  ) ) o->event_mouse->action = TYPEOF_wheel_bwd;
                else                                            o->event_mouse->action = 0;

                o->event_mouse->shift = ( mevent.bstate & BUTTON_SHIFT ) ? true : false;
                o->event_mouse->ctrl  = ( mevent.bstate & BUTTON_CTRL  ) ? true : false;
                o->event_mouse->alt   = ( mevent.bstate & BUTTON_ALT   ) ? true : false;

                //bcore_msg_fa( "#X<u2_t>\r\n", ( u2_t )mevent.bstate );
            }

            o.callback.on_mouse( o.event_mouse );
        }
    }
    else if( o.callback.defines_on_keyboard() )
    {
        er_t err = o.event_keyboard!.from_key_code( key_code );

        if( err == 0 )
        {
             o.callback.on_keyboard( o.event_keyboard );
        }
        else if( err == TYPEOF_bmedia_tconsole_unhandled_key_code )
        {
            if( o.report_unhandled_keys_to_stdout )
            {
                sc_t ncurses_name = :ncurses_key_name( key_code );

                if( ncurses_name )
                {
                    bcore_msg_fa( "\r\nUnhandled keycode 0x#X<u2_t>; ncurses name: #<sc_t>\r\n", (u2_t)key_code, ncurses_name );
                }
                else
                {
                    bcore_msg_fa( "\r\nUnhandled keycode 0x#X<u2_t>\r\n", (u2_t)key_code );
                }
            }
        }
        else
        {
            = err;
        }
    }

    success.0 = true;

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) cycle
{
    if( callback && o.callback != callback ) o.setup( callback );
    if( !o.callback ) = GERR_fa( "No callback defined." );

    bl_t success = true;
    while( success ) o.single_cycle( success.1 );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) bcore_inst_call.down_e
{
    o.close();
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) msg_fv
{
    o.cast( m x_sink* ).push_fv( format, args );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) msg_fa
{
    va_list a;
    va_start( a, format );
    er_t err = o.msg_fv( format, a );
    va_end( a );
    = err;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) err_fv
{
    o.cast( m x_sink* ).push_fv( format, args );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) err_fa
{
    va_list a;
    va_start( a, format );
    er_t err = o.err_fv( format, a );
    va_end( a );
    = err;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) flow_snk
{
    if( !o.window ) o.setup( NULL );
    m$* st = st_s!^;
    for( uz_t i = 0; i < size; i++ )
    {
        u0_t c = ( ( u0_t* )data )[ i ];
        st.push_char( c );
        if( c == '\n' ) st.push_char( '\r' );
    }

    printf( "%s", st.sc );
    refresh();

    = size;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) w_get_size
{
    if( !o.window ) o.setup( NULL );
    o.curses_mutex.create_lock()^;
    int r = 0; int c = 0;

    /// getmaxyx is a macro modifying r, c
    getmaxyx( o->window, r, c );

    if( width.1  ) width.0 = c;
    if( height.1 ) height.0 = r;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) w_attr
{
    if( !o.window ) o.setup( NULL );
    o.curses_mutex.create_lock()^;
    o.attributes.copy( attr );
    o.attributes.apply( o.window );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) w_color
{
    if( !o.window ) o.setup( NULL );
    o.curses_mutex.create_lock()^;
    o.attributes.set_color( foreground, background, brightness );
    o.attributes.apply( o.window );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) w_text
{
    if( !o.window ) o.setup( NULL );
    o.curses_mutex.create_lock()^;
    if( mvwaddstr( o.window, y, x, text ) == ERR ) = GERR_fa( "ncurses: 'mvwaddstr' failed\n" );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) w_text_fv
{
    m$* st = st_s!^.push_fv( format, args );
    = o.w_text( x, y, st.sc );
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) w_text_fa
{
    va_list a; va_start( a, format );
    er_t err = o.w_text_fv( x, y, format, a );
    va_end( a );
    = err;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) w_clear
{
    if( !o.window ) o.setup( NULL );
    o.curses_mutex.create_lock()^;
    if( wclear( o.window ) == ERR ) = GERR_fa( "ncurses: 'refresh' failed\n" );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) w_show
{
    if( !o.window ) o.setup( NULL );
    o.curses_mutex.create_lock()^;
    if( refresh() == ERR ) = GERR_fa( "ncurses: 'refresh' failed\n" );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

