/** This file was generated by the beth-precode-compiler. Not suitable for manual editing.
 *
 *  Copyright and License:
 *    If a specification exists for the repository or folder in which the file is located,
 *    that specification shall apply. Otherwise, the Copyright and License of the associated
 *    source code files containing beth-precode shall apply.
 */

#include "bmath_precoded.h"
#include "bcore_spect.h"
#include "bcore_spect_inst.h"
#include "bcore_sr.h"
#include "bcore_const_manager.h"


/**********************************************************************************************************************/
/// source: bmath_mf3_sx
#include "bmath_mf3_sx.h"

//----------------------------------------------------------------------------------------------------------------------
/// group: bmath_mf3_sx

BCORE_DEFINE_OBJECT_INST( bcore_inst, bmath_mf3_sx_s )\
  "{ aware_t _; sz_t slos; sz_t xons; sz_t rows; sz_t i_stride; sz_t [] i; f3_t [] v; }";

/**********************************************************************************************************************/
/// source: bmath_adaptive
#include "bmath_adaptive.h"

//----------------------------------------------------------------------------------------------------------------------
/// group: bmath_adaptive

#define TYPEOF_bmath_adaptive_s 2435779224
BCORE_DECLARE_SPECT( bmath_adaptive )
{
    bcore_spect_header_s header;
    bmath_adaptive_get_in_size get_in_size;
    bmath_adaptive_set_in_size set_in_size;
    bmath_adaptive_get_out_size get_out_size;
    bmath_adaptive_set_out_size set_out_size;
    bmath_adaptive_get_step get_step;
    bmath_adaptive_set_step set_step;
    bmath_adaptive_get_decay get_decay;
    bmath_adaptive_set_decay set_decay;
    bmath_adaptive_setup setup;
    bmath_adaptive_set_untrained set_untrained;
    bmath_adaptive_arc_to_sink arc_to_sink;
    bmath_adaptive_query query;
    bmath_adaptive_adapt adapt;
};
BCORE_DEFINE_SPECT( bcore_inst, bmath_adaptive )
"{"
    "bcore_spect_header_s header;"
    "feature bmath_adaptive : get_in_size = bmath_adaptive_get_in_size_default;"
    "feature bmath_adaptive : set_in_size = bmath_adaptive_set_in_size_default;"
    "feature bmath_adaptive : get_out_size = bmath_adaptive_get_out_size_default;"
    "feature bmath_adaptive : set_out_size = bmath_adaptive_set_out_size_default;"
    "feature bmath_adaptive : get_step = bmath_adaptive_get_step_default;"
    "feature bmath_adaptive : set_step = bmath_adaptive_set_step_default;"
    "feature bmath_adaptive : get_decay = bmath_adaptive_get_decay_default;"
    "feature bmath_adaptive : set_decay = bmath_adaptive_set_decay_default;"
    "feature bmath_adaptive : setup = bmath_adaptive_setup_default;"
    "feature bmath_adaptive : set_untrained = bmath_adaptive_set_untrained_default;"
    "feature bmath_adaptive : arc_to_sink = bmath_adaptive_arc_to_sink_default;"
    "feature bmath_adaptive : query = bmath_adaptive_query_default;"
    "feature bmath_adaptive : adapt = bmath_adaptive_adapt_default;"
"}";

static sz_t bmath_adaptive_get_in_size_default( const bmath_adaptive* o )
{
    ERR_fa( "Feature has no binding." );
    return *(sz_t*)NULL;
}

sz_t bmath_adaptive_t_get_in_size( tp_t __t, const bmath_adaptive* o )
{
    return bmath_adaptive_s_get_typed( __t )->get_in_size( o );
}

sz_t bmath_adaptive_a_get_in_size( const bmath_adaptive* o )
{
    return bmath_adaptive_s_get_aware( o )->get_in_size( o );
}

sz_t bmath_adaptive_r_get_in_size( const sr_s* o )
{
    return ( (bmath_adaptive_s*)ch_spect_p( o->p, TYPEOF_bmath_adaptive_s ) )->get_in_size( o->o );
}

static void bmath_adaptive_set_in_size_default( bmath_adaptive* o, sz_t size )
{
    ERR_fa( "Feature has no binding." );
}

void bmath_adaptive_t_set_in_size( tp_t __t, bmath_adaptive* o, sz_t size )
{
    bmath_adaptive_s_get_typed( __t )->set_in_size( o, size );
}

