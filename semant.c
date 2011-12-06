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
   entry->pointer    = sem_pt_type_none;
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

   new_entry->pointer    = entry->pointer;

   new_entry->more_info  = entry->more_info;    /* Aqui NAO será um clone */

   return new_entry;
}

void sem_entry_dispose(void* e)
{
   sem_entry* entry = (sem_entry*) e;

   free(entry->string);
   free(entry->type);
   /*free(entry->more_info); ??? */

   free(entry);
}

void sem_init(void)
{
   sem_gl_info.global_table            = sem_table_get();
   sem_gl_info.current_table           = sem_gl_info.global_table;

   sem_gl_info.context_stack           = stack_get();

   sem_gl_info.context_return_type     = NULL;

   sem_register_ident_clear();

   sem_gl_info.attrib_stack            = stack_get();
}

sem_table* sem_table_get(void)
{
   sem_table* t         = (sem_table*) malloc(sizeof(sem_table));

   t->table             = btree_get_tree(SEM_BTREE_ORDER, sem_compare_keys, sem_entry_dispose);
   t->pending_stack     = stack_get();
   t->pending_changes   = sem_entry_get();

   return t;
}

void sem_table_dispose(sem_table* table)
{
   if(!table) return;
   /* TODO: Does not free all allocated memory for the nodes */


   btree_dispose(table->table);
   stack_dispose(table->pending_stack);
   sem_entry_dispose((void*)table->pending_changes);

   free(table);
}

int sem_table_insert(sem_table* table, sem_entry* entry)
{
   return btree_insert(table->table, (void*) entry->string, (void*) entry) == BTREE_SUCCESS ?
          SUCCESS : ERROR;

}

sem_entry* sem_table_find(sem_table* table, char* string)
{
   return (sem_entry*) btree_find(table->table, string);
}

int sem_pending_insert(char* string, sem_category category)
{
   sem_entry* partial_entry = sem_entry_get();

   partial_entry->string = (char*) malloc((strlen(string)+1)*sizeof(char));
   strcpy(partial_entry->string, string);

   partial_entry->category = category;

   if(category == procedure || category == function)
      sem_gl_info.current_table->pending_changes->more_info = (void*) list_get();

   if(sem_table_insert(sem_gl_info.current_table, partial_entry) != SUCCESS)
      return ERROR;

   stack_push(sem_gl_info.current_table->pending_stack, partial_entry);

   return SUCCESS;

}

void sem_pending_update(sem_pending_upd_type upd, void* value)
{
   sem_entry*  temp_entry;
   list*       temp_list;

   int i;
   switch(upd)
   {
      case sem_upd_type:
         sem_gl_info.current_table->pending_changes->type =
            (char*) malloc((strlen((char*)value)+1)*sizeof(char));

         strcpy(sem_gl_info.current_table->pending_changes->type, (char*)value);
         break;

      case sem_upd_pointer:
      /* Nota: none deve ser 0
               var deve ser 01 ou 10
               pointer deve ser 10 ou 01
               var e pointer deve ser 11
      */
         sem_gl_info.current_table->pending_changes->pointer |= (sem_pt_type) value;
         break;

      case sem_upd_more_info:
         sem_gl_info.current_table->pending_changes->more_info = value;
         break;

      case sem_upd_param_insert:
         temp_entry = sem_entry_get();

         temp_entry->string = (char*) malloc((1+strlen((char*) value))*sizeof(char));
         strcpy(temp_entry->string, (char*) value);
         temp_entry->category = variable;

         temp_list = (list*) sem_gl_info.current_table->pending_changes->more_info;

         list_insert(temp_list, (void*) temp_entry);

         break;

      case sem_upd_param_update:
         temp_list = (list*)sem_gl_info.current_table->pending_changes->more_info;

         for(i = temp_list->size - 1;
             (temp_entry = ((sem_entry*) list_elem_at(temp_list, i))) &&
             temp_entry->type == NULL;
             --i)
         {
            int type_len = (1+strlen((char*) sem_gl_info.current_table->pending_changes->type));

            temp_entry->pointer = sem_gl_info.current_table->pending_changes->pointer;
            temp_entry->type    = (char*) malloc(type_len*sizeof(char));

            strcpy(temp_entry->type, (char*)sem_gl_info.current_table->pending_changes->type);

            sem_gl_info.current_table->pending_changes->pointer = sem_pt_type_none;
            free(sem_gl_info.current_table->pending_changes->type);
            sem_gl_info.current_table->pending_changes->type = NULL;
         }
         break;


      default:
         break;
   }
}


