#ifndef LEX_H
#define LEX_H

#include "common.h"

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



/* Automatos usados */
int      aut_identifier    (token* tk);
int      aut_comment       (token* tk);
int      aut_string        (token* tk);
int      aut_notrecognized (token* tk);
int      aut_number        (token* tk);
int      aut_symbol        (token* tk);

/* Chamado pelo programa principal */
token*   get_token();




#endif
