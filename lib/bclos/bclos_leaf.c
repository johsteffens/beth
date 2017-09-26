/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#include "bclos_leaf.h"
#include "bcore_spect_closure.h"
#include "bcore_quicktypes.h"

/**********************************************************************************************************************/

static tp_t t_ret_s3( vc_t o ) { return TYPEOF_s3_t; }
static tp_t t_ret_s2( vc_t o ) { return TYPEOF_s2_t; }
static tp_t t_ret_s1( vc_t o ) { return TYPEOF_s1_t; }
static tp_t t_ret_s0( vc_t o ) { return TYPEOF_s0_t; }
static tp_t t_ret_u3( vc_t o ) { return TYPEOF_u3_t; }
static tp_t t_ret_u2( vc_t o ) { return TYPEOF_u2_t; }
static tp_t t_ret_u1( vc_t o ) { return TYPEOF_u1_t; }
static tp_t t_ret_u0( vc_t o ) { return TYPEOF_u0_t; }
static tp_t t_ret_f3( vc_t o ) { return TYPEOF_f3_t; }
static tp_t t_ret_f2( vc_t o ) { return TYPEOF_f2_t; }
static tp_t t_ret_sz( vc_t o ) { return TYPEOF_sz_t; }
//static tp_t t_ret_bl( vc_t o ) { return TYPEOF_bl_t; }

//static sz_t n_args_1( vc_t o ) { return 1; }
static sz_t n_args_2( vc_t o ) { return 2; }

static ad_s d_arg_cs3_cs3( vc_t o, sz_t i ) { ad_s ad = { TYPEOF_s3_t, AD_F_RD }; return ad; }
static ad_s d_arg_cs2_cs2( vc_t o, sz_t i ) { ad_s ad = { TYPEOF_s2_t, AD_F_RD }; return ad; }
static ad_s d_arg_cs1_cs1( vc_t o, sz_t i ) { ad_s ad = { TYPEOF_s1_t, AD_F_RD }; return ad; }
static ad_s d_arg_cs0_cs0( vc_t o, sz_t i ) { ad_s ad = { TYPEOF_s0_t, AD_F_RD }; return ad; }
static ad_s d_arg_cu3_cu3( vc_t o, sz_t i ) { ad_s ad = { TYPEOF_u3_t, AD_F_RD }; return ad; }
static ad_s d_arg_cu2_cu2( vc_t o, sz_t i ) { ad_s ad = { TYPEOF_u2_t, AD_F_RD }; return ad; }
static ad_s d_arg_cu1_cu1( vc_t o, sz_t i ) { ad_s ad = { TYPEOF_u1_t, AD_F_RD }; return ad; }
static ad_s d_arg_cu0_cu0( vc_t o, sz_t i ) { ad_s ad = { TYPEOF_u0_t, AD_F_RD }; return ad; }
static ad_s d_arg_cf3_cf3( vc_t o, sz_t i ) { ad_s ad = { TYPEOF_f3_t, AD_F_RD }; return ad; }
static ad_s d_arg_cf2_cf2( vc_t o, sz_t i ) { ad_s ad = { TYPEOF_f2_t, AD_F_RD }; return ad; }
static ad_s d_arg_csz_csz( vc_t o, sz_t i ) { ad_s ad = { TYPEOF_sz_t, AD_F_RD }; return ad; }
//static ad_s d_arg_cbl_cbl( vc_t o, sz_t i ) { ad_s ad = { TYPEOF_bl_t, AD_F_RD }; return ad; }

static s3_t* cr_s3() { return bcore_alloc( NULL, sizeof( s3_t ) ); }
static s2_t* cr_s2() { return bcore_alloc( NULL, sizeof( s2_t ) ); }
static s1_t* cr_s1() { return bcore_alloc( NULL, sizeof( s1_t ) ); }
static s0_t* cr_s0() { return bcore_alloc( NULL, sizeof( s0_t ) ); }
static u3_t* cr_u3() { return bcore_alloc( NULL, sizeof( u3_t ) ); }
static u2_t* cr_u2() { return bcore_alloc( NULL, sizeof( u2_t ) ); }
static u1_t* cr_u1() { return bcore_alloc( NULL, sizeof( u1_t ) ); }
static u0_t* cr_u0() { return bcore_alloc( NULL, sizeof( u0_t ) ); }
static f3_t* cr_f3() { return bcore_alloc( NULL, sizeof( f3_t ) ); }
static f2_t* cr_f2() { return bcore_alloc( NULL, sizeof( f2_t ) ); }
static sz_t* cr_sz() { return bcore_alloc( NULL, sizeof( sz_t ) ); }
//static bl_t* cr_bl() { return bcore_alloc( NULL, sizeof( bl_t ) ); }

