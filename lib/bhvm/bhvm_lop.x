/** Author and Copyright 2019 Johannes Bernhard Steffens
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/**********************************************************************************************************************/

/** Collection of Leaf-Operators (Element Operators)
 *  A leaf operator has leaf types as operands. In this case these types are floating point types.
 *  On certain extended types (vectors, matrices, holors) leaf operator can be applied in vectorized manner.
 *
 *  Object Nomenclature:
 *    Dendrite pass is specified by the object (not function)
 *    Suffix: dp_<signature>
 *
 *  Function Nomenclature:
 *    f    - plain pass
 *   of    - functor pass
 *
 */

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

/// nullary operators
group :ar0 =
{
    extending stump verbatim :_s = aware : {};

    signature f2_t f2();
    signature f3_t f3();

    signature void f( tp_t tknit, vd_t r, sz_t s );

    // vector
    body body_cv =
    {
        assert( s == 0 || r );
        switch( tknit )
        {
            case BKNIT_F2: { f2_t v = @_f2(); for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[i]=v; } } break;
            case BKNIT_F3: { f3_t v = @_f3(); for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[i]=v; } } break;
            default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
        }
    };

    stamp :zro_s = { func : .f2 = { return 0; }; func : .f3 = { return 0; }; func : .f = :body_cv; };
    stamp :one_s = { func : .f2 = { return 1; }; func : .f3 = { return 1; }; func : .f = :body_cv; };

    /// dendrite pass ----------------------------------------------------------

    stamp :nul_dp_v_s = { func : .f = { /* no action*/ }; };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// unary operators
