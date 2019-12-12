/** This file was generated from beth-plant source code.
 *  Compiling Agent : bcore_plant_compiler (C) 2019 J.B.Steffens
 *  Last File Update: 2019-12-12T12:53:52Z
 *
 *  Copyright and License of this File:
 *
 *  Generated code inherits the copyright and license of the underlying beth-plant source code.
 *  Source code defining this file is distributed across following files:
 *
 *  bhvm_holor.h
 *  bhvm_hop.h
 *  bhvm_vop.h
 *  bhvm_mcode.h
 *
 */

#ifndef BHVM_PLANTED_H
#define BHVM_PLANTED_H

#include "bcore_control.h"


/**********************************************************************************************************************/
// source: bhvm_holor.h

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm

#define TYPEOF_bhvm 754264922
#define TYPEOF_bhvm_s 672708292
#define TYPEOF_bhvm_shape_s 3092477010
#define BETH_EXPAND_ITEM_bhvm_shape_s \
  BCORE_DECLARE_OBJECT( bhvm_shape_s ) \
    {BCORE_ARRAY_DYN_SOLID_STATIC_S( sz_t, );bl_t htp;}; \
  static inline void bhvm_shape_s_set_space( bhvm_shape_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_bhvm_shape_s, ( bcore_array* )o, size ); } \
  static inline void bhvm_shape_s_set_size( bhvm_shape_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_bhvm_shape_s, ( bcore_array* )o, size ); } \
  static inline void bhvm_shape_s_clear( bhvm_shape_s* o ) { bcore_array_t_set_space( TYPEOF_bhvm_shape_s, ( bcore_array* )o, 0 ); } \
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
  static inline void bhvm_value_s_set_space( bhvm_value_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_bhvm_value_s, ( bcore_array* )o, size ); } \
  static inline void bhvm_value_s_set_size( bhvm_value_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_bhvm_value_s, ( bcore_array* )o, size ); } \
  static inline void bhvm_value_s_clear( bhvm_value_s* o ) { bcore_array_t_set_space( TYPEOF_bhvm_value_s, ( bcore_array* )o, 0 ); }
#define TYPEOF_bhvm_holor_s 2922699917
#define BETH_EXPAND_ITEM_bhvm_holor_s \
  BCORE_DECLARE_OBJECT( bhvm_holor_s ) \
    {aware_t _;bhvm_shape_s s;bhvm_value_s v;};
#define TYPEOF_bhvm_holor_adl_s 329679305
#define BETH_EXPAND_ITEM_bhvm_holor_adl_s \
  BCORE_DECLARE_OBJECT( bhvm_holor_adl_s ) \
    {aware_t _;BCORE_ARRAY_DYN_LINK_STATIC_S( bhvm_holor_s, );}; \
  static inline void bhvm_holor_adl_s_set_space( bhvm_holor_adl_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_bhvm_holor_adl_s, ( bcore_array* )o, size ); } \
  static inline void bhvm_holor_adl_s_set_size( bhvm_holor_adl_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_bhvm_holor_adl_s, ( bcore_array* )o, size ); } \
  static inline void bhvm_holor_adl_s_clear( bhvm_holor_adl_s* o ) { bcore_array_t_set_space( TYPEOF_bhvm_holor_adl_s, ( bcore_array* )o, 0 ); } \
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
  static inline void bhvm_holor_ads_s_set_space( bhvm_holor_ads_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_bhvm_holor_ads_s, ( bcore_array* )o, size ); } \
  static inline void bhvm_holor_ads_s_set_size( bhvm_holor_ads_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_bhvm_holor_ads_s, ( bcore_array* )o, size ); } \
  static inline void bhvm_holor_ads_s_clear( bhvm_holor_ads_s* o ) { bcore_array_t_set_space( TYPEOF_bhvm_holor_ads_s, ( bcore_array* )o, 0 ); } \
  static inline bhvm_holor_s* bhvm_holor_ads_s_push_c( bhvm_holor_ads_s* o, const bhvm_holor_s* v ) { bcore_array_t_push( TYPEOF_bhvm_holor_ads_s, ( bcore_array* )o, sr_twc( TYPEOF_bhvm_holor_s, v ) ); return &o->data[ o->size - 1 ]; } \
  static inline bhvm_holor_s* bhvm_holor_ads_s_push_d( bhvm_holor_ads_s* o,       bhvm_holor_s* v ) { bcore_array_t_push( TYPEOF_bhvm_holor_ads_s, ( bcore_array* )o, sr_tsd( TYPEOF_bhvm_holor_s, v ) ); return &o->data[ o->size - 1 ]; } \
  static inline bhvm_holor_s* bhvm_holor_ads_s_push( bhvm_holor_ads_s* o ) \
  { \
      bcore_array_t_push( TYPEOF_bhvm_holor_ads_s, ( bcore_array* )o, sr_null() ); \
      return &o->data[ o->size - 1 ]; \
  }
#define BETH_EXPAND_GROUP_bhvm \
  BCORE_FORWARD_OBJECT( bhvm ); \
  BCORE_FORWARD_OBJECT( bhvm_shape_s ); \
  BCORE_FORWARD_OBJECT( bhvm_value_s ); \
  BCORE_FORWARD_OBJECT( bhvm_holor_s ); \
  BCORE_FORWARD_OBJECT( bhvm_holor_adl_s ); \
  BCORE_FORWARD_OBJECT( bhvm_holor_ads_s ); \
  BETH_EXPAND_ITEM_bhvm_shape_s \
  BETH_EXPAND_ITEM_bhvm_value_s \
  BETH_EXPAND_ITEM_bhvm_holor_s \
  BETH_EXPAND_ITEM_bhvm_holor_adl_s \
  BETH_EXPAND_ITEM_bhvm_holor_ads_s

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
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2 ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar3 ); \
  BETH_EXPAND_GROUP_bhvm_hop_ar0 \
  BETH_EXPAND_GROUP_bhvm_hop_ar1 \
  BETH_EXPAND_GROUP_bhvm_hop_ar2 \
  BETH_EXPAND_GROUP_bhvm_hop_ar3

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
#define BETH_EXPAND_GROUP_bhvm_hop_ar0 \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar0 ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar0_zro_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar0_one_s ); \
  BETH_EXPAND_ITEM_bhvm_hop_ar0_zro_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar0_one_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hop_ar1

