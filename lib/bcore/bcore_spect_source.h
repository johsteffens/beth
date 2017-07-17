/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCORE_SPECT_SOURCE_H
#define BCORE_SPECT_SOURCE_H

#include "bcore_first.h"
#include "bcore_types.h"
#include "bcore_features.h"
#include "bcore_flect.h"

/// optional source features:
typedef void (*bcore_source_fp_parsevf )( vd_t o, sc_t format, va_list args ); // parse function

/// required source feature:
// awareness

typedef struct bcore_source_s bcore_source_s;
typedef struct bcore_source_s
{
    aware_t p_type; // type of perspective
    tp_t    o_type; // type of object

    bcore_fp_flow_src       fp_flow_src;
    bcore_fp_logvf          fp_parse_errvf;
    bcore_source_fp_parsevf fp_parsevf;

    sz_t (*get_data )(   const bcore_source_s* p, vd_t o, vd_t data, sz_t size );
    void (*parsef)(      const bcore_source_s* p, vd_t o, sc_t format, ... );
    void (*parse_errvf)( const bcore_source_s* p, vd_t o, sc_t format, va_list args );
    void (*parse_errf)(  const bcore_source_s* p, vd_t o, sc_t format, ... );
    bool (*parse_boolf)( const bcore_source_s* p, vd_t o, sc_t format      ); // format must yield just one bool, which is returned

} bcore_source_s;

bcore_flect_self_s* bcore_source_s_create_self();

const bcore_source_s* bcore_source_s_get_typed( tp_t type );

static inline const bcore_source_s* bcore_source_s_get_aware( vc_t obj )
{
    return bcore_source_s_get_typed( *( const aware_t* )obj );
}

/// all sources are aware
sz_t bcore_source_get_data(    vd_t o, vd_t data, sz_t size );
void bcore_source_parsef(      vd_t o, sc_t format, ... );
void bcore_source_parse_errvf( vd_t o, sc_t format, va_list args );
void bcore_source_parse_errf(  vd_t o, sc_t format, ... );
bool bcore_source_parse_boolf( vd_t o, sc_t format      ); // format must yield just one bool, which is returned

#endif // BCORE_SPECT_SOURCE_H