static s3_t* cl_s3( s3_t v ) { s3_t* p = cr_s3(); *p = v; return p; }
static s2_t* cl_s2( s2_t v ) { s2_t* p = cr_s2(); *p = v; return p; }
static s1_t* cl_s1( s1_t v ) { s1_t* p = cr_s1(); *p = v; return p; }
static s0_t* cl_s0( s0_t v ) { s0_t* p = cr_s0(); *p = v; return p; }
static u3_t* cl_u3( u3_t v ) { u3_t* p = cr_u3(); *p = v; return p; }
static u2_t* cl_u2( u2_t v ) { u2_t* p = cr_u2(); *p = v; return p; }
static u1_t* cl_u1( u1_t v ) { u1_t* p = cr_u1(); *p = v; return p; }
static u0_t* cl_u0( u0_t v ) { u0_t* p = cr_u0(); *p = v; return p; }
static f3_t* cl_f3( f3_t v ) { f3_t* p = cr_f3(); *p = v; return p; }
static f2_t* cl_f2( f2_t v ) { f2_t* p = cr_f2(); *p = v; return p; }
static sz_t* cl_sz( sz_t v ) { sz_t* p = cr_sz(); *p = v; return p; }
//static bl_t* cl_bl( bl_t v ) { bl_t* p = cr_bl(); *p = v; return p; }

static vd_t s3_add_s3_s3( vc_t o, s3_t** a, sz_t n ) { assert(n>=2); return cl_s3( ( *a[ 0 ] ) + ( *a[ 1 ] ) ); }
static vd_t s2_add_s2_s2( vc_t o, s2_t** a, sz_t n ) { assert(n>=2); return cl_s2( ( *a[ 0 ] ) + ( *a[ 1 ] ) ); }
static vd_t s1_add_s1_s1( vc_t o, s1_t** a, sz_t n ) { assert(n>=2); return cl_s1( ( *a[ 0 ] ) + ( *a[ 1 ] ) ); }
static vd_t s0_add_s0_s0( vc_t o, s1_t** a, sz_t n ) { assert(n>=2); return cl_s0( ( *a[ 0 ] ) + ( *a[ 1 ] ) ); }
static vd_t u3_add_u3_u3( vc_t o, u3_t** a, sz_t n ) { assert(n>=2); return cl_u3( ( *a[ 0 ] ) + ( *a[ 1 ] ) ); }
static vd_t u2_add_u2_u2( vc_t o, u2_t** a, sz_t n ) { assert(n>=2); return cl_u2( ( *a[ 0 ] ) + ( *a[ 1 ] ) ); }
static vd_t u1_add_u1_u1( vc_t o, u1_t** a, sz_t n ) { assert(n>=2); return cl_u1( ( *a[ 0 ] ) + ( *a[ 1 ] ) ); }
static vd_t u0_add_u0_u0( vc_t o, u0_t** a, sz_t n ) { assert(n>=2); return cl_u0( ( *a[ 0 ] ) + ( *a[ 1 ] ) ); }
static vd_t f3_add_f3_f3( vc_t o, f3_t** a, sz_t n ) { assert(n>=2); return cl_f3( ( *a[ 0 ] ) + ( *a[ 1 ] ) ); }
static vd_t f2_add_f2_f2( vc_t o, f2_t** a, sz_t n ) { assert(n>=2); return cl_f2( ( *a[ 0 ] ) + ( *a[ 1 ] ) ); }
static vd_t sz_add_sz_sz( vc_t o, sz_t** a, sz_t n ) { assert(n>=2); return cl_sz( ( *a[ 0 ] ) + ( *a[ 1 ] ) ); }

static vd_t s3_sub_s3_s3( vc_t o, s3_t** a, sz_t n ) { assert(n>=2); return cl_s3( ( *a[ 0 ] ) - ( *a[ 1 ] ) ); }
static vd_t s2_sub_s2_s2( vc_t o, s2_t** a, sz_t n ) { assert(n>=2); return cl_s2( ( *a[ 0 ] ) - ( *a[ 1 ] ) ); }
static vd_t s1_sub_s1_s1( vc_t o, s1_t** a, sz_t n ) { assert(n>=2); return cl_s1( ( *a[ 0 ] ) - ( *a[ 1 ] ) ); }
static vd_t s0_sub_s0_s0( vc_t o, s1_t** a, sz_t n ) { assert(n>=2); return cl_s0( ( *a[ 0 ] ) - ( *a[ 1 ] ) ); }
static vd_t u3_sub_u3_u3( vc_t o, u3_t** a, sz_t n ) { assert(n>=2); return cl_u3( ( *a[ 0 ] ) - ( *a[ 1 ] ) ); }
static vd_t u2_sub_u2_u2( vc_t o, u2_t** a, sz_t n ) { assert(n>=2); return cl_u2( ( *a[ 0 ] ) - ( *a[ 1 ] ) ); }
static vd_t u1_sub_u1_u1( vc_t o, u1_t** a, sz_t n ) { assert(n>=2); return cl_u1( ( *a[ 0 ] ) - ( *a[ 1 ] ) ); }
static vd_t u0_sub_u0_u0( vc_t o, u0_t** a, sz_t n ) { assert(n>=2); return cl_u0( ( *a[ 0 ] ) - ( *a[ 1 ] ) ); }
static vd_t f3_sub_f3_f3( vc_t o, f3_t** a, sz_t n ) { assert(n>=2); return cl_f3( ( *a[ 0 ] ) - ( *a[ 1 ] ) ); }
static vd_t f2_sub_f2_f2( vc_t o, f2_t** a, sz_t n ) { assert(n>=2); return cl_f2( ( *a[ 0 ] ) - ( *a[ 1 ] ) ); }
static vd_t sz_sub_sz_sz( vc_t o, sz_t** a, sz_t n ) { assert(n>=2); return cl_sz( ( *a[ 0 ] ) - ( *a[ 1 ] ) ); }

