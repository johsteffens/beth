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

 /** Special class of supervising tutors based on a sampling function
  *  generating (random) samples of training and testing pairs.
  */

/**********************************************************************************************************************/

include "bmath_std.h";
include "bhvm_std.h";

/**********************************************************************************************************************/

feature strict void fetch( c @* o, m bcore_prsg* prsg, m bhvm_value_s* en, m bhvm_value_s* ex );
feature strict sz_t get_size_en( c @* o );
feature strict sz_t get_size_ex( c @* o );

// ---------------------------------------------------------------------------------------------------------------------

/** Type: Classifier
 *  Problem: Distinguish between a sine wave of arbitrary amplitude and frequency from a random walk curve.
 */
stamp :sine_random_s = aware :
{
    sz_t size_en = 32;
    f3_t pos_tgt =  0.9;
    f3_t neg_tgt = -0.9;

    func :.get_size_en = { return o.size_en; };
    func :.get_size_ex = { return 1; };
    func :.fetch =
    {
        ASSERT( en.size == o->size_en );
        ASSERT( ex.size == 1 );
        ASSERT( en.type == TYPEOF_f3_t );
        ASSERT( ex.type == TYPEOF_f3_t );

        bl_t pos = prsg.gen_bl();

        m f3_t* en_data = en.data.cast( m f3_t* );
        m f3_t* ex_data = ex.data.cast( m f3_t* );

        if( pos )
        {
            f3_t omega = 1.0 * f3_pi() * prsg.gen_f3( 0, 1 );
            f3_t amplitude = 4.0 * prsg.gen_f3( 0, 1 );

            for( sz_t i = 0; i < o.size_en; i++ )
            {
                f3_t vp = sin( omega * i ) * amplitude;
                en_data[ i ] = vp;
            }
            ex_data[ 0 ] = o.pos_tgt;
        }
        else
        {
            f3_t rwalker = prsg.gen_f3( -1, 1 );
            for( sz_t i = 0; i < o.size_en; i++ )
            {
                rwalker += prsg.gen_f3( -1, 1 );
                f3_t vn = rwalker;
                en_data[ i ] = vn;
            }
            ex_data[ 0 ] = o.neg_tgt;
        }
    };
};

// ---------------------------------------------------------------------------------------------------------------------

/** Type: Encoder
 *  Problem: Add two unsigned integer variables.
 */
stamp :binary_add_s = aware :
{
    sz_t bits  = 4; // input size = bits * 2; output size = bits + 1
    f3_t val_h =  0.9;
    f3_t val_l = -0.9;

    func :.get_size_en = { return o.bits * 2; };
    func :.get_size_ex = { return o.bits + 1; };
    func :.fetch =
    {
        ASSERT( en.size == o.bits * 2 );
        ASSERT( ex.size == o.bits + 1 );
        ASSERT( en.type == TYPEOF_f3_t );
        ASSERT( ex.type == TYPEOF_f3_t );

        m f3_t* en_data = en.data.cast( m f3_t* );
        m f3_t* ex_data = ex.data.cast( m f3_t* );

        u3_t v1 = prsg.gen_bits_u3( o.bits );
        u3_t v2 = prsg.gen_bits_u3( o.bits );
        u3_t vo = v1 + v2;

        for( sz_t i = 0; i < o.bits; i++ )
        {
            en_data[ i          ] = ( ( v1 & ( 1 << i ) ) != 0 ) ? o.val_h : o.val_l;
            en_data[ i + o.bits ] = ( ( v2 & ( 1 << i ) ) != 0 ) ? o.val_h : o.val_l;
        }

        for( sz_t i = 0; i <= o.bits; i++ )
        {
            ex_data[ i ] = ( ( vo & ( 1 << i ) ) != 0 ) ? o.val_h : o.val_l;
        }
    };
};

// ---------------------------------------------------------------------------------------------------------------------

/** Type: Encoder
 *  Problem: Multiply two unsigned integer variables.
 */
stamp :binary_mul_s = aware :
{
    sz_t bits  = 4; // input size = bits * 2; output size = bits * 2
    f3_t val_h =  0.9;
    f3_t val_l = -0.9;

    func :.get_size_en = { return o.bits * 2; };
    func :.get_size_ex = { return o.bits * 2; };
    func :.fetch =
    {
        ASSERT( en.size == o.bits * 2 );
        ASSERT( ex.size == o.bits * 2 );
        ASSERT( en.type == TYPEOF_f3_t );
        ASSERT( ex.type == TYPEOF_f3_t );

        m f3_t* en_data = en.data.cast( m f3_t* );
        m f3_t* ex_data = ex.data.cast( m f3_t* );

        u3_t v1 = prsg.gen_bits_u3( o.bits );
        u3_t v2 = prsg.gen_bits_u3( o.bits );
        u3_t vo = v1 * v2;

        for( sz_t i = 0; i < o.bits; i++ )
        {
            en_data[ i          ] = ( ( v1 & ( 1 << i ) ) != 0 ) ? o.val_h : o.val_l;
            en_data[ i + o.bits ] = ( ( v2 & ( 1 << i ) ) != 0 ) ? o.val_h : o.val_l;
        }

        for( sz_t i = 0; i < ex.size; i++ )
        {
            ex_data[ i ] = ( ( vo & ( 1 << i ) ) != 0 ) ? o.val_h : o.val_l;
        }
    };
};

