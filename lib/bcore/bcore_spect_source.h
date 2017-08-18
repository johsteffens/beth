/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCORE_SPECT_SOURCE_H
#define BCORE_SPECT_SOURCE_H

#include "bcore_first.h"
#include "bcore_types.h"
#include "bcore_features.h"
#include "bcore_flect.h"

/// optional source features:
typedef void (*bcore_source_fp_parsevf )( vd_t o, sc_t format, va_list args ); // parse function
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
    bcore_source_fp_parsevf      fp_parsevf;
    bcore_source_fp_set_supplier fp_set_supplier;

} bcore_source_s;

bcore_flect_self_s* bcore_source_s_create_self( void );

const bcore_source_s* bcore_source_s_get_typed( tp_t type );
const bcore_source_s* bcore_source_s_get_aware( vc_t obj );

sz_t bcore_source_spect_get_data(     const bcore_source_s* p, vd_t o, vd_t data, sz_t size );
void bcore_source_spect_parsevf(      const bcore_source_s* p, vd_t o, sc_t format, va_list args );
void bcore_source_spect_parsef(       const bcore_source_s* p, vd_t o, sc_t format, ... );
void bcore_source_spect_parse_errvf(  const bcore_source_s* p, vd_t o, sc_t format, va_list args );
void bcore_source_spect_parse_errf(   const bcore_source_s* p, vd_t o, sc_t format, ... );
bool bcore_source_spect_parse_boolf(  const bcore_source_s* p, vd_t o, sc_t format      ); // format must yield just one bool, which is returned
void bcore_source_spect_set_supplier( const bcore_source_s* p, vd_t o, vd_t supplier    ); // error when not supported

sz_t bcore_source_aware_get_data(     vd_t o, vd_t data, sz_t size );
void bcore_source_aware_parsevf(      vd_t o, sc_t format, va_list args );
void bcore_source_aware_parsef(       vd_t o, sc_t format, ... );
void bcore_source_aware_parse_errvf(  vd_t o, sc_t format, va_list args );
void bcore_source_aware_parse_errf(   vd_t o, sc_t format, ... );
bool bcore_source_aware_parse_boolf(  vd_t o, sc_t format      ); // format must yield just one bool, which is returned
void bcore_source_aware_set_supplier( vd_t o, vd_t supplier    ); // error when not supported

#endif // BCORE_SPECT_SOURCE_H
