/** This file was generated from beth-plant source code.
 *  Compiling Agent : bcore_plant_compiler (C) 2019 J.B.Steffens
 *  Last File Update: 2019-12-23T11:03:48Z
 *
 *  Copyright and License of this File:
 *
 *  Generated code inherits the copyright and license of the underlying beth-plant source code.
 *  Source code defining this file is distributed across following files:
 *
 *  bhvm_lop.h
 *
 */

#ifndef BHVM_PLANTED_LOP_H
#define BHVM_PLANTED_LOP_H

#include "bcore_control.h"


/**********************************************************************************************************************/
// source: bhvm_lop.h

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_lop

#define TYPEOF_bhvm_lop 347880838
#define TYPEOF_bhvm_lop_s 895566136
#define BETH_EXPAND_GROUP_bhvm_lop \
  BCORE_FORWARD_OBJECT( bhvm_lop ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar0 ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1 ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2 ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar3 ); \
  BETH_EXPAND_GROUP_bhvm_lop_ar0 \
  BETH_EXPAND_GROUP_bhvm_lop_ar1 \
  BETH_EXPAND_GROUP_bhvm_lop_ar2 \
  BETH_EXPAND_GROUP_bhvm_lop_ar3

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_lop_ar0

#define TYPEOF_bhvm_lop_ar0 2712754380
#define TYPEOF_bhvm_lop_ar0_s 169403886
#define TYPEOF_bhvm_lop_ar0_zro_s 1829446612
#define BETH_EXPAND_ITEM_bhvm_lop_ar0_zro_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar0_zro_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar0_zro_s_f2( void ){return 0;} \
  static inline f3_t bhvm_lop_ar0_zro_s_f3( void ){return 0;} \
  void bhvm_lop_ar0_zro_s_f( tp_t tknit, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar0_one_s 3752057989
#define BETH_EXPAND_ITEM_bhvm_lop_ar0_one_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar0_one_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar0_one_s_f2( void ){return 1;} \
  static inline f3_t bhvm_lop_ar0_one_s_f3( void ){return 1;} \
  void bhvm_lop_ar0_one_s_f( tp_t tknit, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar0_nul_dp_v_s 4119604442
#define BETH_EXPAND_ITEM_bhvm_lop_ar0_nul_dp_v_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar0_nul_dp_v_s ) \
    {aware_t _;}; \
  static inline void bhvm_lop_ar0_nul_dp_v_s_f( tp_t tknit, vd_t r, sz_t s ){/* no action*/}
#define BETH_EXPAND_GROUP_bhvm_lop_ar0 \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar0 ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar0_zro_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar0_one_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar0_nul_dp_v_s ); \
  BETH_EXPAND_ITEM_bhvm_lop_ar0_zro_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar0_one_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar0_nul_dp_v_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_lop_ar1

#define TYPEOF_bhvm_lop_ar1 2729531999
#define TYPEOF_bhvm_lop_ar1_s 1721933577
#define TYPEOF_bhvm_lop_ar1_identity_s 3685627580
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_identity_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_identity_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_identity_s_f2( f2_t a ){return  a;} \
  static inline f3_t bhvm_lop_ar1_identity_s_f3( f3_t a ){return  a;} \
  void bhvm_lop_ar1_identity_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_neg_s 1970974992
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_neg_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_neg_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_neg_s_f2( f2_t a ){return -a;} \
  static inline f3_t bhvm_lop_ar1_neg_s_f3( f3_t a ){return -a;} \
  void bhvm_lop_ar1_neg_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_floor_s 959134952
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_floor_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_floor_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_floor_s_f2( f2_t a ){return  floor(a);} \
  static inline f3_t bhvm_lop_ar1_floor_s_f3( f3_t a ){return  floor(a);} \
  void bhvm_lop_ar1_floor_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_ceil_s 1300617787
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_ceil_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_ceil_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_ceil_s_f2( f2_t a ){return   ceil(a);} \
  static inline f3_t bhvm_lop_ar1_ceil_s_f3( f3_t a ){return   ceil(a);} \
  void bhvm_lop_ar1_ceil_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_exp_s 2284292249
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_exp_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_exp_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_exp_s_f2( f2_t a ){return    exp(a);} \
  static inline f3_t bhvm_lop_ar1_exp_s_f3( f3_t a ){return    exp(a);} \
  void bhvm_lop_ar1_exp_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_inv_s 1876691453
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_inv_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_inv_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_inv_s_f2( f2_t a ){return f2_inv(a);} \
  static inline f3_t bhvm_lop_ar1_inv_s_f3( f3_t a ){return f3_inv(a);} \
  void bhvm_lop_ar1_inv_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_tanh_s 3034687379
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_tanh_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_tanh_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_tanh_s_f2( f2_t a ){return ( a < 350 ) ? ( 1.0 - ( 2.0 / ( exp( 2.0 * ( f3_t )a ) + 1.0 ) ) ) : 1.0;} \
  static inline f3_t bhvm_lop_ar1_tanh_s_f3( f3_t a ){return ( a < 350 ) ? ( 1.0 - ( 2.0 / ( exp( 2.0 * ( f3_t )a ) + 1.0 ) ) ) : 1.0;} \
  void bhvm_lop_ar1_tanh_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_tanh_hard_s 1700863853
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_tanh_hard_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_tanh_hard_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_tanh_hard_s_f2( f2_t a ){return ( a < -1.0 ) ? -1.0 : ( a > 1.0 ) ? 1.0 : a;} \
  static inline f3_t bhvm_lop_ar1_tanh_hard_s_f3( f3_t a ){return ( a < -1.0 ) ? -1.0 : ( a > 1.0 ) ? 1.0 : a;} \
  void bhvm_lop_ar1_tanh_hard_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_tanh_leaky_s 2493732654
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_tanh_leaky_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_tanh_leaky_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_tanh_leaky_s_f2( f2_t a ){return ( a < -1.0 ) ? -1.0 + 0.01 * ( a + 1.0 ) : ( a > 1.0 ) ? 1.0 + 0.01 * ( a - 1.0 ) : a;} \
  static inline f3_t bhvm_lop_ar1_tanh_leaky_s_f3( f3_t a ){return ( a < -1.0 ) ? -1.0 + 0.01 * ( a + 1.0 ) : ( a > 1.0 ) ? 1.0 + 0.01 * ( a - 1.0 ) : a;} \
  void bhvm_lop_ar1_tanh_leaky_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_lgst_s 1091249056
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_lgst_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_lgst_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_lgst_s_f2( f2_t a ){return ( a > -700 ) ? ( 1.0 / ( 1.0 + exp( -( f3_t )a ) ) ) : 0;} \
  static inline f3_t bhvm_lop_ar1_lgst_s_f3( f3_t a ){return ( a > -700 ) ? ( 1.0 / ( 1.0 + exp( -( f3_t )a ) ) ) : 0;} \
  void bhvm_lop_ar1_lgst_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_lgst_hard_s 2304664648
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_lgst_hard_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_lgst_hard_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_lgst_hard_s_f2( f2_t a ){return ( a < -2.0 ) ? 0.0 : ( a > 2.0 ) ? 1.0 : 0.25 * ( a + 2.0 );} \
  static inline f3_t bhvm_lop_ar1_lgst_hard_s_f3( f3_t a ){return ( a < -2.0 ) ? 0.0 : ( a > 2.0 ) ? 1.0 : 0.25 * ( a + 2.0 );} \
  void bhvm_lop_ar1_lgst_hard_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_lgst_leaky_s 799225725
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_lgst_leaky_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_lgst_leaky_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_lgst_leaky_s_f2( f2_t a ){return ( a < -2.0 ) ? 0.01 * ( a + 2.0 ) : ( a > 2.0 ) ? 1.0 + 0.01 * ( a - 2.0 ) : 0.25 * ( a + 2.0 );} \
  static inline f3_t bhvm_lop_ar1_lgst_leaky_s_f3( f3_t a ){return ( a < -2.0 ) ? 0.01 * ( a + 2.0 ) : ( a > 2.0 ) ? 1.0 + 0.01 * ( a - 2.0 ) : 0.25 * ( a + 2.0 );} \
  void bhvm_lop_ar1_lgst_leaky_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_softplus_s 1459753746
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_softplus_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_softplus_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_softplus_s_f2( f2_t a ){return ( a < 700 ) ? log( 1.0 + exp( ( f3_t )a ) ) : a;} \
  static inline f3_t bhvm_lop_ar1_softplus_s_f3( f3_t a ){return ( a < 700 ) ? log( 1.0 + exp( ( f3_t )a ) ) : a;} \
  void bhvm_lop_ar1_softplus_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_relu_s 3592263008
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_relu_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_relu_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_relu_s_f2( f2_t a ){return a > 0 ? a : 0;} \
  static inline f3_t bhvm_lop_ar1_relu_s_f3( f3_t a ){return a > 0 ? a : 0;} \
  void bhvm_lop_ar1_relu_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_relu_leaky_s 3127908157
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_relu_leaky_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_relu_leaky_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_relu_leaky_s_f2( f2_t a ){return a > 0 ? a : a * 0.01;} \
  static inline f3_t bhvm_lop_ar1_relu_leaky_s_f3( f3_t a ){return a > 0 ? a : a * 0.01;} \
  void bhvm_lop_ar1_relu_leaky_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_identity_dp_v_s 2644743712
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_identity_dp_v_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_identity_dp_v_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_identity_dp_v_s_f2( f2_t a ){return  a;} \
  static inline f3_t bhvm_lop_ar1_identity_dp_v_s_f3( f3_t a ){return  a;} \
  void bhvm_lop_ar1_identity_dp_v_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_neg_dp_v_s 1337893420
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_neg_dp_v_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_neg_dp_v_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_neg_dp_v_s_f2( f2_t a ){return -a;} \
  static inline f3_t bhvm_lop_ar1_neg_dp_v_s_f3( f3_t a ){return -a;} \
  void bhvm_lop_ar1_neg_dp_v_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_add_dp_a_v_s 1922126999
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_add_dp_a_v_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_add_dp_a_v_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_add_dp_a_v_s_f2( f2_t a ){return  a;} \
  static inline f3_t bhvm_lop_ar1_add_dp_a_v_s_f3( f3_t a ){return  a;} \
  void bhvm_lop_ar1_add_dp_a_v_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_add_dp_b_v_s 1393198986
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_add_dp_b_v_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_add_dp_b_v_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_add_dp_b_v_s_f2( f2_t a ){return  a;} \
  static inline f3_t bhvm_lop_ar1_add_dp_b_v_s_f3( f3_t a ){return  a;} \
  void bhvm_lop_ar1_add_dp_b_v_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_sub_dp_a_v_s 1884295972
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_sub_dp_a_v_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_sub_dp_a_v_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_sub_dp_a_v_s_f2( f2_t a ){return  a;} \
  static inline f3_t bhvm_lop_ar1_sub_dp_a_v_s_f3( f3_t a ){return  a;} \
  void bhvm_lop_ar1_sub_dp_a_v_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_sub_dp_b_v_s 990456445
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_sub_dp_b_v_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_sub_dp_b_v_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_sub_dp_b_v_s_f2( f2_t a ){return -a;} \
  static inline f3_t bhvm_lop_ar1_sub_dp_b_v_s_f3( f3_t a ){return -a;} \
  void bhvm_lop_ar1_sub_dp_b_v_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define BETH_EXPAND_GROUP_bhvm_lop_ar1 \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1 ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_identity_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_neg_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_floor_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_ceil_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_exp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_inv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_tanh_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_tanh_hard_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_tanh_leaky_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_lgst_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_lgst_hard_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_lgst_leaky_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_softplus_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_relu_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_relu_leaky_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_identity_dp_v_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_neg_dp_v_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_add_dp_a_v_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_add_dp_b_v_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_sub_dp_a_v_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_sub_dp_b_v_s ); \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_identity_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_neg_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_floor_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_ceil_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_exp_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_inv_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_tanh_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_tanh_hard_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_tanh_leaky_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_lgst_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_lgst_hard_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_lgst_leaky_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_softplus_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_relu_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_relu_leaky_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_identity_dp_v_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_neg_dp_v_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_add_dp_a_v_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_add_dp_b_v_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_sub_dp_a_v_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_sub_dp_b_v_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_lop_ar2

#define TYPEOF_bhvm_lop_ar2 2746309618
#define TYPEOF_bhvm_lop_ar2_s 3687365548
#define TYPEOF_bhvm_lop_ar2_add_s 2313295804
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_add_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_add_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_add_s_f2( f2_t a, f2_t b ){return a + b;} \
  static inline f3_t bhvm_lop_ar2_add_s_f3( f3_t a, f3_t b ){return a + b;} \
  void bhvm_lop_ar2_add_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_sub_s 2025417869
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_sub_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_sub_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_sub_s_f2( f2_t a, f2_t b ){return a - b;} \
  static inline f3_t bhvm_lop_ar2_sub_s_f3( f3_t a, f3_t b ){return a - b;} \
  void bhvm_lop_ar2_sub_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_sub_sqr_s 2638056300
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_sub_sqr_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_sub_sqr_s ) \
    {aware_t _;}; \
  void bhvm_lop_ar2_sub_sqr_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_div_s 2382132956
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_div_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_div_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_div_s_f2( f2_t a, f2_t b ){return a * f2_inv( b );} \
  static inline f3_t bhvm_lop_ar2_div_s_f3( f3_t a, f3_t b ){return a * f3_inv( b );} \
  void bhvm_lop_ar2_div_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_mul_body_s 1439346934
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_body_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_mul_body_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_mul_body_s_f2( f2_t a, f2_t b ){return a * b;} \
  static inline f3_t bhvm_lop_ar2_mul_body_s_f3( f3_t a, f3_t b ){return a * b;} \
  void bhvm_lop_ar2_mul_body_s_f_vv_cv( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s ); \
  void bhvm_lop_ar2_mul_body_s_f_vv_av( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s ); \
  void bhvm_lop_ar2_mul_body_s_f_vs_cv( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s ); \
  void bhvm_lop_ar2_mul_body_s_f_vs_av( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s ); \
  void bhvm_lop_ar2_mul_body_s_f_vv_as( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s ); \
  void bhvm_lop_ar2_mul_body_s_f_vv_cs( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s ); \
  void bhvm_lop_ar2_mul_body_s_f_mv_cv( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t rows, sz_t cols ); \
  void bhvm_lop_ar2_mul_body_s_f_mv_av( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t rows, sz_t cols ); \
  void bhvm_lop_ar2_mul_body_s_f_vm_cv( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t rows, sz_t cols ); \
  void bhvm_lop_ar2_mul_body_s_f_vm_av( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t rows, sz_t cols ); \
  void bhvm_lop_ar2_mul_body_s_f_vv_cm( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t rows, sz_t cols ); \
  void bhvm_lop_ar2_mul_body_s_f_vv_am( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t rows, sz_t cols );
