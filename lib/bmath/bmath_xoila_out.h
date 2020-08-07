/** This file was generated from beth-plant source code.
 *  Compiling Agent : xoico_compiler (C) 2020 J.B.Steffens
 *  Last File Update: 2020-08-07T21:23:24Z
 *
 *  Copyright and License of this File:
 *
 *  Generated code inherits the copyright and license of the underlying beth-plant source code.
 *  Source code defining this file is distributed across following files:
 *
 *  bmath_f3_op.h
 *  bmath_xsmf2.h
 *  bmath_xsmf3.h
 *  bmath_asmf2.h
 *  bmath_asmf3.h
 *
 */

#ifndef BMATH_XOILA_OUT_H
#define BMATH_XOILA_OUT_H

#include "bcore_control.h"

//To force a rebuild of this target by the plant-compiler, reset the hash key value below to 0.
#define HKEYOF_bmath_xoila_out 14182881912872399678

#define TYPEOF_bmath_xoila_out 0x858ABEC256056040ull

/**********************************************************************************************************************/
// source: bmath_f3_op.h

//----------------------------------------------------------------------------------------------------------------------
// group: bmath_f3_op

#define TYPEOF_bmath_f3_op 0x8E591E3E0E2BE423ull
#define TYPEOF_bmath_f3_op_s 0x391075D8F999AC25ull
#define BETH_EXPAND_GROUP_bmath_f3_op \
  BCORE_FORWARD_OBJECT( bmath_f3_op ); \
  BCORE_FORWARD_OBJECT( bmath_f3_op_ar0 ); \
  BCORE_FORWARD_OBJECT( bmath_f3_op_ar1 ); \
  BCORE_FORWARD_OBJECT( bmath_f3_op_ar2 ); \
  typedef sz_t (*bmath_f3_op_get_arity)( const bmath_f3_op* o ); \
  typedef f3_t (*bmath_f3_op_aofx)( const bmath_f3_op* o, const f3_t* a ); \
  typedef f3_t (*bmath_f3_op_aogx)( const bmath_f3_op* o, const f3_t* a, sz_t ch ); \
  typedef f3_t (*bmath_f3_op_aogy)( const bmath_f3_op* o, const f3_t* a, sz_t ch ); \
  BCORE_DECLARE_SPECT( bmath_f3_op ) \
  { \
      bcore_spect_header_s header; \
      bmath_f3_op_get_arity get_arity; \
      bmath_f3_op_aofx aofx; \
      bmath_f3_op_aogx aogx; \
      bmath_f3_op_aogy aogy; \
  }; \
  static inline bmath_f3_op* bmath_f3_op_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_bmath_f3_op, t ); return ( bmath_f3_op* )bcore_inst_t_create( t ); } \
  static inline bl_t bmath_f3_op_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_bmath_f3_op ); } \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bmath_f3_op ) \
  static inline bl_t bmath_f3_op_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_bmath_f3_op ); } \
  static inline sz_t bmath_f3_op_p_get_arity( const bmath_f3_op_s* __p, const bmath_f3_op* o ) { assert( __p->get_arity ); return __p->get_arity( o ); } \
  static inline sz_t bmath_f3_op_a_get_arity( const bmath_f3_op* o ) { const bmath_f3_op_s* p = bmath_f3_op_s_get_aware( o ); assert( p->get_arity ); return p->get_arity( o ); } \
  static inline bl_t bmath_f3_op_p_defines_get_arity( const bmath_f3_op_s* __p ) { return __p->get_arity != NULL; } \
  static inline bl_t bmath_f3_op_a_defines_get_arity( const bmath_f3_op* o ) { return bmath_f3_op_s_get_aware( o )->get_arity != NULL; } \
  static inline f3_t bmath_f3_op_p_aofx( const bmath_f3_op_s* __p, const bmath_f3_op* o, const f3_t* a ) { assert( __p->aofx ); return __p->aofx( o, a ); } \
  static inline f3_t bmath_f3_op_a_aofx( const bmath_f3_op* o, const f3_t* a ) { const bmath_f3_op_s* p = bmath_f3_op_s_get_aware( o ); assert( p->aofx ); return p->aofx( o, a ); } \
  static inline bl_t bmath_f3_op_p_defines_aofx( const bmath_f3_op_s* __p ) { return __p->aofx != NULL; } \
  static inline bl_t bmath_f3_op_a_defines_aofx( const bmath_f3_op* o ) { return bmath_f3_op_s_get_aware( o )->aofx != NULL; } \
  static inline f3_t bmath_f3_op_p_aogx( const bmath_f3_op_s* __p, const bmath_f3_op* o, const f3_t* a, sz_t ch ) { assert( __p->aogx ); return __p->aogx( o, a, ch ); } \
  static inline f3_t bmath_f3_op_a_aogx( const bmath_f3_op* o, const f3_t* a, sz_t ch ) { const bmath_f3_op_s* p = bmath_f3_op_s_get_aware( o ); assert( p->aogx ); return p->aogx( o, a, ch ); } \
  static inline bl_t bmath_f3_op_p_defines_aogx( const bmath_f3_op_s* __p ) { return __p->aogx != NULL; } \
  static inline bl_t bmath_f3_op_a_defines_aogx( const bmath_f3_op* o ) { return bmath_f3_op_s_get_aware( o )->aogx != NULL; } \
  static inline f3_t bmath_f3_op_p_aogy( const bmath_f3_op_s* __p, const bmath_f3_op* o, const f3_t* a, sz_t ch ) { assert( __p->aogy ); return __p->aogy( o, a, ch ); } \
  static inline f3_t bmath_f3_op_a_aogy( const bmath_f3_op* o, const f3_t* a, sz_t ch ) { const bmath_f3_op_s* p = bmath_f3_op_s_get_aware( o ); assert( p->aogy ); return p->aogy( o, a, ch ); } \
  static inline bl_t bmath_f3_op_p_defines_aogy( const bmath_f3_op_s* __p ) { return __p->aogy != NULL; } \
  static inline bl_t bmath_f3_op_a_defines_aogy( const bmath_f3_op* o ) { return bmath_f3_op_s_get_aware( o )->aogy != NULL; } \
  BETH_EXPAND_GROUP_bmath_f3_op_ar0 \
  BETH_EXPAND_GROUP_bmath_f3_op_ar1 \
  BETH_EXPAND_GROUP_bmath_f3_op_ar2

//----------------------------------------------------------------------------------------------------------------------
// group: bmath_f3_op_ar0

#define TYPEOF_bmath_f3_op_ar0 0x5F083446D94EDCB5ull
#define TYPEOF_bmath_f3_op_ar0_s 0xE9FC6FA7755965B7ull
#define TYPEOF_bmath_f3_op_ar0_zero_s 0x3C86FCD080D3819Eull
#define BETH_EXPAND_ITEM_bmath_f3_op_ar0_zero_s \
  BCORE_DECLARE_OBJECT( bmath_f3_op_ar0_zero_s ) \
    {aware_t _;}; \
  static inline f3_t bmath_f3_op_ar0_zero_s_f( void ){return  0.0;} \
  static inline sz_t bmath_f3_op_ar0_zero_s_get_arity( const bmath_f3_op_ar0_zero_s* o ){return 0;} \
  static inline f3_t bmath_f3_op_ar0_zero_s_of( const bmath_f3_op_ar0_zero_s* o ){return bmath_f3_op_ar0_zero_s_f();} \
  static inline f3_t bmath_f3_op_ar0_zero_s_aofx( const bmath_f3_op_ar0_zero_s* o, const f3_t* a ){return bmath_f3_op_ar0_zero_s_of( o );}
#define TYPEOF_bmath_f3_op_ar0_one_s 0x5B4AAD8AD24F26A8ull
#define BETH_EXPAND_ITEM_bmath_f3_op_ar0_one_s \
  BCORE_DECLARE_OBJECT( bmath_f3_op_ar0_one_s ) \
    {aware_t _;}; \
  static inline f3_t bmath_f3_op_ar0_one_s_f( void ){return  1.0;} \
  static inline sz_t bmath_f3_op_ar0_one_s_get_arity( const bmath_f3_op_ar0_one_s* o ){return 0;} \
  static inline f3_t bmath_f3_op_ar0_one_s_of( const bmath_f3_op_ar0_one_s* o ){return bmath_f3_op_ar0_one_s_f();} \
  static inline f3_t bmath_f3_op_ar0_one_s_aofx( const bmath_f3_op_ar0_one_s* o, const f3_t* a ){return bmath_f3_op_ar0_one_s_of( o );}
