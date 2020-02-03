/** This file was generated from beth-plant source code.
 *  Compiling Agent : bcore_plant_compiler (C) 2019 J.B.Steffens
 *  Last File Update: 2020-02-03T14:17:06Z
 *
 *  Copyright and License of this File:
 *
 *  Generated code inherits the copyright and license of the underlying beth-plant source code.
 *  Source code defining this file is distributed across following files:
 *
 *  bhvm_vop.h
 *
 */

#include "bhvm_planted_vop.h"
#include "bcore_spect.h"
#include "bcore_spect_inst.h"
#include "bcore_sr.h"
#include "bcore_const_manager.h"


/**********************************************************************************************************************/
// source: bhvm_vop.h
#include "bhvm_vop.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_vop

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_arr_s )
"aware bcore_array"
"{"
    "aware bhvm_vop => [];"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ci_s )
"bcore_inst"
"{"
    "u0_t c;"
    "sz_t i;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_arr_ci_s )
"aware bcore_array"
"{"
    "bhvm_vop_ci_s [];"
"}";

void bhvm_vop_arr_ci_s_push_ci( bhvm_vop_arr_ci_s* o, u0_t c, sz_t i )
{
    bhvm_vop_ci_s_push_ci( bhvm_vop_arr_ci_s_push( o ), c, i );
}

BCORE_DEFINE_SPECT( bcore_inst, bhvm_vop )
"{"
    "bcore_spect_header_s header;"
    "feature aware bhvm_vop : run;"
    "feature aware bhvm_vop : arity;"
    "feature aware bhvm_vop : sig;"
    "feature aware bhvm_vop : get_index_arr;"
    "feature aware bhvm_vop : get_index = bhvm_vop_get_index__;"
    "feature aware bhvm_vop : set_index = bhvm_vop_set_index__;"
    "feature aware bhvm_vop : set_arg = bhvm_vop_set_arg__;"
    "feature aware bhvm_vop : set_args = bhvm_vop_set_args__;"
"}";


sz_t bhvm_vop_get_index__( const bhvm_vop* o, sz_t index )
{
    ASSERT( index  >= 0 && index <= bhvm_vop_a_arity( o ) );
    return bhvm_vop_a_get_index_arr( (bhvm_vop*)o )[ index ];
}

bhvm_vop* bhvm_vop_set_index__( bhvm_vop* o, sz_t index, sz_t idx_val )
{
    ASSERT( index  >= 0 && index <= bhvm_vop_a_arity( o ) );
    bhvm_vop_a_get_index_arr( o )[ index ] = idx_val;
    return o;
}

void bhvm_vop_set_arg__( bhvm_vop* o, bhvm_vop_ci_s* ci )
{
    sz_t size   = bhvm_vop_a_arity( o ) + 1;
    sc_t  sig   = bhvm_vop_a_sig( o );
    sz_t* index = bhvm_vop_a_get_index_arr( o );
    for( sz_t i = 0; i < size; i++ )
    {
        u0_t c = sig[ i ];
        ASSERT( c );
        if( ci->c == c )
        {
            index[ i ] = ci->i;
            return;
        }
    }
}

bhvm_vop* bhvm_vop_set_args__( bhvm_vop* o, const bhvm_vop_arr_ci_s* arr )
{
    for( sz_t i = 0; i < arr->size; i++ ) bhvm_vop_a_set_arg( o, &arr->data[ i ] );
    return o;
}
//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_vop_ar0

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar0_index_s )
"bcore_inst"
"{"
    "sz_t [ 1 ] v;"
    "func bcore_inst_call:init_x;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar0_zro_s )
"aware bhvm_vop_ar0"
"{"
    "bhvm_vop_ar0_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar0_one_s )
"aware bhvm_vop_ar0"
"{"
    "bhvm_vop_ar0_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar0_determine_s )
"aware bhvm_vop_ar0"
"{"
    "bhvm_vop_ar0_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar0_vacate_s )
"aware bhvm_vop_ar0"
"{"
    "bhvm_vop_ar0_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar0_randomize_s )
