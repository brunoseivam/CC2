#ifndef BTREE_H
#define BTREE_H

#include <stdlib.h>
#include <stdio.h>

#define BTREE_SUCCESS 1
#define BTREE_FAIL    0

typedef struct btree
{
   struct btreei_node*  root;          /* Root node of a tree                    */
   int                  order;         /* Order of the node (and of the BTree)   */
   int                  (*compare)(const void*, const void*);  /* Compare keys   */
}btree;

btree*                btree_get_tree    (int order, int (*compare)(const void*, const void*));

int                   btree_insert      (btree* tree, void* key, void* value);
void*                 btree_remove      (btree* tree, void* key);
void*                 btree_find        (btree* tree, void* key);

void                  btree_print       (btree* tree, void (*print_key_value)(const void*, const void*));

#endif
