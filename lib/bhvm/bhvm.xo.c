/** This file was generated from xoila source code.
 *  Compiling Agent : xoico_compiler (C) 2020 J.B.Steffens
 *  Last File Update: 2020-12-01T10:47:37Z
 *
 *  Copyright and License of this File:
 *
 *  Generated code inherits the copyright and license of the underlying xoila source code.
 *  Source code defining this file is distributed across following files:
 *
 *  bhvm_holor.h
 *  bhvm_hop.h
 *  bhvm_mcode.h
 *  bhvm_hop.x
 *
 */

#include "bhvm.xo.h"
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
    "func bcore_fp:copy_typed;"
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

BCORE_DEFINE_OBJECT_INST_P( bhvm_stats_s )
"aware bhvm"
"{"
    "f3_t min;"
    "f3_t max;"
    "f3_t sum;"
    "f3_t sqr_sum;"
    "sz_t size;"
"}";

XOILA_DEFINE_SPECT( bcore_inst, bhvm )
"{"
    "bcore_spect_header_s header;"
"}";

/**********************************************************************************************************************/
// source: bhvm_hop.h
#include "bhvm_hop.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hop; embeds: bhvm_hop.x

XOILA_DEFINE_SPECT( bcore_inst, bhvm_hop )
"{"
    "bcore_spect_header_s header;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hop_ar0

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar0_zro_s )
"aware bhvm_hop_ar0"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar0_one_s )
"aware bhvm_hop_ar0"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar0_nul_dp_s )
"aware bhvm_hop_ar0"
"{"
"}";

XOILA_DEFINE_SPECT( bhvm_hop, bhvm_hop_ar0 )
"{"
    "bcore_spect_header_s header;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hop_ar1

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_l1_s )
"aware bhvm_hop_ar1"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_sqrsum_s )
"aware bhvm_hop_ar1"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_cpy_s )
"aware bhvm_hop_ar1"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_neg_s )
"aware bhvm_hop_ar1"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_floor_s )
"aware bhvm_hop_ar1"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_ceil_s )
"aware bhvm_hop_ar1"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_exp_s )
"aware bhvm_hop_ar1"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_log_s )
"aware bhvm_hop_ar1"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_inv_s )
"aware bhvm_hop_ar1"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_abs_s )
"aware bhvm_hop_ar1"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_sig_s )
"aware bhvm_hop_ar1"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_sqr_s )
"aware bhvm_hop_ar1"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_srt_s )
"aware bhvm_hop_ar1"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_sigm_s )
"aware bhvm_hop_ar1"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_sigm_hard_s )
"aware bhvm_hop_ar1"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_sigm_leaky_s )
"aware bhvm_hop_ar1"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_tanh_s )
"aware bhvm_hop_ar1"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_tanh_hard_s )
"aware bhvm_hop_ar1"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_tanh_leaky_s )
"aware bhvm_hop_ar1"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_softplus_s )
"aware bhvm_hop_ar1"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_relu_s )
"aware bhvm_hop_ar1"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_relu_leaky_s )
"aware bhvm_hop_ar1"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_softmax_s )
"aware bhvm_hop_ar1"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_neg_dp_zf_s )
"aware bhvm_hop_ar1"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_cat_dp_zf_s )
"aware bhvm_hop_ar1"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_cat_dp_zg_s )
"aware bhvm_hop_ar1"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_ccat_dp_zf_s )
"aware bhvm_hop_ar1"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_ccat_dp_zg_s )
"aware bhvm_hop_ar1"
"{"
"}";

XOILA_DEFINE_SPECT( bhvm_hop, bhvm_hop_ar1 )
"{"
    "bcore_spect_header_s header;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hop_ar1_eci

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_eci_cpy_acc_s )
"aware bhvm_hop_ar1_eci"
"{"
"}";

void bhvm_hop_ar1_eci_cpy_acc_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    // bhvm_hop.x:103:5
    
    assert( a != r );
    assert(  sz_min( a->v.size, r->v.size ) > 0 );
    sz_t n = sz_gcd( a->v.size, r->v.size );
    sz_t m = sz_max( a->v.size, r->v.size ) / n;
    
    
    #define BHVM_ACC_CASE( TA_T, TR_T, FUNC ) \
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
        case BKNIT_F22: BHVM_ACC_CASE( f2_t, f2_t, bhvm_hop_ar1_eci_cpy_acc_s_f2 ); break;
        case BKNIT_F23: BHVM_ACC_CASE( f2_t, f3_t, bhvm_hop_ar1_eci_cpy_acc_s_f3 ); break;
        case BKNIT_F32: BHVM_ACC_CASE( f3_t, f2_t, bhvm_hop_ar1_eci_cpy_acc_s_f3 ); break;
        case BKNIT_F33: BHVM_ACC_CASE( f3_t, f3_t, bhvm_hop_ar1_eci_cpy_acc_s_f3 ); break;
        default: BKNIT_FA2_ERR( a->v.type, r->v.type ); break;
    }
    
    #undef BHVM_ACC_CASE
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_eci_neg_acc_s )
"aware bhvm_hop_ar1_eci"
"{"
"}";

void bhvm_hop_ar1_eci_neg_acc_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    // bhvm_hop.x:103:5
    
    assert( a != r );
    assert(  sz_min( a->v.size, r->v.size ) > 0 );
    sz_t n = sz_gcd( a->v.size, r->v.size );
    sz_t m = sz_max( a->v.size, r->v.size ) / n;
    
    
    #define BHVM_ACC_CASE( TA_T, TR_T, FUNC ) \
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
        case BKNIT_F22: BHVM_ACC_CASE( f2_t, f2_t, bhvm_hop_ar1_eci_neg_acc_s_f2 ); break;
        case BKNIT_F23: BHVM_ACC_CASE( f2_t, f3_t, bhvm_hop_ar1_eci_neg_acc_s_f3 ); break;
        case BKNIT_F32: BHVM_ACC_CASE( f3_t, f2_t, bhvm_hop_ar1_eci_neg_acc_s_f3 ); break;
        case BKNIT_F33: BHVM_ACC_CASE( f3_t, f3_t, bhvm_hop_ar1_eci_neg_acc_s_f3 ); break;
        default: BKNIT_FA2_ERR( a->v.type, r->v.type ); break;
    }
    
    #undef BHVM_ACC_CASE
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_eci_inv_acc_s )
"aware bhvm_hop_ar1_eci"
"{"
"}";

void bhvm_hop_ar1_eci_inv_acc_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    // bhvm_hop.x:103:5
    
    assert( a != r );
    assert(  sz_min( a->v.size, r->v.size ) > 0 );
    sz_t n = sz_gcd( a->v.size, r->v.size );
    sz_t m = sz_max( a->v.size, r->v.size ) / n;
    
    
    #define BHVM_ACC_CASE( TA_T, TR_T, FUNC ) \
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
        case BKNIT_F22: BHVM_ACC_CASE( f2_t, f2_t, bhvm_hop_ar1_eci_inv_acc_s_f2 ); break;
        case BKNIT_F23: BHVM_ACC_CASE( f2_t, f3_t, bhvm_hop_ar1_eci_inv_acc_s_f3 ); break;
        case BKNIT_F32: BHVM_ACC_CASE( f3_t, f2_t, bhvm_hop_ar1_eci_inv_acc_s_f3 ); break;
        case BKNIT_F33: BHVM_ACC_CASE( f3_t, f3_t, bhvm_hop_ar1_eci_inv_acc_s_f3 ); break;
        default: BKNIT_FA2_ERR( a->v.type, r->v.type ); break;
    }
    
    #undef BHVM_ACC_CASE
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_eci_cpy_s )
"aware bhvm_hop_ar1_eci"
"{"
"}";

void bhvm_hop_ar1_eci_cpy_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    // bhvm_hop.x:143:9
    
    if( a == r )
    {
        bhvm_holor_s* buf = bhvm_holor_s_create();
        bhvm_hop_ar1_eci_cpy_acc_s_f(a, bhvm_holor_s_fit_size( bhvm_holor_s_copy_shape_type( buf,r ) ) );
        if( r->v.size ) bhvm_value_s_cpy( &(buf->v),(&(r->v)) );
        bhvm_holor_s_discard(buf);
    }
    else
    {
        bhvm_value_s_zro( &(r->v));
        bhvm_hop_ar1_eci_cpy_acc_s_f(a, r );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_eci_neg_s )
"aware bhvm_hop_ar1_eci"
"{"
"}";

void bhvm_hop_ar1_eci_neg_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    // bhvm_hop.x:164:9
    
    if( a == r )
    {
        bhvm_holor_s* buf = bhvm_holor_s_create();
        bhvm_hop_ar1_eci_neg_acc_s_f(a, bhvm_holor_s_fit_size( bhvm_holor_s_copy_shape_type( buf,r ) ) );
        if( r->v.size ) bhvm_value_s_cpy( &(buf->v),(&(r->v)) );
        bhvm_holor_s_discard(buf);
    }
    else
    {
        bhvm_value_s_zro( &(r->v));
        bhvm_hop_ar1_eci_neg_acc_s_f(a, r );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_eci_inv_s )
"aware bhvm_hop_ar1_eci"
"{"
"}";

void bhvm_hop_ar1_eci_inv_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    // bhvm_hop.x:185:9
    
    if( a == r )
    {
        bhvm_holor_s* buf = bhvm_holor_s_create();
        bhvm_hop_ar1_eci_inv_acc_s_f(a, bhvm_holor_s_fit_size( bhvm_holor_s_copy_shape_type( buf,r ) ) );
        if( r->v.size ) bhvm_value_s_cpy( &(buf->v),(&(r->v)) );
        bhvm_holor_s_discard(buf);
    }
    else
    {
        bhvm_value_s_zro( &(r->v));
        bhvm_hop_ar1_eci_inv_acc_s_f(a, r );
    }
}

XOILA_DEFINE_SPECT( bhvm_hop, bhvm_hop_ar1_eci )
"{"
    "bcore_spect_header_s header;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hop_ar2

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_sub_sqrsum_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_sub_l1_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_equal_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_unequal_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_larger_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_smaller_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_larger_equal_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_smaller_equal_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_logic_and_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_logic_or_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_abs_dp_zaf_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_exp_dp_zyf_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_log_dp_zaf_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_inv_dp_zyf_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_sqr_dp_zaf_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_srt_dp_zyf_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_sigm_dp_zyf_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_sigm_hard_dp_zyf_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_sigm_leaky_dp_zyf_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_tanh_dp_zyf_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_tanh_hard_dp_zyf_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_tanh_leaky_dp_zyf_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_softplus_dp_zyf_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_relu_dp_zyf_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_relu_leaky_dp_zyf_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_softmax_dp_zyf_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_mvv_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_vmv_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_vvm_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_tvv_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_vtv_s )
"aware bhvm_hop_ar2"
"{"
"}";

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

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_acc_vmv_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_acc_tvv_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_acc_vtv_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_acc_vvm_s )
"aware bhvm_hop_ar2"
"{"
"}";

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

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_ccat_s )
"aware bhvm_hop_ar2"
"{"
"}";

XOILA_DEFINE_SPECT( bhvm_hop, bhvm_hop_ar2 )
"{"
    "bcore_spect_header_s header;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hop_ar2_eci

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_eci_add_acc_s )
"aware bhvm_hop_ar2_eci"
"{"
"}";

