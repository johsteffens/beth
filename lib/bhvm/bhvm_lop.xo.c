/** This file was generated from xoila source code.
 *  Compiling Agent : xoico_compiler (C) 2020 J.B.Steffens
 *  Last File Update: 2020-12-01T10:47:36Z
 *
 *  Copyright and License of this File:
 *
 *  Generated code inherits the copyright and license of the underlying xoila source code.
 *  Source code defining this file is distributed across following files:
 *
 *  bhvm_lop.h
 *  bhvm_lop.x
 *
 */

#include "bhvm_lop.xo.h"
#include "bcore_spect.h"
#include "bcore_spect_inst.h"
#include "bcore_sr.h"
#include "bcore_const_manager.h"


/**********************************************************************************************************************/
// source: bhvm_lop.h
#include "bhvm_lop.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_lop; embeds: bhvm_lop.x

XOILA_DEFINE_SPECT( bcore_inst, bhvm_lop )
"{"
    "bcore_spect_header_s header;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_lop_ar0

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar0_zro_s )
"aware bhvm_lop_ar0"
"{"
"}";

void bhvm_lop_ar0_zro_s_f( tp_t tknit, vd_t r, sz_t s )
{
    // bhvm_lop.x:48:5
    
    assert( s == 0 || r );
    switch( tknit )
    {
        case BKNIT_F2: { f2_t v = bhvm_lop_ar0_zro_s_f2(); for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=v; } } break;
        case BKNIT_F3: { f3_t v = bhvm_lop_ar0_zro_s_f3(); for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=v; } } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar0_one_s )
"aware bhvm_lop_ar0"
"{"
"}";

void bhvm_lop_ar0_one_s_f( tp_t tknit, vd_t r, sz_t s )
{
    // bhvm_lop.x:48:5
    
    assert( s == 0 || r );
    switch( tknit )
    {
        case BKNIT_F2: { f2_t v = bhvm_lop_ar0_one_s_f2(); for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=v; } } break;
        case BKNIT_F3: { f3_t v = bhvm_lop_ar0_one_s_f3(); for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=v; } } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar0_nul_dp_v_s )
"aware bhvm_lop_ar0"
"{"
"}";

XOILA_DEFINE_SPECT( bhvm_lop, bhvm_lop_ar0 )
"{"
    "bcore_spect_header_s header;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_lop_ar1

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_cpy_s )
"aware bhvm_lop_ar1"
"{"
"}";

