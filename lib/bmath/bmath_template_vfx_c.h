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

/** Template: Vector */

/**********************************************************************************************************************/

#include "bmath_template_fx_begin.h"

/**********************************************************************************************************************/
// low level vector functions (partially AVX optimized)

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath,fx,t_vec,sum)( const fx_t* v1, sz_t size )
{
#ifdef BMATH_AVX
    M5_T sum_p = M5_SET_ZERO();
    sz_t i = 0;
    for( ; i <= size - P5_SIZE; i += P5_SIZE )
    {
        sum_p = M5_ADD( M5_LOAD( v1 + i ), sum_p );
    }
#else
    fx_t sum_p[ P5_SIZE ] = P5_ZERO;
    sz_t i;
    for( i = 0; i <= size - P5_SIZE; i += P5_SIZE )
    {
        for( sz_t j = 0; j < P5_SIZE; j++ ) sum_p[ j ] += v1[ i + j ];
    }
#endif // BMATH_AVX
    for( ; i < size; i++ ) sum_p[ 0 ] += v1[ i ];
    return P5_HSUM( sum_p );
}

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath,fx,t_vec,sum_esp)( const fx_t* v1, sz_t size )
{
    if( size <= 64 ) return BCATU(bmath,fx,t_vec,sum)( v1, size );
    sz_t mid = size >> 1;
    return BCATU(bmath,fx,t_vec,sum_esp)( v1, mid ) + BCATU(bmath,fx,t_vec,sum_esp)( v1 + mid, size - mid );
}

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath,fx,t_vec,mul_vec)( const fx_t* v1, const fx_t* v2, sz_t size )
{
#ifdef BMATH_AVX
    M5_T sum_p = M5_SET_ZERO();
    sz_t i = 0;
    for( ; i <= size - P5_SIZE; i += P5_SIZE )
    {
        sum_p = M5_MUL_ADD( M5_LOAD( v1 + i ), M5_LOAD( v2 + i ), sum_p );
    }
#else
    fx_t sum_p[ P5_SIZE ] = P5_ZERO;
    sz_t i;
    for( i = 0; i <= size - P5_SIZE; i += P5_SIZE )
    {
        for( sz_t j = 0; j < P5_SIZE; j++ ) sum_p[ j ] += v1[ i + j ] * v2[ i + j ];
    }
#endif // BMATH_AVX
    for( ; i < size; i++ ) sum_p[ 0 ] += v1[ i ] * v2[ i ];
    return P5_HSUM( sum_p );
}

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath,fx,t_vec,mul_vec_stride)( const fx_t* v1, sz_t stride1, const fx_t* v2, sz_t stride2, sz_t size )
{
    fx_t sum_p4[ 4 ] = { 0, 0, 0, 0 };
    sz_t i;
    for( i = 0; i <= size - 4; i += 4 )
    {
        sum_p4[ 0 ] += v1[ ( i + 0 ) * stride1 ] * v2[ ( i + 0 ) * stride2 ];
        sum_p4[ 1 ] += v1[ ( i + 1 ) * stride1 ] * v2[ ( i + 1 ) * stride2 ];
        sum_p4[ 2 ] += v1[ ( i + 2 ) * stride1 ] * v2[ ( i + 2 ) * stride2 ];
        sum_p4[ 3 ] += v1[ ( i + 3 ) * stride1 ] * v2[ ( i + 3 ) * stride2 ];
    }
    for( ; i < size; i++ ) sum_p4[ 0 ] += v1[ i * stride1 ] * v2[ i * stride2 ];
    return sum_p4[ 0 ] + sum_p4[ 1 ] + sum_p4[ 2 ] + sum_p4[ 3 ];
}

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath,fx,t_vec,mul_vec_esp)( const fx_t* v1, const fx_t* v2, sz_t size )
{
    if( size <= 64 ) return BCATU(bmath,fx,t_vec,mul_vec)( v1, v2, size );
    sz_t mid = size >> 1;
    return BCATU(bmath,fx,t_vec,mul_vec_esp)( v1, v2, mid ) + BCATU(bmath,fx,t_vec,mul_vec_esp)( v1 + mid, v2 + mid, size - mid );
}

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath,fx,t_vec,mul_vec_esp_stride)( const fx_t* v1, sz_t stride1, const fx_t* v2, sz_t stride2, sz_t size )
{
    if( size <= 64 ) return BCATU(bmath,fx,t_vec,mul_vec_stride)( v1, stride1, v2, stride2, size );
    sz_t mid = size >> 1;
    return BCATU(bmath,fx,t_vec,mul_vec_esp_stride)( v1, stride1, v2, stride2, mid ) + BCATU(bmath,fx,t_vec,mul_vec_esp_stride)( v1 + mid * stride1, stride1, v2 + mid * stride2, stride2, size - mid );
}

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath,fx,t_vec,mul3_vec)( const fx_t* v1, const fx_t* v2, const fx_t* v3, sz_t size )
{
#ifdef BMATH_AVX
    M5_T sum_p = M5_SET_ZERO();
    sz_t i = 0;
    for( ; i <= size - P5_SIZE; i += P5_SIZE )
    {
        sum_p = M5_MUL_ADD( M5_MUL( M5_LOAD( v1 + i ), M5_LOAD( v2 + i ) ), M5_LOAD( v3 + i ), sum_p );
    }
#else
    fx_t sum_p[ P5_SIZE ] = P5_ZERO;
    sz_t i;
    for( i = 0; i <= size - P5_SIZE; i += P5_SIZE )
    {
        for( sz_t j = 0; j < P5_SIZE; j++ ) sum_p[ j ] += v1[ i + j ] * v2[ i + j ] * v3[ i + j ];
    }
#endif // BMATH_AVX
    for( ; i < size; i++ ) sum_p[ 0 ] += v1[ i ] * v2[ i ] * v3[ i ];
    return P5_HSUM( sum_p );
}

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath,fx,t_vec,mul3_vec_esp)( const fx_t* v1, const fx_t* v2, const fx_t* v3, sz_t size )
{
    if( size <= 64 ) return BCATU(bmath,fx,t_vec,mul3_vec)( v1, v2, v3, size );
    sz_t mid = size >> 1;
    return BCATU(bmath,fx,t_vec,mul3_vec_esp)( v1, v2, v3, mid ) + BCATU(bmath,fx,t_vec,mul3_vec_esp)( v1 + mid, v2 + mid, v3 + mid, size - mid );
}

