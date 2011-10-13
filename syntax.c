#include <stdlib.h>
#include <stdio.h>

#include "syntax.h"

/* Declaração de todos os firsts estritamente C99.
Fonte: http://stackoverflow.com/questions/3875523/lookup-table-in-c */
const firsts programa_firsts =
{
   .string_list      = (const char * const []){"algoritmo"},
   .string_list_size = 1,

   /*.tk_class_list    = (const token_class []){real_number, integer_number},
   .tk_class_list_size = 2*/

   .tk_class_list    = NULL,
   .tk_class_list_size = 0,

   .other_firsts_list = (const firsts * const []){ &declaracao_global_firsts, &declaracao_local_firsts },
   .other_firsts_list_size = 2
};

int programa()
{
   return 0;
}
