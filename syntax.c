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
*/

int programa()
{
   int ret;

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
   tk = get_token();

   CALL(declaracoes_locais);
   CALL(comandos);

   CHECK_STRING(tk, "fim_algoritmo");
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
      tk = get_token();

      CHECK_STRING(tk, ":");
      tk = get_token();

      CALL(tipo_basico);

      CHECK_STRING(tk, "=");
      tk = get_token();

      if( tk->class != string && tk->class != integer_number && tk->class != real_number)
         CHECK_STRINGS(tk, "verdadeiro", "falso");
      tk = get_token();
   }
   else if( strcmp(tk->string, "tipo") == SUCCESS)
   {
      tk = get_token();

      CHECK_CLASS(tk, identifier);
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

<variavel>                   ::= <identificador> <mais_ident> : <tipo>
*/
int variavel()
{
   int ret;

   CALL(identificador);
   CALL(mais_ident);

   CHECK_STRING(tk, ":");
   tk = get_token();

   CALL(tipo);
   return SUCCESS;
}

/*
Automato 4
Autor: Bruno

<identificador>                 ::= ^ IDENT <outros_ident> <dimensao>
                                 |  IDENT <outros_ident> <dimensao>
*/
int identificador()
{
   int ret;

   if( strcmp(tk->string, "^") == SUCCESS)
      tk = get_token();

   CHECK_CLASS(tk, identifier);
   tk = get_token();

   CALL(outros_ident);
   CALL(dimensao);

   return SUCCESS;
}
/*

Automato 5
Autor: Bruno

<outros_ident>                 ::= . IDENT <outros_ident> | epsilon
*/
int outros_ident()
{
   while( strcmp(tk->string, ".") == SUCCESS)
   {
      tk = get_token();

      CHECK_CLASS(tk, identifier);
      tk = get_token();
   }

   return SUCCESS;
}
/*
Automato 6
Autor: Bruno

<dimensao>                     ::= [ <exp_aritmetica> ] <dimensao>| epsilon
*/
int dimensao()
{
   int ret;
   while( strcmp(tk->string, "[") == SUCCESS)
   {
      tk = get_token();

      CALL(exp_aritmetica);

      CHECK_STRING(tk, "]");
      tk = get_token();
   }
   return SUCCESS;
}
/*
Automato 7
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
      tk = get_token();

      do
      {
         CALL(variavel);
      } while(search_first(tk, identificador_firsts) == SUCCESS);   /* Primeiros(variavel) = primeiros(identificador) */

      CHECK_STRING(tk, "fim_registro");
      tk = get_token();
   }
   else
   {
      CALL(tipo_estendido);
   }
   return SUCCESS;
}

/*
Automato 8
Autor: Bruno

<mais_ident>                   ::= , <identificador> <mais_ident> | epsilon
*/
int mais_ident()
{
   int ret;
   while( strcmp(tk->string, ",") == SUCCESS)
   {
      tk = get_token();
      CALL(identificador);
   }
   return SUCCESS;
}

/*
Automato 9
Autor: Talita

<tipo_basico> ::= literal | inteiro | real | logico
*/

int tipo_basico()
{
   CHECK_STRINGS(tk, "literal", "inteiro", "real", "logico");
   tk = get_token();

   return SUCCESS;
}

/*
Automato 10
Autor: Talita

tipo_estentido ::=  ^ IDENT
                  | ^ tipo_basico
                  | IDENT
                  | tipo_basico
*/

int tipo_estendido()
{
   int ret;

   if ( strcmp(tk->string, "^") == SUCCESS )
      tk = get_token();

   if ( tk->class == identifier )
      tk = get_token();
   else
      CALL(tipo_basico);

   return SUCCESS;
}

/*
Automato 11
Autor: Talita

<declaracao_global> ::=  procedimento IDENT ( <parametros_opcional> ) <declaracoes_locais> <comandos> fim_procedimento
                       | funcao IDENT ( <parametros_opcional> ) : <tipo_estendido> <declaracoes_locais> <comandos> fim_funcao

<parametros_opcional> ::=  <parametro>
                     | epsilon
*/

