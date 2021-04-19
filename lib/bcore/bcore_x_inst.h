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
/// existence

// checks existence of type
func (bl_t exists( tp_t type )) = { return bcore_flect_exists( type ); };

/**********************************************************************************************************************/
/// create/copy/clone/discard

/// func (d obliv x_inst* t_create( tp_t type )) = { return bcore_inst_t_create( type ); }; // this is a default function for groups
    func (d aware x_inst*   create( tp_t type )); // only for aware types (checked at runtime)

func (o t_copy( m obliv @* o, tp_t t, c@* src )) = { bcore_inst_t_copy( t, o.cast( m bcore_inst* ), src ); return o; };
func (o   copy( m aware @* o,         c@* src )) = { bcore_inst_a_copy(    o.cast( m bcore_inst* ), src ); return o; };

func (o t_copy_typed( m obliv @* o, tp_t t, tp_t type, c obliv @* src )) = { bcore_inst_t_copy_typed( t, o.cast( m bcore_inst* ), type, src ); return o; };
func (o   copy_typed( m aware @* o,         tp_t type, c obliv @* src )) = { bcore_inst_a_copy_typed(    o.cast( m bcore_inst* ), type, src ); return o; };

func (d (TO) @* t_clone( c obliv (TO) @* o, tp_t t )) = { return bcore_inst_t_clone( t, o.cast( bcore_inst* ) ); };
func (d (TO) @*   clone( c aware (TO) @* o         )) = { return bcore_inst_a_clone(    o.cast( bcore_inst* ) ); };

func (void t_discard( d obliv @* o, tp_t t )) = { bcore_inst_t_discard( t, o.cast( m bcore_inst* ) ); };
func (void   discard( d aware @* o         )) = { bcore_inst_a_discard(    o.cast( m bcore_inst* ) ); };

/**********************************************************************************************************************/
/// ternary branch

func (m (TO) :* ifd( m@* o, bl_t cond, m (TO) :* b )) = { return cond ? o : b; };
func (c (TO) :* ifc( c@* o, bl_t cond, c (TO) :* b )) = { return cond ? o : b; };

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// standard streams

//----------------------------------------------------------------------------------------------------------------------

func (m bcore_source* stdin()) = { return BCORE_STDIN; };
func (m bcore_sink* stdout()) = { return BCORE_STDOUT; };
func (m bcore_sink* stderr()) = { return BCORE_STDERR; };

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// implementations

//----------------------------------------------------------------------------------------------------------------------

func create =
{
    if( type != 0 && !x_stamp_t_is_aware( type ) )
    {
        ERR_fa( "Function 'x_inst:create' can only be used for aware types. Use 'x_inst:t_create' instead." );
    }
    return bcore_inst_t_create( type );
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#endif // XOILA_SECTION

#endif  // BCORE_X_INST_H
