# BTCL - Beth Text Constructive Language

# What it is

BTCL is a simple and easy to use text based (functional) language
to construct an object.

## Constructive Language
I use the term ***constructive language*** to specify a language specialized for
object construction. It can be placed somewhere between mere markup and general
purpose programming. It offers variables, operators and functions but lacks
interactivity. It represents a state rather than a process.

BTCL is by nature a weakly typed functional language. 

Specifically:

  * Contiguous code represents one expression (built from expressions or literals). There are no statements.
  * An expression represents the state of an object.
  * Functions are first class objects.

# Cheat Sheet

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
```

# One Expression
BTCL has no distinct statements. Any contiguous code represents just one expression.
This can be a composite (or tree) of expressions joined via functions, 
operators or conditional branches.
Non-composite expressions are called ***Literals***.

## Continuation operator
Although the semicolon ';' is traditionally understood as separator for
consecutive statements, in BTCL it is a binary operator combining two 
consecutive expressions. Hence the semicolon behaves as a [binary operator](#list-of-operators).
I call this operator **Continuation**.

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

# Operators

Each operator has a unique priority. On chained operations, higher priority 
operators are evaluated before lower priority operators. Equal operators are
evaluated in the chained order (e.g. ```3 - 1 - 1 == 2```). 
Additionally operators are grouped into priority-groups. Each group is
associated with a letter A ... E. A higher letter means lower priority.

The lists below are sorted in descending priority.

## Group A - Binary

|Symbol|Description|
|:---|:---|
|.|Stamp member access|
|(|Function call or stamp modifier; closed by ')'|

## Group B - Unary

|Symbol|Description|
|:---|:---|
|+|Identity|
|-|Arithmetic Negation|
|!|Logic Negation|
|?|Identity: Object is printed to stdout. Leaf objects are printed in compact form.|
|??|Identity: Object is printed in detail to stdout.|

## Group C - Binary 

|Symbol|Description|
|:---|:---|
|.|Stamp member access|
|(|Function call or stamp modifier; closed by ')'|
|^|**Arithmetic**: exponentiation; result type is f3_t|
|/|**Arithmetic**: division|
|%|**Arithmetic**: modulo division|
|*|**Arithmetic**: multiplication|
|-|**Arithmetic**: subtraction|
|+|**Arithmetic**: addition; Concatenation of strings.|
|::|**List**: [List construction](#list-construction)|
|:|**List**: Joining objects to form a list; Concatenation of lists|
|==|**Logic**: equal|
|!=|**Logic**: unequal|
|>=|**Logic**: larger or equal|
|> |**Logic**: larger|
|<=|**Logic**: smaller or equal|
|< |**Logic**: smaller|
|& |**Logic**: AND|
|\||**Logic**: OR|
|=|Assignment|
|;|Continuation|

## Group D (Reserved)

## Group E - Binary

|Symbol|Description|
|:---|:---|
|;|Continuation|

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

Preferably initiate a recursion by calling ```self```. Avoid recursion by calling the
function via a variable from its lexical frame.

# Stamp
A stamp can be instantiated via btml:

``` C
anystamp = <bcore_arr_st_s></>
```
Stamp members can be access via '.' operator.

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

## List Construction

Operation ```a::b``` can construct and modify a list.

|a-type|b-type|Description|
|:---|:---|:---|
|number|unary function|List of a elements, each set to ```b(index)```
|number|any other|List of a elements, each set to value b
|list|unary function|List of a.SIZE() elements, each set to ```b(a.[index])```

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

------
<sub>&copy; 2024 Johannes B. Steffens</sub>


