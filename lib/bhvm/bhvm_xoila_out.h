/** This file was generated from beth-plant source code.
 *  Compiling Agent : xoico_compiler (C) 2020 J.B.Steffens
 *  Last File Update: 2020-08-06T09:53:50Z
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

#ifndef BHVM_XOILA_OUT_H
#define BHVM_XOILA_OUT_H

#include "bcore_control.h"

//To force a rebuild of this target by the plant-compiler, reset the hash key value below to 0.
#define HKEYOF_bhvm_xoila_out 1852163040

#define TYPEOF_bhvm_xoila_out 3853157877

/**********************************************************************************************************************/
// source: bhvm_holor.h

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm

#define TYPEOF_bhvm 754264922
#define TYPEOF_bhvm_s 672708292
#define TYPEOF_bhvm_shape_s 3092477010
#define BETH_EXPAND_ITEM_bhvm_shape_s \
  BCORE_DECLARE_OBJECT( bhvm_shape_s ) \
    {BCORE_ARRAY_DYN_SOLID_STATIC_S( sz_t, );}; \
  static inline bhvm_shape_s* bhvm_shape_s_set_space( bhvm_shape_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_bhvm_shape_s, ( bcore_array* )o, size ); return o; } \
  static inline bhvm_shape_s* bhvm_shape_s_set_size( bhvm_shape_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_bhvm_shape_s, ( bcore_array* )o, size ); return o; } \
  static inline bhvm_shape_s* bhvm_shape_s_clear( bhvm_shape_s* o ) { bcore_array_t_set_space( TYPEOF_bhvm_shape_s, ( bcore_array* )o, 0 ); return o; } \
  static inline sz_t* bhvm_shape_s_push_c( bhvm_shape_s* o, const sz_t* v ) { bcore_array_t_push( TYPEOF_bhvm_shape_s, ( bcore_array* )o, sr_twc( TYPEOF_sz_t, v ) ); return &o->data[ o->size - 1 ]; } \
  static inline sz_t* bhvm_shape_s_push_d( bhvm_shape_s* o,       sz_t* v ) { bcore_array_t_push( TYPEOF_bhvm_shape_s, ( bcore_array* )o, sr_tsd( TYPEOF_sz_t, v ) ); return &o->data[ o->size - 1 ]; } \
  static inline sz_t* bhvm_shape_s_push( bhvm_shape_s* o ) \
  { \
      bcore_array_t_push( TYPEOF_bhvm_shape_s, ( bcore_array* )o, sr_null() ); \
      return &o->data[ o->size - 1 ]; \
  }
#define TYPEOF_bhvm_value_s 227825478
#define BETH_EXPAND_ITEM_bhvm_value_s \
  BCORE_DECLARE_OBJECT( bhvm_value_s ) \
    {BCORE_ARRAY_DYN_SOLID_TYPED_S( );}; \
  static inline bhvm_value_s* bhvm_value_s_set_space( bhvm_value_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_bhvm_value_s, ( bcore_array* )o, size ); return o; } \
  static inline bhvm_value_s* bhvm_value_s_set_size( bhvm_value_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_bhvm_value_s, ( bcore_array* )o, size ); return o; } \
  static inline bhvm_value_s* bhvm_value_s_clear( bhvm_value_s* o ) { bcore_array_t_set_space( TYPEOF_bhvm_value_s, ( bcore_array* )o, 0 ); return o; }
#define TYPEOF_bhvm_holor_s 2922699917
#define BETH_EXPAND_ITEM_bhvm_holor_s \
  BCORE_DECLARE_OBJECT( bhvm_holor_s ) \
    {aware_t _;bhvm_shape_s s;bhvm_value_s v;}; \
  void bhvm_holor_s_copy_typed( bhvm_holor_s* o, tp_t type, vc_t src );
#define TYPEOF_bhvm_holor_adl_s 329679305
#define BETH_EXPAND_ITEM_bhvm_holor_adl_s \
  BCORE_DECLARE_OBJECT( bhvm_holor_adl_s ) \
    {aware_t _;BCORE_ARRAY_DYN_LINK_STATIC_S( bhvm_holor_s, );}; \
  void bhvm_holor_adl_s_copy_typed( bhvm_holor_adl_s* o, tp_t type, vc_t src ); \
  static inline bhvm_holor_adl_s* bhvm_holor_adl_s_set_space( bhvm_holor_adl_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_bhvm_holor_adl_s, ( bcore_array* )o, size ); return o; } \
  static inline bhvm_holor_adl_s* bhvm_holor_adl_s_set_size( bhvm_holor_adl_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_bhvm_holor_adl_s, ( bcore_array* )o, size ); return o; } \
  static inline bhvm_holor_adl_s* bhvm_holor_adl_s_clear( bhvm_holor_adl_s* o ) { bcore_array_t_set_space( TYPEOF_bhvm_holor_adl_s, ( bcore_array* )o, 0 ); return o; } \
  static inline bhvm_holor_s* bhvm_holor_adl_s_push_c( bhvm_holor_adl_s* o, const bhvm_holor_s* v ) { bcore_array_t_push( TYPEOF_bhvm_holor_adl_s, ( bcore_array* )o, sr_twc( TYPEOF_bhvm_holor_s, v ) ); return o->data[ o->size - 1 ]; } \
  static inline bhvm_holor_s* bhvm_holor_adl_s_push_d( bhvm_holor_adl_s* o,       bhvm_holor_s* v ) { bcore_array_t_push( TYPEOF_bhvm_holor_adl_s, ( bcore_array* )o, sr_tsd( TYPEOF_bhvm_holor_s, v ) ); return o->data[ o->size - 1 ]; } \
  static inline bhvm_holor_s* bhvm_holor_adl_s_push( bhvm_holor_adl_s* o ) \
  { \
      bcore_array_t_push( TYPEOF_bhvm_holor_adl_s, ( bcore_array* )o, sr_t_create( TYPEOF_bhvm_holor_s ) ); \
      return o->data[ o->size - 1 ]; \
  }
#define TYPEOF_bhvm_holor_ads_s 48695984
#define BETH_EXPAND_ITEM_bhvm_holor_ads_s \
  BCORE_DECLARE_OBJECT( bhvm_holor_ads_s ) \
    {aware_t _;BCORE_ARRAY_DYN_SOLID_STATIC_S( bhvm_holor_s, );}; \
  static inline bhvm_holor_ads_s* bhvm_holor_ads_s_set_space( bhvm_holor_ads_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_bhvm_holor_ads_s, ( bcore_array* )o, size ); return o; } \
  static inline bhvm_holor_ads_s* bhvm_holor_ads_s_set_size( bhvm_holor_ads_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_bhvm_holor_ads_s, ( bcore_array* )o, size ); return o; } \
  static inline bhvm_holor_ads_s* bhvm_holor_ads_s_clear( bhvm_holor_ads_s* o ) { bcore_array_t_set_space( TYPEOF_bhvm_holor_ads_s, ( bcore_array* )o, 0 ); return o; } \
  static inline bhvm_holor_s* bhvm_holor_ads_s_push_c( bhvm_holor_ads_s* o, const bhvm_holor_s* v ) { bcore_array_t_push( TYPEOF_bhvm_holor_ads_s, ( bcore_array* )o, sr_twc( TYPEOF_bhvm_holor_s, v ) ); return &o->data[ o->size - 1 ]; } \
  static inline bhvm_holor_s* bhvm_holor_ads_s_push_d( bhvm_holor_ads_s* o,       bhvm_holor_s* v ) { bcore_array_t_push( TYPEOF_bhvm_holor_ads_s, ( bcore_array* )o, sr_tsd( TYPEOF_bhvm_holor_s, v ) ); return &o->data[ o->size - 1 ]; } \
  static inline bhvm_holor_s* bhvm_holor_ads_s_push( bhvm_holor_ads_s* o ) \
  { \
      bcore_array_t_push( TYPEOF_bhvm_holor_ads_s, ( bcore_array* )o, sr_null() ); \
      return &o->data[ o->size - 1 ]; \
  }
#define TYPEOF_bhvm_holor_mdl_s 1507510669
#define BETH_EXPAND_ITEM_bhvm_holor_mdl_s \
  BCORE_DECLARE_OBJECT( bhvm_holor_mdl_s ) \
    {aware_t _;BCORE_ARRAY_DYN_LINK_STATIC_S( bhvm_holor_adl_s, );}; \
  static inline bhvm_holor_mdl_s* bhvm_holor_mdl_s_set_space( bhvm_holor_mdl_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_bhvm_holor_mdl_s, ( bcore_array* )o, size ); return o; } \
  static inline bhvm_holor_mdl_s* bhvm_holor_mdl_s_set_size( bhvm_holor_mdl_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_bhvm_holor_mdl_s, ( bcore_array* )o, size ); return o; } \
  static inline bhvm_holor_mdl_s* bhvm_holor_mdl_s_clear( bhvm_holor_mdl_s* o ) { bcore_array_t_set_space( TYPEOF_bhvm_holor_mdl_s, ( bcore_array* )o, 0 ); return o; } \
  static inline bhvm_holor_adl_s* bhvm_holor_mdl_s_push_c( bhvm_holor_mdl_s* o, const bhvm_holor_adl_s* v ) { bcore_array_t_push( TYPEOF_bhvm_holor_mdl_s, ( bcore_array* )o, sr_twc( TYPEOF_bhvm_holor_adl_s, v ) ); return o->data[ o->size - 1 ]; } \
  static inline bhvm_holor_adl_s* bhvm_holor_mdl_s_push_d( bhvm_holor_mdl_s* o,       bhvm_holor_adl_s* v ) { bcore_array_t_push( TYPEOF_bhvm_holor_mdl_s, ( bcore_array* )o, sr_tsd( TYPEOF_bhvm_holor_adl_s, v ) ); return o->data[ o->size - 1 ]; } \
  static inline bhvm_holor_adl_s* bhvm_holor_mdl_s_push( bhvm_holor_mdl_s* o ) \
  { \
      bcore_array_t_push( TYPEOF_bhvm_holor_mdl_s, ( bcore_array* )o, sr_t_create( TYPEOF_bhvm_holor_adl_s ) ); \
      return o->data[ o->size - 1 ]; \
  }
