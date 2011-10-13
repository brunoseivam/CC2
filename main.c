#include "common.h"
#include "lex.h"
#include "syntax.h"


int main(int argc, char** argv)
{
   if(argc < 3)
   {
      printf("Numero incorreto de parametros. Uso:\n");
      printf("   %s arq_entrada arq_saida\n", argv[0]);
      return 1;
   }
   if(open_files(argv[1], argv[2])!=SUCCESS)
   {
      printf("Erro ao abrir os arquivos %s e %s\n", argv[1], argv[2]);
      return 1;
   }

   /* TESTES */

   token t =
   {
      .class = keyword,
      .string = "algoritmo"
   };

   printf("%d\n", search("faca", keyword_list, keyword_list_size, SEARCH_STRING) == SUCCESS);
   printf("%d\n", search_first(&t, programa_firsts)==SUCCESS);

   /* FIM_TESTES */


   close_files();


   return 0;
}
