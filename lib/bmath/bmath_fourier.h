/** Copyright 2018 Johannes Bernhard Steffens
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

#ifndef BMATH_FOURIER_H
#define BMATH_FOURIER_H

/** Discrete Fourier Transform.
 *  DFT: Canonic implementation: F_k = Sum_j( f_j * w^jk ); w := e^-i2pi/n
 *  FFT: Fast transform (on power 2 sizes) (numerically equivalent to DFT)
 *  To obtain the inverse transform:
 *      - Conjugate src before FT.
 *      - Conjugate dst after FT and scale with 1.0 / size.
 */

#include "bcore_std.h"
#include "bmath_spect_algebraic.h"
#include "bmath_complex.h"

/**********************************************************************************************************************/

/// dft/fft (dst==src allowed); fft: size = power of 2
void bmath_fourier_dft_f3( bmath_cf3_s* dst, const bmath_cf3_s* src, sz_t size );
void bmath_fourier_fft_f3( bmath_cf3_s* dst, const bmath_cf3_s* src, sz_t size );

/**********************************************************************************************************************/

vd_t bmath_fourier_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_FOURIER_H
