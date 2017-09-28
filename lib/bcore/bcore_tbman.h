/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

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

/// returns currently granted space (thread-safe) (use this function to test for memory leaks)
sz_t bcore_tbman_granted_space( void );
sz_t bcore_tbman_s_granted_space( bcore_tbman_s* o );

/// reference management

vd_t bcore_tbman_s_rc_create(  bcore_tbman_s* o, tp_t type ); // creates an object setting up up reference control
vd_t bcore_tbman_s_rc_fork(    bcore_tbman_s* o, vd_t ptr ); // forks a reference
vd_t bcore_tbman_s_rc_discard( bcore_tbman_s* o, vd_t ptr ); // discards reference to object or array; returns NULL

vd_t bcore_tbman_s_rc_alloc
(
    bcore_tbman_s* o,
    tp_t type,
    vd_t current_ptr,
    sz_t current_size,
    sz_t requested_space,
    sz_t* granted_space
);


vd_t bcore_tbman_signal( tp_t target, tp_t signal, vd_t object );

#endif // BCORE_TBMAN_H
