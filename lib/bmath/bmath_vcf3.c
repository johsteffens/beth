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

#include <stdio.h>

#include "bmath_vcf3.h"
#include "bmath_vcf2.h"
#include "bmath_spect_vector.h"
#include "bmath_fourier_f3.h"
#include "bmath_fourier_f2.h"

#define BMATH_TEMPLATE_FX_PREC 3
#include "bmath_template_vcfx_c.h"

/**********************************************************************************************************************/
// bmath_vcf3_s

BCORE_DEFINE_OBJECT_INST( bmath_vector, bmath_vcf3_s )
"{"
    "aware_t _;"
    "bmath_cf3_s [];"
    "func bmath_fp:add;"
    "func bmath_fp:zro;"
    "func bmath_fp:neg;"
    "func bmath_fp:sub;"
    "func bmath_fp:cpy;"
    "func bmath_fp_vector:mul_scl = bmath_vcf3_s_mul_scl;"
    "func bmath_fp_vector:mul_vec = bmath_vcf3_s_mul_vec;"
    "func bcore_fp_copy_typed     = bmath_vcf3_s_copy_typed;"
"}";

//----------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_copy_typed( bmath_vcf3_s* o, tp_t type, vc_t src )
{
    if( !src )
    {
        bmath_vcf3_s_clear( o );
        return;
    }

    switch( type )
    {
        case TYPEOF_bmath_vcf3_s: bmath_vcf3_s_copy( o, src ); break;
        case TYPEOF_bmath_vcf2_s:
        {
            const bmath_vcf2_s* v = src;
            bmath_vcf3_s_set_size( o, v->size );
            BFOR_EACH( i, o )
            {
                o->data[ i ].v[ 0 ] = v->data[ i ].v[ 0 ];
                o->data[ i ].v[ 1 ] = v->data[ i ].v[ 1 ];
            }
        }
        break;
        default: bcore_err_fa( "Cannot copy from #<sc_t>.", ifnameof( type ) ); break;
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

static void test_fourier_accuracy()
{
    /* We test the accuracy of f2 fourier by comparing to to f3 */

    BLM_INIT();

    bmath_vcf2_s* vcf2_1 = BLM_CREATE( bmath_vcf2_s );
    bmath_vcf2_s* vcf2_2 = BLM_CREATE( bmath_vcf2_s );
    bmath_vcf3_s* vcf3_1 = BLM_CREATE( bmath_vcf3_s );
    bmath_vcf3_s* vcf3_2 = BLM_CREATE( bmath_vcf3_s );
    bmath_vcf3_s* vcf3_3 = BLM_CREATE( bmath_vcf3_s );
    bmath_vcf3_s* vcf3_c = BLM_CREATE( bmath_vcf3_s );

    /// f2 fft
    for( sz_t size_exp = 4; size_exp < 20; size_exp++ )
    {
        sz_t size = 1 << size_exp;

        bmath_vcf2_s_set_size( vcf2_1, size );
        bmath_vcf3_s_set_size( vcf3_1, size );
        bmath_vcf2_s_set_size( vcf2_2, size );
        bmath_vcf3_s_set_size( vcf3_2, size );

        bmath_vcf2_s_set_random( vcf2_1, 1.0, -1.0, 1.0, NULL );

        bmath_vcf3_s_copy_a( vcf3_1, vcf2_1 );
        bmath_vcf3_s_copy_a( vcf3_c, vcf2_1 );
        ASSERT( bmath_vcf3_s_fdev( vcf3_1, vcf3_c ) == 0 );

        bmath_fourier_f2_dft( vcf2_1->data, vcf2_2->data, vcf2_1->size );
        bmath_fourier_f3_fft( vcf3_1->data, vcf3_2->data, vcf3_1->size );

        bmath_vcf3_s_copy_a( vcf3_c, vcf2_2 );
        f3_t fdev = bmath_vcf3_s_fdev( vcf3_2, vcf3_c );
        f3_t norm = f3_srt( bmath_vcf3_s_tss( vcf3_2 ) );

        f3_t fft_dev = fdev / norm;
        //bcore_msg_fa( "exp: #<sz_t>: fft_dev: #<f3_t>\n", size_exp, fft_dev );

        if( fft_dev > 0.001 )
        {
            bcore_msg_fa( "Accuray size limit (DFT f2): #<sz_t> \n", 1 << ( size_exp - 1 ) );
            break;
        }
    }

    /// f2 fft
    for( sz_t size_exp = 4; size_exp < 12; size_exp++ )
    {
        sz_t size = 1 << size_exp;

        bmath_vcf3_s_set_size( vcf3_1, size );
        bmath_vcf3_s_set_size( vcf3_2, size );
        bmath_vcf3_s_set_size( vcf3_3, size );

        bmath_vcf3_s_set_random( vcf3_1, 1.0, -1.0, 1.0, NULL );

        bmath_fourier_f3_fft( vcf3_1->data, vcf3_2->data, vcf3_1->size );
        bmath_fourier_f3_dft( vcf3_1->data, vcf3_3->data, vcf3_1->size );

        f3_t fdev = bmath_vcf3_s_fdev( vcf3_2, vcf3_3 );
        f3_t norm = f3_srt( bmath_vcf3_s_tss( vcf3_2 ) );

        f3_t fft_dev = fdev / norm;
        //bcore_msg_fa( "exp: #<sz_t>: fft_dev: #<f3_t>\n", size_exp, fft_dev );

        if( fft_dev > 0.001 )
        {
            bcore_msg_fa( "Accuray size limit (DFT f3): #<sz_t> \n", 1 << ( size_exp - 1 ) );
            break;
        }
    }

    for( sz_t size_exp = 4; size_exp < 20; size_exp++ )
    {
        sz_t size = 1 << size_exp;

        bmath_vcf2_s_set_size( vcf2_1, size );
        bmath_vcf3_s_set_size( vcf3_1, size );
        bmath_vcf2_s_set_size( vcf2_2, size );
        bmath_vcf3_s_set_size( vcf3_2, size );

        bmath_vcf2_s_set_random( vcf2_1, 1.0, -1.0, 1.0, NULL );

        bmath_vcf3_s_copy_a( vcf3_1, vcf2_1 );
        bmath_vcf3_s_copy_a( vcf3_c, vcf2_1 );
        ASSERT( bmath_vcf3_s_fdev( vcf3_1, vcf3_c ) == 0 );

        bmath_fourier_f2_fft( vcf2_1->data, vcf2_2->data, vcf2_1->size );
        bmath_fourier_f3_fft( vcf3_1->data, vcf3_2->data, vcf3_1->size );

        bmath_vcf3_s_copy_a( vcf3_c, vcf2_2 );
        f3_t fdev = bmath_vcf3_s_fdev( vcf3_2, vcf3_c );
        f3_t norm = f3_srt( bmath_vcf3_s_tss( vcf3_2 ) );

        f3_t fft_dev = fdev / norm;

        //bcore_msg_fa( "exp: #<sz_t>: fft_dev: #<f3_t>\n", size_exp, fft_dev );

        if( fft_dev > 0.001 )
        {
            bcore_msg_fa( "Accuray size limit (FFT f2): #<sz_t> \n", 1 << ( size_exp - 1 ) );
            break;
        }
    }

    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

vd_t bmath_vcf3_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_vcf3" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_FFUNC( bmath_fp_add,            bmath_vcf3_s_add );
            BCORE_REGISTER_FFUNC( bmath_fp_zro,            bmath_vcf3_s_zro );
            BCORE_REGISTER_FFUNC( bmath_fp_neg,            bmath_vcf3_s_neg );
            BCORE_REGISTER_FFUNC( bmath_fp_sub,            bmath_vcf3_s_sub );
            BCORE_REGISTER_FFUNC( bmath_fp_cpy,            bmath_vcf3_s_cpy );
            BCORE_REGISTER_FFUNC( bmath_fp_vector_mul_vec, bmath_vcf3_s_mul_vec );
            BCORE_REGISTER_FFUNC( bmath_fp_vector_mul_scl, bmath_vcf3_s_mul_scl );
            BCORE_REGISTER_FFUNC( bcore_fp_copy_typed,     bmath_vcf3_s_copy_typed );
            BCORE_REGISTER_OBJECT( bmath_vcf3_s );
        }
        break;

        case TYPEOF_get_quicktypes:
        {
            BCORE_REGISTER_QUICKTYPE( bmath_vcf3_s );
        }
        break;

        case TYPEOF_selftest:
        {
            test_fourier_accuracy();
            return selftest();
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