int declaracao_global()
{
   int ret;

   if ( strcmp(tk->string, "procedimento") == SUCCESS )
   {
      tk = get_token();

      CHECK_CLASS(tk, identifier);
      tk = get_token();

      CHECK_STRING(tk, "(");
      tk = get_token();

      if ( search_first(tk, parametro_firsts) == SUCCESS )
         CALL(parametro);

      CHECK_STRING(tk, ")");
      tk = get_token();

      CALL(declaracoes_locais);
      CALL(comandos);

      CHECK_STRING(tk, "fim_procedimento");
      tk = get_token();

   }
   else if ( strcmp(tk->string, "funcao") == SUCCESS )
   {
      tk = get_token();

      CHECK_CLASS(tk, identifier);
      tk = get_token();

      CHECK_STRING(tk, "(");
      tk = get_token();

      if ( search_first(tk, parametro_firsts) == SUCCESS )
         CALL(parametro);

      CHECK_STRING(tk, ")");
      tk = get_token();

      CHECK_STRING(tk, ":");
      tk = get_token();

      CALL(tipo_estendido);
      CALL(declaracoes_locais);
      CALL(comandos);

      CHECK_STRING(tk, "fim_funcao");
      tk = get_token();
   }

   return SUCCESS;
}

/*
Automato 12
Autor: Talita

<parametro> ::= <var_opcional> <identificador> <mais_ident> : <tipo_estendido> <mais_parametros>

<var_opcional> ::= var | epsilon

<mais_parametros> ::= , <parametro> | epsilon

*/

int parametro()
{
   int ret;

   while(1)
   {
      if ( strcmp(tk->string, "var") == SUCCESS )
         tk = get_token();

      CALL(identificador);
      CALL(mais_ident);

      CHECK_STRING(tk, ":");
      tk = get_token();

      CALL(tipo_estendido);

      if ( strcmp(tk->string, ",") != SUCCESS )
         break;

      tk = get_token();
   }

   return SUCCESS;
}

/*
Automato 13
Autor: Talita

<declaracoes_locais> ::= <declaracao_local> <declaracoes_locais> | epsilon

*/

int declaracoes_locais()
{
   int ret;

   while ( search_first(tk, declaracao_local_firsts) == SUCCESS )
      CALL(declaracao_local);

   return SUCCESS;
}

/*
Automato 14
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
Automato 15
Autor: Talita

<cmd_leia> ::= leia ( <identificador> <mais_ident> )
*/

int cmd_leia()
{
   int ret;

   CHECK_STRING(tk, "leia");
   tk = get_token();

   CHECK_STRING(tk, "(");
   tk = get_token();

   CALL(identificador);
   CALL(mais_ident);

   CHECK_STRING(tk, ")");
   tk = get_token();

   return SUCCESS;
}

/*
Automato 16
Autor: Talita

<cmd_escreva> ::= escreva ( <expressao> <mais_expressao> )
*/

int cmd_escreva()
{
   int ret;

   CHECK_STRING(tk, "escreva");
   tk = get_token();

   CHECK_STRING(tk, "(");
   tk = get_token();

   CALL(expressao);
   CALL(mais_expressao);

   CHECK_STRING(tk, ")");
   tk = get_token();

   return SUCCESS;
}

/*
Automato 17
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
Automato 18
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
Automato 19
Autor: Lucas

<cmd_para>  ::= para IDENT <- <exp_aritmetica> ate <exp_aritmetica> faca <comandos> fim_para
*/

int cmd_para()
{
   int ret;

   CHECK_STRING(tk, "para");
   tk = get_token();

   CHECK_CLASS(tk, identifier);
   tk = get_token();

   CHECK_STRING(tk, "<-");
   tk = get_token();

   CALL(exp_aritmetica);

   CHECK_STRING(tk, "ate");
   tk = get_token();

   CALL(exp_aritmetica);

   CHECK_STRING(tk, "faca");
   tk = get_token();

   CALL(comandos);

   CHECK_STRING(tk, "fim_para");
   tk = get_token();

   return SUCCESS;
}


