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

#include "bmath_spect_algebraic.h"
#include "bmath_quicktypes.h"


/**********************************************************************************************************************/
// bmath_group_s

#define NPXAG( name ) bmath_group_##name

BCORE_DEFINE_SPECT( bmath_group_s )
"{"
    "bcore_spect_header_s header;"
    "strict feature bcore_inst_s -> spect_inst;"
    "strict feature bmath_fp: add;"
    "strict feature bmath_fp: zro;"
    "strict feature bmath_fp: neg;"
    "       feature bmath_fp: sub;"
    "       feature bmath_fp: cpy;"
"}";


void bmath_group_default_sub( const bmath_group_s* p, vd_t o, vc_t op1, vc_t op2 )
{
    bmath_group_p_neg( p, o, op2 );
    bmath_group_p_add( p, o, op1, o );
}

void bmath_group_default_cpy( const bmath_group_s* p, vd_t o, vc_t op )
{
    bcore_inst_spect_copy( p->spect_inst, o, op );
}

/**********************************************************************************************************************/

// bmath_ring_s

#define NPXRG( name ) bmath_ring_##name

BCORE_DEFINE_SPECT( bmath_ring_s )
"{"
    "bcore_spect_header_s header;"
    "strict feature bcore_inst_s -> spect_inst;"

    "strict feature bmath_fp: add;"
    "strict feature bmath_fp: zro;"
    "strict feature bmath_fp: neg;"
    "       feature bmath_fp: sub;"
    "       feature bmath_fp: cpy;"

    "strict feature bmath_fp: mul;"
    "       feature bmath_fp: one;"
    "       feature bmath_fp: inv;"
    "       feature bmath_fp: div;"
"}";

void bmath_ring_default_sub( const bmath_ring_s* p, vd_t o, vc_t op1, vc_t op2 )
{
    bmath_ring_p_neg( p, o, op2 );
    bmath_ring_p_add( p, o, op1, o );
}

void bmath_ring_default_cpy( const bmath_ring_s* p, vd_t o, vc_t op )
{
    bcore_inst_spect_copy( p->spect_inst, o, op );
}

void bmath_ring_default_div( const bmath_ring_s* p, vd_t o, vc_t op1, vc_t op2 )
{
    bmath_ring_p_inv( p, o, op2 );
    bmath_ring_p_mul( p, o, op1, o );
}

/**********************************************************************************************************************/

static vd_t selftest( void )
{
    ASSERT( bcore_trait_satisfied_type( typeof( "bmath_group" ), TYPEOF_f3_t, NULL ) );

    const bmath_group_s* p_group = bmath_group_s_get_typed( TYPEOF_f3_t );

    f3_t v1 = 3;
    f3_t v2 = 2;
    f3_t v3 = 0;

    bmath_group_p_sub( p_group, &v3, &v1, &v2 );

    ASSERT( v3 == 1 );

    return NULL;
}

/**********************************************************************************************************************/

vd_t bmath_spect_algebraic_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_spect_algebraic" ) ) )
    {
        case TYPEOF_init1:
        {
            // features
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_add );
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_sub );
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_neg );
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_zro );
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_mul );
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_div );
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_inv );
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_one );
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_cpy );

            // reflections
            BCORE_REGISTER_SPECT( bmath_group_s );
            BCORE_REGISTER_SPECT( bmath_ring_s );
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

