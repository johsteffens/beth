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

#include <time.h>

#include "bcore_btree.h"
#include "bcore_control.h"
#include "bcore_name_manager.h"
#include "bcore_signal.h"

/**********************************************************************************************************************/

typedef struct
{
    bcore_btree_ip_key_t key;
    bcore_btree_ip_val_t val;
} bcore_btree_ip_kv_s;

void bcore_btree_ip_kv_s_init( bcore_btree_ip_kv_s* o )
{
    o->key = 0;
    o->val = 0;
}

/**********************************************************************************************************************/

/** Node of a 2-3 btree.
 *  Child pointers can have one of three states:
 *  NULL: corresponding key-value pair is not used (for normal nodes this state only applies to child2)
 *  BNUL_IP: node is a leaf
 *  else: node has children
 */
typedef struct bcore_btree_node_ip_s
{
    bcore_btree_ip_kv_s  kv1;
    bcore_btree_ip_kv_s  kv2;
    struct bcore_btree_node_ip_s* parent;
    struct bcore_btree_node_ip_s* child0;
    struct bcore_btree_node_ip_s* child1;
    struct bcore_btree_node_ip_s* child2;
} bcore_btree_node_ip_s;

/// children of leaf-nodes point to bcore_btree_node_ip_s_null
bcore_btree_node_ip_s bcore_btree_node_ip_s_null_g = { { 0, 0 }, { 0, 0 }, NULL, NULL, NULL, NULL };
#define BNUL_IP ( &bcore_btree_node_ip_s_null_g )

void bcore_btree_node_ip_s_init( bcore_btree_node_ip_s* o )
{
    bcore_btree_ip_kv_s_init( &o->kv1 );
    bcore_btree_ip_kv_s_init( &o->kv2 );
    o->parent = o->child0 = o->child1 = o->child2 = NULL;
}

bcore_btree_node_ip_s* bcore_btree_node_ip_s_find( bcore_btree_node_ip_s* root, bcore_btree_ip_key_t key )
{
    if( !root ) return NULL;
    bcore_btree_node_ip_s* node = NULL;
    while( root->child0 != BNUL_IP && root != node )
    {
        node = root;
        root = ( key < node->kv1.key )                                         ? node->child0 :
               ( !node->child2 && key > node->kv1.key )                        ? node->child1 :
               (  node->child2 && key > node->kv2.key )                        ? node->child2 :
               (  node->child2 && key > node->kv1.key && key < node->kv2.key ) ? node->child1 : node;
    }
    return root;
}

void bcore_btree_node_ip_s_run( const bcore_btree_node_ip_s* root, void(*func)( vd_t arg, bcore_btree_ip_key_t key, bcore_btree_ip_val_t val ), vd_t arg )
{
    if( !root ) return;
    if( !func ) return;
    if( root->child0 )
    {
        if( root->child0 != BNUL_IP ) bcore_btree_node_ip_s_run( root->child0, func, arg );
    }
    if( root->child1 )
    {
        func( arg, root->kv1.key, root->kv1.val );
        if( root->child1 != BNUL_IP ) bcore_btree_node_ip_s_run( root->child1, func, arg );
    }
    if( root->child2 )
    {
        func( arg, root->kv2.key, root->kv2.val );
        if( root->child2 != BNUL_IP ) bcore_btree_node_ip_s_run( root->child2, func, arg );
    }
}

uz_t bcore_btree_node_ip_s_keys( bcore_btree_node_ip_s* root )
{
    if( !root || root == BNUL_IP ) return 0;
    uz_t keys = root->child2 ? 2 : 1;
    keys += bcore_btree_node_ip_s_keys( root->child0 );
    keys += bcore_btree_node_ip_s_keys( root->child1 );
    if( root->child2 ) keys += bcore_btree_node_ip_s_keys( root->child2 );
    return keys;
}

uz_t bcore_btree_node_ip_s_depth( bcore_btree_node_ip_s* root )
{
    if( !root || root == BNUL_IP ) return 0;
    return 1 + bcore_btree_node_ip_s_depth( root->child0 );
}

void bcore_btree_node_ip_s_set_parent_child0( bcore_btree_node_ip_s* o ) { if( o->child0 && o->child0 != BNUL_IP ) o->child0->parent = o; }
void bcore_btree_node_ip_s_set_parent_child1( bcore_btree_node_ip_s* o ) { if( o->child1 && o->child1 != BNUL_IP ) o->child1->parent = o; }
void bcore_btree_node_ip_s_set_parent_child2( bcore_btree_node_ip_s* o ) { if( o->child2 && o->child2 != BNUL_IP ) o->child2->parent = o; }
int  bcore_btree_node_ip_s_is_leaf(  bcore_btree_node_ip_s* o )          { return o->child0 == BNUL_IP; }
int  bcore_btree_node_ip_s_is_full(  bcore_btree_node_ip_s* o )          { return o->child2 != NULL; }
int  bcore_btree_node_ip_s_is_empty( bcore_btree_node_ip_s* o )          { return o->child1 == NULL; }

void bcore_btree_node_ip_s_check_consistency( bcore_btree_node_ip_s* o )
{
    if( bcore_btree_node_ip_s_null_g.kv1.key != 0    ) ERR( "bcore_btree_node_ip_s_null was modified" );
    if( bcore_btree_node_ip_s_null_g.kv1.val != 0    ) ERR( "bcore_btree_node_ip_s_null was modified" );
    if( bcore_btree_node_ip_s_null_g.kv2.key != 0    ) ERR( "bcore_btree_node_ip_s_null was modified" );
    if( bcore_btree_node_ip_s_null_g.kv2.val != 0    ) ERR( "bcore_btree_node_ip_s_null was modified" );
    if( bcore_btree_node_ip_s_null_g.parent  != NULL ) ERR( "bcore_btree_node_ip_s_null was modified" );
    if( bcore_btree_node_ip_s_null_g.child0  != NULL ) ERR( "bcore_btree_node_ip_s_null was modified" );
    if( bcore_btree_node_ip_s_null_g.child1  != NULL ) ERR( "bcore_btree_node_ip_s_null was modified" );
    if( bcore_btree_node_ip_s_null_g.child2  != NULL ) ERR( "bcore_btree_node_ip_s_null was modified" );

    if( !o ) return;
    if( bcore_btree_node_ip_s_is_empty( o ) ) ERR( "empty node" );
    if( o->child0 == NULL )       ERR( "deleted leaf" );
    if( o->child1 && o->child1 != BNUL_IP )
    {
        if( o != o->child0->parent ) ERR( "child0 incorrect parent" );
        if( o != o->child1->parent ) ERR( "child1 incorrect parent" );
        bcore_btree_node_ip_s_check_consistency( o->child0 );
        bcore_btree_node_ip_s_check_consistency( o->child1 );
        if(                      o->kv1.key <= o->child0->kv1.key ) ERR( "(%lu <= %lu)", o->kv1.key, o->child0->kv1.key );
        if( o->child0->child2 && o->kv1.key <= o->child0->kv2.key ) ERR( "(%lu <= %lu)", o->kv1.key, o->child0->kv2.key );
        if(                      o->kv1.key >= o->child1->kv1.key ) ERR( "(%lu >= %lu)", o->kv1.key, o->child1->kv1.key );
        if( o->child1->child2 && o->kv1.key >= o->child1->kv2.key ) ERR( "(%lu >= %lu)", o->kv1.key, o->child1->kv2.key );
    }
    if( o->child2 && o->child2 != BNUL_IP )
    {
        if( o->kv1.key >= o->kv2.key ) ERR( "(%lu >= %lu)", o->kv1.key, o->kv2.key );
        if( o != o->child2->parent ) ERR( "child2 incorrect parent" );
        bcore_btree_node_ip_s_check_consistency( o->child2 );
        if(                      o->kv2.key <= o->child1->kv1.key ) ERR( "(%lu <= %lu)", o->kv2.key, o->child1->kv1.key );
        if( o->child1->child2 && o->kv2.key <= o->child1->kv2.key ) ERR( "(%lu <= %lu)", o->kv2.key, o->child1->kv2.key );
        if(                      o->kv2.key >= o->child2->kv1.key ) ERR( "(%lu >= %lu)", o->kv2.key, o->child2->kv1.key );
        if( o->child2->child2 && o->kv2.key >= o->child2->kv2.key ) ERR( "(%lu >= %lu)", o->kv2.key, o->child2->kv2.key );
    }
}

st_s* bcore_btree_node_ip_s_show( bcore_btree_node_ip_s* o, uz_t depth )
{
    st_s* string = st_s_create();
    if( !o )
    {
        st_s_pushf( string, "()\n" );
        return string;
    }
    if( o->child1 )
    {
        if( o->child0 && o->child0 != BNUL_IP ) st_s_push_st_d( string, bcore_btree_node_ip_s_show( o->child0, depth + 1 ) );
        for( uz_t i = 0; i < depth; i++ ) st_s_pushf( string, "    " );
        st_s_push_fa( string, "(#<u3_t>,#<u3_t>)\n", (u3_t)o->kv1.key, (u3_t)(intptr_t)o->kv1.val );
        if( o->child1 && o->child1 != BNUL_IP ) st_s_pushf( string, "    " );
    }
    if( o->child2 )
    {
        for( uz_t i = 0; i < depth; i++ ) st_s_pushf( string, "    " );
        st_s_push_fa( string, "(#<u3_t>,#<u3_t>)\n", (u3_t)o->kv2.key, (u3_t)(intptr_t)o->kv2.val );
        if( o->child2 && o->child2 != BNUL_IP ) st_s_push_st_d( string, bcore_btree_node_ip_s_show( o->child2, depth + 1 ) );
    }
    return string;
}

/**********************************************************************************************************************/

struct bcore_btree_ip_s
{
    bcore_btree_node_ip_s* root;
    bcore_btree_node_ip_s* chain_beg; // begin of chain of blocks of bcore_btree_node_ip_s[] with last element being pointer to next block
    bcore_btree_node_ip_s* chain_end; // end of chain of blocks
    bcore_btree_node_ip_s* chain_ins; // pointer for new insertions
    bcore_btree_node_ip_s* del_chain; // chain of deleted elements (preferably used by new insertions)
    uz_t   block_size;
};

bcore_btree_ip_s* bcore_btree_ip_s_create()
{
    bcore_btree_ip_s* o = bcore_alloc( NULL, sizeof( bcore_btree_ip_s ) );
    o->root      = NULL;
    o->chain_beg = NULL;
    o->chain_end = NULL;
    o->chain_ins = NULL;
    o->del_chain = NULL;
    o->block_size = 1024;
    return o;
}

void bcore_btree_ip_s_discard( bcore_btree_ip_s* o )
{
    o->root = NULL;

    bcore_btree_node_ip_s* chain_beg = o->chain_beg;
    while( chain_beg )
    {
        bcore_btree_node_ip_s* new_beg = *( bcore_btree_node_ip_s** )( chain_beg + o->block_size );
        bcore_alloc( chain_beg, 0 );
        chain_beg = new_beg;
    }

    bcore_alloc( o, 0 );
}

bcore_btree_node_ip_s* bcore_btree_ip_s_new_node( bcore_btree_ip_s* o )
{
    if( o->del_chain )
    {
        bcore_btree_node_ip_s* new_node = o->del_chain;
        o->del_chain = new_node->parent;
        bcore_btree_node_ip_s_init( new_node );
        return new_node;
    }
    else
    {
        if( o->chain_ins == o->chain_end )
        {
            bcore_btree_node_ip_s* new_ptr = bcore_alloc( NULL, o->block_size * sizeof( bcore_btree_node_ip_s ) + sizeof( bcore_btree_node_ip_s* ) );
            if( !o->chain_beg )
            {
                o->chain_beg = new_ptr;
            }
            else
            {
                ( ( bcore_btree_node_ip_s** )( o->chain_end ) )[ 0 ] = new_ptr;
            }
            o->chain_ins = new_ptr;
            o->chain_end = new_ptr + o->block_size;
            *( bcore_btree_node_ip_s** )( o->chain_end ) = NULL;
        }
        bcore_btree_node_ip_s* new_node = o->chain_ins;
        bcore_btree_node_ip_s_init( new_node );
        o->chain_ins++;
        return new_node;
    }
}

// Deleted nodes are marked by setting all children NULL
// and chained together using pointer bcore_btree_node_ip_s.parent.
void bcore_btree_ip_s_delete_node( bcore_btree_ip_s* o, bcore_btree_node_ip_s* node )
{
    node->child0 = NULL;
    node->child1 = NULL;
    node->child2 = NULL;
    node->parent = o->del_chain;
    o->del_chain = node;
}

// recursively pushes an element into the tree
void bcore_btree_ip_s_push( bcore_btree_ip_s* o, bcore_btree_node_ip_s* node, bcore_btree_ip_kv_s* kv, bcore_btree_node_ip_s* child0, bcore_btree_node_ip_s* child1 )
{
    if( bcore_btree_node_ip_s_is_full( node ) )
    {
        bcore_btree_node_ip_s* l_node = node;
        bcore_btree_node_ip_s* r_node = bcore_btree_ip_s_new_node( o );
        bcore_btree_ip_kv_s root_kv;

        if( kv->key < node->kv1.key )
        {
            root_kv        = l_node->kv1;
            r_node->kv1    = l_node->kv2;
            r_node->child0 = l_node->child1;
            r_node->child1 = l_node->child2;
            l_node->kv1    = *kv;
            l_node->child0 = child0;
            l_node->child1 = child1;
        }
        else if( kv->key > node->kv2.key )
        {
            root_kv        = l_node->kv2;
            r_node->kv1    = *kv;
            r_node->child0 = child0;
            r_node->child1 = child1;
        }
        else
        {
            root_kv        = *kv;
            r_node->kv1    = l_node->kv2;
            r_node->child1 = l_node->child2;
            r_node->child0 = child1;
            l_node->child1 = child0;
        }
        r_node->child2 = NULL;
        l_node->child2 = NULL;

        bcore_btree_node_ip_s_set_parent_child0( r_node );
        bcore_btree_node_ip_s_set_parent_child1( r_node );
        bcore_btree_node_ip_s_set_parent_child0( l_node );
        bcore_btree_node_ip_s_set_parent_child1( l_node );

        if( l_node->parent )
        {
            bcore_btree_ip_s_push( o, l_node->parent, &root_kv, l_node, r_node );
        }
        else
        {
            o->root = bcore_btree_ip_s_new_node( o );
            o->root->kv1    = root_kv;
            o->root->child0 = l_node;
            o->root->child1 = r_node;
            l_node->parent  = o->root;
            r_node->parent  = o->root;
        }
    }
    else
    {
        if( kv->key < node->kv1.key )
        {
            node->kv2    = node->kv1;
            node->kv1    = *kv;
            node->child2 = node->child1;
            node->child1 = child1;
            node->child0 = child0;
            bcore_btree_node_ip_s_set_parent_child0( node );
            bcore_btree_node_ip_s_set_parent_child1( node );
            bcore_btree_node_ip_s_set_parent_child2( node );
        }
        else
        {
            node->kv2    = *kv;
            node->child2 = child1;
            node->child1 = child0;
            bcore_btree_node_ip_s_set_parent_child1( node );
            bcore_btree_node_ip_s_set_parent_child2( node );
        }
    }
}

