/** This file was generated from xoila source code.
 *  Compiling Agent : xoico_compiler (C) 2020 J.B.Steffens
 *  Last File Update: 2020-12-01T10:47:37Z
 *
 *  Copyright and License of this File:
 *
 *  Generated code inherits the copyright and license of the underlying xoila source code.
 *  Source code defining this file is distributed across following files:
 *
 *  bhpt_sketch.h
 *  bhpt_frame.h
 *  bhpt_adaptor.h
 *  bhpt_tutor_sampler.h
 *  bhpt_tutor_language.h
 *
 */

#ifndef __bhpt_xo_H
#define __bhpt_xo_H

#include "bcore_control.h"

//To force a rebuild of this target by xoico, reset the hash key value below to 0.
#define HKEYOF_bhpt_xo 0x60CB9BE7D10E8123ull

#define TYPEOF_bhpt_xo 0xF2F8DBC9C900AFD1ull

/**********************************************************************************************************************/
// source: bhpt_sketch.h

//----------------------------------------------------------------------------------------------------------------------
// group: bhpt

#define TYPEOF_bhpt 0xE648539BD4A63043ull
#define TYPEOF_bhpt_spect_s 0x2DF72F7CB6431B23ull
#define BETH_EXPAND_GROUP_bhpt \
  BCORE_FORWARD_OBJECT( bhpt ); \
  BCORE_FORWARD_OBJECT( bhpt_adaptor ); \
  BCORE_FORWARD_OBJECT( bhpt_adaptive ); \
  BCORE_FORWARD_OBJECT( bhpt_builder ); \
  BCORE_FORWARD_OBJECT( bhpt_tutor ); \
  XOILA_DECLARE_SPECT( bhpt ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bhpt ) \
  BETH_EXPAND_GROUP_bhpt_adaptor \
  BETH_EXPAND_GROUP_bhpt_adaptive \
  BETH_EXPAND_GROUP_bhpt_builder \
  BETH_EXPAND_GROUP_bhpt_tutor

//----------------------------------------------------------------------------------------------------------------------
// group: bhpt_adaptor

#define TYPEOF_bhpt_adaptor 0x8A28A804E11191C1ull
#define TYPEOF_bhpt_adaptor_spect_s 0xDCD4BC6C0B4D8CBDull
#define TYPEOF_bhpt_adaptor_node_s 0x8FA184E365F971E8ull
#define BETH_EXPAND_ITEM_bhpt_adaptor_node_s \
  BCORE_DECLARE_OBJECT( bhpt_adaptor_node_s ) \
  { \
      aware_t _; \
      bhvm_holor_s* axon; \
      bhvm_holor_s* grad; \
  }; \
  void bhpt_adaptor_node_s_get_min_max( const bhpt_adaptor_node_s* o, f3_t* min, f3_t* max ); \
  void bhpt_adaptor_node_s_acc_stats( const bhpt_adaptor_node_s* o, bhvm_stats_s* axon, bhvm_stats_s* grad ); \
  static inline void bhpt_adaptor_node_s_zro_grad( bhpt_adaptor_node_s* o ); \
  static inline void bhpt_adaptor_node_s_acc_grad( bhpt_adaptor_node_s* o, const bhpt_adaptor_node_s* src ); \
  static inline void bhpt_adaptor_node_s_rebind_axon( bhpt_adaptor_node_s* o, bhpt_adaptor_node_s* src ); \
  static inline void bhpt_adaptor_node_s_zro_grad( bhpt_adaptor_node_s* o ){ bhvm_value_s_zro( &(o->grad->v));} \
  static inline void bhpt_adaptor_node_s_acc_grad( bhpt_adaptor_node_s* o, const bhpt_adaptor_node_s* src ){ assert( bhvm_shape_s_is_equal( &(o->grad->s),(&(src->grad->s)) ) ); bhvm_value_s_acc( &(o->grad->v),(&(src->grad->v)) );} \
  static inline void bhpt_adaptor_node_s_rebind_axon( bhpt_adaptor_node_s* o, bhpt_adaptor_node_s* src ){ assert( bhvm_shape_s_is_equal( &(o->axon->s),(&(src->axon->s)) ) ); bhvm_value_s_weak( &(o->axon->v),(&(src->axon->v)) );}
