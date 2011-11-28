#include "btree.h"

/* btreei_ prefix marks B Tree Internal functions */

static btreei_node*   btreei_get_node   (int order);
static int            btreei_is_leaf    (btreei_node* n);
static void           btreei_check_size (btreei_node* n, int order);
static void           btreei_insert     (btreei_node* n, void* key, void* value, int order, btreei_node* left, btreei_node* right);
static int            btreei_remove     (btreei_node* n, void* key, int order);
static btreei_node*   btreei_find       (btreei_node* n, void* key);


static btreei_node* btreei_get_node(int order)
{
   int            i;
   btreei_node*   n;

   n = (btreei_node*) malloc(sizeof(btreei_node));

   n->length   = 0;
   n->parent   = NULL;
   n->values   = (void**)        malloc(   order *sizeof(void*)       );
   n->keys     = (void**)        malloc(   order *sizeof(void*)       );
   n->pointers = (btreei_node**) malloc((1+order)*sizeof(btreei_node*));

   for(i = 0; i < order; ++i)
      n->pointers[i] = n->keys[i] = n->values[i] = NULL;
   n->pointers[i] = NULL;

   return n;
}

static int btreei_is_leaf(btreei_node* n)
{
   return n->pointers[0] == NULL;
}

static void btreei_check_size(btreei_node* n, int order)
{
   if(n->length == order) /* Divisão do nó */
   {
      btreei_node* left  = btreei_get_node(order);
      btreei_node* right = btreei_get_node(order);

      int i;

      for(i = 0; i < order/2; ++i)
         btreei_insert(left, n->keys[i], n->values[i], order, n->pointers[i], n->pointers[i+1]);
      for(i = order/2 + 1; i < order; ++i)
         btreei_insert(right, n->keys[i], n->values[i], order, n->pointers[i], n->pointers[i+1]);

      if(n->parent != NULL)
         btreei_insert(n->parent, n->keys[order/2], n->values[order/2], order, left, right);
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
         btreei_node* son = n->pointers[0];

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
         while((btree_compare_keys(n->parent->keys[i], n->keys[n->length-1]) < 0) && i < n->parent->length)
            ++i;

         btreei_node* left_sibiling  = ((i>0) ? n->parent->pointers[i-1] : NULL);
         btreei_node* right_sibiling = ((i < n->parent->length) ? n->parent->pointers[i+1] : NULL);

         if(left_sibiling && left_sibiling->length > order/2)
         {
            --i;
            btreei_insert(n, n->parent->keys[i], n->parent->values[i], order, NULL, NULL);

            n->parent->keys[i]   = left_sibiling->keys  [left_sibiling->length-1];
            n->parent->values[i] = left_sibiling->values[left_sibiling->length-1];

            btreei_remove(left_sibiling, n->parent->keys[i], order);
            return;
         }
         else if(right_sibiling && right_sibiling->length > order/2)
         {
            btreei_insert(n, n->parent->keys[i], n->parent->values[i], order, NULL, NULL);

            n->parent->keys[i]   = right_sibiling->keys[0];
            n->parent->values[i] = right_sibiling->values[0];

            btreei_remove(right_sibiling, n->parent->keys[i], order);
            return;
         }
         else if(left_sibiling)
         {
            --i;
            btreei_insert(n, n->parent->keys[i], n->parent->values[i], order, n->pointers[0], n->pointers[0]);

            int j;

            for(j=0; j < left_sibiling->length; ++j)
               btreei_insert(n, left_sibiling->keys[j], left_sibiling->values[j], order, left_sibiling->pointers[i], left_sibiling->pointers[i+1]);

            n->parent->pointers[i] = n;
         }
         else if(right_sibiling)
         {
            btreei_insert(n, n->parent->keys[i], n->parent->values[i], order, n->pointers[n->length], n->pointers[n->length]);

            int j;

            for(j=0; j < right_sibiling->length; ++j)
               btreei_insert(n, right_sibiling->keys[j], right_sibiling->values[j], order, right_sibiling->pointers[i], right_sibiling->pointers[i+1]);


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

         btreei_check_size(n->parent, order);
      }
   }
}

