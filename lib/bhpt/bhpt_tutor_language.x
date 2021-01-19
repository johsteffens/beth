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

/** Training a language model. Intended for recurrent networks.
 */

/**********************************************************************************************************************/

include "bmath_std.h";
include "bhvm_std.h";

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

/** Generates automatic text from a trigger
 */
stamp :utf8_chatter_s = aware :
{
    bl_t cyclic_reset = false;
    st_s trigger    = "The chatter trigger:";
    sz_t size_line  = 100;
    sz_t size_lines = 4;
    bl_t newline_to_space = true;
    f3_t heat       = 0.2;
};

// ---------------------------------------------------------------------------------------------------------------------

/** UTF8 character tutor.
 *  Inspired by Andrey Karpathy's work on training language models with RNN:
 *  "The Unreasonable Effectiveness of Recurrent Neural Networks", May, 2015,
 *  http://karpathy.github.io/2015/05/21/rnn-effectiveness/
 */
stamp :utf8_s = aware bhpt_tutor
{
    aware bhpt_builder => builder;
    aware bhpt_adaptor => adaptor;

    aware x_inst => src; // text source (bcore_file_path_s or st_s with inline code)

    hidden st_s => st; // buffered text

    /// updated after priming
    aware bcore_prsg => prsg_priming = bcore_prsg_lcg_u3_00_s;

    /// stays constant (-> same sequence for each test)
    aware bcore_prsg => prsg_testing = bcore_prsg_lcg_u3_01_s;

    /** Transient size: Number of characters fed after cyclic reset in order to obtain
     *  a representative cyclic network state. Used for cyclic networks.
     */

    sz_t size_trans =   20; // transient size
    sz_t size_prime =  200; // prime samples
    sz_t size_test  = 1000; // test samples

    f3_t tgt_pos = 1.0;
    f3_t tgt_neg = 0.0;

    :utf8_chatter_s => chatter;

    hidden bcore_mutex_s mutex;

    func bhpt_tutor.reset = { o->prsg_priming =< bcore_prsg_lcg_u3_00_s!; };
    func bhpt_tutor.create_adaptive;
    func bhpt_tutor.create_adaptor = { return o.adaptor.clone(); };
    func bhpt_tutor.prime;
    func bhpt_tutor.test;
    func bhpt_tutor.status_to_sink =
    {
        if( verbosity > 0 )
        {
            bcore_sink_a_push_fa( sink, "#<sc_t>", ifnameof( o._ ) );
        }
    };
};

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

func (:utf8_s) bhpt_tutor.create_adaptive =
{
    m$* builder = o.builder.clone()^;
    builder.set_format_en( bhvm_holor_s!^.set_type_vector_vacant( TYPEOF_f3_t, 256 ) );
    builder.set_format_ex( bhvm_holor_s!^.set_type_vector_vacant( TYPEOF_f3_t, 256 ) );
    return builder.create_adaptive();
};

// ---------------------------------------------------------------------------------------------------------------------

func (:utf8_s) (void get_string( m@* o, m bcore_prsg* prsg, sz_t size, m st_s* string )) =
{
    if( !o->st )
    {
        o.st = st_s!;
        m bcore_source* source = NULL;

        switch( o.src._ )
        {
            case TYPEOF_bcore_file_path_s:
            {
                source = bcore_file_open_source_path( o.src.cast( c bcore_file_path_s* ) )^^;
            }
            break;

            case TYPEOF_st_s:
            {
                source = bcore_source_string_s_create_from_string( o.src.cast( c st_s* ) )^^;
            }
            break;

            default:
            {
                ERR_fa( "Invalid source type '#<sc_t>'.", ifnameof( o.src._ ) );
            }
            break;
        }

        while( !source.eos() ) st_s_push_char( o.st, source.get_char() );

        ASSERT( o.st.size > 0 );
    }

    sz_t index = prsg.gen_f3( 0, 1 ) * o.st.size;

    string.set_size( 0, size );
    for( sz_t i = 0; i < size; i++ ) string.[ i ] = o.st.[ ( i + index ) % o.st.size ];
};