#define TYPEOF_bhvm_stats_s 4125200492
#define BETH_EXPAND_ITEM_bhvm_stats_s \
  BCORE_DECLARE_OBJECT( bhvm_stats_s ) \
    {aware_t _;f3_t min;f3_t max;f3_t sum;f3_t sqr_sum;sz_t size;};
#define BETH_EXPAND_GROUP_bhvm \
  BCORE_FORWARD_OBJECT( bhvm ); \
  BCORE_FORWARD_OBJECT( bhvm_shape_s ); \
  BCORE_FORWARD_OBJECT( bhvm_value_s ); \
  BCORE_FORWARD_OBJECT( bhvm_holor_s ); \
  BCORE_FORWARD_OBJECT( bhvm_holor_adl_s ); \
  BCORE_FORWARD_OBJECT( bhvm_holor_ads_s ); \
  BCORE_FORWARD_OBJECT( bhvm_holor_mdl_s ); \
  BCORE_FORWARD_OBJECT( bhvm_stats_s ); \
  BETH_EXPAND_ITEM_bhvm_shape_s \
  BETH_EXPAND_ITEM_bhvm_value_s \
  BETH_EXPAND_ITEM_bhvm_holor_s \
  BETH_EXPAND_ITEM_bhvm_holor_adl_s \
  BETH_EXPAND_ITEM_bhvm_holor_ads_s \
  BETH_EXPAND_ITEM_bhvm_holor_mdl_s \
  BETH_EXPAND_ITEM_bhvm_stats_s

/**********************************************************************************************************************/
// source: bhvm_hop.h

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hop

#define TYPEOF_bhvm_hop 3785952858
#define TYPEOF_bhvm_hop_s 3322859972
#define BETH_EXPAND_GROUP_bhvm_hop \
  BCORE_FORWARD_OBJECT( bhvm_hop ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar0 ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1 ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_eci ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2 ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_eci ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar3 ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar3_eci ); \
  BETH_EXPAND_GROUP_bhvm_hop_ar0 \
  BETH_EXPAND_GROUP_bhvm_hop_ar1 \
  BETH_EXPAND_GROUP_bhvm_hop_ar1_eci \
  BETH_EXPAND_GROUP_bhvm_hop_ar2 \
  BETH_EXPAND_GROUP_bhvm_hop_ar2_eci \
  BETH_EXPAND_GROUP_bhvm_hop_ar3 \
  BETH_EXPAND_GROUP_bhvm_hop_ar3_eci

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hop_ar0

#define TYPEOF_bhvm_hop_ar0 3633122584
#define TYPEOF_bhvm_hop_ar0_s 2679910530
#define TYPEOF_bhvm_hop_ar0_zro_s 125223160
#define BETH_EXPAND_ITEM_bhvm_hop_ar0_zro_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar0_zro_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar0_zro_s_f( bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar0_one_s 2519434969
#define BETH_EXPAND_ITEM_bhvm_hop_ar0_one_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar0_one_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar0_one_s_f( bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar0_nul_dp_s 4230901099
#define BETH_EXPAND_ITEM_bhvm_hop_ar0_nul_dp_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar0_nul_dp_s ) \
    {aware_t _;}; \
  static inline void bhvm_hop_ar0_nul_dp_s_f( bhvm_holor_s* r ){/* no action */}
#define BETH_EXPAND_GROUP_bhvm_hop_ar0 \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar0 ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar0_zro_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar0_one_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar0_nul_dp_s ); \
  BETH_EXPAND_ITEM_bhvm_hop_ar0_zro_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar0_one_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar0_nul_dp_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hop_ar1

#define TYPEOF_bhvm_hop_ar1 3649900203
#define TYPEOF_bhvm_hop_ar1_s 484699325
#define TYPEOF_bhvm_hop_ar1_cpy_s 289404708
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_cpy_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_cpy_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar1_cpy_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar1_neg_s 1232253556
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_neg_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_neg_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar1_neg_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar1_floor_s 1498087652
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_floor_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_floor_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar1_floor_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar1_ceil_s 2751150223
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_ceil_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_ceil_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar1_ceil_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar1_exp_s 2274302589
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_exp_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_exp_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar1_exp_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar1_log_s 2496886144
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_log_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_log_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar1_log_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar1_inv_s 3653712297
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_inv_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_inv_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar1_inv_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar1_abs_s 1203095958
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_abs_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_abs_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar1_abs_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar1_sig_s 1903425387
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_sig_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_sig_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar1_sig_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar1_l1_s 3022561237
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_l1_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_l1_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar1_l1_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar1_sqr_s 2905402524
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_sqr_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_sqr_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar1_sqr_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar1_srt_s 2295743837
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_srt_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_srt_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar1_srt_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar1_sqrsum_s 1568796527
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_sqrsum_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_sqrsum_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar1_sqrsum_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar1_sigm_s 2510435958
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_sigm_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_sigm_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar1_sigm_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar1_sigm_hard_s 2623329466
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_sigm_hard_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_sigm_hard_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar1_sigm_hard_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar1_sigm_leaky_s 1452951731
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_sigm_leaky_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_sigm_leaky_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar1_sigm_leaky_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar1_tanh_s 3968541479
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_tanh_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_tanh_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar1_tanh_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar1_tanh_hard_s 907451113
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_tanh_hard_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_tanh_hard_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar1_tanh_hard_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar1_tanh_leaky_s 1427011258
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_tanh_leaky_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_tanh_leaky_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar1_tanh_leaky_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar1_softplus_s 3303687518
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_softplus_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_softplus_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar1_softplus_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar1_relu_s 1047003548
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_relu_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_relu_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar1_relu_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar1_relu_leaky_s 998439233
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_relu_leaky_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_relu_leaky_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar1_relu_leaky_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar1_softmax_s 1161063220
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_softmax_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_softmax_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar1_softmax_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar1_neg_dp_zf_s 2808498694
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_neg_dp_zf_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_neg_dp_zf_s ) \
    {aware_t _;}; \
  static inline void bhvm_hop_ar1_neg_dp_zf_s_f( const bhvm_holor_s* a, bhvm_holor_s* r ){assert( a->v.size == r->v.size ); bhvm_lop_ar1_accn_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );}
#define TYPEOF_bhvm_hop_ar1_cat_dp_zf_s 2312193564
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_cat_dp_zf_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_cat_dp_zf_s ) \
    {aware_t _;}; \
  static inline void bhvm_hop_ar1_cat_dp_zf_s_f( const bhvm_holor_s* a, bhvm_holor_s* r ){bhvm_value_s_acc_offs( &a->v, 0, &r->v, 0, r->v.size );}
#define TYPEOF_bhvm_hop_ar1_cat_dp_zg_s 106536599
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_cat_dp_zg_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_cat_dp_zg_s ) \
    {aware_t _;}; \
  static inline void bhvm_hop_ar1_cat_dp_zg_s_f( const bhvm_holor_s* a, bhvm_holor_s* r ){bhvm_value_s_acc_offs( &a->v, a->v.size - r->v.size, &r->v, 0, r->v.size );}
#define TYPEOF_bhvm_hop_ar1_ccat_dp_zf_s 1764183895
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_ccat_dp_zf_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_ccat_dp_zf_s ) \
    {aware_t _;}; \
  static inline void bhvm_hop_ar1_ccat_dp_zf_s_f( const bhvm_holor_s* a, bhvm_holor_s* r ){bhvm_value_s_acc_offs( &a->v, 0, &r->v, 0, r->v.size );}
#define TYPEOF_bhvm_hop_ar1_ccat_dp_zg_s 3969840860
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_ccat_dp_zg_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_ccat_dp_zg_s ) \
    {aware_t _;}; \
  static inline void bhvm_hop_ar1_ccat_dp_zg_s_f( const bhvm_holor_s* a, bhvm_holor_s* r ){bhvm_value_s_acc_offs( &a->v, a->v.size - r->v.size, &r->v, 0, r->v.size );}
