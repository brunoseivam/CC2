#ifndef STACK_H
#define STACK_H

#include <stdlib.h>

typedef struct stack
{
   struct stacki_node*  top;
}stack;

stack*   stack_get      (void);
void     stack_dispose  (stack* s);
void     stack_push     (stack* s, void* value);
void*    stack_pop      (stack* s);
void*    stack_peek     (stack* s);

#endif
