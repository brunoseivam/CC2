#include <stdlib.h>
#include <stdio.h>

#include "syntax.h"


/* Caso a estrutura first contenha apenas uma string OU
     uma token_class OU um ponteiro ficou decidido que
     seria codificado diretamente no autômato */


/* Declaração de todos os firsts estritamente C99.
Fonte: http://stackoverflow.com/questions/3875523/lookup-table-in-c */


/* Estrutura firsts:
      Três campos principais:
         string_list:      Lista de primeiros que sejam palavra-chave ou simbolo. Devem ser identificados atrabés de sua cadeia
         tk_class_list:    Lista de primeiros que sejam identificados através de sua classe (NUM_INT, IDENT...)
         other_firsts_list:Lista de outros primeiros a se consultar. Por exemplo primeiros(parametro) = { var, primeiros(identificador) }

*/



/* Autor: Bruno
primeiros(declaracao_local) = { declare, constante, tipo }
*/

const firsts declaracao_local_firsts =
{
   .string_list      = (const char * const []){"constante", "declare",  "tipo"},
   .string_list_size = 3,

   .tk_class_list    = NULL,
   .tk_class_list_size = 0,

   .other_firsts_list = NULL,
   .other_firsts_list_size = 0
};



/* Autor: Bruno
primeiros(identificador) = { ^, IDENT}
*/
const firsts identificador_firsts =
{
   .string_list = (const char * const []){"^"},
   .string_list_size = 1,

   .tk_class_list    = (const token_class []){identifier},
   .tk_class_list_size = 1,

   .other_firsts_list = NULL,
   .other_firsts_list_size = 0
};
/* Autor: Talita
primeiros(parametro) = { var, primeiros(identificador) }
*/
const firsts parametro_firsts =
{
   .string_list = (const char * const []){"var"},
   .string_list_size = 1,

   .tk_class_list = NULL,
   .tk_class_list_size = 0,

   .other_firsts_list = (const firsts * const []) { &identificador_firsts },
   .other_firsts_list_size = 1
};

/* Autor: Talita
primeiros(comandos) = { leia, escreva, se, caso, para, enquanto, faca, ^, IDENT, retorne, epsilon }
*/
const firsts comandos_firsts =
{
   .string_list = (const char * const []){"^", "caso", "enquanto", "escreva", "faca", "leia", "para", "retorne", "se"},
   .string_list_size = 9,

   .tk_class_list    = (const token_class []){identifier},
   .tk_class_list_size = 1,

   .other_firsts_list = NULL,
   .other_firsts_list_size = 0
};

/* Autor: Talita
primeiros(declaracao_global) = {funcao, procedimento}
*/

const firsts declaracao_global_firsts =
{
   .string_list = (const char * const []){"funcao", "procedimento"},
   .string_list_size = 2,

   .tk_class_list    = NULL,
   .tk_class_list_size = 0,

   .other_firsts_list = NULL,
   .other_firsts_list_size = 0
};

/* Autor: Marcos
primeiros(selecao) = { -, NUM_INT }
*/
const firsts selecao_firsts =
{
   .string_list = NULL,
   .string_list_size = 0,

   .tk_class_list    = NULL,
   .tk_class_list_size = 0,

   .other_firsts_list = (const firsts * const []){ &constantes_firsts },
   .other_firsts_list_size = 1
};

/* Autor: Marcos
primeiros(constantes) = { -, NUM_INT }
*/
const firsts constantes_firsts =
{
   .string_list = (const char * const []){"-"},
   .string_list_size = 1,

   .tk_class_list    = (const token_class []){ integer_number },
   .tk_class_list_size = 1,

   .other_firsts_list = NULL,
   .other_firsts_list_size = 0
};


/* Autômatos:

   Autômatos com recursão foram implementados com loop (while)

   Podem retornar dois valores: SUCCESS ou SYNTAXERROR. A rotina handle_error, na main(), analisa os casos de erro para separar
     os erros léxicos dos sintáticos, através da classe do token.

   As macros utilizadas estão explicadas em syntax.h, onde estão definidas.
*/



/*
Automato 1
Autor: Bruno

<programa>           ::= <declaracoes> algoritmo <declaracoes_locais> <comandos> fim_algoritmo
<declaracoes>        ::= <declaracao_local> <declaracoes>
                       | <declaracao_global> <declaracoes>
                       | epsilon
<declaracoes_locais> ::= <declaracao_local> <declaracoes_locais> | epsilon
*/

int programa()
{
   int ret;

	/* Pega o primeiro token */
	tk = get_token();

   while(1)
   {
      if(search_first(tk, declaracao_local_firsts) == SUCCESS)
      {
         CALL(declaracao_local);
      }
      else if(search_first(tk, declaracao_global_firsts) == SUCCESS)
      {
         CALL(declaracao_global);
      }
      else
         break;
   }

   CHECK_STRING(tk, "algoritmo");

	/*
	 * Alteração do contexto global (sem_gl_info.sem_global_table)
	 * para o contexto do algoritmo(local -  sem_gl_info.sem_local_table)
	 */
   sem_context_change(sem_scope_global_to_local);
   /*gen_main_begin();*/

   tk = get_token();

   while ( search_first(tk, declaracao_local_firsts) == SUCCESS )
      CALL(declaracao_local);

   CALL(comandos);

   CHECK_STRING(tk, "fim_algoritmo");

   /*gen_main_end();*/

   tk = get_token();

   return SUCCESS;
}