"aware bhvm_vop_ar0"
"{"
    "bhvm_vop_ar0_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "u2_t rseed = 1234;"
    "f3_t min = -0.5;"
    "f3_t max = 0.5;"
    "f3_t density = 1.0;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

void bhvm_vop_ar0_randomize_s_run( const bhvm_vop_ar0_randomize_s* o, bhvm_holor_s* ah )
{
    sz_t i = o->i.v[ 0 ];
    u2_t rval = o->rseed + i;
    bhvm_value_s_set_random( &ah[ i ].v, o->density, o->min, o->max, &rval );
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar0_nul_dp_s )
"aware bhvm_vop_ar0"
"{"
    "bhvm_vop_ar0_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_vop_ar1

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar1_index_s )
"bcore_inst"
"{"
    "sz_t [ 2 ] v;"
    "func bcore_inst_call:init_x;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar1_identity_s )
"aware bhvm_vop_ar1"
"{"
    "bhvm_vop_ar1_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar1_neg_s )
"aware bhvm_vop_ar1"
"{"
    "bhvm_vop_ar1_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar1_floor_s )
"aware bhvm_vop_ar1"
"{"
    "bhvm_vop_ar1_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar1_ceil_s )
"aware bhvm_vop_ar1"
"{"
    "bhvm_vop_ar1_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar1_exp_s )
"aware bhvm_vop_ar1"
"{"
    "bhvm_vop_ar1_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar1_log_s )
"aware bhvm_vop_ar1"
"{"
    "bhvm_vop_ar1_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar1_inv_s )
"aware bhvm_vop_ar1"
"{"
    "bhvm_vop_ar1_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar1_sqr_s )
"aware bhvm_vop_ar1"
"{"
    "bhvm_vop_ar1_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar1_srt_s )
"aware bhvm_vop_ar1"
"{"
    "bhvm_vop_ar1_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar1_abs_s )
"aware bhvm_vop_ar1"
"{"
    "bhvm_vop_ar1_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar1_l1_s )
"aware bhvm_vop_ar1"
"{"
    "bhvm_vop_ar1_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar1_lgst_s )
"aware bhvm_vop_ar1"
"{"
    "bhvm_vop_ar1_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar1_lgst_hard_s )
"aware bhvm_vop_ar1"
"{"
    "bhvm_vop_ar1_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar1_lgst_leaky_s )
"aware bhvm_vop_ar1"
"{"
    "bhvm_vop_ar1_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar1_tanh_s )
"aware bhvm_vop_ar1"
"{"
    "bhvm_vop_ar1_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar1_tanh_hard_s )
"aware bhvm_vop_ar1"
"{"
    "bhvm_vop_ar1_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar1_tanh_leaky_s )
"aware bhvm_vop_ar1"
"{"
    "bhvm_vop_ar1_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar1_softplus_s )
"aware bhvm_vop_ar1"
"{"
    "bhvm_vop_ar1_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar1_relu_s )
"aware bhvm_vop_ar1"
"{"
    "bhvm_vop_ar1_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar1_relu_leaky_s )
"aware bhvm_vop_ar1"
"{"
    "bhvm_vop_ar1_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar1_order_inc_s )
"aware bhvm_vop_ar1"
"{"
    "bhvm_vop_ar1_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "sz_t dim;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar1_order_dec_s )
"aware bhvm_vop_ar1"
"{"
    "bhvm_vop_ar1_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "sz_t idx;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar1_order_dec_fork_s )
"aware bhvm_vop_ar1"
"{"
    "bhvm_vop_ar1_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "sz_t idx;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar1_add_dp_a_s )
"aware bhvm_vop_ar1"
"{"
    "bhvm_vop_ar1_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar1_add_dp_b_s )
"aware bhvm_vop_ar1"
"{"
    "bhvm_vop_ar1_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar1_sub_dp_a_s )
"aware bhvm_vop_ar1"
"{"
    "bhvm_vop_ar1_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar1_sub_dp_b_s )
