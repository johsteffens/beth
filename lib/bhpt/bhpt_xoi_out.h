/** This file was generated from beth-plant source code.
 *  Compiling Agent : xoico_compiler (C) 2020 J.B.Steffens
 *  Last File Update: 2020-08-05T12:45:03Z
 *
 *  Copyright and License of this File:
 *
 *  Generated code inherits the copyright and license of the underlying beth-plant source code.
 *  Source code defining this file is distributed across following files:
 *
 *  bhpt_sketch.h
 *  bhpt_frame.h
 *  bhpt_adaptor.h
 *  bhpt_tutor_sampler.h
 *  bhpt_tutor_language.h
 *
 */

#ifndef BHPT_XOI_OUT_H
#define BHPT_XOI_OUT_H

#include "bcore_control.h"

//To force a rebuild of this target by the plant-compiler, reset the hash key value below to 0.
#define HKEYOF_bhpt_xoi_out 687825138

#define TYPEOF_bhpt_xoi_out 4205361423

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
  void bhpt_adaptor_node_s_get_min_max( const bhpt_adaptor_node_s* o, f3_t* min, f3_t* max ); \
  void bhpt_adaptor_node_s_acc_stats( const bhpt_adaptor_node_s* o, bhvm_stats_s* axon, bhvm_stats_s* grad ); \
  static inline void bhpt_adaptor_node_s_zro_grad( bhpt_adaptor_node_s* o ){bhvm_value_s_zro( &o->grad->v );} \
  static inline void bhpt_adaptor_node_s_acc_grad( bhpt_adaptor_node_s* o, const bhpt_adaptor_node_s* src ){assert( bhvm_shape_s_is_equal( &o->grad->s, &src->grad->s ) ); bhvm_value_s_acc( &o->grad->v, &src->grad->v );} \
  static inline void bhpt_adaptor_node_s_rebind_axon( bhpt_adaptor_node_s* o, bhpt_adaptor_node_s* src ){assert( bhvm_shape_s_is_equal( &o->axon->s, &src->axon->s ) ); bhvm_value_s_weak( &o->axon->v, &src->axon->v );}
