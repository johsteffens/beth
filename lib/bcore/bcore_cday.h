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
#include "bcore.xo.h"
#include "bcore_trait.h"
#include "bcore_st.h"
#include "bcore_spect_source.h"
#include "bcore_spect_sink.h"

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

#ifdef TYPEOF_bcore_cday
XOILA_DEFINE_GROUP( bcore_cday, bcore_inst )
#ifdef XOILA_SECTION

/**********************************************************************************************************************/
/// plain cday functions

// ---------------------------------------------------------------------------------------------------------------------

/// cday of posix epoch (1970-01-01)
func s2_t of_epoch() = 25508;

/// wnum == weeknumber of the year according to ISO 8601
func s2_t to_wnum( s2_t cday );

/// wday == weekday: 0 = monday, ..., 6 = sunday
func s2_t to_wday( s2_t cday );

/// 0 -> "mo", 1 -> "tu", ... , 6 -> "su"
func sc_t wday_to_sc( s2_t wday );

/// converts cday to format "YYYY-MM-DD"
func void to_sink(        s2_t cday, m bcore_sink* sink );
func void to_string(      s2_t cday, m st_s* st );
func void push_to_string( s2_t cday, m st_s* st );

/// computes cday from format "YYYY-MM-DD"
func s2_t from_source( m bcore_source* source );
func s2_t from_sc( sc_t sc );
func s2_t from_string( c st_s* st ) = :from_sc( st.sc );

// ---------------------------------------------------------------------------------------------------------------------

/// year-month-day representation of cday
stamp :ymd_s :
{
    s2_t y; // year
    s2_t m; // month
    s2_t d; // day

    /// conversion from cday
    func o from_cday( m@* o, s2_t cday );

    /// copies from text source in the format "YYYY-MM-DD"
    func o from_source( m@* o, m bcore_source* source );
    func o from_sc(     m@* o, sc_t sc );
    func o from_string( m@* o, c st_s* st ) = o.from_sc( st.sc );

    func d @* create_from_cday( s2_t cday ) = @!.from_cday( cday );

    /// pushes ymd to sink in the format "YYYY-MM-DD"
    func o to_sink(        c@* o, m bcore_sink* sink ) sink.push_fa( "#<s2_t>-#pl2'0'{#<s2_t>}-#pl2'0'{#<s2_t>}", o.y, o.m, o.d );
    func o to_string(      c@* o, m st_s* st ) { st.clear(); = o.to_sink( st ); }
    func o push_to_string( c@* o, m st_s* st ) = o.to_sink( st );

    /// conversion ymd to cday
    func s2_t to_cday( c@* o );

    /// wnum == weeknumber of the year according to ISO 8601
    func s2_t to_wnum( c@* o );
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

/// utc-time stamp: cday, milliseconds from 0:00 into the day
stamp :utc_s bcore_cday
{
    s2_t cday;
    s2_t ms;

    /// obtains the current UTC time from the system
    func o from_system( m@* o );

    /// adjusts values in case ms exceeds the range of the specified cday
    func o normalize( m@* o );

    /// converts time into ISO 8601 time format YYYY-MM-DDThh:mm:ssZ
    func o to_sink(        c@* o, m bcore_sink* sink );
    func o to_string(      c@* o, m st_s* st ) { st.clear(); = o.to_sink( st ); }
    func o push_to_string( c@* o, m st_s* st ) = o.to_sink( st );

    /// converts time from ISO 8601 time format YYYY-MM-DDThh:mm:ssZ
    func o from_source( m@* o, m bcore_source* source );
    func o from_sc(     m@* o, sc_t sc );
    func o from_string( m@* o, c st_s* st ) = o.from_sc( st.sc );

    /// returns time in ms from cday-epoch (1900-03-01T00:00:00Z)
    func s3_t to_ms( c@* o );

    /// returns the time difference (o-b) in milliseconds
    func s3_t diff_ms( c@* o, c@* b );
}

// ---------------------------------------------------------------------------------------------------------------------

#endif // XOILA_SECTION
#endif // TYPEOF_bcore_cday

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

vd_t bcore_cday_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BCORE_CDAY_H