static vd_t s3_mul_s3_s3( vc_t o, s3_t** a, sz_t n ) { assert(n>=2); return cl_s3( ( *a[ 0 ] ) * ( *a[ 1 ] ) ); }
static vd_t s2_mul_s2_s2( vc_t o, s2_t** a, sz_t n ) { assert(n>=2); return cl_s2( ( *a[ 0 ] ) * ( *a[ 1 ] ) ); }
static vd_t s1_mul_s1_s1( vc_t o, s1_t** a, sz_t n ) { assert(n>=2); return cl_s1( ( *a[ 0 ] ) * ( *a[ 1 ] ) ); }
static vd_t s0_mul_s0_s0( vc_t o, s0_t** a, sz_t n ) { assert(n>=2); return cl_s0( ( *a[ 0 ] ) * ( *a[ 1 ] ) ); }
static vd_t u3_mul_u3_u3( vc_t o, u3_t** a, sz_t n ) { assert(n>=2); return cl_u3( ( *a[ 0 ] ) * ( *a[ 1 ] ) ); }
static vd_t u2_mul_u2_u2( vc_t o, u2_t** a, sz_t n ) { assert(n>=2); return cl_u2( ( *a[ 0 ] ) * ( *a[ 1 ] ) ); }
static vd_t u1_mul_u1_u1( vc_t o, u1_t** a, sz_t n ) { assert(n>=2); return cl_u1( ( *a[ 0 ] ) * ( *a[ 1 ] ) ); }
static vd_t u0_mul_u0_u0( vc_t o, u0_t** a, sz_t n ) { assert(n>=2); return cl_u0( ( *a[ 0 ] ) * ( *a[ 1 ] ) ); }
static vd_t f3_mul_f3_f3( vc_t o, f3_t** a, sz_t n ) { assert(n>=2); return cl_f3( ( *a[ 0 ] ) * ( *a[ 1 ] ) ); }
static vd_t f2_mul_f2_f2( vc_t o, f2_t** a, sz_t n ) { assert(n>=2); return cl_f2( ( *a[ 0 ] ) * ( *a[ 1 ] ) ); }
static vd_t sz_mul_sz_sz( vc_t o, sz_t** a, sz_t n ) { assert(n>=2); return cl_sz( ( *a[ 0 ] ) * ( *a[ 1 ] ) ); }

static vd_t s3_div_s3_s3( vc_t o, s3_t** a, sz_t n ) { assert(n>=2); return cl_s3( ( *a[ 0 ] ) / ( *a[ 1 ] ) ); }
static vd_t s2_div_s2_s2( vc_t o, s2_t** a, sz_t n ) { assert(n>=2); return cl_s2( ( *a[ 0 ] ) / ( *a[ 1 ] ) ); }
static vd_t s1_div_s1_s1( vc_t o, s1_t** a, sz_t n ) { assert(n>=2); return cl_s1( ( *a[ 0 ] ) / ( *a[ 1 ] ) ); }
static vd_t s0_div_s0_s0( vc_t o, s0_t** a, sz_t n ) { assert(n>=2); return cl_s0( ( *a[ 0 ] ) / ( *a[ 1 ] ) ); }
static vd_t u3_div_u3_u3( vc_t o, u3_t** a, sz_t n ) { assert(n>=2); return cl_u3( ( *a[ 0 ] ) / ( *a[ 1 ] ) ); }
static vd_t u2_div_u2_u2( vc_t o, u2_t** a, sz_t n ) { assert(n>=2); return cl_u2( ( *a[ 0 ] ) / ( *a[ 1 ] ) ); }
static vd_t u1_div_u1_u1( vc_t o, u1_t** a, sz_t n ) { assert(n>=2); return cl_u1( ( *a[ 0 ] ) / ( *a[ 1 ] ) ); }
static vd_t u0_div_u0_u0( vc_t o, u0_t** a, sz_t n ) { assert(n>=2); return cl_u0( ( *a[ 0 ] ) / ( *a[ 1 ] ) ); }
static vd_t f3_div_f3_f3( vc_t o, f3_t** a, sz_t n ) { assert(n>=2); return cl_f3( ( *a[ 0 ] ) / ( *a[ 1 ] ) ); }
static vd_t f2_div_f2_f2( vc_t o, f2_t** a, sz_t n ) { assert(n>=2); return cl_f2( ( *a[ 0 ] ) / ( *a[ 1 ] ) ); }
static vd_t sz_div_sz_sz( vc_t o, sz_t** a, sz_t n ) { assert(n>=2); return cl_sz( ( *a[ 0 ] ) / ( *a[ 1 ] ) ); }