#define TYPEOF_bhpt_adaptor_probe_s 1084832918
#define BETH_EXPAND_ITEM_bhpt_adaptor_probe_s \
  BCORE_DECLARE_OBJECT( bhpt_adaptor_probe_s ) \
    {aware_t _;BCORE_ARRAY_DYN_SOLID_STATIC_S( bhpt_adaptor_node_s, );}; \
  void bhpt_adaptor_probe_s_get_min_max( const bhpt_adaptor_probe_s* o, f3_t* min, f3_t* max ); \
  static inline void bhpt_adaptor_probe_s_acc_stats( const bhpt_adaptor_probe_s* o, bhvm_stats_s* axon, bhvm_stats_s* grad ){BFOR_EACH( i, o ) bhpt_adaptor_node_s_acc_stats( &o->data[ i ], axon, grad );} \
  static inline void bhpt_adaptor_probe_s_zro_grad( bhpt_adaptor_probe_s* o ){BFOR_EACH( i, o ) bhpt_adaptor_node_s_zro_grad( &o->data[ i ] );} \
  static inline void bhpt_adaptor_probe_s_acc_grad( bhpt_adaptor_probe_s* o, const bhpt_adaptor_probe_s* src ){assert( o->size == src->size ); BFOR_EACH( i, o ) bhpt_adaptor_node_s_acc_grad( &o->data[ i ], &src->data[ i ] );} \
  static inline void bhpt_adaptor_probe_s_rebind_axon( bhpt_adaptor_probe_s* o, bhpt_adaptor_probe_s* src ){assert( o->size == src->size ); BFOR_EACH( i, o ) bhpt_adaptor_node_s_rebind_axon( &o->data[ i ], &src->data[ i ] );} \
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
  typedef void (*bhpt_adaptive_rebind_holors)( bhpt_adaptive* o ); \
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
      bhpt_adaptive_rebind_holors rebind_holors; \
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
  static inline void bhpt_adaptive_a_rebind_holors( bhpt_adaptive* o ) { const bhpt_adaptive_s* p = bhpt_adaptive_s_get_aware( o ); assert( p->rebind_holors ); p->rebind_holors( o ); } \
  static inline bl_t bhpt_adaptive_a_defines_rebind_holors( const bhpt_adaptive* o ) { return bhpt_adaptive_s_get_aware( o )->rebind_holors != NULL; } \
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
  typedef bhpt_adaptive* (*bhpt_tutor_create_adaptive)( const bhpt_tutor* o ); \
  typedef bhpt_adaptor* (*bhpt_tutor_create_adaptor)( const bhpt_tutor* o ); \
  typedef void (*bhpt_tutor_reset)( bhpt_tutor* o ); \
  typedef void (*bhpt_tutor_prime)( bhpt_tutor* o, bhpt_adaptive* adaptive ); \
  typedef void (*bhpt_tutor_test)( bhpt_tutor* o, const bhpt_adaptive* adaptive, sz_t verbosity, bcore_sink* log ); \
  typedef void (*bhpt_tutor_status_to_sink)( const bhpt_tutor* o, sz_t verbosity, bcore_sink* sink ); \
  BCORE_DECLARE_SPECT( bhpt_tutor ) \
  { \
      bcore_spect_header_s header; \
      bhpt_tutor_create_adaptive create_adaptive; \
      bhpt_tutor_create_adaptor create_adaptor; \
      bhpt_tutor_reset reset; \
      bhpt_tutor_prime prime; \
      bhpt_tutor_test test; \
      bhpt_tutor_status_to_sink status_to_sink; \
  }; \
  static inline bhpt_tutor* bhpt_tutor_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_bhpt_tutor, t ); return ( bhpt_tutor* )bcore_inst_t_create( t ); } \
  static inline bl_t bhpt_tutor_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_bhpt_tutor ); } \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bhpt_tutor ) \
  static inline bl_t bhpt_tutor_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_bhpt_tutor ); } \
  static inline bhpt_adaptive* bhpt_tutor_a_create_adaptive( const bhpt_tutor* o ) { const bhpt_tutor_s* p = bhpt_tutor_s_get_aware( o ); assert( p->create_adaptive ); return p->create_adaptive( o ); } \
  static inline bl_t bhpt_tutor_a_defines_create_adaptive( const bhpt_tutor* o ) { return true; } \
  static inline bhpt_adaptor* bhpt_tutor_a_create_adaptor( const bhpt_tutor* o ) { const bhpt_tutor_s* p = bhpt_tutor_s_get_aware( o ); assert( p->create_adaptor ); return p->create_adaptor( o ); } \
  static inline bl_t bhpt_tutor_a_defines_create_adaptor( const bhpt_tutor* o ) { return true; } \
  static inline void bhpt_tutor_a_reset( bhpt_tutor* o ) { const bhpt_tutor_s* p = bhpt_tutor_s_get_aware( o ); assert( p->reset ); p->reset( o ); } \
  static inline bl_t bhpt_tutor_a_defines_reset( const bhpt_tutor* o ) { return true; } \
  static inline void bhpt_tutor_reset__( bhpt_tutor* o ){} \
  static inline void bhpt_tutor_a_prime( bhpt_tutor* o, bhpt_adaptive* adaptive ) { const bhpt_tutor_s* p = bhpt_tutor_s_get_aware( o ); assert( p->prime ); p->prime( o, adaptive ); } \
  static inline bl_t bhpt_tutor_a_defines_prime( const bhpt_tutor* o ) { return true; } \
  static inline void bhpt_tutor_prime__( bhpt_tutor* o, bhpt_adaptive* adaptive ){} \
  static inline void bhpt_tutor_a_test( bhpt_tutor* o, const bhpt_adaptive* adaptive, sz_t verbosity, bcore_sink* log ) { const bhpt_tutor_s* p = bhpt_tutor_s_get_aware( o ); assert( p->test ); p->test( o, adaptive, verbosity, log ); } \
  static inline bl_t bhpt_tutor_a_defines_test( const bhpt_tutor* o ) { return true; } \
  static inline void bhpt_tutor_test__( bhpt_tutor* o, const bhpt_adaptive* adaptive, sz_t verbosity, bcore_sink* log ){} \
  static inline void bhpt_tutor_a_status_to_sink( const bhpt_tutor* o, sz_t verbosity, bcore_sink* sink ) { const bhpt_tutor_s* p = bhpt_tutor_s_get_aware( o ); assert( p->status_to_sink ); p->status_to_sink( o, verbosity, sink ); } \
  static inline bl_t bhpt_tutor_a_defines_status_to_sink( const bhpt_tutor* o ) { return true; } \
  static inline void bhpt_tutor_status_to_sink__( const bhpt_tutor* o, sz_t verbosity, bcore_sink* sink ){if( verbosity > 0 ) bcore_txt_ml_a_to_sink( o, sink );}

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
    {aware_t _;bhpt_tutor* tutor;bhpt_frame_thread_base_s* thread_base;bcore_main_frame_s* main_frame;sz_t threads;sz_t cycle_adapt;sz_t cycle_test;sz_t cycle_backup;sz_t cycle_finish;sz_t verbosity;bhpt_frame_state_s* state;bhvm_stats_s stats_grad;st_s state_path;bcore_sink* log;}; \
  er_t bhpt_frame_s_main( bhpt_frame_s* o, bcore_main_frame_s* frame ); \
  static inline bl_t bhpt_frame_s_exit_required( const bhpt_frame_s* o ){return bcore_main_frame_s_exit_required( o->main_frame );}
