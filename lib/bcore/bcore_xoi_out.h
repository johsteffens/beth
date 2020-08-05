/** This file was generated from beth-plant source code.
 *  Compiling Agent : xoico_compiler (C) 2020 J.B.Steffens
 *  Last File Update: 2020-08-05T16:32:22Z
 *
 *  Copyright and License of this File:
 *
 *  Generated code inherits the copyright and license of the underlying beth-plant source code.
 *  Source code defining this file is distributed across following files:
 *
 *  bcore_file.h
 *  bcore_xoila_inexpandable.h
 *  bcore_xoila_sample.h
 *  bcore_spect_inst_call.h
 *  bcore_spect_via_call.h
 *  bcore_main.h
 *  bcore_hmap_name.h
 *  bcore_cday.h
 *  bcore_error_manager.h
 *  bcore_prsg.h
 *
 */

#ifndef BCORE_XOI_OUT_H
#define BCORE_XOI_OUT_H

#include "bcore_control.h"

//To force a rebuild of this target by the plant-compiler, reset the hash key value below to 0.
#define HKEYOF_bcore_xoi_out 3791754814

#define TYPEOF_bcore_xoi_out 1237696822

/**********************************************************************************************************************/
// source: bcore_file.h

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_file

#define TYPEOF_bcore_file 2537091085
#define TYPEOF_bcore_file_s 2223383039
#define TYPEOF_bcore_file_path_s 691386159
#define BETH_EXPAND_ITEM_bcore_file_path_s \
  BCORE_DECLARE_OBJECT( bcore_file_path_s ) \
    {aware_t _;st_s name;st_s* root;st_s* full;}; \
  void bcore_file_path_s_source( bcore_file_path_s* o, bcore_source* source ); \
  static inline sc_t bcore_file_path_s_get_sc( const bcore_file_path_s* o ){return o->full ? o->full->sc : o->name.sc;} \
  void bcore_file_path_s_set_sc( bcore_file_path_s* o, sc_t name );
#define BETH_EXPAND_GROUP_bcore_file \
  BCORE_FORWARD_OBJECT( bcore_file ); \
  BCORE_FORWARD_OBJECT( bcore_file_path_s ); \
  BETH_EXPAND_ITEM_bcore_file_path_s

/**********************************************************************************************************************/
// source: bcore_xoila_inexpandable.h

/**********************************************************************************************************************/
// source: bcore_xoila_sample.h

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_xoila_sample

#define TYPEOF_bcore_xoila_sample 3192213229
#define TYPEOF_bcore_xoila_sample_s 710029983
#define TYPEOF_anyglobalname 2209051205
#define TYPEOF_bcore_xoila_sample_object2_s 3045603879
#define BETH_EXPAND_ITEM_bcore_xoila_sample_object2_s \
  BCORE_DECLARE_OBJECT( bcore_xoila_sample_object2_s ) \
    {aware_t _;bcore_xoila_sample_features_object_s object;st_s name2;}; \
  void bcore_xoila_sample_object2_s_setup( bcore_xoila_sample_object2_s* o, sz_t n ); \
  void bcore_xoila_sample_object2_s_to_stdout( const bcore_xoila_sample_object2_s* o );
#define BETH_EXPAND_GROUP_bcore_xoila_sample \
  BCORE_FORWARD_OBJECT( bcore_xoila_sample ); \
  BCORE_FORWARD_OBJECT( bcore_xoila_sample_signatures ); \
  BCORE_FORWARD_OBJECT( bcore_xoila_sample_features ); \
  BCORE_FORWARD_OBJECT( bcore_xoila_sample_object2_s ); \
  BETH_EXPAND_GROUP_bcore_xoila_sample_signatures \
  BETH_EXPAND_GROUP_bcore_xoila_sample_features \
  BETH_EXPAND_ITEM_bcore_xoila_sample_object2_s

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_xoila_sample_signatures

#define TYPEOF_bcore_xoila_sample_signatures 2786267517
#define TYPEOF_bcore_xoila_sample_signatures_s 3510692271
#define BETH_EXPAND_GROUP_bcore_xoila_sample_signatures \
  BCORE_FORWARD_OBJECT( bcore_xoila_sample_signatures );

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_xoila_sample_features

#define TYPEOF_bcore_xoila_sample_features 2734102561
#define TYPEOF_bcore_xoila_sample_features_s 1291897483
#define TYPEOF_bcore_xoila_sample_features_object_s 1878847515
#define BETH_EXPAND_ITEM_bcore_xoila_sample_features_object_s \
  BCORE_DECLARE_OBJECT( bcore_xoila_sample_features_object_s ) \
    {aware_t _;st_s name1;BCORE_ARRAY_DYN_LINK_STATIC_S( sz_t, arr_ );}; \
  void bcore_xoila_sample_features_object_s_setup( bcore_xoila_sample_features_object_s* o, sz_t n ); \
  static inline void bcore_xoila_sample_features_object_s_to_stdout( const bcore_xoila_sample_features_object_s* o ){bcore_txt_ml_a_to_stdout( o );} \
  sz_t bcore_xoila_sample_features_object_s_get_size( const bcore_xoila_sample_features_object_s* o ); \
  static inline void bcore_xoila_sample_features_object_s_simple_function( bcore_xoila_sample_features_object_s* o, sz_t n ){/* do nothing */}
#define BETH_EXPAND_GROUP_bcore_xoila_sample_features \
  BCORE_FORWARD_OBJECT( bcore_xoila_sample_features ); \
  BCORE_FORWARD_OBJECT( bcore_xoila_sample_features_object_s ); \
  typedef void (*bcore_xoila_sample_features_setup)( bcore_xoila_sample_features* o, sz_t n ); \
  typedef void (*bcore_xoila_sample_features_to_stdout)( const bcore_xoila_sample_features* o ); \
  typedef sz_t (*bcore_xoila_sample_features_get_size)( const bcore_xoila_sample_features* o ); \
  BCORE_DECLARE_SPECT( bcore_xoila_sample_features ) \
  { \
      bcore_spect_header_s header; \
      bcore_xoila_sample_features_setup setup; \
      bcore_xoila_sample_features_to_stdout to_stdout; \
      bcore_xoila_sample_features_get_size get_size; \
  }; \
  static inline bcore_xoila_sample_features* bcore_xoila_sample_features_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_bcore_xoila_sample_features, t ); return ( bcore_xoila_sample_features* )bcore_inst_t_create( t ); } \
  static inline bl_t bcore_xoila_sample_features_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_bcore_xoila_sample_features ); } \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bcore_xoila_sample_features ) \
  static inline bl_t bcore_xoila_sample_features_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_bcore_xoila_sample_features ); } \
  static inline void bcore_xoila_sample_features_p_setup( const bcore_xoila_sample_features_s* __p, bcore_xoila_sample_features* o, sz_t n ) { assert( __p->setup ); __p->setup( o, n ); } \
  static inline void bcore_xoila_sample_features_t_setup( tp_t __t, bcore_xoila_sample_features* o, sz_t n ) { const bcore_xoila_sample_features_s* p = bcore_xoila_sample_features_s_get_typed( __t ); assert( p->setup ); p->setup( o, n ); } \
  static inline void bcore_xoila_sample_features_a_setup( bcore_xoila_sample_features* o, sz_t n ) { const bcore_xoila_sample_features_s* p = bcore_xoila_sample_features_s_get_aware( o ); assert( p->setup ); p->setup( o, n ); } \
  static inline void bcore_xoila_sample_features_r_setup( const sr_s* o, sz_t n ) { ASSERT( !sr_s_is_const( o ) ); const bcore_xoila_sample_features_s* p = (const bcore_xoila_sample_features_s*)ch_spect_p( o->p, TYPEOF_bcore_xoila_sample_features_s ); assert( p->setup ); p->setup( o->o, n ); } \
  static inline bl_t bcore_xoila_sample_features_p_defines_setup( const bcore_xoila_sample_features_s* __p ) { return true; } \
  static inline bl_t bcore_xoila_sample_features_t_defines_setup( tp_t __t ) { return true; } \
  static inline bl_t bcore_xoila_sample_features_a_defines_setup( const bcore_xoila_sample_features* o ) { return true; } \
  static inline bl_t bcore_xoila_sample_features_r_defines_setup( const sr_s* o ) { return true; } \
  static inline void bcore_xoila_sample_features_p_to_stdout( const bcore_xoila_sample_features_s* __p, const bcore_xoila_sample_features* o ) { assert( __p->to_stdout ); __p->to_stdout( o ); } \
  static inline void bcore_xoila_sample_features_t_to_stdout( tp_t __t, const bcore_xoila_sample_features* o ) { const bcore_xoila_sample_features_s* p = bcore_xoila_sample_features_s_get_typed( __t ); assert( p->to_stdout ); p->to_stdout( o ); } \
  static inline void bcore_xoila_sample_features_a_to_stdout( const bcore_xoila_sample_features* o ) { const bcore_xoila_sample_features_s* p = bcore_xoila_sample_features_s_get_aware( o ); assert( p->to_stdout ); p->to_stdout( o ); } \
  static inline void bcore_xoila_sample_features_r_to_stdout( const sr_s* o ) { const bcore_xoila_sample_features_s* p = (const bcore_xoila_sample_features_s*)ch_spect_p( o->p, TYPEOF_bcore_xoila_sample_features_s ); assert( p->to_stdout ); p->to_stdout( o->o ); } \
  static inline bl_t bcore_xoila_sample_features_p_defines_to_stdout( const bcore_xoila_sample_features_s* __p ) { return true; } \
  static inline bl_t bcore_xoila_sample_features_t_defines_to_stdout( tp_t __t ) { return true; } \
  static inline bl_t bcore_xoila_sample_features_a_defines_to_stdout( const bcore_xoila_sample_features* o ) { return true; } \
  static inline bl_t bcore_xoila_sample_features_r_defines_to_stdout( const sr_s* o ) { return true; } \
  void bcore_xoila_sample_features_to_stdout__( const bcore_xoila_sample_features* o ); \
  static inline sz_t bcore_xoila_sample_features_p_get_size( const bcore_xoila_sample_features_s* __p, const bcore_xoila_sample_features* o ) { assert( __p->get_size ); return __p->get_size( o ); } \
  static inline sz_t bcore_xoila_sample_features_t_get_size( tp_t __t, const bcore_xoila_sample_features* o ) { const bcore_xoila_sample_features_s* p = bcore_xoila_sample_features_s_get_typed( __t ); assert( p->get_size ); return p->get_size( o ); } \
  static inline sz_t bcore_xoila_sample_features_a_get_size( const bcore_xoila_sample_features* o ) { const bcore_xoila_sample_features_s* p = bcore_xoila_sample_features_s_get_aware( o ); assert( p->get_size ); return p->get_size( o ); } \
  static inline sz_t bcore_xoila_sample_features_r_get_size( const sr_s* o ) { const bcore_xoila_sample_features_s* p = (const bcore_xoila_sample_features_s*)ch_spect_p( o->p, TYPEOF_bcore_xoila_sample_features_s ); assert( p->get_size ); return p->get_size( o->o ); } \
  static inline bl_t bcore_xoila_sample_features_p_defines_get_size( const bcore_xoila_sample_features_s* __p ) { return __p->get_size != NULL; } \
  static inline bl_t bcore_xoila_sample_features_t_defines_get_size( tp_t __t ) { return bcore_xoila_sample_features_s_get_typed( __t )->get_size != NULL; } \
  static inline bl_t bcore_xoila_sample_features_a_defines_get_size( const bcore_xoila_sample_features* o ) { return bcore_xoila_sample_features_s_get_aware( o )->get_size != NULL; } \
  static inline bl_t bcore_xoila_sample_features_r_defines_get_size( const sr_s* o ) { return ( (bcore_xoila_sample_features_s*)ch_spect_p( o->p, TYPEOF_bcore_xoila_sample_features_s ) )->get_size != NULL; } \
  BETH_EXPAND_ITEM_bcore_xoila_sample_features_object_s

