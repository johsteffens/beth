/** This file was generated from xoila source code.
 *  Compiling Agent : xoico_compiler (C) 2020 J.B.Steffens
 *  Last File Update: 2021-01-27T11:32:28Z
 *
 *  Copyright and License of this File:
 *
 *  Generated code inherits the copyright and license of the underlying xoila source code.
 *  Source code defining this file is distributed across following files:
 *
 *  bcore_x_root_inexpandable.h
 *  bcore_x_inst.h
 *  bcore_x_array.h
 *  bcore_file.h
 *  bcore_spect_inst_call.h
 *  bcore_spect_via_call.h
 *  bcore_main.h
 *  bcore_hmap_name.h
 *  bcore_cday.h
 *  bcore_error_manager.h
 *  bcore_prsg.h
 *  bcore_arr_inexpandable.x
 *  bcore_flect_inexpandable.x
 *  bcore_hmap_inexpandable.x
 *  bcore_sink_inexpandable.x
 *  bcore_source_inexpandable.x
 *  bcore_st_inexpandable.x
 *
 */

#ifndef __bcore_xo_H
#define __bcore_xo_H

#include "bcore_control.h"

//To force a rebuild of this target by xoico, reset the hash key value below to 0.
#define HKEYOF_bcore 0xC77B2708004813E2ull

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

