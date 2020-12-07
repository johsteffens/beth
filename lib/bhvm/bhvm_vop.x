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

/** Collection of V-Machine-Operators
 */

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

stamp :arr_s = aware bcore_array
{
    aware bhvm_vop => [];
//    func (   @* set_size( mutable, sz_t size ))     = { return x_array_set_size( o, size ); };
//    func (      @* clear( mutable ))                = { return x_array_clear( o ); };
//    func (bhvm_vop* push( mutable ))                = { return x_array_push( o ); };
//    func (bhvm_vop* push_d( mutable, bhvm_vop* v )) = { return x_array_push_d( o, v ); };
//    func (bhvm_vop* push_c( mutable, bhvm_vop* v )) = { return x_array_push_c( o, v ); };
};

feature 'ap' void run( const, bhvm_holor_s** ah );

feature sz_t arity( const );

/// signature of operator (e.g. "ABY")
feature sc_t sig( const );

feature sz_t* get_index_arr( mutable );

feature sz_t get_index( const, sz_t index ) =
{
    ASSERT( index  >= 0 && index <= o.arity() );
    return cast(o,:*).get_index_arr()[ index ];
};

feature :* set_index( mutable, sz_t index, sz_t idx_val ) =
{
    ASSERT( index >= 0 && index <= o.arity() );
    o.get_index_arr()[ index ] = idx_val;
    return o;
};

feature :* set_index_arr( mutable, sz_t* idx_arr, sz_t size ) =
{
    ASSERT( size == o.arity() + 1 );
    sz_t* o_idx_arr = o.get_index_arr();
    BFOR_SIZE( i, size )
    {
        sz_t idx_val = idx_arr[ i ];
        ASSERT( idx_val >= 0 );
        o_idx_arr[ i ] = idx_val;
    }
    return o;
};

signature void push_ci( mutable, u0_t c, sz_t i );

stamp :ci_s = bcore_inst
{
    u0_t c; sz_t i;
    func : .push_ci = { o.c = c; o.i = i; };
};

signature sz_t i_of_c( const, u0_t c );
signature u0_t c_of_i( const, sz_t i );

stamp :arr_ci_s = aware bcore_array
{
    :ci_s [];
    func : .push_ci =
    {
        :ci_s_push_ci( o.push(), c, i );
    };

    func : .i_of_c =
    {
        foreach( $* e in o ) if( e.c == c ) return e.i;
        return -1;
    };

    func : .c_of_i =
    {
        foreach( $* e in o ) if( e.i == i ) return e.c;
        return -1;
    };

//    func (   @* set_size( mutable, sz_t size )) = { return x_array_set_size( o, size ); };
//    func (      @* clear( mutable ))            = { return x_array_clear( o ); };
//    func (:ci_s* push( mutable ))               = { return x_array_push( o ); };
//    func (:ci_s* push_d( mutable, :ci_s* v ))   = { return x_array_push_d( o, v ); };
//    func (:ci_s* push_c( mutable, :ci_s* v ))   = { return x_array_push_c( o, v ); };
};

/** Sets arguments from index data according to signature
  * Identifiers:
  *          entry      exit
  * a-pass: a,b,c,d,e    y
  * d-pass: f,g,h,i,j    z
  */
feature void set_arg( mutable, :ci_s* ci ) =
{
    sz_t size   = o.arity() + 1;
    sc_t  sig   = o.sig();
    sz_t* index = o.get_index_arr();
    for( sz_t i = 0; i < size; i++ )
    {
        u0_t c = sig[ i ];
        ASSERT( c );
        if( ci.c == c )
        {
            index[ i ] = ci.i;
            return;
        }
    }
};

feature :* set_args( mutable, const :arr_ci_s* arr ) =
{
    foreach( $* e in arr ) :a_set_arg( o, e );
    return o;
};

// ---------------------------------------------------------------------------------------------------------------------

