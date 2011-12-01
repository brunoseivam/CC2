#include "semant.h"

int sem_compare_keys(const void* key1, const void* key2)
{
   return strcmp((char*) key1, (char*) key2);
}

sem_entry* sem_entry_get(void)
{
   sem_entry* entry = (sem_entry*) malloc(sizeof(sem_entry));

   entry->string     = NULL;
   entry->category   = 0;
   entry->type       = NULL;
   entry->num_param  = 0;
   entry->is_pointer = 0;
   entry->more_info  = NULL;

   return entry;
}

sem_entry* sem_entry_clone(sem_entry* entry)
{
   sem_entry* new_entry = sem_entry_get();

   if(entry->string)
   {
      new_entry->string = (char*) malloc( (1+strlen(entry->string))*sizeof(char));
      strcpy(new_entry->string , entry->string);
   }

   new_entry->category   = entry->category;

   if(entry->type)
   {
      new_entry->type = (char*) malloc( (1+strlen(entry->type))*sizeof(char));
      strcpy(new_entry->type,entry->type);
   }

   new_entry->num_param  = entry->num_param;
   new_entry->is_pointer = entry->is_pointer;

   new_entry->more_info  = entry->more_info;    /* Aqui NAO será um clone */

   return new_entry;
}

void sem_entry_dispose(sem_entry* entry)
{
   free(entry->string);
   free(entry->type);
   /*free(entry->more_info); ??? */

   free(entry);
}

void sem_init(void)
{
   sem_global_table  = sem_table_get();
   sem_current_table = sem_global_table;

   sem_current_context = sem_ctx_global;

   sem_context_stack = stack_get();
}

sem_table* sem_table_get(void)
{
   sem_table* t         = (sem_table*) malloc(sizeof(sem_table));

   t->table             = btree_get_tree(SEM_BTREE_ORDER, sem_compare_keys);
   t->pending_stack     = stack_get();
   t->pending_changes   = sem_entry_get();

   return t;
}

void sem_table_dispose(sem_table* table)
{
   /* TODO: Does not free all allocated memory for the nodes */
   btree_dispose(table->table);
   stack_dispose(table->pending_stack);
   sem_entry_dispose(table->pending_changes);

   free(table);
}


int sem_table_insert(sem_table* table, sem_entry* entry)
{
   return btree_insert(table->table, (void*) entry->string, (void*) entry);
}

int sem_table_find(sem_table* table, char* string)
{
   if(btree_find(table->table, string)!=NULL)
      return SUCCESS;
   return ERROR;
}

int sem_pending_insert(char* string, sem_category category)
{
   sem_entry* partial_entry = sem_entry_get();

   partial_entry->string = (char*) malloc((strlen(string)+1)*sizeof(char));
   strcpy(partial_entry->string, string);

   partial_entry->category = category;

   /* TODO: pensar: precisa mesmo desse wrapper? */
   if(sem_table_insert(sem_current_table, partial_entry) != SUCCESS)
      return ERROR;

   stack_push(sem_current_table->pending_stack, partial_entry);

   return SUCCESS;

}

void sem_pending_update(sem_pending_upd_type upd, void* value)
{
   switch(upd)
   {
      case sem_upd_type:
         sem_current_table->pending_changes->type =
            (char*) malloc((strlen((char*)value)+1)*sizeof(char));

         strcpy(sem_current_table->pending_changes->type, (char*)value);
         break;

      case sem_upd_is_pointer:
         sem_current_table->pending_changes->is_pointer = (int) value;
         break;

      /* TODO: completar */
      case sem_upd_more_info:
         break;

      case sem_upd_num_param:
         break;

      default:
         break;
   }
}


void sem_pending_commit(void)
{
   sem_entry* entry;
   while((entry = stack_pop(sem_current_table->pending_stack)))
   {
      entry->is_pointer    = sem_current_table->pending_changes->is_pointer;
      entry->num_param     = sem_current_table->pending_changes->num_param;

      if(sem_current_table->pending_changes->type)
      {
         entry->type = (char*) malloc((strlen(sem_current_table->pending_changes->type)+1)*sizeof(char));
         strcpy(entry->type   , sem_current_table->pending_changes->type);
      }
      entry->more_info     = sem_current_table->pending_changes->more_info;
   }
}

int sem_find(char* key)
{
   return ((sem_table_find(sem_current_table, key) == SUCCESS) ||
            (sem_table_find(sem_global_table, key) == SUCCESS))
         ? SUCCESS : ERROR;
}


void sem_error(sem_error_type error)
{
   switch(error)
   {
      case sem_error_ident_ja_declarado:
         printf("Linha %d: identificador %s ja declarado anteriormente\n", line_number, tk->string);
         break;
      case sem_error_tipo_nao_declarado:
         printf("Linha %d: tipo %s nao declarado\n", line_number, tk->string);
         break;
      case sem_error_ident_nao_declarado:
         printf("Linha %d: identificador %s nao declarado\n", line_number, tk->string);
         break;
      case sem_error_incomp_de_parametros:
         printf("Linha %d: incompatibilidade de parametros na chamada de %s\n", line_number, tk->string);
         break;
      case sem_error_atrib_nao_compativel:
         printf("Linha %d: atribuicao nao compativel para %s", line_number, tk->string);
         break;
      case sem_error_retorne_nao_permitido:
         printf("Linha %d: comando retorne nao permitido nesse escopo\n", line_number);
         break;
      default:
         printf("Codigo de erro %d nao conhecido. Linha: %d Token: %s", error, line_number, tk->string);
   }
}

void sem_context_change(sem_scope_chg_type type)
{
   switch(type)
   {
      case: sem_scope_global_to_local:
         sem_table_dispose(sem_local_table);
         sem_local_table = sem_table_get();

         int i;
         for(i = 0; i < sem_global_table->pending_changes->num_param; ++i)
         {
            sem_entry* entry = sem_entry_get();

            entry->string =

            sem_pending_insert(sem_local_table,
         }
         break;

   }
}

