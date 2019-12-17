/** This file was generated from beth-plant source code.
 *  Compiling Agent : bcore_plant_compiler (C) 2019 J.B.Steffens
 *  Last File Update: 2019-12-17T16:25:52Z
 *
 *  Copyright and License of this File:
 *
 *  Generated code inherits the copyright and license of the underlying beth-plant source code.
 *  Source code defining this file is distributed across following files:
 *
 *  bhvm_hf3.h
 *  bhvm_hf3_op.h
 *  bhvm_hf3_vm.h
 *
 */

#ifndef BHVM_PLANTED_HF3_H
#define BHVM_PLANTED_HF3_H

#include "bcore_control.h"


/**********************************************************************************************************************/
// source: bhvm_hf3.h

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3

#define TYPEOF_bhvm_hf3 2560053838
#define TYPEOF_bhvm_hf3_s 3394151696
#define TYPEOF_bhvm_hf3_s 3394151696
#define BETH_EXPAND_ITEM_bhvm_hf3_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_s ) \
    {aware_t _;BCORE_ARRAY_DYN_SOLID_STATIC_S( sz_t, d_ );BCORE_ARRAY_DYN_SOLID_STATIC_S( f3_t, v_ );bl_t htp;}; \
  void bhvm_hf3_s_copy_typed( bhvm_hf3_s* o, tp_t type, vc_t src );
#define TYPEOF_bhvm_hf3_adl_s 963867488
#define BETH_EXPAND_ITEM_bhvm_hf3_adl_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_adl_s ) \
    {aware_t _;BCORE_ARRAY_DYN_LINK_STATIC_S( bhvm_hf3_s, );}; \
  static inline void bhvm_hf3_adl_s_set_space( bhvm_hf3_adl_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_bhvm_hf3_adl_s, ( bcore_array* )o, size ); } \
  static inline void bhvm_hf3_adl_s_set_size( bhvm_hf3_adl_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_bhvm_hf3_adl_s, ( bcore_array* )o, size ); } \
  static inline void bhvm_hf3_adl_s_clear( bhvm_hf3_adl_s* o ) { bcore_array_t_set_space( TYPEOF_bhvm_hf3_adl_s, ( bcore_array* )o, 0 ); } \
  static inline bhvm_hf3_s* bhvm_hf3_adl_s_push_c( bhvm_hf3_adl_s* o, const bhvm_hf3_s* v ) { bcore_array_t_push( TYPEOF_bhvm_hf3_adl_s, ( bcore_array* )o, sr_twc( TYPEOF_bhvm_hf3_s, v ) ); return o->data[ o->size - 1 ]; } \
  static inline bhvm_hf3_s* bhvm_hf3_adl_s_push_d( bhvm_hf3_adl_s* o,       bhvm_hf3_s* v ) { bcore_array_t_push( TYPEOF_bhvm_hf3_adl_s, ( bcore_array* )o, sr_tsd( TYPEOF_bhvm_hf3_s, v ) ); return o->data[ o->size - 1 ]; } \
  static inline bhvm_hf3_s* bhvm_hf3_adl_s_push( bhvm_hf3_adl_s* o ) \
  { \
      bcore_array_t_push( TYPEOF_bhvm_hf3_adl_s, ( bcore_array* )o, sr_t_create( TYPEOF_bhvm_hf3_s ) ); \
      return o->data[ o->size - 1 ]; \
  }
#define BETH_EXPAND_GROUP_bhvm_hf3 \
  BCORE_FORWARD_OBJECT( bhvm_hf3 ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_adl_s ); \
  BETH_EXPAND_ITEM_bhvm_hf3_s \
  BETH_EXPAND_ITEM_bhvm_hf3_adl_s

/**********************************************************************************************************************/
// source: bhvm_hf3_op.h

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_op

#define TYPEOF_bhvm_hf3_op 3787707740
#define TYPEOF_bhvm_hf3_op_s 433841918
#define BETH_EXPAND_GROUP_bhvm_hf3_op \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar0 ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar1 ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar2 ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar3 ); \
  typedef sz_t (*bhvm_hf3_op_get_arity)( const bhvm_hf3_op* o ); \
  typedef void (*bhvm_hf3_op_aof)( const bhvm_hf3_op* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ); \
  typedef sc_t (*bhvm_hf3_op_osig)( const bhvm_hf3_op* o ); \
  BCORE_DECLARE_SPECT( bhvm_hf3_op ) \
  { \
      bcore_spect_header_s header; \
      bhvm_hf3_op_get_arity get_arity; \
      bhvm_hf3_op_aof aof; \
      bhvm_hf3_op_osig osig; \
  }; \
  static inline bhvm_hf3_op* bhvm_hf3_op_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_bhvm_hf3_op, t ); return ( bhvm_hf3_op* )bcore_inst_t_create( t ); } \
  static inline bl_t bhvm_hf3_op_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_bhvm_hf3_op ); } \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bhvm_hf3_op ) \
  static inline bl_t bhvm_hf3_op_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_bhvm_hf3_op ); } \
  static inline sz_t bhvm_hf3_op_p_get_arity( const bhvm_hf3_op_s* __p, const bhvm_hf3_op* o ) { assert( __p->get_arity ); return __p->get_arity( o ); } \
  static inline sz_t bhvm_hf3_op_a_get_arity( const bhvm_hf3_op* o ) { const bhvm_hf3_op_s* p = bhvm_hf3_op_s_get_aware( o ); assert( p->get_arity ); return p->get_arity( o ); } \
  static inline bl_t bhvm_hf3_op_p_defines_get_arity( const bhvm_hf3_op_s* __p ) { return __p->get_arity != NULL; } \
  static inline bl_t bhvm_hf3_op_a_defines_get_arity( const bhvm_hf3_op* o ) { return bhvm_hf3_op_s_get_aware( o )->get_arity != NULL; } \
  static inline void bhvm_hf3_op_p_aof( const bhvm_hf3_op_s* __p, const bhvm_hf3_op* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ) { assert( __p->aof ); __p->aof( o, a, r ); } \
  static inline void bhvm_hf3_op_a_aof( const bhvm_hf3_op* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ) { const bhvm_hf3_op_s* p = bhvm_hf3_op_s_get_aware( o ); assert( p->aof ); p->aof( o, a, r ); } \
  static inline bl_t bhvm_hf3_op_p_defines_aof( const bhvm_hf3_op_s* __p ) { return __p->aof != NULL; } \
  static inline bl_t bhvm_hf3_op_a_defines_aof( const bhvm_hf3_op* o ) { return bhvm_hf3_op_s_get_aware( o )->aof != NULL; } \
  static inline sc_t bhvm_hf3_op_p_osig( const bhvm_hf3_op_s* __p, const bhvm_hf3_op* o ) { assert( __p->osig ); return __p->osig( o ); } \
  static inline sc_t bhvm_hf3_op_a_osig( const bhvm_hf3_op* o ) { const bhvm_hf3_op_s* p = bhvm_hf3_op_s_get_aware( o ); assert( p->osig ); return p->osig( o ); } \
  static inline bl_t bhvm_hf3_op_p_defines_osig( const bhvm_hf3_op_s* __p ) { return __p->osig != NULL; } \
  static inline bl_t bhvm_hf3_op_a_defines_osig( const bhvm_hf3_op* o ) { return bhvm_hf3_op_s_get_aware( o )->osig != NULL; } \
  BETH_EXPAND_GROUP_bhvm_hf3_op_ar0 \
  BETH_EXPAND_GROUP_bhvm_hf3_op_ar1 \
  BETH_EXPAND_GROUP_bhvm_hf3_op_ar2 \
  BETH_EXPAND_GROUP_bhvm_hf3_op_ar3

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_op_ar0

#define TYPEOF_bhvm_hf3_op_ar0 1996187290
#define TYPEOF_bhvm_hf3_op_ar0_s 4104225284
#define TYPEOF_bhvm_hf3_op_ar0_zro_s 1728820786
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar0_zro_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar0_zro_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar0_zro_s_f( bhvm_hf3_s* r ){bhvm_hf3_s_zro(       r );} \
  static inline sz_t bhvm_hf3_op_ar0_zro_s_get_arity( const bhvm_hf3_op_ar0_zro_s* o ){return 0;} \
  static inline void bhvm_hf3_op_ar0_zro_s_of( const bhvm_hf3_op_ar0_zro_s* o, bhvm_hf3_s* r ){bhvm_hf3_op_ar0_zro_s_f( r );} \
  static inline void bhvm_hf3_op_ar0_zro_s_aof( const bhvm_hf3_op_ar0_zro_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar0_zro_s_of( o, r );}
#define TYPEOF_bhvm_hf3_op_ar0_literal_s 3740679784
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar0_literal_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar0_literal_s ) \
    {aware_t _;bhvm_hf3_s* h;}; \
  static inline void bhvm_hf3_op_ar0_literal_s_of( const bhvm_hf3_op_ar0_literal_s* o, bhvm_hf3_s* r ){bhvm_hf3_s_cpy( o->h, r );} \
  static inline sz_t bhvm_hf3_op_ar0_literal_s_get_arity( const bhvm_hf3_op_ar0_literal_s* o ){return 0;} \
  static inline void bhvm_hf3_op_ar0_literal_s_f( bhvm_hf3_s* r ){ERR_fa( "Function 'f' is not available. Need to call 'of' instead?" );} \
  static inline void bhvm_hf3_op_ar0_literal_s_aof( const bhvm_hf3_op_ar0_literal_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar0_literal_s_of( o, r );}
#define BETH_EXPAND_GROUP_bhvm_hf3_op_ar0 \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar0 ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar0_zro_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar0_literal_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar0_dp ); \
  typedef void (*bhvm_hf3_op_ar0_of)( const bhvm_hf3_op_ar0* o, bhvm_hf3_s* r ); \
  BCORE_DECLARE_SPECT( bhvm_hf3_op_ar0 ) \
  { \
      bcore_spect_header_s header; \
      bhvm_hf3_op_ar0_of of; \
  }; \
  static inline bhvm_hf3_op_ar0* bhvm_hf3_op_ar0_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_bhvm_hf3_op_ar0, t ); return ( bhvm_hf3_op_ar0* )bcore_inst_t_create( t ); } \
  static inline bl_t bhvm_hf3_op_ar0_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_bhvm_hf3_op_ar0 ); } \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bhvm_hf3_op_ar0 ) \
  static inline bl_t bhvm_hf3_op_ar0_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_bhvm_hf3_op_ar0 ); } \
  static inline void bhvm_hf3_op_ar0_p_of( const bhvm_hf3_op_ar0_s* __p, const bhvm_hf3_op_ar0* o, bhvm_hf3_s* r ) { assert( __p->of ); __p->of( o, r ); } \
  static inline void bhvm_hf3_op_ar0_a_of( const bhvm_hf3_op_ar0* o, bhvm_hf3_s* r ) { const bhvm_hf3_op_ar0_s* p = bhvm_hf3_op_ar0_s_get_aware( o ); assert( p->of ); p->of( o, r ); } \
  static inline bl_t bhvm_hf3_op_ar0_p_defines_of( const bhvm_hf3_op_ar0_s* __p ) { return __p->of != NULL; } \
  static inline bl_t bhvm_hf3_op_ar0_a_defines_of( const bhvm_hf3_op_ar0* o ) { return bhvm_hf3_op_ar0_s_get_aware( o )->of != NULL; } \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar0_zro_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar0_literal_s \
  BETH_EXPAND_GROUP_bhvm_hf3_op_ar0_dp

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_op_ar0_dp

#define TYPEOF_bhvm_hf3_op_ar0_dp 2288329731
#define TYPEOF_bhvm_hf3_op_ar0_dp_s 3901192933
#define TYPEOF_bhvm_hf3_op_ar0_dp_ca_floor_s 2809581859
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar0_dp_ca_floor_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar0_dp_ca_floor_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar0_dp_ca_floor_s_f( bhvm_hf3_s* r ){bhvm_hf3_s_zro( r );} \
  static inline sc_t bhvm_hf3_op_ar0_dp_ca_floor_s_sig( void ){return "u";} \
  static inline sz_t bhvm_hf3_op_ar0_dp_ca_floor_s_get_arity( const bhvm_hf3_op_ar0_dp_ca_floor_s* o ){return 0;} \
  static inline void bhvm_hf3_op_ar0_dp_ca_floor_s_of( const bhvm_hf3_op_ar0_dp_ca_floor_s* o, bhvm_hf3_s* r ){bhvm_hf3_op_ar0_dp_ca_floor_s_f( r );} \
  static inline void bhvm_hf3_op_ar0_dp_ca_floor_s_aof( const bhvm_hf3_op_ar0_dp_ca_floor_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar0_dp_ca_floor_s_of( o, r );}
#define TYPEOF_bhvm_hf3_op_ar0_dp_ca_ceil_s 2290632494
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar0_dp_ca_ceil_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar0_dp_ca_ceil_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar0_dp_ca_ceil_s_f( bhvm_hf3_s* r ){bhvm_hf3_s_zro( r );} \
  static inline sc_t bhvm_hf3_op_ar0_dp_ca_ceil_s_sig( void ){return "u";} \
  static inline sz_t bhvm_hf3_op_ar0_dp_ca_ceil_s_get_arity( const bhvm_hf3_op_ar0_dp_ca_ceil_s* o ){return 0;} \
  static inline void bhvm_hf3_op_ar0_dp_ca_ceil_s_of( const bhvm_hf3_op_ar0_dp_ca_ceil_s* o, bhvm_hf3_s* r ){bhvm_hf3_op_ar0_dp_ca_ceil_s_f( r );} \
  static inline void bhvm_hf3_op_ar0_dp_ca_ceil_s_aof( const bhvm_hf3_op_ar0_dp_ca_ceil_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar0_dp_ca_ceil_s_of( o, r );}
#define BETH_EXPAND_GROUP_bhvm_hf3_op_ar0_dp \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar0_dp ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar0_dp_ca_floor_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar0_dp_ca_ceil_s ); \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar0_dp_ca_floor_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar0_dp_ca_ceil_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_op_ar1

#define TYPEOF_bhvm_hf3_op_ar1 2012964909
#define TYPEOF_bhvm_hf3_op_ar1_s 1372027103
#define TYPEOF_bhvm_hf3_op_ar1_cpy_s 3037828958
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_cpy_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar1_cpy_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar1_cpy_s_f( const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_s_cpy( a, r );} \
  static inline sz_t bhvm_hf3_op_ar1_cpy_s_get_arity( const bhvm_hf3_op_ar1_cpy_s* o ){return 1;} \
  static inline void bhvm_hf3_op_ar1_cpy_s_of( const bhvm_hf3_op_ar1_cpy_s* o, const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_cpy_s_f(    a   , r );} \
  static inline void bhvm_hf3_op_ar1_cpy_s_aof( const bhvm_hf3_op_ar1_cpy_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_cpy_s_of( o, a[0], r );}
#define TYPEOF_bhvm_hf3_op_ar1_neg_s 1535646186
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_neg_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar1_neg_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar1_neg_s_f( const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_neg_s_fx, r );} \
  static inline sz_t bhvm_hf3_op_ar1_neg_s_get_arity( const bhvm_hf3_op_ar1_neg_s* o ){return 1;} \
  static inline void bhvm_hf3_op_ar1_neg_s_of( const bhvm_hf3_op_ar1_neg_s* o, const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_neg_s_f(    a   , r );} \
  static inline void bhvm_hf3_op_ar1_neg_s_aof( const bhvm_hf3_op_ar1_neg_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_neg_s_of( o, a[0], r );}
#define TYPEOF_bhvm_hf3_op_ar1_floor_s 1914306194
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_floor_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar1_floor_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar1_floor_s_f( const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_s_fp_f3_ar1( a, floor, r );} \
  static inline sz_t bhvm_hf3_op_ar1_floor_s_get_arity( const bhvm_hf3_op_ar1_floor_s* o ){return 1;} \
  static inline void bhvm_hf3_op_ar1_floor_s_of( const bhvm_hf3_op_ar1_floor_s* o, const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_floor_s_f(    a   , r );} \
  static inline void bhvm_hf3_op_ar1_floor_s_aof( const bhvm_hf3_op_ar1_floor_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_floor_s_of( o, a[0], r );}
#define TYPEOF_bhvm_hf3_op_ar1_ceil_s 4149154061
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_ceil_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar1_ceil_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar1_ceil_s_f( const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_s_fp_f3_ar1( a, ceil,  r );} \
  static inline sz_t bhvm_hf3_op_ar1_ceil_s_get_arity( const bhvm_hf3_op_ar1_ceil_s* o ){return 1;} \
  static inline void bhvm_hf3_op_ar1_ceil_s_of( const bhvm_hf3_op_ar1_ceil_s* o, const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_ceil_s_f(    a   , r );} \
  static inline void bhvm_hf3_op_ar1_ceil_s_aof( const bhvm_hf3_op_ar1_ceil_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_ceil_s_of( o, a[0], r );}
#define TYPEOF_bhvm_hf3_op_ar1_exp_s 4149795075
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_exp_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar1_exp_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar1_exp_s_f( const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_s_fp_f3_ar1( a, exp,   r );} \
  static inline sz_t bhvm_hf3_op_ar1_exp_s_get_arity( const bhvm_hf3_op_ar1_exp_s* o ){return 1;} \
  static inline void bhvm_hf3_op_ar1_exp_s_of( const bhvm_hf3_op_ar1_exp_s* o, const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_exp_s_f(    a   , r );} \
  static inline void bhvm_hf3_op_ar1_exp_s_aof( const bhvm_hf3_op_ar1_exp_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_exp_s_of( o, a[0], r );}
#define TYPEOF_bhvm_hf3_op_ar1_lgst_s 2547377190
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_lgst_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar1_lgst_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar1_lgst_s_f( const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_lgst_s_fx,       r );} \
  static inline sz_t bhvm_hf3_op_ar1_lgst_s_get_arity( const bhvm_hf3_op_ar1_lgst_s* o ){return 1;} \
  static inline void bhvm_hf3_op_ar1_lgst_s_of( const bhvm_hf3_op_ar1_lgst_s* o, const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_lgst_s_f(    a   , r );} \
  static inline void bhvm_hf3_op_ar1_lgst_s_aof( const bhvm_hf3_op_ar1_lgst_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_lgst_s_of( o, a[0], r );}
#define TYPEOF_bhvm_hf3_op_ar1_lgst_hard_s 4186446922
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_lgst_hard_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar1_lgst_hard_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar1_lgst_hard_s_f( const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_lgst_hard_s_fx,  r );} \
  static inline sz_t bhvm_hf3_op_ar1_lgst_hard_s_get_arity( const bhvm_hf3_op_ar1_lgst_hard_s* o ){return 1;} \
  static inline void bhvm_hf3_op_ar1_lgst_hard_s_of( const bhvm_hf3_op_ar1_lgst_hard_s* o, const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_lgst_hard_s_f(    a   , r );} \
  static inline void bhvm_hf3_op_ar1_lgst_hard_s_aof( const bhvm_hf3_op_ar1_lgst_hard_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_lgst_hard_s_of( o, a[0], r );}
#define TYPEOF_bhvm_hf3_op_ar1_lgst_leaky_s 1939813155
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_lgst_leaky_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar1_lgst_leaky_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar1_lgst_leaky_s_f( const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_lgst_leaky_s_fx, r );} \
  static inline sz_t bhvm_hf3_op_ar1_lgst_leaky_s_get_arity( const bhvm_hf3_op_ar1_lgst_leaky_s* o ){return 1;} \
  static inline void bhvm_hf3_op_ar1_lgst_leaky_s_of( const bhvm_hf3_op_ar1_lgst_leaky_s* o, const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_lgst_leaky_s_f(    a   , r );} \
  static inline void bhvm_hf3_op_ar1_lgst_leaky_s_aof( const bhvm_hf3_op_ar1_lgst_leaky_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_lgst_leaky_s_of( o, a[0], r );}
#define TYPEOF_bhvm_hf3_op_ar1_tanh_s 3497144045
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_tanh_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar1_tanh_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar1_tanh_s_f( const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_tanh_s_fx,       r );} \
  static inline sz_t bhvm_hf3_op_ar1_tanh_s_get_arity( const bhvm_hf3_op_ar1_tanh_s* o ){return 1;} \
  static inline void bhvm_hf3_op_ar1_tanh_s_of( const bhvm_hf3_op_ar1_tanh_s* o, const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_tanh_s_f(    a   , r );} \
  static inline void bhvm_hf3_op_ar1_tanh_s_aof( const bhvm_hf3_op_ar1_tanh_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_tanh_s_of( o, a[0], r );}
#define TYPEOF_bhvm_hf3_op_ar1_tanh_hard_s 2549621963
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_tanh_hard_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar1_tanh_hard_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar1_tanh_hard_s_f( const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_tanh_hard_s_fx,  r );} \
  static inline sz_t bhvm_hf3_op_ar1_tanh_hard_s_get_arity( const bhvm_hf3_op_ar1_tanh_hard_s* o ){return 1;} \
  static inline void bhvm_hf3_op_ar1_tanh_hard_s_of( const bhvm_hf3_op_ar1_tanh_hard_s* o, const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_tanh_hard_s_f(    a   , r );} \
  static inline void bhvm_hf3_op_ar1_tanh_hard_s_aof( const bhvm_hf3_op_ar1_tanh_hard_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_tanh_hard_s_of( o, a[0], r );}
#define TYPEOF_bhvm_hf3_op_ar1_tanh_leaky_s 1901164464
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_tanh_leaky_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar1_tanh_leaky_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar1_tanh_leaky_s_f( const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_tanh_leaky_s_fx, r );} \
  static inline sz_t bhvm_hf3_op_ar1_tanh_leaky_s_get_arity( const bhvm_hf3_op_ar1_tanh_leaky_s* o ){return 1;} \
  static inline void bhvm_hf3_op_ar1_tanh_leaky_s_of( const bhvm_hf3_op_ar1_tanh_leaky_s* o, const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_tanh_leaky_s_f(    a   , r );} \
  static inline void bhvm_hf3_op_ar1_tanh_leaky_s_aof( const bhvm_hf3_op_ar1_tanh_leaky_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_tanh_leaky_s_of( o, a[0], r );}
#define TYPEOF_bhvm_hf3_op_ar1_softplus_s 1650743528
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_softplus_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar1_softplus_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar1_softplus_s_f( const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_softplus_s_fx,   r );} \
  static inline sz_t bhvm_hf3_op_ar1_softplus_s_get_arity( const bhvm_hf3_op_ar1_softplus_s* o ){return 1;} \
  static inline void bhvm_hf3_op_ar1_softplus_s_of( const bhvm_hf3_op_ar1_softplus_s* o, const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_softplus_s_f(    a   , r );} \
  static inline void bhvm_hf3_op_ar1_softplus_s_aof( const bhvm_hf3_op_ar1_softplus_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_softplus_s_of( o, a[0], r );}
#define TYPEOF_bhvm_hf3_op_ar1_relu_s 381691786
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_relu_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar1_relu_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar1_relu_s_f( const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_relu_s_fx,       r );} \
  static inline sz_t bhvm_hf3_op_ar1_relu_s_get_arity( const bhvm_hf3_op_ar1_relu_s* o ){return 1;} \
  static inline void bhvm_hf3_op_ar1_relu_s_of( const bhvm_hf3_op_ar1_relu_s* o, const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_relu_s_f(    a   , r );} \
  static inline void bhvm_hf3_op_ar1_relu_s_aof( const bhvm_hf3_op_ar1_relu_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_relu_s_of( o, a[0], r );}
#define TYPEOF_bhvm_hf3_op_ar1_relu_leaky_s 3382374047
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_relu_leaky_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar1_relu_leaky_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar1_relu_leaky_s_f( const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_relu_leaky_s_fx, r );} \
  static inline sz_t bhvm_hf3_op_ar1_relu_leaky_s_get_arity( const bhvm_hf3_op_ar1_relu_leaky_s* o ){return 1;} \
  static inline void bhvm_hf3_op_ar1_relu_leaky_s_of( const bhvm_hf3_op_ar1_relu_leaky_s* o, const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_relu_leaky_s_f(    a   , r );} \
  static inline void bhvm_hf3_op_ar1_relu_leaky_s_aof( const bhvm_hf3_op_ar1_relu_leaky_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_relu_leaky_s_of( o, a[0], r );}
#define TYPEOF_bhvm_hf3_op_ar1_cast_htp_s 3828671232
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_cast_htp_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar1_cast_htp_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar1_cast_htp_s_f( const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_s_cast_htp( a, r );} \
  static inline sz_t bhvm_hf3_op_ar1_cast_htp_s_get_arity( const bhvm_hf3_op_ar1_cast_htp_s* o ){return 1;} \
  static inline void bhvm_hf3_op_ar1_cast_htp_s_of( const bhvm_hf3_op_ar1_cast_htp_s* o, const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_cast_htp_s_f(    a   , r );} \
  static inline void bhvm_hf3_op_ar1_cast_htp_s_aof( const bhvm_hf3_op_ar1_cast_htp_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_cast_htp_s_of( o, a[0], r );}
#define BETH_EXPAND_GROUP_bhvm_hf3_op_ar1 \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar1 ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar1_cpy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar1_neg_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar1_floor_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar1_ceil_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar1_exp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar1_lgst_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar1_lgst_hard_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar1_lgst_leaky_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar1_tanh_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar1_tanh_hard_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar1_tanh_leaky_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar1_softplus_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar1_relu_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar1_relu_leaky_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar1_cast_htp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar1_dp ); \
  typedef void (*bhvm_hf3_op_ar1_of)( const bhvm_hf3_op_ar1* o, const bhvm_hf3_s* a, bhvm_hf3_s* r ); \
  BCORE_DECLARE_SPECT( bhvm_hf3_op_ar1 ) \
  { \
      bcore_spect_header_s header; \
      bhvm_hf3_op_ar1_of of; \
  }; \
  static inline bhvm_hf3_op_ar1* bhvm_hf3_op_ar1_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_bhvm_hf3_op_ar1, t ); return ( bhvm_hf3_op_ar1* )bcore_inst_t_create( t ); } \
  static inline bl_t bhvm_hf3_op_ar1_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_bhvm_hf3_op_ar1 ); } \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bhvm_hf3_op_ar1 ) \
  static inline bl_t bhvm_hf3_op_ar1_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_bhvm_hf3_op_ar1 ); } \
  static inline void bhvm_hf3_op_ar1_p_of( const bhvm_hf3_op_ar1_s* __p, const bhvm_hf3_op_ar1* o, const bhvm_hf3_s* a, bhvm_hf3_s* r ) { assert( __p->of ); __p->of( o, a, r ); } \
  static inline void bhvm_hf3_op_ar1_a_of( const bhvm_hf3_op_ar1* o, const bhvm_hf3_s* a, bhvm_hf3_s* r ) { const bhvm_hf3_op_ar1_s* p = bhvm_hf3_op_ar1_s_get_aware( o ); assert( p->of ); p->of( o, a, r ); } \
  static inline bl_t bhvm_hf3_op_ar1_p_defines_of( const bhvm_hf3_op_ar1_s* __p ) { return __p->of != NULL; } \
  static inline bl_t bhvm_hf3_op_ar1_a_defines_of( const bhvm_hf3_op_ar1* o ) { return bhvm_hf3_op_ar1_s_get_aware( o )->of != NULL; } \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_cpy_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_neg_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_floor_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_ceil_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_exp_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_lgst_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_lgst_hard_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_lgst_leaky_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_tanh_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_tanh_hard_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_tanh_leaky_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_softplus_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_relu_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_relu_leaky_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_cast_htp_s \
  BETH_EXPAND_GROUP_bhvm_hf3_op_ar1_dp

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_op_ar1_dp

