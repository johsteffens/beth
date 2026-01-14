//  Last update: 2026-01-14T12:38:26Z (UTC)
/** This file was generated from xoila source code.
 *  Compiling Agent : XOICO (C) 2020 ... 2025 J.B.Steffens
 *  Note that any manual changes in this file can be erased or overwritten by XOICO.
 *
 *  Copyright and License of this File:
 *
 *  Unless explicitly stated otherwise in governing license terms, this file inherits the
 *  copyright and license terms of the immediate source code from which it was compiled.
 *
 *  The immediate source code is distributed across following files:
 *  bcore_x_root_inexpandable.h
 *  bcore_x_inst.h
 *  bcore_x_compare.h
 *  bcore_x_source.h
 *  bcore_x_sink.h
 *  bcore_x_array.h
 *  bcore_x_group.h
 *  bcore_x_stamp.h
 *  bcore_x_threads.h
 *  bcore_file.h
 *  bcore_spect_inst_call.h
 *  bcore_spect_via_call.h
 *  bcore_main.h
 *  bcore_hmap_name.h
 *  bcore_hmap_tp_st.h
 *  bcore_cday.h
 *  bcore_error_manager.h
 *  bcore_prsg.h
 *  bcore_shell.h
 *  bcore_huffman.h
 *  bcore_indexer.h
 *  bcore_parse.h
 *  bcore_x_btml.h
 *  bcore_x_bbml.h
 *  bcore_x_bcml.h
 *  bcore_x_btcl.h
 *  bcore_x_hmap.h
 *  bcore_x_deque.h
 *  bcore_arr_inexpandable.x
 *  bcore_const_manager_inexpandable.x
 *  bcore_flect_inexpandable.x
 *  bcore_folder_inexpandable.x
 *  bcore_function_manager_inexpandable.x
 *  bcore_global_manager_inexpandable.x
 *  bcore_hmap_inexpandable.x
 *  bcore_hmap_tp_sr_inexpandable.x
 *  bcore_huffman.x
 *  bcore_img_inexpandable.x
 *  bcore_name_manager_inexpandable.x
 *  bcore_parse.x
 *  bcore_sink_inexpandable.x
 *  bcore_source_inexpandable.x
 *  bcore_sr_inexpandable.x
 *  bcore_st_inexpandable.x
 *  bcore_via_inexpandable.x
 *  bcore_x_btcl.x
 *  bcore_x_btcl_builtin.x
 *  bcore_x_btcl_functor.x
 *  bcore_x_btcl_functor_f3.x
 *  bcore_x_btcl_net.x
 *  bcore_x_btcl_op.x
 *  bcore_x_btcl_operator.x
 *  bcore_x_btcl_operator_f3.x
 *  bcore_x_btcl_random.x
 *  bcore_x_btcl_test.x
 *  bcore_x_hmap.x
 *
 */

#ifndef __bcore_xo_H
#define __bcore_xo_H

#include "bcore_control.h"
#include "bcore_xoila.h"

#define TYPEOF_bcore 0x52C4008F456A6C1Eull

/**********************************************************************************************************************/
// source: bcore_x_root_inexpandable.h

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_x_root_expandable

#define TYPEOF_bcore_x_root_expandable 0xC8EB05E1BB5DDB27ull
#define TYPEOF_bcore_x_root_expandable_spect_s 0x509717BBCE3C3A17ull
#define TYPEOF_bcore_self_item_s 0x1A76AAC8B75A6B47ull
#define BETH_EXPAND_GROUP_bcore_x_root_expandable \
  BCORE_FORWARD_OBJECT( bcore_x_root_expandable ); \
  XOILA_DECLARE_SPECT( bcore_x_root_expandable ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bcore_x_root_expandable )

/**********************************************************************************************************************/
// source: bcore_x_inst.h

//----------------------------------------------------------------------------------------------------------------------
// group: x_inst
#include "bcore_txt_ml.h"
#include "bcore_bin_ml.h"
#include "bcore_arr.h"
#include "bcore_life.h"
#include "bcore_sinks.h"

#define TYPEOF_x_inst 0xFC4EFF7569E22558ull
#define TYPEOF_x_inst_spect_s 0x6B610F4B8E350C20ull
#define BETH_EXPAND_GROUP_x_inst \
  BCORE_FORWARD_OBJECT( x_inst ); \
  static inline bl_t x_inst_is_creatable( tp_t type ); \
  static inline bl_t x_inst_has_reflection( tp_t type ); \
  static inline x_inst* x_inst_t_copy( x_inst* o, tp_t t, const x_inst* src ); \
  static inline x_inst* x_inst_copy( x_inst* o, const x_inst* src ); \
  static inline x_inst* x_inst_fork( x_inst* o ); \
  static inline er_t x_inst_t_copy_typed( x_inst* o, tp_t t, tp_t src_type, const x_inst* src ); \
  static inline er_t x_inst_copy_typed( x_inst* o, tp_t src_type, const x_inst* src ); \
  static inline x_inst* x_inst_t_clone( const x_inst* o, tp_t t ); \
  static inline x_inst* x_inst_clone( const x_inst* o ); \
  static inline void x_inst_t_discard( x_inst* o, tp_t t ); \
  static inline void x_inst_discard( x_inst* o ); \
  static inline x_inst* x_inst_ifd( x_inst* o, bl_t cond, x_inst* b ); \
  static inline const x_inst* x_inst_ifc( const x_inst* o, bl_t cond, const x_inst* b ); \
  x_inst* x_inst_create( tp_t type ); \
  static inline const x_inst* x_inst_attend( const x_inst* o, const x_inst* a ); \
  static inline const x_inst* x_inst_attn( const x_inst* o, const x_inst* a ); \
  XOILA_DECLARE_SPECT( x_inst ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( x_inst ) \
  static inline bl_t x_inst_is_creatable( tp_t type ){return  bcore_flect_exists( type );} \
  static inline bl_t x_inst_has_reflection( tp_t type ){return  bcore_flect_exists( type );} \
  static inline x_inst* x_inst_t_copy( x_inst* o, tp_t t, const x_inst* src ){bcore_inst_t_copy( t, ((bcore_inst*)(o)), src );return o;} \
  static inline x_inst* x_inst_copy( x_inst* o, const x_inst* src ){bcore_inst_a_copy(    ((bcore_inst*)(o)),((const bcore_inst*)(src )));return o;} \
  static inline x_inst* x_inst_fork( x_inst* o ){return  bcore_fork( o );} \
  static inline er_t x_inst_t_copy_typed( x_inst* o, tp_t t, tp_t src_type, const x_inst* src ){return  bcore_inst_t_copy_typed( t,   ((bcore_inst*)(o)), src_type, src );} \
  static inline er_t x_inst_copy_typed( x_inst* o, tp_t src_type, const x_inst* src ){return  bcore_inst_t_copy_typed( o->_, ((bcore_inst*)(o)), src_type, src );} \
  static inline x_inst* x_inst_t_clone( const x_inst* o, tp_t t ){return  bcore_inst_t_clone( t, ((const bcore_inst*)(o)) );} \
  static inline x_inst* x_inst_clone( const x_inst* o ){return ((x_inst*)( bcore_inst_a_clone(    ((const bcore_inst*)(o)) )));} \
  static inline void x_inst_t_discard( x_inst* o, tp_t t ){bcore_inst_t_discard( t, ((bcore_inst*)(o)) );} \
  static inline void x_inst_discard( x_inst* o ){bcore_inst_a_discard(    ((bcore_inst*)(o)) );} \
  static inline x_inst* x_inst_ifd( x_inst* o, bl_t cond, x_inst* b ){return  cond ? o : b;} \
  static inline const x_inst* x_inst_ifc( const x_inst* o, bl_t cond, const x_inst* b ){return  cond ? o : b;} \
  static inline const x_inst* x_inst_attend( const x_inst* o, const x_inst* a ){return  a;} \
  static inline const x_inst* x_inst_attn( const x_inst* o, const x_inst* a ){return  a;}

/**********************************************************************************************************************/
// source: bcore_x_compare.h

//----------------------------------------------------------------------------------------------------------------------
// group: x_compare
#include "bcore_spect_compare.h"

#define TYPEOF_x_compare 0xDF0D8ACE61974067ull
#define TYPEOF_x_compare_spect_s 0xD13B4A3F5F184257ull
#define BETH_EXPAND_GROUP_x_compare \
  BCORE_FORWARD_OBJECT( x_compare ); \
  static inline s2_t x_compare_t_type_dominant( tp_t ta, const x_inst* a, tp_t tb, const x_inst* b ); \
  static inline s2_t x_compare_type_dominant( const x_inst* a, const x_inst* b ); \
  static inline s2_t x_compare_t_num_dominant( tp_t ta, const x_inst* a, tp_t tb, const x_inst* b ); \
  static inline s2_t x_compare_num_dominant( const x_inst* a, const x_inst* b ); \
  XOILA_DECLARE_SPECT( x_compare ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( x_compare ) \
  static inline s2_t x_compare_t_type_dominant( tp_t ta, const x_inst* a, tp_t tb, const x_inst* b ){return  ( ta == tb ) ? bcore_compare_typed( ta, a, b ) : ( ta < tb ) ? 1 : -1;} \
  static inline s2_t x_compare_type_dominant( const x_inst* a, const x_inst* b ){return  x_compare_t_type_dominant(a ? a->_ : 0, a, b ? b->_ : 0, b );} \
  static inline s2_t x_compare_t_num_dominant( tp_t ta, const x_inst* a, tp_t tb, const x_inst* b ){return  bcore_compare_num_dominant_sr( sr_twc(ta, a ), sr_twc(tb, b ) );} \
  static inline s2_t x_compare_num_dominant( const x_inst* a, const x_inst* b ){return  x_compare_t_num_dominant(a ? a->_ : 0, a, b ? b->_ : 0, b );}

/**********************************************************************************************************************/
// source: bcore_x_source.h

//----------------------------------------------------------------------------------------------------------------------
// group: x_source

#define TYPEOF_x_source 0x2E28FF78616384DDull
#define TYPEOF_x_source_spect_s 0x23C78F93F3ABADD9ull
#define TYPEOF_x_sink 0x7652A8509073840Bull
#define TYPEOF_x_inst 0xFC4EFF7569E22558ull
#define TYPEOF_x_source_point_s 0x717FF038B7471B82ull
#define BETH_EXPAND_ITEM_x_source_point_s \
  BCORE_DECLARE_OBJECT( x_source_point_s ) \
  { \
      aware_t _; \
      x_source* source; \
      s3_t index; \
  }; \
  static inline x_source_point_s* x_source_point_s_setup_from_source( x_source_point_s* o, x_source* source ); \
  static inline x_source_point_s* x_source_point_s__( x_source_point_s* o, x_source* source ); \
  static inline void x_source_point_s_parse_msg_to_sink_fa( const x_source_point_s* o, x_sink* sink, sc_t format, ... ); \
  static inline void x_source_point_s_parse_msg_fa( const x_source_point_s* o, sc_t format, ... ); \
  static inline er_t x_source_point_s_parse_error_fa( const x_source_point_s* o, sc_t format, ... ); \
  void x_source_point_s_parse_msg_fv( const x_source_point_s* o, sc_t format, va_list args ); \
  x_source* x_source_point_s_clone_source( const x_source_point_s* o ); \
  void x_source_point_s_parse_msg_to_sink_fv( const x_source_point_s* o, x_sink* sink, sc_t format, va_list args ); \
  er_t x_source_point_s_parse_error_fv( const x_source_point_s* o, sc_t format, va_list args ); \
  void x_source_point_s_source_reference_to_sink( const x_source_point_s* o, bl_t file_name_only, x_sink* sink ); \
  static inline x_source_point_s* x_source_point_s_setup_from_source( x_source_point_s* o, x_source* source ){ x_source_a_attach( &(o->source ), (x_source*)( ((x_source*)bcore_fork(source)))); o->index = x_source_get_index(source); return o;} \
  static inline x_source_point_s* x_source_point_s__( x_source_point_s* o, x_source* source ){return  x_source_point_s_setup_from_source(o,source );} \
  static inline void x_source_point_s_parse_msg_to_sink_fa( const x_source_point_s* o, x_sink* sink, sc_t format, ... ){ va_list a; va_start( a, format ); x_source_point_s_parse_msg_to_sink_fv(o,sink, format, a ); va_end( a );} \
  static inline void x_source_point_s_parse_msg_fa( const x_source_point_s* o, sc_t format, ... ){ va_list a; va_start( a, format );               x_source_point_s_parse_msg_fv(o,format, a ); va_end( a );} \
  static inline er_t x_source_point_s_parse_error_fa( const x_source_point_s* o, sc_t format, ... ){ va_list a; va_start( a, format ); er_t err =  x_source_point_s_parse_error_fv(o,format, a ); va_end( a ); return  err;}
#define BETH_EXPAND_GROUP_x_source \
  BCORE_FORWARD_OBJECT( x_source ); \
  BCORE_FORWARD_OBJECT( x_sink ); \
  BCORE_FORWARD_OBJECT( x_inst ); \
  BCORE_FORWARD_OBJECT( x_source_point_s ); \
  static inline x_source* x_source_create_from_file( sc_t path ); \
  static inline x_source* x_source_create_from_st( const st_s* st ); \
  static inline x_source* x_source_create_from_st_d( st_s* st ); \
  static inline x_source* x_source_create_from_sc( sc_t sc ); \
  static inline x_source* x_source_create_from_fv( sc_t format, va_list args ); \
  static inline x_source* x_source_create_from_fa( sc_t format, ... ); \
  x_source* x_source_check_create_from_file( sc_t path ); \
  static inline bl_t x_source_eos( x_source* o ); \
  static inline sz_t x_source_get_data( x_source* o, x_inst* data, sz_t size ); \
  static inline sz_t x_source_inspect_data( x_source* o, x_inst* data, sz_t size ); \
  static inline char x_source_get_char( x_source* o ); \
  static inline char x_source_inspect_char( x_source* o ); \
  static inline s3_t x_source_get_s3( x_source* o ); \
  static inline s2_t x_source_get_s2( x_source* o ); \
  static inline s1_t x_source_get_s1( x_source* o ); \
  static inline s0_t x_source_get_s0( x_source* o ); \
  static inline u3_t x_source_get_u3( x_source* o ); \
  static inline u2_t x_source_get_u2( x_source* o ); \
  static inline u1_t x_source_get_u1( x_source* o ); \
  static inline u0_t x_source_get_u0( x_source* o ); \
  static inline f3_t x_source_get_f3( x_source* o ); \
  static inline f2_t x_source_get_f2( x_source* o ); \
  static inline bl_t x_source_get_bl( x_source* o ); \
  static inline tp_t x_source_get_tp( x_source* o ); \
  static inline sz_t x_source_get_sz( x_source* o ); \
  static inline uz_t x_source_get_uz( x_source* o ); \
  static inline s3_t x_source_inspect_s3( x_source* o ); \
  static inline s2_t x_source_inspect_s2( x_source* o ); \
  static inline s1_t x_source_inspect_s1( x_source* o ); \
  static inline s0_t x_source_inspect_s0( x_source* o ); \
  static inline u3_t x_source_inspect_u3( x_source* o ); \
  static inline u2_t x_source_inspect_u2( x_source* o ); \
  static inline u1_t x_source_inspect_u1( x_source* o ); \
  static inline u0_t x_source_inspect_u0( x_source* o ); \
  static inline f3_t x_source_inspect_f3( x_source* o ); \
  static inline f2_t x_source_inspect_f2( x_source* o ); \
  static inline bl_t x_source_inspect_bl( x_source* o ); \
  static inline tp_t x_source_inspect_tp( x_source* o ); \
  static inline sz_t x_source_inspect_sz( x_source* o ); \
  static inline uz_t x_source_inspect_uz( x_source* o ); \
  static inline s3_t x_source_get_index( const x_source* o ); \
  static inline void x_source_set_index( x_source* o, s3_t index ); \
  static inline bl_t x_source_parse_bl( x_source* o, sc_t format ); \
  static inline er_t x_source_parse_fv( x_source* o, sc_t format, va_list args ); \
  static inline er_t x_source_parse_fa( x_source* o, sc_t format, ... ); \
  static inline er_t x_source_parse_error_fv( const x_source* o, sc_t format, va_list args ); \
  static inline er_t x_source_parse_error_fa( const x_source* o, sc_t format, ... ); \
  static inline void x_source_parse_msg_to_sink_fv( const x_source* o, x_sink* sink, sc_t format, va_list args ); \
  static inline void x_source_parse_msg_to_sink_fa( const x_source* o, x_sink* sink, sc_t format, ... ); \
  void x_source_parse_msg_fv( const x_source* o, sc_t format, va_list args ); \
  static inline void x_source_parse_msg_fa( const x_source* o, sc_t format, ... ); \
  static inline sc_t x_source_get_file( const x_source* o ); \
  st_s* x_source_get_d_dir( const x_source* o ); \
  static inline x_source* x_source_stdin( void ); \
  XOILA_DECLARE_SPECT( x_source ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( x_source ) \
  BETH_EXPAND_ITEM_x_source_point_s \
  static inline x_source* x_source_create_from_file( sc_t path ){return ((x_source*)( bcore_file_exists(path ) ? bcore_file_open_source(path ) : NULL));} \
  static inline x_source* x_source_create_from_st( const st_s* st ){return ((x_source*)( bcore_source_string_s_create_from_string(st )));} \
  static inline x_source* x_source_create_from_st_d( st_s* st ){return ((x_source*)( bcore_source_string_s_create_from_string_d(st )));} \
  static inline x_source* x_source_create_from_sc( sc_t sc ){return ((x_source*)( bcore_source_string_s_create_from_sc(sc )));} \
  static inline x_source* x_source_create_from_fv( sc_t format, va_list args ){return ((x_source*)( bcore_source_string_s_create_fv(format, args )));} \
  static inline x_source* x_source_create_from_fa( sc_t format, ... ){ va_list a; va_start( a, format ); x_source* r = x_source_create_from_fv(format, a ); va_end( a ); return  r;} \
  static inline bl_t x_source_eos( x_source* o ){return  bcore_source_a_eos(((bcore_source*)(o)));} \
  static inline sz_t x_source_get_data( x_source* o, x_inst* data, sz_t size ){return  bcore_source_a_get_data(((bcore_source*)(o)),data, size );} \
  static inline sz_t x_source_inspect_data( x_source* o, x_inst* data, sz_t size ){return  bcore_source_a_inspect_data(((bcore_source*)(o)),data, size );} \
  static inline char x_source_get_char( x_source* o ){return  bcore_source_a_get_char(((bcore_source*)(o)));} \
  static inline char x_source_inspect_char( x_source* o ){return  bcore_source_a_inspect_char(((bcore_source*)(o)));} \
  static inline s3_t x_source_get_s3( x_source* o ){ s3_t v = 0; x_source_get_data(o,((x_inst*)((&(v)))), sizeof( v ) ); return  v;} \
  static inline s2_t x_source_get_s2( x_source* o ){ s2_t v = 0; x_source_get_data(o,((x_inst*)((&(v)))), sizeof( v ) ); return  v;} \
  static inline s1_t x_source_get_s1( x_source* o ){ s1_t v = 0; x_source_get_data(o,((x_inst*)((&(v)))), sizeof( v ) ); return  v;} \
  static inline s0_t x_source_get_s0( x_source* o ){ s0_t v = 0; x_source_get_data(o,((x_inst*)((&(v)))), sizeof( v ) ); return  v;} \
  static inline u3_t x_source_get_u3( x_source* o ){ u3_t v = 0; x_source_get_data(o,((x_inst*)((&(v)))), sizeof( v ) ); return  v;} \
  static inline u2_t x_source_get_u2( x_source* o ){ u2_t v = 0; x_source_get_data(o,((x_inst*)((&(v)))), sizeof( v ) ); return  v;} \
  static inline u1_t x_source_get_u1( x_source* o ){ u1_t v = 0; x_source_get_data(o,((x_inst*)((&(v)))), sizeof( v ) ); return  v;} \
  static inline u0_t x_source_get_u0( x_source* o ){return  bcore_source_a_get_u0(((bcore_source*)(o)));} \
  static inline f3_t x_source_get_f3( x_source* o ){ f3_t v = 0; x_source_get_data(o,((x_inst*)((&(v)))), sizeof( v ) ); return  v;} \
  static inline f2_t x_source_get_f2( x_source* o ){ f2_t v = 0; x_source_get_data(o,((x_inst*)((&(v)))), sizeof( v ) ); return  v;} \
  static inline bl_t x_source_get_bl( x_source* o ){return  bcore_source_a_get_u0(((bcore_source*)(o)));} \
  static inline tp_t x_source_get_tp( x_source* o ){ tp_t v = 0; x_source_get_data(o,((x_inst*)((&(v)))), sizeof( v ) ); return  v;} \
  static inline sz_t x_source_get_sz( x_source* o ){ /*!must be s3*/ s3_t v = 0; x_source_get_data(o,((x_inst*)((&(v)))), sizeof( v ) ); return  v;} \
  static inline uz_t x_source_get_uz( x_source* o ){ /*!must be u3*/ u3_t v = 0; x_source_get_data(o,((x_inst*)((&(v)))), sizeof( v ) ); return  v;} \
  static inline s3_t x_source_inspect_s3( x_source* o ){ s3_t v = 0; x_source_inspect_data(o,((x_inst*)((&(v)))), sizeof( v ) ); return  v;} \
  static inline s2_t x_source_inspect_s2( x_source* o ){ s2_t v = 0; x_source_inspect_data(o,((x_inst*)((&(v)))), sizeof( v ) ); return  v;} \
  static inline s1_t x_source_inspect_s1( x_source* o ){ s1_t v = 0; x_source_inspect_data(o,((x_inst*)((&(v)))), sizeof( v ) ); return  v;} \
  static inline s0_t x_source_inspect_s0( x_source* o ){ s0_t v = 0; x_source_inspect_data(o,((x_inst*)((&(v)))), sizeof( v ) ); return  v;} \
  static inline u3_t x_source_inspect_u3( x_source* o ){ u3_t v = 0; x_source_inspect_data(o,((x_inst*)((&(v)))), sizeof( v ) ); return  v;} \
  static inline u2_t x_source_inspect_u2( x_source* o ){ u2_t v = 0; x_source_inspect_data(o,((x_inst*)((&(v)))), sizeof( v ) ); return  v;} \
  static inline u1_t x_source_inspect_u1( x_source* o ){ u1_t v = 0; x_source_inspect_data(o,((x_inst*)((&(v)))), sizeof( v ) ); return  v;} \
  static inline u0_t x_source_inspect_u0( x_source* o ){return  bcore_source_a_inspect_u0(((bcore_source*)(o)));} \
  static inline f3_t x_source_inspect_f3( x_source* o ){ f3_t v = 0; x_source_inspect_data(o,((x_inst*)((&(v)))), sizeof( v ) ); return  v;} \
  static inline f2_t x_source_inspect_f2( x_source* o ){ f2_t v = 0; x_source_inspect_data(o,((x_inst*)((&(v)))), sizeof( v ) ); return  v;} \
  static inline bl_t x_source_inspect_bl( x_source* o ){ bl_t v = 0; x_source_inspect_data(o,((x_inst*)((&(v)))), sizeof( v ) ); return  v;} \
  static inline tp_t x_source_inspect_tp( x_source* o ){ tp_t v = 0; x_source_inspect_data(o,((x_inst*)((&(v)))), sizeof( v ) ); return  v;} \
  static inline sz_t x_source_inspect_sz( x_source* o ){ /*!must be s3*/ s3_t v = 0; x_source_inspect_data(o,((x_inst*)((&(v)))), sizeof( v ) ); return  v;} \
  static inline uz_t x_source_inspect_uz( x_source* o ){ /*!must be u3*/ u3_t v = 0; x_source_inspect_data(o,((x_inst*)((&(v)))), sizeof( v ) ); return  v;} \
  static inline s3_t x_source_get_index( const x_source* o ){return  bcore_source_a_get_index(((const bcore_source*)(o)));} \
  static inline void x_source_set_index( x_source* o, s3_t index ){bcore_source_a_set_index(((bcore_source*)(o)),index );} \
  static inline bl_t x_source_parse_bl( x_source* o, sc_t format ){return  bcore_source_a_parse_bl(((bcore_source*)(o)),format );} \
  static inline er_t x_source_parse_fv( x_source* o, sc_t format, va_list args ){return  bcore_source_a_parse_em_fv(((bcore_source*)(o)),format, args );} \
  static inline er_t x_source_parse_fa( x_source* o, sc_t format, ... ){ va_list a; va_start( a, format ); er_t r = x_source_parse_fv(o,format, a ); va_end( a ); return  r;} \
  static inline er_t x_source_parse_error_fv( const x_source* o, sc_t format, va_list args ){return  bcore_source_a_parse_error_fv(((const bcore_source*)(o)),format, args );} \
  static inline er_t x_source_parse_error_fa( const x_source* o, sc_t format, ... ){ va_list a; va_start( a, format ); er_t r = x_source_parse_error_fv(o,format, a ); va_end( a ); return  r;} \
  static inline void x_source_parse_msg_to_sink_fv( const x_source* o, x_sink* sink, sc_t format, va_list args ){bcore_source_a_parse_msg_to_sink_fv(((bcore_source*)(o)),((bcore_sink*)(sink)), format, args );} \
  static inline void x_source_parse_msg_to_sink_fa( const x_source* o, x_sink* sink, sc_t format, ... ){ va_list a; va_start( a, format ); x_source_parse_msg_to_sink_fv(o,sink, format, a ); va_end( a );} \
  static inline void x_source_parse_msg_fa( const x_source* o, sc_t format, ... ){ va_list a; va_start( a, format ); x_source_parse_msg_fv(o,format, a ); va_end( a );} \
  static inline sc_t x_source_get_file( const x_source* o ){return  bcore_source_a_get_file(((const bcore_source*)(o)));} \
  static inline x_source* x_source_stdin( void ){return  ( x_source* )BCORE_STDIN;}

/**********************************************************************************************************************/
// source: bcore_x_sink.h

//----------------------------------------------------------------------------------------------------------------------
// group: x_sink

#define TYPEOF_x_sink 0x7652A8509073840Bull
#define TYPEOF_x_sink_spect_s 0xD9C03942D97B5EEBull
#define BETH_EXPAND_GROUP_x_sink \
  BCORE_FORWARD_OBJECT( x_sink ); \
  static inline x_sink* x_sink_create_from_file( sc_t path ); \
  x_sink* x_sink_check_create_from_file( sc_t path ); \
  static inline sz_t x_sink_push_data( x_sink* o, const x_inst* data, sz_t size ); \
  static inline x_sink* x_sink_push_char( x_sink* o, char c ); \
  static inline x_sink* x_sink_push_sc( x_sink* o, sc_t sc ); \
  static inline x_sink* x_sink_push_st( x_sink* o, const st_s* st ); \
  static inline x_sink* x_sink_push_st_d( x_sink* o, st_s* st ); \
  static inline x_sink* x_sink_push_fv( x_sink* o, sc_t format, va_list args ); \
  static inline x_sink* x_sink_push_fa( x_sink* o, sc_t format, ... ); \
  static inline x_sink* x_sink_flush( x_sink* o ); \
  static inline x_sink* x_sink_push_s3( x_sink* o, s3_t v ); \
  static inline x_sink* x_sink_push_s2( x_sink* o, s2_t v ); \
  static inline x_sink* x_sink_push_s1( x_sink* o, s1_t v ); \
  static inline x_sink* x_sink_push_s0( x_sink* o, s0_t v ); \
  static inline x_sink* x_sink_push_u3( x_sink* o, u3_t v ); \
  static inline x_sink* x_sink_push_u2( x_sink* o, u2_t v ); \
  static inline x_sink* x_sink_push_u1( x_sink* o, u1_t v ); \
  static inline x_sink* x_sink_push_u0( x_sink* o, u0_t v ); \
  static inline x_sink* x_sink_push_f3( x_sink* o, f3_t v ); \
  static inline x_sink* x_sink_push_f2( x_sink* o, f2_t v ); \
  static inline x_sink* x_sink_push_bl( x_sink* o, bl_t v ); \
  static inline x_sink* x_sink_push_tp( x_sink* o, tp_t v ); \
  static inline x_sink* x_sink_push_sz( x_sink* o, sz_t v ); \
  static inline x_sink* x_sink_push_uz( x_sink* o, uz_t v ); \
  static inline x_sink* x_sink_stdout( void ); \
  static inline x_sink* x_sink_stderr( void ); \
  XOILA_DECLARE_SPECT( x_sink ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( x_sink ) \
  static inline x_sink* x_sink_create_from_file( sc_t path ){return ((x_sink*)( bcore_file_try_open_sink(path )));} \
  static inline sz_t x_sink_push_data( x_sink* o, const x_inst* data, sz_t size ){return  bcore_sink_a_push_data(((bcore_sink*)(o)),data, size );} \
  static inline x_sink* x_sink_push_char( x_sink* o, char c ){return ((x_sink*)( bcore_sink_a_push_char(((bcore_sink*)(o)),c )));} \
  static inline x_sink* x_sink_push_sc( x_sink* o, sc_t sc ){return ((x_sink*)( bcore_sink_a_push_sc(((bcore_sink*)(o)),sc )));} \
  static inline x_sink* x_sink_push_st( x_sink* o, const st_s* st ){return ((x_sink*)( bcore_sink_a_push_string(((bcore_sink*)(o)),st )));} \
  static inline x_sink* x_sink_push_st_d( x_sink* o, st_s* st ){return ((x_sink*)( bcore_sink_a_push_string_d(((bcore_sink*)(o)),st )));} \
  static inline x_sink* x_sink_push_fv( x_sink* o, sc_t format, va_list args ){return ((x_sink*)( bcore_sink_a_push_fv(((bcore_sink*)(o)),format, args )));} \
  static inline x_sink* x_sink_push_fa( x_sink* o, sc_t format, ... ){ va_list a; va_start( a, format ); x_sink_push_fv(o,format, a ); va_end( a ); return o;} \
  static inline x_sink* x_sink_flush( x_sink* o ){return ((x_sink*)( bcore_sink_a_flush(((bcore_sink*)(o)))));} \
  static inline x_sink* x_sink_push_s3( x_sink* o, s3_t v ){x_sink_push_data(o,((const x_inst*)((&(v)))), sizeof( v ) );return o;} \
  static inline x_sink* x_sink_push_s2( x_sink* o, s2_t v ){x_sink_push_data(o,((const x_inst*)((&(v)))), sizeof( v ) );return o;} \
  static inline x_sink* x_sink_push_s1( x_sink* o, s1_t v ){x_sink_push_data(o,((const x_inst*)((&(v)))), sizeof( v ) );return o;} \
  static inline x_sink* x_sink_push_s0( x_sink* o, s0_t v ){x_sink_push_data(o,((const x_inst*)((&(v)))), sizeof( v ) );return o;} \
  static inline x_sink* x_sink_push_u3( x_sink* o, u3_t v ){x_sink_push_data(o,((const x_inst*)((&(v)))), sizeof( v ) );return o;} \
  static inline x_sink* x_sink_push_u2( x_sink* o, u2_t v ){x_sink_push_data(o,((const x_inst*)((&(v)))), sizeof( v ) );return o;} \
  static inline x_sink* x_sink_push_u1( x_sink* o, u1_t v ){x_sink_push_data(o,((const x_inst*)((&(v)))), sizeof( v ) );return o;} \
  static inline x_sink* x_sink_push_u0( x_sink* o, u0_t v ){return ((x_sink*)( bcore_sink_a_push_u0(((bcore_sink*)(o)),v )));} \
  static inline x_sink* x_sink_push_f3( x_sink* o, f3_t v ){x_sink_push_data(o,((const x_inst*)((&(v)))), sizeof( v ) );return o;} \
  static inline x_sink* x_sink_push_f2( x_sink* o, f2_t v ){x_sink_push_data(o,((const x_inst*)((&(v)))), sizeof( v ) );return o;} \
  static inline x_sink* x_sink_push_bl( x_sink* o, bl_t v ){return ((x_sink*)( bcore_sink_a_push_u0(((bcore_sink*)(o)),v )));} \
  static inline x_sink* x_sink_push_tp( x_sink* o, tp_t v ){x_sink_push_data(o,((const x_inst*)((&(v)))), sizeof( v ) );return o;} \
  static inline x_sink* x_sink_push_sz( x_sink* o, sz_t v ){x_sink_push_s3(o,v );return o;} \
  static inline x_sink* x_sink_push_uz( x_sink* o, uz_t v ){x_sink_push_u3(o,v );return o;} \
  static inline x_sink* x_sink_stdout( void ){return  ( x_sink* )BCORE_STDOUT;} \
  static inline x_sink* x_sink_stderr( void ){return  ( x_sink* )BCORE_STDERR;}

/**********************************************************************************************************************/
// source: bcore_x_array.h

//----------------------------------------------------------------------------------------------------------------------
// group: x_array

#define TYPEOF_x_array 0x213BA9DF948F1DC5ull
#define TYPEOF_x_array_spect_s 0xD636C012F755E8E1ull
#define TYPEOF_size 0x4DEA9618E618AE3Cull
#define TYPEOF_space 0x436CF28AB247E3E5ull
#define TYPEOF_data 0x855B556730A34A05ull
#define BETH_EXPAND_GROUP_x_array \
  BCORE_FORWARD_OBJECT( x_array ); \
  BCORE_FORWARD_OBJECT( x_array_feature ); \
  static inline sz_t x_array_t_size( const x_array* o, tp_t t ); \
  static inline sz_t x_array_size( const x_array* o ); \
  static inline x_array* x_array_t_set_size( x_array* o, tp_t t, sz_t size ); \
  static inline x_array* x_array_set_size( x_array* o, sz_t size ); \
  static inline x_array* x_array_t_set_space( x_array* o, tp_t t, sz_t space ); \
  static inline x_array* x_array_set_space( x_array* o, sz_t space ); \
  static inline x_array* x_array_t_clear( x_array* o, tp_t t ); \
  static inline x_array* x_array_clear( x_array* o ); \
  static inline x_array* x_array_t_sort( x_array* o, tp_t t, s2_t direction ); \
  static inline x_array* x_array_sort( x_array* o, s2_t direction ); \
  static inline bl_t x_array_t_is_array( tp_t t ); \
  static inline bl_t x_array_t_is_pure_array( tp_t t ); \
  static inline bl_t x_array_t_is_fixed( tp_t t ); \
  static inline bl_t x_array_t_is_static( tp_t t ); \
  static inline bl_t x_array_t_is_of_aware( tp_t t ); \
  static inline bl_t x_array_t_is_of_links( tp_t t ); \
  static inline bl_t x_array_t_is_mono_typed( tp_t t ); \
  static inline bl_t x_array_is_array( const x_array* o ); \
  static inline bl_t x_array_is_pure_array( const x_array* o ); \
  static inline bl_t x_array_is_fixed( const x_array* o ); \
  static inline bl_t x_array_is_static( const x_array* o ); \
  static inline bl_t x_array_is_of_aware( const x_array* o ); \
  static inline bl_t x_array_is_of_links( const x_array* o ); \
  static inline bl_t x_array_is_mono_typed( const x_array* o ); \
  static inline bl_t x_array_t_is_mutable_mono_typed( tp_t t ); \
  static inline bl_t x_array_is_mutable_mono_typed( const x_array* o ); \
  static inline bl_t x_array_t_is_is_multi_typed( tp_t t ); \
  static inline bl_t x_array_is_is_multi_typed( const x_array* o ); \
  static inline tp_t x_array_t_get_static_type( tp_t t ); \
  static inline tp_t x_array_get_static_type( const x_array* o ); \
  static inline tp_t x_array_t_get_mono_type( const x_array* o, tp_t t ); \
  static inline tp_t x_array_get_mono_type( const x_array* o ); \
  static inline tp_t x_array_t_get_type( const x_array* o, tp_t t, sz_t index ); \
  static inline tp_t x_array_get_type( const x_array* o, sz_t index ); \
  static inline const x_inst* x_array_t_get_data_c( const x_array* o, tp_t t ); \
  static inline const x_inst* x_array_get_data_c( const x_array* o ); \
  static inline x_inst* x_array_t_get_data_m( x_array* o, tp_t t ); \
  static inline x_inst* x_array_get_data_m( x_array* o ); \
  static inline uz_t x_array_t_get_unit_size( const x_array* o, tp_t t ); \
  static inline uz_t x_array_get_unit_size( const x_array* o ); \
  static inline void x_array_t_set_gtype( x_array* o, tp_t t, tp_t type ); \
  static inline void x_array_set_gtype( x_array* o, tp_t type ); \
  static inline void x_array_t_push_sr( x_array* o, tp_t t, sr_s sr ); \
  static inline x_inst* x_array_push_d( x_array* o, x_inst* v ); \
  static inline x_inst* x_array_push_c( x_array* o, const x_inst* v ); \
  static inline x_inst* x_array_push_t( x_array* o, tp_t val_type ); \
  static inline x_inst* x_array_push( x_array* o ); \
  static inline void x_array_push_sr( x_array* o, sr_s sr ); \
  static inline void x_array_t_set_sr( x_array* o, tp_t t, sz_t index, sr_s sr ); \
  static inline void x_array_t_insert_sr( x_array* o, tp_t t, sz_t index, sr_s sr ); \
  static inline void x_array_t_remove( x_array* o, tp_t t, sz_t index ); \
  static inline void x_array_t_pop( x_array* o, tp_t t ); \
  static inline void x_array_t_swap( x_array* o, tp_t t, sz_t index1, sz_t index2 ); \
  static inline x_inst* x_array_set_d( x_array* o, sz_t index, x_inst* v ); \
  static inline x_inst* x_array_set_c( x_array* o, sz_t index, const x_inst* v ); \
  static inline x_inst* x_array_set_t( x_array* o, sz_t index, tp_t val_type ); \
  static inline void x_array_set_sr( x_array* o, sz_t index, sr_s sr ); \
  static inline void x_array_insert_sr( x_array* o, sz_t index, sr_s sr ); \
  static inline void x_array_remove( x_array* o, sz_t index ); \
  static inline void x_array_pop( x_array* o ); \
  static inline void x_array_swap( x_array* o, sz_t index1, sz_t index2 ); \
  static inline void x_array_t_remove_null_elements( x_array* o, tp_t t ); \
  static inline void x_array_remove_null_elements( x_array* o ); \
  static inline sr_s x_array_t_m_get_sr( x_array* o, tp_t t, sz_t index ); \
  static inline sr_s x_array_m_get_sr( x_array* o, sz_t index ); \
  static inline sr_s x_array_c_get_sr( const x_array* o, sz_t index ); \
  static inline f3_t x_array_c_get_f3( const x_array* o, sz_t index ); \
  static inline u3_t x_array_c_get_u3( const x_array* o, sz_t index ); \
  static inline s3_t x_array_c_get_s3( const x_array* o, sz_t index ); \
  static inline bl_t x_array_c_get_bl( const x_array* o, sz_t index ); \
  static inline tp_t x_array_c_get_tp( const x_array* o, sz_t index ); \
  static inline x_array* x_array_t_reorder( x_array* o, tp_t t, const bcore_arr_uz_s* index_array ); \
  static inline x_array* x_array_reorder( x_array* o, const bcore_arr_uz_s* index_array ); \
  x_inst* x_array_t_push_d( x_array* o, tp_t t, x_inst* v ); \
  x_inst* x_array_t_push_c( x_array* o, tp_t t, const x_inst* v ); \
  x_inst* x_array_t_push_t( x_array* o, tp_t t, tp_t val_type ); \
  x_inst* x_array_t_push( x_array* o, tp_t t ); \
  x_inst* x_array_t_set_d( x_array* o, tp_t t, sz_t index, x_inst* v ); \
  x_inst* x_array_t_set_c( x_array* o, tp_t t, sz_t index, const x_inst* v ); \
  x_inst* x_array_t_set_t( x_array* o, tp_t t, sz_t index, tp_t val_type ); \
  x_inst* x_array_t_insert_d( x_array* o, tp_t t, sz_t index, x_inst* v ); \
  x_inst* x_array_t_insert_c( x_array* o, tp_t t, sz_t index, const x_inst* v ); \
  x_inst* x_array_t_insert_t( x_array* o, tp_t t, sz_t index, tp_t val_type ); \
  sr_s x_array_t_c_get_sr( const x_array* o, tp_t t, sz_t index ); \
  XOILA_DECLARE_SPECT( x_array ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( x_array ) \
  BETH_EXPAND_GROUP_x_array_feature \
  static inline sz_t x_array_t_size( const x_array* o, tp_t t ){return  bcore_array_t_get_size ( t, ((bcore_array*)(o)) );} \
  static inline sz_t x_array_size( const x_array* o ){return  x_array_t_size(o,o->_ );} \
  static inline x_array* x_array_t_set_size( x_array* o, tp_t t, sz_t size ){bcore_array_t_set_size ( t, ((bcore_array*)(o)), size  );return o;} \
  static inline x_array* x_array_set_size( x_array* o, sz_t size ){return  x_array_t_set_size(o,o->_, size );} \
  static inline x_array* x_array_t_set_space( x_array* o, tp_t t, sz_t space ){bcore_array_t_set_space( t, ((bcore_array*)(o)), space );return o;} \
  static inline x_array* x_array_set_space( x_array* o, sz_t space ){return  x_array_t_set_space(o,o->_, space );} \
  static inline x_array* x_array_t_clear( x_array* o, tp_t t ){return  x_array_t_set_space(o,t, 0 );} \
  static inline x_array* x_array_clear( x_array* o ){return  x_array_t_clear(o,o->_ );} \
  static inline x_array* x_array_t_sort( x_array* o, tp_t t, s2_t direction ){bcore_array_t_sort( t, ((bcore_array*)(o)), 0, -1, direction );return o;} \
  static inline x_array* x_array_sort( x_array* o, s2_t direction ){return  x_array_t_sort(o,o->_, direction );} \
  static inline bl_t x_array_t_is_array( tp_t t ){return  bcore_spect_trait_supported( TYPEOF_bcore_array, t );} \
  static inline bl_t x_array_t_is_pure_array( tp_t t ){return  bcore_via_t_is_pure_array( t, NULL );} \
  static inline bl_t x_array_t_is_fixed( tp_t t ){return  bcore_array_s_get_typed( t )->size_fix > 0;} \
  static inline bl_t x_array_t_is_static( tp_t t ){return  bcore_array_s_get_typed( t )->is_static;} \
  static inline bl_t x_array_t_is_of_aware( tp_t t ){return  bcore_array_s_get_typed( t )->is_of_aware;} \
  static inline bl_t x_array_t_is_of_links( tp_t t ){return  bcore_array_s_get_typed( t )->is_of_links;} \
  static inline bl_t x_array_t_is_mono_typed( tp_t t ){return  bcore_array_s_get_typed( t )->is_mono_typed;} \
  static inline bl_t x_array_is_array( const x_array* o ){return  x_array_t_is_array(o->_ );} \
  static inline bl_t x_array_is_pure_array( const x_array* o ){return  x_array_t_is_pure_array(o->_ );} \
  static inline bl_t x_array_is_fixed( const x_array* o ){return  x_array_t_is_fixed(o->_ );} \
  static inline bl_t x_array_is_static( const x_array* o ){return  x_array_t_is_static(o->_ );} \
  static inline bl_t x_array_is_of_aware( const x_array* o ){return  x_array_t_is_of_aware(o->_ );} \
  static inline bl_t x_array_is_of_links( const x_array* o ){return  x_array_t_is_of_links(o->_ );} \
  static inline bl_t x_array_is_mono_typed( const x_array* o ){return  x_array_t_is_mono_typed(o->_ );} \
  static inline bl_t x_array_t_is_mutable_mono_typed( tp_t t ){return  bcore_array_p_is_mutable_mono_typed( bcore_array_s_get_typed( t ) );} \
  static inline bl_t x_array_is_mutable_mono_typed( const x_array* o ){return  x_array_t_is_mutable_mono_typed(o->_ );} \
  static inline bl_t x_array_t_is_is_multi_typed( tp_t t ){return  bcore_array_p_is_multi_typed( bcore_array_s_get_typed( t ) );} \
  static inline bl_t x_array_is_is_multi_typed( const x_array* o ){return  x_array_t_is_is_multi_typed(o->_ );} \
  static inline tp_t x_array_t_get_static_type( tp_t t ){return  bcore_array_p_get_static_type( bcore_array_s_get_typed( t ) );} \
  static inline tp_t x_array_get_static_type( const x_array* o ){return  x_array_t_get_static_type(o->_ );} \
  static inline tp_t x_array_t_get_mono_type( const x_array* o, tp_t t ){return  bcore_array_p_get_mono_type( bcore_array_s_get_typed( t ), o );} \
  static inline tp_t x_array_get_mono_type( const x_array* o ){return  x_array_t_get_mono_type(o,o->_ );} \
  static inline tp_t x_array_t_get_type( const x_array* o, tp_t t, sz_t index ){return  bcore_array_p_get_type( bcore_array_s_get_typed( t ), o, index );} \
  static inline tp_t x_array_get_type( const x_array* o, sz_t index ){return  x_array_t_get_type(o,o->_, index );} \
  static inline const x_inst* x_array_t_get_data_c( const x_array* o, tp_t t ){return  bcore_array_p_get_c_data( bcore_array_s_get_typed( t ), o );} \
  static inline const x_inst* x_array_get_data_c( const x_array* o ){return  x_array_t_get_data_c(o,o->_ );} \
  static inline x_inst* x_array_t_get_data_m( x_array* o, tp_t t ){return  bcore_array_p_get_d_data( bcore_array_s_get_typed( t ), o );} \
  static inline x_inst* x_array_get_data_m( x_array* o ){return  x_array_t_get_data_m(o,o->_ );} \
  static inline uz_t x_array_t_get_unit_size( const x_array* o, tp_t t ){return  bcore_array_p_get_unit_size ( bcore_array_s_get_typed( t ), o );} \
  static inline uz_t x_array_get_unit_size( const x_array* o ){return  x_array_t_get_unit_size(o,o->_ );} \
  static inline void x_array_t_set_gtype( x_array* o, tp_t t, tp_t type ){bcore_array_p_set_gtype( bcore_array_s_get_typed( t ), ((bcore_array*)(o)), type );} \
  static inline void x_array_set_gtype( x_array* o, tp_t type ){x_array_t_set_gtype(o,o->_, type );} \
  static inline void x_array_t_push_sr( x_array* o, tp_t t, sr_s sr ){bcore_array_t_push( t, ( bcore_array* )o, sr );} \
  static inline x_inst* x_array_push_d( x_array* o, x_inst* v ){return  x_array_t_push_d(o,o->_, v );} \
  static inline x_inst* x_array_push_c( x_array* o, const x_inst* v ){return  x_array_t_push_c(o,o->_, v );} \
  static inline x_inst* x_array_push_t( x_array* o, tp_t val_type ){return  x_array_t_push_t(o,o->_, val_type );} \
  static inline x_inst* x_array_push( x_array* o ){return  x_array_t_push(o,o->_ );} \
  static inline void x_array_push_sr( x_array* o, sr_s sr ){x_array_t_push_sr(o,o->_, sr );} \
  static inline void x_array_t_set_sr( x_array* o, tp_t t, sz_t index, sr_s sr ){bcore_array_t_set( t, ( bcore_array* )o, index, sr );} \
  static inline void x_array_t_insert_sr( x_array* o, tp_t t, sz_t index, sr_s sr ){bcore_array_t_insert( t, ( bcore_array* )o, index, sr );} \
  static inline void x_array_t_remove( x_array* o, tp_t t, sz_t index ){bcore_array_t_remove( t, ( bcore_array* )o, index );} \
  static inline void x_array_t_pop( x_array* o, tp_t t ){bcore_array_t_pop   ( t, ( bcore_array* )o        );} \
  static inline void x_array_t_swap( x_array* o, tp_t t, sz_t index1, sz_t index2 ){bcore_array_t_swap( t, ( bcore_array* )o, index1, index2 );} \
  static inline x_inst* x_array_set_d( x_array* o, sz_t index, x_inst* v ){return  x_array_t_set_d(o,o->_, index, v );} \
  static inline x_inst* x_array_set_c( x_array* o, sz_t index, const x_inst* v ){return  x_array_t_set_c(o,o->_, index, v );} \
  static inline x_inst* x_array_set_t( x_array* o, sz_t index, tp_t val_type ){return  x_array_t_set_t(o,o->_, index, val_type );} \
  static inline void x_array_set_sr( x_array* o, sz_t index, sr_s sr ){x_array_t_set_sr(o,o->_, index, sr );} \
  static inline void x_array_insert_sr( x_array* o, sz_t index, sr_s sr ){x_array_t_insert_sr(o,o->_, index, sr );} \
  static inline void x_array_remove( x_array* o, sz_t index ){x_array_t_remove(o,o->_, index );} \
  static inline void x_array_pop( x_array* o ){x_array_t_pop(o,o->_        );} \
  static inline void x_array_swap( x_array* o, sz_t index1, sz_t index2 ){x_array_t_swap(o,o->_, index1, index2 );} \
  static inline void x_array_t_remove_null_elements( x_array* o, tp_t t ){bcore_array_t_remove_null_elements( t, ( bcore_array* )o );} \
  static inline void x_array_remove_null_elements( x_array* o ){x_array_t_remove_null_elements(o,o->_ );} \
  static inline sr_s x_array_t_m_get_sr( x_array* o, tp_t t, sz_t index ){return  bcore_array_t_get( t, ((bcore_array*)(o)), index );} \
  static inline sr_s x_array_m_get_sr( x_array* o, sz_t index ){return  x_array_t_m_get_sr(o,o->_, index );} \
  static inline sr_s x_array_c_get_sr( const x_array* o, sz_t index ){return  x_array_t_c_get_sr(o,o->_, index );} \
  static inline f3_t x_array_c_get_f3( const x_array* o, sz_t index ){return  sr_to_f3(x_array_t_c_get_sr(o,o->_, index ));} \
  static inline u3_t x_array_c_get_u3( const x_array* o, sz_t index ){return  sr_to_u3(x_array_t_c_get_sr(o,o->_, index ));} \
  static inline s3_t x_array_c_get_s3( const x_array* o, sz_t index ){return  sr_to_s3(x_array_t_c_get_sr(o,o->_, index ));} \
  static inline bl_t x_array_c_get_bl( const x_array* o, sz_t index ){return  sr_to_bl(x_array_t_c_get_sr(o,o->_, index ));} \
  static inline tp_t x_array_c_get_tp( const x_array* o, sz_t index ){return  sr_to_tp(x_array_t_c_get_sr(o,o->_, index ));} \
  static inline x_array* x_array_t_reorder( x_array* o, tp_t t, const bcore_arr_uz_s* index_array ){bcore_array_t_reorder( t, ((bcore_array*)(o)), index_array );return o;} \
  static inline x_array* x_array_reorder( x_array* o, const bcore_arr_uz_s* index_array ){return  x_array_t_reorder(o,o->_, index_array );}

//----------------------------------------------------------------------------------------------------------------------
// group: x_array_feature

#define TYPEOF_x_array_feature 0xD145574098552F10ull
#define TYPEOF_x_array_feature_spect_s 0xEFB9D754D091A938ull
#define BETH_EXPAND_GROUP_x_array_feature \
  BCORE_FORWARD_OBJECT( x_array_feature ); \
  typedef const x_array* (*x_array_feature_c_get_wrapped_array)(const x_array_feature* o ); \
  typedef x_array* (*x_array_feature_m_get_wrapped_array)(x_array_feature* o ); \
  XOILA_DECLARE_SPECT( x_array_feature ) \
  { \
      bcore_spect_header_s header; \
      x_array_feature_c_get_wrapped_array c_get_wrapped_array; \
      x_array_feature_m_get_wrapped_array m_get_wrapped_array; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( x_array_feature ) \
  static inline const x_array* x_array_feature_a_c_get_wrapped_array( const x_array_feature* o ){ const x_array_feature_spect_s* p = x_array_feature_spect_s_get_aware( o ); assert( p->c_get_wrapped_array ); return p->c_get_wrapped_array( o );} \
  static inline bl_t x_array_feature_defines_c_get_wrapped_array( const x_array_feature* o ){ return  true;} \
  static inline const x_array* x_array_feature_c_get_wrapped_array_default( const x_array_feature* o ){return  NULL;} \
  static inline x_array* x_array_feature_a_m_get_wrapped_array( x_array_feature* o ){ const x_array_feature_spect_s* p = x_array_feature_spect_s_get_aware( o ); assert( p->m_get_wrapped_array ); return p->m_get_wrapped_array( o );} \
  static inline bl_t x_array_feature_defines_m_get_wrapped_array( const x_array_feature* o ){ return  true;} \
  static inline x_array* x_array_feature_m_get_wrapped_array_default( x_array_feature* o ){return  NULL;}

/**********************************************************************************************************************/
// source: bcore_x_group.h

//----------------------------------------------------------------------------------------------------------------------
// group: x_group

#define TYPEOF_x_group 0xA6462A398FB43D2Bull
#define TYPEOF_x_group_spect_s 0x6F98C4E297E5BC0Bull
#define BETH_EXPAND_GROUP_x_group \
  BCORE_FORWARD_OBJECT( x_group ); \
  static inline const bcore_arr_tp_s* x_group_get_arr_traitline_stamps( tp_t group ); \
  XOILA_DECLARE_SPECT( x_group ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( x_group ) \
  static inline const bcore_arr_tp_s* x_group_get_arr_traitline_stamps( tp_t group ){return  bcore_xoila_get_arr_traitline_stamps( group );}

/**********************************************************************************************************************/
// source: bcore_x_stamp.h

//----------------------------------------------------------------------------------------------------------------------
// group: x_stamp

#define TYPEOF_x_stamp 0x7FDBB7509FCFB7EFull
#define TYPEOF_x_stamp_spect_s 0x068E092CFA39F65Full
#define TYPEOF_x_stamp_path_s_array_index 0xDF1039CAE32C5978ull
#define TYPEOF_sr_s 0x79B76918FF0AF12Aull
#define TYPEOF_x_stamp_path_s 0x8B231122A65D4291ull
#define BETH_EXPAND_ITEM_x_stamp_path_s \
  BCORE_DECLARE_OBJECT( x_stamp_path_s ) \
  { \
      aware_t _; \
      BCORE_ARRAY_DYN_SOLID_STATIC_S( tp_t, ); \
  }; \
  static inline x_stamp_path_s* x_stamp_path_s_clear( x_stamp_path_s* o ); \
  static inline void x_stamp_path_s_push( x_stamp_path_s* o, tp_t tp ); \
  void x_stamp_path_s_push_index( x_stamp_path_s* o, sz_t index ); \
  static inline x_stamp_path_s* x_stamp_path_s_parse_sc( x_stamp_path_s* o, sc_t sc ); \
  static inline bl_t x_stamp_path_s_exists_in( const x_stamp_path_s* o, const x_inst* inst ); \
  static inline bl_t x_stamp_path_s_exists_in_t( const x_stamp_path_s* o, tp_t t, const x_inst* inst ); \
  static inline tp_t x_stamp_path_s_type_in( const x_stamp_path_s* o, const x_inst* inst ); \
  static inline tp_t x_stamp_path_s_type_in_t( const x_stamp_path_s* o, tp_t t, const x_inst* inst ); \
  static inline sr_s x_stamp_path_s_get_sr_in( const x_stamp_path_s* o, const x_inst* inst ); \
  static inline const x_inst* x_stamp_path_s_c_get_in( const x_stamp_path_s* o, const x_inst* inst ); \
  static inline x_inst* x_stamp_path_s_m_get_in( const x_stamp_path_s* o, x_inst* inst ); \
  static inline void x_stamp_path_s_set_sr_in( const x_stamp_path_s* o, x_inst* inst, sr_s sr_src ); \
  static inline void x_stamp_path_s_set_sr_in_t( const x_stamp_path_s* o, tp_t t, x_inst* inst, sr_s sr_src ); \
  static inline sr_s x_stamp_path_s_set_sr_ret_in( const x_stamp_path_s* o, x_inst* inst, sr_s sr_src ); \
  static inline x_inst* x_stamp_path_s_set_c_in( const x_stamp_path_s* o, x_inst* inst, const x_inst* src ); \
  static inline x_inst* x_stamp_path_s_set_d_in( const x_stamp_path_s* o, x_inst* inst, x_inst* src ); \
  const x_stamp_path_s* x_stamp_path_s_to_sink( const x_stamp_path_s* o, bcore_sink* sink ); \
  x_stamp_path_s* x_stamp_path_s_parse( x_stamp_path_s* o, x_source* source ); \
  sr_s x_stamp_path_s_get_sr_in_t( const x_stamp_path_s* o, tp_t t, const x_inst* inst ); \
  sr_s x_stamp_path_s_set_sr_ret_in_t( const x_stamp_path_s* o, tp_t t, x_inst* inst, sr_s sr_src ); \
  static inline x_stamp_path_s* x_stamp_path_s_clear( x_stamp_path_s* o ){x_array_clear(((x_array*)(o)));return o;} \
  static inline void x_stamp_path_s_push( x_stamp_path_s* o, tp_t tp ){x_array_push_c(((x_array*)(o)),((const x_inst*)(&(tp))) );} \
  static inline x_stamp_path_s* x_stamp_path_s_parse_sc( x_stamp_path_s* o, sc_t sc ){BLM_INIT_LEVEL(0);BLM_RETURNV(x_stamp_path_s*, x_stamp_path_s_parse(o,((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_sc(sc ))) ))} \
  static inline bl_t x_stamp_path_s_exists_in( const x_stamp_path_s* o, const x_inst* inst ){return  x_stamp_path_s_exists_in_t(o,inst->_, inst );} \
  static inline bl_t x_stamp_path_s_exists_in_t( const x_stamp_path_s* o, tp_t t, const x_inst* inst ){BLM_INIT_LEVEL(0); sr_s sr;BLM_T_INIT_SPUSH(sr_s, &sr);; sr = x_stamp_path_s_get_sr_in_t(o,t, inst ); BLM_RETURNV(bl_t, sr.o != NULL)} \
  static inline tp_t x_stamp_path_s_type_in( const x_stamp_path_s* o, const x_inst* inst ){return  x_stamp_path_s_type_in_t(o,inst->_, inst );} \
  static inline tp_t x_stamp_path_s_type_in_t( const x_stamp_path_s* o, tp_t t, const x_inst* inst ){BLM_INIT_LEVEL(0); sr_s sr;BLM_T_INIT_SPUSH(sr_s, &sr);; sr = x_stamp_path_s_get_sr_in_t(o,t, inst ); BLM_RETURNV(tp_t, sr_s_type(&(sr)))} \
  static inline sr_s x_stamp_path_s_get_sr_in( const x_stamp_path_s* o, const x_inst* inst ){return  x_stamp_path_s_get_sr_in_t(o,inst->_, inst );} \
  static inline const x_inst* x_stamp_path_s_c_get_in( const x_stamp_path_s* o, const x_inst* inst ){ sr_s sr = x_stamp_path_s_get_sr_in(o,inst ); if( sr_s_is_strong(&(sr)) ) sr_s_clear(&(sr)); return  ((const x_inst*)(sr.o));} \
  static inline x_inst* x_stamp_path_s_m_get_in( const x_stamp_path_s* o, x_inst* inst ){ sr_s sr = x_stamp_path_s_get_sr_in(o,inst ); if( sr_s_is_strong(&(sr)) ) sr_s_clear(&(sr)); return  ((x_inst*)(sr.o));} \
  static inline void x_stamp_path_s_set_sr_in( const x_stamp_path_s* o, x_inst* inst, sr_s sr_src ){x_stamp_path_s_set_sr_in_t(o,inst->_, inst, sr_src );} \
  static inline void x_stamp_path_s_set_sr_in_t( const x_stamp_path_s* o, tp_t t, x_inst* inst, sr_s sr_src ){x_stamp_path_s_set_sr_ret_in_t(o,inst->_, inst, sr_src );} \
  static inline sr_s x_stamp_path_s_set_sr_ret_in( const x_stamp_path_s* o, x_inst* inst, sr_s sr_src ){return  x_stamp_path_s_set_sr_ret_in_t(o,inst->_, inst, sr_src );} \
  static inline x_inst* x_stamp_path_s_set_c_in( const x_stamp_path_s* o, x_inst* inst, const x_inst* src ){return  x_stamp_path_s_set_sr_ret_in(o,inst, sr_awc( src ) ).o;} \
  static inline x_inst* x_stamp_path_s_set_d_in( const x_stamp_path_s* o, x_inst* inst, x_inst* src ){return  x_stamp_path_s_set_sr_ret_in(o,inst, sr_asm( src ) ).o;}
#define TYPEOF_x_stamp_path_adl_s 0xB48FCD6B3D836E95ull
#define BETH_EXPAND_ITEM_x_stamp_path_adl_s \
  BCORE_DECLARE_OBJECT( x_stamp_path_adl_s ) \
  { \
      aware_t _; \
      BCORE_ARRAY_DYN_LINK_STATIC_S( x_stamp_path_s, ); \
  };
#define BETH_EXPAND_GROUP_x_stamp \
  BCORE_FORWARD_OBJECT( x_stamp ); \
  BCORE_FORWARD_OBJECT( x_stamp_path_s ); \
  BCORE_FORWARD_OBJECT( x_stamp_path_adl_s ); \
  static inline bl_t x_stamp_is_stamp( const x_stamp* o ); \
  static inline bl_t x_stamp_t_is_stamp( tp_t t ); \
  static inline sz_t x_stamp_size( const x_stamp* o ); \
  static inline sz_t x_stamp_t_size( tp_t t ); \
  static inline bl_t x_stamp_is_leaf( const x_stamp* o ); \
  static inline bl_t x_stamp_t_is_leaf( tp_t t ); \
  static inline bl_t x_stamp_is_aware( const x_stamp* o ); \
  static inline bl_t x_stamp_t_is_aware( tp_t t ); \
  static inline bl_t x_stamp_is_array( const x_stamp* o ); \
  static inline bl_t x_stamp_t_is_array( tp_t t ); \
  static inline bl_t x_stamp_is_pure_array( const x_stamp* o ); \
  static inline bl_t x_stamp_t_is_pure_array( tp_t t ); \
  static inline tp_t x_stamp_name( const x_stamp* o, sz_t index ); \
  static inline tp_t x_stamp_t_name( tp_t t, sz_t index ); \
  static inline tp_t x_stamp_index( const x_stamp* o, tp_t name ); \
  static inline tp_t x_stamp_t_index( tp_t t, tp_t name ); \
  static inline bl_t x_stamp_exists( const x_stamp* o, tp_t name ); \
  static inline bl_t x_stamp_t_exists( tp_t t, tp_t name ); \
  static inline bl_t x_stamp_is_static_i( const x_stamp* o, sz_t index ); \
  static inline bl_t x_stamp_t_is_static_i( tp_t t, sz_t index ); \
  static inline bl_t x_stamp_is_static( const x_stamp* o, tp_t name ); \
  static inline bl_t x_stamp_t_is_static( tp_t t, tp_t name ); \
  static inline bl_t x_stamp_is_link_i( const x_stamp* o, sz_t index ); \
  static inline bl_t x_stamp_t_is_link_i( tp_t t, sz_t index ); \
  static inline bl_t x_stamp_is_link( const x_stamp* o, tp_t name ); \
  static inline bl_t x_stamp_t_is_link( tp_t t, tp_t name ); \
  static inline bl_t x_stamp_is_static_link_i( const x_stamp* o, sz_t index ); \
  static inline bl_t x_stamp_t_is_static_link_i( tp_t t, sz_t index ); \
  static inline bl_t x_stamp_is_static_link( const x_stamp* o, tp_t name ); \
  static inline bl_t x_stamp_t_is_static_link( tp_t t, tp_t name ); \
  static inline bl_t x_stamp_is_typed_link_i( const x_stamp* o, sz_t index ); \
  static inline bl_t x_stamp_t_is_typed_link_i( tp_t t, sz_t index ); \
  static inline bl_t x_stamp_is_typed_link( const x_stamp* o, tp_t name ); \
  static inline bl_t x_stamp_t_is_typed_link( tp_t t, tp_t name ); \
  static inline bl_t x_stamp_is_aware_link_i( const x_stamp* o, sz_t index ); \
  static inline bl_t x_stamp_t_is_aware_link_i( tp_t t, sz_t index ); \
  static inline bl_t x_stamp_is_aware_link( const x_stamp* o, tp_t name ); \
  static inline bl_t x_stamp_t_is_aware_link( tp_t t, tp_t name ); \
  static inline bl_t x_stamp_is_array_i( const x_stamp* o, sz_t index ); \
  static inline bl_t x_stamp_t_is_array_i( tp_t t, sz_t index ); \
  static inline bl_t x_stamp_is_array_n( const x_stamp* o, tp_t name ); \
  static inline bl_t x_stamp_t_is_array_n( tp_t t, tp_t name ); \
  static inline tp_t x_stamp_type( const x_stamp* o, tp_t name ); \
  static inline tp_t x_stamp_t_type( const x_stamp* o, tp_t t, tp_t name ); \
  static inline tp_t x_stamp_type_i( const x_stamp* o, sz_t index ); \
  static inline tp_t x_stamp_t_type_i( const x_stamp* o, tp_t t, sz_t index ); \
  static inline sr_s x_stamp_m_get_sr( x_stamp* o, tp_t name ); \
  static inline sr_s x_stamp_m_get_sr_i( x_stamp* o, sz_t index ); \
  static inline sr_s x_stamp_c_get_sr( const x_stamp* o, tp_t name ); \
  static inline sr_s x_stamp_c_get_sr_i( const x_stamp* o, sz_t index ); \
  static inline const x_inst* x_stamp_t_c_get( const x_stamp* o, tp_t t, tp_t name ); \
  static inline x_inst* x_stamp_t_m_get( x_stamp* o, tp_t t, tp_t name ); \
  static inline const x_inst* x_stamp_t_c_get_i( const x_stamp* o, tp_t t, sz_t index ); \
  static inline x_inst* x_stamp_t_m_get_i( x_stamp* o, tp_t t, sz_t index ); \
  static inline const x_inst* x_stamp_c_get( const x_stamp* o, tp_t name ); \
  static inline x_inst* x_stamp_m_get( x_stamp* o, tp_t name ); \
  static inline const x_inst* x_stamp_c_get_i( const x_stamp* o, sz_t index ); \
  static inline x_inst* x_stamp_m_get_i( x_stamp* o, sz_t index ); \
  static inline f3_t x_stamp_get_f3( const x_stamp* o, tp_t name ); \
  static inline s3_t x_stamp_get_s3( const x_stamp* o, tp_t name ); \
  static inline u3_t x_stamp_get_u3( const x_stamp* o, tp_t name ); \
  static inline tp_t x_stamp_get_tp( const x_stamp* o, tp_t name ); \
  static inline bl_t x_stamp_get_bl( const x_stamp* o, tp_t name ); \
  static inline er_t x_stamp_set_sr( x_stamp* o, tp_t name, sr_s sr_src ); \
  static inline er_t x_stamp_set_sr_i( x_stamp* o, sz_t index, sr_s sr_src ); \
  static inline sr_s x_stamp_set_sr_ret( x_stamp* o, tp_t name, sr_s sr_src ); \
  static inline sr_s x_stamp_set_sr_ret_i( x_stamp* o, sz_t index, sr_s sr_src ); \
  static inline x_inst* x_stamp_set_c( x_stamp* o, tp_t name, const x_inst* src ); \
  static inline x_inst* x_stamp_set_d( x_stamp* o, tp_t name, x_inst* src ); \
  static inline x_inst* x_stamp_set_t_c( x_stamp* o, tp_t name, tp_t src_type, const x_inst* src ); \
  static inline x_inst* x_stamp_set_t_d( x_stamp* o, tp_t name, tp_t src_type, x_inst* src ); \
  static inline er_t x_stamp_set_f3( x_stamp* o, tp_t name, f3_t val ); \
  static inline er_t x_stamp_set_s3( x_stamp* o, tp_t name, s3_t val ); \
  static inline er_t x_stamp_set_u3( x_stamp* o, tp_t name, u3_t val ); \
  static inline er_t x_stamp_set_tp( x_stamp* o, tp_t name, tp_t val ); \
  static inline er_t x_stamp_set_bl( x_stamp* o, tp_t name, bl_t val ); \
  static inline er_t x_stamp_set_st( x_stamp* o, tp_t name, const st_s* val ); \
  static inline void x_stamp_t_mutated( x_stamp* o, tp_t t ); \
  static inline void x_stamp_mutated( x_stamp* o ); \
  static inline void x_stamp_t_shelve( x_stamp* o, tp_t t ); \
  static inline void x_stamp_shelve( x_stamp* o ); \
  static inline void x_stamp_t_source( x_stamp* o, tp_t t, x_source* source ); \
  static inline void x_stamp_source( x_stamp* o, x_source* source ); \
  sr_s x_stamp_t_m_get_sr( x_stamp* o, tp_t t, tp_t name ); \
  sr_s x_stamp_t_m_get_sr_i( x_stamp* o, tp_t t, sz_t index ); \
  er_t x_stamp_t_set_sr( x_stamp* o, tp_t t, tp_t name, sr_s sr_src ); \
  sr_s x_stamp_t_set_sr_ret( x_stamp* o, tp_t t, tp_t name, sr_s sr_src ); \
  er_t x_stamp_t_set_sr_i( x_stamp* o, tp_t t, sz_t index, sr_s sr_src ); \
  sr_s x_stamp_t_set_sr_ret_i( x_stamp* o, tp_t t, sz_t index, sr_s sr_src ); \
  sr_s x_stamp_t_c_get_sr( const x_stamp* o, tp_t t, tp_t name ); \
  sr_s x_stamp_t_c_get_sr_i( const x_stamp* o, tp_t t, sz_t index ); \
  void x_stamp_selftest( void ); \
  XOILA_DECLARE_SPECT( x_stamp ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( x_stamp ) \
  BETH_EXPAND_ITEM_x_stamp_path_s \
  BETH_EXPAND_ITEM_x_stamp_path_adl_s \
  static inline bl_t x_stamp_is_stamp( const x_stamp* o ){return  x_inst_has_reflection(o->_ );} \
  static inline bl_t x_stamp_t_is_stamp( tp_t t ){return  x_inst_has_reflection(t );} \
  static inline sz_t x_stamp_size( const x_stamp* o ){return  bcore_via_a_get_size( ((const bcore_via*)(o)) );} \
  static inline sz_t x_stamp_t_size( tp_t t ){return  bcore_via_t_get_size( t, NULL );} \
  static inline bl_t x_stamp_is_leaf( const x_stamp* o ){return  bcore_via_a_is_leaf(((const bcore_via*)( o )));} \
  static inline bl_t x_stamp_t_is_leaf( tp_t t ){return  bcore_via_t_is_leaf( t, NULL );} \
  static inline bl_t x_stamp_is_aware( const x_stamp* o ){return  bcore_via_a_is_aware( ((const bcore_via*)(o)) );} \
  static inline bl_t x_stamp_t_is_aware( tp_t t ){return  bcore_via_t_is_aware( t, NULL );} \
  static inline bl_t x_stamp_is_array( const x_stamp* o ){return  bcore_spect_trait_supported( TYPEOF_bcore_array, o->_ );} \
  static inline bl_t x_stamp_t_is_array( tp_t t ){return  bcore_spect_trait_supported( TYPEOF_bcore_array, t );} \
  static inline bl_t x_stamp_is_pure_array( const x_stamp* o ){return  bcore_via_a_is_pure_array( ((const bcore_via*)(o)) );} \
  static inline bl_t x_stamp_t_is_pure_array( tp_t t ){return  bcore_via_t_is_pure_array( t, NULL );} \
  static inline tp_t x_stamp_name( const x_stamp* o, sz_t index ){return  bcore_via_a_iget_name( ((const bcore_via*)(o)),index );} \
  static inline tp_t x_stamp_t_name( tp_t t, sz_t index ){return  bcore_via_t_iget_name( t, NULL, index );} \
  static inline tp_t x_stamp_index( const x_stamp* o, tp_t name ){return  bcore_via_a_nget_index( ((const bcore_via*)(o)),name );} \
  static inline tp_t x_stamp_t_index( tp_t t, tp_t name ){return  bcore_via_t_nget_index( t, NULL, name );} \
  static inline bl_t x_stamp_exists( const x_stamp* o, tp_t name ){return  bcore_via_a_nexists( ((const bcore_via*)(o)),name );} \
  static inline bl_t x_stamp_t_exists( tp_t t, tp_t name ){return  bcore_via_t_nexists( t, NULL, name );} \
  static inline bl_t x_stamp_is_static_i( const x_stamp* o, sz_t index ){return  bcore_via_a_iis_static( ((const bcore_via*)(o)),index );} \
  static inline bl_t x_stamp_t_is_static_i( tp_t t, sz_t index ){return  bcore_via_t_iis_static( t, NULL, index );} \
  static inline bl_t x_stamp_is_static( const x_stamp* o, tp_t name ){return  bcore_via_a_nis_static( ((const bcore_via*)(o)), name );} \
  static inline bl_t x_stamp_t_is_static( tp_t t, tp_t name ){return  bcore_via_t_nis_static( t, NULL, name );} \
  static inline bl_t x_stamp_is_link_i( const x_stamp* o, sz_t index ){return  bcore_via_a_iis_link( ((const bcore_via*)(o)),index );} \
  static inline bl_t x_stamp_t_is_link_i( tp_t t, sz_t index ){return  bcore_via_t_iis_link( t, NULL, index );} \
  static inline bl_t x_stamp_is_link( const x_stamp* o, tp_t name ){return  bcore_via_a_nis_link( ((const bcore_via*)(o)), name );} \
  static inline bl_t x_stamp_t_is_link( tp_t t, tp_t name ){return  bcore_via_t_nis_link( t, NULL, name );} \
  static inline bl_t x_stamp_is_static_link_i( const x_stamp* o, sz_t index ){return  bcore_via_a_iis_static_link( ((const bcore_via*)(o)), index );} \
  static inline bl_t x_stamp_t_is_static_link_i( tp_t t, sz_t index ){return  bcore_via_t_iis_static_link( t, NULL, index );} \
  static inline bl_t x_stamp_is_static_link( const x_stamp* o, tp_t name ){return  bcore_via_a_nis_static_link( ((const bcore_via*)(o)), name );} \
  static inline bl_t x_stamp_t_is_static_link( tp_t t, tp_t name ){return  bcore_via_t_nis_static_link( t, NULL, name );} \
  static inline bl_t x_stamp_is_typed_link_i( const x_stamp* o, sz_t index ){return  bcore_via_a_iis_typed_link( ((const bcore_via*)(o)), index );} \
  static inline bl_t x_stamp_t_is_typed_link_i( tp_t t, sz_t index ){return  bcore_via_t_iis_typed_link( t, NULL, index );} \
  static inline bl_t x_stamp_is_typed_link( const x_stamp* o, tp_t name ){return  bcore_via_a_nis_typed_link( ((const bcore_via*)(o)), name );} \
  static inline bl_t x_stamp_t_is_typed_link( tp_t t, tp_t name ){return  bcore_via_t_nis_typed_link( t, NULL, name );} \
  static inline bl_t x_stamp_is_aware_link_i( const x_stamp* o, sz_t index ){return  bcore_via_a_iis_aware_link( ((const bcore_via*)(o)), index );} \
  static inline bl_t x_stamp_t_is_aware_link_i( tp_t t, sz_t index ){return  bcore_via_t_iis_aware_link( t, NULL, index );} \
  static inline bl_t x_stamp_is_aware_link( const x_stamp* o, tp_t name ){return  bcore_via_a_nis_aware_link( ((const bcore_via*)(o)), name );} \
  static inline bl_t x_stamp_t_is_aware_link( tp_t t, tp_t name ){return  bcore_via_t_nis_aware_link( t, NULL, name );} \
  static inline bl_t x_stamp_is_array_i( const x_stamp* o, sz_t index ){return  bcore_via_a_iis_array( ((const bcore_via*)(o)),index );} \
  static inline bl_t x_stamp_t_is_array_i( tp_t t, sz_t index ){return  bcore_via_t_iis_array( t, NULL, index );} \
  static inline bl_t x_stamp_is_array_n( const x_stamp* o, tp_t name ){return  bcore_via_a_nis_array( ((const bcore_via*)(o)), name );} \
  static inline bl_t x_stamp_t_is_array_n( tp_t t, tp_t name ){return  bcore_via_t_nis_array( t, NULL, name );} \
  static inline tp_t x_stamp_type( const x_stamp* o, tp_t name ){return  bcore_via_a_nget_type(    ((const bcore_via*)(o)),name );} \
  static inline tp_t x_stamp_t_type( const x_stamp* o, tp_t t, tp_t name ){return  bcore_via_t_nget_type( t, ((const bcore_via*)(o)), name );} \
  static inline tp_t x_stamp_type_i( const x_stamp* o, sz_t index ){return  bcore_via_a_iget_type(    ((const bcore_via*)(o)),index );} \
  static inline tp_t x_stamp_t_type_i( const x_stamp* o, tp_t t, sz_t index ){return  bcore_via_t_iget_type( t, ((const bcore_via*)(o)), index );} \
  static inline sr_s x_stamp_m_get_sr( x_stamp* o, tp_t name ){return  x_stamp_t_m_get_sr(o,o->_, name );} \
  static inline sr_s x_stamp_m_get_sr_i( x_stamp* o, sz_t index ){return  x_stamp_t_m_get_sr_i(o,o->_, index );} \
  static inline sr_s x_stamp_c_get_sr( const x_stamp* o, tp_t name ){return  x_stamp_t_c_get_sr(o,o->_, name );} \
  static inline sr_s x_stamp_c_get_sr_i( const x_stamp* o, sz_t index ){return  x_stamp_t_c_get_sr_i(o,o->_, index );} \
  static inline const x_inst* x_stamp_t_c_get( const x_stamp* o, tp_t t, tp_t name ){ sr_s sr = x_stamp_t_c_get_sr(o,t, name ); if( sr_s_is_strong(&(sr)) ) sr_s_clear(&(sr)); return  sr.o;} \
  static inline x_inst* x_stamp_t_m_get( x_stamp* o, tp_t t, tp_t name ){ sr_s sr = x_stamp_t_m_get_sr(o,t, name ); if( sr_s_is_strong(&(sr)) ) sr_s_clear(&(sr)); return  sr.o;} \
  static inline const x_inst* x_stamp_t_c_get_i( const x_stamp* o, tp_t t, sz_t index ){ sr_s sr = x_stamp_t_c_get_sr_i(o,t, index ); if( sr_s_is_strong(&(sr)) ) sr_s_clear(&(sr)); return  sr.o;} \
  static inline x_inst* x_stamp_t_m_get_i( x_stamp* o, tp_t t, sz_t index ){ sr_s sr = x_stamp_t_m_get_sr_i(o,t, index ); if( sr_s_is_strong(&(sr)) ) sr_s_clear(&(sr)); return  sr.o;} \
  static inline const x_inst* x_stamp_c_get( const x_stamp* o, tp_t name ){ sr_s sr = x_stamp_c_get_sr(o,name ); if( sr_s_is_strong(&(sr)) ) sr_s_clear(&(sr)); return  sr.o;} \
  static inline x_inst* x_stamp_m_get( x_stamp* o, tp_t name ){ sr_s sr = x_stamp_m_get_sr(o,name ); if( sr_s_is_strong(&(sr)) ) sr_s_clear(&(sr)); return  sr.o;} \
  static inline const x_inst* x_stamp_c_get_i( const x_stamp* o, sz_t index ){ sr_s sr = x_stamp_c_get_sr_i(o,index ); if( sr_s_is_strong(&(sr)) ) sr_s_clear(&(sr)); return  sr.o;} \
  static inline x_inst* x_stamp_m_get_i( x_stamp* o, sz_t index ){ sr_s sr = x_stamp_m_get_sr_i(o,index ); if( sr_s_is_strong(&(sr)) ) sr_s_clear(&(sr)); return  sr.o;} \
  static inline f3_t x_stamp_get_f3( const x_stamp* o, tp_t name ){ sr_s sr = x_stamp_c_get_sr(o,name ); f3_t v = sr_s_to_f3(&(sr)); if( sr_s_is_strong(&(sr)) ) sr_s_clear(&(sr)); return  v;} \
  static inline s3_t x_stamp_get_s3( const x_stamp* o, tp_t name ){ sr_s sr = x_stamp_c_get_sr(o,name ); s3_t v = sr_s_to_f3(&(sr)); if( sr_s_is_strong(&(sr)) ) sr_s_clear(&(sr)); return  v;} \
  static inline u3_t x_stamp_get_u3( const x_stamp* o, tp_t name ){ sr_s sr = x_stamp_c_get_sr(o,name ); u3_t v = sr_s_to_f3(&(sr)); if( sr_s_is_strong(&(sr)) ) sr_s_clear(&(sr)); return  v;} \
  static inline tp_t x_stamp_get_tp( const x_stamp* o, tp_t name ){ sr_s sr = x_stamp_c_get_sr(o,name ); tp_t v = sr_s_to_f3(&(sr)); if( sr_s_is_strong(&(sr)) ) sr_s_clear(&(sr)); return  v;} \
  static inline bl_t x_stamp_get_bl( const x_stamp* o, tp_t name ){ sr_s sr = x_stamp_c_get_sr(o,name ); bl_t v = sr_s_to_f3(&(sr)); if( sr_s_is_strong(&(sr)) ) sr_s_clear(&(sr)); return  v;} \
  static inline er_t x_stamp_set_sr( x_stamp* o, tp_t name, sr_s sr_src ){return  x_stamp_t_set_sr(o,o->_, name, sr_src );} \
  static inline er_t x_stamp_set_sr_i( x_stamp* o, sz_t index, sr_s sr_src ){return  x_stamp_t_set_sr_i(o,o->_, index, sr_src );} \
  static inline sr_s x_stamp_set_sr_ret( x_stamp* o, tp_t name, sr_s sr_src ){return  x_stamp_t_set_sr_ret(o,o->_, name, sr_src );} \
  static inline sr_s x_stamp_set_sr_ret_i( x_stamp* o, sz_t index, sr_s sr_src ){return  x_stamp_t_set_sr_ret_i(o,o->_, index, sr_src );} \
  static inline x_inst* x_stamp_set_c( x_stamp* o, tp_t name, const x_inst* src ){return  x_stamp_set_sr_ret(o,name, sr_awc( src ) ).o;} \
  static inline x_inst* x_stamp_set_d( x_stamp* o, tp_t name, x_inst* src ){return  x_stamp_set_sr_ret(o,name, sr_asm( src ) ).o;} \
  static inline x_inst* x_stamp_set_t_c( x_stamp* o, tp_t name, tp_t src_type, const x_inst* src ){return  x_stamp_set_sr_ret(o,name, sr_twc(src_type, src ) ).o;} \
  static inline x_inst* x_stamp_set_t_d( x_stamp* o, tp_t name, tp_t src_type, x_inst* src ){return  x_stamp_set_sr_ret(o,name, sr_tsm(src_type, src ) ).o;} \
  static inline er_t x_stamp_set_f3( x_stamp* o, tp_t name, f3_t val ){return  x_stamp_set_sr(o,name, sr_twc(((tp_t)(TYPEOF_f3_t)),((const x_inst*)( (&(val)) ))) );} \
  static inline er_t x_stamp_set_s3( x_stamp* o, tp_t name, s3_t val ){return  x_stamp_set_sr(o,name, sr_twc(((tp_t)(TYPEOF_s3_t)),((const x_inst*)( (&(val)) ))) );} \
  static inline er_t x_stamp_set_u3( x_stamp* o, tp_t name, u3_t val ){return  x_stamp_set_sr(o,name, sr_twc(((tp_t)(TYPEOF_u3_t)),((const x_inst*)( (&(val)) ))) );} \
  static inline er_t x_stamp_set_tp( x_stamp* o, tp_t name, tp_t val ){return  x_stamp_set_sr(o,name, sr_twc(((tp_t)(TYPEOF_tp_t)),((const x_inst*)( (&(val)) ))) );} \
  static inline er_t x_stamp_set_bl( x_stamp* o, tp_t name, bl_t val ){return  x_stamp_set_sr(o,name, sr_twc(((tp_t)(TYPEOF_bl_t)),((const x_inst*)( (&(val)) ))) );} \
  static inline er_t x_stamp_set_st( x_stamp* o, tp_t name, const st_s* val ){return  x_stamp_set_sr(o,name, sr_twc(((tp_t)(TYPEOF_st_s)),((const x_inst*)( (val) ))) );} \
  static inline void x_stamp_t_mutated( x_stamp* o, tp_t t ){if( bcore_via_call_t_defines_mutated(t ) ) bcore_via_call_t_mutated(((bcore_via_call*)(o)),t );} \
  static inline void x_stamp_mutated( x_stamp* o ){bcore_via_call_a_mutated(((bcore_via_call*)(o)));} \
  static inline void x_stamp_t_shelve( x_stamp* o, tp_t t ){if( bcore_via_call_t_defines_shelve(t ) ) bcore_via_call_t_shelve(((bcore_via_call*)(o)),t );} \
  static inline void x_stamp_shelve( x_stamp* o ){bcore_via_call_a_shelve(((bcore_via_call*)(o)));} \
  static inline void x_stamp_t_source( x_stamp* o, tp_t t, x_source* source ){if( bcore_via_call_t_defines_source(t ) ) bcore_via_call_t_source(((bcore_via_call*)(o)),t,((bcore_source*)( source )));} \
  static inline void x_stamp_source( x_stamp* o, x_source* source ){bcore_via_call_a_source(((bcore_via_call*)(o)),((bcore_source*)(source )));}

/**********************************************************************************************************************/
// source: bcore_x_threads.h

//----------------------------------------------------------------------------------------------------------------------
// group: x

#define TYPEOF_x 0xAF63F54C86021707ull
#define TYPEOF_x_spect_s 0xF7DA5355152AE8F7ull
#define TYPEOF_x_mutex_s 0x339FC5615E44B9CDull
#define TYPEOF_x_lock_s 0x2AA183F86DEBE2E9ull
#define BETH_EXPAND_ITEM_x_lock_s \
  BCORE_DECLARE_OBJECT( x_lock_s ) \
  { \
      aware_t _; \
      x_mutex_s* mutex; \
  }; \
  static inline x_lock_s* x_lock_s__( x_lock_s* o, x_mutex_s* mutex ); \
  x_lock_s* x_lock_s_set( x_lock_s* o, x_mutex_s* mutex ); \
  x_lock_s* x_lock_s_release( x_lock_s* o ); \
  static inline void x_lock_s_down_e( x_lock_s* o ); \
  static inline x_lock_s* x_lock_s__( x_lock_s* o, x_mutex_s* mutex ){return  x_lock_s_set(o,mutex );} \
  static inline void x_lock_s_down_e( x_lock_s* o ){x_lock_s_release(o);}
#define TYPEOF_x_unlock_s 0x6DA985CDBF521AB6ull
#define BETH_EXPAND_ITEM_x_unlock_s \
  BCORE_DECLARE_OBJECT( x_unlock_s ) \
  { \
      aware_t _; \
      x_mutex_s* mutex; \
  }; \
  static inline x_unlock_s* x_unlock_s__( x_unlock_s* o, x_mutex_s* mutex ); \
  x_unlock_s* x_unlock_s_set( x_unlock_s* o, x_mutex_s* mutex ); \
  x_unlock_s* x_unlock_s_release( x_unlock_s* o ); \
  static inline void x_unlock_s_down_e( x_unlock_s* o ); \
  static inline x_unlock_s* x_unlock_s__( x_unlock_s* o, x_mutex_s* mutex ){return  x_unlock_s_set(o,mutex );} \
  static inline void x_unlock_s_down_e( x_unlock_s* o ){x_unlock_s_release(o);}
#define TYPEOF_x_mutex_s 0x339FC5615E44B9CDull
#define BETH_EXPAND_ITEM_x_mutex_s \
  BCORE_DECLARE_OBJECT( x_mutex_s ) \
  { \
      aware_t _; \
      pthread_mutex_t _mutex; \
  }; \
  void x_mutex_s_init_x( x_mutex_s* o ); \
  void x_mutex_s_down_e( x_mutex_s* o ); \
  void x_mutex_s_lock( x_mutex_s* o ); \
  void x_mutex_s_unlock( x_mutex_s* o ); \
  static inline x_lock_s* x_mutex_s_create_lock( x_mutex_s* o ); \
  static inline x_unlock_s* x_mutex_s_create_unlock( x_mutex_s* o ); \
  static inline x_lock_s* x_mutex_s_create_lock( x_mutex_s* o ){return  x_lock_s_set(x_lock_s_create(),o );} \
  static inline x_unlock_s* x_mutex_s_create_unlock( x_mutex_s* o ){return  x_unlock_s_set(x_unlock_s_create(),o );}
#define TYPEOF_x_condition_s 0x16757800448583FFull
#define BETH_EXPAND_ITEM_x_condition_s \
  BCORE_DECLARE_OBJECT( x_condition_s ) \
  { \
      aware_t _; \
      pthread_cond_t _cond; \
  }; \
  void x_condition_s_init_x( x_condition_s* o ); \
  void x_condition_s_down_e( x_condition_s* o ); \
  void x_condition_s_sleep( x_condition_s* o, x_mutex_s* mutex ); \
  void x_condition_s_wake_one( x_condition_s* o ); \
  void x_condition_s_wake_all( x_condition_s* o );
#define BETH_EXPAND_GROUP_x \
  BCORE_FORWARD_OBJECT( x ); \
  BCORE_FORWARD_OBJECT( x_mutex_s ); \
  BCORE_FORWARD_OBJECT( x_lock_s ); \
  BCORE_FORWARD_OBJECT( x_unlock_s ); \
  BCORE_FORWARD_OBJECT( x_mutex_s ); \
  BCORE_FORWARD_OBJECT( x_condition_s ); \
  BCORE_FORWARD_OBJECT( x_thread ); \
  void x_threads_sleep_ns( u3_t ns ); \
  void x_threads_sleep_us( u3_t us ); \
  void x_threads_sleep_ms( u3_t ms ); \
  static inline tp_t x_threads_get_id( void ); \
  er_t x_threads_set_nice_level( s2_t nice_level ); \
  XOILA_DECLARE_SPECT( x ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( x ) \
  BETH_EXPAND_ITEM_x_lock_s \
  BETH_EXPAND_ITEM_x_unlock_s \
  BETH_EXPAND_ITEM_x_mutex_s \
  BETH_EXPAND_ITEM_x_condition_s \
  BETH_EXPAND_GROUP_x_thread \
  static inline tp_t x_threads_get_id( void ){return  gettid();}

//----------------------------------------------------------------------------------------------------------------------
// group: x_thread

#define TYPEOF_x_thread 0x7A71922A48A7E288ull
#define TYPEOF_x_thread_spect_s 0x9961A6A26F0330D0ull
#define TYPEOF_x_thread_s 0x0206F38246B5A992ull
#define BETH_EXPAND_ITEM_x_thread_s \
  BCORE_DECLARE_OBJECT( x_thread_s ) \
  { \
      aware_t _; \
      pthread_t _thread; \
      bl_t _join; \
  }; \
  void x_thread_s_down_e( x_thread_s* o ); \
  x_thread_s* x_thread_s_call_m_thread_func( x_thread_s* o, x_thread* obj ); \
  x_thread_s* x_thread_s_call_c_thread_func( x_thread_s* o, const x_thread* obj ); \
  const x_thread_result* x_thread_s_join( x_thread_s* o );
#define BETH_EXPAND_GROUP_x_thread \
  BCORE_FORWARD_OBJECT( x_thread ); \
  BCORE_FORWARD_OBJECT( x_thread_result ); \
  BCORE_FORWARD_OBJECT( x_thread_s ); \
  typedef const x_thread_result* (*x_thread_m_thread_func)(x_thread* o ); \
  typedef const x_thread_result* (*x_thread_c_thread_func)(const x_thread* o ); \
  XOILA_DECLARE_SPECT( x_thread ) \
  { \
      bcore_spect_header_s header; \
      x_thread_m_thread_func m_thread_func; \
      x_thread_c_thread_func c_thread_func; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( x_thread ) \
  BETH_EXPAND_GROUP_x_thread_result \
  static inline const x_thread_result* x_thread_a_m_thread_func( x_thread* o ){ const x_thread_spect_s* p = x_thread_spect_s_get_aware( o ); assert( p->m_thread_func ); return p->m_thread_func( o );} \
  static inline bl_t x_thread_defines_m_thread_func( const x_thread* o ){  return x_thread_spect_s_get_aware( o )->m_thread_func != NULL;} \
  static inline const x_thread_result* x_thread_a_c_thread_func( const x_thread* o ){ const x_thread_spect_s* p = x_thread_spect_s_get_aware( o ); assert( p->c_thread_func ); return p->c_thread_func( o );} \
  static inline bl_t x_thread_defines_c_thread_func( const x_thread* o ){  return x_thread_spect_s_get_aware( o )->c_thread_func != NULL;} \
  BETH_EXPAND_ITEM_x_thread_s

//----------------------------------------------------------------------------------------------------------------------
// group: x_thread_result

#define TYPEOF_x_thread_result 0xAABAC804D1010FF4ull
#define TYPEOF_x_thread_result_spect_s 0x834AB89DCF978DACull
#define BETH_EXPAND_GROUP_x_thread_result \
  BCORE_FORWARD_OBJECT( x_thread_result ); \
  XOILA_DECLARE_SPECT( x_thread_result ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( x_thread_result )

/**********************************************************************************************************************/
// source: bcore_file.h

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_file

#define TYPEOF_bcore_file 0xB4D56173D633472Dull
#define TYPEOF_bcore_file_spect_s 0x14159982F66DBE69ull
#define TYPEOF_bcore_file_path_s 0x5CF0BE285C3A10CFull
#define BETH_EXPAND_ITEM_bcore_file_path_s \
  BCORE_DECLARE_OBJECT( bcore_file_path_s ) \
  { \
      aware_t _; \
      st_s name; \
      st_s* root; \
      st_s* full; \
  }; \
  void bcore_file_path_s_source( bcore_file_path_s* o, bcore_source* source ); \
  static inline sc_t bcore_file_path_s_get_sc( const bcore_file_path_s* o ); \
  bcore_file_path_s* bcore_file_path_s_set_sc( bcore_file_path_s* o, sc_t name ); \
  er_t bcore_file_path_s_copy_typed( bcore_file_path_s* o, tp_t type, vc_t src ); \
  bcore_file_path_s* bcore_file_path_s_create_sc( sc_t sc ); \
  bcore_file_path_s* bcore_file_path_s_create_st( const st_s* st ); \
  static inline sc_t bcore_file_path_s_get_sc( const bcore_file_path_s* o ){ return  o->full ? o->full->sc : o->name.sc;}
#define BETH_EXPAND_GROUP_bcore_file \
  BCORE_FORWARD_OBJECT( bcore_file ); \
  BCORE_FORWARD_OBJECT( bcore_file_path_s ); \
  st_s* bcore_file_path_minimized( sc_t path ); \
  sc_t bcore_file_extension( sc_t path ); \
  sc_t bcore_file_name( sc_t path ); \
  st_s* bcore_file_strip_extension( sc_t path ); \
  st_s* bcore_file_folder_path( sc_t path ); \
  st_s* bcore_file_folder_name( sc_t path ); \
  bl_t bcore_file_exists( sc_t name ); \
  bl_t bcore_file_touch( sc_t name ); \
  bl_t bcore_file_delete( sc_t name ); \
  bl_t bcore_file_rename( sc_t src_name, sc_t dst_name ); \
  u3_t bcore_file_last_modification_time_us( sc_t name ); \
  bl_t bcore_file_find_descend( sc_t folder, sc_t path, st_s* result ); \
  bcore_source* bcore_file_open_source( sc_t name ); \
  bcore_source* bcore_file_open_source_path( const bcore_file_path_s* path ); \
  bcore_sink* bcore_file_open_sink( sc_t name ); \
  bcore_sink* bcore_file_try_open_sink( sc_t name ); \
  XOILA_DECLARE_SPECT( bcore_file ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bcore_file ) \
  BETH_EXPAND_ITEM_bcore_file_path_s

/**********************************************************************************************************************/
// source: bcore_spect_inst_call.h

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_inst_call

#define TYPEOF_bcore_inst_call 0x013E1A93A9908C88ull
#define TYPEOF_bcore_inst_call_spect_s 0xD9AC3C15E4251AD0ull
#define TYPEOF_bcore_inst_call_init_x 0x2B5D6DECB7FE6924ull
#define TYPEOF_bcore_inst_call_down_e 0x4A39CD7CE0D6EE91ull
#define TYPEOF_bcore_inst_call_copy_e 0xD7EB06B6898F307Cull
#define TYPEOF_bcore_inst_call_copy_x 0xD7EAFBB6898F1DCBull
#define TYPEOF_bcore_inst_call_discard_e 0x58ADAED42247025Dull
#define BETH_EXPAND_GROUP_bcore_inst_call \
  BCORE_FORWARD_OBJECT( bcore_inst_call ); \
  typedef void (*bcore_inst_call_init_x)(bcore_inst_call* o ); \
  typedef void (*bcore_inst_call_down_e)(bcore_inst_call* o ); \
  typedef void (*bcore_inst_call_copy_e)(bcore_inst_call* o, const bcore_inst_call* src ); \
  typedef void (*bcore_inst_call_copy_x)(bcore_inst_call* o, const bcore_inst_call* src ); \
  typedef void (*bcore_inst_call_discard_e)(bcore_inst_call* o ); \
  XOILA_DECLARE_SPECT( bcore_inst_call ) \
  { \
      bcore_spect_header_s header; \
      bcore_inst_call_init_x init_x; \
      bcore_inst_call_down_e down_e; \
      bcore_inst_call_copy_e copy_e; \
      bcore_inst_call_copy_x copy_x; \
      bcore_inst_call_discard_e discard_e; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bcore_inst_call ) \
  static inline void bcore_inst_call_p_init_x( const bcore_inst_call_spect_s* p, bcore_inst_call* o ){ assert( p->init_x ); p->init_x( o );} \
  static inline bl_t bcore_inst_call_p_defines_init_x( const bcore_inst_call_spect_s* p ){  return p->init_x != NULL;} \
  static inline void bcore_inst_call_p_down_e( const bcore_inst_call_spect_s* p, bcore_inst_call* o ){ assert( p->down_e ); p->down_e( o );} \
  static inline bl_t bcore_inst_call_p_defines_down_e( const bcore_inst_call_spect_s* p ){  return p->down_e != NULL;} \
  static inline void bcore_inst_call_p_copy_e( const bcore_inst_call_spect_s* p, bcore_inst_call* o, const bcore_inst_call* src ){ assert( p->copy_e ); p->copy_e( o, src );} \
  static inline bl_t bcore_inst_call_p_defines_copy_e( const bcore_inst_call_spect_s* p ){  return p->copy_e != NULL;} \
  static inline void bcore_inst_call_p_copy_x( const bcore_inst_call_spect_s* p, bcore_inst_call* o, const bcore_inst_call* src ){ assert( p->copy_x ); p->copy_x( o, src );} \
  static inline bl_t bcore_inst_call_p_defines_copy_x( const bcore_inst_call_spect_s* p ){  return p->copy_x != NULL;} \
  static inline void bcore_inst_call_p_discard_e( const bcore_inst_call_spect_s* p, bcore_inst_call* o ){ assert( p->discard_e ); p->discard_e( o );} \
  static inline bl_t bcore_inst_call_p_defines_discard_e( const bcore_inst_call_spect_s* p ){  return p->discard_e != NULL;}

/**********************************************************************************************************************/
// source: bcore_spect_via_call.h

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_via_call

#define TYPEOF_bcore_via_call 0x93D898570E420116ull
#define TYPEOF_bcore_via_call_spect_s 0xDDF677D8ACEB97A2ull
#define BETH_EXPAND_GROUP_bcore_via_call \
  BCORE_FORWARD_OBJECT( bcore_via_call ); \
  typedef void (*bcore_via_call_source)(bcore_via_call* o, bcore_source* source ); \
  typedef void (*bcore_via_call_mutated)(bcore_via_call* o ); \
  typedef void (*bcore_via_call_shelve)(bcore_via_call* o ); \
  XOILA_DECLARE_SPECT( bcore_via_call ) \
  { \
      bcore_spect_header_s header; \
      bcore_via_call_source source; \
      bcore_via_call_mutated mutated; \
      bcore_via_call_shelve shelve; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bcore_via_call ) \
  static inline void bcore_via_call_a_source( bcore_via_call* o, bcore_source* source ){ const bcore_via_call_spect_s* p = bcore_via_call_spect_s_get_aware( o ); assert( p->source ); p->source( o, source );} \
  static inline bl_t bcore_via_call_defines_source( const bcore_via_call* o ){  return bcore_via_call_spect_s_get_aware( o )->source != NULL;} \
  static inline void bcore_via_call_t_source( bcore_via_call* o, tp_t t, bcore_source* source ){ const bcore_via_call_spect_s* p = bcore_via_call_spect_s_get_typed( t ); assert( p->source ); p->source( o, source );} \
  static inline bl_t bcore_via_call_t_defines_source( tp_t t ){  return bcore_via_call_spect_s_get_typed( t )->source != NULL;} \
  static inline void bcore_via_call_p_source( const bcore_via_call_spect_s* p, bcore_via_call* o, bcore_source* source ){ assert( p->source ); p->source( o, source );} \
  static inline bl_t bcore_via_call_p_defines_source( const bcore_via_call_spect_s* p ){  return p->source != NULL;} \
  static inline void bcore_via_call_r_source( const sr_s* o, bcore_source* source ){ ASSERT( !sr_s_is_const( o ) ); const bcore_via_call_spect_s* p = (const bcore_via_call_spect_s*)ch_spect_p( o->p, TYPEOF_bcore_via_call_spect_s ); assert( p->source ); p->source( o->o, source );} \
  static inline bl_t bcore_via_call_r_defines_source( const sr_s* o ){  return ( (bcore_via_call_spect_s*)ch_spect_p( o->p, TYPEOF_bcore_via_call_spect_s ) )->source != NULL;} \
  static inline void bcore_via_call_a_mutated( bcore_via_call* o ){ const bcore_via_call_spect_s* p = bcore_via_call_spect_s_get_aware( o ); assert( p->mutated ); p->mutated( o );} \
  static inline bl_t bcore_via_call_defines_mutated( const bcore_via_call* o ){  return bcore_via_call_spect_s_get_aware( o )->mutated != NULL;} \
  static inline void bcore_via_call_t_mutated( bcore_via_call* o, tp_t t ){ const bcore_via_call_spect_s* p = bcore_via_call_spect_s_get_typed( t ); assert( p->mutated ); p->mutated( o );} \
  static inline bl_t bcore_via_call_t_defines_mutated( tp_t t ){  return bcore_via_call_spect_s_get_typed( t )->mutated != NULL;} \
  static inline void bcore_via_call_p_mutated( const bcore_via_call_spect_s* p, bcore_via_call* o ){ assert( p->mutated ); p->mutated( o );} \
  static inline bl_t bcore_via_call_p_defines_mutated( const bcore_via_call_spect_s* p ){  return p->mutated != NULL;} \
  static inline void bcore_via_call_r_mutated( const sr_s* o ){ ASSERT( !sr_s_is_const( o ) ); const bcore_via_call_spect_s* p = (const bcore_via_call_spect_s*)ch_spect_p( o->p, TYPEOF_bcore_via_call_spect_s ); assert( p->mutated ); p->mutated( o->o );} \
  static inline bl_t bcore_via_call_r_defines_mutated( const sr_s* o ){  return ( (bcore_via_call_spect_s*)ch_spect_p( o->p, TYPEOF_bcore_via_call_spect_s ) )->mutated != NULL;} \
  static inline void bcore_via_call_a_shelve( bcore_via_call* o ){ const bcore_via_call_spect_s* p = bcore_via_call_spect_s_get_aware( o ); assert( p->shelve ); p->shelve( o );} \
  static inline bl_t bcore_via_call_defines_shelve( const bcore_via_call* o ){  return bcore_via_call_spect_s_get_aware( o )->shelve != NULL;} \
  static inline void bcore_via_call_t_shelve( bcore_via_call* o, tp_t t ){ const bcore_via_call_spect_s* p = bcore_via_call_spect_s_get_typed( t ); assert( p->shelve ); p->shelve( o );} \
  static inline bl_t bcore_via_call_t_defines_shelve( tp_t t ){  return bcore_via_call_spect_s_get_typed( t )->shelve != NULL;} \
  static inline void bcore_via_call_p_shelve( const bcore_via_call_spect_s* p, bcore_via_call* o ){ assert( p->shelve ); p->shelve( o );} \
  static inline bl_t bcore_via_call_p_defines_shelve( const bcore_via_call_spect_s* p ){  return p->shelve != NULL;} \
  static inline void bcore_via_call_r_shelve( const sr_s* o ){ ASSERT( !sr_s_is_const( o ) ); const bcore_via_call_spect_s* p = (const bcore_via_call_spect_s*)ch_spect_p( o->p, TYPEOF_bcore_via_call_spect_s ); assert( p->shelve ); p->shelve( o->o );} \
  static inline bl_t bcore_via_call_r_defines_shelve( const sr_s* o ){  return ( (bcore_via_call_spect_s*)ch_spect_p( o->p, TYPEOF_bcore_via_call_spect_s ) )->shelve != NULL;}

/**********************************************************************************************************************/
// source: bcore_main.h

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_main

#define TYPEOF_bcore_main 0x567676BC9B1A5002ull
#define TYPEOF_bcore_main_spect_s 0x6B2E883BCD2D246Eull
#define TYPEOF_bcore_main_frame_s 0x32D158C73073F54Eull
#define BETH_EXPAND_ITEM_bcore_main_frame_s \
  BCORE_DECLARE_OBJECT( bcore_main_frame_s ) \
  { \
      aware_t _; \
      bl_t first_argument_is_path_to_object; \
      bl_t next_argument_is_path_to_script; \
      bl_t create_log_file; \
      st_s log_file_extension; \
      st_s local_path; \
      bl_t local_path_descend; \
      st_s global_path; \
      tp_t object_default_type; \
      sc_t object_interpreter; \
      bcore_arr_st_s args; \
      bcore_mutex_s mutex; \
      bcore_source* source; \
      bcore_sink* sink; \
      bcore_sink* log; \
      sr_s object_sr; \
      st_s object_path; \
      st_s script_path; \
  }; \
  er_t bcore_main_frame_s_exec( bcore_main_frame_s* o, const bcore_arr_st_s* args ); \
  er_t bcore_main_frame_s_main( bcore_main_frame_s* o, sz_t argc, char** argv );
#define TYPEOF_bcore_main_arr_s 0xAB5CAE6CCF6EDB68ull
#define BETH_EXPAND_ITEM_bcore_main_arr_s \
  BCORE_DECLARE_OBJECT( bcore_main_arr_s ) \
  { \
      aware_t _; \
      BCORE_ARRAY_DYN_LINK_STATIC_S( bcore_main, ); \
  };
#define TYPEOF_bcore_main_set_s 0x3503376AEC71546Full
#define BETH_EXPAND_ITEM_bcore_main_set_s \
  BCORE_DECLARE_OBJECT( bcore_main_set_s ) \
  { \
      aware_t _; \
      bcore_main_arr_s arr; \
      bcore_main* current_object; \
      bcore_mutex_s mutex_current_object; \
  }; \
  static inline const x_array* bcore_main_set_s_c_get_wrapped_array( const bcore_main_set_s* o ); \
  static inline x_array* bcore_main_set_s_m_get_wrapped_array( bcore_main_set_s* o ); \
  er_t bcore_main_set_s_main( bcore_main_set_s* o, bcore_main_frame_s* frame ); \
  bl_t bcore_main_set_s_on_termination( bcore_main_set_s* o, const bcore_main_frame_s* frame ); \
  bl_t bcore_main_set_s_on_interrupt( bcore_main_set_s* o, const bcore_main_frame_s* frame ); \
  bl_t bcore_main_set_s_on_suspend( bcore_main_set_s* o, const bcore_main_frame_s* frame ); \
  static inline const x_array* bcore_main_set_s_c_get_wrapped_array( const bcore_main_set_s* o ){return ((const x_array*)(&( o->arr)));} \
  static inline x_array* bcore_main_set_s_m_get_wrapped_array( bcore_main_set_s* o ){return ((x_array*)(&( o->arr)));}
#define BETH_EXPAND_GROUP_bcore_main \
  BCORE_FORWARD_OBJECT( bcore_main ); \
  BCORE_FORWARD_OBJECT( bcore_main_frame_s ); \
  BCORE_FORWARD_OBJECT( bcore_main_arr_s ); \
  BCORE_FORWARD_OBJECT( bcore_main_set_s ); \
  typedef er_t (*bcore_main_main)(bcore_main* o, bcore_main_frame_s* frame ); \
  typedef bl_t (*bcore_main_on_termination)(bcore_main* o, const bcore_main_frame_s* frame ); \
  typedef bl_t (*bcore_main_on_interrupt)(bcore_main* o, const bcore_main_frame_s* frame ); \
  typedef bl_t (*bcore_main_on_suspend)(bcore_main* o, const bcore_main_frame_s* frame ); \
  XOILA_DECLARE_SPECT( bcore_main ) \
  { \
      bcore_spect_header_s header; \
      bcore_main_main main; \
      bcore_main_on_termination on_termination; \
      bcore_main_on_interrupt on_interrupt; \
      bcore_main_on_suspend on_suspend; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bcore_main ) \
  BETH_EXPAND_ITEM_bcore_main_frame_s \
  static inline er_t bcore_main_a_main( bcore_main* o, bcore_main_frame_s* frame ){ const bcore_main_spect_s* p = bcore_main_spect_s_get_aware( o ); assert( p->main ); return p->main( o, frame );} \
  static inline bl_t bcore_main_defines_main( const bcore_main* o ){  return bcore_main_spect_s_get_aware( o )->main != NULL;} \
  static inline er_t bcore_main_r_main( const sr_s* o, bcore_main_frame_s* frame ){ ASSERT( !sr_s_is_const( o ) ); const bcore_main_spect_s* p = (const bcore_main_spect_s*)ch_spect_p( o->p, TYPEOF_bcore_main_spect_s ); assert( p->main ); return p->main( o->o, frame );} \
  static inline bl_t bcore_main_r_defines_main( const sr_s* o ){  return ( (bcore_main_spect_s*)ch_spect_p( o->p, TYPEOF_bcore_main_spect_s ) )->main != NULL;} \
  static inline bl_t bcore_main_a_on_termination( bcore_main* o, const bcore_main_frame_s* frame ){ const bcore_main_spect_s* p = bcore_main_spect_s_get_aware( o ); assert( p->on_termination ); return p->on_termination( o, frame );} \
  static inline bl_t bcore_main_defines_on_termination( const bcore_main* o ){ return  true;} \
  static inline bl_t bcore_main_r_on_termination( const sr_s* o, const bcore_main_frame_s* frame ){ ASSERT( !sr_s_is_const( o ) ); const bcore_main_spect_s* p = (const bcore_main_spect_s*)ch_spect_p( o->p, TYPEOF_bcore_main_spect_s ); assert( p->on_termination ); return p->on_termination( o->o, frame );} \
  static inline bl_t bcore_main_r_defines_on_termination( const sr_s* o ){ return  true;} \
  static inline bl_t bcore_main_on_termination_default( bcore_main* o, const bcore_main_frame_s* frame ){return  false;} \
  static inline bl_t bcore_main_a_on_interrupt( bcore_main* o, const bcore_main_frame_s* frame ){ const bcore_main_spect_s* p = bcore_main_spect_s_get_aware( o ); assert( p->on_interrupt ); return p->on_interrupt( o, frame );} \
  static inline bl_t bcore_main_defines_on_interrupt( const bcore_main* o ){ return  true;} \
  static inline bl_t bcore_main_r_on_interrupt( const sr_s* o, const bcore_main_frame_s* frame ){ ASSERT( !sr_s_is_const( o ) ); const bcore_main_spect_s* p = (const bcore_main_spect_s*)ch_spect_p( o->p, TYPEOF_bcore_main_spect_s ); assert( p->on_interrupt ); return p->on_interrupt( o->o, frame );} \
  static inline bl_t bcore_main_r_defines_on_interrupt( const sr_s* o ){ return  true;} \
  static inline bl_t bcore_main_on_interrupt_default( bcore_main* o, const bcore_main_frame_s* frame ){return  false;} \
  static inline bl_t bcore_main_a_on_suspend( bcore_main* o, const bcore_main_frame_s* frame ){ const bcore_main_spect_s* p = bcore_main_spect_s_get_aware( o ); assert( p->on_suspend ); return p->on_suspend( o, frame );} \
  static inline bl_t bcore_main_defines_on_suspend( const bcore_main* o ){ return  true;} \
  static inline bl_t bcore_main_r_on_suspend( const sr_s* o, const bcore_main_frame_s* frame ){ ASSERT( !sr_s_is_const( o ) ); const bcore_main_spect_s* p = (const bcore_main_spect_s*)ch_spect_p( o->p, TYPEOF_bcore_main_spect_s ); assert( p->on_suspend ); return p->on_suspend( o->o, frame );} \
  static inline bl_t bcore_main_r_defines_on_suspend( const sr_s* o ){ return  true;} \
  static inline bl_t bcore_main_on_suspend_default( bcore_main* o, const bcore_main_frame_s* frame ){return  false;} \
  BETH_EXPAND_ITEM_bcore_main_arr_s \
  BETH_EXPAND_ITEM_bcore_main_set_s

/**********************************************************************************************************************/
// source: bcore_hmap_name.h

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_hmap_name

#define TYPEOF_bcore_hmap_name 0x5A5AB89E5E2CAF65ull
#define TYPEOF_bcore_hmap_name_spect_s 0x270DB675D008A881ull
#define TYPEOF_bcore_hmap_name_s 0x36F8463812FD3AE7ull
#define BETH_EXPAND_ITEM_bcore_hmap_name_s \
  BCORE_DECLARE_OBJECT( bcore_hmap_name_s ) \
  { \
      aware_t _; \
      bcore_hmap_tp_sr_s map; \
  }; \
  tp_t bcore_hmap_name_s_set_st_c( bcore_hmap_name_s* o, const st_s* st ); \
  tp_t bcore_hmap_name_s_set_st_d( bcore_hmap_name_s* o, st_s* st ); \
  tp_t bcore_hmap_name_s_set_sc( bcore_hmap_name_s* o, sc_t sc ); \
  st_s* bcore_hmap_name_s_get( const bcore_hmap_name_s* o, tp_t key ); \
  sc_t bcore_hmap_name_s_get_sc( const bcore_hmap_name_s* o, tp_t key ); \
  void bcore_hmap_name_s_remove( bcore_hmap_name_s* o, tp_t key ); \
  bl_t bcore_hmap_name_s_exists( const bcore_hmap_name_s* o, tp_t key ); \
  void bcore_hmap_name_s_clear( bcore_hmap_name_s* o );
#define BETH_EXPAND_GROUP_bcore_hmap_name \
  BCORE_FORWARD_OBJECT( bcore_hmap_name ); \
  BCORE_FORWARD_OBJECT( bcore_hmap_name_s ); \
  XOILA_DECLARE_SPECT( bcore_hmap_name ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bcore_hmap_name ) \
  BETH_EXPAND_ITEM_bcore_hmap_name_s

/**********************************************************************************************************************/
// source: bcore_hmap_tp_st.h

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_hmap_tp_st

#define TYPEOF_bcore_hmap_tp_st 0xFF55493843A2D5E8ull
#define TYPEOF_bcore_hmap_tp_st_spect_s 0x2CACD5A0AC8D6330ull
#define TYPEOF_bcore_hmap_tp_st_s 0x7951A041E402C2B2ull
#define BETH_EXPAND_ITEM_bcore_hmap_tp_st_s \
  BCORE_DECLARE_OBJECT( bcore_hmap_tp_st_s ) \
  { \
      aware_t _; \
      bcore_hmap_tpaw_s map; \
  }; \
  static inline const st_s* bcore_hmap_tp_st_s_get( const bcore_hmap_tp_st_s* o, tp_t key ); \
  static inline st_s* bcore_hmap_tp_st_s_set_d( bcore_hmap_tp_st_s* o, tp_t key, st_s* val ); \
  static inline st_s* bcore_hmap_tp_st_s_set( bcore_hmap_tp_st_s* o, tp_t key, const st_s* val ); \
  static inline st_s* bcore_hmap_tp_st_s_set_sc( bcore_hmap_tp_st_s* o, tp_t key, sc_t val ); \
  static inline void bcore_hmap_tp_st_s_remove( bcore_hmap_tp_st_s* o, tp_t key ); \
  static inline bl_t bcore_hmap_tp_st_s_exists( const bcore_hmap_tp_st_s* o, tp_t key ); \
  static inline void bcore_hmap_tp_st_s_clear( bcore_hmap_tp_st_s* o ); \
  static inline sz_t bcore_hmap_tp_st_s_keys( const bcore_hmap_tp_st_s* o ); \
  static inline sz_t bcore_hmap_tp_st_s_size( const bcore_hmap_tp_st_s* o ); \
  static inline tp_t bcore_hmap_tp_st_s_idx_key( const bcore_hmap_tp_st_s* o, sz_t idx ); \
  static inline const st_s* bcore_hmap_tp_st_s_idx_val( const bcore_hmap_tp_st_s* o, sz_t idx ); \
  static inline const st_s* bcore_hmap_tp_st_s_get( const bcore_hmap_tp_st_s* o, tp_t key ){ const st_s** p_st = ((const st_s**)(bcore_hmap_tpaw_s_get(&(o->map),key ))); return  p_st ? (*(p_st)) : NULL;} \
  static inline st_s* bcore_hmap_tp_st_s_set_d( bcore_hmap_tp_st_s* o, tp_t key, st_s* val ){ return  ((st_s*)(bcore_hmap_tpaw_s_set_d(&(o->map),key,((x_inst*)( val )))));} \
  static inline st_s* bcore_hmap_tp_st_s_set( bcore_hmap_tp_st_s* o, tp_t key, const st_s* val ){ return  bcore_hmap_tp_st_s_set_d(o,key, st_s_clone(val) );} \
  static inline st_s* bcore_hmap_tp_st_s_set_sc( bcore_hmap_tp_st_s* o, tp_t key, sc_t val ){ return  bcore_hmap_tp_st_s_set_d(o,key, st_s_create_sc(val ) );} \
  static inline void bcore_hmap_tp_st_s_remove( bcore_hmap_tp_st_s* o, tp_t key ){ bcore_hmap_tpaw_s_remove(&(o->map),key );} \
  static inline bl_t bcore_hmap_tp_st_s_exists( const bcore_hmap_tp_st_s* o, tp_t key ){ return  bcore_hmap_tpaw_s_exists(&(o->map),key );} \
  static inline void bcore_hmap_tp_st_s_clear( bcore_hmap_tp_st_s* o ){ bcore_hmap_tpaw_s_clear(&(o->map));} \
  static inline sz_t bcore_hmap_tp_st_s_keys( const bcore_hmap_tp_st_s* o ){ return  bcore_hmap_tpaw_s_keys(&(o->map));} \
  static inline sz_t bcore_hmap_tp_st_s_size( const bcore_hmap_tp_st_s* o ){ return  bcore_hmap_tpaw_s_size(&(o->map));} \
  static inline tp_t bcore_hmap_tp_st_s_idx_key( const bcore_hmap_tp_st_s* o, sz_t idx ){ return  bcore_hmap_tpaw_s_idx_key(&(o->map),idx );} \
  static inline const st_s* bcore_hmap_tp_st_s_idx_val( const bcore_hmap_tp_st_s* o, sz_t idx ){ return  ((const st_s*)(bcore_hmap_tpaw_s_idx_val(&(o->map),idx )));}
#define BETH_EXPAND_GROUP_bcore_hmap_tp_st \
  BCORE_FORWARD_OBJECT( bcore_hmap_tp_st ); \
  BCORE_FORWARD_OBJECT( bcore_hmap_tp_st_s ); \
  void bcore_hmap_tp_st_selftest( void ); \
  XOILA_DECLARE_SPECT( bcore_hmap_tp_st ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bcore_hmap_tp_st ) \
  BETH_EXPAND_ITEM_bcore_hmap_tp_st_s

/**********************************************************************************************************************/
// source: bcore_cday.h

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_cday

#define TYPEOF_bcore_cday 0x0719B44A6841BD9Aull
#define TYPEOF_bcore_cday_spect_s 0x9D4C34C843299546ull
#define TYPEOF_bcore_cday_ymd_s 0x0925C4B71AD1263Dull
#define BETH_EXPAND_ITEM_bcore_cday_ymd_s \
  BCORE_DECLARE_OBJECT( bcore_cday_ymd_s ) \
  { \
      aware_t _; \
      s2_t y; \
      s2_t m; \
      s2_t d; \
  }; \
  void bcore_cday_ymd_s_check_plausibility( const bcore_cday_ymd_s* o ); \
  er_t bcore_cday_ymd_s_em_check_plausibility( const bcore_cday_ymd_s* o ); \
  er_t bcore_cday_ymd_s_em_from_cday( bcore_cday_ymd_s* o, s2_t cday ); \
  er_t bcore_cday_ymd_s_em_from_source( bcore_cday_ymd_s* o, x_source* source ); \
  er_t bcore_cday_ymd_s_em_from_sc( bcore_cday_ymd_s* o, sc_t sc ); \
  static inline er_t bcore_cday_ymd_s_em_from_string( bcore_cday_ymd_s* o, const st_s* st ); \
  bcore_cday_ymd_s* bcore_cday_ymd_s_from_cday( bcore_cday_ymd_s* o, s2_t cday ); \
  bcore_cday_ymd_s* bcore_cday_ymd_s_from_source( bcore_cday_ymd_s* o, x_source* source ); \
  bcore_cday_ymd_s* bcore_cday_ymd_s_from_sc( bcore_cday_ymd_s* o, sc_t sc ); \
  static inline bcore_cday_ymd_s* bcore_cday_ymd_s_from_string( bcore_cday_ymd_s* o, const st_s* st ); \
  static inline bcore_cday_ymd_s* bcore_cday_ymd_s_create_from_cday( s2_t cday ); \
  static inline const bcore_cday_ymd_s* bcore_cday_ymd_s_to_sink( const bcore_cday_ymd_s* o, bcore_sink* sink ); \
  static inline const bcore_cday_ymd_s* bcore_cday_ymd_s_to_string( const bcore_cday_ymd_s* o, st_s* st ); \
  static inline const bcore_cday_ymd_s* bcore_cday_ymd_s_push_to_string( const bcore_cday_ymd_s* o, st_s* st ); \
  s2_t bcore_cday_ymd_s_to_cday( const bcore_cday_ymd_s* o ); \
  s2_t bcore_cday_ymd_s_to_wnum( const bcore_cday_ymd_s* o ); \
  static inline er_t bcore_cday_ymd_s_em_from_string( bcore_cday_ymd_s* o, const st_s* st ){return  bcore_cday_ymd_s_em_from_sc(o,st->sc );} \
  static inline bcore_cday_ymd_s* bcore_cday_ymd_s_from_string( bcore_cday_ymd_s* o, const st_s* st ){return  bcore_cday_ymd_s_from_sc(o,st->sc );} \
  static inline bcore_cday_ymd_s* bcore_cday_ymd_s_create_from_cday( s2_t cday ){return  bcore_cday_ymd_s_from_cday(bcore_cday_ymd_s_create(),cday );} \
  static inline const bcore_cday_ymd_s* bcore_cday_ymd_s_to_sink( const bcore_cday_ymd_s* o, bcore_sink* sink ){bcore_sink_a_push_fa(sink,"#pl4'0'{#<s2_t>}-#pl2'0'{#<s2_t>}-#pl2'0'{#<s2_t>}", o->y, o->m, o->d );return o;} \
  static inline const bcore_cday_ymd_s* bcore_cday_ymd_s_to_string( const bcore_cday_ymd_s* o, st_s* st ){ st_s_clear(st); return  bcore_cday_ymd_s_to_sink(o,((bcore_sink*)(st )));} \
  static inline const bcore_cday_ymd_s* bcore_cday_ymd_s_push_to_string( const bcore_cday_ymd_s* o, st_s* st ){return  bcore_cday_ymd_s_to_sink(o,((bcore_sink*)(st )));}
#define TYPEOF_bcore_cday_utc_s 0xB3FF29791CCF8201ull
#define BETH_EXPAND_ITEM_bcore_cday_utc_s \
  BCORE_DECLARE_OBJECT( bcore_cday_utc_s ) \
  { \
      aware_t _; \
      s2_t cday; \
      s2_t ms; \
  }; \
  bcore_cday_utc_s* bcore_cday_utc_s_from_system( bcore_cday_utc_s* o ); \
  bcore_cday_utc_s* bcore_cday_utc_s_normalize( bcore_cday_utc_s* o ); \
  const bcore_cday_utc_s* bcore_cday_utc_s_to_sink( const bcore_cday_utc_s* o, bcore_sink* sink ); \
  static inline const bcore_cday_utc_s* bcore_cday_utc_s_to_string( const bcore_cday_utc_s* o, st_s* st ); \
  static inline const bcore_cday_utc_s* bcore_cday_utc_s_push_to_string( const bcore_cday_utc_s* o, st_s* st ); \
  bcore_cday_utc_s* bcore_cday_utc_s_from_source( bcore_cday_utc_s* o, x_source* source ); \
  bcore_cday_utc_s* bcore_cday_utc_s_from_sc( bcore_cday_utc_s* o, sc_t sc ); \
  static inline bcore_cday_utc_s* bcore_cday_utc_s_from_string( bcore_cday_utc_s* o, const st_s* st ); \
  s3_t bcore_cday_utc_s_to_ms( const bcore_cday_utc_s* o ); \
  s3_t bcore_cday_utc_s_diff_ms( const bcore_cday_utc_s* o, const bcore_cday_utc_s* b ); \
  static inline const bcore_cday_utc_s* bcore_cday_utc_s_to_string( const bcore_cday_utc_s* o, st_s* st ){ st_s_clear(st); return  bcore_cday_utc_s_to_sink(o,((bcore_sink*)(st )));} \
  static inline const bcore_cday_utc_s* bcore_cday_utc_s_push_to_string( const bcore_cday_utc_s* o, st_s* st ){return  bcore_cday_utc_s_to_sink(o,((bcore_sink*)(st )));} \
  static inline bcore_cday_utc_s* bcore_cday_utc_s_from_string( bcore_cday_utc_s* o, const st_s* st ){return  bcore_cday_utc_s_from_sc(o,st->sc );}
#define BETH_EXPAND_GROUP_bcore_cday \
  BCORE_FORWARD_OBJECT( bcore_cday ); \
  BCORE_FORWARD_OBJECT( bcore_cday_ymd_s ); \
  BCORE_FORWARD_OBJECT( bcore_cday_utc_s ); \
  static inline s2_t bcore_cday_of_epoch( void ); \
  s2_t bcore_cday_to_wnum( s2_t cday ); \
  s2_t bcore_cday_to_wday( s2_t cday ); \
  sc_t bcore_cday_wday_to_sc( s2_t wday ); \
  void bcore_cday_to_sink( s2_t cday, bcore_sink* sink ); \
  void bcore_cday_to_string( s2_t cday, st_s* st ); \
  void bcore_cday_push_to_string( s2_t cday, st_s* st ); \
  s2_t bcore_cday_from_source( x_source* source ); \
  s2_t bcore_cday_from_sc( sc_t sc ); \
  static inline s2_t bcore_cday_from_string( const st_s* st ); \
  XOILA_DECLARE_SPECT( bcore_cday ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bcore_cday ) \
  BETH_EXPAND_ITEM_bcore_cday_ymd_s \
  BETH_EXPAND_ITEM_bcore_cday_utc_s \
  static inline s2_t bcore_cday_of_epoch( void ){return  25508;} \
  static inline s2_t bcore_cday_from_string( const st_s* st ){return  bcore_cday_from_sc(st->sc );}

/**********************************************************************************************************************/
// source: bcore_error_manager.h

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_error

#define TYPEOF_bcore_error 0x5DEDBC961043059Bull
#define TYPEOF_bcore_error_spect_s 0xFDA5542D4756953Bull
#define TYPEOF_general_error 0x4D9E984C5B21FAEAull
#define TYPEOF_conversion_error 0xB1970E197C3EC6D8ull
#define TYPEOF_parse_error 0x690AF8765815EC4Dull
#define TYPEOF_plant_error 0x6C62A00462378607ull
#define TYPEOF_error_stack 0xF5EEC6731159BABAull
#define TYPEOF_assert_error 0xD4AB5E42E24FDB7Aull
#define BETH_EXPAND_GROUP_bcore_error \
  BCORE_FORWARD_OBJECT( bcore_error ); \
  BCORE_FORWARD_OBJECT( bcore_error_manager ); \
  sz_t bcore_error_stack_size( void ); \
  void bcore_error_clear_stack( void ); \
  void bcore_error_remove_last( void ); \
  void bcore_error_remove_last_if_of( er_t id ); \
  er_t bcore_error_push_sc( er_t id, sc_t msg ); \
  er_t bcore_error_push_fv( er_t id, sc_t format, va_list args ); \
  er_t bcore_error_push_fa( er_t id, sc_t format, ... ); \
  er_t bcore_error_push_ffl_fv( er_t id, sc_t func, sc_t file, sz_t line, sc_t format, va_list args ); \
  er_t bcore_error_push_ffl_fa( er_t id, sc_t func, sc_t file, sz_t line, sc_t format, ... ); \
  er_t bcore_error_push_gffl_fv( sc_t func, sc_t file, sz_t line, sc_t format, va_list args ); \
  er_t bcore_error_push_gffl_fa( sc_t func, sc_t file, sz_t line, sc_t format, ... ); \
  er_t bcore_error_last( void ); \
  bl_t bcore_error_pop_st( er_t* id, st_s* msg ); \
  bl_t bcore_error_pop_to_sink( bcore_sink* sink ); \
  bl_t bcore_error_pop_to_stderr( void ); \
  bl_t bcore_error_pop_all_to_sink( bcore_sink* sink ); \
  bl_t bcore_error_pop_all_to_stderr( void ); \
  bl_t bcore_error_pop_all( void ); \
  static inline st_s* bcore_error_pop_all_to_st( st_s* st ); \
  XOILA_DECLARE_SPECT( bcore_error ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bcore_error ) \
  BETH_EXPAND_GROUP_bcore_error_manager \
  static inline st_s* bcore_error_pop_all_to_st( st_s* st ){ bcore_error_pop_all_to_sink(((bcore_sink*)(st ))); return  st;}

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_error_manager

#define TYPEOF_bcore_error_manager 0x31CBDF7C3004C3B3ull
#define TYPEOF_bcore_error_manager_spect_s 0xF275D9EF563C9093ull
#define TYPEOF_bcore_error_manager_error_s 0x98F2D6C747A7D80Cull
#define BETH_EXPAND_ITEM_bcore_error_manager_error_s \
  BCORE_DECLARE_OBJECT( bcore_error_manager_error_s ) \
  { \
      aware_t _; \
      er_t id; \
      st_s msg; \
  };
#define TYPEOF_bcore_error_manager_error_adl_s 0x5D87C559ED44DAC4ull
#define BETH_EXPAND_ITEM_bcore_error_manager_error_adl_s \
  BCORE_DECLARE_OBJECT( bcore_error_manager_error_adl_s ) \
  { \
      aware_t _; \
      BCORE_ARRAY_DYN_LINK_STATIC_S( bcore_error_manager_error_s, ); \
  }; \
  bcore_error_manager_error_adl_s* bcore_error_manager_error_adl_s_set_size( bcore_error_manager_error_adl_s* o, sz_t size ); \
  bcore_error_manager_error_adl_s* bcore_error_manager_error_adl_s_clear( bcore_error_manager_error_adl_s* o ); \
  bcore_error_manager_error_s* bcore_error_manager_error_adl_s_push_d( bcore_error_manager_error_adl_s* o, bcore_error_manager_error_s* v );
#define TYPEOF_bcore_error_manager_context_s 0x9EF599CE2C2E59C1ull
#define BETH_EXPAND_ITEM_bcore_error_manager_context_s \
  BCORE_DECLARE_OBJECT( bcore_error_manager_context_s ) \
  { \
      aware_t _; \
      bcore_error_manager_error_adl_s adl; \
      bcore_mutex_s mutex; \
  };
#define BETH_EXPAND_GROUP_bcore_error_manager \
  BCORE_FORWARD_OBJECT( bcore_error_manager ); \
  BCORE_FORWARD_OBJECT( bcore_error_manager_error_s ); \
  BCORE_FORWARD_OBJECT( bcore_error_manager_error_adl_s ); \
  BCORE_FORWARD_OBJECT( bcore_error_manager_context_s ); \
  XOILA_DECLARE_SPECT( bcore_error_manager ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bcore_error_manager ) \
  BETH_EXPAND_ITEM_bcore_error_manager_error_s \
  BETH_EXPAND_ITEM_bcore_error_manager_error_adl_s \
  BETH_EXPAND_ITEM_bcore_error_manager_context_s

/**********************************************************************************************************************/
// source: bcore_prsg.h

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_prsg

#define TYPEOF_bcore_prsg 0x74C295DEE84EE713ull
#define TYPEOF_bcore_prsg_spect_s 0x1832833774409C73ull
#define BETH_EXPAND_GROUP_bcore_prsg \
  BCORE_FORWARD_OBJECT( bcore_prsg ); \
  BCORE_FORWARD_OBJECT( bcore_prsg_lcg ); \
  BCORE_FORWARD_OBJECT( bcore_prsg_xsg ); \
  typedef sz_t (*bcore_prsg_bits)(const bcore_prsg* o ); \
  typedef u3_t (*bcore_prsg_max_u3)(const bcore_prsg* o ); \
  typedef u3_t (*bcore_prsg_min_u3)(const bcore_prsg* o ); \
  typedef u3_t (*bcore_prsg_state_u3)(const bcore_prsg* o ); \
  typedef u3_t (*bcore_prsg_state_bits_u3)(const bcore_prsg* o, sz_t bits ); \
  typedef f3_t (*bcore_prsg_state_f3)(const bcore_prsg* o, f3_t min, f3_t max ); \
  typedef bl_t (*bcore_prsg_state_bl)(const bcore_prsg* o ); \
  typedef bcore_prsg* (*bcore_prsg_gen)(bcore_prsg* o ); \
  typedef u3_t (*bcore_prsg_gen_bits_u3)(bcore_prsg* o, sz_t bits ); \
  typedef u3_t (*bcore_prsg_gen_u3)(bcore_prsg* o ); \
  typedef f3_t (*bcore_prsg_gen_f3)(bcore_prsg* o, f3_t min, f3_t max ); \
  typedef bl_t (*bcore_prsg_gen_bl)(bcore_prsg* o ); \
  typedef bcore_prsg* (*bcore_prsg_set_state_u3)(bcore_prsg* o, u3_t seed ); \
  typedef bcore_prsg* (*bcore_prsg_set_state_f3)(bcore_prsg* o, f3_t seed ); \
  typedef bcore_prsg* (*bcore_prsg_set_state_mix)(bcore_prsg* o, const bcore_prsg* a, const bcore_prsg* b ); \
  typedef bcore_prsg* (*bcore_prsg_reseed)(bcore_prsg* o, u3_t seed ); \
  XOILA_DECLARE_SPECT( bcore_prsg ) \
  { \
      bcore_spect_header_s header; \
      bcore_prsg_bits bits; \
      bcore_prsg_max_u3 max_u3; \
      bcore_prsg_min_u3 min_u3; \
      bcore_prsg_state_u3 state_u3; \
      bcore_prsg_state_bits_u3 state_bits_u3; \
      bcore_prsg_state_f3 state_f3; \
      bcore_prsg_state_bl state_bl; \
      bcore_prsg_gen gen; \
      bcore_prsg_gen_bits_u3 gen_bits_u3; \
      bcore_prsg_gen_u3 gen_u3; \
      bcore_prsg_gen_f3 gen_f3; \
      bcore_prsg_gen_bl gen_bl; \
      bcore_prsg_set_state_u3 set_state_u3; \
      bcore_prsg_set_state_f3 set_state_f3; \
      bcore_prsg_set_state_mix set_state_mix; \
      bcore_prsg_reseed reseed; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bcore_prsg ) \
  static inline sz_t bcore_prsg_a_bits( const bcore_prsg* o ){ const bcore_prsg_spect_s* p = bcore_prsg_spect_s_get_aware( o ); assert( p->bits ); return p->bits( o );} \
  static inline bl_t bcore_prsg_defines_bits( const bcore_prsg* o ){ return  true;} \
  static inline u3_t bcore_prsg_a_max_u3( const bcore_prsg* o ){ const bcore_prsg_spect_s* p = bcore_prsg_spect_s_get_aware( o ); assert( p->max_u3 ); return p->max_u3( o );} \
  static inline bl_t bcore_prsg_defines_max_u3( const bcore_prsg* o ){ return  true;} \
  static inline u3_t bcore_prsg_a_min_u3( const bcore_prsg* o ){ const bcore_prsg_spect_s* p = bcore_prsg_spect_s_get_aware( o ); assert( p->min_u3 ); return p->min_u3( o );} \
  static inline bl_t bcore_prsg_defines_min_u3( const bcore_prsg* o ){ return  true;} \
  static inline u3_t bcore_prsg_a_state_u3( const bcore_prsg* o ){ const bcore_prsg_spect_s* p = bcore_prsg_spect_s_get_aware( o ); assert( p->state_u3 ); return p->state_u3( o );} \
  static inline bl_t bcore_prsg_defines_state_u3( const bcore_prsg* o ){ return  true;} \
  static inline u3_t bcore_prsg_a_state_bits_u3( const bcore_prsg* o, sz_t bits ){ const bcore_prsg_spect_s* p = bcore_prsg_spect_s_get_aware( o ); assert( p->state_bits_u3 ); return p->state_bits_u3( o, bits );} \
  static inline bl_t bcore_prsg_defines_state_bits_u3( const bcore_prsg* o ){ return  true;} \
  static inline f3_t bcore_prsg_a_state_f3( const bcore_prsg* o, f3_t min, f3_t max ){ const bcore_prsg_spect_s* p = bcore_prsg_spect_s_get_aware( o ); assert( p->state_f3 ); return p->state_f3( o, min, max );} \
  static inline bl_t bcore_prsg_defines_state_f3( const bcore_prsg* o ){ return  true;} \
  static inline bl_t bcore_prsg_a_state_bl( const bcore_prsg* o ){ const bcore_prsg_spect_s* p = bcore_prsg_spect_s_get_aware( o ); assert( p->state_bl ); return p->state_bl( o );} \
  static inline bl_t bcore_prsg_defines_state_bl( const bcore_prsg* o ){ return  true;} \
  static inline bl_t bcore_prsg_state_bl_default( const bcore_prsg* o ){ return  bcore_prsg_a_state_bits_u3(o,1 ) ? true : false;} \
  static inline bcore_prsg* bcore_prsg_a_gen( bcore_prsg* o ){ const bcore_prsg_spect_s* p = bcore_prsg_spect_s_get_aware( o ); assert( p->gen ); return p->gen( o );} \
  static inline bl_t bcore_prsg_defines_gen( const bcore_prsg* o ){ return  true;} \
  static inline u3_t bcore_prsg_a_gen_bits_u3( bcore_prsg* o, sz_t bits ){ const bcore_prsg_spect_s* p = bcore_prsg_spect_s_get_aware( o ); assert( p->gen_bits_u3 ); return p->gen_bits_u3( o, bits );} \
  static inline bl_t bcore_prsg_defines_gen_bits_u3( const bcore_prsg* o ){ return  true;} \
  static inline u3_t bcore_prsg_a_gen_u3( bcore_prsg* o ){ const bcore_prsg_spect_s* p = bcore_prsg_spect_s_get_aware( o ); assert( p->gen_u3 ); return p->gen_u3( o );} \
  static inline bl_t bcore_prsg_defines_gen_u3( const bcore_prsg* o ){ return  true;} \
  static inline f3_t bcore_prsg_a_gen_f3( bcore_prsg* o, f3_t min, f3_t max ){ const bcore_prsg_spect_s* p = bcore_prsg_spect_s_get_aware( o ); assert( p->gen_f3 ); return p->gen_f3( o, min, max );} \
  static inline bl_t bcore_prsg_defines_gen_f3( const bcore_prsg* o ){ return  true;} \
  static inline bl_t bcore_prsg_a_gen_bl( bcore_prsg* o ){ const bcore_prsg_spect_s* p = bcore_prsg_spect_s_get_aware( o ); assert( p->gen_bl ); return p->gen_bl( o );} \
  static inline bl_t bcore_prsg_defines_gen_bl( const bcore_prsg* o ){ return  true;} \
  static inline bl_t bcore_prsg_gen_bl_default( bcore_prsg* o ){ return  bcore_prsg_a_gen_bits_u3(o,1 ) ? true : false;} \
  static inline bcore_prsg* bcore_prsg_a_set_state_u3( bcore_prsg* o, u3_t seed ){ const bcore_prsg_spect_s* p = bcore_prsg_spect_s_get_aware( o ); assert( p->set_state_u3 ); return p->set_state_u3( o, seed );} \
  static inline bl_t bcore_prsg_defines_set_state_u3( const bcore_prsg* o ){ return  true;} \
  static inline bcore_prsg* bcore_prsg_a_set_state_f3( bcore_prsg* o, f3_t seed ){ const bcore_prsg_spect_s* p = bcore_prsg_spect_s_get_aware( o ); assert( p->set_state_f3 ); return p->set_state_f3( o, seed );} \
  static inline bl_t bcore_prsg_defines_set_state_f3( const bcore_prsg* o ){ return  true;} \
  bcore_prsg* bcore_prsg_set_state_f3_default( bcore_prsg* o, f3_t seed ); \
  static inline bcore_prsg* bcore_prsg_a_set_state_mix( bcore_prsg* o, const bcore_prsg* a, const bcore_prsg* b ){ const bcore_prsg_spect_s* p = bcore_prsg_spect_s_get_aware( o ); assert( p->set_state_mix ); return p->set_state_mix( o, a, b );} \
  static inline bl_t bcore_prsg_defines_set_state_mix( const bcore_prsg* o ){ return  true;} \
  static inline bcore_prsg* bcore_prsg_set_state_mix_default( bcore_prsg* o, const bcore_prsg* a, const bcore_prsg* b ){return  bcore_prsg_a_set_state_u3(o,bcore_prsg_a_state_u3(a) + bcore_prsg_a_state_u3(b) );} \
  static inline bcore_prsg* bcore_prsg_a_reseed( bcore_prsg* o, u3_t seed ){ const bcore_prsg_spect_s* p = bcore_prsg_spect_s_get_aware( o ); assert( p->reseed ); return p->reseed( o, seed );} \
  static inline bl_t bcore_prsg_defines_reseed( const bcore_prsg* o ){ return  true;} \
  static inline bcore_prsg* bcore_prsg_reseed_default( bcore_prsg* o, u3_t seed ){return  bcore_prsg_a_set_state_u3(o,seed );} \
  BETH_EXPAND_GROUP_bcore_prsg_lcg \
  BETH_EXPAND_GROUP_bcore_prsg_xsg

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_prsg_lcg

#define TYPEOF_bcore_prsg_lcg 0x83001D45A194B892ull
#define TYPEOF_bcore_prsg_lcg_spect_s 0x9DF599FE4A06997Eull
#define TYPEOF_bcore_prsg_lcg_u2_00_s 0xEDC5E07658932AEBull
#define BETH_EXPAND_ITEM_bcore_prsg_lcg_u2_00_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_lcg_u2_00_s ) \
  { \
      aware_t _; \
      u2_t state; \
  }; \
  static inline u3_t bcore_prsg_lcg_u2_00_s_gen_u3( bcore_prsg_lcg_u2_00_s* o ); \
  u3_t bcore_prsg_lcg_u2_00_s_state_bits_u3( const bcore_prsg_lcg_u2_00_s* o, sz_t bits ); \
  static inline u3_t bcore_prsg_lcg_u2_00_s_gen_bits_u3( bcore_prsg_lcg_u2_00_s* o, sz_t bits ); \
  static inline f3_t bcore_prsg_lcg_u2_00_s_state_f3( const bcore_prsg_lcg_u2_00_s* o, f3_t min, f3_t max ); \
  static inline f3_t bcore_prsg_lcg_u2_00_s_gen_f3( bcore_prsg_lcg_u2_00_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_lcg_u2_00_s_bits( const bcore_prsg_lcg_u2_00_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_00_s_max_u3( const bcore_prsg_lcg_u2_00_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_00_s_min_u3( const bcore_prsg_lcg_u2_00_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_00_s_state_u3( const bcore_prsg_lcg_u2_00_s* o ); \
  static inline bcore_prsg_lcg_u2_00_s* bcore_prsg_lcg_u2_00_s_set_state_u3( bcore_prsg_lcg_u2_00_s* o, u3_t seed ); \
  static inline bcore_prsg_lcg_u2_00_s* bcore_prsg_lcg_u2_00_s_gen( bcore_prsg_lcg_u2_00_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_00_s_gen_u3( bcore_prsg_lcg_u2_00_s* o ){return  bcore_prsg_lcg_u2_00_s_state_u3(bcore_prsg_lcg_u2_00_s_gen(o));} \
  static inline u3_t bcore_prsg_lcg_u2_00_s_gen_bits_u3( bcore_prsg_lcg_u2_00_s* o, sz_t bits ){return  bcore_prsg_lcg_u2_00_s_state_bits_u3(bcore_prsg_lcg_u2_00_s_gen(o),bits );} \
  static inline f3_t bcore_prsg_lcg_u2_00_s_state_f3( const bcore_prsg_lcg_u2_00_s* o, f3_t min, f3_t max ){return  min + ( max - min ) * ( ( f3_t )( bcore_prsg_lcg_u2_00_s_state_u3(o) - bcore_prsg_lcg_u2_00_s_min_u3(o) ) / ( bcore_prsg_lcg_u2_00_s_max_u3(o) - bcore_prsg_lcg_u2_00_s_min_u3(o) ) );} \
  static inline f3_t bcore_prsg_lcg_u2_00_s_gen_f3( bcore_prsg_lcg_u2_00_s* o, f3_t min, f3_t max ){return  bcore_prsg_lcg_u2_00_s_state_f3(bcore_prsg_lcg_u2_00_s_gen(o),min, max );} \
  static inline sz_t bcore_prsg_lcg_u2_00_s_bits( const bcore_prsg_lcg_u2_00_s* o ){return  32;} \
  static inline u3_t bcore_prsg_lcg_u2_00_s_max_u3( const bcore_prsg_lcg_u2_00_s* o ){return  0xFFFFFFFFu;} \
  static inline u3_t bcore_prsg_lcg_u2_00_s_min_u3( const bcore_prsg_lcg_u2_00_s* o ){return  0;} \
  static inline u3_t bcore_prsg_lcg_u2_00_s_state_u3( const bcore_prsg_lcg_u2_00_s* o ){return  o->state;} \
  static inline bcore_prsg_lcg_u2_00_s* bcore_prsg_lcg_u2_00_s_set_state_u3( bcore_prsg_lcg_u2_00_s* o, u3_t seed ){o->state = seed ^ ( seed >> 32 );return o;} \
  static inline bcore_prsg_lcg_u2_00_s* bcore_prsg_lcg_u2_00_s_gen( bcore_prsg_lcg_u2_00_s* o ){o->state = bcore_lcg00_u2( o->state );return o;}
#define TYPEOF_bcore_prsg_lcg_u2_01_s 0xE310A77651EDDF10ull
#define BETH_EXPAND_ITEM_bcore_prsg_lcg_u2_01_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_lcg_u2_01_s ) \
  { \
      aware_t _; \
      u2_t state; \
  }; \
  static inline u3_t bcore_prsg_lcg_u2_01_s_gen_u3( bcore_prsg_lcg_u2_01_s* o ); \
  u3_t bcore_prsg_lcg_u2_01_s_state_bits_u3( const bcore_prsg_lcg_u2_01_s* o, sz_t bits ); \
  static inline u3_t bcore_prsg_lcg_u2_01_s_gen_bits_u3( bcore_prsg_lcg_u2_01_s* o, sz_t bits ); \
  static inline f3_t bcore_prsg_lcg_u2_01_s_state_f3( const bcore_prsg_lcg_u2_01_s* o, f3_t min, f3_t max ); \
  static inline f3_t bcore_prsg_lcg_u2_01_s_gen_f3( bcore_prsg_lcg_u2_01_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_lcg_u2_01_s_bits( const bcore_prsg_lcg_u2_01_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_01_s_max_u3( const bcore_prsg_lcg_u2_01_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_01_s_min_u3( const bcore_prsg_lcg_u2_01_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_01_s_state_u3( const bcore_prsg_lcg_u2_01_s* o ); \
  static inline bcore_prsg_lcg_u2_01_s* bcore_prsg_lcg_u2_01_s_set_state_u3( bcore_prsg_lcg_u2_01_s* o, u3_t seed ); \
  static inline bcore_prsg_lcg_u2_01_s* bcore_prsg_lcg_u2_01_s_gen( bcore_prsg_lcg_u2_01_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_01_s_gen_u3( bcore_prsg_lcg_u2_01_s* o ){return  bcore_prsg_lcg_u2_01_s_state_u3(bcore_prsg_lcg_u2_01_s_gen(o));} \
  static inline u3_t bcore_prsg_lcg_u2_01_s_gen_bits_u3( bcore_prsg_lcg_u2_01_s* o, sz_t bits ){return  bcore_prsg_lcg_u2_01_s_state_bits_u3(bcore_prsg_lcg_u2_01_s_gen(o),bits );} \
  static inline f3_t bcore_prsg_lcg_u2_01_s_state_f3( const bcore_prsg_lcg_u2_01_s* o, f3_t min, f3_t max ){return  min + ( max - min ) * ( ( f3_t )( bcore_prsg_lcg_u2_01_s_state_u3(o) - bcore_prsg_lcg_u2_01_s_min_u3(o) ) / ( bcore_prsg_lcg_u2_01_s_max_u3(o) - bcore_prsg_lcg_u2_01_s_min_u3(o) ) );} \
  static inline f3_t bcore_prsg_lcg_u2_01_s_gen_f3( bcore_prsg_lcg_u2_01_s* o, f3_t min, f3_t max ){return  bcore_prsg_lcg_u2_01_s_state_f3(bcore_prsg_lcg_u2_01_s_gen(o),min, max );} \
  static inline sz_t bcore_prsg_lcg_u2_01_s_bits( const bcore_prsg_lcg_u2_01_s* o ){return  32;} \
  static inline u3_t bcore_prsg_lcg_u2_01_s_max_u3( const bcore_prsg_lcg_u2_01_s* o ){return  0xFFFFFFFFu;} \
  static inline u3_t bcore_prsg_lcg_u2_01_s_min_u3( const bcore_prsg_lcg_u2_01_s* o ){return  0;} \
  static inline u3_t bcore_prsg_lcg_u2_01_s_state_u3( const bcore_prsg_lcg_u2_01_s* o ){return  o->state;} \
  static inline bcore_prsg_lcg_u2_01_s* bcore_prsg_lcg_u2_01_s_set_state_u3( bcore_prsg_lcg_u2_01_s* o, u3_t seed ){o->state = seed ^ ( seed >> 32 );return o;} \
  static inline bcore_prsg_lcg_u2_01_s* bcore_prsg_lcg_u2_01_s_gen( bcore_prsg_lcg_u2_01_s* o ){o->state = bcore_lcg01_u2( o->state );return o;}
#define TYPEOF_bcore_prsg_lcg_u2_02_s 0xDBC196764E2C002Dull
#define BETH_EXPAND_ITEM_bcore_prsg_lcg_u2_02_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_lcg_u2_02_s ) \
  { \
      aware_t _; \
      u2_t state; \
  }; \
  static inline u3_t bcore_prsg_lcg_u2_02_s_gen_u3( bcore_prsg_lcg_u2_02_s* o ); \
  u3_t bcore_prsg_lcg_u2_02_s_state_bits_u3( const bcore_prsg_lcg_u2_02_s* o, sz_t bits ); \
  static inline u3_t bcore_prsg_lcg_u2_02_s_gen_bits_u3( bcore_prsg_lcg_u2_02_s* o, sz_t bits ); \
  static inline f3_t bcore_prsg_lcg_u2_02_s_state_f3( const bcore_prsg_lcg_u2_02_s* o, f3_t min, f3_t max ); \
  static inline f3_t bcore_prsg_lcg_u2_02_s_gen_f3( bcore_prsg_lcg_u2_02_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_lcg_u2_02_s_bits( const bcore_prsg_lcg_u2_02_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_02_s_max_u3( const bcore_prsg_lcg_u2_02_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_02_s_min_u3( const bcore_prsg_lcg_u2_02_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_02_s_state_u3( const bcore_prsg_lcg_u2_02_s* o ); \
  static inline bcore_prsg_lcg_u2_02_s* bcore_prsg_lcg_u2_02_s_set_state_u3( bcore_prsg_lcg_u2_02_s* o, u3_t seed ); \
  static inline bcore_prsg_lcg_u2_02_s* bcore_prsg_lcg_u2_02_s_gen( bcore_prsg_lcg_u2_02_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_02_s_gen_u3( bcore_prsg_lcg_u2_02_s* o ){return  bcore_prsg_lcg_u2_02_s_state_u3(bcore_prsg_lcg_u2_02_s_gen(o));} \
  static inline u3_t bcore_prsg_lcg_u2_02_s_gen_bits_u3( bcore_prsg_lcg_u2_02_s* o, sz_t bits ){return  bcore_prsg_lcg_u2_02_s_state_bits_u3(bcore_prsg_lcg_u2_02_s_gen(o),bits );} \
  static inline f3_t bcore_prsg_lcg_u2_02_s_state_f3( const bcore_prsg_lcg_u2_02_s* o, f3_t min, f3_t max ){return  min + ( max - min ) * ( ( f3_t )( bcore_prsg_lcg_u2_02_s_state_u3(o) - bcore_prsg_lcg_u2_02_s_min_u3(o) ) / ( bcore_prsg_lcg_u2_02_s_max_u3(o) - bcore_prsg_lcg_u2_02_s_min_u3(o) ) );} \
  static inline f3_t bcore_prsg_lcg_u2_02_s_gen_f3( bcore_prsg_lcg_u2_02_s* o, f3_t min, f3_t max ){return  bcore_prsg_lcg_u2_02_s_state_f3(bcore_prsg_lcg_u2_02_s_gen(o),min, max );} \
  static inline sz_t bcore_prsg_lcg_u2_02_s_bits( const bcore_prsg_lcg_u2_02_s* o ){return  32;} \
  static inline u3_t bcore_prsg_lcg_u2_02_s_max_u3( const bcore_prsg_lcg_u2_02_s* o ){return  0xFFFFFFFFu;} \
  static inline u3_t bcore_prsg_lcg_u2_02_s_min_u3( const bcore_prsg_lcg_u2_02_s* o ){return  0;} \
  static inline u3_t bcore_prsg_lcg_u2_02_s_state_u3( const bcore_prsg_lcg_u2_02_s* o ){return  o->state;} \
  static inline bcore_prsg_lcg_u2_02_s* bcore_prsg_lcg_u2_02_s_set_state_u3( bcore_prsg_lcg_u2_02_s* o, u3_t seed ){o->state = seed ^ ( seed >> 32 );return o;} \
  static inline bcore_prsg_lcg_u2_02_s* bcore_prsg_lcg_u2_02_s_gen( bcore_prsg_lcg_u2_02_s* o ){o->state = bcore_lcg02_u2( o->state );return o;}
#define TYPEOF_bcore_prsg_lcg_u2_03_s 0xD3995D7649B1EC92ull
#define BETH_EXPAND_ITEM_bcore_prsg_lcg_u2_03_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_lcg_u2_03_s ) \
  { \
      aware_t _; \
      u2_t state; \
  }; \
  static inline u3_t bcore_prsg_lcg_u2_03_s_gen_u3( bcore_prsg_lcg_u2_03_s* o ); \
  u3_t bcore_prsg_lcg_u2_03_s_state_bits_u3( const bcore_prsg_lcg_u2_03_s* o, sz_t bits ); \
  static inline u3_t bcore_prsg_lcg_u2_03_s_gen_bits_u3( bcore_prsg_lcg_u2_03_s* o, sz_t bits ); \
  static inline f3_t bcore_prsg_lcg_u2_03_s_state_f3( const bcore_prsg_lcg_u2_03_s* o, f3_t min, f3_t max ); \
  static inline f3_t bcore_prsg_lcg_u2_03_s_gen_f3( bcore_prsg_lcg_u2_03_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_lcg_u2_03_s_bits( const bcore_prsg_lcg_u2_03_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_03_s_max_u3( const bcore_prsg_lcg_u2_03_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_03_s_min_u3( const bcore_prsg_lcg_u2_03_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_03_s_state_u3( const bcore_prsg_lcg_u2_03_s* o ); \
  static inline bcore_prsg_lcg_u2_03_s* bcore_prsg_lcg_u2_03_s_set_state_u3( bcore_prsg_lcg_u2_03_s* o, u3_t seed ); \
  static inline bcore_prsg_lcg_u2_03_s* bcore_prsg_lcg_u2_03_s_gen( bcore_prsg_lcg_u2_03_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_03_s_gen_u3( bcore_prsg_lcg_u2_03_s* o ){return  bcore_prsg_lcg_u2_03_s_state_u3(bcore_prsg_lcg_u2_03_s_gen(o));} \
  static inline u3_t bcore_prsg_lcg_u2_03_s_gen_bits_u3( bcore_prsg_lcg_u2_03_s* o, sz_t bits ){return  bcore_prsg_lcg_u2_03_s_state_bits_u3(bcore_prsg_lcg_u2_03_s_gen(o),bits );} \
  static inline f3_t bcore_prsg_lcg_u2_03_s_state_f3( const bcore_prsg_lcg_u2_03_s* o, f3_t min, f3_t max ){return  min + ( max - min ) * ( ( f3_t )( bcore_prsg_lcg_u2_03_s_state_u3(o) - bcore_prsg_lcg_u2_03_s_min_u3(o) ) / ( bcore_prsg_lcg_u2_03_s_max_u3(o) - bcore_prsg_lcg_u2_03_s_min_u3(o) ) );} \
  static inline f3_t bcore_prsg_lcg_u2_03_s_gen_f3( bcore_prsg_lcg_u2_03_s* o, f3_t min, f3_t max ){return  bcore_prsg_lcg_u2_03_s_state_f3(bcore_prsg_lcg_u2_03_s_gen(o),min, max );} \
  static inline sz_t bcore_prsg_lcg_u2_03_s_bits( const bcore_prsg_lcg_u2_03_s* o ){return  32;} \
  static inline u3_t bcore_prsg_lcg_u2_03_s_max_u3( const bcore_prsg_lcg_u2_03_s* o ){return  0xFFFFFFFFu;} \
  static inline u3_t bcore_prsg_lcg_u2_03_s_min_u3( const bcore_prsg_lcg_u2_03_s* o ){return  0;} \
  static inline u3_t bcore_prsg_lcg_u2_03_s_state_u3( const bcore_prsg_lcg_u2_03_s* o ){return  o->state;} \
  static inline bcore_prsg_lcg_u2_03_s* bcore_prsg_lcg_u2_03_s_set_state_u3( bcore_prsg_lcg_u2_03_s* o, u3_t seed ){o->state = seed ^ ( seed >> 32 );return o;} \
  static inline bcore_prsg_lcg_u2_03_s* bcore_prsg_lcg_u2_03_s_gen( bcore_prsg_lcg_u2_03_s* o ){o->state = bcore_lcg03_u2( o->state );return o;}
#define TYPEOF_bcore_prsg_lcg_u2_04_s 0x0E667C766A7AFEFFull
#define BETH_EXPAND_ITEM_bcore_prsg_lcg_u2_04_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_lcg_u2_04_s ) \
  { \
      aware_t _; \
      u2_t state; \
  }; \
  static inline u3_t bcore_prsg_lcg_u2_04_s_gen_u3( bcore_prsg_lcg_u2_04_s* o ); \
  u3_t bcore_prsg_lcg_u2_04_s_state_bits_u3( const bcore_prsg_lcg_u2_04_s* o, sz_t bits ); \
  static inline u3_t bcore_prsg_lcg_u2_04_s_gen_bits_u3( bcore_prsg_lcg_u2_04_s* o, sz_t bits ); \
  static inline f3_t bcore_prsg_lcg_u2_04_s_state_f3( const bcore_prsg_lcg_u2_04_s* o, f3_t min, f3_t max ); \
  static inline f3_t bcore_prsg_lcg_u2_04_s_gen_f3( bcore_prsg_lcg_u2_04_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_lcg_u2_04_s_bits( const bcore_prsg_lcg_u2_04_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_04_s_max_u3( const bcore_prsg_lcg_u2_04_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_04_s_min_u3( const bcore_prsg_lcg_u2_04_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_04_s_state_u3( const bcore_prsg_lcg_u2_04_s* o ); \
  static inline bcore_prsg_lcg_u2_04_s* bcore_prsg_lcg_u2_04_s_set_state_u3( bcore_prsg_lcg_u2_04_s* o, u3_t seed ); \
  static inline bcore_prsg_lcg_u2_04_s* bcore_prsg_lcg_u2_04_s_gen( bcore_prsg_lcg_u2_04_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_04_s_gen_u3( bcore_prsg_lcg_u2_04_s* o ){return  bcore_prsg_lcg_u2_04_s_state_u3(bcore_prsg_lcg_u2_04_s_gen(o));} \
  static inline u3_t bcore_prsg_lcg_u2_04_s_gen_bits_u3( bcore_prsg_lcg_u2_04_s* o, sz_t bits ){return  bcore_prsg_lcg_u2_04_s_state_bits_u3(bcore_prsg_lcg_u2_04_s_gen(o),bits );} \
  static inline f3_t bcore_prsg_lcg_u2_04_s_state_f3( const bcore_prsg_lcg_u2_04_s* o, f3_t min, f3_t max ){return  min + ( max - min ) * ( ( f3_t )( bcore_prsg_lcg_u2_04_s_state_u3(o) - bcore_prsg_lcg_u2_04_s_min_u3(o) ) / ( bcore_prsg_lcg_u2_04_s_max_u3(o) - bcore_prsg_lcg_u2_04_s_min_u3(o) ) );} \
  static inline f3_t bcore_prsg_lcg_u2_04_s_gen_f3( bcore_prsg_lcg_u2_04_s* o, f3_t min, f3_t max ){return  bcore_prsg_lcg_u2_04_s_state_f3(bcore_prsg_lcg_u2_04_s_gen(o),min, max );} \
  static inline sz_t bcore_prsg_lcg_u2_04_s_bits( const bcore_prsg_lcg_u2_04_s* o ){return  32;} \
  static inline u3_t bcore_prsg_lcg_u2_04_s_max_u3( const bcore_prsg_lcg_u2_04_s* o ){return  0xFFFFFFFFu;} \
  static inline u3_t bcore_prsg_lcg_u2_04_s_min_u3( const bcore_prsg_lcg_u2_04_s* o ){return  0;} \
  static inline u3_t bcore_prsg_lcg_u2_04_s_state_u3( const bcore_prsg_lcg_u2_04_s* o ){return  o->state;} \
  static inline bcore_prsg_lcg_u2_04_s* bcore_prsg_lcg_u2_04_s_set_state_u3( bcore_prsg_lcg_u2_04_s* o, u3_t seed ){o->state = seed ^ ( seed >> 32 );return o;} \
  static inline bcore_prsg_lcg_u2_04_s* bcore_prsg_lcg_u2_04_s_gen( bcore_prsg_lcg_u2_04_s* o ){o->state = bcore_lcg04_u2( o->state );return o;}
#define TYPEOF_bcore_prsg_lcg_u2_05_s 0x0718637666BAC584ull
#define BETH_EXPAND_ITEM_bcore_prsg_lcg_u2_05_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_lcg_u2_05_s ) \
  { \
      aware_t _; \
      u2_t state; \
  }; \
  static inline u3_t bcore_prsg_lcg_u2_05_s_gen_u3( bcore_prsg_lcg_u2_05_s* o ); \
  u3_t bcore_prsg_lcg_u2_05_s_state_bits_u3( const bcore_prsg_lcg_u2_05_s* o, sz_t bits ); \
  static inline u3_t bcore_prsg_lcg_u2_05_s_gen_bits_u3( bcore_prsg_lcg_u2_05_s* o, sz_t bits ); \
  static inline f3_t bcore_prsg_lcg_u2_05_s_state_f3( const bcore_prsg_lcg_u2_05_s* o, f3_t min, f3_t max ); \
  static inline f3_t bcore_prsg_lcg_u2_05_s_gen_f3( bcore_prsg_lcg_u2_05_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_lcg_u2_05_s_bits( const bcore_prsg_lcg_u2_05_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_05_s_max_u3( const bcore_prsg_lcg_u2_05_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_05_s_min_u3( const bcore_prsg_lcg_u2_05_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_05_s_state_u3( const bcore_prsg_lcg_u2_05_s* o ); \
  static inline bcore_prsg_lcg_u2_05_s* bcore_prsg_lcg_u2_05_s_set_state_u3( bcore_prsg_lcg_u2_05_s* o, u3_t seed ); \
  static inline bcore_prsg_lcg_u2_05_s* bcore_prsg_lcg_u2_05_s_gen( bcore_prsg_lcg_u2_05_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_05_s_gen_u3( bcore_prsg_lcg_u2_05_s* o ){return  bcore_prsg_lcg_u2_05_s_state_u3(bcore_prsg_lcg_u2_05_s_gen(o));} \
  static inline u3_t bcore_prsg_lcg_u2_05_s_gen_bits_u3( bcore_prsg_lcg_u2_05_s* o, sz_t bits ){return  bcore_prsg_lcg_u2_05_s_state_bits_u3(bcore_prsg_lcg_u2_05_s_gen(o),bits );} \
  static inline f3_t bcore_prsg_lcg_u2_05_s_state_f3( const bcore_prsg_lcg_u2_05_s* o, f3_t min, f3_t max ){return  min + ( max - min ) * ( ( f3_t )( bcore_prsg_lcg_u2_05_s_state_u3(o) - bcore_prsg_lcg_u2_05_s_min_u3(o) ) / ( bcore_prsg_lcg_u2_05_s_max_u3(o) - bcore_prsg_lcg_u2_05_s_min_u3(o) ) );} \
  static inline f3_t bcore_prsg_lcg_u2_05_s_gen_f3( bcore_prsg_lcg_u2_05_s* o, f3_t min, f3_t max ){return  bcore_prsg_lcg_u2_05_s_state_f3(bcore_prsg_lcg_u2_05_s_gen(o),min, max );} \
  static inline sz_t bcore_prsg_lcg_u2_05_s_bits( const bcore_prsg_lcg_u2_05_s* o ){return  32;} \
  static inline u3_t bcore_prsg_lcg_u2_05_s_max_u3( const bcore_prsg_lcg_u2_05_s* o ){return  0xFFFFFFFFu;} \
  static inline u3_t bcore_prsg_lcg_u2_05_s_min_u3( const bcore_prsg_lcg_u2_05_s* o ){return  0;} \
  static inline u3_t bcore_prsg_lcg_u2_05_s_state_u3( const bcore_prsg_lcg_u2_05_s* o ){return  o->state;} \
  static inline bcore_prsg_lcg_u2_05_s* bcore_prsg_lcg_u2_05_s_set_state_u3( bcore_prsg_lcg_u2_05_s* o, u3_t seed ){o->state = seed ^ ( seed >> 32 );return o;} \
  static inline bcore_prsg_lcg_u2_05_s* bcore_prsg_lcg_u2_05_s_gen( bcore_prsg_lcg_u2_05_s* o ){o->state = bcore_lcg05_u2( o->state );return o;}
#define TYPEOF_bcore_prsg_lcg_u3_00_s 0xE7FB4DA7051A0C0Aull
#define BETH_EXPAND_ITEM_bcore_prsg_lcg_u3_00_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_lcg_u3_00_s ) \
  { \
      aware_t _; \
      u3_t state; \
  }; \
  static inline u3_t bcore_prsg_lcg_u3_00_s_gen_u3( bcore_prsg_lcg_u3_00_s* o ); \
  u3_t bcore_prsg_lcg_u3_00_s_state_bits_u3( const bcore_prsg_lcg_u3_00_s* o, sz_t bits ); \
  static inline u3_t bcore_prsg_lcg_u3_00_s_gen_bits_u3( bcore_prsg_lcg_u3_00_s* o, sz_t bits ); \
  static inline f3_t bcore_prsg_lcg_u3_00_s_state_f3( const bcore_prsg_lcg_u3_00_s* o, f3_t min, f3_t max ); \
  static inline f3_t bcore_prsg_lcg_u3_00_s_gen_f3( bcore_prsg_lcg_u3_00_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_lcg_u3_00_s_bits( const bcore_prsg_lcg_u3_00_s* o ); \
  static inline u3_t bcore_prsg_lcg_u3_00_s_max_u3( const bcore_prsg_lcg_u3_00_s* o ); \
  static inline u3_t bcore_prsg_lcg_u3_00_s_min_u3( const bcore_prsg_lcg_u3_00_s* o ); \
  static inline u3_t bcore_prsg_lcg_u3_00_s_state_u3( const bcore_prsg_lcg_u3_00_s* o ); \
  static inline bcore_prsg_lcg_u3_00_s* bcore_prsg_lcg_u3_00_s_set_state_u3( bcore_prsg_lcg_u3_00_s* o, u3_t seed ); \
  static inline bcore_prsg_lcg_u3_00_s* bcore_prsg_lcg_u3_00_s_gen( bcore_prsg_lcg_u3_00_s* o ); \
  static inline u3_t bcore_prsg_lcg_u3_00_s_gen_u3( bcore_prsg_lcg_u3_00_s* o ){return  bcore_prsg_lcg_u3_00_s_state_u3(bcore_prsg_lcg_u3_00_s_gen(o));} \
  static inline u3_t bcore_prsg_lcg_u3_00_s_gen_bits_u3( bcore_prsg_lcg_u3_00_s* o, sz_t bits ){return  bcore_prsg_lcg_u3_00_s_state_bits_u3(bcore_prsg_lcg_u3_00_s_gen(o),bits );} \
  static inline f3_t bcore_prsg_lcg_u3_00_s_state_f3( const bcore_prsg_lcg_u3_00_s* o, f3_t min, f3_t max ){return  min + ( max - min ) * ( ( f3_t )( bcore_prsg_lcg_u3_00_s_state_u3(o) - bcore_prsg_lcg_u3_00_s_min_u3(o) ) / ( bcore_prsg_lcg_u3_00_s_max_u3(o) - bcore_prsg_lcg_u3_00_s_min_u3(o) ) );} \
  static inline f3_t bcore_prsg_lcg_u3_00_s_gen_f3( bcore_prsg_lcg_u3_00_s* o, f3_t min, f3_t max ){return  bcore_prsg_lcg_u3_00_s_state_f3(bcore_prsg_lcg_u3_00_s_gen(o),min, max );} \
  static inline sz_t bcore_prsg_lcg_u3_00_s_bits( const bcore_prsg_lcg_u3_00_s* o ){return  64;} \
  static inline u3_t bcore_prsg_lcg_u3_00_s_max_u3( const bcore_prsg_lcg_u3_00_s* o ){return  0xFFFFFFFFFFFFFFFFull;} \
  static inline u3_t bcore_prsg_lcg_u3_00_s_min_u3( const bcore_prsg_lcg_u3_00_s* o ){return  0;} \
  static inline u3_t bcore_prsg_lcg_u3_00_s_state_u3( const bcore_prsg_lcg_u3_00_s* o ){return  o->state;} \
  static inline bcore_prsg_lcg_u3_00_s* bcore_prsg_lcg_u3_00_s_set_state_u3( bcore_prsg_lcg_u3_00_s* o, u3_t seed ){o->state = seed;return o;} \
  static inline bcore_prsg_lcg_u3_00_s* bcore_prsg_lcg_u3_00_s_gen( bcore_prsg_lcg_u3_00_s* o ){o->state = bcore_lcg00_u3( o->state );return o;}
#define TYPEOF_bcore_prsg_lcg_u3_01_s 0xEF4A66A708DBF885ull
#define BETH_EXPAND_ITEM_bcore_prsg_lcg_u3_01_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_lcg_u3_01_s ) \
  { \
      aware_t _; \
      u3_t state; \
  }; \
  static inline u3_t bcore_prsg_lcg_u3_01_s_gen_u3( bcore_prsg_lcg_u3_01_s* o ); \
  u3_t bcore_prsg_lcg_u3_01_s_state_bits_u3( const bcore_prsg_lcg_u3_01_s* o, sz_t bits ); \
  static inline u3_t bcore_prsg_lcg_u3_01_s_gen_bits_u3( bcore_prsg_lcg_u3_01_s* o, sz_t bits ); \
  static inline f3_t bcore_prsg_lcg_u3_01_s_state_f3( const bcore_prsg_lcg_u3_01_s* o, f3_t min, f3_t max ); \
  static inline f3_t bcore_prsg_lcg_u3_01_s_gen_f3( bcore_prsg_lcg_u3_01_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_lcg_u3_01_s_bits( const bcore_prsg_lcg_u3_01_s* o ); \
  static inline u3_t bcore_prsg_lcg_u3_01_s_max_u3( const bcore_prsg_lcg_u3_01_s* o ); \
  static inline u3_t bcore_prsg_lcg_u3_01_s_min_u3( const bcore_prsg_lcg_u3_01_s* o ); \
  static inline u3_t bcore_prsg_lcg_u3_01_s_state_u3( const bcore_prsg_lcg_u3_01_s* o ); \
  static inline bcore_prsg_lcg_u3_01_s* bcore_prsg_lcg_u3_01_s_set_state_u3( bcore_prsg_lcg_u3_01_s* o, u3_t seed ); \
  static inline bcore_prsg_lcg_u3_01_s* bcore_prsg_lcg_u3_01_s_gen( bcore_prsg_lcg_u3_01_s* o ); \
  static inline u3_t bcore_prsg_lcg_u3_01_s_gen_u3( bcore_prsg_lcg_u3_01_s* o ){return  bcore_prsg_lcg_u3_01_s_state_u3(bcore_prsg_lcg_u3_01_s_gen(o));} \
  static inline u3_t bcore_prsg_lcg_u3_01_s_gen_bits_u3( bcore_prsg_lcg_u3_01_s* o, sz_t bits ){return  bcore_prsg_lcg_u3_01_s_state_bits_u3(bcore_prsg_lcg_u3_01_s_gen(o),bits );} \
  static inline f3_t bcore_prsg_lcg_u3_01_s_state_f3( const bcore_prsg_lcg_u3_01_s* o, f3_t min, f3_t max ){return  min + ( max - min ) * ( ( f3_t )( bcore_prsg_lcg_u3_01_s_state_u3(o) - bcore_prsg_lcg_u3_01_s_min_u3(o) ) / ( bcore_prsg_lcg_u3_01_s_max_u3(o) - bcore_prsg_lcg_u3_01_s_min_u3(o) ) );} \
  static inline f3_t bcore_prsg_lcg_u3_01_s_gen_f3( bcore_prsg_lcg_u3_01_s* o, f3_t min, f3_t max ){return  bcore_prsg_lcg_u3_01_s_state_f3(bcore_prsg_lcg_u3_01_s_gen(o),min, max );} \
  static inline sz_t bcore_prsg_lcg_u3_01_s_bits( const bcore_prsg_lcg_u3_01_s* o ){return  64;} \
  static inline u3_t bcore_prsg_lcg_u3_01_s_max_u3( const bcore_prsg_lcg_u3_01_s* o ){return  0xFFFFFFFFFFFFFFFFull;} \
  static inline u3_t bcore_prsg_lcg_u3_01_s_min_u3( const bcore_prsg_lcg_u3_01_s* o ){return  0;} \
  static inline u3_t bcore_prsg_lcg_u3_01_s_state_u3( const bcore_prsg_lcg_u3_01_s* o ){return  o->state;} \
  static inline bcore_prsg_lcg_u3_01_s* bcore_prsg_lcg_u3_01_s_set_state_u3( bcore_prsg_lcg_u3_01_s* o, u3_t seed ){o->state = seed;return o;} \
  static inline bcore_prsg_lcg_u3_01_s* bcore_prsg_lcg_u3_01_s_gen( bcore_prsg_lcg_u3_01_s* o ){o->state = bcore_lcg01_u3( o->state );return o;}
#define TYPEOF_bcore_prsg_lcg_u3_02_s 0xF77297A70D55FE88ull
#define BETH_EXPAND_ITEM_bcore_prsg_lcg_u3_02_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_lcg_u3_02_s ) \
  { \
      aware_t _; \
      u3_t state; \
  }; \
  static inline u3_t bcore_prsg_lcg_u3_02_s_gen_u3( bcore_prsg_lcg_u3_02_s* o ); \
  u3_t bcore_prsg_lcg_u3_02_s_state_bits_u3( const bcore_prsg_lcg_u3_02_s* o, sz_t bits ); \
  static inline u3_t bcore_prsg_lcg_u3_02_s_gen_bits_u3( bcore_prsg_lcg_u3_02_s* o, sz_t bits ); \
  static inline f3_t bcore_prsg_lcg_u3_02_s_state_f3( const bcore_prsg_lcg_u3_02_s* o, f3_t min, f3_t max ); \
  static inline f3_t bcore_prsg_lcg_u3_02_s_gen_f3( bcore_prsg_lcg_u3_02_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_lcg_u3_02_s_bits( const bcore_prsg_lcg_u3_02_s* o ); \
  static inline u3_t bcore_prsg_lcg_u3_02_s_max_u3( const bcore_prsg_lcg_u3_02_s* o ); \
  static inline u3_t bcore_prsg_lcg_u3_02_s_min_u3( const bcore_prsg_lcg_u3_02_s* o ); \
  static inline u3_t bcore_prsg_lcg_u3_02_s_state_u3( const bcore_prsg_lcg_u3_02_s* o ); \
  static inline bcore_prsg_lcg_u3_02_s* bcore_prsg_lcg_u3_02_s_set_state_u3( bcore_prsg_lcg_u3_02_s* o, u3_t seed ); \
  static inline bcore_prsg_lcg_u3_02_s* bcore_prsg_lcg_u3_02_s_gen( bcore_prsg_lcg_u3_02_s* o ); \
  static inline u3_t bcore_prsg_lcg_u3_02_s_gen_u3( bcore_prsg_lcg_u3_02_s* o ){return  bcore_prsg_lcg_u3_02_s_state_u3(bcore_prsg_lcg_u3_02_s_gen(o));} \
  static inline u3_t bcore_prsg_lcg_u3_02_s_gen_bits_u3( bcore_prsg_lcg_u3_02_s* o, sz_t bits ){return  bcore_prsg_lcg_u3_02_s_state_bits_u3(bcore_prsg_lcg_u3_02_s_gen(o),bits );} \
  static inline f3_t bcore_prsg_lcg_u3_02_s_state_f3( const bcore_prsg_lcg_u3_02_s* o, f3_t min, f3_t max ){return  min + ( max - min ) * ( ( f3_t )( bcore_prsg_lcg_u3_02_s_state_u3(o) - bcore_prsg_lcg_u3_02_s_min_u3(o) ) / ( bcore_prsg_lcg_u3_02_s_max_u3(o) - bcore_prsg_lcg_u3_02_s_min_u3(o) ) );} \
  static inline f3_t bcore_prsg_lcg_u3_02_s_gen_f3( bcore_prsg_lcg_u3_02_s* o, f3_t min, f3_t max ){return  bcore_prsg_lcg_u3_02_s_state_f3(bcore_prsg_lcg_u3_02_s_gen(o),min, max );} \
  static inline sz_t bcore_prsg_lcg_u3_02_s_bits( const bcore_prsg_lcg_u3_02_s* o ){return  64;} \
  static inline u3_t bcore_prsg_lcg_u3_02_s_max_u3( const bcore_prsg_lcg_u3_02_s* o ){return  0xFFFFFFFFFFFFFFFFull;} \
  static inline u3_t bcore_prsg_lcg_u3_02_s_min_u3( const bcore_prsg_lcg_u3_02_s* o ){return  0;} \
  static inline u3_t bcore_prsg_lcg_u3_02_s_state_u3( const bcore_prsg_lcg_u3_02_s* o ){return  o->state;} \
  static inline bcore_prsg_lcg_u3_02_s* bcore_prsg_lcg_u3_02_s_set_state_u3( bcore_prsg_lcg_u3_02_s* o, u3_t seed ){o->state = seed;return o;} \
  static inline bcore_prsg_lcg_u3_02_s* bcore_prsg_lcg_u3_02_s_gen( bcore_prsg_lcg_u3_02_s* o ){o->state = bcore_lcg02_u3( o->state );return o;}
#define TYPEOF_bcore_prsg_lcg_u3_03_s 0x0227B0A713FB1403ull
#define BETH_EXPAND_ITEM_bcore_prsg_lcg_u3_03_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_lcg_u3_03_s ) \
  { \
      aware_t _; \
      u3_t state; \
  }; \
  static inline u3_t bcore_prsg_lcg_u3_03_s_gen_u3( bcore_prsg_lcg_u3_03_s* o ); \
  u3_t bcore_prsg_lcg_u3_03_s_state_bits_u3( const bcore_prsg_lcg_u3_03_s* o, sz_t bits ); \
  static inline u3_t bcore_prsg_lcg_u3_03_s_gen_bits_u3( bcore_prsg_lcg_u3_03_s* o, sz_t bits ); \
  static inline f3_t bcore_prsg_lcg_u3_03_s_state_f3( const bcore_prsg_lcg_u3_03_s* o, f3_t min, f3_t max ); \
  static inline f3_t bcore_prsg_lcg_u3_03_s_gen_f3( bcore_prsg_lcg_u3_03_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_lcg_u3_03_s_bits( const bcore_prsg_lcg_u3_03_s* o ); \
  static inline u3_t bcore_prsg_lcg_u3_03_s_max_u3( const bcore_prsg_lcg_u3_03_s* o ); \
  static inline u3_t bcore_prsg_lcg_u3_03_s_min_u3( const bcore_prsg_lcg_u3_03_s* o ); \
  static inline u3_t bcore_prsg_lcg_u3_03_s_state_u3( const bcore_prsg_lcg_u3_03_s* o ); \
  static inline bcore_prsg_lcg_u3_03_s* bcore_prsg_lcg_u3_03_s_set_state_u3( bcore_prsg_lcg_u3_03_s* o, u3_t seed ); \
  static inline bcore_prsg_lcg_u3_03_s* bcore_prsg_lcg_u3_03_s_gen( bcore_prsg_lcg_u3_03_s* o ); \
  static inline u3_t bcore_prsg_lcg_u3_03_s_gen_u3( bcore_prsg_lcg_u3_03_s* o ){return  bcore_prsg_lcg_u3_03_s_state_u3(bcore_prsg_lcg_u3_03_s_gen(o));} \
  static inline u3_t bcore_prsg_lcg_u3_03_s_gen_bits_u3( bcore_prsg_lcg_u3_03_s* o, sz_t bits ){return  bcore_prsg_lcg_u3_03_s_state_bits_u3(bcore_prsg_lcg_u3_03_s_gen(o),bits );} \
  static inline f3_t bcore_prsg_lcg_u3_03_s_state_f3( const bcore_prsg_lcg_u3_03_s* o, f3_t min, f3_t max ){return  min + ( max - min ) * ( ( f3_t )( bcore_prsg_lcg_u3_03_s_state_u3(o) - bcore_prsg_lcg_u3_03_s_min_u3(o) ) / ( bcore_prsg_lcg_u3_03_s_max_u3(o) - bcore_prsg_lcg_u3_03_s_min_u3(o) ) );} \
  static inline f3_t bcore_prsg_lcg_u3_03_s_gen_f3( bcore_prsg_lcg_u3_03_s* o, f3_t min, f3_t max ){return  bcore_prsg_lcg_u3_03_s_state_f3(bcore_prsg_lcg_u3_03_s_gen(o),min, max );} \
  static inline sz_t bcore_prsg_lcg_u3_03_s_bits( const bcore_prsg_lcg_u3_03_s* o ){return  64;} \
  static inline u3_t bcore_prsg_lcg_u3_03_s_max_u3( const bcore_prsg_lcg_u3_03_s* o ){return  0xFFFFFFFFFFFFFFFFull;} \
  static inline u3_t bcore_prsg_lcg_u3_03_s_min_u3( const bcore_prsg_lcg_u3_03_s* o ){return  0;} \
  static inline u3_t bcore_prsg_lcg_u3_03_s_state_u3( const bcore_prsg_lcg_u3_03_s* o ){return  o->state;} \
  static inline bcore_prsg_lcg_u3_03_s* bcore_prsg_lcg_u3_03_s_set_state_u3( bcore_prsg_lcg_u3_03_s* o, u3_t seed ){o->state = seed;return o;} \
  static inline bcore_prsg_lcg_u3_03_s* bcore_prsg_lcg_u3_03_s_gen( bcore_prsg_lcg_u3_03_s* o ){o->state = bcore_lcg03_u3( o->state );return o;}
#define BETH_EXPAND_GROUP_bcore_prsg_lcg \
  BCORE_FORWARD_OBJECT( bcore_prsg_lcg ); \
  BCORE_FORWARD_OBJECT( bcore_prsg_lcg_u2_00_s ); \
  BCORE_FORWARD_OBJECT( bcore_prsg_lcg_u2_01_s ); \
  BCORE_FORWARD_OBJECT( bcore_prsg_lcg_u2_02_s ); \
  BCORE_FORWARD_OBJECT( bcore_prsg_lcg_u2_03_s ); \
  BCORE_FORWARD_OBJECT( bcore_prsg_lcg_u2_04_s ); \
  BCORE_FORWARD_OBJECT( bcore_prsg_lcg_u2_05_s ); \
  BCORE_FORWARD_OBJECT( bcore_prsg_lcg_u3_00_s ); \
  BCORE_FORWARD_OBJECT( bcore_prsg_lcg_u3_01_s ); \
  BCORE_FORWARD_OBJECT( bcore_prsg_lcg_u3_02_s ); \
  BCORE_FORWARD_OBJECT( bcore_prsg_lcg_u3_03_s ); \
  XOILA_DECLARE_SPECT( bcore_prsg_lcg ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bcore_prsg_lcg ) \
  BETH_EXPAND_ITEM_bcore_prsg_lcg_u2_00_s \
  BETH_EXPAND_ITEM_bcore_prsg_lcg_u2_01_s \
  BETH_EXPAND_ITEM_bcore_prsg_lcg_u2_02_s \
  BETH_EXPAND_ITEM_bcore_prsg_lcg_u2_03_s \
  BETH_EXPAND_ITEM_bcore_prsg_lcg_u2_04_s \
  BETH_EXPAND_ITEM_bcore_prsg_lcg_u2_05_s \
  BETH_EXPAND_ITEM_bcore_prsg_lcg_u3_00_s \
  BETH_EXPAND_ITEM_bcore_prsg_lcg_u3_01_s \
  BETH_EXPAND_ITEM_bcore_prsg_lcg_u3_02_s \
  BETH_EXPAND_ITEM_bcore_prsg_lcg_u3_03_s

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_prsg_xsg

#define TYPEOF_bcore_prsg_xsg 0x3007514603865DC6ull
#define TYPEOF_bcore_prsg_xsg_spect_s 0x38DC0E7711F0E3D2ull
#define TYPEOF_bcore_prsg_xsg_u2_00_s 0x6446C2147EAACD0Full
#define BETH_EXPAND_ITEM_bcore_prsg_xsg_u2_00_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_xsg_u2_00_s ) \
  { \
      aware_t _; \
      u2_t state; \
  }; \
  static inline u3_t bcore_prsg_xsg_u2_00_s_gen_u3( bcore_prsg_xsg_u2_00_s* o ); \
  u3_t bcore_prsg_xsg_u2_00_s_state_bits_u3( const bcore_prsg_xsg_u2_00_s* o, sz_t bits ); \
  static inline u3_t bcore_prsg_xsg_u2_00_s_gen_bits_u3( bcore_prsg_xsg_u2_00_s* o, sz_t bits ); \
  static inline f3_t bcore_prsg_xsg_u2_00_s_state_f3( const bcore_prsg_xsg_u2_00_s* o, f3_t min, f3_t max ); \
  static inline f3_t bcore_prsg_xsg_u2_00_s_gen_f3( bcore_prsg_xsg_u2_00_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_xsg_u2_00_s_bits( const bcore_prsg_xsg_u2_00_s* o ); \
  static inline u3_t bcore_prsg_xsg_u2_00_s_max_u3( const bcore_prsg_xsg_u2_00_s* o ); \
  static inline u3_t bcore_prsg_xsg_u2_00_s_min_u3( const bcore_prsg_xsg_u2_00_s* o ); \
  static inline u3_t bcore_prsg_xsg_u2_00_s_state_u3( const bcore_prsg_xsg_u2_00_s* o ); \
  static inline bcore_prsg_xsg_u2_00_s* bcore_prsg_xsg_u2_00_s_set_state_u3( bcore_prsg_xsg_u2_00_s* o, u3_t seed ); \
  static inline bcore_prsg_xsg_u2_00_s* bcore_prsg_xsg_u2_00_s_gen( bcore_prsg_xsg_u2_00_s* o ); \
  static inline u3_t bcore_prsg_xsg_u2_00_s_gen_u3( bcore_prsg_xsg_u2_00_s* o ){return  bcore_prsg_xsg_u2_00_s_state_u3(bcore_prsg_xsg_u2_00_s_gen(o));} \
  static inline u3_t bcore_prsg_xsg_u2_00_s_gen_bits_u3( bcore_prsg_xsg_u2_00_s* o, sz_t bits ){return  bcore_prsg_xsg_u2_00_s_state_bits_u3(bcore_prsg_xsg_u2_00_s_gen(o),bits );} \
  static inline f3_t bcore_prsg_xsg_u2_00_s_state_f3( const bcore_prsg_xsg_u2_00_s* o, f3_t min, f3_t max ){return  min + ( max - min ) * ( ( f3_t )( bcore_prsg_xsg_u2_00_s_state_u3(o) - bcore_prsg_xsg_u2_00_s_min_u3(o) ) / ( bcore_prsg_xsg_u2_00_s_max_u3(o) - bcore_prsg_xsg_u2_00_s_min_u3(o) ) );} \
  static inline f3_t bcore_prsg_xsg_u2_00_s_gen_f3( bcore_prsg_xsg_u2_00_s* o, f3_t min, f3_t max ){return  bcore_prsg_xsg_u2_00_s_state_f3(bcore_prsg_xsg_u2_00_s_gen(o),min, max );} \
  static inline sz_t bcore_prsg_xsg_u2_00_s_bits( const bcore_prsg_xsg_u2_00_s* o ){return  32;} \
  static inline u3_t bcore_prsg_xsg_u2_00_s_max_u3( const bcore_prsg_xsg_u2_00_s* o ){return  0xFFFFFFFFu;} \
  static inline u3_t bcore_prsg_xsg_u2_00_s_min_u3( const bcore_prsg_xsg_u2_00_s* o ){return  1;} \
  static inline u3_t bcore_prsg_xsg_u2_00_s_state_u3( const bcore_prsg_xsg_u2_00_s* o ){return  o->state;} \
  static inline bcore_prsg_xsg_u2_00_s* bcore_prsg_xsg_u2_00_s_set_state_u3( bcore_prsg_xsg_u2_00_s* o, u3_t seed ){o->state = u2_max( 1, seed ^ ( seed >> 32 ) );return o;} \
  static inline bcore_prsg_xsg_u2_00_s* bcore_prsg_xsg_u2_00_s_gen( bcore_prsg_xsg_u2_00_s* o ){o->state = bcore_xsg_u2(  o->state );return o;}
#define TYPEOF_bcore_prsg_xsg_u2_01_s 0x5C1EA9147A30EFD4ull
#define BETH_EXPAND_ITEM_bcore_prsg_xsg_u2_01_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_xsg_u2_01_s ) \
  { \
      aware_t _; \
      u2_t state; \
  }; \
  static inline u3_t bcore_prsg_xsg_u2_01_s_gen_u3( bcore_prsg_xsg_u2_01_s* o ); \
  u3_t bcore_prsg_xsg_u2_01_s_state_bits_u3( const bcore_prsg_xsg_u2_01_s* o, sz_t bits ); \
  static inline u3_t bcore_prsg_xsg_u2_01_s_gen_bits_u3( bcore_prsg_xsg_u2_01_s* o, sz_t bits ); \
  static inline f3_t bcore_prsg_xsg_u2_01_s_state_f3( const bcore_prsg_xsg_u2_01_s* o, f3_t min, f3_t max ); \
  static inline f3_t bcore_prsg_xsg_u2_01_s_gen_f3( bcore_prsg_xsg_u2_01_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_xsg_u2_01_s_bits( const bcore_prsg_xsg_u2_01_s* o ); \
  static inline u3_t bcore_prsg_xsg_u2_01_s_max_u3( const bcore_prsg_xsg_u2_01_s* o ); \
  static inline u3_t bcore_prsg_xsg_u2_01_s_min_u3( const bcore_prsg_xsg_u2_01_s* o ); \
  static inline u3_t bcore_prsg_xsg_u2_01_s_state_u3( const bcore_prsg_xsg_u2_01_s* o ); \
  static inline bcore_prsg_xsg_u2_01_s* bcore_prsg_xsg_u2_01_s_set_state_u3( bcore_prsg_xsg_u2_01_s* o, u3_t seed ); \
  static inline bcore_prsg_xsg_u2_01_s* bcore_prsg_xsg_u2_01_s_gen( bcore_prsg_xsg_u2_01_s* o ); \
  static inline u3_t bcore_prsg_xsg_u2_01_s_gen_u3( bcore_prsg_xsg_u2_01_s* o ){return  bcore_prsg_xsg_u2_01_s_state_u3(bcore_prsg_xsg_u2_01_s_gen(o));} \
  static inline u3_t bcore_prsg_xsg_u2_01_s_gen_bits_u3( bcore_prsg_xsg_u2_01_s* o, sz_t bits ){return  bcore_prsg_xsg_u2_01_s_state_bits_u3(bcore_prsg_xsg_u2_01_s_gen(o),bits );} \
  static inline f3_t bcore_prsg_xsg_u2_01_s_state_f3( const bcore_prsg_xsg_u2_01_s* o, f3_t min, f3_t max ){return  min + ( max - min ) * ( ( f3_t )( bcore_prsg_xsg_u2_01_s_state_u3(o) - bcore_prsg_xsg_u2_01_s_min_u3(o) ) / ( bcore_prsg_xsg_u2_01_s_max_u3(o) - bcore_prsg_xsg_u2_01_s_min_u3(o) ) );} \
  static inline f3_t bcore_prsg_xsg_u2_01_s_gen_f3( bcore_prsg_xsg_u2_01_s* o, f3_t min, f3_t max ){return  bcore_prsg_xsg_u2_01_s_state_f3(bcore_prsg_xsg_u2_01_s_gen(o),min, max );} \
  static inline sz_t bcore_prsg_xsg_u2_01_s_bits( const bcore_prsg_xsg_u2_01_s* o ){return  32;} \
  static inline u3_t bcore_prsg_xsg_u2_01_s_max_u3( const bcore_prsg_xsg_u2_01_s* o ){return  0xFFFFFFFFu;} \
  static inline u3_t bcore_prsg_xsg_u2_01_s_min_u3( const bcore_prsg_xsg_u2_01_s* o ){return  1;} \
  static inline u3_t bcore_prsg_xsg_u2_01_s_state_u3( const bcore_prsg_xsg_u2_01_s* o ){return  o->state;} \
  static inline bcore_prsg_xsg_u2_01_s* bcore_prsg_xsg_u2_01_s_set_state_u3( bcore_prsg_xsg_u2_01_s* o, u3_t seed ){o->state = u2_max( 1, seed ^ ( seed >> 32 ) );return o;} \
  static inline bcore_prsg_xsg_u2_01_s* bcore_prsg_xsg_u2_01_s_gen( bcore_prsg_xsg_u2_01_s* o ){o->state = bcore_xsg1_u2( o->state );return o;}
#define TYPEOF_bcore_prsg_xsg_u2_02_s 0x54CF7814766EDA91ull
#define BETH_EXPAND_ITEM_bcore_prsg_xsg_u2_02_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_xsg_u2_02_s ) \
  { \
      aware_t _; \
      u2_t state; \
  }; \
  static inline u3_t bcore_prsg_xsg_u2_02_s_gen_u3( bcore_prsg_xsg_u2_02_s* o ); \
  u3_t bcore_prsg_xsg_u2_02_s_state_bits_u3( const bcore_prsg_xsg_u2_02_s* o, sz_t bits ); \
  static inline u3_t bcore_prsg_xsg_u2_02_s_gen_bits_u3( bcore_prsg_xsg_u2_02_s* o, sz_t bits ); \
  static inline f3_t bcore_prsg_xsg_u2_02_s_state_f3( const bcore_prsg_xsg_u2_02_s* o, f3_t min, f3_t max ); \
  static inline f3_t bcore_prsg_xsg_u2_02_s_gen_f3( bcore_prsg_xsg_u2_02_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_xsg_u2_02_s_bits( const bcore_prsg_xsg_u2_02_s* o ); \
  static inline u3_t bcore_prsg_xsg_u2_02_s_max_u3( const bcore_prsg_xsg_u2_02_s* o ); \
  static inline u3_t bcore_prsg_xsg_u2_02_s_min_u3( const bcore_prsg_xsg_u2_02_s* o ); \
  static inline u3_t bcore_prsg_xsg_u2_02_s_state_u3( const bcore_prsg_xsg_u2_02_s* o ); \
  static inline bcore_prsg_xsg_u2_02_s* bcore_prsg_xsg_u2_02_s_set_state_u3( bcore_prsg_xsg_u2_02_s* o, u3_t seed ); \
  static inline bcore_prsg_xsg_u2_02_s* bcore_prsg_xsg_u2_02_s_gen( bcore_prsg_xsg_u2_02_s* o ); \
  static inline u3_t bcore_prsg_xsg_u2_02_s_gen_u3( bcore_prsg_xsg_u2_02_s* o ){return  bcore_prsg_xsg_u2_02_s_state_u3(bcore_prsg_xsg_u2_02_s_gen(o));} \
  static inline u3_t bcore_prsg_xsg_u2_02_s_gen_bits_u3( bcore_prsg_xsg_u2_02_s* o, sz_t bits ){return  bcore_prsg_xsg_u2_02_s_state_bits_u3(bcore_prsg_xsg_u2_02_s_gen(o),bits );} \
  static inline f3_t bcore_prsg_xsg_u2_02_s_state_f3( const bcore_prsg_xsg_u2_02_s* o, f3_t min, f3_t max ){return  min + ( max - min ) * ( ( f3_t )( bcore_prsg_xsg_u2_02_s_state_u3(o) - bcore_prsg_xsg_u2_02_s_min_u3(o) ) / ( bcore_prsg_xsg_u2_02_s_max_u3(o) - bcore_prsg_xsg_u2_02_s_min_u3(o) ) );} \
  static inline f3_t bcore_prsg_xsg_u2_02_s_gen_f3( bcore_prsg_xsg_u2_02_s* o, f3_t min, f3_t max ){return  bcore_prsg_xsg_u2_02_s_state_f3(bcore_prsg_xsg_u2_02_s_gen(o),min, max );} \
  static inline sz_t bcore_prsg_xsg_u2_02_s_bits( const bcore_prsg_xsg_u2_02_s* o ){return  32;} \
  static inline u3_t bcore_prsg_xsg_u2_02_s_max_u3( const bcore_prsg_xsg_u2_02_s* o ){return  0xFFFFFFFFu;} \
  static inline u3_t bcore_prsg_xsg_u2_02_s_min_u3( const bcore_prsg_xsg_u2_02_s* o ){return  1;} \
  static inline u3_t bcore_prsg_xsg_u2_02_s_state_u3( const bcore_prsg_xsg_u2_02_s* o ){return  o->state;} \
  static inline bcore_prsg_xsg_u2_02_s* bcore_prsg_xsg_u2_02_s_set_state_u3( bcore_prsg_xsg_u2_02_s* o, u3_t seed ){o->state = u2_max( 1, seed ^ ( seed >> 32 ) );return o;} \
  static inline bcore_prsg_xsg_u2_02_s* bcore_prsg_xsg_u2_02_s_gen( bcore_prsg_xsg_u2_02_s* o ){o->state = bcore_xsg2_u2( o->state );return o;}
#define TYPEOF_bcore_prsg_xsg_u2_03_s 0x4AF43F1470833276ull
#define BETH_EXPAND_ITEM_bcore_prsg_xsg_u2_03_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_xsg_u2_03_s ) \
  { \
      aware_t _; \
      u2_t state; \
  }; \
  static inline u3_t bcore_prsg_xsg_u2_03_s_gen_u3( bcore_prsg_xsg_u2_03_s* o ); \
  u3_t bcore_prsg_xsg_u2_03_s_state_bits_u3( const bcore_prsg_xsg_u2_03_s* o, sz_t bits ); \
  static inline u3_t bcore_prsg_xsg_u2_03_s_gen_bits_u3( bcore_prsg_xsg_u2_03_s* o, sz_t bits ); \
  static inline f3_t bcore_prsg_xsg_u2_03_s_state_f3( const bcore_prsg_xsg_u2_03_s* o, f3_t min, f3_t max ); \
  static inline f3_t bcore_prsg_xsg_u2_03_s_gen_f3( bcore_prsg_xsg_u2_03_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_xsg_u2_03_s_bits( const bcore_prsg_xsg_u2_03_s* o ); \
  static inline u3_t bcore_prsg_xsg_u2_03_s_max_u3( const bcore_prsg_xsg_u2_03_s* o ); \
  static inline u3_t bcore_prsg_xsg_u2_03_s_min_u3( const bcore_prsg_xsg_u2_03_s* o ); \
  static inline u3_t bcore_prsg_xsg_u2_03_s_state_u3( const bcore_prsg_xsg_u2_03_s* o ); \
  static inline bcore_prsg_xsg_u2_03_s* bcore_prsg_xsg_u2_03_s_set_state_u3( bcore_prsg_xsg_u2_03_s* o, u3_t seed ); \
  static inline bcore_prsg_xsg_u2_03_s* bcore_prsg_xsg_u2_03_s_gen( bcore_prsg_xsg_u2_03_s* o ); \
  static inline u3_t bcore_prsg_xsg_u2_03_s_gen_u3( bcore_prsg_xsg_u2_03_s* o ){return  bcore_prsg_xsg_u2_03_s_state_u3(bcore_prsg_xsg_u2_03_s_gen(o));} \
  static inline u3_t bcore_prsg_xsg_u2_03_s_gen_bits_u3( bcore_prsg_xsg_u2_03_s* o, sz_t bits ){return  bcore_prsg_xsg_u2_03_s_state_bits_u3(bcore_prsg_xsg_u2_03_s_gen(o),bits );} \
  static inline f3_t bcore_prsg_xsg_u2_03_s_state_f3( const bcore_prsg_xsg_u2_03_s* o, f3_t min, f3_t max ){return  min + ( max - min ) * ( ( f3_t )( bcore_prsg_xsg_u2_03_s_state_u3(o) - bcore_prsg_xsg_u2_03_s_min_u3(o) ) / ( bcore_prsg_xsg_u2_03_s_max_u3(o) - bcore_prsg_xsg_u2_03_s_min_u3(o) ) );} \
  static inline f3_t bcore_prsg_xsg_u2_03_s_gen_f3( bcore_prsg_xsg_u2_03_s* o, f3_t min, f3_t max ){return  bcore_prsg_xsg_u2_03_s_state_f3(bcore_prsg_xsg_u2_03_s_gen(o),min, max );} \
  static inline sz_t bcore_prsg_xsg_u2_03_s_bits( const bcore_prsg_xsg_u2_03_s* o ){return  32;} \
  static inline u3_t bcore_prsg_xsg_u2_03_s_max_u3( const bcore_prsg_xsg_u2_03_s* o ){return  0xFFFFFFFFu;} \
  static inline u3_t bcore_prsg_xsg_u2_03_s_min_u3( const bcore_prsg_xsg_u2_03_s* o ){return  1;} \
  static inline u3_t bcore_prsg_xsg_u2_03_s_state_u3( const bcore_prsg_xsg_u2_03_s* o ){return  o->state;} \
  static inline bcore_prsg_xsg_u2_03_s* bcore_prsg_xsg_u2_03_s_set_state_u3( bcore_prsg_xsg_u2_03_s* o, u3_t seed ){o->state = u2_max( 1, seed ^ ( seed >> 32 ) );return o;} \
  static inline bcore_prsg_xsg_u2_03_s* bcore_prsg_xsg_u2_03_s_gen( bcore_prsg_xsg_u2_03_s* o ){o->state = bcore_xsg3_u2( o->state );return o;}
#define BETH_EXPAND_GROUP_bcore_prsg_xsg \
  BCORE_FORWARD_OBJECT( bcore_prsg_xsg ); \
  BCORE_FORWARD_OBJECT( bcore_prsg_xsg_u2_00_s ); \
  BCORE_FORWARD_OBJECT( bcore_prsg_xsg_u2_01_s ); \
  BCORE_FORWARD_OBJECT( bcore_prsg_xsg_u2_02_s ); \
  BCORE_FORWARD_OBJECT( bcore_prsg_xsg_u2_03_s ); \
  XOILA_DECLARE_SPECT( bcore_prsg_xsg ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bcore_prsg_xsg ) \
  BETH_EXPAND_ITEM_bcore_prsg_xsg_u2_00_s \
  BETH_EXPAND_ITEM_bcore_prsg_xsg_u2_01_s \
  BETH_EXPAND_ITEM_bcore_prsg_xsg_u2_02_s \
  BETH_EXPAND_ITEM_bcore_prsg_xsg_u2_03_s

/**********************************************************************************************************************/
// source: bcore_shell.h

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_shell

#define TYPEOF_bcore_shell 0x2877B9FE4C6D99A7ull
#define TYPEOF_bcore_shell_spect_s 0xF1C3CEFCE7D4C897ull
#define TYPEOF_x_inst 0xFC4EFF7569E22558ull
#define TYPEOF_bcore_main_frame_s 0x32D158C73073F54Eull
#define TYPEOF_bcore_shell_control_s 0x65568789B6F2BDAFull
#define BETH_EXPAND_GROUP_bcore_shell \
  BCORE_FORWARD_OBJECT( bcore_shell ); \
  BCORE_FORWARD_OBJECT( x_inst ); \
  BCORE_FORWARD_OBJECT( bcore_main_frame_s ); \
  BCORE_FORWARD_OBJECT( bcore_shell_control_s ); \
  BCORE_FORWARD_OBJECT( bcore_shell_op ); \
  BCORE_FORWARD_OBJECT( bcore_shell_control ); \
  BCORE_FORWARD_OBJECT( bcore_shell_op_default ); \
  bcore_arr_tp_s* bcore_shell_get_op_stamps( const bcore_shell* o ); \
  void bcore_shell_help_to_sink( bcore_shell* o, const bcore_shell_control_s* control, bcore_sink* sink ); \
  bl_t bcore_shell_run_expression( bcore_shell* o, const bcore_main_frame_s* frame, bcore_shell_control_s* control, x_source* expression ); \
  er_t bcore_shell_loop( bcore_shell* o, const bcore_main_frame_s* frame, bcore_shell_control_s* control ); \
  typedef bl_t (*bcore_shell_loop_callback)(bcore_shell* o ); \
  typedef tp_t (*bcore_shell_op_group)(const bcore_shell* o ); \
  typedef void (*bcore_shell_push_op_groups)(const bcore_shell* o, bcore_arr_tp_s* list ); \
  XOILA_DECLARE_SPECT( bcore_shell ) \
  { \
      bcore_spect_header_s header; \
      bcore_shell_loop_callback loop_callback; \
      bcore_shell_op_group op_group; \
      bcore_shell_push_op_groups push_op_groups; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bcore_shell ) \
  static inline bl_t bcore_shell_a_loop_callback( bcore_shell* o ){ const bcore_shell_spect_s* p = bcore_shell_spect_s_get_aware( o ); assert( p->loop_callback ); return p->loop_callback( o );} \
  static inline bl_t bcore_shell_defines_loop_callback( const bcore_shell* o ){ return  true;} \
  static inline bl_t bcore_shell_loop_callback_default( bcore_shell* o ){return  false;} \
  BETH_EXPAND_GROUP_bcore_shell_op \
  BETH_EXPAND_GROUP_bcore_shell_control \
  static inline tp_t bcore_shell_a_op_group( const bcore_shell* o ){ const bcore_shell_spect_s* p = bcore_shell_spect_s_get_aware( o ); assert( p->op_group ); return p->op_group( o );} \
  static inline bl_t bcore_shell_defines_op_group( const bcore_shell* o ){ return  true;} \
  static inline tp_t bcore_shell_op_group_default( const bcore_shell* o ){return  ((tp_t)(TYPEOF_bcore_shell_op_default));} \
  static inline void bcore_shell_a_push_op_groups( const bcore_shell* o, bcore_arr_tp_s* list ){ const bcore_shell_spect_s* p = bcore_shell_spect_s_get_aware( o ); assert( p->push_op_groups ); p->push_op_groups( o, list );} \
  static inline bl_t bcore_shell_defines_push_op_groups( const bcore_shell* o ){ return  true;} \
  void bcore_shell_push_op_groups_default( const bcore_shell* o, bcore_arr_tp_s* list ); \
  BETH_EXPAND_GROUP_bcore_shell_op_default

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_shell_op

#define TYPEOF_bcore_shell_op 0xAC946BFF99A94E2Full
#define TYPEOF_bcore_shell_op_spect_s 0x5C8AC70A55050C9Full
#define BETH_EXPAND_GROUP_bcore_shell_op \
  BCORE_FORWARD_OBJECT( bcore_shell_op ); \
  bl_t bcore_shell_op_key_match( const bcore_shell_op* o, sc_t key ); \
  bl_t bcore_shell_op_parse_match( bcore_shell_op* o, x_source* source ); \
  bl_t bcore_shell_op_parse_param( bcore_shell_op* o, x_source* source, bcore_sink* sink ); \
  void bcore_shell_op_arg_signature_to_sink( bcore_shell_op* o, bcore_sink* sink ); \
  typedef sc_t (*bcore_shell_op_key)(const bcore_shell_op* o ); \
  typedef void (*bcore_shell_op_get_key)(const bcore_shell_op* o, st_s* key ); \
  typedef sc_t (*bcore_shell_op_info)(const bcore_shell_op* o ); \
  typedef void (*bcore_shell_op_get_info)(const bcore_shell_op* o, st_s* info ); \
  typedef void (*bcore_shell_op_run)(bcore_shell_op* o, bcore_shell* obj, const bcore_main_frame_s* main_frame, x_source* source, bcore_sink* sink, bcore_shell_control_s* control ); \
  XOILA_DECLARE_SPECT( bcore_shell_op ) \
  { \
      bcore_spect_header_s header; \
      bcore_shell_op_key key; \
      bcore_shell_op_get_key get_key; \
      bcore_shell_op_info info; \
      bcore_shell_op_get_info get_info; \
      bcore_shell_op_run run; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bcore_shell_op ) \
  static inline sc_t bcore_shell_op_a_key( const bcore_shell_op* o ){ const bcore_shell_op_spect_s* p = bcore_shell_op_spect_s_get_aware( o ); assert( p->key ); return p->key( o );} \
  static inline bl_t bcore_shell_op_defines_key( const bcore_shell_op* o ){  return bcore_shell_op_spect_s_get_aware( o )->key != NULL;} \
  static inline void bcore_shell_op_a_get_key( const bcore_shell_op* o, st_s* key ){ const bcore_shell_op_spect_s* p = bcore_shell_op_spect_s_get_aware( o ); assert( p->get_key ); p->get_key( o, key );} \
  static inline bl_t bcore_shell_op_defines_get_key( const bcore_shell_op* o ){ return  true;} \
  static inline void bcore_shell_op_get_key_default( const bcore_shell_op* o, st_s* key ){st_s_copy_sc(key,bcore_shell_op_a_key(o) );} \
  static inline sc_t bcore_shell_op_a_info( const bcore_shell_op* o ){ const bcore_shell_op_spect_s* p = bcore_shell_op_spect_s_get_aware( o ); assert( p->info ); return p->info( o );} \
  static inline bl_t bcore_shell_op_defines_info( const bcore_shell_op* o ){  return bcore_shell_op_spect_s_get_aware( o )->info != NULL;} \
  static inline void bcore_shell_op_a_get_info( const bcore_shell_op* o, st_s* info ){ const bcore_shell_op_spect_s* p = bcore_shell_op_spect_s_get_aware( o ); assert( p->get_info ); p->get_info( o, info );} \
  static inline bl_t bcore_shell_op_defines_get_info( const bcore_shell_op* o ){ return  true;} \
  static inline void bcore_shell_op_get_info_default( const bcore_shell_op* o, st_s* info ){st_s_copy_sc(info,bcore_shell_op_a_info(o) );} \
  static inline void bcore_shell_op_a_run( bcore_shell_op* o, bcore_shell* obj, const bcore_main_frame_s* main_frame, x_source* source, bcore_sink* sink, bcore_shell_control_s* control ){ const bcore_shell_op_spect_s* p = bcore_shell_op_spect_s_get_aware( o ); assert( p->run ); p->run( o, obj, main_frame, source, sink, control );} \
  static inline bl_t bcore_shell_op_defines_run( const bcore_shell_op* o ){  return bcore_shell_op_spect_s_get_aware( o )->run != NULL;}

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_shell_control

#define TYPEOF_bcore_shell_control 0x07411255F31A995Dull
#define TYPEOF_bcore_shell_control_spect_s 0xCF41737BCBB25259ull
#define TYPEOF_bcore_shell_control_s 0x65568789B6F2BDAFull
#define BETH_EXPAND_ITEM_bcore_shell_control_s \
  BCORE_DECLARE_OBJECT( bcore_shell_control_s ) \
  { \
      aware_t _; \
      bcore_shell_control_s* parent; \
      x_sink* sink; \
      x_source* source; \
      bcore_hmap_name_s hmap_name; \
      x_hmap_tp_st_s hmap_alias; \
      bl_t exit_loop; \
      st_s path; \
      st_s prompt; \
  }; \
  static inline bcore_shell_control_s* bcore_shell_control_s_reset( bcore_shell_control_s* o ); \
  static inline void bcore_shell_control_s_request_exit_loop( bcore_shell_control_s* o ); \
  static inline void bcore_shell_control_s_request_exit_all( bcore_shell_control_s* o ); \
  static inline bl_t bcore_shell_control_s_exit_loop( const bcore_shell_control_s* o ); \
  static inline bcore_shell_control_s* bcore_shell_control_s_spawn( bcore_shell_control_s* o ); \
  static inline bcore_shell_control_s* bcore_shell_control_s_reset( bcore_shell_control_s* o ){o->exit_loop = false;return o;} \
  static inline void bcore_shell_control_s_request_exit_loop( bcore_shell_control_s* o ){o->exit_loop = true;} \
  static inline void bcore_shell_control_s_request_exit_all( bcore_shell_control_s* o ){ o->exit_loop = true; if( o->parent ) bcore_shell_control_s_request_exit_all(o->parent);} \
  static inline bl_t bcore_shell_control_s_exit_loop( const bcore_shell_control_s* o ){return  o->exit_loop;} \
  static inline bcore_shell_control_s* bcore_shell_control_s_spawn( bcore_shell_control_s* o ){ bcore_shell_control_s* r = bcore_shell_control_s_clone(o); r->parent = o; return  r;}
#define BETH_EXPAND_GROUP_bcore_shell_control \
  BCORE_FORWARD_OBJECT( bcore_shell_control ); \
  BCORE_FORWARD_OBJECT( bcore_shell_control_s ); \
  XOILA_DECLARE_SPECT( bcore_shell_control ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bcore_shell_control ) \
  BETH_EXPAND_ITEM_bcore_shell_control_s

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_shell_op_default

#define TYPEOF_bcore_shell_op_default 0x377F955C9208880Dull
#define TYPEOF_bcore_shell_op_default_spect_s 0xD8576D49161AE849ull
#define TYPEOF_bcore_shell_op_default_help_s 0xF75CEDD411DE13D3ull
#define BETH_EXPAND_ITEM_bcore_shell_op_default_help_s \
  BCORE_DECLARE_OBJECT( bcore_shell_op_default_help_s ) \
  { \
      aware_t _; \
  }; \
  static inline sc_t bcore_shell_op_default_help_s_key( const bcore_shell_op_default_help_s* o ); \
  static inline sc_t bcore_shell_op_default_help_s_info( const bcore_shell_op_default_help_s* o ); \
  static inline void bcore_shell_op_default_help_s_run( bcore_shell_op_default_help_s* o, bcore_shell* obj, const bcore_main_frame_s* main_frame, x_source* source, bcore_sink* sink, bcore_shell_control_s* control ); \
  static inline sc_t bcore_shell_op_default_help_s_key( const bcore_shell_op_default_help_s* o ){return  "?,help";} \
  static inline sc_t bcore_shell_op_default_help_s_info( const bcore_shell_op_default_help_s* o ){return  "Lists available commands";} \
  static inline void bcore_shell_op_default_help_s_run( bcore_shell_op_default_help_s* o, bcore_shell* obj, const bcore_main_frame_s* main_frame, x_source* source, bcore_sink* sink, bcore_shell_control_s* control ){bcore_shell_help_to_sink(obj,control, sink );}
#define TYPEOF_bcore_shell_op_default_ls_s 0xDC836302CF3C1A21ull
#define BETH_EXPAND_ITEM_bcore_shell_op_default_ls_s \
  BCORE_DECLARE_OBJECT( bcore_shell_op_default_ls_s ) \
  { \
      aware_t _; \
      st_s path; \
  }; \
  static inline void bcore_shell_op_default_ls_s_get_key( const bcore_shell_op_default_ls_s* o, st_s* key ); \
  static inline void bcore_shell_op_default_ls_s_get_info( const bcore_shell_op_default_ls_s* o, st_s* info ); \
  void bcore_shell_op_default_ls_s_run( bcore_shell_op_default_ls_s* o, bcore_shell* obj, const bcore_main_frame_s* main_frame, x_source* source, bcore_sink* sink, bcore_shell_control_s* control ); \
  static inline void bcore_shell_op_default_ls_s_get_key( const bcore_shell_op_default_ls_s* o, st_s* key ){st_s_push_fa(key,"ls" );} \
  static inline void bcore_shell_op_default_ls_s_get_info( const bcore_shell_op_default_ls_s* o, st_s* info ){st_s_push_fa(info,"Lists all visible members" );}
#define TYPEOF_bcore_shell_op_default_enter_s 0xAA8F66CF9A301BFAull
#define BETH_EXPAND_ITEM_bcore_shell_op_default_enter_s \
  BCORE_DECLARE_OBJECT( bcore_shell_op_default_enter_s ) \
  { \
      aware_t _; \
      st_s path; \
  }; \
  static inline sc_t bcore_shell_op_default_enter_s_key( const bcore_shell_op_default_enter_s* o ); \
  static inline sc_t bcore_shell_op_default_enter_s_info( const bcore_shell_op_default_enter_s* o ); \
  void bcore_shell_op_default_enter_s_run( bcore_shell_op_default_enter_s* o, bcore_shell* obj, const bcore_main_frame_s* main_frame, x_source* source, bcore_sink* sink, bcore_shell_control_s* control ); \
  static inline sc_t bcore_shell_op_default_enter_s_key( const bcore_shell_op_default_enter_s* o ){return  "n,enter";} \
  static inline sc_t bcore_shell_op_default_enter_s_info( const bcore_shell_op_default_enter_s* o ){return  "Enters object";}
#define TYPEOF_bcore_shell_op_default_exit_s 0xB8E52C4CA484C9E0ull
#define BETH_EXPAND_ITEM_bcore_shell_op_default_exit_s \
  BCORE_DECLARE_OBJECT( bcore_shell_op_default_exit_s ) \
  { \
      aware_t _; \
      bl_t a; \
  }; \
  static inline sc_t bcore_shell_op_default_exit_s_key( const bcore_shell_op_default_exit_s* o ); \
  static inline sc_t bcore_shell_op_default_exit_s_info( const bcore_shell_op_default_exit_s* o ); \
  static inline void bcore_shell_op_default_exit_s_run( bcore_shell_op_default_exit_s* o, bcore_shell* obj, const bcore_main_frame_s* main_frame, x_source* source, bcore_sink* sink, bcore_shell_control_s* control ); \
  static inline sc_t bcore_shell_op_default_exit_s_key( const bcore_shell_op_default_exit_s* o ){return  "x,exit";} \
  static inline sc_t bcore_shell_op_default_exit_s_info( const bcore_shell_op_default_exit_s* o ){return  "Exits object; -a: exits all parent objects";} \
  static inline void bcore_shell_op_default_exit_s_run( bcore_shell_op_default_exit_s* o, bcore_shell* obj, const bcore_main_frame_s* main_frame, x_source* source, bcore_sink* sink, bcore_shell_control_s* control ){ if( o->a ) bcore_shell_control_s_request_exit_all(control); else bcore_shell_control_s_request_exit_loop(control);}
#define TYPEOF_bcore_shell_op_default_get_s 0x6D41BBBB174FE26Cull
#define BETH_EXPAND_ITEM_bcore_shell_op_default_get_s \
  BCORE_DECLARE_OBJECT( bcore_shell_op_default_get_s ) \
  { \
      aware_t _; \
      st_s path; \
  }; \
  static inline sc_t bcore_shell_op_default_get_s_key( const bcore_shell_op_default_get_s* o ); \
  static inline sc_t bcore_shell_op_default_get_s_info( const bcore_shell_op_default_get_s* o ); \
  void bcore_shell_op_default_get_s_run( bcore_shell_op_default_get_s* o, bcore_shell* obj, const bcore_main_frame_s* main_frame, x_source* source, bcore_sink* sink, bcore_shell_control_s* control ); \
  static inline sc_t bcore_shell_op_default_get_s_key( const bcore_shell_op_default_get_s* o ){return  "get";} \
  static inline sc_t bcore_shell_op_default_get_s_info( const bcore_shell_op_default_get_s* o ){return  "Outputs object as btml";}
#define TYPEOF_bcore_shell_op_default_set_s 0xA537CD7AFBBEEAB0ull
#define BETH_EXPAND_ITEM_bcore_shell_op_default_set_s \
  BCORE_DECLARE_OBJECT( bcore_shell_op_default_set_s ) \
  { \
      aware_t _; \
      st_s path; \
      st_s source; \
  }; \
  static inline sc_t bcore_shell_op_default_set_s_key( const bcore_shell_op_default_set_s* o ); \
  static inline sc_t bcore_shell_op_default_set_s_info( const bcore_shell_op_default_set_s* o ); \
  void bcore_shell_op_default_set_s_run( bcore_shell_op_default_set_s* o, bcore_shell* obj, const bcore_main_frame_s* main_frame, x_source* source, bcore_sink* sink, bcore_shell_control_s* control ); \
  static inline sc_t bcore_shell_op_default_set_s_key( const bcore_shell_op_default_set_s* o ){return  "set";} \
  static inline sc_t bcore_shell_op_default_set_s_info( const bcore_shell_op_default_set_s* o ){return  "Sets object from btml";}
#define TYPEOF_bcore_shell_op_default_alias_s 0x69A57BFFEEEF3DCCull
#define BETH_EXPAND_ITEM_bcore_shell_op_default_alias_s \
  BCORE_DECLARE_OBJECT( bcore_shell_op_default_alias_s ) \
  { \
      aware_t _; \
      st_s key; \
      st_s expression; \
  }; \
  static inline sc_t bcore_shell_op_default_alias_s_key( const bcore_shell_op_default_alias_s* o ); \
  static inline sc_t bcore_shell_op_default_alias_s_info( const bcore_shell_op_default_alias_s* o ); \
  void bcore_shell_op_default_alias_s_run( bcore_shell_op_default_alias_s* o, bcore_shell* obj, const bcore_main_frame_s* main_frame, x_source* source, bcore_sink* sink, bcore_shell_control_s* control ); \
  static inline sc_t bcore_shell_op_default_alias_s_key( const bcore_shell_op_default_alias_s* o ){return  "alias";} \
  static inline sc_t bcore_shell_op_default_alias_s_info( const bcore_shell_op_default_alias_s* o ){return  "Defines an alias.";}
#define TYPEOF_bcore_shell_op_default_stdin_s 0xF0D50FC4F17FF346ull
#define BETH_EXPAND_ITEM_bcore_shell_op_default_stdin_s \
  BCORE_DECLARE_OBJECT( bcore_shell_op_default_stdin_s ) \
  { \
      aware_t _; \
  }; \
  static inline sc_t bcore_shell_op_default_stdin_s_key( const bcore_shell_op_default_stdin_s* o ); \
  static inline sc_t bcore_shell_op_default_stdin_s_info( const bcore_shell_op_default_stdin_s* o ); \
  static inline void bcore_shell_op_default_stdin_s_run( bcore_shell_op_default_stdin_s* o, bcore_shell* obj, const bcore_main_frame_s* main_frame, x_source* source, bcore_sink* sink, bcore_shell_control_s* control ); \
  static inline sc_t bcore_shell_op_default_stdin_s_key( const bcore_shell_op_default_stdin_s* o ){return  "stdin";} \
  static inline sc_t bcore_shell_op_default_stdin_s_info( const bcore_shell_op_default_stdin_s* o ){return  "Sets shell-control source to stdin";} \
  static inline void bcore_shell_op_default_stdin_s_run( bcore_shell_op_default_stdin_s* o, bcore_shell* obj, const bcore_main_frame_s* main_frame, x_source* source, bcore_sink* sink, bcore_shell_control_s* control ){x_source_a_attach( &(control->source ), (x_source*)( ((x_source*)bcore_fork(x_source_stdin()))));}
#define TYPEOF_bcore_shell_op_default_stdout_s 0xF43BEAC2442C5E6Bull
#define BETH_EXPAND_ITEM_bcore_shell_op_default_stdout_s \
  BCORE_DECLARE_OBJECT( bcore_shell_op_default_stdout_s ) \
  { \
      aware_t _; \
  }; \
  static inline sc_t bcore_shell_op_default_stdout_s_key( const bcore_shell_op_default_stdout_s* o ); \
  static inline sc_t bcore_shell_op_default_stdout_s_info( const bcore_shell_op_default_stdout_s* o ); \
  static inline void bcore_shell_op_default_stdout_s_run( bcore_shell_op_default_stdout_s* o, bcore_shell* obj, const bcore_main_frame_s* main_frame, x_source* source, bcore_sink* sink, bcore_shell_control_s* control ); \
  static inline sc_t bcore_shell_op_default_stdout_s_key( const bcore_shell_op_default_stdout_s* o ){return  "stdout";} \
  static inline sc_t bcore_shell_op_default_stdout_s_info( const bcore_shell_op_default_stdout_s* o ){return  "Sets shell-control sink to stdout";} \
  static inline void bcore_shell_op_default_stdout_s_run( bcore_shell_op_default_stdout_s* o, bcore_shell* obj, const bcore_main_frame_s* main_frame, x_source* source, bcore_sink* sink, bcore_shell_control_s* control ){x_sink_a_attach( &(control->sink ), (x_sink*)( ((x_sink*)bcore_fork(x_sink_stdout()))));}
#define BETH_EXPAND_GROUP_bcore_shell_op_default \
  BCORE_FORWARD_OBJECT( bcore_shell_op_default ); \
  BCORE_FORWARD_OBJECT( bcore_shell_op_default_help_s ); \
  BCORE_FORWARD_OBJECT( bcore_shell_op_default_ls_s ); \
  BCORE_FORWARD_OBJECT( bcore_shell_op_default_enter_s ); \
  BCORE_FORWARD_OBJECT( bcore_shell_op_default_exit_s ); \
  BCORE_FORWARD_OBJECT( bcore_shell_op_default_get_s ); \
  BCORE_FORWARD_OBJECT( bcore_shell_op_default_set_s ); \
  BCORE_FORWARD_OBJECT( bcore_shell_op_default_alias_s ); \
  BCORE_FORWARD_OBJECT( bcore_shell_op_default_stdin_s ); \
  BCORE_FORWARD_OBJECT( bcore_shell_op_default_stdout_s ); \
  XOILA_DECLARE_SPECT( bcore_shell_op_default ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bcore_shell_op_default ) \
  BETH_EXPAND_ITEM_bcore_shell_op_default_help_s \
  BETH_EXPAND_ITEM_bcore_shell_op_default_ls_s \
  BETH_EXPAND_ITEM_bcore_shell_op_default_enter_s \
  BETH_EXPAND_ITEM_bcore_shell_op_default_exit_s \
  BETH_EXPAND_ITEM_bcore_shell_op_default_get_s \
  BETH_EXPAND_ITEM_bcore_shell_op_default_set_s \
  BETH_EXPAND_ITEM_bcore_shell_op_default_alias_s \
  BETH_EXPAND_ITEM_bcore_shell_op_default_stdin_s \
  BETH_EXPAND_ITEM_bcore_shell_op_default_stdout_s

/**********************************************************************************************************************/
// source: bcore_huffman.h

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_huffman; embeds: bcore_huffman.x

#define TYPEOF_bcore_huffman 0xFB46531DBC90DE4Cull
#define TYPEOF_bcore_huffman_spect_s 0x271965DD8E680584ull
#define TYPEOF_bcore_huffman_codec_s 0x7DBD1C5E923A8433ull
#define BETH_EXPAND_ITEM_bcore_huffman_codec_s \
  BCORE_DECLARE_OBJECT( bcore_huffman_codec_s ) \
  { \
      aware_t _; \
      bcore_huffman_hist_s* hist; \
      bcore_huffman_count_map_s* count_map; \
      bcore_huffman_tree_s* tree; \
      bcore_huffman_index_s* leaf_index; \
  }; \
  bcore_huffman_codec_s* bcore_huffman_codec_s_clear( bcore_huffman_codec_s* o ); \
  bcore_huffman_codec_s* bcore_huffman_codec_s_scan_start( bcore_huffman_codec_s* o ); \
  bcore_huffman_codec_s* bcore_huffman_codec_s_scan_u2( bcore_huffman_codec_s* o, u2_t val ); \
  bcore_huffman_codec_s* bcore_huffman_codec_s_scan_u3( bcore_huffman_codec_s* o, u3_t val ); \
  static inline bcore_huffman_codec_s* bcore_huffman_codec_s_scan_s2( bcore_huffman_codec_s* o, s2_t val ); \
  static inline bcore_huffman_codec_s* bcore_huffman_codec_s_scan_s3( bcore_huffman_codec_s* o, s3_t val ); \
  bcore_huffman_codec_s* bcore_huffman_codec_s_scan_end( bcore_huffman_codec_s* o ); \
  const bcore_huffman_codec_s* bcore_huffman_codec_s_encode_u2( const bcore_huffman_codec_s* o, u2_t val, bcore_huffman_bit_buffer_s* bit_buffer ); \
  u2_t bcore_huffman_codec_s_decode_u2( const bcore_huffman_codec_s* o, bcore_huffman_bit_buffer_iterator_s* iterator ); \
  const bcore_huffman_codec_s* bcore_huffman_codec_s_encode_u3( const bcore_huffman_codec_s* o, u3_t val, bcore_huffman_bit_buffer_s* bit_buffer ); \
  u3_t bcore_huffman_codec_s_decode_u3( const bcore_huffman_codec_s* o, bcore_huffman_bit_buffer_iterator_s* iterator ); \
  static inline const bcore_huffman_codec_s* bcore_huffman_codec_s_encode_s2( const bcore_huffman_codec_s* o, s2_t val, bcore_huffman_bit_buffer_s* bit_buffer ); \
  static inline const bcore_huffman_codec_s* bcore_huffman_codec_s_encode_s3( const bcore_huffman_codec_s* o, s3_t val, bcore_huffman_bit_buffer_s* bit_buffer ); \
  static inline s2_t bcore_huffman_codec_s_decode_s2( const bcore_huffman_codec_s* o, bcore_huffman_bit_buffer_iterator_s* iterator ); \
  static inline s3_t bcore_huffman_codec_s_decode_s3( const bcore_huffman_codec_s* o, bcore_huffman_bit_buffer_iterator_s* iterator ); \
  const bcore_huffman_codec_s* bcore_huffman_codec_s_encode( const bcore_huffman_codec_s* o, bcore_huffman_bit_buffer_s* bit_buffer ); \
  bcore_huffman_codec_s* bcore_huffman_codec_s_decode( bcore_huffman_codec_s* o, bcore_huffman_bit_buffer_iterator_s* iterator ); \
  static inline bcore_huffman_codec_s* bcore_huffman_codec_s_scan_s2( bcore_huffman_codec_s* o, s2_t val ){ return  bcore_huffman_codec_s_scan_u2(o,bcore_huffman_u2_from_s2(val ) );} \
  static inline bcore_huffman_codec_s* bcore_huffman_codec_s_scan_s3( bcore_huffman_codec_s* o, s3_t val ){ return  bcore_huffman_codec_s_scan_u3(o,bcore_huffman_u3_from_s3(val ) );} \
  static inline const bcore_huffman_codec_s* bcore_huffman_codec_s_encode_s2( const bcore_huffman_codec_s* o, s2_t val, bcore_huffman_bit_buffer_s* bit_buffer ){ return  bcore_huffman_codec_s_encode_u2(o,bcore_huffman_u2_from_s2(val ), bit_buffer );} \
  static inline const bcore_huffman_codec_s* bcore_huffman_codec_s_encode_s3( const bcore_huffman_codec_s* o, s3_t val, bcore_huffman_bit_buffer_s* bit_buffer ){ return  bcore_huffman_codec_s_encode_u3(o,bcore_huffman_u3_from_s3(val ), bit_buffer );} \
  static inline s2_t bcore_huffman_codec_s_decode_s2( const bcore_huffman_codec_s* o, bcore_huffman_bit_buffer_iterator_s* iterator ){ return  bcore_huffman_s2_from_u2(bcore_huffman_codec_s_decode_u2(o,iterator ) );} \
  static inline s3_t bcore_huffman_codec_s_decode_s3( const bcore_huffman_codec_s* o, bcore_huffman_bit_buffer_iterator_s* iterator ){ return  bcore_huffman_s3_from_u3(bcore_huffman_codec_s_decode_u3(o,iterator ) );}
#define TYPEOF_bcore_huffman_bit_buffer_s 0xDF0886D237A7C6A1ull
#define BETH_EXPAND_ITEM_bcore_huffman_bit_buffer_s \
  BCORE_DECLARE_OBJECT( bcore_huffman_bit_buffer_s ) \
  { \
      aware_t _; \
      u3_t bits; \
      BCORE_ARRAY_DYN_SOLID_STATIC_S( u0_t, ); \
  }; \
  static inline void bcore_huffman_bit_buffer_s_bbml_body_to_sink( const bcore_huffman_bit_buffer_s* o, x_sink* sink ); \
  static inline er_t bcore_huffman_bit_buffer_s_bbml_body_from_source( bcore_huffman_bit_buffer_s* o, x_source* source ); \
  bcore_huffman_bit_buffer_s* bcore_huffman_bit_buffer_s_clear( bcore_huffman_bit_buffer_s* o ); \
  bcore_huffman_bit_buffer_s* bcore_huffman_bit_buffer_s_push_bl( bcore_huffman_bit_buffer_s* o, bl_t bit ); \
  bcore_huffman_bit_buffer_s* bcore_huffman_bit_buffer_s_push_u3( bcore_huffman_bit_buffer_s* o, u3_t val, sz_t bits ); \
  bcore_huffman_bit_buffer_s* bcore_huffman_bit_buffer_s_push_packed_u3( bcore_huffman_bit_buffer_s* o, u3_t val ); \
  bcore_huffman_bit_buffer_s* bcore_huffman_bit_buffer_s_push_s3( bcore_huffman_bit_buffer_s* o, s3_t val, sz_t bits ); \
  bcore_huffman_bit_buffer_s* bcore_huffman_bit_buffer_s_push_packed_s3( bcore_huffman_bit_buffer_s* o, s3_t val ); \
  bcore_huffman_bit_buffer_s* bcore_huffman_bit_buffer_s_push_f3( bcore_huffman_bit_buffer_s* o, f3_t v, sz_t bits ); \
  bcore_huffman_bit_buffer_s* bcore_huffman_bit_buffer_s_push_bit_buffer( bcore_huffman_bit_buffer_s* o, const bcore_huffman_bit_buffer_s* src ); \
  tp_t bcore_huffman_bit_buffer_s_get_hash( const bcore_huffman_bit_buffer_s* o ); \
  void bcore_huffman_bit_buffer_s_bcml_body_to_sink( const bcore_huffman_bit_buffer_s* o, x_sink* sink ); \
  er_t bcore_huffman_bit_buffer_s_bcml_body_from_source( bcore_huffman_bit_buffer_s* o, x_source* source ); \
  static inline void bcore_huffman_bit_buffer_s_bbml_body_to_sink( const bcore_huffman_bit_buffer_s* o, x_sink* sink ){bcore_huffman_bit_buffer_s_bcml_body_to_sink(o,sink );} \
  static inline er_t bcore_huffman_bit_buffer_s_bbml_body_from_source( bcore_huffman_bit_buffer_s* o, x_source* source ){return  bcore_huffman_bit_buffer_s_bcml_body_from_source(o,source );}
#define TYPEOF_bcore_huffman_bit_buffer_iterator_s 0x093943E0EF30024Cull
#define BETH_EXPAND_ITEM_bcore_huffman_bit_buffer_iterator_s \
  BCORE_DECLARE_OBJECT( bcore_huffman_bit_buffer_iterator_s ) \
  { \
      aware_t _; \
      bcore_huffman_bit_buffer_s* bit_buffer; \
      sz_t bit_index; \
  }; \
  static inline bcore_huffman_bit_buffer_iterator_s* bcore_huffman_bit_buffer_iterator_s_reset( bcore_huffman_bit_buffer_iterator_s* o ); \
  static inline bcore_huffman_bit_buffer_iterator_s* bcore_huffman_bit_buffer_iterator_s_setup( bcore_huffman_bit_buffer_iterator_s* o, const bcore_huffman_bit_buffer_s* bit_buffer ); \
  static inline bl_t bcore_huffman_bit_buffer_iterator_s_eos( bcore_huffman_bit_buffer_iterator_s* o ); \
  bl_t bcore_huffman_bit_buffer_iterator_s_read_bl( bcore_huffman_bit_buffer_iterator_s* o ); \
  u3_t bcore_huffman_bit_buffer_iterator_s_read_u3( bcore_huffman_bit_buffer_iterator_s* o, sz_t bits ); \
  u3_t bcore_huffman_bit_buffer_iterator_s_read_packed_u3( bcore_huffman_bit_buffer_iterator_s* o ); \
  s3_t bcore_huffman_bit_buffer_iterator_s_read_s3( bcore_huffman_bit_buffer_iterator_s* o, sz_t bits ); \
  u3_t bcore_huffman_bit_buffer_iterator_s_read_packed_s3( bcore_huffman_bit_buffer_iterator_s* o ); \
  f3_t bcore_huffman_bit_buffer_iterator_s_read_f3( bcore_huffman_bit_buffer_iterator_s* o, sz_t bits ); \
  static inline bcore_huffman_bit_buffer_iterator_s* bcore_huffman_bit_buffer_iterator_s_reset( bcore_huffman_bit_buffer_iterator_s* o ){ o->bit_index = 0; return  o;} \
  static inline bcore_huffman_bit_buffer_iterator_s* bcore_huffman_bit_buffer_iterator_s_setup( bcore_huffman_bit_buffer_iterator_s* o, const bcore_huffman_bit_buffer_s* bit_buffer ){ bcore_huffman_bit_buffer_iterator_s_reset(o); o->bit_buffer = ((bcore_huffman_bit_buffer_s*)(bit_buffer)); return  o;} \
  static inline bl_t bcore_huffman_bit_buffer_iterator_s_eos( bcore_huffman_bit_buffer_iterator_s* o ){ return  o->bit_index >= o->bit_buffer->bits;}
#define TYPEOF_bcore_huffman_hist_s 0x12C3EA9E8F1C4823ull
#define BETH_EXPAND_ITEM_bcore_huffman_hist_s \
  BCORE_DECLARE_OBJECT( bcore_huffman_hist_s ) \
  { \
      aware_t _; \
      bcore_hmap_tpuz_s hmap_tpuz; \
  }; \
  static inline bcore_huffman_hist_s* bcore_huffman_hist_s_clear( bcore_huffman_hist_s* o ); \
  static inline bcore_huffman_hist_s* bcore_huffman_hist_s_count( bcore_huffman_hist_s* o, u2_t val ); \
  static inline uz_t bcore_huffman_hist_s_get( const bcore_huffman_hist_s* o, u2_t val ); \
  static inline tp_t bcore_huffman_hist_s_key( const bcore_huffman_hist_s* o, u2_t val ); \
  static inline u2_t bcore_huffman_hist_s_val( const bcore_huffman_hist_s* o, tp_t key ); \
  static inline bcore_huffman_hist_s* bcore_huffman_hist_s_clear( bcore_huffman_hist_s* o ){ bcore_hmap_tpuz_s_clear(&(o->hmap_tpuz)); return  o;} \
  static inline bcore_huffman_hist_s* bcore_huffman_hist_s_count( bcore_huffman_hist_s* o, u2_t val ){ (*(bcore_hmap_tpuz_s_fget(&(o->hmap_tpuz),bcore_huffman_hist_s_key(o,val ), 0 )))++; return  o;} \
  static inline uz_t bcore_huffman_hist_s_get( const bcore_huffman_hist_s* o, u2_t val ){ const uz_t* p = bcore_hmap_tpuz_s_get(&(o->hmap_tpuz),bcore_huffman_hist_s_key(o,val ) ); return  p ? (*(p)) : 0;} \
  static inline tp_t bcore_huffman_hist_s_key( const bcore_huffman_hist_s* o, u2_t val ){ return  ((tp_t)(val)) + 1;} \
  static inline u2_t bcore_huffman_hist_s_val( const bcore_huffman_hist_s* o, tp_t key ){ return  key - 1;}
#define TYPEOF_bcore_huffman_index_s 0x8FF2EB05D6F10D89ull
#define BETH_EXPAND_ITEM_bcore_huffman_index_s \
  BCORE_DECLARE_OBJECT( bcore_huffman_index_s ) \
  { \
      aware_t _; \
      bcore_hmap_tpuz_s hmap_tpuz; \
  }; \
  static inline tp_t bcore_huffman_index_s_key( const bcore_huffman_index_s* o, u2_t val ); \
  static inline bcore_huffman_index_s* bcore_huffman_index_s_clear( bcore_huffman_index_s* o ); \
  static inline bcore_huffman_index_s* bcore_huffman_index_s_set( bcore_huffman_index_s* o, u2_t val, uz_t index ); \
  static inline uz_t bcore_huffman_index_s_get( const bcore_huffman_index_s* o, u2_t val ); \
  static inline tp_t bcore_huffman_index_s_key( const bcore_huffman_index_s* o, u2_t val ){ return  ((tp_t)(val)) + 1;} \
  static inline bcore_huffman_index_s* bcore_huffman_index_s_clear( bcore_huffman_index_s* o ){ bcore_hmap_tpuz_s_clear(&(o->hmap_tpuz)); return  o;} \
  static inline bcore_huffman_index_s* bcore_huffman_index_s_set( bcore_huffman_index_s* o, u2_t val, uz_t index ){ bcore_hmap_tpuz_s_set(&(o->hmap_tpuz),bcore_huffman_index_s_key(o,val ), index ); return  o;} \
  static inline uz_t bcore_huffman_index_s_get( const bcore_huffman_index_s* o, u2_t val ){ const uz_t* p = bcore_hmap_tpuz_s_get(&(o->hmap_tpuz),bcore_huffman_index_s_key(o,val ) ); return  p ? (*(p)) : 0;}
#define TYPEOF_bcore_huffman_node_s 0x94670E9F093D735Bull
#define BETH_EXPAND_ITEM_bcore_huffman_node_s \
  BCORE_DECLARE_OBJECT( bcore_huffman_node_s ) \
  { \
      u3_t c; \
      uz_t p; \
      uz_t b0; \
      uz_t b1; \
      u2_t v; \
  }; \
  static inline bl_t bcore_huffman_node_s_is_leaf( const bcore_huffman_node_s* o ); \
  static inline bl_t bcore_huffman_node_s_is_root( const bcore_huffman_node_s* o ); \
  static inline bl_t bcore_huffman_node_s_is_leaf( const bcore_huffman_node_s* o ){ return  o->b0 == 0;} \
  static inline bl_t bcore_huffman_node_s_is_root( const bcore_huffman_node_s* o ){ return  o->p  == 0;}
#define TYPEOF_bcore_huffman_count_node_s 0x4B25BE77CD877C07ull
#define BETH_EXPAND_ITEM_bcore_huffman_count_node_s \
  BCORE_DECLARE_OBJECT( bcore_huffman_count_node_s ) \
  { \
      u3_t c; \
      u2_t v; \
  }; \
  static inline bl_t bcore_huffman_count_node_s_is_equal( const bcore_huffman_count_node_s* o, const bcore_huffman_count_node_s* b ); \
  static inline bl_t bcore_huffman_count_node_s_is_equal( const bcore_huffman_count_node_s* o, const bcore_huffman_count_node_s* b ){ return  ( o->c == b->c ) && ( o->v == b->v );}
#define TYPEOF_bcore_huffman_count_map_s 0x18A300EACC38A963ull
#define BETH_EXPAND_ITEM_bcore_huffman_count_map_s \
  BCORE_DECLARE_OBJECT( bcore_huffman_count_map_s ) \
  { \
      aware_t _; \
      BCORE_ARRAY_DYN_SOLID_STATIC_S( bcore_huffman_count_node_s, ); \
  }; \
  bcore_huffman_count_map_s* bcore_huffman_count_map_s_from_hist( bcore_huffman_count_map_s* o, const bcore_huffman_hist_s* hist ); \
  bl_t bcore_huffman_count_map_s_is_sorted( const bcore_huffman_count_map_s* o ); \
  bl_t bcore_huffman_count_map_s_is_equal( const bcore_huffman_count_map_s* o, const bcore_huffman_count_map_s* b ); \
  bcore_huffman_count_map_s* bcore_huffman_count_map_s_encode( bcore_huffman_count_map_s* o, bcore_huffman_bit_buffer_s* out ); \
  bcore_huffman_count_map_s* bcore_huffman_count_map_s_decode( bcore_huffman_count_map_s* o, bcore_huffman_bit_buffer_iterator_s* in );
#define TYPEOF_bcore_huffman_tree_s 0x09DD23FBA662202Bull
#define BETH_EXPAND_ITEM_bcore_huffman_tree_s \
  BCORE_DECLARE_OBJECT( bcore_huffman_tree_s ) \
  { \
      aware_t _; \
      BCORE_ARRAY_DYN_SOLID_STATIC_S( bcore_huffman_node_s, ); \
  }; \
  bcore_huffman_tree_s* bcore_huffman_tree_s_build( bcore_huffman_tree_s* o, const bcore_huffman_count_map_s* count_map, bcore_huffman_index_s* leaf_index ); \
  static inline sz_t bcore_huffman_tree_s_root_index( const bcore_huffman_tree_s* o ); \
  static inline sz_t bcore_huffman_tree_s_root_index( const bcore_huffman_tree_s* o ){ return  o->size - 1;}
#define BETH_EXPAND_GROUP_bcore_huffman \
  BCORE_FORWARD_OBJECT( bcore_huffman ); \
  BCORE_FORWARD_OBJECT( bcore_huffman_codec_s ); \
  BCORE_FORWARD_OBJECT( bcore_huffman_bit_buffer_s ); \
  BCORE_FORWARD_OBJECT( bcore_huffman_bit_buffer_iterator_s ); \
  BCORE_FORWARD_OBJECT( bcore_huffman_hist_s ); \
  BCORE_FORWARD_OBJECT( bcore_huffman_index_s ); \
  BCORE_FORWARD_OBJECT( bcore_huffman_node_s ); \
  BCORE_FORWARD_OBJECT( bcore_huffman_count_node_s ); \
  BCORE_FORWARD_OBJECT( bcore_huffman_count_map_s ); \
  BCORE_FORWARD_OBJECT( bcore_huffman_tree_s ); \
  sz_t bcore_huffman_min_bits( u3_t v, sz_t n ); \
  static inline u2_t bcore_huffman_u2_from_s2( s2_t s ); \
  static inline u3_t bcore_huffman_u3_from_s3( s3_t s ); \
  static inline s2_t bcore_huffman_s2_from_u2( u2_t u ); \
  static inline s3_t bcore_huffman_s3_from_u3( u3_t u ); \
  void bcore_huffman_selftest( void ); \
  XOILA_DECLARE_SPECT( bcore_huffman ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bcore_huffman ) \
  BETH_EXPAND_ITEM_bcore_huffman_codec_s \
  BETH_EXPAND_ITEM_bcore_huffman_bit_buffer_s \
  BETH_EXPAND_ITEM_bcore_huffman_bit_buffer_iterator_s \
  BETH_EXPAND_ITEM_bcore_huffman_hist_s \
  BETH_EXPAND_ITEM_bcore_huffman_index_s \
  BETH_EXPAND_ITEM_bcore_huffman_node_s \
  BETH_EXPAND_ITEM_bcore_huffman_count_node_s \
  BETH_EXPAND_ITEM_bcore_huffman_count_map_s \
  BETH_EXPAND_ITEM_bcore_huffman_tree_s \
  static inline u2_t bcore_huffman_u2_from_s2( s2_t s ){ return  ( s < 0 ) ? ( ( ( ~(u2_t)s ) << 1 ) + 1 ) : ( (u2_t)s << 1 );} \
  static inline u3_t bcore_huffman_u3_from_s3( s3_t s ){ return  ( s < 0 ) ? ( ( ( ~(u3_t)s ) << 1 ) + 1 ) : ( (u3_t)s << 1 );} \
  static inline s2_t bcore_huffman_s2_from_u2( u2_t u ){ return  ( u & 1 ) ? ~( ( u - 1 ) >> 1 ) : ( u >> 1 );} \
  static inline s3_t bcore_huffman_s3_from_u3( u3_t u ){ return  ( u & 1 ) ? ~( ( u - 1 ) >> 1 ) : ( u >> 1 );}

/**********************************************************************************************************************/
// source: bcore_indexer.h

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_indexer

#define TYPEOF_bcore_indexer 0x1B0C2133A5A684E2ull
#define TYPEOF_bcore_indexer_spect_s 0xC56978EB4AC5F94Eull
#define TYPEOF_bcore_indexer_io_s 0x4F9D0C0C22B50A29ull
#define BETH_EXPAND_ITEM_bcore_indexer_io_s \
  BCORE_DECLARE_OBJECT( bcore_indexer_io_s ) \
  { \
      s3_t i; \
      s3_t o; \
  };
#define TYPEOF_bcore_indexer_io_arr_s 0x443AEF15FC9D03D5ull
#define BETH_EXPAND_ITEM_bcore_indexer_io_arr_s \
  BCORE_DECLARE_OBJECT( bcore_indexer_io_arr_s ) \
  { \
      aware_t _; \
      BCORE_ARRAY_DYN_SOLID_STATIC_S( bcore_indexer_io_s, ); \
  };
#define TYPEOF_bcore_indexer_cs_s 0xDE7D8FB93952861Full
#define BETH_EXPAND_ITEM_bcore_indexer_cs_s \
  BCORE_DECLARE_OBJECT( bcore_indexer_cs_s ) \
  { \
      s3_t c; \
      s3_t s; \
  };
#define TYPEOF_bcore_indexer_cs_arr_s 0x8142CEB2147AE917ull
#define BETH_EXPAND_ITEM_bcore_indexer_cs_arr_s \
  BCORE_DECLARE_OBJECT( bcore_indexer_cs_arr_s ) \
  { \
      aware_t _; \
      BCORE_ARRAY_DYN_SOLID_STATIC_S( bcore_indexer_cs_s, ); \
  };
#define TYPEOF_bcore_indexer_s 0x3FE3E37538D925BCull
#define BETH_EXPAND_ITEM_bcore_indexer_s \
  BCORE_DECLARE_OBJECT( bcore_indexer_s ) \
  { \
      aware_t _; \
      bcore_indexer_io_arr_s io_arr; \
      bcore_indexer_cs_arr_s cs_arr; \
      s0_t mask_bits; \
      s3_t size; \
  }; \
  bcore_indexer_s* bcore_indexer_s_setup( bcore_indexer_s* o, const bcore_arr_s3_s* size_arr ); \
  bl_t bcore_indexer_s_get_io( const bcore_indexer_s* o, s3_t index, bcore_indexer_io_s* io );
#define BETH_EXPAND_GROUP_bcore_indexer \
  BCORE_FORWARD_OBJECT( bcore_indexer ); \
  BCORE_FORWARD_OBJECT( bcore_indexer_io_s ); \
  BCORE_FORWARD_OBJECT( bcore_indexer_io_arr_s ); \
  BCORE_FORWARD_OBJECT( bcore_indexer_cs_s ); \
  BCORE_FORWARD_OBJECT( bcore_indexer_cs_arr_s ); \
  BCORE_FORWARD_OBJECT( bcore_indexer_s ); \
  void bcore_indexer_selftest( void ); \
  XOILA_DECLARE_SPECT( bcore_indexer ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bcore_indexer ) \
  BETH_EXPAND_ITEM_bcore_indexer_io_s \
  BETH_EXPAND_ITEM_bcore_indexer_io_arr_s \
  BETH_EXPAND_ITEM_bcore_indexer_cs_s \
  BETH_EXPAND_ITEM_bcore_indexer_cs_arr_s \
  BETH_EXPAND_ITEM_bcore_indexer_s

/**********************************************************************************************************************/
// source: bcore_parse.h

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_parse; embeds: bcore_parse.x

#define TYPEOF_bcore_parse 0xC9DE673087A88606ull
#define TYPEOF_bcore_parse_spect_s 0xB71B1E6A29428C12ull
#define BETH_EXPAND_GROUP_bcore_parse \
  BCORE_FORWARD_OBJECT( bcore_parse ); \
  er_t bcore_parse_number_literal_to_sr( x_source* source, sr_s* sr ); \
  er_t bcore_parse_number_literal_to_f3( x_source* source, f3_t* val ); \
  er_t bcore_parse_number_literal_to_s3( x_source* source, s3_t* val ); \
  static inline er_t bcore_parse_st_number_literal_to_f3( const st_s* st, f3_t* val ); \
  static inline er_t bcore_parse_st_number_literal_to_s3( const st_s* st, s3_t* val ); \
  XOILA_DECLARE_SPECT( bcore_parse ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bcore_parse ) \
  static inline er_t bcore_parse_st_number_literal_to_f3( const st_s* st, f3_t* val ){BLM_INIT_LEVEL(0);BLM_RETURNV(er_t, bcore_parse_number_literal_to_f3(((x_source*)(((bcore_source_string_s*)BLM_LEVEL_T_PUSH(0,bcore_source_string_s,bcore_source_string_s_create_from_string(st ))))), val ))} \
  static inline er_t bcore_parse_st_number_literal_to_s3( const st_s* st, s3_t* val ){BLM_INIT_LEVEL(0);BLM_RETURNV(er_t, bcore_parse_number_literal_to_s3(((x_source*)(((bcore_source_string_s*)BLM_LEVEL_T_PUSH(0,bcore_source_string_s,bcore_source_string_s_create_from_string(st ))))), val ))}

/**********************************************************************************************************************/
// source: bcore_x_btml.h

//----------------------------------------------------------------------------------------------------------------------
// group: x_btml

#define TYPEOF_x_btml 0xED87A1C2530803FFull
#define TYPEOF_x_btml_spect_s 0x2E954F9F03CE7DAFull
#define BETH_EXPAND_GROUP_x_btml \
  BCORE_FORWARD_OBJECT( x_btml ); \
  static inline er_t x_btml_from_source( x_btml* o, x_source* source ); \
  static inline er_t x_btml_t_from_file( x_btml* o, tp_t t, sc_t file ); \
  static inline er_t x_btml_from_file( x_btml* o, sc_t file ); \
  static inline er_t x_btml_t_from_st( x_btml* o, tp_t t, const st_s* st ); \
  static inline er_t x_btml_from_st( x_btml* o, const st_s* st ); \
  static inline er_t x_btml_t_from_sc( x_btml* o, tp_t t, sc_t sc ); \
  static inline er_t x_btml_from_sc( x_btml* o, sc_t sc ); \
  static inline er_t x_btml_t_body_from_source( x_btml* o, tp_t t, x_source* source ); \
  static inline er_t x_btml_body_from_source( x_btml* o, x_source* source ); \
  static inline er_t x_btml_t_body_from_file( x_btml* o, tp_t t, sc_t file ); \
  static inline er_t x_btml_body_from_file( x_btml* o, sc_t file ); \
  static inline er_t x_btml_t_body_from_st( x_btml* o, tp_t t, const st_s* st ); \
  static inline er_t x_btml_body_from_st( x_btml* o, const st_s* st ); \
  static inline er_t x_btml_t_body_from_sc( x_btml* o, tp_t t, sc_t sc ); \
  static inline er_t x_btml_body_from_sc( x_btml* o, sc_t sc ); \
  static inline x_btml* x_btml_create_from_st_t( const st_s* st, tp_t* type ); \
  static inline x_btml* x_btml_create_from_sc_t( sc_t sc, tp_t* type ); \
  static inline x_btml* x_btml_create_from_file_t( sc_t file, tp_t* type ); \
  static inline x_btml* x_btml_create_from_st( const st_s* st ); \
  static inline x_btml* x_btml_create_from_sc( sc_t sc ); \
  static inline x_btml* x_btml_create_from_file( sc_t file ); \
  static inline x_sink* x_btml_to_sink( const x_btml* o, x_sink* sink ); \
  er_t x_btml_t_to_file( const x_btml* o, tp_t t, sc_t file ); \
  static inline er_t x_btml_to_file( const x_btml* o, sc_t file ); \
  static inline void x_btml_t_to_stdout( const x_btml* o, tp_t t ); \
  static inline void x_btml_to_stdout( const x_btml* o ); \
  static inline sz_t x_btml_test_transfer( const x_btml* o ); \
  er_t x_btml_t_from_source( x_btml* o, tp_t t, x_source* source ); \
  x_btml* x_btml_create_from_source_t( x_source* source, tp_t* type ); \
  x_btml* x_btml_create_from_source( x_source* source ); \
  x_sink* x_btml_t_to_sink( const x_btml* o, tp_t t, x_sink* sink ); \
  static inline bl_t x_btml_is_identifier( x_source* source ); \
  st_s* x_btml_parse_name( x_source* source ); \
  sc_t x_btml_name_of( tp_t type, st_s* buf ); \
  tp_t x_btml_type_of( const st_s* name ); \
  bl_t x_btml_appears_valid( x_source* source ); \
  bl_t x_btml_t_appears_valid( tp_t type, x_source* source ); \
  er_t x_btml_parse_create_object( x_source* source, const sr_s* default_obj, tp_t default_tp, sr_s* obj ); \
  er_t x_btml_t_parse_body( x_btml* o, tp_t t, x_source* source ); \
  er_t x_btml_skip_body( x_source* source ); \
  void x_btml_t_translate_recursive( const x_btml* o, tp_t t, tp_t name, bl_t shelve, x_sink* sink, sz_t depth ); \
  sz_t x_btml_t_test_transfer( const x_btml* o, tp_t t ); \
  void x_btml_selftest( void ); \
  typedef er_t (*x_btml_btml_body_from_source)(x_btml* o, x_source* source ); \
  typedef void (*x_btml_btml_body_to_sink)(const x_btml* o, x_sink* sink ); \
  XOILA_DECLARE_SPECT( x_btml ) \
  { \
      bcore_spect_header_s header; \
      x_btml_btml_body_from_source btml_body_from_source; \
      x_btml_btml_body_to_sink btml_body_to_sink; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( x_btml ) \
  static inline er_t x_btml_a_btml_body_from_source( x_btml* o, x_source* source ){ const x_btml_spect_s* p = x_btml_spect_s_get_aware( o ); assert( p->btml_body_from_source ); return p->btml_body_from_source( o, source );} \
  static inline bl_t x_btml_defines_btml_body_from_source( const x_btml* o ){  return x_btml_spect_s_get_aware( o )->btml_body_from_source != NULL;} \
  static inline er_t x_btml_t_btml_body_from_source( x_btml* o, tp_t t, x_source* source ){ const x_btml_spect_s* p = x_btml_spect_s_get_typed( t ); assert( p->btml_body_from_source ); return p->btml_body_from_source( o, source );} \
  static inline bl_t x_btml_t_defines_btml_body_from_source( tp_t t ){  return x_btml_spect_s_get_typed( t )->btml_body_from_source != NULL;} \
  static inline void x_btml_a_btml_body_to_sink( const x_btml* o, x_sink* sink ){ const x_btml_spect_s* p = x_btml_spect_s_get_aware( o ); assert( p->btml_body_to_sink ); p->btml_body_to_sink( o, sink );} \
  static inline bl_t x_btml_defines_btml_body_to_sink( const x_btml* o ){  return x_btml_spect_s_get_aware( o )->btml_body_to_sink != NULL;} \
  static inline void x_btml_t_btml_body_to_sink( const x_btml* o, tp_t t, x_sink* sink ){ const x_btml_spect_s* p = x_btml_spect_s_get_typed( t ); assert( p->btml_body_to_sink ); p->btml_body_to_sink( o, sink );} \
  static inline bl_t x_btml_t_defines_btml_body_to_sink( tp_t t ){  return x_btml_spect_s_get_typed( t )->btml_body_to_sink != NULL;} \
  static inline er_t x_btml_from_source( x_btml* o, x_source* source ){return  x_btml_t_from_source(o,o->_, source );} \
  static inline er_t x_btml_t_from_file( x_btml* o, tp_t t, sc_t file ){BLM_INIT_LEVEL(0);BLM_RETURNV(er_t, x_btml_t_from_source(o,t, ((x_source*)BLM_LEVEL_A_PUSH(0,x_source_check_create_from_file(file ))) ))} \
  static inline er_t x_btml_from_file( x_btml* o, sc_t file ){return  x_btml_t_from_file(o,o->_, file );} \
  static inline er_t x_btml_t_from_st( x_btml* o, tp_t t, const st_s* st ){BLM_INIT_LEVEL(0);BLM_RETURNV(er_t, x_btml_t_from_source(o,t, ((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_st(st ))) ))} \
  static inline er_t x_btml_from_st( x_btml* o, const st_s* st ){return  x_btml_t_from_st(o,o->_, st );} \
  static inline er_t x_btml_t_from_sc( x_btml* o, tp_t t, sc_t sc ){BLM_INIT_LEVEL(0);BLM_RETURNV(er_t, x_btml_t_from_source(o,t, ((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_sc(sc ))) ))} \
  static inline er_t x_btml_from_sc( x_btml* o, sc_t sc ){return  x_btml_t_from_sc(o,o->_, sc );} \
  static inline er_t x_btml_t_body_from_source( x_btml* o, tp_t t, x_source* source ){return  x_btml_t_parse_body(o,t, source );} \
  static inline er_t x_btml_body_from_source( x_btml* o, x_source* source ){return  x_btml_t_body_from_source(o,o->_, source );} \
  static inline er_t x_btml_t_body_from_file( x_btml* o, tp_t t, sc_t file ){BLM_INIT_LEVEL(0);BLM_RETURNV(er_t, x_btml_t_body_from_source(o,t, ((x_source*)BLM_LEVEL_A_PUSH(0,x_source_check_create_from_file(file ))) ))} \
  static inline er_t x_btml_body_from_file( x_btml* o, sc_t file ){return  x_btml_t_body_from_file(o,o->_, file );} \
  static inline er_t x_btml_t_body_from_st( x_btml* o, tp_t t, const st_s* st ){BLM_INIT_LEVEL(0);BLM_RETURNV(er_t, x_btml_t_body_from_source(o,t, ((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_st(st ))) ))} \
  static inline er_t x_btml_body_from_st( x_btml* o, const st_s* st ){return  x_btml_t_body_from_st(o,o->_, st );} \
  static inline er_t x_btml_t_body_from_sc( x_btml* o, tp_t t, sc_t sc ){BLM_INIT_LEVEL(0);BLM_RETURNV(er_t, x_btml_t_body_from_source(o,t, ((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_sc(sc ))) ))} \
  static inline er_t x_btml_body_from_sc( x_btml* o, sc_t sc ){return  x_btml_t_body_from_sc(o,o->_, sc );} \
  static inline x_btml* x_btml_create_from_st_t( const st_s* st, tp_t* type ){BLM_INIT_LEVEL(0);BLM_RETURNV(x_btml*, x_btml_create_from_source_t(((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_st(st ))), type ))} \
  static inline x_btml* x_btml_create_from_sc_t( sc_t sc, tp_t* type ){BLM_INIT_LEVEL(0);BLM_RETURNV(x_btml*, x_btml_create_from_source_t(((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_sc(sc ))), type ))} \
  static inline x_btml* x_btml_create_from_file_t( sc_t file, tp_t* type ){BLM_INIT_LEVEL(0);BLM_RETURNV(x_btml*, x_btml_create_from_source_t(((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_file(file ))), type ))} \
  static inline x_btml* x_btml_create_from_st( const st_s* st ){BLM_INIT_LEVEL(0);BLM_RETURNV(x_btml*, x_btml_create_from_source(((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_st(st ))) ))} \
  static inline x_btml* x_btml_create_from_sc( sc_t sc ){BLM_INIT_LEVEL(0);BLM_RETURNV(x_btml*, x_btml_create_from_source(((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_sc(sc ))) ))} \
  static inline x_btml* x_btml_create_from_file( sc_t file ){BLM_INIT_LEVEL(0);BLM_RETURNV(x_btml*, x_btml_create_from_source(((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_file(file ))) ))} \
  static inline x_sink* x_btml_to_sink( const x_btml* o, x_sink* sink ){return  x_btml_t_to_sink(o,o->_, sink );} \
  static inline er_t x_btml_to_file( const x_btml* o, sc_t file ){return  x_btml_t_to_file(o,o->_, file );} \
  static inline void x_btml_t_to_stdout( const x_btml* o, tp_t t ){x_btml_t_to_sink(o,t, x_sink_stdout() );} \
  static inline void x_btml_to_stdout( const x_btml* o ){x_btml_t_to_stdout(o,o->_ );} \
  static inline sz_t x_btml_test_transfer( const x_btml* o ){return  x_btml_t_test_transfer(o,o->_ );} \
  static inline bl_t x_btml_is_identifier( x_source* source ){return  x_source_parse_bl(source," #?(([0]>='A'&&[0]<='Z')||([0]>='a'&&[0]<='z')||[0]=='_')" );}

/**********************************************************************************************************************/
// source: bcore_x_bbml.h

//----------------------------------------------------------------------------------------------------------------------
// group: x_bbml

#define TYPEOF_x_bbml 0x88CFA7C2AACE2DD5ull
#define TYPEOF_x_bbml_spect_s 0x86EF4BAF3E70A2B1ull
#define BETH_EXPAND_GROUP_x_bbml \
  BCORE_FORWARD_OBJECT( x_bbml ); \
  static inline er_t x_bbml_from_source( x_bbml* o, x_source* source ); \
  static inline er_t x_bbml_t_from_file( x_bbml* o, tp_t t, sc_t file ); \
  static inline er_t x_bbml_from_file( x_bbml* o, sc_t file ); \
  static inline er_t x_bbml_t_from_st( x_bbml* o, tp_t t, const st_s* st ); \
  static inline er_t x_bbml_from_st( x_bbml* o, const st_s* st ); \
  static inline er_t x_bbml_t_from_sc( x_bbml* o, tp_t t, sc_t sc ); \
  static inline er_t x_bbml_from_sc( x_bbml* o, sc_t sc ); \
  static inline x_bbml* x_bbml_create_from_st_t( const st_s* st, tp_t* type ); \
  static inline x_bbml* x_bbml_create_from_sc_t( sc_t sc, tp_t* type ); \
  static inline x_bbml* x_bbml_create_from_file_t( sc_t file, tp_t* type ); \
  static inline x_bbml* x_bbml_create_from_st( const st_s* st ); \
  static inline x_bbml* x_bbml_create_from_sc( sc_t sc ); \
  static inline x_bbml* x_bbml_create_from_file( sc_t file ); \
  static inline x_sink* x_bbml_to_sink( const x_bbml* o, x_sink* sink ); \
  static inline void x_bbml_t_to_file( const x_bbml* o, tp_t t, sc_t file ); \
  static inline void x_bbml_to_file( const x_bbml* o, sc_t file ); \
  static inline sz_t x_bbml_test_transfer( const x_bbml* o ); \
  er_t x_bbml_t_from_source( x_bbml* o, tp_t t, x_source* source ); \
  x_bbml* x_bbml_create_from_source_t( x_source* source, tp_t* type ); \
  x_bbml* x_bbml_create_from_source( x_source* source ); \
  x_sink* x_bbml_t_to_sink( const x_bbml* o, tp_t t, x_sink* sink ); \
  static inline tp_t x_bbml_parse_type( x_source* source ); \
  static inline bl_t x_bbml_parse_flag( x_source* source ); \
  static inline sz_t x_bbml_parse_size( x_source* source ); \
  bl_t x_bbml_appears_valid( x_source* source ); \
  er_t x_bbml_parse_create_object( x_source* source, sr_s* obj ); \
  er_t x_bbml_t_parse_leaf_body( x_bbml* o, tp_t t, x_source* source ); \
  er_t x_bbml_t_parse_body( x_bbml* o, tp_t t, x_source* source ); \
  static inline void x_bbml_push_type( x_sink* sink, tp_t type ); \
  static inline void x_bbml_push_flag( x_sink* sink, bl_t flag ); \
  static inline void x_bbml_push_size( x_sink* sink, s3_t size ); \
  void x_bbml_push_leaf( x_sink* sink, tp_t t, const x_inst* leaf ); \
  void x_bbml_t_translate_recursive( const x_bbml* o, tp_t t, tp_t name, bl_t shelve, x_sink* sink ); \
  sz_t x_bbml_t_test_transfer( const x_bbml* o, tp_t t ); \
  void x_bbml_selftest( void ); \
  typedef er_t (*x_bbml_bbml_body_from_source)(x_bbml* o, x_source* source ); \
  typedef void (*x_bbml_bbml_body_to_sink)(const x_bbml* o, x_sink* sink ); \
  XOILA_DECLARE_SPECT( x_bbml ) \
  { \
      bcore_spect_header_s header; \
      x_bbml_bbml_body_from_source bbml_body_from_source; \
      x_bbml_bbml_body_to_sink bbml_body_to_sink; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( x_bbml ) \
  static inline er_t x_bbml_a_bbml_body_from_source( x_bbml* o, x_source* source ){ const x_bbml_spect_s* p = x_bbml_spect_s_get_aware( o ); assert( p->bbml_body_from_source ); return p->bbml_body_from_source( o, source );} \
  static inline bl_t x_bbml_defines_bbml_body_from_source( const x_bbml* o ){  return x_bbml_spect_s_get_aware( o )->bbml_body_from_source != NULL;} \
  static inline er_t x_bbml_t_bbml_body_from_source( x_bbml* o, tp_t t, x_source* source ){ const x_bbml_spect_s* p = x_bbml_spect_s_get_typed( t ); assert( p->bbml_body_from_source ); return p->bbml_body_from_source( o, source );} \
  static inline bl_t x_bbml_t_defines_bbml_body_from_source( tp_t t ){  return x_bbml_spect_s_get_typed( t )->bbml_body_from_source != NULL;} \
  static inline void x_bbml_a_bbml_body_to_sink( const x_bbml* o, x_sink* sink ){ const x_bbml_spect_s* p = x_bbml_spect_s_get_aware( o ); assert( p->bbml_body_to_sink ); p->bbml_body_to_sink( o, sink );} \
  static inline bl_t x_bbml_defines_bbml_body_to_sink( const x_bbml* o ){  return x_bbml_spect_s_get_aware( o )->bbml_body_to_sink != NULL;} \
  static inline void x_bbml_t_bbml_body_to_sink( const x_bbml* o, tp_t t, x_sink* sink ){ const x_bbml_spect_s* p = x_bbml_spect_s_get_typed( t ); assert( p->bbml_body_to_sink ); p->bbml_body_to_sink( o, sink );} \
  static inline bl_t x_bbml_t_defines_bbml_body_to_sink( tp_t t ){  return x_bbml_spect_s_get_typed( t )->bbml_body_to_sink != NULL;} \
  static inline er_t x_bbml_from_source( x_bbml* o, x_source* source ){return  x_bbml_t_from_source(o,o->_, source );} \
  static inline er_t x_bbml_t_from_file( x_bbml* o, tp_t t, sc_t file ){BLM_INIT_LEVEL(0);BLM_RETURNV(er_t, x_bbml_t_from_source(o,t, ((x_source*)BLM_LEVEL_A_PUSH(0,x_source_check_create_from_file(file ))) ))} \
  static inline er_t x_bbml_from_file( x_bbml* o, sc_t file ){return  x_bbml_t_from_file(o,o->_, file );} \
  static inline er_t x_bbml_t_from_st( x_bbml* o, tp_t t, const st_s* st ){BLM_INIT_LEVEL(0);BLM_RETURNV(er_t, x_bbml_t_from_source(o,t, ((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_st(st ))) ))} \
  static inline er_t x_bbml_from_st( x_bbml* o, const st_s* st ){return  x_bbml_t_from_st(o,o->_, st );} \
  static inline er_t x_bbml_t_from_sc( x_bbml* o, tp_t t, sc_t sc ){BLM_INIT_LEVEL(0);BLM_RETURNV(er_t, x_bbml_t_from_source(o,t, ((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_sc(sc ))) ))} \
  static inline er_t x_bbml_from_sc( x_bbml* o, sc_t sc ){return  x_bbml_t_from_sc(o,o->_, sc );} \
  static inline x_bbml* x_bbml_create_from_st_t( const st_s* st, tp_t* type ){BLM_INIT_LEVEL(0);BLM_RETURNV(x_bbml*, x_bbml_create_from_source_t(((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_st(st ))), type ))} \
  static inline x_bbml* x_bbml_create_from_sc_t( sc_t sc, tp_t* type ){BLM_INIT_LEVEL(0);BLM_RETURNV(x_bbml*, x_bbml_create_from_source_t(((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_sc(sc ))), type ))} \
  static inline x_bbml* x_bbml_create_from_file_t( sc_t file, tp_t* type ){BLM_INIT_LEVEL(0);BLM_RETURNV(x_bbml*, x_bbml_create_from_source_t(((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_file(file ))), type ))} \
  static inline x_bbml* x_bbml_create_from_st( const st_s* st ){BLM_INIT_LEVEL(0);BLM_RETURNV(x_bbml*, x_bbml_create_from_source(((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_st(st ))) ))} \
  static inline x_bbml* x_bbml_create_from_sc( sc_t sc ){BLM_INIT_LEVEL(0);BLM_RETURNV(x_bbml*, x_bbml_create_from_source(((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_sc(sc ))) ))} \
  static inline x_bbml* x_bbml_create_from_file( sc_t file ){BLM_INIT_LEVEL(0);BLM_RETURNV(x_bbml*, x_bbml_create_from_source(((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_file(file ))) ))} \
  static inline x_sink* x_bbml_to_sink( const x_bbml* o, x_sink* sink ){return  x_bbml_t_to_sink(o,o->_, sink );} \
  static inline void x_bbml_t_to_file( const x_bbml* o, tp_t t, sc_t file ){BLM_INIT_LEVEL(0);x_bbml_t_to_sink(o,t,((x_sink*)( ((bcore_sink*)BLM_LEVEL_A_PUSH(0,bcore_file_open_sink(file ))) )));BLM_DOWN();} \
  static inline void x_bbml_to_file( const x_bbml* o, sc_t file ){x_bbml_t_to_file(o,o->_, file );} \
  static inline sz_t x_bbml_test_transfer( const x_bbml* o ){return  x_bbml_t_test_transfer(o,o->_ );} \
  static inline tp_t x_bbml_parse_type( x_source* source ){ tp_t type = 0; x_source_get_data(source,((x_inst*)((&(type)))), sizeof( type ) ); return  type;} \
  static inline bl_t x_bbml_parse_flag( x_source* source ){ u0_t flag = 0; x_source_get_data(source,((x_inst*)((&(flag)))), sizeof( flag ) ); return  flag;} \
  static inline sz_t x_bbml_parse_size( x_source* source ){ s3_t size = 0; x_source_get_data(source,((x_inst*)((&(size)))), sizeof( size ) ); return  size;} \
  static inline void x_bbml_push_type( x_sink* sink, tp_t type ){x_sink_push_data(sink,((const x_inst*)((&(type)))), sizeof( type ) );} \
  static inline void x_bbml_push_flag( x_sink* sink, bl_t flag ){ u0_t v = flag ? 1 : 0; x_sink_push_data(sink,((const x_inst*)((&(v)))), 1 );} \
  static inline void x_bbml_push_size( x_sink* sink, s3_t size ){x_sink_push_data(sink,((const x_inst*)((&(size)))), sizeof( size ) );}

/**********************************************************************************************************************/
// source: bcore_x_bcml.h

//----------------------------------------------------------------------------------------------------------------------
// group: x_bcml

#define TYPEOF_x_bcml 0x8FBECEC2AE3DEA96ull
#define TYPEOF_x_bcml_spect_s 0x5E7680C51C63B122ull
#define BETH_EXPAND_GROUP_x_bcml \
  BCORE_FORWARD_OBJECT( x_bcml ); \
  static inline er_t x_bcml_from_source( x_bcml* o, x_source* source ); \
  static inline er_t x_bcml_t_from_file( x_bcml* o, tp_t t, sc_t file ); \
  static inline er_t x_bcml_from_file( x_bcml* o, sc_t file ); \
  static inline er_t x_bcml_t_from_st( x_bcml* o, tp_t t, const st_s* st ); \
  static inline er_t x_bcml_from_st( x_bcml* o, const st_s* st ); \
  static inline er_t x_bcml_t_from_sc( x_bcml* o, tp_t t, sc_t sc ); \
  static inline er_t x_bcml_from_sc( x_bcml* o, sc_t sc ); \
  static inline x_bcml* x_bcml_create_from_st_t( const st_s* st, tp_t* type ); \
  static inline x_bcml* x_bcml_create_from_sc_t( sc_t sc, tp_t* type ); \
  static inline x_bcml* x_bcml_create_from_file_t( sc_t file, tp_t* type ); \
  static inline x_bcml* x_bcml_create_from_st( const st_s* st ); \
  static inline x_bcml* x_bcml_create_from_sc( sc_t sc ); \
  static inline x_bcml* x_bcml_create_from_file( sc_t file ); \
  static inline x_sink* x_bcml_to_sink( const x_bcml* o, x_sink* sink ); \
  static inline void x_bcml_t_to_file( const x_bcml* o, tp_t t, sc_t file ); \
  static inline void x_bcml_to_file( const x_bcml* o, sc_t file ); \
  static inline sz_t x_bcml_test_transfer( const x_bcml* o ); \
  er_t x_bcml_t_from_source( x_bcml* o, tp_t t, x_source* source ); \
  x_bcml* x_bcml_create_from_source_t( x_source* source, tp_t* type ); \
  x_bcml* x_bcml_create_from_source( x_source* source ); \
  x_sink* x_bcml_t_to_sink( const x_bcml* o, tp_t t, x_sink* sink ); \
  static inline tp_t x_bcml_parse_type( x_source* source ); \
  static inline bl_t x_bcml_parse_flag( x_source* source ); \
  static inline sz_t x_bcml_parse_size( x_source* source ); \
  bl_t x_bcml_appears_valid( x_source* source ); \
  er_t x_bcml_parse_create_object( bl_t parse_existence, tp_t given_type, x_source* source, sr_s* obj ); \
  er_t x_bcml_t_parse_leaf_body( x_bcml* o, tp_t t, x_source* source ); \
  er_t x_bcml_t_parse_leaf_arr_body( x_bcml* o, tp_t t, sz_t size, x_source* source ); \
  er_t x_bcml_t_parse_body( x_bcml* o, tp_t t, x_source* source ); \
  static inline void x_bcml_push_type( x_sink* sink, tp_t type ); \
  static inline void x_bcml_push_flag( x_sink* sink, bl_t flag ); \
  static inline void x_bcml_push_size( x_sink* sink, s3_t size ); \
  void x_bcml_push_leaf( x_sink* sink, tp_t t, const x_inst* leaf ); \
  void x_bcml_push_leaf_arr( x_sink* sink, tp_t t, const x_inst* leaf, sz_t size ); \
  void x_bcml_t_translate_recursive( const x_bcml* o, tp_t t, bl_t push_existence, bl_t push_type, bl_t shelve, x_sink* sink ); \
  sz_t x_bcml_t_test_transfer( const x_bcml* o, tp_t t ); \
  void x_bcml_selftest( void ); \
  typedef er_t (*x_bcml_bcml_body_from_source)(x_bcml* o, x_source* source ); \
  typedef void (*x_bcml_bcml_body_to_sink)(const x_bcml* o, x_sink* sink ); \
  XOILA_DECLARE_SPECT( x_bcml ) \
  { \
      bcore_spect_header_s header; \
      x_bcml_bcml_body_from_source bcml_body_from_source; \
      x_bcml_bcml_body_to_sink bcml_body_to_sink; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( x_bcml ) \
  static inline er_t x_bcml_a_bcml_body_from_source( x_bcml* o, x_source* source ){ const x_bcml_spect_s* p = x_bcml_spect_s_get_aware( o ); assert( p->bcml_body_from_source ); return p->bcml_body_from_source( o, source );} \
  static inline bl_t x_bcml_defines_bcml_body_from_source( const x_bcml* o ){  return x_bcml_spect_s_get_aware( o )->bcml_body_from_source != NULL;} \
  static inline er_t x_bcml_t_bcml_body_from_source( x_bcml* o, tp_t t, x_source* source ){ const x_bcml_spect_s* p = x_bcml_spect_s_get_typed( t ); assert( p->bcml_body_from_source ); return p->bcml_body_from_source( o, source );} \
  static inline bl_t x_bcml_t_defines_bcml_body_from_source( tp_t t ){  return x_bcml_spect_s_get_typed( t )->bcml_body_from_source != NULL;} \
  static inline void x_bcml_a_bcml_body_to_sink( const x_bcml* o, x_sink* sink ){ const x_bcml_spect_s* p = x_bcml_spect_s_get_aware( o ); assert( p->bcml_body_to_sink ); p->bcml_body_to_sink( o, sink );} \
  static inline bl_t x_bcml_defines_bcml_body_to_sink( const x_bcml* o ){  return x_bcml_spect_s_get_aware( o )->bcml_body_to_sink != NULL;} \
  static inline void x_bcml_t_bcml_body_to_sink( const x_bcml* o, tp_t t, x_sink* sink ){ const x_bcml_spect_s* p = x_bcml_spect_s_get_typed( t ); assert( p->bcml_body_to_sink ); p->bcml_body_to_sink( o, sink );} \
  static inline bl_t x_bcml_t_defines_bcml_body_to_sink( tp_t t ){  return x_bcml_spect_s_get_typed( t )->bcml_body_to_sink != NULL;} \
  static inline er_t x_bcml_from_source( x_bcml* o, x_source* source ){return  x_bcml_t_from_source(o,o->_, source );} \
  static inline er_t x_bcml_t_from_file( x_bcml* o, tp_t t, sc_t file ){BLM_INIT_LEVEL(0);BLM_RETURNV(er_t, x_bcml_t_from_source(o,t, ((x_source*)BLM_LEVEL_A_PUSH(0,x_source_check_create_from_file(file ))) ))} \
  static inline er_t x_bcml_from_file( x_bcml* o, sc_t file ){return  x_bcml_t_from_file(o,o->_, file );} \
  static inline er_t x_bcml_t_from_st( x_bcml* o, tp_t t, const st_s* st ){BLM_INIT_LEVEL(0);BLM_RETURNV(er_t, x_bcml_t_from_source(o,t, ((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_st(st ))) ))} \
  static inline er_t x_bcml_from_st( x_bcml* o, const st_s* st ){return  x_bcml_t_from_st(o,o->_, st );} \
  static inline er_t x_bcml_t_from_sc( x_bcml* o, tp_t t, sc_t sc ){BLM_INIT_LEVEL(0);BLM_RETURNV(er_t, x_bcml_t_from_source(o,t, ((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_sc(sc ))) ))} \
  static inline er_t x_bcml_from_sc( x_bcml* o, sc_t sc ){return  x_bcml_t_from_sc(o,o->_, sc );} \
  static inline x_bcml* x_bcml_create_from_st_t( const st_s* st, tp_t* type ){BLM_INIT_LEVEL(0);BLM_RETURNV(x_bcml*, x_bcml_create_from_source_t(((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_st(st ))), type ))} \
  static inline x_bcml* x_bcml_create_from_sc_t( sc_t sc, tp_t* type ){BLM_INIT_LEVEL(0);BLM_RETURNV(x_bcml*, x_bcml_create_from_source_t(((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_sc(sc ))), type ))} \
  static inline x_bcml* x_bcml_create_from_file_t( sc_t file, tp_t* type ){BLM_INIT_LEVEL(0);BLM_RETURNV(x_bcml*, x_bcml_create_from_source_t(((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_file(file ))), type ))} \
  static inline x_bcml* x_bcml_create_from_st( const st_s* st ){BLM_INIT_LEVEL(0);BLM_RETURNV(x_bcml*, x_bcml_create_from_source(((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_st(st ))) ))} \
  static inline x_bcml* x_bcml_create_from_sc( sc_t sc ){BLM_INIT_LEVEL(0);BLM_RETURNV(x_bcml*, x_bcml_create_from_source(((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_sc(sc ))) ))} \
  static inline x_bcml* x_bcml_create_from_file( sc_t file ){BLM_INIT_LEVEL(0);BLM_RETURNV(x_bcml*, x_bcml_create_from_source(((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_file(file ))) ))} \
  static inline x_sink* x_bcml_to_sink( const x_bcml* o, x_sink* sink ){return  x_bcml_t_to_sink(o,o->_, sink );} \
  static inline void x_bcml_t_to_file( const x_bcml* o, tp_t t, sc_t file ){BLM_INIT_LEVEL(0);x_bcml_t_to_sink(o,t,((x_sink*)( ((bcore_sink*)BLM_LEVEL_A_PUSH(0,bcore_file_open_sink(file ))) )));BLM_DOWN();} \
  static inline void x_bcml_to_file( const x_bcml* o, sc_t file ){x_bcml_t_to_file(o,o->_, file );} \
  static inline sz_t x_bcml_test_transfer( const x_bcml* o ){return  x_bcml_t_test_transfer(o,o->_ );} \
  static inline tp_t x_bcml_parse_type( x_source* source ){ tp_t type = 0; x_source_get_data(source,((x_inst*)((&(type)))), sizeof( type ) ); return  type;} \
  static inline bl_t x_bcml_parse_flag( x_source* source ){ u0_t flag = 0; x_source_get_data(source,((x_inst*)((&(flag)))), sizeof( flag ) ); return  flag;} \
  static inline sz_t x_bcml_parse_size( x_source* source ){ s3_t size = 0; x_source_get_data(source,((x_inst*)((&(size)))), sizeof( size ) ); return  size;} \
  static inline void x_bcml_push_type( x_sink* sink, tp_t type ){x_sink_push_data(sink,((const x_inst*)((&(type)))), sizeof( type ) );} \
  static inline void x_bcml_push_flag( x_sink* sink, bl_t flag ){ u0_t v = flag ? 1 : 0; x_sink_push_data(sink,((const x_inst*)((&(v)))), 1 );} \
  static inline void x_bcml_push_size( x_sink* sink, s3_t size ){x_sink_push_data(sink,((const x_inst*)((&(size)))), sizeof( size ) );}

/**********************************************************************************************************************/
// source: bcore_x_btcl.h

//----------------------------------------------------------------------------------------------------------------------
// group: x_btcl; embeds: bcore_x_btcl.x bcore_x_btcl_builtin.x bcore_x_btcl_op.x

#define TYPEOF_x_btcl 0xEDA9A5C25324F131ull
#define TYPEOF_x_btcl_spect_s 0x235E6DC4678C61EDull
#define TYPEOF_x_btcl_s 0x56270522F5DAAAFBull
#define BETH_EXPAND_ITEM_x_btcl_s \
  BCORE_DECLARE_OBJECT( x_btcl_s ) \
  { \
      aware_t _; \
      bcore_arr_st_s* eval_path_arr; \
  }; \
  static inline er_t x_btcl_s_from_source( const x_btcl_s* o, x_btcl* obj, x_source* source ); \
  static inline er_t x_btcl_s_t_from_file( const x_btcl_s* o, x_btcl* obj, tp_t t, sc_t file ); \
  static inline er_t x_btcl_s_from_file( const x_btcl_s* o, x_btcl* obj, sc_t file ); \
  static inline er_t x_btcl_s_t_from_st( const x_btcl_s* o, x_btcl* obj, tp_t t, const st_s* st ); \
  static inline er_t x_btcl_s_from_st( const x_btcl_s* o, x_btcl* obj, const st_s* st ); \
  static inline er_t x_btcl_s_t_from_sc( const x_btcl_s* o, x_btcl* obj, tp_t t, sc_t sc ); \
  static inline er_t x_btcl_s_from_sc( const x_btcl_s* o, x_btcl* obj, sc_t sc ); \
  static inline x_btcl* x_btcl_s_create_from_st_t( const x_btcl_s* o, const st_s* st, tp_t* type ); \
  static inline x_btcl* x_btcl_s_create_from_sc_t( const x_btcl_s* o, sc_t sc, tp_t* type ); \
  static inline x_btcl* x_btcl_s_create_from_file_t( const x_btcl_s* o, sc_t file, tp_t* type ); \
  static inline x_btcl* x_btcl_s_create_from_st( const x_btcl_s* o, const st_s* st ); \
  static inline x_btcl* x_btcl_s_create_from_sc( const x_btcl_s* o, sc_t sc ); \
  static inline x_btcl* x_btcl_s_create_from_file( const x_btcl_s* o, sc_t file ); \
  static inline er_t x_btcl_s_run_from_st( const x_btcl_s* o, const st_s* st ); \
  static inline er_t x_btcl_s_run_from_sc( const x_btcl_s* o, sc_t sc ); \
  static inline er_t x_btcl_s_run_from_file( const x_btcl_s* o, sc_t file ); \
  er_t x_btcl_s_t_from_source( const x_btcl_s* o, x_btcl* obj, tp_t t, x_source* source ); \
  x_btcl* x_btcl_s_create_from_source_t( const x_btcl_s* o, x_source* source, tp_t* type ); \
  x_btcl* x_btcl_s_create_from_source( const x_btcl_s* o, x_source* source ); \
  er_t x_btcl_s_run_from_source( const x_btcl_s* o, x_source* source ); \
  er_t x_btcl_s_parse_create_object( const x_btcl_s* o, x_source* source, sr_s* obj ); \
  static inline er_t x_btcl_s_from_source( const x_btcl_s* o, x_btcl* obj, x_source* source ){return  x_btcl_s_t_from_source(o,obj, obj->_, source );} \
  static inline er_t x_btcl_s_t_from_file( const x_btcl_s* o, x_btcl* obj, tp_t t, sc_t file ){BLM_INIT_LEVEL(0);BLM_RETURNV(er_t, x_btcl_s_t_from_source(o,obj, t, ((x_source*)BLM_LEVEL_A_PUSH(0,x_source_check_create_from_file(file ))) ))} \
  static inline er_t x_btcl_s_from_file( const x_btcl_s* o, x_btcl* obj, sc_t file ){return  x_btcl_s_t_from_file(o,obj, obj->_, file );} \
  static inline er_t x_btcl_s_t_from_st( const x_btcl_s* o, x_btcl* obj, tp_t t, const st_s* st ){BLM_INIT_LEVEL(0);BLM_RETURNV(er_t, x_btcl_s_t_from_source(o,obj, t, ((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_st(st ))) ))} \
  static inline er_t x_btcl_s_from_st( const x_btcl_s* o, x_btcl* obj, const st_s* st ){return  x_btcl_s_t_from_st(o,obj, obj->_, st );} \
  static inline er_t x_btcl_s_t_from_sc( const x_btcl_s* o, x_btcl* obj, tp_t t, sc_t sc ){BLM_INIT_LEVEL(0);BLM_RETURNV(er_t, x_btcl_s_t_from_source(o,obj, t, ((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_sc(sc ))) ))} \
  static inline er_t x_btcl_s_from_sc( const x_btcl_s* o, x_btcl* obj, sc_t sc ){return  x_btcl_s_t_from_sc(o,obj, obj->_, sc );} \
  static inline x_btcl* x_btcl_s_create_from_st_t( const x_btcl_s* o, const st_s* st, tp_t* type ){BLM_INIT_LEVEL(0);BLM_RETURNV(x_btcl*, x_btcl_s_create_from_source_t(o,((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_st(st ))), type ))} \
  static inline x_btcl* x_btcl_s_create_from_sc_t( const x_btcl_s* o, sc_t sc, tp_t* type ){BLM_INIT_LEVEL(0);BLM_RETURNV(x_btcl*, x_btcl_s_create_from_source_t(o,((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_sc(sc ))), type ))} \
  static inline x_btcl* x_btcl_s_create_from_file_t( const x_btcl_s* o, sc_t file, tp_t* type ){BLM_INIT_LEVEL(0);BLM_RETURNV(x_btcl*, x_btcl_s_create_from_source_t(o,((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_file(file ))), type ))} \
  static inline x_btcl* x_btcl_s_create_from_st( const x_btcl_s* o, const st_s* st ){BLM_INIT_LEVEL(0);BLM_RETURNV(x_btcl*, x_btcl_s_create_from_source(o,((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_st(st ))) ))} \
  static inline x_btcl* x_btcl_s_create_from_sc( const x_btcl_s* o, sc_t sc ){BLM_INIT_LEVEL(0);BLM_RETURNV(x_btcl*, x_btcl_s_create_from_source(o,((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_sc(sc ))) ))} \
  static inline x_btcl* x_btcl_s_create_from_file( const x_btcl_s* o, sc_t file ){BLM_INIT_LEVEL(0);BLM_RETURNV(x_btcl*, x_btcl_s_create_from_source(o,((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_file(file ))) ))} \
  static inline er_t x_btcl_s_run_from_st( const x_btcl_s* o, const st_s* st ){BLM_INIT_LEVEL(0);BLM_RETURNV(er_t, x_btcl_s_run_from_source(o,((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_st(st ))) ))} \
  static inline er_t x_btcl_s_run_from_sc( const x_btcl_s* o, sc_t sc ){BLM_INIT_LEVEL(0);BLM_RETURNV(er_t, x_btcl_s_run_from_source(o,((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_sc(sc ))) ))} \
  static inline er_t x_btcl_s_run_from_file( const x_btcl_s* o, sc_t file ){BLM_INIT_LEVEL(0);BLM_RETURNV(er_t, x_btcl_s_run_from_source(o,((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_file(file ))) ))}
#define TYPEOF_member 0x8CADA599D66EC543ull
#define TYPEOF_frame 0xD8D4335628B35226ull
#define TYPEOF_exp 0xC3120B18F0704408ull
#define TYPEOF_log 0x125073191DAF5431ull
#define TYPEOF_log2 0xCE0796AD70EBC119ull
#define TYPEOF_log10 0x029C1FB6E090C590ull
#define TYPEOF_sin 0x8248A1195CECC4ADull
#define TYPEOF_cos 0xF604FC190D0176DCull
#define TYPEOF_tan 0x56D7B2194448B0D8ull
#define TYPEOF_tanh 0xD93450EF07843B10ull
#define TYPEOF_sign 0x4E0F7F18E637A364ull
#define TYPEOF_sqrt 0x9614CF190FC8D7CFull
#define TYPEOF_abs 0xE71FB2190541727Bull
#define TYPEOF_ceil 0xD9951890FD6D7E88ull
#define TYPEOF_floor 0xA03A74A942DB43BDull
#define TYPEOF_max 0x080F5119176D1FF9ull
#define TYPEOF_min 0x07F4431917564077ull
#define TYPEOF_ife 0x2B84E0192BBDCBD9ull
#define TYPEOF_size 0x4DEA9618E618AE3Cull
#define TYPEOF_identity 0xE90F184C6AA3FFBBull
#define TYPEOF_neg 0x2138E5192571D261ull
#define TYPEOF_not 0x215AD619258E9F4Aull
#define TYPEOF_print 0x2F0792248C7D6068ull
#define TYPEOF_println 0x776357686C3FA546ull
#define TYPEOF_printx 0x673D641AB90A3B30ull
#define TYPEOF_assert 0xF89E75BDB43C5A5Bull
#define TYPEOF_mkdir 0xB8EAA574819A7FF8ull
#define TYPEOF_rmdir 0x77DFA1348BAE30DFull
#define TYPEOF_file_exists 0x768CE74BE3F48C84ull
#define TYPEOF_to_file 0x40A29AC7CC872287ull
#define TYPEOF_to_file_btml 0xA7C6C84DBBBE767Full
#define TYPEOF_to_file_bbml 0x430ECE4E1384A055ull
#define TYPEOF_to_file_bcml 0x49FDF54E16F45D16ull
#define TYPEOF_from_file 0x24E5E8AF273BB218ull
#define TYPEOF_from_file_btml 0x67977335DE4FEC52ull
#define TYPEOF_from_file_bbml 0x10DCF535AD2806DCull
#define TYPEOF_from_file_bcml 0x06A2AE35A6EBB273ull
#define TYPEOF_pow 0x779B5F19564F3B35ull
#define TYPEOF_div 0xCAA83A18F46E5888ull
#define TYPEOF_mod 0x0808591917670EE3ull
#define TYPEOF_chain 0xA74390764C1A3056ull
#define TYPEOF_mul_dot_colon 0xB36CC707CF595B59ull
#define TYPEOF_mul_dot 0x03693B069333F781ull
#define TYPEOF_mul_colon 0x49D715148F547B79ull
#define TYPEOF_mul 0x07E66519174A3CE1ull
#define TYPEOF_sub 0x82719D195D0FC2F5ull
#define TYPEOF_add 0xE70BC3190530E654ull
#define TYPEOF_equal 0x6A7933D70F43FAAFull
#define TYPEOF_unequal 0x1894146C8D27F1EEull
#define TYPEOF_larger_equal 0xA8CA74A2DB9B723Dull
#define TYPEOF_larger 0x87AD347436F18212ull
#define TYPEOF_smaller_equal 0xB8B98A36FD605BE4ull
#define TYPEOF_smaller 0x004C3DBE7CBBB5FBull
#define TYPEOF_and 0xE6F79719051FF286ull
#define TYPEOF_or 0x08B05407B5565CA4ull
#define TYPEOF_conditional 0xD811AB92FB7509D3ull
#define TYPEOF_spawn 0x4328F78AB20E1F98ull
#define TYPEOF_cat 0xF5E307190CE4A327ull
#define TYPEOF_shift_left 0xF19BCC61BF3DFF99ull
#define TYPEOF_assign 0x8FA771BD787ED1ECull
#define TYPEOF_continuation 0xA0DB2A42F2531912ull
#define TYPEOF_if 0x08B73007B55C3E26ull
#define TYPEOF_else 0x7F2B6C605332DD30ull
#define TYPEOF_self 0x2D19E518D40792B7ull
#define TYPEOF_func 0x86628378DB071EA7ull
#define TYPEOF_embed 0x0B0E48B4160DCA6Eull
#define TYPEOF_eval_file 0x5FDA6E0A288B6666ull
#define TYPEOF_eval_string 0xACE57FD123E16C39ull
#define TYPEOF_embed_file 0xFE0A28D0AB7F183Dull
#define TYPEOF_embed_string 0x9FBF83848C0F0A46ull
#define TYPEOF_prefix 0xBA8821B572531E29ull
#define TYPEOF_x_btcl_context_s 0x833EA022A3607597ull
#define BETH_EXPAND_ITEM_x_btcl_context_s \
  BCORE_DECLARE_OBJECT( x_btcl_context_s ) \
  { \
      aware_t _; \
      bcore_arr_st_s* eval_path_arr; \
      bcore_hmap_name_s hmap_reserved_key; \
      bcore_hmap_name_s hmap_reserved_func; \
      bcore_hmap_name_s hmap_reserved_const; \
  }; \
  x_btcl_context_s* x_btcl_context_s_setup( x_btcl_context_s* o ); \
  x_btcl_context_s* x_btcl_context_s_set_reserved_keys( x_btcl_context_s* o ); \
  static inline bl_t x_btcl_context_s_is_reserved_key( const x_btcl_context_s* o, tp_t name ); \
  static inline bl_t x_btcl_context_s_is_reserved_func( const x_btcl_context_s* o, tp_t name ); \
  static inline bl_t x_btcl_context_s_is_reserved_const( const x_btcl_context_s* o, tp_t name ); \
  static inline sc_t x_btcl_context_s_sc_reserved_key( const x_btcl_context_s* o, tp_t name ); \
  static inline sc_t x_btcl_context_s_sc_reserved_func( const x_btcl_context_s* o, tp_t name ); \
  static inline sc_t x_btcl_context_s_sc_reserved_const( const x_btcl_context_s* o, tp_t name ); \
  bl_t x_btcl_context_s_is_reserved( const x_btcl_context_s* o, tp_t name ); \
  sc_t x_btcl_context_s_sc_reserved( const x_btcl_context_s* o, tp_t name ); \
  er_t x_btcl_context_s_check_reserved( const x_btcl_context_s* o, tp_t name, x_source* source ); \
  er_t x_btcl_context_s_get_embedding_file_path( const x_btcl_context_s* o, x_source* source, sc_t in_path, st_s* out_path ); \
  x_btcl_context_s* x_btcl_context_s_set_reserved_funcs( x_btcl_context_s* o ); \
  x_btcl_context_s* x_btcl_context_s_set_reserved_consts( x_btcl_context_s* o ); \
  static inline bl_t x_btcl_context_s_is_reserved_key( const x_btcl_context_s* o, tp_t name ){return  bcore_hmap_name_s_exists(&(o->hmap_reserved_key),name );} \
  static inline bl_t x_btcl_context_s_is_reserved_func( const x_btcl_context_s* o, tp_t name ){return  bcore_hmap_name_s_exists(&(o->hmap_reserved_func),name );} \
  static inline bl_t x_btcl_context_s_is_reserved_const( const x_btcl_context_s* o, tp_t name ){return  bcore_hmap_name_s_exists(&(o->hmap_reserved_const),name );} \
  static inline sc_t x_btcl_context_s_sc_reserved_key( const x_btcl_context_s* o, tp_t name ){return  bcore_hmap_name_s_get_sc(&(o->hmap_reserved_key),name );} \
  static inline sc_t x_btcl_context_s_sc_reserved_func( const x_btcl_context_s* o, tp_t name ){return  bcore_hmap_name_s_get_sc(&(o->hmap_reserved_func),name );} \
  static inline sc_t x_btcl_context_s_sc_reserved_const( const x_btcl_context_s* o, tp_t name ){return  bcore_hmap_name_s_get_sc(&(o->hmap_reserved_const),name );}
#define TYPEOF_x_btcl_frame_s 0xF70F07C4792D4F4Dull
#define BETH_EXPAND_ITEM_x_btcl_frame_s \
  BCORE_DECLARE_OBJECT( x_btcl_frame_s ) \
  { \
      aware_t _; \
      x_btcl_frame_s* parent; \
      x_btcl_context_s* context; \
      sz_t depth; \
      sz_t eval_depth; \
      bcore_arr_sr_s obj_pool; \
      bcore_hmap_name_s hmap_name; \
      bcore_hmap_tp_sr_s var_map; \
  }; \
  static inline bl_t x_btcl_frame_s_is_reserved_key( const x_btcl_frame_s* o, tp_t name ); \
  static inline sc_t x_btcl_frame_s_sc_reserved_key( const x_btcl_frame_s* o, tp_t name ); \
  static inline bl_t x_btcl_frame_s_is_reserved_func( const x_btcl_frame_s* o, tp_t name ); \
  static inline sc_t x_btcl_frame_s_sc_reserved_func( const x_btcl_frame_s* o, tp_t name ); \
  static inline bl_t x_btcl_frame_s_is_reserved_const( const x_btcl_frame_s* o, tp_t name ); \
  static inline sc_t x_btcl_frame_s_sc_reserved_const( const x_btcl_frame_s* o, tp_t name ); \
  static inline bl_t x_btcl_frame_s_is_reserved( const x_btcl_frame_s* o, tp_t name ); \
  static inline sc_t x_btcl_frame_s_sc_reserved( const x_btcl_frame_s* o, tp_t name ); \
  static inline er_t x_btcl_frame_s_check_reserved( const x_btcl_frame_s* o, tp_t name, x_source* source ); \
  x_btcl_frame_s* x_btcl_frame_s_setup_as_root( x_btcl_frame_s* o, x_btcl_context_s* context ); \
  x_btcl_frame_s* x_btcl_frame_s_setup( x_btcl_frame_s* o, const x_btcl_frame_s* parent ); \
  x_btcl_frame_s* x_btcl_frame_s_preserve_and_set_weak( x_btcl_frame_s* o, sr_s* sr ); \
  static inline tp_t x_btcl_frame_s_entypeof( x_btcl_frame_s* o, sc_t name ); \
  sc_t x_btcl_frame_s_nameof( const x_btcl_frame_s* o, tp_t type ); \
  bl_t x_btcl_frame_s_var_exists( const x_btcl_frame_s* o, tp_t name ); \
  sr_s* x_btcl_frame_s_var_get( const x_btcl_frame_s* o, tp_t name ); \
  sr_s* x_btcl_frame_s_var_set( x_btcl_frame_s* o, tp_t name, sr_s sr ); \
  x_btcl_frame_s* x_btcl_frame_s_import_forked_vars( x_btcl_frame_s* o, tp_t prefix, x_btcl_frame_s* src_frame ); \
  er_t x_btcl_frame_s_generic_copy( x_btcl_frame_s* o, const x_source_point_s* sp, sr_s* sr, const sr_s* sb ); \
  tp_t x_btcl_frame_s_get_identifier( x_btcl_frame_s* o, x_source* source, bl_t take_from_source ); \
  er_t x_btcl_frame_s_eval_condition( x_btcl_frame_s* o, x_source* source, bl_t* condition ); \
  er_t x_btcl_frame_s_eval_in_frame( const x_btcl_frame_s* o, s2_t priority, x_source* source, sr_s* obj ); \
  er_t x_btcl_frame_s_eval( x_btcl_frame_s* o, s2_t exit_priority, x_source* source, sr_s* obj ); \
  er_t x_btcl_frame_s_parse_create_final_object( x_btcl_frame_s* o, x_source* source, sr_s* obj ); \
  er_t x_btcl_frame_s_eval_reserved_const( x_btcl_frame_s* o, tp_t name, x_source* source, sr_s* sr ); \
  er_t x_btcl_frame_s_eval_reserved_func( x_btcl_frame_s* o, tp_t name, x_source* source, bl_t postfix, sr_s* sr ); \
  er_t x_btcl_frame_s_eval_op_member( x_btcl_frame_s* o, x_source* source, sr_s* sr ); \
  er_t x_btcl_frame_s_eval_op_functional( x_btcl_frame_s* o, x_source* source, sr_s* sr ); \
  er_t x_btcl_frame_s_eval_op_modifier( x_btcl_frame_s* o, x_source* source, sr_s* sr ); \
  er_t x_btcl_frame_s_eval_op_func_list_unfold( x_btcl_frame_s* o, s2_t op_priority, x_source* source, sr_s* sr ); \
  er_t x_btcl_frame_s_eval_op_func_list_transform( x_btcl_frame_s* o, s2_t op_priority, x_source* source, sr_s* sr ); \
  er_t x_btcl_frame_s_eval_op_func_list_unfold_transform( x_btcl_frame_s* o, s2_t op_priority, x_source* source, sr_s* sr ); \
  er_t x_btcl_frame_s_eval_op_assign( x_btcl_frame_s* o, s2_t op_priority, x_source* source, sr_s* sr ); \
  er_t x_btcl_frame_s_eval_op_continuation( x_btcl_frame_s* o, s2_t op_priority, x_source* source, sr_s* sr ); \
  er_t x_btcl_frame_s_eval_op( x_btcl_frame_s* o, s2_t exit_priority, x_source* source, sr_s* obj ); \
  static inline bl_t x_btcl_frame_s_is_reserved_key( const x_btcl_frame_s* o, tp_t name ){return  x_btcl_context_s_is_reserved_key(o->context,name );} \
  static inline sc_t x_btcl_frame_s_sc_reserved_key( const x_btcl_frame_s* o, tp_t name ){return  x_btcl_context_s_sc_reserved_key(o->context,name );} \
  static inline bl_t x_btcl_frame_s_is_reserved_func( const x_btcl_frame_s* o, tp_t name ){return  x_btcl_context_s_is_reserved_func(o->context,name );} \
  static inline sc_t x_btcl_frame_s_sc_reserved_func( const x_btcl_frame_s* o, tp_t name ){return  x_btcl_context_s_sc_reserved_func(o->context,name );} \
  static inline bl_t x_btcl_frame_s_is_reserved_const( const x_btcl_frame_s* o, tp_t name ){return  x_btcl_context_s_is_reserved_const(o->context,name );} \
  static inline sc_t x_btcl_frame_s_sc_reserved_const( const x_btcl_frame_s* o, tp_t name ){return  x_btcl_context_s_sc_reserved_const(o->context,name );} \
  static inline bl_t x_btcl_frame_s_is_reserved( const x_btcl_frame_s* o, tp_t name ){return  x_btcl_context_s_is_reserved(o->context,name );} \
  static inline sc_t x_btcl_frame_s_sc_reserved( const x_btcl_frame_s* o, tp_t name ){return  x_btcl_context_s_sc_reserved(o->context,name );} \
  static inline er_t x_btcl_frame_s_check_reserved( const x_btcl_frame_s* o, tp_t name, x_source* source ){return  x_btcl_context_s_check_reserved(o->context,name, source );} \
  static inline tp_t x_btcl_frame_s_entypeof( x_btcl_frame_s* o, sc_t name ){return  bcore_hmap_name_s_set_sc(&(o->hmap_name),name );}
#define TYPEOF_x_btcl_null_variable_s 0x636781C0E7610236ull
#define BETH_EXPAND_ITEM_x_btcl_null_variable_s \
  BCORE_DECLARE_OBJECT( x_btcl_null_variable_s ) \
  { \
      aware_t _; \
      tp_t tp_name; \
  }; \
  x_btcl_null_variable_s* x_btcl_null_variable_s__( x_btcl_null_variable_s* o, tp_t tp_name );
#define TYPEOF_x_btcl_null_member_s 0x388AF54280329140ull
#define BETH_EXPAND_ITEM_x_btcl_null_member_s \
  BCORE_DECLARE_OBJECT( x_btcl_null_member_s ) \
  { \
      aware_t _; \
      sr_s base; \
      tp_t tp_name; \
  }; \
  x_btcl_null_member_s* x_btcl_null_member_s_setup( x_btcl_null_member_s* o, sr_s* base, tp_t tp_name ); \
  sr_s x_btcl_null_member_s_set_sr( x_btcl_null_member_s* o, x_btcl_frame_s* frame, const x_source_point_s* sp, sr_s* src );
#define TYPEOF_x_btcl_null_arr_element_s 0x528F7BF4C984AE7Aull
#define BETH_EXPAND_ITEM_x_btcl_null_arr_element_s \
  BCORE_DECLARE_OBJECT( x_btcl_null_arr_element_s ) \
  { \
      aware_t _; \
      sr_s base; \
      s3_t index; \
  }; \
  x_btcl_null_arr_element_s* x_btcl_null_arr_element_s_setup( x_btcl_null_arr_element_s* o, sr_s* base, tp_t index ); \
  sr_s x_btcl_null_arr_element_s_set_sr( x_btcl_null_arr_element_s* o, x_btcl_frame_s* frame, const x_source_point_s* sp, sr_s* src );
#define TYPEOF_x_btcl_list_s 0xDBF13B14DA3E0C18ull
#define BETH_EXPAND_ITEM_x_btcl_list_s \
  BCORE_DECLARE_OBJECT( x_btcl_list_s ) \
  { \
      aware_t _; \
      bcore_arr_sr_s arr; \
  }; \
  static inline sz_t x_btcl_list_s_size( const x_btcl_list_s* o ); \
  static inline x_btcl_list_s* x_btcl_list_s_set_size( x_btcl_list_s* o, sz_t size ); \
  static inline x_btcl_list_s* x_btcl_list_s_push_sr( x_btcl_list_s* o, sr_s sr ); \
  static inline x_btcl_list_s* x_btcl_list_s_push_clone( x_btcl_list_s* o, const sr_s* a ); \
  static inline x_btcl_list_s* x_btcl_list_s_push_fork( x_btcl_list_s* o, sr_s* a ); \
  static inline x_btcl_list_s* x_btcl_list_s_push_list_clone( x_btcl_list_s* o, const x_btcl_list_s* a ); \
  static inline x_btcl_list_s* x_btcl_list_s_push_list_fork( x_btcl_list_s* o, x_btcl_list_s* a ); \
  er_t x_btcl_list_s_to_sink( const x_btcl_list_s* o, bl_t detailed, x_sink* sink ); \
  static inline sz_t x_btcl_list_s_size( const x_btcl_list_s* o ){return  o->arr.size;} \
  static inline x_btcl_list_s* x_btcl_list_s_set_size( x_btcl_list_s* o, sz_t size ){bcore_arr_sr_s_set_size(&(o->arr),size );return o;} \
  static inline x_btcl_list_s* x_btcl_list_s_push_sr( x_btcl_list_s* o, sr_s sr ){bcore_arr_sr_s_push_sr(&(o->arr),sr );return o;} \
  static inline x_btcl_list_s* x_btcl_list_s_push_clone( x_btcl_list_s* o, const sr_s* a ){x_btcl_list_s_push_sr(o,sr_clone( sr_cw(*( a )) ) );return o;} \
  static inline x_btcl_list_s* x_btcl_list_s_push_fork( x_btcl_list_s* o, sr_s* a ){x_btcl_list_s_push_sr(o,sr_fork( sr_cw(*( a )) ) );return o;} \
  static inline x_btcl_list_s* x_btcl_list_s_push_list_clone( x_btcl_list_s* o, const x_btcl_list_s* a ){{const bcore_arr_sr_s* __a=&(a->arr );if(__a)for(sz_t __i=0;__i<__a->size;__i++){const sr_s* e=&(__a->data[__i]); x_btcl_list_s_push_clone(o,e );}}return o;} \
  static inline x_btcl_list_s* x_btcl_list_s_push_list_fork( x_btcl_list_s* o, x_btcl_list_s* a ){{const bcore_arr_sr_s* __a=&(a->arr );if(__a)for(sz_t __i=0;__i<__a->size;__i++){sr_s* e=&(__a->data[__i]); x_btcl_list_s_push_fork(o,e );}}return o;}
#define TYPEOF_x_btcl_block_s 0x7313F38FDCAB8A79ull
#define BETH_EXPAND_ITEM_x_btcl_block_s \
  BCORE_DECLARE_OBJECT( x_btcl_block_s ) \
  { \
      aware_t _; \
      x_source_point_s source_point; \
  }; \
  er_t x_btcl_block_s_parse( x_btcl_block_s* o, x_btcl_frame_s* frame, x_source* source ); \
  er_t x_btcl_block_s_eval( const x_btcl_block_s* o, const x_btcl_frame_s* parent_frame, sr_s* obj );
#define TYPEOF_x_btcl_signature_s 0x7E995B597F71CB9Aull
#define BETH_EXPAND_ITEM_x_btcl_signature_s \
  BCORE_DECLARE_OBJECT( x_btcl_signature_s ) \
  { \
      aware_t _; \
      x_source_point_s source_point; \
      bcore_arr_tp_s arg_list; \
  }; \
  static inline sz_t x_btcl_signature_s_args( const x_btcl_signature_s* o ); \
  static inline tp_t x_btcl_signature_s_arg_name( const x_btcl_signature_s* o, sz_t index ); \
  static inline bl_t x_btcl_signature_s_is_unary( const x_btcl_signature_s* o ); \
  er_t x_btcl_signature_s_parse( x_btcl_signature_s* o, x_btcl_frame_s* frame, x_source* source ); \
  static inline sz_t x_btcl_signature_s_args( const x_btcl_signature_s* o ){return  o->arg_list.size;} \
  static inline tp_t x_btcl_signature_s_arg_name( const x_btcl_signature_s* o, sz_t index ){return  o->arg_list.data[ index ];} \
  static inline bl_t x_btcl_signature_s_is_unary( const x_btcl_signature_s* o ){return  ( o->arg_list.size == 1 );}
#define TYPEOF_x_btcl_external_function_s 0xADCFE4A8A00800B0ull
#define BETH_EXPAND_ITEM_x_btcl_external_function_s \
  BCORE_DECLARE_OBJECT( x_btcl_external_function_s ) \
  { \
      aware_t _; \
      tp_t name; \
      bl_t is_mutable; \
      x_btcl* object; \
  }; \
  x_btcl_external_function_s* x_btcl_external_function_s_setup( x_btcl_external_function_s* o, tp_t name, bl_t is_mutable, const x_btcl* object ); \
  er_t x_btcl_external_function_s_execute( x_btcl_external_function_s* o, const x_source_point_s* sp, x_btcl_frame_s* lexical_frame, const bcore_arr_sr_s* args, sr_s* sr );
#define TYPEOF_x_btcl_function_s 0xE67CE4B7F4BF0460ull
#define BETH_EXPAND_ITEM_x_btcl_function_s \
  BCORE_DECLARE_OBJECT( x_btcl_function_s ) \
  { \
      aware_t _; \
      x_btcl_signature_s* signature; \
      x_btcl_block_s* block; \
      x_btcl_external_function_s* external_function; \
      x_btcl_function_s* wrapped_function; \
      bcore_arr_sr_s* wrapped_arg_list; \
      x_btcl_function_s* tail; \
  }; \
  x_btcl_function_s* x_btcl_function_s_setup( x_btcl_function_s* o, x_btcl_signature_s* signature, x_btcl_block_s* block, const x_btcl_function_s* tail ); \
  x_btcl_function_s* x_btcl_function_s_setup_external_function( x_btcl_function_s* o, tp_t name, s2_t arity, bl_t is_mutable, const x_btcl* object ); \
  x_btcl_function_s* x_btcl_function_s_setup_wrapped_function( x_btcl_function_s* o, x_btcl_function_s* wrapped_function, bcore_arr_sr_s* wrapped_arg_list ); \
  x_btcl_function_s* x_btcl_function_s_append_tail( x_btcl_function_s* o, const x_btcl_function_s* tail ); \
  static inline sz_t x_btcl_function_s_args( const x_btcl_function_s* o ); \
  static inline tp_t x_btcl_function_s_arg_name( const x_btcl_function_s* o, sz_t index ); \
  static inline bl_t x_btcl_function_s_is_unary( const x_btcl_function_s* o ); \
  er_t x_btcl_function_s_setup_frame( x_btcl_function_s* o, x_btcl_frame_s* lexical_frame, x_source_point_s* source_point, x_btcl_frame_s* frame ); \
  er_t x_btcl_function_s_call( x_btcl_function_s* o, x_source_point_s* source_point, x_btcl_frame_s* lexical_frame, bcore_arr_sr_s* arg_list, sr_s* result ); \
  er_t x_btcl_function_s_call_unary( x_btcl_function_s* o, x_source_point_s* source_point, x_btcl_frame_s* lexical_frame, sr_s* s_arg, sr_s* result ); \
  er_t x_btcl_function_s_call_via_arg_list( x_btcl_function_s* o, x_source_point_s* source_point, x_btcl_frame_s* lexical_frame, x_btcl_list_s* arg_list1, sr_s* result ); \
  er_t x_btcl_function_s_call_via_evaluation( x_btcl_function_s* o, x_source* source, x_btcl_frame_s* lexical_frame, sr_s* sr ); \
  static inline sz_t x_btcl_function_s_args( const x_btcl_function_s* o ){return  x_btcl_signature_s_args(o->signature);} \
  static inline tp_t x_btcl_function_s_arg_name( const x_btcl_function_s* o, sz_t index ){return  x_btcl_signature_s_arg_name(o->signature,index );} \
  static inline bl_t x_btcl_function_s_is_unary( const x_btcl_function_s* o ){return  x_btcl_signature_s_is_unary(o->signature);}
#define TYPEOF_SIZE 0x62DC57244A2CB13Cull
#define TYPEOF_EXP 0xD9874B198E84A5A8ull
#define TYPEOF_LOG 0x2560B319B8E23FD1ull
#define TYPEOF_LOG2 0x659037B528728EB9ull
#define TYPEOF_LOG10 0x06931FD3BA9FBFB0ull
#define TYPEOF_SIN 0x97E4E119FA49358Dull
#define TYPEOF_COS 0x0BA0FC19AA5D7AFCull
#define TYPEOF_TAN 0x6C73F219E1A521B8ull
#define TYPEOF_TANH 0xEE2651FA6B98AAD0ull
#define TYPEOF_SIGN 0x630140244A4BA664ull
#define TYPEOF_SQRT 0xAA2CD0247323A3CFull
#define TYPEOF_ABS 0xFA2FF219A0745E1Bull
#define TYPEOF_CEIL 0xEDACD99C60C7DDC8ull
#define TYPEOF_FLOOR 0xCA975D16304CB71Dull
#define TYPEOF_MAX 0x1E855119B582C7D9ull
#define TYPEOF_MIN 0x1E6A4319B56BE857ull
#define TYPEOF_IFE 0x41212019C91A3CB9ull
#define TYPEOF_PRINT 0xADC3387E5F0C2A48ull
#define TYPEOF_PRINTLN 0xB722269D563BC8E6ull
#define TYPEOF_PRINTX 0x4EE30EBB81AB7930ull
#define TYPEOF_ASSERT 0xD98C44AF4A2B839Bull
#define TYPEOF_MKDIR 0x0CCC8DE2A8526718ull
#define TYPEOF_RMDIR 0xCAE889A2B1AE273Full
#define TYPEOF_FILE_EXISTS 0xEDF53AFFC8673DC4ull
#define TYPEOF_TO_FILE 0x68CC91646125D547ull
#define TYPEOF_TO_FILE_BTML 0xF7E14FAE38491C7Full
#define TYPEOF_TO_FILE_BBML 0x940255AE90C73715ull
#define TYPEOF_TO_FILE_BCML 0x9E587CAE971BCFD6ull
#define TYPEOF_FROM_FILE 0xC02E870A0CE7F918ull
#define TYPEOF_FROM_FILE_BTML 0x8E99F95652681E12ull
#define TYPEOF_FROM_FILE_BBML 0x37E07B562141EB9Cull
#define TYPEOF_FROM_FILE_BCML 0x2DA5F4561B052A73ull
#define TYPEOF_true 0x5B5C98EF514DBFA5ull
#define TYPEOF_false 0xB5FAE2C14238B978ull
#define TYPEOF_TRUE 0x704E59FAB561C2A5ull
#define TYPEOF_FALSE 0x09DCCB2F68F0A098ull
#define TYPEOF_PI 0x09421F07B5D1F0B2ull
#define TYPEOF_PATH 0x17DD2E194F31D276ull
#define TYPEOF_DIR 0xE0447E1991CAD034ull
#define TYPEOF_parse 0x0C4E87A7A1F6FD8Cull
#define TYPEOF_parse_file 0xACF2E6FE47ED900Full
#define BETH_EXPAND_GROUP_x_btcl \
  BCORE_FORWARD_OBJECT( x_btcl ); \
  BCORE_FORWARD_OBJECT( x_btcl_s ); \
  BCORE_FORWARD_OBJECT( x_btcl_context_s ); \
  BCORE_FORWARD_OBJECT( x_btcl_frame_s ); \
  BCORE_FORWARD_OBJECT( x_btcl_null_variable_s ); \
  BCORE_FORWARD_OBJECT( x_btcl_null_member_s ); \
  BCORE_FORWARD_OBJECT( x_btcl_null_arr_element_s ); \
  BCORE_FORWARD_OBJECT( x_btcl_list_s ); \
  BCORE_FORWARD_OBJECT( x_btcl_block_s ); \
  BCORE_FORWARD_OBJECT( x_btcl_signature_s ); \
  BCORE_FORWARD_OBJECT( x_btcl_external_function_s ); \
  BCORE_FORWARD_OBJECT( x_btcl_function_s ); \
  BCORE_FORWARD_OBJECT( x_btcl_operator ); \
  BCORE_FORWARD_OBJECT( x_btcl_operator_f3 ); \
  BCORE_FORWARD_OBJECT( x_btcl_net ); \
  BCORE_FORWARD_OBJECT( x_btcl_functor ); \
  BCORE_FORWARD_OBJECT( x_btcl_functor_f3 ); \
  BCORE_FORWARD_OBJECT( x_btcl_random ); \
  BCORE_FORWARD_OBJECT( x_btcl_test ); \
  static inline er_t x_btcl_t_from_source( x_btcl* o, tp_t t, x_source* source ); \
  static inline er_t x_btcl_from_source( x_btcl* o, x_source* source ); \
  static inline er_t x_btcl_t_from_file( x_btcl* o, tp_t t, sc_t file ); \
  static inline er_t x_btcl_from_file( x_btcl* o, sc_t file ); \
  static inline er_t x_btcl_t_from_st( x_btcl* o, tp_t t, const st_s* st ); \
  static inline er_t x_btcl_from_st( x_btcl* o, const st_s* st ); \
  static inline er_t x_btcl_t_from_sc( x_btcl* o, tp_t t, sc_t sc ); \
  static inline er_t x_btcl_from_sc( x_btcl* o, sc_t sc ); \
  static inline x_btcl* x_btcl_create_from_source_t( x_source* source, tp_t* type ); \
  static inline x_btcl* x_btcl_create_from_st_t( const st_s* st, tp_t* type ); \
  static inline x_btcl* x_btcl_create_from_sc_t( sc_t sc, tp_t* type ); \
  static inline x_btcl* x_btcl_create_from_file_t( sc_t file, tp_t* type ); \
  static inline x_btcl* x_btcl_create_from_source( x_source* source ); \
  static inline x_btcl* x_btcl_create_from_st( const st_s* st ); \
  static inline x_btcl* x_btcl_create_from_sc( sc_t sc ); \
  static inline x_btcl* x_btcl_create_from_file( sc_t file ); \
  static inline er_t x_btcl_run_from_source( x_source* source ); \
  static inline er_t x_btcl_run_from_st( const st_s* st ); \
  static inline er_t x_btcl_run_from_sc( sc_t sc ); \
  static inline er_t x_btcl_run_from_file( sc_t file ); \
  static inline er_t x_btcl_parse_create_object( x_source* source, sr_s* obj ); \
  sc_t x_btcl_operator_symbol( tp_t type ); \
  static inline bl_t x_btcl_is_identifier( x_source* source ); \
  void x_btcl_clone_if_weak( sr_s* sr ); \
  void x_btcl_clone_if_weak_or_twice_referenced( sr_s* sr ); \
  er_t x_btcl_generic_copy( sr_s* sr, const sr_s* sb ); \
  static inline sz_t x_btcl_max_frame_depth( void ); \
  static inline sz_t x_btcl_max_frame_eval_depth( void ); \
  static inline s2_t x_btcl_priority_a( void ); \
  static inline s2_t x_btcl_priority_b( void ); \
  static inline s2_t x_btcl_priority_c( void ); \
  static inline s2_t x_btcl_priority_d( void ); \
  static inline s2_t x_btcl_priority_e( void ); \
  er_t x_btcl_to_sink( bl_t detailed, const sr_s* sr, x_sink* sink ); \
  er_t x_btcl_bop_cat_ab( sr_s* a, sr_s* b, sr_s* sr ); \
  void x_btcl_selftest( sc_t file ); \
  typedef sz_t (*x_btcl_btcl_function_arity)(const x_btcl* o, tp_t name ); \
  typedef er_t (*x_btcl_btcl_function)(const x_btcl* o, tp_t name, const x_source_point_s* sp, x_btcl_frame_s* lexical_frame, const bcore_arr_sr_s* args, sr_s* result ); \
  typedef er_t (*x_btcl_m_btcl_function)(x_btcl* o, tp_t name, const x_source_point_s* sp, x_btcl_frame_s* lexical_frame, const bcore_arr_sr_s* args, sr_s* result ); \
  typedef bl_t (*x_btcl_btcl_function_mutable)(const x_btcl* o, tp_t name ); \
  typedef er_t (*x_btcl_btcl_external_parse)(const x_btcl* o, x_source* source, x_btcl_frame_s* lexical_frame, sr_s* result ); \
  typedef er_t (*x_btcl_m_btcl_external_parse)(x_btcl* o, x_source* source, x_btcl_frame_s* lexical_frame, sr_s* result ); \
  typedef f3_t (*x_btcl_nullary_f3)(const x_btcl* o ); \
  typedef f3_t (*x_btcl_unary_f3)(const x_btcl* o, f3_t x ); \
  typedef f3_t (*x_btcl_binary_f3)(const x_btcl* o, f3_t x, f3_t y ); \
  typedef f3_t (*x_btcl_ternary_f3)(const x_btcl* o, f3_t x, f3_t y, f3_t z ); \
  XOILA_DECLARE_SPECT( x_btcl ) \
  { \
      bcore_spect_header_s header; \
      x_btcl_btcl_function_arity btcl_function_arity; \
      x_btcl_btcl_function btcl_function; \
      x_btcl_m_btcl_function m_btcl_function; \
      x_btcl_btcl_function_mutable btcl_function_mutable; \
      x_btcl_btcl_external_parse btcl_external_parse; \
      x_btcl_m_btcl_external_parse m_btcl_external_parse; \
      x_btcl_nullary_f3 nullary_f3; \
      x_btcl_unary_f3 unary_f3; \
      x_btcl_binary_f3 binary_f3; \
      x_btcl_ternary_f3 ternary_f3; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( x_btcl ) \
  BETH_EXPAND_ITEM_x_btcl_s \
  static inline sz_t x_btcl_a_btcl_function_arity( const x_btcl* o, tp_t name ){ const x_btcl_spect_s* p = x_btcl_spect_s_get_aware( o ); assert( p->btcl_function_arity ); return p->btcl_function_arity( o, name );} \
  static inline bl_t x_btcl_defines_btcl_function_arity( const x_btcl* o ){ return  true;} \
  static inline sz_t x_btcl_t_btcl_function_arity( const x_btcl* o, tp_t t, tp_t name ){ const x_btcl_spect_s* p = x_btcl_spect_s_get_typed( t ); assert( p->btcl_function_arity ); return p->btcl_function_arity( o, name );} \
  static inline bl_t x_btcl_t_defines_btcl_function_arity( tp_t t ){ return  true;} \
  static inline sz_t x_btcl_btcl_function_arity_default( const x_btcl* o, tp_t name ){return  -1;} \
  static inline er_t x_btcl_a_btcl_function( const x_btcl* o, tp_t name, const x_source_point_s* sp, x_btcl_frame_s* lexical_frame, const bcore_arr_sr_s* args, sr_s* result ){ const x_btcl_spect_s* p = x_btcl_spect_s_get_aware( o ); assert( p->btcl_function ); return p->btcl_function( o, name, sp, lexical_frame, args, result );} \
  static inline bl_t x_btcl_defines_btcl_function( const x_btcl* o ){  return x_btcl_spect_s_get_aware( o )->btcl_function != NULL;} \
  static inline er_t x_btcl_t_btcl_function( const x_btcl* o, tp_t t, tp_t name, const x_source_point_s* sp, x_btcl_frame_s* lexical_frame, const bcore_arr_sr_s* args, sr_s* result ){ const x_btcl_spect_s* p = x_btcl_spect_s_get_typed( t ); assert( p->btcl_function ); return p->btcl_function( o, name, sp, lexical_frame, args, result );} \
  static inline bl_t x_btcl_t_defines_btcl_function( tp_t t ){  return x_btcl_spect_s_get_typed( t )->btcl_function != NULL;} \
  static inline er_t x_btcl_a_m_btcl_function( x_btcl* o, tp_t name, const x_source_point_s* sp, x_btcl_frame_s* lexical_frame, const bcore_arr_sr_s* args, sr_s* result ){ const x_btcl_spect_s* p = x_btcl_spect_s_get_aware( o ); assert( p->m_btcl_function ); return p->m_btcl_function( o, name, sp, lexical_frame, args, result );} \
  static inline bl_t x_btcl_defines_m_btcl_function( const x_btcl* o ){  return x_btcl_spect_s_get_aware( o )->m_btcl_function != NULL;} \
  static inline er_t x_btcl_t_m_btcl_function( x_btcl* o, tp_t t, tp_t name, const x_source_point_s* sp, x_btcl_frame_s* lexical_frame, const bcore_arr_sr_s* args, sr_s* result ){ const x_btcl_spect_s* p = x_btcl_spect_s_get_typed( t ); assert( p->m_btcl_function ); return p->m_btcl_function( o, name, sp, lexical_frame, args, result );} \
  static inline bl_t x_btcl_t_defines_m_btcl_function( tp_t t ){  return x_btcl_spect_s_get_typed( t )->m_btcl_function != NULL;} \
  static inline bl_t x_btcl_a_btcl_function_mutable( const x_btcl* o, tp_t name ){ const x_btcl_spect_s* p = x_btcl_spect_s_get_aware( o ); assert( p->btcl_function_mutable ); return p->btcl_function_mutable( o, name );} \
  static inline bl_t x_btcl_defines_btcl_function_mutable( const x_btcl* o ){ return  true;} \
  static inline bl_t x_btcl_t_btcl_function_mutable( const x_btcl* o, tp_t t, tp_t name ){ const x_btcl_spect_s* p = x_btcl_spect_s_get_typed( t ); assert( p->btcl_function_mutable ); return p->btcl_function_mutable( o, name );} \
  static inline bl_t x_btcl_t_defines_btcl_function_mutable( tp_t t ){ return  true;} \
  static inline bl_t x_btcl_btcl_function_mutable_default( const x_btcl* o, tp_t name ){return  x_btcl_defines_m_btcl_function(o);} \
  static inline er_t x_btcl_a_btcl_external_parse( const x_btcl* o, x_source* source, x_btcl_frame_s* lexical_frame, sr_s* result ){ const x_btcl_spect_s* p = x_btcl_spect_s_get_aware( o ); assert( p->btcl_external_parse ); return p->btcl_external_parse( o, source, lexical_frame, result );} \
  static inline bl_t x_btcl_defines_btcl_external_parse( const x_btcl* o ){  return x_btcl_spect_s_get_aware( o )->btcl_external_parse != NULL;} \
  static inline er_t x_btcl_t_btcl_external_parse( const x_btcl* o, tp_t t, x_source* source, x_btcl_frame_s* lexical_frame, sr_s* result ){ const x_btcl_spect_s* p = x_btcl_spect_s_get_typed( t ); assert( p->btcl_external_parse ); return p->btcl_external_parse( o, source, lexical_frame, result );} \
  static inline bl_t x_btcl_t_defines_btcl_external_parse( tp_t t ){  return x_btcl_spect_s_get_typed( t )->btcl_external_parse != NULL;} \
  static inline er_t x_btcl_a_m_btcl_external_parse( x_btcl* o, x_source* source, x_btcl_frame_s* lexical_frame, sr_s* result ){ const x_btcl_spect_s* p = x_btcl_spect_s_get_aware( o ); assert( p->m_btcl_external_parse ); return p->m_btcl_external_parse( o, source, lexical_frame, result );} \
  static inline bl_t x_btcl_defines_m_btcl_external_parse( const x_btcl* o ){  return x_btcl_spect_s_get_aware( o )->m_btcl_external_parse != NULL;} \
  static inline er_t x_btcl_t_m_btcl_external_parse( x_btcl* o, tp_t t, x_source* source, x_btcl_frame_s* lexical_frame, sr_s* result ){ const x_btcl_spect_s* p = x_btcl_spect_s_get_typed( t ); assert( p->m_btcl_external_parse ); return p->m_btcl_external_parse( o, source, lexical_frame, result );} \
  static inline bl_t x_btcl_t_defines_m_btcl_external_parse( tp_t t ){  return x_btcl_spect_s_get_typed( t )->m_btcl_external_parse != NULL;} \
  static inline f3_t x_btcl_a_nullary_f3( const x_btcl* o ){ const x_btcl_spect_s* p = x_btcl_spect_s_get_aware( o ); assert( p->nullary_f3 ); return p->nullary_f3( o );} \
  static inline bl_t x_btcl_defines_nullary_f3( const x_btcl* o ){  return x_btcl_spect_s_get_aware( o )->nullary_f3 != NULL;} \
  static inline f3_t x_btcl_t_nullary_f3( const x_btcl* o, tp_t t ){ const x_btcl_spect_s* p = x_btcl_spect_s_get_typed( t ); assert( p->nullary_f3 ); return p->nullary_f3( o );} \
  static inline bl_t x_btcl_t_defines_nullary_f3( tp_t t ){  return x_btcl_spect_s_get_typed( t )->nullary_f3 != NULL;} \
  static inline f3_t x_btcl_a_unary_f3( const x_btcl* o, f3_t x ){ const x_btcl_spect_s* p = x_btcl_spect_s_get_aware( o ); assert( p->unary_f3 ); return p->unary_f3( o, x );} \
  static inline bl_t x_btcl_defines_unary_f3( const x_btcl* o ){  return x_btcl_spect_s_get_aware( o )->unary_f3 != NULL;} \
  static inline f3_t x_btcl_t_unary_f3( const x_btcl* o, tp_t t, f3_t x ){ const x_btcl_spect_s* p = x_btcl_spect_s_get_typed( t ); assert( p->unary_f3 ); return p->unary_f3( o, x );} \
  static inline bl_t x_btcl_t_defines_unary_f3( tp_t t ){  return x_btcl_spect_s_get_typed( t )->unary_f3 != NULL;} \
  static inline f3_t x_btcl_a_binary_f3( const x_btcl* o, f3_t x, f3_t y ){ const x_btcl_spect_s* p = x_btcl_spect_s_get_aware( o ); assert( p->binary_f3 ); return p->binary_f3( o, x, y );} \
  static inline bl_t x_btcl_defines_binary_f3( const x_btcl* o ){  return x_btcl_spect_s_get_aware( o )->binary_f3 != NULL;} \
  static inline f3_t x_btcl_t_binary_f3( const x_btcl* o, tp_t t, f3_t x, f3_t y ){ const x_btcl_spect_s* p = x_btcl_spect_s_get_typed( t ); assert( p->binary_f3 ); return p->binary_f3( o, x, y );} \
  static inline bl_t x_btcl_t_defines_binary_f3( tp_t t ){  return x_btcl_spect_s_get_typed( t )->binary_f3 != NULL;} \
  static inline f3_t x_btcl_a_ternary_f3( const x_btcl* o, f3_t x, f3_t y, f3_t z ){ const x_btcl_spect_s* p = x_btcl_spect_s_get_aware( o ); assert( p->ternary_f3 ); return p->ternary_f3( o, x, y, z );} \
  static inline bl_t x_btcl_defines_ternary_f3( const x_btcl* o ){  return x_btcl_spect_s_get_aware( o )->ternary_f3 != NULL;} \
  static inline f3_t x_btcl_t_ternary_f3( const x_btcl* o, tp_t t, f3_t x, f3_t y, f3_t z ){ const x_btcl_spect_s* p = x_btcl_spect_s_get_typed( t ); assert( p->ternary_f3 ); return p->ternary_f3( o, x, y, z );} \
  static inline bl_t x_btcl_t_defines_ternary_f3( tp_t t ){  return x_btcl_spect_s_get_typed( t )->ternary_f3 != NULL;} \
  BETH_EXPAND_ITEM_x_btcl_context_s \
  BETH_EXPAND_ITEM_x_btcl_frame_s \
  BETH_EXPAND_ITEM_x_btcl_null_variable_s \
  BETH_EXPAND_ITEM_x_btcl_null_member_s \
  BETH_EXPAND_ITEM_x_btcl_null_arr_element_s \
  BETH_EXPAND_ITEM_x_btcl_list_s \
  BETH_EXPAND_ITEM_x_btcl_block_s \
  BETH_EXPAND_ITEM_x_btcl_signature_s \
  BETH_EXPAND_ITEM_x_btcl_external_function_s \
  BETH_EXPAND_ITEM_x_btcl_function_s \
  BETH_EXPAND_GROUP_x_btcl_operator \
  BETH_EXPAND_GROUP_x_btcl_operator_f3 \
  BETH_EXPAND_GROUP_x_btcl_net \
  BETH_EXPAND_GROUP_x_btcl_functor \
  BETH_EXPAND_GROUP_x_btcl_functor_f3 \
  BETH_EXPAND_GROUP_x_btcl_random \
  BETH_EXPAND_GROUP_x_btcl_test \
  static inline er_t x_btcl_t_from_source( x_btcl* o, tp_t t, x_source* source ){BLM_INIT_LEVEL(0);BLM_RETURNV(er_t, x_btcl_s_t_from_source(((x_btcl_s*)BLM_LEVEL_T_PUSH(0,x_btcl_s,x_btcl_s_create())),o, t, source ))} \
  static inline er_t x_btcl_from_source( x_btcl* o, x_source* source ){return  x_btcl_t_from_source(o,o->_, source );} \
  static inline er_t x_btcl_t_from_file( x_btcl* o, tp_t t, sc_t file ){BLM_INIT_LEVEL(0);BLM_RETURNV(er_t, x_btcl_t_from_source(o,t, ((x_source*)BLM_LEVEL_A_PUSH(0,x_source_check_create_from_file(file ))) ))} \
  static inline er_t x_btcl_from_file( x_btcl* o, sc_t file ){return  x_btcl_t_from_file(o,o->_, file );} \
  static inline er_t x_btcl_t_from_st( x_btcl* o, tp_t t, const st_s* st ){BLM_INIT_LEVEL(0);BLM_RETURNV(er_t, x_btcl_t_from_source(o,t, ((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_st(st ))) ))} \
  static inline er_t x_btcl_from_st( x_btcl* o, const st_s* st ){return  x_btcl_t_from_st(o,o->_, st );} \
  static inline er_t x_btcl_t_from_sc( x_btcl* o, tp_t t, sc_t sc ){BLM_INIT_LEVEL(0);BLM_RETURNV(er_t, x_btcl_t_from_source(o,t, ((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_sc(sc ))) ))} \
  static inline er_t x_btcl_from_sc( x_btcl* o, sc_t sc ){return  x_btcl_t_from_sc(o,o->_, sc );} \
  static inline x_btcl* x_btcl_create_from_source_t( x_source* source, tp_t* type ){BLM_INIT_LEVEL(0);BLM_RETURNV(x_btcl*, x_btcl_s_create_from_source_t(((x_btcl_s*)BLM_LEVEL_T_PUSH(0,x_btcl_s,x_btcl_s_create())),source, type ))} \
  static inline x_btcl* x_btcl_create_from_st_t( const st_s* st, tp_t* type ){BLM_INIT_LEVEL(0);BLM_RETURNV(x_btcl*, x_btcl_create_from_source_t(((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_st(st ))), type ))} \
  static inline x_btcl* x_btcl_create_from_sc_t( sc_t sc, tp_t* type ){BLM_INIT_LEVEL(0);BLM_RETURNV(x_btcl*, x_btcl_create_from_source_t(((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_sc(sc ))), type ))} \
  static inline x_btcl* x_btcl_create_from_file_t( sc_t file, tp_t* type ){BLM_INIT_LEVEL(0);BLM_RETURNV(x_btcl*, x_btcl_create_from_source_t(((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_file(file ))), type ))} \
  static inline x_btcl* x_btcl_create_from_source( x_source* source ){BLM_INIT_LEVEL(0);BLM_RETURNV(x_btcl*, x_btcl_s_create_from_source(((x_btcl_s*)BLM_LEVEL_T_PUSH(0,x_btcl_s,x_btcl_s_create())),source ))} \
  static inline x_btcl* x_btcl_create_from_st( const st_s* st ){BLM_INIT_LEVEL(0);BLM_RETURNV(x_btcl*, x_btcl_create_from_source(((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_st(st ))) ))} \
  static inline x_btcl* x_btcl_create_from_sc( sc_t sc ){BLM_INIT_LEVEL(0);BLM_RETURNV(x_btcl*, x_btcl_create_from_source(((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_sc(sc ))) ))} \
  static inline x_btcl* x_btcl_create_from_file( sc_t file ){BLM_INIT_LEVEL(0);BLM_RETURNV(x_btcl*, x_btcl_create_from_source(((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_file(file ))) ))} \
  static inline er_t x_btcl_run_from_source( x_source* source ){BLM_INIT_LEVEL(0);BLM_RETURNV(er_t, x_btcl_s_run_from_source(((x_btcl_s*)BLM_LEVEL_T_PUSH(0,x_btcl_s,x_btcl_s_create())),source ))} \
  static inline er_t x_btcl_run_from_st( const st_s* st ){BLM_INIT_LEVEL(0);BLM_RETURNV(er_t, x_btcl_run_from_source(((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_st(st ))) ))} \
  static inline er_t x_btcl_run_from_sc( sc_t sc ){BLM_INIT_LEVEL(0);BLM_RETURNV(er_t, x_btcl_run_from_source(((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_sc(sc ))) ))} \
  static inline er_t x_btcl_run_from_file( sc_t file ){BLM_INIT_LEVEL(0);BLM_RETURNV(er_t, x_btcl_run_from_source(((x_source*)BLM_LEVEL_A_PUSH(0,x_source_create_from_file(file ))) ))} \
  static inline er_t x_btcl_parse_create_object( x_source* source, sr_s* obj ){BLM_INIT_LEVEL(0);BLM_RETURNV(er_t, x_btcl_s_parse_create_object(((x_btcl_s*)BLM_LEVEL_T_PUSH(0,x_btcl_s,x_btcl_s_create())),source, obj ))} \
  static inline bl_t x_btcl_is_identifier( x_source* source ){return  x_source_parse_bl(source," #?(([0]>='A'&&[0]<='Z')||([0]>='a'&&[0]<='z')||[0]=='_')" );} \
  static inline sz_t x_btcl_max_frame_depth( void ){return  0x500;} \
  static inline sz_t x_btcl_max_frame_eval_depth( void ){return  0x1000;} \
  static inline s2_t x_btcl_priority_a( void ){return  50000;} \
  static inline s2_t x_btcl_priority_b( void ){return  40000;} \
  static inline s2_t x_btcl_priority_c( void ){return  30000;} \
  static inline s2_t x_btcl_priority_d( void ){return  20000;} \
  static inline s2_t x_btcl_priority_e( void ){return  10000;}

//----------------------------------------------------------------------------------------------------------------------
// group: x_btcl_operator; embeds: bcore_x_btcl_operator.x

#define TYPEOF_x_btcl_operator 0x3AF9B7BEC62CAADAull
#define TYPEOF_x_btcl_operator_spect_s 0xD29F4034B3518586ull
#define TYPEOF_x_btcl_operator_uop_s 0x9BBD7DAAD0C98C63ull
#define BETH_EXPAND_ITEM_x_btcl_operator_uop_s \
  BCORE_DECLARE_OBJECT( x_btcl_operator_uop_s ) \
  { \
      aware_t _; \
      tp_t type; \
      sr_s a; \
      x_source_point_s sp; \
  }; \
  static inline bl_t x_btcl_operator_uop_s_is_operator( const x_btcl_operator_uop_s* o ); \
  static inline bl_t x_btcl_operator_uop_s_is_exportable_operand( const x_btcl_operator_uop_s* o ); \
  x_btcl_operator_uop_s* x_btcl_operator_uop_s__( x_btcl_operator_uop_s* o, tp_t type, sr_s* a, x_source_point_s* source_point ); \
  x_btcl_operator_uop_s* x_btcl_operator_uop_s_signal( x_btcl_operator_uop_s* o, tp_t name, x_inst* arg ); \
  er_t x_btcl_operator_uop_s_solve_exportable_a( const x_btcl_operator_uop_s* o, const sr_s* a, sr_s* result, bl_t* success ); \
  er_t x_btcl_operator_uop_s_solve( x_btcl_operator_uop_s* o, x_btcl_frame_s* frame, sr_s* result, bl_t* success ); \
  er_t x_btcl_operator_uop_s_execute( const x_btcl_operator_uop_s* o, sr_s* result ); \
  static inline bl_t x_btcl_operator_uop_s_is_operator( const x_btcl_operator_uop_s* o ){return  true;} \
  static inline bl_t x_btcl_operator_uop_s_is_exportable_operand( const x_btcl_operator_uop_s* o ){return  true;}
#define TYPEOF_x_btcl_operator_bop_s 0x3736F25BC629E322ull
#define BETH_EXPAND_ITEM_x_btcl_operator_bop_s \
  BCORE_DECLARE_OBJECT( x_btcl_operator_bop_s ) \
  { \
      aware_t _; \
      tp_t type; \
      sr_s a; \
      sr_s b; \
      x_source_point_s sp; \
  }; \
  static inline bl_t x_btcl_operator_bop_s_is_operator( const x_btcl_operator_bop_s* o ); \
  static inline bl_t x_btcl_operator_bop_s_is_exportable_operand( const x_btcl_operator_bop_s* o ); \
  x_btcl_operator_bop_s* x_btcl_operator_bop_s__( x_btcl_operator_bop_s* o, tp_t type, sr_s* a, sr_s* b, x_source_point_s* source_point ); \
  x_btcl_operator_bop_s* x_btcl_operator_bop_s_signal( x_btcl_operator_bop_s* o, tp_t name, x_inst* arg ); \
  er_t x_btcl_operator_bop_s_solve_exportable_a_b( const x_btcl_operator_bop_s* o, const sr_s* a, const sr_s* b, sr_s* result, bl_t* success ); \
  er_t x_btcl_operator_bop_s_solve( x_btcl_operator_bop_s* o, x_btcl_frame_s* frame, sr_s* result, bl_t* success ); \
  er_t x_btcl_operator_bop_s_execute( const x_btcl_operator_bop_s* o, sr_s* result ); \
  static inline bl_t x_btcl_operator_bop_s_is_operator( const x_btcl_operator_bop_s* o ){return  true;} \
  static inline bl_t x_btcl_operator_bop_s_is_exportable_operand( const x_btcl_operator_bop_s* o ){return  true;}
#define TYPEOF_x_btcl_operator_top_s 0x25EA5FDB026468E0ull
#define BETH_EXPAND_ITEM_x_btcl_operator_top_s \
  BCORE_DECLARE_OBJECT( x_btcl_operator_top_s ) \
  { \
      aware_t _; \
      tp_t type; \
      sr_s a; \
      sr_s b; \
      sr_s c; \
      x_source_point_s sp; \
  }; \
  static inline bl_t x_btcl_operator_top_s_is_operator( const x_btcl_operator_top_s* o ); \
  static inline bl_t x_btcl_operator_top_s_is_exportable_operand( const x_btcl_operator_top_s* o ); \
  x_btcl_operator_top_s* x_btcl_operator_top_s__( x_btcl_operator_top_s* o, tp_t type, sr_s* a, sr_s* b, sr_s* c, x_source_point_s* source_point ); \
  x_btcl_operator_top_s* x_btcl_operator_top_s_signal( x_btcl_operator_top_s* o, tp_t name, x_inst* arg ); \
  er_t x_btcl_operator_top_s_solve_exportable_a_b_c( const x_btcl_operator_top_s* o, const sr_s* a, const sr_s* b, const sr_s* c, sr_s* result, bl_t* success ); \
  er_t x_btcl_operator_top_s_solve( x_btcl_operator_top_s* o, x_btcl_frame_s* frame, sr_s* result, bl_t* success ); \
  er_t x_btcl_operator_top_s_execute( const x_btcl_operator_top_s* o, sr_s* result ); \
  static inline bl_t x_btcl_operator_top_s_is_operator( const x_btcl_operator_top_s* o ){return  true;} \
  static inline bl_t x_btcl_operator_top_s_is_exportable_operand( const x_btcl_operator_top_s* o ){return  true;}
#define BETH_EXPAND_GROUP_x_btcl_operator \
  BCORE_FORWARD_OBJECT( x_btcl_operator ); \
  BCORE_FORWARD_OBJECT( x_btcl_operator_uop_s ); \
  BCORE_FORWARD_OBJECT( x_btcl_operator_bop_s ); \
  BCORE_FORWARD_OBJECT( x_btcl_operator_top_s ); \
  static inline bl_t x_btcl_operator_sr_is_operator( const sr_s* sr ); \
  static inline bl_t x_btcl_operator_sr_is_exportable_operand( const sr_s* sr ); \
  er_t x_btcl_operator_eval_uop_type( x_btcl_frame_s* frame, tp_t type, s2_t priority, x_source* source, bl_t postfix, sr_s* result ); \
  er_t x_btcl_operator_eval_bop_type( x_btcl_frame_s* frame, tp_t type, s2_t priority, x_source* source, sr_s* sa, sr_s* result ); \
  bl_t x_btcl_operator_is_comparable( const sr_s* a, const sr_s* b ); \
  er_t x_btcl_operator_eval_top_type( x_btcl_frame_s* frame, tp_t type, s2_t priority, x_source* source, sr_s* sa, sr_s* result ); \
  typedef bl_t (*x_btcl_operator_is_operator)(const x_btcl_operator* o ); \
  typedef bl_t (*x_btcl_operator_is_exportable_operand)(const x_btcl_operator* o ); \
  typedef er_t (*x_btcl_operator_solve)(x_btcl_operator* o, x_btcl_frame_s* frame, sr_s* result, bl_t* success ); \
  typedef x_btcl_operator* (*x_btcl_operator_signal)(x_btcl_operator* o, tp_t name, x_inst* arg ); \
  typedef er_t (*x_btcl_operator_execute)(const x_btcl_operator* o, sr_s* result ); \
  XOILA_DECLARE_SPECT( x_btcl_operator ) \
  { \
      bcore_spect_header_s header; \
      x_btcl_operator_is_operator is_operator; \
      x_btcl_operator_is_exportable_operand is_exportable_operand; \
      x_btcl_operator_solve solve; \
      x_btcl_operator_signal signal; \
      x_btcl_operator_execute execute; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( x_btcl_operator ) \
  static inline bl_t x_btcl_operator_a_is_operator( const x_btcl_operator* o ){ const x_btcl_operator_spect_s* p = x_btcl_operator_spect_s_get_aware( o ); assert( p->is_operator ); return p->is_operator( o );} \
  static inline bl_t x_btcl_operator_defines_is_operator( const x_btcl_operator* o ){ return  true;} \
  static inline bl_t x_btcl_operator_t_is_operator( const x_btcl_operator* o, tp_t t ){ const x_btcl_operator_spect_s* p = x_btcl_operator_spect_s_get_typed( t ); assert( p->is_operator ); return p->is_operator( o );} \
  static inline bl_t x_btcl_operator_t_defines_is_operator( tp_t t ){ return  true;} \
  static inline bl_t x_btcl_operator_is_operator_default( const x_btcl_operator* o ){return  false;} \
  static inline bl_t x_btcl_operator_a_is_exportable_operand( const x_btcl_operator* o ){ const x_btcl_operator_spect_s* p = x_btcl_operator_spect_s_get_aware( o ); assert( p->is_exportable_operand ); return p->is_exportable_operand( o );} \
  static inline bl_t x_btcl_operator_defines_is_exportable_operand( const x_btcl_operator* o ){ return  true;} \
  static inline bl_t x_btcl_operator_t_is_exportable_operand( const x_btcl_operator* o, tp_t t ){ const x_btcl_operator_spect_s* p = x_btcl_operator_spect_s_get_typed( t ); assert( p->is_exportable_operand ); return p->is_exportable_operand( o );} \
  static inline bl_t x_btcl_operator_t_defines_is_exportable_operand( tp_t t ){ return  true;} \
  static inline bl_t x_btcl_operator_is_exportable_operand_default( const x_btcl_operator* o ){return  false;} \
  static inline er_t x_btcl_operator_a_solve( x_btcl_operator* o, x_btcl_frame_s* frame, sr_s* result, bl_t* success ){ const x_btcl_operator_spect_s* p = x_btcl_operator_spect_s_get_aware( o ); assert( p->solve ); return p->solve( o, frame, result, success );} \
  static inline bl_t x_btcl_operator_defines_solve( const x_btcl_operator* o ){ return  true;} \
  er_t x_btcl_operator_solve_default( x_btcl_operator* o, x_btcl_frame_s* frame, sr_s* result, bl_t* success ); \
  static inline x_btcl_operator* x_btcl_operator_a_signal( x_btcl_operator* o, tp_t name, x_inst* arg ){ const x_btcl_operator_spect_s* p = x_btcl_operator_spect_s_get_aware( o ); assert( p->signal ); return p->signal( o, name, arg );} \
  static inline bl_t x_btcl_operator_defines_signal( const x_btcl_operator* o ){ return  true;} \
  x_btcl_operator* x_btcl_operator_signal_default( x_btcl_operator* o, tp_t name, x_inst* arg ); \
  static inline er_t x_btcl_operator_a_execute( const x_btcl_operator* o, sr_s* result ){ const x_btcl_operator_spect_s* p = x_btcl_operator_spect_s_get_aware( o ); assert( p->execute ); return p->execute( o, result );} \
  static inline bl_t x_btcl_operator_defines_execute( const x_btcl_operator* o ){ return  true;} \
  static inline er_t x_btcl_operator_t_execute( const x_btcl_operator* o, tp_t t, sr_s* result ){ const x_btcl_operator_spect_s* p = x_btcl_operator_spect_s_get_typed( t ); assert( p->execute ); return p->execute( o, result );} \
  static inline bl_t x_btcl_operator_t_defines_execute( tp_t t ){ return  true;} \
  er_t x_btcl_operator_execute_default( const x_btcl_operator* o, sr_s* result ); \
  BETH_EXPAND_ITEM_x_btcl_operator_uop_s \
  BETH_EXPAND_ITEM_x_btcl_operator_bop_s \
  BETH_EXPAND_ITEM_x_btcl_operator_top_s \
  static inline bl_t x_btcl_operator_sr_is_operator( const sr_s* sr ){return  sr ? sr->o ? x_btcl_operator_t_is_operator(((const x_btcl_operator*)( ((const x_btcl*)(sr->o)))),sr_s_type(sr) ) : false : false;} \
  static inline bl_t x_btcl_operator_sr_is_exportable_operand( const sr_s* sr ){return  x_btcl_operator_t_is_exportable_operand(((const x_btcl_operator*)( ((const x_btcl*)(sr->o)))),sr_s_type(sr) );}

//----------------------------------------------------------------------------------------------------------------------
// group: x_btcl_operator_f3; embeds: bcore_x_btcl_operator_f3.x
#include "bcore_x_btcl_operator_f3.h"

#define TYPEOF_x_btcl_operator_f3 0x1FBA9A06A1E203D8ull
#define TYPEOF_x_btcl_operator_f3_spect_s 0x37A58F457601BAA0ull
#define TYPEOF_x_btcl_operator_f3_arg_s 0xFC500A8767BE1761ull
#define BETH_EXPAND_ITEM_x_btcl_operator_f3_arg_s \
  BCORE_DECLARE_OBJECT( x_btcl_operator_f3_arg_s ) \
  { \
      aware_t _; \
      f3_t val; \
      tp_t name; \
  };
#define TYPEOF_x_btcl_operator_f3_arg_arr_s 0x9145F58AF512531Dull
#define BETH_EXPAND_ITEM_x_btcl_operator_f3_arg_arr_s \
  BCORE_DECLARE_OBJECT( x_btcl_operator_f3_arg_arr_s ) \
  { \
      aware_t _; \
      BCORE_ARRAY_DYN_SOLID_STATIC_S( x_btcl_operator_f3_arg_s, ); \
  };
#define TYPEOF_x_btcl_operator_f3_arg_nop_s 0x04F0240B228764B7ull
#define BETH_EXPAND_ITEM_x_btcl_operator_f3_arg_nop_s \
  BCORE_DECLARE_OBJECT( x_btcl_operator_f3_arg_nop_s ) \
  { \
      aware_t _; \
      sz_t index; \
      tp_t name; \
      f3_t* val; \
      x_source_point_s sp; \
  }; \
  er_t x_btcl_operator_f3_arg_nop_s_set_args( x_btcl_operator_f3_arg_nop_s* o, x_btcl_operator_f3_arg_arr_s* arg_arr ); \
  static inline f3_t x_btcl_operator_f3_arg_nop_s_get( const x_btcl_operator_f3_arg_nop_s* o ); \
  static inline er_t x_btcl_operator_f3_arg_nop_s_check_consistency( const x_btcl_operator_f3_arg_nop_s* o ); \
  static inline f3_t x_btcl_operator_f3_arg_nop_s_get( const x_btcl_operator_f3_arg_nop_s* o ){ assert( o->val ); return  (*(o->val));} \
  static inline er_t x_btcl_operator_f3_arg_nop_s_check_consistency( const x_btcl_operator_f3_arg_nop_s* o ){ BLM_TRY(EM_ASSERT(o->val )) return 0;}
#define TYPEOF_x_btcl_operator_f3_const_nop_s 0x50E723BC207F24DCull
#define BETH_EXPAND_ITEM_x_btcl_operator_f3_const_nop_s \
  BCORE_DECLARE_OBJECT( x_btcl_operator_f3_const_nop_s ) \
  { \
      aware_t _; \
      f3_t val; \
  }; \
  static inline f3_t x_btcl_operator_f3_const_nop_s_get( const x_btcl_operator_f3_const_nop_s* o ); \
  static inline er_t x_btcl_operator_f3_const_nop_s_check_consistency( const x_btcl_operator_f3_const_nop_s* o ); \
  x_btcl_operator_f3_const_nop_s* x_btcl_operator_f3_const_nop_s__( x_btcl_operator_f3_const_nop_s* o, f3_t val ); \
  static inline f3_t x_btcl_operator_f3_const_nop_s_get( const x_btcl_operator_f3_const_nop_s* o ){return  o->val;} \
  static inline er_t x_btcl_operator_f3_const_nop_s_check_consistency( const x_btcl_operator_f3_const_nop_s* o ){ return 0;}
#define TYPEOF_x_btcl_operator_f3_uop_s 0xF7C76D80D69C1EC1ull
#define BETH_EXPAND_ITEM_x_btcl_operator_f3_uop_s \
  BCORE_DECLARE_OBJECT( x_btcl_operator_f3_uop_s ) \
  { \
      aware_t _; \
      tp_t type; \
      x_btcl_operator_f3* a; \
      x_btcl_operator_f3_spect_s* sa; \
      x_btcl_operator_f3_fp_f3_ar1 fp_get; \
      x_source_point_s sp; \
  }; \
  er_t x_btcl_operator_f3_uop_s_setup( x_btcl_operator_f3_uop_s* o ); \
  er_t x_btcl_operator_f3_uop_s_set_args( x_btcl_operator_f3_uop_s* o, x_btcl_operator_f3_arg_arr_s* arg_arr ); \
  static inline f3_t x_btcl_operator_f3_uop_s_get( const x_btcl_operator_f3_uop_s* o ); \
  er_t x_btcl_operator_f3_uop_s_check_consistency( const x_btcl_operator_f3_uop_s* o ); \
  static inline f3_t x_btcl_operator_f3_uop_s_get( const x_btcl_operator_f3_uop_s* o ){ return   o->fp_get( o->sa->get( o->a ) ) ;}
#define TYPEOF_x_btcl_operator_f3_bop_s 0x333D2A8358C8F554ull
#define BETH_EXPAND_ITEM_x_btcl_operator_f3_bop_s \
  BCORE_DECLARE_OBJECT( x_btcl_operator_f3_bop_s ) \
  { \
      aware_t _; \
      tp_t type; \
      x_btcl_operator_f3* a; \
      x_btcl_operator_f3* b; \
      x_btcl_operator_f3_spect_s* sa; \
      x_btcl_operator_f3_spect_s* sb; \
      x_btcl_operator_f3_fp_f3_ar2 fp_get; \
      x_source_point_s sp; \
  }; \
  er_t x_btcl_operator_f3_bop_s_setup( x_btcl_operator_f3_bop_s* o ); \
  er_t x_btcl_operator_f3_bop_s_set_args( x_btcl_operator_f3_bop_s* o, x_btcl_operator_f3_arg_arr_s* arg_arr ); \
  static inline f3_t x_btcl_operator_f3_bop_s_get( const x_btcl_operator_f3_bop_s* o ); \
  er_t x_btcl_operator_f3_bop_s_check_consistency( const x_btcl_operator_f3_bop_s* o ); \
  static inline f3_t x_btcl_operator_f3_bop_s_get( const x_btcl_operator_f3_bop_s* o ){ return   o->fp_get( o->sa->get( o->a ), o->sb->get( o->b ) ) ;}
#define TYPEOF_x_btcl_operator_f3_top_s 0x46B1264E3350E9FEull
#define BETH_EXPAND_ITEM_x_btcl_operator_f3_top_s \
  BCORE_DECLARE_OBJECT( x_btcl_operator_f3_top_s ) \
  { \
      aware_t _; \
      tp_t type; \
      x_btcl_operator_f3* a; \
      x_btcl_operator_f3* b; \
      x_btcl_operator_f3* c; \
      x_btcl_operator_f3_spect_s* sa; \
      x_btcl_operator_f3_spect_s* sb; \
      x_btcl_operator_f3_spect_s* sc; \
      x_btcl_operator_f3_fp_f3_ar3 fp_get; \
      x_source_point_s sp; \
  }; \
  er_t x_btcl_operator_f3_top_s_setup( x_btcl_operator_f3_top_s* o ); \
  er_t x_btcl_operator_f3_top_s_set_args( x_btcl_operator_f3_top_s* o, x_btcl_operator_f3_arg_arr_s* arg_arr ); \
  static inline f3_t x_btcl_operator_f3_top_s_get( const x_btcl_operator_f3_top_s* o ); \
  er_t x_btcl_operator_f3_top_s_check_consistency( const x_btcl_operator_f3_top_s* o ); \
  static inline f3_t x_btcl_operator_f3_top_s_get( const x_btcl_operator_f3_top_s* o ){ return   o->fp_get( o->sa->get( o->a ), o->sb->get( o->b ), o->sc->get( o->c ) ) ;}
#define BETH_EXPAND_GROUP_x_btcl_operator_f3 \
  BCORE_FORWARD_OBJECT( x_btcl_operator_f3 ); \
  BCORE_FORWARD_OBJECT( x_btcl_operator_f3_arg_s ); \
  BCORE_FORWARD_OBJECT( x_btcl_operator_f3_arg_arr_s ); \
  BCORE_FORWARD_OBJECT( x_btcl_operator_f3_arg_nop_s ); \
  BCORE_FORWARD_OBJECT( x_btcl_operator_f3_const_nop_s ); \
  BCORE_FORWARD_OBJECT( x_btcl_operator_f3_uop_s ); \
  BCORE_FORWARD_OBJECT( x_btcl_operator_f3_bop_s ); \
  BCORE_FORWARD_OBJECT( x_btcl_operator_f3_top_s ); \
  void x_btcl_operator_f3_group_signal_init1( void ); \
  x_btcl_operator_f3* x_btcl_operator_f3_create_op( const sr_s* sr ); \
  typedef er_t (*x_btcl_operator_f3_setup)(x_btcl_operator_f3* o ); \
  typedef f3_t (*x_btcl_operator_f3_get)(const x_btcl_operator_f3* o ); \
  typedef er_t (*x_btcl_operator_f3_set_args)(x_btcl_operator_f3* o, x_btcl_operator_f3_arg_arr_s* arg_arr ); \
  typedef er_t (*x_btcl_operator_f3_check_consistency)(const x_btcl_operator_f3* o ); \
  XOILA_DECLARE_SPECT( x_btcl_operator_f3 ) \
  { \
      bcore_spect_header_s header; \
      x_btcl_operator_f3_setup setup; \
      x_btcl_operator_f3_get get; \
      x_btcl_operator_f3_set_args set_args; \
      x_btcl_operator_f3_check_consistency check_consistency; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( x_btcl_operator_f3 ) \
  BETH_EXPAND_ITEM_x_btcl_operator_f3_arg_s \
  BETH_EXPAND_ITEM_x_btcl_operator_f3_arg_arr_s \
  static inline er_t x_btcl_operator_f3_a_setup( x_btcl_operator_f3* o ){ const x_btcl_operator_f3_spect_s* p = x_btcl_operator_f3_spect_s_get_aware( o ); assert( p->setup ); return p->setup( o );} \
  static inline bl_t x_btcl_operator_f3_defines_setup( const x_btcl_operator_f3* o ){ return  true;} \
  static inline er_t x_btcl_operator_f3_setup_default( x_btcl_operator_f3* o ){return  0;} \
  static inline f3_t x_btcl_operator_f3_a_get( const x_btcl_operator_f3* o ){ const x_btcl_operator_f3_spect_s* p = x_btcl_operator_f3_spect_s_get_aware( o ); assert( p->get ); return p->get( o );} \
  static inline bl_t x_btcl_operator_f3_defines_get( const x_btcl_operator_f3* o ){ return  true;} \
  static inline f3_t x_btcl_operator_f3_get_default( const x_btcl_operator_f3* o ){return  0;} \
  static inline er_t x_btcl_operator_f3_a_set_args( x_btcl_operator_f3* o, x_btcl_operator_f3_arg_arr_s* arg_arr ){ const x_btcl_operator_f3_spect_s* p = x_btcl_operator_f3_spect_s_get_aware( o ); assert( p->set_args ); return p->set_args( o, arg_arr );} \
  static inline bl_t x_btcl_operator_f3_defines_set_args( const x_btcl_operator_f3* o ){ return  true;} \
  static inline er_t x_btcl_operator_f3_set_args_default( x_btcl_operator_f3* o, x_btcl_operator_f3_arg_arr_s* arg_arr ){return  0;} \
  static inline er_t x_btcl_operator_f3_a_check_consistency( const x_btcl_operator_f3* o ){ const x_btcl_operator_f3_spect_s* p = x_btcl_operator_f3_spect_s_get_aware( o ); assert( p->check_consistency ); return p->check_consistency( o );} \
  static inline bl_t x_btcl_operator_f3_defines_check_consistency( const x_btcl_operator_f3* o ){ return  true;} \
  static inline er_t x_btcl_operator_f3_check_consistency_default( const x_btcl_operator_f3* o ){return  0;} \
  BETH_EXPAND_ITEM_x_btcl_operator_f3_arg_nop_s \
  BETH_EXPAND_ITEM_x_btcl_operator_f3_const_nop_s \
  BETH_EXPAND_ITEM_x_btcl_operator_f3_uop_s \
  BETH_EXPAND_ITEM_x_btcl_operator_f3_bop_s \
  BETH_EXPAND_ITEM_x_btcl_operator_f3_top_s

//----------------------------------------------------------------------------------------------------------------------
// group: x_btcl_net; embeds: bcore_x_btcl_net.x

#define TYPEOF_x_btcl_net 0xDAC6EB4F342B58EDull
#define TYPEOF_x_btcl_net_spect_s 0x9864167133348429ull
#define TYPEOF_x_btcl_net_plain_branch_s 0xDC4BA7A83D758691ull
#define BETH_EXPAND_ITEM_x_btcl_net_plain_branch_s \
  BCORE_DECLARE_OBJECT( x_btcl_net_plain_branch_s ) \
  { \
      aware_t _; \
      tp_t name; \
      sr_s sr; \
      x_source_point_s* sp; \
  }; \
  x_btcl_net_plain_branch_s* x_btcl_net_plain_branch_s_setup( x_btcl_net_plain_branch_s* o, tp_t name, const x_source_point_s* sp, sr_s* sr );
#define TYPEOF_x_btcl_net_plain_branch_arr_s 0xECBD4E31966752ADull
#define BETH_EXPAND_ITEM_x_btcl_net_plain_branch_arr_s \
  BCORE_DECLARE_OBJECT( x_btcl_net_plain_branch_arr_s ) \
  { \
      aware_t _; \
      BCORE_ARRAY_DYN_SOLID_STATIC_S( x_btcl_net_plain_branch_s, ); \
  };
#define TYPEOF_x_btcl_net_socket_branch_s 0xEE7844BE9FDE5912ull
#define BETH_EXPAND_ITEM_x_btcl_net_socket_branch_s \
  BCORE_DECLARE_OBJECT( x_btcl_net_socket_branch_s ) \
  { \
      aware_t _; \
      tp_t name; \
      sr_s sr; \
      x_source_point_s* sp; \
  }; \
  x_btcl_net_socket_branch_s* x_btcl_net_socket_branch_s_setup( x_btcl_net_socket_branch_s* o, tp_t name, const x_source_point_s* sp, sr_s* sr );
#define TYPEOF_x_btcl_net_socket_branch_arr_s 0xB82B80B892572712ull
#define BETH_EXPAND_ITEM_x_btcl_net_socket_branch_arr_s \
  BCORE_DECLARE_OBJECT( x_btcl_net_socket_branch_arr_s ) \
  { \
      aware_t _; \
      BCORE_ARRAY_DYN_SOLID_STATIC_S( x_btcl_net_socket_branch_s, ); \
  };
#define TYPEOF_rack 0x6DE0001FD211EFD2ull
#define TYPEOF_wire 0xA67EEDF655B14178ull
#define TYPEOF_x_btcl_net_node_s 0xE6B74DF3DDB9D51Cull
#define BETH_EXPAND_ITEM_x_btcl_net_node_s \
  BCORE_DECLARE_OBJECT( x_btcl_net_node_s ) \
  { \
      aware_t _; \
      tp_t type; \
      tp_t name; \
      tp_t sub_name; \
      x_btcl_net_plain_branch_arr_s* plain_branch_arr; \
      x_btcl_net_socket_branch_arr_s* socket_branch_arr; \
      x_source_point_s* sp; \
  }; \
  static inline bl_t x_btcl_net_node_s_is_exportable_operand( const x_btcl_net_node_s* o ); \
  x_btcl_net_node_s* x_btcl_net_node_s_setup( x_btcl_net_node_s* o, tp_t type, tp_t name, const x_source_point_s* sp ); \
  x_btcl_net_node_s* x_btcl_net_node_s_setup_wire( x_btcl_net_node_s* o, tp_t rack_name, tp_t wire_name, const x_source_point_s* sp ); \
  bl_t x_btcl_net_node_s_plain_branch_exists( x_btcl_net_node_s* o, tp_t branch_name ); \
  bl_t x_btcl_net_node_s_socket_branch_exists( x_btcl_net_node_s* o, tp_t branch_name ); \
  static inline sz_t x_btcl_net_node_s_plain_branches( const x_btcl_net_node_s* o ); \
  static inline x_btcl_net_plain_branch_s* x_btcl_net_node_s_m_plain_branch_by_name( x_btcl_net_node_s* o, tp_t name ); \
  static inline const x_btcl_net_plain_branch_s* x_btcl_net_node_s_c_plain_branch_by_name( const x_btcl_net_node_s* o, tp_t name ); \
  static inline x_btcl_net_plain_branch_s* x_btcl_net_node_s_m_plain_branch_by_index( x_btcl_net_node_s* o, sz_t index ); \
  static inline const x_btcl_net_plain_branch_s* x_btcl_net_node_s_c_plain_branch_by_index( const x_btcl_net_node_s* o, sz_t index ); \
  static inline sz_t x_btcl_net_node_s_socket_branches( const x_btcl_net_node_s* o ); \
  static inline x_btcl_net_socket_branch_s* x_btcl_net_node_s_m_socket_branch_by_name( x_btcl_net_node_s* o, tp_t name ); \
  static inline const x_btcl_net_socket_branch_s* x_btcl_net_node_s_c_socket_branch_by_name( const x_btcl_net_node_s* o, tp_t name ); \
  static inline x_btcl_net_socket_branch_s* x_btcl_net_node_s_m_socket_branch_by_index( x_btcl_net_node_s* o, sz_t index ); \
  static inline const x_btcl_net_socket_branch_s* x_btcl_net_node_s_c_socket_branch_by_index( const x_btcl_net_node_s* o, sz_t index ); \
  x_btcl_net_node_s* x_btcl_net_node_s_push_plain_branch( x_btcl_net_node_s* o, tp_t name, bl_t replace, const x_source_point_s* sp, sr_s* sr ); \
  x_btcl_net_node_s* x_btcl_net_node_s_push_socket_branch( x_btcl_net_node_s* o, tp_t name, bl_t replace, const x_source_point_s* sp, sr_s* sr ); \
  static inline bl_t x_btcl_net_node_s_is_exportable_operand( const x_btcl_net_node_s* o ){return  true;} \
  static inline sz_t x_btcl_net_node_s_plain_branches( const x_btcl_net_node_s* o ){return  o->plain_branch_arr ? o->plain_branch_arr->size : 0;} \
  static inline x_btcl_net_plain_branch_s* x_btcl_net_node_s_m_plain_branch_by_name( x_btcl_net_node_s* o, tp_t name ){ {const x_btcl_net_plain_branch_arr_s* __a=o->plain_branch_arr ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){x_btcl_net_plain_branch_s* e=&(__a->data[__i]); if( e->name == name ) return  e; }}return  NULL;} \
  static inline const x_btcl_net_plain_branch_s* x_btcl_net_node_s_c_plain_branch_by_name( const x_btcl_net_node_s* o, tp_t name ){ {const x_btcl_net_plain_branch_arr_s* __a=o->plain_branch_arr ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){const x_btcl_net_plain_branch_s* e=&(__a->data[__i]); if( e->name == name ) return  e; }}return  NULL;} \
  static inline x_btcl_net_plain_branch_s* x_btcl_net_node_s_m_plain_branch_by_index( x_btcl_net_node_s* o, sz_t index ){return &( o->plain_branch_arr->data[ index ]);} \
  static inline const x_btcl_net_plain_branch_s* x_btcl_net_node_s_c_plain_branch_by_index( const x_btcl_net_node_s* o, sz_t index ){return &( o->plain_branch_arr->data[ index ]);} \
  static inline sz_t x_btcl_net_node_s_socket_branches( const x_btcl_net_node_s* o ){return  o->socket_branch_arr ? o->socket_branch_arr->size : 0;} \
  static inline x_btcl_net_socket_branch_s* x_btcl_net_node_s_m_socket_branch_by_name( x_btcl_net_node_s* o, tp_t name ){ {const x_btcl_net_socket_branch_arr_s* __a=o->socket_branch_arr ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){x_btcl_net_socket_branch_s* e=&(__a->data[__i]); if( e->name == name ) return  e; }}return  NULL;} \
  static inline const x_btcl_net_socket_branch_s* x_btcl_net_node_s_c_socket_branch_by_name( const x_btcl_net_node_s* o, tp_t name ){ {const x_btcl_net_socket_branch_arr_s* __a=o->socket_branch_arr ;if(__a)for(sz_t __i=0;__i<__a->size;__i++){const x_btcl_net_socket_branch_s* e=&(__a->data[__i]); if( e->name == name ) return  e; }}return  NULL;} \
  static inline x_btcl_net_socket_branch_s* x_btcl_net_node_s_m_socket_branch_by_index( x_btcl_net_node_s* o, sz_t index ){return &( o->socket_branch_arr->data[ index ]);} \
  static inline const x_btcl_net_socket_branch_s* x_btcl_net_node_s_c_socket_branch_by_index( const x_btcl_net_node_s* o, sz_t index ){return &( o->socket_branch_arr->data[ index ]);}
#define BETH_EXPAND_GROUP_x_btcl_net \
  BCORE_FORWARD_OBJECT( x_btcl_net ); \
  BCORE_FORWARD_OBJECT( x_btcl_net_plain_branch_s ); \
  BCORE_FORWARD_OBJECT( x_btcl_net_plain_branch_arr_s ); \
  BCORE_FORWARD_OBJECT( x_btcl_net_socket_branch_s ); \
  BCORE_FORWARD_OBJECT( x_btcl_net_socket_branch_arr_s ); \
  BCORE_FORWARD_OBJECT( x_btcl_net_node_s ); \
  er_t x_btcl_net_eval_node_modifier( x_btcl_frame_s* frame, x_source* source, sr_s* node_sr ); \
  er_t x_btcl_net_eval_node_member( x_btcl_frame_s* frame, x_source* source, sr_s* sr ); \
  XOILA_DECLARE_SPECT( x_btcl_net ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( x_btcl_net ) \
  BETH_EXPAND_ITEM_x_btcl_net_plain_branch_s \
  BETH_EXPAND_ITEM_x_btcl_net_plain_branch_arr_s \
  BETH_EXPAND_ITEM_x_btcl_net_socket_branch_s \
  BETH_EXPAND_ITEM_x_btcl_net_socket_branch_arr_s \
  BETH_EXPAND_ITEM_x_btcl_net_node_s

//----------------------------------------------------------------------------------------------------------------------
// group: x_btcl_functor; embeds: bcore_x_btcl_functor.x

#define TYPEOF_x_btcl_functor 0xC6D69F549424CC45ull
#define TYPEOF_x_btcl_functor_spect_s 0x74434E1817926761ull
#define TYPEOF_x_btcl_functor_arg_val_s 0x01FC9508F08D5B8Cull
#define BETH_EXPAND_ITEM_x_btcl_functor_arg_val_s \
  BCORE_DECLARE_OBJECT( x_btcl_functor_arg_val_s ) \
  { \
      aware_t _; \
      sr_s sr; \
  }; \
  static inline x_btcl_functor_arg_val_s* x_btcl_functor_arg_val_s_set_f3( x_btcl_functor_arg_val_s* o, f3_t v ); \
  f3_t* x_btcl_functor_arg_val_s_get_f3_ptr( x_btcl_functor_arg_val_s* o, f3_t v_init ); \
  static inline x_btcl_functor_arg_val_s* x_btcl_functor_arg_val_s_set_f3( x_btcl_functor_arg_val_s* o, f3_t v ){sr_s_from_f3(&(o->sr),v );return o;}
#define TYPEOF_signal_arg_uop_update_val 0xB58769FCD4E38A8Bull
#define TYPEOF_x_btcl_functor_arg_uop_s 0x27C97E6C217D8867ull
#define BETH_EXPAND_ITEM_x_btcl_functor_arg_uop_s \
  BCORE_DECLARE_OBJECT( x_btcl_functor_arg_uop_s ) \
  { \
      aware_t _; \
      sz_t index; \
      x_btcl_functor_arg_val_s* val; \
  }; \
  static inline bl_t x_btcl_functor_arg_uop_s_is_operator( const x_btcl_functor_arg_uop_s* o ); \
  static inline bl_t x_btcl_functor_arg_uop_s_is_exportable_operand( const x_btcl_functor_arg_uop_s* o ); \
  er_t x_btcl_functor_arg_uop_s_solve( x_btcl_functor_arg_uop_s* o, x_btcl_frame_s* frame, sr_s* result, bl_t* success ); \
  er_t x_btcl_functor_arg_uop_s_execute( const x_btcl_functor_arg_uop_s* o, sr_s* result ); \
  x_btcl_functor_arg_uop_s* x_btcl_functor_arg_uop_s_signal( x_btcl_functor_arg_uop_s* o, tp_t name, x_inst* arg ); \
  static inline bl_t x_btcl_functor_arg_uop_s_is_operator( const x_btcl_functor_arg_uop_s* o ){return  true;} \
  static inline bl_t x_btcl_functor_arg_uop_s_is_exportable_operand( const x_btcl_functor_arg_uop_s* o ){return  true;}
#define TYPEOF_x_btcl_functor_arg_s 0xAB7F60266D231F10ull
#define BETH_EXPAND_ITEM_x_btcl_functor_arg_s \
  BCORE_DECLARE_OBJECT( x_btcl_functor_arg_s ) \
  { \
      aware_t _; \
      tp_t name; \
      x_btcl_functor_arg_val_s* val; \
      x_btcl_functor_arg_uop_s* uop; \
  }; \
  static inline x_btcl_functor_arg_s* x_btcl_functor_arg_s_set_val( x_btcl_functor_arg_s* o, sr_s* sr ); \
  static inline x_btcl_functor_arg_s* x_btcl_functor_arg_s_set_f3( x_btcl_functor_arg_s* o, f3_t v ); \
  static inline f3_t* x_btcl_functor_arg_s_get_f3_ptr( const x_btcl_functor_arg_s* o, f3_t v_init ); \
  static inline x_btcl_functor_arg_s* x_btcl_functor_arg_s_clear_val( x_btcl_functor_arg_s* o ); \
  x_btcl_functor_arg_uop_s* x_btcl_functor_arg_s_get_uop( x_btcl_functor_arg_s* o ); \
  static inline x_btcl_functor_arg_s* x_btcl_functor_arg_s_set_val( x_btcl_functor_arg_s* o, sr_s* sr ){sr_s_fork_from(&(BCORE_PASS_CREATE(x_btcl_functor_arg_val_s,o->val)->sr),sr );return o;} \
  static inline x_btcl_functor_arg_s* x_btcl_functor_arg_s_set_f3( x_btcl_functor_arg_s* o, f3_t v ){x_btcl_functor_arg_val_s_set_f3(BCORE_PASS_CREATE(x_btcl_functor_arg_val_s,o->val),v );return o;} \
  static inline f3_t* x_btcl_functor_arg_s_get_f3_ptr( const x_btcl_functor_arg_s* o, f3_t v_init ){return  x_btcl_functor_arg_val_s_get_f3_ptr(BCORE_PASS_CREATE(x_btcl_functor_arg_val_s,o->val),v_init );} \
  static inline x_btcl_functor_arg_s* x_btcl_functor_arg_s_clear_val( x_btcl_functor_arg_s* o ){sr_s_clear(&(BCORE_PASS_CREATE(x_btcl_functor_arg_val_s,o->val)->sr));return o;}
#define TYPEOF_x_btcl_functor_arg_arr_s 0x4751FB349F0DEE74ull
#define BETH_EXPAND_ITEM_x_btcl_functor_arg_arr_s \
  BCORE_DECLARE_OBJECT( x_btcl_functor_arg_arr_s ) \
  { \
      aware_t _; \
      BCORE_ARRAY_DYN_SOLID_STATIC_S( x_btcl_functor_arg_s, ); \
  }; \
  x_btcl_functor_arg_arr_s* x_btcl_functor_arg_arr_s_set_size( x_btcl_functor_arg_arr_s* o, sz_t size );
#define TYPEOF_call 0xB5167A90E83BA4A9ull
#define TYPEOF_x_btcl_functor_s 0x73F13731F3140E07ull
#define BETH_EXPAND_ITEM_x_btcl_functor_s \
  BCORE_DECLARE_OBJECT( x_btcl_functor_s ) \
  { \
      aware_t _; \
      x_btcl_functor_arg_arr_s arg_arr; \
      sr_s* op_tree; \
      x_source_point_s sp; \
  }; \
  static inline bl_t x_btcl_functor_s_is_operator( const x_btcl_functor_s* o ); \
  static inline x_btcl_operator* x_btcl_functor_s_operator( x_btcl_functor_s* o ); \
  x_btcl_functor_s* x_btcl_functor_s_signal( x_btcl_functor_s* o, tp_t name, x_inst* arg ); \
  void x_btcl_functor_s_copy_x( x_btcl_functor_s* o, const x_btcl_functor_s* src ); \
  void x_btcl_functor_s_mutated( x_btcl_functor_s* o ); \
  er_t x_btcl_functor_s_setup( x_btcl_functor_s* o, const x_source_point_s* sp, x_btcl_function_s* function, x_btcl_frame_s* lexical_frame ); \
  static inline sz_t x_btcl_functor_s_args( const x_btcl_functor_s* o ); \
  tp_t x_btcl_functor_s_arg_name( const x_btcl_functor_s* o, sz_t index ); \
  sz_t x_btcl_functor_s_arg_index( const x_btcl_functor_s* o, tp_t name ); \
  static inline bl_t x_btcl_functor_s_arg_name_exists( const x_btcl_functor_s* o, tp_t name ); \
  const x_btcl_functor_s* x_btcl_functor_s_set_arg_sr( const x_btcl_functor_s* o, sz_t index, sr_s* sr ); \
  const x_btcl_functor_s* x_btcl_functor_s_set_arg_sr_by_name( const x_btcl_functor_s* o, tp_t name, sr_s* sr ); \
  static inline const x_btcl_functor_s* x_btcl_functor_s_set_arg_f3( const x_btcl_functor_s* o, sz_t index, f3_t v ); \
  f3_t* x_btcl_functor_s_get_arg_f3_ptr( const x_btcl_functor_s* o, sz_t index, f3_t v_init ); \
  const x_btcl_functor_s* x_btcl_functor_s_set_arg_f3_by_name( const x_btcl_functor_s* o, tp_t name, f3_t v ); \
  f3_t* x_btcl_functor_s_get_arg_f3_ptr_by_name( const x_btcl_functor_s* o, tp_t name, f3_t v_init ); \
  er_t x_btcl_functor_s_call( const x_btcl_functor_s* o, sr_s* result ); \
  f3_t x_btcl_functor_s_call_to_f3( const x_btcl_functor_s* o ); \
  sz_t x_btcl_functor_s_btcl_function_arity( const x_btcl_functor_s* o, tp_t name ); \
  er_t x_btcl_functor_s_btcl_function( const x_btcl_functor_s* o, tp_t name, const x_source_point_s* sp, x_btcl_frame_s* lexical_frame, const bcore_arr_sr_s* args, sr_s* result ); \
  f3_t x_btcl_functor_s_nullary_f3( const x_btcl_functor_s* o ); \
  f3_t x_btcl_functor_s_unary_f3( const x_btcl_functor_s* o, f3_t x ); \
  f3_t x_btcl_functor_s_binary_f3( const x_btcl_functor_s* o, f3_t x, f3_t y ); \
  f3_t x_btcl_functor_s_ternary_f3( const x_btcl_functor_s* o, f3_t x, f3_t y, f3_t z ); \
  static inline bl_t x_btcl_functor_s_is_operator( const x_btcl_functor_s* o ){return  ( o->op_tree && x_btcl_operator_sr_is_operator(o->op_tree ) );} \
  static inline x_btcl_operator* x_btcl_functor_s_operator( x_btcl_functor_s* o ){return  ((x_btcl_operator*)(o->op_tree->o));} \
  static inline sz_t x_btcl_functor_s_args( const x_btcl_functor_s* o ){return  o->arg_arr.size;} \
  static inline bl_t x_btcl_functor_s_arg_name_exists( const x_btcl_functor_s* o, tp_t name ){return  x_btcl_functor_s_arg_index(o,name ) >= 0;} \
  static inline const x_btcl_functor_s* x_btcl_functor_s_set_arg_f3( const x_btcl_functor_s* o, sz_t index, f3_t v ){BLM_INIT_LEVEL(0);x_btcl_functor_s_set_arg_sr(o,index, sr_s_from_f3(((sr_s*)BLM_LEVEL_T_PUSH(0,sr_s,sr_s_create())),v ) );BLM_RETURNV(const x_btcl_functor_s*,o)}
#define BETH_EXPAND_GROUP_x_btcl_functor \
  BCORE_FORWARD_OBJECT( x_btcl_functor ); \
  BCORE_FORWARD_OBJECT( x_btcl_functor_arg_val_s ); \
  BCORE_FORWARD_OBJECT( x_btcl_functor_arg_uop_s ); \
  BCORE_FORWARD_OBJECT( x_btcl_functor_arg_s ); \
  BCORE_FORWARD_OBJECT( x_btcl_functor_arg_arr_s ); \
  BCORE_FORWARD_OBJECT( x_btcl_functor_s ); \
  er_t x_btcl_functor_eval_modifier( x_btcl_frame_s* frame, x_source* source, sr_s* functor_sr ); \
  XOILA_DECLARE_SPECT( x_btcl_functor ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( x_btcl_functor ) \
  BETH_EXPAND_ITEM_x_btcl_functor_arg_val_s \
  BETH_EXPAND_ITEM_x_btcl_functor_arg_uop_s \
  BETH_EXPAND_ITEM_x_btcl_functor_arg_s \
  BETH_EXPAND_ITEM_x_btcl_functor_arg_arr_s \
  BETH_EXPAND_ITEM_x_btcl_functor_s

//----------------------------------------------------------------------------------------------------------------------
// group: x_btcl_functor_f3; embeds: bcore_x_btcl_functor_f3.x

#define TYPEOF_x_btcl_functor_f3 0x16A785E00AD753D1ull
#define TYPEOF_x_btcl_functor_f3_spect_s 0xB592EF378E77880Dull
#define TYPEOF_call 0xB5167A90E83BA4A9ull
#define TYPEOF_x_btcl_functor_f3_s 0xFE3EB32D52AF2EDBull
#define BETH_EXPAND_ITEM_x_btcl_functor_f3_s \
  BCORE_DECLARE_OBJECT( x_btcl_functor_f3_s ) \
  { \
      aware_t _; \
      x_btcl_operator_f3_arg_arr_s arg_arr; \
      x_btcl_operator_f3* op_tree; \
      x_source_point_s sp; \
  }; \
  void x_btcl_functor_f3_s_copy_x( x_btcl_functor_f3_s* o, const x_btcl_functor_f3_s* src ); \
  er_t x_btcl_functor_f3_s_copy_typed( x_btcl_functor_f3_s* o, tp_t type, vc_t src ); \
  void x_btcl_functor_f3_s_mutated( x_btcl_functor_f3_s* o ); \
  er_t x_btcl_functor_f3_s_setup_tree( x_btcl_functor_f3_s* o ); \
  er_t x_btcl_functor_f3_s_from_functor( x_btcl_functor_f3_s* o, const x_btcl_functor_s* functor ); \
  er_t x_btcl_functor_f3_s_setup( x_btcl_functor_f3_s* o, const x_source_point_s* sp, x_btcl_function_s* function, x_btcl_frame_s* lexical_frame ); \
  static inline sz_t x_btcl_functor_f3_s_args( const x_btcl_functor_f3_s* o ); \
  tp_t x_btcl_functor_f3_s_arg_name( const x_btcl_functor_f3_s* o, sz_t index ); \
  sz_t x_btcl_functor_f3_s_arg_index( const x_btcl_functor_f3_s* o, tp_t name ); \
  static inline bl_t x_btcl_functor_f3_s_arg_name_exists( const x_btcl_functor_f3_s* o, tp_t name ); \
  const x_btcl_functor_f3_s* x_btcl_functor_f3_s_set_arg_f3( const x_btcl_functor_f3_s* o, sz_t index, f3_t v ); \
  f3_t* x_btcl_functor_f3_s_get_arg_f3_ptr( const x_btcl_functor_f3_s* o, sz_t index ); \
  const x_btcl_functor_f3_s* x_btcl_functor_f3_s_set_arg_f3_by_name( const x_btcl_functor_f3_s* o, tp_t name, f3_t v ); \
  f3_t* x_btcl_functor_f3_s_get_arg_f3_ptr_by_name( const x_btcl_functor_f3_s* o, tp_t name ); \
  static inline f3_t x_btcl_functor_f3_s_call( const x_btcl_functor_f3_s* o ); \
  static inline f3_t x_btcl_functor_f3_s_call_to_f3( const x_btcl_functor_f3_s* o ); \
  sz_t x_btcl_functor_f3_s_btcl_function_arity( const x_btcl_functor_f3_s* o, tp_t name ); \
  er_t x_btcl_functor_f3_s_btcl_function( const x_btcl_functor_f3_s* o, tp_t name, const x_source_point_s* sp, x_btcl_frame_s* lexical_frame, const bcore_arr_sr_s* args, sr_s* result ); \
  f3_t x_btcl_functor_f3_s_nullary_f3( const x_btcl_functor_f3_s* o ); \
  f3_t x_btcl_functor_f3_s_unary_f3( const x_btcl_functor_f3_s* o, f3_t x ); \
  f3_t x_btcl_functor_f3_s_binary_f3( const x_btcl_functor_f3_s* o, f3_t x, f3_t y ); \
  f3_t x_btcl_functor_f3_s_ternary_f3( const x_btcl_functor_f3_s* o, f3_t x, f3_t y, f3_t z ); \
  static inline sz_t x_btcl_functor_f3_s_args( const x_btcl_functor_f3_s* o ){return  o->arg_arr.size;} \
  static inline bl_t x_btcl_functor_f3_s_arg_name_exists( const x_btcl_functor_f3_s* o, tp_t name ){return  x_btcl_functor_f3_s_arg_index(o,name ) >= 0;} \
  static inline f3_t x_btcl_functor_f3_s_call( const x_btcl_functor_f3_s* o ){ assert( o->op_tree ); return  x_btcl_operator_f3_a_get(o->op_tree);} \
  static inline f3_t x_btcl_functor_f3_s_call_to_f3( const x_btcl_functor_f3_s* o ){return  x_btcl_functor_f3_s_call(o);}
#define BETH_EXPAND_GROUP_x_btcl_functor_f3 \
  BCORE_FORWARD_OBJECT( x_btcl_functor_f3 ); \
  BCORE_FORWARD_OBJECT( x_btcl_functor_f3_s ); \
  XOILA_DECLARE_SPECT( x_btcl_functor_f3 ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( x_btcl_functor_f3 ) \
  BETH_EXPAND_ITEM_x_btcl_functor_f3_s

//----------------------------------------------------------------------------------------------------------------------
// group: x_btcl_random; embeds: bcore_x_btcl_random.x

#define TYPEOF_x_btcl_random 0x23F809F82198F285ull
#define TYPEOF_x_btcl_random_spect_s 0xAFD11A1396EA0DA1ull
#define TYPEOF_list 0xBF779AAD69748141ull
#define TYPEOF_x_btcl_random_s 0x90D16CB9E689A947ull
#define BETH_EXPAND_ITEM_x_btcl_random_s \
  BCORE_DECLARE_OBJECT( x_btcl_random_s ) \
  { \
      aware_t _; \
      bcore_prsg* prsg; \
      u3_t seed; \
      f3_t min; \
      f3_t max; \
  }; \
  const x_btcl_random_s* x_btcl_random_s_fill_list( const x_btcl_random_s* o, sz_t size, x_btcl_list_s* list ); \
  sz_t x_btcl_random_s_btcl_function_arity( const x_btcl_random_s* o, tp_t name ); \
  er_t x_btcl_random_s_btcl_function( const x_btcl_random_s* o, tp_t name, const x_source_point_s* sp, x_btcl_frame_s* lexical_frame, const bcore_arr_sr_s* args, sr_s* result );
#define BETH_EXPAND_GROUP_x_btcl_random \
  BCORE_FORWARD_OBJECT( x_btcl_random ); \
  BCORE_FORWARD_OBJECT( x_btcl_random_s ); \
  XOILA_DECLARE_SPECT( x_btcl_random ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( x_btcl_random ) \
  BETH_EXPAND_ITEM_x_btcl_random_s

//----------------------------------------------------------------------------------------------------------------------
// group: x_btcl_test; embeds: bcore_x_btcl_test.x

#define TYPEOF_x_btcl_test 0x45B3376D5455048Aull
#define TYPEOF_x_btcl_test_spect_s 0x108C2602C9B1EEF6ull
#define TYPEOF_add_a_b 0xD859D6BBC345568Dull
#define TYPEOF_add_a 0x22DCF9FDFCE6CD70ull
#define TYPEOF_x_btcl_test_adder_s 0x52CEEACF749AB0EFull
#define BETH_EXPAND_ITEM_x_btcl_test_adder_s \
  BCORE_DECLARE_OBJECT( x_btcl_test_adder_s ) \
  { \
      aware_t _; \
      f3_t additive; \
  }; \
  sz_t x_btcl_test_adder_s_btcl_function_arity( const x_btcl_test_adder_s* o, tp_t name ); \
  er_t x_btcl_test_adder_s_btcl_function( const x_btcl_test_adder_s* o, tp_t name, const x_source_point_s* sp, x_btcl_frame_s* lexical_frame, const bcore_arr_sr_s* args, sr_s* result );
#define TYPEOF_run 0x89B2A81960C55A4Aull
#define TYPEOF_x_btcl_test_minimal_s 0x8E11E5E44C304334ull
#define BETH_EXPAND_ITEM_x_btcl_test_minimal_s \
  BCORE_DECLARE_OBJECT( x_btcl_test_minimal_s ) \
  { \
      aware_t _; \
  }; \
  static inline er_t x_btcl_test_minimal_s_run( const x_btcl_test_minimal_s* o ); \
  static inline sz_t x_btcl_test_minimal_s_btcl_function_arity( const x_btcl_test_minimal_s* o, tp_t name ); \
  static inline er_t x_btcl_test_minimal_s_btcl_function( const x_btcl_test_minimal_s* o, tp_t name, const x_source_point_s* sp, x_btcl_frame_s* lexical_frame, const bcore_arr_sr_s* args, sr_s* result ); \
  static inline er_t x_btcl_test_minimal_s_run( const x_btcl_test_minimal_s* o ){ bcore_msg_fa( "BTCL (#name): 'run' called\n", o->_ ); return  0;} \
  static inline sz_t x_btcl_test_minimal_s_btcl_function_arity( const x_btcl_test_minimal_s* o, tp_t name ){return  ( name == ((tp_t)(TYPEOF_run)) ) ? 0 : -1;} \
  static inline er_t x_btcl_test_minimal_s_btcl_function( const x_btcl_test_minimal_s* o, tp_t name, const x_source_point_s* sp, x_btcl_frame_s* lexical_frame, const bcore_arr_sr_s* args, sr_s* result ){return  ( name == ((tp_t)(TYPEOF_run)) ) ? x_btcl_test_minimal_s_run(o) : 0;}
#define TYPEOF_x_btcl_test_parser_s 0xC1EEDC4594DDCCFAull
#define BETH_EXPAND_ITEM_x_btcl_test_parser_s \
  BCORE_DECLARE_OBJECT( x_btcl_test_parser_s ) \
  { \
      aware_t _; \
  }; \
  er_t x_btcl_test_parser_s_btcl_external_parse( const x_btcl_test_parser_s* o, x_source* source, x_btcl_frame_s* lexical_frame, sr_s* result );
#define BETH_EXPAND_GROUP_x_btcl_test \
  BCORE_FORWARD_OBJECT( x_btcl_test ); \
  BCORE_FORWARD_OBJECT( x_btcl_test_adder_s ); \
  BCORE_FORWARD_OBJECT( x_btcl_test_minimal_s ); \
  BCORE_FORWARD_OBJECT( x_btcl_test_parser_s ); \
  XOILA_DECLARE_SPECT( x_btcl_test ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( x_btcl_test ) \
  BETH_EXPAND_ITEM_x_btcl_test_adder_s \
  BETH_EXPAND_ITEM_x_btcl_test_minimal_s \
  BETH_EXPAND_ITEM_x_btcl_test_parser_s

/**********************************************************************************************************************/
// source: bcore_x_hmap.h

//----------------------------------------------------------------------------------------------------------------------
// group: x_hmap; embeds: bcore_x_hmap.x

#define TYPEOF_x_hmap 0x9FA6546E5B82A976ull
#define TYPEOF_x_hmap_spect_s 0xF4B61F3CCFBA3082ull
#define BETH_EXPAND_GROUP_x_hmap \
  BCORE_FORWARD_OBJECT( x_hmap ); \
  BCORE_FORWARD_OBJECT( x_hmap_tp ); \
  BCORE_FORWARD_OBJECT( x_hmap_tp_test ); \
  XOILA_DECLARE_SPECT( x_hmap ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( x_hmap ) \
  BETH_EXPAND_GROUP_x_hmap_tp \
  BETH_EXPAND_GROUP_x_hmap_tp_test

//----------------------------------------------------------------------------------------------------------------------
// group: x_hmap_tp

#define TYPEOF_x_hmap_tp 0x50EA9D0E0CB248C7ull
#define TYPEOF_x_hmap_tp_spect_s 0x3683902FE0AC12B7ull
#define TYPEOF_x_hmap_tp_s 0x1BA738E6B01C5241ull
#define BETH_EXPAND_ITEM_x_hmap_tp_s \
  BCORE_DECLARE_OBJECT( x_hmap_tp_s ) \
  { \
      aware_t _; \
      bcore_hmap_tpaw_s map; \
  }; \
  static inline const x_inst* x_hmap_tp_s_c_get( const x_hmap_tp_s* o, tp_t key ); \
  static inline x_inst* x_hmap_tp_s_m_get( x_hmap_tp_s* o, tp_t key ); \
  static inline x_inst* x_hmap_tp_s_set_c( x_hmap_tp_s* o, tp_t key, const x_inst* val ); \
  static inline x_inst* x_hmap_tp_s_set_d( x_hmap_tp_s* o, tp_t key, x_inst* val ); \
  static inline x_hmap_tp_s* x_hmap_tp_s_remove( x_hmap_tp_s* o, tp_t key ); \
  static inline bl_t x_hmap_tp_s_exists( const x_hmap_tp_s* o, tp_t key ); \
  static inline x_hmap_tp_s* x_hmap_tp_s_clear( x_hmap_tp_s* o ); \
  static inline sz_t x_hmap_tp_s_keys( const x_hmap_tp_s* o ); \
  static inline sz_t x_hmap_tp_s_size( const x_hmap_tp_s* o ); \
  static inline tp_t x_hmap_tp_s_idx_key( const x_hmap_tp_s* o, sz_t idx ); \
  static inline const x_inst* x_hmap_tp_s_c_idx_val( const x_hmap_tp_s* o, sz_t idx ); \
  static inline x_inst* x_hmap_tp_s_m_idx_val( x_hmap_tp_s* o, sz_t idx ); \
  bcore_arr_tp_s* x_hmap_tp_s_get_key_arr( const x_hmap_tp_s* o, bcore_arr_tp_s* key_arr ); \
  static inline bcore_arr_tp_s* x_hmap_tp_s_all_keys( const x_hmap_tp_s* o ); \
  x_array* x_hmap_tp_s_m_get_val_arr( x_hmap_tp_s* o, x_array* val_arr ); \
  x_array* x_hmap_tp_s_c_get_val_arr( const x_hmap_tp_s* o, x_array* val_arr ); \
  static inline const x_inst* x_hmap_tp_s_c_get( const x_hmap_tp_s* o, tp_t key ){ const vd_t* e = bcore_hmap_tpaw_s_get(&(o->map),key ); return  e ? ((const x_inst*)((*(e)))) : NULL;} \
  static inline x_inst* x_hmap_tp_s_m_get( x_hmap_tp_s* o, tp_t key ){ vd_t* e = bcore_hmap_tpaw_s_get(&(o->map),key ); return  e ? ((x_inst*)((*(e)))) : NULL;} \
  static inline x_inst* x_hmap_tp_s_set_c( x_hmap_tp_s* o, tp_t key, const x_inst* val ){ vd_t* e = bcore_hmap_tpaw_s_set_c(&(o->map),key, val ); return  e ? ((x_inst*)((*(e)))) : NULL;} \
  static inline x_inst* x_hmap_tp_s_set_d( x_hmap_tp_s* o, tp_t key, x_inst* val ){ vd_t* e = bcore_hmap_tpaw_s_set_d(&(o->map),key, val ); return  e ? ((x_inst*)((*(e)))) : NULL;} \
  static inline x_hmap_tp_s* x_hmap_tp_s_remove( x_hmap_tp_s* o, tp_t key ){bcore_hmap_tpaw_s_remove(&(o->map),key );return o;} \
  static inline bl_t x_hmap_tp_s_exists( const x_hmap_tp_s* o, tp_t key ){return  bcore_hmap_tpaw_s_exists(&(o->map),key );} \
  static inline x_hmap_tp_s* x_hmap_tp_s_clear( x_hmap_tp_s* o ){bcore_hmap_tpaw_s_clear(&(o->map));return o;} \
  static inline sz_t x_hmap_tp_s_keys( const x_hmap_tp_s* o ){return  bcore_hmap_tpaw_s_keys(&(o->map));} \
  static inline sz_t x_hmap_tp_s_size( const x_hmap_tp_s* o ){return  bcore_hmap_tpaw_s_size(&(o->map));} \
  static inline tp_t x_hmap_tp_s_idx_key( const x_hmap_tp_s* o, sz_t idx ){return  bcore_hmap_tpaw_s_idx_key(&(o->map),idx );} \
  static inline const x_inst* x_hmap_tp_s_c_idx_val( const x_hmap_tp_s* o, sz_t idx ){return  ((const x_inst*)(bcore_hmap_tpaw_s_idx_val(&(o->map),idx )));} \
  static inline x_inst* x_hmap_tp_s_m_idx_val( x_hmap_tp_s* o, sz_t idx ){return  ((x_inst*)(bcore_hmap_tpaw_s_idx_val(&(o->map),idx )));} \
  static inline bcore_arr_tp_s* x_hmap_tp_s_all_keys( const x_hmap_tp_s* o ){return  x_hmap_tp_s_get_key_arr(o,bcore_arr_tp_s_create() );}
#define TYPEOF_x_hmap_tp_st_s 0xDF6A2FAABF097399ull
#define BETH_EXPAND_ITEM_x_hmap_tp_st_s \
  BCORE_DECLARE_OBJECT( x_hmap_tp_st_s ) \
  { \
      aware_t _; \
      x_hmap_tp_s map; \
  }; \
  st_s* x_hmap_tp_st_s_create_st_status( const x_hmap_tp_st_s* o ); \
  static inline tp_t x_hmap_tp_st_s_TE( const x_hmap_tp_st_s* o ); \
  static inline tp_t x_hmap_tp_st_s_TE( const x_hmap_tp_st_s* o ){ return  TYPEOF_st_s;}
#define BETH_EXPAND_GROUP_x_hmap_tp \
  BCORE_FORWARD_OBJECT( x_hmap_tp ); \
  BCORE_FORWARD_OBJECT( x_hmap_tp_s ); \
  BCORE_FORWARD_OBJECT( x_hmap_tp_st_s ); \
  x_hmap_tp_s* x_hmap_tp_m_map_( x_hmap_tp* o ); \
  const x_hmap_tp_s* x_hmap_tp_c_map_( const x_hmap_tp* o ); \
  static inline const x_inst* x_hmap_tp_c_get( const x_hmap_tp* o, tp_t key ); \
  static inline x_inst* x_hmap_tp_m_get( x_hmap_tp* o, tp_t key ); \
  static inline x_inst* x_hmap_tp_set_c( x_hmap_tp* o, tp_t key, const x_inst* val ); \
  static inline x_inst* x_hmap_tp_set_d( x_hmap_tp* o, tp_t key, x_inst* val ); \
  static inline x_hmap_tp* x_hmap_tp_remove( x_hmap_tp* o, tp_t key ); \
  static inline bl_t x_hmap_tp_exists( const x_hmap_tp* o, tp_t key ); \
  static inline x_hmap_tp* x_hmap_tp_clear( x_hmap_tp* o ); \
  static inline sz_t x_hmap_tp_keys( const x_hmap_tp* o ); \
  static inline sz_t x_hmap_tp_size( const x_hmap_tp* o ); \
  static inline tp_t x_hmap_tp_idx_key( const x_hmap_tp* o, sz_t idx ); \
  static inline const x_inst* x_hmap_tp_c_idx_val( const x_hmap_tp* o, sz_t idx ); \
  static inline x_inst* x_hmap_tp_m_idx_val( x_hmap_tp* o, sz_t idx ); \
  static inline bcore_arr_tp_s* x_hmap_tp_get_key_arr( const x_hmap_tp* o, bcore_arr_tp_s* key_arr ); \
  static inline x_array* x_hmap_tp_m_get_val_arr( x_hmap_tp* o, x_array* val_arr ); \
  static inline x_array* x_hmap_tp_c_get_val_arr( const x_hmap_tp* o, x_array* val_arr ); \
  static inline bcore_arr_tp_s* x_hmap_tp_all_keys( const x_hmap_tp* o ); \
  x_inst* x_hmap_tp_m_getf( x_hmap_tp* o, tp_t key ); \
  typedef tp_t (*x_hmap_tp_TE)(const x_hmap_tp* o ); \
  XOILA_DECLARE_SPECT( x_hmap_tp ) \
  { \
      bcore_spect_header_s header; \
      x_hmap_tp_TE TE; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( x_hmap_tp ) \
  static inline tp_t x_hmap_tp_a_TE( const x_hmap_tp* o ){ const x_hmap_tp_spect_s* p = x_hmap_tp_spect_s_get_aware( o ); assert( p->TE ); return p->TE( o );} \
  static inline bl_t x_hmap_tp_defines_TE( const x_hmap_tp* o ){ return  true;} \
  static inline tp_t x_hmap_tp_TE_default( const x_hmap_tp* o ){return  0;} \
  BETH_EXPAND_ITEM_x_hmap_tp_s \
  BETH_EXPAND_ITEM_x_hmap_tp_st_s \
  static inline const x_inst* x_hmap_tp_c_get( const x_hmap_tp* o, tp_t key ){return  x_hmap_tp_s_c_get(x_hmap_tp_c_map_(o),key );} \
  static inline x_inst* x_hmap_tp_m_get( x_hmap_tp* o, tp_t key ){return  x_hmap_tp_s_m_get(x_hmap_tp_m_map_(o),key );} \
  static inline x_inst* x_hmap_tp_set_c( x_hmap_tp* o, tp_t key, const x_inst* val ){return  x_hmap_tp_s_set_c(x_hmap_tp_m_map_(o),key, val );} \
  static inline x_inst* x_hmap_tp_set_d( x_hmap_tp* o, tp_t key, x_inst* val ){return  x_hmap_tp_s_set_d(x_hmap_tp_m_map_(o),key, val );} \
  static inline x_hmap_tp* x_hmap_tp_remove( x_hmap_tp* o, tp_t key ){return ((x_hmap_tp*)( x_hmap_tp_s_remove(x_hmap_tp_m_map_(o),key )));} \
  static inline bl_t x_hmap_tp_exists( const x_hmap_tp* o, tp_t key ){return  x_hmap_tp_s_exists(x_hmap_tp_c_map_(o),key );} \
  static inline x_hmap_tp* x_hmap_tp_clear( x_hmap_tp* o ){return ((x_hmap_tp*)( x_hmap_tp_s_clear(x_hmap_tp_m_map_(o))));} \
  static inline sz_t x_hmap_tp_keys( const x_hmap_tp* o ){return  x_hmap_tp_s_keys(x_hmap_tp_c_map_(o));} \
  static inline sz_t x_hmap_tp_size( const x_hmap_tp* o ){return  x_hmap_tp_s_size(x_hmap_tp_c_map_(o));} \
  static inline tp_t x_hmap_tp_idx_key( const x_hmap_tp* o, sz_t idx ){return  x_hmap_tp_s_idx_key(x_hmap_tp_c_map_(o),idx );} \
  static inline const x_inst* x_hmap_tp_c_idx_val( const x_hmap_tp* o, sz_t idx ){return  x_hmap_tp_s_c_idx_val(x_hmap_tp_c_map_(o),idx );} \
  static inline x_inst* x_hmap_tp_m_idx_val( x_hmap_tp* o, sz_t idx ){return  x_hmap_tp_s_m_idx_val(x_hmap_tp_m_map_(o),idx );} \
  static inline bcore_arr_tp_s* x_hmap_tp_get_key_arr( const x_hmap_tp* o, bcore_arr_tp_s* key_arr ){return  x_hmap_tp_s_get_key_arr(x_hmap_tp_c_map_(o),key_arr );} \
  static inline x_array* x_hmap_tp_m_get_val_arr( x_hmap_tp* o, x_array* val_arr ){return  x_hmap_tp_s_m_get_val_arr(x_hmap_tp_m_map_(o),val_arr );} \
  static inline x_array* x_hmap_tp_c_get_val_arr( const x_hmap_tp* o, x_array* val_arr ){return  x_hmap_tp_s_c_get_val_arr(x_hmap_tp_c_map_(o),val_arr );} \
  static inline bcore_arr_tp_s* x_hmap_tp_all_keys( const x_hmap_tp* o ){return  x_hmap_tp_s_all_keys(x_hmap_tp_c_map_(o));}

//----------------------------------------------------------------------------------------------------------------------
// group: x_hmap_tp_test

#define TYPEOF_x_hmap_tp_test 0x8BD597B168BD2B8Cull
#define TYPEOF_x_hmap_tp_test_spect_s 0x96F97B6D93952FC4ull
#define TYPEOF_x_hmap_tp_test_kv_s 0xC2118146626BE7CAull
#define BETH_EXPAND_ITEM_x_hmap_tp_test_kv_s \
  BCORE_DECLARE_OBJECT( x_hmap_tp_test_kv_s ) \
  { \
      tp_t key; \
      u3_t val; \
  };
#define TYPEOF_x_hmap_tp_test_kv_arr_s 0xD22D4BA75808939Aull
#define BETH_EXPAND_ITEM_x_hmap_tp_test_kv_arr_s \
  BCORE_DECLARE_OBJECT( x_hmap_tp_test_kv_arr_s ) \
  { \
      aware_t _; \
      BCORE_ARRAY_DYN_SOLID_STATIC_S( x_hmap_tp_test_kv_s, ); \
  };
#define TYPEOF_x_hmap_tp_test_val_s 0x033F73E3560ADAC2ull
#define BETH_EXPAND_ITEM_x_hmap_tp_test_val_s \
  BCORE_DECLARE_OBJECT( x_hmap_tp_test_val_s ) \
  { \
      aware_t _; \
      u3_t val; \
  };
#define TYPEOF_x_hmap_tp_test_map_s 0x97CB9A65516543C7ull
#define BETH_EXPAND_ITEM_x_hmap_tp_test_map_s \
  BCORE_DECLARE_OBJECT( x_hmap_tp_test_map_s ) \
  { \
      aware_t _; \
      x_hmap_tp_s map; \
  }; \
  st_s* x_hmap_tp_test_map_s_create_st_status( const x_hmap_tp_test_map_s* o ); \
  static inline tp_t x_hmap_tp_test_map_s_TE( const x_hmap_tp_test_map_s* o ); \
  static inline tp_t x_hmap_tp_test_map_s_TE( const x_hmap_tp_test_map_s* o ){ return  TYPEOF_x_hmap_tp_test_val_s;}
#define BETH_EXPAND_GROUP_x_hmap_tp_test \
  BCORE_FORWARD_OBJECT( x_hmap_tp_test ); \
  BCORE_FORWARD_OBJECT( x_hmap_tp_test_kv_s ); \
  BCORE_FORWARD_OBJECT( x_hmap_tp_test_kv_arr_s ); \
  BCORE_FORWARD_OBJECT( x_hmap_tp_test_val_s ); \
  BCORE_FORWARD_OBJECT( x_hmap_tp_test_map_s ); \
  void x_hmap_tp_test_selftest( void ); \
  XOILA_DECLARE_SPECT( x_hmap_tp_test ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( x_hmap_tp_test ) \
  BETH_EXPAND_ITEM_x_hmap_tp_test_kv_s \
  BETH_EXPAND_ITEM_x_hmap_tp_test_kv_arr_s \
  BETH_EXPAND_ITEM_x_hmap_tp_test_val_s \
  BETH_EXPAND_ITEM_x_hmap_tp_test_map_s

/**********************************************************************************************************************/
// source: bcore_x_deque.h

//----------------------------------------------------------------------------------------------------------------------
// group: x_deque

#define TYPEOF_x_deque 0xD6DC16133A49F2A4ull
#define TYPEOF_x_deque_spect_s 0xD53402FBE0FED69Cull
#define TYPEOF_x_deque_inst_adl_s 0x751854D4A643B3C7ull
#define BETH_EXPAND_ITEM_x_deque_inst_adl_s \
  BCORE_DECLARE_OBJECT( x_deque_inst_adl_s ) \
  { \
      aware_t _; \
      BCORE_ARRAY_DYN_LINK_STATIC_S( x_inst, ); \
  };
#define TYPEOF_x_deque_inst_s 0xEC22DB67742F0797ull
#define BETH_EXPAND_ITEM_x_deque_inst_s \
  BCORE_DECLARE_OBJECT( x_deque_inst_s ) \
  { \
      aware_t _; \
      x_deque_inst_adl_s adl; \
      sz_t size; \
      sz_t first; \
  }; \
  x_deque_inst_s* x_deque_inst_s_set_space( x_deque_inst_s* o, sz_t space ); \
  x_deque_inst_s* x_deque_inst_s_clear( x_deque_inst_s* o ); \
  static inline sz_t x_deque_inst_s_adl_idx( const x_deque_inst_s* o, sz_t index ); \
  static inline x_inst* x_deque_inst_s_m_get( x_deque_inst_s* o, sz_t index ); \
  static inline const x_inst* x_deque_inst_s_c_get( const x_deque_inst_s* o, sz_t index ); \
  static inline x_inst* x_deque_inst_s_m_get_first( x_deque_inst_s* o ); \
  static inline const x_inst* x_deque_inst_s_c_get_first( const x_deque_inst_s* o ); \
  static inline x_inst* x_deque_inst_s_m_get_last( x_deque_inst_s* o ); \
  static inline const x_inst* x_deque_inst_s_c_get_last( const x_deque_inst_s* o ); \
  x_deque_inst_s* x_deque_inst_s_inc_adl_size( x_deque_inst_s* o ); \
  x_inst* x_deque_inst_s_push_last_d( x_deque_inst_s* o, x_inst* inst ); \
  x_inst* x_deque_inst_s_push_first_d( x_deque_inst_s* o, x_inst* inst ); \
  x_inst* x_deque_inst_s_d_pop_last( x_deque_inst_s* o ); \
  x_inst* x_deque_inst_s_d_pop_first( x_deque_inst_s* o ); \
  static inline x_deque_inst_s* x_deque_inst_s_remove_first( x_deque_inst_s* o ); \
  static inline x_deque_inst_s* x_deque_inst_s_remove_last( x_deque_inst_s* o ); \
  x_deque_inst_s* x_deque_inst_s_remove_first_n( x_deque_inst_s* o, sz_t n ); \
  x_deque_inst_s* x_deque_inst_s_remove_last_n( x_deque_inst_s* o, sz_t n ); \
  static inline sz_t x_deque_inst_s_adl_idx( const x_deque_inst_s* o, sz_t index ){return  ( o->first + index ) % o->adl.size;} \
  static inline x_inst* x_deque_inst_s_m_get( x_deque_inst_s* o, sz_t index ){return  ( index >= 0 && index < o->size ) ? o->adl.data[ x_deque_inst_s_adl_idx(o,index ) ] : NULL;} \
  static inline const x_inst* x_deque_inst_s_c_get( const x_deque_inst_s* o, sz_t index ){return  ( index >= 0 && index < o->size ) ? o->adl.data[ x_deque_inst_s_adl_idx(o,index ) ] : NULL;} \
  static inline x_inst* x_deque_inst_s_m_get_first( x_deque_inst_s* o ){return  ( o->size > 0 ) ? o->adl.data[ o->first ] : NULL;} \
  static inline const x_inst* x_deque_inst_s_c_get_first( const x_deque_inst_s* o ){return  ( o->size > 0 ) ? o->adl.data[ o->first ] : NULL;} \
  static inline x_inst* x_deque_inst_s_m_get_last( x_deque_inst_s* o ){return  ( o->size > 0 ) ? o->adl.data[ x_deque_inst_s_adl_idx(o,o->size - 1 ) ] : NULL;} \
  static inline const x_inst* x_deque_inst_s_c_get_last( const x_deque_inst_s* o ){return  ( o->size > 0 ) ? o->adl.data[ x_deque_inst_s_adl_idx(o,o->size - 1 ) ] : NULL;} \
  static inline x_deque_inst_s* x_deque_inst_s_remove_first( x_deque_inst_s* o ){x_inst_discard(x_deque_inst_s_d_pop_first(o));return o;} \
  static inline x_deque_inst_s* x_deque_inst_s_remove_last( x_deque_inst_s* o ){x_inst_discard(x_deque_inst_s_d_pop_last(o));return o;}
#define BETH_EXPAND_GROUP_x_deque \
  BCORE_FORWARD_OBJECT( x_deque ); \
  BCORE_FORWARD_OBJECT( x_deque_inst_adl_s ); \
  BCORE_FORWARD_OBJECT( x_deque_inst_s ); \
  x_deque_inst_s* x_deque_m_inst_( x_deque* o ); \
  const x_deque_inst_s* x_deque_c_inst_( const x_deque* o ); \
  static inline x_deque* x_deque_set_space( x_deque* o, sz_t space ); \
  static inline x_deque* x_deque_clear( x_deque* o ); \
  static inline sz_t x_deque_size( const x_deque* o ); \
  static inline x_inst* x_deque_m_get( x_deque* o, sz_t index ); \
  static inline const x_inst* x_deque_c_get( const x_deque* o, sz_t index ); \
  static inline x_inst* x_deque_m_get_first( x_deque* o ); \
  static inline const x_inst* x_deque_c_get_first( const x_deque* o ); \
  static inline x_inst* x_deque_m_get_last( x_deque* o ); \
  static inline const x_inst* x_deque_c_get_last( const x_deque* o ); \
  static inline x_inst* x_deque_push_last_d( x_deque* o, x_inst* inst ); \
  static inline x_inst* x_deque_push_first_d( x_deque* o, x_inst* inst ); \
  static inline x_inst* x_deque_d_pop_last( x_deque* o ); \
  static inline x_inst* x_deque_d_pop_first( x_deque* o ); \
  static inline x_deque* x_deque_remove_last( x_deque* o ); \
  static inline x_deque* x_deque_remove_first( x_deque* o ); \
  static inline x_deque* x_deque_remove_last_n( x_deque* o, sz_t n ); \
  static inline x_deque* x_deque_remove_first_n( x_deque* o, sz_t n ); \
  XOILA_DECLARE_SPECT( x_deque ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( x_deque ) \
  BETH_EXPAND_ITEM_x_deque_inst_adl_s \
  BETH_EXPAND_ITEM_x_deque_inst_s \
  static inline x_deque* x_deque_set_space( x_deque* o, sz_t space ){ x_deque_inst_s_set_space(x_deque_m_inst_(o),space ); return  o;} \
  static inline x_deque* x_deque_clear( x_deque* o ){ x_deque_inst_s_clear(x_deque_m_inst_(o));            return  o;} \
  static inline sz_t x_deque_size( const x_deque* o ){return  x_deque_c_inst_(o)->size;} \
  static inline x_inst* x_deque_m_get( x_deque* o, sz_t index ){return  x_deque_inst_s_m_get(x_deque_m_inst_(o),index );} \
  static inline const x_inst* x_deque_c_get( const x_deque* o, sz_t index ){return  x_deque_inst_s_c_get(x_deque_c_inst_(o),index );} \
  static inline x_inst* x_deque_m_get_first( x_deque* o ){return  x_deque_inst_s_m_get_first(x_deque_m_inst_(o));} \
  static inline const x_inst* x_deque_c_get_first( const x_deque* o ){return  x_deque_inst_s_c_get_first(x_deque_c_inst_(o));} \
  static inline x_inst* x_deque_m_get_last( x_deque* o ){return  x_deque_inst_s_m_get_last(x_deque_m_inst_(o));} \
  static inline const x_inst* x_deque_c_get_last( const x_deque* o ){return  x_deque_inst_s_c_get_last(x_deque_c_inst_(o));} \
  static inline x_inst* x_deque_push_last_d( x_deque* o, x_inst* inst ){return  x_deque_inst_s_push_last_d(x_deque_m_inst_(o),inst );} \
  static inline x_inst* x_deque_push_first_d( x_deque* o, x_inst* inst ){return  x_deque_inst_s_push_first_d(x_deque_m_inst_(o),inst );} \
  static inline x_inst* x_deque_d_pop_last( x_deque* o ){return  x_deque_inst_s_d_pop_last(x_deque_m_inst_(o));} \
  static inline x_inst* x_deque_d_pop_first( x_deque* o ){return  x_deque_inst_s_d_pop_first(x_deque_m_inst_(o));} \
  static inline x_deque* x_deque_remove_last( x_deque* o ){return ((x_deque*)( x_deque_inst_s_remove_last(x_deque_m_inst_(o))));} \
  static inline x_deque* x_deque_remove_first( x_deque* o ){return ((x_deque*)( x_deque_inst_s_remove_first(x_deque_m_inst_(o))));} \
  static inline x_deque* x_deque_remove_last_n( x_deque* o, sz_t n ){return ((x_deque*)( x_deque_inst_s_remove_last_n(x_deque_m_inst_(o),n )));} \
  static inline x_deque* x_deque_remove_first_n( x_deque* o, sz_t n ){return ((x_deque*)( x_deque_inst_s_remove_first_n(x_deque_m_inst_(o),n )));}

/**********************************************************************************************************************/

vd_t bcore_xo_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/
// Manifesto


#endif // __bcore_xo_H
// XOICO_BODY_SIGNATURE 0x00E9F36631EEB4CC
// XOICO_FILE_SIGNATURE 0x9513FEAC8A1D6D1A
