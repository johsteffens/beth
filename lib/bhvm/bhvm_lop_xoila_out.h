/** This file was generated from beth-plant source code.
 *  Compiling Agent : xoico_compiler (C) 2020 J.B.Steffens
 *  Last File Update: 2020-08-07T21:18:46Z
 *
 *  Copyright and License of this File:
 *
 *  Generated code inherits the copyright and license of the underlying beth-plant source code.
 *  Source code defining this file is distributed across following files:
 *
 *  bhvm_lop.h
 *
 */

#ifndef BHVM_LOP_XOILA_OUT_H
#define BHVM_LOP_XOILA_OUT_H

#include "bcore_control.h"

//To force a rebuild of this target by the plant-compiler, reset the hash key value below to 0.
#define HKEYOF_bhvm_lop_xoila_out 14679653145571918460

#define TYPEOF_bhvm_lop_xoila_out 0x1731BA367C516089ull

/**********************************************************************************************************************/
// source: bhvm_lop.h

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_lop

#define TYPEOF_bhvm_lop 0xEDEDE46D725F71C6ull
#define TYPEOF_bhvm_lop_s 0x318F09B0D6351918ull
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

#define TYPEOF_bhvm_lop_ar0 0xE8B0A1ADEEC6216Cull
#define TYPEOF_bhvm_lop_ar0_s 0x7BD680212F91DD6Eull
#define TYPEOF_bhvm_lop_ar0_zro_s 0xF87FB24FE8F263B4ull
#define BETH_EXPAND_ITEM_bhvm_lop_ar0_zro_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar0_zro_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar0_zro_s_f2( void ){return 0;} \
  static inline f3_t bhvm_lop_ar0_zro_s_f3( void ){return 0;} \
  void bhvm_lop_ar0_zro_s_f( tp_t tknit, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar0_one_s 0xCF120CA3E507BBC5ull
#define BETH_EXPAND_ITEM_bhvm_lop_ar0_one_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar0_one_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar0_one_s_f2( void ){return 1;} \
  static inline f3_t bhvm_lop_ar0_one_s_f3( void ){return 1;} \
  void bhvm_lop_ar0_one_s_f( tp_t tknit, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar0_nul_dp_v_s 0x8BB836D68F9A8AFAull
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

#define TYPEOF_bhvm_lop_ar1 0xE8B0A2ADEEC6231Full
#define TYPEOF_bhvm_lop_ar1_s 0x85B19921357D4F29ull
#define TYPEOF_bhvm_lop_ar1_cpy_s 0x01DC559A7399E708ull
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_cpy_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_cpy_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_cpy_s_f2( f2_t a ){return  a;} \
  static inline f3_t bhvm_lop_ar1_cpy_s_f3( f3_t a ){return  a;} \
  void bhvm_lop_ar1_cpy_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_identity_s 0xADAD6A1EC200F65Cull
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_identity_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_identity_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_identity_s_f2( f2_t a ){return  a;} \
  static inline f3_t bhvm_lop_ar1_identity_s_f3( f3_t a ){return  a;} \
  void bhvm_lop_ar1_identity_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_neg_s 0x7E3C4B490C67B790ull
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_neg_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_neg_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_neg_s_f2( f2_t a ){return -a;} \
  static inline f3_t bhvm_lop_ar1_neg_s_f3( f3_t a ){return -a;} \
  void bhvm_lop_ar1_neg_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_floor_s 0x95A00E0DC238D5E8ull
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_floor_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_floor_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_floor_s_f2( f2_t a ){return floorf(a);} \
  static inline f3_t bhvm_lop_ar1_floor_s_f3( f3_t a ){return  floor(a);} \
  void bhvm_lop_ar1_floor_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_ceil_s 0x6FDA8FD53947F67Bull
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_ceil_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_ceil_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_ceil_s_f2( f2_t a ){return  ceilf(a);} \
  static inline f3_t bhvm_lop_ar1_ceil_s_f3( f3_t a ){return   ceil(a);} \
  void bhvm_lop_ar1_ceil_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_exp_s 0x69E70AC277BAEA99ull
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_exp_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_exp_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_exp_s_f2( f2_t a ){return f2_exp(a);} \
  static inline f3_t bhvm_lop_ar1_exp_s_f3( f3_t a ){return f3_exp(a);} \
  void bhvm_lop_ar1_exp_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_log_s 0x17D882F093BC919Cull
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_log_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_log_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_log_s_f2( f2_t a ){return f2_log(a);} \
  static inline f3_t bhvm_lop_ar1_log_s_f3( f3_t a ){return f3_log(a);} \
  void bhvm_lop_ar1_log_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_inv_s 0xBC3FE01889D6CABDull
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_inv_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_inv_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_inv_s_f2( f2_t a ){return f2_inv(a);} \
  static inline f3_t bhvm_lop_ar1_inv_s_f3( f3_t a ){return f3_inv(a);} \
  void bhvm_lop_ar1_inv_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_abs_s 0x2D138BFD69C9B94Aull
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_abs_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_abs_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_abs_s_f2( f2_t a ){return f2_abs(a);} \
  static inline f3_t bhvm_lop_ar1_abs_s_f3( f3_t a ){return f3_abs(a);} \
  void bhvm_lop_ar1_abs_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_sig_s 0x2A6ECEA7BF492C87ull
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_sig_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_sig_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_sig_s_f2( f2_t a ){return f2_sig(a);} \
  static inline f3_t bhvm_lop_ar1_sig_s_f3( f3_t a ){return f3_sig(a);} \
  void bhvm_lop_ar1_sig_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_l1_s 0x3C36F856B5775E59ull
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_l1_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_l1_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_l1_s_f2( f2_t a ){return f2_abs(a);} \
  static inline f3_t bhvm_lop_ar1_l1_s_f3( f3_t a ){return f3_abs(a);} \
  void bhvm_lop_ar1_l1_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_sqr_s 0xDDB70A727E154B30ull
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_sqr_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_sqr_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_sqr_s_f2( f2_t a ){return f2_sqr(a);} \
  static inline f3_t bhvm_lop_ar1_sqr_s_f3( f3_t a ){return f3_sqr(a);} \
  void bhvm_lop_ar1_sqr_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_sqrsum_s 0x4BD2195E645E46CBull
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_sqrsum_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_sqrsum_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_sqrsum_s_f2( f2_t a ){return f2_sqr(a);} \
  static inline f3_t bhvm_lop_ar1_sqrsum_s_f3( f3_t a ){return f3_sqr(a);} \
  void bhvm_lop_ar1_sqrsum_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_srt_s 0xA4951E8A50822739ull
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_srt_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_srt_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_srt_s_f2( f2_t a ){return f2_srt(a);} \
  static inline f3_t bhvm_lop_ar1_srt_s_f3( f3_t a ){return f3_srt(a);} \
  void bhvm_lop_ar1_srt_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_tanh_s 0xE0A0C134E99BAC13ull
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_tanh_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_tanh_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_tanh_s_f2( f2_t a ){return ( a < 350 ) ? ( 1.0 - ( 2.0 / ( exp( 2.0 * ( f3_t )a ) + 1.0 ) ) ) : 1.0;} \
  static inline f3_t bhvm_lop_ar1_tanh_s_f3( f3_t a ){return ( a < 350 ) ? ( 1.0 - ( 2.0 / ( exp( 2.0 * ( f3_t )a ) + 1.0 ) ) ) : 1.0;} \
  void bhvm_lop_ar1_tanh_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_tanh_hard_s 0xCE4BC21E4611254Dull
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_tanh_hard_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_tanh_hard_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_tanh_hard_s_f2( f2_t a ){return ( a < -1.0 ) ? -1.0 : ( a > 1.0 ) ? 1.0 : a;} \
  static inline f3_t bhvm_lop_ar1_tanh_hard_s_f3( f3_t a ){return ( a < -1.0 ) ? -1.0 : ( a > 1.0 ) ? 1.0 : a;} \
  void bhvm_lop_ar1_tanh_hard_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_tanh_leaky_s 0xDE773C9510C3A8EEull
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_tanh_leaky_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_tanh_leaky_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_tanh_leaky_s_f2( f2_t a ){return ( a < -1.0 ) ? -1.0 + 0.01 * ( a + 1.0 ) : ( a > 1.0 ) ? 1.0 + 0.01 * ( a - 1.0 ) : a;} \
  static inline f3_t bhvm_lop_ar1_tanh_leaky_s_f3( f3_t a ){return ( a < -1.0 ) ? -1.0 + 0.01 * ( a + 1.0 ) : ( a > 1.0 ) ? 1.0 + 0.01 * ( a - 1.0 ) : a;} \
  void bhvm_lop_ar1_tanh_leaky_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_sigm_s 0xDB07590A4C94E8C2ull
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_sigm_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_sigm_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_sigm_s_f2( f2_t a ){return ( a > -700 ) ? ( 1.0 / ( 1.0 + exp( -( f3_t )a ) ) ) : 0;} \
  static inline f3_t bhvm_lop_ar1_sigm_s_f3( f3_t a ){return ( a > -700 ) ? ( 1.0 / ( 1.0 + exp( -( f3_t )a ) ) ) : 0;} \
  void bhvm_lop_ar1_sigm_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_sigm_hard_s 0x917773910F17313Eull
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_sigm_hard_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_sigm_hard_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_sigm_hard_s_f2( f2_t a ){return ( a < -2.0 ) ? 0.0 : ( a > 2.0 ) ? 1.0 : 0.25 * ( a + 2.0 );} \
  static inline f3_t bhvm_lop_ar1_sigm_hard_s_f3( f3_t a ){return ( a < -2.0 ) ? 0.0 : ( a > 2.0 ) ? 1.0 : 0.25 * ( a + 2.0 );} \
  void bhvm_lop_ar1_sigm_hard_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_sigm_leaky_s 0x291BE7611220A437ull
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_sigm_leaky_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_sigm_leaky_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_sigm_leaky_s_f2( f2_t a ){return ( a < -2.0 ) ? 0.01 * ( a + 2.0 ) : ( a > 2.0 ) ? 1.0 + 0.01 * ( a - 2.0 ) : 0.25 * ( a + 2.0 );} \
  static inline f3_t bhvm_lop_ar1_sigm_leaky_s_f3( f3_t a ){return ( a < -2.0 ) ? 0.01 * ( a + 2.0 ) : ( a > 2.0 ) ? 1.0 + 0.01 * ( a - 2.0 ) : 0.25 * ( a + 2.0 );} \
  void bhvm_lop_ar1_sigm_leaky_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_softplus_s 0x4CF72F80F9EEC232ull
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_softplus_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_softplus_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_softplus_s_f2( f2_t a ){return ( a < 700 ) ? log( 1.0 + exp( ( f3_t )a ) ) : a;} \
  static inline f3_t bhvm_lop_ar1_softplus_s_f3( f3_t a ){return ( a < 700 ) ? log( 1.0 + exp( ( f3_t )a ) ) : a;} \
  void bhvm_lop_ar1_softplus_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_relu_s 0x43471A3B7C98FEC0ull
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_relu_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_relu_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_relu_s_f2( f2_t a ){return a > 0 ? a : 0;} \
  static inline f3_t bhvm_lop_ar1_relu_s_f3( f3_t a ){return a > 0 ? a : 0;} \
  void bhvm_lop_ar1_relu_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_relu_leaky_s 0x94AAD6820A87001Dull
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_relu_leaky_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_relu_leaky_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_relu_leaky_s_f2( f2_t a ){return a > 0 ? a : a * 0.01;} \
  static inline f3_t bhvm_lop_ar1_relu_leaky_s_f3( f3_t a ){return a > 0 ? a : a * 0.01;} \
  void bhvm_lop_ar1_relu_leaky_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_acc_s 0xE3A003F270202E83ull
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_acc_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_acc_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_acc_s_f2( f2_t a ){return  a;} \
  static inline f3_t bhvm_lop_ar1_acc_s_f3( f3_t a ){return  a;} \
  void bhvm_lop_ar1_acc_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar1_accn_s 0x519F35F39FCC1827ull
#define BETH_EXPAND_ITEM_bhvm_lop_ar1_accn_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar1_accn_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar1_accn_s_f2( f2_t a ){return -a;} \
  static inline f3_t bhvm_lop_ar1_accn_s_f3( f3_t a ){return -a;} \
  void bhvm_lop_ar1_accn_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s );
#define BETH_EXPAND_GROUP_bhvm_lop_ar1 \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1 ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_cpy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_identity_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_neg_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_floor_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_ceil_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_exp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_log_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_inv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_abs_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_sig_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_l1_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_sqr_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_sqrsum_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_srt_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_tanh_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_tanh_hard_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_tanh_leaky_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_sigm_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_sigm_hard_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_sigm_leaky_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_softplus_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_relu_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_relu_leaky_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_acc_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar1_accn_s ); \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_cpy_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_identity_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_neg_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_floor_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_ceil_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_exp_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_log_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_inv_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_abs_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_sig_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_l1_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_sqr_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_sqrsum_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_srt_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_tanh_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_tanh_hard_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_tanh_leaky_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_sigm_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_sigm_hard_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_sigm_leaky_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_softplus_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_relu_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_relu_leaky_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_acc_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar1_accn_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_lop_ar2

#define TYPEOF_bhvm_lop_ar2 0xE8B0A3ADEEC624D2ull
#define TYPEOF_bhvm_lop_ar2_s 0x8D00CA21393F646Cull
#define TYPEOF_bhvm_lop_ar2_sub_sqrsum_s 0xDF32BB7AC92A505Full
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_sub_sqrsum_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_sub_sqrsum_s ) \
    {aware_t _;}; \
  void bhvm_lop_ar2_sub_sqrsum_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_sub_l1_s 0x6874177F640240C5ull
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_sub_l1_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_sub_l1_s ) \
    {aware_t _;}; \
  void bhvm_lop_ar2_sub_l1_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_mul_body_s 0xBB251554C8207956ull
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
#define TYPEOF_bhvm_lop_ar2_mul_mvv_s 0x4093ED84D6F45151ull
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_mvv_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_mul_mvv_s ) \
    {aware_t _;}; \
  static inline void bhvm_lop_ar2_mul_mvv_s_f_m( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t rows, sz_t cols ){bhvm_lop_ar2_mul_body_s_f_mv_cv( tknit, a, b, r, rows, cols );}
