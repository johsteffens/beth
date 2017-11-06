/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#ifndef BCORE_SPECT_SOURCE_H
#define BCORE_SPECT_SOURCE_H

#include "bcore_first.h"
#include "bcore_types.h"
#include "bcore_features.h"
#include "bcore_flect.h"

/// optional source features:
typedef void (*bcore_source_fp_parse_fv )( vd_t o, sc_t format, va_list args ); // parse function
typedef void (*bcore_source_fp_set_supplier)( vd_t o, vd_t supplier );         // to create a chain of source units

/// required source features:
/// awareness

typedef struct bcore_source_s bcore_source_s;
typedef struct bcore_source_s
{
    aware_t p_type; // type of perspective
    tp_t    o_type; // type of object

    bcore_fp_flow_src            fp_flow_src;
    bcore_fp_logvf               fp_parse_errvf;
    bcore_source_fp_parse_fv      fp_parse_fv;
    bcore_source_fp_set_supplier fp_set_supplier;

} bcore_source_s;

const bcore_source_s* bcore_source_s_get_typed( tp_t type );
const bcore_source_s* bcore_source_s_get_aware( vc_t obj );

sz_t bcore_source_spect_get_data(     const bcore_source_s* p, vd_t o, vd_t data, sz_t size );
void bcore_source_spect_parse_fv(     const bcore_source_s* p, vd_t o, sc_t format, va_list args );
void bcore_source_spect_parse_fa(     const bcore_source_s* p, vd_t o, sc_t format, ... );
void bcore_source_spect_parse_errvf(  const bcore_source_s* p, vd_t o, sc_t format, va_list args );
void bcore_source_spect_parse_errf(   const bcore_source_s* p, vd_t o, sc_t format, ... );
bl_t bcore_source_spect_parse_bool_f(  const bcore_source_s* p, vd_t o, sc_t format      ); // format must yield just one bool, which is returned
void bcore_source_spect_set_supplier( const bcore_source_s* p, vd_t o, vd_t supplier    ); // error when not supported

sz_t bcore_source_aware_get_data(     vd_t o, vd_t data, sz_t size );
void bcore_source_aware_parse_fv(     vd_t o, sc_t format, va_list args );
void bcore_source_aware_parse_fa(     vd_t o, sc_t format, ... );
void bcore_source_aware_parse_errvf(  vd_t o, sc_t format, va_list args );
void bcore_source_aware_parse_errf(   vd_t o, sc_t format, ... );
bl_t bcore_source_aware_parse_bool_f(  vd_t o, sc_t format      ); // format must yield just one bool, which is returned
void bcore_source_aware_set_supplier( vd_t o, vd_t supplier    ); // error when not supported

sz_t bcore_source_get_data(     sr_s o, vd_t data, sz_t size );
void bcore_source_parse_fv(     sr_s o, sc_t format, va_list args );
void bcore_source_parse_fa(     sr_s o, sc_t format, ... );
void bcore_source_parse_errvf(  sr_s o, sc_t format, va_list args );
void bcore_source_parse_errf(   sr_s o, sc_t format, ... );
bl_t bcore_source_parse_bool_f(  sr_s o, sc_t format      ); // format must yield just one bool, which is returned
void bcore_source_set_supplier( sr_s o, vd_t supplier    ); // error when not supported

sz_t bcore_source_q_get_data(     const sr_s* o, vd_t data, sz_t size );
void bcore_source_q_parse_fv(     const sr_s* o, sc_t format, va_list args );
void bcore_source_q_parse_fa(     const sr_s* o, sc_t format, ... );
void bcore_source_q_parse_errvf(  const sr_s* o, sc_t format, va_list args );
void bcore_source_q_parse_errf(   const sr_s* o, sc_t format, ... );
bl_t bcore_source_q_parse_bool_f(  const sr_s* o, sc_t format      ); // format must yield just one bool, which is returned
void bcore_source_q_set_supplier( const sr_s* o, vd_t supplier    ); // error when not supported

vd_t bcore_spect_source_signal( tp_t target, tp_t signal, vd_t object );

#endif // BCORE_SPECT_SOURCE_H
