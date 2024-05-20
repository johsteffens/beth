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
    bcore_msg( "%9.3g %9.3g\n", o->r, o->i );
}

/**********************************************************************************************************************/

/**********************************************************************************************************************/
// type conversion

//----------------------------------------------------------------------------------------------------------------------

er_t BCATU(bmath_cfx_s,copy_typed)( bmath_cfx_s* o, tp_t type, vc_t src )
{
    if( !src )
    {
        BCATU(bmath_cfx_s,zro)( o );
        return 0;
    }

    switch( type )
    {
        case BCATU(TYPEOF,bmath_cfx_s):
        {
            BCATU(bmath_cfx_s,copy)( o, src );
        }
        break;

        case BCATU(TYPEOF,bmath_cfy_s):
        {
            const bmath_cfy_s* v = src;
            o->r = v->r;
            o->i = v->i;
        }
        break;

        default:
        {
            return bcore_error_push_fa( TYPEOF_conversion_error, "copy_typed to 'bmath_cfx_s': no conversion from '#<sc_t>'.", ifnameof( type ) );
        }
        break;
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

static vd_t selftest( void )
{
    bmath_cfx_s c1 = { .r = 1, .i = 2 };
    bmath_cfx_s c2 = { .r = 3, .i = 4 };
    bmath_cfx_s c3 = { .r = 0, .i = 0 };
    bmath_cfx_s c4 = { .r = 0, .i = 0 };

    BCATU(bmath_cfx_s,div)( &c1, &c2, &c3 );
    bmath_ring_t_div( TYPEOF_bmath_cfx_s, (bmath_ring*)&c1, (bmath_ring*)&c2, (bmath_ring*)&c4 );
    ASSERT( BCATU(bmath_cfx_s,is_equ)( &c3, &c4 ) );

    return NULL;
}

/**********************************************************************************************************************/

#include "bmath_template_fx_end.h"

/**********************************************************************************************************************/