#define TYPEOF_bhvm_lop_ar2_mul_vmv_s 0x93A2F0D8CCC17D83ull
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_vmv_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_mul_vmv_s ) \
    {aware_t _;}; \
  static inline void bhvm_lop_ar2_mul_vmv_s_f_m( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t rows, sz_t cols ){bhvm_lop_ar2_mul_body_s_f_vm_cv( tknit, a, b, r, rows, cols );}
#define TYPEOF_bhvm_lop_ar2_mul_vvm_s 0x38B1668677286A6Dull
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_vvm_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_mul_vvm_s ) \
    {aware_t _;}; \
  static inline void bhvm_lop_ar2_mul_vvm_s_f_m( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t rows, sz_t cols ){bhvm_lop_ar2_mul_body_s_f_vv_cm( tknit, a, b, r, rows, cols );}
#define TYPEOF_bhvm_lop_ar2_mul_acc_mvv_s 0xFB683DB424BC243Full
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_acc_mvv_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_mul_acc_mvv_s ) \
    {aware_t _;}; \
  static inline void bhvm_lop_ar2_mul_acc_mvv_s_f_m( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t rows, sz_t cols ){bhvm_lop_ar2_mul_body_s_f_mv_av( tknit, a, b, r, rows, cols );}
#define TYPEOF_bhvm_lop_ar2_mul_acc_vmv_s 0x823751D198A1BE51ull
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_acc_vmv_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_mul_acc_vmv_s ) \
    {aware_t _;}; \
  static inline void bhvm_lop_ar2_mul_acc_vmv_s_f_m( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t rows, sz_t cols ){bhvm_lop_ar2_mul_body_s_f_vm_av( tknit, a, b, r, rows, cols );}
