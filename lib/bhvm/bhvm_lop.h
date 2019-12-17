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

#define BKNIT_FA2( t1, t2 )         ( BKNIT_FA1( t1 )         * 2 + BKNIT_FA1( t2 ) )
#define BKNIT_FA3( t1, t2, t3 )     ( BKNIT_FA2( t1, t2 )     * 2 + BKNIT_FA1( t3 ) )
#define BKNIT_FA4( t1, t2, t3, t4 ) ( BKNIT_FA3( t1, t2, t3 ) * 2 + BKNIT_FA1( t4 ) )

#define BKNIT_FA_L1_FROM_KNIT( knit ) (   knit        & 1 )
#define BKNIT_FA_L2_FROM_KNIT( knit ) ( ( knit >> 1 ) & 1 )
#define BKNIT_FA_L3_FROM_KNIT( knit ) ( ( knit >> 2 ) & 1 )
#define BKNIT_FA_L4_FROM_KNIT( knit ) ( ( knit >> 3 ) & 1 )

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

    signature void f( plain, tp_t tknit, vd_t r, sz_t s );

    // vector
    body body_cv =
    {
        assert( s == 0 || r );
        switch( tknit )
        {
            case BKNIT_F2: { f2_t v = @_f2(); for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=v; } } break;
            case BKNIT_F3: { f3_t v = @_f3(); for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=v; } } break;
            default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
        }
    };

    stamp :zro = { func : :f2 = { return 0; }; func : :f3 = { return 0; }; func : :f = :body_cv; };
    stamp :one = { func : :f2 = { return 1; }; func : :f3 = { return 1; }; func : :f = :body_cv; };

    /// dendrite pass ----------------------------------------------------------

    stamp :nul_dp_v = { func : :f = { /* no action*/ }; };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// unary operators
