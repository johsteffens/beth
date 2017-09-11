/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCLOS_TREE_H
#define BCLOS_TREE_H

/** bclos_tree_s is a closure representing a tree of closures.
 *  It wraps one closure (--> internal closure).
 *  Its return is the return of its internal closure.
 *  It has as many branches as the internal closure has arguments.
 *  A branch is either NULL (leaf) or an instance of bclos_tree_s.
 *  All leafs are uniquely bound to an argument of bclos_tree_s
 *  in the order induced by the tree.
 */
#include "bcore_spect_closure.h"

typedef struct bclos_tree_s bclos_tree_s;

void bclos_tree_s_init( bclos_tree_s* o );
void bclos_tree_s_down( bclos_tree_s* o );
void bclos_tree_s_copy( bclos_tree_s* o, const bclos_tree_s* src );
bclos_tree_s* bclos_tree_s_create();
void          bclos_tree_s_discard(     bclos_tree_s* o );
bclos_tree_s* bclos_tree_s_clone( const bclos_tree_s* o );

/// bclos_tree_s features
void bclos_tree_s_set_closure( bclos_tree_s* o, sr_s closure );             // sets internal closure; returns o
void bclos_tree_s_set_branch(  bclos_tree_s* o, sz_t index, sr_s closure ); // creates a branch at the indexed argument; returns branch
bcore_closure_r bclos_tree_s_get_closure( bclos_tree_s* o );                // returns internal closure
bcore_closure_r bclos_tree_s_get_branch(  bclos_tree_s* o, sz_t index );    // returns internal branch

/// closure features
vd_t bclos_tree_s_func(   const bclos_tree_s* o, vd_t* args, sz_t n_args );
sz_t bclos_tree_s_n_args( const bclos_tree_s* o );
tp_t bclos_tree_s_t_arg(  const bclos_tree_s* o, sz_t index );
tp_t bclos_tree_s_t_ret(  const bclos_tree_s* o );

vd_t bclos_tree_signal( tp_t target, tp_t signal, vd_t object );

#endif // BCLOS_TREE_H
