#include "lex.h"


token* get_token()
{
   token* tk = (token *) malloc(sizeof(token));

   int ret;
   int comment_recognized;
   char c;

   do
   {
      comment_recognized = 0;

      /* Consome espacos em branco e comentarios */
      c = getc(in_file);
      while(ISWHITESPACE(c))
      {
         if(ISLINEFEED(c))
            ++line_number;
         c = getc(in_file);
      }
      if(ISOPENCURLY(c))
      {
         ret = aut_comment(tk);
         if(ret == RECOGNIZED)
            comment_recognized = 1;
         else if(ret == COMMENTERROR)
            return tk;
      }
   }while(comment_recognized);

   /* Identificador ou palavra reservada */
   if(ISLETTER(c) || ISUNDERSCORE(c))
   {
      aut_identifier(tk);
   }
   /* Cadeia literal */
   else if(ISDQUOTE(c))
   {
      ret = aut_string(tk);
      /* Caractere nao reconhecido (aspas nao fechadas) */
      if(ret == NOTRECOGNIZED)
         aut_notrecognized(tk);
   }
   /* Numero inteiro ou real */
   else if(ISALGARISM(c))
   {
       aut_number(tk);
   }
   else if(ISSYMBOL(c))
   {
      fseek(in_file, -1, SEEK_CUR);    /* Devolve-o ao buffer       */
      aut_symbol(tk);
   }
   /* Fim de arquivo */
   else if(ISEOF(c))
   {
      free(tk);
      tk = NULL;
   }
   else                                /* Caractere não reconhecido */
   {
      fseek(in_file, -1, SEEK_CUR);    /* Devolve-o ao buffer       */
      aut_notrecognized(tk);
   }
   return tk;
}

/* Caractere nao reconhecido - prepara estrutura 'token' */
int aut_notrecognized   (token* tk)
{
   tk->string = (char *) malloc(2*sizeof(char));
   tk->class = error1;
   tk->string[0] = getc(in_file);
   tk->string[1] = '\0';

   return RECOGNIZED;
}

int aut_number          (token* tk)
{
   int count = 1;                                /* 1 caractere jah foi lido                 */
   int n_returned;                               /* Numero de caracteres a serem devolvidos  */
   char c;

   /* Algarismos antes do ponto(real) ou inteiro  */
   do
   {
      c = getc(in_file);
      ++count;
   }while(ISALGARISM(c));

   /* PODE ser numero real */
   if(ISPOINT(c))
   {
       c = getc(in_file);
       ++count;
       /* Eh numero real */
       if (ISALGARISM(c))
       {
           do
           {
                c = getc(in_file);
                ++count;
           } while (ISALGARISM(c));

            tk -> class = real_number;
            n_returned = 1;

       }
       else                      /* Numero inteiro antes do ponto          */
        {
            tk -> class = integer_number;
            n_returned = 2;    /* Volta o ultimo caractere lido + o ponto */
       }
   }
   else /* Numero inteiro */
   {
       tk -> class = integer_number;
       n_returned = 1;
   }

    /* Le string do arquivo */
    fseek(in_file, -count, SEEK_CUR);                                   /* Devolve todos os caracteres ao buffer  */
    tk->string  = (char*) malloc((count-n_returned+1)*sizeof(char));    /* Alocar o tamanho da string             */
    fread(tk->string, sizeof(char), count-n_returned, in_file);         /* Le count-(1 ou 2) caracteres           */
    tk->string[count-n_returned] = '\0';

    return RECOGNIZED;
}

int aut_identifier      (token* tk)
{
   int   count = 1;                                /* Conta o numero de caracteres lidos (1 já foi lido)       */
   char  c;                                        /* Ultimo caractere lido                                    */

   do
   {
      c = getc(in_file);
      ++count;
   }while(ISLETTER(c) || ISUNDERSCORE(c) || ISALGARISM(c));


   fseek(in_file, -count, SEEK_CUR);                  /* Devolve todos os caracteres ao buffer                    */
   tk->string  = (char*) malloc(sizeof(char)*count);  /* Alocar o tamanho da string                               */
   fread(tk->string, sizeof(char), count-1, in_file); /* Le count-1 caracteres. O ultimo e' devolvido(invalido)   */
   tk->string[count-1] = '\0';                        /* Finalização da string                                    */

   if(search(tk->string, keyword_list, keyword_list_size, SEARCH_STRING) == SUCCESS)
      tk->class = keyword;
   else
      tk->class = identifier;

   return RECOGNIZED;
}

