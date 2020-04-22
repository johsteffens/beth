/** This file was generated from beth-plant source code.
 *  Compiling Agent : bcore_plant_compiler (C) 2019 J.B.Steffens
 *  Last File Update: 2020-04-22T11:28:14Z
 *
 *  Copyright and License of this File:
 *
 *  Generated code inherits the copyright and license of the underlying beth-plant source code.
 *  Source code defining this file is distributed across following files:
 *
 *  bhpt_sketch.h
 *  bhpt_frame.h
 *  bhpt_adaptor.h
 *  bhpt_tutor.h
 *
 */

#ifndef BHPT_PLANTED_H
#define BHPT_PLANTED_H

#include "bcore_control.h"


/**********************************************************************************************************************/
// source: bhpt_sketch.h

//----------------------------------------------------------------------------------------------------------------------
// group: bhpt

#define TYPEOF_bhpt 302254947
#define TYPEOF_bhpt_s 3799681029
#define BETH_EXPAND_GROUP_bhpt \
  BCORE_FORWARD_OBJECT( bhpt ); \
  BCORE_FORWARD_OBJECT( bhpt_adaptor ); \
  BCORE_FORWARD_OBJECT( bhpt_adaptive ); \
  BCORE_FORWARD_OBJECT( bhpt_builder ); \
  BCORE_FORWARD_OBJECT( bhpt_tutor ); \
  BETH_EXPAND_GROUP_bhpt_adaptor \
  BETH_EXPAND_GROUP_bhpt_adaptive \
  BETH_EXPAND_GROUP_bhpt_builder \
  BETH_EXPAND_GROUP_bhpt_tutor

//----------------------------------------------------------------------------------------------------------------------
// group: bhpt_adaptor

#define TYPEOF_bhpt_adaptor 1238366945
#define TYPEOF_bhpt_adaptor_s 3695763531
#define TYPEOF_bhpt_adaptor_node_s 1522264424
#define BETH_EXPAND_ITEM_bhpt_adaptor_node_s \
  BCORE_DECLARE_OBJECT( bhpt_adaptor_node_s ) \
    {aware_t _;bhvm_holor_s* axon;bhvm_holor_s* grad;}; \
  void bhpt_adaptor_node_s_get_min_max( const bhpt_adaptor_node_s* o, f3_t* min, f3_t* max );
#define TYPEOF_bhpt_adaptor_probe_s 1084832918
#define BETH_EXPAND_ITEM_bhpt_adaptor_probe_s \
  BCORE_DECLARE_OBJECT( bhpt_adaptor_probe_s ) \
    {aware_t _;BCORE_ARRAY_DYN_SOLID_STATIC_S( bhpt_adaptor_node_s, );}; \
  void bhpt_adaptor_probe_s_get_min_max( const bhpt_adaptor_probe_s* o, f3_t* min, f3_t* max ); \
  static inline bhpt_adaptor_probe_s* bhpt_adaptor_probe_s_set_space( bhpt_adaptor_probe_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_bhpt_adaptor_probe_s, ( bcore_array* )o, size ); return o; } \
  static inline bhpt_adaptor_probe_s* bhpt_adaptor_probe_s_set_size( bhpt_adaptor_probe_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_bhpt_adaptor_probe_s, ( bcore_array* )o, size ); return o; } \
  static inline bhpt_adaptor_probe_s* bhpt_adaptor_probe_s_clear( bhpt_adaptor_probe_s* o ) { bcore_array_t_set_space( TYPEOF_bhpt_adaptor_probe_s, ( bcore_array* )o, 0 ); return o; } \
  static inline bhpt_adaptor_node_s* bhpt_adaptor_probe_s_push_c( bhpt_adaptor_probe_s* o, const bhpt_adaptor_node_s* v ) { bcore_array_t_push( TYPEOF_bhpt_adaptor_probe_s, ( bcore_array* )o, sr_twc( TYPEOF_bhpt_adaptor_node_s, v ) ); return &o->data[ o->size - 1 ]; } \
  static inline bhpt_adaptor_node_s* bhpt_adaptor_probe_s_push_d( bhpt_adaptor_probe_s* o,       bhpt_adaptor_node_s* v ) { bcore_array_t_push( TYPEOF_bhpt_adaptor_probe_s, ( bcore_array* )o, sr_tsd( TYPEOF_bhpt_adaptor_node_s, v ) ); return &o->data[ o->size - 1 ]; } \
  static inline bhpt_adaptor_node_s* bhpt_adaptor_probe_s_push( bhpt_adaptor_probe_s* o ) \
  { \
      bcore_array_t_push( TYPEOF_bhpt_adaptor_probe_s, ( bcore_array* )o, sr_null() ); \
      return &o->data[ o->size - 1 ]; \
  }
