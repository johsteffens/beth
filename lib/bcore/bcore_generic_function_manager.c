/** Author and Copyright 2026 Johannes Bernhard Steffens
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

#include "bcore_generic_function_manager.h"
#include "bcore_threads.h"
#include "bcore_hmap.h"
#include "bcore_life.h"
#include "bcore_signal.h"
#include "bcore_spect_inst.h"
#include "bcore_spect_array.h"
#include "bcore_signal.h"
#include "bcore_tbman.h"
#include "bcore_sources.h"


static bcore_generic_function_manager_s* manager_g = NULL;

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

BCORE_DEFINE_OBJECT_INST( bcore_inst, bcore_generic_function_item_s )
"{"
    "aware_t _;"
    "tp_t type;"
    "tp_t name;" // optional name of argument (in case of return type: function name)
    "tp_t access_class;" // 'const|mutable|discardable'
    "s2_t indirection;"
    "bl_t flag_obliv;"    // obliv indicates that this type can reference obliv objects (extends range of implicit casts)
    "bl_t flag_aware;"    // aware object is required (allows extended tests)
    "bl_t flag_restrict;" // C restrict keyword was used in declaration
"}";

//----------------------------------------------------------------------------------------------------------------------

er_t bcore_generic_function_item_s_parse_cx_code( bcore_generic_function_item_s* o, bcore_source* source )
{
    BLM_INIT();

    o->access_class = TYPEOF_mutable;

    if( bcore_source_a_parse_bl_fa( source, " #?w'obliv'" ) ) o->flag_obliv = true;
    if( bcore_source_a_parse_bl_fa( source, " #?w'aware'" ) ) o->flag_aware = true;

    if     ( bcore_source_a_parse_bl_fa( source, " #?w'c'"           ) ) o->access_class = TYPEOF_const;
    else if( bcore_source_a_parse_bl_fa( source, " #?w'm'"           ) ) o->access_class = TYPEOF_mutable;
    else if( bcore_source_a_parse_bl_fa( source, " #?w'd'"           ) ) o->access_class = TYPEOF_discardable;
    else if( bcore_source_a_parse_bl_fa( source, " #?w'const'"       ) ) o->access_class = TYPEOF_const;
    else if( bcore_source_a_parse_bl_fa( source, " #?w'mutable'"     ) ) o->access_class = TYPEOF_mutable;
    else if( bcore_source_a_parse_bl_fa( source, " #?w'discardable'" ) ) o->access_class = TYPEOF_discardable;

    st_s* st_name = BLM_CREATE( st_s );
    BLM_TRY( bcore_source_a_parse_em_fa( source, " #name", st_name ) );

    if( st_name->size == 0 )
    {
        BLM_TRY( bcore_source_a_parse_error_fa( source, "Type name expected." ) );
    }

    tp_t type = bentypeof( st_name->sc );

    if( !bcore_flect_exists( type ) )
    {
        BLM_TRY( bcore_source_a_parse_error_fa( source, "Type '#name' has no reflection.", type ) );
    }

    o->type = type;

    bcore_source_a_parse_em_fa( source, " " );
    while( bcore_source_a_parse_bl_fa( source, " #?'*'" ) )
    {
        o->indirection++;
    }

    if( bcore_source_a_parse_bl_fa( source, " #?w'restrict'" ) ) o->flag_restrict = true;

    if( bcore_source_a_parse_bl_fa( source, " #?(([0]>='A'&&[0]<='Z')||([0]>='a'&&[0]<='z')||([0]=='_'))" ) )
    {
        BLM_TRY( bcore_source_a_parse_em_fa( source, "#name", st_name ) );
        o->name = bentypeof( st_name->sc );
    }

    BLM_RETURNV( er_t, 0 );
}

//----------------------------------------------------------------------------------------------------------------------

er_t bcore_generic_function_item_s_parse_cx_code_sc( bcore_generic_function_item_s* o, sc_t sc )
{
    st_s* wst = st_s_create_weak_sc( sc );
    er_t err = bcore_generic_function_item_s_parse_cx_code( o, ( bcore_source* )wst );
    st_s_discard( wst );
    return err;
}

//----------------------------------------------------------------------------------------------------------------------

er_t bcore_generic_function_item_s_parse_cx_code_st( bcore_generic_function_item_s* o, const st_s* st )
{
    st_s* wst = st_s_create_weak_sc( st->sc );
    er_t err = bcore_generic_function_item_s_parse_cx_code( o, ( bcore_source* )wst );
    st_s_discard( wst );
    return err;
}

//----------------------------------------------------------------------------------------------------------------------

bcore_generic_function_item_s* bcore_generic_function_item_s_create_parse_cx_code( bcore_source* source )
{
    bcore_generic_function_item_s* item = bcore_generic_function_item_s_create();
    bcore_generic_function_item_s_parse_cx_code( item, source );
    return item;
}

//----------------------------------------------------------------------------------------------------------------------

bcore_generic_function_item_s* bcore_generic_function_item_s_create_parse_cx_code_sc( sc_t sc )
{
    bcore_generic_function_item_s* item = bcore_generic_function_item_s_create();
    bcore_generic_function_item_s_parse_cx_code_sc( item, sc );
    return item;
}

//----------------------------------------------------------------------------------------------------------------------

bcore_generic_function_item_s* bcore_generic_function_item_s_create_parse_cx_code_st( const st_s* st )
{
    bcore_generic_function_item_s* item = bcore_generic_function_item_s_create();
    bcore_generic_function_item_s_parse_cx_code_st( item, st );
    return item;
}

//----------------------------------------------------------------------------------------------------------------------

er_t bcore_generic_function_item_s_push_c_code_raw( const bcore_generic_function_item_s* o, st_s* st )
{
    if( o->access_class == TYPEOF_const ) st_s_push_fa( st, "const " );
    st_s_push_fa( st, "#name", o->type );
    for( sz_t i = 0; i < o->indirection; i++ ) st_s_push_char( st, '*' );
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

er_t bcore_generic_function_item_s_push_c_code( const bcore_generic_function_item_s* o, st_s* st )
{
    if( o->access_class == TYPEOF_const ) st_s_push_fa( st, "const " );
    st_s_push_fa( st, "#name", o->type );
    for( sz_t i = 0; i < o->indirection; i++ ) st_s_push_char( st, '*' );

    if( o->flag_restrict ) st_s_push_fa( st, " restrict" );
    if( o->name ) st_s_push_fa( st, " #name", o->name );
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

er_t bcore_generic_function_item_s_push_x_code( const bcore_generic_function_item_s* o, st_s* st )
{
    if     ( o->flag_aware ) st_s_push_fa( st, "aware " );
    else if( o->flag_obliv ) st_s_push_fa( st, "obliv " );

    if     ( o->access_class == TYPEOF_const   ) st_s_push_fa( st, "c " );
    else if( o->access_class == TYPEOF_mutable ) st_s_push_fa( st, "m " );
    else if( o->access_class == TYPEOF_discardable ) st_s_push_fa( st, "d " );

    st_s_push_fa( st, "#name", o->type );
    for( sz_t i = 0; i < o->indirection; i++ ) st_s_push_char( st, '*' );

    if( o->flag_restrict ) st_s_push_fa( st, " restrict" );
    if( o->name ) st_s_push_fa( st, " #name", o->name );
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

er_t bcore_generic_function_item_s_push_c_code_vd_arg_cast( const bcore_generic_function_item_s* o, st_s* st )
{
    if( o->indirection == 0 ) st_s_push_fa( st, "*" );
    st_s_push_fa( st, "(" );
    BLM_TRY( bcore_generic_function_item_s_push_c_code_raw( o, st ) );
    if( o->indirection == 0 ) st_s_push_fa( st, "*" );
    st_s_push_fa( st, ")" );
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

er_t bcore_generic_function_item_s_push_c_code_vd_ret_cast( const bcore_generic_function_item_s* o, st_s* st )
{
    st_s_push_fa( st, "*" );
    st_s_push_fa( st, "(" );
    BLM_TRY( bcore_generic_function_item_s_push_c_code_raw( o, st ) );
    st_s_push_fa( st, "*" );
    st_s_push_fa( st, ")" );
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

BCORE_DEFINE_OBJECT_INST( bcore_array, bcore_generic_function_s )
"{"
    "aware_t _;"
    "hidden fp_t fp;" // castable to bcore_fp_generic_wrapper
    "bcore_generic_function_item_s base;"
    "bcore_generic_function_item_s => [];"
"}";

//----------------------------------------------------------------------------------------------------------------------

bcore_generic_function_s* bcore_generic_function_s_clear( bcore_generic_function_s* o )
{
    bcore_array_a_set_space( ( bcore_array* )o, 0 );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

bcore_generic_function_s* bcore_generic_function_s_set_size( bcore_generic_function_s* o, uz_t size )
{
    bcore_array_a_set_size( ( bcore_array* )o, size );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

bcore_generic_function_item_s* bcore_generic_function_s_push_item_d( bcore_generic_function_s* o, bcore_generic_function_item_s* item )
{
    bcore_array_a_push( ( bcore_array* )o, sr_asm( item ) );
    return item;
}

//----------------------------------------------------------------------------------------------------------------------

bcore_generic_function_item_s* bcore_generic_function_s_push_item( bcore_generic_function_s* o, const bcore_generic_function_item_s* item )
{
    return bcore_generic_function_s_push_item_d( o, bcore_generic_function_item_s_clone( item ) );
}

//----------------------------------------------------------------------------------------------------------------------

bcore_generic_function_item_s* bcore_generic_function_s_push( bcore_generic_function_s* o )
{
    return bcore_generic_function_s_push_item_d( o, bcore_generic_function_item_s_create() );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_generic_function_s_pop( bcore_generic_function_s* o )
{
    bcore_array_a_pop( ( bcore_array* )o );
}

//----------------------------------------------------------------------------------------------------------------------

er_t bcore_generic_function_s_parse_cx_code( bcore_generic_function_s* o, bcore_source* source )
{
    BLM_INIT();
    BLM_TRY( bcore_generic_function_item_s_parse_cx_code( &o->base, source ) );
    BLM_TRY( bcore_source_a_parse_em_fa( source, " ( " ) );

    while( !bcore_source_a_parse_bl_fa( source, " #=?')'" ) && !bcore_source_a_eos( source ) )
    {
        bcore_generic_function_item_s* item = BLM_A_PUSH( bcore_generic_function_item_s_create_parse_cx_code( source ) );
        if( !bcore_source_a_parse_bl_fa( source, " #=?')'" ) ) BLM_TRY( bcore_source_a_parse_em_fa( source, " , " ) );
        bcore_generic_function_s_push_item_d( o, bcore_fork( item ) );
    }

    if( o->size == 1 && o->data[0]->type == TYPEOF_void && o->data[0]->indirection == 0 )
    {
        bcore_generic_function_s_pop( o );
    }

    BLM_RETURNV( er_t, 0 );
}

//----------------------------------------------------------------------------------------------------------------------

er_t bcore_generic_function_s_parse_cx_code_sc( bcore_generic_function_s* o, sc_t  sc )
{
    st_s* wst = st_s_create_weak_sc( sc );
    er_t err = bcore_generic_function_s_parse_cx_code( o, ( bcore_source* )wst );
    st_s_discard( wst );
    return err;
}

//----------------------------------------------------------------------------------------------------------------------

er_t bcore_generic_function_s_parse_cx_code_st( bcore_generic_function_s* o, const st_s* st )
{
    st_s* wst = st_s_create_weak_sc( st->sc );
    er_t err = bcore_generic_function_s_parse_cx_code( o, ( bcore_source* )wst );
    st_s_discard( wst );
    return err;
}

//----------------------------------------------------------------------------------------------------------------------

er_t bcore_generic_function_s_push_c_code( const bcore_generic_function_s* o, st_s* st )
{
    BLM_TRY( bcore_generic_function_item_s_push_c_code( &o->base, st ) );
    st_s_push_fa( st, "( " );
    if( o->size == 0 ) st_s_push_fa( st, "void" );
    for( sz_t i = 0; i < o->size; i++ )
    {
        bcore_generic_function_item_s_push_c_code( o->data[ i ], st );
        if( i + 1 < o->size ) st_s_push_fa( st, ", " );
    }
    st_s_push_fa( st, " )" );
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

er_t bcore_generic_function_s_push_x_code( const bcore_generic_function_s* o, st_s* st )
{
    BLM_TRY( bcore_generic_function_item_s_push_x_code( &o->base, st ) );
    st_s_push_fa( st, "( " );
    for( sz_t i = 0; i < o->size; i++ )
    {
        bcore_generic_function_item_s_push_x_code( o->data[ i ], st );
        if( i + 1 < o->size ) st_s_push_fa( st, ", " );
    }
    st_s_push_fa( st, " )" );
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_generic_function_s_returns_value( const bcore_generic_function_s* o )
{
    return o->base.indirection > 0 || o->base.type != TYPEOF_void;
}

//----------------------------------------------------------------------------------------------------------------------

er_t bcore_generic_function_s_push_c_code_bgwf_declaration( const bcore_generic_function_s* o, st_s* st )
{
    st_s_push_fa( st, "void #name__bgwf( vd_t r, vd_t a[] )", o->base.name );
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

er_t bcore_generic_function_s_push_c_code_bgwf_definition_one_line( const bcore_generic_function_s* o, st_s* st )
{
    bcore_generic_function_s_push_c_code_bgwf_declaration( o, st );
    st_s_push_fa( st, "{ " );

    if( bcore_generic_function_s_returns_value( o ) )
    {
        bcore_generic_function_item_s_push_c_code_vd_ret_cast( &o->base, st );
        st_s_push_fa( st, "r = " );
    }

    st_s_push_fa( st, "#name( ", o->base.name );

    for( sz_t i = 0; i < o->size; i++ )
    {
        bcore_generic_function_item_s_push_c_code_vd_arg_cast( o->data[ i ], st );
        st_s_push_fa( st, "a[#<sz_t>]", i );
        if( i + 1 < o->size ) st_s_push_fa( st, ", " );
    }

    st_s_push_fa( st, "); }", o->base.name );

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

BCORE_DEFINE_OBJECT_INST( bcore_inst, bcore_generic_function_manager_s )
"{"
    "aware_t _;"
    "bcore_mutex_s mutex;"
    "bcore_hmap_tpaw_s hmap;"
"}";

//----------------------------------------------------------------------------------------------------------------------

sz_t bcore_generic_function_manager_size( void )
{
    bcore_mutex_s_lock( &manager_g->mutex );
    sz_t size = bcore_hmap_tpaw_s_keys( &manager_g->hmap );
    bcore_mutex_s_unlock( &manager_g->mutex );
    return size;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_generic_function_manager_exists( tp_t name )
{
    bcore_mutex_s_lock( &manager_g->mutex );
    bl_t exists = bcore_hmap_tpaw_s_exists( &manager_g->hmap, name );
    bcore_mutex_s_unlock( &manager_g->mutex );
    return exists;
}

//----------------------------------------------------------------------------------------------------------------------

const bcore_generic_function_s* bcore_generic_function_manager_get( tp_t name )
{
    bcore_mutex_s_lock( &manager_g->mutex );
    vd_t* pvd = bcore_hmap_tpaw_s_get( &manager_g->hmap, name );
    bcore_mutex_s_unlock( &manager_g->mutex );
    return pvd ? ( const bcore_generic_function_s* )( *pvd ) : NULL;
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_generic_function_manager_remove( tp_t name )
{
    bcore_mutex_s_lock( &manager_g->mutex );
    bcore_hmap_tpaw_s_remove( &manager_g->hmap, name );
    bcore_mutex_s_unlock( &manager_g->mutex );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_generic_function_manager_set_d( bcore_generic_function_s* function )
{
    bcore_mutex_s_lock( &manager_g->mutex );
    bcore_hmap_tpaw_s_set_d( &manager_g->hmap, function->base.name, function );
    bcore_mutex_s_unlock( &manager_g->mutex );
}

//----------------------------------------------------------------------------------------------------------------------

bcore_generic_function_s* bcore_generic_function_manager_set( tp_t name )
{
    bcore_generic_function_s* function = bcore_generic_function_s_create();
    bcore_generic_function_manager_set_d( function );
    return function;
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_generic_function_manager_set_c( const bcore_generic_function_s* function )
{
    bcore_generic_function_manager_set_d( bcore_generic_function_s_clone( function ) );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_generic_function_manager_register_func( sc_t x_code, bcore_fp_generic_wrapper fp )
{
    bcore_generic_function_s* function = bcore_generic_function_s_create();
    bcore_generic_function_s_parse_cx_code_sc( function, x_code );
    function->fp = fp;
    bcore_generic_function_manager_set_d( function );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// selftest

//----------------------------------------------------------------------------------------------------------------------

static s2_t test_foo( const st_s* st, s2_t a, s2_t b )
{
    return st->size + a + b;
}

//----------------------------------------------------------------------------------------------------------------------

static void test_foo__bgwf( vd_t r, vd_t a[] ){ *(s2_t*)r = test_foo( (const st_s*)a[0], *(s2_t*)a[1], *(s2_t*)a[2]); }

//----------------------------------------------------------------------------------------------------------------------

#include "bcore_x_btml.h"

void test_declaration( sc_t sc )
{
    BLM_INIT();
    bcore_generic_function_s* f = BLM_A_PUSH( bcore_generic_function_s_create() );
    bcore_generic_function_s_parse_cx_code_sc( f, sc );
    st_s* st = BLM_CREATE( st_s );
    bcore_generic_function_s_push_c_code( f, st );
//    bcore_msg_fa( "#<sc_t>\n", sc );
//    bcore_msg_fa( "#<sc_t>\n", st->sc );
    ASSERT( st_s_equal_sc( st, sc ) );
    BLM_DOWN();
}

static st_s* selftest( void )
{
    BLM_INIT();
    test_declaration( "void f( void )" );
    test_declaration( "s2_t* f( s2_t*** a, s2_t b )" );
    test_declaration( "s2_t* f( s2_t*** a, s2_t* restrict b )" );
    test_declaration( "s2_t test_foo( const st_s* st, s2_t a, s2_t b )" );

    bcore_generic_function_s* f0 = BLM_A_PUSH( bcore_generic_function_s_create() );

    sc_t sc_declaration = "s2_t test_foo( const st_s* st, s2_t a, s2_t b )";
    bcore_generic_function_s_parse_cx_code_sc( f0, sc_declaration );
    f0->fp = test_foo__bgwf;

    bcore_generic_function_manager_register_func( sc_declaration, test_foo__bgwf );

    st_s* arg1 = BLM_A_PUSH( st_s_create_sc( "123" ) );
    s2_t arg2 = 10;
    s2_t arg3 = 20;

    s2_t ret = test_foo( arg1, arg2, arg3 );
    ASSERT( ret == 33 );

    vd_t vd_args[ 3 ] = { arg1, &arg2, &arg3 };
    ret = 0;
    test_foo__bgwf( &ret, vd_args );
    ASSERT( ret == 33 );

    const bcore_generic_function_s* f1 = bcore_generic_function_manager_get( btypeof( "test_foo" ) );
    ret = 0;
    f1->fp( &ret, vd_args );
    ASSERT( ret == 33 );

//    st_s* st = BLM_CREATE( st_s );
//    bcore_generic_function_s_push_c_code_bgwf_definition_one_line( f1, st );
//    bcore_msg_fa( "#<sc_t>\n", st->sc );

    BLM_RETURNV( st_s*, NULL );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// signal

//----------------------------------------------------------------------------------------------------------------------

vd_t bcore_generic_function_manager_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_generic_function_manager" ) ) )
    {
        case TYPEOF_init0:
        {
        }
        break;

        case TYPEOF_init1:
        {
            BCORE_REGISTER_OBJECT( bcore_generic_function_item_s );
            BCORE_REGISTER_OBJECT( bcore_generic_function_s );
            BCORE_REGISTER_OBJECT( bcore_generic_function_manager_s );
            manager_g = bcore_generic_function_manager_s_create();
        }
        break;

        case TYPEOF_down1:
        {
            s2_t verbosity = o->object ? *( s2_t* )o->object : 0;
            if( verbosity > 0 )
            {
                uz_t count = bcore_generic_function_manager_size();
                uz_t space = bcore_tbman_total_granted_space();
                bcore_generic_function_manager_s_detach( &manager_g );
                space -= bcore_tbman_total_granted_space();
                bcore_msg( "  bcore_generic_function_manager .. % 7zu (by % 4zu functions   )\n", space, count );
            }
            else
            {
                bcore_generic_function_manager_s_detach( &manager_g );
            }
        }
        break;

        case TYPEOF_selftest:
        {
            return selftest();
        }
        break;

        default: break;
    }

    return NULL;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