static void btreei_insert(btreei_node* n, void* key, void* value, int order, btreei_node* left, btreei_node* right)
{
   if(left!=NULL || btreei_is_leaf(n))
   {
      int i = 0;
      while(i < n->length)
      {
         int comp = btree_compare_keys(key, n->keys[i]);
         if(comp > 0)   /* key > n->keys[i] */
            ++i;
         else if(comp < 0) /* key < n->keys[i] */
         {
            int j;
            for(j = n->length; j > i; --j)   /* TODO: j = m->length -1 ??? */
            {
               n->keys[j]        = n->keys[j-1];
               n->values[j]      = n->values[j-1];
               n->pointers[j+1]  = n->pointers[j];
               n->pointers[j]    = n->pointers[j-1];
            }
            break;
         }
         else
         {
            --n->length;
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
      btreei_check_size(n, order);

   }
   else if(!btreei_is_leaf(n))
   {
      int i    = 0;
      int comp = btree_compare_keys(n->keys[i], key);
      while(i < n->length)
      {
         if(comp > 0)   /* keys[i] > key */
         {
            btreei_insert(n->pointers[i], key, value, order, NULL, NULL);
            return;
         }
         ++i;
      }
      if(i == n->length)
         btreei_insert(n->pointers[i], key, value, order, NULL, NULL);
   }
}
/* TODO: return a proper value */
int btree_insert(btree* tree, void* key, void* value)        /* Interface to the world */
{
   btreei_insert(tree->root, key, value, tree->order, NULL, NULL);
   return 0;
}

int btree_remove(btree* tree, void* key)
{
   /* use find before? TODO: */
   if(btree_find(tree, key))     return btreei_remove(tree->root, key, tree->order);
   else                          return 0;
}

int btreei_remove(btreei_node* n, void* key, int order)
{
   /* Procura pela posição do valor a ser removido */
   int i;
   int comp = 0;

   for(i = 0; i < n->length; ++i)
      if((comp = btree_compare_keys(key, n->keys[0])) <= 0)
         break;

   if((i==n->length) || comp)
      return btreei_is_leaf(n) ? 0 : btreei_remove(n->pointers[i], key, order);

   if(btreei_is_leaf(n))
   {
      for(;i < n->length-1; ++i)
      {
         n->keys[i] = n->keys[i+1];
         n->values[i] = n->values[i+1];
      }

      --n->length;
      btreei_check_size(n, order);

      return 1;
   }
   else
   {
      btreei_node* swap_node = n->pointers[i+1];
      while(!btreei_is_leaf(swap_node))
         swap_node = swap_node->pointers[0];

      void* temp_key = n->keys[i];
      void* temp_val = n->values[i];

      n->keys[i]   = swap_node->keys[0];
      n->values[i] = swap_node->values[0];

      swap_node->keys[0]   = temp_key;
      swap_node->values[0] = temp_val;

      return btreei_remove(swap_node, temp_key, order);
   }
}


void* btree_find(btree* tree, void* key)
{
   int          i;
   btreei_node* n = btreei_find(tree->root, key);

   for(i = 0; i < n->length;++i)
      if(btree_compare_keys(key, n->keys[i]) == 0)
         return n->values[i];

   return NULL;
}

btreei_node* btreei_find(btreei_node* n, void* key)
{
   int i;
   int comp;

   if(btreei_is_leaf(n))
      return n;

   for(i=0; i < n->length; ++i)
   {
      comp = btree_compare_keys(key, n->keys[i]);
      if(comp == 0)
         return n;
      else if(comp < 0)
         break;
   }
   return btreei_find(n->pointers[i], key);
}

btree* btree_get_tree(int order)
{
   btree* t = (btree*) malloc(sizeof(btree));

   t->root  = btreei_get_node(order);
   t->order = order;

   return t;
}



void  btree_print(btree* tree)
{
   btreei_print(tree->root, 1);
}

void btreei_print(btreei_node* n, int level)
{
   int i;

   if(!n)
      return;

   printf("Level %d: [(%d)", level, n->length);

   for(i=0;i<n->length;++i)
   {
      btree_print_value(n->values[i]);
      printf(" ");
   }
   printf("]\n");

   if(!btreei_is_leaf(n))
      for(i=0;i<=n->length;++i)
         btreei_print(n->pointers[i], level+1);
}