/// nullary operators
group :ar0 =
{
    stamp :index_s = bcore_inst { sz_t [ 1 ] v; func bcore_inst_call . init_x = { o.v[0] = -1; }; };

    extending stump verbatim :_s = aware :
    {
        :index_s i;
        func :: .arity = { return 0; };
        func :: .get_index_arr = { return o.i.v; };
    };

    /// axon pass --------------------------------------------------------------

    stamp :zro_s = { func :: .run = { bhvm_hop_ar0_zro_s_f( ah[o->i.v[0]] ); }; func :: .sig = { return "y"; }; };
    stamp :one_s = { func :: .run = { bhvm_hop_ar0_one_s_f( ah[o->i.v[0]] ); }; func :: .sig = { return "y"; }; };

    stamp :determine_s = { func :: .run = { ah[o.i.v[0]].fit_size(); }; func :: .sig = { return "y"; }; };
    stamp :vacate_s    = { func :: .run = { ah[o.i.v[0]].v.clear(); };  func :: .sig = { return "y"; }; };

    stamp :randomize_s =
    {
        u3_t rseed   = 1234;
        f3_t min     = -0.5;
        f3_t max     =  0.5;
        f3_t density =  1.0;

        func :: .run =
        {
            sz_t i = o.i.v[0];
            u3_t rval = o.rseed + i;
            ah[i].v.set_random_u3( o.density, o.min, o.max, rval.1 );
        };

        func :: .sig = { return "y"; };
    };

    /// dendrite pass ----------------------------------------------------------

    stamp :nul_dp_s = { func :: .run = {}; func :: .sig = { return "f"; }; }; // no action

    /// state -------------------------------------------------------------------

    // State operators maintain an internal state, which can also change during use.

    /** Randomizer with internal seed state and updating seed at each use
      * The internal state is protected via mutex
      */
    stamp :rand_s =
    {
        aware bcore_prsg => prsg;
        u3_t rval    = 1234; // depracted, use prsg instead
        f3_t min     = -0.5;
        f3_t max     =  0.5;
        f3_t density =  1.0;

        hidden bcore_mutex_s mutex;

        func :: .run =
        {
            sz_t i = o.i.v[0];
            u3_t* rval = o.rval.cast( u3_t* );
            bcore_mutex_s* mutex = o.mutex.cast( bcore_mutex_s* );
            mutex.lock();
            if( o->prsg )
            {
                ah[ i ].v.set_random( o->density, o->min, o->max, o->prsg );
            }
            else
            {
                ah[ i ].v.set_random_u3( o->density, o->min, o->max, rval );
            }
            mutex.unlock();
        };

        func :: .sig = { return "y"; };
    };

};

// ---------------------------------------------------------------------------------------------------------------------

