/** Copyright 2018 Johannes Bernhard Steffens
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

#include "bcore_feature.h"
#include "bcore_control.h"
#include "bcore_signal.h"
#include "bcore_flect.h"

/**********************************************************************************************************************/

vd_t bcore_feature_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_feature" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_FEATURE( bcore_fp_init );
            BCORE_REGISTER_FEATURE( bcore_fp_down );
            BCORE_REGISTER_FEATURE( bcore_fp_copy );
            BCORE_REGISTER_FEATURE( bcore_fp_move );
            BCORE_REGISTER_FEATURE( bcore_fp_create );
            BCORE_REGISTER_FEATURE( bcore_fp_discard );
            BCORE_REGISTER_FEATURE( bcore_fp_clone );
            BCORE_REGISTER_FEATURE( bcore_fp_copy_typed );
            BCORE_REGISTER_FEATURE( bcore_fp_create_typed );
            BCORE_REGISTER_FEATURE( bcore_fp_translate );
            BCORE_REGISTER_FEATURE( bcore_fp_interpret );
            BCORE_REGISTER_FEATURE( bcore_fp_flow_snk );
            BCORE_REGISTER_FEATURE( bcore_fp_flow_src );
        }
        break;

        case TYPEOF_selftest:
        {
        }
        break;

        default: break;
    }
    return NULL;
}
