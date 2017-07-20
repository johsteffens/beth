/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCORE_SINKS_H
#define BCORE_SINKS_H

/// Objects supporting sink perspective

#include "bcore_spect_sink.h"
#include "bcore_flect.h"

/**********************************************************************************************************************/

/** Chain of data sinks (copyable, serializable)
  * The chain is internally maintained as array
  * The first element represents the final consumer
  * The last element is the the entry point for feeding data.
  * All elements except the first one must accept a consumer.
  */
typedef struct bcore_sink_chain_s
{
    aware_t _;
    union
    {
        bcore_aware_link_array_s arr;
        struct
        {
            vd_t* data;
            sz_t size, space;
        };
    };
} bcore_sink_chain_s;

void                bcore_sink_chain_s_init( bcore_sink_chain_s* o );
void                bcore_sink_chain_s_down( bcore_sink_chain_s* o );
void                bcore_sink_chain_s_copy( bcore_sink_chain_s* o, const bcore_sink_chain_s* src );
bcore_sink_chain_s* bcore_sink_chain_s_create();
void                bcore_sink_chain_s_discard(      bcore_sink_chain_s* o );
bcore_sink_chain_s* bcore_sink_chain_s_clone(  const bcore_sink_chain_s* o );
void                bcore_sink_chain_s_push_d(       bcore_sink_chain_s* o, vd_t sink );
void                bcore_sink_chain_s_push_type(    bcore_sink_chain_s* o, tp_t type );
void                bcore_sink_chain_s_flush(        bcore_sink_chain_s* o );
sz_t                bcore_sink_chain_s_push_data(    bcore_sink_chain_s* o, vc_t data, sz_t size );
void                bcore_sink_chain_s_set_consumer( bcore_sink_chain_s* o, vd_t consumer );

bcore_flect_self_s* bcore_sink_chain_s_create_self();

/**********************************************************************************************************************/

/** Memory based data sink supporting sink and array perspectives.
 *  Can be used as buffer in a sink-chain or as final data sink.
 */
typedef struct bcore_sink_buffer_s
{
    aware_t _;
    union
    {
        bcore_static_array_s arr;
        struct
        {
            u0_t* data;
            sz_t size, space;
        };
    };

    vd_t consumer;      // optional external consumer (sink) turning this sink into a buffer; (ext_consumer is not owned by bcore_sink_buffer_s)
    sz_t entrepot_size; // data amount to pile up before transfered to consumer (if present) (default: 4096)

} bcore_sink_buffer_s;

void                 bcore_sink_buffer_s_init(        bcore_sink_buffer_s* o );
void                 bcore_sink_buffer_s_down(        bcore_sink_buffer_s* o );
void                 bcore_sink_buffer_s_copy(        bcore_sink_buffer_s* o, const bcore_sink_buffer_s* src );
bcore_sink_buffer_s* bcore_sink_buffer_s_create();
void                 bcore_sink_buffer_s_discard(     bcore_sink_buffer_s* o );
bcore_sink_buffer_s* bcore_sink_buffer_s_clone( const bcore_sink_buffer_s* o );
void                 bcore_sink_buffer_s_flush(       bcore_sink_buffer_s* o );
sz_t                 bcore_sink_buffer_s_push_data(   bcore_sink_buffer_s* o, vc_t data, sz_t size );

bcore_flect_self_s*  bcore_sink_buffer_s_create_self();

/// Specifies an external consumer (sink); consumer is not owned by this object
void bcore_sink_buffer_s_set_consumer( bcore_sink_buffer_s* o, vd_t consumer );

/**********************************************************************************************************************/

/** File data sink (copyable, serializable)
  * The 'open' state is not copied;
  */
typedef struct bcore_sink_file_s
{
    aware_t _;
    bcore_string_s* name;
    vd_t handle;
} bcore_sink_file_s;

void                bcore_sink_file_s_init(        bcore_sink_file_s* o );
void                bcore_sink_file_s_down(        bcore_sink_file_s* o );
void                bcore_sink_file_s_copy(        bcore_sink_file_s* o, const bcore_sink_file_s* src );
bcore_sink_file_s*  bcore_sink_file_s_create();
bcore_sink_file_s*  bcore_sink_file_s_create_name( sc_t name );        // file is opened lazily when needed
void                bcore_sink_file_s_discard(     bcore_sink_file_s* o );
bcore_sink_file_s*  bcore_sink_file_s_clone( const bcore_sink_file_s* o );
void                bcore_sink_file_s_open(        bcore_sink_file_s* o ); // (re)opens file
void                bcore_sink_file_s_close(       bcore_sink_file_s* o ); // closes file if open
void                bcore_sink_file_s_flush(       bcore_sink_file_s* o );
sz_t                bcore_sink_file_s_push_data(   bcore_sink_file_s* o, vc_t data, sz_t size );

bcore_flect_self_s* bcore_sink_file_s_create_self();

/**********************************************************************************************************************/

#endif // BCORE_SINKS_H