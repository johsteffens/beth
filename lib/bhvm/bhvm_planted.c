/** This file was generated from beth-plant source code.
 *  Compiling Agent : bcore_plant_compiler (C) 2019 J.B.Steffens
 *  Last File Update: 2019-12-17T16:28:56Z
 *
 *  Copyright and License of this File:
 *
 *  Generated code inherits the copyright and license of the underlying beth-plant source code.
 *  Source code defining this file is distributed across following files:
 *
 *  bhvm_holor.h
 *  bhvm_hop.h
 *  bhvm_mcode.h
 *
 */

#include "bhvm_planted.h"
#include "bcore_spect.h"
#include "bcore_spect_inst.h"
#include "bcore_sr.h"
#include "bcore_const_manager.h"


/**********************************************************************************************************************/
// source: bhvm_holor.h
#include "bhvm_holor.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm

BCORE_DEFINE_OBJECT_INST_P( bhvm_shape_s )
"bcore_array"
"{"
    "sz_t [];"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_value_s )
"bcore_array"
"{"
    "typed [];"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_holor_s )
"aware bcore_inst"
"{"
    "bhvm_shape_s s;"
    "bhvm_value_s v;"
    "func bcore_fp:copy_typed;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_holor_adl_s )
"aware bcore_array"
"{"
    "bhvm_holor_s => [];"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_holor_ads_s )
"aware bcore_array"
"{"
    "bhvm_holor_s [];"
"}";

/**********************************************************************************************************************/
// source: bhvm_hop.h
#include "bhvm_hop.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hop

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hop_ar0

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar0_zro_s )
"aware bhvm_hop_ar0"
"{"
"}";

