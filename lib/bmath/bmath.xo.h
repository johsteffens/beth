/** This file was generated from xoila source code.
 *  Compiling Agent : xoico_compiler (C) 2020 J.B.Steffens
 *  Last File Update: 2020-12-01T10:47:36Z
 *
 *  Copyright and License of this File:
 *
 *  Generated code inherits the copyright and license of the underlying xoila source code.
 *  Source code defining this file is distributed across following files:
 *
 *  bmath_xsmf2.h
 *  bmath_xsmf3.h
 *  bmath_asmf2.h
 *  bmath_asmf3.h
 *
 */

#ifndef __bmath_xo_H
#define __bmath_xo_H

#include "bcore_control.h"

//To force a rebuild of this target by xoico, reset the hash key value below to 0.
#define HKEYOF_bmath_xo 0xFF2D893F443B3FCAull

#define TYPEOF_bmath_xo 0x0E94668A374127B5ull

/**********************************************************************************************************************/
// source: bmath_xsmf2.h

//----------------------------------------------------------------------------------------------------------------------
// group: bmath_xsmf2

#define TYPEOF_bmath_xsmf2 0xA00F532AC396832Cull
#define TYPEOF_bmath_xsmf2_spect_s 0x9F4826A82E6D8F64ull
#define TYPEOF_bmath_xsmf2_s 0x62414774D08F802Eull
#define BETH_EXPAND_ITEM_bmath_xsmf2_s \
  BCORE_DECLARE_OBJECT( bmath_xsmf2_s ) \
  { \
      aware_t _; \
      sz_t slos; \
      sz_t xons; \
      sz_t rows; \
      sz_t i_stride; \
      BCORE_ARRAY_DYN_SOLID_STATIC_S( sz_t, i_ ); \
      BCORE_ARRAY_DYN_SOLID_STATIC_S( f2_t, v_ ); \
  }; \
  void bmath_xsmf2_s_copy_typed( bmath_xsmf2_s* o, tp_t type, vc_t src );
#define TYPEOF_bmath_fp_xsmf2_s_mul 0xB3D376E00071693Eull
#define TYPEOF_bmath_fp_xsmf2_s_mul_htp 0x909F9D1C7AD3244Dull
#define TYPEOF_bmath_fp_xsmf2_s_htp_mul 0x49827253723FB605ull
#define TYPEOF_bmath_fp_xsmf2_s_htp_mul_htp 0xF84FD26B7E9BEC66ull
#define BETH_EXPAND_GROUP_bmath_xsmf2 \
  BCORE_FORWARD_OBJECT( bmath_xsmf2 ); \
  BCORE_FORWARD_OBJECT( bmath_xsmf2_s ); \
  XOILA_DECLARE_SPECT( bmath_xsmf2 ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bmath_xsmf2 ) \
  BETH_EXPAND_ITEM_bmath_xsmf2_s

/**********************************************************************************************************************/
// source: bmath_xsmf3.h

//----------------------------------------------------------------------------------------------------------------------
// group: bmath_xsmf3

#define TYPEOF_bmath_xsmf3 0xA00F542AC39684DFull
#define TYPEOF_bmath_xsmf3_spect_s 0x68FE1BDD8FB1758Full
#define TYPEOF_bmath_xsmf3_s 0x68B76074D3997BE9ull
#define BETH_EXPAND_ITEM_bmath_xsmf3_s \
  BCORE_DECLARE_OBJECT( bmath_xsmf3_s ) \
  { \
      aware_t _; \
      sz_t slos; \
      sz_t xons; \
      sz_t rows; \
      sz_t i_stride; \
      BCORE_ARRAY_DYN_SOLID_STATIC_S( sz_t, i_ ); \
      BCORE_ARRAY_DYN_SOLID_STATIC_S( f3_t, v_ ); \
  }; \
  void bmath_xsmf3_s_copy_typed( bmath_xsmf3_s* o, tp_t type, vc_t src );
#define TYPEOF_bmath_fp_xsmf3_s_mul 0x4F5BCB7DEDE2605Dull
#define TYPEOF_bmath_fp_xsmf3_s_mul_htp 0x58882132D84156DEull
#define TYPEOF_bmath_fp_xsmf3_s_htp_mul 0x86E9F12E5704B076ull
#define TYPEOF_bmath_fp_xsmf3_s_htp_mul_htp 0xE44BD7956EC2F9E5ull
#define BETH_EXPAND_GROUP_bmath_xsmf3 \
  BCORE_FORWARD_OBJECT( bmath_xsmf3 ); \
  BCORE_FORWARD_OBJECT( bmath_xsmf3_s ); \
  XOILA_DECLARE_SPECT( bmath_xsmf3 ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bmath_xsmf3 ) \
  BETH_EXPAND_ITEM_bmath_xsmf3_s