// Recursively pulls an element from a non-leaf into an empty child node
void bcore_btree_ip_s_pull( bcore_btree_ip_s* o, bcore_btree_node_ip_s* node )
{
    if( bcore_btree_node_ip_s_is_empty( node->child0 ) )
    {
        if( bcore_btree_node_ip_s_is_full( node->child1 ) )
        {
            node->child0->kv1    = node->kv1;
            node->child0->child1 = node->child1->child0;
            bcore_btree_node_ip_s_set_parent_child1( node->child0 );
            node->kv1            = node->child1->kv1;
            node->child1->kv1    = node->child1->kv2;
            node->child1->child0 = node->child1->child1;
            node->child1->child1 = node->child1->child2;
            node->child1->child2 = NULL;
        }
        else if( bcore_btree_node_ip_s_is_full( node ) )
        {
            node->child1->kv2    = node->child1->kv1;
            node->child1->kv1    = node->kv1;
            node->child1->child2 = node->child1->child1;
            node->child1->child1 = node->child1->child0;
            node->child1->child0 = node->child0->child0;
            bcore_btree_node_ip_s_set_parent_child0( node->child1 );
            bcore_btree_ip_s_delete_node( o, node->child0 );
            node->kv1    = node->kv2;
            node->child0 = node->child1;
            node->child1 = node->child2;
            node->child2 = NULL;
        }
        else
        {
            node->child1->kv2    = node->child1->kv1;
            node->child1->kv1    = node->kv1;
            node->child1->child2 = node->child1->child1;
            node->child1->child1 = node->child1->child0;
            node->child1->child0 = node->child0->child0;
            bcore_btree_node_ip_s_set_parent_child0( node->child1 );
            bcore_btree_ip_s_delete_node( o, node->child0 );
            node->child0 = node->child1;
            node->child1 = node->child2 = NULL;
            if( node->parent )
            {
                bcore_btree_ip_s_pull( o, node->parent );
            }
            else
            {
                o->root = node->child0;
                o->root->parent = NULL;
                bcore_btree_ip_s_delete_node( o, node );
            }
        }
    }
    else if( bcore_btree_node_ip_s_is_empty( node->child1 ) )
    {
        if( bcore_btree_node_ip_s_is_full( node->child0 ) )
        {
            node->child1->kv1    = node->kv1;
            node->child1->child1 = node->child1->child0;
            node->child1->child0 = node->child0->child2;
            bcore_btree_node_ip_s_set_parent_child0( node->child1 );
            node->kv1            = node->child0->kv2;
            node->child0->child2 = NULL;
        }
        else if( bcore_btree_node_ip_s_is_full( node ) )
        {
            node->child0->kv2    = node->kv1;
            node->child0->child2 = node->child1->child0;
            bcore_btree_node_ip_s_set_parent_child2( node->child0 );
            bcore_btree_ip_s_delete_node( o, node->child1 );
            node->kv1    = node->kv2;
            node->child1 = node->child2;
            node->child2 = NULL;
        }
        else
        {
            node->child0->kv2    = node->kv1;
            node->child0->child2 = node->child1->child0;
            bcore_btree_node_ip_s_set_parent_child2( node->child0 );
            bcore_btree_ip_s_delete_node( o, node->child1 );
            node->child1 = node->child2 = NULL;
            if( node->parent )
            {
                bcore_btree_ip_s_pull( o, node->parent );
            }
            else
            {
                o->root = node->child0;
                o->root->parent = NULL;
                bcore_btree_ip_s_delete_node( o, node );
            }
        }
    }
    else // node->child2 is empty
    {
        if( bcore_btree_node_ip_s_is_full( node ) )
        {
            if( bcore_btree_node_ip_s_is_full( node->child1 ) )
            {
                node->child2->kv1 = node->kv2;
                node->child2->child1 = node->child2->child0;
                node->child2->child0 = node->child1->child2;
                bcore_btree_node_ip_s_set_parent_child0( node->child2 );
                node->kv2            = node->child1->kv2;
                node->child1->child2 = NULL;
            }
            else
            {
                node->child1->kv2    = node->kv2;
                node->child1->child2 = node->child2->child0;
                bcore_btree_node_ip_s_set_parent_child2( node->child1 );
                bcore_btree_ip_s_delete_node( o, node->child2 );
                node->child2         = NULL;
            }
        }
    }
}

bcore_btree_ip_val_t* bcore_btree_ip_s_val( const bcore_btree_ip_s* o, bcore_btree_ip_key_t key )
{
    if( !o )    return NULL;
    bcore_btree_node_ip_s* node = bcore_btree_node_ip_s_find( o->root, key );
    if( !node ) return NULL;
    if( node->kv1.key == key ) return &node->kv1.val;
    if( node->child2 && node->kv2.key == key ) return &node->kv2.val;
    return NULL;
}

int bcore_btree_ip_s_set( bcore_btree_ip_s* o, bcore_btree_ip_key_t key, bcore_btree_ip_val_t val )
{
    if( !o ) return -2;
    if( !o->root )
    {
        o->root = bcore_btree_ip_s_new_node( o );
        o->root->child0 = o->root->child1 = BNUL_IP;
        o->root->child2 = NULL;
        o->root->kv1.key = key;
        o->root->kv1.val = val;
        return 1;
    }

    bcore_btree_node_ip_s* node = bcore_btree_node_ip_s_find( o->root, key );
    if( !node ) return -2;

    if( node->kv1.key == key )
    {
        if( node->kv1.val == val ) return 0;
        node->kv1.val = val;
        return -1;
    }
    else if( node->child2 && node->kv2.key == key )
    {
        if( node->kv2.val == val ) return 0;
        node->kv2.val = val;
        return -1;
    }
    else
    {
        bcore_btree_ip_kv_s kv = { key, val };
        bcore_btree_ip_s_push( o, node, &kv, BNUL_IP, BNUL_IP );
        return 1;
    }
}

int bcore_btree_ip_s_remove( bcore_btree_ip_s* o, bcore_btree_ip_key_t key )
{
    if( !o       ) return -1;
    if( !o->root ) return  0;
    bcore_btree_node_ip_s* node = bcore_btree_node_ip_s_find( o->root, key );
    if( !node    ) return -1;

    if( node->kv1.key == key )
    {
        if( !bcore_btree_node_ip_s_is_leaf( node ) )
        {
            bcore_btree_node_ip_s* trace = node->child0;
            while( !bcore_btree_node_ip_s_is_leaf( trace ) ) trace = ( trace->child2 ) ? trace->child2 : trace->child1;
            if( bcore_btree_node_ip_s_is_full( trace ) )
            {
                node->kv1   = trace->kv2;
                trace->child2 = NULL;
            }
            else
            {
                node->kv1 = trace->kv1;
                trace->child1 = trace->child2 = NULL;
                bcore_btree_ip_s_pull( o, trace->parent );
            }
        }
        else if( bcore_btree_node_ip_s_is_full( node ) )
        {
            node->kv1 = node->kv2;
            node->child2 = NULL;
        }
        else
        {
            node->child1 = node->child2 = NULL;
            if( node->parent )
            {
                bcore_btree_ip_s_pull( o, node->parent );
            }
            else
            {
                bcore_btree_ip_s_delete_node( o, node );
                o->root = NULL;
            }
        }
        return 1;
    }

    if( node->kv2.key == key )
    {
        if( !bcore_btree_node_ip_s_is_leaf( node ) )
        {
            bcore_btree_node_ip_s* trace = ( node->child2 ) ? node->child2 : node->child1;
            while( !bcore_btree_node_ip_s_is_leaf( trace ) ) trace = trace->child0;
            if( bcore_btree_node_ip_s_is_full( trace ) )
            {
                node->kv2     = trace->kv1;
                trace->kv1    = trace->kv2;
                trace->child2 = NULL;
            }
            else
            {
                node->kv2   = trace->kv1;
                trace->child1 = trace->child2 = NULL;
                bcore_btree_ip_s_pull( o, trace->parent );
            }
        }
        else if( bcore_btree_node_ip_s_is_full( node ) )
        {
            node->child2 = NULL;
        }
        else
        {
            node->child1 = node->child2 = NULL;
            if( node->parent )
            {
                bcore_btree_ip_s_pull( o, node->parent );
            }
            else
            {
                bcore_btree_ip_s_delete_node( o, node );
                o->root = NULL;
            }
        }
        return 1;
    }
    return 0;
}

void bcore_btree_ip_s_run( const bcore_btree_ip_s* o, void(*func)( vd_t arg, bcore_btree_ip_key_t key, bcore_btree_ip_val_t val ), vd_t arg )
{
    bcore_btree_node_ip_s_run( o->root, func, arg );
}

st_s* bcore_btree_ip_s_show( bcore_btree_ip_s* o )
{
    bcore_btree_node_ip_s_check_consistency( o->root );
    st_s* s = bcore_btree_node_ip_s_show( o->root, 0 );
    st_s_pushf( s, "\n" );
    return s;
}

st_s* bcore_btree_ip_s_status( bcore_btree_ip_s* o )
{
    uz_t blocks = 0;
    uz_t nodes = 0;
    uz_t deleted_nodes = 0;
    if( o->chain_beg )
    {
        bcore_btree_node_ip_s* chain_beg = o->chain_beg;
        while( chain_beg )
        {
            chain_beg = *( bcore_btree_node_ip_s** )( chain_beg + o->block_size );
            blocks++;
        }
        nodes = blocks * o->block_size - ( o->chain_end - o->chain_ins );
    }
    if( o->del_chain )
    {
        bcore_btree_node_ip_s* del_chain = o->del_chain;
        while( del_chain )
        {
            del_chain = del_chain->parent;
            deleted_nodes++;
        }
    }

    st_s* string = st_s_create();
    uz_t used_nodes = nodes - deleted_nodes;
    st_s_push_fa( string, "keys ........... #<uz_t>\n", bcore_btree_node_ip_s_keys( o->root ) );
    st_s_push_fa( string, "nodes .......... #<uz_t>\n", used_nodes );
    st_s_push_fa( string, "keys/nodes ..... #<f3_t>\n", used_nodes > 0 ? ( f3_t )( bcore_btree_node_ip_s_keys( o->root ) ) / used_nodes : 0 );
    st_s_push_fa( string, "depth .......... #<uz_t>\n", bcore_btree_node_ip_s_depth( o->root ) );
    st_s_push_fa( string, "block size ..... #<uz_t>\n", o->block_size );
    st_s_push_fa( string, "blocks ......... #<uz_t>\n", blocks );
    st_s_push_fa( string, "deleted nodes .. #<uz_t>\n", deleted_nodes );
    return string;
}

static st_s* btree_ip_s_selftest( void )
{
    st_s* log = st_s_createf( "== btree_ip_s_selftest " );
    st_s_push_char_n( log, '=', 80 - log->size );
    st_s_push_char( log, '\n' );
    bcore_btree_ip_s* t = bcore_btree_ip_s_create();
    const uz_t cycles = 200000;

    bcore_btree_ip_kv_s* kvbuf = bcore_alloc( NULL, cycles * sizeof( bcore_btree_ip_kv_s ) );
    uz_t kvbuf_size = 0;

    clock_t time = clock();
    st_s_pushf( log, "Mixed access: " );

    {
        u3_t rval1 = 1;
        u3_t rval2 = 12345;
        for( uz_t i = 0; i < cycles; i++ )
        {
            rval1 = bcore_xsg_u2( rval1 );
            rval2 = bcore_xsg_u2( rval2 );
            bcore_btree_ip_kv_s kv;
            kv.key = ( bcore_btree_ip_key_t )rval1;
            kv.val = ( bcore_btree_ip_val_t )(intptr_t)rval2;
            kvbuf[ kvbuf_size++ ] = kv;

            // set
            bcore_btree_ip_s_set( t, kv.key, kv.val );

            // retrieve
            rval1 = bcore_xsg_u2( rval1 );
            kv = kvbuf[ rval1 % kvbuf_size ];
            bcore_btree_ip_val_t* val_ptr = bcore_btree_ip_s_val( t, kv.key );
            if( kv.val != *val_ptr ) ERR( "value mismatch (%lu vs %lu)", kv.val, (u3_t)(intptr_t)*val_ptr );

            // delete
            rval1 = bcore_xsg_u2( rval1 );
            if( ( ( rval1 >> 10 ) & 1 ) == 1 )
            {
                rval1 = bcore_xsg_u2( rval1 );
                uz_t idx = rval1 % kvbuf_size;
                bcore_btree_ip_kv_s kv = kvbuf[ idx ];
                if( !bcore_btree_ip_s_val( t, kv.key ) )  ERR( "key (%lu) not found", kv.key );
                bcore_btree_ip_s_remove( t, kv.key );
                if( bcore_btree_ip_s_val( t, kv.key ) )  ERR( "deleted key still exists (%lu)", kv.key );
                kvbuf_size--;
                kvbuf[ idx ] = kvbuf[ kvbuf_size ];
            }
        }
    }

    time = clock() - time;
    st_s_pushf( log, "(%5.3fs)\n", ( double )time/CLOCKS_PER_SEC );

    bcore_btree_node_ip_s_check_consistency( t->root );
    st_s_push_st_d( log, bcore_btree_ip_s_status( t ) );

    time = clock();
    st_s_pushf( log, "\nRead-access of %lu keys: ", kvbuf_size );
    uz_t read_cycles = 20;
    for( uz_t j = 0; j < read_cycles; j++ )
    {
        for( uz_t i = 0; i < kvbuf_size; i++ )
        {
            if( *bcore_btree_ip_s_val( t, kvbuf[ i ].key ) != kvbuf[ i ].val ) ERR( "value mismatch (%lu vs %lu)", kvbuf[ i ].key, kvbuf[ i ].val );
        }
    }
    time = clock() - time;
    st_s_pushf( log, "(%gs per access)\n", ( ( double )time/CLOCKS_PER_SEC ) / ( kvbuf_size * read_cycles ) );

    st_s_pushf( log, "\n" );
    st_s_pushf( log, "Removal: " );
    time = clock();
    while( kvbuf_size )
    {
        kvbuf_size--;
        bcore_btree_ip_kv_s kv = kvbuf[ kvbuf_size ];
        if( !bcore_btree_ip_s_val( t, kv.key ) )  ERR( "key (%lu) not found", kv.key );
        bcore_btree_ip_s_remove( t, kv.key );
        if( bcore_btree_ip_s_val( t, kv.key ) )  ERR( "deleted key still exists (%lu)", kv.key );
    }
    time = clock() - time;
    st_s_pushf( log, "(%5.3fs)\n", ( double )time/CLOCKS_PER_SEC );

    if( t->root ) ERR( "root still exists" );

    st_s_push_st_d( log, bcore_btree_ip_s_status( t ) );

    bcore_btree_ip_s_discard( t );
    bcore_alloc( kvbuf, 0 );
    st_s_push_char_n( log, '=', 80 );
    st_s_push_char( log, '\n' );
    return log;
}

/**********************************************************************************************************************/
/**********************************************************************************************************************/
/// btree_ps
/**********************************************************************************************************************/
/**********************************************************************************************************************/

typedef struct
{
    bcore_btree_ps_key_t key;
    bcore_btree_ps_val_t val;
} bcore_btree_ps_kv_s;

void bcore_btree_ps_kv_s_init( bcore_btree_ps_kv_s* o )
{
    o->key = 0;
    o->val = 0;
}

/**********************************************************************************************************************/

/** Node of a 2-3 btree.
 *  Child pointers can have one of three states:
 *  NULL: corresponding key-value pair is not used (for normal nodes this state only applies to child2)
 *  BNUL_PS: node is a leaf
 *  else: node has children
 */
typedef struct bcore_btree_node_ps_s
{
    bcore_btree_ps_kv_s  kv1;
    bcore_btree_ps_kv_s  kv2;
    struct bcore_btree_node_ps_s* parent;
    struct bcore_btree_node_ps_s* child0;
    struct bcore_btree_node_ps_s* child1;
    struct bcore_btree_node_ps_s* child2;
} bcore_btree_node_ps_s;

/// children of leaf-nodes point to bcore_btree_node_ps_s_null
bcore_btree_node_ps_s bcore_btree_node_ps_s_null_g = { { 0, 0 }, { 0, 0 }, NULL, NULL, NULL, NULL };
#define BNUL_PS ( &bcore_btree_node_ps_s_null_g )

void bcore_btree_node_ps_s_init( bcore_btree_node_ps_s* o )
{
    bcore_btree_ps_kv_s_init( &o->kv1 );
    bcore_btree_ps_kv_s_init( &o->kv2 );
    o->parent = o->child0 = o->child1 = o->child2 = NULL;
}

bcore_btree_node_ps_s* bcore_btree_node_ps_s_find( bcore_btree_node_ps_s* root, bcore_btree_ps_key_t key )
{
    if( !root ) return NULL;
    bcore_btree_node_ps_s* node = NULL;
    while( root->child0 != BNUL_PS && root != node )
    {
        node = root;
        root = ( key < node->kv1.key )                                         ? node->child0 :
               ( !node->child2 && key > node->kv1.key )                        ? node->child1 :
               (  node->child2 && key > node->kv2.key )                        ? node->child2 :
               (  node->child2 && key > node->kv1.key && key < node->kv2.key ) ? node->child1 : node;
    }
    return root;
}

