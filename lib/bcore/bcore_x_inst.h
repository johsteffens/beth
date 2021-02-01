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

#include "bcore_txt_ml.h"
#include "bcore_bin_ml.h"
#include "bcore_file.h"
#include "bcore.xo.h"

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( x_inst, bcore_inst )

#ifdef XOILA_SECTION

//----------------------------------------------------------------------------------------------------------------------

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

func (o to_file_txt_ml(   c@* o, sc_t path )) = { return o.to_sink_txt_ml( bcore_file_open_sink( path )^ ); };
func (o to_file_bin_ml(   c@* o, sc_t path )) = { return o.to_sink_bin_ml( bcore_file_open_sink( path )^ ); };
func (o from_file_txt_ml( m@* o, sc_t path )) = { return o.from_source_txt_ml( bcore_file_open_source( path )^ ); };
func (o from_file_bin_ml( m@* o, sc_t path )) = { return o.from_source_bin_ml( bcore_file_open_source( path )^ ); };

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// global sinks

//----------------------------------------------------------------------------------------------------------------------

func (m bcore_sink* stdout()) = { return BCORE_STDOUT; };
func (m bcore_sink* stderr()) = { return BCORE_STDERR; };

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#endif // XOILA_SECTION

#endif  // BCORE_X_INST_H
