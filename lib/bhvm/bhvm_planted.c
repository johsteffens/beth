/** This file was generated from beth-plant source code.
 *  Compiling Agent : bcore_plant_compiler (C) 2019 J.B.Steffens
 *  Last File Update: 2020-02-04T12:26:08Z
 *
 *  Copyright and License of this File:
 *
 *  Generated code inherits the copyright and license of the underlying beth-plant source code.
 *  Source code defining this file is distributed across following files:
 *
 *  bhvm_holor.h
 *  bhvm_hop.h
 *  bhvm_mcode.h
 *
 */

#include "bhvm_planted.h"
#include "bcore_spect.h"
#include "bcore_spect_inst.h"
#include "bcore_sr.h"
#include "bcore_const_manager.h"


/**********************************************************************************************************************/
// source: bhvm_holor.h
#include "bhvm_holor.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm

BCORE_DEFINE_OBJECT_INST_P( bhvm_shape_s )
"bcore_array"
"{"
    "sz_t [];"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_value_s )
"bcore_array"
"{"
    "typed [];"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_holor_s )
"aware bcore_inst"
"{"
    "bhvm_shape_s s;"
    "bhvm_value_s v;"
    "func bcore_fp:copy_typed;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_holor_adl_s )
"aware bcore_array"
"{"
    "bhvm_holor_s => [];"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_holor_ads_s )
"aware bcore_array"
"{"
    "bhvm_holor_s [];"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_holor_mdl_s )
"aware bcore_array"
"{"
    "bhvm_holor_adl_s => [];"
"}";

/**********************************************************************************************************************/
// source: bhvm_hop.h
#include "bhvm_hop.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hop

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hop_ar0

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar0_zro_s )
"aware bhvm_hop_ar0"
"{"
"}";

void bhvm_hop_ar0_zro_s_f( bhvm_holor_s* r )
{
    bhvm_lop_ar0_zro_s_f( BKNIT_FA1( r->v.type ), r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar0_one_s )
"aware bhvm_hop_ar0"
"{"
"}";

void bhvm_hop_ar0_one_s_f( bhvm_holor_s* r )
{
    bhvm_lop_ar0_one_s_f( BKNIT_FA1( r->v.type ), r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar0_nul_dp_s )
"aware bhvm_hop_ar0"
"{"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hop_ar1

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_cpy_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_cpy_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_cpy_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_identity_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_identity_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_identity_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_neg_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_neg_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_neg_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_floor_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_floor_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_floor_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_ceil_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_ceil_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_ceil_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_exp_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_exp_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_exp_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_log_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_log_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_log_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_inv_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_inv_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_inv_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_abs_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_abs_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_abs_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_sig_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_sig_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_sig_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_l1_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_l1_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( r->v.size == 1 );
    bhvm_lop_ar1_l1_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, a->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_sqr_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_sqr_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_sqr_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_srt_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_srt_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_srt_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_sqrsum_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_sqrsum_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( r->v.size == 1 );
    bhvm_lop_ar1_sqrsum_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, a->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_lgst_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_lgst_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_lgst_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_lgst_hard_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_lgst_hard_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_lgst_hard_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_lgst_leaky_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_lgst_leaky_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_lgst_leaky_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_tanh_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_tanh_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_tanh_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_tanh_hard_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_tanh_hard_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_tanh_hard_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_tanh_leaky_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_tanh_leaky_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_tanh_leaky_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_softplus_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_softplus_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_softplus_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_relu_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_relu_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_relu_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_relu_leaky_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_relu_leaky_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_relu_leaky_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_identity_dp_zf_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_identity_dp_zf_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_identity_dp_zf_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_neg_dp_zf_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_neg_dp_zf_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_neg_dp_zf_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hop_ar1_eci

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_eci_cpy_acc_s )
"aware bhvm_hop_ar1_eci"
"{"
"}";

void bhvm_hop_ar1_eci_cpy_acc_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a != r );
    assert(  sz_min( a->v.size, r->v.size ) > 0 );
    sz_t n = sz_gcd( a->v.size, r->v.size );
    sz_t m = sz_max( a->v.size, r->v.size ) / n;
    
    
    #define bhvm_hop_ar1_eci_ACC_CASE( TA_T, TR_T, FUNC ) \
            { \
                const TA_T *a0 = a->v.data, *a1 = a0; \
                      TR_T *r0 = r->v.data, *r1 = r0; \
                for( sz_t i = 0; i < m; i++ ) \
                { \
                    for( sz_t i = 0; i < n; i++ ) r1[ i ] += FUNC( a1[ i ] ); \
                    a1 = ( a1 + n - a0 ) < a->v.size ? ( a1 + n ) : a0; \
                    r1 = ( r1 + n - r0 ) < r->v.size ? ( r1 + n ) : r0; \
                } \
            }
    
    switch( BKNIT_FA2( a->v.type, r->v.type ) )
    {
        case BKNIT_F22: bhvm_hop_ar1_eci_ACC_CASE( f2_t, f2_t, bhvm_hop_ar1_eci_cpy_acc_s_f2 ); break;
        case BKNIT_F23: bhvm_hop_ar1_eci_ACC_CASE( f2_t, f3_t, bhvm_hop_ar1_eci_cpy_acc_s_f3 ); break;
        case BKNIT_F32: bhvm_hop_ar1_eci_ACC_CASE( f3_t, f2_t, bhvm_hop_ar1_eci_cpy_acc_s_f3 ); break;
        case BKNIT_F33: bhvm_hop_ar1_eci_ACC_CASE( f3_t, f3_t, bhvm_hop_ar1_eci_cpy_acc_s_f3 ); break;
        default: BKNIT_FA2_ERR( a->v.type, r->v.type ); break;
    }
    
    #undef bhvm_hop_ar1_eci_ACC_CASE
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_eci_neg_acc_s )
"aware bhvm_hop_ar1_eci"
"{"
"}";

void bhvm_hop_ar1_eci_neg_acc_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a != r );
    assert(  sz_min( a->v.size, r->v.size ) > 0 );
    sz_t n = sz_gcd( a->v.size, r->v.size );
    sz_t m = sz_max( a->v.size, r->v.size ) / n;
    
    
    #define bhvm_hop_ar1_eci_ACC_CASE( TA_T, TR_T, FUNC ) \
            { \
                const TA_T *a0 = a->v.data, *a1 = a0; \
                      TR_T *r0 = r->v.data, *r1 = r0; \
                for( sz_t i = 0; i < m; i++ ) \
                { \
                    for( sz_t i = 0; i < n; i++ ) r1[ i ] += FUNC( a1[ i ] ); \
                    a1 = ( a1 + n - a0 ) < a->v.size ? ( a1 + n ) : a0; \
                    r1 = ( r1 + n - r0 ) < r->v.size ? ( r1 + n ) : r0; \
                } \
            }
    
    switch( BKNIT_FA2( a->v.type, r->v.type ) )
    {
        case BKNIT_F22: bhvm_hop_ar1_eci_ACC_CASE( f2_t, f2_t, bhvm_hop_ar1_eci_neg_acc_s_f2 ); break;
        case BKNIT_F23: bhvm_hop_ar1_eci_ACC_CASE( f2_t, f3_t, bhvm_hop_ar1_eci_neg_acc_s_f3 ); break;
        case BKNIT_F32: bhvm_hop_ar1_eci_ACC_CASE( f3_t, f2_t, bhvm_hop_ar1_eci_neg_acc_s_f3 ); break;
        case BKNIT_F33: bhvm_hop_ar1_eci_ACC_CASE( f3_t, f3_t, bhvm_hop_ar1_eci_neg_acc_s_f3 ); break;
        default: BKNIT_FA2_ERR( a->v.type, r->v.type ); break;
    }
    
    #undef bhvm_hop_ar1_eci_ACC_CASE
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_eci_inv_acc_s )
"aware bhvm_hop_ar1_eci"
"{"
"}";

