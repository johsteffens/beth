# Author J.B. Steffens

import matplotlib.pyplot

def on_window_close( event ):
    global keep_looping
    keep_looping = 0

def setup_plot( plt, fig ):
    global on_window_close
    plt.ion() # interactive mode
    fig.canvas.mpl_connect( 'close_event', on_window_close )

def update_appearance( plt, fig, appearance ):
    if appearance[ 'preferred_style' ] in plt.style.available:
        plt.style.use( appearance[ 'preferred_style' ] )

    # plot font default parameters...

    # general text
    plt.rc( 'font',   size        = appearance[ 'textpoint_font_size' ] )
    plt.rc( 'font',   weight      = appearance[ 'textpoint_font_weight' ] )

    plt.rc( 'axes',   titlesize   = appearance[ 'title_size' ] )
    plt.rc( 'axes',   titleweight = appearance[ 'title_weight' ] )

    # x and y labels
    plt.rc( 'axes',   labelsize   = appearance[ 'label_size' ] )
    plt.rc( 'axes',   labelweight = appearance[ 'label_weight' ] )

    # x and y (tick-) values
    plt.rc( 'xtick',  labelsize  =  appearance[ 'xtick_label_size' ] )
    plt.rc( 'ytick',  labelsize  =  appearance[ 'ytick_label_size' ] )

    plt.rc( 'legend', fontsize   =  appearance[ 'legend_font_size' ] )

    plt.rc( 'figure', titlesize    = appearance[ 'arr_title_size' ] )
    plt.rc( 'figure', titleweight  = appearance[ 'arr_title_weight' ] )

    if appearance['window_title'] is not None:
        fig.canvas.manager.set_window_title( appearance['window_title'] )

    fig.subplots_adjust( hspace = appearance['ver_spacing'], wspace = appearance['hor_spacing'] )

def update_plot( plt, fig, frame_arr, appearance ):
    import math

    if frame_arr['title'] is not None:
        fig.suptitle( frame_arr['title'] )

    linewidth = appearance['line_width']
    plotcols = min( len( frame_arr[ '' ][ '_list_' ] ), appearance['cols'] )

    fig.clear()

    plot_count = len( frame_arr[ '' ][ '_list_' ] )

    for i in range( plot_count ):
        frame = frame_arr[ '' ][ '_list_' ][ i ]

        ax = fig.add_subplot( math.ceil( plot_count / plotcols ), plotcols, i + 1 )

        if frame['textpoint_arr'] is not None:
            for textpoint in frame['textpoint_arr'][ '_list_' ]:
                ax.text( textpoint[ 'x' ], textpoint[ 'y' ], textpoint[ 'text' ] )

        if frame['title'] is not None:
            ax.set_title( frame['title'] )

        if frame['x_axis'] is not None:
            if frame['x_axis'][ 'label' ] is not None: ax.set_xlabel( frame['x_axis'][ 'label' ] )

            if frame['x_axis'][ 'log' ]:
                ax.set_xscale( 'log' )
            else:
                ax.set_xscale( 'linear' )
            if frame['x_axis'][ 'limit' ] is not None:
                ax.set_xlim( frame['x_axis'][ 'limit' ][ 'min' ], frame['x_axis'][ 'limit' ][ 'max' ] )

        if frame['y_axis'] is not None:
            if frame['y_axis'][ 'label' ] is not None: ax.set_ylabel( frame['y_axis'][ 'label' ] )
            if frame['y_axis'][ 'log' ]:
                ax.set_yscale( 'log' )
            else:
                ax.set_yscale( 'linear' )
            if frame['y_axis'][ 'limit' ] is not None:
                ax.set_ylim( frame['y_axis'][ 'limit' ][ 'min' ], frame['y_axis'][ 'limit' ][ 'max' ] )

        show_legend = False

        for y_data in frame[''][ '_list_' ]:
            if( y_data[ 'label' ] is not None ): show_legend = True

        if( frame['x_arr'] is not None ):
            for y_data in frame[''][ '_list_' ]:
                ax.plot( frame['x_arr'][ '_list_' ], y_data[ 'arr' ][ '_list_' ], linestyle = '-', marker = '', label = y_data[ 'label' ], linewidth = linewidth )
        else:
            for y_data in frame[''][ '_list_' ]:
                ax.plot( y_data[ 'arr' ][ '_list_' ], linestyle = '-', marker = '', label = y_data[ 'label' ], linewidth = linewidth )

        if( show_legend ): ax.legend() # adds legend with specified plot labels above


