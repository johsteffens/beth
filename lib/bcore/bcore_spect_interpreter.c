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

#include "bcore_spect_interpreter.h"
#include "bcore_spect_inst.h"
#include "bcore_flect.h"
#include "bcore_spect.h"
#include "bcore_trait.h"
#include "bcore_signal.h"
#include "bcore_life.h"
#include "bcore_txt_ml.h"
#include "bcore_spect_source.h"
#include "bcore_sources.h"

/**********************************************************************************************************************/
// bcore_interpreter_s

BCORE_DEFINE_SPECT( bcore_inst, bcore_interpreter )
"{"
    "bcore_spect_header_s header;"
    "feature strict aware bcore_fp:interpret;"
"}";

sr_s bcore_interpret_auto( sr_s source )
{
    bcore_life_s* l = bcore_life_s_create();
    sr_s src = sr_cp( bcore_life_s_push_sr( l, source ), TYPEOF_bcore_source_s );
    bcore_txt_ml_interpreter_s* txt_ml = bcore_life_s_push_aware( l, bcore_txt_ml_interpreter_s_create() );
    sr_s interpreter = bcore_txt_ml_interpreter_s_interpret( txt_ml, src );

    if( !interpreter.o ) bcore_source_x_parse_err_fa( src, "No interpreter specified." );

    st_s* log = bcore_life_s_push_aware( l, st_s_create() );
    if( !bcore_trait_satisfied_type( typeof( "bcore_interpreter" ), sr_s_type( &interpreter ), log ) )
    {
        bcore_source_x_parse_err_fa( src, "Object '#<sc_t>' is no interpreter.\nReason: #<sc_t>", ifnameof( sr_s_type( &interpreter ) ), log->sc );
    }

    sr_s obj = bcore_interpret_x( interpreter, src );

    bcore_life_s_discard( l );

    return obj;
}

sr_s bcore_interpret_auto_file( sc_t file )
{
    sr_s chain = sr_asd( bcore_source_chain_s_create() );
    bcore_source_chain_s_push_d( chain.o, bcore_source_file_s_create_name( file ) );
    // TODO: test if bcore_source_string_s works correctly on binary files
    bcore_source_chain_s_push_d( chain.o, bcore_inst_t_create( typeof( "bcore_source_string_s" ) ) );
    sr_s ret = bcore_interpret_auto( chain );
    return ret;
}

/**********************************************************************************************************************/

sr_s bcore_interpret_x( sr_s o, sr_s source )
{
    const bcore_interpreter_s* p = ch_spect_p( o.p, TYPEOF_bcore_interpreter_s );
    sr_s ret = p->interpret( o.o, source );
    sr_down( o );
    return ret;
}

sr_s bcore_interpret_r( const sr_s* o, sr_s source )
{
    const bcore_interpreter_s* p = ch_spect_p( o->p, TYPEOF_bcore_interpreter_s );
    return p->interpret( o->o, source );
}

/**********************************************************************************************************************/

BCORE_DEFINE_SPECT_CACHE( bcore_interpreter_s );

vd_t bcore_spect_interpreter_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_spect_interpreter" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_SPECT( bcore_interpreter );
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