/*
Automato 20
Autor: Lucas

<cmd_enquanto> ::= enquanto <expressao> faca <comandos> fim_enquanto

*/

int cmd_enquanto()
{
   int ret;

   CHECK_STRING(tk, "enquanto");
   tk = get_token();

   CALL(expressao);

   CHECK_STRING(tk, "faca");
   tk = get_token();

   CALL(comandos);

   CHECK_STRING(tk, "fim_enquanto");
   tk = get_token();

   return SUCCESS;
}

/*
Automato 21
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

   return SUCCESS;
}

/*
Automato 22
Autor: Lucas

<cmd_pont_ident> ::= ^ IDENT <outros_ident> <dimensao> <- <expressao>
*/

int cmd_pont_ident()
{
   int ret;

   CHECK_STRING(tk, "^");
   tk = get_token();

   CHECK_CLASS(tk, identifier);
   tk = get_token();

   CALL(outros_ident);
   CALL(dimensao);

   CHECK_STRING(tk, "<-");
   tk = get_token();

   CALL(expressao);

   return SUCCESS;
}

/*
Automato 23
Autor: Lucas

<cmd_ident> ::= IDENT <chamada_atribuicao>
<chamada_atribuicao> ::= ( <expressao> <mais_expressao> )
                           | <outros_ident> <dimensao> <- <expressao>
*/

int cmd_ident()
{
   int ret;

   CHECK_CLASS(tk, identifier);
   tk = get_token();

   if( strcmp(tk->string, "(") == SUCCESS )
   {
      tk = get_token();

      CALL(expressao);
      CALL(mais_expressao);

      CHECK_STRING(tk, ")");
      tk = get_token();
   }
   else
   {
      CALL(outros_ident);
      CALL(dimensao);

      CHECK_STRING(tk, "<-");
      tk = get_token();

      CALL(expressao);
   }

   return SUCCESS;
}


/*
Automato 24
Autor: Lucas

<cmd_retorne> ::= retorne <expressao>
*/

int cmd_retorne()
{
   int ret;

   CHECK_STRING(tk, "retorne");
   tk = get_token();

   CALL(expressao);

   return SUCCESS;
}

/*
Automato 25
Autor: Marcos

<mais_expressao>                 ::= , <expressao> <mais_expressao>
							       | epsilon
*/

int mais_expressao()
{
	int ret;

	while(strcmp(tk->string, ",") == SUCCESS)
   {
      tk = get_token();
      CALL(expressao);
   }

	return SUCCESS;
}

/*
Automato 26
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
Automato 27
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
Automato 28
Autor: Marcos

<exp_aritmetica>                 ::= <termo> <outros_termos>
*/

int exp_aritmetica()
{
   int ret;

	CALL(termo);
	CALL(outros_termos);

	return SUCCESS;
}

/*
Automato 29
Autor: Marcos

<termo>                          ::= <fator> <outros_fatores>
*/

int termo()
{
   int ret;

	CALL(fator);
	CALL(outros_fatores);

	return SUCCESS;
}

/*
Automato 30
Autor: Marcos

<outros_termos>                  ::= + <termo> <outros_termos>
								   | - <termo> <outros_termos>
								   | epsilon
*/

int outros_termos()
{
   int ret;

	while(strcmp(tk->string, "+") == SUCCESS || strcmp(tk->string, "-") == SUCCESS)
   {
	   tk = get_token();
	   CALL(termo);
   }
	return SUCCESS;
}

/*
Automato 31
Autor: Marcos

<fator>                          ::= <parcela> <outras_parcelas>
*/

int fator()
{
   int ret;

	CALL(parcela);
	CALL(outras_parcelas);

	return SUCCESS;
}

/*
Automato 32
Autor: Marcos

<outros_fatores>                 ::= * <fator> <outros_fatores>
								   | / <fator> <outros_fatores>
								   | e
*/

