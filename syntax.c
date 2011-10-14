#include <stdlib.h>
#include <stdio.h>

#include "syntax.h"

/* Declaração de todos os firsts estritamente C99.
Fonte: http://stackoverflow.com/questions/3875523/lookup-table-in-c */
const firsts programa_firsts =
{
   .string_list      = (const char * const []){"algoritmo"},
   .string_list_size = 1,

   /*.tk_class_list    = (const token_class []){real_number, integer_number},
   .tk_class_list_size = 2*/

   .tk_class_list    = NULL,
   .tk_class_list_size = 0,

   .other_firsts_list = (const firsts * const []){ &declaracao_global_firsts, &declaracao_local_firsts },
   .other_firsts_list_size = 2
};

/* Autor: Bruno
primeiros(declaracao_local) = { declare, constante, tipo }
*/

const firsts declaracao_local_firsts =
{
   .string_list      = (const char * const []){"delcare", "constante", "tipo"},
   .string_list_size = 3,

   .tk_class_list    = NULL,
   .tk_class_list_size = 0,

   .other_firsts_list = NULL,
   .other_firsts_list_size = 0
};


/* Autor: Bruno
primeiros(variavel) = primeiros(identificador);
*/
const firsts variavel_firsts =
{
   .string_list = NULL,
   .string_list_size = 0,

   .tk_class_list = NULL,
   .tk_class_list_size = 0,

   .other_firsts_list = (const firsts * const []) { &identificador_firsts },
   .other_firsts_list_size = 1
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

/*
Automato 1
Autor: Bruno

<programa>           ::= <declaracoes> algoritmo <declaracoes_locais> <comandos> fim_algoritmo
<declaracoes>        ::= <declaracao_local> <declaracoes>
                       | <declaracao_global> <declaracoes>
                       | ε
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

<outros_ident>                 ::= . IDENT <outros_ident> | ε
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

<dimensao>                     ::= [ <exp_aritmetica> ] <dimensao>| ε
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
<mais_variaveis>                    ::= <variavel> <mais_variaveis> | ε
*/
int tipo()
{
   int ret;
   if( strcmp(tk->string, "registro") == SUCCESS)
   {
      tk = get_token();

      CALL(variavel);

      while(search_first(tk, variavel_firsts) == SUCCESS)
         CALL(variavel);

      CHECK_STRING(tk, "fim_registro");
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

<mais_ident>                   ::= , <identificador> <mais_ident> | ε
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



/* DUMMY */
int declaracao_global()
{
   return 0;
}

int declaracoes_locais()
{
   return 0;
}
int comandos()
{
   return 0;
}
int tipo_basico()
{
   return 0;
}
int tipo_estendido()
{
   return 0;
}
int exp_aritmetica()
{
   return 0;
}

/*
Automato 17
Autor: Acosta

<cmd_se> ::= se <expressao> entao <comandos> <senao_opcional> fim_se
<senao_opcional> ::= senao <comandos> | ε
*/

int cmd_se()
{
	int ret;

	CHECK_STRING(tk, "se");
	tk = get_token();

	CALL(expressao);

	CHECK_STRING(tk, "entao");
	tk = get_token();

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
Autor: Acosta

<cmd_caso> ::= caso <exp_aritmetica> seja <selecao> <senao_opcional> fim_caso
<senao_opcional> ::= senao <comandos> | ε
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
Autor: Acosta

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
Autor: Acosta

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
Autor: Acosta

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
Autor: Acosta

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
Autor: Acosta

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
Autor: Acosta

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

/* DUMMY: TODO automata */
int expressao()
{
	return SUCCESS;
}

int selecao()
{
	return SUCCESS;
}

int mais_expressao()
{
	return SUCCESS;
}
