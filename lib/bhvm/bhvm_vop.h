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

#ifndef BHVM_VOP_H
#define BHVM_VOP_H

/**********************************************************************************************************************/

/** Collection of V-Machine-Operators
 */

/**********************************************************************************************************************/

#include "bmath_std.h"
#include "bhvm_planted_vop.h"
#include "bhvm_hop.h"

/**********************************************************************************************************************/

#ifdef TYPEOF_bhvm_vop

PLANT_GROUP( bhvm_vop, bcore_inst )
#ifdef PLANT_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

feature 'ap' void run( const, bhvm_holor_s* ah );

feature 'a' sz_t arity( const );

/// signature of operator (e.g. "ABY")
feature 'a' sc_t sig( const );

feature 'a' sz_t* get_index( mutable );

stamp :ci = bcore_inst { u0_t c; sz_t i; };
stamp :arr_ci = aware bcore_array { :ci_s []; };

/** Sets arguments from index data according to signature
  * Identifiers:
  *   A ... J : a-pass entry channels
  *   Y       : a-pass exit channel
  *   a ... j : d-pass entry channels
  *   y       : d-pass exit channel
  */
feature 'a' void set_arg( mutable, :ci_s* ci ) =
{
    sz_t size   = :a_arity( o ) + 1;
    sc_t  sig   = :a_sig( o );
    sz_t* index = :a_get_index( o );
    for( sz_t i = 0; i < size; i++ )
    {
        u0_t c = sig[ i ];
        ASSERT( c );
        if( ci->c == c )
        {
            index[ i ] = ci->i;
            return;
        }
    }
};

