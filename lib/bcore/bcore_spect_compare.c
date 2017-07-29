/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_spect_compare.h"
#include "bcore_quicktypes.h"
#include "bcore_spect_array.h"
#include "bcore_spect.h"
#include "bcore_life.h"

/**********************************************************************************************************************/
// bcore_compare_s

static void compare_s_down( bcore_compare_s* o );

static void compare_s_init( bcore_compare_s* o )
{
    bcore_memzero( o, sizeof( bcore_compare_s ) );
    o->p_type = typeof( "bcore_compare_s" );
}

static void compare_s_down( bcore_compare_s* o )
{
}

static bcore_compare_s* compare_s_create()
{
    bcore_compare_s* o = bcore_alloc( NULL, sizeof( bcore_compare_s ) );
    compare_s_init( o );
    return o;
}

static void compare_s_discard( bcore_compare_s* o )
{
    if( !o ) return;
    compare_s_down( o );
    bcore_free( o );
}

static bcore_signature_s* compare_s_create_signature( bcore_compare_s* o )
{
    return bcore_signature_s_create_an( 2, o->p_type, o->o_type );
}

/**********************************************************************************************************************/

static s2_t compare_o( const bcore_compare_s* p, vc_t obj1, vc_t obj2 )
{
    return ( ( bcore_compare_fp )p->fp_compare )( obj1, obj2 );
}

static s2_t compare_generic( const bcore_compare_s* p, vc_t obj1, vc_t obj2 )
{
    if( obj1 == NULL ) return ( obj2 == NULL ) ? 0 :  1;
    if( obj2 == NULL ) return ( obj1 == NULL ) ? 0 : -1;
    const bcore_via_s* via = p->via;

    if( via->size == 0 ) // leaf types
    {
        tp_t o_type = p->o_type;
        switch( o_type )
        {
            case TYPEOF_s3_t: return ( *( s3_t* )obj1 == *( s3_t* )obj2 ) ? 0 : ( *( s3_t* )obj1 < *( s3_t* )obj2 ) ? 1 : -1;
            case TYPEOF_s2_t: return ( *( s2_t* )obj1 == *( s2_t* )obj2 ) ? 0 : ( *( s2_t* )obj1 < *( s2_t* )obj2 ) ? 1 : -1;
            case TYPEOF_s1_t: return ( *( s1_t* )obj1 == *( s1_t* )obj2 ) ? 0 : ( *( s1_t* )obj1 < *( s1_t* )obj2 ) ? 1 : -1;
            case TYPEOF_s0_t: return ( *( s0_t* )obj1 == *( s0_t* )obj2 ) ? 0 : ( *( s0_t* )obj1 < *( s0_t* )obj2 ) ? 1 : -1;
            case TYPEOF_u3_t: return ( *( u3_t* )obj1 == *( u3_t* )obj2 ) ? 0 : ( *( u3_t* )obj1 < *( u3_t* )obj2 ) ? 1 : -1;
            case TYPEOF_u2_t: return ( *( u2_t* )obj1 == *( u2_t* )obj2 ) ? 0 : ( *( u2_t* )obj1 < *( u2_t* )obj2 ) ? 1 : -1;
            case TYPEOF_u1_t: return ( *( u1_t* )obj1 == *( u1_t* )obj2 ) ? 0 : ( *( u1_t* )obj1 < *( u1_t* )obj2 ) ? 1 : -1;
            case TYPEOF_u0_t: return ( *( u0_t* )obj1 == *( u0_t* )obj2 ) ? 0 : ( *( u0_t* )obj1 < *( u0_t* )obj2 ) ? 1 : -1;
            case TYPEOF_f3_t: return ( *( f3_t* )obj1 == *( f3_t* )obj2 ) ? 0 : ( *( f3_t* )obj1 < *( f3_t* )obj2 ) ? 1 : -1;
            case TYPEOF_f2_t: return ( *( f2_t* )obj1 == *( f2_t* )obj2 ) ? 0 : ( *( f2_t* )obj1 < *( f2_t* )obj2 ) ? 1 : -1;
            case TYPEOF_sz_t: return ( *( sz_t* )obj1 == *( sz_t* )obj2 ) ? 0 : ( *( sz_t* )obj1 < *( sz_t* )obj2 ) ? 1 : -1;
            case TYPEOF_sd_t: return ( *( sd_t* )obj1 == *( sd_t* )obj2 ) ? 0 : ( *( sd_t* )obj1 < *( sd_t* )obj2 ) ? 1 : -1;
            case TYPEOF_sc_t: return ( *( sc_t* )obj1 == *( sc_t* )obj2 ) ? 0 : ( *( sc_t* )obj1 < *( sc_t* )obj2 ) ? 1 : -1;
            case TYPEOF_tp_t: return ( *( tp_t* )obj1 == *( tp_t* )obj2 ) ? 0 : ( *( tp_t* )obj1 < *( tp_t* )obj2 ) ? 1 : -1;
            case TYPEOF_bool: return ( *( bool* )obj1 == *( bool* )obj2 ) ? 0 : ( *( bool* )obj1 < *( bool* )obj2 ) ? 1 : -1;
            case TYPEOF_aware_t: return ( *( aware_t* )obj1 == *( aware_t* )obj2 ) ? 0 : ( *( aware_t* )obj1 < *( aware_t* )obj2 ) ? 1 : -1;
            default: ERR( "Cannot compare instances of type %s.", ifnameof( o_type ) );
        }
        return 0;
    }

    for( sz_t i = 0; i < via->size; i++ )
    {
        if( bcore_via_is_array( via, i ) )
        {
            const bcore_array_s* arr_p = bcore_via_get_spect_array( via, i );
            vc_t o1    = via->iget_c( via, obj1, i );
            vc_t o2    = via->iget_c( via, obj2, i );
            sz_t size1 = arr_p->get_size( arr_p, o1 );
            sz_t size2 = arr_p->get_size( arr_p, o2 );
            if( size1 != size2 ) return size1 < size2 ? 1 : -1;
            for( sz_t i = 0; i < size1; i++ )
            {
                tp_t type1 = arr_p->get_itype( arr_p, obj1, i );
                tp_t type2 = arr_p->get_itype( arr_p, obj2, i );
                if( type1 != type2 ) return ( type1 < type2 ) ? 1 : -1;
                if( type1 != 0 )
                {
                    s2_t c = bcore_compare_typed( type1, arr_p->get_c( arr_p, o1, i ), arr_p->get_c( arr_p, o2, i ) );
                    if( c != 0 ) return c;
                }
            }
        }
        else
        {
            tp_t type1 = via->iget_type( via, obj1, i );
            tp_t type2 = via->iget_type( via, obj2, i );
            if( type1 != type2 ) return ( type1 < type2 ) ? 1 : -1;
            if( type1 != 0 )
            {
                s2_t c = bcore_compare_typed( type1, via->iget_c( via, obj1, i ), via->iget_c( via, obj2, i ) );
                if( c != 0 ) return c;
            }
        }
    }

    return 0;
}

