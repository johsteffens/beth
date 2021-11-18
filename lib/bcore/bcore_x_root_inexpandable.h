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

/** Beth inexpandable group.
 *  This group does not generate effective plant-code but is otherwise accessible within the plant framework.
 *  It manages components, which have a low-level implementation: E.g. beth-system features.
 */

#ifndef BCORE_X_ROOT_INEXPANDABLE_H
#define BCORE_X_ROOT_INEXPANDABLE_H

#include "bcore_std.h"
#include "bcore.xo.h"

/**********************************************************************************************************************/

#ifdef TYPEOF_bcore_x_root
XOILA_DEFINE_GROUP( bcore_x_root, x_inst )

#ifdef XOILA_SECTION

set inexpandable;

// External types and identifiers

// primitive types
type u0_t;
type u1_t;
type u2_t;
type u3_t;
type s0_t;
type s1_t;
type s2_t;
type s3_t;
type f2_t;
type f3_t;
type uz_t;
type sz_t;
type tp_t;
type er_t;
type aware_t;
type sc_t;
type sd_t;
type bl_t;
type vd_t;
type vc_t;
type char;
type void;
type int;
type clock_t;

// objects
type bcore_self_s;
type bcore_self_item_s;

type va_list;

identifier NULL;
identifier __DATE__;
identifier __TIME__;
identifier ERR_fa;
identifier WRN_fa;
identifier ASSERT;
identifier ifnameof;
identifier btypeof;
identifier bentypeof;
identifier bnameof;
identifier assert;
identifier tanh;
identifier sizeof;
identifier const;
identifier ABS_TIME_OF;
identifier CLOCKS_PER_SEC;
identifier clock;
identifier sc_t_cmp;
identifier bcore_run_signal_selftest;

identifier bcore_self_item_s_parse_src;
identifier bcore_self_s_parse_source;
identifier bcore_flect_caps_is_array;

identifier bcore_name_push_all;

identifier BCORE_CAPS_EXTERNAL_FUNC;

identifier bcore_error_push_fa;
identifier bcore_error_push_sc;
identifier bcore_error_pop_st;
identifier bcore_error_pop_to_sink;
identifier bcore_error_stack_size;
identifier bcore_error_pop_all_to_stderr;

identifier bcore_folder_get_current;

identifier sz_max;
identifier sz_min;

identifier sc_t_equal;

identifier va_start;
identifier va_end;
identifier bcore_push_traits_of_ancestor;
identifier bcore_msg_fa;

identifier bcore_tp_init;
identifier bcore_tp_fold_tp;
identifier bcore_tp_fold_bl;
identifier bcore_tp_fold_sc;
identifier bcore_tp_fold_sz;
identifier bcore_tp_fold_u0;
identifier bcore_tp_fold_u1;
identifier bcore_tp_fold_u2;
identifier bcore_tp_fold_u3;

identifier bcore_lcg00_u3;

identifier bcore_err_fv;
identifier bcore_strlen;

identifier bcore_bin_ml_a_copy;

identifier bcore_cday_utc_s_from_system;
identifier bcore_cday_utc_s_to_sink;

identifier bcore_life_s_push_aware;

identifier f3_srt;
identifier f3_abs;
identifier f3_max;
identifier f3_min;

//----------------------------------------------------------------------------------------------------------------------

group bcore_inst  = bcore_inst {};
group bcore_array = bcore_inst {};
group bcore_spect = bcore_inst {};

//----------------------------------------------------------------------------------------------------------------------

/// default functions in a stamp
group bcore_stamp_funcs = bcore_inst
{
    signature void init( m @* o );
    signature void down( m @* o );
    signature void copy( m @* o, c @* src );

    signature   d @* create();
    signature void discard( d @* o );
    signature   d @* clone( c @* o );
};

//----------------------------------------------------------------------------------------------------------------------

group bcore_fp = bcore_inst
{
    feature '' void copy_typed( m @* o, tp_t type, vc_t src );
};

//----------------------------------------------------------------------------------------------------------------------

