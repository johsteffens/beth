/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_spect_closure.h"
#include "bcore_quicktypes.h"
#include "bcore_life.h"

#include "bclos_lang_c.h"

/**********************************************************************************************************************/
/// translator

typedef struct bclos_lang_c_translator_s
{
    aware_t _;
}
bclos_lang_c_translator_s;

void bclos_lang_c_translator_s_init( bclos_lang_c_translator_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = typeof( "bclos_lang_c_translator_s" );
}

void bclos_lang_c_translator_s_down( bclos_lang_c_translator_s* o )
{
    bcore_inst_aware_down( o );
}

void bclos_lang_c_translator_s_copy( bclos_lang_c_translator_s* o, const bclos_lang_c_translator_s* src )
{
    bcore_inst_aware_copy( o, src );
}

bclos_lang_c_translator_s* bclos_lang_c_translator_s_create()
{
    return bcore_inst_typed_create( typeof( "bclos_lang_c_translator_s" ) );
}

bclos_lang_c_translator_s* bclos_lang_c_translator_s_clone( const bclos_lang_c_translator_s* o )
{
    return bcore_inst_aware_clone( o );
}

void bclos_lang_c_translator_s_discard( bclos_lang_c_translator_s* o )
{
    bcore_inst_aware_discard( o );
}

static bcore_flect_self_s* translator_s_create_self( void )
{
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( "bclos_lang_c_translator_s = { aware_t _; sz_t tab_size; bool suppress_aware; bool break_leaf;}", sizeof( bclos_lang_c_translator_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bclos_lang_c_translator_s_init,        "bcore_fp_init",             "init"             );
//    bcore_flect_self_s_push_external_func( self, ( fp_t )bclos_lang_c_translator_s_translate_object, "bcore_fp_translate_object", "translate_object" );
//    bcore_flect_self_s_push_external_func( self, ( fp_t )bclos_lang_c_translator_s_translate_body,   "bcore_fp_translate_body",   "translate_body"   );
    return self;
}

/**********************************************************************************************************************/

void bclos_lang_c_define_self_creators( void )
{
    bcore_flect_define_creator( typeof( "bclos_lang_c_translator_s" ), translator_s_create_self );
}

/**********************************************************************************************************************/

bcore_string_s* bclos_lang_c_selftest( void )
{
    return NULL;
}