#define TYPEOF_bhpt_adaptor_probe_s 0x8965717B3CC0FD56ull
#define BETH_EXPAND_ITEM_bhpt_adaptor_probe_s \
  BCORE_DECLARE_OBJECT( bhpt_adaptor_probe_s ) \
  { \
      aware_t _; \
      BCORE_ARRAY_DYN_SOLID_STATIC_S( bhpt_adaptor_node_s, ); \
  }; \
  void bhpt_adaptor_probe_s_get_min_max( const bhpt_adaptor_probe_s* o, f3_t* min, f3_t* max ); \
  static inline void bhpt_adaptor_probe_s_acc_stats( const bhpt_adaptor_probe_s* o, bhvm_stats_s* axon, bhvm_stats_s* grad ); \
  static inline void bhpt_adaptor_probe_s_zro_grad( bhpt_adaptor_probe_s* o ); \
  static inline void bhpt_adaptor_probe_s_acc_grad( bhpt_adaptor_probe_s* o, const bhpt_adaptor_probe_s* src ); \
  static inline void bhpt_adaptor_probe_s_rebind_axon( bhpt_adaptor_probe_s* o, bhpt_adaptor_probe_s* src ); \
  static inline bhpt_adaptor_probe_s* bhpt_adaptor_probe_s_set_space( bhpt_adaptor_probe_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_bhpt_adaptor_probe_s, ( bcore_array* )o, size ); return o; } \
  static inline bhpt_adaptor_probe_s* bhpt_adaptor_probe_s_set_size( bhpt_adaptor_probe_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_bhpt_adaptor_probe_s, ( bcore_array* )o, size ); return o; } \
  static inline bhpt_adaptor_probe_s* bhpt_adaptor_probe_s_clear( bhpt_adaptor_probe_s* o ) { bcore_array_t_set_space( TYPEOF_bhpt_adaptor_probe_s, ( bcore_array* )o, 0 ); return o; } \
  static inline bhpt_adaptor_node_s* bhpt_adaptor_probe_s_push_c( bhpt_adaptor_probe_s* o, const bhpt_adaptor_node_s* v ) { bcore_array_t_push( TYPEOF_bhpt_adaptor_probe_s, ( bcore_array* )o, sr_twc( TYPEOF_bhpt_adaptor_node_s, v ) ); return &o->data[ o->size - 1 ]; } \
  static inline bhpt_adaptor_node_s* bhpt_adaptor_probe_s_push_d( bhpt_adaptor_probe_s* o,       bhpt_adaptor_node_s* v ) { bcore_array_t_push( TYPEOF_bhpt_adaptor_probe_s, ( bcore_array* )o, sr_tsd( TYPEOF_bhpt_adaptor_node_s, v ) ); return &o->data[ o->size - 1 ]; } \
  static inline bhpt_adaptor_node_s* bhpt_adaptor_probe_s_push( bhpt_adaptor_probe_s* o ) \
  { \
      bcore_array_t_push( TYPEOF_bhpt_adaptor_probe_s, ( bcore_array* )o, sr_null() ); \
      return &o->data[ o->size - 1 ]; \
  } \
  static inline void bhpt_adaptor_probe_s_acc_stats( const bhpt_adaptor_probe_s* o, bhvm_stats_s* axon, bhvm_stats_s* grad ){ BFOR_EACH( i, o ) bhpt_adaptor_node_s_acc_stats(&(o->data[ i ]),axon, grad );} \
  static inline void bhpt_adaptor_probe_s_zro_grad( bhpt_adaptor_probe_s* o ){ BFOR_EACH( i, o ) bhpt_adaptor_node_s_zro_grad(&(o->data[ i ]));} \
  static inline void bhpt_adaptor_probe_s_acc_grad( bhpt_adaptor_probe_s* o, const bhpt_adaptor_probe_s* src ){ assert( o->size == src->size ); BFOR_EACH( i, o ) bhpt_adaptor_node_s_acc_grad(&(o->data[ i ]),(&(src->data[ i ])) );} \
  static inline void bhpt_adaptor_probe_s_rebind_axon( bhpt_adaptor_probe_s* o, bhpt_adaptor_probe_s* src ){ assert( o->size == src->size ); BFOR_EACH( i, o ) bhpt_adaptor_node_s_rebind_axon(&(o->data[ i ]),(&(src->data[ i ])) );}
#define TYPEOF_bhpt_adaptor_adl_s 0xF7B0D699810C2663ull
#define BETH_EXPAND_ITEM_bhpt_adaptor_adl_s \
  BCORE_DECLARE_OBJECT( bhpt_adaptor_adl_s ) \
  { \
      aware_t _; \
      BCORE_ARRAY_DYN_LINK_STATIC_S( bhpt_adaptor, ); \
  }; \
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
  XOILA_DECLARE_SPECT( bhpt_adaptor ) \
  { \
      bcore_spect_header_s header; \
      bhpt_adaptor_reset reset; \
      bhpt_adaptor_adapt adapt; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bhpt_adaptor ) \
  BETH_EXPAND_ITEM_bhpt_adaptor_node_s \
  BETH_EXPAND_ITEM_bhpt_adaptor_probe_s \
  static inline void bhpt_adaptor_a_reset( bhpt_adaptor* o ){ const bhpt_adaptor_spect_s* p = bhpt_adaptor_spect_s_get_aware( o ); assert( p->reset ); p->reset( o );} \
  static inline bl_t bhpt_adaptor_a_defines_reset( const bhpt_adaptor* o ){ return  bhpt_adaptor_spect_s_get_aware( o )->reset != NULL;} \
  static inline void bhpt_adaptor_a_adapt( bhpt_adaptor* o, const bhpt_adaptor_node_s* node ){ const bhpt_adaptor_spect_s* p = bhpt_adaptor_spect_s_get_aware( o ); assert( p->adapt ); p->adapt( o, node );} \
  static inline bl_t bhpt_adaptor_a_defines_adapt( const bhpt_adaptor* o ){ return  bhpt_adaptor_spect_s_get_aware( o )->adapt != NULL;} \
  BETH_EXPAND_ITEM_bhpt_adaptor_adl_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhpt_adaptive

