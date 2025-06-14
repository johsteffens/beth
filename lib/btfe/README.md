# btfe: Beth Terminal Front End

This library represents a terminal-based user interface for beth.

## btfe_console_s

Terminal (Text-based) User Interface (text, keyboard, mouse)
* Uses the ncurses API
* Linker: -lncurses

### Usage
* Instantiate btfe_console_s;
* Calling 'setup' is optional. Explicit call controls the switch into Terminal-UI mode. Use callback 'NULL', if no input is desired.
* Calling 'close' is optional. It can be helpful to temporarily leave the Terminal-UI mode. See also [issues](#issues).
* For text output use window-functions: w_...
* For input
   * (if needed) overload btfe_event.on_keyboard
   * (if needed) overload btfe_event.on_mouse
   * call 'cycle' periodically (e.g. in a loop)
   * see group 'event' for more details

### Thread safety
* The ncurers interface is encapsulated.
* Multiple instances of btfe_console_s in different threads are allowed.

### Issues
While btfe_console_s is active the terminal's text handling via ```stdout``` or ```stderr``` is changed: In messages send to the terminal via ```stdout``` or ```stderr```, a newline is not treated as carriage return. This can lead to unexpected text-indentations.

#### Remedies (choose a suitable option)
* btfe_console_s is also a x_sink perspective with correct newline handling.
* Use member functions msg_fa, err_fa.
* Call function 'close' or destroy the instance (this resets the terminal). If needed, re-open it later.



###  Example

``` C
stamp example_s
{
    btfe_console_s console;
    bl_t exit;

    func btfe_console_event.on_keyboard
    {
        if( event.chr == 'q' ) o.exit = true;
        event.to_sink( o.console );
        = 0;
    }

    // displays action type at mouse position
    func btfe_console_event.on_mouse
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

```



<sub>&copy; 2025 Johannes B. Steffens</sub>
