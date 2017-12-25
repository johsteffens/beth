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

#include "bcore_tp.h"
#include "bcore_name_manager.h"

/**********************************************************************************************************************/

vd_t bcore_tp_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bcore_tp" ) ) return NULL;

    if( signal == typeof( "init1" ) )
    {
        return NULL;
    }
    else if( signal == typeof( "selftest" ) )
    {
        return NULL;
    }

    return NULL;
}

/**********************************************************************************************************************/

