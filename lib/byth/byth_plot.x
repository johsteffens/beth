/** Author and Copyright 2024 Johannes Bernhard Steffens
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

/// plot data
stamp :data_s
{
     $ st_s => label;
     $ bcore_arr_f3_s => arr;
     func o set_label( m@* o, sc_t label ) o.label!.copy_sc( label );
     func o push( m@* o, f3_t v ) o.arr!.push( v );
}

//----------------------------------------------------------------------------------------------------------------------

/// limit range
stamp :limit_s
{
     $ f3_t min = 0;
     $ f3_t max = 1E38;
}

//----------------------------------------------------------------------------------------------------------------------

/// plot axis
stamp :axis_s
{
     st_s => label; // typically unit specification
     :limit_s => limit; // limits the display range

     bl_t log;   // logarithmic vs. linear scale
     func o set_label( m@* o, sc_t label ) o.label =< st_s!.copy_sc( label );
     func o set_log( m@* o, bl_t flag ) o.log = flag;
     func o set_limit( m@* o, f3_t min, f3_t max ) o.limit =< :limit_s!( min, max );
}

//----------------------------------------------------------------------------------------------------------------------

/// text at specified coordinates
stamp :textpoint_s
{
    $ f3_t x;
    $ f3_t y;
    $ st_s text;
}

//----------------------------------------------------------------------------------------------------------------------

/// array of textpoints
stamp :textpoint_arr_s x_array
{
    :textpoint_s => [];

    func o push_textpoint_st( m@* o, f3_t x, f3_t y, st_s* st ) o.push_d( :textpoint_s!( x, y, st ) );
    func o push_textpoint_sc( m@* o, f3_t x, f3_t y, sc_t  sc ) o.push_d( :textpoint_s!( x, y, st_s!^.copy_sc( sc ) ) );
}

//----------------------------------------------------------------------------------------------------------------------

/// plot appearance parameters
stamp :appearance_s
{
    st_s preferred_style = "ggplot"; // run 'print( matplotlib.pyplot.style.available )' to get all available styles
    st_s => window_title; // title on window frame

    f3_t line_width   = 0.7;    // line thickness

    // plot title
    f3_t title_size   =  10;
    st_s title_weight = "bold";

    // title of frame_arr
    f3_t arr_title_size   =  12;
    st_s arr_title_weight = "bold";

    // label on axes
    f3_t label_size   = 8;
    st_s label_weight = "bold";

    // axis (tick) values labeling
    f3_t xtick_label_size = 8;
    f3_t ytick_label_size = 8;

    f3_t legend_font_size = 8;

    // textpoints
    f3_t textpoint_font_size   = 8;
    st_s textpoint_font_weight = "normal";

    /// Only for multiple plots...
    sz_t cols = 2;          //  ordering of plots (number of columns); rows = ceil( size / cols )
    f3_t ver_spacing = 0.5; // vertical spacing (spacing between plot rows)
    f3_t hor_spacing = 0.3; // horizontal spacing (spacing between plot columns)
    /// --------------------------

};

//----------------------------------------------------------------------------------------------------------------------

/// Single plot. Multiple curves.
stamp :frame_s x_array
{
    st_s => title; // plot title

    :axis_s => x_axis;
    :axis_s => y_axis;
    bcore_arr_f3_s => x_arr; // optional x-values
    :data_s => []; // array of y data (multiple curves)

    /// optional text points at specified coordinates (coordinate system of plot)
    :textpoint_arr_s => textpoint_arr;

    func o push_textpoint_st( m@* o, f3_t x, f3_t y, st_s* st ) o.textpoint_arr!.push_textpoint_st( x, y, st );
    func o push_textpoint_sc( m@* o, f3_t x, f3_t y, sc_t  sc ) o.textpoint_arr!.push_textpoint_sc( x, y, sc );

    // pushes x value
    func o push_x( m@* o, f3_t v ) o.x_arr!.push( v );

    // shows plot in a dedicated window; appearance can be NULL
    func er_t show( @* o, const :appearance_s* appearance );
}

//----------------------------------------------------------------------------------------------------------------------

/// Multiple plots grouped together.
stamp :frame_arr_s x_array
{
    st_s => title; // global title for given plot-set

    :frame_s => []; // array of frames

    /// optional text points at specified coordinates (coordinate system of window)
    :textpoint_arr_s => textpoint_arr;

    func o push_textpoint_st( m@* o, f3_t x, f3_t y, st_s* st ) o.textpoint_arr!.push_textpoint_st( x, y, st );
    func o push_textpoint_sc( m@* o, f3_t x, f3_t y, sc_t  sc ) o.textpoint_arr!.push_textpoint_sc( x, y, sc );

    // shows plots in a dedicated window; appearance can be NULL
    func er_t show( @* o, const :appearance_s* appearance );
}

//----------------------------------------------------------------------------------------------------------------------

/// runtime environment for plots
stamp :engine_s x_thread
{
    bl_t loop_started_flag;
    bl_t exit_loop_flag;
    x_mutex_s mutex;
    func void lock  ( @* o ) o.cast( m$* ).mutex.lock();
    func void unlock( @* o ) o.cast( m$* ).mutex.unlock();

    func bl_t exit_loop( @* o )
    {
        o.lock();
        bl_t flag = o.exit_loop_flag;
        o.unlock();
        = flag;
    }

    func bl_t loop_started( @* o )
    {
        o.lock();
        bl_t flag = o.loop_started_flag;
        o.unlock();
        = flag;
    }

    func void set_exit_loop_flag( m@* o, bl_t flag )
    {
        o.lock();
        o.exit_loop_flag = flag;
        o.unlock();
    }

    func void set_loop_started_flag( m@* o, bl_t flag )
    {
        o.lock();
        o.loop_started_flag = flag;
        o.unlock();
    }

    func x_thread_result* main( m@* o )
    {
        while( !o.exit_loop() )
        {
            x_threads_sleep_ms( 100 );
        }
        o.set_loop_started_flag( m@* o, false );
        = NULL;
    }



}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