void bmath_adaptive_a_set_in_size( bmath_adaptive* o, sz_t size )
{
    bmath_adaptive_s_get_aware( o )->set_in_size( o, size );
}

void bmath_adaptive_r_set_in_size( const sr_s* o, sz_t size )
{
    ASSERT( !sr_s_is_const( o ) );
    ( (bmath_adaptive_s*)ch_spect_p( o->p, TYPEOF_bmath_adaptive_s ) )->set_in_size( o->o, size );
}

static sz_t bmath_adaptive_get_out_size_default( const bmath_adaptive* o )
{
    ERR_fa( "Feature has no binding." );
    return *(sz_t*)NULL;
}

sz_t bmath_adaptive_t_get_out_size( tp_t __t, const bmath_adaptive* o )
{
    return bmath_adaptive_s_get_typed( __t )->get_out_size( o );
}

sz_t bmath_adaptive_a_get_out_size( const bmath_adaptive* o )
{
    return bmath_adaptive_s_get_aware( o )->get_out_size( o );
}

sz_t bmath_adaptive_r_get_out_size( const sr_s* o )
{
    return ( (bmath_adaptive_s*)ch_spect_p( o->p, TYPEOF_bmath_adaptive_s ) )->get_out_size( o->o );
}

static void bmath_adaptive_set_out_size_default( bmath_adaptive* o, sz_t size )
{
    ERR_fa( "Feature has no binding." );
}

void bmath_adaptive_t_set_out_size( tp_t __t, bmath_adaptive* o, sz_t size )
{
    bmath_adaptive_s_get_typed( __t )->set_out_size( o, size );
}

void bmath_adaptive_a_set_out_size( bmath_adaptive* o, sz_t size )
{
    bmath_adaptive_s_get_aware( o )->set_out_size( o, size );
}

void bmath_adaptive_r_set_out_size( const sr_s* o, sz_t size )
{
    ASSERT( !sr_s_is_const( o ) );
    ( (bmath_adaptive_s*)ch_spect_p( o->p, TYPEOF_bmath_adaptive_s ) )->set_out_size( o->o, size );
}

static f3_t bmath_adaptive_get_step_default( const bmath_adaptive* o )
{
    ERR_fa( "Feature has no binding." );
    return *(f3_t*)NULL;
}

f3_t bmath_adaptive_t_get_step( tp_t __t, const bmath_adaptive* o )
{
    return bmath_adaptive_s_get_typed( __t )->get_step( o );
}

f3_t bmath_adaptive_a_get_step( const bmath_adaptive* o )
{
    return bmath_adaptive_s_get_aware( o )->get_step( o );
}

f3_t bmath_adaptive_r_get_step( const sr_s* o )
{
    return ( (bmath_adaptive_s*)ch_spect_p( o->p, TYPEOF_bmath_adaptive_s ) )->get_step( o->o );
}

static void bmath_adaptive_set_step_default( bmath_adaptive* o, f3_t val )
{
    ERR_fa( "Feature has no binding." );
}

void bmath_adaptive_t_set_step( tp_t __t, bmath_adaptive* o, f3_t val )
{
    bmath_adaptive_s_get_typed( __t )->set_step( o, val );
}

void bmath_adaptive_a_set_step( bmath_adaptive* o, f3_t val )
{
    bmath_adaptive_s_get_aware( o )->set_step( o, val );
}

void bmath_adaptive_r_set_step( const sr_s* o, f3_t val )
{
    ASSERT( !sr_s_is_const( o ) );
    ( (bmath_adaptive_s*)ch_spect_p( o->p, TYPEOF_bmath_adaptive_s ) )->set_step( o->o, val );
}

static f3_t bmath_adaptive_get_decay_default( const bmath_adaptive* o )
{
    ERR_fa( "Feature has no binding." );
    return *(f3_t*)NULL;
}

f3_t bmath_adaptive_t_get_decay( tp_t __t, const bmath_adaptive* o )
{
    return bmath_adaptive_s_get_typed( __t )->get_decay( o );
}

f3_t bmath_adaptive_a_get_decay( const bmath_adaptive* o )
{
    return bmath_adaptive_s_get_aware( o )->get_decay( o );
}

