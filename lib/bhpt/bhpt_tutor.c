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

#include "bhpt_tutor.h"

#ifdef TYPEOF_bhpt_tutor_stamp

/**********************************************************************************************************************/
/// bhpt_tutor_sine_random_s

// ---------------------------------------------------------------------------------------------------------------------

void bhpt_tutor_sine_random_s_reset( bhpt_tutor_sine_random_s* o )
{

}

// ---------------------------------------------------------------------------------------------------------------------

bhpt_adaptive* bhpt_tutor_sine_random_s_create_adaptive( const bhpt_tutor_sine_random_s* o, const bhpt_builder* src_builder )
{
    BLM_INIT();

    bhpt_builder* builder = BLM_A_PUSH( bhpt_builder_a_clone( src_builder ) );
    bhpt_builder_a_set_format_en( builder, bhvm_holor_s_set_type_vector_vacant( BLM_CREATE( bhvm_holor_s ), TYPEOF_f3_t, o->input_size ) );
    bhpt_builder_a_set_format_ex( builder, bhvm_holor_s_set_type_vector_vacant( BLM_CREATE( bhvm_holor_s ), TYPEOF_f3_t, 1             ) );

    BLM_RETURNV( bhpt_adaptive*, bhpt_builder_a_create_adaptive( builder ) );
}

// ---------------------------------------------------------------------------------------------------------------------

/// returns modified random variable
u2_t bhpt_tutor_sine_random_s_fetch_sample( const bhpt_tutor_sine_random_s* o, u2_t rval, bhvm_holor_s* en, bhvm_holor_s* ex )
{
    rval = bcore_xsg1_u2( rval );
    bl_t pos = ( o->rval_prime & 1 ) == 0; // pos vs neg sample
    rval = bcore_xsg1_u2( rval );

    ASSERT( en->v.size == o->input_size );
    ASSERT( ex->v.size == 1 );
    ASSERT( en->v.type == TYPEOF_f3_t );
    ASSERT( ex->v.type == TYPEOF_f3_t );

    f3_t omega = 1.0 * f3_pi() * f3_rnd_pos( &rval );
    f3_t amplitude = 4.0 * f3_rnd_pos( &rval );

    f3_t* en_data = ( f3_t* )en->v.data;
    f3_t* ex_data = ( f3_t* )ex->v.data;

    if( pos )
    {
        for( sz_t i = 0; i < o->input_size; i++ )
        {
            f3_t vp = sin( omega * i ) * amplitude;
            en_data[ i ] = vp;
        }
        ex_data[ 0 ] = o->pos_tgt;
    }
    else
    {
        f3_t rwalker = f3_rnd_sym( &rval );
        for( sz_t i = 0; i < o->input_size; i++ )
        {
            rwalker += f3_rnd_sym( &rval );
            f3_t vn = rwalker;
            en_data[ i ] = vn;
        }
        ex_data[ 0 ] = o->neg_tgt;
    }
    return rval;
}

//----------------------------------------------------------------------------------------------------------------------

void bhpt_tutor_sine_random_s_prime( bhpt_tutor_sine_random_s* o, bhpt_adaptive* adaptive )
{
    BLM_INIT();

    bhvm_holor_s* hx = bhpt_adaptive_a_get_format_en( adaptive, BLM_CREATE( bhvm_holor_s ) );
    bhvm_holor_s* hy = bhpt_adaptive_a_get_format_ex( adaptive, BLM_CREATE( bhvm_holor_s ) );
    bhvm_holor_s* hf = BLM_CLONE( bhvm_holor_s, hy );

    bhvm_holor_s_fit_size( hx );
    bhvm_holor_s_fit_size( hy );
    bhvm_holor_s_fit_size( hf );

    bcore_mutex_s_lock( &o->mutex );
    o->rval_prime = bhpt_tutor_sine_random_s_fetch_sample( o, o->rval_prime, hx, hy );
    bcore_mutex_s_unlock( &o->mutex );

    bhpt_adaptive_a_axon_pass( adaptive, hx, hf );
    bhvm_hop_ar2_eci_sub_s_f( hy, hf, hy );
    bhpt_adaptive_a_dendrite_pass( adaptive, hy, NULL );

    BLM_DOWN();
}

// ---------------------------------------------------------------------------------------------------------------------

void bhpt_tutor_sine_random_s_test( const bhpt_tutor_sine_random_s* o, const bhpt_adaptive* adaptive_src, sz_t verbosity, bcore_sink* log )
{
    BLM_INIT();
    bhpt_adaptive* adaptive = BLM_A_PUSH( bhpt_adaptive_a_clone( adaptive_src ) );
    bhvm_holor_s* hx = bhpt_adaptive_a_get_format_en( adaptive, BLM_CREATE( bhvm_holor_s ) );
    bhvm_holor_s* hy = bhpt_adaptive_a_get_format_ex( adaptive, BLM_CREATE( bhvm_holor_s ) );

    bhvm_holor_s_zro( bhvm_holor_s_fit_size( hx ) );
    bhvm_holor_s_zro( bhvm_holor_s_fit_size( hy ) );
    bhvm_holor_s* hf = BLM_CLONE( bhvm_holor_s, hy );
    bhvm_holor_s* hd = BLM_CLONE( bhvm_holor_s, hy ); // y - f
    bhvm_holor_s* sf = BLM_CLONE( bhvm_holor_s, hy );
    bhvm_holor_s* sy = BLM_CLONE( bhvm_holor_s, hy );

    f3_t sy2 = 0;
    f3_t sd2 = 0;

    u2_t rval = o->rval_test;

    BFOR_SIZE( i, o->test_size )
    {
        rval = bhpt_tutor_sine_random_s_fetch_sample( o, rval, hx, hy );
        bhpt_adaptive_a_axon_pass( adaptive, hx, hf );

        bhvm_hop_ar2_eci_sub_s_f( hy, hf, hd );

        bhvm_holor_s_acc( sf, hf ); // sf += hf
        bhvm_holor_s_acc( sy, hy ); // sy += hy

        sy2 += bhvm_holor_s_sum_sqr( hy );
        sd2 += bhvm_holor_s_sum_sqr( hd );
    }

    sz_t dimy = sy->v.size;
    sz_t n = ( o->test_size > 0 ) ? o->test_size * dimy : 1;

    f3_t e2y = bhvm_holor_s_sum_sqr(     sy )     / ( n * n );
    f3_t e2d = bhvm_holor_s_sub_sqr_sum( sy, sf ) / ( n * n );

    f3_t ey2 = sy2 / n;
    f3_t ed2 = sd2 / n;

    /// variances
    f3_t vy = ey2 - e2y; // variance of training data
    f3_t vd = ed2 - e2d; // variance of output difference

    f3_t error = f3_srt(  vd * f3_inv( vy ) );
    f3_t bias  = f3_srt( e2d * f3_inv( vy ) );

    if( verbosity > 0 )
    {
        bcore_sink_a_push_fa( log, "err = #<f3_t>, bias = #<f3_t>\n", error, bias );
    }

    BLM_DOWN();
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

#endif // TYPEOF_bhpt_tutor_stamp

vd_t bhpt_tutor_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bhpt_tutor" ) ) )
    {
        case TYPEOF_init1:
        {
        }
        break;

        case TYPEOF_get_quicktypes:
        {
        }
        break;

        case TYPEOF_selftest:
        {
        }
        break;

        case TYPEOF_plant:
        {
            bcore_plant_compile( "bhpt_planted", __FILE__ );
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