#define BETH_EXPAND_GROUP_bhpt_frame \
  BCORE_FORWARD_OBJECT( bhpt_frame ); \
  BCORE_FORWARD_OBJECT( bhpt_frame_thread ); \
  BCORE_FORWARD_OBJECT( bhpt_frame_state_s ); \
  BCORE_FORWARD_OBJECT( bhpt_frame_s ); \
  BETH_EXPAND_GROUP_bhpt_frame_thread \
  BETH_EXPAND_ITEM_bhpt_frame_state_s \
  BETH_EXPAND_ITEM_bhpt_frame_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhpt_frame_thread

#define TYPEOF_bhpt_frame_thread 3452286286
#define TYPEOF_bhpt_frame_thread_s 2072199184
#define TYPEOF_bhpt_frame_thread_share_s 4277768516
#define BETH_EXPAND_ITEM_bhpt_frame_thread_share_s \
  BCORE_DECLARE_OBJECT( bhpt_frame_thread_share_s ) \
    {aware_t _;bhpt_tutor* tutor;bhpt_adaptive* adaptive;bcore_condition_s* condition_item;bcore_mutex_s* mutex;sz_t finished_count;};
#define TYPEOF_bhpt_frame_thread_item_s 2009562070
#define BETH_EXPAND_ITEM_bhpt_frame_thread_item_s \
  BCORE_DECLARE_OBJECT( bhpt_frame_thread_item_s ) \
    {aware_t _;bl_t running;sz_t prime_cycles;bcore_thread_s* thread;bcore_mutex_s* mutex;bhpt_frame_thread_share_s* share;bhpt_adaptive* adaptive;}; \
  static inline void bhpt_frame_thread_item_s_down_e( bhpt_frame_thread_item_s* o ){ASSERT( !o->running );} \
  vd_t bhpt_frame_thread_item_s_loop( bhpt_frame_thread_item_s* o ); \
  void bhpt_frame_thread_item_s_loop_enter( bhpt_frame_thread_item_s* o ); \
  void bhpt_frame_thread_item_s_loop_exit( bhpt_frame_thread_item_s* o ); \
  static inline void bhpt_frame_thread_item_s_wait_while_locked( bhpt_frame_thread_item_s* o ){bcore_mutex_s_lock( o->mutex ); bcore_mutex_s_unlock( o->mutex );}