// ---------------------------------------------------------------------------------------------------------------------

func (:utf8_s) (void encode( c bhpt_tutor_language_utf8_s* o, u0_t c, m bhvm_value_s* v )) =
{
    assert( v.type == TYPEOF_f3_t );
    m f3_t* v_data = v.data.cast( m f3_t* );
    for( sz_t i = 0; i < v.size; i++ ) v_data[ i ] = ( i == c ) ? o.tgt_pos : o.tgt_neg;
};

// ---------------------------------------------------------------------------------------------------------------------

func (:utf8_s) bhpt_tutor.prime =
{
    m bhvm_holor_s* hx = adaptive.get_format_en( bhvm_holor_s!^ );
    m bhvm_holor_s* hy = adaptive.get_format_ex( bhvm_holor_s!^ );
    m bhvm_holor_s* hf = hy.clone()^;

    hx.fit_size();
    hy.fit_size();
    hf.fit_size();

    o.mutex.lock();
    st_s^ string;
    o.get_string( o.prsg_priming, o.size_trans + o.size_prime + 1, string );
    o.mutex.unlock();

    adaptive.cyclic_reset();

    sz_t index = 0;

    for( sz_t i = 0; i < o->size_trans; i++ )
    {
        o.encode( string.[ index ], hx.v );
        adaptive.axon_pass( hx, hf );
        index++;
    }

    for( sz_t i = 0; i < o.size_prime; i++ )
    {
        o.encode( string.[ index     ], hx.v );
        o.encode( string.[ index + 1 ], hy.v );
        adaptive.axon_pass( hx, hf );
        bhvm_hop_ar2_eci_sub_s_f( hy, hf, hy );
        adaptive.dendrite_pass( hy, NULL );
        index++;
    }
};

// ---------------------------------------------------------------------------------------------------------------------

func (:utf8_chatter_s) (void run( c@* o, c :utf8_s* tutor, m bhpt_adaptive* adaptive, m bcore_sink* log )) =
{
    m bhvm_holor_s* hx = adaptive.get_format_en( bhvm_holor_s!^ );
    m bhvm_holor_s* hy = adaptive.get_format_ex( bhvm_holor_s!^ );
    hx.fit_size().zro();
    hy.fit_size().zro();

    if( o.cyclic_reset ) adaptive.cyclic_reset();

    log.push_fa( "\n#rn{=}", o.size_line );

    sz_t c_count = 0;
    for( sz_t i = 0; i < o->trigger.size; i++ )
    {
        if( c_count == 0 ) log.push_fa( "\n" );
        char c = o.trigger.[ i ];
        log.push_fa( "#<char>", c );
        tutor.encode( c, hx.v );
        adaptive.axon_pass( hx, hy );
        c_count++;
    }

    c_count = c_count % o.size_line;

    u3_t rval = 12341234 * hy.v.get_max_f3();

    m f3_t* hy_data = hy.v.data.cast( m f3_t* );

    sz_t size_text = sz_max( 0, o.size_lines * o.size_line - o.trigger.size );

    sz_t follow_bytes = 0;

    for( sz_t i = 0; i < size_text; i++ )
    {
        if( c_count == 0 ) log.push_fa( "\n" );
        f3_t max_val = 0;
        u0_t c = 0;
        for( sz_t j = 0; j < 256; j++ )
        {
            f3_t v = hy_data[ j ] + f3_rnd_pos( rval.1 ) * o.heat;
            if( v >= max_val )
            {
                max_val = v;
                c = j;
            }
        }

        bl_t valid = true;

        if( c < 128 || ( c & 0xC0 ) == 0xC0 ) // ASCII char or start of a new sequence
        {
            /// clear previous sequence
            for( ; follow_bytes > 0; follow_bytes-- ) log.push_fa( "#<char>", ( u0_t )0x80 );

            // simple ASCII
            if( c < 128 )
            {
                if( c == '\n' && o.newline_to_space ) c = ' ';
                valid = ( c == '\n' ) || ( c >= 32 && c < 127 );
            }
            else if( ( c & 0xE0 ) == 0xC0 ) // 2-byte sequence
            {
                follow_bytes = 1;
            }
            else if( ( c & 0xF0 ) == 0xE0 ) // 3-byte sequence
            {
                follow_bytes = 2;
            }
            else if( ( c & 0xF8 ) == 0xF0 ) // 4-byte sequence
            {
                follow_bytes = 3;
            }
            else
            {
                valid = false;
            }
        }
        else if( ( c & 0xC0 ) == 0x80 ) // follow byte
        {
            if( follow_bytes > 0 )
            {
                follow_bytes--;
            }
            else
            {
                valid = false;
            }
        }


        if( valid ) log.push_fa( "#<char>", c );

        tutor.encode( c, &hx->v );
        adaptive.axon_pass( hx, hy );
        c_count = ( c_count + 1 ) % o->size_line;

        if( c == '\n' ) c_count = 1;
    }

    log.push_fa( "\n#rn{=}\n", o.size_line );
};

