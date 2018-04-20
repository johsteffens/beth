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
    "strict feature bmath_fp_add add ~> func bmath_fp_add add;"
    "strict feature bmath_fp_zro zro ~> func bmath_fp_zro zro;"
    "strict feature bmath_fp_neg neg ~> func bmath_fp_neg neg;"
    "       feature bmath_fp_sub sub ~> func bmath_fp_sub sub;"
    "       feature bmath_fp_cpy cpy ~> func bmath_fp_cpy cpy;"
"}";

void bmath_group_spect_add( const bmath_group_s* p, vd_t o, vc_t op1, vc_t op2 )
{
    p->fp_add( o, op1, op2 );
}

void bmath_group_spect_zro( const bmath_group_s* p, vd_t o )
{
    p->fp_zro( o );
}

void bmath_group_spect_neg( const bmath_group_s* p, vd_t o, vc_t op )
{
    p->fp_neg( o, op );
}

void bmath_group_spect_sub( const bmath_group_s* p, vd_t o, vc_t op1, vc_t op2 )
{
    if( p->fp_sub )
    {
        p->fp_sub( o, op1, op2 );
    }
    else
    {
        p->fp_neg( o, op2 );
        p->fp_add( o, op1, o );
    }
}

void bmath_group_spect_cpy( const bmath_group_s* p, vd_t o, vc_t op )
{
    if( p->fp_cpy )
    {
        p->fp_cpy( o, op );
    }
    else
    {
        bcore_inst_spect_copy( p->spect_inst, o, op );
    }
}

/**********************************************************************************************************************/

static inline const bmath_group_s* atpdag( tp_t tp ) { return bmath_group_s_get_typed( tp ); }
static inline vc_t w_spectag( sr_s o ) { if( sr_s_is_const( &o ) ) ERR( "Attempt to modify a constant object" ); return ch_spect_p( o.p, TYPEOF_bmath_group_s ); }

void NPXAG(typed_add)( tp_t t, vd_t o, vc_t op1, vc_t op2 ) { NPXAG(spect_add)( atpdag( t ), o, op1, op2 ); }
void NPXAG(typed_zro)( tp_t t, vd_t o                     ) { NPXAG(spect_zro)( atpdag( t ), o           ); }
void NPXAG(typed_neg)( tp_t t, vd_t o, vc_t op1           ) { NPXAG(spect_neg)( atpdag( t ), o, op1      ); }
void NPXAG(typed_sub)( tp_t t, vd_t o, vc_t op1, vc_t op2 ) { NPXAG(spect_sub)( atpdag( t ), o, op1, op2 ); }
void NPXAG(typed_cpy)( tp_t t, vd_t o, vc_t op1           ) { NPXAG(spect_cpy)( atpdag( t ), o, op1      ); }

void NPXAG(aware_add)(         vd_t o, vc_t op1, vc_t op2 ) { NPXAG(typed_add)( *(aware_t*)o, o, op1, op2 ); }
void NPXAG(aware_zro)(         vd_t o                     ) { NPXAG(typed_zro)( *(aware_t*)o, o           ); }
void NPXAG(aware_neg)(         vd_t o, vc_t op1           ) { NPXAG(typed_neg)( *(aware_t*)o, o, op1      ); }
void NPXAG(aware_sub)(         vd_t o, vc_t op1, vc_t op2 ) { NPXAG(typed_sub)( *(aware_t*)o, o, op1, op2 ); }
void NPXAG(aware_cpy)(         vd_t o, vc_t op1           ) { NPXAG(typed_cpy)( *(aware_t*)o, o, op1      ); }

