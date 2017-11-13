/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#ifndef BCORE_NAME_H
#define BCORE_NAME_H

#include "bcore_tp.h"

/// name --> key
static inline tp_t bcore_name_key(   sc_t name         ) { return bcore_tp_hash_sc( name    ); }
static inline tp_t bcore_name_key_n( sc_t name, sz_t n ) { return bcore_tp_hash_vc( name, n ); }

static inline tp_t bcore_name_key_ns( tp_t name_space, sc_t name )
{
    if( name_space )
    {
        return bcore_tp_fold_sc( bcore_tp_fold_u0( name_space, ':' ), name );
    }
    else
    {
        return bcore_name_key( name );
    }
}

static inline tp_t bcore_name_key_ns_n( tp_t name_space, sc_t name, sz_t n )
{
    if( name_space )
    {
        return bcore_tp_fold_vc( bcore_tp_fold_u0( name_space, ':' ), name, n );
    }
    else
    {
        return bcore_name_key_n( name, n );
    }
}

typedef struct bcore_name_s
{
    tp_t key;
    tp_t name_space;
    sd_t name;
} bcore_name_s;

DECLARE_FUNCTION_INIT(    bcore_name_s )
DECLARE_FUNCTION_DOWN(    bcore_name_s )
DECLARE_FUNCTION_COPY(    bcore_name_s )
DECLARE_FUNCTION_CREATE(  bcore_name_s )
DECLARE_FUNCTION_DISCARD( bcore_name_s )
DECLARE_FUNCTION_CLONE(   bcore_name_s )

/// creation using default hashing
bcore_name_s bcore_name_sc( sc_t name );
bcore_name_s bcore_name_ns_sc( tp_t name_space, sc_t name );
bcore_name_s bcore_name_sc_n( sc_t name, sz_t n );
bcore_name_s bcore_name_ns_sc_n( tp_t name_space, sc_t name, sz_t n );

typedef struct bcore_name_map_s
{
    aware_t _;
    bcore_name_s* data;
    bl_t* flags; // flags for tracing
    sz_t size;
    sz_t size_limit;
    sz_t depth_limit;
} bcore_name_map_s;

DECLARE_FUNCTION_INIT(    bcore_name_map_s )
DECLARE_FUNCTION_DOWN(    bcore_name_map_s )
DECLARE_FUNCTION_COPY(    bcore_name_map_s )
DECLARE_FUNCTION_CREATE(  bcore_name_map_s )
DECLARE_FUNCTION_DISCARD( bcore_name_map_s )
DECLARE_FUNCTION_CLONE(   bcore_name_map_s )

bcore_name_s* bcore_name_map_s_get(      const bcore_name_map_s* o, tp_t key ); // returns NULL in case key does not exist
void          bcore_name_map_s_set(            bcore_name_map_s* o, bcore_name_s name ); // takes ownership of name
void          bcore_name_map_s_remove(         bcore_name_map_s* o, tp_t key ); // removes key if existing
bl_t          bcore_name_map_s_exists(   const bcore_name_map_s* o, tp_t key ); // checks if key exists
void          bcore_name_map_s_clear(          bcore_name_map_s* o           ); // removes all entries and frees memory
sz_t          bcore_name_map_s_keys(     const bcore_name_map_s* o           ); // returns number of registered names
sz_t          bcore_name_map_s_size(     const bcore_name_map_s* o           ); // returns current size of the hash map (note that this includes empty places)
bcore_name_s* bcore_name_map_s_idx_name( const bcore_name_map_s* o, sz_t idx ); // returns indexed name (idx indexes the entire table including empty places)

vd_t bcore_name_signal( tp_t target, tp_t signal, vd_t object );

#endif // BCORE_NAME_H
