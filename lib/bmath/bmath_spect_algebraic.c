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

#include "bmath_spect_algebraic.h"
#include "bmath_quicktypes.h"

/**********************************************************************************************************************/
// bmath features for certain leaf types

static void bmath_f3_add( f3_t* o, const f3_t* op1, const f3_t* op2 ) { *o = *op1 + *op2; }
static void bmath_f3_zro( f3_t* o )                                   { *o = 0.0; }
static void bmath_f3_neg( f3_t* o, const f3_t* op )                   { *o = -*op; }
static void bmath_f3_sub( f3_t* o, const f3_t* op1, const f3_t* op2 ) { *o = *op1 - *op2; }

static void bmath_f3_mul( f3_t* o, const f3_t* op1, const f3_t* op2 ) { *o = *op1 * *op2; }
static void bmath_f3_one( f3_t* o )                                   { *o = 1.0; }
static void bmath_f3_inv( f3_t* o, const f3_t* op )                   { *o = 1.0  / *op; }
static void bmath_f3_div( f3_t* o, const f3_t* op1, const f3_t* op2 ) { *o = *op1 / *op2; }

static void bmath_f2_add( f2_t* o, const f2_t* op1, const f2_t* op2 ) { *o = *op1 + *op2; }
static void bmath_f2_zro( f2_t* o )                                   { *o = 0.0; }
static void bmath_f2_neg( f2_t* o, const f2_t* op )                   { *o = -*op; }
static void bmath_f2_sub( f2_t* o, const f2_t* op1, const f2_t* op2 ) { *o = *op1 - *op2; }

static void bmath_f2_mul( f2_t* o, const f2_t* op1, const f2_t* op2 ) { *o = *op1 * *op2; }
static void bmath_f2_one( f2_t* o )                                   { *o = 1.0; }
static void bmath_f2_inv( f2_t* o, const f2_t* op )                   { *o = 1.0  / *op; }
static void bmath_f2_div( f2_t* o, const f2_t* op1, const f2_t* op2 ) { *o = *op1 / *op2; }

static void bmath_s3_add( s3_t* o, const s3_t* op1, const s3_t* op2 ) { *o = *op1 + *op2; }
static void bmath_s3_zro( s3_t* o )                                   { *o = 0; }
static void bmath_s3_neg( s3_t* o, const s3_t* op )                   { *o = -*op; }
static void bmath_s3_sub( s3_t* o, const s3_t* op1, const s3_t* op2 ) { *o = *op1 - *op2; }

static void bmath_s3_mul( s3_t* o, const s3_t* op1, const s3_t* op2 ) { *o = *op1 * *op2; }
static void bmath_s3_one( s3_t* o )                                   { *o = 1; }

/**********************************************************************************************************************/
// bmath_abelian_group_s

static sc_t bmath_abelian_group_s_def = "bmath_abelian_group_s = spect"
"{"
    "bcore_spect_header_s header;"
    "strict feature bmath_fp_add add ~> func bmath_fp_add;"
//    "strict feature bmath_fp_add add;"
    "strict feature bmath_fp_zro zro;"
    "strict feature bmath_fp_neg neg;"
    "       feature bmath_fp_sub sub;"
"}";

BCORE_DEFINE_OBJECT_INST( bmath_abelian_group_s, bmath_abelian_group_s_def )

const bmath_abelian_group_s* bmath_abelian_group_s_get_typed( tp_t o_type )
{
    return bcore_spect_get_typed( TYPEOF_bmath_abelian_group_s, o_type );
}

const bmath_abelian_group_s* bmath_abelian_group_s_get_aware( vc_t obj )
{
    return bmath_abelian_group_s_get_typed( *( const aware_t* )obj );
}

void bmath_abelian_group_spect_add( const bmath_abelian_group_s* p, vd_t o, vc_t op1, vc_t op2 )
{
    p->fp_add( o, op1, op2 );
}

void bmath_abelian_group_spect_zro( const bmath_abelian_group_s* p, vd_t o )
{
    p->fp_zro( o );
}

void bmath_abelian_group_spect_neg( const bmath_abelian_group_s* p, vd_t o, vc_t op )
{
    p->fp_neg( o, op );
}