void NPXAG(add      )(         sr_s o, vc_t op1, vc_t op2 ) { NPXAG(spect_add)( w_spectag( o ), o.o, op1, op2 ); sr_down( o ); }
void NPXAG(zro      )(         sr_s o                     ) { NPXAG(spect_zro)( w_spectag( o ), o.o           ); sr_down( o ); }
void NPXAG(neg      )(         sr_s o, vc_t op1           ) { NPXAG(spect_neg)( w_spectag( o ), o.o, op1      ); sr_down( o ); }
void NPXAG(sub      )(         sr_s o, vc_t op1, vc_t op2 ) { NPXAG(spect_sub)( w_spectag( o ), o.o, op1, op2 ); sr_down( o ); }
void NPXAG(cpy      )(         sr_s o, vc_t op1           ) { NPXAG(spect_cpy)( w_spectag( o ), o.o, op1      ); sr_down( o ); }

void NPXAG(q_add    )(  const sr_s* o, vc_t op1, vc_t op2 ) { NPXAG(spect_add)( w_spectag( *o ), o->o, op1, op2 ); }
void NPXAG(q_zro    )(  const sr_s* o                     ) { NPXAG(spect_zro)( w_spectag( *o ), o->o           ); }
void NPXAG(q_neg    )(  const sr_s* o, vc_t op1           ) { NPXAG(spect_neg)( w_spectag( *o ), o->o, op1      ); }
void NPXAG(q_sub    )(  const sr_s* o, vc_t op1, vc_t op2 ) { NPXAG(spect_sub)( w_spectag( *o ), o->o, op1, op2 ); }
void NPXAG(q_cpy    )(  const sr_s* o, vc_t op1           ) { NPXAG(spect_cpy)( w_spectag( *o ), o->o, op1      ); }

/**********************************************************************************************************************/
// bmath_ring_s

#define NPXRG( name ) bmath_ring_##name

BCORE_DEFINE_SPECT( bmath_ring_s )
"{"
    "bcore_spect_header_s header;"
    "strict feature bcore_inst_s -> spect_inst;"

    "strict feature bmath_fp_add add ~> func bmath_fp_add add;"
    "strict feature bmath_fp_zro zro ~> func bmath_fp_zro zro;"
    "strict feature bmath_fp_neg neg ~> func bmath_fp_neg neg;"
    "       feature bmath_fp_sub sub ~> func bmath_fp_sub sub;"
    "       feature bmath_fp_cpy cpy ~> func bmath_fp_cpy cpy;"

    "strict feature bmath_fp_mul mul ~> func bmath_fp_mul mul;"
    "       feature bmath_fp_one one ~> func bmath_fp_one one;"
    "       feature bmath_fp_inv inv ~> func bmath_fp_inv inv;"
    "       feature bmath_fp_div div ~> func bmath_fp_div div;"
"}";

void bmath_ring_spect_add( const bmath_ring_s* p, vd_t o, vc_t op1, vc_t op2 )
{
    p->fp_add( o, op1, op2 );
}

void bmath_ring_spect_zro( const bmath_ring_s* p, vd_t o )
{
    p->fp_zro( o );
}

void bmath_ring_spect_neg( const bmath_ring_s* p, vd_t o, vc_t op )
{
    p->fp_neg( o, op );
}

void bmath_ring_spect_sub( const bmath_ring_s* p, vd_t o, vc_t op1, vc_t op2 )
{
    if( p->fp_sub )
    {
        p->fp_sub( o, op1, op2 );
    }
    else
    {
        p->fp_neg( o, op2 );
        p->fp_add( o, op1, o );
    }
}

void bmath_ring_spect_cpy( const bmath_ring_s* p, vd_t o, vc_t op )
{
    if( p->fp_cpy )
    {
        p->fp_cpy( o, op );
    }
    else
    {
        bcore_inst_spect_copy( p->spect_inst, o, op );
    }
}

void bmath_ring_spect_mul( const bmath_ring_s* p, vd_t o, vc_t op1, vc_t op2 )
{
    p->fp_mul( o, op1, op2 );
}

void bmath_ring_spect_one( const bmath_ring_s* p, vd_t o )
{
    p->fp_one( o );
}

