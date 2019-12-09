/** This file was generated from beth-plant source code.
 *  Compiling Agent : bcore_plant_compiler (C) 2019 J.B.Steffens
 *  Last File Update: 2019-12-09T10:54:10Z
 *
 *  Copyright and License of this File:
 *
 *  Generated code inherits the copyright and license of the underlying beth-plant source code.
 *  Source code defining this file is distributed across following files:
 *
 *  bhvm_lop.h
 *
 */

#include "bhvm_planted_lop.h"
#include "bcore_spect.h"
#include "bcore_spect_inst.h"
#include "bcore_sr.h"
#include "bcore_const_manager.h"


/**********************************************************************************************************************/
// source: bhvm_lop.h
#include "bhvm_lop.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_lop

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_lop_ar0

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar0_zro_s )
"aware bhvm_lop_ar0"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func bhvm_lop_ar0:k_cv;"
"}";

void bhvm_lop_ar0_zro_s_k_cv( tp_t tknit, vd_t r, sz_t s )
{
    assert( s == 0 || r );
    switch( tknit )
    {
        case BKNIT_F2: { f2_t v = bhvm_lop_ar0_zro_s_f2(); for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=v; } } break;
        case BKNIT_F3: { f3_t v = bhvm_lop_ar0_zro_s_f3(); for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=v; } } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar0_one_s )
"aware bhvm_lop_ar0"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func bhvm_lop_ar0:k_cv;"
"}";

void bhvm_lop_ar0_one_s_k_cv( tp_t tknit, vd_t r, sz_t s )
{
    assert( s == 0 || r );
    switch( tknit )
    {
        case BKNIT_F2: { f2_t v = bhvm_lop_ar0_one_s_f2(); for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=v; } } break;
        case BKNIT_F3: { f3_t v = bhvm_lop_ar0_one_s_f3(); for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=v; } } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_lop_ar1

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_identity_s )
"aware bhvm_lop_ar1"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func ^:k_s_cv;"
    "func bhvm_lop_ar1:k_v_cv;"
"}";

