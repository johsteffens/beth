/** Author and Copyright 2017 Johannes Bernhard Steffens
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

#include "bcore_spect_compare.h"
#include "bcore_signal.h"
#include "bcore_spect_array.h"
#include "bcore_spect.h"
#include "bcore_trait.h"
#include "bcore_spect_translator.h"
#include "bcore_life.h"

/**********************************************************************************************************************/
// bcore_compare_s

//----------------------------------------------------------------------------------------------------------------------

static void compare_s_init( bcore_compare_s* o )
{
    bcore_memzero( o, sizeof( bcore_compare_s ) );
    o->p_type = typeof( "bcore_compare_s" );
}


//----------------------------------------------------------------------------------------------------------------------

static void compare_s_down( bcore_compare_s* o )
{
}


//----------------------------------------------------------------------------------------------------------------------

static bcore_compare_s* compare_s_create()
{
    bcore_compare_s* o = bcore_alloc( NULL, sizeof( bcore_compare_s ) );
    compare_s_init( o );
    return o;
}


//----------------------------------------------------------------------------------------------------------------------

static void compare_s_discard( bcore_compare_s* o )
{
    if( !o ) return;
    bcore_release_obj( ( fp_t )compare_s_down, o );
}


//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

static s2_t compare_s3_t( const bcore_compare_s* p, const s3_t* o1, const s3_t* o2 )
{
    return o1 ? ( o2 ? ( ( *o1 < *o2 ) ? 1 : ( ( *o1 > *o2 ) ? -1 : 0 ) ) : -1 ) : ( o2 ? 1 : 0 );
}

//----------------------------------------------------------------------------------------------------------------------

static s2_t compare_s2_t( const bcore_compare_s* p, const s2_t* o1, const s2_t* o2 )
{
    return o1 ? ( o2 ? ( ( *o1 < *o2 ) ? 1 : ( ( *o1 > *o2 ) ? -1 : 0 ) ) : -1 ) : ( o2 ? 1 : 0 );
}

//----------------------------------------------------------------------------------------------------------------------

static s2_t compare_s1_t( const bcore_compare_s* p, const s1_t* o1, const s1_t* o2 )
{
    return o1 ? ( o2 ? ( ( *o1 < *o2 ) ? 1 : ( ( *o1 > *o2 ) ? -1 : 0 ) ) : -1 ) : ( o2 ? 1 : 0 );
}

//----------------------------------------------------------------------------------------------------------------------

static s2_t compare_s0_t( const bcore_compare_s* p, const s0_t* o1, const s0_t* o2 )
{
    return o1 ? ( o2 ? ( ( *o1 < *o2 ) ? 1 : ( ( *o1 > *o2 ) ? -1 : 0 ) ) : -1 ) : ( o2 ? 1 : 0 );
}

//----------------------------------------------------------------------------------------------------------------------

static s2_t compare_u3_t( const bcore_compare_s* p, const u3_t* o1, const u3_t* o2 )
{
    return o1 ? ( o2 ? ( ( *o1 < *o2 ) ? 1 : ( ( *o1 > *o2 ) ? -1 : 0 ) ) : -1 ) : ( o2 ? 1 : 0 );
}

//----------------------------------------------------------------------------------------------------------------------

static s2_t compare_u2_t( const bcore_compare_s* p, const u2_t* o1, const u2_t* o2 )
{
    return o1 ? ( o2 ? ( ( *o1 < *o2 ) ? 1 : ( ( *o1 > *o2 ) ? -1 : 0 ) ) : -1 ) : ( o2 ? 1 : 0 );
}

//----------------------------------------------------------------------------------------------------------------------

static s2_t compare_u1_t( const bcore_compare_s* p, const u1_t* o1, const u1_t* o2 )
{
    return o1 ? ( o2 ? ( ( *o1 < *o2 ) ? 1 : ( ( *o1 > *o2 ) ? -1 : 0 ) ) : -1 ) : ( o2 ? 1 : 0 );
}

//----------------------------------------------------------------------------------------------------------------------

static s2_t compare_u0_t( const bcore_compare_s* p, const u0_t* o1, const u0_t* o2 )
{
    return o1 ? ( o2 ? ( ( *o1 < *o2 ) ? 1 : ( ( *o1 > *o2 ) ? -1 : 0 ) ) : -1 ) : ( o2 ? 1 : 0 );
}

