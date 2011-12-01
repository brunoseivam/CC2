#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

typedef struct list
{
   void**   elements;
   int      size;
}list;

list*    list_get       (void);
void     list_dispose   (list* l);
void     list_insert    (list* l, void* value);
void*    list_elem_at   (list* l, int position);

#endif
