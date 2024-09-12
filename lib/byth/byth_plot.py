# Author and Copyright 2023 Johannes Bernhard Steffens

import matplotlib.pyplot as plt
import numpy as np
import math as math
import sys
import os

args = sys.argv
data_file = 'plot.data'
window_title = 'Synt Plot'

if( len( args ) > 1  ): data_file = args[ 1 ]
if( len( args ) > 2  ): window_title = args[ 2 ]

data = np.loadtxt( data_file )

x, y = data.T;

fig = plt.figure()
fig.canvas.manager.set_window_title( window_title )
ax1 = fig.add_subplot( 1, 1, 1 ) # specifies where the subplot is displayed

ax1.set_xscale( 'linear' )
ax1.set_yscale( 'linear' )
ax1.set_xlabel( 'sample number' )
ax1.set_ylabel( 'value' )

# plot the function
ax1.plot( x, y, 'red' )

plt.title( 'Data Plot' )

# show the plot
ax1.grid()
plt.show()

if os.path.exists( data_file ): os.remove( data_file );
