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

#define NPX( name ) bmath_matrix_##name

/**********************************************************************************************************************/

static sc_t bmath_matrix_s_def = "bmath_matrix_s = spect"
"{"
    "bcore_spect_header_s header;"

    "strict feature bcore_array_s*  spect_array_matrix;  "
    "strict feature bmath_vector_s* spect_vector_matrix; "
    "strict feature bcore_matrix_s* spect_matrix_matrix; "
    "strict feature bcore_inst_s*   spect_inst_matrix;   "
    "strict feature bmath_vector_s* spect_vector_default ~> const tp_t type_vector_default;"

    "private        bmath_ring_s*   spect_ring_scalar;   " // ring of matrix element

    // ring features
    "feature bmath_fp_add fp_add ~> func bmath_fp_add add;"
    "feature bmath_fp_zro fp_zro ~> func bmath_fp_zro zro;"
    "feature bmath_fp_neg fp_neg ~> func bmath_fp_neg neg;"
    "feature bmath_fp_sub fp_sub ~> func bmath_fp_sub sub;"
    "feature bmath_fp_mul fp_mul ~> func bmath_fp_mul mul;"
    "feature bmath_fp_one fp_one ~> func bmath_fp_one one;"
    "feature bmath_fp_inv fp_inv ~> func bmath_fp_inv inv;"
    "feature bmath_fp_div fp_div ~> func bmath_fp_div div;"

    // matrix features
    "feature bmath_fp_matrix_mul_vec fp_matrix_mul_vec ~> func bmath_fp_matrix_mul_vec matrix_mul_vec;"
    "feature bmath_fp_matrix_mul_scl fp_matrix_mul_scl ~> func bmath_fp_matrix_mul_scl matrix_mul_scl;"
"}";

static bmath_matrix_s* bmath_matrix_s_create_from_self( const bcore_self_s* self )
{
    bmath_matrix_s* o = bcore_spect_create_from_self( bcore_flect_get_self( typeof( "bmath_matrix_s" ) ), self );
    if( !o->spect_array_matrix->item_p ) ERR_fa( "Cannot determine item type in object '#<sc_t>'", ifnameof( self->type ) );
    o->spect_ring_scalar = bmath_ring_s_get_typed( o->spect_array_matrix->item_p->o_type );
    return o;
}

static bcore_self_s* bmath_matrix_s_create_self( void )
{
    bcore_self_s* self = bcore_self_s_build_parse_sc( bmath_matrix_s_def, sizeof( bmath_matrix_s ) );
    bcore_self_s_push_ns_func( self, ( fp_t )bmath_matrix_s_create_from_self, "bcore_spect_fp_create_from_self", "create_from_self" );
    return self;
}

/**********************************************************************************************************************/

sz_t bmath_matrix_spect_get_rows( const bmath_matrix_s* p, vc_t o )
{
    return bcore_matrix_spect_get_rows( p->spect_matrix_matrix, o );
}

sz_t bmath_matrix_spect_get_cols( const bmath_matrix_s* p, vc_t o )
{
    return bcore_matrix_spect_get_cols( p->spect_matrix_matrix, o );
}

void bmath_matrix_spect_zro( const bmath_matrix_s* p, vd_t o )
{
    if( p->fp_zro ) { p->fp_zro( o ); return; }
    bmath_vector_spect_zro( p->spect_vector_matrix, o );
}

void bmath_matrix_spect_neg( const bmath_matrix_s* p, vd_t o, vc_t mat1 )
{
    if( p->fp_neg ) { p->fp_neg( o, mat1 ); return; }
    bmath_vector_spect_neg( p->spect_vector_matrix, o, mat1 );
}

void bmath_matrix_spect_cpy( const bmath_matrix_s* p, vd_t o, vc_t mat1 )
{
    bcore_inst_spect_copy( p->spect_inst_matrix, o, mat1 );
}

void bmath_matrix_spect_add( const bmath_matrix_s* p, vd_t o, vc_t mat1, vc_t mat2 )
{
    if( p->fp_add ) { p->fp_add( o, mat1, mat2 ); return; }
    bmath_vector_spect_add( p->spect_vector_matrix, o, mat1, mat2 );
}

void bmath_matrix_spect_sub( const bmath_matrix_s* p, vd_t o, vc_t mat1, vc_t mat2 )
{
    if( p->fp_sub ) { p->fp_sub( o, mat1, mat2 ); return; }
    bmath_vector_spect_sub( p->spect_vector_matrix, o, mat1, mat2 );
}