void bhvm_hop_ar2_eci_add_acc_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    // bhvm_hop.x:328:5
    
    assert( a != r && b != r );
    assert(  sz_min( sz_min( a->v.size, b->v.size ), r->v.size ) > 0 );
    sz_t n = sz_gcd( sz_gcd( a->v.size, b->v.size ), r->v.size );
    sz_t m = sz_max( sz_max( a->v.size, b->v.size ), r->v.size ) / n;
    
    #define BHVM_ACC_CASE( TA_T, TB_T, TR_T, FUNC ) \
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
        case BKNIT_F222: BHVM_ACC_CASE( f2_t, f2_t, f2_t, bhvm_hop_ar2_eci_add_acc_s_f2 ); break;
        case BKNIT_F223: BHVM_ACC_CASE( f2_t, f2_t, f3_t, bhvm_hop_ar2_eci_add_acc_s_f3 ); break;
        case BKNIT_F232: BHVM_ACC_CASE( f2_t, f3_t, f2_t, bhvm_hop_ar2_eci_add_acc_s_f3 ); break;
        case BKNIT_F233: BHVM_ACC_CASE( f2_t, f3_t, f3_t, bhvm_hop_ar2_eci_add_acc_s_f3 ); break;
        case BKNIT_F322: BHVM_ACC_CASE( f3_t, f2_t, f2_t, bhvm_hop_ar2_eci_add_acc_s_f3 ); break;
        case BKNIT_F323: BHVM_ACC_CASE( f3_t, f2_t, f3_t, bhvm_hop_ar2_eci_add_acc_s_f3 ); break;
        case BKNIT_F332: BHVM_ACC_CASE( f3_t, f3_t, f2_t, bhvm_hop_ar2_eci_add_acc_s_f3 ); break;
        case BKNIT_F333: BHVM_ACC_CASE( f3_t, f3_t, f3_t, bhvm_hop_ar2_eci_add_acc_s_f3 ); break;
        default: BKNIT_FA3_ERR( a->v.type, b->v.type, r->v.type ); break;
    }
    
    #undef BHVM_ACC_CASE
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_eci_sub_acc_s )
"aware bhvm_hop_ar2_eci"
"{"
"}";

void bhvm_hop_ar2_eci_sub_acc_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    // bhvm_hop.x:328:5
    
    assert( a != r && b != r );
    assert(  sz_min( sz_min( a->v.size, b->v.size ), r->v.size ) > 0 );
    sz_t n = sz_gcd( sz_gcd( a->v.size, b->v.size ), r->v.size );
    sz_t m = sz_max( sz_max( a->v.size, b->v.size ), r->v.size ) / n;
    
    #define BHVM_ACC_CASE( TA_T, TB_T, TR_T, FUNC ) \
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
        case BKNIT_F222: BHVM_ACC_CASE( f2_t, f2_t, f2_t, bhvm_hop_ar2_eci_sub_acc_s_f2 ); break;
        case BKNIT_F223: BHVM_ACC_CASE( f2_t, f2_t, f3_t, bhvm_hop_ar2_eci_sub_acc_s_f3 ); break;
        case BKNIT_F232: BHVM_ACC_CASE( f2_t, f3_t, f2_t, bhvm_hop_ar2_eci_sub_acc_s_f3 ); break;
        case BKNIT_F233: BHVM_ACC_CASE( f2_t, f3_t, f3_t, bhvm_hop_ar2_eci_sub_acc_s_f3 ); break;
        case BKNIT_F322: BHVM_ACC_CASE( f3_t, f2_t, f2_t, bhvm_hop_ar2_eci_sub_acc_s_f3 ); break;
        case BKNIT_F323: BHVM_ACC_CASE( f3_t, f2_t, f3_t, bhvm_hop_ar2_eci_sub_acc_s_f3 ); break;
        case BKNIT_F332: BHVM_ACC_CASE( f3_t, f3_t, f2_t, bhvm_hop_ar2_eci_sub_acc_s_f3 ); break;
        case BKNIT_F333: BHVM_ACC_CASE( f3_t, f3_t, f3_t, bhvm_hop_ar2_eci_sub_acc_s_f3 ); break;
        default: BKNIT_FA3_ERR( a->v.type, b->v.type, r->v.type ); break;
    }
    
    #undef BHVM_ACC_CASE
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_eci_mul_acc_s )
"aware bhvm_hop_ar2_eci"
"{"
"}";

void bhvm_hop_ar2_eci_mul_acc_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    // bhvm_hop.x:328:5
    
    assert( a != r && b != r );
    assert(  sz_min( sz_min( a->v.size, b->v.size ), r->v.size ) > 0 );
    sz_t n = sz_gcd( sz_gcd( a->v.size, b->v.size ), r->v.size );
    sz_t m = sz_max( sz_max( a->v.size, b->v.size ), r->v.size ) / n;
    
    #define BHVM_ACC_CASE( TA_T, TB_T, TR_T, FUNC ) \
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
        case BKNIT_F222: BHVM_ACC_CASE( f2_t, f2_t, f2_t, bhvm_hop_ar2_eci_mul_acc_s_f2 ); break;
        case BKNIT_F223: BHVM_ACC_CASE( f2_t, f2_t, f3_t, bhvm_hop_ar2_eci_mul_acc_s_f3 ); break;
        case BKNIT_F232: BHVM_ACC_CASE( f2_t, f3_t, f2_t, bhvm_hop_ar2_eci_mul_acc_s_f3 ); break;
        case BKNIT_F233: BHVM_ACC_CASE( f2_t, f3_t, f3_t, bhvm_hop_ar2_eci_mul_acc_s_f3 ); break;
        case BKNIT_F322: BHVM_ACC_CASE( f3_t, f2_t, f2_t, bhvm_hop_ar2_eci_mul_acc_s_f3 ); break;
        case BKNIT_F323: BHVM_ACC_CASE( f3_t, f2_t, f3_t, bhvm_hop_ar2_eci_mul_acc_s_f3 ); break;
        case BKNIT_F332: BHVM_ACC_CASE( f3_t, f3_t, f2_t, bhvm_hop_ar2_eci_mul_acc_s_f3 ); break;
        case BKNIT_F333: BHVM_ACC_CASE( f3_t, f3_t, f3_t, bhvm_hop_ar2_eci_mul_acc_s_f3 ); break;
        default: BKNIT_FA3_ERR( a->v.type, b->v.type, r->v.type ); break;
    }
    
    #undef BHVM_ACC_CASE
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_eci_div_acc_s )
"aware bhvm_hop_ar2_eci"
"{"
"}";

void bhvm_hop_ar2_eci_div_acc_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    // bhvm_hop.x:328:5
    
    assert( a != r && b != r );
    assert(  sz_min( sz_min( a->v.size, b->v.size ), r->v.size ) > 0 );
    sz_t n = sz_gcd( sz_gcd( a->v.size, b->v.size ), r->v.size );
    sz_t m = sz_max( sz_max( a->v.size, b->v.size ), r->v.size ) / n;
    
    #define BHVM_ACC_CASE( TA_T, TB_T, TR_T, FUNC ) \
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
        case BKNIT_F222: BHVM_ACC_CASE( f2_t, f2_t, f2_t, bhvm_hop_ar2_eci_div_acc_s_f2 ); break;
        case BKNIT_F223: BHVM_ACC_CASE( f2_t, f2_t, f3_t, bhvm_hop_ar2_eci_div_acc_s_f3 ); break;
        case BKNIT_F232: BHVM_ACC_CASE( f2_t, f3_t, f2_t, bhvm_hop_ar2_eci_div_acc_s_f3 ); break;
        case BKNIT_F233: BHVM_ACC_CASE( f2_t, f3_t, f3_t, bhvm_hop_ar2_eci_div_acc_s_f3 ); break;
        case BKNIT_F322: BHVM_ACC_CASE( f3_t, f2_t, f2_t, bhvm_hop_ar2_eci_div_acc_s_f3 ); break;
        case BKNIT_F323: BHVM_ACC_CASE( f3_t, f2_t, f3_t, bhvm_hop_ar2_eci_div_acc_s_f3 ); break;
        case BKNIT_F332: BHVM_ACC_CASE( f3_t, f3_t, f2_t, bhvm_hop_ar2_eci_div_acc_s_f3 ); break;
        case BKNIT_F333: BHVM_ACC_CASE( f3_t, f3_t, f3_t, bhvm_hop_ar2_eci_div_acc_s_f3 ); break;
        default: BKNIT_FA3_ERR( a->v.type, b->v.type, r->v.type ); break;
    }
    
    #undef BHVM_ACC_CASE
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_eci_pow_acc_s )
"aware bhvm_hop_ar2_eci"
"{"
"}";

void bhvm_hop_ar2_eci_pow_acc_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    // bhvm_hop.x:328:5
    
    assert( a != r && b != r );
    assert(  sz_min( sz_min( a->v.size, b->v.size ), r->v.size ) > 0 );
    sz_t n = sz_gcd( sz_gcd( a->v.size, b->v.size ), r->v.size );
    sz_t m = sz_max( sz_max( a->v.size, b->v.size ), r->v.size ) / n;
    
    #define BHVM_ACC_CASE( TA_T, TB_T, TR_T, FUNC ) \
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
        case BKNIT_F222: BHVM_ACC_CASE( f2_t, f2_t, f2_t, bhvm_hop_ar2_eci_pow_acc_s_f2 ); break;
        case BKNIT_F223: BHVM_ACC_CASE( f2_t, f2_t, f3_t, bhvm_hop_ar2_eci_pow_acc_s_f3 ); break;
        case BKNIT_F232: BHVM_ACC_CASE( f2_t, f3_t, f2_t, bhvm_hop_ar2_eci_pow_acc_s_f3 ); break;
        case BKNIT_F233: BHVM_ACC_CASE( f2_t, f3_t, f3_t, bhvm_hop_ar2_eci_pow_acc_s_f3 ); break;
        case BKNIT_F322: BHVM_ACC_CASE( f3_t, f2_t, f2_t, bhvm_hop_ar2_eci_pow_acc_s_f3 ); break;
        case BKNIT_F323: BHVM_ACC_CASE( f3_t, f2_t, f3_t, bhvm_hop_ar2_eci_pow_acc_s_f3 ); break;
        case BKNIT_F332: BHVM_ACC_CASE( f3_t, f3_t, f2_t, bhvm_hop_ar2_eci_pow_acc_s_f3 ); break;
        case BKNIT_F333: BHVM_ACC_CASE( f3_t, f3_t, f3_t, bhvm_hop_ar2_eci_pow_acc_s_f3 ); break;
        default: BKNIT_FA3_ERR( a->v.type, b->v.type, r->v.type ); break;
    }
    
    #undef BHVM_ACC_CASE
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_eci_add_s )
"aware bhvm_hop_ar2_eci"
"{"
"}";

void bhvm_hop_ar2_eci_add_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    // bhvm_hop.x:375:9
    
    if( a == r || b == r )
    {
        bhvm_holor_s* buf = bhvm_holor_s_create();
        bhvm_hop_ar2_eci_add_acc_s_f(a, b, bhvm_holor_s_fit_size( bhvm_holor_s_copy_shape_type( buf,r ) ) );
        if( r->v.size ) bhvm_value_s_cpy( &(buf->v),(&(r->v)) );
        bhvm_holor_s_discard(buf);
    }
    else
    {
        bhvm_value_s_zro( &(r->v));
        bhvm_hop_ar2_eci_add_acc_s_f(a, b, r );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_eci_sub_s )
"aware bhvm_hop_ar2_eci"
"{"
"}";

void bhvm_hop_ar2_eci_sub_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    // bhvm_hop.x:394:9
    
    if( a == r || b == r )
    {
        bhvm_holor_s* buf = bhvm_holor_s_create();
        bhvm_hop_ar2_eci_sub_acc_s_f(a, b, bhvm_holor_s_fit_size( bhvm_holor_s_copy_shape_type( buf,r ) ) );
        if( r->v.size ) bhvm_value_s_cpy( &(buf->v),(&(r->v)) );
        bhvm_holor_s_discard(buf);
    }
    else
    {
         bhvm_value_s_zro( &(r->v));
        bhvm_hop_ar2_eci_sub_acc_s_f(a, b, r );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_eci_mul_s )
"aware bhvm_hop_ar2_eci"
"{"
"}";

void bhvm_hop_ar2_eci_mul_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    // bhvm_hop.x:413:9
    
    if( a == r || b == r )
    {
        bhvm_holor_s* buf = bhvm_holor_s_create();
        bhvm_hop_ar2_eci_mul_acc_s_f(a, b, bhvm_holor_s_fit_size( bhvm_holor_s_copy_shape_type( buf,r ) ) );
        if( r->v.size ) bhvm_value_s_cpy( &(buf->v),(&(r->v)) );
        bhvm_holor_s_discard(buf);
    }
    else
    {
        bhvm_value_s_zro( &(r->v));
        bhvm_hop_ar2_eci_mul_acc_s_f(a, b, r );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_eci_div_s )
