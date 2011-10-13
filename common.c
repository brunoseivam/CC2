#include "common.h"

/* Lista de palavas chave da linguagem */
char*    keyword_list[] = { "algoritmo", "ate", "caso", "constante", "declare", "e",
                           "enquanto", "entao", "escreva", "faca", "falso",
                           "fim_algoritmo", "fim_caso", "fim_enquanto", "fim_funcao",
                           "fim_para", "fim_procedimento", "fim_registro","fim_se",
                           "funcao", "inteiro", "leia", "literal", "logico", "nao",
                           "ou", "para", "procedimento", "real", "registro", "retorne",
                           "se", "seja", "senao", "tipo", "var", "verdadeiro" };
int      keyword_list_size = 37;


int   open_files(char* in, char* out)
{
   if( (in_file  = fopen(in,  "r")) == NULL ||
       (out_file = fopen(out, "w")) == NULL)
      return ERROR;

   line_number = 1;
   return SUCCESS;
}

void   close_files()
{
   fclose(in_file);
   fclose(out_file);
}

int   search(void* kw, void* vec, int vec_size, int type)
{
   int min, max, i, res;

   min = 0;
   max = vec_size-1;
   i = (max + min) / 2;

   while(min<=max)
   {
      if(type == STRING)
         res = strcmp((char*) kw, ((char**) vec)[i]);
      else
         res = ((token_class*) vec)[i] - ((token_class) kw);

      if(res < 0)
         max = i-1;
      else if(res > 0)
         min = i+1;
      else
         return SUCCESS;   /* ENCONTROU      */
      i = (max+min)/2;
   }
   return ERROR;         /* NAO ENCONTROU  */
}

int search_first(token* tk, firsts p)
{
   if(!(tk->string == NULL) &&
        search((void*) tk->string, (void*) p.string_list, p.string_list_size, STRING) == SUCCESS)
      return SUCCESS;

   if(search((void* )tk->class, (void*) p.tk_class_list, p.tk_class_list_size, TK_CLASS) == SUCCESS)
      return SUCCESS;

   int i;
   for(i = 0; i < p.other_firsts_list_size; ++i)
      if(search_first(tk, *p.other_firsts_list[i]) == SUCCESS)
         return SUCCESS;
   return ERROR;
}
