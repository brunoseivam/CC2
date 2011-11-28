#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* A serem usados na função de busca binária. Essa busca funciona tanto para cadeias quanto para classes de token */
#define STRING 1
#define TK_CLASS 2

/* Abaixo, valores de retorno de algumas funções */

/* Constantes gerais de sucesso e erro */
#define SUCCESS   0
#define ERROR    -1

#define  RECOGNIZED        0     /* Reconhecimento de token */
#define  NOTRECOGNIZED     -1    /* Token não reconhecido */
#define  COMMENTERROR      -2    /* Comentário não fechado */
#define  SYNTAXERROR       -3    /* Erro de sintaxe */

typedef enum token_class
{
   identifier = 1,
   string,
   real_number,
   integer_number,
   keyword,
   symbol,
   eof,                    /* Marcador de fim de arquivo */
   error1,                 /* Caractere não reconhecido  */
   error2                  /* Comentário não fechado     */

}token_class;

typedef struct token
{
   token_class class;   /* Classe do token      */
   char*       string;  /* Cadeia reconhecida   */

}token;

/* Código de struct firsts estritamente C99.
Fonte: http://stackoverflow.com/questions/3875523/lookup-table-in-c */


/* Esta estrutura é usada para armazenar os primeiros de uma regra. Há uma lista de cadeias e uma lista de classes
   esperadas como primeiro do automato. Há também uma lista de outros primeiros de outros automatos a serem consultados.
   Isto serve para o caso:
      primeiros(aut1) = { a, b, primeiros(aut2) }
*/
typedef struct firsts
{
   const char * const * string_list;
   const int            string_list_size;

   const token_class*   tk_class_list;
   const int            tk_class_list_size;

   const struct firsts * const *  other_firsts_list;
   int                            other_firsts_list_size;
}firsts;

/* Abaixo, variáveis acessíveis globalmente, de interesse de todos os módulos */


/* Lista de palavras-chave aceitas pela linguagem. */
extern char* keyword_list[];
extern int   keyword_list_size;

/* Arquivos de entrada e saída */
FILE     *in_file, *out_file;

/* Linha atual */
int      line_number;

/* Último token lido */
token*   tk;

/* Funções que abrem e fecham os arquivos especificados */
int      open_files(char* in, char* out);
void     close_files();

/* Procura palavra-chave(busca binária) na lista */
int      search(void* kw, void* vec, int vec_size, int type);

/* Procura por primeiros em uma estrutura de primeiros */
int      search_first(token* tk, firsts p);

#endif