#define TYPEOF_bhpt_adaptor_adl_s 2354735811
#define BETH_EXPAND_ITEM_bhpt_adaptor_adl_s \
  BCORE_DECLARE_OBJECT( bhpt_adaptor_adl_s ) \
    {aware_t _;BCORE_ARRAY_DYN_LINK_STATIC_S( bhpt_adaptor, );}; \
  static inline bhpt_adaptor_adl_s* bhpt_adaptor_adl_s_set_space( bhpt_adaptor_adl_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_bhpt_adaptor_adl_s, ( bcore_array* )o, size ); return o; } \
  static inline bhpt_adaptor_adl_s* bhpt_adaptor_adl_s_set_size( bhpt_adaptor_adl_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_bhpt_adaptor_adl_s, ( bcore_array* )o, size ); return o; } \
  static inline bhpt_adaptor_adl_s* bhpt_adaptor_adl_s_clear( bhpt_adaptor_adl_s* o ) { bcore_array_t_set_space( TYPEOF_bhpt_adaptor_adl_s, ( bcore_array* )o, 0 ); return o; } \
  static inline bhpt_adaptor* bhpt_adaptor_adl_s_push_c( bhpt_adaptor_adl_s* o, const bhpt_adaptor* v ) { bcore_array_t_push( TYPEOF_bhpt_adaptor_adl_s, ( bcore_array* )o, sr_awc( v ) ); return o->data[ o->size - 1 ]; } \
  static inline bhpt_adaptor* bhpt_adaptor_adl_s_push_d( bhpt_adaptor_adl_s* o,       bhpt_adaptor* v ) { bcore_array_t_push( TYPEOF_bhpt_adaptor_adl_s, ( bcore_array* )o, sr_asd( v ) ); return o->data[ o->size - 1 ]; } \
  static inline bhpt_adaptor* bhpt_adaptor_adl_s_push_t( bhpt_adaptor_adl_s* o, tp_t t ) \
  { \
      bcore_trait_assert_satisfied_type( TYPEOF_bhpt_adaptor, t ); \
      bcore_array_t_push( TYPEOF_bhpt_adaptor_adl_s, ( bcore_array* )o, sr_t_create( t ) ); \
      return o->data[ o->size - 1 ]; \
  }
