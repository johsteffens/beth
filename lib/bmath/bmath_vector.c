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

#include "bmath_vector.h"
#include "bmath_spect_vector.h"
#include "bmath_fourier.h"

/**********************************************************************************************************************/
// bmath_vf3_s

static const sc_t bmath_vf3_s_def = "bmath_vf3_s = bmath_vector"
"{"
    "aware_t _;"
    "f3_t [] arr;"
    "func bmath_fp_add add                = bmath_vf3_s_add;"
    "func bmath_fp_zro zro                = bmath_vf3_s_zro;"
    "func bmath_fp_neg neg                = bmath_vf3_s_neg;"
    "func bmath_fp_sub sub                = bmath_vf3_s_sub;"
    "func bmath_fp_cpy cpy                = bmath_vf3_s_cpy;"
    "func bmath_fp_vector_mul  vector_mul = bmath_vf3_s_mul;"
    "func bmath_fp_vector_dot_prd dot_prd = bmath_vf3_s_dot_prd;"
"}";

BCORE_DEFINE_OBJECT_INST( bmath_vf3_s, bmath_vf3_s_def )

bmath_vf3_s* bmath_vf3_s_create_size( sz_t size )
{
    bmath_vf3_s* o = bmath_vf3_s_create();
    bcore_array_aware_set_size( o, size );
    return o;
}

bmath_vf3_s* bmath_vf3_s_create_fill( f3_t val, sz_t size )
{
    bmath_vf3_s* o = bmath_vf3_s_create();
    bcore_array_aware_set_space( o, size );
    o->size = size;
    for( sz_t i = 0; i < size; i++ ) o->data[ i ] = val;
    return o;
}

void bmath_vf3_s_zro( bmath_vf3_s* o )
{
    bcore_u_memzero( sizeof( f3_t ), o->data, o->size );
}

void bmath_vf3_s_neg( bmath_vf3_s* o, const bmath_vf3_s* vec1 )
{
    sz_t size = sz_min( o->size, vec1->size );
    for( sz_t i = 0   ; i < size   ; i++ ) o->data[ i ] = -vec1->data[ i ];
    for( sz_t i = size; i < o->size; i++ ) o->data[ i ] = 0;
}

void bmath_vf3_s_cpy( bmath_vf3_s* o, const bmath_vf3_s* vec1 )
{
    sz_t size = sz_min( o->size, vec1->size );
    for( sz_t i = 0   ; i < size   ; i++ ) o->data[ i ] = vec1->data[ i ];
    for( sz_t i = size; i < o->size; i++ ) o->data[ i ] = 0;
}

void bmath_vf3_s_add( bmath_vf3_s* o, const bmath_vf3_s* vec1, const bmath_vf3_s* vec2 )
{
    sz_t size = sz_min( o->size, sz_min( vec1->size, vec2->size ) );
    for( sz_t i = 0   ; i < size   ; i++ ) o->data[ i ] = vec1->data[ i ] + vec2->data[ i ];
    for( sz_t i = size; i < o->size; i++ ) o->data[ i ] = ( i < vec1->size ) ? vec1->data[ i ] : vec2->data[ i ];
}

void bmath_vf3_s_sub( bmath_vf3_s* o, const bmath_vf3_s* vec1, const bmath_vf3_s* vec2 )
{
    sz_t size = sz_min( o->size, sz_min( vec1->size, vec2->size ) );
    for( sz_t i = 0   ; i < size   ; i++ ) o->data[ i ] = vec1->data[ i ] - vec2->data[ i ];
    for( sz_t i = size; i < o->size; i++ ) o->data[ i ] = ( i < vec1->size ) ? vec1->data[ i ] : -vec2->data[ i ];
}

void bmath_vf3_s_mul( bmath_vf3_s* o, const bmath_vf3_s* vec1, const f3_t* scl2 )
{
    sz_t size = sz_min( o->size, vec1->size );
    for( sz_t i = 0   ; i < size   ; i++ ) o->data[ i ] = vec1->data[ i ] * *scl2;
    for( sz_t i = size; i < o->size; i++ ) o->data[ i ] = 0;
}

void bmath_vf3_s_mul_f3(  bmath_vf3_s* o, const bmath_vf3_s* vec1, f3_t scl2 )
{
    sz_t size = sz_min( o->size, vec1->size );
    for( sz_t i = 0   ; i < size   ; i++ ) o->data[ i ] = vec1->data[ i ] * scl2;
    for( sz_t i = size; i < o->size; i++ ) o->data[ i ] = 0;
}