//----------------------------------------------------------------------------------------------------------------------

// a * b -> r
void BCATU(bmath,fx,t_vec,mul_scl)( const fx_t* a, fx_t b, fx_t* r, sz_t size )
{
    sz_t i = 0;
#ifdef BMATH_AVX
    M5_T b_p = M5_SET_ALL( b );
    for( ; i <= size - P5_SIZE; i += P5_SIZE )
    {
        M5_STOR( r + i, M5_MUL( M5_LOAD( a + i ), b_p ) );
    }
#endif // BMATH_AVX
    for( ; i < size; i++ ) r[ i ] = a[ i ] * b;
}

//----------------------------------------------------------------------------------------------------------------------

// a * b + c -> r
void BCATU(bmath,fx,t_vec,mul_scl_add)( const fx_t* a, fx_t b, const fx_t* c, fx_t* r, sz_t size )
{
    sz_t i = 0;
#ifdef BMATH_AVX
    M5_T b_p = M5_SET_ALL( b );
    for( ; i <= size - P5_SIZE; i += P5_SIZE )
    {
        M5_STOR( r + i, M5_MUL_ADD( M5_LOAD( a + i ), b_p , M5_LOAD( c + i ) ) );
    }
#endif // BMATH_AVX
    for( ; i < size; i++ ) r[ i ] = a[ i ] * b + c[ i ];
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// bmath_vfx_s

void BCATU(bmath_vfx_s,move)( bmath_vfx_s* o, bmath_vfx_s* src )
{
    if( o == src ) return;
    *o = *src;
    src->space = 0;
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,clear)( bmath_vfx_s* o )
{
    bcore_array_a_set_space( (bcore_array*)o, 0 );
}

//----------------------------------------------------------------------------------------------------------------------

bmath_vfx_s* BCATU(bmath_vfx_s,set_size)( bmath_vfx_s* o, uz_t size )
{
    bcore_array_a_set_size( (bcore_array*)o, size );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

bmath_vfx_s* BCATU(bmath_vfx_s,set_space)( bmath_vfx_s* o, uz_t space )
{
    bcore_array_a_set_space( (bcore_array*)o, space );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,set_size_to)( const bmath_vfx_s* o, bmath_vfx_s* res )
{
    BCATU(bmath_vfx_s,set_size)( res, o->size );
}

//----------------------------------------------------------------------------------------------------------------------

bmath_vfx_s* BCATU(bmath_vfx_s,fill)( bmath_vfx_s* o, fx_t val )
{
    for( uz_t i = 0; i < o->size; i++ ) o->data[ i ] = val;
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,set_data)( bmath_vfx_s* o, const fx_t* data, sz_t size )
{
    BCATU(bmath_vfx_s,set_size)( o, size );
    bcore_u_memcpy( sizeof( fx_t ), o->data, data, size );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,set_random)( bmath_vfx_s* o, fx_t density, fx_t min, fx_t max, bcore_prsg* prsg )
{
    BLM_INIT();
    if( !prsg ) prsg = ( bcore_prsg* )BLM_CREATE( bcore_prsg_lcg_u3_00_s );
    for( uz_t i = 0; i < o->size; i++ )
    {
        if( bcore_prsg_a_gen_f3( prsg, 0, 1 ) < density )
        {
            o->data[ i ] = bcore_prsg_a_gen_f3( prsg, min, max );
        }
        else
        {
            o->data[ i ] = 0;
        }
    }
    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,set_random_u3)( bmath_vfx_s* o, fx_t density, fx_t min, fx_t max, u3_t* p_rval )
{
    BLM_INIT();
    bcore_prsg* prsg = ( bcore_prsg* )BLM_CREATE( bcore_prsg_lcg_u3_00_s );
    if( p_rval ) bcore_prsg_a_reseed( prsg, *p_rval );
    BCATU(bmath_vfx_s,set_random)( o, density, min, max, prsg );
    if( p_rval ) *p_rval = bcore_prsg_a_gen_u3( prsg );
    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,push)( bmath_vfx_s* o, fx_t val )
{
    if( o->size < o->space )
    {
        o->data[ o->size++ ] = val;
    }
    else
    {
        bcore_array_a_push_f3( (bcore_array*)o, val );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,push_vfx)( bmath_vfx_s* o, const bmath_vfx_s* vec )
{
    for( uz_t i = 0; i < vec->size; i++ ) BCATU(bmath_vfx_s,push)( o, vec->data[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath_vfx_s,is_near_equ)( const bmath_vfx_s* o, const bmath_vfx_s* op, fx_t max_dev )
{
    if( o->size != op->size ) return false;
    for( uz_t i = 0   ; i < o->size;  i++ ) if( BCATU(fx,abs)( o->data[ i ] - op->data[ i ] ) > max_dev ) return false;
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath_vfx_s,is_near_zro)( const bmath_vfx_s* o, fx_t max_dev )
{
    for( uz_t i = 0; i < o->size; i++ ) if( BCATU(fx,abs)( o->data[ i ] )  > max_dev ) return false;
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

f3_t BCATU(bmath_vfx_s,fdev)( const bmath_vfx_s* o, const bmath_vfx_s* op )
{
    ASSERT( o->size == op->size );
    f3_t sum = 0;
    for( sz_t i = 0 ; i < o->size;  i++ ) sum += f3_sqr( o->data[ i ] - op->data[ i ] );
    return f3_srt( sum );
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath_vfx_s,is_nan)( const bmath_vfx_s* o )
{
    for( uz_t i = 0; i < o->size; i++ ) if( f3_is_nan( o->data[ i ] ) ) return true;
    return false;
}

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath_vfx_s,fx_tss)( const bmath_vfx_s* o )
{
    fx_t sum = 0;
    for( uz_t i = 0; i < o->size; i++ ) sum += BCATU(fx,sqr)( o->data[ i ] );
    return sum;
}

//----------------------------------------------------------------------------------------------------------------------

f3_t BCATU(bmath_vfx_s,tss)( const bmath_vfx_s* o )
{
    f3_t sum = 0;
    for( uz_t i = 0; i < o->size; i++ ) sum += f3_sqr( o->data[ i ] );
    return sum;
}

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath_vfx_s,fdev_equ)( const bmath_vfx_s* a, const bmath_vfx_s* b )
{
    ASSERT( a->size == b->size );
    fx_t sum = 0;
    const fx_t* ad = a->data;
    const fx_t* bd = b->data;
    BFOR_EACH( i, a ) sum += BCATU(fx,sqr)( ad[ i ] - bd[ i ] );
    return ( sum > 0 ) ? BCATU(fx,srt)( sum ) : 0;
}

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath_vfx_s,fdev_zro)( const bmath_vfx_s* o )
{
    fx_t sum = 0;
    const fx_t* od = o->data;
    BFOR_EACH( i, o ) sum += BCATU(fx,sqr)( od[ i ] );
    return ( sum > 0 ) ? BCATU(fx,srt)( sum ) : 0;
}

//----------------------------------------------------------------------------------------------------------------------

bmath_vfx_s* BCATU(bmath_vfx_s,create_set_size)( uz_t size )
{
    bmath_vfx_s* o = BCATU(bmath_vfx_s,create)();
    BCATU(bmath_vfx_s,set_size)( o, size );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

bmath_vfx_s* BCATU(bmath_vfx_s,create_fill)( uz_t size, fx_t val )
{
    bmath_vfx_s* o = BCATU(bmath_vfx_s,create_set_size)( size );
    BCATU(bmath_vfx_s,fill)( o, val );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,zro)( bmath_vfx_s* o )
{
    bcore_u_memzero( sizeof( fx_t ), o->data, o->size );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,neg)( const bmath_vfx_s* o, bmath_vfx_s* res )
{
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ ) res->data[ i ] = -o->data[ i ];
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,cpy)( const bmath_vfx_s* o, bmath_vfx_s* res )
{
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ ) res->data[ i ] = o->data[ i ];
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,cat)( const bmath_vfx_s* a, const bmath_vfx_s* b, bmath_vfx_s* r )
{
    ASSERT( r->size == a->size + b->size );
    for( uz_t i = 0; i < a->size; i++ ) r->data[           i ] = a->data[ i ];
    for( uz_t i = 0; i < b->size; i++ ) r->data[ a->size + i ] = b->data[ i ];
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,add)( const bmath_vfx_s* o, const bmath_vfx_s* op, bmath_vfx_s* res )
{
    ASSERT( o->size == op->size );
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ ) res->data[ i ] = o->data[ i ] + op->data[ i ];
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,sub)( const bmath_vfx_s* o, const bmath_vfx_s* op, bmath_vfx_s* res )
{
    ASSERT( o->size == op->size );
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ ) res->data[ i ] = o->data[ i ] - op->data[ i ];
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,add_scl)( const bmath_vfx_s* o, const fx_t* op, bmath_vfx_s* res )
{
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ ) res->data[ i ] = o->data[ i ] + *op;
}