#define TYPEOF_bhvm_lop_ar2_mul_vvv_s 2412527448
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_vvv_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_mul_vvv_s ) \
    {aware_t _;}; \
  static inline void bhvm_lop_ar2_mul_vvv_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s ){bhvm_lop_ar2_mul_body_s_f_vv_cv( tknit, a, b, r, s );}
#define TYPEOF_bhvm_lop_ar2_mul_vsv_s 2704517073
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_vsv_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_mul_vsv_s ) \
    {aware_t _;}; \
  static inline void bhvm_lop_ar2_mul_vsv_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s ){bhvm_lop_ar2_mul_body_s_f_vs_cv( tknit, a, b, r, s );}
#define TYPEOF_bhvm_lop_ar2_mul_vvs_s 3092390055
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_vvs_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_mul_vvs_s ) \
    {aware_t _;}; \
  static inline void bhvm_lop_ar2_mul_vvs_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s ){bhvm_lop_ar2_mul_body_s_f_vv_cs( tknit, a, b, r, s );}
#define TYPEOF_bhvm_lop_ar2_mul_mvv_s 4276708049
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_mvv_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_mul_mvv_s ) \
    {aware_t _;}; \
  static inline void bhvm_lop_ar2_mul_mvv_s_f_m( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t rows, sz_t cols ){bhvm_lop_ar2_mul_body_s_f_mv_cv( tknit, a, b, r, rows, cols );}