void sem_pending_commit(void)
{
   sem_entry* entry;
   while((entry = stack_pop(sem_gl_info.current_table->pending_stack)))
   {
      entry->pointer    = sem_gl_info.current_table->pending_changes->pointer;

      if(sem_gl_info.current_table->pending_changes->type)
      {
         entry->type = (char*) malloc((strlen(sem_gl_info.current_table->pending_changes->type)+1)*sizeof(char));
         strcpy(entry->type   , sem_gl_info.current_table->pending_changes->type);
      }
      entry->more_info     = sem_gl_info.current_table->pending_changes->more_info;
   }

   sem_gl_info.current_table->pending_changes->pointer     = sem_pt_type_none;
   /*
   sem_gl_info.current_table->pending_changes->more_info   = NULL;

   more_info pode ser usado pela troca de contexto. deve ser anulado nela.
   */
   free(sem_gl_info.current_table->pending_changes->type);
   sem_gl_info.current_table->pending_changes->type        = NULL;

}

extern void print_value(const void*, const void*);
int sem_check(sem_check_type check, char* key)
{
   /*printf("\n\nsearching for %s (line: %d)\n", key, line_number);
   printf("Current table:\n");
   btree_print(sem_gl_info.current_table->table, print_value);*/
   sem_entry* entry = NULL;

   if(key)
   {
      entry = sem_table_find(sem_gl_info.current_table, key);

      if(!entry)
         if(check == sem_check_type_declared || sem_gl_info.current_table == sem_gl_info.local_table)
         {
            /*printf("%s not found! Will search global\n", key);
            btree_print(sem_gl_info.global_table->table, print_value);*/
            entry = sem_table_find(sem_gl_info.global_table, key);
         }

      if(!entry) return ERROR;
   }

   /*printf("\n\n");*/
   switch(check)
   {
      case sem_check_type_declared:
         if(entry->category == type_def)
         {
            sem_gl_info.current_table->pending_changes->more_info = entry->more_info;
            return SUCCESS;
         }
         else
            return ERROR;

      case sem_check_variable_declared:
         return   (entry->category == variable) ?
                  SUCCESS : ERROR;

      case sem_check_var_const_declared:
         return   ((entry->category == variable) || (entry->category == constant)) ?
                  SUCCESS : ERROR;

      case sem_check_proc_func_declared:
         return   ((entry->category == procedure) || (entry->category == function)) ?
                  SUCCESS : ERROR;

      case sem_check_any_declared:
         return   SUCCESS;

      case sem_check_return_allowed:
         return   (sem_gl_info.context_return_type) ?
                  SUCCESS : ERROR;

      case sem_check_attr:
         return sem_pop_check_push(NULL);
         break;

      default:
         return ERROR;
   }

}

void sem_error(sem_error_type error, char* error_string)
{
   switch(error)
   {
      case sem_error_ident_ja_declarado:
         fprintf(out_file, "Linha %d: identificador %s ja declarado anteriormente\n", prev_line_number, error_string);
         break;
      case sem_error_tipo_nao_declarado:
         fprintf(out_file, "Linha %d: tipo %s nao declarado\n", prev_line_number, error_string);
         break;
      case sem_error_ident_nao_declarado:
         fprintf(out_file, "Linha %d: identificador %s nao declarado\n", prev_line_number, error_string);
         break;
      case sem_error_incomp_de_parametros:
         fprintf(out_file, "Linha %d: incompatibilidade de parametros na chamada de %s\n", prev_line_number, error_string);
         break;
      case sem_error_atrib_nao_compativel:
         fprintf(out_file, "Linha %d: atribuicao nao compativel para %s\n", prev_line_number, error_string);
         break;
      case sem_error_retorne_nao_permitido:
         fprintf(out_file, "Linha %d: comando retorne nao permitido nesse escopo\n", prev_line_number);
         break;
      default:
         fprintf(out_file, "Codigo de erro %d nao conhecido. Linha: %d Token: %s", error, prev_line_number, error_string);
   }
}