static f3_t f3_s_dot_prd( const f3_t* v1, const f3_t* v2, sz_t size )
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
    return f3_s_dot_prd( v1, v2, sz1 ) + f3_s_dot_prd( v1 + sz1, v2 + sz1, size - sz1 );
}

f3_t bmath_vf3_s_f3_dot_prd( const bmath_vf3_s* o, const bmath_vf3_s* vec2 )
{
    sz_t size = sz_min( o->size, vec2->size );
    return f3_s_dot_prd( o->data, vec2->data, size );
}

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

f3_t bmath_vf3_s_f3_sqr( const bmath_vf3_s* o )
{
    return f3_s_sqr( o->data, o->size );
}

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

f3_t bmath_vf3_s_f3_sqr_sub( const bmath_vf3_s* o, const bmath_vf3_s* vec2 )
{
    sz_t size = sz_min( o->size, vec2->size );
    f3_t sum  = f3_s_sqr_sub( o->data, vec2->data, size );
    if( o   ->size > size ) sum += f3_s_sqr( o   ->data + size, o   ->size - size );
    if( vec2->size > size ) sum += f3_s_sqr( vec2->data + size, vec2->size - size );
    return sum;
}

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

f3_t bmath_vf3_s_f3_sum( const bmath_vf3_s* o )
{
    return f3_s_f3_sum( o->data, o->size );
}

f3_t bmath_vf3_s_f3_avg( const bmath_vf3_s* o )
{
    return o->size > 0 ? bmath_vf3_s_f3_sum( o ) / o->size : 0;
}

f3_t bmath_vf3_s_f3_var( const bmath_vf3_s* o )
{
    return ( o->size > 1 ) ? bmath_vf3_s_f3_sqr( o ) / ( o->size - 1 ) : 0;
}

f3_t bmath_vf3_s_f3_dev( const bmath_vf3_s* o )
{
    return f3_srt( bmath_vf3_s_f3_var( o ) );
}

void bmath_vf3_s_dot_prd( f3_t* s, const bmath_vf3_s* o, const bmath_vf3_s* vec2 )
{
    *s = bmath_vf3_s_f3_dot_prd( o, vec2 );
}

void bmath_vf3_s_sqr( f3_t* s, const bmath_vf3_s* o )
{
    *s = bmath_vf3_s_f3_sqr( o );
}

void bmath_vf3_s_sqr_sub( f3_t* s, const bmath_vf3_s* o, const bmath_vf3_s* vec2 )
{
    *s = bmath_vf3_s_f3_sqr_sub( o, vec2 );
}

void bmath_vf3_s_sum( f3_t* s, const bmath_vf3_s* o )
{
    *s = bmath_vf3_s_f3_sum( o );
}

void bmath_vf3_s_avg( f3_t* s, const bmath_vf3_s* o )
{
    *s = bmath_vf3_s_f3_avg( o );
}

void bmath_vf3_s_var( f3_t* s, const bmath_vf3_s* o )
{
    *s = bmath_vf3_s_f3_var( o );
}

void bmath_vf3_s_dev( f3_t* s, const bmath_vf3_s* o )
{
    *s = bmath_vf3_s_f3_dev( o );
}

/**********************************************************************************************************************/
// bmath_vcf3_s

static const sc_t bmath_vcf3_s_def = "bmath_vcf3_s = bmath_vector"
"{"
    "aware_t _;"
    "bmath_cf3_s [] arr;"
    "func bmath_fp_add add                = bmath_vcf3_s_add;"
    "func bmath_fp_zro zro                = bmath_vcf3_s_zro;"
    "func bmath_fp_neg neg                = bmath_vcf3_s_neg;"
    "func bmath_fp_sub sub                = bmath_vcf3_s_sub;"
    "func bmath_fp_cpy cpy                = bmath_vcf3_s_cpy;"
    "func bmath_fp_vector_mul  vector_mul = bmath_vcf3_s_mul;"
    "func bmath_fp_vector_dot_prd dot_prd = bmath_vcf3_s_dot_prd;"
"}";

BCORE_DEFINE_OBJECT_INST( bmath_vcf3_s, bmath_vcf3_s_def )

bmath_vcf3_s* bmath_vcf3_s_create_size( sz_t size )
{
    bmath_vcf3_s* o = bmath_vcf3_s_create();
    bcore_array_aware_set_size( o, size );
    return o;
}