typedef bcore_flect_self_s self_s;

static bcore_flect_self_s* crs( sc_t bare_name, fp_t func, fp_t n_args, fp_t d_arg, fp_t t_ret )
{
    bcore_string_s* def = bcore_string_s_createf( "bclos_%s = { aware_t _; }", bare_name );
    self_s* self = bcore_flect_self_s_build_parse_sc( def->sc, sizeof( aware_t ) );
    bcore_string_s_discard( def );
    bcore_flect_self_s_push_external_func( self, func,   "bcore_closure_fp_func",  "func"   );
    bcore_flect_self_s_push_external_func( self, n_args, "bcore_closure_fp_n_args","n_args" );
    bcore_flect_self_s_push_external_func( self, d_arg,  "bcore_closure_fp_d_arg", "d_arg"  );
    bcore_flect_self_s_push_external_func( self, t_ret,  "bcore_closure_fp_t_ret", "t_ret"  );
    return self;
}

static self_s* crs_s3_add_s3_s3(void) { return crs( "s3_add_s3_s3", (fp_t)s3_add_s3_s3, (fp_t)n_args_2, (fp_t)d_arg_cs3_cs3, (fp_t)t_ret_s3 ); }
static self_s* crs_s2_add_s2_s2(void) { return crs( "s2_add_s2_s2", (fp_t)s2_add_s2_s2, (fp_t)n_args_2, (fp_t)d_arg_cs2_cs2, (fp_t)t_ret_s2 ); }
static self_s* crs_s1_add_s1_s1(void) { return crs( "s1_add_s1_s1", (fp_t)s1_add_s1_s1, (fp_t)n_args_2, (fp_t)d_arg_cs1_cs1, (fp_t)t_ret_s1 ); }
static self_s* crs_s0_add_s0_s0(void) { return crs( "s0_add_s0_s0", (fp_t)s0_add_s0_s0, (fp_t)n_args_2, (fp_t)d_arg_cs0_cs0, (fp_t)t_ret_s0 ); }
static self_s* crs_u3_add_u3_u3(void) { return crs( "u3_add_u3_u3", (fp_t)u3_add_u3_u3, (fp_t)n_args_2, (fp_t)d_arg_cu3_cu3, (fp_t)t_ret_u3 ); }
static self_s* crs_u2_add_u2_u2(void) { return crs( "u2_add_u2_u2", (fp_t)u2_add_u2_u2, (fp_t)n_args_2, (fp_t)d_arg_cu2_cu2, (fp_t)t_ret_u2 ); }
static self_s* crs_u1_add_u1_u1(void) { return crs( "u1_add_u1_u1", (fp_t)u1_add_u1_u1, (fp_t)n_args_2, (fp_t)d_arg_cu1_cu1, (fp_t)t_ret_u1 ); }
static self_s* crs_u0_add_u0_u0(void) { return crs( "u0_add_u0_u0", (fp_t)u0_add_u0_u0, (fp_t)n_args_2, (fp_t)d_arg_cu0_cu0, (fp_t)t_ret_u0 ); }
static self_s* crs_f3_add_f3_f3(void) { return crs( "f3_add_f3_f3", (fp_t)f3_add_f3_f3, (fp_t)n_args_2, (fp_t)d_arg_cf3_cf3, (fp_t)t_ret_f3 ); }
static self_s* crs_f2_add_f2_f2(void) { return crs( "f2_add_f2_f2", (fp_t)f2_add_f2_f2, (fp_t)n_args_2, (fp_t)d_arg_cf2_cf2, (fp_t)t_ret_f2 ); }
static self_s* crs_sz_add_sz_sz(void) { return crs( "sz_add_sz_sz", (fp_t)sz_add_sz_sz, (fp_t)n_args_2, (fp_t)d_arg_csz_csz, (fp_t)t_ret_sz ); }