void bhvm_hop_ar1_eci_inv_acc_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a != r );
    assert(  sz_min( a->v.size, r->v.size ) > 0 );
    sz_t n = sz_gcd( a->v.size, r->v.size );
    sz_t m = sz_max( a->v.size, r->v.size ) / n;
    
    
    #define bhvm_hop_ar1_eci_ACC_CASE( TA_T, TR_T, FUNC ) \
            { \
                const TA_T *a0 = a->v.data, *a1 = a0; \
                      TR_T *r0 = r->v.data, *r1 = r0; \
                for( sz_t i = 0; i < m; i++ ) \
                { \
                    for( sz_t i = 0; i < n; i++ ) r1[ i ] += FUNC( a1[ i ] ); \
                    a1 = ( a1 + n - a0 ) < a->v.size ? ( a1 + n ) : a0; \
                    r1 = ( r1 + n - r0 ) < r->v.size ? ( r1 + n ) : r0; \
                } \
            }
    
    switch( BKNIT_FA2( a->v.type, r->v.type ) )
    {
        case BKNIT_F22: bhvm_hop_ar1_eci_ACC_CASE( f2_t, f2_t, bhvm_hop_ar1_eci_inv_acc_s_f2 ); break;
        case BKNIT_F23: bhvm_hop_ar1_eci_ACC_CASE( f2_t, f3_t, bhvm_hop_ar1_eci_inv_acc_s_f3 ); break;
        case BKNIT_F32: bhvm_hop_ar1_eci_ACC_CASE( f3_t, f2_t, bhvm_hop_ar1_eci_inv_acc_s_f3 ); break;
        case BKNIT_F33: bhvm_hop_ar1_eci_ACC_CASE( f3_t, f3_t, bhvm_hop_ar1_eci_inv_acc_s_f3 ); break;
        default: BKNIT_FA2_ERR( a->v.type, r->v.type ); break;
    }
    
    #undef bhvm_hop_ar1_eci_ACC_CASE
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_eci_cpy_s )
"aware bhvm_hop_ar1_eci"
"{"
"}";

void bhvm_hop_ar1_eci_cpy_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    if( a == r )
    {
        bhvm_holor_s* buf = bhvm_holor_s_create();
        bhvm_hop_ar1_eci_cpy_acc_s_f( a, bhvm_holor_s_fit_size( bhvm_holor_s_copy_shape_type( buf, r ) ) );
        if( r->v.size ) bhvm_value_s_cpy( &buf->v, &r->v );
        bhvm_holor_s_discard( buf );
    }
    else
    {
        bhvm_value_s_zro( &r->v );
        bhvm_hop_ar1_eci_cpy_acc_s_f( a, r );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_eci_neg_s )
"aware bhvm_hop_ar1_eci"
"{"
"}";

void bhvm_hop_ar1_eci_neg_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    if( a == r )
    {
        bhvm_holor_s* buf = bhvm_holor_s_create();
        bhvm_hop_ar1_eci_neg_acc_s_f( a, bhvm_holor_s_fit_size( bhvm_holor_s_copy_shape_type( buf, r ) ) );
        if( r->v.size ) bhvm_value_s_cpy( &buf->v, &r->v );
        bhvm_holor_s_discard( buf );
    }
    else
    {
        bhvm_value_s_zro( &r->v );
        bhvm_hop_ar1_eci_neg_acc_s_f( a, r );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_eci_inv_s )
"aware bhvm_hop_ar1_eci"
"{"
"}";

void bhvm_hop_ar1_eci_inv_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    if( a == r )
    {
        bhvm_holor_s* buf = bhvm_holor_s_create();
        bhvm_hop_ar1_eci_inv_acc_s_f( a, bhvm_holor_s_fit_size( bhvm_holor_s_copy_shape_type( buf, r ) ) );
        if( r->v.size ) bhvm_value_s_cpy( &buf->v, &r->v );
        bhvm_holor_s_discard( buf );
    }
    else
    {
        bhvm_value_s_zro( &r->v );
        bhvm_hop_ar1_eci_inv_acc_s_f( a, r );
    }
}

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hop_ar2

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_sub_sqrsum_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_sub_sqrsum_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == b->v.size ); assert( r->v.size == 1         );
    bhvm_lop_ar2_sub_sqrsum_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, a->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_sub_l1_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_sub_l1_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == b->v.size ); assert( r->v.size == 1         );
    bhvm_lop_ar2_sub_l1_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, a->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_equal_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_equal_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_equal_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_larger_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_larger_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_larger_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_smaller_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_smaller_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_smaller_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_larger_equal_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_larger_equal_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_larger_equal_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_smaller_equal_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_smaller_equal_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_smaller_equal_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_logic_and_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_logic_and_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_logic_and_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_logic_or_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_logic_or_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_logic_or_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_exp_dp_zyf_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_exp_dp_zyf_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_exp_dp_zyf_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_log_dp_zaf_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_log_dp_zaf_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_log_dp_zaf_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_inv_dp_zyf_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_inv_dp_zyf_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_inv_dp_zyf_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_sqr_dp_zaf_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_sqr_dp_zaf_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_sqr_dp_zaf_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_srt_dp_zyf_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_srt_dp_zyf_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_srt_dp_zyf_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_lgst_dp_zyf_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_lgst_dp_zyf_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_lgst_dp_zyf_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_lgst_hard_dp_zyf_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_lgst_hard_dp_zyf_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_lgst_hard_dp_zyf_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_lgst_leaky_dp_zyf_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_lgst_leaky_dp_zyf_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_lgst_leaky_dp_zyf_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_tanh_dp_zyf_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_tanh_dp_zyf_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_tanh_dp_zyf_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_tanh_hard_dp_zyf_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_tanh_hard_dp_zyf_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_tanh_hard_dp_zyf_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_tanh_leaky_dp_zyf_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_tanh_leaky_dp_zyf_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_tanh_leaky_dp_zyf_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_softplus_dp_zyf_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_softplus_dp_zyf_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_softplus_dp_zyf_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_relu_dp_zyf_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_relu_dp_zyf_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_relu_dp_zyf_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_relu_leaky_dp_zyf_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_relu_leaky_dp_zyf_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_relu_leaky_dp_zyf_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_branch_vvvv_dp_azg_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_branch_vvvv_dp_azg_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_branch_vvvv_dp_azg_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_branch_vvvv_dp_azh_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_branch_vvvv_dp_azh_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_branch_vvvv_dp_azh_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_mvv_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_mul_mvv_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->s.size == 2 ); assert( b->v.size == a->s.data[ 0 ] ); assert( r->v.size == a->s.data[ 1 ] );
    bhvm_lop_ar2_mul_mvv_s_f_m( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, a->s.data[ 1 ], a->s.data[ 0 ] );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_vmv_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_mul_vmv_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( b->s.size == 2 ); assert( a->v.size == b->s.data[ 1 ] ); assert( r->v.size == b->s.data[ 0 ] );
    bhvm_lop_ar2_mul_vmv_s_f_m( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, b->s.data[ 1 ], b->s.data[ 0 ] );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_vvm_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_mul_vvm_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( r->s.size == 2 ); assert( a->v.size == r->s.data[ 1 ] ); assert( b->v.size == r->s.data[ 0 ] );
    bhvm_lop_ar2_mul_vvm_s_f_m( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->s.data[ 1 ], r->s.data[ 0 ] );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_tvv_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_mul_tvv_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->s.size == 2 ); assert( b->v.size == a->s.data[ 1 ] ); assert( r->v.size == a->s.data[ 0 ] );
    bhvm_lop_ar2_mul_vmv_s_f_m( BKNIT_FA3( b->v.type, a->v.type, r->v.type ), b->v.data, a->v.data, r->v.data, a->s.data[ 1 ], a->s.data[ 0 ] );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_vtv_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_mul_vtv_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( b->s.size == 2 ); assert( a->v.size == b->s.data[ 0 ] ); assert( r->v.size == b->s.data[ 1 ] );
    bhvm_lop_ar2_mul_mvv_s_f_m( BKNIT_FA3( b->v.type, a->v.type, r->v.type ), b->v.data, a->v.data, r->v.data, b->s.data[ 1 ], b->s.data[ 0 ] );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_mmm_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_mtm_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_tmm_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_ttm_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_acc_mvv_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_mul_acc_mvv_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->s.size == 2 ); assert( b->v.size == a->s.data[ 0 ] ); assert( r->v.size == a->s.data[ 1 ] );
    bhvm_lop_ar2_mul_acc_mvv_s_f_m( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, a->s.data[ 1 ], a->s.data[ 0 ] );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_acc_vmv_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_mul_acc_vmv_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( b->s.size == 2 ); assert( a->v.size == b->s.data[ 1 ] ); assert( r->v.size == b->s.data[ 0 ] );
    bhvm_lop_ar2_mul_acc_vmv_s_f_m( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, b->s.data[ 1 ], b->s.data[ 0 ] );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_acc_tvv_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_mul_acc_tvv_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->s.size == 2 ); assert( b->v.size == a->s.data[ 1 ] ); assert( r->v.size == a->s.data[ 0 ] );
    bhvm_lop_ar2_mul_acc_vmv_s_f_m( BKNIT_FA3( b->v.type, a->v.type, r->v.type ), b->v.data, a->v.data, r->v.data, a->s.data[ 1 ], a->s.data[ 0 ] );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_acc_vtv_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_mul_acc_vtv_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( b->s.size == 2 ); assert( a->v.size == b->s.data[ 0 ] ); assert( r->v.size == b->s.data[ 1 ] );
    bhvm_lop_ar2_mul_acc_mvv_s_f_m( BKNIT_FA3( b->v.type, a->v.type, r->v.type ), b->v.data, a->v.data, r->v.data, b->s.data[ 1 ], b->s.data[ 0 ] );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_acc_vvm_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_mul_acc_vvm_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( r->s.size == 2 ); assert( a->v.size == r->s.data[ 1 ] ); assert( b->v.size == r->s.data[ 0 ] );
    bhvm_lop_ar2_mul_acc_vvm_s_f_m( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->s.data[ 1 ], r->s.data[ 0 ] );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_acc_mmm_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_acc_mtm_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_acc_tmm_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_acc_ttm_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_cat_s )
