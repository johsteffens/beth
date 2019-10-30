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

/** Template: Complex Vector */

/**********************************************************************************************************************/

#include "bmath_template_fx_begin.h"

/**********************************************************************************************************************/
/// checks, deviations

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath_vcfx_s,is_near_equ)( const bmath_vcfx_s* o, const bmath_vcfx_s* op, fx_t max_dev )
{
    if( o->size != op->size ) return false;
    for( sz_t i = 0; i < o->size; i++ )
    {
        if( BCATU(fx,abs)( o->data[ i ].v[ 0 ] - op->data[ i ].v[ 0 ] ) > max_dev ) return false;
        if( BCATU(fx,abs)( o->data[ i ].v[ 1 ] - op->data[ i ].v[ 1 ] ) > max_dev ) return false;
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath_vcfx_s,is_near_zro)( const bmath_vcfx_s* o, fx_t max_dev )
{
    for( sz_t i = 0; i < o->size; i++ )
    {
        if( BCATU(fx,abs)( o->data[ i ].v[ 0 ] ) > max_dev ) return false;
        if( BCATU(fx,abs)( o->data[ i ].v[ 1 ] ) > max_dev ) return false;
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath_vcfx_s,is_nan)( const bmath_vcfx_s* o )
{
    for( sz_t i = 0; i < o->size; i++ )
    {
        if( BCATU(fx,is_nan)( o->data[ i ].v[ 0 ] ) ) return true;
        if( BCATU(fx,is_nan)( o->data[ i ].v[ 1 ] ) ) return true;
    }
    return false;
}

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath_vcfx_s,tss)( const bmath_vcfx_s* o )
{
    fx_t sum = 0;
    for( sz_t i = 0; i < o->size; i++ )
    {
        sum += BCATU(bmath_cfx_s,tss)( &o->data[ i ] );
    }
    return sum;
}

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath_vcfx_s,fdev)( const bmath_vcfx_s* o, const bmath_vcfx_s* op )
{
    ASSERT( o->size == op->size );
    fx_t sum = 0;
    for( sz_t i = 0 ; i < o->size; i++ )
    {
        sum += BCATU(fx,sqr)( o->data[ i ].v[ 0 ] - op->data[ i ].v[ 0 ] )
            +  BCATU(fx,sqr)( o->data[ i ].v[ 1 ] - op->data[ i ].v[ 1 ] );
    }
    return BCATU(fx,srt)( sum );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vcfx_s,move)( bmath_vcfx_s* o, bmath_vcfx_s* src )
{
    if( o == src ) return;
    *o = *src;
    o->space = 0;
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vcfx_s,clear)( bmath_vcfx_s* o )
{
    bcore_array_a_set_space( (bcore_array*)o, 0 );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vcfx_s,set_size)( bmath_vcfx_s* o, uz_t size )
{
    bcore_array_a_set_size( (bcore_array*)o, size );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vcfx_s,set_space)( bmath_vcfx_s* o, uz_t space )
{
    bcore_array_a_set_space( (bcore_array*)o, space );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vcfx_s,push)( bmath_vcfx_s* o, bmath_cfx_s val )
{
    if( o->size < o->space )
    {
        o->data[ o->size++ ] = val;
    }
    else
    {
        bcore_array_a_push( (bcore_array*)o, sr_twc( BCATU(TYPEOF_bmath,cfx,s), &val ) );
    }
}

void BCATU(bmath_vcfx_s,push_ri)( bmath_vcfx_s* o, fx_t r, fx_t i )
{
    BCATU(bmath_vcfx_s,push)( o, BCATU(bmath,cfx,init)( r, i ) );
}

//----------------------------------------------------------------------------------------------------------------------

