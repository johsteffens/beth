/** Author and Copyright 2020 Johannes Bernhard Steffens
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

/** Collection of Holor-Operators
 *
 */

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

/// nullary operators
group :ar0 =
{
    extending stump verbatim :_s = aware : {};
    signature void f( m bhvm_holor_s* r );

    /// axon pass --------------------------------------------------------------

    stamp :zro_s = { func : .f = { bhvm_lop_ar0_zro_s_f( BKNIT_FA1( r->v.type ), r->v.data, r->v.size ); }; };
    stamp :one_s = { func : .f = { bhvm_lop_ar0_one_s_f( BKNIT_FA1( r->v.type ), r->v.data, r->v.size ); }; };

    /// dendrite pass ----------------------------------------------------------

    stamp :nul_dp_s = { func : .f = { /* no action */ }; };
};

// ---------------------------------------------------------------------------------------------------------------------

/// unary operators
group :ar1 =
{
    extending stump verbatim :_s = aware : {};
    signature void f( c bhvm_holor_s* a, m bhvm_holor_s* r );

    /// axon pass --------------------------------------------------------------

    stamp :l1_s     = { func : .f = { assert( r.v.size == 1 ); bhvm_lop_ar1_l1_s_f    ( BKNIT_FA2( a.v.type, r.v.type ), a.v.data, r.v.data, a.v.size ); }; };
    stamp :sqrsum_s = { func : .f = { assert( r.v.size == 1 ); bhvm_lop_ar1_sqrsum_s_f( BKNIT_FA2( a.v.type, r.v.type ), a.v.data, r.v.data, a.v.size ); }; };

    stamp :cpy_s   = { func : .f = { assert( a.v.size == r.v.size ); bhvm_lop_ar1_cpy_s_f  ( BKNIT_FA2( a.v.type, r.v.type ), a.v.data, r.v.data, r.v.size ); }; };
    stamp :neg_s   = { func : .f = { assert( a.v.size == r.v.size ); bhvm_lop_ar1_neg_s_f  ( BKNIT_FA2( a.v.type, r.v.type ), a.v.data, r.v.data, r.v.size ); }; };
    stamp :floor_s = { func : .f = { assert( a.v.size == r.v.size ); bhvm_lop_ar1_floor_s_f( BKNIT_FA2( a.v.type, r.v.type ), a.v.data, r.v.data, r.v.size ); }; };
    stamp :ceil_s  = { func : .f = { assert( a.v.size == r.v.size ); bhvm_lop_ar1_ceil_s_f ( BKNIT_FA2( a.v.type, r.v.type ), a.v.data, r.v.data, r.v.size ); }; };
    stamp :exp_s   = { func : .f = { assert( a.v.size == r.v.size ); bhvm_lop_ar1_exp_s_f  ( BKNIT_FA2( a.v.type, r.v.type ), a.v.data, r.v.data, r.v.size ); }; };
    stamp :log_s   = { func : .f = { assert( a.v.size == r.v.size ); bhvm_lop_ar1_log_s_f  ( BKNIT_FA2( a.v.type, r.v.type ), a.v.data, r.v.data, r.v.size ); }; };
    stamp :inv_s   = { func : .f = { assert( a.v.size == r.v.size ); bhvm_lop_ar1_inv_s_f  ( BKNIT_FA2( a.v.type, r.v.type ), a.v.data, r.v.data, r.v.size ); }; };
    stamp :abs_s   = { func : .f = { assert( a.v.size == r.v.size ); bhvm_lop_ar1_abs_s_f  ( BKNIT_FA2( a.v.type, r.v.type ), a.v.data, r.v.data, r.v.size ); }; };
    stamp :sig_s   = { func : .f = { assert( a.v.size == r.v.size ); bhvm_lop_ar1_sig_s_f  ( BKNIT_FA2( a.v.type, r.v.type ), a.v.data, r.v.data, r.v.size ); }; };
    stamp :sqr_s   = { func : .f = { assert( a.v.size == r.v.size ); bhvm_lop_ar1_sqr_s_f  ( BKNIT_FA2( a.v.type, r.v.type ), a.v.data, r.v.data, r.v.size ); }; };
    stamp :srt_s   = { func : .f = { assert( a.v.size == r.v.size ); bhvm_lop_ar1_srt_s_f  ( BKNIT_FA2( a.v.type, r.v.type ), a.v.data, r.v.data, r.v.size ); }; };

    stamp :sigm_s       = { func : .f = { assert( a.v.size == r.v.size ); bhvm_lop_ar1_sigm_s_f      ( BKNIT_FA2( a.v.type, r.v.type ), a.v.data, r.v.data, r.v.size ); }; };
    stamp :sigm_hard_s  = { func : .f = { assert( a.v.size == r.v.size ); bhvm_lop_ar1_sigm_hard_s_f ( BKNIT_FA2( a.v.type, r.v.type ), a.v.data, r.v.data, r.v.size ); }; };
    stamp :sigm_leaky_s = { func : .f = { assert( a.v.size == r.v.size ); bhvm_lop_ar1_sigm_leaky_s_f( BKNIT_FA2( a.v.type, r.v.type ), a.v.data, r.v.data, r.v.size ); }; };

    stamp :tanh_s       = { func : .f = { assert( a.v.size == r.v.size ); bhvm_lop_ar1_tanh_s_f      ( BKNIT_FA2( a.v.type, r.v.type ), a.v.data, r.v.data, r.v.size ); }; };
    stamp :tanh_hard_s  = { func : .f = { assert( a.v.size == r.v.size ); bhvm_lop_ar1_tanh_hard_s_f ( BKNIT_FA2( a.v.type, r.v.type ), a.v.data, r.v.data, r.v.size ); }; };
    stamp :tanh_leaky_s = { func : .f = { assert( a.v.size == r.v.size ); bhvm_lop_ar1_tanh_leaky_s_f( BKNIT_FA2( a.v.type, r.v.type ), a.v.data, r.v.data, r.v.size ); }; };

    stamp :softplus_s   = { func : .f = { assert( a.v.size == r.v.size ); bhvm_lop_ar1_softplus_s_f  ( BKNIT_FA2( a.v.type, r.v.type ), a.v.data, r.v.data, r.v.size ); }; };
    stamp :relu_s       = { func : .f = { assert( a.v.size == r.v.size ); bhvm_lop_ar1_relu_s_f      ( BKNIT_FA2( a.v.type, r.v.type ), a.v.data, r.v.data, r.v.size ); }; };
    stamp :relu_leaky_s = { func : .f = { assert( a.v.size == r.v.size ); bhvm_lop_ar1_relu_leaky_s_f( BKNIT_FA2( a.v.type, r.v.type ), a.v.data, r.v.data, r.v.size ); }; };

    stamp :softmax_s    = { func : .f; };

    /// dendrite pass ----------------------------------------------------------

    stamp :neg_dp_zf_s  = { func : .f = { assert( a.v.size == r.v.size ); bhvm_lop_ar1_accn_s_f( BKNIT_FA2( a.v.type, r.v.type ), a.v.data, r.v.data, r.v.size ); }; };

    stamp :cat_dp_zf_s  = { func : .f = { a.v.acc_offs( 0, r.v.1, 0, r.v.size ); }; };
    stamp :cat_dp_zg_s  = { func : .f = { a.v.acc_offs( a.v.size - r.v.size, r.v.1, 0, r.v.size ); }; };
    stamp :ccat_dp_zf_s = { func : .f = { a.v.acc_offs( 0, r.v.1, 0, r.v.size ); }; };
    stamp :ccat_dp_zg_s = { func : .f = { a.v.acc_offs( a.v.size - r.v.size, r.v.1, 0, r.v.size ); }; };
};

