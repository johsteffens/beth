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
include "bcore_std.h";
include "bmath_std.h";

/** PyObject interface
 *  Implements a PyObject wrapper and conversion functions between stamp and PyObject.
 *  This implementation makes no assumptions about the python ABI. Hence, conversion functions
 *  copy all data across the beth-bython border.
 *
 *  (!) Thread Safety:
 *  :obj_s const-functions are not concurrent unless the Python API is entirely concurrent.
 *  Use byth_frame-locks to be on the safe side.
 */

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func m PyObject* py_incref( m PyObject* o ) { Py_XINCREF( o ); = o; }
func m PyObject* py_decref( m PyObject* o ) { Py_XDECREF( o ); = o; }

/** obj_s wraps PyObject and provides functions for conversion between beth-objects and python-objects.
 *  py_object is either NULL or a strong python reference.
 */
stamp :s
{
    $ private PyObject* py_object;
    func bcore_inst_call.down_e o.clear();

    func o clear( m@* o ) { :py_decref( o.py_object ); o.py_object = NULL; }

    /// creates PyObject by conversion from beth-stamp
    func er_t setup_from_t_inst( m@* o, tp_t type, c obliv x_inst* inst );
    func er_t setup_from_inst  ( m@* o,            c aware x_inst* inst ) = inst ? o.setup_from_t_inst( inst._, inst ) : 0;

    func bl_t is_py_long   ( c@* o ) = o.py_object && PyLong_Check   ( o.py_object );
    func bl_t is_py_float  ( c@* o ) = o.py_object && PyFloat_Check  ( o.py_object );
    func bl_t is_py_list   ( c@* o ) = o.py_object && PyList_Check   ( o.py_object );
    func bl_t is_py_dict   ( c@* o ) = o.py_object && PyDict_Check   ( o.py_object );
    func bl_t is_py_unicode( c@* o ) = o.py_object && PyUnicode_Check( o.py_object );

    /// The following conversions return the underlying value if convertible or 0 otherwise
    func u3_t to_u3( c@* o );
    func s3_t to_s3( c@* o );
    func uz_t to_uz( c@* o );
    func sz_t to_sz( c@* o );
    func f3_t to_f3( c@* o );
    func bl_t to_bl( c@* o );
    func tp_t to_tp( c@* o ) = o.to_u3();

    /// checks if a dictionary contains key
    func bl_t py_dict_contains( c@* o, sc_t key );

    /// gets an element from dictionary (strongified reference)
    func m PyObject* py_dict_get( m@* o, sc_t key ) = :py_incref( PyDict_GetItemString( o.py_object, key ) );

    /// gets an element as obj_s from dictionary
    func d @* py_dict_get_obj( m@* o, sc_t key );

    /// gets an element from list (strongified reference)
    func m PyObject* py_list_get( m@* o, sz_t index ) = :py_incref( PyList_GetItem( o.py_object, index ) );

    /// gets an element as obj_s from list
    func d @* py_list_get_obj( m@* o, sz_t index );

    /// number of elements in list
    func sz_t py_list_size( @* o ) = PyList_Size( o.py_object );

    /// converts data to given instance
    func er_t to_t_inst( c@* o, tp_t type, m obliv x_inst* inst );
    func er_t to_inst  ( c@* o,            m aware x_inst* inst ) = o.to_t_inst( inst._, inst );

    /// creates a (strongly referenced) stamp from PyObject
    func er_t to_sr( c@* o, m sr_s* sr );

    func d st_s* to_string( @* o );
    func o       to_stdout( @* o );
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func (:s) to_u3
{
    if( o.is_py_long() ) = PyLong_AsUnsignedLongLong( o.py_object );
    if( o.is_py_float() ) = PyFloat_AsDouble ( o.py_object );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) to_s3
{
    if( o.is_py_long() ) = PyLong_AsLongLong( o.py_object );
    if( o.is_py_float() ) = PyFloat_AsDouble ( o.py_object );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) to_uz
{
    if( o.is_py_long() ) = PyLong_AsSize_t( o.py_object );
    if( o.is_py_float() ) = PyFloat_AsDouble ( o.py_object );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) to_sz
{
    if( o.is_py_long() ) = PyLong_AsSsize_t( o.py_object );
    if( o.is_py_float() ) = PyFloat_AsDouble ( o.py_object );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) to_f3
{
    if( o.is_py_float() ) = PyFloat_AsDouble ( o.py_object );
    if( o.is_py_long() ) = PyLong_AsLongLong( o.py_object );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) to_bl
{
    if( o.py_object == Py_True ) = true;
    if( o.py_object == Py_False ) = false;
    = ( o.to_f3() != 0 );
}

//----------------------------------------------------------------------------------------------------------------------

/// checks if a dictionary contains key
func (:s) py_dict_contains
{
    if( !o.is_py_dict() ) = false;
    = PyDict_GetItemString( o.py_object, key ) != NULL;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) py_dict_get_obj
{
    m @* obj = @!^( o.py_dict_get( key ) );
    if( !o.py_object )
    {
        GERR_fa( "Error retrieving Item from PyDict." );
        = NULL;
    }
    = obj.fork();
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) py_list_get_obj
{
    m @* obj = @!^( o.py_list_get( index ) );
    if( !o.py_object )
    {
        GERR_fa( "Error retrieving Item from PyList." );
        = NULL;
    }
    = obj.fork();
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) setup_from_t_inst
{
    if( o.py_object ) o.clear();
    if( !inst ) = 0;

    if( !x_inst_exists( type ) ) ERR_fa( "Type value does not represent an object." );

    m PyObject* py_object = Py_None;

    switch( type )
    {
        case u0_t~: py_object = PyLong_FromUnsignedLong    ( inst.cast( u0_t* ).0 ); break;
        case u1_t~: py_object = PyLong_FromUnsignedLong    ( inst.cast( u1_t* ).0 ); break;
        case u2_t~: py_object = PyLong_FromUnsignedLong    ( inst.cast( u2_t* ).0 ); break;
        case u3_t~: py_object = PyLong_FromUnsignedLongLong( inst.cast( u3_t* ).0 ); break;
        case uz_t~: py_object = PyLong_FromSize_t  ( inst.cast( uz_t* ).0 ); break;
        case tp_t~: py_object = PyLong_FromUnsignedLongLong( inst.cast( tp_t* ).0 ); break;
        case s0_t~: py_object = PyLong_FromLong    ( inst.cast( s0_t* ).0 ); break;
        case s1_t~: py_object = PyLong_FromLong    ( inst.cast( s1_t* ).0 ); break;
        case s2_t~: py_object = PyLong_FromLong    ( inst.cast( s2_t* ).0 ); break;
        case s3_t~: py_object = PyLong_FromLongLong( inst.cast( s3_t* ).0 ); break;
        case sz_t~: py_object = PyLong_FromSsize_t ( inst.cast( sz_t* ).0 ); break;
        case f2_t~: py_object = PyFloat_FromDouble ( inst.cast( f2_t* ).0 ); break;
        case f3_t~: py_object = PyFloat_FromDouble ( inst.cast( f3_t* ).0 ); break;
        case bl_t~: py_object = ( inst.cast( bl_t* ).0 ) ? Py_True : Py_False; break;
        case sc_t~: py_object = PyUnicode_FromString( ( sc_t )inst ); break;
        case st_s~: py_object = PyUnicode_FromString( inst.cast( st_s* ).sc ); break;
        default: break;
    }

    if( py_object != Py_None )
    {
        if( py_object == NULL ) = GERR_fa( "Converting '#<sc_t>' failed.", bnameof( type ) );
        o.py_object = py_object;
        = 0;
    }

    py_object = PyDict_New();

    {
        m PyObject* val = PyLong_FromUnsignedLongLong( type );
        s2_t py_ret = PyDict_SetItemString( py_object, "_", val );
        :py_decref( val );
        if( py_ret < 0 ) = GERR_fa( "PyDict_SetItem failed." );
    }

    x_stamp* stamp = inst;
    sz_t size = stamp.t_size( type );

    if( x_stamp_t_is_pure_array( type ) )
    {
        x_array* arr = inst;
        sz_t size = arr.t_size( type );
        m PyObject* key  = PyUnicode_FromString( "_pure_array_" );
        m PyObject* list = PyList_New( size );
        er_t err = 0;
        for( sz_t i = 0; i < size; i++ )
        {
            sr_s sr = arr.t_c_get_sr( type, i );
            m$* item = :s!^;
            item.setup_from_t_inst( sr.type(), sr.o );
            s2_t py_ret = PyList_SetItem( list, i, :py_incref( item.py_object ) );
            sr_down( sr );
            if( py_ret < 0 )
            {
                err = GERR_fa( "PyList_SetItem failed." );
                break;
            }
        }
        if( err ) = err;
        s2_t py_ret = PyDict_SetItem( py_object, key, list );
        :py_decref( key );
        :py_decref( list );
        if( py_ret < 0 ) GERR_fa( "PyDict_SetItem failed." );
    }
    else
    {
        er_t err = 0;
        for( sz_t i = 0; i < size; i++ )
        {
            sr_s sr = stamp.t_c_get_sr_i( type, i );
            tp_t name = stamp.t_name( type, i );

            m$* val = :s!^;
            val.setup_from_t_inst( sr.type(), sr.o );

            m PyObject* py_val = val.py_object;
            s2_t py_ret = PyDict_SetItemString( py_object, bnameof( name ), py_val );
            sr_down( sr );

            if( py_ret < 0 )
            {
                err = GERR_fa( "PyDict_SetItem failed." );
                break;
            }
        }
        if( err ) = err;
    }

    if( py_object != Py_None )
    {
        if( py_object == NULL )
        {
            = GERR_fa( "Converting '#<sc_t>' failed.", bnameof( type ) );
        }
        o.py_object = py_object;
        = 0;
    }

    = GERR_fa( "Converting '#<sc_t>' failed.", bnameof( type ) );
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) to_sr
{
    if( o.is_py_long() )
    {
        sr.from_s3( o.to_s3() );
    }
    else if( o.is_py_float() )
    {
        sr.from_f3( o.to_f3() );
    }
    else if( o.is_py_unicode() )
    {
        m st_s* st = st_s!^;
        o.to_inst( st );
        sr.asm( st.fork() );
    }
    else if( o.is_py_dict() )
    {
        m :s* obj = o.cast( m$* ).py_dict_get_obj( "_" )^;
        if( !obj ) = general_error~;
        tp_t type = obj.to_tp();
        sr.tsm( type, x_inst_create( type ) );
        o.to_t_inst( type, sr.o );
    }
    else if( o.is_py_list() )
    {
        m bcore_arr_sr_s* arr_sr = bcore_arr_sr_s!^;
        o.to_inst( arr_sr );
        sr.asm( arr_sr.fork() );
    }
    else
    {
        = GERR_fa( "Unhandled PyObject." );
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) to_t_inst
{
    if( !inst ) = 0;

    if( !x_inst_exists( type ) ) = GERR_fa( "Type value does not represent an object." );

    switch( type )
    {
        case u0_t~: inst.cast( m u0_t* ).0 = o.to_u3(); = 0;
        case u1_t~: inst.cast( m u1_t* ).0 = o.to_u3(); = 0;
        case u2_t~: inst.cast( m u2_t* ).0 = o.to_u3(); = 0;
        case u3_t~: inst.cast( m u3_t* ).0 = o.to_u3(); = 0;
        case uz_t~: inst.cast( m uz_t* ).0 = o.to_uz(); = 0;
        case tp_t~: inst.cast( m tp_t* ).0 = o.to_u3(); = 0;
        case s0_t~: inst.cast( m s0_t* ).0 = o.to_s3(); = 0;
        case s1_t~: inst.cast( m s1_t* ).0 = o.to_s3(); = 0;
        case s2_t~: inst.cast( m s2_t* ).0 = o.to_s3(); = 0;
        case s3_t~: inst.cast( m s3_t* ).0 = o.to_s3(); = 0;
        case sz_t~: inst.cast( m sz_t* ).0 = o.to_sz(); = 0;
        case f2_t~: inst.cast( m f2_t* ).0 = o.to_f3(); = 0;
        case f3_t~: inst.cast( m f3_t* ).0 = o.to_f3(); = 0;
        case bl_t~: inst.cast( m bl_t* ).0 = o.to_bl(); = 0;
        case sc_t~: = GERR_fa( "Cannot convert to sc_t." );
        case st_s~:
        {
            if( !o.is_py_unicode() ) = GERR_fa( "Attempt to convert to st_s from non unicode PyObject" );
            m st_s* st = inst.cast( m st_s* );
            m PyObject* py_unicode = PyUnicode_AsEncodedString( o.py_object, "utf-8", "strict" ); // create UTF8 encoding
            if( !py_unicode ) = GERR_fa( "PyUnicode_AsEncodedString error" );
            st.copy_sc( PyBytes_AsString( py_unicode ) );
            :py_decref( py_unicode );
            = 0;
        }

        default: break;
    }

    if( o.is_py_dict() )
    {
        if( x_stamp_t_is_pure_array( type ) && o.py_dict_contains( "_pure_array_" ) )
        {
            m x_array* arr = inst;
            arr.t_set_size( type, 0 );

            m@* obj_list = o.cast( m$* ).py_dict_get_obj( "_pure_array_" )^;
            if( !obj_list ) = general_error~;
            if( !obj_list.is_py_list() ) = GERR_fa( "Error retrieving PyList from PyDict." );
            sz_t size = obj_list.py_list_size();

            for( sz_t i = 0; i < size; i++ )
            {
                m PyObject* py_item = obj_list.py_list_get( i );
                if( !py_item ) = GERR_fa( "Error retrieving Item from PyList." );
                sr_s sr = sr_null();
                :s!^( py_item ).to_sr( sr );
                arr.t_push_sr( type, sr );
            }
        }
        else
        {
            m x_stamp* stamp = inst;
            sz_t size = stamp.t_size( type );

            for( sz_t i = 0; i < size; i++ )
            {
                tp_t name = stamp.t_name( type, i );
                if( o.py_dict_contains( bnameof( name ) ) )
                {
                    m :s* item = o.cast( m$* ).py_dict_get_obj( bnameof( name ) )^;
                    if( !item ) = general_error~;
                    if( stamp.t_is_link_i( type, i ) )
                    {
                        sr_s sr = sr_null();
                        item.to_sr( sr );
                        stamp.t_set_sr_i( type, i, sr );
                    }
                    else
                    {
                        sr_s sr = stamp.t_c_get_sr_i( type, i );
                        if( sr.is_mutable() && sr.is_weak() )
                        {
                            item.to_t_inst( sr.type(), sr.o );
                        }
                        else
                        {
                            sr.clear();
                            item.to_sr( sr );
                            stamp.t_set_sr_i( type, i, sr );
                        }
                    }
                }
            }
        }
    }
    else if( o.is_py_list() )
    {
        if( x_stamp_t_is_array( type ) )
        {
            m x_array* arr = inst;
            arr.t_set_size( type, 0 );
            sz_t size = o.py_list_size();
            for( sz_t i = 0; i < size; i++ )
            {
                m :s* item = o.cast( m$* ).py_list_get_obj( i )^;
                if( !item ) = general_error~;
                sr_s sr = sr_null();
                item.to_sr( sr );
                arr.t_push_sr( type, sr );
            }
        }
    }
    else
    {
        = GERR_fa( "Unhandled PyObject" );
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) to_string
{
    if( !o.py_object ) = NULL;

    m PyObject* repr_object = PyObject_Repr( ( PyObject* )o.py_object ); // turns object to a string represenation
    m PyObject* py_unicode = PyUnicode_AsEncodedString( repr_object, "utf-8", "strict" ); // create UTF8 encoding
    sc_t cstr_object = PyBytes_AsString( py_unicode );  // get cstring
    m st_s* st = st_s!^.copy_sc( cstr_object );
    :py_decref( repr_object );
    :py_decref( py_unicode );
    = st.fork();
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) to_stdout
{
    bcore_msg_fa( "#<sc_t>\n", o.to_string()^.sc );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

