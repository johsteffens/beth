/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCORE_TRAIT_H
#define BCORE_TRAIT_H

#include "bcore_types.h"

/** Trait management framework
 *  A trait (identifier) is a unique type (tp_t)
 *  A trait can have a parent trait
 *  The collection of traits of an object is given by the inheritance chain.
 *  (Inheritance is governed by a tp->tp map)
 *  A trait can have a descriptive instance of the trait-object
 *  which reflects on trait characteristics and offers functionality
 *  for testing presence of these characteristics in objects.
 *  A trait instance is associated with its identifier via tp->vd map
 */

void bcore_trait_manager_open();
void bcore_trait_manager_close();

/// associates requirements with a trait
/// this can only be done on new (not yet defined) traits
bl_t bcore_trait_require_function(  tp_t trait, tp_t function, tp_t name );
bl_t bcore_trait_require_awareness( tp_t trait );

/// defines a trait; parent may be 0; reentrant; thread-safe
void bcore_trait_define( tp_t trait, tp_t parent );

/// trait existence; thread-safe
bl_t bcore_trait_exists( tp_t trait );

/// returns immediate parent of a trait; 0 if trait does not exist; thread-safe
tp_t bcore_trait_parent( tp_t trait );

/// checks if ancestor is in the chain of inheritance of trait; thread-safe
bl_t bcore_trait_is( tp_t trait, tp_t ancestor );

vd_t bcore_trait_signal( tp_t target, tp_t signal, vd_t object );

#endif // BCORE_TRAIT_H
