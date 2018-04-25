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

#include "bcore_spect_translator.h"
#include "bcore_spect_inst.h"
#include "bcore_flect.h"
#include "bcore_spect.h"
#include "bcore_trait.h"
#include "bcore_signal.h"

/**********************************************************************************************************************/
// bcore_translator_s

/**********************************************************************************************************************/

static void translator_s_define_trait()
{
    tp_t trait = entypeof( "bcore_translator" );
    bcore_trait_require_function( trait, entypeof( "bcore_fp_translate" ), 0 );
    bcore_trait_set( trait, entypeof( "bcore_inst" ) );
}

BCORE_DEFINE_SPECT( bcore_translator_s )
"{"
    "bcore_spect_header_s header;"
    "strict feature bcore_fp:translate;"
"}";

/**********************************************************************************************************************/

void bcore_translate_x( sr_s o, sr_s obj, sr_s sink )
{
    const bcore_translator_s* p = ch_spect_p( o.p, TYPEOF_bcore_translator_s );
    p->translate( o.o, obj, sink );
    sr_down( o );
}

void bcore_translate_r( const sr_s* o, sr_s obj, sr_s sink )
{
    const bcore_translator_s* p = ch_spect_p( o->p, TYPEOF_bcore_translator_s );
    p->translate( o->o, obj, sink );
}

/**********************************************************************************************************************/

vd_t bcore_spect_translator_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_spect_translator" ) ) )
    {
        case TYPEOF_init1:
        {
            translator_s_define_trait();
            BCORE_REGISTER_SPECT( bcore_translator_s );
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

/**********************************************************************************************************************/

