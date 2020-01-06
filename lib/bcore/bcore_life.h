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

typedef struct bcore_life_item_s
{
    bcore_fp_discard discard;
    tp_t type;  // 0 for untyped objects
    vd_t object;
} bcore_life_item_s;

typedef struct bcore_life_s
{
    aware_t _;
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

vd_t bcore_life_s_push(         bcore_life_s* o, bcore_fp_discard discard, vd_t object ); // explicit discard
vd_t bcore_life_s_push_typed(   bcore_life_s* o, tp_t type,    vd_t object ); // discard via bcore_inst_t_discard
sr_s bcore_life_s_push_sr(      bcore_life_s* o,               sr_s object ); // assumes control in case reference is strong, returns weak reference
vd_t bcore_life_s_push_aware(   bcore_life_s* o,               vd_t object ); // object is aware
vd_t bcore_life_s_push_free(    bcore_life_s* o,               vd_t object ); // uses bcore_free as discard function
vd_t bcore_life_s_typed_create( bcore_life_s* o, tp_t type                 ); // creates new object and manages its lifetime

vd_t bcore_life_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/
// macros

// preferably use abbreviated versions below
#define BCORE_LIFE_INIT() bcore_life_s* __life = bcore_life_s_create()
#define BCORE_LIFE_DOWN() bcore_life_s_detach( &__life )
#define BCORE_LIFE_CREATE( type_name, var_name ) type_name* var_name = bcore_life_s_push_typed( __life, TYPEOF_##type_name, type_name##_create() )
#define BCORE_LIFE_CREATE_AUT( type_name, var_name ) type_name* var_name = bcore_life_s_push_typed( __life, typeof( #type_name ), type_name##_create() )
#define BCORE_LIFE_A_PUSH(       expr ) bcore_life_s_push_aware( __life,       expr )
#define BCORE_LIFE_T_PUSH( type, expr ) bcore_life_s_push_typed( __life, type, expr )
#define BCORE_LIFE_X_PUSH(       expr ) bcore_life_s_push_sr(    __life,       expr )
#define BCORE_LIFE_RETURNV( ret_type, expr ) { ret_type __retv = expr; BCORE_LIFE_DOWN(); return __retv; }
#define BCORE_LIFE_RETURN()                  {                         BCORE_LIFE_DOWN(); return;        }

// abbreviated versions (Note: _CREATE is different)
#define BLM_INIT() bcore_life_s* __life = bcore_life_s_create()
#define BLM_DOWN() bcore_life_s_detach( &__life )
#define BLM_CREATE( tname ) ( tname* )bcore_life_s_push_typed( __life, TYPEOF_##tname, tname##_create() )
#define BLM_CLONE(  tname, src ) ( tname* )bcore_life_s_push_typed( __life, TYPEOF_##tname, tname##_clone( src ) )
#define BLM_A_PUSH(       expr ) bcore_life_s_push_aware( __life,       expr )
#define BLM_T_PUSH( type, expr ) bcore_life_s_push_typed( __life, type, expr )
#define BLM_RETURN()                  {                         BLM_DOWN(); return;        }
#define BLM_RETURNV( ret_type, expr ) { ret_type __retv = expr; BLM_DOWN(); return __retv; }

// creates object and runs a member function as continuation
#define BLM_CREATEC( tname, fname, ... ) tname##_##fname( BLM_CREATE( tname ), __VA_ARGS__ )

#endif // BCORE_LIFE_H
