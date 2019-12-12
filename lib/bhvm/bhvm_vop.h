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
#include "bhvm_planted.h"
#include "bhvm_hop.h"

/**********************************************************************************************************************/

#ifdef TYPEOF_bhvm_vop

PLANT_GROUP( bhvm_vop, bcore_inst )
#ifdef PLANT_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

feature 'ap' void run( const, bhvm_holor_s* ah );

feature 'a' sz_t arity( const );

/// signature of operator (e.g. "aby")
feature 'a' sc_t sig( const );

feature 'a' sz_t* get_index( mutable );

stamp :ci = bcore_inst { u0_t c; sz_t i; };
stamp :arr_ci = aware bcore_array { :ci_s []; };

/** Sets arguments from index data according to signature
  * Identifiers:
  *   a ... j : a-pass input channels
  *   y       : a-pass output channel
  *   v       : d-pass gradient (at output channel)
  *   u       : d-pass gradient (result at selected input channel)
  */
feature 'a' void set_arg( mutable, :ci_s* ci ) =
{
    sz_t size   = :a_arity( o ) + 1;
    sc_t  sig   = :a_sig( o );
    sz_t* index = :a_get_index( o );
    for( sz_t i = 0; i < size; i++ )
    {
        if( ci->c == sig[ i ] )
        {
            index[ i ] = ci->i;
            break;
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

    stamp :zro = { func :: :sig = { return "y"; }; };
    stamp :one = { func :: :sig = { return "y"; }; };

    /// dendrite pass ----------------------------------------------------------

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

    stamp :identity   = { func :: :sig = { return "ay"; }; };
    stamp :neg        = { func :: :sig = { return "ay"; }; };
    stamp :floor      = { func :: :sig = { return "ay"; }; };
    stamp :ceil       = { func :: :sig = { return "ay"; }; };
    stamp :exp        = { func :: :sig = { return "ay"; }; };
    stamp :inv        = { func :: :sig = { return "ay"; }; };
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

    stamp :add_dp_a_v = { func :: :sig = { return "vu"; }; };
    stamp :add_dp_b_v = { func :: :sig = { return "vu"; }; };
    stamp :sub_dp_a_v = { func :: :sig = { return "vu"; }; };
    stamp :sub_dp_b_v = { func :: :sig = { return "vu"; }; };
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

    stamp :add = { func :: :sig = { return "aby"; }; };
    stamp :sub = { func :: :sig = { return "aby"; }; };
    stamp :mul = { func :: :sig = { return "aby"; }; };
    stamp :div = { func :: :sig = { return "aby"; }; };

    /// dendrite pass ----------------------------------------------------------

    stamp :mul_dp_a_vb      = { func :: :sig = { return "vbu"; }; };
    stamp :mul_dp_b_va      = { func :: :sig = { return "vau"; }; };
    stamp :div_dp_a_vb      = { func :: :sig = { return "vbu"; }; };
    stamp :exp_dp_vy        = { func :: :sig = { return "vyu"; }; };
    stamp :inv_dp_vy        = { func :: :sig = { return "vyu"; }; };
    stamp :lgst_dp_vy       = { func :: :sig = { return "vyu"; }; };
    stamp :lgst_hard_dp_vy  = { func :: :sig = { return "vyu"; }; };
    stamp :lgst_leaky_dp_vy = { func :: :sig = { return "vyu"; }; };
    stamp :tanh_dp_vy       = { func :: :sig = { return "vyu"; }; };
    stamp :tanh_hard_dp_vy  = { func :: :sig = { return "vyu"; }; };
    stamp :tanh_leaky_dp_vy = { func :: :sig = { return "vyu"; }; };
    stamp :softplus_dp_vy   = { func :: :sig = { return "vyu"; }; };
    stamp :relu_dp_vy       = { func :: :sig = { return "vyu"; }; };
    stamp :relu_leaky_dp_vy = { func :: :sig = { return "vyu"; }; };
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

    stamp :div_dp_b_vab = { func :: :sig = { return "vabu"; }; };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // PLANT_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // TYPEOF_bhvm_vop

/**********************************************************************************************************************/

vd_t bhvm_vop_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BHVM_VOP_H
