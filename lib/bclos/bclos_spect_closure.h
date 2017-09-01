/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCLOS_SPECT_CLOSURE_H
#define BCLOS_SPECT_CLOSURE_H

// Features
typedef void (*bclos_closure_fp_def  )( vd_t o, bclos_env_s* env );                    // defines a closure: turns (re)binds instance; env is deemed lexical environment (optional)
typedef sr_s (*bclos_closure_fp_call )( vc_t o, bclos_env_s* env, bclos_args_s args ); // calls closure function; env is deemed dynamic environment
typedef sr_s (*bclos_closure_fp_sig  )( vc_t o );                                      // returns signature of closure: number of arguments, type of arguments (types could be perspectives)

#endif // BCLOS_SPECT_CLOSURE_H
