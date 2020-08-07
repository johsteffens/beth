/** This file was generated from beth-plant source code.
 *  Compiling Agent : xoico_compiler (C) 2020 J.B.Steffens
 *  Last File Update: 2020-08-07T21:18:46Z
 *
 *  Copyright and License of this File:
 *
 *  Generated code inherits the copyright and license of the underlying beth-plant source code.
 *  Source code defining this file is distributed across following files:
 *
 *  bhvm_vop.h
 *
 */

#ifndef BHVM_VOP_XOILA_OUT_H
#define BHVM_VOP_XOILA_OUT_H

#include "bcore_control.h"

//To force a rebuild of this target by the plant-compiler, reset the hash key value below to 0.
#define HKEYOF_bhvm_vop_xoila_out 2514568740716378661

#define TYPEOF_bhvm_vop_xoila_out 0x83562D9EA3A913DFull

/**********************************************************************************************************************/
// source: bhvm_vop.h

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_vop

#define TYPEOF_bhvm_vop 0x97415E6D4143BC14ull
#define TYPEOF_bhvm_vop_s 0x1EF3F5E5FC64EC66ull
#define TYPEOF_bhvm_vop_arr_s 0x43EB7A0714225C3Eull
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
#define TYPEOF_bhvm_vop_ci_s 0x7EB8C65607DA851Full
#define BETH_EXPAND_ITEM_bhvm_vop_ci_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ci_s ) \
    {u0_t c;sz_t i;}; \
  static inline void bhvm_vop_ci_s_push_ci( bhvm_vop_ci_s* o, u0_t c, sz_t i ){o->c = c; o->i = i;}
#define TYPEOF_bhvm_vop_arr_ci_s 0x143997AA27F40867ull
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
  typedef void (*bhvm_vop_run)( const bhvm_vop* o, bhvm_holor_s** ah ); \
  typedef sz_t (*bhvm_vop_arity)( const bhvm_vop* o ); \
  typedef sc_t (*bhvm_vop_sig)( const bhvm_vop* o ); \
  typedef sz_t* (*bhvm_vop_get_index_arr)( bhvm_vop* o ); \
  typedef sz_t (*bhvm_vop_get_index)( const bhvm_vop* o, sz_t index ); \
  typedef bhvm_vop* (*bhvm_vop_set_index)( bhvm_vop* o, sz_t index, sz_t idx_val ); \
  typedef bhvm_vop* (*bhvm_vop_set_index_arr)( bhvm_vop* o, sz_t* idx_arr, sz_t size ); \
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
      bhvm_vop_set_index_arr set_index_arr; \
      bhvm_vop_set_arg set_arg; \
      bhvm_vop_set_args set_args; \
  }; \
  static inline bhvm_vop* bhvm_vop_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_bhvm_vop, t ); return ( bhvm_vop* )bcore_inst_t_create( t ); } \
  static inline bl_t bhvm_vop_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_bhvm_vop ); } \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bhvm_vop ) \
  static inline bl_t bhvm_vop_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_bhvm_vop ); } \
  BETH_EXPAND_ITEM_bhvm_vop_arr_s \
  static inline void bhvm_vop_p_run( const bhvm_vop_s* __p, const bhvm_vop* o, bhvm_holor_s** ah ) { assert( __p->run ); __p->run( o, ah ); } \
  static inline void bhvm_vop_a_run( const bhvm_vop* o, bhvm_holor_s** ah ) { const bhvm_vop_s* p = bhvm_vop_s_get_aware( o ); assert( p->run ); p->run( o, ah ); } \
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
  static inline bhvm_vop* bhvm_vop_a_set_index_arr( bhvm_vop* o, sz_t* idx_arr, sz_t size ) { const bhvm_vop_s* p = bhvm_vop_s_get_aware( o ); assert( p->set_index_arr ); return p->set_index_arr( o, idx_arr, size ); } \
  static inline bl_t bhvm_vop_a_defines_set_index_arr( const bhvm_vop* o ) { return true; } \
  bhvm_vop* bhvm_vop_set_index_arr__( bhvm_vop* o, sz_t* idx_arr, sz_t size ); \
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

#define TYPEOF_bhvm_vop_ar0 0xA8CDC36D07781962ull
#define TYPEOF_bhvm_vop_ar0_s 0x29DD8405E3176E3Cull
#define TYPEOF_bhvm_vop_ar0_index_s 0xB3C6D1216DB43C3Full
#define BETH_EXPAND_ITEM_bhvm_vop_ar0_index_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar0_index_s ) \
    {sz_t v[ 1 ];}; \
  static inline void bhvm_vop_ar0_index_s_init_x( bhvm_vop_ar0_index_s* o ){o->v[0] = -1;}
