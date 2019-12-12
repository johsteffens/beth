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

    stamp :zro = { func : :f = :body_lop_cv; };
    stamp :one = { func : :f = :body_lop_cv; };
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

    body body_lop_vv_cv =
    {
        assert( a->v.size == r->v.size );
        assert( b->v.size == r->v.size );
        bhvm_lop_ar2_$R_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
    };

    body body_lop_vvv =
    {
        assert( a->v.size == r->v.size );
        assert( b->v.size == r->v.size );
        bhvm_lop_ar2_$R_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
    };

    body body_lop_svv =
    {
        assert( a->v.size == 1 );
        assert( b->v.size == r->v.size );
        bhvm_lop_ar2_$R_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
    };

    body body_lop_vsv =
    {
        assert( a->v.size == r->v.size );
        assert( b->v.size == 1 );
        bhvm_lop_ar2_$R_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
    };

    body body_lop_vvs =
    {
        assert( a->v.size == b->v.size );
        assert( r->v.size == 1 );
        bhvm_lop_ar2_$R_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
    };

    body body_lop_vs_cv =
    {
        assert( a->v.size == r->v.size );
        assert( b->v.size == 1 );
        bhvm_lop_ar2_$R_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
    };

    body body_lop_vv_av =
    {
        assert( a->v.size == r->v.size );
        assert( b->v.size == r->v.size );
        bhvm_lop_ar2_$R_s_f( BKNIT_FA3( a->v.type, b->v.type, r->v.type ), a->v.data, b->v.data, r->v.data, r->v.size );
    };

    /// axon pass --------------------------------------------------------------

    stamp :add     = { func : :f = :body_lop_vv_cv; };
    stamp :sub     = { func : :f = :body_lop_vv_cv; };
    stamp :mul     = { func : :f = :body_lop_vv_cv; };
    stamp :div     = { func : :f = :body_lop_vv_cv; };

    /// dendrite pass ----------------------------------------------------------

    stamp :mul_dp_a_vb      = { func : :f = :body_lop_vv_av; };
    stamp :mul_dp_b_va      = { func : :f = :body_lop_vv_av; };
    stamp :div_dp_a_vb      = { func : :f = :body_lop_vv_av; };
    stamp :exp_dp_vy        = { func : :f = :body_lop_vv_av; };
    stamp :inv_dp_vy        = { func : :f = :body_lop_vv_av; };
    stamp :lgst_dp_vy       = { func : :f = :body_lop_vv_av; };
    stamp :lgst_hard_dp_vy  = { func : :f = :body_lop_vv_av; };
    stamp :lgst_leaky_dp_vy = { func : :f = :body_lop_vv_av; };
    stamp :tanh_dp_vy       = { func : :f = :body_lop_vv_av; };
    stamp :tanh_hard_dp_vy  = { func : :f = :body_lop_vv_av; };
    stamp :tanh_leaky_dp_vy = { func : :f = :body_lop_vv_av; };
    stamp :softplus_dp_vy   = { func : :f = :body_lop_vv_av; };
    stamp :relu_dp_vy       = { func : :f = :body_lop_vv_av; };
    stamp :relu_leaky_dp_vy = { func : :f = :body_lop_vv_av; };
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
