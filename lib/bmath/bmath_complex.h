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

#ifndef BMATH_COMPLEX_H
#define BMATH_COMPLEX_H

#include "bmath_spect_algebraic.h"
#include "bcore_std.h"
#include "bmath_leaf.h"
#include "bmath_quicktypes.h"

/**********************************************************************************************************************/

/** Nomenclature
 *  add, zro, neg, sub, mul, one, inv, div - according to spect_ring
 *  cnj - complex conjugate
 *  det - squared - magnitude: a^2 + b^2  (determinat of the canonic rot-scale-matrix)
 *  mag - magnitude: sqrt( mag );
 *  arg - phase atan2( im, re )
 *  urt - kth element of nth-roots of unity: urt(k,n) = e^i*2*pi*k/n
 */

/**********************************************************************************************************************/

typedef struct bmath_cf3_s { f3_t v[ 2 ]; } bmath_cf3_s;
BCORE_DECLARE_OBJECT( bmath_cf3_s )

static inline
bmath_cf3_s bmath_cf3_init( f3_t r, f3_t i )
{
    return ( bmath_cf3_s ) { .v[0] = r, .v[1] = i };
}

/// init using mag, arg (radius, angle)
static inline
bmath_cf3_s bmath_cf3_init_mag_arg( f3_t mag, f3_t arg )
{
    return ( bmath_cf3_s ) { .v[0] = mag * cos( arg ), .v[1] = mag * sin( arg ) };
}

/// urt-init (s. nomenclature); k can be negative indicating the circular direction.
static inline
bmath_cf3_s bmath_cf3_init_urt( s3_t k, s3_t n )
{
    f3_t arg = ( f3_tau() * k ) / n;
    return ( bmath_cf3_s ) { .v[0] = cos( arg ), .v[1] = sin( arg ) };
}

static inline
bmath_cf3_s bmath_cf3_zro()
{
    return ( bmath_cf3_s ) { .v[0] = 0, .v[1] = 0 };
}

static inline
bmath_cf3_s bmath_cf3_one()
{
    return ( bmath_cf3_s ) { .v[0] = 1, .v[1] = 0 };
}

static inline
void bmath_cf3_s_add( bmath_cf3_s* o, const bmath_cf3_s* op1, const bmath_cf3_s* op2 )
{
    o->v[ 0 ] = op1->v[ 0 ] + op2->v[ 0 ];
    o->v[ 1 ] = op1->v[ 1 ] + op2->v[ 1 ];
}

static inline
void bmath_cf3_s_add_sqr( bmath_cf3_s* o, const bmath_cf3_s* op1, const bmath_cf3_s* op2 )
{
    o->v[ 0 ] = op1->v[ 0 ] + f3_sqr( op2->v[ 0 ] ) - f3_sqr( op2->v[ 1 ] );
    o->v[ 1 ] = op1->v[ 1 ] + ( op2->v[ 0 ] * op2->v[ 1 ] * 2.0 );
}

static inline
void bmath_cf3_s_zro( bmath_cf3_s* o )
{
    o->v[ 0 ] = 0;
    o->v[ 1 ] = 0;
}

static inline
void bmath_cf3_s_neg( bmath_cf3_s* o, const bmath_cf3_s* op1 )
{
    o->v[ 0 ] = -op1->v[ 0 ];
    o->v[ 1 ] = -op1->v[ 1 ];
}

static inline
bmath_cf3_s bmath_cf3_neg( bmath_cf3_s op1 )
{
    return ( bmath_cf3_s ) { .v[0] = -op1.v[ 0 ], .v[1] = -op1.v[ 1 ] };
}

static inline
void bmath_cf3_s_sub( bmath_cf3_s* o, const bmath_cf3_s* op1, const bmath_cf3_s* op2 )
{
    o->v[ 0 ] = op1->v[ 0 ] - op2->v[ 0 ];
    o->v[ 1 ] = op1->v[ 1 ] - op2->v[ 1 ];
}

static inline
void bmath_cf3_s_mul( bmath_cf3_s* o, const bmath_cf3_s* op1, const bmath_cf3_s* op2 )
{
    f3_t re   = op1->v[ 0 ] * op2->v[ 0 ] - op1->v[ 1 ] * op2->v[ 1 ];
    o->v[ 1 ] = op1->v[ 0 ] * op2->v[ 1 ] + op1->v[ 1 ] * op2->v[ 0 ];
    o->v[ 0 ] = re;
}

static inline
bmath_cf3_s bmath_cf3_mul( bmath_cf3_s op1, bmath_cf3_s op2 )
{
    f3_t re = op1.v[ 0 ] * op2.v[ 0 ] - op1.v[ 1 ] * op2.v[ 1 ];
    f3_t im = op1.v[ 0 ] * op2.v[ 1 ] + op1.v[ 1 ] * op2.v[ 0 ];
    return ( bmath_cf3_s ) { .v[0] = re, .v[1] = im };
}

static inline
bmath_cf3_s bmath_cf3_mul_f3( bmath_cf3_s op1, f3_t op2 )
{
    return ( bmath_cf3_s ) { .v[0] = op1.v[ 0 ] * op2, .v[1] = op1.v[ 1 ] * op2 };
}