#define BETH_EXPAND_GROUP_bhpt_adaptor \
  BCORE_FORWARD_OBJECT( bhpt_adaptor ); \
  BCORE_FORWARD_OBJECT( bhpt_adaptor_node_s ); \
  BCORE_FORWARD_OBJECT( bhpt_adaptor_probe_s ); \
  BCORE_FORWARD_OBJECT( bhpt_adaptor_adl_s ); \
  typedef void (*bhpt_adaptor_reset)( bhpt_adaptor* o ); \
  typedef void (*bhpt_adaptor_adapt)( bhpt_adaptor* o, const bhpt_adaptor_node_s* node ); \
  BCORE_DECLARE_SPECT( bhpt_adaptor ) \
  { \
      bcore_spect_header_s header; \
      bhpt_adaptor_reset reset; \
      bhpt_adaptor_adapt adapt; \
  }; \
  static inline bhpt_adaptor* bhpt_adaptor_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_bhpt_adaptor, t ); return ( bhpt_adaptor* )bcore_inst_t_create( t ); } \
  static inline bl_t bhpt_adaptor_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_bhpt_adaptor ); } \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bhpt_adaptor ) \
  static inline bl_t bhpt_adaptor_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_bhpt_adaptor ); } \
  BETH_EXPAND_ITEM_bhpt_adaptor_node_s \
  BETH_EXPAND_ITEM_bhpt_adaptor_probe_s \
  static inline void bhpt_adaptor_a_reset( bhpt_adaptor* o ) { const bhpt_adaptor_s* p = bhpt_adaptor_s_get_aware( o ); assert( p->reset ); p->reset( o ); } \
  static inline bl_t bhpt_adaptor_a_defines_reset( const bhpt_adaptor* o ) { return bhpt_adaptor_s_get_aware( o )->reset != NULL; } \
  static inline void bhpt_adaptor_a_adapt( bhpt_adaptor* o, const bhpt_adaptor_node_s* node ) { const bhpt_adaptor_s* p = bhpt_adaptor_s_get_aware( o ); assert( p->adapt ); p->adapt( o, node ); } \
  static inline bl_t bhpt_adaptor_a_defines_adapt( const bhpt_adaptor* o ) { return bhpt_adaptor_s_get_aware( o )->adapt != NULL; } \
  BETH_EXPAND_ITEM_bhpt_adaptor_adl_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhpt_adaptive

