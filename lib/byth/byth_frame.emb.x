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

/** Runtime frame for embedded python code.
 *  The frame gets instantiated during the init1 cycle.
 *  Only one instance of byth_frame_s can exist.
 *  It can be obtained and used via function byth_frame_get()
 *  There are also directly routed convenience functions.
 */

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func (:s) initialize
{
    if( o.is_initialized ) = o;
    o.lock();
    if( o.is_initialized ) = o;
    Py_Initialize();
    o.is_initialized = true;
    o.unlock();
    = o;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) finalize
{
    if( !o.is_initialized ) = o;
    o.lock();
    if( !o.is_initialized ) = o;
    //Py_FinalizeEx();
    Py_Exit( 0 ); // Exit wraps FinalizeEx; Not clear what else it is doing (additional cleanup maybe?)
    o.is_initialized = false;
    o.unlock();
    = o;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) init_x
{
    if( bcore_global_exists( byth_frame_global_key~ ) )
    {
        GERR_fa
        (
            "Attempt to instantiate byth_frame_s.\n"
            "byth_frame_s is only instantiated once during init1 cycle and stored in bcore_global_manager.\n"
            "Use function byth_frame_s* byth_frame_get() to obtain a reference.\n"
        );
    }
}


//----------------------------------------------------------------------------------------------------------------------

func (:s) down_e
{
    o.finalize();
}

//----------------------------------------------------------------------------------------------------------------------

/// init1 cycle callback
func void group_signal_init1()
{
    bcore_global_a_set_d( byth_frame_global_key~, :s! );
}

//----------------------------------------------------------------------------------------------------------------------

/// down1 cycle callback
func void group_signal_down1()
{
    bcore_global_remove( byth_frame_global_key~ );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func (:s) run
{
    o.create_lock()^;

    m PyObject* globals = PyDict_New();
    m PyObject* locals  = PyDict_New();

    if( byth_in )
    {
        m$* byth_in_obj = byth_obj_s!^;
        byth_in_obj.setup_from_inst( byth_in );
        s2_t py_ret = PyDict_SetItemString( locals, "byth_in", byth_in_obj.py_object );
        if( py_ret < 0 )
        {
            byth_py_check_error();
            GERR_fa( "PyDict_SetItem failed." );
        }
    }

    PyObject* result = PyRun_String( program, Py_file_input, globals, locals );

    if( byth_out )
    {
        m PyObject* py_byth_out = byth_obj_py_dict_get_item_string( locals, "byth_out" );
        if( py_byth_out )
        {
            Py_XINCREF( py_byth_out );
            m$* byth_out_obj = byth_obj_s!^;
            byth_out_obj.py_object = py_byth_out;
            byth_out_obj.to_inst( byth_out );
        }
    }

    Py_XDECREF( globals );
    Py_XDECREF( locals );

    if( !result )
    {
        byth_py_check_error();
        GERR_fa( "PyRun_String returned NULL." );
    }
    Py_XDECREF( result ); // XDECREF ignores NULL argument
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) run_hmap
{
    o.create_lock()^;

    m PyObject* globals = PyDict_New();
    m PyObject* locals  = NULL;

    if( direct_in )
    {
        m$* byth_in_obj = byth_obj_s!^^;
        byth_in_obj.setup_from_hmap( direct_in );
        locals = byth_py_incref( byth_in_obj.py_object );
    }
    else
    {
        locals  = PyDict_New();
    }

//    m PyObject* py_code = Py_CompileString( program, "" /*filename for debugging*/, Py_file_input );
//    PyObject* result  = PyEval_EvalCode( py_code, globals, locals );
//    Py_XDECREF( py_code );

    PyObject* result = PyRun_String( program, Py_file_input, globals, locals );

    if( byth_out )
    {
        byth_out.clear();
        m PyObject* py_byth_out = byth_obj_py_dict_get_item_string( locals, "byth_out" );
        if( py_byth_out )
        {
            Py_XINCREF( py_byth_out );
            m$* byth_out_obj = byth_obj_s!^;
            byth_out_obj.py_object = py_byth_out;
            byth_out_obj.to_hmap( byth_out );
        }
    }

    Py_XDECREF( globals );
    Py_XDECREF( locals );

    if( !result )
    {
        byth_py_check_error();
        GERR_fa( "PyRun_String returned NULL." );
    }
    Py_XDECREF( result ); // XDECREF ignores NULL argument
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