#define TYPEOF_bhvm_lop_ar2_mul_vmv_s 3627000611
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_vmv_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_mul_vmv_s ) \
    {aware_t _;}; \
  static inline void bhvm_lop_ar2_mul_vmv_s_f_m( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t rows, sz_t cols ){bhvm_lop_ar2_mul_body_s_f_vm_cv( tknit, a, b, r, rows, cols );}
#define TYPEOF_bhvm_lop_ar2_mul_vvm_s 1633426157
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_vvm_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_mul_vvm_s ) \
    {aware_t _;}; \
  static inline void bhvm_lop_ar2_mul_vvm_s_f_m( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t rows, sz_t cols ){bhvm_lop_ar2_mul_body_s_f_vv_cm( tknit, a, b, r, rows, cols );}
#define TYPEOF_bhvm_lop_ar2_mul_acc_vvv_s 590550666
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_acc_vvv_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_mul_acc_vvv_s ) \
    {aware_t _;}; \
  static inline void bhvm_lop_ar2_mul_acc_vvv_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s ){bhvm_lop_ar2_mul_body_s_f_vv_av( tknit, a, b, r, s );}
#define TYPEOF_bhvm_lop_ar2_mul_acc_vsv_s 2629306179
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_acc_vsv_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_mul_acc_vsv_s ) \
    {aware_t _;}; \
  static inline void bhvm_lop_ar2_mul_acc_vsv_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s ){bhvm_lop_ar2_mul_body_s_f_vs_av( tknit, a, b, r, s );}
