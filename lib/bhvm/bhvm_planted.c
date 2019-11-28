/** This file was generated from beth-plant source code.
 *  Compiling Agent : bcore_plant_compiler (C) 2019 J.B.Steffens
 *  Last File Update: 2019-11-28T22:03:50Z
 *
 *  Copyright and License of this File:
 *
 *  Generated code inherits the copyright and license of the underlying beth-plant source code.
 *  Source code defining this file is distributed across following files:
 *
 *  bhvm_hf3.h
 *  bhvm_hf3_op.h
 *  bhvm_hf3_vm.h
 *
 */

#include "bhvm_planted.h"
#include "bcore_spect.h"
#include "bcore_spect_inst.h"
#include "bcore_sr.h"
#include "bcore_const_manager.h"


/**********************************************************************************************************************/
// source: bhvm_hf3.h
#include "bhvm_hf3.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_s )
"aware bcore_inst"
"{"
    "sz_t [] d;"
    "f3_t [] v;"
    "bl_t htp;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_adl_s )
"aware bcore_array"
"{"
    "bhvm_hf3_s => [];"
"}";

/**********************************************************************************************************************/
// source: bhvm_hf3_op.h
#include "bhvm_hf3_op.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_op

BCORE_DEFINE_SPECT( bcore_inst, bhvm_hf3_op )
"{"
    "bcore_spect_header_s header;"
    "feature aware bhvm_hf3_op : get_arity;"
    "feature aware bhvm_hf3_op : aof;"
    "feature aware bhvm_hf3_op : osig;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_op_ar0

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar0_zro_s )
"aware bhvm_hf3_op_ar0"
"{"
    "func ^:f;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar0:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar0_literal_s )
"aware bhvm_hf3_op_ar0"
"{"
    "bhvm_hf3_s -> h;"
    "func ^:of;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar0:f;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_SPECT( bhvm_hf3_op, bhvm_hf3_op_ar0 )
"{"
    "bcore_spect_header_s header;"
    "feature aware bhvm_hf3_op_ar0 : of;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_op_ar0_dp

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar0_dp_ca_floor_s )
"aware bhvm_hf3_op_ar0_dp"
"{"
    "func bhvm_hf3_op_ar0:f;"
    "func bhvm_hf3_op:sig;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar0:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar0_dp_ca_ceil_s )
"aware bhvm_hf3_op_ar0_dp"
"{"
    "func bhvm_hf3_op_ar0:f;"
    "func bhvm_hf3_op:sig;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar0:of;"
    "func bhvm_hf3_op:aof;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_op_ar1

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar1_cpy_s )
"aware bhvm_hf3_op_ar1"
"{"
    "func ^:f;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar1:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar1_neg_s )
"aware bhvm_hf3_op_ar1"
"{"
    "func ^:f;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar1:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar1_floor_s )
"aware bhvm_hf3_op_ar1"
"{"
    "func ^:f;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar1:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar1_ceil_s )
"aware bhvm_hf3_op_ar1"
"{"
    "func ^:f;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar1:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar1_exp_s )
"aware bhvm_hf3_op_ar1"
"{"
    "func ^:f;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar1:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar1_lgst_s )
"aware bhvm_hf3_op_ar1"
"{"
    "func ^:f;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar1:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar1_lgst_hard_s )
"aware bhvm_hf3_op_ar1"
"{"
    "func ^:f;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar1:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar1_lgst_leaky_s )
"aware bhvm_hf3_op_ar1"
"{"
    "func ^:f;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar1:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar1_tanh_s )
"aware bhvm_hf3_op_ar1"
"{"
    "func ^:f;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar1:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar1_tanh_hard_s )
"aware bhvm_hf3_op_ar1"
"{"
    "func ^:f;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar1:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar1_tanh_leaky_s )
"aware bhvm_hf3_op_ar1"
"{"
    "func ^:f;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar1:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar1_softplus_s )
"aware bhvm_hf3_op_ar1"
"{"
    "func ^:f;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar1:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar1_relu_s )
"aware bhvm_hf3_op_ar1"
"{"
    "func ^:f;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar1:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar1_relu_leaky_s )
"aware bhvm_hf3_op_ar1"
"{"
    "func ^:f;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar1:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar1_cast_htp_s )
"aware bhvm_hf3_op_ar1"
"{"
    "func ^:f;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar1:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_SPECT( bhvm_hf3_op, bhvm_hf3_op_ar1 )
"{"
    "bcore_spect_header_s header;"
    "feature aware bhvm_hf3_op_ar1 : of;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_op_ar1_dp

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar1_dp_ca_cpy_s )
"aware bhvm_hf3_op_ar1_dp"
"{"
    "func bhvm_hf3_op_ar1:f;"
    "func bhvm_hf3_op:sig;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar1:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar1_dp_ca_neg_s )
"aware bhvm_hf3_op_ar1_dp"
"{"
    "func bhvm_hf3_op_ar1:f;"
    "func bhvm_hf3_op:sig;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar1:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar1_dp_ca_add_s )
"aware bhvm_hf3_op_ar1_dp"
"{"
    "func bhvm_hf3_op_ar1:f;"
    "func bhvm_hf3_op:sig;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar1:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar1_dp_cb_add_s )
"aware bhvm_hf3_op_ar1_dp"
"{"
    "func bhvm_hf3_op_ar1:f;"
    "func bhvm_hf3_op:sig;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar1:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar1_dp_ca_sub_s )
"aware bhvm_hf3_op_ar1_dp"
"{"
    "func bhvm_hf3_op_ar1:f;"
    "func bhvm_hf3_op:sig;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar1:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar1_dp_cb_sub_s )
"aware bhvm_hf3_op_ar1_dp"
"{"
    "func bhvm_hf3_op_ar1:f;"
    "func bhvm_hf3_op:sig;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar1:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar1_dp_ca_cast_htp_s )
"aware bhvm_hf3_op_ar1_dp"
"{"
    "func bhvm_hf3_op:sig;"
    "func bhvm_hf3_op_ar1:f;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar1:of;"
    "func bhvm_hf3_op:aof;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_op_ar2

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar2_add_s )
"aware bhvm_hf3_op_ar2"
"{"
    "func ^:f;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar2:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar2_sub_s )
"aware bhvm_hf3_op_ar2"
"{"
    "func ^:f;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar2:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar2_hmul_s )
"aware bhvm_hf3_op_ar2"
"{"
    "func ^:f;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar2:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar2_hdiv_s )
"aware bhvm_hf3_op_ar2"
"{"
    "func ^:f;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar2:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar2_bmul_s )
"aware bhvm_hf3_op_ar2"
"{"
    "func ^:f;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar2:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar2_mul_scl_s )
"aware bhvm_hf3_op_ar2"
"{"
    "func ^:f;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar2:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar2_scl_mul_s )
"aware bhvm_hf3_op_ar2"
"{"
    "func ^:f;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar2:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_SPECT( bhvm_hf3_op, bhvm_hf3_op_ar2 )
"{"
    "bcore_spect_header_s header;"
    "feature aware bhvm_hf3_op_ar2 : of;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_op_ar2_dp

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar2_dp_ca_exp_s )
"aware bhvm_hf3_op_ar2_dp"
"{"
    "func bhvm_hf3_op_ar2:f;"
    "func bhvm_hf3_op:sig;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar2:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar2_dp_ca_lgst_s )
"aware bhvm_hf3_op_ar2_dp"
"{"
    "func bhvm_hf3_op_ar2:f;"
    "func bhvm_hf3_op:sig;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar2:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar2_dp_ca_lgst_hard_s )
"aware bhvm_hf3_op_ar2_dp"
"{"
    "func bhvm_hf3_op_ar2:f;"
    "func bhvm_hf3_op:sig;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar2:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar2_dp_ca_lgst_leaky_s )
"aware bhvm_hf3_op_ar2_dp"
"{"
    "func bhvm_hf3_op_ar2:f;"
    "func bhvm_hf3_op:sig;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar2:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar2_dp_ca_tanh_s )
"aware bhvm_hf3_op_ar2_dp"
"{"
    "func bhvm_hf3_op_ar2:f;"
    "func bhvm_hf3_op:sig;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar2:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar2_dp_ca_tanh_hard_s )
"aware bhvm_hf3_op_ar2_dp"
"{"
    "func bhvm_hf3_op_ar2:f;"
    "func bhvm_hf3_op:sig;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar2:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar2_dp_ca_tanh_leaky_s )
"aware bhvm_hf3_op_ar2_dp"
"{"
    "func bhvm_hf3_op_ar2:f;"
    "func bhvm_hf3_op:sig;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar2:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar2_dp_ca_softplus_s )
"aware bhvm_hf3_op_ar2_dp"
"{"
    "func bhvm_hf3_op_ar2:f;"
    "func bhvm_hf3_op:sig;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar2:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar2_dp_ca_relu_s )
"aware bhvm_hf3_op_ar2_dp"
"{"
    "func bhvm_hf3_op_ar2:f;"
    "func bhvm_hf3_op:sig;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar2:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar2_dp_ca_relu_leaky_s )
"aware bhvm_hf3_op_ar2_dp"
"{"
    "func bhvm_hf3_op_ar2:f;"
    "func bhvm_hf3_op:sig;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar2:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar2_dp_ca_hmul_s )
"aware bhvm_hf3_op_ar2_dp"
"{"
    "func bhvm_hf3_op:sig;"
    "func bhvm_hf3_op_ar2:f;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar2:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar2_dp_cb_hmul_s )
"aware bhvm_hf3_op_ar2_dp"
"{"
    "func bhvm_hf3_op:sig;"
    "func bhvm_hf3_op_ar2:f;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar2:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar2_dp_ca_bmul_s )
"aware bhvm_hf3_op_ar2_dp"
"{"
    "func bhvm_hf3_op:sig;"
    "func bhvm_hf3_op_ar2:f;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar2:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar2_dp_cb_bmul_s )
"aware bhvm_hf3_op_ar2_dp"
"{"
    "func bhvm_hf3_op:sig;"
    "func bhvm_hf3_op_ar2:f;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar2:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar2_dp_ca_mul_scl_s )
"aware bhvm_hf3_op_ar2_dp"
"{"
    "func bhvm_hf3_op:sig;"
    "func bhvm_hf3_op_ar2:f;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar2:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar2_dp_cb_mul_scl_s )
"aware bhvm_hf3_op_ar2_dp"
"{"
    "func bhvm_hf3_op:sig;"
    "func bhvm_hf3_op_ar2:f;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar2:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar2_dp_ca_scl_mul_s )
"aware bhvm_hf3_op_ar2_dp"
"{"
    "func bhvm_hf3_op:sig;"
    "func bhvm_hf3_op_ar2:f;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar2:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar2_dp_cb_scl_mul_s )
"aware bhvm_hf3_op_ar2_dp"
"{"
    "func bhvm_hf3_op:sig;"
    "func bhvm_hf3_op_ar2:f;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar2:of;"
    "func bhvm_hf3_op:aof;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_op_ar3

BCORE_DEFINE_SPECT( bhvm_hf3_op, bhvm_hf3_op_ar3 )
"{"
    "bcore_spect_header_s header;"
    "feature aware bhvm_hf3_op_ar3 : of;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_op_ar3_dp

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar3_dp_ca_hdiv_s )
"aware bhvm_hf3_op_ar3_dp"
"{"
    "func bhvm_hf3_op_ar3:f;"
    "func bhvm_hf3_op:sig;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar3:of;"
    "func bhvm_hf3_op:aof;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_op_ar3_dp_cb_hdiv_s )