#define BETH_EXPAND_GROUP_bhvm_hop_ar1 \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1 ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_cpy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_neg_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_floor_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_ceil_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_exp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_log_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_inv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_abs_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_sig_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_l1_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_sqr_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_srt_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_sqrsum_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_sigm_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_sigm_hard_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_sigm_leaky_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_tanh_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_tanh_hard_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_tanh_leaky_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_softplus_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_relu_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_relu_leaky_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_softmax_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_neg_dp_zf_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_cat_dp_zf_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_cat_dp_zg_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_ccat_dp_zf_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_ccat_dp_zg_s ); \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_cpy_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_neg_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_floor_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_ceil_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_exp_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_log_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_inv_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_abs_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_sig_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_l1_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_sqr_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_srt_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_sqrsum_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_sigm_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_sigm_hard_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_sigm_leaky_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_tanh_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_tanh_hard_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_tanh_leaky_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_softplus_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_relu_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_relu_leaky_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_softmax_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_neg_dp_zf_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_cat_dp_zf_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_cat_dp_zg_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_ccat_dp_zf_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_ccat_dp_zg_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hop_ar1_eci

#define TYPEOF_bhvm_hop_ar1_eci 4208302195
#define TYPEOF_bhvm_hop_ar1_eci_s 1467501973
#define TYPEOF_bhvm_hop_ar1_eci_cpy_acc_s 2919922234
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_eci_cpy_acc_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_eci_cpy_acc_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_hop_ar1_eci_cpy_acc_s_f2( f2_t a ){return  a;} \
  static inline f3_t bhvm_hop_ar1_eci_cpy_acc_s_f3( f3_t a ){return  a;} \
  void bhvm_hop_ar1_eci_cpy_acc_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar1_eci_neg_acc_s 1616513354
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_eci_neg_acc_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_eci_neg_acc_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_hop_ar1_eci_neg_acc_s_f2( f2_t a ){return -a;} \
  static inline f3_t bhvm_hop_ar1_eci_neg_acc_s_f3( f3_t a ){return -a;} \
  void bhvm_hop_ar1_eci_neg_acc_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar1_eci_inv_acc_s 203704443
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_eci_inv_acc_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_eci_inv_acc_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_hop_ar1_eci_inv_acc_s_f2( f2_t a ){return f2_inv( a );} \
  static inline f3_t bhvm_hop_ar1_eci_inv_acc_s_f3( f3_t a ){return f3_inv( a );} \
  void bhvm_hop_ar1_eci_inv_acc_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar1_eci_cpy_s 3571407164
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_eci_cpy_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_eci_cpy_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_hop_ar1_eci_cpy_s_f2( f2_t a ){return  a;} \
  static inline f3_t bhvm_hop_ar1_eci_cpy_s_f3( f3_t a ){return  a;} \
  void bhvm_hop_ar1_eci_cpy_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar1_eci_neg_s 3125100
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_eci_neg_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_eci_neg_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_hop_ar1_eci_neg_s_f2( f2_t a ){return -a;} \
  static inline f3_t bhvm_hop_ar1_eci_neg_s_f3( f3_t a ){return -a;} \
  void bhvm_hop_ar1_eci_neg_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar1_eci_inv_s 3987392401
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_eci_inv_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_eci_inv_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_hop_ar1_eci_inv_s_f2( f2_t a ){return f2_inv( a );} \
  static inline f3_t bhvm_hop_ar1_eci_inv_s_f3( f3_t a ){return f3_inv( a );} \
  void bhvm_hop_ar1_eci_inv_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
#define BETH_EXPAND_GROUP_bhvm_hop_ar1_eci \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_eci ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_eci_cpy_acc_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_eci_neg_acc_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_eci_inv_acc_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_eci_cpy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_eci_neg_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_eci_inv_s ); \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_eci_cpy_acc_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_eci_neg_acc_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_eci_inv_acc_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_eci_cpy_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_eci_neg_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_eci_inv_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hop_ar2

#define TYPEOF_bhvm_hop_ar2 3666677822
#define TYPEOF_bhvm_hop_ar2_s 1902904896
#define TYPEOF_bhvm_hop_ar2_sub_sqrsum_s 50268387
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_sub_sqrsum_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_sub_sqrsum_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_sub_sqrsum_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_sub_l1_s 4027321873
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_sub_l1_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_sub_l1_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_sub_l1_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_equal_s 3335757939
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_equal_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_equal_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_equal_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_unequal_s 550621358
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_unequal_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_unequal_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_unequal_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_larger_s 4003471110
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_larger_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_larger_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_larger_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_smaller_s 837483239
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_smaller_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_smaller_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_smaller_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_larger_equal_s 1743628029
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_larger_equal_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_larger_equal_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_larger_equal_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_smaller_equal_s 1879132716
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_smaller_equal_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_smaller_equal_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_smaller_equal_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_logic_and_s 1376536315
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_logic_and_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_logic_and_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_logic_and_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_logic_or_s 3001649959
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_logic_or_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_logic_or_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_logic_or_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_abs_dp_zaf_s 748127710
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_abs_dp_zaf_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_abs_dp_zaf_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_abs_dp_zaf_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_exp_dp_zyf_s 2904686959
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_exp_dp_zyf_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_exp_dp_zyf_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_exp_dp_zyf_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_log_dp_zaf_s 2545962112
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_log_dp_zaf_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_log_dp_zaf_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_log_dp_zaf_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_inv_dp_zyf_s 1319162647
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_inv_dp_zyf_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_inv_dp_zyf_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_inv_dp_zyf_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_sqr_dp_zaf_s 202791216
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_sqr_dp_zaf_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_sqr_dp_zaf_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_sqr_dp_zaf_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_srt_dp_zyf_s 657043147
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_srt_dp_zyf_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_srt_dp_zyf_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_srt_dp_zyf_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_sigm_dp_zyf_s 3017808608
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_sigm_dp_zyf_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_sigm_dp_zyf_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_sigm_dp_zyf_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_sigm_hard_dp_zyf_s 2542366682
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_sigm_hard_dp_zyf_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_sigm_hard_dp_zyf_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_sigm_hard_dp_zyf_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_sigm_leaky_dp_zyf_s 246683619
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_sigm_leaky_dp_zyf_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_sigm_leaky_dp_zyf_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_sigm_leaky_dp_zyf_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_tanh_dp_zyf_s 464161343
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_tanh_dp_zyf_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_tanh_dp_zyf_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_tanh_dp_zyf_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_tanh_hard_dp_zyf_s 1828921807
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_tanh_hard_dp_zyf_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_tanh_hard_dp_zyf_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_tanh_hard_dp_zyf_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_tanh_leaky_dp_zyf_s 397911988
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_tanh_leaky_dp_zyf_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_tanh_leaky_dp_zyf_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_tanh_leaky_dp_zyf_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_softplus_dp_zyf_s 1215527464
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_softplus_dp_zyf_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_softplus_dp_zyf_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_softplus_dp_zyf_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_relu_dp_zyf_s 3167183190
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_relu_dp_zyf_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_relu_dp_zyf_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_relu_dp_zyf_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_relu_leaky_dp_zyf_s 1242969321
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_relu_leaky_dp_zyf_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_relu_leaky_dp_zyf_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_relu_leaky_dp_zyf_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_softmax_dp_zyf_s 1853375756
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_softmax_dp_zyf_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_softmax_dp_zyf_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_softmax_dp_zyf_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_mul_mvv_s 2339926677
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_mvv_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_mul_mvv_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_mul_mvv_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_mul_vmv_s 1014422215
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_vmv_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_mul_vmv_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_mul_vmv_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_mul_vvm_s 4076644353
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_vvm_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_mul_vvm_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_mul_vvm_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_mul_tvv_s 1197108990
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_tvv_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_mul_tvv_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_mul_tvv_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_mul_vtv_s 955500986
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_vtv_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_mul_vtv_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_mul_vtv_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_mul_mmm_s 1861644527
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_mmm_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_mul_mmm_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_mul_mmm_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_mul_mtm_s 2878123466
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_mtm_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_mul_mtm_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_mul_mtm_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_mul_tmm_s 3206841424
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_tmm_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_mul_tmm_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_mul_tmm_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_mul_ttm_s 3904137477
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_ttm_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_mul_ttm_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_mul_ttm_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_mul_acc_mvv_s 3243738491
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_acc_mvv_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_mul_acc_mvv_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_mul_acc_mvv_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_mul_acc_vmv_s 3735208925
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_acc_vmv_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_mul_acc_vmv_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_mul_acc_vmv_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_mul_acc_tvv_s 3138299620
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_acc_tvv_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_mul_acc_tvv_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_mul_acc_tvv_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_mul_acc_vtv_s 3060532856
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_acc_vtv_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_mul_acc_vtv_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_mul_acc_vtv_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_mul_acc_vvm_s 1515366295
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_acc_vvm_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_mul_acc_vvm_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_mul_acc_vvm_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_mul_acc_mmm_s 2444394949
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_acc_mmm_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_mul_acc_mmm_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_mul_acc_mmm_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_mul_acc_mtm_s 1705419024
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_acc_mtm_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_mul_acc_mtm_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_mul_acc_mtm_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_mul_acc_tmm_s 696194558
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_acc_tmm_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_mul_acc_tmm_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_mul_acc_tmm_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_mul_acc_ttm_s 3111666707
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_acc_ttm_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_mul_acc_ttm_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_mul_acc_ttm_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_cat_s 1433501047
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_cat_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_cat_s ) \
    {aware_t _;}; \
  static inline void bhvm_hop_ar2_cat_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r ){bhvm_holor_s_cat( a, b, r );}