#define TYPEOF_bhpt_frame_thread_ads_s 1091463513
#define BETH_EXPAND_ITEM_bhpt_frame_thread_ads_s \
  BCORE_DECLARE_OBJECT( bhpt_frame_thread_ads_s ) \
    {aware_t _;BCORE_ARRAY_DYN_SOLID_STATIC_S( bhpt_frame_thread_item_s, );}; \
  static inline bhpt_frame_thread_ads_s* bhpt_frame_thread_ads_s_set_space( bhpt_frame_thread_ads_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_bhpt_frame_thread_ads_s, ( bcore_array* )o, size ); return o; } \
  static inline bhpt_frame_thread_ads_s* bhpt_frame_thread_ads_s_set_size( bhpt_frame_thread_ads_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_bhpt_frame_thread_ads_s, ( bcore_array* )o, size ); return o; } \
  static inline bhpt_frame_thread_ads_s* bhpt_frame_thread_ads_s_clear( bhpt_frame_thread_ads_s* o ) { bcore_array_t_set_space( TYPEOF_bhpt_frame_thread_ads_s, ( bcore_array* )o, 0 ); return o; } \
  static inline bhpt_frame_thread_item_s* bhpt_frame_thread_ads_s_push_c( bhpt_frame_thread_ads_s* o, const bhpt_frame_thread_item_s* v ) { bcore_array_t_push( TYPEOF_bhpt_frame_thread_ads_s, ( bcore_array* )o, sr_twc( TYPEOF_bhpt_frame_thread_item_s, v ) ); return &o->data[ o->size - 1 ]; } \
  static inline bhpt_frame_thread_item_s* bhpt_frame_thread_ads_s_push_d( bhpt_frame_thread_ads_s* o,       bhpt_frame_thread_item_s* v ) { bcore_array_t_push( TYPEOF_bhpt_frame_thread_ads_s, ( bcore_array* )o, sr_tsd( TYPEOF_bhpt_frame_thread_item_s, v ) ); return &o->data[ o->size - 1 ]; } \
  static inline bhpt_frame_thread_item_s* bhpt_frame_thread_ads_s_push( bhpt_frame_thread_ads_s* o ) \
  { \
      bcore_array_t_push( TYPEOF_bhpt_frame_thread_ads_s, ( bcore_array* )o, sr_null() ); \
      return &o->data[ o->size - 1 ]; \
  }
#define TYPEOF_bhpt_frame_thread_base_s 21291312
#define BETH_EXPAND_ITEM_bhpt_frame_thread_base_s \
  BCORE_DECLARE_OBJECT( bhpt_frame_thread_base_s ) \
    {aware_t _;bhpt_frame_thread_ads_s ads;bhpt_frame_thread_share_s* share;}; \
  void bhpt_frame_thread_base_s_tsetup( bhpt_frame_thread_base_s* o, sz_t threads, bhpt_adaptive* adaptive, bhpt_tutor* tutor ); \
  void bhpt_frame_thread_base_s_tdown( bhpt_frame_thread_base_s* o ); \
  void bhpt_frame_thread_base_s_run( bhpt_frame_thread_base_s* o, sz_t cycles_per_thread ); \
  static inline void bhpt_frame_thread_base_s_down_e( bhpt_frame_thread_base_s* o ){bhpt_frame_thread_base_s_tdown( o );} \
  static inline void bhpt_frame_thread_base_s_copy_e( bhpt_frame_thread_base_s* o ){bhpt_frame_thread_base_s_tdown( o );}
#define BETH_EXPAND_GROUP_bhpt_frame_thread \
  BCORE_FORWARD_OBJECT( bhpt_frame_thread ); \
  BCORE_FORWARD_OBJECT( bhpt_frame_thread_share_s ); \
  BCORE_FORWARD_OBJECT( bhpt_frame_thread_item_s ); \
  BCORE_FORWARD_OBJECT( bhpt_frame_thread_ads_s ); \
  BCORE_FORWARD_OBJECT( bhpt_frame_thread_base_s ); \
  BETH_EXPAND_ITEM_bhpt_frame_thread_share_s \
  BETH_EXPAND_ITEM_bhpt_frame_thread_item_s \
  BETH_EXPAND_ITEM_bhpt_frame_thread_ads_s \
  BETH_EXPAND_ITEM_bhpt_frame_thread_base_s

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
#define TYPEOF_bhpt_adaptor_reg_l2_s 1953171829
#define BETH_EXPAND_ITEM_bhpt_adaptor_reg_l2_s \
  BCORE_DECLARE_OBJECT( bhpt_adaptor_reg_l2_s ) \
    {aware_t _;f3_t lambda;}; \
  static inline void bhpt_adaptor_reg_l2_s_reset( bhpt_adaptor_reg_l2_s* o ){} \
  static inline void bhpt_adaptor_reg_l2_s_adapt( bhpt_adaptor_reg_l2_s* o, const bhpt_adaptor_node_s* node ){bhvm_value_s_mul_scl_f3_acc( &node->axon->v, -o->lambda, &node->axon->v );}
#define TYPEOF_bhpt_adaptor_zro_grad_s 119886870
#define BETH_EXPAND_ITEM_bhpt_adaptor_zro_grad_s \
  BCORE_DECLARE_OBJECT( bhpt_adaptor_zro_grad_s ) \
    {aware_t _;}; \
  static inline void bhpt_adaptor_zro_grad_s_adapt( bhpt_adaptor_zro_grad_s* o, const bhpt_adaptor_node_s* node ){bhvm_value_s_zro( &node->grad->v );}