#define TYPEOF_bhvm_vop_ar0_zro_s 0x0CA05CEF5B90FA1Aull
#define BETH_EXPAND_ITEM_bhvm_vop_ar0_zro_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar0_zro_s ) \
    {aware_t _;bhvm_vop_ar0_index_s i;}; \
  static inline sz_t bhvm_vop_ar0_zro_s_arity( const bhvm_vop_ar0_zro_s* o ){return 0;} \
  static inline sz_t* bhvm_vop_ar0_zro_s_get_index_arr( bhvm_vop_ar0_zro_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar0_zro_s_run( const bhvm_vop_ar0_zro_s* o, bhvm_holor_s** ah ){bhvm_hop_ar0_zro_s_f( ah[o->i.v[0]] );} \
  static inline sc_t bhvm_vop_ar0_zro_s_sig( const bhvm_vop_ar0_zro_s* o ){return "y";}
#define TYPEOF_bhvm_vop_ar0_one_s 0xB26253C2F7D5C407ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar0_one_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar0_one_s ) \
    {aware_t _;bhvm_vop_ar0_index_s i;}; \
  static inline sz_t bhvm_vop_ar0_one_s_arity( const bhvm_vop_ar0_one_s* o ){return 0;} \
  static inline sz_t* bhvm_vop_ar0_one_s_get_index_arr( bhvm_vop_ar0_one_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar0_one_s_run( const bhvm_vop_ar0_one_s* o, bhvm_holor_s** ah ){bhvm_hop_ar0_one_s_f( ah[o->i.v[0]] );} \
  static inline sc_t bhvm_vop_ar0_one_s_sig( const bhvm_vop_ar0_one_s* o ){return "y";}
#define TYPEOF_bhvm_vop_ar0_determine_s 0x8088E4A55449A9BEull
#define BETH_EXPAND_ITEM_bhvm_vop_ar0_determine_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar0_determine_s ) \
    {aware_t _;bhvm_vop_ar0_index_s i;}; \
  static inline sz_t bhvm_vop_ar0_determine_s_arity( const bhvm_vop_ar0_determine_s* o ){return 0;} \
  static inline sz_t* bhvm_vop_ar0_determine_s_get_index_arr( bhvm_vop_ar0_determine_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar0_determine_s_run( const bhvm_vop_ar0_determine_s* o, bhvm_holor_s** ah ){bhvm_holor_s_fit_size( ah[o->i.v[0]] );} \
  static inline sc_t bhvm_vop_ar0_determine_s_sig( const bhvm_vop_ar0_determine_s* o ){return "y";}
#define TYPEOF_bhvm_vop_ar0_vacate_s 0x0EB2C578C5CF6BEFull
#define BETH_EXPAND_ITEM_bhvm_vop_ar0_vacate_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar0_vacate_s ) \
    {aware_t _;bhvm_vop_ar0_index_s i;}; \
  static inline sz_t bhvm_vop_ar0_vacate_s_arity( const bhvm_vop_ar0_vacate_s* o ){return 0;} \
  static inline sz_t* bhvm_vop_ar0_vacate_s_get_index_arr( bhvm_vop_ar0_vacate_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar0_vacate_s_run( const bhvm_vop_ar0_vacate_s* o, bhvm_holor_s** ah ){bhvm_value_s_clear( &ah[o->i.v[0]]->v );} \
  static inline sc_t bhvm_vop_ar0_vacate_s_sig( const bhvm_vop_ar0_vacate_s* o ){return "y";}
#define TYPEOF_bhvm_vop_ar0_randomize_s 0x6EBB9DFFA0FBF3D4ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar0_randomize_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar0_randomize_s ) \
    {aware_t _;bhvm_vop_ar0_index_s i;u3_t rseed;f3_t min;f3_t max;f3_t density;}; \
  static inline sz_t bhvm_vop_ar0_randomize_s_arity( const bhvm_vop_ar0_randomize_s* o ){return 0;} \
  static inline sz_t* bhvm_vop_ar0_randomize_s_get_index_arr( bhvm_vop_ar0_randomize_s* o ){return o->i.v;} \
  void bhvm_vop_ar0_randomize_s_run( const bhvm_vop_ar0_randomize_s* o, bhvm_holor_s** ah ); \
  static inline sc_t bhvm_vop_ar0_randomize_s_sig( const bhvm_vop_ar0_randomize_s* o ){return "y";}
#define TYPEOF_bhvm_vop_ar0_nul_dp_s 0x7D74FC01F78CB1BDull
#define BETH_EXPAND_ITEM_bhvm_vop_ar0_nul_dp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar0_nul_dp_s ) \
    {aware_t _;bhvm_vop_ar0_index_s i;}; \
  static inline sz_t bhvm_vop_ar0_nul_dp_s_arity( const bhvm_vop_ar0_nul_dp_s* o ){return 0;} \
  static inline sz_t* bhvm_vop_ar0_nul_dp_s_get_index_arr( bhvm_vop_ar0_nul_dp_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar0_nul_dp_s_run( const bhvm_vop_ar0_nul_dp_s* o, bhvm_holor_s** ah ){} \
  static inline sc_t bhvm_vop_ar0_nul_dp_s_sig( const bhvm_vop_ar0_nul_dp_s* o ){return "f";}
#define TYPEOF_bhvm_vop_ar0_rand_s 0x8CF9F412058E66E2ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar0_rand_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar0_rand_s ) \
    {aware_t _;bhvm_vop_ar0_index_s i;bcore_prsg* prsg;u3_t rval;f3_t min;f3_t max;f3_t density;bcore_mutex_s mutex;}; \
  static inline sz_t bhvm_vop_ar0_rand_s_arity( const bhvm_vop_ar0_rand_s* o ){return 0;} \
  static inline sz_t* bhvm_vop_ar0_rand_s_get_index_arr( bhvm_vop_ar0_rand_s* o ){return o->i.v;} \
  void bhvm_vop_ar0_rand_s_run( const bhvm_vop_ar0_rand_s* o, bhvm_holor_s** ah ); \
  static inline sc_t bhvm_vop_ar0_rand_s_sig( const bhvm_vop_ar0_rand_s* o ){return "y";}
#define BETH_EXPAND_GROUP_bhvm_vop_ar0 \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar0 ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar0_index_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar0_zro_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar0_one_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar0_determine_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar0_vacate_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar0_randomize_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar0_nul_dp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar0_rand_s ); \
  BETH_EXPAND_ITEM_bhvm_vop_ar0_index_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar0_zro_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar0_one_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar0_determine_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar0_vacate_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar0_randomize_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar0_nul_dp_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar0_rand_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_vop_ar1

#define TYPEOF_bhvm_vop_ar1 0xA8CDC46D07781B15ull
#define TYPEOF_bhvm_vop_ar1_s 0x33B8BD05E9031657ull
#define TYPEOF_bhvm_vop_ar1_index_s 0xB591D080B469689Cull
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_index_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_index_s ) \
    {sz_t v[ 2 ];}; \
  static inline void bhvm_vop_ar1_index_s_init_x( bhvm_vop_ar1_index_s* o ){o->v[0] = o->v[1] = -1;}
#define TYPEOF_bhvm_vop_ar1_neg_s 0x696FC5DD65BC3B62ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_neg_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_neg_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_neg_s_arity( const bhvm_vop_ar1_neg_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_neg_s_get_index_arr( bhvm_vop_ar1_neg_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_neg_s_sig( const bhvm_vop_ar1_neg_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_neg_s_run( const bhvm_vop_ar1_neg_s* o, bhvm_holor_s** ah ){bhvm_hop_ar1_neg_s_f( ah[o->i.v[0]], ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_floor_s 0xC6CD61F8E5C170CAull
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_floor_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_floor_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_floor_s_arity( const bhvm_vop_ar1_floor_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_floor_s_get_index_arr( bhvm_vop_ar1_floor_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_floor_s_sig( const bhvm_vop_ar1_floor_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_floor_s_run( const bhvm_vop_ar1_floor_s* o, bhvm_holor_s** ah ){bhvm_hop_ar1_floor_s_f( ah[o->i.v[0]], ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_ceil_s 0xB2EDFC9D26CC72C5ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_ceil_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_ceil_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_ceil_s_arity( const bhvm_vop_ar1_ceil_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_ceil_s_get_index_arr( bhvm_vop_ar1_ceil_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_ceil_s_sig( const bhvm_vop_ar1_ceil_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_ceil_s_run( const bhvm_vop_ar1_ceil_s* o, bhvm_holor_s** ah ){bhvm_hop_ar1_ceil_s_f( ah[o->i.v[0]], ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_exp_s 0x841F107A1D02633Bull
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_exp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_exp_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_exp_s_arity( const bhvm_vop_ar1_exp_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_exp_s_get_index_arr( bhvm_vop_ar1_exp_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_exp_s_sig( const bhvm_vop_ar1_exp_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_exp_s_run( const bhvm_vop_ar1_exp_s* o, bhvm_holor_s** ah ){bhvm_hop_ar1_exp_s_f( ah[o->i.v[0]], ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_log_s 0x5C8C1E362EB72F06ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_log_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_log_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_log_s_arity( const bhvm_vop_ar1_log_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_log_s_get_index_arr( bhvm_vop_ar1_log_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_log_s_sig( const bhvm_vop_ar1_log_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_log_s_run( const bhvm_vop_ar1_log_s* o, bhvm_holor_s** ah ){bhvm_hop_ar1_log_s_f( ah[o->i.v[0]], ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_inv_s 0x31C65B240AE6B977ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_inv_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_inv_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_inv_s_arity( const bhvm_vop_ar1_inv_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_inv_s_get_index_arr( bhvm_vop_ar1_inv_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_inv_s_sig( const bhvm_vop_ar1_inv_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_inv_s_run( const bhvm_vop_ar1_inv_s* o, bhvm_holor_s** ah ){bhvm_hop_ar1_inv_s_f( ah[o->i.v[0]], ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_sqr_s 0x4FF02CBD18A33E5Aull
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_sqr_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_sqr_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_sqr_s_arity( const bhvm_vop_ar1_sqr_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_sqr_s_get_index_arr( bhvm_vop_ar1_sqr_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_sqr_s_sig( const bhvm_vop_ar1_sqr_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_sqr_s_run( const bhvm_vop_ar1_sqr_s* o, bhvm_holor_s** ah ){bhvm_hop_ar1_sqr_s_f( ah[o->i.v[0]], ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_srt_s 0x4971FCB2443CD99Bull
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_srt_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_srt_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_srt_s_arity( const bhvm_vop_ar1_srt_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_srt_s_get_index_arr( bhvm_vop_ar1_srt_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_srt_s_sig( const bhvm_vop_ar1_srt_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_srt_s_run( const bhvm_vop_ar1_srt_s* o, bhvm_holor_s** ah ){bhvm_hop_ar1_srt_s_f( ah[o->i.v[0]], ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_abs_s 0xAE155D3F1FD4A1D4ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_abs_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_abs_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_abs_s_arity( const bhvm_vop_ar1_abs_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_abs_s_get_index_arr( bhvm_vop_ar1_abs_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_abs_s_sig( const bhvm_vop_ar1_abs_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_abs_s_run( const bhvm_vop_ar1_abs_s* o, bhvm_holor_s** ah ){bhvm_hop_ar1_abs_s_f( ah[o->i.v[0]], ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_l1_s 0x275D4F703D24B947ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_l1_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_l1_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_l1_s_arity( const bhvm_vop_ar1_l1_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_l1_s_get_index_arr( bhvm_vop_ar1_l1_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_l1_s_sig( const bhvm_vop_ar1_l1_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_l1_s_run( const bhvm_vop_ar1_l1_s* o, bhvm_holor_s** ah ){bhvm_hop_ar1_l1_s_f( ah[o->i.v[0]], ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_sigm_s 0xC3EAA8CF6E8F72E0ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_sigm_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_sigm_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_sigm_s_arity( const bhvm_vop_ar1_sigm_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_sigm_s_get_index_arr( bhvm_vop_ar1_sigm_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_sigm_s_sig( const bhvm_vop_ar1_sigm_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_sigm_s_run( const bhvm_vop_ar1_sigm_s* o, bhvm_holor_s** ah ){bhvm_hop_ar1_sigm_s_f( ah[o->i.v[0]], ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_sigm_hard_s 0x39CFDF776B002348ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_sigm_hard_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_sigm_hard_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_sigm_hard_s_arity( const bhvm_vop_ar1_sigm_hard_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_sigm_hard_s_get_index_arr( bhvm_vop_ar1_sigm_hard_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_sigm_hard_s_sig( const bhvm_vop_ar1_sigm_hard_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_sigm_hard_s_run( const bhvm_vop_ar1_sigm_hard_s* o, bhvm_holor_s** ah ){bhvm_hop_ar1_sigm_hard_s_f( ah[o->i.v[0]], ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_sigm_leaky_s 0x1637FCD8D2F0037Dull
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_sigm_leaky_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_sigm_leaky_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_sigm_leaky_s_arity( const bhvm_vop_ar1_sigm_leaky_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_sigm_leaky_s_get_index_arr( bhvm_vop_ar1_sigm_leaky_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_sigm_leaky_s_sig( const bhvm_vop_ar1_sigm_leaky_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_sigm_leaky_s_run( const bhvm_vop_ar1_sigm_leaky_s* o, bhvm_holor_s** ah ){bhvm_hop_ar1_sigm_leaky_s_f( ah[o->i.v[0]], ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_tanh_s 0xBB2F36561F323145ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_tanh_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_tanh_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_tanh_s_arity( const bhvm_vop_ar1_tanh_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_tanh_s_get_index_arr( bhvm_vop_ar1_tanh_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_tanh_s_sig( const bhvm_vop_ar1_tanh_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_tanh_s_run( const bhvm_vop_ar1_tanh_s* o, bhvm_holor_s** ah ){bhvm_hop_ar1_tanh_s_f( ah[o->i.v[0]], ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_tanh_hard_s 0x5A5ED90104006A13ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_tanh_hard_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_tanh_hard_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_tanh_hard_s_arity( const bhvm_vop_ar1_tanh_hard_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_tanh_hard_s_get_index_arr( bhvm_vop_ar1_tanh_hard_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_tanh_hard_s_sig( const bhvm_vop_ar1_tanh_hard_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_tanh_hard_s_run( const bhvm_vop_ar1_tanh_hard_s* o, bhvm_holor_s** ah ){bhvm_hop_ar1_tanh_hard_s_f( ah[o->i.v[0]], ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_tanh_leaky_s 0xFBF5B4E7063D34A8ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_tanh_leaky_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_tanh_leaky_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_tanh_leaky_s_arity( const bhvm_vop_ar1_tanh_leaky_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_tanh_leaky_s_get_index_arr( bhvm_vop_ar1_tanh_leaky_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_tanh_leaky_s_sig( const bhvm_vop_ar1_tanh_leaky_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_tanh_leaky_s_run( const bhvm_vop_ar1_tanh_leaky_s* o, bhvm_holor_s** ah ){bhvm_hop_ar1_tanh_leaky_s_f( ah[o->i.v[0]], ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_softplus_s 0x26A9DAB0A6783EC0ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_softplus_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_softplus_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_softplus_s_arity( const bhvm_vop_ar1_softplus_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_softplus_s_get_index_arr( bhvm_vop_ar1_softplus_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_softplus_s_sig( const bhvm_vop_ar1_softplus_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_softplus_s_run( const bhvm_vop_ar1_softplus_s* o, bhvm_holor_s** ah ){bhvm_hop_ar1_softplus_s_f( ah[o->i.v[0]], ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_softmax_s 0x1EA8B7E3F1825CCEull
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_softmax_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_softmax_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_softmax_s_arity( const bhvm_vop_ar1_softmax_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_softmax_s_get_index_arr( bhvm_vop_ar1_softmax_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_softmax_s_sig( const bhvm_vop_ar1_softmax_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_softmax_s_run( const bhvm_vop_ar1_softmax_s* o, bhvm_holor_s** ah ){bhvm_hop_ar1_softmax_s_f( ah[o->i.v[0]], ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_relu_s 0x7B3E9DC89191A192ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_relu_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_relu_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_relu_s_arity( const bhvm_vop_ar1_relu_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_relu_s_get_index_arr( bhvm_vop_ar1_relu_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_relu_s_sig( const bhvm_vop_ar1_relu_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_relu_s_run( const bhvm_vop_ar1_relu_s* o, bhvm_holor_s** ah ){bhvm_hop_ar1_relu_s_f( ah[o->i.v[0]], ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_relu_leaky_s 0x6F1130097565B587ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_relu_leaky_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_relu_leaky_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_relu_leaky_s_arity( const bhvm_vop_ar1_relu_leaky_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_relu_leaky_s_get_index_arr( bhvm_vop_ar1_relu_leaky_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_relu_leaky_s_sig( const bhvm_vop_ar1_relu_leaky_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_relu_leaky_s_run( const bhvm_vop_ar1_relu_leaky_s* o, bhvm_holor_s** ah ){bhvm_hop_ar1_relu_leaky_s_f( ah[o->i.v[0]], ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_order_inc_s 0x836C0FB5A069554Bull
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_order_inc_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_order_inc_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;sz_t dim;}; \
  static inline sz_t bhvm_vop_ar1_order_inc_s_arity( const bhvm_vop_ar1_order_inc_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_order_inc_s_get_index_arr( bhvm_vop_ar1_order_inc_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar1_order_inc_s_run( const bhvm_vop_ar1_order_inc_s* o, bhvm_holor_s** ah ){bhvm_holor_s_order_inc( ah[o->i.v[0]], o->dim, ah[o->i.v[1]] );} \
  static inline sc_t bhvm_vop_ar1_order_inc_s_sig( const bhvm_vop_ar1_order_inc_s* o ){return "ay";}
#define TYPEOF_bhvm_vop_ar1_order_dec_s 0x1CC404A6159D27DBull
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_order_dec_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_order_dec_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;sz_t idx;}; \
  static inline sz_t bhvm_vop_ar1_order_dec_s_arity( const bhvm_vop_ar1_order_dec_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_order_dec_s_get_index_arr( bhvm_vop_ar1_order_dec_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar1_order_dec_s_run( const bhvm_vop_ar1_order_dec_s* o, bhvm_holor_s** ah ){bhvm_holor_s_order_dec( ah[o->i.v[0]], o->idx, ah[o->i.v[1]] );} \
  static inline sc_t bhvm_vop_ar1_order_dec_s_sig( const bhvm_vop_ar1_order_dec_s* o ){return "ay";}
#define TYPEOF_bhvm_vop_ar1_order_dec_weak_s 0xDEC1A96B7FF08114ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_order_dec_weak_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_order_dec_weak_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;sz_t idx;}; \
  static inline sz_t bhvm_vop_ar1_order_dec_weak_s_arity( const bhvm_vop_ar1_order_dec_weak_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_order_dec_weak_s_get_index_arr( bhvm_vop_ar1_order_dec_weak_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar1_order_dec_weak_s_run( const bhvm_vop_ar1_order_dec_weak_s* o, bhvm_holor_s** ah ){bhvm_holor_s_order_dec_weak( ah[o->i.v[0]], o->idx, ah[o->i.v[1]] );} \
  static inline sc_t bhvm_vop_ar1_order_dec_weak_s_sig( const bhvm_vop_ar1_order_dec_weak_s* o ){return "ay";}
#define TYPEOF_bhvm_vop_ar1_add_dp_a_s 0xC77AEAE2EBF367CCull
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_add_dp_a_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_add_dp_a_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_add_dp_a_s_arity( const bhvm_vop_ar1_add_dp_a_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_add_dp_a_s_get_index_arr( bhvm_vop_ar1_add_dp_a_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar1_add_dp_a_s_run( const bhvm_vop_ar1_add_dp_a_s* o, bhvm_holor_s** ah ){bhvm_hop_ar1_eci_cpy_acc_s_f( ah[o->i.v[0]], ah[o->i.v[1]] );} \
  static inline sc_t bhvm_vop_ar1_add_dp_a_s_sig( const bhvm_vop_ar1_add_dp_a_s* o ){return "zf";}
#define TYPEOF_bhvm_vop_ar1_add_dp_b_s 0xC02CB9E2E8330589ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_add_dp_b_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_add_dp_b_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_add_dp_b_s_arity( const bhvm_vop_ar1_add_dp_b_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_add_dp_b_s_get_index_arr( bhvm_vop_ar1_add_dp_b_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar1_add_dp_b_s_run( const bhvm_vop_ar1_add_dp_b_s* o, bhvm_holor_s** ah ){bhvm_hop_ar1_eci_cpy_acc_s_f( ah[o->i.v[0]], ah[o->i.v[1]] );} \
  static inline sc_t bhvm_vop_ar1_add_dp_b_s_sig( const bhvm_vop_ar1_add_dp_b_s* o ){return "zg";}
#define TYPEOF_bhvm_vop_ar1_sub_dp_a_s 0x0C77F43FEB92338Full
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_sub_dp_a_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_sub_dp_a_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_sub_dp_a_s_arity( const bhvm_vop_ar1_sub_dp_a_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_sub_dp_a_s_get_index_arr( bhvm_vop_ar1_sub_dp_a_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar1_sub_dp_a_s_run( const bhvm_vop_ar1_sub_dp_a_s* o, bhvm_holor_s** ah ){bhvm_hop_ar1_eci_cpy_acc_s_f( ah[o->i.v[0]], ah[o->i.v[1]] );} \
  static inline sc_t bhvm_vop_ar1_sub_dp_a_s_sig( const bhvm_vop_ar1_sub_dp_a_s* o ){return "zf";}
#define TYPEOF_bhvm_vop_ar1_sub_dp_b_s 0xF325713FDD6A98F6ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_sub_dp_b_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_sub_dp_b_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_sub_dp_b_s_arity( const bhvm_vop_ar1_sub_dp_b_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_sub_dp_b_s_get_index_arr( bhvm_vop_ar1_sub_dp_b_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar1_sub_dp_b_s_run( const bhvm_vop_ar1_sub_dp_b_s* o, bhvm_holor_s** ah ){bhvm_hop_ar1_eci_neg_acc_s_f( ah[o->i.v[0]], ah[o->i.v[1]] );} \
  static inline sc_t bhvm_vop_ar1_sub_dp_b_s_sig( const bhvm_vop_ar1_sub_dp_b_s* o ){return "zg";}
#define TYPEOF_bhvm_vop_ar1_neg_dp_s 0x728039EC103CCC73ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_neg_dp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_neg_dp_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_neg_dp_s_arity( const bhvm_vop_ar1_neg_dp_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_neg_dp_s_get_index_arr( bhvm_vop_ar1_neg_dp_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar1_neg_dp_s_run( const bhvm_vop_ar1_neg_dp_s* o, bhvm_holor_s** ah ){bhvm_hop_ar1_eci_neg_acc_s_f( ah[o->i.v[0]], ah[o->i.v[1]] );} \
  static inline sc_t bhvm_vop_ar1_neg_dp_s_sig( const bhvm_vop_ar1_neg_dp_s* o ){return "zf";}
#define TYPEOF_bhvm_vop_ar1_cat_dp_a_s 0x88696E215A318591ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_cat_dp_a_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_cat_dp_a_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_cat_dp_a_s_arity( const bhvm_vop_ar1_cat_dp_a_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_cat_dp_a_s_get_index_arr( bhvm_vop_ar1_cat_dp_a_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar1_cat_dp_a_s_run( const bhvm_vop_ar1_cat_dp_a_s* o, bhvm_holor_s** ah ){bhvm_hop_ar1_cat_dp_zf_s_f( ah[o->i.v[0]], ah[o->i.v[1]] );} \
  static inline sc_t bhvm_vop_ar1_cat_dp_a_s_sig( const bhvm_vop_ar1_cat_dp_a_s* o ){return "zf";}
#define TYPEOF_bhvm_vop_ar1_cat_dp_b_s 0x8FB89F215DF39AD4ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_cat_dp_b_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_cat_dp_b_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_cat_dp_b_s_arity( const bhvm_vop_ar1_cat_dp_b_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_cat_dp_b_s_get_index_arr( bhvm_vop_ar1_cat_dp_b_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar1_cat_dp_b_s_run( const bhvm_vop_ar1_cat_dp_b_s* o, bhvm_holor_s** ah ){bhvm_hop_ar1_cat_dp_zg_s_f( ah[o->i.v[0]], ah[o->i.v[1]] );} \
  static inline sc_t bhvm_vop_ar1_cat_dp_b_s_sig( const bhvm_vop_ar1_cat_dp_b_s* o ){return "zg";}
#define TYPEOF_bhvm_vop_ar1_ccat_dp_a_s 0x40218A5F6C78C5E0ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_ccat_dp_a_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_ccat_dp_a_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_ccat_dp_a_s_arity( const bhvm_vop_ar1_ccat_dp_a_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_ccat_dp_a_s_get_index_arr( bhvm_vop_ar1_ccat_dp_a_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar1_ccat_dp_a_s_run( const bhvm_vop_ar1_ccat_dp_a_s* o, bhvm_holor_s** ah ){bhvm_hop_ar1_ccat_dp_zf_s_f( ah[o->i.v[0]], ah[o->i.v[1]] );} \
  static inline sc_t bhvm_vop_ar1_ccat_dp_a_s_sig( const bhvm_vop_ar1_ccat_dp_a_s* o ){return "zf";}
#define TYPEOF_bhvm_vop_ar1_ccat_dp_b_s 0x37F9795F67FEF63Dull
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_ccat_dp_b_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_ccat_dp_b_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_ccat_dp_b_s_arity( const bhvm_vop_ar1_ccat_dp_b_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_ccat_dp_b_s_get_index_arr( bhvm_vop_ar1_ccat_dp_b_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar1_ccat_dp_b_s_run( const bhvm_vop_ar1_ccat_dp_b_s* o, bhvm_holor_s** ah ){bhvm_hop_ar1_ccat_dp_zg_s_f( ah[o->i.v[0]], ah[o->i.v[1]] );} \
  static inline sc_t bhvm_vop_ar1_ccat_dp_b_s_sig( const bhvm_vop_ar1_ccat_dp_b_s* o ){return "zg";}
#define TYPEOF_bhvm_vop_ar1_cpy_s 0xDB0073A217778536ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_cpy_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_cpy_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_cpy_s_arity( const bhvm_vop_ar1_cpy_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_cpy_s_get_index_arr( bhvm_vop_ar1_cpy_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_cpy_s_sig( const bhvm_vop_ar1_cpy_s* o ){return "ay";} \
  static inline void bhvm_vop_ar1_cpy_s_run( const bhvm_vop_ar1_cpy_s* o, bhvm_holor_s** ah ){bhvm_hop_ar1_cpy_s_f( ah[o->i.v[0]], ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_acc_s 0xB36A853849BA8C1Dull
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_acc_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_acc_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_acc_s_arity( const bhvm_vop_ar1_acc_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_acc_s_get_index_arr( bhvm_vop_ar1_acc_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_acc_s_sig( const bhvm_vop_ar1_acc_s* o ){return "zf";} \
  static inline void bhvm_vop_ar1_acc_s_run( const bhvm_vop_ar1_acc_s* o, bhvm_holor_s** ah ){bhvm_hop_ar1_eci_cpy_acc_s_f( ah[o->i.v[0]], ah[o->i.v[1]] );}
#define TYPEOF_bhvm_vop_ar1_fork_s 0xC1761D92AAB91584ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_fork_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_fork_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;}; \
  static inline sz_t bhvm_vop_ar1_fork_s_arity( const bhvm_vop_ar1_fork_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_fork_s_get_index_arr( bhvm_vop_ar1_fork_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_fork_s_sig( const bhvm_vop_ar1_fork_s* o ){return "ay";} \
  void bhvm_vop_ar1_fork_s_run( const bhvm_vop_ar1_fork_s* o, bhvm_holor_s** ah );
#define TYPEOF_bhvm_vop_ar1_reshape_s 0xC301072280F48B7Eull
#define BETH_EXPAND_ITEM_bhvm_vop_ar1_reshape_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar1_reshape_s ) \
    {aware_t _;bhvm_vop_ar1_index_s i;bhvm_shape_s shape;}; \
  static inline sz_t bhvm_vop_ar1_reshape_s_arity( const bhvm_vop_ar1_reshape_s* o ){return 1;} \
  static inline sz_t* bhvm_vop_ar1_reshape_s_get_index_arr( bhvm_vop_ar1_reshape_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar1_reshape_s_sig( const bhvm_vop_ar1_reshape_s* o ){return "ay";} \
  void bhvm_vop_ar1_reshape_s_run( const bhvm_vop_ar1_reshape_s* o, bhvm_holor_s** ah );
#define BETH_EXPAND_GROUP_bhvm_vop_ar1 \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1 ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_index_s ); \
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
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_sigm_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_sigm_hard_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_sigm_leaky_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_tanh_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_tanh_hard_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_tanh_leaky_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_softplus_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_softmax_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_relu_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_relu_leaky_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_order_inc_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_order_dec_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_order_dec_weak_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_add_dp_a_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_add_dp_b_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_sub_dp_a_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_sub_dp_b_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_neg_dp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_cat_dp_a_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_cat_dp_b_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_ccat_dp_a_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_ccat_dp_b_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_cpy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_acc_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_fork_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar1_reshape_s ); \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_index_s \
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
  BETH_EXPAND_ITEM_bhvm_vop_ar1_sigm_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_sigm_hard_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_sigm_leaky_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_tanh_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_tanh_hard_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_tanh_leaky_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_softplus_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_softmax_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_relu_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_relu_leaky_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_order_inc_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_order_dec_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_order_dec_weak_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_add_dp_a_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_add_dp_b_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_sub_dp_a_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_sub_dp_b_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_neg_dp_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_cat_dp_a_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_cat_dp_b_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_ccat_dp_a_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_ccat_dp_b_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_cpy_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_acc_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_fork_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar1_reshape_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_vop_ar2

#define TYPEOF_bhvm_vop_ar2 0xA8CDC16D077815FCull
#define TYPEOF_bhvm_vop_ar2_s 0x17D93A05D8B0437Eull
#define TYPEOF_bhvm_vop_ar2_index_s 0x8D5B2A7DD5641779ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_index_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_index_s ) \
    {sz_t v[ 3 ];}; \
  static inline void bhvm_vop_ar2_index_s_init_x( bhvm_vop_ar2_index_s* o ){o->v[0] = o->v[1] = o->v[2] = -1;}
#define TYPEOF_bhvm_vop_ar2_cat_s 0x33B64C46A8138E3Dull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_cat_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_cat_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_cat_s_arity( const bhvm_vop_ar2_cat_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_cat_s_get_index_arr( bhvm_vop_ar2_cat_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_cat_s_sig( const bhvm_vop_ar2_cat_s* o ){return "aby";} \
  static inline void bhvm_vop_ar2_cat_s_run( const bhvm_vop_ar2_cat_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_cat_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_ccat_s 0xD8DF9B41D580575Cull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_ccat_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_ccat_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_ccat_s_arity( const bhvm_vop_ar2_ccat_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_ccat_s_get_index_arr( bhvm_vop_ar2_ccat_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_ccat_s_sig( const bhvm_vop_ar2_ccat_s* o ){return "aby";} \
  static inline void bhvm_vop_ar2_ccat_s_run( const bhvm_vop_ar2_ccat_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_ccat_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_add_s 0x5A6E99F435F59B9Aull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_add_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_add_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_add_s_arity( const bhvm_vop_ar2_add_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_add_s_get_index_arr( bhvm_vop_ar2_add_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_add_s_run( const bhvm_vop_ar2_add_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_eci_add_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_add_s_sig( const bhvm_vop_ar2_add_s* o ){return "aby";}
#define TYPEOF_bhvm_vop_ar2_sub_s 0xAA7AEACD68078323ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_sub_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_sub_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_sub_s_arity( const bhvm_vop_ar2_sub_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_sub_s_get_index_arr( bhvm_vop_ar2_sub_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_sub_s_run( const bhvm_vop_ar2_sub_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_eci_sub_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_sub_s_sig( const bhvm_vop_ar2_sub_s* o ){return "aby";}
#define TYPEOF_bhvm_vop_ar2_mul_s 0xC9605874F4578D07ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_s_arity( const bhvm_vop_ar2_mul_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_s_get_index_arr( bhvm_vop_ar2_mul_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_s_run( const bhvm_vop_ar2_mul_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_eci_mul_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_s_sig( const bhvm_vop_ar2_mul_s* o ){return "aby";}
#define TYPEOF_bhvm_vop_ar2_div_s 0x62300FE3509F5126ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_div_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_div_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_div_s_arity( const bhvm_vop_ar2_div_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_div_s_get_index_arr( bhvm_vop_ar2_div_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_div_s_run( const bhvm_vop_ar2_div_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_eci_div_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_div_s_sig( const bhvm_vop_ar2_div_s* o ){return "aby";}
#define TYPEOF_bhvm_vop_ar2_pow_s 0x8D08D91CC2A26B6Bull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_pow_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_pow_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_pow_s_arity( const bhvm_vop_ar2_pow_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_pow_s_get_index_arr( bhvm_vop_ar2_pow_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_pow_s_run( const bhvm_vop_ar2_pow_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_eci_pow_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_pow_s_sig( const bhvm_vop_ar2_pow_s* o ){return "aby";}
#define TYPEOF_bhvm_vop_ar2_mul_mvv_s 0xB8F984B76A1A671Bull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_mvv_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_mvv_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_mvv_s_arity( const bhvm_vop_ar2_mul_mvv_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_mvv_s_get_index_arr( bhvm_vop_ar2_mul_mvv_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_mul_mvv_s_sig( const bhvm_vop_ar2_mul_mvv_s* o ){return "aby";} \
  static inline void bhvm_vop_ar2_mul_mvv_s_run( const bhvm_vop_ar2_mul_mvv_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_mul_mvv_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_mul_vmv_s 0x510F9F9B8703F4DDull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vmv_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_vmv_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_vmv_s_arity( const bhvm_vop_ar2_mul_vmv_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_vmv_s_get_index_arr( bhvm_vop_ar2_mul_vmv_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_mul_vmv_s_sig( const bhvm_vop_ar2_mul_vmv_s* o ){return "aby";} \
  static inline void bhvm_vop_ar2_mul_vmv_s_run( const bhvm_vop_ar2_mul_vmv_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_mul_vmv_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_mul_tvv_s 0xB624861A9D994A64ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_tvv_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_tvv_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_tvv_s_arity( const bhvm_vop_ar2_mul_tvv_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_tvv_s_get_index_arr( bhvm_vop_ar2_mul_tvv_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_mul_tvv_s_sig( const bhvm_vop_ar2_mul_tvv_s* o ){return "aby";} \
  static inline void bhvm_vop_ar2_mul_tvv_s_run( const bhvm_vop_ar2_mul_tvv_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_mul_tvv_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_mul_vtv_s 0xAAB1B65A21077938ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vtv_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_vtv_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_vtv_s_arity( const bhvm_vop_ar2_mul_vtv_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_vtv_s_get_index_arr( bhvm_vop_ar2_mul_vtv_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_mul_vtv_s_sig( const bhvm_vop_ar2_mul_vtv_s* o ){return "aby";} \
  static inline void bhvm_vop_ar2_mul_vtv_s_run( const bhvm_vop_ar2_mul_vtv_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_mul_vtv_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_mul_vvm_s 0x968E116C50B3EDB7ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vvm_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_vvm_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_vvm_s_arity( const bhvm_vop_ar2_mul_vvm_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_vvm_s_get_index_arr( bhvm_vop_ar2_mul_vvm_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_mul_vvm_s_sig( const bhvm_vop_ar2_mul_vvm_s* o ){return "aby";} \
  static inline void bhvm_vop_ar2_mul_vvm_s_run( const bhvm_vop_ar2_mul_vvm_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_mul_vvm_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_mul_mmm_s 0x142D49E13A5E2105ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_mmm_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_mmm_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_mmm_s_arity( const bhvm_vop_ar2_mul_mmm_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_mmm_s_get_index_arr( bhvm_vop_ar2_mul_mmm_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_mul_mmm_s_sig( const bhvm_vop_ar2_mul_mmm_s* o ){return "aby";} \
  static inline void bhvm_vop_ar2_mul_mmm_s_run( const bhvm_vop_ar2_mul_mmm_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_mul_mmm_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_mul_mtm_s 0x04A0E5A5EAC876D0ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_mtm_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_mtm_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_mtm_s_arity( const bhvm_vop_ar2_mul_mtm_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_mtm_s_get_index_arr( bhvm_vop_ar2_mul_mtm_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_mul_mtm_s_sig( const bhvm_vop_ar2_mul_mtm_s* o ){return "aby";} \
  static inline void bhvm_vop_ar2_mul_mtm_s_run( const bhvm_vop_ar2_mul_mtm_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_mul_mtm_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_mul_tmm_s 0x03179C6D7C1EC75Eull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_tmm_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_tmm_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_tmm_s_arity( const bhvm_vop_ar2_mul_tmm_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_tmm_s_get_index_arr( bhvm_vop_ar2_mul_tmm_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_mul_tmm_s_sig( const bhvm_vop_ar2_mul_tmm_s* o ){return "aby";} \
  static inline void bhvm_vop_ar2_mul_tmm_s_run( const bhvm_vop_ar2_mul_tmm_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_mul_tmm_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_mul_ttm_s 0x02A7112BE2233D73ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_ttm_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_ttm_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_ttm_s_arity( const bhvm_vop_ar2_mul_ttm_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_ttm_s_get_index_arr( bhvm_vop_ar2_mul_ttm_s* o ){return o->i.v;} \
  static inline sc_t bhvm_vop_ar2_mul_ttm_s_sig( const bhvm_vop_ar2_mul_ttm_s* o ){return "aby";} \
  static inline void bhvm_vop_ar2_mul_ttm_s_run( const bhvm_vop_ar2_mul_ttm_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_mul_ttm_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );}
#define TYPEOF_bhvm_vop_ar2_logic_equal_s 0x23190C807ED313CCull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_logic_equal_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_logic_equal_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_logic_equal_s_arity( const bhvm_vop_ar2_logic_equal_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_logic_equal_s_get_index_arr( bhvm_vop_ar2_logic_equal_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_logic_equal_s_run( const bhvm_vop_ar2_logic_equal_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_eci_logic_equal_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_logic_equal_s_sig( const bhvm_vop_ar2_logic_equal_s* o ){return "aby";}
#define TYPEOF_bhvm_vop_ar2_logic_unequal_s 0x030254DCDD49EB01ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_logic_unequal_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_logic_unequal_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_logic_unequal_s_arity( const bhvm_vop_ar2_logic_unequal_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_logic_unequal_s_get_index_arr( bhvm_vop_ar2_logic_unequal_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_logic_unequal_s_run( const bhvm_vop_ar2_logic_unequal_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_eci_logic_unequal_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_logic_unequal_s_sig( const bhvm_vop_ar2_logic_unequal_s* o ){return "aby";}
#define TYPEOF_bhvm_vop_ar2_logic_larger_s 0x9441041DD23EFB4Bull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_logic_larger_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_logic_larger_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_logic_larger_s_arity( const bhvm_vop_ar2_logic_larger_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_logic_larger_s_get_index_arr( bhvm_vop_ar2_logic_larger_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_logic_larger_s_run( const bhvm_vop_ar2_logic_larger_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_eci_logic_larger_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_logic_larger_s_sig( const bhvm_vop_ar2_logic_larger_s* o ){return "aby";}
#define TYPEOF_bhvm_vop_ar2_logic_smaller_s 0x83B03A148C178A20ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_logic_smaller_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_logic_smaller_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_logic_smaller_s_arity( const bhvm_vop_ar2_logic_smaller_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_logic_smaller_s_get_index_arr( bhvm_vop_ar2_logic_smaller_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_logic_smaller_s_run( const bhvm_vop_ar2_logic_smaller_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_eci_logic_smaller_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_logic_smaller_s_sig( const bhvm_vop_ar2_logic_smaller_s* o ){return "aby";}
#define TYPEOF_bhvm_vop_ar2_logic_larger_equal_s 0xA6B01BB3F5A2D010ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_logic_larger_equal_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_logic_larger_equal_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_logic_larger_equal_s_arity( const bhvm_vop_ar2_logic_larger_equal_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_logic_larger_equal_s_get_index_arr( bhvm_vop_ar2_logic_larger_equal_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_logic_larger_equal_s_run( const bhvm_vop_ar2_logic_larger_equal_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_eci_logic_larger_equal_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_logic_larger_equal_s_sig( const bhvm_vop_ar2_logic_larger_equal_s* o ){return "aby";}
#define TYPEOF_bhvm_vop_ar2_logic_smaller_equal_s 0xF21EC6F43EC9ACD3ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_logic_smaller_equal_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_logic_smaller_equal_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_logic_smaller_equal_s_arity( const bhvm_vop_ar2_logic_smaller_equal_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_logic_smaller_equal_s_get_index_arr( bhvm_vop_ar2_logic_smaller_equal_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_logic_smaller_equal_s_run( const bhvm_vop_ar2_logic_smaller_equal_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_eci_logic_smaller_equal_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_logic_smaller_equal_s_sig( const bhvm_vop_ar2_logic_smaller_equal_s* o ){return "aby";}
#define TYPEOF_bhvm_vop_ar2_logic_and_s 0x05BAC86493A9A5F1ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_logic_and_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_logic_and_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_logic_and_s_arity( const bhvm_vop_ar2_logic_and_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_logic_and_s_get_index_arr( bhvm_vop_ar2_logic_and_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_logic_and_s_run( const bhvm_vop_ar2_logic_and_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_eci_logic_and_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_logic_and_s_sig( const bhvm_vop_ar2_logic_and_s* o ){return "aby";}
#define TYPEOF_bhvm_vop_ar2_logic_or_s 0xCA5FC4299202AD35ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_logic_or_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_logic_or_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_logic_or_s_arity( const bhvm_vop_ar2_logic_or_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_logic_or_s_get_index_arr( bhvm_vop_ar2_logic_or_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_logic_or_s_run( const bhvm_vop_ar2_logic_or_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_eci_logic_or_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_logic_or_s_sig( const bhvm_vop_ar2_logic_or_s* o ){return "aby";}
#define TYPEOF_bhvm_vop_ar2_mul_mvv_dp_a_s 0x2C734181F81EE1D8ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_mvv_dp_a_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_mvv_dp_a_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_mvv_dp_a_s_arity( const bhvm_vop_ar2_mul_mvv_dp_a_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_mvv_dp_a_s_get_index_arr( bhvm_vop_ar2_mul_mvv_dp_a_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_mvv_dp_a_s_run( const bhvm_vop_ar2_mul_mvv_dp_a_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_mul_acc_vvm_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_mvv_dp_a_s_sig( const bhvm_vop_ar2_mul_mvv_dp_a_s* o ){return "zbf";}
#define TYPEOF_bhvm_vop_ar2_mul_vmv_dp_a_s 0xD7710F6B272F0B1Eull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vmv_dp_a_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_vmv_dp_a_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_vmv_dp_a_s_arity( const bhvm_vop_ar2_mul_vmv_dp_a_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_vmv_dp_a_s_get_index_arr( bhvm_vop_ar2_mul_vmv_dp_a_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_vmv_dp_a_s_run( const bhvm_vop_ar2_mul_vmv_dp_a_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_mul_acc_vtv_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_vmv_dp_a_s_sig( const bhvm_vop_ar2_mul_vmv_dp_a_s* o ){return "zbf";}
#define TYPEOF_bhvm_vop_ar2_mul_tvv_dp_a_s 0x59D6FE0308BEB079ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_tvv_dp_a_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_tvv_dp_a_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_tvv_dp_a_s_arity( const bhvm_vop_ar2_mul_tvv_dp_a_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_tvv_dp_a_s_get_index_arr( bhvm_vop_ar2_mul_tvv_dp_a_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_tvv_dp_a_s_run( const bhvm_vop_ar2_mul_tvv_dp_a_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_mul_acc_vvm_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_tvv_dp_a_s_sig( const bhvm_vop_ar2_mul_tvv_dp_a_s* o ){return "bzf";}
#define TYPEOF_bhvm_vop_ar2_mul_vtv_dp_a_s 0x5D3779C39E68DAFDull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vtv_dp_a_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_vtv_dp_a_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_vtv_dp_a_s_arity( const bhvm_vop_ar2_mul_vtv_dp_a_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_vtv_dp_a_s_get_index_arr( bhvm_vop_ar2_mul_vtv_dp_a_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_vtv_dp_a_s_run( const bhvm_vop_ar2_mul_vtv_dp_a_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_mul_acc_vmv_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_vtv_dp_a_s_sig( const bhvm_vop_ar2_mul_vtv_dp_a_s* o ){return "zbf";}
#define TYPEOF_bhvm_vop_ar2_mul_vvm_dp_a_s 0x40CDB71430174044ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vvm_dp_a_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_vvm_dp_a_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_vvm_dp_a_s_arity( const bhvm_vop_ar2_mul_vvm_dp_a_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_vvm_dp_a_s_get_index_arr( bhvm_vop_ar2_mul_vvm_dp_a_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_vvm_dp_a_s_run( const bhvm_vop_ar2_mul_vvm_dp_a_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_mul_acc_mvv_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_vvm_dp_a_s_sig( const bhvm_vop_ar2_mul_vvm_dp_a_s* o ){return "zbf";}
#define TYPEOF_bhvm_vop_ar2_mul_mvv_dp_b_s 0x21BE1081F179A395ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_mvv_dp_b_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_mvv_dp_b_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_mvv_dp_b_s_arity( const bhvm_vop_ar2_mul_mvv_dp_b_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_mvv_dp_b_s_get_index_arr( bhvm_vop_ar2_mul_mvv_dp_b_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_mvv_dp_b_s_run( const bhvm_vop_ar2_mul_mvv_dp_b_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_mul_acc_tvv_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_mvv_dp_b_s_sig( const bhvm_vop_ar2_mul_mvv_dp_b_s* o ){return "azg";}
#define TYPEOF_bhvm_vop_ar2_mul_vmv_dp_b_s 0xF19C926B360E9677ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vmv_dp_b_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_vmv_dp_b_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_vmv_dp_b_s_arity( const bhvm_vop_ar2_mul_vmv_dp_b_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_vmv_dp_b_s_get_index_arr( bhvm_vop_ar2_mul_vmv_dp_b_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_vmv_dp_b_s_run( const bhvm_vop_ar2_mul_vmv_dp_b_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_mul_acc_vvm_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_vmv_dp_b_s_sig( const bhvm_vop_ar2_mul_vmv_dp_b_s* o ){return "azg";}
#define TYPEOF_bhvm_vop_ar2_mul_tvv_dp_b_s 0x61262F030C80C5BCull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_tvv_dp_b_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_tvv_dp_b_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_tvv_dp_b_s_arity( const bhvm_vop_ar2_mul_tvv_dp_b_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_tvv_dp_b_s_get_index_arr( bhvm_vop_ar2_mul_tvv_dp_b_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_tvv_dp_b_s_run( const bhvm_vop_ar2_mul_tvv_dp_b_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_mul_acc_mvv_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_tvv_dp_b_s_sig( const bhvm_vop_ar2_mul_tvv_dp_b_s* o ){return "azg";}
#define TYPEOF_bhvm_vop_ar2_mul_vtv_dp_b_s 0x65608AC3A2E45DA0ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vtv_dp_b_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_vtv_dp_b_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_vtv_dp_b_s_arity( const bhvm_vop_ar2_mul_vtv_dp_b_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_vtv_dp_b_s_get_index_arr( bhvm_vop_ar2_mul_vtv_dp_b_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_vtv_dp_b_s_run( const bhvm_vop_ar2_mul_vtv_dp_b_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_mul_acc_vvm_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_vtv_dp_b_s_sig( const bhvm_vop_ar2_mul_vtv_dp_b_s* o ){return "zag";}
#define TYPEOF_bhvm_vop_ar2_mul_vvm_dp_b_s 0x38A586142B9D3A41ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vvm_dp_b_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_vvm_dp_b_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_vvm_dp_b_s_arity( const bhvm_vop_ar2_mul_vvm_dp_b_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_vvm_dp_b_s_get_index_arr( bhvm_vop_ar2_mul_vvm_dp_b_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_vvm_dp_b_s_run( const bhvm_vop_ar2_mul_vvm_dp_b_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_mul_acc_vmv_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_vvm_dp_b_s_sig( const bhvm_vop_ar2_mul_vvm_dp_b_s* o ){return "azg";}
#define TYPEOF_bhvm_vop_ar2_mul_mmm_dp_a_s 0x0575E803C25F9556ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_mmm_dp_a_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_mmm_dp_a_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_mmm_dp_a_s_arity( const bhvm_vop_ar2_mul_mmm_dp_a_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_mmm_dp_a_s_get_index_arr( bhvm_vop_ar2_mul_mmm_dp_a_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_mmm_dp_a_s_run( const bhvm_vop_ar2_mul_mmm_dp_a_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_mul_acc_mtm_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_mmm_dp_a_s_sig( const bhvm_vop_ar2_mul_mmm_dp_a_s* o ){return "zbf";}
#define TYPEOF_bhvm_vop_ar2_mul_mtm_dp_a_s 0x9E55B120E7D62BA5ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_mtm_dp_a_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_mtm_dp_a_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_mtm_dp_a_s_arity( const bhvm_vop_ar2_mul_mtm_dp_a_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_mtm_dp_a_s_get_index_arr( bhvm_vop_ar2_mul_mtm_dp_a_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_mtm_dp_a_s_run( const bhvm_vop_ar2_mul_mtm_dp_a_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_mul_acc_mmm_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_mtm_dp_a_s_sig( const bhvm_vop_ar2_mul_mtm_dp_a_s* o ){return "zbf";}
#define TYPEOF_bhvm_vop_ar2_mul_tmm_dp_a_s 0xF18DC14C3F3D41BFull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_tmm_dp_a_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_tmm_dp_a_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_tmm_dp_a_s_arity( const bhvm_vop_ar2_mul_tmm_dp_a_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_tmm_dp_a_s_get_index_arr( bhvm_vop_ar2_mul_tmm_dp_a_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_tmm_dp_a_s_run( const bhvm_vop_ar2_mul_tmm_dp_a_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_mul_acc_mtm_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_tmm_dp_a_s_sig( const bhvm_vop_ar2_mul_tmm_dp_a_s* o ){return "bzf";}
#define TYPEOF_bhvm_vop_ar2_mul_ttm_dp_a_s 0xAAC310FC9B2A9B40ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_ttm_dp_a_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_ttm_dp_a_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_ttm_dp_a_s_arity( const bhvm_vop_ar2_mul_ttm_dp_a_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_ttm_dp_a_s_get_index_arr( bhvm_vop_ar2_mul_ttm_dp_a_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_ttm_dp_a_s_run( const bhvm_vop_ar2_mul_ttm_dp_a_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_mul_acc_ttm_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_ttm_dp_a_s_sig( const bhvm_vop_ar2_mul_ttm_dp_a_s* o ){return "bzf";}
#define TYPEOF_bhvm_vop_ar2_mul_mmm_dp_b_s 0x1EC86B03D0872FEFull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_mmm_dp_b_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_mmm_dp_b_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_mmm_dp_b_s_arity( const bhvm_vop_ar2_mul_mmm_dp_b_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_mmm_dp_b_s_get_index_arr( bhvm_vop_ar2_mul_mmm_dp_b_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_mmm_dp_b_s_run( const bhvm_vop_ar2_mul_mmm_dp_b_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_mul_acc_tmm_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_mmm_dp_b_s_sig( const bhvm_vop_ar2_mul_mmm_dp_b_s* o ){return "azg";}
#define TYPEOF_bhvm_vop_ar2_mul_mtm_dp_b_s 0xA4CAE220EADE9D28ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_mtm_dp_b_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_mtm_dp_b_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_mtm_dp_b_s_arity( const bhvm_vop_ar2_mul_mtm_dp_b_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_mtm_dp_b_s_get_index_arr( bhvm_vop_ar2_mul_mtm_dp_b_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_mtm_dp_b_s_run( const bhvm_vop_ar2_mul_mtm_dp_b_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_mul_acc_tmm_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_mtm_dp_b_s_sig( const bhvm_vop_ar2_mul_mtm_dp_b_s* o ){return "zag";}
#define TYPEOF_bhvm_vop_ar2_mul_tmm_dp_b_s 0xD7613E4C305C0366ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_tmm_dp_b_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_tmm_dp_b_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_tmm_dp_b_s_arity( const bhvm_vop_ar2_mul_tmm_dp_b_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_tmm_dp_b_s_get_index_arr( bhvm_vop_ar2_mul_tmm_dp_b_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_tmm_dp_b_s_run( const bhvm_vop_ar2_mul_tmm_dp_b_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_mul_acc_mmm_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_tmm_dp_b_s_sig( const bhvm_vop_ar2_mul_tmm_dp_b_s* o ){return "azg";}
#define TYPEOF_bhvm_vop_ar2_mul_ttm_dp_b_s 0xA44DFFFC9822601Dull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_ttm_dp_b_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_ttm_dp_b_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_ttm_dp_b_s_arity( const bhvm_vop_ar2_mul_ttm_dp_b_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_ttm_dp_b_s_get_index_arr( bhvm_vop_ar2_mul_ttm_dp_b_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_ttm_dp_b_s_run( const bhvm_vop_ar2_mul_ttm_dp_b_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_mul_acc_ttm_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_ttm_dp_b_s_sig( const bhvm_vop_ar2_mul_ttm_dp_b_s* o ){return "zag";}
#define TYPEOF_bhvm_vop_ar2_mul_dp_a_s 0x14BEBEBD3FAC6454ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_dp_a_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_dp_a_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_dp_a_s_arity( const bhvm_vop_ar2_mul_dp_a_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_dp_a_s_get_index_arr( bhvm_vop_ar2_mul_dp_a_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_dp_a_s_run( const bhvm_vop_ar2_mul_dp_a_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_eci_mul_acc_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_dp_a_s_sig( const bhvm_vop_ar2_mul_dp_a_s* o ){return "zbf";}
#define TYPEOF_bhvm_vop_ar2_mul_dp_b_s 0x0D6F8DBD3BEA4F11ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_dp_b_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_mul_dp_b_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_mul_dp_b_s_arity( const bhvm_vop_ar2_mul_dp_b_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_mul_dp_b_s_get_index_arr( bhvm_vop_ar2_mul_dp_b_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_mul_dp_b_s_run( const bhvm_vop_ar2_mul_dp_b_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_eci_mul_acc_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_mul_dp_b_s_sig( const bhvm_vop_ar2_mul_dp_b_s* o ){return "zag";}
#define TYPEOF_bhvm_vop_ar2_div_dp_a_s 0xD7864B101B9AEF17ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_div_dp_a_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_div_dp_a_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_div_dp_a_s_arity( const bhvm_vop_ar2_div_dp_a_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_div_dp_a_s_get_index_arr( bhvm_vop_ar2_div_dp_a_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_div_dp_a_s_run( const bhvm_vop_ar2_div_dp_a_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_eci_div_acc_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_div_dp_a_s_sig( const bhvm_vop_ar2_div_dp_a_s* o ){return "zbf";}
#define TYPEOF_bhvm_vop_ar2_abs_dp_s 0xFE06F1BECD073BEEull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_abs_dp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_abs_dp_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_abs_dp_s_arity( const bhvm_vop_ar2_abs_dp_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_abs_dp_s_get_index_arr( bhvm_vop_ar2_abs_dp_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_abs_dp_s_run( const bhvm_vop_ar2_abs_dp_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_abs_dp_zaf_s_f(        ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_abs_dp_s_sig( const bhvm_vop_ar2_abs_dp_s* o ){return "zaf";}
#define TYPEOF_bhvm_vop_ar2_exp_dp_s 0xEA668D57084A8B97ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_exp_dp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_exp_dp_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_exp_dp_s_arity( const bhvm_vop_ar2_exp_dp_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_exp_dp_s_get_index_arr( bhvm_vop_ar2_exp_dp_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_exp_dp_s_run( const bhvm_vop_ar2_exp_dp_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_exp_dp_zyf_s_f(        ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_exp_dp_s_sig( const bhvm_vop_ar2_exp_dp_s* o ){return "zyf";}
#define TYPEOF_bhvm_vop_ar2_log_dp_s 0x26CE99AFD09F9914ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_log_dp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_log_dp_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_log_dp_s_arity( const bhvm_vop_ar2_log_dp_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_log_dp_s_get_index_arr( bhvm_vop_ar2_log_dp_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_log_dp_s_run( const bhvm_vop_ar2_log_dp_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_log_dp_zaf_s_f(        ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_log_dp_s_sig( const bhvm_vop_ar2_log_dp_s* o ){return "zaf";}
#define TYPEOF_bhvm_vop_ar2_inv_dp_s 0x8CB165A8F192336Full
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_inv_dp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_inv_dp_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_inv_dp_s_arity( const bhvm_vop_ar2_inv_dp_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_inv_dp_s_get_index_arr( bhvm_vop_ar2_inv_dp_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_inv_dp_s_run( const bhvm_vop_ar2_inv_dp_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_inv_dp_zyf_s_f(        ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_inv_dp_s_sig( const bhvm_vop_ar2_inv_dp_s* o ){return "zyf";}
#define TYPEOF_bhvm_vop_ar2_sqr_dp_s 0x74B9A82CC29149DCull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_sqr_dp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_sqr_dp_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_sqr_dp_s_arity( const bhvm_vop_ar2_sqr_dp_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_sqr_dp_s_get_index_arr( bhvm_vop_ar2_sqr_dp_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_sqr_dp_s_run( const bhvm_vop_ar2_sqr_dp_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_sqr_dp_zaf_s_f(        ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_sqr_dp_s_sig( const bhvm_vop_ar2_sqr_dp_s* o ){return "zaf";}
#define TYPEOF_bhvm_vop_ar2_srt_dp_s 0x69BB3C764AF11FDBull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_srt_dp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_srt_dp_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_srt_dp_s_arity( const bhvm_vop_ar2_srt_dp_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_srt_dp_s_get_index_arr( bhvm_vop_ar2_srt_dp_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_srt_dp_s_run( const bhvm_vop_ar2_srt_dp_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_srt_dp_zyf_s_f(        ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_srt_dp_s_sig( const bhvm_vop_ar2_srt_dp_s* o ){return "zyf";}
#define TYPEOF_bhvm_vop_ar2_sigm_dp_s 0x7C159CAF4436BB00ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_sigm_dp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_sigm_dp_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_sigm_dp_s_arity( const bhvm_vop_ar2_sigm_dp_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_sigm_dp_s_get_index_arr( bhvm_vop_ar2_sigm_dp_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_sigm_dp_s_run( const bhvm_vop_ar2_sigm_dp_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_sigm_dp_zyf_s_f(       ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_sigm_dp_s_sig( const bhvm_vop_ar2_sigm_dp_s* o ){return "zyf";}
#define TYPEOF_bhvm_vop_ar2_sigm_hard_dp_s 0x834FC87F56C2098Aull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_sigm_hard_dp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_sigm_hard_dp_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_sigm_hard_dp_s_arity( const bhvm_vop_ar2_sigm_hard_dp_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_sigm_hard_dp_s_get_index_arr( bhvm_vop_ar2_sigm_hard_dp_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_sigm_hard_dp_s_run( const bhvm_vop_ar2_sigm_hard_dp_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_sigm_hard_dp_zyf_s_f(  ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_sigm_hard_dp_s_sig( const bhvm_vop_ar2_sigm_hard_dp_s* o ){return "zyf";}
#define TYPEOF_bhvm_vop_ar2_sigm_leaky_dp_s 0xB453C04AB154DE7Full
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_sigm_leaky_dp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_sigm_leaky_dp_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_sigm_leaky_dp_s_arity( const bhvm_vop_ar2_sigm_leaky_dp_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_sigm_leaky_dp_s_get_index_arr( bhvm_vop_ar2_sigm_leaky_dp_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_sigm_leaky_dp_s_run( const bhvm_vop_ar2_sigm_leaky_dp_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_sigm_leaky_dp_zyf_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_sigm_leaky_dp_s_sig( const bhvm_vop_ar2_sigm_leaky_dp_s* o ){return "zyf";}
#define TYPEOF_bhvm_vop_ar2_tanh_dp_s 0x77BBA60B2DDF81CFull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_tanh_dp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_tanh_dp_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_tanh_dp_s_arity( const bhvm_vop_ar2_tanh_dp_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_tanh_dp_s_get_index_arr( bhvm_vop_ar2_tanh_dp_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_tanh_dp_s_run( const bhvm_vop_ar2_tanh_dp_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_tanh_dp_zyf_s_f(       ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_tanh_dp_s_sig( const bhvm_vop_ar2_tanh_dp_s* o ){return "zyf";}
#define TYPEOF_bhvm_vop_ar2_tanh_hard_dp_s 0x47659DDBDCC572EBull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_tanh_hard_dp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_tanh_hard_dp_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_tanh_hard_dp_s_arity( const bhvm_vop_ar2_tanh_hard_dp_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_tanh_hard_dp_s_get_index_arr( bhvm_vop_ar2_tanh_hard_dp_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_tanh_hard_dp_s_run( const bhvm_vop_ar2_tanh_hard_dp_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_tanh_hard_dp_zyf_s_f(  ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_tanh_hard_dp_s_sig( const bhvm_vop_ar2_tanh_hard_dp_s* o ){return "zyf";}
#define TYPEOF_bhvm_vop_ar2_tanh_leaky_dp_s 0x6A5943E4D7C49F88ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_tanh_leaky_dp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_tanh_leaky_dp_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_tanh_leaky_dp_s_arity( const bhvm_vop_ar2_tanh_leaky_dp_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_tanh_leaky_dp_s_get_index_arr( bhvm_vop_ar2_tanh_leaky_dp_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_tanh_leaky_dp_s_run( const bhvm_vop_ar2_tanh_leaky_dp_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_tanh_leaky_dp_zyf_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_tanh_leaky_dp_s_sig( const bhvm_vop_ar2_tanh_leaky_dp_s* o ){return "zyf";}
#define TYPEOF_bhvm_vop_ar2_softplus_dp_s 0x136B5A61EE0C9F60ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_softplus_dp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_softplus_dp_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_softplus_dp_s_arity( const bhvm_vop_ar2_softplus_dp_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_softplus_dp_s_get_index_arr( bhvm_vop_ar2_softplus_dp_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_softplus_dp_s_run( const bhvm_vop_ar2_softplus_dp_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_softplus_dp_zyf_s_f(   ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_softplus_dp_s_sig( const bhvm_vop_ar2_softplus_dp_s* o ){return "zyf";}
#define TYPEOF_bhvm_vop_ar2_softmax_dp_s 0x4E9EE8B49A114E64ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_softmax_dp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_softmax_dp_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_softmax_dp_s_arity( const bhvm_vop_ar2_softmax_dp_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_softmax_dp_s_get_index_arr( bhvm_vop_ar2_softmax_dp_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_softmax_dp_s_run( const bhvm_vop_ar2_softmax_dp_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_softmax_dp_zyf_s_f(    ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_softmax_dp_s_sig( const bhvm_vop_ar2_softmax_dp_s* o ){return "zyf";}
#define TYPEOF_bhvm_vop_ar2_relu_dp_s 0xDC25685F09937086ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_relu_dp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_relu_dp_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_relu_dp_s_arity( const bhvm_vop_ar2_relu_dp_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_relu_dp_s_get_index_arr( bhvm_vop_ar2_relu_dp_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_relu_dp_s_run( const bhvm_vop_ar2_relu_dp_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_relu_dp_zyf_s_f(       ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_relu_dp_s_sig( const bhvm_vop_ar2_relu_dp_s* o ){return "zyf";}
#define TYPEOF_bhvm_vop_ar2_relu_leaky_dp_s 0x682A3DF6EFDE1B45ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_relu_leaky_dp_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_relu_leaky_dp_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_relu_leaky_dp_s_arity( const bhvm_vop_ar2_relu_leaky_dp_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_relu_leaky_dp_s_get_index_arr( bhvm_vop_ar2_relu_leaky_dp_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_relu_leaky_dp_s_run( const bhvm_vop_ar2_relu_leaky_dp_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_relu_leaky_dp_zyf_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_relu_leaky_dp_s_sig( const bhvm_vop_ar2_relu_leaky_dp_s* o ){return "zyf";}
#define TYPEOF_bhvm_vop_ar2_iff_dp_b_s 0x8FF92755F01DB416ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_iff_dp_b_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_iff_dp_b_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_iff_dp_b_s_arity( const bhvm_vop_ar2_iff_dp_b_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_iff_dp_b_s_get_index_arr( bhvm_vop_ar2_iff_dp_b_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_iff_dp_b_s_run( const bhvm_vop_ar2_iff_dp_b_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_eci_iff_dp_b_azg_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_iff_dp_b_s_sig( const bhvm_vop_ar2_iff_dp_b_s* o ){return "azg";}
#define TYPEOF_bhvm_vop_ar2_iff_dp_c_s 0x98216055F497C7B1ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar2_iff_dp_c_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar2_iff_dp_c_s ) \
    {aware_t _;bhvm_vop_ar2_index_s i;}; \
  static inline sz_t bhvm_vop_ar2_iff_dp_c_s_arity( const bhvm_vop_ar2_iff_dp_c_s* o ){return 2;} \
  static inline sz_t* bhvm_vop_ar2_iff_dp_c_s_get_index_arr( bhvm_vop_ar2_iff_dp_c_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar2_iff_dp_c_s_run( const bhvm_vop_ar2_iff_dp_c_s* o, bhvm_holor_s** ah ){bhvm_hop_ar2_eci_iff_dp_c_azh_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]] );} \
  static inline sc_t bhvm_vop_ar2_iff_dp_c_s_sig( const bhvm_vop_ar2_iff_dp_c_s* o ){return "azh";}
#define BETH_EXPAND_GROUP_bhvm_vop_ar2 \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2 ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_index_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_cat_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_ccat_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_add_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_sub_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_div_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_pow_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_mvv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_vmv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_tvv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_vtv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_vvm_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_mmm_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_mtm_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_tmm_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_ttm_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_logic_equal_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_logic_unequal_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_logic_larger_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_logic_smaller_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_logic_larger_equal_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_logic_smaller_equal_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_logic_and_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_logic_or_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_mvv_dp_a_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_vmv_dp_a_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_tvv_dp_a_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_vtv_dp_a_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_vvm_dp_a_s ); \
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
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_dp_a_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_mul_dp_b_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_div_dp_a_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_abs_dp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_exp_dp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_log_dp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_inv_dp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_sqr_dp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_srt_dp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_sigm_dp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_sigm_hard_dp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_sigm_leaky_dp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_tanh_dp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_tanh_hard_dp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_tanh_leaky_dp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_softplus_dp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_softmax_dp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_relu_dp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_relu_leaky_dp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_iff_dp_b_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar2_iff_dp_c_s ); \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_index_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_cat_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_ccat_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_add_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_sub_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_div_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_pow_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_mvv_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vmv_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_tvv_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vtv_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vvm_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_mmm_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_mtm_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_tmm_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_ttm_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_logic_equal_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_logic_unequal_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_logic_larger_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_logic_smaller_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_logic_larger_equal_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_logic_smaller_equal_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_logic_and_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_logic_or_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_mvv_dp_a_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vmv_dp_a_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_tvv_dp_a_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vtv_dp_a_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_vvm_dp_a_s \
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
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_dp_a_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_mul_dp_b_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_div_dp_a_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_abs_dp_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_exp_dp_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_log_dp_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_inv_dp_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_sqr_dp_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_srt_dp_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_sigm_dp_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_sigm_hard_dp_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_sigm_leaky_dp_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_tanh_dp_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_tanh_hard_dp_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_tanh_leaky_dp_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_softplus_dp_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_softmax_dp_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_relu_dp_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_relu_leaky_dp_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_iff_dp_b_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar2_iff_dp_c_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_vop_ar3

#define TYPEOF_bhvm_vop_ar3 0xA8CDC26D077817AFull
#define TYPEOF_bhvm_vop_ar3_s 0x228E5305DF5558F9ull
#define TYPEOF_bhvm_vop_ar3_index_s 0x3398E609ED0BBBAEull
#define BETH_EXPAND_ITEM_bhvm_vop_ar3_index_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar3_index_s ) \
    {sz_t v[ 4 ];}; \
  static inline void bhvm_vop_ar3_index_s_init_x( bhvm_vop_ar3_index_s* o ){o->v[0] = o->v[1] = o->v[2] = o->v[3] = -1;}
#define TYPEOF_bhvm_vop_ar3_iff_s 0x555BD65A67851375ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar3_iff_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar3_iff_s ) \
    {aware_t _;bhvm_vop_ar3_index_s i;}; \
  static inline sz_t bhvm_vop_ar3_iff_s_arity( const bhvm_vop_ar3_iff_s* o ){return 3;} \
  static inline sz_t* bhvm_vop_ar3_iff_s_get_index_arr( bhvm_vop_ar3_iff_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar3_iff_s_run( const bhvm_vop_ar3_iff_s* o, bhvm_holor_s** ah ){bhvm_hop_ar3_eci_iff_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]], ah[o->i.v[3]] );} \
  static inline sc_t bhvm_vop_ar3_iff_s_sig( const bhvm_vop_ar3_iff_s* o ){return "abcy";}
#define TYPEOF_bhvm_vop_ar3_div_dp_b_s 0xD550CA780A856BE7ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar3_div_dp_b_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar3_div_dp_b_s ) \
    {aware_t _;bhvm_vop_ar3_index_s i;}; \
  static inline sz_t bhvm_vop_ar3_div_dp_b_s_arity( const bhvm_vop_ar3_div_dp_b_s* o ){return 3;} \
  static inline sz_t* bhvm_vop_ar3_div_dp_b_s_get_index_arr( bhvm_vop_ar3_div_dp_b_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar3_div_dp_b_s_run( const bhvm_vop_ar3_div_dp_b_s* o, bhvm_holor_s** ah ){bhvm_hop_ar3_eci_div_dp_zabg_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]], ah[o->i.v[3]] );} \
  static inline sc_t bhvm_vop_ar3_div_dp_b_s_sig( const bhvm_vop_ar3_div_dp_b_s* o ){return "zabg";}
#define TYPEOF_bhvm_vop_ar3_pow_dp_a_s 0x1F55D5F263A4D7F5ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar3_pow_dp_a_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar3_pow_dp_a_s ) \
    {aware_t _;bhvm_vop_ar3_index_s i;}; \
  static inline sz_t bhvm_vop_ar3_pow_dp_a_s_arity( const bhvm_vop_ar3_pow_dp_a_s* o ){return 3;} \
  static inline sz_t* bhvm_vop_ar3_pow_dp_a_s_get_index_arr( bhvm_vop_ar3_pow_dp_a_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar3_pow_dp_a_s_run( const bhvm_vop_ar3_pow_dp_a_s* o, bhvm_holor_s** ah ){bhvm_hop_ar3_eci_pow_dp_abzf_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]], ah[o->i.v[3]] );} \
  static inline sc_t bhvm_vop_ar3_pow_dp_a_s_sig( const bhvm_vop_ar3_pow_dp_a_s* o ){return "abzf";}
#define TYPEOF_bhvm_vop_ar3_pow_dp_b_s 0x2A0B06F26A4A1638ull
#define BETH_EXPAND_ITEM_bhvm_vop_ar3_pow_dp_b_s \
  BCORE_DECLARE_OBJECT( bhvm_vop_ar3_pow_dp_b_s ) \
    {aware_t _;bhvm_vop_ar3_index_s i;}; \
  static inline sz_t bhvm_vop_ar3_pow_dp_b_s_arity( const bhvm_vop_ar3_pow_dp_b_s* o ){return 3;} \
  static inline sz_t* bhvm_vop_ar3_pow_dp_b_s_get_index_arr( bhvm_vop_ar3_pow_dp_b_s* o ){return o->i.v;} \
  static inline void bhvm_vop_ar3_pow_dp_b_s_run( const bhvm_vop_ar3_pow_dp_b_s* o, bhvm_holor_s** ah ){bhvm_hop_ar3_eci_pow_dp_ayzg_s_f( ah[o->i.v[0]], ah[o->i.v[1]], ah[o->i.v[2]], ah[o->i.v[3]] );} \
  static inline sc_t bhvm_vop_ar3_pow_dp_b_s_sig( const bhvm_vop_ar3_pow_dp_b_s* o ){return "ayzg";}
#define BETH_EXPAND_GROUP_bhvm_vop_ar3 \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar3 ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar3_index_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar3_iff_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar3_div_dp_b_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar3_pow_dp_a_s ); \
  BCORE_FORWARD_OBJECT( bhvm_vop_ar3_pow_dp_b_s ); \
  BETH_EXPAND_ITEM_bhvm_vop_ar3_index_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar3_iff_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar3_div_dp_b_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar3_pow_dp_a_s \
  BETH_EXPAND_ITEM_bhvm_vop_ar3_pow_dp_b_s

/**********************************************************************************************************************/

vd_t bhvm_vop_xoila_out_signal_handler( const bcore_signal_s* o );

#endif // BHVM_VOP_XOILA_OUT_H
// BETH_PLANT_SIGNATURE 5762254806191665214