"aware bhvm_vop_ar1"
"{"
    "bhvm_vop_ar1_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar1_identity_dp_s )
"aware bhvm_vop_ar1"
"{"
    "bhvm_vop_ar1_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar1_neg_dp_s )
"aware bhvm_vop_ar1"
"{"
    "bhvm_vop_ar1_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar1_cpy_ay_s )
"aware bhvm_vop_ar1"
"{"
    "bhvm_vop_ar1_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar1_cpy_by_s )
"aware bhvm_vop_ar1"
"{"
    "bhvm_vop_ar1_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar1_fork_s )
"aware bhvm_vop_ar1"
"{"
    "bhvm_vop_ar1_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_vop_ar2

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_index_s )
"bcore_inst"
"{"
    "sz_t [ 3 ] v;"
    "func bcore_inst_call:init_x;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_cat_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_add_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_sub_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_mul_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_div_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_pow_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_mul_mvv_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_mul_vmv_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_mul_tvv_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_mul_vtv_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_mul_vvm_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_mul_mmm_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_mul_mtm_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_mul_tmm_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_mul_ttm_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_equal_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_larger_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_smaller_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_larger_equal_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_smaller_equal_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_logic_and_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_logic_or_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:sig;"
    "func bhvm_vop:run;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_mul_mvv_dp_a_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_mul_vmv_dp_a_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_mul_tvv_dp_a_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_mul_vtv_dp_a_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_mul_vvm_dp_a_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_mul_mvv_dp_b_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_mul_vmv_dp_b_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_mul_tvv_dp_b_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_mul_vtv_dp_b_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_mul_vvm_dp_b_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_mul_mmm_dp_a_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_mul_mtm_dp_a_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_mul_tmm_dp_a_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_mul_ttm_dp_a_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_mul_mmm_dp_b_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_mul_mtm_dp_b_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_mul_tmm_dp_b_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_mul_ttm_dp_b_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_mul_dp_a_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_mul_dp_b_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_div_dp_a_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_exp_dp_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_log_dp_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_inv_dp_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_sqr_dp_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_srt_dp_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_lgst_dp_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_lgst_hard_dp_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_lgst_leaky_dp_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_tanh_dp_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_tanh_hard_dp_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_tanh_leaky_dp_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_softplus_dp_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_relu_dp_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_relu_leaky_dp_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_branch_vvvv_dp_b_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar2_branch_vvvv_dp_c_s )
"aware bhvm_vop_ar2"
"{"
    "bhvm_vop_ar2_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_vop_ar3

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar3_index_s )
"bcore_inst"
"{"
    "sz_t [ 4 ] v;"
    "func bcore_inst_call:init_x;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar3_branch_vvvv_s )
"aware bhvm_vop_ar3"
"{"
    "bhvm_vop_ar3_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar3_branch_svvv_s )
"aware bhvm_vop_ar3"
"{"
    "bhvm_vop_ar3_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar3_div_dp_b_s )
"aware bhvm_vop_ar3"
"{"
    "bhvm_vop_ar3_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar3_pow_dp_a_s )
"aware bhvm_vop_ar3"
"{"
    "bhvm_vop_ar3_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_vop_ar3_pow_dp_b_s )
"aware bhvm_vop_ar3"
"{"
    "bhvm_vop_ar3_index_s i;"
    "func bhvm_vop:arity;"
    "func bhvm_vop:get_index_arr;"
    "func bhvm_vop:run;"
    "func bhvm_vop:sig;"
"}";

/**********************************************************************************************************************/

