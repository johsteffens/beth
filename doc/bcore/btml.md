# BTML - Beth Text-based Markup Language

## What it is

BTML is the standard markup language in beth used to serialize beth-objects.

BTML aims to be self explanatory and editable in a plain-text editor.

## Cheat Sheet

``` C
<f3_t> 123.4 </>  // f3_t (number) leaf type 
123.4  // shortcut for f3_t

<st_s> some text </>  // st_s (string) leaf type 
"some text" // shortcut for st_s

/* This stamp
stamp myobject =
{
    st_s string;
    tp_t id;
};

serializes to
*/
<myobject>
    string:<st_s>"Some string text"</>
    id:<tp_t>1234</>
</>

/// Shortcuts:
<myobject></> 	// no data passed
<myobject/> 	// no data passed

```

## Comments

BTCL uses C/C++ style comments:

*  ```//```: Comment until the end of line.

*  ```/* ... */ ```: Comment block.

## Generic Syntax

BTML has the following generic syntax for stamps.

```
<type>
   member1: object-data
   member2: object-data
   ...
</>
```

For various types simplified encoding is possible:

* Numbers by just stating the value
* Strings by using quotes ```"..."```

## Version tolerance
Changes in the (stamp-) object do not (generally) produce a parser error. This allows handling of different stamp version without explicit version contol. The following conditions are handled:

* **New stamp element:** An incomplete set of members is allowed by setting unspecified elements to their respective default.
* **Retired stamp element:** An unrecognized element name is ignored by the parser.
* **Changed type:** A type change invokes automatic type conversion. Error in case the type cannot be converted.
* **Changed indirection (static vs link):** Changed indirection is converted intuitively.
* **Retired stamps:** An unrecognized object type is skipped by the parser and treated as if 'NULL' was parsed.

## Overloading
All stamps support the BTML syntax natively. Overloading is discouraged and should be used sparingly. Overloading might be justified in case the stamp must call specific initialization code or two-way syntax change is desirable. If overloading is considered purly to simplify syntax for one-way parsing, try using [BTCL](btcl.md) instead.

#### I/O syntax
Overload following features in [bcore_x_btml.h](../../lib/bcore/bcore_x_btml.h) to define own I/O syntax for a stamp
``` C
btml_body_to_sink
btml_body_from_source
```
#### Initialization and Preparation
Overload following features in [bcore_spect_via_call.h](../../lib/bcore/bcore_spect_via_call.h) for additional stamp handling around serialization.
``` C
bcore_via_call.source
bcore_via_call.mutated
bcore_via_call.shelve
```

## Special Features

#### Instationiation with defaults
Instantiation without explicitly passing data: ```<mystamp_s/></>``` may also be expressed as ```<mystamp_s/>```

#### Embedding a file
**Syntax:** ```<#file> <string in quotes> </>``` (Closing ```</>``` is optional)

**Example:** ```<#file> "data/my_file.txt" </>```

```<#file>``` Represents the object to be constructed from the file's location. Error in case the file does not exist.

#### Resolving a Path
**Syntax:** ```<#path> <string in quotes> </>``` (Closing ```</>``` is optional)

**Example:** ```<#path> "../my_file.txt" </>```

```<#path>``` Represents a string ```<st_s>``` containing the resolved path. It is useful for relative paths that are taken relative to he stream-location. For absolute paths ```<#path>``` and ```<st_s>``` yield identical output.

#### Default Object Definition
Syntax: (to be placed at the beginning of the body):
```#default: <object definition>```

**Usage:**
Inside an object-body, an optional default object can be defined. This default object acts as template for all object creations of the same type inside the body. It is typically useful in arrays in order to specify a set of parameters in array-elements so that they need not be explicitly stated each time.

**Example:**
``` C
<my_array_s>
	// default definition
	#default: <my_element_s> v1: 10 v2: 20 </>

	// elements definition (v1, v2 assume default values as specified above)
	<my_element_s> v3: 1  </>
	<my_element_s> v3: 2  </>
	<my_element_s> v3: 3  </>
</>
```

------
<sub>&copy; Johannes B. Steffens</sub>