group :ar1 =
{
    extending stump verbatim :_s = aware : {};

    signature f2_t f2( f2_t a );
    signature f3_t f3( f3_t a );

    signature void f( tp_t tknit, vc_t a, vd_t r, sz_t s );

    // vector <- vector
    body body_v_cv =
    {
        assert( s == 0 || ( a && r ) );
        switch( tknit )
        {
            case BKNIT_F22: for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[i]=@_f2(cast(a,m f2_t*)[i]); } break;
            case BKNIT_F23: for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[i]=@_f2(cast(a,m f2_t*)[i]); } break;
            case BKNIT_F32: for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[i]=@_f3(cast(a,m f3_t*)[i]); } break;
            case BKNIT_F33: for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[i]=@_f3(cast(a,m f3_t*)[i]); } break;
            default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
        }
    };

    // vector <- vector
    body body_v_av =
    {
        assert( s == 0 || ( a && r ) );
        switch( tknit )
        {
            case BKNIT_F22: for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[i]+=@_f2(cast(a,m f2_t*)[i]); } break;
            case BKNIT_F23: for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[i]+=@_f2(cast(a,m f2_t*)[i]); } break;
            case BKNIT_F32: for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[i]+=@_f3(cast(a,m f3_t*)[i]); } break;
            case BKNIT_F33: for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[i]+=@_f3(cast(a,m f3_t*)[i]); } break;
            default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
        }
    };

    // vector <- scalar
    body body_s_cv =
    {
        assert( s == 0 || ( a && r ) );
        switch( tknit )
        {
            case BKNIT_F22: for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[i]=@_f2(cast(a,m f2_t*)[0]); } break;
            case BKNIT_F23: for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[i]=@_f2(cast(a,m f2_t*)[0]); } break;
            case BKNIT_F32: for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[i]=@_f3(cast(a,m f3_t*)[0]); } break;
            case BKNIT_F33: for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[i]=@_f3(cast(a,m f3_t*)[0]); } break;
            default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
        }
    };

    // scalar <- vector
    body body_v_cs =
    {
        assert( a && r );
        switch( tknit )
        {
            case BKNIT_F22: cast(r,m f2_t*)[0] = 0; for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[0]+=@_f2(cast(a,m f2_t*)[i]); } break;
            case BKNIT_F23: cast(r,m f3_t*)[0] = 0; for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[0]+=@_f2(cast(a,m f2_t*)[i]); } break;
            case BKNIT_F32: cast(r,m f2_t*)[0] = 0; for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[0]+=@_f3(cast(a,m f3_t*)[i]); } break;
            case BKNIT_F33: cast(r,m f3_t*)[0] = 0; for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[0]+=@_f3(cast(a,m f3_t*)[i]); } break;
            default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
        }
    };

    /// axon pass --------------------------------------------------------------

    stamp :cpy_s      = { func : .f2 = { return  a; }; func : .f3 = { return  a; }; func : .f = :body_v_cv; };
    stamp :identity_s = { func : .f2 = { return  a; }; func : .f3 = { return  a; }; func : .f = :body_v_cv; }; // same as cpy
    stamp :neg_s      = { func : .f2 = { return -a; }; func : .f3 = { return -a; }; func : .f = :body_v_cv; };
    stamp :floor_s    = { func : .f2 = { return floorf(a); }; func : .f3 = { return  floor(a); }; func : .f = :body_v_cv; };
    stamp :ceil_s     = { func : .f2 = { return  ceilf(a); }; func : .f3 = { return   ceil(a); }; func : .f = :body_v_cv; };
    stamp :exp_s      = { func : .f2 = { return f2_exp(a); }; func : .f3 = { return f3_exp(a); }; func : .f = :body_v_cv; };
    stamp :log_s      = { func : .f2 = { return f2_log(a); }; func : .f3 = { return f3_log(a); }; func : .f = :body_v_cv; };
    stamp :inv_s      = { func : .f2 = { return f2_inv(a); }; func : .f3 = { return f3_inv(a); }; func : .f = :body_v_cv; };

    stamp :abs_s      = { func : .f2 = { return f2_abs(a); }; func : .f3 = { return f3_abs(a); }; func : .f = :body_v_cv; };
    stamp :sig_s      = { func : .f2 = { return f2_sig(a); }; func : .f3 = { return f3_sig(a); }; func : .f = :body_v_cv; };
    stamp :l1_s       = { func : .f2 = { return f2_abs(a); }; func : .f3 = { return f3_abs(a); }; func : .f = :body_v_cs; };
    stamp :sqr_s      = { func : .f2 = { return f2_sqr(a); }; func : .f3 = { return f3_sqr(a); }; func : .f = :body_v_cv; };
    stamp :sqrsum_s   = { func : .f2 = { return f2_sqr(a); }; func : .f3 = { return f3_sqr(a); }; func : .f = :body_v_cs; }; // squared sum
    stamp :srt_s      = { func : .f2 = { return f2_srt(a); }; func : .f3 = { return f3_srt(a); }; func : .f = :body_v_cv; };

    body body_sigm       = { return ( a > -700 ) ? ( 1.0 / ( 1.0 + exp( - cast( a, f3_t ) ) ) ) : 0; };
    body body_sigm_hard  = { return ( a < -2.0 ) ? 0.0 : ( a > 2.0 ) ? 1.0 : 0.25 * ( a + 2.0 ); };
    body body_sigm_leaky = { return ( a < -2.0 ) ? 0.01 * ( a + 2.0 ) : ( a > 2.0 ) ? 1.0 + 0.01 * ( a - 2.0 ) : 0.25 * ( a + 2.0 ); };
    body body_tanh       = { return ( a < 350 ) ? ( 1.0 - ( 2.0 / ( exp( 2.0 * cast( a, f3_t ) ) + 1.0 ) ) ) : 1.0; };
    body body_tanh_hard  = { return ( a < -1.0 ) ? -1.0 : ( a > 1.0 ) ? 1.0 : a; };
    body body_tanh_leaky = { return ( a < -1.0 ) ? -1.0 + 0.01 * ( a + 1.0 ) : ( a > 1.0 ) ? 1.0 + 0.01 * ( a - 1.0 ) : a; };
    body body_softplus   = { return ( a < 700 ) ? log( 1.0 + exp( cast( a, f3_t ) ) ) : a; };
    body body_relu       = { return a > 0 ? a : 0; };
    body body_relu_leaky = { return a > 0 ? a : a * 0.01; };

    stamp :tanh_s       = { func : .f2 = :body_tanh;       func : .f3 = :body_tanh;       func : .f = :body_v_cv; };
    stamp :tanh_hard_s  = { func : .f2 = :body_tanh_hard;  func : .f3 = :body_tanh_hard;  func : .f = :body_v_cv; };
    stamp :tanh_leaky_s = { func : .f2 = :body_tanh_leaky; func : .f3 = :body_tanh_leaky; func : .f = :body_v_cv; };
    stamp :sigm_s       = { func : .f2 = :body_sigm;       func : .f3 = :body_sigm;       func : .f = :body_v_cv; };
    stamp :sigm_hard_s  = { func : .f2 = :body_sigm_hard;  func : .f3 = :body_sigm_hard;  func : .f = :body_v_cv; };
    stamp :sigm_leaky_s = { func : .f2 = :body_sigm_leaky; func : .f3 = :body_sigm_leaky; func : .f = :body_v_cv; };
    stamp :softplus_s   = { func : .f2 = :body_softplus;   func : .f3 = :body_softplus;   func : .f = :body_v_cv; };
    stamp :relu_s       = { func : .f2 = :body_relu;       func : .f3 = :body_relu;       func : .f = :body_v_cv; };
    stamp :relu_leaky_s = { func : .f2 = :body_relu_leaky; func : .f3 = :body_relu_leaky; func : .f = :body_v_cv; };

    /// dendrite pass ----------------------------------------------------------

    /// simple accumulation
    stamp :acc_s  = { func : .f2 = { return  a; }; func : .f3 = { return  a; }; func : .f = :body_v_av; };
    stamp :accn_s = { func : .f2 = { return -a; }; func : .f3 = { return -a; }; func : .f = :body_v_av; };
};