#define TYPEOF_bhvm_hop_ar2_ccat_s 673538486
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_ccat_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_ccat_s ) \
    {aware_t _;}; \
  static inline void bhvm_hop_ar2_ccat_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r ){bhvm_holor_s_ccat( a, b, r );}
#define BETH_EXPAND_GROUP_bhvm_hop_ar2 \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2 ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_sub_sqrsum_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_sub_l1_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_equal_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_unequal_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_larger_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_smaller_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_larger_equal_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_smaller_equal_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_logic_and_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_logic_or_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_abs_dp_zaf_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_exp_dp_zyf_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_log_dp_zaf_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_inv_dp_zyf_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_sqr_dp_zaf_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_srt_dp_zyf_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_sigm_dp_zyf_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_sigm_hard_dp_zyf_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_sigm_leaky_dp_zyf_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_tanh_dp_zyf_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_tanh_hard_dp_zyf_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_tanh_leaky_dp_zyf_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_softplus_dp_zyf_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_relu_dp_zyf_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_relu_leaky_dp_zyf_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_softmax_dp_zyf_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_mul_mvv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_mul_vmv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_mul_vvm_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_mul_tvv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_mul_vtv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_mul_mmm_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_mul_mtm_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_mul_tmm_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_mul_ttm_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_mul_acc_mvv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_mul_acc_vmv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_mul_acc_tvv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_mul_acc_vtv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_mul_acc_vvm_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_mul_acc_mmm_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_mul_acc_mtm_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_mul_acc_tmm_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_mul_acc_ttm_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_cat_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_ccat_s ); \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_sub_sqrsum_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_sub_l1_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_equal_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_unequal_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_larger_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_smaller_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_larger_equal_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_smaller_equal_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_logic_and_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_logic_or_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_abs_dp_zaf_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_exp_dp_zyf_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_log_dp_zaf_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_inv_dp_zyf_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_sqr_dp_zaf_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_srt_dp_zyf_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_sigm_dp_zyf_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_sigm_hard_dp_zyf_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_sigm_leaky_dp_zyf_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_tanh_dp_zyf_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_tanh_hard_dp_zyf_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_tanh_leaky_dp_zyf_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_softplus_dp_zyf_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_relu_dp_zyf_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_relu_leaky_dp_zyf_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_softmax_dp_zyf_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_mvv_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_vmv_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_vvm_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_tvv_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_vtv_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_mmm_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_mtm_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_tmm_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_ttm_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_acc_mvv_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_acc_vmv_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_acc_tvv_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_acc_vtv_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_acc_vvm_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_acc_mmm_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_acc_mtm_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_acc_tmm_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_acc_ttm_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_cat_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_ccat_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hop_ar2_eci

#define TYPEOF_bhvm_hop_ar2_eci 4104960270
#define TYPEOF_bhvm_hop_ar2_eci_s 2429241296
#define TYPEOF_bhvm_hop_ar2_eci_add_acc_s 786693886
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_add_acc_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_eci_add_acc_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_hop_ar2_eci_add_acc_s_f2( f2_t a, f2_t b ){return a + b;} \
  static inline f3_t bhvm_hop_ar2_eci_add_acc_s_f3( f3_t a, f3_t b ){return a + b;} \
  void bhvm_hop_ar2_eci_add_acc_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_eci_sub_acc_s 169423755
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_sub_acc_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_eci_sub_acc_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_hop_ar2_eci_sub_acc_s_f2( f2_t a, f2_t b ){return a - b;} \
  static inline f3_t bhvm_hop_ar2_eci_sub_acc_s_f3( f3_t a, f3_t b ){return a - b;} \
  void bhvm_hop_ar2_eci_sub_acc_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_eci_mul_acc_s 1361819831
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_mul_acc_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_eci_mul_acc_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_hop_ar2_eci_mul_acc_s_f2( f2_t a, f2_t b ){return a * b;} \
  static inline f3_t bhvm_hop_ar2_eci_mul_acc_s_f3( f3_t a, f3_t b ){return a * b;} \
  void bhvm_hop_ar2_eci_mul_acc_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_eci_div_acc_s 1906090350
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_div_acc_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_eci_div_acc_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_hop_ar2_eci_div_acc_s_f2( f2_t a, f2_t b ){return a * f2_inv( b );} \
  static inline f3_t bhvm_hop_ar2_eci_div_acc_s_f3( f3_t a, f3_t b ){return a * f3_inv( b );} \
  void bhvm_hop_ar2_eci_div_acc_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_eci_pow_acc_s 3892921091
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_pow_acc_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_eci_pow_acc_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_hop_ar2_eci_pow_acc_s_f2( f2_t a, f2_t b ){return  f2_pow( a, b );} \
  static inline f3_t bhvm_hop_ar2_eci_pow_acc_s_f3( f3_t a, f3_t b ){return  f3_pow( a, b );} \
  void bhvm_hop_ar2_eci_pow_acc_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_eci_add_s 2449851384
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_add_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_eci_add_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_eci_add_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_eci_sub_s 470955713
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_sub_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_eci_sub_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_eci_sub_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_eci_mul_s 2500280021
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_mul_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_eci_mul_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_eci_mul_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_eci_div_s 1136271944
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_div_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_eci_div_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_eci_div_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_eci_pow_s 3973378153
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_pow_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_eci_pow_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_eci_pow_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_eci_iff_dp_b_azg_s 2783868751
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_iff_dp_b_azg_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_eci_iff_dp_b_azg_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_hop_ar2_eci_iff_dp_b_azg_s_f2( f2_t a, f2_t b ){return ( a > 0 ) ? b : 0;} \
  static inline f3_t bhvm_hop_ar2_eci_iff_dp_b_azg_s_f3( f3_t a, f3_t b ){return ( a > 0 ) ? b : 0;} \
  void bhvm_hop_ar2_eci_iff_dp_b_azg_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_eci_iff_dp_c_azh_s 446819985
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_iff_dp_c_azh_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_eci_iff_dp_c_azh_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_hop_ar2_eci_iff_dp_c_azh_s_f2( f2_t a, f2_t b ){return ( a > 0 ) ? 0 : b;} \
  static inline f3_t bhvm_hop_ar2_eci_iff_dp_c_azh_s_f3( f3_t a, f3_t b ){return ( a > 0 ) ? 0 : b;} \
  void bhvm_hop_ar2_eci_iff_dp_c_azh_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define BETH_EXPAND_GROUP_bhvm_hop_ar2_eci \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_eci ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_eci_add_acc_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_eci_sub_acc_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_eci_mul_acc_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_eci_div_acc_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_eci_pow_acc_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_eci_add_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_eci_sub_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_eci_mul_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_eci_div_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_eci_pow_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_eci_logic ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_eci_iff_dp_b_azg_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_eci_iff_dp_c_azh_s ); \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_add_acc_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_sub_acc_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_mul_acc_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_div_acc_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_pow_acc_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_add_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_sub_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_mul_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_div_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_pow_s \
  BETH_EXPAND_GROUP_bhvm_hop_ar2_eci_logic \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_iff_dp_b_azg_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_iff_dp_c_azh_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hop_ar2_eci_logic

