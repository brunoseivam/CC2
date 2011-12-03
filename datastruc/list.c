#include "list.h"
#include <stdio.h>

list*   list_get      (void)
{
   list* l     = (list*) malloc(sizeof(list));
   l->elements = NULL;
   l->size     = 0;

   return l;
}

void     list_dispose   (list* l)
{
   free(l->elements);
   free(l);
}

void     list_insert    (list* l, void* value)
{
   //printf("inserindo elemento...\n");
   ++l->size;
   l->elements = (void**) realloc(l->elements, l->size*sizeof(void*));

   l->elements[l->size-1] = value;
   //printf("tamanho lista %d, elemento %s\n", l->size, ((sem_entry*)l->elements[size-1])->string);
}

void*    list_elem_at   (list* l, int position)
{
   if(0 <= position && position < l->size)
      return l->elements[position];
   return NULL;
}
