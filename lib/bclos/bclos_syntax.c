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

#include "bclos_syntax.h"
#include "bclos_quicktypes.h"
#include "bcore_spect_inst.h"
#include "bcore_life.h"
#include "bcore_name.h"
#include "bcore_spect_source.h"

/**********************************************************************************************************************/

DEFINE_FUNCTION_INIT_INST( bclos_syntax_s )
DEFINE_FUNCTION_DOWN_INST( bclos_syntax_s )
DEFINE_FUNCTION_COPY_INST( bclos_syntax_s )
DEFINE_FUNCTION_CREATE(     bclos_syntax_s )
DEFINE_FUNCTION_DISCARD(    bclos_syntax_s )
DEFINE_FUNCTION_CLONE(      bclos_syntax_s )

static bcore_flect_self_s* syntax_s_create_self( void )
{
    sc_t def = "bclos_syntax_s = bcore_inst { aware_t _; }";
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bclos_syntax_s ) );
    return self;
}
/*
static sr_s parse_expression( sr_s src )
{
    bcore_life_s* l = bcore_life_s_create();
    src = bcore_life_s_push_sr( l, sr_cp( src, TYPEOF_bcore_source_s ) );

    sr_s ret = sr_null();

    bcore_name_map_s* name_map = bcore_name_map_s_create();

    if( bcore_source_q_parse_bl_fa( &src, " #?w'class' " ) )
    {
        bcore_flect_self_s* self = bcore_flect_self_s_create();
        st_s* string = st_s_create();
        bcore_source_q_parse_fa( &src, "#name { ", string );
        if( string )
        {
            bcore_name_map_s_set( name_map, bcore_name_sc( string->sc ) );
            self->trait = typeof( string->sc );
        }
        while( !bcore_source_q_parse_bl_fa( &src, " #?'}' " ) )
        {

        }

        st_s_discard( string );
    }

    bcore_life_s_discard( l );

    return ret;
}
*/
/*
bclos_library_s* bclos_syntax_s_create_library( const bclos_syntax_s* o, const bclos_library_arr_s* deps, sr_s src )
{
    src = sr_cp( src, TYPEOF_bcore_source_s );

    void bcore_source_q_parse_fa(     const sr_s* o, sc_t format, ... );


}
*/
/**********************************************************************************************************************/

vd_t bclos_syntax_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bclos_syntax" ) ) return NULL;

    if( signal == typeof( "init1" ) )
    {
        bcore_flect_define_creator( typeof( "bclos_syntax_s" ), syntax_s_create_self );
    }

    return NULL;
}

/**********************************************************************************************************************/

