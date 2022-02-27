/** Author and Copyright 2022 Johannes Bernhard Steffens
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

/** Adapted Fletcher-Munson-Curve calculation based on ISO226-2003 standard.
 *
 *  Motivation (Psychoacoustics):
 *  Perceived loudness depends on sound pressure level (SPL) and frequency.
 *  Changes in SPL yield different changes in perceived loudness based on given SPL and frequency.
 *  This behavior is quantified by so-called Fletcher-Munson-Curves.
 *
 *  Equal-Loudness (EQL) specifies the frequency dependent sound pressure level needed to produce
 *  the same loudness of a pure sine tone compared to the 1KHz tone at a given SPL.
 *
 *  EQL and SPL are both measured in DB.
 *  1kHz is the reference frequency at which loudness and SPL have the same value.
 *  This framework can compute a frequency-loudness function for an arbitrary 1kHz SPL.
 *
 *  The iso standard only covers values up to 12500 Hz. This implementation extends the table
 *  to 20kHz, roughly approximating the expected rise in EQL to higher frequency, but without any
 *  claim to psychoacousic accuracy. (While this implementation outputs finite SPL >= 20kHz,
 *  such a tone would be practically inaudible for most older people.)
 *
 *
 *
 *  Usage:
 *    bmedia_iso226_eql_list_s^ list;
 *    $* func = list.eql_func_log10_f_spl( bmath_spliced_func_c1_s!^, 40 ); // 40DB curve
 *    f3_t loudness = func( f3_log10( 300 ) ); // loudness at 300 Hz
 *
 *
 *
 *  Sources:
 *  [1] J. Parmanen, "Some Reasons to Revise the International Standard ISO 226:2003: Acoustics—Normal Equal-Loudness-Level Contours,"
 *  Open Journal of Acoustics, Vol. 2 No. 4, 2012, pp. 143-149. doi: 10.4236/oja.2012.24016.
 *
 *  [2] https://williamssoundstudio.com/tools/iso-226-equal-loudness-calculator-fletcher-munson.php
 *
 *  Original Research:
 *  [3] Fletcher, H. and Munson, W.A. "Loudness, its definition, measurement and calculation", Journal of the Acoustical Society of America 5, 82–108 (1933)
 */

#ifndef BMEDIA_ISO226_H
#define BMEDIA_ISO226_H

#include "bcore_std.h"
#include "bmath_std.h"

//----------------------------------------------------------------------------------------------------------------------

XOILA_DEFINE_GROUP( bmedia_iso226, x_inst )

#ifdef XOILA_SECTION

/**********************************************************************************************************************/

/// iso-226 L_p functor from key parameters
stamp :L_p_s( f3_t f, f3_t alpha_f, f3_t L_U, f3_t T_f ) func f3_t( @*o, f3_t L_n )
{
    f3_t A_f = 4.47E-3 * ( f3_pow( 10.0, 0.025 * L_n ) - 1.15 ) +
               f3_pow( 0.4 * f3_pow( 10.0, ( o.T_f + o.L_U ) * 0.1 - 9 ), o.alpha_f );

    = ( 10.0 * f3_log10( A_f ) / o.alpha_f ) - o.L_U + 94.0;
}

//----------------------------------------------------------------------------------------------------------------------

stamp :eql_list_s x_array
{
    :L_p_s [];

    func bcore_inst_call.init_x
    {
        o.clear();

        // ISO values: 20 Hz ... 12500 Hz
        o.push()(    20  , 0.532, -31.6, 78.5 );
        o.push()(    25  , 0.506, -27.2, 68.7 );
        o.push()(    31.5, 0.480, -23.0, 59.5 );

        o.push()(    40  , 0.455, -19.1, 51.1 );
        o.push()(    50  , 0.432, -15.9, 44.0 );
        o.push()(    63  , 0.409, -13.0, 37.5 );

        o.push()(    80  , 0.387, -10.3, 31.5 );
        o.push()(   100  , 0.367,  -8.1, 26.5 );
        o.push()(   125  , 0.349,  -6.2, 22.1 );

        o.push()(   160  , 0.330,  -4.5, 17.9 );
        o.push()(   200  , 0.315,  -3.1, 14.4 );
        o.push()(   250  , 0.301,  -2.0, 11.4 );

        o.push()(   315  , 0.288,  -1.1,  8.6 );
        o.push()(   400  , 0.276,  -0.4,  6.2 );
        o.push()(   500  , 0.267,   0.0,  4.4 );

        o.push()(   630  , 0.259,   0.3,  3.0 );
        o.push()(   800  , 0.253,   0.5,  2.2 );
        o.push()(  1000  , 0.250,   0.0,  2.4 );

        o.push()(  1250  , 0.246,  -2.7,  3.5 );
        o.push()(  1600  , 0.244,  -4.1,  1.7 );
        o.push()(  2000  , 0.243,  -1.0, -1.3 );

        o.push()(  2500  , 0.243,   1.7, -4.2 );
        o.push()(  3150  , 0.243,   2.5, -6.0 );
        o.push()(  4000  , 0.242,   1.2, -5.4 );

        o.push()(  5000  , 0.242,  -2.1, -1.5 );
        o.push()(  6300  , 0.245,  -7.1,  6.0 );
        o.push()(  8000  , 0.254, -11.2, 12.6 );

        o.push()( 10000  , 0.271, -10.7, 13.9 );
        o.push()( 12500  , 0.301,  -3.1, 12.3 );

        o.push()( 16000  , 0.3  , -20  , 50.0 ); // non-iso extension
        o.push()( 20000  , 0.4  , -40  ,140.0 ); // non-iso extension
    }

    func splf eql_func_log10_f_spl( @*o, m bmath_spliced_func_c1_s* splf, f3_t spl_1khz )
    {
        splf.clear();
        foreach( $* e in o ) splf.push_xy( f3_log10( e.f ), e.( spl_1khz ) );
        = splf;
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#endif // XOILA_SECTION

#endif // BMEDIA_ISO226_H
