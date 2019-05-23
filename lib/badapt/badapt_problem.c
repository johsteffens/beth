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

#include "badapt_problem.h"
#include "badapt_loss.h"

/**********************************************************************************************************************/
// badapt_problem_sine_random_s

//----------------------------------------------------------------------------------------------------------------------

sz_t badapt_problem_sine_random_s_get_in_size( const badapt_problem_sine_random_s* o ) { return o->input_size; }
sz_t badapt_problem_sine_random_s_get_out_size( const badapt_problem_sine_random_s* o ) { return 1; }
const badapt_loss* badapt_problem_sine_random_s_preferred_loss( const badapt_problem_sine_random_s* o ) { return o->preferred_loss; }

//----------------------------------------------------------------------------------------------------------------------

void badapt_problem_sine_random_s_fetch_sample_tio( badapt_problem_sine_random_s* o, bmath_vf3_s* in, bmath_vf3_s* out )
{
    o->rval = bcore_xsg1_u2( o->rval );
    bl_t pos = ( o->rval & 1 ) == 0; // pos vs neg sample
    o->rval = bcore_xsg1_u2( o->rval );
    bmath_vf3_s_set_size( in,  o->input_size );
    bmath_vf3_s_set_size( out, 1 );

    f3_t omega = 1.0 * f3_pi() * f3_rnd_pos( &o->rval );
    f3_t amplitude = 4.0 * f3_rnd_pos( &o->rval );

    if( pos )
    {
        for( sz_t i = 0; i < o->input_size; i++ )
        {
            f3_t vp = sin( omega * i ) * amplitude;
            in->data[ i ] = vp;
        }
        out->data[ 0 ] = o->pos_tgt;
    }
    else
    {
        f3_t rwalker = f3_rnd_sym( &o->rval );
        for( sz_t i = 0; i < o->input_size; i++ )
        {
            rwalker += f3_rnd_sym( &o->rval );
            f3_t vn = rwalker;
            in->data[ i ] = vn;
        }
        out->data[ 0 ] = o->neg_tgt;
    }
}

//----------------------------------------------------------------------------------------------------------------------

