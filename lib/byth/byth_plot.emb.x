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

func (:frame_s) show
{
    m$* frame_arr = :frame_arr_s!^;
    frame_arr.push_d( o.cast( m$* ).fork() );
    = frame_arr.show( appearance );
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_arr_s) show
{
    m$* run = byth_run_s!^;
    if( !appearance ) appearance = :appearance_s!^^;

    run.set_local_inst( "frame_arr", o );
    run.set_local_inst( "appearance", appearance );
    run.set_global_u3( "keep_looping", 1 );
    run.run_sequence( byth_plot_frame_arr_py_as_string()^.sc );

    // variable keep_looping is set 0 when the window is closed
    while( run.get_global_u3( "keep_looping" ) ) run.run_statement( "plt.pause(0.1)" );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