void bhvm_hop_ar0_zro_s_f( bhvm_holor_s* r )
{
    bhvm_lop_ar0_zro_s_f( BKNIT_FA1( r->v.type ), r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar0_one_s )
"aware bhvm_hop_ar0"
"{"
"}";

void bhvm_hop_ar0_one_s_f( bhvm_holor_s* r )
{
    bhvm_lop_ar0_one_s_f( BKNIT_FA1( r->v.type ), r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar0_nul_dp_s )
"aware bhvm_hop_ar0"
"{"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hop_ar1

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_identity_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_identity_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_identity_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_neg_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_neg_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_neg_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_floor_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_floor_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_floor_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_ceil_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_ceil_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_ceil_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_exp_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_exp_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_exp_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_inv_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_inv_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_inv_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_lgst_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_lgst_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_lgst_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_lgst_hard_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_lgst_hard_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_lgst_hard_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_lgst_leaky_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_lgst_leaky_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_lgst_leaky_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_tanh_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_tanh_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_tanh_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_tanh_hard_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_tanh_hard_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_tanh_hard_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_tanh_leaky_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_tanh_leaky_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_tanh_leaky_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_softplus_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_softplus_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_softplus_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_relu_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_relu_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_relu_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_relu_leaky_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_relu_leaky_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_relu_leaky_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_identity_dp_v_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_identity_dp_v_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_identity_dp_v_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_neg_dp_v_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_neg_dp_v_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_neg_dp_v_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_add_dp_a_v_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_add_dp_a_v_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_add_dp_a_v_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_add_dp_b_v_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_add_dp_b_v_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_add_dp_b_v_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_sub_dp_a_v_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_sub_dp_a_v_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_sub_dp_a_v_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar1_sub_dp_b_v_s )
"aware bhvm_hop_ar1"
"{"
"}";

void bhvm_hop_ar1_sub_dp_b_v_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    bhvm_lop_ar1_sub_dp_b_v_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
}

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hop_ar2

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_add_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_add_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_add_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_sub_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_sub_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_sub_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_div_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_div_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_div_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_equal_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_equal_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_equal_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_larger_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_larger_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_larger_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_smaller_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_smaller_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_smaller_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_larger_equal_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_larger_equal_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_larger_equal_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_smaller_equal_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_smaller_equal_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_smaller_equal_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_logic_and_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_logic_and_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_logic_and_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_logic_or_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_logic_or_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_logic_or_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_div_dp_a_vb_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_div_dp_a_vb_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_div_dp_a_vb_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_exp_dp_vy_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_exp_dp_vy_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_exp_dp_vy_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_inv_dp_vy_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_inv_dp_vy_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_inv_dp_vy_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_lgst_dp_vy_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_lgst_dp_vy_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_lgst_dp_vy_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_lgst_hard_dp_vy_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_lgst_hard_dp_vy_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_lgst_hard_dp_vy_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_lgst_leaky_dp_vy_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_lgst_leaky_dp_vy_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_lgst_leaky_dp_vy_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_tanh_dp_vy_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_tanh_dp_vy_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_tanh_dp_vy_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_tanh_hard_dp_vy_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_tanh_hard_dp_vy_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_tanh_hard_dp_vy_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_tanh_leaky_dp_vy_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_tanh_leaky_dp_vy_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_tanh_leaky_dp_vy_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_softplus_dp_vy_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_softplus_dp_vy_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_softplus_dp_vy_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_relu_dp_vy_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_relu_dp_vy_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_relu_dp_vy_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_relu_leaky_dp_vy_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_relu_leaky_dp_vy_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_relu_leaky_dp_vy_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_vvv_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_mul_vvv_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_mul_vvv_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_vsv_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_mul_vsv_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == 1         );
    bhvm_lop_ar2_mul_vsv_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_svv_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_mul_svv_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == 1         ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_mul_svv_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_vvs_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_mul_vvs_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == b->v.size ); assert( r->v.size == 1         );
    bhvm_lop_ar2_mul_vvs_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_mvv_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_mul_mvv_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->s.size == 2 ); assert( b->v.size == a->s.data[ 0 ] ); assert( r->v.size == a->s.data[ 1 ] );
    bhvm_lop_ar2_mul_mvv_s_f_m( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, a->s.data[ 1 ], a->s.data[ 0 ] );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_vmv_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_mul_vmv_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( b->s.size == 2 ); assert( a->v.size == b->s.data[ 1 ] ); assert( r->v.size == b->s.data[ 0 ] );
    bhvm_lop_ar2_mul_vmv_s_f_m( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, b->s.data[ 1 ], b->s.data[ 0 ] );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_vvm_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_mul_vvm_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( r->s.size == 2 ); assert( a->v.size == r->s.data[ 1 ] ); assert( b->v.size == r->s.data[ 0 ] );
    bhvm_lop_ar2_mul_vvm_s_f_m( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->s.data[ 1 ], r->s.data[ 0 ] );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_tvv_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_mul_tvv_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->s.size == 2 ); assert( b->v.size == a->s.data[ 1 ] ); assert( r->v.size == a->s.data[ 0 ] );
    bhvm_lop_ar2_mul_tvv_s_f_m( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, a->s.data[ 1 ], a->s.data[ 0 ] );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_vtv_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_mul_vtv_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( b->s.size == 2 ); assert( a->v.size == b->s.data[ 0 ] ); assert( r->v.size == b->s.data[ 1 ] );
    bhvm_lop_ar2_mul_vtv_s_f_m( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, b->s.data[ 1 ], b->s.data[ 0 ] );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_mmm_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_mtm_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_tmm_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_ttm_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_acc_vvv_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_mul_acc_vvv_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_mul_acc_vvv_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_acc_vsv_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_mul_acc_vsv_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size ); assert( b->v.size == 1         );
    bhvm_lop_ar2_mul_acc_vsv_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_acc_svv_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_mul_acc_svv_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == 1         ); assert( b->v.size == r->v.size );
    bhvm_lop_ar2_mul_acc_svv_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_acc_vvs_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_mul_acc_vvs_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == b->v.size ); assert( r->v.size == 1         );
    bhvm_lop_ar2_mul_acc_vvs_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_acc_mvv_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_mul_acc_mvv_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->s.size == 2 ); assert( b->v.size == a->s.data[ 0 ] ); assert( r->v.size == a->s.data[ 1 ] );
    bhvm_lop_ar2_mul_acc_mvv_s_f_m( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, a->s.data[ 1 ], a->s.data[ 0 ] );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_acc_vmv_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_mul_acc_vmv_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( b->s.size == 2 ); assert( a->v.size == b->s.data[ 1 ] ); assert( r->v.size == b->s.data[ 0 ] );
    bhvm_lop_ar2_mul_acc_vmv_s_f_m( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, b->s.data[ 1 ], b->s.data[ 0 ] );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_acc_tvv_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_mul_acc_tvv_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->s.size == 2 ); assert( b->v.size == a->s.data[ 1 ] ); assert( r->v.size == a->s.data[ 0 ] );
    bhvm_lop_ar2_mul_acc_tvv_s_f_m( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, a->s.data[ 1 ], a->s.data[ 0 ] );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_acc_vtv_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_mul_acc_vtv_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( b->s.size == 2 ); assert( a->v.size == b->s.data[ 0 ] ); assert( r->v.size == b->s.data[ 1 ] );
    bhvm_lop_ar2_mul_acc_vtv_s_f_m( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, b->s.data[ 1 ], b->s.data[ 0 ] );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_acc_vvm_s )
