#include "stack.h"

struct stacki_node
{
   void*                value;
   struct stacki_node*  next;
}stacki_node;

stack*   stack_get      (void)
{
   stack* s = (stack*) malloc(sizeof(stack));
   s->top   = NULL;

   return s;
}

void     stack_dispose  (stack* s)
{
   free(s); /* TODO: ?? */
}

void     stack_push     (stack* s, void* value)
{

   struct stacki_node* n   = (struct stacki_node*) malloc(sizeof(struct stacki_node));
   n->value                = value;
   n->next                 = s->top;
   s->top                  = n;
}
void*    stack_pop      (stack* s)
{
   if(!s->top) return NULL;

   struct stacki_node* node   = s->top;
   void*               value  = node->value;

   s->top = node->next;

   free(node);
   return value;
}
void*    stack_peek     (stack* s)
{
   if(!(s->top)) return NULL;
   return s->top->value;
}
