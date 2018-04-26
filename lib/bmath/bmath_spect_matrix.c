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

#include "bmath_spect_matrix.h"
#include "bmath_quicktypes.h"

/**********************************************************************************************************************/

BCORE_DEFINE_SPECT( bmath_matrix_s )
"{"
    "bcore_spect_header_s header;"

    "strict feature bcore_array_s  -> spect_array_matrix;  "
    "strict feature bmath_vector_s -> spect_vector_matrix; "
    "strict feature bcore_matrix_s -> spect_matrix_matrix; "
    "strict feature bcore_inst_s   -> spect_inst_matrix;   "
    "strict feature bmath_vector_s -> spect_vector_default ~> const tp_t type_vector_default;"

    "private        bmath_ring_s   -> spect_ring_scalar;   " // ring of matrix element explicitly implemented

    // ring features
    "feature bmath_fp: add;"
    "feature bmath_fp: zro;"
    "feature bmath_fp: neg;"
    "feature bmath_fp: sub;"
    "feature bmath_fp: mul;"
    "feature bmath_fp: one;"
    "feature bmath_fp: inv;"
    "feature bmath_fp: div;"

    // matrix features
    "feature bmath_fp: mul_vec;"
    "feature bmath_fp: mul_scl;"

    // spect functions
    "func bcore_spect_fp: create_from_self;"
"}";

static bmath_matrix_s* bmath_matrix_s_create_from_self( const bcore_self_s* self )
{
    bmath_matrix_s* o = bcore_spect_create_from_self( bcore_flect_get_self( typeof( "bmath_matrix_s" ) ), self );
    if( !o->spect_array_matrix->item_p ) ERR_fa( "Cannot determine item type in object '#<sc_t>'", ifnameof( self->type ) );
    o->spect_ring_scalar = bmath_ring_s_get_typed( o->spect_array_matrix->item_p->o_type );
    return o;
}

/**********************************************************************************************************************/

sz_t bmath_matrix_default_get_rows( const bmath_matrix_s* p, const bmath_matrix* o )
{
    return bcore_matrix_p_get_rows( p->spect_matrix_matrix, ( const bcore_matrix* )o );
}

sz_t bmath_matrix_default_get_cols( const bmath_matrix_s* p, const bmath_matrix* o )
{
    return bcore_matrix_p_get_cols( p->spect_matrix_matrix, ( const bcore_matrix* )o );
}

void bmath_matrix_default_zro( const bmath_matrix_s* p, bmath_matrix* o )
{
    bmath_vector_p_zro( p->spect_vector_matrix, ( bmath_vector* )o );
}

void bmath_matrix_default_neg( const bmath_matrix_s* p, const bmath_matrix* o, bmath_matrix* res )
{
    bmath_vector_p_neg( p->spect_vector_matrix, ( const bmath_vector* )o, ( bmath_vector* )res );
}

void bmath_matrix_default_cpy( const bmath_matrix_s* p, const bmath_matrix* o, bmath_matrix* res )
{
    bmath_vector_p_cpy( p->spect_vector_matrix, ( const bmath_vector* )o, ( bmath_vector* )res );
}

void bmath_matrix_default_add( const bmath_matrix_s* p, const bmath_matrix* o, const bmath_matrix* op, bmath_matrix* res )
{
    bmath_vector_p_add( p->spect_vector_matrix, (const bmath_vector*)o, (bmath_vector*)op, (bmath_vector*)res );
}

void bmath_matrix_default_sub( const bmath_matrix_s* p, const bmath_matrix* o, const bmath_matrix* op, bmath_matrix* res )
{
    bmath_vector_p_sub( p->spect_vector_matrix, (const bmath_vector*)o, (bmath_vector*)op, (bmath_vector*)res );
}

