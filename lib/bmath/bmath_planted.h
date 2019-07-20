/** This file was generated by the plant-compiler. Not suitable for manual editing.
 *
 *  Copyright and License:
 *    If a specification exists for the repository or folder in which the file is located,
 *    that specification shall apply. Otherwise, the Copyright and License of the associated
 *    source code files containing beth plant-code shall apply.
 */

#ifndef BMATH_PLANTED_H
#define BMATH_PLANTED_H

#include "bcore_control.h"


/**********************************************************************************************************************/
// source: bmath_mf3_sx

//----------------------------------------------------------------------------------------------------------------------
// group: bmath_mf3_sx

#define TYPEOF_bmath_mf3_sx 934545414
#define TYPEOF_bmath_mf3_sx_s 948195256
#define TYPEOF_bmath_mf3_sx_s 948195256
#define BETH_EXPAND_ITEM_bmath_mf3_sx_s \
  BCORE_DECLARE_OBJECT( bmath_mf3_sx_s ) \
    {aware_t _;sz_t slos;sz_t xons;sz_t rows;sz_t i_stride;BCORE_ARRAY_DYN_SOLID_STATIC_S( sz_t, i_ );BCORE_ARRAY_DYN_SOLID_STATIC_S( f3_t, v_ );};
#define BETH_EXPAND_GROUP_bmath_mf3_sx \
  BCORE_FORWARD_OBJECT( bmath_mf3_sx ); \
  BCORE_FORWARD_OBJECT( bmath_mf3_sx_s ); \
  BETH_EXPAND_ITEM_bmath_mf3_sx_s

/**********************************************************************************************************************/
// source: bmath_mf3_sf

//----------------------------------------------------------------------------------------------------------------------
// group: bmath_mf3_sf

#define TYPEOF_bmath_mf3_sf 1370763508
#define TYPEOF_bmath_mf3_sf_s 913280742
#define TYPEOF_bmath_mf3_sf_s 913280742
#define BETH_EXPAND_ITEM_bmath_mf3_sf_s \
  BCORE_DECLARE_OBJECT( bmath_mf3_sf_s ) \
    {aware_t _;sz_t cols;sz_t rows;sz_t i_stride;BCORE_ARRAY_DYN_SOLID_STATIC_S( sz_t, i_ );BCORE_ARRAY_DYN_SOLID_STATIC_S( f3_t, v_ );};
#define BETH_EXPAND_GROUP_bmath_mf3_sf \
  BCORE_FORWARD_OBJECT( bmath_mf3_sf ); \
  BCORE_FORWARD_OBJECT( bmath_mf3_sf_s ); \
  BETH_EXPAND_ITEM_bmath_mf3_sf_s

/**********************************************************************************************************************/
// source: bmath_hf3

//----------------------------------------------------------------------------------------------------------------------
// group: bmath_hf3

#define TYPEOF_bmath_hf3 3901573499
#define TYPEOF_bmath_hf3_s 3558242317
#define TYPEOF_bmath_hf3_s 3558242317
#define BETH_EXPAND_ITEM_bmath_hf3_s \
  BCORE_DECLARE_OBJECT( bmath_hf3_s ) \
    {aware_t _;BCORE_ARRAY_DYN_SOLID_STATIC_S( sz_t, d_ );BCORE_ARRAY_DYN_SOLID_STATIC_S( f3_t, v_ );};
#define BETH_EXPAND_GROUP_bmath_hf3 \
  BCORE_FORWARD_OBJECT( bmath_hf3 ); \
  BCORE_FORWARD_OBJECT( bmath_hf3_s ); \
  BETH_EXPAND_ITEM_bmath_hf3_s

/**********************************************************************************************************************/
// source: bmath_hf3_vm

//----------------------------------------------------------------------------------------------------------------------
// group: bmath_hf3_vm