"aware bhvm_hop_ar2_eci"
"{"
"}";

void bhvm_hop_ar2_eci_div_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    // bhvm_hop.x:432:9
    
    if( a == r || b == r )
    {
        bhvm_holor_s* buf = bhvm_holor_s_create();
        bhvm_hop_ar2_eci_div_acc_s_f(a, b, bhvm_holor_s_fit_size( bhvm_holor_s_copy_shape_type( buf,r ) ) );
        if( r->v.size ) bhvm_value_s_cpy( &(buf->v),(&(r->v)) );
        bhvm_holor_s_discard(buf);
    }
    else
    {
        bhvm_value_s_zro( &(r->v));
        bhvm_hop_ar2_eci_div_acc_s_f(a, b, r );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_eci_pow_s )
"aware bhvm_hop_ar2_eci"
"{"
"}";

void bhvm_hop_ar2_eci_pow_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    // bhvm_hop.x:451:9
    
    if( a == r || b == r )
    {
        bhvm_holor_s* buf = bhvm_holor_s_create();
        bhvm_hop_ar2_eci_pow_acc_s_f(a, b, bhvm_holor_s_fit_size( bhvm_holor_s_copy_shape_type( buf,r ) ) );
        if( r->v.size ) bhvm_value_s_cpy( &(buf->v),(&(r->v)) );
        bhvm_holor_s_discard(buf);
    }
    else
    {
        bhvm_value_s_zro( &(r->v));
        bhvm_hop_ar2_eci_pow_acc_s_f(a, b, r );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_eci_iff_dp_b_azg_s )
"aware bhvm_hop_ar2_eci"
"{"
"}";

void bhvm_hop_ar2_eci_iff_dp_b_azg_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    // bhvm_hop.x:328:5
    
    assert( a != r && b != r );
    assert(  sz_min( sz_min( a->v.size, b->v.size ), r->v.size ) > 0 );
    sz_t n = sz_gcd( sz_gcd( a->v.size, b->v.size ), r->v.size );
    sz_t m = sz_max( sz_max( a->v.size, b->v.size ), r->v.size ) / n;
    
    #define BHVM_ACC_CASE( TA_T, TB_T, TR_T, FUNC ) \
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
        case BKNIT_F222: BHVM_ACC_CASE( f2_t, f2_t, f2_t, bhvm_hop_ar2_eci_iff_dp_b_azg_s_f2 ); break;
        case BKNIT_F223: BHVM_ACC_CASE( f2_t, f2_t, f3_t, bhvm_hop_ar2_eci_iff_dp_b_azg_s_f3 ); break;
        case BKNIT_F232: BHVM_ACC_CASE( f2_t, f3_t, f2_t, bhvm_hop_ar2_eci_iff_dp_b_azg_s_f3 ); break;
        case BKNIT_F233: BHVM_ACC_CASE( f2_t, f3_t, f3_t, bhvm_hop_ar2_eci_iff_dp_b_azg_s_f3 ); break;
        case BKNIT_F322: BHVM_ACC_CASE( f3_t, f2_t, f2_t, bhvm_hop_ar2_eci_iff_dp_b_azg_s_f3 ); break;
        case BKNIT_F323: BHVM_ACC_CASE( f3_t, f2_t, f3_t, bhvm_hop_ar2_eci_iff_dp_b_azg_s_f3 ); break;
        case BKNIT_F332: BHVM_ACC_CASE( f3_t, f3_t, f2_t, bhvm_hop_ar2_eci_iff_dp_b_azg_s_f3 ); break;
        case BKNIT_F333: BHVM_ACC_CASE( f3_t, f3_t, f3_t, bhvm_hop_ar2_eci_iff_dp_b_azg_s_f3 ); break;
        default: BKNIT_FA3_ERR( a->v.type, b->v.type, r->v.type ); break;
    }
    
    #undef BHVM_ACC_CASE
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_eci_iff_dp_c_azh_s )
"aware bhvm_hop_ar2_eci"
"{"
"}";

void bhvm_hop_ar2_eci_iff_dp_c_azh_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    // bhvm_hop.x:328:5
    
    assert( a != r && b != r );
    assert(  sz_min( sz_min( a->v.size, b->v.size ), r->v.size ) > 0 );
    sz_t n = sz_gcd( sz_gcd( a->v.size, b->v.size ), r->v.size );
    sz_t m = sz_max( sz_max( a->v.size, b->v.size ), r->v.size ) / n;
    
    #define BHVM_ACC_CASE( TA_T, TB_T, TR_T, FUNC ) \
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
        case BKNIT_F222: BHVM_ACC_CASE( f2_t, f2_t, f2_t, bhvm_hop_ar2_eci_iff_dp_c_azh_s_f2 ); break;
        case BKNIT_F223: BHVM_ACC_CASE( f2_t, f2_t, f3_t, bhvm_hop_ar2_eci_iff_dp_c_azh_s_f3 ); break;
        case BKNIT_F232: BHVM_ACC_CASE( f2_t, f3_t, f2_t, bhvm_hop_ar2_eci_iff_dp_c_azh_s_f3 ); break;
        case BKNIT_F233: BHVM_ACC_CASE( f2_t, f3_t, f3_t, bhvm_hop_ar2_eci_iff_dp_c_azh_s_f3 ); break;
        case BKNIT_F322: BHVM_ACC_CASE( f3_t, f2_t, f2_t, bhvm_hop_ar2_eci_iff_dp_c_azh_s_f3 ); break;
        case BKNIT_F323: BHVM_ACC_CASE( f3_t, f2_t, f3_t, bhvm_hop_ar2_eci_iff_dp_c_azh_s_f3 ); break;
        case BKNIT_F332: BHVM_ACC_CASE( f3_t, f3_t, f2_t, bhvm_hop_ar2_eci_iff_dp_c_azh_s_f3 ); break;
        case BKNIT_F333: BHVM_ACC_CASE( f3_t, f3_t, f3_t, bhvm_hop_ar2_eci_iff_dp_c_azh_s_f3 ); break;
        default: BKNIT_FA3_ERR( a->v.type, b->v.type, r->v.type ); break;
    }
    
    #undef BHVM_ACC_CASE
}

XOILA_DEFINE_SPECT( bhvm_hop, bhvm_hop_ar2_eci )
"{"
    "bcore_spect_header_s header;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hop_ar2_eci_logic

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_eci_logic_equal_s )
"aware bhvm_hop_ar2_eci_logic"
"{"
"}";

void bhvm_hop_ar2_eci_logic_equal_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    // bhvm_hop.x:290:5
    
    assert( a != r && b != r );
    assert(  sz_min( sz_min( a->v.size, b->v.size ), r->v.size ) > 0 );
    sz_t n = sz_gcd( sz_gcd( a->v.size, b->v.size ), r->v.size );
    sz_t m = sz_max( sz_max( a->v.size, b->v.size ), r->v.size ) / n;
    
    #define BHVM_SET_CASE( TA_T, TB_T, TR_T, FUNC ) \
    { \
        const TA_T *a0 = a->v.data, *a1 = a0; \
        const TB_T *b0 = b->v.data, *b1 = b0; \
              TR_T *r0 = r->v.data, *r1 = r0; \
        for( sz_t i = 0; i < m; i++ ) \
        { \
            for( sz_t i = 0; i < n; i++ ) r1[ i ] = FUNC( a1[ i ], b1[ i ] ); \
            a1 = ( a1 + n - a0 ) < a->v.size ? ( a1 + n ) : a0; \
            b1 = ( b1 + n - b0 ) < b->v.size ? ( b1 + n ) : b0; \
            r1 = ( r1 + n - r0 ) < r->v.size ? ( r1 + n ) : r0; \
        } \
    }
    
    switch( BKNIT_FA3( a->v.type, b->v.type, r->v.type ) )
    {
        case BKNIT_F222: BHVM_SET_CASE( f2_t, f2_t, f2_t, bhvm_hop_ar2_eci_logic_equal_s_f2 ); break;
        case BKNIT_F223: BHVM_SET_CASE( f2_t, f2_t, f3_t, bhvm_hop_ar2_eci_logic_equal_s_f3 ); break;
        case BKNIT_F232: BHVM_SET_CASE( f2_t, f3_t, f2_t, bhvm_hop_ar2_eci_logic_equal_s_f3 ); break;
        case BKNIT_F233: BHVM_SET_CASE( f2_t, f3_t, f3_t, bhvm_hop_ar2_eci_logic_equal_s_f3 ); break;
        case BKNIT_F322: BHVM_SET_CASE( f3_t, f2_t, f2_t, bhvm_hop_ar2_eci_logic_equal_s_f3 ); break;
        case BKNIT_F323: BHVM_SET_CASE( f3_t, f2_t, f3_t, bhvm_hop_ar2_eci_logic_equal_s_f3 ); break;
        case BKNIT_F332: BHVM_SET_CASE( f3_t, f3_t, f2_t, bhvm_hop_ar2_eci_logic_equal_s_f3 ); break;
        case BKNIT_F333: BHVM_SET_CASE( f3_t, f3_t, f3_t, bhvm_hop_ar2_eci_logic_equal_s_f3 ); break;
        default: BKNIT_FA3_ERR( a->v.type, b->v.type, r->v.type ); break;
    }
    
    #undef BHVM_SET_CASE
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_eci_logic_unequal_s )
"aware bhvm_hop_ar2_eci_logic"
"{"
"}";

void bhvm_hop_ar2_eci_logic_unequal_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    // bhvm_hop.x:290:5
    
    assert( a != r && b != r );
    assert(  sz_min( sz_min( a->v.size, b->v.size ), r->v.size ) > 0 );
    sz_t n = sz_gcd( sz_gcd( a->v.size, b->v.size ), r->v.size );
    sz_t m = sz_max( sz_max( a->v.size, b->v.size ), r->v.size ) / n;
    
    #define BHVM_SET_CASE( TA_T, TB_T, TR_T, FUNC ) \
    { \
        const TA_T *a0 = a->v.data, *a1 = a0; \
        const TB_T *b0 = b->v.data, *b1 = b0; \
              TR_T *r0 = r->v.data, *r1 = r0; \
        for( sz_t i = 0; i < m; i++ ) \
        { \
            for( sz_t i = 0; i < n; i++ ) r1[ i ] = FUNC( a1[ i ], b1[ i ] ); \
            a1 = ( a1 + n - a0 ) < a->v.size ? ( a1 + n ) : a0; \
            b1 = ( b1 + n - b0 ) < b->v.size ? ( b1 + n ) : b0; \
            r1 = ( r1 + n - r0 ) < r->v.size ? ( r1 + n ) : r0; \
        } \
    }
    
    switch( BKNIT_FA3( a->v.type, b->v.type, r->v.type ) )
    {
        case BKNIT_F222: BHVM_SET_CASE( f2_t, f2_t, f2_t, bhvm_hop_ar2_eci_logic_unequal_s_f2 ); break;
        case BKNIT_F223: BHVM_SET_CASE( f2_t, f2_t, f3_t, bhvm_hop_ar2_eci_logic_unequal_s_f3 ); break;
        case BKNIT_F232: BHVM_SET_CASE( f2_t, f3_t, f2_t, bhvm_hop_ar2_eci_logic_unequal_s_f3 ); break;
        case BKNIT_F233: BHVM_SET_CASE( f2_t, f3_t, f3_t, bhvm_hop_ar2_eci_logic_unequal_s_f3 ); break;
        case BKNIT_F322: BHVM_SET_CASE( f3_t, f2_t, f2_t, bhvm_hop_ar2_eci_logic_unequal_s_f3 ); break;
        case BKNIT_F323: BHVM_SET_CASE( f3_t, f2_t, f3_t, bhvm_hop_ar2_eci_logic_unequal_s_f3 ); break;
        case BKNIT_F332: BHVM_SET_CASE( f3_t, f3_t, f2_t, bhvm_hop_ar2_eci_logic_unequal_s_f3 ); break;
        case BKNIT_F333: BHVM_SET_CASE( f3_t, f3_t, f3_t, bhvm_hop_ar2_eci_logic_unequal_s_f3 ); break;
        default: BKNIT_FA3_ERR( a->v.type, b->v.type, r->v.type ); break;
    }
    
    #undef BHVM_SET_CASE
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_eci_logic_larger_s )
"aware bhvm_hop_ar2_eci_logic"
"{"
"}";