//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,sub_scl)( const bmath_vfx_s* o, const fx_t* op, bmath_vfx_s* res )
{
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ ) res->data[ i ] = o->data[ i ] - *op;
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,add_scl_fx)( const bmath_vfx_s* o, fx_t op, bmath_vfx_s* res )
{
    BCATU(bmath_vfx_s,add_scl)( o, &op, res );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,sub_scl_fx)( const bmath_vfx_s* o, fx_t op, bmath_vfx_s* res )
{
    BCATU(bmath_vfx_s,sub_scl)( o, &op, res );
}

//----------------------------------------------------------------------------------------------------------------------

/// hadamard product: a o b [+ c] -> r  (elementwise)
void BCATU(bmath_vfx_s,mul_hdm)( const bmath_vfx_s* a, const bmath_vfx_s* b, bmath_vfx_s* r )
{
    ASSERT( a->size == b->size );
    ASSERT( a->size == r->size );
    for( uz_t i = 0; i < a->size; i++ ) r->data[ i ] = a->data[ i ] * b->data[ i ];
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,mul_hdm_add)( const bmath_vfx_s* a, const bmath_vfx_s* b, const bmath_vfx_s* c, bmath_vfx_s* r )
{
    ASSERT( a->size == b->size );
    ASSERT( a->size == c->size );
    ASSERT( a->size == r->size );
    for( uz_t i = 0; i < a->size; i++ ) r->data[ i ] = a->data[ i ] * b->data[ i ] + c->data[ i ];
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,pmt_mul)( const bmath_vfx_s* o, const bmath_pmt_s* p, bmath_vfx_s* res )
{
    if( o == res )
    {
        bmath_vfx_s* buf = BCATU(bmath_vfx_s,create)();
        BCATU(bmath_vfx_s,set_size)( buf, o->size );
        BCATU(bmath_vfx_s,pmt_mul)( o, p, buf );
        BCATU(bmath_vfx_s,cpy)( buf, res );
        BCATU(bmath_vfx_s,discard)( buf );
        return;
    }

    ASSERT( o->size == p->size );
    ASSERT( o->size == res->size );

    for( uz_t i = 0; i < p->size; i++ )
    {
        assert( p->data[ i ] < o->size );
        res->data[ p->data[ i ] ] = o->data[ i ];
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,pmt_htp_mul)( const bmath_vfx_s* o, const bmath_pmt_s* p, bmath_vfx_s* res )
{
    if( o == res )
    {
        bmath_vfx_s* buf = BCATU(bmath_vfx_s,create)();
        BCATU(bmath_vfx_s,set_size)( buf, o->size );
        BCATU(bmath_vfx_s,pmt_htp_mul)( o, p, buf );
        BCATU(bmath_vfx_s,cpy)( buf, res );
        BCATU(bmath_vfx_s,discard)( buf );
        return;
    }

    ASSERT( o->size == p->size );
    ASSERT( o->size == res->size );

    for( uz_t i = 0; i < p->size; i++ )
    {
        assert( p->data[ i ] < o->size );
        res->data[ i ] = o->data[ p->data[ i ] ];
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,mul_scl)( const bmath_vfx_s* o, const fx_t* op, bmath_vfx_s* res )
{
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ ) res->data[ i ] = o->data[ i ] * *op;
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,mul_scl_fx)(  const bmath_vfx_s* o, fx_t s, bmath_vfx_s* r )
{
    ASSERT( o->size == r->size );
    for( uz_t i = 0; i < o->size; i++ ) r->data[ i ] = o->data[ i ] * s;
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,mul_scl_fx_add)( const bmath_vfx_s* o, fx_t s, const bmath_vfx_s* a, bmath_vfx_s* r )
{
    ASSERT( o->size == r->size );
    ASSERT( o->size == a->size );
    for( uz_t i = 0; i < o->size; i++ ) r->data[ i ] = o->data[ i ] * s + a->data[ i ];
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,mul_fx)( const bmath_vfx_s* o, fx_t op, bmath_vfx_s* res )
{
    BCATU(bmath_vfx_s,mul_scl_fx)( o, op, res );
}

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath_vfx_s,fx_mul_vec)( const bmath_vfx_s* o, const bmath_vfx_s* vec2 )
{
    ASSERT( o->size == vec2->size );
    return BCATU(bmath,fx,t_vec,mul_vec)( o->data, vec2->data, o->size );
}