#define TYPEOF_bhvm_hop_ar2_eci_logic 2366322535
#define TYPEOF_bhvm_hop_ar2_eci_logic_s 593143361
#define TYPEOF_bhvm_hop_ar2_eci_logic_equal_s 1614856858
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_logic_equal_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_eci_logic_equal_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_hop_ar2_eci_logic_equal_s_f2( f2_t a, f2_t b ){return a == b ? 1 : -1;} \
  static inline f3_t bhvm_hop_ar2_eci_logic_equal_s_f3( f3_t a, f3_t b ){return a == b ? 1 : -1;} \
  void bhvm_hop_ar2_eci_logic_equal_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_eci_logic_unequal_s 4206991803
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_logic_unequal_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_eci_logic_unequal_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_hop_ar2_eci_logic_unequal_s_f2( f2_t a, f2_t b ){return a != b ? 1 : -1;} \
  static inline f3_t bhvm_hop_ar2_eci_logic_unequal_s_f3( f3_t a, f3_t b ){return a != b ? 1 : -1;} \
  void bhvm_hop_ar2_eci_logic_unequal_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_eci_logic_larger_s 3863619497
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_logic_larger_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_eci_logic_larger_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_hop_ar2_eci_logic_larger_s_f2( f2_t a, f2_t b ){return a >  b ? 1 : -1;} \
  static inline f3_t bhvm_hop_ar2_eci_logic_larger_s_f3( f3_t a, f3_t b ){return a >  b ? 1 : -1;} \
  void bhvm_hop_ar2_eci_logic_larger_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_eci_logic_smaller_s 2182465830
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_logic_smaller_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_eci_logic_smaller_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_hop_ar2_eci_logic_smaller_s_f2( f2_t a, f2_t b ){return a <  b ? 1 : -1;} \
  static inline f3_t bhvm_hop_ar2_eci_logic_smaller_s_f3( f3_t a, f3_t b ){return a <  b ? 1 : -1;} \
  void bhvm_hop_ar2_eci_logic_smaller_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_eci_logic_larger_equal_s 838739170
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_logic_larger_equal_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_eci_logic_larger_equal_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_hop_ar2_eci_logic_larger_equal_s_f2( f2_t a, f2_t b ){return a >= b ? 1 : -1;} \
  static inline f3_t bhvm_hop_ar2_eci_logic_larger_equal_s_f3( f3_t a, f3_t b ){return a >= b ? 1 : -1;} \
  void bhvm_hop_ar2_eci_logic_larger_equal_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_eci_logic_smaller_equal_s 374739805
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_logic_smaller_equal_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_eci_logic_smaller_equal_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_hop_ar2_eci_logic_smaller_equal_s_f2( f2_t a, f2_t b ){return a <= b ? 1 : -1;} \
  static inline f3_t bhvm_hop_ar2_eci_logic_smaller_equal_s_f3( f3_t a, f3_t b ){return a <= b ? 1 : -1;} \
  void bhvm_hop_ar2_eci_logic_smaller_equal_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_eci_logic_and_s 3857671755
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_logic_and_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_eci_logic_and_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_hop_ar2_eci_logic_and_s_f2( f2_t a, f2_t b ){return ( ( a > 0 ) && ( b > 0 ) ) ? 1 : -1;} \
  static inline f3_t bhvm_hop_ar2_eci_logic_and_s_f3( f3_t a, f3_t b ){return ( ( a > 0 ) && ( b > 0 ) ) ? 1 : -1;} \
  void bhvm_hop_ar2_eci_logic_and_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_eci_logic_or_s 2477003383
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_logic_or_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_eci_logic_or_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_hop_ar2_eci_logic_or_s_f2( f2_t a, f2_t b ){return ( ( a > 0 ) || ( b > 0 ) ) ? 1 : -1;} \
  static inline f3_t bhvm_hop_ar2_eci_logic_or_s_f3( f3_t a, f3_t b ){return ( ( a > 0 ) || ( b > 0 ) ) ? 1 : -1;} \
  void bhvm_hop_ar2_eci_logic_or_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define BETH_EXPAND_GROUP_bhvm_hop_ar2_eci_logic \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_eci_logic ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_eci_logic_equal_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_eci_logic_unequal_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_eci_logic_larger_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_eci_logic_smaller_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_eci_logic_larger_equal_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_eci_logic_smaller_equal_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_eci_logic_and_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_eci_logic_or_s ); \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_logic_equal_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_logic_unequal_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_logic_larger_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_logic_smaller_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_logic_larger_equal_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_logic_smaller_equal_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_logic_and_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_eci_logic_or_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hop_ar3

#define TYPEOF_bhvm_hop_ar3 3683455441
#define TYPEOF_bhvm_hop_ar3_s 3981872635
#define BETH_EXPAND_GROUP_bhvm_hop_ar3 \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar3 );

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hop_ar3_eci

#define TYPEOF_bhvm_hop_ar3_eci 7208553
#define TYPEOF_bhvm_hop_ar3_eci_s 128954083
#define TYPEOF_bhvm_hop_ar3_eci_iff_s 2735102243
#define BETH_EXPAND_ITEM_bhvm_hop_ar3_eci_iff_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar3_eci_iff_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_hop_ar3_eci_iff_s_f2( f2_t a, f2_t b, f2_t c ){return ( a > 0 ) ? b : c;} \
  static inline f3_t bhvm_hop_ar3_eci_iff_s_f3( f3_t a, f3_t b, f3_t c ){return ( a > 0 ) ? b : c;} \
  void bhvm_hop_ar3_eci_iff_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, const bhvm_holor_s* c, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar3_eci_iff_acc_s 2584949041
#define BETH_EXPAND_ITEM_bhvm_hop_ar3_eci_iff_acc_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar3_eci_iff_acc_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_hop_ar3_eci_iff_acc_s_f2( f2_t a, f2_t b, f2_t c ){return ( a > 0 ) ? b : c;} \
  static inline f3_t bhvm_hop_ar3_eci_iff_acc_s_f3( f3_t a, f3_t b, f3_t c ){return ( a > 0 ) ? b : c;} \
  void bhvm_hop_ar3_eci_iff_acc_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, const bhvm_holor_s* c, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar3_eci_div_dp_zabg_s 476271151
#define BETH_EXPAND_ITEM_bhvm_hop_ar3_eci_div_dp_zabg_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar3_eci_div_dp_zabg_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_hop_ar3_eci_div_dp_zabg_s_f2( f2_t a, f2_t b, f2_t c ){return -a * b * f2_inv( c * c );} \
  static inline f3_t bhvm_hop_ar3_eci_div_dp_zabg_s_f3( f3_t a, f3_t b, f3_t c ){return -a * b * f3_inv( c * c );} \
  void bhvm_hop_ar3_eci_div_dp_zabg_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, const bhvm_holor_s* c, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar3_eci_pow_dp_abzf_s 75365595
#define BETH_EXPAND_ITEM_bhvm_hop_ar3_eci_pow_dp_abzf_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar3_eci_pow_dp_abzf_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_hop_ar3_eci_pow_dp_abzf_s_f2( f2_t a, f2_t b, f2_t c ){return c * b * f2_pow( a, b - 1 );} \
  static inline f3_t bhvm_hop_ar3_eci_pow_dp_abzf_s_f3( f3_t a, f3_t b, f3_t c ){return c * b * f3_pow( a, b - 1 );} \
  void bhvm_hop_ar3_eci_pow_dp_abzf_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, const bhvm_holor_s* c, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar3_eci_pow_dp_ayzg_s 1840913905
#define BETH_EXPAND_ITEM_bhvm_hop_ar3_eci_pow_dp_ayzg_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar3_eci_pow_dp_ayzg_s ) \
    {aware_t _;}; \
  static inline f2_t bhvm_hop_ar3_eci_pow_dp_ayzg_s_f2( f2_t a, f2_t b, f2_t c ){return ( a > 0 ) ? b * c * logf( a ) : 0;} \
  static inline f3_t bhvm_hop_ar3_eci_pow_dp_ayzg_s_f3( f3_t a, f3_t b, f3_t c ){return ( a > 0 ) ? b * c * log ( a ) : 0;} \
  void bhvm_hop_ar3_eci_pow_dp_ayzg_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, const bhvm_holor_s* c, bhvm_holor_s* r );
#define BETH_EXPAND_GROUP_bhvm_hop_ar3_eci \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar3_eci ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar3_eci_iff_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar3_eci_iff_acc_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar3_eci_div_dp_zabg_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar3_eci_pow_dp_abzf_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar3_eci_pow_dp_ayzg_s ); \
  BETH_EXPAND_ITEM_bhvm_hop_ar3_eci_iff_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar3_eci_iff_acc_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar3_eci_div_dp_zabg_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar3_eci_pow_dp_abzf_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar3_eci_pow_dp_ayzg_s

/**********************************************************************************************************************/
// source: bhvm_mcode.h

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_mcode

#define TYPEOF_bhvm_mcode 1834389771
#define TYPEOF_bhvm_mcode_s 3345727837
#define TYPEOF_bhvm_mcode_op_s 2578225005
#define BETH_EXPAND_ITEM_bhvm_mcode_op_s \
  BCORE_DECLARE_OBJECT( bhvm_mcode_op_s ) \
    {aware_t _;bhvm_vop* vop;bhvm_vop_s* p;}; \
  static inline void bhvm_mcode_op_s_copy_x( bhvm_mcode_op_s* o ){o->p = o->vop ? ( bhvm_vop_s* )bhvm_vop_s_get_aware( o->vop ) : NULL;} \
  static inline void bhvm_mcode_op_s_mutated( bhvm_mcode_op_s* o ){bhvm_mcode_op_s_copy_x( o );} \
  static inline void bhvm_mcode_op_s_run( const bhvm_mcode_op_s* o, bhvm_holor_s** ah ){assert( o->p ); assert( o->p->run ); o->p->run( (vc_t)o->vop, ah );}
#define TYPEOF_pclass_ax0 2551261537
#define TYPEOF_pclass_ag0 2251382966
#define TYPEOF_pclass_ax1 2534483918
#define TYPEOF_pclass_ag1 2268160585
#define TYPEOF_bhvm_mcode_node_s 1711993250
#define BETH_EXPAND_ITEM_bhvm_mcode_node_s \
  BCORE_DECLARE_OBJECT( bhvm_mcode_node_s ) \
    {aware_t _;sz_t nidx;sz_t ax0;sz_t ag0;sz_t ax1;sz_t ag1;bl_t param;bl_t adaptive;bl_t cyclic;}; \
  sz_t bhvm_mcode_node_s_get_pclass_idx( const bhvm_mcode_node_s* o, tp_t pclass );
