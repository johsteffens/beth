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

#include "bmath_pmt.h"
#include "bmath_mf3.h"

/**********************************************************************************************************************/

BCORE_DEFINE_OBJECT_INST( bcore_inst, bmath_pmt_s ) "{ aware_t _; uz_t [] arr; }";

//----------------------------------------------------------------------------------------------------------------------

void bmath_pmt_s_set_size( bmath_pmt_s* o, uz_t size )
{
    bcore_array_a_set_size( (bcore_array* )o, size );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_pmt_s_random( bmath_pmt_s* o, u2_t* rval )
{
    bmath_pmt_s_one( o );
    uz_t idx = 0;

    for( uz_t i = 0; i < o->size; i++ )
    {
        idx = i + ( ( idx + ( *rval = bcore_xsg1_u2( *rval ) ) ) % ( o->size - i ) );
        uz_t t = o->data[ i ];
        o->data[ i ] = o->data[ idx ];
        o->data[ idx ] = t;
    }
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_pmt_s_one_n( bmath_pmt_s* o, uz_t n )
{
    bmath_pmt_s_set_size( o, n );
    for( uz_t i = 0; i < n; i++ ) o->data[ i ] = i;
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_pmt_s_one( bmath_pmt_s* o )
{
    for( uz_t i = 0; i < o->size; i++ ) o->data[ i ] = i;
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_pmt_s_cpy( const bmath_pmt_s* o, bmath_pmt_s* res )
{
    ASSERT( o->size == res->size );
    bcore_u_memcpy( sizeof( uz_t ), res->data, o->data, o->size );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_pmt_s_to_mf3( const bmath_pmt_s* o, bmath_mf3_s* res )
{
    ASSERT( o->size == res->rows );
    ASSERT( o->size == res->cols );
    bmath_mf3_s_zro( res );
    for( uz_t i = 0; i < o->size; i++ )
    {
        assert( o->data[ i ] < o->size );
        res->data[ o->data[ i ] * res->stride + i ] = 1;
    }
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_pmt_s_htp( const bmath_pmt_s* o, bmath_pmt_s* res )
{
    if( res == o )
    {
        bmath_pmt_s* buf = bmath_pmt_s_create();
        bmath_pmt_s_one_n( buf, o->size );
        bmath_pmt_s_htp( o, buf );
        bmath_pmt_s_cpy( buf, res );
        bmath_pmt_s_discard( buf );
        return;
    }
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ )
    {
        assert( o->data[ i ] < res->size );
        res->data[ o->data[ i ] ] = i;
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**  y = P1 * x  <=> y_p1[k] = x_k
 *   z = P2 * y  <=> z_p2[l] = y_l  <=> z_p2[p1[k]] = y_p1[k] = x_k
 *   P = P2 * P1  => z_p[k]  = x_k   => p[k] = p2[p1[k]]
 */
void bmath_pmt_s_mul( const bmath_pmt_s* o, const bmath_pmt_s* op, bmath_pmt_s* res )
{
    if( res == o || res == op )
    {
        bmath_pmt_s* buf = bmath_pmt_s_create();
        bmath_pmt_s_one_n( buf, o->size );
        bmath_pmt_s_mul( o, op, buf );
        bmath_pmt_s_cpy( buf, res );
        bmath_pmt_s_discard( buf );
        return;
    }
    ASSERT( o->size ==  op->size );
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ )
    {
        assert(          op->data[ i ]   < o->size );
        assert( o->data[ op->data[ i ] ] < o->size );
        res->data[ i ] = o->data[ op->data[ i ] ];
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**  y = P1^T * x  <=> y_l = x_p1[l]
 *   z = P2 * y    <=> z_p2[l] = y_l = x_p1[l]
 *   P = P2 * P1^T  => z_p[k]  = x_k   => p[p1[l]] = p2[l]
 */
void bmath_pmt_s_mul_htp( const bmath_pmt_s* o, const bmath_pmt_s* op, bmath_pmt_s* res )
{
    if( res == o || res == op )
    {
        bmath_pmt_s* buf = bmath_pmt_s_create();
        bmath_pmt_s_one_n( buf, o->size );
        bmath_pmt_s_mul_htp( o, op, buf );
        bmath_pmt_s_cpy( buf, res );
        bmath_pmt_s_discard( buf );
        return;
    }
    ASSERT( o->size ==  op->size );
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ )
    {
        assert( op->data[ i ] < o->size );
        assert(  o->data[ i ] < o->size );
        res->data[ op->data[ i ] ] = o->data[ i ];
    }
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bmath_pmt_s_is_equ( const bmath_pmt_s* o, const bmath_pmt_s* op )
{
    if( o->size != op->size ) return false;
    for( uz_t i = 0; i < o->size; i++ )
    {
        if( o->data[ i ] != op->data[ i ] ) return false;
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bmath_pmt_s_is_one( const bmath_pmt_s* o )
{
    for( uz_t i = 0; i < o->size; i++ )
    {
        if( o->data[ i ] != i ) return false;
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_pmt_s_to_string( const bmath_pmt_s* o, st_s* string )
{
    st_s_push_fa( string, "(#<uz_t>) { ", o->size );

    for( uz_t i = 0; i < o->size; i++ )
    {
        if( i > 0 ) st_s_push_fa( string, ", ", o->data[ i ] );
        st_s_push_fa( string, "#<uz_t>", o->data[ i ] );
    }
    st_s_push_fa( string, " }\n" );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_pmt_s_to_stdout( const bmath_pmt_s* o )
{
    st_s* s = st_s_create();
    bmath_pmt_s_to_string( o, s );
    bcore_msg_fa( "#<sc_t>", s->sc );
    st_s_discard( s );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

static void selftest()
{
    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( bmath_pmt_s, p1 );
    BCORE_LIFE_CREATE( bmath_pmt_s, p2 );
    BCORE_LIFE_CREATE( bmath_pmt_s, p3 );

    BCORE_LIFE_CREATE( bmath_mf3_s, m1 );
    BCORE_LIFE_CREATE( bmath_mf3_s, m2 );
    BCORE_LIFE_CREATE( bmath_mf3_s, m3 );
    BCORE_LIFE_CREATE( bmath_mf3_s, m4 );

    BCORE_LIFE_CREATE( bmath_vf3_s, v1 );
    BCORE_LIFE_CREATE( bmath_vf3_s, v2 );
    BCORE_LIFE_CREATE( bmath_vf3_s, v3 );

    u2_t rval = 1234;

    uz_t n = 10;

    bmath_pmt_s_set_size( p1, n );
    bmath_pmt_s_set_size( p2, n );
    bmath_pmt_s_set_size( p3, n );

    bmath_pmt_s_random( p1, &rval );

    bmath_pmt_s_cpy( p1, p2 );
    bmath_pmt_s_htp( p2, p2 );
    bmath_pmt_s_mul( p1, p2, p3 );
    ASSERT( bmath_pmt_s_is_one( p3 ) );

    bmath_pmt_s_cpy( p1, p2 );
    bmath_pmt_s_mul_htp( p1, p2, p3 );
    ASSERT( bmath_pmt_s_is_one( p3 ) );

    bmath_mf3_s_set_size( m1, n, n );
    bmath_mf3_s_set_size( m2, n, n );
    bmath_mf3_s_set_size( m3, n, n );
    bmath_mf3_s_set_size( m4, n, n );
    bmath_pmt_s_to_mf3( p1, m1 );
    bmath_mf3_s_set_random( m2, false, false, 0, 1.0, -1, 1, &rval );

    bmath_mf3_s_mul_pmt( m2, p1, m3 );
    bmath_mf3_s_mul(     m2, m1, m4 );
    ASSERT( bmath_mf3_s_is_equ( m3, m4 ) );

    bmath_mf3_s_mul_pmt_htp( m2, p1, m3 );
    bmath_mf3_s_mul_htp(     m2, m1, m4 );
    ASSERT( bmath_mf3_s_is_equ( m3, m4 ) );

    bmath_mf3_s_pmt_mul( m2, p1, m3 );
    bmath_mf3_s_mul(     m1, m2, m4 );
    ASSERT( bmath_mf3_s_is_equ( m3, m4 ) );

    bmath_mf3_s_pmt_htp_mul( m2, p1, m3 );
    bmath_mf3_s_htp( m1, m1 );
    bmath_mf3_s_mul( m1, m2, m4 );
    bmath_mf3_s_htp( m1, m1 );
    ASSERT( bmath_mf3_s_is_equ( m3, m4 ) );

    bmath_vf3_s_set_size( v1, n );
    bmath_vf3_s_set_size( v2, n );
    bmath_vf3_s_set_size( v3, n );
    bmath_vf3_s_set_random( v1, 1.0, -1, 1, &rval );
    bmath_vf3_s_pmt_mul( v1, p1, v2 );
    bmath_mf3_s_mul_vec( m1, v1, v3 );
    ASSERT( bmath_vf3_s_is_equ( v2, v3 ) );

    bmath_vf3_s_pmt_htp_mul( v2, p1, v3 );
    ASSERT( bmath_vf3_s_is_equ( v1, v3 ) );

    BCORE_LIFE_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

vd_t bmath_pmt_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_pmt" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_OBJECT( bmath_pmt_s );
        }
        break;

        case TYPEOF_selftest:
        {
            selftest();
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

