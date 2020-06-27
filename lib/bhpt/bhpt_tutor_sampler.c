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

#include "bhpt_tutor_sampler.h"

#ifdef TYPEOF_bhpt_tutor_sampler

/**********************************************************************************************************************/
/// bhpt_sampler

u3_t bhpt_sampler_sine_random_s_fetch( const bhpt_sampler_sine_random_s* o, u3_t rval, bhvm_value_s* en, bhvm_value_s* ex )
{
    rval = bcore_lcg00_u3( rval );
    bl_t pos = ( rval & 1 ) == 0; // pos vs neg sample
    rval = bcore_lcg00_u3( rval );

    ASSERT( en->size == o->size_en );
    ASSERT( ex->size == 1 );
    ASSERT( en->type == TYPEOF_f3_t );
    ASSERT( ex->type == TYPEOF_f3_t );

    f3_t* en_data = ( f3_t* )en->data;
    f3_t* ex_data = ( f3_t* )ex->data;

    if( pos )
    {
        f3_t omega = 1.0 * f3_pi() * f3_rnd_pos( &rval );
        f3_t amplitude = 4.0 * f3_rnd_pos( &rval );

        for( sz_t i = 0; i < o->size_en; i++ )
        {
            f3_t vp = sin( omega * i ) * amplitude;
            en_data[ i ] = vp;
        }
        ex_data[ 0 ] = o->pos_tgt;
    }
    else
    {
        f3_t rwalker = f3_rnd_sym( &rval );
        for( sz_t i = 0; i < o->size_en; i++ )
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

u3_t bhpt_sampler_binary_add_s_fetch( const bhpt_sampler_binary_add_s* o, u3_t rval, bhvm_value_s* en, bhvm_value_s* ex )
{
    rval = bcore_lcg00_u3( rval );

    ASSERT( en->size == o->bits * 2 );
    ASSERT( ex->size == o->bits + 1 );
    ASSERT( en->type == TYPEOF_f3_t );
    ASSERT( ex->type == TYPEOF_f3_t );

    f3_t* en_data = ( f3_t* )en->data;
    f3_t* ex_data = ( f3_t* )ex->data;

    u3_t v1 = ( rval = bcore_lcg00_u3( rval ) ) & ( ( 1 << o->bits ) - 1 );
    u3_t v2 = ( rval = bcore_lcg00_u3( rval ) ) & ( ( 1 << o->bits ) - 1 );
    u3_t vo = v1 + v2;

    for( sz_t i = 0; i < o->bits; i++ )
    {
        en_data[ i           ] = ( ( v1 & ( 1 << i ) ) != 0 ) ? o->val_h : o->val_l;
        en_data[ i + o->bits ] = ( ( v2 & ( 1 << i ) ) != 0 ) ? o->val_h : o->val_l;
    }

    for( sz_t i = 0; i <= o->bits; i++ )
    {
        ex_data[ i ] = ( ( vo & ( 1 << i ) ) != 0 ) ? o->val_h : o->val_l;
    }

    return rval;
}

//----------------------------------------------------------------------------------------------------------------------

u3_t bhpt_sampler_binary_mul_s_fetch( const bhpt_sampler_binary_mul_s* o, u3_t rval, bhvm_value_s* en, bhvm_value_s* ex )
{
    rval = bcore_lcg00_u3( rval );

    ASSERT( en->size == o->bits * 2 );
    ASSERT( ex->size == o->bits * 2 );
    ASSERT( en->type == TYPEOF_f3_t );
    ASSERT( ex->type == TYPEOF_f3_t );

    f3_t* en_data = ( f3_t* )en->data;
    f3_t* ex_data = ( f3_t* )ex->data;

    u3_t v1 = ( rval = bcore_lcg00_u3( rval ) ) & ( ( 1 << o->bits ) - 1 );
    u3_t v2 = ( rval = bcore_lcg00_u3( rval ) ) & ( ( 1 << o->bits ) - 1 );
    u3_t vo = v1 * v2;

    for( sz_t i = 0; i < o->bits; i++ )
    {
        en_data[ i           ] = ( ( v1 & ( 1 << i ) ) != 0 ) ? o->val_h : o->val_l;
        en_data[ i + o->bits ] = ( ( v2 & ( 1 << i ) ) != 0 ) ? o->val_h : o->val_l;
    }

    for( sz_t i = 0; i < ex->size; i++ )
    {
        ex_data[ i ] = ( ( vo & ( 1 << i ) ) != 0 ) ? o->val_h : o->val_l;
    }

    return rval;
}

//----------------------------------------------------------------------------------------------------------------------

u3_t bhpt_sampler_binary_lcg00_s_fetch( const bhpt_sampler_binary_lcg00_s* o, u3_t rval, bhvm_value_s* en, bhvm_value_s* ex )
{
    rval = bcore_lcg00_u3( rval );

    ASSERT( en->size == o->bits );
    ASSERT( ex->size == o->bits );
    ASSERT( en->type == TYPEOF_f3_t );
    ASSERT( ex->type == TYPEOF_f3_t );

    f3_t* en_data = ( f3_t* )en->data;
    f3_t* ex_data = ( f3_t* )ex->data;

    u3_t vi = ( rval = bcore_lcg00_u3( rval ) ) & ( ( 1 << o->bits ) - 1 );
    u3_t vo = bcore_lcg00_u3( vi ) & ( ( 1 << o->bits ) - 1 );

    for( sz_t i = 0; i < en->size; i++ )
    {
        en_data[ i ] = ( ( vi & ( 1 << i ) ) != 0 ) ? o->val_h : o->val_l;
    }

    for( sz_t i = 0; i < ex->size; i++ )
    {
        ex_data[ i ] = ( ( vo & ( 1 << i ) ) != 0 ) ? o->val_h : o->val_l;
    }

    return rval;
}

//----------------------------------------------------------------------------------------------------------------------

u3_t bhpt_sampler_binary_hash_s_fetch( const bhpt_sampler_binary_hash_s* o, u3_t rval, bhvm_value_s* en, bhvm_value_s* ex )
{
    rval = bcore_lcg00_u3( rval );

    ASSERT( en->size == o->bits );
    ASSERT( ex->size == o->bits );
    ASSERT( en->type == TYPEOF_f3_t );
    ASSERT( ex->type == TYPEOF_f3_t );

    f3_t* en_data = ( f3_t* )en->data;
    f3_t* ex_data = ( f3_t* )ex->data;

    tp_t vi = ( rval = bcore_lcg00_u3( rval ) ) & ( ( 1 << o->bits ) - 1 );
    tp_t vo = bcore_tp_fold_tp( bcore_tp_init(), vi ) & ( ( 1 << o->bits ) - 1 );

    if( o->reverse ) tp_t_swap( &vi, &vo );

    for( sz_t i = 0; i < en->size; i++ )
    {
        en_data[ i ] = ( ( vi & ( 1 << i ) ) != 0 ) ? o->val_h : o->val_l;
    }

    for( sz_t i = 0; i < ex->size; i++ )
    {
        ex_data[ i ] = ( ( vo & ( 1 << i ) ) != 0 ) ? o->val_h : o->val_l;
    }

    return rval;
}

//----------------------------------------------------------------------------------------------------------------------

u3_t bhpt_sampler_polynom_s_fetch( const bhpt_sampler_polynom_s* o, u3_t rval, bhvm_value_s* en, bhvm_value_s* ex )
{
    rval = bcore_lcg00_u3( rval );

    ASSERT( en->size == o->size_en );
    ASSERT( ex->size == o->size_ex );
    ASSERT( en->type == TYPEOF_f3_t );
    ASSERT( ex->type == TYPEOF_f3_t );

    f3_t* en_data = ( f3_t* )en->data;
    f3_t* ex_data = ( f3_t* )ex->data;

    for( sz_t i = 0; i < o->size_ex; i++ ) ex_data[ i ] = f3_rnd_sym( &rval );
    for( sz_t i = 0; i < o->size_en; i++ )
    {
        f3_t x1 = 2.0 * ( ( ( f3_t )i / ( o->size_en - 1 ) ) - 0.5 );
        f3_t x = 1.0;
        f3_t y = 0;
        for( sz_t i = 0; i < o->size_ex; i++ )
        {
            y += x * ex_data[ i ] * o->range;
            x *= x1;
        }

        en_data[ i ] = y + o->noise_level * f3_rnd_sym( &rval );
    }

    return rval;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// bhpt_tutor_sampler_s

// ---------------------------------------------------------------------------------------------------------------------

bhpt_adaptive* bhpt_tutor_sampler_s_create_adaptive( const bhpt_tutor_sampler_s* o )
{
    BLM_INIT();
    bhpt_builder* builder = BLM_A_PUSH( bhpt_builder_a_clone( o->builder ) );
    bhpt_builder_a_set_format_en( builder, bhvm_holor_s_set_type_vector_vacant( BLM_CREATE( bhvm_holor_s ), TYPEOF_f3_t, bhpt_sampler_a_get_size_en( o->sampler ) ) );
    bhpt_builder_a_set_format_ex( builder, bhvm_holor_s_set_type_vector_vacant( BLM_CREATE( bhvm_holor_s ), TYPEOF_f3_t, bhpt_sampler_a_get_size_ex( o->sampler ) ) );
    BLM_RETURNV( bhpt_adaptive*, bhpt_builder_a_create_adaptive( builder ) );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhpt_tutor_sampler_s_prime( bhpt_tutor_sampler_s* o, bhpt_adaptive* adaptive )
{
    BLM_INIT();

    bhvm_holor_s* hx = bhpt_adaptive_a_get_format_en( adaptive, BLM_CREATE( bhvm_holor_s ) );
    bhvm_holor_s* hy = bhpt_adaptive_a_get_format_ex( adaptive, BLM_CREATE( bhvm_holor_s ) );
    bhvm_holor_s* hf = BLM_CLONE( bhvm_holor_s, hy );

    bhvm_holor_s_fit_size( hx );
    bhvm_holor_s_fit_size( hy );
    bhvm_holor_s_fit_size( hf );

    bcore_mutex_s_lock( &o->mutex );
    o->rval_prime = bhpt_sampler_a_fetch( o->sampler, o->rval_prime, &hx->v, &hy->v );
    bcore_mutex_s_unlock( &o->mutex );

    bhpt_adaptive_a_axon_pass( adaptive, hx, hf );
    bhvm_hop_ar2_eci_sub_s_f( hy, hf, hy );
    bhpt_adaptive_a_dendrite_pass( adaptive, hy, NULL );

    BLM_DOWN();
}

// ---------------------------------------------------------------------------------------------------------------------

void bhpt_tutor_sampler_s_test( bhpt_tutor_sampler_s* o, const bhpt_adaptive* adaptive_src, sz_t verbosity, bcore_sink* log )
{
    BLM_INIT();
    bhpt_adaptive* adaptive = BLM_A_PUSH( bhpt_adaptive_a_clone( adaptive_src ) );
    bhvm_holor_s* hx = bhpt_adaptive_a_get_format_en( adaptive, BLM_CREATE( bhvm_holor_s ) );
    bhvm_holor_s* hy = bhpt_adaptive_a_get_format_ex( adaptive, BLM_CREATE( bhvm_holor_s ) );

    bhvm_holor_s_zro( bhvm_holor_s_fit_size( hx ) );
    bhvm_holor_s_zro( bhvm_holor_s_fit_size( hy ) );
    bhvm_holor_s* hf  = BLM_CLONE( bhvm_holor_s, hy );
    bhvm_holor_s* hd  = BLM_CLONE( bhvm_holor_s, hy ); // y - f
    bhvm_holor_s* hsd = BLM_CLONE( bhvm_holor_s, hy ); // y - f

    sz_t dimy = hy->v.size;

    f3_t sy  = 0;
    f3_t sd  = 0;

    f3_t sy2 = 0;
    f3_t sd2 = 0;

    u3_t rval = o->rval_test;

    BFOR_SIZE( i, o->test_size )
    {
        rval = bhpt_sampler_a_fetch( o->sampler, rval, &hx->v, &hy->v );
        bhpt_adaptive_a_axon_pass( adaptive, hx, hf );
        bhvm_hop_ar2_eci_sub_s_f( hy, hf, hd );

        sy  += bhvm_holor_s_sum( hy );
        sd  += bhvm_holor_s_sum( hd );
        sy2 += bhvm_holor_s_sum_sqr( hy );
        sd2 += bhvm_holor_s_sum_sqr( hd );

        bhvm_holor_s_acc( hsd, hd );
    }

    sz_t n = ( o->test_size > 0 ) ? o->test_size * dimy : 1;

    f3_t ey = sy / n;
    f3_t ed = sd / n;

    f3_t ey2 = sy2 / n;
    f3_t ed2 = sd2 / n;

    /// variances
    f3_t vy = ey2 - f3_sqr( ey ); // variance of training data
    f3_t vd = ed2 - f3_sqr( ed ); // variance of output difference

    f3_t error = f3_srt(  vd * f3_inv( vy ) );

    f3_t eb2 = bhvm_holor_s_sum_sqr( hsd ) / ( n * n );

    f3_t bias  = f3_srt( eb2 * f3_inv( vy ) );

    if( verbosity > 0 )
    {
        bcore_sink_a_pushf( log, "err: %5.3f, bias: %7.5f", error, bias );
    }

    BLM_DOWN();
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

#endif // TYPEOF_bhpt_tutor_sampler

vd_t bhpt_tutor_sampler_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bhpt_tutor_sampler" ) ) )
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

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

