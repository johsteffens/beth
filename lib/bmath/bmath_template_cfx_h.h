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
 *  arg - phase atan2( im, re )  (range -pi , ..., pi)
 *  urt - (unity-root) kth element of nth-roots of unity: urt(k,n) = e^i*2*pi*k/n
 */

/**********************************************************************************************************************/

#include "bmath_template_fx_begin.h"

/**********************************************************************************************************************/

static inline
bmath_cfx_s BCATU(bmath,cfx,init)( fx_t r, fx_t i )
{
    return ( bmath_cfx_s ) { .v[0] = r, .v[1] = i };
}

/// init using mag, arg (radius, angle)
static inline
bmath_cfx_s BCATU(bmath,cfx,init_mag_arg)( fx_t mag, fx_t arg )
{
    return ( bmath_cfx_s ) { .v[0] = mag * cos( arg ), .v[1] = mag * sin( arg ) };
}

/// init using mag, arg (radius, angle)
static inline
bmath_cfx_s* BCATU(bmath,cfx,s,set_mag_arg)( bmath_cfx_s* o, fx_t mag, fx_t arg )
{
    o->v[ 0 ] = mag * cos( arg );
    o->v[ 1 ] = mag * sin( arg );
    return o;
}

/// urt-init (s. nomenclature); k can be negative indicating the circular direction.
static inline
bmath_cfx_s BCATU(bmath,cfx,init_urt)( s3_t k, s3_t n )
{
    fx_t arg = ( BCATU(fx,tau)() * k ) / n;
    return ( bmath_cfx_s ) { .v[0] = cos( arg ), .v[1] = sin( arg ) };
}

static inline
bmath_cfx_s BCATU(bmath,cfx,zro)( void )
{
    return ( bmath_cfx_s ) { .v[0] = 0, .v[1] = 0 };
}

static inline
bmath_cfx_s BCATU(bmath,cfx,one)( void )
{
    return ( bmath_cfx_s ) { .v[0] = 1, .v[1] = 0 };
}

static inline
bl_t BCATU(bmath_cfx_s,is_equ)( const bmath_cfx_s* o, const bmath_cfx_s* op )
{
    return ( o->v[ 0 ] == op->v[ 0 ] ) && ( o->v[ 1 ] == op->v[ 1 ] );
}

static inline
bl_t BCATU(bmath_cfx_s,is_zro)( const bmath_cfx_s* o )
{
    return ( o->v[ 0 ] == 0 ) && ( o->v[ 1 ] == 0 );
}

static inline
bl_t BCATU(bmath_cfx_s,is_one)( const bmath_cfx_s* o )
{
    return ( o->v[ 0 ] == 1.0 ) && ( o->v[ 1 ] == 0 );
}

static inline
void BCATU(bmath_cfx_s,add)( const bmath_cfx_s* o, const bmath_cfx_s* op, bmath_cfx_s* res )
{
    res->v[ 0 ] = o->v[ 0 ] + op->v[ 0 ];
    res->v[ 1 ] = o->v[ 1 ] + op->v[ 1 ];
}

static inline
void BCATU(bmath_cfx_s,zro)( bmath_cfx_s* o )
{
    o->v[ 0 ] = 0;
    o->v[ 1 ] = 0;
}

/// srt to urt (s. nomenclature); k can be negative indicating the circular direction.
static inline
bmath_cfx_s* BCATU(bmath_cfx_s,urt)( bmath_cfx_s* o, s3_t k, s3_t n )
{
    fx_t arg = ( BCATU(fx,tau)() * k ) / n;
    o->v[0] = cos( arg );
    o->v[1] = sin( arg );
    return o;
}

static inline
void BCATU(bmath_cfx_s,neg)( const bmath_cfx_s* o, bmath_cfx_s* res )
{
    res->v[ 0 ] = -o->v[ 0 ];
    res->v[ 1 ] = -o->v[ 1 ];
}

static inline
bmath_cfx_s BCATU(bmath,cfx,neg)( bmath_cfx_s o )
{
    return ( bmath_cfx_s ) { .v[0] = -o.v[ 0 ], .v[1] = -o.v[ 1 ] };
}

static inline
void BCATU(bmath_cfx_s,sub)( const bmath_cfx_s* o, const bmath_cfx_s* op, bmath_cfx_s* res )
{
    res->v[ 0 ] = o->v[ 0 ] - op->v[ 0 ];
    res->v[ 1 ] = o->v[ 1 ] - op->v[ 1 ];
}

static inline
void BCATU(bmath_cfx_s,mul)( const bmath_cfx_s* o, const bmath_cfx_s* op, bmath_cfx_s* res )
{
    fx_t re     = o->v[ 0 ] * op->v[ 0 ] - o->v[ 1 ] * op->v[ 1 ];
    res->v[ 1 ] = o->v[ 0 ] * op->v[ 1 ] + o->v[ 1 ] * op->v[ 0 ];
    res->v[ 0 ] = re;
}

