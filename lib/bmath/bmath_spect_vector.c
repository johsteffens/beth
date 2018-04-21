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

#define NPX( name ) bmath_vector_##name

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
    o->spect_ring_scalar = bmath_ring_s_get_typed( o->spect_array_vector->item_p->o_type );
    return o;
}

/**********************************************************************************************************************/

sz_t bmath_vector_spect_get_dim( const bmath_vector_s* p, vc_t o )
{
    return bcore_array_spect_get_size( p->spect_array_vector, o );
}

void bmath_vector_spect_zro( const bmath_vector_s* p, vd_t vec )
{
    if( p->fp_zro ) { p->fp_zro( vec ); return; }

    sz_t dim = bmath_vector_spect_get_dim( p, vec );
    sr_s sr_zero = sr_p_create( p->spect_ring_scalar->spect_inst );

    bmath_ring_p_zro( p->spect_ring_scalar, sr_zero.o );
    for( sz_t i = 0; i < dim; i++ ) bcore_array_spect_set( p->spect_array_vector, vec, i, sr_cw( sr_zero ) );

    sr_down( sr_zero );
}

void bmath_vector_spect_neg( const bmath_vector_s* p, vd_t vec, vc_t vec1 )
{
    if( p->fp_neg ) { p->fp_neg( vec, vec1 ); return; }

    sz_t dim = bmath_vector_spect_get_dim( p, vec  );
    sr_s sr_neg = sr_p_create( p->spect_ring_scalar->spect_inst );

    for( sz_t i = 0; i < dim; i++ )
    {
        sr_s sr1 = bcore_array_spect_get( p->spect_array_vector, vec1, i );

        if( sr1.o )
        {
            bmath_ring_p_neg( p->spect_ring_scalar, sr_neg.o, sr1.o );
        }
        else
        {
            bmath_ring_p_zro( p->spect_ring_scalar, sr_neg.o );
        }

        bcore_array_spect_set( p->spect_array_vector, vec, i, sr_cw( sr_neg ) );

        sr_down( sr1 );
    }

    sr_down( sr_neg );
}

void bmath_vector_spect_cpy( const bmath_vector_s* p, vd_t vec, vc_t vec1 )
{
    if( p->fp_cpy ) { p->fp_cpy( vec, vec1 ); return; }

    sz_t dim = bmath_vector_spect_get_dim( p, vec  );
    sr_s sr_cpy = sr_p_create( p->spect_ring_scalar->spect_inst );

    for( sz_t i = 0; i < dim; i++ )
    {
        sr_s sr1 = bcore_array_spect_get( p->spect_array_vector, vec1, i );

        if( sr1.o )
        {
            bmath_ring_p_cpy( p->spect_ring_scalar, sr_cpy.o, sr1.o );
        }
        else
        {
            bmath_ring_p_zro( p->spect_ring_scalar, sr_cpy.o );
        }

        bcore_array_spect_set( p->spect_array_vector, vec, i, sr_cw( sr_cpy ) );

        sr_down( sr1 );
    }

    sr_down( sr_cpy );
}

void bmath_vector_spect_add( const bmath_vector_s* p, vd_t vec, vc_t vec1, vc_t vec2 )
{
    if( p->fp_add ) { p->fp_add( vec, vec1, vec2 ); return; }

    sz_t dim = bmath_vector_spect_get_dim( p, vec  );
    sr_s sr_add = sr_p_create( p->spect_ring_scalar->spect_inst );

    for( sz_t i = 0; i < dim; i++ )
    {
        sr_s sr1 = bcore_array_spect_get( p->spect_array_vector, vec1, i );
        sr_s sr2 = bcore_array_spect_get( p->spect_array_vector, vec2, i );

        if     ( sr1.o && sr2.o ) bmath_ring_p_add( p->spect_ring_scalar, sr_add.o, sr1.o, sr2.o );
        else if( sr1.o          ) bmath_ring_p_cpy( p->spect_ring_scalar, sr_add.o, sr1.o );
        else if( sr2.o          ) bmath_ring_p_cpy( p->spect_ring_scalar, sr_add.o, sr2.o );
        else                      bmath_ring_p_zro( p->spect_ring_scalar, sr_add.o );

        bcore_array_spect_set( p->spect_array_vector, vec, i, sr_cw( sr_add ) );

        sr_down( sr1 );
        sr_down( sr2 );
    }

    sr_down( sr_add );
}