feature 'a' void set_args( mutable, const :arr_ci_s* arr ) =
{
    for( sz_t i = 0; i < arr->size; i++ ) :a_set_arg( o, &arr->data[ i ] );
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// nullary operators
group :ar0 =
{
    stamp :index = bcore_inst { sz_t [ 1 ] v; };

    extending stump verbatim :_ = aware :
    {
        :index_s i;
        func :: :arity = { return 0; };
        func :: :get_index = { return o->i.v; };
    };

    func :: :run = { bhvm_hop_ar0_$R_s_f( &ah[o->i.v[0]] ); };

    /// axon pass --------------------------------------------------------------

    stamp :zro = { func :: :sig = { return "Y"; }; };
    stamp :one = { func :: :sig = { return "Y"; }; };

    /// dendrite pass ----------------------------------------------------------

    stamp :nul_dp = { func :: :sig = { return "a"; }; }; // no action
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// unary operators
group :ar1 =
{
    stamp :index = bcore_inst { sz_t [ 2 ] v; };

    extending stump verbatim :_ = aware :
    {
        :index_s i;
        func :: :arity = { return 1; };
        func :: :get_index = { return o->i.v; };
    };

    func :: :run = { bhvm_hop_ar1_$R_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] ); };

    /// axon pass --------------------------------------------------------------

    stamp :identity   = { func :: :sig = { return "AY"; }; };
    stamp :neg        = { func :: :sig = { return "AY"; }; };
    stamp :floor      = { func :: :sig = { return "AY"; }; };
    stamp :ceil       = { func :: :sig = { return "AY"; }; };
    stamp :exp        = { func :: :sig = { return "AY"; }; };
    stamp :inv        = { func :: :sig = { return "AY"; }; };
    stamp :lgst       = { func :: :sig = { return "AY"; }; };
    stamp :lgst_hard  = { func :: :sig = { return "AY"; }; };
    stamp :lgst_leaky = { func :: :sig = { return "AY"; }; };
    stamp :tanh       = { func :: :sig = { return "AY"; }; };
    stamp :tanh_hard  = { func :: :sig = { return "AY"; }; };
    stamp :tanh_leaky = { func :: :sig = { return "AY"; }; };
    stamp :softplus   = { func :: :sig = { return "AY"; }; };
    stamp :relu       = { func :: :sig = { return "AY"; }; };
    stamp :relu_leaky = { func :: :sig = { return "AY"; }; };

    /// dendrite pass ----------------------------------------------------------

    stamp :identity_dp = { func :: :run = { bhvm_hop_ar1_identity_dp_v_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] ); }; func :: :sig = { return "ya"; }; };
    stamp :neg_dp      = { func :: :run = { bhvm_hop_ar1_neg_dp_v_s_f(      &ah[o->i.v[0]], &ah[o->i.v[1]] ); }; func :: :sig = { return "ya"; }; };

    stamp :add_dp_a = { func :: :run = { bhvm_hop_ar1_add_dp_a_v_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] ); }; func :: :sig = { return "ya"; }; };
    stamp :add_dp_b = { func :: :run = { bhvm_hop_ar1_add_dp_b_v_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] ); }; func :: :sig = { return "yb"; }; };
    stamp :sub_dp_a = { func :: :run = { bhvm_hop_ar1_sub_dp_a_v_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] ); }; func :: :sig = { return "ya"; }; };
    stamp :sub_dp_b = { func :: :run = { bhvm_hop_ar1_sub_dp_b_v_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] ); }; func :: :sig = { return "yb"; }; };

    /// copy special channel ---------------------------------------------------

    stamp :cpy_ay =
    {
        func :: :sig = { return "AY"; };
        func :: :run = { bhvm_hop_ar1_identity_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] ); };
    };

    stamp :cpy_by =
    {
        func :: :sig = { return "BY"; };
        func :: :run = { bhvm_hop_ar1_identity_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] ); };
    };

    /// cast -------------------------------------------------------------------

    // Cast operators make the target weakly reference source data.
    // Value data is always referenced; shape might be referenced.
    // Cast operators are inert as far as actual vm-processing is concerned.
    // They are not intended to actually execute in ap of dp tracks.
    // They are typically placed in the setup track.

};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// binary operators
group :ar2 =
{
    stamp :index = bcore_inst { sz_t [ 3 ] v; };

    extending stump verbatim :_ = aware :
    {
        :index_s i;
        func :: :arity = { return 2; };
        func :: :get_index = { return o->i.v; };
    };

    func :: :run = { bhvm_hop_ar2_$R_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); };

    /// axon pass --------------------------------------------------------------

    stamp :add     = { func :: :sig = { return "ABY"; }; };
    stamp :sub     = { func :: :sig = { return "ABY"; }; };
    stamp :div     = { func :: :sig = { return "ABY"; }; };

    stamp :mul_vvv = { func :: :sig = { return "ABY"; }; };
    stamp :mul_vsv = { func :: :sig = { return "ABY"; }; };
    stamp :mul_svv = { func :: :sig = { return "ABY"; }; };
    stamp :mul_vvs = { func :: :sig = { return "ABY"; }; };
    stamp :mul_mvv = { func :: :sig = { return "ABY"; }; };
    stamp :mul_vmv = { func :: :sig = { return "ABY"; }; };
    stamp :mul_tvv = { func :: :sig = { return "ABY"; }; };
    stamp :mul_vtv = { func :: :sig = { return "ABY"; }; };
    stamp :mul_vvm = { func :: :sig = { return "ABY"; }; };
    stamp :mul_mmm = { func :: :sig = { return "ABY"; }; };
    stamp :mul_mtm = { func :: :sig = { return "ABY"; }; };
    stamp :mul_tmm = { func :: :sig = { return "ABY"; }; };
    stamp :mul_ttm = { func :: :sig = { return "ABY"; }; };

    /// logic ------------------------------------------------------------------

    stamp :equal         = { func :: :sig = { return "ABY"; }; };
    stamp :larger        = { func :: :sig = { return "ABY"; }; };
    stamp :smaller       = { func :: :sig = { return "ABY"; }; };
    stamp :larger_equal  = { func :: :sig = { return "ABY"; }; };
    stamp :smaller_equal = { func :: :sig = { return "ABY"; }; };
    stamp :logic_and     = { func :: :sig = { return "ABY"; }; };
    stamp :logic_or      = { func :: :sig = { return "ABY"; }; };

    /// dendrite pass ----------------------------------------------------------

    stamp :mul_vvv_dp_a = { func :: :run = { bhvm_hop_ar2_mul_acc_vvv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "yBa"; }; };
    stamp :mul_vsv_dp_a = { func :: :run = { bhvm_hop_ar2_mul_acc_vsv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "yBa"; }; };
    stamp :mul_svv_dp_a = { func :: :run = { bhvm_hop_ar2_mul_acc_vvs_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "yBa"; }; };
    stamp :mul_vvs_dp_a = { func :: :run = { bhvm_hop_ar2_mul_acc_svv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "yBa"; }; };
    stamp :mul_mvv_dp_a = { func :: :run = { bhvm_hop_ar2_mul_acc_vvm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "yBa"; }; };
    stamp :mul_vmv_dp_a = { func :: :run = { bhvm_hop_ar2_mul_acc_vtv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "yBa"; }; };
    stamp :mul_tvv_dp_a = { func :: :run = { bhvm_hop_ar2_mul_acc_vvm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "Bya"; }; };
    stamp :mul_vtv_dp_a = { func :: :run = { bhvm_hop_ar2_mul_acc_vmv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "yBa"; }; };
    stamp :mul_vvm_dp_a = { func :: :run = { bhvm_hop_ar2_mul_acc_mvv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "yBa"; }; };

    stamp :mul_vvv_dp_b = { func :: :run = { bhvm_hop_ar2_mul_acc_vvv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "yAb"; }; };
    stamp :mul_vsv_dp_b = { func :: :run = { bhvm_hop_ar2_mul_acc_vvs_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "Ayb"; }; };
    stamp :mul_svv_dp_b = { func :: :run = { bhvm_hop_ar2_mul_acc_vsv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "yAb"; }; };
    stamp :mul_vvs_dp_b = { func :: :run = { bhvm_hop_ar2_mul_acc_svv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "yAb"; }; };
    stamp :mul_mvv_dp_b = { func :: :run = { bhvm_hop_ar2_mul_acc_tvv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "Ayb"; }; };
    stamp :mul_vmv_dp_b = { func :: :run = { bhvm_hop_ar2_mul_acc_vvm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "Ayb"; }; };
    stamp :mul_tvv_dp_b = { func :: :run = { bhvm_hop_ar2_mul_acc_mvv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "Ayb"; }; };
    stamp :mul_vtv_dp_b = { func :: :run = { bhvm_hop_ar2_mul_acc_vvm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "yAb"; }; };
    stamp :mul_vvm_dp_b = { func :: :run = { bhvm_hop_ar2_mul_acc_vmv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "Ayb"; }; };

    stamp :mul_mmm_dp_a = { func :: :run = { bhvm_hop_ar2_mul_acc_mtm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "yBa"; }; };
    stamp :mul_mtm_dp_a = { func :: :run = { bhvm_hop_ar2_mul_acc_mmm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "yBa"; }; };
    stamp :mul_tmm_dp_a = { func :: :run = { bhvm_hop_ar2_mul_acc_mtm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "Bya"; }; };
    stamp :mul_ttm_dp_a = { func :: :run = { bhvm_hop_ar2_mul_acc_ttm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "Bya"; }; };

    stamp :mul_mmm_dp_b = { func :: :run = { bhvm_hop_ar2_mul_acc_tmm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "Ayb"; }; };
    stamp :mul_mtm_dp_b = { func :: :run = { bhvm_hop_ar2_mul_acc_tmm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "yAb"; }; };
    stamp :mul_tmm_dp_b = { func :: :run = { bhvm_hop_ar2_mul_acc_mmm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "Ayb"; }; };
    stamp :mul_ttm_dp_b = { func :: :run = { bhvm_hop_ar2_mul_acc_ttm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "yAb"; }; };

    stamp :div_dp_a      = { func :: :run = { bhvm_hop_ar2_div_dp_a_vb_s_f(      &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "yBa"; }; };
    stamp :exp_dp        = { func :: :run = { bhvm_hop_ar2_exp_dp_vy_s_f(        &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "yYa"; }; };
    stamp :inv_dp        = { func :: :run = { bhvm_hop_ar2_inv_dp_vy_s_f(        &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "yYa"; }; };
    stamp :lgst_dp       = { func :: :run = { bhvm_hop_ar2_lgst_dp_vy_s_f(       &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "yYa"; }; };
    stamp :lgst_hard_dp  = { func :: :run = { bhvm_hop_ar2_lgst_hard_dp_vy_s_f(  &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "yYa"; }; };
    stamp :lgst_leaky_dp = { func :: :run = { bhvm_hop_ar2_lgst_leaky_dp_vy_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "yYa"; }; };
    stamp :tanh_dp       = { func :: :run = { bhvm_hop_ar2_tanh_dp_vy_s_f(       &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "yYa"; }; };
    stamp :tanh_hard_dp  = { func :: :run = { bhvm_hop_ar2_tanh_hard_dp_vy_s_f(  &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "yYa"; }; };
    stamp :tanh_leaky_dp = { func :: :run = { bhvm_hop_ar2_tanh_leaky_dp_vy_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "yYa"; }; };
    stamp :softplus_dp   = { func :: :run = { bhvm_hop_ar2_softplus_dp_vy_s_f(   &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "yYa"; }; };
    stamp :relu_dp       = { func :: :run = { bhvm_hop_ar2_relu_dp_vy_s_f(       &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "yYa"; }; };
    stamp :relu_leaky_dp = { func :: :run = { bhvm_hop_ar2_relu_leaky_dp_vy_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "yYa"; }; };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// ternary operators
group :ar3 =
{
    stamp :index = bcore_inst { sz_t [ 4 ] v; };

    extending stump verbatim :_ = aware :
    {
        :index_s i;
        func :: :arity = { return 3; };
        func :: :get_index = { return o->i.v; };
    };

    func :: :run = { bhvm_hop_ar3_$R_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]], &ah[o->i.v[3]] ); };

    /// axon pass --------------------------------------------------------------

    /// dendrite pass ----------------------------------------------------------

    stamp :div_dp_b = { func :: :run = { bhvm_hop_ar3_div_dp_b_vab_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]], &ah[o->i.v[3]] ); }; func :: :sig = { return "yABb"; }; };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // PLANT_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // TYPEOF_bhvm_vop

/**********************************************************************************************************************/

vd_t bhvm_vop_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BHVM_VOP_H