static inline
bmath_cfx_s BCATU(bmath,cfx,mul)( bmath_cfx_s op1, bmath_cfx_s op2 )
{
    fx_t re = op1.v[ 0 ] * op2.v[ 0 ] - op1.v[ 1 ] * op2.v[ 1 ];
    fx_t im = op1.v[ 0 ] * op2.v[ 1 ] + op1.v[ 1 ] * op2.v[ 0 ];
    return ( bmath_cfx_s ) { .v[0] = re, .v[1] = im };
}

static inline
bmath_cfx_s BCATU(bmath,cfx,mul,fx)( bmath_cfx_s op1, fx_t op2 )
{
    return ( bmath_cfx_s ) { .v[0] = op1.v[ 0 ] * op2, .v[1] = op1.v[ 1 ] * op2 };
}

static inline
void BCATU(bmath_cfx_s,sqr)( const bmath_cfx_s* o, bmath_cfx_s* res )
{
    fx_t re     = o->v[ 0 ] * o->v[ 0 ] - o->v[ 1 ] * o->v[ 1 ];
    res->v[ 1 ] = o->v[ 0 ] * o->v[ 1 ] * 2.0;
    res->v[ 0 ] = re;
}

// res = ( o - op )^2
static inline
void BCATU(bmath_cfx_s,sub_sqr)( const bmath_cfx_s* o, const bmath_cfx_s* op, bmath_cfx_s* res )
{
    fx_t dre  = o->v[ 0 ] - op->v[ 0 ];
    fx_t dim  = o->v[ 1 ] - op->v[ 1 ];
    res->v[ 0 ] = dre * dre - dim * dim;
    res->v[ 1 ] = dre * dim * 2.0;
}

// res = o^2 + op
static inline
void BCATU(bmath_cfx_s,sqr_add)( const bmath_cfx_s* o, const bmath_cfx_s* op, bmath_cfx_s* res )
{
    fx_t re = op->v[0] + o->v[ 0 ] * o->v[ 0 ] - o->v[ 1 ] * o->v[ 1 ];
    fx_t im = op->v[1] + o->v[ 0 ] * o->v[ 1 ] * 2.0;
    res->v[ 0 ] = re;
    res->v[ 1 ] = im;
}

static inline
void BCATU(bmath_cfx_s,mul,fx)( const bmath_cfx_s* o, fx_t op, bmath_cfx_s* res )
{
    res->v[ 0 ] = o->v[ 0 ] * op;
    res->v[ 1 ] = o->v[ 1 ] * op;
}

// res = o + ( op1 * op2 )
static inline
void BCATU(bmath_cfx_s,add_prod)( const bmath_cfx_s* o, const bmath_cfx_s* op1, const bmath_cfx_s* op2, bmath_cfx_s* res )
{
    fx_t re     = o->v[ 0 ] + ( op1->v[ 0 ] * op2->v[ 0 ] - op1->v[ 1 ] * op2->v[ 1 ] );
    res->v[ 1 ] = o->v[ 1 ] + ( op1->v[ 0 ] * op2->v[ 1 ] + op1->v[ 1 ] * op2->v[ 0 ] );
    res->v[ 0 ] = re;
}

// res = o - ( op1 * op2 )
static inline
void BCATU(bmath_cfx_s,sub_prod)( const bmath_cfx_s* o, const bmath_cfx_s* op1, const bmath_cfx_s* op2, bmath_cfx_s* res )
{
    fx_t re     = o->v[ 0 ] - ( op1->v[ 0 ] * op2->v[ 0 ] - op1->v[ 1 ] * op2->v[ 1 ] );
    res->v[ 1 ] = o->v[ 1 ] - ( op1->v[ 0 ] * op2->v[ 1 ] + op1->v[ 1 ] * op2->v[ 0 ] );
    res->v[ 0 ] = re;
}

// res = o * ( op1 - op2 )
static inline
void BCATU(bmath_cfx_s,mul_diff)( const bmath_cfx_s* o, const bmath_cfx_s* op1, const bmath_cfx_s* op2, bmath_cfx_s* res )
{
    fx_t dre = op1->v[ 0 ] - op2->v[ 0 ];
    fx_t dim = op1->v[ 1 ] - op2->v[ 1 ];

    fx_t re     = o->v[ 0 ] * dre - o->v[ 1 ] * dim;
    res->v[ 1 ] = o->v[ 0 ] * dim + o->v[ 1 ] * dre;
    res->v[ 0 ] = re;
}

static inline
void BCATU(bmath_cfx_s,one)( bmath_cfx_s* o )
{
    o->v[ 0 ] = 1;
    o->v[ 1 ] = 0;
}