#define TYPEOF_bhvm_mcode_nbase_s 4123399117
#define BETH_EXPAND_ITEM_bhvm_mcode_nbase_s \
  BCORE_DECLARE_OBJECT( bhvm_mcode_nbase_s ) \
    {aware_t _;BCORE_ARRAY_DYN_LINK_STATIC_S( bhvm_mcode_node_s, );}; \
  bhvm_mcode_node_s* bhvm_mcode_nbase_s_push_node( bhvm_mcode_nbase_s* o ); \
  static inline bhvm_mcode_nbase_s* bhvm_mcode_nbase_s_set_space( bhvm_mcode_nbase_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_bhvm_mcode_nbase_s, ( bcore_array* )o, size ); return o; } \
  static inline bhvm_mcode_nbase_s* bhvm_mcode_nbase_s_set_size( bhvm_mcode_nbase_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_bhvm_mcode_nbase_s, ( bcore_array* )o, size ); return o; } \
  static inline bhvm_mcode_nbase_s* bhvm_mcode_nbase_s_clear( bhvm_mcode_nbase_s* o ) { bcore_array_t_set_space( TYPEOF_bhvm_mcode_nbase_s, ( bcore_array* )o, 0 ); return o; } \
  static inline bhvm_mcode_node_s* bhvm_mcode_nbase_s_push_c( bhvm_mcode_nbase_s* o, const bhvm_mcode_node_s* v ) { bcore_array_t_push( TYPEOF_bhvm_mcode_nbase_s, ( bcore_array* )o, sr_twc( TYPEOF_bhvm_mcode_node_s, v ) ); return o->data[ o->size - 1 ]; } \
  static inline bhvm_mcode_node_s* bhvm_mcode_nbase_s_push_d( bhvm_mcode_nbase_s* o,       bhvm_mcode_node_s* v ) { bcore_array_t_push( TYPEOF_bhvm_mcode_nbase_s, ( bcore_array* )o, sr_tsd( TYPEOF_bhvm_mcode_node_s, v ) ); return o->data[ o->size - 1 ]; } \
  static inline bhvm_mcode_node_s* bhvm_mcode_nbase_s_push( bhvm_mcode_nbase_s* o ) \
  { \
      bcore_array_t_push( TYPEOF_bhvm_mcode_nbase_s, ( bcore_array* )o, sr_t_create( TYPEOF_bhvm_mcode_node_s ) ); \
      return o->data[ o->size - 1 ]; \
  }
#define TYPEOF_bhvm_mcode_track_s 946103125
#define BETH_EXPAND_ITEM_bhvm_mcode_track_s \
  BCORE_DECLARE_OBJECT( bhvm_mcode_track_s ) \
    {aware_t _;tp_t name;BCORE_ARRAY_DYN_SOLID_STATIC_S( bhvm_mcode_op_s, );}; \
  void bhvm_mcode_track_s_run( const bhvm_mcode_track_s* o, bhvm_holor_s** ah ); \
  void bhvm_mcode_track_s_run_section( const bhvm_mcode_track_s* o, sz_t start, sz_t size, bhvm_holor_s** ah ); \
  sz_t bhvm_mcode_track_s_vop_push_d( bhvm_mcode_track_s* o, bhvm_vop* vop ); \
  sz_t bhvm_mcode_track_s_vop_push_c( bhvm_mcode_track_s* o, const bhvm_vop* vop ); \
  sz_t bhvm_mcode_track_s_push_copy_from_index( bhvm_mcode_track_s* o, sz_t index ); \
  static inline bhvm_mcode_track_s* bhvm_mcode_track_s_set_space( bhvm_mcode_track_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_bhvm_mcode_track_s, ( bcore_array* )o, size ); return o; } \
  static inline bhvm_mcode_track_s* bhvm_mcode_track_s_set_size( bhvm_mcode_track_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_bhvm_mcode_track_s, ( bcore_array* )o, size ); return o; } \
  static inline bhvm_mcode_track_s* bhvm_mcode_track_s_clear( bhvm_mcode_track_s* o ) { bcore_array_t_set_space( TYPEOF_bhvm_mcode_track_s, ( bcore_array* )o, 0 ); return o; } \
  static inline bhvm_mcode_op_s* bhvm_mcode_track_s_push_c( bhvm_mcode_track_s* o, const bhvm_mcode_op_s* v ) { bcore_array_t_push( TYPEOF_bhvm_mcode_track_s, ( bcore_array* )o, sr_twc( TYPEOF_bhvm_mcode_op_s, v ) ); return &o->data[ o->size - 1 ]; } \
  static inline bhvm_mcode_op_s* bhvm_mcode_track_s_push_d( bhvm_mcode_track_s* o,       bhvm_mcode_op_s* v ) { bcore_array_t_push( TYPEOF_bhvm_mcode_track_s, ( bcore_array* )o, sr_tsd( TYPEOF_bhvm_mcode_op_s, v ) ); return &o->data[ o->size - 1 ]; } \
  static inline bhvm_mcode_op_s* bhvm_mcode_track_s_push( bhvm_mcode_track_s* o ) \
  { \
      bcore_array_t_push( TYPEOF_bhvm_mcode_track_s, ( bcore_array* )o, sr_null() ); \
      return &o->data[ o->size - 1 ]; \
  }
#define TYPEOF_bhvm_mcode_track_adl_s 1870241841
#define BETH_EXPAND_ITEM_bhvm_mcode_track_adl_s \
  BCORE_DECLARE_OBJECT( bhvm_mcode_track_adl_s ) \
    {aware_t _;BCORE_ARRAY_DYN_LINK_STATIC_S( bhvm_mcode_track_s, );}; \
  static inline bhvm_mcode_track_adl_s* bhvm_mcode_track_adl_s_set_space( bhvm_mcode_track_adl_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_bhvm_mcode_track_adl_s, ( bcore_array* )o, size ); return o; } \
  static inline bhvm_mcode_track_adl_s* bhvm_mcode_track_adl_s_set_size( bhvm_mcode_track_adl_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_bhvm_mcode_track_adl_s, ( bcore_array* )o, size ); return o; } \
  static inline bhvm_mcode_track_adl_s* bhvm_mcode_track_adl_s_clear( bhvm_mcode_track_adl_s* o ) { bcore_array_t_set_space( TYPEOF_bhvm_mcode_track_adl_s, ( bcore_array* )o, 0 ); return o; } \
  static inline bhvm_mcode_track_s* bhvm_mcode_track_adl_s_push_c( bhvm_mcode_track_adl_s* o, const bhvm_mcode_track_s* v ) { bcore_array_t_push( TYPEOF_bhvm_mcode_track_adl_s, ( bcore_array* )o, sr_twc( TYPEOF_bhvm_mcode_track_s, v ) ); return o->data[ o->size - 1 ]; } \
  static inline bhvm_mcode_track_s* bhvm_mcode_track_adl_s_push_d( bhvm_mcode_track_adl_s* o,       bhvm_mcode_track_s* v ) { bcore_array_t_push( TYPEOF_bhvm_mcode_track_adl_s, ( bcore_array* )o, sr_tsd( TYPEOF_bhvm_mcode_track_s, v ) ); return o->data[ o->size - 1 ]; } \
  static inline bhvm_mcode_track_s* bhvm_mcode_track_adl_s_push( bhvm_mcode_track_adl_s* o ) \
  { \
      bcore_array_t_push( TYPEOF_bhvm_mcode_track_adl_s, ( bcore_array* )o, sr_t_create( TYPEOF_bhvm_mcode_track_s ) ); \
      return o->data[ o->size - 1 ]; \
  }
#define TYPEOF_bhvm_mcode_lib_s 2233001165
#define BETH_EXPAND_ITEM_bhvm_mcode_lib_s \
  BCORE_DECLARE_OBJECT( bhvm_mcode_lib_s ) \
    {aware_t _;bhvm_mcode_track_adl_s arr;bcore_hmap_tpuz_s map;}; \
  static inline void bhvm_mcode_lib_s_clear( bhvm_mcode_lib_s* o ){bhvm_mcode_track_adl_s_clear( &o->arr ); bcore_hmap_tpuz_s_clear( &o->map );} \
  static inline bl_t bhvm_mcode_lib_s_track_exists( const bhvm_mcode_lib_s* o, tp_t name ){return bcore_hmap_tpuz_s_exists( &o->map, name );} \
  bhvm_mcode_track_s* bhvm_mcode_lib_s_track_get( bhvm_mcode_lib_s* o, tp_t name ); \
  bhvm_mcode_track_s* bhvm_mcode_lib_s_track_get_or_new( bhvm_mcode_lib_s* o, tp_t name ); \
  bhvm_mcode_track_s* bhvm_mcode_lib_s_track_reset( bhvm_mcode_lib_s* o, tp_t name ); \
  static inline void bhvm_mcode_lib_s_track_vop_push_c( bhvm_mcode_lib_s* o, tp_t name, const bhvm_vop* vop ){bhvm_mcode_track_s_vop_push_c( bhvm_mcode_lib_s_track_get_or_new( o, name ), vop );} \
  static inline void bhvm_mcode_lib_s_track_vop_push_d( bhvm_mcode_lib_s* o, tp_t name, bhvm_vop* vop ){bhvm_mcode_track_s_vop_push_d( bhvm_mcode_lib_s_track_get_or_new( o, name ), vop );} \
  void bhvm_mcode_lib_s_track_vop_set_args_push_d( bhvm_mcode_lib_s* o, tp_t name, bhvm_vop* vop, const bhvm_vop_arr_ci_s* arr_ci ); \
  void bhvm_mcode_lib_s_track_push( bhvm_mcode_lib_s* o, tp_t name, tp_t src_name ); \
  void bhvm_mcode_lib_s_track_remove( bhvm_mcode_lib_s* o, tp_t name ); \
  static inline void bhvm_mcode_lib_s_track_run_ah( const bhvm_mcode_lib_s* o, tp_t name, bhvm_holor_s** ah ){bhvm_mcode_track_s* t = bhvm_mcode_lib_s_track_get( (bhvm_mcode_lib_s*)o, name ); if( t ) bhvm_mcode_track_s_run( t, ah );}