#define TYPEOF_bhpt_adaptive 0xE6B8AA4A7B4C94BAull
#define TYPEOF_bhpt_adaptive_spect_s 0x2210CFF3154F2566ull
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
  XOILA_DECLARE_SPECT( bhpt_adaptive ) \
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
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bhpt_adaptive ) \
  static inline bhvm_holor_s* bhpt_adaptive_a_get_format_en( const bhpt_adaptive* o, bhvm_holor_s* format ){ const bhpt_adaptive_spect_s* p = bhpt_adaptive_spect_s_get_aware( o ); assert( p->get_format_en ); return p->get_format_en( o, format );} \
  static inline bl_t bhpt_adaptive_a_defines_get_format_en( const bhpt_adaptive* o ){ return  true;} \
  static inline bhvm_holor_s* bhpt_adaptive_a_get_format_ex( const bhpt_adaptive* o, bhvm_holor_s* format ){ const bhpt_adaptive_spect_s* p = bhpt_adaptive_spect_s_get_aware( o ); assert( p->get_format_ex ); return p->get_format_ex( o, format );} \
  static inline bl_t bhpt_adaptive_a_defines_get_format_ex( const bhpt_adaptive* o ){ return  true;} \
  static inline void bhpt_adaptive_a_axon_pass( bhpt_adaptive* o, const bhvm_holor_s* ax_en, bhvm_holor_s* ax_ex ){ const bhpt_adaptive_spect_s* p = bhpt_adaptive_spect_s_get_aware( o ); assert( p->axon_pass ); p->axon_pass( o, ax_en, ax_ex );} \
  static inline bl_t bhpt_adaptive_a_defines_axon_pass( const bhpt_adaptive* o ){ return  true;} \
  static inline void bhpt_adaptive_axon_pass_default( bhpt_adaptive* o, const bhvm_holor_s* ax_en, bhvm_holor_s* ax_ex ){} \
  static inline void bhpt_adaptive_a_dendrite_pass( bhpt_adaptive* o, const bhvm_holor_s* ag_ex, bhvm_holor_s* ag_en ){ const bhpt_adaptive_spect_s* p = bhpt_adaptive_spect_s_get_aware( o ); assert( p->dendrite_pass ); p->dendrite_pass( o, ag_ex, ag_en );} \
  static inline bl_t bhpt_adaptive_a_defines_dendrite_pass( const bhpt_adaptive* o ){ return  true;} \
  static inline void bhpt_adaptive_dendrite_pass_default( bhpt_adaptive* o, const bhvm_holor_s* ag_ex, bhvm_holor_s* ag_en ){} \
  static inline void bhpt_adaptive_a_cyclic_reset( bhpt_adaptive* o ){ const bhpt_adaptive_spect_s* p = bhpt_adaptive_spect_s_get_aware( o ); assert( p->cyclic_reset ); p->cyclic_reset( o );} \
  static inline bl_t bhpt_adaptive_a_defines_cyclic_reset( const bhpt_adaptive* o ){ return  true;} \
  static inline void bhpt_adaptive_cyclic_reset_default( bhpt_adaptive* o ){} \
  static inline bhpt_adaptor_probe_s* bhpt_adaptive_a_get_adaptor_probe( const bhpt_adaptive* o, bhpt_adaptor_probe_s* probe ){ const bhpt_adaptive_spect_s* p = bhpt_adaptive_spect_s_get_aware( o ); assert( p->get_adaptor_probe ); return p->get_adaptor_probe( o, probe );} \
  static inline bl_t bhpt_adaptive_a_defines_get_adaptor_probe( const bhpt_adaptive* o ){ return  true;} \
  static inline bhpt_adaptor_probe_s* bhpt_adaptive_get_adaptor_probe_default( const bhpt_adaptive* o, bhpt_adaptor_probe_s* probe ){ return  probe;} \
  static inline void bhpt_adaptive_a_rebind_holors( bhpt_adaptive* o ){ const bhpt_adaptive_spect_s* p = bhpt_adaptive_spect_s_get_aware( o ); assert( p->rebind_holors ); p->rebind_holors( o );} \
  static inline bl_t bhpt_adaptive_a_defines_rebind_holors( const bhpt_adaptive* o ){ return  bhpt_adaptive_spect_s_get_aware( o )->rebind_holors != NULL;} \
  static inline void bhpt_adaptive_a_status_to_sink( const bhpt_adaptive* o, sz_t verbosity, bcore_sink* sink ){ const bhpt_adaptive_spect_s* p = bhpt_adaptive_spect_s_get_aware( o ); assert( p->status_to_sink ); p->status_to_sink( o, verbosity, sink );} \
  static inline bl_t bhpt_adaptive_a_defines_status_to_sink( const bhpt_adaptive* o ){ return  true;} \
  static inline void bhpt_adaptive_status_to_sink_default( const bhpt_adaptive* o, sz_t verbosity, bcore_sink* sink ){ if( verbosity > 0 ) bcore_txt_ml_a_to_sink( o, sink );}

//----------------------------------------------------------------------------------------------------------------------
// group: bhpt_builder

#define TYPEOF_bhpt_builder 0xAA9D955CE239FAB3ull
#define TYPEOF_bhpt_builder_spect_s 0x0F6DA23E09FEA793ull
#define BETH_EXPAND_GROUP_bhpt_builder \
  BCORE_FORWARD_OBJECT( bhpt_builder ); \
  typedef void (*bhpt_builder_set_format_en)( bhpt_builder* o, const bhvm_holor_s* format ); \
  typedef void (*bhpt_builder_set_format_ex)( bhpt_builder* o, const bhvm_holor_s* format ); \
  typedef bhpt_adaptive* (*bhpt_builder_create_adaptive)( const bhpt_builder* o ); \
  XOILA_DECLARE_SPECT( bhpt_builder ) \
  { \
      bcore_spect_header_s header; \
      bhpt_builder_set_format_en set_format_en; \
      bhpt_builder_set_format_ex set_format_ex; \
      bhpt_builder_create_adaptive create_adaptive; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bhpt_builder ) \
  static inline void bhpt_builder_a_set_format_en( bhpt_builder* o, const bhvm_holor_s* format ){ const bhpt_builder_spect_s* p = bhpt_builder_spect_s_get_aware( o ); assert( p->set_format_en ); p->set_format_en( o, format );} \
  static inline bl_t bhpt_builder_a_defines_set_format_en( const bhpt_builder* o ){ return  true;} \
  static inline void bhpt_builder_a_set_format_ex( bhpt_builder* o, const bhvm_holor_s* format ){ const bhpt_builder_spect_s* p = bhpt_builder_spect_s_get_aware( o ); assert( p->set_format_ex ); p->set_format_ex( o, format );} \
  static inline bl_t bhpt_builder_a_defines_set_format_ex( const bhpt_builder* o ){ return  true;} \
  static inline bhpt_adaptive* bhpt_builder_a_create_adaptive( const bhpt_builder* o ){ const bhpt_builder_spect_s* p = bhpt_builder_spect_s_get_aware( o ); assert( p->create_adaptive ); return p->create_adaptive( o );} \
  static inline bl_t bhpt_builder_a_defines_create_adaptive( const bhpt_builder* o ){ return  true;}

//----------------------------------------------------------------------------------------------------------------------
// group: bhpt_tutor

