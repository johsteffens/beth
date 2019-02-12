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
#include "bmath_cnn.h"

BCORE_DEFINE_OBJECT_INST( bcore_inst, bmath_cnn_act_s )
"{"
    "st_s st_activation;"
    "st_s st_derivative;"
    "hidden bmath_fp_f3_unary fp_activation;"
    "hidden bmath_fp_f3_unary fp_derivative;"
"}";

/// sets function pointers
void bmath_cnn_act_s_setup( bmath_cnn_act_s* o )
{
    o->fp_activation = ( bmath_fp_f3_unary )bcore_function_get( typeof( o->st_activation.sc ) );
    o->fp_derivative = ( bmath_fp_f3_unary )bcore_function_get( typeof( o->st_derivative.sc ) );
}

/**********************************************************************************************************************/
/// math
f3_t bmath_cnn_tanh(  f3_t v ) { return 1.0 - ( 2.0 / ( exp( 2.0 * v ) + 1.0 ) ); }
f3_t bmath_cnn_gtanh( f3_t v ) { return 1.0 - f3_sqr( v ); }

f3_t bmath_cnn_relu(  f3_t v ) { return v > 0 ? v : 0; }
f3_t bmath_cnn_grelu( f3_t v ) { return v > 0 ? 1 : 0; }

f3_t bmath_cnn_leaky_relu(  f3_t v ) { return v > 0 ? v : v * 0.01; }
f3_t bmath_cnn_gleaky_relu( f3_t v ) { return v > 0 ? 1 : 0.01; }

/// smooth approximation of relu
f3_t bmath_cnn_softplus(  f3_t v ) { return log( 1.0 + exp( v ) ); }
f3_t bmath_cnn_gsoftplus( f3_t v ) { f3_t u = exp( v ); return ( u - 1.0 ) / u; }

bmath_cnn_act_s bmath_cnn_act( sc_t f, sc_t gf ) { bmath_cnn_act_s a; bmath_cnn_act_s_init( &a ); st_s_push_sc( &a.st_activation, f ), st_s_push_sc( &a.st_derivative, gf ); return a; }

bmath_cnn_act_s bmath_cnn_act_tanh()       { return bmath_cnn_act( "bmath_cnn_tanh",       "bmath_cnn_gtanh"       ); }
bmath_cnn_act_s bmath_cnn_act_relu()       { return bmath_cnn_act( "bmath_cnn_relu",       "bmath_cnn_grelu"       ); }
bmath_cnn_act_s bmath_cnn_act_softplus()   { return bmath_cnn_act( "bmath_cnn_softplus",   "bmath_cnn_gsoftplus"   ); }
bmath_cnn_act_s bmath_cnn_act_leaky_relu() { return bmath_cnn_act( "bmath_cnn_leaky_relu", "bmath_cnn_gleaky_relu" ); }

/**********************************************************************************************************************/

BCORE_DEFINE_OBJECT_INST( bcore_inst, bmath_cnn_s )
"{"
    "aware_t _;"

    "sz_t input_size;" // input vector size
    "sz_t input_step             = 1;" // input vector stepping
    "sz_t input_convolution_size = 2;" // first layer convolution
    "sz_t input_kernels          = 8;" // kernels on input layer
    "sz_t output_kernels         = 1;" // kernels on output layer
    "f3_t kernels_rate           = 0;" // rate at which number kernels increase per layer (negative: decrease)
    "sz_t reduction_step         = 2;" // dimensionality reduction stepping
    "sz_t convolution_size       = 2;" // dimensionality convolution size

    "bmath_cnn_act_s act_mid;" // middle activation function
    "bmath_cnn_act_s act_out;" // output activation function

    "u2_t random_state = 1234;"        // random state variable (for random initialization)

    "bmath_arr_mf3_s arr_w;" // weight matrix

    "hidden bmath_arr_mf3_s arr_a;" // input  matrix  (weak map to buf)
    "hidden bmath_arr_mf3_s arr_b;" // output matrix  (weak map to buf)
    "hidden bmath_vf3_s     buf_ab;" // data buffer for input and output
    "hidden bmath_arr_mf3_s arr_ga;" // grad input  matrix  (weak map to gbuf)
    "hidden bmath_arr_mf3_s arr_gb;" // grad output matrix  (weak map to gbuf)
    "hidden bmath_vf3_s     buf_gab;"

    "hidden bcore_arr_fp_s  arr_fp_activation;" // activation functions
    "hidden bcore_arr_fp_s  arr_fp_derivative;" // derivative functions

    "hidden bmath_mf3_s     learn_gw;"
    "hidden bmath_vf3_s     in;"
    "hidden bmath_vf3_s     out;"
    "hidden bmath_vf3_s    gout;"
