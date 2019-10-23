/** Author and Copyright 2018 Johannes Bernhard Steffens
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
#include "bcore_img.h"

/**********************************************************************************************************************/
// bmath features for certain leaf types

static bl_t bmath_f3_t_is_equ( const f3_t* o, const f3_t* op ) { return *o == *op; }
static bl_t bmath_f3_t_is_zro( const f3_t* o                 ) { return *o == 0.0; }
static bl_t bmath_f3_t_is_one( const f3_t* o                 ) { return *o == 1.0; }

static void bmath_f3_t_add( const f3_t* o, const f3_t* op, f3_t* res ) { *res = *o + *op; }
static void bmath_f3_t_zro(       f3_t* o                            ) { *o   = 0.0;      }
static void bmath_f3_t_neg( const f3_t* o,                 f3_t* res ) { *res = -*o;      }
static void bmath_f3_t_sub( const f3_t* o, const f3_t* op, f3_t* res ) { *res = *o - *op; }
static void bmath_f3_t_mul( const f3_t* o, const f3_t* op, f3_t* res ) { *res = *o * *op; }
static void bmath_f3_t_one(       f3_t* o                            ) { *o   = 1.0;      }
static void bmath_f3_t_inv( const f3_t* o,                 f3_t* res ) { *res = 1.0 / *o; }
static void bmath_f3_t_div( const f3_t* o, const f3_t* op, f3_t* res ) { *res = *o / *op; }

static bl_t bmath_f2_t_is_equ( const f2_t* o, const f3_t* op ) { return *o == *op; }
static bl_t bmath_f2_t_is_zro( const f2_t* o                 ) { return *o == 0.0; }
static bl_t bmath_f2_t_is_one( const f2_t* o                 ) { return *o == 1.0; }

static void bmath_f2_t_add( const f2_t* o, const f2_t* op, f2_t* res ) { *res = *o + *op; }
static void bmath_f2_t_zro(       f2_t* o                            ) { *o   = 0.0;      }
static void bmath_f2_t_neg( const f2_t* o,                 f2_t* res ) { *res = -*o;      }
static void bmath_f2_t_sub( const f2_t* o, const f2_t* op, f2_t* res ) { *res = *o - *op; }
static void bmath_f2_t_mul( const f2_t* o, const f2_t* op, f2_t* res ) { *res = *o * *op; }
static void bmath_f2_t_one(       f2_t* o                            ) { *o   = 1.0;      }
static void bmath_f2_t_inv( const f2_t* o,                 f2_t* res ) { *res = 1.0 / *o; }
static void bmath_f2_t_div( const f2_t* o, const f2_t* op, f2_t* res ) { *res = *o / *op; }

static bl_t bmath_s3_t_is_equ( const s3_t* o, const f3_t* op ) { return *o == *op; }
static bl_t bmath_s3_t_is_zro( const s3_t* o                 ) { return *o == 0; }
static bl_t bmath_s3_t_is_one( const s3_t* o                 ) { return *o == 1; }

static void bmath_s3_t_add( const s3_t* o, const s3_t* op, s3_t* res ) { *res = *o + *op; }
static void bmath_s3_t_zro(       s3_t* o                            ) { *o   = 0;        }
static void bmath_s3_t_neg( const s3_t* o,                 s3_t* res ) { *res = -*o;      }
static void bmath_s3_t_sub( const s3_t* o, const s3_t* op, s3_t* res ) { *res = *o - *op; }
static void bmath_s3_t_mul( const s3_t* o, const s3_t* op, s3_t* res ) { *res = *o * *op; }
static void bmath_s3_t_one(       s3_t* o                            ) { *o   = 1;        }

//----------------------------------------------------------------------------------------------------------------------