#define TYPEOF_bhpt_adaptor_list_s 3111754760
#define BETH_EXPAND_ITEM_bhpt_adaptor_list_s \
  BCORE_DECLARE_OBJECT( bhpt_adaptor_list_s ) \
    {aware_t _;BCORE_ARRAY_DYN_LINK_STATIC_S( bhpt_adaptor, );}; \
  void bhpt_adaptor_list_s_reset( bhpt_adaptor_list_s* o ); \
  void bhpt_adaptor_list_s_adapt( bhpt_adaptor_list_s* o, const bhpt_adaptor_node_s* node ); \
  static inline bhpt_adaptor_list_s* bhpt_adaptor_list_s_set_space( bhpt_adaptor_list_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_bhpt_adaptor_list_s, ( bcore_array* )o, size ); return o; } \
  static inline bhpt_adaptor_list_s* bhpt_adaptor_list_s_set_size( bhpt_adaptor_list_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_bhpt_adaptor_list_s, ( bcore_array* )o, size ); return o; } \
  static inline bhpt_adaptor_list_s* bhpt_adaptor_list_s_clear( bhpt_adaptor_list_s* o ) { bcore_array_t_set_space( TYPEOF_bhpt_adaptor_list_s, ( bcore_array* )o, 0 ); return o; } \
  static inline bhpt_adaptor* bhpt_adaptor_list_s_push_c( bhpt_adaptor_list_s* o, const bhpt_adaptor* v ) { bcore_array_t_push( TYPEOF_bhpt_adaptor_list_s, ( bcore_array* )o, sr_awc( v ) ); return o->data[ o->size - 1 ]; } \
  static inline bhpt_adaptor* bhpt_adaptor_list_s_push_d( bhpt_adaptor_list_s* o,       bhpt_adaptor* v ) { bcore_array_t_push( TYPEOF_bhpt_adaptor_list_s, ( bcore_array* )o, sr_asd( v ) ); return o->data[ o->size - 1 ]; } \
  static inline bhpt_adaptor* bhpt_adaptor_list_s_push_t( bhpt_adaptor_list_s* o, tp_t t ) \
  { \
      bcore_trait_assert_satisfied_type( TYPEOF_bhpt_adaptor, t ); \
      bcore_array_t_push( TYPEOF_bhpt_adaptor_list_s, ( bcore_array* )o, sr_t_create( t ) ); \
      return o->data[ o->size - 1 ]; \
  }
#define BETH_EXPAND_GROUP_bhpt_adaptor_stamp \
  BCORE_FORWARD_OBJECT( bhpt_adaptor_stamp ); \
  BCORE_FORWARD_OBJECT( bhpt_adaptor_epsilon_s ); \
  BCORE_FORWARD_OBJECT( bhpt_adaptor_reg_l2_s ); \
  BCORE_FORWARD_OBJECT( bhpt_adaptor_zro_grad_s ); \
  BCORE_FORWARD_OBJECT( bhpt_adaptor_list_s ); \
  BETH_EXPAND_ITEM_bhpt_adaptor_epsilon_s \
  BETH_EXPAND_ITEM_bhpt_adaptor_reg_l2_s \
  BETH_EXPAND_ITEM_bhpt_adaptor_zro_grad_s \
  BETH_EXPAND_ITEM_bhpt_adaptor_list_s

/**********************************************************************************************************************/
// source: bhpt_tutor_sampler.h

//----------------------------------------------------------------------------------------------------------------------
// group: bhpt_sampler

#define TYPEOF_bhpt_sampler 4168642596
#define TYPEOF_bhpt_sampler_s 2734099862
#define TYPEOF_bhpt_sampler_sine_random_s 2335148080
#define BETH_EXPAND_ITEM_bhpt_sampler_sine_random_s \
  BCORE_DECLARE_OBJECT( bhpt_sampler_sine_random_s ) \
    {aware_t _;sz_t size_en;f3_t pos_tgt;f3_t neg_tgt;}; \
  void bhpt_sampler_sine_random_s_fetch( const bhpt_sampler_sine_random_s* o, bcore_prsg* prsg, bhvm_value_s* x, bhvm_value_s* y ); \
  static inline sz_t bhpt_sampler_sine_random_s_get_size_en( const bhpt_sampler_sine_random_s* o ){return o->size_en;} \
  static inline sz_t bhpt_sampler_sine_random_s_get_size_ex( const bhpt_sampler_sine_random_s* o ){return 1;}
