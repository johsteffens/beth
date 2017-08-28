/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_quicktypes.h"
#include "bcore_string.h"

/**********************************************************************************************************************/

typedef tp_t (*hf)( sc_t );

static void print_type( hf hash, sc_t name )
{
    bcore_string_s* s = bcore_string_s_createf( "TYPEOF_%s", name );
    sz_t align = 32;
    sz_t pad = s->size < align ? align - s->size : 1;
    bcore_string_s_push_char_n( s, ' ', pad );
    bcore_string_s_pushf( s, "% 10"PRIu32, hash( name ) );
    bcore_msg( "#define %s\n", s->sc );
    bcore_string_s_discard( s );
}

void bclos_quicktypes_to_stdout( tp_t (*hash)( sc_t name ) )
{
    hf hash_l = ( hash ) ? hash : typeof;
    print_type( hash_l, "bclos_tree_s" );
}

/**********************************************************************************************************************/

