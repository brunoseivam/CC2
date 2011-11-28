#include "semant.h"

static   sem_table_entry** stack;
static   int               stack_size;


int  btree_compare_keys(void* key1, void* key2)
{
   return strcmp((char*) key1, (char*) key2);
}


void btree_print_value(void* value)
{
   if(!value)
   {
      printf("[NULL]");
      return;
   }

   sem_table_entry* entry = (sem_table_entry*) value;
   printf(" [%s][%d, %s, %d, %d] ", entry->string, entry->category, entry->type, entry->num_param, entry->is_pointer);
}


void sem_init_table(void)
{
   sem_table   = btree_get_tree(BTREE_ORDER);
   stack_size  = 0;
   stack       = NULL;
}



int sem_insert_pending(char* string, sem_entry_category category)
{
   sem_table_entry* entry  = (sem_table_entry*) malloc(sizeof(sem_table_entry));

   entry->string           = (char*) malloc(strlen(string)*sizeof(char));
   strcpy(entry->string,string);

   entry->category         = category;

   entry->is_pointer       = 0;
   entry->num_param        = 0;
   entry->type             = NULL;
   entry->more_info        = NULL;

   stack = (sem_table_entry**) realloc(stack, (++stack_size)*sizeof(sem_table_entry*));
   stack[stack_size-1]     = entry;

   return btree_insert(sem_table, (void*) string, (void*) entry);
}




void sem_print_stack(void)
{
   int i;
   printf("STACK\n");
   for(i = 0; i < stack_size; ++i)
      printf("STRING: %s, CAT: %d\n", stack[i]->string, stack[i]->category);
   printf("END_STACK\n");
}







/* TODO: check if already in table
void sem_insert(char* string, sem_entry_category category, char* type, int num_param, short is_pointer, void* more_info)
{
   printf("sem_insert %s\n", string);
   sem_table_entry* entry = (sem_table_entry*) malloc(sizeof(sem_table_entry));

   /* TODO ID redundancy
   entry->string = string;
   entry->category = category;
   entry->type = type;
   entry->num_param = num_param;
   entry->is_pointer = is_pointer;
   entry->more_info = more_info;

   btree_insert(sem_table, (void*) string, (void*) entry);
}*/