void bhvm_hop_ar2_eci_logic_larger_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    // bhvm_hop.x:290:5
    
    assert( a != r && b != r );
    assert(  sz_min( sz_min( a->v.size, b->v.size ), r->v.size ) > 0 );
    sz_t n = sz_gcd( sz_gcd( a->v.size, b->v.size ), r->v.size );
    sz_t m = sz_max( sz_max( a->v.size, b->v.size ), r->v.size ) / n;
    
    #define BHVM_SET_CASE( TA_T, TB_T, TR_T, FUNC ) \
    { \
        const TA_T *a0 = a->v.data, *a1 = a0; \
        const TB_T *b0 = b->v.data, *b1 = b0; \
              TR_T *r0 = r->v.data, *r1 = r0; \
        for( sz_t i = 0; i < m; i++ ) \
        { \
            for( sz_t i = 0; i < n; i++ ) r1[ i ] = FUNC( a1[ i ], b1[ i ] ); \
            a1 = ( a1 + n - a0 ) < a->v.size ? ( a1 + n ) : a0; \
            b1 = ( b1 + n - b0 ) < b->v.size ? ( b1 + n ) : b0; \
            r1 = ( r1 + n - r0 ) < r->v.size ? ( r1 + n ) : r0; \
        } \
    }
    
    switch( BKNIT_FA3( a->v.type, b->v.type, r->v.type ) )
    {
        case BKNIT_F222: BHVM_SET_CASE( f2_t, f2_t, f2_t, bhvm_hop_ar2_eci_logic_larger_s_f2 ); break;
        case BKNIT_F223: BHVM_SET_CASE( f2_t, f2_t, f3_t, bhvm_hop_ar2_eci_logic_larger_s_f3 ); break;
        case BKNIT_F232: BHVM_SET_CASE( f2_t, f3_t, f2_t, bhvm_hop_ar2_eci_logic_larger_s_f3 ); break;
        case BKNIT_F233: BHVM_SET_CASE( f2_t, f3_t, f3_t, bhvm_hop_ar2_eci_logic_larger_s_f3 ); break;
        case BKNIT_F322: BHVM_SET_CASE( f3_t, f2_t, f2_t, bhvm_hop_ar2_eci_logic_larger_s_f3 ); break;
        case BKNIT_F323: BHVM_SET_CASE( f3_t, f2_t, f3_t, bhvm_hop_ar2_eci_logic_larger_s_f3 ); break;
        case BKNIT_F332: BHVM_SET_CASE( f3_t, f3_t, f2_t, bhvm_hop_ar2_eci_logic_larger_s_f3 ); break;
        case BKNIT_F333: BHVM_SET_CASE( f3_t, f3_t, f3_t, bhvm_hop_ar2_eci_logic_larger_s_f3 ); break;
        default: BKNIT_FA3_ERR( a->v.type, b->v.type, r->v.type ); break;
    }
    
    #undef BHVM_SET_CASE
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_eci_logic_smaller_s )
"aware bhvm_hop_ar2_eci_logic"
"{"
"}";

void bhvm_hop_ar2_eci_logic_smaller_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    // bhvm_hop.x:290:5
    
    assert( a != r && b != r );
    assert(  sz_min( sz_min( a->v.size, b->v.size ), r->v.size ) > 0 );
    sz_t n = sz_gcd( sz_gcd( a->v.size, b->v.size ), r->v.size );
    sz_t m = sz_max( sz_max( a->v.size, b->v.size ), r->v.size ) / n;
    
    #define BHVM_SET_CASE( TA_T, TB_T, TR_T, FUNC ) \
    { \
        const TA_T *a0 = a->v.data, *a1 = a0; \
        const TB_T *b0 = b->v.data, *b1 = b0; \
              TR_T *r0 = r->v.data, *r1 = r0; \
        for( sz_t i = 0; i < m; i++ ) \
        { \
            for( sz_t i = 0; i < n; i++ ) r1[ i ] = FUNC( a1[ i ], b1[ i ] ); \
            a1 = ( a1 + n - a0 ) < a->v.size ? ( a1 + n ) : a0; \
            b1 = ( b1 + n - b0 ) < b->v.size ? ( b1 + n ) : b0; \
            r1 = ( r1 + n - r0 ) < r->v.size ? ( r1 + n ) : r0; \
        } \
    }
    
    switch( BKNIT_FA3( a->v.type, b->v.type, r->v.type ) )
    {
        case BKNIT_F222: BHVM_SET_CASE( f2_t, f2_t, f2_t, bhvm_hop_ar2_eci_logic_smaller_s_f2 ); break;
        case BKNIT_F223: BHVM_SET_CASE( f2_t, f2_t, f3_t, bhvm_hop_ar2_eci_logic_smaller_s_f3 ); break;
        case BKNIT_F232: BHVM_SET_CASE( f2_t, f3_t, f2_t, bhvm_hop_ar2_eci_logic_smaller_s_f3 ); break;
        case BKNIT_F233: BHVM_SET_CASE( f2_t, f3_t, f3_t, bhvm_hop_ar2_eci_logic_smaller_s_f3 ); break;
        case BKNIT_F322: BHVM_SET_CASE( f3_t, f2_t, f2_t, bhvm_hop_ar2_eci_logic_smaller_s_f3 ); break;
        case BKNIT_F323: BHVM_SET_CASE( f3_t, f2_t, f3_t, bhvm_hop_ar2_eci_logic_smaller_s_f3 ); break;
        case BKNIT_F332: BHVM_SET_CASE( f3_t, f3_t, f2_t, bhvm_hop_ar2_eci_logic_smaller_s_f3 ); break;
        case BKNIT_F333: BHVM_SET_CASE( f3_t, f3_t, f3_t, bhvm_hop_ar2_eci_logic_smaller_s_f3 ); break;
        default: BKNIT_FA3_ERR( a->v.type, b->v.type, r->v.type ); break;
    }
    
    #undef BHVM_SET_CASE
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_eci_logic_larger_equal_s )
"aware bhvm_hop_ar2_eci_logic"
"{"
"}";

void bhvm_hop_ar2_eci_logic_larger_equal_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    // bhvm_hop.x:290:5
    
    assert( a != r && b != r );
    assert(  sz_min( sz_min( a->v.size, b->v.size ), r->v.size ) > 0 );
    sz_t n = sz_gcd( sz_gcd( a->v.size, b->v.size ), r->v.size );
    sz_t m = sz_max( sz_max( a->v.size, b->v.size ), r->v.size ) / n;
    
    #define BHVM_SET_CASE( TA_T, TB_T, TR_T, FUNC ) \
    { \
        const TA_T *a0 = a->v.data, *a1 = a0; \
        const TB_T *b0 = b->v.data, *b1 = b0; \
              TR_T *r0 = r->v.data, *r1 = r0; \
        for( sz_t i = 0; i < m; i++ ) \
        { \
            for( sz_t i = 0; i < n; i++ ) r1[ i ] = FUNC( a1[ i ], b1[ i ] ); \
            a1 = ( a1 + n - a0 ) < a->v.size ? ( a1 + n ) : a0; \
            b1 = ( b1 + n - b0 ) < b->v.size ? ( b1 + n ) : b0; \
            r1 = ( r1 + n - r0 ) < r->v.size ? ( r1 + n ) : r0; \
        } \
    }
    
    switch( BKNIT_FA3( a->v.type, b->v.type, r->v.type ) )
    {
        case BKNIT_F222: BHVM_SET_CASE( f2_t, f2_t, f2_t, bhvm_hop_ar2_eci_logic_larger_equal_s_f2 ); break;
        case BKNIT_F223: BHVM_SET_CASE( f2_t, f2_t, f3_t, bhvm_hop_ar2_eci_logic_larger_equal_s_f3 ); break;
        case BKNIT_F232: BHVM_SET_CASE( f2_t, f3_t, f2_t, bhvm_hop_ar2_eci_logic_larger_equal_s_f3 ); break;
        case BKNIT_F233: BHVM_SET_CASE( f2_t, f3_t, f3_t, bhvm_hop_ar2_eci_logic_larger_equal_s_f3 ); break;
        case BKNIT_F322: BHVM_SET_CASE( f3_t, f2_t, f2_t, bhvm_hop_ar2_eci_logic_larger_equal_s_f3 ); break;
        case BKNIT_F323: BHVM_SET_CASE( f3_t, f2_t, f3_t, bhvm_hop_ar2_eci_logic_larger_equal_s_f3 ); break;
        case BKNIT_F332: BHVM_SET_CASE( f3_t, f3_t, f2_t, bhvm_hop_ar2_eci_logic_larger_equal_s_f3 ); break;
        case BKNIT_F333: BHVM_SET_CASE( f3_t, f3_t, f3_t, bhvm_hop_ar2_eci_logic_larger_equal_s_f3 ); break;
        default: BKNIT_FA3_ERR( a->v.type, b->v.type, r->v.type ); break;
    }
    
    #undef BHVM_SET_CASE
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_eci_logic_smaller_equal_s )
"aware bhvm_hop_ar2_eci_logic"
"{"
"}";

void bhvm_hop_ar2_eci_logic_smaller_equal_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    // bhvm_hop.x:290:5
    
    assert( a != r && b != r );
    assert(  sz_min( sz_min( a->v.size, b->v.size ), r->v.size ) > 0 );
    sz_t n = sz_gcd( sz_gcd( a->v.size, b->v.size ), r->v.size );
    sz_t m = sz_max( sz_max( a->v.size, b->v.size ), r->v.size ) / n;
    
    #define BHVM_SET_CASE( TA_T, TB_T, TR_T, FUNC ) \
    { \
        const TA_T *a0 = a->v.data, *a1 = a0; \
        const TB_T *b0 = b->v.data, *b1 = b0; \
              TR_T *r0 = r->v.data, *r1 = r0; \
        for( sz_t i = 0; i < m; i++ ) \
        { \
            for( sz_t i = 0; i < n; i++ ) r1[ i ] = FUNC( a1[ i ], b1[ i ] ); \
            a1 = ( a1 + n - a0 ) < a->v.size ? ( a1 + n ) : a0; \
            b1 = ( b1 + n - b0 ) < b->v.size ? ( b1 + n ) : b0; \
            r1 = ( r1 + n - r0 ) < r->v.size ? ( r1 + n ) : r0; \
        } \
    }
    
    switch( BKNIT_FA3( a->v.type, b->v.type, r->v.type ) )
    {
        case BKNIT_F222: BHVM_SET_CASE( f2_t, f2_t, f2_t, bhvm_hop_ar2_eci_logic_smaller_equal_s_f2 ); break;
        case BKNIT_F223: BHVM_SET_CASE( f2_t, f2_t, f3_t, bhvm_hop_ar2_eci_logic_smaller_equal_s_f3 ); break;
        case BKNIT_F232: BHVM_SET_CASE( f2_t, f3_t, f2_t, bhvm_hop_ar2_eci_logic_smaller_equal_s_f3 ); break;
        case BKNIT_F233: BHVM_SET_CASE( f2_t, f3_t, f3_t, bhvm_hop_ar2_eci_logic_smaller_equal_s_f3 ); break;
        case BKNIT_F322: BHVM_SET_CASE( f3_t, f2_t, f2_t, bhvm_hop_ar2_eci_logic_smaller_equal_s_f3 ); break;
        case BKNIT_F323: BHVM_SET_CASE( f3_t, f2_t, f3_t, bhvm_hop_ar2_eci_logic_smaller_equal_s_f3 ); break;
        case BKNIT_F332: BHVM_SET_CASE( f3_t, f3_t, f2_t, bhvm_hop_ar2_eci_logic_smaller_equal_s_f3 ); break;
        case BKNIT_F333: BHVM_SET_CASE( f3_t, f3_t, f3_t, bhvm_hop_ar2_eci_logic_smaller_equal_s_f3 ); break;
        default: BKNIT_FA3_ERR( a->v.type, b->v.type, r->v.type ); break;
    }
    
    #undef BHVM_SET_CASE
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_eci_logic_and_s )
"aware bhvm_hop_ar2_eci_logic"
"{"
"}";