/**********************************************************************************************************************/
// source: bcore_spect_inst_call.h

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_inst_call

#define TYPEOF_bcore_inst_call 2782316552
#define TYPEOF_bcore_inst_call_s 1816064754
#define TYPEOF_bcore_inst_call_init_x 622928068
#define TYPEOF_bcore_inst_call_down_e 3705510577
#define TYPEOF_bcore_inst_call_copy_e 1302178876
#define TYPEOF_bcore_inst_call_copy_x 1117625067
#define TYPEOF_bcore_inst_call_discard_e 1806316605
#define BETH_EXPAND_GROUP_bcore_inst_call \
  BCORE_FORWARD_OBJECT( bcore_inst_call ); \
  typedef void (*bcore_inst_call_init_x)( bcore_inst_call* o ); \
  typedef void (*bcore_inst_call_down_e)( bcore_inst_call* o ); \
  typedef void (*bcore_inst_call_copy_e)( bcore_inst_call* o ); \
  typedef void (*bcore_inst_call_copy_x)( bcore_inst_call* o ); \
  typedef void (*bcore_inst_call_discard_e)( bcore_inst_call* o ); \
  BCORE_DECLARE_SPECT( bcore_inst_call ) \
  { \
      bcore_spect_header_s header; \
      bcore_inst_call_init_x init_x; \
      bcore_inst_call_down_e down_e; \
      bcore_inst_call_copy_e copy_e; \
      bcore_inst_call_copy_x copy_x; \
      bcore_inst_call_discard_e discard_e; \
  }; \
  static inline bcore_inst_call* bcore_inst_call_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_bcore_inst_call, t ); return ( bcore_inst_call* )bcore_inst_t_create( t ); } \
  static inline bl_t bcore_inst_call_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_bcore_inst_call ); } \
  static inline void bcore_inst_call_p_init_x( const bcore_inst_call_s* __p, bcore_inst_call* o ) { assert( __p->init_x ); __p->init_x( o ); } \
  static inline bl_t bcore_inst_call_p_defines_init_x( const bcore_inst_call_s* __p ) { return __p->init_x != NULL; } \
  static inline void bcore_inst_call_p_down_e( const bcore_inst_call_s* __p, bcore_inst_call* o ) { assert( __p->down_e ); __p->down_e( o ); } \
  static inline bl_t bcore_inst_call_p_defines_down_e( const bcore_inst_call_s* __p ) { return __p->down_e != NULL; } \
  static inline void bcore_inst_call_p_copy_e( const bcore_inst_call_s* __p, bcore_inst_call* o ) { assert( __p->copy_e ); __p->copy_e( o ); } \
  static inline bl_t bcore_inst_call_p_defines_copy_e( const bcore_inst_call_s* __p ) { return __p->copy_e != NULL; } \
  static inline void bcore_inst_call_p_copy_x( const bcore_inst_call_s* __p, bcore_inst_call* o ) { assert( __p->copy_x ); __p->copy_x( o ); } \
  static inline bl_t bcore_inst_call_p_defines_copy_x( const bcore_inst_call_s* __p ) { return __p->copy_x != NULL; } \
  static inline void bcore_inst_call_p_discard_e( const bcore_inst_call_s* __p, bcore_inst_call* o ) { assert( __p->discard_e ); __p->discard_e( o ); } \
  static inline bl_t bcore_inst_call_p_defines_discard_e( const bcore_inst_call_s* __p ) { return __p->discard_e != NULL; }

/**********************************************************************************************************************/
// source: bcore_spect_via_call.h

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_via_call