#define TYPEOF_bhvm_hf3_op_ar1_dp 1512256210
#define TYPEOF_bhvm_hf3_op_ar1_dp_s 2661300428
#define TYPEOF_bhvm_hf3_op_ar1_dp_ca_cpy_s 1753172160
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_dp_ca_cpy_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar1_dp_ca_cpy_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar1_dp_ca_cpy_s_f( const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_s_add( r, a, r );} \
  static inline sc_t bhvm_hf3_op_ar1_dp_ca_cpy_s_sig( void ){return "vu";} \
  static inline sz_t bhvm_hf3_op_ar1_dp_ca_cpy_s_get_arity( const bhvm_hf3_op_ar1_dp_ca_cpy_s* o ){return 1;} \
  static inline void bhvm_hf3_op_ar1_dp_ca_cpy_s_of( const bhvm_hf3_op_ar1_dp_ca_cpy_s* o, const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_dp_ca_cpy_s_f(    a   , r );} \
  static inline void bhvm_hf3_op_ar1_dp_ca_cpy_s_aof( const bhvm_hf3_op_ar1_dp_ca_cpy_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_dp_ca_cpy_s_of( o, a[0], r );}
#define TYPEOF_bhvm_hf3_op_ar1_dp_ca_neg_s 2092249800
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_dp_ca_neg_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar1_dp_ca_neg_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar1_dp_ca_neg_s_f( const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_s_sub( r, a, r );} \
  static inline sc_t bhvm_hf3_op_ar1_dp_ca_neg_s_sig( void ){return "vu";} \
  static inline sz_t bhvm_hf3_op_ar1_dp_ca_neg_s_get_arity( const bhvm_hf3_op_ar1_dp_ca_neg_s* o ){return 1;} \
  static inline void bhvm_hf3_op_ar1_dp_ca_neg_s_of( const bhvm_hf3_op_ar1_dp_ca_neg_s* o, const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_dp_ca_neg_s_f(    a   , r );} \
  static inline void bhvm_hf3_op_ar1_dp_ca_neg_s_aof( const bhvm_hf3_op_ar1_dp_ca_neg_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_dp_ca_neg_s_of( o, a[0], r );}
#define TYPEOF_bhvm_hf3_op_ar1_dp_ca_add_s 2817658573
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_dp_ca_add_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar1_dp_ca_add_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar1_dp_ca_add_s_f( const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_s_add( r, a, r );} \
  static inline sc_t bhvm_hf3_op_ar1_dp_ca_add_s_sig( void ){return "vu";} \
  static inline sz_t bhvm_hf3_op_ar1_dp_ca_add_s_get_arity( const bhvm_hf3_op_ar1_dp_ca_add_s* o ){return 1;} \
  static inline void bhvm_hf3_op_ar1_dp_ca_add_s_of( const bhvm_hf3_op_ar1_dp_ca_add_s* o, const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_dp_ca_add_s_f(    a   , r );} \
  static inline void bhvm_hf3_op_ar1_dp_ca_add_s_aof( const bhvm_hf3_op_ar1_dp_ca_add_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_dp_ca_add_s_of( o, a[0], r );}
#define TYPEOF_bhvm_hf3_op_ar1_dp_cb_add_s 1036226020
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_dp_cb_add_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar1_dp_cb_add_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar1_dp_cb_add_s_f( const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_s_add( r, a, r );} \
  static inline sc_t bhvm_hf3_op_ar1_dp_cb_add_s_sig( void ){return "vu";} \
  static inline sz_t bhvm_hf3_op_ar1_dp_cb_add_s_get_arity( const bhvm_hf3_op_ar1_dp_cb_add_s* o ){return 1;} \
  static inline void bhvm_hf3_op_ar1_dp_cb_add_s_of( const bhvm_hf3_op_ar1_dp_cb_add_s* o, const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_dp_cb_add_s_f(    a   , r );} \
  static inline void bhvm_hf3_op_ar1_dp_cb_add_s_aof( const bhvm_hf3_op_ar1_dp_cb_add_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_dp_cb_add_s_of( o, a[0], r );}
#define TYPEOF_bhvm_hf3_op_ar1_dp_ca_sub_s 2934330180
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_dp_ca_sub_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar1_dp_ca_sub_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar1_dp_ca_sub_s_f( const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_s_add( r, a, r );} \
  static inline sc_t bhvm_hf3_op_ar1_dp_ca_sub_s_sig( void ){return "vu";} \
  static inline sz_t bhvm_hf3_op_ar1_dp_ca_sub_s_get_arity( const bhvm_hf3_op_ar1_dp_ca_sub_s* o ){return 1;} \
  static inline void bhvm_hf3_op_ar1_dp_ca_sub_s_of( const bhvm_hf3_op_ar1_dp_ca_sub_s* o, const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_dp_ca_sub_s_f(    a   , r );} \
  static inline void bhvm_hf3_op_ar1_dp_ca_sub_s_aof( const bhvm_hf3_op_ar1_dp_ca_sub_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_dp_ca_sub_s_of( o, a[0], r );}
#define TYPEOF_bhvm_hf3_op_ar1_dp_cb_sub_s 3130650133
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_dp_cb_sub_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar1_dp_cb_sub_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar1_dp_cb_sub_s_f( const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_s_sub( r, a, r );} \
  static inline sc_t bhvm_hf3_op_ar1_dp_cb_sub_s_sig( void ){return "vu";} \
  static inline sz_t bhvm_hf3_op_ar1_dp_cb_sub_s_get_arity( const bhvm_hf3_op_ar1_dp_cb_sub_s* o ){return 1;} \
  static inline void bhvm_hf3_op_ar1_dp_cb_sub_s_of( const bhvm_hf3_op_ar1_dp_cb_sub_s* o, const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_dp_cb_sub_s_f(    a   , r );} \
  static inline void bhvm_hf3_op_ar1_dp_cb_sub_s_aof( const bhvm_hf3_op_ar1_dp_cb_sub_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_dp_cb_sub_s_of( o, a[0], r );}
#define TYPEOF_bhvm_hf3_op_ar1_dp_ca_cast_htp_s 3562817030
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_dp_ca_cast_htp_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar1_dp_ca_cast_htp_s ) \
    {aware_t _;}; \
  static inline sc_t bhvm_hf3_op_ar1_dp_ca_cast_htp_s_sig( void ){return "uv";} \
  static inline void bhvm_hf3_op_ar1_dp_ca_cast_htp_s_f( const bhvm_hf3_s* a, bhvm_hf3_s* r ){ASSERT( a->htp != r->htp ); bhvm_hf3_s_cast_htp( a, r );} \
  static inline sz_t bhvm_hf3_op_ar1_dp_ca_cast_htp_s_get_arity( const bhvm_hf3_op_ar1_dp_ca_cast_htp_s* o ){return 1;} \
  static inline void bhvm_hf3_op_ar1_dp_ca_cast_htp_s_of( const bhvm_hf3_op_ar1_dp_ca_cast_htp_s* o, const bhvm_hf3_s* a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_dp_ca_cast_htp_s_f(    a   , r );} \
  static inline void bhvm_hf3_op_ar1_dp_ca_cast_htp_s_aof( const bhvm_hf3_op_ar1_dp_ca_cast_htp_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar1_dp_ca_cast_htp_s_of( o, a[0], r );}
#define BETH_EXPAND_GROUP_bhvm_hf3_op_ar1_dp \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar1_dp ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar1_dp_ca_cpy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar1_dp_ca_neg_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar1_dp_ca_add_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar1_dp_cb_add_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar1_dp_ca_sub_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar1_dp_cb_sub_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar1_dp_ca_cast_htp_s ); \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_dp_ca_cpy_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_dp_ca_neg_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_dp_ca_add_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_dp_cb_add_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_dp_ca_sub_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_dp_cb_sub_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar1_dp_ca_cast_htp_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_op_ar2

#define TYPEOF_bhvm_hf3_op_ar2 1962632052
#define TYPEOF_bhvm_hf3_op_ar2_s 28591462
#define TYPEOF_bhvm_hf3_op_ar2_add_s 2369559474
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_add_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar2_add_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar2_add_s_f( const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_s_add ( a, b, r );} \
  static inline sz_t bhvm_hf3_op_ar2_add_s_get_arity( const bhvm_hf3_op_ar2_add_s* o ){return 2;} \
  static inline void bhvm_hf3_op_ar2_add_s_of( const bhvm_hf3_op_ar2_add_s* o, const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_add_s_f( a, b, r );} \
  static inline void bhvm_hf3_op_ar2_add_s_aof( const bhvm_hf3_op_ar2_add_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_add_s_of( o, a[0], a[1], r );}
#define TYPEOF_bhvm_hf3_op_ar2_sub_s 2900431611
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_sub_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar2_sub_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar2_sub_s_f( const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_s_sub ( a, b, r );} \
  static inline sz_t bhvm_hf3_op_ar2_sub_s_get_arity( const bhvm_hf3_op_ar2_sub_s* o ){return 2;} \
  static inline void bhvm_hf3_op_ar2_sub_s_of( const bhvm_hf3_op_ar2_sub_s* o, const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_sub_s_f( a, b, r );} \
  static inline void bhvm_hf3_op_ar2_sub_s_aof( const bhvm_hf3_op_ar2_sub_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_sub_s_of( o, a[0], a[1], r );}
#define TYPEOF_bhvm_hf3_op_ar2_hmul_s 2796157581
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_hmul_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar2_hmul_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar2_hmul_s_f( const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_s_hmul( a, b, r );} \
  static inline sz_t bhvm_hf3_op_ar2_hmul_s_get_arity( const bhvm_hf3_op_ar2_hmul_s* o ){return 2;} \
  static inline void bhvm_hf3_op_ar2_hmul_s_of( const bhvm_hf3_op_ar2_hmul_s* o, const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_hmul_s_f( a, b, r );} \
  static inline void bhvm_hf3_op_ar2_hmul_s_aof( const bhvm_hf3_op_ar2_hmul_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_hmul_s_of( o, a[0], a[1], r );}
#define TYPEOF_bhvm_hf3_op_ar2_hdiv_s 1245033488
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_hdiv_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar2_hdiv_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar2_hdiv_s_f( const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_s_fp_f3_ar2( a, b, bmath_f3_op_ar2_div_s_fx, r );} \
  static inline sz_t bhvm_hf3_op_ar2_hdiv_s_get_arity( const bhvm_hf3_op_ar2_hdiv_s* o ){return 2;} \
  static inline void bhvm_hf3_op_ar2_hdiv_s_of( const bhvm_hf3_op_ar2_hdiv_s* o, const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_hdiv_s_f( a, b, r );} \
  static inline void bhvm_hf3_op_ar2_hdiv_s_aof( const bhvm_hf3_op_ar2_hdiv_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_hdiv_s_of( o, a[0], a[1], r );}
#define TYPEOF_bhvm_hf3_op_ar2_bmul_s 3903851351
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_bmul_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar2_bmul_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar2_bmul_s_f( const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_s_bmul( a, b, r );} \
  static inline sz_t bhvm_hf3_op_ar2_bmul_s_get_arity( const bhvm_hf3_op_ar2_bmul_s* o ){return 2;} \
  static inline void bhvm_hf3_op_ar2_bmul_s_of( const bhvm_hf3_op_ar2_bmul_s* o, const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_bmul_s_f( a, b, r );} \
  static inline void bhvm_hf3_op_ar2_bmul_s_aof( const bhvm_hf3_op_ar2_bmul_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_bmul_s_of( o, a[0], a[1], r );}
#define TYPEOF_bhvm_hf3_op_ar2_mul_scl_s 609554770
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_mul_scl_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar2_mul_scl_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar2_mul_scl_s_f( const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_s_mul_scl( a, b->v_data, r );} \
  static inline sz_t bhvm_hf3_op_ar2_mul_scl_s_get_arity( const bhvm_hf3_op_ar2_mul_scl_s* o ){return 2;} \
  static inline void bhvm_hf3_op_ar2_mul_scl_s_of( const bhvm_hf3_op_ar2_mul_scl_s* o, const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_mul_scl_s_f( a, b, r );} \
  static inline void bhvm_hf3_op_ar2_mul_scl_s_aof( const bhvm_hf3_op_ar2_mul_scl_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_mul_scl_s_of( o, a[0], a[1], r );}
#define TYPEOF_bhvm_hf3_op_ar2_scl_mul_s 741791070
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_scl_mul_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar2_scl_mul_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar2_scl_mul_s_f( const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_s_mul_scl( b, a->v_data, r );} \
  static inline sz_t bhvm_hf3_op_ar2_scl_mul_s_get_arity( const bhvm_hf3_op_ar2_scl_mul_s* o ){return 2;} \
  static inline void bhvm_hf3_op_ar2_scl_mul_s_of( const bhvm_hf3_op_ar2_scl_mul_s* o, const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_scl_mul_s_f( a, b, r );} \
  static inline void bhvm_hf3_op_ar2_scl_mul_s_aof( const bhvm_hf3_op_ar2_scl_mul_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_scl_mul_s_of( o, a[0], a[1], r );}
#define BETH_EXPAND_GROUP_bhvm_hf3_op_ar2 \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar2 ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar2_add_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar2_sub_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar2_hmul_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar2_hdiv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar2_bmul_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar2_mul_scl_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar2_scl_mul_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar2_dp ); \
  typedef void (*bhvm_hf3_op_ar2_of)( const bhvm_hf3_op_ar2* o, const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ); \
  BCORE_DECLARE_SPECT( bhvm_hf3_op_ar2 ) \
  { \
      bcore_spect_header_s header; \
      bhvm_hf3_op_ar2_of of; \
  }; \
  static inline bhvm_hf3_op_ar2* bhvm_hf3_op_ar2_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_bhvm_hf3_op_ar2, t ); return ( bhvm_hf3_op_ar2* )bcore_inst_t_create( t ); } \
  static inline bl_t bhvm_hf3_op_ar2_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_bhvm_hf3_op_ar2 ); } \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bhvm_hf3_op_ar2 ) \
  static inline bl_t bhvm_hf3_op_ar2_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_bhvm_hf3_op_ar2 ); } \
  static inline void bhvm_hf3_op_ar2_p_of( const bhvm_hf3_op_ar2_s* __p, const bhvm_hf3_op_ar2* o, const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ) { assert( __p->of ); __p->of( o, a, b, r ); } \
  static inline void bhvm_hf3_op_ar2_a_of( const bhvm_hf3_op_ar2* o, const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ) { const bhvm_hf3_op_ar2_s* p = bhvm_hf3_op_ar2_s_get_aware( o ); assert( p->of ); p->of( o, a, b, r ); } \
  static inline bl_t bhvm_hf3_op_ar2_p_defines_of( const bhvm_hf3_op_ar2_s* __p ) { return __p->of != NULL; } \
  static inline bl_t bhvm_hf3_op_ar2_a_defines_of( const bhvm_hf3_op_ar2* o ) { return bhvm_hf3_op_ar2_s_get_aware( o )->of != NULL; } \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_add_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_sub_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_hmul_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_hdiv_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_bmul_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_mul_scl_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_scl_mul_s \
  BETH_EXPAND_GROUP_bhvm_hf3_op_ar2_dp

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_op_ar2_dp

#define TYPEOF_bhvm_hf3_op_ar2_dp 3606592557
#define TYPEOF_bhvm_hf3_op_ar2_dp_s 1820486879
#define TYPEOF_bhvm_hf3_op_ar2_dp_ca_exp_s 499419032
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_dp_ca_exp_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar2_dp_ca_exp_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar2_dp_ca_exp_s_f( const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_s_hmul_add( a, b, r, r );} \
  static inline sc_t bhvm_hf3_op_ar2_dp_ca_exp_s_sig( void ){return "yvu";} \
  static inline sz_t bhvm_hf3_op_ar2_dp_ca_exp_s_get_arity( const bhvm_hf3_op_ar2_dp_ca_exp_s* o ){return 2;} \
  static inline void bhvm_hf3_op_ar2_dp_ca_exp_s_of( const bhvm_hf3_op_ar2_dp_ca_exp_s* o, const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_dp_ca_exp_s_f( a, b, r );} \
  static inline void bhvm_hf3_op_ar2_dp_ca_exp_s_aof( const bhvm_hf3_op_ar2_dp_ca_exp_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_dp_ca_exp_s_of( o, a[0], a[1], r );}
#define TYPEOF_bhvm_hf3_op_ar2_dp_ca_lgst_s 1664038207
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_dp_ca_lgst_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar2_dp_ca_lgst_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar2_dp_ca_lgst_s_f( const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_s_fp_f3_ar1_madd( a, bmath_f3_op_ar1_lgst_s_gy,       b, r );} \
  static inline sc_t bhvm_hf3_op_ar2_dp_ca_lgst_s_sig( void ){return "yvu";} \
  static inline sz_t bhvm_hf3_op_ar2_dp_ca_lgst_s_get_arity( const bhvm_hf3_op_ar2_dp_ca_lgst_s* o ){return 2;} \
  static inline void bhvm_hf3_op_ar2_dp_ca_lgst_s_of( const bhvm_hf3_op_ar2_dp_ca_lgst_s* o, const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_dp_ca_lgst_s_f( a, b, r );} \
  static inline void bhvm_hf3_op_ar2_dp_ca_lgst_s_aof( const bhvm_hf3_op_ar2_dp_ca_lgst_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_dp_ca_lgst_s_of( o, a[0], a[1], r );}
#define TYPEOF_bhvm_hf3_op_ar2_dp_ca_lgst_hard_s 3744039953
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_dp_ca_lgst_hard_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar2_dp_ca_lgst_hard_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar2_dp_ca_lgst_hard_s_f( const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_s_fp_f3_ar1_madd( a, bmath_f3_op_ar1_lgst_hard_s_gy,  b, r );} \
  static inline sc_t bhvm_hf3_op_ar2_dp_ca_lgst_hard_s_sig( void ){return "yvu";} \
  static inline sz_t bhvm_hf3_op_ar2_dp_ca_lgst_hard_s_get_arity( const bhvm_hf3_op_ar2_dp_ca_lgst_hard_s* o ){return 2;} \
  static inline void bhvm_hf3_op_ar2_dp_ca_lgst_hard_s_of( const bhvm_hf3_op_ar2_dp_ca_lgst_hard_s* o, const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_dp_ca_lgst_hard_s_f( a, b, r );} \
  static inline void bhvm_hf3_op_ar2_dp_ca_lgst_hard_s_aof( const bhvm_hf3_op_ar2_dp_ca_lgst_hard_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_dp_ca_lgst_hard_s_of( o, a[0], a[1], r );}
#define TYPEOF_bhvm_hf3_op_ar2_dp_ca_lgst_leaky_s 3117053458
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_dp_ca_lgst_leaky_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar2_dp_ca_lgst_leaky_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar2_dp_ca_lgst_leaky_s_f( const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_s_fp_f3_ar1_madd( a, bmath_f3_op_ar1_lgst_leaky_s_gy, b, r );} \
  static inline sc_t bhvm_hf3_op_ar2_dp_ca_lgst_leaky_s_sig( void ){return "yvu";} \
  static inline sz_t bhvm_hf3_op_ar2_dp_ca_lgst_leaky_s_get_arity( const bhvm_hf3_op_ar2_dp_ca_lgst_leaky_s* o ){return 2;} \
  static inline void bhvm_hf3_op_ar2_dp_ca_lgst_leaky_s_of( const bhvm_hf3_op_ar2_dp_ca_lgst_leaky_s* o, const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_dp_ca_lgst_leaky_s_f( a, b, r );} \
  static inline void bhvm_hf3_op_ar2_dp_ca_lgst_leaky_s_aof( const bhvm_hf3_op_ar2_dp_ca_lgst_leaky_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_dp_ca_lgst_leaky_s_of( o, a[0], a[1], r );}
#define TYPEOF_bhvm_hf3_op_ar2_dp_ca_tanh_s 172907512
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_dp_ca_tanh_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar2_dp_ca_tanh_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar2_dp_ca_tanh_s_f( const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_s_fp_f3_ar1_madd( a, bmath_f3_op_ar1_tanh_s_gy,       b, r );} \
  static inline sc_t bhvm_hf3_op_ar2_dp_ca_tanh_s_sig( void ){return "yvu";} \
  static inline sz_t bhvm_hf3_op_ar2_dp_ca_tanh_s_get_arity( const bhvm_hf3_op_ar2_dp_ca_tanh_s* o ){return 2;} \
  static inline void bhvm_hf3_op_ar2_dp_ca_tanh_s_of( const bhvm_hf3_op_ar2_dp_ca_tanh_s* o, const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_dp_ca_tanh_s_f( a, b, r );} \
  static inline void bhvm_hf3_op_ar2_dp_ca_tanh_s_aof( const bhvm_hf3_op_ar2_dp_ca_tanh_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_dp_ca_tanh_s_of( o, a[0], a[1], r );}
#define TYPEOF_bhvm_hf3_op_ar2_dp_ca_tanh_hard_s 771311408
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_dp_ca_tanh_hard_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar2_dp_ca_tanh_hard_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar2_dp_ca_tanh_hard_s_f( const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_s_fp_f3_ar1_madd( a, bmath_f3_op_ar1_tanh_hard_s_gy,  b, r );} \
  static inline sc_t bhvm_hf3_op_ar2_dp_ca_tanh_hard_s_sig( void ){return "yvu";} \
  static inline sz_t bhvm_hf3_op_ar2_dp_ca_tanh_hard_s_get_arity( const bhvm_hf3_op_ar2_dp_ca_tanh_hard_s* o ){return 2;} \
  static inline void bhvm_hf3_op_ar2_dp_ca_tanh_hard_s_of( const bhvm_hf3_op_ar2_dp_ca_tanh_hard_s* o, const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_dp_ca_tanh_hard_s_f( a, b, r );} \
  static inline void bhvm_hf3_op_ar2_dp_ca_tanh_hard_s_aof( const bhvm_hf3_op_ar2_dp_ca_tanh_hard_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_dp_ca_tanh_hard_s_of( o, a[0], a[1], r );}
#define TYPEOF_bhvm_hf3_op_ar2_dp_ca_tanh_leaky_s 4206769301
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_dp_ca_tanh_leaky_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar2_dp_ca_tanh_leaky_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar2_dp_ca_tanh_leaky_s_f( const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_s_fp_f3_ar1_madd( a, bmath_f3_op_ar1_tanh_leaky_s_gy, b, r );} \
  static inline sc_t bhvm_hf3_op_ar2_dp_ca_tanh_leaky_s_sig( void ){return "yvu";} \
  static inline sz_t bhvm_hf3_op_ar2_dp_ca_tanh_leaky_s_get_arity( const bhvm_hf3_op_ar2_dp_ca_tanh_leaky_s* o ){return 2;} \
  static inline void bhvm_hf3_op_ar2_dp_ca_tanh_leaky_s_of( const bhvm_hf3_op_ar2_dp_ca_tanh_leaky_s* o, const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_dp_ca_tanh_leaky_s_f( a, b, r );} \
  static inline void bhvm_hf3_op_ar2_dp_ca_tanh_leaky_s_aof( const bhvm_hf3_op_ar2_dp_ca_tanh_leaky_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_dp_ca_tanh_leaky_s_of( o, a[0], a[1], r );}
#define TYPEOF_bhvm_hf3_op_ar2_dp_ca_softplus_s 3097492913
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_dp_ca_softplus_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar2_dp_ca_softplus_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar2_dp_ca_softplus_s_f( const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_s_fp_f3_ar1_madd( a, bmath_f3_op_ar1_softplus_s_gy,   b, r );} \
  static inline sc_t bhvm_hf3_op_ar2_dp_ca_softplus_s_sig( void ){return "yvu";} \
  static inline sz_t bhvm_hf3_op_ar2_dp_ca_softplus_s_get_arity( const bhvm_hf3_op_ar2_dp_ca_softplus_s* o ){return 2;} \
  static inline void bhvm_hf3_op_ar2_dp_ca_softplus_s_of( const bhvm_hf3_op_ar2_dp_ca_softplus_s* o, const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_dp_ca_softplus_s_f( a, b, r );} \
  static inline void bhvm_hf3_op_ar2_dp_ca_softplus_s_aof( const bhvm_hf3_op_ar2_dp_ca_softplus_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_dp_ca_softplus_s_of( o, a[0], a[1], r );}
#define TYPEOF_bhvm_hf3_op_ar2_dp_ca_relu_s 2949290875
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_dp_ca_relu_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar2_dp_ca_relu_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar2_dp_ca_relu_s_f( const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_s_fp_f3_ar1_madd( a, bmath_f3_op_ar1_relu_s_gy,       b, r );} \
  static inline sc_t bhvm_hf3_op_ar2_dp_ca_relu_s_sig( void ){return "yvu";} \
  static inline sz_t bhvm_hf3_op_ar2_dp_ca_relu_s_get_arity( const bhvm_hf3_op_ar2_dp_ca_relu_s* o ){return 2;} \
  static inline void bhvm_hf3_op_ar2_dp_ca_relu_s_of( const bhvm_hf3_op_ar2_dp_ca_relu_s* o, const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_dp_ca_relu_s_f( a, b, r );} \
  static inline void bhvm_hf3_op_ar2_dp_ca_relu_s_aof( const bhvm_hf3_op_ar2_dp_ca_relu_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_dp_ca_relu_s_of( o, a[0], a[1], r );}
#define TYPEOF_bhvm_hf3_op_ar2_dp_ca_relu_leaky_s 1640622390
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_dp_ca_relu_leaky_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar2_dp_ca_relu_leaky_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar2_dp_ca_relu_leaky_s_f( const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_s_fp_f3_ar1_madd( a, bmath_f3_op_ar1_relu_leaky_s_gy, b, r );} \
  static inline sc_t bhvm_hf3_op_ar2_dp_ca_relu_leaky_s_sig( void ){return "yvu";} \
  static inline sz_t bhvm_hf3_op_ar2_dp_ca_relu_leaky_s_get_arity( const bhvm_hf3_op_ar2_dp_ca_relu_leaky_s* o ){return 2;} \
  static inline void bhvm_hf3_op_ar2_dp_ca_relu_leaky_s_of( const bhvm_hf3_op_ar2_dp_ca_relu_leaky_s* o, const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_dp_ca_relu_leaky_s_f( a, b, r );} \
  static inline void bhvm_hf3_op_ar2_dp_ca_relu_leaky_s_aof( const bhvm_hf3_op_ar2_dp_ca_relu_leaky_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_dp_ca_relu_leaky_s_of( o, a[0], a[1], r );}
#define TYPEOF_bhvm_hf3_op_ar2_dp_ca_hmul_s 2667674923
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_dp_ca_hmul_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar2_dp_ca_hmul_s ) \
    {aware_t _;}; \
  static inline sc_t bhvm_hf3_op_ar2_dp_ca_hmul_s_sig( void ){return "vbu";} \
  static inline void bhvm_hf3_op_ar2_dp_ca_hmul_s_f( const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_s_hmul_add( a, b, r, r );} \
  static inline sz_t bhvm_hf3_op_ar2_dp_ca_hmul_s_get_arity( const bhvm_hf3_op_ar2_dp_ca_hmul_s* o ){return 2;} \
  static inline void bhvm_hf3_op_ar2_dp_ca_hmul_s_of( const bhvm_hf3_op_ar2_dp_ca_hmul_s* o, const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_dp_ca_hmul_s_f( a, b, r );} \
  static inline void bhvm_hf3_op_ar2_dp_ca_hmul_s_aof( const bhvm_hf3_op_ar2_dp_ca_hmul_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_dp_ca_hmul_s_of( o, a[0], a[1], r );}