#define TYPEOF_bhvm_lop_ar2_mul_acc_vvs_s 733632633
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_acc_vvs_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_mul_acc_vvs_s ) \
    {aware_t _;}; \
  static inline void bhvm_lop_ar2_mul_acc_vvs_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s ){bhvm_lop_ar2_mul_body_s_f_vv_as( tknit, a, b, r, s );}
#define TYPEOF_bhvm_lop_ar2_mul_acc_mvv_s 4270331839
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_acc_mvv_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_mul_acc_mvv_s ) \
    {aware_t _;}; \
  static inline void bhvm_lop_ar2_mul_acc_mvv_s_f_m( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t rows, sz_t cols ){bhvm_lop_ar2_mul_body_s_f_mv_av( tknit, a, b, r, rows, cols );}
#define TYPEOF_bhvm_lop_ar2_mul_acc_vmv_s 1706925809
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_acc_vmv_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_mul_acc_vmv_s ) \
    {aware_t _;}; \
  static inline void bhvm_lop_ar2_mul_acc_vmv_s_f_m( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t rows, sz_t cols ){bhvm_lop_ar2_mul_body_s_f_vm_av( tknit, a, b, r, rows, cols );}
#define TYPEOF_bhvm_lop_ar2_mul_acc_vvm_s 4226443707
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_acc_vvm_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_mul_acc_vvm_s ) \
    {aware_t _;}; \
  static inline void bhvm_lop_ar2_mul_acc_vvm_s_f_m( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t rows, sz_t cols ){bhvm_lop_ar2_mul_body_s_f_vv_am( tknit, a, b, r, rows, cols );}