"}";

// ---------------------------------------------------------------------------------------------------------------------

/// Preserves weights if already initialized, otherwise randomly initializes weights
void bmath_cnn_s_setup( bmath_cnn_s* o )
{
    if( o->act_mid.st_activation.size == 0 ) o->act_mid = bmath_cnn_act_softplus();
    if( o->act_out.st_activation.size == 0 ) o->act_out = bmath_cnn_act_tanh();
    bmath_cnn_act_s_setup( &o->act_mid );
    bmath_cnn_act_s_setup( &o->act_out );

    BCORE_LIFE_INIT();

    typedef struct
    {
        sz_t size;
        sz_t step;
        sz_t steps;
        sz_t kernel_size;
        sz_t kernels;
    } bmath_cnn_arc_item_s;

    sc_t bmath_cnn_arc_item_s_def =
    "{"
        "sz_t size;"
        "sz_t step;"
        "sz_t steps;"
        "sz_t kernel_size;"
        "sz_t kernels;"
    "}";

    tp_t typeof_bmath_cnn_arc_item_s = bcore_flect_type_parse_fa( "bmath_cnn_arc_item_s = #<sc_t>;", bmath_cnn_arc_item_s_def );
    tp_t typeof_arr_bmath_cnn_arc_item_s = bcore_flect_type_parse_fa( "arr_item = { aware_t _; bmath_cnn_arc_item_s [] arr; };" );
    bcore_array* arr_bmath_cnn_arc_item_s = BCORE_LIFE_A_PUSH( bcore_inst_t_create( typeof_arr_bmath_cnn_arc_item_s ) );

    bmath_cnn_arc_item_s item = { 0 };

    f3_t kernels = o->input_kernels;

    // first layer
    {
        item.size  = o->input_size;
        item.step  = o->input_step;
        item.kernel_size = o->input_convolution_size * o->input_step;
        item.steps       = ( ( item.size - item.kernel_size ) / item.step ) + 1;
        ASSERT( item.steps > 0 );
        item.kernels     = lrint( kernels );
        bcore_array_a_push( arr_bmath_cnn_arc_item_s, sr_twc( typeof_bmath_cnn_arc_item_s, &item ) );
    }

    for( ;; )
    {
        sz_t prev_kernels = item.kernels;
        item.size         = prev_kernels * item.steps;
        item.kernel_size  = prev_kernels * o->convolution_size;
        item.kernels      = lrint( kernels );
        ASSERT( item.kernels > 0 );
        kernels *= ( 1.0 + o->kernels_rate );

        for( sz_t reduction_step = o->reduction_step; reduction_step > 0; reduction_step-- )
        {
            item.step = prev_kernels * reduction_step;
            item.steps = ( ( item.size - item.kernel_size ) / item.step ) + 1;
            if( item.steps < 1 ) break;
            if( ( ( item.steps - 1 ) * item.step ) + item.kernel_size == item.size ) break;
        }

        if( item.steps <= 1 ) break;
        bcore_array_a_push( arr_bmath_cnn_arc_item_s, sr_twc( typeof_bmath_cnn_arc_item_s, &item ) );
    }

    // last layer
    {
        const bmath_cnn_arc_item_s* last_item = bcore_array_a_get_last( arr_bmath_cnn_arc_item_s ).o;
        item = *last_item;
        sz_t prev_kernels = item.kernels;
        item.size  = prev_kernels * item.steps;
        item.step  = 0;
        item.steps = 1;
        item.kernel_size = item.size;
        item.kernels = o->output_kernels;
        bcore_array_a_push( arr_bmath_cnn_arc_item_s, sr_twc( typeof_bmath_cnn_arc_item_s, &item ) );
    }

    sz_t layers = bcore_array_a_get_size( arr_bmath_cnn_arc_item_s );
    bmath_arr_mf3_s_set_size( &o->arr_a,  layers );
    bmath_arr_mf3_s_set_size( &o->arr_b,  layers );
    bmath_arr_mf3_s_set_size( &o->arr_ga, layers );
    bmath_arr_mf3_s_set_size( &o->arr_gb, layers );

    bcore_arr_fp_s_set_size( &o->arr_fp_activation, layers );
    bcore_arr_fp_s_set_size( &o->arr_fp_derivative, layers );

    // preexisting weights are reused
    if( o->arr_w.size == 0 )
    {
        bmath_arr_mf3_s_set_size( &o->arr_w, layers );
    }
    else
    {
        ASSERT( o->arr_w.size == layers );
    }


    sz_t buf_size = o->input_size;

    // set up matrices
    for( sz_t i = 0; i < layers; i++ )
    {
        const bmath_cnn_act_s* act = ( i < layers - 1 ) ? &o->act_mid : &o->act_out;
        o->arr_fp_activation.data[ i ] = ( fp_t )act->fp_activation;
        o->arr_fp_derivative.data[ i ] = ( fp_t )act->fp_derivative;

        const bmath_cnn_arc_item_s* item = bcore_array_a_get( arr_bmath_cnn_arc_item_s, i ).o;

        bmath_mf3_s* a  = &o->arr_a.data[ i ];
        bmath_mf3_s* b  = &o->arr_b.data[ i ];
        bmath_mf3_s* w  = &o->arr_w.data[ i ];
        bmath_mf3_s* ga = &o->arr_ga.data[ i ];
        bmath_mf3_s* gb = &o->arr_gb.data[ i ];

        a->rows   = item->steps;
        a->cols   = item->kernel_size;
        a->stride = item->step;
        a->size   = item->size;
        a->space  = 0; // not owning data

        if( w->size == 0 )
        {
            bmath_mf3_s_set_size( w, item->kernel_size, item->kernels );
            bmath_mf3_s_set_random( w, false, false, 0, 1.0, -0.5, 0.5, &o->random_state );
        }
        else
        {
            ASSERT( w->rows == item->kernel_size );
            ASSERT( w->cols == item->kernels );
        }

        b->rows   = a->rows;
        b->cols   = w->cols;
        b->stride = b->cols;
        b->size   = b->rows * b->cols;
        b->space  = 0; // not owning data

        *ga = *a;
        *gb = *b;

        buf_size += b->size;
    }

    bmath_vf3_s_set_size( &o->buf_ab,  buf_size );
    bmath_vf3_s_set_size( &o->buf_gab, buf_size );

    bmath_vf3_s_clear( &o->in );
    o->in.data = o->buf_ab.data;
    o->in.size = o->input_size;

    bmath_vf3_s_clear( &o->out );
    o->out.data = o->buf_ab.data + buf_size - o->output_kernels;
    o->out.size = o->output_kernels;

    bmath_vf3_s_clear( &o->gout );
    o->gout.data = o->buf_gab.data + buf_size - o->output_kernels;
    o->gout.size = o->output_kernels;

    sz_t buf_index = 0;

    // distribute buffer space
    for( sz_t i = 0; i < layers; i++ )
    {
        bmath_mf3_s* a  = &o->arr_a.data[ i ];
        bmath_mf3_s* b  = &o->arr_b.data[ i ];
        bmath_mf3_s* ga = &o->arr_ga.data[ i ];
        bmath_mf3_s* gb = &o->arr_gb.data[ i ];

        a->data  = o->buf_ab.data  + buf_index;
        ga->data = o->buf_gab.data + buf_index;
        buf_index += a->size;
        b->data  = o->buf_ab.data  + buf_index;
        gb->data = o->buf_gab.data + buf_index;

        ASSERT(  a->data - o->buf_ab.data + a->size    <= o->buf_ab.size );
        ASSERT(  b->data - o->buf_ab.data + b->size    <= o->buf_ab.size );
        ASSERT( ga->data - o->buf_gab.data + ga->size  <= o->buf_gab.size );
        ASSERT( gb->data - o->buf_gab.data + gb->size  <= o->buf_gab.size );

        if( i == layers - 1 )
        {
            ASSERT(  b->data - o->buf_ab.data  +  b->size  == o->buf_gab.size );
            ASSERT( gb->data - o->buf_gab.data + gb->size  == o->buf_gab.size );
        }
    }

    BCORE_LIFE_DOWN();
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_cnn_s_reset( bmath_cnn_s* o )
{
    bmath_arr_mf3_s_clear( &o->arr_w );
    bmath_arr_mf3_s_clear( &o->arr_a );
    bmath_arr_mf3_s_clear( &o->arr_b );
    bmath_arr_mf3_s_clear( &o->arr_ga );
    bmath_arr_mf3_s_clear( &o->arr_gb );
    bmath_vf3_s_clear( &o->buf_ab );
    bmath_vf3_s_clear( &o->buf_gab );
    bmath_vf3_s_clear( &o->in );
    bmath_vf3_s_clear( &o->out );
    bmath_vf3_s_clear( &o->gout );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_cnn_s_arc_to_sink( const bmath_cnn_s* o, bcore_sink* sink )
{
    sz_t pad = 24;
    sz_t weights = 0;
    sz_t ops = 0;
    for( sz_t i = 0; i < o->arr_w.size; i++ )
    {
        const bmath_mf3_s* a = &o->arr_a.data[ i ];
        const bmath_mf3_s* w = &o->arr_w.data[ i ];
        weights += w->size;
        ops     += a->rows * a->cols * w->rows;
    }


    bcore_sink_a_push_fa( sink, "#pn.{input size } #pl3 {#<sz_t>}\n", pad, o->input_size );
    bcore_sink_a_push_fa( sink, "#pn.{input step } #pl3 {#<sz_t>}\n", pad, o->input_step );
    bcore_sink_a_push_fa( sink, "#pn.{input convolution size } #pl3 {#<sz_t>}\n", pad, o->input_convolution_size );
    bcore_sink_a_push_fa( sink, "#pn.{input kernels } #pl3 {#<sz_t>}\n", pad, o->input_kernels );
    bcore_sink_a_push_fa( sink, "#pn.{kernels rate}   #<f3_t>\n", pad, o->kernels_rate );
    bcore_sink_a_push_fa( sink, "#pn.{reduction step } #pl3 {#<sz_t>}\n", pad, o->reduction_step );
    bcore_sink_a_push_fa( sink, "#pn.{convolution size } #pl3 {#<sz_t>}\n", pad, o->convolution_size );

    bcore_sink_a_push_fa( sink, "#pn.{layers } #pl3 {#<sz_t>}\n",  pad, o->arr_w.size );
    bcore_sink_a_push_fa( sink, "#pn.{weights } #pl3 {#<sz_t>}\n", pad, weights );
    bcore_sink_a_push_fa( sink, "#pn.{ops } #pl3 {#<sz_t>}\n",     pad, ops );

    for( sz_t i = 0; i < o->arr_w.size; i++ )
    {
        const bmath_mf3_s* a = &o->arr_a.data[ i ];
        const bmath_mf3_s* w = &o->arr_w.data[ i ];
        const bmath_mf3_s* b = &o->arr_b.data[ i ];
        sc_t sc_activation = ( i < o->arr_w.size - 1 ) ? o->act_mid.st_activation.sc : o->act_out.st_activation.sc;

        bcore_sink_a_push_fa( sink,
                              "layer #pl2 {#<sz_t>}:"
                              " a(#pl2 {#<sz_t>} x #pl2 {#<sz_t>})"
                              " w(#pl2 {#<sz_t>} x #pl2 {#<sz_t>})"
                              " b(#pl2 {#<sz_t>} x #pl2 {#<sz_t>})"
                              " #<sc_t>"
                              "\n",
                            i, a->rows, a->cols, w->rows, w->cols, b->rows, b->cols, sc_activation );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_cnn_s_query( bmath_cnn_s* o, const bmath_vf3_s* in, bmath_vf3_s* out )
{
    if( o->arr_a.size == 0 ) bmath_cnn_s_setup( o );
    bmath_vf3_s_cpy( in, &o->in );
    for( sz_t i = 0; i < o->arr_w.size; i++ )
    {
        const bmath_mf3_s* a = &o->arr_a.data[ i ];
              bmath_mf3_s* b = &o->arr_b.data[ i ];
        const bmath_mf3_s* w = &o->arr_w.data[ i ];
        bmath_fp_f3_unary fwd_map = ( bmath_fp_f3_unary )o->arr_fp_activation.data[ i ];

        bmath_mf3_s_mul( a, w, b );           // b = a * w
        bmath_mf3_s_eop_map( b, fwd_map, b ); // b <- fwd_map( b )
    }
    if( out ) bmath_vf3_s_cpy( &o->out, out );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_cnn_s_learn( bmath_cnn_s* o, const bmath_vf3_s* in, const bmath_vf3_s* target, f3_t step, bmath_vf3_s* out )
{
    ASSERT( target->size == o->output_kernels );
    bmath_cnn_s_query( o, in, out );
    bmath_vf3_s_sub( target, &o->out, &o->gout );
    for( sz_t i = o->arr_w.size - 1; i >= 0; i-- )
    {
        const bmath_mf3_s* a = &o->arr_a.data[ i ];
        const bmath_mf3_s* b = &o->arr_b.data[ i ];
        bmath_mf3_s* ga = &o->arr_ga.data[ i ];
        bmath_mf3_s* gb = &o->arr_gb.data[ i ];
        bmath_mf3_s*  w = &o->arr_w.data[ i ];
        bmath_fp_f3_unary bwd_map = ( bmath_fp_f3_unary )o->arr_fp_derivative.data[ i ];

        // apply activation derivative to GB
        bmath_mf3_s_eop_map_mul( b, bwd_map, gb, gb );                  // GB <- bwd_map( GB )
        bmath_mf3_s_mul_htp( gb, w, ga );                               // GA = GB * W^T (GA is folded)
        bmath_mf3_s_mul_add_cps( true, a, false, gb, step, w, 1.0, w ); // W += ( A^T * GB ) * step
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_cnn_s_decay( bmath_cnn_s* o, f3_t step )
{
    f3_t f = ( 1.0 - step );
    f = f3_max( 0, f );
    for( sz_t i = 0; i < o->arr_w.size; i++ ) bmath_mf3_s_mul_scl_f3( &o->arr_w.data[ i ], f, &o->arr_w.data[ i ] );
}

// ---------------------------------------------------------------------------------------------------------------------

f3_t bmath_cnn_s_query_1( bmath_cnn_s* o, const bmath_vf3_s* in )
{
    f3_t out_scl = 0;
    bmath_vf3_s out = bmath_vf3_weak( &out_scl, 1 );
    bmath_cnn_s_query( o, in, &out );
    return out_scl;
}

// ---------------------------------------------------------------------------------------------------------------------

f3_t bmath_cnn_s_learn_1( bmath_cnn_s* o, const bmath_vf3_s* in, f3_t target, f3_t step )
{
    f3_t scl_out = 0;
    bmath_vf3_s vec_out    = bmath_vf3_weak( &scl_out, 1 );
    bmath_vf3_s vec_target = bmath_vf3_weak( &target, 1 );
    bmath_cnn_s_learn( o, in, &vec_target, step, &vec_out );
    return scl_out;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

vd_t bmath_cnn_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_cnn" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_FFUNC( bmath_cnn_fp_unary, bmath_cnn_tanh );
            BCORE_REGISTER_FFUNC( bmath_cnn_fp_unary, bmath_cnn_gtanh );
            BCORE_REGISTER_FFUNC( bmath_cnn_fp_unary, bmath_cnn_relu );
            BCORE_REGISTER_FFUNC( bmath_cnn_fp_unary, bmath_cnn_grelu );
            BCORE_REGISTER_FFUNC( bmath_cnn_fp_unary, bmath_cnn_leaky_relu );
            BCORE_REGISTER_FFUNC( bmath_cnn_fp_unary, bmath_cnn_gleaky_relu );
            BCORE_REGISTER_FFUNC( bmath_cnn_fp_unary, bmath_cnn_softplus );
            BCORE_REGISTER_FFUNC( bmath_cnn_fp_unary, bmath_cnn_gsoftplus );

            BCORE_REGISTER_OBJECT( bmath_cnn_act_s );
            BCORE_REGISTER_OBJECT( bmath_cnn_s );
        }
        break;

        case TYPEOF_get_quicktypes:
        {
            BCORE_REGISTER_QUICKTYPE( bmath_cnn_tanh );
            BCORE_REGISTER_QUICKTYPE( bmath_cnn_gtanh );
            BCORE_REGISTER_QUICKTYPE( bmath_cnn_relu );
            BCORE_REGISTER_QUICKTYPE( bmath_cnn_grelu );
            BCORE_REGISTER_QUICKTYPE( bmath_cnn_leaky_relu );
            BCORE_REGISTER_QUICKTYPE( bmath_cnn_gleaky_relu );
            BCORE_REGISTER_QUICKTYPE( bmath_cnn_softplus );
            BCORE_REGISTER_QUICKTYPE( bmath_cnn_gsoftplus );

            BCORE_REGISTER_QUICKTYPE( bmath_cnn_act_s );
            BCORE_REGISTER_QUICKTYPE( bmath_cnn_s );
        }
        break;

        default: break;
    }

    return NULL;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/


