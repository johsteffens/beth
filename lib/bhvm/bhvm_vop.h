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

feature 'a' sz_t* get_index_arr( mutable );

feature 'a' sz_t get_index( const, sz_t index ) =
{
    ASSERT( index  >= 0 && index <= :a_arity( o ) );
    return :a_get_index_arr( (:*)o )[ index ];
};

feature 'a' :* set_index( mutable, sz_t index, sz_t idx_val ) =
{
    ASSERT( index  >= 0 && index <= :a_arity( o ) );
    :a_get_index_arr( o )[ index ] = idx_val;
    return o;
};

signature void push_ci( mutable, u0_t c, sz_t i );

stamp :ci = bcore_inst
{
    u0_t c; sz_t i;
    func : :push_ci = { o->c = c; o->i = i; };
};

signature sz_t i_of_c( const, u0_t c );
signature u0_t c_of_i( const, sz_t i );

stamp :arr_ci = aware bcore_array
{
    :ci_s [];
    func : :push_ci =
    {
        :ci_s_push_ci( @_push( o ), c, i );
    };

    func : :i_of_c = { BFOR_EACH( j, o ) if( o->data[ j ].c == c ) return o->data[ j ].i; return -1; };
    func : :c_of_i = { BFOR_EACH( j, o ) if( o->data[ j ].i == i ) return o->data[ j ].c; return -1; };
};

/** Sets arguments from index data according to signature
  * Identifiers:
  *          entry      exit
  * a-pass: a,b,c,d,e    y
  * d-pass: f,g,h,i,j    z
  */