bmath_vcfx_s* BCATU(bmath_vcfx_s,create_size)( uz_t size )
{
    bmath_vcfx_s* o = BCATU(bmath_vcfx_s,create)();
    bcore_array_a_set_size( (bcore_array*)o, size );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

bmath_vcfx_s* BCATU(bmath_vcfx_s,create_fill)( bmath_cfx_s val, uz_t size )
{
    bmath_vcfx_s* o = BCATU(bmath_vcfx_s,create)();
    bcore_array_a_set_space( (bcore_array*)o, size );
    o->size = size;
    for( uz_t i = 0; i < size; i++ ) o->data[ i ] = val;
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vcfx_s,set_random)( bmath_vcfx_s* o, fx_t density, fx_t min, fx_t max, u2_t* p_rval )
{
    u2_t rval = p_rval ? *p_rval : 12345;
    fx_t range = max - min;
    for( uz_t i = 0; i < o->size; i++ )
    {
        if( f3_xsg1_pos( &rval ) < density )
        {
            o->data[ i ].v[ 0 ] = ( range * f3_xsg1_pos( &rval ) ) + min;
            o->data[ i ].v[ 1 ] = ( range * f3_xsg1_pos( &rval ) ) + min;
        }
        else
        {
            o->data[ i ].v[ 0 ] = 0;
            o->data[ i ].v[ 1 ] = 0;
        }
    }
    if( p_rval ) *p_rval = rval;
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vcfx_s,zro)( bmath_vcfx_s* o )
{
    bcore_u_memzero( sizeof( bmath_cfx_s ), o->data, o->size );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vcfx_s,neg)( const bmath_vcfx_s* o, bmath_vcfx_s* res )
{
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ ) BCATU(bmath,cfx,s_neg)( &o->data[ i ], &res->data[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vcfx_s,cnj)( const bmath_vcfx_s* o, bmath_vcfx_s* res )
{
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ ) BCATU(bmath,cfx,s_cnj)( &o->data[ i ], &res->data[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vcfx_s,cpy)( const bmath_vcfx_s* o, bmath_vcfx_s* res )
{
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ ) res->data[ i ] = o->data[ i ];
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vcfx_s,add)( const bmath_vcfx_s* o, const bmath_vcfx_s* op, bmath_vcfx_s* res )
{
    ASSERT( o->size == op->size );
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ ) BCATU(bmath,cfx,s_add)( &o->data[ i ], &op->data[ i ], &res->data[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vcfx_s,sub)( const bmath_vcfx_s* o, const bmath_vcfx_s* op, bmath_vcfx_s* res )
{
    ASSERT( o->size == op->size );
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ ) BCATU(bmath,cfx,s_sub)( &o->data[ i ], &op->data[ i ], &res->data[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vcfx_s,mul_scl)( const bmath_vcfx_s* o, const bmath_cfx_s* op, bmath_vcfx_s* res )
{
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ ) BCATU(bmath,cfx,s_mul)( &o->data[ i ], op, &res->data[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vcfx_s,mul_cfx)( const bmath_vcfx_s* o, bmath_cfx_s op, bmath_vcfx_s* res )
{
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ ) BCATU(bmath,cfx,s_mul)( &o->data[ i ], &op, &res->data[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vcfx_s,mul_fx)( const bmath_vcfx_s* o, fx_t op, bmath_vcfx_s* res )
{
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ ) BCATU(bmath,cfx,s_mul,fx)( &o->data[ i ], op, &res->data[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vcfx_s,dft)( const bmath_vcfx_s* o, bmath_vcfx_s* res )
{
    ASSERT( o->size == res->size );
    if( ( o->size & ( o->size - 1 ) ) == 0 )
    {
        BCATU(bmath_fourier,fx,fft)( o->data, res->data, o->size );
    }
    else
    {
        BCATU(bmath_fourier,fx,dft)( o->data, res->data, o->size );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vcfx_s,ift)( const bmath_vcfx_s* o, bmath_vcfx_s* res )
{
    BCATU(bmath_vcfx_s,cnj)( o, res );
    BCATU(bmath_vcfx_s,dft)( res, res );
    BCATU(bmath_vcfx_s,cnj)( res, res );
    if( res->size > 0 ) BCATU(bmath_vcfx_s,mul_fx)( res, 1.0 / res->size, res );
}

//----------------------------------------------------------------------------------------------------------------------

static void vcfx_s_mul_vec( const bmath_cfx_s* v1, const bmath_cfx_s* v2, uz_t size, bmath_cfx_s* res )
{
    switch( size )
    {
        case 0:
        {
            BCATU(bmath,cfx,s_zro)( res );
            return;
        }

        case 1:
        {
            BCATU(bmath,cfx,s_mul)( &v1[0], &v2[0], res );
            return;
        }

        case 2:
        {
            BCATU(bmath,cfx,s_mul)( &v1[0], &v2[0], res );
            BCATU(bmath,cfx,s_add_prod)( res, &v1[1], &v2[1], res );
            return;
        }

        default: break;
    }

    uz_t sz1 = size >> 1;
    bmath_cfx_s val;
    vcfx_s_mul_vec( v1, v2, sz1, res );
    vcfx_s_mul_vec( v1 + sz1, v2 + sz1, size - sz1, &val );
    BCATU(bmath,cfx,s_add)( res, &val, res );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vcfx_s,mul_vec)( const bmath_vcfx_s* o, const bmath_vcfx_s* op, bmath_cfx_s* res )
{
    ASSERT( o->size == op->size );
    vcfx_s_mul_vec( o->data, op->data, o->size, res );
}

//----------------------------------------------------------------------------------------------------------------------

static void vcfx_s_sqr( const bmath_cfx_s* o, uz_t size, bmath_cfx_s* res )
{
    switch( size )
    {
        case 0:
        {
            BCATU(bmath,cfx,s_zro)( res );
            return;
        }

        case 1:
        {
            BCATU(bmath,cfx,s_sqr)( &o[0], res );
            return;
        }

        case 2:
        {
            BCATU(bmath,cfx,s_sqr)( &o[0], res );
            BCATU(bmath,cfx,s_sqr_add)( &o[1], res, res );
            return;
        }

        default: break;
    }

    uz_t sz1 = size >> 1;
    bmath_cfx_s val;
    vcfx_s_sqr( o, sz1, res );
    vcfx_s_sqr( o + sz1, size - sz1, &val );
    BCATU(bmath,cfx,s_add)( res, &val, res );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vcfx_s,sqr)( const bmath_vcfx_s* o, bmath_cfx_s* res )
{
    vcfx_s_sqr( o->data, o->size, res );
}

//----------------------------------------------------------------------------------------------------------------------

static void vcfx_s_sub_sqr( const bmath_cfx_s* v1, const bmath_cfx_s* v2, uz_t size, bmath_cfx_s* res )
{
    switch( size )
    {
        case 0:
        {
            BCATU(bmath,cfx,s_zro)( res );
            return;
        }

        case 1:
        {
            BCATU(bmath,cfx,s_sub_sqr)( &v1[0], &v2[0], res );
            return;
        }

        case 2:
        {
            bmath_cfx_s val;
            BCATU(bmath,cfx,s_sub_sqr)( &v1[0], &v2[0], res );
            BCATU(bmath,cfx,s_sub_sqr)( &v1[1], &v2[1], &val );
            BCATU(bmath,cfx,s_add)( res, &val, res );
            return;
        }

        default: break;
    }

    uz_t sz1 = size >> 1;
    bmath_cfx_s val;
    vcfx_s_sub_sqr( v1, v2, sz1, res );
    vcfx_s_sub_sqr( v1 + sz1, v2 + sz1, size - sz1, &val );
    BCATU(bmath,cfx,s_add)( res, &val, res );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vcfx_s,sub_sqr)( const bmath_vcfx_s* o, const bmath_vcfx_s* op, bmath_cfx_s* res )
{
    ASSERT( o->size == op->size );
    vcfx_s_sub_sqr( o->data, op->data, o->size, res );
}

//----------------------------------------------------------------------------------------------------------------------

static void vcfx_s_sum( const bmath_cfx_s* v1, uz_t size, bmath_cfx_s* res )
{
    switch( size )
    {
        case 0:
        {
            BCATU(bmath,cfx,s_zro)( res );
            return;
        }

        case 1:
        {
            *res = v1[0];
            return;
        }

        case 2:
        {
            res->v[0]=v1[0].v[0]+v1[1].v[0];
            res->v[1]=v1[0].v[1]+v1[1].v[1];
            return;
        }

        case 3:
        {
            res->v[0]=v1[0].v[0]+v1[1].v[0]+v1[2].v[0];
            res->v[1]=v1[0].v[1]+v1[1].v[1]+v1[2].v[1];
            return;
        }

        case 4:
        {
            res->v[0]=v1[0].v[0]+v1[1].v[0]+v1[2].v[0]+v1[3].v[0];
            res->v[1]=v1[0].v[1]+v1[1].v[1]+v1[2].v[1]+v1[3].v[1];
            return;
        }

        default: break;
    }

    uz_t sz1 = size >> 1;
    bmath_cfx_s val;
    vcfx_s_sum( v1, sz1, res );
    vcfx_s_sum( v1 + sz1, size - sz1, &val );
    BCATU(bmath,cfx,s_add)( res, &val, res );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vcfx_s,sum)( const bmath_vcfx_s* o, bmath_cfx_s* res )
{
    vcfx_s_sum( o->data, o->size, res );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vcfx_s,fx_avg)( const bmath_vcfx_s* o, bmath_cfx_s* res )
{
    if( o->size > 0 )
    {
        BCATU(bmath_vcfx_s,sum)( o, res );
        BCATU(bmath,cfx,s_mul,fx)( res, 1.0 / o->size, res );
    }
    else
    {
        BCATU(bmath,cfx,s_zro)( res );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vcfx_s,to_stdout)( const bmath_vcfx_s* o )
{
    const bmath_cfx_s* v = o->data;
    for( uz_t i = 0; i < o->size; i++ ) BCATU(bmath,cfx,s_to_stdout)( &v[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// type conversion

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vcfx_s,copy_typed)( bmath_vcfx_s* o, tp_t type, vc_t src )
{
    if( !src )
    {
        BCATU(bmath_vcfx_s,clear)( o );
        return;
    }

    switch( type )
    {
        case TYPEOF_bmath_vcfx_s:
        {
            BCATU(bmath_vcfx_s,copy)( o, src );
        }
        break;

        case TYPEOF_bmath_vcfy_s:
        {
            const bmath_vcfy_s* v = src;
            BCATU(bmath_vcfx_s,set_size)( o, v->size );
            BFOR_EACH( i, o )
            {
                o->data[ i ].v[ 0 ] = v->data[ i ].v[ 0 ];
                o->data[ i ].v[ 1 ] = v->data[ i ].v[ 1 ];
            }
        }
        break;

        default:
        {
            bcore_err_fa( "Cannot copy from #<sc_t>.", ifnameof( type ) );
        }
        break;
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

static vd_t selftest( void )
{
    bcore_life_s* l = bcore_life_s_create();

    /// cvfx
    {
        uz_t size = 100;
        bmath_vcfx_s* v1 = bcore_life_s_push_aware( l, BCATU(bmath_vcfx_s,create_size)( size ) );

        //u2_t rval = 1234;
        //BCATU(bmath_vcfx_s,set_random)( v1, 1.0, -1.0, 1.0, &rval );


        for( uz_t i = 0; i < size; i++ )
        {
            v1->data[ i ].v[ 0 ] = i + 1;
            v1->data[ i ].v[ 1 ] = i;
        }


        bmath_cfx_s sum1 = BCATU(bmath,cfx,zro)();
        bmath_cfx_s sqr1 = BCATU(bmath,cfx,zro)();
        for( uz_t i = 0; i < size; i++ )
        {
            BCATU(bmath,cfx,s_sqr_add)( &v1->data[ i ], &sqr1, &sqr1 );
            BCATU(bmath,cfx,s_add)( &v1->data[ i ], &sum1, &sum1  );
        }

        bmath_cfx_s sum2 = BCATU(bmath,cfx,zro)();
        bmath_cfx_s sqr2 = BCATU(bmath,cfx,zro)();
        BCATU(bmath_vcfx_s,sum)( v1, &sum2 );
        BCATU(bmath_vcfx_s,sqr)( v1, &sqr2 );

        ASSERT( BCATU(bmath,cfx,equ)( sum1, sum2 ) );
        ASSERT( BCATU(bmath,cfx,equ)( sqr1, sqr2 ) );

        bmath_vcfx_s* v2 = bcore_life_s_push_aware( l, BCATU(bmath_vcfx_s,create_size)( size ) );
        BCATU(bmath_vcfx_s,cpy)( v1, v2 );
        BCATU(bmath_vcfx_s,sum)( v1, &sum1 );
        BCATU(bmath_vcfx_s,sum)( v2, &sum2 );
        ASSERT( BCATU(bmath,cfx,equ)( sum1, sum2 ) );

        BCATU(bmath_vcfx_s,neg)( v1, v2 );
        BCATU(bmath_vcfx_s,sum)( v1, &sum1 );
        BCATU(bmath_vcfx_s,sum)( v2, &sum2 );
        ASSERT( BCATU(bmath,cfx,equ)( sum1, BCATU(bmath,cfx,neg)( sum2 ) ) );

        BCATU(bmath_vcfx_s,mul_fx)( v1, 2.0, v2 );
        BCATU(bmath_vcfx_s,sum)( v1, &sum1 );
        BCATU(bmath_vcfx_s,sum)( v2, &sum2 );
        ASSERT( BCATU(bmath,cfx,equ)( sum2, BCATU(bmath,cfx,mul,fx)( sum1, 2.0 ) ) );
        BCATU(bmath_vcfx_s,sqr)( v1, &sqr1 );
        BCATU(bmath_vcfx_s,mul_vec)( v1, v2, &sqr2 );
        ASSERT( BCATU(bmath,cfx,equ)( sqr2, BCATU(bmath,cfx,mul,fx)( sqr1, 2.0 ) ) );
        bmath_vector_a_mul_vec( ( const bmath_vector* )v1, ( const bmath_vector* )v2, ( bmath_ring* )&sqr2 );
        ASSERT( BCATU(bmath,cfx,equ)( sqr2, BCATU(bmath,cfx,mul,fx)( sqr1, 2.0 ) ) );

        BCATU(bmath_vcfx_s,dft)( v1, v2 );
        BCATU(bmath_vcfx_s,ift)( v2, v2 );
        BCATU(bmath_vcfx_s,sub_sqr)( v1, v2, &sqr1 );
        ASSERT( BCATU(bmath,cfx,mag)( sqr1 ) < ( ( sizeof( fx_t ) == 4 ) ? 1E-3 : 1E-14 ) );
    }

    bcore_life_s_discard( l );

    return NULL;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#include "bmath_template_fx_end.h"

/**********************************************************************************************************************/

