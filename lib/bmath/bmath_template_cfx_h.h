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
    return ( bmath_cfx_s ) { .r = r, .i = i };
}

/// compact initializer
static inline
bmath_cfx_s* BCATU(bmath,cfx,s,_)( bmath_cfx_s* o, fx_t r, fx_t i )
{
    o->r = r;
    o->i = i;
    return o;
}

/// init using mag, arg (radius, angle)
static inline
bmath_cfx_s BCATU(bmath,cfx,init_mag_arg)( fx_t mag, fx_t arg )
{
    return ( bmath_cfx_s ) { .r = mag * cos( arg ), .i = mag * sin( arg ) };
}

/// init using mag, arg (radius, angle)
static inline
bmath_cfx_s* BCATU(bmath,cfx,s,set_mag_arg)( bmath_cfx_s* o, fx_t mag, fx_t arg )
{
    o->r = mag * cos( arg );
    o->i = mag * sin( arg );
    return o;
}

/// urt-init (s. nomenclature); k can be negative indicating the circular direction.
static inline
bmath_cfx_s BCATU(bmath,cfx,init_urt)( s3_t k, s3_t n )
{
    fx_t arg = ( BCATU(fx,tau)() * k ) / n;
    return ( bmath_cfx_s ) { .r = cos( arg ), .i = sin( arg ) };
}

static inline
bmath_cfx_s BCATU(bmath,cfx,zro)( void )
{
    return ( bmath_cfx_s ) { .r = 0, .i = 0 };
}

static inline
bmath_cfx_s BCATU(bmath,cfx,one)( void )
{
    return ( bmath_cfx_s ) { .r = 1, .i = 0 };
}

static inline
bl_t BCATU(bmath_cfx_s,is_equ)( const bmath_cfx_s* o, const bmath_cfx_s* op )
{
    return ( o->r == op->r ) && ( o->i == op->i );
}

static inline
bl_t BCATU(bmath_cfx_s,is_zro)( const bmath_cfx_s* o )
{
    return ( o->r == 0 ) && ( o->i == 0 );
}

static inline
bl_t BCATU(bmath_cfx_s,is_one)( const bmath_cfx_s* o )
{
    return ( o->r == 1.0 ) && ( o->i == 0 );
}

static inline
void BCATU(bmath_cfx_s,add)( const bmath_cfx_s* o, const bmath_cfx_s* op, bmath_cfx_s* res )
{
    res->r = o->r + op->r;
    res->i = o->i + op->i;
}

static inline
void BCATU(bmath_cfx_s,zro)( bmath_cfx_s* o )
{
    o->r = 0;
    o->i = 0;
}

/// srt to urt (s. nomenclature); k can be negative indicating the circular direction.
static inline
bmath_cfx_s* BCATU(bmath_cfx_s,urt)( bmath_cfx_s* o, s3_t k, s3_t n )
{
    fx_t arg = ( BCATU(fx,tau)() * k ) / n;
    o->r = cos( arg );
    o->i = sin( arg );
    return o;
}

static inline
void BCATU(bmath_cfx_s,neg)( const bmath_cfx_s* o, bmath_cfx_s* res )
{
    res->r = -o->r;
    res->i = -o->i;
}

static inline
bmath_cfx_s BCATU(bmath,cfx,neg)( bmath_cfx_s o )
{
    return ( bmath_cfx_s ) { .r = -o.r, .i = -o.i };
}

static inline
void BCATU(bmath_cfx_s,sub)( const bmath_cfx_s* o, const bmath_cfx_s* op, bmath_cfx_s* res )
{
    res->r = o->r - op->r;
    res->i = o->i - op->i;
}

static inline
void BCATU(bmath_cfx_s,mul)( const bmath_cfx_s* o, const bmath_cfx_s* op, bmath_cfx_s* res )
{
    fx_t r = o->r * op->r - o->i * op->i;
    res->i = o->r * op->i + o->i * op->r;
    res->r = r;
}

static inline
bmath_cfx_s BCATU(bmath,cfx,mul)( bmath_cfx_s op1, bmath_cfx_s op2 )
{
    fx_t re = op1.r * op2.r - op1.i * op2.i;
    fx_t im = op1.r * op2.i + op1.i * op2.r;
    return ( bmath_cfx_s ) { .r = re, .i = im };
}

static inline
bmath_cfx_s BCATU(bmath,cfx,mul,fx)( bmath_cfx_s op1, fx_t op2 )
{
    return ( bmath_cfx_s ) { .r = op1.r * op2, .i = op1.i * op2 };
}

static inline
void BCATU(bmath_cfx_s,sqr)( const bmath_cfx_s* o, bmath_cfx_s* res )
{
    fx_t re     = o->r * o->r - o->i * o->i;
    res->i = o->r * o->i * 2.0;
    res->r = re;
}