f3_t bmath_adaptive_r_get_decay( const sr_s* o )
{
    return ( (bmath_adaptive_s*)ch_spect_p( o->p, TYPEOF_bmath_adaptive_s ) )->get_decay( o->o );
}

static void bmath_adaptive_set_decay_default( bmath_adaptive* o, f3_t val )
{
    ERR_fa( "Feature has no binding." );
}

void bmath_adaptive_t_set_decay( tp_t __t, bmath_adaptive* o, f3_t val )
{
    bmath_adaptive_s_get_typed( __t )->set_decay( o, val );
}

void bmath_adaptive_a_set_decay( bmath_adaptive* o, f3_t val )
{
    bmath_adaptive_s_get_aware( o )->set_decay( o, val );
}

void bmath_adaptive_r_set_decay( const sr_s* o, f3_t val )
{
    ASSERT( !sr_s_is_const( o ) );
    ( (bmath_adaptive_s*)ch_spect_p( o->p, TYPEOF_bmath_adaptive_s ) )->set_decay( o->o, val );
}

static void bmath_adaptive_setup_default( bmath_adaptive* o, bl_t training )
{
    ERR_fa( "Feature has no binding." );
}

void bmath_adaptive_t_setup( tp_t __t, bmath_adaptive* o, bl_t training )
{
    bmath_adaptive_s_get_typed( __t )->setup( o, training );
}

void bmath_adaptive_a_setup( bmath_adaptive* o, bl_t training )
{
    bmath_adaptive_s_get_aware( o )->setup( o, training );
}

void bmath_adaptive_r_setup( const sr_s* o, bl_t training )
{
    ASSERT( !sr_s_is_const( o ) );
    ( (bmath_adaptive_s*)ch_spect_p( o->p, TYPEOF_bmath_adaptive_s ) )->setup( o->o, training );
}

static void bmath_adaptive_set_untrained_default( bmath_adaptive* o )
{
    ERR_fa( "Feature has no binding." );
}

void bmath_adaptive_t_set_untrained( tp_t __t, bmath_adaptive* o )
{
    bmath_adaptive_s_get_typed( __t )->set_untrained( o );
}

void bmath_adaptive_a_set_untrained( bmath_adaptive* o )
{
    bmath_adaptive_s_get_aware( o )->set_untrained( o );
}

void bmath_adaptive_r_set_untrained( const sr_s* o )
{
    ASSERT( !sr_s_is_const( o ) );
    ( (bmath_adaptive_s*)ch_spect_p( o->p, TYPEOF_bmath_adaptive_s ) )->set_untrained( o->o );
}

static void bmath_adaptive_arc_to_sink_default( const bmath_adaptive* o, bcore_sink* sink )
{
    ERR_fa( "Feature has no binding." );
}

void bmath_adaptive_t_arc_to_sink( tp_t __t, const bmath_adaptive* o, bcore_sink* sink )
{
    bmath_adaptive_s_get_typed( __t )->arc_to_sink( o, sink );
}

void bmath_adaptive_a_arc_to_sink( const bmath_adaptive* o, bcore_sink* sink )
{
    bmath_adaptive_s_get_aware( o )->arc_to_sink( o, sink );
}

void bmath_adaptive_r_arc_to_sink( const sr_s* o, bcore_sink* sink )
{
    ( (bmath_adaptive_s*)ch_spect_p( o->p, TYPEOF_bmath_adaptive_s ) )->arc_to_sink( o->o, sink );
}

static void bmath_adaptive_query_default( bmath_adaptive* o, const bmath_vf3_s* in, bmath_vf3_s* out )
{
    ERR_fa( "Feature has no binding." );
}

void bmath_adaptive_t_query( tp_t __t, bmath_adaptive* o, const bmath_vf3_s* in, bmath_vf3_s* out )
{
    bmath_adaptive_s_get_typed( __t )->query( o, in, out );
}

void bmath_adaptive_a_query( bmath_adaptive* o, const bmath_vf3_s* in, bmath_vf3_s* out )
{
    bmath_adaptive_s_get_aware( o )->query( o, in, out );
}

void bmath_adaptive_r_query( const sr_s* o, const bmath_vf3_s* in, bmath_vf3_s* out )
{
    ASSERT( !sr_s_is_const( o ) );
    ( (bmath_adaptive_s*)ch_spect_p( o->p, TYPEOF_bmath_adaptive_s ) )->query( o->o, in, out );
}

