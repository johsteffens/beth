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

group bcore_arr = x_inst
{

signature m @* clear    ( m @* o ); // sets size to zero
signature m @* set_space( m @* o, uz_t space );
signature m @* set_size ( m @* o, uz_t size  ); // resize
signature s2_t cmp      (   @* o, @* b );
signature m @* sort     ( m @* o, s2_t order ); // stable
signature m @* reorder  ( m @* o, c bcore_arr_uz_s* order );
signature m @* push_arr ( m @* o, c @* arr ); // appends array

//----------------------------------------------------------------------------------------------------------------------

group :uz = :
{
    signature m @* fill       ( m @* o, uz_t size, uz_t v ); // creates filled array of size <size>
    signature m @* step_fill  ( m @* o, uz_t v_start, s3_t step, uz_t size ); // creates filled array according to stepping
    signature void push       ( m @* o, uz_t v );
    signature m @* push_left  ( m @* o, uz_t v ); // extends array by one; moves all elements up one index and copies v to position 0
    signature uz_t pop        ( m @* o );
    signature uz_t find       ( c @* o,   uz_t start, uz_t end, uz_t v ); // behaves like st_s_find_*
    signature uz_t max        ( c @* o );
    signature uz_t min        ( c @* o );
    signature uz_t idx_max    ( c @* o );
    signature uz_t idx_min    ( c @* o );
    signature uz_t count_equal( c @* o,   uz_t val ); // number of occurrence

    stamp :s = aware x_inst
    {
        uz_t [];
        func :: .clear;
        func :: .set_space;
        func :: .set_size;
        func :: .sort;
        func :: .reorder;
        func  : .fill;
        func  : .step_fill;
        func  : .push;
        func  : .push_left;
        func :: .push_arr;
        func  : .pop;
        func :: .cmp;
        func  : .find;
        func  : .max;
        func  : .min;
        func  : .idx_max;
        func  : .idx_min;
        func  : .count_equal;
    };
};

//----------------------------------------------------------------------------------------------------------------------

group :sz = :
{
    signature m @* fill       ( m @* o, uz_t size, sz_t v ); // creates filled array of size <size>
    signature m @* step_fill  ( m @* o, sz_t v_start, s3_t step, uz_t size ); // creates filled array according to stepping
    signature void push       ( m @* o, sz_t v );
    signature m @* push_left  ( m @* o, sz_t v ); // extends array by one; moves all elements up one index and copies v to position 0
    signature sz_t pop        ( m @* o );
    signature uz_t find       ( c @* o,   uz_t start, uz_t end, sz_t v ); // behaves like st_s_find_*
    signature sz_t max        ( c @* o );
    signature sz_t min        ( c @* o );
    signature uz_t idx_max    ( c @* o );
    signature uz_t idx_min    ( c @* o );
    signature uz_t count_equal( c @* o,   sz_t val ); // number of occurrence

    stamp :s = aware x_inst
    {
        sz_t [];
        func :: .clear;
        func :: .set_space;
        func :: .set_size;
        func :: .sort;
        func :: .reorder;
        func  : .fill;
        func  : .step_fill;
        func  : .push;
        func  : .push_left;
        func :: .push_arr;
        func  : .pop;
        func :: .cmp;
        func  : .find;
        func  : .max;
        func  : .min;
        func  : .idx_max;
        func  : .idx_min;
        func  : .count_equal;
    };
};

//----------------------------------------------------------------------------------------------------------------------

group :u3 = :
{
    signature m @* fill       ( m @* o, uz_t size, u3_t v ); // creates filled array of size <size>
    signature m @* step_fill  ( m @* o, u3_t v_start, s3_t step, uz_t size ); // creates filled array according to stepping
    signature void push       ( m @* o, u3_t v );
    signature m @* push_left  ( m @* o, u3_t v ); // extends array by one; moves all elements up one index and copies v to position 0
    signature u3_t pop        ( m @* o );
    signature uz_t count_equal( c @* o, u3_t val ); // number of occurrence

    stamp :s = aware x_inst
    {
        u3_t [];
        func :: .clear;
        func :: .set_space;
        func :: .set_size;
        func :: .sort;
        func :: .reorder;
        func  : .fill;
        func  : .step_fill;
        func  : .push;
        func  : .push_left;
        func :: .push_arr;
        func  : .pop;
        func :: .cmp;
        func  : .count_equal;
    };
};

//----------------------------------------------------------------------------------------------------------------------

group :s3 = :
{
    signature m @* fill       ( m @* o, uz_t size, s3_t v ); // creates filled array of size <size>
    signature m @* step_fill  ( m @* o, s3_t v_start, s3_t step, uz_t size ); // creates filled array according to stepping
    signature void push       ( m @* o, s3_t v );
    signature m @* push_left  ( m @* o, s3_t v ); // extends array by one; moves all elements up one index and copies v to position 0
    signature s3_t pop        ( m @* o );
    signature uz_t count_equal( c @* o, s3_t val ); // number of occurrence
    signature uz_t find       ( c @* o,   uz_t start, uz_t end, s3_t v ); // behaves like st_s_find_*
    signature s3_t max        ( c @* o );
    signature s3_t min        ( c @* o );
    signature uz_t idx_max    ( c @* o );
    signature uz_t idx_min    ( c @* o );

    stamp :s = aware x_inst
    {
        s3_t [];
        func :: .clear;
        func :: .set_space;
        func :: .set_size;
        func :: .sort;
        func :: .reorder;
        func  : .fill;
        func  : .step_fill;
        func  : .push;
        func  : .push_left;
        func :: .push_arr;
        func  : .pop;
        func :: .cmp;
        func  : .count_equal;
        func  : .find;
        func  : .max;
        func  : .min;
        func  : .idx_max;
        func  : .idx_min;
    };
};

//----------------------------------------------------------------------------------------------------------------------

group :tp = :
{
    signature m @* fill       ( m @* o, uz_t size, tp_t v ); // creates filled array of size <size>
    signature m @* step_fill  ( m @* o, tp_t v_start, s3_t step, uz_t size ); // creates filled array according to stepping
    signature void push       ( m @* o, tp_t v );
    signature m @* push_left  ( m @* o, tp_t v ); // extends array by one; moves all elements up one index and copies v to position 0
    signature tp_t pop        ( m @* o );
    signature uz_t find       ( c @* o, uz_t start, uz_t end, tp_t v ); // behaves like st_s_find_*
    signature bl_t exists     ( c @* o, tp_t v );
    signature uz_t count_equal( c @* o, tp_t val ); // number of occurrence

    stamp :s = aware x_inst
    {
        tp_t [];
        func :: .clear;
        func :: .set_space;
        func :: .set_size;
        func :: .sort;
        func :: .reorder;
        func  : .fill;
        func  : .step_fill;
        func  : .push;
        func  : .push_left;
        func :: .push_arr;
        func  : .pop;
        func :: .cmp;
        func  : .exists;
        func  : .count_equal;
    };
};

//----------------------------------------------------------------------------------------------------------------------

group :bl = :
{
    signature m @* fill       ( m @* o, uz_t size, bl_t v ); // creates filled array of size <size>
    signature void push       ( m @* o, bl_t v );
    signature m @* push_left  ( m @* o, bl_t v ); // extends array by one; moves all elements up one index and copies v to position 0
    signature bl_t pop        ( m @* o );
    signature uz_t count_equal( c @* o,   bl_t val ); // number of occurrence

    stamp :s = aware x_inst
    {
        bl_t [];
        func :: .clear;
        func :: .set_space;
        func :: .set_size;
        func  : .fill;
        func  : .push;
        func  : .push_left;
        func :: .push_arr;
        func  : .pop;
        func :: .cmp;
        func  : .count_equal;
    };
};

//----------------------------------------------------------------------------------------------------------------------

group :st = :
{
    signature m st_s* push     ( m @* o );
    signature m st_s* push_st  ( m @* o, c st_s* st );
    signature m st_s* push_st_d( m @* o, m st_s* st );
    signature m st_s* push_sc  ( m @* o,   sc_t  sc );
    signature m st_s* push_fv  ( m @* o, sc_t format, va_list args );
    signature m st_s* push_fa  ( m @* o, sc_t format, ... );
    signature void pop        ( m @* o );
    signature uz_t count_equal( c @* o, c st_s* val ); // number of occurrence

    /** Outputs to sink the arrays content in the format of a table.
     *  cols or rows can be set to -1, in which case the missing value is estimated.
     *  rows is truncated in case rows * cols exceeds array size.
     *  Example:
     *    arr.push_fa( "Name" );   arr.push_fa( " Number" );
     *    arr.push_fa( "ab" );     arr.push_fa( " 123" );
     *    arr.push_fa( "abcdef" ); arr.push_fa( " 12" );
     *    arr.push_fa( "abc" );    arr.push_fa( " 1" );
     *    arr.table_to_sink( 4, -1, BCORE_STDOUT )
     *  Output:
     *    Name   Number
     *    ab     123
     *    abcdef 12
     *    abc    1
     */
    signature void table_to_sink( c @* o, sz_t rows, sz_t cols, m bcore_sink* sink );

    stamp :s = aware x_inst
    {
        st_s => [];
        func :: .clear;
        func :: .set_space;
        func :: .set_size;
        func :: .sort;
        func :: .reorder;
        func  : .push;
        func  : .push_st;
        func  : .push_st_d;
        func  : .push_sc;
        func  : .push_fv;
        func  : .push_fa;
        func  : .pop;
        func  : .count_equal;
        func  : .table_to_sink;
    };
};

//----------------------------------------------------------------------------------------------------------------------

group :vd = :
{
    signature m @* fill       ( m @* o, uz_t size, vd_t v ); // creates filled array of size <size>
    signature void push       ( m @* o, vd_t v );
    signature vd_t pop        ( m @* o );
    signature uz_t find       ( c @* o, uz_t start, uz_t end, vd_t v ); // behaves like st_s_find_*
    signature uz_t count_equal( c @* o, vd_t val ); // number of occurrence

    stamp :s = aware x_inst
    {
        vd_t [];
        func :: .clear;
        func :: .set_space;
        func :: .set_size;
        func :: .reorder;
        func  : .fill;
        func  : .push;
        func  : .pop;
        func  : .find;
        func  : .count_equal;
    };
};

//----------------------------------------------------------------------------------------------------------------------

group :fp = :
{
    signature m @* fill       ( m @* o, uz_t size, fp_t v ); // creates filled array of size <size>
    signature void push       ( m @* o, fp_t v );
    signature fp_t pop        ( m @* o );
    signature uz_t find       ( c @* o, uz_t start, uz_t end, fp_t v ); // behaves like st_s_find_*
    signature uz_t count_equal( c @* o, fp_t val ); // number of occurrence

    stamp :s = aware x_inst
    {
        fp_t [];
        func :: .clear;
        func :: .set_space;
        func :: .set_size;
        func :: .reorder;
        func  : .fill;
        func  : .push;
        func  : .pop;
        func  : .find;
        func  : .count_equal;
    };
};

//----------------------------------------------------------------------------------------------------------------------

group :sr = :
{
    signature m sr_s* push_sr( m @* o, sr_s v );
    signature m sr_s* push_tp( m @* o, tp_t type );
    signature sr_s pop      ( m @* o );
    signature m @* set_spect( m @* o, tp_t spect_type ); // switches perspective of all references to 'spect'

    stamp :s = aware x_inst
    {
        sr_s [];
        func :: .clear;
        func :: .set_space;
        func :: .set_size;
        func :: .reorder;
        func  : .push_sr;
        func  : .push_tp;
        func  : .pop;
        func  : .set_spect;
    };
};

//----------------------------------------------------------------------------------------------------------------------

};