vd_t bhvm_planted_vop_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bhvm_planted_vop" ) ) )
    {
        case TYPEOF_init1:
        {
            // Comment or remove line below to rebuild this target.
            bcore_const_x_set_d( typeof( "bhvm_planted_vop_hash" ), sr_tp( 1210586706 ) );

            // --------------------------------------------------------------------
            // source: bhvm_vop.h

            // group: bhvm_vop
            BCORE_REGISTER_OBJECT( bhvm_vop_arr_s );
            BCORE_REGISTER_FEATURE( bhvm_vop_run );
            BCORE_REGISTER_FEATURE( bhvm_vop_arity );
            BCORE_REGISTER_FEATURE( bhvm_vop_sig );
            BCORE_REGISTER_FEATURE( bhvm_vop_get_index_arr );
            BCORE_REGISTER_FEATURE( bhvm_vop_get_index );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index, bhvm_vop_get_index__ );
            BCORE_REGISTER_FEATURE( bhvm_vop_set_index );
            BCORE_REGISTER_FFUNC( bhvm_vop_set_index, bhvm_vop_set_index__ );
            BCORE_REGISTER_OBJECT( bhvm_vop_ci_s );
            BCORE_REGISTER_OBJECT( bhvm_vop_arr_ci_s );
            BCORE_REGISTER_FEATURE( bhvm_vop_set_arg );
            BCORE_REGISTER_FFUNC( bhvm_vop_set_arg, bhvm_vop_set_arg__ );
            BCORE_REGISTER_FEATURE( bhvm_vop_set_args );
            BCORE_REGISTER_FFUNC( bhvm_vop_set_args, bhvm_vop_set_args__ );
            BCORE_REGISTER_SPECT( bhvm_vop );

            // group: bhvm_vop_ar0
            BCORE_REGISTER_FFUNC( bcore_inst_call_init_x, bhvm_vop_ar0_index_s_init_x );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar0_index_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar0_zro_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar0_zro_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar0_zro_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar0_zro_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar0_zro_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar0_one_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar0_one_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar0_one_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar0_one_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar0_one_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar0_determine_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar0_determine_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar0_determine_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar0_determine_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar0_determine_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar0_vacate_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar0_vacate_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar0_vacate_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar0_vacate_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar0_vacate_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar0_randomize_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar0_randomize_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar0_randomize_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar0_randomize_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar0_randomize_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar0_nul_dp_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar0_nul_dp_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar0_nul_dp_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar0_nul_dp_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar0_nul_dp_s );
            BCORE_REGISTER_TRAIT( bhvm_vop_ar0, bhvm_vop );

            // group: bhvm_vop_ar1
            BCORE_REGISTER_FFUNC( bcore_inst_call_init_x, bhvm_vop_ar1_index_s_init_x );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar1_index_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar1_identity_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar1_identity_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar1_identity_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar1_identity_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar1_identity_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar1_neg_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar1_neg_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar1_neg_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar1_neg_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar1_neg_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar1_floor_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar1_floor_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar1_floor_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar1_floor_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar1_floor_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar1_ceil_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar1_ceil_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar1_ceil_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar1_ceil_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar1_ceil_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar1_exp_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar1_exp_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar1_exp_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar1_exp_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar1_exp_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar1_log_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar1_log_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar1_log_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar1_log_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar1_log_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar1_inv_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar1_inv_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar1_inv_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar1_inv_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar1_inv_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar1_sqr_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar1_sqr_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar1_sqr_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar1_sqr_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar1_sqr_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar1_srt_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar1_srt_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar1_srt_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar1_srt_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar1_srt_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar1_abs_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar1_abs_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar1_abs_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar1_abs_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar1_abs_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar1_l1_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar1_l1_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar1_l1_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar1_l1_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar1_l1_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar1_lgst_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar1_lgst_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar1_lgst_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar1_lgst_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar1_lgst_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar1_lgst_hard_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar1_lgst_hard_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar1_lgst_hard_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar1_lgst_hard_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar1_lgst_hard_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar1_lgst_leaky_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar1_lgst_leaky_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar1_lgst_leaky_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar1_lgst_leaky_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar1_lgst_leaky_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar1_tanh_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar1_tanh_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar1_tanh_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar1_tanh_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar1_tanh_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar1_tanh_hard_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar1_tanh_hard_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar1_tanh_hard_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar1_tanh_hard_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar1_tanh_hard_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar1_tanh_leaky_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar1_tanh_leaky_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar1_tanh_leaky_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar1_tanh_leaky_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar1_tanh_leaky_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar1_softplus_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar1_softplus_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar1_softplus_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar1_softplus_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar1_softplus_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar1_relu_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar1_relu_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar1_relu_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar1_relu_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar1_relu_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar1_relu_leaky_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar1_relu_leaky_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar1_relu_leaky_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar1_relu_leaky_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar1_relu_leaky_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar1_order_inc_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar1_order_inc_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar1_order_inc_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar1_order_inc_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar1_order_inc_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar1_order_dec_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar1_order_dec_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar1_order_dec_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar1_order_dec_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar1_order_dec_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar1_order_dec_fork_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar1_order_dec_fork_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar1_order_dec_fork_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar1_order_dec_fork_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar1_order_dec_fork_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar1_add_dp_a_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar1_add_dp_a_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar1_add_dp_a_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar1_add_dp_a_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar1_add_dp_a_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar1_add_dp_b_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar1_add_dp_b_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar1_add_dp_b_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar1_add_dp_b_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar1_add_dp_b_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar1_sub_dp_a_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar1_sub_dp_a_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar1_sub_dp_a_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar1_sub_dp_a_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar1_sub_dp_a_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar1_sub_dp_b_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar1_sub_dp_b_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar1_sub_dp_b_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar1_sub_dp_b_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar1_sub_dp_b_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar1_identity_dp_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar1_identity_dp_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar1_identity_dp_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar1_identity_dp_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar1_identity_dp_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar1_neg_dp_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar1_neg_dp_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar1_neg_dp_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar1_neg_dp_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar1_neg_dp_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar1_cpy_ay_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar1_cpy_ay_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar1_cpy_ay_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar1_cpy_ay_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar1_cpy_ay_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar1_cpy_by_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar1_cpy_by_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar1_cpy_by_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar1_cpy_by_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar1_cpy_by_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar1_fork_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar1_fork_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar1_fork_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar1_fork_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar1_fork_s );
            BCORE_REGISTER_TRAIT( bhvm_vop_ar1, bhvm_vop );

            // group: bhvm_vop_ar2
            BCORE_REGISTER_FFUNC( bcore_inst_call_init_x, bhvm_vop_ar2_index_s_init_x );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_index_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_cat_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_cat_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_cat_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_cat_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_cat_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_add_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_add_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_add_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_add_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_add_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_sub_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_sub_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_sub_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_sub_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_sub_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_mul_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_mul_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_mul_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_mul_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_mul_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_div_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_div_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_div_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_div_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_div_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_pow_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_pow_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_pow_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_pow_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_pow_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_mul_mvv_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_mul_mvv_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_mul_mvv_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_mul_mvv_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_mul_mvv_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_mul_vmv_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_mul_vmv_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_mul_vmv_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_mul_vmv_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_mul_vmv_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_mul_tvv_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_mul_tvv_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_mul_tvv_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_mul_tvv_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_mul_tvv_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_mul_vtv_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_mul_vtv_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_mul_vtv_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_mul_vtv_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_mul_vtv_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_mul_vvm_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_mul_vvm_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_mul_vvm_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_mul_vvm_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_mul_vvm_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_mul_mmm_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_mul_mmm_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_mul_mmm_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_mul_mmm_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_mul_mmm_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_mul_mtm_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_mul_mtm_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_mul_mtm_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_mul_mtm_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_mul_mtm_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_mul_tmm_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_mul_tmm_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_mul_tmm_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_mul_tmm_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_mul_tmm_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_mul_ttm_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_mul_ttm_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_mul_ttm_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_mul_ttm_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_mul_ttm_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_equal_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_equal_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_equal_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_equal_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_equal_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_larger_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_larger_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_larger_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_larger_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_larger_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_smaller_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_smaller_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_smaller_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_smaller_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_smaller_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_larger_equal_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_larger_equal_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_larger_equal_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_larger_equal_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_larger_equal_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_smaller_equal_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_smaller_equal_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_smaller_equal_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_smaller_equal_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_smaller_equal_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_logic_and_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_logic_and_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_logic_and_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_logic_and_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_logic_and_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_logic_or_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_logic_or_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_logic_or_s_sig );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_logic_or_s_run );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_logic_or_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_mul_mvv_dp_a_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_mul_mvv_dp_a_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_mul_mvv_dp_a_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_mul_mvv_dp_a_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_mul_mvv_dp_a_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_mul_vmv_dp_a_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_mul_vmv_dp_a_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_mul_vmv_dp_a_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_mul_vmv_dp_a_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_mul_vmv_dp_a_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_mul_tvv_dp_a_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_mul_tvv_dp_a_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_mul_tvv_dp_a_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_mul_tvv_dp_a_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_mul_tvv_dp_a_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_mul_vtv_dp_a_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_mul_vtv_dp_a_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_mul_vtv_dp_a_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_mul_vtv_dp_a_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_mul_vtv_dp_a_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_mul_vvm_dp_a_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_mul_vvm_dp_a_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_mul_vvm_dp_a_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_mul_vvm_dp_a_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_mul_vvm_dp_a_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_mul_mvv_dp_b_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_mul_mvv_dp_b_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_mul_mvv_dp_b_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_mul_mvv_dp_b_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_mul_mvv_dp_b_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_mul_vmv_dp_b_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_mul_vmv_dp_b_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_mul_vmv_dp_b_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_mul_vmv_dp_b_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_mul_vmv_dp_b_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_mul_tvv_dp_b_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_mul_tvv_dp_b_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_mul_tvv_dp_b_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_mul_tvv_dp_b_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_mul_tvv_dp_b_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_mul_vtv_dp_b_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_mul_vtv_dp_b_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_mul_vtv_dp_b_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_mul_vtv_dp_b_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_mul_vtv_dp_b_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_mul_vvm_dp_b_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_mul_vvm_dp_b_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_mul_vvm_dp_b_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_mul_vvm_dp_b_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_mul_vvm_dp_b_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_mul_mmm_dp_a_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_mul_mmm_dp_a_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_mul_mmm_dp_a_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_mul_mmm_dp_a_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_mul_mmm_dp_a_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_mul_mtm_dp_a_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_mul_mtm_dp_a_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_mul_mtm_dp_a_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_mul_mtm_dp_a_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_mul_mtm_dp_a_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_mul_tmm_dp_a_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_mul_tmm_dp_a_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_mul_tmm_dp_a_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_mul_tmm_dp_a_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_mul_tmm_dp_a_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_mul_ttm_dp_a_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_mul_ttm_dp_a_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_mul_ttm_dp_a_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_mul_ttm_dp_a_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_mul_ttm_dp_a_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_mul_mmm_dp_b_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_mul_mmm_dp_b_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_mul_mmm_dp_b_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_mul_mmm_dp_b_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_mul_mmm_dp_b_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_mul_mtm_dp_b_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_mul_mtm_dp_b_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_mul_mtm_dp_b_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_mul_mtm_dp_b_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_mul_mtm_dp_b_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_mul_tmm_dp_b_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_mul_tmm_dp_b_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_mul_tmm_dp_b_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_mul_tmm_dp_b_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_mul_tmm_dp_b_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_mul_ttm_dp_b_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_mul_ttm_dp_b_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_mul_ttm_dp_b_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_mul_ttm_dp_b_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_mul_ttm_dp_b_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_mul_dp_a_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_mul_dp_a_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_mul_dp_a_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_mul_dp_a_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_mul_dp_a_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_mul_dp_b_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_mul_dp_b_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_mul_dp_b_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_mul_dp_b_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_mul_dp_b_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_div_dp_a_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_div_dp_a_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_div_dp_a_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_div_dp_a_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_div_dp_a_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_exp_dp_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_exp_dp_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_exp_dp_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_exp_dp_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_exp_dp_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_log_dp_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_log_dp_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_log_dp_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_log_dp_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_log_dp_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_inv_dp_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_inv_dp_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_inv_dp_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_inv_dp_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_inv_dp_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_sqr_dp_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_sqr_dp_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_sqr_dp_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_sqr_dp_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_sqr_dp_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_srt_dp_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_srt_dp_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_srt_dp_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_srt_dp_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_srt_dp_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_lgst_dp_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_lgst_dp_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_lgst_dp_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_lgst_dp_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_lgst_dp_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_lgst_hard_dp_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_lgst_hard_dp_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_lgst_hard_dp_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_lgst_hard_dp_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_lgst_hard_dp_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_lgst_leaky_dp_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_lgst_leaky_dp_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_lgst_leaky_dp_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_lgst_leaky_dp_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_lgst_leaky_dp_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_tanh_dp_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_tanh_dp_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_tanh_dp_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_tanh_dp_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_tanh_dp_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_tanh_hard_dp_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_tanh_hard_dp_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_tanh_hard_dp_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_tanh_hard_dp_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_tanh_hard_dp_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_tanh_leaky_dp_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_tanh_leaky_dp_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_tanh_leaky_dp_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_tanh_leaky_dp_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_tanh_leaky_dp_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_softplus_dp_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_softplus_dp_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_softplus_dp_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_softplus_dp_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_softplus_dp_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_relu_dp_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_relu_dp_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_relu_dp_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_relu_dp_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_relu_dp_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_relu_leaky_dp_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_relu_leaky_dp_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_relu_leaky_dp_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_relu_leaky_dp_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_relu_leaky_dp_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_branch_vvvv_dp_b_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_branch_vvvv_dp_b_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_branch_vvvv_dp_b_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_branch_vvvv_dp_b_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_branch_vvvv_dp_b_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar2_branch_vvvv_dp_c_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar2_branch_vvvv_dp_c_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar2_branch_vvvv_dp_c_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar2_branch_vvvv_dp_c_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar2_branch_vvvv_dp_c_s );
            BCORE_REGISTER_TRAIT( bhvm_vop_ar2, bhvm_vop );

            // group: bhvm_vop_ar3
            BCORE_REGISTER_FFUNC( bcore_inst_call_init_x, bhvm_vop_ar3_index_s_init_x );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar3_index_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar3_branch_vvvv_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar3_branch_vvvv_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar3_branch_vvvv_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar3_branch_vvvv_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar3_branch_vvvv_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar3_branch_svvv_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar3_branch_svvv_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar3_branch_svvv_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar3_branch_svvv_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar3_branch_svvv_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar3_div_dp_b_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar3_div_dp_b_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar3_div_dp_b_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar3_div_dp_b_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar3_div_dp_b_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar3_pow_dp_a_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar3_pow_dp_a_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar3_pow_dp_a_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar3_pow_dp_a_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar3_pow_dp_a_s );
            BCORE_REGISTER_FFUNC( bhvm_vop_arity, bhvm_vop_ar3_pow_dp_b_s_arity );
            BCORE_REGISTER_FFUNC( bhvm_vop_get_index_arr, bhvm_vop_ar3_pow_dp_b_s_get_index_arr );
            BCORE_REGISTER_FFUNC( bhvm_vop_run, bhvm_vop_ar3_pow_dp_b_s_run );
            BCORE_REGISTER_FFUNC( bhvm_vop_sig, bhvm_vop_ar3_pow_dp_b_s_sig );
            BCORE_REGISTER_OBJECT( bhvm_vop_ar3_pow_dp_b_s );
            BCORE_REGISTER_TRAIT( bhvm_vop_ar3, bhvm_vop );
        }
        break;
        default: break;
    }
    return NULL;
}