// ---------------------------------------------------------------------------------------------------------------------

/// binary operators
group :ar2 =
{
    extending stump verbatim :_s = aware : {};

    signature f2_t f2( f2_t a, f2_t b );
    signature f3_t f3( f3_t a, f3_t b );

    signature void f  ( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
    signature void f_m( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t rows, sz_t cols );

    // vector <- vector <op> vector
    body body_vv_cv =
    {
        assert( s == 0 || ( a && b && r ) );
        switch( tknit )
        {
            case BKNIT_F222: for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[i]=@_f2(cast(a,m f2_t*)[i],cast(b,m f2_t*)[i]); } break;
            case BKNIT_F223: for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[i]=@_f2(cast(a,m f2_t*)[i],cast(b,m f2_t*)[i]); } break;
            case BKNIT_F232: for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[i]=@_f3(cast(a,m f2_t*)[i],cast(b,m f3_t*)[i]); } break;
            case BKNIT_F233: for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[i]=@_f3(cast(a,m f2_t*)[i],cast(b,m f3_t*)[i]); } break;
            case BKNIT_F322: for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[i]=@_f3(cast(a,m f3_t*)[i],cast(b,m f2_t*)[i]); } break;
            case BKNIT_F323: for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[i]=@_f3(cast(a,m f3_t*)[i],cast(b,m f2_t*)[i]); } break;
            case BKNIT_F332: for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[i]=@_f3(cast(a,m f3_t*)[i],cast(b,m f3_t*)[i]); } break;
            case BKNIT_F333: for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[i]=@_f3(cast(a,m f3_t*)[i],cast(b,m f3_t*)[i]); } break;
            default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
        }
    };

    // vector += vector <op> vector
    body body_vv_av =
    {
        assert( s == 0 || ( a && b && r ) );
        switch( tknit )
        {
            case BKNIT_F222: for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[i]+=@_f2(cast(a,m f2_t*)[i],cast(b,m f2_t*)[i]); } break;
            case BKNIT_F223: for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[i]+=@_f2(cast(a,m f2_t*)[i],cast(b,m f2_t*)[i]); } break;
            case BKNIT_F232: for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[i]+=@_f3(cast(a,m f2_t*)[i],cast(b,m f3_t*)[i]); } break;
            case BKNIT_F233: for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[i]+=@_f3(cast(a,m f2_t*)[i],cast(b,m f3_t*)[i]); } break;
            case BKNIT_F322: for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[i]+=@_f3(cast(a,m f3_t*)[i],cast(b,m f2_t*)[i]); } break;
            case BKNIT_F323: for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[i]+=@_f3(cast(a,m f3_t*)[i],cast(b,m f2_t*)[i]); } break;
            case BKNIT_F332: for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[i]+=@_f3(cast(a,m f3_t*)[i],cast(b,m f3_t*)[i]); } break;
            case BKNIT_F333: for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[i]+=@_f3(cast(a,m f3_t*)[i],cast(b,m f3_t*)[i]); } break;
            default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
        }
    };

    // vector += scalar <op> vector
    body body_sv_av =
    {
        assert( s == 0 || ( a && b && r ) );
        switch( tknit )
        {
            case BKNIT_F222: for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[i]+=@_f2(cast(a,m f2_t*)[0],cast(b,m f2_t*)[i]); } break;
            case BKNIT_F223: for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[i]+=@_f2(cast(a,m f2_t*)[0],cast(b,m f2_t*)[i]); } break;
            case BKNIT_F232: for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[i]+=@_f3(cast(a,m f2_t*)[0],cast(b,m f3_t*)[i]); } break;
            case BKNIT_F233: for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[i]+=@_f3(cast(a,m f2_t*)[0],cast(b,m f3_t*)[i]); } break;
            case BKNIT_F322: for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[i]+=@_f3(cast(a,m f3_t*)[0],cast(b,m f2_t*)[i]); } break;
            case BKNIT_F323: for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[i]+=@_f3(cast(a,m f3_t*)[0],cast(b,m f2_t*)[i]); } break;
            case BKNIT_F332: for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[i]+=@_f3(cast(a,m f3_t*)[0],cast(b,m f3_t*)[i]); } break;
            case BKNIT_F333: for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[i]+=@_f3(cast(a,m f3_t*)[0],cast(b,m f3_t*)[i]); } break;
            default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
        }
    };

    body body_mul    = { return a * b; };
    body body_add    = { return a + b; };
    body body_sub    = { return a - b; };
    body body_div_f3 = { return a * f3_inv( b ); };
    body body_div_f2 = { return a * f2_inv( b ); };

    stamp :sub_sqrsum_s = { func : .f; }; // r = ( a - b )^2
    stamp :sub_l1_s     = { func : .f; }; // r = l1 norm of ( a - b )

    /// multiplication ---------------------------------------------------------

    signature extending :f f_vv_cv();
    signature extending :f f_vv_av();
    signature extending :f f_vs_cv();
    signature extending :f f_vs_av();
    signature extending :f f_vv_as();
    signature extending :f f_vv_cs();

    signature extending :f_m f_mv_cv();
    signature extending :f_m f_mv_av();
    signature extending :f_m f_vm_cv();
    signature extending :f_m f_vm_av();
    signature extending :f_m f_vv_cm();
    signature extending :f_m f_vv_am();

    stamp verbatim :mul_body_s = aware bcore_inst
    {
        func : .f2 = :body_mul; func : .f3 = :body_mul;
        func : .f_vv_cv;
        func : .f_vv_av;
        func : .f_vs_cv;
        func : .f_vs_av;
        func : .f_vv_as;
        func : .f_vv_cs;
        func : .f_mv_cv;
        func : .f_mv_av;
        func : .f_vm_cv;
        func : .f_vm_av;
        func : .f_vv_cm;
        func : .f_vv_am;
    };

    /// mul
    stamp :mul_mvv_s = { func : .f_m = { :mul_body_s_f_mv_cv( tknit, a, b, r, rows, cols ); }; };
    stamp :mul_vmv_s = { func : .f_m = { :mul_body_s_f_vm_cv( tknit, a, b, r, rows, cols ); }; };
    stamp :mul_vvm_s = { func : .f_m = { :mul_body_s_f_vv_cm( tknit, a, b, r, rows, cols ); }; };

    /// mul-accumulate
    stamp :mul_acc_mvv_s = { func : .f_m = { :mul_body_s_f_mv_av( tknit, a, b, r, rows, cols ); }; };
    stamp :mul_acc_vmv_s = { func : .f_m = { :mul_body_s_f_vm_av( tknit, a, b, r, rows, cols ); }; };
    stamp :mul_acc_vvm_s = { func : .f_m = { :mul_body_s_f_vv_am( tknit, a, b, r, rows, cols ); }; };

    /// dendrite pass ----------------------------------------------------------

    stamp :exp_dp_zyf_s = { func : .f2 = :body_mul;          func : .f3 = :body_mul;          func : .f = :body_vv_av; };
    stamp :log_dp_zaf_s = { func : .f2 = :body_div_f2;       func : .f3 = :body_div_f3;       func : .f = :body_vv_av; };
    stamp :inv_dp_zyf_s = { func : .f2 = { return -a*b*b; }; func : .f3 = { return -a*b*b; }; func : .f = :body_vv_av; };
    stamp :sqr_dp_zaf_s = { func : .f2 = { return  2*a*b; }; func : .f3 = { return  2*a*b; }; func : .f = :body_vv_av; };
    stamp :srt_dp_zyf_s = { func : .f2 = :body_div_f2;       func : .f3 = :body_div_f3;       func : .f = :body_vv_av; };

    body body_abs_dp_zaf        = { return b >= 0 ? a : -a; };
    body body_sigm_dp_zyf       = { return b * ( 1.0 - b ) * a; };
    body body_sigm_hard_dp_zyf  = { return ( b <  0.0 ) ? 0.0 : ( b > 1.0 ) ? 0.0 : 0.25 * a; };
    body body_sigm_leaky_dp_zyf = { return ( b <  0.0 ) ? 0.01 * a : ( b > 1.0 ) ? 0.01 * a : 0.25 * a; };
    body body_tanh_dp_zyf       = { return ( 1.0 - f3_sqr( b ) ) * a; };
    body body_tanh_hard_dp_zyf  = { return ( b < -1.0 ) ?  0.0 : ( b > 1.0 ) ? 0.0 : a; };
    body body_tanh_leaky_dp_zyf = { return ( b < -1.0 ) ?  0.01 * a : ( b > 1.0 ) ? 0.01 * a : a; };
    body body_softplus_dp_zyf   = { f3_t u = exp( b ); return a * ( u - 1.0 ) * f3_inv( u ); };
    body body_relu_dp_zyf       = { return b > 0 ? a : 0; };
    body body_relu_leaky_dp_zyf = { return b > 0 ? a : 0.01 * a; };

    stamp :abs_dp_zaf_s        = { func : .f2 = :body_abs_dp_zaf;        func : .f3 = :body_abs_dp_zaf;        func : .f = :body_vv_av; };
    stamp :sigm_dp_zyf_s       = { func : .f2 = :body_sigm_dp_zyf;       func : .f3 = :body_sigm_dp_zyf;       func : .f = :body_vv_av; };
    stamp :sigm_hard_dp_zyf_s  = { func : .f2 = :body_sigm_hard_dp_zyf;  func : .f3 = :body_sigm_hard_dp_zyf;  func : .f = :body_vv_av; };
    stamp :sigm_leaky_dp_zyf_s = { func : .f2 = :body_sigm_leaky_dp_zyf; func : .f3 = :body_sigm_leaky_dp_zyf; func : .f = :body_vv_av; };
    stamp :tanh_dp_zyf_s       = { func : .f2 = :body_tanh_dp_zyf;       func : .f3 = :body_tanh_dp_zyf;       func : .f = :body_vv_av; };
    stamp :tanh_hard_dp_zyf_s  = { func : .f2 = :body_tanh_hard_dp_zyf;  func : .f3 = :body_tanh_hard_dp_zyf;  func : .f = :body_vv_av; };
    stamp :tanh_leaky_dp_zyf_s = { func : .f2 = :body_tanh_leaky_dp_zyf; func : .f3 = :body_tanh_leaky_dp_zyf; func : .f = :body_vv_av; };
    stamp :softplus_dp_zyf_s   = { func : .f2 = :body_softplus_dp_zyf;   func : .f3 = :body_softplus_dp_zyf;   func : .f = :body_vv_av; };
    stamp :relu_dp_zyf_s       = { func : .f2 = :body_relu_dp_zyf;       func : .f3 = :body_relu_dp_zyf;       func : .f = :body_vv_av; };
    stamp :relu_leaky_dp_zyf_s = { func : .f2 = :body_relu_leaky_dp_zyf; func : .f3 = :body_relu_leaky_dp_zyf; func : .f = :body_vv_av; };

    stamp :branch_vvvv_dp_azg_s = { func : .f2 = { return a > 0 ? b : 0; }; func : .f3 = { return a > 0 ? b : 0; }; func : .f = :body_vv_av; };
    stamp :branch_vvvv_dp_azh_s = { func : .f2 = { return a > 0 ? 0 : b; }; func : .f3 = { return a > 0 ? 0 : b; }; func : .f = :body_vv_av; };

    /// logic ------------------------------------------------------------------

    body body_equal         = { return a == b ? 1 : -1; };
    body body_unequal       = { return a != b ? 1 : -1; };
    body body_larger        = { return a >  b ? 1 : -1; };
    body body_smaller       = { return a <  b ? 1 : -1; };
    body body_larger_equal  = { return a >= b ? 1 : -1; };
    body body_smaller_equal = { return a <= b ? 1 : -1; };
    body body_logic_and     = { return ( ( a > 0 ) && ( b > 0 ) ) ? 1 : -1; };
    body body_logic_or      = { return ( ( a > 0 ) || ( b > 0 ) ) ? 1 : -1; };

    stamp :equal_s         = { func : .f2 = :body_equal;         func : .f3 = :body_equal;         func : .f = :body_vv_cv; };
    stamp :unequal_s       = { func : .f2 = :body_unequal;       func : .f3 = :body_unequal;       func : .f = :body_vv_cv; };
    stamp :larger_s        = { func : .f2 = :body_larger;        func : .f3 = :body_larger;        func : .f = :body_vv_cv; };
    stamp :smaller_s       = { func : .f2 = :body_smaller;       func : .f3 = :body_smaller;       func : .f = :body_vv_cv; };
    stamp :larger_equal_s  = { func : .f2 = :body_larger_equal;  func : .f3 = :body_larger_equal;  func : .f = :body_vv_cv; };
    stamp :smaller_equal_s = { func : .f2 = :body_smaller_equal; func : .f3 = :body_smaller_equal; func : .f = :body_vv_cv; };
    stamp :logic_and_s     = { func : .f2 = :body_logic_and;     func : .f3 = :body_logic_and;     func : .f = :body_vv_cv; };
    stamp :logic_or_s      = { func : .f2 = :body_logic_or;      func : .f3 = :body_logic_or;      func : .f = :body_vv_cv; };

};