bmath_vcf3_s* bmath_vcf3_s_create_fill( bmath_cf3_s val, sz_t size )
{
    bmath_vcf3_s* o = bmath_vcf3_s_create();
    bcore_array_aware_set_space( o, size );
    o->size = size;
    for( sz_t i = 0; i < size; i++ ) o->data[ i ] = val;
    return o;
}

void bmath_vcf3_s_zro( bmath_vcf3_s* o )
{
    bcore_u_memzero( sizeof( bmath_cf3_s ), o->data, o->size );
}

void bmath_vcf3_s_neg( bmath_vcf3_s* o, const bmath_vcf3_s* vec1 )
{
    sz_t size = sz_min( o->size, vec1->size );
    for( sz_t i = 0   ; i < size   ; i++ ) bmath_cf3_s_neg( &o->data[ i ], &vec1->data[ i ] );
    for( sz_t i = size; i < o->size; i++ ) bmath_cf3_s_zro( &o->data[ i ] );
}

void bmath_vcf3_s_cnj( bmath_vcf3_s* o, const bmath_vcf3_s* vec1 )
{
    sz_t size = sz_min( o->size, vec1->size );
    for( sz_t i = 0   ; i < size   ; i++ ) bmath_cf3_s_cnj( &o->data[ i ], &vec1->data[ i ] );
    for( sz_t i = size; i < o->size; i++ ) bmath_cf3_s_zro( &o->data[ i ] );
}

void bmath_vcf3_s_cpy( bmath_vcf3_s* o, const bmath_vcf3_s* vec1 )
{
    sz_t size = sz_min( o->size, vec1->size );
    for( sz_t i = 0   ; i < size   ; i++ ) o->data[ i ] = vec1->data[ i ];
    for( sz_t i = size; i < o->size; i++ ) bmath_cf3_s_zro( &o->data[ i ] );
}

void bmath_vcf3_s_add( bmath_vcf3_s* o, const bmath_vcf3_s* vec1, const bmath_vcf3_s* vec2 )
{
    sz_t size = sz_min( o->size, sz_min( vec1->size, vec2->size ) );
    for( sz_t i = 0   ; i < size   ; i++ ) bmath_cf3_s_add( &o->data[ i ], &vec1->data[ i ], &vec2->data[ i ] );
    for( sz_t i = size; i < o->size; i++ ) o->data[ i ] = ( i < vec1->size ) ? vec1->data[ i ] : vec2->data[ i ];
}

void bmath_vcf3_s_sub( bmath_vcf3_s* o, const bmath_vcf3_s* vec1, const bmath_vcf3_s* vec2 )
{
    sz_t size = sz_min( o->size, sz_min( vec1->size, vec2->size ) );
    for( sz_t i = 0   ; i < size   ; i++ ) bmath_cf3_s_sub( &o->data[ i ], &vec1->data[ i ], &vec2->data[ i ] );
    for( sz_t i = size; i < o->size; i++ ) o->data[ i ] = ( i < vec1->size ) ? vec1->data[ i ] : bmath_cf3_neg( vec2->data[ i ] );
}

void bmath_vcf3_s_mul( bmath_vcf3_s* o, const bmath_vcf3_s* vec1, const bmath_cf3_s* scl2 )
{
    sz_t size = sz_min( o->size, vec1->size );
    for( sz_t i = 0   ; i < size   ; i++ ) bmath_cf3_s_mul( &o->data[ i ], &vec1->data[ i ], scl2 );
    for( sz_t i = size; i < o->size; i++ ) bmath_cf3_s_zro( &o->data[ i ] );
}

void bmath_vcf3_s_mul_cf3(  bmath_vcf3_s* o, const bmath_vcf3_s* vec1, bmath_cf3_s scl2 )
{
    sz_t size = sz_min( o->size, vec1->size );
    for( sz_t i = 0   ; i < size   ; i++ ) bmath_cf3_s_mul( &o->data[ i ], &vec1->data[ i ], &scl2 );
    for( sz_t i = size; i < o->size; i++ ) bmath_cf3_s_zro( &o->data[ i ] );
}

void bmath_vcf3_s_mul_f3(  bmath_vcf3_s* o, const bmath_vcf3_s* vec1, f3_t scl2 )
{
    sz_t size = sz_min( o->size, vec1->size );
    for( sz_t i = 0   ; i < size   ; i++ ) bmath_cf3_s_mul_f3( &o->data[ i ], &vec1->data[ i ], scl2 );
    for( sz_t i = size; i < o->size; i++ ) bmath_cf3_s_zro( &o->data[ i ] );
}