static void bmath_adaptive_adapt_default( bmath_adaptive* o, const bmath_vf3_s* in, const bmath_vf3_s* target, bmath_vf3_s* out )
{
    ERR_fa( "Feature has no binding." );
}

void bmath_adaptive_t_adapt( tp_t __t, bmath_adaptive* o, const bmath_vf3_s* in, const bmath_vf3_s* target, bmath_vf3_s* out )
{
    bmath_adaptive_s_get_typed( __t )->adapt( o, in, target, out );
}

void bmath_adaptive_a_adapt( bmath_adaptive* o, const bmath_vf3_s* in, const bmath_vf3_s* target, bmath_vf3_s* out )
{
    bmath_adaptive_s_get_aware( o )->adapt( o, in, target, out );
}

void bmath_adaptive_r_adapt( const sr_s* o, const bmath_vf3_s* in, const bmath_vf3_s* target, bmath_vf3_s* out )
{
    ASSERT( !sr_s_is_const( o ) );
    ( (bmath_adaptive_s*)ch_spect_p( o->p, TYPEOF_bmath_adaptive_s ) )->adapt( o->o, in, target, out );
}

/**********************************************************************************************************************/

vd_t bmath_precoded_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_precoded" ) ) )
    {
        case TYPEOF_init1:
        {
            // Comment or remove line below to rebuild this target.
            bcore_const_x_set_d( typeof( "bmath_precoded_hash" ), sr_tp( 2248594911 ) );
            BCORE_REGISTER_OBJECT( bmath_mf3_sx_s );
            BCORE_REGISTER_FEATURE( bmath_adaptive_get_in_size );
            BCORE_REGISTER_FEATURE( bmath_adaptive_set_in_size );
            BCORE_REGISTER_FEATURE( bmath_adaptive_get_out_size );
            BCORE_REGISTER_FEATURE( bmath_adaptive_set_out_size );
            BCORE_REGISTER_FEATURE( bmath_adaptive_get_step );
            BCORE_REGISTER_FEATURE( bmath_adaptive_set_step );
            BCORE_REGISTER_FEATURE( bmath_adaptive_get_decay );
            BCORE_REGISTER_FEATURE( bmath_adaptive_set_decay );
            BCORE_REGISTER_FEATURE( bmath_adaptive_setup );
            BCORE_REGISTER_FEATURE( bmath_adaptive_set_untrained );
            BCORE_REGISTER_FEATURE( bmath_adaptive_arc_to_sink );
            BCORE_REGISTER_FEATURE( bmath_adaptive_query );
            BCORE_REGISTER_FEATURE( bmath_adaptive_adapt );
            BCORE_REGISTER_FFUNC( bmath_adaptive_get_in_size, bmath_adaptive_get_in_size_default );
            BCORE_REGISTER_FFUNC( bmath_adaptive_set_in_size, bmath_adaptive_set_in_size_default );
            BCORE_REGISTER_FFUNC( bmath_adaptive_get_out_size, bmath_adaptive_get_out_size_default );
            BCORE_REGISTER_FFUNC( bmath_adaptive_set_out_size, bmath_adaptive_set_out_size_default );
            BCORE_REGISTER_FFUNC( bmath_adaptive_get_step, bmath_adaptive_get_step_default );
            BCORE_REGISTER_FFUNC( bmath_adaptive_set_step, bmath_adaptive_set_step_default );
            BCORE_REGISTER_FFUNC( bmath_adaptive_get_decay, bmath_adaptive_get_decay_default );
            BCORE_REGISTER_FFUNC( bmath_adaptive_set_decay, bmath_adaptive_set_decay_default );
            BCORE_REGISTER_FFUNC( bmath_adaptive_setup, bmath_adaptive_setup_default );
            BCORE_REGISTER_FFUNC( bmath_adaptive_set_untrained, bmath_adaptive_set_untrained_default );
            BCORE_REGISTER_FFUNC( bmath_adaptive_arc_to_sink, bmath_adaptive_arc_to_sink_default );
            BCORE_REGISTER_FFUNC( bmath_adaptive_query, bmath_adaptive_query_default );
            BCORE_REGISTER_FFUNC( bmath_adaptive_adapt, bmath_adaptive_adapt_default );
            BCORE_REGISTER_SPECT( bmath_adaptive );
        }
        break;
        default: break;
    }
    return NULL;
}