/*
Automato 2
Autor: Bruno

<declaracao_local>         ::= declare <variavel>
                             | constante IDENT : <tipo_basico>  = CADEIA
                             | constante IDENT : <tipo_basico>  = NUM_INT
                             | constante IDENT : <tipo_basico>  = NUM_REAL
                             | constante IDENT : <tipo_basico>  = verdadeiro
                             | constante IDENT : <tipo_basico>  = falso
                             | tipo IDENT : <tipo>
<tipo_basico> ::= literal | inteiro | real | logico
*/
int declaracao_local()
{
   int ret;

   if( strcmp(tk->string, "declare") == SUCCESS )
   {
      tk = get_token();
      CALL(variavel);
   }
   else if( strcmp(tk->string, "constante") == SUCCESS )
   {
      tk = get_token();

      CHECK_CLASS(tk, identifier);
	  
	   /*
	    * Insere o identificador, do tipo constante, criando uma nova entrada na tabela corrente, 
	    *  checando se essa inserção é possível, ou seja, se não há nenhuma outra 
	    *  entrada com esse mesmo nome. Caso haja algum identificador com o mesmo nome
	    *  o analisador semântico acusará erro.
	    */
      SEM_TRY(sem_pending_insert(tk->string, constant), sem_error_ident_ja_declarado, tk->string);

      tk = get_token();

      CHECK_STRING(tk, ":");
      tk = get_token();

      CHECK_STRINGS(tk, "literal", "inteiro", "real", "logico");
	  
	   /*
	    * Atualiza o tipo (type) da pending_changes para posterior commit em todas
	    *  as entradas da pilha (pending_stack), que ainda possuem tipo indefinido. 
	    *  Caso típico é: "declare a,b,c: inteiro". 
	    */	
      sem_pending_update(sem_upd_type, tk->string);
	   tk = get_token();

      CHECK_STRING(tk, "=");
      tk = get_token();

      if( tk->class != string && tk->class != integer_number && tk->class != real_number)
         CHECK_STRINGS(tk, "verdadeiro", "falso");

      /*gen_const(sem_gl_info.current_table,tk->string);*/
		
		
		/*
	    * Atualiza todas as entradas da pilha (pending_stack) com as modificações
	    *  feitas na pending_changes.
	    */	
      sem_pending_commit();

      tk = get_token();
   }
   else if( strcmp(tk->string, "tipo") == SUCCESS)
   {
      tk = get_token();

      CHECK_CLASS(tk, identifier);
		
		/*
	    * Definição de novo tipo, inserindo na tabela (e na pending_stack) uma nova entrada 
		 * com categoria "type_def" (tipo definido pelo usuário), chegando se tipo com mesmo 
		 * nome já existe.
		 * Exemplo: "declare tipo tVinho: ... "
	    */	
      SEM_TRY(sem_pending_insert(tk->string, type_def), sem_error_ident_ja_declarado, tk->string);

      tk = get_token();

      CHECK_STRING(tk, ":");
      tk = get_token();
 
      CALL(tipo);
   }
   else
   {
      return SYNTAXERROR;
   }
   return SUCCESS;
}

/*
Automato 3
Autor: Bruno

<variavel> ::= IDENT <dimensao> <mais_var> : <tipo>
<mais_var> ::= , <variavel> <mais_var>
					| epsilon
*/

int variavel()
{
   int ret;

	while (1)
	{
		CHECK_CLASS(tk, identifier);
		/* Inserção de nova variável na pending_stack e na tabela corrente. */
      SEM_TRY(sem_pending_insert(tk->string, variable), sem_error_ident_ja_declarado, tk->string);

      tk = get_token();

		CALL(dimensao);

		if( strcmp(tk->string, ",") != SUCCESS)
			break;

		tk = get_token();
	}
	CHECK_STRING(tk, ":");
   tk = get_token();

   CALL(tipo);

   /*gen_variable(sem_gl_info.current_table);*/

	/* Commit das informações das variáveis na pending_stack. */
   sem_pending_commit();

	return SUCCESS;
}


/*
Automato 4
Autor: Bruno

<outros_ident>                 ::= . IDENT <outros_ident> | epsilon
*/
int outros_ident()
{
	/* 
	 * Variável que irá contar quantas mudanças no contexto (global/local para registro
	 *  ou registro mais externo para registro mais intero) foram feitas.
	 *  Para cada "." encontrado, uma mudança de contexto é feita.
	 *  Essa variável será utilizada para retornar ao contexto anterior à entrada do registro.
	 */
	 
   int scope_changes = 0;

   while( strcmp(tk->string, ".") == SUCCESS)
   {
		/*
		 * Concatenação do identificador atual com o "."
		 * Isso é utilizado no caso: "vinho.preço", em que há a concatenação de vinho ".", 
		 * para posterior concatenação com "preço",
		 * considerando que a variável register_ident já contém "vinho".
		 */ 
      sem_register_ident_append(tk->string);
      tk = get_token();

      CHECK_CLASS(tk, identifier);
		
		/*
		 * Concatenação do identificador atual com o restante do identificador do registro já lido.
		 * Isso é utilizado no caso: "vinho.preço", em que há a concatenação de vinho com preço, 
		 * considerando que a variável register_ident já contém "vinho.".
		 */ 
      sem_register_ident_append(tk->string);
		
		/* Verifica se a variável está declarada no contexto atual (no caso, regitro). */
      SEM_TRY(sem_check(sem_check_variable_declared, tk->string), sem_error_ident_nao_declarado, sem_gl_info.register_ident);

		/* Atualiza o tipo da variável que está dentro do registro.
		 *  Isso é necessário pois apenas o último tipo importa.
		 *  Exemplo: "a.b.c <- algo", apenas o tipo de 'c' importa para a checagem de  consistência
		 *  da atribuição.
		 */
      sem_attrib_set(sem_type_of(tk->string));

		/* Muda o contexto para o registro interno ao atual. */
      sem_context_change(sem_scope_register_query);
		
      ++scope_changes;

      tk = get_token();
   }

	/* Retorna ao contexto anterior à chamada do registro. */
   while(scope_changes--)
      sem_context_change(sem_scope_register_end);



   return SUCCESS;
}
/*
Automato 5
Autor: Bruno

<dimensao>                     ::= [ <exp_aritmetica> ] <dimensao>| epsilon
*/