int outros_fatores()
{
   int ret;

	while(strcmp(tk->string, "*") ==  SUCCESS || strcmp(tk->string, "/") == SUCCESS)
   {
      tk = get_token();
      CALL(fator);
   }
   return SUCCESS;
}


/*
Automato 33
Autor: Nathan

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

*/

int parcela()
{
   int ret;

   if( strcmp(tk->string, "&") == SUCCESS || tk->class == string )
   {
      if(strcmp(tk->string,"&") == SUCCESS)
      {
          tk = get_token();

          CHECK_CLASS(tk,identifier);
          tk = get_token();

          CALL(outros_ident);
          CALL(dimensao);
      }
      else
      {
          tk = get_token();
      }
   }
   else
   {
      if(strcmp(tk->string,"-") == SUCCESS)
         tk = get_token();

      if(strcmp(tk->string,"^") == SUCCESS)
      {
         tk = get_token();

         CHECK_CLASS(tk, identifier);
         tk = get_token();

         CALL(outros_ident);
         CALL(dimensao);

      }
      else if(tk->class == identifier)
      {
         tk = get_token();
         CALL(chamada_partes);
      }
      else if(tk->class == integer_number)
      {
         tk = get_token();
      }
      else if(tk->class == real_number)
      {
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
   return SUCCESS;
 }


/*
Automato 34
Autor: Nathan

<outras_parcelas> ::= % <parcela> <outras_parcelas> | epsilon

*/

int outras_parcelas()
{
   int ret;
   while(strcmp(tk->string,"%") == SUCCESS)
   {
      tk = get_token();
      CALL(parcela);
   }
   return SUCCESS;
}





/*
Automato 35
Autor: Nathan

<chamada_partes>  ::= ( <expressao> <mais_expressao> )
                    | <outros_ident> <dimensao>
                    | epsilon

*/

int chamada_partes()
{
   int ret;
   if(strcmp(tk->string,"(") == SUCCESS)
   {
      tk = get_token();

      CALL(expressao);
      CALL(mais_expressao);

      CHECK_STRING(tk,")");
      tk = get_token();
   }
   else if(strcmp(tk->string,".") == SUCCESS || strcmp(tk->string,"[") == SUCCESS)
   {
      CALL(outros_ident);
      CALL(dimensao);
   }
   return SUCCESS;
}

/*
Automato 36
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
      }
   }
   return SUCCESS;
}


/*
Automato 37
Autor: Nathan

<expressao>                      ::= <termo_logico> <outros_termos_logicos>

*/
int expressao()
{
   int ret;
   CALL(termo_logico);
   CALL(outros_termos_logicos);

   return SUCCESS;
}


/*
Automato 38
Autor: Nathan

<termo_logico> ::= nao <parcela_logica> <outros_fatores_logicos>
                 | <parcela_logica> <outros_fatores_logicos>
*/

int termo_logico()
{
   int ret;
   if(strcmp(tk->string,"nao") == SUCCESS)
      tk = get_token();

   CALL(parcela_logica);
   CALL(outros_fatores_logicos);

   return SUCCESS;
}



/*
Automato 39
Autor: Nathan

<outros_termos_logicos>   ::= ou <termo_logico> <outros_termos_logicos>  | epsilon

*/

int outros_termos_logicos()
{
   int ret;
   while(strcmp(tk->string,"ou") == SUCCESS)
   {
      tk = get_token();
      CALL(termo_logico);
   }
   return SUCCESS;
}


/*
Automato 40
Autor: Nathan

<outros_fatores_logicos>   ::= e nao <parcela_logica> <outros_fatores_logicos>
                             | e <parcela_logica> <outros_fatores_logicos>
                             | epsilon
*/

int outros_fatores_logicos()
{
   int ret;
   while(strcmp(tk->string,"e") == SUCCESS)
   {
      tk = get_token();

      if(strcmp(tk->string,"nao") == SUCCESS)
         tk = get_token();

      CALL(parcela_logica);
   }
   return SUCCESS;
}
