/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#ifndef BCORE_ST_H
#define BCORE_ST_H

/** Managed string of characters.
 *  The character array always terminates with '0'.
 *  'size' specifies the number of characters in the string (excluding the terminating '0')
 *  'space' specifies the amount of memory managed by the string instance.
 *  If space > 0, the string instance owns and manages all memory of the string; in that case it is space >= size + 1.
 *  If space == 0, the string references an external (constant) 0-terminated string. (--> weak string)
 *  Functions that cause modification of the string data, turn a weak string into a strong one.
 *  sc should be used with functions expecting a 0-terminated string.
 *  sc always represents a valid 0-terminated string. In case of the empty string, sc points to to an external constant "";
 *
 *  Strings support perspectives bcore_spect_source and bcore_spect_sink.
 *  Strings used as bcore_spect_source must be weak. (Strong strings produce a runtime error)
 *
 *  Strings do not support perspective bcore_spect_array.
 */

#include <stdarg.h>

#include "bcore_first.h"
#include "bcore_types.h"
#include "bcore_control.h"

typedef struct st_s
{
    aware_t _;
    union
    {
        sc_t sc;
        sd_t data;
    };
    sz_t size, space;
} st_s;

void  st_s_init(         st_s* o );
void  st_s_initvf(       st_s* o, sc_t format, va_list args );  // formatted initialization (like sprintf)
void  st_s_initf(        st_s* o, sc_t format, ... );           // formatted initialization (like sprintf)
void  st_s_init_fv(      st_s* o, sc_t format, va_list args );  // formatted initialization
void  st_s_init_fa(      st_s* o, sc_t format, ... );           // formatted initialization
void  st_s_init_sc_n(    st_s* o, sc_t sc, sz_t n );            // creates string from first n characters of sc
void  st_s_init_sc(      st_s* o, sc_t sc );                    // creates string from sc
void  st_s_init_weak_sc( st_s* o, sc_t sc );                    // creates a weak string referencing sc
void  st_s_down(         st_s* o );
void  st_s_copy(         st_s* o, const st_s* src );
void  st_s_copyvf(       st_s* o, sc_t format, va_list args  );
void  st_s_copyf(        st_s* o, sc_t format, ...  );
void  st_s_copy_fv(      st_s* o, sc_t format, va_list args  );
void  st_s_copy_fa(      st_s* o, sc_t format, ...  );
void  st_s_copy_sc_n(    st_s* o, sc_t sc, sz_t n );
void  st_s_copy_sc(      st_s* o, sc_t sc );
void  st_s_assign_sc(    st_s* o, sc_t sc ); // assignment producing a weak string
void  st_s_copy_typed(   st_s* o, tp_t type, vc_t src ); // copy with type conversion
void  st_s_copy_aware(   st_s* o,            vc_t src ); // copy with type conversion
void  st_s_move(         st_s* o, st_s* src );
st_s* st_s_create();
st_s* st_s_createvf(     sc_t format, va_list args );
st_s* st_s_createf(      sc_t format, ... );
st_s* st_s_create_fv(    sc_t format, va_list args );
st_s* st_s_create_fa(    sc_t format, ... );
st_s* st_s_create_sc_n(  sc_t sc, sz_t n );
st_s* st_s_create_sc(    sc_t sc );
st_s* st_s_create_weak_sc( sc_t sc );
st_s* st_s_create_typed( tp_t type, vc_t src );
st_s* st_s_create_aware(            vc_t src );
bl_t  st_s_is_weak(       const st_s* o ); // A string is weak when size > 0 && space == 0
void  st_s_make_strong(         st_s* o );
void  st_s_set_min_space(       st_s* o, sz_t min_space ); // ensures o->space >= min_space; reallocates if necessary

/// create with lifetime manager
typedef struct bcore_life_s bcore_life_s;
st_s* st_s_create_l(      bcore_life_s* life );
st_s* st_s_createf_l(     bcore_life_s* life, sc_t format, ... );
st_s* st_s_create_l_sc_n( bcore_life_s* life, sc_t sc, sz_t n );
st_s* st_s_create_l_sc(   bcore_life_s* life, sc_t sc );

void  st_s_discard(       st_s* o );
st_s* st_s_clone(   const st_s* o );
st_s* st_s_crop(    const st_s* o, sz_t start, sz_t end ); // creates a new string with subsection of o;
st_s* st_s_crop_d(        st_s* o, sz_t start, sz_t end ); // discards o;

st_s* st_s_clear( st_s* o ); // empties string