// ---------------------------------------------------------------------------------------------------------------------

/// ternary operators
group :ar3 =
{
    extending stump verbatim :_s = aware : {};

    signature f2_t f2( f2_t a, f2_t b, f2_t c );
    signature f3_t f3( f3_t a, f3_t b, f3_t c );

    signature void f( tp_t tknit, vc_t a, vc_t b, vc_t c, vd_t r, sz_t s );

    body body_vvv_cv =
    {
        assert( s == 0 || ( a && b && c && r ) );
        switch( tknit )
        {
            case BKNIT_F2222: for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[i]=@_f2(cast(a,m f2_t*)[i],cast(b,m f2_t*)[i],cast(c,m f2_t*)[i]); } break;
            case BKNIT_F2223: for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[i]=@_f2(cast(a,m f2_t*)[i],cast(b,m f2_t*)[i],cast(c,m f2_t*)[i]); } break;
            case BKNIT_F2232: for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[i]=@_f3(cast(a,m f2_t*)[i],cast(b,m f2_t*)[i],cast(c,m f3_t*)[i]); } break;
            case BKNIT_F2233: for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[i]=@_f3(cast(a,m f2_t*)[i],cast(b,m f2_t*)[i],cast(c,m f3_t*)[i]); } break;
            case BKNIT_F2322: for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[i]=@_f3(cast(a,m f2_t*)[i],cast(b,m f3_t*)[i],cast(c,m f2_t*)[i]); } break;
            case BKNIT_F2323: for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[i]=@_f3(cast(a,m f2_t*)[i],cast(b,m f3_t*)[i],cast(c,m f2_t*)[i]); } break;
            case BKNIT_F2332: for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[i]=@_f3(cast(a,m f2_t*)[i],cast(b,m f3_t*)[i],cast(c,m f3_t*)[i]); } break;
            case BKNIT_F2333: for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[i]=@_f3(cast(a,m f2_t*)[i],cast(b,m f3_t*)[i],cast(c,m f3_t*)[i]); } break;
            case BKNIT_F3222: for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[i]=@_f3(cast(a,m f3_t*)[i],cast(b,m f2_t*)[i],cast(c,m f2_t*)[i]); } break;
            case BKNIT_F3223: for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[i]=@_f3(cast(a,m f3_t*)[i],cast(b,m f2_t*)[i],cast(c,m f2_t*)[i]); } break;
            case BKNIT_F3232: for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[i]=@_f3(cast(a,m f3_t*)[i],cast(b,m f2_t*)[i],cast(c,m f3_t*)[i]); } break;
            case BKNIT_F3233: for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[i]=@_f3(cast(a,m f3_t*)[i],cast(b,m f2_t*)[i],cast(c,m f3_t*)[i]); } break;
            case BKNIT_F3322: for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[i]=@_f3(cast(a,m f3_t*)[i],cast(b,m f3_t*)[i],cast(c,m f2_t*)[i]); } break;
            case BKNIT_F3323: for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[i]=@_f3(cast(a,m f3_t*)[i],cast(b,m f3_t*)[i],cast(c,m f2_t*)[i]); } break;
            case BKNIT_F3332: for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[i]=@_f3(cast(a,m f3_t*)[i],cast(b,m f3_t*)[i],cast(c,m f3_t*)[i]); } break;
            case BKNIT_F3333: for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[i]=@_f3(cast(a,m f3_t*)[i],cast(b,m f3_t*)[i],cast(c,m f3_t*)[i]); } break;
            default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
        }
    };

    body body_vvv_av =
    {
        assert( s == 0 || ( a && b && c && r ) );
        switch( tknit )
        {
            case BKNIT_F2222: for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[i]+=@_f2(cast(a,m f2_t*)[i],cast(b,m f2_t*)[i],cast(c,m f2_t*)[i]); } break;
            case BKNIT_F2223: for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[i]+=@_f2(cast(a,m f2_t*)[i],cast(b,m f2_t*)[i],cast(c,m f2_t*)[i]); } break;
            case BKNIT_F2232: for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[i]+=@_f3(cast(a,m f2_t*)[i],cast(b,m f2_t*)[i],cast(c,m f3_t*)[i]); } break;
            case BKNIT_F2233: for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[i]+=@_f3(cast(a,m f2_t*)[i],cast(b,m f2_t*)[i],cast(c,m f3_t*)[i]); } break;
            case BKNIT_F2322: for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[i]+=@_f3(cast(a,m f2_t*)[i],cast(b,m f3_t*)[i],cast(c,m f2_t*)[i]); } break;
            case BKNIT_F2323: for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[i]+=@_f3(cast(a,m f2_t*)[i],cast(b,m f3_t*)[i],cast(c,m f2_t*)[i]); } break;
            case BKNIT_F2332: for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[i]+=@_f3(cast(a,m f2_t*)[i],cast(b,m f3_t*)[i],cast(c,m f3_t*)[i]); } break;
            case BKNIT_F2333: for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[i]+=@_f3(cast(a,m f2_t*)[i],cast(b,m f3_t*)[i],cast(c,m f3_t*)[i]); } break;
            case BKNIT_F3222: for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[i]+=@_f3(cast(a,m f3_t*)[i],cast(b,m f2_t*)[i],cast(c,m f2_t*)[i]); } break;
            case BKNIT_F3223: for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[i]+=@_f3(cast(a,m f3_t*)[i],cast(b,m f2_t*)[i],cast(c,m f2_t*)[i]); } break;
            case BKNIT_F3232: for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[i]+=@_f3(cast(a,m f3_t*)[i],cast(b,m f2_t*)[i],cast(c,m f3_t*)[i]); } break;
            case BKNIT_F3233: for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[i]+=@_f3(cast(a,m f3_t*)[i],cast(b,m f2_t*)[i],cast(c,m f3_t*)[i]); } break;
            case BKNIT_F3322: for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[i]+=@_f3(cast(a,m f3_t*)[i],cast(b,m f3_t*)[i],cast(c,m f2_t*)[i]); } break;
            case BKNIT_F3323: for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[i]+=@_f3(cast(a,m f3_t*)[i],cast(b,m f3_t*)[i],cast(c,m f2_t*)[i]); } break;
            case BKNIT_F3332: for(sz_t i=0; i<s; i++) { cast(r,m f2_t* restrict)[i]+=@_f3(cast(a,m f3_t*)[i],cast(b,m f3_t*)[i],cast(c,m f3_t*)[i]); } break;
            case BKNIT_F3333: for(sz_t i=0; i<s; i++) { cast(r,m f3_t* restrict)[i]+=@_f3(cast(a,m f3_t*)[i],cast(b,m f3_t*)[i],cast(c,m f3_t*)[i]); } break;
            default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
        }
    };

    stamp :branch_vvvv_s = { func : .f2 = { return a > 0 ? b : c; }; func : .f3 = { return a > 0 ? b : c; }; func : .f = :body_vvv_cv; };

    /// dendrite pass ----------------------------------------------------------

};

// ---------------------------------------------------------------------------------------------------------------------