"aware bhvm_hf3_op_ar3_dp"
"{"
    "func bhvm_hf3_op_ar3:f;"
    "func bhvm_hf3_op:sig;"
    "func bhvm_hf3_op:get_arity;"
    "func bhvm_hf3_op_ar3:of;"
    "func bhvm_hf3_op:aof;"
"}";

/**********************************************************************************************************************/
// source: bhvm_hf3_vm.h
#include "bhvm_hf3_vm.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_vm

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_holor_s )
"aware bhvm_hf3_vm"
"{"
    "tp_t name;"
    "tp_t type;"
    "sz_t idx_paired = -1;"
    "bhvm_hf3_s h;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_arr_holor_s )
"aware bcore_array"
"{"
    "bhvm_hf3_vm_holor_s [];"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_prop_s )
"aware bhvm_hf3_vm"
"{"
    "aware bhvm_hf3_vm_op => op;"
    "private bhvm_hf3_vm_s* p;"
    "func bcore_inst_call:copy_x;"
    "func bcore_via_call:mutated;"
    "func ^:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_proc_s )
"aware bcore_array"
"{"
    "tp_t name;"
    "bhvm_hf3_vm_prop_s [];"
    "func bhvm_hf3_vm:run;"
"}";

void bhvm_hf3_vm_proc_s_run( const bhvm_hf3_vm_proc_s* o, bhvm_hf3_vm_holor_s* ah )
{
    for( sz_t i = 0; i < o->size; i++ ) bhvm_hf3_vm_prop_s_run( &o->data[ i ], ah );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_library_s )
"aware bcore_array"
"{"
    "bhvm_hf3_vm_proc_s => [];"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_mop_s )
"aware bhvm_hf3_vm"
"{"
    "aware bhvm_hf3_vm_op => op;"
    "private bhvm_hf3_vm_s* p;"
    "func bcore_inst_call:copy_x;"
    "func bcore_via_call:mutated;"
    "func ^:mrun;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_mcode_s )
"aware bcore_array"
"{"
    "tp_t name;"
    "bhvm_hf3_vm_mop_s [];"
    "func bhvm_hf3_vm:mrun;"
    "func bhvm_hf3_vm:op_push_d;"
    "func bhvm_hf3_vm:op_push_c;"
"}";

void bhvm_hf3_vm_mcode_s_op_push_d( bhvm_hf3_vm_mcode_s* o, bhvm_hf3_vm_op* op )
{
    assert( op );
    bhvm_hf3_vm_mop_s* mop = bhvm_hf3_vm_mcode_s_push( o );
    mop->op = op;
    mop->p = (bhvm_hf3_vm_s*)bhvm_hf3_vm_s_get_aware( mop->op );
    assert( mop->p );
}

void bhvm_hf3_vm_mcode_s_op_push_c( bhvm_hf3_vm_mcode_s* o, const bhvm_hf3_vm_op* op )
{
    bhvm_hf3_vm_mcode_s_op_push_d( o, bhvm_hf3_vm_op_a_clone( op ) );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_arr_mcode_s )
"aware bcore_array"
"{"
    "bhvm_hf3_vm_mcode_s => [];"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_lib_mcode_s )
"aware bhvm_hf3_vm"
"{"
    "bhvm_hf3_vm_arr_mcode_s arr;"
    "bcore_hmap_tpuz_s map;"
    "func ^:clear;"
    "func ^:mcode_exists;"
    "func ^:mcode_get;"
    "func ^:mcode_get_or_new;"
    "func ^:mcode_reset;"
    "func ^:mcode_op_push_c;"
    "func ^:mcode_op_push_d;"
    "func ^:mcode_push;"
    "func ^:mcode_push_reverse;"
    "func ^:mcode_remove;"
    "func ^:mcode_mrun;"
"}";

bhvm_hf3_vm_mcode_s* bhvm_hf3_vm_lib_mcode_s_mcode_get( bhvm_hf3_vm_lib_mcode_s* o, tp_t name )
{
    uz_t* pidx = bcore_hmap_tpuz_s_get( &o->map, name );
    if( !pidx ) return NULL;
    return o->arr.data[ *pidx ];
}

bhvm_hf3_vm_mcode_s* bhvm_hf3_vm_lib_mcode_s_mcode_get_or_new( bhvm_hf3_vm_lib_mcode_s* o, tp_t name )
{
    if( bcore_hmap_tpuz_s_exists( &o->map, name ) ) return bhvm_hf3_vm_lib_mcode_s_mcode_get( o, name );
    bcore_hmap_tpuz_s_set( &o->map, name, o->arr.size );
    bhvm_hf3_vm_mcode_s* mcode = bhvm_hf3_vm_arr_mcode_s_push( &o->arr );
    mcode->name = name;
    return mcode;
}

bhvm_hf3_vm_mcode_s* bhvm_hf3_vm_lib_mcode_s_mcode_reset( bhvm_hf3_vm_lib_mcode_s* o, tp_t name )
{
    bhvm_hf3_vm_mcode_s* mcode = bhvm_hf3_vm_lib_mcode_s_mcode_get_or_new( o, name );
    bhvm_hf3_vm_mcode_s_clear( mcode );
    return mcode;
}

void bhvm_hf3_vm_lib_mcode_s_mcode_push( bhvm_hf3_vm_lib_mcode_s* o, tp_t name, tp_t src_name )
{
    bhvm_hf3_vm_mcode_s* src = bhvm_hf3_vm_lib_mcode_s_mcode_get( o, src_name );
    if( !src ) return;
    bhvm_hf3_vm_mcode_s* dst = bhvm_hf3_vm_lib_mcode_s_mcode_get_or_new( o, name );
    BFOR_EACH( i, src ) bhvm_hf3_vm_mcode_s_op_push_c( dst, src->data[ i ].op );
}

void bhvm_hf3_vm_lib_mcode_s_mcode_push_reverse( bhvm_hf3_vm_lib_mcode_s* o, tp_t name, tp_t src_name )
{
    bhvm_hf3_vm_mcode_s* src = bhvm_hf3_vm_lib_mcode_s_mcode_get( o, src_name );
    if( !src ) return;
    bhvm_hf3_vm_mcode_s* dst = bhvm_hf3_vm_lib_mcode_s_mcode_get_or_new( o, name );
    for( sz_t i = src->size - 1; i >= 0; i-- ) bhvm_hf3_vm_mcode_s_op_push_c( dst, src->data[ i ].op );
}

void bhvm_hf3_vm_lib_mcode_s_mcode_remove( bhvm_hf3_vm_lib_mcode_s* o, tp_t name )
{
    uz_t* pidx = bcore_hmap_tpuz_s_get( &o->map, name );
    if( !pidx ) return;
    sz_t idx = *pidx;
    bhvm_hf3_vm_mcode_s_discard( o->arr.data[ idx ] );
    o->arr.size--;
    o->arr.data[ idx ] = o->arr.data[ o->arr.size ];
    o->arr.data[ o->arr.size ] = NULL;
    if( idx < o->arr.size ) bcore_hmap_tpuz_s_set( &o->map, o->arr.data[ idx ]->name, idx );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_frame_s )
"aware bhvm_hf3_vm"
"{"
    "bhvm_hf3_vm_arr_holor_s arr_holor;"
    "bhvm_hf3_vm_lib_mcode_s lib_mcode;"
    "bcore_hmap_name_s map_name;"
    "tp_t mcode_setup = 0;"
    "tp_t mcode_shelve = 0;"
    "bcore_arr_sz_s input;"
    "bcore_arr_sz_s output;"
    "func ^:setup;"
    "func bcore_via_call:mutated;"
    "func bcore_via_call:shelve;"
    "func ^:mcode_exists;"
    "func ^:mcode_get;"
    "func ^:mcode_get_or_new;"
    "func ^:mcode_reset;"
    "func ^:mcode_op_push_d;"
    "func ^:mcode_op_push_c;"
    "func ^:mcode_push;"
    "func ^:mcode_push_reverse;"
    "func ^:mcode_remove;"
    "func ^:mcode_run;"
"}";

BCORE_DEFINE_SPECT( bcore_inst, bhvm_hf3_vm )
"{"
    "bcore_spect_header_s header;"
    "feature aware bhvm_hf3_vm : run;"
    "feature aware bhvm_hf3_vm : mrun;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_vm_op

BCORE_DEFINE_SPECT( bhvm_hf3_vm, bhvm_hf3_vm_op )
"{"
    "bcore_spect_header_s header;"
    "feature aware bhvm_hf3_vm_op : get_arity;"
    "feature aware bhvm_hf3_vm_op : set_indices;"
    "feature aware bhvm_hf3_vm_op : get_indices;"
    "feature aware bhvm_hf3_vm_op : sig;"
    "feature aware bhvm_hf3_vm_op : set_arg;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_vm_op_ar0

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar0_nul_s )
"aware bhvm_hf3_vm_op_ar0"
"{"
    "sz_t a = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
    "func bhvm_hf3_vm_op:sig;"
"}";

void bhvm_hf3_vm_op_ar0_nul_s_set_arg( bhvm_hf3_vm_op_ar0_nul_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar0_nul_s_sig( o )[0] ) o->a = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar0_zro_s )
"aware bhvm_hf3_vm_op_ar0"
"{"
    "sz_t a = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
    "func bhvm_hf3_vm_op:sig;"
"}";

void bhvm_hf3_vm_op_ar0_zro_s_set_arg( bhvm_hf3_vm_op_ar0_zro_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar0_zro_s_sig( o )[0] ) o->a = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar0_zro_nc_s )
"aware bhvm_hf3_vm_op_ar0"
"{"
    "sz_t a = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
    "func bhvm_hf3_vm_op:sig;"
"}";

void bhvm_hf3_vm_op_ar0_zro_nc_s_set_arg( bhvm_hf3_vm_op_ar0_zro_nc_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar0_zro_nc_s_sig( o )[0] ) o->a = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar0_determine_s )
"aware bhvm_hf3_vm_op_ar0"
"{"
    "sz_t a = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
    "func bhvm_hf3_vm_op:sig;"
"}";

void bhvm_hf3_vm_op_ar0_determine_s_set_arg( bhvm_hf3_vm_op_ar0_determine_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar0_determine_s_sig( o )[0] ) o->a = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar0_vacate_s )
"aware bhvm_hf3_vm_op_ar0"
"{"
    "sz_t a = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
    "func bhvm_hf3_vm_op:sig;"
"}";

void bhvm_hf3_vm_op_ar0_vacate_s_set_arg( bhvm_hf3_vm_op_ar0_vacate_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar0_vacate_s_sig( o )[0] ) o->a = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar0_clear_s )
"aware bhvm_hf3_vm_op_ar0"
"{"
    "sz_t a = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
    "func bhvm_hf3_vm_op:sig;"
"}";

void bhvm_hf3_vm_op_ar0_clear_s_set_arg( bhvm_hf3_vm_op_ar0_clear_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar0_clear_s_sig( o )[0] ) o->a = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar0_randomize_s )
"aware bhvm_hf3_vm_op_ar0"
"{"
    "sz_t a = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "u2_t rseed = 1234;"
    "f3_t min = -0.5;"
    "f3_t max = 0.5;"
    "f3_t density = 1.0;"
    "func ^:csetup_randomize;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
    "func bhvm_hf3_vm_op:sig;"
"}";