int dimensao()
{
   int ret;
	
	
   while( strcmp(tk->string, "[") == SUCCESS)
   {
		/* Os appends foram feitos para concatenar a string total de um vetor, exemplo "vetor[0]",
		 *  utilizado para a impressão correta do possível erro semântico.
		 */
      sem_register_ident_append(tk->string);
      tk = get_token();
      sem_register_ident_append(tk->string);

      CALL(exp_aritmetica);
      
		/*
		 * exp_aritmetica sempre empilha algo na attrib_stack, mas dentro de []
		 * não há verificação de tipo, portanto é necessário retirar esse tipo empilhado
		 * da pilha.
		 */
		sem_attrib_pop();


      CHECK_STRING(tk, "]");
      sem_register_ident_append(tk->string);
      tk = get_token();
   }
   return SUCCESS;
}
/*
Automato 6
Autor: Bruno
<tipo>                              ::= registro <variavel> <mais_variaveis> fim_registro
                                     | <tipo_estendido>

<mais_variaveis>                    ::= <variavel> <mais_variaveis> | epsilon
*/

int tipo()
{
   int ret;
   if( strcmp(tk->string, "registro") == SUCCESS)
   {

		/* Atualiza campo tipo da pending_changes para "registro" */
      sem_pending_update(sem_upd_type, tk->string);
		
		/*
		 * Muda o contexto de local/global para o contexto do novo registro
		 * que será criado.
		 */
      sem_context_change(sem_scope_register_insert);
 
      tk = get_token();

      do
      {
         CALL(variavel);
      } while(search_first(tk, identificador_firsts) == SUCCESS);   /* Primeiros(variavel) = primeiros(identificador) */

      CHECK_STRING(tk, "fim_registro");
      tk = get_token();


		
		/* Retorna o contexto de registro para o anterior à declaração do registro. */
      sem_context_change(sem_scope_register_end);
		
		/* Dá o commit das mudanças realizadas na pending_changes para todas as entradas da pending_stack */
      sem_pending_commit();
   }
   else
   {
      CALL(tipo_estendido);
   }

   return SUCCESS;
}

/*
Automato 7
Autor: Talita

tipo_estendido ::=  ^ IDENT
                  | ^ tipo_basico
                  | IDENT
                  | tipo_basico
<tipo_basico> ::= literal | inteiro | real | logico
*/

int tipo_estendido()
{
   if ( strcmp(tk->string, "^") == SUCCESS )
   {
		/* Atualização do campo ponteiro da pending_changes */
      sem_pending_update(sem_upd_pointer, (void*) sem_pt_type_pointer);
      tk = get_token();
   }

   if ( tk->class == identifier )
   {
		/* Verifica se o tipo existe. */
      SEM_TRY(sem_check(sem_check_type_declared, tk->string), sem_error_tipo_nao_declarado, tk->string);
   }
   else
   {
      CHECK_STRINGS(tk, "literal", "inteiro", "real", "logico");
	}
	
	/* Atualiza o campo tipo da pending_changes. */
	sem_pending_update(sem_upd_type, (void*) tk->string);
   tk = get_token();

   return SUCCESS;
}

/*
Automato 8
Autor: Talita

<declaracao_global> ::=  procedimento IDENT ( <parametros_opcional> ) <declaracoes_locais> <comandos> fim_procedimento
                       | funcao IDENT ( <parametros_opcional> ) : <tipo_estendido> <declaracoes_locais> <comandos> fim_funcao

<parametros_opcional> ::=  <parametro>
                     | epsilon
<declaracoes_locais> ::= <declaracao_local> <declaracoes_locais> | epsilon
<identificador>                 ::= ^ IDENT <outros_ident> <dimensao>
                                 |  IDENT <outros_ident> <dimensao>
*/