#define TYPEOF_bmath_hf3_vm 1129494905
#define TYPEOF_bmath_hf3_vm_s 4000694131
#define TYPEOF_bmath_hf3_vm_holor_s 3470165822
#define BETH_EXPAND_ITEM_bmath_hf3_vm_holor_s \
  BCORE_DECLARE_OBJECT( bmath_hf3_vm_holor_s ) \
    {aware_t _;tp_t name;tp_t type;bmath_hf3_s hf3;};
#define TYPEOF_bmath_hf3_vm_arr_holor_s 3798245038
#define BETH_EXPAND_ITEM_bmath_hf3_vm_arr_holor_s \
  BCORE_DECLARE_OBJECT( bmath_hf3_vm_arr_holor_s ) \
    {aware_t _;BCORE_ARRAY_DYN_SOLID_STATIC_S( bmath_hf3_vm_holor_s, );}; \
  static inline void bmath_hf3_vm_arr_holor_s_set_space( bmath_hf3_vm_arr_holor_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_bmath_hf3_vm_arr_holor_s, ( bcore_array* )o, size ); } \
  static inline void bmath_hf3_vm_arr_holor_s_set_size( bmath_hf3_vm_arr_holor_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_bmath_hf3_vm_arr_holor_s, ( bcore_array* )o, size ); } \
  static inline void bmath_hf3_vm_arr_holor_s_clear( bmath_hf3_vm_arr_holor_s* o ) { bcore_array_t_set_space( TYPEOF_bmath_hf3_vm_arr_holor_s, ( bcore_array* )o, 0 ); } \
  static inline void bmath_hf3_vm_arr_holor_s_push_c( bmath_hf3_vm_arr_holor_s* o, const bmath_hf3_vm_holor_s* v ) { bcore_array_t_push( TYPEOF_bmath_hf3_vm_arr_holor_s, ( bcore_array* )o, sr_twc( TYPEOF_bmath_hf3_vm_holor_s, v ) ); } \
  static inline void bmath_hf3_vm_arr_holor_s_push_d( bmath_hf3_vm_arr_holor_s* o,       bmath_hf3_vm_holor_s* v ) { bcore_array_t_push( TYPEOF_bmath_hf3_vm_arr_holor_s, ( bcore_array* )o, sr_tsd( TYPEOF_bmath_hf3_vm_holor_s, v ) ); } \
  static inline bmath_hf3_vm_holor_s* bmath_hf3_vm_arr_holor_s_push( bmath_hf3_vm_arr_holor_s* o ) \
  { \
      bcore_array_t_push( TYPEOF_bmath_hf3_vm_arr_holor_s, ( bcore_array* )o, sr_null() ); \
      return bcore_array_t_get_last( TYPEOF_bmath_hf3_vm_arr_holor_s, ( bcore_array* )o ).o; \
  }
#define TYPEOF_bmath_hf3_vm_op_s 1119490359
#define BETH_EXPAND_ITEM_bmath_hf3_vm_op_s \
  BCORE_DECLARE_OBJECT( bmath_hf3_vm_op_s ) \
    {aware_t _;bmath_hf3_vm* op;bmath_hf3_vm_s* p;}; \
  static inline void bmath_hf3_vm_op_s_copy_x( bmath_hf3_vm_op_s* o ){ o->p = (bmath_hf3_vm_s*)bmath_hf3_vm_s_get_aware( o->op ); } \
  static inline void bmath_hf3_vm_op_s_mutated( bmath_hf3_vm_op_s* o ){ bmath_hf3_vm_op_s_copy_x( o ); } \
  static inline void bmath_hf3_vm_op_s_run( const bmath_hf3_vm_op_s* o, bmath_hf3_vm_arr_holor_s* arr_holor ){ bmath_hf3_vm_p_run( o->p, o->op, arr_holor ); }