#define TYPEOF_bhpt_tutor 0xEE1912F58836A8A8ull
#define TYPEOF_bhpt_tutor_spect_s 0x186AA0F7E90E9EF0ull
#define BETH_EXPAND_GROUP_bhpt_tutor \
  BCORE_FORWARD_OBJECT( bhpt_tutor ); \
  typedef bhpt_adaptive* (*bhpt_tutor_create_adaptive)( const bhpt_tutor* o ); \
  typedef bhpt_adaptor* (*bhpt_tutor_create_adaptor)( const bhpt_tutor* o ); \
  typedef void (*bhpt_tutor_reset)( bhpt_tutor* o ); \
  typedef void (*bhpt_tutor_prime)( bhpt_tutor* o, bhpt_adaptive* adaptive ); \
  typedef void (*bhpt_tutor_test)( bhpt_tutor* o, const bhpt_adaptive* adaptive, sz_t verbosity, bcore_sink* log ); \
  typedef void (*bhpt_tutor_status_to_sink)( const bhpt_tutor* o, sz_t verbosity, bcore_sink* sink ); \
  XOILA_DECLARE_SPECT( bhpt_tutor ) \
  { \
      bcore_spect_header_s header; \
      bhpt_tutor_create_adaptive create_adaptive; \
      bhpt_tutor_create_adaptor create_adaptor; \
      bhpt_tutor_reset reset; \
      bhpt_tutor_prime prime; \
      bhpt_tutor_test test; \
      bhpt_tutor_status_to_sink status_to_sink; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bhpt_tutor ) \
  static inline bhpt_adaptive* bhpt_tutor_a_create_adaptive( const bhpt_tutor* o ){ const bhpt_tutor_spect_s* p = bhpt_tutor_spect_s_get_aware( o ); assert( p->create_adaptive ); return p->create_adaptive( o );} \
  static inline bl_t bhpt_tutor_a_defines_create_adaptive( const bhpt_tutor* o ){ return  true;} \
  static inline bhpt_adaptor* bhpt_tutor_a_create_adaptor( const bhpt_tutor* o ){ const bhpt_tutor_spect_s* p = bhpt_tutor_spect_s_get_aware( o ); assert( p->create_adaptor ); return p->create_adaptor( o );} \
  static inline bl_t bhpt_tutor_a_defines_create_adaptor( const bhpt_tutor* o ){ return  true;} \
  static inline void bhpt_tutor_a_reset( bhpt_tutor* o ){ const bhpt_tutor_spect_s* p = bhpt_tutor_spect_s_get_aware( o ); assert( p->reset ); p->reset( o );} \
  static inline bl_t bhpt_tutor_a_defines_reset( const bhpt_tutor* o ){ return  true;} \
  static inline void bhpt_tutor_reset_default( bhpt_tutor* o ){} \
  static inline void bhpt_tutor_a_prime( bhpt_tutor* o, bhpt_adaptive* adaptive ){ const bhpt_tutor_spect_s* p = bhpt_tutor_spect_s_get_aware( o ); assert( p->prime ); p->prime( o, adaptive );} \
  static inline bl_t bhpt_tutor_a_defines_prime( const bhpt_tutor* o ){ return  true;} \
  static inline void bhpt_tutor_prime_default( bhpt_tutor* o, bhpt_adaptive* adaptive ){} \
  static inline void bhpt_tutor_a_test( bhpt_tutor* o, const bhpt_adaptive* adaptive, sz_t verbosity, bcore_sink* log ){ const bhpt_tutor_spect_s* p = bhpt_tutor_spect_s_get_aware( o ); assert( p->test ); p->test( o, adaptive, verbosity, log );} \
  static inline bl_t bhpt_tutor_a_defines_test( const bhpt_tutor* o ){ return  true;} \
  static inline void bhpt_tutor_test_default( bhpt_tutor* o, const bhpt_adaptive* adaptive, sz_t verbosity, bcore_sink* log ){} \
  static inline void bhpt_tutor_a_status_to_sink( const bhpt_tutor* o, sz_t verbosity, bcore_sink* sink ){ const bhpt_tutor_spect_s* p = bhpt_tutor_spect_s_get_aware( o ); assert( p->status_to_sink ); p->status_to_sink( o, verbosity, sink );} \
  static inline bl_t bhpt_tutor_a_defines_status_to_sink( const bhpt_tutor* o ){ return  true;} \
  static inline void bhpt_tutor_status_to_sink_default( const bhpt_tutor* o, sz_t verbosity, bcore_sink* sink ){ if( verbosity > 0 ) bcore_txt_ml_a_to_sink( o, sink );}

/**********************************************************************************************************************/
// source: bhpt_frame.h

//----------------------------------------------------------------------------------------------------------------------
// group: bhpt_frame

#define TYPEOF_bhpt_frame 0x949980B0AEF7A3B1ull
#define TYPEOF_bhpt_frame_spect_s 0x05DA4836CACE646Dull
#define TYPEOF_bhpt_frame_state_s 0xA6DF3772073FAF89ull
#define BETH_EXPAND_ITEM_bhpt_frame_state_s \
  BCORE_DECLARE_OBJECT( bhpt_frame_state_s ) \
  { \
      aware_t _; \
      sz_t cycle_number; \
      sz_t last_cycle_adapt; \
      sz_t last_cycle_test; \
      sz_t last_cycle_backup; \
      bhpt_adaptive* adaptive; \
      bhpt_adaptor_adl_s* adaptor_adl; \
  };
#define TYPEOF_bhpt_frame_s 0x822E2860E3E0C17Bull
#define BETH_EXPAND_ITEM_bhpt_frame_s \
  BCORE_DECLARE_OBJECT( bhpt_frame_s ) \
  { \
      aware_t _; \
      bhpt_tutor* tutor; \
      bhpt_frame_thread_base_s* thread_base; \
      bcore_main_frame_s* main_frame; \
      sz_t threads; \
      sz_t cycle_adapt; \
      sz_t cycle_test; \
      sz_t cycle_backup; \
      sz_t cycle_finish; \
      sz_t verbosity; \
      bhpt_frame_state_s* state; \
      bhvm_stats_s stats_grad; \
      st_s state_path; \
      bcore_sink* log; \
  }; \
  er_t bhpt_frame_s_main( bhpt_frame_s* o, bcore_main_frame_s* frame ); \
  static inline bl_t bhpt_frame_s_exit_required( const bhpt_frame_s* o ); \
  static inline bl_t bhpt_frame_s_exit_required( const bhpt_frame_s* o ){ return  bcore_main_frame_s_exit_required(o->main_frame);}
#define BETH_EXPAND_GROUP_bhpt_frame \
  BCORE_FORWARD_OBJECT( bhpt_frame ); \
  BCORE_FORWARD_OBJECT( bhpt_frame_thread ); \
  BCORE_FORWARD_OBJECT( bhpt_frame_state_s ); \
  BCORE_FORWARD_OBJECT( bhpt_frame_s ); \
  XOILA_DECLARE_SPECT( bhpt_frame ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bhpt_frame ) \
  BETH_EXPAND_GROUP_bhpt_frame_thread \
  BETH_EXPAND_ITEM_bhpt_frame_state_s \
  BETH_EXPAND_ITEM_bhpt_frame_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhpt_frame_thread

