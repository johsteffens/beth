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

#ifndef BHVM_LOP_H
#define BHVM_LOP_H

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
 *   [o]vf - vectorized pass
 *   suffix: f2 or f3
 *
 */

/**********************************************************************************************************************/

#include "bmath_std.h"
#include "bhvm_planted.h"
#include "bhvm_planted_lop.h"

/**********************************************************************************************************************/

#define SIZEOF_FX( type ) ( ( type == TYPEOF_f2_t ) ? sizeof( f2_t ) : ( type == TYPEOF_f3_t ) ? sizeof( f3_t ) : -1 )

/// type kniting macros
#define BKNIT_FA1( type ) ( ( type == TYPEOF_f2_t ) ? 0 : ( type == TYPEOF_f3_t ) ? 1 : -256 )
#define BKNIT_FA1_ERR( t1 ) ERR_fa( "Invalid type: '#<sc_t>'.", ifnameof( t1 ) )

#define BKNIT_FA2( t1, t2 ) ( BKNIT_FA1( t1 ) * 2 + BKNIT_FA1( t2 ) )
#define BKNIT_FA3( t1, t2, t3 ) ( BKNIT_FA2( t1, t2 ) * 2 + BKNIT_FA1( t3 ) )
#define BKNIT_FA4( t1, t2, t3, t4 ) ( BKNIT_FA3( t1, t2, t3 ) * 2 + BKNIT_FA1( t4 ) )

#define BKNIT_F2 BKNIT_FA1( TYPEOF_f2_t )
#define BKNIT_F3 BKNIT_FA1( TYPEOF_f3_t )

#define BKNIT_F22 BKNIT_FA2( TYPEOF_f2_t, TYPEOF_f2_t )
#define BKNIT_F23 BKNIT_FA2( TYPEOF_f2_t, TYPEOF_f3_t )
#define BKNIT_F32 BKNIT_FA2( TYPEOF_f3_t, TYPEOF_f2_t )
#define BKNIT_F33 BKNIT_FA2( TYPEOF_f3_t, TYPEOF_f3_t )

#define BKNIT_F222 BKNIT_FA3( TYPEOF_f2_t, TYPEOF_f2_t, TYPEOF_f2_t )
#define BKNIT_F223 BKNIT_FA3( TYPEOF_f2_t, TYPEOF_f2_t, TYPEOF_f3_t )
#define BKNIT_F232 BKNIT_FA3( TYPEOF_f2_t, TYPEOF_f3_t, TYPEOF_f2_t )
#define BKNIT_F233 BKNIT_FA3( TYPEOF_f2_t, TYPEOF_f3_t, TYPEOF_f3_t )
#define BKNIT_F322 BKNIT_FA3( TYPEOF_f3_t, TYPEOF_f2_t, TYPEOF_f2_t )
#define BKNIT_F323 BKNIT_FA3( TYPEOF_f3_t, TYPEOF_f2_t, TYPEOF_f3_t )
#define BKNIT_F332 BKNIT_FA3( TYPEOF_f3_t, TYPEOF_f3_t, TYPEOF_f2_t )
#define BKNIT_F333 BKNIT_FA3( TYPEOF_f3_t, TYPEOF_f3_t, TYPEOF_f3_t )

