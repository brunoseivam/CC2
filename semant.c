#include "semant.h"

/*
 * Implementa��o das fun��es descritas no cabe�alho semant.h.
 *  O funcionamento das fun��es est� descrito de maneira geral no
 *  cabe�alho. Aqui descreve-se o mecanismo destas fun��es, onde
 *  for necess�rio elaborar o explicado em semant.h
 *
 */


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
   entry->param_list = NULL;
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
   new_entry->param_list = entry->param_list;   /* Aqui NAO ser� um clone */
   new_entry->more_info  = entry->more_info;    /* Aqui NAO ser� um clone */

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
   sem_gl_info.current_table           = sem_gl_info.global_table;   /* Contexto inicial � o global            */

   sem_gl_info.context_stack           = stack_get();

   sem_gl_info.context_return_type     = NULL;                       /* Contexto global n�o permite 'retorne'  */

   sem_register_ident_clear();                                       /* register_ident = ""                    */

   sem_gl_info.attrib_stack            = stack_get();
   sem_gl_info.attrib_temp             = (char*) malloc(sizeof(char));
   strcpy(sem_gl_info.attrib_temp, "");


   sem_insert_basic_functions();
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

/* Insere uma entrada parcial na tabela sem�ntica */
int sem_pending_insert(char* string, sem_category category)
{
   /* Aloca uma nova entrada */
   sem_entry* partial_entry = sem_entry_get();

   /* Ponteiro para entrada. Ser� usado em uma checagem adiante */
   sem_entry* check_entry;

   /* Inicializa a entrada parcial com os valores passados */
   partial_entry->string = (char*) malloc((strlen(string)+1)*sizeof(char));
   strcpy(partial_entry->string, string);
   partial_entry->category = category;

   /* Caso seja um procedimento ou uma fun��o, aloca uma lista para seus par�metros */
   if(category == procedure || category == function)
      sem_gl_info.current_table->pending_changes->param_list = list_get();


   /* Aqui realiza-se uma checagem por uma peculiaridade da linguagem. N�o se pode
       definir uma vari�vel local com mesmo nome de um procedimento/fun��o global.
      Assim, procura-se por um procedimento/fun��o com mesmo nome da entrada sendo
      inserida. Se for encontrado, a entrada n�o � inserida e ocorre erro.
   */

   check_entry = sem_table_find(sem_gl_info.global_table, partial_entry->string);

   if(check_entry)
      if(check_entry->category == procedure ||
           check_entry->category == function)
           return ERROR;


   /* Tenta-se inserir na tabela de s�mbolos do contexto corrente. Caso j� exista, �
         acusado um erro e a inser��o falha */
   if(sem_table_insert(sem_gl_info.current_table, partial_entry) != SUCCESS)
      return ERROR;

   /* Al�m de inserir na �rvore B, a entrada TAMB�M � inserida na pilha de pendentes. Isto
       mant�m uma pilha de ponteiros para entradas que ainda precisam de tipo. Este mecanismo
       evita seja mantida uma lista de identificadores pendentes e depois seja necess�rio
       busc�-los na �rvore */
   stack_push(sem_gl_info.current_table->pending_stack, partial_entry);

   return SUCCESS;
}