#define TYPEOF_bhpt_frame_thread 0xBA0FD71A0FBADA0Eull
#define TYPEOF_bhpt_frame_thread_spect_s 0x98D0B18D4AA15ABAull
#define TYPEOF_bhpt_frame_thread_share_s 0x25F3CFE39A2EF784ull
#define BETH_EXPAND_ITEM_bhpt_frame_thread_share_s \
  BCORE_DECLARE_OBJECT( bhpt_frame_thread_share_s ) \
  { \
      aware_t _; \
      bhpt_tutor* tutor; \
      bhpt_adaptive* adaptive; \
      bcore_condition_s* condition_item; \
      bcore_mutex_s* mutex; \
      sz_t finished_count; \
  };
#define TYPEOF_bhpt_frame_thread_item_s 0x00273FA5054A7AB6ull
#define BETH_EXPAND_ITEM_bhpt_frame_thread_item_s \
  BCORE_DECLARE_OBJECT( bhpt_frame_thread_item_s ) \
  { \
      aware_t _; \
      bl_t running; \
      sz_t prime_cycles; \
      bcore_thread_s* thread; \
      bcore_mutex_s* mutex; \
      bhpt_frame_thread_share_s* share; \
      bhpt_adaptive* adaptive; \
  }; \
  static inline void bhpt_frame_thread_item_s_down_e( bhpt_frame_thread_item_s* o ); \
  vd_t bhpt_frame_thread_item_s_loop( bhpt_frame_thread_item_s* o ); \
  void bhpt_frame_thread_item_s_loop_enter( bhpt_frame_thread_item_s* o ); \
  void bhpt_frame_thread_item_s_loop_exit( bhpt_frame_thread_item_s* o ); \
  static inline void bhpt_frame_thread_item_s_wait_while_locked( bhpt_frame_thread_item_s* o ); \
  static inline void bhpt_frame_thread_item_s_down_e( bhpt_frame_thread_item_s* o ){ ASSERT( !o->running );} \
  static inline void bhpt_frame_thread_item_s_wait_while_locked( bhpt_frame_thread_item_s* o ){ bcore_mutex_s_lock(o->mutex); bcore_mutex_s_unlock(o->mutex);}
#define TYPEOF_bhpt_frame_thread_ads_s 0x841A40DE09172B39ull
#define BETH_EXPAND_ITEM_bhpt_frame_thread_ads_s \
  BCORE_DECLARE_OBJECT( bhpt_frame_thread_ads_s ) \
  { \
      aware_t _; \
      BCORE_ARRAY_DYN_SOLID_STATIC_S( bhpt_frame_thread_item_s, ); \
  }; \
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
#define TYPEOF_bhpt_frame_thread_base_s 0xB52BEE650D4C2A30ull
#define BETH_EXPAND_ITEM_bhpt_frame_thread_base_s \
  BCORE_DECLARE_OBJECT( bhpt_frame_thread_base_s ) \
  { \
      aware_t _; \
      bhpt_frame_thread_ads_s ads; \
      bhpt_frame_thread_share_s* share; \
  }; \
  void bhpt_frame_thread_base_s_tsetup( bhpt_frame_thread_base_s* o, sz_t threads, bhpt_adaptive* adaptive, bhpt_tutor* tutor ); \
  void bhpt_frame_thread_base_s_tdown( bhpt_frame_thread_base_s* o ); \
  void bhpt_frame_thread_base_s_run( bhpt_frame_thread_base_s* o, sz_t cycles_per_thread ); \
  static inline void bhpt_frame_thread_base_s_down_e( bhpt_frame_thread_base_s* o ); \
  static inline void bhpt_frame_thread_base_s_copy_e( bhpt_frame_thread_base_s* o, const bhpt_frame_thread_base_s* src ); \
  static inline void bhpt_frame_thread_base_s_down_e( bhpt_frame_thread_base_s* o ){ bhpt_frame_thread_base_s_tdown(o);} \
  static inline void bhpt_frame_thread_base_s_copy_e( bhpt_frame_thread_base_s* o, const bhpt_frame_thread_base_s* src ){ bhpt_frame_thread_base_s_tdown(o);}
#define BETH_EXPAND_GROUP_bhpt_frame_thread \
  BCORE_FORWARD_OBJECT( bhpt_frame_thread ); \
  BCORE_FORWARD_OBJECT( bhpt_frame_thread_share_s ); \
  BCORE_FORWARD_OBJECT( bhpt_frame_thread_item_s ); \
  BCORE_FORWARD_OBJECT( bhpt_frame_thread_ads_s ); \
  BCORE_FORWARD_OBJECT( bhpt_frame_thread_base_s ); \
  XOILA_DECLARE_SPECT( bhpt_frame_thread ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bhpt_frame_thread ) \
  BETH_EXPAND_ITEM_bhpt_frame_thread_share_s \
  BETH_EXPAND_ITEM_bhpt_frame_thread_item_s \
  BETH_EXPAND_ITEM_bhpt_frame_thread_ads_s \
  BETH_EXPAND_ITEM_bhpt_frame_thread_base_s

/**********************************************************************************************************************/
// source: bhpt_adaptor.h

//----------------------------------------------------------------------------------------------------------------------
// group: bhpt_adaptor_stamp

#define TYPEOF_bhpt_adaptor_stamp 0xF223BA3B87EF6C61ull
#define TYPEOF_bhpt_adaptor_stamp_spect_s 0x5B46A10315B6A6DDull
#define TYPEOF_bhpt_adaptor_epsilon_s 0x859114A321EC62F6ull
#define BETH_EXPAND_ITEM_bhpt_adaptor_epsilon_s \
  BCORE_DECLARE_OBJECT( bhpt_adaptor_epsilon_s ) \
  { \
      aware_t _; \
      f3_t epsilon; \
  }; \
  static inline void bhpt_adaptor_epsilon_s_reset( bhpt_adaptor_epsilon_s* o ); \
  void bhpt_adaptor_epsilon_s_adapt( bhpt_adaptor_epsilon_s* o, const bhpt_adaptor_node_s* node ); \
  static inline void bhpt_adaptor_epsilon_s_reset( bhpt_adaptor_epsilon_s* o ){}