#define TYPEOF_bmath_hf3_vm_proc_s 2358437536
#define BETH_EXPAND_ITEM_bmath_hf3_vm_proc_s \
  BCORE_DECLARE_OBJECT( bmath_hf3_vm_proc_s ) \
    {aware_t _;tp_t name;BCORE_ARRAY_DYN_SOLID_STATIC_S( bmath_hf3_vm_op_s, );}; \
  static inline void bmath_hf3_vm_proc_s_run( const bmath_hf3_vm_proc_s* o, bmath_hf3_vm_arr_holor_s* arr_holor ){ for( sz_t i = 0; i < o->size; i++ ) bmath_hf3_vm_op_s_run( &o->data[ i ], arr_holor ); } \
  static inline void bmath_hf3_vm_proc_s_set_space( bmath_hf3_vm_proc_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_bmath_hf3_vm_proc_s, ( bcore_array* )o, size ); } \
  static inline void bmath_hf3_vm_proc_s_set_size( bmath_hf3_vm_proc_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_bmath_hf3_vm_proc_s, ( bcore_array* )o, size ); } \
  static inline void bmath_hf3_vm_proc_s_clear( bmath_hf3_vm_proc_s* o ) { bcore_array_t_set_space( TYPEOF_bmath_hf3_vm_proc_s, ( bcore_array* )o, 0 ); } \
  static inline void bmath_hf3_vm_proc_s_push_c( bmath_hf3_vm_proc_s* o, const bmath_hf3_vm_op_s* v ) { bcore_array_t_push( TYPEOF_bmath_hf3_vm_proc_s, ( bcore_array* )o, sr_twc( TYPEOF_bmath_hf3_vm_op_s, v ) ); } \
  static inline void bmath_hf3_vm_proc_s_push_d( bmath_hf3_vm_proc_s* o,       bmath_hf3_vm_op_s* v ) { bcore_array_t_push( TYPEOF_bmath_hf3_vm_proc_s, ( bcore_array* )o, sr_tsd( TYPEOF_bmath_hf3_vm_op_s, v ) ); } \
  static inline bmath_hf3_vm_op_s* bmath_hf3_vm_proc_s_push( bmath_hf3_vm_proc_s* o ) \
  { \
      bcore_array_t_push( TYPEOF_bmath_hf3_vm_proc_s, ( bcore_array* )o, sr_null() ); \
      return bcore_array_t_get_last( TYPEOF_bmath_hf3_vm_proc_s, ( bcore_array* )o ).o; \
  }
#define TYPEOF_bmath_hf3_vm_arr_proc_s 682648400
#define BETH_EXPAND_ITEM_bmath_hf3_vm_arr_proc_s \
  BCORE_DECLARE_OBJECT( bmath_hf3_vm_arr_proc_s ) \
    {aware_t _;BCORE_ARRAY_DYN_SOLID_STATIC_S( bmath_hf3_vm_proc_s, );}; \
  static inline void bmath_hf3_vm_arr_proc_s_set_space( bmath_hf3_vm_arr_proc_s* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_bmath_hf3_vm_arr_proc_s, ( bcore_array* )o, size ); } \
  static inline void bmath_hf3_vm_arr_proc_s_set_size( bmath_hf3_vm_arr_proc_s* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_bmath_hf3_vm_arr_proc_s, ( bcore_array* )o, size ); } \
  static inline void bmath_hf3_vm_arr_proc_s_clear( bmath_hf3_vm_arr_proc_s* o ) { bcore_array_t_set_space( TYPEOF_bmath_hf3_vm_arr_proc_s, ( bcore_array* )o, 0 ); } \
  static inline void bmath_hf3_vm_arr_proc_s_push_c( bmath_hf3_vm_arr_proc_s* o, const bmath_hf3_vm_proc_s* v ) { bcore_array_t_push( TYPEOF_bmath_hf3_vm_arr_proc_s, ( bcore_array* )o, sr_twc( TYPEOF_bmath_hf3_vm_proc_s, v ) ); } \
  static inline void bmath_hf3_vm_arr_proc_s_push_d( bmath_hf3_vm_arr_proc_s* o,       bmath_hf3_vm_proc_s* v ) { bcore_array_t_push( TYPEOF_bmath_hf3_vm_arr_proc_s, ( bcore_array* )o, sr_tsd( TYPEOF_bmath_hf3_vm_proc_s, v ) ); } \
  static inline bmath_hf3_vm_proc_s* bmath_hf3_vm_arr_proc_s_push( bmath_hf3_vm_arr_proc_s* o ) \
  { \
      bcore_array_t_push( TYPEOF_bmath_hf3_vm_arr_proc_s, ( bcore_array* )o, sr_null() ); \
      return bcore_array_t_get_last( TYPEOF_bmath_hf3_vm_arr_proc_s, ( bcore_array* )o ).o; \
  }
