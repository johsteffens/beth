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

/**

BTCL: Beth text constructive language (interpreter).

Constructive Language (Definition):
Language for text based object construction with advanced syntax and semantics. It is easy to use with
syntax-elements common to many programming languages. It is not intended for basic serialization of objects
like a meth-markup language. Yet, a markup language can be a valid subset of a constructive language.

A constructive language can be ranked in-between a markup language and a programming language.

Syntax:

Expression:
    An expression constructs an object via literal (number or string); btml expression or algebraic expression.
    Syntax: <expression>
    Example: <bmath_cf3_s> 1.0 0 </>
    Example: 2 * 7 + x.[0] // operators are evaluated with proper priority
    Example: x.r = <expr>     // expression with assignment x or member r has been declared as variable; expression yields <expr>

    Modified clone:
    Syntax: <expr>( .<path> = <expr1>, .<path> = <expr2>, ... ) // clones <expr> and modifies the specified elements; the result is const;
    Syntax: <expr>( <expr1>, <expr2>, ... ) // expects <expr> to be an array; clones it an sets elements according to list

Statement:
    A statement can be an assignment, variable declaration or function definition.
    A statement is concluded by a semicolon.
    Syntax: <statement>;
    Example: var x = <bmath_cf3_s> r: 1.0 i: 0 </>;

Variable:
    Definition syntax: <identifier> = <expression>;
    Example: x = <expression>; // variable with assignment

Block syntax:
    Variables and functions declared in a block are local to the block.
{
    <statement>;
    <statement>;
}

Function definition syntax

my_func = func( a, b )
{
    <body>
    = <expression>;
}

Function brief syntax
my_func = func( a, b ) = <expression>;

The function body contains a list of statements. The last statement is a return-assignment.

Binary operators <bop> are used in binary operations of the form <lexpr> <bop> <rexpr>:

.   : member object: <rexpr> must evaluate to a string, identifier or index-number
Example: x.loudness_mask --> (x is stamp bcodec_audio_codec_waw_param_s) bcodec_audio_codec_waw_param_s.loudness_mask
Example: x.[2]  --> (x s an array) element 2 of x
Example: x.2    --> same as x.[2]
(Example: x.y    --> y is deemed variable of number or string)

^   : numeric exponentiation (power function)
/   : numeric division
*   : numeric multiplication
-   : numeric subtraction
+   : numeric addition;
Example: 1 + 2 --> 3

+   : string catenation: one operand must be a string and the other convertible to a string
Example: "ab" + "x" --> "abx"
Example: "ab" + 3   --> "ab3"
Example: "ab" + x   --> (if x = 5) "ab5"

:   : generic listing (pair generation, list extension, no tree), Result is :list_s; we denote the content of :list_s as L(..,..,..)
Example: 1:2 --> L(1:2)
Example: 1:<L(2:3)> --> L(1:2:3)

=   : assignment: modifies object of <lexpr> by copying the object of <rexpr>; if lxepr is an unknown label, a variable is created

;   : continuation: result is <rexpr>
Example: x=5; x+3  --> 7
Note: A continuation at the end of file or block is no error.

*/

#ifndef BCORE_X_BTCL_H
#define BCORE_X_BTCL_H

#include "bcore_sr.h"
#include "bcore.xo.h"
#include "bcore_hmap_name.h"
#include "bcore_x_source.h"
#include "bcore_x_sink.h"
#include "bcore_x_btml.h"

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( x_btcl, x_inst )

#ifdef XOILA_SECTION

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

embed "bcore_x_btcl.x";

#endif // XOILA_SECTION

#endif  // BCORE_X_BTCL_H