#define TYPEOF_bhvm_hop_ar1 3649900203
#define TYPEOF_bhvm_hop_ar1_s 484699325
#define TYPEOF_bhvm_hop_ar1_identity_s 3061145192
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_identity_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_identity_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar1_identity_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
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
#define TYPEOF_bhvm_hop_ar1_inv_s 3653712297
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_inv_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_inv_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar1_inv_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar1_lgst_s 1020643468
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_lgst_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_lgst_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar1_lgst_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar1_lgst_hard_s 2998674764
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_lgst_hard_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_lgst_hard_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar1_lgst_hard_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar1_lgst_leaky_s 2850435313
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_lgst_leaky_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_lgst_leaky_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar1_lgst_leaky_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
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
#define TYPEOF_bhvm_hop_ar1_add_dp_a_v_s 406985267
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_add_dp_a_v_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_add_dp_a_v_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar1_add_dp_a_v_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar1_add_dp_b_v_s 3319805078
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_add_dp_b_v_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_add_dp_b_v_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar1_add_dp_b_v_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar1_sub_dp_a_v_s 1676882680
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_sub_dp_a_v_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_sub_dp_a_v_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar1_sub_dp_a_v_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar1_sub_dp_b_v_s 4241042881
#define BETH_EXPAND_ITEM_bhvm_hop_ar1_sub_dp_b_v_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar1_sub_dp_b_v_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar1_sub_dp_b_v_s_f( const bhvm_holor_s* a, bhvm_holor_s* r );
#define BETH_EXPAND_GROUP_bhvm_hop_ar1 \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1 ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_identity_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_neg_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_floor_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_ceil_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_exp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_inv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_lgst_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_lgst_hard_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_lgst_leaky_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_tanh_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_tanh_hard_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_tanh_leaky_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_softplus_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_relu_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_relu_leaky_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_add_dp_a_v_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_add_dp_b_v_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_sub_dp_a_v_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar1_sub_dp_b_v_s ); \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_identity_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_neg_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_floor_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_ceil_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_exp_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_inv_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_lgst_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_lgst_hard_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_lgst_leaky_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_tanh_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_tanh_hard_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_tanh_leaky_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_softplus_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_relu_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_relu_leaky_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_add_dp_a_v_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_add_dp_b_v_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_sub_dp_a_v_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar1_sub_dp_b_v_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hop_ar2

#define TYPEOF_bhvm_hop_ar2 3666677822
#define TYPEOF_bhvm_hop_ar2_s 1902904896
#define TYPEOF_bhvm_hop_ar2_add_s 612584456
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_add_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_add_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_add_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_sub_s 2193252305
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_sub_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_sub_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_sub_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_mul_s 1646508549
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_mul_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_mul_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_div_s 4080434168
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_div_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_div_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_div_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_mul_dp_a_vb_s 2737492855
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_dp_a_vb_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_mul_dp_a_vb_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_mul_dp_a_vb_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_mul_dp_b_va_s 2799769029
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_dp_b_va_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_mul_dp_b_va_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_mul_dp_b_va_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_div_dp_a_vb_s 198953766
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_div_dp_a_vb_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_div_dp_a_vb_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_div_dp_a_vb_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_exp_dp_vy_s 1828078079
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_exp_dp_vy_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_exp_dp_vy_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_exp_dp_vy_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_inv_dp_vy_s 847967687
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_inv_dp_vy_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_inv_dp_vy_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_inv_dp_vy_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_lgst_dp_vy_s 1271326820
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_lgst_dp_vy_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_lgst_dp_vy_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_lgst_dp_vy_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_lgst_hard_dp_vy_s 1045644650
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_lgst_hard_dp_vy_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_lgst_hard_dp_vy_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_lgst_hard_dp_vy_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_lgst_leaky_dp_vy_s 3132176773
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_lgst_leaky_dp_vy_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_lgst_leaky_dp_vy_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_lgst_leaky_dp_vy_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_tanh_dp_vy_s 1292398255
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_tanh_dp_vy_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_tanh_dp_vy_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_tanh_dp_vy_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_tanh_hard_dp_vy_s 1107071327
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_tanh_hard_dp_vy_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_tanh_hard_dp_vy_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_tanh_hard_dp_vy_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_tanh_leaky_dp_vy_s 4000723414
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_tanh_leaky_dp_vy_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_tanh_leaky_dp_vy_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_tanh_leaky_dp_vy_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_softplus_dp_vy_s 4161197826
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_softplus_dp_vy_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_softplus_dp_vy_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_softplus_dp_vy_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_relu_dp_vy_s 1115358144
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_relu_dp_vy_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_relu_dp_vy_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_relu_dp_vy_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define TYPEOF_bhvm_hop_ar2_relu_leaky_dp_vy_s 1353156137
#define BETH_EXPAND_ITEM_bhvm_hop_ar2_relu_leaky_dp_vy_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar2_relu_leaky_dp_vy_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar2_relu_leaky_dp_vy_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );
#define BETH_EXPAND_GROUP_bhvm_hop_ar2 \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2 ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_add_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_sub_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_mul_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_div_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_mul_dp_a_vb_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_mul_dp_b_va_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_div_dp_a_vb_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_exp_dp_vy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_inv_dp_vy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_lgst_dp_vy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_lgst_hard_dp_vy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_lgst_leaky_dp_vy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_tanh_dp_vy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_tanh_hard_dp_vy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_tanh_leaky_dp_vy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_softplus_dp_vy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_relu_dp_vy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar2_relu_leaky_dp_vy_s ); \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_add_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_sub_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_div_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_dp_a_vb_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_mul_dp_b_va_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_div_dp_a_vb_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_exp_dp_vy_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_inv_dp_vy_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_lgst_dp_vy_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_lgst_hard_dp_vy_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_lgst_leaky_dp_vy_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_tanh_dp_vy_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_tanh_hard_dp_vy_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_tanh_leaky_dp_vy_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_softplus_dp_vy_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_relu_dp_vy_s \
  BETH_EXPAND_ITEM_bhvm_hop_ar2_relu_leaky_dp_vy_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hop_ar3

#define TYPEOF_bhvm_hop_ar3 3683455441
#define TYPEOF_bhvm_hop_ar3_s 3981872635
#define TYPEOF_bhvm_hop_ar3_div_dp_b_vab_s 1266979243
#define BETH_EXPAND_ITEM_bhvm_hop_ar3_div_dp_b_vab_s \
  BCORE_DECLARE_OBJECT( bhvm_hop_ar3_div_dp_b_vab_s ) \
    {aware_t _;}; \
  void bhvm_hop_ar3_div_dp_b_vab_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, const bhvm_holor_s* c, bhvm_holor_s* r );
#define BETH_EXPAND_GROUP_bhvm_hop_ar3 \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar3 ); \
  BCORE_FORWARD_OBJECT( bhvm_hop_ar3_div_dp_b_vab_s ); \
  BETH_EXPAND_ITEM_bhvm_hop_ar3_div_dp_b_vab_s

/**********************************************************************************************************************/
// source: bhvm_vop.h

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_vop

#define TYPEOF_bhvm_vop 3907802388
#define TYPEOF_bhvm_vop_s 1194015046
#define TYPEOF_bhvm_vop_ci_s 711211199
#define BETH_EXPAND_ITEM_bhvm_vop_ci_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ci_s ) \
    {u0_t c;sz_t i;};
