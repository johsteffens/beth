# BTCL - Beth Text Constructive Language

# What it is

## BTCL
BTCL is a simple and easy to use text based (functional) language
to construct an object.

## Constructive Language
I use the term ***constructive language*** to specify a language specialized for
object construction. It can be placed somewhere between mere markup and general
purpose programming. It offers variables, operators and functions but lacks
interactivity. It represents a state rather than a process.

BTCL is primarily a functional language.

# Cheat Sheet

``` C
// string
"some text";

// Variable definition and representation of its value '7'
x = 7;

// Number Literal, here '14'
1 + 2 * 3 + x;

// function definition (functions are first class citizens)
f = func( a, b ) { a + b };

// using above function (expression evaluates to '7')
n = f( 3, 4 ); 

// bmath object representation via btml
y = <bcore_arr_s3_s> 1 2 3 </>;

// same result as above (using btcl array initialization)
z = <bcore_arr_s3_s></>( 1 : 1+1 : x-4 );

// btcl element initialization
<bcore_main_frame_s></>( .create_log_file = TRUE .log_file_extension = "log" );

// prints object z to stdout (for messaging, inspection, debugging)
? z;  // compact formatting where possible
?? z; // always btml format

// condition (else-part is optional)
if( a >= b ) { a } else { b };

```

# One Expression, No Statements, Semicolon Operator
BTCL has no distinct statements. Any contiguous code represents just one expression.
This is typically a composite (or tree) of expressions joined via operators. 
Expressions that can not be decomposed are called ***Literals***.


Although the semicolon ';' is traditionally understood as separator for
consecutive statements, in BTCL it is a binary operator combining two 
consecutive expressions.

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

# Operator

## Unary (Prefix)
Unary prefix operators have priority over binary operators.

|Symbol|Description|
|:---|:---|
|+|Identity|
|-|Negation|
|?|Identity: Object is printed to stdout. Leaf objects are printed in compact form.|
|??|Identity: Object is printed in detail to stdout.|
|(|Prioritized Object: closed by ')': Enclosed expression represents object|

## Binary
Binary Operators are executed by priority. 
They are executed in left to right order when of same priority.

**List of operators by descending priority:**

|Symbol|Description|
|:---|:---|
|.|Stamp member access|
|(|Prioritized expression; closed by ')' Enclosed expression is prioritized|
|/|**Arithmetic**: division|
|*|**Arithmetic**: multiplication|
|-|**Arithmetic**: subtraction|
|+|**Arithmetic**: addition; Concatenation of strings.|
|==|**Logic**: equal|
|!=|**Logic**: unequal|
|>=|**Logic**: larger or equal|
|> |**Logic**: larger|
|<=|**Logic**: smaller or equal|
|< |**Logic**: smaller|
|& |**Logic**: AND|
|\||**Logic**: OR|
|:|**List**: Joining objects to form a list; Concatenation of lists|
|=|Assignment|
|;|Continuation|

# Number
Integers are sorted as ```s3_t```.
Floats are stored as ```f3_t```.

Integer literal in decimal:

```1023578```

... in hexadecimal via prefix ```0x```:

```0x378FCD50```

Flotaing point literals are specified by either using a decimal point 
(even if the resulting number is integer)

```123.0```

Or by using exponential notation:

```123E1```

## Arithmetic
Binary arithmetic operators return f3_t when one of the operands is f3_t,
otherwise they return s3_t.

# String
Sting literals are specified using the C-Syntax:

``` C
"This is a string"
"\tThis is a string with tab and newline\n"
```

## Operators
Operator '+' concatenates string with strings and string with numbers by
converting the number to a string

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

# Conditional Expression

## Syntax:

``` C
if (<condition>) { <expression> }
if (<condition>) { <expression1> } else { <expression2> }
```

Depending on the condition the associated block is either evaluated or skipped.

# Function, Signature, Block

Functions, signatures and blocks are all first class objects.
A function is defined by joining a signature with a block.

## Signature syntax

``` C
func( <args, comma separated> )
```

## Block syntax

``` C
{
	<expression>
}
```

## Function syntax

``` C
<signature> : <block>

or

<literal-signature> <literal-block>
```

### Example
``` C
f = func( a, b ) { a + b }; // function definition; ':' can be omitted
f( 1, 2 )                   // function usage; result is 3
```

### Example
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

Preferably use ```self``` with recursions. Avoid recursion by calling the
function via a variable from its lexical frame.


------
<sub>&copy; 2024 Johannes B. Steffens</sub>


