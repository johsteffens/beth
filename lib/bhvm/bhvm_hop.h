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

#ifndef BHVM_HOP_H
#define BHVM_HOP_H

/**********************************************************************************************************************/

/** Collection of Holor-Operators
 *
 */

/**********************************************************************************************************************/

#include "bmath_std.h"
#include "bhvm_planted.h"
#include "bhvm_holor.h"

/**********************************************************************************************************************/

#ifdef TYPEOF_bhvm_hop

PLANT_GROUP( bhvm_hop, bcore_inst )
#ifdef PLANT_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// nullary operators
group :ar0 =
{
    extending stump verbatim :_ = aware : {};
    signature void f( plain, bhvm_holor_s* r );

    body body_lop_cv =
    {
        bhvm_lop_ar0_$R_s_f( BKNIT_FA1( r->v.type ), r->v.data, r->v.size );
    };

    body body_lop_av =
    {
        bhvm_lop_ar0_$R_s_f( BKNIT_FA1( r->v.type ), r->v.data, r->v.size );
    };

    /// axon pass --------------------------------------------------------------

    stamp :zro = { func : :f = :body_lop_cv; };
    stamp :one = { func : :f = :body_lop_cv; };

    /// dendrite pass ----------------------------------------------------------

    stamp :nul_dp = { func : :f = { /* no action */ }; };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// unary operators
group :ar1 =
{
    extending stump verbatim :_ = aware : {};
    signature void f( plain, const bhvm_holor_s* a, bhvm_holor_s* r );

    body body_lop_v_cv =
    {
        assert( a->v.size == r->v.size );
        bhvm_lop_ar1_$R_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
    };

    body body_lop_v_cs =
    {
        assert( r->v.size == 1 );
        bhvm_lop_ar1_$R_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, a->v.size );
    };

    body body_lop_v_av =
    {
        assert( a->v.size == r->v.size );
        bhvm_lop_ar1_$R_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size );
    };

    /// axon pass --------------------------------------------------------------

    stamp :cpy        = { func : :f = :body_lop_v_cv; };
    stamp :identity   = { func : :f = :body_lop_v_cv; };
    stamp :neg        = { func : :f = :body_lop_v_cv; };
    stamp :floor      = { func : :f = :body_lop_v_cv; };
    stamp :ceil       = { func : :f = :body_lop_v_cv; };
    stamp :exp        = { func : :f = :body_lop_v_cv; };
    stamp :log        = { func : :f = :body_lop_v_cv; };
    stamp :inv        = { func : :f = :body_lop_v_cv; };
    stamp :abs        = { func : :f = :body_lop_v_cv; };
    stamp :sig        = { func : :f = :body_lop_v_cv; };
    stamp :l1         = { func : :f = :body_lop_v_cs; };
    stamp :sqr        = { func : :f = :body_lop_v_cv; };
    stamp :srt        = { func : :f = :body_lop_v_cv; };
    stamp :sqrsum     = { func : :f = :body_lop_v_cs; };
    stamp :lgst       = { func : :f = :body_lop_v_cv; };
    stamp :lgst_hard  = { func : :f = :body_lop_v_cv; };
    stamp :lgst_leaky = { func : :f = :body_lop_v_cv; };
    stamp :tanh       = { func : :f = :body_lop_v_cv; };
    stamp :tanh_hard  = { func : :f = :body_lop_v_cv; };
    stamp :tanh_leaky = { func : :f = :body_lop_v_cv; };
    stamp :softplus   = { func : :f = :body_lop_v_cv; };
    stamp :relu       = { func : :f = :body_lop_v_cv; };
    stamp :relu_leaky = { func : :f = :body_lop_v_cv; };

    /// dendrite pass ----------------------------------------------------------

    stamp :identity_dp_zf = { func : :f = { assert( a->v.size == r->v.size ); bhvm_lop_ar1_acc_s_f(  BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size ); }; };
    stamp :neg_dp_zf      = { func : :f = { assert( a->v.size == r->v.size ); bhvm_lop_ar1_accn_s_f( BKNIT_FA2( a->v.type, r->v.type ), a->v.data, r->v.data, r->v.size ); }; };

    stamp :cat_dp_zf = { func : :f = { bhvm_value_s_acc_offs( &a->v, 0, &r->v, 0, r->v.size ); }; };
    stamp :cat_dp_zg = { func : :f = { bhvm_value_s_acc_offs( &a->v, a->v.size - r->v.size, &r->v, 0, r->v.size ); }; };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// extended unary operators (elementary cyclic indexing)