void bhvm_hop_ar2_eci_logic_and_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    // bhvm_hop.x:290:5
    
    assert( a != r && b != r );
    assert(  sz_min( sz_min( a->v.size, b->v.size ), r->v.size ) > 0 );
    sz_t n = sz_gcd( sz_gcd( a->v.size, b->v.size ), r->v.size );
    sz_t m = sz_max( sz_max( a->v.size, b->v.size ), r->v.size ) / n;
    
    #define BHVM_SET_CASE( TA_T, TB_T, TR_T, FUNC ) \
    { \
        const TA_T *a0 = a->v.data, *a1 = a0; \
        const TB_T *b0 = b->v.data, *b1 = b0; \
              TR_T *r0 = r->v.data, *r1 = r0; \
        for( sz_t i = 0; i < m; i++ ) \
        { \
            for( sz_t i = 0; i < n; i++ ) r1[ i ] = FUNC( a1[ i ], b1[ i ] ); \
            a1 = ( a1 + n - a0 ) < a->v.size ? ( a1 + n ) : a0; \
            b1 = ( b1 + n - b0 ) < b->v.size ? ( b1 + n ) : b0; \
            r1 = ( r1 + n - r0 ) < r->v.size ? ( r1 + n ) : r0; \
        } \
    }
    
    switch( BKNIT_FA3( a->v.type, b->v.type, r->v.type ) )
    {
        case BKNIT_F222: BHVM_SET_CASE( f2_t, f2_t, f2_t, bhvm_hop_ar2_eci_logic_and_s_f2 ); break;
        case BKNIT_F223: BHVM_SET_CASE( f2_t, f2_t, f3_t, bhvm_hop_ar2_eci_logic_and_s_f3 ); break;
        case BKNIT_F232: BHVM_SET_CASE( f2_t, f3_t, f2_t, bhvm_hop_ar2_eci_logic_and_s_f3 ); break;
        case BKNIT_F233: BHVM_SET_CASE( f2_t, f3_t, f3_t, bhvm_hop_ar2_eci_logic_and_s_f3 ); break;
        case BKNIT_F322: BHVM_SET_CASE( f3_t, f2_t, f2_t, bhvm_hop_ar2_eci_logic_and_s_f3 ); break;
        case BKNIT_F323: BHVM_SET_CASE( f3_t, f2_t, f3_t, bhvm_hop_ar2_eci_logic_and_s_f3 ); break;
        case BKNIT_F332: BHVM_SET_CASE( f3_t, f3_t, f2_t, bhvm_hop_ar2_eci_logic_and_s_f3 ); break;
        case BKNIT_F333: BHVM_SET_CASE( f3_t, f3_t, f3_t, bhvm_hop_ar2_eci_logic_and_s_f3 ); break;
        default: BKNIT_FA3_ERR( a->v.type, b->v.type, r->v.type ); break;
    }
    
    #undef BHVM_SET_CASE
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_eci_logic_or_s )
"aware bhvm_hop_ar2_eci_logic"
"{"
"}";

void bhvm_hop_ar2_eci_logic_or_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    // bhvm_hop.x:290:5
    
    assert( a != r && b != r );
    assert(  sz_min( sz_min( a->v.size, b->v.size ), r->v.size ) > 0 );
    sz_t n = sz_gcd( sz_gcd( a->v.size, b->v.size ), r->v.size );
    sz_t m = sz_max( sz_max( a->v.size, b->v.size ), r->v.size ) / n;
    
    #define BHVM_SET_CASE( TA_T, TB_T, TR_T, FUNC ) \
    { \
        const TA_T *a0 = a->v.data, *a1 = a0; \
        const TB_T *b0 = b->v.data, *b1 = b0; \
              TR_T *r0 = r->v.data, *r1 = r0; \
        for( sz_t i = 0; i < m; i++ ) \
        { \
            for( sz_t i = 0; i < n; i++ ) r1[ i ] = FUNC( a1[ i ], b1[ i ] ); \
            a1 = ( a1 + n - a0 ) < a->v.size ? ( a1 + n ) : a0; \
            b1 = ( b1 + n - b0 ) < b->v.size ? ( b1 + n ) : b0; \
            r1 = ( r1 + n - r0 ) < r->v.size ? ( r1 + n ) : r0; \
        } \
    }
    
    switch( BKNIT_FA3( a->v.type, b->v.type, r->v.type ) )
    {
        case BKNIT_F222: BHVM_SET_CASE( f2_t, f2_t, f2_t, bhvm_hop_ar2_eci_logic_or_s_f2 ); break;
        case BKNIT_F223: BHVM_SET_CASE( f2_t, f2_t, f3_t, bhvm_hop_ar2_eci_logic_or_s_f3 ); break;
        case BKNIT_F232: BHVM_SET_CASE( f2_t, f3_t, f2_t, bhvm_hop_ar2_eci_logic_or_s_f3 ); break;
        case BKNIT_F233: BHVM_SET_CASE( f2_t, f3_t, f3_t, bhvm_hop_ar2_eci_logic_or_s_f3 ); break;
        case BKNIT_F322: BHVM_SET_CASE( f3_t, f2_t, f2_t, bhvm_hop_ar2_eci_logic_or_s_f3 ); break;
        case BKNIT_F323: BHVM_SET_CASE( f3_t, f2_t, f3_t, bhvm_hop_ar2_eci_logic_or_s_f3 ); break;
        case BKNIT_F332: BHVM_SET_CASE( f3_t, f3_t, f2_t, bhvm_hop_ar2_eci_logic_or_s_f3 ); break;
        case BKNIT_F333: BHVM_SET_CASE( f3_t, f3_t, f3_t, bhvm_hop_ar2_eci_logic_or_s_f3 ); break;
        default: BKNIT_FA3_ERR( a->v.type, b->v.type, r->v.type ); break;
    }
    
    #undef BHVM_SET_CASE
}

XOILA_DEFINE_SPECT( bhvm_hop_ar2_eci, bhvm_hop_ar2_eci_logic )
"{"
    "bcore_spect_header_s header;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hop_ar3

XOILA_DEFINE_SPECT( bhvm_hop, bhvm_hop_ar3 )
"{"
    "bcore_spect_header_s header;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hop_ar3_eci

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar3_eci_iff_s )
"aware bhvm_hop_ar3_eci"
"{"
"}";

void bhvm_hop_ar3_eci_iff_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, const bhvm_holor_s* c, bhvm_holor_s* r )
{
    // bhvm_hop.x:528:5
    
    assert( a != r && b != r && c != r );
    assert(  sz_min( a->v.size, sz_min( b->v.size, sz_min( c->v.size, r->v.size ) ) ) > 0 );
    sz_t n = sz_gcd( a->v.size, sz_gcd( b->v.size, sz_gcd( c->v.size, r->v.size ) ) );
    sz_t m = sz_max( a->v.size, sz_max( b->v.size, sz_max( c->v.size, r->v.size ) ) ) / n;
    
    #define BHVM_SET_CASE( TA_T, TB_T, TC_T, TR_T, FUNC ) \
    { \
        const TA_T *a0 = a->v.data, *a1 = a0; \
        const TB_T *b0 = b->v.data, *b1 = b0; \
        const TC_T *c0 = c->v.data, *c1 = c0; \
              TR_T *r0 = r->v.data, *r1 = r0; \
        for( sz_t i = 0; i < m; i++ ) \
        { \
            for( sz_t i = 0; i < n; i++ ) r1[ i ] = FUNC( a1[ i ], b1[ i ], c1[ i ] ); \
            a1 = ( a1 + n - a0 ) < a->v.size ? ( a1 + n ) : a0; \
            b1 = ( b1 + n - b0 ) < b->v.size ? ( b1 + n ) : b0; \
            c1 = ( c1 + n - c0 ) < c->v.size ? ( c1 + n ) : c0; \
            r1 = ( r1 + n - r0 ) < r->v.size ? ( r1 + n ) : r0; \
        } \
    }
    
    switch( BKNIT_FA4( a->v.type, b->v.type, c->v.type, r->v.type ) )
    {
        case BKNIT_F2222: BHVM_SET_CASE( f2_t, f2_t, f2_t, f2_t, bhvm_hop_ar3_eci_iff_s_f2 ); break;
        case BKNIT_F2223: BHVM_SET_CASE( f2_t, f2_t, f2_t, f3_t, bhvm_hop_ar3_eci_iff_s_f3 ); break;
        case BKNIT_F2232: BHVM_SET_CASE( f2_t, f2_t, f3_t, f2_t, bhvm_hop_ar3_eci_iff_s_f3 ); break;
        case BKNIT_F2233: BHVM_SET_CASE( f2_t, f2_t, f3_t, f3_t, bhvm_hop_ar3_eci_iff_s_f3 ); break;
        case BKNIT_F2322: BHVM_SET_CASE( f2_t, f3_t, f2_t, f2_t, bhvm_hop_ar3_eci_iff_s_f3 ); break;
        case BKNIT_F2323: BHVM_SET_CASE( f2_t, f3_t, f2_t, f3_t, bhvm_hop_ar3_eci_iff_s_f3 ); break;
        case BKNIT_F2332: BHVM_SET_CASE( f2_t, f3_t, f3_t, f2_t, bhvm_hop_ar3_eci_iff_s_f3 ); break;
        case BKNIT_F2333: BHVM_SET_CASE( f2_t, f3_t, f3_t, f3_t, bhvm_hop_ar3_eci_iff_s_f3 ); break;
        case BKNIT_F3222: BHVM_SET_CASE( f3_t, f2_t, f2_t, f2_t, bhvm_hop_ar3_eci_iff_s_f3 ); break;
        case BKNIT_F3223: BHVM_SET_CASE( f3_t, f2_t, f2_t, f3_t, bhvm_hop_ar3_eci_iff_s_f3 ); break;
        case BKNIT_F3232: BHVM_SET_CASE( f3_t, f2_t, f3_t, f2_t, bhvm_hop_ar3_eci_iff_s_f3 ); break;
        case BKNIT_F3233: BHVM_SET_CASE( f3_t, f2_t, f3_t, f3_t, bhvm_hop_ar3_eci_iff_s_f3 ); break;
        case BKNIT_F3322: BHVM_SET_CASE( f3_t, f3_t, f2_t, f2_t, bhvm_hop_ar3_eci_iff_s_f3 ); break;
        case BKNIT_F3323: BHVM_SET_CASE( f3_t, f3_t, f2_t, f3_t, bhvm_hop_ar3_eci_iff_s_f3 ); break;
        case BKNIT_F3332: BHVM_SET_CASE( f3_t, f3_t, f3_t, f2_t, bhvm_hop_ar3_eci_iff_s_f3 ); break;
        case BKNIT_F3333: BHVM_SET_CASE( f3_t, f3_t, f3_t, f3_t, bhvm_hop_ar3_eci_iff_s_f3 ); break;
        default: BKNIT_FA4_ERR( a->v.type, b->v.type, c->v.type, r->v.type ); break;
    }
    
    #undef BHVM_SET_CASE
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar3_eci_iff_acc_s )
"aware bhvm_hop_ar3_eci"
"{"
"}";

