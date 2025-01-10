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
type u0_t, u1_t, u2_t, u3_t;
type s0_t, s1_t, s2_t, s3_t;
type f2_t, f3_t;
type uz_t, sz_t;
type bl_t;
type tp_t, aware_t, er_t, sc_t, sd_t;
type vd_t, vc_t;
type fp_t;
type char, void, int;
type clock_t;

// objects
type bcore_self_s;
type bcore_self_item_s;

type va_list;

identifier system;
identifier NULL;
identifier __DATE__;
identifier __TIME__;
identifier ERR_fa;
identifier WRN_fa;
identifier ASSERT;
identifier assert;
identifier tanh;
identifier sizeof;
identifier const;
identifier ABS_TIME_OF;
identifier CLOCKS_PER_SEC;
identifier clock;
identifier sc_t_cmp;
identifier bcore_run_signal_selftest;

identifier bcore_type_is_leaf;

identifier bcore_self_item_s_parse_src;
identifier bcore_self_s_parse_source;

identifier bcore_flect_caps_e_sc;
identifier bcore_flect_caps_e_u2;
identifier bcore_flect_caps_is_array;
identifier bcore_flect_caps_is_array_dyn;
identifier bcore_flect_caps_is_array_fix;
identifier bcore_flect_caps_is_array_of_links;
identifier bcore_flect_caps_is_aware;
identifier bcore_flect_caps_is_typed;
identifier bcore_flect_caps_get_indirection;

identifier bcore_name_push_all;

identifier BCORE_CAPS_EXTERNAL_FUNC;
identifier BCORE_CAPS_SOLID_STATIC;
identifier BCORE_CAPS_LINK_STATIC;
identifier BCORE_CAPS_LINK_TYPED;
identifier BCORE_CAPS_LINK_AWARE;
identifier BCORE_CAPS_POINTER;
identifier BCORE_CAPS_ARRAY_DYN_SOLID_STATIC;
identifier BCORE_CAPS_ARRAY_DYN_SOLID_TYPED;
identifier BCORE_CAPS_ARRAY_DYN_LINK_STATIC;
identifier BCORE_CAPS_ARRAY_DYN_LINK_TYPED;
identifier BCORE_CAPS_ARRAY_DYN_LINK_AWARE;
identifier BCORE_CAPS_ARRAY_FIX_SOLID_STATIC;
identifier BCORE_CAPS_ARRAY_FIX_LINK_STATIC;
identifier BCORE_CAPS_ARRAY_FIX_LINK_AWARE;
identifier BCORE_CAPS_EXTERNAL_FUNC;

identifier bcore_error_push_fa;
identifier bcore_error_push_sc;
identifier bcore_error_pop_st;
identifier bcore_error_pop_to_sink;
identifier bcore_error_stack_size;
identifier bcore_error_pop_all_to_stderr;

identifier bcore_get_time;
identifier bcore_time_us; /// returns (full) time in microseconds
identifier bcore_time_ms; /// returns (full) time in milliseconds

identifier bcore_folder_get_current;

identifier sz_max, sz_min, sz_t_swap, sz_gcd, sz_sig, sz_abs, sz_sqr;
identifier uz_max, uz_min, uz_t_swap, uz_gcd;
identifier s3_max, s3_min, s3_t_swap, s3_sig, s3_abs, s3_sqr;
identifier s2_max, s2_min, s2_t_swap, s2_sig, s2_abs, s2_sqr;
identifier s1_max, s1_min, s1_t_swap, s1_sig, s1_abs, s1_sqr;
identifier u3_max, u3_min, u3_t_swap;
identifier u2_max, u2_min, u2_t_swap;
identifier u1_max, u1_min, u1_t_swap;

identifier f3_ident, f3_neg, f3_max, f3_min, f3_t_swap, f3_sig, f3_abs, f3_sqr, f3_srt, f3_inv, f3_rs2, f3_rs3;
identifier f2_ident, f2_neg, f2_max, f2_min, f2_t_swap, f2_sig, f2_abs, f2_sqr, f2_srt, f2_inv, f2_rs2, f2_rs3;


identifier sin, cos, tan, tanh, pow, lrint, atan2, frexp;

identifier f3_pi;
identifier f3_pi_2; // PI / 2
identifier f3_tau;  // 2 * PI
identifier f3_srt2; // squareroot( 2 )
identifier f3_is_nan; // nan compares unequal to itself
identifier f3_exp;
identifier f3_log;
identifier f3_log2;
identifier f3_log10;
identifier f3_pow;
identifier f3_erf;
identifier f3_sin;
identifier f3_cos;
identifier f3_tan;
identifier f3_tanh;
identifier f3_ferm; // fermi function (2ferm(2x)-1 == tanh(x))
identifier f3_sign; // sign function
identifier f3_ustp; // unit step function (heavyside function)
identifier f3_sqrt;
identifier f3_ceil;
identifier f3_floor;