#define TYPEOF_bhpt_adaptor_reg_l2_s 0xA9D7B91762269735ull
#define BETH_EXPAND_ITEM_bhpt_adaptor_reg_l2_s \
  BCORE_DECLARE_OBJECT( bhpt_adaptor_reg_l2_s ) \
  { \
      aware_t _; \
      f3_t lambda; \
  }; \
  static inline void bhpt_adaptor_reg_l2_s_reset( bhpt_adaptor_reg_l2_s* o ); \
  static inline void bhpt_adaptor_reg_l2_s_adapt( bhpt_adaptor_reg_l2_s* o, const bhpt_adaptor_node_s* node ); \
  static inline void bhpt_adaptor_reg_l2_s_reset( bhpt_adaptor_reg_l2_s* o ){} \
  static inline void bhpt_adaptor_reg_l2_s_adapt( bhpt_adaptor_reg_l2_s* o, const bhpt_adaptor_node_s* node ){ bhvm_value_s_mul_scl_f3_acc( &(node->axon->v),-o->lambda, (&(node->axon->v)) );}
#define TYPEOF_bhpt_adaptor_zro_grad_s 0xCF651CF4E354F6B6ull
#define BETH_EXPAND_ITEM_bhpt_adaptor_zro_grad_s \
  BCORE_DECLARE_OBJECT( bhpt_adaptor_zro_grad_s ) \
  { \
      aware_t _; \
  }; \
  static inline void bhpt_adaptor_zro_grad_s_adapt( bhpt_adaptor_zro_grad_s* o, const bhpt_adaptor_node_s* node ); \
  static inline void bhpt_adaptor_zro_grad_s_adapt( bhpt_adaptor_zro_grad_s* o, const bhpt_adaptor_node_s* node ){ bhvm_value_s_zro( &(node->grad->v));}
#define TYPEOF_bhpt_adaptor_list_s 0x3275AEBFFED96FA8ull
#define BETH_EXPAND_ITEM_bhpt_adaptor_list_s \
  BCORE_DECLARE_OBJECT( bhpt_adaptor_list_s ) \
  { \
      aware_t _; \
      BCORE_ARRAY_DYN_LINK_STATIC_S( bhpt_adaptor, ); \
  }; \
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
  XOILA_DECLARE_SPECT( bhpt_adaptor_stamp ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bhpt_adaptor_stamp ) \
  BETH_EXPAND_ITEM_bhpt_adaptor_epsilon_s \
  BETH_EXPAND_ITEM_bhpt_adaptor_reg_l2_s \
  BETH_EXPAND_ITEM_bhpt_adaptor_zro_grad_s \
  BETH_EXPAND_ITEM_bhpt_adaptor_list_s

/**********************************************************************************************************************/
// source: bhpt_tutor_sampler.h

//----------------------------------------------------------------------------------------------------------------------
// group: bhpt_sampler

#define TYPEOF_bhpt_sampler 0xFA5C9B139D6FFD84ull
#define TYPEOF_bhpt_sampler_spect_s 0x8BE5C1F9D1E46D7Cull
#define TYPEOF_bhpt_sampler_sine_random_s 0x36B14869A6034D30ull
#define BETH_EXPAND_ITEM_bhpt_sampler_sine_random_s \
  BCORE_DECLARE_OBJECT( bhpt_sampler_sine_random_s ) \
  { \
      aware_t _; \
      sz_t size_en; \
      f3_t pos_tgt; \
      f3_t neg_tgt; \
  }; \
  void bhpt_sampler_sine_random_s_fetch( const bhpt_sampler_sine_random_s* o, bcore_prsg* prsg, bhvm_value_s* x, bhvm_value_s* y ); \
  static inline sz_t bhpt_sampler_sine_random_s_get_size_en( const bhpt_sampler_sine_random_s* o ); \
  static inline sz_t bhpt_sampler_sine_random_s_get_size_ex( const bhpt_sampler_sine_random_s* o ); \
  static inline sz_t bhpt_sampler_sine_random_s_get_size_en( const bhpt_sampler_sine_random_s* o ){ return  o->size_en;} \
  static inline sz_t bhpt_sampler_sine_random_s_get_size_ex( const bhpt_sampler_sine_random_s* o ){ return  1;}
#define TYPEOF_bhpt_sampler_binary_add_s 0x9FD3A65F6B6F81FEull
#define BETH_EXPAND_ITEM_bhpt_sampler_binary_add_s \
  BCORE_DECLARE_OBJECT( bhpt_sampler_binary_add_s ) \
  { \
      aware_t _; \
      sz_t bits; \
      f3_t val_h; \
      f3_t val_l; \
  }; \
  void bhpt_sampler_binary_add_s_fetch( const bhpt_sampler_binary_add_s* o, bcore_prsg* prsg, bhvm_value_s* x, bhvm_value_s* y ); \
  static inline sz_t bhpt_sampler_binary_add_s_get_size_en( const bhpt_sampler_binary_add_s* o ); \
  static inline sz_t bhpt_sampler_binary_add_s_get_size_ex( const bhpt_sampler_binary_add_s* o ); \
  static inline sz_t bhpt_sampler_binary_add_s_get_size_en( const bhpt_sampler_binary_add_s* o ){ return  o->bits * 2;} \
  static inline sz_t bhpt_sampler_binary_add_s_get_size_ex( const bhpt_sampler_binary_add_s* o ){ return  o->bits + 1;}
#define TYPEOF_bhpt_sampler_binary_mul_s 0x4B175C12902FBF03ull
#define BETH_EXPAND_ITEM_bhpt_sampler_binary_mul_s \
  BCORE_DECLARE_OBJECT( bhpt_sampler_binary_mul_s ) \
  { \
      aware_t _; \
      sz_t bits; \
      f3_t val_h; \
      f3_t val_l; \
  }; \
  void bhpt_sampler_binary_mul_s_fetch( const bhpt_sampler_binary_mul_s* o, bcore_prsg* prsg, bhvm_value_s* x, bhvm_value_s* y ); \
  static inline sz_t bhpt_sampler_binary_mul_s_get_size_en( const bhpt_sampler_binary_mul_s* o ); \
  static inline sz_t bhpt_sampler_binary_mul_s_get_size_ex( const bhpt_sampler_binary_mul_s* o ); \
  static inline sz_t bhpt_sampler_binary_mul_s_get_size_en( const bhpt_sampler_binary_mul_s* o ){ return  o->bits * 2;} \
  static inline sz_t bhpt_sampler_binary_mul_s_get_size_ex( const bhpt_sampler_binary_mul_s* o ){ return  o->bits * 2;}