int declaracao_global()
{
   int ret;

   if ( strcmp(tk->string, "procedimento") == SUCCESS )
   {
      tk = get_token();

      CHECK_CLASS(tk, identifier);
		
		/* Inserção do nome do procedimento na pending_stack e na tabela global. */
      SEM_TRY(sem_pending_insert(tk->string, procedure), sem_error_ident_ja_declarado, tk->string);
      tk = get_token();

      CHECK_STRING(tk, "(");
      tk = get_token();

      if ( search_first(tk, parametro_firsts) == SUCCESS )
      {
         CALL(parametro);
      }

      CHECK_STRING(tk, ")");
      tk = get_token();
		
		/*
		 * Atualiza o tipo da pending_changes para SEM_TYPE_UNDEFINED, 
		 *  pois esse campo deve ser indefinido para procedimento.
		 */
      sem_pending_update(sem_upd_type, SEM_TYPE_UNDEFINED);

		/* Atualiza os campos de pending_changes para todas as entradas na pending_stack. */
      sem_pending_commit();

		/* Muda do escopo global para local (interno ao procedimento). */
      sem_context_change(sem_scope_global_to_local);

      while ( search_first(tk, declaracao_local_firsts) == SUCCESS )
      	CALL(declaracao_local);

      CALL(comandos);

      CHECK_STRING(tk, "fim_procedimento");
      tk = get_token();

		/* Retorna do escopo local (procedimento) para o local. */
      sem_context_change(sem_scope_local_to_global);
   }
   else if ( strcmp(tk->string, "funcao") == SUCCESS )
   {
      tk = get_token();

      CHECK_CLASS(tk, identifier);
		
		/* Inserção do nome da função na pending_stack e na tabela global. */
      SEM_TRY(sem_pending_insert(tk->string, function), sem_error_ident_ja_declarado, tk->string);

      tk = get_token();

      CHECK_STRING(tk, "(");
      tk = get_token();

      if ( search_first(tk, parametro_firsts) == SUCCESS )
         CALL(parametro);

      CHECK_STRING(tk, ")");
      tk = get_token();

      CHECK_STRING(tk, ":");
      tk = get_token();

      CALL(tipo_estendido); /* O tipo na pending_changes, nesse caso, é o tipo de retorno.*/

		/* Permite o comando retorne no escopo (escopo de função)*/
      sem_context_change(sem_scope_allows_return);
		
		/* Atualiza os campos de pending_changes para todas as entradas na pending_stack. */
      sem_pending_commit();

		/* Muda do escopo global para local (interno à função). */		
      sem_context_change(sem_scope_global_to_local);

      while ( search_first(tk, declaracao_local_firsts) == SUCCESS )
	      CALL(declaracao_local);

      CALL(comandos);

      CHECK_STRING(tk, "fim_funcao");
      tk = get_token();

		/* Retorna do escopo local (procedimento) para o local. */
      sem_context_change(sem_scope_local_to_global);
		
		/*
		 * Proíbe o comando retorne no escopo, visto que apenas em função 
		 *  é permitido o comando retorne 
		 */
      sem_context_change(sem_scope_forbids_return);
   }

   return SUCCESS;
}

/*
Automato 9
Autor: Talita

<parametro> ::= <var_opcional> <identificador> <mais_ident> : <tipo_estendido> <mais_parametros>

<var_opcional> ::= var | epsilon

<mais_parametros> ::= , <parametro> | epsilon
<mais_ident>                   ::= , <identificador> <mais_ident> | epsilon

<identificador>                 ::= ^ IDENT <outros_ident> <dimensao>
                                 |  IDENT <outros_ident> <dimensao>

*/

int parametro()
{
   int ret;

   while(1)
   {
      if ( strcmp(tk->string, "var") == SUCCESS )
      {
         sem_pending_update(sem_upd_pointer, (void*) sem_pt_type_var);
         tk = get_token();
      }
		
      while(1)
   	{
      	if( strcmp(tk->string, "^") == SUCCESS)
      	{
      	   sem_pending_update(sem_upd_type, (void*)sem_pt_type_pointer);
            tk = get_token();
      	}

         CHECK_CLASS(tk, identifier);
         sem_pending_update(sem_upd_param_insert, tk->string);
         tk = get_token();

			/*
			 * Ignorado, pois consideramos não semântico declarar a.b.c 
			 *  no parâmetro formal de função
			 */
         /*CALL(outros_ident); */

         CALL(dimensao);

         if(strcmp(tk->string, ",") != SUCCESS) break;
         tk = get_token();
   	}

      CHECK_STRING(tk, ":");
      tk = get_token();

      CALL(tipo_estendido);

		/* Insere na lista de parâmetros o novo parâmetro na pending_changes. */
      sem_pending_update(sem_upd_param_update, NULL);

      if ( strcmp(tk->string, ",") != SUCCESS ) break;
      tk = get_token();

   }

   return SUCCESS;
}

/*
Automato 10
Autor: Talita

<comandos> ::=  <cmd_leia> <comandos> | <cmd_escreva> <comandos> | <cmd_se> <comandos> | <cmd_caso> <comandos>
              | <cmd_para> <comandos> | <cmd_enquanto> <comandos> | <cmd_faca> <comandos> | <cmd_pont_ident> <comandos>
              | <cmd_ident> <comandos> | retorne <expressao> <comandos> | epsilon
*/

int comandos()
{
   int ret;

   while ( search_first(tk, comandos_firsts) == SUCCESS )
   {
      if ( tk->class == identifier )
      {
         CALL(cmd_ident);
      }
      else if ( strcmp(tk->string, "leia") == SUCCESS )
      {
         CALL(cmd_leia);
      }
      else if ( strcmp(tk->string, "escreva" ) == SUCCESS )
      {
         CALL(cmd_escreva);
      }
      else if ( strcmp(tk->string, "se" ) == SUCCESS )
      {
         CALL(cmd_se);
      }
      else if ( strcmp(tk->string, "caso" ) == SUCCESS )
      {
         CALL(cmd_caso);
      }
      else if ( strcmp(tk->string, "para" ) == SUCCESS )
      {
         CALL(cmd_para);
      }
      else if ( strcmp(tk->string, "enquanto" ) == SUCCESS )
      {
         CALL(cmd_enquanto);
      }
      else if ( strcmp(tk->string, "faca" ) == SUCCESS )
      {
         CALL(cmd_faca);
      }
      else if ( strcmp(tk->string, "retorne" ) == SUCCESS )
      {
         CALL(cmd_retorne);
      }
      else /* tk->string = ^ */
      {
         CALL(cmd_pont_ident);
      }
   }

   return SUCCESS;
}