#define TYPEOF_bhpt_adaptive 3280124826
#define TYPEOF_bhpt_adaptive_s 2158321412
#define BETH_EXPAND_GROUP_bhpt_adaptive \
  BCORE_FORWARD_OBJECT( bhpt_adaptive ); \
  typedef bhvm_holor_s* (*bhpt_adaptive_get_format_en)( const bhpt_adaptive* o, bhvm_holor_s* format ); \
  typedef bhvm_holor_s* (*bhpt_adaptive_get_format_ex)( const bhpt_adaptive* o, bhvm_holor_s* format ); \
  typedef void (*bhpt_adaptive_axon_pass)( bhpt_adaptive* o, const bhvm_holor_s* ax_en, bhvm_holor_s* ax_ex ); \
  typedef void (*bhpt_adaptive_dendrite_pass)( bhpt_adaptive* o, const bhvm_holor_s* ag_ex, bhvm_holor_s* ag_en ); \
  typedef void (*bhpt_adaptive_cyclic_reset)( bhpt_adaptive* o ); \
  typedef bhpt_adaptor_probe_s* (*bhpt_adaptive_get_adaptor_probe)( const bhpt_adaptive* o, bhpt_adaptor_probe_s* probe ); \
  typedef void (*bhpt_adaptive_status_to_sink)( const bhpt_adaptive* o, sz_t verbosity, bcore_sink* sink ); \
  BCORE_DECLARE_SPECT( bhpt_adaptive ) \
  { \
      bcore_spect_header_s header; \
      bhpt_adaptive_get_format_en get_format_en; \
      bhpt_adaptive_get_format_ex get_format_ex; \
      bhpt_adaptive_axon_pass axon_pass; \
      bhpt_adaptive_dendrite_pass dendrite_pass; \
      bhpt_adaptive_cyclic_reset cyclic_reset; \
      bhpt_adaptive_get_adaptor_probe get_adaptor_probe; \
      bhpt_adaptive_status_to_sink status_to_sink; \
  }; \
  static inline bhpt_adaptive* bhpt_adaptive_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_bhpt_adaptive, t ); return ( bhpt_adaptive* )bcore_inst_t_create( t ); } \
  static inline bl_t bhpt_adaptive_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_bhpt_adaptive ); } \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bhpt_adaptive ) \
  static inline bl_t bhpt_adaptive_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_bhpt_adaptive ); } \
  static inline bhvm_holor_s* bhpt_adaptive_a_get_format_en( const bhpt_adaptive* o, bhvm_holor_s* format ) { const bhpt_adaptive_s* p = bhpt_adaptive_s_get_aware( o ); assert( p->get_format_en ); return p->get_format_en( o, format ); } \
  static inline bl_t bhpt_adaptive_a_defines_get_format_en( const bhpt_adaptive* o ) { return true; } \
  static inline bhvm_holor_s* bhpt_adaptive_a_get_format_ex( const bhpt_adaptive* o, bhvm_holor_s* format ) { const bhpt_adaptive_s* p = bhpt_adaptive_s_get_aware( o ); assert( p->get_format_ex ); return p->get_format_ex( o, format ); } \
  static inline bl_t bhpt_adaptive_a_defines_get_format_ex( const bhpt_adaptive* o ) { return true; } \
  static inline void bhpt_adaptive_a_axon_pass( bhpt_adaptive* o, const bhvm_holor_s* ax_en, bhvm_holor_s* ax_ex ) { const bhpt_adaptive_s* p = bhpt_adaptive_s_get_aware( o ); assert( p->axon_pass ); p->axon_pass( o, ax_en, ax_ex ); } \
  static inline bl_t bhpt_adaptive_a_defines_axon_pass( const bhpt_adaptive* o ) { return true; } \
  static inline void bhpt_adaptive_axon_pass__( bhpt_adaptive* o, const bhvm_holor_s* ax_en, bhvm_holor_s* ax_ex ){} \
  static inline void bhpt_adaptive_a_dendrite_pass( bhpt_adaptive* o, const bhvm_holor_s* ag_ex, bhvm_holor_s* ag_en ) { const bhpt_adaptive_s* p = bhpt_adaptive_s_get_aware( o ); assert( p->dendrite_pass ); p->dendrite_pass( o, ag_ex, ag_en ); } \
  static inline bl_t bhpt_adaptive_a_defines_dendrite_pass( const bhpt_adaptive* o ) { return true; } \
  static inline void bhpt_adaptive_dendrite_pass__( bhpt_adaptive* o, const bhvm_holor_s* ag_ex, bhvm_holor_s* ag_en ){} \
  static inline void bhpt_adaptive_a_cyclic_reset( bhpt_adaptive* o ) { const bhpt_adaptive_s* p = bhpt_adaptive_s_get_aware( o ); assert( p->cyclic_reset ); p->cyclic_reset( o ); } \
  static inline bl_t bhpt_adaptive_a_defines_cyclic_reset( const bhpt_adaptive* o ) { return true; } \
  static inline void bhpt_adaptive_cyclic_reset__( bhpt_adaptive* o ){} \
  static inline bhpt_adaptor_probe_s* bhpt_adaptive_a_get_adaptor_probe( const bhpt_adaptive* o, bhpt_adaptor_probe_s* probe ) { const bhpt_adaptive_s* p = bhpt_adaptive_s_get_aware( o ); assert( p->get_adaptor_probe ); return p->get_adaptor_probe( o, probe ); } \
  static inline bl_t bhpt_adaptive_a_defines_get_adaptor_probe( const bhpt_adaptive* o ) { return true; } \
  static inline bhpt_adaptor_probe_s* bhpt_adaptive_get_adaptor_probe__( const bhpt_adaptive* o, bhpt_adaptor_probe_s* probe ){return probe;} \
  static inline void bhpt_adaptive_a_status_to_sink( const bhpt_adaptive* o, sz_t verbosity, bcore_sink* sink ) { const bhpt_adaptive_s* p = bhpt_adaptive_s_get_aware( o ); assert( p->status_to_sink ); p->status_to_sink( o, verbosity, sink ); } \
  static inline bl_t bhpt_adaptive_a_defines_status_to_sink( const bhpt_adaptive* o ) { return true; } \
  static inline void bhpt_adaptive_status_to_sink__( const bhpt_adaptive* o, sz_t verbosity, bcore_sink* sink ){if( verbosity > 0 ) bcore_txt_ml_a_to_sink( o, sink );}

