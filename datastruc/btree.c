#include "btree.h"

/* btreei_ prefix marks B Tree Internal functions */

struct btreei_node
{
   void**               keys;          /* Array of keys of values                */
   void**               values;        /* Array of values of the node            */
   struct btreei_node** pointers;      /* Array of pointers to other nodes       */
   struct btreei_node*  parent;        /* Parent node                            */
   int                  length;        /* Number of values contained by the node */
}btreei_node;


static struct btreei_node*    btreei_get_node   (                int order);  /* Order of the tree          */

static void                  btreei_dispose_node(struct btreei_node* n);      /* Free node memory           */

static int                    btreei_is_leaf    (struct btreei_node* n);      /* Node to be checked         */

static void                   btreei_check_size (struct btreei_node* n,       /* Node to be checked         */
                                                                 int order,   /* Order of the tree          */
                                                                 int (*compare)(const void*, const void*));
                                                                              /* Compare two keys           */


static int                    btreei_insert     (struct btreei_node* n,       /* Node to try insertion      */
                                                               void* key,     /* Key being inserted         */
                                                               void* value,   /* Value being inserted       */
                                                                 int order,   /* Order of the tree          */
                                                 struct btreei_node* left,    /* Auxiliary nodes being      */
                                                 struct btreei_node* right,   /*    inserted                */
                                                                 int (*compare)(const void*, const void*));
                                                                              /* Compare two keys           */

static void*                  btreei_remove     (struct btreei_node* n,       /* Node to try removal        */
                                                               void* key,     /* Key to be removed          */
                                                                 int order,   /* Order of the tree          */
                                                                 int (*compare)(const void*, const void*));
                                                                              /* Compare two keys           */

static struct btreei_node*    btreei_find       (struct btreei_node* n,       /* Node to start search       */
                                                               void* key,     /* Key being searched for     */
                                                                 int (*compare)(const void*, const void*));
                                                                              /* Compare two keys           */

static void                   btreei_print      (struct btreei_node* n,       /* Node to start printing from*/
                                                                 int level,   /* Current level of the tree  */
                                                                void (*print_key_value)(const void*, const void*));
                                                                              /* Prints a pair key/value    */



static struct btreei_node* btreei_get_node(int order)
{
   int                     i;
   struct btreei_node*     n;

   n = (struct btreei_node*) malloc(sizeof(struct btreei_node));

   n->length   = 0;
   n->parent   = NULL;
   n->values   = (void**)               malloc(   order *sizeof(void*)              );
   n->keys     = (void**)               malloc(   order *sizeof(void*)              );
   n->pointers = (struct btreei_node**) malloc((1+order)*sizeof(struct btreei_node*));

   for(i = 0; i < order; ++i)
      n->pointers[i] = n->keys[i] = n->values[i] = NULL;
   n->pointers[i] = NULL;

   return n;
}

static void btreei_dispose_node(struct btreei_node* n)
{
   /*
   int i;
   if(!btreei_is_leaf(n))
      for(i = 0; i <= n->length; i++)
      {
         btreei_dispose_node(n->pointers[i]);
         n->pointers[i] = NULL;
      }

   dispose(n->values[i]);
   n->keys[i] = NULL;
   n->values[i] = NULL;*/
   free(n);
}

static int btreei_is_leaf(struct btreei_node* n)
{
   return n->pointers[0] == NULL;
}

