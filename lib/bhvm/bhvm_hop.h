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

    stamp :identity   = { func : :f = :body_lop_v_cv; };
    stamp :neg        = { func : :f = :body_lop_v_cv; };
    stamp :floor      = { func : :f = :body_lop_v_cv; };
    stamp :ceil       = { func : :f = :body_lop_v_cv; };
    stamp :exp        = { func : :f = :body_lop_v_cv; };
    stamp :inv        = { func : :f = :body_lop_v_cv; };
    stamp :abs        = { func : :f = :body_lop_v_cv; };
    stamp :sig        = { func : :f = :body_lop_v_cv; };
    stamp :l1         = { func : :f = :body_lop_v_cs; };
    stamp :sqr        = { func : :f = :body_lop_v_cs; };
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

    stamp :identity_dp_v = { func : :f = :body_lop_v_av; };
    stamp :neg_dp_v      = { func : :f = :body_lop_v_av; };

    stamp :add_dp_a_v = { func : :f = :body_lop_v_av; };
    stamp :add_dp_b_v = { func : :f = :body_lop_v_av; };
    stamp :sub_dp_a_v = { func : :f = :body_lop_v_av; };
    stamp :sub_dp_b_v = { func : :f = :body_lop_v_av; };

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
    body body_lop_a = { bhvm_lop_ar2_$R_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, a->v.size ); }; // a has full size

    body body_lop_ma = { bhvm_lop_ar2_$R_s_f_m( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, a->s.data[ 1 ], a->s.data[ 0 ] ); }; // a is matrix
    body body_lop_mb = { bhvm_lop_ar2_$R_s_f_m( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, b->s.data[ 1 ], b->s.data[ 0 ] ); }; // b is matrix
    body body_lop_mr = { bhvm_lop_ar2_$R_s_f_m( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->s.data[ 1 ], r->s.data[ 0 ] ); }; // r is matrix

    /// axon pass --------------------------------------------------------------

    stamp :add     = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :sub     = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :div     = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :sub_sqr = { func : :f = :body_assert_vvs : :body_lop_a; }; // r = ( a - b )^2
    stamp :sub_l1  = { func : :f = :body_assert_vvs : :body_lop_a; }; // r = l1-norm of ( a - b )

    /// logic ------------------------------------------------------------------

    stamp :equal         = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :larger        = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :smaller       = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :larger_equal  = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :smaller_equal = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :logic_and     = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :logic_or      = { func : :f = :body_assert_vvv : :body_lop_r; };

    /// dendrite pass ----------------------------------------------------------

    stamp :div_dp_a_vb      = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :exp_dp_vy        = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :inv_dp_vy        = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :lgst_dp_vy       = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :lgst_hard_dp_vy  = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :lgst_leaky_dp_vy = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :tanh_dp_vy       = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :tanh_hard_dp_vy  = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :tanh_leaky_dp_vy = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :softplus_dp_vy   = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :relu_dp_vy       = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :relu_leaky_dp_vy = { func : :f = :body_assert_vvv : :body_lop_r; };

    /// mul axon pass ----------------------------------------------------------

    stamp :mul_vvv = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :mul_vsv = { func : :f = :body_assert_vsv : :body_lop_r; };
    stamp :mul_svv = { func : :f = :body_assert_svv : { bhvm_lop_ar2_mul_vsv_s_f( BKNIT_FA3( b->v.type, a->v.type, r->v.type ), b->v.data, a->v.data, r->v.data, r->v.size ); }; };
    stamp :mul_vvs = { func : :f = :body_assert_vvs : :body_lop_a; };
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

    stamp :mul_acc_vvv = { func : :f = :body_assert_vvv : :body_lop_r; };
    stamp :mul_acc_vsv = { func : :f = :body_assert_vsv : :body_lop_r; };
    stamp :mul_acc_svv = { func : :f = :body_assert_svv : { bhvm_lop_ar2_mul_acc_vsv_s_f( BKNIT_FA3( b->v.type, a->v.type, r->v.type ), b->v.data, a->v.data, r->v.data, r->v.size ); }; };
    stamp :mul_acc_vvs = { func : :f = :body_assert_vvs : :body_lop_a; };
    stamp :mul_acc_mvv = { func : :f = :body_assert_mvv : :body_lop_ma; };
    stamp :mul_acc_vmv = { func : :f = :body_assert_vmv : :body_lop_mb; };
    stamp :mul_acc_tvv = { func : :f = :body_assert_tvv : { bhvm_lop_ar2_mul_acc_vmv_s_f_m( BKNIT_FA3( b->v.type, a->v.type, r->v.type ), b->v.data, a->v.data, r->v.data, a->s.data[ 1 ], a->s.data[ 0 ] ); }; };
    stamp :mul_acc_vtv = { func : :f = :body_assert_vtv : { bhvm_lop_ar2_mul_acc_mvv_s_f_m( BKNIT_FA3( b->v.type, a->v.type, r->v.type ), b->v.data, a->v.data, r->v.data, b->s.data[ 1 ], b->s.data[ 0 ] ); }; };
    stamp :mul_acc_vvm = { func : :f = :body_assert_vvm : :body_lop_mr; };

    stamp :mul_acc_mmm = { func : :f; };
    stamp :mul_acc_mtm = { func : :f; };
    stamp :mul_acc_tmm = { func : :f; };
    stamp :mul_acc_ttm = { func : :f; };

};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// ternary operators
group :ar3 =
{
    extending stump verbatim :_ = aware : {};
    signature void f( plain, const bhvm_holor_s* a, const bhvm_holor_s* b, const bhvm_holor_s* c, bhvm_holor_s* r );

    body body_lop_vvv_cv =
    {
        assert( a->v.size == r->v.size );
        assert( b->v.size == r->v.size );
        assert( c->v.size == r->v.size );
        bhvm_lop_ar3_$R_s_f( BKNIT_FA4( a->v.type, b->v.type, c->v.type, r->v.type ), a->v.data, b->v.data, c->v.data, r->v.data, r->v.size );
    };

    body body_lop_vvv_av =
    {
        assert( a->v.size == r->v.size );
        assert( b->v.size == r->v.size );
        assert( c->v.size == r->v.size );
        bhvm_lop_ar3_$R_s_f( BKNIT_FA4( a->v.type, b->v.type, c->v.type, r->v.type ), a->v.data, b->v.data, c->v.data, r->v.data, r->v.size );
    };

    /// axon pass --------------------------------------------------------------

    /// dendrite pass ----------------------------------------------------------

    stamp :div_dp_b_vab = { func : :f = :body_lop_vvv_av; };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // PLANT_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // TYPEOF_bhvm_hop

/**********************************************************************************************************************/

vd_t bhvm_hop_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BHVM_HOP_H