void bhvm_hop_ar3_eci_iff_acc_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, const bhvm_holor_s* c, bhvm_holor_s* r )
{
    // bhvm_hop.x:576:5
    
    assert( a != r && b != r && c != r );
    assert(  sz_min( a->v.size, sz_min( b->v.size, sz_min( c->v.size, r->v.size ) ) ) > 0 );
    sz_t n = sz_gcd( a->v.size, sz_gcd( b->v.size, sz_gcd( c->v.size, r->v.size ) ) );
    sz_t m = sz_max( a->v.size, sz_max( b->v.size, sz_max( c->v.size, r->v.size ) ) ) / n;
    
    #define BHVM_ACC_CASE( TA_T, TB_T, TC_T, TR_T, FUNC ) \
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
        case BKNIT_F2222: BHVM_ACC_CASE( f2_t, f2_t, f2_t, f2_t, bhvm_hop_ar3_eci_iff_acc_s_f2 ); break;
        case BKNIT_F2223: BHVM_ACC_CASE( f2_t, f2_t, f2_t, f3_t, bhvm_hop_ar3_eci_iff_acc_s_f3 ); break;
        case BKNIT_F2232: BHVM_ACC_CASE( f2_t, f2_t, f3_t, f2_t, bhvm_hop_ar3_eci_iff_acc_s_f3 ); break;
        case BKNIT_F2233: BHVM_ACC_CASE( f2_t, f2_t, f3_t, f3_t, bhvm_hop_ar3_eci_iff_acc_s_f3 ); break;
        case BKNIT_F2322: BHVM_ACC_CASE( f2_t, f3_t, f2_t, f2_t, bhvm_hop_ar3_eci_iff_acc_s_f3 ); break;
        case BKNIT_F2323: BHVM_ACC_CASE( f2_t, f3_t, f2_t, f3_t, bhvm_hop_ar3_eci_iff_acc_s_f3 ); break;
        case BKNIT_F2332: BHVM_ACC_CASE( f2_t, f3_t, f3_t, f2_t, bhvm_hop_ar3_eci_iff_acc_s_f3 ); break;
        case BKNIT_F2333: BHVM_ACC_CASE( f2_t, f3_t, f3_t, f3_t, bhvm_hop_ar3_eci_iff_acc_s_f3 ); break;
        case BKNIT_F3222: BHVM_ACC_CASE( f3_t, f2_t, f2_t, f2_t, bhvm_hop_ar3_eci_iff_acc_s_f3 ); break;
        case BKNIT_F3223: BHVM_ACC_CASE( f3_t, f2_t, f2_t, f3_t, bhvm_hop_ar3_eci_iff_acc_s_f3 ); break;
        case BKNIT_F3232: BHVM_ACC_CASE( f3_t, f2_t, f3_t, f2_t, bhvm_hop_ar3_eci_iff_acc_s_f3 ); break;
        case BKNIT_F3233: BHVM_ACC_CASE( f3_t, f2_t, f3_t, f3_t, bhvm_hop_ar3_eci_iff_acc_s_f3 ); break;
        case BKNIT_F3322: BHVM_ACC_CASE( f3_t, f3_t, f2_t, f2_t, bhvm_hop_ar3_eci_iff_acc_s_f3 ); break;
        case BKNIT_F3323: BHVM_ACC_CASE( f3_t, f3_t, f2_t, f3_t, bhvm_hop_ar3_eci_iff_acc_s_f3 ); break;
        case BKNIT_F3332: BHVM_ACC_CASE( f3_t, f3_t, f3_t, f2_t, bhvm_hop_ar3_eci_iff_acc_s_f3 ); break;
        case BKNIT_F3333: BHVM_ACC_CASE( f3_t, f3_t, f3_t, f3_t, bhvm_hop_ar3_eci_iff_acc_s_f3 ); break;
        default: BKNIT_FA4_ERR( a->v.type, b->v.type, c->v.type, r->v.type ); break;
    }
    
    #undef BHVM_ACC_CASE
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar3_eci_div_dp_zabg_s )
"aware bhvm_hop_ar3_eci"
"{"
"}";

void bhvm_hop_ar3_eci_div_dp_zabg_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, const bhvm_holor_s* c, bhvm_holor_s* r )
{
    // bhvm_hop.x:576:5
    
    assert( a != r && b != r && c != r );
    assert(  sz_min( a->v.size, sz_min( b->v.size, sz_min( c->v.size, r->v.size ) ) ) > 0 );
    sz_t n = sz_gcd( a->v.size, sz_gcd( b->v.size, sz_gcd( c->v.size, r->v.size ) ) );
    sz_t m = sz_max( a->v.size, sz_max( b->v.size, sz_max( c->v.size, r->v.size ) ) ) / n;
    
    #define BHVM_ACC_CASE( TA_T, TB_T, TC_T, TR_T, FUNC ) \
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
        case BKNIT_F2222: BHVM_ACC_CASE( f2_t, f2_t, f2_t, f2_t, bhvm_hop_ar3_eci_div_dp_zabg_s_f2 ); break;
        case BKNIT_F2223: BHVM_ACC_CASE( f2_t, f2_t, f2_t, f3_t, bhvm_hop_ar3_eci_div_dp_zabg_s_f3 ); break;
        case BKNIT_F2232: BHVM_ACC_CASE( f2_t, f2_t, f3_t, f2_t, bhvm_hop_ar3_eci_div_dp_zabg_s_f3 ); break;
        case BKNIT_F2233: BHVM_ACC_CASE( f2_t, f2_t, f3_t, f3_t, bhvm_hop_ar3_eci_div_dp_zabg_s_f3 ); break;
        case BKNIT_F2322: BHVM_ACC_CASE( f2_t, f3_t, f2_t, f2_t, bhvm_hop_ar3_eci_div_dp_zabg_s_f3 ); break;
        case BKNIT_F2323: BHVM_ACC_CASE( f2_t, f3_t, f2_t, f3_t, bhvm_hop_ar3_eci_div_dp_zabg_s_f3 ); break;
        case BKNIT_F2332: BHVM_ACC_CASE( f2_t, f3_t, f3_t, f2_t, bhvm_hop_ar3_eci_div_dp_zabg_s_f3 ); break;
        case BKNIT_F2333: BHVM_ACC_CASE( f2_t, f3_t, f3_t, f3_t, bhvm_hop_ar3_eci_div_dp_zabg_s_f3 ); break;
        case BKNIT_F3222: BHVM_ACC_CASE( f3_t, f2_t, f2_t, f2_t, bhvm_hop_ar3_eci_div_dp_zabg_s_f3 ); break;
        case BKNIT_F3223: BHVM_ACC_CASE( f3_t, f2_t, f2_t, f3_t, bhvm_hop_ar3_eci_div_dp_zabg_s_f3 ); break;
        case BKNIT_F3232: BHVM_ACC_CASE( f3_t, f2_t, f3_t, f2_t, bhvm_hop_ar3_eci_div_dp_zabg_s_f3 ); break;
        case BKNIT_F3233: BHVM_ACC_CASE( f3_t, f2_t, f3_t, f3_t, bhvm_hop_ar3_eci_div_dp_zabg_s_f3 ); break;
        case BKNIT_F3322: BHVM_ACC_CASE( f3_t, f3_t, f2_t, f2_t, bhvm_hop_ar3_eci_div_dp_zabg_s_f3 ); break;
        case BKNIT_F3323: BHVM_ACC_CASE( f3_t, f3_t, f2_t, f3_t, bhvm_hop_ar3_eci_div_dp_zabg_s_f3 ); break;
        case BKNIT_F3332: BHVM_ACC_CASE( f3_t, f3_t, f3_t, f2_t, bhvm_hop_ar3_eci_div_dp_zabg_s_f3 ); break;
        case BKNIT_F3333: BHVM_ACC_CASE( f3_t, f3_t, f3_t, f3_t, bhvm_hop_ar3_eci_div_dp_zabg_s_f3 ); break;
        default: BKNIT_FA4_ERR( a->v.type, b->v.type, c->v.type, r->v.type ); break;
    }
    
    #undef BHVM_ACC_CASE
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar3_eci_pow_dp_abzf_s )
"aware bhvm_hop_ar3_eci"
"{"
"}";

void bhvm_hop_ar3_eci_pow_dp_abzf_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, const bhvm_holor_s* c, bhvm_holor_s* r )
{
    // bhvm_hop.x:576:5
    
    assert( a != r && b != r && c != r );
    assert(  sz_min( a->v.size, sz_min( b->v.size, sz_min( c->v.size, r->v.size ) ) ) > 0 );
    sz_t n = sz_gcd( a->v.size, sz_gcd( b->v.size, sz_gcd( c->v.size, r->v.size ) ) );
    sz_t m = sz_max( a->v.size, sz_max( b->v.size, sz_max( c->v.size, r->v.size ) ) ) / n;
    
    #define BHVM_ACC_CASE( TA_T, TB_T, TC_T, TR_T, FUNC ) \
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
        case BKNIT_F2222: BHVM_ACC_CASE( f2_t, f2_t, f2_t, f2_t, bhvm_hop_ar3_eci_pow_dp_abzf_s_f2 ); break;
        case BKNIT_F2223: BHVM_ACC_CASE( f2_t, f2_t, f2_t, f3_t, bhvm_hop_ar3_eci_pow_dp_abzf_s_f3 ); break;
        case BKNIT_F2232: BHVM_ACC_CASE( f2_t, f2_t, f3_t, f2_t, bhvm_hop_ar3_eci_pow_dp_abzf_s_f3 ); break;
        case BKNIT_F2233: BHVM_ACC_CASE( f2_t, f2_t, f3_t, f3_t, bhvm_hop_ar3_eci_pow_dp_abzf_s_f3 ); break;
        case BKNIT_F2322: BHVM_ACC_CASE( f2_t, f3_t, f2_t, f2_t, bhvm_hop_ar3_eci_pow_dp_abzf_s_f3 ); break;
        case BKNIT_F2323: BHVM_ACC_CASE( f2_t, f3_t, f2_t, f3_t, bhvm_hop_ar3_eci_pow_dp_abzf_s_f3 ); break;
        case BKNIT_F2332: BHVM_ACC_CASE( f2_t, f3_t, f3_t, f2_t, bhvm_hop_ar3_eci_pow_dp_abzf_s_f3 ); break;
        case BKNIT_F2333: BHVM_ACC_CASE( f2_t, f3_t, f3_t, f3_t, bhvm_hop_ar3_eci_pow_dp_abzf_s_f3 ); break;
        case BKNIT_F3222: BHVM_ACC_CASE( f3_t, f2_t, f2_t, f2_t, bhvm_hop_ar3_eci_pow_dp_abzf_s_f3 ); break;
        case BKNIT_F3223: BHVM_ACC_CASE( f3_t, f2_t, f2_t, f3_t, bhvm_hop_ar3_eci_pow_dp_abzf_s_f3 ); break;
        case BKNIT_F3232: BHVM_ACC_CASE( f3_t, f2_t, f3_t, f2_t, bhvm_hop_ar3_eci_pow_dp_abzf_s_f3 ); break;
        case BKNIT_F3233: BHVM_ACC_CASE( f3_t, f2_t, f3_t, f3_t, bhvm_hop_ar3_eci_pow_dp_abzf_s_f3 ); break;
        case BKNIT_F3322: BHVM_ACC_CASE( f3_t, f3_t, f2_t, f2_t, bhvm_hop_ar3_eci_pow_dp_abzf_s_f3 ); break;
        case BKNIT_F3323: BHVM_ACC_CASE( f3_t, f3_t, f2_t, f3_t, bhvm_hop_ar3_eci_pow_dp_abzf_s_f3 ); break;
        case BKNIT_F3332: BHVM_ACC_CASE( f3_t, f3_t, f3_t, f2_t, bhvm_hop_ar3_eci_pow_dp_abzf_s_f3 ); break;
        case BKNIT_F3333: BHVM_ACC_CASE( f3_t, f3_t, f3_t, f3_t, bhvm_hop_ar3_eci_pow_dp_abzf_s_f3 ); break;
        default: BKNIT_FA4_ERR( a->v.type, b->v.type, c->v.type, r->v.type ); break;
    }
    
    #undef BHVM_ACC_CASE
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar3_eci_pow_dp_ayzg_s )
"aware bhvm_hop_ar3_eci"
"{"
"}";

