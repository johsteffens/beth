# BTCL - Beth Text-based Constructive Language

## What it is

BTCL is a simple, easy to use, text based and weakly typed functional language. Its purpose is constructing an object.

BTCL is backward compatible to [BTML](btml.md).

## Constructive Language
I use the term ***constructive language*** to specify a language specialized for object construction. It can be placed somewhere between mere markup and general purpose programming. It has the marks of a mature language but offers limited interactivity. It is intended to represent a state rather than a process.

Specifically:

  * Contiguous code represents [one expression](#one-expression) (built from expressions or literals). There are no statements.
  * An expression represents an object.
  * Functions are first class objects.

## Cheat Sheet

The language supports C-style comments inside the code.

``` C
// String
"some text";

// Variable definition and representation of its value '7'.
x = 7;

// Expression (Evaluates to 14).
1 + 2 * 3 + x;

// Function definition.
f = func( a, b ) { a + b };

// Applying above function. (Evaluates to 7)
n = f( 3, 4 );

// bmath object representation via btml
y = <bcore_arr_s3_s> 1 2 3 </>;

// same result as above (using btcl array initialization)
z = <bcore_arr_s3_s></>( [1,2,3] );

// element initialization
<bcore_main_frame_s></>( .create_log_file = TRUE, .log_file_extension = "log" );

// prints object z to stdout (for messaging, inspection, debugging)
PRINT(z);  // compact formatting where possible
PRINTX(z); // always btml format

// condition (else-part is optional)
if( a >= b ) { a } else { b };

// List with elements 1, 2, 3
[1,2,3];

// concatenation of objects or lists to form a list
a : b; // if a or b is a list, the list is extended (not nested)

// Initialized list with 5 constants; result is [0,0,0,0,0]
5 :: 0;

// Initialized list with 5 elements via initializer-function.
// Index i runs from 0 to 4. The result is [0,2,4,6,8].
5 :: func(i){2*i};

// Mapped list through function; the function is applied to list elements
// forming a new list. The result is [2,4,10].
[1,2,5] :: func(a){2*a};

// embed the content from another file
// relative path is relative to current file location
embed ( "../data/file.btcl" );

// converting a function into a functor (a functor can be used externally)
<x_btcl_functor_s/>( func( a, b ) { a + b } );

// A trailing semicolon as last valid symbol in a file, block or frame is ignored (no continuation)

```

# Comments
BTCL uses C/C++ style comments:

*  ```//```: Comment until the end of line.

*  ```/* ... */ ```: Comment block.

# One Expression
BTCL has no distinct statements. Any contiguous code represents just one expression. This can be a composite (or tree) of expressions joined via functions, operators or conditional branches. Non-composite expressions are called ***Literals***.

## Continuation operator
Although the semicolon ';' is traditionally understood as separator for consecutive statements, in BTCL it is a binary operator joining two consecutive expressions. Hence the semicolon behaves as a [binary operator](#operators).
In BTCL this operator is called **Continuation**.

```a ; b``` means:

Evaluate first ```a``` then ```b``` but represent only the result of ```b```. 

Superficially, this appears to render expression ```a``` meaningless. However, ```a``` can define a variable, which is visible in ```b```. Thus ```a``` creates context for ```b```. This helps simplifying complex expressions and can make code easier to read and maintain.

More generally: The semicolon operator divides consecutive expressions into
**context creation** and **context usage**.

**Example:**

```
// 7.0/8.0 expressed via a sigmoid function
( x=7.0; x/(ABS(x)+1) )
```

**Note:**
A trailing semicolon is allowed for the last expression in a file, block or frame. In that case no continuation is expected; the semicolon is simply ignored.

## Chain of Expressions

Multiple consecutive expressions can be chained up, much like a list of statements in a procedural language would be chained. The advantage compared to a procedural language is that such a chain can be part of any sub-expression.

# Operators

Each operator has a numeric priority. On chained operations, higher priority operators are evaluated before lower priority operators. Equal operators are evaluated in the given order (e.g. ```4-1-1 == (4-1)-1 == 2```, ```8/2/2 == (8/2)/2 == 2```).
Operators are grouped into priority-groups. Each group is associated with a letter A ... E. Operators in group A have highest and also identical priority.
All other operators have each a unique priority. A higher group letter means lower priority. A lower position within the group means lower priority.

The following tables contain available operators:

* **Symbol:** The symbol used in BTCL Syntax
* **Type Name:** Identifier for the operator. 
  * The type name is for BTCL internal bookkeeping, error reporting and special purpose BTCL extensions.
* **Exportable:** When irreducible, the operator can be exported as a meta object (part of constructed object) where an external builder gives it a suitable meaning. (BTCL solution for polymorphism). Exportable operators are important for constructing a [Functor](#functor).


## Group A - Binary

|Symbol|Description|Type Name|Exportable|
|:---|:---|----|----|
|```.```|Stamp: Member access; [Node](btcl_network_builder.md): Branch access|member|no|
|```(```|Function call or stamp modifier; closed by ')'|frame|no|

## Group B - Unary

|Symbol|Description|Type Name|Exportable|
|:---|:---|----|----|
|```+```|Identity|identity|yes|
|```-```|Arithmetic Negation|neg|yes|
|```!```|Logic Negation|not|yes|

## Group C - Binary

|Symbol|Description|Type Name|Exportable|
|:---|:---|----|----|
|```^```|**Arithmetic**: exponentiation; result type is f3_t|pow|yes|
|```/```|**Arithmetic**: division|div|yes|
|```%```|**Arithmetic**: modulo division|mod|yes|
|```**```|**[Function-Function](#function-operators)**: Chains two functions|chain|yes|
|```*.:```|**[Function-List](#function-operators)**: Applies function to unfolded list elements|mul_dot_colon|no|
|```*.```|**[Function-List](#function-operators)**: Applies function to unfolded list|mul_dot|no|
|```*:```|**[Function-List](#function-operators)**: Applies function to list elements|mul_colon|no|
|```*```|**Arithmetic, [List](#list-operators), [Function](#function-operators)**: multiplication; Unary function application; Assigning r-expression as argument|mul|yes|
|```-```|**Arithmetic**: subtraction|sub|yes|
|```+```|**Arithmetic**: addition; Concatenation of strings.|add|yes|
|```::```|**[List](#list-operators)**: [Spawning Operator](#Spawning-Operator)|spawn|yes|
|```:```|**[List](#list-operators)**: Concatenation of objects to form a list; Concatenation of lists|cat|yes|
|```==```|**Logic**: equal|equal|yes|
|```!=```|**Logic**: unequal|unequal|yes|
|```>=```|**Logic**: larger or equal|larger_equal|yes|
|```>```|**Logic**: larger|larger|yes|
|```<=```|**Logic**: smaller or equal|smaller_equal|yes|
|```<```|**Logic**: smaller|smaller|yes|
|```&&```|**Logic**: AND|and|yes|
|\|\||**Logic**: OR|or|yes|
|```<<```|**[Function](#function-operators)**: Assigning r-expression as argument|shift_left|yes|
|```=```|Assignment|assign|no|

## Group D (Reserved)

## Group E - Binary

|Symbol|Description|Type Name|Exportable|
|:---|:---|----|----|
|```;```|[Continuation](#continuation-operator)|continuation|no|

# Number
Numbers are represented as integer ```s3_t``` or floating point ```f3_t```.

Integer literals can be expressed as decimal or, when using prefix ```0x``` as hexadecimal.

Float literals are specified via decimal point (even if the value is a whole number), or using exponential notation.

## Examples

``` C
1023578     // int (s3_t) in decimal form
0x378FCD50  // int (s3_t) in hexadecimal form
123.0       // float (f3_t)
123E1       // float (f3_t)
```
## Arithmetic
Binary arithmetic operators return f3_t when one of the operands is f3_t, otherwise they return s3_t.

# String
Sting literals are specified using the C-syntax:

``` C
"This is a string"
"\tThis is a string with tab and newline\n"
```

## Operator
Operator '+' concatenates string with string or string with number by first converting the number to a string

|Operation|Result|
|:---|:---|
|"ab"+"cd"|"abcd"|
|"ab"+12|"ab12"|
|12+"ab"|"12ab"|
|""+12|"12"|

# Logic
Logic literals are the keywords ```true``` and ```false```.

A logic value can be drived from a numeric or boolean expression. Numbers are evaluated ```false``` when they are zero and ```true``` otherwise.

Logic binary operators return ```bl_t``` as result.

# Frame
A frame represents the lexical context of a code segment. Variables defined inside a frame are visible inside (including sub-frames) but not outside the frame. A variable definition inside a frame masks any variable of the same name outside that frame.

A sub frame is created by using a [bracket](#bracket) ```(...)``` or a
[block](#block) ```{...}```

Any btcl code is inside of the **global frame**.

# Conditional Expression

## Syntax:

``` C
if (<condition>) { <expression> }
if (<condition>) { <expression1> } else { <expression2> }
```

Depending on the condition, the associated block is either evaluated or skipped.

# Signature

The signature represents the interface of a function. It is identified by the keyword ```func```.
A function is created by joining a signature with a body.

## Syntax
``` C
func( <args, comma separated> )
```

# Bracket

A bracket prioritizes an expression or indicates a function call. A bracket defines a dedicated [subframe](#frame) in which it is evaluated.

## Syntax

``` C
( <expression> )
```

# Block

A block is enclosed in braces ```{...}```.

A block reserves the evaluation of an expression. It is used as part of a function or conditional expression. A block defines a dedicated [subframe](#frame) in which it is evaluated.

<sub>Note: A block is not suitable for immediate (framed) evaluation,
as might be in other programming languages. Use the simple
bracket ```( ... )``` for that purpose. </sub>

## Syntax

``` C
{ <expression> }
```

# Function

A function is defined by joining a [signature](#signature) with a [block](#block).

## Syntax

``` C
<signature> : <block>

or

<literal-signature> <literal-block>
```

## Example
``` C
f = func( a, b ) { a + b }; // function definition; ':' can be omitted
f( 1, 2 )                   // function usage; result is 3
```

## Example
``` C
s = func( a, b ); // signature definition
b = { a + b };    // block definition
f = s : b;        // function definition
f( 1, 2 ) 		  // function usage; result is 3
```

## Lexical Frame
The lexical frame of a function is the frame in which a [function](#function) is used. It is not where the function is defined. 

#### Reasoning: 

The lexical frame in which the function is defined might not exist at the point of usage. This happens, for example, when the function is defined and returned from another function. 

Some languages allow the lexical frame to be the frame of definition, which is deemed more intuitive. Others (e.g. C,C++) circumvent the problem by disallowing function definitions in nested frames.

#### Good practice:

**Best:** Avoid making assumptions about frame properties outside the function. Prefer using the function's argument list for passing any and all external data. 

**Second best**: At most assume a global context defined in the root frame.

## Recursion
The keyword ```self``` represents the function in which it is used.

### Example
``` C
// factorial function using recursion
factorial = func( a )
{
    if( a > 1 ) { a * self( a - 1 ) } else { a }
};

factorial( 3 ) // result is 6 (=1*2*3)
```

Prefer using ```self``` for recursions.

## Partial Calls

A partial function call is a call that define only a subset of arguments. This creates a new function behaving like the old but with the first arguments replaced by constants and the remaining arguments as the new argument set.

### Example
``` C
// f has arity 3
f = func( a, b, c )
{
    a + b + c;
};

// We define g as f with first two arguments fixed (a=1, b=2).
// g has arity 1
g = f( 1, 2 );

g( 3 ) // result is 6 ( = 1+2+3 )
```

## Function Operators

Binary operators where the left operand is a function.

|Operator|Description|
|:---|:---|
|```*```|Applying a function: ```f*x == f(x)```|
|```<<```|Applying a function: ```f<<x == f(x)```; Like operator ```*``` except that ```<<``` has lower priority|
|```**```|Chaining two functions: ```(f1**f2)(x) == f2(f1(x))```|
|```*:```|Transforming a list (f applied to list elements)|
|```*.```|Applying a function using list elements as arguments: ```f*.l == f(l.[0], l.[1], ...)```|
|```*.:```|Transforming a list of lists (Applying f *. l.[i] on list elements)|


## Functor
A functor is an exportable function. It is represented by the object ``` x_btcl_functor_s```;

A functor can be used outside the btcl parser framework.

A BTCL Function can be converted into a functor.

**Explicit conversion:** See example below.

**Implicit conversion:** A function is implicitly converted into a functor when it is externalized.

**Example:**

```C
f = func( a, b, c ) { a + b + c };

<x_btxl_functor_s/>( f ); // explicit conversion

<my_stamp_s/>( .my_member = f ); // implicit conversion

f; // if the script ends here, f is implicitly converted
```

Note: If a function contains inexportable syntax, the conversion attempt fails with a parsing error.  Inexportable syntax is generated when at least one argument is involved in ...

* a branch-condition: ```func(a){ if( a > 0 ) { do_this } else { do_that } }```
* an operand for inexportable operators
* an object modifier

### Using a Functor

``` x_btcl_functor_s``` provides member function to set arguments and to execute the function.

Execute functions are thread safe.

**Example:**

Script file:

```C
func( a, b, c ) { a + b + c };
```

XOILA Code:

```C
m x_inst* inst = x_btcl_create_from_file( "scipt_file.btcl" )^;
if( !inst ) = bcore_error_last();

ASSERT( inst._ == x_btcl_functor_s~ );
m x_btcl_functor_s* functor = inst.cast( m x_btcl_functor_s* );

// sets areguments
functor.set_arg_f3( 0, 2.0 );
functor.set_arg_f3( 1, 3.0 );
functor.set_arg_f3( 2, 4.0 );

// executes function: should output 9 (=2+3+4)
bcore_msg_fa( "#<f3_t>\n", functor.call_to_f3() );
```




# List

``` C
// List literal
mylist = [1,2,3];

// A trailing comma in a non-empty list is allowed
mylist = [1,2,3,]; // same as [1,2,3];

// Element access
mylist.[ 2 ] // is 3 here

// Creating a list by concatenating elements of lists a,b,c
mylist = a : b : c;

// concatenating: if any operand is already a list, it will be unfolded and extended
[1,2]:[3,4] == [1,2,3,4]; // this is TRUE

// concatenating: to explicitly insert a list as element to another list, fold it twice:
a = [1,2];
b = a:[[4,5]];
b == [1,2,[4,5]]; // this is TRUE

```

Stamps, which are arrays can be initialized with a list
by using a modifier:

``` C
list = [1,2,3];
arr = <bcore_arr_s3_s></>( list );

```

The SIZE operator can retrieve the number of elements of a list or array:

``` C
list = [1,2,3];
SIZE(list); // this is 3
list.SIZE(); // this is 3
SIZE(<bcore_arr_s3_s>1 2 3</>); // this is 3

```

## List Operators

### List Multiplication

Operation ```a * b``` where both operands are lists generate a list product defined as follows

``` C
a*b == [ a.[0]:b.[0], a.[0]:b.[1], ..., a.[1]:b.[0], ... a.[n-1]:b.[m-1] ]
```

**Example**

``` C
[1,2] * [1,2,3] == [ [1,1], [1,2], [1,3], [2,1], [2,2], [2,3] ]
```
### Spawning Operator

Binary Operator ```::``` is a multi-tool for constructing or modifying lists or run a recursion. It is used for list based operations using simple rules based on the argument's type.

|a-type|b-type|Result|
|:---|:---|:---|
|number|unary function|List of a elements, each set to ```b(index)```
|number|any other|List of a elements, each set to value b
|list|binary function|Spawned recursion (s. below for details)

### Spawned Recursion
The operation O(L<sub>n</sub>,F) with ...

  * L<sub>n</sub> being a list with n > 0 elements: L[0], ..., L[n-1]
  * L<sub>k</sub> (k<=n) being the leftbound sublist of L<sub>n</sub> with k elements.
  * F being a binary function

... is defined as ...

  * O(L<sub>1</sub>,F) = L[0]
  * O(L<sub>k</sub>,F) = F(O(L<sub>k-1</sub>,L[k-1]))

**Example**

``` C
4 :: b == [b,b,b,b];
4 :: func(i){i} == [0,1,2,3];

f = func(a,b){...};
[1,2,3,4] :: f == f(f(f(1,2),3),4);

```

# Stamp
A general stamp can be instantiated via btml:

``` C
anystamp = <bcore_arr_st_s> /* any btml code here */ </>
```
``` C
anystamp = <bcore_arr_st_s/> // brief instantiation
```
Stamp members can be accessed via '.' operator.

## Stamp Modifiers
A stamp modifier changes a stamp. The result of the modifier  expressioon is a copy of the original stamp with the specified parts modified.

### Entire Stamp
Modifying the entire stamp can be done using a function style syntax:

``` C
<bcore_arr_st_s/>( ["a","b","c"] );
```

The stamp is modified via [generic conversion ](#generic-conversion).

### Elements
Stamp elements can be changed after instantiation via stamp modifiers. Syntactically is has the style of a function call, with elements to be modified identified in the form
```.<element_name> = <source>```
as assignment expression.

``` C
<bcore_main_frame_s/>( .create_log_file = TRUE, .log_file_extension = "log" );
```
Stamp elements are modified using [generic conversion ](#generic-conversion)

### Generic Conversion
Generic conversion is a btcl-specific type conversion when general stamps are involved in an assignement/modification operation using following criteria in given order until one succeeds
*  If source and destination have the same type, as simple copy is instigated.
*  If feature 'copy_from' is overloaded for given type it is used.
*  A generic btcl-conversion is used:
   *  If both types are arrays, the destination array is filled with elements of source.


* If none of the above succeeds, an error message is generated.

# Built-in Functions

Built-in functions are available by the names listed in the table below. They are exportable as unary operator.

|Name|Description|Type Name|Exportable|
|:---|:---|----|----|
|EXP|Exponentiation base ```e```|exp|yes|
|LOG|Logarithm base ```e```|log|yes|
|LOG2|Logarithm base ```2```|log2|yes|
|LOG10|Logarithm base ```10```|log10|yes|
|SIN|Sine|sin|yes|
|COS|Cosine|cos|yes|
|TAN|Tangent|tan|yes|
|TANH|Hyperbolic Tangent|tanh|yes|
|SIGN|Sign of value: ```1 or -1```|sign|yes|
|SQRT|Square root|sqrt|yes|
|ABS|Absolute value|abs|yes|
|CEIL|Ceiling function|ceil|yes|
|FLOOR|Floor function|floor|yes|
|PRINT|Prints object to stdout in compact form; behaves as identity|print|no|
|PRINTX|Prints object to stdout in detailed form; behaves as identity|printx|no|

# Built-in Constants
|Name|Description|
|:---|:---|
|TRUE|Boolean 'true'|
|FALSE|Boolean 'false'|
|true|Boolean 'true'|
|false|Boolean 'false'|
|PI|Math: Pi constant|
|PATH|Path of source file; error if source has no file associated|
|DIR|Directory of source file; error if source has no file associated|

# Including other source files.

The keyword **embed** evaluates code from another file.

## Syntax
```
embed ( "path_to_another_file.txt" )
```
If the file path is relative, it is taken relative to the folder in which the current source is located.

The file is embedded in the current frame (no dedicated frame). This allows defining variables (functions) in the embedded file to be used outside the embedding.

# Advanced
The features below provide special control and functionality for specific use cases that go beyond the typical use of btcl.

* [External Functions](btcl_external_functions.md): Beth-BTCL API for calling external functions from BTCL code.
* [Network Builder](btcl_network_builder.md): Construction of a network meta structure.



------
<sub>&copy; 2024 Johannes B. Steffens</sub>

