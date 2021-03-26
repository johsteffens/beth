/** Author and Copyright 2020 Johannes Bernhard Steffens
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

/** Beth instance group preferably in trait-line of aware x-instances.
 */

#ifndef BCORE_X_INST_H
#define BCORE_X_INST_H

#include "bcore.xo.h"

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( x_inst, bcore_inst )

#ifdef XOILA_SECTION

include 'h' "bcore_txt_ml.h";
include 'h' "bcore_bin_ml.h";
include 'h' "bcore_arr.h";
include 'h' "bcore_life.h";
include 'h' "bcore_sinks.h";
include 'h' "bcore_sources.h";

include 'c' "bcore_file.h";

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

/** Main function
 *  Implementing a function with this signature causes xoico to generate the main function of a program:
 *  int main( int argc, char** argv)
 */
signature s2_t main( bcore_arr_st_s* args );

/**********************************************************************************************************************/
/// existence, creation

// checks existence of type
func (bl_t exists( tp_t type )) = { return bcore_flect_exists( type ); };

// creates instance from type; error in case type does not exist (check with 'exists')
func (d x_inst* create( tp_t type )) = { return bcore_inst_t_create( type ); };

/**********************************************************************************************************************/
/// copying

func (o copy_typed( m@* o, tp_t type, c@* src )) = { bcore_inst_a_copy_typed( o.cast( m bcore_inst* ), type, src ); return o; };

/**********************************************************************************************************************/
/// ternary branch

func (m (TO) :* ifd( m@* o, bl_t cond, m (TO) :* b )) = { return cond ? o : b; };
func (c (TO) :* ifc( c@* o, bl_t cond, c (TO) :* b )) = { return cond ? o : b; };

/**********************************************************************************************************************/
/// serialization

//----------------------------------------------------------------------------------------------------------------------

func (o to_sink_txt_ml(     c@* o, m bcore_sink* sink )) = { bcore_txt_ml_a_to_sink( o, sink ); return o; };
func (o to_sink_bin_ml(     c@* o, m bcore_sink* sink )) = { bcore_bin_ml_a_to_sink( o, sink ); return o; };
func (o from_source_txt_ml( m@* o, m bcore_source* source )) = { bcore_txt_ml_a_from_source( o, source ); return o; };
func (o from_source_bin_ml( m@* o, m bcore_source* source )) = { bcore_bin_ml_a_from_source( o, source ); return o; };

//----------------------------------------------------------------------------------------------------------------------

func (o t_to_sink_txt_ml(     tp_t t, c@* o, m bcore_sink* sink )) = { bcore_txt_ml_t_to_sink( t, o, sink ); return o; };
func (o t_to_sink_bin_ml(     tp_t t, c@* o, m bcore_sink* sink )) = { bcore_bin_ml_t_to_sink( t, o, sink ); return o; };
func (o t_from_source_txt_ml( tp_t t, m@* o, m bcore_source* source )) = { bcore_txt_ml_t_from_source( t, o, source ); return o; };
func (o t_from_source_bin_ml( tp_t t, m@* o, m bcore_source* source )) = { bcore_bin_ml_t_from_source( t, o, source ); return o; };

//----------------------------------------------------------------------------------------------------------------------

func (o to_file_txt_ml(   c@* o, sc_t path )) =
{
    return o.to_sink_txt_ml( bcore_file_open_sink( path )^ );
};

func (o to_file_bin_ml(   c@* o, sc_t path )) =
{
    return o.to_sink_bin_ml( bcore_file_open_sink( path )^ );
};

func (o from_file_txt_ml( m@* o, sc_t path )) =
{
    return o.from_source_txt_ml( bcore_file_open_source( path )^ );
};

func (o from_file_bin_ml( m@* o, sc_t path )) =
{
    return o.from_source_bin_ml( bcore_file_open_source( path )^ );
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// standard streams

//----------------------------------------------------------------------------------------------------------------------

func (m bcore_source* stdin()) = { return BCORE_STDIN; };
func (m bcore_sink* stdout()) = { return BCORE_STDOUT; };
func (m bcore_sink* stderr()) = { return BCORE_STDERR; };

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#endif // XOILA_SECTION

#endif  // BCORE_X_INST_H