void bhvm_lop_ar1_identity_s_k_s_cv( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_identity_s_f2(((f2_t*)a)[0]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_identity_s_f2(((f2_t*)a)[0]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_identity_s_f3(((f3_t*)a)[0]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_identity_s_f3(((f3_t*)a)[0]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

void bhvm_lop_ar1_identity_s_k_v_cv( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_identity_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_identity_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_identity_s_f3(((f3_t*)a)[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_identity_s_f3(((f3_t*)a)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_neg_s )
"aware bhvm_lop_ar1"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func bhvm_lop_ar1:k_v_cv;"
"}";

void bhvm_lop_ar1_neg_s_k_v_cv( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_neg_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_neg_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_neg_s_f3(((f3_t*)a)[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_neg_s_f3(((f3_t*)a)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_floor_s )
"aware bhvm_lop_ar1"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func bhvm_lop_ar1:k_v_cv;"
"}";

void bhvm_lop_ar1_floor_s_k_v_cv( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_floor_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_floor_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_floor_s_f3(((f3_t*)a)[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_floor_s_f3(((f3_t*)a)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_ceil_s )
"aware bhvm_lop_ar1"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func bhvm_lop_ar1:k_v_cv;"
"}";

void bhvm_lop_ar1_ceil_s_k_v_cv( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_ceil_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_ceil_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_ceil_s_f3(((f3_t*)a)[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_ceil_s_f3(((f3_t*)a)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_exp_s )
"aware bhvm_lop_ar1"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func bhvm_lop_ar1:k_v_cv;"
"}";

void bhvm_lop_ar1_exp_s_k_v_cv( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_exp_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_exp_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_exp_s_f3(((f3_t*)a)[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_exp_s_f3(((f3_t*)a)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_inv_s )
"aware bhvm_lop_ar1"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func bhvm_lop_ar1:k_v_cv;"
"}";

void bhvm_lop_ar1_inv_s_k_v_cv( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_inv_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_inv_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_inv_s_f3(((f3_t*)a)[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_inv_s_f3(((f3_t*)a)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_add_dp_a_v_s )
"aware bhvm_lop_ar1"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func ^:k_v_av;"
    "func bhvm_lop_ar1:k_v_cv;"
"}";

void bhvm_lop_ar1_add_dp_a_v_s_k_v_av( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar1_add_dp_a_v_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar1_add_dp_a_v_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar1_add_dp_a_v_s_f3(((f3_t*)a)[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar1_add_dp_a_v_s_f3(((f3_t*)a)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

void bhvm_lop_ar1_add_dp_a_v_s_k_v_cv( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_add_dp_a_v_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_add_dp_a_v_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_add_dp_a_v_s_f3(((f3_t*)a)[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_add_dp_a_v_s_f3(((f3_t*)a)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_add_dp_b_v_s )
"aware bhvm_lop_ar1"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func ^:k_v_av;"
    "func bhvm_lop_ar1:k_v_cv;"
"}";

void bhvm_lop_ar1_add_dp_b_v_s_k_v_av( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar1_add_dp_b_v_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar1_add_dp_b_v_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar1_add_dp_b_v_s_f3(((f3_t*)a)[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar1_add_dp_b_v_s_f3(((f3_t*)a)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

void bhvm_lop_ar1_add_dp_b_v_s_k_v_cv( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_add_dp_b_v_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_add_dp_b_v_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_add_dp_b_v_s_f3(((f3_t*)a)[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_add_dp_b_v_s_f3(((f3_t*)a)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_sub_dp_a_v_s )
"aware bhvm_lop_ar1"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func ^:k_v_av;"
    "func bhvm_lop_ar1:k_v_cv;"
"}";

void bhvm_lop_ar1_sub_dp_a_v_s_k_v_av( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar1_sub_dp_a_v_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar1_sub_dp_a_v_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar1_sub_dp_a_v_s_f3(((f3_t*)a)[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar1_sub_dp_a_v_s_f3(((f3_t*)a)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

void bhvm_lop_ar1_sub_dp_a_v_s_k_v_cv( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_sub_dp_a_v_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_sub_dp_a_v_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_sub_dp_a_v_s_f3(((f3_t*)a)[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_sub_dp_a_v_s_f3(((f3_t*)a)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_sub_dp_b_v_s )
"aware bhvm_lop_ar1"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func ^:k_v_av;"
    "func bhvm_lop_ar1:k_v_cv;"
"}";

void bhvm_lop_ar1_sub_dp_b_v_s_k_v_av( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar1_sub_dp_b_v_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar1_sub_dp_b_v_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar1_sub_dp_b_v_s_f3(((f3_t*)a)[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar1_sub_dp_b_v_s_f3(((f3_t*)a)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

void bhvm_lop_ar1_sub_dp_b_v_s_k_v_cv( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_sub_dp_b_v_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_sub_dp_b_v_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_sub_dp_b_v_s_f3(((f3_t*)a)[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_sub_dp_b_v_s_f3(((f3_t*)a)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_lgst_s )
"aware bhvm_lop_ar1"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func bhvm_lop_ar1:k_v_cv;"
"}";

void bhvm_lop_ar1_lgst_s_k_v_cv( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_lgst_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_lgst_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_lgst_s_f3(((f3_t*)a)[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_lgst_s_f3(((f3_t*)a)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_lgst_hard_s )
"aware bhvm_lop_ar1"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func bhvm_lop_ar1:k_v_cv;"
"}";

void bhvm_lop_ar1_lgst_hard_s_k_v_cv( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_lgst_hard_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_lgst_hard_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_lgst_hard_s_f3(((f3_t*)a)[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_lgst_hard_s_f3(((f3_t*)a)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_lgst_leaky_s )
"aware bhvm_lop_ar1"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func bhvm_lop_ar1:k_v_cv;"
"}";

void bhvm_lop_ar1_lgst_leaky_s_k_v_cv( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_lgst_leaky_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_lgst_leaky_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_lgst_leaky_s_f3(((f3_t*)a)[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_lgst_leaky_s_f3(((f3_t*)a)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_tanh_s )
"aware bhvm_lop_ar1"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func bhvm_lop_ar1:k_v_cv;"
"}";

void bhvm_lop_ar1_tanh_s_k_v_cv( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_tanh_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_tanh_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_tanh_s_f3(((f3_t*)a)[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_tanh_s_f3(((f3_t*)a)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_tanh_hard_s )
"aware bhvm_lop_ar1"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func bhvm_lop_ar1:k_v_cv;"
"}";

void bhvm_lop_ar1_tanh_hard_s_k_v_cv( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_tanh_hard_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_tanh_hard_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_tanh_hard_s_f3(((f3_t*)a)[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_tanh_hard_s_f3(((f3_t*)a)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_tanh_leaky_s )
"aware bhvm_lop_ar1"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func bhvm_lop_ar1:k_v_cv;"
"}";

void bhvm_lop_ar1_tanh_leaky_s_k_v_cv( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_tanh_leaky_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_tanh_leaky_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_tanh_leaky_s_f3(((f3_t*)a)[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_tanh_leaky_s_f3(((f3_t*)a)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_softplus_s )
"aware bhvm_lop_ar1"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func bhvm_lop_ar1:k_v_cv;"
"}";

void bhvm_lop_ar1_softplus_s_k_v_cv( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_softplus_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_softplus_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_softplus_s_f3(((f3_t*)a)[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_softplus_s_f3(((f3_t*)a)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_relu_s )
"aware bhvm_lop_ar1"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func bhvm_lop_ar1:k_v_cv;"
"}";

void bhvm_lop_ar1_relu_s_k_v_cv( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_relu_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_relu_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_relu_s_f3(((f3_t*)a)[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_relu_s_f3(((f3_t*)a)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_relu_leaky_s )
"aware bhvm_lop_ar1"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func bhvm_lop_ar1:k_v_cv;"
"}";

void bhvm_lop_ar1_relu_leaky_s_k_v_cv( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_relu_leaky_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_relu_leaky_s_f2(((f2_t*)a)[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar1_relu_leaky_s_f3(((f3_t*)a)[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar1_relu_leaky_s_f3(((f3_t*)a)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_lop_ar2

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_add_s )
"aware bhvm_lop_ar2"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func ^:k_vs_cv;"
    "func bhvm_lop_ar2:k_vv_cv;"
"}";

void bhvm_lop_ar2_add_s_k_vs_cv( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_add_s_f2(((f2_t*)a)[i],((f2_t*)b)[0]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_add_s_f2(((f2_t*)a)[i],((f2_t*)b)[0]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_add_s_f3(((f2_t*)a)[i],((f3_t*)b)[0]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_add_s_f3(((f2_t*)a)[i],((f3_t*)b)[0]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_add_s_f3(((f3_t*)a)[i],((f2_t*)b)[0]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_add_s_f3(((f3_t*)a)[i],((f2_t*)b)[0]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_add_s_f3(((f3_t*)a)[i],((f3_t*)b)[0]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_add_s_f3(((f3_t*)a)[i],((f3_t*)b)[0]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

void bhvm_lop_ar2_add_s_k_vv_cv( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_add_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_add_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_add_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_add_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_add_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_add_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_add_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_add_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_sub_s )
"aware bhvm_lop_ar2"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func ^:k_vs_cv;"
    "func bhvm_lop_ar2:k_vv_cv;"
"}";

void bhvm_lop_ar2_sub_s_k_vs_cv( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_sub_s_f2(((f2_t*)a)[i],((f2_t*)b)[0]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_sub_s_f2(((f2_t*)a)[i],((f2_t*)b)[0]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_sub_s_f3(((f2_t*)a)[i],((f3_t*)b)[0]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_sub_s_f3(((f2_t*)a)[i],((f3_t*)b)[0]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_sub_s_f3(((f3_t*)a)[i],((f2_t*)b)[0]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_sub_s_f3(((f3_t*)a)[i],((f2_t*)b)[0]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_sub_s_f3(((f3_t*)a)[i],((f3_t*)b)[0]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_sub_s_f3(((f3_t*)a)[i],((f3_t*)b)[0]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

void bhvm_lop_ar2_sub_s_k_vv_cv( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_sub_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_sub_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_sub_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_sub_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_sub_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_sub_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_sub_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_sub_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_mul_s )
"aware bhvm_lop_ar2"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func ^:k_vs_cv;"
    "func ^:k_vv_as;"
    "func ^:k_vv_av;"
    "func ^:k_vs_av;"
    "func bhvm_lop_ar2:k_vv_cv;"
"}";

void bhvm_lop_ar2_mul_s_k_vs_cv( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_mul_s_f2(((f2_t*)a)[i],((f2_t*)b)[0]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_mul_s_f2(((f2_t*)a)[i],((f2_t*)b)[0]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_mul_s_f3(((f2_t*)a)[i],((f3_t*)b)[0]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_mul_s_f3(((f2_t*)a)[i],((f3_t*)b)[0]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_mul_s_f3(((f3_t*)a)[i],((f2_t*)b)[0]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_mul_s_f3(((f3_t*)a)[i],((f2_t*)b)[0]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_mul_s_f3(((f3_t*)a)[i],((f3_t*)b)[0]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_mul_s_f3(((f3_t*)a)[i],((f3_t*)b)[0]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

void bhvm_lop_ar2_mul_s_k_vv_as( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[0]+=bhvm_lop_ar2_mul_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[0]+=bhvm_lop_ar2_mul_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[0]+=bhvm_lop_ar2_mul_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[0]+=bhvm_lop_ar2_mul_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[0]+=bhvm_lop_ar2_mul_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[0]+=bhvm_lop_ar2_mul_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[0]+=bhvm_lop_ar2_mul_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[0]+=bhvm_lop_ar2_mul_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

void bhvm_lop_ar2_mul_s_k_vv_av( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_mul_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_mul_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_mul_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_mul_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_mul_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_mul_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_mul_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_mul_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

void bhvm_lop_ar2_mul_s_k_vs_av( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_mul_s_f2(((f2_t*)a)[i],((f2_t*)b)[0]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_mul_s_f2(((f2_t*)a)[i],((f2_t*)b)[0]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_mul_s_f3(((f2_t*)a)[i],((f3_t*)b)[0]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_mul_s_f3(((f2_t*)a)[i],((f3_t*)b)[0]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_mul_s_f3(((f3_t*)a)[i],((f2_t*)b)[0]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_mul_s_f3(((f3_t*)a)[i],((f2_t*)b)[0]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_mul_s_f3(((f3_t*)a)[i],((f3_t*)b)[0]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_mul_s_f3(((f3_t*)a)[i],((f3_t*)b)[0]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

void bhvm_lop_ar2_mul_s_k_vv_cv( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_mul_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_mul_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_mul_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_mul_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_mul_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_mul_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_mul_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_mul_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_mul_dp_a_vb_s )
"aware bhvm_lop_ar2"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func ^:k_vv_av;"
    "func bhvm_lop_ar2:k_vv_cv;"
"}";

void bhvm_lop_ar2_mul_dp_a_vb_s_k_vv_av( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_mul_dp_a_vb_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_mul_dp_a_vb_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_mul_dp_a_vb_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_mul_dp_a_vb_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_mul_dp_a_vb_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_mul_dp_a_vb_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_mul_dp_a_vb_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_mul_dp_a_vb_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

void bhvm_lop_ar2_mul_dp_a_vb_s_k_vv_cv( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_mul_dp_a_vb_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_mul_dp_a_vb_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_mul_dp_a_vb_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_mul_dp_a_vb_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_mul_dp_a_vb_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_mul_dp_a_vb_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_mul_dp_a_vb_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_mul_dp_a_vb_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_mul_dp_b_va_s )
"aware bhvm_lop_ar2"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func ^:k_vv_av;"
    "func bhvm_lop_ar2:k_vv_cv;"
"}";

void bhvm_lop_ar2_mul_dp_b_va_s_k_vv_av( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_mul_dp_b_va_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_mul_dp_b_va_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_mul_dp_b_va_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_mul_dp_b_va_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_mul_dp_b_va_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_mul_dp_b_va_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_mul_dp_b_va_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_mul_dp_b_va_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

void bhvm_lop_ar2_mul_dp_b_va_s_k_vv_cv( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_mul_dp_b_va_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_mul_dp_b_va_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_mul_dp_b_va_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_mul_dp_b_va_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_mul_dp_b_va_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_mul_dp_b_va_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_mul_dp_b_va_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_mul_dp_b_va_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_div_s )
"aware bhvm_lop_ar2"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func bhvm_lop_ar2:k_vv_cv;"
"}";

void bhvm_lop_ar2_div_s_k_vv_cv( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_div_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_div_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_div_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_div_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_div_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_div_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_div_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_div_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_div_dp_a_vb_s )
"aware bhvm_lop_ar2"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func ^:k_vv_av;"
    "func bhvm_lop_ar2:k_vv_cv;"
"}";

void bhvm_lop_ar2_div_dp_a_vb_s_k_vv_av( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_div_dp_a_vb_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_div_dp_a_vb_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_div_dp_a_vb_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_div_dp_a_vb_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_div_dp_a_vb_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_div_dp_a_vb_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_div_dp_a_vb_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_div_dp_a_vb_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

void bhvm_lop_ar2_div_dp_a_vb_s_k_vv_cv( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_div_dp_a_vb_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_div_dp_a_vb_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_div_dp_a_vb_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_div_dp_a_vb_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_div_dp_a_vb_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_div_dp_a_vb_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_div_dp_a_vb_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_div_dp_a_vb_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_exp_dp_vy_s )
"aware bhvm_lop_ar2"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func ^:k_vv_av;"
    "func bhvm_lop_ar2:k_vv_cv;"
"}";

void bhvm_lop_ar2_exp_dp_vy_s_k_vv_av( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_exp_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_exp_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_exp_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_exp_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_exp_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_exp_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_exp_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_exp_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

void bhvm_lop_ar2_exp_dp_vy_s_k_vv_cv( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_exp_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_exp_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_exp_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_exp_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_exp_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_exp_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_exp_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_exp_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_inv_dp_vy_s )
"aware bhvm_lop_ar2"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func ^:k_vv_av;"
    "func bhvm_lop_ar2:k_vv_cv;"
"}";

void bhvm_lop_ar2_inv_dp_vy_s_k_vv_av( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_inv_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_inv_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_inv_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_inv_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_inv_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_inv_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_inv_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_inv_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

void bhvm_lop_ar2_inv_dp_vy_s_k_vv_cv( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_inv_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_inv_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_inv_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_inv_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_inv_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_inv_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_inv_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_inv_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_lgst_dp_vy_s )
"aware bhvm_lop_ar2"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func ^:k_vv_av;"
    "func bhvm_lop_ar2:k_vv_cv;"
"}";

void bhvm_lop_ar2_lgst_dp_vy_s_k_vv_av( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_lgst_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_lgst_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_lgst_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_lgst_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_lgst_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_lgst_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_lgst_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_lgst_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

void bhvm_lop_ar2_lgst_dp_vy_s_k_vv_cv( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_lgst_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_lgst_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_lgst_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_lgst_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_lgst_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_lgst_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_lgst_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_lgst_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_lgst_hard_dp_vy_s )
"aware bhvm_lop_ar2"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func ^:k_vv_av;"
    "func bhvm_lop_ar2:k_vv_cv;"
"}";

void bhvm_lop_ar2_lgst_hard_dp_vy_s_k_vv_av( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_lgst_hard_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_lgst_hard_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_lgst_hard_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_lgst_hard_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_lgst_hard_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_lgst_hard_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_lgst_hard_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_lgst_hard_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

void bhvm_lop_ar2_lgst_hard_dp_vy_s_k_vv_cv( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_lgst_hard_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_lgst_hard_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_lgst_hard_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_lgst_hard_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_lgst_hard_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_lgst_hard_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_lgst_hard_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_lgst_hard_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_lgst_leaky_dp_vy_s )
"aware bhvm_lop_ar2"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func ^:k_vv_av;"
    "func bhvm_lop_ar2:k_vv_cv;"
"}";

void bhvm_lop_ar2_lgst_leaky_dp_vy_s_k_vv_av( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_lgst_leaky_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_lgst_leaky_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_lgst_leaky_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_lgst_leaky_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_lgst_leaky_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_lgst_leaky_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_lgst_leaky_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_lgst_leaky_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

void bhvm_lop_ar2_lgst_leaky_dp_vy_s_k_vv_cv( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_lgst_leaky_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_lgst_leaky_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_lgst_leaky_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_lgst_leaky_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_lgst_leaky_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_lgst_leaky_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_lgst_leaky_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_lgst_leaky_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_tanh_dp_vy_s )
"aware bhvm_lop_ar2"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func ^:k_vv_av;"
    "func bhvm_lop_ar2:k_vv_cv;"
"}";

void bhvm_lop_ar2_tanh_dp_vy_s_k_vv_av( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_tanh_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_tanh_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_tanh_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_tanh_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_tanh_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_tanh_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_tanh_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_tanh_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

void bhvm_lop_ar2_tanh_dp_vy_s_k_vv_cv( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_tanh_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_tanh_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_tanh_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_tanh_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_tanh_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_tanh_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_tanh_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_tanh_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_tanh_hard_dp_vy_s )
"aware bhvm_lop_ar2"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func ^:k_vv_av;"
    "func bhvm_lop_ar2:k_vv_cv;"
"}";

void bhvm_lop_ar2_tanh_hard_dp_vy_s_k_vv_av( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_tanh_hard_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_tanh_hard_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_tanh_hard_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_tanh_hard_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_tanh_hard_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_tanh_hard_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_tanh_hard_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_tanh_hard_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

void bhvm_lop_ar2_tanh_hard_dp_vy_s_k_vv_cv( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_tanh_hard_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_tanh_hard_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_tanh_hard_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_tanh_hard_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_tanh_hard_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_tanh_hard_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_tanh_hard_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_tanh_hard_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_tanh_leaky_dp_vy_s )
"aware bhvm_lop_ar2"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func ^:k_vv_av;"
    "func bhvm_lop_ar2:k_vv_cv;"
"}";

void bhvm_lop_ar2_tanh_leaky_dp_vy_s_k_vv_av( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_tanh_leaky_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_tanh_leaky_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_tanh_leaky_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_tanh_leaky_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_tanh_leaky_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_tanh_leaky_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_tanh_leaky_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_tanh_leaky_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

void bhvm_lop_ar2_tanh_leaky_dp_vy_s_k_vv_cv( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_tanh_leaky_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_tanh_leaky_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_tanh_leaky_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_tanh_leaky_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_tanh_leaky_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_tanh_leaky_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_tanh_leaky_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_tanh_leaky_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_softplus_dp_vy_s )
"aware bhvm_lop_ar2"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func ^:k_vv_av;"
    "func bhvm_lop_ar2:k_vv_cv;"
"}";

void bhvm_lop_ar2_softplus_dp_vy_s_k_vv_av( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_softplus_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_softplus_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_softplus_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_softplus_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_softplus_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_softplus_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_softplus_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_softplus_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

void bhvm_lop_ar2_softplus_dp_vy_s_k_vv_cv( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_softplus_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_softplus_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_softplus_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_softplus_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_softplus_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_softplus_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_softplus_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_softplus_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_relu_dp_vy_s )
"aware bhvm_lop_ar2"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func ^:k_vv_av;"
    "func bhvm_lop_ar2:k_vv_cv;"
"}";

void bhvm_lop_ar2_relu_dp_vy_s_k_vv_av( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_relu_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_relu_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_relu_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_relu_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_relu_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_relu_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_relu_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_relu_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

void bhvm_lop_ar2_relu_dp_vy_s_k_vv_cv( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_relu_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_relu_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_relu_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_relu_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_relu_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_relu_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_relu_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_relu_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_relu_leaky_dp_vy_s )
"aware bhvm_lop_ar2"
"{"
    "func ^:f2;"
    "func ^:f3;"
    "func ^:k_vv_av;"
    "func bhvm_lop_ar2:k_vv_cv;"
"}";

void bhvm_lop_ar2_relu_leaky_dp_vy_s_k_vv_av( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_relu_leaky_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_relu_leaky_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_relu_leaky_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_relu_leaky_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_relu_leaky_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_relu_leaky_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=bhvm_lop_ar2_relu_leaky_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=bhvm_lop_ar2_relu_leaky_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

void bhvm_lop_ar2_relu_leaky_dp_vy_s_k_vv_cv( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_relu_leaky_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_relu_leaky_dp_vy_s_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_relu_leaky_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_relu_leaky_dp_vy_s_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_relu_leaky_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_relu_leaky_dp_vy_s_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=bhvm_lop_ar2_relu_leaky_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=bhvm_lop_ar2_relu_leaky_dp_vy_s_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

/**********************************************************************************************************************/

vd_t bhvm_planted_lop_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bhvm_planted_lop" ) ) )
    {
        case TYPEOF_init1:
        {
            // Comment or remove line below to rebuild this target.
            bcore_const_x_set_d( typeof( "bhvm_planted_lop_hash" ), sr_tp( 3615917369 ) );

            // --------------------------------------------------------------------
            // source: bhvm_lop.h

            // group: bhvm_lop
            BCORE_REGISTER_TRAIT( bhvm_lop, bcore_inst );

            // group: bhvm_lop_ar0
            BCORE_REGISTER_FFUNC( bhvm_lop_ar0_f2, bhvm_lop_ar0_zro_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar0_f3, bhvm_lop_ar0_zro_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar0_k_cv, bhvm_lop_ar0_zro_s_k_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar0_zro_s );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar0_f2, bhvm_lop_ar0_one_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar0_f3, bhvm_lop_ar0_one_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar0_k_cv, bhvm_lop_ar0_one_s_k_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar0_one_s );
            BCORE_REGISTER_TRAIT( bhvm_lop_ar0, bhvm_lop );

            // group: bhvm_lop_ar1
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f2, bhvm_lop_ar1_identity_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f3, bhvm_lop_ar1_identity_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_k_s_cv, bhvm_lop_ar1_identity_s_k_s_cv );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_k_v_cv, bhvm_lop_ar1_identity_s_k_v_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_identity_s );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f2, bhvm_lop_ar1_neg_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f3, bhvm_lop_ar1_neg_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_k_v_cv, bhvm_lop_ar1_neg_s_k_v_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_neg_s );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f2, bhvm_lop_ar1_floor_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f3, bhvm_lop_ar1_floor_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_k_v_cv, bhvm_lop_ar1_floor_s_k_v_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_floor_s );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f2, bhvm_lop_ar1_ceil_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f3, bhvm_lop_ar1_ceil_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_k_v_cv, bhvm_lop_ar1_ceil_s_k_v_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_ceil_s );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f2, bhvm_lop_ar1_exp_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f3, bhvm_lop_ar1_exp_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_k_v_cv, bhvm_lop_ar1_exp_s_k_v_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_exp_s );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f2, bhvm_lop_ar1_inv_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f3, bhvm_lop_ar1_inv_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_k_v_cv, bhvm_lop_ar1_inv_s_k_v_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_inv_s );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f2, bhvm_lop_ar1_add_dp_a_v_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f3, bhvm_lop_ar1_add_dp_a_v_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_k_v_av, bhvm_lop_ar1_add_dp_a_v_s_k_v_av );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_k_v_cv, bhvm_lop_ar1_add_dp_a_v_s_k_v_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_add_dp_a_v_s );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f2, bhvm_lop_ar1_add_dp_b_v_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f3, bhvm_lop_ar1_add_dp_b_v_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_k_v_av, bhvm_lop_ar1_add_dp_b_v_s_k_v_av );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_k_v_cv, bhvm_lop_ar1_add_dp_b_v_s_k_v_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_add_dp_b_v_s );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f2, bhvm_lop_ar1_sub_dp_a_v_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f3, bhvm_lop_ar1_sub_dp_a_v_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_k_v_av, bhvm_lop_ar1_sub_dp_a_v_s_k_v_av );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_k_v_cv, bhvm_lop_ar1_sub_dp_a_v_s_k_v_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_sub_dp_a_v_s );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f2, bhvm_lop_ar1_sub_dp_b_v_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f3, bhvm_lop_ar1_sub_dp_b_v_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_k_v_av, bhvm_lop_ar1_sub_dp_b_v_s_k_v_av );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_k_v_cv, bhvm_lop_ar1_sub_dp_b_v_s_k_v_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_sub_dp_b_v_s );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f2, bhvm_lop_ar1_lgst_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f3, bhvm_lop_ar1_lgst_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_k_v_cv, bhvm_lop_ar1_lgst_s_k_v_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_lgst_s );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f2, bhvm_lop_ar1_lgst_hard_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f3, bhvm_lop_ar1_lgst_hard_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_k_v_cv, bhvm_lop_ar1_lgst_hard_s_k_v_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_lgst_hard_s );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f2, bhvm_lop_ar1_lgst_leaky_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f3, bhvm_lop_ar1_lgst_leaky_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_k_v_cv, bhvm_lop_ar1_lgst_leaky_s_k_v_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_lgst_leaky_s );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f2, bhvm_lop_ar1_tanh_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f3, bhvm_lop_ar1_tanh_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_k_v_cv, bhvm_lop_ar1_tanh_s_k_v_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_tanh_s );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f2, bhvm_lop_ar1_tanh_hard_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f3, bhvm_lop_ar1_tanh_hard_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_k_v_cv, bhvm_lop_ar1_tanh_hard_s_k_v_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_tanh_hard_s );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f2, bhvm_lop_ar1_tanh_leaky_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f3, bhvm_lop_ar1_tanh_leaky_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_k_v_cv, bhvm_lop_ar1_tanh_leaky_s_k_v_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_tanh_leaky_s );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f2, bhvm_lop_ar1_softplus_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f3, bhvm_lop_ar1_softplus_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_k_v_cv, bhvm_lop_ar1_softplus_s_k_v_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_softplus_s );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f2, bhvm_lop_ar1_relu_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f3, bhvm_lop_ar1_relu_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_k_v_cv, bhvm_lop_ar1_relu_s_k_v_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_relu_s );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f2, bhvm_lop_ar1_relu_leaky_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_f3, bhvm_lop_ar1_relu_leaky_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar1_k_v_cv, bhvm_lop_ar1_relu_leaky_s_k_v_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_relu_leaky_s );
            BCORE_REGISTER_TRAIT( bhvm_lop_ar1, bhvm_lop );

            // group: bhvm_lop_ar2
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_f2, bhvm_lop_ar2_add_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_f3, bhvm_lop_ar2_add_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vs_cv, bhvm_lop_ar2_add_s_k_vs_cv );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vv_cv, bhvm_lop_ar2_add_s_k_vv_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_add_s );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_f2, bhvm_lop_ar2_sub_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_f3, bhvm_lop_ar2_sub_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vs_cv, bhvm_lop_ar2_sub_s_k_vs_cv );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vv_cv, bhvm_lop_ar2_sub_s_k_vv_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_sub_s );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_f2, bhvm_lop_ar2_mul_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_f3, bhvm_lop_ar2_mul_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vs_cv, bhvm_lop_ar2_mul_s_k_vs_cv );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vv_as, bhvm_lop_ar2_mul_s_k_vv_as );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vv_av, bhvm_lop_ar2_mul_s_k_vv_av );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vs_av, bhvm_lop_ar2_mul_s_k_vs_av );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vv_cv, bhvm_lop_ar2_mul_s_k_vv_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_mul_s );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_f2, bhvm_lop_ar2_mul_dp_a_vb_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_f3, bhvm_lop_ar2_mul_dp_a_vb_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vv_av, bhvm_lop_ar2_mul_dp_a_vb_s_k_vv_av );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vv_cv, bhvm_lop_ar2_mul_dp_a_vb_s_k_vv_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_mul_dp_a_vb_s );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_f2, bhvm_lop_ar2_mul_dp_b_va_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_f3, bhvm_lop_ar2_mul_dp_b_va_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vv_av, bhvm_lop_ar2_mul_dp_b_va_s_k_vv_av );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vv_cv, bhvm_lop_ar2_mul_dp_b_va_s_k_vv_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_mul_dp_b_va_s );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_f2, bhvm_lop_ar2_div_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_f3, bhvm_lop_ar2_div_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vv_cv, bhvm_lop_ar2_div_s_k_vv_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_div_s );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_f2, bhvm_lop_ar2_div_dp_a_vb_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_f3, bhvm_lop_ar2_div_dp_a_vb_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vv_av, bhvm_lop_ar2_div_dp_a_vb_s_k_vv_av );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vv_cv, bhvm_lop_ar2_div_dp_a_vb_s_k_vv_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_div_dp_a_vb_s );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_f2, bhvm_lop_ar2_exp_dp_vy_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_f3, bhvm_lop_ar2_exp_dp_vy_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vv_av, bhvm_lop_ar2_exp_dp_vy_s_k_vv_av );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vv_cv, bhvm_lop_ar2_exp_dp_vy_s_k_vv_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_exp_dp_vy_s );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_f2, bhvm_lop_ar2_inv_dp_vy_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_f3, bhvm_lop_ar2_inv_dp_vy_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vv_av, bhvm_lop_ar2_inv_dp_vy_s_k_vv_av );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vv_cv, bhvm_lop_ar2_inv_dp_vy_s_k_vv_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_inv_dp_vy_s );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_f2, bhvm_lop_ar2_lgst_dp_vy_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_f3, bhvm_lop_ar2_lgst_dp_vy_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vv_av, bhvm_lop_ar2_lgst_dp_vy_s_k_vv_av );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vv_cv, bhvm_lop_ar2_lgst_dp_vy_s_k_vv_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_lgst_dp_vy_s );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_f2, bhvm_lop_ar2_lgst_hard_dp_vy_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_f3, bhvm_lop_ar2_lgst_hard_dp_vy_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vv_av, bhvm_lop_ar2_lgst_hard_dp_vy_s_k_vv_av );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vv_cv, bhvm_lop_ar2_lgst_hard_dp_vy_s_k_vv_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_lgst_hard_dp_vy_s );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_f2, bhvm_lop_ar2_lgst_leaky_dp_vy_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_f3, bhvm_lop_ar2_lgst_leaky_dp_vy_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vv_av, bhvm_lop_ar2_lgst_leaky_dp_vy_s_k_vv_av );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vv_cv, bhvm_lop_ar2_lgst_leaky_dp_vy_s_k_vv_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_lgst_leaky_dp_vy_s );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_f2, bhvm_lop_ar2_tanh_dp_vy_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_f3, bhvm_lop_ar2_tanh_dp_vy_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vv_av, bhvm_lop_ar2_tanh_dp_vy_s_k_vv_av );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vv_cv, bhvm_lop_ar2_tanh_dp_vy_s_k_vv_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_tanh_dp_vy_s );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_f2, bhvm_lop_ar2_tanh_hard_dp_vy_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_f3, bhvm_lop_ar2_tanh_hard_dp_vy_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vv_av, bhvm_lop_ar2_tanh_hard_dp_vy_s_k_vv_av );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vv_cv, bhvm_lop_ar2_tanh_hard_dp_vy_s_k_vv_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_tanh_hard_dp_vy_s );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_f2, bhvm_lop_ar2_tanh_leaky_dp_vy_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_f3, bhvm_lop_ar2_tanh_leaky_dp_vy_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vv_av, bhvm_lop_ar2_tanh_leaky_dp_vy_s_k_vv_av );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vv_cv, bhvm_lop_ar2_tanh_leaky_dp_vy_s_k_vv_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_tanh_leaky_dp_vy_s );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_f2, bhvm_lop_ar2_softplus_dp_vy_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_f3, bhvm_lop_ar2_softplus_dp_vy_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vv_av, bhvm_lop_ar2_softplus_dp_vy_s_k_vv_av );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vv_cv, bhvm_lop_ar2_softplus_dp_vy_s_k_vv_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_softplus_dp_vy_s );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_f2, bhvm_lop_ar2_relu_dp_vy_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_f3, bhvm_lop_ar2_relu_dp_vy_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vv_av, bhvm_lop_ar2_relu_dp_vy_s_k_vv_av );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vv_cv, bhvm_lop_ar2_relu_dp_vy_s_k_vv_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_relu_dp_vy_s );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_f2, bhvm_lop_ar2_relu_leaky_dp_vy_s_f2 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_f3, bhvm_lop_ar2_relu_leaky_dp_vy_s_f3 );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vv_av, bhvm_lop_ar2_relu_leaky_dp_vy_s_k_vv_av );
            BCORE_REGISTER_FFUNC( bhvm_lop_ar2_k_vv_cv, bhvm_lop_ar2_relu_leaky_dp_vy_s_k_vv_cv );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_relu_leaky_dp_vy_s );
            BCORE_REGISTER_TRAIT( bhvm_lop_ar2, bhvm_lop );
        }
        break;
        default: break;
    }
    return NULL;
}
