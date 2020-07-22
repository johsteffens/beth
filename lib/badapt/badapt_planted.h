/** This file was generated from beth-plant source code.
 *  Compiling Agent : bcore_plant_compiler (C) 2019, 2020 J.B.Steffens
 *  Last File Update: 2020-07-22T09:05:39Z
 *
 *  Copyright and License of this File:
 *
 *  Generated code inherits the copyright and license of the underlying beth-plant source code.
 *  Source code defining this file is distributed across following files:
 *
 *  badapt_adaptive.h
 *  badapt_problem.h
 *  badapt_activator.h
 *  badapt_loss.h
 *  badapt_mlp.h
 *  badapt_c1d.h
 *  badapt_ern.h
 *  badapt_lstm.h
 *  badapt_training.h
 *  badapt_trainer.h
 *
 */

#ifndef BADAPT_PLANTED_H
#define BADAPT_PLANTED_H

#include "bcore_control.h"

//To force a rebuild of this target by the plant-compiler, reset the hash key value below to 0.
#define HKEYOF_badapt_planted 1194511176

#define TYPEOF_badapt_planted 2807899404

/**********************************************************************************************************************/
// source: badapt_adaptive.h

//----------------------------------------------------------------------------------------------------------------------
// group: badapt_dynamics

#define TYPEOF_badapt_dynamics 776275350
#define TYPEOF_badapt_dynamics_s 1839376072
#define TYPEOF_badapt_dynamics_std_s 2426636234
#define BETH_EXPAND_ITEM_badapt_dynamics_std_s \
  BCORE_DECLARE_OBJECT( badapt_dynamics_std_s ) \
    {aware_t _;f3_t epsilon;f3_t lambda_l1;f3_t lambda_l2;}; \
  void badapt_dynamics_std_s_weights_adapt( const badapt_dynamics_std_s* o, const bmath_vf3_s* in, bmath_mf3_s* w, const bmath_vf3_s* grad_out, f3_t epsilon_factor );
#define BETH_EXPAND_GROUP_badapt_dynamics \
  BCORE_FORWARD_OBJECT( badapt_dynamics ); \
  BCORE_FORWARD_OBJECT( badapt_dynamics_std_s ); \
  typedef void (*badapt_dynamics_weights_adapt)( const badapt_dynamics* o, const bmath_vf3_s* in, bmath_mf3_s* w, const bmath_vf3_s* grad_out, f3_t epsilon_factor ); \
  BCORE_DECLARE_SPECT( badapt_dynamics ) \
  { \
      bcore_spect_header_s header; \
      badapt_dynamics_weights_adapt weights_adapt; \
  }; \
  static inline badapt_dynamics* badapt_dynamics_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_badapt_dynamics, t ); return ( badapt_dynamics* )bcore_inst_t_create( t ); } \
  static inline bl_t badapt_dynamics_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_badapt_dynamics ); } \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( badapt_dynamics ) \
  static inline bl_t badapt_dynamics_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_badapt_dynamics ); } \
  static inline void badapt_dynamics_a_weights_adapt( const badapt_dynamics* o, const bmath_vf3_s* in, bmath_mf3_s* w, const bmath_vf3_s* grad_out, f3_t epsilon_factor ) { const badapt_dynamics_s* p = badapt_dynamics_s_get_aware( o ); assert( p->weights_adapt ); p->weights_adapt( o, in, w, grad_out, epsilon_factor ); } \
  static inline bl_t badapt_dynamics_a_defines_weights_adapt( const badapt_dynamics* o ) { return true; } \
  BETH_EXPAND_ITEM_badapt_dynamics_std_s

//----------------------------------------------------------------------------------------------------------------------
// group: badapt_adaptive

#define TYPEOF_badapt_adaptive 3626296374
#define TYPEOF_badapt_adaptive_s 2278398760
#define BETH_EXPAND_GROUP_badapt_adaptive \
  BCORE_FORWARD_OBJECT( badapt_adaptive ); \
  typedef sz_t (*badapt_adaptive_get_in_size)( const badapt_adaptive* o ); \
  typedef sz_t (*badapt_adaptive_get_out_size)( const badapt_adaptive* o ); \
  typedef void (*badapt_adaptive_infer)( const badapt_adaptive* o, const bmath_vf3_s* in, bmath_vf3_s* out ); \
  typedef void (*badapt_adaptive_minfer)( badapt_adaptive* o, const bmath_vf3_s* in, bmath_vf3_s* out ); \
  typedef void (*badapt_adaptive_bgrad)( const badapt_adaptive* o, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out ); \
  typedef void (*badapt_adaptive_bgrad_adapt)( badapt_adaptive* o, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out ); \
  typedef void (*badapt_adaptive_reset)( badapt_adaptive* o ); \
  typedef void (*badapt_adaptive_get_weights_min_max)( const badapt_adaptive* o, f3_t* min, f3_t* max ); \
  typedef void (*badapt_adaptive_get_dynamics_std)( const badapt_adaptive* o, badapt_dynamics_std_s* dynamics ); \
  typedef void (*badapt_adaptive_set_dynamics_std)( badapt_adaptive* o, const badapt_dynamics_std_s* dynamics ); \
  typedef void (*badapt_adaptive_arc_to_sink)( const badapt_adaptive* o, bcore_sink* sink ); \
  typedef f3_t (*badapt_adaptive_infer_f3)( const badapt_adaptive* o, const bmath_vf3_s* in ); \
  typedef void (*badapt_adaptive_adapt_loss)( badapt_adaptive* o, const badapt_loss* loss, const bmath_vf3_s* in, const bmath_vf3_s* target, bmath_vf3_s* out ); \
  typedef f3_t (*badapt_adaptive_adapt_loss_f3)( badapt_adaptive* o, const badapt_loss* loss, const bmath_vf3_s* in, f3_t target ); \
  BCORE_DECLARE_SPECT( badapt_adaptive ) \
  { \
      bcore_spect_header_s header; \
      badapt_adaptive_get_in_size get_in_size; \
      badapt_adaptive_get_out_size get_out_size; \
      badapt_adaptive_infer infer; \
      badapt_adaptive_minfer minfer; \
      badapt_adaptive_bgrad bgrad; \
      badapt_adaptive_bgrad_adapt bgrad_adapt; \
      badapt_adaptive_reset reset; \
      badapt_adaptive_get_weights_min_max get_weights_min_max; \
      badapt_adaptive_get_dynamics_std get_dynamics_std; \
      badapt_adaptive_set_dynamics_std set_dynamics_std; \
      badapt_adaptive_arc_to_sink arc_to_sink; \
      badapt_adaptive_infer_f3 infer_f3; \
      badapt_adaptive_adapt_loss adapt_loss; \
      badapt_adaptive_adapt_loss_f3 adapt_loss_f3; \
  }; \
  static inline badapt_adaptive* badapt_adaptive_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_badapt_adaptive, t ); return ( badapt_adaptive* )bcore_inst_t_create( t ); } \
  static inline bl_t badapt_adaptive_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_badapt_adaptive ); } \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( badapt_adaptive ) \
  static inline bl_t badapt_adaptive_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_badapt_adaptive ); } \
  static inline sz_t badapt_adaptive_a_get_in_size( const badapt_adaptive* o ) { const badapt_adaptive_s* p = badapt_adaptive_s_get_aware( o ); assert( p->get_in_size ); return p->get_in_size( o ); } \
  static inline bl_t badapt_adaptive_a_defines_get_in_size( const badapt_adaptive* o ) { return true; } \
  static inline sz_t badapt_adaptive_a_get_out_size( const badapt_adaptive* o ) { const badapt_adaptive_s* p = badapt_adaptive_s_get_aware( o ); assert( p->get_out_size ); return p->get_out_size( o ); } \
  static inline bl_t badapt_adaptive_a_defines_get_out_size( const badapt_adaptive* o ) { return true; } \
  static inline void badapt_adaptive_a_infer( const badapt_adaptive* o, const bmath_vf3_s* in, bmath_vf3_s* out ) { const badapt_adaptive_s* p = badapt_adaptive_s_get_aware( o ); assert( p->infer ); p->infer( o, in, out ); } \
  static inline bl_t badapt_adaptive_a_defines_infer( const badapt_adaptive* o ) { return badapt_adaptive_s_get_aware( o )->infer != NULL; } \
  static inline void badapt_adaptive_a_minfer( badapt_adaptive* o, const bmath_vf3_s* in, bmath_vf3_s* out ) { const badapt_adaptive_s* p = badapt_adaptive_s_get_aware( o ); assert( p->minfer ); p->minfer( o, in, out ); } \
  static inline bl_t badapt_adaptive_a_defines_minfer( const badapt_adaptive* o ) { return true; } \
  static inline void badapt_adaptive_a_bgrad( const badapt_adaptive* o, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out ) { const badapt_adaptive_s* p = badapt_adaptive_s_get_aware( o ); assert( p->bgrad ); p->bgrad( o, grad_in, grad_out ); } \
  static inline bl_t badapt_adaptive_a_defines_bgrad( const badapt_adaptive* o ) { return badapt_adaptive_s_get_aware( o )->bgrad != NULL; } \
  static inline void badapt_adaptive_a_bgrad_adapt( badapt_adaptive* o, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out ) { const badapt_adaptive_s* p = badapt_adaptive_s_get_aware( o ); assert( p->bgrad_adapt ); p->bgrad_adapt( o, grad_in, grad_out ); } \
  static inline bl_t badapt_adaptive_a_defines_bgrad_adapt( const badapt_adaptive* o ) { return true; } \
  static inline void badapt_adaptive_a_reset( badapt_adaptive* o ) { const badapt_adaptive_s* p = badapt_adaptive_s_get_aware( o ); assert( p->reset ); p->reset( o ); } \
  static inline bl_t badapt_adaptive_a_defines_reset( const badapt_adaptive* o ) { return badapt_adaptive_s_get_aware( o )->reset != NULL; } \
  static inline void badapt_adaptive_a_get_weights_min_max( const badapt_adaptive* o, f3_t* min, f3_t* max ) { const badapt_adaptive_s* p = badapt_adaptive_s_get_aware( o ); assert( p->get_weights_min_max ); p->get_weights_min_max( o, min, max ); } \
  static inline bl_t badapt_adaptive_a_defines_get_weights_min_max( const badapt_adaptive* o ) { return badapt_adaptive_s_get_aware( o )->get_weights_min_max != NULL; } \
  static inline void badapt_adaptive_a_get_dynamics_std( const badapt_adaptive* o, badapt_dynamics_std_s* dynamics ) { const badapt_adaptive_s* p = badapt_adaptive_s_get_aware( o ); assert( p->get_dynamics_std ); p->get_dynamics_std( o, dynamics ); } \
  static inline bl_t badapt_adaptive_a_defines_get_dynamics_std( const badapt_adaptive* o ) { return badapt_adaptive_s_get_aware( o )->get_dynamics_std != NULL; } \
  static inline void badapt_adaptive_a_set_dynamics_std( badapt_adaptive* o, const badapt_dynamics_std_s* dynamics ) { const badapt_adaptive_s* p = badapt_adaptive_s_get_aware( o ); assert( p->set_dynamics_std ); p->set_dynamics_std( o, dynamics ); } \
  static inline bl_t badapt_adaptive_a_defines_set_dynamics_std( const badapt_adaptive* o ) { return badapt_adaptive_s_get_aware( o )->set_dynamics_std != NULL; } \
  static inline void badapt_adaptive_a_arc_to_sink( const badapt_adaptive* o, bcore_sink* sink ) { const badapt_adaptive_s* p = badapt_adaptive_s_get_aware( o ); assert( p->arc_to_sink ); p->arc_to_sink( o, sink ); } \
  static inline bl_t badapt_adaptive_a_defines_arc_to_sink( const badapt_adaptive* o ) { return true; } \
  static inline void badapt_adaptive_arc_to_sink__( const badapt_adaptive* o, bcore_sink* sink ){bcore_txt_ml_a_to_sink( o, sink );} \
  static inline f3_t badapt_adaptive_a_infer_f3( const badapt_adaptive* o, const bmath_vf3_s* in ) { const badapt_adaptive_s* p = badapt_adaptive_s_get_aware( o ); assert( p->infer_f3 ); return p->infer_f3( o, in ); } \
  static inline bl_t badapt_adaptive_a_defines_infer_f3( const badapt_adaptive* o ) { return true; } \
  f3_t badapt_adaptive_infer_f3__( const badapt_adaptive* o, const bmath_vf3_s* in ); \
  static inline void badapt_adaptive_a_adapt_loss( badapt_adaptive* o, const badapt_loss* loss, const bmath_vf3_s* in, const bmath_vf3_s* target, bmath_vf3_s* out ) { const badapt_adaptive_s* p = badapt_adaptive_s_get_aware( o ); assert( p->adapt_loss ); p->adapt_loss( o, loss, in, target, out ); } \
  static inline bl_t badapt_adaptive_a_defines_adapt_loss( const badapt_adaptive* o ) { return true; } \
  void badapt_adaptive_adapt_loss__( badapt_adaptive* o, const badapt_loss* loss, const bmath_vf3_s* in, const bmath_vf3_s* target, bmath_vf3_s* out ); \
  static inline f3_t badapt_adaptive_a_adapt_loss_f3( badapt_adaptive* o, const badapt_loss* loss, const bmath_vf3_s* in, f3_t target ) { const badapt_adaptive_s* p = badapt_adaptive_s_get_aware( o ); assert( p->adapt_loss_f3 ); return p->adapt_loss_f3( o, loss, in, target ); } \
  static inline bl_t badapt_adaptive_a_defines_adapt_loss_f3( const badapt_adaptive* o ) { return true; } \
  f3_t badapt_adaptive_adapt_loss_f3__( badapt_adaptive* o, const badapt_loss* loss, const bmath_vf3_s* in, f3_t target );

