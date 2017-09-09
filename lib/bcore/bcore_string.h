/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCORE_STRING_H
#define BCORE_STRING_H

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

typedef struct bcore_string_s
{
    aware_t _;
    union
    {
        sc_t sc;
        sd_t data;
    };
    sz_t size, space;
} bcore_string_s;

void            bcore_string_s_init(         bcore_string_s* o );
void            bcore_string_s_initvf(       bcore_string_s* o, sc_t format, va_list args );  // formatted initialization (like sprintf)
void            bcore_string_s_initf(        bcore_string_s* o, sc_t format, ... );           // formatted initialization (like sprintf)
void            bcore_string_s_init_sc(      bcore_string_s* o, sc_t sc );                    // creates string from sc
void            bcore_string_s_init_weak_sc( bcore_string_s* o, sc_t sc );                    // creates a weak string referencing sc
void            bcore_string_s_down(         bcore_string_s* o );
void            bcore_string_s_copy(         bcore_string_s* o, const bcore_string_s* src );
void            bcore_string_s_copyf(        bcore_string_s* o, sc_t format, ...  );
void            bcore_string_s_copy_sc(      bcore_string_s* o, sc_t sc );
void            bcore_string_s_assign_sc(    bcore_string_s* o, sc_t sc ); // assignment producing a weak string
void            bcore_string_s_copy_typed(   bcore_string_s* o, tp_t type, vc_t src ); // copy with type conversion
void            bcore_string_s_copy_aware(   bcore_string_s* o,            vc_t src ); // copy with type conversion
void            bcore_string_s_move(         bcore_string_s* o, bcore_string_s* src );
bcore_string_s* bcore_string_s_create();
bcore_string_s* bcore_string_s_createvf(     sc_t format, va_list args );
bcore_string_s* bcore_string_s_createf(      sc_t format, ... );
bcore_string_s* bcore_string_s_create_sc(    sc_t sc );
bcore_string_s* bcore_string_s_create_weak_sc( sc_t sc );
bcore_string_s* bcore_string_s_create_typed( tp_t type, vc_t src );
bcore_string_s* bcore_string_s_create_aware(            vc_t src );
bl_t            bcore_string_s_is_weak(       const bcore_string_s* o ); // A string is weak when size > 0 && space == 0
void            bcore_string_s_make_strong(         bcore_string_s* o );
void            bcore_string_s_set_min_space(       bcore_string_s* o, sz_t min_space ); // ensures o->space >= min_space; reallocates if necessary

/// create with lifetime manager
typedef struct bcore_life_s bcore_life_s;
bcore_string_s* bcore_string_s_create_l(     bcore_life_s* life );
bcore_string_s* bcore_string_s_createf_l(    bcore_life_s* life, sc_t format, ... );
bcore_string_s* bcore_string_s_create_l_sc(  bcore_life_s* life, sc_t sc );

void            bcore_string_s_discard(       bcore_string_s* o );
bcore_string_s* bcore_string_s_clone(   const bcore_string_s* o );
bcore_string_s* bcore_string_s_crop(    const bcore_string_s* o, sz_t start, sz_t end ); // creates a new string with subsection of o;
bcore_string_s* bcore_string_s_crop_d(        bcore_string_s* o, sz_t start, sz_t end ); // discards o;

bcore_string_s* bcore_string_s_clear( bcore_string_s* o ); // empties string

bcore_string_s* bcore_string_s_push_char(     bcore_string_s* o, char c );
bcore_string_s* bcore_string_s_push_char_n(   bcore_string_s* o, char c, sz_t n ); // pushes c n times
char            bcore_string_s_pop_char(      bcore_string_s* o );                 // removes last character and returns it
void            bcore_string_s_pop_n(         bcore_string_s* o,         sz_t n ); // removes n last characters
bcore_string_s* bcore_string_s_push_string(   bcore_string_s* o, const bcore_string_s* src );
bcore_string_s* bcore_string_s_push_string_d( bcore_string_s* o, bcore_string_s* src ); // discards src
bcore_string_s* bcore_string_s_push_sc(       bcore_string_s* o, sc_t sc );
bcore_string_s* bcore_string_s_push_typed(    bcore_string_s* o, tp_t type, vc_t src ); // push with type conversion
bcore_string_s* bcore_string_s_pushf(         bcore_string_s* o, sc_t format, ... );

/// comparison and equality (for return of comparison 'cmp' see bcore_strcmp)
static inline s2_t bcore_string_s_cmp_sc(       const bcore_string_s* o, sc_t sc ) { return bcore_strcmp( o ? o->sc : NULL, sc ); }
static inline s2_t bcore_string_s_cmp_string(   const bcore_string_s* o, const bcore_string_s* string ) { return bcore_strcmp( o ? o->sc : NULL, string ? string->sc : NULL ); }
static inline bool bcore_string_s_equal_sc(     const bcore_string_s* o, sc_t sc ) { return bcore_strcmp( o ? o->sc : NULL, sc ) == 0; }
static inline bool bcore_string_s_equal_string( const bcore_string_s* o, const bcore_string_s* string ) { return bcore_strcmp( o ? o->sc : NULL, string ? string->sc : NULL ) == 0; }