// ---------------------------------------------------------------------------------------------------------------------

/// extended unary operators (elementary cyclic indexing)
group :ar1_eci =
{
    extending stump verbatim :_s = aware : {};

    signature f2_t f2( f2_t a );
    signature f3_t f3( f3_t a );
    signature void f( c bhvm_holor_s* a, m bhvm_holor_s* r );

    body body_acc =
    {
        assert( a != r );
        assert(  sz_min( a.v.size, r.v.size ) > 0 );
        sz_t n = sz_gcd( a.v.size, r.v.size );
        sz_t m = sz_max( a.v.size, r.v.size ) / n;


        #define BHVM_ACC_CASE( TA_T, TR_T, FUNC ) \
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

        switch( BKNIT_FA2( a.v.type, r.v.type ) )
        {
            case BKNIT_F22: BHVM_ACC_CASE( f2_t, f2_t, @_f2 ); break;
            case BKNIT_F23: BHVM_ACC_CASE( f2_t, f3_t, @_f3 ); break;
            case BKNIT_F32: BHVM_ACC_CASE( f3_t, f2_t, @_f3 ); break;
            case BKNIT_F33: BHVM_ACC_CASE( f3_t, f3_t, @_f3 ); break;
            default: BKNIT_FA2_ERR( a.v.type, r.v.type ); break;
        }

        #undef BHVM_ACC_CASE
    };

    stamp :cpy_acc_s = { func : .f2 = { return  a; };          func : .f3 = { return  a; };          func : .f = :body_acc; };
    stamp :neg_acc_s = { func : .f2 = { return -a; };          func : .f3 = { return -a; };          func : .f = :body_acc; };
    stamp :inv_acc_s = { func : .f2 = { return f2_inv( a ); }; func : .f3 = { return f3_inv( a ); }; func : .f = :body_acc; };

    stamp :cpy_s =
    {
        func : .f2 = { return  a; };
        func : .f3 = { return  a; };
        func : .f =
        {
            if( a == r )
            {
                d bhvm_holor_s* buf = bhvm_holor_s!;
                :cpy_acc_s_f( a, bhvm_holor_s_fit_size( buf.copy_shape_type( r ) ) );
                if( r.v.size ) buf.v.cpy( r.v.1 );
                buf.discard();
            }
            else
            {
                r.v.zro();
                :cpy_acc_s_f( a, r );
            }
        };
    };

    stamp :neg_s =
    {
        func : .f2 = { return -a; };
        func : .f3 = { return -a; };
        func : .f =
        {
            if( a == r )
            {
                d bhvm_holor_s* buf = bhvm_holor_s!;
                :neg_acc_s_f( a, bhvm_holor_s_fit_size( buf.copy_shape_type( r ) ) );
                if( r.v.size ) buf.v.cpy( r.v.1 );
                buf.discard();
            }
            else
            {
                r.v.zro();
                :neg_acc_s_f( a, r );
            }
        };
    };

    stamp :inv_s =
    {
        func : .f2 = { return f2_inv( a ); };
        func : .f3 = { return f3_inv( a ); };
        func : .f =
        {
            if( a == r )
            {
                d bhvm_holor_s* buf = bhvm_holor_s!;
                :inv_acc_s_f( a, bhvm_holor_s_fit_size( buf.copy_shape_type( r ) ) );
                if( r.v.size ) buf.v.cpy( r.v.1 );
                buf.discard();
            }
            else
            {
                r.v.zro();
                :inv_acc_s_f( a, r );
            }
        };
    };
};