group :ar1 =
{
    extending stump verbatim :_ = aware : {};

    signature f2_t f2( plain, f2_t a );
    signature f3_t f3( plain, f3_t a );

    signature void f( plain, tp_t tknit, vc_t a, vd_t r, sz_t s );

    // vector <- vector
    body body_v_cv =
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
    body body_v_av =
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
    body body_s_cv =
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

    /// axon pass --------------------------------------------------------------

    stamp :identity = { func : :f2 = { return  a; }; func : :f3 = { return  a; }; func : :f = :body_v_cv; };
    stamp :neg      = { func : :f2 = { return -a; }; func : :f3 = { return -a; }; func : :f = :body_v_cv; };
    stamp :floor    = { func : :f2 = { return  floor(a); }; func : :f3 = { return  floor(a); }; func : :f = :body_v_cv; };
    stamp :ceil     = { func : :f2 = { return   ceil(a); }; func : :f3 = { return   ceil(a); }; func : :f = :body_v_cv; };
    stamp :exp      = { func : :f2 = { return    exp(a); }; func : :f3 = { return    exp(a); }; func : :f = :body_v_cv; };
    stamp :inv      = { func : :f2 = { return f2_inv(a); }; func : :f3 = { return f3_inv(a); }; func : :f = :body_v_cv; };

    body body_lgst       = { return ( a > -700 ) ? ( 1.0 / ( 1.0 + exp( -( f3_t )a ) ) ) : 0; };
    body body_lgst_hard  = { return ( a < -2.0 ) ? 0.0 : ( a > 2.0 ) ? 1.0 : 0.25 * ( a + 2.0 ); };
    body body_lgst_leaky = { return ( a < -2.0 ) ? 0.01 * ( a + 2.0 ) : ( a > 2.0 ) ? 1.0 + 0.01 * ( a - 2.0 ) : 0.25 * ( a + 2.0 ); };
    body body_tanh       = { return ( a < 350 ) ? ( 1.0 - ( 2.0 / ( exp( 2.0 * ( f3_t )a ) + 1.0 ) ) ) : 1.0; };
    body body_tanh_hard  = { return ( a < -1.0 ) ? -1.0 : ( a > 1.0 ) ? 1.0 : a; };
    body body_tanh_leaky = { return ( a < -1.0 ) ? -1.0 + 0.01 * ( a + 1.0 ) : ( a > 1.0 ) ? 1.0 + 0.01 * ( a - 1.0 ) : a; };
    body body_softplus   = { return ( a < 700 ) ? log( 1.0 + exp( ( f3_t )a ) ) : a; };
    body body_relu       = { return a > 0 ? a : 0; };
    body body_relu_leaky = { return a > 0 ? a : a * 0.01; };

    stamp :tanh       = { func : :f2 = :body_$R;  func : :f3 = :body_$R;  func : :f = :body_v_cv; };
    stamp :tanh_hard  = { func : :f2 = :body_$R;  func : :f3 = :body_$R;  func : :f = :body_v_cv; };
    stamp :tanh_leaky = { func : :f2 = :body_$R;  func : :f3 = :body_$R;  func : :f = :body_v_cv; };
    stamp :lgst       = { func : :f2 = :body_$R;  func : :f3 = :body_$R;  func : :f = :body_v_cv; };
    stamp :lgst_hard  = { func : :f2 = :body_$R;  func : :f3 = :body_$R;  func : :f = :body_v_cv; };
    stamp :lgst_leaky = { func : :f2 = :body_$R;  func : :f3 = :body_$R;  func : :f = :body_v_cv; };
    stamp :softplus   = { func : :f2 = :body_$R;  func : :f3 = :body_$R;  func : :f = :body_v_cv; };
    stamp :relu       = { func : :f2 = :body_$R;  func : :f3 = :body_$R;  func : :f = :body_v_cv; };
    stamp :relu_leaky = { func : :f2 = :body_$R;  func : :f3 = :body_$R;  func : :f = :body_v_cv; };

    /// dendrite pass ----------------------------------------------------------

    stamp :identity_dp_v = { func : :f2 = { return  a; }; func : :f3 = { return  a; }; func : :f = :body_v_av; };
    stamp :neg_dp_v      = { func : :f2 = { return -a; }; func : :f3 = { return -a; }; func : :f = :body_v_av; };

    stamp :add_dp_a_v = { func : :f2 = { return  a; }; func : :f3 = { return  a; }; func : :f = :body_v_av; };
    stamp :add_dp_b_v = { func : :f2 = { return  a; }; func : :f3 = { return  a; }; func : :f = :body_v_av; };
    stamp :sub_dp_a_v = { func : :f2 = { return  a; }; func : :f3 = { return  a; }; func : :f = :body_v_av; };
    stamp :sub_dp_b_v = { func : :f2 = { return -a; }; func : :f3 = { return -a; }; func : :f = :body_v_av; };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// binary operators
group :ar2 =
{
    extending stump verbatim :_ = aware : {};

    signature f2_t f2( plain, f2_t a, f2_t b );
    signature f3_t f3( plain, f3_t a, f3_t b );

    signature void f  ( plain, tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s );
    signature void f_m( plain, tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t rows, sz_t cols );

    // vector <- vector <op> vector
    body body_vv_cv =
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
    body body_vv_av =
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
    body body_vs_cv =
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
    body body_vs_av =
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

    // vector <- scalar <op> vector
    body body_sv_cv =
    {
        assert( s == 0 || ( a && b && r ) );
        switch( tknit )
        {
            case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=@_f2(((f2_t*)a)[0],((f2_t*)b)[i]); } break;
            case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=@_f2(((f2_t*)a)[0],((f2_t*)b)[i]); } break;
            case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=@_f3(((f2_t*)a)[0],((f3_t*)b)[i]); } break;
            case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=@_f3(((f2_t*)a)[0],((f3_t*)b)[i]); } break;
            case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=@_f3(((f3_t*)a)[0],((f2_t*)b)[i]); } break;
            case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=@_f3(((f3_t*)a)[0],((f2_t*)b)[i]); } break;
            case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=@_f3(((f3_t*)a)[0],((f3_t*)b)[i]); } break;
            case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=@_f3(((f3_t*)a)[0],((f3_t*)b)[i]); } break;
            default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
        }
    };

    // vector <- scalar <op> vector
    body body_sv_av =
    {
        assert( s == 0 || ( a && b && r ) );
        switch( tknit )
        {
            case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=@_f2(((f2_t*)a)[0],((f2_t*)b)[i]); } break;
            case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=@_f2(((f2_t*)a)[0],((f2_t*)b)[i]); } break;
            case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=@_f3(((f2_t*)a)[0],((f3_t*)b)[i]); } break;
            case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=@_f3(((f2_t*)a)[0],((f3_t*)b)[i]); } break;
            case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=@_f3(((f3_t*)a)[0],((f2_t*)b)[i]); } break;
            case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=@_f3(((f3_t*)a)[0],((f2_t*)b)[i]); } break;
            case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=@_f3(((f3_t*)a)[0],((f3_t*)b)[i]); } break;
            case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=@_f3(((f3_t*)a)[0],((f3_t*)b)[i]); } break;
            default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
        }
    };

    // scalar += vector <op> vector (e.g. dot product)
    body body_vv_as =
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

    // scalar += vector <op> vector (e.g. dot product)
    body body_vv_cs =
    {
        assert( s == 0 || ( a && b && r ) );
        switch( tknit )
        {
            case BKNIT_F222: ((f2_t*)r)[0] = 0; for(sz_t i=0; i<s; i++) { ((f2_t*)r)[0]+=@_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
            case BKNIT_F223: ((f3_t*)r)[0] = 0; for(sz_t i=0; i<s; i++) { ((f3_t*)r)[0]+=@_f2(((f2_t*)a)[i],((f2_t*)b)[i]); } break;
            case BKNIT_F232: ((f2_t*)r)[0] = 0; for(sz_t i=0; i<s; i++) { ((f2_t*)r)[0]+=@_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
            case BKNIT_F233: ((f3_t*)r)[0] = 0; for(sz_t i=0; i<s; i++) { ((f3_t*)r)[0]+=@_f3(((f2_t*)a)[i],((f3_t*)b)[i]); } break;
            case BKNIT_F322: ((f2_t*)r)[0] = 0; for(sz_t i=0; i<s; i++) { ((f2_t*)r)[0]+=@_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
            case BKNIT_F323: ((f3_t*)r)[0] = 0; for(sz_t i=0; i<s; i++) { ((f3_t*)r)[0]+=@_f3(((f3_t*)a)[i],((f2_t*)b)[i]); } break;
            case BKNIT_F332: ((f2_t*)r)[0] = 0; for(sz_t i=0; i<s; i++) { ((f2_t*)r)[0]+=@_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
            case BKNIT_F333: ((f3_t*)r)[0] = 0; for(sz_t i=0; i<s; i++) { ((f3_t*)r)[0]+=@_f3(((f3_t*)a)[i],((f3_t*)b)[i]); } break;
            default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
        }
    };

    body body_mul    = { return a * b; };
    body body_add    = { return a + b; };
    body body_sub    = { return a - b; };
    body body_div_f3 = { return a * f3_inv( b ); };
    body body_div_f2 = { return a * f2_inv( b ); };

    stamp :add = { func : :f2 = :body_add;    func : :f3 = :body_add;    func : :f = :body_vv_cv; };
    stamp :sub = { func : :f2 = :body_sub;    func : :f3 = :body_sub;    func : :f = :body_vv_cv; };
    stamp :div = { func : :f2 = :body_div_f2; func : :f3 = :body_div_f3; func : :f = :body_vv_cv; };

    /// multiplication ---------------------------------------------------------

    signature :f f_vv_cv;
    signature :f f_vv_av;
    signature :f f_vs_cv;
    signature :f f_vs_av;
    signature :f f_vv_as;
    signature :f f_vv_cs;

    signature :f_m f_mv_cv;
    signature :f_m f_mv_av;
    signature :f_m f_vm_cv;
    signature :f_m f_vm_av;
    signature :f_m f_vv_cm;
    signature :f_m f_vv_am;

    stamp verbatim :mul_body = aware bcore_inst
    {
        func : :f2 = :body_mul; func : :f3 = :body_mul;
        func : :f_vv_cv;
        func : :f_vv_av;
        func : :f_vs_cv;
        func : :f_vs_av;
        func : :f_vv_as;
        func : :f_vv_cs;
        func : :f_mv_cv;
        func : :f_mv_av;
        func : :f_vm_cv;
        func : :f_vm_av;
        func : :f_vv_cm;
        func : :f_vv_am;
    };

    /// mul
    stamp :mul_vvv = { func : :f   = { :mul_body_s_f_vv_cv( tknit, a, b, r, s ); }; };
    stamp :mul_vsv = { func : :f   = { :mul_body_s_f_vs_cv( tknit, a, b, r, s ); }; };
    stamp :mul_svv = { func : :f   = { :mul_body_s_f_vs_cv( tknit, b, a, r, s ); }; };
    stamp :mul_vvs = { func : :f   = { :mul_body_s_f_vv_cs( tknit, a, b, r, s ); }; };
    stamp :mul_mvv = { func : :f_m = { :mul_body_s_f_mv_cv( tknit, a, b, r, rows, cols ); }; };
    stamp :mul_tvv = { func : :f_m = { :mul_body_s_f_vm_cv( tknit, b, a, r, rows, cols ); }; };
    stamp :mul_vmv = { func : :f_m = { :mul_body_s_f_vm_cv( tknit, a, b, r, rows, cols ); }; };
    stamp :mul_vtv = { func : :f_m = { :mul_body_s_f_mv_cv( tknit, b, a, r, rows, cols ); }; };
    stamp :mul_vvm = { func : :f_m = { :mul_body_s_f_vv_cm( tknit, a, b, r, rows, cols ); }; };

    /// mul-accumulate
    stamp :mul_acc_vvv = { func : :f   = { :mul_body_s_f_vv_av( tknit, a, b, r, s ); }; };
    stamp :mul_acc_vsv = { func : :f   = { :mul_body_s_f_vs_av( tknit, a, b, r, s ); }; };
    stamp :mul_acc_svv = { func : :f   = { :mul_body_s_f_vs_av( tknit, b, a, r, s ); }; };
    stamp :mul_acc_vvs = { func : :f   = { :mul_body_s_f_vv_as( tknit, a, b, r, s ); }; };
    stamp :mul_acc_mvv = { func : :f_m = { :mul_body_s_f_mv_av( tknit, a, b, r, rows, cols ); }; };
    stamp :mul_acc_tvv = { func : :f_m = { :mul_body_s_f_vm_av( tknit, b, a, r, rows, cols ); }; };
    stamp :mul_acc_vmv = { func : :f_m = { :mul_body_s_f_vm_av( tknit, a, b, r, rows, cols ); }; };
    stamp :mul_acc_vtv = { func : :f_m = { :mul_body_s_f_mv_av( tknit, b, a, r, rows, cols ); }; };
    stamp :mul_acc_vvm = { func : :f_m = { :mul_body_s_f_vv_am( tknit, a, b, r, rows, cols ); }; };

    /// dendrite pass ----------------------------------------------------------

    stamp :div_dp_a_vb = { func : :f2 = { return a * f2_inv( b ); }; func : :f3 = { return a * f3_inv( b ); }; func : :f = :body_vv_av; };

    stamp :exp_dp_vy   = { func : :f2 = :body_mul;          func : :f3 = :body_mul;          func : :f = :body_vv_av; };
    stamp :inv_dp_vy   = { func : :f2 = { return -a*b*b; }; func : :f3 = { return -a*b*b; }; func : :f = :body_vv_av; };

    body body_lgst_dp_vy       = { return b * ( 1.0 - b ) * a; };
    body body_lgst_hard_dp_vy  = { return ( b <  0.0 ) ? 0.0 : ( b > 1.0 ) ? 0.0 : 0.25 * a; };
    body body_lgst_leaky_dp_vy = { return ( b <  0.0 ) ? 0.01 * a : ( b > 1.0 ) ? 0.01 * a : 0.25 * a; };
    body body_tanh_dp_vy       = { return ( 1.0 - f3_sqr( b ) ) * a; };
    body body_tanh_hard_dp_vy  = { return ( b < -1.0 ) ?  0.0 : ( b > 1.0 ) ? 0.0 : a; };
    body body_tanh_leaky_dp_vy = { return ( b < -1.0 ) ?  0.01 * a : ( b > 1.0 ) ? 0.01 * a : a; };
    body body_softplus_dp_vy   = { f3_t u = exp( b ); return a * ( u - 1.0 ) * f3_inv( u ); };
    body body_relu_dp_vy       = { return b > 0 ? a : 0; };
    body body_relu_leaky_dp_vy = { return b > 0 ? a : 0.01 * a; };

    stamp :lgst_dp_vy       = { func : :f2 = :body_$R; func : :f3 = :body_$R; func : :f = :body_vv_av; };
    stamp :lgst_hard_dp_vy  = { func : :f2 = :body_$R; func : :f3 = :body_$R; func : :f = :body_vv_av; };
    stamp :lgst_leaky_dp_vy = { func : :f2 = :body_$R; func : :f3 = :body_$R; func : :f = :body_vv_av; };
    stamp :tanh_dp_vy       = { func : :f2 = :body_$R; func : :f3 = :body_$R; func : :f = :body_vv_av; };
    stamp :tanh_hard_dp_vy  = { func : :f2 = :body_$R; func : :f3 = :body_$R; func : :f = :body_vv_av; };
    stamp :tanh_leaky_dp_vy = { func : :f2 = :body_$R; func : :f3 = :body_$R; func : :f = :body_vv_av; };
    stamp :softplus_dp_vy   = { func : :f2 = :body_$R; func : :f3 = :body_$R; func : :f = :body_vv_av; };
    stamp :relu_dp_vy       = { func : :f2 = :body_$R; func : :f3 = :body_$R; func : :f = :body_vv_av; };
    stamp :relu_leaky_dp_vy = { func : :f2 = :body_$R; func : :f3 = :body_$R; func : :f = :body_vv_av; };

    /// logic ------------------------------------------------------------------

    body body_equal         = { return a == b ? 1 : -1; };
    body body_larger        = { return a >  b ? 1 : -1; };
    body body_smaller       = { return a <  b ? 1 : -1; };
    body body_larger_equal  = { return a >= b ? 1 : -1; };
    body body_smaller_equal = { return a <= b ? 1 : -1; };
    body body_logic_and     = { return ( ( a > 0 ) && ( b > 0 ) ) ? 1 : -1; };
    body body_logic_or      = { return ( ( a > 0 ) || ( b > 0 ) ) ? 1 : -1; };

    stamp :equal         = { func : :f2 = :body_$R; func : :f3 = :body_$R; func : :f = :body_vv_cv; };
    stamp :larger        = { func : :f2 = :body_$R; func : :f3 = :body_$R; func : :f = :body_vv_cv; };
    stamp :smaller       = { func : :f2 = :body_$R; func : :f3 = :body_$R; func : :f = :body_vv_cv; };
    stamp :larger_equal  = { func : :f2 = :body_$R; func : :f3 = :body_$R; func : :f = :body_vv_cv; };
    stamp :smaller_equal = { func : :f2 = :body_$R; func : :f3 = :body_$R; func : :f = :body_vv_cv; };
    stamp :logic_and     = { func : :f2 = :body_$R; func : :f3 = :body_$R; func : :f = :body_vv_cv; };
    stamp :logic_or      = { func : :f2 = :body_$R; func : :f3 = :body_$R; func : :f = :body_vv_cv; };

};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// ternary operators
group :ar3 =
{
    extending stump verbatim :_ = aware : {};

    signature f2_t f2( plain, f2_t a, f2_t b, f2_t c );
    signature f3_t f3( plain, f3_t a, f3_t b, f3_t c );

    signature void f( plain, tp_t tknit, vc_t a, vc_t b, vc_t c, vd_t r, sz_t s );

    body body_vvv_cv =
    {
        assert( s == 0 || ( a && b && c && r ) );
        switch( tknit )
        {
            case BKNIT_F2222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=@_f2(((f2_t*)a)[i],((f2_t*)b)[i],((f2_t*)c)[i]); } break;
            case BKNIT_F2223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=@_f2(((f2_t*)a)[i],((f2_t*)b)[i],((f2_t*)c)[i]); } break;
            case BKNIT_F2232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=@_f3(((f2_t*)a)[i],((f2_t*)b)[i],((f3_t*)c)[i]); } break;
            case BKNIT_F2233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=@_f3(((f2_t*)a)[i],((f2_t*)b)[i],((f3_t*)c)[i]); } break;
            case BKNIT_F2322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=@_f3(((f2_t*)a)[i],((f3_t*)b)[i],((f2_t*)c)[i]); } break;
            case BKNIT_F2323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=@_f3(((f2_t*)a)[i],((f3_t*)b)[i],((f2_t*)c)[i]); } break;
            case BKNIT_F2332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=@_f3(((f2_t*)a)[i],((f3_t*)b)[i],((f3_t*)c)[i]); } break;
            case BKNIT_F2333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=@_f3(((f2_t*)a)[i],((f3_t*)b)[i],((f3_t*)c)[i]); } break;
            case BKNIT_F3222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=@_f3(((f3_t*)a)[i],((f2_t*)b)[i],((f2_t*)c)[i]); } break;
            case BKNIT_F3223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=@_f3(((f3_t*)a)[i],((f2_t*)b)[i],((f2_t*)c)[i]); } break;
            case BKNIT_F3232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=@_f3(((f3_t*)a)[i],((f2_t*)b)[i],((f3_t*)c)[i]); } break;
            case BKNIT_F3233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=@_f3(((f3_t*)a)[i],((f2_t*)b)[i],((f3_t*)c)[i]); } break;
            case BKNIT_F3322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=@_f3(((f3_t*)a)[i],((f3_t*)b)[i],((f2_t*)c)[i]); } break;
            case BKNIT_F3323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=@_f3(((f3_t*)a)[i],((f3_t*)b)[i],((f2_t*)c)[i]); } break;
            case BKNIT_F3332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]=@_f3(((f3_t*)a)[i],((f3_t*)b)[i],((f3_t*)c)[i]); } break;
            case BKNIT_F3333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]=@_f3(((f3_t*)a)[i],((f3_t*)b)[i],((f3_t*)c)[i]); } break;
            default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
        }
    };

    body body_vvv_av =
    {
        assert( s == 0 || ( a && b && c && r ) );
        switch( tknit )
        {
            case BKNIT_F2222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=@_f2(((f2_t*)a)[i],((f2_t*)b)[i],((f2_t*)c)[i]); } break;
            case BKNIT_F2223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=@_f2(((f2_t*)a)[i],((f2_t*)b)[i],((f2_t*)c)[i]); } break;
            case BKNIT_F2232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=@_f3(((f2_t*)a)[i],((f2_t*)b)[i],((f3_t*)c)[i]); } break;
            case BKNIT_F2233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=@_f3(((f2_t*)a)[i],((f2_t*)b)[i],((f3_t*)c)[i]); } break;
            case BKNIT_F2322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=@_f3(((f2_t*)a)[i],((f3_t*)b)[i],((f2_t*)c)[i]); } break;
            case BKNIT_F2323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=@_f3(((f2_t*)a)[i],((f3_t*)b)[i],((f2_t*)c)[i]); } break;
            case BKNIT_F2332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=@_f3(((f2_t*)a)[i],((f3_t*)b)[i],((f3_t*)c)[i]); } break;
            case BKNIT_F2333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=@_f3(((f2_t*)a)[i],((f3_t*)b)[i],((f3_t*)c)[i]); } break;
            case BKNIT_F3222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=@_f3(((f3_t*)a)[i],((f2_t*)b)[i],((f2_t*)c)[i]); } break;
            case BKNIT_F3223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=@_f3(((f3_t*)a)[i],((f2_t*)b)[i],((f2_t*)c)[i]); } break;
            case BKNIT_F3232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=@_f3(((f3_t*)a)[i],((f2_t*)b)[i],((f3_t*)c)[i]); } break;
            case BKNIT_F3233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=@_f3(((f3_t*)a)[i],((f2_t*)b)[i],((f3_t*)c)[i]); } break;
            case BKNIT_F3322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=@_f3(((f3_t*)a)[i],((f3_t*)b)[i],((f2_t*)c)[i]); } break;
            case BKNIT_F3323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=@_f3(((f3_t*)a)[i],((f3_t*)b)[i],((f2_t*)c)[i]); } break;
            case BKNIT_F3332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i]+=@_f3(((f3_t*)a)[i],((f3_t*)b)[i],((f3_t*)c)[i]); } break;
            case BKNIT_F3333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i]+=@_f3(((f3_t*)a)[i],((f3_t*)b)[i],((f3_t*)c)[i]); } break;
            default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
        }
    };

    body body_div_dp_b_vab_f2 = { return -a * b * f2_inv( c * c ); };
    body body_div_dp_b_vab_f3 = { return -a * b * f3_inv( c * c ); };

    stamp :div_dp_b_vab = { func : :f2 = :body_$R_f2; func : :f3 = :body_$R_f3; func : :f = :body_vvv_av; };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // PLANT_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // TYPEOF_bhvm_lop

/**********************************************************************************************************************/

vd_t bhvm_lop_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BHVM_LOP_H