#define TYPEOF_bhvm_lop_ar2_mul_acc_vvm_s 0xC76EA21237281CBBull
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_acc_vvm_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_mul_acc_vvm_s ) \
    {aware_t _;}; \
  static inline void bhvm_lop_ar2_mul_acc_vvm_s_f_m( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t rows, sz_t cols ){bhvm_lop_ar2_mul_body_s_f_vv_am( tknit, a, b, r, rows, cols );}
#define TYPEOF_bhvm_lop_ar2_exp_dp_zyf_s 0x725308C873D35C53ull
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_exp_dp_zyf_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_exp_dp_zyf_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_exp_dp_zyf_s_f2( f2_t a, f2_t b ){return a * b;} \
  static inline f3_t bhvm_lop_ar2_exp_dp_zyf_s_f3( f3_t a, f3_t b ){return a * b;} \
  void bhvm_lop_ar2_exp_dp_zyf_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_log_dp_zaf_s 0x9DD4475C69E229E4ull
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_log_dp_zaf_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_log_dp_zaf_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_log_dp_zaf_s_f2( f2_t a, f2_t b ){return a * f2_inv( b );} \
  static inline f3_t bhvm_lop_ar2_log_dp_zaf_s_f3( f3_t a, f3_t b ){return a * f3_inv( b );} \
  void bhvm_lop_ar2_log_dp_zaf_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_inv_dp_zyf_s 0x03011181B9565BA3ull
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_inv_dp_zyf_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_inv_dp_zyf_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_inv_dp_zyf_s_f2( f2_t a, f2_t b ){return -a*b*b;} \
  static inline f3_t bhvm_lop_ar2_inv_dp_zyf_s_f3( f3_t a, f3_t b ){return -a*b*b;} \
  void bhvm_lop_ar2_inv_dp_zyf_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_sqr_dp_zaf_s 0xEA285519DAEE518Cull
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_sqr_dp_zaf_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_sqr_dp_zaf_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_sqr_dp_zaf_s_f2( f2_t a, f2_t b ){return  2*a*b;} \
  static inline f3_t bhvm_lop_ar2_sqr_dp_zaf_s_f3( f3_t a, f3_t b ){return  2*a*b;} \
  void bhvm_lop_ar2_sqr_dp_zaf_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_srt_dp_zyf_s 0x5D59392455D6CA7Full
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_srt_dp_zyf_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_srt_dp_zyf_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_srt_dp_zyf_s_f2( f2_t a, f2_t b ){return a * f2_inv( b );} \
  static inline f3_t bhvm_lop_ar2_srt_dp_zyf_s_f3( f3_t a, f3_t b ){return a * f3_inv( b );} \
  void bhvm_lop_ar2_srt_dp_zyf_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_abs_dp_zaf_s 0x03AC9CA9A12D51AAull
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_abs_dp_zaf_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_abs_dp_zaf_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_abs_dp_zaf_s_f2( f2_t a, f2_t b ){return b >= 0 ? a : -a;} \
  static inline f3_t bhvm_lop_ar2_abs_dp_zaf_s_f3( f3_t a, f3_t b ){return b >= 0 ? a : -a;} \
  void bhvm_lop_ar2_abs_dp_zaf_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_sigm_dp_zyf_s 0xC319A3D16226D2E4ull
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_sigm_dp_zyf_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_sigm_dp_zyf_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_sigm_dp_zyf_s_f2( f2_t a, f2_t b ){return b * ( 1.0 - b ) * a;} \
  static inline f3_t bhvm_lop_ar2_sigm_dp_zyf_s_f3( f3_t a, f3_t b ){return b * ( 1.0 - b ) * a;} \
  void bhvm_lop_ar2_sigm_dp_zyf_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_sigm_hard_dp_zyf_s 0xDF3181FD9A8B7806ull
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_sigm_hard_dp_zyf_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_sigm_hard_dp_zyf_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_sigm_hard_dp_zyf_s_f2( f2_t a, f2_t b ){return ( b <  0.0 ) ? 0.0 : ( b > 1.0 ) ? 0.0 : 0.25 * a;} \
  static inline f3_t bhvm_lop_ar2_sigm_hard_dp_zyf_s_f3( f3_t a, f3_t b ){return ( b <  0.0 ) ? 0.0 : ( b > 1.0 ) ? 0.0 : 0.25 * a;} \
  void bhvm_lop_ar2_sigm_hard_dp_zyf_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_sigm_leaky_dp_zyf_s 0x05C68A6A10B6014Full
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_sigm_leaky_dp_zyf_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_sigm_leaky_dp_zyf_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_sigm_leaky_dp_zyf_s_f2( f2_t a, f2_t b ){return ( b <  0.0 ) ? 0.01 * a : ( b > 1.0 ) ? 0.01 * a : 0.25 * a;} \
  static inline f3_t bhvm_lop_ar2_sigm_leaky_dp_zyf_s_f3( f3_t a, f3_t b ){return ( b <  0.0 ) ? 0.01 * a : ( b > 1.0 ) ? 0.01 * a : 0.25 * a;} \
  void bhvm_lop_ar2_sigm_leaky_dp_zyf_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_tanh_dp_zyf_s 0x7C0F8684729F7F13ull
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_tanh_dp_zyf_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_tanh_dp_zyf_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_tanh_dp_zyf_s_f2( f2_t a, f2_t b ){return ( 1.0 - f3_sqr( b ) ) * a;} \
  static inline f3_t bhvm_lop_ar2_tanh_dp_zyf_s_f3( f3_t a, f3_t b ){return ( 1.0 - f3_sqr( b ) ) * a;} \
  void bhvm_lop_ar2_tanh_dp_zyf_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_tanh_hard_dp_zyf_s 0xE31BE336632E888Bull
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_tanh_hard_dp_zyf_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_tanh_hard_dp_zyf_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_tanh_hard_dp_zyf_s_f2( f2_t a, f2_t b ){return ( b < -1.0 ) ?  0.0 : ( b > 1.0 ) ? 0.0 : a;} \
  static inline f3_t bhvm_lop_ar2_tanh_hard_dp_zyf_s_f3( f3_t a, f3_t b ){return ( b < -1.0 ) ?  0.0 : ( b > 1.0 ) ? 0.0 : a;} \
  void bhvm_lop_ar2_tanh_hard_dp_zyf_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_tanh_leaky_dp_zyf_s 0x674686FEA0E60390ull
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_tanh_leaky_dp_zyf_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_tanh_leaky_dp_zyf_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_tanh_leaky_dp_zyf_s_f2( f2_t a, f2_t b ){return ( b < -1.0 ) ?  0.01 * a : ( b > 1.0 ) ? 0.01 * a : a;} \
  static inline f3_t bhvm_lop_ar2_tanh_leaky_dp_zyf_s_f3( f3_t a, f3_t b ){return ( b < -1.0 ) ?  0.01 * a : ( b > 1.0 ) ? 0.01 * a : a;} \
  void bhvm_lop_ar2_tanh_leaky_dp_zyf_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_softplus_dp_zyf_s 0x20D51657411D12A4ull
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_softplus_dp_zyf_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_softplus_dp_zyf_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_softplus_dp_zyf_s_f2( f2_t a, f2_t b ){f3_t u = exp( b ); return a * ( u - 1.0 ) * f3_inv( u );} \
  static inline f3_t bhvm_lop_ar2_softplus_dp_zyf_s_f3( f3_t a, f3_t b ){f3_t u = exp( b ); return a * ( u - 1.0 ) * f3_inv( u );} \
  void bhvm_lop_ar2_softplus_dp_zyf_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_relu_dp_zyf_s 0x4732E32CAF832192ull
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_relu_dp_zyf_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_relu_dp_zyf_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_relu_dp_zyf_s_f2( f2_t a, f2_t b ){return b > 0 ? a : 0;} \
  static inline f3_t bhvm_lop_ar2_relu_dp_zyf_s_f3( f3_t a, f3_t b ){return b > 0 ? a : 0;} \
  void bhvm_lop_ar2_relu_dp_zyf_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_relu_leaky_dp_zyf_s 0x480EBB403B21479Dull
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_relu_leaky_dp_zyf_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_relu_leaky_dp_zyf_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_relu_leaky_dp_zyf_s_f2( f2_t a, f2_t b ){return b > 0 ? a : 0.01 * a;} \
  static inline f3_t bhvm_lop_ar2_relu_leaky_dp_zyf_s_f3( f3_t a, f3_t b ){return b > 0 ? a : 0.01 * a;} \
  void bhvm_lop_ar2_relu_leaky_dp_zyf_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_branch_vvvv_dp_azg_s 0x53792099857A04CCull
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_branch_vvvv_dp_azg_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_branch_vvvv_dp_azg_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_branch_vvvv_dp_azg_s_f2( f2_t a, f2_t b ){return a > 0 ? b : 0;} \
  static inline f3_t bhvm_lop_ar2_branch_vvvv_dp_azg_s_f3( f3_t a, f3_t b ){return a > 0 ? b : 0;} \
  void bhvm_lop_ar2_branch_vvvv_dp_azg_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_branch_vvvv_dp_azh_s 0x6F58AB9995CCE53Dull
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_branch_vvvv_dp_azh_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_branch_vvvv_dp_azh_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_branch_vvvv_dp_azh_s_f2( f2_t a, f2_t b ){return a > 0 ? 0 : b;} \
  static inline f3_t bhvm_lop_ar2_branch_vvvv_dp_azh_s_f3( f3_t a, f3_t b ){return a > 0 ? 0 : b;} \
  void bhvm_lop_ar2_branch_vvvv_dp_azh_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_equal_s 0x00B5206604580E0Full
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_equal_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_equal_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_equal_s_f2( f2_t a, f2_t b ){return a == b ? 1 : -1;} \
  static inline f3_t bhvm_lop_ar2_equal_s_f3( f3_t a, f3_t b ){return a == b ? 1 : -1;} \
  void bhvm_lop_ar2_equal_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_unequal_s 0x238112F7CA0F67AAull
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_unequal_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_unequal_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_unequal_s_f2( f2_t a, f2_t b ){return a != b ? 1 : -1;} \
  static inline f3_t bhvm_lop_ar2_unequal_s_f3( f3_t a, f3_t b ){return a != b ? 1 : -1;} \
  void bhvm_lop_ar2_unequal_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_larger_s 0xC1BCAEA25C242DDAull
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_larger_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_larger_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_larger_s_f2( f2_t a, f2_t b ){return a >  b ? 1 : -1;} \
  static inline f3_t bhvm_lop_ar2_larger_s_f3( f3_t a, f3_t b ){return a >  b ? 1 : -1;} \
  void bhvm_lop_ar2_larger_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_smaller_s 0x20BE0C1AC45B35BBull
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_smaller_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_smaller_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_smaller_s_f2( f2_t a, f2_t b ){return a <  b ? 1 : -1;} \
  static inline f3_t bhvm_lop_ar2_smaller_s_f3( f3_t a, f3_t b ){return a <  b ? 1 : -1;} \
  void bhvm_lop_ar2_smaller_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_larger_equal_s 0xD21DDD5D1726CB31ull
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_larger_equal_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_larger_equal_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_larger_equal_s_f2( f2_t a, f2_t b ){return a >= b ? 1 : -1;} \
  static inline f3_t bhvm_lop_ar2_larger_equal_s_f3( f3_t a, f3_t b ){return a >= b ? 1 : -1;} \
  void bhvm_lop_ar2_larger_equal_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_smaller_equal_s 0xA9041505DF490A60ull
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_smaller_equal_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_smaller_equal_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_smaller_equal_s_f2( f2_t a, f2_t b ){return a <= b ? 1 : -1;} \
  static inline f3_t bhvm_lop_ar2_smaller_equal_s_f3( f3_t a, f3_t b ){return a <= b ? 1 : -1;} \
  void bhvm_lop_ar2_smaller_equal_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_logic_and_s 0xC69491AAC992FFFFull
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_logic_and_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_logic_and_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_logic_and_s_f2( f2_t a, f2_t b ){return ( ( a > 0 ) && ( b > 0 ) ) ? 1 : -1;} \
  static inline f3_t bhvm_lop_ar2_logic_and_s_f3( f3_t a, f3_t b ){return ( ( a > 0 ) && ( b > 0 ) ) ? 1 : -1;} \
  void bhvm_lop_ar2_logic_and_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define TYPEOF_bhvm_lop_ar2_logic_or_s 0x72B23BF80AE8713Bull