#define TYPEOF_bhvm_vop_arr_ci_s 791850599
#define BETH_EXPAND_ITEM_bhvm_vop_arr_ci_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_arr_ci_s ) \
    {aware_t _;BCORE_ARRAY_DYN_SOLID_STATIC_S( bhvm_vop_ci_s, );}; \
  static inline void bhvm_vop_arr_ci_s_set_space( bhvm_vop_arr_ci_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_bhvm_vop_arr_ci_s, ( bcore_array* )o, size ); } \
  static inline void bhvm_vop_arr_ci_s_set_size( bhvm_vop_arr_ci_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_bhvm_vop_arr_ci_s, ( bcore_array* )o, size ); } \
  static inline void bhvm_vop_arr_ci_s_clear( bhvm_vop_arr_ci_s* o ) { bcore_array_t_set_space( TYPEOF_bhvm_vop_arr_ci_s, ( bcore_array* )o, 0 ); } \
  static inline bhvm_vop_ci_s* bhvm_vop_arr_ci_s_push_c( bhvm_vop_arr_ci_s* o, const bhvm_vop_ci_s* v ) { bcore_array_t_push( TYPEOF_bhvm_vop_arr_ci_s, ( bcore_array* )o, sr_twc( TYPEOF_bhvm_vop_ci_s, v ) ); return &o->data[ o->size - 1 ]; } \
  static inline bhvm_vop_ci_s* bhvm_vop_arr_ci_s_push_d( bhvm_vop_arr_ci_s* o,       bhvm_vop_ci_s* v ) { bcore_array_t_push( TYPEOF_bhvm_vop_arr_ci_s, ( bcore_array* )o, sr_tsd( TYPEOF_bhvm_vop_ci_s, v ) ); return &o->data[ o->size - 1 ]; } \
  static inline bhvm_vop_ci_s* bhvm_vop_arr_ci_s_push( bhvm_vop_arr_ci_s* o ) \
  { \
      bcore_array_t_push( TYPEOF_bhvm_vop_arr_ci_s, ( bcore_array* )o, sr_null() ); \
      return &o->data[ o->size - 1 ]; \
  }
#define BETH_EXPAND_GROUP_bhvm_vop \
  BCORE_FORWARD_OBJECT( bhvm_vop ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ci_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_arr_ci_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar0 ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1 ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2 ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar3 ); \
  typedef void (*bhvm_vop_run)( const bhvm_vop* o, bhvm_holor_s* ah ); \
  typedef sz_t (*bhvm_vop_arity)( const bhvm_vop* o ); \
  typedef sc_t (*bhvm_vop_sig)( const bhvm_vop* o ); \
  typedef sz_t* (*bhvm_vop_get_index)( bhvm_vop* o ); \
  typedef void (*bhvm_vop_set_arg)( bhvm_vop* o, bhvm_vop_ci_s* ci ); \
  typedef void (*bhvm_vop_set_args)( bhvm_vop* o, const bhvm_vop_arr_ci_s* arr ); \
  BCORE_DECLARE_SPECT( bhvm_vop ) \
  { \
      bcore_spect_header_s header; \
      bhvm_vop_run run; \
      bhvm_vop_arity arity; \
      bhvm_vop_sig sig; \
      bhvm_vop_get_index get_index; \
      bhvm_vop_set_arg set_arg; \
      bhvm_vop_set_args set_args; \
  }; \
  static inline bhvm_vop* bhvm_vop_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_bhvm_vop, t ); return ( bhvm_vop* )bcore_inst_t_create( t ); } \
  static inline bl_t bhvm_vop_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_bhvm_vop ); } \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bhvm_vop ) \
  static inline bl_t bhvm_vop_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_bhvm_vop ); } \
  static inline void bhvm_vop_p_run( const bhvm_vop_s* __p, const bhvm_vop* o, bhvm_holor_s* ah ) { assert( __p->run ); __p->run( o, ah ); } \
  static inline void bhvm_vop_a_run( const bhvm_vop* o, bhvm_holor_s* ah ) { const bhvm_vop_s* p = bhvm_vop_s_get_aware( o ); assert( p->run ); p->run( o, ah ); } \
  static inline bl_t bhvm_vop_p_defines_run( const bhvm_vop_s* __p ) { return __p->run != NULL; } \
  static inline bl_t bhvm_vop_a_defines_run( const bhvm_vop* o ) { return bhvm_vop_s_get_aware( o )->run != NULL; } \
  static inline sz_t bhvm_vop_a_arity( const bhvm_vop* o ) { const bhvm_vop_s* p = bhvm_vop_s_get_aware( o ); assert( p->arity ); return p->arity( o ); } \
  static inline bl_t bhvm_vop_a_defines_arity( const bhvm_vop* o ) { return bhvm_vop_s_get_aware( o )->arity != NULL; } \
  static inline sc_t bhvm_vop_a_sig( const bhvm_vop* o ) { const bhvm_vop_s* p = bhvm_vop_s_get_aware( o ); assert( p->sig ); return p->sig( o ); } \
  static inline bl_t bhvm_vop_a_defines_sig( const bhvm_vop* o ) { return bhvm_vop_s_get_aware( o )->sig != NULL; } \
  static inline sz_t* bhvm_vop_a_get_index( bhvm_vop* o ) { const bhvm_vop_s* p = bhvm_vop_s_get_aware( o ); assert( p->get_index ); return p->get_index( o ); } \
  static inline bl_t bhvm_vop_a_defines_get_index( const bhvm_vop* o ) { return bhvm_vop_s_get_aware( o )->get_index != NULL; } \
  BETH_EXPAND_ITEM_bhvm_vop_ci_s \
  BETH_EXPAND_ITEM_bhvm_vop_arr_ci_s \
  static inline void bhvm_vop_a_set_arg( bhvm_vop* o, bhvm_vop_ci_s* ci ) { const bhvm_vop_s* p = bhvm_vop_s_get_aware( o ); assert( p->set_arg ); p->set_arg( o, ci ); } \
  static inline bl_t bhvm_vop_a_defines_set_arg( const bhvm_vop* o ) { return true; } \
  void bhvm_vop_set_arg__( bhvm_vop* o, bhvm_vop_ci_s* ci ); \
  static inline void bhvm_vop_a_set_args( bhvm_vop* o, const bhvm_vop_arr_ci_s* arr ) { const bhvm_vop_s* p = bhvm_vop_s_get_aware( o ); assert( p->set_args ); p->set_args( o, arr ); } \
  static inline bl_t bhvm_vop_a_defines_set_args( const bhvm_vop* o ) { return true; } \
  void bhvm_vop_set_args__( bhvm_vop* o, const bhvm_vop_arr_ci_s* arr ); \
  BETH_EXPAND_GROUP_bhvm_vop_ar0 \
  BETH_EXPAND_GROUP_bhvm_vop_ar1 \
  BETH_EXPAND_GROUP_bhvm_vop_ar2 \
  BETH_EXPAND_GROUP_bhvm_vop_ar3

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_vop_ar0

#define TYPEOF_bhvm_vop_ar0 1824388226
#define TYPEOF_bhvm_vop_ar0_s 2236890620
#define TYPEOF_bhvm_vop_ar0_index_s 4130666847
#define BETH_EXPAND_ITEM_bhvm_vop_ar0_index_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar0_index_s ) \
    {sz_t v[ 1 ];};
