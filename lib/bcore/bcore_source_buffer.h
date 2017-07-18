/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCORE_SOURCE_BUFFER_H
#define BCORE_SOURCE_BUFFER_H

#include "bcore_spect_source.h"
#include "bcore_string.h"
#include "bcore_flect.h"

/// Memory based data source supporting source and array perspectives
typedef struct bcore_source_buffer_s
{
    aware_t _;
    u0_t* data;
    sz_t size, space;
    sz_t index;

    vd_t ext_supplier;  // optional external supplier (source) turning this source into a buffer; (ext_supplier is not owned by bcore_source_buffer_s)
    sz_t prefetch_size; // data amount prefetched from supplier (if present) (default: 4096)

} bcore_source_buffer_s;

void                   bcore_source_buffer_s_init(          bcore_source_buffer_s* o );
void                   bcore_source_buffer_s_down(          bcore_source_buffer_s* o );
void                   bcore_source_buffer_s_copy(          bcore_source_buffer_s* o, const bcore_source_buffer_s* src );
bcore_source_buffer_s* bcore_source_buffer_s_create();
void                   bcore_source_buffer_s_discard(       bcore_source_buffer_s* o );
bcore_source_buffer_s* bcore_source_buffer_s_clone(   const bcore_source_buffer_s* o );

bcore_flect_self_s*    bcore_source_buffer_s_create_self();

/// Specifies an external supplier (source); supplier is not owned by this object
void bcore_source_buffer_s_set_supplier( bcore_source_buffer_s* o, vd_t supplier );

#endif // BCORE_SOURCE_BUFFER_H