static self_s* crs_s3_sub_s3_s3(void) { return crs( "s3_sub_s3_s3", (fp_t)s3_sub_s3_s3, (fp_t)n_args_2, (fp_t)d_arg_cs3_cs3, (fp_t)t_ret_s3 ); }
static self_s* crs_s2_sub_s2_s2(void) { return crs( "s2_sub_s2_s2", (fp_t)s2_sub_s2_s2, (fp_t)n_args_2, (fp_t)d_arg_cs2_cs2, (fp_t)t_ret_s2 ); }
static self_s* crs_s1_sub_s1_s1(void) { return crs( "s1_sub_s1_s1", (fp_t)s1_sub_s1_s1, (fp_t)n_args_2, (fp_t)d_arg_cs1_cs1, (fp_t)t_ret_s1 ); }
static self_s* crs_s0_sub_s0_s0(void) { return crs( "s0_sub_s0_s0", (fp_t)s0_sub_s0_s0, (fp_t)n_args_2, (fp_t)d_arg_cs0_cs0, (fp_t)t_ret_s0 ); }
static self_s* crs_u3_sub_u3_u3(void) { return crs( "u3_sub_u3_u3", (fp_t)u3_sub_u3_u3, (fp_t)n_args_2, (fp_t)d_arg_cu3_cu3, (fp_t)t_ret_u3 ); }
static self_s* crs_u2_sub_u2_u2(void) { return crs( "u2_sub_u2_u2", (fp_t)u2_sub_u2_u2, (fp_t)n_args_2, (fp_t)d_arg_cu2_cu2, (fp_t)t_ret_u2 ); }
static self_s* crs_u1_sub_u1_u1(void) { return crs( "u1_sub_u1_u1", (fp_t)u1_sub_u1_u1, (fp_t)n_args_2, (fp_t)d_arg_cu1_cu1, (fp_t)t_ret_u1 ); }
static self_s* crs_u0_sub_u0_u0(void) { return crs( "u0_sub_u0_u0", (fp_t)u0_sub_u0_u0, (fp_t)n_args_2, (fp_t)d_arg_cu0_cu0, (fp_t)t_ret_u0 ); }
static self_s* crs_f3_sub_f3_f3(void) { return crs( "f3_sub_f3_f3", (fp_t)f3_sub_f3_f3, (fp_t)n_args_2, (fp_t)d_arg_cf3_cf3, (fp_t)t_ret_f3 ); }
static self_s* crs_f2_sub_f2_f2(void) { return crs( "f2_sub_f2_f2", (fp_t)f2_sub_f2_f2, (fp_t)n_args_2, (fp_t)d_arg_cf2_cf2, (fp_t)t_ret_f2 ); }
static self_s* crs_sz_sub_sz_sz(void) { return crs( "sz_sub_sz_sz", (fp_t)sz_sub_sz_sz, (fp_t)n_args_2, (fp_t)d_arg_csz_csz, (fp_t)t_ret_sz ); }

static self_s* crs_s3_mul_s3_s3(void) { return crs( "s3_mul_s3_s3", (fp_t)s3_mul_s3_s3, (fp_t)n_args_2, (fp_t)d_arg_cs3_cs3, (fp_t)t_ret_s3 ); }
static self_s* crs_s2_mul_s2_s2(void) { return crs( "s2_mul_s2_s2", (fp_t)s2_mul_s2_s2, (fp_t)n_args_2, (fp_t)d_arg_cs2_cs2, (fp_t)t_ret_s2 ); }
static self_s* crs_s1_mul_s1_s1(void) { return crs( "s1_mul_s1_s1", (fp_t)s1_mul_s1_s1, (fp_t)n_args_2, (fp_t)d_arg_cs1_cs1, (fp_t)t_ret_s1 ); }
static self_s* crs_s0_mul_s0_s0(void) { return crs( "s0_mul_s0_s0", (fp_t)s0_mul_s0_s0, (fp_t)n_args_2, (fp_t)d_arg_cs0_cs0, (fp_t)t_ret_s0 ); }
static self_s* crs_u3_mul_u3_u3(void) { return crs( "u3_mul_u3_u3", (fp_t)u3_mul_u3_u3, (fp_t)n_args_2, (fp_t)d_arg_cu3_cu3, (fp_t)t_ret_u3 ); }
static self_s* crs_u2_mul_u2_u2(void) { return crs( "u2_mul_u2_u2", (fp_t)u2_mul_u2_u2, (fp_t)n_args_2, (fp_t)d_arg_cu2_cu2, (fp_t)t_ret_u2 ); }
static self_s* crs_u1_mul_u1_u1(void) { return crs( "u1_mul_u1_u1", (fp_t)u1_mul_u1_u1, (fp_t)n_args_2, (fp_t)d_arg_cu1_cu1, (fp_t)t_ret_u1 ); }
static self_s* crs_u0_mul_u0_u0(void) { return crs( "u0_mul_u0_u0", (fp_t)u0_mul_u0_u0, (fp_t)n_args_2, (fp_t)d_arg_cu0_cu0, (fp_t)t_ret_u0 ); }
static self_s* crs_f3_mul_f3_f3(void) { return crs( "f3_mul_f3_f3", (fp_t)f3_mul_f3_f3, (fp_t)n_args_2, (fp_t)d_arg_cf3_cf3, (fp_t)t_ret_f3 ); }
static self_s* crs_f2_mul_f2_f2(void) { return crs( "f2_mul_f2_f2", (fp_t)f2_mul_f2_f2, (fp_t)n_args_2, (fp_t)d_arg_cf2_cf2, (fp_t)t_ret_f2 ); }
static self_s* crs_sz_mul_sz_sz(void) { return crs( "sz_mul_sz_sz", (fp_t)sz_mul_sz_sz, (fp_t)n_args_2, (fp_t)d_arg_csz_csz, (fp_t)t_ret_sz ); }