#define TYPEOF_bhvm_vop_ar0_zro_s 2672924538
#define BETH_EXPAND_ITEM_bhvm_vop_ar0_zro_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar0_zro_s ) \
    {aware_t _;bhvm_vop_ar0_index_s i;}; \
  static inline sz_t bhvm_vop_ar0_zro_s_arity( const bhvm_vop_ar0_zro_s* o ){return 0;} \
  static inline sz_t* bhvm_vop_ar0_zro_s_get_index( bhvm_vop_ar0_zro_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar0_zro_s_sig( const bhvm_vop_ar0_zro_s* o ){return "y";} \
  static inline void bhvm_vop_ar0_zro_s_run( const bhvm_vop_ar0_zro_s* o, bhvm_holor_s* ah ){bhvm_hop_ar0_zro_s_f( &ah[o->i.v[0]] );}
#define TYPEOF_bhvm_vop_ar0_one_s 131160455
#define BETH_EXPAND_ITEM_bhvm_vop_ar0_one_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar0_one_s ) \
    {aware_t _;bhvm_vop_ar0_index_s i;}; \
  static inline sz_t bhvm_vop_ar0_one_s_arity( const bhvm_vop_ar0_one_s* o ){return 0;} \
  static inline sz_t* bhvm_vop_ar0_one_s_get_index( bhvm_vop_ar0_one_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar0_one_s_sig( const bhvm_vop_ar0_one_s* o ){return "y";} \
  static inline void bhvm_vop_ar0_one_s_run( const bhvm_vop_ar0_one_s* o, bhvm_holor_s* ah ){bhvm_hop_ar0_one_s_f( &ah[o->i.v[0]] );}
#define BETH_EXPAND_GROUP_bhvm_vop_ar0 \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar0 ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar0_index_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar0_zro_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar0_one_s ); \
  BETH_EXPAND_ITEM_bhvm_vop_ar0_index_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar0_zro_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar0_one_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_vop_ar1

#define TYPEOF_bhvm_vop_ar1 1841165845
#define TYPEOF_bhvm_vop_ar1_s 31233655
#define TYPEOF_bhvm_vop_ar1_index_s 3144050460
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_index_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_index_s ) \
    {sz_t v[ 2 ];};
#define TYPEOF_bhvm_vop_ar1_identity_s 3388163422
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_identity_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_identity_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_identity_s_arity( const bhvm_vop_ar1_identity_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_identity_s_get_index( bhvm_vop_ar1_identity_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_identity_s_sig( const bhvm_vop_ar1_identity_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_identity_s_run( const bhvm_vop_ar1_identity_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_identity_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_neg_s 4102927266
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_neg_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_neg_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_neg_s_arity( const bhvm_vop_ar1_neg_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_neg_s_get_index( bhvm_vop_ar1_neg_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_neg_s_sig( const bhvm_vop_ar1_neg_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_neg_s_run( const bhvm_vop_ar1_neg_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_neg_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_floor_s 294636938
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_floor_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_floor_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_floor_s_arity( const bhvm_vop_ar1_floor_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_floor_s_get_index( bhvm_vop_ar1_floor_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_floor_s_sig( const bhvm_vop_ar1_floor_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_floor_s_run( const bhvm_vop_ar1_floor_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_floor_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_ceil_s 2964243205
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_ceil_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_ceil_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_ceil_s_arity( const bhvm_vop_ar1_ceil_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_ceil_s_get_index( bhvm_vop_ar1_ceil_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_ceil_s_sig( const bhvm_vop_ar1_ceil_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_ceil_s_run( const bhvm_vop_ar1_ceil_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_ceil_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_exp_s 3731945467
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_exp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_exp_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_exp_s_arity( const bhvm_vop_ar1_exp_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_exp_s_get_index( bhvm_vop_ar1_exp_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_exp_s_sig( const bhvm_vop_ar1_exp_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_exp_s_run( const bhvm_vop_ar1_exp_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_exp_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_inv_s 381359607
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_inv_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_inv_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_inv_s_arity( const bhvm_vop_ar1_inv_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_inv_s_get_index( bhvm_vop_ar1_inv_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_inv_s_sig( const bhvm_vop_ar1_inv_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_inv_s_run( const bhvm_vop_ar1_inv_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_inv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_lgst_s 3402156782
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_lgst_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_lgst_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_lgst_s_arity( const bhvm_vop_ar1_lgst_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_lgst_s_get_index( bhvm_vop_ar1_lgst_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_lgst_s_sig( const bhvm_vop_ar1_lgst_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_lgst_s_run( const bhvm_vop_ar1_lgst_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_lgst_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_lgst_hard_s 1312586658
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_lgst_hard_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_lgst_hard_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_lgst_hard_s_arity( const bhvm_vop_ar1_lgst_hard_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_lgst_hard_s_get_index( bhvm_vop_ar1_lgst_hard_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_lgst_hard_s_sig( const bhvm_vop_ar1_lgst_hard_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_lgst_hard_s_run( const bhvm_vop_ar1_lgst_hard_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_lgst_hard_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_lgst_leaky_s 3970548331
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_lgst_leaky_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_lgst_leaky_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_lgst_leaky_s_arity( const bhvm_vop_ar1_lgst_leaky_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_lgst_leaky_s_get_index( bhvm_vop_ar1_lgst_leaky_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_lgst_leaky_s_sig( const bhvm_vop_ar1_lgst_leaky_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_lgst_leaky_s_run( const bhvm_vop_ar1_lgst_leaky_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_lgst_leaky_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_tanh_s 3583954501
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_tanh_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_tanh_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_tanh_s_arity( const bhvm_vop_ar1_tanh_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_tanh_s_get_index( bhvm_vop_ar1_tanh_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_tanh_s_sig( const bhvm_vop_ar1_tanh_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_tanh_s_run( const bhvm_vop_ar1_tanh_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_tanh_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_tanh_hard_s 71062515
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_tanh_hard_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_tanh_hard_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_tanh_hard_s_arity( const bhvm_vop_ar1_tanh_hard_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_tanh_hard_s_get_index( bhvm_vop_ar1_tanh_hard_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_tanh_hard_s_sig( const bhvm_vop_ar1_tanh_hard_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_tanh_hard_s_run( const bhvm_vop_ar1_tanh_hard_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_tanh_hard_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_tanh_leaky_s 567258088
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_tanh_leaky_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_tanh_leaky_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_tanh_leaky_s_arity( const bhvm_vop_ar1_tanh_leaky_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_tanh_leaky_s_get_index( bhvm_vop_ar1_tanh_leaky_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_tanh_leaky_s_sig( const bhvm_vop_ar1_tanh_leaky_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_tanh_leaky_s_run( const bhvm_vop_ar1_tanh_leaky_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_tanh_leaky_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_softplus_s 3784219040
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_softplus_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_softplus_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_softplus_s_arity( const bhvm_vop_ar1_softplus_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_softplus_s_get_index( bhvm_vop_ar1_softplus_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_softplus_s_sig( const bhvm_vop_ar1_softplus_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_softplus_s_run( const bhvm_vop_ar1_softplus_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_softplus_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_relu_s 3052643634
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_relu_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_relu_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_relu_s_arity( const bhvm_vop_ar1_relu_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_relu_s_get_index( bhvm_vop_ar1_relu_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_relu_s_sig( const bhvm_vop_ar1_relu_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_relu_s_run( const bhvm_vop_ar1_relu_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_relu_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_relu_leaky_s 1291048615
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_relu_leaky_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_relu_leaky_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_relu_leaky_s_arity( const bhvm_vop_ar1_relu_leaky_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_relu_leaky_s_get_index( bhvm_vop_ar1_relu_leaky_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_relu_leaky_s_sig( const bhvm_vop_ar1_relu_leaky_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_relu_leaky_s_run( const bhvm_vop_ar1_relu_leaky_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_relu_leaky_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_add_dp_a_v_s 2202221141
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_add_dp_a_v_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_add_dp_a_v_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_add_dp_a_v_s_arity( const bhvm_vop_ar1_add_dp_a_v_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_add_dp_a_v_s_get_index( bhvm_vop_ar1_add_dp_a_v_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_add_dp_a_v_s_sig( const bhvm_vop_ar1_add_dp_a_v_s* o ){return "vu";} \
  static inline void bhvm_vop_ar1_add_dp_a_v_s_run( const bhvm_vop_ar1_add_dp_a_v_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_add_dp_a_v_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_add_dp_b_v_s 3132997756
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_add_dp_b_v_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_add_dp_b_v_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_add_dp_b_v_s_arity( const bhvm_vop_ar1_add_dp_b_v_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_add_dp_b_v_s_get_index( bhvm_vop_ar1_add_dp_b_v_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_add_dp_b_v_s_sig( const bhvm_vop_ar1_add_dp_b_v_s* o ){return "vu";} \
  static inline void bhvm_vop_ar1_add_dp_b_v_s_run( const bhvm_vop_ar1_add_dp_b_v_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_add_dp_b_v_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_sub_dp_a_v_s 487190986
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_sub_dp_a_v_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_sub_dp_a_v_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_sub_dp_a_v_s_arity( const bhvm_vop_ar1_sub_dp_a_v_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_sub_dp_a_v_s_get_index( bhvm_vop_ar1_sub_dp_a_v_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_sub_dp_a_v_s_sig( const bhvm_vop_ar1_sub_dp_a_v_s* o ){return "vu";} \
  static inline void bhvm_vop_ar1_sub_dp_a_v_s_run( const bhvm_vop_ar1_sub_dp_a_v_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_sub_dp_a_v_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_sub_dp_b_v_s 273685207
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_sub_dp_b_v_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_sub_dp_b_v_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_sub_dp_b_v_s_arity( const bhvm_vop_ar1_sub_dp_b_v_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_sub_dp_b_v_s_get_index( bhvm_vop_ar1_sub_dp_b_v_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_sub_dp_b_v_s_sig( const bhvm_vop_ar1_sub_dp_b_v_s* o ){return "vu";} \
  static inline void bhvm_vop_ar1_sub_dp_b_v_s_run( const bhvm_vop_ar1_sub_dp_b_v_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_sub_dp_b_v_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define BETH_EXPAND_GROUP_bhvm_vop_ar1 \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1 ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_index_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_identity_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_neg_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_floor_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_ceil_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_exp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_inv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_lgst_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_lgst_hard_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_lgst_leaky_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_tanh_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_tanh_hard_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_tanh_leaky_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_softplus_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_relu_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_relu_leaky_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_add_dp_a_v_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_add_dp_b_v_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_sub_dp_a_v_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_sub_dp_b_v_s ); \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_index_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_identity_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_neg_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_floor_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_ceil_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_exp_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_inv_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_lgst_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_lgst_hard_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_lgst_leaky_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_tanh_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_tanh_hard_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_tanh_leaky_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_softplus_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_relu_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_relu_leaky_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_add_dp_a_v_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_add_dp_b_v_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_sub_dp_a_v_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_sub_dp_b_v_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_vop_ar2

#define TYPEOF_bhvm_vop_ar2 1790832988
#define TYPEOF_bhvm_vop_ar2_s 2982765310
#define TYPEOF_bhvm_vop_ar2_index_s 180378457
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_index_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_index_s ) \
    {sz_t v[ 3 ];};
#define TYPEOF_bhvm_vop_ar2_add_s 1453720506
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_add_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_add_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_add_s_arity( const bhvm_vop_ar2_add_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_add_s_get_index( bhvm_vop_ar2_add_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_add_s_sig( const bhvm_vop_ar2_add_s* o ){return "aby";} \
  static inline void bhvm_vop_ar2_add_s_run( const bhvm_vop_ar2_add_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_add_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_sub_s 4210861187
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_sub_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_sub_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_sub_s_arity( const bhvm_vop_ar2_sub_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_sub_s_get_index( bhvm_vop_ar2_sub_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_sub_s_sig( const bhvm_vop_ar2_sub_s* o ){return "aby";} \
  static inline void bhvm_vop_ar2_sub_s_run( const bhvm_vop_ar2_sub_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_sub_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_mul_s 3521182695
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_s_arity( const bhvm_vop_ar2_mul_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_s_get_index( bhvm_vop_ar2_mul_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_mul_s_sig( const bhvm_vop_ar2_mul_s* o ){return "aby";} \
  static inline void bhvm_vop_ar2_mul_s_run( const bhvm_vop_ar2_mul_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_div_s 3736072358
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_div_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_div_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_div_s_arity( const bhvm_vop_ar2_div_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_div_s_get_index( bhvm_vop_ar2_div_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_div_s_sig( const bhvm_vop_ar2_div_s* o ){return "aby";} \
  static inline void bhvm_vop_ar2_div_s_run( const bhvm_vop_ar2_div_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_div_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_mul_dp_a_vb_s 1127337549
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_dp_a_vb_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_dp_a_vb_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_dp_a_vb_s_arity( const bhvm_vop_ar2_mul_dp_a_vb_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_dp_a_vb_s_get_index( bhvm_vop_ar2_mul_dp_a_vb_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_mul_dp_a_vb_s_sig( const bhvm_vop_ar2_mul_dp_a_vb_s* o ){return "vbu";} \
  static inline void bhvm_vop_ar2_mul_dp_a_vb_s_run( const bhvm_vop_ar2_mul_dp_a_vb_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_dp_a_vb_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_mul_dp_b_va_s 2720778043
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_dp_b_va_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_dp_b_va_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_dp_b_va_s_arity( const bhvm_vop_ar2_mul_dp_b_va_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_dp_b_va_s_get_index( bhvm_vop_ar2_mul_dp_b_va_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_mul_dp_b_va_s_sig( const bhvm_vop_ar2_mul_dp_b_va_s* o ){return "vau";} \
  static inline void bhvm_vop_ar2_mul_dp_b_va_s_run( const bhvm_vop_ar2_mul_dp_b_va_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_dp_b_va_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_div_dp_a_vb_s 3126568112
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_div_dp_a_vb_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_div_dp_a_vb_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_div_dp_a_vb_s_arity( const bhvm_vop_ar2_div_dp_a_vb_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_div_dp_a_vb_s_get_index( bhvm_vop_ar2_div_dp_a_vb_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_div_dp_a_vb_s_sig( const bhvm_vop_ar2_div_dp_a_vb_s* o ){return "vbu";} \
  static inline void bhvm_vop_ar2_div_dp_a_vb_s_run( const bhvm_vop_ar2_div_dp_a_vb_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_div_dp_a_vb_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_exp_dp_vy_s 2630218229
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_exp_dp_vy_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_exp_dp_vy_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_exp_dp_vy_s_arity( const bhvm_vop_ar2_exp_dp_vy_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_exp_dp_vy_s_get_index( bhvm_vop_ar2_exp_dp_vy_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_exp_dp_vy_s_sig( const bhvm_vop_ar2_exp_dp_vy_s* o ){return "vyu";} \
  static inline void bhvm_vop_ar2_exp_dp_vy_s_run( const bhvm_vop_ar2_exp_dp_vy_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_exp_dp_vy_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_inv_dp_vy_s 268035869
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_inv_dp_vy_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_inv_dp_vy_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_inv_dp_vy_s_arity( const bhvm_vop_ar2_inv_dp_vy_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_inv_dp_vy_s_get_index( bhvm_vop_ar2_inv_dp_vy_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_inv_dp_vy_s_sig( const bhvm_vop_ar2_inv_dp_vy_s* o ){return "vyu";} \
  static inline void bhvm_vop_ar2_inv_dp_vy_s_run( const bhvm_vop_ar2_inv_dp_vy_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_inv_dp_vy_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_lgst_dp_vy_s 3786083238
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_lgst_dp_vy_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_lgst_dp_vy_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_lgst_dp_vy_s_arity( const bhvm_vop_ar2_lgst_dp_vy_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_lgst_dp_vy_s_get_index( bhvm_vop_ar2_lgst_dp_vy_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_lgst_dp_vy_s_sig( const bhvm_vop_ar2_lgst_dp_vy_s* o ){return "vyu";} \
  static inline void bhvm_vop_ar2_lgst_dp_vy_s_run( const bhvm_vop_ar2_lgst_dp_vy_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_lgst_dp_vy_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_lgst_hard_dp_vy_s 3014700400
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_lgst_hard_dp_vy_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_lgst_hard_dp_vy_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_lgst_hard_dp_vy_s_arity( const bhvm_vop_ar2_lgst_hard_dp_vy_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_lgst_hard_dp_vy_s_get_index( bhvm_vop_ar2_lgst_hard_dp_vy_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_lgst_hard_dp_vy_s_sig( const bhvm_vop_ar2_lgst_hard_dp_vy_s* o ){return "vyu";} \
  static inline void bhvm_vop_ar2_lgst_hard_dp_vy_s_run( const bhvm_vop_ar2_lgst_hard_dp_vy_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_lgst_hard_dp_vy_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_lgst_leaky_dp_vy_s 770019967
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_lgst_leaky_dp_vy_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_lgst_leaky_dp_vy_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_lgst_leaky_dp_vy_s_arity( const bhvm_vop_ar2_lgst_leaky_dp_vy_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_lgst_leaky_dp_vy_s_get_index( bhvm_vop_ar2_lgst_leaky_dp_vy_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_lgst_leaky_dp_vy_s_sig( const bhvm_vop_ar2_lgst_leaky_dp_vy_s* o ){return "vyu";} \
  static inline void bhvm_vop_ar2_lgst_leaky_dp_vy_s_run( const bhvm_vop_ar2_lgst_leaky_dp_vy_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_lgst_leaky_dp_vy_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_tanh_dp_vy_s 735291229
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_tanh_dp_vy_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_tanh_dp_vy_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_tanh_dp_vy_s_arity( const bhvm_vop_ar2_tanh_dp_vy_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_tanh_dp_vy_s_get_index( bhvm_vop_ar2_tanh_dp_vy_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_tanh_dp_vy_s_sig( const bhvm_vop_ar2_tanh_dp_vy_s* o ){return "vyu";} \
  static inline void bhvm_vop_ar2_tanh_dp_vy_s_run( const bhvm_vop_ar2_tanh_dp_vy_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_tanh_dp_vy_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_tanh_hard_dp_vy_s 1875744993
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_tanh_hard_dp_vy_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_tanh_hard_dp_vy_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_tanh_hard_dp_vy_s_arity( const bhvm_vop_ar2_tanh_hard_dp_vy_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_tanh_hard_dp_vy_s_get_index( bhvm_vop_ar2_tanh_hard_dp_vy_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_tanh_hard_dp_vy_s_sig( const bhvm_vop_ar2_tanh_hard_dp_vy_s* o ){return "vyu";} \
  static inline void bhvm_vop_ar2_tanh_hard_dp_vy_s_run( const bhvm_vop_ar2_tanh_hard_dp_vy_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_tanh_hard_dp_vy_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_tanh_leaky_dp_vy_s 1447806692
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_tanh_leaky_dp_vy_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_tanh_leaky_dp_vy_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_tanh_leaky_dp_vy_s_arity( const bhvm_vop_ar2_tanh_leaky_dp_vy_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_tanh_leaky_dp_vy_s_get_index( bhvm_vop_ar2_tanh_leaky_dp_vy_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_tanh_leaky_dp_vy_s_sig( const bhvm_vop_ar2_tanh_leaky_dp_vy_s* o ){return "vyu";} \
  static inline void bhvm_vop_ar2_tanh_leaky_dp_vy_s_run( const bhvm_vop_ar2_tanh_leaky_dp_vy_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_tanh_leaky_dp_vy_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_softplus_dp_vy_s 3675332172
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_softplus_dp_vy_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_softplus_dp_vy_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_softplus_dp_vy_s_arity( const bhvm_vop_ar2_softplus_dp_vy_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_softplus_dp_vy_s_get_index( bhvm_vop_ar2_softplus_dp_vy_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_softplus_dp_vy_s_sig( const bhvm_vop_ar2_softplus_dp_vy_s* o ){return "vyu";} \
  static inline void bhvm_vop_ar2_softplus_dp_vy_s_run( const bhvm_vop_ar2_softplus_dp_vy_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_softplus_dp_vy_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_relu_dp_vy_s 3643823654
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_relu_dp_vy_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_relu_dp_vy_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_relu_dp_vy_s_arity( const bhvm_vop_ar2_relu_dp_vy_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_relu_dp_vy_s_get_index( bhvm_vop_ar2_relu_dp_vy_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_relu_dp_vy_s_sig( const bhvm_vop_ar2_relu_dp_vy_s* o ){return "vyu";} \
  static inline void bhvm_vop_ar2_relu_dp_vy_s_run( const bhvm_vop_ar2_relu_dp_vy_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_relu_dp_vy_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_relu_leaky_dp_vy_s 1173129471
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_relu_leaky_dp_vy_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_relu_leaky_dp_vy_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_relu_leaky_dp_vy_s_arity( const bhvm_vop_ar2_relu_leaky_dp_vy_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_relu_leaky_dp_vy_s_get_index( bhvm_vop_ar2_relu_leaky_dp_vy_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_relu_leaky_dp_vy_s_sig( const bhvm_vop_ar2_relu_leaky_dp_vy_s* o ){return "vyu";} \
  static inline void bhvm_vop_ar2_relu_leaky_dp_vy_s_run( const bhvm_vop_ar2_relu_leaky_dp_vy_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_relu_leaky_dp_vy_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define BETH_EXPAND_GROUP_bhvm_vop_ar2 \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2 ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_index_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_add_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_sub_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_div_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_dp_a_vb_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_dp_b_va_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_div_dp_a_vb_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_exp_dp_vy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_inv_dp_vy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_lgst_dp_vy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_lgst_hard_dp_vy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_lgst_leaky_dp_vy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_tanh_dp_vy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_tanh_hard_dp_vy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_tanh_leaky_dp_vy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_softplus_dp_vy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_relu_dp_vy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_relu_leaky_dp_vy_s ); \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_index_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_add_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_sub_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_div_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_dp_a_vb_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_dp_b_va_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_div_dp_a_vb_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_exp_dp_vy_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_inv_dp_vy_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_lgst_dp_vy_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_lgst_hard_dp_vy_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_lgst_leaky_dp_vy_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_tanh_dp_vy_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_tanh_hard_dp_vy_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_tanh_leaky_dp_vy_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_softplus_dp_vy_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_relu_dp_vy_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_relu_leaky_dp_vy_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_vop_ar3

#define TYPEOF_bhvm_vop_ar3 1807610607
#define TYPEOF_bhvm_vop_ar3_s 250670297
#define TYPEOF_bhvm_vop_ar3_index_s 878118510
#define BETH_EXPAND_ITEM_bhvm_vop_ar3_index_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar3_index_s ) \
    {sz_t v[ 4 ];};
#define TYPEOF_bhvm_vop_ar3_div_dp_b_vab_s 3429994365
#define BETH_EXPAND_ITEM_bhvm_vop_ar3_div_dp_b_vab_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar3_div_dp_b_vab_s ) \
    {aware_t _;bhvm_vop_ar3_index_s i;}; \
  static inline sz_t bhvm_vop_ar3_div_dp_b_vab_s_arity( const bhvm_vop_ar3_div_dp_b_vab_s* o ){return 3;} \
  static inline sz_t* bhvm_vop_ar3_div_dp_b_vab_s_get_index( bhvm_vop_ar3_div_dp_b_vab_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar3_div_dp_b_vab_s_sig( const bhvm_vop_ar3_div_dp_b_vab_s* o ){return "vabu";} \
  static inline void bhvm_vop_ar3_div_dp_b_vab_s_run( const bhvm_vop_ar3_div_dp_b_vab_s* o, bhvm_holor_s* ah ){bhvm_hop_ar3_div_dp_b_vab_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]], &ah[o->i.v[3]] );}
#define BETH_EXPAND_GROUP_bhvm_vop_ar3 \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar3 ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar3_index_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar3_div_dp_b_vab_s ); \
  BETH_EXPAND_ITEM_bhvm_vop_ar3_index_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar3_div_dp_b_vab_s

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
  static inline void bhvm_mcode_op_s_run( const bhvm_mcode_op_s* o, bhvm_holor_s* ah ){assert( o->p ); assert( o->p->run ); o->p->run( (vc_t)o->vop, ah );}
#define TYPEOF_bhvm_mcode_track_s 946103125
#define BETH_EXPAND_ITEM_bhvm_mcode_track_s \
  BCORE_DECLARE_OBJECT( bhvm_mcode_track_s ) \
    {aware_t _;tp_t name;BCORE_ARRAY_DYN_SOLID_STATIC_S( bhvm_mcode_op_s, );}; \
  static inline void bhvm_mcode_track_s_run( const bhvm_mcode_track_s* o, bhvm_holor_s* ah ){BFOR_EACH( i, o ) bhvm_mcode_op_s_run( &o->data[ i ], ah );} \
  void bhvm_mcode_track_s_vop_push_d( bhvm_mcode_track_s* o, bhvm_vop* vop ); \
  void bhvm_mcode_track_s_vop_push_c( bhvm_mcode_track_s* o, const bhvm_vop* vop ); \
  static inline void bhvm_mcode_track_s_set_space( bhvm_mcode_track_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_bhvm_mcode_track_s, ( bcore_array* )o, size ); } \
  static inline void bhvm_mcode_track_s_set_size( bhvm_mcode_track_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_bhvm_mcode_track_s, ( bcore_array* )o, size ); } \
  static inline void bhvm_mcode_track_s_clear( bhvm_mcode_track_s* o ) { bcore_array_t_set_space( TYPEOF_bhvm_mcode_track_s, ( bcore_array* )o, 0 ); } \
  static inline bhvm_mcode_op_s* bhvm_mcode_track_s_push_c( bhvm_mcode_track_s* o, const bhvm_mcode_op_s* v ) { bcore_array_t_push( TYPEOF_bhvm_mcode_track_s, ( bcore_array* )o, sr_twc( TYPEOF_bhvm_mcode_op_s, v ) ); return &o->data[ o->size - 1 ]; } \
  static inline bhvm_mcode_op_s* bhvm_mcode_track_s_push_d( bhvm_mcode_track_s* o,       bhvm_mcode_op_s* v ) { bcore_array_t_push( TYPEOF_bhvm_mcode_track_s, ( bcore_array* )o, sr_tsd( TYPEOF_bhvm_mcode_op_s, v ) ); return &o->data[ o->size - 1 ]; } \
  static inline bhvm_mcode_op_s* bhvm_mcode_track_s_push( bhvm_mcode_track_s* o ) \
  { \
      bcore_array_t_push( TYPEOF_bhvm_mcode_track_s, ( bcore_array* )o, sr_null() ); \
      return &o->data[ o->size - 1 ]; \
  }
#define TYPEOF_bhvm_mcode_track_ads_s 2395198312
#define BETH_EXPAND_ITEM_bhvm_mcode_track_ads_s \
  BCORE_DECLARE_OBJECT( bhvm_mcode_track_ads_s ) \
    {aware_t _;BCORE_ARRAY_DYN_LINK_STATIC_S( bhvm_mcode_track_s, );}; \
  static inline void bhvm_mcode_track_ads_s_set_space( bhvm_mcode_track_ads_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_bhvm_mcode_track_ads_s, ( bcore_array* )o, size ); } \
  static inline void bhvm_mcode_track_ads_s_set_size( bhvm_mcode_track_ads_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_bhvm_mcode_track_ads_s, ( bcore_array* )o, size ); } \
  static inline void bhvm_mcode_track_ads_s_clear( bhvm_mcode_track_ads_s* o ) { bcore_array_t_set_space( TYPEOF_bhvm_mcode_track_ads_s, ( bcore_array* )o, 0 ); } \
  static inline bhvm_mcode_track_s* bhvm_mcode_track_ads_s_push_c( bhvm_mcode_track_ads_s* o, const bhvm_mcode_track_s* v ) { bcore_array_t_push( TYPEOF_bhvm_mcode_track_ads_s, ( bcore_array* )o, sr_twc( TYPEOF_bhvm_mcode_track_s, v ) ); return o->data[ o->size - 1 ]; } \
  static inline bhvm_mcode_track_s* bhvm_mcode_track_ads_s_push_d( bhvm_mcode_track_ads_s* o,       bhvm_mcode_track_s* v ) { bcore_array_t_push( TYPEOF_bhvm_mcode_track_ads_s, ( bcore_array* )o, sr_tsd( TYPEOF_bhvm_mcode_track_s, v ) ); return o->data[ o->size - 1 ]; } \
  static inline bhvm_mcode_track_s* bhvm_mcode_track_ads_s_push( bhvm_mcode_track_ads_s* o ) \
  { \
      bcore_array_t_push( TYPEOF_bhvm_mcode_track_ads_s, ( bcore_array* )o, sr_t_create( TYPEOF_bhvm_mcode_track_s ) ); \
      return o->data[ o->size - 1 ]; \
  }
#define TYPEOF_bhvm_mcode_lib_s 2233001165
#define BETH_EXPAND_ITEM_bhvm_mcode_lib_s \
  BCORE_DECLARE_OBJECT( bhvm_mcode_lib_s ) \
    {aware_t _;bhvm_mcode_track_ads_s arr;bcore_hmap_tpuz_s map;}; \
  static inline void bhvm_mcode_lib_s_clear( bhvm_mcode_lib_s* o ){bhvm_mcode_track_ads_s_clear( &o->arr ); bcore_hmap_tpuz_s_clear( &o->map );} \
  static inline bl_t bhvm_mcode_lib_s_track_exists( const bhvm_mcode_lib_s* o, tp_t name ){return bcore_hmap_tpuz_s_exists( &o->map, name );} \
  bhvm_mcode_track_s* bhvm_mcode_lib_s_track_get( bhvm_mcode_lib_s* o, tp_t name ); \
  bhvm_mcode_track_s* bhvm_mcode_lib_s_track_get_or_new( bhvm_mcode_lib_s* o, tp_t name ); \
  bhvm_mcode_track_s* bhvm_mcode_lib_s_track_reset( bhvm_mcode_lib_s* o, tp_t name ); \
  static inline void bhvm_mcode_lib_s_track_vop_push_c( bhvm_mcode_lib_s* o, tp_t name, const bhvm_vop* vop ){bhvm_mcode_track_s_vop_push_c( bhvm_mcode_lib_s_track_get_or_new( o, name ), vop );} \
  static inline void bhvm_mcode_lib_s_track_vop_push_d( bhvm_mcode_lib_s* o, tp_t name, bhvm_vop* vop ){bhvm_mcode_track_s_vop_push_d( bhvm_mcode_lib_s_track_get_or_new( o, name ), vop );} \
  void bhvm_mcode_lib_s_track_push( bhvm_mcode_lib_s* o, tp_t name, tp_t src_name ); \
  void bhvm_mcode_lib_s_track_push_reverse( bhvm_mcode_lib_s* o, tp_t name, tp_t src_name ); \
  void bhvm_mcode_lib_s_track_remove( bhvm_mcode_lib_s* o, tp_t name ); \
  static inline void bhvm_mcode_lib_s_track_run( const bhvm_mcode_lib_s* o, tp_t name, bhvm_holor_s* ah ){bhvm_mcode_track_s* t = bhvm_mcode_lib_s_track_get( (bhvm_mcode_lib_s*)o, name ); if( t ) bhvm_mcode_track_s_run( t, ah );}
#define TYPEOF_bhvm_mcode_frame_s 832185203
#define BETH_EXPAND_ITEM_bhvm_mcode_frame_s \
  BCORE_DECLARE_OBJECT( bhvm_mcode_frame_s ) \
    {aware_t _;bhvm_mcode_lib_s* lib;bhvm_mcode_hbase_s* hbase;}; \
  static inline void bhvm_mcode_frame_s_track_vop_push_c( bhvm_mcode_frame_s* o, tp_t name, const bhvm_vop* vop ){bhvm_mcode_lib_s_track_vop_push_c( o->lib, name, vop );} \
  static inline void bhvm_mcode_frame_s_track_vop_push_d( bhvm_mcode_frame_s* o, tp_t name, bhvm_vop* vop ){bhvm_mcode_lib_s_track_vop_push_d( o->lib, name, vop );}
#define BETH_EXPAND_GROUP_bhvm_mcode \
  BCORE_FORWARD_OBJECT( bhvm_mcode ); \
  BCORE_FORWARD_OBJECT( bhvm_mcode_op_s ); \
  BCORE_FORWARD_OBJECT( bhvm_mcode_hmeta ); \
  BCORE_FORWARD_OBJECT( bhvm_mcode_hbase ); \
  BCORE_FORWARD_OBJECT( bhvm_mcode_track_s ); \
  BCORE_FORWARD_OBJECT( bhvm_mcode_track_ads_s ); \
  BCORE_FORWARD_OBJECT( bhvm_mcode_lib_s ); \
  BCORE_FORWARD_OBJECT( bhvm_mcode_frame_s ); \
  BETH_EXPAND_ITEM_bhvm_mcode_op_s \
  BETH_EXPAND_GROUP_bhvm_mcode_hmeta \
  BETH_EXPAND_GROUP_bhvm_mcode_hbase \
  BETH_EXPAND_ITEM_bhvm_mcode_track_s \
  BETH_EXPAND_ITEM_bhvm_mcode_track_ads_s \
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
  static inline void bhvm_mcode_hmeta_adl_s_set_space( bhvm_mcode_hmeta_adl_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_bhvm_mcode_hmeta_adl_s, ( bcore_array* )o, size ); } \
  static inline void bhvm_mcode_hmeta_adl_s_set_size( bhvm_mcode_hmeta_adl_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_bhvm_mcode_hmeta_adl_s, ( bcore_array* )o, size ); } \
  static inline void bhvm_mcode_hmeta_adl_s_clear( bhvm_mcode_hmeta_adl_s* o ) { bcore_array_t_set_space( TYPEOF_bhvm_mcode_hmeta_adl_s, ( bcore_array* )o, 0 ); } \
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
  BETH_EXPAND_ITEM_bhvm_mcode_hmeta_adl_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_mcode_hbase

#define TYPEOF_bhvm_mcode_hbase 576283881
#define TYPEOF_bhvm_mcode_hbase_s 3977623907
#define TYPEOF_bhvm_mcode_hbase_s 3977623907
#define BETH_EXPAND_ITEM_bhvm_mcode_hbase_s \
  BCORE_DECLARE_OBJECT( bhvm_mcode_hbase_s ) \
    {aware_t _;bhvm_holor_ads_s holor_ads;bhvm_mcode_hmeta_adl_s hmeta_adl;}; \
  static inline void bhvm_mcode_hbase_s_set_size( bhvm_mcode_hbase_s* o, sz_t size ){bhvm_holor_ads_s_set_size( &o->holor_ads, size  ); bhvm_mcode_hmeta_adl_s_set_size( &o->hmeta_adl, size  );} \
  static inline void bhvm_mcode_hbase_s_push_c( bhvm_mcode_hbase_s* o, const bhvm_holor_s* holor, const bhvm_mcode_hmeta* hmeta ){bhvm_holor_ads_s_push_c(   &o->holor_ads, holor ); bhvm_mcode_hmeta_adl_s_push_c(   &o->hmeta_adl, hmeta );}
#define BETH_EXPAND_GROUP_bhvm_mcode_hbase \
  BCORE_FORWARD_OBJECT( bhvm_mcode_hbase ); \
  BCORE_FORWARD_OBJECT( bhvm_mcode_hbase_s ); \
  BETH_EXPAND_ITEM_bhvm_mcode_hbase_s

/**********************************************************************************************************************/

vd_t bhvm_planted_signal_handler( const bcore_signal_s* o );

#endif // BHVM_PLANTED_H