group :ar1_eci =
{
    extending stump verbatim :_ = aware : {};

    signature f2_t f2( plain, f2_t a );
    signature f3_t f3( plain, f3_t a );
    signature void f( plain, const bhvm_holor_s* a, bhvm_holor_s* r );

    body body_acc =
    {
        assert( a != r );
        assert(  sz_min( a->v.size, r->v.size ) > 0 );
        sz_t n = sz_gcd( a->v.size, r->v.size );
        sz_t m = sz_max( a->v.size, r->v.size ) / n;


        #define :ACC_CASE( TA_T, TR_T, FUNC ) \
        { \
            const TA_T *a0 = a->v.data, *a1 = a0; \
                  TR_T *r0 = r->v.data, *r1 = r0; \
            for( sz_t i = 0; i < m; i++ ) \
            { \
                for( sz_t i = 0; i < n; i++ ) r1[ i ] += FUNC( a1[ i ] ); \
                a1 = ( a1 + n - a0 ) < a->v.size ? ( a1 + n ) : a0; \
                r1 = ( r1 + n - r0 ) < r->v.size ? ( r1 + n ) : r0; \
            } \
        }

        switch( BKNIT_FA2( a->v.type, r->v.type ) )
        {
            case BKNIT_F22: :ACC_CASE( f2_t, f2_t, @_f2 ); break;
            case BKNIT_F23: :ACC_CASE( f2_t, f3_t, @_f3 ); break;
            case BKNIT_F32: :ACC_CASE( f3_t, f2_t, @_f3 ); break;
            case BKNIT_F33: :ACC_CASE( f3_t, f3_t, @_f3 ); break;
            default: BKNIT_FA2_ERR( a->v.type, r->v.type ); break;
        }

        #undef :ACC_CASE
    };

    body body_std =
    {
        if( a == r )
        {
            bhvm_holor_s* buf = bhvm_holor_s_create();
            :$R_acc_s_f( a, bhvm_holor_s_fit_size( bhvm_holor_s_copy_shape_type( buf, r ) ) );
            if( r->v.size ) bhvm_value_s_cpy( &buf->v, &r->v );
            bhvm_holor_s_discard( buf );
        }
        else
        {
            bhvm_value_s_zro( &r->v );
            :$R_acc_s_f( a, r );
        }
    };

    stamp :cpy_acc = { func : :f2 = { return  a; };          func : :f3 = { return  a; };          func : :f = :body_acc; };
    stamp :neg_acc = { func : :f2 = { return -a; };          func : :f3 = { return -a; };          func : :f = :body_acc; };
    stamp :inv_acc = { func : :f2 = { return f2_inv( a ); }; func : :f3 = { return f3_inv( a ); }; func : :f = :body_acc; };

    stamp :cpy = { func : :f2 = { return  a; };          func : :f3 = { return  a; };          func : :f = :body_std; };
    stamp :neg = { func : :f2 = { return -a; };          func : :f3 = { return -a; };          func : :f = :body_std; };
    stamp :inv = { func : :f2 = { return f2_inv( a ); }; func : :f3 = { return f3_inv( a ); }; func : :f = :body_std; };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// binary operators
group :ar2 =
{
    extending stump verbatim :_ = aware : {};
    signature void f( plain, const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );

    body body_assert_vvv = { assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size ); };
    body body_assert_vsv = { assert( a->v.size == r->v.size ); assert( b->v.size == 1         ); };
    body body_assert_svv = { assert( a->v.size == 1         ); assert( b->v.size == r->v.size ); };
    body body_assert_vvs = { assert( a->v.size == b->v.size ); assert( r->v.size == 1         ); };

    body body_assert_mvv = { assert( a->s.size == 2 ); assert( b->v.size == a->s.data[ 0 ] ); assert( r->v.size == a->s.data[ 1 ] ); };
    body body_assert_vmv = { assert( b->s.size == 2 ); assert( a->v.size == b->s.data[ 1 ] ); assert( r->v.size == b->s.data[ 0 ] ); };
    body body_assert_vvm = { assert( r->s.size == 2 ); assert( a->v.size == r->s.data[ 1 ] ); assert( b->v.size == r->s.data[ 0 ] ); };

    body body_assert_tvv = { assert( a->s.size == 2 ); assert( b->v.size == a->s.data[ 1 ] ); assert( r->v.size == a->s.data[ 0 ] ); };
    body body_assert_vtv = { assert( b->s.size == 2 ); assert( a->v.size == b->s.data[ 0 ] ); assert( r->v.size == b->s.data[ 1 ] ); };
    body body_assert_vvt = { assert( r->s.size == 2 ); assert( a->v.size == r->s.data[ 0 ] ); assert( b->v.size == r->s.data[ 1 ] ); };

    body body_lop_r = { bhvm_lop_ar2_$R_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size ); }; // r has full size
    body body_lop_r_x = { bhvm_lop_ar2_$R_s_f_x( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, a->v.size, b->v.size, r->v.size ); }; // r has full size

    body body_lop_a = { bhvm_lop_ar2_$R_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, a->v.size ); }; // a has full size

    body body_lop_ma = { bhvm_lop_ar2_$R_s_f_m( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, a->s.data[ 1 ], a->s.data[ 0 ] ); }; // a is matrix
    body body_lop_mb = { bhvm_lop_ar2_$R_s_f_m( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, b->s.data[ 1 ], b->s.data[ 0 ] ); }; // b is matrix
    body body_lop_mr = { bhvm_lop_ar2_$R_s_f_m( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->s.data[ 1 ], r->s.data[ 0 ] ); }; // r is matrix

    /// axon pass --------------------------------------------------------------

    stamp :sub_sqrsum = { func : :f = :body_assert_vvs : :body_lop_a; }; // r = ( a - b )^2
    stamp :sub_l1     = { func : :f = :body_assert_vvs : :body_lop_a; }; // r = l1-norm of ( a - b )

    /// logic ------------------------------------------------------------------

    stamp :equal         = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :larger        = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :smaller       = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :larger_equal  = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :smaller_equal = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :logic_and     = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :logic_or      = { func : :f = :body_assert_vvv : :body_lop_r; };

    /// dendrite pass ----------------------------------------------------------

    stamp :abs_dp_zaf        = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :exp_dp_zyf        = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :log_dp_zaf        = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :inv_dp_zyf        = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :sqr_dp_zaf        = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :srt_dp_zyf        = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :lgst_dp_zyf       = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :lgst_hard_dp_zyf  = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :lgst_leaky_dp_zyf = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :tanh_dp_zyf       = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :tanh_hard_dp_zyf  = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :tanh_leaky_dp_zyf = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :softplus_dp_zyf   = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :relu_dp_zyf       = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :relu_leaky_dp_zyf = { func : :f = :body_assert_vvv : :body_lop_r; };

    stamp :branch_vvvv_dp_azg = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :branch_vvvv_dp_azh = { func : :f = :body_assert_vvv : :body_lop_r; };

    /// mul axon pass ----------------------------------------------------------

    stamp :mul_mvv = { func : :f = :body_assert_mvv : :body_lop_ma; };
    stamp :mul_vmv = { func : :f = :body_assert_vmv : :body_lop_mb; };
    stamp :mul_vvm = { func : :f = :body_assert_vvm : :body_lop_mr; };
    stamp :mul_tvv = { func : :f = :body_assert_tvv : { bhvm_lop_ar2_mul_vmv_s_f_m( BKNIT_FA3( b->v.type, a->v.type, r->v.type ), b->v.data, a->v.data, r->v.data, a->s.data[ 1 ], a->s.data[ 0 ] ); }; };
    stamp :mul_vtv = { func : :f = :body_assert_vtv : { bhvm_lop_ar2_mul_mvv_s_f_m( BKNIT_FA3( b->v.type, a->v.type, r->v.type ), b->v.data, a->v.data, r->v.data, b->s.data[ 1 ], b->s.data[ 0 ] ); }; };

    stamp :mul_mmm = { func : :f; };
    stamp :mul_mtm = { func : :f; };
    stamp :mul_tmm = { func : :f; };
    stamp :mul_ttm = { func : :f; };

    /// mul accumulate ---------------------------------------------------------

    stamp :mul_acc_mvv = { func : :f = :body_assert_mvv : :body_lop_ma; };
    stamp :mul_acc_vmv = { func : :f = :body_assert_vmv : :body_lop_mb; };
    stamp :mul_acc_tvv = { func : :f = :body_assert_tvv : { bhvm_lop_ar2_mul_acc_vmv_s_f_m( BKNIT_FA3( b->v.type, a->v.type, r->v.type ), b->v.data, a->v.data, r->v.data, a->s.data[ 1 ], a->s.data[ 0 ] ); }; };
    stamp :mul_acc_vtv = { func : :f = :body_assert_vtv : { bhvm_lop_ar2_mul_acc_mvv_s_f_m( BKNIT_FA3( b->v.type, a->v.type, r->v.type ), b->v.data, a->v.data, r->v.data, b->s.data[ 1 ], b->s.data[ 0 ] ); }; };
    stamp :mul_acc_vvm = { func : :f = :body_assert_vvm : :body_lop_mr; };

    stamp :mul_acc_mmm = { func : :f; };
    stamp :mul_acc_mtm = { func : :f; };
    stamp :mul_acc_tmm = { func : :f; };
    stamp :mul_acc_ttm = { func : :f; };

    /// other -----------------------------------------------------------------

    stamp :cat = { func : :f = { bhvm_holor_s_cat( a, b, r ); }; };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// extended binary operators (elementary cyclic indexing)