//----------------------------------------------------------------------------------------------------------------------

static fx_t fx_s_sqr( const fx_t* v1, uz_t size )
{
    switch( size )
    {
        case 0: return 0;
        case 1: return v1[0]*v1[0];
        case 2: return v1[0]*v1[0]+v1[1]*v1[1];
        case 3: return v1[0]*v1[0]+v1[1]*v1[1]+v1[2]*v1[2];
        case 4: return v1[0]*v1[0]+v1[1]*v1[1]+v1[2]*v1[2]+v1[3]*v1[3];
        default: break;
    }
    uz_t sz1 = size >> 1;
    return fx_s_sqr( v1, sz1 ) + fx_s_sqr( v1 + sz1, size - sz1 );
}

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath_vfx_s,fx_sqr)( const bmath_vfx_s* o )
{
    return fx_s_sqr( o->data, o->size );
}

//----------------------------------------------------------------------------------------------------------------------

static fx_t fx_s_sqr_sub( const fx_t* v1, const fx_t* v2, uz_t size )
{
    switch( size )
    {
        case 0: return 0;
        case 1: return f3_sqr(v1[0]-v2[0]);
        case 2: return f3_sqr(v1[0]-v2[0])+f3_sqr(v1[1]-v2[1]);
        case 3: return f3_sqr(v1[0]-v2[0])+f3_sqr(v1[1]-v2[1])+f3_sqr(v1[2]-v2[2]);
        case 4: return f3_sqr(v1[0]-v2[0])+f3_sqr(v1[1]-v2[1])+f3_sqr(v1[2]-v2[2])+f3_sqr(v1[3]-v2[3]);
        default: break;
    }
    uz_t sz1 = size >> 1;
    return fx_s_sqr_sub( v1, v2, sz1 ) + fx_s_sqr_sub( v1 + sz1, v2 + sz1, size - sz1 );
}

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath_vfx_s,fx_sub_sqr)( const bmath_vfx_s* o, const bmath_vfx_s* vec2 )
{
    ASSERT( o->size == vec2->size );
    return fx_s_sqr_sub( o->data, vec2->data, o->size );
}