#define TYPEOF_bcore_via_call 3918678614
#define TYPEOF_bcore_via_call_s 2606427784
#define BETH_EXPAND_GROUP_bcore_via_call \
  BCORE_FORWARD_OBJECT( bcore_via_call ); \
  typedef void (*bcore_via_call_source)( bcore_via_call* o, bcore_source* source ); \
  typedef void (*bcore_via_call_mutated)( bcore_via_call* o ); \
  typedef void (*bcore_via_call_shelve)( bcore_via_call* o ); \
  BCORE_DECLARE_SPECT( bcore_via_call ) \
  { \
      bcore_spect_header_s header; \
      bcore_via_call_source source; \
      bcore_via_call_mutated mutated; \
      bcore_via_call_shelve shelve; \
  }; \
  static inline bcore_via_call* bcore_via_call_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_bcore_via_call, t ); return ( bcore_via_call* )bcore_inst_t_create( t ); } \
  static inline bl_t bcore_via_call_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_bcore_via_call ); } \
  static inline void bcore_via_call_p_source( const bcore_via_call_s* __p, bcore_via_call* o, bcore_source* source ) { assert( __p->source ); __p->source( o, source ); } \
  static inline void bcore_via_call_t_source( tp_t __t, bcore_via_call* o, bcore_source* source ) { const bcore_via_call_s* p = bcore_via_call_s_get_typed( __t ); assert( p->source ); p->source( o, source ); } \
  static inline void bcore_via_call_r_source( const sr_s* o, bcore_source* source ) { ASSERT( !sr_s_is_const( o ) ); const bcore_via_call_s* p = (const bcore_via_call_s*)ch_spect_p( o->p, TYPEOF_bcore_via_call_s ); assert( p->source ); p->source( o->o, source ); } \
  static inline bl_t bcore_via_call_p_defines_source( const bcore_via_call_s* __p ) { return __p->source != NULL; } \
  static inline bl_t bcore_via_call_t_defines_source( tp_t __t ) { return bcore_via_call_s_get_typed( __t )->source != NULL; } \
  static inline bl_t bcore_via_call_r_defines_source( const sr_s* o ) { return ( (bcore_via_call_s*)ch_spect_p( o->p, TYPEOF_bcore_via_call_s ) )->source != NULL; } \
  static inline void bcore_via_call_p_mutated( const bcore_via_call_s* __p, bcore_via_call* o ) { assert( __p->mutated ); __p->mutated( o ); } \
  static inline void bcore_via_call_t_mutated( tp_t __t, bcore_via_call* o ) { const bcore_via_call_s* p = bcore_via_call_s_get_typed( __t ); assert( p->mutated ); p->mutated( o ); } \
  static inline void bcore_via_call_r_mutated( const sr_s* o ) { ASSERT( !sr_s_is_const( o ) ); const bcore_via_call_s* p = (const bcore_via_call_s*)ch_spect_p( o->p, TYPEOF_bcore_via_call_s ); assert( p->mutated ); p->mutated( o->o ); } \
  static inline bl_t bcore_via_call_p_defines_mutated( const bcore_via_call_s* __p ) { return __p->mutated != NULL; } \
  static inline bl_t bcore_via_call_t_defines_mutated( tp_t __t ) { return bcore_via_call_s_get_typed( __t )->mutated != NULL; } \
  static inline bl_t bcore_via_call_r_defines_mutated( const sr_s* o ) { return ( (bcore_via_call_s*)ch_spect_p( o->p, TYPEOF_bcore_via_call_s ) )->mutated != NULL; } \
  static inline void bcore_via_call_p_shelve( const bcore_via_call_s* __p, bcore_via_call* o ) { assert( __p->shelve ); __p->shelve( o ); } \
  static inline void bcore_via_call_t_shelve( tp_t __t, bcore_via_call* o ) { const bcore_via_call_s* p = bcore_via_call_s_get_typed( __t ); assert( p->shelve ); p->shelve( o ); } \
  static inline void bcore_via_call_r_shelve( const sr_s* o ) { ASSERT( !sr_s_is_const( o ) ); const bcore_via_call_s* p = (const bcore_via_call_s*)ch_spect_p( o->p, TYPEOF_bcore_via_call_s ); assert( p->shelve ); p->shelve( o->o ); } \
  static inline bl_t bcore_via_call_p_defines_shelve( const bcore_via_call_s* __p ) { return __p->shelve != NULL; } \
  static inline bl_t bcore_via_call_t_defines_shelve( tp_t __t ) { return bcore_via_call_s_get_typed( __t )->shelve != NULL; } \
  static inline bl_t bcore_via_call_r_defines_shelve( const sr_s* o ) { return ( (bcore_via_call_s*)ch_spect_p( o->p, TYPEOF_bcore_via_call_s ) )->shelve != NULL; }

/**********************************************************************************************************************/
// source: bcore_main.h

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_main

#define TYPEOF_bcore_main 117506434
#define TYPEOF_bcore_main_s 3641086716
#define TYPEOF_bcore_main_frame_s 3517913134
#define BETH_EXPAND_ITEM_bcore_main_frame_s \
  BCORE_DECLARE_OBJECT( bcore_main_frame_s ) \
    {aware_t _;bcore_interpreter* interpreter;bcore_arr_st_s args;bcore_mutex_s mutex;bl_t use_first_argument;sc_t local_file;sc_t global_file;}; \
  bl_t bcore_main_frame_s_exit_required( const bcore_main_frame_s* o );
#define TYPEOF_bcore_main_arr_s 2064610792
#define BETH_EXPAND_ITEM_bcore_main_arr_s \
  BCORE_DECLARE_OBJECT( bcore_main_arr_s ) \
    {aware_t _;BCORE_ARRAY_DYN_LINK_STATIC_S( bcore_main, );}; \
  static inline bcore_main_arr_s* bcore_main_arr_s_set_space( bcore_main_arr_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_bcore_main_arr_s, ( bcore_array* )o, size ); return o; } \
  static inline bcore_main_arr_s* bcore_main_arr_s_set_size( bcore_main_arr_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_bcore_main_arr_s, ( bcore_array* )o, size ); return o; } \
  static inline bcore_main_arr_s* bcore_main_arr_s_clear( bcore_main_arr_s* o ) { bcore_array_t_set_space( TYPEOF_bcore_main_arr_s, ( bcore_array* )o, 0 ); return o; } \
  static inline bcore_main* bcore_main_arr_s_push_c( bcore_main_arr_s* o, const bcore_main* v ) { bcore_array_t_push( TYPEOF_bcore_main_arr_s, ( bcore_array* )o, sr_awc( v ) ); return o->data[ o->size - 1 ]; } \
  static inline bcore_main* bcore_main_arr_s_push_d( bcore_main_arr_s* o,       bcore_main* v ) { bcore_array_t_push( TYPEOF_bcore_main_arr_s, ( bcore_array* )o, sr_asd( v ) ); return o->data[ o->size - 1 ]; } \
  static inline bcore_main* bcore_main_arr_s_push_t( bcore_main_arr_s* o, tp_t t ) \
  { \
      bcore_trait_assert_satisfied_type( TYPEOF_bcore_main, t ); \
      bcore_array_t_push( TYPEOF_bcore_main_arr_s, ( bcore_array* )o, sr_t_create( t ) ); \
      return o->data[ o->size - 1 ]; \
  }
#define TYPEOF_bcore_main_set_s 642235823
#define BETH_EXPAND_ITEM_bcore_main_set_s \
  BCORE_DECLARE_OBJECT( bcore_main_set_s ) \
    {aware_t _;bcore_main_arr_s arr;}; \
  er_t bcore_main_set_s_main( bcore_main_set_s* o, bcore_main_frame_s* frame );
#define BETH_EXPAND_GROUP_bcore_main \
  BCORE_FORWARD_OBJECT( bcore_main ); \
  BCORE_FORWARD_OBJECT( bcore_main_frame_s ); \
  BCORE_FORWARD_OBJECT( bcore_main_arr_s ); \
  BCORE_FORWARD_OBJECT( bcore_main_set_s ); \
  typedef er_t (*bcore_main_main)( bcore_main* o, bcore_main_frame_s* frame ); \
  BCORE_DECLARE_SPECT( bcore_main ) \
  { \
      bcore_spect_header_s header; \
      bcore_main_main main; \
  }; \
  static inline bcore_main* bcore_main_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_bcore_main, t ); return ( bcore_main* )bcore_inst_t_create( t ); } \
  static inline bl_t bcore_main_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_bcore_main ); } \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bcore_main ) \
  static inline bl_t bcore_main_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_bcore_main ); } \
  BETH_EXPAND_ITEM_bcore_main_frame_s \
  static inline er_t bcore_main_a_main( bcore_main* o, bcore_main_frame_s* frame ) { const bcore_main_s* p = bcore_main_s_get_aware( o ); assert( p->main ); return p->main( o, frame ); } \
  static inline er_t bcore_main_r_main( const sr_s* o, bcore_main_frame_s* frame ) { ASSERT( !sr_s_is_const( o ) ); const bcore_main_s* p = (const bcore_main_s*)ch_spect_p( o->p, TYPEOF_bcore_main_s ); assert( p->main ); return p->main( o->o, frame ); } \
  static inline bl_t bcore_main_a_defines_main( const bcore_main* o ) { return true; } \
  static inline bl_t bcore_main_r_defines_main( const sr_s* o ) { return true; } \
  BETH_EXPAND_ITEM_bcore_main_arr_s \
  BETH_EXPAND_ITEM_bcore_main_set_s

/**********************************************************************************************************************/
// source: bcore_hmap_name.h

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_hmap_name

#define TYPEOF_bcore_hmap_name 1134505061
#define TYPEOF_bcore_hmap_name_s 2060496135
#define TYPEOF_bcore_hmap_name_s 2060496135
#define BETH_EXPAND_ITEM_bcore_hmap_name_s \
  BCORE_DECLARE_OBJECT( bcore_hmap_name_s ) \
    {aware_t _;bcore_hmap_tp_sr_s map;};
#define BETH_EXPAND_GROUP_bcore_hmap_name \
  BCORE_FORWARD_OBJECT( bcore_hmap_name ); \
  BCORE_FORWARD_OBJECT( bcore_hmap_name_s ); \
  BETH_EXPAND_ITEM_bcore_hmap_name_s

/**********************************************************************************************************************/
// source: bcore_cday.h

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_cday