static self_s* crs_s3_div_s3_s3(void) { return crs( "s3_div_s3_s3", (fp_t)s3_div_s3_s3, (fp_t)n_args_2, (fp_t)d_arg_cs3_cs3, (fp_t)t_ret_s3 ); }
static self_s* crs_s2_div_s2_s2(void) { return crs( "s2_div_s2_s2", (fp_t)s2_div_s2_s2, (fp_t)n_args_2, (fp_t)d_arg_cs2_cs2, (fp_t)t_ret_s2 ); }
static self_s* crs_s1_div_s1_s1(void) { return crs( "s1_div_s1_s1", (fp_t)s1_div_s1_s1, (fp_t)n_args_2, (fp_t)d_arg_cs1_cs1, (fp_t)t_ret_s1 ); }
static self_s* crs_s0_div_s0_s0(void) { return crs( "s0_div_s0_s0", (fp_t)s0_div_s0_s0, (fp_t)n_args_2, (fp_t)d_arg_cs0_cs0, (fp_t)t_ret_s0 ); }
static self_s* crs_u3_div_u3_u3(void) { return crs( "u3_div_u3_u3", (fp_t)u3_div_u3_u3, (fp_t)n_args_2, (fp_t)d_arg_cu3_cu3, (fp_t)t_ret_u3 ); }
static self_s* crs_u2_div_u2_u2(void) { return crs( "u2_div_u2_u2", (fp_t)u2_div_u2_u2, (fp_t)n_args_2, (fp_t)d_arg_cu2_cu2, (fp_t)t_ret_u2 ); }
static self_s* crs_u1_div_u1_u1(void) { return crs( "u1_div_u1_u1", (fp_t)u1_div_u1_u1, (fp_t)n_args_2, (fp_t)d_arg_cu1_cu1, (fp_t)t_ret_u1 ); }
static self_s* crs_u0_div_u0_u0(void) { return crs( "u0_div_u0_u0", (fp_t)u0_div_u0_u0, (fp_t)n_args_2, (fp_t)d_arg_cu0_cu0, (fp_t)t_ret_u0 ); }
static self_s* crs_f3_div_f3_f3(void) { return crs( "f3_div_f3_f3", (fp_t)f3_div_f3_f3, (fp_t)n_args_2, (fp_t)d_arg_cf3_cf3, (fp_t)t_ret_f3 ); }
static self_s* crs_f2_div_f2_f2(void) { return crs( "f2_div_f2_f2", (fp_t)f2_div_f2_f2, (fp_t)n_args_2, (fp_t)d_arg_cf2_cf2, (fp_t)t_ret_f2 ); }
static self_s* crs_sz_div_sz_sz(void) { return crs( "sz_div_sz_sz", (fp_t)sz_div_sz_sz, (fp_t)n_args_2, (fp_t)d_arg_csz_csz, (fp_t)t_ret_sz ); }

/**********************************************************************************************************************/