#define TYPEOF_bhvm_hf3_op_ar2_dp_cb_hmul_s 2509894448
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_dp_cb_hmul_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar2_dp_cb_hmul_s ) \
    {aware_t _;}; \
  static inline sc_t bhvm_hf3_op_ar2_dp_cb_hmul_s_sig( void ){return "avu";} \
  static inline void bhvm_hf3_op_ar2_dp_cb_hmul_s_f( const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_s_hmul_add( a, b, r, r );} \
  static inline sz_t bhvm_hf3_op_ar2_dp_cb_hmul_s_get_arity( const bhvm_hf3_op_ar2_dp_cb_hmul_s* o ){return 2;} \
  static inline void bhvm_hf3_op_ar2_dp_cb_hmul_s_of( const bhvm_hf3_op_ar2_dp_cb_hmul_s* o, const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_dp_cb_hmul_s_f( a, b, r );} \
  static inline void bhvm_hf3_op_ar2_dp_cb_hmul_s_aof( const bhvm_hf3_op_ar2_dp_cb_hmul_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_dp_cb_hmul_s_of( o, a[0], a[1], r );}
#define TYPEOF_bhvm_hf3_op_ar2_dp_ca_bmul_s 3092648689
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_dp_ca_bmul_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar2_dp_ca_bmul_s ) \
    {aware_t _;}; \
  static inline sc_t bhvm_hf3_op_ar2_dp_ca_bmul_s_sig( void ){return "vbu";} \
  static inline void bhvm_hf3_op_ar2_dp_ca_bmul_s_f( const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_s bt = bhvm_hf3_s_get_weak_htp( b ); bhvm_hf3_s_bmul_add( a, &bt, r, r );} \
  static inline sz_t bhvm_hf3_op_ar2_dp_ca_bmul_s_get_arity( const bhvm_hf3_op_ar2_dp_ca_bmul_s* o ){return 2;} \
  static inline void bhvm_hf3_op_ar2_dp_ca_bmul_s_of( const bhvm_hf3_op_ar2_dp_ca_bmul_s* o, const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_dp_ca_bmul_s_f( a, b, r );} \
  static inline void bhvm_hf3_op_ar2_dp_ca_bmul_s_aof( const bhvm_hf3_op_ar2_dp_ca_bmul_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_dp_ca_bmul_s_of( o, a[0], a[1], r );}
#define TYPEOF_bhvm_hf3_op_ar2_dp_cb_bmul_s 1427683426
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_dp_cb_bmul_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar2_dp_cb_bmul_s ) \
    {aware_t _;}; \
  static inline sc_t bhvm_hf3_op_ar2_dp_cb_bmul_s_sig( void ){return "avu";} \
  static inline void bhvm_hf3_op_ar2_dp_cb_bmul_s_f( const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_s at = bhvm_hf3_s_get_weak_htp( a ); bhvm_hf3_s_bmul_add( &at, b, r, r );} \
  static inline sz_t bhvm_hf3_op_ar2_dp_cb_bmul_s_get_arity( const bhvm_hf3_op_ar2_dp_cb_bmul_s* o ){return 2;} \
  static inline void bhvm_hf3_op_ar2_dp_cb_bmul_s_of( const bhvm_hf3_op_ar2_dp_cb_bmul_s* o, const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_dp_cb_bmul_s_f( a, b, r );} \
  static inline void bhvm_hf3_op_ar2_dp_cb_bmul_s_aof( const bhvm_hf3_op_ar2_dp_cb_bmul_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_dp_cb_bmul_s_of( o, a[0], a[1], r );}
#define TYPEOF_bhvm_hf3_op_ar2_dp_ca_mul_scl_s 628614312
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_dp_ca_mul_scl_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar2_dp_ca_mul_scl_s ) \
    {aware_t _;}; \
  static inline sc_t bhvm_hf3_op_ar2_dp_ca_mul_scl_s_sig( void ){return "vbu";} \
  static inline void bhvm_hf3_op_ar2_dp_ca_mul_scl_s_f( const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_s_mul_scl_f3_add( a, b->v_data[ 0 ], r, r );} \
  static inline sz_t bhvm_hf3_op_ar2_dp_ca_mul_scl_s_get_arity( const bhvm_hf3_op_ar2_dp_ca_mul_scl_s* o ){return 2;} \
  static inline void bhvm_hf3_op_ar2_dp_ca_mul_scl_s_of( const bhvm_hf3_op_ar2_dp_ca_mul_scl_s* o, const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_dp_ca_mul_scl_s_f( a, b, r );} \
  static inline void bhvm_hf3_op_ar2_dp_ca_mul_scl_s_aof( const bhvm_hf3_op_ar2_dp_ca_mul_scl_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_dp_ca_mul_scl_s_of( o, a[0], a[1], r );}
#define TYPEOF_bhvm_hf3_op_ar2_dp_cb_mul_scl_s 4048853137
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_dp_cb_mul_scl_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar2_dp_cb_mul_scl_s ) \
    {aware_t _;}; \
  static inline sc_t bhvm_hf3_op_ar2_dp_cb_mul_scl_s_sig( void ){return "avu";} \
  static inline void bhvm_hf3_op_ar2_dp_cb_mul_scl_s_f( const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){r->v_data[ 0 ] += bhvm_hf3_s_f3_vec_mul_vec( a, b );} \
  static inline sz_t bhvm_hf3_op_ar2_dp_cb_mul_scl_s_get_arity( const bhvm_hf3_op_ar2_dp_cb_mul_scl_s* o ){return 2;} \
  static inline void bhvm_hf3_op_ar2_dp_cb_mul_scl_s_of( const bhvm_hf3_op_ar2_dp_cb_mul_scl_s* o, const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_dp_cb_mul_scl_s_f( a, b, r );} \
  static inline void bhvm_hf3_op_ar2_dp_cb_mul_scl_s_aof( const bhvm_hf3_op_ar2_dp_cb_mul_scl_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_dp_cb_mul_scl_s_of( o, a[0], a[1], r );}
#define TYPEOF_bhvm_hf3_op_ar2_dp_ca_scl_mul_s 2858168324
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_dp_ca_scl_mul_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar2_dp_ca_scl_mul_s ) \
    {aware_t _;}; \
  static inline sc_t bhvm_hf3_op_ar2_dp_ca_scl_mul_s_sig( void ){return "vbu";} \
  static inline void bhvm_hf3_op_ar2_dp_ca_scl_mul_s_f( const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){r->v_data[ 0 ] += bhvm_hf3_s_f3_vec_mul_vec( a, b );} \
  static inline sz_t bhvm_hf3_op_ar2_dp_ca_scl_mul_s_get_arity( const bhvm_hf3_op_ar2_dp_ca_scl_mul_s* o ){return 2;} \
  static inline void bhvm_hf3_op_ar2_dp_ca_scl_mul_s_of( const bhvm_hf3_op_ar2_dp_ca_scl_mul_s* o, const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_dp_ca_scl_mul_s_f( a, b, r );} \
  static inline void bhvm_hf3_op_ar2_dp_ca_scl_mul_s_aof( const bhvm_hf3_op_ar2_dp_ca_scl_mul_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_dp_ca_scl_mul_s_of( o, a[0], a[1], r );}
#define TYPEOF_bhvm_hf3_op_ar2_dp_cb_scl_mul_s 2962770133
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_dp_cb_scl_mul_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar2_dp_cb_scl_mul_s ) \
    {aware_t _;}; \
  static inline sc_t bhvm_hf3_op_ar2_dp_cb_scl_mul_s_sig( void ){return "avu";} \
  static inline void bhvm_hf3_op_ar2_dp_cb_scl_mul_s_f( const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_s_mul_scl_f3_add( b, a->v_data[ 0 ], r, r );} \
  static inline sz_t bhvm_hf3_op_ar2_dp_cb_scl_mul_s_get_arity( const bhvm_hf3_op_ar2_dp_cb_scl_mul_s* o ){return 2;} \
  static inline void bhvm_hf3_op_ar2_dp_cb_scl_mul_s_of( const bhvm_hf3_op_ar2_dp_cb_scl_mul_s* o, const bhvm_hf3_s* a, const bhvm_hf3_s* b, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_dp_cb_scl_mul_s_f( a, b, r );} \
  static inline void bhvm_hf3_op_ar2_dp_cb_scl_mul_s_aof( const bhvm_hf3_op_ar2_dp_cb_scl_mul_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar2_dp_cb_scl_mul_s_of( o, a[0], a[1], r );}
#define BETH_EXPAND_GROUP_bhvm_hf3_op_ar2_dp \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar2_dp ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar2_dp_ca_exp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar2_dp_ca_lgst_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar2_dp_ca_lgst_hard_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar2_dp_ca_lgst_leaky_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar2_dp_ca_tanh_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar2_dp_ca_tanh_hard_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar2_dp_ca_tanh_leaky_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar2_dp_ca_softplus_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar2_dp_ca_relu_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar2_dp_ca_relu_leaky_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar2_dp_ca_hmul_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar2_dp_cb_hmul_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar2_dp_ca_bmul_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar2_dp_cb_bmul_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar2_dp_ca_mul_scl_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar2_dp_cb_mul_scl_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar2_dp_ca_scl_mul_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar2_dp_cb_scl_mul_s ); \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_dp_ca_exp_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_dp_ca_lgst_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_dp_ca_lgst_hard_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_dp_ca_lgst_leaky_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_dp_ca_tanh_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_dp_ca_tanh_hard_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_dp_ca_tanh_leaky_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_dp_ca_softplus_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_dp_ca_relu_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_dp_ca_relu_leaky_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_dp_ca_hmul_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_dp_cb_hmul_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_dp_ca_bmul_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_dp_cb_bmul_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_dp_ca_mul_scl_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_dp_cb_mul_scl_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_dp_ca_scl_mul_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar2_dp_cb_scl_mul_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_op_ar3

#define TYPEOF_bhvm_hf3_op_ar3 1979409671
#define TYPEOF_bhvm_hf3_op_ar3_s 2149135905
#define BETH_EXPAND_GROUP_bhvm_hf3_op_ar3 \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar3 ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar3_dp ); \
  typedef void (*bhvm_hf3_op_ar3_of)( const bhvm_hf3_op_ar3* o, const bhvm_hf3_s* a, const bhvm_hf3_s* b, const bhvm_hf3_s* c, bhvm_hf3_s* r ); \
  BCORE_DECLARE_SPECT( bhvm_hf3_op_ar3 ) \
  { \
      bcore_spect_header_s header; \
      bhvm_hf3_op_ar3_of of; \
  }; \
  static inline bhvm_hf3_op_ar3* bhvm_hf3_op_ar3_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_bhvm_hf3_op_ar3, t ); return ( bhvm_hf3_op_ar3* )bcore_inst_t_create( t ); } \
  static inline bl_t bhvm_hf3_op_ar3_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_bhvm_hf3_op_ar3 ); } \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bhvm_hf3_op_ar3 ) \
  static inline bl_t bhvm_hf3_op_ar3_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_bhvm_hf3_op_ar3 ); } \
  static inline void bhvm_hf3_op_ar3_p_of( const bhvm_hf3_op_ar3_s* __p, const bhvm_hf3_op_ar3* o, const bhvm_hf3_s* a, const bhvm_hf3_s* b, const bhvm_hf3_s* c, bhvm_hf3_s* r ) { assert( __p->of ); __p->of( o, a, b, c, r ); } \
  static inline void bhvm_hf3_op_ar3_a_of( const bhvm_hf3_op_ar3* o, const bhvm_hf3_s* a, const bhvm_hf3_s* b, const bhvm_hf3_s* c, bhvm_hf3_s* r ) { const bhvm_hf3_op_ar3_s* p = bhvm_hf3_op_ar3_s_get_aware( o ); assert( p->of ); p->of( o, a, b, c, r ); } \
  static inline bl_t bhvm_hf3_op_ar3_p_defines_of( const bhvm_hf3_op_ar3_s* __p ) { return __p->of != NULL; } \
  static inline bl_t bhvm_hf3_op_ar3_a_defines_of( const bhvm_hf3_op_ar3* o ) { return bhvm_hf3_op_ar3_s_get_aware( o )->of != NULL; } \
  BETH_EXPAND_GROUP_bhvm_hf3_op_ar3_dp

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_op_ar3_dp

#define TYPEOF_bhvm_hf3_op_ar3_dp 4270570012
#define TYPEOF_bhvm_hf3_op_ar3_dp_s 4120421566
#define TYPEOF_bhvm_hf3_op_ar3_dp_ca_hdiv_s 2407520399
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar3_dp_ca_hdiv_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar3_dp_ca_hdiv_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar3_dp_ca_hdiv_s_f( const bhvm_hf3_s* a, const bhvm_hf3_s* b, const bhvm_hf3_s* c, bhvm_hf3_s* r ){bhvm_hf3_s_fp_f3_ar2_madd( a, b, bmath_f3_op_ar2_div_s_gxa, c, r );} \
  static inline sc_t bhvm_hf3_op_ar3_dp_ca_hdiv_s_sig( void ){return "abvu";} \
  static inline sz_t bhvm_hf3_op_ar3_dp_ca_hdiv_s_get_arity( const bhvm_hf3_op_ar3_dp_ca_hdiv_s* o ){return 3;} \
  static inline void bhvm_hf3_op_ar3_dp_ca_hdiv_s_of( const bhvm_hf3_op_ar3_dp_ca_hdiv_s* o, const bhvm_hf3_s* a, const bhvm_hf3_s* b, const bhvm_hf3_s* c, bhvm_hf3_s* r ){bhvm_hf3_op_ar3_dp_ca_hdiv_s_f( a, b, c, r );} \
  static inline void bhvm_hf3_op_ar3_dp_ca_hdiv_s_aof( const bhvm_hf3_op_ar3_dp_ca_hdiv_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar3_dp_ca_hdiv_s_of( o, a[0], a[1], a[2], r );}
#define TYPEOF_bhvm_hf3_op_ar3_dp_cb_hdiv_s 3297879648
#define BETH_EXPAND_ITEM_bhvm_hf3_op_ar3_dp_cb_hdiv_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_op_ar3_dp_cb_hdiv_s ) \
    {aware_t _;}; \
  static inline void bhvm_hf3_op_ar3_dp_cb_hdiv_s_f( const bhvm_hf3_s* a, const bhvm_hf3_s* b, const bhvm_hf3_s* c, bhvm_hf3_s* r ){bhvm_hf3_s_fp_f3_ar2_madd( a, b, bmath_f3_op_ar2_div_s_gxb, c, r );} \
  static inline sc_t bhvm_hf3_op_ar3_dp_cb_hdiv_s_sig( void ){return "abvu";} \
  static inline sz_t bhvm_hf3_op_ar3_dp_cb_hdiv_s_get_arity( const bhvm_hf3_op_ar3_dp_cb_hdiv_s* o ){return 3;} \
  static inline void bhvm_hf3_op_ar3_dp_cb_hdiv_s_of( const bhvm_hf3_op_ar3_dp_cb_hdiv_s* o, const bhvm_hf3_s* a, const bhvm_hf3_s* b, const bhvm_hf3_s* c, bhvm_hf3_s* r ){bhvm_hf3_op_ar3_dp_cb_hdiv_s_f( a, b, c, r );} \
  static inline void bhvm_hf3_op_ar3_dp_cb_hdiv_s_aof( const bhvm_hf3_op_ar3_dp_cb_hdiv_s* o, const bhvm_hf3_s** a, bhvm_hf3_s* r ){bhvm_hf3_op_ar3_dp_cb_hdiv_s_of( o, a[0], a[1], a[2], r );}
#define BETH_EXPAND_GROUP_bhvm_hf3_op_ar3_dp \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar3_dp ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar3_dp_ca_hdiv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_op_ar3_dp_cb_hdiv_s ); \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar3_dp_ca_hdiv_s \
  BETH_EXPAND_ITEM_bhvm_hf3_op_ar3_dp_cb_hdiv_s

/**********************************************************************************************************************/
// source: bhvm_hf3_vm.h

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_vm

#define TYPEOF_bhvm_hf3_vm 3671691430
#define TYPEOF_bhvm_hf3_vm_s 390359832
#define TYPEOF_bhvm_hf3_vm_hparam_s 3257174776
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_hparam_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_hparam_s ) \
    {aware_t _;tp_t name;tp_t type;sz_t idx_paired;};
#define TYPEOF_bhvm_hf3_vm_holor_s 3516685217
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_holor_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_holor_s ) \
    {aware_t _;bhvm_hf3_vm_hparam_s p;bhvm_hf3_s h;};
#define TYPEOF_bhvm_hf3_vm_arr_holor_s 2636035365
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_arr_holor_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_arr_holor_s ) \
    {aware_t _;BCORE_ARRAY_DYN_SOLID_STATIC_S( bhvm_hf3_vm_holor_s, );}; \
  static inline void bhvm_hf3_vm_arr_holor_s_set_space( bhvm_hf3_vm_arr_holor_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_bhvm_hf3_vm_arr_holor_s, ( bcore_array* )o, size ); } \
  static inline void bhvm_hf3_vm_arr_holor_s_set_size( bhvm_hf3_vm_arr_holor_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_bhvm_hf3_vm_arr_holor_s, ( bcore_array* )o, size ); } \
  static inline void bhvm_hf3_vm_arr_holor_s_clear( bhvm_hf3_vm_arr_holor_s* o ) { bcore_array_t_set_space( TYPEOF_bhvm_hf3_vm_arr_holor_s, ( bcore_array* )o, 0 ); } \
  static inline bhvm_hf3_vm_holor_s* bhvm_hf3_vm_arr_holor_s_push_c( bhvm_hf3_vm_arr_holor_s* o, const bhvm_hf3_vm_holor_s* v ) { bcore_array_t_push( TYPEOF_bhvm_hf3_vm_arr_holor_s, ( bcore_array* )o, sr_twc( TYPEOF_bhvm_hf3_vm_holor_s, v ) ); return &o->data[ o->size - 1 ]; } \
  static inline bhvm_hf3_vm_holor_s* bhvm_hf3_vm_arr_holor_s_push_d( bhvm_hf3_vm_arr_holor_s* o,       bhvm_hf3_vm_holor_s* v ) { bcore_array_t_push( TYPEOF_bhvm_hf3_vm_arr_holor_s, ( bcore_array* )o, sr_tsd( TYPEOF_bhvm_hf3_vm_holor_s, v ) ); return &o->data[ o->size - 1 ]; } \
  static inline bhvm_hf3_vm_holor_s* bhvm_hf3_vm_arr_holor_s_push( bhvm_hf3_vm_arr_holor_s* o ) \
  { \
      bcore_array_t_push( TYPEOF_bhvm_hf3_vm_arr_holor_s, ( bcore_array* )o, sr_null() ); \
      return &o->data[ o->size - 1 ]; \
  }
#define TYPEOF_bhvm_hf3_vm_mop_s 3650793127
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_mop_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_mop_s ) \
    {aware_t _;bhvm_hf3_vm_op* op;bhvm_hf3_vm_s* p;}; \
  static inline void bhvm_hf3_vm_mop_s_copy_x( bhvm_hf3_vm_mop_s* o ){o->p = o->op ? (bhvm_hf3_vm_s*)bhvm_hf3_vm_s_get_aware( o->op ) : NULL;} \
  static inline void bhvm_hf3_vm_mop_s_mutated( bhvm_hf3_vm_mop_s* o ){bhvm_hf3_vm_mop_s_copy_x( o );} \
  static inline void bhvm_hf3_vm_mop_s_mrun( const bhvm_hf3_vm_mop_s* o, bhvm_hf3_vm_holor_s* ah ){assert( o->p ); assert( o->p->run ); o->p->run( (vc_t)o->op, ah );}
#define TYPEOF_bhvm_hf3_vm_mcode_s 1683505657
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_mcode_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_mcode_s ) \
    {aware_t _;tp_t name;BCORE_ARRAY_DYN_SOLID_STATIC_S( bhvm_hf3_vm_mop_s, );}; \
  static inline void bhvm_hf3_vm_mcode_s_mrun( const bhvm_hf3_vm_mcode_s* o, bhvm_hf3_vm_holor_s* ah ){BFOR_EACH( i, o ) bhvm_hf3_vm_mop_s_mrun( &o->data[ i ], ah );} \
  void bhvm_hf3_vm_mcode_s_op_push_d( bhvm_hf3_vm_mcode_s* o, bhvm_hf3_vm_op* op ); \
  void bhvm_hf3_vm_mcode_s_op_push_c( bhvm_hf3_vm_mcode_s* o, const bhvm_hf3_vm_op* op ); \
  static inline void bhvm_hf3_vm_mcode_s_set_space( bhvm_hf3_vm_mcode_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_bhvm_hf3_vm_mcode_s, ( bcore_array* )o, size ); } \
  static inline void bhvm_hf3_vm_mcode_s_set_size( bhvm_hf3_vm_mcode_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_bhvm_hf3_vm_mcode_s, ( bcore_array* )o, size ); } \
  static inline void bhvm_hf3_vm_mcode_s_clear( bhvm_hf3_vm_mcode_s* o ) { bcore_array_t_set_space( TYPEOF_bhvm_hf3_vm_mcode_s, ( bcore_array* )o, 0 ); } \
  static inline bhvm_hf3_vm_mop_s* bhvm_hf3_vm_mcode_s_push_c( bhvm_hf3_vm_mcode_s* o, const bhvm_hf3_vm_mop_s* v ) { bcore_array_t_push( TYPEOF_bhvm_hf3_vm_mcode_s, ( bcore_array* )o, sr_twc( TYPEOF_bhvm_hf3_vm_mop_s, v ) ); return &o->data[ o->size - 1 ]; } \
  static inline bhvm_hf3_vm_mop_s* bhvm_hf3_vm_mcode_s_push_d( bhvm_hf3_vm_mcode_s* o,       bhvm_hf3_vm_mop_s* v ) { bcore_array_t_push( TYPEOF_bhvm_hf3_vm_mcode_s, ( bcore_array* )o, sr_tsd( TYPEOF_bhvm_hf3_vm_mop_s, v ) ); return &o->data[ o->size - 1 ]; } \
  static inline bhvm_hf3_vm_mop_s* bhvm_hf3_vm_mcode_s_push( bhvm_hf3_vm_mcode_s* o ) \
  { \
      bcore_array_t_push( TYPEOF_bhvm_hf3_vm_mcode_s, ( bcore_array* )o, sr_null() ); \
      return &o->data[ o->size - 1 ]; \
  }
#define TYPEOF_bhvm_hf3_vm_arr_mcode_s 3871377749
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_arr_mcode_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_arr_mcode_s ) \
    {aware_t _;BCORE_ARRAY_DYN_LINK_STATIC_S( bhvm_hf3_vm_mcode_s, );}; \
  static inline void bhvm_hf3_vm_arr_mcode_s_set_space( bhvm_hf3_vm_arr_mcode_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_bhvm_hf3_vm_arr_mcode_s, ( bcore_array* )o, size ); } \
  static inline void bhvm_hf3_vm_arr_mcode_s_set_size( bhvm_hf3_vm_arr_mcode_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_bhvm_hf3_vm_arr_mcode_s, ( bcore_array* )o, size ); } \
  static inline void bhvm_hf3_vm_arr_mcode_s_clear( bhvm_hf3_vm_arr_mcode_s* o ) { bcore_array_t_set_space( TYPEOF_bhvm_hf3_vm_arr_mcode_s, ( bcore_array* )o, 0 ); } \
  static inline bhvm_hf3_vm_mcode_s* bhvm_hf3_vm_arr_mcode_s_push_c( bhvm_hf3_vm_arr_mcode_s* o, const bhvm_hf3_vm_mcode_s* v ) { bcore_array_t_push( TYPEOF_bhvm_hf3_vm_arr_mcode_s, ( bcore_array* )o, sr_twc( TYPEOF_bhvm_hf3_vm_mcode_s, v ) ); return o->data[ o->size - 1 ]; } \
  static inline bhvm_hf3_vm_mcode_s* bhvm_hf3_vm_arr_mcode_s_push_d( bhvm_hf3_vm_arr_mcode_s* o,       bhvm_hf3_vm_mcode_s* v ) { bcore_array_t_push( TYPEOF_bhvm_hf3_vm_arr_mcode_s, ( bcore_array* )o, sr_tsd( TYPEOF_bhvm_hf3_vm_mcode_s, v ) ); return o->data[ o->size - 1 ]; } \
  static inline bhvm_hf3_vm_mcode_s* bhvm_hf3_vm_arr_mcode_s_push( bhvm_hf3_vm_arr_mcode_s* o ) \
  { \
      bcore_array_t_push( TYPEOF_bhvm_hf3_vm_arr_mcode_s, ( bcore_array* )o, sr_t_create( TYPEOF_bhvm_hf3_vm_mcode_s ) ); \
      return o->data[ o->size - 1 ]; \
  }
#define TYPEOF_bhvm_hf3_vm_lib_mcode_s 259873893
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_lib_mcode_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_lib_mcode_s ) \
    {aware_t _;bhvm_hf3_vm_arr_mcode_s arr;bcore_hmap_tpuz_s map;}; \
  static inline void bhvm_hf3_vm_lib_mcode_s_clear( bhvm_hf3_vm_lib_mcode_s* o ){bhvm_hf3_vm_arr_mcode_s_clear( &o->arr ); bcore_hmap_tpuz_s_clear( &o->map );} \
  static inline bl_t bhvm_hf3_vm_lib_mcode_s_mcode_exists( const bhvm_hf3_vm_lib_mcode_s* o, tp_t name ){return bcore_hmap_tpuz_s_exists( &o->map, name );} \
  bhvm_hf3_vm_mcode_s* bhvm_hf3_vm_lib_mcode_s_mcode_get( bhvm_hf3_vm_lib_mcode_s* o, tp_t name ); \
  bhvm_hf3_vm_mcode_s* bhvm_hf3_vm_lib_mcode_s_mcode_get_or_new( bhvm_hf3_vm_lib_mcode_s* o, tp_t name ); \
  bhvm_hf3_vm_mcode_s* bhvm_hf3_vm_lib_mcode_s_mcode_reset( bhvm_hf3_vm_lib_mcode_s* o, tp_t name ); \
  static inline void bhvm_hf3_vm_lib_mcode_s_mcode_op_push_c( bhvm_hf3_vm_lib_mcode_s* o, tp_t name, const bhvm_hf3_vm_op* op ){bhvm_hf3_vm_mcode_s_op_push_c( bhvm_hf3_vm_lib_mcode_s_mcode_get_or_new( o, name ), op );} \
  static inline void bhvm_hf3_vm_lib_mcode_s_mcode_op_push_d( bhvm_hf3_vm_lib_mcode_s* o, tp_t name, bhvm_hf3_vm_op* op ){bhvm_hf3_vm_mcode_s_op_push_d( bhvm_hf3_vm_lib_mcode_s_mcode_get_or_new( o, name ), op );} \
  void bhvm_hf3_vm_lib_mcode_s_mcode_push( bhvm_hf3_vm_lib_mcode_s* o, tp_t name, tp_t src_name ); \
  void bhvm_hf3_vm_lib_mcode_s_mcode_push_reverse( bhvm_hf3_vm_lib_mcode_s* o, tp_t name, tp_t src_name ); \
  void bhvm_hf3_vm_lib_mcode_s_mcode_remove( bhvm_hf3_vm_lib_mcode_s* o, tp_t name ); \
  static inline void bhvm_hf3_vm_lib_mcode_s_mcode_mrun( bhvm_hf3_vm_lib_mcode_s* o, tp_t name, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_vm_mcode_s* mc = bhvm_hf3_vm_lib_mcode_s_mcode_get( o, name ); if( mc ) bhvm_hf3_vm_mcode_s_mrun( mc, ah );}
