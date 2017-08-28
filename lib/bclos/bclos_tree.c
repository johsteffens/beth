/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bclos_tree.h"

#include "bcore_spect_inst.h"
#include "bcore_spect_via.h"
#include "bcore_spect_array.h"
#include "bcore_life.h"
#include "bcore_spect.h"
#include "bcore_quicktypes.h"
#include "bcore_txt_ml.h"
#include "bclos_quicktypes.h"

typedef struct bclos_tree_s
{
    aware_t _;
    bcore_closure_r closure;

    union
    {
        bcore_static_link_array_s arr;
        struct
        {
            bcore_closure_r* branch;
            sz_t size, space; // size specifies the number of arguments expected by closure; a NULL branch consumes the specific argument from the args list
        };
    };
} bclos_tree_s;

void bclos_tree_s_init( bclos_tree_s* o )
{
    bcore_inst_typed_init( TYPEOF_bclos_tree_s, o );
}

void bclos_tree_s_down( bclos_tree_s* o )
{
    bcore_inst_typed_down( o->_, o );
}

void bclos_tree_s_copy( bclos_tree_s* o, const bclos_tree_s* src )
{
    bcore_inst_typed_copy( o->_, o, src );
}

void bclos_tree_s_discard( bclos_tree_s* o )
{
    if( o ) bcore_inst_typed_discard( o->_, o );
}

bclos_tree_s* bclos_tree_s_create()
{
    return bcore_inst_typed_create( TYPEOF_bclos_tree_s );
}

bclos_tree_s* bclos_tree_s_clone( const bclos_tree_s* o )
{
    return o ? bcore_inst_typed_clone( o->_, o ) : NULL;
}

/**********************************************************************************************************************/
// bclos_tree_s features

void bclos_tree_s_set_closure( bclos_tree_s* o, sr_s closure )
{
    bcore_closure_r_set( &o->closure, closure );
    bcore_array_spect_set_size( bcore_array_s_get_typed( TYPEOF_bclos_tree_s ), o, bcore_closure_r_n_args( &o->closure ) );
}

ad_s bclos_tree_s_d_arg( const bclos_tree_s* o, sz_t index );

void bclos_tree_s_set_branch( bclos_tree_s* o, sz_t index, sr_s closure )
{
    if( index >= o->size ) ERR( "index (%zu) to array of size (%zu)", index, o->size );
    ad_s d = bclos_tree_s_d_arg( o, index );
    if( d.flags == AD_F_WR ) ERR( "Closure '%s' modifies argument at index '%zu'.", index );
    tp_t t_ret = bcore_closure_t_ret( sr_cw( closure ) );
    if( d.type != t_ret )
    {
        ERR( "Type mismatch at index '%zu': '%s' expected but '%s' returned by closure '%s'.",
             index,
             ifnameof( d.type ),
             ifnameof( t_ret ),
             ifnameof( sr_type( closure ) ) );
    }
    bcore_closure_r_set( &o->branch[ index ], closure );
}

bcore_closure_r bclos_tree_s_get_closure( bclos_tree_s* o )
{
    return o->closure;
}

bcore_closure_r bclos_tree_s_get_branch( bclos_tree_s* o, sz_t index )
{
    assert( index < o->size );
    return o->branch[ index ];
}

/**********************************************************************************************************************/
// closure features

static vd_t func( const bclos_tree_s* o, vd_t** p_args, sz_t* p_n_args )
{
    vd_t* args_l = bcore_un_alloc( sizeof( vc_t ), NULL, 0, o->size, NULL );
    bcore_life_s* l = bcore_life_s_create();
    for( sz_t i = 0; i < o->size; i++ )
    {
        if( o->branch[ i ].o )
        {
            bcore_closure_r r = o->branch[ i ];
            if( spect_tp_o( r.p ) == TYPEOF_bclos_tree_s )
            {
                args_l[ i ] = func( r.o, p_args, p_n_args );
            }
            else
            {
                sz_t n_args = bcore_closure_r_n_args( &r );
                if( *p_n_args < n_args ) ERR( "%zu arguments expected", bclos_tree_s_n_args( o ) );
                args_l[ i ] = bcore_closure_r_func( &r, *p_args, *p_n_args );
                *p_args   += n_args;
                *p_n_args -= n_args;
            }
            bcore_life_s_push_typed( l, bcore_closure_r_t_ret( &r ), ( vd_t )args_l[ i ] );
        }
        else
        {
            if( *p_n_args == 0 ) ERR( "%zu arguments expected", bclos_tree_s_n_args( o ) );
            args_l[ i ] = *( *p_args )++;
            ( *p_n_args )--;
        }
    }
    vd_t ret = bcore_closure_r_func( &o->closure, args_l, o->size );
    bcore_life_s_discard( l );
    bcore_un_alloc( sizeof( vc_t ), args_l, o->size, 0, NULL );
    return ret;
}