void bmath_vector_spect_sub( const bmath_vector_s* p, vd_t vec, vc_t vec1, vc_t vec2 )
{
    if( p->fp_sub ) { p->fp_sub( vec, vec1, vec2 ); return; }

    sz_t dim = bmath_vector_spect_get_dim( p, vec  );
    sr_s sr_sub = sr_p_create( p->spect_ring_scalar->spect_inst );

    for( sz_t i = 0; i < dim; i++ )
    {
        sr_s sr1 = bcore_array_spect_get( p->spect_array_vector, vec1, i );
        sr_s sr2 = bcore_array_spect_get( p->spect_array_vector, vec2, i );

        if     ( sr1.o && sr2.o ) bmath_ring_p_sub( p->spect_ring_scalar, sr_sub.o, sr1.o, sr2.o );
        else if( sr1.o          ) bmath_ring_p_cpy( p->spect_ring_scalar, sr_sub.o, sr1.o );
        else if( sr2.o          ) bmath_ring_p_neg( p->spect_ring_scalar, sr_sub.o, sr2.o );
        else                      bmath_ring_p_zro( p->spect_ring_scalar, sr_sub.o );

        bcore_array_spect_set( p->spect_array_vector, vec, i, sr_cw( sr_sub ) );

        sr_down( sr1 );
        sr_down( sr2 );
    }

    sr_down( sr_sub );
}

void bmath_vector_spect_mul( const bmath_vector_s* p, vd_t vec, vc_t vec1, vc_t scl2 )
{
    if( p->fp_mul ) { p->fp_mul( vec, vec1, scl2 ); return; }
    if( !scl2 )     { bmath_vector_spect_zro( p, vec ); return; }

    sr_s sr_mul = sr_p_create( p->spect_ring_scalar->spect_inst );

    sz_t dim = bmath_vector_spect_get_dim( p, vec  );
    for( sz_t i = 0; i < dim; i++ )
    {
        sr_s sr1 = bcore_array_spect_get( p->spect_array_vector, vec1, i );
        if   ( sr1.o ) bmath_ring_p_mul( p->spect_ring_scalar, sr_mul.o, sr1.o, scl2 );
        else           bmath_ring_p_zro( p->spect_ring_scalar, sr_mul.o );
        bcore_array_spect_set( p->spect_array_vector, vec, i, sr_cw( sr_mul ) );
        sr_down( sr1 );
    }

    sr_down( sr_mul );
}

void bmath_vector_spect_dot_prd( const bmath_vector_s* p, vd_t scl, vc_t vec1, vc_t vec2 )
{
    if( p->fp_dot_prd ) { p->fp_dot_prd( scl, vec1, vec2 ); return; }

    const bcore_inst_s* spect_ring_inst = p->spect_ring_scalar->spect_inst;

    sz_t dim1 = bmath_vector_spect_get_dim( p, vec1 );
    sz_t dim2 = bmath_vector_spect_get_dim( p, vec2 );
    sz_t dim = dim1 < dim2 ? dim1 : dim2;
    sr_s sr_mul = sr_p_create( spect_ring_inst );
    sr_s sr_sum = sr_pwd( spect_ring_inst, scl );

    bmath_ring_p_zro( p->spect_ring_scalar, sr_sum.o );

    for( sz_t i = 0; i < dim; i++ )
    {
        sr_s sr1 = bcore_array_spect_get( p->spect_array_vector, vec1, i );
        sr_s sr2 = bcore_array_spect_get( p->spect_array_vector, vec2, i );

        if ( sr1.o && sr2.o ) bmath_ring_p_mul( p->spect_ring_scalar, sr_mul.o, sr1.o, sr2.o );
        else                  bmath_ring_p_zro( p->spect_ring_scalar, sr_mul.o );
        bmath_ring_p_add( p->spect_ring_scalar, sr_sum.o, sr_sum.o, sr_mul.o );

        sr_down( sr1 );
        sr_down( sr2 );
    }

    sr_down( sr_sum );
    sr_down( sr_mul );
}