#define TYPEOF_bcore_cday 383355802
#define TYPEOF_bcore_cday_s 2925571844
#define TYPEOF_bcore_cday_ymd_s 3333433405
#define BETH_EXPAND_ITEM_bcore_cday_ymd_s \
  BCORE_DECLARE_OBJECT( bcore_cday_ymd_s ) \
    {aware_t _;s2_t y;s2_t m;s2_t d;};
#define TYPEOF_bcore_cday_utc_s 772421473
#define BETH_EXPAND_ITEM_bcore_cday_utc_s \
  BCORE_DECLARE_OBJECT( bcore_cday_utc_s ) \
    {aware_t _;s2_t cday;s2_t ms;};
#define BETH_EXPAND_GROUP_bcore_cday \
  BCORE_FORWARD_OBJECT( bcore_cday ); \
  BCORE_FORWARD_OBJECT( bcore_cday_ymd_s ); \
  BCORE_FORWARD_OBJECT( bcore_cday_utc_s ); \
  BETH_EXPAND_ITEM_bcore_cday_ymd_s \
  BETH_EXPAND_ITEM_bcore_cday_utc_s

/**********************************************************************************************************************/
// source: bcore_error_manager.h

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_error_manager

#define TYPEOF_bcore_error_manager 1847905235
#define TYPEOF_bcore_error_manager_s 3358571317
#define TYPEOF_bcore_error_manager_error_s 2883695404
#define BETH_EXPAND_ITEM_bcore_error_manager_error_s \
  BCORE_DECLARE_OBJECT( bcore_error_manager_error_s ) \
    {aware_t _;er_t id;st_s msg;};
#define TYPEOF_bcore_error_manager_error_adl_s 2167477956
#define BETH_EXPAND_ITEM_bcore_error_manager_error_adl_s \
  BCORE_DECLARE_OBJECT( bcore_error_manager_error_adl_s ) \
    {aware_t _;BCORE_ARRAY_DYN_LINK_STATIC_S( bcore_error_manager_error_s, );}; \
  static inline bcore_error_manager_error_adl_s* bcore_error_manager_error_adl_s_set_space( bcore_error_manager_error_adl_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_bcore_error_manager_error_adl_s, ( bcore_array* )o, size ); return o; } \
  static inline bcore_error_manager_error_adl_s* bcore_error_manager_error_adl_s_set_size( bcore_error_manager_error_adl_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_bcore_error_manager_error_adl_s, ( bcore_array* )o, size ); return o; } \
  static inline bcore_error_manager_error_adl_s* bcore_error_manager_error_adl_s_clear( bcore_error_manager_error_adl_s* o ) { bcore_array_t_set_space( TYPEOF_bcore_error_manager_error_adl_s, ( bcore_array* )o, 0 ); return o; } \
  static inline bcore_error_manager_error_s* bcore_error_manager_error_adl_s_push_c( bcore_error_manager_error_adl_s* o, const bcore_error_manager_error_s* v ) { bcore_array_t_push( TYPEOF_bcore_error_manager_error_adl_s, ( bcore_array* )o, sr_twc( TYPEOF_bcore_error_manager_error_s, v ) ); return o->data[ o->size - 1 ]; } \
  static inline bcore_error_manager_error_s* bcore_error_manager_error_adl_s_push_d( bcore_error_manager_error_adl_s* o,       bcore_error_manager_error_s* v ) { bcore_array_t_push( TYPEOF_bcore_error_manager_error_adl_s, ( bcore_array* )o, sr_tsd( TYPEOF_bcore_error_manager_error_s, v ) ); return o->data[ o->size - 1 ]; } \
  static inline bcore_error_manager_error_s* bcore_error_manager_error_adl_s_push( bcore_error_manager_error_adl_s* o ) \
  { \
      bcore_array_t_push( TYPEOF_bcore_error_manager_error_adl_s, ( bcore_array* )o, sr_t_create( TYPEOF_bcore_error_manager_error_s ) ); \
      return o->data[ o->size - 1 ]; \
  }
#define TYPEOF_bcore_error_manager_context_s 269699777
#define BETH_EXPAND_ITEM_bcore_error_manager_context_s \
  BCORE_DECLARE_OBJECT( bcore_error_manager_context_s ) \
    {aware_t _;bcore_error_manager_error_adl_s adl;bcore_mutex_s mutex;};
#define TYPEOF_general_error 681885290
#define TYPEOF_parse_error 1107704077
#define TYPEOF_plant_error 742081543
#define TYPEOF_error_stack 2991660346
#define BETH_EXPAND_GROUP_bcore_error_manager \
  BCORE_FORWARD_OBJECT( bcore_error_manager ); \
  BCORE_FORWARD_OBJECT( bcore_error_manager_error_s ); \
  BCORE_FORWARD_OBJECT( bcore_error_manager_error_adl_s ); \
  BCORE_FORWARD_OBJECT( bcore_error_manager_context_s ); \
  BETH_EXPAND_ITEM_bcore_error_manager_error_s \
  BETH_EXPAND_ITEM_bcore_error_manager_error_adl_s \
  BETH_EXPAND_ITEM_bcore_error_manager_context_s

/**********************************************************************************************************************/
// source: bcore_prsg.h

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_prsg

