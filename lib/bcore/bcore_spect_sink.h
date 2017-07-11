/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCORE_SPECT_SINK_H
#define BCORE_SPECT_SINK_H

#include "bcore_first.h"
#include "bcore_types.h"
#include "bcore_features.h"
#include "bcore_flect.h"

typedef struct bcore_sink_s
{
    aware_t p_type; // type of perspective
    tp_t    o_type; // type of object

    bcore_fp_flow_snk flow_snk;

    void (*push_data    )( const struct bcore_sink_s* p, vd_t o, vc_t data, sz_t size ); // pushes size bytes of data
    void (*pushf        )( const struct bcore_sink_s* p, vd_t o, sc_t format, ... );
    void (*push_char    )( const struct bcore_sink_s* p, vd_t o, char c );
    void (*push_sc      )( const struct bcore_sink_s* p, vd_t o, sc_t sc );
    void (*push_string  )( const struct bcore_sink_s* p, vd_t o, const bcore_string_s* string );
    void (*push_string_d)( const struct bcore_sink_s* p, vd_t o,       bcore_string_s* string );

} bcore_sink_s;

bcore_flect_self_s* bcore_sink_s_create_self();

const bcore_sink_s* bcore_sink_s_get_typed( tp_t type );

static inline const bcore_sink_s* bcore_sink_s_get_aware( vc_t obj )
{
    return bcore_sink_s_get_typed( *( const aware_t* )obj );
}

#endif // BCORE_SPECT_SINK_H