#define BETH_EXPAND_ITEM_bhvm_lop_ar2_logic_or_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar2_logic_or_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar2_logic_or_s_f2( f2_t a, f2_t b ){return ( ( a > 0 ) || ( b > 0 ) ) ? 1 : -1;} \
  static inline f3_t bhvm_lop_ar2_logic_or_s_f3( f3_t a, f3_t b ){return ( ( a > 0 ) || ( b > 0 ) ) ? 1 : -1;} \
  void bhvm_lop_ar2_logic_or_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
#define BETH_EXPAND_GROUP_bhvm_lop_ar2 \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2 ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_sub_sqrsum_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_sub_l1_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_mul_body_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_mul_mvv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_mul_vmv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_mul_vvm_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_mul_acc_mvv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_mul_acc_vmv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_mul_acc_vvm_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_exp_dp_zyf_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_log_dp_zaf_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_inv_dp_zyf_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_sqr_dp_zaf_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_srt_dp_zyf_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_abs_dp_zaf_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_sigm_dp_zyf_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_sigm_hard_dp_zyf_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_sigm_leaky_dp_zyf_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_tanh_dp_zyf_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_tanh_hard_dp_zyf_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_tanh_leaky_dp_zyf_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_softplus_dp_zyf_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_relu_dp_zyf_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_relu_leaky_dp_zyf_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_branch_vvvv_dp_azg_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_branch_vvvv_dp_azh_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_equal_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_unequal_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_larger_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_smaller_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_larger_equal_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_smaller_equal_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_logic_and_s ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar2_logic_or_s ); \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_sub_sqrsum_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_sub_l1_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_body_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_mvv_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_vmv_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_vvm_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_acc_mvv_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_acc_vmv_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_mul_acc_vvm_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_exp_dp_zyf_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_log_dp_zaf_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_inv_dp_zyf_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_sqr_dp_zaf_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_srt_dp_zyf_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_abs_dp_zaf_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_sigm_dp_zyf_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_sigm_hard_dp_zyf_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_sigm_leaky_dp_zyf_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_tanh_dp_zyf_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_tanh_hard_dp_zyf_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_tanh_leaky_dp_zyf_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_softplus_dp_zyf_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_relu_dp_zyf_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_relu_leaky_dp_zyf_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_branch_vvvv_dp_azg_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_branch_vvvv_dp_azh_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_equal_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_unequal_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_larger_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_smaller_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_larger_equal_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_smaller_equal_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_logic_and_s \
  BETH_EXPAND_ITEM_bhvm_lop_ar2_logic_or_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_lop_ar3