void bcore_btree_node_ps_s_run( const bcore_btree_node_ps_s* root, void(*func)( vd_t arg, bcore_btree_ps_key_t key, bcore_btree_ps_val_t val ), vd_t arg )
{
    if( !root ) return;
    if( !func ) return;
    if( root->child0 )
    {
        if( root->child0 != BNUL_PS ) bcore_btree_node_ps_s_run( root->child0, func, arg );
    }
    if( root->child1 )
    {
        func( arg, root->kv1.key, root->kv1.val );
        if( root->child1 != BNUL_PS ) bcore_btree_node_ps_s_run( root->child1, func, arg );
    }
    if( root->child2 )
    {
        func( arg, root->kv2.key, root->kv2.val );
        if( root->child2 != BNUL_PS ) bcore_btree_node_ps_s_run( root->child2, func, arg );
    }
}

uz_t bcore_btree_node_ps_s_count( const bcore_btree_node_ps_s* root, bl_t (*func)( vd_t arg, bcore_btree_ps_key_t key, bcore_btree_ps_val_t val ), vd_t arg )
{
    uz_t count = 0;
    if( !root ) return count;
    if( root->child0 )
    {
        if( root->child0 != BNUL_PS ) count += bcore_btree_node_ps_s_count( root->child0, func, arg );
    }
    if( root->child1 )
    {
        count += ( func ) ? func( arg, root->kv1.key, root->kv1.val ) : 1;
        if( root->child1 != BNUL_PS ) count += bcore_btree_node_ps_s_count( root->child1, func, arg );
    }
    if( root->child2 )
    {
        count += ( func ) ? func( arg, root->kv2.key, root->kv2.val ) : 1;
        if( root->child2 != BNUL_PS ) count += bcore_btree_node_ps_s_count( root->child2, func, arg );
    }
    return count;
}

uz_t bcore_btree_node_ps_s_sum( const bcore_btree_node_ps_s* root, bl_t (*func)( vd_t arg, bcore_btree_ps_key_t key, bcore_btree_ps_val_t val ), vd_t arg )
{
    uz_t sum = 0;
    if( !root ) return sum;
    if( root->child0 )
    {
        if( root->child0 != BNUL_PS ) sum += bcore_btree_node_ps_s_sum( root->child0, func, arg );
    }
    if( root->child1 )
    {
        sum += root->kv1.val * ( ( func ) ? func( arg, root->kv1.key, root->kv1.val ) : 1 );
        if( root->child1 != BNUL_PS ) sum += bcore_btree_node_ps_s_sum( root->child1, func, arg );
    }
    if( root->child2 )
    {
        sum += root->kv2.val * ( ( func ) ? func( arg, root->kv2.key, root->kv2.val ) : 1 );
        if( root->child2 != BNUL_PS ) sum += bcore_btree_node_ps_s_sum( root->child2, func, arg );
    }
    return sum;
}

uz_t bcore_btree_node_ps_s_keys( bcore_btree_node_ps_s* root )
{
    if( !root || root == BNUL_PS ) return 0;
    uz_t keys = root->child2 ? 2 : 1;
    keys += bcore_btree_node_ps_s_keys( root->child0 );
    keys += bcore_btree_node_ps_s_keys( root->child1 );
    if( root->child2 ) keys += bcore_btree_node_ps_s_keys( root->child2 );
    return keys;
}

uz_t bcore_btree_node_ps_s_depth( bcore_btree_node_ps_s* root )
{
    if( !root || root == BNUL_PS ) return 0;
    return 1 + bcore_btree_node_ps_s_depth( root->child0 );
}

void bcore_btree_node_ps_s_set_parent_child0( bcore_btree_node_ps_s* o ) { if( o->child0 && o->child0 != BNUL_PS ) o->child0->parent = o; }
void bcore_btree_node_ps_s_set_parent_child1( bcore_btree_node_ps_s* o ) { if( o->child1 && o->child1 != BNUL_PS ) o->child1->parent = o; }
void bcore_btree_node_ps_s_set_parent_child2( bcore_btree_node_ps_s* o ) { if( o->child2 && o->child2 != BNUL_PS ) o->child2->parent = o; }
int  bcore_btree_node_ps_s_is_leaf(  bcore_btree_node_ps_s* o )          { return o->child0 == BNUL_PS; }
int  bcore_btree_node_ps_s_is_full(  bcore_btree_node_ps_s* o )          { return o->child2 != NULL; }
int  bcore_btree_node_ps_s_is_empty( bcore_btree_node_ps_s* o )          { return o->child1 == NULL; }

void bcore_btree_node_ps_s_check_consistency( bcore_btree_node_ps_s* o )
{
    if( bcore_btree_node_ps_s_null_g.kv1.key != 0    ) ERR( "bcore_btree_node_ps_s_null was modified" );
    if( bcore_btree_node_ps_s_null_g.kv1.val != 0    ) ERR( "bcore_btree_node_ps_s_null was modified" );
    if( bcore_btree_node_ps_s_null_g.kv2.key != 0    ) ERR( "bcore_btree_node_ps_s_null was modified" );
    if( bcore_btree_node_ps_s_null_g.kv2.val != 0    ) ERR( "bcore_btree_node_ps_s_null was modified" );
    if( bcore_btree_node_ps_s_null_g.parent  != NULL ) ERR( "bcore_btree_node_ps_s_null was modified" );
    if( bcore_btree_node_ps_s_null_g.child0  != NULL ) ERR( "bcore_btree_node_ps_s_null was modified" );
    if( bcore_btree_node_ps_s_null_g.child1  != NULL ) ERR( "bcore_btree_node_ps_s_null was modified" );
    if( bcore_btree_node_ps_s_null_g.child2  != NULL ) ERR( "bcore_btree_node_ps_s_null was modified" );

    if( !o ) return;
    if( bcore_btree_node_ps_s_is_empty( o ) ) ERR( "empty node" );
    if( o->child0 == NULL )       ERR( "deleted leaf" );
    if( o->child1 && o->child1 != BNUL_PS )
    {
        if( o != o->child0->parent ) ERR( "child0 incorrect parent" );
        if( o != o->child1->parent ) ERR( "child1 incorrect parent" );
        bcore_btree_node_ps_s_check_consistency( o->child0 );
        bcore_btree_node_ps_s_check_consistency( o->child1 );
        if(                      o->kv1.key <= o->child0->kv1.key ) ERR( "(%lu <= %lu)", o->kv1.key, o->child0->kv1.key );
        if( o->child0->child2 && o->kv1.key <= o->child0->kv2.key ) ERR( "(%lu <= %lu)", o->kv1.key, o->child0->kv2.key );
        if(                      o->kv1.key >= o->child1->kv1.key ) ERR( "(%lu >= %lu)", o->kv1.key, o->child1->kv1.key );
        if( o->child1->child2 && o->kv1.key >= o->child1->kv2.key ) ERR( "(%lu >= %lu)", o->kv1.key, o->child1->kv2.key );
    }
    if( o->child2 && o->child2 != BNUL_PS )
    {
        if( o->kv1.key >= o->kv2.key ) ERR( "(%lu >= %lu)", o->kv1.key, o->kv2.key );
        if( o != o->child2->parent ) ERR( "child2 incorrect parent" );
        bcore_btree_node_ps_s_check_consistency( o->child2 );
        if(                      o->kv2.key <= o->child1->kv1.key ) ERR( "(%lu <= %lu)", o->kv2.key, o->child1->kv1.key );
        if( o->child1->child2 && o->kv2.key <= o->child1->kv2.key ) ERR( "(%lu <= %lu)", o->kv2.key, o->child1->kv2.key );
        if(                      o->kv2.key >= o->child2->kv1.key ) ERR( "(%lu >= %lu)", o->kv2.key, o->child2->kv1.key );
        if( o->child2->child2 && o->kv2.key >= o->child2->kv2.key ) ERR( "(%lu >= %lu)", o->kv2.key, o->child2->kv2.key );
    }
}

st_s* bcore_btree_node_ps_s_show( bcore_btree_node_ps_s* o, uz_t depth )
{
    st_s* string = st_s_create();
    if( !o )
    {
        st_s_pushf( string, "()\n" );
        return string;
    }
    if( o->child1 )
    {
        if( o->child0 && o->child0 != BNUL_PS ) st_s_push_st_d( string, bcore_btree_node_ps_s_show( o->child0, depth + 1 ) );
        for( uz_t i = 0; i < depth; i++ ) st_s_pushf( string, "    " );
        st_s_pushf( string, "(%lu,%lu)\n", ( int64_t )(intptr_t)o->kv1.key, ( int64_t )(intptr_t)o->kv1.val );
        if( o->child1 && o->child1 != BNUL_PS ) st_s_pushf( string, "    " );
    }
    if( o->child2 )
    {
        for( uz_t i = 0; i < depth; i++ ) st_s_pushf( string, "    " );
        st_s_pushf( string, "(%lu,%lu)\n", ( int64_t )(intptr_t)o->kv2.key, ( int64_t )(intptr_t)o->kv2.val );
        if( o->child2 && o->child2 != BNUL_PS ) st_s_push_st_d( string, bcore_btree_node_ps_s_show( o->child2, depth + 1 ) );
    }
    return string;
}

/**********************************************************************************************************************/

struct bcore_btree_ps_s
{
    bcore_btree_node_ps_s* root;
    bcore_btree_node_ps_s* chain_beg; // begin of chain of blocks of bcore_btree_node_ps_s[] with last element being pointer to next block
    bcore_btree_node_ps_s* chain_end; // end of chain of blocks
    bcore_btree_node_ps_s* chain_ins; // pointer for new insertions
    bcore_btree_node_ps_s* del_chain; // chain of deleted elements (preferably used by new insertions)
    vd_t (*alloc)( vd_t, uz_t size ); // alloc function
    uz_t   block_size;
};

bcore_btree_ps_s* bcore_btree_ps_s_create( vd_t (*alloc)( vd_t, uz_t size ) )
{
    bcore_btree_ps_s* o = NULL;
    if( alloc )
    {
        o = alloc( NULL, sizeof( bcore_btree_ps_s ) );
        o->alloc = alloc;
    }
    else
    {
        o = bcore_alloc( NULL, sizeof( bcore_btree_ps_s ) );
        o->alloc = bcore_alloc;
    }
    o->root      = NULL;
    o->chain_beg = NULL;
    o->chain_end = NULL;
    o->chain_ins = NULL;
    o->del_chain = NULL;
    o->block_size = 1024;
    return o;
}

void bcore_btree_ps_s_discard( bcore_btree_ps_s* o )
{
    o->root = NULL;

    bcore_btree_node_ps_s* chain_beg = o->chain_beg;
    while( chain_beg )
    {
        bcore_btree_node_ps_s* new_beg = *( bcore_btree_node_ps_s** )( chain_beg + o->block_size );
        o->alloc( chain_beg, 0 );
        chain_beg = new_beg;
    }

    o->alloc( o, 0 );
}

bcore_btree_node_ps_s* bcore_btree_ps_s_new_node( bcore_btree_ps_s* o )
{
    if( o->del_chain )
    {
        bcore_btree_node_ps_s* new_node = o->del_chain;
        o->del_chain = new_node->parent;
        bcore_btree_node_ps_s_init( new_node );
        return new_node;
    }
    else
    {
        if( o->chain_ins == o->chain_end )
        {
            bcore_btree_node_ps_s* new_ptr = o->alloc( NULL, o->block_size * sizeof( bcore_btree_node_ps_s ) + sizeof( bcore_btree_node_ps_s* ) );
            if( !o->chain_beg )
            {
                o->chain_beg = new_ptr;
            }
            else
            {
                ( ( bcore_btree_node_ps_s** )( o->chain_end ) )[ 0 ] = new_ptr;
            }
            o->chain_ins = new_ptr;
            o->chain_end = new_ptr + o->block_size;
            *( bcore_btree_node_ps_s** )( o->chain_end ) = NULL;
        }
        bcore_btree_node_ps_s* new_node = o->chain_ins;
        bcore_btree_node_ps_s_init( new_node );
        o->chain_ins++;
        return new_node;
    }
}

// Deleted nodes are marked by setting all children NULL
// and chained together using pointer bcore_btree_node_ps_s.parent.
void bcore_btree_ps_s_delete_node( bcore_btree_ps_s* o, bcore_btree_node_ps_s* node )
{
    node->child0 = NULL;
    node->child1 = NULL;
    node->child2 = NULL;
    node->parent = o->del_chain;
    o->del_chain = node;
}

// recursively pushes an element into the tree
void bcore_btree_ps_s_push( bcore_btree_ps_s* o, bcore_btree_node_ps_s* node, bcore_btree_ps_kv_s* kv, bcore_btree_node_ps_s* child0, bcore_btree_node_ps_s* child1 )
{
    if( bcore_btree_node_ps_s_is_full( node ) )
    {
        bcore_btree_node_ps_s* l_node = node;
        bcore_btree_node_ps_s* r_node = bcore_btree_ps_s_new_node( o );
        bcore_btree_ps_kv_s root_kv;

        if( kv->key < node->kv1.key )
        {
            root_kv        = l_node->kv1;
            r_node->kv1    = l_node->kv2;
            r_node->child0 = l_node->child1;
            r_node->child1 = l_node->child2;
            l_node->kv1    = *kv;
            l_node->child0 = child0;
            l_node->child1 = child1;
        }
        else if( kv->key > node->kv2.key )
        {
            root_kv        = l_node->kv2;
            r_node->kv1    = *kv;
            r_node->child0 = child0;
            r_node->child1 = child1;
        }
        else
        {
            root_kv        = *kv;
            r_node->kv1    = l_node->kv2;
            r_node->child1 = l_node->child2;
            r_node->child0 = child1;
            l_node->child1 = child0;
        }
        r_node->child2 = NULL;
        l_node->child2 = NULL;

        bcore_btree_node_ps_s_set_parent_child0( r_node );
        bcore_btree_node_ps_s_set_parent_child1( r_node );
        bcore_btree_node_ps_s_set_parent_child0( l_node );
        bcore_btree_node_ps_s_set_parent_child1( l_node );

        if( l_node->parent )
        {
            bcore_btree_ps_s_push( o, l_node->parent, &root_kv, l_node, r_node );
        }
        else
        {
            o->root = bcore_btree_ps_s_new_node( o );
            o->root->kv1    = root_kv;
            o->root->child0 = l_node;
            o->root->child1 = r_node;
            l_node->parent  = o->root;
            r_node->parent  = o->root;
        }
    }
    else
    {
        if( kv->key < node->kv1.key )
        {
            node->kv2    = node->kv1;
            node->kv1    = *kv;
            node->child2 = node->child1;
            node->child1 = child1;
            node->child0 = child0;
            bcore_btree_node_ps_s_set_parent_child0( node );
            bcore_btree_node_ps_s_set_parent_child1( node );
            bcore_btree_node_ps_s_set_parent_child2( node );
        }
        else
        {
            node->kv2    = *kv;
            node->child2 = child1;
            node->child1 = child0;
            bcore_btree_node_ps_s_set_parent_child1( node );
            bcore_btree_node_ps_s_set_parent_child2( node );
        }
    }
}