"aware bhvm_hop_ar2"
"{"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hop_ar2_eci

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_eci_add_acc_s )
"aware bhvm_hop_ar2_eci"
"{"
"}";

void bhvm_hop_ar2_eci_add_acc_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a != r && b != r );
    assert(  sz_min( sz_min( a->v.size, b->v.size ), r->v.size ) > 0 );
    sz_t n = sz_gcd( sz_gcd( a->v.size, b->v.size ), r->v.size );
    sz_t m = sz_max( sz_max( a->v.size, b->v.size ), r->v.size ) / n;
    
    #define bhvm_hop_ar2_eci_ACC_CASE( TA_T, TB_T, TR_T, FUNC ) \
            { \
                const TA_T *a0 = a->v.data, *a1 = a0; \
                const TB_T *b0 = b->v.data, *b1 = b0; \
                      TR_T *r0 = r->v.data, *r1 = r0; \
                for( sz_t i = 0; i < m; i++ ) \
                { \
                    for( sz_t i = 0; i < n; i++ ) r1[ i ] += FUNC( a1[ i ], b1[ i ] ); \
                    a1 = ( a1 + n - a0 ) < a->v.size ? ( a1 + n ) : a0; \
                    b1 = ( b1 + n - b0 ) < b->v.size ? ( b1 + n ) : b0; \
                    r1 = ( r1 + n - r0 ) < r->v.size ? ( r1 + n ) : r0; \
                } \
            }
    
    switch( BKNIT_FA3( a->v.type, b->v.type, r->v.type ) )
    {
        case BKNIT_F222: bhvm_hop_ar2_eci_ACC_CASE( f2_t, f2_t, f2_t, bhvm_hop_ar2_eci_add_acc_s_f2 ); break;
        case BKNIT_F223: bhvm_hop_ar2_eci_ACC_CASE( f2_t, f2_t, f3_t, bhvm_hop_ar2_eci_add_acc_s_f3 ); break;
        case BKNIT_F232: bhvm_hop_ar2_eci_ACC_CASE( f2_t, f3_t, f2_t, bhvm_hop_ar2_eci_add_acc_s_f3 ); break;
        case BKNIT_F233: bhvm_hop_ar2_eci_ACC_CASE( f2_t, f3_t, f3_t, bhvm_hop_ar2_eci_add_acc_s_f3 ); break;
        case BKNIT_F322: bhvm_hop_ar2_eci_ACC_CASE( f3_t, f2_t, f2_t, bhvm_hop_ar2_eci_add_acc_s_f3 ); break;
        case BKNIT_F323: bhvm_hop_ar2_eci_ACC_CASE( f3_t, f2_t, f3_t, bhvm_hop_ar2_eci_add_acc_s_f3 ); break;
        case BKNIT_F332: bhvm_hop_ar2_eci_ACC_CASE( f3_t, f3_t, f2_t, bhvm_hop_ar2_eci_add_acc_s_f3 ); break;
        case BKNIT_F333: bhvm_hop_ar2_eci_ACC_CASE( f3_t, f3_t, f3_t, bhvm_hop_ar2_eci_add_acc_s_f3 ); break;
        default: BKNIT_FA3_ERR( a->v.type, b->v.type, r->v.type ); break;
    }
    
    #undef bhvm_hop_ar2_eci_ACC_CASE
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_eci_sub_acc_s )
"aware bhvm_hop_ar2_eci"
"{"
"}";

void bhvm_hop_ar2_eci_sub_acc_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a != r && b != r );
    assert(  sz_min( sz_min( a->v.size, b->v.size ), r->v.size ) > 0 );
    sz_t n = sz_gcd( sz_gcd( a->v.size, b->v.size ), r->v.size );
    sz_t m = sz_max( sz_max( a->v.size, b->v.size ), r->v.size ) / n;
    
    #define bhvm_hop_ar2_eci_ACC_CASE( TA_T, TB_T, TR_T, FUNC ) \
            { \
                const TA_T *a0 = a->v.data, *a1 = a0; \
                const TB_T *b0 = b->v.data, *b1 = b0; \
                      TR_T *r0 = r->v.data, *r1 = r0; \
                for( sz_t i = 0; i < m; i++ ) \
                { \
                    for( sz_t i = 0; i < n; i++ ) r1[ i ] += FUNC( a1[ i ], b1[ i ] ); \
                    a1 = ( a1 + n - a0 ) < a->v.size ? ( a1 + n ) : a0; \
                    b1 = ( b1 + n - b0 ) < b->v.size ? ( b1 + n ) : b0; \
                    r1 = ( r1 + n - r0 ) < r->v.size ? ( r1 + n ) : r0; \
                } \
            }
    
    switch( BKNIT_FA3( a->v.type, b->v.type, r->v.type ) )
    {
        case BKNIT_F222: bhvm_hop_ar2_eci_ACC_CASE( f2_t, f2_t, f2_t, bhvm_hop_ar2_eci_sub_acc_s_f2 ); break;
        case BKNIT_F223: bhvm_hop_ar2_eci_ACC_CASE( f2_t, f2_t, f3_t, bhvm_hop_ar2_eci_sub_acc_s_f3 ); break;
        case BKNIT_F232: bhvm_hop_ar2_eci_ACC_CASE( f2_t, f3_t, f2_t, bhvm_hop_ar2_eci_sub_acc_s_f3 ); break;
        case BKNIT_F233: bhvm_hop_ar2_eci_ACC_CASE( f2_t, f3_t, f3_t, bhvm_hop_ar2_eci_sub_acc_s_f3 ); break;
        case BKNIT_F322: bhvm_hop_ar2_eci_ACC_CASE( f3_t, f2_t, f2_t, bhvm_hop_ar2_eci_sub_acc_s_f3 ); break;
        case BKNIT_F323: bhvm_hop_ar2_eci_ACC_CASE( f3_t, f2_t, f3_t, bhvm_hop_ar2_eci_sub_acc_s_f3 ); break;
        case BKNIT_F332: bhvm_hop_ar2_eci_ACC_CASE( f3_t, f3_t, f2_t, bhvm_hop_ar2_eci_sub_acc_s_f3 ); break;
        case BKNIT_F333: bhvm_hop_ar2_eci_ACC_CASE( f3_t, f3_t, f3_t, bhvm_hop_ar2_eci_sub_acc_s_f3 ); break;
        default: BKNIT_FA3_ERR( a->v.type, b->v.type, r->v.type ); break;
    }
    
    #undef bhvm_hop_ar2_eci_ACC_CASE
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_eci_mul_acc_s )
"aware bhvm_hop_ar2_eci"
"{"
"}";

