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
} bcore_source_string_s;

void                   bcore_source_string_s_init(          bcore_source_string_s* o );
void                   bcore_source_string_s_down(          bcore_source_string_s* o );
void                   bcore_source_string_s_copy(          bcore_source_string_s* o, const bcore_source_string_s* src );
bcore_source_string_s* bcore_source_string_s_create();
void                   bcore_source_string_s_discard(       bcore_source_string_s* o );
bcore_source_string_s* bcore_source_string_s_clone(   const bcore_source_string_s* o );
void                   bcore_source_string_s_rewind(        bcore_source_string_s* o );

bcore_source_string_s* bcore_source_string_s_create_string( const bcore_string_s* string );
bcore_source_string_s* bcore_source_string_s_create_string_d( bcore_string_s* string );
bcore_source_string_s* bcore_source_string_s_create_sc( sc_t sc );
bcore_flect_self_s*    bcore_source_string_s_create_self();

#endif // BCORE_SOURCE_STRING_H