/// unary operators
group :ar1 =
{
    stamp :index_s = bcore_inst { sz_t [ 2 ] v; func bcore_inst_call . init_x = { o.v[0] = o.v[1] = -1; }; };

    extending stump verbatim :_s = aware :
    {
        :index_s i;
        func :: .arity = { return 1; };
        func :: .get_index_arr = { return o.i.v; };
    };

    /// axon pass --------------------------------------------------------------

    stamp :neg_s        = { func :: .run = { bhvm_hop_ar1_neg_s_f       ( ah[o.i.v[0]], ah[o.i.v[1]] ); }; func :: .sig = { return "ay"; }; };
    stamp :floor_s      = { func :: .run = { bhvm_hop_ar1_floor_s_f     ( ah[o.i.v[0]], ah[o.i.v[1]] ); }; func :: .sig = { return "ay"; }; };
    stamp :ceil_s       = { func :: .run = { bhvm_hop_ar1_ceil_s_f      ( ah[o.i.v[0]], ah[o.i.v[1]] ); }; func :: .sig = { return "ay"; }; };
    stamp :exp_s        = { func :: .run = { bhvm_hop_ar1_exp_s_f       ( ah[o.i.v[0]], ah[o.i.v[1]] ); }; func :: .sig = { return "ay"; }; };
    stamp :log_s        = { func :: .run = { bhvm_hop_ar1_log_s_f       ( ah[o.i.v[0]], ah[o.i.v[1]] ); }; func :: .sig = { return "ay"; }; };
    stamp :inv_s        = { func :: .run = { bhvm_hop_ar1_inv_s_f       ( ah[o.i.v[0]], ah[o.i.v[1]] ); }; func :: .sig = { return "ay"; }; };
    stamp :sqr_s        = { func :: .run = { bhvm_hop_ar1_sqr_s_f       ( ah[o.i.v[0]], ah[o.i.v[1]] ); }; func :: .sig = { return "ay"; }; };
    stamp :srt_s        = { func :: .run = { bhvm_hop_ar1_srt_s_f       ( ah[o.i.v[0]], ah[o.i.v[1]] ); }; func :: .sig = { return "ay"; }; };
    stamp :abs_s        = { func :: .run = { bhvm_hop_ar1_abs_s_f       ( ah[o.i.v[0]], ah[o.i.v[1]] ); }; func :: .sig = { return "ay"; }; };
    stamp :l1_s         = { func :: .run = { bhvm_hop_ar1_l1_s_f        ( ah[o.i.v[0]], ah[o.i.v[1]] ); }; func :: .sig = { return "ay"; }; };
    stamp :sigm_s       = { func :: .run = { bhvm_hop_ar1_sigm_s_f      ( ah[o.i.v[0]], ah[o.i.v[1]] ); }; func :: .sig = { return "ay"; }; };
    stamp :sigm_hard_s  = { func :: .run = { bhvm_hop_ar1_sigm_hard_s_f ( ah[o.i.v[0]], ah[o.i.v[1]] ); }; func :: .sig = { return "ay"; }; };
    stamp :sigm_leaky_s = { func :: .run = { bhvm_hop_ar1_sigm_leaky_s_f( ah[o.i.v[0]], ah[o.i.v[1]] ); }; func :: .sig = { return "ay"; }; };
    stamp :tanh_s       = { func :: .run = { bhvm_hop_ar1_tanh_s_f      ( ah[o.i.v[0]], ah[o.i.v[1]] ); }; func :: .sig = { return "ay"; }; };
    stamp :tanh_hard_s  = { func :: .run = { bhvm_hop_ar1_tanh_hard_s_f ( ah[o.i.v[0]], ah[o.i.v[1]] ); }; func :: .sig = { return "ay"; }; };
    stamp :tanh_leaky_s = { func :: .run = { bhvm_hop_ar1_tanh_leaky_s_f( ah[o.i.v[0]], ah[o.i.v[1]] ); }; func :: .sig = { return "ay"; }; };
    stamp :softplus_s   = { func :: .run = { bhvm_hop_ar1_softplus_s_f  ( ah[o.i.v[0]], ah[o.i.v[1]] ); }; func :: .sig = { return "ay"; }; };
    stamp :softmax_s    = { func :: .run = { bhvm_hop_ar1_softmax_s_f   ( ah[o.i.v[0]], ah[o.i.v[1]] ); }; func :: .sig = { return "ay"; }; };
    stamp :relu_s       = { func :: .run = { bhvm_hop_ar1_relu_s_f      ( ah[o.i.v[0]], ah[o.i.v[1]] ); }; func :: .sig = { return "ay"; }; };
    stamp :relu_leaky_s = { func :: .run = { bhvm_hop_ar1_relu_leaky_s_f( ah[o.i.v[0]], ah[o.i.v[1]] ); }; func :: .sig = { return "ay"; }; };

    stamp :order_inc_s =
    {
        sz_t dim;
        func :: .run = { ah[o->i.v[0]].order_inc( o.dim, ah[o.i.v[1]] ); };
        func :: .sig = { return "ay"; };
    };

    stamp :order_dec_s =
    {
        sz_t idx;
        func :: .run = { ah[o.i.v[0]].order_dec( o->idx, ah[o.i.v[1]] ); };
        func :: .sig = { return "ay"; };
    };

    stamp :order_dec_weak_s =
    {
        sz_t idx;
        func :: .run = { ah[o.i.v[0]].order_dec_weak( o->idx, ah[o.i.v[1]] ); };
        func :: .sig = { return "ay"; };
    };

    /// dendrite pass ----------------------------------------------------------

    stamp :add_dp_a_s = { func :: .run = { bhvm_hop_ar1_eci_cpy_acc_s_f ( ah[o.i.v[0]], ah[o.i.v[1]] ); }; func :: .sig = { return "zf"; }; };
    stamp :add_dp_b_s = { func :: .run = { bhvm_hop_ar1_eci_cpy_acc_s_f ( ah[o.i.v[0]], ah[o.i.v[1]] ); }; func :: .sig = { return "zg"; }; };
    stamp :sub_dp_a_s = { func :: .run = { bhvm_hop_ar1_eci_cpy_acc_s_f ( ah[o.i.v[0]], ah[o.i.v[1]] ); }; func :: .sig = { return "zf"; }; };
    stamp :sub_dp_b_s = { func :: .run = { bhvm_hop_ar1_eci_neg_acc_s_f ( ah[o.i.v[0]], ah[o.i.v[1]] ); }; func :: .sig = { return "zg"; }; };
    stamp :neg_dp_s    = { func :: .run = { bhvm_hop_ar1_eci_neg_acc_s_f( ah[o.i.v[0]], ah[o.i.v[1]] ); }; func :: .sig = { return "zf"; }; };
    stamp :cat_dp_a_s  = { func :: .run = { bhvm_hop_ar1_cat_dp_zf_s_f  ( ah[o.i.v[0]], ah[o.i.v[1]] ); }; func :: .sig = { return "zf"; }; };
    stamp :cat_dp_b_s  = { func :: .run = { bhvm_hop_ar1_cat_dp_zg_s_f  ( ah[o.i.v[0]], ah[o.i.v[1]] ); }; func :: .sig = { return "zg"; }; };
    stamp :ccat_dp_a_s = { func :: .run = { bhvm_hop_ar1_ccat_dp_zf_s_f ( ah[o.i.v[0]], ah[o.i.v[1]] ); }; func :: .sig = { return "zf"; }; };
    stamp :ccat_dp_b_s = { func :: .run = { bhvm_hop_ar1_ccat_dp_zg_s_f ( ah[o.i.v[0]], ah[o.i.v[1]] ); }; func :: .sig = { return "zg"; }; };


    /// copy special channel ---------------------------------------------------

    stamp :cpy_s =
    {
        func :: .sig = { return "ay"; };
        func :: .run = { bhvm_hop_ar1_cpy_s_f( ah[o.i.v[0]], ah[o.i.v[1]] ); };
    };

    /// accumulate: dendrite pass of copy
    stamp :acc_s =
    {
        func :: .sig = { return "zf"; };
        func :: .run = { bhvm_hop_ar1_eci_cpy_acc_s_f( ah[o.i.v[0]], ah[o.i.v[1]] ); };
    };

    /// cast -------------------------------------------------------------------

    // Cast operators provide 'fixed wiring'.
    // This is typically achieved by having the target reference source data.
    // Value data is always referenced; shape might be referenced.
    // Cast operators are inert as far as actual vm-processing is concerned.
    // They are not intended to actually execute in ap of dp tracks.
    // They are typically placed in the setup track.

    /// y references a (forking shape and value)
    stamp :fork_s =
    {
        func :: .sig = { return "ay"; };
        func :: .run =
        {
            bhvm_holor_s* a = ah[o.i.v[0]];
            bhvm_holor_s* y = ah[o.i.v[1]];
            y.fork_from( a );
        };
    };

    /// value of y references value of a; shape of y is set from internal shape parameter
    stamp :reshape_s =
    {
        bhvm_shape_s shape; // shape of y
        func :: .sig = { return "ay"; };
        func :: .run =
        {
            bhvm_holor_s* a = ah[o.i.v[0]];
            bhvm_holor_s* y = ah[o.i.v[1]];
            assert( o.shape.get_volume() ==  a.s.get_volume() );
            y.s.copy( o.shape );
            y.v.fork_from( a.v.1 );
        };
    };
};

