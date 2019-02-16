/** Copyright 2018 Johannes Bernhard Steffens
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

#include "bmath_spect_adaptive.h"
#include "bmath_quicktypes.h"

/**********************************************************************************************************************/

BCORE_DEFINE_SPECT( bcore_inst, bmath_adaptive )
"{"
    "bcore_spect_header_s header;"
    "feature bmath_fp_adaptive: get_in_size;"
    "feature bmath_fp_adaptive: set_in_size;"
    "feature bmath_fp_adaptive: get_out_size;"
    "feature bmath_fp_adaptive: set_out_size;"
    "feature bmath_fp_adaptive: get_step;"
    "feature bmath_fp_adaptive: set_step;"
    "feature bmath_fp_adaptive: get_decay;"
    "feature bmath_fp_adaptive: set_decay;"
    "feature bmath_fp_adaptive: set_untrained;"
    "feature bmath_fp_adaptive: arc_to_sink;"
    "feature bmath_fp_adaptive: query;"
    "feature bmath_fp_adaptive: adapt;"
"}";

// ---------------------------------------------------------------------------------------------------------------------

f3_t bmath_adaptive_default_get_step( const bmath_adaptive_s* p, const bmath_adaptive* o )
{
    return 0;
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_adaptive_default_set_step( const bmath_adaptive_s* p, bmath_adaptive* o, f3_t val )
{
    /* nothing to do */
}

// ---------------------------------------------------------------------------------------------------------------------

f3_t bmath_adaptive_default_get_decay( const bmath_adaptive_s* p, const bmath_adaptive* o )
{
    return 0;
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_adaptive_default_set_decay( const bmath_adaptive_s* p, bmath_adaptive* o, f3_t val )
{
    /* nothing to do */
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_adaptive_default_arc_to_sink( const bmath_adaptive_s* p, const bmath_adaptive* o, bcore_sink* sink )
{
    bcore_txt_ml_a_to_sink( o, sink );
}

// ---------------------------------------------------------------------------------------------------------------------

f3_t bmath_adaptive_default_query_1( const bmath_adaptive_s* p, const bmath_adaptive* o, const bmath_vf3_s* in )
{
    f3_t out_scl = 0;
    bmath_vf3_s out = bmath_vf3_weak( &out_scl, 1 );
    bmath_adaptive_p_query( p, o, in, &out );
    return out_scl;
}

// ---------------------------------------------------------------------------------------------------------------------

f3_t bmath_adaptive_default_adapt_1( const bmath_adaptive_s* p, bmath_adaptive* o, const bmath_vf3_s* in, f3_t target )
{
    f3_t scl_out = 0;
    bmath_vf3_s vec_out    = bmath_vf3_weak( &scl_out, 1 );
    bmath_vf3_s vec_target = bmath_vf3_weak( &target, 1 );
    bmath_adaptive_p_adapt( p, o, in, &vec_target, &vec_out );
    return scl_out;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

void bmath_adaptive_a_test( const bmath_adaptive* const_o )
{
    BCORE_LIFE_INIT();

    bmath_adaptive* o = BCORE_LIFE_A_PUSH( bcore_inst_a_clone( ( bcore_inst* )const_o ) );
    bmath_adaptive_a_set_in_size(  o, 32 );
    bmath_adaptive_a_set_out_size( o, 1 );
    bmath_adaptive_a_set_untrained( o );
    bmath_adaptive_a_arc_to_sink(  o, BCORE_STDOUT );

    /* Learn differentiating between a sine wave of arbitrary amplitude and frequency from
       a random walk curve.
    */
    BCORE_LIFE_CREATE( bcore_arr_sr_s, pos_set_trn );
    BCORE_LIFE_CREATE( bcore_arr_sr_s, neg_set_trn );
    BCORE_LIFE_CREATE( bcore_arr_sr_s, pos_set_tst );
    BCORE_LIFE_CREATE( bcore_arr_sr_s, neg_set_tst );

    sz_t samples = 10000;
    u2_t rval = 123;
    for( sz_t i = 0; i < samples * 2; i++ )
    {
        sz_t input_size = bmath_adaptive_a_get_in_size( o );
        bmath_vf3_s* pos_vec = bmath_vf3_s_create();
        bmath_vf3_s* neg_vec = bmath_vf3_s_create();
        bmath_vf3_s_set_size( pos_vec, input_size );
        bmath_vf3_s_set_size( neg_vec, input_size );

        f3_t omega = 1.0 * f3_pi() * f3_rnd_pos( &rval );
        f3_t amplitude = 4.0 * f3_rnd_pos( &rval );

        f3_t rwalker = f3_rnd_sym( &rval );

        for( sz_t i = 0; i < input_size; i++ )
        {
            rwalker += f3_rnd_sym( &rval );
            f3_t vp = sin( omega * i ) * amplitude;
            f3_t vr = rwalker;
            f3_t vn = vr;

            pos_vec->data[ i ] = vp;
            neg_vec->data[ i ] = vn;
        }

        if( ( i & 1 ) == 0 )
        {
            bcore_arr_sr_s_push_sr( pos_set_trn, sr_asd( pos_vec ) );
            bcore_arr_sr_s_push_sr( neg_set_trn, sr_asd( neg_vec ) );
        }
        else
        {
            bcore_arr_sr_s_push_sr( pos_set_tst, sr_asd( pos_vec ) );
            bcore_arr_sr_s_push_sr( neg_set_tst, sr_asd( neg_vec ) );
        }
    }

    sz_t epochs = 100;
    f3_t learn_step = 0.0001;
    f3_t decay = 0.0001 * learn_step;
    f3_t pos_tgt = 0.9;
    f3_t neg_tgt = -pos_tgt;

    bmath_adaptive_a_set_step(  o, learn_step );
    bmath_adaptive_a_set_decay( o, decay );

    for( sz_t i = 0; i < epochs; i++ )
    {
        f3_t err = 0;
        for( sz_t j = 0; j < samples; j++ )
        {
            const bmath_vf3_s* pos_vec = pos_set_trn->data[ j ].o;
            const bmath_vf3_s* neg_vec = neg_set_trn->data[ j ].o;
            f3_t pos_est = bmath_adaptive_a_adapt_1( o, pos_vec, pos_tgt );
            f3_t neg_est = bmath_adaptive_a_adapt_1( o, neg_vec, neg_tgt );
            err += f3_sqr( pos_est - pos_tgt );
            err += f3_sqr( neg_est - neg_tgt );
        }

        err = f3_srt( err / ( samples * 2 ) );

        bcore_msg_fa( "#pl6 {#<sz_t>}: err = #<f3_t>\n", i, err );
    }

    bcore_bin_ml_a_to_file( o, "temp/adaptive.bin" );
    bmath_adaptive* o_tst = BCORE_LIFE_A_PUSH( bcore_inst_t_create( *( aware_t* )o ) );
    bcore_bin_ml_a_from_file( o_tst, "temp/adaptive.bin" );

    {
        f3_t err = 0;
        for( sz_t j = 0; j < samples; j++ )
        {
            const bmath_vf3_s* pos_vec = pos_set_tst->data[ j ].o;
            const bmath_vf3_s* neg_vec = neg_set_tst->data[ j ].o;
            f3_t pos_est = bmath_adaptive_a_query_1( o_tst, pos_vec );
            f3_t neg_est = bmath_adaptive_a_query_1( o_tst, neg_vec );
            err += f3_sqr( pos_est - pos_tgt );
            err += f3_sqr( neg_est - neg_tgt );
        }

        err = f3_srt( err / ( samples * 2 ) );

        bcore_msg_fa( "tst_err = #<f3_t>\n", err );
    }
    BCORE_LIFE_RETURN();
}

/**********************************************************************************************************************/

vd_t bmath_spect_adaptive_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_spect_adaptive" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_FEATURE( bmath_fp_adaptive_get_in_size );
            BCORE_REGISTER_FEATURE( bmath_fp_adaptive_set_in_size );
            BCORE_REGISTER_FEATURE( bmath_fp_adaptive_get_out_size );
            BCORE_REGISTER_FEATURE( bmath_fp_adaptive_set_out_size );
            BCORE_REGISTER_FEATURE( bmath_fp_adaptive_get_step );
            BCORE_REGISTER_FEATURE( bmath_fp_adaptive_set_step );
            BCORE_REGISTER_FEATURE( bmath_fp_adaptive_get_decay );
            BCORE_REGISTER_FEATURE( bmath_fp_adaptive_set_decay );
            BCORE_REGISTER_FEATURE( bmath_fp_adaptive_set_untrained );
            BCORE_REGISTER_FEATURE( bmath_fp_adaptive_arc_to_sink );
            BCORE_REGISTER_FEATURE( bmath_fp_adaptive_query );
            BCORE_REGISTER_FEATURE( bmath_fp_adaptive_adapt );
            BCORE_REGISTER_SPECT( bmath_adaptive );
        }
        break;

        case TYPEOF_get_quicktypes:
        {
            BCORE_REGISTER_QUICKTYPE( bmath_adaptive );
            BCORE_REGISTER_QUICKTYPE( bmath_adaptive_s );
        }
        break;

        case TYPEOF_selftest:
        {
            return NULL;
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