void bmath_vcf3_s_dft( bmath_vcf3_s* o, const bmath_vcf3_s* vec1 )
{
    sz_t size = sz_min( o->size, vec1->size );
    if( ( size & ( size - 1 ) ) == 0 )
    {
        bmath_fourier_fft_f3( o->data, vec1->data, size );
    }
    else
    {
        bmath_fourier_dft_f3( o->data, vec1->data, size );
    }
    for( sz_t i = size; i < o->size; i++ ) bmath_cf3_s_zro( &o->data[ i ] );
}

void bmath_vcf3_s_ift( bmath_vcf3_s* o, const bmath_vcf3_s* vec1 )
{
    bmath_vcf3_s_cnj( o, vec1 );
    bmath_vcf3_s_dft( o, o );
    bmath_vcf3_s_cnj( o, o );
    if( o->size > 0 ) bmath_vcf3_s_mul_f3( o, o, 1.0 / o->size );
}

static void vcf3_s_dot_prd( bmath_cf3_s* scl, const bmath_cf3_s* v1, const bmath_cf3_s* v2, sz_t size )
{
    switch( size )
    {
        case 0:
        {
            bmath_cf3_s_zro( scl );
            return;
        }

        case 1:
        {
            bmath_cf3_s_mul( scl, &v1[0], &v2[0] );
            return;
        }

        case 2:
        {
            bmath_cf3_s_mul( scl, &v1[0], &v2[0] );
            bmath_cf3_s_add_mul( scl, scl, &v1[1], &v2[1] );
            return;
        }

        default: break;
    }

    sz_t sz1 = size >> 1;
    bmath_cf3_s val;
    vcf3_s_dot_prd( scl, v1, v2, sz1 );
    vcf3_s_dot_prd( &val, v1 + sz1, v2 + sz1, size - sz1 );
    bmath_cf3_s_add( scl, scl, &val );
}

void bmath_vcf3_s_dot_prd( bmath_cf3_s* s, const bmath_vcf3_s* o, const bmath_vcf3_s* vec2 )
{
    sz_t size = sz_min( o->size, vec2->size );
    vcf3_s_dot_prd( s, o->data, vec2->data, size );
}

static void vcf3_s_sqr( bmath_cf3_s* scl, const bmath_cf3_s* v1, sz_t size )
{
    switch( size )
    {
        case 0:
        {
            bmath_cf3_s_zro( scl );
            return;
        }

        case 1:
        {
            bmath_cf3_s_sqr( scl, &v1[0] );
            return;
        }

        case 2:
        {
            bmath_cf3_s_sqr( scl, &v1[0] );
            bmath_cf3_s_add_sqr( scl, scl, &v1[1] );
            return;
        }

        default: break;
    }

    sz_t sz1 = size >> 1;
    bmath_cf3_s val;
    vcf3_s_sqr( scl, v1, sz1 );
    vcf3_s_sqr( &val, v1 + sz1, size - sz1 );
    bmath_cf3_s_add( scl, scl, &val );
}

void bmath_vcf3_s_sqr( bmath_cf3_s* s, const bmath_vcf3_s* o )
{
    vcf3_s_sqr( s, o->data, o->size );
}

static void vcf3_s_sqr_sub( bmath_cf3_s* scl, const bmath_cf3_s* v1, const bmath_cf3_s* v2, sz_t size )
{
    switch( size )
    {
        case 0:
        {
            bmath_cf3_s_zro( scl );
            return;
        }

        case 1:
        {
            bmath_cf3_s_sqr_sub( scl, &v1[0], &v2[0] );
            return;
        }

        case 2:
        {
            bmath_cf3_s val;
            bmath_cf3_s_sqr_sub(  scl, &v1[0], &v2[0] );
            bmath_cf3_s_sqr_sub( &val, &v1[1], &v2[1] );
            bmath_cf3_s_add( scl, scl, &val );
            return;
        }

        default: break;
    }

    sz_t sz1 = size >> 1;
    bmath_cf3_s val;
    vcf3_s_sqr_sub( scl, v1, v2, sz1 );
    vcf3_s_sqr_sub( &val, v1 + sz1, v2 + sz1, size - sz1 );
    bmath_cf3_s_add( scl, scl, &val );
}