/*
Automato 11
Autor: Talita

<cmd_leia> ::= leia ( <identificador> <mais_ident> )
<mais_ident>                   ::= , <identificador> <mais_ident> | epsilon
*/

int cmd_leia()
{
   int ret;

   CHECK_STRING(tk, "leia");
   tk = get_token();

   CHECK_STRING(tk, "(");

   do
   {
      tk = get_token();
		
      if( strcmp(tk->string, "^") == SUCCESS)
         tk = get_token();

      CALL(identificador_novo);
		
		/* Não há restrições de tipo para o comando leia. */
      sem_attrib_pop();

   }while( strcmp(tk->string, ",") == SUCCESS);

   CHECK_STRING(tk, ")");
   tk = get_token();

   return SUCCESS;
}

/*
Automato 12
Autor: Talita

<cmd_escreva> ::= escreva ( <expressao> <mais_expressao> )
<mais_expressao>                 ::= , <expressao> <mais_expressao>
							       | epsilon

*/

int cmd_escreva()
{
   int ret;

   CHECK_STRING(tk, "escreva");
   tk = get_token();

   CHECK_STRING(tk, "(");

  	do
   {
      tk = get_token();
      CALL(expressao);
		
		/* Não há restrições de tipo para o comando escreva. */
      sem_attrib_pop();

   }while(strcmp(tk->string, ",") == SUCCESS);

   CHECK_STRING(tk, ")");
   tk = get_token();

   return SUCCESS;
}

/*
Automato 13
Autor: Lucas

<cmd_se> ::= se <expressao> entao <comandos> senao <comandos> fim_se
             se <expressao> entao <comandos> fim_se
*/

int cmd_se()
{
   int ret;

   CHECK_STRING(tk, "se");
   tk = get_token();

   CALL(expressao);
	
	/* Não é feita a checagem para o tipo de expressão no comando se. */
   sem_attrib_pop();

   CHECK_STRING(tk, "entao");
   tk = get_token();

   CALL(comandos);

   if( strcmp(tk->string, "senao") == SUCCESS )
   {
      tk = get_token();
      CALL(comandos);
   }

   CHECK_STRING(tk, "fim_se");
   tk = get_token();

   return SUCCESS;
}

/*
Automato 14
Autor: Lucas

<cmd_caso> ::= caso <exp_aritmetica> seja <selecao> <senao_opcional> fim_caso
<senao_opcional> ::= senao <comandos> | epsilon
*/

int cmd_caso()
{
   int ret;

   CHECK_STRING(tk, "caso");
   tk = get_token();

   CALL(exp_aritmetica);
	
	/* Não é feita a checagem para o tipo de expressão no comando caso. */
	sem_attrib_pop();

   CHECK_STRING(tk, "seja");
   tk = get_token();

   CALL(selecao);

   if( strcmp(tk->string, "senao") == SUCCESS )
   {
      tk = get_token();
      CALL(comandos);
   }

   CHECK_STRING(tk, "fim_caso");
   tk = get_token();

   return SUCCESS;
}


/*
Automato 15
Autor: Lucas

<cmd_para>  ::= para IDENT <- <exp_aritmetica> ate <exp_aritmetica> faca <comandos> fim_para
*/

int cmd_para()
{
   int ret;

   CHECK_STRING(tk, "para");
   tk = get_token();

   CHECK_CLASS(tk, identifier);
	
	/* Verifica a existência do identificador. */
   SEM_TRY(sem_check(sem_check_variable_declared, tk->string), sem_error_ident_nao_declarado, tk->string);
   tk = get_token();

   CHECK_STRING(tk, "<-");
   tk = get_token();

   CALL(exp_aritmetica);
	sem_attrib_pop();
	
   CHECK_STRING(tk, "ate");
   tk = get_token();

   CALL(exp_aritmetica);
	sem_attrib_pop();

   CHECK_STRING(tk, "faca");
   tk = get_token();

   CALL(comandos);

   CHECK_STRING(tk, "fim_para");
   tk = get_token();

   return SUCCESS;
}


/*
Automato 16
Autor: Lucas

<cmd_enquanto> ::= enquanto <expressao> faca <comandos> fim_enquanto

*/

int cmd_enquanto()
{
   int ret;

   CHECK_STRING(tk, "enquanto");
   tk = get_token();

   CALL(expressao);
   sem_attrib_pop();

   CHECK_STRING(tk, "faca");
   tk = get_token();

   CALL(comandos);

   CHECK_STRING(tk, "fim_enquanto");
   tk = get_token();

   return SUCCESS;
}

/*
Automato 17
Autor: Lucas

<cmd_faca> ::= faca <comandos> ate <expressao>
*/

int cmd_faca()
{
   int ret;

   CHECK_STRING(tk, "faca");
   tk = get_token();

   CALL(comandos);

   CHECK_STRING(tk, "ate");
   tk = get_token();

   CALL(expressao);
   sem_attrib_pop();

   return SUCCESS;
}

/*
Automato 18
Autor: Lucas

<cmd_pont_ident> ::= ^ IDENT <outros_ident> <dimensao> <- <expressao>
*/

