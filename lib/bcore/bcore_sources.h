/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#ifndef BCORE_SOURCES_H
#define BCORE_SOURCES_H

/// Objects supporting source perspective

#include "bcore_spect_source.h"
#include "bcore_st.h"
#include "bcore_flect.h"

/**********************************************************************************************************************/

/** Chain of data sources (copyable, serializable)
  * The chain is internally maintained as array
  * The first element represents the principal source
  * The last element provides the chain's data source functions.
  * All elements except the first one must accept a supplier.
  *
  * The chain lazily supports bcore_source_fp_parse_fv by pushing source_string
  * if not already supported by the last element.
  */
typedef struct bcore_source_chain_s
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
} bcore_source_chain_s;

void                  bcore_source_chain_s_init( bcore_source_chain_s* o );
void                  bcore_source_chain_s_down( bcore_source_chain_s* o );
void                  bcore_source_chain_s_copy( bcore_source_chain_s* o, const bcore_source_chain_s* src );
bcore_source_chain_s* bcore_source_chain_s_create();
void                  bcore_source_chain_s_discard(     bcore_source_chain_s* o );
bcore_source_chain_s* bcore_source_chain_s_clone( const bcore_source_chain_s* o );
void                  bcore_source_chain_s_push_d(       bcore_source_chain_s* o, vd_t source );
void                  bcore_source_chain_s_push_type(    bcore_source_chain_s* o, tp_t type );
sz_t                  bcore_source_chain_s_get_data(     bcore_source_chain_s* o, vd_t data, sz_t size );
void                  bcore_source_chain_s_set_supplier( bcore_source_chain_s* o, vd_t supplier );

/**********************************************************************************************************************/

/** Memory based data source supporting source and array perspectives.
 *  Can be used as buffer in a source-chain or as initial data source.
 */
typedef struct bcore_source_buffer_s
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
sz_t                   bcore_source_buffer_s_get_data(      bcore_source_buffer_s* o, vd_t data, sz_t size );

bcore_source_buffer_s* bcore_source_buffer_s_create_from_data( vc_t data, sz_t size );

/// Specifies an external supplier (source); supplier is not owned by this object
void bcore_source_buffer_s_set_supplier( bcore_source_buffer_s* o, vd_t supplier );

/**********************************************************************************************************************/

/** String based data source supporting bcore_source_s perspective.
 *  Can be used as buffer in a source-chain or as initial data source.
 *  Implements feature bcore_source_fp_parse_fv.
 */
typedef struct bcore_source_string_s
{
    aware_t _;
    st_s* string;
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

bcore_source_string_s* bcore_source_string_s_create_from_string( const st_s* string );
bcore_source_string_s* bcore_source_string_s_create_from_string_d(     st_s* string );
bcore_source_string_s* bcore_source_string_s_create_sc( sc_t sc );
sz_t                   bcore_source_string_s_get_data(      bcore_source_string_s* o, vd_t data, sz_t size );

/** Specifies an external supplier (source); supplier is not owned by this object.
 *  The supplier is automatically detached when empty.
 */
void bcore_source_string_s_set_supplier( bcore_source_string_s* o, vd_t supplier );

/**********************************************************************************************************************/

/** File data source (copyable, serializable)
  * The 'open' state is not copied;
  */
typedef struct bcore_source_file_s
{
    aware_t _;
    st_s* name;
    vd_t handle;
} bcore_source_file_s;

void                 bcore_source_file_s_init( bcore_source_file_s* o );
void                 bcore_source_file_s_down( bcore_source_file_s* o );
void                 bcore_source_file_s_copy( bcore_source_file_s* o, const bcore_source_file_s* src );
bcore_source_file_s* bcore_source_file_s_create();
bcore_source_file_s* bcore_source_file_s_create_name( sc_t name );        // file is opened lazily when needed
void                 bcore_source_file_s_discard(       bcore_source_file_s* o );
bcore_source_file_s* bcore_source_file_s_clone(   const bcore_source_file_s* o );
void                 bcore_source_file_s_open(          bcore_source_file_s* o ); // (re)opens file
void                 bcore_source_file_s_close(         bcore_source_file_s* o ); // closes file if open
sz_t                 bcore_source_file_s_get_data(      bcore_source_file_s* o, vd_t data, sz_t size );

/**********************************************************************************************************************/

vd_t bcore_sources_signal( tp_t target, tp_t signal, vd_t object );

/**********************************************************************************************************************/

#endif // BCORE_SOURCES_H