#define TYPEOF_bmath_hf3_vm_op_linear_s 2207840001
#define BETH_EXPAND_ITEM_bmath_hf3_vm_op_linear_s \
  BCORE_DECLARE_OBJECT( bmath_hf3_vm_op_linear_s ) \
    {aware_t _;sz_t a;sz_t b;}; \
  static inline void bmath_hf3_vm_op_linear_s_run( const bmath_hf3_vm_op_linear_s* o, bmath_hf3_vm_arr_holor_s* arr_holor ){ bmath_hf3_s_cpy( &arr_holor->data[ o->a ].hf3, &arr_holor->data[ o->b ].hf3 ); } \
  void bmath_hf3_vm_op_linear_s_set_arg( bmath_hf3_vm_op_linear_s* o, char id, sz_t idx );
#define TYPEOF_bmath_hf3_vm_op_tanh_s 3338062853
#define BETH_EXPAND_ITEM_bmath_hf3_vm_op_tanh_s \
  BCORE_DECLARE_OBJECT( bmath_hf3_vm_op_tanh_s ) \
    {aware_t _;sz_t a;sz_t b;}; \
  static inline void bmath_hf3_vm_op_tanh_s_run( const bmath_hf3_vm_op_tanh_s* o, bmath_hf3_vm_arr_holor_s* arr_holor ){ bmath_hf3_s_tanh( &arr_holor->data[ o->a ].hf3, &arr_holor->data[ o->b ].hf3 ); } \
  void bmath_hf3_vm_op_tanh_s_set_arg( bmath_hf3_vm_op_tanh_s* o, char id, sz_t idx );
#define TYPEOF_bmath_hf3_vm_op_add_s 815329967
#define BETH_EXPAND_ITEM_bmath_hf3_vm_op_add_s \
  BCORE_DECLARE_OBJECT( bmath_hf3_vm_op_add_s ) \
    {aware_t _;sz_t a;sz_t b;sz_t c;}; \
  static inline void bmath_hf3_vm_op_add_s_run( const bmath_hf3_vm_op_add_s* o, bmath_hf3_vm_arr_holor_s* arr_holor ){ bmath_hf3_s_add( &arr_holor->data[ o->a ].hf3, &arr_holor->data[ o->b ].hf3, &arr_holor->data[ o->c ].hf3 ); } \
  void bmath_hf3_vm_op_add_s_set_arg( bmath_hf3_vm_op_add_s* o, char id, sz_t idx );
#define TYPEOF_bmath_hf3_vm_op_sub_s 4128506606
#define BETH_EXPAND_ITEM_bmath_hf3_vm_op_sub_s \
  BCORE_DECLARE_OBJECT( bmath_hf3_vm_op_sub_s ) \
    {aware_t _;sz_t a;sz_t b;sz_t c;}; \
  static inline void bmath_hf3_vm_op_sub_s_run( const bmath_hf3_vm_op_sub_s* o, bmath_hf3_vm_arr_holor_s* arr_holor ){ bmath_hf3_s_sub( &arr_holor->data[ o->a ].hf3, &arr_holor->data[ o->b ].hf3, &arr_holor->data[ o->c ].hf3 ); } \
  void bmath_hf3_vm_op_sub_s_set_arg( bmath_hf3_vm_op_sub_s* o, char id, sz_t idx );
