/** Author and Copyright 2017 Johannes Bernhard Steffens
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/** An adapted, simplified and stand-alone version of this memory manager has
 *  been released on https://github.com/johsteffens/tbman under a public license.
 */

#ifndef BCORE_TBMAN_H
#define BCORE_TBMAN_H

#include "bcore_types.h"
#include "bcore_st.h"

typedef void (*fp_down_obj)(           vd_t obj );
typedef void (*fp_down_arg)( vc_t arg, vd_t obj );

/**********************************************************************************************************************/

typedef struct bcore_tbman_s bcore_tbman_s;

bcore_tbman_s* bcore_tbman_s_create
(
    uz_t pool_size,
    uz_t min_block_size,
    uz_t max_block_size,
    uz_t stepping_method,
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
 *                      A value of 0 makes the function ignore current_ptr (assumes it is NULL) and does not free
 *                      memory it may reference.
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
vd_t bcore_tbman_b_alloc(                      vd_t current_ptr,                     uz_t requested_bytes, uz_t* granted_bytes );
vd_t bcore_tbman_bn_alloc(                     vd_t current_ptr, uz_t current_bytes, uz_t requested_bytes, uz_t* granted_bytes );
vd_t bcore_tbman_s_b_alloc(  bcore_tbman_s* o, vd_t current_ptr,                     uz_t requested_bytes, uz_t* granted_bytes );
vd_t bcore_tbman_s_bn_alloc( bcore_tbman_s* o, vd_t current_ptr, uz_t current_bytes, uz_t requested_bytes, uz_t* granted_bytes );

/// Like b(n)_alloc except that sizes are given as multiple of the unit-size. (thread-safe)
vd_t bcore_tbman_u_alloc(                      uz_t unit_bytes, vd_t current_ptr,                     uz_t requested_units, uz_t* granted_units );
vd_t bcore_tbman_un_alloc(                     uz_t unit_bytes, vd_t current_ptr, uz_t current_units, uz_t requested_units, uz_t* granted_units );
vd_t bcore_tbman_s_u_alloc(  bcore_tbman_s* o, uz_t unit_bytes, vd_t current_ptr,                     uz_t requested_units, uz_t* granted_units );
vd_t bcore_tbman_s_un_alloc( bcore_tbman_s* o, uz_t unit_bytes, vd_t current_ptr, uz_t current_units, uz_t requested_units, uz_t* granted_units );

/// Simplified alloc, traditional malloc, free and realloc (thread-safe).
static inline vd_t bcore_tbman_alloc(   vd_t ptr, uz_t size ) { return bcore_tbman_b_alloc( ptr,  size, NULL ); }
static inline vd_t bcore_tbman_malloc(            uz_t size ) { return bcore_tbman_b_alloc( NULL, size, NULL ); }
static inline vd_t bcore_tbman_realloc( vd_t ptr, uz_t size ) { return bcore_tbman_b_alloc( ptr,  size, NULL ); }
static inline void bcore_tbman_free(    vd_t ptr            ) {        bcore_tbman_b_alloc( ptr,  0,    NULL ); }

/** Obtains the memory instance of ptr. (Instance must exist)
  * *root is set to the instance root address,
  * *granted_space to the space the instance and 'true' is returned.
  *
  * root or granted_space may be NULL
  */
void bcore_tbman_s_get_instance( bcore_tbman_s* o, vc_t ptr, vd_t* root, uz_t* granted_space );
void bcore_tbman_get_instance(                     vc_t ptr, vd_t* root, uz_t* granted_space );

/**********************************************************************************************************************/
/// Reference Control (thread-safe)

/**
 * Functions below make use of automatic reference control for the lifetime of a root object.
 * A root object is an object which address coincides with a memory address allocated by tbman.
 * A reference to a nested object is counted as references to its respective root.
 *
 * Control data is lazily set up for root objects and only allocated when needed.
 * Objects on the stack have no root. Using reference management on those produces an error.
 *
 * For an object under reference control the destruction by functions free, alloc( size = 0 ), realloc
 * is delayed until the last reference is terminated.
 *
 * Functions:
 *  fork
 *    Creates a new reference, which must be terminated via release;
 *    Returns NULL if argument is NULL.
 *
 *  release
 *    Termination of a forked reference. Termination by owner for objects without destructor.
 *    Terminates the object when no longer referenced.
 *
 *  release_*
 *    Advanced release functions for root objects with destructor.
 *    The owner calls the proper function for release to specify how the object is to be destroyed
 *    when last reference terminates. The function can be called any time during the object's lifetime.
 *    Release discriminates single objects, arrays, objects with single-argument destructor
 *    and objects with two-argument destructor. The latter can be used for a destructor served by a perspective.
 *    down == NULL indicates that the object needs no destructor.
 *
 *  references
 *    Returns the total number of references of the root object.
 *    Returns 0 when *ptr has no root.
 *    Returning 0 implies an invalid pointer. But the inverse is not necessarily true.
 */

vd_t bcore_tbman_s_fork           ( bcore_tbman_s* o,                             vd_t ptr );
void bcore_tbman_s_release        ( bcore_tbman_s* o,                             vd_t ptr );
void bcore_tbman_s_release_obj    ( bcore_tbman_s* o, fp_down_obj down,           vd_t ptr );
void bcore_tbman_s_release_arg    ( bcore_tbman_s* o, fp_down_arg down, vc_t arg, vd_t ptr );
void bcore_tbman_s_release_obj_arr( bcore_tbman_s* o, fp_down_obj down,           vd_t ptr, uz_t size, uz_t step );
void bcore_tbman_s_release_arg_arr( bcore_tbman_s* o, fp_down_arg down, vc_t arg, vd_t ptr, uz_t size, uz_t step );
uz_t bcore_tbman_s_references     ( bcore_tbman_s* o,                             vc_t ptr );

vd_t bcore_tbman_fork           (                             vd_t ptr );
void bcore_tbman_release        (                             vd_t ptr );
void bcore_tbman_release_obj    ( fp_down_obj down,           vd_t ptr );
void bcore_tbman_release_arg    ( fp_down_arg down, vc_t arg, vd_t ptr );
void bcore_tbman_release_obj_arr( fp_down_obj down,           vd_t ptr, uz_t size, uz_t step );
void bcore_tbman_release_arg_arr( fp_down_arg down, vc_t arg, vd_t ptr, uz_t size, uz_t step );
uz_t bcore_tbman_references     (                             vc_t ptr );

/**********************************************************************************************************************/
/// Diagnostics

/// Current allocations (thread-safe)
uz_t bcore_tbman_s_granted_space(       bcore_tbman_s* o, vc_t ptr ); // space of memory instance indicated by ptr
uz_t bcore_tbman_s_total_granted_space( bcore_tbman_s* o );
uz_t bcore_tbman_s_total_instances(     bcore_tbman_s* o );
uz_t bcore_tbman_s_total_references(    bcore_tbman_s* o );

/** Applies a function to all instances.
 *  The callback function may change the state of memory manager. (E.g. free memory)
 *  Eligible instances and their order are determined before calling the first callback.
 */
void bcore_tbman_s_for_each_instance( bcore_tbman_s* o, void (*fp)( vd_t arg, vd_t ptr, uz_t space ), vd_t arg );

/// runs instance diagnostics (e.g. to determine the cause of memory leaks)
void bcore_tbman_s_instance_disgnostics( bcore_tbman_s* o );

uz_t bcore_tbman_granted_space( vc_t ptr ); // space of memory instance indicated by ptr
uz_t bcore_tbman_total_granted_space( void );
uz_t bcore_tbman_total_instances( void );
uz_t bcore_tbman_total_references( void );
void bcore_tbman_for_each_instance( void (*fp)( vd_t arg, vd_t ptr, uz_t space ), vd_t arg );
void bcore_tbman_s_instance_disgnostics();

/**********************************************************************************************************************/
/// Signal

vd_t bcore_tbman_signal_handler( const bcore_signal_s* o );

#endif // BCORE_TBMAN_H
