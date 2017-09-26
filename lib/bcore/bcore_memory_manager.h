/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#ifndef BCORE_MEMORY_MANAGER_H
#define BCORE_MEMORY_MANAGER_H

#include "bcore_types.h"
#include "bcore_string.h"

typedef struct bcore_memory_manager_s bcore_memory_manager_s;

bcore_memory_manager_s* bcore_memory_manager_s_create(  sz_t pool_size, sz_t min_block_size,
                                                        sz_t max_block_size, sz_t stepping_method,
                                                        bl_t full_align );

void bcore_memory_manager_s_discard( bcore_memory_manager_s* o );

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
 *    the returned memory is aligned to the lesser of m and BCORE_MEMORY_MANAGER_ALIGN.
 */
vd_t bcore_memory_manager_b_alloc(                               vd_t current_ptr,                     sz_t requested_bytes, sz_t* granted_bytes );
vd_t bcore_memory_manager_bn_alloc(                              vd_t current_ptr, sz_t current_bytes, sz_t requested_bytes, sz_t* granted_bytes );
vd_t bcore_memory_manager_s_b_alloc(  bcore_memory_manager_s* o, vd_t current_ptr,                     sz_t requested_bytes, sz_t* granted_bytes );
vd_t bcore_memory_manager_s_bn_alloc( bcore_memory_manager_s* o, vd_t current_ptr, sz_t current_bytes, sz_t requested_bytes, sz_t* granted_bytes );

/// Like b(n)_alloc except that sizes are given as multiple of the unit-size. (thread-safe)
vd_t bcore_memory_manager_u_alloc(                               sz_t unit_bytes, vd_t current_ptr,                     sz_t requested_units, sz_t* granted_units );
vd_t bcore_memory_manager_un_alloc(                              sz_t unit_bytes, vd_t current_ptr, sz_t current_units, sz_t requested_units, sz_t* granted_units );
vd_t bcore_memory_manager_s_u_alloc(  bcore_memory_manager_s* o, sz_t unit_bytes, vd_t current_ptr,                     sz_t requested_units, sz_t* granted_units );
vd_t bcore_memory_manager_s_un_alloc( bcore_memory_manager_s* o, sz_t unit_bytes, vd_t current_ptr, sz_t current_units, sz_t requested_units, sz_t* granted_units );

/// Simplified alloc, traditional malloc, free and realloc (thread-safe).
static inline vd_t bcore_memory_manager_alloc(   vd_t ptr, sz_t size ) { return bcore_memory_manager_b_alloc( ptr,  size, NULL ); }
static inline vd_t bcore_memory_manager_malloc(            sz_t size ) { return bcore_memory_manager_b_alloc( NULL, size, NULL ); }
static inline vd_t bcore_memory_manager_realloc( vd_t ptr, sz_t size ) { return bcore_memory_manager_b_alloc( ptr,  size, NULL ); }
static inline void bcore_memory_manager_free(    vd_t ptr            ) {        bcore_memory_manager_b_alloc( ptr,  0,    NULL ); }

/// returns currently granted space (thread-safe) (use this function to test for memory leaks)
sz_t bcore_memory_manager_granted_space( void );
sz_t bcore_memory_manager_s_granted_space( bcore_memory_manager_s* o );

/// reference management
/*
vd_t bcore_memory_manager_rm_fork( vd_t ptr ); // forks a reference
vd_t bcore_memory_manager_rm_discard_f( vd_t ptr, fp_t down ); // discards using down function
vd_t bcore_memory_manager_rm_discard_p( vd_t ptr, const bcore_inst_s* spect ); // discards via perspective
*/
vd_t bcore_memory_manager_signal( tp_t target, tp_t signal, vd_t object );

#endif // BCORE_MEMORY_MANAGER_H