u2_t bmath_u2_argb_from_f3( vd_t o, f3_t v )
{
    if( v == 0 ) return 0;
    u0_t r = ( v < 0 ) ? ( ( v >= -1.0 ) ? -v * ( 255 - 64 ) + 64 : 255 ) : 0;
    u0_t g = ( v > 0 ) ? ( ( v <=  1.0 ) ?  v * ( 255 - 64 ) + 64 : 255 ) : 0;
    u0_t b = ( f3_abs( v ) > 1.0 ) ? ( ( f3_abs( v ) - 1.0 ) / ( f3_abs( v ) ) ) * ( 255 - 64 ) + 64 : 64;
    return bcore_img_u2_pixel_from_rgb( TYPEOF_bcore_img_u2_argb, r, g, b );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

vd_t bmath_leaf_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_leaf" ) ) )
    {
        case TYPEOF_init1:
        {
            // functions
            bcore_flect_push_ns_func( TYPEOF_f3_t, ( fp_t )bmath_f3_t_is_equ, "bmath_fp_is_equ", "is_equ" );
            bcore_flect_push_ns_func( TYPEOF_f3_t, ( fp_t )bmath_f3_t_is_zro, "bmath_fp_is_zro", "is_zro" );
            bcore_flect_push_ns_func( TYPEOF_f3_t, ( fp_t )bmath_f3_t_is_one, "bmath_fp_is_one", "is_one" );

            bcore_flect_push_ns_func( TYPEOF_f3_t, ( fp_t )bmath_f3_t_add, "bmath_fp_add", "add" );
            bcore_flect_push_ns_func( TYPEOF_f3_t, ( fp_t )bmath_f3_t_zro, "bmath_fp_zro", "zro" );
            bcore_flect_push_ns_func( TYPEOF_f3_t, ( fp_t )bmath_f3_t_neg, "bmath_fp_neg", "neg" );
            bcore_flect_push_ns_func( TYPEOF_f3_t, ( fp_t )bmath_f3_t_sub, "bmath_fp_sub", "sub" );
            bcore_flect_push_ns_func( TYPEOF_f3_t, ( fp_t )bmath_f3_t_mul, "bmath_fp_mul", "mul" );
            bcore_flect_push_ns_func( TYPEOF_f3_t, ( fp_t )bmath_f3_t_one, "bmath_fp_one", "one" );
            bcore_flect_push_ns_func( TYPEOF_f3_t, ( fp_t )bmath_f3_t_inv, "bmath_fp_inv", "inv" );
            bcore_flect_push_ns_func( TYPEOF_f3_t, ( fp_t )bmath_f3_t_div, "bmath_fp_div", "div" );

            bcore_flect_push_ns_func( TYPEOF_f2_t, ( fp_t )bmath_f2_t_is_equ, "bmath_fp_is_equ", "is_equ" );
            bcore_flect_push_ns_func( TYPEOF_f2_t, ( fp_t )bmath_f2_t_is_zro, "bmath_fp_is_zro", "is_zro" );
            bcore_flect_push_ns_func( TYPEOF_f2_t, ( fp_t )bmath_f2_t_is_one, "bmath_fp_is_one", "is_one" );

            bcore_flect_push_ns_func( TYPEOF_f2_t, ( fp_t )bmath_f2_t_add, "bmath_fp_add", "add" );
            bcore_flect_push_ns_func( TYPEOF_f2_t, ( fp_t )bmath_f2_t_zro, "bmath_fp_zro", "zro" );
            bcore_flect_push_ns_func( TYPEOF_f2_t, ( fp_t )bmath_f2_t_neg, "bmath_fp_neg", "neg" );
            bcore_flect_push_ns_func( TYPEOF_f2_t, ( fp_t )bmath_f2_t_sub, "bmath_fp_sub", "sub" );
            bcore_flect_push_ns_func( TYPEOF_f2_t, ( fp_t )bmath_f2_t_mul, "bmath_fp_mul", "mul" );
            bcore_flect_push_ns_func( TYPEOF_f2_t, ( fp_t )bmath_f2_t_one, "bmath_fp_one", "one" );
            bcore_flect_push_ns_func( TYPEOF_f2_t, ( fp_t )bmath_f2_t_inv, "bmath_fp_inv", "inv" );
            bcore_flect_push_ns_func( TYPEOF_f2_t, ( fp_t )bmath_f2_t_div, "bmath_fp_div", "div" );

            bcore_flect_push_ns_func( TYPEOF_s3_t, ( fp_t )bmath_s3_t_is_equ, "bmath_fp_is_equ", "is_equ" );
            bcore_flect_push_ns_func( TYPEOF_s3_t, ( fp_t )bmath_s3_t_is_zro, "bmath_fp_is_zro", "is_zro" );
            bcore_flect_push_ns_func( TYPEOF_s3_t, ( fp_t )bmath_s3_t_is_one, "bmath_fp_is_one", "is_one" );

            bcore_flect_push_ns_func( TYPEOF_s3_t, ( fp_t )bmath_s3_t_add, "bmath_fp_add", "add" );
            bcore_flect_push_ns_func( TYPEOF_s3_t, ( fp_t )bmath_s3_t_zro, "bmath_fp_zro", "zro" );
            bcore_flect_push_ns_func( TYPEOF_s3_t, ( fp_t )bmath_s3_t_neg, "bmath_fp_neg", "neg" );
            bcore_flect_push_ns_func( TYPEOF_s3_t, ( fp_t )bmath_s3_t_sub, "bmath_fp_sub", "sub" );
            bcore_flect_push_ns_func( TYPEOF_s3_t, ( fp_t )bmath_s3_t_mul, "bmath_fp_mul", "mul" );
            bcore_flect_push_ns_func( TYPEOF_s3_t, ( fp_t )bmath_s3_t_one, "bmath_fp_one", "one" );

            BCORE_REGISTER_FFUNC( bmath_fp_u2_argb_from_f3, bmath_u2_argb_from_f3 );

            BCORE_REGISTER_FEATURE( bmath_fp_f3_ar0 );
            BCORE_REGISTER_FEATURE( bmath_fp_f3_ar1 );
            BCORE_REGISTER_FEATURE( bmath_fp_f3_ar2 );

            BCORE_REGISTER_FEATURE( bmath_fp_f3_op_ar0 );
            BCORE_REGISTER_FEATURE( bmath_fp_f3_op_ar1 );
            BCORE_REGISTER_FEATURE( bmath_fp_f3_op_ar2 );
        }
        break;

        case TYPEOF_get_quicktypes:
        {
            BCORE_REGISTER_QUICKTYPE( bmath_fp_f3_ar0 );
            BCORE_REGISTER_QUICKTYPE( bmath_fp_f3_ar1 );
            BCORE_REGISTER_QUICKTYPE( bmath_fp_f3_ar2 );

            BCORE_REGISTER_QUICKTYPE( bmath_fp_f3_op_ar0 );
            BCORE_REGISTER_QUICKTYPE( bmath_fp_f3_op_ar1 );
            BCORE_REGISTER_QUICKTYPE( bmath_fp_f3_op_ar2 );
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

