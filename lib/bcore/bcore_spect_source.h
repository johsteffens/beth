/** Copyright 2017 Johannes Bernhard Steffens
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

#ifndef BCORE_SPECT_SOURCE_H
#define BCORE_SPECT_SOURCE_H

#include "bcore_first.h"
#include "bcore_types.h"
#include "bcore_feature.h"
#include "bcore_spect.h"
#include "bcore_spect_inst.h"

BCORE_FORWARD_OBJECT( bcore_sink );

/// optional source features:
typedef void (*bcore_source_fp_parse_fv )(       vd_t o, sc_t format, va_list args ); // parse function
typedef void (*bcore_source_fp_set_supplier)(    vd_t o, vd_t supplier );             // to create a chain of source units
typedef bl_t (*bcore_source_fp_eos)(             vc_t o );                            // (required) end of source has been reached
typedef sc_t (*bcore_source_fp_get_file)(        vc_t o );                            // returns file name if source is of a file system or "" otherwise
typedef uz_t (*bcore_source_fp_get_index)(       vc_t o );                            // returns the current read index (0 if not supported)
typedef void (*bcore_source_fp_set_index)(       vd_t o, uz_t index );                // sets read index to requested position (error if not supported)
typedef void (*bcore_source_fp_context_to_sink)( vc_t o, bcore_sink* sink );          // writes context information to sink

/// required source features:
/// awareness
BCORE_DECLARE_SPECT( bcore_source )
{
    bcore_spect_header_s header;
    bcore_fp_flow_src               get_data;
    bcore_source_fp_context_to_sink context_to_sink;
    bcore_source_fp_parse_fv        parse_fv;
    bcore_source_fp_set_supplier    set_supplier;
    bcore_source_fp_eos             eos;
    bcore_source_fp_get_file        get_file;
    bcore_source_fp_get_index       get_index;
    bcore_source_fp_set_index       set_index;
};

static inline
u0_t bcore_source_default_get_u0( const bcore_source_s* p, bcore_source* o )
{
    u0_t v = 0;
    p->get_data( o, &v, 1 );
    return v;
}

/// Parse functions can be used for scannlerless parsing or lexing.

/// Reads and consumes data. Returns number of bytes consumed.
BCORE_FUNC_SPECT_CONST0_RET1_ARG2_MAP1( bcore_source, get_data, uz_t, vd_t, data, uz_t, size )

/// Reads and consumes a single byte. Returns 0 in case eos is reached.
BCORE_FUNC_SPECT_CONST0_RET1_ARG0_MAP0( bcore_source, get_u0, u0_t )

/// Reads data at current position without consumption. Returns number of bytes copied.
BCORE_FUNC_SPECT_CONST0_RET1_ARG2_MAP0( bcore_source, inspect_data, uz_t, vd_t, data, uz_t, size )

/// Reads a single byte current position without consumption. Returns 0 in case eos is reached.
BCORE_FUNC_SPECT_CONST0_RET1_ARG0_MAP0( bcore_source, inspect_u0, u0_t )

/// Reads and consumes data using formatted parsing according to st_s_parse_fv fromat rules.
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP1( bcore_source, parse_fv,           sc_t, format, va_list, args )

/// Writes context info about current read position to sink (used to create descriptive parse messages)
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP0( bcore_source, context_to_sink, bcore_sink*, sink )

/// Produces a parse message
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_source, parse_errvf,  sc_t, format, va_list, args )
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_source, parse_err_fv, sc_t, format, va_list, args )
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_source, parse_msgvf,  sc_t, format, va_list, args )
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_source, parse_msg_fv, sc_t, format, va_list, args )
BCORE_FUNC_SPECT_CONST0_RET0_ARG3_MAP0( bcore_source, parse_msg_to_sink_fv, bcore_sink*, sink, sc_t, format, va_list, args )

/// Returns the boolean of a formatted parse. 'format' must yield just one bool, which is returned.
BCORE_FUNC_SPECT_CONST0_RET1_ARG1_MAP0( bcore_source, parse_bl_fa,  bl_t, sc_t, format )

/// Indicates end-of-stream
BCORE_FUNC_SPECT_CONST1_RET1_ARG0_MAP1( bcore_source, eos, bl_t )

/// Retrieves the file name associated with a stream if available
BCORE_FUNC_SPECT_CONST1_RET1_ARG0_MAP0( bcore_source, get_file, sc_t )

/// Defines the supplier of a stream. Error when not supported.
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP1( bcore_source, set_supplier, vd_t, supplier )

/** Random access:
 *  Functions set_index, get_index provide random access within a specific stream configuration.
 *  Access features need not be supported.
 *  Unsupported set_index produces an error. Unsupported get_index returns always 0.
 *  Index values and value range is stream dependent and may also be negative.
 *  When supported: set_index is able to reset the stream to any read-position previously obtained via get_index.
 */
BCORE_FUNC_SPECT_CONST1_RET1_ARG0_MAP0( bcore_source, get_index,  s3_t )
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP0( bcore_source, set_index,  s3_t, index )
BCORE_FUNC_SPECT_CONST0_RET0_ARG1_MAP0( bcore_source, move_index, s3_t, delta ) // set_index( get_index() + delta );

void bcore_source_p_parse_msg_to_sink_fa( const bcore_source_s* p, bcore_source* o, bcore_sink* sink, sc_t format, ... );
void bcore_source_a_parse_msg_to_sink_fa( bcore_source* o, bcore_sink* sink, sc_t format, ... );
void bcore_source_x_parse_msg_to_sink_fa( sr_s o, bcore_sink* sink, sc_t format, ... );
void bcore_source_r_parse_msg_to_sink_fa( const sr_s* o, bcore_sink* sink, sc_t format, ... );

void bcore_source_p_parse_msg_fa( const bcore_source_s* p, bcore_source* o, sc_t format, ... );
void bcore_source_a_parse_msg_fa( bcore_source* o, sc_t format, ... );
void bcore_source_x_parse_msg_fa( sr_s o, sc_t format, ... );
void bcore_source_r_parse_msg_fa( const sr_s* o, sc_t format, ... );

void bcore_source_p_parse_err_fa( const bcore_source_s* p, bcore_source* o, sc_t format, ... );
void bcore_source_p_parse_errf  ( const bcore_source_s* p, bcore_source* o, sc_t format, ... );
void bcore_source_p_parse_fa    ( const bcore_source_s* p, bcore_source* o, sc_t format, ... );
void bcore_source_a_parse_err_fa( bcore_source* o, sc_t format, ... );
void bcore_source_a_parse_fa    ( bcore_source* o, sc_t format, ... );
void bcore_source_x_parse_fa    ( sr_s o, sc_t format, ... );
void bcore_source_x_parse_errf  ( sr_s o, sc_t format, ... );
void bcore_source_x_parse_err_fa( sr_s o, sc_t format, ... );
void bcore_source_r_parse_fa    ( const sr_s* o, sc_t format, ... );
void bcore_source_r_parse_errf  ( const sr_s* o, sc_t format, ... );
void bcore_source_r_parse_err_fa( const sr_s* o, sc_t format, ... );

bcore_source* bcore_source_t_clone( tp_t type );
void bcore_source_a_discard( bcore_source* o );
void bcore_source_a_detach( bcore_source** o );
void bcore_source_a_attach( bcore_source** o, bcore_source* src );

vd_t bcore_spect_source_signal_handler( const bcore_signal_s* o );

#endif // BCORE_SPECT_SOURCE_H