//----------------------------------------------------------------------------------------------------------------------
// group: badapt_builder

#define TYPEOF_badapt_builder 2907309879
#define TYPEOF_badapt_builder_s 3971266449
#define BETH_EXPAND_GROUP_badapt_builder \
  BCORE_FORWARD_OBJECT( badapt_builder ); \
  typedef sz_t (*badapt_builder_get_in_size)( const badapt_builder* o ); \
  typedef void (*badapt_builder_set_in_size)( badapt_builder* o, sz_t size ); \
  typedef sz_t (*badapt_builder_get_out_size)( const badapt_builder* o ); \
  typedef void (*badapt_builder_set_out_size)( badapt_builder* o, sz_t size ); \
  typedef badapt_adaptive* (*badapt_builder_build)( const badapt_builder* o ); \
  BCORE_DECLARE_SPECT( badapt_builder ) \
  { \
      bcore_spect_header_s header; \
      badapt_builder_get_in_size get_in_size; \
      badapt_builder_set_in_size set_in_size; \
      badapt_builder_get_out_size get_out_size; \
      badapt_builder_set_out_size set_out_size; \
      badapt_builder_build build; \
  }; \
  static inline badapt_builder* badapt_builder_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_badapt_builder, t ); return ( badapt_builder* )bcore_inst_t_create( t ); } \
  static inline bl_t badapt_builder_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_badapt_builder ); } \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( badapt_builder ) \
  static inline bl_t badapt_builder_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_badapt_builder ); } \
  static inline sz_t badapt_builder_a_get_in_size( const badapt_builder* o ) { const badapt_builder_s* p = badapt_builder_s_get_aware( o ); assert( p->get_in_size ); return p->get_in_size( o ); } \
  static inline bl_t badapt_builder_a_defines_get_in_size( const badapt_builder* o ) { return true; } \
  static inline void badapt_builder_a_set_in_size( badapt_builder* o, sz_t size ) { const badapt_builder_s* p = badapt_builder_s_get_aware( o ); assert( p->set_in_size ); p->set_in_size( o, size ); } \
  static inline bl_t badapt_builder_a_defines_set_in_size( const badapt_builder* o ) { return true; } \
  static inline sz_t badapt_builder_a_get_out_size( const badapt_builder* o ) { const badapt_builder_s* p = badapt_builder_s_get_aware( o ); assert( p->get_out_size ); return p->get_out_size( o ); } \
  static inline bl_t badapt_builder_a_defines_get_out_size( const badapt_builder* o ) { return true; } \
  static inline void badapt_builder_a_set_out_size( badapt_builder* o, sz_t size ) { const badapt_builder_s* p = badapt_builder_s_get_aware( o ); assert( p->set_out_size ); p->set_out_size( o, size ); } \
  static inline bl_t badapt_builder_a_defines_set_out_size( const badapt_builder* o ) { return true; } \
  static inline badapt_adaptive* badapt_builder_a_build( const badapt_builder* o ) { const badapt_builder_s* p = badapt_builder_s_get_aware( o ); assert( p->build ); return p->build( o ); } \
  static inline bl_t badapt_builder_a_defines_build( const badapt_builder* o ) { return true; }

/**********************************************************************************************************************/
// source: badapt_problem.h

//----------------------------------------------------------------------------------------------------------------------
// group: badapt_problem

#define TYPEOF_badapt_problem 978854679
#define TYPEOF_badapt_problem_s 3095822769
#define TYPEOF_badapt_problem_sine_random_s 232405763
#define BETH_EXPAND_ITEM_badapt_problem_sine_random_s \
  BCORE_DECLARE_OBJECT( badapt_problem_sine_random_s ) \
    {aware_t _;sz_t input_size;u3_t rval;f3_t pos_tgt;f3_t neg_tgt;badapt_loss* preferred_loss;}; \
  static inline const badapt_loss* badapt_problem_sine_random_s_preferred_loss( const badapt_problem_sine_random_s* o ){return o->preferred_loss;} \
  static inline sz_t badapt_problem_sine_random_s_get_in_size( const badapt_problem_sine_random_s* o ){return o->input_size;} \
  static inline sz_t badapt_problem_sine_random_s_get_out_size( const badapt_problem_sine_random_s* o ){return 1;} \
  void badapt_problem_sine_random_s_fetch_sample_tio( badapt_problem_sine_random_s* o, bmath_vf3_s* in, bmath_vf3_s* out ); \
  void badapt_problem_sine_random_s_fetch_sample_vio( badapt_problem_sine_random_s* o, bmath_vf3_s* in, bmath_vf3_s* out );
#define TYPEOF_badapt_problem_binary_add_s 2458357191
#define BETH_EXPAND_ITEM_badapt_problem_binary_add_s \
  BCORE_DECLARE_OBJECT( badapt_problem_binary_add_s ) \
    {aware_t _;sz_t bits;u3_t rval;f3_t val_h;f3_t val_l;badapt_loss* preferred_loss;}; \
  static inline const badapt_loss* badapt_problem_binary_add_s_preferred_loss( const badapt_problem_binary_add_s* o ){return o->preferred_loss;} \
  static inline sz_t badapt_problem_binary_add_s_get_in_size( const badapt_problem_binary_add_s* o ){return o->bits * 2;} \
  static inline sz_t badapt_problem_binary_add_s_get_out_size( const badapt_problem_binary_add_s* o ){return o->bits + 1;} \
  void badapt_problem_binary_add_s_fetch_sample_tio( badapt_problem_binary_add_s* o, bmath_vf3_s* in, bmath_vf3_s* out ); \
  void badapt_problem_binary_add_s_fetch_sample_vio( badapt_problem_binary_add_s* o, bmath_vf3_s* in, bmath_vf3_s* out );
#define TYPEOF_badapt_problem_binary_mul_s 2588868138
#define BETH_EXPAND_ITEM_badapt_problem_binary_mul_s \
  BCORE_DECLARE_OBJECT( badapt_problem_binary_mul_s ) \
    {aware_t _;sz_t bits;u3_t rval;f3_t val_h;f3_t val_l;badapt_loss* preferred_loss;}; \
  static inline const badapt_loss* badapt_problem_binary_mul_s_preferred_loss( const badapt_problem_binary_mul_s* o ){return o->preferred_loss;} \
  static inline sz_t badapt_problem_binary_mul_s_get_in_size( const badapt_problem_binary_mul_s* o ){return o->bits * 2;} \
  static inline sz_t badapt_problem_binary_mul_s_get_out_size( const badapt_problem_binary_mul_s* o ){return o->bits * 2;} \
  void badapt_problem_binary_mul_s_fetch_sample_tio( badapt_problem_binary_mul_s* o, bmath_vf3_s* in, bmath_vf3_s* out ); \
  void badapt_problem_binary_mul_s_fetch_sample_vio( badapt_problem_binary_mul_s* o, bmath_vf3_s* in, bmath_vf3_s* out );
#define TYPEOF_badapt_problem_binary_xsg3_s 1139719047
#define BETH_EXPAND_ITEM_badapt_problem_binary_xsg3_s \
  BCORE_DECLARE_OBJECT( badapt_problem_binary_xsg3_s ) \
    {aware_t _;sz_t bits;u3_t rval;f3_t val_h;f3_t val_l;badapt_loss* preferred_loss;}; \
  static inline const badapt_loss* badapt_problem_binary_xsg3_s_preferred_loss( const badapt_problem_binary_xsg3_s* o ){return o->preferred_loss;} \
  static inline sz_t badapt_problem_binary_xsg3_s_get_in_size( const badapt_problem_binary_xsg3_s* o ){return o->bits;} \
  static inline sz_t badapt_problem_binary_xsg3_s_get_out_size( const badapt_problem_binary_xsg3_s* o ){return o->bits;} \
  void badapt_problem_binary_xsg3_s_fetch_sample_tio( badapt_problem_binary_xsg3_s* o, bmath_vf3_s* in, bmath_vf3_s* out ); \
  void badapt_problem_binary_xsg3_s_fetch_sample_vio( badapt_problem_binary_xsg3_s* o, bmath_vf3_s* in, bmath_vf3_s* out );
#define TYPEOF_badapt_problem_binary_hash_s 2595152420
#define BETH_EXPAND_ITEM_badapt_problem_binary_hash_s \
  BCORE_DECLARE_OBJECT( badapt_problem_binary_hash_s ) \
    {aware_t _;sz_t bits;u3_t rval;f3_t val_h;f3_t val_l;bl_t reverse;badapt_loss* preferred_loss;}; \
  static inline const badapt_loss* badapt_problem_binary_hash_s_preferred_loss( const badapt_problem_binary_hash_s* o ){return o->preferred_loss;} \
  static inline sz_t badapt_problem_binary_hash_s_get_in_size( const badapt_problem_binary_hash_s* o ){return o->bits;} \
  static inline sz_t badapt_problem_binary_hash_s_get_out_size( const badapt_problem_binary_hash_s* o ){return o->bits;} \
  void badapt_problem_binary_hash_s_fetch_sample_tio( badapt_problem_binary_hash_s* o, bmath_vf3_s* in, bmath_vf3_s* out ); \
  void badapt_problem_binary_hash_s_fetch_sample_vio( badapt_problem_binary_hash_s* o, bmath_vf3_s* in, bmath_vf3_s* out );
#define TYPEOF_badapt_problem_polynom_s 4086382194
#define BETH_EXPAND_ITEM_badapt_problem_polynom_s \
  BCORE_DECLARE_OBJECT( badapt_problem_polynom_s ) \
    {aware_t _;sz_t input_size;sz_t output_size;f3_t range;u3_t rval;f3_t noise_level;badapt_loss* preferred_loss;}; \
  static inline const badapt_loss* badapt_problem_polynom_s_preferred_loss( const badapt_problem_polynom_s* o ){return o->preferred_loss;} \
  static inline sz_t badapt_problem_polynom_s_get_in_size( const badapt_problem_polynom_s* o ){return o->input_size;} \
  static inline sz_t badapt_problem_polynom_s_get_out_size( const badapt_problem_polynom_s* o ){return o->output_size;} \
  void badapt_problem_polynom_s_fetch_sample_tio( badapt_problem_polynom_s* o, bmath_vf3_s* in, bmath_vf3_s* out ); \
  void badapt_problem_polynom_s_fetch_sample_vio( badapt_problem_polynom_s* o, bmath_vf3_s* in, bmath_vf3_s* out );
#define BETH_EXPAND_GROUP_badapt_problem \
  BCORE_FORWARD_OBJECT( badapt_problem ); \
  BCORE_FORWARD_OBJECT( badapt_problem_sine_random_s ); \
  BCORE_FORWARD_OBJECT( badapt_problem_binary_add_s ); \
  BCORE_FORWARD_OBJECT( badapt_problem_binary_mul_s ); \
  BCORE_FORWARD_OBJECT( badapt_problem_binary_xsg3_s ); \
  BCORE_FORWARD_OBJECT( badapt_problem_binary_hash_s ); \
  BCORE_FORWARD_OBJECT( badapt_problem_polynom_s ); \
  BETH_EXPAND_ITEM_badapt_problem_sine_random_s \
  BETH_EXPAND_ITEM_badapt_problem_binary_add_s \
  BETH_EXPAND_ITEM_badapt_problem_binary_mul_s \
  BETH_EXPAND_ITEM_badapt_problem_binary_xsg3_s \
  BETH_EXPAND_ITEM_badapt_problem_binary_hash_s \
  BETH_EXPAND_ITEM_badapt_problem_polynom_s

/**********************************************************************************************************************/
// source: badapt_activator.h

//----------------------------------------------------------------------------------------------------------------------
// group: badapt_activation

#define TYPEOF_badapt_activation 1242735104
#define TYPEOF_badapt_activation_s 314872026
#define TYPEOF_badapt_activation_zero_s 1163169501
#define BETH_EXPAND_ITEM_badapt_activation_zero_s \
  BCORE_DECLARE_OBJECT( badapt_activation_zero_s ) \
    {aware_t _;}; \
  static inline f3_t badapt_activation_zero_s_fx( const badapt_activation_zero_s* o, f3_t x ){return 0.0;} \
  static inline f3_t badapt_activation_zero_s_dy( const badapt_activation_zero_s* o, f3_t y ){return 0.0;}
#define TYPEOF_badapt_activation_one_s 3799420289
#define BETH_EXPAND_ITEM_badapt_activation_one_s \
  BCORE_DECLARE_OBJECT( badapt_activation_one_s ) \
    {aware_t _;}; \
  static inline f3_t badapt_activation_one_s_fx( const badapt_activation_one_s* o, f3_t x ){return 1.0;} \
  static inline f3_t badapt_activation_one_s_dy( const badapt_activation_one_s* o, f3_t y ){return 0.0;}