#define TYPEOF_bmath_f3_op_ar0_literal_s 0x59E9F00211C1A3B7ull
#define BETH_EXPAND_ITEM_bmath_f3_op_ar0_literal_s \
  BCORE_DECLARE_OBJECT( bmath_f3_op_ar0_literal_s ) \
    {aware_t _;f3_t v;}; \
  static inline f3_t bmath_f3_op_ar0_literal_s_of( const bmath_f3_op_ar0_literal_s* o ){return o->v;} \
  static inline sz_t bmath_f3_op_ar0_literal_s_get_arity( const bmath_f3_op_ar0_literal_s* o ){return 0;} \
  static inline f3_t bmath_f3_op_ar0_literal_s_f( void ){ERR_fa( "Not available." ); return 0;} \
  static inline f3_t bmath_f3_op_ar0_literal_s_aofx( const bmath_f3_op_ar0_literal_s* o, const f3_t* a ){return bmath_f3_op_ar0_literal_s_of( o );}
#define BETH_EXPAND_GROUP_bmath_f3_op_ar0 \
  BCORE_FORWARD_OBJECT( bmath_f3_op_ar0 ); \
  BCORE_FORWARD_OBJECT( bmath_f3_op_ar0_zero_s ); \
  BCORE_FORWARD_OBJECT( bmath_f3_op_ar0_one_s ); \
  BCORE_FORWARD_OBJECT( bmath_f3_op_ar0_literal_s ); \
  typedef f3_t (*bmath_f3_op_ar0_of)( const bmath_f3_op_ar0* o ); \
  BCORE_DECLARE_SPECT( bmath_f3_op_ar0 ) \
  { \
      bcore_spect_header_s header; \
      bmath_f3_op_ar0_of of; \
  }; \
  static inline bmath_f3_op_ar0* bmath_f3_op_ar0_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_bmath_f3_op_ar0, t ); return ( bmath_f3_op_ar0* )bcore_inst_t_create( t ); } \
  static inline bl_t bmath_f3_op_ar0_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_bmath_f3_op_ar0 ); } \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bmath_f3_op_ar0 ) \
  static inline bl_t bmath_f3_op_ar0_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_bmath_f3_op_ar0 ); } \
  static inline f3_t bmath_f3_op_ar0_p_of( const bmath_f3_op_ar0_s* __p, const bmath_f3_op_ar0* o ) { assert( __p->of ); return __p->of( o ); } \
  static inline f3_t bmath_f3_op_ar0_a_of( const bmath_f3_op_ar0* o ) { const bmath_f3_op_ar0_s* p = bmath_f3_op_ar0_s_get_aware( o ); assert( p->of ); return p->of( o ); } \
  static inline bl_t bmath_f3_op_ar0_p_defines_of( const bmath_f3_op_ar0_s* __p ) { return __p->of != NULL; } \
  static inline bl_t bmath_f3_op_ar0_a_defines_of( const bmath_f3_op_ar0* o ) { return bmath_f3_op_ar0_s_get_aware( o )->of != NULL; } \
  BETH_EXPAND_ITEM_bmath_f3_op_ar0_zero_s \
  BETH_EXPAND_ITEM_bmath_f3_op_ar0_one_s \
  BETH_EXPAND_ITEM_bmath_f3_op_ar0_literal_s

//----------------------------------------------------------------------------------------------------------------------
// group: bmath_f3_op_ar1

#define TYPEOF_bmath_f3_op_ar1 0x5F083346D94EDB02ull
#define TYPEOF_bmath_f3_op_ar1_s 0xE1D436A770DF521Cull
#define TYPEOF_bmath_f3_op_ar1_zero_s 0x70C359706D39B263ull
#define BETH_EXPAND_ITEM_bmath_f3_op_ar1_zero_s \
  BCORE_DECLARE_OBJECT( bmath_f3_op_ar1_zero_s ) \
    {aware_t _;}; \
  static inline f3_t bmath_f3_op_ar1_zero_s_fx( f3_t a ){return       0.0;} \
  static inline f3_t bmath_f3_op_ar1_zero_s_gy( f3_t y ){return  0.0;} \
  static inline sz_t bmath_f3_op_ar1_zero_s_get_arity( const bmath_f3_op_ar1_zero_s* o ){return 1;} \
  static inline f3_t bmath_f3_op_ar1_zero_s_ofx( const bmath_f3_op_ar1_zero_s* o, f3_t a ){return bmath_f3_op_ar1_zero_s_fx( a );} \
  static inline f3_t bmath_f3_op_ar1_zero_s_ogy( const bmath_f3_op_ar1_zero_s* o, f3_t y ){return bmath_f3_op_ar1_zero_s_gy( y );} \
  static inline f3_t bmath_f3_op_ar1_zero_s_aofx( const bmath_f3_op_ar1_zero_s* o, const f3_t* a ){return bmath_f3_op_ar1_zero_s_ofx( o, a[0] );} \
  static inline f3_t bmath_f3_op_ar1_zero_s_aogy( const bmath_f3_op_ar1_zero_s* o, const f3_t* a, sz_t ch ){return bmath_f3_op_ar1_zero_s_ogy( o, a[0] );}
#define TYPEOF_bmath_f3_op_ar1_one_s 0xE035C76947A64CA7ull
#define BETH_EXPAND_ITEM_bmath_f3_op_ar1_one_s \
  BCORE_DECLARE_OBJECT( bmath_f3_op_ar1_one_s ) \
    {aware_t _;}; \
  static inline f3_t bmath_f3_op_ar1_one_s_fx( f3_t a ){return       1.0;} \
  static inline f3_t bmath_f3_op_ar1_one_s_gy( f3_t y ){return  0.0;} \
  static inline sz_t bmath_f3_op_ar1_one_s_get_arity( const bmath_f3_op_ar1_one_s* o ){return 1;} \
  static inline f3_t bmath_f3_op_ar1_one_s_ofx( const bmath_f3_op_ar1_one_s* o, f3_t a ){return bmath_f3_op_ar1_one_s_fx( a );} \
  static inline f3_t bmath_f3_op_ar1_one_s_ogy( const bmath_f3_op_ar1_one_s* o, f3_t y ){return bmath_f3_op_ar1_one_s_gy( y );} \
  static inline f3_t bmath_f3_op_ar1_one_s_aofx( const bmath_f3_op_ar1_one_s* o, const f3_t* a ){return bmath_f3_op_ar1_one_s_ofx( o, a[0] );} \
  static inline f3_t bmath_f3_op_ar1_one_s_aogy( const bmath_f3_op_ar1_one_s* o, const f3_t* a, sz_t ch ){return bmath_f3_op_ar1_one_s_ogy( o, a[0] );}
#define TYPEOF_bmath_f3_op_ar1_identity_s 0xCA7A5E785F893D33ull
#define BETH_EXPAND_ITEM_bmath_f3_op_ar1_identity_s \
  BCORE_DECLARE_OBJECT( bmath_f3_op_ar1_identity_s ) \
    {aware_t _;}; \
  static inline f3_t bmath_f3_op_ar1_identity_s_fx( f3_t a ){return        a ;} \
  static inline f3_t bmath_f3_op_ar1_identity_s_gy( f3_t y ){return  1.0;} \
  static inline sz_t bmath_f3_op_ar1_identity_s_get_arity( const bmath_f3_op_ar1_identity_s* o ){return 1;} \
  static inline f3_t bmath_f3_op_ar1_identity_s_ofx( const bmath_f3_op_ar1_identity_s* o, f3_t a ){return bmath_f3_op_ar1_identity_s_fx( a );} \
  static inline f3_t bmath_f3_op_ar1_identity_s_ogy( const bmath_f3_op_ar1_identity_s* o, f3_t y ){return bmath_f3_op_ar1_identity_s_gy( y );} \
  static inline f3_t bmath_f3_op_ar1_identity_s_aofx( const bmath_f3_op_ar1_identity_s* o, const f3_t* a ){return bmath_f3_op_ar1_identity_s_ofx( o, a[0] );} \
  static inline f3_t bmath_f3_op_ar1_identity_s_aogy( const bmath_f3_op_ar1_identity_s* o, const f3_t* a, sz_t ch ){return bmath_f3_op_ar1_identity_s_ogy( o, a[0] );}
