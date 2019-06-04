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

#include "badapt_activator.h"

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

badapt_activator* badapt_activator_create_from_types( tp_t tp_activator, tp_t tp_activation )
{
    badapt_activator*  activator  = bcore_inst_t_create( tp_activator );
    badapt_activation* activation = bcore_inst_t_create( tp_activation );
    badapt_activator_a_set_activation( activator, activation );
    bcore_inst_a_discard( ( bcore_inst* )activation );
    return activator;
}

//----------------------------------------------------------------------------------------------------------------------

badapt_activator* badapt_activator_create_from_names( sc_t sc_activator, sc_t sc_activation )
{
    BCORE_LIFE_INIT();
    tp_t tp_activator  = typeof( ( ( st_s* )BCORE_LIFE_A_PUSH( st_s_create_fa( "badapt_activator_#<sc_t>_s", sc_activator   ) ) )->sc );
    tp_t tp_activation = typeof( ( ( st_s* )BCORE_LIFE_A_PUSH( st_s_create_fa( "badapt_activation_#<sc_t>_s", sc_activation ) ) )->sc );
    badapt_activator* activator = badapt_activator_create_from_types( tp_activator, tp_activation );
    BCORE_LIFE_RETURN( activator );
}

//----------------------------------------------------------------------------------------------------------------------