#define TYPEOF_badapt_activation_linear_s 3097414786
#define BETH_EXPAND_ITEM_badapt_activation_linear_s \
  BCORE_DECLARE_OBJECT( badapt_activation_linear_s ) \
    {aware_t _;}; \
  static inline f3_t badapt_activation_linear_s_fx( const badapt_activation_linear_s* o, f3_t x ){return   x;} \
  static inline f3_t badapt_activation_linear_s_dy( const badapt_activation_linear_s* o, f3_t y ){return 1.0;}
#define TYPEOF_badapt_activation_sigm_s 747753319
#define BETH_EXPAND_ITEM_badapt_activation_sigm_s \
  BCORE_DECLARE_OBJECT( badapt_activation_sigm_s ) \
    {aware_t _;}; \
  static inline f3_t badapt_activation_sigm_s_fx( const badapt_activation_sigm_s* o, f3_t x ){return ( x > -700 ) ? ( 1.0 / ( 1.0 + exp( -x ) ) ) : 0;} \
  static inline f3_t badapt_activation_sigm_s_dy( const badapt_activation_sigm_s* o, f3_t y ){return y * ( 1.0 - y );}
#define TYPEOF_badapt_activation_sigm_hard_s 710496809
#define BETH_EXPAND_ITEM_badapt_activation_sigm_hard_s \
  BCORE_DECLARE_OBJECT( badapt_activation_sigm_hard_s ) \
    {aware_t _;}; \
  static inline f3_t badapt_activation_sigm_hard_s_fx( const badapt_activation_sigm_hard_s* o, f3_t x ){return ( x < -2.0 ) ? 0.0 : ( x > 2.0 ) ? 1.0 : 0.25 * ( x + 2.0 );} \
  static inline f3_t badapt_activation_sigm_hard_s_dy( const badapt_activation_sigm_hard_s* o, f3_t y ){return ( y <  0.0 ) ? 0.0 : ( y > 1.0 ) ? 0.0 : 0.25;}
#define TYPEOF_badapt_activation_sigm_leaky_s 2268012026
#define BETH_EXPAND_ITEM_badapt_activation_sigm_leaky_s \
  BCORE_DECLARE_OBJECT( badapt_activation_sigm_leaky_s ) \
    {aware_t _;}; \
  static inline f3_t badapt_activation_sigm_leaky_s_fx( const badapt_activation_sigm_leaky_s* o, f3_t x ){return ( x < -2.0 ) ? 0.01 * ( x + 2.0 ) : ( x > 2.0 ) ? 1.0 + 0.01 * ( x - 2.0 ) : 0.25 * ( x + 2.0 );} \
  static inline f3_t badapt_activation_sigm_leaky_s_dy( const badapt_activation_sigm_leaky_s* o, f3_t y ){return ( y <  0.0 ) ? 0.01 : ( y > 1.0 ) ? 0.01 : 0.25;}
#define TYPEOF_badapt_activation_tanh_s 2100774626
#define BETH_EXPAND_ITEM_badapt_activation_tanh_s \
  BCORE_DECLARE_OBJECT( badapt_activation_tanh_s ) \
    {aware_t _;}; \
  static inline f3_t badapt_activation_tanh_s_fx( const badapt_activation_tanh_s* o, f3_t x ){return ( x < 350 ) ? ( 1.0 - ( 2.0 / ( exp( 2.0 * x ) + 1.0 ) ) ) : 1.0;} \
  static inline f3_t badapt_activation_tanh_s_dy( const badapt_activation_tanh_s* o, f3_t y ){return 1.0 - f3_sqr( y );}
#define TYPEOF_badapt_activation_tanh_hard_s 873661470
#define BETH_EXPAND_ITEM_badapt_activation_tanh_hard_s \
  BCORE_DECLARE_OBJECT( badapt_activation_tanh_hard_s ) \
    {aware_t _;}; \
  static inline f3_t badapt_activation_tanh_hard_s_fx( const badapt_activation_tanh_hard_s* o, f3_t x ){return ( x < -1.0 ) ? -1.0 : ( x > 1.0 ) ? 1.0 : x;} \
  static inline f3_t badapt_activation_tanh_hard_s_dy( const badapt_activation_tanh_hard_s* o, f3_t y ){return ( y < -1.0 ) ?  0.0 : ( y > 1.0 ) ? 0.0 : 1.0;}
#define TYPEOF_badapt_activation_tanh_leaky_s 2702577623
#define BETH_EXPAND_ITEM_badapt_activation_tanh_leaky_s \
  BCORE_DECLARE_OBJECT( badapt_activation_tanh_leaky_s ) \
    {aware_t _;}; \
  static inline f3_t badapt_activation_tanh_leaky_s_fx( const badapt_activation_tanh_leaky_s* o, f3_t x ){return ( x < -1.0 ) ? -1.0 + 0.01 * ( x + 1.0 ) : ( x > 1.0 ) ? 1.0 + 0.01 * ( x - 1.0 ) : x;} \
  static inline f3_t badapt_activation_tanh_leaky_s_dy( const badapt_activation_tanh_leaky_s* o, f3_t y ){return ( y < -1.0 ) ?  0.01 : ( y > 1.0 ) ? 0.01 : 1.0;}
#define TYPEOF_badapt_activation_softplus_s 3728332191
#define BETH_EXPAND_ITEM_badapt_activation_softplus_s \
  BCORE_DECLARE_OBJECT( badapt_activation_softplus_s ) \
    {aware_t _;}; \
  static inline f3_t badapt_activation_softplus_s_fx( const badapt_activation_softplus_s* o, f3_t x ){return ( x < 700 ) ? log( 1.0 + exp( x ) ) : x;} \
  static inline f3_t badapt_activation_softplus_s_dy( const badapt_activation_softplus_s* o, f3_t y ){f3_t u = exp( y ); return ( u - 1.0 ) / u;}
#define TYPEOF_badapt_activation_relu_s 708121501
#define BETH_EXPAND_ITEM_badapt_activation_relu_s \
  BCORE_DECLARE_OBJECT( badapt_activation_relu_s ) \
    {aware_t _;}; \
  static inline f3_t badapt_activation_relu_s_fx( const badapt_activation_relu_s* o, f3_t x ){return x > 0 ? x : 0;} \
  static inline f3_t badapt_activation_relu_s_dy( const badapt_activation_relu_s* o, f3_t y ){return y > 0 ? 1 : 0;}
#define TYPEOF_badapt_activation_leaky_relu_s 841402970
#define BETH_EXPAND_ITEM_badapt_activation_leaky_relu_s \
  BCORE_DECLARE_OBJECT( badapt_activation_leaky_relu_s ) \
    {aware_t _;}; \
  static inline f3_t badapt_activation_leaky_relu_s_fx( const badapt_activation_leaky_relu_s* o, f3_t x ){return x > 0 ? x : x * 0.01;} \
  static inline f3_t badapt_activation_leaky_relu_s_dy( const badapt_activation_leaky_relu_s* o, f3_t y ){return y > 0 ? 1 : 0.01;}
#define BETH_EXPAND_GROUP_badapt_activation \
  BCORE_FORWARD_OBJECT( badapt_activation ); \
  BCORE_FORWARD_OBJECT( badapt_activation_zero_s ); \
  BCORE_FORWARD_OBJECT( badapt_activation_one_s ); \
  BCORE_FORWARD_OBJECT( badapt_activation_linear_s ); \
  BCORE_FORWARD_OBJECT( badapt_activation_sigm_s ); \
  BCORE_FORWARD_OBJECT( badapt_activation_sigm_hard_s ); \
  BCORE_FORWARD_OBJECT( badapt_activation_sigm_leaky_s ); \
  BCORE_FORWARD_OBJECT( badapt_activation_tanh_s ); \
  BCORE_FORWARD_OBJECT( badapt_activation_tanh_hard_s ); \
  BCORE_FORWARD_OBJECT( badapt_activation_tanh_leaky_s ); \
  BCORE_FORWARD_OBJECT( badapt_activation_softplus_s ); \
  BCORE_FORWARD_OBJECT( badapt_activation_relu_s ); \
  BCORE_FORWARD_OBJECT( badapt_activation_leaky_relu_s ); \
  typedef f3_t (*badapt_activation_fx)( const badapt_activation* o, f3_t x ); \
  typedef f3_t (*badapt_activation_dy)( const badapt_activation* o, f3_t y ); \
  BCORE_DECLARE_SPECT( badapt_activation ) \
  { \
      bcore_spect_header_s header; \
      badapt_activation_fx fx; \
      badapt_activation_dy dy; \
  }; \
  static inline badapt_activation* badapt_activation_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_badapt_activation, t ); return ( badapt_activation* )bcore_inst_t_create( t ); } \
  static inline bl_t badapt_activation_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_badapt_activation ); } \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( badapt_activation ) \
  static inline bl_t badapt_activation_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_badapt_activation ); } \
  static inline f3_t badapt_activation_p_fx( const badapt_activation_s* __p, const badapt_activation* o, f3_t x ) { assert( __p->fx ); return __p->fx( o, x ); } \
  static inline f3_t badapt_activation_a_fx( const badapt_activation* o, f3_t x ) { const badapt_activation_s* p = badapt_activation_s_get_aware( o ); assert( p->fx ); return p->fx( o, x ); } \
  static inline bl_t badapt_activation_p_defines_fx( const badapt_activation_s* __p ) { return true; } \
  static inline bl_t badapt_activation_a_defines_fx( const badapt_activation* o ) { return true; } \
  static inline f3_t badapt_activation_p_dy( const badapt_activation_s* __p, const badapt_activation* o, f3_t y ) { assert( __p->dy ); return __p->dy( o, y ); } \
  static inline f3_t badapt_activation_a_dy( const badapt_activation* o, f3_t y ) { const badapt_activation_s* p = badapt_activation_s_get_aware( o ); assert( p->dy ); return p->dy( o, y ); } \
  static inline bl_t badapt_activation_p_defines_dy( const badapt_activation_s* __p ) { return true; } \
  static inline bl_t badapt_activation_a_defines_dy( const badapt_activation* o ) { return true; } \
  BETH_EXPAND_ITEM_badapt_activation_zero_s \
  BETH_EXPAND_ITEM_badapt_activation_one_s \
  BETH_EXPAND_ITEM_badapt_activation_linear_s \
  BETH_EXPAND_ITEM_badapt_activation_sigm_s \
  BETH_EXPAND_ITEM_badapt_activation_sigm_hard_s \
  BETH_EXPAND_ITEM_badapt_activation_sigm_leaky_s \
  BETH_EXPAND_ITEM_badapt_activation_tanh_s \
  BETH_EXPAND_ITEM_badapt_activation_tanh_hard_s \
  BETH_EXPAND_ITEM_badapt_activation_tanh_leaky_s \
  BETH_EXPAND_ITEM_badapt_activation_softplus_s \
  BETH_EXPAND_ITEM_badapt_activation_relu_s \
  BETH_EXPAND_ITEM_badapt_activation_leaky_relu_s

//----------------------------------------------------------------------------------------------------------------------
// group: badapt_activator

#define TYPEOF_badapt_activator 1765962851
#define TYPEOF_badapt_activator_s 991795461
#define TYPEOF_badapt_activator_plain_s 3369734202
#define BETH_EXPAND_ITEM_badapt_activator_plain_s \
  BCORE_DECLARE_OBJECT( badapt_activator_plain_s ) \
    {aware_t _;badapt_activation* activation;}; \
  void badapt_activator_plain_s_infer( const badapt_activator_plain_s* o, const bmath_vf3_s* in, bmath_vf3_s* out ); \
  void badapt_activator_plain_s_bgrad( const badapt_activator_plain_s* o, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out, const bmath_vf3_s* out ); \
  static inline void badapt_activator_plain_s_set_activation( badapt_activator_plain_s* o, const badapt_activation* activation ){badapt_activation_a_replicate( &o->activation, activation );} \
  static inline const badapt_activation* badapt_activator_plain_s_get_activation( const badapt_activator_plain_s* o ){return o->activation;}
#define TYPEOF_badapt_activator_softmax_s 3206266268
#define BETH_EXPAND_ITEM_badapt_activator_softmax_s \
  BCORE_DECLARE_OBJECT( badapt_activator_softmax_s ) \
    {aware_t _;}; \
  void badapt_activator_softmax_s_infer( const badapt_activator_softmax_s* o, const bmath_vf3_s* in, bmath_vf3_s* out ); \
  void badapt_activator_softmax_s_bgrad( const badapt_activator_softmax_s* o, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out, const bmath_vf3_s* out );
#define TYPEOF_badapt_layer_activator_s 4048593039
#define BETH_EXPAND_ITEM_badapt_layer_activator_s \
  BCORE_DECLARE_OBJECT( badapt_layer_activator_s ) \
    {aware_t _;sz_t layer;badapt_activator* activator;};