#define TYPEOF_bmath_hf3_vm_op_kmul_s 2155733865
#define BETH_EXPAND_ITEM_bmath_hf3_vm_op_kmul_s \
  BCORE_DECLARE_OBJECT( bmath_hf3_vm_op_kmul_s ) \
    {aware_t _;sz_t k;sz_t a;sz_t b;sz_t c;}; \
  static inline void bmath_hf3_vm_op_kmul_s_run( const bmath_hf3_vm_op_kmul_s* o, bmath_hf3_vm_arr_holor_s* arr_holor ){ bmath_hf3_s_kmul( &arr_holor->data[ o->a ].hf3, o->k, &arr_holor->data[ o->b ].hf3, &arr_holor->data[ o->c ].hf3 ); } \
  void bmath_hf3_vm_op_kmul_s_set_arg( bmath_hf3_vm_op_kmul_s* o, char id, sz_t idx );
#define TYPEOF_bmath_hf3_vm_op_hmul_s 2747155358
#define BETH_EXPAND_ITEM_bmath_hf3_vm_op_hmul_s \
  BCORE_DECLARE_OBJECT( bmath_hf3_vm_op_hmul_s ) \
    {aware_t _;sz_t a;sz_t b;sz_t c;}; \
  static inline void bmath_hf3_vm_op_hmul_s_run( const bmath_hf3_vm_op_hmul_s* o, bmath_hf3_vm_arr_holor_s* arr_holor ){ bmath_hf3_s_hmul( &arr_holor->data[ o->a ].hf3, &arr_holor->data[ o->b ].hf3, &arr_holor->data[ o->c ].hf3 ); } \
  void bmath_hf3_vm_op_hmul_s_set_arg( bmath_hf3_vm_op_hmul_s* o, char id, sz_t idx );
#define TYPEOF_bmath_hf3_vm_frame_s 1174748325
#define BETH_EXPAND_ITEM_bmath_hf3_vm_frame_s \
  BCORE_DECLARE_OBJECT( bmath_hf3_vm_frame_s ) \
    {aware_t _;bmath_hf3_vm_arr_holor_s arr_holor;bmath_hf3_vm_arr_proc_s arr_proc;bcore_hmap_tpuz_s map_proc;bcore_hmap_tpuz_s map_holor;bcore_hmap_name_s map_name;};
