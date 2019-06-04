/** This file was generated by the beth-precode-compiler. Not suitable for manual editing.
 *
 *  Copyright and License:
 *    If a specification exists for the repository or folder in which the file is located,
 *    that specification shall apply. Otherwise, the Copyright and License of the associated
 *    source code files containing beth-precode shall apply.
 */

#ifndef BCORE_PRECODED_H
#define BCORE_PRECODED_H

#include "bcore_control.h"


/**********************************************************************************************************************/
// source: bcore_precoder_sample

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_precoder_sample_signatures

#define TYPEOF_bcore_precoder_sample_signatures 753998242
#define BETH_EXPAND_GROUP_bcore_precoder_sample_signatures

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_precoder_sample_features

#define TYPEOF_bcore_precoder_sample_features 1533308178
#define TYPEOF_bcore_precoder_sample_features_s 2917243660
#define TYPEOF_anyglobalname 2209051205
#define BETH_EXPAND_GROUP_bcore_precoder_sample_features \
  BCORE_FORWARD_OBJECT( bcore_precoder_sample_features ); \
  typedef void (*bcore_precoder_sample_features_setup)( bcore_precoder_sample_features* o, sz_t n ); \
  typedef void (*bcore_precoder_sample_features_to_stdout)( const bcore_precoder_sample_features* o ); \
  typedef sz_t (*bcore_precoder_sample_features_get_size)( const bcore_precoder_sample_features* o ); \
  BCORE_DECLARE_SPECT( bcore_precoder_sample_features ) \
  { \
      bcore_spect_header_s header; \
      bcore_precoder_sample_features_setup setup; \
      bcore_precoder_sample_features_to_stdout to_stdout; \
      bcore_precoder_sample_features_get_size get_size; \
  }; \
  static inline bcore_precoder_sample_features* bcore_precoder_sample_features_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_bcore_precoder_sample_features, t ); return ( bcore_precoder_sample_features* )bcore_inst_t_create( t ); } \
  static inline bcore_precoder_sample_features* bcore_precoder_sample_features_a_clone( const bcore_precoder_sample_features* o ) { return ( bcore_precoder_sample_features* )bcore_inst_a_clone( ( bcore_inst* )o ); } \
  static inline void bcore_precoder_sample_features_a_discard( bcore_precoder_sample_features* o ) { bcore_inst_a_discard( ( bcore_inst* )o ); } \
  static inline void bcore_precoder_sample_features_a_replicate( bcore_precoder_sample_features** o, const bcore_precoder_sample_features* src ) { bcore_inst_a_replicate( ( bcore_inst** )o, ( bcore_inst* )src ); } \
  static inline void bcore_precoder_sample_features_p_setup( const bcore_precoder_sample_features_s* __p, bcore_precoder_sample_features* o, sz_t n ) { __p->setup( o, n ); } \
  static inline void bcore_precoder_sample_features_t_setup( tp_t __t, bcore_precoder_sample_features* o, sz_t n ) { bcore_precoder_sample_features_s_get_typed( __t )->setup( o, n ); } \
  static inline void bcore_precoder_sample_features_a_setup( bcore_precoder_sample_features* o, sz_t n ) { bcore_precoder_sample_features_s_get_aware( o )->setup( o, n ); } \
  static inline void bcore_precoder_sample_features_r_setup( const sr_s* o, sz_t n ) { ASSERT( !sr_s_is_const( o ) ); ( (bcore_precoder_sample_features_s*)ch_spect_p( o->p, TYPEOF_bcore_precoder_sample_features_s ) )->setup( o->o, n ); } \
  static inline bl_t bcore_precoder_sample_features_p_defines_setup( const bcore_precoder_sample_features_s* __p ) { return true; } \
  static inline bl_t bcore_precoder_sample_features_t_defines_setup( tp_t __t ) { return true; } \
  static inline bl_t bcore_precoder_sample_features_a_defines_setup( const bcore_precoder_sample_features* o ) { return true; } \
  static inline bl_t bcore_precoder_sample_features_r_defines_setup( const sr_s* o ) { return true; } \
  static inline void bcore_precoder_sample_features_p_to_stdout( const bcore_precoder_sample_features_s* __p, const bcore_precoder_sample_features* o ) { __p->to_stdout( o ); } \
  static inline void bcore_precoder_sample_features_t_to_stdout( tp_t __t, const bcore_precoder_sample_features* o ) { bcore_precoder_sample_features_s_get_typed( __t )->to_stdout( o ); } \
  static inline void bcore_precoder_sample_features_a_to_stdout( const bcore_precoder_sample_features* o ) { bcore_precoder_sample_features_s_get_aware( o )->to_stdout( o ); } \
  static inline void bcore_precoder_sample_features_r_to_stdout( const sr_s* o ) { ( (bcore_precoder_sample_features_s*)ch_spect_p( o->p, TYPEOF_bcore_precoder_sample_features_s ) )->to_stdout( o->o ); } \
  static inline bl_t bcore_precoder_sample_features_p_defines_to_stdout( const bcore_precoder_sample_features_s* __p ) { return true; } \
  static inline bl_t bcore_precoder_sample_features_t_defines_to_stdout( tp_t __t ) { return true; } \
  static inline bl_t bcore_precoder_sample_features_a_defines_to_stdout( const bcore_precoder_sample_features* o ) { return true; } \
  static inline bl_t bcore_precoder_sample_features_r_defines_to_stdout( const sr_s* o ) { return true; } \
  void bcore_precoder_sample_features_to_stdout__( const bcore_precoder_sample_features* o ); \
  static inline sz_t bcore_precoder_sample_features_p_get_size( const bcore_precoder_sample_features_s* __p, const bcore_precoder_sample_features* o ) { return __p->get_size( o ); } \
  static inline sz_t bcore_precoder_sample_features_t_get_size( tp_t __t, const bcore_precoder_sample_features* o ) { return bcore_precoder_sample_features_s_get_typed( __t )->get_size( o ); } \
  static inline sz_t bcore_precoder_sample_features_a_get_size( const bcore_precoder_sample_features* o ) { return bcore_precoder_sample_features_s_get_aware( o )->get_size( o ); } \
  static inline sz_t bcore_precoder_sample_features_r_get_size( const sr_s* o ) { return ( (bcore_precoder_sample_features_s*)ch_spect_p( o->p, TYPEOF_bcore_precoder_sample_features_s ) )->get_size( o->o ); } \
  static inline bl_t bcore_precoder_sample_features_p_defines_get_size( const bcore_precoder_sample_features_s* __p ) { return __p->get_size != NULL; } \
  static inline bl_t bcore_precoder_sample_features_t_defines_get_size( tp_t __t ) { return bcore_precoder_sample_features_s_get_typed( __t )->get_size != NULL; } \
  static inline bl_t bcore_precoder_sample_features_a_defines_get_size( const bcore_precoder_sample_features* o ) { return bcore_precoder_sample_features_s_get_aware( o )->get_size != NULL; } \
  static inline bl_t bcore_precoder_sample_features_r_defines_get_size( const sr_s* o ) { return ( (bcore_precoder_sample_features_s*)ch_spect_p( o->p, TYPEOF_bcore_precoder_sample_features_s ) )->get_size != NULL; }

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_precoder_sample