int cmd_pont_ident()
{
   int ret;
   char* left_ident;

   CHECK_STRING(tk, "^");
   sem_register_ident_set(tk->string);
   tk = get_token();

   CALL(identificador_novo);
   /* identificador_novo ja coloca seu tipo esperado na pilha */

   left_ident = (char *) malloc(sizeof(char)*(1+strlen(sem_gl_info.register_ident)));
   strcpy(left_ident, sem_gl_info.register_ident);

   CHECK_STRING(tk, "<-");
   tk = get_token();

   CALL(expressao);
   SEM_TRY(sem_check(sem_check_attr, NULL), sem_error_atrib_nao_compativel, left_ident);

   free(left_ident);

   return SUCCESS;
}

/*
Automato 19
Autor: Lucas

<cmd_ident> ::= IDENT <chamada_atribuicao>
<chamada_atribuicao> ::= ( <expressao> <mais_expressao> )
                           | <outros_ident> <dimensao> <- <expressao>
<mais_expressao>                 ::= , <expressao> <mais_expressao>
							       | epsilon
*/

int cmd_ident()
{
   int ret;
   token* last_token;
   token* cur_token;

   CHECK_CLASS(tk, identifier);

   last_token           = (token*) malloc(sizeof(token));
   last_token->string   = (char*) malloc((strlen(tk->string)+1)*sizeof(char));
   last_token->class    = tk->class;
   strcpy(last_token->string, tk->string);

   tk = get_token();

   cur_token = tk;

   if( strcmp(tk->string, "(") == SUCCESS )
   {
      list* param_list = sem_list_of(last_token->string);
      sem_entry* entry;
      int error_occurred = 0;
      int i = 0;

      do
   	{
      	tk = get_token();
      	CALL(expressao);

         entry = list_elem_at(param_list, i);

         if(!entry)
         {
            if(!error_occurred)
            {
               sem_error(sem_error_incomp_de_parametros, last_token->string);
               error_occurred = 1;
            }
            sem_attrib_pop();
         }
         else
         {
            sem_attrib_push(entry->type);

            if(!error_occurred)
            {
               if(sem_check(sem_check_attr, NULL) != SUCCESS)
               {
                  sem_error(sem_error_incomp_de_parametros, last_token->string);
                  error_occurred = 1;
               }
            }
         }

         i++;
   	}while(strcmp(tk->string, ",") == SUCCESS);

      CHECK_STRING(tk, ")");
      tk = get_token();
   }
   else
   {
      char* left_ident;

      /*stack_print(sem_gl_info.attrib_stack);*/

      sem_register_ident_set(last_token->string);
      SEM_TRY(sem_check(sem_check_variable_declared, last_token->string), sem_error_ident_nao_declarado, sem_gl_info.register_ident);

      sem_attrib_set(sem_type_of(last_token->string));

      tk = last_token;
      sem_context_change(sem_scope_register_query);
      tk = cur_token;

      CALL(outros_ident);


      sem_attrib_push(NULL);


      /*stack_print(sem_gl_info.attrib_stack);*/

      sem_context_change(sem_scope_register_end);

      CALL(dimensao);

      left_ident = (char *) malloc(sizeof(char)*(1+strlen(sem_gl_info.register_ident)));
      strcpy(left_ident, sem_gl_info.register_ident);

      CHECK_STRING(tk, "<-");
      tk = get_token();

      CALL(expressao);

      SEM_TRY(sem_check(sem_check_attr, NULL), sem_error_atrib_nao_compativel, left_ident);

      free(left_ident);
   }


   free(last_token->string);
   free(last_token);

   return SUCCESS;
}


/*
Automato 20
Autor: Lucas

<cmd_retorne> ::= retorne <expressao>
*/

int cmd_retorne()
{
   int ret;

   CHECK_STRING(tk, "retorne");
   SEM_TRY(sem_check(sem_check_return_allowed, NULL), sem_error_retorne_nao_permitido, NULL);

   sem_attrib_push(sem_gl_info.context_return_type);

   tk = get_token();

   CALL(expressao);

   /* Para habilitar a checagem do tipo do retorne, descomentar a linha do SEM_TRY e comentar a seguinte */
   /*SEM_TRY(sem_check(sem_check_attr, NULL), sem_error_atrib_nao_compativel, "retorne");*/
   sem_pop_check_push(NULL);

   return SUCCESS;
}

/*
Automato 21
Autor: Marcos

<selecao>                        ::= <constantes> : <comandos> <selecao>
                                   | <constantes> : <comandos>
*/

int selecao()
{
	int ret;

	do
   {
      CALL(constantes);

	   CHECK_STRING(tk,":");
	   tk = get_token();

	   CALL(comandos);
   }while( search_first(tk,selecao_firsts) == SUCCESS);

	return SUCCESS;
}

/*
Automato 22
Autor: Marcos

<constantes>             ::=   NUM_INT .. - NUM_INT , <constantes>
									| - NUM_INT .. NUM_INT , <constantes>
									| - NUM_INT .. - NUM_INT , <constantes>
									|   NUM_INT .. NUM_INT , <constantes>
									| - NUM_INT , <constantes>
									|   NUM_INT , <constantes>
                           |   NUM_INT .. - NUM_INT
									| - NUM_INT .. NUM_INT
									| - NUM_INT .. - NUM_INT
									|   NUM_INT .. NUM_INT
									| - NUM_INT
									|   NUM_INT
*/

