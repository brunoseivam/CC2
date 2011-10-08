#ifndef LEX_H
#define LEX_H


#define  ISLETTER(c)       ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
#define  ISALGARISM(c)     (c >= '0' && c <= '9')
#define  ISUNDERSCORE(c)   (c == '_')
#define  ISWHITESPACE(c)   (c == ' ' || c == '\t' || c == '\r' || c == '\n')
#define  ISLINEFEED(c)     (c == '\n')
#define  ISOPENCURLY(c)    (c == '{' )
#define  ISCLOSECURLY(c)   (c == '}' )
#define  ISDQUOTE(c)       (c == '"' )
#define  ISPOINT(c)        (c == '.')

/* Simbolos que só podem ser únicos */
#define  ISSYMBOL(c)       (c == ':' || c == ',' || c == '^' || c == '(' || c == ')' ||\
                            c == '-' || c == '%' || c == '*' || c == '/' || c == '+' ||\
                            c == '&' || c == '=' || c == '[' || c == ']' || c == '<' ||\
                            c == '>' || c == '.')

#define  ISEOF(c)          (c == EOF)

#define  RECOGNIZED        0
#define  NOTRECOGNIZED     -1
#define  COMMENTERROR      -2

FILE  *in_file, *out_file;
int   line_number;

typedef enum token_class
{
   identifier,
   string,
   real_number,
   integer_number,
   keyword,
   symbol,
   error1,                 /* Caractere não reconhecido  */
   error2                  /* Comentário não fechado     */
}token_class;

typedef struct token
{
   token_class class;
   char*       string;

}token;

char*   keyword_list[] = {"algoritmo", "ate", "caso", "constante", "declare", "e",
"enquanto", "entao", "escreva", "faca", "falso", "fim_algoritmo", "fim_caso",
"fim_enquanto", "fim_funcao", "fim_para", "fim_procedimento", "fim_registro",
"fim_se", "funcao", "inteiro", "leia", "literal", "logico", "nao", "ou", "para",
"procedimento", "real", "registro", "retorne", "se", "seja", "senao", "tipo", "var",
"verdadeiro"};
int      keyword_list_size = 37;

/* Automatos usados */
int      aut_identifier    (token* tk);
int      aut_comment       (token* tk);
int      aut_string        (token* tk);
int      aut_notrecognized (token* tk);
int      aut_number        (token* tk);
int      aut_symbol        (token* tk);

/* Chamado pelo programa principal */
token*   get_token();

/* Procura palavra-chave(busca binária) na lista */
int      search_keyword(char* kw);


#endif