#define TYPEOF_badapt_arr_layer_activator_s 1342407707
#define BETH_EXPAND_ITEM_badapt_arr_layer_activator_s \
  BCORE_DECLARE_OBJECT( badapt_arr_layer_activator_s ) \
    {aware_t _;BCORE_ARRAY_DYN_SOLID_STATIC_S( badapt_layer_activator_s, arr_ );}; \
  static inline badapt_arr_layer_activator_s* badapt_arr_layer_activator_s_set_space( badapt_arr_layer_activator_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_badapt_arr_layer_activator_s, ( bcore_array* )o, size ); return o; } \
  static inline badapt_arr_layer_activator_s* badapt_arr_layer_activator_s_set_size( badapt_arr_layer_activator_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_badapt_arr_layer_activator_s, ( bcore_array* )o, size ); return o; } \
  static inline badapt_arr_layer_activator_s* badapt_arr_layer_activator_s_clear( badapt_arr_layer_activator_s* o ) { bcore_array_t_set_space( TYPEOF_badapt_arr_layer_activator_s, ( bcore_array* )o, 0 ); return o; } \
  static inline badapt_layer_activator_s* badapt_arr_layer_activator_s_push_c( badapt_arr_layer_activator_s* o, const badapt_layer_activator_s* v ) { bcore_array_t_push( TYPEOF_badapt_arr_layer_activator_s, ( bcore_array* )o, sr_twc( TYPEOF_badapt_layer_activator_s, v ) ); return &o->arr_data[ o->arr_size - 1 ]; } \
  static inline badapt_layer_activator_s* badapt_arr_layer_activator_s_push_d( badapt_arr_layer_activator_s* o,       badapt_layer_activator_s* v ) { bcore_array_t_push( TYPEOF_badapt_arr_layer_activator_s, ( bcore_array* )o, sr_tsd( TYPEOF_badapt_layer_activator_s, v ) ); return &o->arr_data[ o->arr_size - 1 ]; } \
  static inline badapt_layer_activator_s* badapt_arr_layer_activator_s_push( badapt_arr_layer_activator_s* o ) \
  { \
      bcore_array_t_push( TYPEOF_badapt_arr_layer_activator_s, ( bcore_array* )o, sr_null() ); \
      return &o->arr_data[ o->arr_size - 1 ]; \
  }
#define TYPEOF_badapt_arr_activator_s 3921137337
#define BETH_EXPAND_ITEM_badapt_arr_activator_s \
  BCORE_DECLARE_OBJECT( badapt_arr_activator_s ) \
    {aware_t _;BCORE_ARRAY_DYN_LINK_STATIC_S( badapt_activator, arr_ );}; \
  static inline badapt_arr_activator_s* badapt_arr_activator_s_set_space( badapt_arr_activator_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_badapt_arr_activator_s, ( bcore_array* )o, size ); return o; } \
  static inline badapt_arr_activator_s* badapt_arr_activator_s_set_size( badapt_arr_activator_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_badapt_arr_activator_s, ( bcore_array* )o, size ); return o; } \
  static inline badapt_arr_activator_s* badapt_arr_activator_s_clear( badapt_arr_activator_s* o ) { bcore_array_t_set_space( TYPEOF_badapt_arr_activator_s, ( bcore_array* )o, 0 ); return o; } \
  static inline badapt_activator* badapt_arr_activator_s_push_c( badapt_arr_activator_s* o, const badapt_activator* v ) { bcore_array_t_push( TYPEOF_badapt_arr_activator_s, ( bcore_array* )o, sr_awc( v ) ); return o->arr_data[ o->arr_size - 1 ]; } \
  static inline badapt_activator* badapt_arr_activator_s_push_d( badapt_arr_activator_s* o,       badapt_activator* v ) { bcore_array_t_push( TYPEOF_badapt_arr_activator_s, ( bcore_array* )o, sr_asd( v ) ); return o->arr_data[ o->arr_size - 1 ]; } \
  static inline badapt_activator* badapt_arr_activator_s_push_t( badapt_arr_activator_s* o, tp_t t ) \
  { \
      bcore_trait_assert_satisfied_type( TYPEOF_badapt_activator, t ); \
      bcore_array_t_push( TYPEOF_badapt_arr_activator_s, ( bcore_array* )o, sr_t_create( t ) ); \
      return o->arr_data[ o->arr_size - 1 ]; \
  }
#define BETH_EXPAND_GROUP_badapt_activator \
  BCORE_FORWARD_OBJECT( badapt_activator ); \
  BCORE_FORWARD_OBJECT( badapt_activator_plain_s ); \
  BCORE_FORWARD_OBJECT( badapt_activator_softmax_s ); \
  BCORE_FORWARD_OBJECT( badapt_layer_activator_s ); \
  BCORE_FORWARD_OBJECT( badapt_arr_layer_activator_s ); \
  BCORE_FORWARD_OBJECT( badapt_arr_activator_s ); \
  typedef const badapt_activation* (*badapt_activator_get_activation)( const badapt_activator* o ); \
  typedef void (*badapt_activator_set_activation)( badapt_activator* o, const badapt_activation* activation ); \
  typedef void (*badapt_activator_infer)( const badapt_activator* o, const bmath_vf3_s* in, bmath_vf3_s* out ); \
  typedef void (*badapt_activator_bgrad)( const badapt_activator* o, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out, const bmath_vf3_s* out ); \
  BCORE_DECLARE_SPECT( badapt_activator ) \
  { \
      bcore_spect_header_s header; \
      badapt_activator_get_activation get_activation; \
      badapt_activator_set_activation set_activation; \
      badapt_activator_infer infer; \
      badapt_activator_bgrad bgrad; \
  }; \
  static inline badapt_activator* badapt_activator_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_badapt_activator, t ); return ( badapt_activator* )bcore_inst_t_create( t ); } \
  static inline bl_t badapt_activator_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_badapt_activator ); } \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( badapt_activator ) \
  static inline bl_t badapt_activator_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_badapt_activator ); } \
  static inline const badapt_activation* badapt_activator_a_get_activation( const badapt_activator* o ) { const badapt_activator_s* p = badapt_activator_s_get_aware( o ); assert( p->get_activation ); return p->get_activation( o ); } \
  static inline bl_t badapt_activator_a_defines_get_activation( const badapt_activator* o ) { return true; } \
  static inline const badapt_activation* badapt_activator_get_activation__( const badapt_activator* o ){return NULL;} \
  static inline void badapt_activator_a_set_activation( badapt_activator* o, const badapt_activation* activation ) { const badapt_activator_s* p = badapt_activator_s_get_aware( o ); assert( p->set_activation ); p->set_activation( o, activation ); } \
  static inline bl_t badapt_activator_a_defines_set_activation( const badapt_activator* o ) { return true; } \
  static inline void badapt_activator_set_activation__( badapt_activator* o, const badapt_activation* activation ){} \
  static inline void badapt_activator_a_infer( const badapt_activator* o, const bmath_vf3_s* in, bmath_vf3_s* out ) { const badapt_activator_s* p = badapt_activator_s_get_aware( o ); assert( p->infer ); p->infer( o, in, out ); } \
  static inline bl_t badapt_activator_a_defines_infer( const badapt_activator* o ) { return true; } \
  static inline void badapt_activator_a_bgrad( const badapt_activator* o, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out, const bmath_vf3_s* out ) { const badapt_activator_s* p = badapt_activator_s_get_aware( o ); assert( p->bgrad ); p->bgrad( o, grad_in, grad_out, out ); } \
  static inline bl_t badapt_activator_a_defines_bgrad( const badapt_activator* o ) { return true; } \
  BETH_EXPAND_ITEM_badapt_activator_plain_s \
  BETH_EXPAND_ITEM_badapt_activator_softmax_s \
  BETH_EXPAND_ITEM_badapt_layer_activator_s \
  BETH_EXPAND_ITEM_badapt_arr_layer_activator_s \
  BETH_EXPAND_ITEM_badapt_arr_activator_s

/**********************************************************************************************************************/
// source: badapt_loss.h

//----------------------------------------------------------------------------------------------------------------------
// group: badapt_loss

#define TYPEOF_badapt_loss 2315142799
#define TYPEOF_badapt_loss_s 3349774265
#define TYPEOF_badapt_loss_l2_s 537646796
#define BETH_EXPAND_ITEM_badapt_loss_l2_s \
  BCORE_DECLARE_OBJECT( badapt_loss_l2_s ) \
    {aware_t _;}; \
  static inline f3_t badapt_loss_l2_s_loss( const badapt_loss_l2_s* o, const bmath_vf3_s* out, const bmath_vf3_s* target ){return bmath_vf3_s_f3_sub_sqr( target, out );} \
  static inline f3_t badapt_loss_l2_s_loss_f3( const badapt_loss_l2_s* o, f3_t out, f3_t target ){return f3_sqr( target - out );} \
  static inline void badapt_loss_l2_s_bgrad( const badapt_loss_l2_s* o, const bmath_vf3_s* out, const bmath_vf3_s* target, bmath_vf3_s* grad ){bmath_vf3_s_sub( target, out, grad );}
#define TYPEOF_badapt_loss_log_s 193592684
#define BETH_EXPAND_ITEM_badapt_loss_log_s \
  BCORE_DECLARE_OBJECT( badapt_loss_log_s ) \
    {aware_t _;}; \
  f3_t badapt_loss_log_s_loss( const badapt_loss_log_s* o, const bmath_vf3_s* out, const bmath_vf3_s* target ); \
  f3_t badapt_loss_log_s_loss_f3( const badapt_loss_log_s* o, f3_t out, f3_t target ); \
  void badapt_loss_log_s_bgrad( const badapt_loss_log_s* o, const bmath_vf3_s* out, const bmath_vf3_s* target, bmath_vf3_s* grad );
#define BETH_EXPAND_GROUP_badapt_loss \
  BCORE_FORWARD_OBJECT( badapt_loss ); \
  BCORE_FORWARD_OBJECT( badapt_loss_l2_s ); \
  BCORE_FORWARD_OBJECT( badapt_loss_log_s ); \
  typedef f3_t (*badapt_loss_loss)( const badapt_loss* o, const bmath_vf3_s* out, const bmath_vf3_s* target ); \
  typedef f3_t (*badapt_loss_loss_f3)( const badapt_loss* o, f3_t out, f3_t target ); \
  typedef void (*badapt_loss_bgrad)( const badapt_loss* o, const bmath_vf3_s* out, const bmath_vf3_s* target, bmath_vf3_s* grad ); \
  BCORE_DECLARE_SPECT( badapt_loss ) \
  { \
      bcore_spect_header_s header; \
      badapt_loss_loss loss; \
      badapt_loss_loss_f3 loss_f3; \
      badapt_loss_bgrad bgrad; \
  }; \
  static inline badapt_loss* badapt_loss_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_badapt_loss, t ); return ( badapt_loss* )bcore_inst_t_create( t ); } \
  static inline bl_t badapt_loss_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_badapt_loss ); } \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( badapt_loss ) \
  static inline bl_t badapt_loss_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_badapt_loss ); } \
  static inline f3_t badapt_loss_p_loss( const badapt_loss_s* __p, const badapt_loss* o, const bmath_vf3_s* out, const bmath_vf3_s* target ) { assert( __p->loss ); return __p->loss( o, out, target ); } \
  static inline f3_t badapt_loss_a_loss( const badapt_loss* o, const bmath_vf3_s* out, const bmath_vf3_s* target ) { const badapt_loss_s* p = badapt_loss_s_get_aware( o ); assert( p->loss ); return p->loss( o, out, target ); } \
  static inline bl_t badapt_loss_p_defines_loss( const badapt_loss_s* __p ) { return true; } \
  static inline bl_t badapt_loss_a_defines_loss( const badapt_loss* o ) { return true; } \
  static inline f3_t badapt_loss_p_loss_f3( const badapt_loss_s* __p, const badapt_loss* o, f3_t out, f3_t target ) { assert( __p->loss_f3 ); return __p->loss_f3( o, out, target ); } \
  static inline f3_t badapt_loss_a_loss_f3( const badapt_loss* o, f3_t out, f3_t target ) { const badapt_loss_s* p = badapt_loss_s_get_aware( o ); assert( p->loss_f3 ); return p->loss_f3( o, out, target ); } \
  static inline bl_t badapt_loss_p_defines_loss_f3( const badapt_loss_s* __p ) { return true; } \
  static inline bl_t badapt_loss_a_defines_loss_f3( const badapt_loss* o ) { return true; } \
  static inline void badapt_loss_p_bgrad( const badapt_loss_s* __p, const badapt_loss* o, const bmath_vf3_s* out, const bmath_vf3_s* target, bmath_vf3_s* grad ) { assert( __p->bgrad ); __p->bgrad( o, out, target, grad ); } \
  static inline void badapt_loss_a_bgrad( const badapt_loss* o, const bmath_vf3_s* out, const bmath_vf3_s* target, bmath_vf3_s* grad ) { const badapt_loss_s* p = badapt_loss_s_get_aware( o ); assert( p->bgrad ); p->bgrad( o, out, target, grad ); } \
  static inline bl_t badapt_loss_p_defines_bgrad( const badapt_loss_s* __p ) { return true; } \
  static inline bl_t badapt_loss_a_defines_bgrad( const badapt_loss* o ) { return true; } \
  BETH_EXPAND_ITEM_badapt_loss_l2_s \
  BETH_EXPAND_ITEM_badapt_loss_log_s

/**********************************************************************************************************************/
// source: badapt_mlp.h

//----------------------------------------------------------------------------------------------------------------------
// group: badapt_mlp

#define TYPEOF_badapt_mlp 498425923
#define TYPEOF_badapt_mlp_s 3664397861
#define TYPEOF_badapt_mlp_layer_s 189394571
#define BETH_EXPAND_ITEM_badapt_mlp_layer_s \
  BCORE_DECLARE_OBJECT( badapt_mlp_layer_s ) \
    {aware_t _;sz_t input_size;sz_t kernels;bmath_mf3_s w;bmath_vf3_s b;badapt_activator* a;bmath_vf3_s o;};