#define TYPEOF_bhvm_lop_ar2_div_dp_a_vb_s 3796727154
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_div_dp_a_vb_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_div_dp_a_vb_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_div_dp_a_vb_s_f2( f2_t a, f2_t b ){return a * f2_inv( b );} \
  static inline f3_t bhvm_lop_ar2_div_dp_a_vb_s_f3( f3_t a, f3_t b ){return a * f3_inv( b );} \
  void bhvm_lop_ar2_div_dp_a_vb_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_exp_dp_vy_s 2660066683
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_exp_dp_vy_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_exp_dp_vy_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_exp_dp_vy_s_f2( f2_t a, f2_t b ){return a * b;} \
  static inline f3_t bhvm_lop_ar2_exp_dp_vy_s_f3( f3_t a, f3_t b ){return a * b;} \
  void bhvm_lop_ar2_exp_dp_vy_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_inv_dp_vy_s 2297448235
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_inv_dp_vy_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_inv_dp_vy_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_inv_dp_vy_s_f2( f2_t a, f2_t b ){return -a*b*b;} \
  static inline f3_t bhvm_lop_ar2_inv_dp_vy_s_f3( f3_t a, f3_t b ){return -a*b*b;} \
  void bhvm_lop_ar2_inv_dp_vy_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_lgst_dp_vy_s 3179503032
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_lgst_dp_vy_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_lgst_dp_vy_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_lgst_dp_vy_s_f2( f2_t a, f2_t b ){return b * ( 1.0 - b ) * a;} \
  static inline f3_t bhvm_lop_ar2_lgst_dp_vy_s_f3( f3_t a, f3_t b ){return b * ( 1.0 - b ) * a;} \
  void bhvm_lop_ar2_lgst_dp_vy_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_lgst_hard_dp_vy_s 43757894
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_lgst_hard_dp_vy_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_lgst_hard_dp_vy_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_lgst_hard_dp_vy_s_f2( f2_t a, f2_t b ){return ( b <  0.0 ) ? 0.0 : ( b > 1.0 ) ? 0.0 : 0.25 * a;} \
  static inline f3_t bhvm_lop_ar2_lgst_hard_dp_vy_s_f3( f3_t a, f3_t b ){return ( b <  0.0 ) ? 0.0 : ( b > 1.0 ) ? 0.0 : 0.25 * a;} \
  void bhvm_lop_ar2_lgst_hard_dp_vy_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_lgst_leaky_dp_vy_s 3630882849
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_lgst_leaky_dp_vy_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_lgst_leaky_dp_vy_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_lgst_leaky_dp_vy_s_f2( f2_t a, f2_t b ){return ( b <  0.0 ) ? 0.01 * a : ( b > 1.0 ) ? 0.01 * a : 0.25 * a;} \
  static inline f3_t bhvm_lop_ar2_lgst_leaky_dp_vy_s_f3( f3_t a, f3_t b ){return ( b <  0.0 ) ? 0.01 * a : ( b > 1.0 ) ? 0.01 * a : 0.25 * a;} \
  void bhvm_lop_ar2_lgst_leaky_dp_vy_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_tanh_dp_vy_s 1171553755
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_tanh_dp_vy_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_tanh_dp_vy_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_tanh_dp_vy_s_f2( f2_t a, f2_t b ){return ( 1.0 - f3_sqr( b ) ) * a;} \
  static inline f3_t bhvm_lop_ar2_tanh_dp_vy_s_f3( f3_t a, f3_t b ){return ( 1.0 - f3_sqr( b ) ) * a;} \
  void bhvm_lop_ar2_tanh_dp_vy_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_tanh_hard_dp_vy_s 158363315
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_tanh_hard_dp_vy_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_tanh_hard_dp_vy_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_tanh_hard_dp_vy_s_f2( f2_t a, f2_t b ){return ( b < -1.0 ) ?  0.0 : ( b > 1.0 ) ? 0.0 : a;} \
  static inline f3_t bhvm_lop_ar2_tanh_hard_dp_vy_s_f3( f3_t a, f3_t b ){return ( b < -1.0 ) ?  0.0 : ( b > 1.0 ) ? 0.0 : a;} \
  void bhvm_lop_ar2_tanh_hard_dp_vy_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_tanh_leaky_dp_vy_s 3405524042
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_tanh_leaky_dp_vy_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_tanh_leaky_dp_vy_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_tanh_leaky_dp_vy_s_f2( f2_t a, f2_t b ){return ( b < -1.0 ) ?  0.01 * a : ( b > 1.0 ) ? 0.01 * a : a;} \
  static inline f3_t bhvm_lop_ar2_tanh_leaky_dp_vy_s_f3( f3_t a, f3_t b ){return ( b < -1.0 ) ?  0.01 * a : ( b > 1.0 ) ? 0.01 * a : a;} \
  void bhvm_lop_ar2_tanh_leaky_dp_vy_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_softplus_dp_vy_s 3718749382
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_softplus_dp_vy_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_softplus_dp_vy_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_softplus_dp_vy_s_f2( f2_t a, f2_t b ){f3_t u = exp( b ); return a * ( u - 1.0 ) * f3_inv( u );} \
  static inline f3_t bhvm_lop_ar2_softplus_dp_vy_s_f3( f3_t a, f3_t b ){f3_t u = exp( b ); return a * ( u - 1.0 ) * f3_inv( u );} \
  void bhvm_lop_ar2_softplus_dp_vy_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_relu_dp_vy_s 943382932
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_relu_dp_vy_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_relu_dp_vy_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_relu_dp_vy_s_f2( f2_t a, f2_t b ){return b > 0 ? a : 0;} \
  static inline f3_t bhvm_lop_ar2_relu_dp_vy_s_f3( f3_t a, f3_t b ){return b > 0 ? a : 0;} \
  void bhvm_lop_ar2_relu_dp_vy_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_relu_leaky_dp_vy_s 704824309
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_relu_leaky_dp_vy_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_relu_leaky_dp_vy_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_relu_leaky_dp_vy_s_f2( f2_t a, f2_t b ){return b > 0 ? a : 0.01 * a;} \
  static inline f3_t bhvm_lop_ar2_relu_leaky_dp_vy_s_f3( f3_t a, f3_t b ){return b > 0 ? a : 0.01 * a;} \
  void bhvm_lop_ar2_relu_leaky_dp_vy_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_equal_s 3498142479
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_equal_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_equal_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_equal_s_f2( f2_t a, f2_t b ){return a == b ? 1 : -1;} \
  static inline f3_t bhvm_lop_ar2_equal_s_f3( f3_t a, f3_t b ){return a == b ? 1 : -1;} \
  void bhvm_lop_ar2_equal_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_larger_s 3685391226
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_larger_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_larger_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_larger_s_f2( f2_t a, f2_t b ){return a >  b ? 1 : -1;} \
  static inline f3_t bhvm_lop_ar2_larger_s_f3( f3_t a, f3_t b ){return a >  b ? 1 : -1;} \
  void bhvm_lop_ar2_larger_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_smaller_s 3196623035
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_smaller_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_smaller_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_smaller_s_f2( f2_t a, f2_t b ){return a <  b ? 1 : -1;} \
  static inline f3_t bhvm_lop_ar2_smaller_s_f3( f3_t a, f3_t b ){return a <  b ? 1 : -1;} \
  void bhvm_lop_ar2_smaller_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_larger_equal_s 1705623121
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_larger_equal_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_larger_equal_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_larger_equal_s_f2( f2_t a, f2_t b ){return a >= b ? 1 : -1;} \
  static inline f3_t bhvm_lop_ar2_larger_equal_s_f3( f3_t a, f3_t b ){return a >= b ? 1 : -1;} \
  void bhvm_lop_ar2_larger_equal_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_smaller_equal_s 1297058080
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_smaller_equal_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_smaller_equal_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_smaller_equal_s_f2( f2_t a, f2_t b ){return a <= b ? 1 : -1;} \
  static inline f3_t bhvm_lop_ar2_smaller_equal_s_f3( f3_t a, f3_t b ){return a <= b ? 1 : -1;} \
  void bhvm_lop_ar2_smaller_equal_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_logic_and_s 394038367
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_logic_and_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_logic_and_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_logic_and_s_f2( f2_t a, f2_t b ){return ( ( a > 0 ) && ( b > 0 ) ) ? 1 : -1;} \
  static inline f3_t bhvm_lop_ar2_logic_and_s_f3( f3_t a, f3_t b ){return ( ( a > 0 ) && ( b > 0 ) ) ? 1 : -1;} \
  void bhvm_lop_ar2_logic_and_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_logic_or_s 3332782907
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_logic_or_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_logic_or_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_logic_or_s_f2( f2_t a, f2_t b ){return ( ( a > 0 ) || ( b > 0 ) ) ? 1 : -1;} \
  static inline f3_t bhvm_lop_ar2_logic_or_s_f3( f3_t a, f3_t b ){return ( ( a > 0 ) || ( b > 0 ) ) ? 1 : -1;} \
  void bhvm_lop_ar2_logic_or_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define BETH_EXPAND_GROUP_bhvm_lop_ar2 \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2 ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_add_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_sub_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_sub_sqr_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_div_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_mul_body_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_mul_vvv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_mul_vsv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_mul_vvs_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_mul_mvv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_mul_vmv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_mul_vvm_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_mul_acc_vvv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_mul_acc_vsv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_mul_acc_vvs_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_mul_acc_mvv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_mul_acc_vmv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_mul_acc_vvm_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_div_dp_a_vb_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_exp_dp_vy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_inv_dp_vy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_lgst_dp_vy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_lgst_hard_dp_vy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_lgst_leaky_dp_vy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_tanh_dp_vy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_tanh_hard_dp_vy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_tanh_leaky_dp_vy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_softplus_dp_vy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_relu_dp_vy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_relu_leaky_dp_vy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_equal_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_larger_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_smaller_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_larger_equal_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_smaller_equal_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_logic_and_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_logic_or_s ); \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_add_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_sub_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_sub_sqr_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_div_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_body_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_vvv_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_vsv_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_vvs_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_mvv_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_vmv_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_vvm_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_acc_vvv_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_acc_vsv_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_acc_vvs_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_acc_mvv_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_acc_vmv_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_acc_vvm_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_div_dp_a_vb_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_exp_dp_vy_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_inv_dp_vy_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_lgst_dp_vy_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_lgst_hard_dp_vy_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_lgst_leaky_dp_vy_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_tanh_dp_vy_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_tanh_hard_dp_vy_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_tanh_leaky_dp_vy_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_softplus_dp_vy_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_relu_dp_vy_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_relu_leaky_dp_vy_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_equal_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_larger_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_smaller_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_larger_equal_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_smaller_equal_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_logic_and_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_logic_or_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_lop_ar3

#define TYPEOF_bhvm_lop_ar3 2763087237
#define TYPEOF_bhvm_lop_ar3_s 1512942695
#define TYPEOF_bhvm_lop_ar3_div_dp_b_vab_s 2834205167
#define BETH_EXPAND_ITEM_bhvm_lop_ar3_div_dp_b_vab_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar3_div_dp_b_vab_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar3_div_dp_b_vab_s_f2( f2_t a, f2_t b, f2_t c ){return -a * b * f2_inv( c * c );} \
  static inline f3_t bhvm_lop_ar3_div_dp_b_vab_s_f3( f3_t a, f3_t b, f3_t c ){return -a * b * f3_inv( c * c );} \
  void bhvm_lop_ar3_div_dp_b_vab_s_f( tp_t tknit, vc_t a, vc_t b, vc_t c, vd_t r, sz_t s );
#define BETH_EXPAND_GROUP_bhvm_lop_ar3 \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar3 ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar3_div_dp_b_vab_s ); \
  BETH_EXPAND_ITEM_bhvm_lop_ar3_div_dp_b_vab_s

/**********************************************************************************************************************/

vd_t bhvm_planted_lop_signal_handler( const bcore_signal_s* o );

#endif // BHVM_PLANTED_LOP_H
