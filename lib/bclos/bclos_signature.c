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

#include "bclos_signature.h"
#include "bclos_quicktypes.h"
#include "bcore_spect_inst.h"
#include "bcore_spect_array.h"
#include "bcore_spect_source.h"
#include "bcore_sources.h"
#include "bcore_life.h"

/**********************************************************************************************************************/

BCORE_DEFINE_FUNCTION_INIT_FLAT( bclos_signature_arg_s )
BCORE_DEFINE_FUNCTION_DOWN_FLAT( bclos_signature_arg_s )
BCORE_DEFINE_FUNCTION_COPY_FLAT( bclos_signature_arg_s )
BCORE_DEFINE_FUNCTION_CREATE(    bclos_signature_arg_s )
BCORE_DEFINE_FUNCTION_DISCARD(   bclos_signature_arg_s )
BCORE_DEFINE_FUNCTION_CLONE(     bclos_signature_arg_s )

static bcore_flect_self_s* arg_signature_s_create_self( void )
{
    sc_t def = "bclos_signature_arg_s = bcore_inst { tp_t name; tp_t type; bl_t is_const; }";
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bclos_signature_arg_s ) );
    return self;
}

/**********************************************************************************************************************/

BCORE_DEFINE_FUNCTION_INIT_INST( bclos_signature_s )
BCORE_DEFINE_FUNCTION_DOWN_INST( bclos_signature_s )
BCORE_DEFINE_FUNCTION_COPY_INST( bclos_signature_s )
BCORE_DEFINE_FUNCTION_CREATE(     bclos_signature_s )
BCORE_DEFINE_FUNCTION_DISCARD(    bclos_signature_s )
BCORE_DEFINE_FUNCTION_CLONE(      bclos_signature_s )

void bclos_signature_s_push( bclos_signature_s* o, bclos_signature_arg_s arg )
{
    if( o->size > o->space ) bcore_array_typed_make_strong( TYPEOF_bclos_signature_s, o );
    if( o->size == o->space )
    {
        o->data = bcore_un_alloc
        (
            sizeof( bclos_signature_arg_s ),
            o->data,
            o->space,
            o->space > 0 ? o->space * 2 : 1,
            &o->space
        );
    }
    o->data[ o->size++ ] = arg;
}

bclos_signature_s* bclos_signature_s_parse_from_source( sr_s source )
{
    bcore_life_s* l = bcore_life_s_create();
    source = bcore_life_s_push_sr( l, source );
    bclos_signature_s* s = bclos_signature_s_create();

    st_s* name = st_s_create_l( l );
    bcore_source_q_parse_fa( &source, " #name", name );
    if( bcore_source_q_parse_bl_fa( &source, " #?'('" ) )
    {
        s->ret = 0;
        s->name = bcore_name_enroll( name->sc );
    }
    else
    {
        s->ret = bcore_name_enroll( name->sc );
        bcore_source_q_parse_fa( &source, " #name (", name );
        s->name = bcore_name_enroll( name->sc );
    }

    while( !bcore_source_q_parse_bl_fa( &source, " #?')'" ) )
    {
        if( s->size > 0 ) bcore_source_q_parse_fa( &source, " ," );
        bclos_signature_arg_s arg;
        arg.is_const = bcore_source_q_parse_bl_fa( &source, " #?w'const'" );
        bcore_source_q_parse_fa( &source, " #name ", name );
        arg.type = bcore_name_enroll( name->sc );
        bcore_source_q_parse_fa( &source, " #name ", name );
        arg.name = bcore_name_enroll( name->sc );
        bclos_signature_s_push( s, arg );
    }

    bcore_life_s_discard( l );
    return s;
}

bclos_signature_s* bclos_signature_s_parse_from_sc( sc_t sc )
{
    return bclos_signature_s_parse_from_source( sr_asd( st_s_create_weak_sc( sc ) ) );
}

static bcore_flect_self_s* sig_s_create_self( void )
{
    sc_t def = "bclos_signature_s = bcore_inst { bclos_signature_arg_s [] arr; tp_t name; tp_t ret; }";
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bclos_signature_s ) );
    return self;
}

/**********************************************************************************************************************/

vd_t bclos_signature_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bclos_signature" ) ) return NULL;

    if( signal == typeof( "init1" ) )
    {
        bcore_flect_define_creator( typeof( "bclos_signature_arg_s" ), arg_signature_s_create_self );
        bcore_flect_define_creator( typeof( "bclos_signature_s"     ), sig_s_create_self );
    }

    return NULL;
}