// ---------------------------------------------------------------------------------------------------------------------

/// binary operators
group :ar2 =
{
    stamp :index_s = bcore_inst { sz_t [ 3 ] v; func bcore_inst_call . init_x = { o.v[0] = o.v[1] = o.v[2] = -1; }; };

    extending stump verbatim :_s = aware :
    {
        :index_s i;
        func :: .arity = { return 2; };
        func :: .get_index_arr = { return o.i.v; };
    };

    /// axon pass --------------------------------------------------------------

    stamp :cat_s  = { func :: .run = { bhvm_hop_ar2_cat_s_f ( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "aby"; }; };
    stamp :ccat_s = { func :: .run = { bhvm_hop_ar2_ccat_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "aby"; }; };

    stamp :add_s = { func :: .run = { bhvm_hop_ar2_eci_add_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "aby"; }; };
    stamp :sub_s = { func :: .run = { bhvm_hop_ar2_eci_sub_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "aby"; }; };
    stamp :mul_s = { func :: .run = { bhvm_hop_ar2_eci_mul_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "aby"; }; };
    stamp :div_s = { func :: .run = { bhvm_hop_ar2_eci_div_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "aby"; }; };
    stamp :pow_s = { func :: .run = { bhvm_hop_ar2_eci_pow_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "aby"; }; };

    stamp :mul_mvv_s = { func :: .run = { bhvm_hop_ar2_mul_mvv_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "aby"; }; };
    stamp :mul_vmv_s = { func :: .run = { bhvm_hop_ar2_mul_vmv_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "aby"; }; };
    stamp :mul_tvv_s = { func :: .run = { bhvm_hop_ar2_mul_tvv_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "aby"; }; };
    stamp :mul_vtv_s = { func :: .run = { bhvm_hop_ar2_mul_vtv_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "aby"; }; };
    stamp :mul_vvm_s = { func :: .run = { bhvm_hop_ar2_mul_vvm_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "aby"; }; };
    stamp :mul_mmm_s = { func :: .run = { bhvm_hop_ar2_mul_mmm_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "aby"; }; };
    stamp :mul_mtm_s = { func :: .run = { bhvm_hop_ar2_mul_mtm_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "aby"; }; };
    stamp :mul_tmm_s = { func :: .run = { bhvm_hop_ar2_mul_tmm_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "aby"; }; };
    stamp :mul_ttm_s = { func :: .run = { bhvm_hop_ar2_mul_ttm_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "aby"; }; };

    /// logic ------------------------------------------------------------------

    stamp :logic_equal_s         = { func :: .run = { bhvm_hop_ar2_eci_logic_equal_s_f        ( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "aby"; }; };
    stamp :logic_unequal_s       = { func :: .run = { bhvm_hop_ar2_eci_logic_unequal_s_f      ( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "aby"; }; };
    stamp :logic_larger_s        = { func :: .run = { bhvm_hop_ar2_eci_logic_larger_s_f       ( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "aby"; }; };
    stamp :logic_smaller_s       = { func :: .run = { bhvm_hop_ar2_eci_logic_smaller_s_f      ( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "aby"; }; };
    stamp :logic_larger_equal_s  = { func :: .run = { bhvm_hop_ar2_eci_logic_larger_equal_s_f ( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "aby"; }; };
    stamp :logic_smaller_equal_s = { func :: .run = { bhvm_hop_ar2_eci_logic_smaller_equal_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "aby"; }; };
    stamp :logic_and_s           = { func :: .run = { bhvm_hop_ar2_eci_logic_and_s_f          ( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "aby"; }; };
    stamp :logic_or_s            = { func :: .run = { bhvm_hop_ar2_eci_logic_or_s_f           ( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "aby"; }; };

    /// dendrite pass ----------------------------------------------------------

    stamp :mul_mvv_dp_a_s = { func :: .run = { bhvm_hop_ar2_mul_acc_vvm_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "zbf"; }; };
    stamp :mul_vmv_dp_a_s = { func :: .run = { bhvm_hop_ar2_mul_acc_vtv_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "zbf"; }; };
    stamp :mul_tvv_dp_a_s = { func :: .run = { bhvm_hop_ar2_mul_acc_vvm_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "bzf"; }; };
    stamp :mul_vtv_dp_a_s = { func :: .run = { bhvm_hop_ar2_mul_acc_vmv_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "zbf"; }; };
    stamp :mul_vvm_dp_a_s = { func :: .run = { bhvm_hop_ar2_mul_acc_mvv_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "zbf"; }; };

    stamp :mul_mvv_dp_b_s = { func :: .run = { bhvm_hop_ar2_mul_acc_tvv_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "azg"; }; };
    stamp :mul_vmv_dp_b_s = { func :: .run = { bhvm_hop_ar2_mul_acc_vvm_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "azg"; }; };
    stamp :mul_tvv_dp_b_s = { func :: .run = { bhvm_hop_ar2_mul_acc_mvv_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "azg"; }; };
    stamp :mul_vtv_dp_b_s = { func :: .run = { bhvm_hop_ar2_mul_acc_vvm_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "zag"; }; };
    stamp :mul_vvm_dp_b_s = { func :: .run = { bhvm_hop_ar2_mul_acc_vmv_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "azg"; }; };

    stamp :mul_mmm_dp_a_s = { func :: .run = { bhvm_hop_ar2_mul_acc_mtm_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "zbf"; }; };
    stamp :mul_mtm_dp_a_s = { func :: .run = { bhvm_hop_ar2_mul_acc_mmm_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "zbf"; }; };
    stamp :mul_tmm_dp_a_s = { func :: .run = { bhvm_hop_ar2_mul_acc_mtm_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "bzf"; }; };
    stamp :mul_ttm_dp_a_s = { func :: .run = { bhvm_hop_ar2_mul_acc_ttm_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "bzf"; }; };

    stamp :mul_mmm_dp_b_s = { func :: .run = { bhvm_hop_ar2_mul_acc_tmm_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "azg"; }; };
    stamp :mul_mtm_dp_b_s = { func :: .run = { bhvm_hop_ar2_mul_acc_tmm_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "zag"; }; };
    stamp :mul_tmm_dp_b_s = { func :: .run = { bhvm_hop_ar2_mul_acc_mmm_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "azg"; }; };
    stamp :mul_ttm_dp_b_s = { func :: .run = { bhvm_hop_ar2_mul_acc_ttm_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "zag"; }; };

    stamp :mul_dp_a_s     = { func :: .run = { bhvm_hop_ar2_eci_mul_acc_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "zbf"; }; };
    stamp :mul_dp_b_s     = { func :: .run = { bhvm_hop_ar2_eci_mul_acc_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "zag"; }; };
    stamp :div_dp_a_s     = { func :: .run = { bhvm_hop_ar2_eci_div_acc_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "zbf"; }; };

    stamp :abs_dp_s        = { func :: .run = { bhvm_hop_ar2_abs_dp_zaf_s_f(        ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "zaf"; }; };
    stamp :exp_dp_s        = { func :: .run = { bhvm_hop_ar2_exp_dp_zyf_s_f(        ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "zyf"; }; };
    stamp :log_dp_s        = { func :: .run = { bhvm_hop_ar2_log_dp_zaf_s_f(        ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "zaf"; }; };
    stamp :inv_dp_s        = { func :: .run = { bhvm_hop_ar2_inv_dp_zyf_s_f(        ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "zyf"; }; };
    stamp :sqr_dp_s        = { func :: .run = { bhvm_hop_ar2_sqr_dp_zaf_s_f(        ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "zaf"; }; };
    stamp :srt_dp_s        = { func :: .run = { bhvm_hop_ar2_srt_dp_zyf_s_f(        ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "zyf"; }; };
    stamp :sigm_dp_s       = { func :: .run = { bhvm_hop_ar2_sigm_dp_zyf_s_f(       ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "zyf"; }; };
    stamp :sigm_hard_dp_s  = { func :: .run = { bhvm_hop_ar2_sigm_hard_dp_zyf_s_f(  ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "zyf"; }; };
    stamp :sigm_leaky_dp_s = { func :: .run = { bhvm_hop_ar2_sigm_leaky_dp_zyf_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "zyf"; }; };
    stamp :tanh_dp_s       = { func :: .run = { bhvm_hop_ar2_tanh_dp_zyf_s_f(       ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "zyf"; }; };
    stamp :tanh_hard_dp_s  = { func :: .run = { bhvm_hop_ar2_tanh_hard_dp_zyf_s_f(  ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "zyf"; }; };
    stamp :tanh_leaky_dp_s = { func :: .run = { bhvm_hop_ar2_tanh_leaky_dp_zyf_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "zyf"; }; };
    stamp :softplus_dp_s   = { func :: .run = { bhvm_hop_ar2_softplus_dp_zyf_s_f(   ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "zyf"; }; };
    stamp :softmax_dp_s    = { func :: .run = { bhvm_hop_ar2_softmax_dp_zyf_s_f(    ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "zyf"; }; };
    stamp :relu_dp_s       = { func :: .run = { bhvm_hop_ar2_relu_dp_zyf_s_f(       ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "zyf"; }; };
    stamp :relu_leaky_dp_s = { func :: .run = { bhvm_hop_ar2_relu_leaky_dp_zyf_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "zyf"; }; };

    stamp :iff_dp_b_s = { func :: .run = { bhvm_hop_ar2_eci_iff_dp_b_azg_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "azg"; }; };
    stamp :iff_dp_c_s = { func :: .run = { bhvm_hop_ar2_eci_iff_dp_c_azh_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]] ); }; func :: .sig = { return "azh"; }; };
};

// ---------------------------------------------------------------------------------------------------------------------

/// ternary operators
group :ar3 =
{
    stamp :index_s = bcore_inst { sz_t [ 4 ] v; func bcore_inst_call . init_x = { o.v[0] = o.v[1] = o.v[2] = o.v[3] = -1; }; };

    extending stump verbatim :_s = aware :
    {
        :index_s i;
        func :: .arity = { return 3; };
        func :: .get_index_arr = { return o.i.v; };
    };

    /// axon pass --------------------------------------------------------------

    stamp :iff_s =
    {
        func :: .run = { bhvm_hop_ar3_eci_iff_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]], ah[o.i.v[3]] ); };
        func :: .sig = { return "abcy"; };
    };

    /// dendrite pass ----------------------------------------------------------

    stamp :div_dp_b_s =
    {
        func :: .run = { bhvm_hop_ar3_eci_div_dp_zabg_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]], ah[o.i.v[3]] ); };
        func :: .sig = { return "zabg"; };
    };

    stamp :pow_dp_a_s =
    {
        func :: .run = { bhvm_hop_ar3_eci_pow_dp_abzf_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]], ah[o.i.v[3]] ); };
        func :: .sig = { return "abzf"; };
    };

    stamp :pow_dp_b_s =
    {
        func :: .run = { bhvm_hop_ar3_eci_pow_dp_ayzg_s_f( ah[o.i.v[0]], ah[o.i.v[1]], ah[o.i.v[2]], ah[o.i.v[3]] ); };
        func :: .sig = { return "ayzg"; };
    };
};

// ---------------------------------------------------------------------------------------------------------------------