int aut_comment         (token* tk)
{
   char  c;                                        /* Ultimo caractere lido                                                */

   while((c = getc(in_file)))
   {
      if(ISCLOSECURLY(c))
         return RECOGNIZED;
      else if(ISLINEFEED(c) || ISEOF(c))
      {
         tk->class = error2;                       /* Comentario não fechado                                               */
         tk->string = NULL;
         ++line_number;                            /* Motivo de discórdia. Não deveria ser no reconhecdor de whitespace?   */
         return COMMENTERROR;
      }
   }

   /* nao deve ocorrer esse caso */
   return NOTRECOGNIZED;
}

int aut_string          (token* tk)
{
   char  c;                                        /* Ultimo caractere lido                           */
   int   count = 1;                                /* 1 caractere jah foi lido                        */

   while((c = getc(in_file)))
   {
      ++count;
      if(ISLINEFEED(c) || ISEOF(c))                /* Nova linha lida, string não fechada             */
      {
         fseek(in_file, -count, SEEK_CUR);         /* Devolve todos os caracteres                     */
         return NOTRECOGNIZED;
      }
      else if(ISDQUOTE(c))                         /* Proximas aspas duplas                           */
         break;
   }

   fseek(in_file, -count, SEEK_CUR);                     /* Devolve todos os caracteres ao buffer        */
   tk->string  = (char*) malloc(sizeof(char)*count+1);   /* Alocar o tamanho da string +1 ('\0')         */
   fread(tk->string, sizeof(char), count, in_file);      /* Le count caracteres.                         */
   tk->string[count] = '\0';                             /* Finalização da string                        */
   tk->class = string;

   return RECOGNIZED;
}

int aut_symbol          (token* tk)
{
   char  c1 = getc(in_file);                                      /* Ultimo caractere lido                  */
   char  c2 = getc(in_file);                                      /* Ultimo caractere lido                  */
   int   count = 2;                                               /* 2 caracteres jah foram lidos           */
   int   n_returned = 1;                                          /* Numero de simbolos devolvidos          */

   /* Verifica se o simbolo lido pode ser duplo */
   switch(c1)
   {
      case '<':
         if( c2 == '-' || c2 == '>' || c2 == '=' )
            --n_returned;
         break;

      case '>':
         if( c2 == '=' )
            --n_returned;
         break;

      case '.':
         if( c2 == '.' )
            --n_returned;
         break;
   }

   fseek(in_file, -count, SEEK_CUR);                                 /* Devolve todos os caracteres ao buffer                    */
   tk->string  = (char*) malloc((count-n_returned+1)*sizeof(char));  /* Alocar o tamanho da string                               */
   fread(tk->string, sizeof(char), count-n_returned, in_file);       /* Le count-1 caracteres. O ultimo e' devolvido(invalido)   */
   tk->string[count-n_returned] = '\0';                              /* Finalização da string                                    */
   tk->class = symbol;

   return RECOGNIZED;
}

void print_token        (token* tk)
{
   switch(tk->class)
   {
      case identifier:
         fprintf(out_file, "%s - identificador\n", tk->string);
         break;
      case string:
         fprintf(out_file, "%s - cadeia_literal\n", tk->string);
         break;
      case real_number:
         fprintf(out_file, "%s - numero_real\n", tk->string);
         break;
      case integer_number:
         fprintf(out_file, "%s - numero_inteiro\n", tk->string);
         break;
      case keyword:
      case symbol:
         fprintf(out_file, "%s - %s\n", tk->string, tk->string);
         break;
      case error1:
         fprintf(out_file, "Linha %d: %s - simbolo nao identificado\n", line_number, tk->string);
         break;
      case error2:
         fprintf(out_file, "Linha %d: comentario nao fechado\n", line_number);
         break;
      default:
         fprintf(out_file, "tk->class nao reconhecido! [%d]\n", tk->class);
   }
}

/*
int main(int argc, char** argv)
{
   tk != null (EOF)
   while((tk = get_token()))
   {
      print_token(tk);
      free(tk->string);
      free(tk);
      fflush(stdout);
   }
   fclose(in_file);
   fclose(out_file);

   return 0;
}*/