"aware bhvm_hop_ar2"
"{"
"}";

void bhvm_hop_ar2_mul_acc_vvm_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( r->s.size == 2 ); assert( a->v.size == r->s.data[ 1 ] ); assert( b->v.size == r->s.data[ 0 ] );
    bhvm_lop_ar2_mul_acc_vvm_s_f_m( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->s.data[ 1 ], r->s.data[ 0 ] );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_acc_mmm_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_acc_mtm_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_acc_tmm_s )
"aware bhvm_hop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar2_mul_acc_ttm_s )
"aware bhvm_hop_ar2"
"{"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hop_ar3

BCORE_DEFINE_OBJECT_INST_P( bhvm_hop_ar3_div_dp_b_vab_s )
"aware bhvm_hop_ar3"
"{"
"}";

void bhvm_hop_ar3_div_dp_b_vab_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, const bhvm_holor_s* c, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    assert( b->v.size == r->v.size );
    assert( c->v.size == r->v.size );
    bhvm_lop_ar3_div_dp_b_vab_s_f( BKNIT_FA4( a->v.type, b->v.type, c->v.type, r->v.type ), a->v.data, b->v.data, c->v.data, r->v.data, r->v.size );
}

/**********************************************************************************************************************/
// source: bhvm_mcode.h
#include "bhvm_mcode.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_mcode

BCORE_DEFINE_OBJECT_INST_P( bhvm_mcode_op_s )
"aware bhvm_mcode"
"{"
    "aware bhvm_vop => vop;"
    "private bhvm_vop_s* p;"
    "func bcore_inst_call:copy_x;"
    "func bcore_via_call:mutated;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_mcode_track_s )
"aware bcore_array"
"{"
    "tp_t name;"
    "bhvm_mcode_op_s [];"
    "func bhvm_vop:run;"
"}";

void bhvm_mcode_track_s_vop_push_d( bhvm_mcode_track_s* o, bhvm_vop* vop )
{
    assert( vop );
    bhvm_mcode_op_s* op = bhvm_mcode_track_s_push( o );
    op->vop = vop;
    op->p = ( bhvm_vop_s* )bhvm_vop_s_get_aware( op->vop );
    assert( op->p );
}

void bhvm_mcode_track_s_vop_push_c( bhvm_mcode_track_s* o, const bhvm_vop* vop )
{
    bhvm_mcode_track_s_vop_push_d( o, bhvm_vop_a_clone( vop ) );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_mcode_track_ads_s )
"aware bcore_array"
"{"
    "bhvm_mcode_track_s => [];"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_mcode_lib_s )
"aware bhvm_mcode"
"{"
    "bhvm_mcode_track_ads_s arr;"
    "bcore_hmap_tpuz_s map;"
"}";

bhvm_mcode_track_s* bhvm_mcode_lib_s_track_get( bhvm_mcode_lib_s* o, tp_t name )
{
    uz_t* pidx = bcore_hmap_tpuz_s_get( &o->map, name );
    if( !pidx ) return NULL;
    return o->arr.data[ *pidx ];
}

bhvm_mcode_track_s* bhvm_mcode_lib_s_track_get_or_new( bhvm_mcode_lib_s* o, tp_t name )
{
    if( bcore_hmap_tpuz_s_exists( &o->map, name ) ) return bhvm_mcode_lib_s_track_get( o, name );
    bcore_hmap_tpuz_s_set( &o->map, name, o->arr.size );
    bhvm_mcode_track_s* track = bhvm_mcode_track_ads_s_push( &o->arr );
    track->name = name;
    return track;
}

