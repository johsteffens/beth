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

#include "bcore_tp.h"
#include "bcore_name_manager.h"
#include "bcore_signal.h"

/**********************************************************************************************************************/


//----------------------------------------------------------------------------------------------------------------------

tp_t bcore_tp_is_numeric( tp_t t )
{
    switch( t )
    {
        case TYPEOF_s3_t: return true;
        case TYPEOF_s2_t: return true;
        case TYPEOF_s1_t: return true;
        case TYPEOF_s0_t: return true;
        case TYPEOF_u3_t: return true;
        case TYPEOF_u2_t: return true;
        case TYPEOF_u1_t: return true;
        case TYPEOF_u0_t: return true;
        case TYPEOF_f3_t: return true;
        case TYPEOF_f2_t: return true;
        case TYPEOF_sz_t: return true;
        case TYPEOF_uz_t: return true;
        case TYPEOF_tp_t: return true;
        case TYPEOF_bl_t: return true;
        case TYPEOF_aware_t: return true;
        default: return false;
    }
}

//----------------------------------------------------------------------------------------------------------------------

tp_t bcore_tp_is_float( tp_t t )
{
    switch( t )
    {
        case TYPEOF_f3_t: return true;
        case TYPEOF_f2_t: return true;
        default: return false;
    }
}

//----------------------------------------------------------------------------------------------------------------------

tp_t bcore_tp_is_integer( tp_t t )
{
    return bcore_tp_is_numeric( t ) && !bcore_tp_is_float( t );
}

//----------------------------------------------------------------------------------------------------------------------

tp_t bcore_tp_is_unsigned( tp_t t )
{
    switch( t )
    {
        case TYPEOF_u3_t:    return true;
        case TYPEOF_u2_t:    return true;
        case TYPEOF_u1_t:    return true;
        case TYPEOF_u0_t:    return true;
        case TYPEOF_uz_t:    return true;
        case TYPEOF_tp_t:    return true;
        case TYPEOF_aware_t: return true;
        default: return false;
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

vd_t bcore_tp_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_tp" ) ) )
    {
        case TYPEOF_init1:
        {
            return NULL;
        }
        break;

        default: break;
    }
    return NULL;
}

/**********************************************************************************************************************/