#define TYPEOF_bmath_f3_op_ar1_neg_s 0xD1949891137A52B1ull
#define BETH_EXPAND_ITEM_bmath_f3_op_ar1_neg_s \
  BCORE_DECLARE_OBJECT( bmath_f3_op_ar1_neg_s ) \
    {aware_t _;}; \
  static inline f3_t bmath_f3_op_ar1_neg_s_fx( f3_t a ){return       -a ;} \
  static inline f3_t bmath_f3_op_ar1_neg_s_gy( f3_t y ){return -1.0;} \
  static inline sz_t bmath_f3_op_ar1_neg_s_get_arity( const bmath_f3_op_ar1_neg_s* o ){return 1;} \
  static inline f3_t bmath_f3_op_ar1_neg_s_ofx( const bmath_f3_op_ar1_neg_s* o, f3_t a ){return bmath_f3_op_ar1_neg_s_fx( a );} \
  static inline f3_t bmath_f3_op_ar1_neg_s_ogy( const bmath_f3_op_ar1_neg_s* o, f3_t y ){return bmath_f3_op_ar1_neg_s_gy( y );} \
  static inline f3_t bmath_f3_op_ar1_neg_s_aofx( const bmath_f3_op_ar1_neg_s* o, const f3_t* a ){return bmath_f3_op_ar1_neg_s_ofx( o, a[0] );} \
  static inline f3_t bmath_f3_op_ar1_neg_s_aogy( const bmath_f3_op_ar1_neg_s* o, const f3_t* a, sz_t ch ){return bmath_f3_op_ar1_neg_s_ogy( o, a[0] );}
#define TYPEOF_bmath_f3_op_ar1_floor_s 0xC170BF61C8D69D6Dull
#define BETH_EXPAND_ITEM_bmath_f3_op_ar1_floor_s \
  BCORE_DECLARE_OBJECT( bmath_f3_op_ar1_floor_s ) \
    {aware_t _;}; \
  static inline f3_t bmath_f3_op_ar1_floor_s_fx( f3_t a ){return  floor(a);} \
  static inline f3_t bmath_f3_op_ar1_floor_s_gy( f3_t y ){return  0.0;} \
  static inline sz_t bmath_f3_op_ar1_floor_s_get_arity( const bmath_f3_op_ar1_floor_s* o ){return 1;} \
  static inline f3_t bmath_f3_op_ar1_floor_s_ofx( const bmath_f3_op_ar1_floor_s* o, f3_t a ){return bmath_f3_op_ar1_floor_s_fx( a );} \
  static inline f3_t bmath_f3_op_ar1_floor_s_ogy( const bmath_f3_op_ar1_floor_s* o, f3_t y ){return bmath_f3_op_ar1_floor_s_gy( y );} \
  static inline f3_t bmath_f3_op_ar1_floor_s_aofx( const bmath_f3_op_ar1_floor_s* o, const f3_t* a ){return bmath_f3_op_ar1_floor_s_ofx( o, a[0] );} \
  static inline f3_t bmath_f3_op_ar1_floor_s_aogy( const bmath_f3_op_ar1_floor_s* o, const f3_t* a, sz_t ch ){return bmath_f3_op_ar1_floor_s_ogy( o, a[0] );}
#define TYPEOF_bmath_f3_op_ar1_ceil_s 0x3902852373265340ull
#define BETH_EXPAND_ITEM_bmath_f3_op_ar1_ceil_s \
  BCORE_DECLARE_OBJECT( bmath_f3_op_ar1_ceil_s ) \
    {aware_t _;}; \
  static inline f3_t bmath_f3_op_ar1_ceil_s_fx( f3_t a ){return   ceil(a);} \
  static inline f3_t bmath_f3_op_ar1_ceil_s_gy( f3_t y ){return  0.0;} \
  static inline sz_t bmath_f3_op_ar1_ceil_s_get_arity( const bmath_f3_op_ar1_ceil_s* o ){return 1;} \
  static inline f3_t bmath_f3_op_ar1_ceil_s_ofx( const bmath_f3_op_ar1_ceil_s* o, f3_t a ){return bmath_f3_op_ar1_ceil_s_fx( a );} \
  static inline f3_t bmath_f3_op_ar1_ceil_s_ogy( const bmath_f3_op_ar1_ceil_s* o, f3_t y ){return bmath_f3_op_ar1_ceil_s_gy( y );} \
  static inline f3_t bmath_f3_op_ar1_ceil_s_aofx( const bmath_f3_op_ar1_ceil_s* o, const f3_t* a ){return bmath_f3_op_ar1_ceil_s_ofx( o, a[0] );} \
  static inline f3_t bmath_f3_op_ar1_ceil_s_aogy( const bmath_f3_op_ar1_ceil_s* o, const f3_t* a, sz_t ch ){return bmath_f3_op_ar1_ceil_s_ogy( o, a[0] );}
#define TYPEOF_bmath_f3_op_ar1_exp_s 0x460A4A597914DB80ull
#define BETH_EXPAND_ITEM_bmath_f3_op_ar1_exp_s \
  BCORE_DECLARE_OBJECT( bmath_f3_op_ar1_exp_s ) \
    {aware_t _;}; \
  static inline f3_t bmath_f3_op_ar1_exp_s_fx( f3_t a ){return    exp(a);} \
  static inline f3_t bmath_f3_op_ar1_exp_s_gy( f3_t y ){return    y;} \
  static inline sz_t bmath_f3_op_ar1_exp_s_get_arity( const bmath_f3_op_ar1_exp_s* o ){return 1;} \
  static inline f3_t bmath_f3_op_ar1_exp_s_ofx( const bmath_f3_op_ar1_exp_s* o, f3_t a ){return bmath_f3_op_ar1_exp_s_fx( a );} \
  static inline f3_t bmath_f3_op_ar1_exp_s_ogy( const bmath_f3_op_ar1_exp_s* o, f3_t y ){return bmath_f3_op_ar1_exp_s_gy( y );} \
  static inline f3_t bmath_f3_op_ar1_exp_s_aofx( const bmath_f3_op_ar1_exp_s* o, const f3_t* a ){return bmath_f3_op_ar1_exp_s_ofx( o, a[0] );} \
  static inline f3_t bmath_f3_op_ar1_exp_s_aogy( const bmath_f3_op_ar1_exp_s* o, const f3_t* a, sz_t ch ){return bmath_f3_op_ar1_exp_s_ogy( o, a[0] );}
#define TYPEOF_bmath_f3_op_ar1_inv_s 0xD2680B1AB5C249F0ull
#define BETH_EXPAND_ITEM_bmath_f3_op_ar1_inv_s \
  BCORE_DECLARE_OBJECT( bmath_f3_op_ar1_inv_s ) \
    {aware_t _;}; \
  static inline f3_t bmath_f3_op_ar1_inv_s_fx( f3_t a ){return f3_inv(a);} \
  static inline f3_t bmath_f3_op_ar1_inv_s_gy( f3_t y ){return -y*y;} \
  static inline sz_t bmath_f3_op_ar1_inv_s_get_arity( const bmath_f3_op_ar1_inv_s* o ){return 1;} \
  static inline f3_t bmath_f3_op_ar1_inv_s_ofx( const bmath_f3_op_ar1_inv_s* o, f3_t a ){return bmath_f3_op_ar1_inv_s_fx( a );} \
  static inline f3_t bmath_f3_op_ar1_inv_s_ogy( const bmath_f3_op_ar1_inv_s* o, f3_t y ){return bmath_f3_op_ar1_inv_s_gy( y );} \
  static inline f3_t bmath_f3_op_ar1_inv_s_aofx( const bmath_f3_op_ar1_inv_s* o, const f3_t* a ){return bmath_f3_op_ar1_inv_s_ofx( o, a[0] );} \
  static inline f3_t bmath_f3_op_ar1_inv_s_aogy( const bmath_f3_op_ar1_inv_s* o, const f3_t* a, sz_t ch ){return bmath_f3_op_ar1_inv_s_ogy( o, a[0] );}
#define TYPEOF_bmath_f3_op_ar1_sigm_s 0x2DE49095AA707FF9ull
#define BETH_EXPAND_ITEM_bmath_f3_op_ar1_sigm_s \
  BCORE_DECLARE_OBJECT( bmath_f3_op_ar1_sigm_s ) \
    {aware_t _;}; \
  static inline f3_t bmath_f3_op_ar1_sigm_s_fx( f3_t a ){return ( a > -700 ) ? ( 1.0 / ( 1.0 + exp( -a ) ) ) : 0;} \
  static inline f3_t bmath_f3_op_ar1_sigm_s_gy( f3_t y ){return y * ( 1.0 - y );} \
  static inline sz_t bmath_f3_op_ar1_sigm_s_get_arity( const bmath_f3_op_ar1_sigm_s* o ){return 1;} \
  static inline f3_t bmath_f3_op_ar1_sigm_s_ofx( const bmath_f3_op_ar1_sigm_s* o, f3_t a ){return bmath_f3_op_ar1_sigm_s_fx( a );} \
  static inline f3_t bmath_f3_op_ar1_sigm_s_ogy( const bmath_f3_op_ar1_sigm_s* o, f3_t y ){return bmath_f3_op_ar1_sigm_s_gy( y );} \
  static inline f3_t bmath_f3_op_ar1_sigm_s_aofx( const bmath_f3_op_ar1_sigm_s* o, const f3_t* a ){return bmath_f3_op_ar1_sigm_s_ofx( o, a[0] );} \
  static inline f3_t bmath_f3_op_ar1_sigm_s_aogy( const bmath_f3_op_ar1_sigm_s* o, const f3_t* a, sz_t ch ){return bmath_f3_op_ar1_sigm_s_ogy( o, a[0] );}