/// bcore_mutex_s
group bcore_mutex = bcore_inst
{
    signature void lock(   m @* o );
    signature void unlock( m @* o );

    signature d bcore_lock_s*   create_lock( m @* o );
    signature d bcore_unlock_s* create_unlock( m @* o );

    stamp :s = obliv bcore_inst
    {
        func : .lock;
        func : .unlock;
        func : .create_lock;
        func : .create_unlock;
    };
};

//----------------------------------------------------------------------------------------------------------------------

/// bcore_lock_s
group bcore_lock = bcore_inst
{
    /** Sets lock on given mutex;
     *  The lock is released on function release or when lock is shut down.
     */
    signature void set( m @* o, mutable bcore_mutex_s* mutex );
    signature void release( m @* o );

    stamp :s = aware bcore_inst
    {
        private bcore_mutex_s* mutex;
        func :.set;
        func :.release;
    };
};

//----------------------------------------------------------------------------------------------------------------------

/// bcore_unlock_s
group bcore_unlock = bcore_inst
{
    /** Sets lock on given mutex;
     *  The lock is released on function release or when lock is shut down.
     */
    signature void set( m @* o, mutable bcore_mutex_s* mutex );
    signature void release( m @* o );

    stamp :s = aware bcore_inst
    {
        private bcore_mutex_s* mutex;
        func :.set;
        func :.release;
    };
};

//----------------------------------------------------------------------------------------------------------------------

/// bcore_condition_s
group bcore_condition = bcore_inst
{
    signature void sleep   ( m @* o, m bcore_mutex_s* mutex );
    signature void wake_one( m @* o );
    signature void wake_all( m @* o );

    stamp :s = obliv bcore_inst
    {
        func : .sleep;
        func : .wake_one;
        func : .wake_all;
    };
};

//----------------------------------------------------------------------------------------------------------------------

/// bcore_thread_s
identifier bcore_fp_thread;
group bcore_thread = bcore_inst
{
    signature void call( m @* o, bcore_fp_thread func, vd_t arg );
    signature vd_t join( m @* o );

    stamp :s = obliv bcore_inst
    {
        func : .call;
        func : .join;
    };
};

//----------------------------------------------------------------------------------------------------------------------

/// bcore_img
group bcore_img = bcore_inst
{
    stamp :u2_s = obliv x_array
    {
        tp_t format; // pixel color space
        uz_t rows;
        uz_t cols;
        uz_t stride;
        u2_t [];

        /** Allocates image and initializes pixels with 0
         *  if o->format == 0: o->format = TYPEOF_bcore_img_u2_argb
         */
        func (void set_size( m @* o, uz_t rows, uz_t cols ));

        // pnm format
        func (void pnm_to_sink(       @* o, bcore_sink* dst ));
        func (void pnm_to_file(       @* o, sc_t file ));
        func (void pnm_from_source( m @* o, bcore_source* src ));
        func (void pnm_from_file(   m @* o, sc_t file ));

        // pixel access
        func (void set_pixel( m @* o, uz_t row, uz_t col, u2_t v ));
        func (u2_t get_pixel(   @* o, uz_t row, uz_t col ));
        func (void set_rgb( m @* o, uz_t row, uz_t col, u0_t  r, u0_t  g, u0_t  b ));
        func (void get_rgb(   @* o, uz_t row, uz_t col, u0_t* r, u0_t* g, u0_t* b ));
    };


};


//----------------------------------------------------------------------------------------------------------------------

embed "bcore_source_inexpandable.x";
embed "bcore_sink_inexpandable.x";
embed "bcore_st_inexpandable.x";
embed "bcore_arr_inexpandable.x";
embed "bcore_hmap_inexpandable.x";
embed "bcore_flect_inexpandable.x";
embed "bcore_via_inexpandable.x";
embed "bcore_sr_inexpandable.x";

#endif // XOILA_SECTION

#endif // TYPEOF_bcore_x_root

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( bcore_x_root_expandable, bcore_inst )

#ifdef XOILA_SECTION

name bcore_self_item_s;

//----------------------------------------------------------------------------------------------------------------------

#endif // XOILA_SECTION

/**********************************************************************************************************************/

#endif  // BCORE_X_ROOT_INEXPANDABLE_H
