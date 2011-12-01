#include "list.h"

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
   free(l); /* TODO: ?? */
}

void     list_insert    (list* l, void* value)
{
   ++l->size;
   l->elements = (void*) realloc(l->elements, l->size*sizeof(void*));

   l->elements[l->size-1] = value;
}

void*    list_elem_at    (list* l, int position)
{
   if(0 <= position && position < l->size)
      return l->elements[position];
   return NULL;
}