static inline
fx_t BCATU(bmath,cfx,det)( bmath_cfx_s o )
{
    return BCATU(fx,sqr)( o.v[ 0 ] ) + BCATU(fx,sqr)( o.v[ 1 ] );
}

/// total sum of squares (same as det)
static inline
fx_t BCATU(bmath,cfx,tss)( bmath_cfx_s o )
{
    return BCATU(fx,sqr)( o.v[ 0 ] ) + BCATU(fx,sqr)( o.v[ 1 ] );
}

/// total sum of squares (same as det)
static inline
fx_t BCATU(bmath,cfx,s,tss)( const bmath_cfx_s* o )
{
    return BCATU(fx,sqr)( o->v[ 0 ] ) + BCATU(fx,sqr)( o->v[ 1 ] );
}

/// magnitude (srt(tss))
static inline
fx_t BCATU(bmath,cfx,s,mag)( const bmath_cfx_s* o )
{
    return BCATU(fx,srt)( BCATU(bmath,cfx,s,tss)( o ) );
}

/// phase
static inline
fx_t BCATU(bmath,cfx,s,arg)( const bmath_cfx_s* o )
{
    return ( o->v[ 0 ] != 0 || o->v[ 1 ] != 0 ) ? atan2( o->v[ 1 ], o->v[ 0 ] ) : 0;
}

static inline
void BCATU(bmath_cfx_s,inv)( const bmath_cfx_s* o, bmath_cfx_s* res )
{
    fx_t inv = BCATU(fx,inv)( BCATU(bmath,cfx,det)( *o ) );
    res->v[ 0 ] =   o->v[ 0 ] * inv;
    res->v[ 1 ] = - o->v[ 1 ] * inv;
}

static inline
void BCATU(bmath_cfx_s,div)( const bmath_cfx_s* o, const bmath_cfx_s* op, bmath_cfx_s* res )
{
    fx_t inv = BCATU(fx,inv)( BCATU(bmath,cfx,det)( *op ) );
    fx_t re     = ( o->v[ 0 ] * op->v[ 0 ] + o->v[ 1 ] * op->v[ 1 ] ) * inv;
    res->v[ 1 ] = ( o->v[ 1 ] * op->v[ 0 ] - o->v[ 0 ] * op->v[ 1 ] ) * inv;
    res->v[ 0 ] = re;
}

static inline
void BCATU(bmath_cfx_s,cnj)( const bmath_cfx_s* o, bmath_cfx_s* res )
{
    res->v[ 0 ] =  o->v[ 0 ];
    res->v[ 1 ] = -o->v[ 1 ];
}

static inline
void BCATU(bmath_cfx_s,self_cnj)( bmath_cfx_s* o )
{
    o->v[ 1 ] = -o->v[ 1 ];
}

static inline
fx_t BCATU(bmath,cfx,mag)( bmath_cfx_s o )
{
    return sqrt( BCATU(bmath,cfx,det)( o ) );
}

static inline
fx_t BCATU(bmath,cfx,diff_mag)( bmath_cfx_s o, bmath_cfx_s b )
{
    bmath_cfx_s r;
    BCATU(bmath_cfx_s,sub)( &o, &b, &r );
    return sqrt( BCATU(bmath,cfx,det)( r ) );
}

static inline
fx_t BCATU(bmath,cfx,arg)( bmath_cfx_s o )
{
    return ( o.v[ 0 ] != 0 || o.v[ 1 ] != 0 ) ? atan2( o.v[ 1 ], o.v[ 0 ] ) : 0;
}

static inline
bl_t BCATU(bmath,cfx,equ)( bmath_cfx_s o, bmath_cfx_s op1 )
{
    return o.v[ 0 ] == op1.v[ 0 ] && o.v[ 1 ] == op1.v[ 1 ];
}

/// For easy inspection
void BCATU(bmath_cfx_s,to_stdout)( const bmath_cfx_s* o );

/**********************************************************************************************************************/
// type conversion

void BCATU(bmath_cfx_s,copy_typed)( bmath_cfx_s* o, tp_t type, vc_t src );

static inline void BCATU(bmath_cfx_s,copy_t)( bmath_cfx_s* o, tp_t type, vc_t src )
{
    BCATU(bmath_cfx_s,copy_typed)( o, type, src );
}

static inline bmath_cfx_s* BCATU(bmath_cfx_s,clone_t)( tp_t type, vc_t src )
{
    if( !src ) return NULL;
    bmath_cfx_s* o = BCATU(bmath_cfx_s,create)();
    BCATU(bmath_cfx_s,copy_t)( o, type, src );
    return o;
}

/**********************************************************************************************************************/

#include "bmath_template_fx_end.h"

/**********************************************************************************************************************/

