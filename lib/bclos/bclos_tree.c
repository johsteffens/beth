/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bclos_tree.h"

#include "bcore_spect_inst.h"
#include "bcore_spect_via.h"
#include "bcore_spect_array.h"
#include "bcore_life.h"

typedef struct bclos_tree_s
{
    aware_t _;
    vd_t closure_o;
    vc_t closure_p;
    bclos_tree_s** branch;
    sz_t size, space; // size specifies the number of arguments expected by closure; a NULL branch consumes the specific argument from the args list
} bclos_tree_s;

void bclos_tree_s_init( bclos_tree_s* o )
{
    bcore_inst_typed_init( typeof( "bclos_tree_s" ), o );
}

void bclos_tree_s_down( bclos_tree_s* o )
{
    bcore_inst_typed_down( o->_, o );
}

static void tree_copy_a( vd_t nc )
{
    struct { ap_t a; vc_t p; bclos_tree_s* dst; const bclos_tree_s* src; } * nc_l = nc;
    nc_l->a( nc ); // default
    nc_l->dst->closure_p = nc_l->src->closure_p;
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
    return bcore_inst_typed_create( typeof( "bclos_tree_s" ) );
}

bclos_tree_s* bclos_tree_s_create_d( vd_t closure )
{
    bclos_tree_s* o = bclos_tree_s_create();
    bclos_tree_s_set_closure_d( o, closure );
    return o;
}

bclos_tree_s* bclos_tree_s_create_typed( tp_t closure_type )
{
    return bclos_tree_s_create_d( bcore_inst_typed_create( closure_type ) );
}

bclos_tree_s* bclos_tree_s_clone( const bclos_tree_s* o )
{
    return o ? bcore_inst_typed_clone( o->_, o ) : NULL;
}

static void tree_interpret_body_a( vd_t nc )
{
    struct { ap_t a; vc_t p; vc_t inter; vd_t sink; tp_t type; bclos_tree_s* obj; } * nc_l = nc;
    bclos_tree_s* obj = nc_l->obj;
    obj->closure_p = NULL;
    nc_l->a( nc ); // default
    if( obj->closure_o ) obj->closure_p = bcore_closure_s_get_aware( obj->closure_o );
}

/**********************************************************************************************************************/
// bclos_tree_s features

bclos_tree_s* bclos_tree_s_set_closure_d( bclos_tree_s* o, vd_t closure )
{
    bcore_inst_aware_discard( o->closure_o );
    o->closure_o = closure;
    o->closure_p = bcore_closure_s_get_aware( o->closure_o );
    const bcore_array_s* arr_p = bcore_array_s_get_aware( o );
    arr_p->set_size( arr_p, o, bcore_closure_spect_n_args( o->closure_p, o->closure_o ) );
    return o;
}

bclos_tree_s* bclos_tree_s_set_closure_typed( bclos_tree_s* o, tp_t type )
{
    return bclos_tree_s_set_closure_d( o, bcore_inst_typed_create( type ) );
}

bclos_tree_s* bclos_tree_s_set_branch_d( bclos_tree_s* o, sz_t index, vd_t closure )
{
    if( index >= o->size ) ERR( "indexing (%zu) array of size (%zu)", index, o->size );
    bcore_inst_aware_discard( o->branch[ index ] );

    if( ( *( aware_t* )closure ) == o->_ )
    {
        o->branch[ index ] = closure;
    }
    else
    {
        o->branch[ index ] = bclos_tree_s_create_d( closure );
    }
    return o->branch[ index ];
}

bclos_tree_s* bclos_tree_s_set_branch_typed(  bclos_tree_s* o, sz_t index, tp_t type )
{
    return bclos_tree_s_set_branch_d( o, index, bcore_inst_typed_create( type ) );
}

/**********************************************************************************************************************/
// closure features