// Recursively pulls an element from a non-leaf into an empty child node
void bcore_btree_ps_s_pull( bcore_btree_ps_s* o, bcore_btree_node_ps_s* node )
{
    if( bcore_btree_node_ps_s_is_empty( node->child0 ) )
    {
        if( bcore_btree_node_ps_s_is_full( node->child1 ) )
        {
            node->child0->kv1    = node->kv1;
            node->child0->child1 = node->child1->child0;
            bcore_btree_node_ps_s_set_parent_child1( node->child0 );
            node->kv1            = node->child1->kv1;
            node->child1->kv1    = node->child1->kv2;
            node->child1->child0 = node->child1->child1;
            node->child1->child1 = node->child1->child2;
            node->child1->child2 = NULL;
        }
        else if( bcore_btree_node_ps_s_is_full( node ) )
        {
            node->child1->kv2    = node->child1->kv1;
            node->child1->kv1    = node->kv1;
            node->child1->child2 = node->child1->child1;
            node->child1->child1 = node->child1->child0;
            node->child1->child0 = node->child0->child0;
            bcore_btree_node_ps_s_set_parent_child0( node->child1 );
            bcore_btree_ps_s_delete_node( o, node->child0 );
            node->kv1    = node->kv2;
            node->child0 = node->child1;
            node->child1 = node->child2;
            node->child2 = NULL;
        }
        else
        {
            node->child1->kv2    = node->child1->kv1;
            node->child1->kv1    = node->kv1;
            node->child1->child2 = node->child1->child1;
            node->child1->child1 = node->child1->child0;
            node->child1->child0 = node->child0->child0;
            bcore_btree_node_ps_s_set_parent_child0( node->child1 );
            bcore_btree_ps_s_delete_node( o, node->child0 );
            node->child0 = node->child1;
            node->child1 = node->child2 = NULL;
            if( node->parent )
            {
                bcore_btree_ps_s_pull( o, node->parent );
            }
            else
            {
                o->root = node->child0;
                o->root->parent = NULL;
                bcore_btree_ps_s_delete_node( o, node );
            }
        }
    }
    else if( bcore_btree_node_ps_s_is_empty( node->child1 ) )
    {
        if( bcore_btree_node_ps_s_is_full( node->child0 ) )
        {
            node->child1->kv1    = node->kv1;
            node->child1->child1 = node->child1->child0;
            node->child1->child0 = node->child0->child2;
            bcore_btree_node_ps_s_set_parent_child0( node->child1 );
            node->kv1            = node->child0->kv2;
            node->child0->child2 = NULL;
        }
        else if( bcore_btree_node_ps_s_is_full( node ) )
        {
            node->child0->kv2    = node->kv1;
            node->child0->child2 = node->child1->child0;
            bcore_btree_node_ps_s_set_parent_child2( node->child0 );
            bcore_btree_ps_s_delete_node( o, node->child1 );
            node->kv1    = node->kv2;
            node->child1 = node->child2;
            node->child2 = NULL;
        }
        else
        {
            node->child0->kv2    = node->kv1;
            node->child0->child2 = node->child1->child0;
            bcore_btree_node_ps_s_set_parent_child2( node->child0 );
            bcore_btree_ps_s_delete_node( o, node->child1 );
            node->child1 = node->child2 = NULL;
            if( node->parent )
            {
                bcore_btree_ps_s_pull( o, node->parent );
            }
            else
            {
                o->root = node->child0;
                o->root->parent = NULL;
                bcore_btree_ps_s_delete_node( o, node );
            }
        }
    }
    else // node->child2 is empty
    {
        if( bcore_btree_node_ps_s_is_full( node ) )
        {
            if( bcore_btree_node_ps_s_is_full( node->child1 ) )
            {
                node->child2->kv1 = node->kv2;
                node->child2->child1 = node->child2->child0;
                node->child2->child0 = node->child1->child2;
                bcore_btree_node_ps_s_set_parent_child0( node->child2 );
                node->kv2            = node->child1->kv2;
                node->child1->child2 = NULL;
            }
            else
            {
                node->child1->kv2    = node->kv2;
                node->child1->child2 = node->child2->child0;
                bcore_btree_node_ps_s_set_parent_child2( node->child1 );
                bcore_btree_ps_s_delete_node( o, node->child2 );
                node->child2         = NULL;
            }
        }
    }
}

bcore_btree_ps_val_t* bcore_btree_ps_s_val( const bcore_btree_ps_s* o, bcore_btree_ps_key_t key )
{
    if( !o )    return NULL;
    bcore_btree_node_ps_s* node = bcore_btree_node_ps_s_find( o->root, key );
    if( !node ) return NULL;
    if( node->kv1.key == key ) return &node->kv1.val;
    if( node->child2 && node->kv2.key == key ) return &node->kv2.val;
    return NULL;
}

int bcore_btree_ps_s_set( bcore_btree_ps_s* o, bcore_btree_ps_key_t key, bcore_btree_ps_val_t val )
{
    if( !o ) return -2;
    if( !o->root )
    {
        o->root = bcore_btree_ps_s_new_node( o );
        o->root->child0 = o->root->child1 = BNUL_PS;
        o->root->child2 = NULL;
        o->root->kv1.key = key;
        o->root->kv1.val = val;
        return 1;
    }

    bcore_btree_node_ps_s* node = bcore_btree_node_ps_s_find( o->root, key );
    if( !node ) return -2;

    if( node->kv1.key == key )
    {
        if( node->kv1.val == val ) return 0;
        node->kv1.val = val;
        return -1;
    }
    else if( node->child2 && node->kv2.key == key )
    {
        if( node->kv2.val == val ) return 0;
        node->kv2.val = val;
        return -1;
    }
    else
    {
        bcore_btree_ps_kv_s kv = { key, val };
        bcore_btree_ps_s_push( o, node, &kv, BNUL_PS, BNUL_PS );
        return 1;
    }
}

int bcore_btree_ps_s_remove( bcore_btree_ps_s* o, bcore_btree_ps_key_t key )
{
    if( !o       ) return -1;
    if( !o->root ) return  0;
    bcore_btree_node_ps_s* node = bcore_btree_node_ps_s_find( o->root, key );
    if( !node    ) return -1;

    if( node->kv1.key == key )
    {
        if( !bcore_btree_node_ps_s_is_leaf( node ) )
        {
            bcore_btree_node_ps_s* trace = node->child0;
            while( !bcore_btree_node_ps_s_is_leaf( trace ) ) trace = ( trace->child2 ) ? trace->child2 : trace->child1;
            if( bcore_btree_node_ps_s_is_full( trace ) )
            {
                node->kv1   = trace->kv2;
                trace->child2 = NULL;
            }
            else
            {
                node->kv1 = trace->kv1;
                trace->child1 = trace->child2 = NULL;
                bcore_btree_ps_s_pull( o, trace->parent );
            }
        }
        else if( bcore_btree_node_ps_s_is_full( node ) )
        {
            node->kv1 = node->kv2;
            node->child2 = NULL;
        }
        else
        {
            node->child1 = node->child2 = NULL;
            if( node->parent )
            {
                bcore_btree_ps_s_pull( o, node->parent );
            }
            else
            {
                bcore_btree_ps_s_delete_node( o, node );
                o->root = NULL;
            }
        }
        return 1;
    }

    if( node->kv2.key == key )
    {
        if( !bcore_btree_node_ps_s_is_leaf( node ) )
        {
            bcore_btree_node_ps_s* trace = ( node->child2 ) ? node->child2 : node->child1;
            while( !bcore_btree_node_ps_s_is_leaf( trace ) ) trace = trace->child0;
            if( bcore_btree_node_ps_s_is_full( trace ) )
            {
                node->kv2     = trace->kv1;
                trace->kv1    = trace->kv2;
                trace->child2 = NULL;
            }
            else
            {
                node->kv2   = trace->kv1;
                trace->child1 = trace->child2 = NULL;
                bcore_btree_ps_s_pull( o, trace->parent );
            }
        }
        else if( bcore_btree_node_ps_s_is_full( node ) )
        {
            node->child2 = NULL;
        }
        else
        {
            node->child1 = node->child2 = NULL;
            if( node->parent )
            {
                bcore_btree_ps_s_pull( o, node->parent );
            }
            else
            {
                bcore_btree_ps_s_delete_node( o, node );
                o->root = NULL;
            }
        }
        return 1;
    }
    return 0;
}

void bcore_btree_ps_s_run( const bcore_btree_ps_s* o, void(*func)( vd_t arg, bcore_btree_ps_key_t key, bcore_btree_ps_val_t val ), vd_t arg )
{
    bcore_btree_node_ps_s_run( o->root, func, arg );
}

uz_t bcore_btree_ps_s_count( const bcore_btree_ps_s* o, bl_t(*func)( vd_t arg, bcore_btree_ps_key_t key, bcore_btree_ps_val_t val ), vd_t arg )
{
    return bcore_btree_node_ps_s_count( o->root, func, arg );
}

uz_t bcore_btree_ps_s_sum( const bcore_btree_ps_s* o, bl_t(*func)( vd_t arg, bcore_btree_ps_key_t key, bcore_btree_ps_val_t val ), vd_t arg )
{
    return bcore_btree_node_ps_s_sum( o->root, func, arg );
}

uz_t bcore_btree_ps_s_depth( const bcore_btree_ps_s* o )
{
    return bcore_btree_node_ps_s_depth( o->root );
}

st_s* bcore_btree_ps_s_show( bcore_btree_ps_s* o )
{
    bcore_btree_node_ps_s_check_consistency( o->root );
    st_s* s = bcore_btree_node_ps_s_show( o->root, 0 );
    st_s_pushf( s, "\n" );
    return s;
}

st_s* bcore_btree_ps_s_status( bcore_btree_ps_s* o )
{
    uz_t blocks = 0;
    uz_t nodes = 0;
    uz_t deleted_nodes = 0;
    if( o->chain_beg )
    {
        bcore_btree_node_ps_s* chain_beg = o->chain_beg;
        while( chain_beg )
        {
            chain_beg = *( bcore_btree_node_ps_s** )( chain_beg + o->block_size );
            blocks++;
        }
        nodes = blocks * o->block_size - ( o->chain_end - o->chain_ins );
    }
    if( o->del_chain )
    {
        bcore_btree_node_ps_s* del_chain = o->del_chain;
        while( del_chain )
        {
            del_chain = del_chain->parent;
            deleted_nodes++;
        }
    }

    st_s* string = st_s_create();
    uz_t used_nodes = nodes - deleted_nodes;
    st_s_pushf( string, "keys ........... %lu\n", bcore_btree_node_ps_s_keys( o->root ) );
    st_s_pushf( string, "nodes .......... %lu\n", used_nodes );
    st_s_pushf( string, "keys/nodes ..... %5.4f\n", used_nodes > 0 ? ( double )( bcore_btree_node_ps_s_keys( o->root ) ) / used_nodes : 0 );
    st_s_pushf( string, "depth .......... %lu\n", bcore_btree_node_ps_s_depth( o->root ) );
    st_s_pushf( string, "block size ..... %lu\n", o->block_size );
    st_s_pushf( string, "blocks ......... %lu\n", blocks );
    st_s_pushf( string, "deleted nodes .. %lu\n", deleted_nodes );
    return string;
}

static st_s* btree_ps_s_selftest( void )
{
    st_s* log = st_s_createf( "== btree_ps_s_selftest " );
    st_s_push_char_n( log, '=', 80 - log->size );
    st_s_push_char( log, '\n' );
    bcore_btree_ps_s* t = bcore_btree_ps_s_create( NULL );
    const uz_t cycles = 200000;

    bcore_btree_ps_kv_s* kvbuf = bcore_alloc( NULL, cycles * sizeof( bcore_btree_ps_kv_s ) );
    uz_t kvbuf_size = 0;

    clock_t time = clock();
    st_s_pushf( log, "Mixed access: " );

    {
        u3_t rval1 = 1;
        u3_t rval2 = 12345;
        for( uz_t i = 0; i < cycles; i++ )
        {
            rval1 = bcore_xsg_u2( rval1 );
            rval2 = bcore_xsg_u2( rval2 );
            bcore_btree_ps_kv_s kv;
            kv.key = ( bcore_btree_ps_key_t )(intptr_t)rval1;
            kv.val = ( bcore_btree_ps_val_t )(intptr_t)rval2;
            kvbuf[ kvbuf_size++ ] = kv;

            // set
            bcore_btree_ps_s_set( t, kv.key, kv.val );

            // retrieve
            rval1 = bcore_xsg_u2( rval1 );
            kv = kvbuf[ rval1 % kvbuf_size ];
            bcore_btree_ps_val_t* val_ptr = bcore_btree_ps_s_val( t, kv.key );
            if( kv.val != *val_ptr ) ERR( "value mismatch (%lu vs %lu)", kv.val, *val_ptr );

            // delete
            rval1 = bcore_xsg_u2( rval1 );
            if( ( ( rval1 >> 10 ) & 1 ) == 1 )
            {
                rval1 = bcore_xsg_u2( rval1 );
                uz_t idx = rval1 % kvbuf_size;
                bcore_btree_ps_kv_s kv = kvbuf[ idx ];
                if( !bcore_btree_ps_s_val( t, kv.key ) )  ERR( "key (%lu) not found", kv.key );
                bcore_btree_ps_s_remove( t, kv.key );
                if( bcore_btree_ps_s_val( t, kv.key ) )  ERR( "deleted key still exists (%lu)", kv.key );
                kvbuf_size--;
                kvbuf[ idx ] = kvbuf[ kvbuf_size ];
            }
        }
    }

    time = clock() - time;
    st_s_pushf( log, "(%5.3fs)\n", ( double )time/CLOCKS_PER_SEC );

    bcore_btree_node_ps_s_check_consistency( t->root );
    st_s_push_st_d( log, bcore_btree_ps_s_status( t ) );

    time = clock();
    st_s_pushf( log, "\nRead-access of %lu keys: ", kvbuf_size );
    uz_t read_cycles = 20;
    for( uz_t j = 0; j < read_cycles; j++ )
    {
        for( uz_t i = 0; i < kvbuf_size; i++ )
        {
            if( *bcore_btree_ps_s_val( t, kvbuf[ i ].key ) != kvbuf[ i ].val ) ERR( "value mismatch (%lu vs %lu)", kvbuf[ i ].key, kvbuf[ i ].val );
        }
    }
    time = clock() - time;
    st_s_pushf( log, "(%gs per access)\n", ( ( double )time/CLOCKS_PER_SEC ) / ( kvbuf_size * read_cycles ) );

    st_s_pushf( log, "\n" );
    st_s_pushf( log, "Removal: " );
    time = clock();
    while( kvbuf_size )
    {
        kvbuf_size--;
        bcore_btree_ps_kv_s kv = kvbuf[ kvbuf_size ];
        if( !bcore_btree_ps_s_val( t, kv.key ) )  ERR( "key (%lu) not found", kv.key );
        bcore_btree_ps_s_remove( t, kv.key );
        if( bcore_btree_ps_s_val( t, kv.key ) )  ERR( "deleted key still exists (%lu)", kv.key );
    }
    time = clock() - time;
    st_s_pushf( log, "(%5.3fs)\n", ( double )time/CLOCKS_PER_SEC );

    if( t->root ) ERR( "root still exists" );

    st_s_push_st_d( log, bcore_btree_ps_s_status( t ) );

    bcore_btree_ps_s_discard( t );
    bcore_alloc( kvbuf, 0 );
    st_s_push_char_n( log, '=', 80 );
    st_s_push_char( log, '\n' );
    return log;
}

/**********************************************************************************************************************/
/**********************************************************************************************************************/
/// btree_pp
/**********************************************************************************************************************/
/**********************************************************************************************************************/

void bcore_btree_pp_kv_s_init( bcore_btree_pp_kv_s* o )
{
    o->key = 0;
    o->val = 0;
}

/**********************************************************************************************************************/

/** Node of a 2-3 btree.
 *  Child pointers can have one of three states:
 *  NULL: corresponding key-value pair is not used (for normal nodes this state only applies to child2)
 *  BNUL_PS: node is a leaf
 *  else: node has children
 */
typedef struct bcore_btree_node_pp_s
{
    bcore_btree_pp_kv_s  kv1;
    bcore_btree_pp_kv_s  kv2;
    struct bcore_btree_node_pp_s* parent;
    struct bcore_btree_node_pp_s* child0;
    struct bcore_btree_node_pp_s* child1;
    struct bcore_btree_node_pp_s* child2;
} bcore_btree_node_pp_s;

/// children of leaf-nodes point to bcore_btree_node_pp_s_null
bcore_btree_node_pp_s bcore_btree_node_pp_s_null_g = { { 0, 0 }, { 0, 0 }, NULL, NULL, NULL, NULL };
#define BNUL_PP ( &bcore_btree_node_pp_s_null_g )

void bcore_btree_node_pp_s_init( bcore_btree_node_pp_s* o )
{
    bcore_btree_pp_kv_s_init( &o->kv1 );
    bcore_btree_pp_kv_s_init( &o->kv2 );
    o->parent = o->child0 = o->child1 = o->child2 = NULL;
}

bcore_btree_node_pp_s* bcore_btree_node_pp_s_find( bcore_btree_node_pp_s* root, bcore_btree_pp_key_t key )
{
    if( !root ) return NULL;
    bcore_btree_node_pp_s* node = NULL;
    while( root->child0 != BNUL_PP && root != node )
    {
        node = root;
        root = ( key < node->kv1.key )                                         ? node->child0 :
               ( !node->child2 && key > node->kv1.key )                        ? node->child1 :
               (  node->child2 && key > node->kv2.key )                        ? node->child2 :
               (  node->child2 && key > node->kv1.key && key < node->kv2.key ) ? node->child1 : node;
    }
    return root;
}