//----------------------------------------------------------------------------------------------------------------------

static fx_t fx_max( fx_t a, fx_t b ) { return ( a > b ) ?  a : b; }
static fx_t fx_min( fx_t a, fx_t b ) { return ( a < b ) ?  a : b; }
static fx_t fx_abs( fx_t a         ) { return ( a < 0 ) ? -a : a; }

static fx_t fx_s_fx_max( const fx_t* v1, uz_t size )
{
    fx_t max = ( size > 0 ) ? v1[ 0 ] : 0;
    for( uz_t i = 1; i < size; i++ ) max = fx_max( max, v1[ i ] );
    return max;
}

//----------------------------------------------------------------------------------------------------------------------

static fx_t fx_s_fx_max_abs( const fx_t* v1, uz_t size )
{
    fx_t max = ( size > 0 ) ? fx_abs( v1[ 0 ] ) : 0;
    for( uz_t i = 1; i < size; i++ ) max = fx_max( max, fx_abs( v1[ i ] ) );
    return max;
}

//----------------------------------------------------------------------------------------------------------------------

static fx_t fx_s_fx_min( const fx_t* v1, uz_t size )
{
    fx_t min = ( size > 0 ) ? v1[ 0 ] : 0;
    for( uz_t i = 1; i < size; i++ ) min = fx_min( min, v1[ i ] );
    return min;
}