#define TYPEOF_bcore_precoder_sample 933626240
  #define TYPEOF_bcore_precoder_sample_object_s 1940901916
  #define BETH_EXPAND_ITEM_bcore_precoder_sample_object_s \
    BCORE_DECLARE_OBJECT( bcore_precoder_sample_object_s ) \
      {aware_t _;st_s name1;BCORE_ARRAY_DYN_LINK_STATIC_S( sz_t, arr_ );}; \
    void bcore_precoder_sample_object_s_setup( bcore_precoder_sample_object_s* o, sz_t n ); \
    void bcore_precoder_sample_object_s_to_stdout( const bcore_precoder_sample_object_s* o ); \
    sz_t bcore_precoder_sample_object_s_get_size( const bcore_precoder_sample_object_s* o );
  #define TYPEOF_bcore_precoder_sample_object2_s 2687764618
  #define BETH_EXPAND_ITEM_bcore_precoder_sample_object2_s \
    BCORE_DECLARE_OBJECT( bcore_precoder_sample_object2_s ) \
      {aware_t _;bcore_precoder_sample_object_s object;st_s name2;}; \
    void bcore_precoder_sample_object2_s_setup( bcore_precoder_sample_object2_s* o, sz_t n ); \
    void bcore_precoder_sample_object2_s_to_stdout( const bcore_precoder_sample_object2_s* o );