void bmath_ring_spect_inv( const bmath_ring_s* p, vd_t o, vc_t op )
{
    if( p->fp_inv )
    {
        p->fp_inv( o, op );
    }
    else
    {
        WRN_fa( "Inversion is not available for object '#<sc_t>'.", ifnameof( p->header.o_type ) );
    }
}

void bmath_ring_spect_div( const bmath_ring_s* p, vd_t o, vc_t op1, vc_t op2 )
{
    if( p->fp_div )
    {
        p->fp_div( o, op1, op2 );
    }
    else if( p->fp_inv )
    {
        p->fp_inv( o, op2 );
        p->fp_mul( o, op1, o );
    }
    else
    {
        WRN_fa( "Division is not available for object '#<sc_t>'.", ifnameof( p->header.o_type ) );
    }
}

/**********************************************************************************************************************/

static inline const bmath_ring_s* atpdrg( tp_t tp ) { return bmath_ring_s_get_typed( tp ); }
static inline vc_t w_spectrg( sr_s o ) { if( sr_s_is_const( &o ) ) ERR( "Attempt to modify a constant object" ); return ch_spect_p( o.p, TYPEOF_bmath_ring_s ); }

void NPXRG(typed_add)( tp_t t, vd_t o, vc_t op1, vc_t op2 ) { NPXRG(spect_add)( atpdrg( t ), o, op1, op2 ); }
void NPXRG(typed_zro)( tp_t t, vd_t o                     ) { NPXRG(spect_zro)( atpdrg( t ), o           ); }
void NPXRG(typed_neg)( tp_t t, vd_t o, vc_t op1           ) { NPXRG(spect_neg)( atpdrg( t ), o, op1      ); }
void NPXRG(typed_sub)( tp_t t, vd_t o, vc_t op1, vc_t op2 ) { NPXRG(spect_sub)( atpdrg( t ), o, op1, op2 ); }
void NPXRG(typed_cpy)( tp_t t, vd_t o, vc_t op1           ) { NPXRG(spect_cpy)( atpdrg( t ), o, op1      ); }
void NPXRG(typed_mul)( tp_t t, vd_t o, vc_t op1, vc_t op2 ) { NPXRG(spect_mul)( atpdrg( t ), o, op1, op2 ); }
void NPXRG(typed_one)( tp_t t, vd_t o                     ) { NPXRG(spect_one)( atpdrg( t ), o           ); }
void NPXRG(typed_inv)( tp_t t, vd_t o, vc_t op1           ) { NPXRG(spect_inv)( atpdrg( t ), o, op1      ); }
void NPXRG(typed_div)( tp_t t, vd_t o, vc_t op1, vc_t op2 ) { NPXRG(spect_div)( atpdrg( t ), o, op1, op2 ); }

void NPXRG(aware_add)(         vd_t o, vc_t op1, vc_t op2 ) { NPXRG(typed_add)( *(aware_t*)o, o, op1, op2 ); }
void NPXRG(aware_zro)(         vd_t o                     ) { NPXRG(typed_zro)( *(aware_t*)o, o           ); }
void NPXRG(aware_neg)(         vd_t o, vc_t op1           ) { NPXRG(typed_neg)( *(aware_t*)o, o, op1      ); }
void NPXRG(aware_sub)(         vd_t o, vc_t op1, vc_t op2 ) { NPXRG(typed_sub)( *(aware_t*)o, o, op1, op2 ); }
void NPXRG(aware_cpy)(         vd_t o, vc_t op1           ) { NPXRG(typed_cpy)( *(aware_t*)o, o, op1      ); }
void NPXRG(aware_mul)(         vd_t o, vc_t op1, vc_t op2 ) { NPXRG(typed_mul)( *(aware_t*)o, o, op1, op2 ); }
void NPXRG(aware_one)(         vd_t o                     ) { NPXRG(typed_one)( *(aware_t*)o, o           ); }
void NPXRG(aware_inv)(         vd_t o, vc_t op1           ) { NPXRG(typed_inv)( *(aware_t*)o, o, op1      ); }
void NPXRG(aware_div)(         vd_t o, vc_t op1, vc_t op2 ) { NPXRG(typed_div)( *(aware_t*)o, o, op1, op2 ); }