void bhvm_hop_ar2_eci_mul_acc_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a != r && b != r );
    assert(  sz_min( sz_min( a->v.size, b->v.size ), r->v.size ) > 0 );
    sz_t n = sz_gcd( sz_gcd( a->v.size, b->v.size ), r->v.size );
    sz_t m = sz_max( sz_max( a->v.size, b->v.size ), r->v.size ) / n;
    
    #define bhvm_hop_ar2_eci_ACC_CASE( TA_T, TB_T, TR_T, FUNC ) \
            { \
                const TA_T *a0 = a->v.data, *a1 = a0; \
                const TB_T *b0 = b->v.data, *b1 = b0; \
                      TR_T *r0 = r->v.data, *r1 = r0; \
                for( sz_t i = 0; i < m; i++ ) \
                { \
                    for( sz_t i = 0; i < n; i++ ) r1[ i ] += FUNC( a1[ i ], b1[ i ] ); \
                    a1 = ( a1 + n - a0 ) < a->v.size ? ( a1 + n ) : a0; \
                    b1 = ( b1 + n - b0 ) < b->v.size ? ( b1 + n ) : b0; \
                    r1 = ( r1 + n - r0 ) < r->v.size ? ( r1 + n ) : r0; \
                } \
            }
    
    switch( BKNIT_FA3( a->v.type, b->v.type, r->v.type ) )
    {
        case BKNIT_F222: bhvm_hop_ar2_eci_ACC_CASE( f2_t, f2_t, f2_t, bhvm_hop_ar2_eci_mul_acc_s_f2 ); break;
        case BKNIT_F223: bhvm_hop_ar2_eci_ACC_CASE( f2_t, f2_t, f3_t, bhvm_hop_ar2_eci_mul_acc_s_f3 ); break;
        case BKNIT_F232: bhvm_hop_ar2_eci_ACC_CASE( f2_t, f3_t, f2_t, bhvm_hop_ar2_eci_mul_acc_s_f3 ); break;
        case BKNIT_F233: bhvm_hop_ar2_eci_ACC_CASE( f2_t, f3_t, f3_t, bhvm_hop_ar2_eci_mul_acc_s_f3 ); break;
        case BKNIT_F322: bhvm_hop_ar2_eci_ACC_CASE( f3_t, f2_t, f2_t, bhvm_hop_ar2_eci_mul_acc_s_f3 ); break;
        case BKNIT_F323: bhvm_hop_ar2_eci_ACC_CASE( f3_t, f2_t, f3_t, bhvm_hop_ar2_eci_mul_acc_s_f3 ); break;
        case BKNIT_F332: bhvm_hop_ar2_eci_ACC_CASE( f3_t, f3_t, f2_t, bhvm_hop_ar2_eci_mul_acc_s_f3 ); break;
        case BKNIT_F333: bhvm_hop_ar2_eci_ACC_CASE( f3_t, f3_t, f3_t, bhvm_hop_ar2_eci_mul_acc_s_f3 ); break;
        default: BKNIT_FA3_ERR( a->v.type, b->v.type, r->v.type ); break;
    }
    
    #undef bhvm_hop_ar2_eci_ACC_CASE
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_eci_div_acc_s )
"aware bhvm_hop_ar2_eci"
"{"
"}";

void bhvm_hop_ar2_eci_div_acc_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a != r && b != r );
    assert(  sz_min( sz_min( a->v.size, b->v.size ), r->v.size ) > 0 );
    sz_t n = sz_gcd( sz_gcd( a->v.size, b->v.size ), r->v.size );
    sz_t m = sz_max( sz_max( a->v.size, b->v.size ), r->v.size ) / n;
    
    #define bhvm_hop_ar2_eci_ACC_CASE( TA_T, TB_T, TR_T, FUNC ) \
            { \
                const TA_T *a0 = a->v.data, *a1 = a0; \
                const TB_T *b0 = b->v.data, *b1 = b0; \
                      TR_T *r0 = r->v.data, *r1 = r0; \
                for( sz_t i = 0; i < m; i++ ) \
                { \
                    for( sz_t i = 0; i < n; i++ ) r1[ i ] += FUNC( a1[ i ], b1[ i ] ); \
                    a1 = ( a1 + n - a0 ) < a->v.size ? ( a1 + n ) : a0; \
                    b1 = ( b1 + n - b0 ) < b->v.size ? ( b1 + n ) : b0; \
                    r1 = ( r1 + n - r0 ) < r->v.size ? ( r1 + n ) : r0; \
                } \
            }
    
    switch( BKNIT_FA3( a->v.type, b->v.type, r->v.type ) )
    {
        case BKNIT_F222: bhvm_hop_ar2_eci_ACC_CASE( f2_t, f2_t, f2_t, bhvm_hop_ar2_eci_div_acc_s_f2 ); break;
        case BKNIT_F223: bhvm_hop_ar2_eci_ACC_CASE( f2_t, f2_t, f3_t, bhvm_hop_ar2_eci_div_acc_s_f3 ); break;
        case BKNIT_F232: bhvm_hop_ar2_eci_ACC_CASE( f2_t, f3_t, f2_t, bhvm_hop_ar2_eci_div_acc_s_f3 ); break;
        case BKNIT_F233: bhvm_hop_ar2_eci_ACC_CASE( f2_t, f3_t, f3_t, bhvm_hop_ar2_eci_div_acc_s_f3 ); break;
        case BKNIT_F322: bhvm_hop_ar2_eci_ACC_CASE( f3_t, f2_t, f2_t, bhvm_hop_ar2_eci_div_acc_s_f3 ); break;
        case BKNIT_F323: bhvm_hop_ar2_eci_ACC_CASE( f3_t, f2_t, f3_t, bhvm_hop_ar2_eci_div_acc_s_f3 ); break;
        case BKNIT_F332: bhvm_hop_ar2_eci_ACC_CASE( f3_t, f3_t, f2_t, bhvm_hop_ar2_eci_div_acc_s_f3 ); break;
        case BKNIT_F333: bhvm_hop_ar2_eci_ACC_CASE( f3_t, f3_t, f3_t, bhvm_hop_ar2_eci_div_acc_s_f3 ); break;
        default: BKNIT_FA3_ERR( a->v.type, b->v.type, r->v.type ); break;
    }
    
    #undef bhvm_hop_ar2_eci_ACC_CASE
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_eci_pow_acc_s )
"aware bhvm_hop_ar2_eci"
"{"
"}";

void bhvm_hop_ar2_eci_pow_acc_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a != r && b != r );
    assert(  sz_min( sz_min( a->v.size, b->v.size ), r->v.size ) > 0 );
    sz_t n = sz_gcd( sz_gcd( a->v.size, b->v.size ), r->v.size );
    sz_t m = sz_max( sz_max( a->v.size, b->v.size ), r->v.size ) / n;
    
    #define bhvm_hop_ar2_eci_ACC_CASE( TA_T, TB_T, TR_T, FUNC ) \
            { \
                const TA_T *a0 = a->v.data, *a1 = a0; \
                const TB_T *b0 = b->v.data, *b1 = b0; \
                      TR_T *r0 = r->v.data, *r1 = r0; \
                for( sz_t i = 0; i < m; i++ ) \
                { \
                    for( sz_t i = 0; i < n; i++ ) r1[ i ] += FUNC( a1[ i ], b1[ i ] ); \
                    a1 = ( a1 + n - a0 ) < a->v.size ? ( a1 + n ) : a0; \
                    b1 = ( b1 + n - b0 ) < b->v.size ? ( b1 + n ) : b0; \
                    r1 = ( r1 + n - r0 ) < r->v.size ? ( r1 + n ) : r0; \
                } \
            }
    
    switch( BKNIT_FA3( a->v.type, b->v.type, r->v.type ) )
    {
        case BKNIT_F222: bhvm_hop_ar2_eci_ACC_CASE( f2_t, f2_t, f2_t, bhvm_hop_ar2_eci_pow_acc_s_f2 ); break;
        case BKNIT_F223: bhvm_hop_ar2_eci_ACC_CASE( f2_t, f2_t, f3_t, bhvm_hop_ar2_eci_pow_acc_s_f3 ); break;
        case BKNIT_F232: bhvm_hop_ar2_eci_ACC_CASE( f2_t, f3_t, f2_t, bhvm_hop_ar2_eci_pow_acc_s_f3 ); break;
        case BKNIT_F233: bhvm_hop_ar2_eci_ACC_CASE( f2_t, f3_t, f3_t, bhvm_hop_ar2_eci_pow_acc_s_f3 ); break;
        case BKNIT_F322: bhvm_hop_ar2_eci_ACC_CASE( f3_t, f2_t, f2_t, bhvm_hop_ar2_eci_pow_acc_s_f3 ); break;
        case BKNIT_F323: bhvm_hop_ar2_eci_ACC_CASE( f3_t, f2_t, f3_t, bhvm_hop_ar2_eci_pow_acc_s_f3 ); break;
        case BKNIT_F332: bhvm_hop_ar2_eci_ACC_CASE( f3_t, f3_t, f2_t, bhvm_hop_ar2_eci_pow_acc_s_f3 ); break;
        case BKNIT_F333: bhvm_hop_ar2_eci_ACC_CASE( f3_t, f3_t, f3_t, bhvm_hop_ar2_eci_pow_acc_s_f3 ); break;
        default: BKNIT_FA3_ERR( a->v.type, b->v.type, r->v.type ); break;
    }
    
    #undef bhvm_hop_ar2_eci_ACC_CASE
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_eci_add_s )
"aware bhvm_hop_ar2_eci"
"{"
"}";