bhvm_hf3_vm_op* bhvm_hf3_vm_op_ar0_randomize_s_csetup_randomize( bhvm_hf3_vm_op_ar0_randomize_s* o, sz_t idx_a, u2_t rseed, f3_t density, f3_t min, f3_t max )
{
    if( !o ) o = bhvm_hf3_vm_op_ar0_randomize_s_create();
    o->a = idx_a;
    o->rseed   = rseed;
    o->density = density;
    o->min     = min;
    o->max     = max;
    return (bhvm_hf3_vm_op*)o;
}

void bhvm_hf3_vm_op_ar0_randomize_s_set_arg( bhvm_hf3_vm_op_ar0_randomize_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar0_randomize_s_sig( o )[0] ) o->a = idx;
}

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_vm_op_ar0_dp

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar0_dp_ca_floor_s )
"aware bhvm_hf3_vm_op_ar0"
"{"
    "sz_t a = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:sig;"
    "func bhvm_hf3_vm_op:set_arg;"
"}";

void bhvm_hf3_vm_op_ar0_dp_ca_floor_s_set_arg( bhvm_hf3_vm_op_ar0_dp_ca_floor_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar0_dp_ca_floor_s_sig( o )[0] ) o->a = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar0_dp_ca_ceil_s )
"aware bhvm_hf3_vm_op_ar0"
"{"
    "sz_t a = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:sig;"
    "func bhvm_hf3_vm_op:set_arg;"
"}";

void bhvm_hf3_vm_op_ar0_dp_ca_ceil_s_set_arg( bhvm_hf3_vm_op_ar0_dp_ca_ceil_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar0_dp_ca_ceil_s_sig( o )[0] ) o->a = idx;
}

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_vm_op_ar1

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar1_unary_s )
"aware bhvm_hf3_vm_op_ar1"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "bmath_fp_f3_ar1 unary;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
    "func bhvm_hf3_vm_op:sig;"
"}";

void bhvm_hf3_vm_op_ar1_unary_s_set_arg( bhvm_hf3_vm_op_ar1_unary_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar1_unary_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar1_unary_s_sig( o )[ 1 ] ) o->b = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar1_cpy_s )
"aware bhvm_hf3_vm_op_ar1"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
    "func bhvm_hf3_vm_op:sig;"
"}";

void bhvm_hf3_vm_op_ar1_cpy_s_set_arg( bhvm_hf3_vm_op_ar1_cpy_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar1_cpy_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar1_cpy_s_sig( o )[ 1 ] ) o->b = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar1_neg_s )
"aware bhvm_hf3_vm_op_ar1"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
    "func bhvm_hf3_vm_op:sig;"
"}";

void bhvm_hf3_vm_op_ar1_neg_s_set_arg( bhvm_hf3_vm_op_ar1_neg_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar1_neg_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar1_neg_s_sig( o )[ 1 ] ) o->b = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar1_floor_s )
"aware bhvm_hf3_vm_op_ar1"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
    "func bhvm_hf3_vm_op:sig;"
"}";

void bhvm_hf3_vm_op_ar1_floor_s_set_arg( bhvm_hf3_vm_op_ar1_floor_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar1_floor_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar1_floor_s_sig( o )[ 1 ] ) o->b = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar1_ceil_s )
"aware bhvm_hf3_vm_op_ar1"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
    "func bhvm_hf3_vm_op:sig;"
"}";

void bhvm_hf3_vm_op_ar1_ceil_s_set_arg( bhvm_hf3_vm_op_ar1_ceil_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar1_ceil_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar1_ceil_s_sig( o )[ 1 ] ) o->b = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar1_exp_s )
"aware bhvm_hf3_vm_op_ar1"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
    "func bhvm_hf3_vm_op:sig;"
"}";

void bhvm_hf3_vm_op_ar1_exp_s_set_arg( bhvm_hf3_vm_op_ar1_exp_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar1_exp_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar1_exp_s_sig( o )[ 1 ] ) o->b = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar1_cpy_ay_s )
"aware bhvm_hf3_vm_op_ar1"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm_op:sig;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
"}";

void bhvm_hf3_vm_op_ar1_cpy_ay_s_set_arg( bhvm_hf3_vm_op_ar1_cpy_ay_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar1_cpy_ay_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar1_cpy_ay_s_sig( o )[ 1 ] ) o->b = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar1_cpy_by_s )
"aware bhvm_hf3_vm_op_ar1"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm_op:sig;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
"}";

void bhvm_hf3_vm_op_ar1_cpy_by_s_set_arg( bhvm_hf3_vm_op_ar1_cpy_by_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar1_cpy_by_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar1_cpy_by_s_sig( o )[ 1 ] ) o->b = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar1_lgst_s )
"aware bhvm_hf3_vm_op_ar1"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
    "func bhvm_hf3_vm_op:sig;"
"}";

void bhvm_hf3_vm_op_ar1_lgst_s_set_arg( bhvm_hf3_vm_op_ar1_lgst_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar1_lgst_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar1_lgst_s_sig( o )[ 1 ] ) o->b = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar1_lgst_hard_s )
"aware bhvm_hf3_vm_op_ar1"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
    "func bhvm_hf3_vm_op:sig;"
"}";

void bhvm_hf3_vm_op_ar1_lgst_hard_s_set_arg( bhvm_hf3_vm_op_ar1_lgst_hard_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar1_lgst_hard_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar1_lgst_hard_s_sig( o )[ 1 ] ) o->b = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar1_lgst_leaky_s )
"aware bhvm_hf3_vm_op_ar1"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
    "func bhvm_hf3_vm_op:sig;"
"}";

void bhvm_hf3_vm_op_ar1_lgst_leaky_s_set_arg( bhvm_hf3_vm_op_ar1_lgst_leaky_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar1_lgst_leaky_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar1_lgst_leaky_s_sig( o )[ 1 ] ) o->b = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar1_tanh_s )
"aware bhvm_hf3_vm_op_ar1"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
    "func bhvm_hf3_vm_op:sig;"
"}";

void bhvm_hf3_vm_op_ar1_tanh_s_set_arg( bhvm_hf3_vm_op_ar1_tanh_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar1_tanh_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar1_tanh_s_sig( o )[ 1 ] ) o->b = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar1_tanh_hard_s )
"aware bhvm_hf3_vm_op_ar1"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
    "func bhvm_hf3_vm_op:sig;"
"}";

void bhvm_hf3_vm_op_ar1_tanh_hard_s_set_arg( bhvm_hf3_vm_op_ar1_tanh_hard_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar1_tanh_hard_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar1_tanh_hard_s_sig( o )[ 1 ] ) o->b = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar1_tanh_leaky_s )
"aware bhvm_hf3_vm_op_ar1"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
    "func bhvm_hf3_vm_op:sig;"
"}";

void bhvm_hf3_vm_op_ar1_tanh_leaky_s_set_arg( bhvm_hf3_vm_op_ar1_tanh_leaky_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar1_tanh_leaky_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar1_tanh_leaky_s_sig( o )[ 1 ] ) o->b = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar1_softplus_s )
"aware bhvm_hf3_vm_op_ar1"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
    "func bhvm_hf3_vm_op:sig;"
"}";

void bhvm_hf3_vm_op_ar1_softplus_s_set_arg( bhvm_hf3_vm_op_ar1_softplus_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar1_softplus_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar1_softplus_s_sig( o )[ 1 ] ) o->b = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar1_relu_s )
"aware bhvm_hf3_vm_op_ar1"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
    "func bhvm_hf3_vm_op:sig;"
"}";

void bhvm_hf3_vm_op_ar1_relu_s_set_arg( bhvm_hf3_vm_op_ar1_relu_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar1_relu_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar1_relu_s_sig( o )[ 1 ] ) o->b = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar1_relu_leaky_s )
"aware bhvm_hf3_vm_op_ar1"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
    "func bhvm_hf3_vm_op:sig;"
"}";

void bhvm_hf3_vm_op_ar1_relu_leaky_s_set_arg( bhvm_hf3_vm_op_ar1_relu_leaky_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar1_relu_leaky_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar1_relu_leaky_s_sig( o )[ 1 ] ) o->b = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar1_cast_htp_s )
"aware bhvm_hf3_vm_op_ar1"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
    "func bhvm_hf3_vm_op:sig;"
"}";

void bhvm_hf3_vm_op_ar1_cast_htp_s_set_arg( bhvm_hf3_vm_op_ar1_cast_htp_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar1_cast_htp_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar1_cast_htp_s_sig( o )[ 1 ] ) o->b = idx;
}

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_vm_op_ar1_dp

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar1_dp_ca_cpy_s )
"aware bhvm_hf3_vm_op_ar1"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:sig;"
    "func bhvm_hf3_vm_op:set_arg;"
"}";

void bhvm_hf3_vm_op_ar1_dp_ca_cpy_s_set_arg( bhvm_hf3_vm_op_ar1_dp_ca_cpy_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar1_dp_ca_cpy_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar1_dp_ca_cpy_s_sig( o )[ 1 ] ) o->b = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar1_dp_ca_neg_s )
"aware bhvm_hf3_vm_op_ar1"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:sig;"
    "func bhvm_hf3_vm_op:set_arg;"
"}";

void bhvm_hf3_vm_op_ar1_dp_ca_neg_s_set_arg( bhvm_hf3_vm_op_ar1_dp_ca_neg_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar1_dp_ca_neg_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar1_dp_ca_neg_s_sig( o )[ 1 ] ) o->b = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar1_dp_ca_add_s )
"aware bhvm_hf3_vm_op_ar1"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:sig;"
    "func bhvm_hf3_vm_op:set_arg;"
"}";

void bhvm_hf3_vm_op_ar1_dp_ca_add_s_set_arg( bhvm_hf3_vm_op_ar1_dp_ca_add_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar1_dp_ca_add_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar1_dp_ca_add_s_sig( o )[ 1 ] ) o->b = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar1_dp_cb_add_s )
"aware bhvm_hf3_vm_op_ar1"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:sig;"
    "func bhvm_hf3_vm_op:set_arg;"
"}";

void bhvm_hf3_vm_op_ar1_dp_cb_add_s_set_arg( bhvm_hf3_vm_op_ar1_dp_cb_add_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar1_dp_cb_add_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar1_dp_cb_add_s_sig( o )[ 1 ] ) o->b = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar1_dp_ca_sub_s )
"aware bhvm_hf3_vm_op_ar1"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:sig;"
    "func bhvm_hf3_vm_op:set_arg;"
"}";

void bhvm_hf3_vm_op_ar1_dp_ca_sub_s_set_arg( bhvm_hf3_vm_op_ar1_dp_ca_sub_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar1_dp_ca_sub_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar1_dp_ca_sub_s_sig( o )[ 1 ] ) o->b = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar1_dp_cb_sub_s )
"aware bhvm_hf3_vm_op_ar1"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:sig;"
    "func bhvm_hf3_vm_op:set_arg;"
"}";

void bhvm_hf3_vm_op_ar1_dp_cb_sub_s_set_arg( bhvm_hf3_vm_op_ar1_dp_cb_sub_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar1_dp_cb_sub_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar1_dp_cb_sub_s_sig( o )[ 1 ] ) o->b = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar1_dp_ca_cast_htp_s )
"aware bhvm_hf3_vm_op_ar1"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm_op:sig;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
"}";

void bhvm_hf3_vm_op_ar1_dp_ca_cast_htp_s_set_arg( bhvm_hf3_vm_op_ar1_dp_ca_cast_htp_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar1_dp_ca_cast_htp_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar1_dp_ca_cast_htp_s_sig( o )[ 1 ] ) o->b = idx;
}

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_vm_op_ar2

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar2_add_s )
"aware bhvm_hf3_vm_op_ar2"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "sz_t c = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
    "func bhvm_hf3_vm_op:sig;"
