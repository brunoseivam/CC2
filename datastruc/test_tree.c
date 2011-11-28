#include "btree.h"
#include <string.h>

struct obj
{
   char* key;
   int value1, value2;
   double value3;
}obj;

int comp(const void* key1, const void* key2)
{
   return strcmp((char*) key1, (char*) key2);
}

void print(const void* key, const void* value)
{
   char*       k     = (char*) key;
   struct obj* val   = (struct obj*) value;

   printf("[%s : %d %d %.2f]", k, val->value1, val->value2, val->value3);
}

int main()
{
   struct obj objects[10] = {
      {"blablah", 1, 2, 2.0},
      {"okey", 42, 27, 1.2},
      {"omg", 123, 321, 4.4},
      {"aaa", 3, 5, 123.32},
      {"casa", 4, 33, 44.45},
      {"franca", 9, 22, 32.3},
      {"ribeirao", 0, 11, 67.3},
      {"sao carlos", 12, 33, 12.9},
      {"sao jose", 25, 67, 7.7},
      {"sao paulo", 75, 89, 9.9},
   };
   btree* tree = btree_get_tree(3, comp);

   printf("Insert: %d\n", btree_insert(tree, objects[5].key, &objects[5]));
   btree_print(tree, print);

   printf("Insert: %d\n", btree_insert(tree, objects[3].key, &objects[3]));
   btree_print(tree, print);

   printf("Insert: %d\n", btree_insert(tree, objects[8].key, &objects[8]));
   btree_print(tree, print);

   printf("Insert: %d\n", btree_insert(tree, objects[0].key, &objects[0]));
   btree_print(tree, print);

   printf("Insert: %d\n", btree_insert(tree, objects[2].key, &objects[2]));
   btree_print(tree, print);

   printf("Insert: %d\n", btree_insert(tree, objects[5].key, &objects[5]));
   btree_print(tree, print);


   return 0;
}
