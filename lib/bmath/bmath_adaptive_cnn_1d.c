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
#include "bmath_adaptive_cnn_1d.h"

#ifdef TYPEOF_bmath_adaptive_cnn_1d

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

sz_t bmath_adaptive_cnn_1d_s_get_in_size(  const bmath_adaptive_cnn_1d_s* o )
{
    return o->input_size;
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_adaptive_cnn_1d_s_set_in_size( bmath_adaptive_cnn_1d_s* o, sz_t size )
{
    if( o->input_size != size )
    {
        o->input_size = size;
        bmath_adaptive_cnn_1d_s_set_untrained( o );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

sz_t bmath_adaptive_cnn_1d_s_get_out_size( const bmath_adaptive_cnn_1d_s* o )
{
    return o->output_kernels;
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_adaptive_cnn_1d_s_set_out_size( bmath_adaptive_cnn_1d_s* o, sz_t size )
{
    if( o->output_kernels != size )
    {
        o->output_kernels = size;
        bmath_adaptive_cnn_1d_s_set_untrained( o );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

f3_t bmath_adaptive_cnn_1d_s_get_step( const bmath_adaptive_cnn_1d_s* o )
{
    return o->adapt_step;
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_adaptive_cnn_1d_s_set_step( bmath_adaptive_cnn_1d_s* o, f3_t val )
{
    o->adapt_step = val;
}

// ---------------------------------------------------------------------------------------------------------------------

f3_t bmath_adaptive_cnn_1d_s_get_decay( const bmath_adaptive_cnn_1d_s* o )
{
    return o->decay_step;
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_adaptive_cnn_1d_s_set_decay( bmath_adaptive_cnn_1d_s* o, f3_t val )
{
    o->decay_step = val;
}

// ---------------------------------------------------------------------------------------------------------------------
/// Preserves weights if already initialized, otherwise randomly initializes weights
void bmath_adaptive_cnn_1d_s_setup( bmath_adaptive_cnn_1d_s* o, bl_t learning )
{
    if( !o->act_mid.o ) o->act_mid = sr_create( TYPEOF_bmath_activation_softplus_s );
    if( !o->act_out.o ) o->act_out = sr_create( TYPEOF_bmath_activation_tanh_s );

    BCORE_LIFE_INIT();

    typedef struct
    {
        sz_t size;
        sz_t step;
        sz_t steps;
        sz_t kernel_size;
        sz_t kernels;
    } bmath_adaptive_cnn_1d_arc_item_s;

    sc_t bmath_adaptive_cnn_1d_arc_item_s_def =
    "{"
        "sz_t size;"
        "sz_t step;"
        "sz_t steps;"
        "sz_t kernel_size;"
        "sz_t kernels;"
    "}";

    tp_t typeof_bmath_adaptive_cnn_1d_arc_item_s = bcore_flect_type_parse_fa( "bmath_adaptive_cnn_1d_arc_item_s = #<sc_t>;", bmath_adaptive_cnn_1d_arc_item_s_def );
    tp_t typeof_arr_bmath_adaptive_cnn_1d_arc_item_s = bcore_flect_type_parse_fa( "{ aware_t _; bmath_adaptive_cnn_1d_arc_item_s [] arr; };" );
    bcore_array* arr_bmath_adaptive_cnn_1d_arc_item_s = BCORE_LIFE_A_PUSH( bcore_inst_t_create( typeof_arr_bmath_adaptive_cnn_1d_arc_item_s ) );

    bmath_adaptive_cnn_1d_arc_item_s item = { 0 };

    f3_t kernels = o->input_kernels;

    // first layer
    {
        item.size  = o->input_size;
        item.step  = o->input_step;
        item.kernel_size = o->input_convolution_size * o->input_step;
        item.steps       = ( ( item.size - item.kernel_size ) / item.step ) + 1;
        ASSERT( item.steps > 0 );
        item.kernels     = lrint( kernels );
        bcore_array_a_push( arr_bmath_adaptive_cnn_1d_arc_item_s, sr_twc( typeof_bmath_adaptive_cnn_1d_arc_item_s, &item ) );
    }

    for( ;; )
    {
        kernels *= ( 1.0 + o->kernels_rate );
        sz_t prev_kernels = item.kernels;
        item.size         = prev_kernels * item.steps;
        item.kernel_size  = prev_kernels * o->convolution_size;
        item.kernels      = lrint( kernels );
        ASSERT( item.kernels > 0 );

        for( sz_t reduction_step = o->reduction_step; reduction_step > 0; reduction_step-- )
        {
            item.step = prev_kernels * reduction_step;
            item.steps = ( ( item.size - item.kernel_size ) / item.step ) + 1;
            if( item.steps < 1 ) break;
            if( ( ( item.steps - 1 ) * item.step ) + item.kernel_size == item.size ) break;
        }

        if( item.steps <= 1 ) break;
        bcore_array_a_push( arr_bmath_adaptive_cnn_1d_arc_item_s, sr_twc( typeof_bmath_adaptive_cnn_1d_arc_item_s, &item ) );
    }

    // last layer
    {
        const bmath_adaptive_cnn_1d_arc_item_s* last_item = bcore_array_a_get_last( arr_bmath_adaptive_cnn_1d_arc_item_s ).o;
        item = *last_item;
        sz_t prev_kernels = item.kernels;
        item.size  = prev_kernels * item.steps;
        item.step  = 0;
        item.steps = 1;
        item.kernel_size = item.size;
        item.kernels = o->output_kernels;
        bcore_array_a_push( arr_bmath_adaptive_cnn_1d_arc_item_s, sr_twc( typeof_bmath_adaptive_cnn_1d_arc_item_s, &item ) );
    }

    sz_t layers = bcore_array_a_get_size( arr_bmath_adaptive_cnn_1d_arc_item_s );

    bmath_arr_mf3_s_set_size( &o->arr_a,  layers );
    bmath_arr_mf3_s_set_size( &o->arr_b,  layers );
    bcore_arr_sr_s_set_size( &o->arr_activation, layers );

    if( learning )
    {
        bmath_arr_mf3_s_set_size( &o->arr_ga, layers );
        bmath_arr_mf3_s_set_size( &o->arr_gb, layers );
    }
    else
    {
        bmath_arr_mf3_s_clear( &o->arr_ga );
        bmath_arr_mf3_s_clear( &o->arr_gb );
    }

    // preexisting weights are reused
    if( o->arr_w.size != layers )
    {
        bmath_arr_mf3_s_clear( &o->arr_w );
        bmath_arr_mf3_s_set_size( &o->arr_w, layers );
    }

    sz_t buf_full_size = o->input_size;
    sz_t max_g_size = 0;

    // set up matrices
    for( sz_t i = 0; i < layers; i++ )
    {
        sr_s_copy( &o->arr_activation.data[ i ], ( i < layers - 1 ) ? &o->act_mid : &o->act_out );

        const bmath_adaptive_cnn_1d_arc_item_s* item = bcore_array_a_get( arr_bmath_adaptive_cnn_1d_arc_item_s, i ).o;

        bmath_mf3_s* a  = &o->arr_a.data[ i ];
        bmath_mf3_s* b  = &o->arr_b.data[ i ];
        bmath_mf3_s* w  = &o->arr_w.data[ i ];
        bmath_mf3_s* ga = ( learning ) ? &o->arr_ga.data[ i ] : NULL;
        bmath_mf3_s* gb = ( learning ) ? &o->arr_gb.data[ i ] : NULL;

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

        if( learning )
        {
            *ga = *a;
            *gb = *b;
        }

        buf_full_size += b->size;
        max_g_size = sz_max( max_g_size, a->size );
        max_g_size = sz_max( max_g_size, b->size );
    }

    bcore_arr_sr_s_set_spect( &o->arr_activation, TYPEOF_bmath_activation_s );

    sz_t buf_part_size = max_g_size * 2;

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
    for( sz_t i = 0; i < layers; i++ )
    {
        bl_t even_layer = ( ( i & 1 ) == 0 );

        bmath_mf3_s* a  = &o->arr_a.data[ i ];
        bmath_mf3_s* b  = &o->arr_b.data[ i ];
        bmath_mf3_s* ga = ( learning ) ? &o->arr_ga.data[ i ] : NULL;
        bmath_mf3_s* gb = ( learning ) ? &o->arr_gb.data[ i ] : NULL;

        if( learning )
        {
            a->data = o->buf_ab.data + buf_index;
            b->data = o->buf_ab.data + buf_index + a->size;
            ga->data =  even_layer ? o->buf_gab.data : o->buf_gab.data + max_g_size;
            gb->data = !even_layer ? o->buf_gab.data : o->buf_gab.data + max_g_size;
        }
        else
        {
            a->data =  even_layer ? o->buf_ab.data : o->buf_ab.data + max_g_size;
            b->data = !even_layer ? o->buf_ab.data : o->buf_ab.data + max_g_size;
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
        if( i == layers - 1 )
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

void bmath_adaptive_cnn_1d_s_set_untrained( bmath_adaptive_cnn_1d_s* o )
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
void bmath_adaptive_cnn_1d_s_arc_to_sink( const bmath_adaptive_cnn_1d_s* o, bcore_sink* sink )
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
        sc_t sc_activation = ifnameof( sr_s_type( &o->arr_activation.data[ i ] ) );

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

void bmath_adaptive_cnn_1d_s_query( bmath_adaptive_cnn_1d_s* o, const bmath_vf3_s* in, bmath_vf3_s* out )
{
    if( o->arr_a.size == 0 ) bmath_adaptive_cnn_1d_s_setup( o, false );
    bmath_vf3_s_cpy( in, &o->in );
    for( sz_t i = 0; i < o->arr_w.size; i++ )
    {
        const bmath_mf3_s* a = &o->arr_a.data[ i ];
              bmath_mf3_s* b = &o->arr_b.data[ i ];
        const bmath_mf3_s* w = &o->arr_w.data[ i ];
        const sr_s* sr_act = &o->arr_activation.data[ i ];
        assert( sr_s_p_type( sr_act ) == TYPEOF_bmath_activation_s );

        bmath_mf3_s_mul( a, w, b );           // a * w -> b

        for( sz_t i = 0; i < b->size; i++ ) b->data[ i ] = bmath_activation_p_fx( sr_act->p, sr_act->o, b->data[ i ] );
    }
    if( out ) bmath_vf3_s_cpy( &o->out, out );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_adaptive_cnn_1d_s_adapt( bmath_adaptive_cnn_1d_s* o, const bmath_vf3_s* in, const bmath_vf3_s* target, bmath_vf3_s* out )
{
    if( o->arr_ga.size == 0 ) bmath_adaptive_cnn_1d_s_setup( o, true );
    ASSERT( target->size == o->output_kernels );
    bmath_adaptive_cnn_1d_s_query( o, in, out );
    bmath_vf3_s_sub( target, &o->out, &o->gout );
    for( sz_t i = o->arr_w.size - 1; i >= 0; i-- )
    {
        const bmath_mf3_s*  a = &o->arr_a.data[ i ];
        const bmath_mf3_s*  b = &o->arr_b.data[ i ];
              bmath_mf3_s* ga = &o->arr_ga.data[ i ];
              bmath_mf3_s* gb = &o->arr_gb.data[ i ];
              bmath_mf3_s*  w = &o->arr_w.data[ i ];

        const sr_s* sr_act = &o->arr_activation.data[ i ];
        assert( sr_s_p_type( sr_act ) == TYPEOF_bmath_activation_s );

        f3_t wscale = f3_max( 0, ( 1.0 - o->decay_step ) );

        // apply activation derivative to GB
        for( sz_t i = 0; i < b->size; i++ ) gb->data[ i ] *= bmath_activation_p_dy( sr_act->p, sr_act->o, b->data[ i ] );

        bmath_mf3_s_mul_add_cps( false, gb, true,  w,  1.0,  NULL, 0, ga ); // GB * W^T (GA is folded) -> GA
        bmath_mf3_s_mul_add_cps( true,  a,  false, gb, o->adapt_step, w, wscale, w ); // W * wscale + ( A^T * GB ) * step -> W
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_adaptive_cnn_1d_s_decay( bmath_adaptive_cnn_1d_s* o, f3_t decay )
{
    f3_t f = ( 1.0 - decay );
    f = f3_max( 0, f );
    for( sz_t i = 0; i < o->arr_w.size; i++ ) bmath_mf3_s_mul_scl_f3( &o->arr_w.data[ i ], f, &o->arr_w.data[ i ] );
}

// ---------------------------------------------------------------------------------------------------------------------

f3_t bmath_adaptive_cnn_1d_s_query_1( bmath_adaptive_cnn_1d_s* o, const bmath_vf3_s* in )
{
    f3_t out_scl = 0;
    bmath_vf3_s out = bmath_vf3_weak( &out_scl, 1 );
    bmath_adaptive_cnn_1d_s_query( o, in, &out );
    return out_scl;
}

// ---------------------------------------------------------------------------------------------------------------------

f3_t bmath_adaptive_cnn_1d_s_adapt_1( bmath_adaptive_cnn_1d_s* o, const bmath_vf3_s* in, f3_t target )
{
    f3_t scl_out = 0;
    bmath_vf3_s vec_out    = bmath_vf3_weak( &scl_out, 1 );
    bmath_vf3_s vec_target = bmath_vf3_weak( &target, 1 );
    bmath_adaptive_cnn_1d_s_adapt( o, in, &vec_target, &vec_out );
    return scl_out;
}

// ---------------------------------------------------------------------------------------------------------------------

#endif // TYPEOF_bmath_adaptive_cnn

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

vd_t bmath_adaptive_cnn_1d_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_adaptive_cnn_1d" ) ) )
    {
        case TYPEOF_init1:
        {
        }
        break;

        case TYPEOF_get_quicktypes:
        {
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

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/