int constantes()
{
	while(1)
	{
		if(strcmp(tk->string,"-") == SUCCESS)
			tk = get_token();

		CHECK_CLASS(tk,integer_number);
		tk = get_token();

		if(strcmp(tk->string,"..") == SUCCESS)
		{
			tk = get_token();

			if(strcmp(tk->string,"-") == SUCCESS)
				tk = get_token();

			CHECK_CLASS(tk,integer_number);
			tk = get_token();

			if(strcmp(tk->string,",") == SUCCESS)
				tk = get_token();
			else
				break;
		}
		else if(strcmp(tk->string,",") == SUCCESS)
			tk = get_token();
      else
         break;
	}
	return SUCCESS;
}

/*
Automato 23
Autor: Marcos

<exp_aritmetica>                 ::= <termo> <outros_termos>
<outros_termos>                  ::= <op_adicao> <termo> <outros_termos> | epsilon
<op_adicao> ::= + | -

*/

int exp_aritmetica()
{
   int ret;

	CALL(termo);

	while(strcmp(tk->string, "+") == SUCCESS || strcmp(tk->string, "-") == SUCCESS)
   {
      /*sem_attrib_enforce_top_type(SEM_TYPE_NUMBER);*/

	   tk = get_token();
	   CALL(termo);

	   sem_pop_check_push(sem_attrib_peek());
   }

	return SUCCESS;
}

/*
Automato 24
Autor: Marcos

<termo>                          ::= <fator> <outros_fatores>
<outros_fatores>                 ::= * <fator> <outros_fatores>
								   | / <fator> <outros_fatores>
								   | e
*/

int termo()
{
   int ret;

	CALL(fator);

	while(strcmp(tk->string, "*") ==  SUCCESS || strcmp(tk->string, "/") == SUCCESS)
   {
      sem_attrib_enforce_top_type(SEM_TYPE_NUMBER);

      tk = get_token();
      CALL(fator);

      sem_pop_check_push("inteiro");
   }

	return SUCCESS;
}

/*
Automato 25
Autor: Marcos

<fator>                          ::= <parcela> <outras_parcelas>

<parcela> ::= - ^ IDENT <outros_ident> <dimensao>
            | - IDENT <chamada_partes>
            | - NUM_INT
            | - NUM_REAL
            | - ( <expressao> )
            | ^ IDENT <outros_ident> <dimensao>
            | IDENT <chamada_partes>
            | NUM_INT
            | NUM_REAL
            | ( <expressao> )
            | & IDENT <outros_ident> <dimensao>
            | CADEIA

<outras_parcelas> ::= % <parcela> <outras_parcelas> | epsilon
*/

int fator()
{
   int ret;

   /*if( strcmp(tk->string, "&") == SUCCESS || tk->class == string )
   {*/
   if(strcmp(tk->string,"&") == SUCCESS)
   {
       tk = get_token();

       CALL(identificador_novo);

       /*sem_attrib_pop();            Não interessa o tipo de identificador */
       /*sem_attrib_push("inteiro");   Se é um endereço = inteiro */

   }
   else if(tk->class == string)
   {
      sem_attrib_push("literal");
      tk = get_token();
   }
   /*}*/
   else
   {
      if(strcmp(tk->string,"-") == SUCCESS)
      {
         tk = get_token();
      }

      if(strcmp(tk->string,"^") == SUCCESS)
      {
         tk = get_token();

         CALL(identificador_novo);

      }
      else if(tk->class == identifier)
      {
         SEM_TRY(sem_check(sem_check_any_declared, tk->string), sem_error_ident_nao_declarado, tk->string);

         token* last_token;
         token* cur_token;

         last_token           = (token*) malloc(sizeof(token));
         last_token->string   = (char*) malloc((strlen(tk->string)+1)*sizeof(char));
         last_token->class    = tk->class;
         strcpy(last_token->string, tk->string);

         sem_register_ident_set(tk->string);
         sem_attrib_push(sem_type_of(tk->string));

         tk = get_token();
         cur_token = tk;

         /* Chamada partes */
         if(strcmp(tk->string,"(") == SUCCESS)
         {
            list* param_list = sem_list_of(last_token->string);
            sem_entry* entry;
            int error_occurred = 0;
            int i = 0;

            if(!param_list)
               error_occurred = 1;

            do
            {
               tk = get_token();
               CALL(expressao);

               entry = list_elem_at(param_list, i);

               if(!entry)
               {
                  if(!error_occurred)
                  {
                     sem_error(sem_error_incomp_de_parametros, last_token->string);
                     error_occurred = 1;
                  }
                  sem_attrib_pop();
               }
               else
               {
                  sem_attrib_push(entry->type);

                  if(!error_occurred)
                  {
                     if(sem_check(sem_check_attr, last_token->string) != SUCCESS)
                     {
                        sem_error(sem_error_incomp_de_parametros, last_token->string);
                        error_occurred = 1;
                     }
                  }
               }

               i++;
            }while(strcmp(tk->string, ",") == SUCCESS);

            CHECK_STRING(tk,")");
            tk = get_token();
            /*
            do
            {
               tk = get_token();
               CALL(expressao);
            }while(strcmp(tk->string, ",") == SUCCESS);

            CHECK_STRING(tk,")");
            tk = get_token();
            */
         }
         else if(strcmp(tk->string,".") == SUCCESS || strcmp(tk->string,"[") == SUCCESS)
         {
            if(strcmp(tk->string,".") == SUCCESS)
            {
               sem_attrib_pop();

               tk = last_token;
               sem_context_change(sem_scope_register_query);
               tk = cur_token;

               CALL(outros_ident);
               sem_context_change(sem_scope_register_end);

               sem_attrib_push(NULL);
            }

            CALL(dimensao);
         }
         /* Fim chamada partes */

      }
      else if(tk->class == integer_number)
      {
         sem_attrib_push("inteiro");
         tk = get_token();
      }
      else if(tk->class == real_number)
      {
         sem_attrib_push("real");
         tk = get_token();
      }
      else
      {
         CHECK_STRING(tk,"(");
         tk = get_token();

         CALL(expressao);

         CHECK_STRING(tk,")");
         tk = get_token();
      }
   }



   if(strcmp(tk->string,"%") == SUCCESS)
   {
      sem_attrib_enforce_top_type(SEM_TYPE_NUMBER);

      tk = get_token();
      CALL(fator);

      sem_pop_check_push("inteiro");

      /* O sem_pop_check_push, como está, aceita fazer real % inteiro e inteiro % real */
   }


	return SUCCESS;
}

