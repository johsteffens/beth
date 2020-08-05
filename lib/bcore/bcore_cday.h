/** Author and Copyright 2019 Johannes Bernhard Steffens
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

/**********************************************************************************************************************/

/** Calendar-Day encoded in s2_t integer. (Originally conceived for project crossing_guard)
 *
 *  cday is represented as singed integer with value zero mapped to the date 1900-03-01.
 *  ymd (year-month-day) is a structure encoding a date in dedicated integers.
 *
 *  Conversions between cday and ymd are currently supported within range [1900-03-01, 2100-02-28].
 *  Weeknumber computation is according to ISO 8601
 *
 *  Date string notations are in the format "YYYY-MM-DD".
 *
 */

/**********************************************************************************************************************/

#ifndef BCORE_CDAY_H
#define BCORE_CDAY_H

#include "bcore_xoila.h"
#include "bcore_xoi_out.h"
#include "bcore_trait.h"
#include "bcore_st.h"
#include "bcore_spect_source.h"
#include "bcore_spect_sink.h"

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

#ifdef TYPEOF_bcore_cday
XOILA_DEFINE_GROUP( bcore_cday, bcore_inst )
#ifdef PLANT_SECTION

stamp :ymd = aware : { s2_t y; s2_t m; s2_t d; }; // year - month - day
stamp :utc = aware : { s2_t cday; s2_t ms; };     // utc-time stamp: cday, milliseconds from 0:00 into the day

#endif // PLANT_SECTION
#endif // TYPEOF_bcore_cday

// ---------------------------------------------------------------------------------------------------------------------
// ymd

/// conversion from cday
void bcore_cday_ymd_s_from_cday( bcore_cday_ymd_s* o, s2_t cday );

/// conversion from cday
bcore_cday_ymd_s* bcore_cday_ymd_s_create_from_cday( s2_t cday );

/// copies from text source in the format "YYYY-MM-DD"
void bcore_cday_ymd_s_from_source( bcore_cday_ymd_s* o, bcore_source* source );
void bcore_cday_ymd_s_from_string( bcore_cday_ymd_s* o, const st_s* st );
void bcore_cday_ymd_s_from_sc(     bcore_cday_ymd_s* o, sc_t sc );

/// pushes ymd to sink in the format "YYYY-MM-DD"
void bcore_cday_ymd_s_to_sink(        const bcore_cday_ymd_s* o, bcore_sink* sink );
void bcore_cday_ymd_s_to_string(      const bcore_cday_ymd_s* o, st_s* st );
void bcore_cday_ymd_s_push_to_string( const bcore_cday_ymd_s* o, st_s* st );

/// pushes ymd to sink in the format "YYYY-MM-DD"
void bcore_cday_ymd_s_to_sink( const bcore_cday_ymd_s* o, bcore_sink* sink );

/// conversion ymd to cday
s2_t bcore_cday_ymd_s_to_cday( const bcore_cday_ymd_s* o );

/// wnum == weeknumber of the year according to ISO 8601
s2_t bcore_cday_ymd_s_to_wnum( const bcore_cday_ymd_s* o );

/// cday of posix epoch (1970-01-01)
static inline s2_t bcore_cday_of_epoch( void ) { return 25508; }

// ---------------------------------------------------------------------------------------------------------------------
// cday

/// wnum == weeknumber of the year according to ISO 8601
s2_t bcore_cday_to_wnum( s2_t cday );

/// wday == weekday: 0 = monday, ..., 6 = sunday
s2_t bcore_cday_to_wday( s2_t cday );

/// 0 -> "mo", 1 -> "tu", ... , 6 -> "su"
sc_t bcore_wday_to_sc( s2_t wday );

/// converts cday to format "YYYY-MM-DD"
void bcore_cday_to_sink(        s2_t cday, bcore_sink* sink );
void bcore_cday_to_string(      s2_t cday, st_s* st );
void bcore_cday_push_to_string( s2_t cday, st_s* st );

/// pushes cday to text-sink with format "YYYY-MM-DD"
void bcore_cday_to_sink( s2_t cday, bcore_sink* sink );

/// computes cday from format "YYYY-MM-DD"
s2_t bcore_cday_from_source( bcore_source* source );
s2_t bcore_cday_from_string( const st_s* st );
s2_t bcore_cday_from_sc( sc_t sc );

// ---------------------------------------------------------------------------------------------------------------------
// time

/// obtains the current UTC time from the system
void bcore_cday_utc_s_from_system( bcore_cday_utc_s* o );

/// adjusts values in case ms exceeds the range of the specified cday
void bcore_cday_utc_s_normalize( bcore_cday_utc_s* o );

/// converts time into ISO 8601 time format YYYY-MM-DDThh:mm:ssZ
void bcore_cday_utc_s_to_sink(        const bcore_cday_utc_s* o, bcore_sink* sink );
void bcore_cday_utc_s_to_string(      const bcore_cday_utc_s* o, st_s* st );
void bcore_cday_utc_s_push_to_string( const bcore_cday_utc_s* o, st_s* st );

/// converts time from ISO 8601 time format YYYY-MM-DDThh:mm:ssZ
void bcore_cday_utc_s_from_source( bcore_cday_utc_s* o, bcore_source* source );
void bcore_cday_utc_s_from_string( bcore_cday_utc_s* o, const st_s* st );
void bcore_cday_utc_s_from_sc(     bcore_cday_utc_s* o, sc_t sc );

/// returns time in ms from cday-epoch (1900-03-01T00:00:00Z)
s3_t bcore_cday_utc_s_to_ms( const bcore_cday_utc_s* o );

/// returns the time difference (o-b) in milliseconds
s3_t bcore_cday_utc_s_diff_ms( const bcore_cday_utc_s* o, const bcore_cday_utc_s* b );

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

vd_t bcore_cday_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BCORE_CDAY_H