typedef struct { ap_t ap; const bcore_compare_s* p; vc_t obj1; vc_t obj2; s2_t ret; } compare_nc;
static void compare_amoeba( compare_nc* nc ) { compare_generic( nc->p, nc->obj1, nc->obj2 ); }

static s2_t compare_amoebic( const bcore_compare_s* p, vc_t obj1, vc_t obj2 )
{
    compare_nc nc = { ( ap_t )compare_amoeba, p, obj1, obj2, 0 };
    p->fp_compare( &nc );
    return nc.ret;
}

/**********************************************************************************************************************/

static bcore_compare_s* create_from_self( const bcore_flect_self_s** p_self )
{
    assert( p_self != NULL );
    const bcore_flect_self_s* self = *p_self;
    assert( self != NULL );

    bcore_compare_s* o = compare_s_create();
    o->o_type  = self->type;
    o->via     = bcore_via_s_get_typed( o->o_type );
    fp_t cmp_a =                     bcore_flect_self_s_try_external_fp( self, typeof( "ap_t" ), typeof( "compare" ) );
    fp_t cmp_o = ( cmp_a ) ? cmp_a : bcore_flect_self_s_try_external_fp( self, typeof( "bcore_compare_fp" ), 0 );

    o->fp_compare = cmp_o;
    o->compare    = cmp_a ? compare_amoebic : ( cmp_o ? compare_o : compare_generic );

    return o;
}

/**********************************************************************************************************************/