//----------------------------------------------------------------------------------------------------------------------
// group: bhpt_builder

#define TYPEOF_bhpt_builder 2820951603
#define TYPEOF_bhpt_builder_s 1545361237
#define BETH_EXPAND_GROUP_bhpt_builder \
  BCORE_FORWARD_OBJECT( bhpt_builder ); \
  typedef void (*bhpt_builder_set_format_en)( bhpt_builder* o, const bhvm_holor_s* format ); \
  typedef void (*bhpt_builder_set_format_ex)( bhpt_builder* o, const bhvm_holor_s* format ); \
  typedef bhpt_adaptive* (*bhpt_builder_create_adaptive)( const bhpt_builder* o ); \
  BCORE_DECLARE_SPECT( bhpt_builder ) \
  { \
      bcore_spect_header_s header; \
      bhpt_builder_set_format_en set_format_en; \
      bhpt_builder_set_format_ex set_format_ex; \
      bhpt_builder_create_adaptive create_adaptive; \
  }; \
  static inline bhpt_builder* bhpt_builder_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_bhpt_builder, t ); return ( bhpt_builder* )bcore_inst_t_create( t ); } \
  static inline bl_t bhpt_builder_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_bhpt_builder ); } \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bhpt_builder ) \
  static inline bl_t bhpt_builder_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_bhpt_builder ); } \
  static inline void bhpt_builder_a_set_format_en( bhpt_builder* o, const bhvm_holor_s* format ) { const bhpt_builder_s* p = bhpt_builder_s_get_aware( o ); assert( p->set_format_en ); p->set_format_en( o, format ); } \
  static inline bl_t bhpt_builder_a_defines_set_format_en( const bhpt_builder* o ) { return true; } \
  static inline void bhpt_builder_a_set_format_ex( bhpt_builder* o, const bhvm_holor_s* format ) { const bhpt_builder_s* p = bhpt_builder_s_get_aware( o ); assert( p->set_format_ex ); p->set_format_ex( o, format ); } \
  static inline bl_t bhpt_builder_a_defines_set_format_ex( const bhpt_builder* o ) { return true; } \
  static inline bhpt_adaptive* bhpt_builder_a_create_adaptive( const bhpt_builder* o ) { const bhpt_builder_s* p = bhpt_builder_s_get_aware( o ); assert( p->create_adaptive ); return p->create_adaptive( o ); } \
  static inline bl_t bhpt_builder_a_defines_create_adaptive( const bhpt_builder* o ) { return true; }

//----------------------------------------------------------------------------------------------------------------------
// group: bhpt_tutor