// ---------------------------------------------------------------------------------------------------------------------

func (:utf8_s) bhpt_tutor.test =
{
    m bhpt_adaptive* adaptive_test = adaptive.clone()^;
    m bhvm_holor_s* hx = adaptive_test.get_format_en( bhvm_holor_s!^ );
    m bhvm_holor_s* hy = adaptive_test.get_format_ex( bhvm_holor_s!^ );

    hx.fit_size().zro();
    hy.fit_size().zro();
    m bhvm_holor_s* hf  = hy.clone()^;
    m bhvm_holor_s* hd  = hy.clone()^; // y - f
    m bhvm_holor_s* hsd = hy.clone()^; // y - f

    m bcore_prsg* prsg = o.prsg_testing.clone()^;

    st_s^ string;
    o.cast( m$* ).mutex.lock();
    o.cast( m$* ).get_string( prsg, o.size_trans + o.size_test + 1, string );
    o.cast( m$* ).mutex.unlock();

    adaptive_test.cyclic_reset();

    sz_t dimy = hy.v.size;

    f3_t sy  = 0;
    f3_t sd  = 0;

    f3_t sy2 = 0;
    f3_t sd2 = 0;

    sz_t index = 0;

    for( sz_t i = 0; i < o.size_trans; i++ )
    {
        o.encode( string.[ index ], hx.v );
        adaptive_test.axon_pass( hx, hf );
        index++;
    }

    for( sz_t i = 0; i < o.size_test; i++ )
    {
        o.encode( string.[ index     ], hx.v );
        o.encode( string.[ index + 1 ], hy.v );
        adaptive_test.axon_pass( hx, hf );
        bhvm_hop_ar2_eci_sub_s_f( hy, hf, hd );

        sy  += hy.sum();
        sd  += hd.sum();
        sy2 += hy.sum_sqr();
        sd2 += hd.sum_sqr();

        hsd.acc( hd );

        index++;
    }

    sz_t n = ( o.size_test > 0 ) ? o.size_test * dimy : 1;

    f3_t ey = sy / n;
    f3_t ed = sd / n;

    f3_t ey2 = sy2 / n;
    f3_t ed2 = sd2 / n;

    /// variances
    f3_t vy = ey2 - f3_sqr( ey ); // variance of training data
    f3_t vd = ed2 - f3_sqr( ed ); // variance of output difference

    f3_t error = f3_srt(  vd * f3_inv( vy ) );

    f3_t eb2 = bhvm_holor_s_sum_sqr( hsd ) / ( n * n );

    f3_t bias = f3_srt( eb2 * f3_inv( vy ) );

    if( verbosity > 1 )
    {
        if( o.chatter ) o.chatter.run( o, adaptive_test, log );
        log.pushf( "db-size: %5.3fM, ", o.st.size * 1E-6 );
    }

    if( verbosity > 0 ) log.pushf( "err: %5.3f, bias: %7.5f", error, bias );
};

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