static void btreei_check_size(struct btreei_node* n, int order, int (*compare)(const void*, const void*))
{
   if(n->length == order) /* Divisão do nó */
   {
      struct btreei_node* left  = btreei_get_node(order);
      struct btreei_node* right = btreei_get_node(order);

      int i;

      for(i = 0; i < order/2; ++i)
         btreei_insert(left, n->keys[i], n->values[i], order, n->pointers[i], n->pointers[i+1], compare);
      for(i = order/2 + 1; i < order; ++i)
         btreei_insert(right, n->keys[i], n->values[i], order, n->pointers[i], n->pointers[i+1], compare);

      if(n->parent != NULL)
         btreei_insert(n->parent, n->keys[order/2], n->values[order/2], order, left, right, compare);
      else
      {
         n->keys[0]        = n->keys[order/2];
         n->values[0]      = n->values[order/2];
         n->pointers[0]    = left;
         n->pointers[1]    = right;
         n->length         = 1;
         left->parent      = n;
         right->parent     = n;
      }
   }
   else if(n->length < order/2)
   {
      if(!n->parent) /* Nó raiz */
      {
         if(n->length) return;   /* Pode ter order/2 elementos */

         /* Se vazio, promove primeiro filho a raiz */
         struct btreei_node* son    = n->pointers[0];

         int i = 0;

         while(i < son->length)
         {
            n->keys[i]              = son->keys[i];
            n->values[i]            = son->values[i];
            n->pointers[i]          = son->pointers[i];

            if(n->pointers[i])
               n->pointers[i]->parent  = n;
            ++i;
         }
         n->pointers[i]          = son->pointers[i];

         if(n->pointers[i])
               n->pointers[i]->parent  = n;

         n->length               = son->length;
         free(son);

         return;
      }
      else  /* Find sibilings */
      {
         int i = 0;
         while((compare(n->parent->keys[i], n->keys[n->length-1]) < 0) && i < n->parent->length)
            ++i;

         struct btreei_node* left_sibiling  = ((i>0) ? n->parent->pointers[i-1] : NULL);
         struct btreei_node* right_sibiling = ((i < n->parent->length) ? n->parent->pointers[i+1] : NULL);

         if(left_sibiling && left_sibiling->length > order/2)
         {
            --i;
            btreei_insert(n, n->parent->keys[i], n->parent->values[i], order, NULL, NULL, compare);

            n->parent->keys[i]   = left_sibiling->keys  [left_sibiling->length-1];
            n->parent->values[i] = left_sibiling->values[left_sibiling->length-1];

            btreei_remove(left_sibiling, n->parent->keys[i], order, compare);
            return;
         }
         else if(right_sibiling && right_sibiling->length > order/2)
         {
            btreei_insert(n, n->parent->keys[i], n->parent->values[i], order, NULL, NULL, compare);

            n->parent->keys[i]   = right_sibiling->keys[0];
            n->parent->values[i] = right_sibiling->values[0];

            btreei_remove(right_sibiling, n->parent->keys[i], order, compare);
            return;
         }
         else if(left_sibiling)
         {
            --i;
            btreei_insert(n, n->parent->keys[i], n->parent->values[i], order, n->pointers[0], n->pointers[0], compare);

            int j;

            for(j=0; j < left_sibiling->length; ++j)
               btreei_insert(n, left_sibiling->keys[j], left_sibiling->values[j], order,
                             left_sibiling->pointers[i], left_sibiling->pointers[i+1], compare);

            n->parent->pointers[i] = n;
         }
         else if(right_sibiling)
         {
            btreei_insert(n, n->parent->keys[i], n->parent->values[i], order,
                          n->pointers[n->length], n->pointers[n->length], compare);

            int j;

            for(j=0; j < right_sibiling->length; ++j)
               btreei_insert(n, right_sibiling->keys[j], right_sibiling->values[j], order,
                             right_sibiling->pointers[i], right_sibiling->pointers[i+1], compare);

            n->parent->pointers[i+1] = n;

         }
         while(i < n->parent->length - 1)
         {
            n->parent->keys[i]         = n->parent->keys[i+1];
            n->parent->values[i]       = n->parent->values[i+1];
            n->parent->pointers[i+1]   = n->parent->pointers[i+2];
            ++i;
         }
         --n->parent->length;

         if(!n->parent->length)
            n->parent->pointers[0] = n;   /* Tell the empty root node that n will be its son*/

         btreei_check_size(n->parent, order, compare);
      }
   }
}

static int  btreei_insert(struct btreei_node* n, void* key, void* value, int order,
                          struct btreei_node* left, struct btreei_node* right, int (*compare)(const void*, const void*))
{