#define TYPEOF_bcore_prsg 3663626195
#define TYPEOF_bcore_prsg_s 329420597
#define BETH_EXPAND_GROUP_bcore_prsg \
  BCORE_FORWARD_OBJECT( bcore_prsg ); \
  BCORE_FORWARD_OBJECT( bcore_prsg_lcg ); \
  BCORE_FORWARD_OBJECT( bcore_prsg_xsg ); \
  typedef sz_t (*bcore_prsg_bits)( const bcore_prsg* o ); \
  typedef u3_t (*bcore_prsg_max_u3)( const bcore_prsg* o ); \
  typedef u3_t (*bcore_prsg_min_u3)( const bcore_prsg* o ); \
  typedef u3_t (*bcore_prsg_state_u3)( const bcore_prsg* o ); \
  typedef u3_t (*bcore_prsg_state_bits_u3)( const bcore_prsg* o, sz_t bits ); \
  typedef f3_t (*bcore_prsg_state_f3)( const bcore_prsg* o, f3_t min, f3_t max ); \
  typedef bl_t (*bcore_prsg_state_bl)( const bcore_prsg* o ); \
  typedef void (*bcore_prsg_gen)( bcore_prsg* o ); \
  typedef u3_t (*bcore_prsg_gen_bits_u3)( bcore_prsg* o, sz_t bits ); \
  typedef u3_t (*bcore_prsg_gen_u3)( bcore_prsg* o ); \
  typedef f3_t (*bcore_prsg_gen_f3)( bcore_prsg* o, f3_t min, f3_t max ); \
  typedef bl_t (*bcore_prsg_gen_bl)( bcore_prsg* o ); \
  typedef bcore_prsg* (*bcore_prsg_set_state_u3)( bcore_prsg* o, u3_t seed ); \
  typedef bcore_prsg* (*bcore_prsg_set_state_mix)( bcore_prsg* o, const bcore_prsg* a, const bcore_prsg* b ); \
  typedef void (*bcore_prsg_reseed)( bcore_prsg* o, u3_t seed ); \
  BCORE_DECLARE_SPECT( bcore_prsg ) \
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
  static inline bcore_prsg* bcore_prsg_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_bcore_prsg, t ); return ( bcore_prsg* )bcore_inst_t_create( t ); } \
  static inline bl_t bcore_prsg_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_bcore_prsg ); } \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bcore_prsg ) \
  static inline bl_t bcore_prsg_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_bcore_prsg ); } \
  static inline sz_t bcore_prsg_a_bits( const bcore_prsg* o ) { const bcore_prsg_s* p = bcore_prsg_s_get_aware( o ); assert( p->bits ); return p->bits( o ); } \
  static inline bl_t bcore_prsg_a_defines_bits( const bcore_prsg* o ) { return true; } \
  static inline u3_t bcore_prsg_a_max_u3( const bcore_prsg* o ) { const bcore_prsg_s* p = bcore_prsg_s_get_aware( o ); assert( p->max_u3 ); return p->max_u3( o ); } \
  static inline bl_t bcore_prsg_a_defines_max_u3( const bcore_prsg* o ) { return true; } \
  static inline u3_t bcore_prsg_a_min_u3( const bcore_prsg* o ) { const bcore_prsg_s* p = bcore_prsg_s_get_aware( o ); assert( p->min_u3 ); return p->min_u3( o ); } \
  static inline bl_t bcore_prsg_a_defines_min_u3( const bcore_prsg* o ) { return true; } \
  static inline u3_t bcore_prsg_a_state_u3( const bcore_prsg* o ) { const bcore_prsg_s* p = bcore_prsg_s_get_aware( o ); assert( p->state_u3 ); return p->state_u3( o ); } \
  static inline bl_t bcore_prsg_a_defines_state_u3( const bcore_prsg* o ) { return true; } \
  static inline u3_t bcore_prsg_a_state_bits_u3( const bcore_prsg* o, sz_t bits ) { const bcore_prsg_s* p = bcore_prsg_s_get_aware( o ); assert( p->state_bits_u3 ); return p->state_bits_u3( o, bits ); } \
  static inline bl_t bcore_prsg_a_defines_state_bits_u3( const bcore_prsg* o ) { return true; } \
  static inline f3_t bcore_prsg_a_state_f3( const bcore_prsg* o, f3_t min, f3_t max ) { const bcore_prsg_s* p = bcore_prsg_s_get_aware( o ); assert( p->state_f3 ); return p->state_f3( o, min, max ); } \
  static inline bl_t bcore_prsg_a_defines_state_f3( const bcore_prsg* o ) { return true; } \
  static inline bl_t bcore_prsg_a_state_bl( const bcore_prsg* o ) { const bcore_prsg_s* p = bcore_prsg_s_get_aware( o ); assert( p->state_bl ); return p->state_bl( o ); } \
  static inline bl_t bcore_prsg_a_defines_state_bl( const bcore_prsg* o ) { return true; } \
  static inline bl_t bcore_prsg_state_bl__( const bcore_prsg* o ){return bcore_prsg_a_state_bits_u3( o, 1 ) ? true : false;} \
  static inline void bcore_prsg_a_gen( bcore_prsg* o ) { const bcore_prsg_s* p = bcore_prsg_s_get_aware( o ); assert( p->gen ); p->gen( o ); } \
  static inline bl_t bcore_prsg_a_defines_gen( const bcore_prsg* o ) { return true; } \
  static inline u3_t bcore_prsg_a_gen_bits_u3( bcore_prsg* o, sz_t bits ) { const bcore_prsg_s* p = bcore_prsg_s_get_aware( o ); assert( p->gen_bits_u3 ); return p->gen_bits_u3( o, bits ); } \
  static inline bl_t bcore_prsg_a_defines_gen_bits_u3( const bcore_prsg* o ) { return true; } \
  static inline u3_t bcore_prsg_a_gen_u3( bcore_prsg* o ) { const bcore_prsg_s* p = bcore_prsg_s_get_aware( o ); assert( p->gen_u3 ); return p->gen_u3( o ); } \
  static inline bl_t bcore_prsg_a_defines_gen_u3( const bcore_prsg* o ) { return true; } \
  static inline f3_t bcore_prsg_a_gen_f3( bcore_prsg* o, f3_t min, f3_t max ) { const bcore_prsg_s* p = bcore_prsg_s_get_aware( o ); assert( p->gen_f3 ); return p->gen_f3( o, min, max ); } \
  static inline bl_t bcore_prsg_a_defines_gen_f3( const bcore_prsg* o ) { return true; } \
  static inline bl_t bcore_prsg_a_gen_bl( bcore_prsg* o ) { const bcore_prsg_s* p = bcore_prsg_s_get_aware( o ); assert( p->gen_bl ); return p->gen_bl( o ); } \
  static inline bl_t bcore_prsg_a_defines_gen_bl( const bcore_prsg* o ) { return true; } \
  static inline bl_t bcore_prsg_gen_bl__( bcore_prsg* o ){return bcore_prsg_a_gen_bits_u3( o, 1 ) ? true : false;} \
  static inline bcore_prsg* bcore_prsg_a_set_state_u3( bcore_prsg* o, u3_t seed ) { const bcore_prsg_s* p = bcore_prsg_s_get_aware( o ); assert( p->set_state_u3 ); return p->set_state_u3( o, seed ); } \
  static inline bl_t bcore_prsg_a_defines_set_state_u3( const bcore_prsg* o ) { return true; } \
  static inline bcore_prsg* bcore_prsg_a_set_state_mix( bcore_prsg* o, const bcore_prsg* a, const bcore_prsg* b ) { const bcore_prsg_s* p = bcore_prsg_s_get_aware( o ); assert( p->set_state_mix ); return p->set_state_mix( o, a, b ); } \
  static inline bl_t bcore_prsg_a_defines_set_state_mix( const bcore_prsg* o ) { return true; } \
  bcore_prsg* bcore_prsg_set_state_mix__( bcore_prsg* o, const bcore_prsg* a, const bcore_prsg* b ); \
  static inline void bcore_prsg_a_reseed( bcore_prsg* o, u3_t seed ) { const bcore_prsg_s* p = bcore_prsg_s_get_aware( o ); assert( p->reseed ); p->reseed( o, seed ); } \
  static inline bl_t bcore_prsg_a_defines_reseed( const bcore_prsg* o ) { return true; } \
  static inline void bcore_prsg_reseed__( bcore_prsg* o, u3_t seed ){bcore_prsg_a_set_state_u3( o, seed );} \
  BETH_EXPAND_GROUP_bcore_prsg_lcg \
  BETH_EXPAND_GROUP_bcore_prsg_xsg

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_prsg_lcg