st_s* st_s_push_char(   st_s* o, char c );
st_s* st_s_push_char_n( st_s* o, char c, sz_t n ); // pushes c n times
char  st_s_pop_char(    st_s* o );                 // removes last character and returns it
void  st_s_pop_n(       st_s* o,         sz_t n ); // removes n last characters
st_s* st_s_push_st(     st_s* o, const st_s* src );
st_s* st_s_push_st_d(   st_s* o, st_s* src ); // discards src
st_s* st_s_push_sc_n(   st_s* o, sc_t sc, sz_t n );
st_s* st_s_push_sc(     st_s* o, sc_t sc );
st_s* st_s_push_typed(  st_s* o, tp_t type, vc_t src ); // push with type conversion
st_s* st_s_pushvf(      st_s* o, sc_t format, va_list args );
st_s* st_s_pushf(       st_s* o, sc_t format, ... );
st_s* st_s_push_fv(     st_s* o, sc_t format, va_list args );
st_s* st_s_push_fa(     st_s* o, sc_t format, ... );

/// comparison and equality (for return of comparison 'cmp' see bcore_strcmp)
static inline s2_t st_s_cmp_sc(     const st_s* o, sc_t sc         ) { return bcore_strcmp( o ? o->sc : NULL, sc ); }
static inline s2_t st_s_cmp_sc_n(   const st_s* o, sc_t sc, sz_t n ) { return bcore_strcmp_n( o ? o->sc : NULL, o ? o->size : 0, sc, n ); }
static inline s2_t st_s_cmp_st(     const st_s* o, const st_s* st  ) { return bcore_strcmp( o ? o->sc : NULL, st ? st->sc : NULL ); }
static inline bool st_s_equal_sc(   const st_s* o, sc_t sc         ) { return bcore_strcmp( o ? o->sc : NULL, sc ) == 0; }
static inline bool st_s_equal_sc_n( const st_s* o, sc_t sc, sz_t n ) { return bcore_strcmp_n( o ? o->sc : NULL, o ? o->size : 0, sc, n ) == 0; }
static inline bool st_s_equal_st(   const st_s* o, const st_s* st  ) { return bcore_strcmp( o ? o->sc : NULL, st ? st->sc : NULL ) == 0; }

/** Search:
 *  Search involving positions between start to end in direction start --> end
 *  Detects first position nearest to start.
 *  'start' may be bigger then 'end' in which case the search direction is backwards.
 *  The search range is [start, end-1] when start < end and [end, start-1] otherwise.
 *  if not found, the function returns the bigger of (start, end) but maximally o->size.
 */
sz_t st_s_find_char(    const st_s* o, sz_t start, sz_t end, char c );
sz_t st_s_find_sc(      const st_s* o, sz_t start, sz_t end, sc_t sc );
sz_t st_s_find_st(      const st_s* o, sz_t start, sz_t end, const st_s* st );
sz_t st_s_find_st_d(    const st_s* o, sz_t start, sz_t end,       st_s* st ); // discards string
sz_t st_s_find_any_sc(  const st_s* o, sz_t start, sz_t end, sc_t sc ); // any character in sc
sz_t st_s_find_none_sc( const st_s* o, sz_t start, sz_t end, sc_t sc ); // any character not in sc

/// Counting (expects start >= end)
sz_t st_s_count_char( const st_s* o, sz_t start, sz_t end, char c );
sz_t st_s_count_sc(   const st_s* o, sz_t start, sz_t end, sc_t sc );
sz_t st_s_count_st(   const st_s* o, sz_t start, sz_t end, const st_s* st );

/// Inserts at position <start>
st_s* st_s_insert_char( st_s* o, sz_t start, char c );
st_s* st_s_insert_sc(   st_s* o, sz_t start, sc_t sc );
st_s* st_s_insert_st(   st_s* o, sz_t start, const st_s* st );
st_s* st_s_insert_st_d( st_s* o, sz_t start, st_s* st ); // discards string

/// Replaces existing characters at position <start> (string length does not change)
st_s* st_s_replace_char( st_s* o, sz_t start, char c );
st_s* st_s_replace_sc(   st_s* o, sz_t start, sc_t sc );
st_s* st_s_replace_st(   st_s* o, sz_t start, const st_s* st );
st_s* st_s_replace_st_d( st_s* o, sz_t start, st_s* st ); // discards string

/// Removes from position start: <size> characters. If not enough characters left, the entire tail is removed. Returns o.
st_s* st_s_remove( st_s* o, sz_t start, sz_t size );

/// Replaces all occurrences of <c> with <st>
st_s* st_s_replace_char_sc(   st_s* o, char c, sc_t sc );
st_s* st_s_replace_char_st(   st_s* o, char c, const st_s* st );
st_s* st_s_replace_char_st_d( st_s* o, char c, st_s* st );