#define TYPEOF_bhpt_sampler_binary_add_s 579736158
#define BETH_EXPAND_ITEM_bhpt_sampler_binary_add_s \
  BCORE_DECLARE_OBJECT( bhpt_sampler_binary_add_s ) \
    {aware_t _;sz_t bits;f3_t val_h;f3_t val_l;}; \
  void bhpt_sampler_binary_add_s_fetch( const bhpt_sampler_binary_add_s* o, bcore_prsg* prsg, bhvm_value_s* x, bhvm_value_s* y ); \
  static inline sz_t bhpt_sampler_binary_add_s_get_size_en( const bhpt_sampler_binary_add_s* o ){return o->bits * 2;} \
  static inline sz_t bhpt_sampler_binary_add_s_get_size_ex( const bhpt_sampler_binary_add_s* o ){return o->bits + 1;}
#define TYPEOF_bhpt_sampler_binary_mul_s 1593470499
#define BETH_EXPAND_ITEM_bhpt_sampler_binary_mul_s \
  BCORE_DECLARE_OBJECT( bhpt_sampler_binary_mul_s ) \
    {aware_t _;sz_t bits;f3_t val_h;f3_t val_l;}; \
  void bhpt_sampler_binary_mul_s_fetch( const bhpt_sampler_binary_mul_s* o, bcore_prsg* prsg, bhvm_value_s* x, bhvm_value_s* y ); \
  static inline sz_t bhpt_sampler_binary_mul_s_get_size_en( const bhpt_sampler_binary_mul_s* o ){return o->bits * 2;} \
  static inline sz_t bhpt_sampler_binary_mul_s_get_size_ex( const bhpt_sampler_binary_mul_s* o ){return o->bits * 2;}
#define TYPEOF_bhpt_sampler_binary_lcg00_s 3949471327
#define BETH_EXPAND_ITEM_bhpt_sampler_binary_lcg00_s \
  BCORE_DECLARE_OBJECT( bhpt_sampler_binary_lcg00_s ) \
    {aware_t _;sz_t bits;f3_t val_h;f3_t val_l;}; \
  void bhpt_sampler_binary_lcg00_s_fetch( const bhpt_sampler_binary_lcg00_s* o, bcore_prsg* prsg, bhvm_value_s* x, bhvm_value_s* y ); \
  static inline sz_t bhpt_sampler_binary_lcg00_s_get_size_en( const bhpt_sampler_binary_lcg00_s* o ){return o->bits;} \
  static inline sz_t bhpt_sampler_binary_lcg00_s_get_size_ex( const bhpt_sampler_binary_lcg00_s* o ){return o->bits;}
#define TYPEOF_bhpt_sampler_binary_hash_s 1225036579
#define BETH_EXPAND_ITEM_bhpt_sampler_binary_hash_s \
  BCORE_DECLARE_OBJECT( bhpt_sampler_binary_hash_s ) \
    {aware_t _;sz_t bits;f3_t val_h;f3_t val_l;bl_t reverse;}; \
  void bhpt_sampler_binary_hash_s_fetch( const bhpt_sampler_binary_hash_s* o, bcore_prsg* prsg, bhvm_value_s* x, bhvm_value_s* y ); \
  static inline sz_t bhpt_sampler_binary_hash_s_get_size_en( const bhpt_sampler_binary_hash_s* o ){return o->bits;} \
  static inline sz_t bhpt_sampler_binary_hash_s_get_size_ex( const bhpt_sampler_binary_hash_s* o ){return o->bits;}
#define TYPEOF_bhpt_sampler_polynom_s 4138254329
#define BETH_EXPAND_ITEM_bhpt_sampler_polynom_s \
  BCORE_DECLARE_OBJECT( bhpt_sampler_polynom_s ) \
    {aware_t _;sz_t size_en;sz_t size_ex;f3_t range;f3_t noise_level;}; \
  void bhpt_sampler_polynom_s_fetch( const bhpt_sampler_polynom_s* o, bcore_prsg* prsg, bhvm_value_s* x, bhvm_value_s* y ); \
  static inline sz_t bhpt_sampler_polynom_s_get_size_en( const bhpt_sampler_polynom_s* o ){return o->size_en;} \
  static inline sz_t bhpt_sampler_polynom_s_get_size_ex( const bhpt_sampler_polynom_s* o ){return o->size_ex;}