void sem_context_change(sem_scope_chg_type type)
{
   static sem_table* last_context = NULL;
   sem_table*        t            = NULL;

   sem_entry*        temp_entry   = NULL;
   int i;

   switch(type)
   {
      case sem_scope_global_to_local:

         sem_gl_info.local_table = sem_table_get();

         /* Insere os parâmetros da funcao/procedimento na tabela local */
         if(sem_gl_info.global_table->pending_changes->more_info)
            for(i = 0;
               i < ((list*) sem_gl_info.global_table->pending_changes->more_info)->size;
               ++i)
            {
               sem_entry* entry = sem_entry_clone(list_elem_at(((list*) sem_gl_info.global_table->pending_changes->more_info), i));
               sem_table_insert(sem_gl_info.local_table, entry);
            }

         /* Impede que funções/procedimentos futuros usem o mesmo more_info */
         sem_gl_info.global_table->pending_changes->more_info = NULL;

         sem_gl_info.current_table = sem_gl_info.local_table;
         break;

      case sem_scope_local_to_global:
         sem_table_dispose(sem_gl_info.local_table);
         sem_gl_info.local_table = NULL;

         sem_gl_info.current_table = sem_gl_info.global_table;

         break;

      case sem_scope_register_insert:

         t = sem_table_get();
         sem_pending_update(sem_upd_more_info, t);

         /*sem_gl_info.current_table->pending_changes->more_info = (void*) sem_table_get();*/

         if(!stack_peek(sem_gl_info.context_stack))     /* Empty stack */
            last_context = sem_gl_info.current_table;

         stack_push(sem_gl_info.context_stack, t);

         sem_gl_info.current_table = (sem_table*) stack_peek(sem_gl_info.context_stack);
         break;

      case sem_scope_register_query:

         temp_entry = sem_table_find(sem_gl_info.current_table, tk->string);

         if(!temp_entry)
            temp_entry = sem_table_find(sem_gl_info.global_table, tk->string);


         if(!stack_peek(sem_gl_info.context_stack))     /* Empty stack */
            last_context = sem_gl_info.current_table;

         if(temp_entry && temp_entry->more_info)
         {
            stack_push(sem_gl_info.context_stack, temp_entry->more_info);
            sem_gl_info.current_table = (sem_table*) stack_peek(sem_gl_info.context_stack);
         }

         break;

      case sem_scope_register_end:
         stack_pop(sem_gl_info.context_stack);

         if(! (sem_gl_info.current_table = (sem_table*) stack_peek(sem_gl_info.context_stack)) )
            sem_gl_info.current_table = last_context;

         break;

      case sem_scope_allows_return:
         sem_gl_info.context_return_type =
                   (char*) malloc((1+strlen(sem_gl_info.current_table->pending_changes->type))*sizeof(char));

         strcpy(sem_gl_info.context_return_type, sem_gl_info.current_table->pending_changes->type);
         break;

      case sem_scope_forbids_return:
         free(sem_gl_info.context_return_type);
         sem_gl_info.context_return_type = NULL;
         break;
   }
}

void sem_register_ident_set(char* str)
{
   sem_register_ident_clear();
   sem_register_ident_append(str);
}

void sem_register_ident_append(char* str)
{
   int len = strlen(sem_gl_info.register_ident);
   int put_point = len > 0;

   len += strlen(str) + put_point;

   sem_gl_info.register_ident = (char*) realloc(sem_gl_info.register_ident, (len + 1)*sizeof(char));

   if(put_point)
      strcat(sem_gl_info.register_ident, ".");

   strcat(sem_gl_info.register_ident, str);
}

void sem_register_ident_clear(void)
{
   sem_gl_info.register_ident = (char*) realloc(sem_gl_info.register_ident, sizeof(char));
   strcpy(sem_gl_info.register_ident, "");
}





char* sem_type_of(char* key)
{
   sem_entry* entry = sem_table_find(sem_gl_info.current_table, key);

   if(!entry)
      if(sem_gl_info.current_table == sem_gl_info.local_table)
         entry = sem_table_find(sem_gl_info.global_table, key);

   if(!entry) return NULL;

   return entry->type;
}

list* sem_list_of(char* key)
{
   sem_entry* entry = sem_table_find(sem_gl_info.global_table, key);

   if(!entry) return NULL;

   return (list*) entry->more_info;
}

extern void stack_print(stack* s);
void sem_attrib_push(char* type)
{
   if(!type) type = sem_gl_info.attrib_temp;

   if(strcmp(type, "inteiro") == SUCCESS ||
      strcmp(type, "real") == SUCCESS)
      type = SEM_TYPE_NUMBER;

   stack_push(sem_gl_info.attrib_stack, (void*) type);

/*
   printf("will push %s. after push:\n", type);
   stack_print(sem_gl_info.attrib_stack);
   printf("end_stack\n\n\n");
*/

}

char* sem_attrib_pop()
{
   return (char*) stack_pop(sem_gl_info.attrib_stack);
}

char* sem_attrib_peek()
{
   return (char*) stack_peek(sem_gl_info.attrib_stack);
}

void sem_attrib_set(char* value)
{
   if(!value) return;
   sem_gl_info.attrib_temp = (char*) realloc(sem_gl_info.attrib_temp, (1+strlen(value))*sizeof(char));
   strcpy(sem_gl_info.attrib_temp, value);
}

int sem_pop_check_push(char* type)
{
   char *attr1, *attr2;

   attr1 = sem_attrib_pop();
   attr2 = sem_attrib_pop();

   if(strcmp(attr1, SEM_TYPE_UNDEFINED) == SUCCESS ||
      strcmp(attr2, SEM_TYPE_UNDEFINED) == SUCCESS ||
      strcmp(attr1, attr2) != SUCCESS)
   {
      if(type) sem_attrib_push(SEM_TYPE_UNDEFINED);
      return ERROR;
   }

   /*printf("will push after sem_pop_check_push\n");*/
   if(type) sem_attrib_push(type);
   return SUCCESS;
}

void sem_attrib_enforce_top_type(char* type)
{
   if(strcmp(type, sem_attrib_peek()) != SUCCESS)
   {
      sem_attrib_pop();
      sem_attrib_push(SEM_TYPE_UNDEFINED);
   }
}


