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

group bcore_arr = bcore_inst
{

signature @* clear    ( mutable ); // sets size to zero
signature @* set_space( mutable, uz_t space );
signature @* set_size ( mutable, uz_t size  ); // resize
signature @* sort     ( mutable, s2_t order ); // stable
signature @* reorder  ( mutable, const bcore_arr_uz_s* order );

//----------------------------------------------------------------------------------------------------------------------

group :uz = :
{
    signature @*   fill       ( mutable, uz_t size, uz_t v ); // creates filled array of size <size>
    signature @*   step_fill  ( mutable, uz_t v_start, s3_t step, uz_t size ); // creates filled array according to stepping
    signature void push       ( mutable, uz_t v );
    signature @*   push_left  ( mutable, uz_t v ); // extends array by one; moves all elements up one index and copies v to position 0
    signature uz_t pop        ( mutable );
    signature uz_t find       ( const,   uz_t start, uz_t end, uz_t v ); // behaves like st_s_find_*
    signature uz_t max        ( const );
    signature uz_t min        ( const );
    signature uz_t idx_max    ( const );
    signature uz_t idx_min    ( const );
    signature uz_t count_equal( const,   uz_t val ); // number of occurrence

    stamp : = aware bcore_inst
    {
        uz_t [];
        func :: :clear;
        func :: :set_sapce;
        func :: :set_size;
        func :: :sort;
        func :: :reorder;
        func  : :fill;
        func  : :step_fill;
        func  : :push;
        func  : :push_left;
        func  : :pop;
        func  : :find;
        func  : :max;
        func  : :min;
        func  : :idx_max;
        func  : :idx_min;
        func  : :count_equal;
    };
};

//----------------------------------------------------------------------------------------------------------------------

group :sz = :
{
    signature @*   fill       ( mutable, uz_t size, sz_t v ); // creates filled array of size <size>
    signature @*   step_fill  ( mutable, sz_t v_start, s3_t step, uz_t size ); // creates filled array according to stepping
    signature void push       ( mutable, sz_t v );
    signature @*   push_left  ( mutable, sz_t v ); // extends array by one; moves all elements up one index and copies v to position 0
    signature sz_t pop        ( mutable );
    signature uz_t find       ( const,   uz_t start, uz_t end, sz_t v ); // behaves like st_s_find_*
    signature sz_t max        ( const );
    signature sz_t min        ( const );
    signature uz_t idx_max    ( const );
    signature uz_t idx_min    ( const );
    signature uz_t count_equal( const,   sz_t val ); // number of occurrence

    stamp : = aware bcore_inst
    {
        sz_t [];
        func :: :clear;
        func :: :set_sapce;
        func :: :set_size;
        func :: :sort;
        func :: :reorder;
        func  : :fill;
        func  : :step_fill;
        func  : :push;
        func  : :push_left;
        func  : :pop;
        func  : :find;
        func  : :max;
        func  : :min;
        func  : :idx_max;
        func  : :idx_min;
        func  : :count_equal;
    };
};

//----------------------------------------------------------------------------------------------------------------------

group :u3 = :
{
    signature @*   fill       ( mutable, uz_t size, u3_t v ); // creates filled array of size <size>
    signature @*   step_fill  ( mutable, u3_t v_start, s3_t step, uz_t size ); // creates filled array according to stepping
    signature void push       ( mutable, u3_t v );
    signature @*   push_left  ( mutable, u3_t v ); // extends array by one; moves all elements up one index and copies v to position 0
    signature u3_t pop        ( mutable );
    signature uz_t count_equal( const,   u3_t val ); // number of occurrence

    stamp : = aware bcore_inst
    {
        u3_t [];
        func :: :clear;
        func :: :set_sapce;
        func :: :set_size;
        func :: :sort;
        func :: :reorder;
        func  : :fill;
        func  : :step_fill;
        func  : :push;
        func  : :push_left;
        func  : :pop;
        func  : :count_equal;
    };
};

//----------------------------------------------------------------------------------------------------------------------

group :tp = :
{
    signature @*   fill       ( mutable, uz_t size, tp_t v ); // creates filled array of size <size>
    signature @*   step_fill  ( mutable, tp_t v_start, s3_t step, uz_t size ); // creates filled array according to stepping
    signature void push       ( mutable, tp_t v );
    signature @*   push_left  ( mutable, tp_t v ); // extends array by one; moves all elements up one index and copies v to position 0
    signature tp_t pop        ( mutable );
    signature uz_t count_equal( const,   tp_t val ); // number of occurrence

    stamp : = aware bcore_inst
    {
        tp_t [];
        func :: :clear;
        func :: :set_sapce;
        func :: :set_size;
        func :: :sort;
        func :: :reorder;
        func  : :fill;
        func  : :step_fill;
        func  : :push;
        func  : :push_left;
        func  : :pop;
        func  : :count_equal;
    };
};

//----------------------------------------------------------------------------------------------------------------------

group :bl = :
{
    signature @*   fill       ( mutable, uz_t size, bl_t v ); // creates filled array of size <size>
    signature void push       ( mutable, bl_t v );
    signature @*   push_left  ( mutable, bl_t v ); // extends array by one; moves all elements up one index and copies v to position 0
    signature bl_t pop        ( mutable );
    signature uz_t count_equal( const,   bl_t val ); // number of occurrence

    stamp : = aware bcore_inst
    {
        bl_t [];
        func :: :clear;
        func :: :set_sapce;
        func :: :set_size;
        func  : :fill;
        func  : :push;
        func  : :push_left;
        func  : :pop;
        func  : :count_equal;
    };
};

//----------------------------------------------------------------------------------------------------------------------

group :st = :
{
    signature st_s* push_st    ( mutable, const st_s* st );
    signature st_s* push_st_d  ( mutable, st_s* st );
    signature st_s* push_sc    ( mutable, sc_t sc );
    signature void  pop        ( mutable );
    signature uz_t  count_equal( const,   const st_s* val ); // number of occurrence

    stamp : = aware bcore_inst
    {
        st_s => [];
        func :: :clear;
        func :: :set_sapce;
        func :: :set_size;
        func :: :sort;
        func :: :reorder;
        func  : :push_st;
        func  : :push_st_d;
        func  : :push_sc;
        func  : :pop;
        func  : :count_equal;
    };
};

//----------------------------------------------------------------------------------------------------------------------

group :vd = :
{
    signature @*   fill       ( mutable, uz_t size, vd_t v ); // creates filled array of size <size>
    signature void push       ( mutable, vd_t v );
    signature vd_t pop        ( mutable );
    signature uz_t find       ( const,   uz_t start, uz_t end, vd_t v ); // behaves like st_s_find_*
    signature uz_t count_equal( const,   vd_t val ); // number of occurrence

    stamp : = aware bcore_inst
    {
        vd_t [];
        func :: :clear;
        func :: :set_sapce;
        func :: :set_size;
        func :: :reorder;
        func  : :fill;
        func  : :push;
        func  : :pop;
        func  : :find;
        func  : :count_equal;
    };
};

//----------------------------------------------------------------------------------------------------------------------

group :fp = :
{
    signature @*   fill       ( mutable, uz_t size, fp_t v ); // creates filled array of size <size>
    signature void push       ( mutable, fp_t v );
    signature fp_t pop        ( mutable );
    signature uz_t find       ( const,   uz_t start, uz_t end, fp_t v ); // behaves like st_s_find_*
    signature uz_t count_equal( const,   fp_t val ); // number of occurrence

    stamp : = aware bcore_inst
    {
        fp_t [];
        func :: :clear;
        func :: :set_sapce;
        func :: :set_size;
        func :: :reorder;
        func  : :fill;
        func  : :push;
        func  : :pop;
        func  : :find;
        func  : :count_equal;
    };
};

//----------------------------------------------------------------------------------------------------------------------

group :sr = :
{
    signature sr_s* push_sr  ( mutable, sr_s v );
    signature sr_s* push_tp  ( mutable, tp_t type );
    signature sr_s  pop      ( mutable );
    signature @*    set_spect( mutable, tp_t spect_type ); // switches perspective of all references to 'spect'

    stamp : = aware bcore_inst
    {
        sr_s [];
        func :: :clear;
        func :: :set_sapce;
        func :: :set_size;
        func :: :reorder;
        func  : :push_sr;
        func  : :push_tp;
        func  : :pop;
        func  : :set_spect;
    };
};

//----------------------------------------------------------------------------------------------------------------------

};
