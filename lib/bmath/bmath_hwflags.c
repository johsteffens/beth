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

#include "bmath_hwflags.h"

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

void bmath_hwflags_to_string( st_s* string )
{

#ifdef BMATH_SSE
    st_s_push_fa( string, "SSE " );
#endif // BMATH_SSE

#ifdef BMATH_SSE2
    st_s_push_fa( string, "SSE2 " );
#endif // BMATH_SSE2

#ifdef BMATH_AVX
    st_s_push_fa( string, "AVX " );
#endif // BMATH_AVX

#ifdef BMATH_AVX2
    st_s_push_fa( string, "AVX2 " );
#endif // BMATH_AVX2

#ifdef BMATH_AVX2_FMA
    st_s_push_fa( string, "AVX2_FMA " );
#endif // BMATH_AVX2_FMA

    st_s_push_fa( string, "\n" );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_hwflags_to_stdout( void )
{
    st_s* s = st_s_create();
    bmath_hwflags_to_string( s );
    bcore_msg_fa( "#<sc_t>", s->sc );
    st_s_discard( s );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/