#define TYPEOF_bhpt_sampler_binary_lcg00_s 0x615CED88873A5BBFull
#define BETH_EXPAND_ITEM_bhpt_sampler_binary_lcg00_s \
  BCORE_DECLARE_OBJECT( bhpt_sampler_binary_lcg00_s ) \
  { \
      aware_t _; \
      sz_t bits; \
      f3_t val_h; \
      f3_t val_l; \
  }; \
  void bhpt_sampler_binary_lcg00_s_fetch( const bhpt_sampler_binary_lcg00_s* o, bcore_prsg* prsg, bhvm_value_s* x, bhvm_value_s* y ); \
  static inline sz_t bhpt_sampler_binary_lcg00_s_get_size_en( const bhpt_sampler_binary_lcg00_s* o ); \
  static inline sz_t bhpt_sampler_binary_lcg00_s_get_size_ex( const bhpt_sampler_binary_lcg00_s* o ); \
  static inline sz_t bhpt_sampler_binary_lcg00_s_get_size_en( const bhpt_sampler_binary_lcg00_s* o ){ return  o->bits;} \
  static inline sz_t bhpt_sampler_binary_lcg00_s_get_size_ex( const bhpt_sampler_binary_lcg00_s* o ){ return  o->bits;}
#define TYPEOF_bhpt_sampler_binary_hash_s 0xA8C69E282CA50BA3ull
#define BETH_EXPAND_ITEM_bhpt_sampler_binary_hash_s \
  BCORE_DECLARE_OBJECT( bhpt_sampler_binary_hash_s ) \
  { \
      aware_t _; \
      sz_t bits; \
      f3_t val_h; \
      f3_t val_l; \
      bl_t reverse; \
  }; \
  void bhpt_sampler_binary_hash_s_fetch( const bhpt_sampler_binary_hash_s* o, bcore_prsg* prsg, bhvm_value_s* x, bhvm_value_s* y ); \
  static inline sz_t bhpt_sampler_binary_hash_s_get_size_en( const bhpt_sampler_binary_hash_s* o ); \
  static inline sz_t bhpt_sampler_binary_hash_s_get_size_ex( const bhpt_sampler_binary_hash_s* o ); \
  static inline sz_t bhpt_sampler_binary_hash_s_get_size_en( const bhpt_sampler_binary_hash_s* o ){ return  o->bits;} \
  static inline sz_t bhpt_sampler_binary_hash_s_get_size_ex( const bhpt_sampler_binary_hash_s* o ){ return  o->bits;}
#define TYPEOF_bhpt_sampler_polynom_s 0x9B6624FAD6D25D59ull
#define BETH_EXPAND_ITEM_bhpt_sampler_polynom_s \
  BCORE_DECLARE_OBJECT( bhpt_sampler_polynom_s ) \
  { \
      aware_t _; \
      sz_t size_en; \
      sz_t size_ex; \
      f3_t range; \
      f3_t noise_level; \
  }; \
  void bhpt_sampler_polynom_s_fetch( const bhpt_sampler_polynom_s* o, bcore_prsg* prsg, bhvm_value_s* x, bhvm_value_s* y ); \
  static inline sz_t bhpt_sampler_polynom_s_get_size_en( const bhpt_sampler_polynom_s* o ); \
  static inline sz_t bhpt_sampler_polynom_s_get_size_ex( const bhpt_sampler_polynom_s* o ); \
  static inline sz_t bhpt_sampler_polynom_s_get_size_en( const bhpt_sampler_polynom_s* o ){ return  o->size_en;} \
  static inline sz_t bhpt_sampler_polynom_s_get_size_ex( const bhpt_sampler_polynom_s* o ){ return  o->size_ex;}
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
  XOILA_DECLARE_SPECT( bhpt_sampler ) \
  { \
      bcore_spect_header_s header; \
      bhpt_sampler_fetch fetch; \
      bhpt_sampler_get_size_en get_size_en; \
      bhpt_sampler_get_size_ex get_size_ex; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bhpt_sampler ) \
  static inline void bhpt_sampler_a_fetch( const bhpt_sampler* o, bcore_prsg* prsg, bhvm_value_s* x, bhvm_value_s* y ){ const bhpt_sampler_spect_s* p = bhpt_sampler_spect_s_get_aware( o ); assert( p->fetch ); p->fetch( o, prsg, x, y );} \
  static inline bl_t bhpt_sampler_a_defines_fetch( const bhpt_sampler* o ){ return  true;} \
  static inline sz_t bhpt_sampler_a_get_size_en( const bhpt_sampler* o ){ const bhpt_sampler_spect_s* p = bhpt_sampler_spect_s_get_aware( o ); assert( p->get_size_en ); return p->get_size_en( o );} \
  static inline bl_t bhpt_sampler_a_defines_get_size_en( const bhpt_sampler* o ){ return  true;} \
  static inline sz_t bhpt_sampler_a_get_size_ex( const bhpt_sampler* o ){ const bhpt_sampler_spect_s* p = bhpt_sampler_spect_s_get_aware( o ); assert( p->get_size_ex ); return p->get_size_ex( o );} \
  static inline bl_t bhpt_sampler_a_defines_get_size_ex( const bhpt_sampler* o ){ return  true;} \
  BETH_EXPAND_ITEM_bhpt_sampler_sine_random_s \
  BETH_EXPAND_ITEM_bhpt_sampler_binary_add_s \
  BETH_EXPAND_ITEM_bhpt_sampler_binary_mul_s \
  BETH_EXPAND_ITEM_bhpt_sampler_binary_lcg00_s \
  BETH_EXPAND_ITEM_bhpt_sampler_binary_hash_s \
  BETH_EXPAND_ITEM_bhpt_sampler_polynom_s

//----------------------------------------------------------------------------------------------------------------------
// group: bhpt_tutor_sampler

