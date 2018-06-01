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

#include <stdio.h>

#include "bmath_vector.h"
#include "bmath_spect_vector.h"
#include "bmath_fourier.h"

/**********************************************************************************************************************/
// bmath_vf3_s

BCORE_DEFINE_OBJECT_INST( bmath_vector, bmath_vf3_s )
"{"
    "aware_t _;"
    "f3_t [] arr;"
    "func bmath_fp:add;"
    "func bmath_fp:zro;"
    "func bmath_fp:neg;"
    "func bmath_fp:sub;"
    "func bmath_fp:cpy;"
    "func bmath_fp_vector:mul_scl = bmath_vf3_s_mul_scl;"
    "func bmath_fp_vector:mul_vec = bmath_vf3_s_mul_vec;"
"}";

//---------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_move( bmath_vf3_s* o, bmath_vf3_s* src )
{
    if( o == src ) return;
    *o = *src;
    src->space = 0;
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_clear( bmath_vf3_s* o )
{
    bcore_array_a_set_space( (bcore_array*)o, 0 );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_set_size( bmath_vf3_s* o, sz_t size )
{
    bcore_array_a_set_size( (bcore_array*)o, size );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_set_size_to( const bmath_vf3_s* o, bmath_vf3_s* res )
{
    bmath_vf3_s_set_size( res, o->size );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_fill( bmath_vf3_s* o, f3_t val )
{
    for( sz_t i = 0; i < o->size; i++ ) o->data[ i ] = val;
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_fill_random( bmath_vf3_s* o, f3_t min, f3_t max, u2_t* rval )
{
    f3_t range = max - min;
    for( sz_t i = 0; i < o->size; i++ ) o->data[ i ] = ( range * f3_xsg1_pos( rval ) ) + min;
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_push( bmath_vf3_s* o, f3_t val )
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

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_vf3_s_is_equ( const bmath_vf3_s* o, const bmath_vf3_s* op )
{
    sz_t size = sz_min( o->size, op->size );
    for( sz_t i = 0   ; i < size   ;  i++ ) if(  o->data[ i ] != op->data[ i ] ) return false;
    for( sz_t i = size; i < o->size;  i++ ) if(  o->data[ i ] != 0             ) return false;
    for( sz_t i = size; i < op->size; i++ ) if( op->data[ i ] != 0             ) return false;
    return true;
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_vf3_s_is_zro( const bmath_vf3_s* o )
{
    for( sz_t i = 0; i < o->size; i++ ) if( o->data[ i ] != 0 ) return false;
    return true;
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_vf3_s_is_near_equ( const bmath_vf3_s* o, const bmath_vf3_s* op, f3_t max_dev )
{
    sz_t size = sz_min( o->size, op->size );
    for( sz_t i = 0   ; i < size   ;  i++ ) if( f3_abs(  o->data[ i ]- op->data[ i ] ) > max_dev ) return false;
    for( sz_t i = size; i < o->size;  i++ ) if( f3_abs(  o->data[ i ] ) > max_dev                ) return false;
    for( sz_t i = size; i < op->size; i++ ) if( f3_abs( op->data[ i ] ) > max_dev                ) return false;
    return true;
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_vf3_s_is_near_zro( const bmath_vf3_s* o, f3_t max_dev )
{
    for( sz_t i = 0; i < o->size; i++ ) if( f3_abs( o->data[ i ] )  > max_dev ) return false;
    return true;
}

//---------------------------------------------------------------------------------------------------------------------

bmath_vf3_s* bmath_vf3_s_create_set_size( sz_t size )
{
    bmath_vf3_s* o = bmath_vf3_s_create();
    bmath_vf3_s_set_size( o, size );
    return o;
}

//---------------------------------------------------------------------------------------------------------------------

bmath_vf3_s* bmath_vf3_s_create_fill( sz_t size, f3_t val )
{
    bmath_vf3_s* o = bmath_vf3_s_create_set_size( size );
    bmath_vf3_s_fill( o, val );
    return o;
}

//---------------------------------------------------------------------------------------------------------------------

bmath_vf3_s* bmath_vf3_s_create_fill_random( sz_t size, f3_t min, f3_t max, u2_t* rval )
{
    bmath_vf3_s* o = bmath_vf3_s_create_set_size( size );
    bmath_vf3_s_fill_random( o, min, max, rval );
    return o;
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_zro( bmath_vf3_s* o )
{
    bcore_u_memzero( sizeof( f3_t ), o->data, o->size );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_neg( const bmath_vf3_s* o, bmath_vf3_s* res )
{
    sz_t size = sz_min( o->size, res->size );
    for( sz_t i = 0   ; i < size     ; i++ ) res->data[ i ] = -o->data[ i ];
    for( sz_t i = size; i < res->size; i++ ) res->data[ i ] = 0;
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_cpy( const bmath_vf3_s* o, bmath_vf3_s* res )
{
    sz_t size = sz_min( o->size, res->size );
    for( sz_t i = 0   ; i < size     ; i++ ) res->data[ i ] = o->data[ i ];
    for( sz_t i = size; i < res->size; i++ ) res->data[ i ] = 0;
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_add( const bmath_vf3_s* o, const bmath_vf3_s* op, bmath_vf3_s* res )
{
    sz_t size = sz_min( o->size, sz_min( op->size, res->size ) );
    for( sz_t i = 0   ; i < size     ; i++ ) res->data[ i ] = o->data[ i ] + op->data[ i ];
    for( sz_t i = size; i < res->size; i++ ) res->data[ i ] = ( i < o->size ) ? o->data[ i ] : op->data[ i ];
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_sub( const bmath_vf3_s* o, const bmath_vf3_s* op, bmath_vf3_s* res )
{
    sz_t size = sz_min( o->size, sz_min( op->size, res->size ) );
    for( sz_t i = 0   ; i < size     ; i++ ) res->data[ i ] = o->data[ i ] - op->data[ i ];
    for( sz_t i = size; i < res->size; i++ ) res->data[ i ] = ( i < o->size ) ? o->data[ i ] : -op->data[ i ];
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_mul_scl( const bmath_vf3_s* o, const f3_t* op, bmath_vf3_s* res )
{
    sz_t size = sz_min( o->size, res->size );
    for( sz_t i = 0   ; i < size     ; i++ ) res->data[ i ] = o->data[ i ] * *op;
    for( sz_t i = size; i < res->size; i++ ) res->data[ i ] = 0;
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_mul_f3(  const bmath_vf3_s* o, f3_t op, bmath_vf3_s* res )
{
    sz_t size = sz_min( o->size, res->size );
    for( sz_t i = 0   ; i < size     ; i++ ) res->data[ i ] = o->data[ i ] * op;
    for( sz_t i = size; i < res->size; i++ ) res->data[ i ] = 0;
}

//---------------------------------------------------------------------------------------------------------------------

static f3_t f3_s_mul_vec( const f3_t* v1, const f3_t* v2, sz_t size )
{
    switch( size )
    {
        case 0: return 0;
        case 1: return v1[0]*v2[0];
        case 2: return v1[0]*v2[0]+v1[1]*v2[1];
        case 3: return v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2];
        case 4: return v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2]+v1[3]*v2[3];
        default: break;
    }
    sz_t sz1 = size >> 1;
    return f3_s_mul_vec( v1, v2, sz1 ) + f3_s_mul_vec( v1 + sz1, v2 + sz1, size - sz1 );
}

//---------------------------------------------------------------------------------------------------------------------

f3_t bmath_vf3_s_f3_mul_vec( const bmath_vf3_s* o, const bmath_vf3_s* vec2 )
{
    sz_t size = sz_min( o->size, vec2->size );
    return f3_s_mul_vec( o->data, vec2->data, size );
}

//---------------------------------------------------------------------------------------------------------------------

static f3_t f3_s_sqr( const f3_t* v1, sz_t size )
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
    sz_t sz1 = size >> 1;
    return f3_s_sqr( v1, sz1 ) + f3_s_sqr( v1 + sz1, size - sz1 );
}

//---------------------------------------------------------------------------------------------------------------------

f3_t bmath_vf3_s_f3_sqr( const bmath_vf3_s* o )
{
    return f3_s_sqr( o->data, o->size );
}

//---------------------------------------------------------------------------------------------------------------------

static f3_t f3_s_sqr_sub( const f3_t* v1, const f3_t* v2, sz_t size )
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
    sz_t sz1 = size >> 1;
    return f3_s_sqr_sub( v1, v2, sz1 ) + f3_s_sqr_sub( v1 + sz1, v2 + sz1, size - sz1 );
}

//---------------------------------------------------------------------------------------------------------------------

f3_t bmath_vf3_s_f3_sub_sqr( const bmath_vf3_s* o, const bmath_vf3_s* vec2 )
{
    sz_t size = sz_min( o->size, vec2->size );
    f3_t sum  = f3_s_sqr_sub( o->data, vec2->data, size );
    if( o   ->size > size ) sum += f3_s_sqr( o   ->data + size, o   ->size - size );
    if( vec2->size > size ) sum += f3_s_sqr( vec2->data + size, vec2->size - size );
    return sum;
}

//---------------------------------------------------------------------------------------------------------------------

static f3_t f3_s_f3_sum( const f3_t* v1, sz_t size )
{
    switch( size )
    {
        case 0: return 0;
        case 1: return v1[0];
        case 2: return v1[0]+v1[1];
        case 3: return v1[0]+v1[1]+v1[2];
        case 4: return v1[0]+v1[1]+v1[2]+v1[3];
        default: break;
    }
    sz_t sz1 = size >> 1;
    return f3_s_f3_sum( v1, sz1 ) + f3_s_f3_sum( v1 + sz1, size - sz1 );
}

//---------------------------------------------------------------------------------------------------------------------

static f3_t f3_s_f3_max( const f3_t* v1, sz_t size )
{
    f3_t max = ( size > 0 ) ? v1[ 0 ] : 0;
    for( sz_t i = 0; i < size; i++ ) max = f3_max( max, v1[ i ] );
    return max;
}

//---------------------------------------------------------------------------------------------------------------------

static f3_t f3_s_f3_min( const f3_t* v1, sz_t size )
{
    f3_t min = ( size > 0 ) ? v1[ 0 ] : 0;
    for( sz_t i = 0; i < size; i++ ) min = f3_min( min, v1[ i ] );
    return min;
}

//---------------------------------------------------------------------------------------------------------------------

f3_t bmath_vf3_s_f3_sum( const bmath_vf3_s* o )
{
    return f3_s_f3_sum( o->data, o->size );
}

//---------------------------------------------------------------------------------------------------------------------

f3_t bmath_vf3_s_f3_avg( const bmath_vf3_s* o )
{
    return o->size > 0 ? bmath_vf3_s_f3_sum( o ) / o->size : 0;
}

//---------------------------------------------------------------------------------------------------------------------

f3_t bmath_vf3_s_f3_var( const bmath_vf3_s* o )
{
    f3_t e_v  = bmath_vf3_s_f3_sum( o ) / ( ( o->size > 1 ) ? o->size : 1 );
    f3_t e_vv = bmath_vf3_s_f3_sqr( o ) / ( ( o->size > 1 ) ? o->size : 1 );
    return f3_max( 0.0, e_vv - f3_sqr( e_v ) );
}

//---------------------------------------------------------------------------------------------------------------------

f3_t bmath_vf3_s_f3_dev( const bmath_vf3_s* o )
{
    return f3_srt( bmath_vf3_s_f3_var( o ) );
}

//---------------------------------------------------------------------------------------------------------------------

f3_t bmath_vf3_s_f3_max( const bmath_vf3_s* o )
{
    return f3_s_f3_max( o->data, o->size );
}

//---------------------------------------------------------------------------------------------------------------------

f3_t bmath_vf3_s_f3_min( const bmath_vf3_s* o )
{
    return f3_s_f3_min( o->data, o->size );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_mul_vec( const bmath_vf3_s* o, const bmath_vf3_s* op, f3_t* res )
{
    *res = bmath_vf3_s_f3_mul_vec( o, op );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_sqr( const bmath_vf3_s* o, f3_t* res )
{
    *res = bmath_vf3_s_f3_sqr( o );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_sub_sqr( const bmath_vf3_s* o, const bmath_vf3_s* op, f3_t* res )
{
    *res = bmath_vf3_s_f3_sub_sqr( o, op );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_sum( const bmath_vf3_s* o, f3_t* res )
{
    *res = bmath_vf3_s_f3_sum( o );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_avg( const bmath_vf3_s* o, f3_t* res )
{
    *res = bmath_vf3_s_f3_avg( o );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_var( const bmath_vf3_s* o, f3_t* res )
{
    *res = bmath_vf3_s_f3_var( o );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_dev( const bmath_vf3_s* o, f3_t* res )
{
    *res = bmath_vf3_s_f3_dev( o );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_set_sqr( bmath_vf3_s* o, f3_t val )
{
    ASSERT( val >= 0 );
    f3_t cur = bmath_vf3_s_f3_sqr( o );
    f3_t fac = ( cur > 0 ) ? sqrt( val / cur ) : 0;
    bmath_vf3_s_mul_f3( o, fac, o );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_set_sum( bmath_vf3_s* o, f3_t val )
{
    f3_t cur = bmath_vf3_s_f3_sum( o );
    f3_t add = o->size > 0 ? ( val - cur ) / o->size : 0;
    for( sz_t i = 0; i < o->size; i++ ) o->data[ i ] += add;
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_set_avg( bmath_vf3_s* o, f3_t val )
{
    f3_t cur = bmath_vf3_s_f3_avg( o );
    f3_t add = val - cur;
    for( sz_t i = 0; i < o->size; i++ ) o->data[ i ] += add;
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_to_stdout( const bmath_vf3_s* o )
{
    const f3_t* v = o->data;
    printf( "(%zu)\n", o->size );
    for( sz_t i = 0; i < o->size; i++ ) printf( "%9.3g\n", v[ i ] );
}

void bmath_vf3_s_stat_to_stdout( const bmath_vf3_s* o )
{
    bcore_msg_fa( "(#<sz_t>)\n", o->size );
    bcore_msg_fa( "  sum .. #<f3_t>\n", bmath_vf3_s_f3_sum( o ) );
    bcore_msg_fa( "  avg .. #<f3_t>\n", bmath_vf3_s_f3_avg( o ) );
    bcore_msg_fa( "  max .. #<f3_t>\n", bmath_vf3_s_f3_max( o ) );
    bcore_msg_fa( "  min .. #<f3_t>\n", bmath_vf3_s_f3_min( o ) );
    bcore_msg_fa( "  dev .. #<f3_t>\n", bmath_vf3_s_f3_dev( o ) );
}

/**********************************************************************************************************************/
// bmath_vcf3_s

BCORE_DEFINE_OBJECT_INST( bmath_vector, bmath_vcf3_s )
"{"
    "aware_t _;"
    "bmath_cf3_s [] arr;"
    "func bmath_fp:add;"
    "func bmath_fp:zro;"
    "func bmath_fp:neg;"
    "func bmath_fp:sub;"
    "func bmath_fp:cpy;"
    "func bmath_fp_vector:mul_scl = bmath_vcf3_s_mul_scl;"
    "func bmath_fp_vector:mul_vec = bmath_vcf3_s_mul_vec;"
"}";

//---------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_move( bmath_vcf3_s* o, bmath_vcf3_s* src )
{
    if( o == src ) return;
    *o = *src;
    o->space = 0;
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_set_size( bmath_vcf3_s* o, sz_t size )
{
    bcore_array_a_set_size( (bcore_array*)o, size );
}

//---------------------------------------------------------------------------------------------------------------------

bmath_vcf3_s* bmath_vcf3_s_create_size( sz_t size )
{
    bmath_vcf3_s* o = bmath_vcf3_s_create();
    bcore_array_a_set_size( (bcore_array*)o, size );
    return o;
}

//---------------------------------------------------------------------------------------------------------------------

bmath_vcf3_s* bmath_vcf3_s_create_fill( bmath_cf3_s val, sz_t size )
{
    bmath_vcf3_s* o = bmath_vcf3_s_create();
    bcore_array_a_set_space( (bcore_array*)o, size );
    o->size = size;
    for( sz_t i = 0; i < size; i++ ) o->data[ i ] = val;
    return o;
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_zro( bmath_vcf3_s* o )
{
    bcore_u_memzero( sizeof( bmath_cf3_s ), o->data, o->size );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_neg( const bmath_vcf3_s* o, bmath_vcf3_s* res )
{
    sz_t size = sz_min( o->size, res->size );
    for( sz_t i = 0   ; i < size     ; i++ ) bmath_cf3_s_neg( &o->data[ i ], &res->data[ i ] );
    for( sz_t i = size; i < res->size; i++ ) bmath_cf3_s_zro( &res->data[ i ] );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_cnj( const bmath_vcf3_s* o, bmath_vcf3_s* res )
{
    sz_t size = sz_min( o->size, res->size );
    for( sz_t i = 0   ; i < size     ; i++ ) bmath_cf3_s_cnj( &o->data[ i ], &res->data[ i ] );
    for( sz_t i = size; i < res->size; i++ ) bmath_cf3_s_zro( &res->data[ i ] );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_cpy( const bmath_vcf3_s* o, bmath_vcf3_s* res )
{
    sz_t size = sz_min( o->size, res->size );
    for( sz_t i = 0   ; i < size     ; i++ ) res->data[ i ] = o->data[ i ];
    for( sz_t i = size; i < res->size; i++ ) bmath_cf3_s_zro( &res->data[ i ] );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_add( const bmath_vcf3_s* o, const bmath_vcf3_s* op, bmath_vcf3_s* res )
{
    sz_t size = sz_min( o->size, sz_min( op->size, res->size ) );
    for( sz_t i = 0   ; i < size     ; i++ ) bmath_cf3_s_add( &o->data[ i ], &op->data[ i ], &res->data[ i ] );
    for( sz_t i = size; i < res->size; i++ ) res->data[ i ] = ( i < o->size ) ? o->data[ i ] : op->data[ i ];
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_sub( const bmath_vcf3_s* o, const bmath_vcf3_s* op, bmath_vcf3_s* res )
{
    sz_t size = sz_min( o->size, sz_min( op->size, res->size ) );
    for( sz_t i = 0   ; i < size     ; i++ ) bmath_cf3_s_sub( &o->data[ i ], &op->data[ i ], &res->data[ i ] );
    for( sz_t i = size; i < res->size; i++ ) res->data[ i ] = ( i < o->size ) ? o->data[ i ] : bmath_cf3_neg( op->data[ i ] );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_mul_scl( const bmath_vcf3_s* o, const bmath_cf3_s* op, bmath_vcf3_s* res )
{
    sz_t size = sz_min( o->size, res->size );
    for( sz_t i = 0   ; i < size     ; i++ ) bmath_cf3_s_mul( &o->data[ i ], op, &res->data[ i ] );
    for( sz_t i = size; i < res->size; i++ ) bmath_cf3_s_zro( &res->data[ i ] );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_mul_cf3( const bmath_vcf3_s* o, bmath_cf3_s op, bmath_vcf3_s* res )
{
    sz_t size = sz_min( o->size, res->size );
    for( sz_t i = 0   ; i < size     ; i++ ) bmath_cf3_s_mul( &o->data[ i ], &op, &res->data[ i ] );
    for( sz_t i = size; i < res->size; i++ ) bmath_cf3_s_zro( &res->data[ i ] );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_mul_f3( const bmath_vcf3_s* o, f3_t op, bmath_vcf3_s* res )
{
    sz_t size = sz_min( o->size, res->size );
    for( sz_t i = 0   ; i < size     ; i++ ) bmath_cf3_s_mul_f3( &o->data[ i ], op, &res->data[ i ] );
    for( sz_t i = size; i < res->size; i++ ) bmath_cf3_s_zro( &res->data[ i ] );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_dft( const bmath_vcf3_s* o, bmath_vcf3_s* res )
{
    sz_t size = sz_min( o->size, res->size );
    if( ( size & ( size - 1 ) ) == 0 )
    {
        bmath_fourier_fft_f3( o->data, res->data, size );
    }
    else
    {
        bmath_fourier_dft_f3( o->data, res->data, size );
    }
    for( sz_t i = size; i < res->size; i++ ) bmath_cf3_s_zro( &res->data[ i ] );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_ift( const bmath_vcf3_s* o, bmath_vcf3_s* res )
{
    bmath_vcf3_s_cnj( o, res );
    bmath_vcf3_s_dft( res, res );
    bmath_vcf3_s_cnj( res, res );
    if( res->size > 0 ) bmath_vcf3_s_mul_f3( res, 1.0 / res->size, res );
}

//---------------------------------------------------------------------------------------------------------------------

static void vcf3_s_mul_vec( const bmath_cf3_s* v1, const bmath_cf3_s* v2, sz_t size, bmath_cf3_s* res )
{
    switch( size )
    {
        case 0:
        {
            bmath_cf3_s_zro( res );
            return;
        }

        case 1:
        {
            bmath_cf3_s_mul( &v1[0], &v2[0], res );
            return;
        }

        case 2:
        {
            bmath_cf3_s_mul( &v1[0], &v2[0], res );
            bmath_cf3_s_add_mul( res, &v1[1], &v2[1], res );
            return;
        }

        default: break;
    }

    sz_t sz1 = size >> 1;
    bmath_cf3_s val;
    vcf3_s_mul_vec( v1, v2, sz1, res );
    vcf3_s_mul_vec( v1 + sz1, v2 + sz1, size - sz1, &val );
    bmath_cf3_s_add( res, &val, res );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_mul_vec( const bmath_vcf3_s* o, const bmath_vcf3_s* op, bmath_cf3_s* res )
{
    sz_t size = sz_min( o->size, op->size );
    vcf3_s_mul_vec( o->data, op->data, size, res );
}

//---------------------------------------------------------------------------------------------------------------------

static void vcf3_s_sqr( const bmath_cf3_s* o, sz_t size, bmath_cf3_s* res )
{
    switch( size )
    {
        case 0:
        {
            bmath_cf3_s_zro( res );
            return;
        }

        case 1:
        {
            bmath_cf3_s_sqr( &o[0], res );
            return;
        }

        case 2:
        {
            bmath_cf3_s_sqr( &o[0], res );
            bmath_cf3_s_sqr_add( &o[1], res, res );
            return;
        }

        default: break;
    }

    sz_t sz1 = size >> 1;
    bmath_cf3_s val;
    vcf3_s_sqr( o, sz1, res );
    vcf3_s_sqr( o + sz1, size - sz1, &val );
    bmath_cf3_s_add( res, &val, res );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_sqr( const bmath_vcf3_s* o, bmath_cf3_s* res )
{
    vcf3_s_sqr( o->data, o->size, res );
}

//---------------------------------------------------------------------------------------------------------------------

static void vcf3_s_sub_sqr( const bmath_cf3_s* v1, const bmath_cf3_s* v2, sz_t size, bmath_cf3_s* res )
{
    switch( size )
    {
        case 0:
        {
            bmath_cf3_s_zro( res );
            return;
        }

        case 1:
        {
            bmath_cf3_s_sub_sqr( &v1[0], &v2[0], res );
            return;
        }

        case 2:
        {
            bmath_cf3_s val;
            bmath_cf3_s_sub_sqr( &v1[0], &v2[0], res );
            bmath_cf3_s_sub_sqr( &v1[1], &v2[1], &val );
            bmath_cf3_s_add( res, &val, res );
            return;
        }

        default: break;
    }

    sz_t sz1 = size >> 1;
    bmath_cf3_s val;
    vcf3_s_sub_sqr( v1, v2, sz1, res );
    vcf3_s_sub_sqr( v1 + sz1, v2 + sz1, size - sz1, &val );
    bmath_cf3_s_add( res, &val, res );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_sub_sqr( const bmath_vcf3_s* o, const bmath_vcf3_s* op, bmath_cf3_s* res )
{
    sz_t size = sz_min( o->size, op->size );
    vcf3_s_sub_sqr( o->data, op->data, size, res );
    if( o->size > size )
    {
        bmath_cf3_s val;
        vcf3_s_sqr( o->data + size, o->size - size, &val );
        bmath_cf3_s_add( res, &val, res );
    }

    if( op->size > size )
    {
        bmath_cf3_s val;
        vcf3_s_sqr( op->data + size, op->size - size, &val );
        bmath_cf3_s_sub( res, &val, res );
    }
}

//---------------------------------------------------------------------------------------------------------------------

static void vcf3_s_sum( const bmath_cf3_s* v1, sz_t size, bmath_cf3_s* res )
{
    switch( size )
    {
        case 0:
        {
            bmath_cf3_s_zro( res );
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

    sz_t sz1 = size >> 1;
    bmath_cf3_s val;
    vcf3_s_sum( v1, sz1, res );
    vcf3_s_sum( v1 + sz1, size - sz1, &val );
    bmath_cf3_s_add( res, &val, res );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_sum( const bmath_vcf3_s* o, bmath_cf3_s* res )
{
    vcf3_s_sum( o->data, o->size, res );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_f3_avg( const bmath_vcf3_s* o, bmath_cf3_s* res )
{
    if( o->size > 0 )
    {
        bmath_vcf3_s_sum( o, res );
        bmath_cf3_s_mul_f3( res, 1.0 / o->size, res );
    }
    else
    {
        bmath_cf3_s_zro( res );
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_vcf3_s_to_stdout( const bmath_vcf3_s* o )
{
    const bmath_cf3_s* v = o->data;
    for( sz_t i = 0; i < o->size; i++ ) bmath_cf3_s_to_stdout( &v[ i ] );
}

/**********************************************************************************************************************/
// array of vf3_s

BCORE_DEFINE_OBJECT_INST( bcore_array, bmath_arr_vf3_s )
"{"
    "aware_t _;"
    "bmath_vf3_s [] arr;"
"}";

//---------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_clear( bmath_arr_vf3_s* o )
{
    bcore_array_a_set_space( ( bcore_array* )o, 0 );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_set_space(  bmath_arr_vf3_s* o, sz_t space )
{
    bcore_array_a_set_space( ( bcore_array* )o, space );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_set_size( bmath_arr_vf3_s* o, sz_t size )
{
    bcore_array_a_set_size( ( bcore_array* )o, size );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_push( bmath_arr_vf3_s* o, const bmath_vf3_s* vec )
{
    bcore_array_a_push( ( bcore_array* )o, sr_awc( vec ) );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_push_arr( bmath_arr_vf3_s* o, const bmath_arr_vf3_s* arr )
{
    bcore_array_a_push_array( ( bcore_array* )o, sr_awc( arr ) );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_pop( bmath_arr_vf3_s* o )
{
    bcore_array_a_pop( ( bcore_array* )o );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_reorder( bmath_arr_vf3_s* o, const bcore_arr_sz_s* order )
{
    bcore_array_a_reorder( ( bcore_array* )o, order );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_on_section_set_size( bmath_arr_vf3_s* o, sz_t start, sz_t end, sz_t size )
{
    end = end < o->size ? end : o->size;
    for( sz_t i = start; i < end; i++ ) bmath_vf3_s_set_size( &o->data[ i ], size );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_on_section_fill( bmath_arr_vf3_s* o, sz_t start, sz_t end, f3_t val )
{
    end = end < o->size ? end : o->size;
    for( sz_t i = start; i < end; i++ ) bmath_vf3_s_fill( &o->data[ i ], val );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_on_section_fill_random( bmath_arr_vf3_s* o, sz_t start, sz_t end, f3_t min, f3_t max, u2_t* rval )
{
    end = end < o->size ? end : o->size;
    for( sz_t i = start; i < end; i++ ) bmath_vf3_s_fill_random( &o->data[ i ], min, max, rval );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_on_section_zro( bmath_arr_vf3_s* o, sz_t start, sz_t end )
{
    end = end < o->size ? end : o->size;
    for( sz_t i = start; i < end; i++ ) bmath_vf3_s_zro( &o->data[ i ] );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_on_section_neg( const bmath_arr_vf3_s* o, sz_t start, sz_t end, bmath_arr_vf3_s* res )
{
    end = end < o->size ? end : o->size;
    ASSERT( res->size >= end );
    for( sz_t i = start; i < end; i++ ) bmath_vf3_s_neg( &o->data[ i ], &res->data[ i ] );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_on_section_cpy( const bmath_arr_vf3_s* o, sz_t start, sz_t end, bmath_arr_vf3_s* res )
{
    end = end < o->size ? end : o->size;
    ASSERT( res->size >= end );
    for( sz_t i = start; i < end; i++ ) bmath_vf3_s_cpy( &o->data[ i ], &res->data[ i ] );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_on_section_add( const bmath_arr_vf3_s* o, sz_t start, sz_t end, const bmath_vf3_s* op, bmath_arr_vf3_s* res )
{
    end = end < o->size ? end : o->size;
    ASSERT( res->size >= end );
    for( sz_t i = start; i < end; i++ ) bmath_vf3_s_add( &o->data[ i ], op, &res->data[ i ] );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_on_section_sub( const bmath_arr_vf3_s* o, sz_t start, sz_t end, const bmath_vf3_s* op, bmath_arr_vf3_s* res )
{
    end = end < o->size ? end : o->size;
    ASSERT( res->size >= end );
    for( sz_t i = start; i < end; i++ ) bmath_vf3_s_sub( &o->data[ i ], op, &res->data[ i ] );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_on_section_mul_f3(  const bmath_arr_vf3_s* o, sz_t start, sz_t end, f3_t op, bmath_arr_vf3_s* res )
{
    end = end < o->size ? end : o->size;
    ASSERT( res->size >= end );
    for( sz_t i = start; i < end; i++ ) bmath_vf3_s_mul_f3( &o->data[ i ], op, &res->data[ i ] );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_on_section_set_sqr( bmath_arr_vf3_s* o, sz_t start, sz_t end, f3_t val )
{
    end = end < o->size ? end : o->size;
    for( sz_t i = start; i < end; i++ ) bmath_vf3_s_set_sqr( &o->data[ i ], val );
}

void bmath_arr_vf3_s_on_section_set_sum( bmath_arr_vf3_s* o, sz_t start, sz_t end, f3_t val )
{
    end = end < o->size ? end : o->size;
    for( sz_t i = start; i < end; i++ ) bmath_vf3_s_set_sum( &o->data[ i ], val );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_on_section_set_avg( bmath_arr_vf3_s* o, sz_t start, sz_t end, f3_t val )
{
    end = end < o->size ? end : o->size;
    for( sz_t i = start; i < end; i++ ) bmath_vf3_s_set_avg( &o->data[ i ], val );
}

//---------------------------------------------------------------------------------------------------------------------

f3_t bmath_arr_vf3_s_on_section_f3_sum_sprec( const bmath_arr_vf3_s* o, sz_t start, sz_t end, sz_t index )
{
    end   = end < o->size ? end : o->size;
    start = start > end ? end : start;
    switch( end - start )
    {
        case 0: return 0;
        case 1: return bmath_vf3_s_get_f3( &o->data[ start ], index );

        case 2: return bmath_vf3_s_get_f3( &o->data[ start     ], index ) +
                       bmath_vf3_s_get_f3( &o->data[ start + 1 ], index );

        case 3: return bmath_vf3_s_get_f3( &o->data[ start     ], index ) +
                       bmath_vf3_s_get_f3( &o->data[ start + 1 ], index ) +
                       bmath_vf3_s_get_f3( &o->data[ start + 2 ], index );

        case 4: return bmath_vf3_s_get_f3( &o->data[ start     ], index ) +
                       bmath_vf3_s_get_f3( &o->data[ start + 1 ], index ) +
                       bmath_vf3_s_get_f3( &o->data[ start + 2 ], index ) +
                       bmath_vf3_s_get_f3( &o->data[ start + 3 ], index );

        default: break;
    }

    sz_t mid = start + ( ( end - start ) >> 1 );

    return bmath_arr_vf3_s_on_section_f3_sum_sprec( o, start, mid, index ) +
           bmath_arr_vf3_s_on_section_f3_sum_sprec( o, mid,   end, index );
}

//---------------------------------------------------------------------------------------------------------------------

f3_t bmath_arr_vf3_s_on_section_f3_sum_coprd_sprec( const bmath_arr_vf3_s* o, sz_t start, sz_t end, f3_t ai, f3_t aj, sz_t i, sz_t j )
{
    end   = end < o->size ? end : o->size;
    start = start > end ? end : start;
    switch( end - start )
    {
        case 0: return 0;
        case 1:
        {
            const bmath_vf3_s* v = &o->data[ start ];
            ASSERT( i < v->size && j < v->size );
            return ( v->data[ i ] - ai ) * ( v->data[ j ] - aj );
        }
        break;

        case 2:
        {
            const bmath_vf3_s* v1 = &o->data[ start     ];
            const bmath_vf3_s* v2 = &o->data[ start + 1 ];
            ASSERT( i < v1->size && j < v1->size );
            ASSERT( i < v2->size && j < v2->size );
            return ( v1->data[ i ] - ai ) * ( v1->data[ j ] - aj ) +
                   ( v2->data[ i ] - ai ) * ( v2->data[ j ] - aj );
        }
        break;

        case 3:
        {
            const bmath_vf3_s* v1 = &o->data[ start     ];
            const bmath_vf3_s* v2 = &o->data[ start + 1 ];
            const bmath_vf3_s* v3 = &o->data[ start + 2 ];
            ASSERT( i < v1->size && j < v1->size );
            ASSERT( i < v2->size && j < v2->size );
            ASSERT( i < v3->size && j < v3->size );
            return ( v1->data[ i ] - ai ) * ( v1->data[ j ] - aj ) +
                   ( v2->data[ i ] - ai ) * ( v2->data[ j ] - aj ) +
                   ( v3->data[ i ] - ai ) * ( v3->data[ j ] - aj );
        }
        break;

        case 4:
        {
            const bmath_vf3_s* v1 = &o->data[ start     ];
            const bmath_vf3_s* v2 = &o->data[ start + 1 ];
            const bmath_vf3_s* v3 = &o->data[ start + 2 ];
            const bmath_vf3_s* v4 = &o->data[ start + 3 ];
            ASSERT( i < v1->size && j < v1->size );
            ASSERT( i < v2->size && j < v2->size );
            ASSERT( i < v3->size && j < v3->size );
            ASSERT( i < v4->size && j < v4->size );
            return ( v1->data[ i ] - ai ) * ( v1->data[ j ] - aj ) +
                   ( v2->data[ i ] - ai ) * ( v2->data[ j ] - aj ) +
                   ( v3->data[ i ] - ai ) * ( v3->data[ j ] - aj ) +
                   ( v4->data[ i ] - ai ) * ( v4->data[ j ] - aj );
        }
        break;

        default: break;
    }

    sz_t mid = start + ( ( end - start ) >> 1 );

    return bmath_arr_vf3_s_on_section_f3_sum_coprd_sprec( o, start, mid, ai, aj, i, j ) +
           bmath_arr_vf3_s_on_section_f3_sum_coprd_sprec( o, mid,   end, ai, aj, i, j );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_on_section_get_sum_sprc( const bmath_arr_vf3_s* o, sz_t start, sz_t end, bmath_vf3_s* res )
{
    for( sz_t i = 0; i < res->size; i++ ) res->data[ i ] = bmath_arr_vf3_s_on_section_f3_sum_sprec( o, start, end, i );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_on_section_get_avg_sprc( const bmath_arr_vf3_s* o, sz_t start, sz_t end, bmath_vf3_s* res )
{
    end = end < o->size ? end : o->size;
    start = start > end ? end : start;
    bmath_arr_vf3_s_on_section_get_sum_sprc( o, start, end, res );
    if( end > start ) bmath_vf3_s_mul_f3( res, 1.0 / (end - start), res );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_on_section_get_sum_fast( const bmath_arr_vf3_s* o, sz_t start, sz_t end, bmath_vf3_s* res )
{
    end = end < o->size ? end : o->size;
    bmath_vf3_s_zro( res );
    for( sz_t i = start; i < end; i++ ) bmath_vf3_s_add( &o->data[ i ], res, res );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_arr_vf3_s_on_section_get_avg_fast( const bmath_arr_vf3_s* o, sz_t start, sz_t end, bmath_vf3_s* res )
{
    end = end < o->size ? end : o->size;
    start = start > end ? end : start;
    bmath_arr_vf3_s_on_section_get_sum_fast( o, start, end, res );
    if( end > start ) bmath_vf3_s_mul_f3( res, 1.0 / (end - start), res );
}

//---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

static vd_t selftest( void )
{
    bcore_life_s* l = bcore_life_s_create();

    /// vf3
    {
        sz_t size = 100;
        bmath_vf3_s* v1 = bcore_life_s_push_aware( l, bmath_vf3_s_create_set_size( size ) );
        for( sz_t i = 0; i < size; i++ ) v1->data[ i ] = i + 1;
        f3_t sqr_sum = 0;
        for( sz_t i = 0; i < size; i++ ) sqr_sum += f3_sqr( v1->data[ i ] );
        ASSERT( bmath_vf3_s_f3_sum( v1 ) == ( size * ( size + 1 ) ) / 2 );
        ASSERT( bmath_vf3_s_f3_avg( v1 ) == ( f3_t )( size * ( size + 1 ) ) / ( 2 * size ) );
        ASSERT( bmath_vf3_s_f3_sqr( v1 ) == sqr_sum );
        ASSERT( bmath_vf3_s_f3_sqr( v1 ) == bmath_vf3_s_f3_mul_vec( v1, v1 ) );

        bmath_vf3_s* v2 = bcore_life_s_push_aware( l, bmath_vf3_s_create_set_size( size ) );
        bmath_vf3_s_cpy( v1, v2 );
        ASSERT( bmath_vf3_s_f3_sum( v2 ) ==   bmath_vf3_s_f3_sum( v1 ) );
        bmath_vf3_s_neg( v1, v2 );
        ASSERT( bmath_vf3_s_f3_sum( v2 ) == - bmath_vf3_s_f3_sum( v1 ) );
        bmath_vf3_s_mul_f3( v1, 2.0, v2 );
        ASSERT( bmath_vf3_s_f3_sum(     v2     ) == 2.0 * bmath_vf3_s_f3_sum( v1 ) );
        ASSERT( bmath_vf3_s_f3_mul_vec( v1, v2 ) == 2.0 * bmath_vf3_s_f3_sqr( v1 ) );

        f3_t v = 0;
        bmath_vector_a_mul_vec( ( const bmath_vector* )v1, ( const bmath_vector* )v2, ( bmath_ring* )&v );
        ASSERT( bmath_vf3_s_f3_mul_vec( v1, v2 ) == v );
    }

    /// cvf3
    {
        sz_t size = 100;
        bmath_vcf3_s* v1 = bcore_life_s_push_aware( l, bmath_vcf3_s_create_size( size ) );
        for( sz_t i = 0; i < size; i++ )
        {
            v1->data[ i ].v[ 0 ] = i + 1;
            v1->data[ i ].v[ 1 ] = i;
        }

//        bcore_txt_ml_aware_to_stdout( v1 );

        bmath_cf3_s sum1 = bmath_cf3_zro();
        bmath_cf3_s sqr1 = bmath_cf3_zro();
        for( sz_t i = 0; i < size; i++ )
        {
            bmath_cf3_s_sqr_add( &v1->data[ i ], &sqr1, &sqr1 );
            bmath_cf3_s_add( &v1->data[ i ], &sum1, &sum1  );
        }

//        bcore_txt_ml_to_stdout( sr_twc( TYPEOF_bmath_cf3_s, &sum1 ) );
//        bcore_txt_ml_to_stdout( sr_twc( TYPEOF_bmath_cf3_s, &sqr1 ) );

        bmath_cf3_s sum2 = bmath_cf3_zro();
        bmath_cf3_s sqr2 = bmath_cf3_zro();
        bmath_vcf3_s_sum( v1, &sum2 );
        bmath_vcf3_s_sqr( v1, &sqr2 );

        ASSERT( bmath_cf3_equ( sum1, sum2 ) );
        ASSERT( bmath_cf3_equ( sqr1, sqr2 ) );

        bmath_vcf3_s* v2 = bcore_life_s_push_aware( l, bmath_vcf3_s_create_size( size ) );
        bmath_vcf3_s_cpy( v1, v2 );
        bmath_vcf3_s_sum( v1, &sum1 );
        bmath_vcf3_s_sum( v2, &sum2 );
        ASSERT( bmath_cf3_equ( sum1, sum2 ) );

        bmath_vcf3_s_neg( v1, v2 );
        bmath_vcf3_s_sum( v1, &sum1 );
        bmath_vcf3_s_sum( v2, &sum2 );
        ASSERT( bmath_cf3_equ( sum1, bmath_cf3_neg( sum2 ) ) );

        bmath_vcf3_s_mul_f3( v1, 2.0, v2 );
        bmath_vcf3_s_sum( v1, &sum1 );
        bmath_vcf3_s_sum( v2, &sum2 );
        ASSERT( bmath_cf3_equ( sum2, bmath_cf3_mul_f3( sum1, 2.0 ) ) );
        bmath_vcf3_s_sqr( v1, &sqr1 );
        bmath_vcf3_s_mul_vec( v1, v2, &sqr2 );
        ASSERT( bmath_cf3_equ( sqr2, bmath_cf3_mul_f3( sqr1, 2.0 ) ) );
        bmath_vector_a_mul_vec( ( const bmath_vector* )v1, ( const bmath_vector* )v2, ( bmath_ring* )&sqr2 );
        ASSERT( bmath_cf3_equ( sqr2, bmath_cf3_mul_f3( sqr1, 2.0 ) ) );

        bmath_vcf3_s_dft( v1, v2 );
        bmath_vcf3_s_ift( v2, v2 );
        bmath_vcf3_s_sub_sqr( v1, v2, &sqr1 );
        ASSERT( bmath_cf3_mag( sqr1 ) < 1E-14 );
    }

    /// bmath_arr_vf3_s
    {
        sz_t size = 10;
        sz_t n    = 5;
        bmath_arr_vf3_s* a1 = bcore_life_s_push_aware( l, bmath_arr_vf3_s_create() );
        bmath_vf3_s*     v1 = bcore_life_s_push_aware( l, bmath_vf3_s_create() );
        bmath_vf3_s*     v2 = bcore_life_s_push_aware( l, bmath_vf3_s_create() );

        bmath_arr_vf3_s_set_size( a1, size );
        bmath_vf3_s_set_size( v1, n );
        bmath_vf3_s_set_size( v2, n );

        bmath_arr_vf3_s_on_section_set_size( a1, 0, -1, n );
        u2_t rval = 123;
        bmath_arr_vf3_s_on_section_fill_random( a1, 0, -1, -1, 1, &rval );
        bmath_arr_vf3_s_on_section_set_sqr( a1, 0, -1, 4 );
        for( sz_t i = 0; i < size; i++ ) ASSERT( f3_abs( bmath_vf3_s_f3_sqr( &a1->data[ i ] ) - 4.0 ) < 1E-10 );
        bmath_arr_vf3_s_on_section_set_avg( a1, 0, -1, 2 );
        bmath_arr_vf3_s_on_section_get_avg_fast( a1, 0, -1, v1 );
        ASSERT( f3_abs( bmath_vf3_s_f3_avg( v1 ) - 2 ) < 1E-10 );
        bmath_arr_vf3_s_on_section_get_avg_sprc( a1, 0, -1, v2 );
        ASSERT( bmath_vf3_s_is_near_equ( v1, v2, 1E-10 ) );
    }

    bcore_life_s_discard( l );

    return NULL;
}

/**********************************************************************************************************************/

vd_t bmath_vector_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_vector" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_FFUNC( bmath_fp_add,            bmath_vf3_s_add );
            BCORE_REGISTER_FFUNC( bmath_fp_zro,            bmath_vf3_s_zro );
            BCORE_REGISTER_FFUNC( bmath_fp_neg,            bmath_vf3_s_neg );
            BCORE_REGISTER_FFUNC( bmath_fp_sub,            bmath_vf3_s_sub );
            BCORE_REGISTER_FFUNC( bmath_fp_cpy,            bmath_vf3_s_cpy );
            BCORE_REGISTER_FFUNC( bmath_fp_vector_mul_vec, bmath_vf3_s_mul_vec );
            BCORE_REGISTER_FFUNC( bmath_fp_vector_mul_scl, bmath_vf3_s_mul_scl );
            BCORE_REGISTER_OBJECT( bmath_vf3_s );

            BCORE_REGISTER_OBJECT( bmath_arr_vf3_s );

            BCORE_REGISTER_FFUNC( bmath_fp_add,            bmath_vcf3_s_add );
            BCORE_REGISTER_FFUNC( bmath_fp_zro,            bmath_vcf3_s_zro );
            BCORE_REGISTER_FFUNC( bmath_fp_neg,            bmath_vcf3_s_neg );
            BCORE_REGISTER_FFUNC( bmath_fp_sub,            bmath_vcf3_s_sub );
            BCORE_REGISTER_FFUNC( bmath_fp_cpy,            bmath_vcf3_s_cpy );
            BCORE_REGISTER_FFUNC( bmath_fp_vector_mul_vec, bmath_vcf3_s_mul_vec );
            BCORE_REGISTER_FFUNC( bmath_fp_vector_mul_scl, bmath_vcf3_s_mul_scl );
            BCORE_REGISTER_OBJECT( bmath_vcf3_s );
        }
        break;

        case TYPEOF_selftest:
        {
            return selftest();
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