void bmath_vector_spect_sqr_sub( const bmath_vector_s* p, vd_t scl, vc_t vec1, vc_t vec2 )
{
    if( p->fp_dot_prd ) { p->fp_dot_prd( scl, vec1, vec2 ); return; }

    const bcore_inst_s* spect_ring_inst = p->spect_ring_scalar->spect_inst;

    sz_t dim1 = bmath_vector_spect_get_dim( p, vec1 );
    sz_t dim2 = bmath_vector_spect_get_dim( p, vec2 );
    sz_t dim = dim1 < dim2 ? dim1 : dim2;
    sr_s sr_mul = sr_p_create( spect_ring_inst );
    sr_s sr_sum = sr_pwd( spect_ring_inst, scl );

    bmath_ring_p_zro( p->spect_ring_scalar, sr_sum.o );

    for( sz_t i = 0; i < dim; i++ )
    {
        sr_s sr1 = bcore_array_spect_get( p->spect_array_vector, vec1, i );
        sr_s sr2 = bcore_array_spect_get( p->spect_array_vector, vec2, i );

        if( sr1.o && sr2.o )
        {
            bmath_ring_p_sub( p->spect_ring_scalar, sr_mul.o, sr1.o, sr2.o );
            bmath_ring_p_mul( p->spect_ring_scalar, sr_mul.o, sr_mul.o, sr_mul.o );
        }
        else
        {
            if     ( sr1.o ) bmath_ring_p_mul( p->spect_ring_scalar, sr_mul.o, sr1.o, sr1.o );
            else if( sr2.o ) bmath_ring_p_mul( p->spect_ring_scalar, sr_mul.o, sr2.o, sr2.o );
            else             bmath_ring_p_zro( p->spect_ring_scalar, sr_mul.o );
        }
        bmath_ring_p_add( p->spect_ring_scalar, sr_sum.o, sr_sum.o, sr_mul.o );

        sr_down( sr1 );
        sr_down( sr2 );
    }

    sr_down( sr_sum );
    sr_down( sr_mul );
}

void bmath_vector_spect_sqr( const bmath_vector_s* p, vd_t scl, vc_t vec1 )
{
    bmath_vector_spect_dot_prd( p, scl, vec1, vec1 );
}

/**********************************************************************************************************************/

static inline const bmath_vector_s* atpd( tp_t tp ) { return bmath_vector_s_get_typed( tp ); }
static inline vc_t w_spect( sr_s o ) { if( sr_s_is_const( &o ) ) ERR( "Attempt to modify a constant object" ); return ch_spect_p( o.p, TYPEOF_bmath_vector_s ); }
static inline vc_t r_spect( sr_s o ) { return ch_spect_p( o.p, TYPEOF_bmath_vector_s ); }

sz_t NPX(typed_get_dim)( tp_t t, vc_t o                   ) { return NPX(spect_get_dim)( atpd( t ), o  ); }
void NPX(typed_zro    )( tp_t t, vd_t o                   ) { NPX(spect_zro    )( atpd( t ), o         ); }
void NPX(typed_neg    )( tp_t t, vd_t o, vc_t v1          ) { NPX(spect_neg    )( atpd( t ), o, v1     ); }
void NPX(typed_cpy    )( tp_t t, vd_t o, vc_t v1          ) { NPX(spect_cpy    )( atpd( t ), o, v1     ); }
void NPX(typed_add    )( tp_t t, vd_t o, vc_t v1, vc_t v2 ) { NPX(spect_add    )( atpd( t ), o, v1, v2 ); }
void NPX(typed_sub    )( tp_t t, vd_t o, vc_t v1, vc_t v2 ) { NPX(spect_sub    )( atpd( t ), o, v1, v2 ); }
void NPX(typed_mul    )( tp_t t, vd_t o, vc_t v1, vc_t s2 ) { NPX(spect_mul    )( atpd( t ), o, v1, s2 ); }
void NPX(typed_dot_prd)( tp_t t, vd_t s, vc_t o,  vc_t v2 ) { NPX(spect_dot_prd)( atpd( t ), s,  o, v2 ); }
void NPX(typed_sqr    )( tp_t t, vd_t s, vc_t o           ) { NPX(spect_sqr    )( atpd( t ), s,  o     ); }
void NPX(typed_sqr_sub)( tp_t t, vd_t s, vc_t o,  vc_t v2 ) { NPX(spect_sqr_sub)( atpd( t ), s,  o, v2 ); }

sz_t NPX(aware_get_dim)( vc_t o                   ) { return NPX(typed_get_dim)( *(aware_t*)o, o  ); }
void NPX(aware_zro    )( vd_t o                   ) { NPX(typed_zro    )( *(aware_t*)o, o         ); }
void NPX(aware_neg    )( vd_t o, vc_t v1          ) { NPX(typed_neg    )( *(aware_t*)o, o, v1     ); }
void NPX(aware_cpy    )( vd_t o, vc_t v1          ) { NPX(typed_cpy    )( *(aware_t*)o, o, v1     ); }
void NPX(aware_add    )( vd_t o, vc_t v1, vc_t v2 ) { NPX(typed_add    )( *(aware_t*)o, o, v1, v2 ); }
void NPX(aware_sub    )( vd_t o, vc_t v1, vc_t v2 ) { NPX(typed_sub    )( *(aware_t*)o, o, v1, v2 ); }
void NPX(aware_mul    )( vd_t o, vc_t v1, vc_t s2 ) { NPX(typed_mul    )( *(aware_t*)o, o, v1, s2 ); }
void NPX(aware_dot_prd)( vd_t s, vc_t o,  vc_t v2 ) { NPX(typed_dot_prd)( *(aware_t*)o, s,  o, v2 ); }
void NPX(aware_sqr    )( vd_t s, vc_t o           ) { NPX(typed_sqr    )( *(aware_t*)o, s,  o     ); }
void NPX(aware_sqr_sub)( vd_t s, vc_t o,  vc_t v2 ) { NPX(typed_sqr_sub)( *(aware_t*)o, s,  o, v2 ); }

