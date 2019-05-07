/** Copyright 2019 Johannes Bernhard Steffens
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

#include "bmath_adaptive.h"

/**********************************************************************************************************************/

f3_t bmath_adaptive_a_query_1( bmath_adaptive* o, const bmath_vf3_s* in )
{
    f3_t out_scl = 0;
    bmath_vf3_s out = bmath_vf3_weak( &out_scl, 1 );
    bmath_adaptive_a_query( o, in, &out );
    return out_scl;
}

// ---------------------------------------------------------------------------------------------------------------------

f3_t bmath_adaptive_a_adapt_1( bmath_adaptive* o, const bmath_vf3_s* in, f3_t target )
{
    f3_t scl_out = 0;
    bmath_vf3_s vec_out    = bmath_vf3_weak( &scl_out, 1 );
    bmath_vf3_s vec_target = bmath_vf3_weak( &target, 1 );
    bmath_adaptive_a_adapt( o, in, &vec_target, &vec_out );
    return scl_out;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

void bmath_adaptive_a_test_sine_random( const bmath_adaptive* const_o )
{
    BCORE_LIFE_INIT();

    bmath_adaptive* o = BCORE_LIFE_A_PUSH( bcore_inst_a_clone( ( bcore_inst* )const_o ) );
    bmath_adaptive_a_set_in_size(  o, 32 );
    bmath_adaptive_a_set_out_size( o, 1 );
    bmath_adaptive_a_set_untrained( o );
    bmath_adaptive_a_setup( o, true ); // only for display
    bmath_adaptive_a_arc_to_sink( o, BCORE_STDOUT );

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

    sz_t epochs = 30;
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


    bcore_sink_buffer_s* sink_buf = BCORE_LIFE_A_PUSH( bcore_sink_buffer_s_create() );
    bcore_source_buffer_s* source_buf = BCORE_LIFE_A_PUSH( bcore_source_buffer_s_create() );
    bcore_bin_ml_a_to_sink( o, ( bcore_sink* )sink_buf );

    source_buf->data = sink_buf->data;
    source_buf->size = sink_buf->size;

    bmath_adaptive* o_tst = BCORE_LIFE_A_PUSH( bcore_inst_t_create( *( aware_t* )o ) );
    bcore_bin_ml_a_from_source( o_tst, ( bcore_source* )source_buf );

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

void bmath_adaptive_a_test_encode_add( const bmath_adaptive* const_o )
{
    BCORE_LIFE_INIT();

    sz_t in_bits = 6;
    sz_t out_bits = in_bits + 1;

    bmath_adaptive* o = BCORE_LIFE_A_PUSH( bcore_inst_a_clone( ( bcore_inst* )const_o ) );
    bmath_adaptive_a_set_in_size(  o, in_bits * 2 );
    bmath_adaptive_a_set_out_size( o, out_bits );
    bmath_adaptive_a_set_untrained( o );
    bmath_adaptive_a_setup( o, true ); // only for display
    bmath_adaptive_a_arc_to_sink( o, BCORE_STDOUT );

    BCORE_LIFE_CREATE( bcore_arr_sr_s, set_trn );
    BCORE_LIFE_CREATE( bcore_arr_sr_s, set_tst );
    BCORE_LIFE_CREATE( bmath_vf3_s, vec_inp );
    BCORE_LIFE_CREATE( bmath_vf3_s, vec_out );

    sz_t samples = 10000;
    u2_t rval = 123;
    sz_t epochs = 300;
    f3_t learn_step = 0.0003;
    f3_t decay = 0.0001 * learn_step;
    f3_t pos_tgt = 0.9;
    f3_t neg_tgt = -pos_tgt;

    bmath_vf3_s_set_size( vec_inp, bmath_adaptive_a_get_in_size(  o ) );
    bmath_vf3_s_set_size( vec_out, bmath_adaptive_a_get_out_size( o ) );

    for( sz_t i = 0; i < samples * 2; i++ )
    {
        u3_t v1 = ( rval = bcore_xsg_u2( rval ) ) & ( ( 1 << in_bits ) - 1 );
        u3_t v2 = ( rval = bcore_xsg_u2( rval ) ) & ( ( 1 << in_bits ) - 1 );
        u3_t vo = v1 + v2;

        for( sz_t i = 0; i < in_bits; i++ )
        {
            vec_inp->data[ i           ] = ( ( v1 & ( 1 << i ) ) != 0 ) ? 1.0 : -1.0;
            vec_inp->data[ i + in_bits ] = ( ( v2 & ( 1 << i ) ) != 0 ) ? 1.0 : -1.0;
        }

        for( sz_t i = 0; i < out_bits; i++ )
        {
            vec_out->data[ i ] = ( ( vo & ( 1 << i ) ) != 0 ) ? pos_tgt : neg_tgt;
        }


        if( ( i & 1 ) == 0 )
        {
            bcore_arr_sr_s_push_sr( set_trn, sr_asd( bmath_vf3_s_clone( vec_inp ) ) );
            bcore_arr_sr_s_push_sr( set_trn, sr_asd( bmath_vf3_s_clone( vec_out ) ) );
        }
        else
        {
            bcore_arr_sr_s_push_sr( set_tst, sr_asd( bmath_vf3_s_clone( vec_inp ) ) );
            bcore_arr_sr_s_push_sr( set_tst, sr_asd( bmath_vf3_s_clone( vec_out ) ) );
        }
    }

    bmath_adaptive_a_set_step(  o, learn_step );
    bmath_adaptive_a_set_decay( o, decay );

    for( sz_t i = 0; i < epochs; i++ )
    {
        f3_t err = 0;
        f3_t wgt = 0;
        for( sz_t j = 0; j < set_trn->size; j += 2 )
        {
            const bmath_vf3_s* trn_inp = set_trn->data[ j + 0 ].o;
            const bmath_vf3_s* trn_out = set_trn->data[ j + 1 ].o;

            bmath_adaptive_a_adapt( o, trn_inp, trn_out, vec_out );
            err += bmath_vf3_s_f3_sub_sqr( trn_out, vec_out );
            wgt += vec_out->size;
        }
        err = f3_srt( err / wgt );
        bcore_msg_fa( "#pl6 {#<sz_t>}: err = #<f3_t>\n", i, err );
    }

    bcore_bin_ml_a_to_file( o, "temp/adaptive.bin" );
    bmath_adaptive* o_tst = BCORE_LIFE_A_PUSH( bcore_inst_t_create( *( aware_t* )o ) );
    bcore_bin_ml_a_from_file( o_tst, "temp/adaptive.bin" );

    {
        f3_t err = 0;
        f3_t wgt = 0;
        for( sz_t j = 0; j < samples; j++ )
        {
            const bmath_vf3_s* trn_inp = set_tst->data[ j * 2 + 0 ].o;
            const bmath_vf3_s* trn_out = set_tst->data[ j * 2 + 1 ].o;
            bmath_adaptive_a_adapt( o, trn_inp, trn_out, vec_out );
            err += bmath_vf3_s_f3_sub_sqr( trn_out, vec_out );
            wgt += vec_out->size;
        }
        err = f3_srt( err / wgt );
        bcore_msg_fa( "tst_err = #<f3_t>\n", err );
    }
    BCORE_LIFE_RETURN();
}

/**********************************************************************************************************************/

vd_t bmath_adaptive_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_adaptive" ) ) )
    {
        case TYPEOF_init1:
        {
        }
        break;

        case TYPEOF_selftest:
        {
            return NULL;
        }
        break;

        case TYPEOF_precoder:
        {
            bcore_precoder_compile( "bmath_precoded", __FILE__ );
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

