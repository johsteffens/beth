/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCORE_STRING_SOURCE_H
#define BCORE_STRING_SOURCE_H

#include "bcore_source_perspective.h"
#include "bcore_string.h"
#include "bcore_flect.h"

/// String based data source supporting bcore_source_s perspective
typedef struct bcore_string_source_s
{
    aware_t _;
    bcore_string_s* string;
    sz_t index;
} bcore_string_source_s;

void                   bcore_string_source_s_init(          bcore_string_source_s* o );
void                   bcore_string_source_s_down(          bcore_string_source_s* o );
void                   bcore_string_source_s_copy(          bcore_string_source_s* o, const bcore_string_source_s* src );
bcore_string_source_s* bcore_string_source_s_create();
void                   bcore_string_source_s_discard(       bcore_string_source_s* o );
bcore_string_source_s* bcore_string_source_s_clone(   const bcore_string_source_s* o );
void                   bcore_string_source_s_rewind(        bcore_string_source_s* o );

bcore_string_source_s* bcore_string_source_s_create_string( const bcore_string_s* string );
bcore_string_source_s* bcore_string_source_s_create_string_d( bcore_string_s* string );
bcore_string_source_s* bcore_string_source_s_create_sc( sc_t sc );
bcore_flect_self_s*    bcore_string_source_s_create_self();

#endif // BCORE_STRING_SOURCE_H