#define TYPEOF_badapt_mlp_arr_layer_s 2923615799
#define BETH_EXPAND_ITEM_badapt_mlp_arr_layer_s \
  BCORE_DECLARE_OBJECT( badapt_mlp_arr_layer_s ) \
    {aware_t _;BCORE_ARRAY_DYN_SOLID_STATIC_S( badapt_mlp_layer_s, arr_ );}; \
  static inline badapt_mlp_arr_layer_s* badapt_mlp_arr_layer_s_set_space( badapt_mlp_arr_layer_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_badapt_mlp_arr_layer_s, ( bcore_array* )o, size ); return o; } \
  static inline badapt_mlp_arr_layer_s* badapt_mlp_arr_layer_s_set_size( badapt_mlp_arr_layer_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_badapt_mlp_arr_layer_s, ( bcore_array* )o, size ); return o; } \
  static inline badapt_mlp_arr_layer_s* badapt_mlp_arr_layer_s_clear( badapt_mlp_arr_layer_s* o ) { bcore_array_t_set_space( TYPEOF_badapt_mlp_arr_layer_s, ( bcore_array* )o, 0 ); return o; } \
  static inline badapt_mlp_layer_s* badapt_mlp_arr_layer_s_push_c( badapt_mlp_arr_layer_s* o, const badapt_mlp_layer_s* v ) { bcore_array_t_push( TYPEOF_badapt_mlp_arr_layer_s, ( bcore_array* )o, sr_twc( TYPEOF_badapt_mlp_layer_s, v ) ); return &o->arr_data[ o->arr_size - 1 ]; } \
  static inline badapt_mlp_layer_s* badapt_mlp_arr_layer_s_push_d( badapt_mlp_arr_layer_s* o,       badapt_mlp_layer_s* v ) { bcore_array_t_push( TYPEOF_badapt_mlp_arr_layer_s, ( bcore_array* )o, sr_tsd( TYPEOF_badapt_mlp_layer_s, v ) ); return &o->arr_data[ o->arr_size - 1 ]; } \
  static inline badapt_mlp_layer_s* badapt_mlp_arr_layer_s_push( badapt_mlp_arr_layer_s* o ) \
  { \
      bcore_array_t_push( TYPEOF_badapt_mlp_arr_layer_s, ( bcore_array* )o, sr_null() ); \
      return &o->arr_data[ o->arr_size - 1 ]; \
  }
#define TYPEOF_badapt_mlp_s 3664397861
#define BETH_EXPAND_ITEM_badapt_mlp_s \
  BCORE_DECLARE_OBJECT( badapt_mlp_s ) \
    {aware_t _;badapt_dynamics_std_s dynamics;badapt_mlp_arr_layer_s arr_layer;sz_t max_buffer_size;bmath_vf3_s in;}; \
  static inline sz_t badapt_mlp_s_get_in_size( const badapt_mlp_s* o ){return o->arr_layer.arr_size > 0 ? o->arr_layer.arr_data[ 0 ].input_size : 0;} \
  static inline sz_t badapt_mlp_s_get_out_size( const badapt_mlp_s* o ){return o->arr_layer.arr_size > 0 ? o->arr_layer.arr_data[ o->arr_layer.arr_size - 1 ].kernels : 0;} \
  static inline void badapt_mlp_s_get_dynamics_std( const badapt_mlp_s* o, badapt_dynamics_std_s* dynamics ){badapt_dynamics_std_s_copy( dynamics, &o->dynamics );} \
  static inline void badapt_mlp_s_set_dynamics_std( badapt_mlp_s* o, const badapt_dynamics_std_s* dynamics ){badapt_dynamics_std_s_copy( &o->dynamics, dynamics );} \
  void badapt_mlp_s_arc_to_sink( const badapt_mlp_s* o, bcore_sink* sink ); \
  void badapt_mlp_s_infer( const badapt_mlp_s* o, const bmath_vf3_s* in, bmath_vf3_s* out ); \
  void badapt_mlp_s_minfer( badapt_mlp_s* o, const bmath_vf3_s* in, bmath_vf3_s* out ); \
  void badapt_mlp_s_bgrad( const badapt_mlp_s* o, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out ); \
  void badapt_mlp_s_bgrad_adapt( badapt_mlp_s* o, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out );
#define TYPEOF_badapt_builder_mlp_funnel_s 3643146818
#define BETH_EXPAND_ITEM_badapt_builder_mlp_funnel_s \
  BCORE_DECLARE_OBJECT( badapt_builder_mlp_funnel_s ) \
    {aware_t _;sz_t input_size;sz_t input_kernels;sz_t output_kernels;sz_t layers;f3_t kernels_rate;u2_t random_seed;badapt_dynamics_std_s dynamics;badapt_arr_layer_activator_s arr_layer_activator;}; \
  static inline sz_t badapt_builder_mlp_funnel_s_get_in_size( const badapt_builder_mlp_funnel_s* o ){return o->input_size;} \
  static inline void badapt_builder_mlp_funnel_s_set_in_size( badapt_builder_mlp_funnel_s* o, sz_t size ){o->input_size = size;} \
  static inline sz_t badapt_builder_mlp_funnel_s_get_out_size( const badapt_builder_mlp_funnel_s* o ){return o->output_kernels;} \
  static inline void badapt_builder_mlp_funnel_s_set_out_size( badapt_builder_mlp_funnel_s* o, sz_t size ){o->output_kernels = size;} \
  badapt_adaptive* badapt_builder_mlp_funnel_s_build( const badapt_builder_mlp_funnel_s* o );
#define BETH_EXPAND_GROUP_badapt_mlp \
  BCORE_FORWARD_OBJECT( badapt_mlp ); \
  BCORE_FORWARD_OBJECT( badapt_mlp_layer_s ); \
  BCORE_FORWARD_OBJECT( badapt_mlp_arr_layer_s ); \
  BCORE_FORWARD_OBJECT( badapt_mlp_s ); \
  BCORE_FORWARD_OBJECT( badapt_builder_mlp_funnel_s ); \
  BETH_EXPAND_ITEM_badapt_mlp_layer_s \
  BETH_EXPAND_ITEM_badapt_mlp_arr_layer_s \
  BETH_EXPAND_ITEM_badapt_mlp_s \
  BETH_EXPAND_ITEM_badapt_builder_mlp_funnel_s

/**********************************************************************************************************************/
// source: badapt_c1d.h

//----------------------------------------------------------------------------------------------------------------------
// group: badapt_c1d

#define TYPEOF_badapt_c1d 4280081428
#define TYPEOF_badapt_c1d_s 2405996614
#define TYPEOF_badapt_c1d_layer_s 169352500
#define BETH_EXPAND_ITEM_badapt_c1d_layer_s \
  BCORE_DECLARE_OBJECT( badapt_c1d_layer_s ) \
    {aware_t _;sz_t input_size;sz_t stride;sz_t steps;sz_t kernel_size;sz_t kernels;bmath_mf3_s w;bmath_vf3_s b;badapt_activator* a;bmath_vf3_s o;};
#define TYPEOF_badapt_c1d_arr_layer_s 2858590876
#define BETH_EXPAND_ITEM_badapt_c1d_arr_layer_s \
  BCORE_DECLARE_OBJECT( badapt_c1d_arr_layer_s ) \
    {aware_t _;BCORE_ARRAY_DYN_SOLID_STATIC_S( badapt_c1d_layer_s, arr_ );}; \
  static inline badapt_c1d_arr_layer_s* badapt_c1d_arr_layer_s_set_space( badapt_c1d_arr_layer_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_badapt_c1d_arr_layer_s, ( bcore_array* )o, size ); return o; } \
  static inline badapt_c1d_arr_layer_s* badapt_c1d_arr_layer_s_set_size( badapt_c1d_arr_layer_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_badapt_c1d_arr_layer_s, ( bcore_array* )o, size ); return o; } \
  static inline badapt_c1d_arr_layer_s* badapt_c1d_arr_layer_s_clear( badapt_c1d_arr_layer_s* o ) { bcore_array_t_set_space( TYPEOF_badapt_c1d_arr_layer_s, ( bcore_array* )o, 0 ); return o; } \
  static inline badapt_c1d_layer_s* badapt_c1d_arr_layer_s_push_c( badapt_c1d_arr_layer_s* o, const badapt_c1d_layer_s* v ) { bcore_array_t_push( TYPEOF_badapt_c1d_arr_layer_s, ( bcore_array* )o, sr_twc( TYPEOF_badapt_c1d_layer_s, v ) ); return &o->arr_data[ o->arr_size - 1 ]; } \
  static inline badapt_c1d_layer_s* badapt_c1d_arr_layer_s_push_d( badapt_c1d_arr_layer_s* o,       badapt_c1d_layer_s* v ) { bcore_array_t_push( TYPEOF_badapt_c1d_arr_layer_s, ( bcore_array* )o, sr_tsd( TYPEOF_badapt_c1d_layer_s, v ) ); return &o->arr_data[ o->arr_size - 1 ]; } \
  static inline badapt_c1d_layer_s* badapt_c1d_arr_layer_s_push( badapt_c1d_arr_layer_s* o ) \
  { \
      bcore_array_t_push( TYPEOF_badapt_c1d_arr_layer_s, ( bcore_array* )o, sr_null() ); \
      return &o->arr_data[ o->arr_size - 1 ]; \
  }
#define TYPEOF_badapt_c1d_s 2405996614
#define BETH_EXPAND_ITEM_badapt_c1d_s \
  BCORE_DECLARE_OBJECT( badapt_c1d_s ) \
    {aware_t _;badapt_dynamics_std_s dynamics;badapt_c1d_arr_layer_s arr_layer;sz_t max_buffer_size;bmath_vf3_s in;}; \
  static inline sz_t badapt_c1d_s_get_in_size( const badapt_c1d_s* o ){return o->arr_layer.arr_size > 0 ? o->arr_layer.arr_data[ 0 ].input_size : 0;} \
  static inline sz_t badapt_c1d_s_get_out_size( const badapt_c1d_s* o ){return o->arr_layer.arr_size > 0 ? o->arr_layer.arr_data[ o->arr_layer.arr_size - 1 ].kernels : 0;} \
  static inline void badapt_c1d_s_get_dynamics_std( const badapt_c1d_s* o, badapt_dynamics_std_s* dynamics ){badapt_dynamics_std_s_copy( dynamics, &o->dynamics );} \
  static inline void badapt_c1d_s_set_dynamics_std( badapt_c1d_s* o, const badapt_dynamics_std_s* dynamics ){badapt_dynamics_std_s_copy( &o->dynamics, dynamics );} \
  void badapt_c1d_s_arc_to_sink( const badapt_c1d_s* o, bcore_sink* sink ); \
  void badapt_c1d_s_infer( const badapt_c1d_s* o, const bmath_vf3_s* in, bmath_vf3_s* out ); \
  void badapt_c1d_s_minfer( badapt_c1d_s* o, const bmath_vf3_s* in, bmath_vf3_s* out ); \
  void badapt_c1d_s_bgrad( const badapt_c1d_s* o, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out ); \
  void badapt_c1d_s_bgrad_adapt( badapt_c1d_s* o, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out );
#define TYPEOF_badapt_builder_c1d_funnel_s 437767695
#define BETH_EXPAND_ITEM_badapt_builder_c1d_funnel_s \
  BCORE_DECLARE_OBJECT( badapt_builder_c1d_funnel_s ) \
    {aware_t _;sz_t input_size;sz_t input_step;sz_t input_convolution_size;sz_t input_kernels;sz_t output_kernels;f3_t kernels_rate;sz_t reduction_step;sz_t convolution_size;u2_t random_seed;badapt_dynamics_std_s dynamics;badapt_arr_layer_activator_s arr_layer_activator;}; \
  static inline sz_t badapt_builder_c1d_funnel_s_get_in_size( const badapt_builder_c1d_funnel_s* o ){return o->input_size;} \
  static inline void badapt_builder_c1d_funnel_s_set_in_size( badapt_builder_c1d_funnel_s* o, sz_t size ){o->input_size = size;} \
  static inline sz_t badapt_builder_c1d_funnel_s_get_out_size( const badapt_builder_c1d_funnel_s* o ){return o->output_kernels;} \
  static inline void badapt_builder_c1d_funnel_s_set_out_size( badapt_builder_c1d_funnel_s* o, sz_t size ){o->output_kernels = size;} \
  badapt_adaptive* badapt_builder_c1d_funnel_s_build( const badapt_builder_c1d_funnel_s* o );
#define BETH_EXPAND_GROUP_badapt_c1d \
  BCORE_FORWARD_OBJECT( badapt_c1d ); \
  BCORE_FORWARD_OBJECT( badapt_c1d_layer_s ); \
  BCORE_FORWARD_OBJECT( badapt_c1d_arr_layer_s ); \
  BCORE_FORWARD_OBJECT( badapt_c1d_s ); \
  BCORE_FORWARD_OBJECT( badapt_builder_c1d_funnel_s ); \
  BETH_EXPAND_ITEM_badapt_c1d_layer_s \
  BETH_EXPAND_ITEM_badapt_c1d_arr_layer_s \
  BETH_EXPAND_ITEM_badapt_c1d_s \
  BETH_EXPAND_ITEM_badapt_builder_c1d_funnel_s

/**********************************************************************************************************************/
// source: badapt_ern.h