void bhvm_hop_ar2_eci_add_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    if( a == r || b == r )
    {
        bhvm_holor_s* buf = bhvm_holor_s_create();
        bhvm_hop_ar2_eci_add_acc_s_f( a, b, bhvm_holor_s_fit_size( bhvm_holor_s_copy_shape_type( buf, r ) ) );
        if( r->v.size ) bhvm_value_s_cpy( &buf->v, &r->v );
        bhvm_holor_s_discard( buf );
    }
    else
    {
        bhvm_value_s_zro( &r->v );
        bhvm_hop_ar2_eci_add_acc_s_f( a, b, r );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_eci_sub_s )
"aware bhvm_hop_ar2_eci"
"{"
"}";

void bhvm_hop_ar2_eci_sub_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    if( a == r || b == r )
    {
        bhvm_holor_s* buf = bhvm_holor_s_create();
        bhvm_hop_ar2_eci_sub_acc_s_f( a, b, bhvm_holor_s_fit_size( bhvm_holor_s_copy_shape_type( buf, r ) ) );
        if( r->v.size ) bhvm_value_s_cpy( &buf->v, &r->v );
        bhvm_holor_s_discard( buf );
    }
    else
    {
        bhvm_value_s_zro( &r->v );
        bhvm_hop_ar2_eci_sub_acc_s_f( a, b, r );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_eci_mul_s )
"aware bhvm_hop_ar2_eci"
"{"
"}";

void bhvm_hop_ar2_eci_mul_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    if( a == r || b == r )
    {
        bhvm_holor_s* buf = bhvm_holor_s_create();
        bhvm_hop_ar2_eci_mul_acc_s_f( a, b, bhvm_holor_s_fit_size( bhvm_holor_s_copy_shape_type( buf, r ) ) );
        if( r->v.size ) bhvm_value_s_cpy( &buf->v, &r->v );
        bhvm_holor_s_discard( buf );
    }
    else
    {
        bhvm_value_s_zro( &r->v );
        bhvm_hop_ar2_eci_mul_acc_s_f( a, b, r );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_eci_div_s )
"aware bhvm_hop_ar2_eci"
"{"
"}";

void bhvm_hop_ar2_eci_div_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    if( a == r || b == r )
    {
        bhvm_holor_s* buf = bhvm_holor_s_create();
        bhvm_hop_ar2_eci_div_acc_s_f( a, b, bhvm_holor_s_fit_size( bhvm_holor_s_copy_shape_type( buf, r ) ) );
        if( r->v.size ) bhvm_value_s_cpy( &buf->v, &r->v );
        bhvm_holor_s_discard( buf );
    }
    else
    {
        bhvm_value_s_zro( &r->v );
        bhvm_hop_ar2_eci_div_acc_s_f( a, b, r );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_eci_pow_s )
"aware bhvm_hop_ar2_eci"
"{"
"}";

void bhvm_hop_ar2_eci_pow_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    if( a == r || b == r )
    {
        bhvm_holor_s* buf = bhvm_holor_s_create();
        bhvm_hop_ar2_eci_pow_acc_s_f( a, b, bhvm_holor_s_fit_size( bhvm_holor_s_copy_shape_type( buf, r ) ) );
        if( r->v.size ) bhvm_value_s_cpy( &buf->v, &r->v );
        bhvm_holor_s_discard( buf );
    }
    else
    {
        bhvm_value_s_zro( &r->v );
        bhvm_hop_ar2_eci_pow_acc_s_f( a, b, r );
    }
}

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hop_ar3

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar3_branch_vvvv_s )
"aware bhvm_hop_ar3"
"{"
"}";

void bhvm_hop_ar3_branch_vvvv_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, const bhvm_holor_s* c, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    assert( b->v.size == r->v.size );
    assert( c->v.size == r->v.size );
    bhvm_lop_ar3_branch_vvvv_s_f( BKNIT_FA4( a->v.type, b->v.type, c->v.type, r->v.type ), a->v.data, b->v.data, c->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar3_branch_svvv_s )
"aware bhvm_hop_ar3"
"{"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hop_ar3_eci

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar3_eci_div_dp_zabg_s )
"aware bhvm_hop_ar3_eci"
"{"
"}";

void bhvm_hop_ar3_eci_div_dp_zabg_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, const bhvm_holor_s* c, bhvm_holor_s* r )
{
    assert( a != r && b != r && c != r );
    assert(  sz_min( a->v.size, sz_min( b->v.size, sz_min( c->v.size, r->v.size ) ) ) > 0 );
    sz_t n = sz_gcd( a->v.size, sz_gcd( b->v.size, sz_gcd( c->v.size, r->v.size ) ) );
    sz_t m = sz_max( a->v.size, sz_max( b->v.size, sz_max( c->v.size, r->v.size ) ) ) / n;
    
    #define bhvm_hop_ar3_eci_ACC_CASE( TA_T, TB_T, TC_T, TR_T, FUNC ) \
            { \
                const TA_T *a0 = a->v.data, *a1 = a0; \
                const TB_T *b0 = b->v.data, *b1 = b0; \
                const TC_T *c0 = c->v.data, *c1 = c0; \
                      TR_T *r0 = r->v.data, *r1 = r0; \
                for( sz_t i = 0; i < m; i++ ) \
                { \
                    for( sz_t i = 0; i < n; i++ ) r1[ i ] += FUNC( a1[ i ], b1[ i ], c1[ i ] ); \
                    a1 = ( a1 + n - a0 ) < a->v.size ? ( a1 + n ) : a0; \
                    b1 = ( b1 + n - b0 ) < b->v.size ? ( b1 + n ) : b0; \
                    c1 = ( c1 + n - c0 ) < c->v.size ? ( c1 + n ) : c0; \
                    r1 = ( r1 + n - r0 ) < r->v.size ? ( r1 + n ) : r0; \
                } \
            }
    
    switch( BKNIT_FA4( a->v.type, b->v.type, c->v.type, r->v.type ) )
    {
        case BKNIT_F2222: bhvm_hop_ar3_eci_ACC_CASE( f2_t, f2_t, f2_t, f2_t, bhvm_hop_ar3_eci_div_dp_zabg_s_f2 ); break;
        case BKNIT_F2223: bhvm_hop_ar3_eci_ACC_CASE( f2_t, f2_t, f2_t, f3_t, bhvm_hop_ar3_eci_div_dp_zabg_s_f3 ); break;
        case BKNIT_F2232: bhvm_hop_ar3_eci_ACC_CASE( f2_t, f2_t, f3_t, f2_t, bhvm_hop_ar3_eci_div_dp_zabg_s_f3 ); break;
        case BKNIT_F2233: bhvm_hop_ar3_eci_ACC_CASE( f2_t, f2_t, f3_t, f3_t, bhvm_hop_ar3_eci_div_dp_zabg_s_f3 ); break;
        case BKNIT_F2322: bhvm_hop_ar3_eci_ACC_CASE( f2_t, f3_t, f2_t, f2_t, bhvm_hop_ar3_eci_div_dp_zabg_s_f3 ); break;
        case BKNIT_F2323: bhvm_hop_ar3_eci_ACC_CASE( f2_t, f3_t, f2_t, f3_t, bhvm_hop_ar3_eci_div_dp_zabg_s_f3 ); break;
        case BKNIT_F2332: bhvm_hop_ar3_eci_ACC_CASE( f2_t, f3_t, f3_t, f2_t, bhvm_hop_ar3_eci_div_dp_zabg_s_f3 ); break;
        case BKNIT_F2333: bhvm_hop_ar3_eci_ACC_CASE( f2_t, f3_t, f3_t, f3_t, bhvm_hop_ar3_eci_div_dp_zabg_s_f3 ); break;
        case BKNIT_F3222: bhvm_hop_ar3_eci_ACC_CASE( f3_t, f2_t, f2_t, f2_t, bhvm_hop_ar3_eci_div_dp_zabg_s_f3 ); break;
        case BKNIT_F3223: bhvm_hop_ar3_eci_ACC_CASE( f3_t, f2_t, f2_t, f3_t, bhvm_hop_ar3_eci_div_dp_zabg_s_f3 ); break;
        case BKNIT_F3232: bhvm_hop_ar3_eci_ACC_CASE( f3_t, f2_t, f3_t, f2_t, bhvm_hop_ar3_eci_div_dp_zabg_s_f3 ); break;
        case BKNIT_F3233: bhvm_hop_ar3_eci_ACC_CASE( f3_t, f2_t, f3_t, f3_t, bhvm_hop_ar3_eci_div_dp_zabg_s_f3 ); break;
        case BKNIT_F3322: bhvm_hop_ar3_eci_ACC_CASE( f3_t, f3_t, f2_t, f2_t, bhvm_hop_ar3_eci_div_dp_zabg_s_f3 ); break;
        case BKNIT_F3323: bhvm_hop_ar3_eci_ACC_CASE( f3_t, f3_t, f2_t, f3_t, bhvm_hop_ar3_eci_div_dp_zabg_s_f3 ); break;
        case BKNIT_F3332: bhvm_hop_ar3_eci_ACC_CASE( f3_t, f3_t, f3_t, f2_t, bhvm_hop_ar3_eci_div_dp_zabg_s_f3 ); break;
        case BKNIT_F3333: bhvm_hop_ar3_eci_ACC_CASE( f3_t, f3_t, f3_t, f3_t, bhvm_hop_ar3_eci_div_dp_zabg_s_f3 ); break;
        default: BKNIT_FA4_ERR( a->v.type, b->v.type, c->v.type, r->v.type ); break;
    }
    
    #undef bhvm_hop_ar3_eci_ACC_CASE
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar3_eci_pow_dp_abzf_s )
"aware bhvm_hop_ar3_eci"
"{"
"}";

