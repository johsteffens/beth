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

/// Types ans structure for amoebic function handling.

#ifndef BCORE_AMOEBA_H
#define BCORE_AMOEBA_H

#include "bcore_types.h"

typedef struct bcore_nucleus_s bcore_nucleus_s;

/// (old style) pointer to amoeba function
typedef void ( *ap_t )( vd_t nucleus );

/// pointer to amoeba function (should eventually replace ap_t)
typedef void ( *bcore_amoebic_t )( bcore_nucleus_s* nucleus );

/** Minimal nucleus (typically used for amoebic init and down functions).
 *  Actual nucleus may be bigger depending on argument list and return type.
 */
typedef struct bcore_nucleus_s
{
    bcore_amoebic_t default_handler;
    vc_t spect;
    vd_t client;
} bcore_nucleus_s;

#endif // BCORE_AMOEBA_H