#define BETH_EXPAND_GROUP_bmath_hf3_vm \
  BCORE_FORWARD_OBJECT( bmath_hf3_vm ); \
  BCORE_FORWARD_OBJECT( bmath_hf3_vm_holor_s ); \
  BCORE_FORWARD_OBJECT( bmath_hf3_vm_arr_holor_s ); \
  BCORE_FORWARD_OBJECT( bmath_hf3_vm_op_s ); \
  BCORE_FORWARD_OBJECT( bmath_hf3_vm_proc_s ); \
  BCORE_FORWARD_OBJECT( bmath_hf3_vm_arr_proc_s ); \
  BCORE_FORWARD_OBJECT( bmath_hf3_vm_op_linear_s ); \
  BCORE_FORWARD_OBJECT( bmath_hf3_vm_op_tanh_s ); \
  BCORE_FORWARD_OBJECT( bmath_hf3_vm_op_add_s ); \
  BCORE_FORWARD_OBJECT( bmath_hf3_vm_op_sub_s ); \
  BCORE_FORWARD_OBJECT( bmath_hf3_vm_op_kmul_s ); \
  BCORE_FORWARD_OBJECT( bmath_hf3_vm_op_hmul_s ); \
  BCORE_FORWARD_OBJECT( bmath_hf3_vm_frame_s ); \
  typedef void (*bmath_hf3_vm_run)( const bmath_hf3_vm* o, bmath_hf3_vm_arr_holor_s* arr_holor ); \
  typedef void (*bmath_hf3_vm_set_arg)( bmath_hf3_vm* o, char id, sz_t idx ); \
  BCORE_DECLARE_SPECT( bmath_hf3_vm ) \
  { \
      bcore_spect_header_s header; \
      bmath_hf3_vm_run run; \
      bmath_hf3_vm_set_arg set_arg; \
  }; \
  static inline bmath_hf3_vm* bmath_hf3_vm_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_bmath_hf3_vm, t ); return ( bmath_hf3_vm* )bcore_inst_t_create( t ); } \
  static inline bl_t bmath_hf3_vm_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_bmath_hf3_vm ); } \
  static inline bmath_hf3_vm* bmath_hf3_vm_a_clone( const bmath_hf3_vm* o ) { return ( bmath_hf3_vm* )bcore_inst_a_clone( ( bcore_inst* )o ); } \
  static inline void bmath_hf3_vm_a_discard( bmath_hf3_vm* o ) { bcore_inst_a_discard( ( bcore_inst* )o ); } \
  static inline void bmath_hf3_vm_a_detach( bmath_hf3_vm** o ) { if( !o ) return; bcore_inst_a_discard( ( bcore_inst* )*o ); *o = NULL; } \
  static inline void bmath_hf3_vm_a_attach( bmath_hf3_vm** o, bmath_hf3_vm* src ) { if( src ) bcore_inst_a_attach( ( bcore_inst** )o, ( bcore_inst* )src ); } \
  static inline void bmath_hf3_vm_a_replicate( bmath_hf3_vm** o, const bmath_hf3_vm* src ) { bcore_inst_a_replicate( ( bcore_inst** )o, ( bcore_inst* )src ); } \
  static inline bl_t bmath_hf3_vm_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_bmath_hf3_vm ); } \
  BETH_EXPAND_ITEM_bmath_hf3_vm_holor_s \
  BETH_EXPAND_ITEM_bmath_hf3_vm_arr_holor_s \
  static inline void bmath_hf3_vm_p_run( const bmath_hf3_vm_s* __p, const bmath_hf3_vm* o, bmath_hf3_vm_arr_holor_s* arr_holor ) { __p->run( o, arr_holor ); } \
  static inline void bmath_hf3_vm_a_run( const bmath_hf3_vm* o, bmath_hf3_vm_arr_holor_s* arr_holor ) { bmath_hf3_vm_s_get_aware( o )->run( o, arr_holor ); } \
  static inline bl_t bmath_hf3_vm_p_defines_run( const bmath_hf3_vm_s* __p ) { return __p->run != NULL; } \
  static inline bl_t bmath_hf3_vm_a_defines_run( const bmath_hf3_vm* o ) { return bmath_hf3_vm_s_get_aware( o )->run != NULL; } \
  BETH_EXPAND_ITEM_bmath_hf3_vm_op_s \
  BETH_EXPAND_ITEM_bmath_hf3_vm_proc_s \
  BETH_EXPAND_ITEM_bmath_hf3_vm_arr_proc_s \
  static inline void bmath_hf3_vm_a_set_arg( bmath_hf3_vm* o, char id, sz_t idx ) { bmath_hf3_vm_s_get_aware( o )->set_arg( o, id, idx ); } \
  static inline bl_t bmath_hf3_vm_a_defines_set_arg( const bmath_hf3_vm* o ) { return true; } \
  static inline void bmath_hf3_vm_set_arg__( bmath_hf3_vm* o, char id, sz_t idx ) { ERR_fa( "Not implemented in #<sc_t>", ifnameof( *(aware_t*)o ) ); } \
  BETH_EXPAND_ITEM_bmath_hf3_vm_op_linear_s \
  BETH_EXPAND_ITEM_bmath_hf3_vm_op_tanh_s \
  BETH_EXPAND_ITEM_bmath_hf3_vm_op_add_s \
  BETH_EXPAND_ITEM_bmath_hf3_vm_op_sub_s \
  BETH_EXPAND_ITEM_bmath_hf3_vm_op_kmul_s \
  BETH_EXPAND_ITEM_bmath_hf3_vm_op_hmul_s \
  BETH_EXPAND_ITEM_bmath_hf3_vm_frame_s

/**********************************************************************************************************************/

vd_t bmath_planted_signal_handler( const bcore_signal_s* o );

#endif // BMATH_PLANTED_H