void bhvm_hop_ar3_eci_pow_dp_abzf_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, const bhvm_holor_s* c, bhvm_holor_s* r )
{
    assert( a != r && b != r && c != r );
    assert(  sz_min( a->v.size, sz_min( b->v.size, sz_min( c->v.size, r->v.size ) ) ) > 0 );
    sz_t n = sz_gcd( a->v.size, sz_gcd( b->v.size, sz_gcd( c->v.size, r->v.size ) ) );
    sz_t m = sz_max( a->v.size, sz_max( b->v.size, sz_max( c->v.size, r->v.size ) ) ) / n;
    
    #define bhvm_hop_ar3_eci_ACC_CASE( TA_T, TB_T, TC_T, TR_T, FUNC ) \
            { \
                const TA_T *a0 = a->v.data, *a1 = a0; \
                const TB_T *b0 = b->v.data, *b1 = b0; \
                const TC_T *c0 = c->v.data, *c1 = c0; \
                      TR_T *r0 = r->v.data, *r1 = r0; \
                for( sz_t i = 0; i < m; i++ ) \
                { \
                    for( sz_t i = 0; i < n; i++ ) r1[ i ] += FUNC( a1[ i ], b1[ i ], c1[ i ] ); \
                    a1 = ( a1 + n - a0 ) < a->v.size ? ( a1 + n ) : a0; \
                    b1 = ( b1 + n - b0 ) < b->v.size ? ( b1 + n ) : b0; \
                    c1 = ( c1 + n - c0 ) < c->v.size ? ( c1 + n ) : c0; \
                    r1 = ( r1 + n - r0 ) < r->v.size ? ( r1 + n ) : r0; \
                } \
            }
    
    switch( BKNIT_FA4( a->v.type, b->v.type, c->v.type, r->v.type ) )
    {
        case BKNIT_F2222: bhvm_hop_ar3_eci_ACC_CASE( f2_t, f2_t, f2_t, f2_t, bhvm_hop_ar3_eci_pow_dp_abzf_s_f2 ); break;
        case BKNIT_F2223: bhvm_hop_ar3_eci_ACC_CASE( f2_t, f2_t, f2_t, f3_t, bhvm_hop_ar3_eci_pow_dp_abzf_s_f3 ); break;
        case BKNIT_F2232: bhvm_hop_ar3_eci_ACC_CASE( f2_t, f2_t, f3_t, f2_t, bhvm_hop_ar3_eci_pow_dp_abzf_s_f3 ); break;
        case BKNIT_F2233: bhvm_hop_ar3_eci_ACC_CASE( f2_t, f2_t, f3_t, f3_t, bhvm_hop_ar3_eci_pow_dp_abzf_s_f3 ); break;
        case BKNIT_F2322: bhvm_hop_ar3_eci_ACC_CASE( f2_t, f3_t, f2_t, f2_t, bhvm_hop_ar3_eci_pow_dp_abzf_s_f3 ); break;
        case BKNIT_F2323: bhvm_hop_ar3_eci_ACC_CASE( f2_t, f3_t, f2_t, f3_t, bhvm_hop_ar3_eci_pow_dp_abzf_s_f3 ); break;
        case BKNIT_F2332: bhvm_hop_ar3_eci_ACC_CASE( f2_t, f3_t, f3_t, f2_t, bhvm_hop_ar3_eci_pow_dp_abzf_s_f3 ); break;
        case BKNIT_F2333: bhvm_hop_ar3_eci_ACC_CASE( f2_t, f3_t, f3_t, f3_t, bhvm_hop_ar3_eci_pow_dp_abzf_s_f3 ); break;
        case BKNIT_F3222: bhvm_hop_ar3_eci_ACC_CASE( f3_t, f2_t, f2_t, f2_t, bhvm_hop_ar3_eci_pow_dp_abzf_s_f3 ); break;
        case BKNIT_F3223: bhvm_hop_ar3_eci_ACC_CASE( f3_t, f2_t, f2_t, f3_t, bhvm_hop_ar3_eci_pow_dp_abzf_s_f3 ); break;
        case BKNIT_F3232: bhvm_hop_ar3_eci_ACC_CASE( f3_t, f2_t, f3_t, f2_t, bhvm_hop_ar3_eci_pow_dp_abzf_s_f3 ); break;
        case BKNIT_F3233: bhvm_hop_ar3_eci_ACC_CASE( f3_t, f2_t, f3_t, f3_t, bhvm_hop_ar3_eci_pow_dp_abzf_s_f3 ); break;
        case BKNIT_F3322: bhvm_hop_ar3_eci_ACC_CASE( f3_t, f3_t, f2_t, f2_t, bhvm_hop_ar3_eci_pow_dp_abzf_s_f3 ); break;
        case BKNIT_F3323: bhvm_hop_ar3_eci_ACC_CASE( f3_t, f3_t, f2_t, f3_t, bhvm_hop_ar3_eci_pow_dp_abzf_s_f3 ); break;
        case BKNIT_F3332: bhvm_hop_ar3_eci_ACC_CASE( f3_t, f3_t, f3_t, f2_t, bhvm_hop_ar3_eci_pow_dp_abzf_s_f3 ); break;
        case BKNIT_F3333: bhvm_hop_ar3_eci_ACC_CASE( f3_t, f3_t, f3_t, f3_t, bhvm_hop_ar3_eci_pow_dp_abzf_s_f3 ); break;
        default: BKNIT_FA4_ERR( a->v.type, b->v.type, c->v.type, r->v.type ); break;
    }
    
    #undef bhvm_hop_ar3_eci_ACC_CASE
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar3_eci_pow_dp_ayzg_s )
"aware bhvm_hop_ar3_eci"
"{"
"}";