//----------------------------------------------------------------------------------------------------------------------
// group: badapt_ern

#define TYPEOF_badapt_ern 3572010651
#define TYPEOF_badapt_ern_s 3488918381
#define TYPEOF_badapt_ern_layer_s 879149859
#define BETH_EXPAND_ITEM_badapt_ern_layer_s \
  BCORE_DECLARE_OBJECT( badapt_ern_layer_s ) \
    {bmath_vf3_s v_x;bmath_vf3_s v_c;bmath_vf3_s v_h;};
#define TYPEOF_badapt_ern_arr_layer_s 3542192735
#define BETH_EXPAND_ITEM_badapt_ern_arr_layer_s \
  BCORE_DECLARE_OBJECT( badapt_ern_arr_layer_s ) \
    {aware_t _;BCORE_ARRAY_DYN_LINK_STATIC_S( badapt_ern_layer_s, arr_ );}; \
  static inline badapt_ern_arr_layer_s* badapt_ern_arr_layer_s_set_space( badapt_ern_arr_layer_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_badapt_ern_arr_layer_s, ( bcore_array* )o, size ); return o; } \
  static inline badapt_ern_arr_layer_s* badapt_ern_arr_layer_s_set_size( badapt_ern_arr_layer_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_badapt_ern_arr_layer_s, ( bcore_array* )o, size ); return o; } \
  static inline badapt_ern_arr_layer_s* badapt_ern_arr_layer_s_clear( badapt_ern_arr_layer_s* o ) { bcore_array_t_set_space( TYPEOF_badapt_ern_arr_layer_s, ( bcore_array* )o, 0 ); return o; } \
  static inline badapt_ern_layer_s* badapt_ern_arr_layer_s_push_c( badapt_ern_arr_layer_s* o, const badapt_ern_layer_s* v ) { bcore_array_t_push( TYPEOF_badapt_ern_arr_layer_s, ( bcore_array* )o, sr_twc( TYPEOF_badapt_ern_layer_s, v ) ); return o->arr_data[ o->arr_size - 1 ]; } \
  static inline badapt_ern_layer_s* badapt_ern_arr_layer_s_push_d( badapt_ern_arr_layer_s* o,       badapt_ern_layer_s* v ) { bcore_array_t_push( TYPEOF_badapt_ern_arr_layer_s, ( bcore_array* )o, sr_tsd( TYPEOF_badapt_ern_layer_s, v ) ); return o->arr_data[ o->arr_size - 1 ]; } \
  static inline badapt_ern_layer_s* badapt_ern_arr_layer_s_push( badapt_ern_arr_layer_s* o ) \
  { \
      bcore_array_t_push( TYPEOF_badapt_ern_arr_layer_s, ( bcore_array* )o, sr_t_create( TYPEOF_badapt_ern_layer_s ) ); \
      return o->arr_data[ o->arr_size - 1 ]; \
  }
#define TYPEOF_badapt_ern_s 3488918381
#define BETH_EXPAND_ITEM_badapt_ern_s \
  BCORE_DECLARE_OBJECT( badapt_ern_s ) \
    {aware_t _;sz_t size_input;sz_t size_hidden;sz_t size_output;sz_t size_unfolded;badapt_dynamics_std_s dynamics;bmath_mf3_s w_hx;bmath_mf3_s w_hc;bmath_mf3_s w_oh;bmath_vf3_s b_h;bmath_vf3_s b_o;badapt_activator* a_h;badapt_activator* a_o;bmath_vf3_s v_o;bmath_vf3_s d_v_o;bmath_vf3_s d_v_c;bmath_vf3_s d_v_h;bmath_mf3_s d_w_hx;bmath_mf3_s d_w_hc;bmath_vf3_s d_b_h;badapt_ern_arr_layer_s arr_layer;}; \
  static inline sz_t badapt_ern_s_get_in_size( const badapt_ern_s* o ){return o->size_input;} \
  static inline sz_t badapt_ern_s_get_out_size( const badapt_ern_s* o ){return o->size_output;} \
  static inline void badapt_ern_s_get_dynamics_std( const badapt_ern_s* o, badapt_dynamics_std_s* dynamics ){badapt_dynamics_std_s_copy( dynamics, &o->dynamics );} \
  static inline void badapt_ern_s_set_dynamics_std( badapt_ern_s* o, const badapt_dynamics_std_s* dynamics ){badapt_dynamics_std_s_copy( &o->dynamics, dynamics );} \
  void badapt_ern_s_arc_to_sink( const badapt_ern_s* o, bcore_sink* sink ); \
  void badapt_ern_s_minfer( badapt_ern_s* o, const bmath_vf3_s* in, bmath_vf3_s* out ); \
  void badapt_ern_s_bgrad_adapt( badapt_ern_s* o, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out ); \
  void badapt_ern_s_get_weights_min_max( const badapt_ern_s* o, f3_t* min, f3_t* max );
#define TYPEOF_badapt_ern_builder_s 178034157
#define BETH_EXPAND_ITEM_badapt_ern_builder_s \
  BCORE_DECLARE_OBJECT( badapt_ern_builder_s ) \
    {aware_t _;sz_t size_input;sz_t size_hidden;sz_t size_output;sz_t size_unfolded;badapt_dynamics_std_s dynamics;u2_t random_seed;badapt_activator* a_h;badapt_activator* a_o;}; \
  static inline sz_t badapt_ern_builder_s_get_in_size( const badapt_ern_builder_s* o ){return o->size_input;} \
  static inline void badapt_ern_builder_s_set_in_size( badapt_ern_builder_s* o, sz_t size ){o->size_input = size;} \
  static inline sz_t badapt_ern_builder_s_get_out_size( const badapt_ern_builder_s* o ){return o->size_output;} \
  static inline void badapt_ern_builder_s_set_out_size( badapt_ern_builder_s* o, sz_t size ){o->size_output = size;} \
  badapt_adaptive* badapt_ern_builder_s_build( const badapt_ern_builder_s* o );
#define BETH_EXPAND_GROUP_badapt_ern \
  BCORE_FORWARD_OBJECT( badapt_ern ); \
  BCORE_FORWARD_OBJECT( badapt_ern_layer_s ); \
  BCORE_FORWARD_OBJECT( badapt_ern_arr_layer_s ); \
  BCORE_FORWARD_OBJECT( badapt_ern_s ); \
  BCORE_FORWARD_OBJECT( badapt_ern_builder_s ); \
  BETH_EXPAND_ITEM_badapt_ern_layer_s \
  BETH_EXPAND_ITEM_badapt_ern_arr_layer_s \
  BETH_EXPAND_ITEM_badapt_ern_s \
  BETH_EXPAND_ITEM_badapt_ern_builder_s

/**********************************************************************************************************************/
// source: badapt_lstm.h

//----------------------------------------------------------------------------------------------------------------------
// group: badapt_lstm

#define TYPEOF_badapt_lstm 3143739258
#define TYPEOF_badapt_lstm_s 1040139428
#define TYPEOF_badapt_lstm_layer_s 3112546698
#define BETH_EXPAND_ITEM_badapt_lstm_layer_s \
  BCORE_DECLARE_OBJECT( badapt_lstm_layer_s ) \
    {bmath_vf3_s v_x;bmath_vf3_s v_f;bmath_vf3_s v_i;bmath_vf3_s v_o;bmath_vf3_s v_q;bmath_vf3_s v_c;bmath_vf3_s v_d;bmath_vf3_s v_h;};
#define TYPEOF_badapt_lstm_arr_layer_s 99245790
#define BETH_EXPAND_ITEM_badapt_lstm_arr_layer_s \
  BCORE_DECLARE_OBJECT( badapt_lstm_arr_layer_s ) \
    {aware_t _;BCORE_ARRAY_DYN_LINK_STATIC_S( badapt_lstm_layer_s, arr_ );}; \
  static inline badapt_lstm_arr_layer_s* badapt_lstm_arr_layer_s_set_space( badapt_lstm_arr_layer_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_badapt_lstm_arr_layer_s, ( bcore_array* )o, size ); return o; } \
  static inline badapt_lstm_arr_layer_s* badapt_lstm_arr_layer_s_set_size( badapt_lstm_arr_layer_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_badapt_lstm_arr_layer_s, ( bcore_array* )o, size ); return o; } \
  static inline badapt_lstm_arr_layer_s* badapt_lstm_arr_layer_s_clear( badapt_lstm_arr_layer_s* o ) { bcore_array_t_set_space( TYPEOF_badapt_lstm_arr_layer_s, ( bcore_array* )o, 0 ); return o; } \
  static inline badapt_lstm_layer_s* badapt_lstm_arr_layer_s_push_c( badapt_lstm_arr_layer_s* o, const badapt_lstm_layer_s* v ) { bcore_array_t_push( TYPEOF_badapt_lstm_arr_layer_s, ( bcore_array* )o, sr_twc( TYPEOF_badapt_lstm_layer_s, v ) ); return o->arr_data[ o->arr_size - 1 ]; } \
  static inline badapt_lstm_layer_s* badapt_lstm_arr_layer_s_push_d( badapt_lstm_arr_layer_s* o,       badapt_lstm_layer_s* v ) { bcore_array_t_push( TYPEOF_badapt_lstm_arr_layer_s, ( bcore_array* )o, sr_tsd( TYPEOF_badapt_lstm_layer_s, v ) ); return o->arr_data[ o->arr_size - 1 ]; } \
  static inline badapt_lstm_layer_s* badapt_lstm_arr_layer_s_push( badapt_lstm_arr_layer_s* o ) \
  { \
      bcore_array_t_push( TYPEOF_badapt_lstm_arr_layer_s, ( bcore_array* )o, sr_t_create( TYPEOF_badapt_lstm_layer_s ) ); \
      return o->arr_data[ o->arr_size - 1 ]; \
  }
#define TYPEOF_badapt_lstm_s 1040139428
#define BETH_EXPAND_ITEM_badapt_lstm_s \
  BCORE_DECLARE_OBJECT( badapt_lstm_s ) \
    {aware_t _;sz_t size_input;sz_t size_hidden;sz_t size_output;sz_t size_unfolded;badapt_dynamics_std_s dynamics;bmath_mf3_s w_fx;bmath_mf3_s w_fh;bmath_mf3_s w_ix;bmath_mf3_s w_ih;bmath_mf3_s w_ox;bmath_mf3_s w_oh;bmath_mf3_s w_qx;bmath_mf3_s w_qh;bmath_mf3_s w_rh;bmath_vf3_s b_f;bmath_vf3_s b_i;bmath_vf3_s b_o;bmath_vf3_s b_q;bmath_vf3_s b_r;badapt_activator* a_f;badapt_activator* a_i;badapt_activator* a_o;badapt_activator* a_q;badapt_activator* a_d;badapt_activator* a_r;bmath_vf3_s v_r;bmath_vf3_s d_v_r;bmath_vf3_s d_v_f;bmath_vf3_s d_v_i;bmath_vf3_s d_v_o;bmath_vf3_s d_v_q;bmath_vf3_s d_v_c;bmath_vf3_s d_v_d;bmath_vf3_s d_v_h;bmath_mf3_s d_w_fx;bmath_mf3_s d_w_fh;bmath_mf3_s d_w_ix;bmath_mf3_s d_w_ih;bmath_mf3_s d_w_ox;bmath_mf3_s d_w_oh;bmath_mf3_s d_w_qx;bmath_mf3_s d_w_qh;bmath_vf3_s d_b_f;bmath_vf3_s d_b_i;bmath_vf3_s d_b_o;bmath_vf3_s d_b_q;badapt_lstm_arr_layer_s arr_layer;}; \
  static inline sz_t badapt_lstm_s_get_in_size( const badapt_lstm_s* o ){return o->size_input;} \
  static inline sz_t badapt_lstm_s_get_out_size( const badapt_lstm_s* o ){return o->size_output;} \
  static inline void badapt_lstm_s_get_dynamics_std( const badapt_lstm_s* o, badapt_dynamics_std_s* dynamics ){badapt_dynamics_std_s_copy( dynamics, &o->dynamics );} \
  static inline void badapt_lstm_s_set_dynamics_std( badapt_lstm_s* o, const badapt_dynamics_std_s* dynamics ){badapt_dynamics_std_s_copy( &o->dynamics, dynamics );} \
  void badapt_lstm_s_arc_to_sink( const badapt_lstm_s* o, bcore_sink* sink ); \
  void badapt_lstm_s_minfer( badapt_lstm_s* o, const bmath_vf3_s* in, bmath_vf3_s* out ); \
  void badapt_lstm_s_bgrad_adapt( badapt_lstm_s* o, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out ); \
  void badapt_lstm_s_reset( badapt_lstm_s* o ); \
  void badapt_lstm_s_get_weights_min_max( const badapt_lstm_s* o, f3_t* min, f3_t* max );
