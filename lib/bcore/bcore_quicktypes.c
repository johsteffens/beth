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

void bcore_quicktypes_to_stdout( tp_t (*hash)( sc_t name ) )
{
    hf hash_l = ( hash ) ? hash : typeof;
    bcore_msg( "// leaf types\n" );
    print_type( hash_l, "s3_t" );
    print_type( hash_l, "s2_t" );
    print_type( hash_l, "s1_t" );
    print_type( hash_l, "s0_t" );
    print_type( hash_l, "u3_t" );
    print_type( hash_l, "u2_t" );
    print_type( hash_l, "u1_t" );
    print_type( hash_l, "u0_t" );
    print_type( hash_l, "f3_t" );
    print_type( hash_l, "f2_t" );
    print_type( hash_l, "sz_t" );
    print_type( hash_l, "sd_t" );
    print_type( hash_l, "sc_t" );
    print_type( hash_l, "vd_t" );
    print_type( hash_l, "vc_t" );
    print_type( hash_l, "fp_t" );
    print_type( hash_l, "tp_t" );
    print_type( hash_l, "bool" );
    print_type( hash_l, "aware_t" );
    print_type( hash_l, "bcore_flect_self_s" );

    bcore_msg( "\n// frequently used types\n" );
    print_type( hash_l, "bcore_string_s" );
    print_type( hash_l, "bcore_source_string_s" );
    print_type( hash_l, "bcore_source_buffer_s" );

    bcore_msg( "\n// attributes\n" );
    print_type( hash_l, "private" );
    print_type( hash_l, "cyclic" );
    print_type( hash_l, "external" );
}

/**********************************************************************************************************************/