void NPXRG(add      )(         sr_s o, vc_t op1, vc_t op2 ) { NPXRG(spect_add)( w_spectrg( o ), o.o, op1, op2 ); sr_down( o ); }
void NPXRG(zro      )(         sr_s o                     ) { NPXRG(spect_zro)( w_spectrg( o ), o.o           ); sr_down( o ); }
void NPXRG(neg      )(         sr_s o, vc_t op1           ) { NPXRG(spect_neg)( w_spectrg( o ), o.o, op1      ); sr_down( o ); }
void NPXRG(sub      )(         sr_s o, vc_t op1, vc_t op2 ) { NPXRG(spect_sub)( w_spectrg( o ), o.o, op1, op2 ); sr_down( o ); }
void NPXRG(cpy      )(         sr_s o, vc_t op1           ) { NPXRG(spect_cpy)( w_spectrg( o ), o.o, op1      ); sr_down( o ); }
void NPXRG(mul      )(         sr_s o, vc_t op1, vc_t op2 ) { NPXRG(spect_mul)( w_spectrg( o ), o.o, op1, op2 ); sr_down( o ); }
void NPXRG(one      )(         sr_s o                     ) { NPXRG(spect_one)( w_spectrg( o ), o.o           ); sr_down( o ); }
void NPXRG(inv      )(         sr_s o, vc_t op1           ) { NPXRG(spect_inv)( w_spectrg( o ), o.o, op1      ); sr_down( o ); }
void NPXRG(div      )(         sr_s o, vc_t op1, vc_t op2 ) { NPXRG(spect_div)( w_spectrg( o ), o.o, op1, op2 ); sr_down( o ); }

void NPXRG(q_add    )(  const sr_s* o, vc_t op1, vc_t op2 ) { NPXRG(spect_add)( w_spectrg( *o ), o->o, op1, op2 ); }
void NPXRG(q_zro    )(  const sr_s* o                     ) { NPXRG(spect_zro)( w_spectrg( *o ), o->o           ); }
void NPXRG(q_neg    )(  const sr_s* o, vc_t op1           ) { NPXRG(spect_neg)( w_spectrg( *o ), o->o, op1      ); }
void NPXRG(q_sub    )(  const sr_s* o, vc_t op1, vc_t op2 ) { NPXRG(spect_sub)( w_spectrg( *o ), o->o, op1, op2 ); }
void NPXRG(q_cpy    )(  const sr_s* o, vc_t op1           ) { NPXRG(spect_cpy)( w_spectrg( *o ), o->o, op1      ); }
void NPXRG(q_mul    )(  const sr_s* o, vc_t op1, vc_t op2 ) { NPXRG(spect_mul)( w_spectrg( *o ), o->o, op1, op2 ); }
void NPXRG(q_one    )(  const sr_s* o                     ) { NPXRG(spect_one)( w_spectrg( *o ), o->o           ); }
void NPXRG(q_inv    )(  const sr_s* o, vc_t op1           ) { NPXRG(spect_inv)( w_spectrg( *o ), o->o, op1      ); }
void NPXRG(q_div    )(  const sr_s* o, vc_t op1, vc_t op2 ) { NPXRG(spect_div)( w_spectrg( *o ), o->o, op1, op2 ); }

/**********************************************************************************************************************/

static vd_t selftest( void )
{
    ASSERT( bcore_trait_satisfied_type( typeof( "bmath_group" ), TYPEOF_f3_t, NULL ) );

    const bmath_group_s* p_group = bmath_group_s_get_typed( TYPEOF_f3_t );

    f3_t v1 = 3;
    f3_t v2 = 2;
    f3_t v3 = 0;

    bmath_group_spect_sub( p_group, &v3, &v1, &v2 );

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