void bclos_leaf_define_self_creators(void)
{
    bcore_flect_define_creator( typeof( "bclos_s3_add_s3_s3" ), crs_s3_add_s3_s3 );
    bcore_flect_define_creator( typeof( "bclos_s2_add_s2_s2" ), crs_s2_add_s2_s2 );
    bcore_flect_define_creator( typeof( "bclos_s1_add_s1_s1" ), crs_s1_add_s1_s1 );
    bcore_flect_define_creator( typeof( "bclos_s0_add_s0_s0" ), crs_s0_add_s0_s0 );
    bcore_flect_define_creator( typeof( "bclos_u3_add_u3_u3" ), crs_u3_add_u3_u3 );
    bcore_flect_define_creator( typeof( "bclos_u2_add_u2_u2" ), crs_u2_add_u2_u2 );
    bcore_flect_define_creator( typeof( "bclos_u1_add_u1_u1" ), crs_u1_add_u1_u1 );
    bcore_flect_define_creator( typeof( "bclos_u0_add_u0_u0" ), crs_u0_add_u0_u0 );
    bcore_flect_define_creator( typeof( "bclos_f3_add_f3_f3" ), crs_f3_add_f3_f3 );
    bcore_flect_define_creator( typeof( "bclos_f2_add_f2_f2" ), crs_f2_add_f2_f2 );
    bcore_flect_define_creator( typeof( "bclos_sz_add_sz_sz" ), crs_sz_add_sz_sz );

    bcore_flect_define_creator( typeof( "bclos_s3_sub_s3_s3" ), crs_s3_sub_s3_s3 );
    bcore_flect_define_creator( typeof( "bclos_s2_sub_s2_s2" ), crs_s2_sub_s2_s2 );
    bcore_flect_define_creator( typeof( "bclos_s1_sub_s1_s1" ), crs_s1_sub_s1_s1 );
    bcore_flect_define_creator( typeof( "bclos_s0_sub_s0_s0" ), crs_s0_sub_s0_s0 );
    bcore_flect_define_creator( typeof( "bclos_u3_sub_u3_u3" ), crs_u3_sub_u3_u3 );
    bcore_flect_define_creator( typeof( "bclos_u2_sub_u2_u2" ), crs_u2_sub_u2_u2 );
    bcore_flect_define_creator( typeof( "bclos_u1_sub_u1_u1" ), crs_u1_sub_u1_u1 );
    bcore_flect_define_creator( typeof( "bclos_u0_sub_u0_u0" ), crs_u0_sub_u0_u0 );
    bcore_flect_define_creator( typeof( "bclos_f3_sub_f3_f3" ), crs_f3_sub_f3_f3 );
    bcore_flect_define_creator( typeof( "bclos_f2_sub_f2_f2" ), crs_f2_sub_f2_f2 );
    bcore_flect_define_creator( typeof( "bclos_sz_sub_sz_sz" ), crs_sz_sub_sz_sz );

    bcore_flect_define_creator( typeof( "bclos_s3_mul_s3_s3" ), crs_s3_mul_s3_s3 );
    bcore_flect_define_creator( typeof( "bclos_s2_mul_s2_s2" ), crs_s2_mul_s2_s2 );
    bcore_flect_define_creator( typeof( "bclos_s1_mul_s1_s1" ), crs_s1_mul_s1_s1 );
    bcore_flect_define_creator( typeof( "bclos_s0_mul_s0_s0" ), crs_s0_mul_s0_s0 );
    bcore_flect_define_creator( typeof( "bclos_u3_mul_u3_u3" ), crs_u3_mul_u3_u3 );
    bcore_flect_define_creator( typeof( "bclos_u2_mul_u2_u2" ), crs_u2_mul_u2_u2 );
    bcore_flect_define_creator( typeof( "bclos_u1_mul_u1_u1" ), crs_u1_mul_u1_u1 );
    bcore_flect_define_creator( typeof( "bclos_u0_mul_u0_u0" ), crs_u0_mul_u0_u0 );
    bcore_flect_define_creator( typeof( "bclos_f3_mul_f3_f3" ), crs_f3_mul_f3_f3 );
    bcore_flect_define_creator( typeof( "bclos_f2_mul_f2_f2" ), crs_f2_mul_f2_f2 );
    bcore_flect_define_creator( typeof( "bclos_sz_mul_sz_sz" ), crs_sz_mul_sz_sz );

    bcore_flect_define_creator( typeof( "bclos_s3_div_s3_s3" ), crs_s3_div_s3_s3 );
    bcore_flect_define_creator( typeof( "bclos_s2_div_s2_s2" ), crs_s2_div_s2_s2 );
    bcore_flect_define_creator( typeof( "bclos_s1_div_s1_s1" ), crs_s1_div_s1_s1 );
    bcore_flect_define_creator( typeof( "bclos_s0_div_s0_s0" ), crs_s0_div_s0_s0 );
    bcore_flect_define_creator( typeof( "bclos_u3_div_u3_u3" ), crs_u3_div_u3_u3 );
    bcore_flect_define_creator( typeof( "bclos_u2_div_u2_u2" ), crs_u2_div_u2_u2 );
    bcore_flect_define_creator( typeof( "bclos_u1_div_u1_u1" ), crs_u1_div_u1_u1 );
    bcore_flect_define_creator( typeof( "bclos_u0_div_u0_u0" ), crs_u0_div_u0_u0 );
    bcore_flect_define_creator( typeof( "bclos_f3_div_f3_f3" ), crs_f3_div_f3_f3 );
    bcore_flect_define_creator( typeof( "bclos_f2_div_f2_f2" ), crs_f2_div_f2_f2 );
    bcore_flect_define_creator( typeof( "bclos_sz_div_sz_sz" ), crs_sz_div_sz_sz );
}

