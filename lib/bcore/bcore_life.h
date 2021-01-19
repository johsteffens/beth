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

#ifndef BCORE_LIFE_H
#define BCORE_LIFE_H

#include "bcore_feature.h"
#include "bcore_flect.h"

/** Simple lifetime manager for objects.
 *  bcore_life assumes ownership of objects with respect to managing their lifetime.
 *  Objects remain alive until the instance of bcore_life terminates.
 */

typedef void (*bcore_fp_down_or_discard       )(            vd_t  o );
typedef void (*bcore_fp_typed_down_or_discard )( tp_t type, vd_t  o );

typedef struct bcore_life_item_s
{
    union
    {
        bcore_fp_down_or_discard             down_or_discard;
        bcore_fp_typed_down_or_discard typed_down_or_discard;
    };
    tp_t type;  // 0 for untyped objects
    vd_t object;
} bcore_life_item_s;


typedef struct bcore_life_s
{
    aware_t _;

    /** The parent is used to indicate a block-nested life definition inside a function;
     *  Used in function bcore_life_s_down_all.
     */
    bcore_life_s* parent;
    sz_t nesting_level;

    union
    {
        bcore_array_dyn_solid_static_s arr;
        struct
        {
            bcore_life_item_s* data;
            uz_t size; uz_t space;
        };
    };

} bcore_life_s;

BCORE_DECLARE_FUNCTION_INIT(    bcore_life_s )
BCORE_DECLARE_FUNCTION_DOWN(    bcore_life_s )
BCORE_DECLARE_FUNCTION_CREATE(  bcore_life_s )
BCORE_DECLARE_FUNCTION_DISCARD( bcore_life_s )
BCORE_DECLARE_FUNCTION_DETACH(  bcore_life_s )

void bcore_life_s_init_setup( bcore_life_s* o, bcore_life_s* parent );
void bcore_life_s_setup( bcore_life_s* o, bcore_life_s* parent );

/// 'push': objects on heap ...
vd_t bcore_life_s_push(         bcore_life_s* o, bcore_fp_discard discard, vd_t object ); // explicit discard
vd_t bcore_life_s_push_typed(   bcore_life_s* o, tp_t type,                vd_t object ); // discard via bcore_inst_t_discard
sr_s bcore_life_s_push_sr(      bcore_life_s* o,                           sr_s object ); // assumes control in case reference is strong, returns weak reference
vd_t bcore_life_s_push_aware(   bcore_life_s* o,                           vd_t object ); // object is aware (discard via inst_a_discard)
vd_t bcore_life_s_push_free(    bcore_life_s* o,                           vd_t object ); // uses bcore_free as discard function

vd_t bcore_life_s_push_level_typed( bcore_life_s* o, sz_t nesting_level, tp_t type, vd_t object ); // discard via bcore_inst_t_discard
vd_t bcore_life_s_push_level_aware( bcore_life_s* o, sz_t nesting_level,            vd_t object ); // object is aware (discard via inst_a_discard)

/// 'spush' objects on stack ...
vd_t bcore_life_s_spush(         bcore_life_s* o, bcore_fp_down down, vd_t object ); // explicit down
vd_t bcore_life_s_spush_typed(   bcore_life_s* o, tp_t type,          vd_t object ); // down via bcore_inst_t_down
vd_t bcore_life_s_spush_aware(   bcore_life_s* o,                     vd_t object ); // object is aware (discard via inst_a_down)

vd_t bcore_life_s_typed_create( bcore_life_s* o, tp_t type ); // creates new object and manages its lifetime

/// shuts down *o and zeros *o (o can be NULL)
void bcore_life_s_down_zero( bcore_life_s** o );

/// shuts down *o and all parents where nesting level >= nesting_level (o can be NULL)
void bcore_life_s_down_level( bcore_life_s* o, sz_t nesting_level );

/// down/discard o and all parents (o can be NULL)
void bcore_life_s_down_all(    bcore_life_s* o );
void bcore_life_s_discard_all( bcore_life_s* o );

vd_t bcore_life_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/
// macros

/** Block-Lifetime-Management
 *  Usage:
 *  {
 *      BLM_INIT(); // at the beginning of a managed block
 *
 *      .... regular code inside a block
 *
 *      BLM_DOWN(); // at the end of a managed block
 *  }
 *
 *  BLM_CREATE, BLM_CLONE, BLM_A_PUSH, BLM_T_PUSH
 *  place objects on lifetime management.
 *
 *  BLM_DOWN, BLM_RETURN or BLM_RETURNV
 *  shut down the manager and discard all managed objects
 *
 *  BLM_RETURN(V) also return from a function.
 *  It may be called even inside a nested block in which case
 *  all eligible parent managers are shut down as well.
 *
 *  Example:
 *  {
 *      BLM_INIT(); // bock 1
 *      ....
 *      {
 *          BLM_INIT(); // bock 2
 *          ....
 *          if( condition )
 *          {
 *              // This shuts down BLMs of blocks 2 and 1, then returns the function.
 *              BLM_RETURNV( type, val );
 *          }
 *          ....
 *          BLM_DOWN();
 *      }
 *      ....
 *      BLM_DOWN();
 *  }
 *
 */