void bmath_matrix_default_mul( const bmath_matrix_s* p, const bmath_matrix* o, const bmath_matrix* op, bmath_matrix* res )
{
    sz_t rows1 = bmath_matrix_p_get_rows( p, o );
    sz_t rows2 = bmath_matrix_p_get_rows( p, op );
    sz_t cols1 = bmath_matrix_p_get_cols( p, o );
    sz_t cols2 = bmath_matrix_p_get_cols( p, op );
    sz_t steps = cols1 < rows2 ? cols1 : rows2;

    sr_s sr_mul = sr_p_create( p->spect_ring_scalar->spect_inst );

    for( sz_t i = 0; i < rows1; i++ )
    {
        for( sz_t j = 0; j < cols2; j++ )
        {
            sr_s sr_sum = sr_p_create( p->spect_ring_scalar->spect_inst );
            bmath_ring_r_zro( &sr_sum );
            for( sz_t k = 0; k < steps; k++ )
            {
                sr_s sr1 = bcore_matrix_p_get_cell( p->spect_matrix_matrix, ( const bcore_matrix* )o,  i, k );
                sr_s sr2 = bcore_matrix_p_get_cell( p->spect_matrix_matrix, ( const bcore_matrix* )op, k, j );
                if ( sr1.o && sr2.o ) bmath_ring_p_mul( p->spect_ring_scalar, sr1.o, sr2.o, sr_mul.o );
                else                  bmath_ring_p_zro( p->spect_ring_scalar, sr_mul.o );
                bmath_ring_p_add( p->spect_ring_scalar, sr_mul.o, sr_sum.o, sr_sum.o );
            }
            bcore_matrix_p_set_cell( p->spect_matrix_matrix, ( bcore_matrix* )res, i, j, sr_sum );
        }
    }

    sr_down( sr_mul );
}

void bmath_matrix_default_one( const bmath_matrix_s* p, bmath_matrix* o )
{
    bmath_matrix_p_zro( p, o );
    sz_t rows = bmath_matrix_p_get_rows( p, o );
    sz_t cols = bmath_matrix_p_get_cols( p, o );
    sz_t steps = rows < cols ? rows : cols;
    sr_s sr_one = sr_p_create( p->spect_ring_scalar->spect_inst );
    bmath_ring_p_one( p->spect_ring_scalar, sr_one.o );

    for( sz_t i = 0; i < steps; i++ )
    {
        bcore_matrix_p_set_cell( p->spect_matrix_matrix, ( bcore_matrix* )o, i, i, sr_cw( sr_one ) );
    }

    sr_down( sr_one );
}

void bmath_matrix_default_div( const bmath_matrix_s* p, const bmath_matrix* o, const bmath_matrix* op, bmath_matrix* res )
{
    sr_s sr_inv = sr_p_create( p );
    bmath_matrix_p_inv( p, op, sr_inv.o );
    bmath_matrix_p_mul( p, o, sr_inv.o, res );
    sr_down( sr_inv );
}

void bmath_matrix_default_mul_scl( const bmath_matrix_s* p, const bmath_matrix* o, const bmath_ring* op, bmath_matrix* res )
{
    bmath_vector_p_mul( p->spect_vector_matrix, (const bmath_vector*)o, op, (bmath_vector*)res );
}

void bmath_matrix_default_mul_vec( const bmath_matrix_s* p, const bmath_matrix* o, const bmath_vector* op, bmath_vector* res )
{
    const bcore_array_s* spect_array_vector = p->spect_vector_default->spect_array_vector;

    sz_t rows1 = bmath_matrix_p_get_rows( p, o );
    sz_t cols1 = bmath_matrix_p_get_cols( p, o );
    sz_t  dim1 = bmath_vector_p_get_dim( p->spect_vector_default, res );
    sz_t  dim2 = bmath_vector_p_get_dim( p->spect_vector_default, op );

    sz_t steps = cols1 < dim2 ? cols1 : dim2;

    sr_s sr_mul = sr_p_create( p->spect_ring_scalar->spect_inst );

    for( sz_t i = 0; i < dim1; i++ )
    {
        sr_s sr_sum = sr_p_create( p->spect_ring_scalar->spect_inst );
        bmath_ring_r_zro( &sr_sum );
        if( i < rows1 )
        {
            for( sz_t k = 0; k < steps; k++ )
            {
                sr_s sr1 = bcore_matrix_p_get_cell( p->spect_matrix_matrix, ( const bcore_matrix* )o, i, k );
                sr_s sr2 = bcore_array_spect_get( spect_array_vector, op, k );
                if ( sr1.o && sr2.o ) bmath_ring_p_mul( p->spect_ring_scalar, sr1.o, sr2.o, sr_mul.o );
                else                  bmath_ring_p_zro( p->spect_ring_scalar, sr_mul.o );
                bmath_ring_p_add( p->spect_ring_scalar, sr_mul.o, sr_sum.o, sr_sum.o );
            }
        }
        bcore_array_spect_set( spect_array_vector, res, i, sr_sum );
    }

    sr_down( sr_mul );
}

/**********************************************************************************************************************/

static vd_t selftest( void )
{
    return NULL;
}

/**********************************************************************************************************************/

vd_t bmath_spect_matrix_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_spect_matrix" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_matrix_mul_vec );
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_matrix_mul_scl );
            BCORE_REGISTER_SPECT( bmath_matrix_s );
            BCORE_REGISTER_FFUNC( bcore_spect_fp_create_from_self, bmath_matrix_s_create_from_self );
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