//----------------------------------------------------------------------------------------------------------------------

static s2_t compare_f3_t( const bcore_compare_s* p, const f3_t* o1, const f3_t* o2 )
{
    return o1 ? ( o2 ? ( ( *o1 < *o2 ) ? 1 : ( ( *o1 > *o2 ) ? -1 : 0 ) ) : -1 ) : ( o2 ? 1 : 0 );
}

//----------------------------------------------------------------------------------------------------------------------

static s2_t compare_f2_t( const bcore_compare_s* p, const f2_t* o1, const f2_t* o2 )
{
    return o1 ? ( o2 ? ( ( *o1 < *o2 ) ? 1 : ( ( *o1 > *o2 ) ? -1 : 0 ) ) : -1 ) : ( o2 ? 1 : 0 );
}

//----------------------------------------------------------------------------------------------------------------------

static s2_t compare_sz_t( const bcore_compare_s* p, const sz_t* o1, const sz_t* o2 )
{
    return o1 ? ( o2 ? ( ( *o1 < *o2 ) ? 1 : ( ( *o1 > *o2 ) ? -1 : 0 ) ) : -1 ) : ( o2 ? 1 : 0 );
}

//----------------------------------------------------------------------------------------------------------------------

static s2_t compare_uz_t( const bcore_compare_s* p, const uz_t* o1, const uz_t* o2 )
{
    return o1 ? ( o2 ? ( ( *o1 < *o2 ) ? 1 : ( ( *o1 > *o2 ) ? -1 : 0 ) ) : -1 ) : ( o2 ? 1 : 0 );
}

//----------------------------------------------------------------------------------------------------------------------

static s2_t compare_tp_t( const bcore_compare_s* p, const tp_t* o1, const tp_t* o2 )
{
    return o1 ? ( o2 ? ( ( *o1 < *o2 ) ? 1 : ( ( *o1 > *o2 ) ? -1 : 0 ) ) : -1 ) : ( o2 ? 1 : 0 );
}

//----------------------------------------------------------------------------------------------------------------------

static s2_t compare_o( const bcore_compare_s* p, vc_t obj1, vc_t obj2 )
{
    return ( ( bcore_fp_compare )p->fp_compare )( obj1, obj2 );
}

//----------------------------------------------------------------------------------------------------------------------

static s2_t compare_leaf_or_string( tp_t type, vc_t obj1, vc_t obj2 )
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
        case TYPEOF_uz_t: return ( *( uz_t* )obj1 == *( uz_t* )obj2 ) ? 0 : ( *( uz_t* )obj1 < *( uz_t* )obj2 ) ? 1 : -1;
        case TYPEOF_sd_t: return bcore_strcmp( *( sd_t* )obj1, *( sd_t* )obj2 );
        case TYPEOF_sc_t: return bcore_strcmp( *( sc_t* )obj1, *( sc_t* )obj2 );
        case TYPEOF_st_s: return bcore_strcmp( ( ( st_s* )obj1 )->sc, ( ( st_s* )obj2 )->sc );
        case TYPEOF_tp_t: return ( *( tp_t* )obj1 == *( tp_t* )obj2 ) ? 0 : ( *( tp_t* )obj1 < *( tp_t* )obj2 ) ? 1 : -1;
        case TYPEOF_bl_t: return ( *( bl_t* )obj1 == *( bl_t* )obj2 ) ? 0 : ( *( bl_t* )obj1 < *( bl_t* )obj2 ) ? 1 : -1;
        case TYPEOF_aware_t: return ( *( aware_t* )obj1 == *( aware_t* )obj2 ) ? 0 : ( *( aware_t* )obj1 < *( aware_t* )obj2 ) ? 1 : -1;
        default: ERR( "Cannot compare instances of leaf type %s.", ifnameof( type ) );
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