static inline
void bmath_cf3_s_sqr( bmath_cf3_s* o, const bmath_cf3_s* op1 )
{
    f3_t re   = op1->v[ 0 ] * op1->v[ 0 ] - op1->v[ 1 ] * op1->v[ 1 ];
    o->v[ 1 ] = op1->v[ 0 ] * op1->v[ 1 ] * 2.0;
    o->v[ 0 ] = re;
}

static inline
void bmath_cf3_s_sqr_sub( bmath_cf3_s* o, const bmath_cf3_s* op1, const bmath_cf3_s* op2 )
{
    f3_t dre  = op1->v[ 0 ] - op2->v[ 0 ];
    f3_t dim  = op1->v[ 1 ] - op2->v[ 1 ];
    o->v[ 0 ] = dre * dre - dim * dim;
    o->v[ 1 ] = dre * dim * 2.0;
}

static inline
void bmath_cf3_s_mul_f3( bmath_cf3_s* o, const bmath_cf3_s* op1, f3_t op2 )
{
    o->v[ 0 ] = op1->v[ 0 ] * op2;
    o->v[ 1 ] = op1->v[ 1 ] * op2;
}

// o = op1 + ( op2 * op3 )
static inline
void bmath_cf3_s_add_mul( bmath_cf3_s* o, const bmath_cf3_s* op1, const bmath_cf3_s* op2, const bmath_cf3_s* op3 )
{
    f3_t re   = op1->v[ 0 ] + ( op2->v[ 0 ] * op3->v[ 0 ] - op2->v[ 1 ] * op3->v[ 1 ] );
    o->v[ 1 ] = op1->v[ 1 ] + ( op2->v[ 0 ] * op3->v[ 1 ] + op2->v[ 1 ] * op3->v[ 0 ] );
    o->v[ 0 ] = re;
}

// o = op1 - ( op2 * op3 )
static inline
void bmath_cf3_s_sub_mul( bmath_cf3_s* o, const bmath_cf3_s* op1, const bmath_cf3_s* op2, const bmath_cf3_s* op3 )
{
    f3_t re   = op1->v[ 0 ] - ( op2->v[ 0 ] * op3->v[ 0 ] - op2->v[ 1 ] * op3->v[ 1 ] );
    o->v[ 1 ] = op1->v[ 1 ] - ( op2->v[ 0 ] * op3->v[ 1 ] + op2->v[ 1 ] * op3->v[ 0 ] );
    o->v[ 0 ] = re;
}

// o = op1 * ( op2 - op3 )
static inline
void bmath_cf3_s_mul_sub( bmath_cf3_s* o, const bmath_cf3_s* op1, const bmath_cf3_s* op2, const bmath_cf3_s* op3 )
{
    f3_t dre = op2->v[ 0 ] - op3->v[ 0 ];
    f3_t dim = op2->v[ 1 ] - op3->v[ 1 ];

    f3_t re   = op1->v[ 0 ] * dre - op1->v[ 1 ] * dim;
    o->v[ 1 ] = op1->v[ 0 ] * dim + op1->v[ 1 ] * dre;
    o->v[ 0 ] = re;
}

static inline
void bmath_cf3_s_one( bmath_cf3_s* o )
{
    o->v[ 0 ] = 1;
    o->v[ 1 ] = 0;
}

static inline
f3_t bmath_cf3_det( bmath_cf3_s o )
{
    return f3_sqr( o.v[ 0 ] ) + f3_sqr( o.v[ 1 ] );
}

static inline
void bmath_cf3_s_inv( bmath_cf3_s* o, const bmath_cf3_s* op1 )
{
    f3_t inv = f3_inv( bmath_cf3_det( *op1 ) );
    o->v[ 0 ] =   op1->v[ 0 ] * inv;
    o->v[ 1 ] = - op1->v[ 1 ] * inv;
}

static inline
void bmath_cf3_s_div( bmath_cf3_s* o, const bmath_cf3_s* op1, const bmath_cf3_s* op2 )
{
    f3_t inv = f3_inv( bmath_cf3_det( *op2 ) );
    f3_t re   = ( op1->v[ 0 ] * op2->v[ 0 ] + op1->v[ 1 ] * op2->v[ 1 ] ) * inv;
    o->v[ 1 ] = ( op1->v[ 1 ] * op2->v[ 0 ] - op1->v[ 0 ] * op2->v[ 1 ] ) * inv;
    o->v[ 0 ] = re;
}

static inline
void bmath_cf3_s_cnj( bmath_cf3_s* o, const bmath_cf3_s* op1 )
{
    o->v[ 0 ] =  op1->v[ 0 ];
    o->v[ 1 ] = -op1->v[ 1 ];
}

static inline
void bmath_cf3_s_self_cnj( bmath_cf3_s* o )
{
    o->v[ 1 ] = -o->v[ 1 ];
}

static inline
f3_t bmath_cf3_mag( bmath_cf3_s o )
{
    return sqrt( bmath_cf3_det( o ) );
}

static inline
f3_t bmath_cf3_arg( bmath_cf3_s o )
{
    return ( o.v[ 0 ] != 0 || o.v[ 1 ] != 0 ) ? atan2( o.v[ 1 ], o.v[ 0 ] ) : 0;
}

static inline
bl_t bmath_cf3_equ( bmath_cf3_s o, bmath_cf3_s op1 )
{
    return o.v[ 0 ] == op1.v[ 0 ] && o.v[ 1 ] == op1.v[ 1 ];
}

/**********************************************************************************************************************/

vd_t bmath_complex_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_COMPLEX_H

