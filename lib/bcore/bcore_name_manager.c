/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_name_manager.h"
#include "bcore_control.h"
#include "bcore_string.h"
#include "bcore_threads.h"

/**********************************************************************************************************************/

// node of binary tree
typedef struct node_s
{
    u2_t           hash;
    bcore_string_s string;
    struct node_s* child0;
    struct node_s* child1;
} node_s;

static void node_s_init( node_s* o )
{
    o->hash = 0;
    bcore_string_s_init( &o->string );
    o->child0 = NULL;
    o->child1 = NULL;
}

static void node_s_down( node_s* o )
{
    if( o->child0 )
    {
        node_s_down( o->child0 );
        o->child0 = bcore_free( o->child0 );
    }
    if( o->child1 )
    {
        node_s_down( o->child1 );
        o->child1 = bcore_free( o->child1 );
    }
    o->hash = 0;
    bcore_string_s_down( &o->string );
}

static void node_s_insert( node_s* o, u2_t hash, const char* name )
{
    if( hash < o->hash )
    {
        if( o->child0 )
        {
            node_s_insert( o->child0, hash, name );
        }
        else
        {
            o->child0 = bcore_alloc( NULL, sizeof( node_s ) );
            node_s_init( o->child0 );
            bcore_string_s_copy_sc( &o->child0->string, name );
            o->child0->hash = hash;
        }
    }
    else if( hash > o->hash )
    {
        if( o->child1 )
        {
            node_s_insert( o->child1, hash, name );
        }
        else
        {
            o->child1 = bcore_alloc( NULL, sizeof( node_s ) );
            node_s_init( o->child1 );
            bcore_string_s_copy_sc( &o->child1->string, name );
            o->child1->hash = hash;
        }
    }
    else
    {
        if( !bcore_string_s_equal_sc( &o->string, name ) != 0 ) ERR( "%s collides with %s", name, o->string.sc );
    }
}

static const char* node_s_name( const node_s* o, u2_t hash )
{
    if( hash < o->hash )
    {
        if( o->child0 )
        {
            return node_s_name( o->child0, hash );
        }
        else
        {
            return NULL;
        }
    }
    else if( hash > o->hash )
    {
        if( o->child1 )
        {
            return node_s_name( o->child1, hash );
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        return o->string.sc;
    }
}

/**********************************************************************************************************************/

// binary tree
typedef struct tree_s
{
    node_s* root;
    bcore_mutex_t mutex;
} tree_s;

static void tree_s_init( tree_s* o )
{
    o->root = NULL;
    bcore_mutex_init( &o->mutex );
}

void tree_s_down( tree_s* o )
{
    bcore_mutex_lock( &o->mutex );
    if( o->root )
    {
        node_s_down( o->root );
        o->root = bcore_free( o->root );
    }
    bcore_mutex_unlock( &o->mutex );
    bcore_mutex_down( &o->mutex );
}

/**********************************************************************************************************************/

tree_s* tree_s_g = NULL;

static void create_name_tree_s()
{
    if( tree_s_g == NULL )
    {
        tree_s_g = bcore_alloc( NULL, sizeof( tree_s ) );
        tree_s_init( tree_s_g );
    }
}

static void discard_name_tree_s()
{
    if( tree_s_g )
    {
        tree_s_down( tree_s_g );
        tree_s_g = bcore_free( tree_s_g );
    }
}

void bcore_name_open()
{
    static bcore_once_t flag = bcore_once_init;
    bcore_once( &flag, create_name_tree_s );
}

void bcore_name_manager_close()
{
    discard_name_tree_s();
}

const char* bcore_name_try_name( u2_t hash )
{
    if( !tree_s_g ) bcore_name_open();
    bcore_mutex_lock( &tree_s_g->mutex );
    const char* name = node_s_name( tree_s_g->root, hash );
    bcore_mutex_unlock( &tree_s_g->mutex );
    return name;
}

const char* bcore_name_get_name( u2_t hash )
{
    const char* name = bcore_name_try_name( hash );
    if( !name ) ERR( "hash %u has no name", hash );
    return name;
}

u2_t bcore_name_enroll( const char* name )
{
    if( !tree_s_g ) bcore_name_open();
    u2_t hash =  bcore_name_get_type( name );
    if( hash == 0 ) ERR( "Hash of '%s' is zero. Zero is a reserved value.", name );
    bcore_mutex_lock( &tree_s_g->mutex );
    if( tree_s_g->root )
    {
        node_s_insert( tree_s_g->root, hash, name );
    }
    else
    {
        tree_s_g->root = bcore_alloc( NULL, sizeof( node_s ) );
        node_s_init( tree_s_g->root );
        tree_s_g->root->hash = hash;
        bcore_string_s_copy_sc( &tree_s_g->root->string, name );
    }
    bcore_mutex_unlock( &tree_s_g->mutex );
    return hash;
}

/**********************************************************************************************************************/

static void bcore_name_print_predefined_type( tp_t (*hash)( sc_t name ), sc_t name )
{
    bcore_msg( "#define BCORE_TYPEOF_%s 0x%08x\n", name, hash( name ) );
}

void bcore_name_print_predefined_list( tp_t (*hash)( sc_t name ) )
{
    bcore_name_print_predefined_type( hash, "s3_t" );
    bcore_name_print_predefined_type( hash, "s2_t" );
    bcore_name_print_predefined_type( hash, "s1_t" );
    bcore_name_print_predefined_type( hash, "s0_t" );
    bcore_name_print_predefined_type( hash, "u3_t" );
    bcore_name_print_predefined_type( hash, "u2_t" );
    bcore_name_print_predefined_type( hash, "u1_t" );
    bcore_name_print_predefined_type( hash, "u0_t" );
    bcore_name_print_predefined_type( hash, "f3_t" );
    bcore_name_print_predefined_type( hash, "f2_t" );
    bcore_name_print_predefined_type( hash, "sz_t" );
    bcore_name_print_predefined_type( hash, "sd_t" );
    bcore_name_print_predefined_type( hash, "sc_t" );
    bcore_name_print_predefined_type( hash, "vd_t" );
    bcore_name_print_predefined_type( hash, "vc_t" );
    bcore_name_print_predefined_type( hash, "fp_t" );
    bcore_name_print_predefined_type( hash, "tp_t" );
    bcore_name_print_predefined_type( hash, "bool" );
    bcore_name_print_predefined_type( hash, "aware_t" );
    bcore_name_print_predefined_type( hash, "bcore_string_s" );
}

/**********************************************************************************************************************/

