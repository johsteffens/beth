# BTCL - Beth Text-based Constructive Language

## What it is

BTCL is a simple and easy to use text based weakly typed functional language
to construct an object.

## Constructive Language
I use the term ***constructive language*** to specify a language specialized for
object construction. It can be placed somewhere between mere markup and general
purpose programming. It offers variables, operators and functions but offers limited
interactivity. It it intended to represent a state rather than a process.

Specifically:

  * Contiguous code represents [one expression](one-expression) (built from expressions or literals). There are no statements.
  * An expression represents an object.
  * Functions are first class objects.

## Cheat Sheet

``` C
// String
"some text";

// Variable definition and representation of its value '7'.
x = 7;

// Expression (Evaluates to 14).
1 + 2 * 3 + x;

// Function definition.
f = func( a, b ) { a + b };

// Calling above function. (Evaluates to 7)
n = f( 3, 4 );

// bmath object representation via btml
y = <bcore_arr_s3_s> 1 2 3 </>;

// same result as above (using btcl array initialization)
z = <bcore_arr_s3_s></>( [1,2,3] );

// btcl element initialization
<bcore_main_frame_s></>( .create_log_file = TRUE .log_file_extension = "log" );

// prints object z to stdout (for messaging, inspection, debugging)
 ? z;  // compact formatting where possible
?? z; // always btml format

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

```

# One Expression
BTCL has no distinct statements. Any contiguous code represents just one expression.
This can be a composite (or tree) of expressions joined via functions,
operators or conditional branches. Non-composite expressions are called ***Literals***.

