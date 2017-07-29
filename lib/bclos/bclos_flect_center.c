/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_flect.h"

#include "bclos_tree.h"
#include "bclos_algebra.h"
#include "bclos_flect_center.h"

void bclos_flect_define_self_creators( void )
{
    bcore_flect_define_creator( typeof( "bclos_tree_s" ), bclos_tree_s_create_self );
    bclos_algebra_define_self_creators();
}

