#include "common.h"
#include "lex.h"
#include "syntax.h"

void handle_error(int error)
{
   printf("handle error called %d\n", error);
   return;
}
int main(int argc, char** argv)
{
   int ret;

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

   tk = get_token();

   ret = programa();
   if(ret != SUCCESS)
      handle_error(ret);

   close_files();


   return 0;
}
