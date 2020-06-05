/** Author and Copyright 2017 Johannes Bernhard Steffens
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

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
        bcore_array_dyn_link_aware_s arr;
        struct
        {
            vd_t* data;
            uz_t size, space;
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
uz_t                  bcore_source_chain_s_get_data(     bcore_source_chain_s* o, vd_t data, uz_t size );
void                  bcore_source_chain_s_set_supplier( bcore_source_chain_s* o, vd_t supplier );
bl_t                  bcore_source_chain_s_eos(       const bcore_source_chain_s* o );
sc_t                  bcore_source_chain_s_get_file(  const bcore_source_chain_s* o );
s3_t                  bcore_source_chain_s_get_index( const bcore_source_chain_s* o );
void                  bcore_source_chain_s_set_index(       bcore_source_chain_s* o, s3_t index );

/**********************************************************************************************************************/

/** Memory based data source supporting source and array perspectives.
 *  Can be used as buffer in a source-chain or as initial data source.
 */
typedef struct bcore_source_buffer_s
{
    aware_t _;
    union
    {
        bcore_array_dyn_solid_static_s arr;
        struct
        {
            u0_t* data;
            uz_t size, space;
        };
    };

    uz_t index;

    vd_t ext_supplier;  // optional external supplier (source) turning this source into a buffer; (ext_supplier is not owned by bcore_source_buffer_s)
    uz_t prefetch_size; // data amount prefetched from supplier (if present) (default: 4096)

} bcore_source_buffer_s;

void                   bcore_source_buffer_s_init(          bcore_source_buffer_s* o );
void                   bcore_source_buffer_s_down(          bcore_source_buffer_s* o );
void                   bcore_source_buffer_s_copy(          bcore_source_buffer_s* o, const bcore_source_buffer_s* src );
bcore_source_buffer_s* bcore_source_buffer_s_create();
void                   bcore_source_buffer_s_discard(       bcore_source_buffer_s* o );
bcore_source_buffer_s* bcore_source_buffer_s_clone(   const bcore_source_buffer_s* o );
uz_t                   bcore_source_buffer_s_get_data(      bcore_source_buffer_s* o, vd_t data, uz_t size );

bcore_source_buffer_s* bcore_source_buffer_s_create_from_data( vc_t data, uz_t size );

/// Specifies an external supplier (source); supplier is not owned by this object
void bcore_source_buffer_s_set_supplier( bcore_source_buffer_s* o, vd_t supplier );

bl_t bcore_source_buffer_s_eos(       const bcore_source_buffer_s* o );
sc_t bcore_source_buffer_s_get_file(  const bcore_source_buffer_s* o );
s3_t bcore_source_buffer_s_get_index( const bcore_source_buffer_s* o );
void bcore_source_buffer_s_set_index(       bcore_source_buffer_s* o, s3_t index );

/**********************************************************************************************************************/

/** String based data source supporting bcore_source_s perspective.
 *  Can be used as buffer in a source-chain or as initial data source.
 *  Implements feature bcore_source_fp_parse_fv.
 */