// res = ( o - op )^2
static inline
void BCATU(bmath_cfx_s,sub_sqr)( const bmath_cfx_s* o, const bmath_cfx_s* op, bmath_cfx_s* res )
{
    fx_t dre = o->r - op->r;
    fx_t dim = o->i - op->i;
    res->r = dre * dre - dim * dim;
    res->i = dre * dim * 2.0;
}

// res = o^2 + op
static inline
void BCATU(bmath_cfx_s,sqr_add)( const bmath_cfx_s* o, const bmath_cfx_s* op, bmath_cfx_s* res )
{
    fx_t re = op->r + o->r * o->r - o->i * o->i;
    fx_t im = op->i + o->r * o->i * 2.0;
    res->r = re;
    res->i = im;
}

static inline
void BCATU(bmath_cfx_s,mul,fx)( const bmath_cfx_s* o, fx_t op, bmath_cfx_s* res )
{
    res->r = o->r * op;
    res->i = o->i * op;
}

// res = o + ( op1 * op2 )
static inline
void BCATU(bmath_cfx_s,add_prod)( const bmath_cfx_s* o, const bmath_cfx_s* op1, const bmath_cfx_s* op2, bmath_cfx_s* res )
{
    fx_t re = o->r + ( op1->r * op2->r - op1->i * op2->i );
    res->i = o->i + ( op1->r * op2->i + op1->i * op2->r );
    res->r = re;
}

// res = o - ( op1 * op2 )
static inline
void BCATU(bmath_cfx_s,sub_prod)( const bmath_cfx_s* o, const bmath_cfx_s* op1, const bmath_cfx_s* op2, bmath_cfx_s* res )
{
    fx_t re = o->r - ( op1->r * op2->r - op1->i * op2->i );
    res->i = o->i - ( op1->r * op2->i + op1->i * op2->r );
    res->r = re;
}

// res = o * ( op1 - op2 )
static inline
void BCATU(bmath_cfx_s,mul_diff)( const bmath_cfx_s* o, const bmath_cfx_s* op1, const bmath_cfx_s* op2, bmath_cfx_s* res )
{
    fx_t dre = op1->r - op2->r;
    fx_t dim = op1->i - op2->i;

    fx_t re = o->r * dre - o->i * dim;
    res->i = o->r * dim + o->i * dre;
    res->r = re;
}

static inline
void BCATU(bmath_cfx_s,one)( bmath_cfx_s* o )
{
    o->r = 1;
    o->i = 0;
}

static inline
fx_t BCATU(bmath,cfx,det)( bmath_cfx_s o )
{
    return BCATU(fx,sqr)( o.r ) + BCATU(fx,sqr)( o.i );
}

/// total sum of squares (same as det)
static inline
fx_t BCATU(bmath,cfx,tss)( bmath_cfx_s o )
{
    return BCATU(fx,sqr)( o.r ) + BCATU(fx,sqr)( o.i );
}

/// total sum of squares (same as det)
static inline
fx_t BCATU(bmath,cfx,s,tss)( const bmath_cfx_s* o )
{
    return BCATU(fx,sqr)( o->r ) + BCATU(fx,sqr)( o->i );
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
    return ( o->r != 0 || o->i != 0 ) ? atan2( o->i, o->r ) : 0;
}

static inline
void BCATU(bmath_cfx_s,inv)( const bmath_cfx_s* o, bmath_cfx_s* res )
{
    fx_t inv = BCATU(fx,inv)( BCATU(bmath,cfx,det)( *o ) );
    res->r =   o->r * inv;
    res->i = - o->i * inv;
}

static inline
void BCATU(bmath_cfx_s,div)( const bmath_cfx_s* o, const bmath_cfx_s* op, bmath_cfx_s* res )
{
    fx_t inv = BCATU(fx,inv)( BCATU(bmath,cfx,det)( *op ) );
    fx_t re = ( o->r * op->r + o->i * op->i ) * inv;
    res->i = ( o->i * op->r - o->r * op->i ) * inv;
    res->r = re;
}

static inline
void BCATU(bmath_cfx_s,cnj)( const bmath_cfx_s* o, bmath_cfx_s* res )
{
    res->r =  o->r;
    res->i = -o->i;
}

static inline
void BCATU(bmath_cfx_s,self_cnj)( bmath_cfx_s* o )
{
    o->i = -o->i;
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
    return ( o.r != 0 || o.i != 0 ) ? atan2( o.i, o.r ) : 0;
}

static inline
bl_t BCATU(bmath,cfx,equ)( bmath_cfx_s o, bmath_cfx_s op1 )
{
    return o.r == op1.r && o.i == op1.i;
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