void bmath_vcf3_s_sqr_sub( bmath_cf3_s* s, const bmath_vcf3_s* o, const bmath_vcf3_s* vec2 )
{
    sz_t size = sz_min( o->size, vec2->size );
    vcf3_s_sqr_sub( s, o->data, vec2->data, size );
    if( o->size > size )
    {
        bmath_cf3_s val;
        vcf3_s_sqr( &val, o->data + size, o->size - size );
        bmath_cf3_s_add( s, s, &val );
    }

    if( vec2->size > size )
    {
        bmath_cf3_s val;
        vcf3_s_sqr( &val, vec2->data + size, vec2->size - size );
        bmath_cf3_s_sub( s, s, &val );
    }
}

static void vcf3_s_sum( bmath_cf3_s* scl, const bmath_cf3_s* v1, sz_t size )
{
    switch( size )
    {
        case 0:
        {
            bmath_cf3_s_zro( scl );
            return;
        }

        case 1:
        {
            *scl = v1[0];
            return;
        }

        case 2:
        {
            scl->v[0]=v1[0].v[0]+v1[1].v[0];
            scl->v[1]=v1[0].v[1]+v1[1].v[1];
            return;
        }

        case 3:
        {
            scl->v[0]=v1[0].v[0]+v1[1].v[0]+v1[2].v[0];
            scl->v[1]=v1[0].v[1]+v1[1].v[1]+v1[2].v[1];
            return;
        }

        case 4:
        {
            scl->v[0]=v1[0].v[0]+v1[1].v[0]+v1[2].v[0]+v1[3].v[0];
            scl->v[1]=v1[0].v[1]+v1[1].v[1]+v1[2].v[1]+v1[3].v[1];
            return;
        }

        default: break;
    }

    sz_t sz1 = size >> 1;
    bmath_cf3_s val;
    vcf3_s_sum( scl, v1, sz1 );
    vcf3_s_sum( &val, v1 + sz1, size - sz1 );
    bmath_cf3_s_add( scl, scl, &val );
}

void bmath_vcf3_s_sum( bmath_cf3_s* scl, const bmath_vcf3_s* o )
{
    vcf3_s_sum( scl, o->data, o->size );
}

void bmath_vcf3_s_f3_avg( bmath_cf3_s* scl, const bmath_vcf3_s* o )
{
    if( o->size > 0 )
    {
        bmath_vcf3_s_sum( scl, o );
        bmath_cf3_s_mul_f3( scl, scl, 1.0 / o->size );
    }
    else
    {
        bmath_cf3_s_zro( scl );
    }
}

/**********************************************************************************************************************/

