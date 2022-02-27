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
}

//----------------------------------------------------------------------------------------------------------------------

embed "bmath_cosine.x";

/**********************************************************************************************************************/

#endif // XOILA_SECTION

#endif  // BMATH_COSINE_H
