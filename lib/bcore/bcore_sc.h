/** Copyright 2017 Johannes Bernhard Steffens
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

#ifndef BCORE_SC_H
#define BCORE_SC_H

/// Functions around to 0-terminated strings.

#include <stdarg.h>
#include "bcore_types.h"
#include "bcore_feature.h"

/** Formatted strings:
 *  Variadic functions with postfix _fv, _fa, _fnv
 *  Create strings or parse strings according to rules in a specified
 *  format-string.
 */

/**********************************************************************************************************************/

/** Creating formatted strings:
 *  The format strings contains characters and rules to produce a formated output.
 *  Characters that do not belong to a conversion rule are simply copied.
 *
 *  Rules containing {<expression>}
 *      Arguments: According to <expression>
 *      Expression may contain any content, including formating rules.
 *      Expression may contain {} character provided each opening bracket has a
 *      corresponding closing bracket.
 *      Alternatively, expression may be enclosed in other brackets of (), [], or <>.
 *      '{,}' provided
 *
 *  Format rules:
 *
 *  Type -> string conversion:
 *  "#<type>" or "#<type*>"
 *      Converts argument.
 *      Argument: <type> or pointer to <type>
 *      Supported types: u*_t, s*_t, f*_t, uz_t, bl_t.
 *      Example: "#<u3_t>"
 *
 *  "#<char>" or "#<char*>"
 *      Converts single character.
 *      Argument: char or char*
 *
 *  Truncation:
 *    "#t<num>{<expression>}"
 *    "#tl<num>{<expression>}"
 *      Limits converted expression to <num> characters.
 *      #tl...: left-truncation, otherwise right-truncation.
 *      Example: "#t10{#<sc_t>}"  (-->truncates string to 10 characters)
 *
 *    "#tn{<expression>}"
 *    "#tln{<expression>}"
 *      Like above #t rule except that limit is given by argument.
 *      Argument: uz_t
 *      Example: "#tn{#<sc_t>}"
 *
 *  Padding:
 *    "#p<num><c>{<expression>}"
 *    "#p<num>'<c>'{<expression>}"
 *    "#pl<num><c>{<expression>}"
 *    "#pl<num>'<c>'{<expression>}"
 *      Extends converted expression to <num> characters.
 *      Arguments: according to <expression>
 *      #pl...: left-padding, otherwise right-padding.
 *      Examples:
 *        "#pl10 {#<u3_t>}"       --> number converted to at least 10 chars left-bound number with leading spaces
 *        "#pl10'0'{#<u3_t>}"     --> number converted to at least 10 chars left-bound number with leading zeros (padding zero character enclosed in '' to avoid interpreting it as size)
 *        "#pl10 {#t10{#<sc_t>}}" --> string converted to exact 10 chars with left space-padding or right truncation
 *
 *    "#pn<c>{<expression>}"
 *    "#pln<c>{<expression>}"
 *      Like above #p rule except that size is given by argument.
 *      Argument: uz_t
 *      Example: "#pln10 {#<u3_t>}"
 *
 *  Repetition:
 *    "#r<num>{<expression>}"
 *      Repeats converted expression <num> times.
 *      Expression is evaluated exactly once, even when <num> is zero.
 *      Example: "#r80{_}" --> repeats character '_' 80 times.
 *
 *    "#rn{<expression>}"
 *      Like above #r rule except that size is given by argument.
 *      Argument: uz_t
 *      Example: "#rn{_}"
 */

/**********************************************************************************************************************/

/** Inserts space characters including terminating 0.
 *  Returns number of characters needed for string completion.
 *  If space is large enough, the return number is the length of the
 *  created string.
 *  Characters in format that do not belong to a format rule are simply
 *  copied to the destination string.
 */
uz_t sc_t_fnv( sd_t o, uz_t space, sc_t format, uz_t fsize, va_list* p_args );

/** Scans type; returns number of characters consumed; returns -1 in case of syntax error
 *  Syntax:
 *    - Elementary types may be expressed as is, e.g. uz_t.
 *    - Any type may be enclosed in <>
 *    - A pointer is specified via <...*> e.g. <u3_t*>
 *    - no whitespaces permitted
 */
s2_t sc_t_scan_type( sc_t format, tp_t* p_type, bl_t* p_is_ptr );

/// like stdlib strcpy but when dst == NULL, function allocates destination first, copies and returns address
sd_t sc_t_cpy( sd_t dst, sc_t src );

/** Compatible to stdlib strcmp but also accepts NULL and provides more distinctive results
 *  returns   0 when both strings are equal
 *  returns  +1 when src2 is longer than str 1 but at the length of str1 all characters match
 *  returns  -1 when src1 is longer than str 2 but at the length of str2 all characters match
 *  returns >+1 when none of above conditions apply and str2 comes after str1 in lexicographic order
 *  returns <-1 when none of above conditions apply and str1 comes after str2 in lexicographic order
 *  NULL is ordered before the empty string.
 *
 * Examples:
 *   if str1 is equal to str2 ...............: if( bcore_strcmp( str1, str2 ) == 0 )
 *   if str1 is ordered before str2 .........: if( bcore_strcmp( str1, str2 ) >  0 )
 *   if str2 is ordered before str1 .........: if( bcore_strcmp( str1, str2 ) <  0 )
 *   if str1 matches the beginning of str2 ..: if( (  bcore_strcmp( str1, str2 ) >> 1 ) == 0 )
 *   if str2 matches the beginning of str1 ..: if( ( -bcore_strcmp( str1, str2 ) >> 1 ) == 0 )
 *
 */
s2_t sc_t_cmp( sc_t str1, sc_t str2 );
s2_t sc_t_cmp_n( sc_t str1, uz_t n1, sc_t str2, uz_t n2 );

static inline bl_t sc_t_equ( sc_t str1, sc_t str2 ) { return sc_t_cmp( str1, str2 ) == 0; }
static inline bl_t sc_t_equ_n( sc_t str1, uz_t n1, sc_t str2, uz_t n2 ) { return sc_t_cmp_n( str1, n1, str2, n2 ) == 0; }

/// like stdlib strlen but also accepts NULL
uz_t sc_t_len( sc_t str );

/// returns true iff c occurs in str
bl_t sc_t_any( char c, sc_t str );

vd_t bcore_sc_signal_handler( const bcore_signal_s* o );

#endif //BCORE_SC_H