#define TYPEOF_badapt_lstm_builder_s 2648274872
#define BETH_EXPAND_ITEM_badapt_lstm_builder_s \
  BCORE_DECLARE_OBJECT( badapt_lstm_builder_s ) \
    {aware_t _;sz_t size_input;sz_t size_hidden;sz_t size_output;sz_t size_unfolded;badapt_dynamics_std_s dynamics;u2_t random_seed;f3_t random_range;badapt_activator* a_f;badapt_activator* a_i;badapt_activator* a_o;badapt_activator* a_q;badapt_activator* a_d;badapt_activator* a_r;}; \
  void badapt_lstm_builder_s_init_x( badapt_lstm_builder_s* o ); \
  static inline sz_t badapt_lstm_builder_s_get_in_size( const badapt_lstm_builder_s* o ){return o->size_input;} \
  static inline void badapt_lstm_builder_s_set_in_size( badapt_lstm_builder_s* o, sz_t size ){o->size_input = size;} \
  static inline sz_t badapt_lstm_builder_s_get_out_size( const badapt_lstm_builder_s* o ){return o->size_output;} \
  static inline void badapt_lstm_builder_s_set_out_size( badapt_lstm_builder_s* o, sz_t size ){o->size_output = size;} \
  badapt_adaptive* badapt_lstm_builder_s_build( const badapt_lstm_builder_s* o );
#define BETH_EXPAND_GROUP_badapt_lstm \
  BCORE_FORWARD_OBJECT( badapt_lstm ); \
  BCORE_FORWARD_OBJECT( badapt_lstm_layer_s ); \
  BCORE_FORWARD_OBJECT( badapt_lstm_arr_layer_s ); \
  BCORE_FORWARD_OBJECT( badapt_lstm_s ); \
  BCORE_FORWARD_OBJECT( badapt_lstm_builder_s ); \
  BETH_EXPAND_ITEM_badapt_lstm_layer_s \
  BETH_EXPAND_ITEM_badapt_lstm_arr_layer_s \
  BETH_EXPAND_ITEM_badapt_lstm_s \
  BETH_EXPAND_ITEM_badapt_lstm_builder_s

/**********************************************************************************************************************/
// source: badapt_training.h

//----------------------------------------------------------------------------------------------------------------------
// group: badapt_supplier

#define TYPEOF_badapt_supplier 1387458544
#define TYPEOF_badapt_supplier_s 2616919562
#define BETH_EXPAND_GROUP_badapt_supplier \
  BCORE_FORWARD_OBJECT( badapt_supplier ); \
  typedef sz_t (*badapt_supplier_get_in_size)( const badapt_supplier* o ); \
  typedef sz_t (*badapt_supplier_get_out_size)( const badapt_supplier* o ); \
  typedef void (*badapt_supplier_fetch_sample_tin)( badapt_supplier* o, bmath_vf3_s* in ); \
  typedef void (*badapt_supplier_fetch_sample_tio)( badapt_supplier* o, bmath_vf3_s* in, bmath_vf3_s* out ); \
  typedef void (*badapt_supplier_fetch_sample_vin)( badapt_supplier* o, bmath_vf3_s* in ); \
  typedef void (*badapt_supplier_fetch_sample_vio)( badapt_supplier* o, bmath_vf3_s* in, bmath_vf3_s* out ); \
  typedef const badapt_loss* (*badapt_supplier_preferred_loss)( const badapt_supplier* o ); \
  typedef void (*badapt_supplier_setup_builder)( const badapt_supplier* o, badapt_builder* builder ); \
  BCORE_DECLARE_SPECT( badapt_supplier ) \
  { \
      bcore_spect_header_s header; \
      badapt_supplier_get_in_size get_in_size; \
      badapt_supplier_get_out_size get_out_size; \
      badapt_supplier_fetch_sample_tin fetch_sample_tin; \
      badapt_supplier_fetch_sample_tio fetch_sample_tio; \
      badapt_supplier_fetch_sample_vin fetch_sample_vin; \
      badapt_supplier_fetch_sample_vio fetch_sample_vio; \
      badapt_supplier_preferred_loss preferred_loss; \
      badapt_supplier_setup_builder setup_builder; \
  }; \
  static inline badapt_supplier* badapt_supplier_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_badapt_supplier, t ); return ( badapt_supplier* )bcore_inst_t_create( t ); } \
  static inline bl_t badapt_supplier_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_badapt_supplier ); } \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( badapt_supplier ) \
  static inline bl_t badapt_supplier_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_badapt_supplier ); } \
  static inline sz_t badapt_supplier_a_get_in_size( const badapt_supplier* o ) { const badapt_supplier_s* p = badapt_supplier_s_get_aware( o ); assert( p->get_in_size ); return p->get_in_size( o ); } \
  static inline bl_t badapt_supplier_a_defines_get_in_size( const badapt_supplier* o ) { return true; } \
  static inline sz_t badapt_supplier_a_get_out_size( const badapt_supplier* o ) { const badapt_supplier_s* p = badapt_supplier_s_get_aware( o ); assert( p->get_out_size ); return p->get_out_size( o ); } \
  static inline bl_t badapt_supplier_a_defines_get_out_size( const badapt_supplier* o ) { return true; } \
  static inline void badapt_supplier_a_fetch_sample_tin( badapt_supplier* o, bmath_vf3_s* in ) { const badapt_supplier_s* p = badapt_supplier_s_get_aware( o ); assert( p->fetch_sample_tin ); p->fetch_sample_tin( o, in ); } \
  static inline bl_t badapt_supplier_a_defines_fetch_sample_tin( const badapt_supplier* o ) { return badapt_supplier_s_get_aware( o )->fetch_sample_tin != NULL; } \
  static inline void badapt_supplier_a_fetch_sample_tio( badapt_supplier* o, bmath_vf3_s* in, bmath_vf3_s* out ) { const badapt_supplier_s* p = badapt_supplier_s_get_aware( o ); assert( p->fetch_sample_tio ); p->fetch_sample_tio( o, in, out ); } \
  static inline bl_t badapt_supplier_a_defines_fetch_sample_tio( const badapt_supplier* o ) { return badapt_supplier_s_get_aware( o )->fetch_sample_tio != NULL; } \
  static inline void badapt_supplier_a_fetch_sample_vin( badapt_supplier* o, bmath_vf3_s* in ) { const badapt_supplier_s* p = badapt_supplier_s_get_aware( o ); assert( p->fetch_sample_vin ); p->fetch_sample_vin( o, in ); } \
  static inline bl_t badapt_supplier_a_defines_fetch_sample_vin( const badapt_supplier* o ) { return badapt_supplier_s_get_aware( o )->fetch_sample_vin != NULL; } \
  static inline void badapt_supplier_a_fetch_sample_vio( badapt_supplier* o, bmath_vf3_s* in, bmath_vf3_s* out ) { const badapt_supplier_s* p = badapt_supplier_s_get_aware( o ); assert( p->fetch_sample_vio ); p->fetch_sample_vio( o, in, out ); } \
  static inline bl_t badapt_supplier_a_defines_fetch_sample_vio( const badapt_supplier* o ) { return badapt_supplier_s_get_aware( o )->fetch_sample_vio != NULL; } \
  static inline const badapt_loss* badapt_supplier_a_preferred_loss( const badapt_supplier* o ) { const badapt_supplier_s* p = badapt_supplier_s_get_aware( o ); assert( p->preferred_loss ); return p->preferred_loss( o ); } \
  static inline bl_t badapt_supplier_a_defines_preferred_loss( const badapt_supplier* o ) { return badapt_supplier_s_get_aware( o )->preferred_loss != NULL; } \
  static inline void badapt_supplier_a_setup_builder( const badapt_supplier* o, badapt_builder* builder ) { const badapt_supplier_s* p = badapt_supplier_s_get_aware( o ); assert( p->setup_builder ); p->setup_builder( o, builder ); } \
  static inline bl_t badapt_supplier_a_defines_setup_builder( const badapt_supplier* o ) { return true; } \
  void badapt_supplier_setup_builder_default( const badapt_supplier* o, badapt_builder* builder );

//----------------------------------------------------------------------------------------------------------------------
// group: badapt_guide

#define TYPEOF_badapt_guide 4278624408
#define TYPEOF_badapt_guide_s 2128916482
#define BETH_EXPAND_GROUP_badapt_guide \
  BCORE_FORWARD_OBJECT( badapt_guide ); \
  typedef bl_t (*badapt_guide_callback)( const badapt_guide* o, badapt_training_state* state ); \
  BCORE_DECLARE_SPECT( badapt_guide ) \
  { \
      bcore_spect_header_s header; \
      badapt_guide_callback callback; \
  }; \
  static inline badapt_guide* badapt_guide_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_badapt_guide, t ); return ( badapt_guide* )bcore_inst_t_create( t ); } \
  static inline bl_t badapt_guide_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_badapt_guide ); } \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( badapt_guide ) \
  static inline bl_t badapt_guide_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_badapt_guide ); } \
  static inline bl_t badapt_guide_a_callback( const badapt_guide* o, badapt_training_state* state ) { const badapt_guide_s* p = badapt_guide_s_get_aware( o ); assert( p->callback ); return p->callback( o, state ); } \
  static inline bl_t badapt_guide_a_defines_callback( const badapt_guide* o ) { return true; }

//----------------------------------------------------------------------------------------------------------------------
// group: badapt_training_state

#define TYPEOF_badapt_training_state 2190295860
#define TYPEOF_badapt_training_state_s 3622237734
#define BETH_EXPAND_GROUP_badapt_training_state \
  BCORE_FORWARD_OBJECT( badapt_training_state ); \
  typedef void (*badapt_training_state_set_adaptive)( badapt_training_state* o, const badapt_adaptive* adaptive ); \
  typedef badapt_adaptive* (*badapt_training_state_get_adaptive)( const badapt_training_state* o ); \
  typedef void (*badapt_training_state_set_supplier)( badapt_training_state* o, const badapt_supplier* supplier ); \
  typedef badapt_supplier* (*badapt_training_state_get_supplier)( const badapt_training_state* o ); \
  typedef void (*badapt_training_state_set_progress)( badapt_training_state* o, const badapt_progress_s* progress ); \
  typedef badapt_progress_s* (*badapt_training_state_get_progress)( const badapt_training_state* o ); \
  typedef void (*badapt_training_state_set_guide)( badapt_training_state* o, const badapt_guide* guide ); \
  typedef badapt_guide* (*badapt_training_state_get_guide)( const badapt_training_state* o ); \
  typedef void (*badapt_training_state_set_backup_path)( badapt_training_state* o, sc_t name ); \
  typedef sc_t (*badapt_training_state_get_backup_path)( const badapt_training_state* o ); \
  typedef bl_t (*badapt_training_state_backup)( const badapt_training_state* o ); \
  typedef bl_t (*badapt_training_state_recover)( badapt_training_state* o ); \
  BCORE_DECLARE_SPECT( badapt_training_state ) \
  { \
      bcore_spect_header_s header; \
      badapt_training_state_set_adaptive set_adaptive; \
      badapt_training_state_get_adaptive get_adaptive; \
      badapt_training_state_set_supplier set_supplier; \
      badapt_training_state_get_supplier get_supplier; \
      badapt_training_state_set_progress set_progress; \
      badapt_training_state_get_progress get_progress; \
      badapt_training_state_set_guide set_guide; \
      badapt_training_state_get_guide get_guide; \
      badapt_training_state_set_backup_path set_backup_path; \
      badapt_training_state_get_backup_path get_backup_path; \
      badapt_training_state_backup backup; \
      badapt_training_state_recover recover; \
  }; \
  static inline badapt_training_state* badapt_training_state_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_badapt_training_state, t ); return ( badapt_training_state* )bcore_inst_t_create( t ); } \
  static inline bl_t badapt_training_state_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_badapt_training_state ); } \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( badapt_training_state ) \
  static inline bl_t badapt_training_state_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_badapt_training_state ); } \
  static inline void badapt_training_state_a_set_adaptive( badapt_training_state* o, const badapt_adaptive* adaptive ) { const badapt_training_state_s* p = badapt_training_state_s_get_aware( o ); assert( p->set_adaptive ); p->set_adaptive( o, adaptive ); } \
  static inline bl_t badapt_training_state_a_defines_set_adaptive( const badapt_training_state* o ) { return badapt_training_state_s_get_aware( o )->set_adaptive != NULL; } \
  static inline badapt_adaptive* badapt_training_state_a_get_adaptive( const badapt_training_state* o ) { const badapt_training_state_s* p = badapt_training_state_s_get_aware( o ); assert( p->get_adaptive ); return p->get_adaptive( o ); } \
  static inline bl_t badapt_training_state_a_defines_get_adaptive( const badapt_training_state* o ) { return badapt_training_state_s_get_aware( o )->get_adaptive != NULL; } \
  static inline void badapt_training_state_a_set_supplier( badapt_training_state* o, const badapt_supplier* supplier ) { const badapt_training_state_s* p = badapt_training_state_s_get_aware( o ); assert( p->set_supplier ); p->set_supplier( o, supplier ); } \
  static inline bl_t badapt_training_state_a_defines_set_supplier( const badapt_training_state* o ) { return badapt_training_state_s_get_aware( o )->set_supplier != NULL; } \
  static inline badapt_supplier* badapt_training_state_a_get_supplier( const badapt_training_state* o ) { const badapt_training_state_s* p = badapt_training_state_s_get_aware( o ); assert( p->get_supplier ); return p->get_supplier( o ); } \
  static inline bl_t badapt_training_state_a_defines_get_supplier( const badapt_training_state* o ) { return badapt_training_state_s_get_aware( o )->get_supplier != NULL; } \
  static inline void badapt_training_state_a_set_progress( badapt_training_state* o, const badapt_progress_s* progress ) { const badapt_training_state_s* p = badapt_training_state_s_get_aware( o ); assert( p->set_progress ); p->set_progress( o, progress ); } \
  static inline bl_t badapt_training_state_a_defines_set_progress( const badapt_training_state* o ) { return badapt_training_state_s_get_aware( o )->set_progress != NULL; } \
  static inline badapt_progress_s* badapt_training_state_a_get_progress( const badapt_training_state* o ) { const badapt_training_state_s* p = badapt_training_state_s_get_aware( o ); assert( p->get_progress ); return p->get_progress( o ); } \
  static inline bl_t badapt_training_state_a_defines_get_progress( const badapt_training_state* o ) { return badapt_training_state_s_get_aware( o )->get_progress != NULL; } \
  static inline void badapt_training_state_a_set_guide( badapt_training_state* o, const badapt_guide* guide ) { const badapt_training_state_s* p = badapt_training_state_s_get_aware( o ); assert( p->set_guide ); p->set_guide( o, guide ); } \
  static inline bl_t badapt_training_state_a_defines_set_guide( const badapt_training_state* o ) { return badapt_training_state_s_get_aware( o )->set_guide != NULL; } \
  static inline badapt_guide* badapt_training_state_a_get_guide( const badapt_training_state* o ) { const badapt_training_state_s* p = badapt_training_state_s_get_aware( o ); assert( p->get_guide ); return p->get_guide( o ); } \
  static inline bl_t badapt_training_state_a_defines_get_guide( const badapt_training_state* o ) { return badapt_training_state_s_get_aware( o )->get_guide != NULL; } \
  static inline void badapt_training_state_a_set_backup_path( badapt_training_state* o, sc_t name ) { const badapt_training_state_s* p = badapt_training_state_s_get_aware( o ); assert( p->set_backup_path ); p->set_backup_path( o, name ); } \
  static inline bl_t badapt_training_state_a_defines_set_backup_path( const badapt_training_state* o ) { return badapt_training_state_s_get_aware( o )->set_backup_path != NULL; } \
  static inline sc_t badapt_training_state_a_get_backup_path( const badapt_training_state* o ) { const badapt_training_state_s* p = badapt_training_state_s_get_aware( o ); assert( p->get_backup_path ); return p->get_backup_path( o ); } \
  static inline bl_t badapt_training_state_a_defines_get_backup_path( const badapt_training_state* o ) { return badapt_training_state_s_get_aware( o )->get_backup_path != NULL; } \
  static inline bl_t badapt_training_state_a_backup( const badapt_training_state* o ) { const badapt_training_state_s* p = badapt_training_state_s_get_aware( o ); assert( p->backup ); return p->backup( o ); } \
  static inline bl_t badapt_training_state_a_defines_backup( const badapt_training_state* o ) { return true; } \
  bl_t badapt_training_state_backup__( const badapt_training_state* o ); \
  static inline bl_t badapt_training_state_a_recover( badapt_training_state* o ) { const badapt_training_state_s* p = badapt_training_state_s_get_aware( o ); assert( p->recover ); return p->recover( o ); } \
  static inline bl_t badapt_training_state_a_defines_recover( const badapt_training_state* o ) { return true; } \
  bl_t badapt_training_state_recover__( badapt_training_state* o );