#define TYPEOF_bmath_f3_op_ar1_sigm_hard_s 0x4EDA78C032743D9Full
#define BETH_EXPAND_ITEM_bmath_f3_op_ar1_sigm_hard_s \
  BCORE_DECLARE_OBJECT( bmath_f3_op_ar1_sigm_hard_s ) \
    {aware_t _;}; \
  static inline f3_t bmath_f3_op_ar1_sigm_hard_s_fx( f3_t a ){return ( a < -2.0 ) ? 0.0 : ( a > 2.0 ) ? 1.0 : 0.25 * ( a + 2.0 );} \
  static inline f3_t bmath_f3_op_ar1_sigm_hard_s_gy( f3_t y ){return ( y <  0.0 ) ? 0.0 : ( y > 1.0 ) ? 0.0 : 0.25;} \
  static inline sz_t bmath_f3_op_ar1_sigm_hard_s_get_arity( const bmath_f3_op_ar1_sigm_hard_s* o ){return 1;} \
  static inline f3_t bmath_f3_op_ar1_sigm_hard_s_ofx( const bmath_f3_op_ar1_sigm_hard_s* o, f3_t a ){return bmath_f3_op_ar1_sigm_hard_s_fx( a );} \
  static inline f3_t bmath_f3_op_ar1_sigm_hard_s_ogy( const bmath_f3_op_ar1_sigm_hard_s* o, f3_t y ){return bmath_f3_op_ar1_sigm_hard_s_gy( y );} \
  static inline f3_t bmath_f3_op_ar1_sigm_hard_s_aofx( const bmath_f3_op_ar1_sigm_hard_s* o, const f3_t* a ){return bmath_f3_op_ar1_sigm_hard_s_ofx( o, a[0] );} \
  static inline f3_t bmath_f3_op_ar1_sigm_hard_s_aogy( const bmath_f3_op_ar1_sigm_hard_s* o, const f3_t* a, sz_t ch ){return bmath_f3_op_ar1_sigm_hard_s_ogy( o, a[0] );}
#define TYPEOF_bmath_f3_op_ar1_sigm_leaky_s 0x5AA45AFC241963B4ull
#define BETH_EXPAND_ITEM_bmath_f3_op_ar1_sigm_leaky_s \
  BCORE_DECLARE_OBJECT( bmath_f3_op_ar1_sigm_leaky_s ) \
    {aware_t _;}; \
  static inline f3_t bmath_f3_op_ar1_sigm_leaky_s_fx( f3_t a ){return ( a < -2.0 ) ? 0.01 * ( a + 2.0 ) : ( a > 2.0 ) ? 1.0 + 0.01 * ( a - 2.0 ) : 0.25 * ( a + 2.0 );} \
  static inline f3_t bmath_f3_op_ar1_sigm_leaky_s_gy( f3_t y ){return ( y <  0.0 ) ? 0.01 : ( y > 1.0 ) ? 0.01 : 0.25;} \
  static inline sz_t bmath_f3_op_ar1_sigm_leaky_s_get_arity( const bmath_f3_op_ar1_sigm_leaky_s* o ){return 1;} \
  static inline f3_t bmath_f3_op_ar1_sigm_leaky_s_ofx( const bmath_f3_op_ar1_sigm_leaky_s* o, f3_t a ){return bmath_f3_op_ar1_sigm_leaky_s_fx( a );} \
  static inline f3_t bmath_f3_op_ar1_sigm_leaky_s_ogy( const bmath_f3_op_ar1_sigm_leaky_s* o, f3_t y ){return bmath_f3_op_ar1_sigm_leaky_s_gy( y );} \
  static inline f3_t bmath_f3_op_ar1_sigm_leaky_s_aofx( const bmath_f3_op_ar1_sigm_leaky_s* o, const f3_t* a ){return bmath_f3_op_ar1_sigm_leaky_s_ofx( o, a[0] );} \
  static inline f3_t bmath_f3_op_ar1_sigm_leaky_s_aogy( const bmath_f3_op_ar1_sigm_leaky_s* o, const f3_t* a, sz_t ch ){return bmath_f3_op_ar1_sigm_leaky_s_ogy( o, a[0] );}
#define TYPEOF_bmath_f3_op_ar1_tanh_s 0x1576CEDC5E564100ull
#define BETH_EXPAND_ITEM_bmath_f3_op_ar1_tanh_s \
  BCORE_DECLARE_OBJECT( bmath_f3_op_ar1_tanh_s ) \
    {aware_t _;}; \
  static inline f3_t bmath_f3_op_ar1_tanh_s_fx( f3_t a ){return ( a < 350 ) ? ( 1.0 - ( 2.0 / ( exp( 2.0 * a ) + 1.0 ) ) ) : 1.0;} \
  static inline f3_t bmath_f3_op_ar1_tanh_s_gy( f3_t y ){return 1.0 - f3_sqr( y );} \
  static inline sz_t bmath_f3_op_ar1_tanh_s_get_arity( const bmath_f3_op_ar1_tanh_s* o ){return 1;} \
  static inline f3_t bmath_f3_op_ar1_tanh_s_ofx( const bmath_f3_op_ar1_tanh_s* o, f3_t a ){return bmath_f3_op_ar1_tanh_s_fx( a );} \
  static inline f3_t bmath_f3_op_ar1_tanh_s_ogy( const bmath_f3_op_ar1_tanh_s* o, f3_t y ){return bmath_f3_op_ar1_tanh_s_gy( y );} \
  static inline f3_t bmath_f3_op_ar1_tanh_s_aofx( const bmath_f3_op_ar1_tanh_s* o, const f3_t* a ){return bmath_f3_op_ar1_tanh_s_ofx( o, a[0] );} \
  static inline f3_t bmath_f3_op_ar1_tanh_s_aogy( const bmath_f3_op_ar1_tanh_s* o, const f3_t* a, sz_t ch ){return bmath_f3_op_ar1_tanh_s_ogy( o, a[0] );}
#define TYPEOF_bmath_f3_op_ar1_tanh_hard_s 0xB730EF9FCB1D1B28ull
#define BETH_EXPAND_ITEM_bmath_f3_op_ar1_tanh_hard_s \
  BCORE_DECLARE_OBJECT( bmath_f3_op_ar1_tanh_hard_s ) \
    {aware_t _;}; \
  static inline f3_t bmath_f3_op_ar1_tanh_hard_s_fx( f3_t a ){return ( a < -1.0 ) ? -1.0 : ( a > 1.0 ) ? 1.0 : a;} \
  static inline f3_t bmath_f3_op_ar1_tanh_hard_s_gy( f3_t y ){return ( y < -1.0 ) ?  0.0 : ( y > 1.0 ) ? 0.0 : 1.0;} \
  static inline sz_t bmath_f3_op_ar1_tanh_hard_s_get_arity( const bmath_f3_op_ar1_tanh_hard_s* o ){return 1;} \
  static inline f3_t bmath_f3_op_ar1_tanh_hard_s_ofx( const bmath_f3_op_ar1_tanh_hard_s* o, f3_t a ){return bmath_f3_op_ar1_tanh_hard_s_fx( a );} \
  static inline f3_t bmath_f3_op_ar1_tanh_hard_s_ogy( const bmath_f3_op_ar1_tanh_hard_s* o, f3_t y ){return bmath_f3_op_ar1_tanh_hard_s_gy( y );} \
  static inline f3_t bmath_f3_op_ar1_tanh_hard_s_aofx( const bmath_f3_op_ar1_tanh_hard_s* o, const f3_t* a ){return bmath_f3_op_ar1_tanh_hard_s_ofx( o, a[0] );} \
  static inline f3_t bmath_f3_op_ar1_tanh_hard_s_aogy( const bmath_f3_op_ar1_tanh_hard_s* o, const f3_t* a, sz_t ch ){return bmath_f3_op_ar1_tanh_hard_s_ogy( o, a[0] );}
