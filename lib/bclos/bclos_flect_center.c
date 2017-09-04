/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_flect.h"

#include "bclos_flect_center.h"
#include "bclos_tree.h"
#include "bclos_leaf.h"
#include "bclos_env.h"
#include "bclos_args.h"
#include "bclos_sig.h"
#include "bclos_spect_closure.h"
#include "bclos_statement.h"
#include "bclos_procedure.h"

void bclos_flect_define_self_creators( void )
{
    bcore_flect_define_creator( typeof( "bclos_tree_s" ), bclos_tree_s_create_self );
    bclos_leaf_define_self_creators();
    bclos_env_define_self_creators();
    bclos_args_define_self_creators();
    bclos_sig_define_self_creators();
    bclos_spect_closure_define_self_creators();
    bclos_statement_define_self_creators();
    bclos_procedure_define_self_creators();
}

