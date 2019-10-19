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

/**********************************************************************************************************************/

/** Collection of algorithms related to matrix multiplication.
 *  All routines have been designed from scratch and optimized with modern architectures in mind.
 *
 *  Blocking and microkernel strategy:
 *    - Recursion rows-cols trying to reach BLOCK_SIZE
 *    - Microkernels are optimized for BLOCK_SIZE and use AVX-SIMD when available
 */

/**********************************************************************************************************************/

#include "bmath_template_fx_begin.h"

/**********************************************************************************************************************/

/**********************************************************************************************************************/
// other mul functions

//----------------------------------------------------------------------------------------------------------------------

void bmath_mfx_s_(mul_esp)( const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res )
{
    if( res == o )
    {
        bmath_mfx_s* buf = bmath_mfx_s_(create)();
        bmath_mfx_s_(set_size)( buf, res->rows, res->cols );
        bmath_mfx_s_(mul_esp)( o, op, buf );
        bmath_mfx_s_(cpy)( buf, res );
        bmath_mfx_s_(discard)( buf );
        return;
    }

    // res == op allowed at this point

    ASSERT(  o->cols ==  op->rows );
    ASSERT(  o->rows == res->rows );
    ASSERT( op->cols == res->cols );

    bmath_vfx_s v;
    bmath_vfx_s_(init)( &v );
    bmath_vfx_s_(set_size)( &v, op->rows );
    for( uz_t j = 0; j < op->cols; j++ )
    {
        for( uz_t i = 0; i < op->rows; i++ ) v.data[ i ] = op->data[ i * op->stride + j ];
        for( uz_t i = 0; i <  o->rows; i++ )
        {
            res->data[ i * res->stride + j ] = bmath_fx_t_vec_(mul_vec_esp)( o->data + i * o->stride, v.data, v.size );
        }
    }

    bmath_vfx_s_(down)( &v );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mfx_s_(mul_htp_esp)( const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res )
{
    ASSERT( o->cols  == op->cols );
    ASSERT( o->rows  == res->rows );
    ASSERT( op->rows == res->cols );

    if( o == op ) // result is symmetric -> we can save half of the work
    {
        if( res == o )
        {
            bmath_mfx_s* buf = bmath_mfx_s_(create)();
            bmath_mfx_s_(set_size)( buf, res->rows, res->cols );
            bmath_mfx_s_(mul_htp_esp)( o, op, buf );
            bmath_mfx_s_(cpy)( buf, res );
            bmath_mfx_s_(discard)( buf );
        }
        else
        {
            for( uz_t i = 0; i < o->rows; i++ )
            {
                const fx_t* vi = o->data + i * o->stride;
                for( uz_t j = 0; j <= i ; j++ )
                {
                    fx_t sum = bmath_fx_t_vec_(mul_vec_esp)( vi, o->data + j * o->stride, o->cols );
                    res->data[ i * res->stride + j ] = sum;
                    res->data[ j * res->stride + i ] = sum;
                }
            }
        }
        return;
    }

    if( res == o )
    {
        bmath_vfx_s row;
        bmath_vfx_s_(init)( &row );
        bmath_vfx_s_(set_size)( &row, o->cols );
        for( uz_t i = 0; i < o->rows; i++ )
        {
            const fx_t* voi = o->data + i *   o->stride;
            bmath_vfx_s_(zro)( &row );
            for( uz_t j = 0; j < op->rows; j++ )
            {
                row.data[ j ] = bmath_fx_t_vec_(mul_vec_esp)( voi, op->data + j * op->stride, o->cols );
            }

            fx_t* vri = res->data + i * res->stride;
            for( uz_t k = 0; k < row.size; k++ ) vri[ k ] = row.data[ k ];
        }
        bmath_vfx_s_(down)( &row );
    }
    else if( res == op )
    {
        bmath_mfx_s* buf = bmath_mfx_s_(create)();
        bmath_mfx_s_(set_size)( buf, res->rows, res->cols );
        bmath_mfx_s_(mul_htp_esp)( o, op, buf );
        bmath_mfx_s_(cpy)( buf, res );
        bmath_mfx_s_(discard)( buf );
    }
    else
    {
        bmath_mfx_s_(zro)( res );
        for( uz_t i = 0; i < o->rows; i++ )
        {
                  fx_t* vri = res->data + i * res->stride;
            const fx_t* voi =   o->data + i *   o->stride;
            for( uz_t j = 0; j < op->rows; j++ )
            {
                vri[ j ] = bmath_fx_t_vec_(mul_vec_esp)( voi, op->data + j * op->stride, o->cols );
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mfx_s_(htp_mul_esp)( const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res )
{
    ASSERT( o->rows  == op->rows );
    ASSERT( o->cols  == res->rows );
    ASSERT( op->cols == res->cols );

    if( o == op ) // result is symmetric -> we can save half of the work
    {
        if( res == o )
        {
            bmath_mfx_s* buf = bmath_mfx_s_(create)();
            bmath_mfx_s_(set_size)( buf, res->rows, res->cols );
            bmath_mfx_s_(htp_mul_esp)( o, op, buf );
            bmath_mfx_s_(cpy)( buf, res );
            bmath_mfx_s_(discard)( buf );
        }
        else
        {
            for( uz_t i = 0; i < o->cols; i++ )
            {
                const fx_t* vi = o->data + i;
                for( uz_t j = 0; j <= i ; j++ )
                {
                    fx_t sum = bmath_fx_t_vec_(mul_vec_esp_stride)( vi, o->stride, o->data + j, o->stride, o->rows );
                    res->data[ i * res->stride + j ] = sum;
                    res->data[ j * res->stride + i ] = sum;
                }
            }
        }
        return;
    }

    if( res == o || res == op )
    {
        bmath_mfx_s* buf = bmath_mfx_s_(create)();
        bmath_mfx_s_(set_size)( buf, res->rows, res->cols );
        bmath_mfx_s_(mul_htp_esp)( o, op, buf );
        bmath_mfx_s_(cpy)( buf, res );
        bmath_mfx_s_(discard)( buf );
    }
    else
    {
        bmath_mfx_s_(zro)( res );
        for( uz_t i = 0; i < o->cols; i++ )
        {
                  fx_t* vri = res->data + i * res->stride;
            const fx_t* voi =   o->data + i;
            for( uz_t j = 0; j < op->cols; j++ )
            {
                vri[ j ] = bmath_fx_t_vec_(mul_vec_esp_stride)( voi, o->stride, op->data + j, op->stride, o->rows );
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void bmath_mfx_s_(mul_udu_htp_)( const bmath_mfx_s* u, const bmath_vfx_s* d, bl_t esp, bmath_mfx_s* r )
{
    ASSERT( r != u );
    ASSERT( u->rows == r->cols );
    ASSERT( u->rows == r->rows );
    ASSERT( u->cols == d->size );

    bmath_vfx_s* x = bmath_vfx_s_(create)();
    bmath_vfx_s_(set_size)( x, d->size );

    const fx_t* vd = d->data;
          fx_t* vx = x->data;
    for( sz_t i = 0; i < u->rows; i++ )
    {
        const fx_t* ui = u->data + i * u->stride;
              fx_t* ri = r->data + i * r->stride;
        for( sz_t j = 0; j < d->size; j++ ) vx[ j ] = ui[ j ] * vd[ j ];
        for( sz_t j = 0; j < u->rows; j++ )
        {
            const fx_t* uj = u->data + j * u->stride;
            ri[ j ] = esp ? bmath_fx_t_vec_(mul_vec_esp)( vx, uj, d->size ) :
                            bmath_fx_t_vec_(mul_vec)(     vx, uj, d->size );
        }
    }

    bmath_vfx_s_(discard)( x );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mfx_s_(mul_udu_htp)( const bmath_mfx_s* u, const bmath_vfx_s* d, bmath_mfx_s* r )
{
    bmath_mfx_s_(mul_udu_htp_)( u, d, false, r );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mfx_s_(mul_udu_htp_esp)( const bmath_mfx_s* u, const bmath_vfx_s* d, bmath_mfx_s* r )
{
    bmath_mfx_s_(mul_udu_htp_)( u, d, true, r );
}

//----------------------------------------------------------------------------------------------------------------------

static void bmath_mfx_s_(mul_udv_htp_)( const bmath_mfx_s* u, const bmath_vfx_s* d, const bmath_mfx_s* v, bl_t esp, bmath_mfx_s* r )
{
    ASSERT( r != u );
    ASSERT( r != v );
    ASSERT( r->rows == u->rows );
    ASSERT( r->cols == v->rows );
    ASSERT( u->cols >= d->size );
    ASSERT( v->cols >= d->size );

    bmath_vfx_s* x = bmath_vfx_s_(create)();
    bmath_vfx_s_(set_size)( x, d->size );

    const fx_t* vd = d->data;
          fx_t* vx = x->data;
    for( sz_t i = 0; i < u->rows; i++ )
    {
        const fx_t* ui = u->data + i * u->stride;
              fx_t* ri = r->data + i * r->stride;

        for( sz_t j = 0; j < d->size; j++ ) vx[ j ] = ui[ j ] * vd[ j ];

        for( sz_t j = 0; j < v->rows; j++ )
        {
            const fx_t* vj = v->data + j * v->stride;
            ri[ j ] = esp ? bmath_fx_t_vec_(mul_vec_esp)( vx, vj, d->size ) :
                            bmath_fx_t_vec_(mul_vec)(     vx, vj, d->size );
        }
    }

    bmath_vfx_s_(discard)( x );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mfx_s_(mul_udv_htp)( const bmath_mfx_s* u, const bmath_vfx_s* d, const bmath_mfx_s* v, bmath_mfx_s* r )
{
    bmath_mfx_s_(mul_udv_htp_)( u, d, v, false, r );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mfx_s_(mul_udv_htp_esp)( const bmath_mfx_s* u, const bmath_vfx_s* d, const bmath_mfx_s* v, bmath_mfx_s* r )
{
    bmath_mfx_s_(mul_udv_htp_)( u, d, v, true, r );
}

//----------------------------------------------------------------------------------------------------------------------

static void bmath_mfx_s_(mul_utv_htp_)( const bmath_mfx_s* u, const bmath_mfx_s* t, const bmath_mfx_s* v, bl_t esp, bmath_mfx_s* r )
{
    ASSERT( r != u );
    ASSERT( r != v );
    ASSERT( u->rows == r->rows );
    ASSERT( v->rows == r->cols );
    ASSERT( u->cols == t->rows );
    ASSERT( t->cols == v->cols );

    if( bmath_mfx_s_(is_dag)( t ) )
    {
        bmath_vfx_s* d = bmath_vfx_s_(create)();
        bmath_vfx_s_(set_size)( d, sz_min( t->rows, t->cols ) );
        bmath_mfx_s_(get_dag_vec)( t, d );
        bmath_mfx_s_(mul_udv_htp_)( u, d, v, esp, r );
        bmath_vfx_s_(discard)( d );
        return;
    }

    bmath_vfx_s* dm = bmath_vfx_s_(create)(); // main  diag
    bmath_vfx_s* du = bmath_vfx_s_(create)(); // upper diag (left aligned)
    bmath_vfx_s* dl = bmath_vfx_s_(create)(); // lower diag (left aligned)
    bmath_vfx_s* x  = bmath_vfx_s_(create)(); // temp vector

    bmath_vfx_s_(set_size)( dm, sz_min( t->rows, t->cols ) );

    if( dm->size > 0 )
    {
        bmath_vfx_s_(set_size)( du, ( ( t->cols > t->rows ) ? dm->size : dm->size - 1 ) );
        bmath_vfx_s_(set_size)( dl, ( ( t->rows > t->cols ) ? dm->size : dm->size - 1 ) );
        bmath_vfx_s_(set_size)( x,  dm->size );

        for( sz_t i = 0; i < dm->size - 1; i++ )
        {
            dm->data[ i ] = t->data[   i       * t->stride +   i       ];
            dl->data[ i ] = t->data[ ( i + 1 ) * t->stride +   i       ];
            du->data[ i ] = t->data[   i       * t->stride + ( i + 1 ) ];
        }

                                   dm->data[ dm->size - 1 ] = t->data[ ( dm->size - 1 ) * t->stride + ( dm->size - 1 ) ];
        if( du->size == dm->size ) du->data[ dm->size - 1 ] = t->data[ ( dm->size - 1 ) * t->stride +   dm->size       ];
        if( dl->size == dm->size ) dl->data[ dm->size - 1 ] = t->data[   dm->size       * t->stride + ( dm->size - 1 ) ];
    }

    const fx_t* vdm = dm->data;
    const fx_t* vdu = du->data;
    const fx_t* vdl = dl->data;
          fx_t* vx  =  x->data;

    for( uz_t i = 0; i < u->rows; i++ )
    {
        const fx_t* ui = u->data + i * u->stride;
              fx_t* ri = r->data + i * r->stride;

        for( sz_t j = 0; j < dm->size; j++ ) vx[ j     ]  = ui[ j     ] * vdm[ j ];
        for( sz_t j = 0; j < du->size; j++ ) vx[ j + 1 ] += ui[ j     ] * vdu[ j ];
        for( sz_t j = 0; j < dl->size; j++ ) vx[ j     ] += ui[ j + 1 ] * vdl[ j ];

        for( sz_t j = 0; j < v->rows; j++ )
        {
            const fx_t* vj = v->data + j * v->stride;
            ri[ j ] = esp ? bmath_fx_t_vec_(mul_vec_esp)( vx, vj, dm->size ) :
                            bmath_fx_t_vec_(mul_vec)(     vx, vj, dm->size );
        }
    }

    bmath_vfx_s_(discard)( dm );
    bmath_vfx_s_(discard)( du );
    bmath_vfx_s_(discard)( dl );

    bmath_vfx_s_(discard)( x );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mfx_s_(mul_utv_htp)( const bmath_mfx_s* u, const bmath_mfx_s* t, const bmath_mfx_s* v, bmath_mfx_s* r )
{
    bmath_mfx_s_(mul_utv_htp_)( u, t, v, false, r );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mfx_s_(mul_utv_htp_esp)( const bmath_mfx_s* u, const bmath_mfx_s* t, const bmath_mfx_s* v, bmath_mfx_s* r )
{
    bmath_mfx_s_(mul_utv_htp_)( u, t, v, false, r );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mfx_s_(mul_add)( const bmath_mfx_s* o, const bmath_mfx_s* b, const bmath_mfx_s* c, bmath_mfx_s* r )
{
    if( r == o || r == b )
    {
        bmath_mfx_s* buf = bmath_mfx_s_(create)();
        bmath_mfx_s_(set_size)( buf, r->rows, r->cols );
        bmath_mfx_s_(mul_add)( o, b, c, buf );
        bmath_mfx_s_(cpy)( buf, r );
        bmath_mfx_s_(discard)( buf );
        return;
    }

    ASSERT( o->cols == b->rows );
    ASSERT( o->rows == r->rows );
    ASSERT( b->cols == r->cols );
    ASSERT( c->rows == r->rows );
    ASSERT( c->cols == r->cols );

    if( c != r ) bmath_mfx_s_(cpy)( c, r );

    bmath_mfx_s_(fx_t_mul)( o->data, o->stride, o->rows, o->cols, b->data, b->stride, b->cols, r->data, r->stride );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mfx_s_(mul_htp_add)( const bmath_mfx_s* o, const bmath_mfx_s* b, const bmath_mfx_s* c, bmath_mfx_s* r )
{
    if( r == o || r == b )
    {
        bmath_mfx_s* buf = bmath_mfx_s_(create)();
        bmath_mfx_s_(set_size)( buf, r->rows, r->cols );
        bmath_mfx_s_(mul_htp_add)( o, b, c, buf );
        bmath_mfx_s_(cpy)( buf, r );
        bmath_mfx_s_(discard)( buf );
        return;
    }

    ASSERT( o->cols == b->cols );
    ASSERT( o->rows == r->rows );
    ASSERT( b->rows == r->cols );
    ASSERT( c->rows == r->rows );
    ASSERT( c->cols == r->cols );

    if( c != r ) bmath_mfx_s_(cpy)( c, r );

    bmath_mfx_s_(fx_t_mul_htp)( o->data, o->stride, o->rows, o->cols, b->data, b->stride, b->rows, r->data, r->stride, false );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mfx_s_(htp_mul_add)( const bmath_mfx_s* o, const bmath_mfx_s* b, const bmath_mfx_s* c, bmath_mfx_s* r )
{
    if( r == o || r == b )
    {
        bmath_mfx_s* buf = bmath_mfx_s_(create)();
        bmath_mfx_s_(set_size)( buf, r->rows, r->cols );
        bmath_mfx_s_(htp_mul_add)( o, b, c, buf );
        bmath_mfx_s_(cpy)( buf, r );
        bmath_mfx_s_(discard)( buf );
        return;
    }

    ASSERT( o->rows == b->rows );
    ASSERT( o->cols == r->rows );
    ASSERT( b->cols == r->cols );
    ASSERT( c->rows == r->rows );
    ASSERT( c->cols == r->cols );

    if( c != r ) bmath_mfx_s_(cpy)( c, r );

    bmath_mfx_s_(fx_t_htp_mul)( o->data, o->stride, o->rows, o->cols, b->data, b->stride, b->cols, r->data, r->stride, false );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mfx_s_(htp_mul_htp_add)( const bmath_mfx_s* o, const bmath_mfx_s* b, const bmath_mfx_s* c, bmath_mfx_s* r )
{
    if( r == o || r == b )
    {
        bmath_mfx_s* buf = bmath_mfx_s_(create)();
        bmath_mfx_s_(set_size)( buf, r->rows, r->cols );
        bmath_mfx_s_(htp_mul_htp_add)( o, b, c, buf );
        bmath_mfx_s_(cpy)( buf, r );
        bmath_mfx_s_(discard)( buf );
        return;
    }

    ASSERT( o->rows == b->cols );
    ASSERT( o->cols == r->rows );
    ASSERT( b->rows == r->cols );
    ASSERT( c->rows == r->rows );
    ASSERT( c->cols == r->cols );

    if( c != r ) bmath_mfx_s_(cpy)( c, r );

    bmath_mfx_s_(fx_t_htp_mul_htp)( o->data, o->stride, o->rows, o->cols, b->data, b->stride, b->rows, r->data, r->stride );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mfx_s_(mul_add_cps)( bl_t htpa, const bmath_mfx_s* a, bl_t htpb, const bmath_mfx_s* b, fx_t c, const bmath_mfx_s* d, fx_t e, bmath_mfx_s* r )
{
    if( r == a || r == b )
    {
        ASSERT( !bmath_mfx_s_(is_folded)( r ) );
        bmath_mfx_s* buf = bmath_mfx_s_(create)();
        bmath_mfx_s_(set_size)( buf, r->rows, r->cols );
        bmath_mfx_s_(mul_add_cps)( htpa, a, htpb, b, c, d, e, buf );
        bmath_mfx_s_(cpy)( buf, r );
        bmath_mfx_s_(discard)( buf );
        return;
    }

    ASSERT( ( htpa ? a->rows : a->cols ) == ( htpb ? b->cols : b->rows ) );
    ASSERT( ( htpa ? a->cols : a->rows ) == r->rows );
    ASSERT( ( htpb ? b->rows : b->cols ) == r->cols );

    if( d )
    {
        ASSERT( d->rows == r->rows );
        ASSERT( d->cols == r->cols );
        if( c != 0 )
        {
            if( c == e )
            {
                if( d != r ) bmath_mfx_s_(cpy)( d, r );
            }
            else
            {
                bmath_mfx_s_(mul_scl_fx)( d, e / c, r );
            }
        }
        else
        {
            if( e == 1.0 )
            {
                if( d != r ) bmath_mfx_s_(cpy)( d, r );
            }
            else
            {
                bmath_mfx_s_(mul_scl_fx)( d, e, r );
            }
        }
    }
    else
    {
        bmath_mfx_s_(zro)( r );
    }

    if( c != 0 )
    {
        bl_t symmetry = ( !d || e == 0.0 ) && ( a == b ) && ( htpa + htpb == 1 );
        switch( htpa * 2 + htpb )
        {
            case 0: bmath_mfx_s_(fx_t_mul)(         a->data, a->stride, a->rows, a->cols, b->data, b->stride, b->cols, r->data, r->stride           ); break; // a * b
            case 1: bmath_mfx_s_(fx_t_mul_htp)(     a->data, a->stride, a->rows, a->cols, b->data, b->stride, b->rows, r->data, r->stride, symmetry ); break; // a * b^T
            case 2: bmath_mfx_s_(fx_t_htp_mul)(     a->data, a->stride, a->rows, a->cols, b->data, b->stride, b->cols, r->data, r->stride, symmetry ); break; // a^T * b
            case 3: bmath_mfx_s_(fx_t_htp_mul_htp)( a->data, a->stride, a->rows, a->cols, b->data, b->stride, b->rows, r->data, r->stride           ); break; // a^T * b^T
            default: break;
        }
        if( symmetry )
        {
            ASSERT( !bmath_mfx_s_(is_folded)( r ) );
            for( sz_t i = 0; i < r->rows; i++ )
            {
                for( sz_t j = 0; j < i; j++ ) r->data[ j * r->stride + i ] = r->data[ i * r->stride + j ];
            }
        }
        if( c != 1.0 ) bmath_mfx_s_(mul_scl_fx)( r, c, r );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mfx_s_(mul_add_cps_selftest)()
{
    BLM_INIT();

    bmath_mfx_s* a  = BLM_A_PUSH( bmath_mfx_s_(create)() );
    bmath_mfx_s* b  = BLM_A_PUSH( bmath_mfx_s_(create)() );
    bmath_mfx_s* ab = BLM_A_PUSH( bmath_mfx_s_(create)() );
    bmath_mfx_s* d  = BLM_A_PUSH( bmath_mfx_s_(create)() );
    bmath_mfx_s* r1 = BLM_A_PUSH( bmath_mfx_s_(create)() );
    bmath_mfx_s* r2 = BLM_A_PUSH( bmath_mfx_s_(create)() );

    sz_t n  = 25;
    sz_t m1 = 37;
    sz_t m2 = 15;

    fx_t c = 0.5;
    fx_t e = 0.3;

    // a * b
    {
        bmath_mfx_s_(set_size)(  a, m1, n  );
        bmath_mfx_s_(set_size)(  b, n,  m2 );
        bmath_mfx_s_(set_size)(  d, m1, m2 );
        u2_t rval = 1236;
        bmath_mfx_s_(set_random)( a, false, false, 0, 1.0, -1, 1, &rval );
        bmath_mfx_s_(set_random)( b, false, false, 0, 1.0, -1, 1, &rval );
        bmath_mfx_s_(set_random)( d, false, false, 0, 1.0, -1, 1, &rval );
        bmath_mfx_s_(set_size)( ab, m1, m2 );
        bmath_mfx_s_(set_size)( r1, m1, m2 );
        bmath_mfx_s_(set_size)( r2, m1, m2 );
        bmath_mfx_s_(mul_add_cps)( false, a, false, b, c, d, e, r1 );
        bmath_mfx_s_(mul_esp)( a, b, ab );
        bmath_mfx_s_(mul_scl_fx)( ab, c, ab );
        bmath_mfx_s_(mul_scl_fx)( d,  e, r2 );
        bmath_mfx_s_(add)( ab, r2, r2 );
        ASSERT( bmath_mfx_s_(is_near_equ)( r1, r2, 1E-10 ) );
    }

    // a^T * b
    {
        bmath_mfx_s_(set_size)(  a, n,  m1  );
        bmath_mfx_s_(set_size)(  b, n,  m2 );
        bmath_mfx_s_(set_size)(  d, m1, m2 );
        u2_t rval = 1236;
        bmath_mfx_s_(set_random)( a, false, false, 0, 1.0, -1, 1, &rval );
        bmath_mfx_s_(set_random)( b, false, false, 0, 1.0, -1, 1, &rval );
        bmath_mfx_s_(set_random)( d, false, false, 0, 1.0, -1, 1, &rval );
        bmath_mfx_s_(set_size)( ab, m1, m2 );
        bmath_mfx_s_(set_size)( r1, m1, m2 );
        bmath_mfx_s_(set_size)( r2, m1, m2 );
        bmath_mfx_s_(mul_add_cps)( true, a, false, b, c, d, e, r1 );
        bmath_mfx_s_(htp_mul_esp)( a, b, ab );
        bmath_mfx_s_(mul_scl_fx)( ab, c, ab );
        bmath_mfx_s_(mul_scl_fx)( d,  e, r2 );
        bmath_mfx_s_(add)( ab, r2, r2 );
        ASSERT( bmath_mfx_s_(is_near_equ)( r1, r2, 1E-10 ) );
    }

    // a * b^T
    {
        bmath_mfx_s_(set_size)(  a, m1, n  );
        bmath_mfx_s_(set_size)(  b, m2, n  );
        bmath_mfx_s_(set_size)(  d, m1, m2 );
        u2_t rval = 1236;
        bmath_mfx_s_(set_random)( a, false, false, 0, 1.0, -1, 1, &rval );
        bmath_mfx_s_(set_random)( b, false, false, 0, 1.0, -1, 1, &rval );
        bmath_mfx_s_(set_random)( d, false, false, 0, 1.0, -1, 1, &rval );
        bmath_mfx_s_(set_size)( ab, m1, m2 );
        bmath_mfx_s_(set_size)( r1, m1, m2 );
        bmath_mfx_s_(set_size)( r2, m1, m2 );
        bmath_mfx_s_(mul_add_cps)( false, a, true, b, c, d, e, r1 );
        bmath_mfx_s_(mul_htp_esp)( a, b, ab );
        bmath_mfx_s_(mul_scl_fx)( ab, c, ab );
        bmath_mfx_s_(mul_scl_fx)( d,  e, r2 );
        bmath_mfx_s_(add)( ab, r2, r2 );
        ASSERT( bmath_mfx_s_(is_near_equ)( r1, r2, 1E-10 ) );
    }

    // a^T * b^T
    {
        bmath_mfx_s_(set_size)(  a, n,  m1 );
        bmath_mfx_s_(set_size)(  b, m2, n  );
        bmath_mfx_s_(set_size)(  d, m1, m2 );
        u2_t rval = 1236;
        bmath_mfx_s_(set_random)( a, false, false, 0, 1.0, -1, 1, &rval );
        bmath_mfx_s_(set_random)( b, false, false, 0, 1.0, -1, 1, &rval );
        bmath_mfx_s_(set_random)( d, false, false, 0, 1.0, -1, 1, &rval );
        bmath_mfx_s_(set_size)( ab, m1, m2 );
        bmath_mfx_s_(set_size)( r1, m1, m2 );
        bmath_mfx_s_(set_size)( r2, m1, m2 );
        bmath_mfx_s_(mul_add_cps)( true, a, true, b, c, d, e, r1 );
        bmath_mfx_s_(htp_mul_htp)( a, b, ab );
        bmath_mfx_s_(mul_scl_fx)( ab, c, ab );
        bmath_mfx_s_(mul_scl_fx)( d,  e, r2 );
        bmath_mfx_s_(add)( ab, r2, r2 );
        ASSERT( bmath_mfx_s_(is_near_equ)( r1, r2, 1E-10 ) );
    }

    // a^T * a
    {
        bmath_mfx_s_(set_size)(  a, m1, n );
        u2_t rval = 1236;
        bmath_mfx_s_(set_random)( a, false, false, 0, 1.0, -1, 1, &rval );
        bmath_mfx_s_(set_size)( ab, n, n );
        bmath_mfx_s_(set_size)( r1, n, n );
        bmath_mfx_s_(set_size)( r2, n, n );
        bmath_mfx_s_(mul_add_cps)( true, a, false, a, c, NULL, e, r1 );
        bmath_mfx_s_(htp_mul_esp)( a, a, r2 );
        bmath_mfx_s_(mul_scl_fx)( r2, c, r2 );
        ASSERT( bmath_mfx_s_(is_near_equ)( r1, r2, 1E-10 ) );
    }

    // a * a^T
    {
        bmath_mfx_s_(set_size)(  a, n, m1 );
        u2_t rval = 1236;
        bmath_mfx_s_(set_random)( a, false, false, 0, 1.0, -1, 1, &rval );
        bmath_mfx_s_(set_size)( ab, n, n );
        bmath_mfx_s_(set_size)( r1, n, n );
        bmath_mfx_s_(set_size)( r2, n, n );
        bmath_mfx_s_(mul_add_cps)( false, a, true, a, c, NULL, e, r1 );
        bmath_mfx_s_(mul_htp_esp)( a, a, r2 );
        bmath_mfx_s_(mul_scl_fx)( r2, c, r2 );
        ASSERT( bmath_mfx_s_(is_near_equ)( r1, r2, 1E-10 ) );
    }

    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// closing template

#include "bmath_template_fx_end.h"

/**********************************************************************************************************************/

