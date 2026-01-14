/** Author and Copyright 2026 Johannes Bernhard Steffens
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

/** Specialized parse functions */

#ifndef BCORE_PARSE_H
#define BCORE_PARSE_H

#include "bcore_sr.h"
#include "bcore.xo.h"
#include "bcore_hmap_name.h"
#include "bcore_x_source.h"

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( bcore_parse, x_inst )

#ifdef XOILA_SECTION

embed "bcore_parse.x";

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func er_t number_literal_to_f3( m x_source* source, m f3_t* val )
{
    m$* sr = sr_s!^;
    :number_literal_to_sr( source, sr );
    if( val ) val.0 = sr.to_f3();
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func er_t number_literal_to_s3( m x_source* source, m s3_t* val )
{
    m$* sr = sr_s!^;
    :number_literal_to_sr( source, sr );
    if( val ) val.0 = sr.to_s3();
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func er_t st_number_literal_to_f3( st_s* st, m f3_t* val ) = :number_literal_to_f3( bcore_source_string_s_create_from_string( st )^, val );
func er_t st_number_literal_to_s3( st_s* st, m s3_t* val ) = :number_literal_to_s3( bcore_source_string_s_create_from_string( st )^, val );

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

#endif // XOILA_SECTION

#endif  // BCORE_PARSE_H
