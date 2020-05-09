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

#include "bhpt_tutor_language.h"

/**********************************************************************************************************************/

#ifdef TYPEOF_bhpt_tutor_language

bhpt_adaptive* bhpt_tutor_language_utf8_s_create_adaptive( const bhpt_tutor_language_utf8_s* o )
{
    BLM_INIT();
    bhpt_builder* builder = BLM_A_PUSH( bhpt_builder_a_clone( o->builder ) );
    bhpt_builder_a_set_format_en( builder, bhvm_holor_s_set_type_vector_vacant( BLM_CREATE( bhvm_holor_s ), TYPEOF_f3_t, 256 ) );
    bhpt_builder_a_set_format_ex( builder, bhvm_holor_s_set_type_vector_vacant( BLM_CREATE( bhvm_holor_s ), TYPEOF_f3_t, 256 ) );
    BLM_RETURNV( bhpt_adaptive*, bhpt_builder_a_create_adaptive( builder ) );
}

// ---------------------------------------------------------------------------------------------------------------------

static u2_t bhpt_tutor_language_utf8_s_get_string( bhpt_tutor_language_utf8_s* o, u2_t rval, sz_t size, st_s* string )
{
    if( !o->st )
    {
        BLM_INIT();
        o->st = st_s_create();
        bcore_source* source = NULL;

        switch( *(aware_t*)o->src )
        {
            case TYPEOF_bcore_file_path_s:
            {
                source = BLM_A_PUSH( bcore_file_open_source_path( ( const bcore_file_path_s* )o->src ) );
            }
            break;

            case TYPEOF_st_s:
            {
                source = BLM_A_PUSH( bcore_source_string_s_create_from_string( ( const st_s* )o->src ) );
            }
            break;

            default:
            {
                ERR_fa( "Invalid source type '#<sc_t>'.", ifnameof( *(aware_t*)o->src ) );
            }
            break;
        }

        while( !bcore_source_a_eos( source ) ) st_s_push_char( o->st, bcore_source_a_get_u0( source ) );

        ASSERT( o->st->size > 0 );

        BLM_DOWN();
    }

    sz_t index = f3_xsg1_pos( &rval ) * o->st->size;

    st_s_set_size( string, 0, size );
    for( sz_t i = 0; i < size; i++ ) string->data[ i ] = o->st->data[ ( i + index ) % o->st->size ];


    return rval;
}

// ---------------------------------------------------------------------------------------------------------------------

static void bhpt_tutor_language_utf8_s_encode( const bhpt_tutor_language_utf8_s* o, u0_t c, bhvm_value_s* v )
{
    assert( v->type == TYPEOF_f3_t );
    f3_t* v_data = v->data;
    BFOR_SIZE( i, v->size ) v_data[ i ] = ( i == c ) ? o->tgt_pos : o->tgt_neg;
}

// ---------------------------------------------------------------------------------------------------------------------

