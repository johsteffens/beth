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

/**********************************************************************************************************************/

/** Complex Number
 *
 *  Nomenclature
 *  add, zro, neg, sub, mul, one, inv, div - according to spect_ring
 *  cnj - complex conjugate
 *  det - squared - magnitude: a^2 + b^2  (determinat of the canonic rot-scale-matrix)
 *  mag - magnitude: sqrt( mag );
 *  arg - phase atan2( im, re )
 *  urt - kth element of nth-roots of unity: urt(k,n) = e^i*2*pi*k/n
 */

/**********************************************************************************************************************/

#include "bmath_template_fx_begin.h"

/**********************************************************************************************************************/

void BCATU(bmath_cfx_s,to_stdout)( const bmath_cfx_s* o )
{
    printf( "%9.3g %9.3g\n", o->v[ 0 ], o->v[ 1 ] );
}

/**********************************************************************************************************************/

static vd_t selftest( void )
{
    bmath_cfx_s c1 = { .v[0] = 1, .v[1] = 2 };
    bmath_cfx_s c2 = { .v[0] = 3, .v[1] = 4 };
    bmath_cfx_s c3 = { .v[0] = 0, .v[1] = 0 };
    bmath_cfx_s c4 = { .v[0] = 0, .v[1] = 0 };

    BCATU(bmath_cfx_s,div)( &c1, &c2, &c3 );
    bmath_ring_t_div( TYPEOF_bmath_cfx_s, (bmath_ring*)&c1, (bmath_ring*)&c2, (bmath_ring*)&c4 );
    ASSERT( BCATU(bmath_cfx_s,is_equ)( &c3, &c4 ) );

    return NULL;
}

/**********************************************************************************************************************/

#include "bmath_template_fx_end.h"

/**********************************************************************************************************************/