#define TYPEOF_bhvm_hf3_vm_thread_s 3180417685
#define TYPEOF_bhvm_hf3_vm_pi 297524904
#define TYPEOF_bhvm_hf3_vm_fpi_s 3143578054
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_fpi_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_fpi_s ) \
    {aware_t _;bhvm_hf3_vm_pi* pi;bhvm_hf3_vm_s* p;}; \
  static inline void bhvm_hf3_vm_fpi_s_copy_x( bhvm_hf3_vm_fpi_s* o ){o->p = o->pi ? (bhvm_hf3_vm_s*)bhvm_hf3_vm_s_get_aware( o->pi ) : NULL;} \
  static inline void bhvm_hf3_vm_fpi_s_mutated( bhvm_hf3_vm_fpi_s* o ){bhvm_hf3_vm_fpi_s_copy_x( o );} \
  static inline void bhvm_hf3_vm_fpi_s_trun( const bhvm_hf3_vm_fpi_s* o, bhvm_hf3_vm_thread_s* t ){assert( o->p ); assert( o->p->trun ); o->p->trun( (vc_t)o->pi, t );}
#define TYPEOF_bhvm_hf3_vm_fcode_s 1831497336
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_fcode_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_fcode_s ) \
    {aware_t _;tp_t name;BCORE_ARRAY_DYN_SOLID_STATIC_S( bhvm_hf3_vm_fpi_s, );}; \
  void bhvm_hf3_vm_fcode_s_pi_push_d( bhvm_hf3_vm_fcode_s* o, bhvm_hf3_vm_pi* pi ); \
  void bhvm_hf3_vm_fcode_s_pi_push_c( bhvm_hf3_vm_fcode_s* o, const bhvm_hf3_vm_pi* pi ); \
  void bhvm_hf3_vm_fcode_s_trun( const bhvm_hf3_vm_fcode_s* o, bhvm_hf3_vm_thread_s* t ); \
  static inline void bhvm_hf3_vm_fcode_s_set_space( bhvm_hf3_vm_fcode_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_bhvm_hf3_vm_fcode_s, ( bcore_array* )o, size ); } \
  static inline void bhvm_hf3_vm_fcode_s_set_size( bhvm_hf3_vm_fcode_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_bhvm_hf3_vm_fcode_s, ( bcore_array* )o, size ); } \
  static inline void bhvm_hf3_vm_fcode_s_clear( bhvm_hf3_vm_fcode_s* o ) { bcore_array_t_set_space( TYPEOF_bhvm_hf3_vm_fcode_s, ( bcore_array* )o, 0 ); } \
  static inline bhvm_hf3_vm_fpi_s* bhvm_hf3_vm_fcode_s_push_c( bhvm_hf3_vm_fcode_s* o, const bhvm_hf3_vm_fpi_s* v ) { bcore_array_t_push( TYPEOF_bhvm_hf3_vm_fcode_s, ( bcore_array* )o, sr_twc( TYPEOF_bhvm_hf3_vm_fpi_s, v ) ); return &o->data[ o->size - 1 ]; } \
  static inline bhvm_hf3_vm_fpi_s* bhvm_hf3_vm_fcode_s_push_d( bhvm_hf3_vm_fcode_s* o,       bhvm_hf3_vm_fpi_s* v ) { bcore_array_t_push( TYPEOF_bhvm_hf3_vm_fcode_s, ( bcore_array* )o, sr_tsd( TYPEOF_bhvm_hf3_vm_fpi_s, v ) ); return &o->data[ o->size - 1 ]; } \
  static inline bhvm_hf3_vm_fpi_s* bhvm_hf3_vm_fcode_s_push( bhvm_hf3_vm_fcode_s* o ) \
  { \
      bcore_array_t_push( TYPEOF_bhvm_hf3_vm_fcode_s, ( bcore_array* )o, sr_null() ); \
      return &o->data[ o->size - 1 ]; \
  }
#define TYPEOF_bhvm_hf3_vm_arr_fcode_s 3062995108
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_arr_fcode_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_arr_fcode_s ) \
    {aware_t _;BCORE_ARRAY_DYN_LINK_STATIC_S( bhvm_hf3_vm_fcode_s, );}; \
  static inline void bhvm_hf3_vm_arr_fcode_s_set_space( bhvm_hf3_vm_arr_fcode_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_bhvm_hf3_vm_arr_fcode_s, ( bcore_array* )o, size ); } \
  static inline void bhvm_hf3_vm_arr_fcode_s_set_size( bhvm_hf3_vm_arr_fcode_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_bhvm_hf3_vm_arr_fcode_s, ( bcore_array* )o, size ); } \
  static inline void bhvm_hf3_vm_arr_fcode_s_clear( bhvm_hf3_vm_arr_fcode_s* o ) { bcore_array_t_set_space( TYPEOF_bhvm_hf3_vm_arr_fcode_s, ( bcore_array* )o, 0 ); } \
  static inline bhvm_hf3_vm_fcode_s* bhvm_hf3_vm_arr_fcode_s_push_c( bhvm_hf3_vm_arr_fcode_s* o, const bhvm_hf3_vm_fcode_s* v ) { bcore_array_t_push( TYPEOF_bhvm_hf3_vm_arr_fcode_s, ( bcore_array* )o, sr_twc( TYPEOF_bhvm_hf3_vm_fcode_s, v ) ); return o->data[ o->size - 1 ]; } \
  static inline bhvm_hf3_vm_fcode_s* bhvm_hf3_vm_arr_fcode_s_push_d( bhvm_hf3_vm_arr_fcode_s* o,       bhvm_hf3_vm_fcode_s* v ) { bcore_array_t_push( TYPEOF_bhvm_hf3_vm_arr_fcode_s, ( bcore_array* )o, sr_tsd( TYPEOF_bhvm_hf3_vm_fcode_s, v ) ); return o->data[ o->size - 1 ]; } \
  static inline bhvm_hf3_vm_fcode_s* bhvm_hf3_vm_arr_fcode_s_push( bhvm_hf3_vm_arr_fcode_s* o ) \
  { \
      bcore_array_t_push( TYPEOF_bhvm_hf3_vm_arr_fcode_s, ( bcore_array* )o, sr_t_create( TYPEOF_bhvm_hf3_vm_fcode_s ) ); \
      return o->data[ o->size - 1 ]; \
  }
#define TYPEOF_bhvm_hf3_vm_thread_s 3180417685
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_thread_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_thread_s ) \
    {aware_t _;bhvm_hf3_vm_frame_s* frame;bhvm_hf3_vm_fcode_s* fcode;sz_t ip;sz_t sp;bcore_arr_sz_s stack;};
#define TYPEOF_bhvm_hf3_vm_lib_fcode_s 1539394548
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_lib_fcode_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_lib_fcode_s ) \
    {aware_t _;bhvm_hf3_vm_arr_fcode_s arr;bcore_hmap_tpuz_s map;}; \
  static inline void bhvm_hf3_vm_lib_fcode_s_clear( bhvm_hf3_vm_lib_fcode_s* o ){bhvm_hf3_vm_arr_fcode_s_clear( &o->arr ); bcore_hmap_tpuz_s_clear( &o->map );} \
  static inline bl_t bhvm_hf3_vm_lib_fcode_s_fcode_exists( const bhvm_hf3_vm_lib_fcode_s* o, tp_t name ){return bcore_hmap_tpuz_s_exists( &o->map, name );} \
  bhvm_hf3_vm_fcode_s* bhvm_hf3_vm_lib_fcode_s_fcode_get( bhvm_hf3_vm_lib_fcode_s* o, tp_t name ); \
  bhvm_hf3_vm_fcode_s* bhvm_hf3_vm_lib_fcode_s_fcode_get_or_new( bhvm_hf3_vm_lib_fcode_s* o, tp_t name ); \
  bhvm_hf3_vm_fcode_s* bhvm_hf3_vm_lib_fcode_s_fcode_reset( bhvm_hf3_vm_lib_fcode_s* o, tp_t name ); \
  static inline void bhvm_hf3_vm_lib_fcode_s_fcode_pi_push_c( bhvm_hf3_vm_lib_fcode_s* o, tp_t name, const bhvm_hf3_vm_pi* pi ){bhvm_hf3_vm_fcode_s_pi_push_c( bhvm_hf3_vm_lib_fcode_s_fcode_get_or_new( o, name ), pi );} \
  static inline void bhvm_hf3_vm_lib_fcode_s_fcode_pi_push_d( bhvm_hf3_vm_lib_fcode_s* o, tp_t name, bhvm_hf3_vm_pi* pi ){bhvm_hf3_vm_fcode_s_pi_push_d( bhvm_hf3_vm_lib_fcode_s_fcode_get_or_new( o, name ), pi );} \
  void bhvm_hf3_vm_lib_fcode_s_fcode_push( bhvm_hf3_vm_lib_fcode_s* o, tp_t name, tp_t src_name ); \
  void bhvm_hf3_vm_lib_fcode_s_fcode_remove( bhvm_hf3_vm_lib_fcode_s* o, tp_t name ); \
  static inline void bhvm_hf3_vm_lib_fcode_s_fcode_trun( bhvm_hf3_vm_lib_fcode_s* o, tp_t name, bhvm_hf3_vm_thread_s* t ){bhvm_hf3_vm_fcode_s* fc = bhvm_hf3_vm_lib_fcode_s_fcode_get( o, name ); if( fc ) bhvm_hf3_vm_fcode_s_trun( fc, t );}
#define TYPEOF_bhvm_hf3_vm_frame_s 2173405754
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_frame_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_frame_s ) \
    {aware_t _;bhvm_hf3_vm_arr_holor_s arr_holor;bhvm_hf3_vm_lib_mcode_s lib_mcode;bhvm_hf3_vm_lib_fcode_s lib_fcode;bcore_hmap_name_s map_name;tp_t mcode_setup;tp_t mcode_shelve;bcore_arr_sz_s input;bcore_arr_sz_s output;}; \
  void bhvm_hf3_vm_frame_s_setup( bhvm_hf3_vm_frame_s* o ); \
  static inline void bhvm_hf3_vm_frame_s_mutated( bhvm_hf3_vm_frame_s* o ){bhvm_hf3_vm_frame_s_setup( o );} \
  void bhvm_hf3_vm_frame_s_shelve( bhvm_hf3_vm_frame_s* o ); \
  static inline bl_t bhvm_hf3_vm_frame_s_mcode_exists( const bhvm_hf3_vm_frame_s* o, tp_t name ){return bhvm_hf3_vm_lib_mcode_s_mcode_exists( &o->lib_mcode, name );} \
  static inline bhvm_hf3_vm_mcode_s* bhvm_hf3_vm_frame_s_mcode_get( bhvm_hf3_vm_frame_s* o, tp_t name ){return bhvm_hf3_vm_lib_mcode_s_mcode_get(    &o->lib_mcode, name );} \
  static inline bhvm_hf3_vm_mcode_s* bhvm_hf3_vm_frame_s_mcode_get_or_new( bhvm_hf3_vm_frame_s* o, tp_t name ){return bhvm_hf3_vm_lib_mcode_s_mcode_get_or_new( &o->lib_mcode, name );} \
  static inline bhvm_hf3_vm_mcode_s* bhvm_hf3_vm_frame_s_mcode_reset( bhvm_hf3_vm_frame_s* o, tp_t name ){return bhvm_hf3_vm_lib_mcode_s_mcode_reset(  &o->lib_mcode, name );} \
  static inline void bhvm_hf3_vm_frame_s_mcode_op_push_d( bhvm_hf3_vm_frame_s* o, tp_t name, bhvm_hf3_vm_op* op ){bhvm_hf3_vm_lib_mcode_s_mcode_op_push_d(     &o->lib_mcode, name, op );} \
  static inline void bhvm_hf3_vm_frame_s_mcode_op_push_c( bhvm_hf3_vm_frame_s* o, tp_t name, const bhvm_hf3_vm_op* op ){bhvm_hf3_vm_lib_mcode_s_mcode_op_push_c(     &o->lib_mcode, name, op );} \
  static inline void bhvm_hf3_vm_frame_s_mcode_push( bhvm_hf3_vm_frame_s* o, tp_t name, tp_t src_name ){bhvm_hf3_vm_lib_mcode_s_mcode_push(          &o->lib_mcode, name, src_name );} \
  static inline void bhvm_hf3_vm_frame_s_mcode_push_reverse( bhvm_hf3_vm_frame_s* o, tp_t name, tp_t src_name ){bhvm_hf3_vm_lib_mcode_s_mcode_push_reverse(  &o->lib_mcode, name, src_name );} \
  static inline void bhvm_hf3_vm_frame_s_mcode_remove( bhvm_hf3_vm_frame_s* o, tp_t name ){bhvm_hf3_vm_lib_mcode_s_mcode_remove(        &o->lib_mcode, name );} \
  static inline void bhvm_hf3_vm_frame_s_mcode_run( bhvm_hf3_vm_frame_s* o, tp_t name ){bhvm_hf3_vm_lib_mcode_s_mcode_mrun(          &o->lib_mcode, name, o->arr_holor.data );} \
  static inline bl_t bhvm_hf3_vm_frame_s_fcode_exists( const bhvm_hf3_vm_frame_s* o, tp_t name ){return bhvm_hf3_vm_lib_fcode_s_fcode_exists( &o->lib_fcode, name );} \
  static inline bhvm_hf3_vm_fcode_s* bhvm_hf3_vm_frame_s_fcode_get( bhvm_hf3_vm_frame_s* o, tp_t name ){return bhvm_hf3_vm_lib_fcode_s_fcode_get(    &o->lib_fcode, name );} \
  static inline bhvm_hf3_vm_fcode_s* bhvm_hf3_vm_frame_s_fcode_get_or_new( bhvm_hf3_vm_frame_s* o, tp_t name ){return bhvm_hf3_vm_lib_fcode_s_fcode_get_or_new( &o->lib_fcode, name );} \
  static inline bhvm_hf3_vm_fcode_s* bhvm_hf3_vm_frame_s_fcode_reset( bhvm_hf3_vm_frame_s* o, tp_t name ){return bhvm_hf3_vm_lib_fcode_s_fcode_reset(  &o->lib_fcode, name );} \
  static inline void bhvm_hf3_vm_frame_s_fcode_pi_push_d( bhvm_hf3_vm_frame_s* o, tp_t name, bhvm_hf3_vm_pi* pi ){bhvm_hf3_vm_lib_fcode_s_fcode_pi_push_d(     &o->lib_fcode, name, pi );} \
  static inline void bhvm_hf3_vm_frame_s_fcode_pi_push_c( bhvm_hf3_vm_frame_s* o, tp_t name, const bhvm_hf3_vm_pi* pi ){bhvm_hf3_vm_lib_fcode_s_fcode_pi_push_c(     &o->lib_fcode, name, pi );} \
  static inline void bhvm_hf3_vm_frame_s_fcode_push( bhvm_hf3_vm_frame_s* o, tp_t name, tp_t src_name ){bhvm_hf3_vm_lib_fcode_s_fcode_push(          &o->lib_fcode, name, src_name );} \
  static inline void bhvm_hf3_vm_frame_s_fcode_remove( bhvm_hf3_vm_frame_s* o, tp_t name ){bhvm_hf3_vm_lib_fcode_s_fcode_remove(        &o->lib_fcode, name );}
#define BETH_EXPAND_GROUP_bhvm_hf3_vm \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_hparam_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_holor_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_arr_holor_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_mop_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_mcode_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_arr_mcode_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_lib_mcode_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_thread_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_pi ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_fpi_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_fcode_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_arr_fcode_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_thread_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_lib_fcode_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_frame_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_pi ); \
  typedef void (*bhvm_hf3_vm_run)( const bhvm_hf3_vm* o, bhvm_hf3_vm_holor_s* ah ); \
  typedef void (*bhvm_hf3_vm_mrun)( const bhvm_hf3_vm* o, bhvm_hf3_vm_holor_s* ah ); \
  typedef void (*bhvm_hf3_vm_trun)( const bhvm_hf3_vm* o, bhvm_hf3_vm_thread_s* t ); \
  BCORE_DECLARE_SPECT( bhvm_hf3_vm ) \
  { \
      bcore_spect_header_s header; \
      bhvm_hf3_vm_run run; \
      bhvm_hf3_vm_mrun mrun; \
      bhvm_hf3_vm_trun trun; \
  }; \
  static inline bhvm_hf3_vm* bhvm_hf3_vm_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_bhvm_hf3_vm, t ); return ( bhvm_hf3_vm* )bcore_inst_t_create( t ); } \
  static inline bl_t bhvm_hf3_vm_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_bhvm_hf3_vm ); } \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bhvm_hf3_vm ) \
  static inline bl_t bhvm_hf3_vm_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_bhvm_hf3_vm ); } \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_hparam_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_holor_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_arr_holor_s \
  static inline void bhvm_hf3_vm_p_run( const bhvm_hf3_vm_s* __p, const bhvm_hf3_vm* o, bhvm_hf3_vm_holor_s* ah ) { assert( __p->run ); __p->run( o, ah ); } \
  static inline void bhvm_hf3_vm_a_run( const bhvm_hf3_vm* o, bhvm_hf3_vm_holor_s* ah ) { const bhvm_hf3_vm_s* p = bhvm_hf3_vm_s_get_aware( o ); assert( p->run ); p->run( o, ah ); } \
  static inline bl_t bhvm_hf3_vm_p_defines_run( const bhvm_hf3_vm_s* __p ) { return __p->run != NULL; } \
  static inline bl_t bhvm_hf3_vm_a_defines_run( const bhvm_hf3_vm* o ) { return bhvm_hf3_vm_s_get_aware( o )->run != NULL; } \
  BETH_EXPAND_GROUP_bhvm_hf3_vm_op \
  static inline void bhvm_hf3_vm_p_mrun( const bhvm_hf3_vm_s* __p, const bhvm_hf3_vm* o, bhvm_hf3_vm_holor_s* ah ) { assert( __p->mrun ); __p->mrun( o, ah ); } \
  static inline void bhvm_hf3_vm_a_mrun( const bhvm_hf3_vm* o, bhvm_hf3_vm_holor_s* ah ) { const bhvm_hf3_vm_s* p = bhvm_hf3_vm_s_get_aware( o ); assert( p->mrun ); p->mrun( o, ah ); } \
  static inline bl_t bhvm_hf3_vm_p_defines_mrun( const bhvm_hf3_vm_s* __p ) { return __p->mrun != NULL; } \
  static inline bl_t bhvm_hf3_vm_a_defines_mrun( const bhvm_hf3_vm* o ) { return bhvm_hf3_vm_s_get_aware( o )->mrun != NULL; } \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_mop_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_mcode_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_arr_mcode_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_lib_mcode_s \
  static inline void bhvm_hf3_vm_p_trun( const bhvm_hf3_vm_s* __p, const bhvm_hf3_vm* o, bhvm_hf3_vm_thread_s* t ) { assert( __p->trun ); __p->trun( o, t ); } \
  static inline void bhvm_hf3_vm_a_trun( const bhvm_hf3_vm* o, bhvm_hf3_vm_thread_s* t ) { const bhvm_hf3_vm_s* p = bhvm_hf3_vm_s_get_aware( o ); assert( p->trun ); p->trun( o, t ); } \
  static inline bl_t bhvm_hf3_vm_p_defines_trun( const bhvm_hf3_vm_s* __p ) { return __p->trun != NULL; } \
  static inline bl_t bhvm_hf3_vm_a_defines_trun( const bhvm_hf3_vm* o ) { return bhvm_hf3_vm_s_get_aware( o )->trun != NULL; } \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_fpi_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_fcode_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_arr_fcode_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_thread_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_lib_fcode_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_frame_s \
  BETH_EXPAND_GROUP_bhvm_hf3_vm_pi

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_vm_op

#define TYPEOF_bhvm_hf3_vm_op 282174308
#define TYPEOF_bhvm_hf3_vm_op_s 2268828886
#define BETH_EXPAND_GROUP_bhvm_hf3_vm_op \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar0 ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar1 ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar2 ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar3 ); \
  typedef sz_t (*bhvm_hf3_vm_op_get_arity)( const bhvm_hf3_vm_op* o ); \
  typedef void (*bhvm_hf3_vm_op_set_indices)( bhvm_hf3_vm_op* o, sz_t* a ); \
  typedef void (*bhvm_hf3_vm_op_get_indices)( const bhvm_hf3_vm_op* o, sz_t* a ); \
  typedef sc_t (*bhvm_hf3_vm_op_sig)( const bhvm_hf3_vm_op* o ); \
  typedef void (*bhvm_hf3_vm_op_set_arg)( bhvm_hf3_vm_op* o, char id, sz_t idx ); \
  BCORE_DECLARE_SPECT( bhvm_hf3_vm_op ) \
  { \
      bcore_spect_header_s header; \
      bhvm_hf3_vm_op_get_arity get_arity; \
      bhvm_hf3_vm_op_set_indices set_indices; \
      bhvm_hf3_vm_op_get_indices get_indices; \
      bhvm_hf3_vm_op_sig sig; \
      bhvm_hf3_vm_op_set_arg set_arg; \
  }; \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_bhvm_hf3_vm_op, t ); return ( bhvm_hf3_vm_op* )bcore_inst_t_create( t ); } \
  static inline bl_t bhvm_hf3_vm_op_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_bhvm_hf3_vm_op ); } \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bhvm_hf3_vm_op ) \
  static inline bl_t bhvm_hf3_vm_op_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_bhvm_hf3_vm_op ); } \
  static inline sz_t bhvm_hf3_vm_op_a_get_arity( const bhvm_hf3_vm_op* o ) { const bhvm_hf3_vm_op_s* p = bhvm_hf3_vm_op_s_get_aware( o ); assert( p->get_arity ); return p->get_arity( o ); } \
  static inline bl_t bhvm_hf3_vm_op_a_defines_get_arity( const bhvm_hf3_vm_op* o ) { return bhvm_hf3_vm_op_s_get_aware( o )->get_arity != NULL; } \
  static inline void bhvm_hf3_vm_op_a_set_indices( bhvm_hf3_vm_op* o, sz_t* a ) { const bhvm_hf3_vm_op_s* p = bhvm_hf3_vm_op_s_get_aware( o ); assert( p->set_indices ); p->set_indices( o, a ); } \
  static inline bl_t bhvm_hf3_vm_op_a_defines_set_indices( const bhvm_hf3_vm_op* o ) { return bhvm_hf3_vm_op_s_get_aware( o )->set_indices != NULL; } \
  static inline void bhvm_hf3_vm_op_a_get_indices( const bhvm_hf3_vm_op* o, sz_t* a ) { const bhvm_hf3_vm_op_s* p = bhvm_hf3_vm_op_s_get_aware( o ); assert( p->get_indices ); p->get_indices( o, a ); } \
  static inline bl_t bhvm_hf3_vm_op_a_defines_get_indices( const bhvm_hf3_vm_op* o ) { return bhvm_hf3_vm_op_s_get_aware( o )->get_indices != NULL; } \
  static inline sc_t bhvm_hf3_vm_op_a_sig( const bhvm_hf3_vm_op* o ) { const bhvm_hf3_vm_op_s* p = bhvm_hf3_vm_op_s_get_aware( o ); assert( p->sig ); return p->sig( o ); } \
  static inline bl_t bhvm_hf3_vm_op_a_defines_sig( const bhvm_hf3_vm_op* o ) { return bhvm_hf3_vm_op_s_get_aware( o )->sig != NULL; } \
  static inline void bhvm_hf3_vm_op_a_set_arg( bhvm_hf3_vm_op* o, char id, sz_t idx ) { const bhvm_hf3_vm_op_s* p = bhvm_hf3_vm_op_s_get_aware( o ); assert( p->set_arg ); p->set_arg( o, id, idx ); } \
  static inline bl_t bhvm_hf3_vm_op_a_defines_set_arg( const bhvm_hf3_vm_op* o ) { return bhvm_hf3_vm_op_s_get_aware( o )->set_arg != NULL; } \
  BETH_EXPAND_GROUP_bhvm_hf3_vm_op_ar0 \
  BETH_EXPAND_GROUP_bhvm_hf3_vm_op_ar1 \
  BETH_EXPAND_GROUP_bhvm_hf3_vm_op_ar2 \
  BETH_EXPAND_GROUP_bhvm_hf3_vm_op_ar3

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_vm_op_ar0