/* Atualiza uma informa��o para as entradas pendentes. */
void sem_pending_update(sem_pending_upd_type upd, void* value)
{
   sem_entry*  temp_entry;
   list*       temp_list;

   int i;
   switch(upd)
   {
		/* Atualiza o tipo das entradas incompletas pendentes */
      case sem_upd_type:
         sem_gl_info.current_table->pending_changes->type =
            (char*) malloc((strlen((char*)value)+1)*sizeof(char));

         strcpy(sem_gl_info.current_table->pending_changes->type, (char*)value);
         break;

		/* Atualiza o campo 'pointer' das entradas pendentes */
      case sem_upd_pointer:
      /* Nota: none deve ser 0
               var deve ser 01 ou 10
               pointer deve ser 10 ou 01
               var e pointer deve ser 11
      */
         sem_gl_info.current_table->pending_changes->pointer |= (sem_pt_type) value;
         break;

		/* Atualiza o ponteiro para mais informa��es das entradas pendentes */
      case sem_upd_more_info:
         sem_gl_info.current_table->pending_changes->more_info = value;
         break;

		/* Insere um par�metro na lista de par�metros da fun��o/procedimento pendente.
		   O par�metro inserido ainda ser� carente de tipo */
      case sem_upd_param_insert:
         temp_entry = sem_entry_get();

         temp_entry->string = (char*) malloc((1+strlen((char*) value))*sizeof(char));
         strcpy(temp_entry->string, (char*) value);
         temp_entry->category = variable;

         list_insert(sem_gl_info.current_table->pending_changes->param_list,
                     (void*) temp_entry);

         /* TODO: checar se j� est� na lista? */

         break;

		/* Atualia o tipo dos par�metros sem tipo. Os par�metros sem tipo estar�o na cauda da lista.
			Assim, basta percorrer a lista de tr�s pra frente, atualizando os tipos at� encontrar o
			 primeiro par�metro que j� contenha tipo. */
      case sem_upd_param_update:
         temp_list = sem_gl_info.current_table->pending_changes->param_list;

			/* Itera sobre a lista */
         for(i = temp_list->size - 1;
             (temp_entry = ((sem_entry*) list_elem_at(temp_list, i))) &&
             temp_entry->type == NULL;
             --i)
         {
            int type_len = (1+strlen((char*) sem_gl_info.current_table->pending_changes->type));

            temp_entry->pointer = sem_gl_info.current_table->pending_changes->pointer;
            temp_entry->type    = (char*) malloc(type_len*sizeof(char));

            strcpy(temp_entry->type, (char*)sem_gl_info.current_table->pending_changes->type);

            temp_entry->more_info = sem_gl_info.current_table->pending_changes->more_info;

            sem_gl_info.current_table->pending_changes->pointer = sem_pt_type_none;
            free(sem_gl_info.current_table->pending_changes->type);
            sem_gl_info.current_table->pending_changes->type = NULL;
         }
         break;


      default:
         break;
   }
}

/* Aplica de fato as altera��es pendentes nas entradas carentes de tipo, que est�o na pilha
    pending_stack */
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

      entry->param_list    = sem_gl_info.current_table->pending_changes->param_list;

      entry->more_info     = sem_gl_info.current_table->pending_changes->more_info;

		/* Lembrando que n�o precisa inserir esta entrada na �rvore, pois ela j� est� l�!
		   entry � um ponteiro para esta entrada para dentro da �rvore, basta ele ser desempilhado. */
   }

	/* Limpa alguns campos da pending_change, para n�o interferir em commit's futuros */
   sem_gl_info.current_table->pending_changes->pointer     = sem_pt_type_none;
   free(sem_gl_info.current_table->pending_changes->type);
   sem_gl_info.current_table->pending_changes->type        = NULL;

}