"}";

void bhvm_hf3_vm_op_ar2_add_s_set_arg( bhvm_hf3_vm_op_ar2_add_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar2_add_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar2_add_s_sig( o )[ 1 ] ) o->b = idx;
    if( id == bhvm_hf3_vm_op_ar2_add_s_sig( o )[ 2 ] ) o->c = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar2_sub_s )
"aware bhvm_hf3_vm_op_ar2"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "sz_t c = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
    "func bhvm_hf3_vm_op:sig;"
"}";

void bhvm_hf3_vm_op_ar2_sub_s_set_arg( bhvm_hf3_vm_op_ar2_sub_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar2_sub_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar2_sub_s_sig( o )[ 1 ] ) o->b = idx;
    if( id == bhvm_hf3_vm_op_ar2_sub_s_sig( o )[ 2 ] ) o->c = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar2_hmul_s )
"aware bhvm_hf3_vm_op_ar2"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "sz_t c = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
    "func bhvm_hf3_vm_op:sig;"
"}";

void bhvm_hf3_vm_op_ar2_hmul_s_set_arg( bhvm_hf3_vm_op_ar2_hmul_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar2_hmul_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar2_hmul_s_sig( o )[ 1 ] ) o->b = idx;
    if( id == bhvm_hf3_vm_op_ar2_hmul_s_sig( o )[ 2 ] ) o->c = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar2_hdiv_s )
"aware bhvm_hf3_vm_op_ar2"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "sz_t c = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
    "func bhvm_hf3_vm_op:sig;"
"}";

void bhvm_hf3_vm_op_ar2_hdiv_s_set_arg( bhvm_hf3_vm_op_ar2_hdiv_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar2_hdiv_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar2_hdiv_s_sig( o )[ 1 ] ) o->b = idx;
    if( id == bhvm_hf3_vm_op_ar2_hdiv_s_sig( o )[ 2 ] ) o->c = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar2_bmul_s )
"aware bhvm_hf3_vm_op_ar2"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "sz_t c = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
    "func bhvm_hf3_vm_op:sig;"
"}";

void bhvm_hf3_vm_op_ar2_bmul_s_set_arg( bhvm_hf3_vm_op_ar2_bmul_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar2_bmul_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar2_bmul_s_sig( o )[ 1 ] ) o->b = idx;
    if( id == bhvm_hf3_vm_op_ar2_bmul_s_sig( o )[ 2 ] ) o->c = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar2_mul_scl_s )
"aware bhvm_hf3_vm_op_ar2"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "sz_t c = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
    "func bhvm_hf3_vm_op:sig;"
"}";

void bhvm_hf3_vm_op_ar2_mul_scl_s_set_arg( bhvm_hf3_vm_op_ar2_mul_scl_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar2_mul_scl_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar2_mul_scl_s_sig( o )[ 1 ] ) o->b = idx;
    if( id == bhvm_hf3_vm_op_ar2_mul_scl_s_sig( o )[ 2 ] ) o->c = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar2_scl_mul_s )
"aware bhvm_hf3_vm_op_ar2"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "sz_t c = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
    "func bhvm_hf3_vm_op:sig;"
"}";

void bhvm_hf3_vm_op_ar2_scl_mul_s_set_arg( bhvm_hf3_vm_op_ar2_scl_mul_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar2_scl_mul_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar2_scl_mul_s_sig( o )[ 1 ] ) o->b = idx;
    if( id == bhvm_hf3_vm_op_ar2_scl_mul_s_sig( o )[ 2 ] ) o->c = idx;
}

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_vm_op_ar2_dp

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar2_dp_ca_exp_s )
"aware bhvm_hf3_vm_op_ar2"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "sz_t c = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:sig;"
    "func bhvm_hf3_vm_op:set_arg;"
"}";

void bhvm_hf3_vm_op_ar2_dp_ca_exp_s_set_arg( bhvm_hf3_vm_op_ar2_dp_ca_exp_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_exp_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_exp_s_sig( o )[ 1 ] ) o->b = idx;
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_exp_s_sig( o )[ 2 ] ) o->c = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar2_dp_ca_lgst_s )
"aware bhvm_hf3_vm_op_ar2"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "sz_t c = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:sig;"
    "func bhvm_hf3_vm_op:set_arg;"
"}";

void bhvm_hf3_vm_op_ar2_dp_ca_lgst_s_set_arg( bhvm_hf3_vm_op_ar2_dp_ca_lgst_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_lgst_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_lgst_s_sig( o )[ 1 ] ) o->b = idx;
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_lgst_s_sig( o )[ 2 ] ) o->c = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar2_dp_ca_lgst_hard_s )
"aware bhvm_hf3_vm_op_ar2"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "sz_t c = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:sig;"
    "func bhvm_hf3_vm_op:set_arg;"
"}";

void bhvm_hf3_vm_op_ar2_dp_ca_lgst_hard_s_set_arg( bhvm_hf3_vm_op_ar2_dp_ca_lgst_hard_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_lgst_hard_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_lgst_hard_s_sig( o )[ 1 ] ) o->b = idx;
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_lgst_hard_s_sig( o )[ 2 ] ) o->c = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar2_dp_ca_lgst_leaky_s )
"aware bhvm_hf3_vm_op_ar2"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "sz_t c = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:sig;"
    "func bhvm_hf3_vm_op:set_arg;"
"}";

void bhvm_hf3_vm_op_ar2_dp_ca_lgst_leaky_s_set_arg( bhvm_hf3_vm_op_ar2_dp_ca_lgst_leaky_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_lgst_leaky_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_lgst_leaky_s_sig( o )[ 1 ] ) o->b = idx;
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_lgst_leaky_s_sig( o )[ 2 ] ) o->c = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar2_dp_ca_tanh_s )
"aware bhvm_hf3_vm_op_ar2"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "sz_t c = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:sig;"
    "func bhvm_hf3_vm_op:set_arg;"
"}";

void bhvm_hf3_vm_op_ar2_dp_ca_tanh_s_set_arg( bhvm_hf3_vm_op_ar2_dp_ca_tanh_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_tanh_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_tanh_s_sig( o )[ 1 ] ) o->b = idx;
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_tanh_s_sig( o )[ 2 ] ) o->c = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar2_dp_ca_tanh_hard_s )
"aware bhvm_hf3_vm_op_ar2"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "sz_t c = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:sig;"
    "func bhvm_hf3_vm_op:set_arg;"
"}";

void bhvm_hf3_vm_op_ar2_dp_ca_tanh_hard_s_set_arg( bhvm_hf3_vm_op_ar2_dp_ca_tanh_hard_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_tanh_hard_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_tanh_hard_s_sig( o )[ 1 ] ) o->b = idx;
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_tanh_hard_s_sig( o )[ 2 ] ) o->c = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar2_dp_ca_tanh_leaky_s )
"aware bhvm_hf3_vm_op_ar2"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "sz_t c = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:sig;"
    "func bhvm_hf3_vm_op:set_arg;"
"}";

void bhvm_hf3_vm_op_ar2_dp_ca_tanh_leaky_s_set_arg( bhvm_hf3_vm_op_ar2_dp_ca_tanh_leaky_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_tanh_leaky_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_tanh_leaky_s_sig( o )[ 1 ] ) o->b = idx;
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_tanh_leaky_s_sig( o )[ 2 ] ) o->c = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar2_dp_ca_softplus_s )
"aware bhvm_hf3_vm_op_ar2"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "sz_t c = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:sig;"
    "func bhvm_hf3_vm_op:set_arg;"
"}";

void bhvm_hf3_vm_op_ar2_dp_ca_softplus_s_set_arg( bhvm_hf3_vm_op_ar2_dp_ca_softplus_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_softplus_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_softplus_s_sig( o )[ 1 ] ) o->b = idx;
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_softplus_s_sig( o )[ 2 ] ) o->c = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar2_dp_ca_relu_s )
"aware bhvm_hf3_vm_op_ar2"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "sz_t c = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:sig;"
    "func bhvm_hf3_vm_op:set_arg;"
"}";

void bhvm_hf3_vm_op_ar2_dp_ca_relu_s_set_arg( bhvm_hf3_vm_op_ar2_dp_ca_relu_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_relu_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_relu_s_sig( o )[ 1 ] ) o->b = idx;
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_relu_s_sig( o )[ 2 ] ) o->c = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar2_dp_ca_relu_leaky_s )
"aware bhvm_hf3_vm_op_ar2"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "sz_t c = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:sig;"
    "func bhvm_hf3_vm_op:set_arg;"
"}";

void bhvm_hf3_vm_op_ar2_dp_ca_relu_leaky_s_set_arg( bhvm_hf3_vm_op_ar2_dp_ca_relu_leaky_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_relu_leaky_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_relu_leaky_s_sig( o )[ 1 ] ) o->b = idx;
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_relu_leaky_s_sig( o )[ 2 ] ) o->c = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar2_dp_ca_hmul_s )
"aware bhvm_hf3_vm_op_ar2"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "sz_t c = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm_op:sig;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
"}";

void bhvm_hf3_vm_op_ar2_dp_ca_hmul_s_set_arg( bhvm_hf3_vm_op_ar2_dp_ca_hmul_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_hmul_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_hmul_s_sig( o )[ 1 ] ) o->b = idx;
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_hmul_s_sig( o )[ 2 ] ) o->c = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar2_dp_cb_hmul_s )
"aware bhvm_hf3_vm_op_ar2"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "sz_t c = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm_op:sig;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
"}";

void bhvm_hf3_vm_op_ar2_dp_cb_hmul_s_set_arg( bhvm_hf3_vm_op_ar2_dp_cb_hmul_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar2_dp_cb_hmul_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar2_dp_cb_hmul_s_sig( o )[ 1 ] ) o->b = idx;
    if( id == bhvm_hf3_vm_op_ar2_dp_cb_hmul_s_sig( o )[ 2 ] ) o->c = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar2_dp_ca_bmul_s )
"aware bhvm_hf3_vm_op_ar2"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "sz_t c = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm_op:sig;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
"}";

void bhvm_hf3_vm_op_ar2_dp_ca_bmul_s_set_arg( bhvm_hf3_vm_op_ar2_dp_ca_bmul_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_bmul_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_bmul_s_sig( o )[ 1 ] ) o->b = idx;
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_bmul_s_sig( o )[ 2 ] ) o->c = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar2_dp_cb_bmul_s )
"aware bhvm_hf3_vm_op_ar2"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "sz_t c = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm_op:sig;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
"}";

void bhvm_hf3_vm_op_ar2_dp_cb_bmul_s_set_arg( bhvm_hf3_vm_op_ar2_dp_cb_bmul_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar2_dp_cb_bmul_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar2_dp_cb_bmul_s_sig( o )[ 1 ] ) o->b = idx;
    if( id == bhvm_hf3_vm_op_ar2_dp_cb_bmul_s_sig( o )[ 2 ] ) o->c = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar2_dp_ca_mul_scl_s )
"aware bhvm_hf3_vm_op_ar2"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "sz_t c = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm_op:sig;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
"}";

void bhvm_hf3_vm_op_ar2_dp_ca_mul_scl_s_set_arg( bhvm_hf3_vm_op_ar2_dp_ca_mul_scl_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_mul_scl_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_mul_scl_s_sig( o )[ 1 ] ) o->b = idx;
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_mul_scl_s_sig( o )[ 2 ] ) o->c = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar2_dp_cb_mul_scl_s )
"aware bhvm_hf3_vm_op_ar2"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "sz_t c = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm_op:sig;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
"}";