#define BETH_EXPAND_GROUP_bcore_precoder_sample \
  BETH_EXPAND_ITEM_bcore_precoder_sample_object_s \
  BETH_EXPAND_ITEM_bcore_precoder_sample_object2_s

/**********************************************************************************************************************/
// source: bcore_spect_inst_call

//----------------------------------------------------------------------------------------------------------------------
// group: bcore_inst_call

#define TYPEOF_bcore_inst_call 2782316552
#define TYPEOF_bcore_inst_call_s 1816064754
#define TYPEOF_bcore_inst_call_init_x 622928068
#define TYPEOF_bcore_inst_call_down_e 3705510577
#define TYPEOF_bcore_inst_call_copy_e 1302178876
#define TYPEOF_bcore_inst_call_copy_x 1117625067
#define BETH_EXPAND_GROUP_bcore_inst_call \
  BCORE_FORWARD_OBJECT( bcore_inst_call ); \
  typedef void (*bcore_inst_call_init_x)( bcore_inst_call* o ); \
  typedef void (*bcore_inst_call_down_e)( bcore_inst_call* o ); \
  typedef void (*bcore_inst_call_copy_e)( bcore_inst_call* o ); \
  typedef void (*bcore_inst_call_copy_x)( bcore_inst_call* o ); \
  BCORE_DECLARE_SPECT( bcore_inst_call ) \
  { \
      bcore_spect_header_s header; \
      bcore_inst_call_init_x init_x; \
      bcore_inst_call_down_e down_e; \
      bcore_inst_call_copy_e copy_e; \
      bcore_inst_call_copy_x copy_x; \
  }; \
  static inline bcore_inst_call* bcore_inst_call_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_bcore_inst_call, t ); return ( bcore_inst_call* )bcore_inst_t_create( t ); } \
  static inline void bcore_inst_call_p_init_x( const bcore_inst_call_s* __p, bcore_inst_call* o ) { __p->init_x( o ); } \
  static inline bl_t bcore_inst_call_p_defines_init_x( const bcore_inst_call_s* __p ) { return __p->init_x != NULL; } \
  static inline void bcore_inst_call_p_down_e( const bcore_inst_call_s* __p, bcore_inst_call* o ) { __p->down_e( o ); } \
  static inline bl_t bcore_inst_call_p_defines_down_e( const bcore_inst_call_s* __p ) { return __p->down_e != NULL; } \
  static inline void bcore_inst_call_p_copy_e( const bcore_inst_call_s* __p, bcore_inst_call* o ) { __p->copy_e( o ); } \
  static inline bl_t bcore_inst_call_p_defines_copy_e( const bcore_inst_call_s* __p ) { return __p->copy_e != NULL; } \
  static inline void bcore_inst_call_p_copy_x( const bcore_inst_call_s* __p, bcore_inst_call* o ) { __p->copy_x( o ); } \
  static inline bl_t bcore_inst_call_p_defines_copy_x( const bcore_inst_call_s* __p ) { return __p->copy_x != NULL; }

/**********************************************************************************************************************/

vd_t bcore_precoded_signal_handler( const bcore_signal_s* o );

#endif // BCORE_PRECODED_H