// ---------------------------------------------------------------------------------------------------------------------

/// binary operators
group :ar2 =
{
    extending stump verbatim :_s = aware : {};
    signature void f( c bhvm_holor_s* a, c bhvm_holor_s* b, m bhvm_holor_s* r );

    /// axon pass --------------------------------------------------------------

    stamp :sub_sqrsum_s = { func : .f = { BODY_ASSERT_VVS; bhvm_lop_ar2_sub_sqrsum_s_f( BKNIT_FA3( a.v.type, b.v.type, r.v.type ), a.v.data, b.v.data, r.v.data, a.v.size ); }; }; // r = ( a - b )^2
    stamp :sub_l1_s     = { func : .f = { BODY_ASSERT_VVS; bhvm_lop_ar2_sub_l1_s_f    ( BKNIT_FA3( a.v.type, b.v.type, r.v.type ), a.v.data, b.v.data, r.v.data, a.v.size ); }; }; // r = l1-norm of ( a - b )

    /// logic ------------------------------------------------------------------

    stamp :equal_s         = { func : .f = { BODY_ASSERT_VVV; bhvm_lop_ar2_equal_s_f        ( BKNIT_FA3( a.v.type, b.v.type, r.v.type ), a.v.data, b.v.data, r.v.data, r.v.size ); }; };
    stamp :unequal_s       = { func : .f = { BODY_ASSERT_VVV; bhvm_lop_ar2_unequal_s_f      ( BKNIT_FA3( a.v.type, b.v.type, r.v.type ), a.v.data, b.v.data, r.v.data, r.v.size ); }; };
    stamp :larger_s        = { func : .f = { BODY_ASSERT_VVV; bhvm_lop_ar2_larger_s_f       ( BKNIT_FA3( a.v.type, b.v.type, r.v.type ), a.v.data, b.v.data, r.v.data, r.v.size ); }; };
    stamp :smaller_s       = { func : .f = { BODY_ASSERT_VVV; bhvm_lop_ar2_smaller_s_f      ( BKNIT_FA3( a.v.type, b.v.type, r.v.type ), a.v.data, b.v.data, r.v.data, r.v.size ); }; };
    stamp :larger_equal_s  = { func : .f = { BODY_ASSERT_VVV; bhvm_lop_ar2_larger_equal_s_f ( BKNIT_FA3( a.v.type, b.v.type, r.v.type ), a.v.data, b.v.data, r.v.data, r.v.size ); }; };
    stamp :smaller_equal_s = { func : .f = { BODY_ASSERT_VVV; bhvm_lop_ar2_smaller_equal_s_f( BKNIT_FA3( a.v.type, b.v.type, r.v.type ), a.v.data, b.v.data, r.v.data, r.v.size ); }; };
    stamp :logic_and_s     = { func : .f = { BODY_ASSERT_VVV; bhvm_lop_ar2_logic_and_s_f    ( BKNIT_FA3( a.v.type, b.v.type, r.v.type ), a.v.data, b.v.data, r.v.data, r.v.size ); }; };
    stamp :logic_or_s      = { func : .f = { BODY_ASSERT_VVV; bhvm_lop_ar2_logic_or_s_f     ( BKNIT_FA3( a.v.type, b.v.type, r.v.type ), a.v.data, b.v.data, r.v.data, r.v.size ); }; };

    /// dendrite pass ----------------------------------------------------------

    stamp :abs_dp_zaf_s        = { func : .f = { BODY_ASSERT_VVV; bhvm_lop_ar2_abs_dp_zaf_s_f ( BKNIT_FA3( a.v.type, b.v.type, r.v.type ), a.v.data, b.v.data, r.v.data, r.v.size ); }; };
    stamp :exp_dp_zyf_s        = { func : .f = { BODY_ASSERT_VVV; bhvm_lop_ar2_exp_dp_zyf_s_f ( BKNIT_FA3( a.v.type, b.v.type, r.v.type ), a.v.data, b.v.data, r.v.data, r.v.size ); }; };
    stamp :log_dp_zaf_s        = { func : .f = { BODY_ASSERT_VVV; bhvm_lop_ar2_log_dp_zaf_s_f ( BKNIT_FA3( a.v.type, b.v.type, r.v.type ), a.v.data, b.v.data, r.v.data, r.v.size ); }; };
    stamp :inv_dp_zyf_s        = { func : .f = { BODY_ASSERT_VVV; bhvm_lop_ar2_inv_dp_zyf_s_f ( BKNIT_FA3( a.v.type, b.v.type, r.v.type ), a.v.data, b.v.data, r.v.data, r.v.size ); }; };
    stamp :sqr_dp_zaf_s        = { func : .f = { BODY_ASSERT_VVV; bhvm_lop_ar2_sqr_dp_zaf_s_f ( BKNIT_FA3( a.v.type, b.v.type, r.v.type ), a.v.data, b.v.data, r.v.data, r.v.size ); }; };
    stamp :srt_dp_zyf_s        = { func : .f = { BODY_ASSERT_VVV; bhvm_lop_ar2_srt_dp_zyf_s_f ( BKNIT_FA3( a.v.type, b.v.type, r.v.type ), a.v.data, b.v.data, r.v.data, r.v.size ); }; };
    stamp :sigm_dp_zyf_s       = { func : .f = { BODY_ASSERT_VVV; bhvm_lop_ar2_sigm_dp_zyf_s_f( BKNIT_FA3( a.v.type, b.v.type, r.v.type ), a.v.data, b.v.data, r.v.data, r.v.size ); }; };
    stamp :sigm_hard_dp_zyf_s  = { func : .f = { BODY_ASSERT_VVV; bhvm_lop_ar2_sigm_hard_dp_zyf_s_f ( BKNIT_FA3( a.v.type, b.v.type, r.v.type ), a.v.data, b.v.data, r.v.data, r.v.size ); }; };
    stamp :sigm_leaky_dp_zyf_s = { func : .f = { BODY_ASSERT_VVV; bhvm_lop_ar2_sigm_leaky_dp_zyf_s_f( BKNIT_FA3( a.v.type, b.v.type, r.v.type ), a.v.data, b.v.data, r.v.data, r.v.size ); }; };
    stamp :tanh_dp_zyf_s       = { func : .f = { BODY_ASSERT_VVV; bhvm_lop_ar2_tanh_dp_zyf_s_f      ( BKNIT_FA3( a.v.type, b.v.type, r.v.type ), a.v.data, b.v.data, r.v.data, r.v.size ); }; };
    stamp :tanh_hard_dp_zyf_s  = { func : .f = { BODY_ASSERT_VVV; bhvm_lop_ar2_tanh_hard_dp_zyf_s_f ( BKNIT_FA3( a.v.type, b.v.type, r.v.type ), a.v.data, b.v.data, r.v.data, r.v.size ); }; };
    stamp :tanh_leaky_dp_zyf_s = { func : .f = { BODY_ASSERT_VVV; bhvm_lop_ar2_tanh_leaky_dp_zyf_s_f( BKNIT_FA3( a.v.type, b.v.type, r.v.type ), a.v.data, b.v.data, r.v.data, r.v.size ); }; };
    stamp :softplus_dp_zyf_s   = { func : .f = { BODY_ASSERT_VVV; bhvm_lop_ar2_softplus_dp_zyf_s_f  ( BKNIT_FA3( a.v.type, b.v.type, r.v.type ), a.v.data, b.v.data, r.v.data, r.v.size ); }; };
    stamp :relu_dp_zyf_s       = { func : .f = { BODY_ASSERT_VVV; bhvm_lop_ar2_relu_dp_zyf_s_f      ( BKNIT_FA3( a.v.type, b.v.type, r.v.type ), a.v.data, b.v.data, r.v.data, r.v.size ); }; };
    stamp :relu_leaky_dp_zyf_s = { func : .f = { BODY_ASSERT_VVV; bhvm_lop_ar2_relu_leaky_dp_zyf_s_f( BKNIT_FA3( a.v.type, b.v.type, r.v.type ), a.v.data, b.v.data, r.v.data, r.v.size ); }; };
    stamp :softmax_dp_zyf_s    = { func : .f; };

    /// mul axon pass ----------------------------------------------------------

    stamp :mul_mvv_s = { func : .f = { BODY_ASSERT_MVV; bhvm_lop_ar2_mul_mvv_s_f_m( BKNIT_FA3( a.v.type, b.v.type, r.v.type ), a.v.data, b.v.data, r.v.data, a.s.[ 1 ], a.s.[ 0 ] ); }; };
    stamp :mul_vmv_s = { func : .f = { BODY_ASSERT_VMV; bhvm_lop_ar2_mul_vmv_s_f_m( BKNIT_FA3( a.v.type, b.v.type, r.v.type ), a.v.data, b.v.data, r.v.data, b.s.[ 1 ], b.s.[ 0 ] ); }; };
    stamp :mul_vvm_s = { func : .f = { BODY_ASSERT_VVM; bhvm_lop_ar2_mul_vvm_s_f_m( BKNIT_FA3( a.v.type, b.v.type, r.v.type ), a.v.data, b.v.data, r.v.data, r.s.[ 1 ], r.s.[ 0 ] ); }; };
    stamp :mul_tvv_s = { func : .f = { BODY_ASSERT_TVV; bhvm_lop_ar2_mul_vmv_s_f_m( BKNIT_FA3( b.v.type, a.v.type, r.v.type ), b.v.data, a.v.data, r.v.data, a.s.[ 1 ], a.s.[ 0 ] ); }; };
    stamp :mul_vtv_s = { func : .f = { BODY_ASSERT_VTV; bhvm_lop_ar2_mul_mvv_s_f_m( BKNIT_FA3( b.v.type, a.v.type, r.v.type ), b.v.data, a.v.data, r.v.data, b.s.[ 1 ], b.s.[ 0 ] ); }; };

    stamp :mul_mmm_s = { func : .f; };
    stamp :mul_mtm_s = { func : .f; };
    stamp :mul_tmm_s = { func : .f; };
    stamp :mul_ttm_s = { func : .f; };

    /// mul accumulate ---------------------------------------------------------

    stamp :mul_acc_mvv_s = { func : .f = { BODY_ASSERT_MVV; bhvm_lop_ar2_mul_acc_mvv_s_f_m( BKNIT_FA3( a.v.type, b.v.type, r.v.type ), a.v.data, b.v.data, r.v.data, a.s.[ 1 ], a.s.[ 0 ] ); }; };
    stamp :mul_acc_vmv_s = { func : .f = { BODY_ASSERT_VMV; bhvm_lop_ar2_mul_acc_vmv_s_f_m( BKNIT_FA3( a.v.type, b.v.type, r.v.type ), a.v.data, b.v.data, r.v.data, b.s.[ 1 ], b.s.[ 0 ] ); }; };
    stamp :mul_acc_tvv_s = { func : .f = { BODY_ASSERT_TVV; bhvm_lop_ar2_mul_acc_vmv_s_f_m( BKNIT_FA3( b.v.type, a.v.type, r.v.type ), b.v.data, a.v.data, r.v.data, a.s.[ 1 ], a.s.[ 0 ] ); }; };
    stamp :mul_acc_vtv_s = { func : .f = { BODY_ASSERT_VTV; bhvm_lop_ar2_mul_acc_mvv_s_f_m( BKNIT_FA3( b.v.type, a.v.type, r.v.type ), b.v.data, a.v.data, r.v.data, b.s.[ 1 ], b.s.[ 0 ] ); }; };
    stamp :mul_acc_vvm_s = { func : .f = { BODY_ASSERT_VVM; bhvm_lop_ar2_mul_acc_vvm_s_f_m( BKNIT_FA3( a.v.type, b.v.type, r.v.type ), a.v.data, b.v.data, r.v.data, r.s.[ 1 ], r.s.[ 0 ] ); }; };

    stamp :mul_acc_mmm_s = { func : .f; };
    stamp :mul_acc_mtm_s = { func : .f; };
    stamp :mul_acc_tmm_s = { func : .f; };
    stamp :mul_acc_ttm_s = { func : .f; };

    /// other -----------------------------------------------------------------

    stamp :cat_s  = { func : .f = { a.cat ( b, r ); }; };
    stamp :ccat_s = { func : .f = { a.ccat( b, r ); }; };
};

