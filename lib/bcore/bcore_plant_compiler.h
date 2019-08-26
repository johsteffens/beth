/** Copyright 2019 Johannes Bernhard Steffens
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

/** Plant Framework (Automatic code generation)
 *  Beth-Plant is a widely declarative language that can be immersed with
 *  c-code. bcore_plant_compiler translates beth-plant code into c-source code,
 *  which is written to dedicated files, which can be included and compiled with
 *  other c-source code.
 *
 *  The term plant reflects the notion of 'planting' (embedding) dedicated code with regular
 *  source code. Copyright applies to beth-plant source code as well regular C source code.
 *
 *  A beth-source participates in 'planting' when its signal-handler executes upon
 *  TYPEOF_plant the function bcore_plant_compile( <planted-file>, __FILE__ );
 *
 *  <planted-file> is the name of the output file containing generated code.
 *  <planted-file>.c and <planted-file>.h are generated in the folder of the beth-source.
 *  Before <planted-file>.? is being modified it is renamed to <planted-file>.?.backup
 *
 *  During development, bl_t bcore_plant_run_globally() should be executed at the beginning
 *  of the program. If it returns 'true', planted sources were modified and a rebuild is required.
 *
 *  Usage:
 *  PLANT_GROUP( <group_name>, <trait_name> )
 *  <plant-opener>
 *      [set enroll;]
 *
 *      signature <ret> <func_name>( const | mutable, <args> );
 *      signature <signature> <func_name>[( <additional-args> )];
 *      signature [ [<signature-group>] : ] <signature-name> [<func_name>] [( <additional-args> )];
 *
 *      feature [strict] '<flags>' <syntax-of-signature> [ = <body> ];
 *
 *      stamp <name> = [aware] <trait> { reflection-body };
 *
 *      name <name>;
 *
 *      group <name> = [<trait>] { ... };
 *
 *      ...
 *  <plant-closer>
 *
 *  plant-opener:
 *    c-style comment-opener or '#ifdef PLANT_SECTION'
 *    Macro PLANT_SECTION must never be defined.
 *
 *  plant-closer:
 *    c-style comment-closer or '#endif' (depending on what was used for plant-opener)
 *
 *  ----------------------------------------------------------------------------------------------
 *  feature:
 *    The feature definition triggers implicit definition of a perspective <group_name>
 *    and corresponding inline implementations for each (virtual) function 'func_name'.
 *    <flags>-setting (s.below) determines which referencing method is offered for the feature.
 *
 *       Calling a feature depending on referencing method.
 *       Note:
 *         Calling an unbound function can result in a segmentation fault.
 *         Use check below to probe for a valid binding.
 *       <ret> <group_name>_p_<func_name>( const <group_name>_s* p, <args> );
 *       <ret> <group_name>_t_<func_name>( tp_t __t, [const] <group_name>* o, <args> );
 *       <ret> <group_name>_a_<func_name>( [const] <group_name>* o, <args> );
 *       <ret> <group_name>_r_<func_name>( const sr_s* o, <args> );
 *
 *       Checks whether binding exists for given object.
 *       Existing <default_func> qualifies as valid binding for all objects.
 *       bl_t <group_name>_p_defines_<func_name>( const <group_name>_s* p );
 *       bl_t <group_name>_t_defines_<func_name>( tp_t t );
 *       bl_t <group_name>_a_defines_<func_name>( const <group_name>* o );
 *       bl_t <group_name>_r_defines_<func_name>( const sr_s* o );
 *
 *    flags:
 *        Any character sub-sequence of 'ptar' specifying which type of virtual function shall
 *        be inline implemented.
 *
 *    strict:
 *        Any object referenced by the perspective must define the function.
 *
 *    default_name:
 *        Optional default function used when the object does not define the feature.
 *        Requires implementing <ret> <group_name>_<default_name>( [const] <group_name>* o, <args> );
 *
 *  ----------------------------------------------------------------------------------------------
 *  name:
 *    Identifier definition. Generates tp_t TYPEOF_<name>. Registers <name> in name manager.
 *    <name> has global scope. Multiple definitions of the same name at different places are allowed.
 *
 *  ----------------------------------------------------------------------------------------------
 *  stamp:
 *
 *    Reflection of object definition for objects (see flect.h for general syntax)
 *    Function definitions in reflections are matched and bound to features or signatures.
 *    C-Prototypes are generated and registered as featured function.
 *
 *    Function Syntax:
 *      *  func <group_name> : <signature_name> [ = { <body> }];
 *           <signature_name> is a signature defined in group <group_name>
 *           Creates a function <stamp_name>_s_<signature_name>.
 *
 *      *  func <group_name> : <feature_name> [ = { <body> }];
 *           <feature_name> is a feature defined in group <group_name>
 *           Creates a function <stamp_name>_s_<feature_name> and binds it to <group_name>_<feature_name>.
 *
 *    Special conditions:
 *      ':' following stamp means that '<group_name>_' is prefixed to the following name
 *      ':' instead of trait means that '<group_name>' is declared as trait
 *      ':' (no func) preceding an identifier name means that '<group_name>_' is prefixed to the following name
 *      ':' (in func) if no perspective name precedes ':' then <group_name> is used as perspective
 *     'func ^ : <func_name>'  <trait_name> is used as perspective
 *
 *    Special traits:
 *      bcore_array: expands additional array-functions
 *        void set_space( o, size ) // preallocation
 *        void set_size(  o, size ) // preallocation + size specification
 *        void clear(     o,      ) // clears array (sets space to 0)
 *        void push_c(    o, v    ) // pushes element by copying it
 *        void push_d(    o, v    ) // pushes element by passing ownership  (whether element is copied depends on array-type)
 *
 *  Functions:
 *    Features and functions inside a stamp can have a body.
 *    The body is enclosed in '{ ... }' and contains ordinary c-code.
 *    Exceptions:
 *      Symbol ':' immediately followed by a name serves as namespace expander
 *
 *  ----------------------------------------------------------------------------------------------
 *  set:
 *    Keyword 'set' defines certain group-specific flags:
 *      enroll: This flag forces all stamps in the group to be fully enrolled.
 *              This requires extra memory and is normally not recommended.
 *              It is useful when objects are intended to be always reverse-traceable by trait.
 *
 */

#ifndef BCORE_PLANT_COMPILER_H
#define BCORE_PLANT_COMPILER_H

#include "bcore_control.h"
#include "bcore_signal.h"
#include "bcore_flect.h"
#include "bcore_spect_array.h"
#include "bcore_hmap.h"

/**********************************************************************************************************************/
/// Plant interface

/// Compiles plants into target files: directory( file )/target{ .c | .h }
void bcore_plant_compile( sc_t target_name, sc_t source_path );

/// Updates planted files if necessary; returns true in case a file was modified
bl_t bcore_plant_expand();

/** Runs plant compiler globally (compiling, expansion);
 *  Returns true in case a planted-file was generated or modified.
 */
bl_t bcore_plant_run_globally();

/**********************************************************************************************************************/

vd_t bcore_plant_compiler_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

/// Macros

#define BETH_PRECODE( name ) \
    BETH_EXPAND_GROUP_##name

#define PLANT_GROUP( name, trait ) \
    BETH_EXPAND_GROUP_##name

#endif // BCORE_PLANT_COMPILER_H