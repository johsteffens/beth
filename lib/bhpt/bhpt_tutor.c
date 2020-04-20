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

    bhvm_holor_s* holor_en = BLM_CREATE( bhvm_holor_s );
    bhvm_holor_s* holor_ex = BLM_CREATE( bhvm_holor_s );

    bhvm_holor_s_set_type_vector_vacant( holor_en, TYPEOF_f3_t, o->input_size );
    bhvm_holor_s_set_type_scalar_vacant( holor_ex, TYPEOF_f3_t );

    bhpt_builder* builder = BLM_A_PUSH( bhpt_builder_a_clone( src_builder ) );
    bhpt_builder_a_set_holor_en( builder, holor_en );
    bhpt_builder_a_set_holor_ex( builder, holor_ex );

    BLM_RETURNV( bhpt_adaptive*, bhpt_builder_a_create_adaptive( builder ) );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhpt_tutor_sine_random_s_fetch_sample( const bhpt_tutor_sine_random_s* o, u2_t* rval, bhvm_holor_s* en, bhvm_holor_s* ex )
{
    *rval = bcore_xsg1_u2( *rval );
    bl_t pos = ( o->rval_prime & 1 ) == 0; // pos vs neg sample
    *rval = bcore_xsg1_u2( *rval );

    bhvm_value_s_set_size( &en->v, o->input_size );
    bhvm_value_s_set_size( &ex->v, 1 );

    f3_t omega = 1.0 * f3_pi() * f3_rnd_pos( rval );
    f3_t amplitude = 4.0 * f3_rnd_pos( rval );

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
        f3_t rwalker = f3_rnd_sym( rval );
        for( sz_t i = 0; i < o->input_size; i++ )
        {
            rwalker += f3_rnd_sym( rval );
            f3_t vn = rwalker;
            en_data[ i ] = vn;
        }
        ex_data[ 0 ] = o->neg_tgt;
    }
}

//----------------------------------------------------------------------------------------------------------------------

void bhpt_tutor_sine_random_s_prime( bhpt_tutor_sine_random_s* o, bhpt_adaptive* adaptive )
{
    BLM_INIT();

    bhvm_holor_s* hen = BLM_CREATE( bhvm_holor_s );
    bhvm_holor_s* hex = BLM_CREATE( bhvm_holor_s );

    bhpt_adaptive_a_get_holor_en( adaptive, hen );
    bhpt_adaptive_a_get_holor_ex( adaptive, hex );

    bhvm_holor_s* buf = BLM_CLONE( bhvm_holor_s, hex );

    bcore_mutex_s_lock( &o->mutex );
    bhpt_tutor_sine_random_s_fetch_sample( o, &o->rval_prime, hen, hex );
    bcore_mutex_s_unlock( &o->mutex );

    bhpt_adaptive_a_axon_pass( adaptive, hen, buf );
    bhvm_hop_ar2_eci_sub_s_f( hex, buf, buf );
    bhpt_adaptive_a_dendrite_pass( adaptive, buf, NULL );

    BLM_DOWN();
}

// ---------------------------------------------------------------------------------------------------------------------

void bhpt_tutor_sine_random_s_test( const bhpt_tutor_sine_random_s* o, const bhpt_adaptive* adaptive, sz_t verbosity, bcore_sink* log )
{

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

