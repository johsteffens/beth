/** Author and Copyright 2020 Johannes Bernhard Steffens
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

#include "bmath_stat.h"
#include "bcore_flect.h"

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

f3_t bmath_stat_regularized_gamma( f3_t s, f3_t x, bl_t* success )
{
    if( x < 1E-300 )
    {
        if( success ) *success = true;
        return 0;
    }

    const sz_t max_iterations = 1000;
    bl_t success_l = false;
    f3_t log_x = log( x );
    f3_t sum = 0;

    for( sz_t k = 0; k < max_iterations; k++ )
    {
        f3_t log_p = k * log_x - lgamma( s + k + 1 );

        if( log_p > 700 ) break;

        sum += exp( log_p );

        if( log( sum ) - log_p > 37 ) // sufficient convergence
        {
            success_l = true;
            break;
        }
    }

    f3_t rg = exp( s * log( x ) - x + log( sum ) );

    if( rg > 1.0 )
    {
        rg = 1.0;
        success_l = false;
    }

    if( success ) *success = success_l;

    return rg;
}

// ---------------------------------------------------------------------------------------------------------------------

f3_t bmath_stat_chisqu_cdf( sz_t k, f3_t x, bl_t* success )
{
    return bmath_stat_regularized_gamma( k * 0.5, x * 0.5, success );
}

// ---------------------------------------------------------------------------------------------------------------------

f3_t bmath_stat_chisqu_cdf_inv( sz_t k, f3_t p, bl_t* success )
{
    ASSERT( p < 1.0 );

    f3_t x = 0;
    f3_t x_min = 0;
    f3_t x_max = 0;
    f3_t pr = 0;
    bl_t success_l = true;

    for( x = 0; success_l; x += 1.0 )
    {
        pr = bmath_stat_chisqu_cdf( k, x, &success_l );
        if( pr > p )
        {
            x_max = x;
            break;
        }
        x_min = x;
    }

    for( sz_t i = 0; success_l && ( i < 16 ); i++ )
    {
        x = ( x_min + x_max ) * 0.5;
        pr = bmath_stat_chisqu_cdf( k, x, &success_l );
        x_max = ( pr > p ) ? x : x_max;
        x_min = ( pr < p ) ? x : x_min;
    }

    if( success ) *success = success_l;
    return ( x_min + x_max ) * 0.5;
}

// ---------------------------------------------------------------------------------------------------------------------

f3_t bmath_erf_inv( f3_t y )
{
    if( y >=  1.0 ) return  f3_lim_inf;
    if( y <= -1.0 ) return -f3_lim_inf;

    f3_t x_min = -10;
    f3_t x_max =  10;

    while( y < bmath_erf( x_min ) ) { x_max = x_min; x_min *= 2; }
    while( y > bmath_erf( x_max ) ) { x_min = x_max; x_max *= 2; }

    for( sz_t i = 0; i < 64; i++ )
    {
        f3_t x = ( x_min + x_max ) * 0.5;
        f3_t y1 = bmath_erf( x );
        x_max = ( y1 > y ) ? x : x_max;
        x_min = ( y1 < y ) ? x : x_min;
    }

    return ( x_min + x_max ) * 0.5;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

vd_t bmath_stat_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_stat" ) ) )
    {
        case TYPEOF_init1:
        {
        }
        break;

        case TYPEOF_get_quicktypes:
        {
        }
        break;

        case TYPEOF_selftest:
        {
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