static s2_t compare_generic( const bcore_compare_s* p, vc_t obj1, vc_t obj2 )
{
    if( obj1 == obj2 ) return  0;
    if( obj1 == NULL ) return  1;
    if( obj2 == NULL ) return -1;
    if( bcore_via_p_is_leaf( p->via, NULL ) || p->o_type == TYPEOF_st_s )
    {
        return compare_leaf_or_string( p->o_type, obj1, obj2 );
    }

    if( bcore_via_p_is_pure_array( p->via, NULL ) )
    {
        const bcore_array_s* arr_p = bcore_array_s_get_typed( p->via->header.o_type );
        uz_t size1 = bcore_array_p_get_size( arr_p, obj1 );
        uz_t size2 = bcore_array_p_get_size( arr_p, obj2 );
        if( size1 != size2 ) return size1 < size2 ? 1 : -1;
        for( uz_t j = 0; j < size1; j++ )
        {
            s2_t c = bcore_compare_sr( arr_p->get( arr_p, obj1, j ), arr_p->get( arr_p, obj2, j ) );
            if( c != 0 ) return c;
        }
        return 0;
    }

    const bcore_via_s* via = p->via;
    for( uz_t i = 0; i < via->size; i++ )
    {
        s2_t c = bcore_compare_sr( bcore_via_p_iget( via, obj1, i ), bcore_via_p_iget( via, obj2, i ) );
        if( c != 0 ) return c;
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

static s2_t compare_generic_num_dominant( const bcore_compare_s* p, vc_t obj1, vc_t obj2 )
{
    if( obj1 == obj2 ) return  0;
    if( obj1 == NULL ) return  1;
    if( obj2 == NULL ) return -1;
    if( bcore_via_p_is_leaf( p->via, NULL ) || p->o_type == TYPEOF_st_s )
    {
        return compare_leaf_or_string( p->o_type, obj1, obj2 );
    }

    if( bcore_via_p_is_pure_array( p->via, NULL ) )
    {
        const bcore_array_s* arr_p = bcore_array_s_get_typed( p->via->header.o_type );
        uz_t size1 = bcore_array_p_get_size( arr_p, obj1 );
        uz_t size2 = bcore_array_p_get_size( arr_p, obj2 );
        if( size1 != size2 ) return size1 < size2 ? 1 : -1;
        for( uz_t j = 0; j < size1; j++ )
        {
            s2_t c = bcore_compare_num_dominant_sr( arr_p->get( arr_p, obj1, j ), arr_p->get( arr_p, obj2, j ) );
            if( c != 0 ) return c;
        }
        return 0;
    }

    const bcore_via_s* via = p->via;
    for( uz_t i = 0; i < via->size; i++ )
    {
        s2_t c = bcore_compare_num_dominant_sr( bcore_via_p_iget( via, obj1, i ), bcore_via_p_iget( via, obj2, i ) );
        if( c != 0 ) return c;
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

typedef struct { ap_t ap; const bcore_compare_s* p; vc_t obj1; vc_t obj2; s2_t ret; } compare_nc;
static void compare_amoeba( compare_nc* nc ) { compare_generic( nc->p, nc->obj1, nc->obj2 ); }

static s2_t compare_amoebic( const bcore_compare_s* p, vc_t obj1, vc_t obj2 )
{
    compare_nc nc = { ( ap_t )compare_amoeba, p, obj1, obj2, 0 };
    ( ( ap_t )p->fp_compare )( &nc );
    return nc.ret;
}

//----------------------------------------------------------------------------------------------------------------------

static st_s* diff_generic( const bcore_compare_s* p, vc_t obj1, vc_t obj2 )
{
    if( obj1 == obj2 ) return NULL;
    if( obj1 == NULL ) return st_s_createf( "obj1 == NULL, obj2!%s", ifnameof( p->o_type ) );
    if( obj2 == NULL ) return st_s_createf( "obj1!%s, obj2 == NULL", ifnameof( p->o_type ) );
    if( bcore_via_p_is_leaf( p->via, NULL ) || p->o_type == TYPEOF_st_s )
    {
        s2_t c = compare_leaf_or_string( p->o_type, obj1, obj2 );
        if( c != 0 )
        {
            st_s* s = st_s_create();
            st_s_pushf( s, "!%s: obj1:", ifnameof( p->o_type ) );
            st_s_push_typed( s, p->o_type, obj1 );
            st_s_pushf( s, "obj2:", ifnameof( p->o_type ) );
            st_s_push_typed( s, p->o_type, obj2 );
            return s;
        }
        return NULL;
    }

    if( bcore_via_p_is_pure_array( p->via, NULL ) )
    {
        const bcore_array_s* arr_p = bcore_array_s_get_typed( p->via->header.o_type );
        uz_t size1 = bcore_array_p_get_size( arr_p, obj1 );
        uz_t size2 = bcore_array_p_get_size( arr_p, obj2 );
        if( size1 != size2 ) return st_s_createf( "!%s: size1:%zu, size2:%zu", ifnameof( p->o_type ), size1, size2 );
        for( uz_t j = 0; j < size1; j++ )
        {
            st_s* s = bcore_diff_sr( arr_p->get( arr_p, obj1, j ), arr_p->get( arr_p, obj2, j ) );
            if( s != NULL )
            {
                st_s_pushf( s, "\n!%s:[%zu]",ifnameof( p->o_type ), j );
                return s;
            }
        }
        return NULL;
    }

    const bcore_via_s* via = p->via;
    for( uz_t i = 0; i < via->size; i++ )
    {
        st_s* s = bcore_diff_sr( bcore_via_p_iget( via, obj1, i ), bcore_via_p_iget( via, obj2, i ) );
        if( s != NULL )
        {
            return st_s_pushf( s, "\n!%s::%s", ifnameof( p->o_type ), ifnameof( bcore_via_p_iget_name( via, NULL, i ) ) );
        }
    }
    return NULL;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

static void compare_s_define_trait()
{
    bcore_trait_set( entypeof( "bcore_compare" ), entypeof( "bcore_inst" ) );
}

//----------------------------------------------------------------------------------------------------------------------

static bcore_compare_s* create_from_self( const bcore_self_s* self )
{
    assert( self != NULL );

    bcore_compare_s* o = compare_s_create();
    o->o_type  = self->type;
    o->via     = bcore_via_s_get_typed( o->o_type );
    fp_t cmp_a =                     bcore_self_s_try_external_fp( self, typeof( "ap_t" ), typeof( "compare" ) );
    fp_t cmp_o = ( cmp_a ) ? cmp_a : bcore_self_s_try_external_fp( self, typeof( "bcore_fp_compare" ), 0 );

    o->fp_compare = cmp_o;

    typedef s2_t (*fp_icmp)( const bcore_compare_s*, vc_t, vc_t );

    switch( o->o_type )
    {
        case TYPEOF_s3_t: o->compare = ( fp_icmp )compare_s3_t; break;
        case TYPEOF_s2_t: o->compare = ( fp_icmp )compare_s2_t; break;
        case TYPEOF_s1_t: o->compare = ( fp_icmp )compare_s1_t; break;
        case TYPEOF_s0_t: o->compare = ( fp_icmp )compare_s0_t; break;
        case TYPEOF_u3_t: o->compare = ( fp_icmp )compare_u3_t; break;
        case TYPEOF_u2_t: o->compare = ( fp_icmp )compare_u2_t; break;
        case TYPEOF_u1_t: o->compare = ( fp_icmp )compare_u1_t; break;
        case TYPEOF_u0_t: o->compare = ( fp_icmp )compare_u0_t; break;
        case TYPEOF_f3_t: o->compare = ( fp_icmp )compare_f3_t; break;
        case TYPEOF_f2_t: o->compare = ( fp_icmp )compare_f2_t; break;
        case TYPEOF_sz_t: o->compare = ( fp_icmp )compare_sz_t; break;
        case TYPEOF_uz_t: o->compare = ( fp_icmp )compare_uz_t; break;
        case TYPEOF_tp_t: o->compare = ( fp_icmp )compare_tp_t; break;
        default:
        {
            o->compare = cmp_a ? compare_amoebic : ( cmp_o ? compare_o : compare_generic );
        }
        break;
    }

    return o;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

static bcore_self_s* compare_s_create_self( void )
{
    sc_t def = "bcore_compare_s = spect { aware_t p_type; tp_t o_type; ... }";
    bcore_self_s* self = BCORE_SELF_S_BUILD_PARSE_SC( def, bcore_compare_s );
    bcore_self_s_push_ns_func( self, ( fp_t )compare_s_init,             "bcore_fp_init",                    "init"         );
    bcore_self_s_push_ns_func( self, ( fp_t )compare_s_down,             "bcore_fp_down",                    "down"         );
    bcore_self_s_push_ns_func( self, ( fp_t )compare_s_create,           "bcore_fp_create",                  "create"       );
    bcore_self_s_push_ns_func( self, ( fp_t )compare_s_discard,          "bcore_fp_discard",                 "discard"      );
    bcore_self_s_push_ns_func( self, ( fp_t )create_from_self,           "bcore_spect_fp_create_from_self",  "create_from_self" );
    return self;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

s2_t bcore_compare_spect( const bcore_compare_s* p, vc_t obj1, vc_t obj2 )
{
    return p->compare( p, obj1, obj2 );
}

//----------------------------------------------------------------------------------------------------------------------

s2_t bcore_compare_num_dominant_spect( const bcore_compare_s* p, vc_t obj1, vc_t obj2 )
{
    return compare_generic_num_dominant( p, obj1, obj2 );
}

//----------------------------------------------------------------------------------------------------------------------

s2_t bcore_compare_typed( tp_t type, vc_t obj1, vc_t obj2 )
{
    return bcore_compare_spect( bcore_compare_s_get_typed( type ), obj1, obj2 );
}

//----------------------------------------------------------------------------------------------------------------------

s2_t bcore_compare_num_dominant_typed( tp_t type, vc_t obj1, vc_t obj2 )
{
    return bcore_compare_num_dominant_spect( bcore_compare_s_get_typed( type ), obj1, obj2 );
}

//----------------------------------------------------------------------------------------------------------------------

s2_t bcore_compare_sr( sr_s obj1, sr_s obj2 )
{
    if( sr_s_type( &obj1 ) != sr_s_type( &obj2 ) ) return ( sr_s_type( &obj1 ) < sr_s_type( &obj2 ) ) ? 1 : -1;
    if( sr_s_type( &obj1 ) == 0 ) return 0;
    s2_t c = bcore_compare_typed( sr_s_type( &obj1 ), obj1.o, obj2.o );
    sr_down( obj1 );
    sr_down( obj2 );
    return c;
}

//----------------------------------------------------------------------------------------------------------------------

s2_t bcore_compare_num_dominant_sr( sr_s sr_a, sr_s sr_b )
{
    if( sr_s_is_numeric( &sr_a ) && sr_s_is_numeric( &sr_b ) && sr_a.o && sr_b.o )
    {
        s2_t c = 0;
        if( sr_s_is_float( &sr_a ) || sr_s_is_float( &sr_b ) )
        {
            f3_t fa = sr_s_to_f3( &sr_a );
            f3_t fb = sr_s_to_f3( &sr_b );
            c = ( fa == fb ) ? 0 : ( fa < fb ) ? 1 : -1;
        }
        else if( sr_s_is_unsigned( &sr_a ) )
        {
            u3_t ua = sr_s_to_u3( &sr_a );
            if( sr_s_is_unsigned( &sr_b ) )
            {
                u3_t ub = sr_s_to_u3( &sr_b );
                c = ( ua == ub ) ? 0 : ( ua < ub ) ? 1 : -1;
            }
            else
            {
                s3_t sb = sr_s_to_s3( &sr_b );
                c = ( ua == sb ) ? 0 : ( ua < sb ) ? 1 : -1;
            }
        }
        else
        {
            s3_t sa = sr_s_to_s3( &sr_a );
            if( sr_s_is_unsigned( &sr_b ) )
            {
                u3_t ub = sr_s_to_u3( &sr_b );
                c = ( sa == ub ) ? 0 : ( sa < ub ) ? 1 : -1;
            }
            else
            {
                s3_t sb = sr_s_to_s3( &sr_b );
                c = ( sa == sb ) ? 0 : ( sa < sb ) ? 1 : -1;
            }
        }
        sr_down( sr_a );
        sr_down( sr_b );
        return c;
    }
    else
    {
        if( sr_s_type( &sr_a ) != sr_s_type( &sr_b ) ) return ( sr_s_type( &sr_a ) < sr_s_type( &sr_b ) ) ? 1 : -1;
        if( sr_s_type( &sr_a ) == 0 ) return 0;
        s2_t c = bcore_compare_num_dominant_typed( sr_s_type( &sr_a ), sr_a.o, sr_b.o );
        sr_down( sr_a );
        sr_down( sr_b );
        return c;
    }
}

//----------------------------------------------------------------------------------------------------------------------

s2_t bcore_compare_q_sr( const sr_s* obj1, const sr_s* obj2 )
{
    if( sr_s_type( obj1 ) != sr_s_type( obj2 ) ) return ( sr_s_type( obj1 ) < sr_s_type( obj2 ) ) ? 1 : -1;
    if( sr_s_type( obj1 ) == 0 ) return 0;
    return bcore_compare_typed( sr_s_type( obj1 ), obj1->o, obj2->o );
}

//----------------------------------------------------------------------------------------------------------------------

s2_t bcore_compare_bityped( tp_t type1, vc_t obj1, tp_t type2, vc_t obj2 )
{
    if( type1 != type2 ) return ( type1 < type2 ) ? 1 : -1;
    if( type1 == 0     ) return 0;
    return bcore_compare_typed( type1, obj1, obj2 );
}

//----------------------------------------------------------------------------------------------------------------------

bool bcore_equal_spect( const bcore_compare_s* p, vc_t obj1, vc_t obj2 )
{
    return bcore_compare_spect( p, obj1, obj2 ) == 0;
}

//----------------------------------------------------------------------------------------------------------------------

bool bcore_equal_typed( tp_t type, vc_t obj1, vc_t obj2 )
{
    return bcore_compare_typed( type, obj1, obj2 ) == 0;
}

//----------------------------------------------------------------------------------------------------------------------

bool bcore_equal_bityped( tp_t type1, vc_t obj1, tp_t type2, vc_t obj2 )
{
    return bcore_compare_bityped( type1, obj1, type2, obj2 ) == 0;
}

//----------------------------------------------------------------------------------------------------------------------

s2_t bcore_compare_aware( vc_t obj1, vc_t obj2 )
{
    tp_t type1 = obj1 ? *( aware_t* )obj1 : 0;
    tp_t type2 = obj2 ? *( aware_t* )obj2 : 0;
    return bcore_compare_bityped( type1, obj1, type2, obj2 );
}

//----------------------------------------------------------------------------------------------------------------------

bool bcore_equal_aware( vc_t obj1, vc_t obj2 )
{
    return bcore_compare_aware( obj1, obj2 ) == 0;
}

//----------------------------------------------------------------------------------------------------------------------

st_s* bcore_diff_spect( const bcore_compare_s* p, vc_t obj1, vc_t obj2 )
{
    if( p->compare == compare_generic )
    {
        return diff_generic( p, obj1, obj2 );
    }
    else
    {
        if( bcore_compare_spect( p, obj1, obj2 ) != 0 )
        {
            st_s* s = st_s_createf( "!%s: objects overloading comparison differ:", ifnameof( p->o_type ) );
            st_s_pushf( s, "\n\tobj1: " );
            bcore_translate_x( bcore_inst_t_create_sr( typeof( "bcore_txt_ml_translator_s" ) ), sr_twc( p->o_type, obj1 ), sr_awm( s ) );
            st_s_pushf( s, "\tobj2: " );
            bcore_translate_x( bcore_inst_t_create_sr( typeof( "bcore_txt_ml_translator_s" ) ), sr_twc( p->o_type, obj2 ), sr_awm( s ) );
            return s;
        }
    }
    return NULL;
}

//----------------------------------------------------------------------------------------------------------------------

st_s* bcore_diff_typed( tp_t type, vc_t obj1, vc_t obj2 )
{
    return bcore_diff_spect( bcore_compare_s_get_typed( type ), obj1, obj2 );
}

//----------------------------------------------------------------------------------------------------------------------

st_s* bcore_diff_bityped( tp_t type1, vc_t obj1, tp_t type2, vc_t obj2 )
{
    if( type1 != type2 ) return st_s_createf( "obj1 of '%s', obj2 of '%s'", ifnameof( type1 ), ifnameof( type2 ) );
    if( type1 == 0 ) return NULL;
    return bcore_diff_typed( type1, obj1, obj2 );
}

//----------------------------------------------------------------------------------------------------------------------

st_s* bcore_diff_aware( vc_t obj1, vc_t obj2 )
{
    tp_t type1 = obj1 ? *( aware_t* )obj1 : 0;
    tp_t type2 = obj2 ? *( aware_t* )obj2 : 0;
    return bcore_diff_bityped( type1, obj1, type2, obj2 );
}

//----------------------------------------------------------------------------------------------------------------------

st_s* bcore_diff_sr( sr_s obj1, sr_s obj2 )
{
    if( sr_s_type( &obj1 ) != sr_s_type( &obj2 ) ) return st_s_createf( "obj1 of '%s', obj2 of '%s'", ifnameof( sr_s_type( &obj1 ) ), ifnameof( sr_s_type( &obj2 ) ) );
    if( sr_s_type( &obj1 ) == 0 ) return NULL;
    st_s* s = bcore_diff_typed( sr_s_type( &obj1 ), obj1.o, obj2.o );
    sr_down( obj1 );
    sr_down( obj2 );
    return s;
}

//----------------------------------------------------------------------------------------------------------------------

st_s* bcore_diff_q_sr( const sr_s* obj1, const sr_s* obj2 )
{
    if( sr_s_type( obj1 ) != sr_s_type( obj2 ) ) return st_s_createf( "obj1 of '%s', obj2 of '%s'", ifnameof( sr_s_type( obj1 ) ), ifnameof( sr_s_type( obj2 ) ) );
    if( sr_s_type( obj1 ) == 0 ) return NULL;
    return bcore_diff_typed( sr_s_type( obj1 ), obj1->o, obj2->o );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// testing

//----------------------------------------------------------------------------------------------------------------------

static st_s* spect_compare_selftest( void )
{
    bcore_life_s* l = bcore_life_s_create();

    bcore_flect_define_parse_sc( "cmp_specs = { aware_t _; uz_t size; u2_t param1; s2_t; }" );
    vd_t specs = bcore_life_s_push_aware( l, bcore_inst_t_create( typeof( "cmp_specs" ) ) );

    {
        const bcore_via_s* v = bcore_via_s_get_aware( specs );
        bcore_via_p_nset_uz( v, specs, typeof( "size"   ), 10  );
        bcore_via_p_nset_u3( v, specs, typeof( "param1" ), 200 );
        bcore_via_p_nset_s3( v, specs, typeof( ""       ), -50 );
    }

    bcore_flect_define_parse_sc( "cmp_specs_arr = { aware_t _; u3_t flags; cmp_specs [] arr; }" );
    vd_t arr1 = bcore_life_s_push_aware( l, bcore_inst_t_create( typeof( "cmp_specs_arr" ) ) );

    const bcore_array_s* arr_p = bcore_array_s_get_aware( arr1 );

    for( uz_t i = 0; i < 100; i++ ) bcore_array_p_push( arr_p, arr1, sr_awc( specs ) );

    for( uz_t i = 0; i < bcore_array_p_get_size( arr_p, arr1 ); i++ )
    {
        vd_t specs = arr_p->get( arr_p, arr1, i ).o;
        const bcore_via_s* v = bcore_via_s_get_aware( specs );
        bcore_via_p_nset_uz( v, specs, typeof( "size" ), i );
    }

    const bcore_via_s* v = bcore_via_s_get_aware( specs );
    vd_t arr2 = bcore_life_s_push_aware( l, bcore_inst_a_clone( arr1 ) );

    ASSERT( bcore_compare_aware( arr1, arr2 ) == 0 );
    bcore_via_p_nset_uz( v, arr_p->get( arr_p, arr2, 5 ).o, typeof( "size" ), 20 );
    ASSERT( bcore_compare_aware( arr1, arr2 ) >  0 );
    bcore_via_p_nset_uz( v, arr_p->get( arr_p, arr2, 5 ).o, typeof( "size" ), 0 );
    ASSERT( bcore_compare_aware( arr1, arr2 ) <  0 );

    bcore_life_s_discard( l );
    return NULL;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// signal

//----------------------------------------------------------------------------------------------------------------------

BCORE_DEFINE_SPECT_CACHE( bcore_compare_s );

vd_t bcore_spect_compare_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_spect_compare" ) ) )
    {
        case TYPEOF_init1:
        {
            compare_s_define_trait();
            bcore_flect_define_creator( typeof( "bcore_compare_s"  ), compare_s_create_self  );
            bcore_spect_setup_cache( &bcore_compare_s_cache_g );
        }
        break;

        case TYPEOF_selftest:
        {
            return spect_compare_selftest();
        }
        break;

        default: break;
    }

    return NULL;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