vd_t bclos_leaf_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bclos_leaf" ) ) return NULL;

    if( signal == typeof( "init1" ) )
    {
        bcore_flect_define_creator( typeof( "bclos_s3_add_s3_s3" ), crs_s3_add_s3_s3 );
        bcore_flect_define_creator( typeof( "bclos_s2_add_s2_s2" ), crs_s2_add_s2_s2 );
        bcore_flect_define_creator( typeof( "bclos_s1_add_s1_s1" ), crs_s1_add_s1_s1 );
        bcore_flect_define_creator( typeof( "bclos_s0_add_s0_s0" ), crs_s0_add_s0_s0 );
        bcore_flect_define_creator( typeof( "bclos_u3_add_u3_u3" ), crs_u3_add_u3_u3 );
        bcore_flect_define_creator( typeof( "bclos_u2_add_u2_u2" ), crs_u2_add_u2_u2 );
        bcore_flect_define_creator( typeof( "bclos_u1_add_u1_u1" ), crs_u1_add_u1_u1 );
        bcore_flect_define_creator( typeof( "bclos_u0_add_u0_u0" ), crs_u0_add_u0_u0 );
        bcore_flect_define_creator( typeof( "bclos_f3_add_f3_f3" ), crs_f3_add_f3_f3 );
        bcore_flect_define_creator( typeof( "bclos_f2_add_f2_f2" ), crs_f2_add_f2_f2 );
        bcore_flect_define_creator( typeof( "bclos_sz_add_sz_sz" ), crs_sz_add_sz_sz );

        bcore_flect_define_creator( typeof( "bclos_s3_sub_s3_s3" ), crs_s3_sub_s3_s3 );
        bcore_flect_define_creator( typeof( "bclos_s2_sub_s2_s2" ), crs_s2_sub_s2_s2 );
        bcore_flect_define_creator( typeof( "bclos_s1_sub_s1_s1" ), crs_s1_sub_s1_s1 );
        bcore_flect_define_creator( typeof( "bclos_s0_sub_s0_s0" ), crs_s0_sub_s0_s0 );
        bcore_flect_define_creator( typeof( "bclos_u3_sub_u3_u3" ), crs_u3_sub_u3_u3 );
        bcore_flect_define_creator( typeof( "bclos_u2_sub_u2_u2" ), crs_u2_sub_u2_u2 );
        bcore_flect_define_creator( typeof( "bclos_u1_sub_u1_u1" ), crs_u1_sub_u1_u1 );
        bcore_flect_define_creator( typeof( "bclos_u0_sub_u0_u0" ), crs_u0_sub_u0_u0 );
        bcore_flect_define_creator( typeof( "bclos_f3_sub_f3_f3" ), crs_f3_sub_f3_f3 );
        bcore_flect_define_creator( typeof( "bclos_f2_sub_f2_f2" ), crs_f2_sub_f2_f2 );
        bcore_flect_define_creator( typeof( "bclos_sz_sub_sz_sz" ), crs_sz_sub_sz_sz );

        bcore_flect_define_creator( typeof( "bclos_s3_mul_s3_s3" ), crs_s3_mul_s3_s3 );
        bcore_flect_define_creator( typeof( "bclos_s2_mul_s2_s2" ), crs_s2_mul_s2_s2 );
        bcore_flect_define_creator( typeof( "bclos_s1_mul_s1_s1" ), crs_s1_mul_s1_s1 );
        bcore_flect_define_creator( typeof( "bclos_s0_mul_s0_s0" ), crs_s0_mul_s0_s0 );
        bcore_flect_define_creator( typeof( "bclos_u3_mul_u3_u3" ), crs_u3_mul_u3_u3 );
        bcore_flect_define_creator( typeof( "bclos_u2_mul_u2_u2" ), crs_u2_mul_u2_u2 );
        bcore_flect_define_creator( typeof( "bclos_u1_mul_u1_u1" ), crs_u1_mul_u1_u1 );
        bcore_flect_define_creator( typeof( "bclos_u0_mul_u0_u0" ), crs_u0_mul_u0_u0 );
        bcore_flect_define_creator( typeof( "bclos_f3_mul_f3_f3" ), crs_f3_mul_f3_f3 );
        bcore_flect_define_creator( typeof( "bclos_f2_mul_f2_f2" ), crs_f2_mul_f2_f2 );
        bcore_flect_define_creator( typeof( "bclos_sz_mul_sz_sz" ), crs_sz_mul_sz_sz );

        bcore_flect_define_creator( typeof( "bclos_s3_div_s3_s3" ), crs_s3_div_s3_s3 );
        bcore_flect_define_creator( typeof( "bclos_s2_div_s2_s2" ), crs_s2_div_s2_s2 );
        bcore_flect_define_creator( typeof( "bclos_s1_div_s1_s1" ), crs_s1_div_s1_s1 );
        bcore_flect_define_creator( typeof( "bclos_s0_div_s0_s0" ), crs_s0_div_s0_s0 );
        bcore_flect_define_creator( typeof( "bclos_u3_div_u3_u3" ), crs_u3_div_u3_u3 );
        bcore_flect_define_creator( typeof( "bclos_u2_div_u2_u2" ), crs_u2_div_u2_u2 );
        bcore_flect_define_creator( typeof( "bclos_u1_div_u1_u1" ), crs_u1_div_u1_u1 );
        bcore_flect_define_creator( typeof( "bclos_u0_div_u0_u0" ), crs_u0_div_u0_u0 );
        bcore_flect_define_creator( typeof( "bclos_f3_div_f3_f3" ), crs_f3_div_f3_f3 );
        bcore_flect_define_creator( typeof( "bclos_f2_div_f2_f2" ), crs_f2_div_f2_f2 );
        bcore_flect_define_creator( typeof( "bclos_sz_div_sz_sz" ), crs_sz_div_sz_sz );
    }

    return NULL;
}

/**********************************************************************************************************************/