bhvm_mcode_track_s* bhvm_mcode_lib_s_track_reset( bhvm_mcode_lib_s* o, tp_t name )
{
    bhvm_mcode_track_s* track = bhvm_mcode_lib_s_track_get_or_new( o, name );
    bhvm_mcode_track_s_clear( track );
    return track;
}

void bhvm_mcode_lib_s_track_vop_set_args_push_d( bhvm_mcode_lib_s* o, tp_t name, bhvm_vop* vop, const bhvm_vop_arr_ci_s* arr_ci )
{
    bhvm_vop_a_set_args( vop, arr_ci );
    bhvm_mcode_track_s_vop_push_d( bhvm_mcode_lib_s_track_get_or_new( o, name ), vop );
}

void bhvm_mcode_lib_s_track_push( bhvm_mcode_lib_s* o, tp_t name, tp_t src_name )
{
    bhvm_mcode_track_s* src = bhvm_mcode_lib_s_track_get( o, src_name );
    if( !src ) return;
    bhvm_mcode_track_s* dst = bhvm_mcode_lib_s_track_get_or_new( o, name );
    BFOR_EACH( i, src ) bhvm_mcode_track_s_vop_push_c( dst, src->data[ i ].vop );
}

void bhvm_mcode_lib_s_track_push_reverse( bhvm_mcode_lib_s* o, tp_t name, tp_t src_name )
{
    bhvm_mcode_track_s* src = bhvm_mcode_lib_s_track_get( o, src_name );
    if( !src ) return;
    bhvm_mcode_track_s* dst = bhvm_mcode_lib_s_track_get_or_new( o, name );
    for( sz_t i = src->size - 1; i >= 0; i-- ) bhvm_mcode_track_s_vop_push_c( dst, src->data[ i ].vop );
}

void bhvm_mcode_lib_s_track_remove( bhvm_mcode_lib_s* o, tp_t name )
{
    uz_t* pidx = bcore_hmap_tpuz_s_get( &o->map, name );
    if( !pidx ) return;
    sz_t idx = *pidx;
    bhvm_mcode_track_s_discard( o->arr.data[ idx ] );
    o->arr.size--;
    o->arr.data[ idx ] = o->arr.data[ o->arr.size ];
    o->arr.data[ o->arr.size ] = NULL;
    if( idx < o->arr.size ) bcore_hmap_tpuz_s_set( &o->map, o->arr.data[ idx ]->name, idx );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_mcode_frame_s )
"aware bhvm_mcode"
"{"
    "bhvm_mcode_lib_s => lib;"
    "bhvm_mcode_hbase_s => hbase;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_mcode_hmeta

BCORE_DEFINE_OBJECT_INST_P( bhvm_mcode_hmeta_adl_s )
"aware bcore_array"
"{"
    "aware bhvm_mcode_hmeta=> [];"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_mcode_hbase

BCORE_DEFINE_OBJECT_INST_P( bhvm_mcode_hbase_s )
"aware bhvm_mcode_hbase"
"{"
    "bhvm_holor_ads_s holor_ads;"
    "bhvm_mcode_hmeta_adl_s hmeta_adl;"
"}";

/**********************************************************************************************************************/

