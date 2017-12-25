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

typedef pthread_mutex_t bcore_mutex_t;
void bcore_mutex_init(   bcore_mutex_t* o );
void bcore_mutex_down(   bcore_mutex_t* o );
void bcore_mutex_lock(   bcore_mutex_t* o );
void bcore_mutex_unlock( bcore_mutex_t* o );

typedef pthread_once_t bcore_once_t;
#define bcore_once_init PTHREAD_ONCE_INIT
void bcore_once( bcore_once_t* flag, void (*func)() );

vd_t bcore_threads_signal( tp_t target, tp_t signal, vd_t object );

#endif // BCORE_THREADS_H

