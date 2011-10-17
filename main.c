#include "common.h"
#include "lex.h"
#include "syntax.h"

void handle_error()
{
   switch(tk->class)
	{
		case error1:
			fprintf(out_file, "Linha %d: %s - simbolo nao identificado\n",
					line_number, tk->string);
			break;

		case error2:
			fprintf(out_file, "Linha %d: comentario nao fechado\n",
					line_number);
			break;

		default:
			fprintf(out_file, "Linha %d: erro sintatico proximo a %s\n",
					line_number, tk->string);
			break;
	}
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

   if((ret = programa()) != SUCCESS)
      handle_error(ret);

   fprintf(out_file, "Fim da compilacao\n"); /* Imprimir mensagem de Fim da compilacao aqui? */
   close_files();


   return 0;
}
