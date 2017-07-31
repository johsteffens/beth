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
bclos_tree_s* bclos_tree_s_create_d(     vd_t closure );      // attaches closure as internal closure of the new tree
bclos_tree_s* bclos_tree_s_create_typed( tp_t closure_type ); // attaches closure as internal closure of the new tree
void          bclos_tree_s_discard(     bclos_tree_s* o );
bclos_tree_s* bclos_tree_s_clone( const bclos_tree_s* o );

/// bclos_tree_s features
bclos_tree_s*bclos_tree_s_set_closure_d(     bclos_tree_s* o, vd_t closure );             // sets internal closure; returns o
bclos_tree_s*bclos_tree_s_set_closure_typed( bclos_tree_s* o, tp_t type );                // sets internal closure; returns o
bclos_tree_s*bclos_tree_s_set_branch_d(      bclos_tree_s* o, sz_t index, vd_t closure ); // creates a branch at the indexed argument; returns branch
bclos_tree_s*bclos_tree_s_set_branch_typed(  bclos_tree_s* o, sz_t index, tp_t type );    // creates a branch at the indexed argument; returns branch

/// closure features
vd_t bclos_tree_s_func(   const bclos_tree_s* o, vc_t* args, sz_t n_args );
sz_t bclos_tree_s_n_args( const bclos_tree_s* o );
tp_t bclos_tree_s_t_arg(  const bclos_tree_s* o, sz_t index );
tp_t bclos_tree_s_t_ret(  const bclos_tree_s* o );

bcore_flect_self_s* bclos_tree_s_create_self( void );

/**********************************************************************************************************************/
// testing, debugging

bcore_string_s* bclos_tree_selftest( void );

#endif // BCLOS_TREE_H
