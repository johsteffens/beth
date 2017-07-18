/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCORE_SOURCE_CHAIN_H
#define BCORE_SOURCE_CHAIN_H

#include "bcore_spect_source.h"
#include "bcore_string.h"
#include "bcore_flect.h"

/** Chain of data sources (copyable, serializable)
  * The chain is internally maintained as array
  * The fist entry represents the root source
  * The last entry provides the chain's data source functions.
  * All entries except the first one must accept a supplier.
  *
  * The chain supports bcore_source_fp_parsevf.
  * If the last entry does not support this feature, the chain extends
  * itself lazily adding source_string.
  */
typedef struct bcore_source_chain_s
{
    aware_t _;
    vd_t* data;
    sz_t size, space;
} bcore_source_chain_s;

void                  bcore_source_chain_s_init( bcore_source_chain_s* o );
void                  bcore_source_chain_s_down( bcore_source_chain_s* o );
void                  bcore_source_chain_s_copy( bcore_source_chain_s* o, const bcore_source_chain_s* src );
bcore_source_chain_s* bcore_source_chain_s_create();
void                  bcore_source_chain_s_discard(     bcore_source_chain_s* o );
bcore_source_chain_s* bcore_source_chain_s_clone( const bcore_source_chain_s* o );
void                  bcore_source_chain_s_push_d( bcore_source_chain_s* o, vd_t source );
void                  bcore_source_chain_s_push_type( bcore_source_chain_s* o, tp_t type );

bcore_flect_self_s*   bcore_source_chain_s_create_self();

#endif // BCORE_SOURCE_CHAIN_H
