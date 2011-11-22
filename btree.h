#ifndef BTREE_H
#define BTREE_H

#include <stdlib.h>
#include <stdio.h>

typedef struct btreei_node
{
   void**               keys;          /* Array of keys of values                */
   void**               values;        /* Array of values of the node            */
   struct btreei_node** pointers;      /* Array of pointers to other nodes       */
   struct btreei_node*  parent;        /* Parent node                            */
   int                  length;        /* Number of values contained by the node */
}btreei_node;

typedef struct btree
{
   btreei_node*         root;          /* Root node of a tree                    */
   int                  order;         /* Order of the node (and of the BTree)   */
}btree;

extern int  btree_compare_keys(void*, void*);
extern void btree_print_value(void*);

/* Internal functions */
static btreei_node*   btreei_get_node   (int order);
static int            btreei_is_leaf    (btreei_node* n);
static void           btreei_check_size (btreei_node* n, int order);
static void           btreei_insert     (btreei_node* n, void* key, void* value, int order, btreei_node* left, btreei_node* right);
static int            btreei_remove     (btreei_node* n, void* key, int order);
static btreei_node*   btreei_find       (btreei_node* n, void* key);
void                  btreei_print      (btreei_node* n, int level);

/* External Interface */
btree*                btree_get_tree    (int order);
void                  btree_insert      (btree* tree, void* key, void* value);
int                   btree_remove      (btree* tree, void* key);
void*                 btree_find        (btree* tree, void* key);

void                  btree_print       (btree* tree);





#endif