#define TYPEOF_bmath_f3_op_ar1_tanh_leaky_s 0x633BA6FCF604CA5Dull
#define BETH_EXPAND_ITEM_bmath_f3_op_ar1_tanh_leaky_s \
  BCORE_DECLARE_OBJECT( bmath_f3_op_ar1_tanh_leaky_s ) \
    {aware_t _;}; \
  static inline f3_t bmath_f3_op_ar1_tanh_leaky_s_fx( f3_t a ){return ( a < -1.0 ) ? -1.0 + 0.01 * ( a + 1.0 ) : ( a > 1.0 ) ? 1.0 + 0.01 * ( a - 1.0 ) : a;} \
  static inline f3_t bmath_f3_op_ar1_tanh_leaky_s_gy( f3_t y ){return ( y < -1.0 ) ?  0.01 : ( y > 1.0 ) ? 0.01 : 1.0;} \
  static inline sz_t bmath_f3_op_ar1_tanh_leaky_s_get_arity( const bmath_f3_op_ar1_tanh_leaky_s* o ){return 1;} \
  static inline f3_t bmath_f3_op_ar1_tanh_leaky_s_ofx( const bmath_f3_op_ar1_tanh_leaky_s* o, f3_t a ){return bmath_f3_op_ar1_tanh_leaky_s_fx( a );} \
  static inline f3_t bmath_f3_op_ar1_tanh_leaky_s_ogy( const bmath_f3_op_ar1_tanh_leaky_s* o, f3_t y ){return bmath_f3_op_ar1_tanh_leaky_s_gy( y );} \
  static inline f3_t bmath_f3_op_ar1_tanh_leaky_s_aofx( const bmath_f3_op_ar1_tanh_leaky_s* o, const f3_t* a ){return bmath_f3_op_ar1_tanh_leaky_s_ofx( o, a[0] );} \
  static inline f3_t bmath_f3_op_ar1_tanh_leaky_s_aogy( const bmath_f3_op_ar1_tanh_leaky_s* o, const f3_t* a, sz_t ch ){return bmath_f3_op_ar1_tanh_leaky_s_ogy( o, a[0] );}
#define TYPEOF_bmath_f3_op_ar1_softplus_s 0xC27151808653F0F9ull
#define BETH_EXPAND_ITEM_bmath_f3_op_ar1_softplus_s \
  BCORE_DECLARE_OBJECT( bmath_f3_op_ar1_softplus_s ) \
    {aware_t _;}; \
  static inline f3_t bmath_f3_op_ar1_softplus_s_fx( f3_t a ){return ( a < 700 ) ? log( 1.0 + exp( a ) ) : a;} \
  static inline f3_t bmath_f3_op_ar1_softplus_s_gy( f3_t y ){f3_t u = exp( y ); return ( u - 1.0 ) / u;} \
  static inline sz_t bmath_f3_op_ar1_softplus_s_get_arity( const bmath_f3_op_ar1_softplus_s* o ){return 1;} \
  static inline f3_t bmath_f3_op_ar1_softplus_s_ofx( const bmath_f3_op_ar1_softplus_s* o, f3_t a ){return bmath_f3_op_ar1_softplus_s_fx( a );} \
  static inline f3_t bmath_f3_op_ar1_softplus_s_ogy( const bmath_f3_op_ar1_softplus_s* o, f3_t y ){return bmath_f3_op_ar1_softplus_s_gy( y );} \
  static inline f3_t bmath_f3_op_ar1_softplus_s_aofx( const bmath_f3_op_ar1_softplus_s* o, const f3_t* a ){return bmath_f3_op_ar1_softplus_s_ofx( o, a[0] );} \
  static inline f3_t bmath_f3_op_ar1_softplus_s_aogy( const bmath_f3_op_ar1_softplus_s* o, const f3_t* a, sz_t ch ){return bmath_f3_op_ar1_softplus_s_ogy( o, a[0] );}
#define TYPEOF_bmath_f3_op_ar1_relu_s 0x21A5DDB4ED851D33ull
#define BETH_EXPAND_ITEM_bmath_f3_op_ar1_relu_s \
  BCORE_DECLARE_OBJECT( bmath_f3_op_ar1_relu_s ) \
    {aware_t _;}; \
  static inline f3_t bmath_f3_op_ar1_relu_s_fx( f3_t a ){return a > 0 ? a : 0;} \
  static inline f3_t bmath_f3_op_ar1_relu_s_gy( f3_t y ){return y > 0 ? 1 : 0;} \
  static inline sz_t bmath_f3_op_ar1_relu_s_get_arity( const bmath_f3_op_ar1_relu_s* o ){return 1;} \
  static inline f3_t bmath_f3_op_ar1_relu_s_ofx( const bmath_f3_op_ar1_relu_s* o, f3_t a ){return bmath_f3_op_ar1_relu_s_fx( a );} \
  static inline f3_t bmath_f3_op_ar1_relu_s_ogy( const bmath_f3_op_ar1_relu_s* o, f3_t y ){return bmath_f3_op_ar1_relu_s_gy( y );} \
  static inline f3_t bmath_f3_op_ar1_relu_s_aofx( const bmath_f3_op_ar1_relu_s* o, const f3_t* a ){return bmath_f3_op_ar1_relu_s_ofx( o, a[0] );} \
  static inline f3_t bmath_f3_op_ar1_relu_s_aogy( const bmath_f3_op_ar1_relu_s* o, const f3_t* a, sz_t ch ){return bmath_f3_op_ar1_relu_s_ogy( o, a[0] );}
#define TYPEOF_bmath_f3_op_ar1_relu_leaky_s 0xE8A8A3690613A44Eull
#define BETH_EXPAND_ITEM_bmath_f3_op_ar1_relu_leaky_s \
  BCORE_DECLARE_OBJECT( bmath_f3_op_ar1_relu_leaky_s ) \
    {aware_t _;}; \
  static inline f3_t bmath_f3_op_ar1_relu_leaky_s_fx( f3_t a ){return a > 0 ? a : a * 0.01;} \
  static inline f3_t bmath_f3_op_ar1_relu_leaky_s_gy( f3_t y ){return y > 0 ? 1 : 0.01;} \
  static inline sz_t bmath_f3_op_ar1_relu_leaky_s_get_arity( const bmath_f3_op_ar1_relu_leaky_s* o ){return 1;} \
  static inline f3_t bmath_f3_op_ar1_relu_leaky_s_ofx( const bmath_f3_op_ar1_relu_leaky_s* o, f3_t a ){return bmath_f3_op_ar1_relu_leaky_s_fx( a );} \
  static inline f3_t bmath_f3_op_ar1_relu_leaky_s_ogy( const bmath_f3_op_ar1_relu_leaky_s* o, f3_t y ){return bmath_f3_op_ar1_relu_leaky_s_gy( y );} \
  static inline f3_t bmath_f3_op_ar1_relu_leaky_s_aofx( const bmath_f3_op_ar1_relu_leaky_s* o, const f3_t* a ){return bmath_f3_op_ar1_relu_leaky_s_ofx( o, a[0] );} \
  static inline f3_t bmath_f3_op_ar1_relu_leaky_s_aogy( const bmath_f3_op_ar1_relu_leaky_s* o, const f3_t* a, sz_t ch ){return bmath_f3_op_ar1_relu_leaky_s_ogy( o, a[0] );}