// ---------------------------------------------------------------------------------------------------------------------

/** Type: Encoder
 *  Problem: Forward randomization.
 */
stamp :binary_lcg00_s = aware :
{
    sz_t bits  = 4; // input size = bits; output size = bits
    f3_t val_h =  0.9;
    f3_t val_l = -0.9;

    func :.get_size_en = { return o.bits; };
    func :.get_size_ex = { return o.bits; };
    func :.fetch =
    {
        ASSERT( en.size == o.bits );
        ASSERT( ex.size == o.bits );
        ASSERT( en.type == TYPEOF_f3_t );
        ASSERT( ex.type == TYPEOF_f3_t );

        m f3_t* en_data = en.data.cast( m f3_t* );
        m f3_t* ex_data = ex.data.cast( m f3_t* );

        u3_t vi = prsg.gen_bits_u3( o.bits );
        u3_t vo = bcore_lcg00_u3( vi ) & ( ( 1 << o.bits ) - 1 );

        for( sz_t i = 0; i < en.size; i++ )
        {
            en_data[ i ] = ( ( vi & ( 1 << i ) ) != 0 ) ? o.val_h : o.val_l;
        }

        for( sz_t i = 0; i < ex.size; i++ )
        {
            ex_data[ i ] = ( ( vo & ( 1 << i ) ) != 0 ) ? o.val_h : o.val_l;
        }
    };
};

// ---------------------------------------------------------------------------------------------------------------------

/** Type: Encoder
 *  Problem: (Reverse) Hashing.
 */
stamp :binary_hash_s = aware :
{
    sz_t bits  = 4; // input size = bits; output size = bits
    f3_t val_h =  0.9;
    f3_t val_l = -0.9;
    bl_t reverse = false;
    func :.get_size_en = { return o.bits; };
    func :.get_size_ex = { return o.bits; };
    func :.fetch =
    {
        ASSERT( en.size == o.bits );
        ASSERT( ex.size == o.bits );
        ASSERT( en.type == TYPEOF_f3_t );
        ASSERT( ex.type == TYPEOF_f3_t );

        m f3_t* en_data = en.data.cast( m f3_t* );
        m f3_t* ex_data = ex.data.cast( m f3_t* );

        tp_t vi = prsg.gen_bits_u3( o.bits );
        tp_t vo = bcore_tp_fold_tp( bcore_tp_init(), vi ) & ( ( 1 << o.bits ) - 1 );

        if( o.reverse ) tp_t_swap( &vi, &vo );

        for( sz_t i = 0; i < en.size; i++ )
        {
            en_data[ i ] = ( ( vi & ( 1 << i ) ) != 0 ) ? o.val_h : o.val_l;
        }

        for( sz_t i = 0; i < ex.size; i++ )
        {
            ex_data[ i ] = ( ( vo & ( 1 << i ) ) != 0 ) ? o.val_h : o.val_l;
        }
    };
};

// ---------------------------------------------------------------------------------------------------------------------

/** Type: Regression
 *  Problem: Estimate polynomial coefficients for a signal within range -1, 1.
 */
stamp :polynom_s = aware :
{
    sz_t size_en = 32;
    sz_t size_ex = 3;   // polynomial order + 1
    f3_t range   = 1.0; // +/- range of coefficients
    f3_t noise_level = 0;   // additive noise to input signal
    func :.get_size_en = { return o.size_en; };
    func :.get_size_ex = { return o.size_ex; };
    func :.fetch =
    {
        ASSERT( en.size == o.size_en );
        ASSERT( ex.size == o.size_ex );
        ASSERT( en.type == TYPEOF_f3_t );
        ASSERT( ex.type == TYPEOF_f3_t );

        m f3_t* en_data = en.data.cast( m f3_t* );
        m f3_t* ex_data = ex.data.cast( m f3_t* );

        for( sz_t i = 0; i < o.size_ex; i++ ) ex_data[ i ] = prsg.gen_f3( -1, 1 );
        for( sz_t i = 0; i < o.size_en; i++ )
        {
            f3_t x1 = 2.0 * ( ( ( f3_t )i / ( o.size_en - 1 ) ) - 0.5 );
            f3_t x = 1.0;
            f3_t y = 0;
            for( sz_t i = 0; i < o.size_ex; i++ )
            {
                y += x * ex_data[ i ] * o.range;
                x *= x1;
            }

            en_data[ i ] = y + o.noise_level * prsg.gen_f3( -1, 1 );
        }
    };
};

