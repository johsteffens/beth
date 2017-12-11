/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#ifndef BCORE_SPECT_SOURCE_H
#define BCORE_SPECT_SOURCE_H

#include "bcore_first.h"
#include "bcore_types.h"
#include "bcore_features.h"
#include "bcore_flect.h"

/// optional source features:
typedef void (*bcore_source_fp_parse_fv )(    vd_t o, sc_t format, va_list args ); // parse function
typedef void (*bcore_source_fp_set_supplier)( vd_t o, vd_t supplier );             // to create a chain of source units
typedef bl_t (*bcore_source_fp_eos)(          vc_t o );                            // (required) end of source has been reached
typedef sc_t (*bcore_source_fp_file)(         vc_t o );                            // returns file name if source is of a file system or "" otherwise

/// required source features:
/// awareness

typedef struct bcore_source_s bcore_source_s;
typedef struct bcore_source_s
{
    aware_t p_type; // type of perspective
    tp_t    o_type; // type of object

    bcore_fp_flow_src            fp_flow_src;
    bcore_fp_logvf               fp_parse_errvf;
    bcore_source_fp_parse_fv     fp_parse_fv;
    bcore_source_fp_set_supplier fp_set_supplier;
    bcore_source_fp_eos          fp_eos;
    bcore_source_fp_file         fp_file;

} bcore_source_s;

const bcore_source_s* bcore_source_s_get_typed( tp_t type );
const bcore_source_s* bcore_source_s_get_aware( vc_t obj );

sz_t bcore_source_spect_get_data    ( const bcore_source_s* p, vd_t o, vd_t data, sz_t size ); // returns number of bytes read
u0_t bcore_source_spect_get_u0      ( const bcore_source_s* p, vd_t o ); // reads single byte and returns it (returns 0 in case eos is reached
void bcore_source_spect_parse_fv    ( const bcore_source_s* p, vd_t o, sc_t format, va_list args );
void bcore_source_spect_parse_fa    ( const bcore_source_s* p, vd_t o, sc_t format, ... );
void bcore_source_spect_parse_errvf ( const bcore_source_s* p, vd_t o, sc_t format, va_list args );
void bcore_source_spect_parse_errf  ( const bcore_source_s* p, vd_t o, sc_t format, ... );
void bcore_source_spect_parse_err_fv( const bcore_source_s* p, vd_t o, sc_t format, va_list args );
void bcore_source_spect_parse_err_fa( const bcore_source_s* p, vd_t o, sc_t format, ... );
bl_t bcore_source_spect_parse_bl_fa ( const bcore_source_s* p, vd_t o, sc_t format      ); // format must yield just one bool, which is returned
void bcore_source_spect_set_supplier( const bcore_source_s* p, vd_t o, vd_t supplier    ); // error when not supported
bl_t bcore_source_spect_eos         ( const bcore_source_s* p, vc_t o );
sc_t bcore_source_spect_file        ( const bcore_source_s* p, vc_t o );

sz_t bcore_source_aware_get_data    ( vd_t o, vd_t data, sz_t size );
u0_t bcore_source_aware_get_u0      ( vd_t o );
void bcore_source_aware_parse_fv    ( vd_t o, sc_t format, va_list args );
void bcore_source_aware_parse_fa    ( vd_t o, sc_t format, ... );
void bcore_source_aware_parse_errvf ( vd_t o, sc_t format, va_list args );
void bcore_source_aware_parse_errf  ( vd_t o, sc_t format, ... );
void bcore_source_aware_parse_err_fv( vd_t o, sc_t format, va_list args );
void bcore_source_aware_parse_err_fa( vd_t o, sc_t format, ... );
bl_t bcore_source_aware_parse_bl_fa ( vd_t o, sc_t format      ); // format must yield just one bool, which is returned
void bcore_source_aware_set_supplier( vd_t o, vd_t supplier    ); // error when not supported
bl_t bcore_source_aware_eos         ( vc_t o );
sc_t bcore_source_aware_file        ( vc_t o );

sz_t bcore_source_get_data    ( sr_s o, vd_t data, sz_t size );
u0_t bcore_source_get_u0      ( sr_s o );
void bcore_source_parse_fv    ( sr_s o, sc_t format, va_list args );
void bcore_source_parse_fa    ( sr_s o, sc_t format, ... );
void bcore_source_parse_errvf ( sr_s o, sc_t format, va_list args );
void bcore_source_parse_errf  ( sr_s o, sc_t format, ... );
void bcore_source_parse_err_fv( sr_s o, sc_t format, va_list args );
void bcore_source_parse_err_fa( sr_s o, sc_t format, ... );
bl_t bcore_source_parse_bl_fa ( sr_s o, sc_t format      ); // format must yield just one bool, which is returned
void bcore_source_set_supplier( sr_s o, vd_t supplier    ); // error when not supported
bl_t bcore_source_eos         ( sr_s o );
sc_t bcore_source_file        ( sr_s o );

sz_t bcore_source_q_get_data    ( const sr_s* o, vd_t data, sz_t size );
u0_t bcore_source_q_get_u0      ( const sr_s* o );
void bcore_source_q_parse_fv    ( const sr_s* o, sc_t format, va_list args );
void bcore_source_q_parse_fa    ( const sr_s* o, sc_t format, ... );
void bcore_source_q_parse_errvf ( const sr_s* o, sc_t format, va_list args );
void bcore_source_q_parse_errf  ( const sr_s* o, sc_t format, ... );
void bcore_source_q_parse_err_fv( const sr_s* o, sc_t format, va_list args );
void bcore_source_q_parse_err_fa( const sr_s* o, sc_t format, ... );
bl_t bcore_source_q_parse_bl_fa ( const sr_s* o, sc_t format      ); // format must yield just one bool, which is returned
void bcore_source_q_set_supplier( const sr_s* o, vd_t supplier    ); // error when not supported
bl_t bcore_source_q_eos         ( const sr_s* o );
sc_t bcore_source_q_file        ( const sr_s* o );

vd_t bcore_spect_source_signal( tp_t target, tp_t signal, vd_t object );

#endif // BCORE_SPECT_SOURCE_H
