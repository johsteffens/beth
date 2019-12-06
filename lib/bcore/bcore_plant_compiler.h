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

/** Plant Framework (Automatic code generation)
 *  Beth-Plant is a widely declarative language that can be immersed with
 *  c-code. bcore_plant_compiler translates beth-plant code into c-source code.
 *  Beth-plant code is located in header files.
 *  Translated c-source code is written into dedicated files, which can be
 *  included and compiled with other c-source code.
 *
 *  The term plant reflects the notion of 'planting' (embedding) dedicated code with regular
 *  source code.
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
 *  Copyright and License of generated code:
 *    Code generated by the beth-plant compiler inherits the copyright and license of the beth-plant
 *    source code. This information should have been placed inside the header file containing beth-plant
 *    source code. Generated files contain a list of source files used.
 *
 *  Usage:
 *  PLANT_GROUP( <group_name>, <trait_name> )
 *  <plant-opener>
 *      signature <ret> <func_name>( {plain | const | mutable}, <args> );
 *             plain: plain function (no object pointer as first argument)
 *             const:   const member function;   first argument: const @* o
 *             mutable: mutable member function; first argument:       @* o
 *
 *      signature <signature> <func_name>[( <additional-args> )];
 *      signature [ [<signature-group>] : ] <signature-name> [<func_name>] [( <additional-args> )];
 *
 *      feature [strict] '<flags>' <syntax-of-signature> [ = <body> ];
 *
 *      func feature : name [= { <body> }];
 *
 *      stamp <name> = [aware] <trait> { reflection-body };
 *      stamp <name> = extending <tmpl> { reflection-body-extension }; // uses stamp 'tmpl' as template to be extended
 *      stump ... // same as stamp definition. A 'stump' is not expanded into code. But it can be used as template. Stump-functions are overloadable.
 *      {stamp|stump} verbatim ....  // keyword 'verbatim' disables all automatic extensions for this definition
 *
 *      // automatic extending
 *      extending <name>;    // stamp 'name' will be extended in all subsequent stamps that do not explicitly use 'extending'
 *      extending stump ...; // extends this stump definition
 *      extending;           // turns automatic extending off
 *
 *      name <name>;
 *
 *      group <name> = [<trait>] { ... }; // sub-group; inheriting from parent: 'extending', 'func'
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
 *  func:
 *    Generic default function definition that is automatically applied to all stamps in this group.
 *    A stamp can overload this definition by defining func of the same name.
 *    In a group a function can be redefined under the same name.
 *
 *  ----------------------------------------------------------------------------------------------
 *  name:
 *    Identifier definition. Generates tp_t TYPEOF_<name>. Registers <name> in name manager.
 *    <name> has global scope. Multiple definitions of the same name at different places are allowed.
 *
 *  ----------------------------------------------------------------------------------------------
 *  forward:
 *    Forward declaration. Generates tp_t TYPEOF_<name> and BCORE_FORWARD_OBJECT( <name> );
 *    <name> has global scope. Multiple definitions of the same forwarding at different places are allowed.
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
 *    Special characters:
 *      '@' : inserts the stamps full type name.
 *      '$R': inserts the stamps relative type name without '_s'.
 *            Example: In group 'bcore_fog'
 *               stamp :blue       -> $R: "blue"
 *               stamp bcore_blue' -> $R: "blue"
 *               stamp bmath_blue' -> $R: "bmath_blue"
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

/** Plant Compiler
 *  The plant framework is invoked by function 'bcore_plant_run_globally'. It would typically be executed
 *  at the beginning of the program. If it returns 'true', the program should be terminated and rebuilt.
 *
 *  The plant compiler is instantiated and configured with the first call of function 'bcore_plant_compile'.
 *  If a configuration file of name ".bcore_plant_compiler.cfg" exists in any parent path between the
 *  current folder and the root folder, it is loaded by the compiler.
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