void bhvm_hop_ar3_eci_pow_dp_ayzg_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, const bhvm_holor_s* c, bhvm_holor_s* r )
{
    // bhvm_hop.x:576:5
    
    assert( a != r && b != r && c != r );
    assert(  sz_min( a->v.size, sz_min( b->v.size, sz_min( c->v.size, r->v.size ) ) ) > 0 );
    sz_t n = sz_gcd( a->v.size, sz_gcd( b->v.size, sz_gcd( c->v.size, r->v.size ) ) );
    sz_t m = sz_max( a->v.size, sz_max( b->v.size, sz_max( c->v.size, r->v.size ) ) ) / n;
    
    #define BHVM_ACC_CASE( TA_T, TB_T, TC_T, TR_T, FUNC ) \
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
        case BKNIT_F2222: BHVM_ACC_CASE( f2_t, f2_t, f2_t, f2_t, bhvm_hop_ar3_eci_pow_dp_ayzg_s_f2 ); break;
        case BKNIT_F2223: BHVM_ACC_CASE( f2_t, f2_t, f2_t, f3_t, bhvm_hop_ar3_eci_pow_dp_ayzg_s_f3 ); break;
        case BKNIT_F2232: BHVM_ACC_CASE( f2_t, f2_t, f3_t, f2_t, bhvm_hop_ar3_eci_pow_dp_ayzg_s_f3 ); break;
        case BKNIT_F2233: BHVM_ACC_CASE( f2_t, f2_t, f3_t, f3_t, bhvm_hop_ar3_eci_pow_dp_ayzg_s_f3 ); break;
        case BKNIT_F2322: BHVM_ACC_CASE( f2_t, f3_t, f2_t, f2_t, bhvm_hop_ar3_eci_pow_dp_ayzg_s_f3 ); break;
        case BKNIT_F2323: BHVM_ACC_CASE( f2_t, f3_t, f2_t, f3_t, bhvm_hop_ar3_eci_pow_dp_ayzg_s_f3 ); break;
        case BKNIT_F2332: BHVM_ACC_CASE( f2_t, f3_t, f3_t, f2_t, bhvm_hop_ar3_eci_pow_dp_ayzg_s_f3 ); break;
        case BKNIT_F2333: BHVM_ACC_CASE( f2_t, f3_t, f3_t, f3_t, bhvm_hop_ar3_eci_pow_dp_ayzg_s_f3 ); break;
        case BKNIT_F3222: BHVM_ACC_CASE( f3_t, f2_t, f2_t, f2_t, bhvm_hop_ar3_eci_pow_dp_ayzg_s_f3 ); break;
        case BKNIT_F3223: BHVM_ACC_CASE( f3_t, f2_t, f2_t, f3_t, bhvm_hop_ar3_eci_pow_dp_ayzg_s_f3 ); break;
        case BKNIT_F3232: BHVM_ACC_CASE( f3_t, f2_t, f3_t, f2_t, bhvm_hop_ar3_eci_pow_dp_ayzg_s_f3 ); break;
        case BKNIT_F3233: BHVM_ACC_CASE( f3_t, f2_t, f3_t, f3_t, bhvm_hop_ar3_eci_pow_dp_ayzg_s_f3 ); break;
        case BKNIT_F3322: BHVM_ACC_CASE( f3_t, f3_t, f2_t, f2_t, bhvm_hop_ar3_eci_pow_dp_ayzg_s_f3 ); break;
        case BKNIT_F3323: BHVM_ACC_CASE( f3_t, f3_t, f2_t, f3_t, bhvm_hop_ar3_eci_pow_dp_ayzg_s_f3 ); break;
        case BKNIT_F3332: BHVM_ACC_CASE( f3_t, f3_t, f3_t, f2_t, bhvm_hop_ar3_eci_pow_dp_ayzg_s_f3 ); break;
        case BKNIT_F3333: BHVM_ACC_CASE( f3_t, f3_t, f3_t, f3_t, bhvm_hop_ar3_eci_pow_dp_ayzg_s_f3 ); break;
        default: BKNIT_FA4_ERR( a->v.type, b->v.type, c->v.type, r->v.type ); break;
    }
    
    #undef BHVM_ACC_CASE
}

XOILA_DEFINE_SPECT( bhvm_hop, bhvm_hop_ar3_eci )
"{"
    "bcore_spect_header_s header;"
"}";

/**********************************************************************************************************************/
// source: bhvm_mcode.h
#include "bhvm_mcode.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_mcode

BCORE_DEFINE_OBJECT_INST_P( bhvm_mcode_op_s )
"aware bhvm_mcode"
"{"
    "aware bhvm_vop => vop;"
    "private bhvm_vop_spect_s* p;"
    "func bcore_inst_call:copy_x;"
    "func bcore_via_call:mutated;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_mcode_node_s )
"aware bhvm_mcode"
"{"
    "sz_t nidx = -1;"
    "sz_t ax0 = -1;"
    "sz_t ag0 = -1;"
    "sz_t ax1 = -1;"
    "sz_t ag1 = -1;"
    "bl_t param;"
    "bl_t adaptive;"
    "bl_t cyclic;"
"}";

sz_t bhvm_mcode_node_s_get_pclass_idx( const bhvm_mcode_node_s* o, tp_t pclass )
{
    // bhvm_mcode.h:98:5
    
    switch( pclass )
    {
        case TYPEOF_pclass_ax0: return  o->ax0;
        case TYPEOF_pclass_ag0: return  o->ag0;
        case TYPEOF_pclass_ax1: return  o->ax1;
        case TYPEOF_pclass_ag1: return  o->ag1;
        default: break;
    }
    return  -1;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_mcode_nbase_s )
"aware bcore_array"
"{"
    "bhvm_mcode_node_s => [];"
"}";

bhvm_mcode_node_s* bhvm_mcode_nbase_s_push_node( bhvm_mcode_nbase_s* o )
{
    // bhvm_mcode.h:117:5
    
    sz_t nidx = o->size;
    bcore_array_a_push( ((bcore_array*)(o)),sr_asd( bhvm_mcode_node_s_create() ) );
    bhvm_mcode_node_s* node = o->data[ nidx ];
    node->nidx = nidx;
    return  node;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_mcode_track_s )
"aware bcore_array"
"{"
    "tp_t name;"
    "bhvm_mcode_op_s [];"
    "func bhvm_vop:run;"
"}";

void bhvm_mcode_track_s_run_section( const bhvm_mcode_track_s* o, sz_t start, sz_t size, bhvm_holor_s** ah )
{
    // bhvm_mcode.h:218:5
    
    assert( start >= 0 && start < o->size - size );
    for( sz_t i = 0; i < size; i++ ) bhvm_mcode_op_s_run(&(o->data[ i + start ]),ah );
}

sz_t bhvm_mcode_track_s_vop_push_d( bhvm_mcode_track_s* o, bhvm_vop* vop )
{
    // bhvm_mcode.h:224:5
    
    assert( vop );
    bhvm_mcode_op_s* op = bhvm_mcode_track_s_push( o);
    op->vop = vop;
    op->p = ( bhvm_vop_spect_s* )bhvm_vop_spect_s_get_aware( op->vop );
    assert( op->p );
    return  o->size - 1;
}

sz_t bhvm_mcode_track_s_push_copy_from_index( bhvm_mcode_track_s* o, sz_t index )
{
    // bhvm_mcode.h:236:5
    
    assert( index >= 0 && index < o->size );
    return  bhvm_mcode_track_s_vop_push_d(o,bhvm_vop_a_clone(o->data[ index ].vop) );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_mcode_track_adl_s )
"aware bcore_array"
"{"
    "bhvm_mcode_track_s => [];"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_mcode_lib_s )
"aware bhvm_mcode"
"{"
    "bhvm_mcode_track_adl_s arr;"
    "bcore_hmap_tpuz_s map;"
"}";

bhvm_mcode_track_s* bhvm_mcode_lib_s_track_get( bhvm_mcode_lib_s* o, tp_t name )
{
    // bhvm_mcode.h:271:5
    
    uz_t* pidx = bcore_hmap_tpuz_s_get(&(o->map),name );
    return  pidx ? o->arr.data[ (*(pidx)) ] : NULL;
}

bhvm_mcode_track_s* bhvm_mcode_lib_s_track_get_or_new( bhvm_mcode_lib_s* o, tp_t name )
{
    // bhvm_mcode.h:277:5
    
    if( bcore_hmap_tpuz_s_exists(&(o->map),name ) ) return  bhvm_mcode_lib_s_track_get(o,name );
    bcore_hmap_tpuz_s_set(&(o->map),name, o->arr.size );
    bhvm_mcode_track_s* track = bhvm_mcode_track_adl_s_push( &(o->arr));
    track->name = name;
    return  track;
}

bhvm_mcode_track_s* bhvm_mcode_lib_s_track_reset( bhvm_mcode_lib_s* o, tp_t name )
{
    // bhvm_mcode.h:286:5
    
    bhvm_mcode_track_s* track = bhvm_mcode_lib_s_track_get_or_new(o,name );
    bhvm_mcode_track_s_clear( track);
    return  track;
}

void bhvm_mcode_lib_s_track_vop_set_args_push_d( bhvm_mcode_lib_s* o, tp_t name, bhvm_vop* vop, const bhvm_vop_arr_ci_s* arr_ci )
{
    // bhvm_mcode.h:296:5
    
    bhvm_vop_a_set_args(vop,arr_ci );
    bhvm_mcode_track_s_vop_push_d(bhvm_mcode_lib_s_track_get_or_new(o,name ),vop );
}

void bhvm_mcode_lib_s_track_push( bhvm_mcode_lib_s* o, tp_t name, tp_t src_name )
{
    // bhvm_mcode.h:302:5
    
    bhvm_mcode_track_s* src = bhvm_mcode_lib_s_track_get(o,src_name );
    if( !src ) return ;
    bhvm_mcode_track_s* dst = bhvm_mcode_lib_s_track_get_or_new(o,name );
    {const bhvm_mcode_track_s* __a=src ;if(__a)for(sz_t __i=0; __i<__a->size; __i++){bhvm_mcode_op_s* e=&(__a->data[__i]); bhvm_mcode_track_s_vop_push_c(dst,e->vop );}}
}

void bhvm_mcode_lib_s_track_remove( bhvm_mcode_lib_s* o, tp_t name )
{
    // bhvm_mcode.h:310:5
    
    uz_t* pidx = bcore_hmap_tpuz_s_get(&(o->map),name );
    if( !pidx ) return ;
    sz_t idx = (*(pidx));
    bhvm_mcode_track_s_discard(o->arr.data[ idx ]);
    o->arr.size--;
    o->arr.data[ idx ] = o->arr.data[ o->arr.size ];
    o->arr.data[ o->arr.size ] = NULL;
    if( idx < o->arr.size ) bcore_hmap_tpuz_s_set(&(o->map),o->arr.data[ idx ]->name, ((uz_t)(idx)) );
}

void bhvm_mcode_lib_s_track_run_ah( const bhvm_mcode_lib_s* o, tp_t name, bhvm_holor_s** ah )
{
    // bhvm_mcode.h:322:5
    
    bhvm_mcode_track_s* t = bhvm_mcode_lib_s_track_get(((bhvm_mcode_lib_s*)(o)),name );
    if( t ) bhvm_mcode_track_s_run(t,ah );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_mcode_frame_s )
"aware bhvm_mcode"
"{"
    "bhvm_mcode_lib_s => lib;"
    "bhvm_mcode_hbase_s => hbase;"
    "bhvm_mcode_nbase_s => nbase;"
    "func bcore_via_call:mutated;"
    "func bcore_inst_call:copy_x;"
"}";

XOILA_DEFINE_SPECT( bcore_inst, bhvm_mcode )
"{"
    "bcore_spect_header_s header;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_mcode_hmeta

BCORE_DEFINE_OBJECT_INST_P( bhvm_mcode_hmeta_adl_s )
"aware bcore_array"
"{"
    "aware bhvm_mcode_hmeta=> [];"
"}";

XOILA_DEFINE_SPECT( bhvm_mcode, bhvm_mcode_hmeta )
"{"
    "bcore_spect_header_s header;"
    "feature aware bhvm_mcode_hmeta : get_name = bhvm_mcode_hmeta_get_name_default;"
    "feature aware bhvm_mcode_hmeta : get_pclass = bhvm_mcode_hmeta_get_pclass_default;"
    "feature aware bhvm_mcode_hmeta : get_node = bhvm_mcode_hmeta_get_node_default;"
    "feature aware bhvm_mcode_hmeta : set_node = bhvm_mcode_hmeta_set_node_default;"
    "feature aware bhvm_mcode_hmeta : is_rollable = bhvm_mcode_hmeta_is_rollable_default;"
    "feature aware bhvm_mcode_hmeta : is_active = bhvm_mcode_hmeta_is_active_default;"
    "feature aware bhvm_mcode_hmeta : get_custom = bhvm_mcode_hmeta_get_custom_default;"
    "feature aware bhvm_mcode_hmeta : set_custom = bhvm_mcode_hmeta_set_custom_default;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_mcode_hbase

