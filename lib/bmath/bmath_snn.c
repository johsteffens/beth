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
#include "bmath_snn.h"

BCORE_DEFINE_OBJECT_INST( bcore_inst, bmath_snn_act_s )
"{"
    "st_s st_activation;"
    "st_s st_derivative;"
    "hidden bmath_fp_f3_unary fp_activation;"
    "hidden bmath_fp_f3_unary fp_derivative;"
"}";

/// sets function pointers
void bmath_snn_act_s_setup( bmath_snn_act_s* o )
{
    o->fp_activation = ( bmath_fp_f3_unary )bcore_function_get( typeof( o->st_activation.sc ) );
    o->fp_derivative = ( bmath_fp_f3_unary )bcore_function_get( typeof( o->st_derivative.sc ) );
}

/**********************************************************************************************************************/
/// math
f3_t bmath_snn_tanh(  f3_t v ) { return 1.0 - ( 2.0 / ( exp( 2.0 * v ) + 1.0 ) ); }
f3_t bmath_snn_gtanh( f3_t v ) { return 1.0 - f3_sqr( v ); }

f3_t bmath_snn_relu(  f3_t v ) { return v > 0 ? v : 0; }
f3_t bmath_snn_grelu( f3_t v ) { return v > 0 ? 1 : 0; }

f3_t bmath_snn_leaky_relu(  f3_t v ) { return v > 0 ? v : v * 0.01; }
f3_t bmath_snn_gleaky_relu( f3_t v ) { return v > 0 ? 1 : 0.01; }

/// smooth approximation of relu
f3_t bmath_snn_softplus(  f3_t v ) { return log( 1.0 + exp( v ) ); }
f3_t bmath_snn_gsoftplus( f3_t v ) { f3_t u = exp( v ); return ( u - 1.0 ) / u; }

bmath_snn_act_s bmath_snn_act( sc_t f, sc_t gf ) { bmath_snn_act_s a; bmath_snn_act_s_init( &a ); st_s_push_sc( &a.st_activation, f ), st_s_push_sc( &a.st_derivative, gf ); return a; }

bmath_snn_act_s bmath_snn_act_tanh()       { return bmath_snn_act( "bmath_snn_tanh",       "bmath_snn_gtanh"       ); }
bmath_snn_act_s bmath_snn_act_relu()       { return bmath_snn_act( "bmath_snn_relu",       "bmath_snn_grelu"       ); }
bmath_snn_act_s bmath_snn_act_softplus()   { return bmath_snn_act( "bmath_snn_softplus",   "bmath_snn_gsoftplus"   ); }
bmath_snn_act_s bmath_snn_act_leaky_relu() { return bmath_snn_act( "bmath_snn_leaky_relu", "bmath_snn_gleaky_relu" ); }

/**********************************************************************************************************************/