#define BETH_EXPAND_GROUP_bmath_f3_op_ar1 \
  BCORE_FORWARD_OBJECT( bmath_f3_op_ar1 ); \
  BCORE_FORWARD_OBJECT( bmath_f3_op_ar1_zero_s ); \
  BCORE_FORWARD_OBJECT( bmath_f3_op_ar1_one_s ); \
  BCORE_FORWARD_OBJECT( bmath_f3_op_ar1_identity_s ); \
  BCORE_FORWARD_OBJECT( bmath_f3_op_ar1_neg_s ); \
  BCORE_FORWARD_OBJECT( bmath_f3_op_ar1_floor_s ); \
  BCORE_FORWARD_OBJECT( bmath_f3_op_ar1_ceil_s ); \
  BCORE_FORWARD_OBJECT( bmath_f3_op_ar1_exp_s ); \
  BCORE_FORWARD_OBJECT( bmath_f3_op_ar1_inv_s ); \
  BCORE_FORWARD_OBJECT( bmath_f3_op_ar1_sigm_s ); \
  BCORE_FORWARD_OBJECT( bmath_f3_op_ar1_sigm_hard_s ); \
  BCORE_FORWARD_OBJECT( bmath_f3_op_ar1_sigm_leaky_s ); \
  BCORE_FORWARD_OBJECT( bmath_f3_op_ar1_tanh_s ); \
  BCORE_FORWARD_OBJECT( bmath_f3_op_ar1_tanh_hard_s ); \
  BCORE_FORWARD_OBJECT( bmath_f3_op_ar1_tanh_leaky_s ); \
  BCORE_FORWARD_OBJECT( bmath_f3_op_ar1_softplus_s ); \
  BCORE_FORWARD_OBJECT( bmath_f3_op_ar1_relu_s ); \
  BCORE_FORWARD_OBJECT( bmath_f3_op_ar1_relu_leaky_s ); \
  typedef f3_t (*bmath_f3_op_ar1_ofx)( const bmath_f3_op_ar1* o, f3_t a ); \
  typedef f3_t (*bmath_f3_op_ar1_ogy)( const bmath_f3_op_ar1* o, f3_t y ); \
  BCORE_DECLARE_SPECT( bmath_f3_op_ar1 ) \
  { \
      bcore_spect_header_s header; \
      bmath_f3_op_ar1_ofx ofx; \
      bmath_f3_op_ar1_ogy ogy; \
  }; \
  static inline bmath_f3_op_ar1* bmath_f3_op_ar1_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_bmath_f3_op_ar1, t ); return ( bmath_f3_op_ar1* )bcore_inst_t_create( t ); } \
  static inline bl_t bmath_f3_op_ar1_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_bmath_f3_op_ar1 ); } \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bmath_f3_op_ar1 ) \
  static inline bl_t bmath_f3_op_ar1_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_bmath_f3_op_ar1 ); } \
  static inline f3_t bmath_f3_op_ar1_p_ofx( const bmath_f3_op_ar1_s* __p, const bmath_f3_op_ar1* o, f3_t a ) { assert( __p->ofx ); return __p->ofx( o, a ); } \
  static inline f3_t bmath_f3_op_ar1_a_ofx( const bmath_f3_op_ar1* o, f3_t a ) { const bmath_f3_op_ar1_s* p = bmath_f3_op_ar1_s_get_aware( o ); assert( p->ofx ); return p->ofx( o, a ); } \
  static inline bl_t bmath_f3_op_ar1_p_defines_ofx( const bmath_f3_op_ar1_s* __p ) { return __p->ofx != NULL; } \
  static inline bl_t bmath_f3_op_ar1_a_defines_ofx( const bmath_f3_op_ar1* o ) { return bmath_f3_op_ar1_s_get_aware( o )->ofx != NULL; } \
  static inline f3_t bmath_f3_op_ar1_p_ogy( const bmath_f3_op_ar1_s* __p, const bmath_f3_op_ar1* o, f3_t y ) { assert( __p->ogy ); return __p->ogy( o, y ); } \
  static inline f3_t bmath_f3_op_ar1_a_ogy( const bmath_f3_op_ar1* o, f3_t y ) { const bmath_f3_op_ar1_s* p = bmath_f3_op_ar1_s_get_aware( o ); assert( p->ogy ); return p->ogy( o, y ); } \
  static inline bl_t bmath_f3_op_ar1_p_defines_ogy( const bmath_f3_op_ar1_s* __p ) { return __p->ogy != NULL; } \
  static inline bl_t bmath_f3_op_ar1_a_defines_ogy( const bmath_f3_op_ar1* o ) { return bmath_f3_op_ar1_s_get_aware( o )->ogy != NULL; } \
  BETH_EXPAND_ITEM_bmath_f3_op_ar1_zero_s \
  BETH_EXPAND_ITEM_bmath_f3_op_ar1_one_s \
  BETH_EXPAND_ITEM_bmath_f3_op_ar1_identity_s \
  BETH_EXPAND_ITEM_bmath_f3_op_ar1_neg_s \
  BETH_EXPAND_ITEM_bmath_f3_op_ar1_floor_s \
  BETH_EXPAND_ITEM_bmath_f3_op_ar1_ceil_s \
  BETH_EXPAND_ITEM_bmath_f3_op_ar1_exp_s \
  BETH_EXPAND_ITEM_bmath_f3_op_ar1_inv_s \
  BETH_EXPAND_ITEM_bmath_f3_op_ar1_sigm_s \
  BETH_EXPAND_ITEM_bmath_f3_op_ar1_sigm_hard_s \
  BETH_EXPAND_ITEM_bmath_f3_op_ar1_sigm_leaky_s \
  BETH_EXPAND_ITEM_bmath_f3_op_ar1_tanh_s \
  BETH_EXPAND_ITEM_bmath_f3_op_ar1_tanh_hard_s \
  BETH_EXPAND_ITEM_bmath_f3_op_ar1_tanh_leaky_s \
  BETH_EXPAND_ITEM_bmath_f3_op_ar1_softplus_s \
  BETH_EXPAND_ITEM_bmath_f3_op_ar1_relu_s \
  BETH_EXPAND_ITEM_bmath_f3_op_ar1_relu_leaky_s

//----------------------------------------------------------------------------------------------------------------------
// group: bmath_f3_op_ar2

#define TYPEOF_bmath_f3_op_ar2 0x5F083246D94ED94Full
#define TYPEOF_bmath_f3_op_ar2_s 0xD71F05A76A3A13D9ull
#define TYPEOF_bmath_f3_op_ar2_add_s 0xAC8ACBCE55DEB3F5ull
#define BETH_EXPAND_ITEM_bmath_f3_op_ar2_add_s \
  BCORE_DECLARE_OBJECT( bmath_f3_op_ar2_add_s ) \
    {aware_t _;}; \
  static inline f3_t bmath_f3_op_ar2_add_s_fx( f3_t a, f3_t b ){return a + b;} \
  static inline f3_t bmath_f3_op_ar2_add_s_gxa( f3_t a, f3_t b ){return 1;} \
  static inline f3_t bmath_f3_op_ar2_add_s_gxb( f3_t a, f3_t b ){return 1;} \
  static inline f3_t bmath_f3_op_ar2_add_s_ofx( const bmath_f3_op_ar2_add_s* o, f3_t a, f3_t b ){return bmath_f3_op_ar2_add_s_fx(  a, b );} \
  static inline f3_t bmath_f3_op_ar2_add_s_ogxa( const bmath_f3_op_ar2_add_s* o, f3_t a, f3_t b ){return bmath_f3_op_ar2_add_s_gxa( a, b );} \
  static inline f3_t bmath_f3_op_ar2_add_s_ogxb( const bmath_f3_op_ar2_add_s* o, f3_t a, f3_t b ){return bmath_f3_op_ar2_add_s_gxb( a, b );} \
  static inline sz_t bmath_f3_op_ar2_add_s_get_arity( const bmath_f3_op_ar2_add_s* o ){return 2;} \
  static inline f3_t bmath_f3_op_ar2_add_s_aofx( const bmath_f3_op_ar2_add_s* o, const f3_t* a ){return bmath_f3_op_ar2_add_s_ofx( o, a[0], a[1] );} \
  static inline f3_t bmath_f3_op_ar2_add_s_aogx( const bmath_f3_op_ar2_add_s* o, const f3_t* a, sz_t ch ){return ( ch == 0 ) ? bmath_f3_op_ar2_add_s_ogxa( o, a[0], a[1] ) : bmath_f3_op_ar2_add_s_ogxb( o, a[0], a[1] );}
#define TYPEOF_bmath_f3_op_ar2_sub_s 0x67FDB8D7196DA02Cull
#define BETH_EXPAND_ITEM_bmath_f3_op_ar2_sub_s \
  BCORE_DECLARE_OBJECT( bmath_f3_op_ar2_sub_s ) \
    {aware_t _;}; \
  static inline f3_t bmath_f3_op_ar2_sub_s_fx( f3_t a, f3_t b ){return a - b;} \
  static inline f3_t bmath_f3_op_ar2_sub_s_gxa( f3_t a, f3_t b ){return  1;} \
  static inline f3_t bmath_f3_op_ar2_sub_s_gxb( f3_t a, f3_t b ){return -1;} \
  static inline f3_t bmath_f3_op_ar2_sub_s_ofx( const bmath_f3_op_ar2_sub_s* o, f3_t a, f3_t b ){return bmath_f3_op_ar2_sub_s_fx(  a, b );} \
  static inline f3_t bmath_f3_op_ar2_sub_s_ogxa( const bmath_f3_op_ar2_sub_s* o, f3_t a, f3_t b ){return bmath_f3_op_ar2_sub_s_gxa( a, b );} \
  static inline f3_t bmath_f3_op_ar2_sub_s_ogxb( const bmath_f3_op_ar2_sub_s* o, f3_t a, f3_t b ){return bmath_f3_op_ar2_sub_s_gxb( a, b );} \
  static inline sz_t bmath_f3_op_ar2_sub_s_get_arity( const bmath_f3_op_ar2_sub_s* o ){return 2;} \
  static inline f3_t bmath_f3_op_ar2_sub_s_aofx( const bmath_f3_op_ar2_sub_s* o, const f3_t* a ){return bmath_f3_op_ar2_sub_s_ofx( o, a[0], a[1] );} \
  static inline f3_t bmath_f3_op_ar2_sub_s_aogx( const bmath_f3_op_ar2_sub_s* o, const f3_t* a, sz_t ch ){return ( ch == 0 ) ? bmath_f3_op_ar2_sub_s_ogxa( o, a[0], a[1] ) : bmath_f3_op_ar2_sub_s_ogxb( o, a[0], a[1] );}
