/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

/** An adapted, simplified and stand-alone version of this memory manager has
 *  been released on https://github.com/johsteffens/tbman under a public license.
 */

#ifndef BCORE_TBMAN_H
#define BCORE_TBMAN_H

#include "bcore_types.h"
#include "bcore_string.h"

typedef struct bcore_tbman_s bcore_tbman_s;

bcore_tbman_s* bcore_tbman_s_create
(
    sz_t pool_size,
    sz_t min_block_size,
    sz_t max_block_size,
    sz_t stepping_method,
    bl_t full_align
);

void bcore_tbman_s_discard( bcore_tbman_s* o );

/**********************************************************************************************************************/
/** Advanced memory management using the internal manager (thread-safe).
 *  This function provides allocation, re-allocation and freeing of memory
 *  with advanced controls to improve memory efficiency.
 *  In this context, a free request is represented as re-allocation with requested_bytes == 0
 *
 *  Arguments
 *    current_ptr:
 *      Pointer to current memory location:
 *        ==NULL for pure-allocation
 *        !=NULL for re-allocation or freeing
 *
 *    current_bytes:
 *      Optional information to memory manager about last reserved or requested amount.
 *      Allowed values: 0 or previously requested or reserved amount.
 *                      0 makes the function ignore current_ptr (assumes it is NULL)
 *
 *    requested_bytes:
 *       > 0 for pure-allocation or re-allocation
 *      == 0 for freeing
 *
 *    granted_bytes
 *      The memory manager grants at least the requested amount of bytes. But it may grant more memory.
 *      The requester may use granted memory without re-allocation. (E.g. for dynamic arrays or variable size.)
 *      The granted amount is provided as feedback in case pointer granted_bytes is not NULL.
 *
 *  Return
 *    Allocated memory address or NULL after freeing.
 *
 *  Alignment:
 *    When requesting memory of size n*m when n is a positive integer and m is a power of 2, then
 *    the returned memory is aligned to the lesser of m and BCORE_TBMAN_ALIGN.
 */
vd_t bcore_tbman_b_alloc(                      vd_t current_ptr,                     sz_t requested_bytes, sz_t* granted_bytes );
vd_t bcore_tbman_bn_alloc(                     vd_t current_ptr, sz_t current_bytes, sz_t requested_bytes, sz_t* granted_bytes );
vd_t bcore_tbman_s_b_alloc(  bcore_tbman_s* o, vd_t current_ptr,                     sz_t requested_bytes, sz_t* granted_bytes );
vd_t bcore_tbman_s_bn_alloc( bcore_tbman_s* o, vd_t current_ptr, sz_t current_bytes, sz_t requested_bytes, sz_t* granted_bytes );

/// Like b(n)_alloc except that sizes are given as multiple of the unit-size. (thread-safe)
vd_t bcore_tbman_u_alloc(                      sz_t unit_bytes, vd_t current_ptr,                     sz_t requested_units, sz_t* granted_units );
vd_t bcore_tbman_un_alloc(                     sz_t unit_bytes, vd_t current_ptr, sz_t current_units, sz_t requested_units, sz_t* granted_units );
vd_t bcore_tbman_s_u_alloc(  bcore_tbman_s* o, sz_t unit_bytes, vd_t current_ptr,                     sz_t requested_units, sz_t* granted_units );
vd_t bcore_tbman_s_un_alloc( bcore_tbman_s* o, sz_t unit_bytes, vd_t current_ptr, sz_t current_units, sz_t requested_units, sz_t* granted_units );

/// Simplified alloc, traditional malloc, free and realloc (thread-safe).
static inline vd_t bcore_tbman_alloc(   vd_t ptr, sz_t size ) { return bcore_tbman_b_alloc( ptr,  size, NULL ); }
static inline vd_t bcore_tbman_malloc(            sz_t size ) { return bcore_tbman_b_alloc( NULL, size, NULL ); }
static inline vd_t bcore_tbman_realloc( vd_t ptr, sz_t size ) { return bcore_tbman_b_alloc( ptr,  size, NULL ); }
static inline void bcore_tbman_free(    vd_t ptr            ) {        bcore_tbman_b_alloc( ptr,  0,    NULL ); }

/// Current allocations (thread-safe)
sz_t bcore_tbman_s_granted_space( bcore_tbman_s* o ); // total granted space
sz_t bcore_tbman_s_instances( bcore_tbman_s* o );     // total instances
sz_t bcore_tbman_s_rc_instances( bcore_tbman_s* o );  // total instances under reference control
sz_t bcore_tbman_s_rc_references( bcore_tbman_s* o ); // total references

sz_t bcore_tbman_granted_space( void );
sz_t bcore_tbman_instances( void );
sz_t bcore_tbman_rc_instances( void );
sz_t bcore_tbman_rc_references( void );

/**********************************************************************************************************************/
/// Reference Control

/// Forks a reference.
vd_t bcore_tbman_s_rc_fork( bcore_tbman_s* o, vd_t ptr );

/// Creates a root object with reference control.
vd_t bcore_tbman_s_rc_create( bcore_tbman_s* o, tp_t type );

/** Updates type of root object in rcp.
 *  Sets up reference control if not existing.
 *  Does not modify the object itself.
 */
void bcore_tbman_s_rc_update( bcore_tbman_s* o, vc_t ptr, tp_t type );

/// Releases reference to object (ptr can also be an array); returns NULL
vd_t bcore_tbman_s_rc_release( bcore_tbman_s* o, vd_t ptr );

/// Creates an array of given space and size 0 with reference control.
vd_t bcore_tbman_s_rc_create_arr(  bcore_tbman_s* o, tp_t type, sz_t space );

/** Updates array info of root object in rcp.
 *  Sets up reference control if not existing.
 *  Does not modify the array itself.
 */
void bcore_tbman_s_rc_update_arr(  bcore_tbman_s* o, vc_t arr_ptr, tp_t type, sz_t size );

/// Releases array reference to object; Updated array info if necessary; returns NULL
vd_t bcore_tbman_s_rc_release_arr( bcore_tbman_s* o, vd_t arr_ptr, tp_t type, sz_t size );

/// Above functions with internal memory manager
vd_t bcore_tbman_rc_fork       ( vd_t ptr            );
vd_t bcore_tbman_rc_create     (           tp_t type );
void bcore_tbman_rc_update     ( vc_t ptr, tp_t type );
vd_t bcore_tbman_rc_release    ( vd_t ptr            );
vd_t bcore_tbman_rc_create_arr (               tp_t type, sz_t space );
void bcore_tbman_rc_update_arr ( vc_t arr_ptr, tp_t type, sz_t size  );
vd_t bcore_tbman_rc_release_arr( vd_t arr_ptr, tp_t type, sz_t size  );

vd_t bcore_tbman_signal( tp_t target, tp_t signal, vd_t object );

#endif // BCORE_TBMAN_H
