#ifndef SYNTAX_H
#define SYNTAX_H

#include "common.h"
#include "lex.h"

#define  CALL(automata)          if( (ret = automata() ) != SUCCESS ) return ret
#define  CHECK_STRING(tk, str)   if( !tk->string || (strcmp(tk->string, str) != SUCCESS) ) return SYNTAXERROR
#define  CHECK_CLASS(tk, cls)    if( tk->class != cls ) return SYNTAXERROR

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


/* Bruno */
const firsts programa_firsts;          /* Não utilizado */
const firsts declaracao_local_firsts;  /* Não utilizado */
const firsts variavel_firsts;
const firsts identificador_firsts;
const firsts outros_ident_firsts;      /* Não utilizado */
const firsts dimensao_firsts;          /* Não utilizado */
const firsts tipo_firsts;              /* Não utilizado */
const firsts mais_ident_firsts;        /* Não utilizado */

const firsts tipo_basico_firsts;
const firsts tipo_estendido_firsts;
const firsts declaracao_global_firsts;
const firsts parametro_firsts;
const firsts declaracoes_locais_firsts;
const firsts comandos_firsts;
const firsts cmd_leia_firsts;
const firsts cmd_escreva_firsts;

/* Acosta */
const firsts cmd_se_firsts; /* Não Usado */
const firsts cmd_caso_firsts; /* Não Usado */
const firsts cmd_para_firsts; /* Não Usado */
const firsts cmd_enquanto_firsts; /* Não Usado */
const firsts cmd_faca_firsts; /* Não Usado */
const firsts cmd_pont_ident_firsts; /* Não Usado */
const firsts cmd_ident_firsts; /* Não Usado */
const firsts cmd_retorne_firsts; /* Não Usado */


const firsts mais_expressao_firsts;
const firsts selecao_firsts;
const firsts constantes_firsts;
const firsts exp_aritmetica_firsts;
const firsts termo_firsts;
const firsts outros_termos_firsts;
const firsts fator_firsts;
const firsts outros_fatores_firsts;
const firsts parcela_firsts;
const firsts outras_parcelas_firsts;
const firsts chamada_partes_firsts;
const firsts parcela_logica_firsts;
const firsts expressao_firsts;
const firsts termo_logico_firsts;
const firsts outros_termos_logicos_firsts;
const firsts outros_fatores_logicos_firsts;

int programa();
int declaracao_local();
int variavel();
int identificador();
int outros_ident();
int dimensao();
int tipo();
int mais_ident();
int tipo_basico();
int tipo_estendido();
int declaracao_global();
int parametro();
int declaracoes_locais();
int comandos();
int cmd_leia();
int cmd_escreva();
int cmd_se();
int cmd_caso();
int cmd_para();
int cmd_enquanto();
int cmd_faca();
int cmd_pont_ident();
int cmd_ident();
int cmd_retorne();
int mais_expressao();
int selecao();
int constantes();
int exp_aritmetica();
int termo();
int outros_termos();
int fator();
int outros_fatores();
int parcela();
int outras_parcelas();
int chamada_partes();
int parcela_logica();
int expressao();
int termo_logico();
int outros_termos_logicos();
int outros_fatores_logicos();

#endif
