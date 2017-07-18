/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCORE_SOURCE_STRING_H
#define BCORE_SOURCE_STRING_H

#include "bcore_spect_source.h"
#include "bcore_string.h"
#include "bcore_flect.h"

/// String based data source supporting bcore_source_s perspective
typedef struct bcore_source_string_s
{
    aware_t _;
    bcore_string_s* string;
    sz_t index;
    vd_t ext_supplier;       // optional external supplier (source) turning this source into a buffer; (ext_supplier is not owned by bcore_source_string_s)
    sz_t preceding_lines;    // line counter (only for lines processed and removed from string)
    sz_t refill_limit;       // size size limit to request refill from supplier (if present)
    sz_t prefetch_size;      // data amount prefetched from supplier (if present)
} bcore_source_string_s;

void                   bcore_source_string_s_init(          bcore_source_string_s* o );
void                   bcore_source_string_s_down(          bcore_source_string_s* o );
void                   bcore_source_string_s_copy(          bcore_source_string_s* o, const bcore_source_string_s* src );
bcore_source_string_s* bcore_source_string_s_create();
void                   bcore_source_string_s_discard(       bcore_source_string_s* o );
bcore_source_string_s* bcore_source_string_s_clone(   const bcore_source_string_s* o );

bcore_source_string_s* bcore_source_string_s_create_string( const bcore_string_s* string );
bcore_source_string_s* bcore_source_string_s_create_string_d(     bcore_string_s* string );
bcore_source_string_s* bcore_source_string_s_create_sc( sc_t sc );
bcore_flect_self_s*    bcore_source_string_s_create_self();

/** Specifies an external supplier (source); supplier is not owned by this object.
 *  The supplier is automatically detached when empty.
 */
void bcore_source_string_s_set_supplier( bcore_source_string_s* o, vd_t supplier );

#endif // BCORE_SOURCE_STRING_H