void bhvm_hop_ar3_eci_pow_dp_ayzg_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, const bhvm_holor_s* c, bhvm_holor_s* r )
{
    assert( a != r && b != r && c != r );
    assert(  sz_min( a->v.size, sz_min( b->v.size, sz_min( c->v.size, r->v.size ) ) ) > 0 );
    sz_t n = sz_gcd( a->v.size, sz_gcd( b->v.size, sz_gcd( c->v.size, r->v.size ) ) );
    sz_t m = sz_max( a->v.size, sz_max( b->v.size, sz_max( c->v.size, r->v.size ) ) ) / n;
    
    #define bhvm_hop_ar3_eci_ACC_CASE( TA_T, TB_T, TC_T, TR_T, FUNC ) \
            { \
                const TA_T *a0 = a->v.data, *a1 = a0; \
                const TB_T *b0 = b->v.data, *b1 = b0; \
                const TC_T *c0 = c->v.data, *c1 = c0; \
                      TR_T *r0 = r->v.data, *r1 = r0; \
                for( sz_t i = 0; i < m; i++ ) \
                { \
                    for( sz_t i = 0; i < n; i++ ) r1[ i ] += FUNC( a1[ i ], b1[ i ], c1[ i ] ); \
                    a1 = ( a1 + n - a0 ) < a->v.size ? ( a1 + n ) : a0; \
                    b1 = ( b1 + n - b0 ) < b->v.size ? ( b1 + n ) : b0; \
                    c1 = ( c1 + n - c0 ) < c->v.size ? ( c1 + n ) : c0; \
                    r1 = ( r1 + n - r0 ) < r->v.size ? ( r1 + n ) : r0; \
                } \
            }
    
    switch( BKNIT_FA4( a->v.type, b->v.type, c->v.type, r->v.type ) )
    {
        case BKNIT_F2222: bhvm_hop_ar3_eci_ACC_CASE( f2_t, f2_t, f2_t, f2_t, bhvm_hop_ar3_eci_pow_dp_ayzg_s_f2 ); break;
        case BKNIT_F2223: bhvm_hop_ar3_eci_ACC_CASE( f2_t, f2_t, f2_t, f3_t, bhvm_hop_ar3_eci_pow_dp_ayzg_s_f3 ); break;
        case BKNIT_F2232: bhvm_hop_ar3_eci_ACC_CASE( f2_t, f2_t, f3_t, f2_t, bhvm_hop_ar3_eci_pow_dp_ayzg_s_f3 ); break;
        case BKNIT_F2233: bhvm_hop_ar3_eci_ACC_CASE( f2_t, f2_t, f3_t, f3_t, bhvm_hop_ar3_eci_pow_dp_ayzg_s_f3 ); break;
        case BKNIT_F2322: bhvm_hop_ar3_eci_ACC_CASE( f2_t, f3_t, f2_t, f2_t, bhvm_hop_ar3_eci_pow_dp_ayzg_s_f3 ); break;
        case BKNIT_F2323: bhvm_hop_ar3_eci_ACC_CASE( f2_t, f3_t, f2_t, f3_t, bhvm_hop_ar3_eci_pow_dp_ayzg_s_f3 ); break;
        case BKNIT_F2332: bhvm_hop_ar3_eci_ACC_CASE( f2_t, f3_t, f3_t, f2_t, bhvm_hop_ar3_eci_pow_dp_ayzg_s_f3 ); break;
        case BKNIT_F2333: bhvm_hop_ar3_eci_ACC_CASE( f2_t, f3_t, f3_t, f3_t, bhvm_hop_ar3_eci_pow_dp_ayzg_s_f3 ); break;
        case BKNIT_F3222: bhvm_hop_ar3_eci_ACC_CASE( f3_t, f2_t, f2_t, f2_t, bhvm_hop_ar3_eci_pow_dp_ayzg_s_f3 ); break;
        case BKNIT_F3223: bhvm_hop_ar3_eci_ACC_CASE( f3_t, f2_t, f2_t, f3_t, bhvm_hop_ar3_eci_pow_dp_ayzg_s_f3 ); break;
        case BKNIT_F3232: bhvm_hop_ar3_eci_ACC_CASE( f3_t, f2_t, f3_t, f2_t, bhvm_hop_ar3_eci_pow_dp_ayzg_s_f3 ); break;
        case BKNIT_F3233: bhvm_hop_ar3_eci_ACC_CASE( f3_t, f2_t, f3_t, f3_t, bhvm_hop_ar3_eci_pow_dp_ayzg_s_f3 ); break;
        case BKNIT_F3322: bhvm_hop_ar3_eci_ACC_CASE( f3_t, f3_t, f2_t, f2_t, bhvm_hop_ar3_eci_pow_dp_ayzg_s_f3 ); break;
        case BKNIT_F3323: bhvm_hop_ar3_eci_ACC_CASE( f3_t, f3_t, f2_t, f3_t, bhvm_hop_ar3_eci_pow_dp_ayzg_s_f3 ); break;
        case BKNIT_F3332: bhvm_hop_ar3_eci_ACC_CASE( f3_t, f3_t, f3_t, f2_t, bhvm_hop_ar3_eci_pow_dp_ayzg_s_f3 ); break;
        case BKNIT_F3333: bhvm_hop_ar3_eci_ACC_CASE( f3_t, f3_t, f3_t, f3_t, bhvm_hop_ar3_eci_pow_dp_ayzg_s_f3 ); break;
        default: BKNIT_FA4_ERR( a->v.type, b->v.type, c->v.type, r->v.type ); break;
    }
    
    #undef bhvm_hop_ar3_eci_ACC_CASE
}

/**********************************************************************************************************************/
// source: bhvm_mcode.h
#include "bhvm_mcode.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_mcode

BCORE_DEFINE_OBJECT_INST_P( bhvm_mcode_op_s )
"aware bhvm_mcode"
"{"
    "aware bhvm_vop => vop;"
    "private bhvm_vop_s* p;"
    "func bcore_inst_call:copy_x;"
    "func bcore_via_call:mutated;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_mcode_track_s )
"aware bcore_array"
"{"
    "tp_t name;"
    "bhvm_mcode_op_s [];"
    "func bhvm_vop:run;"
"}";

void bhvm_mcode_track_s_vop_push_d( bhvm_mcode_track_s* o, bhvm_vop* vop )
{
    assert( vop );
    bhvm_mcode_op_s* op = bhvm_mcode_track_s_push( o );
    op->vop = vop;
    op->p = ( bhvm_vop_s* )bhvm_vop_s_get_aware( op->vop );
    assert( op->p );
}

void bhvm_mcode_track_s_vop_push_c( bhvm_mcode_track_s* o, const bhvm_vop* vop )
{
    bhvm_mcode_track_s_vop_push_d( o, bhvm_vop_a_clone( vop ) );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_mcode_track_ads_s )
"aware bcore_array"
"{"
    "bhvm_mcode_track_s => [];"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_mcode_lib_s )
"aware bhvm_mcode"
"{"
    "bhvm_mcode_track_ads_s arr;"
    "bcore_hmap_tpuz_s map;"
"}";

bhvm_mcode_track_s* bhvm_mcode_lib_s_track_get( bhvm_mcode_lib_s* o, tp_t name )
{
    uz_t* pidx = bcore_hmap_tpuz_s_get( &o->map, name );
    if( !pidx ) return NULL;
    return o->arr.data[ *pidx ];
}

bhvm_mcode_track_s* bhvm_mcode_lib_s_track_get_or_new( bhvm_mcode_lib_s* o, tp_t name )
{
    if( bcore_hmap_tpuz_s_exists( &o->map, name ) ) return bhvm_mcode_lib_s_track_get( o, name );
    bcore_hmap_tpuz_s_set( &o->map, name, o->arr.size );
    bhvm_mcode_track_s* track = bhvm_mcode_track_ads_s_push( &o->arr );
    track->name = name;
    return track;
}

bhvm_mcode_track_s* bhvm_mcode_lib_s_track_reset( bhvm_mcode_lib_s* o, tp_t name )
{
    bhvm_mcode_track_s* track = bhvm_mcode_lib_s_track_get_or_new( o, name );
    bhvm_mcode_track_s_clear( track );
    return track;
}

void bhvm_mcode_lib_s_track_vop_set_args_push_d( bhvm_mcode_lib_s* o, tp_t name, bhvm_vop* vop, const bhvm_vop_arr_ci_s* arr_ci )
{
    bhvm_vop_a_set_args( vop, arr_ci );
    bhvm_mcode_track_s_vop_push_d( bhvm_mcode_lib_s_track_get_or_new( o, name ), vop );
}

void bhvm_mcode_lib_s_track_push( bhvm_mcode_lib_s* o, tp_t name, tp_t src_name )
{
    bhvm_mcode_track_s* src = bhvm_mcode_lib_s_track_get( o, src_name );
    if( !src ) return;
    bhvm_mcode_track_s* dst = bhvm_mcode_lib_s_track_get_or_new( o, name );
    BFOR_EACH( i, src ) bhvm_mcode_track_s_vop_push_c( dst, src->data[ i ].vop );
}

void bhvm_mcode_lib_s_track_push_reverse( bhvm_mcode_lib_s* o, tp_t name, tp_t src_name )
{
    bhvm_mcode_track_s* src = bhvm_mcode_lib_s_track_get( o, src_name );
    if( !src ) return;
    bhvm_mcode_track_s* dst = bhvm_mcode_lib_s_track_get_or_new( o, name );
    for( sz_t i = src->size - 1; i >= 0; i-- ) bhvm_mcode_track_s_vop_push_c( dst, src->data[ i ].vop );
}

void bhvm_mcode_lib_s_track_remove( bhvm_mcode_lib_s* o, tp_t name )
{
    uz_t* pidx = bcore_hmap_tpuz_s_get( &o->map, name );
    if( !pidx ) return;
    sz_t idx = *pidx;
    bhvm_mcode_track_s_discard( o->arr.data[ idx ] );
    o->arr.size--;
    o->arr.data[ idx ] = o->arr.data[ o->arr.size ];
    o->arr.data[ o->arr.size ] = NULL;
    if( idx < o->arr.size ) bcore_hmap_tpuz_s_set( &o->map, o->arr.data[ idx ]->name, idx );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_mcode_frame_s )
