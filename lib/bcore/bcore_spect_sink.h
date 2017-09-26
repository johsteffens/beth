/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#ifndef BCORE_SPECT_SINK_H
#define BCORE_SPECT_SINK_H

#include "bcore_first.h"
#include "bcore_types.h"
#include "bcore_features.h"
#include "bcore_flect.h"

/// optional sink features:
typedef void (*bcore_sink_fp_set_consumer)( vd_t o, vd_t consumer );  // to create a chain of sink units
typedef void (*bcore_sink_fp_flush       )( vd_t o                );  // flushes buffers

typedef struct bcore_sink_s bcore_sink_s;
typedef struct bcore_sink_s
{
    aware_t p_type; // type of perspective
    tp_t    o_type; // type of object

    bcore_fp_flow_snk          flow_snk;
    bcore_sink_fp_set_consumer fp_set_consumer;
    bcore_sink_fp_flush        fp_flush;
} bcore_sink_s;

const bcore_sink_s* bcore_sink_s_get_typed( tp_t type );
const bcore_sink_s* bcore_sink_s_get_aware( vc_t obj );

sz_t bcore_sink_spect_push_data    ( const bcore_sink_s* p, vd_t o, vc_t data, sz_t size );
void bcore_sink_spect_flush        ( const bcore_sink_s* p, vd_t o );
void bcore_sink_spect_pushvf       ( const bcore_sink_s* p, vd_t o, sc_t format, va_list args );
void bcore_sink_spect_pushf        ( const bcore_sink_s* p, vd_t o, sc_t format, ... );
void bcore_sink_spect_push_char    ( const bcore_sink_s* p, vd_t o, char c );
void bcore_sink_spect_push_sc      ( const bcore_sink_s* p, vd_t o, sc_t sc );
void bcore_sink_spect_push_string  ( const bcore_sink_s* p, vd_t o, const bcore_string_s* string );
void bcore_sink_spect_push_string_d( const bcore_sink_s* p, vd_t o,       bcore_string_s* string );
void bcore_sink_spect_set_consumer ( const bcore_sink_s* p, vd_t o, vd_t consumer ); // error when not supported

sz_t bcore_sink_aware_push_data    ( vd_t o, vc_t data, sz_t size );
void bcore_sink_aware_flush        ( vd_t o );
void bcore_sink_aware_pushvf       ( vd_t o, sc_t format, va_list args );
void bcore_sink_aware_pushf        ( vd_t o, sc_t format, ... );
void bcore_sink_aware_push_char    ( vd_t o, char c );
void bcore_sink_aware_push_sc      ( vd_t o, sc_t sc );
void bcore_sink_aware_push_string  ( vd_t o, const bcore_string_s* string );
void bcore_sink_aware_push_string_d( vd_t o,       bcore_string_s* string );
void bcore_sink_aware_set_consumer ( vd_t o, vd_t consumer ); // error when not supported

sz_t bcore_sink_push_data    ( sr_s o, vc_t data, sz_t size );
void bcore_sink_flush        ( sr_s o );
void bcore_sink_pushvf       ( sr_s o, sc_t format, va_list args );
void bcore_sink_pushf        ( sr_s o, sc_t format, ... );
void bcore_sink_push_char    ( sr_s o, char c );
void bcore_sink_push_sc      ( sr_s o, sc_t sc );
void bcore_sink_push_string  ( sr_s o, const bcore_string_s* string );
void bcore_sink_push_string_d( sr_s o,       bcore_string_s* string );
void bcore_sink_set_consumer ( sr_s o, vd_t consumer ); // error when not supported

sz_t bcore_sink_q_push_data    ( const sr_s* o, vc_t data, sz_t size );
void bcore_sink_q_flush        ( const sr_s* o );
void bcore_sink_q_pushvf       ( const sr_s* o, sc_t format, va_list args );
void bcore_sink_q_pushf        ( const sr_s* o, sc_t format, ... );
void bcore_sink_q_push_char    ( const sr_s* o, char c );
void bcore_sink_q_push_sc      ( const sr_s* o, sc_t sc );
void bcore_sink_q_push_string  ( const sr_s* o, const bcore_string_s* string );
void bcore_sink_q_push_string_d( const sr_s* o,       bcore_string_s* string );
void bcore_sink_q_set_consumer ( const sr_s* o, vd_t consumer ); // error when not supported

vd_t bcore_spect_sink_signal( tp_t target, tp_t signal, vd_t object );

#endif // BCORE_SPECT_SINK_H