#define BETH_EXPAND_GROUP_bhpt_sampler \
  BCORE_FORWARD_OBJECT( bhpt_sampler ); \
  BCORE_FORWARD_OBJECT( bhpt_sampler_sine_random_s ); \
  BCORE_FORWARD_OBJECT( bhpt_sampler_binary_add_s ); \
  BCORE_FORWARD_OBJECT( bhpt_sampler_binary_mul_s ); \
  BCORE_FORWARD_OBJECT( bhpt_sampler_binary_lcg00_s ); \
  BCORE_FORWARD_OBJECT( bhpt_sampler_binary_hash_s ); \
  BCORE_FORWARD_OBJECT( bhpt_sampler_polynom_s ); \
  typedef void (*bhpt_sampler_fetch)( const bhpt_sampler* o, bcore_prsg* prsg, bhvm_value_s* x, bhvm_value_s* y ); \
  typedef sz_t (*bhpt_sampler_get_size_en)( const bhpt_sampler* o ); \
  typedef sz_t (*bhpt_sampler_get_size_ex)( const bhpt_sampler* o ); \
  BCORE_DECLARE_SPECT( bhpt_sampler ) \
  { \
      bcore_spect_header_s header; \
      bhpt_sampler_fetch fetch; \
      bhpt_sampler_get_size_en get_size_en; \
      bhpt_sampler_get_size_ex get_size_ex; \
  }; \
  static inline bhpt_sampler* bhpt_sampler_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_bhpt_sampler, t ); return ( bhpt_sampler* )bcore_inst_t_create( t ); } \
  static inline bl_t bhpt_sampler_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_bhpt_sampler ); } \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bhpt_sampler ) \
  static inline bl_t bhpt_sampler_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_bhpt_sampler ); } \
  static inline void bhpt_sampler_a_fetch( const bhpt_sampler* o, bcore_prsg* prsg, bhvm_value_s* x, bhvm_value_s* y ) { const bhpt_sampler_s* p = bhpt_sampler_s_get_aware( o ); assert( p->fetch ); p->fetch( o, prsg, x, y ); } \
  static inline bl_t bhpt_sampler_a_defines_fetch( const bhpt_sampler* o ) { return true; } \
  static inline sz_t bhpt_sampler_a_get_size_en( const bhpt_sampler* o ) { const bhpt_sampler_s* p = bhpt_sampler_s_get_aware( o ); assert( p->get_size_en ); return p->get_size_en( o ); } \
  static inline bl_t bhpt_sampler_a_defines_get_size_en( const bhpt_sampler* o ) { return true; } \
  static inline sz_t bhpt_sampler_a_get_size_ex( const bhpt_sampler* o ) { const bhpt_sampler_s* p = bhpt_sampler_s_get_aware( o ); assert( p->get_size_ex ); return p->get_size_ex( o ); } \
  static inline bl_t bhpt_sampler_a_defines_get_size_ex( const bhpt_sampler* o ) { return true; } \
  BETH_EXPAND_ITEM_bhpt_sampler_sine_random_s \
  BETH_EXPAND_ITEM_bhpt_sampler_binary_add_s \
  BETH_EXPAND_ITEM_bhpt_sampler_binary_mul_s \
  BETH_EXPAND_ITEM_bhpt_sampler_binary_lcg00_s \
  BETH_EXPAND_ITEM_bhpt_sampler_binary_hash_s \
  BETH_EXPAND_ITEM_bhpt_sampler_polynom_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhpt_tutor_sampler

#define TYPEOF_bhpt_tutor_sampler 3705855743
#define TYPEOF_bhpt_tutor_sampler_s 3882161769
#define TYPEOF_bhpt_tutor_sampler_s 3882161769
#define BETH_EXPAND_ITEM_bhpt_tutor_sampler_s \
  BCORE_DECLARE_OBJECT( bhpt_tutor_sampler_s ) \
    {aware_t _;bhpt_builder* builder;bhpt_adaptor* adaptor;bhpt_sampler* sampler;bcore_prsg* prsg_priming;bcore_prsg* prsg_testing;sz_t test_size;bcore_mutex_s mutex;}; \
  static inline void bhpt_tutor_sampler_s_reset( bhpt_tutor_sampler_s* o ){} \
  bhpt_adaptive* bhpt_tutor_sampler_s_create_adaptive( const bhpt_tutor_sampler_s* o ); \
  static inline bhpt_adaptor* bhpt_tutor_sampler_s_create_adaptor( const bhpt_tutor_sampler_s* o ){return bhpt_adaptor_a_clone( o->adaptor );} \
  void bhpt_tutor_sampler_s_prime( bhpt_tutor_sampler_s* o, bhpt_adaptive* adaptive ); \
  void bhpt_tutor_sampler_s_test( bhpt_tutor_sampler_s* o, const bhpt_adaptive* adaptive, sz_t verbosity, bcore_sink* log ); \
  void bhpt_tutor_sampler_s_status_to_sink( const bhpt_tutor_sampler_s* o, sz_t verbosity, bcore_sink* sink );
