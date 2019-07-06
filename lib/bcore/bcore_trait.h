/** Copyright 2017 Johannes Bernhard Steffens
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

#ifndef BCORE_TRAIT_H
#define BCORE_TRAIT_H

#include "bcore_flect.h"

/** Trait management framework
 *  A trait (identifier) is a unique type (tp_t)
 *  A trait can have a parent trait
 *  The collection of traits of an object is given by the inheritance chain.
 *  (Inheritance is governed by a tp->tp hashmap)
 *  A trait can have a descriptive instance of the trait-object
 *  which reflects on trait characteristics and offers functionality
 *  for testing presence of these characteristics in objects.
 *  A trait instance is associated with its identifier via tpto map
 */

/// Support feature: Tests if object supports certain requirements. In case of non-support a log != NULL may be filled with descriptive reasons
typedef bl_t ( *bcore_trait_fp_supports )( const bcore_self_s* self, st_s* log );

/// associates requirements with a trait
/// this must be finished before the trait is defined below
void bcore_trait_require_in_ancestry( tp_t trait ); // trait must be in the ancestry of object's self.trait
void bcore_trait_require_awareness(   tp_t trait );
void bcore_trait_require_function(    tp_t trait, tp_t function, tp_t name );
void bcore_trait_register_fp_support( tp_t trait, bcore_trait_fp_supports f );

/** Defines a trait; parent may be 0; reentrant; thread-safe
 *  This function does not register reflections and can therefore be used
 *  inside the reflection-registry-lock.
 *  Parent can be any value.
 *  Checks for cyclic ancestry.
 */
void bcore_trait_set( tp_t trait, tp_t parent );

/// trait existence; thread-safe
bl_t bcore_trait_exists( tp_t trait );

/// returns direct parent of a trait; 0 if trait does not exist; thread-safe
tp_t bcore_trait_parent( tp_t trait );

/// Checks if ancestor is in the chain of inheritance of trait. (thread-safe)
bl_t bcore_trait_is_of( tp_t trait, tp_t ancestor );

/// fills 'traits' with traits that inherit 'parent' immediately
void bcore_get_traits_of_parent( tp_t parent, bcore_arr_tp_s* traits );

/// fills 'traits' with traits that inherit 'ancestor' (not necessarily immediately)
void bcore_get_traits_of_ancestor( tp_t ancestor, bcore_arr_tp_s* traits );

void bcore_get_traits_of_parent_to_sink(   tp_t parent,   bcore_sink* sink );
void bcore_get_traits_of_ancestor_to_sink( tp_t ancestor, bcore_sink* sink );

/** Checks if a reflection supports a trait including all ancestors
 *  by matching the trait requirements with the reflection.
 */
bl_t bcore_trait_supported( tp_t trait, const bcore_self_s* self, st_s* log );

/** Checks if trait is satisfied by object.
  * This is true when trait is ancestor of self->trait or when bcore_trait_supports returns true.
  */
bl_t bcore_trait_satisfied_self( tp_t trait, const bcore_self_s* self, st_s* log );

/** Checks if trait is satisfied by object
 *  This is true when trait is ancestor of self->trait or when bcore_trait_supports returns true.
 *  This function should be used to determine if a perspective (given by trait) of the object
 *  can be created.
 */
bl_t bcore_trait_satisfied_type( tp_t trait, tp_t object_type, st_s* log );
void bcore_trait_assert_satisfied_type( tp_t trait, tp_t object_type ); // assertion producing error message and aborts

sz_t  bcore_trait_size(); // number of registered names
st_s* bcore_trait_show(); // store all registered traits with ancestry in a string, separated by newline
void  bcore_trait_show_to_sink( bcore_sink* sink );

vd_t bcore_trait_signal_handler( const bcore_signal_s* o );

// Macros
#define BCORE_REGISTER_TRAIT( trait, parent ) \
    bcore_trait_set( entypeof( #trait ), entypeof( #parent ) );

#define BCORE_REGISTER_FFUNC( feature, func ) \
    bcore_trait_set( entypeof( #func ), entypeof( #feature ) ); \
    BCORE_REGISTER_FUNC( func )


#endif // BCORE_TRAIT_H