//----------------------------------------------------------------------------------------------------------------------
// group: badapt_trainer

#define TYPEOF_badapt_trainer 1053730437
#define TYPEOF_badapt_trainer_s 385523047
#define BETH_EXPAND_GROUP_badapt_trainer \
  BCORE_FORWARD_OBJECT( badapt_trainer ); \
  typedef void (*badapt_trainer_run)( const badapt_trainer* o, badapt_training_state* state ); \
  typedef badapt_training_state* (*badapt_trainer_create_state)( const badapt_trainer* o ); \
  BCORE_DECLARE_SPECT( badapt_trainer ) \
  { \
      bcore_spect_header_s header; \
      badapt_trainer_run run; \
      badapt_trainer_create_state create_state; \
  }; \
  static inline badapt_trainer* badapt_trainer_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_badapt_trainer, t ); return ( badapt_trainer* )bcore_inst_t_create( t ); } \
  static inline bl_t badapt_trainer_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_badapt_trainer ); } \
  BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( badapt_trainer ) \
  static inline bl_t badapt_trainer_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_badapt_trainer ); } \
  static inline void badapt_trainer_a_run( const badapt_trainer* o, badapt_training_state* state ) { const badapt_trainer_s* p = badapt_trainer_s_get_aware( o ); assert( p->run ); p->run( o, state ); } \
  static inline bl_t badapt_trainer_a_defines_run( const badapt_trainer* o ) { return true; } \
  static inline badapt_training_state* badapt_trainer_a_create_state( const badapt_trainer* o ) { const badapt_trainer_s* p = badapt_trainer_s_get_aware( o ); assert( p->create_state ); return p->create_state( o ); } \
  static inline bl_t badapt_trainer_a_defines_create_state( const badapt_trainer* o ) { return true; }

//----------------------------------------------------------------------------------------------------------------------
// group: badapt_training_objects

#define TYPEOF_badapt_training_objects 446020241
#define TYPEOF_badapt_training_objects_s 990621627
#define TYPEOF_badapt_progress_s 1786273765
#define BETH_EXPAND_ITEM_badapt_progress_s \
  BCORE_DECLARE_OBJECT( badapt_progress_s ) \
    {sz_t iteration;f3_t error;f3_t improved;f3_t bias;};
#define TYPEOF_badapt_training_state_std_s 3157709416
#define BETH_EXPAND_ITEM_badapt_training_state_std_s \
  BCORE_DECLARE_OBJECT( badapt_training_state_std_s ) \
    {aware_t _;badapt_progress_s progress;badapt_adaptive* adaptive;badapt_supplier* supplier;badapt_guide* guide;st_s backup_file_name;}; \
  static inline void badapt_training_state_std_s_set_adaptive( badapt_training_state_std_s* o, const badapt_adaptive* adaptive ){badapt_adaptive_a_replicate( &o->adaptive, adaptive );} \
  static inline badapt_adaptive* badapt_training_state_std_s_get_adaptive( const badapt_training_state_std_s* o ){return o->adaptive;} \
  static inline void badapt_training_state_std_s_set_supplier( badapt_training_state_std_s* o, const badapt_supplier* supplier ){badapt_supplier_a_replicate( &o->supplier, supplier );} \
  static inline badapt_supplier* badapt_training_state_std_s_get_supplier( const badapt_training_state_std_s* o ){return o->supplier;} \
  static inline void badapt_training_state_std_s_set_progress( badapt_training_state_std_s* o, const badapt_progress_s* progress ){badapt_progress_s_copy( &o->progress, progress );} \
  static inline badapt_progress_s* badapt_training_state_std_s_get_progress( const badapt_training_state_std_s* o ){return ( badapt_progress_s* )&o->progress;} \
  static inline void badapt_training_state_std_s_set_guide( badapt_training_state_std_s* o, const badapt_guide* guide ){badapt_guide_a_replicate( &o->guide, guide );} \
  static inline badapt_guide* badapt_training_state_std_s_get_guide( const badapt_training_state_std_s* o ){return ( badapt_guide* )o->guide;} \
  static inline void badapt_training_state_std_s_set_backup_path( badapt_training_state_std_s* o, sc_t name ){st_s_copy_sc( &o->backup_file_name, name );} \
  static inline sc_t badapt_training_state_std_s_get_backup_path( const badapt_training_state_std_s* o ){return o->backup_file_name.sc;}
#define TYPEOF_badapt_guide_std_s 1442673228
#define BETH_EXPAND_ITEM_badapt_guide_std_s \
  BCORE_DECLARE_OBJECT( badapt_guide_std_s ) \
    {aware_t _;f3_t annealing_factor;bcore_sink* log;}; \
  static inline void badapt_guide_std_s_init_x( badapt_guide_std_s* o ){o->log = bcore_fork( BCORE_STDOUT );} \
  bl_t badapt_guide_std_s_callback( const badapt_guide_std_s* o, badapt_training_state* state );
#define BETH_EXPAND_GROUP_badapt_training_objects \
  BCORE_FORWARD_OBJECT( badapt_training_objects ); \
  BCORE_FORWARD_OBJECT( badapt_progress_s ); \
  BCORE_FORWARD_OBJECT( badapt_training_state_std_s ); \
  BCORE_FORWARD_OBJECT( badapt_guide_std_s ); \
  BETH_EXPAND_ITEM_badapt_progress_s \
  BETH_EXPAND_ITEM_badapt_training_state_std_s \
  BETH_EXPAND_ITEM_badapt_guide_std_s

/**********************************************************************************************************************/
// source: badapt_trainer.h

//----------------------------------------------------------------------------------------------------------------------
// group: badapt_trainer_objects

#define TYPEOF_badapt_trainer_objects 2941531770
#define TYPEOF_badapt_trainer_objects_s 44166052
#define TYPEOF_badapt_trainer_batch_s 3745932548
#define BETH_EXPAND_ITEM_badapt_trainer_batch_s \
  BCORE_DECLARE_OBJECT( badapt_trainer_batch_s ) \
    {aware_t _;badapt_loss* loss;sz_t batch_size;sz_t batch_cycles_per_fetch;sz_t fetch_cycles_per_iteration;sz_t valid_size;sz_t max_iterations;}; \
  void badapt_trainer_batch_s_run( const badapt_trainer_batch_s* o, badapt_training_state* state ); \
  badapt_training_state* badapt_trainer_batch_s_create_state( const badapt_trainer_batch_s* o );
#define TYPEOF_badapt_sample_batch_s 3659720691
#define BETH_EXPAND_ITEM_badapt_sample_batch_s \
  BCORE_DECLARE_OBJECT( badapt_sample_batch_s ) \
    {bmath_vf3_s in;bmath_vf3_s out;};
#define TYPEOF_badapt_arr_sample_batch_s 1005014423
#define BETH_EXPAND_ITEM_badapt_arr_sample_batch_s \
  BCORE_DECLARE_OBJECT( badapt_arr_sample_batch_s ) \
    {aware_t _;BCORE_ARRAY_DYN_SOLID_STATIC_S( badapt_sample_batch_s, arr_ );}; \
  static inline badapt_arr_sample_batch_s* badapt_arr_sample_batch_s_set_space( badapt_arr_sample_batch_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_badapt_arr_sample_batch_s, ( bcore_array* )o, size ); return o; } \
  static inline badapt_arr_sample_batch_s* badapt_arr_sample_batch_s_set_size( badapt_arr_sample_batch_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_badapt_arr_sample_batch_s, ( bcore_array* )o, size ); return o; } \
  static inline badapt_arr_sample_batch_s* badapt_arr_sample_batch_s_clear( badapt_arr_sample_batch_s* o ) { bcore_array_t_set_space( TYPEOF_badapt_arr_sample_batch_s, ( bcore_array* )o, 0 ); return o; } \
  static inline badapt_sample_batch_s* badapt_arr_sample_batch_s_push_c( badapt_arr_sample_batch_s* o, const badapt_sample_batch_s* v ) { bcore_array_t_push( TYPEOF_badapt_arr_sample_batch_s, ( bcore_array* )o, sr_twc( TYPEOF_badapt_sample_batch_s, v ) ); return &o->arr_data[ o->arr_size - 1 ]; } \
  static inline badapt_sample_batch_s* badapt_arr_sample_batch_s_push_d( badapt_arr_sample_batch_s* o,       badapt_sample_batch_s* v ) { bcore_array_t_push( TYPEOF_badapt_arr_sample_batch_s, ( bcore_array* )o, sr_tsd( TYPEOF_badapt_sample_batch_s, v ) ); return &o->arr_data[ o->arr_size - 1 ]; } \
  static inline badapt_sample_batch_s* badapt_arr_sample_batch_s_push( badapt_arr_sample_batch_s* o ) \
  { \
      bcore_array_t_push( TYPEOF_badapt_arr_sample_batch_s, ( bcore_array* )o, sr_null() ); \
      return &o->arr_data[ o->arr_size - 1 ]; \
  }
#define TYPEOF_badapt_trainer_main_s 3593175425
#define BETH_EXPAND_ITEM_badapt_trainer_main_s \
  BCORE_DECLARE_OBJECT( badapt_trainer_main_s ) \
    {aware_t _;badapt_supplier* problem;badapt_builder* builder;badapt_guide* guide;badapt_trainer* trainer;}; \
  er_t badapt_trainer_main_s_main( badapt_trainer_main_s* o, bcore_main_frame_s* frame );
#define BETH_EXPAND_GROUP_badapt_trainer_objects \
  BCORE_FORWARD_OBJECT( badapt_trainer_objects ); \
  BCORE_FORWARD_OBJECT( badapt_trainer_batch_s ); \
  BCORE_FORWARD_OBJECT( badapt_sample_batch_s ); \
  BCORE_FORWARD_OBJECT( badapt_arr_sample_batch_s ); \
  BCORE_FORWARD_OBJECT( badapt_trainer_main_s ); \
  BETH_EXPAND_ITEM_badapt_trainer_batch_s \
  BETH_EXPAND_ITEM_badapt_sample_batch_s \
  BETH_EXPAND_ITEM_badapt_arr_sample_batch_s \
  BETH_EXPAND_ITEM_badapt_trainer_main_s

/**********************************************************************************************************************/

vd_t badapt_planted_signal_handler( const bcore_signal_s* o );

#endif // BADAPT_PLANTED_H
// BETH_PLANT_SIGNATURE 4058675543