/*
Automato 26
Autor: Nathan

<chamada_partes>  ::= ( <expressao> <mais_expressao> )
                    | <outros_ident> <dimensao>
                    | epsilon

<mais_expressao>                 ::= , <expressao> <mais_expressao>
							       | epsilon


int chamada_partes()
{
   int ret;

   printf("WARNING! %d SAI DAQUI!\n", line_number);

   if(strcmp(tk->string,"(") == SUCCESS)
   {
      do
   	{
      	tk = get_token();
      	CALL(expressao);
   	}while(strcmp(tk->string, ",") == SUCCESS);

      CHECK_STRING(tk,")");
      tk = get_token();
   }
   else if(strcmp(tk->string,".") == SUCCESS || strcmp(tk->string,"[") == SUCCESS)
   {
      sem_attrib_pop();
      CALL(outros_ident);

      sem_attrib_push(NULL);

      CALL(dimensao);
   }
   return SUCCESS;
}*/

/*
Automato 27
Autor: Nathan

<parcela_logica> ::= verdadeiro
           | falso
           | <exp_aritmetica> = <exp_aritmetica>
           | <exp_aritmetica> <> <exp_aritmetica>
           | <exp_aritmetica> >= <exp_aritmetica>
           | <exp_aritmetica> <= <exp_aritmetica>
           | <exp_aritmetica> > <exp_aritmetica>
           | <exp_aritmetica> < <exp_aritmetica>
           | <exp_aritmetica>

*/

int parcela_logica()
{
   int ret;

   if(strcmp(tk->string,"verdadeiro") == SUCCESS || strcmp(tk->string,"falso") == SUCCESS)
   {
      sem_attrib_push("logico");
      tk = get_token();
   }
   else
   {
      CALL(exp_aritmetica);

      if(strcmp(tk->string,"=") ==  SUCCESS || strcmp(tk->string,"<>") ==  SUCCESS || strcmp(tk->string,">=") ==  SUCCESS ||
         strcmp(tk->string,"<=") ==  SUCCESS || strcmp(tk->string,">") ==  SUCCESS || strcmp(tk->string,"<") ==  SUCCESS)
      {
         tk = get_token();
         CALL(exp_aritmetica);

         sem_pop_check_push("logico");
      }

   }
   return SUCCESS;
}


/*
Automato 28
Autor: Nathan

<expressao>                      ::= <termo_logico> <outros_termos_logicos>

<termo_logico> ::= nao <parcela_logica> <outros_fatores_logicos>
                 | <parcela_logica> <outros_fatores_logicos>

<outros_termos_logicos>   ::= ou <expressao>  | epsilon


*/
int expressao()
{
   int ret;

   if(strcmp(tk->string,"nao") == SUCCESS)
   {
      tk = get_token();
      CALL(parcela_logica);
      sem_attrib_enforce_top_type("logico");
   }
   else
   {
      CALL(parcela_logica);
   }

   CALL(outros_fatores_logicos);

   sem_pop_check_push(sem_attrib_peek());

   if (strcmp(tk->string,"ou") == SUCCESS)
   {
      sem_attrib_enforce_top_type("logico");

      tk = get_token();
      CALL(expressao);

      sem_pop_check_push("logico");
   }

   return SUCCESS;
}

/*
Automato 29
Autor: Nathan

<outros_fatores_logicos>   ::= e nao <parcela_logica> <outros_fatores_logicos>
                             | e <parcela_logica> <outros_fatores_logicos>
                             | epsilon
*/

int outros_fatores_logicos()
{
   int ret;

   if(strcmp(tk->string,"e") == SUCCESS)
   {
      tk = get_token();

      if(strcmp(tk->string,"nao") == SUCCESS)
         tk = get_token();

      CALL(parcela_logica);
      sem_attrib_enforce_top_type("logico");

      CALL(outros_fatores_logicos);
      sem_pop_check_push("logico");

   }
   else
   {
      sem_attrib_push(sem_attrib_peek());
   }
   return SUCCESS;
}

/*
Automato 30

Novo identificador, que agrupa algumas chamadas comuns dos automatos
cmd_leia
cmd_pont_ident
fator

*/
int identificador_novo()
{
   int ret;

   CHECK_CLASS(tk, identifier);

   sem_register_ident_set(tk->string);

   SEM_TRY(sem_check(sem_check_variable_declared, tk->string), sem_error_ident_nao_declarado, sem_gl_info.register_ident);

   sem_attrib_set(sem_type_of(tk->string));

   sem_context_change(sem_scope_register_query);

   tk = get_token();

   CALL(outros_ident);

   sem_attrib_push(NULL);

   sem_context_change(sem_scope_register_end);

   CALL(dimensao);

   return SUCCESS;
}