void bhvm_lop_ar1_cpy_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    // bhvm_lop.x:80:5
    
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_cpy_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_cpy_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_cpy_s_f3(((f3_t*)(a))[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_cpy_s_f3(((f3_t*)(a))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_identity_s )
"aware bhvm_lop_ar1"
"{"
"}";

void bhvm_lop_ar1_identity_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    // bhvm_lop.x:80:5
    
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_identity_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_identity_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_identity_s_f3(((f3_t*)(a))[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_identity_s_f3(((f3_t*)(a))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_neg_s )
"aware bhvm_lop_ar1"
"{"
"}";

void bhvm_lop_ar1_neg_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    // bhvm_lop.x:80:5
    
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_neg_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_neg_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_neg_s_f3(((f3_t*)(a))[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_neg_s_f3(((f3_t*)(a))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_floor_s )
"aware bhvm_lop_ar1"
"{"
"}";

void bhvm_lop_ar1_floor_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    // bhvm_lop.x:80:5
    
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_floor_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_floor_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_floor_s_f3(((f3_t*)(a))[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_floor_s_f3(((f3_t*)(a))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_ceil_s )
"aware bhvm_lop_ar1"
"{"
"}";

void bhvm_lop_ar1_ceil_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    // bhvm_lop.x:80:5
    
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_ceil_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_ceil_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_ceil_s_f3(((f3_t*)(a))[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_ceil_s_f3(((f3_t*)(a))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_exp_s )
"aware bhvm_lop_ar1"
"{"
"}";

void bhvm_lop_ar1_exp_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    // bhvm_lop.x:80:5
    
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_exp_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_exp_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_exp_s_f3(((f3_t*)(a))[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_exp_s_f3(((f3_t*)(a))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_log_s )
"aware bhvm_lop_ar1"
"{"
"}";

void bhvm_lop_ar1_log_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    // bhvm_lop.x:80:5
    
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_log_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_log_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_log_s_f3(((f3_t*)(a))[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_log_s_f3(((f3_t*)(a))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_inv_s )
"aware bhvm_lop_ar1"
"{"
"}";

void bhvm_lop_ar1_inv_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    // bhvm_lop.x:80:5
    
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_inv_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_inv_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_inv_s_f3(((f3_t*)(a))[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_inv_s_f3(((f3_t*)(a))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_abs_s )
"aware bhvm_lop_ar1"
"{"
"}";

void bhvm_lop_ar1_abs_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    // bhvm_lop.x:80:5
    
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_abs_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_abs_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_abs_s_f3(((f3_t*)(a))[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_abs_s_f3(((f3_t*)(a))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_sig_s )
"aware bhvm_lop_ar1"
"{"
"}";

void bhvm_lop_ar1_sig_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    // bhvm_lop.x:80:5
    
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_sig_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_sig_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_sig_s_f3(((f3_t*)(a))[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_sig_s_f3(((f3_t*)(a))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_l1_s )
"aware bhvm_lop_ar1"
"{"
"}";

void bhvm_lop_ar1_l1_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    // bhvm_lop.x:122:5
    
    assert( a && r );
    switch( tknit )
    {
        case BKNIT_F22: ((f2_t*)(r))[0] = 0; for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[0]+=bhvm_lop_ar1_l1_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F23: ((f3_t*)(r))[0] = 0; for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[0]+=bhvm_lop_ar1_l1_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F32: ((f2_t*)(r))[0] = 0; for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[0]+=bhvm_lop_ar1_l1_s_f3(((f3_t*)(a))[i]); } break;
        case BKNIT_F33: ((f3_t*)(r))[0] = 0; for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[0]+=bhvm_lop_ar1_l1_s_f3(((f3_t*)(a))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_sqr_s )
"aware bhvm_lop_ar1"
"{"
"}";

void bhvm_lop_ar1_sqr_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    // bhvm_lop.x:80:5
    
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_sqr_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_sqr_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_sqr_s_f3(((f3_t*)(a))[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_sqr_s_f3(((f3_t*)(a))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_sqrsum_s )
"aware bhvm_lop_ar1"
"{"
"}";

void bhvm_lop_ar1_sqrsum_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    // bhvm_lop.x:122:5
    
    assert( a && r );
    switch( tknit )
    {
        case BKNIT_F22: ((f2_t*)(r))[0] = 0; for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[0]+=bhvm_lop_ar1_sqrsum_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F23: ((f3_t*)(r))[0] = 0; for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[0]+=bhvm_lop_ar1_sqrsum_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F32: ((f2_t*)(r))[0] = 0; for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[0]+=bhvm_lop_ar1_sqrsum_s_f3(((f3_t*)(a))[i]); } break;
        case BKNIT_F33: ((f3_t*)(r))[0] = 0; for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[0]+=bhvm_lop_ar1_sqrsum_s_f3(((f3_t*)(a))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_srt_s )
"aware bhvm_lop_ar1"
"{"
"}";

void bhvm_lop_ar1_srt_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    // bhvm_lop.x:80:5
    
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_srt_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_srt_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_srt_s_f3(((f3_t*)(a))[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_srt_s_f3(((f3_t*)(a))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_tanh_s )
"aware bhvm_lop_ar1"
"{"
"}";

void bhvm_lop_ar1_tanh_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    // bhvm_lop.x:80:5
    
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_tanh_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_tanh_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_tanh_s_f3(((f3_t*)(a))[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_tanh_s_f3(((f3_t*)(a))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_tanh_hard_s )
"aware bhvm_lop_ar1"
"{"
"}";

void bhvm_lop_ar1_tanh_hard_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    // bhvm_lop.x:80:5
    
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_tanh_hard_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_tanh_hard_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_tanh_hard_s_f3(((f3_t*)(a))[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_tanh_hard_s_f3(((f3_t*)(a))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_tanh_leaky_s )
"aware bhvm_lop_ar1"
"{"
"}";

void bhvm_lop_ar1_tanh_leaky_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    // bhvm_lop.x:80:5
    
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_tanh_leaky_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_tanh_leaky_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_tanh_leaky_s_f3(((f3_t*)(a))[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_tanh_leaky_s_f3(((f3_t*)(a))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_sigm_s )
"aware bhvm_lop_ar1"
"{"
"}";

void bhvm_lop_ar1_sigm_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    // bhvm_lop.x:80:5
    
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_sigm_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_sigm_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_sigm_s_f3(((f3_t*)(a))[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_sigm_s_f3(((f3_t*)(a))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_sigm_hard_s )
"aware bhvm_lop_ar1"
"{"
"}";

void bhvm_lop_ar1_sigm_hard_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    // bhvm_lop.x:80:5
    
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_sigm_hard_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_sigm_hard_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_sigm_hard_s_f3(((f3_t*)(a))[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_sigm_hard_s_f3(((f3_t*)(a))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_sigm_leaky_s )
"aware bhvm_lop_ar1"
"{"
"}";

void bhvm_lop_ar1_sigm_leaky_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    // bhvm_lop.x:80:5
    
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_sigm_leaky_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_sigm_leaky_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_sigm_leaky_s_f3(((f3_t*)(a))[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_sigm_leaky_s_f3(((f3_t*)(a))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_softplus_s )
"aware bhvm_lop_ar1"
"{"
"}";

void bhvm_lop_ar1_softplus_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    // bhvm_lop.x:80:5
    
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_softplus_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_softplus_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_softplus_s_f3(((f3_t*)(a))[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_softplus_s_f3(((f3_t*)(a))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_relu_s )
"aware bhvm_lop_ar1"
"{"
"}";

void bhvm_lop_ar1_relu_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    // bhvm_lop.x:80:5
    
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_relu_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_relu_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_relu_s_f3(((f3_t*)(a))[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_relu_s_f3(((f3_t*)(a))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_relu_leaky_s )
"aware bhvm_lop_ar1"
"{"
"}";

void bhvm_lop_ar1_relu_leaky_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    // bhvm_lop.x:80:5
    
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_relu_leaky_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_relu_leaky_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar1_relu_leaky_s_f3(((f3_t*)(a))[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar1_relu_leaky_s_f3(((f3_t*)(a))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_acc_s )
"aware bhvm_lop_ar1"
"{"
"}";

void bhvm_lop_ar1_acc_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    // bhvm_lop.x:94:5
    
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar1_acc_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar1_acc_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar1_acc_s_f3(((f3_t*)(a))[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar1_acc_s_f3(((f3_t*)(a))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar1_accn_s )
"aware bhvm_lop_ar1"
"{"
"}";

void bhvm_lop_ar1_accn_s_f( tp_t tknit, vc_t a, vd_t r, sz_t s )
{
    // bhvm_lop.x:94:5
    
    assert( s == 0 || ( a && r ) );
    switch( tknit )
    {
        case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar1_accn_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar1_accn_s_f2(((f2_t*)(a))[i]); } break;
        case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar1_accn_s_f3(((f3_t*)(a))[i]); } break;
        case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar1_accn_s_f3(((f3_t*)(a))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

XOILA_DEFINE_SPECT( bhvm_lop, bhvm_lop_ar1 )
"{"
    "bcore_spect_header_s header;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_lop_ar2

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_sub_sqrsum_s )
"aware bhvm_lop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_sub_l1_s )
"aware bhvm_lop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_mul_body_s )
"aware bcore_inst"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_mul_mvv_s )
"aware bhvm_lop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_mul_vmv_s )
"aware bhvm_lop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_mul_vvm_s )
"aware bhvm_lop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_mul_acc_mvv_s )
"aware bhvm_lop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_mul_acc_vmv_s )
"aware bhvm_lop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_mul_acc_vvm_s )
"aware bhvm_lop_ar2"
"{"
"}";

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_exp_dp_zyf_s )
"aware bhvm_lop_ar2"
"{"
"}";

void bhvm_lop_ar2_exp_dp_zyf_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    // bhvm_lop.x:212:5
    
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_exp_dp_zyf_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_exp_dp_zyf_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_exp_dp_zyf_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_exp_dp_zyf_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_exp_dp_zyf_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_exp_dp_zyf_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_exp_dp_zyf_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_exp_dp_zyf_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_log_dp_zaf_s )
"aware bhvm_lop_ar2"
"{"
"}";

void bhvm_lop_ar2_log_dp_zaf_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    // bhvm_lop.x:212:5
    
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_log_dp_zaf_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_log_dp_zaf_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_log_dp_zaf_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_log_dp_zaf_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_log_dp_zaf_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_log_dp_zaf_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_log_dp_zaf_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_log_dp_zaf_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_inv_dp_zyf_s )
"aware bhvm_lop_ar2"
"{"
"}";

void bhvm_lop_ar2_inv_dp_zyf_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    // bhvm_lop.x:212:5
    
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_inv_dp_zyf_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_inv_dp_zyf_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_inv_dp_zyf_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_inv_dp_zyf_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_inv_dp_zyf_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_inv_dp_zyf_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_inv_dp_zyf_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_inv_dp_zyf_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_sqr_dp_zaf_s )
"aware bhvm_lop_ar2"
"{"
"}";

void bhvm_lop_ar2_sqr_dp_zaf_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    // bhvm_lop.x:212:5
    
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_sqr_dp_zaf_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_sqr_dp_zaf_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_sqr_dp_zaf_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_sqr_dp_zaf_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_sqr_dp_zaf_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_sqr_dp_zaf_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_sqr_dp_zaf_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_sqr_dp_zaf_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_srt_dp_zyf_s )
"aware bhvm_lop_ar2"
"{"
"}";

void bhvm_lop_ar2_srt_dp_zyf_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    // bhvm_lop.x:212:5
    
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_srt_dp_zyf_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_srt_dp_zyf_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_srt_dp_zyf_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_srt_dp_zyf_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_srt_dp_zyf_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_srt_dp_zyf_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_srt_dp_zyf_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_srt_dp_zyf_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_abs_dp_zaf_s )
"aware bhvm_lop_ar2"
"{"
"}";

void bhvm_lop_ar2_abs_dp_zaf_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    // bhvm_lop.x:212:5
    
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_abs_dp_zaf_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_abs_dp_zaf_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_abs_dp_zaf_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_abs_dp_zaf_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_abs_dp_zaf_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_abs_dp_zaf_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_abs_dp_zaf_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_abs_dp_zaf_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_sigm_dp_zyf_s )
"aware bhvm_lop_ar2"
"{"
"}";

void bhvm_lop_ar2_sigm_dp_zyf_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    // bhvm_lop.x:212:5
    
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_sigm_dp_zyf_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_sigm_dp_zyf_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_sigm_dp_zyf_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_sigm_dp_zyf_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_sigm_dp_zyf_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_sigm_dp_zyf_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_sigm_dp_zyf_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_sigm_dp_zyf_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_sigm_hard_dp_zyf_s )
"aware bhvm_lop_ar2"
"{"
"}";

void bhvm_lop_ar2_sigm_hard_dp_zyf_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    // bhvm_lop.x:212:5
    
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_sigm_hard_dp_zyf_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_sigm_hard_dp_zyf_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_sigm_hard_dp_zyf_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_sigm_hard_dp_zyf_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_sigm_hard_dp_zyf_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_sigm_hard_dp_zyf_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_sigm_hard_dp_zyf_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_sigm_hard_dp_zyf_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_sigm_leaky_dp_zyf_s )
"aware bhvm_lop_ar2"
"{"
"}";

void bhvm_lop_ar2_sigm_leaky_dp_zyf_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    // bhvm_lop.x:212:5
    
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_sigm_leaky_dp_zyf_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_sigm_leaky_dp_zyf_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_sigm_leaky_dp_zyf_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_sigm_leaky_dp_zyf_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_sigm_leaky_dp_zyf_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_sigm_leaky_dp_zyf_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_sigm_leaky_dp_zyf_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_sigm_leaky_dp_zyf_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_tanh_dp_zyf_s )
"aware bhvm_lop_ar2"
"{"
"}";

void bhvm_lop_ar2_tanh_dp_zyf_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    // bhvm_lop.x:212:5
    
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_tanh_dp_zyf_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_tanh_dp_zyf_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_tanh_dp_zyf_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_tanh_dp_zyf_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_tanh_dp_zyf_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_tanh_dp_zyf_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_tanh_dp_zyf_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_tanh_dp_zyf_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_tanh_hard_dp_zyf_s )
"aware bhvm_lop_ar2"
"{"
"}";

void bhvm_lop_ar2_tanh_hard_dp_zyf_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    // bhvm_lop.x:212:5
    
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_tanh_hard_dp_zyf_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_tanh_hard_dp_zyf_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_tanh_hard_dp_zyf_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_tanh_hard_dp_zyf_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_tanh_hard_dp_zyf_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_tanh_hard_dp_zyf_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_tanh_hard_dp_zyf_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_tanh_hard_dp_zyf_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_tanh_leaky_dp_zyf_s )
"aware bhvm_lop_ar2"
"{"
"}";

void bhvm_lop_ar2_tanh_leaky_dp_zyf_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    // bhvm_lop.x:212:5
    
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_tanh_leaky_dp_zyf_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_tanh_leaky_dp_zyf_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_tanh_leaky_dp_zyf_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_tanh_leaky_dp_zyf_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_tanh_leaky_dp_zyf_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_tanh_leaky_dp_zyf_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_tanh_leaky_dp_zyf_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_tanh_leaky_dp_zyf_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_softplus_dp_zyf_s )
"aware bhvm_lop_ar2"
"{"
"}";

void bhvm_lop_ar2_softplus_dp_zyf_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    // bhvm_lop.x:212:5
    
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_softplus_dp_zyf_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_softplus_dp_zyf_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_softplus_dp_zyf_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_softplus_dp_zyf_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_softplus_dp_zyf_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_softplus_dp_zyf_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_softplus_dp_zyf_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_softplus_dp_zyf_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_relu_dp_zyf_s )
"aware bhvm_lop_ar2"
"{"
"}";

void bhvm_lop_ar2_relu_dp_zyf_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    // bhvm_lop.x:212:5
    
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_relu_dp_zyf_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_relu_dp_zyf_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_relu_dp_zyf_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_relu_dp_zyf_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_relu_dp_zyf_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_relu_dp_zyf_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_relu_dp_zyf_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_relu_dp_zyf_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_relu_leaky_dp_zyf_s )
"aware bhvm_lop_ar2"
"{"
"}";

void bhvm_lop_ar2_relu_leaky_dp_zyf_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    // bhvm_lop.x:212:5
    
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_relu_leaky_dp_zyf_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_relu_leaky_dp_zyf_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_relu_leaky_dp_zyf_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_relu_leaky_dp_zyf_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_relu_leaky_dp_zyf_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_relu_leaky_dp_zyf_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_relu_leaky_dp_zyf_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_relu_leaky_dp_zyf_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_branch_vvvv_dp_azg_s )
"aware bhvm_lop_ar2"
"{"
"}";

void bhvm_lop_ar2_branch_vvvv_dp_azg_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    // bhvm_lop.x:212:5
    
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_branch_vvvv_dp_azg_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_branch_vvvv_dp_azg_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_branch_vvvv_dp_azg_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_branch_vvvv_dp_azg_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_branch_vvvv_dp_azg_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_branch_vvvv_dp_azg_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_branch_vvvv_dp_azg_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_branch_vvvv_dp_azg_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_branch_vvvv_dp_azh_s )
"aware bhvm_lop_ar2"
"{"
"}";

void bhvm_lop_ar2_branch_vvvv_dp_azh_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    // bhvm_lop.x:212:5
    
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_branch_vvvv_dp_azh_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_branch_vvvv_dp_azh_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_branch_vvvv_dp_azh_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_branch_vvvv_dp_azh_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_branch_vvvv_dp_azh_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_branch_vvvv_dp_azh_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]+=bhvm_lop_ar2_branch_vvvv_dp_azh_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]+=bhvm_lop_ar2_branch_vvvv_dp_azh_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_equal_s )
"aware bhvm_lop_ar2"
"{"
"}";

void bhvm_lop_ar2_equal_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    // bhvm_lop.x:194:5
    
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar2_equal_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar2_equal_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar2_equal_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar2_equal_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar2_equal_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar2_equal_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar2_equal_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar2_equal_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_unequal_s )
"aware bhvm_lop_ar2"
"{"
"}";

void bhvm_lop_ar2_unequal_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    // bhvm_lop.x:194:5
    
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar2_unequal_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar2_unequal_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar2_unequal_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar2_unequal_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar2_unequal_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar2_unequal_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar2_unequal_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar2_unequal_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_larger_s )
"aware bhvm_lop_ar2"
"{"
"}";

void bhvm_lop_ar2_larger_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    // bhvm_lop.x:194:5
    
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar2_larger_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar2_larger_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar2_larger_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar2_larger_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar2_larger_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar2_larger_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar2_larger_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar2_larger_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_smaller_s )
"aware bhvm_lop_ar2"
"{"
"}";

void bhvm_lop_ar2_smaller_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    // bhvm_lop.x:194:5
    
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar2_smaller_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar2_smaller_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar2_smaller_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar2_smaller_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar2_smaller_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar2_smaller_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar2_smaller_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar2_smaller_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_larger_equal_s )
"aware bhvm_lop_ar2"
"{"
"}";

void bhvm_lop_ar2_larger_equal_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    // bhvm_lop.x:194:5
    
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar2_larger_equal_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar2_larger_equal_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar2_larger_equal_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar2_larger_equal_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar2_larger_equal_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar2_larger_equal_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar2_larger_equal_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar2_larger_equal_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_smaller_equal_s )
"aware bhvm_lop_ar2"
"{"
"}";

void bhvm_lop_ar2_smaller_equal_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    // bhvm_lop.x:194:5
    
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar2_smaller_equal_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar2_smaller_equal_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar2_smaller_equal_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar2_smaller_equal_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar2_smaller_equal_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar2_smaller_equal_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar2_smaller_equal_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar2_smaller_equal_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_logic_and_s )
"aware bhvm_lop_ar2"
"{"
"}";

void bhvm_lop_ar2_logic_and_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    // bhvm_lop.x:194:5
    
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar2_logic_and_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar2_logic_and_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar2_logic_and_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar2_logic_and_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar2_logic_and_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar2_logic_and_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar2_logic_and_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar2_logic_and_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar2_logic_or_s )
"aware bhvm_lop_ar2"
"{"
"}";

void bhvm_lop_ar2_logic_or_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    // bhvm_lop.x:194:5
    
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar2_logic_or_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar2_logic_or_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar2_logic_or_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar2_logic_or_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar2_logic_or_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar2_logic_or_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i]); } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar2_logic_or_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar2_logic_or_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

XOILA_DEFINE_SPECT( bhvm_lop, bhvm_lop_ar2 )
"{"
    "bcore_spect_header_s header;"
"}";

//----------------------------------------------------------------------------------------------------------------------
// group: bhvm_lop_ar3

BCORE_DEFINE_OBJECT_INST_P( bhvm_lop_ar3_branch_vvvv_s )
"aware bhvm_lop_ar3"
"{"
"}";

void bhvm_lop_ar3_branch_vvvv_s_f( tp_t tknit, vc_t a, vc_t b, vc_t c, vd_t r, sz_t s )
{
    // bhvm_lop.x:366:5
    
    assert( s == 0 || ( a && b && c && r ) );
    switch( tknit )
    {
        case BKNIT_F2222: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar3_branch_vvvv_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i],((f2_t*)(c))[i]); } break;
        case BKNIT_F2223: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar3_branch_vvvv_s_f2(((f2_t*)(a))[i],((f2_t*)(b))[i],((f2_t*)(c))[i]); } break;
        case BKNIT_F2232: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar3_branch_vvvv_s_f3(((f2_t*)(a))[i],((f2_t*)(b))[i],((f3_t*)(c))[i]); } break;
        case BKNIT_F2233: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar3_branch_vvvv_s_f3(((f2_t*)(a))[i],((f2_t*)(b))[i],((f3_t*)(c))[i]); } break;
        case BKNIT_F2322: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar3_branch_vvvv_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i],((f2_t*)(c))[i]); } break;
        case BKNIT_F2323: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar3_branch_vvvv_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i],((f2_t*)(c))[i]); } break;
        case BKNIT_F2332: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar3_branch_vvvv_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i],((f3_t*)(c))[i]); } break;
        case BKNIT_F2333: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar3_branch_vvvv_s_f3(((f2_t*)(a))[i],((f3_t*)(b))[i],((f3_t*)(c))[i]); } break;
        case BKNIT_F3222: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar3_branch_vvvv_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i],((f2_t*)(c))[i]); } break;
        case BKNIT_F3223: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar3_branch_vvvv_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i],((f2_t*)(c))[i]); } break;
        case BKNIT_F3232: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar3_branch_vvvv_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i],((f3_t*)(c))[i]); } break;
        case BKNIT_F3233: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar3_branch_vvvv_s_f3(((f3_t*)(a))[i],((f2_t*)(b))[i],((f3_t*)(c))[i]); } break;
        case BKNIT_F3322: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar3_branch_vvvv_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i],((f2_t*)(c))[i]); } break;
        case BKNIT_F3323: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar3_branch_vvvv_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i],((f2_t*)(c))[i]); } break;
        case BKNIT_F3332: for(sz_t i=0; i<s; i++) { ((f2_t*)(r))[i]=bhvm_lop_ar3_branch_vvvv_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i],((f3_t*)(c))[i]); } break;
        case BKNIT_F3333: for(sz_t i=0; i<s; i++) { ((f3_t*)(r))[i]=bhvm_lop_ar3_branch_vvvv_s_f3(((f3_t*)(a))[i],((f3_t*)(b))[i],((f3_t*)(c))[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

XOILA_DEFINE_SPECT( bhvm_lop, bhvm_lop_ar3 )
"{"
    "bcore_spect_header_s header;"
"}";

/**********************************************************************************************************************/

vd_t bmath_general_signal_handler( const bcore_signal_s* o );

vd_t bhvm_lop_xo_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bhvm_lop_xo" ) ) )
    {
        case TYPEOF_init1:
        {

            // --------------------------------------------------------------------
            // source: bhvm_lop.h

            // group: bhvm_lop
            XOILA_REGISTER_SPECT( bhvm_lop );

            // group: bhvm_lop_ar0
            BCORE_REGISTER_OBJECT( bhvm_lop_ar0_zro_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar0_one_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar0_nul_dp_v_s );
            XOILA_REGISTER_SPECT( bhvm_lop_ar0 );

            // group: bhvm_lop_ar1
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_cpy_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_identity_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_neg_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_floor_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_ceil_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_exp_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_log_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_inv_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_abs_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_sig_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_l1_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_sqr_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_sqrsum_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_srt_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_tanh_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_tanh_hard_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_tanh_leaky_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_sigm_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_sigm_hard_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_sigm_leaky_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_softplus_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_relu_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_relu_leaky_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_acc_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar1_accn_s );
            XOILA_REGISTER_SPECT( bhvm_lop_ar1 );

            // group: bhvm_lop_ar2
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_sub_sqrsum_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_sub_l1_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_mul_body_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_mul_mvv_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_mul_vmv_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_mul_vvm_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_mul_acc_mvv_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_mul_acc_vmv_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_mul_acc_vvm_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_exp_dp_zyf_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_log_dp_zaf_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_inv_dp_zyf_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_sqr_dp_zaf_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_srt_dp_zyf_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_abs_dp_zaf_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_sigm_dp_zyf_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_sigm_hard_dp_zyf_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_sigm_leaky_dp_zyf_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_tanh_dp_zyf_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_tanh_hard_dp_zyf_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_tanh_leaky_dp_zyf_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_softplus_dp_zyf_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_relu_dp_zyf_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_relu_leaky_dp_zyf_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_branch_vvvv_dp_azg_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_branch_vvvv_dp_azh_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_equal_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_unequal_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_larger_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_smaller_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_larger_equal_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_smaller_equal_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_logic_and_s );
            BCORE_REGISTER_OBJECT( bhvm_lop_ar2_logic_or_s );
            XOILA_REGISTER_SPECT( bhvm_lop_ar2 );

            // group: bhvm_lop_ar3
            BCORE_REGISTER_OBJECT( bhvm_lop_ar3_branch_vvvv_s );
            XOILA_REGISTER_SPECT( bhvm_lop_ar3 );
        }
        break;
        case TYPEOF_push_dependencies:
        {
            ASSERT( o->object && ( *( aware_t* )o->object ) == TYPEOF_bcore_arr_fp_s );
            bcore_arr_fp_s* arr_fp = o->object;
            bcore_arr_fp_s_push( arr_fp, ( fp_t )bmath_general_signal_handler );
        }
        break;
        default: break;
    }
    return NULL;
}
// XOILA_OUT_SIGNATURE 0x904EFF1E79880213ull