typedef struct bcore_source_string_s
{
    aware_t _;
    st_s* string;
    uz_t index;
    vd_t ext_supplier;       // optional external supplier (source) turning this source into a buffer; (ext_supplier is not owned by bcore_source_string_s)
    uz_t refill_limit;       // size size limit to request refill from supplier (if present)
    uz_t prefetch_size;      // data amount prefetched from supplier (if present)
    bcore_source_chain_s* chain; // governing chain (if any)
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
uz_t                   bcore_source_string_s_get_data(      bcore_source_string_s* o, vd_t data, uz_t size );

/** Specifies an external supplier (source); supplier is not owned by this object.
 *  The supplier is automatically detached when empty.
 */
void bcore_source_string_s_set_supplier( bcore_source_string_s* o, vd_t supplier );

bl_t bcore_source_string_s_eos(  const bcore_source_string_s* o );
sc_t bcore_source_string_s_get_file(  const bcore_source_string_s* o );
s3_t bcore_source_string_s_get_index( const bcore_source_string_s* o );
void bcore_source_string_s_set_index(       bcore_source_string_s* o, s3_t index );

/**********************************************************************************************************************/

/** File data source (copyable, serializable)
  * The 'open' state is not copied;
  */
typedef struct bcore_source_file_s bcore_source_file_s;

void                 bcore_source_file_s_init( bcore_source_file_s* o );
void                 bcore_source_file_s_down( bcore_source_file_s* o );
void                 bcore_source_file_s_copy( bcore_source_file_s* o, const bcore_source_file_s* src );
bcore_source_file_s* bcore_source_file_s_create();
bcore_source_file_s* bcore_source_file_s_create_name( sc_t name );        // file is opened lazily when needed
sc_t                 bcore_source_file_s_get_name( const bcore_source_file_s* o );

void                 bcore_source_file_s_discard(         bcore_source_file_s* o );
bcore_source_file_s* bcore_source_file_s_clone(     const bcore_source_file_s* o );
void                 bcore_source_file_s_open(            bcore_source_file_s* o ); // (re)opens file
void                 bcore_source_file_s_close(           bcore_source_file_s* o ); // closes file if open
uz_t                 bcore_source_file_s_get_data(        bcore_source_file_s* o, vd_t data, uz_t size );
bl_t                 bcore_source_file_s_eos(       const bcore_source_file_s* o );
sc_t                 bcore_source_file_s_get_file(  const bcore_source_file_s* o );
s3_t                 bcore_source_file_s_get_index( const bcore_source_file_s* o );
void                 bcore_source_file_s_set_index(       bcore_source_file_s* o, s3_t index );

/// obtains context information for index position (only for text files)
void bcore_source_file_s_get_line_col_context( bcore_source_file_s* o, s3_t index, uz_t* p_line, uz_t* p_col, st_s* context );

/// See bcore_file.h for file system related functions

/**********************************************************************************************************************/

/** bcore_source_point_s represents a source (by weak reference) and an index.
 *  It is intended to be used to traceback a specific source location.
 *  It is typically used for generating descriptive messages (e.g. parse errors.)
 */
BCORE_DECLARE_OBJECT( bcore_source_point_s )
{
    aware_t _;
    bcore_source* source; // shallow reference
    sz_t index;
};

void bcore_source_point_s_set(               bcore_source_point_s* o, bcore_source* source );
void bcore_source_point_s_parse_err_fv( const bcore_source_point_s* o, sc_t format, va_list args );
void bcore_source_point_s_parse_err_fa( const bcore_source_point_s* o, sc_t format, ... );
void bcore_source_point_s_parse_msg_fv( const bcore_source_point_s* o, sc_t format, va_list args );
void bcore_source_point_s_parse_msg_fa( const bcore_source_point_s* o, sc_t format, ... );
void bcore_source_point_s_parse_msg_to_sink_fv( const bcore_source_point_s* o, bcore_sink* sink, sc_t format, va_list args );
void bcore_source_point_s_parse_msg_to_sink_fa( const bcore_source_point_s* o, bcore_sink* sink, sc_t format, ... );

/// generates a parse error message and pushes it to the error stack (see bcore_error_manager.h); returns err_id
er_t bcore_source_point_s_parse_err_to_em_fv( const bcore_source_point_s* o, er_t err_id, sc_t format, va_list args );
er_t bcore_source_point_s_parse_err_to_em_fa( const bcore_source_point_s* o, er_t err_id, sc_t format, ... );

/**********************************************************************************************************************/
// syntactic sugar

/// opens a (string-)buffered file for reading
bcore_source_chain_s* bcore_source_open_file( sc_t file_name );

/**********************************************************************************************************************/

vd_t bcore_sources_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BCORE_SOURCES_H
