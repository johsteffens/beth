/** This file was generated from beth-plant source code.
 *  Compiling Agent : bcore_plant_compiler (C) 2019 J.B.Steffens
 *  Last File Update: 2020-01-28T10:49:22Z
 *
 *  Copyright and License of this File:
 *
 *  Generated code inherits the copyright and license of the underlying beth-plant source code.
 *  Source code defining this file is distributed across following files:
 *
 *  bhvm_vop.h
 *
 */

#ifndef BHVM_PLANTED_VOP_H
#define BHVM_PLANTED_VOP_H

#include "bcore_control.h"


/**********************************************************************************************************************/
// source: bhvm_vop.h

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_vop

#define TYPEOF_bhvm_vop 3907802388
#define TYPEOF_bhvm_vop_s 1194015046
#define TYPEOF_bhvm_vop_arr_s 3971129534
#define BETH_EXPAND_ITEM_bhvm_vop_arr_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_arr_s ) \
    {aware_t _;BCORE_ARRAY_DYN_LINK_STATIC_S( bhvm_vop, );}; \
  static inline bhvm_vop_arr_s* bhvm_vop_arr_s_set_space( bhvm_vop_arr_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_bhvm_vop_arr_s, ( bcore_array* )o, size ); return o; } \
  static inline bhvm_vop_arr_s* bhvm_vop_arr_s_set_size( bhvm_vop_arr_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_bhvm_vop_arr_s, ( bcore_array* )o, size ); return o; } \
  static inline bhvm_vop_arr_s* bhvm_vop_arr_s_clear( bhvm_vop_arr_s* o ) { bcore_array_t_set_space( TYPEOF_bhvm_vop_arr_s, ( bcore_array* )o, 0 ); return o; } \
  static inline bhvm_vop* bhvm_vop_arr_s_push_c( bhvm_vop_arr_s* o, const bhvm_vop* v ) { bcore_array_t_push( TYPEOF_bhvm_vop_arr_s, ( bcore_array* )o, sr_awc( v ) ); return o->data[ o->size - 1 ]; } \
  static inline bhvm_vop* bhvm_vop_arr_s_push_d( bhvm_vop_arr_s* o,       bhvm_vop* v ) { bcore_array_t_push( TYPEOF_bhvm_vop_arr_s, ( bcore_array* )o, sr_asd( v ) ); return o->data[ o->size - 1 ]; } \
  static inline bhvm_vop* bhvm_vop_arr_s_push_t( bhvm_vop_arr_s* o, tp_t t ) \
  { \
      bcore_trait_assert_satisfied_type( TYPEOF_bhvm_vop, t ); \
      bcore_array_t_push( TYPEOF_bhvm_vop_arr_s, ( bcore_array* )o, sr_t_create( t ) ); \
      return o->data[ o->size - 1 ]; \
  }
#define TYPEOF_bhvm_vop_ci_s 711211199
#define BETH_EXPAND_ITEM_bhvm_vop_ci_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ci_s ) \
    {u0_t c;sz_t i;}; \
  static inline void bhvm_vop_ci_s_push_ci( bhvm_vop_ci_s* o, u0_t c, sz_t i ){o->c = c; o->i = i;}
#define TYPEOF_bhvm_vop_arr_ci_s 791850599
#define BETH_EXPAND_ITEM_bhvm_vop_arr_ci_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_arr_ci_s ) \
    {aware_t _;BCORE_ARRAY_DYN_SOLID_STATIC_S( bhvm_vop_ci_s, );}; \
  void bhvm_vop_arr_ci_s_push_ci( bhvm_vop_arr_ci_s* o, u0_t c, sz_t i ); \
  static inline sz_t bhvm_vop_arr_ci_s_i_of_c( const bhvm_vop_arr_ci_s* o, u0_t c ){BFOR_EACH( j, o ) if( o->data[ j ].c == c ) return o->data[ j ].i; return -1;} \
  static inline u0_t bhvm_vop_arr_ci_s_c_of_i( const bhvm_vop_arr_ci_s* o, sz_t i ){BFOR_EACH( j, o ) if( o->data[ j ].i == i ) return o->data[ j ].c; return -1;} \
  static inline bhvm_vop_arr_ci_s* bhvm_vop_arr_ci_s_set_space( bhvm_vop_arr_ci_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_bhvm_vop_arr_ci_s, ( bcore_array* )o, size ); return o; } \
  static inline bhvm_vop_arr_ci_s* bhvm_vop_arr_ci_s_set_size( bhvm_vop_arr_ci_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_bhvm_vop_arr_ci_s, ( bcore_array* )o, size ); return o; } \
  static inline bhvm_vop_arr_ci_s* bhvm_vop_arr_ci_s_clear( bhvm_vop_arr_ci_s* o ) { bcore_array_t_set_space( TYPEOF_bhvm_vop_arr_ci_s, ( bcore_array* )o, 0 ); return o; } \
  static inline bhvm_vop_ci_s* bhvm_vop_arr_ci_s_push_c( bhvm_vop_arr_ci_s* o, const bhvm_vop_ci_s* v ) { bcore_array_t_push( TYPEOF_bhvm_vop_arr_ci_s, ( bcore_array* )o, sr_twc( TYPEOF_bhvm_vop_ci_s, v ) ); return &o->data[ o->size - 1 ]; } \
  static inline bhvm_vop_ci_s* bhvm_vop_arr_ci_s_push_d( bhvm_vop_arr_ci_s* o,       bhvm_vop_ci_s* v ) { bcore_array_t_push( TYPEOF_bhvm_vop_arr_ci_s, ( bcore_array* )o, sr_tsd( TYPEOF_bhvm_vop_ci_s, v ) ); return &o->data[ o->size - 1 ]; } \
  static inline bhvm_vop_ci_s* bhvm_vop_arr_ci_s_push( bhvm_vop_arr_ci_s* o ) \
  { \
      bcore_array_t_push( TYPEOF_bhvm_vop_arr_ci_s, ( bcore_array* )o, sr_null() ); \
      return &o->data[ o->size - 1 ]; \
  }
