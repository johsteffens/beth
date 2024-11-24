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

include "byth_base.h";
include "bcore_std.h";
include "bmath_std.h";

/// PyObject interface

/**********************************************************************************************************************/
/** XOILA Registration and forwarding of Python/C API functions and objects.
 *  Purpose: Make XOICO aware of the Python/C API interface and alleviate xoico syntax checks where appropriate.
 */

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// Objects

//----------------------------------------------------------------------------------------------------------------------

/// object
type       PyObject;   // python object
type       Py_ssize_t;
identifier Py_DECREF;  // decrements reference counter (discards object)
identifier Py_INCREF;  // increments reference counter
identifier Py_XDECREF; // decrements reference counter (discards object)
identifier Py_XINCREF; // increments reference counter
identifier PyObject_Repr; // turns object into a string representation

//----------------------------------------------------------------------------------------------------------------------

/// type
type       PyTypeObject;  // object holding the type of an object
identifier PyType_Type;   // type of PyTypeObject
identifier PyType_Check;  // checks if object is of type PyTypeObject

//----------------------------------------------------------------------------------------------------------------------

/// none singleton
identifier Py_None;       // immortal PyObject* representing none  (check for none using '==' in C)

//----------------------------------------------------------------------------------------------------------------------

/// integer (arbitrary size; covers all integer types in C)
type       PyLongObject;  // arbitrary size integer
identifier PyLong_Type;   // type of PyLongObject
identifier PyLong_Check;  // checks if object is of type PyLongObject

// conversion from type
identifier PyLong_FromLong;         // converts from long
identifier PyLong_FromUnsignedLong; // converts from unsigned long
identifier PyLong_FromSsize_t;      // converts from Py_ssize_t
identifier PyLong_FromSize_t;       // converts from size_t
identifier PyLong_FromLongLong;     // converts from long long
identifier PyLong_FromUnsignedLongLong; // converts from unsigned long long
identifier PyLong_FromDouble;       // converts from double

// conversion to type (complements to above From... functions (returns target type))
identifier PyLong_AsLong;
identifier PyLong_AsUnsignedLong;
identifier PyLong_AsSsize_t;
identifier PyLong_AsSize_t;
identifier PyLong_AsLongLong;
identifier PyLong_AsUnsignedLongLong;
identifier PyLong_AsDouble;

//----------------------------------------------------------------------------------------------------------------------

/// floating point (no arbitrary precision; probably corresponds to 'double' in C)
type       PyFloatObject;    // floating point
identifier PyFloat_Type;     // type of PyFloatObject
identifier PyFloat_Check;    // checks if object is of type PyFloatObject
identifier PyFloat_FromDouble; // converts from double
identifier PyFloat_AsDouble;   // converts to double

//----------------------------------------------------------------------------------------------------------------------

/// boolean
type       PyBoolObject;
identifier PyBool_Type;
identifier PyBool_Check;
identifier Py_True;  // true singleton; immortal PyObject* representing true
identifier Py_False; // false singleton; immortal PyObject* representing false

//----------------------------------------------------------------------------------------------------------------------

/// low level bytes array (typically not recognized as string inside python)
identifier PyBytesObject; // string representation (unicode encoding)
identifier PyBytes_Type;
identifier PyBytes_Check;
identifier PyBytes_Size;  // returns size (ssize_t) of aray
identifier PyBytes_FromString; // creates object from C-String
identifier PyBytes_AsString; // returns char* pointer to contents

//----------------------------------------------------------------------------------------------------------------------

/// generic string of characters (unicode)
identifier PyUnicodeObject; // string representation (unicode encoding)
identifier PyUnicode_Type;
identifier PyUnicode_Check;
identifier PyUnicode_GET_LENGTH; // returns length of string
identifier PyUnicode_FromString; // creates unicode object from C-String in UTF-8 encoding
identifier PyUnicode_AsEncodedString; // creates PyBytesObject with encoded string

//----------------------------------------------------------------------------------------------------------------------

/// list (dynamic of arbitrary objects)
type       PyListObject;   // list object
identifier PyList_Type;    // type of PyListObject
identifier PyList_Check;   // checks if object is of type PyListObject
identifier PyList_New;     // creates list of ssize_t elements (one argument)
identifier PyList_Size;    // returns size (ssize_t)
identifier PyList_GetItem; // retrieves indexed (ssize_t) element  (NOTE: returns a borrowed reference (weak reference))
identifier PyList_SetItem; // sets indexed (ssize_t) element  (NOTE: steals reference from passed element)
identifier PyList_Append;  // appends item to list (?? without reference stealing ??)

//----------------------------------------------------------------------------------------------------------------------

/// dictionary
type       PyDictObject;  // dictionary object (map of keys and objects)
identifier PyDict_Type;   // type of PyDictObject
identifier PyDict_Check;  // checks if object is of type PyDictObject
identifier PyDict_New;    // creates dictionary
identifier PyDict_Clear;  // clears dictionary
identifier PyDict_Contains; // checks if dictionary contains key
identifier PyDict_Copy;
identifier PyDict_SetItem;  // sets key-value
identifier PyDict_GetItemWithError;  // retrieves value from key (borrowed reference) (do not use PyDict_GetItem and PyDict_GetItemString)
identifier PyDict_SetItemString;  // sets key-value
identifier PyDict_DelItem;  // deletes key
identifier PyDict_Next;     // iterate over all dictionary items

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// Errors

identifier PyErr_Occurred; // returns borrowed reference to error object, if occurred
identifier PyErr_Print;    // prints last error to stderr  (use only when one occurred)
identifier PyErr_DisplayException; // prints specified error to stderr  (use only when one occurred)
identifier PyErr_Clear; // clears all errors

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// Running a program

//----------------------------------------------------------------------------------------------------------------------

identifier PyRun_String;  // Executes commands from a string
identifier Py_file_input;   // argument for PyRun_String: used to indicate a sequence of statements
identifier Py_single_input; // argument for PyRun_String: used to indicate a single statements
identifier Py_eval_input; // argument for PyRun_String: used to indicate a single expression

identifier Py_CompileString; // Compiles commands from a string
identifier PyEval_EvalCode;  // Executes code generated by Py_CompileString


//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

/**********************************************************************************************************************/
/// Global Framework

//----------------------------------------------------------------------------------------------------------------------

identifier Py_Initialize;
identifier Py_FinalizeEx;
identifier Py_Exit;

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