#define BKNIT_F2222 BKNIT_FA4( TYPEOF_f2_t, TYPEOF_f2_t, TYPEOF_f2_t, TYPEOF_f2_t )
#define BKNIT_F2223 BKNIT_FA4( TYPEOF_f2_t, TYPEOF_f2_t, TYPEOF_f2_t, TYPEOF_f3_t )
#define BKNIT_F2232 BKNIT_FA4( TYPEOF_f2_t, TYPEOF_f2_t, TYPEOF_f3_t, TYPEOF_f2_t )
#define BKNIT_F2233 BKNIT_FA4( TYPEOF_f2_t, TYPEOF_f2_t, TYPEOF_f3_t, TYPEOF_f3_t )
#define BKNIT_F2322 BKNIT_FA4( TYPEOF_f2_t, TYPEOF_f3_t, TYPEOF_f2_t, TYPEOF_f2_t )
#define BKNIT_F2323 BKNIT_FA4( TYPEOF_f2_t, TYPEOF_f3_t, TYPEOF_f2_t, TYPEOF_f3_t )
#define BKNIT_F2332 BKNIT_FA4( TYPEOF_f2_t, TYPEOF_f3_t, TYPEOF_f3_t, TYPEOF_f2_t )
#define BKNIT_F2333 BKNIT_FA4( TYPEOF_f2_t, TYPEOF_f3_t, TYPEOF_f3_t, TYPEOF_f3_t )
#define BKNIT_F3222 BKNIT_FA4( TYPEOF_f3_t, TYPEOF_f2_t, TYPEOF_f2_t, TYPEOF_f2_t )
#define BKNIT_F3223 BKNIT_FA4( TYPEOF_f3_t, TYPEOF_f2_t, TYPEOF_f2_t, TYPEOF_f3_t )
#define BKNIT_F3232 BKNIT_FA4( TYPEOF_f3_t, TYPEOF_f2_t, TYPEOF_f3_t, TYPEOF_f2_t )
#define BKNIT_F3233 BKNIT_FA4( TYPEOF_f3_t, TYPEOF_f2_t, TYPEOF_f3_t, TYPEOF_f3_t )
#define BKNIT_F3322 BKNIT_FA4( TYPEOF_f3_t, TYPEOF_f3_t, TYPEOF_f2_t, TYPEOF_f2_t )
#define BKNIT_F3323 BKNIT_FA4( TYPEOF_f3_t, TYPEOF_f3_t, TYPEOF_f2_t, TYPEOF_f3_t )
#define BKNIT_F3332 BKNIT_FA4( TYPEOF_f3_t, TYPEOF_f3_t, TYPEOF_f3_t, TYPEOF_f2_t )
#define BKNIT_F3333 BKNIT_FA4( TYPEOF_f3_t, TYPEOF_f3_t, TYPEOF_f3_t, TYPEOF_f3_t )

/**********************************************************************************************************************/

#ifdef TYPEOF_bhvm_lop

PLANT_GROUP( bhvm_lop, bcore_inst )
#ifdef PLANT_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// nullary operators
group :ar0 =
{
    extending stump verbatim :_ = aware : {};

    signature f2_t f2( plain );
    signature f3_t f3( plain );

    signature void knit( plain, tp_t tknit, vd_t r, sz_t s );

    signature : :knit k_cv;

    // vector
    body body_k_cv =
    {
        assert( s == 0 || r );
        switch( tknit )
        {
            case BKNIT_F2: { f2_t v = @_f2(); for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=v; } } break;
            case BKNIT_F3: { f3_t v = @_f3(); for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=v; } } break;
            default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
        }
    };

    /// k_cv applies to all stamps
    func : :k_cv = :body_k_cv;

    stamp :zro = { func : :f2 = { return 0; }; func : :f3 = { return 0; }; };
    stamp :one = { func : :f2 = { return 1; }; func : :f3 = { return 1; }; };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// unary operators
