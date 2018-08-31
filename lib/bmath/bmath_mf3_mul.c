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

/** Collection of algorithms related to matrix multiplication.
 *  All routines have been designed from scratch and optimized with modern architectures in mind.
 *
 *  This header is included in bmath_mf3.h and should not be used in isolation
 *  See bmath_mf3.h for nomenclature.
 */

#include "bmath_mf3.h"
#include "bmath_mf3_mul.h"

/**********************************************************************************************************************/

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res )
{
    if( res == o )
    {
        bmath_mf3_s* buf = bmath_mf3_s_create();
        bmath_mf3_s_set_size( buf, res->rows, res->cols );
        bmath_mf3_s_mul( o, op, buf );
        bmath_mf3_s_cpy( buf, res );
        bmath_mf3_s_discard( buf );
        return;
    }

    // res == op allowed at this point

    ASSERT(  o->cols ==  op->rows );
    ASSERT(  o->rows == res->rows );
    ASSERT( op->cols == res->cols );

    bmath_vf3_s v;
    bmath_vf3_s_init( &v );
    bmath_vf3_s_set_size( &v, op->rows );
    for( uz_t j = 0; j < op->cols; j++ )
    {
        for( uz_t i = 0; i < op->rows; i++ ) v.data[ i ] = op->data[ i * op->stride + j ];
        for( uz_t i = 0; i <  o->rows; i++ )
        {
            res->data[ i * res->stride + j ] = bmath_f3_t_vec_mul_vec( o->data + i * o->stride, v.data, v.size );
        }
    }

    bmath_vf3_s_down( &v );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul_esp( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res )
{
    if( res == o )
    {
        bmath_mf3_s* buf = bmath_mf3_s_create();
        bmath_mf3_s_set_size( buf, res->rows, res->cols );
        bmath_mf3_s_mul_esp( o, op, buf );
        bmath_mf3_s_cpy( buf, res );
        bmath_mf3_s_discard( buf );
        return;
    }

    // res == op allowed at this point

    ASSERT(  o->cols ==  op->rows );
    ASSERT(  o->rows == res->rows );
    ASSERT( op->cols == res->cols );

    bmath_vf3_s v;
    bmath_vf3_s_init( &v );
    bmath_vf3_s_set_size( &v, op->rows );
    for( uz_t j = 0; j < op->cols; j++ )
    {
        for( uz_t i = 0; i < op->rows; i++ ) v.data[ i ] = op->data[ i * op->stride + j ];
        for( uz_t i = 0; i <  o->rows; i++ )
        {
            res->data[ i * res->stride + j ] = bmath_f3_t_vec_mul_vec_esp( o->data + i * o->stride, v.data, v.size );
        }
    }

    bmath_vf3_s_down( &v );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul_htp( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res )
{
    ASSERT( o->cols  == op->cols );
    ASSERT( o->rows  == res->rows );
    ASSERT( op->rows == res->cols );

    if( o == op ) // result is symmetric -> we can save half of the work
    {
        if( res == o )
        {
            bmath_mf3_s* buf = bmath_mf3_s_create();
            bmath_mf3_s_set_size( buf, res->rows, res->cols );
            bmath_mf3_s_mul_htp( o, op, buf );
            bmath_mf3_s_cpy( buf, res );
            bmath_mf3_s_discard( buf );
        }
        else
        {
            for( uz_t i = 0; i < o->rows; i++ )
            {
                const f3_t* vi = o->data + i * o->stride;
                for( uz_t j = 0; j <= i ; j++ )
                {
                    f3_t sum = bmath_f3_t_vec_mul_vec( vi, o->data + j * o->stride, o->cols );
                    res->data[ i * res->stride + j ] = sum;
                    res->data[ j * res->stride + i ] = sum;
                }
            }
        }
        return;
    }

    if( res == o )
    {
        bmath_vf3_s row;
        bmath_vf3_s_init( &row );
        bmath_vf3_s_set_size( &row, o->cols );
        for( uz_t i = 0; i < o->rows; i++ )
        {
            const f3_t* voi = o->data + i *   o->stride;
            bmath_vf3_s_zro( &row );
            for( uz_t j = 0; j < op->rows; j++ )
            {
                row.data[ j ] = bmath_f3_t_vec_mul_vec( voi, op->data + j * op->stride, o->cols );
            }

            f3_t* vri = res->data + i * res->stride;
            for( uz_t k = 0; k < row.size; k++ ) vri[ k ] = row.data[ k ];
        }
        bmath_vf3_s_down( &row );
    }
    else if( res == op )
    {
        bmath_mf3_s* buf = bmath_mf3_s_create();
        bmath_mf3_s_set_size( buf, res->rows, res->cols );
        bmath_mf3_s_mul_htp( o, op, buf );
        bmath_mf3_s_cpy( buf, res );
        bmath_mf3_s_discard( buf );
    }
    else
    {
        bmath_mf3_s_zro( res );
        for( uz_t i = 0; i < o->rows; i++ )
        {
                  f3_t* vri = res->data + i * res->stride;
            const f3_t* voi =   o->data + i *   o->stride;
            for( uz_t j = 0; j < op->rows; j++ )
            {
                vri[ j ] = bmath_f3_t_vec_mul_vec( voi, op->data + j * op->stride, o->cols );
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul_htp_esp( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res )
{
    ASSERT( o->cols  == op->cols );
    ASSERT( o->rows  == res->rows );
    ASSERT( op->rows == res->cols );

    if( o == op ) // result is symmetric -> we can save half of the work
    {
        if( res == o )
        {
            bmath_mf3_s* buf = bmath_mf3_s_create();
            bmath_mf3_s_set_size( buf, res->rows, res->cols );
            bmath_mf3_s_mul_htp_esp( o, op, buf );
            bmath_mf3_s_cpy( buf, res );
            bmath_mf3_s_discard( buf );
        }
        else
        {
            for( uz_t i = 0; i < o->rows; i++ )
            {
                const f3_t* vi = o->data + i * o->stride;
                for( uz_t j = 0; j <= i ; j++ )
                {
                    f3_t sum = bmath_f3_t_vec_mul_vec_esp( vi, o->data + j * o->stride, o->cols );
                    res->data[ i * res->stride + j ] = sum;
                    res->data[ j * res->stride + i ] = sum;
                }
            }
        }
        return;
    }

    if( res == o )
    {
        bmath_vf3_s row;
        bmath_vf3_s_init( &row );
        bmath_vf3_s_set_size( &row, o->cols );
        for( uz_t i = 0; i < o->rows; i++ )
        {
            const f3_t* voi = o->data + i *   o->stride;
            bmath_vf3_s_zro( &row );
            for( uz_t j = 0; j < op->rows; j++ )
            {
                row.data[ j ] = bmath_f3_t_vec_mul_vec_esp( voi, op->data + j * op->stride, o->cols );
            }

            f3_t* vri = res->data + i * res->stride;
            for( uz_t k = 0; k < row.size; k++ ) vri[ k ] = row.data[ k ];
        }
        bmath_vf3_s_down( &row );
    }
    else if( res == op )
    {
        bmath_mf3_s* buf = bmath_mf3_s_create();
        bmath_mf3_s_set_size( buf, res->rows, res->cols );
        bmath_mf3_s_mul_htp_esp( o, op, buf );
        bmath_mf3_s_cpy( buf, res );
        bmath_mf3_s_discard( buf );
    }
    else
    {
        bmath_mf3_s_zro( res );
        for( uz_t i = 0; i < o->rows; i++ )
        {
                  f3_t* vri = res->data + i * res->stride;
            const f3_t* voi =   o->data + i *   o->stride;
            for( uz_t j = 0; j < op->rows; j++ )
            {
                vri[ j ] = bmath_f3_t_vec_mul_vec_esp( voi, op->data + j * op->stride, o->cols );
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul_dag( const bmath_mf3_s* o, const bmath_vf3_s* d, bmath_mf3_s* res )
{
    ASSERT( o->cols == o->rows   );
    ASSERT( o->rows == d->size );
    ASSERT( o->rows == res->rows );
    ASSERT( o->cols == res->cols );
    const f3_t* vd = d->data;
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* vo =   o->data + i *   o->stride;
              f3_t* vr = res->data + i * res->stride;
        for( uz_t j = 0; j < o->cols; j++ ) vr[ j ] = vo[ j ] * vd[ j ];
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_dag_mul( const bmath_vf3_s* d, const bmath_mf3_s* o, bmath_mf3_s* res )
{
    ASSERT( o->cols == o->rows   );
    ASSERT( o->rows == d->size );
    ASSERT( o->rows == res->rows );
    ASSERT( o->cols == res->cols );
    const f3_t* vd = d->data;
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* vo =   o->data + i *   o->stride;
              f3_t* vr = res->data + i * res->stride;
              f3_t d = vd[ i ];
        for( uz_t j = 0; j < o->cols; j++ ) vr[ j ] = vo[ j ] * d;
    }
}

//---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