//----------------------------------------------------------------------------------------------------------------------

static fx_t fx_s_fx_min_abs( const fx_t* v1, uz_t size )
{
    fx_t min = ( size > 0 ) ? fx_abs( v1[ 0 ] ) : 0;
    for( uz_t i = 1; i < size; i++ ) min = fx_min( min, fx_abs( v1[ i ] ) );
    return min;
}

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath_vfx_s,fx_sum)( const bmath_vfx_s* o )
{
    return BCATU(bmath,fx,t_vec,sum)( o->data, o->size );
}

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath_vfx_s,fx_avg)( const bmath_vfx_s* o )
{
    return o->size > 0 ? BCATU(bmath_vfx_s,fx_sum)( o ) / o->size : 0;
}

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath_vfx_s,fx_var)( const bmath_vfx_s* o )
{
    fx_t e_v  = BCATU(bmath_vfx_s,fx_sum)( o ) / ( ( o->size > 1 ) ? o->size : 1 );
    fx_t e_vv = BCATU(bmath_vfx_s,fx_sqr)( o ) / ( ( o->size > 1 ) ? o->size : 1 );
    return f3_max( 0.0, e_vv - f3_sqr( e_v ) );
}

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath_vfx_s,fx_dev)( const bmath_vfx_s* o )
{
    return f3_srt( BCATU(bmath_vfx_s,fx_var)( o ) );
}

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath_vfx_s,fx_max)( const bmath_vfx_s* o )
{
    return fx_s_fx_max( o->data, o->size );
}

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath_vfx_s,fx_max_abs)( const bmath_vfx_s* o )
{
    return fx_s_fx_max_abs( o->data, o->size );
}

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath_vfx_s,fx_min)( const bmath_vfx_s* o )
{
    return fx_s_fx_min( o->data, o->size );
}

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath_vfx_s,fx_min_abs)( const bmath_vfx_s* o )
{
    return fx_s_fx_min_abs( o->data, o->size );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,mul_vec)( const bmath_vfx_s* o, const bmath_vfx_s* op, fx_t* res )
{
    *res = BCATU(bmath_vfx_s,fx_mul_vec)( o, op );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,sqr)( const bmath_vfx_s* o, fx_t* res )
{
    *res = BCATU(bmath_vfx_s,fx_sqr)( o );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,sub_sqr)( const bmath_vfx_s* o, const bmath_vfx_s* op, fx_t* res )
{
    *res = BCATU(bmath_vfx_s,fx_sub_sqr)( o, op );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,sum)( const bmath_vfx_s* o, fx_t* res )
{
    *res = BCATU(bmath_vfx_s,fx_sum)( o );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,avg)( const bmath_vfx_s* o, fx_t* res )
{
    *res = BCATU(bmath_vfx_s,fx_avg)( o );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,var)( const bmath_vfx_s* o, fx_t* res )
{
    *res = BCATU(bmath_vfx_s,fx_var)( o );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,dev)( const bmath_vfx_s* o, fx_t* res )
{
    *res = BCATU(bmath_vfx_s,fx_dev)( o );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,set_sqr)( bmath_vfx_s* o, fx_t val )
{
    ASSERT( val >= 0 );
    fx_t cur = BCATU(bmath_vfx_s,fx_sqr)( o );
    fx_t fac = ( cur > 0 ) ? sqrt( val / cur ) : 0;
    BCATU(bmath_vfx_s,mul_fx)( o, fac, o );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,set_sum)( bmath_vfx_s* o, fx_t val )
{
    fx_t cur = BCATU(bmath_vfx_s,fx_sum)( o );
    fx_t add = o->size > 0 ? ( val - cur ) / o->size : 0;
    for( uz_t i = 0; i < o->size; i++ ) o->data[ i ] += add;
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,set_avg)( bmath_vfx_s* o, fx_t val )
{
    fx_t cur = BCATU(bmath_vfx_s,fx_avg)( o );
    fx_t add = val - cur;
    for( uz_t i = 0; i < o->size; i++ ) o->data[ i ] += add;
}

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath_vfx_s,max)( const bmath_vfx_s* o )
{
    fx_t v = o->size > 0 ? o->data[ 0 ] : 0;
    for( uz_t i = 1; i < o->size; i++ ) v = fx_max( v, o->data[ i ] );
    return v;
}

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath_vfx_s,min)( const bmath_vfx_s* o )
{
    fx_t v = o->size > 0 ? o->data[ 0 ] : 0;
    for( uz_t i = 1; i < o->size; i++ ) v = fx_min( v, o->data[ i ] );
    return v;
}

