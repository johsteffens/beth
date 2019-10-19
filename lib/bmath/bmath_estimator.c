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

#include "bmath_estimator.h"

BCORE_DEFINE_OBJECT_INST( bcore_inst, bmath_estimator_s )
"{"
    "aware_t _;"
    "f3_t eps = 1E-8;" // epsilon (for pseudoinverse)
    "bmath_mf3_s xx;"
    "bmath_mf3_s yx;"
"}";

//----------------------------------------------------------------------------------------------------------------------

void bmath_estimator_s_clear( bmath_estimator_s* o )
{
    bmath_mf3_s_set_size( &o->xx, 0, 0 );
    bmath_mf3_s_set_size( &o->yx, 0, 0 );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_estimator_s_digest( bmath_estimator_s* o, f3_t weight, const bmath_vf3_s* x, const bmath_vf3_s* y )
{
    uz_t n = x->size;
    uz_t m = y->size;

    if( o->xx.rows == 0 && o->xx.cols == 0 )
    {
        bmath_mf3_s_set_size( &o->xx, n + 1, n + 1 );
        bmath_mf3_s_set_size( &o->yx, m    , n + 1 );
        bmath_mf3_s_zro( &o->xx );
        bmath_mf3_s_zro( &o->yx );
    }

    for( uz_t i = 0; i < n; i++ )
    {
        f3_t  xi = x->data[ i ];
        f3_t* vi = o->xx.data + i * o->xx.stride;
        for( uz_t j = 0; j < n; j++ )
        {
            f3_t xj = x->data[ j ];
            vi[ j ] += xi * xj * weight;
        }
        vi[ n ] += xi * weight;
    }

    f3_t* vn = o->xx.data + n * o->xx.stride;
    for( uz_t j = 0; j < n; j++ )
    {
        f3_t xj = x->data[ j ];
        vn[ j ] += xj * weight;
    }
    vn[ n ] += weight;

    for( uz_t i = 0; i < m; i++ )
    {
        f3_t  yi = y->data[ i ];
        f3_t* vi = o->yx.data + i * o->yx.stride;
        for( uz_t j = 0; j < n; j++ )
        {
            f3_t xj = x->data[ j ];
            vi[ j ] += yi * xj * weight;
        }
        vi[ n ] += yi * weight;
    }
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_estimator_s_reweight( bmath_estimator_s* o, f3_t weight )
{
    bmath_mf3_s_mul_scl_fx( &o->xx, weight, &o->xx );
    bmath_mf3_s_mul_scl_fx( &o->yx, weight, &o->yx );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_estimator_s_get_matrix( const bmath_estimator_s* o, bmath_mf3_s* mat )
{
    bmath_mf3_s* xx_piv = bmath_mf3_s_create();
    bmath_mf3_s_set_size_to( &o->xx, xx_piv );
    bmath_mf3_s_hsm_piv( &o->xx, o->eps, xx_piv );
    bmath_mf3_s_set_size( mat, o->yx.rows, o->yx.cols );
    bmath_mf3_s_mul_htp( &o->yx, xx_piv, mat );
    bmath_mf3_s_discard( xx_piv );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

static vd_t selftest( void )
{
    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( bmath_estimator_s, est );
    BCORE_LIFE_CREATE( bmath_mf3_s, m1 );
    BCORE_LIFE_CREATE( bmath_mf3_s, m2 );
//    BCORE_LIFE_CREATE( bmath_mf3_s, m3 );
    BCORE_LIFE_CREATE( bmath_vf3_s, v1 );
    BCORE_LIFE_CREATE( bmath_vf3_s, v2 );
    BCORE_LIFE_CREATE( bmath_vf3_s, v3 );
    BCORE_LIFE_CREATE( bmath_vf3_s, v4 );

    {
        u2_t rval = 1236;
        uz_t n = 40;
        uz_t m = 30;
        uz_t samples = 2000;

        bmath_mf3_s_set_size( m1, m, n + 1 );
        bmath_mf3_s_set_random( m1, false, false, 0, 1.0, -1, 1, &rval );

        bmath_vf3_s_set_size( v1, n );
        bmath_vf3_s_set_size( v2, m );
        bmath_vf3_s_set_size( v3, m );
        bmath_vf3_s_set_size( v4, m );
        bmath_estimator_s_clear( est );

        for( uz_t i = 0; i < samples; i++ )
        {
            bmath_vf3_s_set_random( v1, 1.0, -1, 1, &rval );
            bmath_mf3_s_mul_av1( m1, v1, v2 );

            // noisify the result a bit
            bmath_vf3_s_set_random( v3, 1.0, -0.1, 0.1, &rval );
            bmath_vf3_s_add( v2, v3, v4 );

            bmath_estimator_s_digest( est, 1.0, v1, v4 );
            bmath_estimator_s_reweight( est, 0.999 );
        }

        bmath_estimator_s_get_matrix( est, m2 );
        ASSERT( bmath_mf3_s_is_near_equ( m1, m2, 1E-2 ) );
    }
//    bcore_txt_ml_a_to_stdout( p );

    BCORE_LIFE_RETURNV( vd_t, NULL );
}

/**********************************************************************************************************************/

vd_t bmath_estimator_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_estimator" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_OBJECT( bmath_estimator_s );
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

