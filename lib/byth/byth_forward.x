/** Author and Copyright 2024 Johannes Bernhard Steffens
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

/// object
type       PyObject;   // python object
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
identifier PyDict_GetItem;  // retrieves value from key (borrowed reference)
identifier PyDict_SetItemString;  // sets key-value
identifier PyDict_GetItemString;  // retrieves value from key (borrowed reference)
identifier PyDict_DelItem;  // deletes key

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

