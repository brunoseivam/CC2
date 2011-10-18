#include "common.h"
#include "lex.h"
#include "syntax.h"

/* Trata erro.
     Primeiro checa o token caso ele seja ERROR1 ou ERROR2. Se não for nenhum dos dois erros léxicos, é um erro sintático
*/
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

   /* Checa os argumentos passados */
   if(argc < 3)
   {
      printf("Numero incorreto de parametros. Uso:\n");
      printf("   %s arq_entrada arq_saida\n", argv[0]);
      return 1;
   }

   /* Abre os arquivos de entrada e saida */
   if(open_files(argv[1], argv[2])!=SUCCESS)
   {
      printf("Erro ao abrir os arquivos %s e %s\n", argv[1], argv[2]);
      return 1;
   }

   /* Pega o primeiro token */
   tk = get_token();

   if((ret = programa()) != SUCCESS)
      handle_error(ret);               /* Trata o erro, caso exista */

   fprintf(out_file, "Fim da compilacao\n");
   close_files();                      /* Fecha os arquivos abertos */


   return 0;
}