/** Search:
 *  Search involving positions between start to end in direction start --> end
 *  Detects first position nearest to start.
 *  'start' may be bigger then 'end' in which case the search direction is backwards.
 *  The search range is [start, end-1] when start < end and [end, start-1] otherwise.
 *  if not found, the function returns the bigger of (start, end) but maximally o->size.
 */
sz_t bcore_string_s_find_char(     const bcore_string_s* o, sz_t start, sz_t end, char c );
sz_t bcore_string_s_find_sc(       const bcore_string_s* o, sz_t start, sz_t end, sc_t sc );
sz_t bcore_string_s_find_string(   const bcore_string_s* o, sz_t start, sz_t end, const bcore_string_s* string );
sz_t bcore_string_s_find_string_d( const bcore_string_s* o, sz_t start, sz_t end,       bcore_string_s* string ); // discards string
sz_t bcore_string_s_find_any_sc(   const bcore_string_s* o, sz_t start, sz_t end, sc_t sc ); // any character in sc
sz_t bcore_string_s_find_none_sc(  const bcore_string_s* o, sz_t start, sz_t end, sc_t sc ); // any character not in sc

/// Counting (expects start >= end)
sz_t bcore_string_s_count_char(   const bcore_string_s* o, sz_t start, sz_t end, char c );
sz_t bcore_string_s_count_sc(     const bcore_string_s* o, sz_t start, sz_t end, sc_t sc );
sz_t bcore_string_s_count_string( const bcore_string_s* o, sz_t start, sz_t end, const bcore_string_s* string );

/// Inserts at position <start>
bcore_string_s* bcore_string_s_insert_char(     bcore_string_s* o, sz_t start, char c );
bcore_string_s* bcore_string_s_insert_sc(       bcore_string_s* o, sz_t start, sc_t sc );
bcore_string_s* bcore_string_s_insert_string(   bcore_string_s* o, sz_t start, const bcore_string_s* string );
bcore_string_s* bcore_string_s_insert_string_d( bcore_string_s* o, sz_t start, bcore_string_s* string ); // discards string

/// Removes from position start: <size> characters. If not enough characters left, the entire tail is removed. Returns o.
bcore_string_s* bcore_string_s_remove( bcore_string_s* o, sz_t start, sz_t size );

/// Replaces all occurrences of <c> with <string>
bcore_string_s* bcore_string_s_replace_char_sc(       bcore_string_s* o, char c, sc_t sc );
bcore_string_s* bcore_string_s_replace_char_string(   bcore_string_s* o, char c, const bcore_string_s* string );
bcore_string_s* bcore_string_s_replace_char_string_d( bcore_string_s* o, char c, bcore_string_s* string );

/// Replaces all occurrences of <match> with <replace>
bcore_string_s* bcore_string_s_replace_sc_sc(             bcore_string_s* o, sc_t match, sc_t replace );
bcore_string_s* bcore_string_s_replace_string_string(     bcore_string_s* o, const bcore_string_s* match, const bcore_string_s* replace );
bcore_string_s* bcore_string_s_replace_string_d_string_d( bcore_string_s* o, bcore_string_s* match, bcore_string_s* replace ); // discards input strings

/// Text navigation
sz_t bcore_string_s_lineof(    const bcore_string_s* o, sz_t pos );            // line number of character position in text (line counting starts with '1')
sz_t bcore_string_s_colof(     const bcore_string_s* o, sz_t pos );            // column of character position in text (column counting starts with '1')
sz_t bcore_string_s_posof(     const bcore_string_s* o, sz_t line, sz_t col ); // transforms line and column into a position
sz_t bcore_string_s_posofline( const bcore_string_s* o, sz_t pos );            // returns the position at which the line begins containing the character indicated by pos

/// string to stdout
void bcore_string_s_print( const bcore_string_s* o );
void bcore_string_s_print_d(     bcore_string_s* o ); // discards o

/// parsing

/** Functions supporting text-parsing with specific rules.
 *  Format rules
 *  "#name"   - scans a name-string consisting of valid name characters; argument: bcore_string_s*
 *  "#string" - scans a string enclosed in quotes '"'; Quotes inside the string are escaped '\"'; backslashes are escaped '\\'; argument: bcore_string_s*
 *  "#until'<char>'" - scans a string until <char> is reached; the character is not consumed; argument: bcore_string_s*
 *  "#?'...'" - test presence and consumes the string literal between '' if exactly matching; argument: bl_t*
 *  " "       - consumes whitespaces including c-style comments
 *  "#<type>" - matches content to <type>; argument: <type>*
 *              Example: #u3_t matches to u3_t and requires u3_t* as argument.
 *              supported types: u*_t, s*_t, f*_t, sz_t, bl_t
 *  Return:
 *     Index position after scan completes.
 *     This value would be be used as 'start' for continued scanning.
 */
sz_t bcore_string_s_parsevf( const bcore_string_s* o, sz_t start, sz_t end, sc_t format, va_list args );
sz_t bcore_string_s_parsef(  const bcore_string_s* o, sz_t start, sz_t end, sc_t format, ... );

/// constructs a double-line with a visible position indicator (useful for context messages/warnings/errors with highlighted position)
bcore_string_s* bcore_string_s_show_line_context( const bcore_string_s* o, sz_t pos );

vd_t bcore_string_signal( tp_t target, tp_t signal, vd_t object );

/**********************************************************************************************************************/
// testing, debugging

void bcore_string_s_quicktest( void );

#endif //BCORE_STRING_H
