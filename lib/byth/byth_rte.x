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


//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/** Runtime environment to run python programs or python code fragments.
 *  Provides an interface to interact with python data.
 *  Use one instance of byth_rte_core_s per running python program.
 *  Calls Py_Initialize() on setup and Py_Finalize on destruction.
 */
name byth_rte_s_global_key;
stamp :s
{
    bl_t is_setup;

    byth_obj_s -> globals;
    byth_obj_s -> locals;
    byth_obj_s -> result; // result of last run

    func bcore_inst_call.init_x o.setup();
    func bcore_inst_call.down_e o.shut_down();

    func o shut_down( m@* o )
    {
        if( !o.is_setup ) = o;
        o->globals =< NULL;
        o->locals  =< NULL;
        o->result  =< NULL;
        // Calling FinalizeEx does not seem to allo stable re-initialization.
        // Since Py_Initialize() is reentrant, we can also keep python initialized until the program closes
        //if( Py_FinalizeEx() ) WRN_fa( "Py_FinalizeEx() failed.\n" );
        bcore_global_remove( byth_rte_s_global_key~ );
        o.is_setup = false;
        = o;
    }

    func o setup( m@* o )
    {
        if( o.is_setup ) = o;

        if( bcore_global_exists( byth_rte_s_global_key~ ) )
        {
            ERR_fa
            (
                "Attempt to instantiate byth_rte_core_s more than once.\n"
                "Only one instance of byth_rte_core_s should exist at a given time.\n"
            );
        }

        bcore_global_t_set_d( byth_rte_s_global_key~, 0, NULL );
        Py_Initialize();

        o.globals =< byth_obj_s!.setup_as_dict();
        o.locals  =< byth_obj_s!.setup_as_dict();
        o.result  =< byth_obj_s!;
        o.is_setup = true;
        = o;
    }

    func o set_local_t_inst( m@* o, sc_t name, tp_t type, obliv x_inst* inst )
    {
        if( !inst ) = o;
        m$* obj_val = byth_obj_s!^;
        obj_val.setup_from_t_inst( type, inst );
        o.locals.py_dict_set( name, obj_val.py_object );
        = o;
    }

    func o set_local_inst( m@* o, sc_t name, aware x_inst* inst )
    {
        if( !inst ) = o;
        = o.set_local_t_inst( name, inst._, inst );
    }

    func o set_global_t_inst( m@* o, sc_t name, tp_t type, obliv x_inst* inst )
    {
        if( !inst ) = o;
        m$* obj_val = byth_obj_s!^;
        obj_val.setup_from_t_inst( type, inst );
        o.globals.py_dict_set( name, obj_val.py_object );
        = o;
    }

    func o set_global_inst( m@* o, sc_t name, aware x_inst* inst )
    {
        if( !inst ) = o;
        = o.set_global_t_inst( name, inst._, inst );
    }

    func er_t get_local_t_inst( @* o, sc_t name, tp_t type, m obliv x_inst* inst )
    {
        if( !inst ) = 0;
        m byth_obj_s* obj = o.locals.py_dict_get_obj( name )^;
        obj.to_t_inst( type, inst );
        = 0;
    }

    func er_t get_global_t_inst( @* o, sc_t name, tp_t type, m obliv x_inst* inst )
    {
        if( !inst ) = 0;
        m byth_obj_s* obj = o.globals.py_dict_get_obj( name )^;
        obj.to_t_inst( type, inst );
        = 0;
    }

    func o set_local_u3( m@* o, sc_t name, u3_t v ) = o.set_local_t_inst( name, TYPEOF_u3_t, v.1 );
    func o set_local_s3( m@* o, sc_t name, s3_t v ) = o.set_local_t_inst( name, TYPEOF_s3_t, v.1 );
    func o set_local_uz( m@* o, sc_t name, uz_t v ) = o.set_local_t_inst( name, TYPEOF_uz_t, v.1 );
    func o set_local_sz( m@* o, sc_t name, sz_t v ) = o.set_local_t_inst( name, TYPEOF_sz_t, v.1 );
    func o set_local_f3( m@* o, sc_t name, f3_t v ) = o.set_local_t_inst( name, TYPEOF_f3_t, v.1 );
    func o set_local_bl( m@* o, sc_t name, bl_t v ) = o.set_local_t_inst( name, TYPEOF_bl_t, v.1 );
    func o set_local_tp( m@* o, sc_t name, tp_t v ) = o.set_local_t_inst( name, TYPEOF_tp_t, v.1 );

    func o set_global_u3( m@* o, sc_t name, u3_t v ) = o.set_global_t_inst( name, TYPEOF_u3_t, v.1 );
    func o set_global_s3( m@* o, sc_t name, s3_t v ) = o.set_global_t_inst( name, TYPEOF_s3_t, v.1 );
    func o set_global_uz( m@* o, sc_t name, uz_t v ) = o.set_global_t_inst( name, TYPEOF_uz_t, v.1 );
    func o set_global_sz( m@* o, sc_t name, sz_t v ) = o.set_global_t_inst( name, TYPEOF_sz_t, v.1 );
    func o set_global_f3( m@* o, sc_t name, f3_t v ) = o.set_global_t_inst( name, TYPEOF_f3_t, v.1 );
    func o set_global_bl( m@* o, sc_t name, bl_t v ) = o.set_global_t_inst( name, TYPEOF_bl_t, v.1 );
    func o set_global_tp( m@* o, sc_t name, tp_t v ) = o.set_global_t_inst( name, TYPEOF_tp_t, v.1 );

    func u3_t get_local_u3( @* o, sc_t name ) { u3_t v = 0; o.get_local_t_inst( name, TYPEOF_u3_t, v.1 ); = v; }
    func s3_t get_local_s3( @* o, sc_t name ) { s3_t v = 0; o.get_local_t_inst( name, TYPEOF_s3_t, v.1 ); = v; }
    func uz_t get_local_uz( @* o, sc_t name ) { uz_t v = 0; o.get_local_t_inst( name, TYPEOF_uz_t, v.1 ); = v; }
    func sz_t get_local_sz( @* o, sc_t name ) { sz_t v = 0; o.get_local_t_inst( name, TYPEOF_sz_t, v.1 ); = v; }
    func f3_t get_local_f3( @* o, sc_t name ) { f3_t v = 0; o.get_local_t_inst( name, TYPEOF_f3_t, v.1 ); = v; }
    func bl_t get_local_bl( @* o, sc_t name ) { bl_t v = 0; o.get_local_t_inst( name, TYPEOF_bl_t, v.1 ); = v; }
    func tp_t get_local_tp( @* o, sc_t name ) { tp_t v = 0; o.get_local_t_inst( name, TYPEOF_tp_t, v.1 ); = v; }

    func u3_t get_global_u3( @* o, sc_t name ) { u3_t v = 0; o.get_global_t_inst( name, TYPEOF_u3_t, v.1 ); = v; }
    func s3_t get_global_s3( @* o, sc_t name ) { s3_t v = 0; o.get_global_t_inst( name, TYPEOF_s3_t, v.1 ); = v; }
    func uz_t get_global_uz( @* o, sc_t name ) { uz_t v = 0; o.get_global_t_inst( name, TYPEOF_uz_t, v.1 ); = v; }
    func sz_t get_global_sz( @* o, sc_t name ) { sz_t v = 0; o.get_global_t_inst( name, TYPEOF_sz_t, v.1 ); = v; }
    func f3_t get_global_f3( @* o, sc_t name ) { f3_t v = 0; o.get_global_t_inst( name, TYPEOF_f3_t, v.1 ); = v; }
    func bl_t get_global_bl( @* o, sc_t name ) { bl_t v = 0; o.get_global_t_inst( name, TYPEOF_bl_t, v.1 ); = v; }
    func tp_t get_global_tp( @* o, sc_t name ) { tp_t v = 0; o.get_global_t_inst( name, TYPEOF_tp_t, v.1 ); = v; }

    /// run s a program (fragment) (sequence of statements)
    func o run_program( m@* o, sc_t string )
    {
        o.result.clear();

        o.result.py_object = PyRun_String( string, Py_file_input, o.globals.py_object, o.locals.py_object );

        if( !o.result.py_object )
        {
            byth_py_check_error();
            EM_ERR_fa( "PyRun_String returned NULL." );
        }

        = o;
    }

    /// single statement
    func o run_statement( m@* o, sc_t string )
    {
        o.result.clear();

        o.result.py_object = PyRun_String( string, Py_single_input, o.globals.py_object, o.locals.py_object );

        if( !o.result.py_object )
        {
            byth_py_check_error();
            EM_ERR_fa( "PyRun_String returned NULL." );
        }

        = o;
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
