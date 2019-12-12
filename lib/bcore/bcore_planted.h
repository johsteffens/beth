/** This file was generated from beth-plant source code.
 *  Compiling Agent : bcore_plant_compiler (C) 2019 J.B.Steffens
 *  Last File Update: 2019-12-12T12:53:51Z
 *
 *  Copyright and License of this File:
 *
 *  Generated code inherits the copyright and license of the underlying beth-plant source code.
 *  Source code defining this file is distributed across following files:
 *
 *  bcore_file.h
 *  bcore_plant_inexpandable.h
 *  bcore_plant_sample.h
 *  bcore_spect_inst_call.h
 *  bcore_spect_via_call.h
 *  bcore_main.h
 *  bcore_hmap_name.h
 *  bcore_cday.h
 *
 */

#ifndef BCORE_PLANTED_H
#define BCORE_PLANTED_H

#include "bcore_control.h"


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
// source: bcore_plant_inexpandable.h

/**********************************************************************************************************************/
// source: bcore_plant_sample.h

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_plant_sample

#define TYPEOF_bcore_plant_sample 910331991
#define TYPEOF_bcore_plant_sample_s 2634114545
#define TYPEOF_anyglobalname 2209051205
#define TYPEOF_bcore_plant_sample_object2_s 2356042977
#define BETH_EXPAND_ITEM_bcore_plant_sample_object2_s \
  BCORE_DECLARE_OBJECT( bcore_plant_sample_object2_s ) \
    {aware_t _;bcore_plant_sample_features_object_s object;st_s name2;}; \
  void bcore_plant_sample_object2_s_setup( bcore_plant_sample_object2_s* o, sz_t n ); \
  void bcore_plant_sample_object2_s_to_stdout( const bcore_plant_sample_object2_s* o );
#define BETH_EXPAND_GROUP_bcore_plant_sample \
  BCORE_FORWARD_OBJECT( bcore_plant_sample ); \
  BCORE_FORWARD_OBJECT( bcore_plant_sample_signatures ); \
  BCORE_FORWARD_OBJECT( bcore_plant_sample_features ); \
  BCORE_FORWARD_OBJECT( bcore_plant_sample_object2_s ); \
  BETH_EXPAND_GROUP_bcore_plant_sample_signatures \
  BETH_EXPAND_GROUP_bcore_plant_sample_features \
  BETH_EXPAND_ITEM_bcore_plant_sample_object2_s

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_plant_sample_signatures

#define TYPEOF_bcore_plant_sample_signatures 2237955315
#define TYPEOF_bcore_plant_sample_signatures_s 234423829
#define BETH_EXPAND_GROUP_bcore_plant_sample_signatures \
  BCORE_FORWARD_OBJECT( bcore_plant_sample_signatures );

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_plant_sample_features

#define TYPEOF_bcore_plant_sample_features 528495263
#define TYPEOF_bcore_plant_sample_features_s 3695459401
#define TYPEOF_bcore_plant_sample_features_object_s 484501233
#define BETH_EXPAND_ITEM_bcore_plant_sample_features_object_s \
  BCORE_DECLARE_OBJECT( bcore_plant_sample_features_object_s ) \
    {aware_t _;st_s name1;BCORE_ARRAY_DYN_LINK_STATIC_S( sz_t, arr_ );}; \
  void bcore_plant_sample_features_object_s_setup( bcore_plant_sample_features_object_s* o, sz_t n ); \
  static inline void bcore_plant_sample_features_object_s_to_stdout( const bcore_plant_sample_features_object_s* o ){bcore_txt_ml_a_to_stdout( o );} \
  sz_t bcore_plant_sample_features_object_s_get_size( const bcore_plant_sample_features_object_s* o ); \
  static inline void bcore_plant_sample_features_object_s_simple_function( bcore_plant_sample_features_object_s* o, sz_t n ){/* do nothing */}