#define TYPEOF_bhpt_tutor 2207603144
#define TYPEOF_bhpt_tutor_s 1394724786
#define BETH_EXPAND_GROUP_bhpt_tutor \
  BCORE_FORWARD_OBJECT( bhpt_tutor ); \
  typedef bhpt_adaptive* (*bhpt_tutor_create_adaptive)( const bhpt_tutor* o, const bhpt_builder* builder ); \
  typedef void (*bhpt_tutor_reset)( bhpt_tutor* o ); \
  typedef void (*bhpt_tutor_prime)( bhpt_tutor* o, bhpt_adaptive* adaptive ); \
  typedef void (*bhpt_tutor_test)( const bhpt_tutor* o, const bhpt_adaptive* adaptive, sz_t verbosity, bcore_sink* log ); \
  BCORE_DECLARE_SPECT( bhpt_tutor ) \
  { \
      bcore_spect_header_s header; \
      bhpt_tutor_create_adaptive create_adaptive; \
      bhpt_tutor_reset reset; \
      bhpt_tutor_prime prime; \
      bhpt_tutor_test test; \
  }; \
  static inline bhpt_tutor* bhpt_tutor_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_bhpt_tutor, t ); return ( bhpt_tutor* )bcore_inst_t_create( t ); } \
  static inline bl_t bhpt_tutor_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_bhpt_tutor ); } \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bhpt_tutor ) \
  static inline bl_t bhpt_tutor_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_bhpt_tutor ); } \
  static inline bhpt_adaptive* bhpt_tutor_a_create_adaptive( const bhpt_tutor* o, const bhpt_builder* builder ) { const bhpt_tutor_s* p = bhpt_tutor_s_get_aware( o ); assert( p->create_adaptive ); return p->create_adaptive( o, builder ); } \
  static inline bl_t bhpt_tutor_a_defines_create_adaptive( const bhpt_tutor* o ) { return true; } \
  static inline void bhpt_tutor_a_reset( bhpt_tutor* o ) { const bhpt_tutor_s* p = bhpt_tutor_s_get_aware( o ); assert( p->reset ); p->reset( o ); } \
  static inline bl_t bhpt_tutor_a_defines_reset( const bhpt_tutor* o ) { return true; } \
  static inline void bhpt_tutor_reset__( bhpt_tutor* o ){} \
  static inline void bhpt_tutor_a_prime( bhpt_tutor* o, bhpt_adaptive* adaptive ) { const bhpt_tutor_s* p = bhpt_tutor_s_get_aware( o ); assert( p->prime ); p->prime( o, adaptive ); } \
  static inline bl_t bhpt_tutor_a_defines_prime( const bhpt_tutor* o ) { return true; } \
  static inline void bhpt_tutor_prime__( bhpt_tutor* o, bhpt_adaptive* adaptive ){} \
  static inline void bhpt_tutor_a_test( const bhpt_tutor* o, const bhpt_adaptive* adaptive, sz_t verbosity, bcore_sink* log ) { const bhpt_tutor_s* p = bhpt_tutor_s_get_aware( o ); assert( p->test ); p->test( o, adaptive, verbosity, log ); } \
  static inline bl_t bhpt_tutor_a_defines_test( const bhpt_tutor* o ) { return true; } \
  static inline void bhpt_tutor_test__( const bhpt_tutor* o, const bhpt_adaptive* adaptive, sz_t verbosity, bcore_sink* log ){}

/**********************************************************************************************************************/
// source: bhpt_frame.h

//----------------------------------------------------------------------------------------------------------------------
// group: bhpt_frame

#define TYPEOF_bhpt_frame 3994553873
#define TYPEOF_bhpt_frame_s 3192581947
#define TYPEOF_bhpt_frame_state_s 2524905321
#define BETH_EXPAND_ITEM_bhpt_frame_state_s \
  BCORE_DECLARE_OBJECT( bhpt_frame_state_s ) \
    {aware_t _;sz_t cycle_number;sz_t last_cycle_adapt;sz_t last_cycle_test;sz_t last_cycle_backup;bhpt_adaptive* adaptive;bhpt_adaptor_adl_s* adaptor_adl;};
#define TYPEOF_bhpt_frame_s 3192581947
#define BETH_EXPAND_ITEM_bhpt_frame_s \
  BCORE_DECLARE_OBJECT( bhpt_frame_s ) \
    {aware_t _;bhpt_tutor* tutor;bhpt_builder* builder;bhpt_adaptor* adaptor;sz_t cycle_adapt;sz_t cycle_test;sz_t cycle_backup;sz_t cycle_finish;sz_t verbosity;bhpt_frame_state_s* state;st_s state_path;bcore_sink* log;}; \
  s2_t bhpt_frame_s_main( bhpt_frame_s* o, const bcore_arr_st_s* args );
#define BETH_EXPAND_GROUP_bhpt_frame \
  BCORE_FORWARD_OBJECT( bhpt_frame ); \
  BCORE_FORWARD_OBJECT( bhpt_frame_state_s ); \
  BCORE_FORWARD_OBJECT( bhpt_frame_s ); \
  BETH_EXPAND_ITEM_bhpt_frame_state_s \
  BETH_EXPAND_ITEM_bhpt_frame_s

/**********************************************************************************************************************/
// source: bhpt_adaptor.h

