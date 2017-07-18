/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCORE_SOURCE_FILE_H
#define BCORE_SOURCE_FILE_H

#include "bcore_spect_source.h"
#include "bcore_string.h"
#include "bcore_flect.h"

/** File data source (copyable, serializable)
  * The 'open' state is not copied;
  */
typedef struct bcore_source_file_s
{
    aware_t _;
    bcore_string_s* name;
    vd_t handle;
} bcore_source_file_s;

void                 bcore_source_file_s_init( bcore_source_file_s* o );
void                 bcore_source_file_s_down( bcore_source_file_s* o );
void                 bcore_source_file_s_copy( bcore_source_file_s* o, const bcore_source_file_s* src );
bcore_source_file_s* bcore_source_file_s_create();
bcore_source_file_s* bcore_source_file_s_create_name( sc_t name );        // file is opened lazily when needed
void                 bcore_source_file_s_discard(       bcore_source_file_s* o );
bcore_source_file_s* bcore_source_file_s_clone(   const bcore_source_file_s* o );
void                 bcore_source_file_s_open(  bcore_source_file_s* o ); // (re)opens file
void                 bcore_source_file_s_close( bcore_source_file_s* o ); // closes file if open
void                 bcore_source_file_s_flush( bcore_source_file_s* o ); // flushes file if open
bcore_flect_self_s*  bcore_source_file_s_create_self();

#endif // BCORE_SOURCE_FILE_H
