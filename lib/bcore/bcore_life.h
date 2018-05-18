/** Copyright 2017 Johannes Bernhard Steffens
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

#include "bcore_features.h"
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
            sz_t size; sz_t space;
        };
    };

} bcore_life_s;

void                 bcore_life_s_init(      bcore_life_s* o );
void                 bcore_life_s_down(      bcore_life_s* o );
struct bcore_life_s* bcore_life_s_create();
void                 bcore_life_s_discard(   bcore_life_s* o );

vd_t bcore_life_s_push(         bcore_life_s* o, bcore_fp_discard discard, vd_t object ); // explicit discard
vd_t bcore_life_s_push_typed(   bcore_life_s* o, tp_t type,    vd_t object ); // discard via bcore_inst_t_discard
sr_s bcore_life_s_push_sr(      bcore_life_s* o,               sr_s object ); // assumes control in case reference is strong, returns weak reference
vd_t bcore_life_s_push_aware(   bcore_life_s* o,               vd_t object ); // object is aware
vd_t bcore_life_s_push_free(    bcore_life_s* o,               vd_t object ); // uses bcore_free as discard function
vd_t bcore_life_s_typed_create( bcore_life_s* o, tp_t type                 ); // creates new object and manages its lifetime

vd_t bcore_life_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/
// macros

#define BCORE_LIFE_INIT() bcore_life_s* __life = bcore_life_s_create()
#define BCORE_LIFE_DOWN() bcore_life_s_discard( __life )
#define BCORE_LIFE_CREATE( type_name, var_name ) type_name* var_name = bcore_life_s_push_typed( __life, TYPEOF_##type_name, type_name##_create() )

#endif // BCORE_LIFE_H