#define TYPEOF_x_inst 0xFC4EFF7569E22558ull
#define TYPEOF_x_inst_spect_s 0x6B610F4B8E350C20ull
#define BETH_EXPAND_GROUP_x_inst \
  BCORE_FORWARD_OBJECT( x_inst ); \
  static inline x_inst* x_inst_ifd( x_inst* o, bl_t cond, x_inst* b ); \
  static inline const x_inst* x_inst_ifc( const x_inst* o, bl_t cond, const x_inst* b ); \
  static inline const x_inst* x_inst_to_sink_txt_ml( const x_inst* o, bcore_sink* sink ); \
  static inline const x_inst* x_inst_to_sink_bin_ml( const x_inst* o, bcore_sink* sink ); \
  static inline x_inst* x_inst_from_source_txt_ml( x_inst* o, bcore_source* source ); \
  static inline x_inst* x_inst_from_source_bin_ml( x_inst* o, bcore_source* source ); \
  static inline const x_inst* x_inst_to_file_txt_ml( const x_inst* o, sc_t path ); \
  static inline const x_inst* x_inst_to_file_bin_ml( const x_inst* o, sc_t path ); \
  static inline x_inst* x_inst_from_file_txt_ml( x_inst* o, sc_t path ); \
  static inline x_inst* x_inst_from_file_bin_ml( x_inst* o, sc_t path ); \
  XOILA_DECLARE_SPECT( x_inst ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( x_inst ) \
  static inline x_inst* x_inst_ifd( x_inst* o, bl_t cond, x_inst* b ){ return  cond ? o : b;} \
  static inline const x_inst* x_inst_ifc( const x_inst* o, bl_t cond, const x_inst* b ){ return  cond ? o : b;} \
  static inline const x_inst* x_inst_to_sink_txt_ml( const x_inst* o, bcore_sink* sink ){ bcore_txt_ml_a_to_sink( o, sink ); return  o;} \
  static inline const x_inst* x_inst_to_sink_bin_ml( const x_inst* o, bcore_sink* sink ){ bcore_bin_ml_a_to_sink( o, sink ); return  o;} \
  static inline x_inst* x_inst_from_source_txt_ml( x_inst* o, bcore_source* source ){ bcore_txt_ml_a_from_source( o, source ); return  o;} \
  static inline x_inst* x_inst_from_source_bin_ml( x_inst* o, bcore_source* source ){ bcore_bin_ml_a_from_source( o, source ); return  o;} \
  static inline const x_inst* x_inst_to_file_txt_ml( const x_inst* o, sc_t path ){BLM_INIT_LEVEL(0); BLM_RETURNV(const x_inst*, ((const x_inst*)(x_inst_to_sink_txt_ml(o,((bcore_sink*)BLM_LEVEL_A_PUSH(0,bcore_file_open_sink(path ))) ))))} \
  static inline const x_inst* x_inst_to_file_bin_ml( const x_inst* o, sc_t path ){BLM_INIT_LEVEL(0); BLM_RETURNV(const x_inst*, ((const x_inst*)(x_inst_to_sink_bin_ml(o,((bcore_sink*)BLM_LEVEL_A_PUSH(0,bcore_file_open_sink(path ))) ))))} \
  static inline x_inst* x_inst_from_file_txt_ml( x_inst* o, sc_t path ){BLM_INIT_LEVEL(0); BLM_RETURNV(x_inst*, ((x_inst*)(x_inst_from_source_txt_ml(o,((bcore_source*)BLM_LEVEL_A_PUSH(0,bcore_file_open_source(path ))) ))))} \
  static inline x_inst* x_inst_from_file_bin_ml( x_inst* o, sc_t path ){BLM_INIT_LEVEL(0); BLM_RETURNV(x_inst*, ((x_inst*)(x_inst_from_source_bin_ml(o,((bcore_source*)BLM_LEVEL_A_PUSH(0,bcore_file_open_source(path ))) ))))}

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
  static inline x_array* x_array_t_set_size( x_array* o, tp_t t, sz_t size ); \
  static inline x_array* x_array_t_set_space( x_array* o, tp_t t, sz_t space ); \
  static inline x_array* x_array_t_clear( x_array* o, tp_t t ); \
  static inline x_array* x_array_t_sort( x_array* o, tp_t t, s2_t direction ); \
  x_inst* x_array_t_push_d( x_array* o, tp_t t, x_inst* v ); \
  x_inst* x_array_t_push_c( x_array* o, tp_t t, const x_inst* v ); \
  x_inst* x_array_t_push_t( x_array* o, tp_t t, tp_t val_type ); \
  x_inst* x_array_t_push( x_array* o, tp_t t ); \
  static inline x_array* x_array_clear( x_array* o ); \
  static inline x_array* x_array_set_size( x_array* o, sz_t size ); \
  static inline x_array* x_array_set_space( x_array* o, sz_t space ); \
  static inline x_array* x_array_sort( x_array* o, s2_t direction ); \
  static inline x_inst* x_array_push_d( x_array* o, x_inst* v ); \
  static inline x_inst* x_array_push_c( x_array* o, const x_inst* v ); \
  static inline x_inst* x_array_push_t( x_array* o, tp_t val_type ); \
  static inline x_inst* x_array_push( x_array* o ); \
  XOILA_DECLARE_SPECT( x_array ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( x_array ) \
  static inline x_array* x_array_t_set_size( x_array* o, tp_t t, sz_t size ){ bcore_array_t_set_size ( t, ((bcore_array*)(o)), size  ); return  o;} \
  static inline x_array* x_array_t_set_space( x_array* o, tp_t t, sz_t space ){ bcore_array_t_set_space( t, ((bcore_array*)(o)), space ); return  o;} \
  static inline x_array* x_array_t_clear( x_array* o, tp_t t ){ return  x_array_t_set_space(o,t, 0 );} \
  static inline x_array* x_array_t_sort( x_array* o, tp_t t, s2_t direction ){ bcore_array_t_sort( t, ((bcore_array*)(o)), 0, -1, direction ); return  o;} \
  static inline x_array* x_array_clear( x_array* o ){ return  x_array_t_clear(o,o->_ );} \
  static inline x_array* x_array_set_size( x_array* o, sz_t size ){ return  x_array_t_set_size(o,o->_, size );} \
  static inline x_array* x_array_set_space( x_array* o, sz_t space ){ return  x_array_t_set_space(o,o->_, space );} \
  static inline x_array* x_array_sort( x_array* o, s2_t direction ){ return  x_array_t_sort(o,o->_, direction );} \
  static inline x_inst* x_array_push_d( x_array* o, x_inst* v ){ return  x_array_t_push_d(o,o->_, v );} \
  static inline x_inst* x_array_push_c( x_array* o, const x_inst* v ){ return  x_array_t_push_c(o,o->_, v );} \
  static inline x_inst* x_array_push_t( x_array* o, tp_t val_type ){ return  x_array_t_push_t(o,o->_, val_type );} \
  static inline x_inst* x_array_push( x_array* o ){ return  x_array_t_push(o,o->_ );}

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
  void bcore_file_path_s_set_sc( bcore_file_path_s* o, sc_t name ); \
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
  bl_t bcore_file_find_descend( sc_t folder, sc_t name, st_s* result ); \
  bcore_source* bcore_file_open_source( sc_t name ); \
  bcore_source* bcore_file_open_source_path( const bcore_file_path_s* path ); \
  bcore_sink* bcore_file_open_sink( sc_t name ); \
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
  static inline bl_t bcore_inst_call_p_defines_init_x( const bcore_inst_call_spect_s* p ){ return p->init_x != NULL;} \
  static inline void bcore_inst_call_p_down_e( const bcore_inst_call_spect_s* p, bcore_inst_call* o ){ assert( p->down_e ); p->down_e( o );} \
  static inline bl_t bcore_inst_call_p_defines_down_e( const bcore_inst_call_spect_s* p ){ return p->down_e != NULL;} \
  static inline void bcore_inst_call_p_copy_e( const bcore_inst_call_spect_s* p, bcore_inst_call* o, const bcore_inst_call* src ){ assert( p->copy_e ); p->copy_e( o, src );} \
  static inline bl_t bcore_inst_call_p_defines_copy_e( const bcore_inst_call_spect_s* p ){ return p->copy_e != NULL;} \
  static inline void bcore_inst_call_p_copy_x( const bcore_inst_call_spect_s* p, bcore_inst_call* o, const bcore_inst_call* src ){ assert( p->copy_x ); p->copy_x( o, src );} \
  static inline bl_t bcore_inst_call_p_defines_copy_x( const bcore_inst_call_spect_s* p ){ return p->copy_x != NULL;} \
  static inline void bcore_inst_call_p_discard_e( const bcore_inst_call_spect_s* p, bcore_inst_call* o ){ assert( p->discard_e ); p->discard_e( o );} \
  static inline bl_t bcore_inst_call_p_defines_discard_e( const bcore_inst_call_spect_s* p ){ return p->discard_e != NULL;}

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
  static inline void bcore_via_call_t_source( tp_t t, bcore_via_call* o, bcore_source* source ){ const bcore_via_call_spect_s* p = bcore_via_call_spect_s_get_typed( t ); assert( p->source ); p->source( o, source );} \
  static inline bl_t bcore_via_call_t_defines_source( tp_t t ){ return bcore_via_call_spect_s_get_typed( t )->source != NULL;} \
  static inline void bcore_via_call_p_source( const bcore_via_call_spect_s* p, bcore_via_call* o, bcore_source* source ){ assert( p->source ); p->source( o, source );} \
  static inline bl_t bcore_via_call_p_defines_source( const bcore_via_call_spect_s* p ){ return p->source != NULL;} \
  static inline void bcore_via_call_r_source( const sr_s* o, bcore_source* source ){ ASSERT( !sr_s_is_const( o ) ); const bcore_via_call_spect_s* p = (const bcore_via_call_spect_s*)ch_spect_p( o->p, TYPEOF_bcore_via_call_spect_s ); assert( p->source ); p->source( o->o, source );} \
  static inline bl_t bcore_via_call_r_defines_source( const sr_s* o ){ return ( (bcore_via_call_spect_s*)ch_spect_p( o->p, TYPEOF_bcore_via_call_spect_s ) )->source != NULL;} \
  static inline void bcore_via_call_t_mutated( tp_t t, bcore_via_call* o ){ const bcore_via_call_spect_s* p = bcore_via_call_spect_s_get_typed( t ); assert( p->mutated ); p->mutated( o );} \
  static inline bl_t bcore_via_call_t_defines_mutated( tp_t t ){ return bcore_via_call_spect_s_get_typed( t )->mutated != NULL;} \
  static inline void bcore_via_call_p_mutated( const bcore_via_call_spect_s* p, bcore_via_call* o ){ assert( p->mutated ); p->mutated( o );} \
  static inline bl_t bcore_via_call_p_defines_mutated( const bcore_via_call_spect_s* p ){ return p->mutated != NULL;} \
  static inline void bcore_via_call_r_mutated( const sr_s* o ){ ASSERT( !sr_s_is_const( o ) ); const bcore_via_call_spect_s* p = (const bcore_via_call_spect_s*)ch_spect_p( o->p, TYPEOF_bcore_via_call_spect_s ); assert( p->mutated ); p->mutated( o->o );} \
  static inline bl_t bcore_via_call_r_defines_mutated( const sr_s* o ){ return ( (bcore_via_call_spect_s*)ch_spect_p( o->p, TYPEOF_bcore_via_call_spect_s ) )->mutated != NULL;} \
  static inline void bcore_via_call_t_shelve( tp_t t, bcore_via_call* o ){ const bcore_via_call_spect_s* p = bcore_via_call_spect_s_get_typed( t ); assert( p->shelve ); p->shelve( o );} \
  static inline bl_t bcore_via_call_t_defines_shelve( tp_t t ){ return bcore_via_call_spect_s_get_typed( t )->shelve != NULL;} \
  static inline void bcore_via_call_p_shelve( const bcore_via_call_spect_s* p, bcore_via_call* o ){ assert( p->shelve ); p->shelve( o );} \
  static inline bl_t bcore_via_call_p_defines_shelve( const bcore_via_call_spect_s* p ){ return p->shelve != NULL;} \
  static inline void bcore_via_call_r_shelve( const sr_s* o ){ ASSERT( !sr_s_is_const( o ) ); const bcore_via_call_spect_s* p = (const bcore_via_call_spect_s*)ch_spect_p( o->p, TYPEOF_bcore_via_call_spect_s ); assert( p->shelve ); p->shelve( o->o );} \
  static inline bl_t bcore_via_call_r_defines_shelve( const sr_s* o ){ return ( (bcore_via_call_spect_s*)ch_spect_p( o->p, TYPEOF_bcore_via_call_spect_s ) )->shelve != NULL;}

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
      bcore_interpreter* interpreter; \
      bcore_arr_st_s args; \
      bcore_mutex_s mutex; \
      bl_t use_first_argument; \
      sc_t local_file; \
      sc_t global_file; \
  }; \
  bl_t bcore_main_frame_s_exit_required( const bcore_main_frame_s* o );
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
  }; \
  er_t bcore_main_set_s_main( bcore_main_set_s* o, bcore_main_frame_s* frame );
#define BETH_EXPAND_GROUP_bcore_main \
  BCORE_FORWARD_OBJECT( bcore_main ); \
  BCORE_FORWARD_OBJECT( bcore_main_frame_s ); \
  BCORE_FORWARD_OBJECT( bcore_main_arr_s ); \
  BCORE_FORWARD_OBJECT( bcore_main_set_s ); \
  typedef er_t (*bcore_main_main)(bcore_main* o, bcore_main_frame_s* frame ); \
  XOILA_DECLARE_SPECT( bcore_main ) \
  { \
      bcore_spect_header_s header; \
      bcore_main_main main; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bcore_main ) \
  BETH_EXPAND_ITEM_bcore_main_frame_s \
  static inline er_t bcore_main_a_main( bcore_main* o, bcore_main_frame_s* frame ){ const bcore_main_spect_s* p = bcore_main_spect_s_get_aware( o ); assert( p->main ); return p->main( o, frame );} \
  static inline bl_t bcore_main_defines_main( const bcore_main* o ){ return  true;} \
  static inline er_t bcore_main_r_main( const sr_s* o, bcore_main_frame_s* frame ){ ASSERT( !sr_s_is_const( o ) ); const bcore_main_spect_s* p = (const bcore_main_spect_s*)ch_spect_p( o->p, TYPEOF_bcore_main_spect_s ); assert( p->main ); return p->main( o->o, frame );} \
  static inline bl_t bcore_main_r_defines_main( const sr_s* o ){ return  true;} \
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
  };
