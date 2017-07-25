/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bclos_tree.h"

#include "bcore_spect_inst.h"
#include "bcore_spect_via.h"
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

static vd_t func( bclos_tree_s* o, bcore_life_s* life, sz_t* p_size, vc_t** p_args )
{
    vc_t* args_l = bcore_un_alloc( sizeof( vc_t ), NULL, 0, o->size, NULL );
    bcore_life_s* life_l = bcore_life_s_create();

    for( sz_t i = 0; i < o->size; i++ )
    {
        if( o->branch[ i ] )
        {
            args_l[ i ] = func( o->branch[ i ], life_l, p_size, p_args );
        }
        else
        {
            if( *p_size == 0 ) ERR( "Insufficient amount of arguments." );
            args_l[ i ] = *( *p_args )++;
            ( *p_size )--;
        }
    }
    vd_t ret = bcore_closure_spect_func( o->closure_p, o, args_l, o->size );
    bcore_life_s_discard( life_l );
    bcore_un_alloc( sizeof( vc_t ), args_l, o->size, 0, NULL );

    if( life ) bcore_life_s_push_typed( life, bcore_closure_spect_tp_ret( o->closure_p, o ), ret );
    return ret;
}

static vd_t tree_func( bclos_tree_s* o, sz_t size, vc_t* args )
{
    return func( o, NULL, &size, &args );
}

static tp_t tree_tp_ret( const bclos_tree_s* o )
{
    return ( o->closure_o ) ? bcore_closure_spect_tp_ret( o->closure_p, o->closure_o ) : 0;
}

static sz_t tree_n_args( const bclos_tree_s* o )
{
    sz_t n_args = 0;
    for( sz_t i = 0; i < o->size; i++ )
    {
        if( o->branch[ i ] )
        {
            n_args += tree_n_args( o->branch[ i ] );
        }
        else
        {
            n_args++;
        }
    }
    return n_args;
}

static tp_t tree_tp_arg( const bclos_tree_s* o, sz_t index )
{
    for( sz_t i = 0; i < o->size; i++ )
    {
        if( o->branch[ i ] )
        {
            tp_t tp_arg = tree_tp_arg( o->branch[ i ], index );
            if( tp_arg != 0 ) return tp_arg;
        }
        else
        {
            if( index == 0 ) return bcore_closure_spect_tp_arg( o->closure_p, o->closure_o, i );
            index--;
        }
    }
    return 0;
}

const bcore_flect_self_s* bclos_tree_s_create_self()
{
    sc_t def =
    "bclos_tree_s = "
    "{ "
      "aware_t _; "
      "aware * closure_o; "
      "external vd_t closure_p;"
      "bclos_tree_s * [] branch; "
    "}";

    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bclos_tree_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )tree_copy_a, "ap_t", "copy" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )tree_interpret_body_a, "ap_t", "interpret_body" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )tree_func,   "bcore_closure_fp_func", "func" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )tree_n_args, "bcore_closure_fp_n_args", "n_args" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )tree_tp_ret, "bcore_closure_fp_tp_ret", "tp_ret" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )tree_tp_arg, "bcore_closure_fp_tp_arg", "tp_arg" );

    return self;
}