sz_t NPX(get_dim)( sr_s o                   ) { sz_t r = NPX(spect_get_dim )( r_spect( o ), o.o ); sr_down( o ); return r; }
void NPX(zro    )( sr_s o                   ) { NPX(spect_zro    )( w_spect( o ), o.o         ); sr_down( o ); }
void NPX(neg    )( sr_s o, vc_t v1          ) { NPX(spect_neg    )( w_spect( o ), o.o, v1     ); sr_down( o ); }
void NPX(cpy    )( sr_s o, vc_t v1          ) { NPX(spect_cpy    )( w_spect( o ), o.o, v1     ); sr_down( o ); }
void NPX(add    )( sr_s o, vc_t v1, vc_t v2 ) { NPX(spect_add    )( w_spect( o ), o.o, v1, v2 ); sr_down( o ); }
void NPX(sub    )( sr_s o, vc_t v1, vc_t v2 ) { NPX(spect_sub    )( w_spect( o ), o.o, v1, v2 ); sr_down( o ); }
void NPX(mul    )( sr_s o, vc_t v1, vc_t s2 ) { NPX(spect_mul    )( w_spect( o ), o.o, v1, s2 ); sr_down( o ); }
void NPX(dot_prd)( vd_t s, sr_s o,  vc_t v2 ) { NPX(spect_dot_prd)( r_spect( o ), s, o.o, v2  ); sr_down( o ); }
void NPX(sqr    )( vd_t s, sr_s o           ) { NPX(spect_sqr    )( r_spect( o ), s, o.o      ); sr_down( o ); }
void NPX(sqr_sub)( vd_t s, sr_s o,  vc_t v2 ) { NPX(spect_sqr_sub)( r_spect( o ), s, o.o, v2  ); sr_down( o ); }

sz_t NPX(q_get_dim)( const sr_s* o                   ) { sz_t r = NPX(spect_get_dim )( r_spect( *o ), o->o ); return r; }
void NPX(q_zro    )( const sr_s* o                   ) { NPX(spect_zro    )( w_spect( *o ), o->o         ); }
void NPX(q_neg    )( const sr_s* o, vc_t v1          ) { NPX(spect_neg    )( w_spect( *o ), o->o, v1     ); }
void NPX(q_cpy    )( const sr_s* o, vc_t v1          ) { NPX(spect_cpy    )( w_spect( *o ), o->o, v1     ); }
void NPX(q_add    )( const sr_s* o, vc_t v1, vc_t v2 ) { NPX(spect_add    )( w_spect( *o ), o->o, v1, v2 ); }
void NPX(q_sub    )( const sr_s* o, vc_t v1, vc_t v2 ) { NPX(spect_sub    )( w_spect( *o ), o->o, v1, v2 ); }
void NPX(q_mul    )( const sr_s* o, vc_t v1, vc_t s2 ) { NPX(spect_mul    )( w_spect( *o ), o->o, v1, s2 ); }
void NPX(q_dot_prd)( vd_t s, const sr_s* o,  vc_t v2 ) { NPX(spect_dot_prd)( r_spect( *o ), s, o->o, v2  ); }
void NPX(q_sqr    )( vd_t s, const sr_s* o           ) { NPX(spect_sqr    )( r_spect( *o ), s, o->o      ); }
void NPX(q_sqr_sub)( vd_t s, const sr_s* o,  vc_t v2 ) { NPX(spect_sqr_sub)( r_spect( *o ), s, o->o, v2  ); }

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

    bmath_vector_add( sv3, sv1.o, sv2.o );

    ASSERT( v3->data[ 0 ] == 4 );
    ASSERT( v3->data[ 1 ] == 4 );
    ASSERT( v3->data[ 2 ] == 4 );

    bmath_vector_sub( sv3, sv1.o, sv2.o );

    ASSERT( v3->data[ 0 ] == -2 );
    ASSERT( v3->data[ 1 ] ==  0 );
    ASSERT( v3->data[ 2 ] ==  2 );

    f3_t f = 2.0;
    bmath_vector_mul( sv3, sv1.o, &f );

    ASSERT( v3->data[ 0 ] == 2 );
    ASSERT( v3->data[ 1 ] == 4 );
    ASSERT( v3->data[ 2 ] == 6 );

    bmath_vector_sqr( &f, sv1 );

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

