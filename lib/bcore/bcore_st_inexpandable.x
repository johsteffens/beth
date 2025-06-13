/** Author and Copyright 2020 Johannes Bernhard Steffens
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

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

group bcore_st = x_inst
{
    stamp st_s = aware x_inst
    {
        sd_t data;
        sc_t sc;
        uz_t size;
        uz_t space;

        func o set_size( m @* o, u0_t fill_char, uz_t size ); // allocates and fills with fill_char
        func o copyvf(   m @* o, sc_t format, va_list args );
        func o copyf(    m @* o, sc_t format, ...  );
        func o copy_fv(  m @* o, sc_t format, va_list args );
        func o copy_fa(  m @* o, sc_t format, ...  );
        func o copy_sc_n( m @* o, sc_t sc, uz_t n );
        func o copy_sc(   m @* o, sc_t sc );
        func er_t copy_typed( m @* o, tp_t type, vc_t src ); // copy with type conversion
        func o copy_aware( m @* o,            vc_t src ); // copy with type conversion

        func d @* createvf(     sc_t format, va_list args ); // c-style formatting
        func d @* createf(      sc_t format, ... );          // c-style formatting
        func d @* create_fv(    sc_t format, va_list args ); // beth-style formatting
        func d @* create_fa(    sc_t format, ... );          // beth-style formatting
        func d @* create_sc_n(  sc_t sc, uz_t n );
        func d @* create_sd_d(  sd_t sd );
        func d @* create_sc(    sc_t sc );
        func d @* create_weak_sc( sc_t sc );
        func d @* create_typed(  tp_t type, vc_t src );
        func d @* create_aware(             vc_t src );

        func bl_t is_weak( c @* o ); // A string is weak when size > 0 && space == 0
        func void make_strong( m @* o );
        func void set_min_space( m @* o, uz_t min_space ); // ensures o->space >= min_space; reallocates if necessary

        func d @* crop( c @* o, uz_t start, uz_t end ); // creates a new string with subsection of o;
        func d @* crop_d( m @* o, uz_t start, uz_t end ); // discards o;

        func m @* clear( m @* o ); // empties string

        func o push_char(   m @* o, char c );
        func o push_char_n( m @* o, char c, uz_t n ); // pushes c n times
        func char  pop_char(    m @* o );                 // removes last character and returns it
        func void  pop_n(       m @* o,         uz_t n ); // removes n last characters
        func o push_st(     m @* o, c st_s* src );
        func o push_st_d(   m @* o, d st_s* src ); // discards src
        func o push_sc_n(   m @* o, sc_t sc, uz_t n );
        func o push_sc(     m @* o, sc_t sc );
        func o push_typed(  m @* o, tp_t type, vc_t src ); // push with type conversion
        func o pushvf(      m @* o, sc_t format, va_list args );
        func o pushf(       m @* o, sc_t format, ... );
        func o push_fv(     m @* o, sc_t format, va_list args );
        func o push_fa(     m @* o, sc_t format, ... );

        /// comparison and equality (for return of comparison 'cmp' see bcore_strcmp)
        func s2_t cmp_sc(     c @* o, sc_t sc         );
        func s2_t cmp_sc_n(   c @* o, sc_t sc, uz_t n );
        func s2_t cmp_st(     c @* o, c st_s* st  );
        func s2_t cmp   (     c @* o, c st_s* st  ); // same as cmp_st
        func bl_t equal_sc(   c @* o, sc_t sc         );
        func bl_t equal_sc_n( c @* o, sc_t sc, uz_t n );
        func bl_t equal_st(   c @* o, c st_s* st  );
        func bl_t equal   (   c @* o, c st_s* st  ); // same as equal_st

        /// true in case the rightbound characters of o are equal to st
        func bl_t ends_in_st( c @* o, c st_s* st );
        func bl_t ends_in_sc( c @* o, sc_t sc );

        /** Search:
         *  Search involving positions between start to end in direction start --> end
         *  Detects first position nearest to start.
         *  'start' may be larger then 'end' in which case the search direction is backwards.
         *  The search range is [start, end-1] when start < end and [end, start-1] otherwise.
         *  if not found, the function returns the larger of (start, end) but maximally o->size.
         */
        func uz_t find_char(    c @* o, uz_t start, uz_t end, char c );
        func uz_t find_sc(      c @* o, uz_t start, uz_t end, sc_t sc );
        func uz_t find_st(      c @* o, uz_t start, uz_t end, c st_s* st );
        func uz_t find_st_d(    c @* o, uz_t start, uz_t end, d st_s* st );
        func uz_t find_any_sc(  c @* o, uz_t start, uz_t end, sc_t sc ); // any character in sc
        func uz_t find_none_sc( c @* o, uz_t start, uz_t end, sc_t sc ); // any character not in sc

        /// Counting matches (expects start >= end)
        func uz_t count_char( c @* o, uz_t start, uz_t end, char c );
        func uz_t count_sc(   c @* o, uz_t start, uz_t end, sc_t sc );
        func uz_t count_st(   c @* o, uz_t start, uz_t end, c st_s* st );

        /// Inserts at position <start>
        func o insert_char( m @* o, uz_t start, char c );
        func o insert_sc(   m @* o, uz_t start, sc_t sc );
        func o insert_st(   m @* o, uz_t start, c st_s* st );
        func o insert_st_d( m @* o, uz_t start, m st_s* st ); // discards string
        func o insert_fv(   m @* o, uz_t start, sc_t format, va_list args );
        func o insert_fa(   m @* o, uz_t start, sc_t format, ... );

        /// Replaces existing characters at position <start> (string length does not change)
        func o replace_char( m @* o, uz_t start, char c );
        func o replace_sc(   m @* o, uz_t start, sc_t sc );
        func o replace_st(   m @* o, uz_t start, c st_s* st );
        func o replace_st_d( m @* o, uz_t start, m st_s* st ); // discards string

        /// Removes from position start: <size> characters. If not enough characters left, the entire tail is removed. Returns o.
        func o remove( m @* o, uz_t start, uz_t size );

        /// Replaces all occurrences of <c> with string <replace>
        func o replace_char_char( m @* o, char c,   char  replace );
        func o replace_char_sc(   m @* o, char c,   sc_t  replace );
        func o replace_char_st(   m @* o, char c, c st_s* replace );
        func o replace_char_st_d( m @* o, char c, m st_s* replace );

        /// Replaces all occurrences of <match> with <replace>
        func o replace_sc_sc(     m @* o,    sc_t match,   sc_t replace );
        func o replace_st_st(     m @* o, c st_s* match, c st_s* replace );
        func o replace_st_d_st_d( m @* o, m st_s* match, m st_s* replace ); // discards input strings

        /// Replaces all characters within 'A' ... 'Z' or 'a' ... 'z' with corresponding lower/upper case characters
        func void set_lowercase( m @* o );
        func void set_uppercase( m @* o );

        /// Text navigation
        func uz_t lineof(    c @* o, uz_t pos );            // line number of character position in text (line counting starts with '1')
        func uz_t colof(     c @* o, uz_t pos );            // column of character position in text (column counting starts with '1')
        func uz_t posof(     c @* o, uz_t line, uz_t col ); // transforms line and column into a position
        func uz_t posofline( c @* o, uz_t pos );            // returns the position at which the line begins containing the character indicated by pos

        /// string to stdout
        func void to_stdout( c @* o );
        func void to_stdout_d( m @* o ); // discards o
        func void to_stdout_fv( sc_t format, va_list args );
        func void to_stdout_fa( sc_t format, ... );

        /// parse functions using error manager and returning error state
        func er_t parse_em_fv( c @* o, uz_t start, uz_t end, m uz_t* out_idx, sc_t format, va_list args );
        func er_t parse_em_fa( c @* o, uz_t start, uz_t end, m uz_t* out_idx, sc_t format, ... );

        func uz_t parse_fv( c @* o, uz_t start, uz_t end, sc_t format, va_list args );
        func uz_t parse_fa( c @* o, uz_t start, uz_t end, sc_t format, ... );

        /// constructs a double-line with a visible position indicator (useful for context messages/warnings/errors with highlighted position)
        func d @* show_line_context( c st_s* o, uz_t pos );
    }
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