feature 'a' void set_arg( mutable, :ci_s* ci ) =
{
    sz_t size   = :a_arity( o ) + 1;
    sc_t  sig   = :a_sig( o );
    sz_t* index = :a_get_index_arr( o );
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

feature 'a' :* set_args( mutable, const :arr_ci_s* arr ) =
{
    for( sz_t i = 0; i < arr->size; i++ ) :a_set_arg( o, &arr->data[ i ] );
    return o;
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
        func :: :get_index_arr = { return o->i.v; };
    };

    /// axon pass --------------------------------------------------------------

    stamp :zro = { func :: :run = { bhvm_hop_ar0_zro_s_f( &ah[o->i.v[0]] ); }; func :: :sig = { return "y"; }; };
    stamp :one = { func :: :run = { bhvm_hop_ar0_one_s_f( &ah[o->i.v[0]] ); }; func :: :sig = { return "y"; }; };

    stamp :determine = { func :: :run = { bhvm_holor_s_fit_size( &ah[o->i.v[0]] ); }; func :: :sig = { return "y"; }; };
    stamp :vacate    = { func :: :run = { bhvm_value_s_clear( &ah[o->i.v[0]].v ); };  func :: :sig = { return "y"; }; };

    stamp :randomize =
    {
        u2_t rseed   = 1234;
        f3_t min     = -0.5;
        f3_t max     =  0.5;
        f3_t density =  1.0;

        func :: :run =
        {
            sz_t i = o->i.v[ 0 ];
            u2_t rval = o->rseed + i;
            bhvm_value_s_set_random( &ah[ i ].v, o->density, o->min, o->max, &rval );
        };

        func :: :sig = { return "y"; };
    };

    /// dendrite pass ----------------------------------------------------------

    stamp :nul_dp = { func :: :run = {}; func :: :sig = { return "f"; }; }; // no action
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
        func :: :get_index_arr = { return o->i.v; };
    };

    func :: :run = { bhvm_hop_ar1_$R_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] ); };

    /// axon pass --------------------------------------------------------------

    stamp :identity   = { func :: :sig = { return "ay"; }; };
    stamp :neg        = { func :: :sig = { return "ay"; }; };
    stamp :floor      = { func :: :sig = { return "ay"; }; };
    stamp :ceil       = { func :: :sig = { return "ay"; }; };
    stamp :exp        = { func :: :sig = { return "ay"; }; };
    stamp :inv        = { func :: :sig = { return "ay"; }; };
    stamp :abs        = { func :: :sig = { return "ay"; }; };
    stamp :l1         = { func :: :sig = { return "ay"; }; };
    stamp :lgst       = { func :: :sig = { return "ay"; }; };
    stamp :lgst_hard  = { func :: :sig = { return "ay"; }; };
    stamp :lgst_leaky = { func :: :sig = { return "ay"; }; };
    stamp :tanh       = { func :: :sig = { return "ay"; }; };
    stamp :tanh_hard  = { func :: :sig = { return "ay"; }; };
    stamp :tanh_leaky = { func :: :sig = { return "ay"; }; };
    stamp :softplus   = { func :: :sig = { return "ay"; }; };
    stamp :relu       = { func :: :sig = { return "ay"; }; };
    stamp :relu_leaky = { func :: :sig = { return "ay"; }; };

    /// dendrite pass ----------------------------------------------------------

    stamp :identity_dp = { func :: :run = { bhvm_hop_ar1_identity_dp_zf_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] ); }; func :: :sig = { return "zf"; }; };
    stamp :neg_dp      = { func :: :run = { bhvm_hop_ar1_neg_dp_zf_s_f(      &ah[o->i.v[0]], &ah[o->i.v[1]] ); }; func :: :sig = { return "zf"; }; };

    stamp :add_dp_a = { func :: :run = { bhvm_hop_ar1_add_dp_zf_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] ); }; func :: :sig = { return "zf"; }; };
    stamp :add_dp_b = { func :: :run = { bhvm_hop_ar1_add_dp_zg_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] ); }; func :: :sig = { return "zg"; }; };
    stamp :sub_dp_a = { func :: :run = { bhvm_hop_ar1_sub_dp_zf_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] ); }; func :: :sig = { return "zf"; }; };
    stamp :sub_dp_b = { func :: :run = { bhvm_hop_ar1_sub_dp_zg_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] ); }; func :: :sig = { return "zg"; }; };

    /// copy special channel ---------------------------------------------------

    stamp :cpy_ay =
    {
        func :: :sig = { return "ay"; };
        func :: :run = { bhvm_hop_ar1_identity_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] ); };
    };

    stamp :cpy_by =
    {
        func :: :sig = { return "by"; };
        func :: :run = { bhvm_hop_ar1_identity_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]] ); };
    };

    /// cast -------------------------------------------------------------------

    // Cast operators make the target weakly reference source data.
    // Value data is always referenced; shape might be referenced.
    // Cast operators are inert as far as actual vm-processing is concerned.
    // They are not intended to actually execute in ap of dp tracks.
    // They are typically placed in the setup track.

    /// y references a
    stamp :fork =
    {
        func :: :sig = { return "ay"; };
        func :: :run = { bhvm_holor_s_fork( &ah[o->i.v[1]], &ah[o->i.v[0]] ); };
    };

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
        func :: :get_index_arr = { return o->i.v; };
    };

    func :: :run = { bhvm_hop_ar2_$R_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); };

    /// axon pass --------------------------------------------------------------

    stamp :add     = { func :: :sig = { return "aby"; }; };
    stamp :sub     = { func :: :sig = { return "aby"; }; };
    stamp :div     = { func :: :sig = { return "aby"; }; };

    stamp :mul_vvv = { func :: :sig = { return "aby"; }; };
    stamp :mul_vsv = { func :: :sig = { return "aby"; }; };
    stamp :mul_svv = { func :: :sig = { return "aby"; }; };
    stamp :mul_vvs = { func :: :sig = { return "aby"; }; };
    stamp :mul_mvv = { func :: :sig = { return "aby"; }; };
    stamp :mul_vmv = { func :: :sig = { return "aby"; }; };
    stamp :mul_tvv = { func :: :sig = { return "aby"; }; };
    stamp :mul_vtv = { func :: :sig = { return "aby"; }; };
    stamp :mul_vvm = { func :: :sig = { return "aby"; }; };
    stamp :mul_mmm = { func :: :sig = { return "aby"; }; };
    stamp :mul_mtm = { func :: :sig = { return "aby"; }; };
    stamp :mul_tmm = { func :: :sig = { return "aby"; }; };
    stamp :mul_ttm = { func :: :sig = { return "aby"; }; };

    /// logic ------------------------------------------------------------------

    stamp :equal         = { func :: :sig = { return "aby"; }; };
    stamp :larger        = { func :: :sig = { return "aby"; }; };
    stamp :smaller       = { func :: :sig = { return "aby"; }; };
    stamp :larger_equal  = { func :: :sig = { return "aby"; }; };
    stamp :smaller_equal = { func :: :sig = { return "aby"; }; };
    stamp :logic_and     = { func :: :sig = { return "aby"; }; };
    stamp :logic_or      = { func :: :sig = { return "aby"; }; };

    /// dendrite pass ----------------------------------------------------------

    stamp :mul_vvv_dp_a = { func :: :run = { bhvm_hop_ar2_mul_acc_vvv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "zbf"; }; };
    stamp :mul_vsv_dp_a = { func :: :run = { bhvm_hop_ar2_mul_acc_vsv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "zbf"; }; };
    stamp :mul_svv_dp_a = { func :: :run = { bhvm_hop_ar2_mul_acc_vvs_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "zbf"; }; };
    stamp :mul_vvs_dp_a = { func :: :run = { bhvm_hop_ar2_mul_acc_svv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "zbf"; }; };
    stamp :mul_mvv_dp_a = { func :: :run = { bhvm_hop_ar2_mul_acc_vvm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "zbf"; }; };
    stamp :mul_vmv_dp_a = { func :: :run = { bhvm_hop_ar2_mul_acc_vtv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "zbf"; }; };
    stamp :mul_tvv_dp_a = { func :: :run = { bhvm_hop_ar2_mul_acc_vvm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "bzf"; }; };
    stamp :mul_vtv_dp_a = { func :: :run = { bhvm_hop_ar2_mul_acc_vmv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "zbf"; }; };
    stamp :mul_vvm_dp_a = { func :: :run = { bhvm_hop_ar2_mul_acc_mvv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "zbf"; }; };

    stamp :mul_vvv_dp_b = { func :: :run = { bhvm_hop_ar2_mul_acc_vvv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "zag"; }; };
    stamp :mul_vsv_dp_b = { func :: :run = { bhvm_hop_ar2_mul_acc_vvs_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "azg"; }; };
    stamp :mul_svv_dp_b = { func :: :run = { bhvm_hop_ar2_mul_acc_vsv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "zag"; }; };
    stamp :mul_vvs_dp_b = { func :: :run = { bhvm_hop_ar2_mul_acc_svv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "zag"; }; };
    stamp :mul_mvv_dp_b = { func :: :run = { bhvm_hop_ar2_mul_acc_tvv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "azg"; }; };
    stamp :mul_vmv_dp_b = { func :: :run = { bhvm_hop_ar2_mul_acc_vvm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "azg"; }; };
    stamp :mul_tvv_dp_b = { func :: :run = { bhvm_hop_ar2_mul_acc_mvv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "azg"; }; };
    stamp :mul_vtv_dp_b = { func :: :run = { bhvm_hop_ar2_mul_acc_vvm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "zag"; }; };
    stamp :mul_vvm_dp_b = { func :: :run = { bhvm_hop_ar2_mul_acc_vmv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "azg"; }; };

    stamp :mul_mmm_dp_a = { func :: :run = { bhvm_hop_ar2_mul_acc_mtm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "zbf"; }; };
    stamp :mul_mtm_dp_a = { func :: :run = { bhvm_hop_ar2_mul_acc_mmm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "zbf"; }; };
    stamp :mul_tmm_dp_a = { func :: :run = { bhvm_hop_ar2_mul_acc_mtm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "bzf"; }; };
    stamp :mul_ttm_dp_a = { func :: :run = { bhvm_hop_ar2_mul_acc_ttm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "bzf"; }; };

    stamp :mul_mmm_dp_b = { func :: :run = { bhvm_hop_ar2_mul_acc_tmm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "azg"; }; };
    stamp :mul_mtm_dp_b = { func :: :run = { bhvm_hop_ar2_mul_acc_tmm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "zag"; }; };
    stamp :mul_tmm_dp_b = { func :: :run = { bhvm_hop_ar2_mul_acc_mmm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "azg"; }; };
    stamp :mul_ttm_dp_b = { func :: :run = { bhvm_hop_ar2_mul_acc_ttm_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "zag"; }; };

    stamp :div_dp_a      = { func :: :run = { bhvm_hop_ar2_div_dp_zbf_s_f(        &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "zbf"; }; };
    stamp :exp_dp        = { func :: :run = { bhvm_hop_ar2_exp_dp_zyf_s_f(        &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "zyf"; }; };
    stamp :inv_dp        = { func :: :run = { bhvm_hop_ar2_inv_dp_zyf_s_f(        &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "zyf"; }; };
    stamp :lgst_dp       = { func :: :run = { bhvm_hop_ar2_lgst_dp_zyf_s_f(       &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "zyf"; }; };
    stamp :lgst_hard_dp  = { func :: :run = { bhvm_hop_ar2_lgst_hard_dp_zyf_s_f(  &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "zyf"; }; };
    stamp :lgst_leaky_dp = { func :: :run = { bhvm_hop_ar2_lgst_leaky_dp_zyf_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "zyf"; }; };
    stamp :tanh_dp       = { func :: :run = { bhvm_hop_ar2_tanh_dp_zyf_s_f(       &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "zyf"; }; };
    stamp :tanh_hard_dp  = { func :: :run = { bhvm_hop_ar2_tanh_hard_dp_zyf_s_f(  &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "zyf"; }; };
    stamp :tanh_leaky_dp = { func :: :run = { bhvm_hop_ar2_tanh_leaky_dp_zyf_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "zyf"; }; };
    stamp :softplus_dp   = { func :: :run = { bhvm_hop_ar2_softplus_dp_zyf_s_f(   &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "zyf"; }; };
    stamp :relu_dp       = { func :: :run = { bhvm_hop_ar2_relu_dp_zyf_s_f(       &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "zyf"; }; };
    stamp :relu_leaky_dp = { func :: :run = { bhvm_hop_ar2_relu_leaky_dp_zyf_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "zyf"; }; };

    stamp :branch_vvvv_dp_b = { func :: :run = { bhvm_hop_ar2_branch_vvvv_dp_azg_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "azg"; }; };
    stamp :branch_vvvv_dp_c = { func :: :run = { bhvm_hop_ar2_branch_vvvv_dp_azh_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]] ); }; func :: :sig = { return "azh"; }; };
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
        func :: :get_index_arr = { return o->i.v; };
    };

    func :: :run = { bhvm_hop_ar3_$R_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]], &ah[o->i.v[3]] ); };

    /// axon pass --------------------------------------------------------------

    stamp :branch_vvvv = { func :: :run = { bhvm_hop_ar3_branch_vvvv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]], &ah[o->i.v[3]] ); }; func :: :sig = { return "abcy"; }; };
    stamp :branch_svvv = { func :: :run = { bhvm_hop_ar3_branch_svvv_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]], &ah[o->i.v[3]] ); }; func :: :sig = { return "abcy"; }; };

    /// dendrite pass ----------------------------------------------------------

    stamp :div_dp_b =
    {
        func :: :run = { bhvm_hop_ar3_div_dp_zabg_s_f( &ah[o->i.v[0]], &ah[o->i.v[1]], &ah[o->i.v[2]], &ah[o->i.v[3]] ); };
        func :: :sig = { return "zabg"; };
    };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // PLANT_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void bhvm_vop_a_to_sink( bhvm_vop* o, bcore_sink* sink );

#endif // TYPEOF_bhvm_vop

/**********************************************************************************************************************/

vd_t bhvm_vop_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BHVM_VOP_H