bcore_btree_pp_kv_s* bcore_btree_node_pp_s_largest_below_equal( bcore_btree_node_pp_s* root, bcore_btree_pp_key_t key )
{
    if( !root ) return NULL;
    if( root->child0 == BNUL_PP )
    {
        if( ( root->child2 ) && key >= root->kv2.key ) return &root->kv2;
        return ( key >= root->kv1.key ) ? &root->kv1 : NULL;
    }
    else if( ( root->child2 ) && key >= root->kv2.key )
    {
        bcore_btree_pp_kv_s* branch_kv = bcore_btree_node_pp_s_largest_below_equal( root->child2, key );
        return ( branch_kv ) ? branch_kv : &root->kv2;
    }
    else if( key >= root->kv1.key )
    {
        bcore_btree_pp_kv_s* branch_kv = bcore_btree_node_pp_s_largest_below_equal( root->child1, key );
        return ( branch_kv ) ? branch_kv : &root->kv1;
    }
    else
    {
        return bcore_btree_node_pp_s_largest_below_equal( root->child0, key );
    }
}

void bcore_btree_node_pp_s_run( const bcore_btree_node_pp_s* root, void(*func)( vd_t arg, bcore_btree_pp_kv_s kv ), vd_t arg )
{
    if( !root ) return;
    if( !func ) return;
    if( root->child0 )
    {
        if( root->child0 != BNUL_PP ) bcore_btree_node_pp_s_run( root->child0, func, arg );
    }
    if( root->child1 )
    {
        func( arg, root->kv1 );
        if( root->child1 != BNUL_PP ) bcore_btree_node_pp_s_run( root->child1, func, arg );
    }
    if( root->child2 )
    {
        func( arg, root->kv2 );
        if( root->child2 != BNUL_PP ) bcore_btree_node_pp_s_run( root->child2, func, arg );
    }
}

uz_t bcore_btree_node_pp_s_count( const bcore_btree_node_pp_s* root, bl_t (*func)( vd_t arg, bcore_btree_pp_kv_s kv ), vd_t arg )
{
    uz_t count = 0;
    if( !root ) return count;
    if( root->child0 )
    {
        if( root->child0 != BNUL_PP ) count += bcore_btree_node_pp_s_count( root->child0, func, arg );
    }
    if( root->child1 )
    {
        count += ( func ) ? func( arg, root->kv1 ) : 1;
        if( root->child1 != BNUL_PP ) count += bcore_btree_node_pp_s_count( root->child1, func, arg );
    }
    if( root->child2 )
    {
        count += ( func ) ? func( arg, root->kv2 ) : 1;
        if( root->child2 != BNUL_PP ) count += bcore_btree_node_pp_s_count( root->child2, func, arg );
    }
    return count;
}

uz_t bcore_btree_node_pp_s_keys( bcore_btree_node_pp_s* root )
{
    if( !root || root == BNUL_PP ) return 0;
    uz_t keys = root->child2 ? 2 : 1;
    keys += bcore_btree_node_pp_s_keys( root->child0 );
    keys += bcore_btree_node_pp_s_keys( root->child1 );
    if( root->child2 ) keys += bcore_btree_node_pp_s_keys( root->child2 );
    return keys;
}

uz_t bcore_btree_node_pp_s_depth( bcore_btree_node_pp_s* root )
{
    if( !root || root == BNUL_PP ) return 0;
    return 1 + bcore_btree_node_pp_s_depth( root->child0 );
}

void bcore_btree_node_pp_s_set_parent_child0( bcore_btree_node_pp_s* o ) { if( o->child0 && o->child0 != BNUL_PP ) o->child0->parent = o; }
void bcore_btree_node_pp_s_set_parent_child1( bcore_btree_node_pp_s* o ) { if( o->child1 && o->child1 != BNUL_PP ) o->child1->parent = o; }
void bcore_btree_node_pp_s_set_parent_child2( bcore_btree_node_pp_s* o ) { if( o->child2 && o->child2 != BNUL_PP ) o->child2->parent = o; }
int  bcore_btree_node_pp_s_is_leaf(  bcore_btree_node_pp_s* o )          { return o->child0 == BNUL_PP; }
int  bcore_btree_node_pp_s_is_full(  bcore_btree_node_pp_s* o )          { return o->child2 != NULL; }
int  bcore_btree_node_pp_s_is_empty( bcore_btree_node_pp_s* o )          { return o->child1 == NULL; }

void bcore_btree_node_pp_s_check_consistency( bcore_btree_node_pp_s* o )
{
    if( bcore_btree_node_pp_s_null_g.kv1.key != 0    ) ERR( "bcore_btree_node_pp_s_null was modified" );
    if( bcore_btree_node_pp_s_null_g.kv1.val != 0    ) ERR( "bcore_btree_node_pp_s_null was modified" );
    if( bcore_btree_node_pp_s_null_g.kv2.key != 0    ) ERR( "bcore_btree_node_pp_s_null was modified" );
    if( bcore_btree_node_pp_s_null_g.kv2.val != 0    ) ERR( "bcore_btree_node_pp_s_null was modified" );
    if( bcore_btree_node_pp_s_null_g.parent  != NULL ) ERR( "bcore_btree_node_pp_s_null was modified" );
    if( bcore_btree_node_pp_s_null_g.child0  != NULL ) ERR( "bcore_btree_node_pp_s_null was modified" );
    if( bcore_btree_node_pp_s_null_g.child1  != NULL ) ERR( "bcore_btree_node_pp_s_null was modified" );
    if( bcore_btree_node_pp_s_null_g.child2  != NULL ) ERR( "bcore_btree_node_pp_s_null was modified" );

    if( !o ) return;
    if( bcore_btree_node_pp_s_is_empty( o ) ) ERR( "empty node" );
    if( o->child0 == NULL )       ERR( "deleted leaf" );
    if( o->child1 && o->child1 != BNUL_PP )
    {
        if( o != o->child0->parent ) ERR( "child0 incorrect parent" );
        if( o != o->child1->parent ) ERR( "child1 incorrect parent" );
        bcore_btree_node_pp_s_check_consistency( o->child0 );
        bcore_btree_node_pp_s_check_consistency( o->child1 );
        if(                      o->kv1.key <= o->child0->kv1.key ) ERR( "(%lu <= %lu)", o->kv1.key, o->child0->kv1.key );
        if( o->child0->child2 && o->kv1.key <= o->child0->kv2.key ) ERR( "(%lu <= %lu)", o->kv1.key, o->child0->kv2.key );
        if(                      o->kv1.key >= o->child1->kv1.key ) ERR( "(%lu >= %lu)", o->kv1.key, o->child1->kv1.key );
        if( o->child1->child2 && o->kv1.key >= o->child1->kv2.key ) ERR( "(%lu >= %lu)", o->kv1.key, o->child1->kv2.key );
    }
    if( o->child2 && o->child2 != BNUL_PP )
    {
        if( o->kv1.key >= o->kv2.key ) ERR( "(%lu >= %lu)", o->kv1.key, o->kv2.key );
        if( o != o->child2->parent ) ERR( "child2 incorrect parent" );
        bcore_btree_node_pp_s_check_consistency( o->child2 );
        if(                      o->kv2.key <= o->child1->kv1.key ) ERR( "(%lu <= %lu)", o->kv2.key, o->child1->kv1.key );
        if( o->child1->child2 && o->kv2.key <= o->child1->kv2.key ) ERR( "(%lu <= %lu)", o->kv2.key, o->child1->kv2.key );
        if(                      o->kv2.key >= o->child2->kv1.key ) ERR( "(%lu >= %lu)", o->kv2.key, o->child2->kv1.key );
        if( o->child2->child2 && o->kv2.key >= o->child2->kv2.key ) ERR( "(%lu >= %lu)", o->kv2.key, o->child2->kv2.key );
    }
}

st_s* bcore_btree_node_pp_s_show( bcore_btree_node_pp_s* o, uz_t depth )
{
    st_s* string = st_s_create();
    if( !o )
    {
        st_s_pushf( string, "()\n" );
        return string;
    }
    if( o->child1 )
    {
        if( o->child0 && o->child0 != BNUL_PP ) st_s_push_st_d( string, bcore_btree_node_pp_s_show( o->child0, depth + 1 ) );
        for( uz_t i = 0; i < depth; i++ ) st_s_pushf( string, "    " );
        st_s_pushf( string, "(%lu,%lu)\n", ( int64_t )(intptr_t)o->kv1.key, ( int64_t )(intptr_t)o->kv1.val );
        if( o->child1 && o->child1 != BNUL_PP ) st_s_pushf( string, "    " );
    }
    if( o->child2 )
    {
        for( uz_t i = 0; i < depth; i++ ) st_s_pushf( string, "    " );
        st_s_pushf( string, "(%lu,%lu)\n", ( int64_t )(intptr_t)o->kv2.key, ( int64_t )(intptr_t)o->kv2.val );
        if( o->child2 && o->child2 != BNUL_PP ) st_s_push_st_d( string, bcore_btree_node_pp_s_show( o->child2, depth + 1 ) );
    }
    return string;
}

/**********************************************************************************************************************/

struct bcore_btree_pp_s
{
    bcore_btree_node_pp_s* root;
    bcore_btree_node_pp_s* chain_beg; // begin of chain of blocks of bcore_btree_node_pp_s[] with last element being pointer to next block
    bcore_btree_node_pp_s* chain_end; // end of chain of blocks
    bcore_btree_node_pp_s* chain_ins; // pointer for new insertions
    bcore_btree_node_pp_s* del_chain; // chain of deleted elements (preferably used by new insertions)
    vd_t (*alloc)( vd_t, uz_t size ); // alloc function
    uz_t   block_size;
};

bcore_btree_pp_s* bcore_btree_pp_s_create( vd_t (*alloc)( vd_t, uz_t size ) )
{
    bcore_btree_pp_s* o = NULL;
    if( alloc )
    {
        o = alloc( NULL, sizeof( bcore_btree_pp_s ) );
        o->alloc = alloc;
    }
    else
    {
        o = bcore_alloc( NULL, sizeof( bcore_btree_pp_s ) );
        o->alloc = bcore_alloc;
    }
    o->root      = NULL;
    o->chain_beg = NULL;
    o->chain_end = NULL;
    o->chain_ins = NULL;
    o->del_chain = NULL;
    o->block_size = 1024;
    return o;
}

void bcore_btree_pp_s_discard( bcore_btree_pp_s* o )
{
    o->root = NULL;

    bcore_btree_node_pp_s* chain_beg = o->chain_beg;
    while( chain_beg )
    {
        bcore_btree_node_pp_s* new_beg = *( bcore_btree_node_pp_s** )( chain_beg + o->block_size );
        o->alloc( chain_beg, 0 );
        chain_beg = new_beg;
    }

    o->alloc( o, 0 );
}

bcore_btree_node_pp_s* bcore_btree_pp_s_new_node( bcore_btree_pp_s* o )
{
    if( o->del_chain )
    {
        bcore_btree_node_pp_s* new_node = o->del_chain;
        o->del_chain = new_node->parent;
        bcore_btree_node_pp_s_init( new_node );
        return new_node;
    }
    else
    {
        if( o->chain_ins == o->chain_end )
        {
            bcore_btree_node_pp_s* new_ptr = o->alloc( NULL, o->block_size * sizeof( bcore_btree_node_pp_s ) + sizeof( bcore_btree_node_pp_s* ) );
            if( !o->chain_beg )
            {
                o->chain_beg = new_ptr;
            }
            else
            {
                ( ( bcore_btree_node_pp_s** )( o->chain_end ) )[ 0 ] = new_ptr;
            }
            o->chain_ins = new_ptr;
            o->chain_end = new_ptr + o->block_size;
            *( bcore_btree_node_pp_s** )( o->chain_end ) = NULL;
        }
        bcore_btree_node_pp_s* new_node = o->chain_ins;
        bcore_btree_node_pp_s_init( new_node );
        o->chain_ins++;
        return new_node;
    }
}

// Deleted nodes are marked by setting all children NULL
// and chained together using pointer bcore_btree_node_pp_s.parent.
void bcore_btree_pp_s_delete_node( bcore_btree_pp_s* o, bcore_btree_node_pp_s* node )
{
    node->child0 = NULL;
    node->child1 = NULL;
    node->child2 = NULL;
    node->parent = o->del_chain;
    o->del_chain = node;
}

// recursively pushes an element into the tree
void bcore_btree_pp_s_push( bcore_btree_pp_s* o, bcore_btree_node_pp_s* node, bcore_btree_pp_kv_s* kv, bcore_btree_node_pp_s* child0, bcore_btree_node_pp_s* child1 )
{
    if( bcore_btree_node_pp_s_is_full( node ) )
    {
        bcore_btree_node_pp_s* l_node = node;
        bcore_btree_node_pp_s* r_node = bcore_btree_pp_s_new_node( o );
        bcore_btree_pp_kv_s root_kv;

        if( kv->key < node->kv1.key )
        {
            root_kv        = l_node->kv1;
            r_node->kv1    = l_node->kv2;
            r_node->child0 = l_node->child1;
            r_node->child1 = l_node->child2;
            l_node->kv1    = *kv;
            l_node->child0 = child0;
            l_node->child1 = child1;
        }
        else if( kv->key > node->kv2.key )
        {
            root_kv        = l_node->kv2;
            r_node->kv1    = *kv;
            r_node->child0 = child0;
            r_node->child1 = child1;
        }
        else
        {
            root_kv        = *kv;
            r_node->kv1    = l_node->kv2;
            r_node->child1 = l_node->child2;
            r_node->child0 = child1;
            l_node->child1 = child0;
        }
        r_node->child2 = NULL;
        l_node->child2 = NULL;

        bcore_btree_node_pp_s_set_parent_child0( r_node );
        bcore_btree_node_pp_s_set_parent_child1( r_node );
        bcore_btree_node_pp_s_set_parent_child0( l_node );
        bcore_btree_node_pp_s_set_parent_child1( l_node );

        if( l_node->parent )
        {
            bcore_btree_pp_s_push( o, l_node->parent, &root_kv, l_node, r_node );
        }
        else
        {
            o->root = bcore_btree_pp_s_new_node( o );
            o->root->kv1    = root_kv;
            o->root->child0 = l_node;
            o->root->child1 = r_node;
            l_node->parent  = o->root;
            r_node->parent  = o->root;
        }
    }
    else
    {
        if( kv->key < node->kv1.key )
        {
            node->kv2    = node->kv1;
            node->kv1    = *kv;
            node->child2 = node->child1;
            node->child1 = child1;
            node->child0 = child0;
            bcore_btree_node_pp_s_set_parent_child0( node );
            bcore_btree_node_pp_s_set_parent_child1( node );
            bcore_btree_node_pp_s_set_parent_child2( node );
        }
        else
        {
            node->kv2    = *kv;
            node->child2 = child1;
            node->child1 = child0;
            bcore_btree_node_pp_s_set_parent_child1( node );
            bcore_btree_node_pp_s_set_parent_child2( node );
        }
    }
}