group :ar2_eci =
{
    extending stump verbatim :_ = aware : {};

    signature f2_t f2( plain, f2_t a, f2_t b );
    signature f3_t f3( plain, f3_t a, f3_t b );
    signature void f( plain, const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );

    // vector += vector <op> vector
    body body_zro = { bhvm_value_s_zro( &r->v ); };

    body body_acc =
    {
        assert( a != r && b != r );
        assert(  sz_min( sz_min( a->v.size, b->v.size ), r->v.size ) > 0 );
        sz_t n = sz_gcd( sz_gcd( a->v.size, b->v.size ), r->v.size );
        sz_t m = sz_max( sz_max( a->v.size, b->v.size ), r->v.size ) / n;

        #define :ACC_CASE( TA_T, TB_T, TR_T, FUNC ) \
        { \
            const TA_T *a0 = a->v.data, *a1 = a0; \
            const TB_T *b0 = b->v.data, *b1 = b0; \
                  TR_T *r0 = r->v.data, *r1 = r0; \
            for( sz_t i = 0; i < m; i++ ) \
            { \
                for( sz_t i = 0; i < n; i++ ) r1[ i ] += FUNC( a1[ i ], b1[ i ] ); \
                a1 = ( a1 + n - a0 ) < a->v.size ? ( a1 + n ) : a0; \
                b1 = ( b1 + n - b0 ) < b->v.size ? ( b1 + n ) : b0; \
                r1 = ( r1 + n - r0 ) < r->v.size ? ( r1 + n ) : r0; \
            } \
        }

        switch( BKNIT_FA3( a->v.type, b->v.type, r->v.type ) )
        {
            case BKNIT_F222: :ACC_CASE( f2_t, f2_t, f2_t, @_f2 ); break;
            case BKNIT_F223: :ACC_CASE( f2_t, f2_t, f3_t, @_f3 ); break;
            case BKNIT_F232: :ACC_CASE( f2_t, f3_t, f2_t, @_f3 ); break;
            case BKNIT_F233: :ACC_CASE( f2_t, f3_t, f3_t, @_f3 ); break;
            case BKNIT_F322: :ACC_CASE( f3_t, f2_t, f2_t, @_f3 ); break;
            case BKNIT_F323: :ACC_CASE( f3_t, f2_t, f3_t, @_f3 ); break;
            case BKNIT_F332: :ACC_CASE( f3_t, f3_t, f2_t, @_f3 ); break;
            case BKNIT_F333: :ACC_CASE( f3_t, f3_t, f3_t, @_f3 ); break;
            default: BKNIT_FA3_ERR( a->v.type, b->v.type, r->v.type ); break;
        }

        #undef :ACC_CASE
    };

    body body_std =
    {
        if( a == r || b == r )
        {
            bhvm_holor_s* buf = bhvm_holor_s_create();
            :$R_acc_s_f( a, b, bhvm_holor_s_fit_size( bhvm_holor_s_copy_shape_type( buf, r ) ) );
            if( r->v.size ) bhvm_value_s_cpy( &buf->v, &r->v );
            bhvm_holor_s_discard( buf );
        }
        else
        {
            bhvm_value_s_zro( &r->v );
            :$R_acc_s_f( a, b, r );
        }
    };

    /// axon pass --------------------------------------------------------------

    stamp :add_acc = { func : :f2 = { return a + b; };           func : :f3 = { return a + b; };           func : :f = :body_acc; };
    stamp :sub_acc = { func : :f2 = { return a - b; };           func : :f3 = { return a - b; };           func : :f = :body_acc; };
    stamp :mul_acc = { func : :f2 = { return a * b; };           func : :f3 = { return a * b; };           func : :f = :body_acc; };
    stamp :div_acc = { func : :f2 = { return a * f2_inv( b ); }; func : :f3 = { return a * f3_inv( b ); }; func : :f = :body_acc; };
    stamp :pow_acc = { func : :f2 = { return  f2_pow( a, b ); }; func : :f3 = { return  f3_pow( a, b ); }; func : :f = :body_acc; };
    stamp :add     = { func : :f = :body_std; };
    stamp :sub     = { func : :f = :body_std; };
    stamp :mul     = { func : :f = :body_std; };
    stamp :div     = { func : :f = :body_std; };
    stamp :pow     = { func : :f = :body_std; };

    /// logic ------------------------------------------------------------------

    /// dendrite pass ----------------------------------------------------------

};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// ternary operators
group :ar3 =
{
    extending stump verbatim :_ = aware : {};
    signature void f( plain, const bhvm_holor_s* a, const bhvm_holor_s* b, const bhvm_holor_s* c, bhvm_holor_s* r );

    body body_lop_vvvv =
    {
        assert( a->v.size == r->v.size );
        assert( b->v.size == r->v.size );
        assert( c->v.size == r->v.size );
        bhvm_lop_ar3_$R_s_f( BKNIT_FA4( a->v.type, b->v.type, c->v.type, r->v.type ), a->v.data, b->v.data, c->v.data, r->v.data, r->v.size );
    };

    /// axon pass --------------------------------------------------------------

    stamp :branch_vvvv = { func : :f = :body_lop_vvvv; };
    stamp :branch_svvv = { func : :f; };

    /// dendrite pass ----------------------------------------------------------

};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// extended ternary operators (elementary cyclic indexing)
group :ar3_eci =
{
    extending stump verbatim :_ = aware : {};

    signature f2_t f2( plain, f2_t a, f2_t b, f2_t c );
    signature f3_t f3( plain, f3_t a, f3_t b, f3_t c );
    signature void f( plain, const bhvm_holor_s* a, const bhvm_holor_s* b, const bhvm_holor_s* c, bhvm_holor_s* r );

    // vector += vector <op> vector
    body body_zro = { bhvm_value_s_zro( &r->v ); };

    body body_acc =
    {
        assert( a != r && b != r && c != r );
        assert(  sz_min( a->v.size, sz_min( b->v.size, sz_min( c->v.size, r->v.size ) ) ) > 0 );
        sz_t n = sz_gcd( a->v.size, sz_gcd( b->v.size, sz_gcd( c->v.size, r->v.size ) ) );
        sz_t m = sz_max( a->v.size, sz_max( b->v.size, sz_max( c->v.size, r->v.size ) ) ) / n;

        #define :ACC_CASE( TA_T, TB_T, TC_T, TR_T, FUNC ) \
        { \
            const TA_T *a0 = a->v.data, *a1 = a0; \
            const TB_T *b0 = b->v.data, *b1 = b0; \
            const TC_T *c0 = c->v.data, *c1 = c0; \
                  TR_T *r0 = r->v.data, *r1 = r0; \
            for( sz_t i = 0; i < m; i++ ) \
            { \
                for( sz_t i = 0; i < n; i++ ) r1[ i ] += FUNC( a1[ i ], b1[ i ], c1[ i ] ); \
                a1 = ( a1 + n - a0 ) < a->v.size ? ( a1 + n ) : a0; \
                b1 = ( b1 + n - b0 ) < b->v.size ? ( b1 + n ) : b0; \
                c1 = ( c1 + n - c0 ) < c->v.size ? ( c1 + n ) : c0; \
                r1 = ( r1 + n - r0 ) < r->v.size ? ( r1 + n ) : r0; \
            } \
        }

        switch( BKNIT_FA4( a->v.type, b->v.type, c->v.type, r->v.type ) )
        {
            case BKNIT_F2222: :ACC_CASE( f2_t, f2_t, f2_t, f2_t, @_f2 ); break;
            case BKNIT_F2223: :ACC_CASE( f2_t, f2_t, f2_t, f3_t, @_f3 ); break;
            case BKNIT_F2232: :ACC_CASE( f2_t, f2_t, f3_t, f2_t, @_f3 ); break;
            case BKNIT_F2233: :ACC_CASE( f2_t, f2_t, f3_t, f3_t, @_f3 ); break;
            case BKNIT_F2322: :ACC_CASE( f2_t, f3_t, f2_t, f2_t, @_f3 ); break;
            case BKNIT_F2323: :ACC_CASE( f2_t, f3_t, f2_t, f3_t, @_f3 ); break;
            case BKNIT_F2332: :ACC_CASE( f2_t, f3_t, f3_t, f2_t, @_f3 ); break;
            case BKNIT_F2333: :ACC_CASE( f2_t, f3_t, f3_t, f3_t, @_f3 ); break;
            case BKNIT_F3222: :ACC_CASE( f3_t, f2_t, f2_t, f2_t, @_f3 ); break;
            case BKNIT_F3223: :ACC_CASE( f3_t, f2_t, f2_t, f3_t, @_f3 ); break;
            case BKNIT_F3232: :ACC_CASE( f3_t, f2_t, f3_t, f2_t, @_f3 ); break;
            case BKNIT_F3233: :ACC_CASE( f3_t, f2_t, f3_t, f3_t, @_f3 ); break;
            case BKNIT_F3322: :ACC_CASE( f3_t, f3_t, f2_t, f2_t, @_f3 ); break;
            case BKNIT_F3323: :ACC_CASE( f3_t, f3_t, f2_t, f3_t, @_f3 ); break;
            case BKNIT_F3332: :ACC_CASE( f3_t, f3_t, f3_t, f2_t, @_f3 ); break;
            case BKNIT_F3333: :ACC_CASE( f3_t, f3_t, f3_t, f3_t, @_f3 ); break;
            default: BKNIT_FA4_ERR( a->v.type, b->v.type, c->v.type, r->v.type ); break;
        }

        #undef :ACC_CASE
    };

    /// axon pass --------------------------------------------------------------

    /// logic ------------------------------------------------------------------

    /// dendrite pass ----------------------------------------------------------

    body body_div_dp_zabg_f2 = { return -a * b * f2_inv( c * c ); };
    body body_div_dp_zabg_f3 = { return -a * b * f3_inv( c * c ); };
    stamp :div_dp_zabg = { func : :f2 = :body_$R_f2; func : :f3 = :body_$R_f3; func : :f = :body_acc; };

    body body_pow_dp_abzf_f2 = { return c * b * f2_pow( a, b - 1 ); };
    body body_pow_dp_abzf_f3 = { return c * b * f3_pow( a, b - 1 ); };
    stamp :pow_dp_abzf = { func : :f2 = :body_$R_f2; func : :f3 = :body_$R_f3; func : :f = :body_acc; };

    body body_pow_dp_ayzg_f2 = { return ( a > 0 ) ? b * c * logf( a ) : 0; };
    body body_pow_dp_ayzg_f3 = { return ( a > 0 ) ? b * c * log ( a ) : 0; };
    stamp :pow_dp_ayzg = { func : :f2 = :body_$R_f2; func : :f3 = :body_$R_f3; func : :f = :body_acc; };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // PLANT_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // TYPEOF_bhvm_hop

/**********************************************************************************************************************/

vd_t bhvm_hop_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BHVM_HOP_H