/// Replaces all occurrences of <match> with <replace>
st_s* st_s_replace_sc_sc(     st_s* o, sc_t match, sc_t replace );
st_s* st_s_replace_st_st(     st_s* o, const st_s* match, const st_s* replace );
st_s* st_s_replace_st_d_st_d( st_s* o, st_s* match, st_s* replace ); // discards input strings

/// Text navigation
sz_t st_s_lineof(    const st_s* o, sz_t pos );            // line number of character position in text (line counting starts with '1')
sz_t st_s_colof(     const st_s* o, sz_t pos );            // column of character position in text (column counting starts with '1')
sz_t st_s_posof(     const st_s* o, sz_t line, sz_t col ); // transforms line and column into a position
sz_t st_s_posofline( const st_s* o, sz_t pos );            // returns the position at which the line begins containing the character indicated by pos

/// string to stdout
void st_s_print( const st_s* o );
void st_s_print_d(     st_s* o ); // discards o

/** Parsing strings:
 *  The format string contains characters and rules to parse an input string.
 *  Characters that do not belong to a conversion rule are simply consumed as is.
 *  A space ' ' in the format string consumes all whitespaces {' ', '\t', '\n', '\r', c-style comments}
 *
 *  Format rules
 *  "#name"
 *      Argument: st_s*
 *      Consumes a name-string consisting of valid name characters.
 *      A valid first character: letter  | '_'.
 *      A valid non-first character: <valid first character> | digit.
 *
 *  "#string"
 *      Argument: st_s*
 *      Consumes a string in o enclosed in quotes '"'.
 *      Quotes inside the string are to be escaped '\"'.
 *      Backslashes are to be escaped '\\'.
 *
 *  "#until'<char>'"  (Example: "#until'\n'")
 *      Consumes a string until <char> is reached.
 *      The character is not consumed.
 *
 *  "#?'...'"  (Example: "#?'hidden'")
 *      Argument: bl_t*
 *      Consumes the string literal ... if exactly matching.
 *      Consumes nothing if not exactly matching.
 *      Sets argument 'true' on exact match, false otherwise.
 *      Enclosure ' should be preferred. It can be replaced by suitable character
 *      from "|*.-+=/ in case the enclosure occurs in the in the literal.
 *
 *  "#?(...)"
 *      Argument: bl_t*
 *      Evaluates a logical expression (...) on string characters ahead.
 *      Consumes no characters.
 *      Valid relations: ==,!=,>=,<=,>,<
 *      Valid operators: &&,||,!
 *      Expression may not contain whitespaces.
 *      Expressions can be arbitrarily nested inside extra brackets (...).
 *      Examples:
 *          "#?([0]=='H'&&[1]=='A'&&[2]=='L')" behaves as "#=?'HAL'"
 *          Test for a digit:  "#?([0]>='0'&&[0]<='9')"
 *          Test for a letter: "#?(([0]>='A'&&[0]<='Z')&&([0]>='a'&&[0]<='z'))"
 *
 *  "#?!..."
 *      Argument: bl_t*
 *      Performs a boolean evaluation #?... and inverts the result.
 *
 *  "#<type>"  (Example: "#u3_t" or "#<u3_t*>")
 *      Argument: <type>*
 *      Matches content to <type>.
 *      Example: #u3_t matches to u3_t and requires u3_t* as argument.
 *      Supported types: u*_t, s*_t, f*_t, sz_t, bl_t.
 *
 *  "#-..."  (Example: "#-<u3_t>" )
 *      Argument: none
 *      For any above rule: If '-' is inserted after '#', the rule applies
 *      only to consume characters. No argument in the argument list is
 *      accessed or consumed.
 *
 *  "#=..."  (Example: "#=?'{'" )
 *      Argument: according to rule
 *      For any above rule: If '=' is inserted after '#', the rule applies
 *      without consuming characters. Arguments are served and consumed as intended.
 *
 *  Return:
 *     Index position after scan completes.
 *     This value would be be used as 'start' for continued scanning.
 */
sz_t st_s_parse_fv( const st_s* o, sz_t start, sz_t end, sc_t format, va_list args );
sz_t st_s_parse_fa( const st_s* o, sz_t start, sz_t end, sc_t format, ... );

/// constructs a double-line with a visible position indicator (useful for context messages/warnings/errors with highlighted position)
st_s* st_s_show_line_context( const st_s* o, sz_t pos );

vd_t bcore_st_signal( tp_t target, tp_t signal, vd_t object );

#endif //BCORE_ST_H