#define BETH_EXPAND_GROUP_bcore_plant_sample_features \
  BCORE_FORWARD_OBJECT( bcore_plant_sample_features ); \
  BCORE_FORWARD_OBJECT( bcore_plant_sample_features_object_s ); \
  typedef void (*bcore_plant_sample_features_setup)( bcore_plant_sample_features* o, sz_t n ); \
  typedef void (*bcore_plant_sample_features_to_stdout)( const bcore_plant_sample_features* o ); \
  typedef sz_t (*bcore_plant_sample_features_get_size)( const bcore_plant_sample_features* o ); \
  BCORE_DECLARE_SPECT( bcore_plant_sample_features ) \
  { \
      bcore_spect_header_s header; \
      bcore_plant_sample_features_setup setup; \
      bcore_plant_sample_features_to_stdout to_stdout; \
      bcore_plant_sample_features_get_size get_size; \
  }; \
  static inline bcore_plant_sample_features* bcore_plant_sample_features_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_bcore_plant_sample_features, t ); return ( bcore_plant_sample_features* )bcore_inst_t_create( t ); } \
  static inline bl_t bcore_plant_sample_features_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_bcore_plant_sample_features ); } \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bcore_plant_sample_features ) \
  static inline bl_t bcore_plant_sample_features_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_bcore_plant_sample_features ); } \
  static inline void bcore_plant_sample_features_p_setup( const bcore_plant_sample_features_s* __p, bcore_plant_sample_features* o, sz_t n ) { assert( __p->setup ); __p->setup( o, n ); } \
  static inline void bcore_plant_sample_features_t_setup( tp_t __t, bcore_plant_sample_features* o, sz_t n ) { const bcore_plant_sample_features_s* p = bcore_plant_sample_features_s_get_typed( __t ); assert( p->setup ); p->setup( o, n ); } \
  static inline void bcore_plant_sample_features_a_setup( bcore_plant_sample_features* o, sz_t n ) { const bcore_plant_sample_features_s* p = bcore_plant_sample_features_s_get_aware( o ); assert( p->setup ); p->setup( o, n ); } \
  static inline void bcore_plant_sample_features_r_setup( const sr_s* o, sz_t n ) { ASSERT( !sr_s_is_const( o ) ); const bcore_plant_sample_features_s* p = (const bcore_plant_sample_features_s*)ch_spect_p( o->p, TYPEOF_bcore_plant_sample_features_s ); assert( p->setup ); p->setup( o->o, n ); } \
  static inline bl_t bcore_plant_sample_features_p_defines_setup( const bcore_plant_sample_features_s* __p ) { return true; } \
  static inline bl_t bcore_plant_sample_features_t_defines_setup( tp_t __t ) { return true; } \
  static inline bl_t bcore_plant_sample_features_a_defines_setup( const bcore_plant_sample_features* o ) { return true; } \
  static inline bl_t bcore_plant_sample_features_r_defines_setup( const sr_s* o ) { return true; } \
  static inline void bcore_plant_sample_features_p_to_stdout( const bcore_plant_sample_features_s* __p, const bcore_plant_sample_features* o ) { assert( __p->to_stdout ); __p->to_stdout( o ); } \
  static inline void bcore_plant_sample_features_t_to_stdout( tp_t __t, const bcore_plant_sample_features* o ) { const bcore_plant_sample_features_s* p = bcore_plant_sample_features_s_get_typed( __t ); assert( p->to_stdout ); p->to_stdout( o ); } \
  static inline void bcore_plant_sample_features_a_to_stdout( const bcore_plant_sample_features* o ) { const bcore_plant_sample_features_s* p = bcore_plant_sample_features_s_get_aware( o ); assert( p->to_stdout ); p->to_stdout( o ); } \
  static inline void bcore_plant_sample_features_r_to_stdout( const sr_s* o ) { const bcore_plant_sample_features_s* p = (const bcore_plant_sample_features_s*)ch_spect_p( o->p, TYPEOF_bcore_plant_sample_features_s ); assert( p->to_stdout ); p->to_stdout( o->o ); } \
  static inline bl_t bcore_plant_sample_features_p_defines_to_stdout( const bcore_plant_sample_features_s* __p ) { return true; } \
  static inline bl_t bcore_plant_sample_features_t_defines_to_stdout( tp_t __t ) { return true; } \
  static inline bl_t bcore_plant_sample_features_a_defines_to_stdout( const bcore_plant_sample_features* o ) { return true; } \
  static inline bl_t bcore_plant_sample_features_r_defines_to_stdout( const sr_s* o ) { return true; } \
  void bcore_plant_sample_features_to_stdout__( const bcore_plant_sample_features* o ); \
  static inline sz_t bcore_plant_sample_features_p_get_size( const bcore_plant_sample_features_s* __p, const bcore_plant_sample_features* o ) { assert( __p->get_size ); return __p->get_size( o ); } \
  static inline sz_t bcore_plant_sample_features_t_get_size( tp_t __t, const bcore_plant_sample_features* o ) { const bcore_plant_sample_features_s* p = bcore_plant_sample_features_s_get_typed( __t ); assert( p->get_size ); return p->get_size( o ); } \
  static inline sz_t bcore_plant_sample_features_a_get_size( const bcore_plant_sample_features* o ) { const bcore_plant_sample_features_s* p = bcore_plant_sample_features_s_get_aware( o ); assert( p->get_size ); return p->get_size( o ); } \
  static inline sz_t bcore_plant_sample_features_r_get_size( const sr_s* o ) { const bcore_plant_sample_features_s* p = (const bcore_plant_sample_features_s*)ch_spect_p( o->p, TYPEOF_bcore_plant_sample_features_s ); assert( p->get_size ); return p->get_size( o->o ); } \
  static inline bl_t bcore_plant_sample_features_p_defines_get_size( const bcore_plant_sample_features_s* __p ) { return __p->get_size != NULL; } \
  static inline bl_t bcore_plant_sample_features_t_defines_get_size( tp_t __t ) { return bcore_plant_sample_features_s_get_typed( __t )->get_size != NULL; } \
  static inline bl_t bcore_plant_sample_features_a_defines_get_size( const bcore_plant_sample_features* o ) { return bcore_plant_sample_features_s_get_aware( o )->get_size != NULL; } \
  static inline bl_t bcore_plant_sample_features_r_defines_get_size( const sr_s* o ) { return ( (bcore_plant_sample_features_s*)ch_spect_p( o->p, TYPEOF_bcore_plant_sample_features_s ) )->get_size != NULL; } \
  BETH_EXPAND_ITEM_bcore_plant_sample_features_object_s

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
    {aware_t _;bcore_interpreter* interpreter;bl_t use_first_argument;sc_t local_file;sc_t global_file;};