bcore_flect_self_s* bcore_compare_s_create_self( void )
{
    bcore_flect_self_s* self = bcore_flect_self_s_create_plain( bcore_name_enroll( "bcore_compare_s" ), sizeof( bcore_compare_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )compare_s_init,             "bcore_fp_init",                    "init"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )compare_s_down,             "bcore_fp_down",                    "down"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )compare_s_create,           "bcore_fp_create",                  "create"       );
    bcore_flect_self_s_push_external_func( self, ( fp_t )compare_s_discard,          "bcore_fp_discard",                 "discard"      );
    bcore_flect_self_s_push_external_func( self, ( fp_t )compare_s_create_signature, "bcore_spect_fp_create_signature",  "create_signature" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )create_from_self,           "bcore_spect_fp_create_from_self",  "create_from_self" );
    return self;
}

/**********************************************************************************************************************/

const bcore_compare_s* bcore_compare_s_get_typed( tp_t type )
{
    return bcore_spect_get_typed( typeof( "bcore_compare_s" ), type );
}

s2_t bcore_compare_spect( const bcore_compare_s* p, vc_t obj1, vc_t obj2 )
{
    return p->compare( p, obj1, obj2 );
}

s2_t bcore_compare_typed( tp_t type, vc_t obj1, vc_t obj2 )
{
    return bcore_compare_spect( bcore_compare_s_get_typed( type ), obj1, obj2 );
}

bool bcore_equal_spect( const bcore_compare_s* p, vc_t obj1, vc_t obj2 )
{
    return bcore_compare_spect( p, obj1, obj2 ) == 0;
}

bool bcore_equal_typed( tp_t type, vc_t obj1, vc_t obj2 )
{
    return bcore_compare_typed( type, obj1, obj2 ) == 0;
}

s2_t bcore_compare_aware( vc_t obj1, vc_t obj2 )
{
    if( !obj1 ) return obj2 ?  1 : 0;
    if( !obj2 ) return obj1 ? -1 : 0;
    tp_t type1 = *( aware_t* )obj1;
    tp_t type2 = *( aware_t* )obj2;
    if( type1 != type2 ) return type1 < type2 ? 1 : -1;
    return bcore_compare_typed( type1, obj1, obj2 );
}

bool bcore_equal_aware( vc_t obj1, vc_t obj2 )
{
    return bcore_compare_aware( obj1, obj2 ) == 0;
}

/**********************************************************************************************************************/
/// testing

bcore_string_s* bcore_spect_compare_selftest()
{
    bcore_life_s* l = bcore_life_s_create();

    bcore_flect_parse_sc( "cmp_specs = { aware_t _; sz_t size; u2_t param1; s2_t; }" );
    vd_t specs = bcore_life_s_push_aware( l, bcore_inst_typed_create( typeof( "cmp_specs" ) ) );

    {
        sz_t size   =  10;
        u2_t param1 = 200;
        s2_t param2 = -50;
        const bcore_via_s* v = bcore_via_s_get_aware( specs );
        v->nset_c( v, specs, typeof( "size"   ), &size   );
        v->nset_c( v, specs, typeof( "param1" ), &param1 );
        v->nset_c( v, specs, typeof( ""       ), &param2 );
    }

    bcore_flect_parse_sc( "cmp_specs_arr = { aware_t _; u3_t flags; cmp_specs [] arr; }" );
    vd_t arr1 = bcore_life_s_push_aware( l, bcore_inst_typed_create( typeof( "cmp_specs_arr" ) ) );

    const bcore_array_s* arr_p = bcore_array_s_get_aware( arr1 );

    for( sz_t i = 0; i < 100; i++ ) arr_p->push_c( arr_p, arr1, specs );

    for( sz_t i = 0; i < arr_p->get_size( arr_p, arr1 ); i++ )
    {
        vd_t specs = arr_p->get_d( arr_p, arr1, i );
        const bcore_via_s* v = bcore_via_s_get_aware( specs );
        v->nset_c( v, specs, typeof( "size" ), &i );
    }

    const bcore_via_s* v = bcore_via_s_get_aware( specs );
    vd_t arr2 = bcore_life_s_push_aware( l, bcore_inst_aware_clone( arr1 ) );

    ASSERT( bcore_compare_aware( arr1, arr2 ) == 0 );
    sz_t num = 20;
    v->nset_c( v, arr_p->get_d( arr_p, arr2, 5 ), typeof( "size" ), &num );
    ASSERT( bcore_compare_aware( arr1, arr2 ) >  0 );
    num = 0;
    v->nset_c( v, arr_p->get_d( arr_p, arr2, 5 ), typeof( "size" ), &num );
    ASSERT( bcore_compare_aware( arr1, arr2 ) <  0 );

    bcore_life_s_discard( l );
    return NULL;
}