void bmath_abelian_group_spect_sub( const bmath_abelian_group_s* p, vd_t o, vc_t op1, vc_t op2 )
{
    if( p->fp_sub )
    {
        p->fp_sub( o, op1, op2 );
    }
    else
    {
        p->fp_neg( o, op2 );
        p->fp_add( o, op1, o );
    }
}

/**********************************************************************************************************************/
// bmath_ring_s

static sc_t bmath_ring_s_def = "bmath_ring_s = spect"
"{"
    "bcore_spect_header_s header;"
    "strict feature bmath_fp_add add ~> func bmath_fp_add add;"

//    "strict feature bmath_fp_add add;"
    "strict feature bmath_fp_zro zro;"
    "strict feature bmath_fp_neg neg;"
    "       feature bmath_fp_sub sub;"

    "strict feature bmath_fp_mul mul;"
    "       feature bmath_fp_one one;"
    "       feature bmath_fp_inv inv;"
    "       feature bmath_fp_div div;"
"}";

BCORE_DEFINE_OBJECT_INST( bmath_ring_s, bmath_ring_s_def )

const bmath_ring_s* bmath_ring_s_get_typed( tp_t o_type )
{
    return bcore_spect_get_typed( TYPEOF_bmath_ring_s, o_type );
}

const bmath_ring_s* bmath_ring_s_get_aware( vc_t obj )
{
    return bmath_ring_s_get_typed( *( const aware_t* )obj );
}

void bmath_ring_spect_add( const bmath_ring_s* p, vd_t o, vc_t op1, vc_t op2 )
{
    p->fp_add( o, op1, op2 );
}

void bmath_ring_spect_zro( const bmath_ring_s* p, vd_t o )
{
    p->fp_zro( o );
}

void bmath_ring_spect_neg( const bmath_ring_s* p, vd_t o, vc_t op )
{
    p->fp_neg( o, op );
}

void bmath_ring_spect_sub( const bmath_ring_s* p, vd_t o, vc_t op1, vc_t op2 )
{
    if( p->fp_sub )
    {
        p->fp_sub( o, op1, op2 );
    }
    else
    {
        p->fp_neg( o, op2 );
        p->fp_add( o, op1, o );
    }
}

void bmath_ring_spect_mul( const bmath_ring_s* p, vd_t o, vc_t op1, vc_t op2 )
{
    p->fp_mul( o, op1, op2 );
}

void bmath_ring_spect_one( const bmath_ring_s* p, vd_t o )
{
    p->fp_one( o );
}

void bmath_ring_spect_inv( const bmath_ring_s* p, vd_t o, vc_t op )
{
    if( p->fp_inv )
    {
        p->fp_inv( o, op );
    }
    else
    {
        WRN_fa( "Inversion is not available for object '#<sc_t>'.", ifnameof( p->header.o_type ) );
    }
}

void bmath_ring_spect_div( const bmath_ring_s* p, vd_t o, vc_t op1, vc_t op2 )
{
    if( p->fp_div )
    {
        p->fp_div( o, op1, op2 );
    }
    else if( p->fp_inv )
    {
        p->fp_inv( o, op2 );
        p->fp_mul( o, op1, o );
    }
    else
    {
        WRN_fa( "Division is not available for object '#<sc_t>'.", ifnameof( p->header.o_type ) );
    }
}

/**********************************************************************************************************************/

static vd_t selftest( void )
{
    ASSERT( bcore_trait_satisfied_type( typeof( "bmath_abelian_group" ), TYPEOF_f3_t, NULL ) );

    const bmath_abelian_group_s* p_group = bmath_abelian_group_s_get_typed( TYPEOF_f3_t );

    f3_t v1 = 3;
    f3_t v2 = 2;
    f3_t v3 = 0;

    bmath_abelian_group_spect_sub( p_group, &v3, &v1, &v2 );

    ASSERT( v3 == 1 );

    return NULL;
}

/**********************************************************************************************************************/

