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

#include "bcore_cday.h"
#include "bcore_life.h"
#include "bcore_sinks.h"
#include "bcore_error_manager.h"

// ---------------------------------------------------------------------------------------------------------------------

void bcore_cday_ymd_s_check_plausibility( const bcore_cday_ymd_s* o )
{
    ASSERT( ( o->y >= 1900 ) && ( o->y <= 2100 ) );
    ASSERT( o->m >= 1 && o->m <= 12 );
    ASSERT( o->d >= 1 && o->d <= 31 );

    if( o->y == 1900 ) ASSERT( o->m >= 3 );
    if( o->y == 2100 ) ASSERT( o->m <  3 );

    switch( o->m )
    {
        case  1: ASSERT( o->d <= 31 ); break;
        case  2: ASSERT( o->d <= 29 ); break;
        case  3: ASSERT( o->d <= 31 ); break;
        case  4: ASSERT( o->d <= 30 ); break;
        case  5: ASSERT( o->d <= 31 ); break;
        case  6: ASSERT( o->d <= 30 ); break;
        case  7: ASSERT( o->d <= 31 ); break;
        case  8: ASSERT( o->d <= 31 ); break;
        case  9: ASSERT( o->d <= 30 ); break;
        case 10: ASSERT( o->d <= 31 ); break;
        case 11: ASSERT( o->d <= 30 ); break;
        case 12: ASSERT( o->d <= 31 ); break;
        default: break;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

er_t bcore_cday_ymd_s_em_check_plausibility( const bcore_cday_ymd_s* o )
{
    er_t er = 0;
    er = EM_ASSERT( ( o->y >= 1900 ) && ( o->y <= 2100 ) );
    if( er ) return er;
    er = EM_ASSERT( o->m >= 1 && o->m <= 12 );
    if( er ) return er;
    er = EM_ASSERT( o->d >= 1 && o->d <= 31 );
    if( er ) return er;

    if( o->y == 1900 ) er = EM_ASSERT( o->m >= 3 );
    if( er ) return er;
    if( o->y == 2100 ) er = EM_ASSERT( o->m <  3 );
    if( er ) return er;

    switch( o->m )
    {
        case  1: er = EM_ASSERT( o->d <= 31 ); break;
        case  2: er = EM_ASSERT( o->d <= 29 ); break;
        case  3: er = EM_ASSERT( o->d <= 31 ); break;
        case  4: er = EM_ASSERT( o->d <= 30 ); break;
        case  5: er = EM_ASSERT( o->d <= 31 ); break;
        case  6: er = EM_ASSERT( o->d <= 30 ); break;
        case  7: er = EM_ASSERT( o->d <= 31 ); break;
        case  8: er = EM_ASSERT( o->d <= 31 ); break;
        case  9: er = EM_ASSERT( o->d <= 30 ); break;
        case 10: er = EM_ASSERT( o->d <= 31 ); break;
        case 11: er = EM_ASSERT( o->d <= 30 ); break;
        case 12: er = EM_ASSERT( o->d <= 31 ); break;
        default: break;
    }

    return er;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_cday_ymd_s* bcore_cday_ymd_s_from_cday( bcore_cday_ymd_s* o, s2_t cday )
{
    s2_t lyr = cday / ( 365 * 4 + 1 );
    s2_t ldy = cday - lyr * ( 365 * 4 + 1 );
    s2_t qyr = ldy / 365;
    qyr = ( qyr > 3 ) ? 3 : qyr;
    s2_t yr = ( lyr * 4 ) + qyr;
    s2_t dy = ldy - qyr * 365;
    s2_t mt = 3;

    s2_t dtbl[] = { 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 31 };

    for( s2_t i = 0; i < 11; i++ )
    {
        if( dtbl[ i ] > dy ) break;
        dy -= dtbl[ i ];
        mt++;
    }

    o->d   = dy + 1;
    o->m = ( mt <= 12 ) ? mt : mt - 12;
    o->y  = 1900 + yr + ( ( mt <= 12 ) ? 0 : 1 );

    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

er_t bcore_cday_ymd_s_em_from_cday( bcore_cday_ymd_s* o, s2_t cday )
{
    s2_t lyr = cday / ( 365 * 4 + 1 );
    s2_t ldy = cday - lyr * ( 365 * 4 + 1 );
    s2_t qyr = ldy / 365;
    qyr = ( qyr > 3 ) ? 3 : qyr;
    s2_t yr = ( lyr * 4 ) + qyr;
    s2_t dy = ldy - qyr * 365;
    s2_t mt = 3;

    s2_t dtbl[] = { 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 31 };

    for( s2_t i = 0; i < 11; i++ )
    {
        if( dtbl[ i ] > dy ) break;
        dy -= dtbl[ i ];
        mt++;
    }

    o->d   = dy + 1;
    o->m = ( mt <= 12 ) ? mt : mt - 12;
    o->y  = 1900 + yr + ( ( mt <= 12 ) ? 0 : 1 );

    return bcore_cday_ymd_s_em_check_plausibility( o );
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_cday_ymd_s* bcore_cday_ymd_s_from_source( bcore_cday_ymd_s* o, x_source* source )
{
    char y[4] = { 0 };
    char m[2] = { 0 };
    char d[2] = { 0 };

    // We deliberately parse characters (rather than using direct integer conversions) in order to enforce the correct format.
    x_source_parse_fa( source, "#<char*>#<char*>#<char*>#<char*>-#<char*>#<char*>-#<char*>#<char*>", &y[0], &y[1], &y[2], &y[3], &m[0], &m[1], &d[0], &d[1] );

    for( sz_t i = 0; i < 4; i++ ) ASSERT( y[i] >= '0' && y[i] <= '9' );
    for( sz_t i = 0; i < 2; i++ ) ASSERT( m[i] >= '0' && m[i] <= '9' );
    for( sz_t i = 0; i < 2; i++ ) ASSERT( d[i] >= '0' && d[i] <= '9' );

    o->y = ( y[0] - '0' ) * 1000 + ( y[1] - '0' ) * 100 + ( y[2] - '0' ) * 10 + ( y[3] - '0' );
    o->m = ( m[0] - '0' ) * 10   + ( m[1] - '0' );
    o->d = ( d[0] - '0' ) * 10   + ( d[1] - '0' );

    bcore_cday_ymd_s_check_plausibility( o );

    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

er_t bcore_cday_ymd_s_em_from_source( bcore_cday_ymd_s* o, x_source* source )
{
    char y[4] = { 0 };
    char m[2] = { 0 };
    char d[2] = { 0 };

    // We deliberately parse characters (rather than using direct integer conversions) in order to enforce the correct format.

    er_t er = x_source_parse_fa( source, "#<char*>#<char*>#<char*>#<char*>-#<char*>#<char*>-#<char*>#<char*>", &y[0], &y[1], &y[2], &y[3], &m[0], &m[1], &d[0], &d[1] );
    if( er ) return er;

    for( sz_t i = 0; i < 4; i++ ) ASSERT( y[i] >= '0' && y[i] <= '9' );
    for( sz_t i = 0; i < 2; i++ ) ASSERT( m[i] >= '0' && m[i] <= '9' );
    for( sz_t i = 0; i < 2; i++ ) ASSERT( d[i] >= '0' && d[i] <= '9' );

    o->y = ( y[0] - '0' ) * 1000 + ( y[1] - '0' ) * 100 + ( y[2] - '0' ) * 10 + ( y[3] - '0' );
    o->m = ( m[0] - '0' ) * 10   + ( m[1] - '0' );
    o->d = ( d[0] - '0' ) * 10   + ( d[1] - '0' );

    return bcore_cday_ymd_s_em_check_plausibility( o );
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_cday_ymd_s* bcore_cday_ymd_s_from_sc( bcore_cday_ymd_s* o, sc_t sc )
{
    st_s st;
    st_s_init_weak_sc( &st, sc );
    bcore_cday_ymd_s_from_source( o, ( x_source* )&st );
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

er_t bcore_cday_ymd_s_em_from_sc( bcore_cday_ymd_s* o, sc_t sc )
{
    st_s st;
    st_s_init_weak_sc( &st, sc );
    return bcore_cday_ymd_s_em_from_source( o, ( x_source* )&st );
}

// ---------------------------------------------------------------------------------------------------------------------

s2_t bcore_cday_ymd_s_to_cday( const bcore_cday_ymd_s* o )
{
    bcore_cday_ymd_s_check_plausibility( o );

    s2_t yr = o->y - 1900;
    s2_t leap_days = 1 + ( yr / 4 );
    if( ( o->m < 3 ) && ( yr % 4 == 0 ) ) leap_days--;

    s2_t sum = yr * 365 + leap_days;

    switch( o->m - 1 )
    {
        case 11: sum += 30;
        case 10: sum += 31;
        case  9: sum += 30;
        case  8: sum += 31;
        case  7: sum += 31;
        case  6: sum += 30;
        case  5: sum += 31;
        case  4: sum += 30;
        case  3: sum += 31;
        case  2: sum += 28;
        case  1: sum += 31;
        case  0:
        default: break;
    }

    sum += o->d - 1;

    if( sum < 60 ) ERR( "Lowest possible cday '0' subceeded. 0 is mapped to 1900-03-01" );
    sum -= 60;

    return sum;
}

// ---------------------------------------------------------------------------------------------------------------------

s2_t bcore_cday_to_wday( s2_t cday )
{
    return ( cday + 3 ) % 7;
}

// ---------------------------------------------------------------------------------------------------------------------

sc_t bcore_cday_wday_to_sc( s2_t wday )
{
    switch( wday )
    {
        case 0: return "mo";
        case 1: return "tu";
        case 2: return "we";
        case 3: return "th";
        case 4: return "fr";
        case 5: return "sa";
        case 6: return "su";
        default: ERR_fa( "Invalid wday number '#<s2_t>'.", wday );
    }
    return "";
}

// ---------------------------------------------------------------------------------------------------------------------

s2_t bcore_cday_ymd_s_to_wnum( const bcore_cday_ymd_s* o )
{
    bcore_cday_ymd_s ymd_l;
    bcore_cday_ymd_s_init( &ymd_l );
    ymd_l.y = o->y;
    ymd_l.m = 1;
    ymd_l.d = 1;

    sz_t first_cday = bcore_cday_ymd_s_to_cday( &ymd_l );
    sz_t first_wday = bcore_cday_to_wday( first_cday );

    // monday of week of first_cday
    sz_t mo1_cday = first_cday - first_wday;
    if( first_wday > 3 ) mo1_cday += 7;

    sz_t cur_cday = bcore_cday_ymd_s_to_cday( o );

    if( cur_cday < mo1_cday ) // last week of last year
    {
        if( first_wday == 4 ) return 53;
        if( first_wday == 5 ) return ( ( ymd_l.y - 1 ) % 4 ) == 0 ? 53 : 52;
        return 52;
    }
    else
    {
        sz_t wnum = 1 + ( cur_cday - mo1_cday ) / 7;
        if( wnum == 53 )
        {
            if( ( first_wday == 3 ) || ( ( ( ymd_l.y % 4 ) == 0 ) && ( first_wday == 2 ) ) )
            {
                return 53;
            }
            else
            {
                return 1;
            }
        }
        return wnum;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

s2_t bcore_cday_to_wnum( s2_t cday )
{
    bcore_cday_ymd_s ymd_l;
    bcore_cday_ymd_s_init( &ymd_l );
    bcore_cday_ymd_s_from_cday( &ymd_l, cday );
    s2_t wnum = bcore_cday_ymd_s_to_wnum( &ymd_l );
    return wnum;
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_cday_push_to_string( s2_t cday, st_s* st )
{
    bcore_cday_ymd_s* ymd = bcore_cday_ymd_s_create_from_cday( cday );
    bcore_cday_ymd_s_push_to_string( ymd, st );
    bcore_cday_ymd_s_discard( ymd );
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_cday_to_sink( s2_t cday, bcore_sink* sink )
{
    bcore_cday_ymd_s* ymd = bcore_cday_ymd_s_create_from_cday( cday );
    bcore_cday_ymd_s_to_sink( ymd, sink );
    bcore_cday_ymd_s_discard( ymd );
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_cday_to_string( s2_t cday, st_s* st )
{
    st_s_clear( st );
    bcore_cday_push_to_string( cday, st );
}

// ---------------------------------------------------------------------------------------------------------------------

s2_t bcore_cday_from_sc( sc_t sc )
{
    bcore_cday_ymd_s* ymd = bcore_cday_ymd_s_create();
    bcore_cday_ymd_s_from_sc( ymd, sc );
    s2_t cday = bcore_cday_ymd_s_to_cday( ymd );
    bcore_cday_ymd_s_discard( ymd );
    return cday;
}

// ---------------------------------------------------------------------------------------------------------------------

s2_t bcore_cday_from_source( x_source* source )
{
    bcore_cday_ymd_s* ymd = bcore_cday_ymd_s_create();
    bcore_cday_ymd_s_from_source( ymd, source );
    s2_t cday = bcore_cday_ymd_s_to_cday( ymd );
    bcore_cday_ymd_s_discard( ymd );
    return cday;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// time

// ---------------------------------------------------------------------------------------------------------------------

bcore_cday_utc_s* bcore_cday_utc_s_from_system( bcore_cday_utc_s* o )
{
    u3_t stime = ( u3_t )time( NULL );
    s2_t epoch_days = stime / ( 24 * 3600 );
    s2_t seconds    = stime % ( 24 * 3600 );
    o->cday = bcore_cday_of_epoch() + epoch_days;
    o->ms = seconds * 1000;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_cday_utc_s* bcore_cday_utc_s_normalize( bcore_cday_utc_s* o )
{
    s2_t range = 24 * 3600 * 1000;
    while( o->ms <      0 ) { o->cday--; o->ms += range; }
    while( o->ms >= range ) { o->cday++; o->ms -= range; }
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

const bcore_cday_utc_s* bcore_cday_utc_s_to_sink( const bcore_cday_utc_s* o, bcore_sink* sink )
{
    bcore_cday_to_sink( o->cday, sink );
    s2_t secs = o->ms / 1000;
    s2_t hh = secs / 3600;
    secs = secs % 3600;
    s2_t mm = secs / 60;
    secs = secs % 60;
    s2_t ss = secs;
    bcore_sink_a_push_fa( sink, "T#pl2'0'{#<s2_t>}:#pl2'0'{#<s2_t>}:#pl2'0'{#<s2_t>}Z", hh, mm, ss );
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_cday_utc_s* bcore_cday_utc_s_from_source( bcore_cday_utc_s* o, x_source* source )
{
    o->cday = bcore_cday_from_source( source );
    s2_t hh = 0;
    s2_t mm = 0;
    s2_t ss = 0;
    x_source_parse_fa( source, "T#-?'0'#<s2_t*>:#-?'0'#<s2_t*>:#-?'0'#<s2_t*>Z", &hh, &mm, &ss );
    o->ms = ( hh * 3600 + mm * 60 + ss ) * 1000;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_cday_utc_s* bcore_cday_utc_s_from_sc( bcore_cday_utc_s* o, sc_t sc )
{
    st_s st;
    st_s_init_weak_sc( &st, sc );
    return bcore_cday_utc_s_from_source( o, ( x_source* )&st );
}

// ---------------------------------------------------------------------------------------------------------------------

s3_t bcore_cday_utc_s_to_ms( const bcore_cday_utc_s* o )
{
    return ( s3_t )o->cday * ( 24 * 3600 * 1000 ) + o->ms;
}

// ---------------------------------------------------------------------------------------------------------------------

s3_t bcore_cday_utc_s_diff_ms( const bcore_cday_utc_s* o, const bcore_cday_utc_s* b )
{
    return bcore_cday_utc_s_to_ms( o ) - bcore_cday_utc_s_to_ms( b );
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

static void selftest( void )
{
    BLM_INIT();

    bcore_cday_ymd_s* ymd = BLM_CREATE( bcore_cday_ymd_s );

    for( s2_t cday = 0; cday < 60000; cday++ )
    {
        bcore_cday_ymd_s_from_cday( ymd, cday );
        ASSERT( cday == bcore_cday_ymd_s_to_cday( ymd ) );
    }

    st_s* string = BLM_CREATE( st_s );

    bcore_cday_to_string( 0, string );
    ASSERT( st_s_equal_sc( string, "1900-03-01" ) );

    bcore_cday_to_string( 73048, string );
    ASSERT( st_s_equal_sc( string, "2100-02-28" ) );

    bcore_cday_ymd_s_from_sc( ymd, "2018-05-04" );
    bcore_cday_ymd_s_to_string( ymd, string );
    ASSERT( st_s_equal_sc( string, "2018-05-04" ) );

    bcore_cday_ymd_s_from_sc( ymd, "2012-12-29" );
    bcore_cday_ymd_s_to_string( ymd, string );
    ASSERT( st_s_equal_sc( string, "2012-12-29" ) );

    //bcore_msg_fa( "#<s2_t>\n", bcore_cday_from_sc( "1970-01-01" ) );

    ASSERT( bcore_cday_to_wnum( bcore_cday_from_sc( "2019-11-26" ) ) == 48 );
    ASSERT( bcore_cday_to_wday( bcore_cday_from_sc( "2019-11-26" ) ) ==  1 );
    ASSERT( bcore_cday_to_wnum( bcore_cday_from_sc( "1969-06-11" ) ) == 24 );
    ASSERT( bcore_cday_to_wnum( bcore_cday_from_sc( "2012-01-01" ) ) == 52 );
    ASSERT( bcore_cday_to_wnum( bcore_cday_from_sc( "2012-12-31" ) ) ==  1 );
    ASSERT( bcore_cday_to_wnum( bcore_cday_from_sc( "2020-12-31" ) ) == 53 );
    ASSERT( bcore_cday_to_wnum( bcore_cday_from_sc( "2027-01-01" ) ) == 53 );
    ASSERT( bcore_cday_to_wnum( bcore_cday_from_sc( "2027-01-09" ) ) ==  1 );

    bcore_cday_utc_s* utc1 = BLM_CREATE( bcore_cday_utc_s );
    bcore_cday_utc_s* utc2 = BLM_CREATE( bcore_cday_utc_s );
    bcore_cday_utc_s_from_system( utc1 );
    bcore_cday_utc_s_to_string( utc1, string );
    bcore_cday_utc_s_from_string( utc2, string );
    ASSERT( bcore_cday_utc_s_diff_ms( utc2, utc1 ) == 0 );

    bcore_cday_utc_s_from_sc( utc1, "2027-01-09T09:07:23Z" );
    ASSERT( utc1->ms == ( 9 * 3600 + 7 * 60 + 23 ) * 1000 );

    //bcore_sink_a_push_fa( BCORE_STDOUT, "#<sc_t>\n", string->sc );

    BLM_DOWN();
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

vd_t bcore_cday_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_cday" ) ) )
    {
        case TYPEOF_init1:
        {
        }
        break;

        case TYPEOF_down1:
        {
        }
        break;

        case TYPEOF_selftest:
        {
            selftest();
        }
        break;

        default: break;
    }

    return NULL;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/