vd_t bhvm_planted_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bhvm_planted" ) ) )
    {
        case TYPEOF_init1:
        {
            // Comment or remove line below to rebuild this target.
            bcore_const_x_set_d( typeof( "bhvm_planted_hash" ), sr_tp( 2234091348 ) );

            // --------------------------------------------------------------------
            // source: bhvm_holor.h

            // group: bhvm
            BCORE_REGISTER_OBJECT( bhvm_shape_s );
            BCORE_REGISTER_OBJECT( bhvm_value_s );
            BCORE_REGISTER_FFUNC( bcore_fp_copy_typed, bhvm_holor_s_copy_typed );
            BCORE_REGISTER_OBJECT( bhvm_holor_s );
            BCORE_REGISTER_OBJECT( bhvm_holor_adl_s );
            BCORE_REGISTER_OBJECT( bhvm_holor_ads_s );
            BCORE_REGISTER_TRAIT( bhvm, bcore_inst );

            // --------------------------------------------------------------------
            // source: bhvm_hop.h

            // group: bhvm_hop
            BCORE_REGISTER_TRAIT( bhvm_hop, bcore_inst );

            // group: bhvm_hop_ar0
            BCORE_REGISTER_OBJECT( bhvm_hop_ar0_zro_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar0_one_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar0_nul_dp_s );
            BCORE_REGISTER_TRAIT( bhvm_hop_ar0, bhvm_hop );

            // group: bhvm_hop_ar1
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_identity_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_neg_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_floor_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_ceil_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_exp_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_inv_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_lgst_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_lgst_hard_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_lgst_leaky_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_tanh_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_tanh_hard_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_tanh_leaky_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_softplus_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_relu_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_relu_leaky_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_identity_dp_v_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_neg_dp_v_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_add_dp_a_v_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_add_dp_b_v_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_sub_dp_a_v_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar1_sub_dp_b_v_s );
            BCORE_REGISTER_TRAIT( bhvm_hop_ar1, bhvm_hop );

            // group: bhvm_hop_ar2
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_add_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_sub_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_div_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_equal_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_larger_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_smaller_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_larger_equal_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_smaller_equal_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_logic_and_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_logic_or_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_div_dp_a_vb_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_exp_dp_vy_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_inv_dp_vy_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_lgst_dp_vy_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_lgst_hard_dp_vy_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_lgst_leaky_dp_vy_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_tanh_dp_vy_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_tanh_hard_dp_vy_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_tanh_leaky_dp_vy_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_softplus_dp_vy_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_relu_dp_vy_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_relu_leaky_dp_vy_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_vvv_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_vsv_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_svv_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_vvs_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_mvv_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_vmv_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_vvm_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_tvv_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_vtv_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_mmm_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_mtm_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_tmm_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_ttm_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_acc_vvv_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_acc_vsv_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_acc_svv_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_acc_vvs_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_acc_mvv_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_acc_vmv_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_acc_tvv_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_acc_vtv_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_acc_vvm_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_acc_mmm_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_acc_mtm_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_acc_tmm_s );
            BCORE_REGISTER_OBJECT( bhvm_hop_ar2_mul_acc_ttm_s );
            BCORE_REGISTER_TRAIT( bhvm_hop_ar2, bhvm_hop );

            // group: bhvm_hop_ar3
            BCORE_REGISTER_OBJECT( bhvm_hop_ar3_div_dp_b_vab_s );
            BCORE_REGISTER_TRAIT( bhvm_hop_ar3, bhvm_hop );

            // --------------------------------------------------------------------
            // source: bhvm_mcode.h

            // group: bhvm_mcode
            BCORE_REGISTER_FFUNC( bcore_inst_call_copy_x, bhvm_mcode_op_s_copy_x );
            BCORE_REGISTER_FFUNC( bcore_via_call_mutated, bhvm_mcode_op_s_mutated );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_mcode_op_s_run );
            BCORE_REGISTER_OBJECT( bhvm_mcode_op_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_mcode_track_s_run );
            BCORE_REGISTER_OBJECT( bhvm_mcode_track_s );
            BCORE_REGISTER_OBJECT( bhvm_mcode_track_ads_s );
            BCORE_REGISTER_OBJECT( bhvm_mcode_lib_s );
            BCORE_REGISTER_OBJECT( bhvm_mcode_frame_s );
            BCORE_REGISTER_TRAIT( bhvm_mcode, bcore_inst );

            // group: bhvm_mcode_hmeta
            BCORE_REGISTER_OBJECT( bhvm_mcode_hmeta_adl_s );
            BCORE_REGISTER_TRAIT( bhvm_mcode_hmeta, bhvm_mcode );

            // group: bhvm_mcode_hbase
            BCORE_REGISTER_OBJECT( bhvm_mcode_hbase_s );
            BCORE_REGISTER_TRAIT( bhvm_mcode_hbase, bhvm_mcode );
        }
        break;
        default: break;
    }
    return NULL;
}