void bmath_matrix_spect_mul( const bmath_matrix_s* p, vd_t o, vc_t mat1, vc_t mat2 )
{
    if( p->fp_mul ) { p->fp_mul( o, mat1, mat2 ); return; }
    sz_t rows1 = bmath_matrix_spect_get_rows( p, mat1 );
    sz_t rows2 = bmath_matrix_spect_get_rows( p, mat2 );
    sz_t cols1 = bmath_matrix_spect_get_cols( p, mat1 );
    sz_t cols2 = bmath_matrix_spect_get_cols( p, mat2 );
    sz_t steps = cols1 < rows2 ? cols1 : rows2;

    sr_s sr_mul = sr_p_create( p->spect_ring_scalar->spect_inst );

    for( sz_t i = 0; i < rows1; i++ )
    {
        for( sz_t j = 0; j < cols2; j++ )
        {
            sr_s sr_sum = sr_p_create( p->spect_ring_scalar->spect_inst );
            bmath_ring_q_zro( &sr_sum );
            for( sz_t k = 0; k < steps; k++ )
            {
                sr_s sr1 = bcore_matrix_spect_get_cell( p->spect_matrix_matrix, mat1, i, k );
                sr_s sr2 = bcore_matrix_spect_get_cell( p->spect_matrix_matrix, mat2, k, j );
                if ( sr1.o && sr2.o ) bmath_ring_spect_mul( p->spect_ring_scalar, sr_mul.o, sr1.o, sr2.o );
                else                  bmath_ring_spect_zro( p->spect_ring_scalar, sr_mul.o );
                bmath_ring_spect_add( p->spect_ring_scalar, sr_sum.o, sr_sum.o, sr_mul.o );
            }
            bcore_matrix_spect_set_cell( p->spect_matrix_matrix, o, i, j, sr_sum );
        }
    }

    sr_down( sr_mul );
}

void bmath_matrix_spect_one( const bmath_matrix_s* p, vd_t o )
{
    if( p->fp_one ) { p->fp_one( o ); return; }
    bmath_matrix_spect_zro( p, o );
    sz_t rows = bmath_matrix_spect_get_rows( p, o );
    sz_t cols = bmath_matrix_spect_get_cols( p, o );
    sz_t steps = rows < cols ? rows : cols;
    sr_s sr_one = sr_p_create( p->spect_ring_scalar->spect_inst );
    bmath_ring_spect_one( p->spect_ring_scalar, sr_one.o );

    for( sz_t i = 0; i < steps; i++ )
    {
        bcore_matrix_spect_set_cell( p->spect_matrix_matrix, o, i, i, sr_cw( sr_one ) );
    }

    sr_down( sr_one );
}

void bmath_matrix_spect_inv( const bmath_matrix_s* p, vd_t o, vc_t mat1 )
{
    if( p->fp_inv ) { p->fp_inv( o, mat1 ); return; }
    ERR_fa( "Function not implemented for #<sc_t>", ifnameof( p->header.o_type ) );
}

void bmath_matrix_spect_div( const bmath_matrix_s* p, vd_t o, vc_t mat1, vc_t mat2 )
{
    if( p->fp_div ) { p->fp_div( o, mat1, mat2 ); return; }
    sr_s sr_inv = sr_p_create( p );
    bmath_matrix_spect_inv( p, sr_inv.o, mat2 );
    bmath_matrix_spect_mul( p, o, mat1, sr_inv.o );
    sr_down( sr_inv );
}

void bmath_matrix_spect_mul_scl( const bmath_matrix_s* p, vd_t o, vc_t mat1, vc_t scl2 )
{
    if( p->fp_matrix_mul_scl ) { p->fp_matrix_mul_scl( o, mat1, scl2 ); return; }
    bmath_vector_spect_mul( p->spect_vector_matrix, o, mat1, scl2 );
}

void bmath_matrix_spect_mul_vec( const bmath_matrix_s* p, vd_t vec, vc_t mat1, vc_t vec2 )
{
    if( p->fp_matrix_mul_vec ) { p->fp_matrix_mul_vec( vec, mat1, vec2 ); return; }

    const bcore_array_s* spect_array_vector = p->spect_vector_default->spect_array_vector;

    sz_t rows1 = bmath_matrix_spect_get_rows( p, mat1 );
    sz_t cols1 = bmath_matrix_spect_get_cols( p, mat1 );
    sz_t  dim0 = bmath_vector_spect_get_dim( p->spect_vector_default, vec );
    sz_t  dim2 = bmath_vector_spect_get_dim( p->spect_vector_default, vec2 );

    sz_t steps = cols1 < dim2 ? cols1 : dim2;

    sr_s sr_mul = sr_p_create( p->spect_ring_scalar->spect_inst );

    for( sz_t i = 0; i < dim0; i++ )
    {
        sr_s sr_sum = sr_p_create( p->spect_ring_scalar->spect_inst );
        bmath_ring_q_zro( &sr_sum );
        if( i < rows1 )
        {
            for( sz_t k = 0; k < steps; k++ )
            {
                sr_s sr1 = bcore_matrix_spect_get_cell( p->spect_matrix_matrix, mat1, i, k );
                sr_s sr2 = bcore_array_spect_get( spect_array_vector, vec2, k );
                if ( sr1.o && sr2.o ) bmath_ring_spect_mul( p->spect_ring_scalar, sr_mul.o, sr1.o, sr2.o );
                else                  bmath_ring_spect_zro( p->spect_ring_scalar, sr_mul.o );
                bmath_ring_spect_add( p->spect_ring_scalar, sr_sum.o, sr_sum.o, sr_mul.o );
            }
        }
        bcore_array_spect_set( spect_array_vector, vec, i, sr_sum );
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
            BCORE_REGISTER_PLAIN( bmath_fp_matrix_mul_vec, function_pointer );
            BCORE_REGISTER_PLAIN( bmath_fp_matrix_mul_scl, function_pointer );
            BCORE_REGISTER_SPECT( bmath_matrix_s );
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

