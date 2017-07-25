/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCLOS_TREE_H
#define BCLOS_TREE_H

/// Closure designed as tree of closures

#include "bcore_spect_closure.h"

typedef struct bclos_tree_s bclos_tree_s;

const bcore_flect_self_s* bclos_tree_s_create_self();

/**********************************************************************************************************************/
// testing, debugging

bcore_string_s* bclos_tree_selftest();

#endif // BCLOS_TREE_H