void bhvm_hf3_vm_op_ar2_dp_cb_mul_scl_s_set_arg( bhvm_hf3_vm_op_ar2_dp_cb_mul_scl_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar2_dp_cb_mul_scl_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar2_dp_cb_mul_scl_s_sig( o )[ 1 ] ) o->b = idx;
    if( id == bhvm_hf3_vm_op_ar2_dp_cb_mul_scl_s_sig( o )[ 2 ] ) o->c = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar2_dp_ca_scl_mul_s )
"aware bhvm_hf3_vm_op_ar2"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "sz_t c = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm_op:sig;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
"}";

void bhvm_hf3_vm_op_ar2_dp_ca_scl_mul_s_set_arg( bhvm_hf3_vm_op_ar2_dp_ca_scl_mul_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_scl_mul_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_scl_mul_s_sig( o )[ 1 ] ) o->b = idx;
    if( id == bhvm_hf3_vm_op_ar2_dp_ca_scl_mul_s_sig( o )[ 2 ] ) o->c = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar2_dp_cb_scl_mul_s )
"aware bhvm_hf3_vm_op_ar2"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "sz_t c = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm_op:sig;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
"}";

void bhvm_hf3_vm_op_ar2_dp_cb_scl_mul_s_set_arg( bhvm_hf3_vm_op_ar2_dp_cb_scl_mul_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar2_dp_cb_scl_mul_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar2_dp_cb_scl_mul_s_sig( o )[ 1 ] ) o->b = idx;
    if( id == bhvm_hf3_vm_op_ar2_dp_cb_scl_mul_s_sig( o )[ 2 ] ) o->c = idx;
}

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_vm_op_ar3

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_hf3_vm_op_ar3_dp

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar3_dp_ca_hdiv_s )
"aware bhvm_hf3_vm_op_ar3"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "sz_t c = -1;"
    "sz_t d = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm_op:sig;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
"}";

void bhvm_hf3_vm_op_ar3_dp_ca_hdiv_s_set_arg( bhvm_hf3_vm_op_ar3_dp_ca_hdiv_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar3_dp_ca_hdiv_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar3_dp_ca_hdiv_s_sig( o )[ 1 ] ) o->b = idx;
    if( id == bhvm_hf3_vm_op_ar3_dp_ca_hdiv_s_sig( o )[ 2 ] ) o->c = idx;
    if( id == bhvm_hf3_vm_op_ar3_dp_ca_hdiv_s_sig( o )[ 3 ] ) o->d = idx;
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_hf3_vm_op_ar3_dp_cb_hdiv_s )
"aware bhvm_hf3_vm_op_ar3"
"{"
    "sz_t a = -1;"
    "sz_t b = -1;"
    "sz_t c = -1;"
    "sz_t d = -1;"
    "func bhvm_hf3_vm_op:get_arity;"
    "func bhvm_hf3_vm_op:set_indices;"
    "func bhvm_hf3_vm_op:get_indices;"
    "func ^:csetup;"
    "func bhvm_hf3_vm_op:sig;"
    "func bhvm_hf3_vm:run;"
    "func bhvm_hf3_vm_op:set_arg;"
"}";

void bhvm_hf3_vm_op_ar3_dp_cb_hdiv_s_set_arg( bhvm_hf3_vm_op_ar3_dp_cb_hdiv_s* o, char id, sz_t idx )
{
    if( id == bhvm_hf3_vm_op_ar3_dp_cb_hdiv_s_sig( o )[ 0 ] ) o->a = idx;
    if( id == bhvm_hf3_vm_op_ar3_dp_cb_hdiv_s_sig( o )[ 1 ] ) o->b = idx;
    if( id == bhvm_hf3_vm_op_ar3_dp_cb_hdiv_s_sig( o )[ 2 ] ) o->c = idx;
    if( id == bhvm_hf3_vm_op_ar3_dp_cb_hdiv_s_sig( o )[ 3 ] ) o->d = idx;
}

/**********************************************************************************************************************/

