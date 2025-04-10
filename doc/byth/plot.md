# Byth Plot Interface 

[Byth](../../lib/byth/README.md) is a beth-python interface using the &#x1F517;[Python/C API](https://docs.python.org/3/c-api/index.html) .

## Plotting

The plotting interface wraps python's &#x1F517;[matplotlib](https://matplotlib.org/).

### Simple Plot

**Principle approach:**

- Instantiate `byth_plot_frame_s`, as defined in [byth_plot.x](byth_plot.x).
- Set parameters as needed. 
  - These are usually self explanatory. Referenced member objects are optional. Consider using convenience functions where suiatble.
  - A curve is defined by `byth_plot_data_s`.
  - `byth_plot_frame_s` is an array of `byth_plot_data_s`, each representing a separate curve.
  - Optional explicit x values can be supplied via member `x_arr`. If left undefined, the x axis represents the index.
- Call `(byth_plot_frame_s) show()`
  - Use argument NULL for default appearance.
  - Use argument `byth_plot_appearance_s` for custom appearance.
  - Ths function show a plot request to the [plot runtime manager](#plot-runtime-manager) and returns.

**Example for a plot with two curves:**

``` C
m$* frame = byth_plot_frame_s!^;

frame.title!.push_fa( "My Plot Title" );
frame.x_axis!.label!.push_fa( "x" );
frame.y_axis!.label!.push_fa( "f(x)" );
frame.push().label!.push_fa( "sin(x)" );       // curve 0
frame.push().label!.push_fa( "cos(x)*0.5" );   // curve 1

frame.push_textpoint_sc( 0, 0, "Textpoint:\nAt (0,0)" );

sz_t samples = 1000;
f3_t periods = 2;

for( sz_t i = 0; i < samples; i++ )
{
    f3_t x = ( periods * 2.0 * f3_pi() * i ) / samples;
    frame.push_x( x );
    frame.[0].push( f3_sin( x )       ); // curve 0
    frame.[1].push( f3_cos( x ) * 0.5 ); // curve 1
}

// Displaying plot in dedicated window.
// Use argument byth_plot_appearance_s for custom appearance or NULL for default appearance
// Repeated calls to 'show' would update the plot window accordingly.
frame.show( NULL ); 
```

**Result:**

![](../../data/byth/doc/plot_example1.png)

### Multiple Plots in one Window

**Principle approach:**

- Instantiate `byth_plot_frame_arr_s`, as defined in [byth_plot.x](../../lib/byth/byth_plot.x).
- Setup individual plots `byth_plot_frame_s` as array elements.
- Note: byth_plot_apperance_s holds parameters on how to arrange multiple plots in the plot window.
- Call `(byth_plot_frame_arr_s) show()`
  - Use argument NULL for default appearance.
  - Use argument `byth_plot_appearance_s` for custom appearance.

## Plot Runtime Manager
The byth plot runtime manager works in the background. 

When a plot request is received, the manager updates a graphical plot window in which all plots are drawn. The plot-window is newly opened upon the first plot request. The window stays open until closed manually or the program ends. When the window it was closed manually, it reopens on new data provided a proper appearance parameter is set.

# BTCL Interface
The byth plot framework offers a BTCL interface to allow plotting BTCL functions:

* Instantiate ```x_byth_plot_btcl_function_s```; set parameters as desired.
* Call function ```<x_byth_plot_btcl_function_s/>.plot( ... )```.
* Possible argument types:
  * ```x_btcl_function_s```, ```x_btcl_functor_s```,  ```x_btcl_functor_f3_s```: Plots that function.
  * \[ \<function\>, \<function\>, ... \]: Plots multiple functions.
  * \[ [\<text\>, \<function\>] ,[\<text\>, \<function\>], ... \]: Plots multiple functions with labels

**Example (BTCL):**

```C
plotter = <byth_plot_btcl_function_s/>
(
    .wait_for_enter_key = true, // halts execution after plotting until enter is pressed
    .title = "Sigmoid Functions: x/(a+ABS(x))",
    .x1 = -10,
    .x2 =  10,
    .samples = 1000,
);

plotter.plot( [ 0.25, 0.5, 1, 2, 4 ] :: func( a ) { ( "a="+a ) : func( a, x ){ x/(a+ABS(x)) }( a ) } );
```

**Result:**

![](../../data/byth/doc/plot_example2.png)


------
<sub>&copy; 2024 Johannes B. Steffens</sub>

