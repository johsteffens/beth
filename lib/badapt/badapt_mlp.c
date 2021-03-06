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

#include "bcore_std.h"
#include "bmath_std.h"
#include "bmath_plot.h"
#include "badapt_mlp.h"
#include "badapt_trainer.h"
#include "badapt_problem.h"

/**********************************************************************************************************************/
// badapt_mlp_s

// ---------------------------------------------------------------------------------------------------------------------
#ifdef TYPEOF_badapt_mlp_s

void badapt_mlp_s_arc_to_sink( const badapt_mlp_s* o, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "#<sc_t>\n",  ifnameof( *(aware_t*)o ) );

    sz_t pad = 24;
    sz_t weights = 0;
    sz_t ops = 0;
    for( sz_t i = 0; i < o->arr_layer.arr_size; i++ )
    {
        const bmath_mf3_s* w = &o->arr_layer.arr_data[ i ].w;
        weights += w->size;
        ops     += w->size;
    }

    bcore_sink_a_push_fa( sink, "#pn.{input size } #pl3 {#<sz_t>}\n", pad, badapt_mlp_s_get_in_size( o ) );
    bcore_sink_a_push_fa( sink, "#pn.{output size } #pl3 {#<sz_t>}\n", pad, badapt_mlp_s_get_out_size( o ) );

    bcore_sink_a_push_fa( sink, "#pn.{layers } #pl3 {#<sz_t>}\n",  pad, o->arr_layer.arr_size );
    bcore_sink_a_push_fa( sink, "#pn.{weights } #pl3 {#<sz_t>}\n", pad, weights );
    bcore_sink_a_push_fa( sink, "#pn.{ops } #pl3 {#<sz_t>}\n",     pad, ops );

    for( sz_t i = 0; i < o->arr_layer.arr_size; i++ )
    {
        const badapt_mlp_layer_s* layer = &o->arr_layer.arr_data[ i ];

        const bmath_mf3_s* w = &layer->w;
        const bmath_vf3_s* b = &layer->o;
        st_s* st_activation = st_s_create();

        st_s_push_fa( st_activation, "#<sc_t>", ifnameof( *( aware_t* )layer->a ) );
        const badapt_activation* activation = badapt_activator_a_get_activation( layer->a );
        st_s_push_fa( st_activation, " : #<sc_t>", activation ? ifnameof( *( aware_t* )activation ) : "" );

        bcore_sink_a_push_fa( sink,
                              "layer #pl2 {#<sz_t>}:"
                              " a(#pl2 {#<sz_t>})"
                              " w(#pl2 {#<sz_t>} x #pl2 {#<sz_t>})"
                              " b(#pl2 {#<sz_t>})"
                              " #<sc_t>"
                              "\n",
                              i, layer->input_size, w->rows, w->cols, b->size, st_activation->sc );

        st_s_discard( st_activation );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_mlp_s_infer( const badapt_mlp_s* o, const bmath_vf3_s* in, bmath_vf3_s* out )
{
    if( o->arr_layer.arr_size == 0 ) ERR_fa( "Network was not setup. Call setup() first." );
    bmath_vf3_s a, b;
    bmath_vf3_s_init( &a );
    bmath_vf3_s_init( &b );
    bmath_vf3_s_set_space( &a, o->max_buffer_size );
    bmath_vf3_s_set_space( &b, o->max_buffer_size );

    bmath_vf3_s_copy( &a, in );

    for( sz_t i = 0; i < o->arr_layer.arr_size; i++ )
    {
        badapt_mlp_layer_s* layer = &o->arr_layer.arr_data[ i ];
        const bmath_mf3_s* w = &layer->w;
        b.size = w->rows;
        bmath_mf3_s_mul_vec( w, &a, &b );       // b = w * a
        bmath_vf3_s_add( &b, &layer->b, &b );   // b += bias
        badapt_activator_a_infer( layer->a, &b, &b );
        bmath_vf3_s_swapr( &a, &b );
    }

    if( out ) bmath_vf3_s_cpy( &a, out );

    bmath_vf3_s_down( &a );
    bmath_vf3_s_down( &b );
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_mlp_s_minfer( badapt_mlp_s* o, const bmath_vf3_s* in, bmath_vf3_s* out )
{
    if( o->arr_layer.arr_size == 0 ) ERR_fa( "Network was not setup. Call setup() first." );
    bmath_vf3_s_copy( &o->in, in );
    bmath_vf3_s* b = NULL;
    for( sz_t i = 0; i < o->arr_layer.arr_size; i++ )
    {
        badapt_mlp_layer_s* layer = &o->arr_layer.arr_data[ i ];
        const bmath_vf3_s* a = ( i > 0 ) ? &o->arr_layer.arr_data[ i - 1 ].o : &o->in;
        b = &layer->o;
        const bmath_mf3_s* w = &layer->w;
        if( b->size != w->rows ) bmath_vf3_s_set_size( b, w->rows );

        bmath_mf3_s_mul_vec( w, a, b );       // b = w * a
        bmath_vf3_s_add( b, &layer->b, b );   // b += bias
        badapt_activator_a_infer( layer->a, b, b );
    }
    if( out ) bmath_vf3_s_cpy( b, out );
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_mlp_s_bgrad( const badapt_mlp_s* o, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out )
{
    bmath_vf3_s ga, gb;
    bmath_vf3_s_init( &ga );
    bmath_vf3_s_init( &gb );
    bmath_vf3_s_set_space( &ga, o->max_buffer_size );
    bmath_vf3_s_set_space( &gb, o->max_buffer_size );
    bmath_vf3_s_copy( &gb, grad_out );

    for( sz_t i = o->arr_layer.arr_size - 1; i >= 0; i-- )
    {
        badapt_mlp_layer_s* layer = &o->arr_layer.arr_data[ i ];
        const bmath_vf3_s* b = &layer->o;
        const bmath_mf3_s* w = &layer->w;
        ga.size = w->cols;
        badapt_activator_a_bgrad( layer->a, &gb, &gb, b );
        bmath_mf3_s_htp_mul_vec( w, &gb, &ga );          // GA <- W^T * GB
        bmath_vf3_s_swapr( &ga, &gb );
    }

    if( grad_in ) bmath_vf3_s_cpy( &gb, grad_in );

    bmath_vf3_s_down( &ga );
    bmath_vf3_s_down( &gb );
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_mlp_s_bgrad_adapt( badapt_mlp_s* o, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out )
{
    bmath_vf3_s ga, gb;
    bmath_vf3_s_init( &ga );
    bmath_vf3_s_init( &gb );
    bmath_vf3_s_set_space( &ga, o->max_buffer_size );
    bmath_vf3_s_set_space( &gb, o->max_buffer_size );
    bmath_vf3_s_copy( &gb, grad_out );

    for( sz_t i = o->arr_layer.arr_size - 1; i >= 0; i-- )
    {
        badapt_mlp_layer_s* layer = &o->arr_layer.arr_data[ i ];
        const bmath_vf3_s* a = ( i > 0 ) ? &o->arr_layer.arr_data[ i - 1 ].o : &o->in;
        const bmath_vf3_s* b = &o->arr_layer.arr_data[ i ].o;
              bmath_mf3_s* w = &layer->w;

        ga.size = w->cols;

        badapt_activator_a_bgrad( layer->a, &gb, &gb, b );
        bmath_vf3_s_mul_scl_f3_add( &gb, o->dynamics.epsilon, &layer->b, &layer->b );

        bmath_mf3_s_htp_mul_vec( w, &gb, &ga );          // GA <- W^T * GB

        f3_t l2_reg_factor = ( 1.0 - o->dynamics.lambda_l2  * o->dynamics.epsilon );
        f3_t l1_reg_offset = o->dynamics.lambda_l1 * o->dynamics.epsilon;

        for( sz_t i = 0; i < w->rows; i++ )
        {
            f3_t* wr = w->data + i * w->stride;
            f3_t gi = o->dynamics.epsilon * gb.data[ i ];
            for( sz_t j = 0; j < w->cols; j++ ) wr[ j ] = ( wr[ j ] + a->data[ j ] * gi ) * l2_reg_factor;
            if( l1_reg_offset > 0 )
            {
                for( sz_t j = 0; j < w->cols; j++ ) wr[ j ] += ( wr[ j ] > 0 ) ? -l1_reg_offset : l1_reg_offset;
            }
        }

        bmath_vf3_s_swapr( &ga, &gb );
    }

    if( grad_in ) bmath_vf3_s_cpy( &gb, grad_in );

    bmath_vf3_s_down( &ga );
    bmath_vf3_s_down( &gb );
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// badapt_builder_mlp_funnel_s

// ---------------------------------------------------------------------------------------------------------------------

badapt_adaptive* badapt_builder_mlp_funnel_s_build( const badapt_builder_mlp_funnel_s* o )
{
    ASSERT( o->layers >= 2 );
    ASSERT( o->input_size     > 0 );
    ASSERT( o->output_kernels > 0 );

    BLM_INIT();

    u3_t random_state = o->random_seed;
    badapt_mlp_s* mlp = badapt_mlp_s_create();
    badapt_mlp_arr_layer_s_set_size( &mlp->arr_layer, o->layers );

    f3_t kernels = o->input_kernels;

    const badapt_mlp_layer_s* prev_layer = NULL;

    // first layer
    {
        badapt_mlp_layer_s* layer = &mlp->arr_layer.arr_data[ 0 ];
        layer->input_size = o->input_size;
        layer->kernels = lrint( kernels );
        prev_layer = layer;
    }

    for( sz_t i = 1; i < o->layers - 1; i++ )
    {
        kernels *= ( 1.0 + o->kernels_rate );
        badapt_mlp_layer_s* layer = &mlp->arr_layer.arr_data[ i ];
        layer->input_size = prev_layer->kernels;
        layer->kernels = lrint( kernels );
        prev_layer = layer;
    }

    // last layer
    {
        badapt_mlp_layer_s* layer = &mlp->arr_layer.arr_data[ o->layers - 1 ];
        layer->input_size = prev_layer->kernels;
        layer->kernels = o->output_kernels;
    }

    sz_t max_buffer_size = 0;

    // set up matrix and activator
    for( sz_t i = 0; i < o->layers; i++ )
    {
        badapt_mlp_layer_s* layer = &mlp->arr_layer.arr_data[ i ];
        bmath_mf3_s_set_size( &layer->w, layer->kernels, layer->input_size );
        bmath_mf3_s_set_random_u3( &layer->w, false, false, 0, 1.0, -0.5, 0.5, &random_state );

        bmath_vf3_s_set_size( &layer->b, layer->kernels );
        bmath_vf3_s_zro( &layer->b );

        layer->a = badapt_activator_a_clone( badapt_arr_layer_activator_s_get_activator( &o->arr_layer_activator, i, o->layers ) );
        max_buffer_size = sz_max( max_buffer_size, layer->input_size );
        max_buffer_size = sz_max( max_buffer_size, layer->kernels );
    }

    mlp->max_buffer_size = max_buffer_size;

    badapt_dynamics_std_s_copy( &mlp->dynamics, &o->dynamics );

    BLM_RETURNV( badapt_adaptive*, ( badapt_adaptive* )mlp );
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// tests

// ---------------------------------------------------------------------------------------------------------------------

void badapt_mlp_s_run_training( badapt_supplier* problem, badapt_builder* builder, const badapt_trainer_batch_s* trainer )
{
    BLM_INIT();
    badapt_training_state* state = BLM_A_PUSH( badapt_trainer_batch_s_create_state( trainer ) );
    badapt_supplier_a_setup_builder( problem, builder );
    badapt_training_state_a_set_adaptive( state, BLM_A_PUSH( badapt_builder_a_build( builder ) ) );
    badapt_training_state_a_set_supplier( state, problem );
    badapt_training_state_a_set_guide( state, ( badapt_guide* )BLM_A_PUSH( badapt_guide_std_s_create() ) );
    badapt_adaptive_a_arc_to_sink( badapt_training_state_a_get_adaptive( state ), BCORE_STDOUT );
    badapt_trainer_batch_s_run( trainer, ( badapt_training_state* )state );
    BLM_RETURN();
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_mlp_s_test_sine_random()
{
    BLM_INIT();
    badapt_problem_sine_random_s* problem = BLM_CREATE( badapt_problem_sine_random_s );
    badapt_builder_mlp_funnel_s*  builder = BLM_CREATE( badapt_builder_mlp_funnel_s);
    badapt_trainer_batch_s*       trainer = BLM_CREATE( badapt_trainer_batch_s );

    problem->input_size = 32;

    builder->input_kernels = 16;
    builder->layers        = 4;
    builder->kernels_rate  = 0;
    builder->random_seed   = 124;
    builder->dynamics.epsilon  = 0.0008;

    badapt_arr_layer_activator_s_push_from_names( &builder->arr_layer_activator,  0, "plain", "leaky_relu" );
    badapt_arr_layer_activator_s_push_from_names( &builder->arr_layer_activator, -1, "plain", "tanh" );

    trainer->fetch_cycles_per_iteration = 10;
    trainer->max_iterations = 10;

    badapt_mlp_s_run_training( ( badapt_supplier* )problem, ( badapt_builder* )builder, trainer );
    BLM_RETURN();
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_mlp_s_test_binary_add()
{
    BLM_INIT();
    badapt_problem_binary_add_s* problem = BLM_CREATE( badapt_problem_binary_add_s );
    badapt_builder_mlp_funnel_s* builder = BLM_CREATE( badapt_builder_mlp_funnel_s );
    badapt_trainer_batch_s*      trainer = BLM_CREATE( badapt_trainer_batch_s );

    problem->bits = 8;

    builder->input_kernels      = 32;
    builder->layers             = 4;
    builder->kernels_rate       = 0.3;
    builder->random_seed        = 124;
    builder->dynamics.epsilon   = 0.0003;

//    builder->dynamics.lambda_l1 = 0.0001;

    badapt_arr_layer_activator_s_push_from_names( &builder->arr_layer_activator,  0, "plain", "leaky_relu" );
    badapt_arr_layer_activator_s_push_from_names( &builder->arr_layer_activator, -1, "plain", "tanh" );

    trainer->fetch_cycles_per_iteration = 30;
    trainer->max_iterations = 10;

    badapt_mlp_s_run_training( ( badapt_supplier* )problem, ( badapt_builder* )builder, trainer );
    BLM_RETURN();
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_mlp_s_test_binary_mul()
{
    BLM_INIT();
    badapt_problem_binary_mul_s* problem = BLM_CREATE( badapt_problem_binary_mul_s );
    badapt_builder_mlp_funnel_s* builder = BLM_CREATE( badapt_builder_mlp_funnel_s );
    badapt_trainer_batch_s*      trainer = BLM_CREATE( badapt_trainer_batch_s );

    problem->bits = 5;

    builder->input_kernels = 32;
    builder->layers        = 6;
    builder->kernels_rate  = 0.1;
    builder->random_seed   = 124;
    builder->dynamics.epsilon   = 0.0001;
    builder->dynamics.lambda_l1 = 0.001;

    /// Note: To solve the multiplyier a bias seems inhibiting (for addition it seems supportive)
    badapt_arr_layer_activator_s_push_from_names( &builder->arr_layer_activator,  0, "plain", "leaky_relu" );
    badapt_arr_layer_activator_s_push_from_names( &builder->arr_layer_activator, -1, "plain", "tanh" );

    trainer->fetch_cycles_per_iteration = 30;
    trainer->max_iterations = 10;

    badapt_mlp_s_run_training( ( badapt_supplier* )problem, ( badapt_builder* )builder, trainer );
    BLM_RETURN();
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_mlp_s_test_binary_xsg3()
{
    BLM_INIT();
    badapt_problem_binary_xsg3_s* problem = BLM_CREATE( badapt_problem_binary_xsg3_s );
    badapt_builder_mlp_funnel_s*  builder = BLM_CREATE( badapt_builder_mlp_funnel_s );
    badapt_trainer_batch_s*       trainer = BLM_CREATE( badapt_trainer_batch_s );

    /// xsg3 <= 17 bits is learned very easily with 2 layers, while >= 18 bits seems extremely difficult for any configurations
    problem->bits = 17;

    builder->input_kernels = 30;
    builder->layers        = 2;
    builder->kernels_rate  = 0;
    builder->random_seed   = 124;
    builder->dynamics.epsilon   = 0.0001;
    builder->dynamics.lambda_l1 = 0.001;

    badapt_arr_layer_activator_s_push_from_names( &builder->arr_layer_activator,  0, "plain", "leaky_relu" );
    badapt_arr_layer_activator_s_push_from_names( &builder->arr_layer_activator, -1, "plain",  "tanh" );

    trainer->fetch_cycles_per_iteration = 30;
    trainer->max_iterations = 10;

    badapt_mlp_s_run_training( ( badapt_supplier* )problem, ( badapt_builder* )builder, trainer );
    BLM_RETURN();
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_mlp_s_test_binary_hash()
{
    BLM_INIT();
    badapt_problem_binary_hash_s* problem = BLM_CREATE( badapt_problem_binary_hash_s );
    badapt_builder_mlp_funnel_s*  builder = BLM_CREATE( badapt_builder_mlp_funnel_s );
    badapt_trainer_batch_s*       trainer = BLM_CREATE( badapt_trainer_batch_s );

    problem->bits = 9;
    problem->reverse = true;

    builder->input_kernels = 32;
    builder->layers        = 6;
    builder->kernels_rate  = 0;
    builder->random_seed   = 125;
    builder->dynamics.epsilon   = 0.001;
//    builder->dynamics.lambda_l1 = 0.001;
    builder->dynamics.lambda_l2 = 0.001;

    badapt_arr_layer_activator_s_push_from_names( &builder->arr_layer_activator,  0, "plain", "leaky_relu" );
    badapt_arr_layer_activator_s_push_from_names( &builder->arr_layer_activator, -1, "plain",  "tanh" );

    trainer->fetch_cycles_per_iteration = 30;
    trainer->max_iterations = 10;

    badapt_mlp_s_run_training( ( badapt_supplier* )problem, ( badapt_builder* )builder, trainer );
    BLM_RETURN();
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_mlp_s_test_polynom()
{
    BLM_INIT();
    badapt_problem_polynom_s*     problem = BLM_CREATE( badapt_problem_polynom_s );
    badapt_builder_mlp_funnel_s*  builder = BLM_CREATE( badapt_builder_mlp_funnel_s );
    badapt_trainer_batch_s*       trainer = BLM_CREATE( badapt_trainer_batch_s );

    problem->input_size  = 16;
    problem->output_size = 8;   // polynomial order + 1
    problem->range       = 2.0; // +/- range of coefficients
    problem->noise_level = 0.001;

    builder->input_kernels = 32;
    builder->layers        = 3;
    builder->kernels_rate  = 0;
    builder->random_seed   = 125;
    builder->dynamics.epsilon   = 0.001;
//    builder->dynamics.lambda_l1 = 0.001;
    builder->dynamics.lambda_l2 = 0.001;

//    badapt_arr_layer_activator_s_push_from_names( &builder->arr_layer_activator,  0, "plain", "leaky_relu" );
    badapt_arr_layer_activator_s_push_from_names( &builder->arr_layer_activator,  0, "plain", "leaky_relu" );
    badapt_arr_layer_activator_s_push_from_names( &builder->arr_layer_activator, -1, "plain", "tanh" );

    trainer->fetch_cycles_per_iteration = 3;
    trainer->batch_cycles_per_fetch = 10;
    trainer->max_iterations = 10;

    badapt_mlp_s_run_training( ( badapt_supplier* )problem, ( badapt_builder* )builder, trainer );
    BLM_RETURN();
}

// ---------------------------------------------------------------------------------------------------------------------

#endif

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

vd_t badapt_mlp_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "badapt_mlp" ) ) )
    {
        case TYPEOF_init1:
        {
        }
        break;

        case TYPEOF_selftest:
        {
            badapt_mlp_s_test_sine_random();
        }
        break;

        default: break;
    }

    return NULL;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/