#define TYPEOF_bhpt_tutor_sampler 0xEC15340B67783F5Full
#define TYPEOF_bhpt_tutor_sampler_spect_s 0x0D7FC6B80D33800Full
#define TYPEOF_bhpt_tutor_sampler_s 0xAABCB483B01FDA69ull
#define BETH_EXPAND_ITEM_bhpt_tutor_sampler_s \
  BCORE_DECLARE_OBJECT( bhpt_tutor_sampler_s ) \
  { \
      aware_t _; \
      bhpt_builder* builder; \
      bhpt_adaptor* adaptor; \
      bhpt_sampler* sampler; \
      bcore_prsg* prsg_priming; \
      bcore_prsg* prsg_testing; \
      sz_t test_size; \
      bcore_mutex_s mutex; \
  }; \
  static inline void bhpt_tutor_sampler_s_reset( bhpt_tutor_sampler_s* o ); \
  bhpt_adaptive* bhpt_tutor_sampler_s_create_adaptive( const bhpt_tutor_sampler_s* o ); \
  static inline bhpt_adaptor* bhpt_tutor_sampler_s_create_adaptor( const bhpt_tutor_sampler_s* o ); \
  void bhpt_tutor_sampler_s_prime( bhpt_tutor_sampler_s* o, bhpt_adaptive* adaptive ); \
  void bhpt_tutor_sampler_s_test( bhpt_tutor_sampler_s* o, const bhpt_adaptive* adaptive, sz_t verbosity, bcore_sink* log ); \
  void bhpt_tutor_sampler_s_status_to_sink( const bhpt_tutor_sampler_s* o, sz_t verbosity, bcore_sink* sink ); \
  static inline void bhpt_tutor_sampler_s_reset( bhpt_tutor_sampler_s* o ){} \
  static inline bhpt_adaptor* bhpt_tutor_sampler_s_create_adaptor( const bhpt_tutor_sampler_s* o ){ return  bhpt_adaptor_a_clone(o->adaptor);}
#define BETH_EXPAND_GROUP_bhpt_tutor_sampler \
  BCORE_FORWARD_OBJECT( bhpt_tutor_sampler ); \
  BCORE_FORWARD_OBJECT( bhpt_tutor_sampler_s ); \
  XOILA_DECLARE_SPECT( bhpt_tutor_sampler ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bhpt_tutor_sampler ) \
  BETH_EXPAND_ITEM_bhpt_tutor_sampler_s

/**********************************************************************************************************************/
// source: bhpt_tutor_language.h

//----------------------------------------------------------------------------------------------------------------------
// group: bhpt_tutor_language

#define TYPEOF_bhpt_tutor_language 0xEE9959202264240Bull
#define TYPEOF_bhpt_tutor_language_spect_s 0x654B4A1997BFFEEBull
#define TYPEOF_bhpt_tutor_language_utf8_chatter_s 0xC9C2DCA78A81343Full
#define BETH_EXPAND_ITEM_bhpt_tutor_language_utf8_chatter_s \
  BCORE_DECLARE_OBJECT( bhpt_tutor_language_utf8_chatter_s ) \
  { \
      aware_t _; \
      bl_t cyclic_reset; \
      st_s trigger; \
      sz_t size_line; \
      sz_t size_lines; \
      bl_t newline_to_space; \
      f3_t heat; \
  };
#define TYPEOF_bhpt_tutor_language_utf8_s 0xFB74B1BFF7676A1Dull
#define BETH_EXPAND_ITEM_bhpt_tutor_language_utf8_s \
  BCORE_DECLARE_OBJECT( bhpt_tutor_language_utf8_s ) \
  { \
      aware_t _; \
      bhpt_builder* builder; \
      bhpt_adaptor* adaptor; \
      vd_t src; \
      st_s* st; \
      bcore_prsg* prsg_priming; \
      bcore_prsg* prsg_testing; \
      sz_t size_trans; \
      sz_t size_prime; \
      sz_t size_test; \
      f3_t tgt_pos; \
      f3_t tgt_neg; \
      bhpt_tutor_language_utf8_chatter_s* chatter; \
      bcore_mutex_s mutex; \
  }; \
  static inline void bhpt_tutor_language_utf8_s_reset( bhpt_tutor_language_utf8_s* o ); \
  bhpt_adaptive* bhpt_tutor_language_utf8_s_create_adaptive( const bhpt_tutor_language_utf8_s* o ); \
  static inline bhpt_adaptor* bhpt_tutor_language_utf8_s_create_adaptor( const bhpt_tutor_language_utf8_s* o ); \
  void bhpt_tutor_language_utf8_s_prime( bhpt_tutor_language_utf8_s* o, bhpt_adaptive* adaptive ); \
  void bhpt_tutor_language_utf8_s_test( bhpt_tutor_language_utf8_s* o, const bhpt_adaptive* adaptive, sz_t verbosity, bcore_sink* log ); \
  void bhpt_tutor_language_utf8_s_status_to_sink( const bhpt_tutor_language_utf8_s* o, sz_t verbosity, bcore_sink* sink ); \
  static inline void bhpt_tutor_language_utf8_s_reset( bhpt_tutor_language_utf8_s* o ){ bcore_prsg_a_attach( &(o->prsg_priming ), (bcore_prsg*)( bcore_prsg_lcg_u3_00_s_create()));} \
  static inline bhpt_adaptor* bhpt_tutor_language_utf8_s_create_adaptor( const bhpt_tutor_language_utf8_s* o ){ return  bhpt_adaptor_a_clone(o->adaptor);}
#define BETH_EXPAND_GROUP_bhpt_tutor_language \
  BCORE_FORWARD_OBJECT( bhpt_tutor_language ); \
  BCORE_FORWARD_OBJECT( bhpt_tutor_language_utf8_chatter_s ); \
  BCORE_FORWARD_OBJECT( bhpt_tutor_language_utf8_s ); \
  XOILA_DECLARE_SPECT( bhpt_tutor_language ) \
  { \
      bcore_spect_header_s header; \
  }; \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( bhpt_tutor_language ) \
  BETH_EXPAND_ITEM_bhpt_tutor_language_utf8_chatter_s \
  BETH_EXPAND_ITEM_bhpt_tutor_language_utf8_s

/**********************************************************************************************************************/

vd_t bhpt_xo_signal_handler( const bcore_signal_s* o );

#endif // __bhpt_xo_H
// XOILA_OUT_SIGNATURE 0x87D92FDD55B26E7Dull