#define TYPEOF_bhvm_mcode_frame_s 832185203
#define BETH_EXPAND_ITEM_bhvm_mcode_frame_s \
  BCORE_DECLARE_OBJECT( bhvm_mcode_frame_s ) \
    {aware_t _;bhvm_mcode_lib_s* lib;bhvm_mcode_hbase_s* hbase;bhvm_mcode_nbase_s* nbase;}; \
  void bhvm_mcode_frame_s_mutated( bhvm_mcode_frame_s* o ); \
  void bhvm_mcode_frame_s_copy_x( bhvm_mcode_frame_s* o ); \
  static inline bhvm_mcode_track_s* bhvm_mcode_frame_s_track_get( bhvm_mcode_frame_s* o, tp_t name ){if( !o->lib ) return NULL; return bhvm_mcode_lib_s_track_get( o->lib, name );} \
  static inline void bhvm_mcode_frame_s_track_vop_push_c( bhvm_mcode_frame_s* o, tp_t name, const bhvm_vop* vop ){if( !o->lib ) o->lib = bhvm_mcode_lib_s_create(); bhvm_mcode_lib_s_track_vop_push_c( o->lib, name, vop );} \
  static inline void bhvm_mcode_frame_s_track_vop_push_d( bhvm_mcode_frame_s* o, tp_t name, bhvm_vop* vop ){if( !o->lib ) o->lib = bhvm_mcode_lib_s_create(); bhvm_mcode_lib_s_track_vop_push_d( o->lib, name, vop );} \
  static inline void bhvm_mcode_frame_s_track_vop_set_args_push_d( bhvm_mcode_frame_s* o, tp_t name, bhvm_vop* vop, const bhvm_vop_arr_ci_s* arr_ci ){if( !o->lib ) o->lib = bhvm_mcode_lib_s_create(); bhvm_mcode_lib_s_track_vop_set_args_push_d( o->lib, name, vop, arr_ci );} \
  static inline sz_t bhvm_mcode_frame_s_push_hm( bhvm_mcode_frame_s* o, const bhvm_holor_s* h, const bhvm_mcode_hmeta* m ){if( !o->hbase ) o->hbase = bhvm_mcode_hbase_s_create(); return bhvm_mcode_hbase_s_push_hm(   o->hbase, h, m            );} \
  static inline sz_t bhvm_mcode_frame_s_push_hmc( bhvm_mcode_frame_s* o, const bhvm_holor_s* h, const bhvm_mcode_hmeta* m, char c, bhvm_vop_arr_ci_s* arr_ci ){if( !o->hbase ) o->hbase = bhvm_mcode_hbase_s_create(); return bhvm_mcode_hbase_s_push_hmc(  o->hbase, h, m, c, arr_ci );} \
  static inline bhvm_mcode_node_s* bhvm_mcode_frame_s_push_node( bhvm_mcode_frame_s* o ){if( !o->nbase ) o->nbase = bhvm_mcode_nbase_s_create(); return bhvm_mcode_nbase_s_push_node( o->nbase                  );} \
  static inline void bhvm_mcode_frame_s_track_run( const bhvm_mcode_frame_s* o, tp_t name ){if( !o->lib ) return; bhvm_mcode_lib_s_track_run_ah( o->lib, name, o->hbase->holor_adl.data );}
#define BETH_EXPAND_GROUP_bhvm_mcode \
  BCORE_FORWARD_OBJECT( bhvm_mcode ); \
  BCORE_FORWARD_OBJECT( bhvm_mcode_op_s ); \
  BCORE_FORWARD_OBJECT( bhvm_mcode_node_s ); \
  BCORE_FORWARD_OBJECT( bhvm_mcode_nbase_s ); \
  BCORE_FORWARD_OBJECT( bhvm_mcode_hmeta ); \
  BCORE_FORWARD_OBJECT( bhvm_mcode_hbase ); \
  BCORE_FORWARD_OBJECT( bhvm_mcode_track_s ); \
  BCORE_FORWARD_OBJECT( bhvm_mcode_track_adl_s ); \
  BCORE_FORWARD_OBJECT( bhvm_mcode_lib_s ); \
  BCORE_FORWARD_OBJECT( bhvm_mcode_frame_s ); \
  BETH_EXPAND_ITEM_bhvm_mcode_op_s \
  BETH_EXPAND_ITEM_bhvm_mcode_node_s \
  BETH_EXPAND_ITEM_bhvm_mcode_nbase_s \
  BETH_EXPAND_GROUP_bhvm_mcode_hmeta \
  BETH_EXPAND_GROUP_bhvm_mcode_hbase \
  BETH_EXPAND_ITEM_bhvm_mcode_track_s \
  BETH_EXPAND_ITEM_bhvm_mcode_track_adl_s \
  BETH_EXPAND_ITEM_bhvm_mcode_lib_s \
  BETH_EXPAND_ITEM_bhvm_mcode_frame_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_mcode_hmeta

#define TYPEOF_bhvm_mcode_hmeta 816277129
#define TYPEOF_bhvm_mcode_hmeta_s 4233538115
#define TYPEOF_bhvm_mcode_hmeta_adl_s 2477847419
#define BETH_EXPAND_ITEM_bhvm_mcode_hmeta_adl_s \
  BCORE_DECLARE_OBJECT( bhvm_mcode_hmeta_adl_s ) \
    {aware_t _;BCORE_ARRAY_DYN_LINK_STATIC_S( bhvm_mcode_hmeta, );}; \
  static inline bhvm_mcode_hmeta_adl_s* bhvm_mcode_hmeta_adl_s_set_space( bhvm_mcode_hmeta_adl_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_bhvm_mcode_hmeta_adl_s, ( bcore_array* )o, size ); return o; } \
  static inline bhvm_mcode_hmeta_adl_s* bhvm_mcode_hmeta_adl_s_set_size( bhvm_mcode_hmeta_adl_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_bhvm_mcode_hmeta_adl_s, ( bcore_array* )o, size ); return o; } \
  static inline bhvm_mcode_hmeta_adl_s* bhvm_mcode_hmeta_adl_s_clear( bhvm_mcode_hmeta_adl_s* o ) { bcore_array_t_set_space( TYPEOF_bhvm_mcode_hmeta_adl_s, ( bcore_array* )o, 0 ); return o; } \
  static inline bhvm_mcode_hmeta* bhvm_mcode_hmeta_adl_s_push_c( bhvm_mcode_hmeta_adl_s* o, const bhvm_mcode_hmeta* v ) { bcore_array_t_push( TYPEOF_bhvm_mcode_hmeta_adl_s, ( bcore_array* )o, sr_awc( v ) ); return o->data[ o->size - 1 ]; } \
  static inline bhvm_mcode_hmeta* bhvm_mcode_hmeta_adl_s_push_d( bhvm_mcode_hmeta_adl_s* o,       bhvm_mcode_hmeta* v ) { bcore_array_t_push( TYPEOF_bhvm_mcode_hmeta_adl_s, ( bcore_array* )o, sr_asd( v ) ); return o->data[ o->size - 1 ]; } \
  static inline bhvm_mcode_hmeta* bhvm_mcode_hmeta_adl_s_push_t( bhvm_mcode_hmeta_adl_s* o, tp_t t ) \
  { \
      bcore_trait_assert_satisfied_type( TYPEOF_bhvm_mcode_hmeta, t ); \
      bcore_array_t_push( TYPEOF_bhvm_mcode_hmeta_adl_s, ( bcore_array* )o, sr_t_create( t ) ); \
      return o->data[ o->size - 1 ]; \
  }