## Continuation operator
Although the semicolon ';' is traditionally understood as separator for
consecutive statements, in BTCL it is a binary operator combining two
consecutive expressions. Hence the semicolon behaves as a [binary operator](#operators).
In BTCL this operator is called **Continuation**.

```a ; b``` means:

Evaluate first ```a``` then ```b``` and represent the result of ```b```.
Superficially, this seems to render ```a``` meaningless. However ```a```
can define a variable, which is used in ```b``` at multiple places.

**Example:**

```
// 7.0/8.0 expressed via a sigmoid function
( x=7.0; x/(ABS(x)+1) )
```

More generally: The semicolon operator divides consecutive expressions into
**context creation** and **context usage**.

## Chain of Expressions

Multiple consecutive expressions can be chained up much like a list of statements in a procedural language would be chained. 
However, the last expression representing the sate of the chain must not be terminated by a semicolon.
The advantage compared to a procedural language is that such a chain can be part of any sub-expression.

# Operators

Each operator has a numeric priority. On chained operations, higher priority
operators are evaluated before lower priority operators. Equal operators are
evaluated in the chained order (e.g. ```3 - 1 - 1 == 2```).
Additionally operators are grouped into priority-groups. Each group is
associated with a letter A ... E. Operators in group A have highest and also identical prority.
All other operators have each a unique priority. A higher group letter means lower priority.
A lower position within the group means lower priority.

## Group A - Binary

|Symbol|Description|
|:---|:---|
|```.```|Stamp member access|
|```(```|Function call or stamp modifier; closed by ')'|

## Group B - Unary

|Symbol|Description|
|:---|:---|
|```+```|Identity|
|```-```|Arithmetic Negation|
|```!```|Logic Negation|
|```?```|Identity: Object is printed to stdout. Leaf objects are printed in compact form.|
|```??```|Identity: Object is printed in detail to stdout.|

## Group C - Binary

|Symbol|Description|
|:---|:---|
|```^```|**Arithmetic**: exponentiation; result type is f3_t|
|```/```|**Arithmetic**: division|
|```%```|**Arithmetic**: modulo division|
|```**```|**[Function](#function-operators)**: Chains two functions|
|```*.:```|**[Function](#function-operators)**: Applies function to unfolded list elements|
|```*.```|**[Function](#function-operators)**: Applies function to unfolded list|
|```*:```|**[Function](#function-operators)**: Applies function to list elements|
|```*```|**Arithmetic,[List](#list-operators),[Function](#function-operators)**: multiplication; Unary function application|
|```-```|**Arithmetic**: subtraction|
|```+```|**Arithmetic**: addition; Concatenation of strings.|
|```::```|**[List](#list-operators)**: [List construction](#list-construction)|
|```:```|**[List](#list-operators)**: Joining objects to form a list; Concatenation of lists|
|```==```|**Logic**: equal|
|```!=```|**Logic**: unequal|
|```>=```|**Logic**: larger or equal|
|```>```|**Logic**: larger|
|```<=```|**Logic**: smaller or equal|
|```<```|**Logic**: smaller|
|```&```|**Logic**: AND|
|```|```|**Logic**: OR|
|```=```|Assignment|

## Group D (Reserved)

## Group E - Binary

|Symbol|Description|
|:---|:---|
|```;```|[Continuation](continuation-operator)|

# Number
Numbers are represented as integer ```s3_t``` or floating point ```f3_t```.

Integer literals can be expressed as decimal or, when using prefix ```0x```
as hexadecimal.

Float literals are specified via decimal point
(even if the value is a whole number), or using exponential notation.

## Examples

``` C
1023578     // int (s3_t) in decimal form
0x378FCD50  // int (s3_t) in hexadecimal form
123.0       // float (f3_t)
123E1       // float (f3_t)
```
## Arithmetic
Binary arithmetic operators return f3_t when one of the operands is f3_t,
otherwise they return s3_t.

# String
Sting literals are specified using the C-syntax:

``` C
"This is a string"
"\tThis is a string with tab and newline\n"
```

## Operator
Operator '+' concatenates string with string or string with number by
first converting the number to a string

|Operation|Result|
|:---|:---|
|"ab"+"cd"|"abcd"|
|"ab"+12|"ab12"|
|12+"ab"|"12ab"|
|""+12|"12"|

# Logic
Logic literals are the keywords ```true``` and ```false```.

A logic value can be drived from a numeric or boolean expression.
Numbers are evaluated ```false``` when they are zero and ```true``` otherwise.

Logic binary operators return ```bl_t``` as result.

# Frame
A frame represents the lexical context of a code segment.
Variables defined inside a frame are visible inside (including sub-frames)
but not outside the frame. A variable definition inside a frame masks any
variable of the same name outside that frame.

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

The signature represents the interface of a function.
A function is created by joining a signature with a body.

## Syntax
``` C
func( <args, comma separated> )
```

# Bracket

A bracket prioritizes an expression or indicates a function call.
A bracket defines a dedicated [subframe](#frame) in which it is evaluated.

## Syntax

``` C
( <expression> )
```

# Block

A block is enclosed in braces ```{...}```.

A block reserves the evaluation of an expression.
It is used as part of a function or conditional expression.
A block defines a dedicated [subframe](#frame) in which it is evaluated.

<sub>Note: A block is not suitable for immediate (framed) evaluation,
as might be in other programming languages. Use the simple
bracket ```( ... )``` for that purpose. </sub>

## Syntax

``` C
{ <expression> }
```

# Function

A function is defined by joining a signature with a block.

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
The lexical frame is the frame in which a function us used and not where
it is defined.

**Reason:** The lexical frame in which the function is defined might not
exist at the point of usage. This happens when the function is defined
and returned from another function.

**Good practice:** Sometimes it can be helpful to call other functions from
the lexical frame. But generally, it is recommended to avoid assuming a
specific lexical frame from inside a function at all. Prefer using the
function's argument list for passing any and all external data.

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

## Function Operators

Binary operators where the left operand is a function.

|Operator|Description|
|:---|:---|
|```*```|Applying a function: ```f*x == f(x)```|
|```**```|Chaining two functions: ```(f1**f2)(x) == f2(f1(x))```|
|```*:```|Transforming a list (f applied to list elements)|
|```*.```|Applying a function using list elements as arguments: ```f*.l == f(l.[0], l.[1], ...)```|
|```*.:```|Transforming a list of lists (Applying f *. l.[i] on list elements)|

# List

``` C
// List literal
mylist = [1,2,3];

// Element access
mylist.[ 2 ] // is 3 here

// Creating a list by concatenating elements or lists
mylist = a : b : c;

// if any operand is already a list, it will be unfolded and extended
[1,2]:3 == 1:2:3; // this is TRUE

// to explicitly insert a list as element to another list, fold it twice:
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

Operation ```a * b``` where both operands are lists generate a list product
defined as follows

``` C
a*b == [ a.[0]:b.[0], a.[0]:b.[1], ..., a.[1]:b.[0], ... a.[n-1]:b.[m-1] ]
```

**Example**

``` C
[1,2] * [1,2,3] == [ [1,1], [1,2], [1,3], [2,1], [2,2], [2,3] ]
```
### Spawning Operator

Operation ```a::b``` can construct or modify lists or run a recursion.

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
4 :: func(x){x} == [0,1,2,3];

f = func(a,b){...};
[1,2,3,4] :: f == f(f(f(1,2),3),4);

```

# Stamp
A stamp can be instantiated via btml:

``` C
anystamp = <bcore_arr_st_s></>
```
Stamp members can be accessed via '.' operator.

# Built-in Operators
The following operators are hardwired in form of unary functions.

  * EXP
  * LOG
  * LOG2
  * LOG10
  * SIN
  * COS
  * TAN
  * TANH
  * SIGN
  * SQRT
  * ABS

# Built-in Constants
The following operators are hardwired constants.

  * TRUE
  * FALSE
  * true
  * false
  * PI

# Including other source files.

The keyword **embed** evaluates code from another file.

## Syntax
```
embed ( "path_to_another_file.txt" )
```
If the file path is relative it is taken relative to the folder in which
the current source is located.

The file is embedded in the current frame (no dedicated frame).
This allows defining variables (functions) in the embedded file to be used
outside the embedding.

# External Functions
BTCL can access stamp member functions by implementing the features 

``` C
feature sz_t btcl_function_arity( @* o, tp_t name ) = -1; // return -1 when function 'name' is not defined
feature er_t btcl_function(       @* o, tp_t name, bcore_arr_sr_s* args, m sr_s* result ); // must handle all names as indicated by btcl_function_arity
```
These define a set of functions. Each can be used in btcl like a member
function of the stamp that implements the features.

### Example: Definition (X)

``` C
name add_a;

stamp my_stamp
{
    f3_t additive = 0;
    func x_btcl.btcl_function_arity = ( name == add_a~ ) 1 : -1; // return -1 for all invalid names
    func x_btcl.btcl_function
    {
         switch( name )
         {
             case add_a~: sr.from_f3( args.[0].to_f3() + o.addditive ); break;
             default: = general_error~;
         }
         = 0;
    }
}
```

### Example: Usage (BTCL)

``` C
obj = <my_stamp/>( .additive = 10 );
? obj.add_a( 1 ) // outputs '11'
```
------
<sub>&copy; 2024 Johannes B. Steffens</sub>