#define TYPEOF_bcore_cday_utc_s 0xB3FF29791CCF8201ull
#define BETH_EXPAND_ITEM_bcore_cday_utc_s \
  BCORE_DECLARE_OBJECT( bcore_cday_utc_s ) \
  { \
      aware_t _; \
      s2_t cday; \
      s2_t ms; \
  };
#define BETH_EXPAND_GROUP_bcore_cday \
  BCORE_FORWARD_OBJECT( bcore_cday ); \
  BCORE_FORWARD_OBJECT( bcore_cday_ymd_s ); \
  BCORE_FORWARD_OBJECT( bcore_cday_utc_s ); \
  XOILA_DECLARE_SPECT( bcore_cday ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bcore_cday ) \
  BETH_EXPAND_ITEM_bcore_cday_ymd_s \
  BETH_EXPAND_ITEM_bcore_cday_utc_s

/**********************************************************************************************************************/
// source: bcore_error_manager.h

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
#define TYPEOF_general_error 0x4D9E984C5B21FAEAull
#define TYPEOF_parse_error 0x690AF8765815EC4Dull
#define TYPEOF_plant_error 0x6C62A00462378607ull
#define TYPEOF_error_stack 0xF5EEC6731159BABAull
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
  typedef void (*bcore_prsg_gen)(bcore_prsg* o ); \
  typedef u3_t (*bcore_prsg_gen_bits_u3)(bcore_prsg* o, sz_t bits ); \
  typedef u3_t (*bcore_prsg_gen_u3)(bcore_prsg* o ); \
  typedef f3_t (*bcore_prsg_gen_f3)(bcore_prsg* o, f3_t min, f3_t max ); \
  typedef bl_t (*bcore_prsg_gen_bl)(bcore_prsg* o ); \
  typedef bcore_prsg* (*bcore_prsg_set_state_u3)(bcore_prsg* o, u3_t seed ); \
  typedef bcore_prsg* (*bcore_prsg_set_state_mix)(bcore_prsg* o, const bcore_prsg* a, const bcore_prsg* b ); \
  typedef void (*bcore_prsg_reseed)(bcore_prsg* o, u3_t seed ); \
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
  static inline bl_t bcore_prsg_state_bl_default( const bcore_prsg* o ){ return  bcore_prsg_a_state_bits_u3( o,1 ) ? true : false;} \
  static inline void bcore_prsg_a_gen( bcore_prsg* o ){ const bcore_prsg_spect_s* p = bcore_prsg_spect_s_get_aware( o ); assert( p->gen ); p->gen( o );} \
  static inline bl_t bcore_prsg_defines_gen( const bcore_prsg* o ){ return  true;} \
  static inline u3_t bcore_prsg_a_gen_bits_u3( bcore_prsg* o, sz_t bits ){ const bcore_prsg_spect_s* p = bcore_prsg_spect_s_get_aware( o ); assert( p->gen_bits_u3 ); return p->gen_bits_u3( o, bits );} \
  static inline bl_t bcore_prsg_defines_gen_bits_u3( const bcore_prsg* o ){ return  true;} \
  static inline u3_t bcore_prsg_a_gen_u3( bcore_prsg* o ){ const bcore_prsg_spect_s* p = bcore_prsg_spect_s_get_aware( o ); assert( p->gen_u3 ); return p->gen_u3( o );} \
  static inline bl_t bcore_prsg_defines_gen_u3( const bcore_prsg* o ){ return  true;} \
  static inline f3_t bcore_prsg_a_gen_f3( bcore_prsg* o, f3_t min, f3_t max ){ const bcore_prsg_spect_s* p = bcore_prsg_spect_s_get_aware( o ); assert( p->gen_f3 ); return p->gen_f3( o, min, max );} \
  static inline bl_t bcore_prsg_defines_gen_f3( const bcore_prsg* o ){ return  true;} \
  static inline bl_t bcore_prsg_a_gen_bl( bcore_prsg* o ){ const bcore_prsg_spect_s* p = bcore_prsg_spect_s_get_aware( o ); assert( p->gen_bl ); return p->gen_bl( o );} \
  static inline bl_t bcore_prsg_defines_gen_bl( const bcore_prsg* o ){ return  true;} \
  static inline bl_t bcore_prsg_gen_bl_default( bcore_prsg* o ){ return  bcore_prsg_a_gen_bits_u3( o,1 ) ? true : false;} \
  static inline bcore_prsg* bcore_prsg_a_set_state_u3( bcore_prsg* o, u3_t seed ){ const bcore_prsg_spect_s* p = bcore_prsg_spect_s_get_aware( o ); assert( p->set_state_u3 ); return p->set_state_u3( o, seed );} \
  static inline bl_t bcore_prsg_defines_set_state_u3( const bcore_prsg* o ){ return  true;} \
  static inline bcore_prsg* bcore_prsg_a_set_state_mix( bcore_prsg* o, const bcore_prsg* a, const bcore_prsg* b ){ const bcore_prsg_spect_s* p = bcore_prsg_spect_s_get_aware( o ); assert( p->set_state_mix ); return p->set_state_mix( o, a, b );} \
  static inline bl_t bcore_prsg_defines_set_state_mix( const bcore_prsg* o ){ return  true;} \
  bcore_prsg* bcore_prsg_set_state_mix_default( bcore_prsg* o, const bcore_prsg* a, const bcore_prsg* b ); \
  static inline void bcore_prsg_a_reseed( bcore_prsg* o, u3_t seed ){ const bcore_prsg_spect_s* p = bcore_prsg_spect_s_get_aware( o ); assert( p->reseed ); p->reseed( o, seed );} \
  static inline bl_t bcore_prsg_defines_reseed( const bcore_prsg* o ){ return  true;} \
  static inline void bcore_prsg_reseed_default( bcore_prsg* o, u3_t seed ){ bcore_prsg_a_set_state_u3( o,seed );} \
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
  u3_t bcore_prsg_lcg_u2_00_s_gen_u3( bcore_prsg_lcg_u2_00_s* o ); \
  u3_t bcore_prsg_lcg_u2_00_s_state_bits_u3( const bcore_prsg_lcg_u2_00_s* o, sz_t bits ); \
  u3_t bcore_prsg_lcg_u2_00_s_gen_bits_u3( bcore_prsg_lcg_u2_00_s* o, sz_t bits ); \
  f3_t bcore_prsg_lcg_u2_00_s_state_f3( const bcore_prsg_lcg_u2_00_s* o, f3_t min, f3_t max ); \
  f3_t bcore_prsg_lcg_u2_00_s_gen_f3( bcore_prsg_lcg_u2_00_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_lcg_u2_00_s_bits( const bcore_prsg_lcg_u2_00_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_00_s_max_u3( const bcore_prsg_lcg_u2_00_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_00_s_min_u3( const bcore_prsg_lcg_u2_00_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_00_s_state_u3( const bcore_prsg_lcg_u2_00_s* o ); \
  static inline bcore_prsg* bcore_prsg_lcg_u2_00_s_set_state_u3( bcore_prsg_lcg_u2_00_s* o, u3_t seed ); \
  static inline void bcore_prsg_lcg_u2_00_s_gen( bcore_prsg_lcg_u2_00_s* o ); \
  static inline sz_t bcore_prsg_lcg_u2_00_s_bits( const bcore_prsg_lcg_u2_00_s* o ){ return  32;} \
  static inline u3_t bcore_prsg_lcg_u2_00_s_max_u3( const bcore_prsg_lcg_u2_00_s* o ){ return  0xFFFFFFFFu;} \
  static inline u3_t bcore_prsg_lcg_u2_00_s_min_u3( const bcore_prsg_lcg_u2_00_s* o ){ return  0;} \
  static inline u3_t bcore_prsg_lcg_u2_00_s_state_u3( const bcore_prsg_lcg_u2_00_s* o ){ return  o->state;} \
  static inline bcore_prsg* bcore_prsg_lcg_u2_00_s_set_state_u3( bcore_prsg_lcg_u2_00_s* o, u3_t seed ){ o->state = seed ^ ( seed >> 32 ); return  (bcore_prsg*)o;} \
  static inline void bcore_prsg_lcg_u2_00_s_gen( bcore_prsg_lcg_u2_00_s* o ){ o->state = bcore_lcg00_u2( o->state );}
#define TYPEOF_bcore_prsg_lcg_u2_01_s 0xE310A77651EDDF10ull
#define BETH_EXPAND_ITEM_bcore_prsg_lcg_u2_01_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_lcg_u2_01_s ) \
  { \
      aware_t _; \
      u2_t state; \
  }; \
  u3_t bcore_prsg_lcg_u2_01_s_gen_u3( bcore_prsg_lcg_u2_01_s* o ); \
  u3_t bcore_prsg_lcg_u2_01_s_state_bits_u3( const bcore_prsg_lcg_u2_01_s* o, sz_t bits ); \
  u3_t bcore_prsg_lcg_u2_01_s_gen_bits_u3( bcore_prsg_lcg_u2_01_s* o, sz_t bits ); \
  f3_t bcore_prsg_lcg_u2_01_s_state_f3( const bcore_prsg_lcg_u2_01_s* o, f3_t min, f3_t max ); \
  f3_t bcore_prsg_lcg_u2_01_s_gen_f3( bcore_prsg_lcg_u2_01_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_lcg_u2_01_s_bits( const bcore_prsg_lcg_u2_01_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_01_s_max_u3( const bcore_prsg_lcg_u2_01_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_01_s_min_u3( const bcore_prsg_lcg_u2_01_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_01_s_state_u3( const bcore_prsg_lcg_u2_01_s* o ); \
  static inline bcore_prsg* bcore_prsg_lcg_u2_01_s_set_state_u3( bcore_prsg_lcg_u2_01_s* o, u3_t seed ); \
  static inline void bcore_prsg_lcg_u2_01_s_gen( bcore_prsg_lcg_u2_01_s* o ); \
  static inline sz_t bcore_prsg_lcg_u2_01_s_bits( const bcore_prsg_lcg_u2_01_s* o ){ return  32;} \
  static inline u3_t bcore_prsg_lcg_u2_01_s_max_u3( const bcore_prsg_lcg_u2_01_s* o ){ return  0xFFFFFFFFu;} \
  static inline u3_t bcore_prsg_lcg_u2_01_s_min_u3( const bcore_prsg_lcg_u2_01_s* o ){ return  0;} \
  static inline u3_t bcore_prsg_lcg_u2_01_s_state_u3( const bcore_prsg_lcg_u2_01_s* o ){ return  o->state;} \
  static inline bcore_prsg* bcore_prsg_lcg_u2_01_s_set_state_u3( bcore_prsg_lcg_u2_01_s* o, u3_t seed ){ o->state = seed ^ ( seed >> 32 ); return  (bcore_prsg*)o;} \
  static inline void bcore_prsg_lcg_u2_01_s_gen( bcore_prsg_lcg_u2_01_s* o ){ o->state = bcore_lcg01_u2( o->state );}
#define TYPEOF_bcore_prsg_lcg_u2_02_s 0xDBC196764E2C002Dull
#define BETH_EXPAND_ITEM_bcore_prsg_lcg_u2_02_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_lcg_u2_02_s ) \
  { \
      aware_t _; \
      u2_t state; \
  }; \
  u3_t bcore_prsg_lcg_u2_02_s_gen_u3( bcore_prsg_lcg_u2_02_s* o ); \
  u3_t bcore_prsg_lcg_u2_02_s_state_bits_u3( const bcore_prsg_lcg_u2_02_s* o, sz_t bits ); \
  u3_t bcore_prsg_lcg_u2_02_s_gen_bits_u3( bcore_prsg_lcg_u2_02_s* o, sz_t bits ); \
  f3_t bcore_prsg_lcg_u2_02_s_state_f3( const bcore_prsg_lcg_u2_02_s* o, f3_t min, f3_t max ); \
  f3_t bcore_prsg_lcg_u2_02_s_gen_f3( bcore_prsg_lcg_u2_02_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_lcg_u2_02_s_bits( const bcore_prsg_lcg_u2_02_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_02_s_max_u3( const bcore_prsg_lcg_u2_02_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_02_s_min_u3( const bcore_prsg_lcg_u2_02_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_02_s_state_u3( const bcore_prsg_lcg_u2_02_s* o ); \
  static inline bcore_prsg* bcore_prsg_lcg_u2_02_s_set_state_u3( bcore_prsg_lcg_u2_02_s* o, u3_t seed ); \
  static inline void bcore_prsg_lcg_u2_02_s_gen( bcore_prsg_lcg_u2_02_s* o ); \
  static inline sz_t bcore_prsg_lcg_u2_02_s_bits( const bcore_prsg_lcg_u2_02_s* o ){ return  32;} \
  static inline u3_t bcore_prsg_lcg_u2_02_s_max_u3( const bcore_prsg_lcg_u2_02_s* o ){ return  0xFFFFFFFFu;} \
  static inline u3_t bcore_prsg_lcg_u2_02_s_min_u3( const bcore_prsg_lcg_u2_02_s* o ){ return  0;} \
  static inline u3_t bcore_prsg_lcg_u2_02_s_state_u3( const bcore_prsg_lcg_u2_02_s* o ){ return  o->state;} \
  static inline bcore_prsg* bcore_prsg_lcg_u2_02_s_set_state_u3( bcore_prsg_lcg_u2_02_s* o, u3_t seed ){ o->state = seed ^ ( seed >> 32 ); return  (bcore_prsg*)o;} \
  static inline void bcore_prsg_lcg_u2_02_s_gen( bcore_prsg_lcg_u2_02_s* o ){ o->state = bcore_lcg02_u2( o->state );}
#define TYPEOF_bcore_prsg_lcg_u2_03_s 0xD3995D7649B1EC92ull
#define BETH_EXPAND_ITEM_bcore_prsg_lcg_u2_03_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_lcg_u2_03_s ) \
  { \
      aware_t _; \
      u2_t state; \
  }; \
  u3_t bcore_prsg_lcg_u2_03_s_gen_u3( bcore_prsg_lcg_u2_03_s* o ); \
  u3_t bcore_prsg_lcg_u2_03_s_state_bits_u3( const bcore_prsg_lcg_u2_03_s* o, sz_t bits ); \
  u3_t bcore_prsg_lcg_u2_03_s_gen_bits_u3( bcore_prsg_lcg_u2_03_s* o, sz_t bits ); \
  f3_t bcore_prsg_lcg_u2_03_s_state_f3( const bcore_prsg_lcg_u2_03_s* o, f3_t min, f3_t max ); \
  f3_t bcore_prsg_lcg_u2_03_s_gen_f3( bcore_prsg_lcg_u2_03_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_lcg_u2_03_s_bits( const bcore_prsg_lcg_u2_03_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_03_s_max_u3( const bcore_prsg_lcg_u2_03_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_03_s_min_u3( const bcore_prsg_lcg_u2_03_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_03_s_state_u3( const bcore_prsg_lcg_u2_03_s* o ); \
  static inline bcore_prsg* bcore_prsg_lcg_u2_03_s_set_state_u3( bcore_prsg_lcg_u2_03_s* o, u3_t seed ); \
  static inline void bcore_prsg_lcg_u2_03_s_gen( bcore_prsg_lcg_u2_03_s* o ); \
  static inline sz_t bcore_prsg_lcg_u2_03_s_bits( const bcore_prsg_lcg_u2_03_s* o ){ return  32;} \
  static inline u3_t bcore_prsg_lcg_u2_03_s_max_u3( const bcore_prsg_lcg_u2_03_s* o ){ return  0xFFFFFFFFu;} \
  static inline u3_t bcore_prsg_lcg_u2_03_s_min_u3( const bcore_prsg_lcg_u2_03_s* o ){ return  0;} \
  static inline u3_t bcore_prsg_lcg_u2_03_s_state_u3( const bcore_prsg_lcg_u2_03_s* o ){ return  o->state;} \
  static inline bcore_prsg* bcore_prsg_lcg_u2_03_s_set_state_u3( bcore_prsg_lcg_u2_03_s* o, u3_t seed ){ o->state = seed ^ ( seed >> 32 ); return  (bcore_prsg*)o;} \
  static inline void bcore_prsg_lcg_u2_03_s_gen( bcore_prsg_lcg_u2_03_s* o ){ o->state = bcore_lcg03_u2( o->state );}
#define TYPEOF_bcore_prsg_lcg_u2_04_s 0x0E667C766A7AFEFFull
#define BETH_EXPAND_ITEM_bcore_prsg_lcg_u2_04_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_lcg_u2_04_s ) \
  { \
      aware_t _; \
      u2_t state; \
  }; \
  u3_t bcore_prsg_lcg_u2_04_s_gen_u3( bcore_prsg_lcg_u2_04_s* o ); \
  u3_t bcore_prsg_lcg_u2_04_s_state_bits_u3( const bcore_prsg_lcg_u2_04_s* o, sz_t bits ); \
  u3_t bcore_prsg_lcg_u2_04_s_gen_bits_u3( bcore_prsg_lcg_u2_04_s* o, sz_t bits ); \
  f3_t bcore_prsg_lcg_u2_04_s_state_f3( const bcore_prsg_lcg_u2_04_s* o, f3_t min, f3_t max ); \
  f3_t bcore_prsg_lcg_u2_04_s_gen_f3( bcore_prsg_lcg_u2_04_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_lcg_u2_04_s_bits( const bcore_prsg_lcg_u2_04_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_04_s_max_u3( const bcore_prsg_lcg_u2_04_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_04_s_min_u3( const bcore_prsg_lcg_u2_04_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_04_s_state_u3( const bcore_prsg_lcg_u2_04_s* o ); \
  static inline bcore_prsg* bcore_prsg_lcg_u2_04_s_set_state_u3( bcore_prsg_lcg_u2_04_s* o, u3_t seed ); \
  static inline void bcore_prsg_lcg_u2_04_s_gen( bcore_prsg_lcg_u2_04_s* o ); \
  static inline sz_t bcore_prsg_lcg_u2_04_s_bits( const bcore_prsg_lcg_u2_04_s* o ){ return  32;} \
  static inline u3_t bcore_prsg_lcg_u2_04_s_max_u3( const bcore_prsg_lcg_u2_04_s* o ){ return  0xFFFFFFFFu;} \
  static inline u3_t bcore_prsg_lcg_u2_04_s_min_u3( const bcore_prsg_lcg_u2_04_s* o ){ return  0;} \
  static inline u3_t bcore_prsg_lcg_u2_04_s_state_u3( const bcore_prsg_lcg_u2_04_s* o ){ return  o->state;} \
  static inline bcore_prsg* bcore_prsg_lcg_u2_04_s_set_state_u3( bcore_prsg_lcg_u2_04_s* o, u3_t seed ){ o->state = seed ^ ( seed >> 32 ); return  (bcore_prsg*)o;} \
  static inline void bcore_prsg_lcg_u2_04_s_gen( bcore_prsg_lcg_u2_04_s* o ){ o->state = bcore_lcg04_u2( o->state );}
#define TYPEOF_bcore_prsg_lcg_u2_05_s 0x0718637666BAC584ull
#define BETH_EXPAND_ITEM_bcore_prsg_lcg_u2_05_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_lcg_u2_05_s ) \
  { \
      aware_t _; \
      u2_t state; \
  }; \
  u3_t bcore_prsg_lcg_u2_05_s_gen_u3( bcore_prsg_lcg_u2_05_s* o ); \
  u3_t bcore_prsg_lcg_u2_05_s_state_bits_u3( const bcore_prsg_lcg_u2_05_s* o, sz_t bits ); \
  u3_t bcore_prsg_lcg_u2_05_s_gen_bits_u3( bcore_prsg_lcg_u2_05_s* o, sz_t bits ); \
  f3_t bcore_prsg_lcg_u2_05_s_state_f3( const bcore_prsg_lcg_u2_05_s* o, f3_t min, f3_t max ); \
  f3_t bcore_prsg_lcg_u2_05_s_gen_f3( bcore_prsg_lcg_u2_05_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_lcg_u2_05_s_bits( const bcore_prsg_lcg_u2_05_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_05_s_max_u3( const bcore_prsg_lcg_u2_05_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_05_s_min_u3( const bcore_prsg_lcg_u2_05_s* o ); \
  static inline u3_t bcore_prsg_lcg_u2_05_s_state_u3( const bcore_prsg_lcg_u2_05_s* o ); \
  static inline bcore_prsg* bcore_prsg_lcg_u2_05_s_set_state_u3( bcore_prsg_lcg_u2_05_s* o, u3_t seed ); \
  static inline void bcore_prsg_lcg_u2_05_s_gen( bcore_prsg_lcg_u2_05_s* o ); \
  static inline sz_t bcore_prsg_lcg_u2_05_s_bits( const bcore_prsg_lcg_u2_05_s* o ){ return  32;} \
  static inline u3_t bcore_prsg_lcg_u2_05_s_max_u3( const bcore_prsg_lcg_u2_05_s* o ){ return  0xFFFFFFFFu;} \
  static inline u3_t bcore_prsg_lcg_u2_05_s_min_u3( const bcore_prsg_lcg_u2_05_s* o ){ return  0;} \
  static inline u3_t bcore_prsg_lcg_u2_05_s_state_u3( const bcore_prsg_lcg_u2_05_s* o ){ return  o->state;} \
  static inline bcore_prsg* bcore_prsg_lcg_u2_05_s_set_state_u3( bcore_prsg_lcg_u2_05_s* o, u3_t seed ){ o->state = seed ^ ( seed >> 32 ); return  (bcore_prsg*)o;} \
  static inline void bcore_prsg_lcg_u2_05_s_gen( bcore_prsg_lcg_u2_05_s* o ){ o->state = bcore_lcg05_u2( o->state );}
#define TYPEOF_bcore_prsg_lcg_u3_00_s 0xE7FB4DA7051A0C0Aull
#define BETH_EXPAND_ITEM_bcore_prsg_lcg_u3_00_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_lcg_u3_00_s ) \
  { \
      aware_t _; \
      u3_t state; \
  }; \
  u3_t bcore_prsg_lcg_u3_00_s_gen_u3( bcore_prsg_lcg_u3_00_s* o ); \
  u3_t bcore_prsg_lcg_u3_00_s_state_bits_u3( const bcore_prsg_lcg_u3_00_s* o, sz_t bits ); \
  u3_t bcore_prsg_lcg_u3_00_s_gen_bits_u3( bcore_prsg_lcg_u3_00_s* o, sz_t bits ); \
  f3_t bcore_prsg_lcg_u3_00_s_state_f3( const bcore_prsg_lcg_u3_00_s* o, f3_t min, f3_t max ); \
  f3_t bcore_prsg_lcg_u3_00_s_gen_f3( bcore_prsg_lcg_u3_00_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_lcg_u3_00_s_bits( const bcore_prsg_lcg_u3_00_s* o ); \
  static inline u3_t bcore_prsg_lcg_u3_00_s_max_u3( const bcore_prsg_lcg_u3_00_s* o ); \
  static inline u3_t bcore_prsg_lcg_u3_00_s_min_u3( const bcore_prsg_lcg_u3_00_s* o ); \
  static inline u3_t bcore_prsg_lcg_u3_00_s_state_u3( const bcore_prsg_lcg_u3_00_s* o ); \
  static inline bcore_prsg* bcore_prsg_lcg_u3_00_s_set_state_u3( bcore_prsg_lcg_u3_00_s* o, u3_t seed ); \
  static inline void bcore_prsg_lcg_u3_00_s_gen( bcore_prsg_lcg_u3_00_s* o ); \
  static inline sz_t bcore_prsg_lcg_u3_00_s_bits( const bcore_prsg_lcg_u3_00_s* o ){ return  64;} \
  static inline u3_t bcore_prsg_lcg_u3_00_s_max_u3( const bcore_prsg_lcg_u3_00_s* o ){ return  0xFFFFFFFFFFFFFFFFull;} \
  static inline u3_t bcore_prsg_lcg_u3_00_s_min_u3( const bcore_prsg_lcg_u3_00_s* o ){ return  0;} \
  static inline u3_t bcore_prsg_lcg_u3_00_s_state_u3( const bcore_prsg_lcg_u3_00_s* o ){ return  o->state;} \
  static inline bcore_prsg* bcore_prsg_lcg_u3_00_s_set_state_u3( bcore_prsg_lcg_u3_00_s* o, u3_t seed ){ o->state = seed; return  (bcore_prsg*)o;} \
  static inline void bcore_prsg_lcg_u3_00_s_gen( bcore_prsg_lcg_u3_00_s* o ){ o->state = bcore_lcg00_u3( o->state );}
#define TYPEOF_bcore_prsg_lcg_u3_01_s 0xEF4A66A708DBF885ull
#define BETH_EXPAND_ITEM_bcore_prsg_lcg_u3_01_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_lcg_u3_01_s ) \
  { \
      aware_t _; \
      u3_t state; \
  }; \
  u3_t bcore_prsg_lcg_u3_01_s_gen_u3( bcore_prsg_lcg_u3_01_s* o ); \
  u3_t bcore_prsg_lcg_u3_01_s_state_bits_u3( const bcore_prsg_lcg_u3_01_s* o, sz_t bits ); \
  u3_t bcore_prsg_lcg_u3_01_s_gen_bits_u3( bcore_prsg_lcg_u3_01_s* o, sz_t bits ); \
  f3_t bcore_prsg_lcg_u3_01_s_state_f3( const bcore_prsg_lcg_u3_01_s* o, f3_t min, f3_t max ); \
  f3_t bcore_prsg_lcg_u3_01_s_gen_f3( bcore_prsg_lcg_u3_01_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_lcg_u3_01_s_bits( const bcore_prsg_lcg_u3_01_s* o ); \
  static inline u3_t bcore_prsg_lcg_u3_01_s_max_u3( const bcore_prsg_lcg_u3_01_s* o ); \
  static inline u3_t bcore_prsg_lcg_u3_01_s_min_u3( const bcore_prsg_lcg_u3_01_s* o ); \
  static inline u3_t bcore_prsg_lcg_u3_01_s_state_u3( const bcore_prsg_lcg_u3_01_s* o ); \
  static inline bcore_prsg* bcore_prsg_lcg_u3_01_s_set_state_u3( bcore_prsg_lcg_u3_01_s* o, u3_t seed ); \
  static inline void bcore_prsg_lcg_u3_01_s_gen( bcore_prsg_lcg_u3_01_s* o ); \
  static inline sz_t bcore_prsg_lcg_u3_01_s_bits( const bcore_prsg_lcg_u3_01_s* o ){ return  64;} \
  static inline u3_t bcore_prsg_lcg_u3_01_s_max_u3( const bcore_prsg_lcg_u3_01_s* o ){ return  0xFFFFFFFFFFFFFFFFull;} \
  static inline u3_t bcore_prsg_lcg_u3_01_s_min_u3( const bcore_prsg_lcg_u3_01_s* o ){ return  0;} \
  static inline u3_t bcore_prsg_lcg_u3_01_s_state_u3( const bcore_prsg_lcg_u3_01_s* o ){ return  o->state;} \
  static inline bcore_prsg* bcore_prsg_lcg_u3_01_s_set_state_u3( bcore_prsg_lcg_u3_01_s* o, u3_t seed ){ o->state = seed; return  (bcore_prsg*)o;} \
  static inline void bcore_prsg_lcg_u3_01_s_gen( bcore_prsg_lcg_u3_01_s* o ){ o->state = bcore_lcg01_u3( o->state );}
#define TYPEOF_bcore_prsg_lcg_u3_02_s 0xF77297A70D55FE88ull
#define BETH_EXPAND_ITEM_bcore_prsg_lcg_u3_02_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_lcg_u3_02_s ) \
  { \
      aware_t _; \
      u3_t state; \
  }; \
  u3_t bcore_prsg_lcg_u3_02_s_gen_u3( bcore_prsg_lcg_u3_02_s* o ); \
  u3_t bcore_prsg_lcg_u3_02_s_state_bits_u3( const bcore_prsg_lcg_u3_02_s* o, sz_t bits ); \
  u3_t bcore_prsg_lcg_u3_02_s_gen_bits_u3( bcore_prsg_lcg_u3_02_s* o, sz_t bits ); \
  f3_t bcore_prsg_lcg_u3_02_s_state_f3( const bcore_prsg_lcg_u3_02_s* o, f3_t min, f3_t max ); \
  f3_t bcore_prsg_lcg_u3_02_s_gen_f3( bcore_prsg_lcg_u3_02_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_lcg_u3_02_s_bits( const bcore_prsg_lcg_u3_02_s* o ); \
  static inline u3_t bcore_prsg_lcg_u3_02_s_max_u3( const bcore_prsg_lcg_u3_02_s* o ); \
  static inline u3_t bcore_prsg_lcg_u3_02_s_min_u3( const bcore_prsg_lcg_u3_02_s* o ); \
  static inline u3_t bcore_prsg_lcg_u3_02_s_state_u3( const bcore_prsg_lcg_u3_02_s* o ); \
  static inline bcore_prsg* bcore_prsg_lcg_u3_02_s_set_state_u3( bcore_prsg_lcg_u3_02_s* o, u3_t seed ); \
  static inline void bcore_prsg_lcg_u3_02_s_gen( bcore_prsg_lcg_u3_02_s* o ); \
  static inline sz_t bcore_prsg_lcg_u3_02_s_bits( const bcore_prsg_lcg_u3_02_s* o ){ return  64;} \
  static inline u3_t bcore_prsg_lcg_u3_02_s_max_u3( const bcore_prsg_lcg_u3_02_s* o ){ return  0xFFFFFFFFFFFFFFFFull;} \
  static inline u3_t bcore_prsg_lcg_u3_02_s_min_u3( const bcore_prsg_lcg_u3_02_s* o ){ return  0;} \
  static inline u3_t bcore_prsg_lcg_u3_02_s_state_u3( const bcore_prsg_lcg_u3_02_s* o ){ return  o->state;} \
  static inline bcore_prsg* bcore_prsg_lcg_u3_02_s_set_state_u3( bcore_prsg_lcg_u3_02_s* o, u3_t seed ){ o->state = seed; return  (bcore_prsg*)o;} \
  static inline void bcore_prsg_lcg_u3_02_s_gen( bcore_prsg_lcg_u3_02_s* o ){ o->state = bcore_lcg02_u3( o->state );}
#define TYPEOF_bcore_prsg_lcg_u3_03_s 0x0227B0A713FB1403ull
#define BETH_EXPAND_ITEM_bcore_prsg_lcg_u3_03_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_lcg_u3_03_s ) \
  { \
      aware_t _; \
      u3_t state; \
  }; \
  u3_t bcore_prsg_lcg_u3_03_s_gen_u3( bcore_prsg_lcg_u3_03_s* o ); \
  u3_t bcore_prsg_lcg_u3_03_s_state_bits_u3( const bcore_prsg_lcg_u3_03_s* o, sz_t bits ); \
  u3_t bcore_prsg_lcg_u3_03_s_gen_bits_u3( bcore_prsg_lcg_u3_03_s* o, sz_t bits ); \
  f3_t bcore_prsg_lcg_u3_03_s_state_f3( const bcore_prsg_lcg_u3_03_s* o, f3_t min, f3_t max ); \
  f3_t bcore_prsg_lcg_u3_03_s_gen_f3( bcore_prsg_lcg_u3_03_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_lcg_u3_03_s_bits( const bcore_prsg_lcg_u3_03_s* o ); \
  static inline u3_t bcore_prsg_lcg_u3_03_s_max_u3( const bcore_prsg_lcg_u3_03_s* o ); \
  static inline u3_t bcore_prsg_lcg_u3_03_s_min_u3( const bcore_prsg_lcg_u3_03_s* o ); \
  static inline u3_t bcore_prsg_lcg_u3_03_s_state_u3( const bcore_prsg_lcg_u3_03_s* o ); \
  static inline bcore_prsg* bcore_prsg_lcg_u3_03_s_set_state_u3( bcore_prsg_lcg_u3_03_s* o, u3_t seed ); \
  static inline void bcore_prsg_lcg_u3_03_s_gen( bcore_prsg_lcg_u3_03_s* o ); \
  static inline sz_t bcore_prsg_lcg_u3_03_s_bits( const bcore_prsg_lcg_u3_03_s* o ){ return  64;} \
  static inline u3_t bcore_prsg_lcg_u3_03_s_max_u3( const bcore_prsg_lcg_u3_03_s* o ){ return  0xFFFFFFFFFFFFFFFFull;} \
  static inline u3_t bcore_prsg_lcg_u3_03_s_min_u3( const bcore_prsg_lcg_u3_03_s* o ){ return  0;} \
  static inline u3_t bcore_prsg_lcg_u3_03_s_state_u3( const bcore_prsg_lcg_u3_03_s* o ){ return  o->state;} \
  static inline bcore_prsg* bcore_prsg_lcg_u3_03_s_set_state_u3( bcore_prsg_lcg_u3_03_s* o, u3_t seed ){ o->state = seed; return  (bcore_prsg*)o;} \
  static inline void bcore_prsg_lcg_u3_03_s_gen( bcore_prsg_lcg_u3_03_s* o ){ o->state = bcore_lcg03_u3( o->state );}
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
  u3_t bcore_prsg_xsg_u2_00_s_gen_u3( bcore_prsg_xsg_u2_00_s* o ); \
  u3_t bcore_prsg_xsg_u2_00_s_state_bits_u3( const bcore_prsg_xsg_u2_00_s* o, sz_t bits ); \
  u3_t bcore_prsg_xsg_u2_00_s_gen_bits_u3( bcore_prsg_xsg_u2_00_s* o, sz_t bits ); \
  f3_t bcore_prsg_xsg_u2_00_s_state_f3( const bcore_prsg_xsg_u2_00_s* o, f3_t min, f3_t max ); \
  f3_t bcore_prsg_xsg_u2_00_s_gen_f3( bcore_prsg_xsg_u2_00_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_xsg_u2_00_s_bits( const bcore_prsg_xsg_u2_00_s* o ); \
  static inline u3_t bcore_prsg_xsg_u2_00_s_max_u3( const bcore_prsg_xsg_u2_00_s* o ); \
  static inline u3_t bcore_prsg_xsg_u2_00_s_min_u3( const bcore_prsg_xsg_u2_00_s* o ); \
  static inline u3_t bcore_prsg_xsg_u2_00_s_state_u3( const bcore_prsg_xsg_u2_00_s* o ); \
  static inline bcore_prsg* bcore_prsg_xsg_u2_00_s_set_state_u3( bcore_prsg_xsg_u2_00_s* o, u3_t seed ); \
  static inline void bcore_prsg_xsg_u2_00_s_gen( bcore_prsg_xsg_u2_00_s* o ); \
  static inline sz_t bcore_prsg_xsg_u2_00_s_bits( const bcore_prsg_xsg_u2_00_s* o ){ return  32;} \
  static inline u3_t bcore_prsg_xsg_u2_00_s_max_u3( const bcore_prsg_xsg_u2_00_s* o ){ return  0xFFFFFFFFu;} \
  static inline u3_t bcore_prsg_xsg_u2_00_s_min_u3( const bcore_prsg_xsg_u2_00_s* o ){ return  1;} \
  static inline u3_t bcore_prsg_xsg_u2_00_s_state_u3( const bcore_prsg_xsg_u2_00_s* o ){ return  o->state;} \
  static inline bcore_prsg* bcore_prsg_xsg_u2_00_s_set_state_u3( bcore_prsg_xsg_u2_00_s* o, u3_t seed ){ o->state = u2_max( 1, seed ^ ( seed >> 32 ) ); return  (bcore_prsg*)o;} \
  static inline void bcore_prsg_xsg_u2_00_s_gen( bcore_prsg_xsg_u2_00_s* o ){ o->state = bcore_xsg_u2(  o->state );}
#define TYPEOF_bcore_prsg_xsg_u2_01_s 0x5C1EA9147A30EFD4ull
#define BETH_EXPAND_ITEM_bcore_prsg_xsg_u2_01_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_xsg_u2_01_s ) \
  { \
      aware_t _; \
      u2_t state; \
  }; \
  u3_t bcore_prsg_xsg_u2_01_s_gen_u3( bcore_prsg_xsg_u2_01_s* o ); \
  u3_t bcore_prsg_xsg_u2_01_s_state_bits_u3( const bcore_prsg_xsg_u2_01_s* o, sz_t bits ); \
  u3_t bcore_prsg_xsg_u2_01_s_gen_bits_u3( bcore_prsg_xsg_u2_01_s* o, sz_t bits ); \
  f3_t bcore_prsg_xsg_u2_01_s_state_f3( const bcore_prsg_xsg_u2_01_s* o, f3_t min, f3_t max ); \
  f3_t bcore_prsg_xsg_u2_01_s_gen_f3( bcore_prsg_xsg_u2_01_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_xsg_u2_01_s_bits( const bcore_prsg_xsg_u2_01_s* o ); \
  static inline u3_t bcore_prsg_xsg_u2_01_s_max_u3( const bcore_prsg_xsg_u2_01_s* o ); \
  static inline u3_t bcore_prsg_xsg_u2_01_s_min_u3( const bcore_prsg_xsg_u2_01_s* o ); \
  static inline u3_t bcore_prsg_xsg_u2_01_s_state_u3( const bcore_prsg_xsg_u2_01_s* o ); \
  static inline bcore_prsg* bcore_prsg_xsg_u2_01_s_set_state_u3( bcore_prsg_xsg_u2_01_s* o, u3_t seed ); \
  static inline void bcore_prsg_xsg_u2_01_s_gen( bcore_prsg_xsg_u2_01_s* o ); \
  static inline sz_t bcore_prsg_xsg_u2_01_s_bits( const bcore_prsg_xsg_u2_01_s* o ){ return  32;} \
  static inline u3_t bcore_prsg_xsg_u2_01_s_max_u3( const bcore_prsg_xsg_u2_01_s* o ){ return  0xFFFFFFFFu;} \
  static inline u3_t bcore_prsg_xsg_u2_01_s_min_u3( const bcore_prsg_xsg_u2_01_s* o ){ return  1;} \
  static inline u3_t bcore_prsg_xsg_u2_01_s_state_u3( const bcore_prsg_xsg_u2_01_s* o ){ return  o->state;} \
  static inline bcore_prsg* bcore_prsg_xsg_u2_01_s_set_state_u3( bcore_prsg_xsg_u2_01_s* o, u3_t seed ){ o->state = u2_max( 1, seed ^ ( seed >> 32 ) ); return  (bcore_prsg*)o;} \
  static inline void bcore_prsg_xsg_u2_01_s_gen( bcore_prsg_xsg_u2_01_s* o ){ o->state = bcore_xsg1_u2( o->state );}
#define TYPEOF_bcore_prsg_xsg_u2_02_s 0x54CF7814766EDA91ull
#define BETH_EXPAND_ITEM_bcore_prsg_xsg_u2_02_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_xsg_u2_02_s ) \
  { \
      aware_t _; \
      u2_t state; \
  }; \
  u3_t bcore_prsg_xsg_u2_02_s_gen_u3( bcore_prsg_xsg_u2_02_s* o ); \
  u3_t bcore_prsg_xsg_u2_02_s_state_bits_u3( const bcore_prsg_xsg_u2_02_s* o, sz_t bits ); \
  u3_t bcore_prsg_xsg_u2_02_s_gen_bits_u3( bcore_prsg_xsg_u2_02_s* o, sz_t bits ); \
  f3_t bcore_prsg_xsg_u2_02_s_state_f3( const bcore_prsg_xsg_u2_02_s* o, f3_t min, f3_t max ); \
  f3_t bcore_prsg_xsg_u2_02_s_gen_f3( bcore_prsg_xsg_u2_02_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_xsg_u2_02_s_bits( const bcore_prsg_xsg_u2_02_s* o ); \
  static inline u3_t bcore_prsg_xsg_u2_02_s_max_u3( const bcore_prsg_xsg_u2_02_s* o ); \
  static inline u3_t bcore_prsg_xsg_u2_02_s_min_u3( const bcore_prsg_xsg_u2_02_s* o ); \
  static inline u3_t bcore_prsg_xsg_u2_02_s_state_u3( const bcore_prsg_xsg_u2_02_s* o ); \
  static inline bcore_prsg* bcore_prsg_xsg_u2_02_s_set_state_u3( bcore_prsg_xsg_u2_02_s* o, u3_t seed ); \
  static inline void bcore_prsg_xsg_u2_02_s_gen( bcore_prsg_xsg_u2_02_s* o ); \
  static inline sz_t bcore_prsg_xsg_u2_02_s_bits( const bcore_prsg_xsg_u2_02_s* o ){ return  32;} \
  static inline u3_t bcore_prsg_xsg_u2_02_s_max_u3( const bcore_prsg_xsg_u2_02_s* o ){ return  0xFFFFFFFFu;} \
  static inline u3_t bcore_prsg_xsg_u2_02_s_min_u3( const bcore_prsg_xsg_u2_02_s* o ){ return  1;} \
  static inline u3_t bcore_prsg_xsg_u2_02_s_state_u3( const bcore_prsg_xsg_u2_02_s* o ){ return  o->state;} \
  static inline bcore_prsg* bcore_prsg_xsg_u2_02_s_set_state_u3( bcore_prsg_xsg_u2_02_s* o, u3_t seed ){ o->state = u2_max( 1, seed ^ ( seed >> 32 ) ); return  (bcore_prsg*)o;} \
  static inline void bcore_prsg_xsg_u2_02_s_gen( bcore_prsg_xsg_u2_02_s* o ){ o->state = bcore_xsg2_u2( o->state );}
#define TYPEOF_bcore_prsg_xsg_u2_03_s 0x4AF43F1470833276ull
#define BETH_EXPAND_ITEM_bcore_prsg_xsg_u2_03_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_xsg_u2_03_s ) \
  { \
      aware_t _; \
      u2_t state; \
  }; \
  u3_t bcore_prsg_xsg_u2_03_s_gen_u3( bcore_prsg_xsg_u2_03_s* o ); \
  u3_t bcore_prsg_xsg_u2_03_s_state_bits_u3( const bcore_prsg_xsg_u2_03_s* o, sz_t bits ); \
  u3_t bcore_prsg_xsg_u2_03_s_gen_bits_u3( bcore_prsg_xsg_u2_03_s* o, sz_t bits ); \
  f3_t bcore_prsg_xsg_u2_03_s_state_f3( const bcore_prsg_xsg_u2_03_s* o, f3_t min, f3_t max ); \
  f3_t bcore_prsg_xsg_u2_03_s_gen_f3( bcore_prsg_xsg_u2_03_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_xsg_u2_03_s_bits( const bcore_prsg_xsg_u2_03_s* o ); \
  static inline u3_t bcore_prsg_xsg_u2_03_s_max_u3( const bcore_prsg_xsg_u2_03_s* o ); \
  static inline u3_t bcore_prsg_xsg_u2_03_s_min_u3( const bcore_prsg_xsg_u2_03_s* o ); \
  static inline u3_t bcore_prsg_xsg_u2_03_s_state_u3( const bcore_prsg_xsg_u2_03_s* o ); \
  static inline bcore_prsg* bcore_prsg_xsg_u2_03_s_set_state_u3( bcore_prsg_xsg_u2_03_s* o, u3_t seed ); \
  static inline void bcore_prsg_xsg_u2_03_s_gen( bcore_prsg_xsg_u2_03_s* o ); \
  static inline sz_t bcore_prsg_xsg_u2_03_s_bits( const bcore_prsg_xsg_u2_03_s* o ){ return  32;} \
  static inline u3_t bcore_prsg_xsg_u2_03_s_max_u3( const bcore_prsg_xsg_u2_03_s* o ){ return  0xFFFFFFFFu;} \
  static inline u3_t bcore_prsg_xsg_u2_03_s_min_u3( const bcore_prsg_xsg_u2_03_s* o ){ return  1;} \
  static inline u3_t bcore_prsg_xsg_u2_03_s_state_u3( const bcore_prsg_xsg_u2_03_s* o ){ return  o->state;} \
  static inline bcore_prsg* bcore_prsg_xsg_u2_03_s_set_state_u3( bcore_prsg_xsg_u2_03_s* o, u3_t seed ){ o->state = u2_max( 1, seed ^ ( seed >> 32 ) ); return  (bcore_prsg*)o;} \
  static inline void bcore_prsg_xsg_u2_03_s_gen( bcore_prsg_xsg_u2_03_s* o ){ o->state = bcore_xsg3_u2( o->state );}
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

vd_t bcore_xo_signal_handler( const bcore_signal_s* o );

#endif // __bcore_xo_H
// XOILA_OUT_SIGNATURE 0x822F464C4CD7F227ull
