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

/** Spliced Function: Approximates a R->R function from a finite set of x-y samples.
 *
 *  Uses a spliced polynomial interpolation of a set of sample points.
 *  Properties:
 *  - Meets each sample point.
 *  - c0: Continuous for 0th derivative.         (Linear interpolation)
 *  -     If x is within x-boundaries, y stays in the y-boundary given by parameters.
 *  - c1: Continuous for 0th and 1st derivative. (Qubic interpolation)
 *  -     If x is within x-boundaries, y can still exceed y-boundary given by parameters.
 *  - Extrapolates (quadratic/)linear/const using the nearest available sample points.
 *  - Save with any amount of samples (in case of zero samples: y = 0).
 *  - Recall speed O(nlog(n)).
 *
 *  Setup:
 *  bmath_spliced_func_c1_s^ f;  // or bmath_spliced_func_c0_s^ f;
 *  f.push_xy( x[0], y[0] );
 *  .....
 *  f.push_xy( x[n], y[n] ); // requires x[i+1] > x[i]
 *
 *  Recall:
 *  f3_t y = f( x );
 */

#ifndef BMATH_SPLICED_FUNC_H
#define BMATH_SPLICED_FUNC_H

#include "bmath.xo.h"

XOILA_DEFINE_GROUP( bmath_spliced_func, x_inst )

#ifdef XOILA_SECTION

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp :xy_s( f3_t x, f3_t y ) obliv;

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp :c0_s x_array
{
    :xy_s [];

    func o push_xy( m@*o, f3_t x, f3_t y )
    {
        ASSERT( o.size == 0 || o.[ o.size - 1 ].x < x );
        o.push()( x, y );
        = o;
    }

    // linear splicing
    func f3_t y_of_2( f3_t x, :xy_s* xy0, :xy_s* xy1 ) { = xy0.y + ( x - xy0.x ) * ( xy1.y - xy0.y ) / ( xy1.x - xy0.x ); }

    // const
    func f3_t y_of_1( f3_t x, :xy_s* xy0 ) { = xy0.y; }

    // uses splicing via linear interpolation
    func f3_t y_( @*o, f3_t x, sz_t i1, sz_t i2 )
    {
        sz_t d = ( i2 - i1 ) >> 1;
        if( d > 0 )
        {
            = o.y_
            (
                x,
                ( x >= o.[ i1 + d ].x ) ? i1 + d : i1,
                ( x <  o.[ i2 - d ].x ) ? i2 - d : i2
            );
        }
        else
        {
            = o.y_of_2( x, o.[i1], o.[i2] );
        }
    }

    func f3_t _( @*o, f3_t x )
    {
        if( o.size  > 2 ) = o.y_( x, 0, o.size - 1 );
        if( o.size == 2 ) = o.y_of_2( x, o.[0], o.[1] );
        if( o.size == 1 ) = o.y_of_1( x, o.[0] );
        = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp :c1_s x_array
{
    :xy_s [];

    func o push_xy( m@*o, f3_t x, f3_t y )
    {
        ASSERT( o.size == 0 || o.[ o.size - 1 ].x < x );
        o.push()( x, y );
        = o;
    }

    // quadratic splicing
    func f3_t y_of_3( f3_t x, :xy_s* xy0, :xy_s* xy1, :xy_s* xy2 )
    {
        f3_t dx = ( xy2.x - xy0.x );
        f3_t _a = ( xy1.x - xy0.x ) / dx;
        f3_t  v = ( xy1.y - xy0.y ) / _a;
        f3_t  y = xy2.y - xy0.y;
        f3_t xt = ( x - xy0.x ) / dx;
        f3_t inv_1_a = 1.0 / ( 1.0 - _a );
        f3_t b = inv_1_a * ( v - y * _a );
        f3_t c = inv_1_a * ( y - v      );
        = xy0.y + ( b + c * xt ) * xt;
    }

    // linear splicing
    func f3_t y_of_2( f3_t x, :xy_s* xy0, :xy_s* xy1 ) { = xy0.y + ( x - xy0.x ) * ( xy1.y - xy0.y ) / ( xy1.x - xy0.x ); }

    // const
    func f3_t y_of_1( f3_t x, :xy_s* xy0 ) { = xy0.y; }

    // uses splicing such that the output is continuous in the 0th and 1st derivative (curve has no corners)
    func f3_t y_( @*o, f3_t x, sz_t i1, sz_t i2 )
    {
        sz_t d = ( i2 - i1 ) >> 1;
        if( d > 0 )
        {
            = o.y_
            (
                x,
                ( x >= o.[ i1 + d ].x ) ? i1 + d : i1,
                ( x <  o.[ i2 - d ].x ) ? i2 - d : i2
            );
        }
        else
        {
            f3_t xt = ( x - o.[i1].x ) / ( o.[i2].x - o.[i1].x );
            f3_t w = xt > 0 ? ( xt < 1.0 ? xt : 1.0 ) : 0.0;
            = o.y_of_3( x, o.[i1-1], o.[i1], o.[i2  ] ) * ( 1 - w ) +
              o.y_of_3( x, o.[i1  ], o.[i2], o.[i2+1] ) * w;
        }
    }

    func f3_t _( @*o, f3_t x )
    {
        if( o.size  > 3 ) = o.y_( x, 1, o.size - 2 );
        if( o.size == 3 ) = o.y_of_3( x, o.[0], o.[1], o.[2] );
        if( o.size == 2 ) = o.y_of_2( x, o.[0], o.[1] );
        if( o.size == 1 ) = o.y_of_1( x, o.[0] );
        = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#endif // XOILA_SECTION

#endif  // BMATH_SPLICED_FUNC_H