"aware bhvm_mcode"
"{"
    "bhvm_mcode_lib_s => lib;"
    "bhvm_mcode_hbase_s => hbase;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_mcode_hmeta

BCORE_DEFINE_OBJECT_INST_P( bhvm_mcode_hmeta_adl_s )
"aware bcore_array"
"{"
    "aware bhvm_mcode_hmeta=> [];"
"}";

BCORE_DEFINE_SPECT( bhvm_mcode, bhvm_mcode_hmeta )
"{"
    "bcore_spect_header_s header;"
    "feature aware bhvm_mcode_hmeta : do_nothing;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_mcode_hbase

BCORE_DEFINE_OBJECT_INST_P( bhvm_mcode_hbase_s )
"aware bhvm_mcode_hbase"
"{"
    "bhvm_holor_ads_s holor_ads;"
    "bhvm_mcode_hmeta_adl_s hmeta_adl;"
"}";

bhvm_mcode_hbase_s* bhvm_mcode_hbase_s_set_size( bhvm_mcode_hbase_s* o, sz_t size )
{
    bhvm_holor_ads_s_set_size( &o->holor_ads, size  );
    bhvm_mcode_hmeta_adl_s_set_size( &o->hmeta_adl, size  );
    return o;
}

sz_t bhvm_mcode_hbase_s_push_hm( bhvm_mcode_hbase_s* o, const bhvm_holor_s* h, const bhvm_mcode_hmeta* m )
{
    sz_t idx = o->holor_ads.size;
    bhvm_mcode_hmeta_adl_s_push_c( &o->hmeta_adl, m );
    bhvm_holor_ads_s_push_c( &o->holor_ads, h );
    return idx;
}

sz_t bhvm_mcode_hbase_s_push_hmc( bhvm_mcode_hbase_s* o, const bhvm_holor_s* h, const bhvm_mcode_hmeta* m, char c, bhvm_vop_arr_ci_s* arr_ci )
{
    bhvm_vop_ci_s ci;
    bhvm_vop_ci_s_init( &ci );
    ci.c = c;
    ci.i = bhvm_mcode_hbase_s_push_hm( o, h, m );
    *bhvm_vop_arr_ci_s_push( arr_ci ) = ci;
    return ci.i;
}

/**********************************************************************************************************************/

vd_t bhvm_planted_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bhvm_planted" ) ) )
    {
        case TYPEOF_init1:
        {
            // Comment or remove line below to rebuild this target.
            bcore_const_x_set_d( typeof( "bhvm_planted_hash" ), sr_tp( 3606087739 ) );

            // --------------------------------------------------------------------
            // source: bhvm_holor.h

            // group: bhvm
            BCORE_REGISTER_OBJECT( bhvm_shape_s );
            BCORE_REGISTER_OBJECT( bhvm_value_s );
            BCORE_REGISTER_FFUNC( bcore_fp_copy_typed, bhvm_holor_s_copy_typed );
            BCORE_REGISTER_OBJECT( bhvm_holor_s );
            BCORE_REGISTER_OBJECT( bhvm_holor_adl_s );
            BCORE_REGISTER_OBJECT( bhvm_holor_ads_s );
            BCORE_REGISTER_OBJECT( bhvm_holor_mdl_s );
            BCORE_REGISTER_TRAIT( bhvm, bcore_inst );

            // --------------------------------------------------------------------
            // source: bhvm_hop.h

            // group: bhvm_hop
            BCORE_REGISTER_TRAIT( bhvm_hop, bcore_inst );

            // group: bhvm_hop_ar0
            BCORE_REGISTER_OBJECT( bhvm_hop_ar0_zro_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar0_one_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar0_nul_dp_s );
            BCORE_REGISTER_TRAIT( bhvm_hop_ar0, bhvm_hop );

            // group: bhvm_hop_ar1
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_cpy_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_identity_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_neg_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_floor_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_ceil_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_exp_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_log_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_inv_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_abs_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_sig_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_l1_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_sqr_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_srt_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_sqrsum_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_lgst_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_lgst_hard_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_lgst_leaky_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_tanh_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_tanh_hard_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_tanh_leaky_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_softplus_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_relu_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_relu_leaky_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_identity_dp_zf_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_neg_dp_zf_s );
            BCORE_REGISTER_TRAIT( bhvm_hop_ar1, bhvm_hop );

            // group: bhvm_hop_ar1_eci
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_eci_cpy_acc_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_eci_neg_acc_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_eci_inv_acc_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_eci_cpy_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_eci_neg_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_eci_inv_s );
            BCORE_REGISTER_TRAIT( bhvm_hop_ar1_eci, bhvm_hop );

            // group: bhvm_hop_ar2
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_sub_sqrsum_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_sub_l1_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_equal_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_larger_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_smaller_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_larger_equal_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_smaller_equal_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_logic_and_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_logic_or_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_exp_dp_zyf_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_log_dp_zaf_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_inv_dp_zyf_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_sqr_dp_zaf_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_srt_dp_zyf_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_lgst_dp_zyf_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_lgst_hard_dp_zyf_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_lgst_leaky_dp_zyf_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_tanh_dp_zyf_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_tanh_hard_dp_zyf_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_tanh_leaky_dp_zyf_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_softplus_dp_zyf_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_relu_dp_zyf_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_relu_leaky_dp_zyf_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_branch_vvvv_dp_azg_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_branch_vvvv_dp_azh_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_mvv_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_vmv_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_vvm_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_tvv_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_vtv_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_mmm_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_mtm_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_tmm_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_ttm_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_acc_mvv_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_acc_vmv_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_acc_tvv_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_acc_vtv_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_acc_vvm_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_acc_mmm_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_acc_mtm_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_acc_tmm_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_acc_ttm_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_cat_s );
            BCORE_REGISTER_TRAIT( bhvm_hop_ar2, bhvm_hop );

            // group: bhvm_hop_ar2_eci
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_eci_add_acc_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_eci_sub_acc_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_eci_mul_acc_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_eci_div_acc_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_eci_pow_acc_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_eci_add_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_eci_sub_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_eci_mul_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_eci_div_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_eci_pow_s );
            BCORE_REGISTER_TRAIT( bhvm_hop_ar2_eci, bhvm_hop );

            // group: bhvm_hop_ar3
            BCORE_REGISTER_OBJECT( bhvm_hop_ar3_branch_vvvv_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar3_branch_svvv_s );
            BCORE_REGISTER_TRAIT( bhvm_hop_ar3, bhvm_hop );

            // group: bhvm_hop_ar3_eci
            BCORE_REGISTER_OBJECT( bhvm_hop_ar3_eci_div_dp_zabg_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar3_eci_pow_dp_abzf_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar3_eci_pow_dp_ayzg_s );
            BCORE_REGISTER_TRAIT( bhvm_hop_ar3_eci, bhvm_hop );

            // --------------------------------------------------------------------
            // source: bhvm_mcode.h

            // group: bhvm_mcode
            BCORE_REGISTER_FFUNC( bcore_inst_call_copy_x, bhvm_mcode_op_s_copy_x );
            BCORE_REGISTER_FFUNC( bcore_via_call_mutated, bhvm_mcode_op_s_mutated );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_mcode_op_s_run );
            BCORE_REGISTER_OBJECT( bhvm_mcode_op_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_mcode_track_s_run );
            BCORE_REGISTER_OBJECT( bhvm_mcode_track_s );
            BCORE_REGISTER_OBJECT( bhvm_mcode_track_ads_s );
            BCORE_REGISTER_OBJECT( bhvm_mcode_lib_s );
            BCORE_REGISTER_OBJECT( bhvm_mcode_frame_s );
            BCORE_REGISTER_TRAIT( bhvm_mcode, bcore_inst );

            // group: bhvm_mcode_hmeta
            BCORE_REGISTER_FEATURE( bhvm_mcode_hmeta_do_nothing );
            BCORE_REGISTER_OBJECT( bhvm_mcode_hmeta_adl_s );
            BCORE_REGISTER_SPECT( bhvm_mcode_hmeta );

            // group: bhvm_mcode_hbase
            BCORE_REGISTER_OBJECT( bhvm_mcode_hbase_s );
            BCORE_REGISTER_TRAIT( bhvm_mcode_hbase, bhvm_mcode );
        }
        break;
        default: break;
    }
    return NULL;
}