vd_t bmath_spect_algebraic_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_spect_algebraic" ) ) )
    {
        case TYPEOF_init1:
        {
            // features
            BCORE_REGISTER_PLAIN( bmath_fp_add, function_pointer );
            BCORE_REGISTER_PLAIN( bmath_fp_sub, function_pointer );
            BCORE_REGISTER_PLAIN( bmath_fp_neg, function_pointer );
            BCORE_REGISTER_PLAIN( bmath_fp_zro, function_pointer );
            BCORE_REGISTER_PLAIN( bmath_fp_mul, function_pointer );
            BCORE_REGISTER_PLAIN( bmath_fp_div, function_pointer );
            BCORE_REGISTER_PLAIN( bmath_fp_inv, function_pointer );
            BCORE_REGISTER_PLAIN( bmath_fp_one, function_pointer );

            // functions
            bcore_flect_push_ns_func( TYPEOF_f3_t, ( fp_t )bmath_f3_add, "bmath_fp_add", "add" );
            bcore_flect_push_ns_func( TYPEOF_f3_t, ( fp_t )bmath_f3_zro, "bmath_fp_zro", "zro" );
            bcore_flect_push_ns_func( TYPEOF_f3_t, ( fp_t )bmath_f3_neg, "bmath_fp_neg", "neg" );
            bcore_flect_push_ns_func( TYPEOF_f3_t, ( fp_t )bmath_f3_sub, "bmath_fp_sub", "sub" );
            bcore_flect_push_ns_func( TYPEOF_f3_t, ( fp_t )bmath_f3_mul, "bmath_fp_mul", "mul" );
            bcore_flect_push_ns_func( TYPEOF_f3_t, ( fp_t )bmath_f3_one, "bmath_fp_one", "one" );
            bcore_flect_push_ns_func( TYPEOF_f3_t, ( fp_t )bmath_f3_inv, "bmath_fp_inv", "inv" );
            bcore_flect_push_ns_func( TYPEOF_f3_t, ( fp_t )bmath_f3_div, "bmath_fp_div", "div" );

            bcore_flect_push_ns_func( TYPEOF_f2_t, ( fp_t )bmath_f2_add, "bmath_fp_add", "add" );
            bcore_flect_push_ns_func( TYPEOF_f2_t, ( fp_t )bmath_f2_zro, "bmath_fp_zro", "zro" );
            bcore_flect_push_ns_func( TYPEOF_f2_t, ( fp_t )bmath_f2_neg, "bmath_fp_neg", "neg" );
            bcore_flect_push_ns_func( TYPEOF_f2_t, ( fp_t )bmath_f2_sub, "bmath_fp_sub", "sub" );
            bcore_flect_push_ns_func( TYPEOF_f2_t, ( fp_t )bmath_f2_mul, "bmath_fp_mul", "mul" );
            bcore_flect_push_ns_func( TYPEOF_f2_t, ( fp_t )bmath_f2_one, "bmath_fp_one", "one" );
            bcore_flect_push_ns_func( TYPEOF_f2_t, ( fp_t )bmath_f2_inv, "bmath_fp_inv", "inv" );
            bcore_flect_push_ns_func( TYPEOF_f2_t, ( fp_t )bmath_f2_div, "bmath_fp_div", "div" );

            bcore_flect_push_ns_func( TYPEOF_s3_t, ( fp_t )bmath_s3_add, "bmath_fp_add", "add" );
            bcore_flect_push_ns_func( TYPEOF_s3_t, ( fp_t )bmath_s3_zro, "bmath_fp_zro", "zro" );
            bcore_flect_push_ns_func( TYPEOF_s3_t, ( fp_t )bmath_s3_neg, "bmath_fp_neg", "neg" );
            bcore_flect_push_ns_func( TYPEOF_s3_t, ( fp_t )bmath_s3_sub, "bmath_fp_sub", "sub" );
            bcore_flect_push_ns_func( TYPEOF_s3_t, ( fp_t )bmath_s3_mul, "bmath_fp_mul", "mul" );
            bcore_flect_push_ns_func( TYPEOF_s3_t, ( fp_t )bmath_s3_one, "bmath_fp_one", "one" );

            // reflections
            BCORE_REGISTER_SPECT( bmath_abelian_group_s );
            BCORE_REGISTER_SPECT( bmath_ring_s );
        }
        break;

        case TYPEOF_selftest:
        {
            return selftest();
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

