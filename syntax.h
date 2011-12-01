#ifndef SYNTAX_H
#define SYNTAX_H

#include "common.h"
#include "lex.h"
#include "semant.h"

/* As macros abaixo servem para trechos comuns do código, que se repetiriam muitas vezes.
   Além disso, elas provém certa elegância no código, pois, por exemplo, é mais significativo ler

   CALL(automata);

   do que

   if( ret = automata() != SUCCESS) return ret;
*/


/* Esta macro chama o autômato passado como argumento. Caso o autômato encontre erro, interrompe-se o
   fluxo de execução retornando-se o erro encontrado

   Obs.: int ret; deve estar declarada na função que chama esta macro
   */

#define  CALL(automata)          if( (ret = automata() ) != SUCCESS ) return ret

/* Esta macro checa se o token passado contém a string esperada. Caso negativo, retorna com SYNTAXERROR */
#define  CHECK_STRING(tk, str)   if( strcmp(tk->string, str) != SUCCESS ) return SYNTAXERROR

/* Esta macro checa se o token passado contém a classe esperada. Caso negativo, retorna com SYNTAXERROR */
#define  CHECK_CLASS(tk, cls)    if( tk->class != cls ) return SYNTAXERROR

/* Esta macro não foi implementada como função por uma questão de desempenho. Como um compilador usa e abusa
   de pilhas de funções, procuramos como regra geral evitar ao máximo chamadas de funções.

   Esta macro checa se o token passado contém uma das strings passadas. Se não contiver, retorna SYNTAXERROR
   Por exemplo, CHECK_STRINGS(tk, "a", "b") vai retornar erro caso tk->string não seja "a" NEM "b"

   O loop do{}while(0) serve apenas para se poder agrupar várias instruções em uma macro.
*/


#define  CHECK_STRINGS(tk, ...)  do{                                                      \
                                    char* vec[] = {__VA_ARGS__};                          \
                                    int count = sizeof(vec)/sizeof(char*);                \
                                    while(count)                                          \
                                    {                                                     \
                                       if( strcmp(tk->string, vec[count-1]) == SUCCESS )  \
                                            break;                                        \
                                       --count;                                           \
                                    }                                                     \
                                    if(!count) return SYNTAXERROR;                        \
                                 }while(0)

/* Faz o mesmo que acima, mas com tk->class */
#define  CHECK_CLASSES(tk, ...)  do{                                                      \
                                    token_class vec[] = {__VA_ARGS__};                    \
                                    int count = sizeof(vec)/sizeof(token_class);          \
                                    while(count)                                          \
                                    {                                                     \
                                       if( tk->class == vec[count-1] )                    \
                                            break;                                        \
                                       --count;                                           \
                                    }                                                     \
                                    if(!count) return SYNTAXERROR;                        \
                                 }while(0)

#define  CHECK_SEM(func, code)   do{                                                      \
                                    if(func != BTREE_SUCCESS)                                   \
                                    {                                                     \
                                       sem_error(code);                                   \
                                    }                                                     \
                                 }while(0)

/* Protótipos */

/* Bruno */
const firsts programa_firsts;
const firsts declaracao_local_firsts;
const firsts variavel_firsts;
const firsts identificador_firsts;
const firsts outros_ident_firsts;
const firsts dimensao_firsts;
const firsts tipo_firsts;
const firsts mais_ident_firsts;

/* Talita */
const firsts tipo_basico_firsts;
const firsts tipo_estendido_firsts;
const firsts declaracao_global_firsts;
const firsts parametro_firsts;
const firsts declaracoes_locais_firsts;
const firsts comandos_firsts;
const firsts cmd_leia_firsts;
const firsts cmd_escreva_firsts;

/* Lucas */
const firsts cmd_se_firsts;
const firsts cmd_caso_firsts;
const firsts cmd_para_firsts;
const firsts cmd_enquanto_firsts;
const firsts cmd_faca_firsts;
const firsts cmd_pont_ident_firsts;
const firsts cmd_ident_firsts;
const firsts cmd_retorne_firsts;

/* Marcos */
const firsts mais_expressao_firsts;
const firsts selecao_firsts;
const firsts constantes_firsts;
const firsts exp_aritmetica_firsts;
const firsts termo_firsts;
const firsts outros_termos_firsts;
const firsts fator_firsts;
const firsts outros_fatores_firsts;

/* Nathan */
const firsts parcela_firsts;
const firsts outras_parcelas_firsts;
const firsts chamada_partes_firsts;
const firsts parcela_logica_firsts;
const firsts expressao_firsts;
const firsts termo_logico_firsts;
const firsts outros_termos_logicos_firsts;
const firsts outros_fatores_logicos_firsts;

/* Bruno */
int programa();
int declaracao_local();
int variavel();
int identificador();
int outros_ident();
int dimensao();
int tipo();
int mais_ident();

/* Talita */
int tipo_basico();
int tipo_estendido();
int declaracao_global();
int parametro();
int declaracoes_locais();
int comandos();
int cmd_leia();
int cmd_escreva();

/* Lucas */
int cmd_se();
int cmd_caso();
int cmd_para();
int cmd_enquanto();
int cmd_faca();
int cmd_pont_ident();
int cmd_ident();
int cmd_retorne();

/* Marcos */
int mais_expressao();
int selecao();
int constantes();
int exp_aritmetica();
int termo();
int outros_termos();
int fator();
int outros_fatores();

/* Nathan */
int parcela();
int outras_parcelas();
int chamada_partes();
int parcela_logica();
int expressao();
int termo_logico();
int outros_termos_logicos();
int outros_fatores_logicos();

#endif
