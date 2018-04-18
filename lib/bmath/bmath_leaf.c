/** Copyright 2018 Johannes Bernhard Steffens
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

#include "bmath_leaf.h"
#include "bcore_flect.h"

/**********************************************************************************************************************/
// bmath features for certain leaf types

static void bmath_f3_t_add( f3_t* o, const f3_t* op1, const f3_t* op2 ) { *o = *op1 + *op2; }
static void bmath_f3_t_zro( f3_t* o )                                   { *o = 0.0; }
static void bmath_f3_t_neg( f3_t* o, const f3_t* op )                   { *o = -*op; }
static void bmath_f3_t_sub( f3_t* o, const f3_t* op1, const f3_t* op2 ) { *o = *op1 - *op2; }

static void bmath_f3_t_mul( f3_t* o, const f3_t* op1, const f3_t* op2 ) { *o = *op1 * *op2; }
static void bmath_f3_t_one( f3_t* o )                                   { *o = 1.0; }
static void bmath_f3_t_inv( f3_t* o, const f3_t* op )                   { *o = 1.0  / *op; }
static void bmath_f3_t_div( f3_t* o, const f3_t* op1, const f3_t* op2 ) { *o = *op1 / *op2; }

static void bmath_f2_t_add( f2_t* o, const f2_t* op1, const f2_t* op2 ) { *o = *op1 + *op2; }
static void bmath_f2_t_zro( f2_t* o )                                   { *o = 0.0; }
static void bmath_f2_t_neg( f2_t* o, const f2_t* op )                   { *o = -*op; }
static void bmath_f2_t_sub( f2_t* o, const f2_t* op1, const f2_t* op2 ) { *o = *op1 - *op2; }

static void bmath_f2_t_mul( f2_t* o, const f2_t* op1, const f2_t* op2 ) { *o = *op1 * *op2; }
static void bmath_f2_t_one( f2_t* o )                                   { *o = 1.0; }
static void bmath_f2_t_inv( f2_t* o, const f2_t* op )                   { *o = 1.0  / *op; }
static void bmath_f2_t_div( f2_t* o, const f2_t* op1, const f2_t* op2 ) { *o = *op1 / *op2; }

static void bmath_s3_t_add( s3_t* o, const s3_t* op1, const s3_t* op2 ) { *o = *op1 + *op2; }
static void bmath_s3_t_zro( s3_t* o )                                   { *o = 0; }
static void bmath_s3_t_neg( s3_t* o, const s3_t* op )                   { *o = -*op; }
static void bmath_s3_t_sub( s3_t* o, const s3_t* op1, const s3_t* op2 ) { *o = *op1 - *op2; }

static void bmath_s3_t_mul( s3_t* o, const s3_t* op1, const s3_t* op2 ) { *o = *op1 * *op2; }
static void bmath_s3_t_one( s3_t* o )                                   { *o = 1; }

/**********************************************************************************************************************/

vd_t bmath_leaf_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_leaf" ) ) )
    {
        case TYPEOF_init1:
        {
            // functions
            bcore_flect_push_ns_func( TYPEOF_f3_t, ( fp_t )bmath_f3_t_add, "bmath_fp_add", "add" );
            bcore_flect_push_ns_func( TYPEOF_f3_t, ( fp_t )bmath_f3_t_zro, "bmath_fp_zro", "zro" );
            bcore_flect_push_ns_func( TYPEOF_f3_t, ( fp_t )bmath_f3_t_neg, "bmath_fp_neg", "neg" );
            bcore_flect_push_ns_func( TYPEOF_f3_t, ( fp_t )bmath_f3_t_sub, "bmath_fp_sub", "sub" );
            bcore_flect_push_ns_func( TYPEOF_f3_t, ( fp_t )bmath_f3_t_mul, "bmath_fp_mul", "mul" );
            bcore_flect_push_ns_func( TYPEOF_f3_t, ( fp_t )bmath_f3_t_one, "bmath_fp_one", "one" );
            bcore_flect_push_ns_func( TYPEOF_f3_t, ( fp_t )bmath_f3_t_inv, "bmath_fp_inv", "inv" );
            bcore_flect_push_ns_func( TYPEOF_f3_t, ( fp_t )bmath_f3_t_div, "bmath_fp_div", "div" );

            bcore_flect_push_ns_func( TYPEOF_f2_t, ( fp_t )bmath_f2_t_add, "bmath_fp_add", "add" );
            bcore_flect_push_ns_func( TYPEOF_f2_t, ( fp_t )bmath_f2_t_zro, "bmath_fp_zro", "zro" );
            bcore_flect_push_ns_func( TYPEOF_f2_t, ( fp_t )bmath_f2_t_neg, "bmath_fp_neg", "neg" );
            bcore_flect_push_ns_func( TYPEOF_f2_t, ( fp_t )bmath_f2_t_sub, "bmath_fp_sub", "sub" );
            bcore_flect_push_ns_func( TYPEOF_f2_t, ( fp_t )bmath_f2_t_mul, "bmath_fp_mul", "mul" );
            bcore_flect_push_ns_func( TYPEOF_f2_t, ( fp_t )bmath_f2_t_one, "bmath_fp_one", "one" );
            bcore_flect_push_ns_func( TYPEOF_f2_t, ( fp_t )bmath_f2_t_inv, "bmath_fp_inv", "inv" );
            bcore_flect_push_ns_func( TYPEOF_f2_t, ( fp_t )bmath_f2_t_div, "bmath_fp_div", "div" );

            bcore_flect_push_ns_func( TYPEOF_s3_t, ( fp_t )bmath_s3_t_add, "bmath_fp_add", "add" );
            bcore_flect_push_ns_func( TYPEOF_s3_t, ( fp_t )bmath_s3_t_zro, "bmath_fp_zro", "zro" );
            bcore_flect_push_ns_func( TYPEOF_s3_t, ( fp_t )bmath_s3_t_neg, "bmath_fp_neg", "neg" );
            bcore_flect_push_ns_func( TYPEOF_s3_t, ( fp_t )bmath_s3_t_sub, "bmath_fp_sub", "sub" );
            bcore_flect_push_ns_func( TYPEOF_s3_t, ( fp_t )bmath_s3_t_mul, "bmath_fp_mul", "mul" );
            bcore_flect_push_ns_func( TYPEOF_s3_t, ( fp_t )bmath_s3_t_one, "bmath_fp_one", "one" );
        }
        break;

        case TYPEOF_selftest:
        {
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

