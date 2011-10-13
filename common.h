#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define SUCCESS   0
#define ERROR     -1

#define STRING 1
#define TK_CLASS 2

#define  RECOGNIZED        0
#define  NOTRECOGNIZED     -1
#define  COMMENTERROR      -2
#define  SYNTAXERROR       -3

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
   token_class class;
   char*       string;

}token;

/* Código de struct firsts estritamente C99.
Fonte: http://stackoverflow.com/questions/3875523/lookup-table-in-c */
typedef struct firsts
{
   const char * const * string_list;
   const int            string_list_size;

   const token_class*   tk_class_list;
   const int            tk_class_list_size;

   const struct firsts * const *  other_firsts_list;
   int                            other_firsts_list_size;
}firsts;



extern char* keyword_list[];
extern int   keyword_list_size;

FILE     *in_file, *out_file;
int      line_number;
token*   tk;


int      open_files(char* in, char* out);
void     close_files();

/* Procura palavra-chave(busca binária) na lista */
int      search(void* kw, void* vec, int vec_size, int type);
int      search_first(token* tk, firsts p);

#endif
