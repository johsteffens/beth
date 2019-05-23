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

#include "bcore_std.h"
#include "bmath_std.h"
#include "bmath_plot.h"
#include "badapt_c1d.h"
#include "badapt_trainer.h"
#include "badapt_problem.h"

/**********************************************************************************************************************/
// badapt_c1d_s

// ---------------------------------------------------------------------------------------------------------------------
#ifdef TYPEOF_badapt_c1d_s
sz_t badapt_c1d_s_get_in_size(  const badapt_c1d_s* o )
{
    return o->arr_layer.arr_size > 0 ? o->arr_layer.arr_data[ 0 ].input_size : 0;
}

// ---------------------------------------------------------------------------------------------------------------------

sz_t badapt_c1d_s_get_out_size( const badapt_c1d_s* o )
{
    return o->arr_layer.arr_size > 0 ? o->arr_layer.arr_data[ o->arr_layer.arr_size - 1 ].kernels : 0;
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_c1d_s_get_dynamics( const badapt_c1d_s* o, badapt_dynamics_s* dynamics )
{
    badapt_dynamics_s_copy( dynamics, &o->dynamics );
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_c1d_s_set_dynamics( badapt_c1d_s* o, const badapt_dynamics_s* dynamics )
{
    badapt_dynamics_s_copy( &o->dynamics, dynamics );
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_c1d_s_arc_to_sink( const badapt_c1d_s* o, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "#<sc_t>\n",  ifnameof( *(aware_t*)o ) );

    sz_t pad = 24;
    sz_t weights = 0;
    sz_t ops = 0;
    for( sz_t i = 0; i < o->arr_layer.arr_size; i++ )
    {
        badapt_c1d_layer_s* layer = &o->arr_layer.arr_data[ i ];

        sz_t a_rows = layer->steps;
        sz_t a_cols = layer->wgt.rows;
        const bmath_mf3_s* w = &layer->wgt;
        weights += w->size;
        ops     += a_rows * a_cols * w->rows;
    }

    bcore_sink_a_push_fa( sink, "#pn.{input size } #pl3 {#<sz_t>}\n",  pad, badapt_adaptive_a_get_in_size(  (badapt_adaptive* )o ) );
    bcore_sink_a_push_fa( sink, "#pn.{output size } #pl3 {#<sz_t>}\n", pad, badapt_adaptive_a_get_out_size( (badapt_adaptive* )o ) );

    bcore_sink_a_push_fa( sink, "#pn.{layers } #pl3 {#<sz_t>}\n",  pad, o->arr_layer.arr_size );
    bcore_sink_a_push_fa( sink, "#pn.{weights } #pl3 {#<sz_t>}\n", pad, weights );
    bcore_sink_a_push_fa( sink, "#pn.{ops } #pl3 {#<sz_t>}\n",     pad, ops );

    for( sz_t i = 0; i < o->arr_layer.arr_size; i++ )
    {
        badapt_c1d_layer_s* layer = &o->arr_layer.arr_data[ i ];

        sz_t a_rows = layer->steps;
        sz_t a_cols = layer->wgt.rows;
        sz_t b_rows = a_rows;
        sz_t b_cols = layer->wgt.cols;
        const bmath_mf3_s* w = &layer->wgt;

        st_s* st_activation = st_s_create();

        st_s_push_fa( st_activation, "#<sc_t>", ifnameof( *( aware_t* )layer->act ) );
        st_s_push_fa( st_activation, " : #<sc_t>", ifnameof( *( aware_t* )badapt_activator_a_get_activation( layer->act ) ) );

        bcore_sink_a_push_fa( sink,
                              "layer #pl2 {#<sz_t>}:"
                              " a(#pl4 {#<sz_t>})"
                              " ma(#pl2 {#<sz_t>} x #pl2 {#<sz_t>} : #pl2 {#<sz_t>})"
                              " mw(#pl2 {#<sz_t>} x #pl2 {#<sz_t>})"
                              " mb(#pl2 {#<sz_t>} x #pl2 {#<sz_t>})"
                              " b(#pl4 {#<sz_t>})"
                              " #<sc_t>"
                              "\n",
                            i, layer->input_size, a_rows, a_cols, layer->stride, w->rows, w->cols, b_rows, b_cols, a_rows * layer->kernels, st_activation->sc );

        st_s_discard( st_activation );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_c1d_s_infer( const badapt_c1d_s* o, const bmath_vf3_s* in, bmath_vf3_s* out )
{
    if( o->arr_layer.arr_size == 0 ) ERR_fa( "Network has no layers." );
    bmath_vf3_s a, b;
    bmath_vf3_s_init( &a );
    bmath_vf3_s_init( &b );
    bmath_vf3_s_set_space( &a, o->max_buffer_size );
    bmath_vf3_s_set_space( &b, o->max_buffer_size );
    bmath_mf3_s ma, mb;
    bmath_mf3_s_init( &ma );
    bmath_mf3_s_init( &mb );

    assert( in->size <= o->max_buffer_size );

    bmath_vf3_s_copy( &a, in );

    for( sz_t i = 0; i < o->arr_layer.arr_size; i++ )
    {
        badapt_c1d_layer_s* layer = &o->arr_layer.arr_data[ i ];
        ma.rows = layer->steps;
        ma.cols = layer->wgt.rows;
        ma.data = a.data;
        ma.size = a.size;
        ma.stride = layer->stride;

        assert( ( ma.rows - 1 ) * ma.stride + ma.cols == ma.size );

        mb.rows = ma.rows;
        mb.cols = layer->wgt.cols;
        mb.data = b.data;
        mb.size = mb.rows * mb.cols;
        mb.stride = mb.cols;
        b.size = mb.size;

        assert( b.size <= o->max_buffer_size );

        bmath_mf3_s_mul( &ma, &layer->wgt, &mb ); // a * w -> b

        badapt_activator_a_infer( layer->act, &b, &b );

        assert( !bmath_vf3_s_is_nan( &b ) );
        bmath_vf3_s_swapr( &a, &b );
    }

    if( out ) bmath_vf3_s_cpy( &a, out );

    bmath_vf3_s_down( &a );
    bmath_vf3_s_down( &b );
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_c1d_s_minfer( badapt_c1d_s* o, const bmath_vf3_s* in, bmath_vf3_s* out )
{
    if( o->arr_layer.arr_size == 0 ) ERR_fa( "Network has no layers." );
    bmath_vf3_s_copy( &o->in, in );
    bmath_vf3_s* b = NULL;

    bmath_mf3_s ma, mb;
    bmath_mf3_s_init( &ma ); // used weakly
    bmath_mf3_s_init( &mb ); // used weakly

    assert( in->size < o->max_buffer_size );

    for( sz_t i = 0; i < o->arr_layer.arr_size; i++ )
    {
        badapt_c1d_layer_s* layer = &o->arr_layer.arr_data[ i ];
        const bmath_vf3_s* a = ( i > 0 ) ? &o->arr_layer.arr_data[ i - 1 ].out : &o->in;
        b = &layer->out;
        const bmath_mf3_s* mw = &layer->wgt;

        ma.rows = layer->steps;
        ma.cols = layer->wgt.rows;
        ma.data = a->data;
        ma.size = a->size;
        ma.stride = layer->stride;

        assert( ( ma.rows - 1 ) * ma.stride + ma.cols == ma.size );
        if( b->size != ma.rows * mw->cols ) bmath_vf3_s_set_size( b, ma.rows * mw->cols );
        assert( b->size <= o->max_buffer_size );

        mb.rows = ma.rows;
        mb.cols = layer->wgt.cols;
        mb.data = b->data;
        mb.size = mb.rows * mb.cols;
        mb.stride = mb.cols;

        assert( !bmath_mf3_s_is_nan( &ma ) );
        assert( !bmath_mf3_s_is_nan( mw ) );
        bmath_mf3_s_mul( &ma, mw, &mb ); // a * w -> b

        badapt_activator_a_infer( layer->act, b, b );
        assert( !bmath_vf3_s_is_nan( b ) );
    }

    if( out ) bmath_vf3_s_cpy( b, out );
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_c1d_s_bgrad( const badapt_c1d_s* o, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out )
{
    bmath_vf3_s ga, gb;
    bmath_vf3_s_init( &ga );
    bmath_vf3_s_init( &gb );
    bmath_vf3_s_set_space( &ga, o->max_buffer_size );
    bmath_vf3_s_set_space( &gb, o->max_buffer_size );
    bmath_vf3_s_copy( &gb, grad_out );

    bmath_mf3_s ma, mb, mga, mgb;
    bmath_mf3_s_init( &ma  ); // used weakly
    bmath_mf3_s_init( &mb  ); // used weakly
    bmath_mf3_s_init( &mga ); // used weakly
    bmath_mf3_s_init( &mgb ); // used weakly

    bmath_vf3_s_copy( &gb, grad_out );

    for( sz_t i = o->arr_layer.arr_size - 1; i >= 0; i-- )
    {
        badapt_c1d_layer_s* layer = &o->arr_layer.arr_data[ i ];
        const bmath_vf3_s* a = ( i > 0 ) ? &o->arr_layer.arr_data[ i - 1 ].out : &o->in;
        const bmath_vf3_s* b = &o->arr_layer.arr_data[ i ].out;
              bmath_mf3_s* mw = &layer->wgt;

        ga.size = a->size;

        ma.rows   = layer->steps;
        ma.cols   = layer->wgt.rows;
        ma.size   = a->size;
        ma.stride = layer->stride;

        mb.rows   = ma.rows;
        mb.cols   = layer->wgt.cols;
        mb.size   = mb.rows * mb.cols;
        mb.stride = mb.cols;

        mga = ma;
        mgb = mb;

        ma.data  = a->data;
        mb.data  = b->data;
        mga.data = ga.data;
        mgb.data = gb.data;

        assert( ga.size <= o->max_buffer_size );

        badapt_activator_a_adapt( layer->act, &gb, &gb, b, o->dynamics.epsilon );

        // Note: "Dangling Elements"
        // Elements in A computed during progression but not influencing loss.
        // This is possible at certain degenerate (though valid) configurations (e.g. reduction > convolution size).
        // The gradient of dangling elements must be explicitly set to zero since spliced multiplication will not
        // reach them.
        bmath_vf3_s_zro( &ga );

        // GB * W^T (GA is folded) -> GA
        bmath_mf3_s_mul_add_cps( false, &mgb, true,  mw,  1.0,  NULL, 0, &mga );

        assert( !bmath_vf3_s_is_nan( &gb ) );

        bmath_vf3_s_swapr( &ga, &gb );
    }

    if( grad_in ) bmath_vf3_s_cpy( &gb, grad_in );

    bmath_vf3_s_down( &ga );
    bmath_vf3_s_down( &gb );
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_c1d_s_bgrad_adapt( badapt_c1d_s* o, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out )
{
    bmath_vf3_s ga, gb;
    bmath_vf3_s_init( &ga );
    bmath_vf3_s_init( &gb );
    bmath_vf3_s_set_space( &ga, o->max_buffer_size );
    bmath_vf3_s_set_space( &gb, o->max_buffer_size );
    bmath_vf3_s_copy( &gb, grad_out );

    bmath_mf3_s ma, mb, mga, mgb;
    bmath_mf3_s_init( &ma  ); // used weakly
    bmath_mf3_s_init( &mb  ); // used weakly
    bmath_mf3_s_init( &mga ); // used weakly
    bmath_mf3_s_init( &mgb ); // used weakly

    bmath_vf3_s_copy( &gb, grad_out );

    for( sz_t i = o->arr_layer.arr_size - 1; i >= 0; i-- )
    {
        badapt_c1d_layer_s* layer = &o->arr_layer.arr_data[ i ];
        const bmath_vf3_s* a = ( i > 0 ) ? &o->arr_layer.arr_data[ i - 1 ].out : &o->in;
        const bmath_vf3_s* b = &o->arr_layer.arr_data[ i ].out;
              bmath_mf3_s* mw = &layer->wgt;

        ga.size = a->size;

        ma.rows   = layer->steps;
        ma.cols   = layer->wgt.rows;
        ma.size   = a->size;
        ma.stride = layer->stride;

        mb.rows   = ma.rows;
        mb.cols   = layer->wgt.cols;
        mb.size   = mb.rows * mb.cols;
        mb.stride = mb.cols;

        mga = ma;
        mgb = mb;

        ma.data  = a->data;
        mb.data  = b->data;
        mga.data = ga.data;
        mgb.data = gb.data;

        assert( ga.size <= o->max_buffer_size );

        badapt_activator_a_adapt( layer->act, &gb, &gb, b, o->dynamics.epsilon );

        f3_t l2_reg_factor = ( 1.0 - o->dynamics.lambda_l2  * o->dynamics.epsilon );
        f3_t l1_reg_offset = o->dynamics.lambda_l1 * o->dynamics.epsilon;

        // Note: "Dangling Elements"
        // Elements in A computed during progression but not influencing loss.
        // This is possible at certain degenerate (though valid) configurations (e.g. reduction > convolution size).
        // The gradient of dangling elements must be explicitly set to zero since spliced multiplication will not
        // reach them.
        bmath_vf3_s_zro( &ga );

        // GB * W^T (GA is folded) -> GA
        bmath_mf3_s_mul_add_cps( false, &mgb, true,  mw,  1.0,  NULL, 0, &mga );

        // W * l2_reg_factor + ( A^T * GB ) * step -> W
        bmath_mf3_s_mul_add_cps( true,  &ma,  false, &mgb, o->dynamics.epsilon, mw, l2_reg_factor, mw );

        if( l1_reg_offset != 0 )
        {
            for( sz_t i = 0; i < mw->size; i++ ) mw->data[ i ] += ( mw->data[ i ] > 0 ) ? -l1_reg_offset : l1_reg_offset;
        }

        assert( !bmath_vf3_s_is_nan( &gb ) );

        bmath_vf3_s_swapr( &ga, &gb );
    }

    if( grad_in ) bmath_vf3_s_cpy( &gb, grad_in );

    bmath_vf3_s_down( &ga );
    bmath_vf3_s_down( &gb );
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// badapt_builder_c1d_funnel_s

// ---------------------------------------------------------------------------------------------------------------------

sz_t badapt_builder_c1d_funnel_s_get_in_size( const badapt_builder_c1d_funnel_s* o )
{
    return o->input_size;
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_builder_c1d_funnel_s_set_in_size( badapt_builder_c1d_funnel_s* o, sz_t size )
{
    o->input_size = size;
}

// ---------------------------------------------------------------------------------------------------------------------

sz_t badapt_builder_c1d_funnel_s_get_out_size( const badapt_builder_c1d_funnel_s* o )
{
    return o->output_kernels;
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_builder_c1d_funnel_s_set_out_size( badapt_builder_c1d_funnel_s* o, sz_t size )
{
    o->output_kernels = size;
}

// ---------------------------------------------------------------------------------------------------------------------

badapt_adaptive* badapt_builder_c1d_funnel_s_build( const badapt_builder_c1d_funnel_s* o )
{
    BCORE_LIFE_INIT();

    u2_t random_state = o->random_seed;
    badapt_c1d_s* c1d = badapt_c1d_s_create();

    f3_t kernels = o->input_kernels;

    const badapt_c1d_layer_s* prev_layer = NULL;

    // first layer
    {
        badapt_c1d_layer_s* layer = badapt_c1d_arr_layer_s_push( &c1d->arr_layer );

        layer->input_size  = o->input_size;
        layer->stride      = o->input_step;
        layer->kernel_size = o->input_convolution_size * o->input_step;
        layer->steps       = ( ( layer->input_size - layer->kernel_size ) / layer->stride ) + 1;
        ASSERT( layer->steps > 0 );
        layer->kernels     = sz_max( 1, lrint( kernels ) );
        prev_layer = layer;
    }

    while( prev_layer->steps > 1 )
    {
        kernels *= ( 1.0 + o->kernels_rate );
        badapt_c1d_layer_s* layer = badapt_c1d_arr_layer_s_push( &c1d->arr_layer );

        layer->input_size  = prev_layer->kernels * prev_layer->steps;
        layer->kernel_size = prev_layer->kernels * o->convolution_size;
        layer->kernels     = sz_max( 1, lrint( kernels ) );
        ASSERT( layer->kernels > 0 );

        for( sz_t reduction_step = o->reduction_step; reduction_step > 0; reduction_step-- )
        {
            layer->stride = prev_layer->kernels * reduction_step;
            layer->steps  = ( ( layer->input_size - layer->kernel_size ) / layer->stride ) + 1;
            if( layer->steps > 0 )
            {
                if( ( ( layer->steps - 1 ) * layer->stride ) + layer->kernel_size == layer->input_size ) break;
            }
        }

        // if no solutions with steps > 0 possible, create fully connected layer
        if( layer->steps < 1 )
        {
            layer->kernel_size = layer->input_size;
            layer->steps  = 1;
            layer->stride = 0;
        }

        prev_layer = layer;
    }

    // last layer (fully connected)
    {
        badapt_c1d_layer_s* layer = badapt_c1d_arr_layer_s_push( &c1d->arr_layer );
        layer->input_size  = prev_layer->kernels * prev_layer->steps;
        layer->kernel_size = layer->input_size;
        layer->kernels     = o->output_kernels;
        layer->steps       = 1;
        layer->stride      = 0;
        prev_layer = layer;
    }

    sz_t layers = c1d->arr_layer.arr_size;

    sz_t max_buffer_size = 0;

    // set up matrix and activator
    for( sz_t i = 0; i < layers; i++ )
    {
        badapt_c1d_layer_s* layer = &c1d->arr_layer.arr_data[ i ];
        bmath_mf3_s_set_size( &layer->wgt, layer->kernel_size, layer->kernels );
        bmath_mf3_s_set_random( &layer->wgt, false, false, 0, 1.0, -0.5, 0.5, &random_state );
        layer->act = badapt_activator_a_clone( badapt_arr_layer_activator_s_get_activator( &o->arr_layer_activator, i, layers ) );
        badapt_activator_a_setup( layer->act );
        max_buffer_size = sz_max( max_buffer_size, layer->input_size );
        max_buffer_size = sz_max( max_buffer_size, layer->kernels * layer->steps );
    }

    c1d->max_buffer_size = max_buffer_size;
    badapt_dynamics_s_copy( &c1d->dynamics, &o->dynamics );

    BCORE_LIFE_RETURN( ( badapt_adaptive* )c1d );
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

void badapt_c1d_s_run_training( badapt_supplier* problem, badapt_builder* builder, const badapt_trainer_batch_s* trainer )
{
    BCORE_LIFE_INIT();
    badapt_training_state* state = BCORE_LIFE_A_PUSH( badapt_trainer_batch_s_create_state( trainer ) );
    badapt_supplier_a_setup_builder( problem, builder );
    badapt_training_state_a_set_adaptive( state, BCORE_LIFE_A_PUSH( badapt_builder_a_build( builder ) ) );
    badapt_training_state_a_set_supplier( state, problem );
    badapt_training_state_a_set_guide( state, ( badapt_guide* )BCORE_LIFE_A_PUSH( badapt_guide_std_s_create() ) );
    badapt_adaptive_a_arc_to_sink( badapt_training_state_a_get_adaptive( state ), BCORE_STDOUT );
    badapt_trainer_batch_s_run( trainer, ( badapt_training_state* )state );
    BCORE_LIFE_RETURN();
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_c1d_s_test_sine_random()
{
    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( badapt_problem_sine_random_s, problem );
    BCORE_LIFE_CREATE( badapt_builder_c1d_funnel_s,  builder );
    BCORE_LIFE_CREATE( badapt_trainer_batch_s,             trainer );

    problem->input_size = 32;

    builder->input_kernels     = 4;
    builder->input_step        = 2;
    builder->input_convolution_size = 4;
    builder->convolution_size  = 2;
    builder->reduction_step    = 2;
    builder->kernels_rate      = 0;
    builder->random_seed       = 124;
    builder->dynamics.epsilon  = 0.001;

    badapt_arr_layer_activator_s_push_from_names( &builder->arr_layer_activator,  0, "bias", "leaky_relu" );
    badapt_arr_layer_activator_s_push_from_names( &builder->arr_layer_activator, -1, "bias", "tanh" );

    trainer->fetch_cycles_per_iteration = 10;
    trainer->max_iterations = 10;

    badapt_c1d_s_run_training( ( badapt_supplier* )problem, ( badapt_builder* )builder, trainer );
    BCORE_LIFE_RETURN();
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_c1d_s_test_binary_add()
{
    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( badapt_problem_binary_add_s, problem );
    BCORE_LIFE_CREATE( badapt_builder_c1d_funnel_s, builder );
    BCORE_LIFE_CREATE( badapt_trainer_batch_s,            trainer );

    problem->bits = 8;

    builder->input_kernels     = 7;
    builder->input_step        = 2;
    builder->input_convolution_size = 4;
    builder->convolution_size  = 2;
    builder->reduction_step    = 2;
    builder->kernels_rate      = 0.5;
    builder->random_seed       = 124;
    builder->dynamics.epsilon  = 0.0003;
//    builder->dynamics.lambda_l2 = 0.0001;
//    builder->dynamics.lambda_l1 = 0.0001;

    badapt_arr_layer_activator_s_push_from_names( &builder->arr_layer_activator,  0, "bias", "leaky_relu" );
    badapt_arr_layer_activator_s_push_from_names( &builder->arr_layer_activator, -1, "bias", "tanh" );

    trainer->fetch_cycles_per_iteration = 30;
    trainer->max_iterations = 10;

    badapt_c1d_s_run_training( ( badapt_supplier* )problem, ( badapt_builder* )builder, trainer );
    BCORE_LIFE_RETURN();
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_c1d_s_test_binary_mul()
{
    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( badapt_problem_binary_mul_s, problem );
    BCORE_LIFE_CREATE( badapt_builder_c1d_funnel_s, builder );
    BCORE_LIFE_CREATE( badapt_trainer_batch_s,            trainer );

    problem->bits = 5;

    builder->input_kernels     = 32;
    builder->input_step        = 1;
    builder->input_convolution_size = 4;
    builder->convolution_size  = 2;
    builder->reduction_step    = 2;
    builder->kernels_rate      = 0.1;
    builder->random_seed       = 124;
    builder->dynamics.epsilon  = 0.0003;

    badapt_arr_layer_activator_s_push_from_names( &builder->arr_layer_activator,  0, "plain", "leaky_relu" );
    badapt_arr_layer_activator_s_push_from_names( &builder->arr_layer_activator, -1, "plain", "tanh" );

    trainer->fetch_cycles_per_iteration = 30;
    trainer->max_iterations = 10;

    badapt_c1d_s_run_training( ( badapt_supplier* )problem, ( badapt_builder* )builder, trainer );
    BCORE_LIFE_RETURN();
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_c1d_s_test_binary_xsg3()
{
    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( badapt_problem_binary_xsg3_s, problem );
    BCORE_LIFE_CREATE( badapt_builder_c1d_funnel_s,  builder );
    BCORE_LIFE_CREATE( badapt_trainer_batch_s,             trainer );

    /// xsg3 <= 17 bits is learned very easily with 2 layers, while >= 18 bits seems extremely difficult for any configurations
    problem->bits = 17;

    /// xsg3 seems to be best (very easily) learned with a shallow mlp. A cnn appears to have trouble.
    builder->input_kernels     = 16;
    builder->input_step        = 2;
    builder->input_convolution_size = 4;
    builder->convolution_size  = 3;
    builder->reduction_step    = 3;
    builder->kernels_rate      = 0.2;
    builder->random_seed       = 126;
    builder->dynamics.epsilon  = 0.001;

    badapt_arr_layer_activator_s_push_from_names( &builder->arr_layer_activator,  0, "bias", "leaky_relu" );
    badapt_arr_layer_activator_s_push_from_names( &builder->arr_layer_activator, -1, "bias",  "tanh" );

    trainer->fetch_cycles_per_iteration = 30;
    trainer->max_iterations = 10;

    badapt_c1d_s_run_training( ( badapt_supplier* )problem, ( badapt_builder* )builder, trainer );
    BCORE_LIFE_RETURN();
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_c1d_s_test_binary_hash()
{
    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( badapt_problem_binary_hash_s, problem );
    BCORE_LIFE_CREATE( badapt_builder_c1d_funnel_s,  builder );
    BCORE_LIFE_CREATE( badapt_trainer_batch_s,             trainer );

    problem->bits = 9;
    problem->reverse = true; // reverse hashing is a hard problem for mlp and cnn

    builder->input_kernels     = 16;
    builder->input_step        = 1;
    builder->input_convolution_size = 2;
    builder->convolution_size  = 2;
    builder->reduction_step    = 2;
    builder->kernels_rate      = 0.5;
    builder->random_seed       = 124;
    builder->dynamics.epsilon  = 0.0003;

    badapt_arr_layer_activator_s_push_from_names( &builder->arr_layer_activator,  0, "bias", "leaky_relu" );
    badapt_arr_layer_activator_s_push_from_names( &builder->arr_layer_activator, -1, "bias",  "tanh" );

    trainer->fetch_cycles_per_iteration = 30;
    trainer->max_iterations = 100;

    badapt_c1d_s_run_training( ( badapt_supplier* )problem, ( badapt_builder* )builder, trainer );
    BCORE_LIFE_RETURN();
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_c1d_s_test_polynom()
{
    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( badapt_problem_polynom_s,     problem );
    BCORE_LIFE_CREATE( badapt_builder_c1d_funnel_s,  builder );
    BCORE_LIFE_CREATE( badapt_trainer_batch_s,             trainer );

    problem->input_size  = 16;
    problem->output_size = 8;   // polynomial order + 1
    problem->range       = 2.0; // +/- range of coefficients
    problem->noise_level = 0.001;

    builder->input_kernels     = 4;
    builder->input_step        = 1;
    builder->input_convolution_size = 4;
    builder->convolution_size  = 2;
    builder->reduction_step    = 2;
    builder->kernels_rate      = 0.3;
    builder->random_seed       = 124;
    builder->dynamics.epsilon  = 0.001;
//    builder->dynamics.lambda_l2  = 0.001;

    badapt_arr_layer_activator_s_push_from_names( &builder->arr_layer_activator,  0, "bias", "leaky_relu" );
    badapt_arr_layer_activator_s_push_from_names( &builder->arr_layer_activator, -1, "bias", "tanh" );

    trainer->fetch_cycles_per_iteration = 3;
    trainer->batch_cycles_per_fetch = 10;
    trainer->max_iterations = 10;

    badapt_c1d_s_run_training( ( badapt_supplier* )problem, ( badapt_builder* )builder, trainer );
    BCORE_LIFE_RETURN();
}

// ---------------------------------------------------------------------------------------------------------------------

#endif

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

vd_t badapt_c1d_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "badapt_c1d" ) ) )
    {
        case TYPEOF_init1:
        {
        }
        break;

        case TYPEOF_selftest:
        {
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

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/


