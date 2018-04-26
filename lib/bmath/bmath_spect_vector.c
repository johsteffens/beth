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

#include "bmath_spect_vector.h"
#include "bmath_quicktypes.h"

/**********************************************************************************************************************/


BCORE_DEFINE_SPECT( bmath_vector_s )
"{"
    "bcore_spect_header_s header;"
    "private        bmath_ring_s  -> spect_ring_scalar;"
    "strict feature bcore_array_s -> spect_array_vector;"
    "strict feature bcore_inst_s  -> spect_inst_vector;"
    "       feature bmath_fp:        add;"
    "       feature bmath_fp:        zro;"
    "       feature bmath_fp:        neg;"
    "       feature bmath_fp:        sub;"
    "       feature bmath_fp:        cpy;"
    "       feature bmath_fp:        vector_mul;"
    "       feature bmath_fp_vector: dot_prd;"


    "func bcore_spect_fp: create_from_self;"
"}";

static bmath_vector_s* bmath_vector_s_create_from_self( const bcore_self_s* self )
{
    bmath_vector_s* o = bcore_spect_create_from_self( bcore_flect_get_self( typeof( "bmath_vector_s" ) ), self );
    if( !o->spect_array_vector->item_p ) ERR_fa( "Cannot determine item type in object '#<sc_t>'", ifnameof( self->type ) );
    o->spect_ring_scalar = bmath_ring_s_get_typed( o->spect_array_vector->item_p->header.o_type );
    return o;
}

/**********************************************************************************************************************/

sz_t bmath_vector_default_get_dim( const bmath_vector_s* p, const bmath_vector* o )
{
    return bcore_array_spect_get_size( p->spect_array_vector, ( vc_t )o );
}

void bmath_vector_default_zro( const bmath_vector_s* p, bmath_vector* vec )
{
    if( p->zro ) { p->zro( ( bmath_group* )vec ); return; }

    sz_t dim = bmath_vector_default_get_dim( p, vec );
    sr_s sr_zero = sr_p_create( p->spect_ring_scalar->spect_inst );

    bmath_ring_p_zro( p->spect_ring_scalar, sr_zero.o );
    for( sz_t i = 0; i < dim; i++ ) bcore_array_spect_set( p->spect_array_vector, vec, i, sr_cw( sr_zero ) );

    sr_down( sr_zero );
}

void bmath_vector_default_neg( const bmath_vector_s* p, const bmath_vector* vec, bmath_vector* res )
{
    if( p->neg ) { p->neg( ( const bmath_group* )vec, ( bmath_group* )res ); return; }

    sz_t res_dim = bmath_vector_default_get_dim( p, res );
    sr_s sr_neg  = sr_p_create( p->spect_ring_scalar->spect_inst );

    for( sz_t i = 0; i < res_dim; i++ )
    {
        sr_s sr1 = bcore_array_spect_get( p->spect_array_vector, vec, i );

        if( sr1.o )
        {
            bmath_ring_p_neg( p->spect_ring_scalar, sr1.o, sr_neg.o );
        }
        else
        {
            bmath_ring_p_zro( p->spect_ring_scalar, sr_neg.o );
        }

        bcore_array_spect_set( p->spect_array_vector, res, i, sr_cw( sr_neg ) );

        sr_down( sr1 );
    }

    sr_down( sr_neg );
}

void bmath_vector_default_cpy( const bmath_vector_s* p, const bmath_vector* vec, bmath_vector* res )
{
    if( p->cpy ) { p->cpy( ( const bmath_group* )vec, ( bmath_group* )res ); return; }

    sz_t res_dim = bmath_vector_default_get_dim( p, res );
    sr_s sr_cpy = sr_p_create( p->spect_ring_scalar->spect_inst );

    for( sz_t i = 0; i < res_dim; i++ )
    {
        sr_s sr1 = bcore_array_spect_get( p->spect_array_vector, vec, i );

        if( sr1.o )
        {
            bmath_ring_p_cpy( p->spect_ring_scalar, sr1.o, sr_cpy.o );
        }
        else
        {
            bmath_ring_p_zro( p->spect_ring_scalar, sr_cpy.o );
        }

        bcore_array_spect_set( p->spect_array_vector, res, i, sr_cw( sr_cpy ) );

        sr_down( sr1 );
    }

    sr_down( sr_cpy );
}

