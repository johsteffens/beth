/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#ifndef BCORE_TRAIT_H
#define BCORE_TRAIT_H

#include "bcore_flect.h"

/** Trait management framework
 *  A trait (identifier) is a unique type (tp_t)
 *  A trait can have a parent trait
 *  The collection of traits of an object is given by the inheritance chain.
 *  (Inheritance is governed by a tp->tp map)
 *  A trait can have a descriptive instance of the trait-object
 *  which reflects on trait characteristics and offers functionality
 *  for testing presence of these characteristics in objects.
 *  A trait instance is associated with its identifier via tpto map
 */

/// Support feature: Tests if object supports certain requirements. In case of non-support a log != NULL may be filled with descriptive reasons
typedef bl_t ( *bcore_trait_fp_supports )( const bcore_flect_self_s* self, st_s* log );

/// associates requirements with a trait
/// this must be finished before the trait is defined below
void bcore_trait_require_in_ancestry( tp_t trait ); // trait must be in the ancestry of object's self.trait
void bcore_trait_require_awareness(   tp_t trait );
void bcore_trait_require_function(    tp_t trait, tp_t function, tp_t name );
void bcore_trait_register_fp_support( tp_t trait, bcore_trait_fp_supports f );

/// defines a trait; parent may be 0; reentrant; thread-safe
void bcore_trait_set( tp_t trait, tp_t parent );

/// trait existence; thread-safe
bl_t bcore_trait_exists( tp_t trait );

/// returns direct parent of a trait; 0 if trait does not exist; thread-safe
tp_t bcore_trait_parent( tp_t trait );

/// checks if ancestor is in the chain of inheritance of trait; thread-safe
bl_t bcore_trait_is( tp_t trait, tp_t ancestor );

/// checks if a reflection supports a trait including all ancestors (ignores trait specification in reflection)
bl_t bcore_trait_supported( tp_t trait, const bcore_flect_self_s* self, st_s* log );

/// checks if trait is satisfied by object (this is true when trait is ancestor of self->trait or when bcore_trait_supports returns true)
bl_t bcore_trait_satisfied_self( tp_t trait, const bcore_flect_self_s* self, st_s* log );

/// checks if trait is satisfied by object (this is true when trait is ancestor of self->trait or when bcore_trait_supports returns true)
bl_t bcore_trait_satisfied_type( tp_t trait, tp_t object_type, st_s* log );

vd_t bcore_trait_signal( tp_t target, tp_t signal, vd_t object );

#endif // BCORE_TRAIT_H