//----------------------------------------------------------------------------------------------------------------------
// group: bhpt_adaptor_stamp

#define TYPEOF_bhpt_adaptor_stamp 2535804545
#define TYPEOF_bhpt_adaptor_stamp_s 3869143851
#define TYPEOF_bhpt_adaptor_epsilon_s 526346998
#define BETH_EXPAND_ITEM_bhpt_adaptor_epsilon_s \
  BCORE_DECLARE_OBJECT( bhpt_adaptor_epsilon_s ) \
    {aware_t _;f3_t epsilon;}; \
  static inline void bhpt_adaptor_epsilon_s_reset( bhpt_adaptor_epsilon_s* o ){} \
  void bhpt_adaptor_epsilon_s_adapt( bhpt_adaptor_epsilon_s* o, const bhpt_adaptor_node_s* node );
#define TYPEOF_bhpt_adaptor_list_s 3111754760
#define BETH_EXPAND_ITEM_bhpt_adaptor_list_s \
  BCORE_DECLARE_OBJECT( bhpt_adaptor_list_s ) \
    {aware_t _;bhpt_adaptor_adl_s adl;}; \
  static inline void bhpt_adaptor_list_s_reset( bhpt_adaptor_list_s* o ){BFOR_EACH( i, &o->adl ) bhpt_adaptor_a_reset( o->adl.data[ i ] );} \
  static inline void bhpt_adaptor_list_s_adapt( bhpt_adaptor_list_s* o, const bhpt_adaptor_node_s* node ){BFOR_EACH( i, &o->adl ) bhpt_adaptor_a_adapt( o->adl.data[ i ], node );}
#define BETH_EXPAND_GROUP_bhpt_adaptor_stamp \
  BCORE_FORWARD_OBJECT( bhpt_adaptor_stamp ); \
  BCORE_FORWARD_OBJECT( bhpt_adaptor_epsilon_s ); \
  BCORE_FORWARD_OBJECT( bhpt_adaptor_list_s ); \
  BETH_EXPAND_ITEM_bhpt_adaptor_epsilon_s \
  BETH_EXPAND_ITEM_bhpt_adaptor_list_s

/**********************************************************************************************************************/
// source: bhpt_tutor.h

//----------------------------------------------------------------------------------------------------------------------
// group: bhpt_tutor_stamp

#define TYPEOF_bhpt_tutor_stamp 4217977908
#define TYPEOF_bhpt_tutor_stamp_s 818550566
#define TYPEOF_bhpt_tutor_sine_random_s 4152364972
#define BETH_EXPAND_ITEM_bhpt_tutor_sine_random_s \
  BCORE_DECLARE_OBJECT( bhpt_tutor_sine_random_s ) \
    {aware_t _;sz_t input_size;f3_t pos_tgt;f3_t neg_tgt;u2_t rval_prime;u2_t rval_test;sz_t test_size;bcore_mutex_s mutex;}; \
  void bhpt_tutor_sine_random_s_reset( bhpt_tutor_sine_random_s* o ); \
  bhpt_adaptive* bhpt_tutor_sine_random_s_create_adaptive( const bhpt_tutor_sine_random_s* o, const bhpt_builder* builder ); \
  void bhpt_tutor_sine_random_s_prime( bhpt_tutor_sine_random_s* o, bhpt_adaptive* adaptive ); \
  void bhpt_tutor_sine_random_s_test( const bhpt_tutor_sine_random_s* o, const bhpt_adaptive* adaptive, sz_t verbosity, bcore_sink* log );
#define BETH_EXPAND_GROUP_bhpt_tutor_stamp \
  BCORE_FORWARD_OBJECT( bhpt_tutor_stamp ); \
  BCORE_FORWARD_OBJECT( bhpt_tutor_sine_random_s ); \
  BETH_EXPAND_ITEM_bhpt_tutor_sine_random_s

/**********************************************************************************************************************/

vd_t bhpt_planted_signal_handler( const bcore_signal_s* o );

#endif // BHPT_PLANTED_H