void bmath_vector_default_add( const bmath_vector_s* p, const bmath_vector* vec1, const bmath_vector* vec2, bmath_vector* res )
{
    if( p->add ) { p->add( ( const bmath_group* )vec1, ( const bmath_group* )vec2, ( bmath_group* )res ); return; }

    sz_t res_dim = bmath_vector_default_get_dim( p, res );
    sr_s sr_add = sr_p_create( p->spect_ring_scalar->spect_inst );

    for( sz_t i = 0; i < res_dim; i++ )
    {
        sr_s sr1 = bcore_array_spect_get( p->spect_array_vector, vec1, i );
        sr_s sr2 = bcore_array_spect_get( p->spect_array_vector, vec2, i );

        if     ( sr1.o && sr2.o ) bmath_ring_p_add( p->spect_ring_scalar, sr1.o, sr2.o, sr_add.o );
        else if( sr1.o          ) bmath_ring_p_cpy( p->spect_ring_scalar, sr1.o, sr_add.o );
        else if( sr2.o          ) bmath_ring_p_cpy( p->spect_ring_scalar, sr2.o, sr_add.o );
        else                      bmath_ring_p_zro( p->spect_ring_scalar, sr_add.o );

        bcore_array_spect_set( p->spect_array_vector, res, i, sr_cw( sr_add ) );

        sr_down( sr1 );
        sr_down( sr2 );
    }

    sr_down( sr_add );
}

void bmath_vector_default_sub( const bmath_vector_s* p, const bmath_vector* vec1, const bmath_vector* vec2, bmath_vector* res )
{
    if( p->sub ) { p->sub( ( const bmath_group* )vec1, ( const bmath_group* )vec2, ( bmath_group* )res ); return; }

    sz_t res_dim = bmath_vector_default_get_dim( p, res );
    sr_s sr_sub = sr_p_create( p->spect_ring_scalar->spect_inst );

    for( sz_t i = 0; i < res_dim; i++ )
    {
        sr_s sr1 = bcore_array_spect_get( p->spect_array_vector, vec1, i );
        sr_s sr2 = bcore_array_spect_get( p->spect_array_vector, vec2, i );

        if     ( sr1.o && sr2.o ) bmath_ring_p_sub( p->spect_ring_scalar, sr1.o, sr2.o, sr_sub.o );
        else if( sr1.o          ) bmath_ring_p_cpy( p->spect_ring_scalar, sr1.o, sr_sub.o );
        else if( sr2.o          ) bmath_ring_p_neg( p->spect_ring_scalar, sr2.o, sr_sub.o );
        else                      bmath_ring_p_zro( p->spect_ring_scalar, sr_sub.o );

        bcore_array_spect_set( p->spect_array_vector, res, i, sr_cw( sr_sub ) );

        sr_down( sr1 );
        sr_down( sr2 );
    }

    sr_down( sr_sub );
}

void bmath_vector_default_mul( const bmath_vector_s* p, const bmath_vector* vec1, const bmath_ring* scl2, bmath_vector* res  )
{
    if( p->mul ) { p->mul( vec1, scl2, res ); return; }
    if( !scl2 )  { bmath_vector_default_zro( p, res ); return; }

    sr_s sr_mul = sr_p_create( p->spect_ring_scalar->spect_inst );

    sz_t res_dim = bmath_vector_default_get_dim( p, res );
    for( sz_t i = 0; i < res_dim; i++ )
    {
        sr_s sr1 = bcore_array_spect_get( p->spect_array_vector, vec1, i );
        if   ( sr1.o ) bmath_ring_p_mul( p->spect_ring_scalar, sr1.o, scl2, sr_mul.o );
        else           bmath_ring_p_zro( p->spect_ring_scalar, sr_mul.o );
        bcore_array_spect_set( p->spect_array_vector, res, i, sr_cw( sr_mul ) );
        sr_down( sr1 );
    }

    sr_down( sr_mul );
}

void bmath_vector_default_dot_prd( const bmath_vector_s* p, const bmath_vector* vec1, const bmath_vector* vec2, bmath_ring* res )
{
    if( p->dot_prd ) { p->dot_prd( vec1, vec2, res ); return; }

    const bcore_inst_s* spect_ring_inst = p->spect_ring_scalar->spect_inst;

    sz_t dim1 = bmath_vector_default_get_dim( p, vec1 );
    sz_t dim2 = bmath_vector_default_get_dim( p, vec2 );
    sz_t dim = dim1 < dim2 ? dim1 : dim2;
    sr_s sr_mul = sr_p_create( spect_ring_inst );
    sr_s sr_sum = sr_pwd( spect_ring_inst, res );

    bmath_ring_p_zro( p->spect_ring_scalar, sr_sum.o );

    for( sz_t i = 0; i < dim; i++ )
    {
        sr_s sr1 = bcore_array_spect_get( p->spect_array_vector, vec1, i );
        sr_s sr2 = bcore_array_spect_get( p->spect_array_vector, vec2, i );

        if ( sr1.o && sr2.o ) bmath_ring_p_mul( p->spect_ring_scalar, sr1.o, sr2.o, sr_mul.o );
        else                  bmath_ring_p_zro( p->spect_ring_scalar, sr_mul.o );
        bmath_ring_p_add( p->spect_ring_scalar, sr_sum.o, sr_mul.o, sr_sum.o );

        sr_down( sr1 );
        sr_down( sr2 );
    }

    sr_down( sr_sum );
    sr_down( sr_mul );
}