BCORE_DEFINE_OBJECT_INST( bcore_inst, bmath_snn_s )
"{"
    "aware_t _;"

    /// === architecture parameters ================================
    "sz_t input_size;"             // input vector size
    "sz_t input_kernels    = 8;"   // (default 8) kernels on input layer
    "sz_t output_kernels   = 1;"   // (default 1) kernels on output layer
    "sz_t layers           = 2;"   // (default 2) number of layers
    "f3_t kernels_rate     = 0;"   // (default 0) rate at which number of kernels increase per layer (negative: decrease); last layer excluded
    "f3_t adapt_step       = 0.0001;" // learning rate
    "f3_t decay_step       = 0;" // weight decay rate
    "bmath_snn_act_s act_mid;"     // (default: softplus) middle activation function
    "bmath_snn_act_s act_out;"     // (default: tanh) output activation function
    "u2_t random_state     =1234;" // (default: 1234) random state variable (for random initialization)
    /// ==============================================================

    /// === runtime data =============================================
    "bmath_arr_mf3_s   arr_w;"  // weight matrix
    "hidden bmath_arr_vf3_s   arr_a;"  // input  vector (weak map to buf)
    "hidden bmath_arr_vf3_s   arr_b;"  // output vector (weak map to buf)
    "hidden bmath_vf3_s       buf_ab;" // data buffer for a and b vector
    "hidden bmath_arr_vf3_s   arr_ga;"  // gradient input  vector (weak map to gbuf)
    "hidden bmath_arr_vf3_s   arr_gb;"  // gradient output vector (weak map to gbuf)
    "hidden bmath_vf3_s       buf_gab;" // data buffer for ga and gb matrix
    "hidden bcore_arr_fp_s    arr_fp_activation;" // activation functions
    "hidden bcore_arr_fp_s    arr_fp_derivative;" // derivative functions
    "hidden bmath_vf3_s       in;"   // input vector weak map to arr_a[ 0 ]
    "hidden bmath_vf3_s       out;"  // output vector weak map to arr_b[ layers-1 ]
    "hidden bmath_vf3_s       gout;" // output vector weak map to arr_gb[ layers-1 ]
    /// ==============================================================

    /// === functions ================================================
    "func bmath_fp_sadaptiv: get_in_size;"
    "func bmath_fp_sadaptiv: set_in_size;"
    "func bmath_fp_sadaptiv: get_out_size;"
    "func bmath_fp_sadaptiv: set_out_size;"
    "func bmath_fp_sadaptiv: get_step;"
    "func bmath_fp_sadaptiv: set_step;"
    "func bmath_fp_sadaptiv: get_decay;"
    "func bmath_fp_sadaptiv: set_decay;"
    "func bmath_fp_sadaptiv: setup;"
    "func bmath_fp_sadaptiv: set_untrained;"
    "func bmath_fp_sadaptiv: arc_to_sink;"
    "func bmath_fp_sadaptiv: query;"
    "func bmath_fp_sadaptiv: adapt;"
    /// ==============================================================
"}";

// ---------------------------------------------------------------------------------------------------------------------

