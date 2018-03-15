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
#include "bcore_features.h"
#include "bcore_flect.h"

/// optional source features:
typedef void (*bcore_source_fp_parse_fv )(    vd_t o, sc_t format, va_list args ); // parse function
typedef void (*bcore_source_fp_set_supplier)( vd_t o, vd_t supplier );             // to create a chain of source units
typedef bl_t (*bcore_source_fp_eos)(          vc_t o );                            // (required) end of source has been reached
typedef sc_t (*bcore_source_fp_get_file)(     vc_t o );                            // returns file name if source is of a file system or "" otherwise
typedef sz_t (*bcore_source_fp_get_index)(    vc_t o );                            // returns the current read index (0 if not supported)
typedef void (*bcore_source_fp_set_index)(    vd_t o, sz_t index );                // sets read index to requested position (error if not supported)

/// required source features:
/// awareness

typedef struct bcore_source_s bcore_source_s;
typedef struct bcore_source_s
{
    aware_t p_type; // type of perspective
    tp_t    o_type; // type of object

    bcore_fp_flow_src            fp_flow_src;
    bcore_fp_logvf               fp_parse_errvf;
    bcore_source_fp_parse_fv     fp_parse_fv;
    bcore_source_fp_set_supplier fp_set_supplier;
    bcore_source_fp_eos          fp_eos;
    bcore_source_fp_get_file     fp_get_file;
    bcore_source_fp_get_index    fp_get_index;
    bcore_source_fp_set_index    fp_set_index;

} bcore_source_s;

const bcore_source_s* bcore_source_s_get_typed( tp_t type );
const bcore_source_s* bcore_source_s_get_aware( vc_t obj );

sz_t bcore_source_spect_get_data    ( const bcore_source_s* p, vd_t o, vd_t data, sz_t size ); // returns number of bytes read
u0_t bcore_source_spect_get_u0      ( const bcore_source_s* p, vd_t o ); // reads single byte and returns it (returns 0 in case eos is reached
void bcore_source_spect_parse_fv    ( const bcore_source_s* p, vd_t o, sc_t format, va_list args );
void bcore_source_spect_parse_fa    ( const bcore_source_s* p, vd_t o, sc_t format, ... );
void bcore_source_spect_parse_errvf ( const bcore_source_s* p, vd_t o, sc_t format, va_list args );
void bcore_source_spect_parse_errf  ( const bcore_source_s* p, vd_t o, sc_t format, ... );
void bcore_source_spect_parse_err_fv( const bcore_source_s* p, vd_t o, sc_t format, va_list args );
void bcore_source_spect_parse_err_fa( const bcore_source_s* p, vd_t o, sc_t format, ... );
bl_t bcore_source_spect_parse_bl_fa ( const bcore_source_s* p, vd_t o, sc_t format      ); // format must yield just one bool, which is returned
void bcore_source_spect_set_supplier( const bcore_source_s* p, vd_t o, vd_t supplier    ); // error when not supported
bl_t bcore_source_spect_eos         ( const bcore_source_s* p, vc_t o );
sc_t bcore_source_spect_get_file    ( const bcore_source_s* p, vc_t o );

/** Random access:
 *  Functions set_index, get_index provide random access within a specific stream configuration.
 *  Access features need not be supported.
 *  Unsupported set_index produces an error. Unsupported get_index returns always 0.
 *  Index values and value range is stream dependent and may also be negative.
 *  When supported: set_index can reset the stream(-chain) to any read-position previously obtained via get_index.
 */
s3_t bcore_source_spect_get_index   ( const bcore_source_s* p, vc_t o );
void bcore_source_spect_set_index   ( const bcore_source_s* p, vd_t o, s3_t index );

sz_t bcore_source_aware_get_data    ( vd_t o, vd_t data, sz_t size );
u0_t bcore_source_aware_get_u0      ( vd_t o );
void bcore_source_aware_parse_fv    ( vd_t o, sc_t format, va_list args );
void bcore_source_aware_parse_fa    ( vd_t o, sc_t format, ... );
void bcore_source_aware_parse_errvf ( vd_t o, sc_t format, va_list args );
void bcore_source_aware_parse_errf  ( vd_t o, sc_t format, ... );
void bcore_source_aware_parse_err_fv( vd_t o, sc_t format, va_list args );
void bcore_source_aware_parse_err_fa( vd_t o, sc_t format, ... );
bl_t bcore_source_aware_parse_bl_fa ( vd_t o, sc_t format      ); // format must yield just one bool, which is returned
void bcore_source_aware_set_supplier( vd_t o, vd_t supplier    ); // error when not supported
bl_t bcore_source_aware_eos         ( vc_t o );
sc_t bcore_source_aware_get_file    ( vc_t o );
s3_t bcore_source_aware_get_index   ( vc_t o );
void bcore_source_aware_set_index   ( vd_t o, s3_t index );

sz_t bcore_source_get_data    ( sr_s o, vd_t data, sz_t size );
u0_t bcore_source_get_u0      ( sr_s o );
void bcore_source_parse_fv    ( sr_s o, sc_t format, va_list args );
void bcore_source_parse_fa    ( sr_s o, sc_t format, ... );
void bcore_source_parse_errvf ( sr_s o, sc_t format, va_list args );
void bcore_source_parse_errf  ( sr_s o, sc_t format, ... );
void bcore_source_parse_err_fv( sr_s o, sc_t format, va_list args );
void bcore_source_parse_err_fa( sr_s o, sc_t format, ... );
bl_t bcore_source_parse_bl_fa ( sr_s o, sc_t format      ); // format must yield just one bool, which is returned
void bcore_source_set_supplier( sr_s o, vd_t supplier    ); // error when not supported
bl_t bcore_source_eos         ( sr_s o );
sc_t bcore_source_get_file    ( sr_s o );
s3_t bcore_source_get_index   ( sr_s o );
void bcore_source_set_index   ( sr_s o, s3_t index );

sz_t bcore_source_q_get_data    ( const sr_s* o, vd_t data, sz_t size );
u0_t bcore_source_q_get_u0      ( const sr_s* o );
void bcore_source_q_parse_fv    ( const sr_s* o, sc_t format, va_list args );
void bcore_source_q_parse_fa    ( const sr_s* o, sc_t format, ... );
void bcore_source_q_parse_errvf ( const sr_s* o, sc_t format, va_list args );
void bcore_source_q_parse_errf  ( const sr_s* o, sc_t format, ... );
void bcore_source_q_parse_err_fv( const sr_s* o, sc_t format, va_list args );
void bcore_source_q_parse_err_fa( const sr_s* o, sc_t format, ... );
bl_t bcore_source_q_parse_bl_fa ( const sr_s* o, sc_t format      ); // format must yield just one bool, which is returned
void bcore_source_q_set_supplier( const sr_s* o, vd_t supplier    ); // error when not supported
bl_t bcore_source_q_eos         ( const sr_s* o );
sc_t bcore_source_q_get_file    ( const sr_s* o );
s3_t bcore_source_q_get_index   ( const sr_s* o );
void bcore_source_q_set_index   ( const sr_s* o, s3_t index );

vd_t bcore_spect_source_signal_handler( const bcore_signal_s* o );

#endif // BCORE_SPECT_SOURCE_H