extern bcore_life_s* __bcore_life; // always NULL; needed to ingrain a life-chain

#define BLM_INIT() \
    bcore_life_s* __bcore_life_curent = __bcore_life; \
    bcore_life_s  __bcore_life_on_stack; \
    bcore_life_s* __bcore_life = &__bcore_life_on_stack; \
    bcore_life_s_init_setup( __bcore_life, __bcore_life_curent ); \
    __bcore_life_curent = __bcore_life;

/// explicitly sets an (incremental) nesting level
#define BLM_INIT_LEVEL( arg_nesting_level ) \
    BLM_INIT(); \
    assert( arg_nesting_level >= __bcore_life_curent->nesting_level ); \
    __bcore_life_curent->nesting_level = arg_nesting_level;

/// BLM_DOWN can be used even when __bcore_life is NULL
#define BLM_DOWN() bcore_life_s_down_zero( &__bcore_life )

#define BLM_CREATE( tname ) ( tname* )bcore_life_s_push_typed( __bcore_life_curent, TYPEOF_##tname, tname##_create() )
#define BLM_CLONE(  tname, expr ) ( tname* )bcore_life_s_push_typed( __bcore_life_curent, TYPEOF_##tname, tname##_clone( expr ) )
#define BLM_A_PUSH(        expr ) bcore_life_s_push_aware( __bcore_life_curent,       expr )
#define BLM_T_PUSH(  type, expr ) bcore_life_s_push_typed( __bcore_life_curent, TYPEOF_##type, expr )
#define BLM_T_SPUSH( type, expr ) bcore_life_s_spush_typed( __bcore_life_curent, TYPEOF_##type, expr )
#define BLM_T_INIT_SPUSH( type, expr ) type##_init( expr ); bcore_life_s_spush_typed( __bcore_life_curent, TYPEOF_##type, expr )
#define BLM_LEVEL_A_PUSH( level,       expr ) bcore_life_s_push_level_aware( __bcore_life_curent, level, expr )
#define BLM_LEVEL_T_PUSH( level, type, expr ) bcore_life_s_push_level_typed( __bcore_life_curent, level, TYPEOF_##type, expr )
#define BLM_X_PUSH(  expr ) bcore_life_s_push_sr(    __bcore_life_curent,       expr )
#define BLM_A_CLONE( expr ) bcore_life_s_push_aware( __bcore_life_curent, bcore_inst_a_clone( ( bcore_inst* )expr ) )

/** breaks current nesting level (unsafe)
 *  Preferably use BLM_BREAK_LEVEL( nesting_level ) to ensure all nested life managers are shut down
 */
#define BLM_BREAK() \
{ \
    bcore_life_s_down_zero( &__bcore_life ); \
    break; \
}

/// breaks all levels down to (including) nesting_level
#define BLM_BREAK_LEVEL( nesting_level ) \
{ \
    bcore_life_s_down_level( __bcore_life, nesting_level ); \
    break; \
}

#define BLM_RETURN() \
{ \
    bcore_life_s_down_all( __bcore_life ); \
    return; \
}

#define BLM_RETURNV( ret_type, expr ) \
{ \
    ret_type __retv = expr;  \
    bcore_life_s_down_all( __bcore_life );  \
    return __retv;  \
}

/// creates object and runs a member function as continuation
#define BLM_CREATEC( tname, fname, ... ) tname##_##fname( BLM_CREATE( tname ), __VA_ARGS__ )

/// clones object and runs a member function as continuation
#define BLM_CLONEC( tname, src, fname, ... ) tname##_##fname( BLM_CLONE( tname, src ), __VA_ARGS__ )

/** Running expression with error management:
 *    - BLM_TRY should be used inside a function returning er_t.
 *    - expression should return er_t.
 */
#define BLM_TRY( expression ) \
{ \
    er_t __blm_err = expression; \
    if( __blm_err ) BLM_RETURNV( er_t, __blm_err ) \
}

/// Error report in function with error management
#define BLM_ERR_FA( ... ) \
BLM_RETURNV( er_t, bcore_error_push_fa( TYPEOF_general_error, __VA_ARGS__ ) )

#endif // BCORE_LIFE_H
