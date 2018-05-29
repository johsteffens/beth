/** Copyright 2018 Johannes Bernhard Steffens
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

/** Framework for plotting charts */

#ifndef BMATH_PLOT_H
#define BMATH_PLOT_H

#include "bcore_std.h"
#include "bmath_vector.h"

/**********************************************************************************************************************/
// bmath_plot_s

BCORE_DECLARE_OBJECT_( bmath_plot_s )
{
    aware_t _;
    sr_s data; // plot data  (vf3, arr_vf3)

    st_s title;
    st_s x_label;
    st_s y_label;
    bcore_arr_st_s y_data_label;

    bl_t x_log; // log vs. linear
    bl_t y_log; // log vs. linear
};

/** Runs gnuplot via system command with data (gnuplot must be installed).
 *  Opens a window in persistent mode (requires manual closing).
 *  Creates/overwrites plot data in data_folder.
 *  Returns return value of system call.
 */
s2_t bmath_plot_s_run_gnuplot( const bmath_plot_s* o, sc_t data_folder );

/**********************************************************************************************************************/

vd_t bmath_plot_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_PLOT_H