#define TYPEOF_bcore_prsg_lcg 2865172018
#define TYPEOF_bcore_prsg_lcg_s 2153731564
#define TYPEOF_bcore_prsg_lcg_u2_00_s 1211325803
#define BETH_EXPAND_ITEM_bcore_prsg_lcg_u2_00_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_lcg_u2_00_s ) \
    {aware_t _;u2_t state;}; \
  u3_t bcore_prsg_lcg_u2_00_s_gen_u3( bcore_prsg_lcg_u2_00_s* o ); \
  u3_t bcore_prsg_lcg_u2_00_s_state_bits_u3( const bcore_prsg_lcg_u2_00_s* o, sz_t bits ); \
  u3_t bcore_prsg_lcg_u2_00_s_gen_bits_u3( bcore_prsg_lcg_u2_00_s* o, sz_t bits ); \
  f3_t bcore_prsg_lcg_u2_00_s_state_f3( const bcore_prsg_lcg_u2_00_s* o, f3_t min, f3_t max ); \
  f3_t bcore_prsg_lcg_u2_00_s_gen_f3( bcore_prsg_lcg_u2_00_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_lcg_u2_00_s_bits( const bcore_prsg_lcg_u2_00_s* o ){return 32;} \
  static inline u3_t bcore_prsg_lcg_u2_00_s_max_u3( const bcore_prsg_lcg_u2_00_s* o ){return 0xFFFFFFFFu;} \
  static inline u3_t bcore_prsg_lcg_u2_00_s_min_u3( const bcore_prsg_lcg_u2_00_s* o ){return 0;} \
  static inline u3_t bcore_prsg_lcg_u2_00_s_state_u3( const bcore_prsg_lcg_u2_00_s* o ){return o->state;} \
  static inline bcore_prsg* bcore_prsg_lcg_u2_00_s_set_state_u3( bcore_prsg_lcg_u2_00_s* o, u3_t seed ){o->state = seed ^ ( seed >> 32 ); return (bcore_prsg*)o;} \
  static inline void bcore_prsg_lcg_u2_00_s_gen( bcore_prsg_lcg_u2_00_s* o ){o->state = bcore_lcg00_u2( o->state );}
#define TYPEOF_bcore_prsg_lcg_u2_01_s 3427325360
#define BETH_EXPAND_ITEM_bcore_prsg_lcg_u2_01_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_lcg_u2_01_s ) \
    {aware_t _;u2_t state;}; \
  u3_t bcore_prsg_lcg_u2_01_s_gen_u3( bcore_prsg_lcg_u2_01_s* o ); \
  u3_t bcore_prsg_lcg_u2_01_s_state_bits_u3( const bcore_prsg_lcg_u2_01_s* o, sz_t bits ); \
  u3_t bcore_prsg_lcg_u2_01_s_gen_bits_u3( bcore_prsg_lcg_u2_01_s* o, sz_t bits ); \
  f3_t bcore_prsg_lcg_u2_01_s_state_f3( const bcore_prsg_lcg_u2_01_s* o, f3_t min, f3_t max ); \
  f3_t bcore_prsg_lcg_u2_01_s_gen_f3( bcore_prsg_lcg_u2_01_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_lcg_u2_01_s_bits( const bcore_prsg_lcg_u2_01_s* o ){return 32;} \
  static inline u3_t bcore_prsg_lcg_u2_01_s_max_u3( const bcore_prsg_lcg_u2_01_s* o ){return 0xFFFFFFFFu;} \
  static inline u3_t bcore_prsg_lcg_u2_01_s_min_u3( const bcore_prsg_lcg_u2_01_s* o ){return 0;} \
  static inline u3_t bcore_prsg_lcg_u2_01_s_state_u3( const bcore_prsg_lcg_u2_01_s* o ){return o->state;} \
  static inline bcore_prsg* bcore_prsg_lcg_u2_01_s_set_state_u3( bcore_prsg_lcg_u2_01_s* o, u3_t seed ){o->state = seed ^ ( seed >> 32 ); return (bcore_prsg*)o;} \
  static inline void bcore_prsg_lcg_u2_01_s_gen( bcore_prsg_lcg_u2_01_s* o ){o->state = bcore_lcg01_u2( o->state );}
#define TYPEOF_bcore_prsg_lcg_u2_02_s 1998777197
#define BETH_EXPAND_ITEM_bcore_prsg_lcg_u2_02_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_lcg_u2_02_s ) \
    {aware_t _;u2_t state;}; \
  u3_t bcore_prsg_lcg_u2_02_s_gen_u3( bcore_prsg_lcg_u2_02_s* o ); \
  u3_t bcore_prsg_lcg_u2_02_s_state_bits_u3( const bcore_prsg_lcg_u2_02_s* o, sz_t bits ); \
  u3_t bcore_prsg_lcg_u2_02_s_gen_bits_u3( bcore_prsg_lcg_u2_02_s* o, sz_t bits ); \
  f3_t bcore_prsg_lcg_u2_02_s_state_f3( const bcore_prsg_lcg_u2_02_s* o, f3_t min, f3_t max ); \
  f3_t bcore_prsg_lcg_u2_02_s_gen_f3( bcore_prsg_lcg_u2_02_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_lcg_u2_02_s_bits( const bcore_prsg_lcg_u2_02_s* o ){return 32;} \
  static inline u3_t bcore_prsg_lcg_u2_02_s_max_u3( const bcore_prsg_lcg_u2_02_s* o ){return 0xFFFFFFFFu;} \
  static inline u3_t bcore_prsg_lcg_u2_02_s_min_u3( const bcore_prsg_lcg_u2_02_s* o ){return 0;} \
  static inline u3_t bcore_prsg_lcg_u2_02_s_state_u3( const bcore_prsg_lcg_u2_02_s* o ){return o->state;} \
  static inline bcore_prsg* bcore_prsg_lcg_u2_02_s_set_state_u3( bcore_prsg_lcg_u2_02_s* o, u3_t seed ){o->state = seed ^ ( seed >> 32 ); return (bcore_prsg*)o;} \
  static inline void bcore_prsg_lcg_u2_02_s_gen( bcore_prsg_lcg_u2_02_s* o ){o->state = bcore_lcg02_u2( o->state );}
#define TYPEOF_bcore_prsg_lcg_u2_03_s 4204330994
#define BETH_EXPAND_ITEM_bcore_prsg_lcg_u2_03_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_lcg_u2_03_s ) \
    {aware_t _;u2_t state;}; \
  u3_t bcore_prsg_lcg_u2_03_s_gen_u3( bcore_prsg_lcg_u2_03_s* o ); \
  u3_t bcore_prsg_lcg_u2_03_s_state_bits_u3( const bcore_prsg_lcg_u2_03_s* o, sz_t bits ); \
  u3_t bcore_prsg_lcg_u2_03_s_gen_bits_u3( bcore_prsg_lcg_u2_03_s* o, sz_t bits ); \
  f3_t bcore_prsg_lcg_u2_03_s_state_f3( const bcore_prsg_lcg_u2_03_s* o, f3_t min, f3_t max ); \
  f3_t bcore_prsg_lcg_u2_03_s_gen_f3( bcore_prsg_lcg_u2_03_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_lcg_u2_03_s_bits( const bcore_prsg_lcg_u2_03_s* o ){return 32;} \
  static inline u3_t bcore_prsg_lcg_u2_03_s_max_u3( const bcore_prsg_lcg_u2_03_s* o ){return 0xFFFFFFFFu;} \
  static inline u3_t bcore_prsg_lcg_u2_03_s_min_u3( const bcore_prsg_lcg_u2_03_s* o ){return 0;} \
  static inline u3_t bcore_prsg_lcg_u2_03_s_state_u3( const bcore_prsg_lcg_u2_03_s* o ){return o->state;} \
  static inline bcore_prsg* bcore_prsg_lcg_u2_03_s_set_state_u3( bcore_prsg_lcg_u2_03_s* o, u3_t seed ){o->state = seed ^ ( seed >> 32 ); return (bcore_prsg*)o;} \
  static inline void bcore_prsg_lcg_u2_03_s_gen( bcore_prsg_lcg_u2_03_s* o ){o->state = bcore_lcg03_u2( o->state );}
#define TYPEOF_bcore_prsg_lcg_u2_04_s 4107291135
#define BETH_EXPAND_ITEM_bcore_prsg_lcg_u2_04_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_lcg_u2_04_s ) \
    {aware_t _;u2_t state;}; \
  u3_t bcore_prsg_lcg_u2_04_s_gen_u3( bcore_prsg_lcg_u2_04_s* o ); \
  u3_t bcore_prsg_lcg_u2_04_s_state_bits_u3( const bcore_prsg_lcg_u2_04_s* o, sz_t bits ); \
  u3_t bcore_prsg_lcg_u2_04_s_gen_bits_u3( bcore_prsg_lcg_u2_04_s* o, sz_t bits ); \
  f3_t bcore_prsg_lcg_u2_04_s_state_f3( const bcore_prsg_lcg_u2_04_s* o, f3_t min, f3_t max ); \
  f3_t bcore_prsg_lcg_u2_04_s_gen_f3( bcore_prsg_lcg_u2_04_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_lcg_u2_04_s_bits( const bcore_prsg_lcg_u2_04_s* o ){return 32;} \
  static inline u3_t bcore_prsg_lcg_u2_04_s_max_u3( const bcore_prsg_lcg_u2_04_s* o ){return 0xFFFFFFFFu;} \
  static inline u3_t bcore_prsg_lcg_u2_04_s_min_u3( const bcore_prsg_lcg_u2_04_s* o ){return 0;} \
  static inline u3_t bcore_prsg_lcg_u2_04_s_state_u3( const bcore_prsg_lcg_u2_04_s* o ){return o->state;} \
  static inline bcore_prsg* bcore_prsg_lcg_u2_04_s_set_state_u3( bcore_prsg_lcg_u2_04_s* o, u3_t seed ){o->state = seed ^ ( seed >> 32 ); return (bcore_prsg*)o;} \
  static inline void bcore_prsg_lcg_u2_04_s_gen( bcore_prsg_lcg_u2_04_s* o ){o->state = bcore_lcg04_u2( o->state );}
#define TYPEOF_bcore_prsg_lcg_u2_05_s 2523630500
#define BETH_EXPAND_ITEM_bcore_prsg_lcg_u2_05_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_lcg_u2_05_s ) \
    {aware_t _;u2_t state;}; \
  u3_t bcore_prsg_lcg_u2_05_s_gen_u3( bcore_prsg_lcg_u2_05_s* o ); \
  u3_t bcore_prsg_lcg_u2_05_s_state_bits_u3( const bcore_prsg_lcg_u2_05_s* o, sz_t bits ); \
  u3_t bcore_prsg_lcg_u2_05_s_gen_bits_u3( bcore_prsg_lcg_u2_05_s* o, sz_t bits ); \
  f3_t bcore_prsg_lcg_u2_05_s_state_f3( const bcore_prsg_lcg_u2_05_s* o, f3_t min, f3_t max ); \
  f3_t bcore_prsg_lcg_u2_05_s_gen_f3( bcore_prsg_lcg_u2_05_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_lcg_u2_05_s_bits( const bcore_prsg_lcg_u2_05_s* o ){return 32;} \
  static inline u3_t bcore_prsg_lcg_u2_05_s_max_u3( const bcore_prsg_lcg_u2_05_s* o ){return 0xFFFFFFFFu;} \
  static inline u3_t bcore_prsg_lcg_u2_05_s_min_u3( const bcore_prsg_lcg_u2_05_s* o ){return 0;} \
  static inline u3_t bcore_prsg_lcg_u2_05_s_state_u3( const bcore_prsg_lcg_u2_05_s* o ){return o->state;} \
  static inline bcore_prsg* bcore_prsg_lcg_u2_05_s_set_state_u3( bcore_prsg_lcg_u2_05_s* o, u3_t seed ){o->state = seed ^ ( seed >> 32 ); return (bcore_prsg*)o;} \
  static inline void bcore_prsg_lcg_u2_05_s_gen( bcore_prsg_lcg_u2_05_s* o ){o->state = bcore_lcg05_u2( o->state );}
#define TYPEOF_bcore_prsg_lcg_u3_00_s 193544906
#define BETH_EXPAND_ITEM_bcore_prsg_lcg_u3_00_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_lcg_u3_00_s ) \
    {aware_t _;u3_t state;}; \
  u3_t bcore_prsg_lcg_u3_00_s_gen_u3( bcore_prsg_lcg_u3_00_s* o ); \
  u3_t bcore_prsg_lcg_u3_00_s_state_bits_u3( const bcore_prsg_lcg_u3_00_s* o, sz_t bits ); \
  u3_t bcore_prsg_lcg_u3_00_s_gen_bits_u3( bcore_prsg_lcg_u3_00_s* o, sz_t bits ); \
  f3_t bcore_prsg_lcg_u3_00_s_state_f3( const bcore_prsg_lcg_u3_00_s* o, f3_t min, f3_t max ); \
  f3_t bcore_prsg_lcg_u3_00_s_gen_f3( bcore_prsg_lcg_u3_00_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_lcg_u3_00_s_bits( const bcore_prsg_lcg_u3_00_s* o ){return 64;} \
  static inline u3_t bcore_prsg_lcg_u3_00_s_max_u3( const bcore_prsg_lcg_u3_00_s* o ){return 0xFFFFFFFFFFFFFFFFull;} \
  static inline u3_t bcore_prsg_lcg_u3_00_s_min_u3( const bcore_prsg_lcg_u3_00_s* o ){return 0;} \
  static inline u3_t bcore_prsg_lcg_u3_00_s_state_u3( const bcore_prsg_lcg_u3_00_s* o ){return o->state;} \
  static inline bcore_prsg* bcore_prsg_lcg_u3_00_s_set_state_u3( bcore_prsg_lcg_u3_00_s* o, u3_t seed ){o->state = seed; return (bcore_prsg*)o;} \
  static inline void bcore_prsg_lcg_u3_00_s_gen( bcore_prsg_lcg_u3_00_s* o ){o->state = bcore_lcg00_u3( o->state );}
#define TYPEOF_bcore_prsg_lcg_u3_01_s 1735628837
#define BETH_EXPAND_ITEM_bcore_prsg_lcg_u3_01_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_lcg_u3_01_s ) \
    {aware_t _;u3_t state;}; \
  u3_t bcore_prsg_lcg_u3_01_s_gen_u3( bcore_prsg_lcg_u3_01_s* o ); \
  u3_t bcore_prsg_lcg_u3_01_s_state_bits_u3( const bcore_prsg_lcg_u3_01_s* o, sz_t bits ); \
  u3_t bcore_prsg_lcg_u3_01_s_gen_bits_u3( bcore_prsg_lcg_u3_01_s* o, sz_t bits ); \
  f3_t bcore_prsg_lcg_u3_01_s_state_f3( const bcore_prsg_lcg_u3_01_s* o, f3_t min, f3_t max ); \
  f3_t bcore_prsg_lcg_u3_01_s_gen_f3( bcore_prsg_lcg_u3_01_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_lcg_u3_01_s_bits( const bcore_prsg_lcg_u3_01_s* o ){return 64;} \
  static inline u3_t bcore_prsg_lcg_u3_01_s_max_u3( const bcore_prsg_lcg_u3_01_s* o ){return 0xFFFFFFFFFFFFFFFFull;} \
  static inline u3_t bcore_prsg_lcg_u3_01_s_min_u3( const bcore_prsg_lcg_u3_01_s* o ){return 0;} \
  static inline u3_t bcore_prsg_lcg_u3_01_s_state_u3( const bcore_prsg_lcg_u3_01_s* o ){return o->state;} \
  static inline bcore_prsg* bcore_prsg_lcg_u3_01_s_set_state_u3( bcore_prsg_lcg_u3_01_s* o, u3_t seed ){o->state = seed; return (bcore_prsg*)o;} \
  static inline void bcore_prsg_lcg_u3_01_s_gen( bcore_prsg_lcg_u3_01_s* o ){o->state = bcore_lcg01_u3( o->state );}
#define TYPEOF_bcore_prsg_lcg_u3_02_s 3690718216
#define BETH_EXPAND_ITEM_bcore_prsg_lcg_u3_02_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_lcg_u3_02_s ) \
    {aware_t _;u3_t state;}; \
  u3_t bcore_prsg_lcg_u3_02_s_gen_u3( bcore_prsg_lcg_u3_02_s* o ); \
  u3_t bcore_prsg_lcg_u3_02_s_state_bits_u3( const bcore_prsg_lcg_u3_02_s* o, sz_t bits ); \
  u3_t bcore_prsg_lcg_u3_02_s_gen_bits_u3( bcore_prsg_lcg_u3_02_s* o, sz_t bits ); \
  f3_t bcore_prsg_lcg_u3_02_s_state_f3( const bcore_prsg_lcg_u3_02_s* o, f3_t min, f3_t max ); \
  f3_t bcore_prsg_lcg_u3_02_s_gen_f3( bcore_prsg_lcg_u3_02_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_lcg_u3_02_s_bits( const bcore_prsg_lcg_u3_02_s* o ){return 64;} \
  static inline u3_t bcore_prsg_lcg_u3_02_s_max_u3( const bcore_prsg_lcg_u3_02_s* o ){return 0xFFFFFFFFFFFFFFFFull;} \
  static inline u3_t bcore_prsg_lcg_u3_02_s_min_u3( const bcore_prsg_lcg_u3_02_s* o ){return 0;} \
  static inline u3_t bcore_prsg_lcg_u3_02_s_state_u3( const bcore_prsg_lcg_u3_02_s* o ){return o->state;} \
  static inline bcore_prsg* bcore_prsg_lcg_u3_02_s_set_state_u3( bcore_prsg_lcg_u3_02_s* o, u3_t seed ){o->state = seed; return (bcore_prsg*)o;} \
  static inline void bcore_prsg_lcg_u3_02_s_gen( bcore_prsg_lcg_u3_02_s* o ){o->state = bcore_lcg02_u3( o->state );}
#define TYPEOF_bcore_prsg_lcg_u3_03_s 958623203
#define BETH_EXPAND_ITEM_bcore_prsg_lcg_u3_03_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_lcg_u3_03_s ) \
    {aware_t _;u3_t state;}; \
  u3_t bcore_prsg_lcg_u3_03_s_gen_u3( bcore_prsg_lcg_u3_03_s* o ); \
  u3_t bcore_prsg_lcg_u3_03_s_state_bits_u3( const bcore_prsg_lcg_u3_03_s* o, sz_t bits ); \
  u3_t bcore_prsg_lcg_u3_03_s_gen_bits_u3( bcore_prsg_lcg_u3_03_s* o, sz_t bits ); \
  f3_t bcore_prsg_lcg_u3_03_s_state_f3( const bcore_prsg_lcg_u3_03_s* o, f3_t min, f3_t max ); \
  f3_t bcore_prsg_lcg_u3_03_s_gen_f3( bcore_prsg_lcg_u3_03_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_lcg_u3_03_s_bits( const bcore_prsg_lcg_u3_03_s* o ){return 64;} \
  static inline u3_t bcore_prsg_lcg_u3_03_s_max_u3( const bcore_prsg_lcg_u3_03_s* o ){return 0xFFFFFFFFFFFFFFFFull;} \
  static inline u3_t bcore_prsg_lcg_u3_03_s_min_u3( const bcore_prsg_lcg_u3_03_s* o ){return 0;} \
  static inline u3_t bcore_prsg_lcg_u3_03_s_state_u3( const bcore_prsg_lcg_u3_03_s* o ){return o->state;} \
  static inline bcore_prsg* bcore_prsg_lcg_u3_03_s_set_state_u3( bcore_prsg_lcg_u3_03_s* o, u3_t seed ){o->state = seed; return (bcore_prsg*)o;} \
  static inline void bcore_prsg_lcg_u3_03_s_gen( bcore_prsg_lcg_u3_03_s* o ){o->state = bcore_lcg03_u3( o->state );}
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

#define TYPEOF_bcore_prsg_xsg 749034726
#define TYPEOF_bcore_prsg_xsg_s 1517635160
#define TYPEOF_bcore_prsg_xsg_u2_00_s 2389956751
#define BETH_EXPAND_ITEM_bcore_prsg_xsg_u2_00_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_xsg_u2_00_s ) \
    {aware_t _;u2_t state;}; \
  u3_t bcore_prsg_xsg_u2_00_s_gen_u3( bcore_prsg_xsg_u2_00_s* o ); \
  u3_t bcore_prsg_xsg_u2_00_s_state_bits_u3( const bcore_prsg_xsg_u2_00_s* o, sz_t bits ); \
  u3_t bcore_prsg_xsg_u2_00_s_gen_bits_u3( bcore_prsg_xsg_u2_00_s* o, sz_t bits ); \
  f3_t bcore_prsg_xsg_u2_00_s_state_f3( const bcore_prsg_xsg_u2_00_s* o, f3_t min, f3_t max ); \
  f3_t bcore_prsg_xsg_u2_00_s_gen_f3( bcore_prsg_xsg_u2_00_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_xsg_u2_00_s_bits( const bcore_prsg_xsg_u2_00_s* o ){return 32;} \
  static inline u3_t bcore_prsg_xsg_u2_00_s_max_u3( const bcore_prsg_xsg_u2_00_s* o ){return 0xFFFFFFFFu;} \
  static inline u3_t bcore_prsg_xsg_u2_00_s_min_u3( const bcore_prsg_xsg_u2_00_s* o ){return 1;} \
  static inline u3_t bcore_prsg_xsg_u2_00_s_state_u3( const bcore_prsg_xsg_u2_00_s* o ){return o->state;} \
  static inline bcore_prsg* bcore_prsg_xsg_u2_00_s_set_state_u3( bcore_prsg_xsg_u2_00_s* o, u3_t seed ){o->state = u2_max( 1, seed ^ ( seed >> 32 ) ); return (bcore_prsg*)o;} \
  static inline void bcore_prsg_xsg_u2_00_s_gen( bcore_prsg_xsg_u2_00_s* o ){o->state = bcore_xsg_u2(  o->state );}
#define TYPEOF_bcore_prsg_xsg_u2_01_s 837530228
#define BETH_EXPAND_ITEM_bcore_prsg_xsg_u2_01_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_xsg_u2_01_s ) \
    {aware_t _;u2_t state;}; \
  u3_t bcore_prsg_xsg_u2_01_s_gen_u3( bcore_prsg_xsg_u2_01_s* o ); \
  u3_t bcore_prsg_xsg_u2_01_s_state_bits_u3( const bcore_prsg_xsg_u2_01_s* o, sz_t bits ); \
  u3_t bcore_prsg_xsg_u2_01_s_gen_bits_u3( bcore_prsg_xsg_u2_01_s* o, sz_t bits ); \
  f3_t bcore_prsg_xsg_u2_01_s_state_f3( const bcore_prsg_xsg_u2_01_s* o, f3_t min, f3_t max ); \
  f3_t bcore_prsg_xsg_u2_01_s_gen_f3( bcore_prsg_xsg_u2_01_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_xsg_u2_01_s_bits( const bcore_prsg_xsg_u2_01_s* o ){return 32;} \
  static inline u3_t bcore_prsg_xsg_u2_01_s_max_u3( const bcore_prsg_xsg_u2_01_s* o ){return 0xFFFFFFFFu;} \
  static inline u3_t bcore_prsg_xsg_u2_01_s_min_u3( const bcore_prsg_xsg_u2_01_s* o ){return 1;} \
  static inline u3_t bcore_prsg_xsg_u2_01_s_state_u3( const bcore_prsg_xsg_u2_01_s* o ){return o->state;} \
  static inline bcore_prsg* bcore_prsg_xsg_u2_01_s_set_state_u3( bcore_prsg_xsg_u2_01_s* o, u3_t seed ){o->state = u2_max( 1, seed ^ ( seed >> 32 ) ); return (bcore_prsg*)o;} \
  static inline void bcore_prsg_xsg_u2_01_s_gen( bcore_prsg_xsg_u2_01_s* o ){o->state = bcore_xsg1_u2( o->state );}
#define TYPEOF_bcore_prsg_xsg_u2_02_s 3146174033
#define BETH_EXPAND_ITEM_bcore_prsg_xsg_u2_02_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_xsg_u2_02_s ) \
    {aware_t _;u2_t state;}; \
  u3_t bcore_prsg_xsg_u2_02_s_gen_u3( bcore_prsg_xsg_u2_02_s* o ); \
  u3_t bcore_prsg_xsg_u2_02_s_state_bits_u3( const bcore_prsg_xsg_u2_02_s* o, sz_t bits ); \
  u3_t bcore_prsg_xsg_u2_02_s_gen_bits_u3( bcore_prsg_xsg_u2_02_s* o, sz_t bits ); \
  f3_t bcore_prsg_xsg_u2_02_s_state_f3( const bcore_prsg_xsg_u2_02_s* o, f3_t min, f3_t max ); \
  f3_t bcore_prsg_xsg_u2_02_s_gen_f3( bcore_prsg_xsg_u2_02_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_xsg_u2_02_s_bits( const bcore_prsg_xsg_u2_02_s* o ){return 32;} \
  static inline u3_t bcore_prsg_xsg_u2_02_s_max_u3( const bcore_prsg_xsg_u2_02_s* o ){return 0xFFFFFFFFu;} \
  static inline u3_t bcore_prsg_xsg_u2_02_s_min_u3( const bcore_prsg_xsg_u2_02_s* o ){return 1;} \
  static inline u3_t bcore_prsg_xsg_u2_02_s_state_u3( const bcore_prsg_xsg_u2_02_s* o ){return o->state;} \
  static inline bcore_prsg* bcore_prsg_xsg_u2_02_s_set_state_u3( bcore_prsg_xsg_u2_02_s* o, u3_t seed ){o->state = u2_max( 1, seed ^ ( seed >> 32 ) ); return (bcore_prsg*)o;} \
  static inline void bcore_prsg_xsg_u2_02_s_gen( bcore_prsg_xsg_u2_02_s* o ){o->state = bcore_xsg2_u2( o->state );}
#define TYPEOF_bcore_prsg_xsg_u2_03_s 1056863702
#define BETH_EXPAND_ITEM_bcore_prsg_xsg_u2_03_s \
  BCORE_DECLARE_OBJECT( bcore_prsg_xsg_u2_03_s ) \
    {aware_t _;u2_t state;}; \
  u3_t bcore_prsg_xsg_u2_03_s_gen_u3( bcore_prsg_xsg_u2_03_s* o ); \
  u3_t bcore_prsg_xsg_u2_03_s_state_bits_u3( const bcore_prsg_xsg_u2_03_s* o, sz_t bits ); \
  u3_t bcore_prsg_xsg_u2_03_s_gen_bits_u3( bcore_prsg_xsg_u2_03_s* o, sz_t bits ); \
  f3_t bcore_prsg_xsg_u2_03_s_state_f3( const bcore_prsg_xsg_u2_03_s* o, f3_t min, f3_t max ); \
  f3_t bcore_prsg_xsg_u2_03_s_gen_f3( bcore_prsg_xsg_u2_03_s* o, f3_t min, f3_t max ); \
  static inline sz_t bcore_prsg_xsg_u2_03_s_bits( const bcore_prsg_xsg_u2_03_s* o ){return 32;} \
  static inline u3_t bcore_prsg_xsg_u2_03_s_max_u3( const bcore_prsg_xsg_u2_03_s* o ){return 0xFFFFFFFFu;} \
  static inline u3_t bcore_prsg_xsg_u2_03_s_min_u3( const bcore_prsg_xsg_u2_03_s* o ){return 1;} \
  static inline u3_t bcore_prsg_xsg_u2_03_s_state_u3( const bcore_prsg_xsg_u2_03_s* o ){return o->state;} \
  static inline bcore_prsg* bcore_prsg_xsg_u2_03_s_set_state_u3( bcore_prsg_xsg_u2_03_s* o, u3_t seed ){o->state = u2_max( 1, seed ^ ( seed >> 32 ) ); return (bcore_prsg*)o;} \
  static inline void bcore_prsg_xsg_u2_03_s_gen( bcore_prsg_xsg_u2_03_s* o ){o->state = bcore_xsg3_u2( o->state );}
#define BETH_EXPAND_GROUP_bcore_prsg_xsg \
  BCORE_FORWARD_OBJECT( bcore_prsg_xsg ); \
  BCORE_FORWARD_OBJECT( bcore_prsg_xsg_u2_00_s ); \
  BCORE_FORWARD_OBJECT( bcore_prsg_xsg_u2_01_s ); \
  BCORE_FORWARD_OBJECT( bcore_prsg_xsg_u2_02_s ); \
  BCORE_FORWARD_OBJECT( bcore_prsg_xsg_u2_03_s ); \
  BETH_EXPAND_ITEM_bcore_prsg_xsg_u2_00_s \
  BETH_EXPAND_ITEM_bcore_prsg_xsg_u2_01_s \
  BETH_EXPAND_ITEM_bcore_prsg_xsg_u2_02_s \
  BETH_EXPAND_ITEM_bcore_prsg_xsg_u2_03_s

/**********************************************************************************************************************/

vd_t bcore_xoi_out_signal_handler( const bcore_signal_s* o );

#endif // BCORE_XOI_OUT_H
// BETH_PLANT_SIGNATURE 3909787697