sz_t bmath_snn_s_get_in_size(  const bmath_snn_s* o )
{
    return o->input_size;
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_snn_s_set_in_size( bmath_snn_s* o, sz_t size )
{
    if( o->input_size != size )
    {
        o->input_size = size;
        bmath_snn_s_set_untrained( o );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

sz_t bmath_snn_s_get_out_size( const bmath_snn_s* o )
{
    return o->output_kernels;
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_snn_s_set_out_size( bmath_snn_s* o, sz_t size )
{
    if( o->output_kernels != size )
    {
        o->output_kernels = size;
        bmath_snn_s_set_untrained( o );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

f3_t bmath_snn_s_get_step( const bmath_snn_s* o )
{
    return o->adapt_step;
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_snn_s_set_step( bmath_snn_s* o, f3_t val )
{
    o->adapt_step = val;
}

// ---------------------------------------------------------------------------------------------------------------------

f3_t bmath_snn_s_get_decay( const bmath_snn_s* o )
{
    return o->decay_step;
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_snn_s_set_decay( bmath_snn_s* o, f3_t val )
{
    o->decay_step = val;
}

// ---------------------------------------------------------------------------------------------------------------------

/// Preserves weights if already initialized, otherwise randomly initializes weights
void bmath_snn_s_setup( bmath_snn_s* o, bl_t learning )
{
    ASSERT( o->layers >= 2 );
    if( o->act_mid.st_activation.size == 0 ) o->act_mid = bmath_snn_act_softplus();
    if( o->act_out.st_activation.size == 0 ) o->act_out = bmath_snn_act_tanh();
    bmath_snn_act_s_setup( &o->act_mid );
    bmath_snn_act_s_setup( &o->act_out );

    BCORE_LIFE_INIT();

    typedef struct
    {
        sz_t size;
        sz_t kernels;
    } bmath_snn_arc_item_s;

    sc_t bmath_snn_arc_item_s_def =
    "{"
        "sz_t size;"
        "sz_t kernels;"
    "}";

    tp_t typeof_bmath_snn_arc_item_s = bcore_flect_type_parse_fa( "bmath_snn_arc_item_s = #<sc_t>;", bmath_snn_arc_item_s_def );
    tp_t typeof_arr_bmath_snn_arc_item_s = bcore_flect_type_parse_fa( "{ aware_t _; bmath_snn_arc_item_s [] arr; };" );
    bcore_array* arr_bmath_snn_arc_item_s = BCORE_LIFE_A_PUSH( bcore_inst_t_create( typeof_arr_bmath_snn_arc_item_s ) );

    bmath_snn_arc_item_s item = { 0 };

    f3_t kernels = o->input_kernels;

    // first layer
    {
        item.size    = o->input_size;
        item.kernels = lrint( kernels );
        bcore_array_a_push( arr_bmath_snn_arc_item_s, sr_twc( typeof_bmath_snn_arc_item_s, &item ) );
    }

    for( sz_t i = 1; i < o->layers - 1; i++ )
    {
        kernels *= ( 1.0 + o->kernels_rate );
        sz_t prev_kernels = item.kernels;
        item.size         = prev_kernels;
        item.kernels      = lrint( kernels );
        ASSERT( item.kernels > 0 );
        bcore_array_a_push( arr_bmath_snn_arc_item_s, sr_twc( typeof_bmath_snn_arc_item_s, &item ) );
    }

    // last layer
    {
        const bmath_snn_arc_item_s* last_item = bcore_array_a_get_last( arr_bmath_snn_arc_item_s ).o;
        item = *last_item;
        sz_t prev_kernels = item.kernels;
        item.size  = prev_kernels;
        item.kernels = o->output_kernels;
        bcore_array_a_push( arr_bmath_snn_arc_item_s, sr_twc( typeof_bmath_snn_arc_item_s, &item ) );
    }

    bmath_arr_vf3_s_set_size( &o->arr_a, o->layers );
    bmath_arr_vf3_s_set_size( &o->arr_b, o->layers );
    bcore_arr_fp_s_set_size( &o->arr_fp_activation, o->layers );

    if( learning )
    {
        bmath_arr_vf3_s_set_size( &o->arr_ga, o->layers );
        bmath_arr_vf3_s_set_size( &o->arr_gb, o->layers );
        bcore_arr_fp_s_set_size( &o->arr_fp_derivative, o->layers );
    }
    else
    {
        bmath_arr_vf3_s_clear( &o->arr_ga );
        bmath_arr_vf3_s_clear( &o->arr_gb );
        bcore_arr_fp_s_clear( &o->arr_fp_derivative );
    }

    // preexisting weights are reused
    if( o->arr_w.size != o->layers )
    {
        bmath_arr_mf3_s_clear( &o->arr_w );
        bmath_arr_mf3_s_set_size( &o->arr_w, o->layers );
    }

    sz_t buf_full_size = o->input_size;
    sz_t buf_max_size = 0;

    // set up matrices
    for( sz_t i = 0; i < o->layers; i++ )
    {
        const bmath_snn_act_s* act = ( i < o->layers - 1 ) ? &o->act_mid : &o->act_out;
        o->arr_fp_activation.data[ i ] = ( fp_t )act->fp_activation;
        if( learning ) o->arr_fp_derivative.data[ i ] = ( fp_t )act->fp_derivative;

        const bmath_snn_arc_item_s* item = bcore_array_a_get( arr_bmath_snn_arc_item_s, i ).o;

        bmath_vf3_s* a  = &o->arr_a.data[ i ];
        bmath_vf3_s* b  = &o->arr_b.data[ i ];
        bmath_mf3_s* w  = &o->arr_w.data[ i ];
        bmath_vf3_s* ga = ( learning ) ? &o->arr_ga.data[ i ] : NULL;
        bmath_vf3_s* gb = ( learning ) ? &o->arr_gb.data[ i ] : NULL;

        a->size   = item->size;
        a->space  = 0; // not owning data
        b->size   = item->kernels;
        b->space  = 0; // not owning data

        if( learning )
        {
            *ga = *a;
            *gb = *b;
        }

        if( w->size == 0 )
        {
            bmath_mf3_s_set_size( w, item->kernels, item->size );
            bmath_mf3_s_set_random( w, false, false, 0, 1.0, -0.5, 0.5, &o->random_state );
        }
        else
        {
            ASSERT( w->rows == item->kernels );
            ASSERT( w->cols == item->size );
        }

        buf_full_size += b->size;
        buf_max_size = sz_max( buf_max_size, a->size );
        buf_max_size = sz_max( buf_max_size, b->size );
    }

    sz_t buf_part_size = buf_max_size * 2;

    if( learning )
    {
        bmath_vf3_s_set_size( &o->buf_ab,  buf_full_size );
        bmath_vf3_s_set_size( &o->buf_gab, buf_part_size );
    }
    else
    {
        bmath_vf3_s_clear( &o->buf_gab );
        bmath_vf3_s_set_size( &o->buf_ab, buf_part_size );
    }

    sz_t buf_index = 0;

    // distribute buffer space
    for( sz_t i = 0; i < o->layers; i++ )
    {
        bl_t even_layer = ( ( i & 1 ) == 0 );

        bmath_vf3_s* a  = &o->arr_a.data[ i ];
        bmath_vf3_s* b  = &o->arr_b.data[ i ];
        bmath_vf3_s* ga = ( learning ) ? &o->arr_ga.data[ i ] : NULL;
        bmath_vf3_s* gb = ( learning ) ? &o->arr_gb.data[ i ] : NULL;

        if( learning )
        {
            a->data = o->buf_ab.data + buf_index;
            b->data = o->buf_ab.data + buf_index + a->size;
            ga->data =  even_layer ? o->buf_gab.data : o->buf_gab.data + buf_max_size;
            gb->data = !even_layer ? o->buf_gab.data : o->buf_gab.data + buf_max_size;
        }
        else
        {
            a->data =  even_layer ? o->buf_ab.data : o->buf_ab.data + buf_max_size;
            b->data = !even_layer ? o->buf_ab.data : o->buf_ab.data + buf_max_size;
        }

        buf_index += a->size;

        ASSERT( a->data - o->buf_ab.data + a->size <= o->buf_ab.size );
        ASSERT( b->data - o->buf_ab.data + b->size <= o->buf_ab.size );

        // first layer
        if( i == 0 )
        {
            bmath_vf3_s_clear( &o->in );
            o->in.data = a->data;
            o->in.size = o->input_size;
        }

        // last layer
        if( i == o->layers - 1 )
        {
            bmath_vf3_s_clear( &o->out );
            o->out.data = b->data;
            o->out.size = o->output_kernels;

            bmath_vf3_s_clear( &o->gout );

            if( learning )
            {
                o->gout.data = gb->data;
                o->gout.size = o->output_kernels;
            }
        }
    }

    BCORE_LIFE_DOWN();
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_snn_s_set_untrained( bmath_snn_s* o )
{
    bmath_arr_mf3_s_clear( &o->arr_w );
    bmath_arr_vf3_s_clear( &o->arr_a );
    bmath_arr_vf3_s_clear( &o->arr_b );
    bmath_arr_vf3_s_clear( &o->arr_ga );
    bmath_arr_vf3_s_clear( &o->arr_gb );
    bmath_vf3_s_clear( &o->buf_ab );
    bmath_vf3_s_clear( &o->buf_gab );
    bmath_vf3_s_clear( &o->in );
    bmath_vf3_s_clear( &o->out );
    bmath_vf3_s_clear( &o->gout );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_snn_s_arc_to_sink( const bmath_snn_s* o, bcore_sink* sink )
{
    sz_t pad = 24;
    sz_t weights = 0;
    sz_t ops = 0;
    for( sz_t i = 0; i < o->arr_w.size; i++ )
    {
        const bmath_mf3_s* w = &o->arr_w.data[ i ];
        weights += w->size;
        ops     += w->size;
    }


    bcore_sink_a_push_fa( sink, "#pn.{input size } #pl3 {#<sz_t>}\n", pad, o->input_size );
    bcore_sink_a_push_fa( sink, "#pn.{input kernels } #pl3 {#<sz_t>}\n", pad, o->input_kernels );
    bcore_sink_a_push_fa( sink, "#pn.{output kernels } #pl3 {#<sz_t>}\n", pad, o->output_kernels );
    bcore_sink_a_push_fa( sink, "#pn.{kernels rate}   #<f3_t>\n", pad, o->kernels_rate );

    bcore_sink_a_push_fa( sink, "#pn.{layers } #pl3 {#<sz_t>}\n",  pad, o->layers );
    bcore_sink_a_push_fa( sink, "#pn.{weights } #pl3 {#<sz_t>}\n", pad, weights );
    bcore_sink_a_push_fa( sink, "#pn.{ops } #pl3 {#<sz_t>}\n",     pad, ops );

    for( sz_t i = 0; i < o->arr_w.size; i++ )
    {
        const bmath_vf3_s* a = &o->arr_a.data[ i ];
        const bmath_mf3_s* w = &o->arr_w.data[ i ];
        const bmath_vf3_s* b = &o->arr_b.data[ i ];
        sc_t sc_activation = ( i < o->arr_w.size - 1 ) ? o->act_mid.st_activation.sc : o->act_out.st_activation.sc;

        bcore_sink_a_push_fa( sink,
                              "layer #pl2 {#<sz_t>}:"
                              " a(#pl2 {#<sz_t>})"
                              " w(#pl2 {#<sz_t>} x #pl2 {#<sz_t>})"
                              " b(#pl2 {#<sz_t>})"
                              " #<sc_t>"
                              "\n",
                            i, a->size, w->rows, w->cols, b->size, sc_activation );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_snn_s_query( bmath_snn_s* o, const bmath_vf3_s* in, bmath_vf3_s* out )
{
    if( o->arr_a.size == 0 ) bmath_snn_s_setup( o, false );
    bmath_vf3_s_cpy( in, &o->in );
    for( sz_t i = 0; i < o->arr_w.size; i++ )
    {
        const bmath_vf3_s* a = &o->arr_a.data[ i ];
              bmath_vf3_s* b = &o->arr_b.data[ i ];
        const bmath_mf3_s* w = &o->arr_w.data[ i ];
        bmath_fp_f3_unary fwd_map = ( bmath_fp_f3_unary )o->arr_fp_activation.data[ i ];

        bmath_mf3_s_mul_vec( w, a, b );       // b = w * a
        bmath_vf3_s_eop_map( b, fwd_map, b ); // b <- fwd_map( b )
    }
    if( out ) bmath_vf3_s_cpy( &o->out, out );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_snn_s_adapt( bmath_snn_s* o, const bmath_vf3_s* in, const bmath_vf3_s* target, bmath_vf3_s* out )
{
    if( o->arr_ga.size == 0 ) bmath_snn_s_setup( o, true );
    ASSERT( target->size == o->output_kernels );
    bmath_snn_s_query( o, in, out );
    bmath_vf3_s_sub( target, &o->out, &o->gout );
    for( sz_t i = o->arr_w.size - 1; i >= 0; i-- )
    {
        const bmath_vf3_s*  a = &o->arr_a.data[ i ];
        const bmath_vf3_s*  b = &o->arr_b.data[ i ];
              bmath_vf3_s* ga = &o->arr_ga.data[ i ];
              bmath_vf3_s* gb = &o->arr_gb.data[ i ];
              bmath_mf3_s*  w = &o->arr_w.data[ i ];

        bmath_fp_f3_unary bwd_map = ( bmath_fp_f3_unary )o->arr_fp_derivative.data[ i ];
        f3_t wscale = f3_max( 0, ( 1.0 - o->decay_step ) );

        // apply activation derivative to GB

        bmath_vf3_s_eop_map_mul( b, bwd_map, gb, gb ); // GB <- bwd_map( b ) * GB
        bmath_mf3_s_htp_mul_vec( w, gb, ga );          // GA <- W^T * GB

        for( sz_t i = 0; i < w->rows; i++ )
        {
            f3_t* wr = w->data + i * w->stride;
            f3_t gi = o->adapt_step * gb->data[ i ];
            for( sz_t j = 0; j < w->cols; j++ )
            {
                wr[ j ] = wr[ j ] * wscale + a->data[ j ] * gi;
            }
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_snn_s_decay( bmath_snn_s* o, f3_t decay )
{
    f3_t f = ( 1.0 - decay );
    f = f3_max( 0, f );
    for( sz_t i = 0; i < o->arr_w.size; i++ ) bmath_mf3_s_mul_scl_f3( &o->arr_w.data[ i ], f, &o->arr_w.data[ i ] );
}

// ---------------------------------------------------------------------------------------------------------------------

f3_t bmath_snn_s_query_1( bmath_snn_s* o, const bmath_vf3_s* in )
{
    f3_t out_scl = 0;
    bmath_vf3_s out = bmath_vf3_weak( &out_scl, 1 );
    bmath_snn_s_query( o, in, &out );
    return out_scl;
}

// ---------------------------------------------------------------------------------------------------------------------

f3_t bmath_snn_s_adapt_1( bmath_snn_s* o, const bmath_vf3_s* in, f3_t target )
{
    f3_t scl_out = 0;
    bmath_vf3_s vec_out    = bmath_vf3_weak( &scl_out, 1 );
    bmath_vf3_s vec_target = bmath_vf3_weak( &target, 1 );
    bmath_snn_s_adapt( o, in, &vec_target, &vec_out );
    return scl_out;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

vd_t bmath_snn_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_snn" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_FFUNC( bmath_snn_fp_unary, bmath_snn_tanh );
            BCORE_REGISTER_FFUNC( bmath_snn_fp_unary, bmath_snn_gtanh );
            BCORE_REGISTER_FFUNC( bmath_snn_fp_unary, bmath_snn_relu );
            BCORE_REGISTER_FFUNC( bmath_snn_fp_unary, bmath_snn_grelu );
            BCORE_REGISTER_FFUNC( bmath_snn_fp_unary, bmath_snn_leaky_relu );
            BCORE_REGISTER_FFUNC( bmath_snn_fp_unary, bmath_snn_gleaky_relu );
            BCORE_REGISTER_FFUNC( bmath_snn_fp_unary, bmath_snn_softplus );
            BCORE_REGISTER_FFUNC( bmath_snn_fp_unary, bmath_snn_gsoftplus );

            BCORE_REGISTER_FFUNC( bmath_fp_sadaptiv_get_in_size,   bmath_snn_s_get_in_size );
            BCORE_REGISTER_FFUNC( bmath_fp_sadaptiv_set_in_size,   bmath_snn_s_set_in_size );
            BCORE_REGISTER_FFUNC( bmath_fp_sadaptiv_get_out_size,  bmath_snn_s_get_out_size );
            BCORE_REGISTER_FFUNC( bmath_fp_sadaptiv_set_out_size,  bmath_snn_s_set_out_size );
            BCORE_REGISTER_FFUNC( bmath_fp_sadaptiv_get_step,      bmath_snn_s_get_step );
            BCORE_REGISTER_FFUNC( bmath_fp_sadaptiv_set_step,      bmath_snn_s_set_step );
            BCORE_REGISTER_FFUNC( bmath_fp_sadaptiv_get_decay,     bmath_snn_s_get_decay );
            BCORE_REGISTER_FFUNC( bmath_fp_sadaptiv_set_decay,     bmath_snn_s_set_decay );
            BCORE_REGISTER_FFUNC( bmath_fp_sadaptiv_setup,         bmath_snn_s_setup );
            BCORE_REGISTER_FFUNC( bmath_fp_sadaptiv_set_untrained, bmath_snn_s_set_untrained );
            BCORE_REGISTER_FFUNC( bmath_fp_sadaptiv_arc_to_sink,   bmath_snn_s_arc_to_sink );
            BCORE_REGISTER_FFUNC( bmath_fp_sadaptiv_query,         bmath_snn_s_query );
            BCORE_REGISTER_FFUNC( bmath_fp_sadaptiv_adapt,         bmath_snn_s_adapt );

            BCORE_REGISTER_OBJECT( bmath_snn_act_s );
            BCORE_REGISTER_OBJECT( bmath_snn_s );
        }
        break;

        case TYPEOF_get_quicktypes:
        {
            BCORE_REGISTER_QUICKTYPE( bmath_snn_tanh );
            BCORE_REGISTER_QUICKTYPE( bmath_snn_gtanh );
            BCORE_REGISTER_QUICKTYPE( bmath_snn_relu );
            BCORE_REGISTER_QUICKTYPE( bmath_snn_grelu );
            BCORE_REGISTER_QUICKTYPE( bmath_snn_leaky_relu );
            BCORE_REGISTER_QUICKTYPE( bmath_snn_gleaky_relu );
            BCORE_REGISTER_QUICKTYPE( bmath_snn_softplus );
            BCORE_REGISTER_QUICKTYPE( bmath_snn_gsoftplus );

            BCORE_REGISTER_QUICKTYPE( bmath_snn_act_s );
            BCORE_REGISTER_QUICKTYPE( bmath_snn_s );
        }
        break;

        default: break;
    }

    return NULL;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/