#define TYPEOF_bhvm_lop_ar3 0xE8B0A4ADEEC62685ull
#define TYPEOF_bhvm_lop_ar3_s 0x97B503213FE2FD47ull
#define TYPEOF_bhvm_lop_ar3_branch_vvvv_s 0x0AEBD12D8003DA45ull
#define BETH_EXPAND_ITEM_bhvm_lop_ar3_branch_vvvv_s \
  BCORE_DECLARE_OBJECT( bhvm_lop_ar3_branch_vvvv_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_lop_ar3_branch_vvvv_s_f2( f2_t a, f2_t b, f2_t c ){return a > 0 ? b : c;} \
  static inline f3_t bhvm_lop_ar3_branch_vvvv_s_f3( f3_t a, f3_t b, f3_t c ){return a > 0 ? b : c;} \
  void bhvm_lop_ar3_branch_vvvv_s_f( tp_t tknit, vc_t a, vc_t b, vc_t c, vd_t r, sz_t s );
#define BETH_EXPAND_GROUP_bhvm_lop_ar3 \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar3 ); \
  BCORE_FORWARD_OBJECT( bhvm_lop_ar3_branch_vvvv_s ); \
  BETH_EXPAND_ITEM_bhvm_lop_ar3_branch_vvvv_s

/**********************************************************************************************************************/

vd_t bhvm_lop_xoila_out_signal_handler( const bcore_signal_s* o );

#endif // BHVM_LOP_XOILA_OUT_H
// BETH_PLANT_SIGNATURE 1284301651798765898