#define BETH_EXPAND_GROUP_bhvm_vop \
  BCORE_FORWARD_OBJECT( bhvm_vop ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_arr_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ci_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_arr_ci_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar0 ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1 ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2 ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar3 ); \
  typedef void (*bhvm_vop_run)( const bhvm_vop* o, bhvm_holor_s* ah ); \
  typedef sz_t (*bhvm_vop_arity)( const bhvm_vop* o ); \
  typedef sc_t (*bhvm_vop_sig)( const bhvm_vop* o ); \
  typedef sz_t* (*bhvm_vop_get_index_arr)( bhvm_vop* o ); \
  typedef sz_t (*bhvm_vop_get_index)( const bhvm_vop* o, sz_t index ); \
  typedef bhvm_vop* (*bhvm_vop_set_index)( bhvm_vop* o, sz_t index, sz_t idx_val ); \
  typedef void (*bhvm_vop_set_arg)( bhvm_vop* o, bhvm_vop_ci_s* ci ); \
  typedef bhvm_vop* (*bhvm_vop_set_args)( bhvm_vop* o, const bhvm_vop_arr_ci_s* arr ); \
  BCORE_DECLARE_SPECT( bhvm_vop ) \
  { \
      bcore_spect_header_s header; \
      bhvm_vop_run run; \
      bhvm_vop_arity arity; \
      bhvm_vop_sig sig; \
      bhvm_vop_get_index_arr get_index_arr; \
      bhvm_vop_get_index get_index; \
      bhvm_vop_set_index set_index; \
      bhvm_vop_set_arg set_arg; \
      bhvm_vop_set_args set_args; \
  }; \
  static inline bhvm_vop* bhvm_vop_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_bhvm_vop, t ); return ( bhvm_vop* )bcore_inst_t_create( t ); } \
  static inline bl_t bhvm_vop_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_bhvm_vop ); } \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bhvm_vop ) \
  static inline bl_t bhvm_vop_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_bhvm_vop ); } \
  BETH_EXPAND_ITEM_bhvm_vop_arr_s \
  static inline void bhvm_vop_p_run( const bhvm_vop_s* __p, const bhvm_vop* o, bhvm_holor_s* ah ) { assert( __p->run ); __p->run( o, ah ); } \
  static inline void bhvm_vop_a_run( const bhvm_vop* o, bhvm_holor_s* ah ) { const bhvm_vop_s* p = bhvm_vop_s_get_aware( o ); assert( p->run ); p->run( o, ah ); } \
  static inline bl_t bhvm_vop_p_defines_run( const bhvm_vop_s* __p ) { return __p->run != NULL; } \
  static inline bl_t bhvm_vop_a_defines_run( const bhvm_vop* o ) { return bhvm_vop_s_get_aware( o )->run != NULL; } \
  static inline sz_t bhvm_vop_a_arity( const bhvm_vop* o ) { const bhvm_vop_s* p = bhvm_vop_s_get_aware( o ); assert( p->arity ); return p->arity( o ); } \
  static inline bl_t bhvm_vop_a_defines_arity( const bhvm_vop* o ) { return bhvm_vop_s_get_aware( o )->arity != NULL; } \
  static inline sc_t bhvm_vop_a_sig( const bhvm_vop* o ) { const bhvm_vop_s* p = bhvm_vop_s_get_aware( o ); assert( p->sig ); return p->sig( o ); } \
  static inline bl_t bhvm_vop_a_defines_sig( const bhvm_vop* o ) { return bhvm_vop_s_get_aware( o )->sig != NULL; } \
  static inline sz_t* bhvm_vop_a_get_index_arr( bhvm_vop* o ) { const bhvm_vop_s* p = bhvm_vop_s_get_aware( o ); assert( p->get_index_arr ); return p->get_index_arr( o ); } \
  static inline bl_t bhvm_vop_a_defines_get_index_arr( const bhvm_vop* o ) { return bhvm_vop_s_get_aware( o )->get_index_arr != NULL; } \
  static inline sz_t bhvm_vop_a_get_index( const bhvm_vop* o, sz_t index ) { const bhvm_vop_s* p = bhvm_vop_s_get_aware( o ); assert( p->get_index ); return p->get_index( o, index ); } \
  static inline bl_t bhvm_vop_a_defines_get_index( const bhvm_vop* o ) { return true; } \
  sz_t bhvm_vop_get_index__( const bhvm_vop* o, sz_t index ); \
  static inline bhvm_vop* bhvm_vop_a_set_index( bhvm_vop* o, sz_t index, sz_t idx_val ) { const bhvm_vop_s* p = bhvm_vop_s_get_aware( o ); assert( p->set_index ); return p->set_index( o, index, idx_val ); } \
  static inline bl_t bhvm_vop_a_defines_set_index( const bhvm_vop* o ) { return true; } \
  bhvm_vop* bhvm_vop_set_index__( bhvm_vop* o, sz_t index, sz_t idx_val ); \
  BETH_EXPAND_ITEM_bhvm_vop_ci_s \
  BETH_EXPAND_ITEM_bhvm_vop_arr_ci_s \
  static inline void bhvm_vop_a_set_arg( bhvm_vop* o, bhvm_vop_ci_s* ci ) { const bhvm_vop_s* p = bhvm_vop_s_get_aware( o ); assert( p->set_arg ); p->set_arg( o, ci ); } \
  static inline bl_t bhvm_vop_a_defines_set_arg( const bhvm_vop* o ) { return true; } \
  void bhvm_vop_set_arg__( bhvm_vop* o, bhvm_vop_ci_s* ci ); \
  static inline bhvm_vop* bhvm_vop_a_set_args( bhvm_vop* o, const bhvm_vop_arr_ci_s* arr ) { const bhvm_vop_s* p = bhvm_vop_s_get_aware( o ); assert( p->set_args ); return p->set_args( o, arr ); } \
  static inline bl_t bhvm_vop_a_defines_set_args( const bhvm_vop* o ) { return true; } \
  bhvm_vop* bhvm_vop_set_args__( bhvm_vop* o, const bhvm_vop_arr_ci_s* arr ); \
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
  static inline sz_t* bhvm_vop_ar0_zro_s_get_index_arr( bhvm_vop_ar0_zro_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar0_zro_s_run( const bhvm_vop_ar0_zro_s* o, bhvm_holor_s* ah ){bhvm_hop_ar0_zro_s_f( &ah[o->i.v[0]] );} \
  static inline sc_t bhvm_vop_ar0_zro_s_sig( const bhvm_vop_ar0_zro_s* o ){return "y";}
#define TYPEOF_bhvm_vop_ar0_one_s 131160455
#define BETH_EXPAND_ITEM_bhvm_vop_ar0_one_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar0_one_s ) \
    {aware_t _;bhvm_vop_ar0_index_s i;}; \
  static inline sz_t bhvm_vop_ar0_one_s_arity( const bhvm_vop_ar0_one_s* o ){return 0;} \
  static inline sz_t* bhvm_vop_ar0_one_s_get_index_arr( bhvm_vop_ar0_one_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar0_one_s_run( const bhvm_vop_ar0_one_s* o, bhvm_holor_s* ah ){bhvm_hop_ar0_one_s_f( &ah[o->i.v[0]] );} \
  static inline sc_t bhvm_vop_ar0_one_s_sig( const bhvm_vop_ar0_one_s* o ){return "y";}
#define TYPEOF_bhvm_vop_ar0_determine_s 4243788798
#define BETH_EXPAND_ITEM_bhvm_vop_ar0_determine_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar0_determine_s ) \
    {aware_t _;bhvm_vop_ar0_index_s i;}; \
  static inline sz_t bhvm_vop_ar0_determine_s_arity( const bhvm_vop_ar0_determine_s* o ){return 0;} \
  static inline sz_t* bhvm_vop_ar0_determine_s_get_index_arr( bhvm_vop_ar0_determine_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar0_determine_s_run( const bhvm_vop_ar0_determine_s* o, bhvm_holor_s* ah ){bhvm_holor_s_fit_size( &ah[o->i.v[0]] );} \
  static inline sc_t bhvm_vop_ar0_determine_s_sig( const bhvm_vop_ar0_determine_s* o ){return "y";}
#define TYPEOF_bhvm_vop_ar0_vacate_s 1495941295
#define BETH_EXPAND_ITEM_bhvm_vop_ar0_vacate_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar0_vacate_s ) \
    {aware_t _;bhvm_vop_ar0_index_s i;}; \
  static inline sz_t bhvm_vop_ar0_vacate_s_arity( const bhvm_vop_ar0_vacate_s* o ){return 0;} \
  static inline sz_t* bhvm_vop_ar0_vacate_s_get_index_arr( bhvm_vop_ar0_vacate_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar0_vacate_s_run( const bhvm_vop_ar0_vacate_s* o, bhvm_holor_s* ah ){bhvm_value_s_clear( &ah[o->i.v[0]].v );} \
  static inline sc_t bhvm_vop_ar0_vacate_s_sig( const bhvm_vop_ar0_vacate_s* o ){return "y";}
#define TYPEOF_bhvm_vop_ar0_randomize_s 3200359220
#define BETH_EXPAND_ITEM_bhvm_vop_ar0_randomize_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar0_randomize_s ) \
    {aware_t _;bhvm_vop_ar0_index_s i;u2_t rseed;f3_t min;f3_t max;f3_t density;}; \
  static inline sz_t bhvm_vop_ar0_randomize_s_arity( const bhvm_vop_ar0_randomize_s* o ){return 0;} \
  static inline sz_t* bhvm_vop_ar0_randomize_s_get_index_arr( bhvm_vop_ar0_randomize_s* o ){return o->i.v;} \
  void bhvm_vop_ar0_randomize_s_run( const bhvm_vop_ar0_randomize_s* o, bhvm_holor_s* ah ); \
  static inline sc_t bhvm_vop_ar0_randomize_s_sig( const bhvm_vop_ar0_randomize_s* o ){return "y";}
#define TYPEOF_bhvm_vop_ar0_nul_dp_s 2224397213
#define BETH_EXPAND_ITEM_bhvm_vop_ar0_nul_dp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar0_nul_dp_s ) \
    {aware_t _;bhvm_vop_ar0_index_s i;}; \
  static inline sz_t bhvm_vop_ar0_nul_dp_s_arity( const bhvm_vop_ar0_nul_dp_s* o ){return 0;} \
  static inline sz_t* bhvm_vop_ar0_nul_dp_s_get_index_arr( bhvm_vop_ar0_nul_dp_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar0_nul_dp_s_run( const bhvm_vop_ar0_nul_dp_s* o, bhvm_holor_s* ah ){} \
  static inline sc_t bhvm_vop_ar0_nul_dp_s_sig( const bhvm_vop_ar0_nul_dp_s* o ){return "f";}
#define BETH_EXPAND_GROUP_bhvm_vop_ar0 \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar0 ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar0_index_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar0_zro_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar0_one_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar0_determine_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar0_vacate_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar0_randomize_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar0_nul_dp_s ); \
  BETH_EXPAND_ITEM_bhvm_vop_ar0_index_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar0_zro_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar0_one_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar0_determine_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar0_vacate_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar0_randomize_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar0_nul_dp_s

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
  static inline sz_t* bhvm_vop_ar1_identity_s_get_index_arr( bhvm_vop_ar1_identity_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_identity_s_sig( const bhvm_vop_ar1_identity_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_identity_s_run( const bhvm_vop_ar1_identity_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_identity_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_neg_s 4102927266
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_neg_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_neg_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_neg_s_arity( const bhvm_vop_ar1_neg_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_neg_s_get_index_arr( bhvm_vop_ar1_neg_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_neg_s_sig( const bhvm_vop_ar1_neg_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_neg_s_run( const bhvm_vop_ar1_neg_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_neg_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_floor_s 294636938
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_floor_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_floor_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_floor_s_arity( const bhvm_vop_ar1_floor_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_floor_s_get_index_arr( bhvm_vop_ar1_floor_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_floor_s_sig( const bhvm_vop_ar1_floor_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_floor_s_run( const bhvm_vop_ar1_floor_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_floor_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_ceil_s 2964243205
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_ceil_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_ceil_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_ceil_s_arity( const bhvm_vop_ar1_ceil_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_ceil_s_get_index_arr( bhvm_vop_ar1_ceil_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_ceil_s_sig( const bhvm_vop_ar1_ceil_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_ceil_s_run( const bhvm_vop_ar1_ceil_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_ceil_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_exp_s 3731945467
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_exp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_exp_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_exp_s_arity( const bhvm_vop_ar1_exp_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_exp_s_get_index_arr( bhvm_vop_ar1_exp_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_exp_s_sig( const bhvm_vop_ar1_exp_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_exp_s_run( const bhvm_vop_ar1_exp_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_exp_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_log_s 3614077830
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_log_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_log_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_log_s_arity( const bhvm_vop_ar1_log_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_log_s_get_index_arr( bhvm_vop_ar1_log_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_log_s_sig( const bhvm_vop_ar1_log_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_log_s_run( const bhvm_vop_ar1_log_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_log_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_inv_s 381359607
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_inv_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_inv_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_inv_s_arity( const bhvm_vop_ar1_inv_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_inv_s_get_index_arr( bhvm_vop_ar1_inv_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_inv_s_sig( const bhvm_vop_ar1_inv_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_inv_s_run( const bhvm_vop_ar1_inv_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_inv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_sqr_s 2596911066
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_sqr_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_sqr_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_sqr_s_arity( const bhvm_vop_ar1_sqr_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_sqr_s_get_index_arr( bhvm_vop_ar1_sqr_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_sqr_s_sig( const bhvm_vop_ar1_sqr_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_sqr_s_run( const bhvm_vop_ar1_sqr_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_sqr_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_srt_s 942160667
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_srt_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_srt_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_srt_s_arity( const bhvm_vop_ar1_srt_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_srt_s_get_index_arr( bhvm_vop_ar1_srt_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_srt_s_sig( const bhvm_vop_ar1_srt_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_srt_s_run( const bhvm_vop_ar1_srt_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_srt_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_abs_s 3513184244
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_abs_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_abs_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_abs_s_arity( const bhvm_vop_ar1_abs_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_abs_s_get_index_arr( bhvm_vop_ar1_abs_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_abs_s_sig( const bhvm_vop_ar1_abs_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_abs_s_run( const bhvm_vop_ar1_abs_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_abs_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_l1_s 2512502983
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_l1_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_l1_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_l1_s_arity( const bhvm_vop_ar1_l1_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_l1_s_get_index_arr( bhvm_vop_ar1_l1_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_l1_s_sig( const bhvm_vop_ar1_l1_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_l1_s_run( const bhvm_vop_ar1_l1_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_l1_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_lgst_s 3402156782
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_lgst_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_lgst_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_lgst_s_arity( const bhvm_vop_ar1_lgst_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_lgst_s_get_index_arr( bhvm_vop_ar1_lgst_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_lgst_s_sig( const bhvm_vop_ar1_lgst_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_lgst_s_run( const bhvm_vop_ar1_lgst_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_lgst_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_lgst_hard_s 1312586658
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_lgst_hard_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_lgst_hard_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_lgst_hard_s_arity( const bhvm_vop_ar1_lgst_hard_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_lgst_hard_s_get_index_arr( bhvm_vop_ar1_lgst_hard_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_lgst_hard_s_sig( const bhvm_vop_ar1_lgst_hard_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_lgst_hard_s_run( const bhvm_vop_ar1_lgst_hard_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_lgst_hard_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_lgst_leaky_s 3970548331
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_lgst_leaky_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_lgst_leaky_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_lgst_leaky_s_arity( const bhvm_vop_ar1_lgst_leaky_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_lgst_leaky_s_get_index_arr( bhvm_vop_ar1_lgst_leaky_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_lgst_leaky_s_sig( const bhvm_vop_ar1_lgst_leaky_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_lgst_leaky_s_run( const bhvm_vop_ar1_lgst_leaky_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_lgst_leaky_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_tanh_s 3583954501
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_tanh_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_tanh_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_tanh_s_arity( const bhvm_vop_ar1_tanh_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_tanh_s_get_index_arr( bhvm_vop_ar1_tanh_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_tanh_s_sig( const bhvm_vop_ar1_tanh_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_tanh_s_run( const bhvm_vop_ar1_tanh_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_tanh_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_tanh_hard_s 71062515
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_tanh_hard_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_tanh_hard_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_tanh_hard_s_arity( const bhvm_vop_ar1_tanh_hard_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_tanh_hard_s_get_index_arr( bhvm_vop_ar1_tanh_hard_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_tanh_hard_s_sig( const bhvm_vop_ar1_tanh_hard_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_tanh_hard_s_run( const bhvm_vop_ar1_tanh_hard_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_tanh_hard_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_tanh_leaky_s 567258088
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_tanh_leaky_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_tanh_leaky_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_tanh_leaky_s_arity( const bhvm_vop_ar1_tanh_leaky_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_tanh_leaky_s_get_index_arr( bhvm_vop_ar1_tanh_leaky_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_tanh_leaky_s_sig( const bhvm_vop_ar1_tanh_leaky_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_tanh_leaky_s_run( const bhvm_vop_ar1_tanh_leaky_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_tanh_leaky_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_softplus_s 3784219040
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_softplus_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_softplus_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_softplus_s_arity( const bhvm_vop_ar1_softplus_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_softplus_s_get_index_arr( bhvm_vop_ar1_softplus_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_softplus_s_sig( const bhvm_vop_ar1_softplus_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_softplus_s_run( const bhvm_vop_ar1_softplus_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_softplus_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_relu_s 3052643634
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_relu_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_relu_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_relu_s_arity( const bhvm_vop_ar1_relu_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_relu_s_get_index_arr( bhvm_vop_ar1_relu_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_relu_s_sig( const bhvm_vop_ar1_relu_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_relu_s_run( const bhvm_vop_ar1_relu_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_relu_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_relu_leaky_s 1291048615
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_relu_leaky_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_relu_leaky_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_relu_leaky_s_arity( const bhvm_vop_ar1_relu_leaky_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_relu_leaky_s_get_index_arr( bhvm_vop_ar1_relu_leaky_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_relu_leaky_s_sig( const bhvm_vop_ar1_relu_leaky_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_relu_leaky_s_run( const bhvm_vop_ar1_relu_leaky_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_relu_leaky_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_order_inc_s 2313321483
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_order_inc_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_order_inc_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;sz_t dim;}; \
  static inline sz_t bhvm_vop_ar1_order_inc_s_arity( const bhvm_vop_ar1_order_inc_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_order_inc_s_get_index_arr( bhvm_vop_ar1_order_inc_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar1_order_inc_s_run( const bhvm_vop_ar1_order_inc_s* o, bhvm_holor_s* ah ){bhvm_holor_s_order_inc( &ah[o->i.v[0]], o->dim, &ah[o->i.v[1]] );} \
  static inline sc_t bhvm_vop_ar1_order_inc_s_sig( const bhvm_vop_ar1_order_inc_s* o ){return "ay";}
#define TYPEOF_bhvm_vop_ar1_order_dec_s 2688499259
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_order_dec_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_order_dec_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;sz_t idx;}; \
  static inline sz_t bhvm_vop_ar1_order_dec_s_arity( const bhvm_vop_ar1_order_dec_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_order_dec_s_get_index_arr( bhvm_vop_ar1_order_dec_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar1_order_dec_s_run( const bhvm_vop_ar1_order_dec_s* o, bhvm_holor_s* ah ){bhvm_holor_s_order_dec( &ah[o->i.v[0]], o->idx, &ah[o->i.v[1]] );} \
  static inline sc_t bhvm_vop_ar1_order_dec_s_sig( const bhvm_vop_ar1_order_dec_s* o ){return "ay";}
#define TYPEOF_bhvm_vop_ar1_order_dec_fork_s 2965629832
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_order_dec_fork_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_order_dec_fork_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;sz_t idx;}; \
  static inline sz_t bhvm_vop_ar1_order_dec_fork_s_arity( const bhvm_vop_ar1_order_dec_fork_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_order_dec_fork_s_get_index_arr( bhvm_vop_ar1_order_dec_fork_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar1_order_dec_fork_s_run( const bhvm_vop_ar1_order_dec_fork_s* o, bhvm_holor_s* ah ){bhvm_holor_s_order_dec_fork( &ah[o->i.v[0]], o->idx, &ah[o->i.v[1]] );} \
  static inline sc_t bhvm_vop_ar1_order_dec_fork_s_sig( const bhvm_vop_ar1_order_dec_fork_s* o ){return "ay";}
#define TYPEOF_bhvm_vop_ar1_add_dp_a_s 4154047980
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_add_dp_a_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_add_dp_a_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_add_dp_a_s_arity( const bhvm_vop_ar1_add_dp_a_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_add_dp_a_s_get_index_arr( bhvm_vop_ar1_add_dp_a_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar1_add_dp_a_s_run( const bhvm_vop_ar1_add_dp_a_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_eci_cpy_acc_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );} \
  static inline sc_t bhvm_vop_ar1_add_dp_a_s_sig( const bhvm_vop_ar1_add_dp_a_s* o ){return "zf";}
#define TYPEOF_bhvm_vop_ar1_add_dp_b_s 2188512841
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_add_dp_b_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_add_dp_b_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_add_dp_b_s_arity( const bhvm_vop_ar1_add_dp_b_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_add_dp_b_s_get_index_arr( bhvm_vop_ar1_add_dp_b_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar1_add_dp_b_s_run( const bhvm_vop_ar1_add_dp_b_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_eci_cpy_acc_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );} \
  static inline sc_t bhvm_vop_ar1_add_dp_b_s_sig( const bhvm_vop_ar1_add_dp_b_s* o ){return "zg";}
#define TYPEOF_bhvm_vop_ar1_sub_dp_a_s 1634025359
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_sub_dp_a_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_sub_dp_a_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_sub_dp_a_s_arity( const bhvm_vop_ar1_sub_dp_a_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_sub_dp_a_s_get_index_arr( bhvm_vop_ar1_sub_dp_a_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar1_sub_dp_a_s_run( const bhvm_vop_ar1_sub_dp_a_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_eci_cpy_acc_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );} \
  static inline sc_t bhvm_vop_ar1_sub_dp_a_s_sig( const bhvm_vop_ar1_sub_dp_a_s* o ){return "zf";}
#define TYPEOF_bhvm_vop_ar1_sub_dp_b_s 300932310
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_sub_dp_b_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_sub_dp_b_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_sub_dp_b_s_arity( const bhvm_vop_ar1_sub_dp_b_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_sub_dp_b_s_get_index_arr( bhvm_vop_ar1_sub_dp_b_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar1_sub_dp_b_s_run( const bhvm_vop_ar1_sub_dp_b_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_eci_neg_acc_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );} \
  static inline sc_t bhvm_vop_ar1_sub_dp_b_s_sig( const bhvm_vop_ar1_sub_dp_b_s* o ){return "zg";}
#define TYPEOF_bhvm_vop_ar1_identity_dp_s 3432655863
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_identity_dp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_identity_dp_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_identity_dp_s_arity( const bhvm_vop_ar1_identity_dp_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_identity_dp_s_get_index_arr( bhvm_vop_ar1_identity_dp_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar1_identity_dp_s_run( const bhvm_vop_ar1_identity_dp_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_eci_cpy_acc_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );} \
  static inline sc_t bhvm_vop_ar1_identity_dp_s_sig( const bhvm_vop_ar1_identity_dp_s* o ){return "zf";}
#define TYPEOF_bhvm_vop_ar1_neg_dp_s 2857525139
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_neg_dp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_neg_dp_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_neg_dp_s_arity( const bhvm_vop_ar1_neg_dp_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_neg_dp_s_get_index_arr( bhvm_vop_ar1_neg_dp_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar1_neg_dp_s_run( const bhvm_vop_ar1_neg_dp_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_eci_neg_acc_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );} \
  static inline sc_t bhvm_vop_ar1_neg_dp_s_sig( const bhvm_vop_ar1_neg_dp_s* o ){return "zf";}
#define TYPEOF_bhvm_vop_ar1_cpy_ay_s 3008825357
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_cpy_ay_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_cpy_ay_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_cpy_ay_s_arity( const bhvm_vop_ar1_cpy_ay_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_cpy_ay_s_get_index_arr( bhvm_vop_ar1_cpy_ay_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_cpy_ay_s_sig( const bhvm_vop_ar1_cpy_ay_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_cpy_ay_s_run( const bhvm_vop_ar1_cpy_ay_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_identity_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_cpy_by_s 1671780378
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_cpy_by_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_cpy_by_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_cpy_by_s_arity( const bhvm_vop_ar1_cpy_by_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_cpy_by_s_get_index_arr( bhvm_vop_ar1_cpy_by_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_cpy_by_s_sig( const bhvm_vop_ar1_cpy_by_s* o ){return "by";} \
  static inline void bhvm_vop_ar1_cpy_by_s_run( const bhvm_vop_ar1_cpy_by_s* o, bhvm_holor_s* ah ){bhvm_hop_ar1_identity_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_fork_s 438891684
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_fork_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_fork_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_fork_s_arity( const bhvm_vop_ar1_fork_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_fork_s_get_index_arr( bhvm_vop_ar1_fork_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_fork_s_sig( const bhvm_vop_ar1_fork_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_fork_s_run( const bhvm_vop_ar1_fork_s* o, bhvm_holor_s* ah ){bhvm_holor_s_fork( &ah[o->i.v[1]], &ah[o->i.v[0]] );}
#define BETH_EXPAND_GROUP_bhvm_vop_ar1 \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1 ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_index_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_identity_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_neg_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_floor_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_ceil_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_exp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_log_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_inv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_sqr_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_srt_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_abs_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_l1_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_lgst_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_lgst_hard_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_lgst_leaky_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_tanh_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_tanh_hard_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_tanh_leaky_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_softplus_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_relu_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_relu_leaky_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_order_inc_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_order_dec_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_order_dec_fork_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_add_dp_a_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_add_dp_b_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_sub_dp_a_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_sub_dp_b_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_identity_dp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_neg_dp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_cpy_ay_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_cpy_by_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_fork_s ); \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_index_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_identity_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_neg_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_floor_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_ceil_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_exp_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_log_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_inv_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_sqr_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_srt_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_abs_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_l1_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_lgst_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_lgst_hard_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_lgst_leaky_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_tanh_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_tanh_hard_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_tanh_leaky_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_softplus_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_relu_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_relu_leaky_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_order_inc_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_order_dec_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_order_dec_fork_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_add_dp_a_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_add_dp_b_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_sub_dp_a_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_sub_dp_b_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_identity_dp_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_neg_dp_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_cpy_ay_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_cpy_by_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_fork_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_vop_ar2

#define TYPEOF_bhvm_vop_ar2 1790832988
#define TYPEOF_bhvm_vop_ar2_s 2982765310
#define TYPEOF_bhvm_vop_ar2_index_s 180378457
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_index_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_index_s ) \
    {sz_t v[ 3 ];};
#define TYPEOF_bhvm_vop_ar2_cat_s 2758102237
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_cat_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_cat_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_cat_s_arity( const bhvm_vop_ar2_cat_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_cat_s_get_index_arr( bhvm_vop_ar2_cat_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_cat_s_sig( const bhvm_vop_ar2_cat_s* o ){return "aby";} \
  static inline void bhvm_vop_ar2_cat_s_run( const bhvm_vop_ar2_cat_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_cat_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_add_s 1453720506
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_add_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_add_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_add_s_arity( const bhvm_vop_ar2_add_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_add_s_get_index_arr( bhvm_vop_ar2_add_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_add_s_run( const bhvm_vop_ar2_add_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_eci_add_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_add_s_sig( const bhvm_vop_ar2_add_s* o ){return "aby";}
#define TYPEOF_bhvm_vop_ar2_sub_s 4210861187
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_sub_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_sub_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_sub_s_arity( const bhvm_vop_ar2_sub_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_sub_s_get_index_arr( bhvm_vop_ar2_sub_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_sub_s_run( const bhvm_vop_ar2_sub_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_eci_sub_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_sub_s_sig( const bhvm_vop_ar2_sub_s* o ){return "aby";}
#define TYPEOF_bhvm_vop_ar2_div_s 3736072358
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_div_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_div_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_div_s_arity( const bhvm_vop_ar2_div_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_div_s_get_index_arr( bhvm_vop_ar2_div_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_div_s_run( const bhvm_vop_ar2_div_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_eci_div_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_div_s_sig( const bhvm_vop_ar2_div_s* o ){return "aby";}
#define TYPEOF_bhvm_vop_ar2_mul_vvv_s 1776950774
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vvv_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_vvv_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_vvv_s_arity( const bhvm_vop_ar2_mul_vvv_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_vvv_s_get_index_arr( bhvm_vop_ar2_mul_vvv_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_mul_vvv_s_sig( const bhvm_vop_ar2_mul_vvv_s* o ){return "aby";} \
  static inline void bhvm_vop_ar2_mul_vvv_s_run( const bhvm_vop_ar2_mul_vvv_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_vvv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_mul_vsv_s 3710841495
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vsv_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_vsv_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_vsv_s_arity( const bhvm_vop_ar2_mul_vsv_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_vsv_s_get_index_arr( bhvm_vop_ar2_mul_vsv_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_mul_vsv_s_sig( const bhvm_vop_ar2_mul_vsv_s* o ){return "aby";} \
  static inline void bhvm_vop_ar2_mul_vsv_s_run( const bhvm_vop_ar2_mul_vsv_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_vsv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_mul_svv_s 1653340249
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_svv_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_svv_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_svv_s_arity( const bhvm_vop_ar2_mul_svv_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_svv_s_get_index_arr( bhvm_vop_ar2_mul_svv_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_mul_svv_s_sig( const bhvm_vop_ar2_mul_svv_s* o ){return "aby";} \
  static inline void bhvm_vop_ar2_mul_svv_s_run( const bhvm_vop_ar2_mul_svv_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_svv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_mul_vvs_s 1011872477
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vvs_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_vvs_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_vvs_s_arity( const bhvm_vop_ar2_mul_vvs_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_vvs_s_get_index_arr( bhvm_vop_ar2_mul_vvs_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_mul_vvs_s_sig( const bhvm_vop_ar2_mul_vvs_s* o ){return "aby";} \
  static inline void bhvm_vop_ar2_mul_vvs_s_run( const bhvm_vop_ar2_mul_vvs_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_vvs_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_mul_mvv_s 1358382171
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_mvv_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_mvv_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_mvv_s_arity( const bhvm_vop_ar2_mul_mvv_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_mvv_s_get_index_arr( bhvm_vop_ar2_mul_mvv_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_mul_mvv_s_sig( const bhvm_vop_ar2_mul_mvv_s* o ){return "aby";} \
  static inline void bhvm_vop_ar2_mul_mvv_s_run( const bhvm_vop_ar2_mul_mvv_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_mvv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_mul_vmv_s 3016743869
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vmv_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_vmv_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_vmv_s_arity( const bhvm_vop_ar2_mul_vmv_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_vmv_s_get_index_arr( bhvm_vop_ar2_mul_vmv_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_mul_vmv_s_sig( const bhvm_vop_ar2_mul_vmv_s* o ){return "aby";} \
  static inline void bhvm_vop_ar2_mul_vmv_s_run( const bhvm_vop_ar2_mul_vmv_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_vmv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_mul_tvv_s 1232258116
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_tvv_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_tvv_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_tvv_s_arity( const bhvm_vop_ar2_mul_tvv_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_tvv_s_get_index_arr( bhvm_vop_ar2_mul_tvv_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_mul_tvv_s_sig( const bhvm_vop_ar2_mul_tvv_s* o ){return "aby";} \
  static inline void bhvm_vop_ar2_mul_tvv_s_run( const bhvm_vop_ar2_mul_tvv_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_tvv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_mul_vtv_s 2342067800
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vtv_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_vtv_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_vtv_s_arity( const bhvm_vop_ar2_mul_vtv_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_vtv_s_get_index_arr( bhvm_vop_ar2_mul_vtv_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_mul_vtv_s_sig( const bhvm_vop_ar2_mul_vtv_s* o ){return "aby";} \
  static inline void bhvm_vop_ar2_mul_vtv_s_run( const bhvm_vop_ar2_mul_vtv_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_vtv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_mul_vvm_s 817586423
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vvm_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_vvm_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_vvm_s_arity( const bhvm_vop_ar2_mul_vvm_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_vvm_s_get_index_arr( bhvm_vop_ar2_mul_vvm_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_mul_vvm_s_sig( const bhvm_vop_ar2_mul_vvm_s* o ){return "aby";} \
  static inline void bhvm_vop_ar2_mul_vvm_s_run( const bhvm_vop_ar2_mul_vvm_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_vvm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_mul_mmm_s 962810917
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_mmm_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_mmm_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_mmm_s_arity( const bhvm_vop_ar2_mul_mmm_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_mmm_s_get_index_arr( bhvm_vop_ar2_mul_mmm_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_mul_mmm_s_sig( const bhvm_vop_ar2_mul_mmm_s* o ){return "aby";} \
  static inline void bhvm_vop_ar2_mul_mmm_s_run( const bhvm_vop_ar2_mul_mmm_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_mmm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_mul_mtm_s 244623344
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_mtm_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_mtm_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_mtm_s_arity( const bhvm_vop_ar2_mul_mtm_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_mtm_s_get_index_arr( bhvm_vop_ar2_mul_mtm_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_mul_mtm_s_sig( const bhvm_vop_ar2_mul_mtm_s* o ){return "aby";} \
  static inline void bhvm_vop_ar2_mul_mtm_s_run( const bhvm_vop_ar2_mul_mtm_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_mtm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_mul_tmm_s 3530366174
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_tmm_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_tmm_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_tmm_s_arity( const bhvm_vop_ar2_mul_tmm_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_tmm_s_get_index_arr( bhvm_vop_ar2_mul_tmm_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_mul_tmm_s_sig( const bhvm_vop_ar2_mul_tmm_s* o ){return "aby";} \
  static inline void bhvm_vop_ar2_mul_tmm_s_run( const bhvm_vop_ar2_mul_tmm_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_tmm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_mul_ttm_s 1650871027
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_ttm_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_ttm_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_ttm_s_arity( const bhvm_vop_ar2_mul_ttm_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_ttm_s_get_index_arr( bhvm_vop_ar2_mul_ttm_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_mul_ttm_s_sig( const bhvm_vop_ar2_mul_ttm_s* o ){return "aby";} \
  static inline void bhvm_vop_ar2_mul_ttm_s_run( const bhvm_vop_ar2_mul_ttm_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_ttm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_equal_s 1566585749
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_equal_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_equal_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_equal_s_arity( const bhvm_vop_ar2_equal_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_equal_s_get_index_arr( bhvm_vop_ar2_equal_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_equal_s_sig( const bhvm_vop_ar2_equal_s* o ){return "aby";} \
  static inline void bhvm_vop_ar2_equal_s_run( const bhvm_vop_ar2_equal_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_equal_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_larger_s 2097285264
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_larger_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_larger_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_larger_s_arity( const bhvm_vop_ar2_larger_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_larger_s_get_index_arr( bhvm_vop_ar2_larger_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_larger_s_sig( const bhvm_vop_ar2_larger_s* o ){return "aby";} \
  static inline void bhvm_vop_ar2_larger_s_run( const bhvm_vop_ar2_larger_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_larger_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_smaller_s 2414206337
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_smaller_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_smaller_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_smaller_s_arity( const bhvm_vop_ar2_smaller_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_smaller_s_get_index_arr( bhvm_vop_ar2_smaller_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_smaller_s_sig( const bhvm_vop_ar2_smaller_s* o ){return "aby";} \
  static inline void bhvm_vop_ar2_smaller_s_run( const bhvm_vop_ar2_smaller_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_smaller_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_larger_equal_s 1646128483
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_larger_equal_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_larger_equal_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_larger_equal_s_arity( const bhvm_vop_ar2_larger_equal_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_larger_equal_s_get_index_arr( bhvm_vop_ar2_larger_equal_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_larger_equal_s_sig( const bhvm_vop_ar2_larger_equal_s* o ){return "aby";} \
  static inline void bhvm_vop_ar2_larger_equal_s_run( const bhvm_vop_ar2_larger_equal_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_larger_equal_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_smaller_equal_s 1134314202
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_smaller_equal_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_smaller_equal_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_smaller_equal_s_arity( const bhvm_vop_ar2_smaller_equal_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_smaller_equal_s_get_index_arr( bhvm_vop_ar2_smaller_equal_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_smaller_equal_s_sig( const bhvm_vop_ar2_smaller_equal_s* o ){return "aby";} \
  static inline void bhvm_vop_ar2_smaller_equal_s_run( const bhvm_vop_ar2_smaller_equal_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_smaller_equal_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_logic_and_s 2842280081
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_logic_and_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_logic_and_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_logic_and_s_arity( const bhvm_vop_ar2_logic_and_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_logic_and_s_get_index_arr( bhvm_vop_ar2_logic_and_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_logic_and_s_sig( const bhvm_vop_ar2_logic_and_s* o ){return "aby";} \
  static inline void bhvm_vop_ar2_logic_and_s_run( const bhvm_vop_ar2_logic_and_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_logic_and_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_logic_or_s 1289690677
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_logic_or_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_logic_or_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_logic_or_s_arity( const bhvm_vop_ar2_logic_or_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_logic_or_s_get_index_arr( bhvm_vop_ar2_logic_or_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_logic_or_s_sig( const bhvm_vop_ar2_logic_or_s* o ){return "aby";} \
  static inline void bhvm_vop_ar2_logic_or_s_run( const bhvm_vop_ar2_logic_or_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_logic_or_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_mul_vvv_dp_a_s 3190592231
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vvv_dp_a_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_vvv_dp_a_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_vvv_dp_a_s_arity( const bhvm_vop_ar2_mul_vvv_dp_a_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_vvv_dp_a_s_get_index_arr( bhvm_vop_ar2_mul_vvv_dp_a_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_vvv_dp_a_s_run( const bhvm_vop_ar2_mul_vvv_dp_a_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_acc_vvv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_vvv_dp_a_s_sig( const bhvm_vop_ar2_mul_vvv_dp_a_s* o ){return "zbf";}
#define TYPEOF_bhvm_vop_ar2_mul_vsv_dp_a_s 3105202820
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vsv_dp_a_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_vsv_dp_a_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_vsv_dp_a_s_arity( const bhvm_vop_ar2_mul_vsv_dp_a_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_vsv_dp_a_s_get_index_arr( bhvm_vop_ar2_mul_vsv_dp_a_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_vsv_dp_a_s_run( const bhvm_vop_ar2_mul_vsv_dp_a_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_acc_vsv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_vsv_dp_a_s_sig( const bhvm_vop_ar2_mul_vsv_dp_a_s* o ){return "zbf";}
#define TYPEOF_bhvm_vop_ar2_mul_svv_dp_a_s 2122618618
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_svv_dp_a_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_svv_dp_a_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_svv_dp_a_s_arity( const bhvm_vop_ar2_mul_svv_dp_a_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_svv_dp_a_s_get_index_arr( bhvm_vop_ar2_mul_svv_dp_a_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_svv_dp_a_s_run( const bhvm_vop_ar2_mul_svv_dp_a_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_acc_vvs_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_svv_dp_a_s_sig( const bhvm_vop_ar2_mul_svv_dp_a_s* o ){return "zbf";}
#define TYPEOF_bhvm_vop_ar2_mul_vvs_dp_a_s 472241214
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vvs_dp_a_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_vvs_dp_a_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_vvs_dp_a_s_arity( const bhvm_vop_ar2_mul_vvs_dp_a_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_vvs_dp_a_s_get_index_arr( bhvm_vop_ar2_mul_vvs_dp_a_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_vvs_dp_a_s_run( const bhvm_vop_ar2_mul_vvs_dp_a_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_acc_svv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_vvs_dp_a_s_sig( const bhvm_vop_ar2_mul_vvs_dp_a_s* o ){return "zbf";}
#define TYPEOF_bhvm_vop_ar2_mul_mvv_dp_a_s 1021985144
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_mvv_dp_a_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_mvv_dp_a_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_mvv_dp_a_s_arity( const bhvm_vop_ar2_mul_mvv_dp_a_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_mvv_dp_a_s_get_index_arr( bhvm_vop_ar2_mul_mvv_dp_a_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_mvv_dp_a_s_run( const bhvm_vop_ar2_mul_mvv_dp_a_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_acc_vvm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_mvv_dp_a_s_sig( const bhvm_vop_ar2_mul_mvv_dp_a_s* o ){return "zbf";}
#define TYPEOF_bhvm_vop_ar2_mul_vmv_dp_a_s 1328027294
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vmv_dp_a_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_vmv_dp_a_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_vmv_dp_a_s_arity( const bhvm_vop_ar2_mul_vmv_dp_a_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_vmv_dp_a_s_get_index_arr( bhvm_vop_ar2_mul_vmv_dp_a_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_vmv_dp_a_s_run( const bhvm_vop_ar2_mul_vmv_dp_a_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_acc_vtv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_vmv_dp_a_s_sig( const bhvm_vop_ar2_mul_vmv_dp_a_s* o ){return "zbf";}
#define TYPEOF_bhvm_vop_ar2_mul_tvv_dp_a_s 877041689
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_tvv_dp_a_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_tvv_dp_a_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_tvv_dp_a_s_arity( const bhvm_vop_ar2_mul_tvv_dp_a_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_tvv_dp_a_s_get_index_arr( bhvm_vop_ar2_mul_tvv_dp_a_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_tvv_dp_a_s_run( const bhvm_vop_ar2_mul_tvv_dp_a_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_acc_vvm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_tvv_dp_a_s_sig( const bhvm_vop_ar2_mul_tvv_dp_a_s* o ){return "bzf";}
#define TYPEOF_bhvm_vop_ar2_mul_vtv_dp_a_s 3506629981
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vtv_dp_a_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_vtv_dp_a_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_vtv_dp_a_s_arity( const bhvm_vop_ar2_mul_vtv_dp_a_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_vtv_dp_a_s_get_index_arr( bhvm_vop_ar2_mul_vtv_dp_a_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_vtv_dp_a_s_run( const bhvm_vop_ar2_mul_vtv_dp_a_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_acc_vmv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_vtv_dp_a_s_sig( const bhvm_vop_ar2_mul_vtv_dp_a_s* o ){return "zbf";}
#define TYPEOF_bhvm_vop_ar2_mul_vvm_dp_a_s 793119460
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vvm_dp_a_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_vvm_dp_a_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_vvm_dp_a_s_arity( const bhvm_vop_ar2_mul_vvm_dp_a_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_vvm_dp_a_s_get_index_arr( bhvm_vop_ar2_mul_vvm_dp_a_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_vvm_dp_a_s_run( const bhvm_vop_ar2_mul_vvm_dp_a_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_acc_mvv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_vvm_dp_a_s_sig( const bhvm_vop_ar2_mul_vvm_dp_a_s* o ){return "zbf";}
#define TYPEOF_bhvm_vop_ar2_mul_vvv_dp_b_s 1847053422
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vvv_dp_b_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_vvv_dp_b_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_vvv_dp_b_s_arity( const bhvm_vop_ar2_mul_vvv_dp_b_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_vvv_dp_b_s_get_index_arr( bhvm_vop_ar2_mul_vvv_dp_b_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_vvv_dp_b_s_run( const bhvm_vop_ar2_mul_vvv_dp_b_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_acc_vvv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_vvv_dp_b_s_sig( const bhvm_vop_ar2_mul_vvv_dp_b_s* o ){return "zag";}
#define TYPEOF_bhvm_vop_ar2_mul_vsv_dp_b_s 1150113441
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vsv_dp_b_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_vsv_dp_b_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_vsv_dp_b_s_arity( const bhvm_vop_ar2_mul_vsv_dp_b_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_vsv_dp_b_s_get_index_arr( bhvm_vop_ar2_mul_vsv_dp_b_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_vsv_dp_b_s_run( const bhvm_vop_ar2_mul_vsv_dp_b_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_acc_vvs_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_vsv_dp_b_s_sig( const bhvm_vop_ar2_mul_vsv_dp_b_s* o ){return "azg";}
#define TYPEOF_bhvm_vop_ar2_mul_svv_dp_b_s 2918827859
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_svv_dp_b_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_svv_dp_b_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_svv_dp_b_s_arity( const bhvm_vop_ar2_mul_svv_dp_b_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_svv_dp_b_s_get_index_arr( bhvm_vop_ar2_mul_svv_dp_b_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_svv_dp_b_s_run( const bhvm_vop_ar2_mul_svv_dp_b_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_acc_vsv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_svv_dp_b_s_sig( const bhvm_vop_ar2_mul_svv_dp_b_s* o ){return "zag";}
#define TYPEOF_bhvm_vop_ar2_mul_vvs_dp_b_s 1774203319
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vvs_dp_b_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_vvs_dp_b_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_vvs_dp_b_s_arity( const bhvm_vop_ar2_mul_vvs_dp_b_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_vvs_dp_b_s_get_index_arr( bhvm_vop_ar2_mul_vvs_dp_b_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_vvs_dp_b_s_run( const bhvm_vop_ar2_mul_vvs_dp_b_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_acc_svv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_vvs_dp_b_s_sig( const bhvm_vop_ar2_mul_vvs_dp_b_s* o ){return "zag";}
#define TYPEOF_bhvm_vop_ar2_mul_mvv_dp_b_s 3372308821
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_mvv_dp_b_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_mvv_dp_b_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_mvv_dp_b_s_arity( const bhvm_vop_ar2_mul_mvv_dp_b_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_mvv_dp_b_s_get_index_arr( bhvm_vop_ar2_mul_mvv_dp_b_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_mvv_dp_b_s_run( const bhvm_vop_ar2_mul_mvv_dp_b_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_acc_tvv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_mvv_dp_b_s_sig( const bhvm_vop_ar2_mul_mvv_dp_b_s* o ){return "azg";}
#define TYPEOF_bhvm_vop_ar2_mul_vmv_dp_b_s 2629989399
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vmv_dp_b_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_vmv_dp_b_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_vmv_dp_b_s_arity( const bhvm_vop_ar2_mul_vmv_dp_b_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_vmv_dp_b_s_get_index_arr( bhvm_vop_ar2_mul_vmv_dp_b_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_vmv_dp_b_s_run( const bhvm_vop_ar2_mul_vmv_dp_b_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_acc_vvm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_vmv_dp_b_s_sig( const bhvm_vop_ar2_mul_vmv_dp_b_s* o ){return "azg";}
#define TYPEOF_bhvm_vop_ar2_mul_tvv_dp_b_s 2821788476
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_tvv_dp_b_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_tvv_dp_b_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_tvv_dp_b_s_arity( const bhvm_vop_ar2_mul_tvv_dp_b_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_tvv_dp_b_s_get_index_arr( bhvm_vop_ar2_mul_tvv_dp_b_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_tvv_dp_b_s_run( const bhvm_vop_ar2_mul_tvv_dp_b_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_acc_mvv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_tvv_dp_b_s_sig( const bhvm_vop_ar2_mul_tvv_dp_b_s* o ){return "azg";}
#define TYPEOF_bhvm_vop_ar2_mul_vtv_dp_b_s 629868256
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vtv_dp_b_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_vtv_dp_b_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_vtv_dp_b_s_arity( const bhvm_vop_ar2_mul_vtv_dp_b_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_vtv_dp_b_s_get_index_arr( bhvm_vop_ar2_mul_vtv_dp_b_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_vtv_dp_b_s_run( const bhvm_vop_ar2_mul_vtv_dp_b_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_acc_vvm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_vtv_dp_b_s_sig( const bhvm_vop_ar2_mul_vtv_dp_b_s* o ){return "zag";}
#define TYPEOF_bhvm_vop_ar2_mul_vvm_dp_b_s 3132997377
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vvm_dp_b_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_vvm_dp_b_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_vvm_dp_b_s_arity( const bhvm_vop_ar2_mul_vvm_dp_b_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_vvm_dp_b_s_get_index_arr( bhvm_vop_ar2_mul_vvm_dp_b_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_vvm_dp_b_s_run( const bhvm_vop_ar2_mul_vvm_dp_b_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_acc_vmv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_vvm_dp_b_s_sig( const bhvm_vop_ar2_mul_vvm_dp_b_s* o ){return "azg";}
#define TYPEOF_bhvm_vop_ar2_mul_mmm_dp_a_s 1222515222
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_mmm_dp_a_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_mmm_dp_a_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_mmm_dp_a_s_arity( const bhvm_vop_ar2_mul_mmm_dp_a_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_mmm_dp_a_s_get_index_arr( bhvm_vop_ar2_mul_mmm_dp_a_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_mmm_dp_a_s_run( const bhvm_vop_ar2_mul_mmm_dp_a_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_acc_mtm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_mmm_dp_a_s_sig( const bhvm_vop_ar2_mul_mmm_dp_a_s* o ){return "zbf";}
#define TYPEOF_bhvm_vop_ar2_mul_mtm_dp_a_s 487219333
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_mtm_dp_a_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_mtm_dp_a_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_mtm_dp_a_s_arity( const bhvm_vop_ar2_mul_mtm_dp_a_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_mtm_dp_a_s_get_index_arr( bhvm_vop_ar2_mul_mtm_dp_a_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_mtm_dp_a_s_run( const bhvm_vop_ar2_mul_mtm_dp_a_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_acc_mmm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_mtm_dp_a_s_sig( const bhvm_vop_ar2_mul_mtm_dp_a_s* o ){return "zbf";}
#define TYPEOF_bhvm_vop_ar2_mul_tmm_dp_a_s 3000466623
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_tmm_dp_a_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_tmm_dp_a_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_tmm_dp_a_s_arity( const bhvm_vop_ar2_mul_tmm_dp_a_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_tmm_dp_a_s_get_index_arr( bhvm_vop_ar2_mul_tmm_dp_a_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_tmm_dp_a_s_run( const bhvm_vop_ar2_mul_tmm_dp_a_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_acc_mtm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_tmm_dp_a_s_sig( const bhvm_vop_ar2_mul_tmm_dp_a_s* o ){return "bzf";}
#define TYPEOF_bhvm_vop_ar2_mul_ttm_dp_a_s 4259170976
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_ttm_dp_a_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_ttm_dp_a_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_ttm_dp_a_s_arity( const bhvm_vop_ar2_mul_ttm_dp_a_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_ttm_dp_a_s_get_index_arr( bhvm_vop_ar2_mul_ttm_dp_a_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_ttm_dp_a_s_run( const bhvm_vop_ar2_mul_ttm_dp_a_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_acc_ttm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_ttm_dp_a_s_sig( const bhvm_vop_ar2_mul_ttm_dp_a_s* o ){return "bzf";}
#define TYPEOF_bhvm_vop_ar2_mul_mmm_dp_b_s 2514031567
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_mmm_dp_b_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_mmm_dp_b_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_mmm_dp_b_s_arity( const bhvm_vop_ar2_mul_mmm_dp_b_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_mmm_dp_b_s_get_index_arr( bhvm_vop_ar2_mul_mmm_dp_b_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_mmm_dp_b_s_run( const bhvm_vop_ar2_mul_mmm_dp_b_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_acc_tmm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_mmm_dp_b_s_sig( const bhvm_vop_ar2_mul_mmm_dp_b_s* o ){return "azg";}
#define TYPEOF_bhvm_vop_ar2_mul_mtm_dp_b_s 2442411880
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_mtm_dp_b_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_mtm_dp_b_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_mtm_dp_b_s_arity( const bhvm_vop_ar2_mul_mtm_dp_b_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_mtm_dp_b_s_get_index_arr( bhvm_vop_ar2_mul_mtm_dp_b_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_mtm_dp_b_s_run( const bhvm_vop_ar2_mul_mtm_dp_b_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_acc_tmm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_mtm_dp_b_s_sig( const bhvm_vop_ar2_mul_mtm_dp_b_s* o ){return "zag";}
#define TYPEOF_bhvm_vop_ar2_mul_tmm_dp_b_s 1698607686
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_tmm_dp_b_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_tmm_dp_b_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_tmm_dp_b_s_arity( const bhvm_vop_ar2_mul_tmm_dp_b_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_tmm_dp_b_s_get_index_arr( bhvm_vop_ar2_mul_tmm_dp_b_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_tmm_dp_b_s_run( const bhvm_vop_ar2_mul_tmm_dp_b_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_acc_mmm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_tmm_dp_b_s_sig( const bhvm_vop_ar2_mul_tmm_dp_b_s* o ){return "azg";}
#define TYPEOF_bhvm_vop_ar2_mul_ttm_dp_b_s 2840862237
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_ttm_dp_b_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_ttm_dp_b_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_ttm_dp_b_s_arity( const bhvm_vop_ar2_mul_ttm_dp_b_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_ttm_dp_b_s_get_index_arr( bhvm_vop_ar2_mul_ttm_dp_b_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_ttm_dp_b_s_run( const bhvm_vop_ar2_mul_ttm_dp_b_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_mul_acc_ttm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_ttm_dp_b_s_sig( const bhvm_vop_ar2_mul_ttm_dp_b_s* o ){return "zag";}
#define TYPEOF_bhvm_vop_ar2_div_dp_a_s 1030641815
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_div_dp_a_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_div_dp_a_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_div_dp_a_s_arity( const bhvm_vop_ar2_div_dp_a_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_div_dp_a_s_get_index_arr( bhvm_vop_ar2_div_dp_a_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_div_dp_a_s_run( const bhvm_vop_ar2_div_dp_a_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_eci_div_s_f(    &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_div_dp_a_s_sig( const bhvm_vop_ar2_div_dp_a_s* o ){return "zbf";}
#define TYPEOF_bhvm_vop_ar2_exp_dp_s 2147217239
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_exp_dp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_exp_dp_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_exp_dp_s_arity( const bhvm_vop_ar2_exp_dp_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_exp_dp_s_get_index_arr( bhvm_vop_ar2_exp_dp_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_exp_dp_s_run( const bhvm_vop_ar2_exp_dp_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_exp_dp_zyf_s_f(        &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_exp_dp_s_sig( const bhvm_vop_ar2_exp_dp_s* o ){return "zyf";}
#define TYPEOF_bhvm_vop_ar2_log_dp_s 2569678196
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_log_dp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_log_dp_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_log_dp_s_arity( const bhvm_vop_ar2_log_dp_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_log_dp_s_get_index_arr( bhvm_vop_ar2_log_dp_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_log_dp_s_run( const bhvm_vop_ar2_log_dp_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_log_dp_zaf_s_f(        &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_log_dp_s_sig( const bhvm_vop_ar2_log_dp_s* o ){return "zaf";}
#define TYPEOF_bhvm_vop_ar2_inv_dp_s 2950633583
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_inv_dp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_inv_dp_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_inv_dp_s_arity( const bhvm_vop_ar2_inv_dp_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_inv_dp_s_get_index_arr( bhvm_vop_ar2_inv_dp_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_inv_dp_s_run( const bhvm_vop_ar2_inv_dp_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_inv_dp_zyf_s_f(        &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_inv_dp_s_sig( const bhvm_vop_ar2_inv_dp_s* o ){return "zyf";}
#define TYPEOF_bhvm_vop_ar2_sqr_dp_s 1818950460
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_sqr_dp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_sqr_dp_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_sqr_dp_s_arity( const bhvm_vop_ar2_sqr_dp_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_sqr_dp_s_get_index_arr( bhvm_vop_ar2_sqr_dp_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_sqr_dp_s_run( const bhvm_vop_ar2_sqr_dp_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_sqr_dp_zaf_s_f(        &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_sqr_dp_s_sig( const bhvm_vop_ar2_sqr_dp_s* o ){return "zaf";}
#define TYPEOF_bhvm_vop_ar2_srt_dp_s 2560073435
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_srt_dp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_srt_dp_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_srt_dp_s_arity( const bhvm_vop_ar2_srt_dp_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_srt_dp_s_get_index_arr( bhvm_vop_ar2_srt_dp_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_srt_dp_s_run( const bhvm_vop_ar2_srt_dp_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_srt_dp_zyf_s_f(        &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_srt_dp_s_sig( const bhvm_vop_ar2_srt_dp_s* o ){return "zyf";}
#define TYPEOF_bhvm_vop_ar2_lgst_dp_s 2656749734
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_lgst_dp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_lgst_dp_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_lgst_dp_s_arity( const bhvm_vop_ar2_lgst_dp_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_lgst_dp_s_get_index_arr( bhvm_vop_ar2_lgst_dp_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_lgst_dp_s_run( const bhvm_vop_ar2_lgst_dp_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_lgst_dp_zyf_s_f(       &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_lgst_dp_s_sig( const bhvm_vop_ar2_lgst_dp_s* o ){return "zyf";}
#define TYPEOF_bhvm_vop_ar2_lgst_hard_dp_s 1750505756
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_lgst_hard_dp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_lgst_hard_dp_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_lgst_hard_dp_s_arity( const bhvm_vop_ar2_lgst_hard_dp_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_lgst_hard_dp_s_get_index_arr( bhvm_vop_ar2_lgst_hard_dp_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_lgst_hard_dp_s_run( const bhvm_vop_ar2_lgst_hard_dp_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_lgst_hard_dp_zyf_s_f(  &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_lgst_hard_dp_s_sig( const bhvm_vop_ar2_lgst_hard_dp_s* o ){return "zyf";}
#define TYPEOF_bhvm_vop_ar2_lgst_leaky_dp_s 353391877
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_lgst_leaky_dp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_lgst_leaky_dp_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_lgst_leaky_dp_s_arity( const bhvm_vop_ar2_lgst_leaky_dp_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_lgst_leaky_dp_s_get_index_arr( bhvm_vop_ar2_lgst_leaky_dp_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_lgst_leaky_dp_s_run( const bhvm_vop_ar2_lgst_leaky_dp_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_lgst_leaky_dp_zyf_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_lgst_leaky_dp_s_sig( const bhvm_vop_ar2_lgst_leaky_dp_s* o ){return "zyf";}
#define TYPEOF_bhvm_vop_ar2_tanh_dp_s 3481712559
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_tanh_dp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_tanh_dp_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_tanh_dp_s_arity( const bhvm_vop_ar2_tanh_dp_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_tanh_dp_s_get_index_arr( bhvm_vop_ar2_tanh_dp_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_tanh_dp_s_run( const bhvm_vop_ar2_tanh_dp_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_tanh_dp_zyf_s_f(       &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_tanh_dp_s_sig( const bhvm_vop_ar2_tanh_dp_s* o ){return "zyf";}
#define TYPEOF_bhvm_vop_ar2_tanh_hard_dp_s 4221168715
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_tanh_hard_dp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_tanh_hard_dp_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_tanh_hard_dp_s_arity( const bhvm_vop_ar2_tanh_hard_dp_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_tanh_hard_dp_s_get_index_arr( bhvm_vop_ar2_tanh_hard_dp_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_tanh_hard_dp_s_run( const bhvm_vop_ar2_tanh_hard_dp_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_tanh_hard_dp_zyf_s_f(  &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_tanh_hard_dp_s_sig( const bhvm_vop_ar2_tanh_hard_dp_s* o ){return "zyf";}
#define TYPEOF_bhvm_vop_ar2_tanh_leaky_dp_s 816792840
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_tanh_leaky_dp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_tanh_leaky_dp_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_tanh_leaky_dp_s_arity( const bhvm_vop_ar2_tanh_leaky_dp_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_tanh_leaky_dp_s_get_index_arr( bhvm_vop_ar2_tanh_leaky_dp_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_tanh_leaky_dp_s_run( const bhvm_vop_ar2_tanh_leaky_dp_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_tanh_leaky_dp_zyf_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_tanh_leaky_dp_s_sig( const bhvm_vop_ar2_tanh_leaky_dp_s* o ){return "zyf";}
#define TYPEOF_bhvm_vop_ar2_softplus_dp_s 2672117248
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_softplus_dp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_softplus_dp_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_softplus_dp_s_arity( const bhvm_vop_ar2_softplus_dp_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_softplus_dp_s_get_index_arr( bhvm_vop_ar2_softplus_dp_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_softplus_dp_s_run( const bhvm_vop_ar2_softplus_dp_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_softplus_dp_zyf_s_f(   &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_softplus_dp_s_sig( const bhvm_vop_ar2_softplus_dp_s* o ){return "zyf";}
#define TYPEOF_bhvm_vop_ar2_relu_dp_s 414292006
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_relu_dp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_relu_dp_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_relu_dp_s_arity( const bhvm_vop_ar2_relu_dp_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_relu_dp_s_get_index_arr( bhvm_vop_ar2_relu_dp_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_relu_dp_s_run( const bhvm_vop_ar2_relu_dp_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_relu_dp_zyf_s_f(       &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_relu_dp_s_sig( const bhvm_vop_ar2_relu_dp_s* o ){return "zyf";}
#define TYPEOF_bhvm_vop_ar2_relu_leaky_dp_s 3140790405
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_relu_leaky_dp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_relu_leaky_dp_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_relu_leaky_dp_s_arity( const bhvm_vop_ar2_relu_leaky_dp_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_relu_leaky_dp_s_get_index_arr( bhvm_vop_ar2_relu_leaky_dp_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_relu_leaky_dp_s_run( const bhvm_vop_ar2_relu_leaky_dp_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_relu_leaky_dp_zyf_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_relu_leaky_dp_s_sig( const bhvm_vop_ar2_relu_leaky_dp_s* o ){return "zyf";}
#define TYPEOF_bhvm_vop_ar2_branch_vvvv_dp_b_s 2302479556
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_branch_vvvv_dp_b_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_branch_vvvv_dp_b_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_branch_vvvv_dp_b_s_arity( const bhvm_vop_ar2_branch_vvvv_dp_b_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_branch_vvvv_dp_b_s_get_index_arr( bhvm_vop_ar2_branch_vvvv_dp_b_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_branch_vvvv_dp_b_s_run( const bhvm_vop_ar2_branch_vvvv_dp_b_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_branch_vvvv_dp_azg_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_branch_vvvv_dp_b_s_sig( const bhvm_vop_ar2_branch_vvvv_dp_b_s* o ){return "azg";}
#define TYPEOF_bhvm_vop_ar2_branch_vvvv_dp_c_s 3865351839
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_branch_vvvv_dp_c_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_branch_vvvv_dp_c_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_branch_vvvv_dp_c_s_arity( const bhvm_vop_ar2_branch_vvvv_dp_c_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_branch_vvvv_dp_c_s_get_index_arr( bhvm_vop_ar2_branch_vvvv_dp_c_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_branch_vvvv_dp_c_s_run( const bhvm_vop_ar2_branch_vvvv_dp_c_s* o, bhvm_holor_s* ah ){bhvm_hop_ar2_branch_vvvv_dp_azh_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_branch_vvvv_dp_c_s_sig( const bhvm_vop_ar2_branch_vvvv_dp_c_s* o ){return "azh";}
#define BETH_EXPAND_GROUP_bhvm_vop_ar2 \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2 ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_index_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_cat_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_add_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_sub_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_div_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_vvv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_vsv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_svv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_vvs_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_mvv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_vmv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_tvv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_vtv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_vvm_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_mmm_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_mtm_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_tmm_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_ttm_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_equal_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_larger_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_smaller_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_larger_equal_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_smaller_equal_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_logic_and_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_logic_or_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_vvv_dp_a_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_vsv_dp_a_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_svv_dp_a_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_vvs_dp_a_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_mvv_dp_a_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_vmv_dp_a_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_tvv_dp_a_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_vtv_dp_a_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_vvm_dp_a_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_vvv_dp_b_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_vsv_dp_b_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_svv_dp_b_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_vvs_dp_b_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_mvv_dp_b_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_vmv_dp_b_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_tvv_dp_b_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_vtv_dp_b_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_vvm_dp_b_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_mmm_dp_a_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_mtm_dp_a_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_tmm_dp_a_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_ttm_dp_a_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_mmm_dp_b_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_mtm_dp_b_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_tmm_dp_b_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_ttm_dp_b_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_div_dp_a_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_exp_dp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_log_dp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_inv_dp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_sqr_dp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_srt_dp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_lgst_dp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_lgst_hard_dp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_lgst_leaky_dp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_tanh_dp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_tanh_hard_dp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_tanh_leaky_dp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_softplus_dp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_relu_dp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_relu_leaky_dp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_branch_vvvv_dp_b_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_branch_vvvv_dp_c_s ); \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_index_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_cat_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_add_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_sub_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_div_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vvv_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vsv_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_svv_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vvs_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_mvv_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vmv_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_tvv_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vtv_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vvm_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_mmm_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_mtm_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_tmm_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_ttm_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_equal_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_larger_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_smaller_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_larger_equal_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_smaller_equal_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_logic_and_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_logic_or_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vvv_dp_a_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vsv_dp_a_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_svv_dp_a_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vvs_dp_a_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_mvv_dp_a_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vmv_dp_a_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_tvv_dp_a_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vtv_dp_a_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vvm_dp_a_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vvv_dp_b_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vsv_dp_b_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_svv_dp_b_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vvs_dp_b_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_mvv_dp_b_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vmv_dp_b_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_tvv_dp_b_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vtv_dp_b_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vvm_dp_b_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_mmm_dp_a_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_mtm_dp_a_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_tmm_dp_a_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_ttm_dp_a_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_mmm_dp_b_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_mtm_dp_b_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_tmm_dp_b_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_ttm_dp_b_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_div_dp_a_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_exp_dp_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_log_dp_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_inv_dp_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_sqr_dp_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_srt_dp_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_lgst_dp_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_lgst_hard_dp_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_lgst_leaky_dp_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_tanh_dp_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_tanh_hard_dp_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_tanh_leaky_dp_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_softplus_dp_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_relu_dp_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_relu_leaky_dp_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_branch_vvvv_dp_b_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_branch_vvvv_dp_c_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_vop_ar3

#define TYPEOF_bhvm_vop_ar3 1807610607
#define TYPEOF_bhvm_vop_ar3_s 250670297
#define TYPEOF_bhvm_vop_ar3_index_s 878118510
#define BETH_EXPAND_ITEM_bhvm_vop_ar3_index_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar3_index_s ) \
    {sz_t v[ 4 ];};
#define TYPEOF_bhvm_vop_ar3_branch_vvvv_s 1240146799
#define BETH_EXPAND_ITEM_bhvm_vop_ar3_branch_vvvv_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar3_branch_vvvv_s ) \
    {aware_t _;bhvm_vop_ar3_index_s i;}; \
  static inline sz_t bhvm_vop_ar3_branch_vvvv_s_arity( const bhvm_vop_ar3_branch_vvvv_s* o ){return 3;} \
  static inline sz_t* bhvm_vop_ar3_branch_vvvv_s_get_index_arr( bhvm_vop_ar3_branch_vvvv_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar3_branch_vvvv_s_run( const bhvm_vop_ar3_branch_vvvv_s* o, bhvm_holor_s* ah ){bhvm_hop_ar3_branch_vvvv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]], &ah[o->i.v[3]] );} \
  static inline sc_t bhvm_vop_ar3_branch_vvvv_s_sig( const bhvm_vop_ar3_branch_vvvv_s* o ){return "abcy";}
#define TYPEOF_bhvm_vop_ar3_branch_svvv_s 569089082
#define BETH_EXPAND_ITEM_bhvm_vop_ar3_branch_svvv_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar3_branch_svvv_s ) \
    {aware_t _;bhvm_vop_ar3_index_s i;}; \
  static inline sz_t bhvm_vop_ar3_branch_svvv_s_arity( const bhvm_vop_ar3_branch_svvv_s* o ){return 3;} \
  static inline sz_t* bhvm_vop_ar3_branch_svvv_s_get_index_arr( bhvm_vop_ar3_branch_svvv_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar3_branch_svvv_s_run( const bhvm_vop_ar3_branch_svvv_s* o, bhvm_holor_s* ah ){bhvm_hop_ar3_branch_svvv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]], &ah[o->i.v[3]] );} \
  static inline sc_t bhvm_vop_ar3_branch_svvv_s_sig( const bhvm_vop_ar3_branch_svvv_s* o ){return "abcy";}
#define TYPEOF_bhvm_vop_ar3_div_dp_b_s 2060282055
#define BETH_EXPAND_ITEM_bhvm_vop_ar3_div_dp_b_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar3_div_dp_b_s ) \
    {aware_t _;bhvm_vop_ar3_index_s i;}; \
  static inline sz_t bhvm_vop_ar3_div_dp_b_s_arity( const bhvm_vop_ar3_div_dp_b_s* o ){return 3;} \
  static inline sz_t* bhvm_vop_ar3_div_dp_b_s_get_index_arr( bhvm_vop_ar3_div_dp_b_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar3_div_dp_b_s_run( const bhvm_vop_ar3_div_dp_b_s* o, bhvm_holor_s* ah ){bhvm_hop_ar3_eci_div_dp_zabg_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]], &ah[o->i.v[3]] );} \
  static inline sc_t bhvm_vop_ar3_div_dp_b_s_sig( const bhvm_vop_ar3_div_dp_b_s* o ){return "zabg";}
#define BETH_EXPAND_GROUP_bhvm_vop_ar3 \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar3 ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar3_index_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar3_branch_vvvv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar3_branch_svvv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar3_div_dp_b_s ); \
  BETH_EXPAND_ITEM_bhvm_vop_ar3_index_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar3_branch_vvvv_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar3_branch_svvv_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar3_div_dp_b_s

/**********************************************************************************************************************/

vd_t bhvm_planted_vop_signal_handler( const bcore_signal_s* o );

#endif // BHVM_PLANTED_VOP_H
