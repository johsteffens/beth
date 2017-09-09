/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCLOS_LEAF_H
#define BCLOS_LEAF_H

#include "bcore_types.h"

/// Collection of elementary closures (leaf-closures)

void bclos_leaf_define_self_creators( void );
vd_t bclos_leaf_signal( tp_t target, tp_t signal, vd_t object );

/**********************************************************************************************************************/

#endif // BCLOS_LEAF_H