#define TYPEOF_bhvm_hf3_vm_op_ar0 451650482
#define TYPEOF_bhvm_hf3_vm_op_ar0_s 429893484
#define TYPEOF_bhvm_hf3_vm_op_ar0_nul_s 2545579148
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar0_nul_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar0_nul_s ) \
    {aware_t _;sz_t a;}; \
  static inline sz_t bhvm_hf3_vm_op_ar0_nul_s_get_arity( const bhvm_hf3_vm_op_ar0_nul_s* o ){return 0;} \
  static inline void bhvm_hf3_vm_op_ar0_nul_s_set_indices( bhvm_hf3_vm_op_ar0_nul_s* o, sz_t* a ){o->a = a[0];} \
  static inline void bhvm_hf3_vm_op_ar0_nul_s_get_indices( const bhvm_hf3_vm_op_ar0_nul_s* o, sz_t* a ){a[0] = o->a;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar0_nul_s_csetup( bhvm_hf3_vm_op_ar0_nul_s* o, sz_t idx_a ){if( !o ) o = bhvm_hf3_vm_op_ar0_nul_s_create(); o->a = idx_a; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar0_nul_s_run( const bhvm_hf3_vm_op_ar0_nul_s* o, bhvm_hf3_vm_holor_s* ah ){} \
  void bhvm_hf3_vm_op_ar0_nul_s_set_arg( bhvm_hf3_vm_op_ar0_nul_s* o, char id, sz_t idx ); \
  static inline sc_t bhvm_hf3_vm_op_ar0_nul_s_sig( const bhvm_hf3_vm_op_ar0_nul_s* o ){return "y";}
#define TYPEOF_bhvm_hf3_vm_op_ar0_zro_s 2471423050
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar0_zro_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar0_zro_s ) \
    {aware_t _;sz_t a;}; \
  static inline sz_t bhvm_hf3_vm_op_ar0_zro_s_get_arity( const bhvm_hf3_vm_op_ar0_zro_s* o ){return 0;} \
  static inline void bhvm_hf3_vm_op_ar0_zro_s_set_indices( bhvm_hf3_vm_op_ar0_zro_s* o, sz_t* a ){o->a = a[0];} \
  static inline void bhvm_hf3_vm_op_ar0_zro_s_get_indices( const bhvm_hf3_vm_op_ar0_zro_s* o, sz_t* a ){a[0] = o->a;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar0_zro_s_csetup( bhvm_hf3_vm_op_ar0_zro_s* o, sz_t idx_a ){if( !o ) o = bhvm_hf3_vm_op_ar0_zro_s_create(); o->a = idx_a; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar0_zro_s_run( const bhvm_hf3_vm_op_ar0_zro_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_s_zro(        &ah[ o->a ].h );} \
  void bhvm_hf3_vm_op_ar0_zro_s_set_arg( bhvm_hf3_vm_op_ar0_zro_s* o, char id, sz_t idx ); \
  static inline sc_t bhvm_hf3_vm_op_ar0_zro_s_sig( const bhvm_hf3_vm_op_ar0_zro_s* o ){return "y";}
#define TYPEOF_bhvm_hf3_vm_op_ar0_zro_nc_s 4169540224
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar0_zro_nc_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar0_zro_nc_s ) \
    {aware_t _;sz_t a;}; \
  static inline sz_t bhvm_hf3_vm_op_ar0_zro_nc_s_get_arity( const bhvm_hf3_vm_op_ar0_zro_nc_s* o ){return 0;} \
  static inline void bhvm_hf3_vm_op_ar0_zro_nc_s_set_indices( bhvm_hf3_vm_op_ar0_zro_nc_s* o, sz_t* a ){o->a = a[0];} \
  static inline void bhvm_hf3_vm_op_ar0_zro_nc_s_get_indices( const bhvm_hf3_vm_op_ar0_zro_nc_s* o, sz_t* a ){a[0] = o->a;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar0_zro_nc_s_csetup( bhvm_hf3_vm_op_ar0_zro_nc_s* o, sz_t idx_a ){if( !o ) o = bhvm_hf3_vm_op_ar0_zro_nc_s_create(); o->a = idx_a; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar0_zro_nc_s_run( const bhvm_hf3_vm_op_ar0_zro_nc_s* o, bhvm_hf3_vm_holor_s* ah ){if( ah[ o->a ].h.v_space > 0 ) bhvm_hf3_s_zro( &ah[ o->a ].h );} \
  void bhvm_hf3_vm_op_ar0_zro_nc_s_set_arg( bhvm_hf3_vm_op_ar0_zro_nc_s* o, char id, sz_t idx ); \
  static inline sc_t bhvm_hf3_vm_op_ar0_zro_nc_s_sig( const bhvm_hf3_vm_op_ar0_zro_nc_s* o ){return "y";}
#define TYPEOF_bhvm_hf3_vm_op_ar0_determine_s 2720720910
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar0_determine_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar0_determine_s ) \
    {aware_t _;sz_t a;}; \
  static inline sz_t bhvm_hf3_vm_op_ar0_determine_s_get_arity( const bhvm_hf3_vm_op_ar0_determine_s* o ){return 0;} \
  static inline void bhvm_hf3_vm_op_ar0_determine_s_set_indices( bhvm_hf3_vm_op_ar0_determine_s* o, sz_t* a ){o->a = a[0];} \
  static inline void bhvm_hf3_vm_op_ar0_determine_s_get_indices( const bhvm_hf3_vm_op_ar0_determine_s* o, sz_t* a ){a[0] = o->a;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar0_determine_s_csetup( bhvm_hf3_vm_op_ar0_determine_s* o, sz_t idx_a ){if( !o ) o = bhvm_hf3_vm_op_ar0_determine_s_create(); o->a = idx_a; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar0_determine_s_run( const bhvm_hf3_vm_op_ar0_determine_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_s_fit_v_size( &ah[ o->a ].h );} \
  void bhvm_hf3_vm_op_ar0_determine_s_set_arg( bhvm_hf3_vm_op_ar0_determine_s* o, char id, sz_t idx ); \
  static inline sc_t bhvm_hf3_vm_op_ar0_determine_s_sig( const bhvm_hf3_vm_op_ar0_determine_s* o ){return "y";}
#define TYPEOF_bhvm_hf3_vm_op_ar0_vacate_s 3528096543
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar0_vacate_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar0_vacate_s ) \
    {aware_t _;sz_t a;}; \
  static inline sz_t bhvm_hf3_vm_op_ar0_vacate_s_get_arity( const bhvm_hf3_vm_op_ar0_vacate_s* o ){return 0;} \
  static inline void bhvm_hf3_vm_op_ar0_vacate_s_set_indices( bhvm_hf3_vm_op_ar0_vacate_s* o, sz_t* a ){o->a = a[0];} \
  static inline void bhvm_hf3_vm_op_ar0_vacate_s_get_indices( const bhvm_hf3_vm_op_ar0_vacate_s* o, sz_t* a ){a[0] = o->a;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar0_vacate_s_csetup( bhvm_hf3_vm_op_ar0_vacate_s* o, sz_t idx_a ){if( !o ) o = bhvm_hf3_vm_op_ar0_vacate_s_create(); o->a = idx_a; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar0_vacate_s_run( const bhvm_hf3_vm_op_ar0_vacate_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_s_set_vacant( &ah[ o->a ].h );} \
  void bhvm_hf3_vm_op_ar0_vacate_s_set_arg( bhvm_hf3_vm_op_ar0_vacate_s* o, char id, sz_t idx ); \
  static inline sc_t bhvm_hf3_vm_op_ar0_vacate_s_sig( const bhvm_hf3_vm_op_ar0_vacate_s* o ){return "y";}
#define TYPEOF_bhvm_hf3_vm_op_ar0_clear_s 746184210
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar0_clear_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar0_clear_s ) \
    {aware_t _;sz_t a;}; \
  static inline sz_t bhvm_hf3_vm_op_ar0_clear_s_get_arity( const bhvm_hf3_vm_op_ar0_clear_s* o ){return 0;} \
  static inline void bhvm_hf3_vm_op_ar0_clear_s_set_indices( bhvm_hf3_vm_op_ar0_clear_s* o, sz_t* a ){o->a = a[0];} \
  static inline void bhvm_hf3_vm_op_ar0_clear_s_get_indices( const bhvm_hf3_vm_op_ar0_clear_s* o, sz_t* a ){a[0] = o->a;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar0_clear_s_csetup( bhvm_hf3_vm_op_ar0_clear_s* o, sz_t idx_a ){if( !o ) o = bhvm_hf3_vm_op_ar0_clear_s_create(); o->a = idx_a; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar0_clear_s_run( const bhvm_hf3_vm_op_ar0_clear_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_s_clear(      &ah[ o->a ].h );} \
  void bhvm_hf3_vm_op_ar0_clear_s_set_arg( bhvm_hf3_vm_op_ar0_clear_s* o, char id, sz_t idx ); \
  static inline sc_t bhvm_hf3_vm_op_ar0_clear_s_sig( const bhvm_hf3_vm_op_ar0_clear_s* o ){return "y";}
#define TYPEOF_bhvm_hf3_vm_op_ar0_randomize_s 614377668
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar0_randomize_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar0_randomize_s ) \
    {aware_t _;sz_t a;u2_t rseed;f3_t min;f3_t max;f3_t density;}; \
  static inline sz_t bhvm_hf3_vm_op_ar0_randomize_s_get_arity( const bhvm_hf3_vm_op_ar0_randomize_s* o ){return 0;} \
  static inline void bhvm_hf3_vm_op_ar0_randomize_s_set_indices( bhvm_hf3_vm_op_ar0_randomize_s* o, sz_t* a ){o->a = a[0];} \
  static inline void bhvm_hf3_vm_op_ar0_randomize_s_get_indices( const bhvm_hf3_vm_op_ar0_randomize_s* o, sz_t* a ){a[0] = o->a;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar0_randomize_s_csetup( bhvm_hf3_vm_op_ar0_randomize_s* o, sz_t idx_a ){if( !o ) o = bhvm_hf3_vm_op_ar0_randomize_s_create(); o->a = idx_a; return (bhvm_hf3_vm_op*)o;} \
  bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar0_randomize_s_csetup_randomize( bhvm_hf3_vm_op_ar0_randomize_s* o, sz_t idx_a, u2_t rseed, f3_t density, f3_t min, f3_t max ); \
  static inline void bhvm_hf3_vm_op_ar0_randomize_s_run( const bhvm_hf3_vm_op_ar0_randomize_s* o, bhvm_hf3_vm_holor_s* ah ){u2_t rval = o->rseed + o->a; bhvm_hf3_s_set_random( &ah[ o->a ].h, o->density, o->min, o->max, &rval );} \
  void bhvm_hf3_vm_op_ar0_randomize_s_set_arg( bhvm_hf3_vm_op_ar0_randomize_s* o, char id, sz_t idx ); \
  static inline sc_t bhvm_hf3_vm_op_ar0_randomize_s_sig( const bhvm_hf3_vm_op_ar0_randomize_s* o ){return "y";}
#define BETH_EXPAND_GROUP_bhvm_hf3_vm_op_ar0 \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar0 ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar0_nul_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar0_zro_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar0_zro_nc_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar0_determine_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar0_vacate_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar0_clear_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar0_randomize_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar0_dp ); \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar0_nul_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar0_zro_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar0_zro_nc_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar0_determine_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar0_vacate_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar0_clear_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar0_randomize_s \
  BETH_EXPAND_GROUP_bhvm_hf3_vm_op_ar0_dp

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_vm_op_ar0_dp

#define TYPEOF_bhvm_hf3_vm_op_ar0_dp 1819962235
#define TYPEOF_bhvm_hf3_vm_op_ar0_dp_s 915237389
#define TYPEOF_bhvm_hf3_vm_op_ar0_dp_ca_floor_s 202185547
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar0_dp_ca_floor_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar0_dp_ca_floor_s ) \
    {aware_t _;sz_t a;}; \
  static inline sz_t bhvm_hf3_vm_op_ar0_dp_ca_floor_s_get_arity( const bhvm_hf3_vm_op_ar0_dp_ca_floor_s* o ){return 0;} \
  static inline void bhvm_hf3_vm_op_ar0_dp_ca_floor_s_set_indices( bhvm_hf3_vm_op_ar0_dp_ca_floor_s* o, sz_t* a ){o->a = a[0];} \
  static inline void bhvm_hf3_vm_op_ar0_dp_ca_floor_s_get_indices( const bhvm_hf3_vm_op_ar0_dp_ca_floor_s* o, sz_t* a ){a[0] = o->a;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar0_dp_ca_floor_s_csetup( bhvm_hf3_vm_op_ar0_dp_ca_floor_s* o, sz_t idx_a ){if( !o ) o = bhvm_hf3_vm_op_ar0_dp_ca_floor_s_create(); o->a = idx_a; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar0_dp_ca_floor_s_run( const bhvm_hf3_vm_op_ar0_dp_ca_floor_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar0_dp_ca_floor_s_f( &ah[o->a].h );} \
  static inline sc_t bhvm_hf3_vm_op_ar0_dp_ca_floor_s_sig( const bhvm_hf3_vm_op_ar0_dp_ca_floor_s* o ){return "u";} \
  void bhvm_hf3_vm_op_ar0_dp_ca_floor_s_set_arg( bhvm_hf3_vm_op_ar0_dp_ca_floor_s* o, char id, sz_t idx );
#define TYPEOF_bhvm_hf3_vm_op_ar0_dp_ca_ceil_s 3161708566
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar0_dp_ca_ceil_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar0_dp_ca_ceil_s ) \
    {aware_t _;sz_t a;}; \
  static inline sz_t bhvm_hf3_vm_op_ar0_dp_ca_ceil_s_get_arity( const bhvm_hf3_vm_op_ar0_dp_ca_ceil_s* o ){return 0;} \
  static inline void bhvm_hf3_vm_op_ar0_dp_ca_ceil_s_set_indices( bhvm_hf3_vm_op_ar0_dp_ca_ceil_s* o, sz_t* a ){o->a = a[0];} \
  static inline void bhvm_hf3_vm_op_ar0_dp_ca_ceil_s_get_indices( const bhvm_hf3_vm_op_ar0_dp_ca_ceil_s* o, sz_t* a ){a[0] = o->a;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar0_dp_ca_ceil_s_csetup( bhvm_hf3_vm_op_ar0_dp_ca_ceil_s* o, sz_t idx_a ){if( !o ) o = bhvm_hf3_vm_op_ar0_dp_ca_ceil_s_create(); o->a = idx_a; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar0_dp_ca_ceil_s_run( const bhvm_hf3_vm_op_ar0_dp_ca_ceil_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar0_dp_ca_ceil_s_f ( &ah[o->a].h );} \
  static inline sc_t bhvm_hf3_vm_op_ar0_dp_ca_ceil_s_sig( const bhvm_hf3_vm_op_ar0_dp_ca_ceil_s* o ){return "u";} \
  void bhvm_hf3_vm_op_ar0_dp_ca_ceil_s_set_arg( bhvm_hf3_vm_op_ar0_dp_ca_ceil_s* o, char id, sz_t idx );
#define BETH_EXPAND_GROUP_bhvm_hf3_vm_op_ar0_dp \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar0_dp ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar0_dp_ca_floor_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar0_dp_ca_ceil_s ); \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar0_dp_ca_floor_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar0_dp_ca_ceil_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_vm_op_ar1

#define TYPEOF_bhvm_hf3_vm_op_ar1 468428101
#define TYPEOF_bhvm_hf3_vm_op_ar1_s 2508758055
#define TYPEOF_bhvm_hf3_vm_op_ar1_unary_s 1350751771
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_unary_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar1_unary_s ) \
    {aware_t _;sz_t a;sz_t b;bmath_fp_f3_ar1 unary;}; \
  static inline sz_t bhvm_hf3_vm_op_ar1_unary_s_get_arity( const bhvm_hf3_vm_op_ar1_unary_s* o ){return 1;} \
  static inline void bhvm_hf3_vm_op_ar1_unary_s_set_indices( bhvm_hf3_vm_op_ar1_unary_s* o, sz_t* a ){o->a = a[0]; o->b = a[1];} \
  static inline void bhvm_hf3_vm_op_ar1_unary_s_get_indices( const bhvm_hf3_vm_op_ar1_unary_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar1_unary_s_csetup( bhvm_hf3_vm_op_ar1_unary_s* o, sz_t idx_a, sz_t idx_b ){if( !o ) o = bhvm_hf3_vm_op_ar1_unary_s_create(); o->a = idx_a; o->b = idx_b; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar1_unary_s_run( const bhvm_hf3_vm_op_ar1_unary_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_s_fp_f3_ar1( &ah[o->a].h, o->unary, &ah[o->b].h );} \
  void bhvm_hf3_vm_op_ar1_unary_s_set_arg( bhvm_hf3_vm_op_ar1_unary_s* o, char id, sz_t idx ); \
  static inline sc_t bhvm_hf3_vm_op_ar1_unary_s_sig( const bhvm_hf3_vm_op_ar1_unary_s* o ){return "ay";}
#define TYPEOF_bhvm_hf3_vm_op_ar1_cpy_s 237558182
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_cpy_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar1_cpy_s ) \
    {aware_t _;sz_t a;sz_t b;}; \
  static inline sz_t bhvm_hf3_vm_op_ar1_cpy_s_get_arity( const bhvm_hf3_vm_op_ar1_cpy_s* o ){return 1;} \
  static inline void bhvm_hf3_vm_op_ar1_cpy_s_set_indices( bhvm_hf3_vm_op_ar1_cpy_s* o, sz_t* a ){o->a = a[0]; o->b = a[1];} \
  static inline void bhvm_hf3_vm_op_ar1_cpy_s_get_indices( const bhvm_hf3_vm_op_ar1_cpy_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar1_cpy_s_csetup( bhvm_hf3_vm_op_ar1_cpy_s* o, sz_t idx_a, sz_t idx_b ){if( !o ) o = bhvm_hf3_vm_op_ar1_cpy_s_create(); o->a = idx_a; o->b = idx_b; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar1_cpy_s_run( const bhvm_hf3_vm_op_ar1_cpy_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar1_cpy_s_f  ( &ah[o->a].h, &ah[o->b].h );} \
  void bhvm_hf3_vm_op_ar1_cpy_s_set_arg( bhvm_hf3_vm_op_ar1_cpy_s* o, char id, sz_t idx ); \
  static inline sc_t bhvm_hf3_vm_op_ar1_cpy_s_sig( const bhvm_hf3_vm_op_ar1_cpy_s* o ){return "ay";}
#define TYPEOF_bhvm_hf3_vm_op_ar1_neg_s 2942976434
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_neg_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar1_neg_s ) \
    {aware_t _;sz_t a;sz_t b;}; \
  static inline sz_t bhvm_hf3_vm_op_ar1_neg_s_get_arity( const bhvm_hf3_vm_op_ar1_neg_s* o ){return 1;} \
  static inline void bhvm_hf3_vm_op_ar1_neg_s_set_indices( bhvm_hf3_vm_op_ar1_neg_s* o, sz_t* a ){o->a = a[0]; o->b = a[1];} \
  static inline void bhvm_hf3_vm_op_ar1_neg_s_get_indices( const bhvm_hf3_vm_op_ar1_neg_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar1_neg_s_csetup( bhvm_hf3_vm_op_ar1_neg_s* o, sz_t idx_a, sz_t idx_b ){if( !o ) o = bhvm_hf3_vm_op_ar1_neg_s_create(); o->a = idx_a; o->b = idx_b; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar1_neg_s_run( const bhvm_hf3_vm_op_ar1_neg_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar1_neg_s_f  ( &ah[o->a].h, &ah[o->b].h );} \
  void bhvm_hf3_vm_op_ar1_neg_s_set_arg( bhvm_hf3_vm_op_ar1_neg_s* o, char id, sz_t idx ); \
  static inline sc_t bhvm_hf3_vm_op_ar1_neg_s_sig( const bhvm_hf3_vm_op_ar1_neg_s* o ){return "ay";}
#define TYPEOF_bhvm_hf3_vm_op_ar1_floor_s 4202880442
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_floor_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar1_floor_s ) \
    {aware_t _;sz_t a;sz_t b;}; \
  static inline sz_t bhvm_hf3_vm_op_ar1_floor_s_get_arity( const bhvm_hf3_vm_op_ar1_floor_s* o ){return 1;} \
  static inline void bhvm_hf3_vm_op_ar1_floor_s_set_indices( bhvm_hf3_vm_op_ar1_floor_s* o, sz_t* a ){o->a = a[0]; o->b = a[1];} \
  static inline void bhvm_hf3_vm_op_ar1_floor_s_get_indices( const bhvm_hf3_vm_op_ar1_floor_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar1_floor_s_csetup( bhvm_hf3_vm_op_ar1_floor_s* o, sz_t idx_a, sz_t idx_b ){if( !o ) o = bhvm_hf3_vm_op_ar1_floor_s_create(); o->a = idx_a; o->b = idx_b; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar1_floor_s_run( const bhvm_hf3_vm_op_ar1_floor_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar1_floor_s_f( &ah[o->a].h, &ah[o->b].h );} \
  void bhvm_hf3_vm_op_ar1_floor_s_set_arg( bhvm_hf3_vm_op_ar1_floor_s* o, char id, sz_t idx ); \
  static inline sc_t bhvm_hf3_vm_op_ar1_floor_s_sig( const bhvm_hf3_vm_op_ar1_floor_s* o ){return "ay";}
#define TYPEOF_bhvm_hf3_vm_op_ar1_ceil_s 2328616501
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_ceil_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar1_ceil_s ) \
    {aware_t _;sz_t a;sz_t b;}; \
  static inline sz_t bhvm_hf3_vm_op_ar1_ceil_s_get_arity( const bhvm_hf3_vm_op_ar1_ceil_s* o ){return 1;} \
  static inline void bhvm_hf3_vm_op_ar1_ceil_s_set_indices( bhvm_hf3_vm_op_ar1_ceil_s* o, sz_t* a ){o->a = a[0]; o->b = a[1];} \
  static inline void bhvm_hf3_vm_op_ar1_ceil_s_get_indices( const bhvm_hf3_vm_op_ar1_ceil_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar1_ceil_s_csetup( bhvm_hf3_vm_op_ar1_ceil_s* o, sz_t idx_a, sz_t idx_b ){if( !o ) o = bhvm_hf3_vm_op_ar1_ceil_s_create(); o->a = idx_a; o->b = idx_b; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar1_ceil_s_run( const bhvm_hf3_vm_op_ar1_ceil_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar1_ceil_s_f ( &ah[o->a].h, &ah[o->b].h );} \
  void bhvm_hf3_vm_op_ar1_ceil_s_set_arg( bhvm_hf3_vm_op_ar1_ceil_s* o, char id, sz_t idx ); \
  static inline sc_t bhvm_hf3_vm_op_ar1_ceil_s_sig( const bhvm_hf3_vm_op_ar1_ceil_s* o ){return "ay";}
#define TYPEOF_bhvm_hf3_vm_op_ar1_exp_s 928754955
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_exp_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar1_exp_s ) \
    {aware_t _;sz_t a;sz_t b;}; \
  static inline sz_t bhvm_hf3_vm_op_ar1_exp_s_get_arity( const bhvm_hf3_vm_op_ar1_exp_s* o ){return 1;} \
  static inline void bhvm_hf3_vm_op_ar1_exp_s_set_indices( bhvm_hf3_vm_op_ar1_exp_s* o, sz_t* a ){o->a = a[0]; o->b = a[1];} \
  static inline void bhvm_hf3_vm_op_ar1_exp_s_get_indices( const bhvm_hf3_vm_op_ar1_exp_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar1_exp_s_csetup( bhvm_hf3_vm_op_ar1_exp_s* o, sz_t idx_a, sz_t idx_b ){if( !o ) o = bhvm_hf3_vm_op_ar1_exp_s_create(); o->a = idx_a; o->b = idx_b; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar1_exp_s_run( const bhvm_hf3_vm_op_ar1_exp_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar1_exp_s_f  ( &ah[o->a].h, &ah[o->b].h );} \
  void bhvm_hf3_vm_op_ar1_exp_s_set_arg( bhvm_hf3_vm_op_ar1_exp_s* o, char id, sz_t idx ); \
  static inline sc_t bhvm_hf3_vm_op_ar1_exp_s_sig( const bhvm_hf3_vm_op_ar1_exp_s* o ){return "ay";}
#define TYPEOF_bhvm_hf3_vm_op_ar1_cpy_ay_s 2763339645
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_cpy_ay_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar1_cpy_ay_s ) \
    {aware_t _;sz_t a;sz_t b;}; \
  static inline sz_t bhvm_hf3_vm_op_ar1_cpy_ay_s_get_arity( const bhvm_hf3_vm_op_ar1_cpy_ay_s* o ){return 1;} \
  static inline void bhvm_hf3_vm_op_ar1_cpy_ay_s_set_indices( bhvm_hf3_vm_op_ar1_cpy_ay_s* o, sz_t* a ){o->a = a[0]; o->b = a[1];} \
  static inline void bhvm_hf3_vm_op_ar1_cpy_ay_s_get_indices( const bhvm_hf3_vm_op_ar1_cpy_ay_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar1_cpy_ay_s_csetup( bhvm_hf3_vm_op_ar1_cpy_ay_s* o, sz_t idx_a, sz_t idx_b ){if( !o ) o = bhvm_hf3_vm_op_ar1_cpy_ay_s_create(); o->a = idx_a; o->b = idx_b; return (bhvm_hf3_vm_op*)o;} \
  static inline sc_t bhvm_hf3_vm_op_ar1_cpy_ay_s_sig( const bhvm_hf3_vm_op_ar1_cpy_ay_s* o ){return "ay";} \
  static inline void bhvm_hf3_vm_op_ar1_cpy_ay_s_run( const bhvm_hf3_vm_op_ar1_cpy_ay_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar1_cpy_s_f( &ah[o->a].h, &ah[o->b].h );} \
  void bhvm_hf3_vm_op_ar1_cpy_ay_s_set_arg( bhvm_hf3_vm_op_ar1_cpy_ay_s* o, char id, sz_t idx );
#define TYPEOF_bhvm_hf3_vm_op_ar1_cpy_by_s 1436740426
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_cpy_by_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar1_cpy_by_s ) \
    {aware_t _;sz_t a;sz_t b;}; \
  static inline sz_t bhvm_hf3_vm_op_ar1_cpy_by_s_get_arity( const bhvm_hf3_vm_op_ar1_cpy_by_s* o ){return 1;} \
  static inline void bhvm_hf3_vm_op_ar1_cpy_by_s_set_indices( bhvm_hf3_vm_op_ar1_cpy_by_s* o, sz_t* a ){o->a = a[0]; o->b = a[1];} \
  static inline void bhvm_hf3_vm_op_ar1_cpy_by_s_get_indices( const bhvm_hf3_vm_op_ar1_cpy_by_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar1_cpy_by_s_csetup( bhvm_hf3_vm_op_ar1_cpy_by_s* o, sz_t idx_a, sz_t idx_b ){if( !o ) o = bhvm_hf3_vm_op_ar1_cpy_by_s_create(); o->a = idx_a; o->b = idx_b; return (bhvm_hf3_vm_op*)o;} \
  static inline sc_t bhvm_hf3_vm_op_ar1_cpy_by_s_sig( const bhvm_hf3_vm_op_ar1_cpy_by_s* o ){return "by";} \
  static inline void bhvm_hf3_vm_op_ar1_cpy_by_s_run( const bhvm_hf3_vm_op_ar1_cpy_by_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar1_cpy_s_f( &ah[o->a].h, &ah[o->b].h );} \
  void bhvm_hf3_vm_op_ar1_cpy_by_s_set_arg( bhvm_hf3_vm_op_ar1_cpy_by_s* o, char id, sz_t idx );
#define TYPEOF_bhvm_hf3_vm_op_ar1_lgst_s 2637714270
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_lgst_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar1_lgst_s ) \
    {aware_t _;sz_t a;sz_t b;}; \
  static inline sz_t bhvm_hf3_vm_op_ar1_lgst_s_get_arity( const bhvm_hf3_vm_op_ar1_lgst_s* o ){return 1;} \
  static inline void bhvm_hf3_vm_op_ar1_lgst_s_set_indices( bhvm_hf3_vm_op_ar1_lgst_s* o, sz_t* a ){o->a = a[0]; o->b = a[1];} \
  static inline void bhvm_hf3_vm_op_ar1_lgst_s_get_indices( const bhvm_hf3_vm_op_ar1_lgst_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar1_lgst_s_csetup( bhvm_hf3_vm_op_ar1_lgst_s* o, sz_t idx_a, sz_t idx_b ){if( !o ) o = bhvm_hf3_vm_op_ar1_lgst_s_create(); o->a = idx_a; o->b = idx_b; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar1_lgst_s_run( const bhvm_hf3_vm_op_ar1_lgst_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar1_lgst_s_f      ( &ah[o->a].h, &ah[o->b].h );} \
  void bhvm_hf3_vm_op_ar1_lgst_s_set_arg( bhvm_hf3_vm_op_ar1_lgst_s* o, char id, sz_t idx ); \
  static inline sc_t bhvm_hf3_vm_op_ar1_lgst_s_sig( const bhvm_hf3_vm_op_ar1_lgst_s* o ){return "ay";}
#define TYPEOF_bhvm_hf3_vm_op_ar1_lgst_hard_s 759313010
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_lgst_hard_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar1_lgst_hard_s ) \
    {aware_t _;sz_t a;sz_t b;}; \
  static inline sz_t bhvm_hf3_vm_op_ar1_lgst_hard_s_get_arity( const bhvm_hf3_vm_op_ar1_lgst_hard_s* o ){return 1;} \
  static inline void bhvm_hf3_vm_op_ar1_lgst_hard_s_set_indices( bhvm_hf3_vm_op_ar1_lgst_hard_s* o, sz_t* a ){o->a = a[0]; o->b = a[1];} \
  static inline void bhvm_hf3_vm_op_ar1_lgst_hard_s_get_indices( const bhvm_hf3_vm_op_ar1_lgst_hard_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar1_lgst_hard_s_csetup( bhvm_hf3_vm_op_ar1_lgst_hard_s* o, sz_t idx_a, sz_t idx_b ){if( !o ) o = bhvm_hf3_vm_op_ar1_lgst_hard_s_create(); o->a = idx_a; o->b = idx_b; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar1_lgst_hard_s_run( const bhvm_hf3_vm_op_ar1_lgst_hard_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar1_lgst_hard_s_f ( &ah[o->a].h, &ah[o->b].h );} \
  void bhvm_hf3_vm_op_ar1_lgst_hard_s_set_arg( bhvm_hf3_vm_op_ar1_lgst_hard_s* o, char id, sz_t idx ); \
  static inline sc_t bhvm_hf3_vm_op_ar1_lgst_hard_s_sig( const bhvm_hf3_vm_op_ar1_lgst_hard_s* o ){return "ay";}
#define TYPEOF_bhvm_hf3_vm_op_ar1_lgst_leaky_s 2429277723
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_lgst_leaky_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar1_lgst_leaky_s ) \
    {aware_t _;sz_t a;sz_t b;}; \
  static inline sz_t bhvm_hf3_vm_op_ar1_lgst_leaky_s_get_arity( const bhvm_hf3_vm_op_ar1_lgst_leaky_s* o ){return 1;} \
  static inline void bhvm_hf3_vm_op_ar1_lgst_leaky_s_set_indices( bhvm_hf3_vm_op_ar1_lgst_leaky_s* o, sz_t* a ){o->a = a[0]; o->b = a[1];} \
  static inline void bhvm_hf3_vm_op_ar1_lgst_leaky_s_get_indices( const bhvm_hf3_vm_op_ar1_lgst_leaky_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar1_lgst_leaky_s_csetup( bhvm_hf3_vm_op_ar1_lgst_leaky_s* o, sz_t idx_a, sz_t idx_b ){if( !o ) o = bhvm_hf3_vm_op_ar1_lgst_leaky_s_create(); o->a = idx_a; o->b = idx_b; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar1_lgst_leaky_s_run( const bhvm_hf3_vm_op_ar1_lgst_leaky_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar1_lgst_leaky_s_f( &ah[o->a].h, &ah[o->b].h );} \
  void bhvm_hf3_vm_op_ar1_lgst_leaky_s_set_arg( bhvm_hf3_vm_op_ar1_lgst_leaky_s* o, char id, sz_t idx ); \
  static inline sc_t bhvm_hf3_vm_op_ar1_lgst_leaky_s_sig( const bhvm_hf3_vm_op_ar1_lgst_leaky_s* o ){return "ay";}
#define TYPEOF_bhvm_hf3_vm_op_ar1_tanh_s 2624717141
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_tanh_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar1_tanh_s ) \
    {aware_t _;sz_t a;sz_t b;}; \
  static inline sz_t bhvm_hf3_vm_op_ar1_tanh_s_get_arity( const bhvm_hf3_vm_op_ar1_tanh_s* o ){return 1;} \
  static inline void bhvm_hf3_vm_op_ar1_tanh_s_set_indices( bhvm_hf3_vm_op_ar1_tanh_s* o, sz_t* a ){o->a = a[0]; o->b = a[1];} \
  static inline void bhvm_hf3_vm_op_ar1_tanh_s_get_indices( const bhvm_hf3_vm_op_ar1_tanh_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar1_tanh_s_csetup( bhvm_hf3_vm_op_ar1_tanh_s* o, sz_t idx_a, sz_t idx_b ){if( !o ) o = bhvm_hf3_vm_op_ar1_tanh_s_create(); o->a = idx_a; o->b = idx_b; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar1_tanh_s_run( const bhvm_hf3_vm_op_ar1_tanh_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar1_tanh_s_f      ( &ah[o->a].h, &ah[o->b].h );} \
  void bhvm_hf3_vm_op_ar1_tanh_s_set_arg( bhvm_hf3_vm_op_ar1_tanh_s* o, char id, sz_t idx ); \
  static inline sc_t bhvm_hf3_vm_op_ar1_tanh_s_sig( const bhvm_hf3_vm_op_ar1_tanh_s* o ){return "ay";}
#define TYPEOF_bhvm_hf3_vm_op_ar1_tanh_hard_s 1778994403
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_tanh_hard_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar1_tanh_hard_s ) \
    {aware_t _;sz_t a;sz_t b;}; \
  static inline sz_t bhvm_hf3_vm_op_ar1_tanh_hard_s_get_arity( const bhvm_hf3_vm_op_ar1_tanh_hard_s* o ){return 1;} \
  static inline void bhvm_hf3_vm_op_ar1_tanh_hard_s_set_indices( bhvm_hf3_vm_op_ar1_tanh_hard_s* o, sz_t* a ){o->a = a[0]; o->b = a[1];} \
  static inline void bhvm_hf3_vm_op_ar1_tanh_hard_s_get_indices( const bhvm_hf3_vm_op_ar1_tanh_hard_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar1_tanh_hard_s_csetup( bhvm_hf3_vm_op_ar1_tanh_hard_s* o, sz_t idx_a, sz_t idx_b ){if( !o ) o = bhvm_hf3_vm_op_ar1_tanh_hard_s_create(); o->a = idx_a; o->b = idx_b; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar1_tanh_hard_s_run( const bhvm_hf3_vm_op_ar1_tanh_hard_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar1_tanh_hard_s_f ( &ah[o->a].h, &ah[o->b].h );} \
  void bhvm_hf3_vm_op_ar1_tanh_hard_s_set_arg( bhvm_hf3_vm_op_ar1_tanh_hard_s* o, char id, sz_t idx ); \
  static inline sc_t bhvm_hf3_vm_op_ar1_tanh_hard_s_sig( const bhvm_hf3_vm_op_ar1_tanh_hard_s* o ){return "ay";}
#define TYPEOF_bhvm_hf3_vm_op_ar1_tanh_leaky_s 2826818232
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_tanh_leaky_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar1_tanh_leaky_s ) \
    {aware_t _;sz_t a;sz_t b;}; \
  static inline sz_t bhvm_hf3_vm_op_ar1_tanh_leaky_s_get_arity( const bhvm_hf3_vm_op_ar1_tanh_leaky_s* o ){return 1;} \
  static inline void bhvm_hf3_vm_op_ar1_tanh_leaky_s_set_indices( bhvm_hf3_vm_op_ar1_tanh_leaky_s* o, sz_t* a ){o->a = a[0]; o->b = a[1];} \
  static inline void bhvm_hf3_vm_op_ar1_tanh_leaky_s_get_indices( const bhvm_hf3_vm_op_ar1_tanh_leaky_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar1_tanh_leaky_s_csetup( bhvm_hf3_vm_op_ar1_tanh_leaky_s* o, sz_t idx_a, sz_t idx_b ){if( !o ) o = bhvm_hf3_vm_op_ar1_tanh_leaky_s_create(); o->a = idx_a; o->b = idx_b; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar1_tanh_leaky_s_run( const bhvm_hf3_vm_op_ar1_tanh_leaky_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar1_tanh_leaky_s_f( &ah[o->a].h, &ah[o->b].h );} \
  void bhvm_hf3_vm_op_ar1_tanh_leaky_s_set_arg( bhvm_hf3_vm_op_ar1_tanh_leaky_s* o, char id, sz_t idx ); \
  static inline sc_t bhvm_hf3_vm_op_ar1_tanh_leaky_s_sig( const bhvm_hf3_vm_op_ar1_tanh_leaky_s* o ){return "ay";}
#define TYPEOF_bhvm_hf3_vm_op_ar1_softplus_s 3729825904
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_softplus_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar1_softplus_s ) \
    {aware_t _;sz_t a;sz_t b;}; \
  static inline sz_t bhvm_hf3_vm_op_ar1_softplus_s_get_arity( const bhvm_hf3_vm_op_ar1_softplus_s* o ){return 1;} \
  static inline void bhvm_hf3_vm_op_ar1_softplus_s_set_indices( bhvm_hf3_vm_op_ar1_softplus_s* o, sz_t* a ){o->a = a[0]; o->b = a[1];} \
  static inline void bhvm_hf3_vm_op_ar1_softplus_s_get_indices( const bhvm_hf3_vm_op_ar1_softplus_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar1_softplus_s_csetup( bhvm_hf3_vm_op_ar1_softplus_s* o, sz_t idx_a, sz_t idx_b ){if( !o ) o = bhvm_hf3_vm_op_ar1_softplus_s_create(); o->a = idx_a; o->b = idx_b; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar1_softplus_s_run( const bhvm_hf3_vm_op_ar1_softplus_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar1_softplus_s_f  ( &ah[o->a].h, &ah[o->b].h );} \
  void bhvm_hf3_vm_op_ar1_softplus_s_set_arg( bhvm_hf3_vm_op_ar1_softplus_s* o, char id, sz_t idx ); \
  static inline sc_t bhvm_hf3_vm_op_ar1_softplus_s_sig( const bhvm_hf3_vm_op_ar1_softplus_s* o ){return "ay";}
#define TYPEOF_bhvm_hf3_vm_op_ar1_relu_s 1254389858
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_relu_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar1_relu_s ) \
    {aware_t _;sz_t a;sz_t b;}; \
  static inline sz_t bhvm_hf3_vm_op_ar1_relu_s_get_arity( const bhvm_hf3_vm_op_ar1_relu_s* o ){return 1;} \
  static inline void bhvm_hf3_vm_op_ar1_relu_s_set_indices( bhvm_hf3_vm_op_ar1_relu_s* o, sz_t* a ){o->a = a[0]; o->b = a[1];} \
  static inline void bhvm_hf3_vm_op_ar1_relu_s_get_indices( const bhvm_hf3_vm_op_ar1_relu_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar1_relu_s_csetup( bhvm_hf3_vm_op_ar1_relu_s* o, sz_t idx_a, sz_t idx_b ){if( !o ) o = bhvm_hf3_vm_op_ar1_relu_s_create(); o->a = idx_a; o->b = idx_b; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar1_relu_s_run( const bhvm_hf3_vm_op_ar1_relu_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar1_relu_s_f      ( &ah[o->a].h, &ah[o->b].h );} \
  void bhvm_hf3_vm_op_ar1_relu_s_set_arg( bhvm_hf3_vm_op_ar1_relu_s* o, char id, sz_t idx ); \
  static inline sc_t bhvm_hf3_vm_op_ar1_relu_s_sig( const bhvm_hf3_vm_op_ar1_relu_s* o ){return "ay";}
#define TYPEOF_bhvm_hf3_vm_op_ar1_relu_leaky_s 967685463
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_relu_leaky_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar1_relu_leaky_s ) \
    {aware_t _;sz_t a;sz_t b;}; \
  static inline sz_t bhvm_hf3_vm_op_ar1_relu_leaky_s_get_arity( const bhvm_hf3_vm_op_ar1_relu_leaky_s* o ){return 1;} \
  static inline void bhvm_hf3_vm_op_ar1_relu_leaky_s_set_indices( bhvm_hf3_vm_op_ar1_relu_leaky_s* o, sz_t* a ){o->a = a[0]; o->b = a[1];} \
  static inline void bhvm_hf3_vm_op_ar1_relu_leaky_s_get_indices( const bhvm_hf3_vm_op_ar1_relu_leaky_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar1_relu_leaky_s_csetup( bhvm_hf3_vm_op_ar1_relu_leaky_s* o, sz_t idx_a, sz_t idx_b ){if( !o ) o = bhvm_hf3_vm_op_ar1_relu_leaky_s_create(); o->a = idx_a; o->b = idx_b; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar1_relu_leaky_s_run( const bhvm_hf3_vm_op_ar1_relu_leaky_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar1_relu_leaky_s_f( &ah[o->a].h, &ah[o->b].h );} \
  void bhvm_hf3_vm_op_ar1_relu_leaky_s_set_arg( bhvm_hf3_vm_op_ar1_relu_leaky_s* o, char id, sz_t idx ); \
  static inline sc_t bhvm_hf3_vm_op_ar1_relu_leaky_s_sig( const bhvm_hf3_vm_op_ar1_relu_leaky_s* o ){return "ay";}
#define TYPEOF_bhvm_hf3_vm_op_ar1_cast_htp_s 4267632328
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_cast_htp_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar1_cast_htp_s ) \
    {aware_t _;sz_t a;sz_t b;}; \
  static inline sz_t bhvm_hf3_vm_op_ar1_cast_htp_s_get_arity( const bhvm_hf3_vm_op_ar1_cast_htp_s* o ){return 1;} \
  static inline void bhvm_hf3_vm_op_ar1_cast_htp_s_set_indices( bhvm_hf3_vm_op_ar1_cast_htp_s* o, sz_t* a ){o->a = a[0]; o->b = a[1];} \
  static inline void bhvm_hf3_vm_op_ar1_cast_htp_s_get_indices( const bhvm_hf3_vm_op_ar1_cast_htp_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar1_cast_htp_s_csetup( bhvm_hf3_vm_op_ar1_cast_htp_s* o, sz_t idx_a, sz_t idx_b ){if( !o ) o = bhvm_hf3_vm_op_ar1_cast_htp_s_create(); o->a = idx_a; o->b = idx_b; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar1_cast_htp_s_run( const bhvm_hf3_vm_op_ar1_cast_htp_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar1_cast_htp_s_f( &ah[o->a].h, &ah[o->b].h );} \
  void bhvm_hf3_vm_op_ar1_cast_htp_s_set_arg( bhvm_hf3_vm_op_ar1_cast_htp_s* o, char id, sz_t idx ); \
  static inline sc_t bhvm_hf3_vm_op_ar1_cast_htp_s_sig( const bhvm_hf3_vm_op_ar1_cast_htp_s* o ){return "ay";}
#define BETH_EXPAND_GROUP_bhvm_hf3_vm_op_ar1 \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar1 ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar1_unary_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar1_cpy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar1_neg_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar1_floor_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar1_ceil_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar1_exp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar1_cpy_ay_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar1_cpy_by_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar1_lgst_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar1_lgst_hard_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar1_lgst_leaky_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar1_tanh_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar1_tanh_hard_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar1_tanh_leaky_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar1_softplus_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar1_relu_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar1_relu_leaky_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar1_cast_htp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar1_dp ); \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_unary_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_cpy_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_neg_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_floor_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_ceil_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_exp_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_cpy_ay_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_cpy_by_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_lgst_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_lgst_hard_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_lgst_leaky_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_tanh_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_tanh_hard_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_tanh_leaky_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_softplus_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_relu_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_relu_leaky_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_cast_htp_s \
  BETH_EXPAND_GROUP_bhvm_hf3_vm_op_ar1_dp

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_vm_op_ar1_dp

#define TYPEOF_bhvm_hf3_vm_op_ar1_dp 3403601098
#define TYPEOF_bhvm_hf3_vm_op_ar1_dp_s 2598297268
#define TYPEOF_bhvm_hf3_vm_op_ar1_dp_ca_cpy_s 79187336
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_dp_ca_cpy_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar1_dp_ca_cpy_s ) \
    {aware_t _;sz_t a;sz_t b;}; \
  static inline sz_t bhvm_hf3_vm_op_ar1_dp_ca_cpy_s_get_arity( const bhvm_hf3_vm_op_ar1_dp_ca_cpy_s* o ){return 1;} \
  static inline void bhvm_hf3_vm_op_ar1_dp_ca_cpy_s_set_indices( bhvm_hf3_vm_op_ar1_dp_ca_cpy_s* o, sz_t* a ){o->a = a[0]; o->b = a[1];} \
  static inline void bhvm_hf3_vm_op_ar1_dp_ca_cpy_s_get_indices( const bhvm_hf3_vm_op_ar1_dp_ca_cpy_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar1_dp_ca_cpy_s_csetup( bhvm_hf3_vm_op_ar1_dp_ca_cpy_s* o, sz_t idx_a, sz_t idx_b ){if( !o ) o = bhvm_hf3_vm_op_ar1_dp_ca_cpy_s_create(); o->a = idx_a; o->b = idx_b; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar1_dp_ca_cpy_s_run( const bhvm_hf3_vm_op_ar1_dp_ca_cpy_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar1_dp_ca_cpy_s_f( &ah[o->a].h, &ah[o->b].h );} \
  static inline sc_t bhvm_hf3_vm_op_ar1_dp_ca_cpy_s_sig( const bhvm_hf3_vm_op_ar1_dp_ca_cpy_s* o ){return "vu";} \
  void bhvm_hf3_vm_op_ar1_dp_ca_cpy_s_set_arg( bhvm_hf3_vm_op_ar1_dp_ca_cpy_s* o, char id, sz_t idx );
#define TYPEOF_bhvm_hf3_vm_op_ar1_dp_ca_neg_s 2354317744
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_dp_ca_neg_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar1_dp_ca_neg_s ) \
    {aware_t _;sz_t a;sz_t b;}; \
  static inline sz_t bhvm_hf3_vm_op_ar1_dp_ca_neg_s_get_arity( const bhvm_hf3_vm_op_ar1_dp_ca_neg_s* o ){return 1;} \
  static inline void bhvm_hf3_vm_op_ar1_dp_ca_neg_s_set_indices( bhvm_hf3_vm_op_ar1_dp_ca_neg_s* o, sz_t* a ){o->a = a[0]; o->b = a[1];} \
  static inline void bhvm_hf3_vm_op_ar1_dp_ca_neg_s_get_indices( const bhvm_hf3_vm_op_ar1_dp_ca_neg_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar1_dp_ca_neg_s_csetup( bhvm_hf3_vm_op_ar1_dp_ca_neg_s* o, sz_t idx_a, sz_t idx_b ){if( !o ) o = bhvm_hf3_vm_op_ar1_dp_ca_neg_s_create(); o->a = idx_a; o->b = idx_b; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar1_dp_ca_neg_s_run( const bhvm_hf3_vm_op_ar1_dp_ca_neg_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar1_dp_ca_neg_s_f( &ah[o->a].h, &ah[o->b].h );} \
  static inline sc_t bhvm_hf3_vm_op_ar1_dp_ca_neg_s_sig( const bhvm_hf3_vm_op_ar1_dp_ca_neg_s* o ){return "vu";} \
  void bhvm_hf3_vm_op_ar1_dp_ca_neg_s_set_arg( bhvm_hf3_vm_op_ar1_dp_ca_neg_s* o, char id, sz_t idx );
#define TYPEOF_bhvm_hf3_vm_op_ar1_dp_ca_add_s 1464649029
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_dp_ca_add_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar1_dp_ca_add_s ) \
    {aware_t _;sz_t a;sz_t b;}; \
  static inline sz_t bhvm_hf3_vm_op_ar1_dp_ca_add_s_get_arity( const bhvm_hf3_vm_op_ar1_dp_ca_add_s* o ){return 1;} \
  static inline void bhvm_hf3_vm_op_ar1_dp_ca_add_s_set_indices( bhvm_hf3_vm_op_ar1_dp_ca_add_s* o, sz_t* a ){o->a = a[0]; o->b = a[1];} \
  static inline void bhvm_hf3_vm_op_ar1_dp_ca_add_s_get_indices( const bhvm_hf3_vm_op_ar1_dp_ca_add_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar1_dp_ca_add_s_csetup( bhvm_hf3_vm_op_ar1_dp_ca_add_s* o, sz_t idx_a, sz_t idx_b ){if( !o ) o = bhvm_hf3_vm_op_ar1_dp_ca_add_s_create(); o->a = idx_a; o->b = idx_b; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar1_dp_ca_add_s_run( const bhvm_hf3_vm_op_ar1_dp_ca_add_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar1_dp_ca_add_s_f( &ah[o->a].h, &ah[o->b].h );} \
  static inline sc_t bhvm_hf3_vm_op_ar1_dp_ca_add_s_sig( const bhvm_hf3_vm_op_ar1_dp_ca_add_s* o ){return "vu";} \
  void bhvm_hf3_vm_op_ar1_dp_ca_add_s_set_arg( bhvm_hf3_vm_op_ar1_dp_ca_add_s* o, char id, sz_t idx );
#define TYPEOF_bhvm_hf3_vm_op_ar1_dp_cb_add_s 1611453404
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_dp_cb_add_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar1_dp_cb_add_s ) \
    {aware_t _;sz_t a;sz_t b;}; \
  static inline sz_t bhvm_hf3_vm_op_ar1_dp_cb_add_s_get_arity( const bhvm_hf3_vm_op_ar1_dp_cb_add_s* o ){return 1;} \
  static inline void bhvm_hf3_vm_op_ar1_dp_cb_add_s_set_indices( bhvm_hf3_vm_op_ar1_dp_cb_add_s* o, sz_t* a ){o->a = a[0]; o->b = a[1];} \
  static inline void bhvm_hf3_vm_op_ar1_dp_cb_add_s_get_indices( const bhvm_hf3_vm_op_ar1_dp_cb_add_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar1_dp_cb_add_s_csetup( bhvm_hf3_vm_op_ar1_dp_cb_add_s* o, sz_t idx_a, sz_t idx_b ){if( !o ) o = bhvm_hf3_vm_op_ar1_dp_cb_add_s_create(); o->a = idx_a; o->b = idx_b; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar1_dp_cb_add_s_run( const bhvm_hf3_vm_op_ar1_dp_cb_add_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar1_dp_cb_add_s_f( &ah[o->a].h, &ah[o->b].h );} \
  static inline sc_t bhvm_hf3_vm_op_ar1_dp_cb_add_s_sig( const bhvm_hf3_vm_op_ar1_dp_cb_add_s* o ){return "vu";} \
  void bhvm_hf3_vm_op_ar1_dp_cb_add_s_set_arg( bhvm_hf3_vm_op_ar1_dp_cb_add_s* o, char id, sz_t idx );
#define TYPEOF_bhvm_hf3_vm_op_ar1_dp_ca_sub_s 838990012
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_dp_ca_sub_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar1_dp_ca_sub_s ) \
    {aware_t _;sz_t a;sz_t b;}; \
  static inline sz_t bhvm_hf3_vm_op_ar1_dp_ca_sub_s_get_arity( const bhvm_hf3_vm_op_ar1_dp_ca_sub_s* o ){return 1;} \
  static inline void bhvm_hf3_vm_op_ar1_dp_ca_sub_s_set_indices( bhvm_hf3_vm_op_ar1_dp_ca_sub_s* o, sz_t* a ){o->a = a[0]; o->b = a[1];} \
  static inline void bhvm_hf3_vm_op_ar1_dp_ca_sub_s_get_indices( const bhvm_hf3_vm_op_ar1_dp_ca_sub_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar1_dp_ca_sub_s_csetup( bhvm_hf3_vm_op_ar1_dp_ca_sub_s* o, sz_t idx_a, sz_t idx_b ){if( !o ) o = bhvm_hf3_vm_op_ar1_dp_ca_sub_s_create(); o->a = idx_a; o->b = idx_b; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar1_dp_ca_sub_s_run( const bhvm_hf3_vm_op_ar1_dp_ca_sub_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar1_dp_ca_sub_s_f( &ah[o->a].h, &ah[o->b].h );} \
  static inline sc_t bhvm_hf3_vm_op_ar1_dp_ca_sub_s_sig( const bhvm_hf3_vm_op_ar1_dp_ca_sub_s* o ){return "vu";} \
  void bhvm_hf3_vm_op_ar1_dp_ca_sub_s_set_arg( bhvm_hf3_vm_op_ar1_dp_ca_sub_s* o, char id, sz_t idx );
#define TYPEOF_bhvm_hf3_vm_op_ar1_dp_cb_sub_s 2045117741
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_dp_cb_sub_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar1_dp_cb_sub_s ) \
    {aware_t _;sz_t a;sz_t b;}; \
  static inline sz_t bhvm_hf3_vm_op_ar1_dp_cb_sub_s_get_arity( const bhvm_hf3_vm_op_ar1_dp_cb_sub_s* o ){return 1;} \
  static inline void bhvm_hf3_vm_op_ar1_dp_cb_sub_s_set_indices( bhvm_hf3_vm_op_ar1_dp_cb_sub_s* o, sz_t* a ){o->a = a[0]; o->b = a[1];} \
  static inline void bhvm_hf3_vm_op_ar1_dp_cb_sub_s_get_indices( const bhvm_hf3_vm_op_ar1_dp_cb_sub_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar1_dp_cb_sub_s_csetup( bhvm_hf3_vm_op_ar1_dp_cb_sub_s* o, sz_t idx_a, sz_t idx_b ){if( !o ) o = bhvm_hf3_vm_op_ar1_dp_cb_sub_s_create(); o->a = idx_a; o->b = idx_b; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar1_dp_cb_sub_s_run( const bhvm_hf3_vm_op_ar1_dp_cb_sub_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar1_dp_cb_sub_s_f( &ah[o->a].h, &ah[o->b].h );} \
  static inline sc_t bhvm_hf3_vm_op_ar1_dp_cb_sub_s_sig( const bhvm_hf3_vm_op_ar1_dp_cb_sub_s* o ){return "vu";} \
  void bhvm_hf3_vm_op_ar1_dp_cb_sub_s_set_arg( bhvm_hf3_vm_op_ar1_dp_cb_sub_s* o, char id, sz_t idx );
#define TYPEOF_bhvm_hf3_vm_op_ar1_dp_ca_cast_htp_s 3380699054
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_dp_ca_cast_htp_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar1_dp_ca_cast_htp_s ) \
    {aware_t _;sz_t a;sz_t b;}; \
  static inline sz_t bhvm_hf3_vm_op_ar1_dp_ca_cast_htp_s_get_arity( const bhvm_hf3_vm_op_ar1_dp_ca_cast_htp_s* o ){return 1;} \
  static inline void bhvm_hf3_vm_op_ar1_dp_ca_cast_htp_s_set_indices( bhvm_hf3_vm_op_ar1_dp_ca_cast_htp_s* o, sz_t* a ){o->a = a[0]; o->b = a[1];} \
  static inline void bhvm_hf3_vm_op_ar1_dp_ca_cast_htp_s_get_indices( const bhvm_hf3_vm_op_ar1_dp_ca_cast_htp_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar1_dp_ca_cast_htp_s_csetup( bhvm_hf3_vm_op_ar1_dp_ca_cast_htp_s* o, sz_t idx_a, sz_t idx_b ){if( !o ) o = bhvm_hf3_vm_op_ar1_dp_ca_cast_htp_s_create(); o->a = idx_a; o->b = idx_b; return (bhvm_hf3_vm_op*)o;} \
  static inline sc_t bhvm_hf3_vm_op_ar1_dp_ca_cast_htp_s_sig( const bhvm_hf3_vm_op_ar1_dp_ca_cast_htp_s* o ){return bhvm_hf3_op_ar1_dp_ca_cast_htp_s_sig();} \
  static inline void bhvm_hf3_vm_op_ar1_dp_ca_cast_htp_s_run( const bhvm_hf3_vm_op_ar1_dp_ca_cast_htp_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar1_dp_ca_cast_htp_s_f( &ah[o->a].h, &ah[o->b].h );} \
  void bhvm_hf3_vm_op_ar1_dp_ca_cast_htp_s_set_arg( bhvm_hf3_vm_op_ar1_dp_ca_cast_htp_s* o, char id, sz_t idx );
#define BETH_EXPAND_GROUP_bhvm_hf3_vm_op_ar1_dp \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar1_dp ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar1_dp_ca_cpy_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar1_dp_ca_neg_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar1_dp_ca_add_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar1_dp_cb_add_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar1_dp_ca_sub_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar1_dp_cb_sub_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar1_dp_ca_cast_htp_s ); \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_dp_ca_cpy_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_dp_ca_neg_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_dp_ca_add_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_dp_cb_add_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_dp_ca_sub_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_dp_cb_sub_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar1_dp_ca_cast_htp_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_vm_op_ar2

#define TYPEOF_bhvm_hf3_vm_op_ar2 418095244
#define TYPEOF_bhvm_hf3_vm_op_ar2_s 1206899118
#define TYPEOF_bhvm_hf3_vm_op_ar2_add_s 1381170762
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_add_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar2_add_s ) \
    {aware_t _;sz_t a;sz_t b;sz_t c;}; \
  static inline sz_t bhvm_hf3_vm_op_ar2_add_s_get_arity( const bhvm_hf3_vm_op_ar2_add_s* o ){return 2;} \
  static inline void bhvm_hf3_vm_op_ar2_add_s_set_indices( bhvm_hf3_vm_op_ar2_add_s* o, sz_t* a ){o->a = a[0]; o->b = a[1]; o->c = a[2];} \
  static inline void bhvm_hf3_vm_op_ar2_add_s_get_indices( const bhvm_hf3_vm_op_ar2_add_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b; a[2] = o->c;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar2_add_s_csetup( bhvm_hf3_vm_op_ar2_add_s* o, sz_t idx_a, sz_t idx_b, sz_t idx_c ){if( !o ) o = bhvm_hf3_vm_op_ar2_add_s_create(); o->a = idx_a; o->b = idx_b; o->c = idx_c; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar2_add_s_run( const bhvm_hf3_vm_op_ar2_add_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar2_add_s_f(     &ah[o->a].h, &ah[o->b].h, &ah[o->c].h );} \
  void bhvm_hf3_vm_op_ar2_add_s_set_arg( bhvm_hf3_vm_op_ar2_add_s* o, char id, sz_t idx ); \
  static inline sc_t bhvm_hf3_vm_op_ar2_add_s_sig( const bhvm_hf3_vm_op_ar2_add_s* o ){return "aby";}
#define TYPEOF_bhvm_hf3_vm_op_ar2_sub_s 2879155667
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_sub_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar2_sub_s ) \
    {aware_t _;sz_t a;sz_t b;sz_t c;}; \
  static inline sz_t bhvm_hf3_vm_op_ar2_sub_s_get_arity( const bhvm_hf3_vm_op_ar2_sub_s* o ){return 2;} \
  static inline void bhvm_hf3_vm_op_ar2_sub_s_set_indices( bhvm_hf3_vm_op_ar2_sub_s* o, sz_t* a ){o->a = a[0]; o->b = a[1]; o->c = a[2];} \
  static inline void bhvm_hf3_vm_op_ar2_sub_s_get_indices( const bhvm_hf3_vm_op_ar2_sub_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b; a[2] = o->c;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar2_sub_s_csetup( bhvm_hf3_vm_op_ar2_sub_s* o, sz_t idx_a, sz_t idx_b, sz_t idx_c ){if( !o ) o = bhvm_hf3_vm_op_ar2_sub_s_create(); o->a = idx_a; o->b = idx_b; o->c = idx_c; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar2_sub_s_run( const bhvm_hf3_vm_op_ar2_sub_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar2_sub_s_f(     &ah[o->a].h, &ah[o->b].h, &ah[o->c].h );} \
  void bhvm_hf3_vm_op_ar2_sub_s_set_arg( bhvm_hf3_vm_op_ar2_sub_s* o, char id, sz_t idx ); \
  static inline sc_t bhvm_hf3_vm_op_ar2_sub_s_sig( const bhvm_hf3_vm_op_ar2_sub_s* o ){return "aby";}
#define TYPEOF_bhvm_hf3_vm_op_ar2_hmul_s 3452639589
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_hmul_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar2_hmul_s ) \
    {aware_t _;sz_t a;sz_t b;sz_t c;}; \
  static inline sz_t bhvm_hf3_vm_op_ar2_hmul_s_get_arity( const bhvm_hf3_vm_op_ar2_hmul_s* o ){return 2;} \
  static inline void bhvm_hf3_vm_op_ar2_hmul_s_set_indices( bhvm_hf3_vm_op_ar2_hmul_s* o, sz_t* a ){o->a = a[0]; o->b = a[1]; o->c = a[2];} \
  static inline void bhvm_hf3_vm_op_ar2_hmul_s_get_indices( const bhvm_hf3_vm_op_ar2_hmul_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b; a[2] = o->c;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar2_hmul_s_csetup( bhvm_hf3_vm_op_ar2_hmul_s* o, sz_t idx_a, sz_t idx_b, sz_t idx_c ){if( !o ) o = bhvm_hf3_vm_op_ar2_hmul_s_create(); o->a = idx_a; o->b = idx_b; o->c = idx_c; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar2_hmul_s_run( const bhvm_hf3_vm_op_ar2_hmul_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar2_hmul_s_f(    &ah[o->a].h, &ah[o->b].h, &ah[o->c].h );} \
  void bhvm_hf3_vm_op_ar2_hmul_s_set_arg( bhvm_hf3_vm_op_ar2_hmul_s* o, char id, sz_t idx ); \
  static inline sc_t bhvm_hf3_vm_op_ar2_hmul_s_sig( const bhvm_hf3_vm_op_ar2_hmul_s* o ){return "aby";}
#define TYPEOF_bhvm_hf3_vm_op_ar2_hdiv_s 1570809560
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_hdiv_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar2_hdiv_s ) \
    {aware_t _;sz_t a;sz_t b;sz_t c;}; \
  static inline sz_t bhvm_hf3_vm_op_ar2_hdiv_s_get_arity( const bhvm_hf3_vm_op_ar2_hdiv_s* o ){return 2;} \
  static inline void bhvm_hf3_vm_op_ar2_hdiv_s_set_indices( bhvm_hf3_vm_op_ar2_hdiv_s* o, sz_t* a ){o->a = a[0]; o->b = a[1]; o->c = a[2];} \
  static inline void bhvm_hf3_vm_op_ar2_hdiv_s_get_indices( const bhvm_hf3_vm_op_ar2_hdiv_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b; a[2] = o->c;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar2_hdiv_s_csetup( bhvm_hf3_vm_op_ar2_hdiv_s* o, sz_t idx_a, sz_t idx_b, sz_t idx_c ){if( !o ) o = bhvm_hf3_vm_op_ar2_hdiv_s_create(); o->a = idx_a; o->b = idx_b; o->c = idx_c; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar2_hdiv_s_run( const bhvm_hf3_vm_op_ar2_hdiv_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar2_hdiv_s_f(    &ah[o->a].h, &ah[o->b].h, &ah[o->c].h );} \
  void bhvm_hf3_vm_op_ar2_hdiv_s_set_arg( bhvm_hf3_vm_op_ar2_hdiv_s* o, char id, sz_t idx ); \
  static inline sc_t bhvm_hf3_vm_op_ar2_hdiv_s_sig( const bhvm_hf3_vm_op_ar2_hdiv_s* o ){return "aby";}
#define TYPEOF_bhvm_hf3_vm_op_ar2_bmul_s 1612377823
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_bmul_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar2_bmul_s ) \
    {aware_t _;sz_t a;sz_t b;sz_t c;}; \
  static inline sz_t bhvm_hf3_vm_op_ar2_bmul_s_get_arity( const bhvm_hf3_vm_op_ar2_bmul_s* o ){return 2;} \
  static inline void bhvm_hf3_vm_op_ar2_bmul_s_set_indices( bhvm_hf3_vm_op_ar2_bmul_s* o, sz_t* a ){o->a = a[0]; o->b = a[1]; o->c = a[2];} \
  static inline void bhvm_hf3_vm_op_ar2_bmul_s_get_indices( const bhvm_hf3_vm_op_ar2_bmul_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b; a[2] = o->c;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar2_bmul_s_csetup( bhvm_hf3_vm_op_ar2_bmul_s* o, sz_t idx_a, sz_t idx_b, sz_t idx_c ){if( !o ) o = bhvm_hf3_vm_op_ar2_bmul_s_create(); o->a = idx_a; o->b = idx_b; o->c = idx_c; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar2_bmul_s_run( const bhvm_hf3_vm_op_ar2_bmul_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar2_bmul_s_f(    &ah[o->a].h, &ah[o->b].h, &ah[o->c].h );} \
  void bhvm_hf3_vm_op_ar2_bmul_s_set_arg( bhvm_hf3_vm_op_ar2_bmul_s* o, char id, sz_t idx ); \
  static inline sc_t bhvm_hf3_vm_op_ar2_bmul_s_sig( const bhvm_hf3_vm_op_ar2_bmul_s* o ){return "aby";}
#define TYPEOF_bhvm_hf3_vm_op_ar2_mul_scl_s 2150904682
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_mul_scl_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar2_mul_scl_s ) \
    {aware_t _;sz_t a;sz_t b;sz_t c;}; \
  static inline sz_t bhvm_hf3_vm_op_ar2_mul_scl_s_get_arity( const bhvm_hf3_vm_op_ar2_mul_scl_s* o ){return 2;} \
  static inline void bhvm_hf3_vm_op_ar2_mul_scl_s_set_indices( bhvm_hf3_vm_op_ar2_mul_scl_s* o, sz_t* a ){o->a = a[0]; o->b = a[1]; o->c = a[2];} \
  static inline void bhvm_hf3_vm_op_ar2_mul_scl_s_get_indices( const bhvm_hf3_vm_op_ar2_mul_scl_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b; a[2] = o->c;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar2_mul_scl_s_csetup( bhvm_hf3_vm_op_ar2_mul_scl_s* o, sz_t idx_a, sz_t idx_b, sz_t idx_c ){if( !o ) o = bhvm_hf3_vm_op_ar2_mul_scl_s_create(); o->a = idx_a; o->b = idx_b; o->c = idx_c; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar2_mul_scl_s_run( const bhvm_hf3_vm_op_ar2_mul_scl_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar2_mul_scl_s_f( &ah[o->a].h, &ah[o->b].h, &ah[o->c].h );} \
  void bhvm_hf3_vm_op_ar2_mul_scl_s_set_arg( bhvm_hf3_vm_op_ar2_mul_scl_s* o, char id, sz_t idx ); \
  static inline sc_t bhvm_hf3_vm_op_ar2_mul_scl_s_sig( const bhvm_hf3_vm_op_ar2_mul_scl_s* o ){return "aby";}
#define TYPEOF_bhvm_hf3_vm_op_ar2_scl_mul_s 3667377846
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_scl_mul_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar2_scl_mul_s ) \
    {aware_t _;sz_t a;sz_t b;sz_t c;}; \
  static inline sz_t bhvm_hf3_vm_op_ar2_scl_mul_s_get_arity( const bhvm_hf3_vm_op_ar2_scl_mul_s* o ){return 2;} \
  static inline void bhvm_hf3_vm_op_ar2_scl_mul_s_set_indices( bhvm_hf3_vm_op_ar2_scl_mul_s* o, sz_t* a ){o->a = a[0]; o->b = a[1]; o->c = a[2];} \
  static inline void bhvm_hf3_vm_op_ar2_scl_mul_s_get_indices( const bhvm_hf3_vm_op_ar2_scl_mul_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b; a[2] = o->c;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar2_scl_mul_s_csetup( bhvm_hf3_vm_op_ar2_scl_mul_s* o, sz_t idx_a, sz_t idx_b, sz_t idx_c ){if( !o ) o = bhvm_hf3_vm_op_ar2_scl_mul_s_create(); o->a = idx_a; o->b = idx_b; o->c = idx_c; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar2_scl_mul_s_run( const bhvm_hf3_vm_op_ar2_scl_mul_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar2_scl_mul_s_f( &ah[o->a].h, &ah[o->b].h, &ah[o->c].h );} \
  void bhvm_hf3_vm_op_ar2_scl_mul_s_set_arg( bhvm_hf3_vm_op_ar2_scl_mul_s* o, char id, sz_t idx ); \
  static inline sc_t bhvm_hf3_vm_op_ar2_scl_mul_s_sig( const bhvm_hf3_vm_op_ar2_scl_mul_s* o ){return "aby";}
#define BETH_EXPAND_GROUP_bhvm_hf3_vm_op_ar2 \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar2 ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar2_add_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar2_sub_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar2_hmul_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar2_hdiv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar2_bmul_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar2_mul_scl_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar2_scl_mul_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar2_dp ); \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_add_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_sub_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_hmul_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_hdiv_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_bmul_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_mul_scl_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_scl_mul_s \
  BETH_EXPAND_GROUP_bhvm_hf3_vm_op_ar2_dp

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_vm_op_ar2_dp

#define TYPEOF_bhvm_hf3_vm_op_ar2_dp 241628869
#define TYPEOF_bhvm_hf3_vm_op_ar2_dp_s 1911818663
#define TYPEOF_bhvm_hf3_vm_op_ar2_dp_ca_exp_s 687239648
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_dp_ca_exp_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_exp_s ) \
    {aware_t _;sz_t a;sz_t b;sz_t c;}; \
  static inline sz_t bhvm_hf3_vm_op_ar2_dp_ca_exp_s_get_arity( const bhvm_hf3_vm_op_ar2_dp_ca_exp_s* o ){return 2;} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_exp_s_set_indices( bhvm_hf3_vm_op_ar2_dp_ca_exp_s* o, sz_t* a ){o->a = a[0]; o->b = a[1]; o->c = a[2];} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_exp_s_get_indices( const bhvm_hf3_vm_op_ar2_dp_ca_exp_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b; a[2] = o->c;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar2_dp_ca_exp_s_csetup( bhvm_hf3_vm_op_ar2_dp_ca_exp_s* o, sz_t idx_a, sz_t idx_b, sz_t idx_c ){if( !o ) o = bhvm_hf3_vm_op_ar2_dp_ca_exp_s_create(); o->a = idx_a; o->b = idx_b; o->c = idx_c; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_exp_s_run( const bhvm_hf3_vm_op_ar2_dp_ca_exp_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar2_dp_ca_exp_s_f(        &ah[o->a].h, &ah[o->b].h, &ah[o->c].h );} \
  static inline sc_t bhvm_hf3_vm_op_ar2_dp_ca_exp_s_sig( const bhvm_hf3_vm_op_ar2_dp_ca_exp_s* o ){return "yvu";} \
  void bhvm_hf3_vm_op_ar2_dp_ca_exp_s_set_arg( bhvm_hf3_vm_op_ar2_dp_ca_exp_s* o, char id, sz_t idx );
#define TYPEOF_bhvm_hf3_vm_op_ar2_dp_ca_lgst_s 1890628599
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_dp_ca_lgst_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_lgst_s ) \
    {aware_t _;sz_t a;sz_t b;sz_t c;}; \
  static inline sz_t bhvm_hf3_vm_op_ar2_dp_ca_lgst_s_get_arity( const bhvm_hf3_vm_op_ar2_dp_ca_lgst_s* o ){return 2;} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_lgst_s_set_indices( bhvm_hf3_vm_op_ar2_dp_ca_lgst_s* o, sz_t* a ){o->a = a[0]; o->b = a[1]; o->c = a[2];} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_lgst_s_get_indices( const bhvm_hf3_vm_op_ar2_dp_ca_lgst_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b; a[2] = o->c;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar2_dp_ca_lgst_s_csetup( bhvm_hf3_vm_op_ar2_dp_ca_lgst_s* o, sz_t idx_a, sz_t idx_b, sz_t idx_c ){if( !o ) o = bhvm_hf3_vm_op_ar2_dp_ca_lgst_s_create(); o->a = idx_a; o->b = idx_b; o->c = idx_c; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_lgst_s_run( const bhvm_hf3_vm_op_ar2_dp_ca_lgst_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar2_dp_ca_lgst_s_f(       &ah[o->a].h, &ah[o->b].h, &ah[o->c].h );} \
  static inline sc_t bhvm_hf3_vm_op_ar2_dp_ca_lgst_s_sig( const bhvm_hf3_vm_op_ar2_dp_ca_lgst_s* o ){return "yvu";} \
  void bhvm_hf3_vm_op_ar2_dp_ca_lgst_s_set_arg( bhvm_hf3_vm_op_ar2_dp_ca_lgst_s* o, char id, sz_t idx );
#define TYPEOF_bhvm_hf3_vm_op_ar2_dp_ca_lgst_hard_s 1734294361
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_dp_ca_lgst_hard_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_lgst_hard_s ) \
    {aware_t _;sz_t a;sz_t b;sz_t c;}; \
  static inline sz_t bhvm_hf3_vm_op_ar2_dp_ca_lgst_hard_s_get_arity( const bhvm_hf3_vm_op_ar2_dp_ca_lgst_hard_s* o ){return 2;} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_lgst_hard_s_set_indices( bhvm_hf3_vm_op_ar2_dp_ca_lgst_hard_s* o, sz_t* a ){o->a = a[0]; o->b = a[1]; o->c = a[2];} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_lgst_hard_s_get_indices( const bhvm_hf3_vm_op_ar2_dp_ca_lgst_hard_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b; a[2] = o->c;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar2_dp_ca_lgst_hard_s_csetup( bhvm_hf3_vm_op_ar2_dp_ca_lgst_hard_s* o, sz_t idx_a, sz_t idx_b, sz_t idx_c ){if( !o ) o = bhvm_hf3_vm_op_ar2_dp_ca_lgst_hard_s_create(); o->a = idx_a; o->b = idx_b; o->c = idx_c; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_lgst_hard_s_run( const bhvm_hf3_vm_op_ar2_dp_ca_lgst_hard_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar2_dp_ca_lgst_hard_s_f(  &ah[o->a].h, &ah[o->b].h, &ah[o->c].h );} \
  static inline sc_t bhvm_hf3_vm_op_ar2_dp_ca_lgst_hard_s_sig( const bhvm_hf3_vm_op_ar2_dp_ca_lgst_hard_s* o ){return "yvu";} \
  void bhvm_hf3_vm_op_ar2_dp_ca_lgst_hard_s_set_arg( bhvm_hf3_vm_op_ar2_dp_ca_lgst_hard_s* o, char id, sz_t idx );
#define TYPEOF_bhvm_hf3_vm_op_ar2_dp_ca_lgst_leaky_s 1423051210
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_dp_ca_lgst_leaky_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_lgst_leaky_s ) \
    {aware_t _;sz_t a;sz_t b;sz_t c;}; \
  static inline sz_t bhvm_hf3_vm_op_ar2_dp_ca_lgst_leaky_s_get_arity( const bhvm_hf3_vm_op_ar2_dp_ca_lgst_leaky_s* o ){return 2;} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_lgst_leaky_s_set_indices( bhvm_hf3_vm_op_ar2_dp_ca_lgst_leaky_s* o, sz_t* a ){o->a = a[0]; o->b = a[1]; o->c = a[2];} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_lgst_leaky_s_get_indices( const bhvm_hf3_vm_op_ar2_dp_ca_lgst_leaky_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b; a[2] = o->c;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar2_dp_ca_lgst_leaky_s_csetup( bhvm_hf3_vm_op_ar2_dp_ca_lgst_leaky_s* o, sz_t idx_a, sz_t idx_b, sz_t idx_c ){if( !o ) o = bhvm_hf3_vm_op_ar2_dp_ca_lgst_leaky_s_create(); o->a = idx_a; o->b = idx_b; o->c = idx_c; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_lgst_leaky_s_run( const bhvm_hf3_vm_op_ar2_dp_ca_lgst_leaky_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar2_dp_ca_lgst_leaky_s_f( &ah[o->a].h, &ah[o->b].h, &ah[o->c].h );} \
  static inline sc_t bhvm_hf3_vm_op_ar2_dp_ca_lgst_leaky_s_sig( const bhvm_hf3_vm_op_ar2_dp_ca_lgst_leaky_s* o ){return "yvu";} \
  void bhvm_hf3_vm_op_ar2_dp_ca_lgst_leaky_s_set_arg( bhvm_hf3_vm_op_ar2_dp_ca_lgst_leaky_s* o, char id, sz_t idx );
#define TYPEOF_bhvm_hf3_vm_op_ar2_dp_ca_tanh_s 3324329088
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_dp_ca_tanh_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_tanh_s ) \
    {aware_t _;sz_t a;sz_t b;sz_t c;}; \
  static inline sz_t bhvm_hf3_vm_op_ar2_dp_ca_tanh_s_get_arity( const bhvm_hf3_vm_op_ar2_dp_ca_tanh_s* o ){return 2;} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_tanh_s_set_indices( bhvm_hf3_vm_op_ar2_dp_ca_tanh_s* o, sz_t* a ){o->a = a[0]; o->b = a[1]; o->c = a[2];} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_tanh_s_get_indices( const bhvm_hf3_vm_op_ar2_dp_ca_tanh_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b; a[2] = o->c;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar2_dp_ca_tanh_s_csetup( bhvm_hf3_vm_op_ar2_dp_ca_tanh_s* o, sz_t idx_a, sz_t idx_b, sz_t idx_c ){if( !o ) o = bhvm_hf3_vm_op_ar2_dp_ca_tanh_s_create(); o->a = idx_a; o->b = idx_b; o->c = idx_c; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_tanh_s_run( const bhvm_hf3_vm_op_ar2_dp_ca_tanh_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar2_dp_ca_tanh_s_f(       &ah[o->a].h, &ah[o->b].h, &ah[o->c].h );} \
  static inline sc_t bhvm_hf3_vm_op_ar2_dp_ca_tanh_s_sig( const bhvm_hf3_vm_op_ar2_dp_ca_tanh_s* o ){return "yvu";} \
  void bhvm_hf3_vm_op_ar2_dp_ca_tanh_s_set_arg( bhvm_hf3_vm_op_ar2_dp_ca_tanh_s* o, char id, sz_t idx );
#define TYPEOF_bhvm_hf3_vm_op_ar2_dp_ca_tanh_hard_s 3499620456
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_dp_ca_tanh_hard_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_tanh_hard_s ) \
    {aware_t _;sz_t a;sz_t b;sz_t c;}; \
  static inline sz_t bhvm_hf3_vm_op_ar2_dp_ca_tanh_hard_s_get_arity( const bhvm_hf3_vm_op_ar2_dp_ca_tanh_hard_s* o ){return 2;} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_tanh_hard_s_set_indices( bhvm_hf3_vm_op_ar2_dp_ca_tanh_hard_s* o, sz_t* a ){o->a = a[0]; o->b = a[1]; o->c = a[2];} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_tanh_hard_s_get_indices( const bhvm_hf3_vm_op_ar2_dp_ca_tanh_hard_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b; a[2] = o->c;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar2_dp_ca_tanh_hard_s_csetup( bhvm_hf3_vm_op_ar2_dp_ca_tanh_hard_s* o, sz_t idx_a, sz_t idx_b, sz_t idx_c ){if( !o ) o = bhvm_hf3_vm_op_ar2_dp_ca_tanh_hard_s_create(); o->a = idx_a; o->b = idx_b; o->c = idx_c; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_tanh_hard_s_run( const bhvm_hf3_vm_op_ar2_dp_ca_tanh_hard_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar2_dp_ca_tanh_hard_s_f(  &ah[o->a].h, &ah[o->b].h, &ah[o->c].h );} \
  static inline sc_t bhvm_hf3_vm_op_ar2_dp_ca_tanh_hard_s_sig( const bhvm_hf3_vm_op_ar2_dp_ca_tanh_hard_s* o ){return "yvu";} \
  void bhvm_hf3_vm_op_ar2_dp_ca_tanh_hard_s_set_arg( bhvm_hf3_vm_op_ar2_dp_ca_tanh_hard_s* o, char id, sz_t idx );
#define TYPEOF_bhvm_hf3_vm_op_ar2_dp_ca_tanh_leaky_s 858384285
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_dp_ca_tanh_leaky_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_tanh_leaky_s ) \
    {aware_t _;sz_t a;sz_t b;sz_t c;}; \
  static inline sz_t bhvm_hf3_vm_op_ar2_dp_ca_tanh_leaky_s_get_arity( const bhvm_hf3_vm_op_ar2_dp_ca_tanh_leaky_s* o ){return 2;} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_tanh_leaky_s_set_indices( bhvm_hf3_vm_op_ar2_dp_ca_tanh_leaky_s* o, sz_t* a ){o->a = a[0]; o->b = a[1]; o->c = a[2];} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_tanh_leaky_s_get_indices( const bhvm_hf3_vm_op_ar2_dp_ca_tanh_leaky_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b; a[2] = o->c;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar2_dp_ca_tanh_leaky_s_csetup( bhvm_hf3_vm_op_ar2_dp_ca_tanh_leaky_s* o, sz_t idx_a, sz_t idx_b, sz_t idx_c ){if( !o ) o = bhvm_hf3_vm_op_ar2_dp_ca_tanh_leaky_s_create(); o->a = idx_a; o->b = idx_b; o->c = idx_c; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_tanh_leaky_s_run( const bhvm_hf3_vm_op_ar2_dp_ca_tanh_leaky_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar2_dp_ca_tanh_leaky_s_f( &ah[o->a].h, &ah[o->b].h, &ah[o->c].h );} \
  static inline sc_t bhvm_hf3_vm_op_ar2_dp_ca_tanh_leaky_s_sig( const bhvm_hf3_vm_op_ar2_dp_ca_tanh_leaky_s* o ){return "yvu";} \
  void bhvm_hf3_vm_op_ar2_dp_ca_tanh_leaky_s_set_arg( bhvm_hf3_vm_op_ar2_dp_ca_tanh_leaky_s* o, char id, sz_t idx );
#define TYPEOF_bhvm_hf3_vm_op_ar2_dp_ca_softplus_s 3219719705
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_dp_ca_softplus_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_softplus_s ) \
    {aware_t _;sz_t a;sz_t b;sz_t c;}; \
  static inline sz_t bhvm_hf3_vm_op_ar2_dp_ca_softplus_s_get_arity( const bhvm_hf3_vm_op_ar2_dp_ca_softplus_s* o ){return 2;} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_softplus_s_set_indices( bhvm_hf3_vm_op_ar2_dp_ca_softplus_s* o, sz_t* a ){o->a = a[0]; o->b = a[1]; o->c = a[2];} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_softplus_s_get_indices( const bhvm_hf3_vm_op_ar2_dp_ca_softplus_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b; a[2] = o->c;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar2_dp_ca_softplus_s_csetup( bhvm_hf3_vm_op_ar2_dp_ca_softplus_s* o, sz_t idx_a, sz_t idx_b, sz_t idx_c ){if( !o ) o = bhvm_hf3_vm_op_ar2_dp_ca_softplus_s_create(); o->a = idx_a; o->b = idx_b; o->c = idx_c; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_softplus_s_run( const bhvm_hf3_vm_op_ar2_dp_ca_softplus_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar2_dp_ca_softplus_s_f(   &ah[o->a].h, &ah[o->b].h, &ah[o->c].h );} \
  static inline sc_t bhvm_hf3_vm_op_ar2_dp_ca_softplus_s_sig( const bhvm_hf3_vm_op_ar2_dp_ca_softplus_s* o ){return "yvu";} \
  void bhvm_hf3_vm_op_ar2_dp_ca_softplus_s_set_arg( bhvm_hf3_vm_op_ar2_dp_ca_softplus_s* o, char id, sz_t idx );
#define TYPEOF_bhvm_hf3_vm_op_ar2_dp_ca_relu_s 2218255251
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_dp_ca_relu_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_relu_s ) \
    {aware_t _;sz_t a;sz_t b;sz_t c;}; \
  static inline sz_t bhvm_hf3_vm_op_ar2_dp_ca_relu_s_get_arity( const bhvm_hf3_vm_op_ar2_dp_ca_relu_s* o ){return 2;} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_relu_s_set_indices( bhvm_hf3_vm_op_ar2_dp_ca_relu_s* o, sz_t* a ){o->a = a[0]; o->b = a[1]; o->c = a[2];} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_relu_s_get_indices( const bhvm_hf3_vm_op_ar2_dp_ca_relu_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b; a[2] = o->c;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar2_dp_ca_relu_s_csetup( bhvm_hf3_vm_op_ar2_dp_ca_relu_s* o, sz_t idx_a, sz_t idx_b, sz_t idx_c ){if( !o ) o = bhvm_hf3_vm_op_ar2_dp_ca_relu_s_create(); o->a = idx_a; o->b = idx_b; o->c = idx_c; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_relu_s_run( const bhvm_hf3_vm_op_ar2_dp_ca_relu_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar2_dp_ca_relu_s_f(       &ah[o->a].h, &ah[o->b].h, &ah[o->c].h );} \
  static inline sc_t bhvm_hf3_vm_op_ar2_dp_ca_relu_s_sig( const bhvm_hf3_vm_op_ar2_dp_ca_relu_s* o ){return "yvu";} \
  void bhvm_hf3_vm_op_ar2_dp_ca_relu_s_set_arg( bhvm_hf3_vm_op_ar2_dp_ca_relu_s* o, char id, sz_t idx );
#define TYPEOF_bhvm_hf3_vm_op_ar2_dp_ca_relu_leaky_s 1600631598
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_dp_ca_relu_leaky_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_relu_leaky_s ) \
    {aware_t _;sz_t a;sz_t b;sz_t c;}; \
  static inline sz_t bhvm_hf3_vm_op_ar2_dp_ca_relu_leaky_s_get_arity( const bhvm_hf3_vm_op_ar2_dp_ca_relu_leaky_s* o ){return 2;} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_relu_leaky_s_set_indices( bhvm_hf3_vm_op_ar2_dp_ca_relu_leaky_s* o, sz_t* a ){o->a = a[0]; o->b = a[1]; o->c = a[2];} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_relu_leaky_s_get_indices( const bhvm_hf3_vm_op_ar2_dp_ca_relu_leaky_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b; a[2] = o->c;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar2_dp_ca_relu_leaky_s_csetup( bhvm_hf3_vm_op_ar2_dp_ca_relu_leaky_s* o, sz_t idx_a, sz_t idx_b, sz_t idx_c ){if( !o ) o = bhvm_hf3_vm_op_ar2_dp_ca_relu_leaky_s_create(); o->a = idx_a; o->b = idx_b; o->c = idx_c; return (bhvm_hf3_vm_op*)o;} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_relu_leaky_s_run( const bhvm_hf3_vm_op_ar2_dp_ca_relu_leaky_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar2_dp_ca_relu_leaky_s_f( &ah[o->a].h, &ah[o->b].h, &ah[o->c].h );} \
  static inline sc_t bhvm_hf3_vm_op_ar2_dp_ca_relu_leaky_s_sig( const bhvm_hf3_vm_op_ar2_dp_ca_relu_leaky_s* o ){return "yvu";} \
  void bhvm_hf3_vm_op_ar2_dp_ca_relu_leaky_s_set_arg( bhvm_hf3_vm_op_ar2_dp_ca_relu_leaky_s* o, char id, sz_t idx );
#define TYPEOF_bhvm_hf3_vm_op_ar2_dp_ca_hmul_s 1325510595
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_dp_ca_hmul_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_hmul_s ) \
    {aware_t _;sz_t a;sz_t b;sz_t c;}; \
  static inline sz_t bhvm_hf3_vm_op_ar2_dp_ca_hmul_s_get_arity( const bhvm_hf3_vm_op_ar2_dp_ca_hmul_s* o ){return 2;} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_hmul_s_set_indices( bhvm_hf3_vm_op_ar2_dp_ca_hmul_s* o, sz_t* a ){o->a = a[0]; o->b = a[1]; o->c = a[2];} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_hmul_s_get_indices( const bhvm_hf3_vm_op_ar2_dp_ca_hmul_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b; a[2] = o->c;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar2_dp_ca_hmul_s_csetup( bhvm_hf3_vm_op_ar2_dp_ca_hmul_s* o, sz_t idx_a, sz_t idx_b, sz_t idx_c ){if( !o ) o = bhvm_hf3_vm_op_ar2_dp_ca_hmul_s_create(); o->a = idx_a; o->b = idx_b; o->c = idx_c; return (bhvm_hf3_vm_op*)o;} \
  static inline sc_t bhvm_hf3_vm_op_ar2_dp_ca_hmul_s_sig( const bhvm_hf3_vm_op_ar2_dp_ca_hmul_s* o ){return bhvm_hf3_op_ar2_dp_ca_hmul_s_sig();} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_hmul_s_run( const bhvm_hf3_vm_op_ar2_dp_ca_hmul_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar2_dp_ca_hmul_s_f( &ah[o->a].h, &ah[o->b].h, &ah[o->c].h );} \
  void bhvm_hf3_vm_op_ar2_dp_ca_hmul_s_set_arg( bhvm_hf3_vm_op_ar2_dp_ca_hmul_s* o, char id, sz_t idx );
#define TYPEOF_bhvm_hf3_vm_op_ar2_dp_cb_hmul_s 1234424712
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_dp_cb_hmul_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar2_dp_cb_hmul_s ) \
    {aware_t _;sz_t a;sz_t b;sz_t c;}; \
  static inline sz_t bhvm_hf3_vm_op_ar2_dp_cb_hmul_s_get_arity( const bhvm_hf3_vm_op_ar2_dp_cb_hmul_s* o ){return 2;} \
  static inline void bhvm_hf3_vm_op_ar2_dp_cb_hmul_s_set_indices( bhvm_hf3_vm_op_ar2_dp_cb_hmul_s* o, sz_t* a ){o->a = a[0]; o->b = a[1]; o->c = a[2];} \
  static inline void bhvm_hf3_vm_op_ar2_dp_cb_hmul_s_get_indices( const bhvm_hf3_vm_op_ar2_dp_cb_hmul_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b; a[2] = o->c;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar2_dp_cb_hmul_s_csetup( bhvm_hf3_vm_op_ar2_dp_cb_hmul_s* o, sz_t idx_a, sz_t idx_b, sz_t idx_c ){if( !o ) o = bhvm_hf3_vm_op_ar2_dp_cb_hmul_s_create(); o->a = idx_a; o->b = idx_b; o->c = idx_c; return (bhvm_hf3_vm_op*)o;} \
  static inline sc_t bhvm_hf3_vm_op_ar2_dp_cb_hmul_s_sig( const bhvm_hf3_vm_op_ar2_dp_cb_hmul_s* o ){return bhvm_hf3_op_ar2_dp_cb_hmul_s_sig();} \
  static inline void bhvm_hf3_vm_op_ar2_dp_cb_hmul_s_run( const bhvm_hf3_vm_op_ar2_dp_cb_hmul_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar2_dp_cb_hmul_s_f( &ah[o->a].h, &ah[o->b].h, &ah[o->c].h );} \
  void bhvm_hf3_vm_op_ar2_dp_cb_hmul_s_set_arg( bhvm_hf3_vm_op_ar2_dp_cb_hmul_s* o, char id, sz_t idx );
#define TYPEOF_bhvm_hf3_vm_op_ar2_dp_ca_bmul_s 2943988441
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_dp_ca_bmul_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_bmul_s ) \
    {aware_t _;sz_t a;sz_t b;sz_t c;}; \
  static inline sz_t bhvm_hf3_vm_op_ar2_dp_ca_bmul_s_get_arity( const bhvm_hf3_vm_op_ar2_dp_ca_bmul_s* o ){return 2;} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_bmul_s_set_indices( bhvm_hf3_vm_op_ar2_dp_ca_bmul_s* o, sz_t* a ){o->a = a[0]; o->b = a[1]; o->c = a[2];} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_bmul_s_get_indices( const bhvm_hf3_vm_op_ar2_dp_ca_bmul_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b; a[2] = o->c;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar2_dp_ca_bmul_s_csetup( bhvm_hf3_vm_op_ar2_dp_ca_bmul_s* o, sz_t idx_a, sz_t idx_b, sz_t idx_c ){if( !o ) o = bhvm_hf3_vm_op_ar2_dp_ca_bmul_s_create(); o->a = idx_a; o->b = idx_b; o->c = idx_c; return (bhvm_hf3_vm_op*)o;} \
  static inline sc_t bhvm_hf3_vm_op_ar2_dp_ca_bmul_s_sig( const bhvm_hf3_vm_op_ar2_dp_ca_bmul_s* o ){return bhvm_hf3_op_ar2_dp_ca_bmul_s_sig();} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_bmul_s_run( const bhvm_hf3_vm_op_ar2_dp_ca_bmul_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar2_dp_ca_bmul_s_f( &ah[o->a].h, &ah[o->b].h, &ah[o->c].h );} \
  void bhvm_hf3_vm_op_ar2_dp_ca_bmul_s_set_arg( bhvm_hf3_vm_op_ar2_dp_ca_bmul_s* o, char id, sz_t idx );
#define TYPEOF_bhvm_hf3_vm_op_ar2_dp_cb_bmul_s 3002030410
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_dp_cb_bmul_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar2_dp_cb_bmul_s ) \
    {aware_t _;sz_t a;sz_t b;sz_t c;}; \
  static inline sz_t bhvm_hf3_vm_op_ar2_dp_cb_bmul_s_get_arity( const bhvm_hf3_vm_op_ar2_dp_cb_bmul_s* o ){return 2;} \
  static inline void bhvm_hf3_vm_op_ar2_dp_cb_bmul_s_set_indices( bhvm_hf3_vm_op_ar2_dp_cb_bmul_s* o, sz_t* a ){o->a = a[0]; o->b = a[1]; o->c = a[2];} \
  static inline void bhvm_hf3_vm_op_ar2_dp_cb_bmul_s_get_indices( const bhvm_hf3_vm_op_ar2_dp_cb_bmul_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b; a[2] = o->c;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar2_dp_cb_bmul_s_csetup( bhvm_hf3_vm_op_ar2_dp_cb_bmul_s* o, sz_t idx_a, sz_t idx_b, sz_t idx_c ){if( !o ) o = bhvm_hf3_vm_op_ar2_dp_cb_bmul_s_create(); o->a = idx_a; o->b = idx_b; o->c = idx_c; return (bhvm_hf3_vm_op*)o;} \
  static inline sc_t bhvm_hf3_vm_op_ar2_dp_cb_bmul_s_sig( const bhvm_hf3_vm_op_ar2_dp_cb_bmul_s* o ){return bhvm_hf3_op_ar2_dp_cb_bmul_s_sig();} \
  static inline void bhvm_hf3_vm_op_ar2_dp_cb_bmul_s_run( const bhvm_hf3_vm_op_ar2_dp_cb_bmul_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar2_dp_cb_bmul_s_f( &ah[o->a].h, &ah[o->b].h, &ah[o->c].h );} \
  void bhvm_hf3_vm_op_ar2_dp_cb_bmul_s_set_arg( bhvm_hf3_vm_op_ar2_dp_cb_bmul_s* o, char id, sz_t idx );
#define TYPEOF_bhvm_hf3_vm_op_ar2_dp_ca_mul_scl_s 2569118688
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_dp_ca_mul_scl_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_mul_scl_s ) \
    {aware_t _;sz_t a;sz_t b;sz_t c;}; \
  static inline sz_t bhvm_hf3_vm_op_ar2_dp_ca_mul_scl_s_get_arity( const bhvm_hf3_vm_op_ar2_dp_ca_mul_scl_s* o ){return 2;} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_mul_scl_s_set_indices( bhvm_hf3_vm_op_ar2_dp_ca_mul_scl_s* o, sz_t* a ){o->a = a[0]; o->b = a[1]; o->c = a[2];} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_mul_scl_s_get_indices( const bhvm_hf3_vm_op_ar2_dp_ca_mul_scl_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b; a[2] = o->c;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar2_dp_ca_mul_scl_s_csetup( bhvm_hf3_vm_op_ar2_dp_ca_mul_scl_s* o, sz_t idx_a, sz_t idx_b, sz_t idx_c ){if( !o ) o = bhvm_hf3_vm_op_ar2_dp_ca_mul_scl_s_create(); o->a = idx_a; o->b = idx_b; o->c = idx_c; return (bhvm_hf3_vm_op*)o;} \
  static inline sc_t bhvm_hf3_vm_op_ar2_dp_ca_mul_scl_s_sig( const bhvm_hf3_vm_op_ar2_dp_ca_mul_scl_s* o ){return bhvm_hf3_op_ar2_dp_ca_mul_scl_s_sig();} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_mul_scl_s_run( const bhvm_hf3_vm_op_ar2_dp_ca_mul_scl_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar2_dp_ca_mul_scl_s_f( &ah[o->a].h, &ah[o->b].h, &ah[o->c].h );} \
  void bhvm_hf3_vm_op_ar2_dp_ca_mul_scl_s_set_arg( bhvm_hf3_vm_op_ar2_dp_ca_mul_scl_s* o, char id, sz_t idx );
#define TYPEOF_bhvm_hf3_vm_op_ar2_dp_cb_mul_scl_s 462451241
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_dp_cb_mul_scl_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar2_dp_cb_mul_scl_s ) \
    {aware_t _;sz_t a;sz_t b;sz_t c;}; \
  static inline sz_t bhvm_hf3_vm_op_ar2_dp_cb_mul_scl_s_get_arity( const bhvm_hf3_vm_op_ar2_dp_cb_mul_scl_s* o ){return 2;} \
  static inline void bhvm_hf3_vm_op_ar2_dp_cb_mul_scl_s_set_indices( bhvm_hf3_vm_op_ar2_dp_cb_mul_scl_s* o, sz_t* a ){o->a = a[0]; o->b = a[1]; o->c = a[2];} \
  static inline void bhvm_hf3_vm_op_ar2_dp_cb_mul_scl_s_get_indices( const bhvm_hf3_vm_op_ar2_dp_cb_mul_scl_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b; a[2] = o->c;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar2_dp_cb_mul_scl_s_csetup( bhvm_hf3_vm_op_ar2_dp_cb_mul_scl_s* o, sz_t idx_a, sz_t idx_b, sz_t idx_c ){if( !o ) o = bhvm_hf3_vm_op_ar2_dp_cb_mul_scl_s_create(); o->a = idx_a; o->b = idx_b; o->c = idx_c; return (bhvm_hf3_vm_op*)o;} \
  static inline sc_t bhvm_hf3_vm_op_ar2_dp_cb_mul_scl_s_sig( const bhvm_hf3_vm_op_ar2_dp_cb_mul_scl_s* o ){return bhvm_hf3_op_ar2_dp_cb_mul_scl_s_sig();} \
  static inline void bhvm_hf3_vm_op_ar2_dp_cb_mul_scl_s_run( const bhvm_hf3_vm_op_ar2_dp_cb_mul_scl_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar2_dp_cb_mul_scl_s_f( &ah[o->a].h, &ah[o->b].h, &ah[o->c].h );} \
  void bhvm_hf3_vm_op_ar2_dp_cb_mul_scl_s_set_arg( bhvm_hf3_vm_op_ar2_dp_cb_mul_scl_s* o, char id, sz_t idx );
#define TYPEOF_bhvm_hf3_vm_op_ar2_dp_ca_scl_mul_s 635018844
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_dp_ca_scl_mul_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_scl_mul_s ) \
    {aware_t _;sz_t a;sz_t b;sz_t c;}; \
  static inline sz_t bhvm_hf3_vm_op_ar2_dp_ca_scl_mul_s_get_arity( const bhvm_hf3_vm_op_ar2_dp_ca_scl_mul_s* o ){return 2;} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_scl_mul_s_set_indices( bhvm_hf3_vm_op_ar2_dp_ca_scl_mul_s* o, sz_t* a ){o->a = a[0]; o->b = a[1]; o->c = a[2];} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_scl_mul_s_get_indices( const bhvm_hf3_vm_op_ar2_dp_ca_scl_mul_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b; a[2] = o->c;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar2_dp_ca_scl_mul_s_csetup( bhvm_hf3_vm_op_ar2_dp_ca_scl_mul_s* o, sz_t idx_a, sz_t idx_b, sz_t idx_c ){if( !o ) o = bhvm_hf3_vm_op_ar2_dp_ca_scl_mul_s_create(); o->a = idx_a; o->b = idx_b; o->c = idx_c; return (bhvm_hf3_vm_op*)o;} \
  static inline sc_t bhvm_hf3_vm_op_ar2_dp_ca_scl_mul_s_sig( const bhvm_hf3_vm_op_ar2_dp_ca_scl_mul_s* o ){return bhvm_hf3_op_ar2_dp_ca_scl_mul_s_sig();} \
  static inline void bhvm_hf3_vm_op_ar2_dp_ca_scl_mul_s_run( const bhvm_hf3_vm_op_ar2_dp_ca_scl_mul_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar2_dp_ca_scl_mul_s_f( &ah[o->a].h, &ah[o->b].h, &ah[o->c].h );} \
  void bhvm_hf3_vm_op_ar2_dp_ca_scl_mul_s_set_arg( bhvm_hf3_vm_op_ar2_dp_ca_scl_mul_s* o, char id, sz_t idx );
#define TYPEOF_bhvm_hf3_vm_op_ar2_dp_cb_scl_mul_s 1803807341
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_dp_cb_scl_mul_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar2_dp_cb_scl_mul_s ) \
    {aware_t _;sz_t a;sz_t b;sz_t c;}; \
  static inline sz_t bhvm_hf3_vm_op_ar2_dp_cb_scl_mul_s_get_arity( const bhvm_hf3_vm_op_ar2_dp_cb_scl_mul_s* o ){return 2;} \
  static inline void bhvm_hf3_vm_op_ar2_dp_cb_scl_mul_s_set_indices( bhvm_hf3_vm_op_ar2_dp_cb_scl_mul_s* o, sz_t* a ){o->a = a[0]; o->b = a[1]; o->c = a[2];} \
  static inline void bhvm_hf3_vm_op_ar2_dp_cb_scl_mul_s_get_indices( const bhvm_hf3_vm_op_ar2_dp_cb_scl_mul_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b; a[2] = o->c;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar2_dp_cb_scl_mul_s_csetup( bhvm_hf3_vm_op_ar2_dp_cb_scl_mul_s* o, sz_t idx_a, sz_t idx_b, sz_t idx_c ){if( !o ) o = bhvm_hf3_vm_op_ar2_dp_cb_scl_mul_s_create(); o->a = idx_a; o->b = idx_b; o->c = idx_c; return (bhvm_hf3_vm_op*)o;} \
  static inline sc_t bhvm_hf3_vm_op_ar2_dp_cb_scl_mul_s_sig( const bhvm_hf3_vm_op_ar2_dp_cb_scl_mul_s* o ){return bhvm_hf3_op_ar2_dp_cb_scl_mul_s_sig();} \
  static inline void bhvm_hf3_vm_op_ar2_dp_cb_scl_mul_s_run( const bhvm_hf3_vm_op_ar2_dp_cb_scl_mul_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar2_dp_cb_scl_mul_s_f( &ah[o->a].h, &ah[o->b].h, &ah[o->c].h );} \
  void bhvm_hf3_vm_op_ar2_dp_cb_scl_mul_s_set_arg( bhvm_hf3_vm_op_ar2_dp_cb_scl_mul_s* o, char id, sz_t idx );
#define BETH_EXPAND_GROUP_bhvm_hf3_vm_op_ar2_dp \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar2_dp ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_exp_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_lgst_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_lgst_hard_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_lgst_leaky_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_tanh_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_tanh_hard_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_tanh_leaky_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_softplus_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_relu_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_relu_leaky_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_hmul_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar2_dp_cb_hmul_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_bmul_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar2_dp_cb_bmul_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_mul_scl_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar2_dp_cb_mul_scl_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_scl_mul_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar2_dp_cb_scl_mul_s ); \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_dp_ca_exp_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_dp_ca_lgst_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_dp_ca_lgst_hard_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_dp_ca_lgst_leaky_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_dp_ca_tanh_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_dp_ca_tanh_hard_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_dp_ca_tanh_leaky_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_dp_ca_softplus_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_dp_ca_relu_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_dp_ca_relu_leaky_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_dp_ca_hmul_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_dp_cb_hmul_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_dp_ca_bmul_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_dp_cb_bmul_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_dp_ca_mul_scl_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_dp_cb_mul_scl_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_dp_ca_scl_mul_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar2_dp_cb_scl_mul_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_vm_op_ar3

#define TYPEOF_bhvm_hf3_vm_op_ar3 434872863
#define TYPEOF_bhvm_hf3_vm_op_ar3_s 2759325641
#define BETH_EXPAND_GROUP_bhvm_hf3_vm_op_ar3 \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar3 ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar3_dp ); \
  BETH_EXPAND_GROUP_bhvm_hf3_vm_op_ar3_dp

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_vm_op_ar3_dp

#define TYPEOF_bhvm_hf3_vm_op_ar3_dp 2813735124
#define TYPEOF_bhvm_hf3_vm_op_ar3_dp_s 1894898950
#define TYPEOF_bhvm_hf3_vm_op_ar3_dp_ca_hdiv_s 1420475735
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar3_dp_ca_hdiv_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar3_dp_ca_hdiv_s ) \
    {aware_t _;sz_t a;sz_t b;sz_t c;sz_t d;}; \
  static inline sz_t bhvm_hf3_vm_op_ar3_dp_ca_hdiv_s_get_arity( const bhvm_hf3_vm_op_ar3_dp_ca_hdiv_s* o ){return 3;} \
  static inline void bhvm_hf3_vm_op_ar3_dp_ca_hdiv_s_set_indices( bhvm_hf3_vm_op_ar3_dp_ca_hdiv_s* o, sz_t* a ){o->a = a[0]; o->b = a[1]; o->c = a[2]; o->d = a[3];} \
  static inline void bhvm_hf3_vm_op_ar3_dp_ca_hdiv_s_get_indices( const bhvm_hf3_vm_op_ar3_dp_ca_hdiv_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b; a[2] = o->c; a[3] = o->d;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar3_dp_ca_hdiv_s_csetup( bhvm_hf3_vm_op_ar3_dp_ca_hdiv_s* o, sz_t idx_a, sz_t idx_b, sz_t idx_c, sz_t idx_d ){if( !o ) o = bhvm_hf3_vm_op_ar3_dp_ca_hdiv_s_create(); o->a = idx_a; o->b = idx_b; o->c = idx_c; o->d = idx_d; return (bhvm_hf3_vm_op*)o;} \
  static inline sc_t bhvm_hf3_vm_op_ar3_dp_ca_hdiv_s_sig( const bhvm_hf3_vm_op_ar3_dp_ca_hdiv_s* o ){return bhvm_hf3_op_ar3_dp_ca_hdiv_s_sig();} \
  static inline void bhvm_hf3_vm_op_ar3_dp_ca_hdiv_s_run( const bhvm_hf3_vm_op_ar3_dp_ca_hdiv_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar3_dp_ca_hdiv_s_f( &ah[o->a].h, &ah[o->b].h, &ah[o->c].h, &ah[o->d].h );} \
  void bhvm_hf3_vm_op_ar3_dp_ca_hdiv_s_set_arg( bhvm_hf3_vm_op_ar3_dp_ca_hdiv_s* o, char id, sz_t idx );
#define TYPEOF_bhvm_hf3_vm_op_ar3_dp_cb_hdiv_s 4252627112
#define BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar3_dp_cb_hdiv_s \
  BCORE_DECLARE_OBJECT( bhvm_hf3_vm_op_ar3_dp_cb_hdiv_s ) \
    {aware_t _;sz_t a;sz_t b;sz_t c;sz_t d;}; \
  static inline sz_t bhvm_hf3_vm_op_ar3_dp_cb_hdiv_s_get_arity( const bhvm_hf3_vm_op_ar3_dp_cb_hdiv_s* o ){return 3;} \
  static inline void bhvm_hf3_vm_op_ar3_dp_cb_hdiv_s_set_indices( bhvm_hf3_vm_op_ar3_dp_cb_hdiv_s* o, sz_t* a ){o->a = a[0]; o->b = a[1]; o->c = a[2]; o->d = a[3];} \
  static inline void bhvm_hf3_vm_op_ar3_dp_cb_hdiv_s_get_indices( const bhvm_hf3_vm_op_ar3_dp_cb_hdiv_s* o, sz_t* a ){a[0] = o->a; a[1] = o->b; a[2] = o->c; a[3] = o->d;} \
  static inline bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar3_dp_cb_hdiv_s_csetup( bhvm_hf3_vm_op_ar3_dp_cb_hdiv_s* o, sz_t idx_a, sz_t idx_b, sz_t idx_c, sz_t idx_d ){if( !o ) o = bhvm_hf3_vm_op_ar3_dp_cb_hdiv_s_create(); o->a = idx_a; o->b = idx_b; o->c = idx_c; o->d = idx_d; return (bhvm_hf3_vm_op*)o;} \
  static inline sc_t bhvm_hf3_vm_op_ar3_dp_cb_hdiv_s_sig( const bhvm_hf3_vm_op_ar3_dp_cb_hdiv_s* o ){return bhvm_hf3_op_ar3_dp_cb_hdiv_s_sig();} \
  static inline void bhvm_hf3_vm_op_ar3_dp_cb_hdiv_s_run( const bhvm_hf3_vm_op_ar3_dp_cb_hdiv_s* o, bhvm_hf3_vm_holor_s* ah ){bhvm_hf3_op_ar3_dp_cb_hdiv_s_f( &ah[o->a].h, &ah[o->b].h, &ah[o->c].h, &ah[o->d].h );} \
  void bhvm_hf3_vm_op_ar3_dp_cb_hdiv_s_set_arg( bhvm_hf3_vm_op_ar3_dp_cb_hdiv_s* o, char id, sz_t idx );
#define BETH_EXPAND_GROUP_bhvm_hf3_vm_op_ar3_dp \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar3_dp ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar3_dp_ca_hdiv_s ); \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_op_ar3_dp_cb_hdiv_s ); \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar3_dp_ca_hdiv_s \
  BETH_EXPAND_ITEM_bhvm_hf3_vm_op_ar3_dp_cb_hdiv_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_vm_pi

#define TYPEOF_bhvm_hf3_vm_pi 297524904
#define TYPEOF_bhvm_hf3_vm_pi_s 121495378
#define BETH_EXPAND_GROUP_bhvm_hf3_vm_pi \
  BCORE_FORWARD_OBJECT( bhvm_hf3_vm_pi ); \
  typedef tp_t (*bhvm_hf3_vm_pi_get_class)( const bhvm_hf3_vm_pi* o ); \
  BCORE_DECLARE_SPECT( bhvm_hf3_vm_pi ) \
  { \
      bcore_spect_header_s header; \
      bhvm_hf3_vm_pi_get_class get_class; \
  }; \
  static inline bhvm_hf3_vm_pi* bhvm_hf3_vm_pi_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_bhvm_hf3_vm_pi, t ); return ( bhvm_hf3_vm_pi* )bcore_inst_t_create( t ); } \
  static inline bl_t bhvm_hf3_vm_pi_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_bhvm_hf3_vm_pi ); } \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bhvm_hf3_vm_pi ) \
  static inline bl_t bhvm_hf3_vm_pi_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_bhvm_hf3_vm_pi ); } \
  static inline tp_t bhvm_hf3_vm_pi_a_get_class( const bhvm_hf3_vm_pi* o ) { const bhvm_hf3_vm_pi_s* p = bhvm_hf3_vm_pi_s_get_aware( o ); assert( p->get_class ); return p->get_class( o ); } \
  static inline bl_t bhvm_hf3_vm_pi_a_defines_get_class( const bhvm_hf3_vm_pi* o ) { return bhvm_hf3_vm_pi_s_get_aware( o )->get_class != NULL; }

/**********************************************************************************************************************/

vd_t bhvm_planted_hf3_signal_handler( const bcore_signal_s* o );

#endif // BHVM_PLANTED_HF3_H
