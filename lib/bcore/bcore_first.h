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

#ifndef BCORE_FIRST_H
#define BCORE_FIRST_H

/// This header should be included first of all other headers

/** Runtime checks are enabled by flag RTCHECKS which should
 *  preferably enabled by the build environment in DEBUG modes.
 */
#ifndef RTCHECKS
    #ifndef NDEBUG
        #define NDEBUG
    #endif
#endif

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <stdalign.h>

#include <math.h>

#endif // BCORE_FIRST_H