#define BETH_EXPAND_GROUP_bcore_main \
  BCORE_FORWARD_OBJECT( bcore_main ); \
  BCORE_FORWARD_OBJECT( bcore_main_frame_s ); \
  typedef s2_t (*bcore_main_main)( bcore_main* o, const bcore_arr_st_s* args ); \
  BCORE_DECLARE_SPECT( bcore_main ) \
  { \
      bcore_spect_header_s header; \
      bcore_main_main main; \
  }; \
  static inline bcore_main* bcore_main_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_bcore_main, t ); return ( bcore_main* )bcore_inst_t_create( t ); } \
  static inline bl_t bcore_main_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_bcore_main ); } \
  static inline s2_t bcore_main_r_main( const sr_s* o, const bcore_arr_st_s* args ) { ASSERT( !sr_s_is_const( o ) ); const bcore_main_s* p = (const bcore_main_s*)ch_spect_p( o->p, TYPEOF_bcore_main_s ); assert( p->main ); return p->main( o->o, args ); } \
  static inline bl_t bcore_main_r_defines_main( const sr_s* o ) { return true; } \
  BETH_EXPAND_ITEM_bcore_main_frame_s

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

vd_t bcore_planted_signal_handler( const bcore_signal_s* o );

#endif // BCORE_PLANTED_H
