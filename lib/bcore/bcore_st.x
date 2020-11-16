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

group bcore_st = bcore_inst
{
    stamp st = aware bcore_inst
    {
        sd_t data;
        sc_t sc;
        uz_t size;
        uz_t space;

        func( void  set_size( mutable, u0_t fill_char, uz_t size ) ); // allocates and fills with fill_char
        func( void  copyvf(   mutable, sc_t format, va_list args ) );
        func( void  copyf(    mutable, sc_t format, ...  ) );
        func( void  copy_fv(  mutable, sc_t format, va_list args ) );
        func( void  copy_fa(  mutable, sc_t format, ...  ) );
        func( void  copy_sc_n( mutable, sc_t sc, uz_t n ) );
        func( void  copy_sc(   mutable, sc_t sc ) );
        func( void  copy_typed( mutable, tp_t type, vc_t src ) ); // copy with type conversion
        func( void  copy_aware( mutable,            vc_t src ) ); // copy with type conversion

        func( @* createvf(     sc_t format, va_list args ) ); // c-style formatting
        func( @* createf(      sc_t format, ... ) );          // c-style formatting
        func( @* create_fv(    sc_t format, va_list args ) ); // beth-style formatting
        func( @* create_fa(    sc_t format, ... ) );          // beth-style formatting
        func( @* create_sc_n(  sc_t sc, uz_t n ) );
        func( @* create_sd_d(  sd_t sd ) );
        func( @* create_sc(    sc_t sc ) );
        func( @* create_weak_sc( sc_t sc ) );
        func( @* create_typed(  tp_t type, vc_t src ) );
        func( @* create_aware(             vc_t src ) );

        func( bl_t is_weak( const) ); // A string is weak when size > 0 && space == 0
        func( void make_strong( mutable ) );
        func( void set_min_space( mutable, uz_t min_space ) ); // ensures o->space >= min_space; reallocates if necessary

        func( @* crop( const, uz_t start, uz_t end ) ); // creates a new string with subsection of o;
        func( @* crop_d( mutable, uz_t start, uz_t end ) ); // discards o;

        func( @* clear( mutable ) ); // empties string

        func( st_s* push_char(   mutable, char c ) );
        func( st_s* push_char_n( mutable, char c, uz_t n ) ); // pushes c n times
        func( char  pop_char(    mutable ) );                 // removes last character and returns it
        func( void  pop_n(       mutable,         uz_t n ) ); // removes n last characters
        func( st_s* push_st(     mutable, const st_s* src ) );
        func( st_s* push_st_d(   mutable, st_s* src ) ); // discards src
        func( st_s* push_sc_n(   mutable, sc_t sc, uz_t n ) );
        func( st_s* push_sc(     mutable, sc_t sc ) );
        func( st_s* push_typed(  mutable, tp_t type, vc_t src ) ); // push with type conversion
        func( st_s* pushvf(      mutable, sc_t format, va_list args ) );
        func( st_s* pushf(       mutable, sc_t format, ... ) );
        func( st_s* push_fv(     mutable, sc_t format, va_list args ) );
        func( st_s* push_fa(     mutable, sc_t format, ... ) );

        /// comparison and equality (for return of comparison 'cmp' see bcore_strcmp)
        func( s2_t cmp_sc(     const, sc_t sc         ) );
        func( s2_t cmp_sc_n(   const, sc_t sc, uz_t n ) );
        func( s2_t cmp_st(     const, const st_s* st  ) );
        func( bl_t equal_sc(   const, sc_t sc         ) );
        func( bl_t equal_sc_n( const, sc_t sc, uz_t n ) );
        func( bl_t equal_st(   const, const st_s* st  ) );

        /** Search:
         *  Search involving positions between start to end in direction start --> end
         *  Detects first position nearest to start.
         *  'start' may be larger then 'end' in which case the search direction is backwards.
         *  The search range is [start, end-1] when start < end and [end, start-1] otherwise.
         *  if not found, the function returns the larger of (start, end) but maximally o->size.
         */
        func( uz_t find_char(    const, uz_t start, uz_t end, char c ) );
        func( uz_t find_sc(      const, uz_t start, uz_t end, sc_t sc ) );
        func( uz_t find_st(      const, uz_t start, uz_t end, const st_s* st ) );
        func( uz_t find_st_d(    const, uz_t start, uz_t end,       st_s* st ) ); // discards string
        func( uz_t find_any_sc(  const, uz_t start, uz_t end, sc_t sc ) ); // any character in sc
        func( uz_t find_none_sc( const, uz_t start, uz_t end, sc_t sc ) ); // any character not in sc

        /// Counting matches (expects start >= end)
        func( uz_t count_char( const, uz_t start, uz_t end, char c ) );
        func( uz_t count_sc(   const, uz_t start, uz_t end, sc_t sc ) );
        func( uz_t count_st(   const, uz_t start, uz_t end, const st_s* st ) );

        /// Inserts at position <start>
        func( @* insert_char( mutable, uz_t start, char c ) );
        func( @* insert_sc(   mutable, uz_t start, sc_t sc ) );
        func( @* insert_st(   mutable, uz_t start, const st_s* st ) );
        func( @* insert_st_d( mutable, uz_t start, st_s* st ) ); // discards string
        func( @* insert_fv(   mutable, uz_t start, sc_t format, va_list args ) );
        func( @* insert_fa(   mutable, uz_t start, sc_t format, ... ) );

        /// Replaces existing characters at position <start> (string length does not change)
        func( @* replace_char( mutable, uz_t start, char c ) );
        func( @* replace_sc(   mutable, uz_t start, sc_t sc ) );
        func( @* replace_st(   mutable, uz_t start, const st_s* st ) );
        func( @* replace_st_d( mutable, uz_t start, st_s* st ) ); // discards string

        /// Removes from position start: <size> characters. If not enough characters left, the entire tail is removed. Returns o.
        func( @* remove( mutable, uz_t start, uz_t size ) );

        /// Replaces all occurrences of <c> with string <replace>
        func( @* replace_char_char( mutable, char c,       char replace ) );
        func( @* replace_char_sc(   mutable, char c,       sc_t replace ) );
        func( @* replace_char_st(   mutable, char c, const st_s* replace ) );
        func( @* replace_char_st_d( mutable, char c,       st_s* replace ) );

        /// Replaces all occurrences of <match> with <replace>
        func( @* replace_sc_sc(     mutable,        sc_t match,       sc_t replace ) );
        func( @* replace_st_st(     mutable, const st_s* match, const st_s* replace ) );
        func( @* replace_st_d_st_d( mutable,       st_s* match,       st_s* replace ) ); // discards input strings

        /// Replaces all characters within 'A' ... 'Z' or 'a' ... 'z' with corresponding lower/upper case characters
        func( void set_lowercase( mutable ) );
        func( void set_uppercase( mutable ) );

        /// Text navigation
        func( uz_t lineof(    const, uz_t pos ) );            // line number of character position in text (line counting starts with '1')
        func( uz_t colof(     const, uz_t pos ) );            // column of character position in text (column counting starts with '1')
        func( uz_t posof(     const, uz_t line, uz_t col ) ); // transforms line and column into a position
        func( uz_t posofline( const, uz_t pos ) );            // returns the position at which the line begins containing the character indicated by pos

        /// string to stdout
        func( void to_stdout( const ) );
        func( void to_stdout_d( mutable ) ); // discards o
        func( void to_stdout_fv( sc_t format, va_list args ) );
        func( void to_stdout_fa( sc_t format, ... ) );

        /// parse functions using error manager and returning error state
        func( er_t parse_em_fv( const, uz_t start, uz_t end, uz_t* out_idx, sc_t format, va_list args ) );
        func( er_t parse_em_fa( const, uz_t start, uz_t end, uz_t* out_idx, sc_t format, ... ) );

        /// constructs a double-line with a visible position indicator (useful for context messages/warnings/errors with highlighted position)
        func( @* show_line_context( const st_s* o, uz_t pos ) );
    };

};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