ad_s d_arg( const bclos_tree_s* o, sz_t* p_index )
{
    ad_s ad = { .type = 0, .flags = 0 };
    for( sz_t i = 0; i < o->size; i++ )
    {
        if( o->branch[ i ].o )
        {
            bcore_closure_r r = o->branch[ i ];
            if( spect_tp_o( r.p ) == TYPEOF_bclos_tree_s )
            {
                ad = d_arg( r.o, p_index );
                if( *p_index == 0 ) return ad;
            }
            else
            {
                sz_t n_args = bcore_closure_r_n_args( &r );
                if( *p_index < n_args ) return bcore_closure_r_d_arg( &r, *p_index );
                *p_index -= n_args;
            }
        }
        else
        {
            if( *p_index == 0 ) return bcore_closure_r_d_arg( &o->closure, i );
            ( *p_index )--;
        }
    }
    return ad;
}

vd_t bclos_tree_s_func( const bclos_tree_s* o, vd_t* args, sz_t n_args )
{
    return func( o, &args, &n_args );
}

sz_t bclos_tree_s_n_args( const bclos_tree_s* o )
{
    sz_t n_args = 0;
    for( sz_t i = 0; i < o->size; i++ )
    {
        if( o->branch[ i ].o )
        {
            n_args += bcore_closure_r_n_args( &o->branch[ i ] );
        }
        else
        {
            n_args++;
        }
    }
    return n_args;
}

ad_s bclos_tree_s_d_arg( const bclos_tree_s* o, sz_t index )
{
    return d_arg( o, &index );
}

tp_t bclos_tree_s_t_ret( const bclos_tree_s* o )
{
    return bcore_closure_r_t_ret( &o->closure );
}

bcore_flect_self_s* bclos_tree_s_create_self( void )
{
    sc_t def =
    "bclos_tree_s = "
    "{ "
      "aware_t _; "
      "bcore_closure_r closure;"
      "bcore_closure_r [] branch;"
    "}";

    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bclos_tree_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bclos_tree_s_func,   "bcore_closure_fp_func",   "func" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bclos_tree_s_n_args, "bcore_closure_fp_n_args", "n_args" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bclos_tree_s_d_arg,  "bcore_closure_fp_d_arg",  "d_arg" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bclos_tree_s_t_ret,  "bcore_closure_fp_t_ret",  "t_ret" );

    return self;
}

/**********************************************************************************************************************/

bcore_string_s* bclos_tree_selftest( void )
{
    bcore_life_s* l = bcore_life_s_create();

    sr_s add12 = sr_create( typeof( "bclos_f3_add_f3_f3" ) );
    sr_s mul34 = sr_create( typeof( "bclos_f3_mul_f3_f3" ) );
    sr_s sub56 = sr_create( typeof( "bclos_f3_sub_f3_f3" ) );
    sr_s tsub1 = sr_create( typeof( "bclos_tree_s" ) );
    bclos_tree_s_set_closure( tsub1.o, sr_create( typeof( "bclos_f3_sub_f3_f3" ) ) );
    bclos_tree_s_set_branch(  tsub1.o, 0, add12 );
    bclos_tree_s_set_branch(  tsub1.o, 1, mul34 );

    sr_s troot = bcore_life_s_push_sr( l, sr_create( typeof( "bclos_tree_s" ) ) );
    bclos_tree_s_set_closure( troot.o, sr_create( typeof( "bclos_f3_mul_f3_f3" ) ) );
    bclos_tree_s_set_branch(  troot.o, 0, tsub1 );
    bclos_tree_s_set_branch(  troot.o, 1, sub56 );

    f3_t v1 = 535.0;
    f3_t v2 = 683.0;
    f3_t v3 = 379.0;
    f3_t v4 = 302.0;
    f3_t v5 = 102.0;
    f3_t v6 = 900.0;
    const vd_t args[] = { &v1, &v2, &v3, &v4, &v5, &v6 };

    ASSERT( ( ( v1 + v2 ) - ( v3 * v4 ) ) * ( v5 - v6 ) == *( f3_t* )bcore_life_s_push_typed( l, typeof( "f3_t" ), bcore_closure_func( troot, args, 6 ) ) );

    sr_s tclone = bcore_life_s_push_sr( l, bcore_inst_clone_sr( troot ) );

    ASSERT( ( ( v1 + v2 ) - ( v3 * v4 ) ) * ( v5 - v6 ) == *( f3_t* )bcore_life_s_push_typed( l, typeof( "f3_t" ), bcore_closure_func( tclone, args, 6 ) ) );


    bcore_txt_ml_to_stdout( tclone );

    bcore_life_s_discard( l );
    return NULL;
}