void bhpt_tutor_language_utf8_s_prime( bhpt_tutor_language_utf8_s* o, bhpt_adaptive* adaptive )
{
//    bcore_mutex_s_lock( &o->mutex );
    BLM_INIT();

    bhvm_holor_s* hx = bhpt_adaptive_a_get_format_en( adaptive, BLM_CREATE( bhvm_holor_s ) );
    bhvm_holor_s* hy = bhpt_adaptive_a_get_format_ex( adaptive, BLM_CREATE( bhvm_holor_s ) );
    bhvm_holor_s* hf = BLM_CLONE( bhvm_holor_s, hy );

    bhvm_holor_s_fit_size( hx );
    bhvm_holor_s_fit_size( hy );
    bhvm_holor_s_fit_size( hf );

    bcore_mutex_s_lock( &o->mutex );
    st_s* string = BLM_CREATE( st_s );
    o->rval_prime = bhpt_tutor_language_utf8_s_get_string( o, o->rval_prime, o->size_trans + o->size_prime + 1, string );
    bcore_mutex_s_unlock( &o->mutex );

    bhpt_adaptive_a_cyclic_reset( adaptive );

    sz_t index = 0;

    BFOR_SIZE( i, o->size_trans )
    {
        bhpt_tutor_language_utf8_s_encode( o, string->data[ index ], &hx->v );
        bhpt_adaptive_a_axon_pass( adaptive, hx, hf );
        index++;
    }

    BFOR_SIZE( i, o->size_prime )
    {
        bhpt_tutor_language_utf8_s_encode( o, string->data[ index     ], &hx->v );
        bhpt_tutor_language_utf8_s_encode( o, string->data[ index + 1 ], &hy->v );
        bhpt_adaptive_a_axon_pass( adaptive, hx, hf );
        bhvm_hop_ar2_eci_sub_s_f( hy, hf, hy );
        bhpt_adaptive_a_dendrite_pass( adaptive, hy, NULL );
        index++;
    }

    BLM_DOWN();
//    bcore_mutex_s_unlock( &o->mutex );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhpt_tutor_language_utf8_chatter_s_run( const bhpt_tutor_language_utf8_chatter_s* o, const bhpt_tutor_language_utf8_s* tutor, bhpt_adaptive* adaptive, bcore_sink* log )
{
    BLM_INIT();

    bhvm_holor_s* hx = bhpt_adaptive_a_get_format_en( adaptive, BLM_CREATE( bhvm_holor_s ) );
    bhvm_holor_s* hy = bhpt_adaptive_a_get_format_ex( adaptive, BLM_CREATE( bhvm_holor_s ) );
    bhvm_holor_s_zro( bhvm_holor_s_fit_size( hx ) );
    bhvm_holor_s_zro( bhvm_holor_s_fit_size( hy ) );

    if( o->cyclic_reset ) bhpt_adaptive_a_cyclic_reset( adaptive );

    bcore_sink_a_push_fa( log, "\n#rn{=}", o->size_line );

    sz_t c_count = 0;
    BFOR_SIZE( i, o->trigger.size )
    {
        if( c_count == 0 ) { bcore_sink_a_push_fa( log, "\n" ); }
        char c = o->trigger.data[ i ];
        bcore_sink_a_push_fa( log, "#<char>", c );
        bhpt_tutor_language_utf8_s_encode( tutor, c, &hx->v );
        bhpt_adaptive_a_axon_pass( adaptive, hx, hy );
        c_count = ( c_count + 1 ) % o->size_line;
    }

    u2_t rval = 12341234 * bhvm_value_s_get_max_f3( &hy->v );

    f3_t* hy_data = hy->v.data;

    sz_t size_text = sz_max( 0, o->size_lines * o->size_line - o->trigger.size );

    sz_t follow_bytes = 0;

    BFOR_SIZE( i, size_text )
    {
        if( c_count == 0 ) { bcore_sink_a_push_fa( log, "\n" ); }
        f3_t max_val = 0;
        u0_t c = 0;
        BFOR_SIZE( j, 256 )
        {
            f3_t v = hy_data[ j ] + f3_rnd_pos( &rval ) * o->heat;
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
            for( ; follow_bytes > 0; follow_bytes-- ) bcore_sink_a_push_fa( log, "#<char>", ( u0_t )0x80 );

            // simple ASCII
            if( c < 128 )
            {
                if( c == '\n' ) c = ' ';
                valid = ( c >= 32 && c < 127 );
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


        if( valid ) bcore_sink_a_push_fa( log, "#<char>", c );

        bhpt_tutor_language_utf8_s_encode( tutor, c, &hx->v );
        bhpt_adaptive_a_axon_pass( adaptive, hx, hy );
        c_count = ( c_count + 1 ) % o->size_line;
    }

    bcore_sink_a_push_fa( log, "\n#rn{=}\n", o->size_line );
    BLM_DOWN();
}

// ---------------------------------------------------------------------------------------------------------------------

void bhpt_tutor_language_utf8_s_test( bhpt_tutor_language_utf8_s* o, const bhpt_adaptive* adaptive_src, sz_t verbosity, bcore_sink* log )
{
    BLM_INIT();
    bhpt_adaptive* adaptive = BLM_A_PUSH( bhpt_adaptive_a_clone( adaptive_src ) );
    bhvm_holor_s* hx = bhpt_adaptive_a_get_format_en( adaptive, BLM_CREATE( bhvm_holor_s ) );
    bhvm_holor_s* hy = bhpt_adaptive_a_get_format_ex( adaptive, BLM_CREATE( bhvm_holor_s ) );

    bhvm_holor_s_zro( bhvm_holor_s_fit_size( hx ) );
    bhvm_holor_s_zro( bhvm_holor_s_fit_size( hy ) );
    bhvm_holor_s* hf  = BLM_CLONE( bhvm_holor_s, hy );
    bhvm_holor_s* hd  = BLM_CLONE( bhvm_holor_s, hy ); // y - f
    bhvm_holor_s* hsd = BLM_CLONE( bhvm_holor_s, hy ); // y - f

    bcore_mutex_s_lock( &( ( bhpt_tutor_language_utf8_s* )o )->mutex );
    st_s* string = BLM_CREATE( st_s );
    bhpt_tutor_language_utf8_s_get_string( ( bhpt_tutor_language_utf8_s* )o, o->rval_test, o->size_trans + o->size_test + 1, string );
    bcore_mutex_s_unlock( &( ( bhpt_tutor_language_utf8_s* )o )->mutex );

    bhpt_adaptive_a_cyclic_reset( adaptive );

    sz_t dimy = hy->v.size;

    f3_t sy  = 0;
    f3_t sd  = 0;

    f3_t sy2 = 0;
    f3_t sd2 = 0;

    sz_t index = 0;

    BFOR_SIZE( i, o->size_trans )
    {
        bhpt_tutor_language_utf8_s_encode( o, string->data[ index ], &hx->v );
        bhpt_adaptive_a_axon_pass( adaptive, hx, hf );
        index++;
    }

    BFOR_SIZE( i, o->size_test )
    {
        bhpt_tutor_language_utf8_s_encode( o, string->data[ index     ], &hx->v );
        bhpt_tutor_language_utf8_s_encode( o, string->data[ index + 1 ], &hy->v );
        bhpt_adaptive_a_axon_pass( adaptive, hx, hf );
        bhvm_hop_ar2_eci_sub_s_f( hy, hf, hd );

        sy  += bhvm_holor_s_sum( hy );
        sd  += bhvm_holor_s_sum( hd );
        sy2 += bhvm_holor_s_sum_sqr( hy );
        sd2 += bhvm_holor_s_sum_sqr( hd );

        bhvm_holor_s_acc( hsd, hd );

        index++;
    }

    sz_t n = ( o->size_test > 0 ) ? o->size_test * dimy : 1;

    f3_t ey = sy / n;
    f3_t ed = sd / n;

    f3_t ey2 = sy2 / n;
    f3_t ed2 = sd2 / n;

    /// variances
    f3_t vy = ey2 - f3_sqr( ey ); // variance of training data
    f3_t vd = ed2 - f3_sqr( ed ); // variance of output difference

    f3_t error = f3_srt(  vd * f3_inv( vy ) );

    f3_t eb2 = bhvm_holor_s_sum_sqr( hsd ) / ( n * n );

    f3_t bias  = f3_srt( eb2 * f3_inv( vy ) );

    if( verbosity > 1 )
    {
        if( o->chatter ) bhpt_tutor_language_utf8_chatter_s_run( o->chatter, o, adaptive, log );
        bcore_sink_a_pushf( log, "db-size: %5.3fM, ", o->st->size * 1E-6 );
    }

    if( verbosity > 0 ) bcore_sink_a_pushf( log, "err: %5.3f, bias: %7.5f", error, bias );

    BLM_DOWN();
}

// ---------------------------------------------------------------------------------------------------------------------

#endif // TYPEOF_bhpt_tutor_language

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

vd_t bhpt_tutor_language_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bhpt_tutor_language" ) ) )
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

