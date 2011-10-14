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

/* Autor: Acosta
   primeiros(?) = {?}
*/
const firsts xis =
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