/* Realiza v�rias checagens, dependendo do seletor usado */
int sem_check(sem_check_type check, char* key)
{
   sem_entry* entry = NULL;

	/* Caso a checagem se d� sobre uma entrada espec�fica, busca-se essa entrada
	    para se obter um ponteiro para ela. */
   if(key)
   {
      entry = sem_table_find(sem_gl_info.current_table, key);

      if(!entry)
         if(check == sem_check_type_declared || sem_gl_info.current_table == sem_gl_info.local_table)
            entry = sem_table_find(sem_gl_info.global_table, key);

      if(!entry) return ERROR;	/* Se a entrada n�o for encontrada, � imposs�vel
		                               realizar a checagem requisitada sobre ela */
   }

   switch(check)
   {
		/* Verifica se um tipo espec�fico est� declarado */
      case sem_check_type_declared:

			/* Caso ele esteja declarado e � um tipo definido pelo usu�rio, copia o conte�do deste tipo
			(campos armazenados em uma tabela sem�ntica) para o pending_changes. Este artif�cio serve para
			facilitar a checagem de tipos de registros aninhados */
         if(entry->category == type_def)
         {
            sem_gl_info.current_table->pending_changes->more_info = entry->more_info;
            return SUCCESS;
         }
         else
            return ERROR;

		/* Checa se a vari�vel especificada foi declarada */
      case sem_check_variable_declared:
         return   (entry->category == variable) ?
                  SUCCESS : ERROR;

		/* Checa se a constante especificada foi declarada */
      case sem_check_var_const_declared:
         return   ((entry->category == variable) || (entry->category == constant)) ?
                  SUCCESS : ERROR;

		/* Checa se a fun��o/procedimento especificado foi delcarado */
      case sem_check_proc_func_declared:
         return   ((entry->category == procedure) || (entry->category == function)) ?
                  SUCCESS : ERROR;

		/* Checa se h� uma entrada de qualquer categoria declarada com o identificador especificado */
      case sem_check_any_declared:
         return   SUCCESS;

		/* Checa se o contexto atual permite o comando 'retorne' */
      case sem_check_return_allowed:
         return   (sem_gl_info.context_return_type) ?
                  SUCCESS : ERROR;

		/* Checa se os dois tipos do topo da pilha s�o compat�veis, desempilhando-os */
      case sem_check_attr:
         return sem_pop_check_push(NULL);
         break;

      default:
         return ERROR;
   }

}

/* Fun��o respons�vel pela impress�o dos erros especificados pela professora.
   O uso de prev_line_number foi uma tentativa de fazer os erros serem acusados nas mesmas linhas especificadas pela professora
	 nos casos de teste. Ele ajudou, mas n�o eliminou todas as discrep�ncias. */
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

