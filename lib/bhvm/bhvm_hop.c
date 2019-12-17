/** Author and Copyright 2019 Johannes Bernhard Steffens
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

#include "bhvm_hop.h"

#ifdef TYPEOF_bhvm_hop

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

enum mul_type_s
{
    MUL_MMM,
    MUL_MTM,
    MUL_TMM,
    MUL_TTM,
    MUL_ACC_MMM,
    MUL_ACC_MTM,
    MUL_ACC_TMM,
    MUL_ACC_TTM,
};

static void bhvm_hop_ar2_mul( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r, enum mul_type_s mul_type )
{
    assert( a->s.size == 2 );
    assert( b->s.size == 2 );
    assert( r->s.size == 2 );
    assert( a->v.size == bhvm_shape_s_get_volume( &a->s ) );
    assert( b->v.size == bhvm_shape_s_get_volume( &b->s ) );
    assert( r->v.size == bhvm_shape_s_get_volume( &r->s ) );
    assert( a->v.type );
    assert( b->v.type );
    assert( r->v.type );

    BLM_INIT();

    vd_t vma = NULL;
    vd_t vmb = NULL;
    vd_t vmr = NULL;

    if( a->v.type == TYPEOF_f2_t )
    {
        bmath_mf2_s_init_weak( ( vma = BLM_CREATE( bmath_mf2_s ) ), a->s.data[1], a->s.data[0], a->s.data[0], a->v.data );
    }
    else
    {
        bmath_mf3_s_init_weak( ( vma = BLM_CREATE( bmath_mf3_s ) ), a->s.data[1], a->s.data[0], a->s.data[0], a->v.data );
    }

    if( b->v.type == TYPEOF_f2_t )
    {
        bmath_mf2_s_init_weak( ( vmb = BLM_CREATE( bmath_mf2_s ) ), b->s.data[1], b->s.data[0], b->s.data[0], b->v.data );
    }
    else
    {
        bmath_mf3_s_init_weak( ( vmb = BLM_CREATE( bmath_mf3_s ) ), b->s.data[1], b->s.data[0], b->s.data[0], b->v.data );
    }

    if( r->v.type == TYPEOF_f2_t )
    {
        bmath_mf2_s_init_weak( ( vmr = BLM_CREATE( bmath_mf2_s ) ), r->s.data[1], r->s.data[0], r->s.data[0], r->v.data );
    }
    else
    {
        bmath_mf3_s_init_weak( ( vmr = BLM_CREATE( bmath_mf3_s ) ), r->s.data[1], r->s.data[0], r->s.data[0], r->v.data );
    }

    if( a->v.type == TYPEOF_f2_t && b->v.type == TYPEOF_f2_t )
    {
        bmath_mf2_s* ma = vma;
        bmath_mf2_s* mb = vmb;
        bmath_mf2_s* mr = ( r->v.type == TYPEOF_f2_t ) ? vmr : BLM_A_PUSH( bmath_mf2_s_clone_a( vmr ) );
        switch( mul_type )
        {
            case MUL_MMM:     bmath_mf2_s_mul            ( ma, mb, mr ); break;
            case MUL_MTM:     bmath_mf2_s_mul_htp        ( ma, mb, mr ); break;
            case MUL_TMM:     bmath_mf2_s_htp_mul        ( ma, mb, mr ); break;
            case MUL_TTM:     bmath_mf2_s_htp_mul_htp    ( ma, mb, mr ); break;
            case MUL_ACC_MMM: bmath_mf2_s_mul_add        ( ma, mb, mr, mr ); break;
            case MUL_ACC_MTM: bmath_mf2_s_mul_htp_add    ( ma, mb, mr, mr ); break;
            case MUL_ACC_TMM: bmath_mf2_s_htp_mul_add    ( ma, mb, mr, mr ); break;
            case MUL_ACC_TTM: bmath_mf2_s_htp_mul_htp_add( ma, mb, mr, mr ); break;
            default: break;
        }
        if( r->v.type == TYPEOF_f3_t ) bmath_mf3_s_copy_a( vmr, mr );
    }
    else
    {
        bmath_mf3_s* ma = ( a->v.type == TYPEOF_f2_t ) ? BLM_A_PUSH( bmath_mf3_s_clone_a( vma ) ) : vma;
        bmath_mf3_s* mb = ( b->v.type == TYPEOF_f2_t ) ? BLM_A_PUSH( bmath_mf3_s_clone_a( vmb ) ) : vmb;
        bmath_mf3_s* mr = ( r->v.type == TYPEOF_f2_t ) ? BLM_A_PUSH( bmath_mf3_s_clone_a( vmr ) ) : vmr;
        switch( mul_type )
        {
            case MUL_MMM:     bmath_mf3_s_mul            ( ma, mb, mr ); break;
            case MUL_MTM:     bmath_mf3_s_mul_htp        ( ma, mb, mr ); break;
            case MUL_TMM:     bmath_mf3_s_htp_mul        ( ma, mb, mr ); break;
            case MUL_TTM:     bmath_mf3_s_htp_mul_htp    ( ma, mb, mr ); break;
            case MUL_ACC_MMM: bmath_mf3_s_mul_add        ( ma, mb, mr, mr ); break;
            case MUL_ACC_MTM: bmath_mf3_s_mul_htp_add    ( ma, mb, mr, mr ); break;
            case MUL_ACC_TMM: bmath_mf3_s_htp_mul_add    ( ma, mb, mr, mr ); break;
            case MUL_ACC_TTM: bmath_mf3_s_htp_mul_htp_add( ma, mb, mr, mr ); break;
            default: break;
        }
        if( r->v.type == TYPEOF_f2_t ) bmath_mf2_s_copy_a( vmr, mr );
    }

    BLM_DOWN();
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hop_ar2_mul_mmm_s_f(     const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r ) { bhvm_hop_ar2_mul( a, b, r, MUL_MMM ); }
void bhvm_hop_ar2_mul_mtm_s_f(     const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r ) { bhvm_hop_ar2_mul( a, b, r, MUL_MTM ); }
void bhvm_hop_ar2_mul_tmm_s_f(     const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r ) { bhvm_hop_ar2_mul( a, b, r, MUL_TMM ); }
void bhvm_hop_ar2_mul_ttm_s_f(     const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r ) { bhvm_hop_ar2_mul( a, b, r, MUL_TTM ); }
void bhvm_hop_ar2_mul_acc_mmm_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r ) { bhvm_hop_ar2_mul( a, b, r, MUL_ACC_MMM ); }
void bhvm_hop_ar2_mul_acc_mtm_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r ) { bhvm_hop_ar2_mul( a, b, r, MUL_ACC_MTM ); }
void bhvm_hop_ar2_mul_acc_tmm_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r ) { bhvm_hop_ar2_mul( a, b, r, MUL_ACC_TMM ); }
void bhvm_hop_ar2_mul_acc_ttm_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r ) { bhvm_hop_ar2_mul( a, b, r, MUL_ACC_TTM ); }

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

#endif // TYPEOF_bhvm_hop

vd_t bhvm_hop_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bhvm_hop" ) ) )
    {
        case TYPEOF_init1:
        {
        }
        break;

        case TYPEOF_get_quicktypes:
        {
        }
        break;

        case TYPEOF_selftest:
        {
        }
        break;

        case TYPEOF_plant:
        {
            bcore_plant_compile( "bhvm_planted", __FILE__ );
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

