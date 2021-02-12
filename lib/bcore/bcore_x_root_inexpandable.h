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

// External types

// primitives
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

///TODO: remove below (does not belong here)
type bmath_vf3_s;

// objects
type bcore_self_s;
type bcore_self_item_s;

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

    stamp :s = obliv bcore_inst
    {
        func : .lock;
        func : .unlock;
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
