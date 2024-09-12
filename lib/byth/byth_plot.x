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
stamp :data_s x_array
{
     st_s label;
     f3_t [];
}

//----------------------------------------------------------------------------------------------------------------------

/// plot axis
stamp :axis_s
{
     $ st_s label; // typically unit specification
     $ bl_t log;   // logarithmic vs. linear
}

//----------------------------------------------------------------------------------------------------------------------

stamp :frame_s x_array
{
    :axis_s => x_axis;
    :axis_s => y_axis;
    :data_s => x_data; // optional x-data
    :data_s => []; // array of y data (multiple curves)
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
