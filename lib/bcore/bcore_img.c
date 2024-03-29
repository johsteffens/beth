/** Author and Copyright 2018 Johannes Bernhard Steffens
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

#include "bcore_img.h"
#include "bcore_life.h"
#include "bcore_signal.h"
#include "bcore_spect_inst.h"
#include "bcore_spect_array.h"
#include "bcore_spect_compare.h"
#include "bcore_sr.h"
#include "bcore_signal.h"
#include "bcore_sinks.h"
#include "bcore_sources.h"

/**********************************************************************************************************************/

BCORE_DEFINE_OBJECT_INST( bcore_matrix, bcore_img_u2_s )
"{"
    "aware_t _;"
    "tp_t format;"
    "uz_t rows;"
    "uz_t cols;"
    "uz_t stride;"
    "u2_t [] arr;"
"}";

/**********************************************************************************************************************/

void bcore_img_u2_s_set_size( bcore_img_u2_s* o, uz_t rows, uz_t cols )
{
    bcore_array_a_set_size( ( bcore_array* )o, rows * cols );
    bcore_u_memzero( sizeof( u2_t ), o->data, o->size );
    o->rows = rows;
    o->cols = cols;
    o->stride = cols;

    if( !o->format ) o->format = TYPEOF_bcore_img_u2_argb;
}

// ---------------------------------------------------------------------------------------------------------------------

const bcore_img_u2_s* bcore_img_u2_s_pnm_to_sink( const bcore_img_u2_s* o, bcore_sink* snk )
{
    bcore_sink_a_push_fa( snk, "P6\n#<uz_t> #<uz_t>\n255\n", o->cols, o->rows );
    for( sz_t i = 0; i < o->rows; i++ )
    {
        for( sz_t j = 0; j < o->cols; j++ )
        {
            u0_t rgb[ 3 ];
            bcore_img_u2_s_get_rgb( o, i, j, &rgb[ 0 ], &rgb[ 1 ], &rgb[ 2 ] );
            bcore_sink_a_push_data( snk, rgb, 3 );
        }
    }
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

const bcore_img_u2_s* bcore_img_u2_s_pnm_to_file( const bcore_img_u2_s* o, sc_t file )
{
    bcore_sink* snk = ( bcore_sink* )bcore_sink_open_file( file );
    bcore_img_u2_s_pnm_to_sink( o, snk );
    bcore_inst_a_discard( snk );
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_img_u2_s* bcore_img_u2_s_pnm_from_source( bcore_img_u2_s* o, bcore_source* source )
{
    BLM_INIT();

    // We avoid source-parse functions so that source can stay a mere data buffer.
    // Instead we scan the first text lines into a string:
    st_s* first_three_lines = BLM_CREATE( st_s );
    sz_t count = 0;
    while( !bcore_source_a_eos( source ) )
    {
        char c = bcore_source_a_get_char( source );
        st_s_push_char( first_three_lines, c );
        if( c == '\n' )
        {
            count++;
            if( count == 3 ) break;
        }
    }

    uz_t rows = 0, cols = 0;

    bcore_source_a_parse_fa
    (
        BLM_A_PUSH( bcore_source_string_s_create_from_string( first_three_lines ) ),
        "P6 #<uz_t*> #<uz_t*> 255\n",
        &cols,
        &rows
    );

    bcore_img_u2_s_set_size( o, rows, cols );

    for( sz_t i = 0; i < o->rows; i++ )
    {
        for( sz_t j = 0; j < o->cols; j++ )
        {
            u0_t rgb[ 3 ];
            bcore_source_a_get_data( source, rgb, 3 );
            bcore_img_u2_s_set_rgb( o, i, j, rgb[ 0 ], rgb[ 1 ], rgb[ 2 ] );
        }
    }

    BLM_DOWN();
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_img_u2_s* bcore_img_u2_s_pnm_from_file( bcore_img_u2_s* o, sc_t file )
{
    bcore_source* src = ( bcore_source* )bcore_source_open_file( file );
    bcore_img_u2_s_pnm_from_source( o, src );
    bcore_inst_a_discard( src );
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

static void selftest( void )
{
    BLM_INIT();
    bcore_img_u2_s* i1 = BLM_CREATE( bcore_img_u2_s );
    bcore_img_u2_s* i2 = BLM_CREATE( bcore_img_u2_s );
    sz_t rows = 128;
    sz_t cols = 256;
    bcore_img_u2_s_set_size( i1, rows, cols );
    for( sz_t i = 0; i < rows; i++ )
    {
        for( sz_t j = 0; j < cols; j++ )
        {
            bcore_img_u2_s_set_rgb( i1, i, j, ( i & 32 ) ? 255 : 0, ( j & 32 ) ? 255 : 0, ( ( j + i ) & 32 ) ? 255 : 0 );
        }
    }

    bcore_sink_buffer_s* sink = BLM_CREATE( bcore_sink_buffer_s );
    bcore_img_u2_s_pnm_to_sink( i1, ( bcore_sink* )sink );

    bcore_source_buffer_s* source = BLM_A_PUSH( bcore_source_buffer_s_create_from_data( sink->data, sink->size ) );
    bcore_img_u2_s_pnm_from_source( i2, ( bcore_source* )source );
    ASSERT( bcore_compare_aware( i1, i2 ) == 0 );

    BLM_DOWN();
}


/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

vd_t bcore_img_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_img" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_OBJECT( bcore_img_u2_s );
        }
        break;

        case TYPEOF_selftest:
        {
            selftest();
        }
        break;

        default: break;
    }

    return NULL;
}