vd_t bhvm_planted_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bhvm_planted" ) ) )
    {
        case TYPEOF_init1:
        {
            // Comment or remove line below to rebuild this target.
            bcore_const_x_set_d( typeof( "bhvm_planted_hash" ), sr_tp( 653308204 ) );

            // --------------------------------------------------------------------
            // source: bhvm_hf3.h

            // group: bhvm_hf3
            BCORE_REGISTER_OBJECT( bhvm_hf3_s );
            BCORE_REGISTER_OBJECT( bhvm_hf3_adl_s );
            BCORE_REGISTER_TRAIT( bhvm_hf3, bcore_inst );

            // --------------------------------------------------------------------
            // source: bhvm_hf3_op.h

            // group: bhvm_hf3_op
            BCORE_REGISTER_FEATURE( bhvm_hf3_op_get_arity );
            BCORE_REGISTER_FEATURE( bhvm_hf3_op_aof );
            BCORE_REGISTER_FEATURE( bhvm_hf3_op_osig );
            BCORE_REGISTER_SPECT( bhvm_hf3_op );

            // group: bhvm_hf3_op_ar0
            BCORE_REGISTER_FEATURE( bhvm_hf3_op_ar0_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar0_f, bhvm_hf3_op_ar0_zro_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar0_zro_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar0_of, bhvm_hf3_op_ar0_zro_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar0_zro_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar0_zro_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar0_of, bhvm_hf3_op_ar0_literal_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar0_literal_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar0_f, bhvm_hf3_op_ar0_literal_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar0_literal_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar0_literal_s );
            BCORE_REGISTER_SPECT( bhvm_hf3_op_ar0 );

            // group: bhvm_hf3_op_ar0_dp
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar0_f, bhvm_hf3_op_ar0_dp_ca_floor_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_sig, bhvm_hf3_op_ar0_dp_ca_floor_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar0_dp_ca_floor_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar0_of, bhvm_hf3_op_ar0_dp_ca_floor_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar0_dp_ca_floor_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar0_dp_ca_floor_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar0_f, bhvm_hf3_op_ar0_dp_ca_ceil_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_sig, bhvm_hf3_op_ar0_dp_ca_ceil_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar0_dp_ca_ceil_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar0_of, bhvm_hf3_op_ar0_dp_ca_ceil_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar0_dp_ca_ceil_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar0_dp_ca_ceil_s );
            BCORE_REGISTER_TRAIT( bhvm_hf3_op_ar0_dp, bhvm_hf3_op_ar0 );

            // group: bhvm_hf3_op_ar1
            BCORE_REGISTER_FEATURE( bhvm_hf3_op_ar1_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_f, bhvm_hf3_op_ar1_cpy_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar1_cpy_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_of, bhvm_hf3_op_ar1_cpy_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar1_cpy_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar1_cpy_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_f, bhvm_hf3_op_ar1_neg_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar1_neg_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_of, bhvm_hf3_op_ar1_neg_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar1_neg_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar1_neg_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_f, bhvm_hf3_op_ar1_floor_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar1_floor_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_of, bhvm_hf3_op_ar1_floor_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar1_floor_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar1_floor_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_f, bhvm_hf3_op_ar1_ceil_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar1_ceil_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_of, bhvm_hf3_op_ar1_ceil_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar1_ceil_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar1_ceil_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_f, bhvm_hf3_op_ar1_exp_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar1_exp_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_of, bhvm_hf3_op_ar1_exp_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar1_exp_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar1_exp_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_f, bhvm_hf3_op_ar1_lgst_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar1_lgst_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_of, bhvm_hf3_op_ar1_lgst_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar1_lgst_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar1_lgst_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_f, bhvm_hf3_op_ar1_lgst_hard_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar1_lgst_hard_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_of, bhvm_hf3_op_ar1_lgst_hard_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar1_lgst_hard_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar1_lgst_hard_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_f, bhvm_hf3_op_ar1_lgst_leaky_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar1_lgst_leaky_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_of, bhvm_hf3_op_ar1_lgst_leaky_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar1_lgst_leaky_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar1_lgst_leaky_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_f, bhvm_hf3_op_ar1_tanh_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar1_tanh_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_of, bhvm_hf3_op_ar1_tanh_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar1_tanh_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar1_tanh_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_f, bhvm_hf3_op_ar1_tanh_hard_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar1_tanh_hard_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_of, bhvm_hf3_op_ar1_tanh_hard_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar1_tanh_hard_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar1_tanh_hard_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_f, bhvm_hf3_op_ar1_tanh_leaky_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar1_tanh_leaky_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_of, bhvm_hf3_op_ar1_tanh_leaky_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar1_tanh_leaky_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar1_tanh_leaky_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_f, bhvm_hf3_op_ar1_softplus_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar1_softplus_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_of, bhvm_hf3_op_ar1_softplus_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar1_softplus_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar1_softplus_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_f, bhvm_hf3_op_ar1_relu_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar1_relu_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_of, bhvm_hf3_op_ar1_relu_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar1_relu_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar1_relu_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_f, bhvm_hf3_op_ar1_relu_leaky_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar1_relu_leaky_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_of, bhvm_hf3_op_ar1_relu_leaky_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar1_relu_leaky_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar1_relu_leaky_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_f, bhvm_hf3_op_ar1_cast_htp_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar1_cast_htp_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_of, bhvm_hf3_op_ar1_cast_htp_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar1_cast_htp_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar1_cast_htp_s );
            BCORE_REGISTER_SPECT( bhvm_hf3_op_ar1 );

            // group: bhvm_hf3_op_ar1_dp
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_f, bhvm_hf3_op_ar1_dp_ca_cpy_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_sig, bhvm_hf3_op_ar1_dp_ca_cpy_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar1_dp_ca_cpy_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_of, bhvm_hf3_op_ar1_dp_ca_cpy_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar1_dp_ca_cpy_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar1_dp_ca_cpy_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_f, bhvm_hf3_op_ar1_dp_ca_neg_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_sig, bhvm_hf3_op_ar1_dp_ca_neg_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar1_dp_ca_neg_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_of, bhvm_hf3_op_ar1_dp_ca_neg_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar1_dp_ca_neg_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar1_dp_ca_neg_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_f, bhvm_hf3_op_ar1_dp_ca_add_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_sig, bhvm_hf3_op_ar1_dp_ca_add_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar1_dp_ca_add_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_of, bhvm_hf3_op_ar1_dp_ca_add_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar1_dp_ca_add_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar1_dp_ca_add_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_f, bhvm_hf3_op_ar1_dp_cb_add_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_sig, bhvm_hf3_op_ar1_dp_cb_add_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar1_dp_cb_add_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_of, bhvm_hf3_op_ar1_dp_cb_add_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar1_dp_cb_add_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar1_dp_cb_add_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_f, bhvm_hf3_op_ar1_dp_ca_sub_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_sig, bhvm_hf3_op_ar1_dp_ca_sub_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar1_dp_ca_sub_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_of, bhvm_hf3_op_ar1_dp_ca_sub_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar1_dp_ca_sub_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar1_dp_ca_sub_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_f, bhvm_hf3_op_ar1_dp_cb_sub_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_sig, bhvm_hf3_op_ar1_dp_cb_sub_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar1_dp_cb_sub_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_of, bhvm_hf3_op_ar1_dp_cb_sub_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar1_dp_cb_sub_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar1_dp_cb_sub_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_sig, bhvm_hf3_op_ar1_dp_ca_cast_htp_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_f, bhvm_hf3_op_ar1_dp_ca_cast_htp_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar1_dp_ca_cast_htp_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar1_of, bhvm_hf3_op_ar1_dp_ca_cast_htp_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar1_dp_ca_cast_htp_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar1_dp_ca_cast_htp_s );
            BCORE_REGISTER_TRAIT( bhvm_hf3_op_ar1_dp, bhvm_hf3_op_ar1 );

            // group: bhvm_hf3_op_ar2
            BCORE_REGISTER_FEATURE( bhvm_hf3_op_ar2_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_f, bhvm_hf3_op_ar2_add_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar2_add_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_of, bhvm_hf3_op_ar2_add_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar2_add_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar2_add_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_f, bhvm_hf3_op_ar2_sub_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar2_sub_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_of, bhvm_hf3_op_ar2_sub_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar2_sub_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar2_sub_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_f, bhvm_hf3_op_ar2_hmul_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar2_hmul_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_of, bhvm_hf3_op_ar2_hmul_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar2_hmul_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar2_hmul_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_f, bhvm_hf3_op_ar2_hdiv_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar2_hdiv_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_of, bhvm_hf3_op_ar2_hdiv_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar2_hdiv_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar2_hdiv_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_f, bhvm_hf3_op_ar2_bmul_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar2_bmul_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_of, bhvm_hf3_op_ar2_bmul_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar2_bmul_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar2_bmul_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_f, bhvm_hf3_op_ar2_mul_scl_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar2_mul_scl_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_of, bhvm_hf3_op_ar2_mul_scl_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar2_mul_scl_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar2_mul_scl_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_f, bhvm_hf3_op_ar2_scl_mul_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar2_scl_mul_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_of, bhvm_hf3_op_ar2_scl_mul_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar2_scl_mul_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar2_scl_mul_s );
            BCORE_REGISTER_SPECT( bhvm_hf3_op_ar2 );

            // group: bhvm_hf3_op_ar2_dp
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_f, bhvm_hf3_op_ar2_dp_ca_exp_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_sig, bhvm_hf3_op_ar2_dp_ca_exp_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar2_dp_ca_exp_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_of, bhvm_hf3_op_ar2_dp_ca_exp_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar2_dp_ca_exp_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar2_dp_ca_exp_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_f, bhvm_hf3_op_ar2_dp_ca_lgst_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_sig, bhvm_hf3_op_ar2_dp_ca_lgst_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar2_dp_ca_lgst_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_of, bhvm_hf3_op_ar2_dp_ca_lgst_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar2_dp_ca_lgst_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar2_dp_ca_lgst_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_f, bhvm_hf3_op_ar2_dp_ca_lgst_hard_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_sig, bhvm_hf3_op_ar2_dp_ca_lgst_hard_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar2_dp_ca_lgst_hard_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_of, bhvm_hf3_op_ar2_dp_ca_lgst_hard_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar2_dp_ca_lgst_hard_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar2_dp_ca_lgst_hard_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_f, bhvm_hf3_op_ar2_dp_ca_lgst_leaky_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_sig, bhvm_hf3_op_ar2_dp_ca_lgst_leaky_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar2_dp_ca_lgst_leaky_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_of, bhvm_hf3_op_ar2_dp_ca_lgst_leaky_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar2_dp_ca_lgst_leaky_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar2_dp_ca_lgst_leaky_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_f, bhvm_hf3_op_ar2_dp_ca_tanh_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_sig, bhvm_hf3_op_ar2_dp_ca_tanh_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar2_dp_ca_tanh_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_of, bhvm_hf3_op_ar2_dp_ca_tanh_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar2_dp_ca_tanh_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar2_dp_ca_tanh_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_f, bhvm_hf3_op_ar2_dp_ca_tanh_hard_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_sig, bhvm_hf3_op_ar2_dp_ca_tanh_hard_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar2_dp_ca_tanh_hard_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_of, bhvm_hf3_op_ar2_dp_ca_tanh_hard_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar2_dp_ca_tanh_hard_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar2_dp_ca_tanh_hard_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_f, bhvm_hf3_op_ar2_dp_ca_tanh_leaky_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_sig, bhvm_hf3_op_ar2_dp_ca_tanh_leaky_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar2_dp_ca_tanh_leaky_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_of, bhvm_hf3_op_ar2_dp_ca_tanh_leaky_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar2_dp_ca_tanh_leaky_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar2_dp_ca_tanh_leaky_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_f, bhvm_hf3_op_ar2_dp_ca_softplus_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_sig, bhvm_hf3_op_ar2_dp_ca_softplus_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar2_dp_ca_softplus_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_of, bhvm_hf3_op_ar2_dp_ca_softplus_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar2_dp_ca_softplus_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar2_dp_ca_softplus_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_f, bhvm_hf3_op_ar2_dp_ca_relu_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_sig, bhvm_hf3_op_ar2_dp_ca_relu_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar2_dp_ca_relu_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_of, bhvm_hf3_op_ar2_dp_ca_relu_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar2_dp_ca_relu_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar2_dp_ca_relu_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_f, bhvm_hf3_op_ar2_dp_ca_relu_leaky_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_sig, bhvm_hf3_op_ar2_dp_ca_relu_leaky_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar2_dp_ca_relu_leaky_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_of, bhvm_hf3_op_ar2_dp_ca_relu_leaky_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar2_dp_ca_relu_leaky_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar2_dp_ca_relu_leaky_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_sig, bhvm_hf3_op_ar2_dp_ca_hmul_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_f, bhvm_hf3_op_ar2_dp_ca_hmul_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar2_dp_ca_hmul_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_of, bhvm_hf3_op_ar2_dp_ca_hmul_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar2_dp_ca_hmul_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar2_dp_ca_hmul_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_sig, bhvm_hf3_op_ar2_dp_cb_hmul_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_f, bhvm_hf3_op_ar2_dp_cb_hmul_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar2_dp_cb_hmul_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_of, bhvm_hf3_op_ar2_dp_cb_hmul_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar2_dp_cb_hmul_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar2_dp_cb_hmul_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_sig, bhvm_hf3_op_ar2_dp_ca_bmul_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_f, bhvm_hf3_op_ar2_dp_ca_bmul_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar2_dp_ca_bmul_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_of, bhvm_hf3_op_ar2_dp_ca_bmul_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar2_dp_ca_bmul_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar2_dp_ca_bmul_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_sig, bhvm_hf3_op_ar2_dp_cb_bmul_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_f, bhvm_hf3_op_ar2_dp_cb_bmul_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar2_dp_cb_bmul_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_of, bhvm_hf3_op_ar2_dp_cb_bmul_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar2_dp_cb_bmul_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar2_dp_cb_bmul_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_sig, bhvm_hf3_op_ar2_dp_ca_mul_scl_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_f, bhvm_hf3_op_ar2_dp_ca_mul_scl_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar2_dp_ca_mul_scl_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_of, bhvm_hf3_op_ar2_dp_ca_mul_scl_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar2_dp_ca_mul_scl_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar2_dp_ca_mul_scl_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_sig, bhvm_hf3_op_ar2_dp_cb_mul_scl_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_f, bhvm_hf3_op_ar2_dp_cb_mul_scl_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar2_dp_cb_mul_scl_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_of, bhvm_hf3_op_ar2_dp_cb_mul_scl_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar2_dp_cb_mul_scl_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar2_dp_cb_mul_scl_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_sig, bhvm_hf3_op_ar2_dp_ca_scl_mul_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_f, bhvm_hf3_op_ar2_dp_ca_scl_mul_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar2_dp_ca_scl_mul_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_of, bhvm_hf3_op_ar2_dp_ca_scl_mul_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar2_dp_ca_scl_mul_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar2_dp_ca_scl_mul_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_sig, bhvm_hf3_op_ar2_dp_cb_scl_mul_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_f, bhvm_hf3_op_ar2_dp_cb_scl_mul_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar2_dp_cb_scl_mul_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar2_of, bhvm_hf3_op_ar2_dp_cb_scl_mul_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar2_dp_cb_scl_mul_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar2_dp_cb_scl_mul_s );
            BCORE_REGISTER_TRAIT( bhvm_hf3_op_ar2_dp, bhvm_hf3_op_ar2 );

            // group: bhvm_hf3_op_ar3
            BCORE_REGISTER_FEATURE( bhvm_hf3_op_ar3_of );
            BCORE_REGISTER_SPECT( bhvm_hf3_op_ar3 );

            // group: bhvm_hf3_op_ar3_dp
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar3_f, bhvm_hf3_op_ar3_dp_ca_hdiv_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_sig, bhvm_hf3_op_ar3_dp_ca_hdiv_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar3_dp_ca_hdiv_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar3_of, bhvm_hf3_op_ar3_dp_ca_hdiv_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar3_dp_ca_hdiv_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar3_dp_ca_hdiv_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar3_f, bhvm_hf3_op_ar3_dp_cb_hdiv_s_f );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_sig, bhvm_hf3_op_ar3_dp_cb_hdiv_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_get_arity, bhvm_hf3_op_ar3_dp_cb_hdiv_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_ar3_of, bhvm_hf3_op_ar3_dp_cb_hdiv_s_of );
            BCORE_REGISTER_FFUNC( bhvm_hf3_op_aof, bhvm_hf3_op_ar3_dp_cb_hdiv_s_aof );
            BCORE_REGISTER_OBJECT( bhvm_hf3_op_ar3_dp_cb_hdiv_s );
            BCORE_REGISTER_TRAIT( bhvm_hf3_op_ar3_dp, bhvm_hf3_op_ar3 );

            // --------------------------------------------------------------------
            // source: bhvm_hf3_vm.h

            // group: bhvm_hf3_vm
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_holor_s );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_arr_holor_s );
            BCORE_REGISTER_FEATURE( bhvm_hf3_vm_run );
            BCORE_REGISTER_FFUNC( bcore_inst_call_copy_x, bhvm_hf3_vm_prop_s_copy_x );
            BCORE_REGISTER_FFUNC( bcore_via_call_mutated, bhvm_hf3_vm_prop_s_mutated );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_prop_s_run );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_prop_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_proc_s_run );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_proc_s );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_library_s );
            BCORE_REGISTER_FEATURE( bhvm_hf3_vm_mrun );
            BCORE_REGISTER_FFUNC( bcore_inst_call_copy_x, bhvm_hf3_vm_mop_s_copy_x );
            BCORE_REGISTER_FFUNC( bcore_via_call_mutated, bhvm_hf3_vm_mop_s_mutated );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_mrun, bhvm_hf3_vm_mop_s_mrun );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_mop_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_mrun, bhvm_hf3_vm_mcode_s_mrun );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_push_d, bhvm_hf3_vm_mcode_s_op_push_d );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_push_c, bhvm_hf3_vm_mcode_s_op_push_c );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_mcode_s );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_arr_mcode_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_clear, bhvm_hf3_vm_lib_mcode_s_clear );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_mcode_exists, bhvm_hf3_vm_lib_mcode_s_mcode_exists );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_mcode_get, bhvm_hf3_vm_lib_mcode_s_mcode_get );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_mcode_get_or_new, bhvm_hf3_vm_lib_mcode_s_mcode_get_or_new );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_mcode_reset, bhvm_hf3_vm_lib_mcode_s_mcode_reset );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_mcode_op_push_c, bhvm_hf3_vm_lib_mcode_s_mcode_op_push_c );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_mcode_op_push_d, bhvm_hf3_vm_lib_mcode_s_mcode_op_push_d );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_mcode_push, bhvm_hf3_vm_lib_mcode_s_mcode_push );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_mcode_push_reverse, bhvm_hf3_vm_lib_mcode_s_mcode_push_reverse );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_mcode_remove, bhvm_hf3_vm_lib_mcode_s_mcode_remove );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_mcode_mrun, bhvm_hf3_vm_lib_mcode_s_mcode_mrun );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_lib_mcode_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_setup, bhvm_hf3_vm_frame_s_setup );
            BCORE_REGISTER_FFUNC( bcore_via_call_mutated, bhvm_hf3_vm_frame_s_mutated );
            BCORE_REGISTER_FFUNC( bcore_via_call_shelve, bhvm_hf3_vm_frame_s_shelve );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_mcode_exists, bhvm_hf3_vm_frame_s_mcode_exists );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_mcode_get, bhvm_hf3_vm_frame_s_mcode_get );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_mcode_get_or_new, bhvm_hf3_vm_frame_s_mcode_get_or_new );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_mcode_reset, bhvm_hf3_vm_frame_s_mcode_reset );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_mcode_op_push_d, bhvm_hf3_vm_frame_s_mcode_op_push_d );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_mcode_op_push_c, bhvm_hf3_vm_frame_s_mcode_op_push_c );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_mcode_push, bhvm_hf3_vm_frame_s_mcode_push );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_mcode_push_reverse, bhvm_hf3_vm_frame_s_mcode_push_reverse );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_mcode_remove, bhvm_hf3_vm_frame_s_mcode_remove );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_mcode_run, bhvm_hf3_vm_frame_s_mcode_run );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_frame_s );
            BCORE_REGISTER_SPECT( bhvm_hf3_vm );

            // group: bhvm_hf3_vm_op
            BCORE_REGISTER_FEATURE( bhvm_hf3_vm_op_get_arity );
            BCORE_REGISTER_FEATURE( bhvm_hf3_vm_op_set_indices );
            BCORE_REGISTER_FEATURE( bhvm_hf3_vm_op_get_indices );
            BCORE_REGISTER_FEATURE( bhvm_hf3_vm_op_sig );
            BCORE_REGISTER_FEATURE( bhvm_hf3_vm_op_set_arg );
            BCORE_REGISTER_SPECT( bhvm_hf3_vm_op );

            // group: bhvm_hf3_vm_op_ar0
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar0_nul_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar0_nul_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar0_nul_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar0_csetup, bhvm_hf3_vm_op_ar0_nul_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar0_nul_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar0_nul_s_set_arg );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar0_nul_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar0_nul_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar0_zro_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar0_zro_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar0_zro_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar0_csetup, bhvm_hf3_vm_op_ar0_zro_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar0_zro_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar0_zro_s_set_arg );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar0_zro_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar0_zro_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar0_zro_nc_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar0_zro_nc_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar0_zro_nc_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar0_csetup, bhvm_hf3_vm_op_ar0_zro_nc_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar0_zro_nc_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar0_zro_nc_s_set_arg );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar0_zro_nc_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar0_zro_nc_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar0_determine_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar0_determine_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar0_determine_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar0_csetup, bhvm_hf3_vm_op_ar0_determine_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar0_determine_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar0_determine_s_set_arg );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar0_determine_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar0_determine_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar0_vacate_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar0_vacate_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar0_vacate_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar0_csetup, bhvm_hf3_vm_op_ar0_vacate_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar0_vacate_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar0_vacate_s_set_arg );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar0_vacate_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar0_vacate_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar0_clear_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar0_clear_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar0_clear_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar0_csetup, bhvm_hf3_vm_op_ar0_clear_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar0_clear_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar0_clear_s_set_arg );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar0_clear_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar0_clear_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar0_randomize_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar0_randomize_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar0_randomize_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar0_csetup, bhvm_hf3_vm_op_ar0_randomize_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar0_csetup_randomize, bhvm_hf3_vm_op_ar0_randomize_s_csetup_randomize );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar0_randomize_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar0_randomize_s_set_arg );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar0_randomize_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar0_randomize_s );
            BCORE_REGISTER_TRAIT( bhvm_hf3_vm_op_ar0, bhvm_hf3_vm_op );

            // group: bhvm_hf3_vm_op_ar0_dp
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar0_dp_ca_floor_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar0_dp_ca_floor_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar0_dp_ca_floor_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar0_csetup, bhvm_hf3_vm_op_ar0_dp_ca_floor_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar0_dp_ca_floor_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar0_dp_ca_floor_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar0_dp_ca_floor_s_set_arg );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar0_dp_ca_floor_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar0_dp_ca_ceil_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar0_dp_ca_ceil_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar0_dp_ca_ceil_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar0_csetup, bhvm_hf3_vm_op_ar0_dp_ca_ceil_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar0_dp_ca_ceil_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar0_dp_ca_ceil_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar0_dp_ca_ceil_s_set_arg );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar0_dp_ca_ceil_s );
            BCORE_REGISTER_TRAIT( bhvm_hf3_vm_op_ar0_dp, bhvm_hf3_vm_op_ar0 );

            // group: bhvm_hf3_vm_op_ar1
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar1_unary_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar1_unary_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar1_unary_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar1_csetup, bhvm_hf3_vm_op_ar1_unary_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar1_unary_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar1_unary_s_set_arg );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar1_unary_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar1_unary_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar1_cpy_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar1_cpy_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar1_cpy_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar1_csetup, bhvm_hf3_vm_op_ar1_cpy_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar1_cpy_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar1_cpy_s_set_arg );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar1_cpy_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar1_cpy_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar1_neg_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar1_neg_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar1_neg_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar1_csetup, bhvm_hf3_vm_op_ar1_neg_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar1_neg_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar1_neg_s_set_arg );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar1_neg_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar1_neg_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar1_floor_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar1_floor_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar1_floor_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar1_csetup, bhvm_hf3_vm_op_ar1_floor_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar1_floor_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar1_floor_s_set_arg );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar1_floor_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar1_floor_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar1_ceil_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar1_ceil_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar1_ceil_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar1_csetup, bhvm_hf3_vm_op_ar1_ceil_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar1_ceil_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar1_ceil_s_set_arg );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar1_ceil_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar1_ceil_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar1_exp_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar1_exp_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar1_exp_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar1_csetup, bhvm_hf3_vm_op_ar1_exp_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar1_exp_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar1_exp_s_set_arg );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar1_exp_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar1_exp_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar1_cpy_ay_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar1_cpy_ay_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar1_cpy_ay_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar1_csetup, bhvm_hf3_vm_op_ar1_cpy_ay_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar1_cpy_ay_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar1_cpy_ay_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar1_cpy_ay_s_set_arg );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar1_cpy_ay_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar1_cpy_by_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar1_cpy_by_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar1_cpy_by_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar1_csetup, bhvm_hf3_vm_op_ar1_cpy_by_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar1_cpy_by_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar1_cpy_by_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar1_cpy_by_s_set_arg );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar1_cpy_by_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar1_lgst_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar1_lgst_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar1_lgst_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar1_csetup, bhvm_hf3_vm_op_ar1_lgst_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar1_lgst_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar1_lgst_s_set_arg );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar1_lgst_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar1_lgst_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar1_lgst_hard_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar1_lgst_hard_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar1_lgst_hard_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar1_csetup, bhvm_hf3_vm_op_ar1_lgst_hard_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar1_lgst_hard_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar1_lgst_hard_s_set_arg );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar1_lgst_hard_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar1_lgst_hard_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar1_lgst_leaky_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar1_lgst_leaky_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar1_lgst_leaky_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar1_csetup, bhvm_hf3_vm_op_ar1_lgst_leaky_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar1_lgst_leaky_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar1_lgst_leaky_s_set_arg );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar1_lgst_leaky_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar1_lgst_leaky_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar1_tanh_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar1_tanh_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar1_tanh_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar1_csetup, bhvm_hf3_vm_op_ar1_tanh_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar1_tanh_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar1_tanh_s_set_arg );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar1_tanh_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar1_tanh_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar1_tanh_hard_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar1_tanh_hard_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar1_tanh_hard_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar1_csetup, bhvm_hf3_vm_op_ar1_tanh_hard_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar1_tanh_hard_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar1_tanh_hard_s_set_arg );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar1_tanh_hard_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar1_tanh_hard_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar1_tanh_leaky_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar1_tanh_leaky_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar1_tanh_leaky_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar1_csetup, bhvm_hf3_vm_op_ar1_tanh_leaky_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar1_tanh_leaky_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar1_tanh_leaky_s_set_arg );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar1_tanh_leaky_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar1_tanh_leaky_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar1_softplus_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar1_softplus_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar1_softplus_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar1_csetup, bhvm_hf3_vm_op_ar1_softplus_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar1_softplus_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar1_softplus_s_set_arg );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar1_softplus_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar1_softplus_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar1_relu_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar1_relu_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar1_relu_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar1_csetup, bhvm_hf3_vm_op_ar1_relu_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar1_relu_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar1_relu_s_set_arg );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar1_relu_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar1_relu_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar1_relu_leaky_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar1_relu_leaky_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar1_relu_leaky_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar1_csetup, bhvm_hf3_vm_op_ar1_relu_leaky_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar1_relu_leaky_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar1_relu_leaky_s_set_arg );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar1_relu_leaky_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar1_relu_leaky_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar1_cast_htp_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar1_cast_htp_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar1_cast_htp_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar1_csetup, bhvm_hf3_vm_op_ar1_cast_htp_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar1_cast_htp_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar1_cast_htp_s_set_arg );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar1_cast_htp_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar1_cast_htp_s );
            BCORE_REGISTER_TRAIT( bhvm_hf3_vm_op_ar1, bhvm_hf3_vm_op );

            // group: bhvm_hf3_vm_op_ar1_dp
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar1_dp_ca_cpy_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar1_dp_ca_cpy_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar1_dp_ca_cpy_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar1_csetup, bhvm_hf3_vm_op_ar1_dp_ca_cpy_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar1_dp_ca_cpy_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar1_dp_ca_cpy_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar1_dp_ca_cpy_s_set_arg );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar1_dp_ca_cpy_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar1_dp_ca_neg_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar1_dp_ca_neg_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar1_dp_ca_neg_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar1_csetup, bhvm_hf3_vm_op_ar1_dp_ca_neg_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar1_dp_ca_neg_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar1_dp_ca_neg_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar1_dp_ca_neg_s_set_arg );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar1_dp_ca_neg_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar1_dp_ca_add_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar1_dp_ca_add_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar1_dp_ca_add_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar1_csetup, bhvm_hf3_vm_op_ar1_dp_ca_add_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar1_dp_ca_add_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar1_dp_ca_add_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar1_dp_ca_add_s_set_arg );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar1_dp_ca_add_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar1_dp_cb_add_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar1_dp_cb_add_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar1_dp_cb_add_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar1_csetup, bhvm_hf3_vm_op_ar1_dp_cb_add_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar1_dp_cb_add_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar1_dp_cb_add_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar1_dp_cb_add_s_set_arg );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar1_dp_cb_add_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar1_dp_ca_sub_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar1_dp_ca_sub_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar1_dp_ca_sub_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar1_csetup, bhvm_hf3_vm_op_ar1_dp_ca_sub_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar1_dp_ca_sub_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar1_dp_ca_sub_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar1_dp_ca_sub_s_set_arg );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar1_dp_ca_sub_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar1_dp_cb_sub_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar1_dp_cb_sub_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar1_dp_cb_sub_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar1_csetup, bhvm_hf3_vm_op_ar1_dp_cb_sub_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar1_dp_cb_sub_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar1_dp_cb_sub_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar1_dp_cb_sub_s_set_arg );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar1_dp_cb_sub_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar1_dp_ca_cast_htp_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar1_dp_ca_cast_htp_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar1_dp_ca_cast_htp_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar1_csetup, bhvm_hf3_vm_op_ar1_dp_ca_cast_htp_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar1_dp_ca_cast_htp_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar1_dp_ca_cast_htp_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar1_dp_ca_cast_htp_s_set_arg );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar1_dp_ca_cast_htp_s );
            BCORE_REGISTER_TRAIT( bhvm_hf3_vm_op_ar1_dp, bhvm_hf3_vm_op_ar1 );

            // group: bhvm_hf3_vm_op_ar2
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar2_add_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar2_add_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar2_add_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar2_csetup, bhvm_hf3_vm_op_ar2_add_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar2_add_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar2_add_s_set_arg );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar2_add_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar2_add_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar2_sub_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar2_sub_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar2_sub_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar2_csetup, bhvm_hf3_vm_op_ar2_sub_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar2_sub_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar2_sub_s_set_arg );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar2_sub_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar2_sub_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar2_hmul_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar2_hmul_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar2_hmul_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar2_csetup, bhvm_hf3_vm_op_ar2_hmul_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar2_hmul_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar2_hmul_s_set_arg );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar2_hmul_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar2_hmul_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar2_hdiv_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar2_hdiv_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar2_hdiv_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar2_csetup, bhvm_hf3_vm_op_ar2_hdiv_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar2_hdiv_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar2_hdiv_s_set_arg );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar2_hdiv_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar2_hdiv_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar2_bmul_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar2_bmul_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar2_bmul_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar2_csetup, bhvm_hf3_vm_op_ar2_bmul_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar2_bmul_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar2_bmul_s_set_arg );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar2_bmul_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar2_bmul_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar2_mul_scl_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar2_mul_scl_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar2_mul_scl_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar2_csetup, bhvm_hf3_vm_op_ar2_mul_scl_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar2_mul_scl_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar2_mul_scl_s_set_arg );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar2_mul_scl_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar2_mul_scl_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar2_scl_mul_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar2_scl_mul_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar2_scl_mul_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar2_csetup, bhvm_hf3_vm_op_ar2_scl_mul_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar2_scl_mul_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar2_scl_mul_s_set_arg );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar2_scl_mul_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar2_scl_mul_s );
            BCORE_REGISTER_TRAIT( bhvm_hf3_vm_op_ar2, bhvm_hf3_vm_op );

            // group: bhvm_hf3_vm_op_ar2_dp
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar2_dp_ca_exp_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar2_dp_ca_exp_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar2_dp_ca_exp_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar2_csetup, bhvm_hf3_vm_op_ar2_dp_ca_exp_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar2_dp_ca_exp_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar2_dp_ca_exp_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar2_dp_ca_exp_s_set_arg );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_exp_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar2_dp_ca_lgst_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar2_dp_ca_lgst_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar2_dp_ca_lgst_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar2_csetup, bhvm_hf3_vm_op_ar2_dp_ca_lgst_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar2_dp_ca_lgst_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar2_dp_ca_lgst_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar2_dp_ca_lgst_s_set_arg );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_lgst_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar2_dp_ca_lgst_hard_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar2_dp_ca_lgst_hard_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar2_dp_ca_lgst_hard_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar2_csetup, bhvm_hf3_vm_op_ar2_dp_ca_lgst_hard_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar2_dp_ca_lgst_hard_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar2_dp_ca_lgst_hard_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar2_dp_ca_lgst_hard_s_set_arg );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_lgst_hard_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar2_dp_ca_lgst_leaky_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar2_dp_ca_lgst_leaky_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar2_dp_ca_lgst_leaky_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar2_csetup, bhvm_hf3_vm_op_ar2_dp_ca_lgst_leaky_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar2_dp_ca_lgst_leaky_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar2_dp_ca_lgst_leaky_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar2_dp_ca_lgst_leaky_s_set_arg );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_lgst_leaky_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar2_dp_ca_tanh_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar2_dp_ca_tanh_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar2_dp_ca_tanh_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar2_csetup, bhvm_hf3_vm_op_ar2_dp_ca_tanh_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar2_dp_ca_tanh_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar2_dp_ca_tanh_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar2_dp_ca_tanh_s_set_arg );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_tanh_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar2_dp_ca_tanh_hard_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar2_dp_ca_tanh_hard_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar2_dp_ca_tanh_hard_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar2_csetup, bhvm_hf3_vm_op_ar2_dp_ca_tanh_hard_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar2_dp_ca_tanh_hard_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar2_dp_ca_tanh_hard_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar2_dp_ca_tanh_hard_s_set_arg );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_tanh_hard_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar2_dp_ca_tanh_leaky_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar2_dp_ca_tanh_leaky_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar2_dp_ca_tanh_leaky_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar2_csetup, bhvm_hf3_vm_op_ar2_dp_ca_tanh_leaky_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar2_dp_ca_tanh_leaky_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar2_dp_ca_tanh_leaky_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar2_dp_ca_tanh_leaky_s_set_arg );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_tanh_leaky_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar2_dp_ca_softplus_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar2_dp_ca_softplus_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar2_dp_ca_softplus_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar2_csetup, bhvm_hf3_vm_op_ar2_dp_ca_softplus_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar2_dp_ca_softplus_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar2_dp_ca_softplus_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar2_dp_ca_softplus_s_set_arg );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_softplus_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar2_dp_ca_relu_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar2_dp_ca_relu_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar2_dp_ca_relu_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar2_csetup, bhvm_hf3_vm_op_ar2_dp_ca_relu_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar2_dp_ca_relu_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar2_dp_ca_relu_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar2_dp_ca_relu_s_set_arg );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_relu_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar2_dp_ca_relu_leaky_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar2_dp_ca_relu_leaky_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar2_dp_ca_relu_leaky_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar2_csetup, bhvm_hf3_vm_op_ar2_dp_ca_relu_leaky_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar2_dp_ca_relu_leaky_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar2_dp_ca_relu_leaky_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar2_dp_ca_relu_leaky_s_set_arg );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_relu_leaky_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar2_dp_ca_hmul_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar2_dp_ca_hmul_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar2_dp_ca_hmul_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar2_csetup, bhvm_hf3_vm_op_ar2_dp_ca_hmul_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar2_dp_ca_hmul_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar2_dp_ca_hmul_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar2_dp_ca_hmul_s_set_arg );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_hmul_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar2_dp_cb_hmul_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar2_dp_cb_hmul_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar2_dp_cb_hmul_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar2_csetup, bhvm_hf3_vm_op_ar2_dp_cb_hmul_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar2_dp_cb_hmul_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar2_dp_cb_hmul_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar2_dp_cb_hmul_s_set_arg );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar2_dp_cb_hmul_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar2_dp_ca_bmul_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar2_dp_ca_bmul_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar2_dp_ca_bmul_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar2_csetup, bhvm_hf3_vm_op_ar2_dp_ca_bmul_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar2_dp_ca_bmul_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar2_dp_ca_bmul_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar2_dp_ca_bmul_s_set_arg );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_bmul_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar2_dp_cb_bmul_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar2_dp_cb_bmul_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar2_dp_cb_bmul_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar2_csetup, bhvm_hf3_vm_op_ar2_dp_cb_bmul_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar2_dp_cb_bmul_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar2_dp_cb_bmul_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar2_dp_cb_bmul_s_set_arg );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar2_dp_cb_bmul_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar2_dp_ca_mul_scl_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar2_dp_ca_mul_scl_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar2_dp_ca_mul_scl_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar2_csetup, bhvm_hf3_vm_op_ar2_dp_ca_mul_scl_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar2_dp_ca_mul_scl_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar2_dp_ca_mul_scl_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar2_dp_ca_mul_scl_s_set_arg );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_mul_scl_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar2_dp_cb_mul_scl_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar2_dp_cb_mul_scl_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar2_dp_cb_mul_scl_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar2_csetup, bhvm_hf3_vm_op_ar2_dp_cb_mul_scl_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar2_dp_cb_mul_scl_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar2_dp_cb_mul_scl_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar2_dp_cb_mul_scl_s_set_arg );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar2_dp_cb_mul_scl_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar2_dp_ca_scl_mul_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar2_dp_ca_scl_mul_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar2_dp_ca_scl_mul_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar2_csetup, bhvm_hf3_vm_op_ar2_dp_ca_scl_mul_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar2_dp_ca_scl_mul_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar2_dp_ca_scl_mul_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar2_dp_ca_scl_mul_s_set_arg );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar2_dp_ca_scl_mul_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar2_dp_cb_scl_mul_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar2_dp_cb_scl_mul_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar2_dp_cb_scl_mul_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar2_csetup, bhvm_hf3_vm_op_ar2_dp_cb_scl_mul_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar2_dp_cb_scl_mul_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar2_dp_cb_scl_mul_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar2_dp_cb_scl_mul_s_set_arg );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar2_dp_cb_scl_mul_s );
            BCORE_REGISTER_TRAIT( bhvm_hf3_vm_op_ar2_dp, bhvm_hf3_vm_op_ar2 );

            // group: bhvm_hf3_vm_op_ar3
            BCORE_REGISTER_TRAIT( bhvm_hf3_vm_op_ar3, bhvm_hf3_vm_op );

            // group: bhvm_hf3_vm_op_ar3_dp
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar3_dp_ca_hdiv_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar3_dp_ca_hdiv_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar3_dp_ca_hdiv_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar3_csetup, bhvm_hf3_vm_op_ar3_dp_ca_hdiv_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar3_dp_ca_hdiv_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar3_dp_ca_hdiv_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar3_dp_ca_hdiv_s_set_arg );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar3_dp_ca_hdiv_s );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_arity, bhvm_hf3_vm_op_ar3_dp_cb_hdiv_s_get_arity );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_indices, bhvm_hf3_vm_op_ar3_dp_cb_hdiv_s_set_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_get_indices, bhvm_hf3_vm_op_ar3_dp_cb_hdiv_s_get_indices );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_ar3_csetup, bhvm_hf3_vm_op_ar3_dp_cb_hdiv_s_csetup );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_sig, bhvm_hf3_vm_op_ar3_dp_cb_hdiv_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_run, bhvm_hf3_vm_op_ar3_dp_cb_hdiv_s_run );
            BCORE_REGISTER_FFUNC( bhvm_hf3_vm_op_set_arg, bhvm_hf3_vm_op_ar3_dp_cb_hdiv_s_set_arg );
            BCORE_REGISTER_OBJECT( bhvm_hf3_vm_op_ar3_dp_cb_hdiv_s );
            BCORE_REGISTER_TRAIT( bhvm_hf3_vm_op_ar3_dp, bhvm_hf3_vm_op_ar3 );
        }
        break;
        default: break;
    }
    return NULL;
}
