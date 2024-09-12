# Embedded Python via Python/C API

Objects in this library make use of the &#x1F517;[Python/C API](https://docs.python.org/3/c-api/index.html) .

### Prerequisites

- Install python packages:
  - `sudo apt install python3-dev`
  - `sudo apt install python3-matplotlib`

- One can use `python3-config` for release settings in the makefile:
  - `python3-config --embed --cflags`  for compiler flags
    - Additional compiler flags to counteract some undesired `python3-config` settings: `-Wno-sign-compare -Wno-comment`

  - `python3-config --embed --ldflags` for linker flags

- Debug mode: `python3-config` does not seem to offer a dedicated debug mode. Workaround: Dissect the returned flags and adapt them to the desired debug settings. Only compiler flags need modifications. Following additional settings work for the current python3 release (might change in future)
  - `-I/usr/include/python3.10 -fstack-protector-strong`


## byth_obj_s

*  Wrapper of the generic python object PyObject. 
*  Provides an interface to convert between beth objects and python objects.
  *  No ABI assumptions: Conversions between beth and python involve full data copying.
*  (!) Thread Safety
   *   ```byth_obj_s``` const-functions are not concurrent unless the Python API used is entirely concurrent.
   *  Use byth_frame-locks to be on the safe side.


### Leaf Objects
| Beth-Type | Python Type | Notes    |
| :------- | :---------- | :------- |
| u0_t, u1_t, u2_t | PyLong | via `UnsignedLong` functions |
| u3_t | PyLong | via `UnsignedLongLong` functions |
| tp_t | PyLong | via `UnsignedLongLong` functions |
| s0_t, s1_t, s2_t | PyLong | via `Long` functions |
| s3_t | PyLong | via `LongLong` functions |
| uz_t | PyLong | via `Size_t` functions |
| sz_t | PyLong | via `Ssize_t` functions |
| bl_t | Py_True or Py_False | py constants |
| f2_t, f3_t | PyDouble | via `Double` functions |
| st_s | PyUnicode | UTF8 encoding |
| sc_t | PyUnicode | UTF8 encoding; only beth -> python |

### General x_inst
Stamps are normaly converted into **PyDict**. converting the bcore_via mapping into a dictionary mapping using the same key names.

A pure array is converted into **PyList** using the key `"_pure_array_"`

## Conversion Functions
| Name | Descrition |
| :------- | :---------- |
| from_t_inst, from_inst | Conversion from a beth instance |
| to_t_inst, to_inst | Conversion to a beth instance |
| to_sr | Conversion to a beth instance by creating a smart reference |
