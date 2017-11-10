/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#ifndef BCORE_SC_H
#define BCORE_SC_H

/// Functions around to 0-terminated strings.

#include <stdarg.h>
#include "bcore_types.h"

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
 *      Supported types: u*_t, s*_t, f*_t, sz_t, bl_t.
 *      Example: "#<u3_t>"
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
 *      Argument: sz_t
 *      Example: "#tn{#<sc_t>}"
 *
 *  Padding:
 *    "#p<num><c>{<expression>}"
 *    "#pl<num><c>{<expression>}"
 *      Extends converted expression to <num> characters.
 *      Arguments: according to <expression>
 *      #pl...: left-padding, otherwise right-padding.
 *      Examples:
 *        "#pl10 {#<u3_t>}"       --> number converted to at least 10 chars left-bound number with leading spaces
 *        "#pl10 {#t10{#<sc_t>}}" --> string converted to exact 10 chars with left space-padding or right truncation
 *
 *    "#pn<c>{<expression>}"
 *    "#pln<c>{<expression>}"
 *      Like above #p rule except that size is given by argument.
 *      Argument: sz_t
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
 *      Argument: sz_t
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
sz_t sc_t_fnv( sd_t o, sz_t space, sc_t format, sz_t fsize, va_list* p_args );

/** Scans type; returns number of characters consumed; returns -1 in case of syntax error
 *  Syntax:
 *    - Elementary types may be expressed as is, e.g. sz_t.
 *    - Any type may be enclosed in <>
 *    - A pointer is specified via <...*> e.g. <u3_t*>
 *    - no whitespaces permitted
 */
s2_t sc_t_scan_type( sc_t format, tp_t* p_type, bl_t* p_is_ptr );

vd_t bcore_sc_signal( tp_t target, tp_t signal, vd_t object );

#endif //BCORE_SC_H