// Recursively pulls an element from a non-leaf into an empty child node
void bcore_btree_pp_s_pull( bcore_btree_pp_s* o, bcore_btree_node_pp_s* node )
{
    if( bcore_btree_node_pp_s_is_empty( node->child0 ) )
    {
        if( bcore_btree_node_pp_s_is_full( node->child1 ) )
        {
            node->child0->kv1    = node->kv1;
            node->child0->child1 = node->child1->child0;
            bcore_btree_node_pp_s_set_parent_child1( node->child0 );
            node->kv1            = node->child1->kv1;
            node->child1->kv1    = node->child1->kv2;
            node->child1->child0 = node->child1->child1;
            node->child1->child1 = node->child1->child2;
            node->child1->child2 = NULL;
        }
        else if( bcore_btree_node_pp_s_is_full( node ) )
        {
            node->child1->kv2    = node->child1->kv1;
            node->child1->kv1    = node->kv1;
            node->child1->child2 = node->child1->child1;
            node->child1->child1 = node->child1->child0;
            node->child1->child0 = node->child0->child0;
            bcore_btree_node_pp_s_set_parent_child0( node->child1 );
            bcore_btree_pp_s_delete_node( o, node->child0 );
            node->kv1    = node->kv2;
            node->child0 = node->child1;
            node->child1 = node->child2;
            node->child2 = NULL;
        }
        else
        {
            node->child1->kv2    = node->child1->kv1;
            node->child1->kv1    = node->kv1;
            node->child1->child2 = node->child1->child1;
            node->child1->child1 = node->child1->child0;
            node->child1->child0 = node->child0->child0;
            bcore_btree_node_pp_s_set_parent_child0( node->child1 );
            bcore_btree_pp_s_delete_node( o, node->child0 );
            node->child0 = node->child1;
            node->child1 = node->child2 = NULL;
            if( node->parent )
            {
                bcore_btree_pp_s_pull( o, node->parent );
            }
            else
            {
                o->root = node->child0;
                o->root->parent = NULL;
                bcore_btree_pp_s_delete_node( o, node );
            }
        }
    }
    else if( bcore_btree_node_pp_s_is_empty( node->child1 ) )
    {
        if( bcore_btree_node_pp_s_is_full( node->child0 ) )
        {
            node->child1->kv1    = node->kv1;
            node->child1->child1 = node->child1->child0;
            node->child1->child0 = node->child0->child2;
            bcore_btree_node_pp_s_set_parent_child0( node->child1 );
            node->kv1            = node->child0->kv2;
            node->child0->child2 = NULL;
        }
        else if( bcore_btree_node_pp_s_is_full( node ) )
        {
            node->child0->kv2    = node->kv1;
            node->child0->child2 = node->child1->child0;
            bcore_btree_node_pp_s_set_parent_child2( node->child0 );
            bcore_btree_pp_s_delete_node( o, node->child1 );
            node->kv1    = node->kv2;
            node->child1 = node->child2;
            node->child2 = NULL;
        }
        else
        {
            node->child0->kv2    = node->kv1;
            node->child0->child2 = node->child1->child0;
            bcore_btree_node_pp_s_set_parent_child2( node->child0 );
            bcore_btree_pp_s_delete_node( o, node->child1 );
            node->child1 = node->child2 = NULL;
            if( node->parent )
            {
                bcore_btree_pp_s_pull( o, node->parent );
            }
            else
            {
                o->root = node->child0;
                o->root->parent = NULL;
                bcore_btree_pp_s_delete_node( o, node );
            }
        }
    }
    else // node->child2 is empty
    {
        if( bcore_btree_node_pp_s_is_full( node ) )
        {
            if( bcore_btree_node_pp_s_is_full( node->child1 ) )
            {
                node->child2->kv1 = node->kv2;
                node->child2->child1 = node->child2->child0;
                node->child2->child0 = node->child1->child2;
                bcore_btree_node_pp_s_set_parent_child0( node->child2 );
                node->kv2            = node->child1->kv2;
                node->child1->child2 = NULL;
            }
            else
            {
                node->child1->kv2    = node->kv2;
                node->child1->child2 = node->child2->child0;
                bcore_btree_node_pp_s_set_parent_child2( node->child1 );
                bcore_btree_pp_s_delete_node( o, node->child2 );
                node->child2         = NULL;
            }
        }
    }
}

bl_t bcore_btree_pp_s_exists( const bcore_btree_pp_s* o, bcore_btree_pp_key_t key )
{
    if( !o ) return false;
    bcore_btree_node_pp_s* node = bcore_btree_node_pp_s_find( o->root, key );
    if( !node ) return false;
    if( node->kv1.key == key ) return true;
    if( node->child2 && node->kv2.key == key ) return true;
    return false;
}

bcore_btree_pp_kv_s* bcore_btree_pp_s_largest_below_equal( const bcore_btree_pp_s* o, bcore_btree_pp_key_t key )
{
    return bcore_btree_node_pp_s_largest_below_equal( o->root, key );
}

bcore_btree_pp_val_t* bcore_btree_pp_s_val( const bcore_btree_pp_s* o, bcore_btree_pp_key_t key )
{
    if( !o )    return NULL;
    bcore_btree_node_pp_s* node = bcore_btree_node_pp_s_find( o->root, key );
    if( !node ) return NULL;
    if( node->kv1.key == key ) return &node->kv1.val;
    if( node->child2 && node->kv2.key == key ) return &node->kv2.val;
    return NULL;
}

int bcore_btree_pp_s_set( bcore_btree_pp_s* o, bcore_btree_pp_key_t key, bcore_btree_pp_val_t val )
{
    if( !o ) return -2;
    if( !o->root )
    {
        o->root = bcore_btree_pp_s_new_node( o );
        o->root->child0 = o->root->child1 = BNUL_PP;
        o->root->child2 = NULL;
        o->root->kv1.key = key;
        o->root->kv1.val = val;
        return 1;
    }

    bcore_btree_node_pp_s* node = bcore_btree_node_pp_s_find( o->root, key );
    if( !node ) return -2;

    if( node->kv1.key == key )
    {
        if( node->kv1.val == val ) return 0;
        node->kv1.val = val;
        return -1;
    }
    else if( node->child2 && node->kv2.key == key )
    {
        if( node->kv2.val == val ) return 0;
        node->kv2.val = val;
        return -1;
    }
    else
    {
        bcore_btree_pp_kv_s kv = { key, val };
        bcore_btree_pp_s_push( o, node, &kv, BNUL_PP, BNUL_PP );
        return 1;
    }
}

int bcore_btree_pp_s_remove( bcore_btree_pp_s* o, bcore_btree_pp_key_t key )
{
    if( !o       ) return -1;
    if( !o->root ) return  0;
    bcore_btree_node_pp_s* node = bcore_btree_node_pp_s_find( o->root, key );
    if( !node    ) return -1;

    if( node->kv1.key == key )
    {
        if( !bcore_btree_node_pp_s_is_leaf( node ) )
        {
            bcore_btree_node_pp_s* trace = node->child0;
            while( !bcore_btree_node_pp_s_is_leaf( trace ) ) trace = ( trace->child2 ) ? trace->child2 : trace->child1;
            if( bcore_btree_node_pp_s_is_full( trace ) )
            {
                node->kv1   = trace->kv2;
                trace->child2 = NULL;
            }
            else
            {
                node->kv1 = trace->kv1;
                trace->child1 = trace->child2 = NULL;
                bcore_btree_pp_s_pull( o, trace->parent );
            }
        }
        else if( bcore_btree_node_pp_s_is_full( node ) )
        {
            node->kv1 = node->kv2;
            node->child2 = NULL;
        }
        else
        {
            node->child1 = node->child2 = NULL;
            if( node->parent )
            {
                bcore_btree_pp_s_pull( o, node->parent );
            }
            else
            {
                bcore_btree_pp_s_delete_node( o, node );
                o->root = NULL;
            }
        }
        return 1;
    }

    if( node->kv2.key == key )
    {
        if( !bcore_btree_node_pp_s_is_leaf( node ) )
        {
            bcore_btree_node_pp_s* trace = ( node->child2 ) ? node->child2 : node->child1;
            while( !bcore_btree_node_pp_s_is_leaf( trace ) ) trace = trace->child0;
            if( bcore_btree_node_pp_s_is_full( trace ) )
            {
                node->kv2     = trace->kv1;
                trace->kv1    = trace->kv2;
                trace->child2 = NULL;
            }
            else
            {
                node->kv2   = trace->kv1;
                trace->child1 = trace->child2 = NULL;
                bcore_btree_pp_s_pull( o, trace->parent );
            }
        }
        else if( bcore_btree_node_pp_s_is_full( node ) )
        {
            node->child2 = NULL;
        }
        else
        {
            node->child1 = node->child2 = NULL;
            if( node->parent )
            {
                bcore_btree_pp_s_pull( o, node->parent );
            }
            else
            {
                bcore_btree_pp_s_delete_node( o, node );
                o->root = NULL;
            }
        }
        return 1;
    }
    return 0;
}

void bcore_btree_pp_s_run( const bcore_btree_pp_s* o, void(*func)( vd_t arg, bcore_btree_pp_kv_s kv ), vd_t arg )
{
    bcore_btree_node_pp_s_run( o->root, func, arg );
}

uz_t bcore_btree_pp_s_count( const bcore_btree_pp_s* o, bl_t(*func)( vd_t arg, bcore_btree_pp_kv_s kv ), vd_t arg )
{
    return bcore_btree_node_pp_s_count( o->root, func, arg );
}

uz_t bcore_btree_pp_s_depth( const bcore_btree_pp_s* o )
{
    return bcore_btree_node_pp_s_depth( o->root );
}

st_s* bcore_btree_pp_s_show( bcore_btree_pp_s* o )
{
    bcore_btree_node_pp_s_check_consistency( o->root );
    st_s* s = bcore_btree_node_pp_s_show( o->root, 0 );
    st_s_pushf( s, "\n" );
    return s;
}

st_s* bcore_btree_pp_s_status( bcore_btree_pp_s* o )
{
    uz_t blocks = 0;
    uz_t nodes = 0;
    uz_t deleted_nodes = 0;
    if( o->chain_beg )
    {
        bcore_btree_node_pp_s* chain_beg = o->chain_beg;
        while( chain_beg )
        {
            chain_beg = *( bcore_btree_node_pp_s** )( chain_beg + o->block_size );
            blocks++;
        }
        nodes = blocks * o->block_size - ( o->chain_end - o->chain_ins );
    }
    if( o->del_chain )
    {
        bcore_btree_node_pp_s* del_chain = o->del_chain;
        while( del_chain )
        {
            del_chain = del_chain->parent;
            deleted_nodes++;
        }
    }

    st_s* string = st_s_create();
    uz_t used_nodes = nodes - deleted_nodes;
    st_s_push_fa( string, "keys ........... #<uz_t>\n", bcore_btree_node_pp_s_keys( o->root ) );
    st_s_push_fa( string, "nodes .......... #<uz_t>\n", used_nodes );
    st_s_push_fa( string, "keys/nodes ..... #<f3_t>\n", used_nodes > 0 ? ( f3_t )( bcore_btree_node_pp_s_keys( o->root ) ) / used_nodes : 0 );
    st_s_push_fa( string, "depth .......... #<uz_t>\n", bcore_btree_node_pp_s_depth( o->root ) );
    st_s_push_fa( string, "block size ..... #<uz_t>\n", o->block_size );
    st_s_push_fa( string, "blocks ......... #<uz_t>\n", blocks );
    st_s_push_fa( string, "deleted nodes .. #<uz_t>\n", deleted_nodes );
    return string;
}

static st_s* btree_pp_s_selftest( void )
{
    st_s* log = st_s_createf( "== btree_pp_s_selftest " );
    st_s_push_char_n( log, '=', 80 - log->size );
    st_s_push_char( log, '\n' );
    bcore_btree_pp_s* t = bcore_btree_pp_s_create( NULL );
    const uz_t cycles = 200000;

    bcore_btree_pp_kv_s* kvbuf = bcore_alloc( NULL, cycles * sizeof( bcore_btree_pp_kv_s ) );
    uz_t kvbuf_size = 0;

    clock_t time = clock();
    st_s_pushf( log, "Mixed access: " );

    {
        u3_t rval1 = 1;
        u3_t rval2 = 12345;
        for( uz_t i = 0; i < cycles; i++ )
        {
            bcore_btree_pp_kv_s kv;

            // use multiple of 256 to allow nearest_... tests
            // The left-shift can produce key repetitions on 32bit systems. The loop below ensures stored keys are not repeated.
            while( bcore_btree_pp_s_exists( t, ( kv.key = ( bcore_btree_pp_key_t )(intptr_t)( ( rval1 = bcore_xsg_u2( rval1 ) ) << 8 ) ) ) );
            kv.val = ( bcore_btree_pp_val_t )(intptr_t)( rval2 = bcore_xsg_u2( rval2 ) );
            kvbuf[ kvbuf_size++ ] = kv;

            // set
            bcore_btree_pp_s_set( t, kv.key, kv.val );

            // retrieve
            rval1 = bcore_xsg_u2( rval1 );
            kv = kvbuf[ rval1 % kvbuf_size ];
            bcore_btree_pp_val_t* val_ptr = bcore_btree_pp_s_val( t, kv.key );
            if( kv.val != *val_ptr ) ERR( "value mismatch (%lu vs %lu)", kv.val, *val_ptr );

            // delete
            rval1 = bcore_xsg_u2( rval1 );
            if( ( ( rval1 >> 10 ) & 1 ) == 1 )
            {
                rval1 = bcore_xsg_u2( rval1 );
                uz_t idx = rval1 % kvbuf_size;
                bcore_btree_pp_kv_s kv = kvbuf[ idx ];
                if( !bcore_btree_pp_s_val( t, kv.key ) )  ERR( "key (%lu) not found", kv.key );
                bcore_btree_pp_s_remove( t, kv.key );
                if( bcore_btree_pp_s_val( t, kv.key ) )  ERR( "deleted key still exists (%lu)", kv.key );
                kvbuf_size--;
                kvbuf[ idx ] = kvbuf[ kvbuf_size ];
            }
        }
    }

    time = clock() - time;
    st_s_pushf( log, "(%5.3fs)\n", ( double )time/CLOCKS_PER_SEC );

    bcore_btree_node_pp_s_check_consistency( t->root );
    st_s_push_st_d( log, bcore_btree_pp_s_status( t ) );

    time = clock();
    st_s_pushf( log, "\nRead-access of %lu keys: ", kvbuf_size );
    uz_t read_cycles = 20;
    for( uz_t j = 0; j < read_cycles; j++ )
    {
        for( uz_t i = 0; i < kvbuf_size; i++ )
        {
            if( *bcore_btree_pp_s_val( t, kvbuf[ i ].key ) != kvbuf[ i ].val ) ERR( "value mismatch (%lu vs %lu)", kvbuf[ i ].key, kvbuf[ i ].val );
        }
    }
    time = clock() - time;
    st_s_pushf( log, "(%gs per access)\n", ( ( double )time/CLOCKS_PER_SEC ) / ( kvbuf_size * read_cycles ) );

    time = clock();
    st_s_pushf( log, "largest_below_equal - test: ", kvbuf_size );
    if( bcore_btree_pp_s_largest_below_equal( t, (bcore_btree_pp_key_t)1 ) != NULL ) ERR( "largest_below_equal( 1 ) failed" );
    for( uz_t j = 0; j < 10; j++ )
    {
        for( uz_t i = 0; i < kvbuf_size; i++ )
        {
            bcore_btree_pp_key_t key0 = ( u0_t* )kvbuf[ i ].key + j;
            bcore_btree_pp_kv_s* kv1 = bcore_btree_pp_s_largest_below_equal( t, key0 );
            if( !kv1 ) ERR( "largest_below_equal( %lu ) failed", key0 );
            // stored keys have a minimum distance of 256, so this test should yield key0 - key1 == j;
            if( ( (u0_t*)key0 - (u0_t*)kv1->key != j ) ) ERR( "largest_below_equal failed at distance %j: key0=%lu, key1=%lu", j, key0, kv1->key );
        }
    }
    time = clock() - time;
    st_s_pushf( log, "(%5.3gs per test)\n", ( ( double )time/CLOCKS_PER_SEC ) / ( kvbuf_size * 10 ) );

    st_s_pushf( log, "\n" );
    st_s_pushf( log, "Removal: " );
    time = clock();
    while( kvbuf_size )
    {
        kvbuf_size--;
        bcore_btree_pp_kv_s kv = kvbuf[ kvbuf_size ];
        if( !bcore_btree_pp_s_val( t, kv.key ) )  ERR( "key (%lu) not found", kv.key );
        bcore_btree_pp_s_remove( t, kv.key );
        if( bcore_btree_pp_s_val( t, kv.key ) )  ERR( "deleted key still exists (%lu)", kv.key );
    }
    time = clock() - time;
    st_s_pushf( log, "(%5.3fs)\n", ( double )time/CLOCKS_PER_SEC );

    if( t->root ) ERR( "root still exists" );

    st_s_push_st_d( log, bcore_btree_pp_s_status( t ) );

    bcore_btree_pp_s_discard( t );
    bcore_alloc( kvbuf, 0 );
    st_s_push_char_n( log, '=', 80 );
    st_s_push_char( log, '\n' );
    return log;
}

/**********************************************************************************************************************/
/**********************************************************************************************************************/
/// btree_vd
/**********************************************************************************************************************/
/**********************************************************************************************************************/

// E use the specifier 'kv' in this structure for consistency reasons even though this tree has no dedicated value
typedef struct
{
    bcore_btree_vd_key_t key;
} bcore_btree_vd_kv_s;

void bcore_btree_vd_kv_s_init( bcore_btree_vd_kv_s* o )
{
    o->key = 0;
}

/**********************************************************************************************************************/

/** Node of a 2-3 btree.
 *  Child pointers can have one of three states:
 *  NULL: corresponding key-value pair is not used (for normal nodes this state only applies to child2)
 *  BNUL_VP: node is a leaf
 *  else: node has children
 */