BCORE_DEFINE_OBJECT_INST_P( bhvm_mcode_hbase_s )
"aware bhvm_mcode_hbase"
"{"
    "bhvm_holor_adl_s holor_adl;"
    "bhvm_mcode_hmeta_adl_s hmeta_adl;"
    "sz_t copy_size_limit = -1;"
    "func bcore_via_call:mutated;"
    "func bcore_inst_call:copy_x;"
"}";

bhvm_mcode_hbase_s* bhvm_mcode_hbase_s_set_size( bhvm_mcode_hbase_s* o, sz_t size )
{
    // bhvm_mcode.h:163:9
    
    bhvm_holor_adl_s_set_size( &(o->holor_adl),size );
    bhvm_mcode_hmeta_adl_s_set_size( &(o->hmeta_adl),size );
    return  o;
}

sz_t bhvm_mcode_hbase_s_push_hm( bhvm_mcode_hbase_s* o, const bhvm_holor_s* h, const bhvm_mcode_hmeta* m )
{
    // bhvm_mcode.h:177:9
    
    sz_t idx = o->holor_adl.size;
    bhvm_mcode_hmeta_adl_s_push_c( &(o->hmeta_adl),m );
    bhvm_holor_adl_s_push_c( &(o->holor_adl),h );
    return  idx;
}

sz_t bhvm_mcode_hbase_s_push_hmc( bhvm_mcode_hbase_s* o, const bhvm_holor_s* h, const bhvm_mcode_hmeta* m, char c, bhvm_vop_arr_ci_s* arr_ci )
{
    // bhvm_mcode.h:185:9
    
    bhvm_vop_ci_s ci;
    bhvm_vop_ci_s_init( (&(ci)) );
    ci.c = c;
    ci.i = bhvm_mcode_hbase_s_push_hm(o,h, m );
    bhvm_vop_arr_ci_s_push_c( arr_ci,(&(ci)) );
    return  ci.i;
}

sz_t bhvm_mcode_hbase_s_push_copy_from_index( bhvm_mcode_hbase_s* o, sz_t index )
{
    // bhvm_mcode.h:195:9
    
    assert( index >= 0 && index < o->holor_adl.size );
    sz_t ret = o->holor_adl.size;
    bhvm_mcode_hmeta_adl_s_push_c( &(o->hmeta_adl),o->hmeta_adl.data[ index ] );
    bhvm_holor_adl_s_push_c( &(o->holor_adl),o->holor_adl.data[ index ] );
    return  ret;
}

XOILA_DEFINE_SPECT( bhvm_mcode, bhvm_mcode_hbase )
"{"
    "bcore_spect_header_s header;"
"}";

/**********************************************************************************************************************/

vd_t bmath_general_signal_handler( const bcore_signal_s* o );
vd_t bhvm_general_signal_handler( const bcore_signal_s* o );
vd_t bhvm_general_signal_handler( const bcore_signal_s* o );

vd_t bhvm_xo_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bhvm_xo" ) ) )
    {
        case TYPEOF_init1:
        {

            // --------------------------------------------------------------------
            // source: bhvm_holor.h

            // group: bhvm
            BCORE_REGISTER_OBJECT( bhvm_shape_s );
            BCORE_REGISTER_OBJECT( bhvm_value_s );
            BCORE_REGISTER_FFUNC( bcore_fp_copy_typed, bhvm_holor_s_copy_typed );
            BCORE_REGISTER_OBJECT( bhvm_holor_s );
            BCORE_REGISTER_FFUNC( bcore_fp_copy_typed, bhvm_holor_adl_s_copy_typed );
            BCORE_REGISTER_OBJECT( bhvm_holor_adl_s );
            BCORE_REGISTER_OBJECT( bhvm_holor_ads_s );
            BCORE_REGISTER_OBJECT( bhvm_holor_mdl_s );
            BCORE_REGISTER_OBJECT( bhvm_stats_s );
            XOILA_REGISTER_SPECT( bhvm );

            // --------------------------------------------------------------------
            // source: bhvm_hop.h

            // group: bhvm_hop
            XOILA_REGISTER_SPECT( bhvm_hop );

            // group: bhvm_hop_ar0
            BCORE_REGISTER_OBJECT( bhvm_hop_ar0_zro_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar0_one_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar0_nul_dp_s );
            XOILA_REGISTER_SPECT( bhvm_hop_ar0 );

            // group: bhvm_hop_ar1
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_l1_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_sqrsum_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_cpy_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_neg_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_floor_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_ceil_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_exp_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_log_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_inv_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_abs_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_sig_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_sqr_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_srt_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_sigm_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_sigm_hard_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_sigm_leaky_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_tanh_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_tanh_hard_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_tanh_leaky_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_softplus_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_relu_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_relu_leaky_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_softmax_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_neg_dp_zf_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_cat_dp_zf_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_cat_dp_zg_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_ccat_dp_zf_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_ccat_dp_zg_s );
            XOILA_REGISTER_SPECT( bhvm_hop_ar1 );

            // group: bhvm_hop_ar1_eci
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_eci_cpy_acc_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_eci_neg_acc_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_eci_inv_acc_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_eci_cpy_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_eci_neg_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_eci_inv_s );
            XOILA_REGISTER_SPECT( bhvm_hop_ar1_eci );

            // group: bhvm_hop_ar2
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_sub_sqrsum_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_sub_l1_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_equal_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_unequal_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_larger_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_smaller_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_larger_equal_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_smaller_equal_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_logic_and_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_logic_or_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_abs_dp_zaf_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_exp_dp_zyf_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_log_dp_zaf_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_inv_dp_zyf_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_sqr_dp_zaf_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_srt_dp_zyf_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_sigm_dp_zyf_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_sigm_hard_dp_zyf_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_sigm_leaky_dp_zyf_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_tanh_dp_zyf_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_tanh_hard_dp_zyf_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_tanh_leaky_dp_zyf_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_softplus_dp_zyf_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_relu_dp_zyf_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_relu_leaky_dp_zyf_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_softmax_dp_zyf_s );
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
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_ccat_s );
            XOILA_REGISTER_SPECT( bhvm_hop_ar2 );

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
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_eci_iff_dp_b_azg_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_eci_iff_dp_c_azh_s );
            XOILA_REGISTER_SPECT( bhvm_hop_ar2_eci );

            // group: bhvm_hop_ar2_eci_logic
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_eci_logic_equal_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_eci_logic_unequal_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_eci_logic_larger_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_eci_logic_smaller_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_eci_logic_larger_equal_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_eci_logic_smaller_equal_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_eci_logic_and_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_eci_logic_or_s );
            XOILA_REGISTER_SPECT( bhvm_hop_ar2_eci_logic );

            // group: bhvm_hop_ar3
            XOILA_REGISTER_SPECT( bhvm_hop_ar3 );

            // group: bhvm_hop_ar3_eci
            BCORE_REGISTER_OBJECT( bhvm_hop_ar3_eci_iff_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar3_eci_iff_acc_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar3_eci_div_dp_zabg_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar3_eci_pow_dp_abzf_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar3_eci_pow_dp_ayzg_s );
            XOILA_REGISTER_SPECT( bhvm_hop_ar3_eci );

            // --------------------------------------------------------------------
            // source: bhvm_mcode.h

            // group: bhvm_mcode
            BCORE_REGISTER_FFUNC( bcore_inst_call_copy_x, bhvm_mcode_op_s_copy_x );
            BCORE_REGISTER_FFUNC( bcore_via_call_mutated, bhvm_mcode_op_s_mutated );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_mcode_op_s_run );
            BCORE_REGISTER_OBJECT( bhvm_mcode_op_s );
            BCORE_REGISTER_NAME( pclass_ax0 );
            BCORE_REGISTER_NAME( pclass_ag0 );
            BCORE_REGISTER_NAME( pclass_ax1 );
            BCORE_REGISTER_NAME( pclass_ag1 );
            BCORE_REGISTER_OBJECT( bhvm_mcode_node_s );
            BCORE_REGISTER_OBJECT( bhvm_mcode_nbase_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_mcode_track_s_run );
            BCORE_REGISTER_OBJECT( bhvm_mcode_track_s );
            BCORE_REGISTER_OBJECT( bhvm_mcode_track_adl_s );
            BCORE_REGISTER_OBJECT( bhvm_mcode_lib_s );
            BCORE_REGISTER_FFUNC( bcore_via_call_mutated, bhvm_mcode_frame_s_mutated );
            BCORE_REGISTER_FFUNC( bcore_inst_call_copy_x, bhvm_mcode_frame_s_copy_x );
            BCORE_REGISTER_OBJECT( bhvm_mcode_frame_s );
            XOILA_REGISTER_SPECT( bhvm_mcode );

            // group: bhvm_mcode_hmeta
            BCORE_REGISTER_FEATURE( bhvm_mcode_hmeta_get_name );
            BCORE_REGISTER_FFUNC( bhvm_mcode_hmeta_get_name, bhvm_mcode_hmeta_get_name_default );
            BCORE_REGISTER_FEATURE( bhvm_mcode_hmeta_get_pclass );
            BCORE_REGISTER_FFUNC( bhvm_mcode_hmeta_get_pclass, bhvm_mcode_hmeta_get_pclass_default );
            BCORE_REGISTER_FEATURE( bhvm_mcode_hmeta_get_node );
            BCORE_REGISTER_FFUNC( bhvm_mcode_hmeta_get_node, bhvm_mcode_hmeta_get_node_default );
            BCORE_REGISTER_FEATURE( bhvm_mcode_hmeta_set_node );
            BCORE_REGISTER_FFUNC( bhvm_mcode_hmeta_set_node, bhvm_mcode_hmeta_set_node_default );
            BCORE_REGISTER_FEATURE( bhvm_mcode_hmeta_is_rollable );
            BCORE_REGISTER_FFUNC( bhvm_mcode_hmeta_is_rollable, bhvm_mcode_hmeta_is_rollable_default );
            BCORE_REGISTER_FEATURE( bhvm_mcode_hmeta_is_active );
            BCORE_REGISTER_FFUNC( bhvm_mcode_hmeta_is_active, bhvm_mcode_hmeta_is_active_default );
            BCORE_REGISTER_FEATURE( bhvm_mcode_hmeta_get_custom );
            BCORE_REGISTER_FFUNC( bhvm_mcode_hmeta_get_custom, bhvm_mcode_hmeta_get_custom_default );
            BCORE_REGISTER_FEATURE( bhvm_mcode_hmeta_set_custom );
            BCORE_REGISTER_FFUNC( bhvm_mcode_hmeta_set_custom, bhvm_mcode_hmeta_set_custom_default );
            BCORE_REGISTER_OBJECT( bhvm_mcode_hmeta_adl_s );
            XOILA_REGISTER_SPECT( bhvm_mcode_hmeta );

            // group: bhvm_mcode_hbase
            BCORE_REGISTER_FFUNC( bcore_via_call_mutated, bhvm_mcode_hbase_s_mutated );
            BCORE_REGISTER_FFUNC( bcore_inst_call_copy_x, bhvm_mcode_hbase_s_copy_x );
            BCORE_REGISTER_OBJECT( bhvm_mcode_hbase_s );
            XOILA_REGISTER_SPECT( bhvm_mcode_hbase );
        }
        break;
        case TYPEOF_push_dependencies:
        {
            ASSERT( o->object && ( *( aware_t* )o->object ) == TYPEOF_bcore_arr_fp_s );
            bcore_arr_fp_s* arr_fp = o->object;
            bcore_arr_fp_s_push( arr_fp, ( fp_t )bmath_general_signal_handler );
            bcore_arr_fp_s_push( arr_fp, ( fp_t )bhvm_general_signal_handler );
            bcore_arr_fp_s_push( arr_fp, ( fp_t )bhvm_general_signal_handler );
        }
        break;
        default: break;
    }
    return NULL;
}
// XOILA_OUT_SIGNATURE 0x20E2399ECF462506ull
