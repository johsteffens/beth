/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_spect_compare.h"
#include "bcore_quicktypes.h"
#include "bcore_spect_array.h"
#include "bcore_spect.h"
#include "bcore_life.h"
#include "bcore_bml.h"

/**********************************************************************************************************************/
// bcore_compare_s

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
    return ( ( bcore_fp_compare )p->fp_compare )( obj1, obj2 );
}

static s2_t compare_leaf( tp_t type, vc_t obj1, vc_t obj2 )
{
    switch( type )
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
        case TYPEOF_sd_t: return bcore_strcmp( *( sd_t* )obj1, *( sd_t* )obj2 );
        case TYPEOF_sc_t: return bcore_strcmp( *( sc_t* )obj1, *( sc_t* )obj2 );
        case TYPEOF_tp_t: return ( *( tp_t* )obj1 == *( tp_t* )obj2 ) ? 0 : ( *( tp_t* )obj1 < *( tp_t* )obj2 ) ? 1 : -1;
        case TYPEOF_bl_t: return ( *( bl_t* )obj1 == *( bl_t* )obj2 ) ? 0 : ( *( bl_t* )obj1 < *( bl_t* )obj2 ) ? 1 : -1;
        case TYPEOF_aware_t: return ( *( aware_t* )obj1 == *( aware_t* )obj2 ) ? 0 : ( *( aware_t* )obj1 < *( aware_t* )obj2 ) ? 1 : -1;
        default: ERR( "Cannot compare instances of leaf type %s.", ifnameof( type ) );
    }
    return 0;
}

