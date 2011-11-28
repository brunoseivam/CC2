#ifndef STACK_H
#define STACK_H

typedef struct stacki_node
{
   void*                value;
   struct stacki_node*  next;
}stacki_node;

typedef struct stack
{

}stack;

stack*   get_stack   ();
void     stack_push  (stack* s, void* value);
void*    stack_pop   (stack* s);
void*    stack_peek  (stack* s);



#endif
