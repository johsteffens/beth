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

    vd_t vma = BLM_A_PUSH( bhvm_holor_s_mfx_create_weak( a ) );
    vd_t vmb = BLM_A_PUSH( bhvm_holor_s_mfx_create_weak( b ) );
    vd_t vmr = BLM_A_PUSH( bhvm_holor_s_mfx_create_weak( r ) );

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
        if( r->v.type == TYPEOF_f3_t )
        {
            bhvm_lop_ar1_cpy_s_f( BKNIT_FA2( TYPEOF_f2_t, TYPEOF_f3_t ), mr->data, r->v.data, mr->rows * mr->cols );
        }
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
        if( r->v.type == TYPEOF_f2_t )
        {
            bhvm_lop_ar1_cpy_s_f( BKNIT_FA2( TYPEOF_f3_t, TYPEOF_f2_t ), mr->data, r->v.data, mr->rows * mr->cols );
        }
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

void bhvm_hop_ar3_branch_svvv_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, const bhvm_holor_s* c, bhvm_holor_s* r )
{
    assert( a->v.size == 1 );
    bl_t flag = ( a->v.type == TYPEOF_f2_t ) ? ( ( ( f2_t* )a->v.data )[ 0 ] > 0 ) : ( ( ( f3_t* )a->v.data )[ 0 ] > 0 );
    if( flag )
    {
        bhvm_hop_ar1_cpy_s_f( b, r );
    }
    else
    {
        bhvm_hop_ar1_cpy_s_f( c, r );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hop_ar1_softmax_s_f( const bhvm_holor_s* a, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    sz_t size = a->v.size;
    f3_t max = bhvm_value_s_get_max_f3( &a->v );
    f3_t sum = 0;

    vc_t ap = a->v.data;
    vd_t rp = r->v.data;

    switch( BKNIT_FA2( a->v.type, r->v.type ) )
    {
        case BKNIT_F22: for( sz_t i = 0; i < size; i++ ) { f3_t v = f3_exp( ( ( f2_t* )ap )[ i ] - max ); sum += v; ( ( f2_t* )rp )[ i ] = v; } break;
        case BKNIT_F23: for( sz_t i = 0; i < size; i++ ) { f3_t v = f3_exp( ( ( f2_t* )ap )[ i ] - max ); sum += v; ( ( f3_t* )rp )[ i ] = v; } break;
        case BKNIT_F32: for( sz_t i = 0; i < size; i++ ) { f3_t v = f3_exp( ( ( f3_t* )ap )[ i ] - max ); sum += v; ( ( f2_t* )rp )[ i ] = v; } break;
        case BKNIT_F33: for( sz_t i = 0; i < size; i++ ) { f3_t v = f3_exp( ( ( f3_t* )ap )[ i ] - max ); sum += v; ( ( f3_t* )rp )[ i ] = v; } break;
        default: BKNIT_FA2_ERR( a->v.type, r->v.type ); break;
    }

    f3_t inv = f3_inv( sum );

    switch( BKNIT_FA1( r->v.type ) )
    {
        case BKNIT_F2: for( sz_t i = 0; i < size; i++ ) { ( ( f2_t* )rp )[ i ] *= inv; } break;
        case BKNIT_F3: for( sz_t i = 0; i < size; i++ ) { ( ( f3_t* )rp )[ i ] *= inv; } break;
        default: break;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hop_ar2_softmax_dp_zyf_s_f( const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r )
{
    assert( a->v.size == r->v.size );
    assert( b->v.size == r->v.size );
    sz_t size = a->v.size;

    vc_t ap = a->v.data;
    vc_t bp = b->v.data;
    vd_t rp = r->v.data;

    f3_t dpd = 0;

    switch( BKNIT_FA2( a->v.type, b->v.type ) )
    {
        case BKNIT_F22: for( sz_t i = 0; i < size; i++ ) { dpd += ( ( f2_t* )ap )[ i ] * ( ( f2_t* )bp )[ i ]; } break;
        case BKNIT_F23: for( sz_t i = 0; i < size; i++ ) { dpd += ( ( f2_t* )ap )[ i ] * ( ( f3_t* )bp )[ i ]; } break;
        case BKNIT_F32: for( sz_t i = 0; i < size; i++ ) { dpd += ( ( f3_t* )ap )[ i ] * ( ( f2_t* )bp )[ i ]; } break;
        case BKNIT_F33: for( sz_t i = 0; i < size; i++ ) { dpd += ( ( f3_t* )ap )[ i ] * ( ( f3_t* )bp )[ i ]; } break;
        default: BKNIT_FA2_ERR( a->v.type, b->v.type ); break;
    }

    switch( BKNIT_FA3( r->v.type, a->v.type, b->v.type ) )
    {
        case BKNIT_F222: for( sz_t i = 0; i < size; i++ ) { ( ( f2_t* )rp )[ i ] += ( ( ( f2_t* )ap )[ i ] - dpd ) * ( ( f2_t* )bp )[ i ]; } break;
        case BKNIT_F223: for( sz_t i = 0; i < size; i++ ) { ( ( f2_t* )rp )[ i ] += ( ( ( f2_t* )ap )[ i ] - dpd ) * ( ( f3_t* )bp )[ i ]; } break;
        case BKNIT_F232: for( sz_t i = 0; i < size; i++ ) { ( ( f2_t* )rp )[ i ] += ( ( ( f3_t* )ap )[ i ] - dpd ) * ( ( f2_t* )bp )[ i ]; } break;
        case BKNIT_F233: for( sz_t i = 0; i < size; i++ ) { ( ( f2_t* )rp )[ i ] += ( ( ( f3_t* )ap )[ i ] - dpd ) * ( ( f3_t* )bp )[ i ]; } break;
        case BKNIT_F322: for( sz_t i = 0; i < size; i++ ) { ( ( f3_t* )rp )[ i ] += ( ( ( f2_t* )ap )[ i ] - dpd ) * ( ( f2_t* )bp )[ i ]; } break;
        case BKNIT_F323: for( sz_t i = 0; i < size; i++ ) { ( ( f3_t* )rp )[ i ] += ( ( ( f2_t* )ap )[ i ] - dpd ) * ( ( f3_t* )bp )[ i ]; } break;
        case BKNIT_F332: for( sz_t i = 0; i < size; i++ ) { ( ( f3_t* )rp )[ i ] += ( ( ( f3_t* )ap )[ i ] - dpd ) * ( ( f2_t* )bp )[ i ]; } break;
        case BKNIT_F333: for( sz_t i = 0; i < size; i++ ) { ( ( f3_t* )rp )[ i ] += ( ( ( f3_t* )ap )[ i ] - dpd ) * ( ( f3_t* )bp )[ i ]; } break;
        default: BKNIT_FA3_ERR( r->v.type, a->v.type, b->v.type ); break;
    }
}

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