group :ar1 =
{
    extending stump verbatim :_ = aware : {};

    signature f2_t f2( plain, f2_t a );
    signature f3_t f3( plain, f3_t a );

    signature void knit( plain, tp_t tknit, vc_t a, vd_t r, sz_t s );

    signature : :knit k_v_cv;
    signature : :knit k_s_cv;
    signature : :knit k_v_av;
    signature : :knit k_s_av;
    signature : :knit k_v_as;

    // vector <- vector
    body body_k_v_cv =
    {
        assert( s == 0 || ( a && r ) );
        switch( tknit )
        {
            case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=@_f2(((f2_t*)a)[i]); } break;
            case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=@_f2(((f2_t*)a)[i]); } break;
            case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=@_f3(((f3_t*)a)[i]); } break;
            case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=@_f3(((f3_t*)a)[i]); } break;
            default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
        }
    };

    // vector <- vector
    body body_k_v_av =
    {
        assert( s == 0 || ( a && r ) );
        switch( tknit )
        {
            case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=@_f2(((f2_t*)a)[i]); } break;
            case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=@_f2(((f2_t*)a)[i]); } break;
            case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=@_f3(((f3_t*)a)[i]); } break;
            case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=@_f3(((f3_t*)a)[i]); } break;
            default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
        }
    };

    // vector <- scalar
    body body_k_s_cv =
    {
        assert( s == 0 || ( a && r ) );
        switch( tknit )
        {
            case BKNIT_F22: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=@_f2(((f2_t*)a)[0]); } break;
            case BKNIT_F23: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=@_f2(((f2_t*)a)[0]); } break;
            case BKNIT_F32: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=@_f3(((f3_t*)a)[0]); } break;
            case BKNIT_F33: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=@_f3(((f3_t*)a)[0]); } break;
            default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
        }
    };

    /// k_vv_cv applies to all stamps
    func : :k_v_cv = :body_k_v_cv;

    stamp :identity = { func : :f2 = { return        a ; }; func : :f3 = { return        a ; }; func : :k_s_cv = :body_k_s_cv; };
    stamp :neg      = { func : :f2 = { return       -a ; }; func : :f3 = { return       -a ; }; };
    stamp :floor    = { func : :f2 = { return  floor(a); }; func : :f3 = { return  floor(a); }; };
    stamp :ceil     = { func : :f2 = { return   ceil(a); }; func : :f3 = { return   ceil(a); }; };
    stamp :exp      = { func : :f2 = { return    exp(a); }; func : :f3 = { return    exp(a); }; };
    stamp :inv      = { func : :f2 = { return f2_inv(a); }; func : :f3 = { return f3_inv(a); }; };

    body body_add_dp_a_v = { return a; };
    body body_add_dp_b_v = { return a; };
    stamp :add_dp_a_v    = { func : :f2 = :body_add_dp_a_v; func : :f3 = :body_add_dp_a_v; func : :k_v_av = :body_k_v_av; };
    stamp :add_dp_b_v    = { func : :f2 = :body_add_dp_b_v; func : :f3 = :body_add_dp_b_v; func : :k_v_av = :body_k_v_av;};

    body body_sub_dp_a_v = { return  a; };
    body body_sub_dp_b_v = { return -a; };
    stamp :sub_dp_a_v    = { func : :f2 = :body_sub_dp_a_v; func : :f3 = :body_sub_dp_a_v; func : :k_v_av = :body_k_v_av; };
    stamp :sub_dp_b_v    = { func : :f2 = :body_sub_dp_b_v; func : :f3 = :body_sub_dp_b_v; func : :k_v_av = :body_k_v_av; };

    body body_lgst_f3    = { return ( a > -700 ) ? ( 1.0 / ( 1.0 + exp( -( f3_t )a ) ) ) : 0; };
    body body_lgst_hard  = { return ( a < -2.0 ) ? 0.0 : ( a > 2.0 ) ? 1.0 : 0.25 * ( a + 2.0 ); };
    body body_lgst_leaky = { return ( a < -2.0 ) ? 0.01 * ( a + 2.0 ) : ( a > 2.0 ) ? 1.0 + 0.01 * ( a - 2.0 ) : 0.25 * ( a + 2.0 ); };

    stamp :lgst       = { func : :f2 = :body_lgst_f3;         func : :f3 = :body_lgst_f3; };
    stamp :lgst_hard  = { func : :f2 = :body_lgst_hard;       func : :f3 = :body_lgst_hard; };
    stamp :lgst_leaky = { func : :f2 = :body_lgst_leaky;      func : :f3 = :body_lgst_leaky; };

    body body_tanh_f3    = { return ( a < 350 ) ? ( 1.0 - ( 2.0 / ( exp( 2.0 * ( f3_t )a ) + 1.0 ) ) ) : 1.0; };
    body body_tanh_hard  = { return ( a < -1.0 ) ? -1.0 : ( a > 1.0 ) ? 1.0 : a; };
    body body_tanh_leaky = { return ( a < -1.0 ) ? -1.0 + 0.01 * ( a + 1.0 ) : ( a > 1.0 ) ? 1.0 + 0.01 * ( a - 1.0 ) : a; };

    stamp :tanh       = { func : :f2 = :body_tanh_f3;         func : :f3 = :body_tanh_f3; };
    stamp :tanh_hard  = { func : :f2 = :body_tanh_hard;       func : :f3 = :body_tanh_hard; };
    stamp :tanh_leaky = { func : :f2 = :body_tanh_leaky;      func : :f3 = :body_tanh_leaky; };

    body body_softplus_f3 = { return ( a < 700 ) ? log( 1.0 + exp( ( f3_t )a ) ) : a; };
    body body_relu        = { return a > 0 ? a : 0; };
    body body_relu_leaky  = { return a > 0 ? a : a * 0.01; };

    stamp :softplus   = { func : :f2 = :body_softplus_f3;     func : :f3 = :body_softplus_f3; };
    stamp :relu       = { func : :f2 = :body_relu;            func : :f3 = :body_relu; };
    stamp :relu_leaky = { func : :f2 = :body_relu_leaky;      func : :f3 = :body_relu_leaky; };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// binary operators