// ---------------------------------------------------------------------------------------------------------------------

stamp bhpt_tutor_sampler_s = aware bhpt_tutor
{
    aware bhpt_builder => builder;
    aware bhpt_adaptor => adaptor;
    aware bhpt_sampler => sampler;

    /// updated after priming
    aware bcore_prsg => prsg_priming = bcore_prsg_lcg_u3_00_s;

    /// stays constant (-> same sequence for each test)
    aware bcore_prsg => prsg_testing = bcore_prsg_lcg_u3_01_s;

    sz_t test_size = 1000;

    hidden bcore_mutex_s mutex;

    func bhpt_tutor.reset = {};

    func bhpt_tutor.create_adaptive =
    {
        m bhpt_builder* builder = o.builder.clone()^;
        builder.set_format_en( bhvm_holor_s!^.set_type_vector_vacant( TYPEOF_f3_t, o.sampler.get_size_en() ) );
        builder.set_format_ex( bhvm_holor_s!^.set_type_vector_vacant( TYPEOF_f3_t, o.sampler.get_size_ex() ) );
        return builder.create_adaptive();
    };

    func bhpt_tutor.create_adaptor = { return o.adaptor.clone(); };
    func bhpt_tutor.test;
    func bhpt_tutor.status_to_sink =
    {
        if( verbosity > 0 )
        {
            sink.push_fa( "#<sc_t> : #<sc_t>", ifnameof( o._ ), ifnameof( o.sampler ? o.sampler._ : 0 ) );
        }
    };

    func bhpt_tutor.prime =
    {
        m bhvm_holor_s* hx = adaptive.get_format_en( bhvm_holor_s!^ ).fit_size();
        m bhvm_holor_s* hy = adaptive.get_format_ex( bhvm_holor_s!^ ).fit_size();
        m bhvm_holor_s* hf = hy.clone()^.fit_size();
        o.mutex.lock();
        o.sampler.fetch( o.prsg_priming, hx.v, hy.v );
        o.mutex.unlock();
        adaptive.axon_pass( hx, hf );
        bhvm_hop_ar2_eci_sub_s_f( hy, hf, hy );
        adaptive.dendrite_pass( hy, NULL );
    };
};

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

func (bhpt_tutor_sampler_s) bhpt_tutor.test =
{
    m bhpt_adaptive* adaptive_test = adaptive.clone()^;
    m bhvm_holor_s* hx = adaptive_test.get_format_en( bhvm_holor_s!^ ).fit_size().zro();
    m bhvm_holor_s* hy = adaptive_test.get_format_ex( bhvm_holor_s!^ ).fit_size().zro();
    m bhvm_holor_s* hf  = hy.clone()^;
    m bhvm_holor_s* hd  = hy.clone()^; // y - f
    m bhvm_holor_s* hsd = hy.clone()^;

    sz_t dimy = hy->v.size;

    f3_t sy  = 0;
    f3_t sd  = 0;

    f3_t sy2 = 0;
    f3_t sd2 = 0;

    m bcore_prsg* prsg = o.prsg_testing.clone()^;

    for( sz_t i = 0; i < o.test_size; i++ )
    {
        o.sampler.fetch( prsg, hx.v, hy.v );
        adaptive_test.axon_pass( hx, hf );
        bhvm_hop_ar2_eci_sub_s_f( hy, hf, hd );

        sy  += hy.sum();
        sd  += hd.sum();
        sy2 += hy.sum_sqr();
        sd2 += hd.sum_sqr();

        hsd.acc( hd );
    }

    sz_t n = ( o.test_size > 0 ) ? o.test_size * dimy : 1;

    f3_t ey = sy / n;
    f3_t ed = sd / n;

    f3_t ey2 = sy2 / n;
    f3_t ed2 = sd2 / n;

    /// variances
    f3_t vy = ey2 - f3_sqr( ey ); // variance of training data
    f3_t vd = ed2 - f3_sqr( ed ); // variance of output difference

    f3_t error = f3_srt(  vd * f3_inv( vy ) );

    f3_t eb2 = hsd.sum_sqr() / ( n * n );

    f3_t bias = f3_srt( eb2 * f3_inv( vy ) );

    if( verbosity > 0 )
    {
        log.pushf( "err: %5.3f, bias: %7.5f", error, bias );
    }
};

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