identifier f3_lim_min; // minimum normalized representable positive number
identifier f3_lim_max; // maximum normalized representable positive number
identifier f3_lim_eps; // minimum representable positive difference to 1.0
identifier f3_lim_inf; // floating point representation of infinity

identifier f2_pi;
identifier f2_pi_2; // PI / 2
identifier f2_tau; // 2 * PI
identifier f2_srt2; // squareroot( 2 )
identifier f2_is_nan; // nan compares unequal to itself
identifier f2_exp;
identifier f2_log;
identifier f2_log2;
identifier f2_log10;
identifier f2_pow;
identifier f2_erf;
identifier f2_sin;
identifier f2_cos;
identifier f2_tan;
identifier f2_tanh;
identifier f2_ferm; // fermi function (2ferm(2x)-1 == tanh(x))
identifier f2_sign; // sign function
identifier f2_ustp; // unit step function (heavyside function)
identifier f2_sqrt;
identifier f2_ceil;
identifier f2_floor;

identifier f2_lim_min; // minimum normalized representable positive number
identifier f2_lim_max; // maximum normalized representable positive number
identifier f2_lim_eps; // minimum representable positive difference to 1.0
identifier f2_lim_inf; // floating point representation of infinity

identifier tp_t_swap;
identifier f3_t_swap;
identifier f2_t_swap;
identifier vd_t_swap;
identifier vc_t_swap;

identifier sc_t_equal;

identifier va_start;
identifier va_end;
identifier bcore_push_traits_of_ancestor;

identifier bcore_msgv;
identifier bcore_msg;
identifier bcore_msg_fv;
identifier bcore_msg_fa;
identifier bcore_err_fa;
identifier bcore_wrn_fa;

identifier bcore_memset;
identifier bcore_memcpy;

identifier bcore_tp_init;
identifier bcore_tp_fold_tp;
identifier bcore_tp_fold_bl;
identifier bcore_tp_fold_sc;
identifier bcore_tp_fold_sz;
identifier bcore_tp_fold_u0;
identifier bcore_tp_fold_u1;
identifier bcore_tp_fold_u2;
identifier bcore_tp_fold_u3;
identifier bcore_tp_is_numeric;
identifier bcore_tp_is_float;

identifier bcore_lcg00_u3;

identifier bcore_err_fv;
identifier bcore_strlen;

identifier bcore_bin_ml_a_copy;

identifier bcore_cday_utc_s_from_system;
identifier bcore_cday_utc_s_to_sink;
identifier bcore_cday_ymd_s_to_cday;
identifier bcore_cday_from_source;
identifier bcore_cday_from_string;
identifier bcore_cday_from_sc;

identifier bcore_life_s_push_aware;

identifier bcore_abort;
identifier bcore_exit;

identifier TYPEOF_;

/// compare ...
identifier bcore_compare_spect;
identifier bcore_compare_typed;
identifier bcore_compare_bityped;
identifier bcore_compare_sr;
identifier bcore_compare_q_sr;

identifier bcore_equal_spect;
identifier bcore_equal_typed;
identifier bcore_equal_bityped;

/// Aware operations can handle objects of different type by imposing a type order.
identifier bcore_compare_aware;
identifier bcore_equal_aware;

/// diff: in case values differ, a string is created describing the nature of difference
identifier bcore_diff_spect;
identifier bcore_diff_typed;
identifier bcore_diff_bityped;
identifier bcore_diff_aware;
identifier bcore_diff_sr;
identifier bcore_diff_q_sr;

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
    feature '' er_t copy_typed( m @* o, tp_t type, vc_t src );
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

embed "bcore_name_manager_inexpandable.x";
embed "bcore_const_manager_inexpandable.x";
embed "bcore_global_manager_inexpandable.x";
embed "bcore_function_manager_inexpandable.x";
embed "bcore_source_inexpandable.x";
embed "bcore_sink_inexpandable.x";
embed "bcore_st_inexpandable.x";
embed "bcore_arr_inexpandable.x";
embed "bcore_hmap_inexpandable.x";
embed "bcore_hmap_tp_sr_inexpandable.x";
embed "bcore_flect_inexpandable.x";
embed "bcore_via_inexpandable.x";
embed "bcore_sr_inexpandable.x";
embed "bcore_img_inexpandable.x";
embed "bcore_folder_inexpandable.x";

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