static vd_t func( bclos_tree_s* o, vc_t** p_args, sz_t* p_n_args )
{
    vc_t* args_l = bcore_un_alloc( sizeof( vc_t ), NULL, 0, o->size, NULL );
    bcore_life_s* l = bcore_life_s_create();
    for( sz_t i = 0; i < o->size; i++ )
    {
        if( o->branch[ i ] )
        {
            bclos_tree_s* tree = o->branch[ i ];
            args_l[ i ] = func( tree, p_args, p_n_args );
            bcore_life_s_push_typed( l, bclos_tree_s_t_ret( tree ), ( vd_t )args_l[ i ] );
        }
        else
        {
            if( *p_n_args == 0 )
            {
                ERR( "%zu arguments expected", bclos_tree_s_n_args( o ) );
            }
            args_l[ i ] = *( *p_args )++;
            ( *p_n_args )--;
        }
    }
    vd_t ret = bcore_closure_spect_func( o->closure_p, o->closure_o, args_l, o->size );
    bcore_life_s_discard( l );
    bcore_un_alloc( sizeof( vc_t ), args_l, o->size, 0, NULL );
    return ret;
}

tp_t t_arg( const bclos_tree_s* o, sz_t* p_index )
{
    for( sz_t i = 0; i < o->size; i++ )
    {
        if( o->branch[ i ] )
        {
            tp_t type = t_arg( o->branch[ i ], p_index );
            if( *p_index == 0 ) return type;
        }
        else
        {
            if( *p_index == 0 ) return bcore_closure_spect_t_arg( o->closure_p, o->closure_o, i );
            ( *p_index )--;
        }
    }
    return 0;
}

vd_t bclos_tree_s_func( bclos_tree_s* o, vc_t* args, sz_t n_args )
{
    return func( o, &args, &n_args );
}

sz_t bclos_tree_s_n_args( const bclos_tree_s* o )
{
    sz_t n_args = 0;
    for( sz_t i = 0; i < o->size; i++ )
    {
        if( o->branch[ i ] )
        {
            n_args += bclos_tree_s_n_args( o->branch[ i ] );
        }
        else
        {
            n_args++;
        }
    }
    return n_args;
}

tp_t bclos_tree_s_t_arg( const bclos_tree_s* o, sz_t index )
{
    return t_arg( o, &index );
}

tp_t bclos_tree_s_t_ret( const bclos_tree_s* o )
{
    return bcore_closure_spect_t_ret( o->closure_p, o->closure_o );
}

bcore_flect_self_s* bclos_tree_s_create_self( void )
{
    sc_t def =
    "bclos_tree_s = "
    "{ "
      "aware_t _; "
      "aware * closure_o; "
      "external void * closure_p;"
      "bclos_tree_s * [] branch; "
    "}";

    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bclos_tree_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )tree_copy_a, "ap_t", "copy" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )tree_interpret_body_a, "ap_t", "interpret_body" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bclos_tree_s_func,   "bcore_closure_fp_func",   "func" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bclos_tree_s_n_args, "bcore_closure_fp_n_args", "n_args" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bclos_tree_s_t_arg,  "bcore_closure_fp_t_arg", "t_arg" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bclos_tree_s_t_ret,  "bcore_closure_fp_t_ret", "t_ret" );

    return self;
}

/**********************************************************************************************************************/

bcore_string_s* bclos_tree_selftest()
{
    bcore_string_s* log = bcore_string_s_create();
    bcore_life_s* l = bcore_life_s_create();

    bclos_tree_s* tree = bcore_life_s_push_aware( l, bclos_tree_s_create_typed( typeof( "bclos_f3_mul" ) ) );
    bclos_tree_s_set_branch_typed( bclos_tree_s_set_branch_typed( tree, 0, typeof( "bclos_f3_add" ) ), 1, typeof( "bclos_f3_mul" ) );
    bclos_tree_s_set_branch_typed( tree, 1, typeof( "bclos_f3_add" ) );

    f3_t v1 = 5.0;
    f3_t v2 = 6.0;
    f3_t v3 = 3.0;
    f3_t v4 = 7.0;
    f3_t v5 = 8.0;
    const vc_t args[] = { &v1, &v2, &v3, &v4, &v5 };

    ASSERT( ( v1 + ( v2 * v3 ) ) * ( v4 + v5 ) == *( f3_t* )bcore_life_s_push_typed( l, typeof( "f3_t" ), bcore_closure_aware_func( tree, args, 5 ) ) );

    bcore_life_s_discard( l );
    return log;
}