typedef struct bcore_btree_node_vd_s
{
    bcore_btree_vd_kv_s  kv1;
    bcore_btree_vd_kv_s  kv2;
    struct bcore_btree_node_vd_s* parent;
    struct bcore_btree_node_vd_s* child0;
    struct bcore_btree_node_vd_s* child1;
    struct bcore_btree_node_vd_s* child2;
} bcore_btree_node_vd_s;

/// children of leaf-nodes point to bcore_btree_node_vd_s_null
bcore_btree_node_vd_s bcore_btree_node_vd_s_null_g = { { NULL }, { NULL }, NULL, NULL, NULL, NULL };
#define BNUL_VP ( &bcore_btree_node_vd_s_null_g )

void bcore_btree_node_vd_s_init( bcore_btree_node_vd_s* o )
{
    bcore_btree_vd_kv_s_init( &o->kv1 );
    bcore_btree_vd_kv_s_init( &o->kv2 );
    o->parent = o->child0 = o->child1 = o->child2 = NULL;
}

bcore_btree_node_vd_s* bcore_btree_node_vd_s_find( bcore_btree_node_vd_s* root, bcore_btree_vd_key_t key )
{
    if( !root ) return NULL;
    bcore_btree_node_vd_s* node = NULL;
    while( root->child0 != BNUL_VP && root != node )
    {
        node = root;
        root = ( key < node->kv1.key )                                         ? node->child0 :
               ( !node->child2 && key > node->kv1.key )                        ? node->child1 :
               (  node->child2 && key > node->kv2.key )                        ? node->child2 :
               (  node->child2 && key > node->kv1.key && key < node->kv2.key ) ? node->child1 : node;
    }
    return root;
}

bcore_btree_vd_key_t bcore_btree_node_vd_s_largest_below_equal( bcore_btree_node_vd_s* root, bcore_btree_vd_key_t key )
{
    if( !root ) return 0;
    if( root->child0 == BNUL_VP )
    {
        if( ( root->child2 ) && key >= root->kv2.key ) return root->kv2.key;
        return ( key >= root->kv1.key ) ? root->kv1.key : 0;
    }
    else if( ( root->child2 ) && key >= root->kv2.key )
    {
        bcore_btree_vd_key_t branch_key = bcore_btree_node_vd_s_largest_below_equal( root->child2, key );
        return ( branch_key >= root->kv2.key ) ? branch_key : root->kv2.key;
    }
    else if( key >= root->kv1.key )
    {
        bcore_btree_vd_key_t branch_key = bcore_btree_node_vd_s_largest_below_equal( root->child1, key );
        return ( branch_key >= root->kv1.key ) ? branch_key : root->kv1.key;
    }
    else
    {
        return bcore_btree_node_vd_s_largest_below_equal( root->child0, key );
    }
}

void bcore_btree_node_vd_s_run( const bcore_btree_node_vd_s* root, void(*func)( vd_t arg, bcore_btree_vd_key_t key ), vd_t arg )
{
    if( !root ) return;
    if( !func ) return;
    if( root->child0 )
    {
        if( root->child0 != BNUL_VP ) bcore_btree_node_vd_s_run( root->child0, func, arg );
    }
    if( root->child1 )
    {
        func( arg, root->kv1.key );
        if( root->child1 != BNUL_VP ) bcore_btree_node_vd_s_run( root->child1, func, arg );
    }
    if( root->child2 )
    {
        func( arg, root->kv2.key );
        if( root->child2 != BNUL_VP ) bcore_btree_node_vd_s_run( root->child2, func, arg );
    }
}

uz_t bcore_btree_node_vd_s_count( const bcore_btree_node_vd_s* root, bl_t (*func)( vd_t arg, bcore_btree_vd_key_t key ), vd_t arg )
{
    uz_t count = 0;
    if( !root ) return count;
    if( root->child0 )
    {
        if( root->child0 != BNUL_VP ) count += bcore_btree_node_vd_s_count( root->child0, func, arg );
    }
    if( root->child1 )
    {
        count += ( func ) ? func( arg, root->kv1.key ) : 1;
        if( root->child1 != BNUL_VP ) count += bcore_btree_node_vd_s_count( root->child1, func, arg );
    }
    if( root->child2 )
    {
        count += ( func ) ? func( arg, root->kv2.key ) : 1;
        if( root->child2 != BNUL_VP ) count += bcore_btree_node_vd_s_count( root->child2, func, arg );
    }
    return count;
}

uz_t bcore_btree_node_vd_s_keys( bcore_btree_node_vd_s* root )
{
    if( !root || root == BNUL_VP ) return 0;
    uz_t keys = root->child2 ? 2 : 1;
    keys += bcore_btree_node_vd_s_keys( root->child0 );
    keys += bcore_btree_node_vd_s_keys( root->child1 );
    if( root->child2 ) keys += bcore_btree_node_vd_s_keys( root->child2 );
    return keys;
}

uz_t bcore_btree_node_vd_s_depth( bcore_btree_node_vd_s* root )
{
    if( !root || root == BNUL_VP ) return 0;
    return 1 + bcore_btree_node_vd_s_depth( root->child0 );
}

void bcore_btree_node_vd_s_set_parent_child0( bcore_btree_node_vd_s* o ) { if( o->child0 && o->child0 != BNUL_VP ) o->child0->parent = o; }
void bcore_btree_node_vd_s_set_parent_child1( bcore_btree_node_vd_s* o ) { if( o->child1 && o->child1 != BNUL_VP ) o->child1->parent = o; }
void bcore_btree_node_vd_s_set_parent_child2( bcore_btree_node_vd_s* o ) { if( o->child2 && o->child2 != BNUL_VP ) o->child2->parent = o; }
int  bcore_btree_node_vd_s_is_leaf(  bcore_btree_node_vd_s* o )          { return o->child0 == BNUL_VP; }
int  bcore_btree_node_vd_s_is_full(  bcore_btree_node_vd_s* o )          { return o->child2 != NULL; }
int  bcore_btree_node_vd_s_is_empty( bcore_btree_node_vd_s* o )          { return o->child1 == NULL; }

void bcore_btree_node_vd_s_check_consistency( bcore_btree_node_vd_s* o )
{
    if( bcore_btree_node_vd_s_null_g.kv1.key != 0    ) ERR( "bcore_btree_node_vd_s_null was modified" );
    if( bcore_btree_node_vd_s_null_g.kv2.key != 0    ) ERR( "bcore_btree_node_vd_s_null was modified" );
    if( bcore_btree_node_vd_s_null_g.parent  != NULL ) ERR( "bcore_btree_node_vd_s_null was modified" );
    if( bcore_btree_node_vd_s_null_g.child0  != NULL ) ERR( "bcore_btree_node_vd_s_null was modified" );
    if( bcore_btree_node_vd_s_null_g.child1  != NULL ) ERR( "bcore_btree_node_vd_s_null was modified" );
    if( bcore_btree_node_vd_s_null_g.child2  != NULL ) ERR( "bcore_btree_node_vd_s_null was modified" );

    if( !o ) return;
    if( bcore_btree_node_vd_s_is_empty( o ) ) ERR( "empty node" );
    if( o->child0 == NULL )       ERR( "deleted leaf" );
    if( o->child1 && o->child1 != BNUL_VP )
    {
        if( o != o->child0->parent ) ERR( "child0 incorrect parent" );
        if( o != o->child1->parent ) ERR( "child1 incorrect parent" );
        bcore_btree_node_vd_s_check_consistency( o->child0 );
        bcore_btree_node_vd_s_check_consistency( o->child1 );
        if(                      o->kv1.key <= o->child0->kv1.key ) ERR( "(%lu <= %lu)", o->kv1.key, o->child0->kv1.key );
        if( o->child0->child2 && o->kv1.key <= o->child0->kv2.key ) ERR( "(%lu <= %lu)", o->kv1.key, o->child0->kv2.key );
        if(                      o->kv1.key >= o->child1->kv1.key ) ERR( "(%lu >= %lu)", o->kv1.key, o->child1->kv1.key );
        if( o->child1->child2 && o->kv1.key >= o->child1->kv2.key ) ERR( "(%lu >= %lu)", o->kv1.key, o->child1->kv2.key );
    }
    if( o->child2 && o->child2 != BNUL_VP )
    {
        if( o->kv1.key >= o->kv2.key ) ERR( "(%lu >= %lu)", o->kv1.key, o->kv2.key );
        if( o != o->child2->parent ) ERR( "child2 incorrect parent" );
        bcore_btree_node_vd_s_check_consistency( o->child2 );
        if(                      o->kv2.key <= o->child1->kv1.key ) ERR( "(%lu <= %lu)", o->kv2.key, o->child1->kv1.key );
        if( o->child1->child2 && o->kv2.key <= o->child1->kv2.key ) ERR( "(%lu <= %lu)", o->kv2.key, o->child1->kv2.key );
        if(                      o->kv2.key >= o->child2->kv1.key ) ERR( "(%lu >= %lu)", o->kv2.key, o->child2->kv1.key );
        if( o->child2->child2 && o->kv2.key >= o->child2->kv2.key ) ERR( "(%lu >= %lu)", o->kv2.key, o->child2->kv2.key );
    }
}

st_s* bcore_btree_node_vd_s_show( bcore_btree_node_vd_s* o, uz_t depth )
{
    st_s* string = st_s_create();
    if( !o )
    {
        st_s_pushf( string, "()\n" );
        return string;
    }
    if( o->child1 )
    {
        if( o->child0 && o->child0 != BNUL_VP ) st_s_push_st_d( string, bcore_btree_node_vd_s_show( o->child0, depth + 1 ) );
        for( uz_t i = 0; i < depth; i++ ) st_s_pushf( string, "    " );
        st_s_pushf( string, "(%lu)\n", ( int64_t )(intptr_t)o->kv1.key );
        if( o->child1 && o->child1 != BNUL_VP ) st_s_pushf( string, "    " );
    }
    if( o->child2 )
    {
        for( uz_t i = 0; i < depth; i++ ) st_s_pushf( string, "    " );
        st_s_pushf( string, "(%lu)\n", ( int64_t )(intptr_t)o->kv2.key );
        if( o->child2 && o->child2 != BNUL_VP ) st_s_push_st_d( string, bcore_btree_node_vd_s_show( o->child2, depth + 1 ) );
    }
    return string;
}

/**********************************************************************************************************************/

struct bcore_btree_vd_s
{
    bcore_btree_node_vd_s* root;
    bcore_btree_node_vd_s* chain_beg;   // begin of chain of blocks of bcore_btree_node_vd_s[] with last element being pointer to next block
    bcore_btree_node_vd_s* chain_end;   // end of chain of blocks
    bcore_btree_node_vd_s* chain_ins;   // pointer for new insertions
    bcore_btree_node_vd_s* del_chain;   // chain of deleted elements (preferably used by new insertions)
    vd_t (*alloc)( vd_t, uz_t size ); // alloc function
    uz_t   block_size;
};

bcore_btree_vd_s* bcore_btree_vd_s_create( vd_t (*alloc)( vd_t, uz_t size ) )
{
    bcore_btree_vd_s* o = NULL;
    if( alloc )
    {
        o = alloc( NULL, sizeof( bcore_btree_vd_s ) );
        o->alloc = alloc;
    }
    else
    {
        o = bcore_alloc( NULL, sizeof( bcore_btree_vd_s ) );
        o->alloc = bcore_alloc;
    }
    o->root      = NULL;
    o->chain_beg = NULL;
    o->chain_end = NULL;
    o->chain_ins = NULL;
    o->del_chain = NULL;
    o->block_size = 1024;
    return o;
}

void bcore_btree_vd_s_discard( bcore_btree_vd_s* o )
{
    o->root = NULL;

    bcore_btree_node_vd_s* chain_beg = o->chain_beg;
    while( chain_beg )
    {
        bcore_btree_node_vd_s* new_beg = *( bcore_btree_node_vd_s** )( chain_beg + o->block_size );
        o->alloc( chain_beg, 0 );
        chain_beg = new_beg;
    }

    o->alloc( o, 0 );
}

bcore_btree_node_vd_s* bcore_btree_vd_s_new_node( bcore_btree_vd_s* o )
{
    if( o->del_chain )
    {
        bcore_btree_node_vd_s* new_node = o->del_chain;
        o->del_chain = new_node->parent;
        bcore_btree_node_vd_s_init( new_node );
        return new_node;
    }
    else
    {
        if( o->chain_ins == o->chain_end )
        {
            bcore_btree_node_vd_s* new_ptr = o->alloc( NULL, o->block_size * sizeof( bcore_btree_node_vd_s ) + sizeof( bcore_btree_node_vd_s* ) );
            if( !o->chain_beg )
            {
                o->chain_beg = new_ptr;
            }
            else
            {
                ( ( bcore_btree_node_vd_s** )( o->chain_end ) )[ 0 ] = new_ptr;
            }
            o->chain_ins = new_ptr;
            o->chain_end = new_ptr + o->block_size;
            *( bcore_btree_node_vd_s** )( o->chain_end ) = NULL;
        }
        bcore_btree_node_vd_s* new_node = o->chain_ins;
        bcore_btree_node_vd_s_init( new_node );
        o->chain_ins++;
        return new_node;
    }
}

// Deleted nodes are marked by setting all children NULL
// and chained together using pointer bcore_btree_node_vd_s.parent.
void bcore_btree_vd_s_delete_node( bcore_btree_vd_s* o, bcore_btree_node_vd_s* node )
{
    node->child0 = NULL;
    node->child1 = NULL;
    node->child2 = NULL;
    node->parent = o->del_chain;
    o->del_chain = node;
}

// recursively pushes an element into the tree
void bcore_btree_vd_s_push( bcore_btree_vd_s* o, bcore_btree_node_vd_s* node, bcore_btree_vd_kv_s* kv, bcore_btree_node_vd_s* child0, bcore_btree_node_vd_s* child1 )
{
    if( bcore_btree_node_vd_s_is_full( node ) )
    {
        bcore_btree_node_vd_s* l_node = node;
        bcore_btree_node_vd_s* r_node = bcore_btree_vd_s_new_node( o );
        bcore_btree_vd_kv_s root_kv;

        if( kv->key < node->kv1.key )
        {
            root_kv        = l_node->kv1;
            r_node->kv1    = l_node->kv2;
            r_node->child0 = l_node->child1;
            r_node->child1 = l_node->child2;
            l_node->kv1    = *kv;
            l_node->child0 = child0;
            l_node->child1 = child1;
        }
        else if( kv->key > node->kv2.key )
        {
            root_kv        = l_node->kv2;
            r_node->kv1    = *kv;
            r_node->child0 = child0;
            r_node->child1 = child1;
        }
        else
        {
            root_kv        = *kv;
            r_node->kv1    = l_node->kv2;
            r_node->child1 = l_node->child2;
            r_node->child0 = child1;
            l_node->child1 = child0;
        }
        r_node->child2 = NULL;
        l_node->child2 = NULL;

        bcore_btree_node_vd_s_set_parent_child0( r_node );
        bcore_btree_node_vd_s_set_parent_child1( r_node );
        bcore_btree_node_vd_s_set_parent_child0( l_node );
        bcore_btree_node_vd_s_set_parent_child1( l_node );

        if( l_node->parent )
        {
            bcore_btree_vd_s_push( o, l_node->parent, &root_kv, l_node, r_node );
        }
        else
        {
            o->root = bcore_btree_vd_s_new_node( o );
            o->root->kv1    = root_kv;
            o->root->child0 = l_node;
            o->root->child1 = r_node;
            l_node->parent  = o->root;
            r_node->parent  = o->root;
        }
    }
    else
    {
        if( kv->key < node->kv1.key )
        {
            node->kv2    = node->kv1;
            node->kv1    = *kv;
            node->child2 = node->child1;
            node->child1 = child1;
            node->child0 = child0;
            bcore_btree_node_vd_s_set_parent_child0( node );
            bcore_btree_node_vd_s_set_parent_child1( node );
            bcore_btree_node_vd_s_set_parent_child2( node );
        }
        else
        {
            node->kv2    = *kv;
            node->child2 = child1;
            node->child1 = child0;
            bcore_btree_node_vd_s_set_parent_child1( node );
            bcore_btree_node_vd_s_set_parent_child2( node );
        }
    }
}