   if(left!=NULL || btreei_is_leaf(n))
   {
      int i = 0;
      while(i < n->length)
      {
         int comp = compare(key, n->keys[i]);

         if(comp == 0)
            return BTREE_FAIL;
         else if(comp > 0)    /* key > n->keys[i] */
            ++i;
         else                 /* key < n->keys[i] */
         {
            int j;
            for(j = n->length; j > i; --j)
            {
               n->keys[j]        = n->keys[j-1];
               n->values[j]      = n->values[j-1];
               n->pointers[j+1]  = n->pointers[j];
               n->pointers[j]    = n->pointers[j-1];
            }
            break;
         }
      }
      n->keys[i]     = key;
      n->values[i]   = value;

      if(left!=NULL)
      {
         left->parent      = n;
         right->parent     = n;
         n->pointers[i]    = left;
         n->pointers[i+1]  = right;
      }
      ++n->length;
      btreei_check_size(n, order, compare);
      return BTREE_SUCCESS;
   }
   else if(!btreei_is_leaf(n))
   {
      int i    = 0;

      while(i <= n->length)
      {
         int comp = compare(n->keys[i], key);
         if(comp == 0)
            return BTREE_FAIL;
         else if(comp > 0 || i==n->length)   /* keys[i] > key */
            return btreei_insert(n->pointers[i], key, value, order, NULL, NULL, compare);
         ++i;
      }
   }
   /* SHOULDN'T BE HERE */
   return BTREE_ERROR;
}

void* btreei_remove(struct btreei_node* n, void* key, int order, int (*compare)(const void*, const void*))
{
   /* Search for the value to be removed */
   int i;
   int comp        = 0;
   void* removed   = NULL;

   for(i = 0; i < n->length; ++i)
      if((comp = compare(key, n->keys[0])) <= 0)
         break;

   if((i==n->length) || comp)
      return btreei_is_leaf(n) ? removed : btreei_remove(n->pointers[i], key, order, compare);

   removed = n->values[i];

   if(btreei_is_leaf(n))
   {
      for(;i < n->length-1; ++i)
      {
         n->keys[i] = n->keys[i+1];
         n->values[i] = n->values[i+1];
      }

      --n->length;
      btreei_check_size(n, order, compare);

      return removed;
   }
   else
   {
      struct btreei_node* swap_node = n->pointers[i+1];
      while(!btreei_is_leaf(swap_node))
         swap_node = swap_node->pointers[0];

      void* temp_key = n->keys[i];
      void* temp_val = n->values[i];

      n->keys[i]   = swap_node->keys[0];
      n->values[i] = swap_node->values[0];

      swap_node->keys[0]   = temp_key;
      swap_node->values[0] = temp_val;

      return btreei_remove(swap_node, temp_key, order, compare);
   }
}

struct btreei_node* btreei_find(struct btreei_node* n, void* key, int (*compare)(const void*, const void*))
{
   int i;
   int comp;

   if(btreei_is_leaf(n))
      return n;

   for(i=0; i < n->length; ++i)
   {
      comp = compare(key, n->keys[i]);
      if(comp == 0)
         return n;
      else if(comp < 0)
         break;
   }
   return btreei_find(n->pointers[i], key, compare);
}

void btreei_print(struct btreei_node* n, int level, void (*print_key_value)(const void*, const void*))
{
   int i;

   if(!n)   return;

   printf("Level %d: [(%d)", level, n->length);

   for(i=0 ; i < n->length ; ++i)
   {
      print_key_value(n->keys[i], n->values[i]);
      printf(" ");
   }
   printf("]\n");

   if(!btreei_is_leaf(n))
      for(i=0;i<=n->length;++i)
         btreei_print(n->pointers[i], level+1, print_key_value);
}





btree* btree_get_tree(int order, int (*compare)(const void*, const void*), void (*dispose)(void*))
{
   btree* t = (btree*) malloc(sizeof(btree));

   t->root  = btreei_get_node(order);
   t->order = order;

   t->compare = compare;
   t->dispose = dispose;

   return t;
}

void btree_dispose(btree* tree)
{
   /*btreei_dispose_node(tree->root);*/


   free(tree);    /* TODO: ?? */
}

int btree_insert(btree* tree, void* key, void* value)        /* Interface to the world */
{
   return btreei_insert(tree->root, key, value, tree->order, NULL, NULL, tree->compare);
}

void* btree_remove(btree* tree, void* key)
{
   return btreei_remove(tree->root, key, tree->order, tree->compare);
}

void* btree_find(btree* tree, void* key)
{
   int i;
   struct btreei_node* n = btreei_find(tree->root, key, tree->compare);

   for(i = 0; i < n->length;++i)
      if(tree->compare(key, n->keys[i]) == 0)
         return n->values[i];

   return NULL;
}

void  btree_print(btree* tree, void (*print_key_value)(const void*, const void*))
{
   btreei_print(tree->root, 1, print_key_value);
}