void badapt_problem_sine_random_s_fetch_sample_vio( badapt_problem_sine_random_s* o, bmath_vf3_s* in, bmath_vf3_s* out )
{
    /// no need to separate between batch and valid
    badapt_problem_sine_random_s_fetch_sample_tio( o, in, out );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// badapt_problem_binary_add_s

//----------------------------------------------------------------------------------------------------------------------

sz_t  badapt_problem_binary_add_s_get_in_size(  const badapt_problem_binary_add_s* o ) { return o->bits * 2; }
sz_t  badapt_problem_binary_add_s_get_out_size( const badapt_problem_binary_add_s* o ) { return o->bits + 1; }
const badapt_loss* badapt_problem_binary_add_s_preferred_loss( const badapt_problem_binary_add_s* o ) { return o->preferred_loss; }

//----------------------------------------------------------------------------------------------------------------------

void badapt_problem_binary_add_s_fetch_sample_tio( badapt_problem_binary_add_s* o, bmath_vf3_s* in, bmath_vf3_s* out )
{
    bmath_vf3_s_set_size( in,  o->bits * 2 );
    bmath_vf3_s_set_size( out, o->bits + 1 );

    u2_t v1 = ( o->rval = bcore_xsg1_u2( o->rval ) ) & ( ( 1 << o->bits ) - 1 );
    u2_t v2 = ( o->rval = bcore_xsg1_u2( o->rval ) ) & ( ( 1 << o->bits ) - 1 );
    u2_t vo = v1 + v2;

    for( sz_t i = 0; i < o->bits; i++ )
    {
        in->data[ i           ] = ( ( v1 & ( 1 << i ) ) != 0 ) ? 1.0 : -1.0;
        in->data[ i + o->bits ] = ( ( v2 & ( 1 << i ) ) != 0 ) ? 1.0 : -1.0;
    }

    for( sz_t i = 0; i <= o->bits; i++ )
    {
        out->data[ i ] = ( ( vo & ( 1 << i ) ) != 0 ) ? 1.0 : -1.0;
    }
}

//----------------------------------------------------------------------------------------------------------------------

void badapt_problem_binary_add_s_fetch_sample_vio( badapt_problem_binary_add_s* o, bmath_vf3_s* in, bmath_vf3_s* out )
{
    /// no need to separate between batch and valid
    badapt_problem_binary_add_s_fetch_sample_tio( o, in, out );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// badapt_problem_binary_mul_s

//----------------------------------------------------------------------------------------------------------------------

sz_t  badapt_problem_binary_mul_s_get_in_size(  const badapt_problem_binary_mul_s* o ) { return o->bits * 2; }
sz_t  badapt_problem_binary_mul_s_get_out_size( const badapt_problem_binary_mul_s* o ) { return o->bits * 2; }
const badapt_loss* badapt_problem_binary_mul_s_preferred_loss( const badapt_problem_binary_mul_s* o ) { return o->preferred_loss; }

//----------------------------------------------------------------------------------------------------------------------

void badapt_problem_binary_mul_s_fetch_sample_tio( badapt_problem_binary_mul_s* o, bmath_vf3_s* in, bmath_vf3_s* out )
{
    bmath_vf3_s_set_size( in,  o->bits * 2 );
    bmath_vf3_s_set_size( out, o->bits * 2 );

    u2_t v1 = ( o->rval = bcore_xsg1_u2( o->rval ) ) & ( ( 1 << o->bits ) - 1 );
    u2_t v2 = ( o->rval = bcore_xsg1_u2( o->rval ) ) & ( ( 1 << o->bits ) - 1 );
    u2_t vo = v1 * v2;

    for( sz_t i = 0; i < o->bits; i++ )
    {
        in->data[ i           ] = ( ( v1 & ( 1 << i ) ) != 0 ) ? 1.0 : -1.0;
        in->data[ i + o->bits ] = ( ( v2 & ( 1 << i ) ) != 0 ) ? 1.0 : -1.0;
    }

    for( sz_t i = 0; i < out->size; i++ )
    {
        out->data[ i ] = ( ( vo & ( 1 << i ) ) != 0 ) ? 1.0 : -1.0;
    }
}

//----------------------------------------------------------------------------------------------------------------------

void badapt_problem_binary_mul_s_fetch_sample_vio( badapt_problem_binary_mul_s* o, bmath_vf3_s* in, bmath_vf3_s* out )
{
    /// no need to separate between batch and valid
    badapt_problem_binary_mul_s_fetch_sample_tio( o, in, out );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// badapt_problem_binary_xsg3_s

//----------------------------------------------------------------------------------------------------------------------

sz_t  badapt_problem_binary_xsg3_s_get_in_size(  const badapt_problem_binary_xsg3_s* o ) { return o->bits; }
sz_t  badapt_problem_binary_xsg3_s_get_out_size( const badapt_problem_binary_xsg3_s* o ) { return o->bits; }
const badapt_loss* badapt_problem_binary_xsg3_s_preferred_loss( const badapt_problem_binary_xsg3_s* o ) { return o->preferred_loss; }

//----------------------------------------------------------------------------------------------------------------------

void badapt_problem_binary_xsg3_s_fetch_sample_tio( badapt_problem_binary_xsg3_s* o, bmath_vf3_s* in, bmath_vf3_s* out )
{
    bmath_vf3_s_set_size( in,  o->bits );
    bmath_vf3_s_set_size( out, o->bits );

    u2_t vi = ( o->rval = bcore_xsg1_u2( o->rval ) ) & ( ( 1 << o->bits ) - 1 );
    u2_t vo = bcore_xsg3_u2( vi ) & ( ( 1 << o->bits ) - 1 );

    for( sz_t i = 0; i < o->bits; i++ )
    {
        in->data[ i ] = ( ( vi & ( 1 << i ) ) != 0 ) ? 1.0 : -1.0;
    }

    for( sz_t i = 0; i < out->size; i++ )
    {
        out->data[ i ] = ( ( vo & ( 1 << i ) ) != 0 ) ? 1.0 : -1.0;
    }
}

//----------------------------------------------------------------------------------------------------------------------

void badapt_problem_binary_xsg3_s_fetch_sample_vio( badapt_problem_binary_xsg3_s* o, bmath_vf3_s* in, bmath_vf3_s* out )
{
    /// no need to separate between batch and valid
    badapt_problem_binary_xsg3_s_fetch_sample_tio( o, in, out );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// badapt_problem_binary_hash_s

//----------------------------------------------------------------------------------------------------------------------

sz_t  badapt_problem_binary_hash_s_get_in_size(  const badapt_problem_binary_hash_s* o ) { return o->bits; }
sz_t  badapt_problem_binary_hash_s_get_out_size( const badapt_problem_binary_hash_s* o ) { return o->bits; }
const badapt_loss* badapt_problem_binary_hash_s_preferred_loss( const badapt_problem_binary_hash_s* o ) { return o->preferred_loss; }

//----------------------------------------------------------------------------------------------------------------------

void badapt_problem_binary_hash_s_fetch_sample_tio( badapt_problem_binary_hash_s* o, bmath_vf3_s* in, bmath_vf3_s* out )
{
    bmath_vf3_s_set_size( in,  o->bits );
    bmath_vf3_s_set_size( out, o->bits );

    tp_t vi = ( o->rval = bcore_xsg1_u2( o->rval ) ) & ( ( 1 << o->bits ) - 1 );
    tp_t vo = bcore_tp_fold_tp( bcore_tp_init(), vi ) & ( ( 1 << o->bits ) - 1 );

    if( o->reverse ) tp_t_swap( &vi, &vo );

    for( sz_t i = 0; i < o->bits; i++ )
    {
        in->data[ i ] = ( ( vi & ( 1 << i ) ) != 0 ) ? 1.0 : -1.0;
    }

    for( sz_t i = 0; i < out->size; i++ )
    {
        out->data[ i ] = ( ( vo & ( 1 << i ) ) != 0 ) ? 1.0 : -1.0;
    }
}

//----------------------------------------------------------------------------------------------------------------------

void badapt_problem_binary_hash_s_fetch_sample_vio( badapt_problem_binary_hash_s* o, bmath_vf3_s* in, bmath_vf3_s* out )
{
    /// no need to separate between batch and valid
    badapt_problem_binary_hash_s_fetch_sample_tio( o, in, out );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// badapt_problem_polynom_s

//----------------------------------------------------------------------------------------------------------------------

sz_t  badapt_problem_polynom_s_get_in_size(  const badapt_problem_polynom_s* o ) { return o->input_size; }
sz_t  badapt_problem_polynom_s_get_out_size( const badapt_problem_polynom_s* o ) { return o->output_size; }
const badapt_loss* badapt_problem_polynom_s_preferred_loss( const badapt_problem_polynom_s* o ) { return o->preferred_loss; }

//----------------------------------------------------------------------------------------------------------------------

void badapt_problem_polynom_s_fetch_sample_tio( badapt_problem_polynom_s* o, bmath_vf3_s* in, bmath_vf3_s* out )
{
    bmath_vf3_s_set_size( in,  o->input_size );
    bmath_vf3_s_set_size( out, o->output_size );

    for( sz_t i = 0; i < o->output_size; i++ ) out->data[ i ] = f3_xsg1_sym( &o->rval );
    for( sz_t i = 0; i < o->input_size;  i++ )
    {
        f3_t x1 = 2.0 * ( ( ( f3_t )i / ( o->input_size - 1 ) ) - 0.5 );
        f3_t x = 1.0;
        f3_t y = 0;
        for( sz_t i = 0; i < o->output_size; i++ )
        {
            y += x * out->data[ i ] * o->range;
            x *= x1;
        }

        in->data[ i ] = y + o->noise_level * f3_xsg1_sym( &o->rval );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void badapt_problem_polynom_s_fetch_sample_vio( badapt_problem_polynom_s* o, bmath_vf3_s* in, bmath_vf3_s* out )
{
    /// no need to separate between batch and valid
    badapt_problem_polynom_s_fetch_sample_tio( o, in, out );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

vd_t badapt_problem_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "badapt_problem" ) ) )
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

