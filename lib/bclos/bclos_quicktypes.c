/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#include "bcore_quicktypes.h"
#include "bcore_st.h"
#include "bcore_spect_array.h"
#include "bcore_spect_inst.h"

/**********************************************************************************************************************/

typedef tp_t (*hf)( sc_t );

static st_s* get_def_quicktype( hf hash, sr_s string, sz_t align )
{
    sc_t name = ( ( st_s* )string.o )->sc;
    st_s* s = st_s_createf( "#define TYPEOF_%s", name );
    sz_t pad = s->size < align ? align - s->size : 1;
    st_s_push_char_n( s, ' ', pad );
    st_s_pushf( s, "% 10"PRIu32, hash( name ) );
    st_s_push_char( s, '\n' );
    sr_down( string );
    return s;
}

sr_s typelist()
{
    sr_s list = bcore_inst_typed_create_sr( bcore_flect_type_parsef( "{ st_s * [] arr; }" ) );
    bcore_array_q_push_sc( &list, "bclos_environment_s"  );
    bcore_array_q_push_sc( &list, "bclos_proc_binding_s" );
    bcore_array_q_push_sc( &list, "bclos_arguments_s" );
    bcore_array_q_push_sc( &list, "bclos_closure_s" );
    bcore_array_q_push_sc( &list, "bclos_arg_sig_s" );
    bcore_array_q_push_sc( &list, "bclos_signature_s" );
    bcore_array_q_push_sc( &list, "bclos_statement_s" );
    bcore_array_q_push_sc( &list, "bclos_procedure_s" );
    bcore_array_q_sort( &list, 0, -1, 1 );
    return list;
}

static sz_t max_len( const sr_s* list )
{
    sz_t len = 0;
    for( sz_t i = 0; i < bcore_array_q_get_size( list ); i++ )
    {
        sz_t size = ( ( st_s* )bcore_array_q_get( list, i ).o )->size;
        len = size > len ? size : len;
    }
    return len;
}

void bclos_quicktypes_to_stdout( tp_t (*hash)( sc_t name ) )
{
    hf hash_l = ( hash ) ? hash : typeof;
    sr_s list = typelist();
    for( sz_t i = 0; i < bcore_array_q_get_size( &list ); i++ ) st_s_print_d( get_def_quicktype( hash_l, bcore_array_q_get( &list, i ), 16 + max_len( &list ) ) );
    sr_down( list );
}

/**********************************************************************************************************************/