badapt_layer_activator_s* badapt_layer_activator_s_create_from_types( sz_t layer, tp_t tp_activator, tp_t tp_activation )
{
    badapt_layer_activator_s* o = badapt_layer_activator_s_create();
    o->layer = layer;
    o->activator = badapt_activator_create_from_types( tp_activator, tp_activation );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

void badapt_arr_layer_activator_s_push_from_types( badapt_arr_layer_activator_s* o, sz_t layer, tp_t tp_activator, tp_t tp_activation )
{
    badapt_arr_layer_activator_s_push_d( o, badapt_layer_activator_s_create_from_types( layer, tp_activator, tp_activation ) );
}

//----------------------------------------------------------------------------------------------------------------------

void badapt_arr_layer_activator_s_push_from_names( badapt_arr_layer_activator_s* o, sz_t layer, sc_t activator, sc_t activation )
{
    BCORE_LIFE_INIT();
    tp_t tp_activator  = typeof( ( ( st_s* )BCORE_LIFE_A_PUSH( st_s_create_fa( "badapt_activator_#<sc_t>_s", activator   ) ) )->sc );
    tp_t tp_activation = typeof( ( ( st_s* )BCORE_LIFE_A_PUSH( st_s_create_fa( "badapt_activation_#<sc_t>_s", activation ) ) )->sc );
    badapt_arr_layer_activator_s_push_from_types( o, layer, tp_activator, tp_activation );
    BCORE_LIFE_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

void badapt_arr_activator_s_setup_from_arr_layer_activator( badapt_arr_activator_s* o, const badapt_arr_layer_activator_s* arg_arr, sz_t layers )
{
    BCORE_LIFE_INIT();

    badapt_arr_layer_activator_s* arr = BCORE_LIFE_A_PUSH( badapt_arr_layer_activator_s_clone( arg_arr ) );
    bcore_array_a_sort( ( bcore_array* )arr, 0, -1, 1 );
    badapt_arr_activator_s_set_size( o, layers );

    sz_t last_layer = 0;

    for( sz_t i = 0; i < arr->arr_size; i++ )
    {
        sz_t layer = arr->arr_data[ i ].layer;
        const badapt_activator* activator = arr->arr_data[ i ].activator;
        if( layer < 0 )
        {
            for( sz_t j = last_layer + 1; j < layer; j++ )
            {
                if( layers + j < o->arr_size ) badapt_activator_a_replicate( &o->arr_data[ layers + j ], o->arr_data[ last_layer < 0 ? layers + last_layer : last_layer ] );
            }
            if( layers + layer < o->arr_size )
            {
                badapt_activator_a_replicate( &o->arr_data[ layers + layer ], activator );
                last_layer = layer;
            }
        }
        else
        {
            for( sz_t j = last_layer + 1; j < layer; j++ )
            {
                if( j < o->arr_size )  badapt_activator_a_replicate( &o->arr_data[ j ], o->arr_data[ last_layer < 0 ? layers + last_layer : last_layer ] );
            }
            if( layer < o->arr_size )
            {
                badapt_activator_a_replicate( &o->arr_data[ layer ], activator );
                last_layer = layer;
            }
        }
    }

    // fill gaps
    for( sz_t i = 0; i < layers; i++ )
    {
        if( !o->arr_data[ i ] && ( i > 0 ) )
        {
            o->arr_data[ i ] = badapt_activator_a_clone( o->arr_data[ i - 1 ] );
        }

        if( !o->arr_data[ i ] ) ERR_fa( "Layer #<sz_t> could not be initialized.", i );
    }

    BCORE_LIFE_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

const badapt_activator* badapt_arr_layer_activator_s_get_activator( const badapt_arr_layer_activator_s* o, sz_t index, sz_t layers )
{
    sz_t best_layer_index = -1;
    sz_t best_arr_index = -1;
    for( sz_t i = 0; i < o->arr_size; i++ )
    {
        const badapt_layer_activator_s* layer_activator = &o->arr_data[ i ];
        sz_t layer_index = layer_activator->layer < 0 ? layers + layer_activator->layer : layer_activator->layer;
        if( index >= layer_index && layer_index > best_layer_index )
        {
            best_layer_index = layer_index;
            best_arr_index = i;
        }
    }

    if( best_arr_index == -1 ) ERR_fa( "No activator found for index '#<sz_t>' of '#<sz_t>' layers.", index, layers );

    return o->arr_data[ best_arr_index ].activator;
}

//----------------------------------------------------------------------------------------------------------------------

badapt_activator_plain_s* badapt_activator_plain_s_create_activation( const badapt_activation* activation )
{
    badapt_activator_plain_s* o = badapt_activator_plain_s_create();
    badapt_activation_a_replicate( &o->activation, activation );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

void badapt_activator_plain_s_setup( badapt_activator_plain_s* o )
{
}

//----------------------------------------------------------------------------------------------------------------------

void badapt_activator_plain_s_reset( badapt_activator_plain_s* o )
{
   // nothing to do
}

//----------------------------------------------------------------------------------------------------------------------

void badapt_activator_plain_s_infer( const badapt_activator_plain_s* o, const bmath_vf3_s* in, bmath_vf3_s* out )
{
    assert( in->size == out->size );
    const badapt_activation_s* activation_p = badapt_activation_s_get_aware( o->activation );
    for( sz_t i = 0; i < out->size; i++ ) out->data[ i ] = badapt_activation_p_fx( activation_p, o->activation, in->data[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

void badapt_activator_plain_s_bgrad( const badapt_activator_plain_s* o, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out, const bmath_vf3_s* out )
{
    assert( grad_in->size == grad_out->size );
    assert( grad_in->size ==      out->size );
    const badapt_activation_s* activation_p = badapt_activation_s_get_aware( o->activation );
    for( sz_t i = 0; i < out->size; i++ ) grad_in->data[ i ] = badapt_activation_p_dy( activation_p, o->activation, out->data[ i ] ) * grad_out->data[ i ];
}

//----------------------------------------------------------------------------------------------------------------------

void badapt_activator_plain_s_adapt( badapt_activator_plain_s* o, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out, const bmath_vf3_s* out, f3_t epsilon )
{
    badapt_activator_plain_s_bgrad( o, grad_in, grad_out, out );
}

//----------------------------------------------------------------------------------------------------------------------

void badapt_activator_plain_s_adapt_defer( badapt_activator_plain_s* o, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out, const bmath_vf3_s* out )
{
    badapt_activator_plain_s_bgrad( o, grad_in, grad_out, out );
}

//----------------------------------------------------------------------------------------------------------------------

void badapt_activator_plain_s_adapt_apply( badapt_activator_plain_s* o, f3_t epsilon )
{
    /// nothing to do
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

void badapt_activator_bias_s_setup( badapt_activator_bias_s* o )
{
}

//----------------------------------------------------------------------------------------------------------------------

void badapt_activator_bias_s_reset( badapt_activator_bias_s* o )
{
    bmath_vf3_s_clear( &o->v_bias );
    bmath_vf3_s_clear( &o->v_bias_deferred );
}

//----------------------------------------------------------------------------------------------------------------------

void badapt_activator_bias_s_infer( const badapt_activator_bias_s* o, const bmath_vf3_s* in, bmath_vf3_s* out )
{
    assert( in->size == out->size );

    const badapt_activation_s* activation_p = badapt_activation_s_get_aware( o->activation );

    if( o->v_bias.size == 0 )
    {
        for( sz_t i = 0; i < out->size; i++ ) out->data[ i ] = badapt_activation_p_fx( activation_p, o->activation, in->data[ i ] );
    }
    else
    {
        assert( in->size == o->v_bias.size );
        for( sz_t i = 0; i < out->size; i++ ) out->data[ i ] = badapt_activation_p_fx( activation_p, o->activation, in->data[ i ] + o->v_bias.data[ i ] );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void badapt_activator_bias_s_bgrad( const badapt_activator_bias_s* o, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out, const bmath_vf3_s* out )
{
    assert( grad_in->size == grad_out->size );
    assert( grad_in->size ==      out->size );
    const badapt_activation_s* activation_p = badapt_activation_s_get_aware( o->activation );
    for( sz_t i = 0; i < out->size; i++ ) grad_in->data[ i ] = badapt_activation_p_dy( activation_p, o->activation, out->data[ i ] ) * grad_out->data[ i ];
}

//----------------------------------------------------------------------------------------------------------------------

void badapt_activator_bias_s_adapt( badapt_activator_bias_s* o, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out, const bmath_vf3_s* out, f3_t epsilon )
{
    badapt_activator_bias_s_bgrad( o, grad_in, grad_out, out );

    if( o->v_bias.size != out->size )
    {
        bmath_vf3_s_set_size( &o->v_bias, out->size );
        bmath_vf3_s_zro( &o->v_bias );
    }

    for( sz_t i = 0; i < out->size; i++ ) o->v_bias.data[ i ] += grad_in->data[ i ] * epsilon;
}

//----------------------------------------------------------------------------------------------------------------------

void badapt_activator_bias_s_adapt_defer( badapt_activator_bias_s* o, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out, const bmath_vf3_s* out )
{
    badapt_activator_bias_s_bgrad( o, grad_in, grad_out, out );

    if( o->v_bias_deferred.size != out->size )
    {
        bmath_vf3_s_set_size( &o->v_bias_deferred, out->size );
        bmath_vf3_s_zro( &o->v_bias_deferred );
    }

    for( sz_t i = 0; i < out->size; i++ ) o->v_bias_deferred.data[ i ] += grad_in->data[ i ];
}

//----------------------------------------------------------------------------------------------------------------------

void badapt_activator_bias_s_adapt_apply( badapt_activator_bias_s* o, f3_t epsilon )
{
    if( o->v_bias.size != o->v_bias_deferred.size )
    {
        bmath_vf3_s_set_size( &o->v_bias, o->v_bias_deferred.size );
        bmath_vf3_s_zro( &o->v_bias );
    }

    for( sz_t i = 0; i < o->v_bias.size; i++ ) o->v_bias.data[ i ] += o->v_bias_deferred.data[ i ] * epsilon;
    bmath_vf3_s_zro( &o->v_bias_deferred );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

vd_t badapt_activator_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "badapt_activator" ) ) )
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
            bcore_precoder_compile( "badapt_precoded", __FILE__ );
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