group :ar2 =
{
    extending stump verbatim :_ = aware : {};

    signature f2_t f2( plain, f2_t a, f2_t b );
    signature f3_t f3( plain, f3_t a, f3_t b );

    signature void knit( plain, tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );

    signature : :knit k_vv_cv;
    signature : :knit k_vs_cv;
    signature : :knit k_vv_av;
    signature : :knit k_vs_av;
    signature : :knit k_vv_as;

    func : : f2 = { return @_f3( a, b ); };

    // vector <- vector <op> vector
    body body_k_vv_cv =
    {
        assert( s == 0 || ( a && b && r ) );
        switch( tknit )
        {
            case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=@_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
            case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=@_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
            case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=@_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
            case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=@_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
            case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=@_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
            case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=@_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
            case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=@_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
            case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=@_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
            default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
        }
    };

    // vector += vector <op> vector
    body body_k_vv_av =
    {
        assert( s == 0 || ( a && b && r ) );
        switch( tknit )
        {
            case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=@_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
            case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=@_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
            case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=@_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
            case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=@_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
            case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=@_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
            case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=@_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
            case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=@_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
            case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=@_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
            default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
        }
    };

    // vector <- vector <op> scalar
    body body_k_vs_cv =
    {
        assert( s == 0 || ( a && b && r ) );
        switch( tknit )
        {
            case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=@_f2(((f2_t*)a)[i],((f2_t*)b)[0]); } break;
            case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=@_f2(((f2_t*)a)[i],((f2_t*)b)[0]); } break;
            case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=@_f3(((f2_t*)a)[i],((f3_t*)b)[0]); } break;
            case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=@_f3(((f2_t*)a)[i],((f3_t*)b)[0]); } break;
            case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=@_f3(((f3_t*)a)[i],((f2_t*)b)[0]); } break;
            case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=@_f3(((f3_t*)a)[i],((f2_t*)b)[0]); } break;
            case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=@_f3(((f3_t*)a)[i],((f3_t*)b)[0]); } break;
            case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=@_f3(((f3_t*)a)[i],((f3_t*)b)[0]); } break;
            default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
        }
    };

    // vector += vector <op> scalar
    body body_k_vs_av =
    {
        assert( s == 0 || ( a && b && r ) );
        switch( tknit )
        {
            case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=@_f2(((f2_t*)a)[i],((f2_t*)b)[0]); } break;
            case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=@_f2(((f2_t*)a)[i],((f2_t*)b)[0]); } break;
            case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=@_f3(((f2_t*)a)[i],((f3_t*)b)[0]); } break;
            case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=@_f3(((f2_t*)a)[i],((f3_t*)b)[0]); } break;
            case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=@_f3(((f3_t*)a)[i],((f2_t*)b)[0]); } break;
            case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=@_f3(((f3_t*)a)[i],((f2_t*)b)[0]); } break;
            case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=@_f3(((f3_t*)a)[i],((f3_t*)b)[0]); } break;
            case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=@_f3(((f3_t*)a)[i],((f3_t*)b)[0]); } break;
            default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
        }
    };

    // scalar += vector <op> vector (e.g. dot product)  (not that target needs to be initialized zero for a pure dot product)
    body body_k_vv_as =
    {
        assert( s == 0 || ( a && b && r ) );
        switch( tknit )
        {
            case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[0]+=@_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
            case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[0]+=@_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
            case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[0]+=@_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
            case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[0]+=@_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
            case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[0]+=@_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
            case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[0]+=@_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
            case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[0]+=@_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
            case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[0]+=@_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
            default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
        }
    };

    /// k_vv_cv applies to all stamps
    func : :k_vv_cv = :body_k_vv_cv;

    body body_add    = { return a + b; };
    body body_sub    = { return a - b; };
    body body_mul    = { return a * b; };
    body body_div_f3 = { return a * f3_inv( b ); };
    body body_div_f2 = { return a * f2_inv( b ); };

    stamp :add =
    {
        func : :f2 = :body_add;
        func : :f3 = :body_add;
        func : :k_vs_cv = :body_k_vs_cv;
    };

    stamp :sub =
    {
        func : :f2 = :body_sub;
        func : :f3 = :body_sub;
        func : :k_vs_cv = :body_k_vs_cv;
    };

    stamp :mul =
    {
        func : :f2 = :body_mul;
        func : :f3 = :body_mul;
        func : :k_vs_cv = :body_k_vs_cv;
        func : :k_vv_as = :body_k_vv_as;
        func : :k_vv_av = :body_k_vv_av;
        func : :k_vs_av = :body_k_vs_av;
    };

    body body_mul_dp_a_vb = { return a * b; };
    body body_mul_dp_b_va = { return a * b; };
    stamp :mul_dp_a_vb    = { func : :f2 = :body_mul_dp_a_vb; func : :f3 = :body_mul_dp_a_vb; func : :k_vv_av = :body_k_vv_av; };
    stamp :mul_dp_b_va    = { func : :f2 = :body_mul_dp_b_va; func : :f3 = :body_mul_dp_b_va; func : :k_vv_av = :body_k_vv_av; };

    stamp :div =
    {
        func : :f2 = :body_div_f2;
        func : :f3 = :body_div_f3;
    };

    body body_div_dp_a_vb_f2 = { return a * f2_inv( b ); };
    body body_div_dp_a_vb_f3 = { return a * f3_inv( b ); };
    stamp :div_dp_a_vb = { func : :f2 = :body_div_dp_a_vb_f2; func : :f3 = :body_div_dp_a_vb_f3; func : :k_vv_av = :body_k_vv_av; };
    /// TODO: div_dp_b_vab  -- as ar3 operation

    stamp :exp_dp_vy = { func : :f2 = { return    b*a; }; func : :f3 = { return    b*a; }; func : :k_vv_av = :body_k_vv_av; };
    stamp :inv_dp_vy = { func : :f2 = { return -b*b*a; }; func : :f3 = { return -b*b*a; }; func : :k_vv_av = :body_k_vv_av; };

    body body_lgst_dp_vy       = { return b * ( 1.0 - b ) * a; };
    body body_lgst_hard_dp_vy  = { return ( b <  0.0 ) ? 0.0 : ( b > 1.0 ) ? 0.0 : 0.25 * a; };
    body body_lgst_leaky_dp_vy = { return ( b <  0.0 ) ? 0.01 * a : ( b > 1.0 ) ? 0.01 * a : 0.25 * a; };

    stamp :lgst_dp_vy       = { func : :f2 = :body_lgst_dp_vy;       func : :f3 = :body_lgst_dp_vy;       func : :k_vv_av = :body_k_vv_av; };
    stamp :lgst_hard_dp_vy  = { func : :f2 = :body_lgst_hard_dp_vy;  func : :f3 = :body_lgst_hard_dp_vy;  func : :k_vv_av = :body_k_vv_av; };
    stamp :lgst_leaky_dp_vy = { func : :f2 = :body_lgst_leaky_dp_vy; func : :f3 = :body_lgst_leaky_dp_vy; func : :k_vv_av = :body_k_vv_av; };

    body body_tanh_dp_vy       = { return ( 1.0 - f3_sqr( b ) ) * a; };
    body body_tanh_hard_dp_vy  = { return ( b < -1.0 ) ?  0.0 : ( b > 1.0 ) ? 0.0 : a; };
    body body_tanh_leaky_dp_vy = { return ( b < -1.0 ) ?  0.01 * a : ( b > 1.0 ) ? 0.01 * a : a; };

    stamp :tanh_dp_vy       = { func : :f2 = :body_tanh_dp_vy;       func : :f3 = :body_tanh_dp_vy;       func : :k_vv_av = :body_k_vv_av; };
    stamp :tanh_hard_dp_vy  = { func : :f2 = :body_tanh_hard_dp_vy;  func : :f3 = :body_tanh_hard_dp_vy;  func : :k_vv_av = :body_k_vv_av; };
    stamp :tanh_leaky_dp_vy = { func : :f2 = :body_tanh_leaky_dp_vy; func : :f3 = :body_tanh_leaky_dp_vy; func : :k_vv_av = :body_k_vv_av; };

    body body_softplus_dp_vy   = { f3_t u = exp( b ); return a * ( u - 1.0 ) * f3_inv( u ); };
    body body_relu_dp_vy       = { return b > 0 ? a : 0; };
    body body_relu_leaky_dp_vy = { return b > 0 ? a : 0.01 * a; };

    stamp :softplus_dp_vy   = { func : :f2 = :body_softplus_dp_vy;   func : :f3 = :body_softplus_dp_vy;   func : :k_vv_av = :body_k_vv_av; };
    stamp :relu_dp_vy       = { func : :f2 = :body_relu_dp_vy;       func : :f3 = :body_relu_dp_vy;       func : :k_vv_av = :body_k_vv_av; };
    stamp :relu_leaky_dp_vy = { func : :f2 = :body_relu_leaky_dp_vy; func : :f3 = :body_relu_leaky_dp_vy; func : :k_vv_av = :body_k_vv_av; };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // PLANT_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // TYPEOF_bhvm_lop

/**********************************************************************************************************************/

vd_t bhvm_lop_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BHVM_LOP_H
