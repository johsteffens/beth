/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#ifndef BCORE_SPECT_CLOSURE_H
#define BCORE_SPECT_CLOSURE_H

/** Closure perspective
 */

#include "bcore_flect.h"
#include "bcore_features.h"
#include "bcore_spect_via.h"

// Features:

/// argument descriptor
typedef struct ad_s
{
    tp_t type;
    tp_t flags;
} ad_s;

/// argument descriptor flags
#define AD_F_RD 1 // (read ), state of argument may influence result
#define AD_F_WR 2 // (write), state of argument may be changed by function
#define AD_F_RW 3 // (read-write), argument may be evaluated and changed

// Closure function.
// Arguments are externally created.
// If the function returns an object, it creates it and passes ownership to caller, otherwise it returns NULL.
typedef vd_t (*bcore_closure_fp_func    )( vc_t o, const vd_t* args, sz_t n_args );
typedef sz_t (*bcore_closure_fp_n_args  )( vc_t o ); // number of required arguments
typedef ad_s (*bcore_closure_fp_d_arg   )( vc_t o, sz_t index ); // descriptor of indexed argument
typedef tp_t (*bcore_closure_fp_t_ret   )( vc_t o );             // type of return object

// Sets the (external) environment outside the closure (optional).
// Closure does not assume ownership but has the right to modify it.
typedef void (*bcore_closure_fp_set_env )( vd_t o, vd_t env );

typedef struct bcore_closure_s
{
    aware_t p_type; // type of perspective
    tp_t    o_type; // type of object
    bcore_closure_fp_func    fp_func;     // closure function
    bcore_closure_fp_n_args  fp_n_args;   // number of arguments
    bcore_closure_fp_d_arg   fp_d_arg;    // type of argument
    bcore_closure_fp_t_ret   fp_t_ret;    // type of return
    bcore_closure_fp_set_env fp_set_env;  // sets environment (optional)

} bcore_closure_s;

bcore_flect_self_s*    bcore_closure_s_create_self( void );
const bcore_closure_s* bcore_closure_s_get_typed( tp_t type );
const bcore_closure_s* bcore_closure_s_get_aware( vc_t o );

vd_t bcore_closure_spect_func  (  const bcore_closure_s* p, vd_t o, const vd_t* args, sz_t n_args );
sz_t bcore_closure_spect_n_args(  const bcore_closure_s* p, vc_t o ); // number of expected args
ad_s bcore_closure_spect_d_arg(   const bcore_closure_s* p, vc_t o, sz_t index );
tp_t bcore_closure_spect_t_ret(   const bcore_closure_s* p, vc_t o );
void bcore_closure_spect_set_env( const bcore_closure_s* p, vd_t o, vd_t env );

vd_t bcore_closure_aware_func  (  vd_t o, const vd_t* args, sz_t n_args );
sz_t bcore_closure_aware_n_args(  vc_t o ); // number of expected args
ad_s bcore_closure_aware_d_arg(   vc_t o, sz_t index );
tp_t bcore_closure_aware_t_ret(   vc_t o );
void bcore_closure_aware_set_env( vd_t o, vd_t env );

vd_t bcore_closure_func  (  sr_s o, const vd_t* args, sz_t n_args );
sz_t bcore_closure_n_args(  sr_s o ); // number of expected args
ad_s bcore_closure_d_arg(   sr_s o, sz_t index );
tp_t bcore_closure_t_ret(   sr_s o );
void bcore_closure_set_env( sr_s o, vd_t env );

vd_t bcore_closure_q_func  (  const sr_s* o, const vd_t* args, sz_t n_args );
sz_t bcore_closure_q_n_args(  const sr_s* o ); // number of expected args
ad_s bcore_closure_q_d_arg(   const sr_s* o, sz_t index );
tp_t bcore_closure_q_t_ret(   const sr_s* o );
void bcore_closure_q_set_env( const sr_s* o, vd_t env );

/**********************************************************************************************************************/

/// closure reference; (with reflection)
typedef struct bcore_closure_r
{
    vd_t                   o;
    const bcore_closure_s* p;
} bcore_closure_r;

void bcore_closure_r_init(          bcore_closure_r* o );
void bcore_closure_r_down(          bcore_closure_r* o );
void bcore_closure_r_copy(          bcore_closure_r* o, const bcore_closure_r* src );
void bcore_closure_r_set(           bcore_closure_r* o, sr_s src );
sr_s bcore_closure_r_get(           bcore_closure_r* o );
vd_t bcore_closure_r_func   ( const bcore_closure_r* o, const vd_t* args, sz_t n_args );
sz_t bcore_closure_r_n_args ( const bcore_closure_r* o ); // number of expected args
ad_s bcore_closure_r_d_arg  ( const bcore_closure_r* o, sz_t index );
tp_t bcore_closure_r_t_ret  ( const bcore_closure_r* o );
void bcore_closure_r_set_env( const bcore_closure_r* o, vd_t env );

/**********************************************************************************************************************/

vd_t bcore_spect_closure_signal( tp_t target, tp_t signal, vd_t object );

#endif // BCORE_SPECT_CLOSURE_H