#define BETH_EXPAND_GROUP_bhpt_tutor_sampler \
  BCORE_FORWARD_OBJECT( bhpt_tutor_sampler ); \
  BCORE_FORWARD_OBJECT( bhpt_tutor_sampler_s ); \
  BETH_EXPAND_ITEM_bhpt_tutor_sampler_s

/**********************************************************************************************************************/
// source: bhpt_tutor_language.h

//----------------------------------------------------------------------------------------------------------------------
// group: bhpt_tutor_language

#define TYPEOF_bhpt_tutor_language 3148785547
#define TYPEOF_bhpt_tutor_language_s 289799133
#define TYPEOF_bhpt_tutor_language_utf8_chatter_s 1375128031
#define BETH_EXPAND_ITEM_bhpt_tutor_language_utf8_chatter_s \
  BCORE_DECLARE_OBJECT( bhpt_tutor_language_utf8_chatter_s ) \
    {aware_t _;bl_t cyclic_reset;st_s trigger;sz_t size_line;sz_t size_lines;bl_t newline_to_space;f3_t heat;};
#define TYPEOF_bhpt_tutor_language_utf8_s 409996605
#define BETH_EXPAND_ITEM_bhpt_tutor_language_utf8_s \
  BCORE_DECLARE_OBJECT( bhpt_tutor_language_utf8_s ) \
    {aware_t _;bhpt_builder* builder;bhpt_adaptor* adaptor;vd_t src;st_s* st;bcore_prsg* prsg_priming;bcore_prsg* prsg_testing;sz_t size_trans;sz_t size_prime;sz_t size_test;f3_t tgt_pos;f3_t tgt_neg;bhpt_tutor_language_utf8_chatter_s* chatter;bcore_mutex_s mutex;}; \
  static inline void bhpt_tutor_language_utf8_s_reset( bhpt_tutor_language_utf8_s* o ){bcore_prsg_a_attach( &o->prsg_priming, bcore_prsg_t_create( TYPEOF_bcore_prsg_lcg_u3_00_s ) );} \
  bhpt_adaptive* bhpt_tutor_language_utf8_s_create_adaptive( const bhpt_tutor_language_utf8_s* o ); \
  static inline bhpt_adaptor* bhpt_tutor_language_utf8_s_create_adaptor( const bhpt_tutor_language_utf8_s* o ){return bhpt_adaptor_a_clone( o->adaptor );} \
  void bhpt_tutor_language_utf8_s_prime( bhpt_tutor_language_utf8_s* o, bhpt_adaptive* adaptive ); \
  void bhpt_tutor_language_utf8_s_test( bhpt_tutor_language_utf8_s* o, const bhpt_adaptive* adaptive, sz_t verbosity, bcore_sink* log ); \
  void bhpt_tutor_language_utf8_s_status_to_sink( const bhpt_tutor_language_utf8_s* o, sz_t verbosity, bcore_sink* sink );
#define BETH_EXPAND_GROUP_bhpt_tutor_language \
  BCORE_FORWARD_OBJECT( bhpt_tutor_language ); \
  BCORE_FORWARD_OBJECT( bhpt_tutor_language_utf8_chatter_s ); \
  BCORE_FORWARD_OBJECT( bhpt_tutor_language_utf8_s ); \
  BETH_EXPAND_ITEM_bhpt_tutor_language_utf8_chatter_s \
  BETH_EXPAND_ITEM_bhpt_tutor_language_utf8_s

/**********************************************************************************************************************/

vd_t bhpt_xoi_out_signal_handler( const bcore_signal_s* o );

#endif // BHPT_XOI_OUT_H
// BETH_PLANT_SIGNATURE 2974870919