// ---------------------------------------------------------------------------------------------------------------------

/// extended binary operators (elementary cyclic indexing)
group :ar2_eci =
{
    extending stump verbatim :_s = aware : {};

    signature f2_t f2( f2_t a, f2_t b );
    signature f3_t f3( f3_t a, f3_t b );
    signature void f( c bhvm_holor_s* a, c bhvm_holor_s* b, m bhvm_holor_s* r );

    /// sets the target value: r = op( a, b )
    body body_set =
    {
        assert( a != r && b != r );
        assert(  sz_min( sz_min( a.v.size, b.v.size ), r.v.size ) > 0 );
        sz_t n = sz_gcd( sz_gcd( a.v.size, b.v.size ), r.v.size );
        sz_t m = sz_max( sz_max( a.v.size, b.v.size ), r.v.size ) / n;

        #define BHVM_SET_CASE( TA_T, TB_T, TR_T, FUNC ) \
        { \
            const TA_T *a0 = a->v.data, *a1 = a0; \
            const TB_T *b0 = b->v.data, *b1 = b0; \
                  TR_T *r0 = r->v.data, *r1 = r0; \
            for( sz_t i = 0; i < m; i++ ) \
            { \
                for( sz_t i = 0; i < n; i++ ) r1[ i ] = FUNC( a1[ i ], b1[ i ] ); \
                a1 = ( a1 + n - a0 ) < a->v.size ? ( a1 + n ) : a0; \
                b1 = ( b1 + n - b0 ) < b->v.size ? ( b1 + n ) : b0; \
                r1 = ( r1 + n - r0 ) < r->v.size ? ( r1 + n ) : r0; \
            } \
        }

        switch( BKNIT_FA3( a.v.type, b.v.type, r.v.type ) )
        {
            case BKNIT_F222: BHVM_SET_CASE( f2_t, f2_t, f2_t, @_f2 ); break;
            case BKNIT_F223: BHVM_SET_CASE( f2_t, f2_t, f3_t, @_f3 ); break;
            case BKNIT_F232: BHVM_SET_CASE( f2_t, f3_t, f2_t, @_f3 ); break;
            case BKNIT_F233: BHVM_SET_CASE( f2_t, f3_t, f3_t, @_f3 ); break;
            case BKNIT_F322: BHVM_SET_CASE( f3_t, f2_t, f2_t, @_f3 ); break;
            case BKNIT_F323: BHVM_SET_CASE( f3_t, f2_t, f3_t, @_f3 ); break;
            case BKNIT_F332: BHVM_SET_CASE( f3_t, f3_t, f2_t, @_f3 ); break;
            case BKNIT_F333: BHVM_SET_CASE( f3_t, f3_t, f3_t, @_f3 ); break;
            default: BKNIT_FA3_ERR( a.v.type, b.v.type, r.v.type ); break;
        }

        #undef BHVM_SET_CASE
    };

    /// accumulates the target value: r += op( a, b )
    body body_acc =
    {
        assert( a != r && b != r );
        assert(  sz_min( sz_min( a.v.size, b.v.size ), r.v.size ) > 0 );
        sz_t n = sz_gcd( sz_gcd( a.v.size, b.v.size ), r.v.size );
        sz_t m = sz_max( sz_max( a.v.size, b.v.size ), r.v.size ) / n;

        #define BHVM_ACC_CASE( TA_T, TB_T, TR_T, FUNC ) \
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

        switch( BKNIT_FA3( a.v.type, b.v.type, r.v.type ) )
        {
            case BKNIT_F222: BHVM_ACC_CASE( f2_t, f2_t, f2_t, @_f2 ); break;
            case BKNIT_F223: BHVM_ACC_CASE( f2_t, f2_t, f3_t, @_f3 ); break;
            case BKNIT_F232: BHVM_ACC_CASE( f2_t, f3_t, f2_t, @_f3 ); break;
            case BKNIT_F233: BHVM_ACC_CASE( f2_t, f3_t, f3_t, @_f3 ); break;
            case BKNIT_F322: BHVM_ACC_CASE( f3_t, f2_t, f2_t, @_f3 ); break;
            case BKNIT_F323: BHVM_ACC_CASE( f3_t, f2_t, f3_t, @_f3 ); break;
            case BKNIT_F332: BHVM_ACC_CASE( f3_t, f3_t, f2_t, @_f3 ); break;
            case BKNIT_F333: BHVM_ACC_CASE( f3_t, f3_t, f3_t, @_f3 ); break;
            default: BKNIT_FA3_ERR( a.v.type, b.v.type, r.v.type ); break;
        }

        #undef BHVM_ACC_CASE
    };

    /// axon pass --------------------------------------------------------------

    stamp :add_acc_s = { func : .f2 = { return a + b; };           func : .f3 = { return a + b; };           func : .f = :body_acc; };
    stamp :sub_acc_s = { func : .f2 = { return a - b; };           func : .f3 = { return a - b; };           func : .f = :body_acc; };
    stamp :mul_acc_s = { func : .f2 = { return a * b; };           func : .f3 = { return a * b; };           func : .f = :body_acc; };
    stamp :div_acc_s = { func : .f2 = { return a * f2_inv( b ); }; func : .f3 = { return a * f3_inv( b ); }; func : .f = :body_acc; };
    stamp :pow_acc_s = { func : .f2 = { return  f2_pow( a, b ); }; func : .f3 = { return  f3_pow( a, b ); }; func : .f = :body_acc; };

    stamp :add_s =
    {
        func : .f =
        {
            if( a == r || b == r )
            {
                d bhvm_holor_s* buf = bhvm_holor_s!;
                :add_acc_s_f( a, b, bhvm_holor_s_fit_size( buf.copy_shape_type( r ) ) );
                if( r->v.size ) buf.v.cpy( r.v.1 );
                buf.discard();
            }
            else
            {
                r.v.zro();
                :add_acc_s_f( a, b, r );
            }
        };
    };

    stamp :sub_s =
    {
        func : .f =
        {
            if( a == r || b == r )
            {
                d bhvm_holor_s* buf = bhvm_holor_s!;
                :sub_acc_s_f( a, b, bhvm_holor_s_fit_size( buf.copy_shape_type( r ) ) );
                if( r.v.size ) buf.v.cpy( r.v.1 );
                buf.discard();
            }
            else
            {
                 r.v.zro();
                :sub_acc_s_f( a, b, r );
            }
        };
    };

    stamp :mul_s =
    {
        func : .f =
        {
            if( a == r || b == r )
            {
                d bhvm_holor_s* buf = bhvm_holor_s!;
                :mul_acc_s_f( a, b, bhvm_holor_s_fit_size( buf.copy_shape_type( r ) ) );
                if( r.v.size ) buf.v.cpy( r.v.1 );
                buf.discard();
            }
            else
            {
                r.v.zro();
                :mul_acc_s_f( a, b, r );
            }
        };
    };

    stamp :div_s =
    {
        func : .f =
        {
            if( a == r || b == r )
            {
                d bhvm_holor_s* buf = bhvm_holor_s!;
                :div_acc_s_f( a, b, bhvm_holor_s_fit_size( buf.copy_shape_type( r ) ) );
                if( r.v.size ) buf.v.cpy( r.v.1 );
                buf.discard();
            }
            else
            {
                r.v.zro();
                :div_acc_s_f( a, b, r );
            }
        };
    };

    stamp :pow_s =
    {
        func : .f =
        {
            if( a == r || b == r )
            {
                d bhvm_holor_s* buf = bhvm_holor_s!;
                :pow_acc_s_f( a, b, bhvm_holor_s_fit_size( buf.copy_shape_type( r ) ) );
                if( r.v.size ) buf.v.cpy( r.v.1 );
                buf.discard();
            }
            else
            {
                r.v.zro();
                :pow_acc_s_f( a, b, r );
            }
        };
    };

    /// logic ------------------------------------------------------------------

    group :logic =
    {
        extending stump verbatim :_s = aware : {};

        body body_equal         = { return a == b ? 1 : -1; };
        body body_unequal       = { return a != b ? 1 : -1; };
        body body_larger        = { return a >  b ? 1 : -1; };
        body body_smaller       = { return a <  b ? 1 : -1; };
        body body_larger_equal  = { return a >= b ? 1 : -1; };
        body body_smaller_equal = { return a <= b ? 1 : -1; };
        body body_and           = { return ( ( a > 0 ) && ( b > 0 ) ) ? 1 : -1; };
        body body_or            = { return ( ( a > 0 ) || ( b > 0 ) ) ? 1 : -1; };

        stamp :equal_s         = { func :: .f2 = :body_equal;         func :: .f3 = :body_equal;         func :: .f = ::body_set; };
        stamp :unequal_s       = { func :: .f2 = :body_unequal;       func :: .f3 = :body_unequal;       func :: .f = ::body_set; };
        stamp :larger_s        = { func :: .f2 = :body_larger;        func :: .f3 = :body_larger;        func :: .f = ::body_set; };
        stamp :smaller_s       = { func :: .f2 = :body_smaller;       func :: .f3 = :body_smaller;       func :: .f = ::body_set; };
        stamp :larger_equal_s  = { func :: .f2 = :body_larger_equal;  func :: .f3 = :body_larger_equal;  func :: .f = ::body_set; };
        stamp :smaller_equal_s = { func :: .f2 = :body_smaller_equal; func :: .f3 = :body_smaller_equal; func :: .f = ::body_set; };
        stamp :and_s           = { func :: .f2 = :body_and;           func :: .f3 = :body_and;           func :: .f = ::body_set; };
        stamp :or_s            = { func :: .f2 = :body_or;            func :: .f3 = :body_or;            func :: .f = ::body_set; };
    };

    /// dendrite pass ----------------------------------------------------------

    body body_iff_dp_b = { return ( a > 0 ) ? b : 0; };
    body body_iff_dp_c = { return ( a > 0 ) ? 0 : b; };
    stamp :iff_dp_b_azg_s = { func : .f2 = :body_iff_dp_b; func : .f3 = :body_iff_dp_b; func : .f = :body_acc; };
    stamp :iff_dp_c_azh_s = { func : .f2 = :body_iff_dp_c; func : .f3 = :body_iff_dp_c; func : .f = :body_acc; };

};