void bmath_vector_default_sub_sqr( const bmath_vector_s* p, const bmath_vector* vec1, const bmath_vector* vec2, bmath_ring* res )
{
    if( p->dot_prd ) { p->dot_prd( vec1, vec2, res ); return; }

    const bcore_inst_s* spect_ring_inst = p->spect_ring_scalar->spect_inst;

    sz_t dim1 = bmath_vector_default_get_dim( p, vec1 );
    sz_t dim2 = bmath_vector_default_get_dim( p, vec2 );
    sz_t dim = dim1 < dim2 ? dim1 : dim2;
    sr_s sr_mul = sr_p_create( spect_ring_inst );
    sr_s sr_sum = sr_pwd( spect_ring_inst, res );

    bmath_ring_p_zro( p->spect_ring_scalar, sr_sum.o );

    for( sz_t i = 0; i < dim; i++ )
    {
        sr_s sr1 = bcore_array_spect_get( p->spect_array_vector, vec1, i );
        sr_s sr2 = bcore_array_spect_get( p->spect_array_vector, vec2, i );

        if( sr1.o && sr2.o )
        {
            bmath_ring_p_sub( p->spect_ring_scalar, sr1.o, sr2.o, sr_mul.o );
            bmath_ring_p_mul( p->spect_ring_scalar, sr_mul.o, sr_mul.o, sr_mul.o );
        }
        else
        {
            if     ( sr1.o ) bmath_ring_p_mul( p->spect_ring_scalar, sr1.o, sr1.o, sr_mul.o );
            else if( sr2.o ) bmath_ring_p_mul( p->spect_ring_scalar, sr2.o, sr2.o, sr_mul.o );
            else             bmath_ring_p_zro( p->spect_ring_scalar, sr_mul.o );
        }
        bmath_ring_p_add( p->spect_ring_scalar, sr_sum.o, sr_mul.o, sr_sum.o );

        sr_down( sr1 );
        sr_down( sr2 );
    }

    sr_down( sr_sum );
    sr_down( sr_mul );
}

void bmath_vector_default_sqr( const bmath_vector_s* p, const bmath_vector* vec1, bmath_ring* res )
{
    bmath_vector_default_dot_prd( p, vec1, vec1, res );
}

/**********************************************************************************************************************/

static vd_t selftest( void )
{
    bcore_life_s* l = bcore_life_s_create();

    typedef struct vector_s { f3_t data[ 3 ]; } vector_s;
    tp_t t_vector = bcore_flect_type_parse_sc( "{ f3_t [ 3 ] data; }" );

    sr_s sv1 = bcore_life_s_push_sr( l, sr_create( t_vector ) );
    sr_s sv2 = bcore_life_s_push_sr( l, sr_create( t_vector ) );
    sr_s sv3 = bcore_life_s_push_sr( l, sr_create( t_vector ) );
    vector_s* v1 = sv1.o;
    vector_s* v2 = sv2.o;
    vector_s* v3 = sv3.o;

    v1->data[ 0 ] = 1;
    v1->data[ 1 ] = 2;
    v1->data[ 2 ] = 3;
    v2->data[ 0 ] = 3;
    v2->data[ 1 ] = 2;
    v2->data[ 2 ] = 1;

    bmath_vector_x_add( sv1, sv2.o, sv3.o );

    ASSERT( v3->data[ 0 ] == 4 );
    ASSERT( v3->data[ 1 ] == 4 );
    ASSERT( v3->data[ 2 ] == 4 );

    bmath_vector_x_sub( sv1, sv2.o, sv3.o );

    ASSERT( v3->data[ 0 ] == -2 );
    ASSERT( v3->data[ 1 ] ==  0 );
    ASSERT( v3->data[ 2 ] ==  2 );

    f3_t f = 2.0;
    bmath_vector_x_mul( sv1, ( const bmath_ring* )&f, sv3.o );

    ASSERT( v3->data[ 0 ] == 2 );
    ASSERT( v3->data[ 1 ] == 4 );
    ASSERT( v3->data[ 2 ] == 6 );

    bmath_vector_x_sqr( sv1, ( bmath_ring* )&f );

    ASSERT( f == 14 );

    bcore_life_s_discard( l );

    return NULL;
}

/**********************************************************************************************************************/

vd_t bmath_spect_vector_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_spect_vector" ) ) )
    {
        case TYPEOF_init1:
        {
            // features
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_vector_mul );
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_vector_dot_prd );
            BCORE_REGISTER_FFUNC( bcore_spect_fp_create_from_self, bmath_vector_s_create_from_self );
            BCORE_REGISTER_SPECT( bmath_vector_s );
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

