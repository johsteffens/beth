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

/// Discrete Cosine Transform

#ifndef BMATH_COSINE_H
#define BMATH_COSINE_H

#include "bmath_fourier_f2.h"
#include "bmath_fourier_f3.h"
#include "bmath.xo.h"

XOILA_DEFINE_GROUP( bmath_cosine, x_inst )

#ifdef XOILA_SECTION

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/** Discrete cosine transform.
 */
group :dct
{
    stamp :f3_s
    {
        /// reusable buffer for temporary data
        $ hidden bmath_vcf3_s => buf_;

        // DCT2 direct implementation (slow)
        func void df( c f3_t* src, m f3_t* dst, sz_t size );
        func dst dfv( m@* o, c bmath_vf3_s* src, m bmath_vf3_s* dst );

        // Inverse of DCT2 direct implementation (slow)  (DCT3 with factor 2/N)
        func void di( c f3_t* src, m f3_t* dst, sz_t size );
        func dst div( m@* o, c bmath_vf3_s* src, m bmath_vf3_s* dst );

        /** DCT Fast Forward Transformation (via FFT)
         *  Transforms size source-samples into size destination samples
         *  src, dst can be the same pointer
         */
        func void ff( m@* o, c f3_t* src, m f3_t* dst, sz_t size );
        func dst ffv( m@* o, c bmath_vf3_s* src, m bmath_vf3_s* dst );

        /** DCT2 Fast Inverse Transformation (via FFT)  (DCT3 with factor 2/N)
         *  Transforms size source-samples into size destination samples
         *  src, dst can be the same pointer
         */
        func void fi( m@* o, c f3_t* src, m f3_t* dst, sz_t size );
        func dst fiv( m@* o, c bmath_vf3_s* src, m bmath_vf3_s* dst );
    }

    //------------------------------------------------------------------------------------------------------------------

    stamp :f2_s
    {
        /// reusable buffer for temporary data
        $ hidden bmath_vcf2_s => buf_;

        // DCT2 direct implementation (slow)
        func void df( c f2_t* src, m f2_t* dst, sz_t size );
        func dst dfv( m@* o, c bmath_vf2_s* src, m bmath_vf2_s* dst );

        // Inverse of DCT2 direct implementation (slow)  (DCT3 with factor 2/N)
        func void di( c f2_t* src, m f2_t* dst, sz_t size );
        func dst div( m@* o, c bmath_vf2_s* src, m bmath_vf2_s* dst );

        /** DCT Fast Forward Transformation (via FFT)
         *  Transforms size source-samples into size destination samples
         *  src, dst can be the same pointer
         */
        func void ff( m@* o, c f2_t* src, m f2_t* dst, sz_t size );
        func dst ffv( m@* o, c bmath_vf2_s* src, m bmath_vf2_s* dst );

        /** DCT2 Fast Inverse Transformation (via FFT)  (DCT3 with factor 2/N)
         *  Transforms size source-samples into size destination samples
         *  src, dst can be the same pointer
         */
        func void fi( m@* o, c f2_t* src, m f2_t* dst, sz_t size );
        func dst fiv( m@* o, c bmath_vf2_s* src, m bmath_vf2_s* dst );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/** Modified discrete cosine transform.
 *  See beth/doc/MDCT_VIA_DFT.odt for details.
 */
group :mdct
{
    stamp :f3_s
    {
        /// reusable buffer for temporary data
        hidden bmath_vcf3_s => buf_;

        /** Fast Forward Transformation (via FFT)
         *  Transforms src_size source-samples into src_size/2 destination samples
         *  src, dst can be the same pointer
         */
        func void ff( m@* o, c f3_t* src, m f3_t* dst, sz_t src_size );
        func dst ffv( m@* o, c bmath_vf3_s* src, m bmath_vf3_s* dst );

        /** Fast Inverse Transformation (via FFT)
         *  Transforms dst_size/2 source-samples into dst_size destination samples
         *  src, dst can be the same pointer
         */
        func void fi( m@* o, c f3_t* src, m f3_t* dst, sz_t dst_size );
        func dst fiv( m@* o, c bmath_vf3_s* src, m bmath_vf3_s* dst );
    }

    //------------------------------------------------------------------------------------------------------------------

    stamp :f2_s
    {
        /// reusable buffer for temporary data
        hidden bmath_vcf2_s => buf_;

        /** Fast Forward Transformation (via FFT)
         *  Transforms src_size source-samples into src_size/2 destination samples
         *  src, dst can be the same pointer
         */
        func void ff( m@* o, c f2_t* src, m f2_t* dst, sz_t src_size );
        func dst ffv( m@* o, c bmath_vf2_s* src, m bmath_vf2_s* dst );

        /** Fast Inverse Transformation (via FFT)
         *  Transforms dst_size/2 source-samples into dst_size destination samples
         *  src, dst can be the same pointer
         */
        func void fi( m@* o, c f2_t* src, m f2_t* dst, sz_t dst_size );
        func dst fiv( m@* o, c bmath_vf2_s* src, m bmath_vf2_s* dst );
    }

    //------------------------------------------------------------------------------------------------------------------

    // Window functions usable with mdct
    group :window_function =
    {
        feature o reset( m@* o, sz_t size ) {}
        feature f3_t get( m@* o );

        func vec gen_f2( m@* o, sz_t size, m bmath_vf2_s* vec ) { o.reset( size ); vec.set_size( size ); for( sz_t i = 0; i < size; i++ ) vec.[ i ] = o.get(); }
        func vec gen_f3( m@* o, sz_t size, m bmath_vf3_s* vec ) { o.reset( size ); vec.set_size( size ); for( sz_t i = 0; i < size; i++ ) vec.[ i ] = o.get(); }

        stamp :const_s func :.get = 1.0;

        stamp :cosine_s
        {
            bmath_cf3_s c;
            bmath_cf3_s w;
            func :.reset { o.c.urt( 1, size * 4 ); o.w.urt( 1, size * 2 ); }
            func :.get { f3_t v = o.c.i; o.c.mul( o.w, o.c ); = v * f3_srt2(); }
        }

        stamp :sine_cosine_s
        {
            bmath_cf3_s c;
            bmath_cf3_s w;
            func :.reset { o.c.urt( 1, size * 4 ); o.w.urt( 1, size * 2 ); }
            func :.get { f3_t v = sin( 0.5 * f3_pi() * o.c.i * o.c.i ); o.c.mul( o.w, o.c ); = v * f3_srt2(); }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

embed "bmath_cosine.x";

/**********************************************************************************************************************/

#endif // XOILA_SECTION

#endif  // BMATH_COSINE_H