/* Fun��o respons�vel pelas mudan�as de contexto. */
void sem_context_change(sem_scope_chg_type type)
{
   static sem_table* last_context = NULL;
   sem_table*        t            = NULL;
   sem_entry*        temp_entry   = NULL;
   list*             param_list   = NULL;
   int i;

   switch(type)
   {
		/* Muda o escopo global para o escopo local */
      case sem_scope_global_to_local:

			/* Cria uma nova tabela para o escopo global */
         sem_gl_info.local_table = sem_table_get();

         param_list = sem_gl_info.global_table->pending_changes->param_list;

         /* Insere os par�metros da funcao/procedimento na tabela local */
         if(param_list)
            for(i = 0;
               i < param_list->size;
               ++i)
            {
               sem_entry* entry = sem_entry_clone(list_elem_at(param_list, i));
               sem_table_insert(sem_gl_info.local_table, entry);
            }

         /* Impede que fun��es/procedimentos futuros usem o mesmo param_list */
         sem_gl_info.global_table->pending_changes->param_list = NULL;

         sem_gl_info.current_table = sem_gl_info.local_table;	/* Agora o contexto corrente � a tabela local */
         break;

		/* Sai do contexto de fun��o/procedimento para o escopo global */
      case sem_scope_local_to_global:
         sem_table_dispose(sem_gl_info.local_table);
         sem_gl_info.local_table = NULL;

         sem_gl_info.current_table = sem_gl_info.global_table;	/* Agora o contexto corrente � a tabela global */

         break;

		/* Prepara o contexto para a inser��o de campos de um registro */
      case sem_scope_register_insert:

			/* Os campos de um registro s�o armazenados em uma tabela de s�mbolos */
         t = sem_table_get();								/* Aloca uma nova tabela 																		*/
         sem_pending_update(sem_upd_more_info, t);	/* Faz o more_info do pending_changes apontar para esta nova tabela alocada 	*/

			/* Deste modo, ao dar o sem_commit posteriormente, o more_info do pending_changes ser� copiado para o more_info da
			    entrada do registro */

			if(!stack_peek(sem_gl_info.context_stack))     	/* Empty stack */
				last_context = sem_gl_info.current_table;		/* Salva o contexto corrente, caso ele n�o seja um contexto de registro
				                                                 (ou seja, caso ele seja local_table ou global_table						*/

			stack_push(sem_gl_info.context_stack, t);			/* Empilha o novo contexto */

         sem_gl_info.current_table = (sem_table*) stack_peek(sem_gl_info.context_stack);	/* Muda o ponteiro corrente para o novo contexto */
         break;

		/* Prepara o contexto para realizar uma consulta dentro de um registro
			Busca o registro e muda o contexto corrente para o contexto de registro.
			Salva o contexto corrente */

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

		/* Encerra consulta/inser��o de registro, alterando o contexto de acordo */
      case sem_scope_register_end:
         stack_pop(sem_gl_info.context_stack);

         if(! (sem_gl_info.current_table = (sem_table*) stack_peek(sem_gl_info.context_stack)) )
            sem_gl_info.current_table = last_context;

         break;

		/* Os dois pr�ximos trechos apenas controlam se o contexto atual permite o comando retorne */
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
   if(strcmp(sem_gl_info.register_ident,"^"))
      sem_register_ident_clear();

   sem_register_ident_append(str);
}

void sem_register_ident_append(char* str)
{
   int len = strlen(sem_gl_info.register_ident) + strlen(str);

   sem_gl_info.register_ident = (char*) realloc(sem_gl_info.register_ident, (len + 1)*sizeof(char));

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

   return entry->param_list;
}

void sem_attrib_push(char* type)
{
   if(!type) type = sem_gl_info.attrib_temp;

   if(strcmp(type, "inteiro") == SUCCESS ||
      strcmp(type, "real") == SUCCESS)
      type = SEM_TYPE_NUMBER;

   stack_push(sem_gl_info.attrib_stack, (void*) type);


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

/*
void limpa_entrada(void);
char *subLiteral(char *, int, int);
double sen(double);
double pot(double, double);
int trunca(double);
double frac(double);
double raiz(double);
double cos(double);
*/
void sem_insert_basic_functions(void)
{
   struct func
   {
      sem_category   cat;
      char           name[15];
      char           type[15];

      int            n_param;

      char           p_name[3][15];
      char           p_type[3][25];

   }basic_functions[8] =
   {
      {
         procedure,
         "limpa_entrada",
         SEM_TYPE_UNDEFINED,
         0
      },

      {
         function,
         "subLiteral",
         "literal",
         3,
         {"p1", "p2", "p3"},
         {"literal", "inteiro", "inteiro"}
      },

      {
         function,
         "sen",
         "real",
         1,
         {"p1"},
         {"real"}
      },

      {
         function,
         "pot",
         "real",
         2,
         {"p1", "p2"},
         {"real", "real"}
      },

      {
         function,
         "trunca",
         "inteiro",
         1,
         {"p1"},
         {"real"}
      },

      {
         function,
         "frac",
         "real",
         1,
         {"p1"},
         {"real"}
      },

      {
         function,
         "raiz",
         "real",
         1,
         {"p1"},
         {"real"}
      },

      {
         function,
         "cos",
         "real",
         1,
         {"p1"},
         {"real"}
      }
   };

   int i;

   for(i = 0; i < 8; ++i)
   {
      sem_entry* entry  = sem_entry_get();

      entry->category   = basic_functions[i].cat;

      entry->string     = (char*) malloc(sizeof(char)*(1+strlen(basic_functions[i].name)));
      entry->type       = (char*) malloc(sizeof(char)*(1+strlen(basic_functions[i].type)));

      strcpy(entry->string,   basic_functions[i].name);
      strcpy(entry->type,     basic_functions[i].type);

      entry->param_list = list_get();

      int j;

      for(j = 0; j < basic_functions[i].n_param; ++j)
      {
         sem_entry* p_entry   = sem_entry_get();

         p_entry->category    = variable;

         p_entry->string      = (char*) malloc(sizeof(char)*(1+strlen(basic_functions[i].p_name[j])));
         p_entry->type        = (char*) malloc(sizeof(char)*(1+strlen(basic_functions[i].p_type[j])));

         strcpy(p_entry->string, basic_functions[i].p_name[j]);
         strcpy(p_entry->type,   basic_functions[i].p_type[j]);

         list_insert(entry->param_list, p_entry);
      }

      sem_table_insert(sem_gl_info.global_table, entry);
   }

}