#define BETH_EXPAND_GROUP_bhvm_mcode_hmeta \
  BCORE_FORWARD_OBJECT( bhvm_mcode_hmeta ); \
  BCORE_FORWARD_OBJECT( bhvm_mcode_hmeta_adl_s ); \
  typedef tp_t (*bhvm_mcode_hmeta_get_name)( const bhvm_mcode_hmeta* o ); \
  typedef tp_t (*bhvm_mcode_hmeta_get_pclass)( const bhvm_mcode_hmeta* o ); \
  typedef bhvm_mcode_node_s* (*bhvm_mcode_hmeta_get_node)( const bhvm_mcode_hmeta* o ); \
  typedef void (*bhvm_mcode_hmeta_set_node)( bhvm_mcode_hmeta* o, bhvm_mcode_node_s* node ); \
  typedef bl_t (*bhvm_mcode_hmeta_is_rollable)( const bhvm_mcode_hmeta* o ); \
  typedef bl_t (*bhvm_mcode_hmeta_is_active)( const bhvm_mcode_hmeta* o ); \
  typedef bcore_inst* (*bhvm_mcode_hmeta_get_custom)( const bhvm_mcode_hmeta* o ); \
  typedef bcore_inst* (*bhvm_mcode_hmeta_set_custom)( bhvm_mcode_hmeta* o, const bcore_inst* custom ); \
  BCORE_DECLARE_SPECT( bhvm_mcode_hmeta ) \
  { \
      bcore_spect_header_s header; \
      bhvm_mcode_hmeta_get_name get_name; \
      bhvm_mcode_hmeta_get_pclass get_pclass; \
      bhvm_mcode_hmeta_get_node get_node; \
      bhvm_mcode_hmeta_set_node set_node; \
      bhvm_mcode_hmeta_is_rollable is_rollable; \
      bhvm_mcode_hmeta_is_active is_active; \
      bhvm_mcode_hmeta_get_custom get_custom; \
      bhvm_mcode_hmeta_set_custom set_custom; \
  }; \
  static inline bhvm_mcode_hmeta* bhvm_mcode_hmeta_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_bhvm_mcode_hmeta, t ); return ( bhvm_mcode_hmeta* )bcore_inst_t_create( t ); } \
  static inline bl_t bhvm_mcode_hmeta_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_bhvm_mcode_hmeta ); } \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bhvm_mcode_hmeta ) \
  static inline bl_t bhvm_mcode_hmeta_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_bhvm_mcode_hmeta ); } \
  static inline tp_t bhvm_mcode_hmeta_a_get_name( const bhvm_mcode_hmeta* o ) { const bhvm_mcode_hmeta_s* p = bhvm_mcode_hmeta_s_get_aware( o ); assert( p->get_name ); return p->get_name( o ); } \
  static inline bl_t bhvm_mcode_hmeta_a_defines_get_name( const bhvm_mcode_hmeta* o ) { return true; } \
  static inline tp_t bhvm_mcode_hmeta_get_name__( const bhvm_mcode_hmeta* o ){return 0;} \
  static inline tp_t bhvm_mcode_hmeta_a_get_pclass( const bhvm_mcode_hmeta* o ) { const bhvm_mcode_hmeta_s* p = bhvm_mcode_hmeta_s_get_aware( o ); assert( p->get_pclass ); return p->get_pclass( o ); } \
  static inline bl_t bhvm_mcode_hmeta_a_defines_get_pclass( const bhvm_mcode_hmeta* o ) { return true; } \
  static inline tp_t bhvm_mcode_hmeta_get_pclass__( const bhvm_mcode_hmeta* o ){return 0;} \
  static inline bhvm_mcode_node_s* bhvm_mcode_hmeta_a_get_node( const bhvm_mcode_hmeta* o ) { const bhvm_mcode_hmeta_s* p = bhvm_mcode_hmeta_s_get_aware( o ); assert( p->get_node ); return p->get_node( o ); } \
  static inline bl_t bhvm_mcode_hmeta_a_defines_get_node( const bhvm_mcode_hmeta* o ) { return true; } \
  static inline bhvm_mcode_node_s* bhvm_mcode_hmeta_get_node__( const bhvm_mcode_hmeta* o ){return NULL;} \
  static inline void bhvm_mcode_hmeta_a_set_node( bhvm_mcode_hmeta* o, bhvm_mcode_node_s* node ) { const bhvm_mcode_hmeta_s* p = bhvm_mcode_hmeta_s_get_aware( o ); assert( p->set_node ); p->set_node( o, node ); } \
  static inline bl_t bhvm_mcode_hmeta_a_defines_set_node( const bhvm_mcode_hmeta* o ) { return true; } \
  static inline void bhvm_mcode_hmeta_set_node__( bhvm_mcode_hmeta* o, bhvm_mcode_node_s* node ){} \
  static inline bl_t bhvm_mcode_hmeta_a_is_rollable( const bhvm_mcode_hmeta* o ) { const bhvm_mcode_hmeta_s* p = bhvm_mcode_hmeta_s_get_aware( o ); assert( p->is_rollable ); return p->is_rollable( o ); } \
  static inline bl_t bhvm_mcode_hmeta_a_defines_is_rollable( const bhvm_mcode_hmeta* o ) { return true; } \
  static inline bl_t bhvm_mcode_hmeta_is_rollable__( const bhvm_mcode_hmeta* o ){return false;} \
  static inline bl_t bhvm_mcode_hmeta_a_is_active( const bhvm_mcode_hmeta* o ) { const bhvm_mcode_hmeta_s* p = bhvm_mcode_hmeta_s_get_aware( o ); assert( p->is_active ); return p->is_active( o ); } \
  static inline bl_t bhvm_mcode_hmeta_a_defines_is_active( const bhvm_mcode_hmeta* o ) { return true; } \
  static inline bl_t bhvm_mcode_hmeta_is_active__( const bhvm_mcode_hmeta* o ){return true;} \
  static inline bcore_inst* bhvm_mcode_hmeta_a_get_custom( const bhvm_mcode_hmeta* o ) { const bhvm_mcode_hmeta_s* p = bhvm_mcode_hmeta_s_get_aware( o ); assert( p->get_custom ); return p->get_custom( o ); } \
  static inline bl_t bhvm_mcode_hmeta_a_defines_get_custom( const bhvm_mcode_hmeta* o ) { return true; } \
  static inline bcore_inst* bhvm_mcode_hmeta_get_custom__( const bhvm_mcode_hmeta* o ){return NULL;} \
  static inline bcore_inst* bhvm_mcode_hmeta_a_set_custom( bhvm_mcode_hmeta* o, const bcore_inst* custom ) { const bhvm_mcode_hmeta_s* p = bhvm_mcode_hmeta_s_get_aware( o ); assert( p->set_custom ); return p->set_custom( o, custom ); } \
  static inline bl_t bhvm_mcode_hmeta_a_defines_set_custom( const bhvm_mcode_hmeta* o ) { return true; } \
  static inline bcore_inst* bhvm_mcode_hmeta_set_custom__( bhvm_mcode_hmeta* o, const bcore_inst* custom ){return NULL;} \
  BETH_EXPAND_ITEM_bhvm_mcode_hmeta_adl_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_mcode_hbase

#define TYPEOF_bhvm_mcode_hbase 576283881
#define TYPEOF_bhvm_mcode_hbase_s 3977623907
#define TYPEOF_bhvm_mcode_hbase_s 3977623907
#define BETH_EXPAND_ITEM_bhvm_mcode_hbase_s \
  BCORE_DECLARE_OBJECT( bhvm_mcode_hbase_s ) \
    {aware_t _;bhvm_holor_adl_s holor_adl;bhvm_mcode_hmeta_adl_s hmeta_adl;sz_t copy_size_limit;}; \
  bhvm_mcode_hbase_s* bhvm_mcode_hbase_s_set_size( bhvm_mcode_hbase_s* o, sz_t size ); \
  static inline void bhvm_mcode_hbase_s_mutated( bhvm_mcode_hbase_s* o ){if( o->copy_size_limit >= 0 ) bhvm_mcode_hbase_s_set_size( o, o->copy_size_limit );} \
  static inline void bhvm_mcode_hbase_s_copy_x( bhvm_mcode_hbase_s* o ){if( o->copy_size_limit >= 0 ) bhvm_mcode_hbase_s_set_size( o, o->copy_size_limit );} \
  static inline sz_t bhvm_mcode_hbase_s_get_size( const bhvm_mcode_hbase_s* o ){return o->holor_adl.size;} \
  sz_t bhvm_mcode_hbase_s_push_hm( bhvm_mcode_hbase_s* o, const bhvm_holor_s* h, const bhvm_mcode_hmeta* m ); \
  sz_t bhvm_mcode_hbase_s_push_hmc( bhvm_mcode_hbase_s* o, const bhvm_holor_s* h, const bhvm_mcode_hmeta* m, char c, bhvm_vop_arr_ci_s* arr_ci ); \
  sz_t bhvm_mcode_hbase_s_push_copy_from_index( bhvm_mcode_hbase_s* o, sz_t index ); \
  static inline bhvm_holor_s* bhvm_mcode_hbase_s_get_holor( const bhvm_mcode_hbase_s* o, sz_t index ){assert( index >= 0 && index < o->holor_adl.size ); return o->holor_adl.data[ index ];} \
  static inline bhvm_mcode_hmeta* bhvm_mcode_hbase_s_get_hmeta( const bhvm_mcode_hbase_s* o, sz_t index ){assert( index >= 0 && index < o->hmeta_adl.size ); return o->hmeta_adl.data[ index ];}
#define BETH_EXPAND_GROUP_bhvm_mcode_hbase \
  BCORE_FORWARD_OBJECT( bhvm_mcode_hbase ); \
  BCORE_FORWARD_OBJECT( bhvm_mcode_hbase_s ); \
  BETH_EXPAND_ITEM_bhvm_mcode_hbase_s

/**********************************************************************************************************************/

vd_t bhvm_xoila_out_signal_handler( const bcore_signal_s* o );

#endif // BHVM_XOILA_OUT_H
// BETH_PLANT_SIGNATURE 2901127916