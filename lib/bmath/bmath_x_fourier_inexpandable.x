/** Author and Copyright 2021 Johannes Bernhard Steffens
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

/**********************************************************************************************************************/

/** Discrete Fourier Transform.
 *  DFT: Canonic implementation: F_k = Sum_j( f_j * w^jk ); w := e^-i2pi/n
 *  FFT: Fast transform (on power 2 sizes) (numerically equivalent to DFT)
 *  To obtain the inverse transform:
 *      - Conjugate src before FT.
 *      - Conjugate dst after FT and scale with 1.0 / size.
 *
 *  Note:
 *  The numerical stability of FFT scales much better than of the general DCT.
 *  In nearly all practical use cases FFT is the preferable choice. This remains true
 *  even with the use of padding to match the power-2 size requirement.
 *  Due to DCT's simplicity, the DCT can be used for understanding the FT and
 *  in verification or debugging purposes (for small enough data sizes).
 */

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

group bmath_fourier =
{
    /// DFT/FFT (dst==src allowed); fft: size = power of 2

    /** (Slow) Discrete Fourier Transformation for all vector sizes.
     *
     *  - Complexity O(n^2)
     *  - dst==src allowed
     *  - Accuracy:
     *      f2: tolerable accuracy at size <= 1<<8
     *      f3: accuracy limit probably between size 1<<17 and 1<<20
     */
    func void f2_dft( const bmath_cf2_s* src, bmath_cf2_s* dst, uz_t size );
    func void f3_dft( const bmath_cf3_s* src, bmath_cf3_s* dst, uz_t size );

    /** Fast Fourier Transformation for vector size being power of 2.
     *  - Complexity O(n*log(n))
     *  - dst==src allowed
     *  - Accuracy: f2,f3 both stable up to very large sizes (see function test_fourier_accuracy)
     *              Deviation remains below 0.1% of vector norm for all tested sizes.
     *  - AVX2 optimized
     *  - Allocates an internal buffer. Use fft_buf below to re-use a preallocated buffer.
     */
    func void f2_fft( const bmath_cf2_s* src, bmath_cf2_s* dst, uz_t size );
    func void f3_fft( const bmath_cf3_s* src, bmath_cf3_s* dst, uz_t size );

    /** FFT with reusable buffer. Return value is buffer to be discarded or reused by caller.
     *  Start with buf = NULL or buf = bcore_malloc( sizeof( bmath_cfx_s ) * size )
     *  Continue with buf = return from previous call (size <= size of first call)
     *  Finish by keeping ownership of returned_buf (e.g. bcore_free( returned_buf ))
     *
     *  - buf!=dst required
     *  - dst==src allowed
     *  - buf==src allowed
     *
     *  Other specs: see fx_fft above
     */
    func vd_t f2_fft_buf( const bmath_cf2_s* src, bmath_cf2_s* dst, uz_t size, vd_t buf );
    func vd_t f3_fft_buf( const bmath_cf3_s* src, bmath_cf3_s* dst, uz_t size, vd_t buf );
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