// Recursively pulls an element from a non-leaf into an empty child node
void bcore_btree_vd_s_pull( bcore_btree_vd_s* o, bcore_btree_node_vd_s* node )
{
    if( bcore_btree_node_vd_s_is_empty( node->child0 ) )
    {
        if( bcore_btree_node_vd_s_is_full( node->child1 ) )
        {
            node->child0->kv1    = node->kv1;
            node->child0->child1 = node->child1->child0;
            bcore_btree_node_vd_s_set_parent_child1( node->child0 );
            node->kv1            = node->child1->kv1;
            node->child1->kv1    = node->child1->kv2;
            node->child1->child0 = node->child1->child1;
            node->child1->child1 = node->child1->child2;
            node->child1->child2 = NULL;
        }
        else if( bcore_btree_node_vd_s_is_full( node ) )
        {
            node->child1->kv2    = node->child1->kv1;
            node->child1->kv1    = node->kv1;
            node->child1->child2 = node->child1->child1;
            node->child1->child1 = node->child1->child0;
            node->child1->child0 = node->child0->child0;
            bcore_btree_node_vd_s_set_parent_child0( node->child1 );
            bcore_btree_vd_s_delete_node( o, node->child0 );
            node->kv1    = node->kv2;
            node->child0 = node->child1;
            node->child1 = node->child2;
            node->child2 = NULL;
        }
        else
        {
            node->child1->kv2    = node->child1->kv1;
            node->child1->kv1    = node->kv1;
            node->child1->child2 = node->child1->child1;
            node->child1->child1 = node->child1->child0;
            node->child1->child0 = node->child0->child0;
            bcore_btree_node_vd_s_set_parent_child0( node->child1 );
            bcore_btree_vd_s_delete_node( o, node->child0 );
            node->child0 = node->child1;
            node->child1 = node->child2 = NULL;
            if( node->parent )
            {
                bcore_btree_vd_s_pull( o, node->parent );
            }
            else
            {
                o->root = node->child0;
                o->root->parent = NULL;
                bcore_btree_vd_s_delete_node( o, node );
            }
        }
    }
    else if( bcore_btree_node_vd_s_is_empty( node->child1 ) )
    {
        if( bcore_btree_node_vd_s_is_full( node->child0 ) )
        {
            node->child1->kv1    = node->kv1;
            node->child1->child1 = node->child1->child0;
            node->child1->child0 = node->child0->child2;
            bcore_btree_node_vd_s_set_parent_child0( node->child1 );
            node->kv1            = node->child0->kv2;
            node->child0->child2 = NULL;
        }
        else if( bcore_btree_node_vd_s_is_full( node ) )
        {
            node->child0->kv2    = node->kv1;
            node->child0->child2 = node->child1->child0;
            bcore_btree_node_vd_s_set_parent_child2( node->child0 );
            bcore_btree_vd_s_delete_node( o, node->child1 );
            node->kv1    = node->kv2;
            node->child1 = node->child2;
            node->child2 = NULL;
        }
        else
        {
            node->child0->kv2    = node->kv1;
            node->child0->child2 = node->child1->child0;
            bcore_btree_node_vd_s_set_parent_child2( node->child0 );
            bcore_btree_vd_s_delete_node( o, node->child1 );
            node->child1 = node->child2 = NULL;
            if( node->parent )
            {
                bcore_btree_vd_s_pull( o, node->parent );
            }
            else
            {
                o->root = node->child0;
                o->root->parent = NULL;
                bcore_btree_vd_s_delete_node( o, node );
            }
        }
    }
    else // node->child2 is empty
    {
        if( bcore_btree_node_vd_s_is_full( node ) )
        {
            if( bcore_btree_node_vd_s_is_full( node->child1 ) )
            {
                node->child2->kv1 = node->kv2;
                node->child2->child1 = node->child2->child0;
                node->child2->child0 = node->child1->child2;
                bcore_btree_node_vd_s_set_parent_child0( node->child2 );
                node->kv2            = node->child1->kv2;
                node->child1->child2 = NULL;
            }
            else
            {
                node->child1->kv2    = node->kv2;
                node->child1->child2 = node->child2->child0;
                bcore_btree_node_vd_s_set_parent_child2( node->child1 );
                bcore_btree_vd_s_delete_node( o, node->child2 );
                node->child2         = NULL;
            }
        }
    }
}

bl_t bcore_btree_vd_s_exists( const bcore_btree_vd_s* o, bcore_btree_vd_key_t key )
{
    if( !o )    return false;
    bcore_btree_node_vd_s* node = bcore_btree_node_vd_s_find( o->root, key );
    if( !node ) return false;
    if( node->kv1.key == key ) return true;
    if( node->child2 && node->kv2.key == key ) return true;
    return false;
}

bcore_btree_vd_key_t bcore_btree_vd_s_largest_below_equal( const bcore_btree_vd_s* o, bcore_btree_vd_key_t key )
{
    bcore_btree_vd_key_t ret = bcore_btree_node_vd_s_largest_below_equal( o->root, key );

    // for the (hypothetical) case where ( NULL != (vp_t)0 )
    return ( ret == 0 ) ? NULL : ret;
}

int bcore_btree_vd_s_set( bcore_btree_vd_s* o, bcore_btree_vd_key_t key )
{
    if( !o ) return -2;
    if( !o->root )
    {
        o->root = bcore_btree_vd_s_new_node( o );
        o->root->child0 = o->root->child1 = BNUL_VP;
        o->root->child2 = NULL;
        o->root->kv1.key = key;
        return 1;
    }

    bcore_btree_node_vd_s* node = bcore_btree_node_vd_s_find( o->root, key );
    if( !node ) return -2;

    if( node->kv1.key == key )
    {
        return 0;
    }
    else if( node->child2 && node->kv2.key == key )
    {
        return 0;
    }
    else
    {
        bcore_btree_vd_kv_s kv = { key };
        bcore_btree_vd_s_push( o, node, &kv, BNUL_VP, BNUL_VP );
        return 1;
    }
}

int bcore_btree_vd_s_remove( bcore_btree_vd_s* o, bcore_btree_vd_key_t key )
{
    if( !o       ) return -1;
    if( !o->root ) return  0;
    bcore_btree_node_vd_s* node = bcore_btree_node_vd_s_find( o->root, key );
    if( !node    ) return -1;

    if( node->kv1.key == key )
    {
        if( !bcore_btree_node_vd_s_is_leaf( node ) )
        {
            bcore_btree_node_vd_s* trace = node->child0;
            while( !bcore_btree_node_vd_s_is_leaf( trace ) ) trace = ( trace->child2 ) ? trace->child2 : trace->child1;
            if( bcore_btree_node_vd_s_is_full( trace ) )
            {
                node->kv1   = trace->kv2;
                trace->child2 = NULL;
            }
            else
            {
                node->kv1 = trace->kv1;
                trace->child1 = trace->child2 = NULL;
                bcore_btree_vd_s_pull( o, trace->parent );
            }
        }
        else if( bcore_btree_node_vd_s_is_full( node ) )
        {
            node->kv1 = node->kv2;
            node->child2 = NULL;
        }
        else
        {
            node->child1 = node->child2 = NULL;
            if( node->parent )
            {
                bcore_btree_vd_s_pull( o, node->parent );
            }
            else
            {
                bcore_btree_vd_s_delete_node( o, node );
                o->root = NULL;
            }
        }
        return 1;
    }

    if( node->kv2.key == key )
    {
        if( !bcore_btree_node_vd_s_is_leaf( node ) )
        {
            bcore_btree_node_vd_s* trace = ( node->child2 ) ? node->child2 : node->child1;
            while( !bcore_btree_node_vd_s_is_leaf( trace ) ) trace = trace->child0;
            if( bcore_btree_node_vd_s_is_full( trace ) )
            {
                node->kv2     = trace->kv1;
                trace->kv1    = trace->kv2;
                trace->child2 = NULL;
            }
            else
            {
                node->kv2   = trace->kv1;
                trace->child1 = trace->child2 = NULL;
                bcore_btree_vd_s_pull( o, trace->parent );
            }
        }
        else if( bcore_btree_node_vd_s_is_full( node ) )
        {
            node->child2 = NULL;
        }
        else
        {
            node->child1 = node->child2 = NULL;
            if( node->parent )
            {
                bcore_btree_vd_s_pull( o, node->parent );
            }
            else
            {
                bcore_btree_vd_s_delete_node( o, node );
                o->root = NULL;
            }
        }
        return 1;
    }
    return 0;
}

void bcore_btree_vd_s_run( const bcore_btree_vd_s* o, void(*func)( vd_t arg, bcore_btree_vd_key_t key ), vd_t arg )
{
    bcore_btree_node_vd_s_run( o->root, func, arg );
}

uz_t bcore_btree_vd_s_count( const bcore_btree_vd_s* o, bl_t(*func)( vd_t arg, bcore_btree_vd_key_t key ), vd_t arg )
{
    return bcore_btree_node_vd_s_count( o->root, func, arg );
}

uz_t bcore_btree_vd_s_depth( const bcore_btree_vd_s* o )
{
    return bcore_btree_node_vd_s_depth( o->root );
}

st_s* bcore_btree_vd_s_show( bcore_btree_vd_s* o )
{
    bcore_btree_node_vd_s_check_consistency( o->root );
    st_s* s = bcore_btree_node_vd_s_show( o->root, 0 );
    st_s_pushf( s, "\n" );
    return s;
}

st_s* bcore_btree_vd_s_status( bcore_btree_vd_s* o )
{
    uz_t blocks = 0;
    uz_t nodes = 0;
    uz_t deleted_nodes = 0;
    if( o->chain_beg )
    {
        bcore_btree_node_vd_s* chain_beg = o->chain_beg;
        while( chain_beg )
        {
            chain_beg = *( bcore_btree_node_vd_s** )( chain_beg + o->block_size );
            blocks++;
        }
        nodes = blocks * o->block_size - ( o->chain_end - o->chain_ins );
    }
    if( o->del_chain )
    {
        bcore_btree_node_vd_s* del_chain = o->del_chain;
        while( del_chain )
        {
            del_chain = del_chain->parent;
            deleted_nodes++;
        }
    }

    st_s* string = st_s_create();
    uz_t used_nodes = nodes - deleted_nodes;
    st_s_pushf( string, "keys ........... %lu\n", bcore_btree_node_vd_s_keys( o->root ) );
    st_s_pushf( string, "nodes .......... %lu\n", used_nodes );
    st_s_pushf( string, "keys/nodes ..... %5.4f\n", used_nodes > 0 ? ( double )( bcore_btree_node_vd_s_keys( o->root ) ) / used_nodes : 0 );
    st_s_pushf( string, "depth .......... %lu\n", bcore_btree_node_vd_s_depth( o->root ) );
    st_s_pushf( string, "block size ..... %lu\n", o->block_size );
    st_s_pushf( string, "blocks ......... %lu\n", blocks );
    st_s_pushf( string, "deleted nodes .. %lu\n", deleted_nodes );
    return string;
}

static st_s* btree_vd_s_selftest( void )
{
    st_s* log = st_s_createf( "== btree_vd_s_selftest " );
    st_s_push_char_n( log, '=', 80 - log->size );
    st_s_push_char( log, '\n' );

    bcore_btree_vd_s* t = bcore_btree_vd_s_create( NULL );
    const uz_t cycles = 200000;

    bcore_btree_vd_kv_s* kvbuf = bcore_alloc( NULL, cycles * sizeof( bcore_btree_vd_kv_s ) );
    uz_t kvbuf_size = 0;

    clock_t time = clock();
    st_s_pushf( log, "Mixed access: " );

    {
        u3_t rval = 1;
        for( uz_t i = 0; i < cycles; i++ )
        {
            bcore_btree_vd_kv_s kv;

            // use multiple of 256 to allow nearest_... tests
            // The left-shift can produce key repetitions on 32bit systems. The loop below ensures stored keys are not repeated.
            while( bcore_btree_vd_s_exists( t, ( kv.key = ( bcore_btree_vd_key_t )(intptr_t)( ( rval = bcore_xsg_u2( rval ) ) << 8 ) ) ) );
            kvbuf[ kvbuf_size++ ] = kv;

            // set
            bcore_btree_vd_s_set( t, kv.key );

            // retrieve
            rval = bcore_xsg_u2( rval );
            kv = kvbuf[ rval % kvbuf_size ];
            if( !bcore_btree_vd_s_exists( t, kv.key ) ) ERR( "key %lu does not exist", kv.key );

            // delete
            rval = bcore_xsg_u2( rval );
            if( ( ( rval >> 10 ) & 1 ) == 1 )
            {
                rval = bcore_xsg_u2( rval );
                uz_t idx = rval % kvbuf_size;
                bcore_btree_vd_kv_s kv = kvbuf[ idx ];
                if( !bcore_btree_vd_s_exists( t, kv.key ) ) ERR( "key %lu does not exist", kv.key );
                bcore_btree_vd_s_remove( t, kv.key );
                if( bcore_btree_vd_s_exists( t, kv.key ) ) ERR( "deleted key still exists (%lu)", kv.key );
                kvbuf_size--;
                kvbuf[ idx ] = kvbuf[ kvbuf_size ];
            }
        }
    }

    time = clock() - time;
    st_s_pushf( log, "(%5.3fs)\n", ( double )time/CLOCKS_PER_SEC );

    bcore_btree_node_vd_s_check_consistency( t->root );
    st_s_push_st_d( log, bcore_btree_vd_s_status( t ) );

    time = clock();
    st_s_pushf( log, "\nRead-access of %lu keys: ", kvbuf_size );
    uz_t read_cycles = 20;
    for( uz_t j = 0; j < read_cycles; j++ )
    {
        for( uz_t i = 0; i < kvbuf_size; i++ )
        {
            if( !bcore_btree_vd_s_exists( t, kvbuf[ i ].key ) ) ERR( "key %lu does not exist", kvbuf[ i ].key );
        }
    }
    time = clock() - time;
    st_s_pushf( log, "(%gs per access)\n", ( ( double )time/CLOCKS_PER_SEC ) / ( kvbuf_size * read_cycles ) );

    time = clock();
    st_s_pushf( log, "largest_below_equal - test: ", kvbuf_size );
    if( bcore_btree_vd_s_largest_below_equal( t, (bcore_btree_vd_key_t)1 ) != 0 ) ERR( "largest_below_equal( 1 ) failed" );
    for( uz_t j = 0; j < 10; j++ )
    {
        for( uz_t i = 0; i < kvbuf_size; i++ )
        {
            bcore_btree_vd_key_t key0 = ( u0_t* )kvbuf[ i ].key + j;
            bcore_btree_vd_key_t key1 = bcore_btree_vd_s_largest_below_equal( t, key0 );
            // stored keys have a minimum distance of 256, so this test should yield key0 - key1 == j;
            if( ( (u0_t*)key0 - (u0_t*)key1 != j ) ) ERR( "largest_below_equal failed at distance %j: key0=%lu, key1=%lu", j, key0, key1 );
        }
    }
    time = clock() - time;
    st_s_pushf( log, "(%5.3gs per test)\n", ( ( double )time/CLOCKS_PER_SEC ) / ( kvbuf_size * 10 ) );


    st_s_pushf( log, "\n" );
    st_s_pushf( log, "Removal: " );
    time = clock();
    while( kvbuf_size )
    {
        kvbuf_size--;
        bcore_btree_vd_kv_s kv = kvbuf[ kvbuf_size ];
        if( !bcore_btree_vd_s_exists( t, kv.key ) ) ERR( "key %lu does not exist", kv.key );
        bcore_btree_vd_s_remove( t, kv.key );
        if( bcore_btree_vd_s_exists( t, kv.key ) ) ERR( "deleted key still exists (%lu)", kv.key );
    }
    time = clock() - time;
    st_s_pushf( log, "(%5.3fs)\n", ( double )time/CLOCKS_PER_SEC );

    if( t->root ) ERR( "root still exists" );

    st_s_push_st_d( log, bcore_btree_vd_s_status( t ) );

    bcore_btree_vd_s_discard( t );
    bcore_alloc( kvbuf, 0 );
    st_s_push_char_n( log, '=', 80 );
    st_s_push_char( log, '\n' );
    return log;
}

/**********************************************************************************************************************/

vd_t bcore_btree_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_btree" ) ) )
    {
        case TYPEOF_init1:
        {
        }
        break;

        case TYPEOF_selftest:
        {
            st_s* log = st_s_create();
            st_s_push_st_d( log, btree_ip_s_selftest() );
            st_s_push_st_d( log, btree_ps_s_selftest() );
            st_s_push_st_d( log, btree_pp_s_selftest() );
            st_s_push_st_d( log, btree_vd_s_selftest() );
            return log;
        }
        break;

        default: break;
    }

    return NULL;
}
