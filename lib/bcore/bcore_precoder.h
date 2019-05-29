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

/** Precoder Framework (Automatic code generation)
 *
 *  Precoder translates beth-precode source code into c-source code, which is
 *  written to dedicated precoded-files, which can be included and compiled with
 *  other c-source code.
 *
 *  A beth-source participates in precoding when its signal-handler executes upon
 *  TYPEOF_precoder the function bcore_precoder_compile( <precoded-file>, __FILE__ );
 *
 *  <precoded-file> is the name of the output (precoded) file given without path or extension.
 *  <precoded-file>.c and <precoded-file>.h are generated in the folder of the beth-source.
 *  Before <precoded-file>.? is being modified it is renamed to <precoded-file>.?.backup
 *
 *  During development, bl_t bcore_precoder_run_globally() should be executed at the beginning
 *  of the program. If it returns 'true' precoded sources were modified and a rebuild is required.
 *
 *  Usage:
 *  BCORE_PRECODE( <group_name> )
 *  <precode-opener>
 *      signature <ret> <func_name>( const | mutable, <args> );
 *      signature <signature> <func_name>[( <additional-args> )];
 *      signature [ [<signature-group>] : ] <signature-name> [<func_name>] [( <additional-args> )];
 *
 *      feature [strict] '<flags>' <syntax-of-signature> [ = <default_func> ];
 *
 *      self <name> = <trait> { reflection-definition };
 *
 *      name <name>;
 *
 *      ...
 *  <precode-closer>
 *
 *  precode-opener:
 *    c-style comment-opener or '#ifdef BETH_PRECODE_SECTION'
 *    Macro BETH_PRECODE_SECTION must never be defined.
 *
 *  precode-closer:
 *    c-style comment-closer or '#endif' (depending on what was used for precode-opener)
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
 *  self:
 *    Reflection definition for objects (see flect.h for details)
 *    (Perspectives should not be explicitly defined here.)
 *    Function definitions in reflections are matched and bound to features. C-Prototypes are generated
 *    and registered as featured function.
 *
 *    Special traits:
 *      bcore_array: expands additional array-functions
 *        void set_space( o, size ) // preallocation
 *        void set_size(  o, size ) // preallocation + size specification
 *        void clear(     o,      ) // clears array (sets space to 0)
 *        void push_c(    o, v    ) // pushes element by copying it
 *        void push_d(    o, v    ) // pushes element by passing ownership  (whether element is copied depends on array-type)
 */

#ifndef BCORE_PRECODER_H
#define BCORE_PRECODER_H

#include "bcore_control.h"
#include "bcore_signal.h"
#include "bcore_flect.h"
#include "bcore_spect_array.h"
#include "bcore_hmap.h"

/**********************************************************************************************************************/

/// compiles precode into target files: directory( file )/target{ .c | .h }
void bcore_precoder_compile( sc_t target_name, sc_t source_path );

/// updates precoded files if necessary; returns true in case a file was modified
bl_t bcore_precoder_expand();

/** Runs precoder globally (compiling, expansion);
 *  Returns true in case a precoded-file was generated or modified.
 */
bl_t bcore_precoder_run_globally();

/**********************************************************************************************************************/

vd_t bcore_precoder_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/
/// Macros
#define BETH_PRECODE( name ) \
    BETH_EXPAND_GROUP_##name

#endif  // BCORE_PRECODER_H