#define TYPEOF_bmath_f3_op_ar2_mul_s 0xE8B0F61C45DBC790ull
#define BETH_EXPAND_ITEM_bmath_f3_op_ar2_mul_s \
  BCORE_DECLARE_OBJECT( bmath_f3_op_ar2_mul_s ) \
    {aware_t _;}; \
  static inline f3_t bmath_f3_op_ar2_mul_s_fx( f3_t a, f3_t b ){return a * b;} \
  static inline f3_t bmath_f3_op_ar2_mul_s_gxa( f3_t a, f3_t b ){return b;} \
  static inline f3_t bmath_f3_op_ar2_mul_s_gxb( f3_t a, f3_t b ){return a;} \
  static inline f3_t bmath_f3_op_ar2_mul_s_ofx( const bmath_f3_op_ar2_mul_s* o, f3_t a, f3_t b ){return bmath_f3_op_ar2_mul_s_fx(  a, b );} \
  static inline f3_t bmath_f3_op_ar2_mul_s_ogxa( const bmath_f3_op_ar2_mul_s* o, f3_t a, f3_t b ){return bmath_f3_op_ar2_mul_s_gxa( a, b );} \
  static inline f3_t bmath_f3_op_ar2_mul_s_ogxb( const bmath_f3_op_ar2_mul_s* o, f3_t a, f3_t b ){return bmath_f3_op_ar2_mul_s_gxb( a, b );} \
  static inline sz_t bmath_f3_op_ar2_mul_s_get_arity( const bmath_f3_op_ar2_mul_s* o ){return 2;} \
  static inline f3_t bmath_f3_op_ar2_mul_s_aofx( const bmath_f3_op_ar2_mul_s* o, const f3_t* a ){return bmath_f3_op_ar2_mul_s_ofx( o, a[0], a[1] );} \
  static inline f3_t bmath_f3_op_ar2_mul_s_aogx( const bmath_f3_op_ar2_mul_s* o, const f3_t* a, sz_t ch ){return ( ch == 0 ) ? bmath_f3_op_ar2_mul_s_ogxa( o, a[0], a[1] ) : bmath_f3_op_ar2_mul_s_ogxb( o, a[0], a[1] );}
#define TYPEOF_bmath_f3_op_ar2_div_s 0x17232AEFEA00AAEDull
#define BETH_EXPAND_ITEM_bmath_f3_op_ar2_div_s \
  BCORE_DECLARE_OBJECT( bmath_f3_op_ar2_div_s ) \
    {aware_t _;}; \
  static inline f3_t bmath_f3_op_ar2_div_s_fx( f3_t a, f3_t b ){return b != 0 ?   a / b         :  a * f3_lim_inf;} \
  static inline f3_t bmath_f3_op_ar2_div_s_gxa( f3_t a, f3_t b ){return b != 0 ? 1.0 / b         :      f3_lim_inf;} \
  static inline f3_t bmath_f3_op_ar2_div_s_gxb( f3_t a, f3_t b ){return b != 0 ?  -a / ( b * b ) : -a * f3_lim_inf;} \
  static inline f3_t bmath_f3_op_ar2_div_s_ofx( const bmath_f3_op_ar2_div_s* o, f3_t a, f3_t b ){return bmath_f3_op_ar2_div_s_fx(  a, b );} \
  static inline f3_t bmath_f3_op_ar2_div_s_ogxa( const bmath_f3_op_ar2_div_s* o, f3_t a, f3_t b ){return bmath_f3_op_ar2_div_s_gxa( a, b );} \
  static inline f3_t bmath_f3_op_ar2_div_s_ogxb( const bmath_f3_op_ar2_div_s* o, f3_t a, f3_t b ){return bmath_f3_op_ar2_div_s_gxb( a, b );} \
  static inline sz_t bmath_f3_op_ar2_div_s_get_arity( const bmath_f3_op_ar2_div_s* o ){return 2;} \
  static inline f3_t bmath_f3_op_ar2_div_s_aofx( const bmath_f3_op_ar2_div_s* o, const f3_t* a ){return bmath_f3_op_ar2_div_s_ofx( o, a[0], a[1] );} \
  static inline f3_t bmath_f3_op_ar2_div_s_aogx( const bmath_f3_op_ar2_div_s* o, const f3_t* a, sz_t ch ){return ( ch == 0 ) ? bmath_f3_op_ar2_div_s_ogxa( o, a[0], a[1] ) : bmath_f3_op_ar2_div_s_ogxb( o, a[0], a[1] );}
#define BETH_EXPAND_GROUP_bmath_f3_op_ar2 \
  BCORE_FORWARD_OBJECT( bmath_f3_op_ar2 ); \
  BCORE_FORWARD_OBJECT( bmath_f3_op_ar2_add_s ); \
  BCORE_FORWARD_OBJECT( bmath_f3_op_ar2_sub_s ); \
  BCORE_FORWARD_OBJECT( bmath_f3_op_ar2_mul_s ); \
  BCORE_FORWARD_OBJECT( bmath_f3_op_ar2_div_s ); \
  typedef f3_t (*bmath_f3_op_ar2_ofx)( const bmath_f3_op_ar2* o, f3_t a, f3_t b ); \
  typedef f3_t (*bmath_f3_op_ar2_ogxa)( const bmath_f3_op_ar2* o, f3_t a, f3_t b ); \
  typedef f3_t (*bmath_f3_op_ar2_ogxb)( const bmath_f3_op_ar2* o, f3_t a, f3_t b ); \
  BCORE_DECLARE_SPECT( bmath_f3_op_ar2 ) \
  { \
      bcore_spect_header_s header; \
      bmath_f3_op_ar2_ofx ofx; \
      bmath_f3_op_ar2_ogxa ogxa; \
      bmath_f3_op_ar2_ogxb ogxb; \
  }; \
  static inline bmath_f3_op_ar2* bmath_f3_op_ar2_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_bmath_f3_op_ar2, t ); return ( bmath_f3_op_ar2* )bcore_inst_t_create( t ); } \
  static inline bl_t bmath_f3_op_ar2_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_bmath_f3_op_ar2 ); } \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bmath_f3_op_ar2 ) \
  static inline bl_t bmath_f3_op_ar2_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_bmath_f3_op_ar2 ); } \
  static inline f3_t bmath_f3_op_ar2_p_ofx( const bmath_f3_op_ar2_s* __p, const bmath_f3_op_ar2* o, f3_t a, f3_t b ) { assert( __p->ofx ); return __p->ofx( o, a, b ); } \
  static inline f3_t bmath_f3_op_ar2_a_ofx( const bmath_f3_op_ar2* o, f3_t a, f3_t b ) { const bmath_f3_op_ar2_s* p = bmath_f3_op_ar2_s_get_aware( o ); assert( p->ofx ); return p->ofx( o, a, b ); } \
  static inline bl_t bmath_f3_op_ar2_p_defines_ofx( const bmath_f3_op_ar2_s* __p ) { return __p->ofx != NULL; } \
  static inline bl_t bmath_f3_op_ar2_a_defines_ofx( const bmath_f3_op_ar2* o ) { return bmath_f3_op_ar2_s_get_aware( o )->ofx != NULL; } \
  static inline f3_t bmath_f3_op_ar2_p_ogxa( const bmath_f3_op_ar2_s* __p, const bmath_f3_op_ar2* o, f3_t a, f3_t b ) { assert( __p->ogxa ); return __p->ogxa( o, a, b ); } \
  static inline f3_t bmath_f3_op_ar2_a_ogxa( const bmath_f3_op_ar2* o, f3_t a, f3_t b ) { const bmath_f3_op_ar2_s* p = bmath_f3_op_ar2_s_get_aware( o ); assert( p->ogxa ); return p->ogxa( o, a, b ); } \
  static inline bl_t bmath_f3_op_ar2_p_defines_ogxa( const bmath_f3_op_ar2_s* __p ) { return __p->ogxa != NULL; } \
  static inline bl_t bmath_f3_op_ar2_a_defines_ogxa( const bmath_f3_op_ar2* o ) { return bmath_f3_op_ar2_s_get_aware( o )->ogxa != NULL; } \
  static inline f3_t bmath_f3_op_ar2_p_ogxb( const bmath_f3_op_ar2_s* __p, const bmath_f3_op_ar2* o, f3_t a, f3_t b ) { assert( __p->ogxb ); return __p->ogxb( o, a, b ); } \
  static inline f3_t bmath_f3_op_ar2_a_ogxb( const bmath_f3_op_ar2* o, f3_t a, f3_t b ) { const bmath_f3_op_ar2_s* p = bmath_f3_op_ar2_s_get_aware( o ); assert( p->ogxb ); return p->ogxb( o, a, b ); } \
  static inline bl_t bmath_f3_op_ar2_p_defines_ogxb( const bmath_f3_op_ar2_s* __p ) { return __p->ogxb != NULL; } \
  static inline bl_t bmath_f3_op_ar2_a_defines_ogxb( const bmath_f3_op_ar2* o ) { return bmath_f3_op_ar2_s_get_aware( o )->ogxb != NULL; } \
  BETH_EXPAND_ITEM_bmath_f3_op_ar2_add_s \
  BETH_EXPAND_ITEM_bmath_f3_op_ar2_sub_s \
  BETH_EXPAND_ITEM_bmath_f3_op_ar2_mul_s \
  BETH_EXPAND_ITEM_bmath_f3_op_ar2_div_s

