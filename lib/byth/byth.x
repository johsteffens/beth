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

include "byth_base.h";

/** Utility functions
 */

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func o py_incref( m PyObject* o ) { Py_XINCREF( o ); = o; }
func o py_decref( m PyObject* o ) { Py_XDECREF( o ); = o; }

func bl_t py_check_error()
{
    if( PyErr_Occurred() )
    {
        PyErr_Print();
        PyErr_Clear();
        = true;
    }
    = false;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

