/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_perspective.h"
#include "bcore_name_manager.h"
#include "bcore_control.h"
#include "bcore_threads.h"

/**********************************************************************************************************************/

void bcore_perspective_s_init( bcore_perspective_s* o, fp_t p_down )
{
    bcore_memzero( o, sizeof( *o ) );
    o->p_down = p_down;
}

void bcore_perspective_p_down( bcore_perspective_s* o )
{
    if( o->p_down ) o->p_down( o );
}

void bcore_perspective_s_discard( bcore_perspective_s* o )
{
    bcore_perspective_p_down( o );
    bcore_free( o );
}

/**********************************************************************************************************************/
// perspective manager

// node of binary tree
typedef struct node_s
{
    u3_t key;  // key = composite type
    bcore_perspective_s*  spect;
    unsigned holds_spect : 1;
    struct node_s* child0;
    struct node_s* child1;
} node_s;

static void node_s_init( node_s* o )
{
    o->key = 0;
    o->spect = NULL;
    o->holds_spect = 0;
    o->child0 = NULL;
    o->child1 = NULL;
}

static void node_p_down( node_s* o )
{
    if( o->child0 )
    {
        node_p_down( o->child0 );
        o->child0 = bcore_free( o->child0 );
    }
    if( o->child1 )
    {
        node_p_down( o->child1 );
        o->child1 = bcore_free( o->child1 );
    }
    o->key = 0;
    if( o->holds_spect ) bcore_perspective_s_discard( o->spect );
    o->spect = NULL;
    o->holds_spect = 0;
}

static void node_s_insert( node_s* o, u3_t key, bcore_perspective_s* spect, bool hold_spect )
{
    if( key < o->key )
    {
        if( o->child0 )
        {
            node_s_insert( o->child0, key, spect, hold_spect );
        }
        else
        {
            o->child0 = bcore_alloc( NULL, sizeof( node_s ) );
            node_s_init( o->child0 );
            o->child0->spect = spect;
            o->child0->holds_spect = hold_spect;
            o->child0->key = key;
        }
    }
    else if( key > o->key )
    {
        if( o->child1 )
        {
            node_s_insert( o->child1, key, spect, hold_spect );
        }
        else
        {
            o->child1 = bcore_alloc( NULL, sizeof( node_s ) );
            node_s_init( o->child1 );
            o->child1->spect = spect;
            o->child1->holds_spect = hold_spect;
            o->child1->key = key;
        }
    }
    else // already registered --> discard spect
    {
        if( hold_spect ) bcore_perspective_s_discard( spect );
    }
}

static bcore_perspective_s* node_s_spect( const node_s* o, u3_t key )
{
    if( key < o->key )
    {
        if( o->child0 )
        {
            return node_s_spect( o->child0, key );
        }
        else
        {
            return NULL;
        }
    }
    else if( key > o->key )
    {
        if( o->child1 )
        {
            return node_s_spect( o->child1, key );
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        return o->spect;
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

static void tree_p_down( tree_s* o )
{
    bcore_mutex_lock( &o->mutex );
    if( o->root )
    {
        node_p_down( o->root );
        o->root = bcore_free( o->root );
    }
    bcore_mutex_unlock( &o->mutex );
    bcore_mutex_down( &o->mutex );
}

static void tree_s_insert( tree_s* o, u3_t key, bcore_perspective_s* spect, bool hold_spect )
{
    bcore_mutex_lock( &o->mutex );
    if( o->root )
    {
        node_s_insert( o->root, key, spect, hold_spect );
    }
    else
    {
        o->root = bcore_alloc( NULL, sizeof( node_s ) );
        node_s_init( o->root );
        o->root->key = key;
        o->root->spect = spect;
        o->root->holds_spect = hold_spect;
    }
    bcore_mutex_unlock( &o->mutex );
}

/**********************************************************************************************************************/

static tree_s* tree_s_g = NULL;

void bcore_create_perspective_tree()
{
    if( tree_s_g == NULL )
    {
        tree_s_g = bcore_alloc( NULL, sizeof( tree_s ) );
        tree_s_init( tree_s_g );
    }
}

void bcore_discard_perspective_tree()
{
    if( tree_s_g )
    {
        tree_p_down( tree_s_g );
        tree_s_g = bcore_free( tree_s_g );
    }
}

void bcore_perspective_open()
{
    static bcore_once_t flag = bcore_once_init;
    bcore_once( &flag, bcore_create_perspective_tree );
}

void bcore_perspective_close()
{
    bcore_discard_perspective_tree();
}

const bcore_perspective_s* bcore_perspective_try_perspective( u2_t i_type, u2_t o_type )
{
    if( !tree_s_g ) bcore_perspective_open();
    const bcore_perspective_s* spect = NULL;
    bcore_mutex_lock( &tree_s_g->mutex );
    if( tree_s_g->root )
    {
        u3_t key = ( ( u3_t )i_type << 32 ) | o_type;
        spect = node_s_spect( tree_s_g->root, key );
    }
    bcore_mutex_unlock( &tree_s_g->mutex );
    return spect;
}

const bcore_perspective_s* bcore_perspective_get_perspective( u2_t i_type, u2_t o_type )
{
    const bcore_perspective_s* spect = bcore_perspective_try_perspective( i_type, o_type );
    if( !spect )
    {
        const char* p_name = bcore_name_try_name( i_type );
        if( p_name )
        {
            const char* o_name = bcore_name_try_name( o_type );
            if( o_name )
            {
                ERR( "perspective '%s' for object '%s' not enrolled", p_name, o_name );
            }
            else
            {
                ERR( "perspective '%s' for object '%u' not enrolled", p_name, o_type );
            }
        }
        else
        {
            const char* o_name = bcore_name_try_name( o_type );
            if( o_name )
            {
                ERR( "perspective '%u' for object '%s' not enrolled", i_type, o_name );
            }
            else
            {
                ERR( "perspective '%u' for object '%u' not enrolled", i_type, o_type );
            }
        }
    }
    return spect;
}

void bcore_perspective_enroll( u2_t i_type, u2_t o_type, bcore_perspective_s* spect )
{
    if( !tree_s_g ) bcore_perspective_open();
    u3_t key = ( ( u3_t )i_type << 32 ) | o_type;
    tree_s_insert( tree_s_g, key, spect, true );
}

/**********************************************************************************************************************/