/**********************************************************************************************************************/
// source: bmath_asmf2.h

//----------------------------------------------------------------------------------------------------------------------
// group: bmath_asmf2

#define TYPEOF_bmath_asmf2 0x93DA60428A7774D3ull
#define TYPEOF_bmath_asmf2_spect_s 0x24DCFCEE9C514F33ull
#define TYPEOF_bmath_asmf2_s 0x07ECFB5F02914B95ull
#define BETH_EXPAND_ITEM_bmath_asmf2_s \
  BCORE_DECLARE_OBJECT( bmath_asmf2_s ) \
  { \
      aware_t _; \
      sz_t cols; \
      sz_t rows; \
      sz_t i_stride; \
      BCORE_ARRAY_DYN_SOLID_STATIC_S( sz_t, i_ ); \
      BCORE_ARRAY_DYN_SOLID_STATIC_S( f2_t, v_ ); \
  }; \
  void bmath_asmf2_s_copy_typed( bmath_asmf2_s* o, tp_t type, vc_t src );
#define TYPEOF_bmath_fp_asmf2_s_mul 0x944CFC8975D09AB9ull
#define TYPEOF_bmath_fp_asmf2_s_mul_htp 0x25D843C9CDFE7E5Aull
#define TYPEOF_bmath_fp_asmf2_s_htp_mul 0xD41DD6EF44C270E2ull
#define TYPEOF_bmath_fp_asmf2_s_htp_mul_htp 0x8F69F40E5670D601ull
#define BETH_EXPAND_GROUP_bmath_asmf2 \
  BCORE_FORWARD_OBJECT( bmath_asmf2 ); \
  BCORE_FORWARD_OBJECT( bmath_asmf2_s ); \
  XOILA_DECLARE_SPECT( bmath_asmf2 ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bmath_asmf2 ) \
  BETH_EXPAND_ITEM_bmath_asmf2_s

/**********************************************************************************************************************/
// source: bmath_asmf3.h

//----------------------------------------------------------------------------------------------------------------------
// group: bmath_asmf3

#define TYPEOF_bmath_asmf3 0x93DA5F428A777320ull
#define TYPEOF_bmath_asmf3_spect_s 0x66A9349FCC3FC688ull
#define TYPEOF_bmath_asmf3_s 0x0177E25EFF8902DAull
#define BETH_EXPAND_ITEM_bmath_asmf3_s \
  BCORE_DECLARE_OBJECT( bmath_asmf3_s ) \
  { \
      aware_t _; \
      sz_t cols; \
      sz_t rows; \
      sz_t i_stride; \
      BCORE_ARRAY_DYN_SOLID_STATIC_S( sz_t, i_ ); \
      BCORE_ARRAY_DYN_SOLID_STATIC_S( f3_t, v_ ); \
  }; \
  void bmath_asmf3_s_copy_typed( bmath_asmf3_s* o, tp_t type, vc_t src );
#define TYPEOF_bmath_fp_asmf3_s_mul 0x60BEB16512728D1Aull
#define TYPEOF_bmath_fp_asmf3_s_mul_htp 0xAE7BE7207513F3B9ull
#define TYPEOF_bmath_fp_asmf3_s_htp_mul 0x1F9A3194CFAFF931ull
#define TYPEOF_bmath_fp_asmf3_s_htp_mul_htp 0x7EDE0E76766D9852ull
#define BETH_EXPAND_GROUP_bmath_asmf3 \
  BCORE_FORWARD_OBJECT( bmath_asmf3 ); \
  BCORE_FORWARD_OBJECT( bmath_asmf3_s ); \
  XOILA_DECLARE_SPECT( bmath_asmf3 ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bmath_asmf3 ) \
  BETH_EXPAND_ITEM_bmath_asmf3_s

/**********************************************************************************************************************/

vd_t bmath_xo_signal_handler( const bcore_signal_s* o );

#endif // __bmath_xo_H
// XOILA_OUT_SIGNATURE 0xF49C6421609C01FAull