static s2_t compare_generic( const bcore_compare_s* p, vc_t obj1, vc_t obj2 )
{
    if( obj1 == obj2 ) return  0;
    if( obj1 == NULL ) return  1;
    if( obj2 == NULL ) return -1;
    if( p->via->size == 0 ) return compare_leaf( p->o_type, obj1, obj2 );
    const bcore_via_s* via = p->via;
    for( sz_t i = 0; i < via->size; i++ )
    {
        if( bcore_via_spect_iis_array( via, i ) )
        {
            const bcore_array_s* arr_p = bcore_via_spect_iget_array( via, i );
            vc_t o1_l  = bcore_via_spect_iget_c( via, obj1, i );
            vc_t o2_l  = bcore_via_spect_iget_c( via, obj2, i );
            sz_t size1 = arr_p->get_size( arr_p, o1_l );
            sz_t size2 = arr_p->get_size( arr_p, o2_l );
            if( size1 != size2 ) return size1 < size2 ? 1 : -1;
            for( sz_t j = 0; j < size1; j++ )
            {
                tp_t type1 = arr_p->get_itype( arr_p, o1_l, j );
                tp_t type2 = arr_p->get_itype( arr_p, o2_l, j );
                vc_t o1_ll = arr_p->get_c(     arr_p, o1_l, j );
                vc_t o2_ll = arr_p->get_c(     arr_p, o2_l, j );
                s2_t c = bcore_compare_bityped( type1, o1_ll, type2, o2_ll );
                if( c != 0 ) return c;
            }
        }
        else
        {
            tp_t type1 = bcore_via_spect_iget_type( via, obj1, i );
            tp_t type2 = bcore_via_spect_iget_type( via, obj2, i );
            vc_t o1_l  = bcore_via_spect_iget_c(    via, obj1, i );
            vc_t o2_l  = bcore_via_spect_iget_c(    via, obj2, i );
            s2_t c = bcore_compare_bityped( type1, o1_l, type2, o2_l );
            if( c != 0 ) return c;
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

static bcore_string_s* diff_generic( const bcore_compare_s* p, vc_t obj1, vc_t obj2 )
{
    if( obj1 == obj2 ) return NULL;
    if( obj1 == NULL ) return bcore_string_s_createf( "obj1 == NULL, obj2!%s", ifnameof( p->o_type ) );
    if( obj2 == NULL ) return bcore_string_s_createf( "obj1!%s, obj2 == NULL", ifnameof( p->o_type ) );
    if( p->via->size == 0 )
    {
        s2_t c = compare_leaf( p->o_type, obj1, obj2 );
        if( c != 0 )
        {
            bcore_string_s* s = bcore_string_s_create();
            bcore_string_s_pushf( s, "!%s: obj1:", ifnameof( p->o_type ) );
            bcore_string_s_push_typed( s, p->o_type, obj1 );
            bcore_string_s_pushf( s, "obj2:", ifnameof( p->o_type ) );
            bcore_string_s_push_typed( s, p->o_type, obj2 );
            return s;
        }
    }
    const bcore_via_s* via = p->via;
    for( sz_t i = 0; i < via->size; i++ )
    {
        if( bcore_via_spect_iis_array( via, i ) )
        {
            const bcore_array_s* arr_p = bcore_via_spect_iget_array( via, i );
            vc_t o1_l  = bcore_via_spect_iget_c( via, obj1, i );
            vc_t o2_l  = bcore_via_spect_iget_c( via, obj2, i );
            sz_t size1 = arr_p->get_size( arr_p, o1_l );
            sz_t size2 = arr_p->get_size( arr_p, o2_l );
            if( size1 != size2 )
            {
                return bcore_string_s_createf( "!%s::%s: !%s: size1:%zu, size2:%zu",
                                              ifnameof( p->o_type ), ifnameof( bcore_via_spect_iget_name( via, i ) ),
                                              ifnameof( bcore_via_spect_iget_type( via, obj1, i ) ), size1, size2 );
            }
            for( sz_t j = 0; j < size1; j++ )
            {
                tp_t type1 = arr_p->get_itype( arr_p, o1_l, j );
                tp_t type2 = arr_p->get_itype( arr_p, o2_l, j );
                vc_t o1_ll = arr_p->get_c(     arr_p, o1_l, j );
                vc_t o2_ll = arr_p->get_c(     arr_p, o2_l, j );
                bcore_string_s* s = bcore_diff_bityped( type1, o1_ll, type2, o2_ll );
                if( s != NULL )
                {
                    return bcore_string_s_pushf( s, "\n!%s::%s: !%s[%zu]",
                                                ifnameof( p->o_type ), ifnameof( bcore_via_spect_iget_name( via, i ) ),
                                                ifnameof( bcore_via_spect_iget_type( via, obj1, i ) ), j );
                }
            }
        }
        else
        {
            tp_t type1 = bcore_via_spect_iget_type( via, obj1, i );
            tp_t type2 = bcore_via_spect_iget_type( via, obj2, i );
            vc_t o1_l  = bcore_via_spect_iget_c(    via, obj1, i );
            vc_t o2_l  = bcore_via_spect_iget_c(    via, obj2, i );
            bcore_string_s* s = bcore_diff_bityped( type1, o1_l, type2, o2_l );
            if( s != NULL )
            {
                return bcore_string_s_pushf( s, "\n!%s::%s", ifnameof( p->o_type ), ifnameof( bcore_via_spect_iget_name( via, i ) ) );
            }
        }
    }
    return NULL;
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
    fp_t cmp_o = ( cmp_a ) ? cmp_a : bcore_flect_self_s_try_external_fp( self, typeof( "bcore_fp_compare" ), 0 );

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

s2_t bcore_compare_bityped( tp_t type1, vc_t obj1, tp_t type2, vc_t obj2 )
{
    if( type1 != type2 ) return ( type1 < type2 ) ? 1 : -1;
    if( type1 == 0     ) return 0;
    return bcore_compare_typed( type1, obj1, obj2 );
}

bool bcore_equal_spect( const bcore_compare_s* p, vc_t obj1, vc_t obj2 )
{
    return bcore_compare_spect( p, obj1, obj2 ) == 0;
}

bool bcore_equal_typed( tp_t type, vc_t obj1, vc_t obj2 )
{
    return bcore_compare_typed( type, obj1, obj2 ) == 0;
}

bool bcore_equal_bityped( tp_t type1, vc_t obj1, tp_t type2, vc_t obj2 )
{
    return bcore_compare_bityped( type1, obj1, type2, obj2 ) == 0;
}

s2_t bcore_compare_aware( vc_t obj1, vc_t obj2 )
{
    tp_t type1 = obj1 ? *( aware_t* )obj1 : 0;
    tp_t type2 = obj2 ? *( aware_t* )obj2 : 0;
    return bcore_compare_bityped( type1, obj1, type2, obj2 );
}

bool bcore_equal_aware( vc_t obj1, vc_t obj2 )
{
    return bcore_compare_aware( obj1, obj2 ) == 0;
}

bcore_string_s* bcore_diff_spect( const bcore_compare_s* p, vc_t obj1, vc_t obj2 )
{
    if( p->compare == compare_generic )
    {
        return diff_generic( p, obj1, obj2 );
    }
    else
    {
        if( bcore_compare_spect( p, obj1, obj2 ) != 0 )
        {
            bcore_string_s* s = bcore_string_s_createf( "!%s: objects overloading comparison differ:", ifnameof( p->o_type ) );
            bcore_string_s_pushf( s, "\n\tobj1: " );
            bcore_bml_typed_to_string( p->o_type, obj1, s );
            bcore_string_s_pushf( s, "\tobj2: " );
            bcore_bml_typed_to_string( p->o_type, obj2, s );
            return s;
        }
    }
    return NULL;
}

bcore_string_s* bcore_diff_typed( tp_t type, vc_t obj1, vc_t obj2 )
{
    return bcore_diff_spect( bcore_compare_s_get_typed( type ), obj1, obj2 );
}

bcore_string_s* bcore_diff_bityped( tp_t type1, vc_t obj1, tp_t type2, vc_t obj2 )
{
    if( type1 != type2 ) return bcore_string_s_createf( "obj1 of '%s', obj2 of '%s'", ifnameof( type1 ), ifnameof( type2 ) );
    if( type1 == 0 ) return NULL;
    return bcore_diff_typed( type1, obj1, obj2 );
}

bcore_string_s* bcore_diff_aware( vc_t obj1, vc_t obj2 )
{
    tp_t type1 = obj1 ? *( aware_t* )obj1 : 0;
    tp_t type2 = obj2 ? *( aware_t* )obj2 : 0;
    return bcore_diff_bityped( type1, obj1, type2, obj2 );
}

/**********************************************************************************************************************/
/// testing

bcore_string_s* bcore_spect_compare_selftest( void )
{
    bcore_life_s* l = bcore_life_s_create();

    bcore_flect_define_parse_sc( "cmp_specs = { aware_t _; sz_t size; u2_t param1; s2_t; }" );
    vd_t specs = bcore_life_s_push_aware( l, bcore_inst_typed_create( typeof( "cmp_specs" ) ) );

    {
        sz_t size   =  10;
        u2_t param1 = 200;
        s2_t param2 = -50;
        const bcore_via_s* v = bcore_via_s_get_aware( specs );
        bcore_via_spect_nset_c( v, specs, typeof( "size"   ), &size   );
        bcore_via_spect_nset_c( v, specs, typeof( "param1" ), &param1 );
        bcore_via_spect_nset_c( v, specs, typeof( ""       ), &param2 );
    }

    bcore_flect_define_parse_sc( "cmp_specs_arr = { aware_t _; u3_t flags; cmp_specs [] arr; }" );
    vd_t arr1 = bcore_life_s_push_aware( l, bcore_inst_typed_create( typeof( "cmp_specs_arr" ) ) );

    const bcore_array_s* arr_p = bcore_array_s_get_aware( arr1 );

    for( sz_t i = 0; i < 100; i++ ) arr_p->push_c( arr_p, arr1, specs );

    for( sz_t i = 0; i < arr_p->get_size( arr_p, arr1 ); i++ )
    {
        vd_t specs = arr_p->get_d( arr_p, arr1, i );
        const bcore_via_s* v = bcore_via_s_get_aware( specs );
        bcore_via_spect_nset_c( v, specs, typeof( "size" ), &i );
    }

    const bcore_via_s* v = bcore_via_s_get_aware( specs );
    vd_t arr2 = bcore_life_s_push_aware( l, bcore_inst_aware_clone( arr1 ) );

    ASSERT( bcore_compare_aware( arr1, arr2 ) == 0 );
    sz_t num = 20;
    bcore_via_spect_nset_c( v, arr_p->get_d( arr_p, arr2, 5 ), typeof( "size" ), &num );
    ASSERT( bcore_compare_aware( arr1, arr2 ) >  0 );
    num = 0;
    bcore_via_spect_nset_c( v, arr_p->get_d( arr_p, arr2, 5 ), typeof( "size" ), &num );
    ASSERT( bcore_compare_aware( arr1, arr2 ) <  0 );

    bcore_life_s_discard( l );
    return NULL;
}

