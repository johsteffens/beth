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

#ifndef BCORE_THREADS_H
#define BCORE_THREADS_H

#include <pthread.h>
#include "bcore_types.h"
#include "bcore_features.h"

/**********************************************************************************************************************/
// mutex

typedef pthread_mutex_t bcore_mutex_s;

void           bcore_mutex_init( bcore_mutex_s* o );
void           bcore_mutex_down( bcore_mutex_s* o );
bcore_mutex_s* bcore_mutex_create();
void           bcore_mutex_discard( bcore_mutex_s* o );

void bcore_mutex_s_lock(   bcore_mutex_s* o );
void bcore_mutex_s_unlock( bcore_mutex_s* o );

/**********************************************************************************************************************/
// once

typedef pthread_once_t bcore_once_s;
#define bcore_once_init PTHREAD_ONCE_INIT
void bcore_once_s_run( bcore_once_s* flag, void (*func)() );

/**********************************************************************************************************************/
// condition

typedef pthread_cond_t bcore_condition_s;

void               bcore_condition_init( bcore_condition_s* o );
void               bcore_condition_down( bcore_condition_s* o );
bcore_condition_s* bcore_condition_create();
void               bcore_condition_discard( bcore_condition_s* o );

/**********************************************************************************************************************/

vd_t bcore_threads_signal_handler( const bcore_signal_s* o );

#endif // BCORE_THREADS_H