/**********************************************************************************************************************/
// source: bmath_xsmf2.h

//----------------------------------------------------------------------------------------------------------------------
// group: bmath_xsmf2

#define TYPEOF_bmath_xsmf2 0xA00F532AC396832Cull
#define TYPEOF_bmath_xsmf2_s 0x62414774D08F802Eull
#define TYPEOF_bmath_xsmf2_s 0x62414774D08F802Eull
#define BETH_EXPAND_ITEM_bmath_xsmf2_s \
  BCORE_DECLARE_OBJECT( bmath_xsmf2_s ) \
    {aware_t _;sz_t slos;sz_t xons;sz_t rows;sz_t i_stride;BCORE_ARRAY_DYN_SOLID_STATIC_S( sz_t, i_ );BCORE_ARRAY_DYN_SOLID_STATIC_S( f2_t, v_ );}; \
  void bmath_xsmf2_s_copy_typed( bmath_xsmf2_s* o, tp_t type, vc_t src );
#define TYPEOF_bmath_fp_xsmf2_s_mul 0xB3D376E00071693Eull
#define TYPEOF_bmath_fp_xsmf2_s_mul_htp 0x909F9D1C7AD3244Dull
#define TYPEOF_bmath_fp_xsmf2_s_htp_mul 0x49827253723FB605ull
#define TYPEOF_bmath_fp_xsmf2_s_htp_mul_htp 0xF84FD26B7E9BEC66ull
#define BETH_EXPAND_GROUP_bmath_xsmf2 \
  BCORE_FORWARD_OBJECT( bmath_xsmf2 ); \
  BCORE_FORWARD_OBJECT( bmath_xsmf2_s ); \
  BETH_EXPAND_ITEM_bmath_xsmf2_s

/**********************************************************************************************************************/
// source: bmath_xsmf3.h

//----------------------------------------------------------------------------------------------------------------------
// group: bmath_xsmf3

#define TYPEOF_bmath_xsmf3 0xA00F542AC39684DFull
#define TYPEOF_bmath_xsmf3_s 0x68B76074D3997BE9ull
#define TYPEOF_bmath_xsmf3_s 0x68B76074D3997BE9ull
#define BETH_EXPAND_ITEM_bmath_xsmf3_s \
  BCORE_DECLARE_OBJECT( bmath_xsmf3_s ) \
    {aware_t _;sz_t slos;sz_t xons;sz_t rows;sz_t i_stride;BCORE_ARRAY_DYN_SOLID_STATIC_S( sz_t, i_ );BCORE_ARRAY_DYN_SOLID_STATIC_S( f3_t, v_ );}; \
  void bmath_xsmf3_s_copy_typed( bmath_xsmf3_s* o, tp_t type, vc_t src );
#define TYPEOF_bmath_fp_xsmf3_s_mul 0x4F5BCB7DEDE2605Dull
#define TYPEOF_bmath_fp_xsmf3_s_mul_htp 0x58882132D84156DEull
#define TYPEOF_bmath_fp_xsmf3_s_htp_mul 0x86E9F12E5704B076ull
#define TYPEOF_bmath_fp_xsmf3_s_htp_mul_htp 0xE44BD7956EC2F9E5ull
#define BETH_EXPAND_GROUP_bmath_xsmf3 \
  BCORE_FORWARD_OBJECT( bmath_xsmf3 ); \
  BCORE_FORWARD_OBJECT( bmath_xsmf3_s ); \
  BETH_EXPAND_ITEM_bmath_xsmf3_s

/**********************************************************************************************************************/
// source: bmath_asmf2.h

//----------------------------------------------------------------------------------------------------------------------
// group: bmath_asmf2

#define TYPEOF_bmath_asmf2 0x93DA60428A7774D3ull
#define TYPEOF_bmath_asmf2_s 0x07ECFB5F02914B95ull
#define TYPEOF_bmath_asmf2_s 0x07ECFB5F02914B95ull
#define BETH_EXPAND_ITEM_bmath_asmf2_s \
  BCORE_DECLARE_OBJECT( bmath_asmf2_s ) \
    {aware_t _;sz_t cols;sz_t rows;sz_t i_stride;BCORE_ARRAY_DYN_SOLID_STATIC_S( sz_t, i_ );BCORE_ARRAY_DYN_SOLID_STATIC_S( f2_t, v_ );}; \
  void bmath_asmf2_s_copy_typed( bmath_asmf2_s* o, tp_t type, vc_t src );
#define TYPEOF_bmath_fp_asmf2_s_mul 0x944CFC8975D09AB9ull
#define TYPEOF_bmath_fp_asmf2_s_mul_htp 0x25D843C9CDFE7E5Aull
#define TYPEOF_bmath_fp_asmf2_s_htp_mul 0xD41DD6EF44C270E2ull
#define TYPEOF_bmath_fp_asmf2_s_htp_mul_htp 0x8F69F40E5670D601ull
#define BETH_EXPAND_GROUP_bmath_asmf2 \
  BCORE_FORWARD_OBJECT( bmath_asmf2 ); \
  BCORE_FORWARD_OBJECT( bmath_asmf2_s ); \
  BETH_EXPAND_ITEM_bmath_asmf2_s

/**********************************************************************************************************************/
// source: bmath_asmf3.h

//----------------------------------------------------------------------------------------------------------------------
// group: bmath_asmf3

#define TYPEOF_bmath_asmf3 0x93DA5F428A777320ull
#define TYPEOF_bmath_asmf3_s 0x0177E25EFF8902DAull
#define TYPEOF_bmath_asmf3_s 0x0177E25EFF8902DAull
#define BETH_EXPAND_ITEM_bmath_asmf3_s \
  BCORE_DECLARE_OBJECT( bmath_asmf3_s ) \
    {aware_t _;sz_t cols;sz_t rows;sz_t i_stride;BCORE_ARRAY_DYN_SOLID_STATIC_S( sz_t, i_ );BCORE_ARRAY_DYN_SOLID_STATIC_S( f3_t, v_ );}; \
  void bmath_asmf3_s_copy_typed( bmath_asmf3_s* o, tp_t type, vc_t src );
#define TYPEOF_bmath_fp_asmf3_s_mul 0x60BEB16512728D1Aull
#define TYPEOF_bmath_fp_asmf3_s_mul_htp 0xAE7BE7207513F3B9ull
#define TYPEOF_bmath_fp_asmf3_s_htp_mul 0x1F9A3194CFAFF931ull
#define TYPEOF_bmath_fp_asmf3_s_htp_mul_htp 0x7EDE0E76766D9852ull
#define BETH_EXPAND_GROUP_bmath_asmf3 \
  BCORE_FORWARD_OBJECT( bmath_asmf3 ); \
  BCORE_FORWARD_OBJECT( bmath_asmf3_s ); \
  BETH_EXPAND_ITEM_bmath_asmf3_s

/**********************************************************************************************************************/

vd_t bmath_xoila_out_signal_handler( const bcore_signal_s* o );

#endif // BMATH_XOILA_OUT_H
// BETH_PLANT_SIGNATURE 386904000325482755