// ---------------------------------------------------------------------------------------------------------------------

/// ternary operators
group :ar3 =
{
    extending stump verbatim :_s = aware : {};
    signature void f( c bhvm_holor_s* a, c bhvm_holor_s* b, c bhvm_holor_s* c, m bhvm_holor_s* r );

    /// axon pass --------------------------------------------------------------

    /// dendrite pass ----------------------------------------------------------

};

// ---------------------------------------------------------------------------------------------------------------------

/// extended ternary operators (elementary cyclic indexing)
group :ar3_eci =
{
    extending stump verbatim :_s = aware : {};

    signature f2_t f2( f2_t a, f2_t b, f2_t c );
    signature f3_t f3( f3_t a, f3_t b, f3_t c );
    signature void f( c bhvm_holor_s* a, c bhvm_holor_s* b, c bhvm_holor_s* c, m bhvm_holor_s* r );

    /// sets the target value: r = op( a, b, c )
    body body_set =
    {
        assert( a != r && b != r && c != r );
        assert(  sz_min( a.v.size, sz_min( b.v.size, sz_min( c.v.size, r.v.size ) ) ) > 0 );
        sz_t n = sz_gcd( a.v.size, sz_gcd( b.v.size, sz_gcd( c.v.size, r.v.size ) ) );
        sz_t m = sz_max( a.v.size, sz_max( b.v.size, sz_max( c.v.size, r.v.size ) ) ) / n;

        #define BHVM_SET_CASE( TA_T, TB_T, TC_T, TR_T, FUNC ) \
        { \
            const TA_T *a0 = a->v.data, *a1 = a0; \
            const TB_T *b0 = b->v.data, *b1 = b0; \
            const TC_T *c0 = c->v.data, *c1 = c0; \
                  TR_T *r0 = r->v.data, *r1 = r0; \
            for( sz_t i = 0; i < m; i++ ) \
            { \
                for( sz_t i = 0; i < n; i++ ) r1[ i ] = FUNC( a1[ i ], b1[ i ], c1[ i ] ); \
                a1 = ( a1 + n - a0 ) < a->v.size ? ( a1 + n ) : a0; \
                b1 = ( b1 + n - b0 ) < b->v.size ? ( b1 + n ) : b0; \
                c1 = ( c1 + n - c0 ) < c->v.size ? ( c1 + n ) : c0; \
                r1 = ( r1 + n - r0 ) < r->v.size ? ( r1 + n ) : r0; \
            } \
        }

        switch( BKNIT_FA4( a.v.type, b.v.type, c.v.type, r.v.type ) )
        {
            case BKNIT_F2222: BHVM_SET_CASE( f2_t, f2_t, f2_t, f2_t, @_f2 ); break;
            case BKNIT_F2223: BHVM_SET_CASE( f2_t, f2_t, f2_t, f3_t, @_f3 ); break;
            case BKNIT_F2232: BHVM_SET_CASE( f2_t, f2_t, f3_t, f2_t, @_f3 ); break;
            case BKNIT_F2233: BHVM_SET_CASE( f2_t, f2_t, f3_t, f3_t, @_f3 ); break;
            case BKNIT_F2322: BHVM_SET_CASE( f2_t, f3_t, f2_t, f2_t, @_f3 ); break;
            case BKNIT_F2323: BHVM_SET_CASE( f2_t, f3_t, f2_t, f3_t, @_f3 ); break;
            case BKNIT_F2332: BHVM_SET_CASE( f2_t, f3_t, f3_t, f2_t, @_f3 ); break;
            case BKNIT_F2333: BHVM_SET_CASE( f2_t, f3_t, f3_t, f3_t, @_f3 ); break;
            case BKNIT_F3222: BHVM_SET_CASE( f3_t, f2_t, f2_t, f2_t, @_f3 ); break;
            case BKNIT_F3223: BHVM_SET_CASE( f3_t, f2_t, f2_t, f3_t, @_f3 ); break;
            case BKNIT_F3232: BHVM_SET_CASE( f3_t, f2_t, f3_t, f2_t, @_f3 ); break;
            case BKNIT_F3233: BHVM_SET_CASE( f3_t, f2_t, f3_t, f3_t, @_f3 ); break;
            case BKNIT_F3322: BHVM_SET_CASE( f3_t, f3_t, f2_t, f2_t, @_f3 ); break;
            case BKNIT_F3323: BHVM_SET_CASE( f3_t, f3_t, f2_t, f3_t, @_f3 ); break;
            case BKNIT_F3332: BHVM_SET_CASE( f3_t, f3_t, f3_t, f2_t, @_f3 ); break;
            case BKNIT_F3333: BHVM_SET_CASE( f3_t, f3_t, f3_t, f3_t, @_f3 ); break;
            default: BKNIT_FA4_ERR( a.v.type, b.v.type, c.v.type, r.v.type ); break;
        }

        #undef BHVM_SET_CASE
    };

    /// accumulates the target value: r += op( a, b, c )
    body body_acc =
    {
        assert( a != r && b != r && c != r );
        assert(  sz_min( a.v.size, sz_min( b.v.size, sz_min( c.v.size, r.v.size ) ) ) > 0 );
        sz_t n = sz_gcd( a.v.size, sz_gcd( b.v.size, sz_gcd( c.v.size, r.v.size ) ) );
        sz_t m = sz_max( a.v.size, sz_max( b.v.size, sz_max( c.v.size, r.v.size ) ) ) / n;

        #define BHVM_ACC_CASE( TA_T, TB_T, TC_T, TR_T, FUNC ) \
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

        switch( BKNIT_FA4( a.v.type, b.v.type, c.v.type, r.v.type ) )
        {
            case BKNIT_F2222: BHVM_ACC_CASE( f2_t, f2_t, f2_t, f2_t, @_f2 ); break;
            case BKNIT_F2223: BHVM_ACC_CASE( f2_t, f2_t, f2_t, f3_t, @_f3 ); break;
            case BKNIT_F2232: BHVM_ACC_CASE( f2_t, f2_t, f3_t, f2_t, @_f3 ); break;
            case BKNIT_F2233: BHVM_ACC_CASE( f2_t, f2_t, f3_t, f3_t, @_f3 ); break;
            case BKNIT_F2322: BHVM_ACC_CASE( f2_t, f3_t, f2_t, f2_t, @_f3 ); break;
            case BKNIT_F2323: BHVM_ACC_CASE( f2_t, f3_t, f2_t, f3_t, @_f3 ); break;
            case BKNIT_F2332: BHVM_ACC_CASE( f2_t, f3_t, f3_t, f2_t, @_f3 ); break;
            case BKNIT_F2333: BHVM_ACC_CASE( f2_t, f3_t, f3_t, f3_t, @_f3 ); break;
            case BKNIT_F3222: BHVM_ACC_CASE( f3_t, f2_t, f2_t, f2_t, @_f3 ); break;
            case BKNIT_F3223: BHVM_ACC_CASE( f3_t, f2_t, f2_t, f3_t, @_f3 ); break;
            case BKNIT_F3232: BHVM_ACC_CASE( f3_t, f2_t, f3_t, f2_t, @_f3 ); break;
            case BKNIT_F3233: BHVM_ACC_CASE( f3_t, f2_t, f3_t, f3_t, @_f3 ); break;
            case BKNIT_F3322: BHVM_ACC_CASE( f3_t, f3_t, f2_t, f2_t, @_f3 ); break;
            case BKNIT_F3323: BHVM_ACC_CASE( f3_t, f3_t, f2_t, f3_t, @_f3 ); break;
            case BKNIT_F3332: BHVM_ACC_CASE( f3_t, f3_t, f3_t, f2_t, @_f3 ); break;
            case BKNIT_F3333: BHVM_ACC_CASE( f3_t, f3_t, f3_t, f3_t, @_f3 ); break;
            default: BKNIT_FA4_ERR( a.v.type, b.v.type, c.v.type, r.v.type ); break;
        }

        #undef BHVM_ACC_CASE
    };

    /// axon pass --------------------------------------------------------------

    /// logic ------------------------------------------------------------------

    body body_iff  = { return ( a > 0 ) ? b : c; };
    stamp :iff_s     = { func : .f2 = :body_iff; func : .f3 = :body_iff; func : .f = :body_set; };
    stamp :iff_acc_s = { func : .f2 = :body_iff; func : .f3 = :body_iff; func : .f = :body_acc; };

    /// dendrite pass ----------------------------------------------------------

    body body_div_dp_zabg_f2 = { return -a * b * f2_inv( c * c ); };
    body body_div_dp_zabg_f3 = { return -a * b * f3_inv( c * c ); };
    stamp :div_dp_zabg_s = { func : .f2 = :body_div_dp_zabg_f2; func : .f3 = :body_div_dp_zabg_f3; func : .f = :body_acc; };

    body body_pow_dp_abzf_f2 = { return c * b * f2_pow( a, b - 1 ); };
    body body_pow_dp_abzf_f3 = { return c * b * f3_pow( a, b - 1 ); };
    stamp :pow_dp_abzf_s = { func : .f2 = :body_pow_dp_abzf_f2; func : .f3 = :body_pow_dp_abzf_f3; func : .f = :body_acc; };

    body body_pow_dp_ayzg_f2 = { return ( a > 0 ) ? b * c * logf( a ) : 0; };
    body body_pow_dp_ayzg_f3 = { return ( a > 0 ) ? b * c * log ( a ) : 0; };
    stamp :pow_dp_ayzg_s = { func : .f2 = :body_pow_dp_ayzg_f2; func : .f3 = :body_pow_dp_ayzg_f3; func : .f = :body_acc; };
};

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