static vd_t selftest( void )
{
    bcore_life_s* l = bcore_life_s_create();

    /// vf3
    {
        sz_t size = 100;
        bmath_vf3_s* v1 = bcore_life_s_push_aware( l, bmath_vf3_s_create_size( size ) );
        for( sz_t i = 0; i < size; i++ ) v1->data[ i ] = i + 1;
        f3_t sqr_sum = 0;
        for( sz_t i = 0; i < size; i++ ) sqr_sum += f3_sqr( v1->data[ i ] );
        ASSERT( bmath_vf3_s_f3_sum( v1 ) == ( size * ( size + 1 ) ) / 2 );
        ASSERT( bmath_vf3_s_f3_avg( v1 ) == ( f3_t )( size * ( size + 1 ) ) / ( 2 * size ) );
        ASSERT( bmath_vf3_s_f3_sqr( v1 ) == sqr_sum );
        ASSERT( bmath_vf3_s_f3_sqr( v1 ) == bmath_vf3_s_f3_dot_prd( v1, v1 ) );
        ASSERT( bmath_vf3_s_f3_var( v1 ) == sqr_sum / ( size - 1 ) );
        ASSERT( bmath_vf3_s_f3_dev( v1 ) == f3_srt( sqr_sum / ( size - 1 ) ) );

        bmath_vf3_s* v2 = bcore_life_s_push_aware( l, bmath_vf3_s_create_size( size ) );
        bmath_vf3_s_cpy( v2, v1 );
        ASSERT( bmath_vf3_s_f3_sum( v2 ) ==   bmath_vf3_s_f3_sum( v1 ) );
        bmath_vf3_s_neg( v2, v1 );
        ASSERT( bmath_vf3_s_f3_sum( v2 ) == - bmath_vf3_s_f3_sum( v1 ) );
        bmath_vf3_s_mul_f3( v2, v1, 2.0 );
        ASSERT( bmath_vf3_s_f3_sum(     v2     ) == 2.0 * bmath_vf3_s_f3_sum( v1 ) );
        ASSERT( bmath_vf3_s_f3_dot_prd( v1, v2 ) == 2.0 * bmath_vf3_s_f3_sqr( v1 ) );

        f3_t v = 0;
        bmath_vector_aware_dot_prd( &v, v1, v2 );
        ASSERT( bmath_vf3_s_f3_dot_prd( v1, v2 ) == v );
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
            bmath_cf3_s_add_sqr( &sqr1, &sqr1, &v1->data[ i ] );
            bmath_cf3_s_add( &sum1, &sum1, &v1->data[ i ] );
        }

//        bcore_txt_ml_to_stdout( sr_twc( TYPEOF_bmath_cf3_s, &sum1 ) );
//        bcore_txt_ml_to_stdout( sr_twc( TYPEOF_bmath_cf3_s, &sqr1 ) );

        bmath_cf3_s sum2 = bmath_cf3_zro();
        bmath_cf3_s sqr2 = bmath_cf3_zro();
        bmath_vcf3_s_sum( &sum2, v1 );
        bmath_vcf3_s_sqr( &sqr2, v1 );

        ASSERT( bmath_cf3_equ( sum1, sum2 ) );
        ASSERT( bmath_cf3_equ( sqr1, sqr2 ) );

        bmath_vcf3_s* v2 = bcore_life_s_push_aware( l, bmath_vcf3_s_create_size( size ) );
        bmath_vcf3_s_cpy( v2, v1 );
        bmath_vcf3_s_sum( &sum1, v1 );
        bmath_vcf3_s_sum( &sum2, v2 );
        ASSERT( bmath_cf3_equ( sum1, sum2 ) );

        bmath_vcf3_s_neg( v2, v1 );
        bmath_vcf3_s_sum( &sum1, v1 );
        bmath_vcf3_s_sum( &sum2, v2 );
        ASSERT( bmath_cf3_equ( sum1, bmath_cf3_neg( sum2 ) ) );

        bmath_vcf3_s_mul_f3( v2, v1, 2.0 );
        bmath_vcf3_s_sum( &sum1, v1 );
        bmath_vcf3_s_sum( &sum2, v2 );
        ASSERT( bmath_cf3_equ( sum2, bmath_cf3_mul_f3( sum1, 2.0 ) ) );
        bmath_vcf3_s_sqr(     &sqr1, v1 );
        bmath_vcf3_s_dot_prd( &sqr2, v1, v2 );
        ASSERT( bmath_cf3_equ( sqr2, bmath_cf3_mul_f3( sqr1, 2.0 ) ) );
        bmath_vector_aware_dot_prd( &sqr2, v1, v2 );
        ASSERT( bmath_cf3_equ( sqr2, bmath_cf3_mul_f3( sqr1, 2.0 ) ) );

        bmath_vcf3_s_dft( v2, v1 );
        bmath_vcf3_s_ift( v2, v2 );
        bmath_vcf3_s_sqr_sub( &sqr1, v1, v2 );
        ASSERT( bmath_cf3_mag( sqr1 ) < 1E-14 );
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
            BCORE_REGISTER_FFUNC( bmath_fp_vector_dot_prd, bmath_vf3_s_dot_prd );
            BCORE_REGISTER_FFUNC( bmath_fp_vector_mul,     bmath_vf3_s_mul );
            BCORE_REGISTER_OBJECT( bmath_vf3_s );

            BCORE_REGISTER_FFUNC( bmath_fp_add,            bmath_vcf3_s_add );
            BCORE_REGISTER_FFUNC( bmath_fp_zro,            bmath_vcf3_s_zro );
            BCORE_REGISTER_FFUNC( bmath_fp_neg,            bmath_vcf3_s_neg );
            BCORE_REGISTER_FFUNC( bmath_fp_sub,            bmath_vcf3_s_sub );
            BCORE_REGISTER_FFUNC( bmath_fp_cpy,            bmath_vcf3_s_cpy );
            BCORE_REGISTER_FFUNC( bmath_fp_vector_dot_prd, bmath_vcf3_s_dot_prd );
            BCORE_REGISTER_FFUNC( bmath_fp_vector_mul,     bmath_vcf3_s_mul );
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

