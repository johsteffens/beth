/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_source_buffer.h"
#include "bcore_spect_inst.h"
#include "bcore_quicktypes.h"

/**********************************************************************************************************************/

void bcore_source_buffer_s_init(    bcore_source_buffer_s* o ) { bcore_inst_typed_init(    TYPEOF_bcore_source_buffer_s, o ); }
void bcore_source_buffer_s_down(    bcore_source_buffer_s* o ) { bcore_inst_typed_down(    TYPEOF_bcore_source_buffer_s, o ); }
void bcore_source_buffer_s_discard( bcore_source_buffer_s* o ) { bcore_inst_typed_discard( TYPEOF_bcore_source_buffer_s, o ); }

void bcore_source_buffer_s_copy( bcore_source_buffer_s* o, const bcore_source_buffer_s* src )
{
    bcore_inst_typed_copy( TYPEOF_bcore_source_buffer_s, o, src );
}

bcore_source_buffer_s* bcore_source_buffer_s_create()
{
    return bcore_inst_typed_create( TYPEOF_bcore_source_buffer_s );
}

bcore_source_buffer_s* bcore_source_buffer_s_clone( const bcore_source_buffer_s* o )
{
    return bcore_inst_typed_clone( TYPEOF_bcore_source_buffer_s, o );
}

/**********************************************************************************************************************/

static sz_t flow_src( bcore_source_buffer_s* o, vd_t data, sz_t size )
{
    if( size < o->size - o->index )
    {
        bcore_memcpy( data, o->data + o->index, size );
        o->index += size;
        return size;
    }

    sz_t size1 = o->size - o->index;
    bcore_memcpy( data, o->data + o->index, size1 );
    o->index += size1;
    data = ( u0_t* )data + size1;
    size -= size1;
    o->size = o->index = 0;
    if( !o->ext_supplier ) return size1;

    if( size > o->prefetch_size ) return bcore_source_get_data( o->ext_supplier, data, size ) + size1;

    if( o->space < o->prefetch_size )
    {
        o->data = bcore_bn_alloc( o->data, o->space,                0, &o->space );
        o->data = bcore_bn_alloc( o->data, o->space, o->prefetch_size, &o->space );
    }
    o->size = bcore_source_get_data( o->ext_supplier, o->data, o->prefetch_size );

    if( o->size < o->prefetch_size ) o->ext_supplier = NULL;

    if( size < o->size )
    {
        bcore_memcpy( data, o->data, size );
        o->index += size;
        return size1 + size;
    }
    else
    {
        bcore_memcpy( data, o->data, o->size );
        o->size = o->index = 0;
        return size1 + o->size;
    }
}

static void p_errorvf( bcore_source_buffer_s* o, sc_t format, va_list args )
{
    if( o->ext_supplier )
    {
        bcore_source_parse_errvf( o->ext_supplier, format, args );
    }
    else
    {
        bcore_errv( "bcore_source_buffer_s error:\n%s\n", args );
    }
}

bcore_flect_self_s* bcore_source_buffer_s_create_self()
{
    sc_t def =
    "bcore_source_buffer_s = "
    "{ "
      "aware_t _; "
      "u0_t [] data; "
      "sz_t index; "
      "external void * supplier; "
      "sz_t prefetch_size; "
    "}";
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bcore_source_buffer_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )flow_src,  "bcore_fp_flow_src", "flow_src"  );
    bcore_flect_self_s_push_external_func( self, ( fp_t )p_errorvf, "bcore_fp_logvf",    "p_errorvf" );
    return self;
}

void bcore_source_buffer_s_set_supplier( bcore_source_buffer_s* o, vd_t supplier, sz_t prefetch_size )
{
    o->ext_supplier = supplier;
    o->prefetch_size = prefetch_size;
}

/**********************************************************************************************************************/