//----------------------------------------------------------------------------------------------------------------------

uz_t BCATU(bmath_vfx_s,idx_max)( const bmath_vfx_s* o )
{
    fx_t v = o->size > 0 ? o->data[ 0 ] : 0;
    uz_t i = 0;
    for( uz_t j = 1; j < o->size; j++ )
    {
        i = o->data[ j ] > v ? j : i;
        v = o->data[ j ] > v ? o->data[ j ] : v;
    }
    return i;
}

//----------------------------------------------------------------------------------------------------------------------

uz_t BCATU(bmath_vfx_s,idx_min)( const bmath_vfx_s* o )
{
    fx_t v = o->size > 0 ? o->data[ 0 ] : 0;
    uz_t i = 0;
    for( uz_t j = 1; j < o->size; j++ )
    {
        i = o->data[ j ] < v ? j : i;
        v = o->data[ j ] < v ? o->data[ j ] : v;
    }
    return i;
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,eop_map)( const bmath_vfx_s* o, BCATU(bmath_fp,fx,ar1) b, bmath_vfx_s* r )
{
    ASSERT( o->size == r->size );
    for( sz_t j = 0; j < o->size; j++ ) r->data[ j ] = b( o->data[ j ] );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,eop_map_mul)( const bmath_vfx_s* o, BCATU(bmath_fp,fx,ar1) b, const bmath_vfx_s* c, bmath_vfx_s* r )
{
    ASSERT( o->size == r->size );
    ASSERT( o->size == c->size );
    for( sz_t j = 0; j < o->size; j++ ) r->data[ j ] = b( o->data[ j ] ) * c->data[ j ];
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// type conversion

//----------------------------------------------------------------------------------------------------------------------

er_t BCATU(bmath_vfx_s,copy_typed)( bmath_vfx_s* o, tp_t type, vc_t src )
{
    if( !src )
    {
        BCATU(bmath_vfx_s,clear)( o );
        return 0;
    }

    switch( type )
    {
        case TYPEOF_bmath_vfx_s:
        {
            BCATU(bmath_vfx_s,copy)( o, src );
        }
        break;

        case TYPEOF_bmath_vfy_s:
        {
            const bmath_vfy_s* v = src;
            BCATU(bmath_vfx_s,set_size)( o, v->size );
            BFOR_EACH( i, o ) o->data[ i ] = v->data[ i ];
        }
        break;

        default:
        {
            return bcore_error_push_fa( TYPEOF_conversion_error, "copy_typed to '#<sc_t>': no conversion from '#<sc_t>'.", ifnameof( o->_ ), ifnameof( type ) );
        }
        break;
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,to_sink)( const bmath_vfx_s* o, bcore_sink* sink )
{
    const fx_t* v = o->data;
    bcore_sink_a_push_fa( sink, "(#<uz_t>)\n", o->size );
    for( uz_t i = 0; i < o->size; i++ ) bcore_sink_a_push_fa( sink, "#<f3_t>\n", v[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_vfx_s,stat_to_sink)( const bmath_vfx_s* o, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "(#<uz_t>)\n", o->size );
    bcore_sink_a_push_fa( sink, "  sum .. #<f3_t>\n", BCATU(bmath_vfx_s,fx_sum)( o ) );
    bcore_sink_a_push_fa( sink, "  avg .. #<f3_t>\n", BCATU(bmath_vfx_s,fx_avg)( o ) );
    bcore_sink_a_push_fa( sink, "  max .. #<f3_t>\n", BCATU(bmath_vfx_s,fx_max)( o ) );
    bcore_sink_a_push_fa( sink, "  min .. #<f3_t>\n", BCATU(bmath_vfx_s,fx_min)( o ) );
    bcore_sink_a_push_fa( sink, "  dev .. #<f3_t>\n", BCATU(bmath_vfx_s,fx_dev)( o ) );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#include "bmath_spect_vector.h"

static void selftest( void )
{
    BLM_INIT();

    {
        uz_t size = 100;
        bmath_vfx_s* v1 = BLM_A_PUSH( BCATU(bmath_vfx_s,create_set_size)( size ) );
        for( uz_t i = 0; i < size; i++ ) v1->data[ i ] = i + 1;
        f3_t sqr_sum = 0;
        for( uz_t i = 0; i < size; i++ ) sqr_sum += f3_sqr( v1->data[ i ] );
        ASSERT( BCATU(bmath_vfx_s,fx_sum)( v1 ) == ( size * ( size + 1 ) ) / 2 );
        ASSERT( BCATU(bmath_vfx_s,fx_avg)( v1 ) == ( fx_t )( size * ( size + 1 ) ) / ( 2 * size ) );
        ASSERT( BCATU(bmath_vfx_s,fx_sqr)( v1 ) == sqr_sum );
        ASSERT( BCATU(bmath_vfx_s,fx_sqr)( v1 ) == BCATU(bmath_vfx_s,fx_mul_vec)( v1, v1 ) );

        bmath_vfx_s* v2 = BLM_A_PUSH( BCATU(bmath_vfx_s,create_set_size)( size ) );
        BCATU(bmath_vfx_s,cpy)( v1, v2 );
        ASSERT( BCATU(bmath_vfx_s,fx_sum)( v2 ) ==  BCATU(bmath_vfx_s,fx_sum)( v1 ) );
        BCATU(bmath_vfx_s,neg)( v1, v2 );
        ASSERT( BCATU(bmath_vfx_s,fx_sum)( v2 ) == -BCATU(bmath_vfx_s,fx_sum)( v1 ) );
        BCATU(bmath_vfx_s,mul_fx)( v1, 2.0, v2 );
        ASSERT( BCATU(bmath_vfx_s,fx_sum)(     v2     ) == 2.0 * BCATU(bmath_vfx_s,fx_sum)( v1 ) );
        ASSERT( BCATU(bmath_vfx_s,fx_mul_vec)( v1, v2 ) == 2.0 * BCATU(bmath_vfx_s,fx_sqr)( v1 ) );

        fx_t v = 0;
        bmath_vector_a_mul_vec( ( const bmath_vector* )v1, ( const bmath_vector* )v2, ( bmath_ring* )&v );
        ASSERT( BCATU(bmath_vfx_s,fx_mul_vec)( v1, v2 ) == v );
    }

    BLM_DOWN();
}

/**********************************************************************************************************************/

#include "bmath_template_fx_end.h"

/**********************************************************************************************************************/

